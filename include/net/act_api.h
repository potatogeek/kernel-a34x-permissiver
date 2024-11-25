/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __NET_ACT_API_H
#define __NET_ACT_API_H

/*
 * Public action API for classifiers/qdiscs
*/

#include <linux/refcount.h>
#include <net/sch_generic.h>
#include <net/pkt_sched.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>

struct tcf_idrinfo {
<<<<<<< HEAD
	spinlock_t	lock;
=======
	struct mutex	lock;
>>>>>>> upstream/android-13
	struct idr	action_idr;
	struct net	*net;
};

struct tc_action_ops;

struct tc_action {
	const struct tc_action_ops	*ops;
	__u32				type; /* for backward compat(TCA_OLD_COMPAT) */
<<<<<<< HEAD
	__u32				order;
=======
>>>>>>> upstream/android-13
	struct tcf_idrinfo		*idrinfo;

	u32				tcfa_index;
	refcount_t			tcfa_refcnt;
	atomic_t			tcfa_bindcnt;
	int				tcfa_action;
	struct tcf_t			tcfa_tm;
	struct gnet_stats_basic_packed	tcfa_bstats;
<<<<<<< HEAD
=======
	struct gnet_stats_basic_packed	tcfa_bstats_hw;
>>>>>>> upstream/android-13
	struct gnet_stats_queue		tcfa_qstats;
	struct net_rate_estimator __rcu *tcfa_rate_est;
	spinlock_t			tcfa_lock;
	struct gnet_stats_basic_cpu __percpu *cpu_bstats;
<<<<<<< HEAD
	struct gnet_stats_queue __percpu *cpu_qstats;
	struct tc_cookie	__rcu *act_cookie;
	struct tcf_chain	*goto_chain;
=======
	struct gnet_stats_basic_cpu __percpu *cpu_bstats_hw;
	struct gnet_stats_queue __percpu *cpu_qstats;
	struct tc_cookie	__rcu *act_cookie;
	struct tcf_chain	__rcu *goto_chain;
	u32			tcfa_flags;
	u8			hw_stats;
	u8			used_hw_stats;
	bool			used_hw_stats_valid;
>>>>>>> upstream/android-13
};
#define tcf_index	common.tcfa_index
#define tcf_refcnt	common.tcfa_refcnt
#define tcf_bindcnt	common.tcfa_bindcnt
#define tcf_action	common.tcfa_action
#define tcf_tm		common.tcfa_tm
#define tcf_bstats	common.tcfa_bstats
#define tcf_qstats	common.tcfa_qstats
#define tcf_rate_est	common.tcfa_rate_est
#define tcf_lock	common.tcfa_lock

<<<<<<< HEAD
=======
#define TCA_ACT_HW_STATS_ANY (TCA_ACT_HW_STATS_IMMEDIATE | \
			      TCA_ACT_HW_STATS_DELAYED)

/* Reserve 16 bits for user-space. See TCA_ACT_FLAGS_NO_PERCPU_STATS. */
#define TCA_ACT_FLAGS_USER_BITS 16
#define TCA_ACT_FLAGS_USER_MASK 0xffff
#define TCA_ACT_FLAGS_POLICE	(1U << TCA_ACT_FLAGS_USER_BITS)
#define TCA_ACT_FLAGS_BIND	(1U << (TCA_ACT_FLAGS_USER_BITS + 1))
#define TCA_ACT_FLAGS_REPLACE	(1U << (TCA_ACT_FLAGS_USER_BITS + 2))
#define TCA_ACT_FLAGS_NO_RTNL	(1U << (TCA_ACT_FLAGS_USER_BITS + 3))

>>>>>>> upstream/android-13
/* Update lastuse only if needed, to avoid dirtying a cache line.
 * We use a temp variable to avoid fetching jiffies twice.
 */
static inline void tcf_lastuse_update(struct tcf_t *tm)
{
	unsigned long now = jiffies;

	if (tm->lastuse != now)
		tm->lastuse = now;
	if (unlikely(!tm->firstuse))
		tm->firstuse = now;
}

