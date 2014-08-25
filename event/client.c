
#include "test_uv.h"

static uv_buf_t testmsg[2] = {
    { .base = "gogogogo", .len=8},
    { .base = "gogogogo", .len=8}
};

void on_write(uv_write_)
void connect_cb(uv_connect_t *connect,int status)
{
    if (status != 0)
    {
        LEELA_SOCK_ERR(status);
    }

    uv_stream_t *tcp = connect->handle;
    uv_write_t wreq;
    uv_write(&wrep,tcp,testmsg,2,uv_write_cb)
}

int main(int argc,char *argv[])
{
    int port = 7890;
    if (argc > 1)
    {
        port = atoi(argv[1]);
        g_assert(port > 1024);
    }

    uv_loop_t *loop = uv_default_loop();
    struct sockaddr_in addr;
    uv_ip4_addr("127.0.0.1",port,&addr);
#define CLIENT_NUM (1000)
    uv_tcp_t conns[CLIENT_NUM];
    uv_connect_t connect[CLIENT_NUM];
    for(int i=0;i<CLIENT_NUM;i++)
    {
        uv_tcp_init(loop,conns+i);
        uv_tcp_connect(connect+i,conns+i,(struct sockaddr *)&addr,connect_cb);
    }

    uv_run(loop,UV_RUN_DEFAULT);

    return 0;
}
