<<<<<<< HEAD
/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2004 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2004 Netfilter Core Team <coreteam@netfilter.org>
>>>>>>> upstream/android-13
 */

#include <linux/types.h>
#include <linux/ip.h>
#include <linux/netfilter.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <net/netns/generic.h>
#include <net/route.h>
#include <net/ip.h>

#include <linux/netfilter_bridge.h>
#include <linux/netfilter_ipv4.h>
#include <net/netfilter/ipv4/nf_defrag_ipv4.h>
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
#include <net/netfilter/nf_conntrack.h>
#endif
#include <net/netfilter/nf_conntrack_zones.h>

static DEFINE_MUTEX(defrag4_mutex);

static int nf_ct_ipv4_gather_frags(struct net *net, struct sk_buff *skb,
				   u_int32_t user)
{
	int err;

	local_bh_disable();
	err = ip_defrag(net, skb, user);
	local_bh_enable();

	if (!err)
		skb->ignore_df = 1;

	return err;
}

static enum ip_defrag_users nf_ct_defrag_user(unsigned int hooknum,
					      struct sk_buff *skb)
{
	u16 zone_id = NF_CT_DEFAULT_ZONE_ID;
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
	if (skb_nfct(skb)) {
		enum ip_conntrack_info ctinfo;
		const struct nf_conn *ct = nf_ct_get(skb, &ctinfo);

		zone_id = nf_ct_zone_id(nf_ct_zone(ct), CTINFO2DIR(ctinfo));
	}
#endif
	if (nf_bridge_in_prerouting(skb))
		return IP_DEFRAG_CONNTRACK_BRIDGE_IN + zone_id;

	if (hooknum == NF_INET_PRE_ROUTING)
		return IP_DEFRAG_CONNTRACK_IN + zone_id;
	else
		return IP_DEFRAG_CONNTRACK_OUT + zone_id;
}

static unsigned int ipv4_conntrack_defrag(void *priv,
					  struct sk_buff *skb,
					  const struct nf_hook_state *state)
{
	struct sock *sk = skb->sk;

	if (sk && sk_fullsock(sk) && (sk->sk_family == PF_INET) &&
	    inet_sk(sk)->nodefrag)
		return NF_ACCEPT;

#if IS_ENABLED(CONFIG_NF_CONNTRACK)
#if !IS_ENABLED(CONFIG_NF_NAT)
	/* Previously seen (loopback)?  Ignore.  Do this before
	   fragment check. */
	if (skb_nfct(skb) && !nf_ct_is_template((struct nf_conn *)skb_nfct(skb)))
		return NF_ACCEPT;
#endif
	if (skb->_nfct == IP_CT_UNTRACKED)
		return NF_ACCEPT;
#endif
	/* Gather fragments. */
	if (ip_is_fragment(ip_hdr(skb))) {
		enum ip_defrag_users user =
			nf_ct_defrag_user(state->hook, skb);

		if (nf_ct_ipv4_gather_frags(state->net, skb, user))
			return NF_STOLEN;
	}
	return NF_ACCEPT;
}

static const struct nf_hook_ops ipv4_defrag_ops[] = {
	{
		.hook		= ipv4_conntrack_defrag,
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_PRE_ROUTING,
		.priority	= NF_IP_PRI_CONNTRACK_DEFRAG,
	},
	{
		.hook           = ipv4_conntrack_defrag,
		.pf             = NFPROTO_IPV4,
		.hooknum        = NF_INET_LOCAL_OUT,
		.priority       = NF_IP_PRI_CONNTRACK_DEFRAG,
	},
};

static void __net_exit defrag4_net_exit(struct net *net)
{
<<<<<<< HEAD
	if (net->nf.defrag_ipv4) {
		nf_unregister_net_hooks(net, ipv4_defrag_ops,
					ARRAY_SIZE(ipv4_defrag_ops));
		net->nf.defrag_ipv4 = false;
=======
	if (net->nf.defrag_ipv4_users) {
		nf_unregister_net_hooks(net, ipv4_defrag_ops,
					ARRAY_SIZE(ipv4_defrag_ops));
		net->nf.defrag_ipv4_users = 0;
>>>>>>> upstream/android-13
	}
}

static struct pernet_operations defrag4_net_ops = {
	.exit = defrag4_net_exit,
};

static int __init nf_defrag_init(void)
{
	return register_pernet_subsys(&defrag4_net_ops);
}

static void __exit nf_defrag_fini(void)
{
	unregister_pernet_subsys(&defrag4_net_ops);
}

int nf_defrag_ipv4_enable(struct net *net)
{
	int err = 0;

<<<<<<< HEAD
	might_sleep();

	if (net->nf.defrag_ipv4)
		return 0;

	mutex_lock(&defrag4_mutex);
	if (net->nf.defrag_ipv4)
		goto out_unlock;
=======
	mutex_lock(&defrag4_mutex);
	if (net->nf.defrag_ipv4_users == UINT_MAX) {
		err = -EOVERFLOW;
		goto out_unlock;
	}

	if (net->nf.defrag_ipv4_users) {
		net->nf.defrag_ipv4_users++;
		goto out_unlock;
	}
>>>>>>> upstream/android-13

	err = nf_register_net_hooks(net, ipv4_defrag_ops,
				    ARRAY_SIZE(ipv4_defrag_ops));
	if (err == 0)
<<<<<<< HEAD
		net->nf.defrag_ipv4 = true;
=======
		net->nf.defrag_ipv4_users = 1;
>>>>>>> upstream/android-13

 out_unlock:
	mutex_unlock(&defrag4_mutex);
	return err;
}
EXPORT_SYMBOL_GPL(nf_defrag_ipv4_enable);

<<<<<<< HEAD
=======
void nf_defrag_ipv4_disable(struct net *net)
{
	mutex_lock(&defrag4_mutex);
	if (net->nf.defrag_ipv4_users) {
		net->nf.defrag_ipv4_users--;
		if (net->nf.defrag_ipv4_users == 0)
			nf_unregister_net_hooks(net, ipv4_defrag_ops,
						ARRAY_SIZE(ipv4_defrag_ops));
	}

	mutex_unlock(&defrag4_mutex);
}
EXPORT_SYMBOL_GPL(nf_defrag_ipv4_disable);

>>>>>>> upstream/android-13
module_init(nf_defrag_init);
module_exit(nf_defrag_fini);

MODULE_LICENSE("GPL");
