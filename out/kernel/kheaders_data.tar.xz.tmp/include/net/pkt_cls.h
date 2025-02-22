/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __NET_PKT_CLS_H
#define __NET_PKT_CLS_H

#include <linux/pkt_cls.h>
#include <linux/workqueue.h>
#include <net/sch_generic.h>
#include <net/act_api.h>


#define TC_ACT_REINSERT		(TC_ACT_VALUE_MAX + 1)



struct tcf_walker {
	int	stop;
	int	skip;
	int	count;
	unsigned long cookie;
	int	(*fn)(struct tcf_proto *, void *node, struct tcf_walker *);
};

int register_tcf_proto_ops(struct tcf_proto_ops *ops);
int unregister_tcf_proto_ops(struct tcf_proto_ops *ops);

enum tcf_block_binder_type {
	TCF_BLOCK_BINDER_TYPE_UNSPEC,
	TCF_BLOCK_BINDER_TYPE_CLSACT_INGRESS,
	TCF_BLOCK_BINDER_TYPE_CLSACT_EGRESS,
};

struct tcf_block_ext_info {
	enum tcf_block_binder_type binder_type;
	tcf_chain_head_change_t *chain_head_change;
	void *chain_head_change_priv;
	u32 block_index;
};

struct tcf_block_cb;
bool tcf_queue_work(struct rcu_work *rwork, work_func_t func);

#ifdef CONFIG_NET_CLS
struct tcf_chain *tcf_chain_get_by_act(struct tcf_block *block,
				       u32 chain_index);
void tcf_chain_put_by_act(struct tcf_chain *chain);
void tcf_block_netif_keep_dst(struct tcf_block *block);
int tcf_block_get(struct tcf_block **p_block,
		  struct tcf_proto __rcu **p_filter_chain, struct Qdisc *q,
		  struct netlink_ext_ack *extack);
int tcf_block_get_ext(struct tcf_block **p_block, struct Qdisc *q,
		      struct tcf_block_ext_info *ei,
		      struct netlink_ext_ack *extack);
void tcf_block_put(struct tcf_block *block);
void tcf_block_put_ext(struct tcf_block *block, struct Qdisc *q,
		       struct tcf_block_ext_info *ei);

static inline bool tcf_block_shared(struct tcf_block *block)
{
	return block->index;
}

static inline struct Qdisc *tcf_block_q(struct tcf_block *block)
{
	WARN_ON(tcf_block_shared(block));
	return block->q;
}

static inline struct net_device *tcf_block_dev(struct tcf_block *block)
{
	return tcf_block_q(block)->dev_queue->dev;
}

void *tcf_block_cb_priv(struct tcf_block_cb *block_cb);
struct tcf_block_cb *tcf_block_cb_lookup(struct tcf_block *block,
					 tc_setup_cb_t *cb, void *cb_ident);
void tcf_block_cb_incref(struct tcf_block_cb *block_cb);
unsigned int tcf_block_cb_decref(struct tcf_block_cb *block_cb);
struct tcf_block_cb *__tcf_block_cb_register(struct tcf_block *block,
					     tc_setup_cb_t *cb, void *cb_ident,
					     void *cb_priv,
					     struct netlink_ext_ack *extack);
int tcf_block_cb_register(struct tcf_block *block,
			  tc_setup_cb_t *cb, void *cb_ident,
			  void *cb_priv, struct netlink_ext_ack *extack);
void __tcf_block_cb_unregister(struct tcf_block *block,
			       struct tcf_block_cb *block_cb);
void tcf_block_cb_unregister(struct tcf_block *block,
			     tc_setup_cb_t *cb, void *cb_ident);

int tcf_classify(struct sk_buff *skb, const struct tcf_proto *tp,
		 struct tcf_result *res, bool compat_mode);

#else
static inline
int tcf_block_get(struct tcf_block **p_block,
		  struct tcf_proto __rcu **p_filter_chain, struct Qdisc *q,
		  struct netlink_ext_ack *extack)
{
	return 0;
}

