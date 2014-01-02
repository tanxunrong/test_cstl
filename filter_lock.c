
#include "filter_lock.h"

static int same_or_higher(txr_lock_t *lock,int tid,int level)
{
	for(int i=0;i<T_N;i++)
	{
		if (i != tid && lock->level[i] >= level) return 1;
	}
	return 0;
}

void acquire(txr_lock_t *lock,int idx)
{
	assert(idx < T_N);
	for(int j=1;j<T_N;j++)
	{
		lock->level[idx] = j;
		lock->victim[j] = idx;
		while(lock->victim[j] == idx && same_or_higher(lock,idx,j)) { };
	}
	printf("thread %d got lock\n",idx);
}

void release(txr_lock_t *lock,int idx)
{
	assert(idx < T_N);
	lock->level[idx] = 0;
	printf("thread %d release lock\n",idx);
}

