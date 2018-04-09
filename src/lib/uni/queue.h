/*
 * queue.h
 *
 *  Created on: 9 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_LIB_UNI_QUEUE_H_
#define SRC_LIB_UNI_QUEUE_H_

#include <uni/_queue.h>

/*
 * Iterate over list starting from SLIST_NEXT(from_prev, list) element
 */
#define SLIST_FOREACH_PREV_FROM(from_prev, iterator, iterator_prev, memb) \
	for(iterator_prev = from_prev,  iterator = SLIST_NEXT(iterator_prev, memb); \
	iterator != NULL; \
	iterator_prev = iterator, iterator = SLIST_NEXT(iterator, memb))

#endif /* SRC_LIB_UNI_QUEUE_H_ */