static inline
int tcf_block_get_ext(struct tcf_block **p_block, struct Qdisc *q,
		      struct tcf_block_ext_info *ei,
		      struct netlink_ext_ack *extack)
{
	return 0;
}

static inline void tcf_block_put(struct tcf_block *block)
{
}

static inline
void tcf_block_put_ext(struct tcf_block *block, struct Qdisc *q,
		       struct tcf_block_ext_info *ei)
{
}

static inline struct Qdisc *tcf_block_q(struct tcf_block *block)
{
	return NULL;
}

static inline struct net_device *tcf_block_dev(struct tcf_block *block)
{
	return NULL;
}

static inline
int tc_setup_cb_block_register(struct tcf_block *block, tc_setup_cb_t *cb,
			       void *cb_priv)
{
	return 0;
}

static inline
void tc_setup_cb_block_unregister(struct tcf_block *block, tc_setup_cb_t *cb,
				  void *cb_priv)
{
}

static inline
void *tcf_block_cb_priv(struct tcf_block_cb *block_cb)
{
	return NULL;
}

static inline
struct tcf_block_cb *tcf_block_cb_lookup(struct tcf_block *block,
					 tc_setup_cb_t *cb, void *cb_ident)
{
	return NULL;
}

static inline
void tcf_block_cb_incref(struct tcf_block_cb *block_cb)
{
}

static inline
unsigned int tcf_block_cb_decref(struct tcf_block_cb *block_cb)
{
	return 0;
}

static inline
struct tcf_block_cb *__tcf_block_cb_register(struct tcf_block *block,
					     tc_setup_cb_t *cb, void *cb_ident,
					     void *cb_priv,
					     struct netlink_ext_ack *extack)
{
	return NULL;
}

static inline
int tcf_block_cb_register(struct tcf_block *block,
			  tc_setup_cb_t *cb, void *cb_ident,
			  void *cb_priv, struct netlink_ext_ack *extack)
{
	return 0;
}

static inline
void __tcf_block_cb_unregister(struct tcf_block *block,
			       struct tcf_block_cb *block_cb)
{
}

static inline
void tcf_block_cb_unregister(struct tcf_block *block,
			     tc_setup_cb_t *cb, void *cb_ident)
{
}

static inline int tcf_classify(struct sk_buff *skb, const struct tcf_proto *tp,
			       struct tcf_result *res, bool compat_mode)
{
	return TC_ACT_UNSPEC;
}
#endif

static inline unsigned long
__cls_set_class(unsigned long *clp, unsigned long cl)
{
	return xchg(clp, cl);
}

static inline void
__tcf_bind_filter(struct Qdisc *q, struct tcf_result *r, unsigned long base)
{
	unsigned long cl;

	cl = q->ops->cl_ops->bind_tcf(q, base, r->classid);
	cl = __cls_set_class(&r->class, cl);
	if (cl)
		q->ops->cl_ops->unbind_tcf(q, cl);
}

static inline void
tcf_bind_filter(struct tcf_proto *tp, struct tcf_result *r, unsigned long base)
{
	struct Qdisc *q = tp->chain->block->q;

	
	if (!q)
		return;
	sch_tree_lock(q);
	__tcf_bind_filter(q, r, base);
	sch_tree_unlock(q);
}

static inline void
__tcf_unbind_filter(struct Qdisc *q, struct tcf_result *r)
{
	unsigned long cl;

	if ((cl = __cls_set_class(&r->class, 0)) != 0)
		q->ops->cl_ops->unbind_tcf(q, cl);
}

static inline void
tcf_unbind_filter(struct tcf_proto *tp, struct tcf_result *r)
{
	struct Qdisc *q = tp->chain->block->q;

	if (!q)
		return;
	__tcf_unbind_filter(q, r);
}

