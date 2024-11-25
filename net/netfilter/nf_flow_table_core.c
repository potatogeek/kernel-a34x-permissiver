<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/rhashtable.h>
#include <linux/netdevice.h>
#include <net/ip.h>
#include <net/ip6_route.h>
#include <net/netfilter/nf_tables.h>
#include <net/netfilter/nf_flow_table.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
<<<<<<< HEAD
#include <net/netfilter/nf_conntrack_tuple.h>

struct flow_offload_entry {
	struct flow_offload	flow;
	struct nf_conn		*ct;
	struct rcu_head		rcu_head;
};

=======
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_tuple.h>

>>>>>>> upstream/android-13
static DEFINE_MUTEX(flowtable_lock);
static LIST_HEAD(flowtables);

static void
<<<<<<< HEAD
flow_offload_fill_dir(struct flow_offload *flow, struct nf_conn *ct,
		      struct nf_flow_route *route,
		      enum flow_offload_tuple_dir dir)
{
	struct flow_offload_tuple *ft = &flow->tuplehash[dir].tuple;
	struct nf_conntrack_tuple *ctt = &ct->tuplehash[dir].tuple;
	struct dst_entry *other_dst = route->tuple[!dir].dst;
	struct dst_entry *dst = route->tuple[dir].dst;
=======
flow_offload_fill_dir(struct flow_offload *flow,
		      enum flow_offload_tuple_dir dir)
{
	struct flow_offload_tuple *ft = &flow->tuplehash[dir].tuple;
	struct nf_conntrack_tuple *ctt = &flow->ct->tuplehash[dir].tuple;
>>>>>>> upstream/android-13

	ft->dir = dir;

	switch (ctt->src.l3num) {
	case NFPROTO_IPV4:
		ft->src_v4 = ctt->src.u3.in;
		ft->dst_v4 = ctt->dst.u3.in;
<<<<<<< HEAD
		ft->mtu = ip_dst_mtu_maybe_forward(dst, true);
=======
>>>>>>> upstream/android-13
		break;
	case NFPROTO_IPV6:
		ft->src_v6 = ctt->src.u3.in6;
		ft->dst_v6 = ctt->dst.u3.in6;
<<<<<<< HEAD
		ft->mtu = ip6_dst_mtu_forward(dst);
=======
>>>>>>> upstream/android-13
		break;
	}

	ft->l3proto = ctt->src.l3num;
	ft->l4proto = ctt->dst.protonum;
	ft->src_port = ctt->src.u.tcp.port;
	ft->dst_port = ctt->dst.u.tcp.port;
<<<<<<< HEAD

	ft->iifidx = other_dst->dev->ifindex;
	ft->oifidx = dst->dev->ifindex;
	ft->dst_cache = dst;
}

struct flow_offload *
flow_offload_alloc(struct nf_conn *ct, struct nf_flow_route *route)
{
	struct flow_offload_entry *entry;
	struct flow_offload *flow;

	if (unlikely(nf_ct_is_dying(ct) ||
	    !atomic_inc_not_zero(&ct->ct_general.use)))
		return NULL;

	entry = kzalloc(sizeof(*entry), GFP_ATOMIC);
	if (!entry)
		goto err_ct_refcnt;

	flow = &entry->flow;

	if (!dst_hold_safe(route->tuple[FLOW_OFFLOAD_DIR_ORIGINAL].dst))
		goto err_dst_cache_original;

	if (!dst_hold_safe(route->tuple[FLOW_OFFLOAD_DIR_REPLY].dst))
		goto err_dst_cache_reply;

	entry->ct = ct;

	flow_offload_fill_dir(flow, ct, route, FLOW_OFFLOAD_DIR_ORIGINAL);
	flow_offload_fill_dir(flow, ct, route, FLOW_OFFLOAD_DIR_REPLY);

	if (ct->status & IPS_SRC_NAT)
		flow->flags |= FLOW_OFFLOAD_SNAT;
	if (ct->status & IPS_DST_NAT)
		flow->flags |= FLOW_OFFLOAD_DNAT;

	return flow;

err_dst_cache_reply:
	dst_release(route->tuple[FLOW_OFFLOAD_DIR_ORIGINAL].dst);
err_dst_cache_original:
	kfree(entry);
=======
}

struct flow_offload *flow_offload_alloc(struct nf_conn *ct)
{
	struct flow_offload *flow;

	if (unlikely(nf_ct_is_dying(ct) ||
	    !refcount_inc_not_zero(&ct->ct_general.use)))
		return NULL;

