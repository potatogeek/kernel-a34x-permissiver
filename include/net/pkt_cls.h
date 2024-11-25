/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __NET_PKT_CLS_H
#define __NET_PKT_CLS_H

#include <linux/pkt_cls.h>
#include <linux/workqueue.h>
#include <net/sch_generic.h>
#include <net/act_api.h>
<<<<<<< HEAD

/* TC action not accessible from user space */
#define TC_ACT_REINSERT		(TC_ACT_VALUE_MAX + 1)
=======
#include <net/net_namespace.h>

/* TC action not accessible from user space */
#define TC_ACT_CONSUMED		(TC_ACT_VALUE_MAX + 1)
>>>>>>> upstream/android-13

/* Basic packet classifier frontend definitions. */

struct tcf_walker {
	int	stop;
	int	skip;
	int	count;
<<<<<<< HEAD
=======
	bool	nonempty;
>>>>>>> upstream/android-13
	unsigned long cookie;
	int	(*fn)(struct tcf_proto *, void *node, struct tcf_walker *);
};

int register_tcf_proto_ops(struct tcf_proto_ops *ops);
int unregister_tcf_proto_ops(struct tcf_proto_ops *ops);

<<<<<<< HEAD
enum tcf_block_binder_type {
	TCF_BLOCK_BINDER_TYPE_UNSPEC,
	TCF_BLOCK_BINDER_TYPE_CLSACT_INGRESS,
	TCF_BLOCK_BINDER_TYPE_CLSACT_EGRESS,
};

struct tcf_block_ext_info {
	enum tcf_block_binder_type binder_type;
=======
struct tcf_block_ext_info {
	enum flow_block_binder_type binder_type;
>>>>>>> upstream/android-13
	tcf_chain_head_change_t *chain_head_change;
	void *chain_head_change_priv;
	u32 block_index;
};

<<<<<<< HEAD
=======
struct tcf_qevent {
	struct tcf_block	*block;
	struct tcf_block_ext_info info;
	struct tcf_proto __rcu *filter_chain;
};

>>>>>>> upstream/android-13
struct tcf_block_cb;
bool tcf_queue_work(struct rcu_work *rwork, work_func_t func);

#ifdef CONFIG_NET_CLS
struct tcf_chain *tcf_chain_get_by_act(struct tcf_block *block,
				       u32 chain_index);
void tcf_chain_put_by_act(struct tcf_chain *chain);
<<<<<<< HEAD
=======
struct tcf_chain *tcf_get_next_chain(struct tcf_block *block,
				     struct tcf_chain *chain);
struct tcf_proto *tcf_get_next_proto(struct tcf_chain *chain,
				     struct tcf_proto *tp);
>>>>>>> upstream/android-13
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

<<<<<<< HEAD
=======
static inline bool tcf_block_non_null_shared(struct tcf_block *block)
{
	return block && block->index;
}

>>>>>>> upstream/android-13
static inline struct Qdisc *tcf_block_q(struct tcf_block *block)
{
	WARN_ON(tcf_block_shared(block));
	return block->q;
}

<<<<<<< HEAD
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
=======
int tcf_classify(struct sk_buff *skb,
		 const struct tcf_block *block,
		 const struct tcf_proto *tp, struct tcf_result *res,
		 bool compat_mode);

#else
static inline bool tcf_block_shared(struct tcf_block *block)
{
	return false;
}

static inline bool tcf_block_non_null_shared(struct tcf_block *block)
{
	return false;
}

>>>>>>> upstream/android-13
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

<<<<<<< HEAD
static inline struct net_device *tcf_block_dev(struct tcf_block *block)
{
	return NULL;
}

static inline
int tc_setup_cb_block_register(struct tcf_block *block, tc_setup_cb_t *cb,
=======
static inline
int tc_setup_cb_block_register(struct tcf_block *block, flow_setup_cb_t *cb,
>>>>>>> upstream/android-13
			       void *cb_priv)
{
	return 0;
}

static inline
<<<<<<< HEAD
void tc_setup_cb_block_unregister(struct tcf_block *block, tc_setup_cb_t *cb,
=======
void tc_setup_cb_block_unregister(struct tcf_block *block, flow_setup_cb_t *cb,
>>>>>>> upstream/android-13
				  void *cb_priv)
{
}

<<<<<<< HEAD
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
=======
static inline int tcf_classify(struct sk_buff *skb,
			       const struct tcf_block *block,
			       const struct tcf_proto *tp,
>>>>>>> upstream/android-13
			       struct tcf_result *res, bool compat_mode)
{
	return TC_ACT_UNSPEC;
}
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
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

	/* Check q as it is not set for shared blocks. In that case,
	 * setting class is not supported.
	 */
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
	__u32	type; /* for backward compat(TCA_OLD_COMPAT) */
	int nr_actions;
	struct tc_action **actions;
	struct net *net;
#endif
	/* Map to export classifier specific extension TLV types to the
	 * generic extensions API. Unsupported extensions must be set to 0.
	 */
	int action;
	int police;
};

<<<<<<< HEAD
static inline int tcf_exts_init(struct tcf_exts *exts, int action, int police)
=======
static inline int tcf_exts_init(struct tcf_exts *exts, struct net *net,
				int action, int police)
>>>>>>> upstream/android-13
{
#ifdef CONFIG_NET_CLS_ACT
	exts->type = 0;
	exts->nr_actions = 0;
<<<<<<< HEAD
	exts->net = NULL;
=======
	exts->net = net;
>>>>>>> upstream/android-13
	exts->actions = kcalloc(TCA_ACT_MAX_PRIO, sizeof(struct tc_action *),
				GFP_KERNEL);
	if (!exts->actions)
		return -ENOMEM;
#endif
	exts->action = action;
	exts->police = police;
	return 0;
}

/* Return false if the netns is being destroyed in cleanup_net(). Callers
 * need to do cleanup synchronously in this case, otherwise may race with
 * tc_action_net_exit(). Return true for other cases.
 */
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
<<<<<<< HEAD
		      u64 bytes, u64 packets, u64 lastuse)
=======
		      u64 bytes, u64 packets, u64 drops, u64 lastuse,
		      u8 used_hw_stats, bool used_hw_stats_valid)
