// SPDX-License-Identifier: MIT
#include "hss_thread.h"

#include <pthread.h>
#include <string.h>

/*
 * This is an implementation of our threaded abstraction using the
 * POSIX pthread API
 *
 * C11 has a similar (but not precisely identical) API to the one that POSIX
 * defines (at least for what we do; all we need is thread create/join and
 * mutex's, which *any* thread library should provide).  I'd code up the
 * support for that API as well (using the same base logic, with typedef's and
 * helper inlines to isolate the differences), however I don't have a C11
 * implementation handy to test it
 */

#define MAX_THREAD 16   /* Number try to create more than 16 threads, no */
                        /* matter what the application tries to tell us */
#define DEFAULT_THREAD 16 /* The number of threads to run if the */
                        /* application doesn't tell us otherwise (e.g. */
                        /* passes in 0) */

#define MIN_DETAIL 16   /* So the alignment kludge we do doesn't waste space */

/* The information we track about a thread we may have launched */
struct thread_state {
    pthread_t thread_id;
    enum { never_was, alive, dead } state;
};

struct work_item {
    struct work_item *link;    /* They're in a linked list */

    void (*function)(const void *detail,   /* Function to call */
                             struct thread_collection *col);

        /* These two items are used to pass the thread state to the thread */
        /* if this is the first work item for the thread to process */
    struct thread_collection *col; /* The parent thread_collection */
    struct thread_state *state; /* The pointer into the thread collection */
                               /* state for the state of this thread */

       /* The detail structure that we pass to the function */
       /* We'll malloc enough space to hold the entire structure */
    union {                    /* union here so that the detail array is */
        void *align1;          /* correctly aligned for various datatypes */
        long long align2;
        void (*align3)(void);
        unsigned char detail[MIN_DETAIL];
    } x;
};

struct thread_collection {
    pthread_mutex_t lock;       /* Must be locked before this structure is */
                                /* accessed if there might be a thread */
    pthread_mutex_t write_lock; /* Must be locked before common user data is */
                                /* written */

    unsigned num_thread;
    unsigned current_ptr;       /* There two are here to avoid O(N) table */
    unsigned num_alive;         /* scanning in the most common scenarios */

        /* Information about the worker threads we may have created */
    struct thread_state threads[MAX_THREAD];

        /*
         * Queue (FIFO) of work items submitted, and which can't be processed
         * immedately.  We do a FIFO, rather than a stack, so that we perform
         * the requests in the order they were issued (which isn't something
         * the interface guarantees; however it doesn't interfere with the
         * request ordering we ask applications to make)
         */
    struct work_item *top_work_queue;
    struct work_item *end_work_queue;
};

/*
 * Allocate a thread control structure
 */
struct thread_collection *hss_thread_init(int num_thread) {
    if (num_thread == 0) num_thread = DEFAULT_THREAD;
    if (num_thread <= 1) return 0;  /* Not an error: an indication to run */
                                    /* single threaded */
    if (num_thread > MAX_THREAD) num_thread = MAX_THREAD;

    struct thread_collection *col = OQS_MEM_malloc( sizeof *col );
    if (!col) return 0;  /* On malloc failure, run single threaded */

    col->num_thread = num_thread;

    if (0 != pthread_mutex_init( &col->lock, 0 )) {
        OQS_MEM_insecure_free(col);
        return 0;
    }

    if (0 != pthread_mutex_init( &col->write_lock, 0 )) {
        pthread_mutex_destroy( &col->lock );
        OQS_MEM_insecure_free(col);
        return 0;
    }

    col->current_ptr = 0;
    col->num_alive = 0;
    int i;
    for (i=0; i<num_thread; i++) {
        col->threads[i].state = never_was;
    }
    col->top_work_queue = 0;
    col->end_work_queue = 0;

    return col;
}

/*
 * This is the base routine that a worker thread runs
 */
static void *worker_thread( void *arg ) {
    struct work_item *w = arg;  /* The initial work item */
    struct thread_collection *col = w->col;
    struct thread_state *state = w->state;

    for (;;) {
        /* Perform the work item in front of us */
        (w->function)(w->x.detail, col);

        /* Ok, we did that */
        OQS_MEM_insecure_free(w);

        /* Check if there's anything else to do */
        pthread_mutex_lock( &col->lock );

        w = col->top_work_queue;
        if (w) {
            /* More work; pull it off the queue */
            col->top_work_queue = w->link;
            if (w == col->end_work_queue) col->end_work_queue = 0;

            /* And go handle it */
            pthread_mutex_unlock( &col->lock );
            continue;
        }

        /* No more work for us to do; post our obituary */
        state->state = dead;
        col->num_alive -= 1;
        pthread_mutex_unlock( &col->lock );

        /* And that's all folks */
        return 0;
    }
}

