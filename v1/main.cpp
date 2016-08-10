#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <algorithm>
#include "TcpInfo.h"
using namespace std;
//src_ip src_port dst_ip dst_port
int main(int argc, char* argv[]){
	TcpInfo tcpInfo;
	struct inet_diag_sockid sockid={0};
	if(argc >= 5){
		sockid.idiag_src[0] = inet_addr( argv[1]);
		sockid.idiag_src[1] = 0;
		sockid.idiag_src[2] = 0;
		sockid.idiag_src[3] = 0;
		sockid.idiag_sport = htons(atoi(argv[2]));
		sockid.idiag_dst[0] = inet_addr( argv[3]);
		sockid.idiag_dst[1] = 0;
		sockid.idiag_dst[2] = 0;
		sockid.idiag_dst[3] = 0;
		sockid.idiag_dport = htons(atoi(argv[4]));
		sockid.idiag_if = 0;
		sockid.idiag_cookie[0] = INET_DIAG_NOCOOKIE;
		sockid.idiag_cookie[1] = INET_DIAG_NOCOOKIE;
	}
	cout<<"search sockid:"<<hex<<sockid.idiag_src[0]<<":"<<sockid.idiag_sport<<" "<<sockid.idiag_dst[0]<<":"<< sockid.idiag_dport<<endl;
	forward_list<struct tcp_info> infos = tcpInfo.getTcpInfo(sockid);
	for_each(infos.begin(),infos.end(), TcpInfo::print);
}
