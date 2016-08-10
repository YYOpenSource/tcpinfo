#ifndef __YY_DIAG_H_
#define __YY_DIAG_H_
#define YY_DIAG_FAMILY_ID 100
enum {
       YY_A_UNSPEC,
       YY_TCP_A_MSG,
       __YY_A_MAX,
};
#define YY_A_MAX (__YY_A_MAX - 1)

 /* commands */
 enum {
       YY_C_UNSPEC,
       YY_TCP_C_GETINFO,
       __YY_C_MAX,
 };
 #define YY_C_MAX (__YY_C_MAX - 1)
struct yy_tcp_sockid {
	__be16	sport;
	__be16	dport;
	__be32	src;
	__be32	dst;
};


struct yy_tcp_info{
	//u64 bytes_received;
	//u32 segs_in;
	__u32 rcv_wnd;
	__u32 rcv_nxt;
	__u32 bytes_acked;
	__u32 total_retrans;	
	__u32 rttvar;
	__u32 srtt;
	struct {
        u32 rtt;  
        u32 seq;  
        u32 time;  
    } rcv_rtt_est;

};

#endif
