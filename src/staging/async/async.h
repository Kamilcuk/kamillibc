/*
 * async.h
 *
 *  Created on: 17 lip 2018
 *      Author: Kamil Cukrowski
 *     License: Jointly under MIT License and Beerware License
 */

#ifndef ASYNC_ASYNC_H_
#define ASYNC_ASYNC_H_

#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>

typedef void *(*async_funcptr_t)();

typedef struct async_s async_t;

struct async_s {
	// created thread
	pthread_t thread;

	struct {
		// mutex protecting this struct
		pthread_mutex_t mutex;
		// Becomes the value of f() returned after readyfd is ready
		void *returned;
		// allocated file descriptor of the process shared with thread
		// stays open until thread is executing
		int readyfd;
		// Means that calling thread called async_detach or async_cancel
		// and our thread should deallocate memory itself;
		bool do_free;
	};

	// caller function to be called
	union {
		async_funcptr_t f;
		void *(*f_voidptr)(void *);
		void *(*f_asyncarr)(async_t *asyncs[], size_t asyncs_cnt);
	};
	// arguments passed to function f
	union {
		void *arg;
		struct {
			async_t **asyncs;
			size_t asyncs_cnt;
		};
	};
};

/* Exported Functions ---------------------------------------------------------------- */
/**
 * @defgroup Constructors
 * @{
 */

/**
 * Create new async object. Run function f with argument arg.
 * @return Valid async_t object on success. NULL on error.
 */
__attribute__((__nonnull__(1), __warn_unused_result__))
async_t *async_create(void *(*f)(void*), void *arg);
/**
 * Same as async_create, but to function f is passed the returned value.
 * @return Valid async_t object on success. NULL on error.
 */
__attribute__((__nonnull__(1), __warn_unused_result__))
async_t *async_create_arg_self(void *(*f)(async_t*));
/**
 * Attach the continuation function f to t.
 * @return Valid async_t object on success. NULL on error.
 */
__attribute__((__nonnull__, __warn_unused_result__))
async_t *async_then(async_t *t, void *(*f)(async_t*));
/**
 * Attach the continuation function f to when all asyncs object are ready
 * @return Valid async_t object on success. NULL on error.
 */
__attribute__((__nonnull__(3), __warn_unused_result__))
async_t *async_when_all(async_t *asyncs[], size_t asyncs_cnt, void *(*f)(async_t *asyncs[], size_t asyncs_cnt));
/**
 * Attach the continuation function f to when any of the asyncs object are ready
 * @return Valid async_t object on success. NULL on error.
 */
__attribute__((__nonnull__(3), __warn_unused_result__))
async_t *async_when_any(async_t *asyncs[], size_t asyncs_cnt, void *(*f)(async_t *asyncs[], size_t asyncs_cnt));

/**
 * @}
 * @defgroup Desctuctors
 * @{
 */
/**
 * Detaches async object, calls pthread_detach(3)
 * Call when you want the async object to execute but want to lose the handle.
 */
__attribute__((__nonnull__(1)))
void async_detach(async_t **t);
/**
 * Canceles async object, calls pthread_cancel(3)
 * Call when you want the async object to end
 */
__attribute__((__nonnull__(1)))
void async_cancel(async_t **t);

/**
 * @}
 * @defgroup Methods
 * @{
 */
/**
 * Checks if the shared state is ready
 */
__attribute__((__nonnull__(1), __warn_unused_result__))
bool async_is_ready(async_t *t);
/**
 * Returns the result
 */
__attribute__((__nonnull__(1)))
void *async_get(async_t *t);
/**
 * Waits for the result to become available
 */
__attribute__((__nonnull__(1)))
void async_wait(async_t *t);
/**
 * Wait for the result to become available for specified timeout.
 * @param t valid handle to async_t object
 * @param timeout_ms timeout passed to poll(2) function
 */
__attribute__((__nonnull__(1)))
bool async_wait_for(async_t *t, int timeout_ms);

/**
 * @}
 * @ Methods for multiple async objects
 * @{
 */
/**
 * For each async in asyncs call async_detach
 */
void asyncs_detach(async_t *asyncs[], size_t asyncs_cnt);
/**
 * For each async in asyncs call async_cancel
 */
void asyncs_cancel(async_t *asyncs[], size_t asyncs_cnt);
/**
 * Wait for all asyncs to complete
 */
void asyncs_wait(async_t *asyncs[], size_t asyncs_cnt);
/**
 * Wait for all asyncs to complete in specified timeout
 */
int asyncs_wait_for(async_t *asyncs[], size_t asyncs_cnt, int timeout);
/**
 * Wait for any asyncs to complete in specified timeout
 */
int asyncs_anywait_for(async_t *asyncs[], size_t asyncs_cnt, int timeout);


/**
 * @}
 * @defgroup Miscelanous
 * @{
 */
/**
 * Async failure handler, declared with weak linkeage
 */
void async_fail_handler(const char file[], int line, const char func[], const char expr[]);

/**
 * @}
 * @defgroup Tests
 * @{
 */
/**
 * Runs internal async unittesting
 */
__attribute__((__warn_unused_result__))
int async_unittest(void);

/**
 * @}
 */

#endif /* ASYNC_ASYNC_H_ */


