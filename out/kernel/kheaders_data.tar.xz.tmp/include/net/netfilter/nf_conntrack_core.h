/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _NF_CONNTRACK_CORE_H
#define _NF_CONNTRACK_CORE_H

#include <linux/netfilter.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_ecache.h>


unsigned int nf_conntrack_in(struct net *net, u_int8_t pf, unsigned int hooknum,
			     struct sk_buff *skb);

int nf_conntrack_init_net(struct net *net);
void nf_conntrack_cleanup_net(struct net *net);
void nf_conntrack_cleanup_net_list(struct list_head *net_exit_list);

int nf_conntrack_proto_pernet_init(struct net *net);
void nf_conntrack_proto_pernet_fini(struct net *net);

int nf_conntrack_proto_init(void);
void nf_conntrack_proto_fini(void);

int nf_conntrack_init_start(void);
void nf_conntrack_cleanup_start(void);

void nf_conntrack_init_end(void);
void nf_conntrack_cleanup_end(void);

bool nf_ct_invert_tuple(struct nf_conntrack_tuple *inverse,
			const struct nf_conntrack_tuple *orig,
			const struct nf_conntrack_l4proto *l4proto);


struct nf_conntrack_tuple_hash *
nf_conntrack_find_get(struct net *net,
		      const struct nf_conntrack_zone *zone,
		      const struct nf_conntrack_tuple *tuple);

int __nf_conntrack_confirm(struct sk_buff *skb);


static inline int nf_conntrack_confirm(struct sk_buff *skb)
{
	struct nf_conn *ct = (struct nf_conn *)skb_nfct(skb);
	int ret = NF_ACCEPT;

	if (ct) {
		if (!nf_ct_is_confirmed(ct))
			ret = __nf_conntrack_confirm(skb);
		if (likely(ret == NF_ACCEPT))
			nf_ct_deliver_cached_events(ct);
	}
	return ret;
}

void print_tuple(struct seq_file *s, const struct nf_conntrack_tuple *tuple,
		 const struct nf_conntrack_l4proto *proto);

#define CONNTRACK_LOCKS 1024

extern spinlock_t nf_conntrack_locks[CONNTRACK_LOCKS];
void nf_conntrack_lock(spinlock_t *lock);

extern spinlock_t nf_conntrack_expect_lock;

#endif 
