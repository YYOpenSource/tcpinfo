#ifndef __TCPINFO_H_
#define __TCPINFO_H_
#include <linux/netlink.h>
#include <linux/inet_diag.h>
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/inet_diag.h>
#include <netinet/tcp.h>
#include <cstdint>
#include <forward_list>
#include <linux/genetlink.h>
#include "../yy_diag.h"
struct DiagReq{
    struct nlmsghdr nlh;
    struct genlmsghdr gnlh;
	struct yy_tcp_sockid id; 
};
class TcpInfo{
public:
	
	TcpInfo();
	~TcpInfo(){close(fd);}
	std::forward_list<struct yy_tcp_info> getTcpInfo(struct yy_tcp_sockid& sockid);
	static void print(struct yy_tcp_info&);
private:
	void connectDiag();
	void setSockToDump(struct yy_tcp_sockid & sockid);
private:
	int fd;
	uint64_t  nl_seq;
	struct DiagReq req;
	struct iovec iov;	
	struct sockaddr_nl dest_addr;
	struct msghdr msg;
};
#endif
