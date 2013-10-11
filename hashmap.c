
#include <string.h>
#include "hashmap.h"

hashmap_ptr create_hashmap()
{
    hashmap_ptr hm = (hashmap_ptr)malloc(sizeof(struct hashmap));
    hm->val_num = 0;
    hm->bucket_num = HASH_CAPACITY;
    hm->bucket = (HItem **)malloc(sizeof(HItem *) * HASH_CAPACITY);
    for(int i=0;i<hm->bucket_num;i++)
        hm->bucket[i]=NULL;
    hm->keys = create_vector();
    hm->destructor = free;
    return hm;
}

void hashmap_set(hashmap_ptr hm, char *key, void *val, size_t len)
{
    int keylen = strlen(key)+1;
    int idx = hash_func(key) % hm->bucket_num;
    HItem *cur = hm->bucket[idx];
    HItem *last = NULL;
    while(cur != NULL)
    {
        if(strcmp(key,cur->key) == 0)
        {
            hm->destructor(cur->val);
            cur->len = len;
            cur->val = (void*)malloc(cur->len);
            memcpy(cur->val,val,cur->len);
            return;
        }
        last = cur;
        cur = cur->next;
    }
    cur = (HItem *)malloc(sizeof(HItem));
    cur->len = len;
    hm->destructor(cur->val);
    free(cur->key);
    cur->val = malloc(cur->len);
    cur->key = (char *)malloc(keylen);
    memcpy(cur->val,val,len);
    memcpy(cur->key,key,keylen);
    cur->next = NULL;
    if (last == NULL) hm->bucket[idx] = cur;
    else last->next = cur;
    hm->val_num ++;
    vector_add(hm->keys,key,keylen);
    return;
}

void* hashmap_get(hashmap_ptr hm, char *key)
{
    if(key == NULL || strlen(key) == 0) return NULL;
    int idx = hash_func(key) % hm->bucket_num;
    HItem *cur = hm->bucket[idx];
    while(cur != NULL)
    {
        if(strcmp(key,cur->key) == 0)
            return cur->val;
        cur=cur->next;
    }
    return NULL;
}

void hashmap_remove(hashmap_ptr hm, char *key)
{
    if(key == NULL || strlen(key) == 0) return;
    int idx = hash_func(key) % hm->bucket_num;
    HItem *cur =hm->bucket[idx];
    HItem *last = NULL;
    while(cur != NULL)
    {
        if(strcmp(key,cur->key) == 0)
        {
            if(last == NULL) hm->bucket[idx] = cur->next;
            else last->next = cur->next;
            int keyidx = vector_index(hm->keys,key,strlen(key)+1);
            vector_remove(hm->keys,keyidx);
            hm->destructor(cur->val);
            free(cur->key);
            free(cur);
            hm->val_num --;
        }
        last = cur;
        cur=cur->next;
    }
}

void destroy_hashmap(hashmap_ptr hm)
{
    for(int i=0;i<hm->bucket_num;i++)
    {
        HItem *cur = hm->bucket[i];
        HItem *last = NULL;
        while(cur != NULL)
        {
            free(cur->key);
            hm->destructor(cur->val);
            last = cur;
            cur = cur->next;
            free(last);
        }
    }
    destroy_vector(hm->keys);
    free(hm->bucket);
    free(hm);
}

void hashmap_resize(hashmap_ptr hm, size_t new_size)
{
    if (new_size <= hm->bucket_num)
        return ;
    hm->bucket = (HItem **)realloc(hm->bucket,new_size * sizeof(HItem *));
    for(int i=0;i<hm->val_num;i++)
    {
        char *key=(char*)vector_get(hm->keys,i);
        int idx = hash_func(key) % hm->bucket_num;
        HItem *cur= hm->bucket[idx];
        HItem *last=NULL;
        while(cur != NULL)
        {
            if(strcmp(cur->key,key) == 0)
                break;
            last=cur;
            cur=cur->next;
        }
        if (cur != NULL)
        {
            if (last == NULL) hm->bucket[idx] = cur->next;
            else last->next = cur->next;
            cur->next = NULL;
        }
        int new_idx = hash_func(key) % new_size;
        last = hm->bucket[new_idx];
        if(last == NULL)
            hm->bucket[new_idx]=cur;
        else
        {
            while(last->next != NULL)
                last=last->next;
            last=cur;
        }
    }
    hm->bucket_num = new_size;
}

size_t hash_func(const char *str)
{
    //D.J.B
    long hash = 5381;
    for(int i = 0; i < strlen(str); i++)
    {
        hash = ((hash << 5) + hash) + (int)str[i] - '0';
    }
    return hash;
}
