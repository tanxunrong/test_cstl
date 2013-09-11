
#ifndef __LIBTXR_LIST_H__
#define __LIBTXR_LIST_H__

#include <stdlib.h>
#include <string.h>

struct l_item {
    void *data;
    int len;
    struct l_item *prev,*next;
};
typedef struct l_item LItem;
struct _list {
    size_t size;
    void (*destructor) (void*);
    LItem *start,*end;
};

typedef struct _list * list_ptr;

list_ptr create_list();
void list_push(list_ptr dl,void *data,size_t len);
void list_unshift(list_ptr dl,void *data,size_t len);
void *list_pop(list_ptr dl);
void *list_shift(list_ptr dl);
size_t list_size(list_ptr dl);
void destroy_list(list_ptr dl);

#endif
