
#include "bakery_lock.h"

static int label_higher(txr_lock_t *lock,int idx)
{
	for (int k=0;k<T_N;k++)
	{
		if (k == idx || lock->flag[k] == 0)
			continue;
		//(label[k],k) << (lable[idx],idx)
		// <=>
		//label[k] < label[idx] || (label[k] == label[idx] && k < idx)
		// <=>
		if (lock->label[k] < lock->label[idx] 
			|| ( lock->label[k] == lock->label[idx] && k < idx))
		{
			return 1;
		}
	}
	return 0;
}

void acquire(txr_lock_t *lock,int idx)
{
	assert(idx < T_N);
	lock->flag[idx] = 1;
	int max=0;
	for(int i=0;i<idx;i++)
	{
		if (max < lock->label[i])
		{
			max = lock->label[i];
		}
	}
	lock->label[idx] = max+1;
	while(lock->flag[idx] && label_higher(lock,idx)) {};
	printf("%d got\n",idx);
}

void release(txr_lock_t *lock,int idx)
{
	assert(idx < T_N);
	lock->flag[idx] = 0;
	printf("%d release\n",idx);
}