	flow = kzalloc(sizeof(*flow), GFP_ATOMIC);
	if (!flow)
		goto err_ct_refcnt;

	flow->ct = ct;

	flow_offload_fill_dir(flow, FLOW_OFFLOAD_DIR_ORIGINAL);
	flow_offload_fill_dir(flow, FLOW_OFFLOAD_DIR_REPLY);

	if (ct->status & IPS_SRC_NAT)
		__set_bit(NF_FLOW_SNAT, &flow->flags);
	if (ct->status & IPS_DST_NAT)
		__set_bit(NF_FLOW_DNAT, &flow->flags);

	return flow;

>>>>>>> upstream/android-13
err_ct_refcnt:
	nf_ct_put(ct);

	return NULL;
}
EXPORT_SYMBOL_GPL(flow_offload_alloc);

<<<<<<< HEAD
=======
static u32 flow_offload_dst_cookie(struct flow_offload_tuple *flow_tuple)
{
	const struct rt6_info *rt;

	if (flow_tuple->l3proto == NFPROTO_IPV6) {
		rt = (const struct rt6_info *)flow_tuple->dst_cache;
		return rt6_get_cookie(rt);
	}

	return 0;
}

static int flow_offload_fill_route(struct flow_offload *flow,
				   const struct nf_flow_route *route,
				   enum flow_offload_tuple_dir dir)
{
	struct flow_offload_tuple *flow_tuple = &flow->tuplehash[dir].tuple;
	struct dst_entry *dst = route->tuple[dir].dst;
	int i, j = 0;

	switch (flow_tuple->l3proto) {
	case NFPROTO_IPV4:
		flow_tuple->mtu = ip_dst_mtu_maybe_forward(dst, true);
		break;
	case NFPROTO_IPV6:
		flow_tuple->mtu = ip6_dst_mtu_maybe_forward(dst, true);
		break;
	}

	flow_tuple->iifidx = route->tuple[dir].in.ifindex;
	for (i = route->tuple[dir].in.num_encaps - 1; i >= 0; i--) {
		flow_tuple->encap[j].id = route->tuple[dir].in.encap[i].id;
		flow_tuple->encap[j].proto = route->tuple[dir].in.encap[i].proto;
		if (route->tuple[dir].in.ingress_vlans & BIT(i))
			flow_tuple->in_vlan_ingress |= BIT(j);
		j++;
	}
	flow_tuple->encap_num = route->tuple[dir].in.num_encaps;

	switch (route->tuple[dir].xmit_type) {
	case FLOW_OFFLOAD_XMIT_DIRECT:
		memcpy(flow_tuple->out.h_dest, route->tuple[dir].out.h_dest,
		       ETH_ALEN);
		memcpy(flow_tuple->out.h_source, route->tuple[dir].out.h_source,
		       ETH_ALEN);
		flow_tuple->out.ifidx = route->tuple[dir].out.ifindex;
		flow_tuple->out.hw_ifidx = route->tuple[dir].out.hw_ifindex;
		break;
	case FLOW_OFFLOAD_XMIT_XFRM:
	case FLOW_OFFLOAD_XMIT_NEIGH:
		if (!dst_hold_safe(route->tuple[dir].dst))
			return -1;

		flow_tuple->dst_cache = dst;
		flow_tuple->dst_cookie = flow_offload_dst_cookie(flow_tuple);
		break;
	default:
		WARN_ON_ONCE(1);
		break;
	}
	flow_tuple->xmit_type = route->tuple[dir].xmit_type;

