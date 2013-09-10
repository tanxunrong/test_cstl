
#ifndef __LIBTXR_VECTOR_H__
#define __LIBTXR_VECTOR_H__
#include <stdlib.h>
#include <string.h>

#define INIT_CAPACITY 100
#define CAPACITY_RATIO 1.5f

struct vector {
    void **data;
    int *size;
    size_t length;
    size_t capacity;
    void (*destructor) (void*);
};

typedef struct vector * vector_ptr;
vector_ptr create_vector();
void vector_add(vector_ptr vec,void *data,size_t datalen);
void* vector_get(vector_ptr vec,int index);
int vector_index(vector_ptr vec,void *data,size_t datalen);
int vector_set(vector_ptr vec,void *data,size_t datalen,int index);
int vector_insert(vector_ptr vec,void *data,size_t datalen,int index);
int vector_remove(vector_ptr vec,int index);
void check_length(vector_ptr vec);
void destroy_vector(vector_ptr vec);
int vector_swap(vector_ptr vec,int n,int m);
vector_ptr vector_subvec(vector_ptr vec,int start,int end);
#endif
