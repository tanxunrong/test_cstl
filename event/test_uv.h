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
#define LEELA_BEAT_DURATION (60 * 1000)

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

    uint8_t close;
};

void leela_on_conn(uv_stream_t *server,int status);

struct leela_conn *leela_conn_new(struct leela_sock_server *server);
void leela_conn_close(struct leela_sock_server *server, struct leela_conn *conn);

struct leela_sock_server * leela_server_new(int pub_port,int pri_port);
void leela_server_close(struct leela_sock_server *server);

void leela_on_alloc(uv_handle_t* handle,size_t suggested_size,uv_buf_t* buf);
void leela_on_read(uv_stream_t *client,ssize_t nread,uv_buf_t *buf);

void leela_server_start(struct leela_sock_server *server,uv_connection_cb cb);
void leela_conn_beat(uv_timer_t *timer,int status);