>>>>>>> upstream/android-13
{
#ifdef CONFIG_NET_CLS_ACT
	int i;

	preempt_disable();

	for (i = 0; i < exts->nr_actions; i++) {
		struct tc_action *a = exts->actions[i];

<<<<<<< HEAD
		tcf_action_stats_update(a, bytes, packets, lastuse);
=======
		tcf_action_stats_update(a, bytes, packets, drops,
					lastuse, true);
		a->used_hw_stats = used_hw_stats;
		a->used_hw_stats_valid = used_hw_stats_valid;
>>>>>>> upstream/android-13
	}

	preempt_enable();
#endif
}

/**
 * tcf_exts_has_actions - check if at least one action is present
 * @exts: tc filter extensions handle
 *
 * Returns true if at least one action is present.
 */
static inline bool tcf_exts_has_actions(struct tcf_exts *exts)
{
#ifdef CONFIG_NET_CLS_ACT
	return exts->nr_actions;
#else
	return false;
#endif
}

/**
<<<<<<< HEAD
 * tcf_exts_has_one_action - check if exactly one action is present
 * @exts: tc filter extensions handle
 *
 * Returns true if exactly one action is present.
 */
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

/**
=======
>>>>>>> upstream/android-13
 * tcf_exts_exec - execute tc filter extensions
 * @skb: socket buffer
 * @exts: tc filter extensions handle
 * @res: desired result
 *
 * Executes all configured extensions. Returns TC_ACT_OK on a normal execution,
 * a negative number if the filter must be considered unmatched or
 * a positive action code (TC_ACT_*) which must be returned to the
 * underlying layer.
 */
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
<<<<<<< HEAD
		      struct tcf_exts *exts, bool ovr,
=======
		      struct tcf_exts *exts, u32 flags,
>>>>>>> upstream/android-13
		      struct netlink_ext_ack *extack);
void tcf_exts_destroy(struct tcf_exts *exts);
void tcf_exts_change(struct tcf_exts *dst, struct tcf_exts *src);
int tcf_exts_dump(struct sk_buff *skb, struct tcf_exts *exts);
<<<<<<< HEAD
=======
int tcf_exts_terse_dump(struct sk_buff *skb, struct tcf_exts *exts);
>>>>>>> upstream/android-13
int tcf_exts_dump_stats(struct sk_buff *skb, struct tcf_exts *exts);

