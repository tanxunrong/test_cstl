
#include <stdio.h>
#include "hashmap.h"
#include <assert.h>

int main(void)
{
    hashmap_ptr dict = create_hashmap();
    int age = 18;
    hashmap_set(dict,"joe",(void*)&age,sizeof(int));
    printf("age %d\n",*(int *)hashmap_get(dict,"joe"));
    age = 20;
    hashmap_set(dict,"chick",(void*)&age,sizeof(int));
    printf("age %d\n",*(int *)hashmap_get(dict,"joe"));
    hashmap_remove(dict,"joe");
    assert(hashmap_get(dict,"joe") == NULL);
    hashmap_resize(dict,1000);
    printf("age %d\n",*(int *)hashmap_get(dict,"chick"));
    destroy_hashmap(dict);
    return 0;
}