static inline void tcf_tm_dump(struct tcf_t *dtm, const struct tcf_t *stm)
{
	dtm->install = jiffies_to_clock_t(jiffies - stm->install);
	dtm->lastuse = jiffies_to_clock_t(jiffies - stm->lastuse);
	dtm->firstuse = stm->firstuse ?
		jiffies_to_clock_t(jiffies - stm->firstuse) : 0;
	dtm->expires = jiffies_to_clock_t(stm->expires);
}

#ifdef CONFIG_NET_CLS_ACT

#define ACT_P_CREATED 1
#define ACT_P_DELETED 1

<<<<<<< HEAD
struct tc_action_ops {
	struct list_head head;
	char    kind[IFNAMSIZ];
	__u32   type; /* TBD to match kind */
=======
typedef void (*tc_action_priv_destructor)(void *priv);

struct tc_action_ops {
	struct list_head head;
	char    kind[IFNAMSIZ];
	enum tca_id  id; /* identifier should match kind */
>>>>>>> upstream/android-13
	size_t	size;
	struct module		*owner;
	int     (*act)(struct sk_buff *, const struct tc_action *,
		       struct tcf_result *); /* called under RCU BH lock*/
	int     (*dump)(struct sk_buff *, struct tc_action *, int, int);
	void	(*cleanup)(struct tc_action *);
<<<<<<< HEAD
	int     (*lookup)(struct net *net, struct tc_action **a, u32 index,
			  struct netlink_ext_ack *extack);
	int     (*init)(struct net *net, struct nlattr *nla,
			struct nlattr *est, struct tc_action **act, int ovr,
			int bind, bool rtnl_held,
			struct netlink_ext_ack *extack);
=======
	int     (*lookup)(struct net *net, struct tc_action **a, u32 index);
	int     (*init)(struct net *net, struct nlattr *nla,
			struct nlattr *est, struct tc_action **act,
			struct tcf_proto *tp,
			u32 flags, struct netlink_ext_ack *extack);
>>>>>>> upstream/android-13
	int     (*walk)(struct net *, struct sk_buff *,
			struct netlink_callback *, int,
			const struct tc_action_ops *,
			struct netlink_ext_ack *);
<<<<<<< HEAD
	void	(*stats_update)(struct tc_action *, u64, u32, u64);
	size_t  (*get_fill_size)(const struct tc_action *act);
	struct net_device *(*get_dev)(const struct tc_action *a);
	void	(*put_dev)(struct net_device *dev);
=======
	void	(*stats_update)(struct tc_action *, u64, u64, u64, u64, bool);
	size_t  (*get_fill_size)(const struct tc_action *act);
	struct net_device *(*get_dev)(const struct tc_action *a,
				      tc_action_priv_destructor *destructor);
	struct psample_group *
	(*get_psample_group)(const struct tc_action *a,
			     tc_action_priv_destructor *destructor);
>>>>>>> upstream/android-13
};

struct tc_action_net {
	struct tcf_idrinfo *idrinfo;
	const struct tc_action_ops *ops;
};

static inline
int tc_action_net_init(struct net *net, struct tc_action_net *tn,
		       const struct tc_action_ops *ops)
{
	int err = 0;

	tn->idrinfo = kmalloc(sizeof(*tn->idrinfo), GFP_KERNEL);
	if (!tn->idrinfo)
		return -ENOMEM;
	tn->ops = ops;
	tn->idrinfo->net = net;
<<<<<<< HEAD
	spin_lock_init(&tn->idrinfo->lock);
=======
	mutex_init(&tn->idrinfo->lock);
>>>>>>> upstream/android-13
	idr_init(&tn->idrinfo->action_idr);
	return err;
}

void tcf_idrinfo_destroy(const struct tc_action_ops *ops,
			 struct tcf_idrinfo *idrinfo);

static inline void tc_action_net_exit(struct list_head *net_list,
				      unsigned int id)
{
	struct net *net;

	rtnl_lock();
	list_for_each_entry(net, net_list, exit_list) {
		struct tc_action_net *tn = net_generic(net, id);

		tcf_idrinfo_destroy(tn->ops, tn->idrinfo);
		kfree(tn->idrinfo);
	}
	rtnl_unlock();
}

int tcf_generic_walker(struct tc_action_net *tn, struct sk_buff *skb,
		       struct netlink_callback *cb, int type,
		       const struct tc_action_ops *ops,
		       struct netlink_ext_ack *extack);
int tcf_idr_search(struct tc_action_net *tn, struct tc_action **a, u32 index);
int tcf_idr_create(struct tc_action_net *tn, u32 index, struct nlattr *est,
		   struct tc_action **a, const struct tc_action_ops *ops,
<<<<<<< HEAD
		   int bind, bool cpustats);
void tcf_idr_insert(struct tc_action_net *tn, struct tc_action *a);

void tcf_idr_cleanup(struct tc_action_net *tn, u32 index);
int tcf_idr_check_alloc(struct tc_action_net *tn, u32 *index,
			struct tc_action **a, int bind);
int __tcf_idr_release(struct tc_action *a, bool bind, bool strict);

static inline int tcf_idr_release(struct tc_action *a, bool bind)
{
	return __tcf_idr_release(a, bind, false);
}
=======
		   int bind, bool cpustats, u32 flags);
