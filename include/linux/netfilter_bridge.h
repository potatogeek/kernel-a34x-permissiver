/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_BRIDGE_NETFILTER_H
#define __LINUX_BRIDGE_NETFILTER_H

#include <uapi/linux/netfilter_bridge.h>
#include <linux/skbuff.h>

<<<<<<< HEAD
=======
struct nf_bridge_frag_data {
	char    mac[ETH_HLEN];
	bool    vlan_present;
	u16     vlan_tci;
	__be16  vlan_proto;
};

>>>>>>> upstream/android-13
#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)

int br_handle_frame_finish(struct net *net, struct sock *sk, struct sk_buff *skb);

static inline void br_drop_fake_rtable(struct sk_buff *skb)
{
	struct dst_entry *dst = skb_dst(skb);

	if (dst && (dst->flags & DST_FAKE_RTABLE))
		skb_dst_drop(skb);
}

<<<<<<< HEAD
static inline int nf_bridge_get_physinif(const struct sk_buff *skb)
{
	struct nf_bridge_info *nf_bridge;

	if (skb->nf_bridge == NULL)
		return 0;

	nf_bridge = skb->nf_bridge;
=======
static inline struct nf_bridge_info *
nf_bridge_info_get(const struct sk_buff *skb)
{
	return skb_ext_find(skb, SKB_EXT_BRIDGE_NF);
}

static inline bool nf_bridge_info_exists(const struct sk_buff *skb)
{
	return skb_ext_exist(skb, SKB_EXT_BRIDGE_NF);
}

static inline int nf_bridge_get_physinif(const struct sk_buff *skb)
{
	const struct nf_bridge_info *nf_bridge = nf_bridge_info_get(skb);

	if (!nf_bridge)
		return 0;

>>>>>>> upstream/android-13
	return nf_bridge->physindev ? nf_bridge->physindev->ifindex : 0;
}

static inline int nf_bridge_get_physoutif(const struct sk_buff *skb)
{
<<<<<<< HEAD
	struct nf_bridge_info *nf_bridge;

	if (skb->nf_bridge == NULL)
		return 0;

	nf_bridge = skb->nf_bridge;
=======
	const struct nf_bridge_info *nf_bridge = nf_bridge_info_get(skb);

	if (!nf_bridge)
		return 0;

>>>>>>> upstream/android-13
	return nf_bridge->physoutdev ? nf_bridge->physoutdev->ifindex : 0;
}

static inline struct net_device *
nf_bridge_get_physindev(const struct sk_buff *skb)
{
<<<<<<< HEAD
	return skb->nf_bridge ? skb->nf_bridge->physindev : NULL;
=======
	const struct nf_bridge_info *nf_bridge = nf_bridge_info_get(skb);

	return nf_bridge ? nf_bridge->physindev : NULL;
>>>>>>> upstream/android-13
}

static inline struct net_device *
nf_bridge_get_physoutdev(const struct sk_buff *skb)
{
<<<<<<< HEAD
	return skb->nf_bridge ? skb->nf_bridge->physoutdev : NULL;
=======
	const struct nf_bridge_info *nf_bridge = nf_bridge_info_get(skb);

	return nf_bridge ? nf_bridge->physoutdev : NULL;
>>>>>>> upstream/android-13
}

static inline bool nf_bridge_in_prerouting(const struct sk_buff *skb)
{
<<<<<<< HEAD
	return skb->nf_bridge && skb->nf_bridge->in_prerouting;
=======
	const struct nf_bridge_info *nf_bridge = nf_bridge_info_get(skb);

	return nf_bridge && nf_bridge->in_prerouting;
>>>>>>> upstream/android-13
}
#else
#define br_drop_fake_rtable(skb)	        do { } while (0)
static inline bool nf_bridge_in_prerouting(const struct sk_buff *skb)
{
	return false;
}
#endif /* CONFIG_BRIDGE_NETFILTER */

#endif
