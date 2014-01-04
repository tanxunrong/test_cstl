
#ifndef __LIBTXR_RBTREE_H__
#define __LIBTXR_RBTREE_H__

#include <stdlib.h>

typedef int (*treenodecmpfunc)(void*,void*);

enum rbcolor {
    RED,
    BLACK
};

struct tnode {
    struct tnode *parent,*left,*right;
    void *key,*data;
    size_t datalen;
    enum rbcolor color;
};

typedef struct tnode * tnode_p;

struct rbtree {
    treenodecmpfunc cmp;
    tnode_p root;
};

typedef struct rbtree * rbtree_p;

rbtree_p create_rbtree(treenodecmpfunc rbcmsp);
int rbtree_insert(rbtree_p rb,void *key,void *data,int len);
void *rbtree_lookup(rbtree_p rb,void *key);
int rbtree_remove(rbtree_p rb,void *key);
#endif