struct tcf_exts {
#ifdef CONFIG_NET_CLS_ACT
	__u32	type; 
	int nr_actions;
	struct tc_action **actions;
	struct net *net;
#endif
	
	int action;
	int police;
};

static inline int tcf_exts_init(struct tcf_exts *exts, int action, int police)
{
#ifdef CONFIG_NET_CLS_ACT
	exts->type = 0;
	exts->nr_actions = 0;
	exts->net = NULL;
	exts->actions = kcalloc(TCA_ACT_MAX_PRIO, sizeof(struct tc_action *),
				GFP_KERNEL);
	if (!exts->actions)
		return -ENOMEM;
#endif
	exts->action = action;
	exts->police = police;
	return 0;
}


static inline bool tcf_exts_get_net(struct tcf_exts *exts)
{
#ifdef CONFIG_NET_CLS_ACT
	exts->net = maybe_get_net(exts->net);
	return exts->net != NULL;
#else
	return true;
#endif
}

static inline void tcf_exts_put_net(struct tcf_exts *exts)
{
#ifdef CONFIG_NET_CLS_ACT
	if (exts->net)
		put_net(exts->net);
#endif
}

#ifdef CONFIG_NET_CLS_ACT
#define tcf_exts_for_each_action(i, a, exts) \
	for (i = 0; i < TCA_ACT_MAX_PRIO && ((a) = (exts)->actions[i]); i++)
#else
#define tcf_exts_for_each_action(i, a, exts) \
	for (; 0; (void)(i), (void)(a), (void)(exts))
#endif

static inline void
tcf_exts_stats_update(const struct tcf_exts *exts,
		      u64 bytes, u64 packets, u64 lastuse)
{
#ifdef CONFIG_NET_CLS_ACT
	int i;

	preempt_disable();

	for (i = 0; i < exts->nr_actions; i++) {
		struct tc_action *a = exts->actions[i];

		tcf_action_stats_update(a, bytes, packets, lastuse);
	}

	preempt_enable();
#endif
}


static inline bool tcf_exts_has_actions(struct tcf_exts *exts)
{
#ifdef CONFIG_NET_CLS_ACT
	return exts->nr_actions;
#else
	return false;
#endif
}


static inline bool tcf_exts_has_one_action(struct tcf_exts *exts)
{
#ifdef CONFIG_NET_CLS_ACT
	return exts->nr_actions == 1;
#else
	return false;
#endif
}

static inline struct tc_action *tcf_exts_first_action(struct tcf_exts *exts)
{
#ifdef CONFIG_NET_CLS_ACT
	return exts->actions[0];
#else
	return NULL;
#endif
}


static inline int
tcf_exts_exec(struct sk_buff *skb, struct tcf_exts *exts,
	      struct tcf_result *res)
{
#ifdef CONFIG_NET_CLS_ACT
	return tcf_action_exec(skb, exts->actions, exts->nr_actions, res);
#endif
	return TC_ACT_OK;
}

int tcf_exts_validate(struct net *net, struct tcf_proto *tp,
		      struct nlattr **tb, struct nlattr *rate_tlv,
		      struct tcf_exts *exts, bool ovr,
		      struct netlink_ext_ack *extack);
void tcf_exts_destroy(struct tcf_exts *exts);
void tcf_exts_change(struct tcf_exts *dst, struct tcf_exts *src);
int tcf_exts_dump(struct sk_buff *skb, struct tcf_exts *exts);
int tcf_exts_dump_stats(struct sk_buff *skb, struct tcf_exts *exts);


struct tcf_pkt_info {
	unsigned char *		ptr;
	int			nexthdr;
};

#ifdef CONFIG_NET_EMATCH

struct tcf_ematch_ops;


struct tcf_ematch {
	struct tcf_ematch_ops * ops;
	unsigned long		data;
	unsigned int		datalen;
	u16			matchid;
	u16			flags;
	struct net		*net;
};

