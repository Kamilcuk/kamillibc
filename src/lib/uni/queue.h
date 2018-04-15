/*
 * queue.h
 *
 *  Created on: 9 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_UNI_QUEUE_H_
#define SRC_LIB_UNI_QUEUE_H_

#include <uni/_queue.h>

/**
 * Iterate over list starting from SLIST_NEXT(from_prev, list) element
 */
#define SLIST_FOREACH_PREV_FROM(from_prev, iterator, iterator_prev, memb) \
	for(iterator_prev = from_prev,  iterator = SLIST_NEXT(iterator_prev, memb); \
	iterator != NULL; \
	iterator_prev = iterator, iterator = SLIST_NEXT(iterator, memb))


/**
 * varp is NULL, when var = SLIST_FIRST(head)
 */
#define	SLIST_FOREACH_PREVPTR2(var, varp, head, field) \
	for ((varp) = NULL, \
	(var) = SLIST_FIRST((head)); \
	(var); \
	(varp) = (var), \
	(var) = SLIST_NEXT((var), field))


/**
 * SAFE - you may call remove within loop
 * PREPTR - previous pointer is saved, to remove fast from loop
 */
#define	SLIST_FOREACH_PREVPTR_SAFE(var, varp, head, field, tvar) \
	for ((varp) = NULL, \
	(var) = SLIST_FIRST((head)); \
	(var) && ((tvar) = SLIST_NEXT((var), field), 1); \
	(varp) = (var), \
	(var) = (tvar))

/**
 * Insert elm in the list, either after var (if var != NULL), or as new head (if var == NULL)
 */
#define SLIST_INSERT_HEAD_OR_AFTER(var, head, elm, field) do{ \
	if (var == NULL) { \
		SLIST_INSERT_HEAD(head, elm, field); \
	} else { \
		SLIST_INSERT_AFTER(varp, elm, field); \
	} \
}while(0)

/**
 * removes var from list, taking into account it may be head.
 * If previous pointer == NULL, that means that var = SLIST_FIRST(head)
 */
#define SLIST_REMOVE_HEAD_OR_AFTER(var, varp, head, field) do{ \
	assert(SLIST_NEXT(var, field) == varp); \
	if (varp == NULL) { \
		SLIST_REMOVE_HEAD(head, field); \
	} else { \
		SLIST_REMOVE_AFTER(varp, field); \
	} \
}while(0)

/**
 * check if elm exist in list chain
 */
#define SLIST_IS_IN_LIST_ANSI(ret, head, elm, field) do{ \
	ret = false; \
	__typeof__(SLIST_FIRST(head)) curelm; \
	SLIST_FOREACH(curelm, head, field) { \
		if (curelm == elm) { \
			ret = true; \
			break; \
		} \
	} \
}while(0)

/**
 * Check if elm is in list, __GNUC__ extension
 */
#define SLIST_IS_IN_LIST(head, elm, field) __extension__({ \
	bool ret; \
	SLIST_IS_IN_LIST_ANSI(ret, head, elm, field); \
	ret; \
})

#endif /* SRC_LIB_UNI_QUEUE_H_ */