/**
 * struct tcf_pkt_info - packet information
<<<<<<< HEAD
=======
 *
 * @ptr: start of the pkt data
 * @nexthdr: offset of the next header
>>>>>>> upstream/android-13
 */
struct tcf_pkt_info {
	unsigned char *		ptr;
	int			nexthdr;
};

#ifdef CONFIG_NET_EMATCH

struct tcf_ematch_ops;

/**
 * struct tcf_ematch - extended match (ematch)
 * 
 * @matchid: identifier to allow userspace to reidentify a match
 * @flags: flags specifying attributes and the relation to other matches
 * @ops: the operations lookup table of the corresponding ematch module
 * @datalen: length of the ematch specific configuration data
 * @data: ematch specific data
<<<<<<< HEAD
=======
 * @net: the network namespace
>>>>>>> upstream/android-13
 */
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
	
/**
 * struct tcf_ematch_tree - ematch tree handle
 *
 * @hdr: ematch tree header supplied by userspace
 * @matches: array of ematches
 */
struct tcf_ematch_tree {
	struct tcf_ematch_tree_hdr hdr;
	struct tcf_ematch *	matches;
	
};

/**
 * struct tcf_ematch_ops - ematch module operations
 * 
 * @kind: identifier (kind) of this ematch module
 * @datalen: length of expected configuration data (optional)
 * @change: called during validation (optional)
 * @match: called during ematch tree evaluation, must return 1/0
 * @destroy: called during destroyage (optional)
 * @dump: called during dumping process (optional)
 * @owner: owner, must be set to THIS_MODULE
 * @link: link to previous/next ematch module (internal use)
 */
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

/**
 * tcf_em_tree_match - evaulate an ematch tree
 *
 * @skb: socket buffer of the packet in question
 * @tree: ematch tree to be used for evaluation
 * @info: packet information examined by classifier
 *
 * This function matches @skb against the ematch tree in @tree by going
 * through all ematches respecting their logic relations returning
 * as soon as the result is obvious.
 *
 * Returns 1 if the ematch tree as-one matches, no ematches are configured
 * or ematch is not enabled in the kernel, otherwise 0 is returned.
 */
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

#else /* CONFIG_NET_EMATCH */

struct tcf_ematch_tree {
};

#define tcf_em_tree_validate(tp, tb, t) ((void)(t), 0)
#define tcf_em_tree_destroy(t) do { (void)(t); } while(0)
#define tcf_em_tree_dump(skb, t, tlv) (0)
#define tcf_em_tree_match(skb, t, info) ((void)(info), 1)

#endif /* CONFIG_NET_EMATCH */

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

<<<<<<< HEAD
#ifdef CONFIG_NET_CLS_IND
#include <net/net_namespace.h>

