#include <stdio.h>
#include <string.h>

#include "vector.h"

int main(int argc,char *argv[])
{
    vector_ptr tt = create_vector();
    char *str1 = "to president";
    char *str2 = "to prince";
    for(int i=0;i<15;i++)
    {
        vector_add(tt,(void*)str1,strlen(str1)+1);
    }
    vector_insert(tt,(void*)str2,strlen(str2)+1,14);
    vector_set(tt,(void*)str2,strlen(str2)+1,7);
    printf("the 7th %s\n",(char*)vector_get(tt,7));
    vector_remove(tt,15);
    for(int i = 0;i< tt->length;i++)
    {
        printf("%s\n",(char*)vector_get(tt,i));
    }
    vector_ptr subtt = vector_subvec(tt,7,14);
    vector_swap(subtt,0,1);
    int idx = vector_index(subtt,(void*)str2,strlen(str2)+1);
    printf("idx %d \n",idx);
    for(int i = 0;i< subtt->length;i++)
    {
        printf("%s\n",(char*)vector_get(subtt,i));
    }
    destroy_vector(tt);
    destroy_vector(subtt);
    return 0;
}
