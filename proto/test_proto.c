#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "message.pb-c.h"
int main(int argc,char *argv[])
{
	BackCall *msg = malloc(sizeof(BackCall));
	back_call__init(msg);
	void *buf;
	unsigned int len;
	if ( argc < 4 )
	{
		fprintf(stderr,"usage : test_proto $class $func $uid $params ... \n");
		exit(EXIT_FAILURE);
	}
	msg->clazz = argv[1];
	msg->funcname = argv[2];
	msg->uid = atoi(argv[3]);
	if ( argc > 4 )
	{
		msg->n_params = argc - 4;
		size_t plen = sizeof(char*) * (argc-4);
		msg->params = (char**)malloc(plen);
		for(int i=4;i<argc;i++)
		{
			char *addr = *(msg->params)+i-4; 
		    memcpy(&addr,&argv[i],sizeof(char*));
		}
	}

	len = back_call__get_packed_size(msg);
	buf = malloc(len);
	back_call__pack(msg,buf);
	fprintf(stderr,"writing %d bytes\n",len);
	fwrite(buf,len,1,stdout);
	return 0;
}