static inline int tcf_em_is_container(struct tcf_ematch *em)
{
	return !em->ops;
}

static inline int tcf_em_is_simple(struct tcf_ematch *em)
{
	return em->flags & TCF_EM_SIMPLE;
}

static inline int tcf_em_is_inverted(struct tcf_ematch *em)
{
	return em->flags & TCF_EM_INVERT;
}

static inline int tcf_em_last_match(struct tcf_ematch *em)
{
	return (em->flags & TCF_EM_REL_MASK) == TCF_EM_REL_END;
}

static inline int tcf_em_early_end(struct tcf_ematch *em, int result)
{
	if (tcf_em_last_match(em))
		return 1;

	if (result == 0 && em->flags & TCF_EM_REL_AND)
		return 1;

	if (result != 0 && em->flags & TCF_EM_REL_OR)
		return 1;

	return 0;
}
	

struct tcf_ematch_tree {
	struct tcf_ematch_tree_hdr hdr;
	struct tcf_ematch *	matches;
	
};


struct tcf_ematch_ops {
	int			kind;
	int			datalen;
	int			(*change)(struct net *net, void *,
					  int, struct tcf_ematch *);
	int			(*match)(struct sk_buff *, struct tcf_ematch *,
					 struct tcf_pkt_info *);
	void			(*destroy)(struct tcf_ematch *);
	int			(*dump)(struct sk_buff *, struct tcf_ematch *);
	struct module		*owner;
	struct list_head	link;
};

int tcf_em_register(struct tcf_ematch_ops *);
void tcf_em_unregister(struct tcf_ematch_ops *);
int tcf_em_tree_validate(struct tcf_proto *, struct nlattr *,
			 struct tcf_ematch_tree *);
void tcf_em_tree_destroy(struct tcf_ematch_tree *);
int tcf_em_tree_dump(struct sk_buff *, struct tcf_ematch_tree *, int);
int __tcf_em_tree_match(struct sk_buff *, struct tcf_ematch_tree *,
			struct tcf_pkt_info *);


static inline int tcf_em_tree_match(struct sk_buff *skb,
				    struct tcf_ematch_tree *tree,
				    struct tcf_pkt_info *info)
{
	if (tree->hdr.nmatches)
		return __tcf_em_tree_match(skb, tree, info);
	else
		return 1;
}

#define MODULE_ALIAS_TCF_EMATCH(kind)	MODULE_ALIAS("ematch-kind-" __stringify(kind))

#else 

struct tcf_ematch_tree {
};

#define tcf_em_tree_validate(tp, tb, t) ((void)(t), 0)
#define tcf_em_tree_destroy(t) do { (void)(t); } while(0)
#define tcf_em_tree_dump(skb, t, tlv) (0)
#define tcf_em_tree_match(skb, t, info) ((void)(info), 1)

#endif 

static inline unsigned char * tcf_get_base_ptr(struct sk_buff *skb, int layer)
{
	switch (layer) {
		case TCF_LAYER_LINK:
			return skb_mac_header(skb);
		case TCF_LAYER_NETWORK:
			return skb_network_header(skb);
		case TCF_LAYER_TRANSPORT:
			return skb_transport_header(skb);
	}

	return NULL;
}

static inline int tcf_valid_offset(const struct sk_buff *skb,
				   const unsigned char *ptr, const int len)
{
	return likely((ptr + len) <= skb_tail_pointer(skb) &&
		      ptr >= skb->head &&
		      (ptr <= (ptr + len)));
}

#ifdef CONFIG_NET_CLS_IND
#include <net/net_namespace.h>

static inline int
tcf_change_indev(struct net *net, struct nlattr *indev_tlv,
		 struct netlink_ext_ack *extack)
{
	char indev[IFNAMSIZ];
	struct net_device *dev;

