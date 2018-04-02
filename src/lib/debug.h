#ifndef LIB_DEBUG_H_
#define LIB_DEBUG_H_

#include <stdio.h>

/* Configuration -------------------------------------- */

#ifndef DBG_FILE
# define DBG_FILE stdout
#endif

#ifndef DBG_LEVEL
# define DBG_LEVEL -1
#endif

#ifndef DBG_MASK
# define DBG_MASK 0xFFFF
#endif

#ifndef DBG_ENV_ENABLED
#define DBG_ENV_ENABLED 1
#endif

/* Exported Constnats ------------------------------------ */

enum {
	DBG_LEVEL_MAX = 16,
};

enum DBG_MASKS_e {
	DBGM_1 = 1<<0,
	DBGM_2 = 1<<1,
	DBGM_3 = 1<<2,
	DBGM_4 = 1<<3,
	DBGM_5 = 1<<4,
	DBGM_6 = 1<<5,
	DBGM_7 = 1<<6,
	DBGM_8 = 1<<7,
	DBGM_9 = 1<<8,
	DBGM_10 = 1<<9,
	DBGM_11 = 1<<10,
	DBGM_12 = 1<<11,
	DBGM_13 = 1<<12,
	DBGM_14 = 1<<13,
	DBGM_15 = 1<<14,
	DBGM_16 = 1<<15,
};

/* Exported functions ------------------------------------- */

int dbg_get_lvl_from_env();

/**
 * dbgl
 * Prints a debugging message, only if debugging is greater then specified level
 */
#if DEBUG
void dbgl(int lvl, const char *fmt, ...) __attribute__((__format__(__printf__,2,3)));
#define dbgl(lvl, fmt, ...) do{ \
	if ( (lvl) < (DBG_LEVEL) || (lvl) < dbg_get_lvl_from_env() ) { \
		fprintf(DBG_FILE, fmt, ##__VA_ARGS__); \
	} \
}while(0)
#else
#define dbgl(lvl, fmt, ...) do{}while(0)
#endif

/**
 * dbgm
 * Print debugging message, only if debugging mask in DBG_MASK is set
 */
#if DEBUG
void dbgm(int mask, const char *fmt, ...) __attribute__((__format__(__printf__,2,3)));
#define dbgm(mask, fmt, ...) do{ \
	if ( (DBG_MASK)&(mask) ) { \
		fprintf(DBG_FILE, fmt, ##__VA_ARGS__); \
	} \
}while(0)
#else
#define dbgm(mask, fmt, ...) do{}while(0)
#endif

/**
 * dbg
 * equal to calling dbgl with lvl set to 0
 */
#define dbg(...)  dbgl(0, __VA_ARGS__)

#endif //LIB_DEBUG_H_
