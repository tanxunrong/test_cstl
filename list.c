
#include "list.h"

list_ptr create_list()
{
    list_ptr dl = (list_ptr)malloc(sizeof(struct list));
    memset(list_ptr,0,sizeof(struct list));
    dl->destructor=free;
    dl->start = NULL;
    dl->end = NULL;
    dl->size = 0;
    return dl;
}

void list_push(list_ptr dl, void *data, size_t len)
{
    litem_ptr node = (litem_ptr)malloc(sizeof(struct l_item));
    memset(node,0,sizeof(struct l_item));
    node->data = (void*)malloc(len);
    memcpy(node->data,data,len);
    litem_ptr cur = dl->end;
    if (dl->end == NULL)
    {
        dl->start = node;
    }
    else
    {
        cur->next = node;
        node->prev = cur;
    }
    dl->end = node;
    dl->size ++;
}

void *list_pop(list_ptr dl)
{
    if (dl->size == 0)
        return NULL;
    litem_ptr cur = dl->end;
    cur = cur->prev;
    cur->next = NULL;
    litem_ptr pop = dl->end;
    pop->prev = NULL;
    dl->end = cur;
    void *data = pop->data;
    free(pop);
    return data;
}

void list_unshift(list_ptr dl, void *data, size_t len)
{
    litem_ptr node = (litem_ptr)malloc(sizeof(struct l_item));
    memset(node,0,sizeof(struct l_item));
    node->data = (void*)malloc(len);
    memcpy(node->data,data,len);
    litem_ptr cur = dl->start;
    if (dl->start == NULL)
    {
        dl->end = node;
    }
    else
    {
        cur->next = node;
        node->prev = cur;
    }
    dl->end = node;
    dl->size ++;
}
