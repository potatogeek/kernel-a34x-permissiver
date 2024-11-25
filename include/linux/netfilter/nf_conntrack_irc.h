/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_CONNTRACK_IRC_H
#define _NF_CONNTRACK_IRC_H

<<<<<<< HEAD
#ifdef __KERNEL__
=======
#include <linux/netfilter.h>
#include <linux/skbuff.h>
#include <net/netfilter/nf_conntrack_expect.h>
>>>>>>> upstream/android-13

#define IRC_PORT	6667

extern unsigned int (*nf_nat_irc_hook)(struct sk_buff *skb,
				       enum ip_conntrack_info ctinfo,
				       unsigned int protoff,
				       unsigned int matchoff,
				       unsigned int matchlen,
				       struct nf_conntrack_expect *exp);

<<<<<<< HEAD
#endif /* __KERNEL__ */
=======
>>>>>>> upstream/android-13
#endif /* _NF_CONNTRACK_IRC_H */
