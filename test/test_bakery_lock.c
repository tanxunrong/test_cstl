
#include "../bakery_lock.h"

static int sum = 0;
static txr_lock_t *lock;
void *worker(void *param)
{
	int i = *((int*)param);
	acquire(lock,i);
	sum++;
	sleep(1);
	assert(sum <= 1);
	sum--;
	release(lock,i);
	free(param);
	return NULL;
}

int main(int argc,char *argv[])
{
	lock = (txr_lock_t *)malloc(sizeof(txr_lock_t));
	assert(lock);
	memset(lock,0,sizeof(txr_lock_t));
#define THREAD_COUNT 20
	pthread_t workers[THREAD_COUNT];
	for(int k=0;k<THREAD_COUNT;k++)
	{
		int *j = (int*)malloc(sizeof(int));
		memcpy(j,&k,sizeof(int));
		if (pthread_create(&workers[k],NULL,worker,j) != 0)
		{
			fprintf(stderr,"create thread fail\n");
			exit(1);
		}
	}

	for(int k=0;k<THREAD_COUNT;k++)
	{
		pthread_join(workers[k],NULL);
	}
	free(lock);
	return 0;
}


