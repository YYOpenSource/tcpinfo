/*
 * yy_diag	Module for monitoring TCP transport protocols sockets.
 *
 * Authors:	Liu Jingyang of yysec, <liujingyang@yy.com>
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */


#include <linux/module.h>
#include <linux/inet_diag.h>

#include <linux/tcp.h>

#include <net/tcp.h>
#include "yy_diag.h"
static void yy_diag_get_info(struct sock *sk, struct inet_diag_msg *r,
			      void *_info)
{
	const struct tcp_sock *tp = tcp_sk(sk);
	struct yy_tcp_info *info = _info;

	if (sk->sk_state == TCP_LISTEN) {
		r->idiag_rqueue = sk->sk_ack_backlog;
		r->idiag_wqueue = sk->sk_max_ack_backlog;
	} else {
		r->idiag_rqueue = max_t(int, tp->rcv_nxt - tp->copied_seq, 0);
		r->idiag_wqueue = tp->write_seq - tp->snd_una;
	}
	if (info != NULL)
		tcp_get_info(sk, &(info->tcp_info));
	memcpy(&(info->tcp_sock), tp, sizeof(struct tcp_sock));
}

static const struct inet_diag_handler yy_diag_handler = {
	.idiag_hashinfo	 = &tcp_hashinfo,
	.idiag_get_info	 = yy_diag_get_info,
	.idiag_type	 = IPPROTO_TCP,
	.idiag_info_size = sizeof(struct yy_tcp_info),
};

static int __init yy_diag_init(void)
{
	return inet_diag_register(&yy_diag_handler);
}

static void __exit yy_diag_exit(void)
{
	inet_diag_unregister(&yy_diag_handler);
}

module_init(yy_diag_init);
module_exit(yy_diag_exit);
MODULE_LICENSE("GPL");
MODULE_ALIAS_NET_PF_PROTO_TYPE(PF_NETLINK, NETLINK_INET_DIAG, YY_TCPDIAG_GETSOCK);
