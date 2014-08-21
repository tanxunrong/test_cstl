
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <uv.h>

#define LEELA_ERR(code) \
    do { \
    fprintf(stderr,"error name %s detail %s\n",uv_err_name((code)),uv_strerror((code))); \
    exit(-1);\
    }while(0);

struct leela_conn {
    int id;
    int fd;
    struct sockaddr_in addr;
    uv_tcp_t userv;
    uv_buf_t buff;
    int read_len;
    int write_len;
};

struct leela_sock_server {
//    int poll_fd;
    int num;
    uint session_id;
    GMutex mutex;
    GHashTable *all_conn;
    uv_tcp_t userv;
};

static struct leela_sock_server *GS = NULL;

typedef struct {
  uv_write_t req;
  uv_buf_t buf;
} write_req_t;

void leela_on_alloc(uv_handle_t* handle,size_t suggested_size,uv_buf_t* buf)
{
    if (handle->type == UV_TCP)
    {
        *buf = uv_buf_init(g_malloc0(suggested_size),suggested_size);
    }
}

void leela_on_write(uv_write_t *req,int status)
{
    write_req_t *weq;
    weq = (write_req_t *)req;
    if (status)
    {
        LEELA_ERR(status);
    }
    g_assert(weq->req.type == UV_WRITE);
    fprintf(stderr,"conn write %ld\n",weq->buf.len);

    g_free(weq->buf.base);
    g_free(weq);
}

void leela_on_read(uv_stream_t *client,ssize_t nread,uv_buf_t *buf)
{
    if (nread == UV_EOF)
    {
        uv_close((uv_handle_t *)client,NULL);
        fprintf(stderr,"close conn \n");
    }
    else if (nread > 0)
    {
        fprintf(stderr,"conn read %ld bytes\n",nread);
        write_req_t *weq = g_malloc0(sizeof(*weq));
        weq->buf = uv_buf_init(buf->base,nread);
        uv_write(&weq->req,client,&weq->buf,1,leela_on_write);
    }
}

void leela_on_conn(uv_stream_t *server,int status)
{
    if (status == -1)
    {
        LEELA_ERR(status);
    }

    //init conn
    struct leela_conn *conn = g_malloc0(sizeof(*conn));

    g_mutex_lock(&GS->mutex);
    conn->id = GS->session_id + 10;
    g_hash_table_insert(GS->all_conn,&conn->id,conn);
    GS->session_id = (uint)(GS->session_id + 10);
    GS->num++;
    g_mutex_unlock(&GS->mutex);

    uv_tcp_init(uv_default_loop(),&conn->userv);

    //accept
    if (uv_accept(server,(struct uv_stream_t *)&conn->userv) == 0)
    {
        uv_read_start((struct uv_stream_t *)&conn->userv,leela_on_alloc,leela_on_read);
    }
    else
    {
        uv_close(&conn->userv,NULL);
        g_free(conn->buff.base);
        g_mutex_lock(&GS->mutex);
        g_hash_table_remove(GS->all_conn,&conn->id);
        GS->num--;
        g_mutex_unlock(&GS->mutex);
    }
}

int main(int argc,char *argv[])
{
    uv_loop_t *loop = uv_default_loop();

    //init server
    struct leela_sock_server *server = g_malloc0(sizeof(*server));
    server->all_conn = g_hash_table_new(g_int_hash,g_int_equal);
    g_mutex_init(&server->mutex);
    uv_tcp_init(loop,&server->userv);
    server->session_id = 10000;
    GS = server;

    //bind
    struct sockaddr_in bind_addr ;
    int ret = 0;
    if ((ret = uv_ip4_addr("0.0.0.0",7890,&bind_addr)))
    {
        printf("addr wrong : %s\n",uv_err_name(ret));
        exit(-1);
    }

    //uv_tcp init
    uv_tcp_bind(&server->userv,(struct sockaddr *)&bind_addr,0);

    //listen
    ret = uv_listen((uv_stream_t *)&server->userv,1<<8,leela_on_conn);
    if (ret)
    {
        LEELA_ERR(ret)
    }

    //RUN
    uv_run(loop,UV_RUN_DEFAULT);
    return 0;
}
