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
#include <stdarg.h>

#define ASYNC_THREAD_STARTUP_TIME_TIMESPEC  { .tv_sec = 1, .tv_nsec = 0 }

typedef struct async_s async_t;
typedef void *(*async_funcptr_void_t)(void *arg);
typedef void *(*async_funcptr_then_t)(async_t *async);
typedef void *(*async_funcptr_when_t)(async_t *asyncs[], size_t asyncs_len);

enum async_type_e {
	ASYNC_TYPE_VOID,
	ASYNC_TYPE_THEN,
	ASYNC_TYPE_WHENALL,
	ASYNC_TYPE_WHENANY,
};
enum {
	ASYNC_TYPE_MAX = ASYNC_TYPE_WHENANY,
};
enum async_attr_e {
	ASYNC_ATTR_ASYNC        = 0<<0,
	ASYNC_ATTR_DEFERRED     = 1<<0
};

union async_call_u {
	// valid only if type == ASYNC_TYPE_VOID
	struct async_call_void_s {
		void *(*f)(void *);
		void *arg;
	} v;
	// valid only if type == ASYNC_TYPE_THEN
	struct async_call_async_s {
		void *(*f)(async_t *);
		async_t *arg;
	} t;
	// valid only if type = ASYNC_TYPE_WHEN*
	struct async_call_asyncs_s {
		void *(*f)(async_t *asyncs[], size_t asyncs_cnt);
		async_t **asyncs;
		size_t asyncs_cnt;
	} w;
};

struct async_s {
	union {
		pthread_t thread;
		pid_t pid;
	};

	struct {
		// mutex protecting this struct
		pthread_mutex_t mutex;
		// Eventfd file descriptor shared with calling thread
		int readyfd;
		// Becomes the returned value of called function after readyfd is ready
		void *returned;
		// Reference count
		unsigned int refcnt;
		// thread started?
		bool started;
	};

	enum async_attr_e attr;
	enum async_type_e type;
	union async_call_u c;

};

/* Exported Functions ---------------------------------------------------------------- */
/**
 * @defgroup Constructors
 * @{
 */

__attribute__((__warn_unused_result__))
async_t *async_create_ex(enum async_attr_e attr, enum async_type_e type, union async_call_u call);

/**
 * Create new async object. Run function f with argument arg.
 * @return Valid async_t object on success. NULL on error.
 */
__attribute__((__nonnull__(1), __warn_unused_result__))
async_t *async_create(void *(*f)(void*), void *arg);
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

__attribute__((__nonnull__(1), __sentinel__))
async_t *async_create_chain(void *(*f)(void*), void *arg, ...);
__attribute__((__nonnull__(1,2), __sentinel__))
async_t *async_then_chain(async_t *t, void *(*f)(async_t *), ...);
__attribute__((__nonnull__(1)))
async_t *async_create_chain_va(void *(*f)(void*), void *arg, va_list va);
__attribute__((__nonnull__(1,2)))
async_t *async_then_chain_va(async_t *t, void *(*f)(async_t *), va_list va);

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
 * Get is a attribute is deferred
 */
static inline bool async_attr_isDefered(enum async_attr_e attr)
{
	return attr & ASYNC_ATTR_DEFERRED;
}
/**
 * Get if a attribute is asynchronous
 */
static inline bool async_attr_isAsync(enum async_attr_e attr)
{
	return !(async_attr_isDefered(attr));
}
/**
 * Checks if the shared state is ready
 */
__attribute__((__nonnull__(1), __warn_unused_result__))
int async_is_ready(async_t *t);
/**
 * Returns the result
 */
__attribute__((__nonnull__(1)))
void *async_get(async_t *t);
/**
 * Waits for the result to become available
 */
__attribute__((__nonnull__(1)))
int async_wait(async_t *t);
/**
 * Wait for the result to become available for specified timeout.
 * @param t valid handle to async_t object
 * @param timeout_ms timeout passed to poll(2) function
 */
__attribute__((__nonnull__(1)))
int async_wait_for(async_t *t, int timeout_ms);

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
int asyncs_wait(async_t *asyncs[], size_t asyncs_cnt);
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
__attribute__((__nonnull__))
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


