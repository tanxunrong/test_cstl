
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <uv.h>

#define LEELA_ERR(code) \
    do { \
    fprintf(stderr,"error name %s detail %s\n",uv_err_name((code)),uv_strerror((code))); \
    exit(-1);\
    }while(0);
#define LEELA_ID_STEP 10

struct leela_conn {
    int conn_id;  //conn id
    struct sockaddr_in addr; //remote addr
    uv_tcp_t guest; // remote user tcp

    uv_timer_t beat; //heart beat timer
    uint64_t last_beat; //last beat time
    uint8_t status; // status

};

struct leela_sock_server {
    int num; // conn num
    int max_id; // conn max id
    GMutex mutex;
    GHashTable *all_conn;

    struct sockaddr_in door_addr;
    uv_tcp_t door; //public door
    struct sockaddr_in bkdoor_addr;
    uv_tcp_t bkdoor; //private door

    uv_loop_t loop; //loop
};

static struct leela_sock_server *GS = NULL;

void leela_on_conn(uv_stream_t *server,int status);

struct leela_conn *leela_conn_new(struct leela_sock_server *server);
void leela_conn_close(struct leela_sock_server *server, struct leela_conn *conn);

struct leela_sock_server * leela_server_new(int pub_port,int pri_port);
void leela_server_close(struct leela_sock_server *server);

void leela_on_alloc(uv_handle_t* handle,size_t suggested_size,uv_buf_t* buf);
void leela_on_read(uv_stream_t *client,ssize_t nread,uv_buf_t *buf);

/*****************
 * conn
 *****************/
struct leela_conn *leela_conn_new(struct leela_sock_server *server)
{
    struct leela_conn *conn = g_malloc0(sizeof(*conn));

    g_mutex_lock(&server->mutex);

    conn->conn_id = server->max_id + LEELA_ID_STEP;
    g_hash_table_insert(server->all_conn,&conn->conn_id,conn);
    server->max_id = (uint)(server->max_id + LEELA_ID_STEP);
    server->num++;

    g_mutex_unlock(&server->mutex);
}

void leela_conn_close(struct leela_sock_server *server, struct leela_conn *conn)
{
    uv_close(&conn->guest,NULL);

    g_mutex_lock(&server->mutex);
    g_hash_table_remove(server->all_conn,&conn->conn_id);
    server->num--;
    g_mutex_unlock(&server->mutex);
}

/***************
 * server
 ***************/
struct leela_sock_server * leela_server_new(int pub_port,int pri_port)
{
    struct leela_sock_server *server = g_malloc0(sizeof(*server));

    server->all_conn = g_hash_table_new(g_int_hash,g_int_equal);
    g_mutex_init(&server->mutex);

    uv_loop_init(&server->loop);
    uv_tcp_init(&server->loop,&server->door);
    uv_tcp_init(&server->loop,&server->bkdoor);

    int ret;
    // public addr
    ret = uv_ip4_addr("0.0.0.0",pub_port,&server->door_addr);
    if (ret)
        LEELA_ERR(ret);

    // inter addr , 192.168.*.*
    ret = uv_ip4_addr("127.0.0.1",pri_port,&server->bkdoor_addr);
    if (ret)
        LEELA_ERR(ret);

    ret = uv_tcp_bind(&server->door,&server->door_addr,0);
    if (ret)
        LEELA_ERR(ret);

    ret = uv_tcp_bind(&server->bkdoor,&server->bkdoor_addr,0);
    if (ret)
        LEELA_ERR(ret);

    server->max_id = 10000;
    server->num = 0;

    return server;
}

void leela_on_alloc(uv_handle_t* handle,size_t suggested_size,uv_buf_t* buf)
{
    buf->base = g_malloc0(suggested_size);
    buf->len = suggested_size;
}

void leela_on_read(uv_stream_t *client,ssize_t nread,uv_buf_t *buf)
{
    if (nread == UV_EOF)
    {
        uv_close(client,NULL);
    }
    else if (nread > 0)
    {
        fprintf(stderr,"conn read %ld bytes\n",nread);
    }
}

void leela_on_conn(uv_stream_t *door,int status)
{
    if (status == -1)
    {
        LEELA_ERR(status);
    }

    //init conn
    struct leela_conn *conn = leela_conn_new(GS);

    uv_tcp_init(&GS->loop,&conn->guest);

    //accept
    if (uv_accept(door,(struct uv_stream_t *)&conn->guest) == 0)
    {
        uv_tcp_getpeername(conn,&conn->addr,sizeof(conn->addr));
        uv_read_start((struct uv_stream_t *)&conn->guest,leela_on_alloc,leela_on_read);
    }
    else
    {
        leela_conn_close(GS,conn);
    }
}

int main(int argc,char *argv[])
{

    //init server
    struct leela_sock_server *server = leela_server_new(7890,8890);
    GS = server;

    int ret;
    //listen
    ret = uv_listen((uv_stream_t *)&server->door,1<<8,leela_on_conn);
    if (ret)
        LEELA_ERR(ret);

    uv_run(&server->loop,UV_RUN_DEFAULT);

    return 0;
}