=======
>>>>>>> upstream/android-13
static inline int
tcf_change_indev(struct net *net, struct nlattr *indev_tlv,
		 struct netlink_ext_ack *extack)
{
	char indev[IFNAMSIZ];
	struct net_device *dev;

<<<<<<< HEAD
	if (nla_strlcpy(indev, indev_tlv, IFNAMSIZ) >= IFNAMSIZ) {
		NL_SET_ERR_MSG(extack, "Interface name too long");
		return -EINVAL;
	}
	dev = __dev_get_by_name(net, indev);
	if (!dev)
		return -ENODEV;
=======
	if (nla_strscpy(indev, indev_tlv, IFNAMSIZ) < 0) {
		NL_SET_ERR_MSG_ATTR(extack, indev_tlv,
				    "Interface name too long");
		return -EINVAL;
	}
	dev = __dev_get_by_name(net, indev);
	if (!dev) {
		NL_SET_ERR_MSG_ATTR(extack, indev_tlv,
				    "Network device not found");
		return -ENODEV;
	}
>>>>>>> upstream/android-13
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
<<<<<<< HEAD
#endif /* CONFIG_NET_CLS_IND */

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
=======

int tc_setup_flow_action(struct flow_action *flow_action,
			 const struct tcf_exts *exts);
void tc_cleanup_flow_action(struct flow_action *flow_action);

int tc_setup_cb_call(struct tcf_block *block, enum tc_setup_type type,
		     void *type_data, bool err_stop, bool rtnl_held);
int tc_setup_cb_add(struct tcf_block *block, struct tcf_proto *tp,
		    enum tc_setup_type type, void *type_data, bool err_stop,
		    u32 *flags, unsigned int *in_hw_count, bool rtnl_held);
int tc_setup_cb_replace(struct tcf_block *block, struct tcf_proto *tp,
			enum tc_setup_type type, void *type_data, bool err_stop,
			u32 *old_flags, unsigned int *old_in_hw_count,
			u32 *new_flags, unsigned int *new_in_hw_count,
			bool rtnl_held);
int tc_setup_cb_destroy(struct tcf_block *block, struct tcf_proto *tp,
			enum tc_setup_type type, void *type_data, bool err_stop,
			u32 *flags, unsigned int *in_hw_count, bool rtnl_held);
int tc_setup_cb_reoffload(struct tcf_block *block, struct tcf_proto *tp,
			  bool add, flow_setup_cb_t *cb,
			  enum tc_setup_type type, void *type_data,
			  void *cb_priv, u32 *flags, unsigned int *in_hw_count);
unsigned int tcf_exts_num_actions(struct tcf_exts *exts);

#ifdef CONFIG_NET_CLS_ACT
int tcf_qevent_init(struct tcf_qevent *qe, struct Qdisc *sch,
		    enum flow_block_binder_type binder_type,
		    struct nlattr *block_index_attr,
		    struct netlink_ext_ack *extack);
void tcf_qevent_destroy(struct tcf_qevent *qe, struct Qdisc *sch);
int tcf_qevent_validate_change(struct tcf_qevent *qe, struct nlattr *block_index_attr,
			       struct netlink_ext_ack *extack);
struct sk_buff *tcf_qevent_handle(struct tcf_qevent *qe, struct Qdisc *sch, struct sk_buff *skb,
				  struct sk_buff **to_free, int *ret);
int tcf_qevent_dump(struct sk_buff *skb, int attr_name, struct tcf_qevent *qe);
#else
static inline int tcf_qevent_init(struct tcf_qevent *qe, struct Qdisc *sch,
				  enum flow_block_binder_type binder_type,
				  struct nlattr *block_index_attr,
				  struct netlink_ext_ack *extack)
{
	return 0;
}

static inline void tcf_qevent_destroy(struct tcf_qevent *qe, struct Qdisc *sch)
{
}

static inline int tcf_qevent_validate_change(struct tcf_qevent *qe, struct nlattr *block_index_attr,
					     struct netlink_ext_ack *extack)
{
	return 0;
}

static inline struct sk_buff *
tcf_qevent_handle(struct tcf_qevent *qe, struct Qdisc *sch, struct sk_buff *skb,
		  struct sk_buff **to_free, int *ret)
{
	return skb;
}

static inline int tcf_qevent_dump(struct sk_buff *skb, int attr_name, struct tcf_qevent *qe)
{
	return 0;
}
#endif

struct tc_cls_u32_knode {
	struct tcf_exts *exts;
	struct tcf_result *res;
>>>>>>> upstream/android-13
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
<<<<<<< HEAD
	struct tc_cls_common_offload common;
=======
	struct flow_cls_common_offload common;
>>>>>>> upstream/android-13
	/* knode values */
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
<<<<<<< HEAD
			      struct tc_cls_common_offload *common)
=======
			      struct flow_cls_common_offload *common)
>>>>>>> upstream/android-13
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

