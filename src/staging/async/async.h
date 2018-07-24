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
#include <assert.h>

/* Exported Macros ------------------------------------------------------- */

#define ASYNC_THREAD_STARTUP_TIME_TIMESPEC  { .tv_sec = 1, .tv_nsec = 0 }

/* Exported Types -------------------------------------------------------- */

typedef struct async_s async_t;
typedef void *(*async_funcptr_void_t)(void *arg);
typedef void *(*async_funcptr_then_t)(async_t *async);
typedef void *(*async_funcptr_when_t)(async_t *asyncs[], size_t asyncs_len);

enum async_attr_e {
	ASYNC_ATTR_ASYNC        = 0<<0,
	ASYNC_ATTR_DEFERRED     = 1<<0
};

enum async_type_e {
	ASYNC_TYPE_VOID,
	ASYNC_TYPE_THEN,
	ASYNC_TYPE_WHENALL,
	ASYNC_TYPE_WHENANY,
};
enum {
	ASYNC_TYPE_MAX = ASYNC_TYPE_WHENANY,
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
	// private:
	// thread running or not
	pthread_t thread;
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
		// user cleanup function
		void (*cleanup)(async_t*);
	};

	enum async_attr_e attr;

	// public:
	enum async_type_e type;
	union async_call_u c;
};

/* Exported Functions ---------------------------------------------------------------- */
/**
 * @defgroup Constructors
 * @{
 */

/**
 * Creates async object using specified attributes with a callback of specified type
 * @return NULL on error, otherwise a valid aynchronous object to be detached or canceled
 */
__attribute__((__warn_unused_result__))
async_t *async_create_ex(enum async_attr_e attr, enum async_type_e type, union async_call_u call);

/**
 * Create new async object. Run function f with argument arg.
 * @return Valid async_t object on success. NULL on error.
 */
__attribute__((__nonnull__(1), __warn_unused_result__))
static inline async_t *async_create(void *(*f)(void*), void *arg)
{
	return async_create_ex(0, ASYNC_TYPE_VOID, (union async_call_u){ .v = {f, arg}});
}

/**
 * Attach the continuation function f to t.
 * @return Valid async_t object on success. NULL on error.
 */
__attribute__((__nonnull__, __warn_unused_result__))
static inline async_t *async_then(async_t *t, void *(*f)(async_t*))
{
	return async_create_ex(0, ASYNC_TYPE_THEN, (union async_call_u){ .t = { .arg = t, .f = f }});
}

/**
 * Attach the continuation function f to when all asyncs object are ready
 * @return Valid async_t object on success. NULL on error.
 */
__attribute__((__nonnull__(3), __warn_unused_result__))
static inline async_t *async_when_all(async_t *asyncs[], size_t asyncs_cnt, void *(*f)(async_t *asyncs[], size_t asyncs_cnt))
{
	return async_create_ex(0, ASYNC_TYPE_WHENALL, (union async_call_u){ .w = {f, asyncs, asyncs_cnt}});
}

/**
 * Attach the continuation function f to when any of the asyncs object are ready
 * @return Valid async_t object on success. NULL on error.
 */
__attribute__((__nonnull__(3), __warn_unused_result__))
static inline async_t *async_when_any(async_t *asyncs[], size_t asyncs_cnt, void *(*f)(async_t *asyncs[], size_t asyncs_cnt))
{
	return async_create_ex(0, ASYNC_TYPE_WHENANY, (union async_call_u){ .w = {f, asyncs, asyncs_cnt}});
}

/**
 * Calls async_create_chain_va
 */
__attribute__((__nonnull__(1), __sentinel__))
async_t *async_create_chain(void *(*f)(void*), void *arg, ...);
/**
 * Calls async_then_chain_va
 */
__attribute__((__nonnull__(1,2), __sentinel__))
async_t *async_then_chain(async_t *t, void *(*f)(async_t *), ...);
/**
 * Creates a chain of async objects each dependend of each other
 * @param
 * @param
 * @param va - NULL terminated list of void *(*f)(async_t*) functions
 * @return NULL on error, otherwise the last handler to the last async object
 */
__attribute__((__nonnull__(1)))
async_t *async_create_chain_va(void *(*f)(void*), void *arg, va_list va);
/**
 * Creates a chain of async objects each dependend of each other
 * @param The async object from which it starts. The object is not modified,
 *        ie. user need to call async_detach or async_cancel on object t
 *        after this function returns
 * @param
 * @param va - NULL terminated list of void *(*f)(async_t*) functions
 * @return NULL on error, otherwise the last handler to the last async object
 */
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
 * This is abnormal thread termination, generally async_detach should be used
 * Call when you want the async object to end
 * Functions registered with async_cleanup_* will be called nonetheless
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
 * Registers cleanup function to be called on thread termination or cancellation
 */
__attribute__((__nonnull__(1,2)))
int async_cleanup_reg(async_t *t, void (*cleanup_func)(async_t *));
/**
 * Check if deffered flag is set in attribute
 */
static inline bool async_attr_isDefered(enum async_attr_e attr)
{
	return attr & ASYNC_ATTR_DEFERRED;
}
/**
 * Check if asynchronous flag is set in attribute
 */
static inline bool async_attr_isAsync(enum async_attr_e attr)
{
	return !(async_attr_isDefered(attr));
}
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