int tcf_idr_create_from_flags(struct tc_action_net *tn, u32 index,
			      struct nlattr *est, struct tc_action **a,
			      const struct tc_action_ops *ops, int bind,
			      u32 flags);
void tcf_idr_insert_many(struct tc_action *actions[]);
void tcf_idr_cleanup(struct tc_action_net *tn, u32 index);
int tcf_idr_check_alloc(struct tc_action_net *tn, u32 *index,
			struct tc_action **a, int bind);
int tcf_idr_release(struct tc_action *a, bool bind);
>>>>>>> upstream/android-13

int tcf_register_action(struct tc_action_ops *a, struct pernet_operations *ops);
int tcf_unregister_action(struct tc_action_ops *a,
			  struct pernet_operations *ops);
int tcf_action_destroy(struct tc_action *actions[], int bind);
int tcf_action_exec(struct sk_buff *skb, struct tc_action **actions,
		    int nr_actions, struct tcf_result *res);
int tcf_action_init(struct net *net, struct tcf_proto *tp, struct nlattr *nla,
<<<<<<< HEAD
		    struct nlattr *est, char *name, int ovr, int bind,
		    struct tc_action *actions[], size_t *attr_size,
		    bool rtnl_held, struct netlink_ext_ack *extack);
struct tc_action *tcf_action_init_1(struct net *net, struct tcf_proto *tp,
				    struct nlattr *nla, struct nlattr *est,
				    char *name, int ovr, int bind,
				    bool rtnl_held,
				    struct netlink_ext_ack *extack);
int tcf_action_dump(struct sk_buff *skb, struct tc_action *actions[], int bind,
		    int ref);
int tcf_action_dump_old(struct sk_buff *skb, struct tc_action *a, int, int);
int tcf_action_dump_1(struct sk_buff *skb, struct tc_action *a, int, int);
int tcf_action_copy_stats(struct sk_buff *, struct tc_action *, int);

#endif /* CONFIG_NET_CLS_ACT */

static inline void tcf_action_stats_update(struct tc_action *a, u64 bytes,
					   u64 packets, u64 lastuse)
=======
		    struct nlattr *est,
		    struct tc_action *actions[], int init_res[], size_t *attr_size,
		    u32 flags, struct netlink_ext_ack *extack);
struct tc_action_ops *tc_action_load_ops(struct nlattr *nla, bool police,
					 bool rtnl_held,
					 struct netlink_ext_ack *extack);
struct tc_action *tcf_action_init_1(struct net *net, struct tcf_proto *tp,
				    struct nlattr *nla, struct nlattr *est,
				    struct tc_action_ops *a_o, int *init_res,
				    u32 flags, struct netlink_ext_ack *extack);
