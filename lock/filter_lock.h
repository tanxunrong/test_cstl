
#ifndef _TXR_FILTER_LOCK_H_
#define _TXR_FILTER_LOCK_H_
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#define T_N	100

typedef struct txr_lock txr_lock_t;
struct txr_lock {
	int level[T_N];
	int victim[T_N];
};

void acquire(txr_lock_t *lock,int idx);
void release(txr_lock_t *lock,int idx);

#endif
