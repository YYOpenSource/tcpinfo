#include <linux/netlink.h>
#include <linux/inet_diag.h>
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/inet_diag.h>
#include <netinet/tcp.h>
#include <cstdint>
#include <forward_list>
struct DiagReq{
    struct nlmsghdr nlh;
    struct inet_diag_req r;
};
class TcpInfo{
public:
	
	TcpInfo();
	~TcpInfo(){close(fd);}
	std::forward_list<struct tcp_info> getTcpInfo(struct inet_diag_sockid& sockid);
	static void print(struct tcp_info&);
private:
	void connectDiag();
private:
	int fd;
	uint64_t  nl_seq;
	struct DiagReq req;
	struct iovec iov;	
	struct sockaddr_nl dest_addr;
	struct msghdr msg;
};

