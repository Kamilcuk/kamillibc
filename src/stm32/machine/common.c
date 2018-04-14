/*
 * common.c
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */
#include <machine/common.h>

#include "stm32xxxx_ll.h" // LL_GetUID_Word{0,1,2}

unsigned int HW_srand_seed()
{
	const uint32_t ID1 = LL_GetUID_Word0();
	const uint32_t ID2 = LL_GetUID_Word1();
	const uint32_t ID3 = LL_GetUID_Word2();
	return ID1 ^ ID2 ^ ID3;
}

static inline uint8_t * HW_unique_id_32(uint8_t *pnt, uint32_t ID)
{
	pnt++[0] = ID;
	pnt++[0] = ID >> 8;
	pnt++[0] = ID >> 16;
	pnt++[0] = ID >> 24;
	return pnt;
}

void HW_unique_id(uint8_t *pnt, size_t pntsize)
{
	assert(pntsize == 3*4);
	HW_unique_id_32(
			HW_unique_id_32(
					HW_unique_id_32(pnt,
							LL_GetUID_Word0()
					), LL_GetUID_Word1()
			), LL_GetUID_Word2()
	);
}