	if (nla_strlcpy(indev, indev_tlv, IFNAMSIZ) >= IFNAMSIZ) {
		NL_SET_ERR_MSG(extack, "Interface name too long");
		return -EINVAL;
	}
	dev = __dev_get_by_name(net, indev);
	if (!dev)
		return -ENODEV;
	return dev->ifindex;
}

static inline bool
tcf_match_indev(struct sk_buff *skb, int ifindex)
{
	if (!ifindex)
		return true;
	if  (!skb->skb_iif)
		return false;
	return ifindex == skb->skb_iif;
}
#endif 

int tc_setup_cb_call(struct tcf_block *block, struct tcf_exts *exts,
		     enum tc_setup_type type, void *type_data, bool err_stop);

enum tc_block_command {
	TC_BLOCK_BIND,
	TC_BLOCK_UNBIND,
};

struct tc_block_offload {
	enum tc_block_command command;
	enum tcf_block_binder_type binder_type;
	struct tcf_block *block;
	struct netlink_ext_ack *extack;
};

struct tc_cls_common_offload {
	u32 chain_index;
	__be16 protocol;
	u32 prio;
	struct netlink_ext_ack *extack;
};

struct tc_cls_u32_knode {
	struct tcf_exts *exts;
	struct tc_u32_sel *sel;
	u32 handle;
	u32 val;
	u32 mask;
	u32 link_handle;
	u8 fshift;
};

struct tc_cls_u32_hnode {
	u32 handle;
	u32 prio;
	unsigned int divisor;
};

enum tc_clsu32_command {
	TC_CLSU32_NEW_KNODE,
	TC_CLSU32_REPLACE_KNODE,
	TC_CLSU32_DELETE_KNODE,
	TC_CLSU32_NEW_HNODE,
	TC_CLSU32_REPLACE_HNODE,
	TC_CLSU32_DELETE_HNODE,
};

struct tc_cls_u32_offload {
	struct tc_cls_common_offload common;
	
	enum tc_clsu32_command command;
	union {
		struct tc_cls_u32_knode knode;
		struct tc_cls_u32_hnode hnode;
	};
};

static inline bool tc_can_offload(const struct net_device *dev)
{
	return dev->features & NETIF_F_HW_TC;
}

static inline bool tc_can_offload_extack(const struct net_device *dev,
					 struct netlink_ext_ack *extack)
{
	bool can = tc_can_offload(dev);

	if (!can)
		NL_SET_ERR_MSG(extack, "TC offload is disabled on net device");

	return can;
}

static inline bool
tc_cls_can_offload_and_chain0(const struct net_device *dev,
			      struct tc_cls_common_offload *common)
{
	if (!tc_can_offload_extack(dev, common->extack))
		return false;
	if (common->chain_index) {
		NL_SET_ERR_MSG(common->extack,
			       "Driver supports only offload of chain 0");
		return false;
	}
	return true;
}

static inline bool tc_skip_hw(u32 flags)
{
	return (flags & TCA_CLS_FLAGS_SKIP_HW) ? true : false;
}

static inline bool tc_skip_sw(u32 flags)
{
	return (flags & TCA_CLS_FLAGS_SKIP_SW) ? true : false;
}


static inline bool tc_flags_valid(u32 flags)
{
	if (flags & ~(TCA_CLS_FLAGS_SKIP_HW | TCA_CLS_FLAGS_SKIP_SW |
		      TCA_CLS_FLAGS_VERBOSE))
		return false;

	flags &= TCA_CLS_FLAGS_SKIP_HW | TCA_CLS_FLAGS_SKIP_SW;
	if (!(flags ^ (TCA_CLS_FLAGS_SKIP_HW | TCA_CLS_FLAGS_SKIP_SW)))
		return false;

	return true;
}

static inline bool tc_in_hw(u32 flags)
{
	return (flags & TCA_CLS_FLAGS_IN_HW) ? true : false;
}

