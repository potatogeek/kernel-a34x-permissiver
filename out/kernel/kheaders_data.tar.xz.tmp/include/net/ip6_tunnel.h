/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NET_IP6_TUNNEL_H
#define _NET_IP6_TUNNEL_H

#include <linux/ipv6.h>
#include <linux/netdevice.h>
#include <linux/if_tunnel.h>
#include <linux/ip6_tunnel.h>
#include <net/ip_tunnels.h>
#include <net/dst_cache.h>

#define IP6TUNNEL_ERR_TIMEO (30*HZ)


#define IP6_TNL_F_CAP_XMIT 0x10000

#define IP6_TNL_F_CAP_RCV 0x20000

#define IP6_TNL_F_CAP_PER_PACKET 0x40000

struct __ip6_tnl_parm {
	char name[IFNAMSIZ];	
	int link;		
	__u8 proto;		
	__u8 encap_limit;	
	__u8 hop_limit;		
	bool collect_md;
	__be32 flowinfo;	
	__u32 flags;		
	struct in6_addr laddr;	
	struct in6_addr raddr;	

	__be16			i_flags;
	__be16			o_flags;
	__be32			i_key;
	__be32			o_key;

	__u32			fwmark;
	__u32			index;	
	__u8			erspan_ver;	
	__u8			dir;	
	__u16			hwid;	
};


struct ip6_tnl {
	struct ip6_tnl __rcu *next;	
	struct net_device *dev;	
	struct net *net;	
	struct __ip6_tnl_parm parms;	
	struct flowi fl;	
	struct dst_cache dst_cache;	
	struct gro_cells gro_cells;

	int err_count;
	unsigned long err_time;

	
	__u32 i_seqno;	
	__u32 o_seqno;	
	int hlen;       
	int tun_hlen;	
	int encap_hlen; 
	struct ip_tunnel_encap encap;
	int mlink;
};

struct ip6_tnl_encap_ops {
	size_t (*encap_hlen)(struct ip_tunnel_encap *e);
	int (*build_header)(struct sk_buff *skb, struct ip_tunnel_encap *e,
			    u8 *protocol, struct flowi6 *fl6);
};

#ifdef CONFIG_INET

extern const struct ip6_tnl_encap_ops __rcu *
		ip6tun_encaps[MAX_IPTUN_ENCAP_OPS];

int ip6_tnl_encap_add_ops(const struct ip6_tnl_encap_ops *ops,
			  unsigned int num);
int ip6_tnl_encap_del_ops(const struct ip6_tnl_encap_ops *ops,
			  unsigned int num);
int ip6_tnl_encap_setup(struct ip6_tnl *t,
			struct ip_tunnel_encap *ipencap);

static inline int ip6_encap_hlen(struct ip_tunnel_encap *e)
{
	const struct ip6_tnl_encap_ops *ops;
	int hlen = -EINVAL;

	if (e->type == TUNNEL_ENCAP_NONE)
		return 0;

	if (e->type >= MAX_IPTUN_ENCAP_OPS)
		return -EINVAL;

	rcu_read_lock();
	ops = rcu_dereference(ip6tun_encaps[e->type]);
	if (likely(ops && ops->encap_hlen))
		hlen = ops->encap_hlen(e);
	rcu_read_unlock();

	return hlen;
}

static inline int ip6_tnl_encap(struct sk_buff *skb, struct ip6_tnl *t,
				u8 *protocol, struct flowi6 *fl6)
{
	const struct ip6_tnl_encap_ops *ops;
	int ret = -EINVAL;

	if (t->encap.type == TUNNEL_ENCAP_NONE)
		return 0;

	if (t->encap.type >= MAX_IPTUN_ENCAP_OPS)
		return -EINVAL;

	rcu_read_lock();
	ops = rcu_dereference(ip6tun_encaps[t->encap.type]);
	if (likely(ops && ops->build_header))
		ret = ops->build_header(skb, &t->encap, protocol, fl6);
	rcu_read_unlock();

	return ret;
}



struct ipv6_tlv_tnl_enc_lim {
	__u8 type;		
	__u8 length;		
	__u8 encap_limit;	
} __packed;

int ip6_tnl_rcv_ctl(struct ip6_tnl *t, const struct in6_addr *laddr,
		const struct in6_addr *raddr);
int ip6_tnl_rcv(struct ip6_tnl *tunnel, struct sk_buff *skb,
		const struct tnl_ptk_info *tpi, struct metadata_dst *tun_dst,
		bool log_ecn_error);
int ip6_tnl_xmit_ctl(struct ip6_tnl *t, const struct in6_addr *laddr,
		     const struct in6_addr *raddr);
int ip6_tnl_xmit(struct sk_buff *skb, struct net_device *dev, __u8 dsfield,
		 struct flowi6 *fl6, int encap_limit, __u32 *pmtu, __u8 proto);
__u16 ip6_tnl_parse_tlv_enc_lim(struct sk_buff *skb, __u8 *raw);
__u32 ip6_tnl_get_cap(struct ip6_tnl *t, const struct in6_addr *laddr,
			     const struct in6_addr *raddr);
struct net *ip6_tnl_get_link_net(const struct net_device *dev);
int ip6_tnl_get_iflink(const struct net_device *dev);
int ip6_tnl_change_mtu(struct net_device *dev, int new_mtu);

static inline void ip6tunnel_xmit(struct sock *sk, struct sk_buff *skb,
				  struct net_device *dev)
{
	int pkt_len, err;

	memset(skb->cb, 0, sizeof(struct inet6_skb_parm));
	pkt_len = skb->len - skb_inner_network_offset(skb);
	err = ip6_local_out(dev_net(skb_dst(skb)->dev), sk, skb);

	if (dev) {
		if (unlikely(net_xmit_eval(err)))
			pkt_len = -1;
		iptunnel_xmit_stats(dev, pkt_len);
	}
}
#endif
#endif