	return 0;
}

static void nft_flow_dst_release(struct flow_offload *flow,
				 enum flow_offload_tuple_dir dir)
{
	if (flow->tuplehash[dir].tuple.xmit_type == FLOW_OFFLOAD_XMIT_NEIGH ||
	    flow->tuplehash[dir].tuple.xmit_type == FLOW_OFFLOAD_XMIT_XFRM)
		dst_release(flow->tuplehash[dir].tuple.dst_cache);
}

int flow_offload_route_init(struct flow_offload *flow,
			    const struct nf_flow_route *route)
{
	int err;

	err = flow_offload_fill_route(flow, route, FLOW_OFFLOAD_DIR_ORIGINAL);
	if (err < 0)
		return err;

	err = flow_offload_fill_route(flow, route, FLOW_OFFLOAD_DIR_REPLY);
	if (err < 0)
		goto err_route_reply;

	flow->type = NF_FLOW_OFFLOAD_ROUTE;

	return 0;

err_route_reply:
	nft_flow_dst_release(flow, FLOW_OFFLOAD_DIR_ORIGINAL);

	return err;
}
EXPORT_SYMBOL_GPL(flow_offload_route_init);

>>>>>>> upstream/android-13
static void flow_offload_fixup_tcp(struct ip_ct_tcp *tcp)
{
	tcp->state = TCP_CONNTRACK_ESTABLISHED;
	tcp->seen[0].td_maxwin = 0;
	tcp->seen[1].td_maxwin = 0;
}

<<<<<<< HEAD
#define NF_FLOWTABLE_TCP_PICKUP_TIMEOUT	(120 * HZ)
#define NF_FLOWTABLE_UDP_PICKUP_TIMEOUT	(30 * HZ)

static void flow_offload_fixup_ct_state(struct nf_conn *ct)
{
	const struct nf_conntrack_l4proto *l4proto;
	unsigned int timeout;
	int l4num;

	l4num = nf_ct_protonum(ct);
	if (l4num == IPPROTO_TCP)
		flow_offload_fixup_tcp(&ct->proto.tcp);

	l4proto = __nf_ct_l4proto_find(nf_ct_l3num(ct), l4num);
	if (!l4proto)
		return;

	if (l4num == IPPROTO_TCP)
		timeout = NF_FLOWTABLE_TCP_PICKUP_TIMEOUT;
	else if (l4num == IPPROTO_UDP)
		timeout = NF_FLOWTABLE_UDP_PICKUP_TIMEOUT;
	else
		return;

	ct->timeout = nfct_time_stamp + timeout;
=======
static void flow_offload_fixup_ct_timeout(struct nf_conn *ct)
{
	struct net *net = nf_ct_net(ct);
	int l4num = nf_ct_protonum(ct);
	s32 timeout;

	if (l4num == IPPROTO_TCP) {
		struct nf_tcp_net *tn = nf_tcp_pernet(net);

		timeout = tn->timeouts[TCP_CONNTRACK_ESTABLISHED];
		timeout -= tn->offload_timeout;
	} else if (l4num == IPPROTO_UDP) {
		struct nf_udp_net *tn = nf_udp_pernet(net);

		timeout = tn->timeouts[UDP_CT_REPLIED];
		timeout -= tn->offload_timeout;
	} else {
		return;
	}

	if (timeout < 0)
		timeout = 0;

	if (nf_flow_timeout_delta(READ_ONCE(ct->timeout)) > (__s32)timeout)
		WRITE_ONCE(ct->timeout, nfct_time_stamp + timeout);
}

static void flow_offload_fixup_ct_state(struct nf_conn *ct)
{
	if (nf_ct_protonum(ct) == IPPROTO_TCP)
		flow_offload_fixup_tcp(&ct->proto.tcp);
}

static void flow_offload_fixup_ct(struct nf_conn *ct)
{
	flow_offload_fixup_ct_state(ct);
	flow_offload_fixup_ct_timeout(ct);
}

static void flow_offload_route_release(struct flow_offload *flow)
{
	nft_flow_dst_release(flow, FLOW_OFFLOAD_DIR_ORIGINAL);
	nft_flow_dst_release(flow, FLOW_OFFLOAD_DIR_REPLY);
>>>>>>> upstream/android-13
}

void flow_offload_free(struct flow_offload *flow)
{
<<<<<<< HEAD
	struct flow_offload_entry *e;

	dst_release(flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.dst_cache);
	dst_release(flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple.dst_cache);
	e = container_of(flow, struct flow_offload_entry, flow);
	if (flow->flags & FLOW_OFFLOAD_DYING)
		nf_ct_delete(e->ct, 0, 0);
	nf_ct_put(e->ct);
	kfree_rcu(e, rcu_head);
=======
	switch (flow->type) {
	case NF_FLOW_OFFLOAD_ROUTE:
		flow_offload_route_release(flow);
		break;
	default:
		break;
	}
	nf_ct_put(flow->ct);
	kfree_rcu(flow, rcu_head);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(flow_offload_free);

static u32 flow_offload_hash(const void *data, u32 len, u32 seed)
{
	const struct flow_offload_tuple *tuple = data;

<<<<<<< HEAD
	return jhash(tuple, offsetof(struct flow_offload_tuple, dir), seed);
=======
	return jhash(tuple, offsetof(struct flow_offload_tuple, __hash), seed);
>>>>>>> upstream/android-13
}

static u32 flow_offload_hash_obj(const void *data, u32 len, u32 seed)
{
	const struct flow_offload_tuple_rhash *tuplehash = data;

<<<<<<< HEAD
	return jhash(&tuplehash->tuple, offsetof(struct flow_offload_tuple, dir), seed);
=======
	return jhash(&tuplehash->tuple, offsetof(struct flow_offload_tuple, __hash), seed);
>>>>>>> upstream/android-13
}

static int flow_offload_hash_cmp(struct rhashtable_compare_arg *arg,
					const void *ptr)
{
	const struct flow_offload_tuple *tuple = arg->key;
	const struct flow_offload_tuple_rhash *x = ptr;

<<<<<<< HEAD
	if (memcmp(&x->tuple, tuple, offsetof(struct flow_offload_tuple, dir)))
=======
	if (memcmp(&x->tuple, tuple, offsetof(struct flow_offload_tuple, __hash)))
>>>>>>> upstream/android-13
		return 1;

	return 0;
}

static const struct rhashtable_params nf_flow_offload_rhash_params = {
	.head_offset		= offsetof(struct flow_offload_tuple_rhash, node),
	.hashfn			= flow_offload_hash,
	.obj_hashfn		= flow_offload_hash_obj,
	.obj_cmpfn		= flow_offload_hash_cmp,
	.automatic_shrinking	= true,
};

<<<<<<< HEAD
=======
unsigned long flow_offload_get_timeout(struct flow_offload *flow)
{
	unsigned long timeout = NF_FLOW_TIMEOUT;
	struct net *net = nf_ct_net(flow->ct);
	int l4num = nf_ct_protonum(flow->ct);

	if (l4num == IPPROTO_TCP) {
		struct nf_tcp_net *tn = nf_tcp_pernet(net);

		timeout = tn->offload_timeout;
	} else if (l4num == IPPROTO_UDP) {
		struct nf_udp_net *tn = nf_udp_pernet(net);

		timeout = tn->offload_timeout;
	}

	return timeout;
}

>>>>>>> upstream/android-13
int flow_offload_add(struct nf_flowtable *flow_table, struct flow_offload *flow)
{
	int err;

<<<<<<< HEAD
	flow->timeout = (u32)jiffies + NF_FLOW_TIMEOUT;
=======
	flow->timeout = nf_flowtable_time_stamp + flow_offload_get_timeout(flow);
>>>>>>> upstream/android-13

	err = rhashtable_insert_fast(&flow_table->rhashtable,
				     &flow->tuplehash[0].node,
				     nf_flow_offload_rhash_params);
	if (err < 0)
		return err;

	err = rhashtable_insert_fast(&flow_table->rhashtable,
				     &flow->tuplehash[1].node,
				     nf_flow_offload_rhash_params);
	if (err < 0) {
		rhashtable_remove_fast(&flow_table->rhashtable,
				       &flow->tuplehash[0].node,
				       nf_flow_offload_rhash_params);
		return err;
	}

<<<<<<< HEAD
=======
	nf_ct_offload_timeout(flow->ct);

	if (nf_flowtable_hw_offload(flow_table)) {
		__set_bit(NF_FLOW_HW, &flow->flags);
		nf_flow_offload_add(flow_table, flow);
	}

>>>>>>> upstream/android-13
	return 0;
}
EXPORT_SYMBOL_GPL(flow_offload_add);

<<<<<<< HEAD
static void flow_offload_del(struct nf_flowtable *flow_table,
			     struct flow_offload *flow)
{
	struct flow_offload_entry *e;

=======
void flow_offload_refresh(struct nf_flowtable *flow_table,
			  struct flow_offload *flow)
{
	u32 timeout;

	timeout = nf_flowtable_time_stamp + flow_offload_get_timeout(flow);
	if (READ_ONCE(flow->timeout) != timeout)
		WRITE_ONCE(flow->timeout, timeout);

	if (likely(!nf_flowtable_hw_offload(flow_table)))
		return;

	nf_flow_offload_add(flow_table, flow);
}
EXPORT_SYMBOL_GPL(flow_offload_refresh);

static inline bool nf_flow_has_expired(const struct flow_offload *flow)
{
	return nf_flow_timeout_delta(flow->timeout) <= 0;
}

static void flow_offload_del(struct nf_flowtable *flow_table,
			     struct flow_offload *flow)
{
>>>>>>> upstream/android-13
	rhashtable_remove_fast(&flow_table->rhashtable,
			       &flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].node,
			       nf_flow_offload_rhash_params);
	rhashtable_remove_fast(&flow_table->rhashtable,
			       &flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].node,
			       nf_flow_offload_rhash_params);

<<<<<<< HEAD
	e = container_of(flow, struct flow_offload_entry, flow);
	clear_bit(IPS_OFFLOAD_BIT, &e->ct->status);
=======
	clear_bit(IPS_OFFLOAD_BIT, &flow->ct->status);

	if (nf_flow_has_expired(flow))
		flow_offload_fixup_ct(flow->ct);
	else
		flow_offload_fixup_ct_timeout(flow->ct);
>>>>>>> upstream/android-13

	flow_offload_free(flow);
}

void flow_offload_teardown(struct flow_offload *flow)
{
<<<<<<< HEAD
	struct flow_offload_entry *e;

	flow->flags |= FLOW_OFFLOAD_TEARDOWN;

	e = container_of(flow, struct flow_offload_entry, flow);
	flow_offload_fixup_ct_state(e->ct);
=======
	set_bit(NF_FLOW_TEARDOWN, &flow->flags);

	flow_offload_fixup_ct_state(flow->ct);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(flow_offload_teardown);

struct flow_offload_tuple_rhash *
flow_offload_lookup(struct nf_flowtable *flow_table,
		    struct flow_offload_tuple *tuple)
{
	struct flow_offload_tuple_rhash *tuplehash;
	struct flow_offload *flow;
	int dir;

<<<<<<< HEAD
	tuplehash = rhashtable_lookup_fast(&flow_table->rhashtable, tuple,
					   nf_flow_offload_rhash_params);
=======
	tuplehash = rhashtable_lookup(&flow_table->rhashtable, tuple,
				      nf_flow_offload_rhash_params);
>>>>>>> upstream/android-13
	if (!tuplehash)
		return NULL;

	dir = tuplehash->tuple.dir;
	flow = container_of(tuplehash, struct flow_offload, tuplehash[dir]);
<<<<<<< HEAD
	if (flow->flags & (FLOW_OFFLOAD_DYING | FLOW_OFFLOAD_TEARDOWN))
=======
	if (test_bit(NF_FLOW_TEARDOWN, &flow->flags))
		return NULL;

	if (unlikely(nf_ct_is_dying(flow->ct)))
>>>>>>> upstream/android-13
		return NULL;

	return tuplehash;
}
EXPORT_SYMBOL_GPL(flow_offload_lookup);

<<<<<<< HEAD
int nf_flow_table_iterate(struct nf_flowtable *flow_table,
			  void (*iter)(struct flow_offload *flow, void *data),
			  void *data)
=======
static int
nf_flow_table_iterate(struct nf_flowtable *flow_table,
		      void (*iter)(struct flow_offload *flow, void *data),
		      void *data)
>>>>>>> upstream/android-13
{
	struct flow_offload_tuple_rhash *tuplehash;
	struct rhashtable_iter hti;
	struct flow_offload *flow;
<<<<<<< HEAD
	int err;

	err = rhashtable_walk_init(&flow_table->rhashtable, &hti, GFP_KERNEL);
	if (err)
		return err;

=======
	int err = 0;

	rhashtable_walk_enter(&flow_table->rhashtable, &hti);
>>>>>>> upstream/android-13
	rhashtable_walk_start(&hti);

	while ((tuplehash = rhashtable_walk_next(&hti))) {
		if (IS_ERR(tuplehash)) {
<<<<<<< HEAD
			err = PTR_ERR(tuplehash);
			if (err != -EAGAIN)
				goto out;

=======
			if (PTR_ERR(tuplehash) != -EAGAIN) {
				err = PTR_ERR(tuplehash);
				break;
			}
>>>>>>> upstream/android-13
			continue;
		}
		if (tuplehash->tuple.dir)
			continue;

		flow = container_of(tuplehash, struct flow_offload, tuplehash[0]);

		iter(flow, data);
	}
<<<<<<< HEAD
out:
=======
>>>>>>> upstream/android-13
	rhashtable_walk_stop(&hti);
	rhashtable_walk_exit(&hti);

	return err;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(nf_flow_table_iterate);

static inline bool nf_flow_has_expired(const struct flow_offload *flow)
{
	return (__s32)(flow->timeout - (u32)jiffies) <= 0;
}

static int nf_flow_offload_gc_step(struct nf_flowtable *flow_table)
{
	struct flow_offload_tuple_rhash *tuplehash;
	struct rhashtable_iter hti;
	struct flow_offload *flow;
	int err;

	err = rhashtable_walk_init(&flow_table->rhashtable, &hti, GFP_KERNEL);
	if (err)
		return 0;

	rhashtable_walk_start(&hti);

	while ((tuplehash = rhashtable_walk_next(&hti))) {
		if (IS_ERR(tuplehash)) {
			err = PTR_ERR(tuplehash);
			if (err != -EAGAIN)
				goto out;

			continue;
		}
		if (tuplehash->tuple.dir)
			continue;

		flow = container_of(tuplehash, struct flow_offload, tuplehash[0]);

		if (nf_flow_has_expired(flow) ||
		    (flow->flags & (FLOW_OFFLOAD_DYING |
				    FLOW_OFFLOAD_TEARDOWN)))
			flow_offload_del(flow_table, flow);
	}
out:
	rhashtable_walk_stop(&hti);
	rhashtable_walk_exit(&hti);

	return 1;
=======

static bool flow_offload_stale_dst(struct flow_offload_tuple *tuple)
{
	struct dst_entry *dst;

	if (tuple->xmit_type == FLOW_OFFLOAD_XMIT_NEIGH ||
	    tuple->xmit_type == FLOW_OFFLOAD_XMIT_XFRM) {
		dst = tuple->dst_cache;
		if (!dst_check(dst, tuple->dst_cookie))
			return true;
	}

	return false;
}

static bool nf_flow_has_stale_dst(struct flow_offload *flow)
{
	return flow_offload_stale_dst(&flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple) ||
	       flow_offload_stale_dst(&flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple);
}

static void nf_flow_offload_gc_step(struct flow_offload *flow, void *data)
{
	struct nf_flowtable *flow_table = data;

	if (nf_flow_has_expired(flow) ||
	    nf_ct_is_dying(flow->ct) ||
	    nf_flow_has_stale_dst(flow))
		set_bit(NF_FLOW_TEARDOWN, &flow->flags);

	if (test_bit(NF_FLOW_TEARDOWN, &flow->flags)) {
		if (test_bit(NF_FLOW_HW, &flow->flags)) {
			if (!test_bit(NF_FLOW_HW_DYING, &flow->flags))
				nf_flow_offload_del(flow_table, flow);
			else if (test_bit(NF_FLOW_HW_DEAD, &flow->flags))
				flow_offload_del(flow_table, flow);
		} else {
			flow_offload_del(flow_table, flow);
		}
	} else if (test_bit(NF_FLOW_HW, &flow->flags)) {
		nf_flow_offload_stats(flow_table, flow);
	}
>>>>>>> upstream/android-13
}

static void nf_flow_offload_work_gc(struct work_struct *work)
{
	struct nf_flowtable *flow_table;

	flow_table = container_of(work, struct nf_flowtable, gc_work.work);
<<<<<<< HEAD
	nf_flow_offload_gc_step(flow_table);
	queue_delayed_work(system_power_efficient_wq, &flow_table->gc_work, HZ);
}

static int nf_flow_nat_port_tcp(struct sk_buff *skb, unsigned int thoff,
				__be16 port, __be16 new_port)
{
	struct tcphdr *tcph;

	if (!pskb_may_pull(skb, thoff + sizeof(*tcph)) ||
	    skb_try_make_writable(skb, thoff + sizeof(*tcph)))
		return -1;

	tcph = (void *)(skb_network_header(skb) + thoff);
	inet_proto_csum_replace2(&tcph->check, skb, port, new_port, false);

	return 0;
}

static int nf_flow_nat_port_udp(struct sk_buff *skb, unsigned int thoff,
				__be16 port, __be16 new_port)
{
	struct udphdr *udph;

	if (!pskb_may_pull(skb, thoff + sizeof(*udph)) ||
	    skb_try_make_writable(skb, thoff + sizeof(*udph)))
		return -1;

=======
	nf_flow_table_iterate(flow_table, nf_flow_offload_gc_step, flow_table);
	queue_delayed_work(system_power_efficient_wq, &flow_table->gc_work, HZ);
}

static void nf_flow_nat_port_tcp(struct sk_buff *skb, unsigned int thoff,
				 __be16 port, __be16 new_port)
{
	struct tcphdr *tcph;

	tcph = (void *)(skb_network_header(skb) + thoff);
	inet_proto_csum_replace2(&tcph->check, skb, port, new_port, false);
}

static void nf_flow_nat_port_udp(struct sk_buff *skb, unsigned int thoff,
				 __be16 port, __be16 new_port)
{
	struct udphdr *udph;

>>>>>>> upstream/android-13
	udph = (void *)(skb_network_header(skb) + thoff);
	if (udph->check || skb->ip_summed == CHECKSUM_PARTIAL) {
		inet_proto_csum_replace2(&udph->check, skb, port,
					 new_port, false);
		if (!udph->check)
			udph->check = CSUM_MANGLED_0;
	}
<<<<<<< HEAD

	return 0;
}

static int nf_flow_nat_port(struct sk_buff *skb, unsigned int thoff,
			    u8 protocol, __be16 port, __be16 new_port)
{
	switch (protocol) {
	case IPPROTO_TCP:
		if (nf_flow_nat_port_tcp(skb, thoff, port, new_port) < 0)
			return NF_DROP;
		break;
	case IPPROTO_UDP:
		if (nf_flow_nat_port_udp(skb, thoff, port, new_port) < 0)
			return NF_DROP;
		break;
	}

	return 0;
}

int nf_flow_snat_port(const struct flow_offload *flow,
		      struct sk_buff *skb, unsigned int thoff,
		      u8 protocol, enum flow_offload_tuple_dir dir)
=======
}

static void nf_flow_nat_port(struct sk_buff *skb, unsigned int thoff,
			     u8 protocol, __be16 port, __be16 new_port)
{
	switch (protocol) {
	case IPPROTO_TCP:
		nf_flow_nat_port_tcp(skb, thoff, port, new_port);
		break;
	case IPPROTO_UDP:
		nf_flow_nat_port_udp(skb, thoff, port, new_port);
		break;
	}
}

void nf_flow_snat_port(const struct flow_offload *flow,
		       struct sk_buff *skb, unsigned int thoff,
		       u8 protocol, enum flow_offload_tuple_dir dir)
>>>>>>> upstream/android-13
{
	struct flow_ports *hdr;
	__be16 port, new_port;

<<<<<<< HEAD
	if (!pskb_may_pull(skb, thoff + sizeof(*hdr)) ||
	    skb_try_make_writable(skb, thoff + sizeof(*hdr)))
		return -1;

=======
>>>>>>> upstream/android-13
	hdr = (void *)(skb_network_header(skb) + thoff);

	switch (dir) {
	case FLOW_OFFLOAD_DIR_ORIGINAL:
		port = hdr->source;
		new_port = flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple.dst_port;
		hdr->source = new_port;
		break;
	case FLOW_OFFLOAD_DIR_REPLY:
		port = hdr->dest;
		new_port = flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.src_port;
		hdr->dest = new_port;
		break;
<<<<<<< HEAD
	default:
		return -1;
	}

	return nf_flow_nat_port(skb, thoff, protocol, port, new_port);
}
EXPORT_SYMBOL_GPL(nf_flow_snat_port);

int nf_flow_dnat_port(const struct flow_offload *flow,
		      struct sk_buff *skb, unsigned int thoff,
		      u8 protocol, enum flow_offload_tuple_dir dir)
=======
	}

	nf_flow_nat_port(skb, thoff, protocol, port, new_port);
}
EXPORT_SYMBOL_GPL(nf_flow_snat_port);

void nf_flow_dnat_port(const struct flow_offload *flow, struct sk_buff *skb,
		       unsigned int thoff, u8 protocol,
		       enum flow_offload_tuple_dir dir)
>>>>>>> upstream/android-13
{
	struct flow_ports *hdr;
	__be16 port, new_port;

<<<<<<< HEAD
	if (!pskb_may_pull(skb, thoff + sizeof(*hdr)) ||
	    skb_try_make_writable(skb, thoff + sizeof(*hdr)))
		return -1;

=======
>>>>>>> upstream/android-13
	hdr = (void *)(skb_network_header(skb) + thoff);

	switch (dir) {
	case FLOW_OFFLOAD_DIR_ORIGINAL:
		port = hdr->dest;
		new_port = flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple.src_port;
		hdr->dest = new_port;
		break;
	case FLOW_OFFLOAD_DIR_REPLY:
		port = hdr->source;
		new_port = flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.dst_port;
		hdr->source = new_port;
		break;
<<<<<<< HEAD
	default:
		return -1;
	}

	return nf_flow_nat_port(skb, thoff, protocol, port, new_port);
=======
	}

	nf_flow_nat_port(skb, thoff, protocol, port, new_port);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(nf_flow_dnat_port);

int nf_flow_table_init(struct nf_flowtable *flowtable)
{
	int err;

<<<<<<< HEAD
	INIT_DEFERRABLE_WORK(&flowtable->gc_work, nf_flow_offload_work_gc);
=======
	INIT_DELAYED_WORK(&flowtable->gc_work, nf_flow_offload_work_gc);
	flow_block_init(&flowtable->flow_block);
	init_rwsem(&flowtable->flow_block_lock);
>>>>>>> upstream/android-13

	err = rhashtable_init(&flowtable->rhashtable,
			      &nf_flow_offload_rhash_params);
	if (err < 0)
		return err;

	queue_delayed_work(system_power_efficient_wq,
			   &flowtable->gc_work, HZ);

	mutex_lock(&flowtable_lock);
	list_add(&flowtable->list, &flowtables);
	mutex_unlock(&flowtable_lock);

	return 0;
}
EXPORT_SYMBOL_GPL(nf_flow_table_init);

static void nf_flow_table_do_cleanup(struct flow_offload *flow, void *data)
{
	struct net_device *dev = data;
<<<<<<< HEAD
	struct flow_offload_entry *e;

	e = container_of(flow, struct flow_offload_entry, flow);
=======
>>>>>>> upstream/android-13

	if (!dev) {
		flow_offload_teardown(flow);
		return;
	}
<<<<<<< HEAD
	if (net_eq(nf_ct_net(e->ct), dev_net(dev)) &&
	    (flow->tuplehash[0].tuple.iifidx == dev->ifindex ||
	     flow->tuplehash[1].tuple.iifidx == dev->ifindex))
		flow_offload_dead(flow);
}

static void nf_flow_table_iterate_cleanup(struct nf_flowtable *flowtable,
					  struct net_device *dev)
{
	nf_flow_table_iterate(flowtable, nf_flow_table_do_cleanup, dev);
	flush_delayed_work(&flowtable->gc_work);
}

void nf_flow_table_cleanup(struct net *net, struct net_device *dev)
=======

	if (net_eq(nf_ct_net(flow->ct), dev_net(dev)) &&
	    (flow->tuplehash[0].tuple.iifidx == dev->ifindex ||
	     flow->tuplehash[1].tuple.iifidx == dev->ifindex))
		flow_offload_teardown(flow);
}

void nf_flow_table_gc_cleanup(struct nf_flowtable *flowtable,
			      struct net_device *dev)
{
	nf_flow_table_iterate(flowtable, nf_flow_table_do_cleanup, dev);
	flush_delayed_work(&flowtable->gc_work);
	nf_flow_table_offload_flush(flowtable);
}

void nf_flow_table_cleanup(struct net_device *dev)
>>>>>>> upstream/android-13
{
	struct nf_flowtable *flowtable;

	mutex_lock(&flowtable_lock);
	list_for_each_entry(flowtable, &flowtables, list)
<<<<<<< HEAD
		nf_flow_table_iterate_cleanup(flowtable, dev);
=======
		nf_flow_table_gc_cleanup(flowtable, dev);
>>>>>>> upstream/android-13
	mutex_unlock(&flowtable_lock);
}
EXPORT_SYMBOL_GPL(nf_flow_table_cleanup);

void nf_flow_table_free(struct nf_flowtable *flow_table)
{
	mutex_lock(&flowtable_lock);
	list_del(&flow_table->list);
	mutex_unlock(&flowtable_lock);
<<<<<<< HEAD
	cancel_delayed_work_sync(&flow_table->gc_work);
	nf_flow_table_iterate(flow_table, nf_flow_table_do_cleanup, NULL);
	WARN_ON(!nf_flow_offload_gc_step(flow_table));
=======

	cancel_delayed_work_sync(&flow_table->gc_work);
	nf_flow_table_iterate(flow_table, nf_flow_table_do_cleanup, NULL);
	nf_flow_table_iterate(flow_table, nf_flow_offload_gc_step, flow_table);
	nf_flow_table_offload_flush(flow_table);
	if (nf_flowtable_hw_offload(flow_table))
		nf_flow_table_iterate(flow_table, nf_flow_offload_gc_step,
				      flow_table);
>>>>>>> upstream/android-13
	rhashtable_destroy(&flow_table->rhashtable);
}
EXPORT_SYMBOL_GPL(nf_flow_table_free);

<<<<<<< HEAD
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pablo Neira Ayuso <pablo@netfilter.org>");
=======
static int __init nf_flow_table_module_init(void)
{
	return nf_flow_table_offload_init();
}

static void __exit nf_flow_table_module_exit(void)
{
	nf_flow_table_offload_exit();
}

module_init(nf_flow_table_module_init);
module_exit(nf_flow_table_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pablo Neira Ayuso <pablo@netfilter.org>");
MODULE_DESCRIPTION("Netfilter flow table module");
>>>>>>> upstream/android-13
