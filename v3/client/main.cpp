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
	struct yy_tcp_sockid sockid={0};
	if(argc >= 5){
		sockid.src = inet_addr( argv[1]);
		sockid.sport = htons(atoi(argv[2]));
		sockid.dst = inet_addr( argv[3]);
		sockid.dport = htons(atoi(argv[4]));
	}
	cout<<"search sockid:"<<hex<<sockid.src<<":"<<sockid.sport<<" "<<sockid.dst<<":"<< sockid.dport<<endl;
	forward_list<struct yy_tcp_info> infos = tcpInfo.getTcpInfo(sockid);
	for_each(infos.begin(),infos.end(), TcpInfo::print);
}
