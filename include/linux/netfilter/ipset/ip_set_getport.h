/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _IP_SET_GETPORT_H
#define _IP_SET_GETPORT_H

<<<<<<< HEAD
extern bool ip_set_get_ip4_port(const struct sk_buff *skb, bool src,
				__be16 *port, u8 *proto);

#if defined(CONFIG_IP6_NF_IPTABLES) || defined(CONFIG_IP6_NF_IPTABLES_MODULE)
=======
#include <linux/skbuff.h>
#include <linux/types.h>
#include <uapi/linux/in.h>

extern bool ip_set_get_ip4_port(const struct sk_buff *skb, bool src,
				__be16 *port, u8 *proto);

#if IS_ENABLED(CONFIG_IP6_NF_IPTABLES)
>>>>>>> upstream/android-13
extern bool ip_set_get_ip6_port(const struct sk_buff *skb, bool src,
				__be16 *port, u8 *proto);
#else
static inline bool ip_set_get_ip6_port(const struct sk_buff *skb, bool src,
				       __be16 *port, u8 *proto)
{
	return false;
}
#endif

<<<<<<< HEAD
extern bool ip_set_get_ip_port(const struct sk_buff *skb, u8 pf, bool src,
				__be16 *port);

=======
>>>>>>> upstream/android-13
static inline bool ip_set_proto_with_ports(u8 proto)
{
	switch (proto) {
	case IPPROTO_TCP:
	case IPPROTO_SCTP:
	case IPPROTO_UDP:
	case IPPROTO_UDPLITE:
		return true;
	}
	return false;
}

#endif /*_IP_SET_GETPORT_H*/
