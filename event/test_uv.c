
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <uv.h>

struct leela_conn {
    int id;
    int fd;
    struct sockaddr_in addr;
    uv_tcp_t userv;
    uv_buf_t buff;
};

struct leela_sock_server {
//    int poll_fd;
    int num;
    uint session_id;
    GMutex mutex;
    GQueue *conn_list;
    uv_tcp_t userv;
};

static struct leela_sock_server *GS = NULL;

void leela_on_read(uv_stream_t)
void leela_on_conn(uv_stream_t *st,int status)
{
    if (status == -1)
    {
        return ;
    }

    struct leela_conn *conn = g_malloc0(sizeof(*conn));
    g_mutex_lock(&GS->mutex);

    g_queue_push_tail(GS->conn_list);
    conn->id = GS->session_id + 100;
    GS->session_id = (uint)(GS->session_id + 100);
    GS->num++;

    g_mutex_unlock(&GS->mutex);

    uv_tcp_init(uv_default_loop(),conn->userv);

    if (uv_accept(st,conn->userv) == 0)
    {
        uv_read_start(conn->userv,NULL,leela_on_read);
    }
}

int main(int argc,char *argv[])
{
    uv_loop_t *loop = uv_default_loop();
    struct leela_sock_server *server = g_malloc0(sizeof(*server));
    server->conn_list = g_queue_new();
    g_mutex_init(&server->mutex);
    uv_tcp_init(loop,&server->userv);
    server->session_id = 10000;
    GS = server;

    struct sockaddr_in bind_addr ;
    int ret = 0;
    if ((ret = uv_ip4_addr("0.0.0.0",7890,&bind_addr)))
    {
        printf("addr wrong : %s\n",uv_err_name(ret));
        exit(-1);
    }

    uv_tcp_bind(&server->userv,&bind_addr,0);

    ret = uv_listen((uv_stream_t *)&server->userv,2<<8,leela_on_conn);
    if (ret)
    {
        printf("listen err %s\n",uv_err_name(ret));
        exit(-1);
    }

    uv_run(loop,UV_RUN_DEFAULT);
    return 0;
}
