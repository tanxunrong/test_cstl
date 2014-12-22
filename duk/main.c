#include <stdlib.h>
#include <stdio.h>
#include <duktape.h>

int main(int argc,char *argv[])
{
    duk_context *c =  duk_create_heap_default();
    duk_destroy_heap(c);
	return 0;
}
