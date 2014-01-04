
#include "vector.h"

vector_ptr create_vector()
{
    vector_ptr vec = (vector_ptr)malloc(sizeof(struct vector));
    vec->data = (void**)malloc(sizeof(void *) * INIT_CAPACITY);
    vec->size = (int*)malloc(sizeof(int) * INIT_CAPACITY);
    vec->length = 0;
    vec->capacity = INIT_CAPACITY;
    vec->destructor = free;
    return vec;
}

void check_length(vector_ptr vec)
{
    if(vec->length >= vec->capacity)
    {
       vec->capacity *= CAPACITY_RATIO;
       vec->data = (void**)realloc((void*)vec->data,sizeof(void*) * vec->capacity);
       vec->size = (int*)realloc((void*)vec->size,sizeof(int) * vec->capacity);
    }
}

void vector_add(vector_ptr vec, void *data, size_t datalen)
{
    check_length(vec);
    vec->data[vec->length] = (void*)malloc(datalen);
    vec->size[vec->length] = datalen;
    memcpy(vec->data[vec->length],data,datalen);
    vec->length +=1;
}

void* vector_get(vector_ptr vec, int index)
{
    if(vec->length <= index)
        return NULL;
    return vec->data[index];
}

int vector_set(vector_ptr vec, void *data, size_t datalen, int index)
{
    if(index < 0 || index >= vec->length)
        return -1;
    vec->destructor(vec->data[index]);
    vec->data[index] = (void*)malloc(datalen);
    memcpy(vec->data[index],data,datalen);
    vec->size[index] = datalen;
    return 0;
}

int vector_index(vector_ptr vec, void *data, size_t datalen)
{
    for(int i = 0;i<vec->length;i++)
    {
        if (data == vec->data[i] || memcmp(data,vec->data[i],datalen) == 0)
            return i;
    }
    return -1;
}

int vector_insert(vector_ptr vec, void *data, size_t datalen, int index)
{
    check_length(vec);
    if(index >= vec->length || index < 0)
        return -1;
    for(int i=vec->length;i>index;i--)
    {
        vec->data[i] = vec->data[i-1];
        vec->size[i] = vec->size[i-1];
    }
    vec->data[index] = (void*)malloc(datalen);
    vec->size[index] = datalen;
    memcpy(vec->data[index],data,datalen);
    vec->length +=1;
    return 0;
}

int  vector_remove(vector_ptr vec, int index)
{
    check_length(vec);
    if(index < 0 || index >= vec->length)
        return -1;
    vec->destructor(vec->data[index]);
    for(int i=index;i<vec->length;i++)
    {
        vec->data[i]=vec->data[i+1];
        vec->size[i]=vec->size[i+1];
    }
    vec->length --;
    return 0;
}

void destroy_vector(vector_ptr vec)
{
    for(int i=0;i<vec->length;i++)
    {
        vec->destructor(vec->data[i]);
    }
    free(vec->data);
    free(vec->size);
    free(vec);
}

int vector_swap(vector_ptr vec, int n, int m)
{
    if (n < 0 || m < 0 || n >= vec->length || m >= vec->length)
        return -1;
    if (n == m)
        return 0;
    void *tmp=(void*)malloc(vec->size[n]);
    memcpy(tmp,vec->data[n],vec->size[n]);
    int tmplen = vec->size[n];
    vec->destructor(vec->data[n]);
    vec->data[n]=vec->data[m];
    vec->size[n]=vec->size[m];
    vec->data[m]= tmp;
    vec->size[m]=tmplen;
    return 0;
}

vector_ptr vector_subvec(vector_ptr vec, int start, int end)
{
    if (start < 0 || end < 0 || start >= vec->length || end >= vec->length || start > end)
        return NULL;
    vector_ptr subvec = create_vector();
    for(int i = start;i<=end;i++)
    {
        vector_add(subvec,vec->data[i],vec->size[i]);
    }
    return subvec;
}
