// SPDX-License-Identifier: MIT
#include "hss_thread.h"
#include "config.h"

/*
 * This is a trivial implementation of our threading abstraction.
 * It's used if we don't have any threading support
 */

/*
 * This requests that an object that tracks the threads be created.  We have
 * no threads, hence we don't need such an object
 */
struct thread_collection *hss_thread_init(int num_thread) {
    LMS_UNUSED(num_thread);
    return 0;
}

/*
 * This asks that function be called sometime between now, and when
 * hss_thread_done is called.  We just go ahead, and do it now
 */
void hss_thread_issue_work(struct thread_collection *collect,
            void (*function)(const void *detail,
                             struct thread_collection *col),
            const void *detail, size_t size_detail_structure) {
    LMS_UNUSED(size_detail_structure);
        /* If we were asked to make sure something is done, just do it */
    function( detail, collect );
}

/*
 * This asks for all the work requests we've issued to completed, and that
 * the collection object be freed.  We did all the work when it was
 * requested, and we never allocated a collection object in the first place
 */
void hss_thread_done(struct thread_collection *collect) {
    LMS_UNUSED(collect);
}

/*
 * A thread calls this when it will write into a common area (so that no
 * other thread will access it at the same time).  No threads means that
 * there is no need to lock
 */
void hss_thread_before_write(struct thread_collection *collect) {
    LMS_UNUSED(collect);
}

/*
 * This releases the above lock
 */
void hss_thread_after_write(struct thread_collection *collect) {
    LMS_UNUSED(collect);
}

/*
 * This tells the application that we really have only one thread
 * (the main one)
 */
unsigned hss_thread_num_tracks(int num_thread) {
    LMS_UNUSED(num_thread);
    return 1;
}
