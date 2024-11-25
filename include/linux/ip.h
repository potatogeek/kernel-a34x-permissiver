<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the IP protocol.
 *
 * Version:	@(#)ip.h	1.0.2	04/28/93
 *
 * Authors:	Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
<<<<<<< HEAD
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */
#ifndef _LINUX_IP_H
#define _LINUX_IP_H

#include <linux/skbuff.h>
#include <uapi/linux/ip.h>

static inline struct iphdr *ip_hdr(const struct sk_buff *skb)
{
	return (struct iphdr *)skb_network_header(skb);
}

static inline struct iphdr *inner_ip_hdr(const struct sk_buff *skb)
{
	return (struct iphdr *)skb_inner_network_header(skb);
}

static inline struct iphdr *ipip_hdr(const struct sk_buff *skb)
{
	return (struct iphdr *)skb_transport_header(skb);
}
<<<<<<< HEAD
=======

static inline unsigned int ip_transport_len(const struct sk_buff *skb)
{
	return ntohs(ip_hdr(skb)->tot_len) - skb_network_header_len(skb);
}
>>>>>>> upstream/android-13
#endif	/* _LINUX_IP_H */