int tcf_action_dump(struct sk_buff *skb, struct tc_action *actions[], int bind,
		    int ref, bool terse);
int tcf_action_dump_old(struct sk_buff *skb, struct tc_action *a, int, int);
int tcf_action_dump_1(struct sk_buff *skb, struct tc_action *a, int, int);

static inline void tcf_action_update_bstats(struct tc_action *a,
					    struct sk_buff *skb)
{
	if (likely(a->cpu_bstats)) {
		bstats_cpu_update(this_cpu_ptr(a->cpu_bstats), skb);
		return;
	}
	spin_lock(&a->tcfa_lock);
	bstats_update(&a->tcfa_bstats, skb);
	spin_unlock(&a->tcfa_lock);
}

static inline void tcf_action_inc_drop_qstats(struct tc_action *a)
{
	if (likely(a->cpu_qstats)) {
		qstats_drop_inc(this_cpu_ptr(a->cpu_qstats));
		return;
	}
	spin_lock(&a->tcfa_lock);
	qstats_drop_inc(&a->tcfa_qstats);
	spin_unlock(&a->tcfa_lock);
}

static inline void tcf_action_inc_overlimit_qstats(struct tc_action *a)
{
	if (likely(a->cpu_qstats)) {
		qstats_overlimit_inc(this_cpu_ptr(a->cpu_qstats));
		return;
	}
	spin_lock(&a->tcfa_lock);
	qstats_overlimit_inc(&a->tcfa_qstats);
	spin_unlock(&a->tcfa_lock);
}

void tcf_action_update_stats(struct tc_action *a, u64 bytes, u64 packets,
			     u64 drops, bool hw);
int tcf_action_copy_stats(struct sk_buff *, struct tc_action *, int);

int tcf_action_check_ctrlact(int action, struct tcf_proto *tp,
			     struct tcf_chain **handle,
			     struct netlink_ext_ack *newchain);
struct tcf_chain *tcf_action_set_ctrlact(struct tc_action *a, int action,
					 struct tcf_chain *newchain);

#ifdef CONFIG_INET
DECLARE_STATIC_KEY_FALSE(tcf_frag_xmit_count);
#endif

int tcf_dev_queue_xmit(struct sk_buff *skb, int (*xmit)(struct sk_buff *skb));
#endif /* CONFIG_NET_CLS_ACT */

static inline void tcf_action_stats_update(struct tc_action *a, u64 bytes,
					   u64 packets, u64 drops,
					   u64 lastuse, bool hw)
>>>>>>> upstream/android-13
{
#ifdef CONFIG_NET_CLS_ACT
	if (!a->ops->stats_update)
		return;

<<<<<<< HEAD
	a->ops->stats_update(a, bytes, packets, lastuse);
#endif
}

#ifdef CONFIG_NET_CLS_ACT
int tc_setup_cb_egdev_register(const struct net_device *dev,
			       tc_setup_cb_t *cb, void *cb_priv);
void tc_setup_cb_egdev_unregister(const struct net_device *dev,
				  tc_setup_cb_t *cb, void *cb_priv);
int tc_setup_cb_egdev_call(const struct net_device *dev,
			   enum tc_setup_type type, void *type_data,
			   bool err_stop);
#else
static inline
int tc_setup_cb_egdev_register(const struct net_device *dev,
			       tc_setup_cb_t *cb, void *cb_priv)
{
	return 0;
}

static inline
void tc_setup_cb_egdev_unregister(const struct net_device *dev,
				  tc_setup_cb_t *cb, void *cb_priv)
{
}

static inline
int tc_setup_cb_egdev_call(const struct net_device *dev,
			   enum tc_setup_type type, void *type_data,
			   bool err_stop)
{
	return 0;
}
#endif
=======
	a->ops->stats_update(a, bytes, packets, drops, lastuse, hw);
#endif
}

>>>>>>> upstream/android-13

#endif
