
#ifndef __LIBTXR_SLAB_H__
#define __LIBTXR_SLAB_H__
#include <stdlib.h>
#include <stdint.h>

struct slab_chunk {
	unsigned int itemsize,itemcount;
	unsigned int refcount,end;
	void *data;//连续内存
	size_t memsize;
	struct slab_chunk *prev,*next;
};
struct slab_chain {
	struct slab_chunk *full,*empty,*partial;
};


#endif