static inline void
tc_cls_common_offload_init(struct tc_cls_common_offload *cls_common,
			   const struct tcf_proto *tp, u32 flags,
			   struct netlink_ext_ack *extack)
{
	cls_common->chain_index = tp->chain->index;
	cls_common->protocol = tp->protocol;
	cls_common->prio = tp->prio;
	if (tc_skip_sw(flags) || flags & TCA_CLS_FLAGS_VERBOSE)
		cls_common->extack = extack;
}

enum tc_fl_command {
	TC_CLSFLOWER_REPLACE,
	TC_CLSFLOWER_DESTROY,
	TC_CLSFLOWER_STATS,
	TC_CLSFLOWER_TMPLT_CREATE,
	TC_CLSFLOWER_TMPLT_DESTROY,
};

struct tc_cls_flower_offload {
	struct tc_cls_common_offload common;
	enum tc_fl_command command;
	unsigned long cookie;
	struct flow_dissector *dissector;
	struct fl_flow_key *mask;
	struct fl_flow_key *key;
	struct tcf_exts *exts;
	u32 classid;
};

enum tc_matchall_command {
	TC_CLSMATCHALL_REPLACE,
	TC_CLSMATCHALL_DESTROY,
};

struct tc_cls_matchall_offload {
	struct tc_cls_common_offload common;
	enum tc_matchall_command command;
	struct tcf_exts *exts;
	unsigned long cookie;
};

enum tc_clsbpf_command {
	TC_CLSBPF_OFFLOAD,
	TC_CLSBPF_STATS,
};

struct tc_cls_bpf_offload {
	struct tc_cls_common_offload common;
	enum tc_clsbpf_command command;
	struct tcf_exts *exts;
	struct bpf_prog *prog;
	struct bpf_prog *oldprog;
	const char *name;
	bool exts_integrated;
};

struct tc_mqprio_qopt_offload {
	
	struct tc_mqprio_qopt qopt;
	u16 mode;
	u16 shaper;
	u32 flags;
	u64 min_rate[TC_QOPT_MAX_QUEUE];
	u64 max_rate[TC_QOPT_MAX_QUEUE];
};


struct tc_cookie {
	u8  *data;
	u32 len;
	struct rcu_head rcu;
};

struct tc_qopt_offload_stats {
	struct gnet_stats_basic_packed *bstats;
	struct gnet_stats_queue *qstats;
};

enum tc_mq_command {
	TC_MQ_CREATE,
	TC_MQ_DESTROY,
	TC_MQ_STATS,
};

struct tc_mq_qopt_offload {
	enum tc_mq_command command;
	u32 handle;
	struct tc_qopt_offload_stats stats;
};

enum tc_red_command {
	TC_RED_REPLACE,
	TC_RED_DESTROY,
	TC_RED_STATS,
	TC_RED_XSTATS,
};

struct tc_red_qopt_offload_params {
	u32 min;
	u32 max;
	u32 probability;
	bool is_ecn;
	struct gnet_stats_queue *qstats;
};

struct tc_red_qopt_offload {
	enum tc_red_command command;
	u32 handle;
	u32 parent;
	union {
		struct tc_red_qopt_offload_params set;
		struct tc_qopt_offload_stats stats;
		struct red_stats *xstats;
	};
};

enum tc_prio_command {
	TC_PRIO_REPLACE,
	TC_PRIO_DESTROY,
	TC_PRIO_STATS,
	TC_PRIO_GRAFT,
};

struct tc_prio_qopt_offload_params {
	int bands;
	u8 priomap[TC_PRIO_MAX + 1];
	
	struct gnet_stats_queue *qstats;
};

struct tc_prio_qopt_offload_graft_params {
	u8 band;
	u32 child_handle;
};

struct tc_prio_qopt_offload {
	enum tc_prio_command command;
	u32 handle;
	u32 parent;
	union {
		struct tc_prio_qopt_offload_params replace_params;
		struct tc_qopt_offload_stats stats;
		struct tc_prio_qopt_offload_graft_params graft_params;
	};
};

#endif