/* SKIP_HW and SKIP_SW are mutually exclusive flags. */
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
<<<<<<< HEAD
tc_cls_common_offload_init(struct tc_cls_common_offload *cls_common,
=======
tc_cls_common_offload_init(struct flow_cls_common_offload *cls_common,
>>>>>>> upstream/android-13
			   const struct tcf_proto *tp, u32 flags,
			   struct netlink_ext_ack *extack)
{
	cls_common->chain_index = tp->chain->index;
	cls_common->protocol = tp->protocol;
<<<<<<< HEAD
	cls_common->prio = tp->prio;
=======
	cls_common->prio = tp->prio >> 16;
>>>>>>> upstream/android-13
	if (tc_skip_sw(flags) || flags & TCA_CLS_FLAGS_VERBOSE)
		cls_common->extack = extack;
}

<<<<<<< HEAD
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
=======
#if IS_ENABLED(CONFIG_NET_TC_SKB_EXT)
static inline struct tc_skb_ext *tc_skb_ext_alloc(struct sk_buff *skb)
{
	struct tc_skb_ext *tc_skb_ext = skb_ext_add(skb, TC_SKB_EXT);

	if (tc_skb_ext)
		memset(tc_skb_ext, 0, sizeof(*tc_skb_ext));
	return tc_skb_ext;
}
#endif
>>>>>>> upstream/android-13

enum tc_matchall_command {
	TC_CLSMATCHALL_REPLACE,
	TC_CLSMATCHALL_DESTROY,
<<<<<<< HEAD
};

struct tc_cls_matchall_offload {
	struct tc_cls_common_offload common;
	enum tc_matchall_command command;
	struct tcf_exts *exts;
=======
	TC_CLSMATCHALL_STATS,
};

struct tc_cls_matchall_offload {
	struct flow_cls_common_offload common;
	enum tc_matchall_command command;
	struct flow_rule *rule;
	struct flow_stats stats;
>>>>>>> upstream/android-13
	unsigned long cookie;
};

enum tc_clsbpf_command {
	TC_CLSBPF_OFFLOAD,
	TC_CLSBPF_STATS,
};

struct tc_cls_bpf_offload {
<<<<<<< HEAD
	struct tc_cls_common_offload common;
=======
	struct flow_cls_common_offload common;
>>>>>>> upstream/android-13
	enum tc_clsbpf_command command;
	struct tcf_exts *exts;
	struct bpf_prog *prog;
	struct bpf_prog *oldprog;
	const char *name;
	bool exts_integrated;
};

struct tc_mqprio_qopt_offload {
	/* struct tc_mqprio_qopt must always be the first element */
	struct tc_mqprio_qopt qopt;
	u16 mode;
	u16 shaper;
	u32 flags;
	u64 min_rate[TC_QOPT_MAX_QUEUE];
	u64 max_rate[TC_QOPT_MAX_QUEUE];
};

/* This structure holds cookie structure that is passed from user
 * to the kernel for actions and classifiers
 */
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
<<<<<<< HEAD
=======
	TC_MQ_GRAFT,
};

struct tc_mq_opt_offload_graft_params {
	unsigned long queue;
	u32 child_handle;
>>>>>>> upstream/android-13
};

struct tc_mq_qopt_offload {
	enum tc_mq_command command;
	u32 handle;
<<<<<<< HEAD
	struct tc_qopt_offload_stats stats;
};

=======
	union {
		struct tc_qopt_offload_stats stats;
		struct tc_mq_opt_offload_graft_params graft_params;
	};
};

enum tc_htb_command {
	/* Root */
	TC_HTB_CREATE, /* Initialize HTB offload. */
	TC_HTB_DESTROY, /* Destroy HTB offload. */

	/* Classes */
	/* Allocate qid and create leaf. */
	TC_HTB_LEAF_ALLOC_QUEUE,
	/* Convert leaf to inner, preserve and return qid, create new leaf. */
	TC_HTB_LEAF_TO_INNER,
	/* Delete leaf, while siblings remain. */
	TC_HTB_LEAF_DEL,
	/* Delete leaf, convert parent to leaf, preserving qid. */
	TC_HTB_LEAF_DEL_LAST,
	/* TC_HTB_LEAF_DEL_LAST, but delete driver data on hardware errors. */
	TC_HTB_LEAF_DEL_LAST_FORCE,
	/* Modify parameters of a node. */
	TC_HTB_NODE_MODIFY,

	/* Class qdisc */
	TC_HTB_LEAF_QUERY_QUEUE, /* Query qid by classid. */
};

struct tc_htb_qopt_offload {
	struct netlink_ext_ack *extack;
	enum tc_htb_command command;
	u32 parent_classid;
	u16 classid;
	u16 qid;
	u64 rate;
	u64 ceil;
};

#define TC_HTB_CLASSID_ROOT U32_MAX

>>>>>>> upstream/android-13
enum tc_red_command {
	TC_RED_REPLACE,
	TC_RED_DESTROY,
	TC_RED_STATS,
	TC_RED_XSTATS,
<<<<<<< HEAD
=======
	TC_RED_GRAFT,
>>>>>>> upstream/android-13
};

