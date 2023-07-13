#if !defined( HSS_THREAD_H_ )
#define HSS_THREAD_H_
/*
 * This is our internal abstraction of multithreading; this allows the
 * "application" (in this case, the HSS code) to issue multiple requests that
 * can potentially run on different threads, in a way that doesn't depend on
 * the actual threading capability of the OS.  If we don't actually have
 * multiple threads avaiable (either because the OS doesn't provide us with
 * multiple threads, or we hit an internal error trying to generate new
 * threads), this will just have the main thread do all the work (and hence
 * the application doesn't have to worry its pretty little head about error
 * handling, or whether we actually implement threads in the first place)
 *
 * This is designed to handle this sort of task: we have a series of
 * computational problems to do; each can be done independently of the others,
 * and each problem results in a fairly short answer.  All the children do is
 * computation; there's no I/O or any other interaction with the OS at all.
 *
 * The general paradigm is:
 * - The main thread generates a thread collection (via the hss_thread_init
 *   call)
 * - The main thread then issues a series of tasks (via the
 *   hss_thread_issue_work call).  This may spawn off other threads (which
 *   will then call the function passed); alternatively, the main thread may
 *   call the function.
 * - The main thread then waits for all the tasks to be done (via the
 *   hss_thread_done call)
 * The function(s) passed to the hss_thread_issue_work call will be completed
 * by the time hss_thread_done returns
 */
#include <stdlib.h>

/* This is our abstract object that stands for a set of threads */
struct thread_collection;

/*
 * This is called to initialize a set of threads, and returns the identifier.
 * Note that this cannot fail; if it returns 0, it's not a failure; instead,
 * it's a valid return (which essentially means we're running in nonthreaded
 * mode)
 * The integer passed is a recommendation on the number of threads
 */
struct thread_collection *hss_thread_init(int);

/*
 * This issues another work item to our collection of threads.  At some point
 * (between when hss_thread_issue_work is called and when hss_thread_done
 * returns), we'll have function called, with a pointer to a copy of the detail
 * structure.  function may be called by this thread, or it may be called by a
 * different one.
 *
 * The passed detail structure will not be referenced after this returns, and
 * hence it is safe if the caller modifies (or frees) it afterwards.  If the
 * function isn't completed by the time hss_thread_issue_work returns, we'll
 * squirrel away a copy of detail (which is why we ask the caller to
 * pass size_detail_structure; so we know how much to copy)
 *
 * We suggest that the application issue the work orders in largest-to-smallest
 * order.  The ordering doesn't matter for correctness (the API makes no
 * guarrantees about when the requests will be completed), however we suggest
 * this for expected performance reasons.  hss_thread_done will not return
 * until all threads are done; what we want to avoid is scenarios where all but
 * one of the threads are done, and that last thread is working on an expensive
 * function; that would slow things down, and the entire point of this thread
 * library is to speed things up.  Assigning work items to threads optimally is
 * an NP-hard problem, however the simple heuristic of packing 'largest first'
 * works fairly well in practice (and is easy to implement). The thread library
 * does try to make a best effort attempt to preserve the issue order (assuming
 * no intermediate malloc or thread spawn issues; in those cases, the library
 * prioritizes correctness over efficiency)
 */
void hss_thread_issue_work(struct thread_collection *col,
            void (*function)(const void *detail,
                             struct thread_collection *col),
            const void *detail, size_t size_detail_structure);

/*
 * This waits for all the work items we have issued (via hss_thread_issue_work)
 * to be completed (that is, 'function' has returned, and cleans up the
 * collection
 *
 * col must not be used after this; if it was malloc'ed, this will free it
 */
void hss_thread_done(struct thread_collection *col);

/*
 * This should be called before a thread writes to common data
 *
 * We do this because we sometimes have different threads write data to
 * adjacent memory locations; if the compiler has the CPU do a
 * read/modify/write to the entire word (or however the CPU has memory
 * organized), this could cause a race condition.  Forcing those writes to be
 * serialized avoids the issue; such a race condition would actually be fairly
 * unlikely, but would be a *really* difficult bug to track down if it did
 * occur, so it makes sense to go the extra mile to avoid the possibility
 *
 * Doing this locking also means that the working thread can safely do things
 * such as incrementing a global [1] counter to report its results, should
 * that be appropriate
 *
 * We don't bother doing this if we're writing into a malloc'ed region, *if*
 * we're the only thread that will be writing into that specific region; we
 * assume that the malloc infrastructure will separate distinct malloc'ed
 * regions enough to avoid such race conditions
 *
 * [1] actually, automatic to the main thread; there are no literal globals
 *     in this package, apart from the verbose debugging flag
 */
void hss_thread_before_write(struct thread_collection *collect);

/*
 * This should be called after a thread writes to common data; it releases
 * the lock
 */
void hss_thread_after_write(struct thread_collection *collect);

/*
 * This gives the application guidance for how many worker threads we have
 * available, that is, how many work items we can expect to run at once
 *
 * This is used to decide the level of granularity we need; we we have only 2
 * cores, there's no point is splitting the job up to 50 separate requests;
 * however if there are 100 cores, we want (if possible) to do at least 100
 *
 * The issue with having not enough requests is that we will have idle threads
 * (which could potentially do useful work, if we are able to divide the work
 * further).  The issue with having too many requests is that the requests use
 * up some memory, and we'd prefer not to use up too much memory (we don't
 * fail on malloc failure, however we do drop back to a single threaded model)
 *
 * The value passed is the value we'll pass to hss_thread_init
 */
unsigned hss_thread_num_tracks(int num_threads);

#endif /* HSS_THREAD_H_ */
