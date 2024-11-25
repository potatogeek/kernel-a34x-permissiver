/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _IPV4_NF_REJECT_H
#define _IPV4_NF_REJECT_H

#include <linux/skbuff.h>
#include <net/ip.h>
#include <net/icmp.h>
<<<<<<< HEAD

void nf_send_unreach(struct sk_buff *skb_in, int code, int hook);
void nf_send_reset(struct net *net, struct sk_buff *oldskb, int hook);

=======
#include <net/netfilter/nf_reject.h>

void nf_send_unreach(struct sk_buff *skb_in, int code, int hook);
void nf_send_reset(struct net *net, struct sock *, struct sk_buff *oldskb,
		   int hook);
>>>>>>> upstream/android-13
const struct tcphdr *nf_reject_ip_tcphdr_get(struct sk_buff *oldskb,
					     struct tcphdr *_oth, int hook);
struct iphdr *nf_reject_iphdr_put(struct sk_buff *nskb,
				  const struct sk_buff *oldskb,
				  __u8 protocol, int ttl);
void nf_reject_ip_tcphdr_put(struct sk_buff *nskb, const struct sk_buff *oldskb,
			     const struct tcphdr *oth);

<<<<<<< HEAD
=======
struct sk_buff *nf_reject_skb_v4_unreach(struct net *net,
                                         struct sk_buff *oldskb,
                                         const struct net_device *dev,
                                         int hook, u8 code);
struct sk_buff *nf_reject_skb_v4_tcp_reset(struct net *net,
					   struct sk_buff *oldskb,
					   const struct net_device *dev,
					   int hook);


>>>>>>> upstream/android-13
#endif /* _IPV4_NF_REJECT_H */
