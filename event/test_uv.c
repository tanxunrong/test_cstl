
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <uv.h>

#define UTCPT_IN_CONN 1
#define UTCPT_IN_SERVER 2;

#define LEELA_SOCK_ERR(code) \
    do { \
    fprintf(stderr,"error name %s detail %s\n",uv_err_name((code)),uv_strerror((code))); \
    exit(-1);\
    }while(0);
#define LEELA_ID_STEP 10
#define LEELA_FIND_CONN(uvtcpt) (((struct leela_conn_addr *)(uvtcpt)->data)->u.conn)
#define LEELA_FIND_SERVER(uvtcpt) (((struct leela_conn_addr *)(uvtcpt)->data)->u.server)

struct leela_conn;
struct leela_sock_server;

struct leela_conn_addr {
    union {
        struct leela_conn *conn;
        struct leela_sock_server *server;
    } u;
    uint8_t type;
};

struct leela_conn {
    int conn_id;  //conn id
    struct sockaddr_in addr; //remote addr
    uv_tcp_t guest; // remote user tcp

    uv_timer_t beat; //heart beat timer
    uint64_t last_beat; //last beat time
    uint8_t status; // status

    struct leela_sock_server *server;
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
    conn->server = server;
    uv_tcp_init(&server->loop,&conn->guest);

    struct leela_conn_addr *find_addr = g_malloc0(sizeof(*find_addr));
    find_addr->type = UTCPT_IN_CONN;
    find_addr->u.conn = conn;
    conn->guest.data = find_addr;

    fprintf(stderr,"conn id %d create \n",conn->conn_id);
    return conn;
}

void leela_conn_close(struct leela_sock_server *server, struct leela_conn *conn)
{
    uv_close((uv_handle_t *)&conn->guest,NULL);

    g_mutex_lock(&server->mutex);
    g_hash_table_remove(server->all_conn,&conn->conn_id);
    server->num--;
    g_mutex_unlock(&server->mutex);

    fprintf(stderr,"conn id %d close \n",conn->conn_id);
    g_free(conn->guest.data);
    g_free(conn);
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

    struct leela_conn_addr *find_addr_1 = g_malloc0(sizeof(*find_addr_1));
    find_addr_1->type = UTCPT_IN_SERVER;
    find_addr_1->u.server = server;
    server->door.data = find_addr_1;

    struct leela_conn_addr *find_addr_2 = g_malloc0(sizeof(*find_addr_2));
    find_addr_2->type = UTCPT_IN_SERVER;
    find_addr_2->u.server = server;
    server->bkdoor.data = find_addr_2;

    int ret;
    // public addr
    ret = uv_ip4_addr("0.0.0.0",pub_port,&server->door_addr);
    if (ret)
        LEELA_SOCK_ERR(ret);

    // inter addr , 192.168.*.*
    ret = uv_ip4_addr("127.0.0.1",pri_port,&server->bkdoor_addr);
    if (ret)
        LEELA_SOCK_ERR(ret);

    ret = uv_tcp_bind(&server->door,(struct sockaddr *)&server->door_addr,0);
    if (ret)
        LEELA_SOCK_ERR(ret);

    ret = uv_tcp_bind(&server->bkdoor,(struct sockaddr *)&server->bkdoor_addr,0);
    if (ret)
        LEELA_SOCK_ERR(ret);

    server->max_id = 10000;
    server->num = 0;

    return server;
}

void leela_on_alloc(uv_handle_t* handle,size_t suggested_size,uv_buf_t* buf)
{
    if (handle->type == UV_TCP)
    {
        buf->base = g_malloc0(suggested_size);
        buf->len = suggested_size;
    }
}

void leela_on_read(uv_stream_t *client,ssize_t nread,uv_buf_t *buf)
{
    struct leela_conn *conn = LEELA_FIND_CONN(client);
    if (nread == UV_EOF)
    {
        leela_conn_close(conn->server,conn);
    }
    else if (nread > 0)
    {
        fprintf(stderr,"conn %d read %ld bytes\n",conn->conn_id,nread);
        g_free(buf->base);
    }
}

void leela_on_conn(uv_stream_t *door,int status)
{
    if (status == -1)
    {
        LEELA_SOCK_ERR(status);
    }

    struct leela_sock_server *server = LEELA_FIND_SERVER(door);
    g_assert(server);

    //init conn
    struct leela_conn *conn = leela_conn_new(server);

    //accept
    if (uv_accept(door,(struct uv_stream_t *)&conn->guest) == 0)
    {
        int ret;
        int namelen = sizeof(struct sockaddr_in);
        ret = uv_tcp_getpeername(&conn->guest,&conn->addr,&namelen);
        if (ret)
        {
            LEELA_SOCK_ERR(ret);
        }
        uv_read_start((struct uv_stream_t *)&conn->guest,leela_on_alloc,leela_on_read);
    }
    else
    {
        leela_conn_close(server,conn);
    }
}

int main(int argc,char *argv[])
{

    //init server
    struct leela_sock_server *server = leela_server_new(7890,8890);

    int ret;
    //listen
    ret = uv_listen((uv_stream_t *)&server->door,128,leela_on_conn);
    if (ret)
        LEELA_SOCK_ERR(ret);

    uv_run(&server->loop,UV_RUN_DEFAULT);

    return 0;
}
