
#include "filter_lock.h"
#include <string.h>
#include <unistd.h>

static txr_lock_t *lock;
void *worker(void *param)
{
	int i = *((int*)param);
	acquire(lock,i);
	sleep(1);
	release(lock,i);
	free(param);
	return NULL;
}

int main(int argc,char *argv[])
{
	pthread_t sid = pthread_self();
	printf("main thread %ld\n",sid);
	lock = (txr_lock_t *)malloc(sizeof(txr_lock_t));
	assert(lock);
	memset(lock,0,sizeof(txr_lock_t));
	pthread_t workers[6];
	for(int k=0;k<6;k++)
	{
		int *j = (int*)malloc(sizeof(int));
		memcpy(j,&k,sizeof(int));
		if (pthread_create(&workers[k],NULL,worker,j) != 0)
		{
			fprintf(stderr,"create thread fail\n");
			exit(1);
		}
	}

	for(int k=0;k<6;k++)
	{
		pthread_join(workers[k],NULL);
	}
	free(lock);
	return 0;
}


