
#include <linux/tcp.h>

#include <net/tcp.h>
#define YY_TCPDIAG_GETSOCK 100

struct yy_tcp_info{
	struct tcp_info tcp_info;
	struct tcp_sock tcp_sock;
}; 
