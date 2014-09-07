#include <rocksdb/c.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//int test_cmp(void *data,const char *a,size_t alen,const char *b,size_t blen)
//{

//}

enum OperType {
    DEL,
    GET,
    SET
};

int main(int argc,char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr,"test set $key $value,get|del $key");
        exit(-1);
    }
    char *operation = argv[1];
    char *key,*val = NULL;
    enum OperType type;
    if (strcasecmp(operation,"set") == 0 )
    {
        type = SET;
        if (argc >= 4)
        {
            key = argv[2];val = argv[3];
        }
        else goto err_params;
    }
    else if (strcasecmp(operation,"get") == 0)
    {
        type = GET;
        if (argc >= 3)
        {
            key = argv[2];
        }
        else goto err_params;
    }
    else if (strcasecmp(operation,"del") == 0)
    {
        type = DEL;
        if (argc >= 3)
        {
            key = argv[2];
        }
        else goto err_params;
    }
    else goto err_params;

    rocksdb_options_t *opt = rocksdb_options_create();
    assert(opt);
    rocksdb_options_set_create_if_missing(opt,1);

    char *err=NULL;
    rocksdb_t *db = rocksdb_open(opt,"/tmp/testrocksdb",&err);
    if (err != NULL)
    {
        fprintf(stderr,"db open failed %s",err);
        exit(-1);
    }

    rocksdb_writeoptions_t *put_opt = rocksdb_writeoptions_create();
    assert(put_opt);
    rocksdb_writeoptions_set_sync(put_opt,1);

    rocksdb_readoptions_t *get_opt = rocksdb_readoptions_create();
    assert(get_opt);
    rocksdb_readoptions_set_verify_checksums(get_opt,1);

    if (type == SET)
    {
        rocksdb_put(db,put_opt,key,strlen(key),val,strlen(val),&err);
        if (err != NULL)
        {
            fprintf(stderr,"db set %s = %s failed %s",key,val,err);
            exit(-1);
        }
        printf("set ok\n");
    }
    else if (type == GET)
    {
        size_t len;
        val = rocksdb_get(db,get_opt,key,strlen(key),&len,&err);
        if (err != NULL)
        {
            fprintf(stderr,"db get %s failed %s",key,err);
            exit(-1);
        }
        if (len > 0)
        {
            printf("get %s = %s\n",key,val);
        }
        else
        {
            printf("get %s empty\n",key);
        }
    }
    else
    {
        rocksdb_delete(db,put_opt,key,strlen(key),&err);
        if (err != NULL)
        {
            fprintf(stderr,"db del %s failed %s",key,err);
            exit(-1);
        }
        printf("del %s ok\n",key);
    }

    rocksdb_options_destroy(opt);
    rocksdb_writeoptions_destroy(put_opt);
    rocksdb_readoptions_destroy(get_opt);
    rocksdb_close(db);
    return 0;
err_params:
    fprintf(stderr,"test set $key $value,get|del $key");
    exit(-1);

}
