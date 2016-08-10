#include <linux/module.h>
#include <linux/netlink.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <net/genetlink.h>
#include <linux/skbuff.h>
#include "yy_diag.h"
#include <net/tcp.h>
#include <net/net_namespace.h>
static struct nla_policy yy_genl_policy[YY_A_MAX + 1] = {
       [YY_TCP_A_MSG] = { .type = NLA_NUL_STRING },
};

static struct genl_family yy_gnl_family = {
       .id = YY_DIAG_FAMILY_ID,
       .hdrsize = sizeof(struct yy_tcp_sockid),
       .name = "yy_diag",
       .version = 1,
       .maxattr = YY_A_MAX,
 };
static struct yy_tcp_sockid sockid;
/* handler */
static int yy_tcp_getinfo(struct sk_buff *skb, struct netlink_callback *cb)
{
	char* data = skb->data;
	if(sockid.dport == 0){
		//dump
	}
	skb_put(skb, sizeof(struct yy_tcp_info));
	struct sock * sk = inet_lookup(&init_net, &tcp_hashinfo, sockid.dst,
				 sockid.dport, sockid.src,
				 sockid.sport, 0);
	if(sk == NULL)
		return -1;
	struct tcp_sock* tp = tcp_sk(sk);
	struct yy_tcp_info info={0};
	info.rcv_nxt = tp->rcv_nxt;
	info.rcv_wnd = tp->rcv_wnd;
	memcpy(data, &info ,sizeof(struct yy_tcp_info));
	printk("broler:skb len:%d\n",skb->len);
	memset(&sockid, 0 ,sizeof(struct yy_tcp_sockid));
	return skb->len;
}
static int yy_tcp_doit_getinfo(struct sk_buff *skb, struct genl_info *info){
	struct yy_tcp_sockid* _sockid = info->userhdr; 

	printk("receive req with len:%d,sockid:%d:%d,%d:%d\n",skb->len,_sockid->src, _sockid->sport, _sockid->dst, _sockid->dport);
	sockid = *_sockid;
//	skb_put(skb,5);
//	skb->data[0]=3;
//	skb->data[1]=4;
//	skb->data[2]=3;
//    skb->data[3]=4;
//	skb->data[4]=3;
	return 0;
}
 /* operation definition */
 static struct genl_ops yy_gnl_ops_tcp_getinfo = {
       .cmd = YY_TCP_C_GETINFO,
       .flags = 0,
       .policy = yy_genl_policy,
       .doit = yy_tcp_doit_getinfo,
       .dumpit = yy_tcp_getinfo,
 };


static __init void yy_init(void){
	int rc;
	rc = genl_register_family(&yy_gnl_family);
 	rc = genl_register_ops(&yy_gnl_family, &yy_gnl_ops_tcp_getinfo);
}

static void  __exit yy_exit(void){
	genl_unregister_family(&yy_gnl_family);
	genl_unregister_ops(&yy_gnl_family, &yy_gnl_ops_tcp_getinfo);
}

module_init( yy_init);
module_exit( yy_exit);
MODULE_LICENSE("GPL");
