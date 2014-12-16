
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

static jmp_buf env;
int real_func(int i)
{
	if (i==0) {
		longjmp(env,1);
	}

	return i;
}

int rust_callfunc(int i)
{
	int ret;
	ret = setjmp(env);
	switch (ret)	{
		case 0:
			printf("normal jump\n");
			return real_func(i);
		case 1:
			fprintf(stderr,"error jump 1\n");
			break;
		default:
			fprintf(stderr,"error jump from nowhere\n");
			break;
	}
	return -1;
}

int main(int argc,char *argv[])
{
	rust_callfunc(1);
	return rust_callfunc(0);
}