/*
 * This adds function/details to the list of things that need to be done
 * It either creates a thread to do it, or (if we're maxed out) add it to
 * our honey-do list (or, as last resort, just does it itself)
 */
void hss_thread_issue_work(struct thread_collection *col,
            void (*function)(const void *detail,
                             struct thread_collection *col),
            const void *detail, size_t size_detail_structure) {

    /* If we're running in single-threaded mode */
    if (!col) {
        function( detail, col );
        return;
    }

    /* Allocate a work structure to hold this request */
    size_t extra_space;
    if (size_detail_structure < MIN_DETAIL) extra_space = 0;
    else extra_space = size_detail_structure - MIN_DETAIL;
    struct work_item *w = OQS_MEM_malloc(sizeof *w + extra_space);

    if (!w) {
        /* Can't allocate the work structure; fall back to single-threaded */
        function( detail, col );
        return;
    }
    w->col = col;
    w->function = function;
    memcpy( w->x.detail, detail, size_detail_structure );

    unsigned num_thread = col->num_thread;

    pthread_mutex_lock( &col->lock );

    /* Check if we can spawn a new thread */
    if (col->num_alive < num_thread) {
        /* There's supposed to be room for another */
        /* Look for the empty slot */
        unsigned i, j;
        j = col->current_ptr;  /* Do round-robin (so we don't bang on */
                               /* slot 0 whenever we try to start a thread) */
        for (i=0; i<num_thread; i++, j = (j+1) % num_thread) {
            struct thread_state *p = &col->threads[j];
            switch (p->state) {
            case alive: continue; /* This one's busy */
            case dead:
                {
                /* This one just died; grab its status (not that we care, */
                /* however that'll tell the thread library it can clean up) */
                pthread_t thread_id = p->thread_id;
                void *status;   /* Ignored, but we need to place thread */
                                /* status somewhere */
                pthread_mutex_unlock( &col->lock );
                pthread_join( thread_id, &status );
                pthread_mutex_lock( &col->lock );
                p->state = never_was;
                }
                /* FALL THROUGH */
            case never_was:
                /* Now, we can spawn a new thread */
                w->state = p;
                if (0 != pthread_create( &p->thread_id,
                                         NULL, worker_thread, w )) {
                    /* Hmmm, couldn't spawn it; fall back */
                    default: /* On error condition */
                    pthread_mutex_unlock( &col->lock );
                    OQS_MEM_insecure_free(w);
                    function( detail, col );
                    return;
                }

                /* We've kicked off the thread */
                p->state = alive;
                col->num_alive += 1;
                    /* For the next request, start scanning at the next */
                    /* thread object */
                col->current_ptr = (j+1) % num_thread;
                pthread_mutex_unlock( &col->lock );
                return;
            }
        }
        col->num_alive = num_thread; /* Hmmmm, everything was alive??? */
    }

    /* We can't create any more threads; enqueue this (and someone will get */
    /* to it) */
    w->link = 0;
    if (col->end_work_queue) {
        col->end_work_queue->link = w;
    }
    col->end_work_queue = w;
    if (!col->top_work_queue) col->top_work_queue = w;

    pthread_mutex_unlock( &col->lock );
}

/*
 * This will wait for all the work items we'e issued to complete
 */
void hss_thread_done(struct thread_collection *col) {
    if (!col) return;

    unsigned i;
    pthread_mutex_lock( &col->lock );
    for (i=0; i<col->num_thread; i++) {
        /*
         * Wait for each thread that we have spawned.
         * We're the only one that will spawn them, and so we don't have to
         * worry about any new ones appearing while we scan through the list
         */
        if (col->threads[i].state != never_was) {
            void *status;
            pthread_t thread_id = col->threads[i].thread_id;
            pthread_mutex_unlock( &col->lock );
            pthread_join( thread_id, &status );
            pthread_mutex_lock( &col->lock );
        }
    }
    pthread_mutex_unlock( &col->lock );

    /* Ok, all the threads have finished; tear things down */

    pthread_mutex_destroy( &col->lock );
    pthread_mutex_destroy( &col->write_lock );
    OQS_MEM_insecure_free(col);
}

void hss_thread_before_write(struct thread_collection *col) {
    if (!col) return;
    pthread_mutex_lock( &col->write_lock );
}

void hss_thread_after_write(struct thread_collection *col) {
    if (!col) return;
    pthread_mutex_unlock( &col->write_lock );
}


unsigned hss_thread_num_tracks(int num_thread) {
    if (num_thread == 0) num_thread = DEFAULT_THREAD;
    if (num_thread <= 1) return 1;
    if (num_thread >= MAX_THREAD) return MAX_THREAD;
    return num_thread;
}
