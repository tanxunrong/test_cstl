
#ifndef __LIBTXR_HASH_H__
#define __LIBTXR_HASH_H__

#include <stdlib.h>
#include "vector.h"

struct h_item {
    char *key;
    void *val;
    int len;
    struct h_item *next;
};

typedef struct h_item HItem;
#define HASH_CAPACITY 101

struct hashmap {
    HItem **bucket;
    vector_ptr keys;
    size_t val_num;
    size_t bucket_num;
    void (*destructor)(void*);
};
typedef struct hashmap * hashmap_ptr;
size_t hash_func(const char *str);

hashmap_ptr create_hashmap();
void hashmap_set(hashmap_ptr hm,char *key,void *val,size_t len);
void *hashmap_get(hashmap_ptr hm,char *key);
void hashmap_remove(hashmap_ptr hm,char *key);
void destroy_hashmap(hashmap_ptr hm);
void hashmap_resize(hashmap_ptr hm,size_t new_size);
#endif