struct tc_red_qopt_offload_params {
	u32 min;
	u32 max;
	u32 probability;
<<<<<<< HEAD
	bool is_ecn;
=======
	u32 limit;
	bool is_ecn;
	bool is_harddrop;
	bool is_nodrop;
>>>>>>> upstream/android-13
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
<<<<<<< HEAD
=======
		u32 child_handle;
	};
};

enum tc_gred_command {
	TC_GRED_REPLACE,
	TC_GRED_DESTROY,
	TC_GRED_STATS,
};

struct tc_gred_vq_qopt_offload_params {
	bool present;
	u32 limit;
	u32 prio;
	u32 min;
	u32 max;
	bool is_ecn;
	bool is_harddrop;
	u32 probability;
	/* Only need backlog, see struct tc_prio_qopt_offload_params */
	u32 *backlog;
};

struct tc_gred_qopt_offload_params {
	bool grio_on;
	bool wred_on;
	unsigned int dp_cnt;
	unsigned int dp_def;
	struct gnet_stats_queue *qstats;
	struct tc_gred_vq_qopt_offload_params tab[MAX_DPs];
};

struct tc_gred_qopt_offload_stats {
	struct gnet_stats_basic_packed bstats[MAX_DPs];
	struct gnet_stats_queue qstats[MAX_DPs];
	struct red_stats *xstats[MAX_DPs];
};

struct tc_gred_qopt_offload {
	enum tc_gred_command command;
	u32 handle;
	u32 parent;
	union {
		struct tc_gred_qopt_offload_params set;
		struct tc_gred_qopt_offload_stats stats;
>>>>>>> upstream/android-13
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
<<<<<<< HEAD
	/* In case that a prio qdisc is offloaded and now is changed to a
	 * non-offloadedable config, it needs to update the backlog & qlen
	 * values to negate the HW backlog & qlen values (and only them).
=======
	/* At the point of un-offloading the Qdisc, the reported backlog and
	 * qlen need to be reduced by the portion that is in HW.
>>>>>>> upstream/android-13
	 */
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

<<<<<<< HEAD
=======
enum tc_root_command {
	TC_ROOT_GRAFT,
};

struct tc_root_qopt_offload {
	enum tc_root_command command;
	u32 handle;
	bool ingress;
};

enum tc_ets_command {
	TC_ETS_REPLACE,
	TC_ETS_DESTROY,
	TC_ETS_STATS,
	TC_ETS_GRAFT,
};

struct tc_ets_qopt_offload_replace_params {
	unsigned int bands;
	u8 priomap[TC_PRIO_MAX + 1];
	unsigned int quanta[TCQ_ETS_MAX_BANDS];	/* 0 for strict bands. */
	unsigned int weights[TCQ_ETS_MAX_BANDS];
	struct gnet_stats_queue *qstats;
};

struct tc_ets_qopt_offload_graft_params {
	u8 band;
	u32 child_handle;
};

struct tc_ets_qopt_offload {
	enum tc_ets_command command;
	u32 handle;
	u32 parent;
	union {
		struct tc_ets_qopt_offload_replace_params replace_params;
		struct tc_qopt_offload_stats stats;
		struct tc_ets_qopt_offload_graft_params graft_params;
	};
};

enum tc_tbf_command {
	TC_TBF_REPLACE,
	TC_TBF_DESTROY,
	TC_TBF_STATS,
};

struct tc_tbf_qopt_offload_replace_params {
	struct psched_ratecfg rate;
	u32 max_size;
	struct gnet_stats_queue *qstats;
};

struct tc_tbf_qopt_offload {
	enum tc_tbf_command command;
	u32 handle;
	u32 parent;
	union {
		struct tc_tbf_qopt_offload_replace_params replace_params;
		struct tc_qopt_offload_stats stats;
	};
};

enum tc_fifo_command {
	TC_FIFO_REPLACE,
	TC_FIFO_DESTROY,
	TC_FIFO_STATS,
};

struct tc_fifo_qopt_offload {
	enum tc_fifo_command command;
	u32 handle;
	u32 parent;
	union {
		struct tc_qopt_offload_stats stats;
	};
};

>>>>>>> upstream/android-13
#endif
