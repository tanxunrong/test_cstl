
#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>

static ucontext_t ctx;
int real_func(int i)
{
	if (i==0) {
		setcontext(&ctx);
	}

	return i;
}

int rust_callfunc(int i)
{
	int ret;
	volatile int finish = 0;
	ret = getcontext(&ctx);
	if (!ret) {
		printf("get ctx success\n");
		if (!finish) {
			finish = 1;
			printf("do real func\n");
			return real_func(i);
		} else {
			printf("finished\n");
			return -1;
		}
	} else {
		printf("get ctx fail\n");
		return ret;
	}
}

int main(int argc,char *argv[])
{
	printf("step 1\n");
	rust_callfunc(1);
	printf("step 2\n");
	return rust_callfunc(0);
}
