#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "TcpInfo.h"
#include <forward_list>
#include <iostream>
//using namespace std;
using std::cout;
using std::endl;
using std::hex;
using std::dec;
using std::cerr;
using std::forward_list;
TcpInfo::TcpInfo()	
{
	nl_seq=0;
	memset(&req, 0, sizeof(DiagReq));
	req.nlh.nlmsg_len = sizeof(DiagReq);
	req.nlh.nlmsg_type = TCPDIAG_GETSOCK;
	req.nlh.nlmsg_flags = NLM_F_ATOMIC| NLM_F_REQUEST;
	req.nlh.nlmsg_pid = 0;
	req.nlh.nlmsg_seq = 0;
	req.r.idiag_family = AF_INET;
    req.r.idiag_src_len = 1;
	req.r.idiag_dst_len = 1;
	req.r.idiag_ext = INET_DIAG_INFO; 
    req.r.idiag_states = 0xff; //states to dump
	

	iov.iov_base = &req;
	iov.iov_len = sizeof(DiagReq); 
	
    dest_addr.nl_family = AF_NETLINK,
    dest_addr.nl_pad=0;
    dest_addr.nl_pid = 0,
    dest_addr.nl_groups = 0;
	memset(&msg,0, sizeof(msg));
	msg.msg_name = (void*)&dest_addr;
	msg.msg_namelen = sizeof(struct sockaddr_nl);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	connectDiag();
}
void TcpInfo::connectDiag(){
		fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_INET_DIAG);
		if(fd < 0){
			cerr<<"creating inet_diag netlink failed"<<endl;	
		}
		int ret = fcntl(fd, F_SETFL, O_NONBLOCK);
    	if (ret < 0) {
        	cerr<<"Can't set socket flags" <<endl;
        	close(fd);
		}
		struct sockaddr_nl src_addr;
		
		src_addr.nl_family = AF_NETLINK;
		src_addr.nl_pid = getpid();
		src_addr.nl_groups = 0;
		

    	if (bind(fd, (struct sockaddr *)&src_addr, sizeof(struct sockaddr_nl)) < 0) {
        	cerr<<"bind socket error "<< strerror(errno) <<endl;
    	}
       	return ;
}
/*
void printSockid(struct inet_diag_sockid& sockid){
	cout<<hex<<"src ip:"<<
}*/
void TcpInfo::print(struct tcp_info& info){
	cout<<"{"<<dec
		<<"\"tcp_state\":"<<static_cast<int>(info.tcpi_state)
		<<",\"tcpi_ca_state\":"<<static_cast<int>(info.tcpi_ca_state)
		<<",\"tcpi_retransmits\":"<<static_cast<int>(info.tcpi_retransmits)
		<<",\"tcpi_probes\":"<<static_cast<int>(info.tcpi_probes)
		<<",\"tcpi_backoff\":"<<static_cast<int>(info.tcpi_backoff)
		<<",\"tcpi_options\":"<<static_cast<int>(info.tcpi_options)
		<<",\"tcpi_snd_wscale\":"<<static_cast<int>(info.tcpi_snd_wscale)
		<<",\"tcpi_rcv_wscale\":"<<static_cast<int>(info.tcpi_rcv_wscale)
		<<",\"tcpi_rto\":"<<info.tcpi_rto
		<<",\"tcpi_ato\":"<<info.tcpi_ato
		<<",\"tcpi_snd_mss\":"<<info.tcpi_snd_mss
		<<",\"tcpi_rcv_mss\":"<<info.tcpi_rcv_mss
		<<",\"tcpi_unacked\":"<<info.tcpi_unacked
		<<",\"tcpi_sacked\":"<<info.tcpi_sacked
		<<",\"tcpi_lost\":"<<info.tcpi_lost
		<<",\"tcpi_retrans\":"<<info.tcpi_retrans
		<<",\"tcpi_fackets\":"<<info.tcpi_fackets
		<<",\"tcpi_last_data_sent\":"<<info.tcpi_last_data_sent
		<<",\"tcpi_last_ack_sent\":"<<info.tcpi_last_ack_sent
		<<",\"tcpi_last_data_recv\":"<<info.tcpi_last_data_recv
		<<",\"tcpi_last_ack_recv\":"<<info.tcpi_last_ack_recv
		<<",\"tcpi_pmtu\":"<<info.tcpi_pmtu
		<<",\"tcpi_rcv_ssthresh\":"<<info.tcpi_rcv_ssthresh
		<<",\"tcpi_rtt\":"<<info.tcpi_rtt
		<<",\"tcpi_rttvar\":"<<info.tcpi_rttvar
		<<",\"tcpi_snd_ssthresh\":"<<info.tcpi_snd_ssthresh
		<<",\"tcpi_snd_cwnd\":"<<info.tcpi_snd_cwnd
		<<",\"tcpi_advmss\":"<<info.tcpi_advmss
		<<",\"tcpi_reordering\":"<<info.tcpi_reordering
		<<",\"tcpi_rcv_rtt\":"<<info.tcpi_rcv_rtt
		<<",\"tcpi_rcv_space\":"<<info.tcpi_rcv_space
		<<",\"tcpi_total_retrans\":"<<info.tcpi_total_retrans
		<<"}"<<endl;
}
std::forward_list<struct tcp_info> TcpInfo::getTcpInfo(struct inet_diag_sockid& sockid){
	iov.iov_base = &req;
	iov.iov_len = sizeof(DiagReq);
	req.r.id = sockid;
	if(sockid.idiag_sport == 0){
		req.nlh.nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;
	}
    if (sendmsg(fd, &msg, 0) < 0) {
        printf("send error:%s\n", strerror(errno));
        return;
    }
	char buf[8192];
	struct iovec rcv_iov={0};
	rcv_iov.iov_base = buf;
	rcv_iov.iov_len = sizeof(buf);

	struct msghdr rcv_msg = {0};
	rcv_msg.msg_name = (void*)&dest_addr;
	rcv_msg.msg_namelen = sizeof(struct sockaddr_nl);
	rcv_msg.msg_iov = &rcv_iov;
	rcv_msg.msg_iovlen = 1;
        //printf("while1\n");
        int status = recvmsg(fd, &rcv_msg, 0);
        printf("status = %d\n", status);
        if (status < 0) {
            cerr<<"errno = "<< strerror(errno)<<endl;
			return;
        }
        if (status == 0) {
            printf("EOF\n");
            return ;
        }
/*	std::cout<<"rcv body:";
	for(uint64_t i=0; i< status; i++){
		if(i % 10 == 0)
			cout<<endl;
		cout<<hex<<uint16_t(buf[i])<<" ";
	}
	cout<<endl; */
	struct nlmsghdr *h  = (struct nlmsghdr *)buf;
	cout<<"nlmshdr:nlmsg_len:"<<dec<<h->nlmsg_len<<".nlmsg_type:"<<h->nlmsg_type<<".nlmsg_flags:"<<h->nlmsg_flags<<".nlmsg_seq:"<<h->nlmsg_seq<<".nlmsg_pid:"<<h->nlmsg_pid<<endl;
	forward_list<struct tcp_info> tcp_infos;
        while (NLMSG_OK(h, status)) {
            //printf("while2\n");
            if (h->nlmsg_type == NLMSG_DONE) {
                printf("NLMSG_DONE\n");
                return ;
            }

            if (h->nlmsg_type == NLMSG_ERROR) {
                struct nlmsgerr *err;
                err = (struct nlmsgerr*)NLMSG_DATA(h);
                fprintf(stderr, "%d Error %d:%s\n", __LINE__, -(err->error), strerror(-(err->error)));
                printf("NLMSG_ERROR\n");
                return ;
            }
	tcp_infos.push_front(*(struct tcp_info*)(buf + sizeof(struct nlmsghdr)));
			h = NLMSG_NEXT(h, status);
            /*struct inet_diag_msg * pkg = (struct inet_diag_msg *)NLMSG_DATA(h);
            memset(src_ip, 0, sizeof(src_ip));
            memset(dest_ip, 0, sizeof(dest_ip));
            inet_ntop(pkg->idiag_family, pkg->id.idiag_src, src_ip, sizeof(src_ip));
            inet_ntop(pkg->idiag_family, pkg->id.idiag_dst, dest_ip, sizeof(dest_ip));
            printf("%-8s %4d %40s:%-6hu %40s:%-6hu\n", pkg->idiag_family == AF_INET ? "AF_INET" : "AF_INET6", pkg->idiag_state
                , src_ip, ntohs(pkg->id.idiag_sport), dest_ip, ntohs(pkg->id.idiag_dport));
            // get_tcp_state(pkg->idiag_state);
            h = NLMSG_NEXT(h, status);
            //printf("status = %d\n\n", status);*/
        }//while
		return tcp_infos;
}
