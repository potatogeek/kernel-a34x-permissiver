<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the ICMP module.
 *
 * Version:	@(#)icmp.h	1.0.4	05/13/93
 *
 * Authors:	Ross Biro
 *		Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
<<<<<<< HEAD
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */
#ifndef _ICMP_H
#define	_ICMP_H

#include <linux/icmp.h>

#include <net/inet_sock.h>
#include <net/snmp.h>
#include <net/ip.h>

struct icmp_err {
  int		errno;
  unsigned int	fatal:1;
};

extern const struct icmp_err icmp_err_convert[];
#define ICMP_INC_STATS(net, field)	SNMP_INC_STATS((net)->mib.icmp_statistics, field)
#define __ICMP_INC_STATS(net, field)	__SNMP_INC_STATS((net)->mib.icmp_statistics, field)
#define ICMPMSGOUT_INC_STATS(net, field)	SNMP_INC_STATS_ATOMIC_LONG((net)->mib.icmpmsg_statistics, field+256)
#define ICMPMSGIN_INC_STATS(net, field)		SNMP_INC_STATS_ATOMIC_LONG((net)->mib.icmpmsg_statistics, field)

struct dst_entry;
struct net_proto_family;
struct sk_buff;
struct net;

void __icmp_send(struct sk_buff *skb_in, int type, int code, __be32 info,
		 const struct ip_options *opt);
static inline void icmp_send(struct sk_buff *skb_in, int type, int code, __be32 info)
{
	__icmp_send(skb_in, type, code, info, &IPCB(skb_in)->opt);
}

#if IS_ENABLED(CONFIG_NF_NAT)
void icmp_ndo_send(struct sk_buff *skb_in, int type, int code, __be32 info);
#else
static inline void icmp_ndo_send(struct sk_buff *skb_in, int type, int code, __be32 info)
{
	struct ip_options opts = { 0 };
	__icmp_send(skb_in, type, code, info, &opts);
}
#endif

int icmp_rcv(struct sk_buff *skb);
<<<<<<< HEAD
void icmp_err(struct sk_buff *skb, u32 info);
int icmp_init(void);
void icmp_out_count(struct net *net, unsigned char type);
=======
int icmp_err(struct sk_buff *skb, u32 info);
int icmp_init(void);
void icmp_out_count(struct net *net, unsigned char type);
bool icmp_build_probe(struct sk_buff *skb, struct icmphdr *icmphdr);
>>>>>>> upstream/android-13

#endif	/* _ICMP_H */
