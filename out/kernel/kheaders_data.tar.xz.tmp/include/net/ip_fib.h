

#ifndef _NET_IP_FIB_H
#define _NET_IP_FIB_H

#include <net/flow.h>
#include <linux/seq_file.h>
#include <linux/rcupdate.h>
#include <net/fib_notifier.h>
#include <net/fib_rules.h>
#include <net/inetpeer.h>
#include <linux/percpu.h>
#include <linux/notifier.h>
#include <linux/refcount.h>

struct fib_config {
	u8			fc_dst_len;
	u8			fc_tos;
	u8			fc_protocol;
	u8			fc_scope;
	u8			fc_type;
	
	u32			fc_table;
	__be32			fc_dst;
	__be32			fc_gw;
	int			fc_oif;
	u32			fc_flags;
	u32			fc_priority;
	__be32			fc_prefsrc;
	struct nlattr		*fc_mx;
	struct rtnexthop	*fc_mp;
	int			fc_mx_len;
	int			fc_mp_len;
	u32			fc_flow;
	u32			fc_nlflags;
	struct nl_info		fc_nlinfo;
	struct nlattr		*fc_encap;
	u16			fc_encap_type;
};

struct fib_info;
struct rtable;

struct fib_nh_exception {
	struct fib_nh_exception __rcu	*fnhe_next;
	int				fnhe_genid;
	__be32				fnhe_daddr;
	u32				fnhe_pmtu;
	bool				fnhe_mtu_locked;
	__be32				fnhe_gw;
	unsigned long			fnhe_expires;
	struct rtable __rcu		*fnhe_rth_input;
	struct rtable __rcu		*fnhe_rth_output;
	unsigned long			fnhe_stamp;
	struct rcu_head			rcu;
};

struct fnhe_hash_bucket {
	struct fib_nh_exception __rcu	*chain;
};

#define FNHE_HASH_SHIFT		11
#define FNHE_HASH_SIZE		(1 << FNHE_HASH_SHIFT)
#define FNHE_RECLAIM_DEPTH	5

struct fib_nh {
	struct net_device	*nh_dev;
	struct hlist_node	nh_hash;
	struct fib_info		*nh_parent;
	unsigned int		nh_flags;
	unsigned char		nh_scope;
#ifdef CONFIG_IP_ROUTE_MULTIPATH
	int			nh_weight;
	atomic_t		nh_upper_bound;
#endif
#ifdef CONFIG_IP_ROUTE_CLASSID
	__u32			nh_tclassid;
#endif
	int			nh_oif;
	__be32			nh_gw;
	__be32			nh_saddr;
	int			nh_saddr_genid;
	struct rtable __rcu * __percpu *nh_pcpu_rth_output;
	struct rtable __rcu	*nh_rth_input;
	struct fnhe_hash_bucket	__rcu *nh_exceptions;
	struct lwtunnel_state	*nh_lwtstate;
};



struct fib_info {
	struct hlist_node	fib_hash;
	struct hlist_node	fib_lhash;
	struct net		*fib_net;
	int			fib_treeref;
	refcount_t		fib_clntref;
	unsigned int		fib_flags;
	unsigned char		fib_dead;
	unsigned char		fib_protocol;
	unsigned char		fib_scope;
	unsigned char		fib_type;
	__be32			fib_prefsrc;
	u32			fib_tb_id;
	u32			fib_priority;
	struct dst_metrics	*fib_metrics;
#define fib_mtu fib_metrics->metrics[RTAX_MTU-1]
#define fib_window fib_metrics->metrics[RTAX_WINDOW-1]
#define fib_rtt fib_metrics->metrics[RTAX_RTT-1]
#define fib_advmss fib_metrics->metrics[RTAX_ADVMSS-1]
	int			fib_nhs;
	struct rcu_head		rcu;
	struct fib_nh		fib_nh[0];
#define fib_dev		fib_nh[0].nh_dev
};


#ifdef CONFIG_IP_MULTIPLE_TABLES
struct fib_rule;
#endif

struct fib_table;
struct fib_result {
	__be32		prefix;
	unsigned char	prefixlen;
	unsigned char	nh_sel;
	unsigned char	type;
	unsigned char	scope;
	u32		tclassid;
	struct fib_info *fi;
	struct fib_table *table;
	struct hlist_head *fa_head;
};

struct fib_result_nl {
	__be32		fl_addr;   
	u32		fl_mark;
	unsigned char	fl_tos;
	unsigned char   fl_scope;
	unsigned char   tb_id_in;

	unsigned char   tb_id;      
	unsigned char	prefixlen;
	unsigned char	nh_sel;
	unsigned char	type;
	unsigned char	scope;
	int             err;
};

#ifdef CONFIG_IP_ROUTE_MULTIPATH
#define FIB_RES_NH(res)		((res).fi->fib_nh[(res).nh_sel])
#else 
#define FIB_RES_NH(res)		((res).fi->fib_nh[0])
#endif 

#ifdef CONFIG_IP_MULTIPLE_TABLES
#define FIB_TABLE_HASHSZ 256
#else
#define FIB_TABLE_HASHSZ 2
#endif

__be32 fib_info_update_nh_saddr(struct net *net, struct fib_nh *nh);

#define FIB_RES_SADDR(net, res)				\
	((FIB_RES_NH(res).nh_saddr_genid ==		\
	  atomic_read(&(net)->ipv4.dev_addr_genid)) ?	\
	 FIB_RES_NH(res).nh_saddr :			\
	 fib_info_update_nh_saddr((net), &FIB_RES_NH(res)))
#define FIB_RES_GW(res)			(FIB_RES_NH(res).nh_gw)
#define FIB_RES_DEV(res)		(FIB_RES_NH(res).nh_dev)
#define FIB_RES_OIF(res)		(FIB_RES_NH(res).nh_oif)

#define FIB_RES_PREFSRC(net, res)	((res).fi->fib_prefsrc ? : \
					 FIB_RES_SADDR(net, res))

struct fib_entry_notifier_info {
	struct fib_notifier_info info; 
	u32 dst;
	int dst_len;
	struct fib_info *fi;
	u8 tos;
	u8 type;
	u32 tb_id;
};

struct fib_nh_notifier_info {
	struct fib_notifier_info info; 
	struct fib_nh *fib_nh;
};

int call_fib4_notifier(struct notifier_block *nb, struct net *net,
		       enum fib_event_type event_type,
		       struct fib_notifier_info *info);
int call_fib4_notifiers(struct net *net, enum fib_event_type event_type,
			struct fib_notifier_info *info);

int __net_init fib4_notifier_init(struct net *net);
void __net_exit fib4_notifier_exit(struct net *net);

void fib_notify(struct net *net, struct notifier_block *nb);

struct fib_table {
	struct hlist_node	tb_hlist;
	u32			tb_id;
	int			tb_num_default;
	struct rcu_head		rcu;
	unsigned long 		*tb_data;
	unsigned long		__data[0];
};

int fib_table_lookup(struct fib_table *tb, const struct flowi4 *flp,
		     struct fib_result *res, int fib_flags);
int fib_table_insert(struct net *, struct fib_table *, struct fib_config *,
		     struct netlink_ext_ack *extack);
int fib_table_delete(struct net *, struct fib_table *, struct fib_config *,
		     struct netlink_ext_ack *extack);
int fib_table_dump(struct fib_table *table, struct sk_buff *skb,
		   struct netlink_callback *cb);
int fib_table_flush(struct net *net, struct fib_table *table, bool flush_all);
struct fib_table *fib_trie_unmerge(struct fib_table *main_tb);
void fib_table_flush_external(struct fib_table *table);
void fib_free_table(struct fib_table *tb);

#ifndef CONFIG_IP_MULTIPLE_TABLES

#define TABLE_LOCAL_INDEX	(RT_TABLE_LOCAL & (FIB_TABLE_HASHSZ - 1))
#define TABLE_MAIN_INDEX	(RT_TABLE_MAIN  & (FIB_TABLE_HASHSZ - 1))

static inline struct fib_table *fib_get_table(struct net *net, u32 id)
{
	struct hlist_node *tb_hlist;
	struct hlist_head *ptr;

	ptr = id == RT_TABLE_LOCAL ?
		&net->ipv4.fib_table_hash[TABLE_LOCAL_INDEX] :
		&net->ipv4.fib_table_hash[TABLE_MAIN_INDEX];

	tb_hlist = rcu_dereference_rtnl(hlist_first_rcu(ptr));

	return hlist_entry(tb_hlist, struct fib_table, tb_hlist);
}

static inline struct fib_table *fib_new_table(struct net *net, u32 id)
{
	return fib_get_table(net, id);
}

static inline int fib_lookup(struct net *net, const struct flowi4 *flp,
			     struct fib_result *res, unsigned int flags)
{
	struct fib_table *tb;
	int err = -ENETUNREACH;

	rcu_read_lock();

	tb = fib_get_table(net, RT_TABLE_MAIN);
	if (tb)
		err = fib_table_lookup(tb, flp, res, flags | FIB_LOOKUP_NOREF);

	if (err == -EAGAIN)
		err = -ENETUNREACH;

	rcu_read_unlock();

	return err;
}

static inline bool fib4_rule_default(const struct fib_rule *rule)
{
	return true;
}

static inline int fib4_rules_dump(struct net *net, struct notifier_block *nb)
{
	return 0;
}

static inline unsigned int fib4_rules_seq_read(struct net *net)
{
	return 0;
}

static inline bool fib4_rules_early_flow_dissect(struct net *net,
						 struct sk_buff *skb,
						 struct flowi4 *fl4,
						 struct flow_keys *flkeys)
{
	return false;
}
#else 
int __net_init fib4_rules_init(struct net *net);
void __net_exit fib4_rules_exit(struct net *net);

struct fib_table *fib_new_table(struct net *net, u32 id);
struct fib_table *fib_get_table(struct net *net, u32 id);

int __fib_lookup(struct net *net, struct flowi4 *flp,
		 struct fib_result *res, unsigned int flags);

static inline int fib_lookup(struct net *net, struct flowi4 *flp,
			     struct fib_result *res, unsigned int flags)
{
	struct fib_table *tb;
	int err = -ENETUNREACH;

	flags |= FIB_LOOKUP_NOREF;
	if (net->ipv4.fib_has_custom_rules)
		return __fib_lookup(net, flp, res, flags);

	rcu_read_lock();

	res->tclassid = 0;

	tb = rcu_dereference_rtnl(net->ipv4.fib_main);
	if (tb)
		err = fib_table_lookup(tb, flp, res, flags);

	if (!err)
		goto out;

	tb = rcu_dereference_rtnl(net->ipv4.fib_default);
	if (tb)
		err = fib_table_lookup(tb, flp, res, flags);

out:
	if (err == -EAGAIN)
		err = -ENETUNREACH;

	rcu_read_unlock();

	return err;
}

bool fib4_rule_default(const struct fib_rule *rule);
int fib4_rules_dump(struct net *net, struct notifier_block *nb);
unsigned int fib4_rules_seq_read(struct net *net);

static inline bool fib4_rules_early_flow_dissect(struct net *net,
						 struct sk_buff *skb,
						 struct flowi4 *fl4,
						 struct flow_keys *flkeys)
{
	unsigned int flag = FLOW_DISSECTOR_F_STOP_AT_ENCAP;

	if (!net->ipv4.fib_rules_require_fldissect)
		return false;

	skb_flow_dissect_flow_keys(skb, flkeys, flag);
	fl4->fl4_sport = flkeys->ports.src;
	fl4->fl4_dport = flkeys->ports.dst;
	fl4->flowi4_proto = flkeys->basic.ip_proto;

	return true;
}

#endif 


extern const struct nla_policy rtm_ipv4_policy[];
void ip_fib_init(void);
__be32 fib_compute_spec_dst(struct sk_buff *skb);
int fib_validate_source(struct sk_buff *skb, __be32 src, __be32 dst,
			u8 tos, int oif, struct net_device *dev,
			struct in_device *idev, u32 *itag);
#ifdef CONFIG_IP_ROUTE_CLASSID
static inline int fib_num_tclassid_users(struct net *net)
{
	return net->ipv4.fib_num_tclassid_users;
}
#else
static inline int fib_num_tclassid_users(struct net *net)
{
	return 0;
}
#endif
int fib_unmerge(struct net *net);


int ip_fib_check_default(__be32 gw, struct net_device *dev);
int fib_sync_down_dev(struct net_device *dev, unsigned long event, bool force);
int fib_sync_down_addr(struct net_device *dev, __be32 local);
int fib_sync_up(struct net_device *dev, unsigned int nh_flags);
void fib_sync_mtu(struct net_device *dev, u32 orig_mtu);

#ifdef CONFIG_IP_ROUTE_MULTIPATH
int fib_multipath_hash(const struct net *net, const struct flowi4 *fl4,
		       const struct sk_buff *skb, struct flow_keys *flkeys);
#endif
void fib_select_multipath(struct fib_result *res, int hash);
void fib_select_path(struct net *net, struct fib_result *res,
		     struct flowi4 *fl4, const struct sk_buff *skb);


void fib_trie_init(void);
struct fib_table *fib_trie_table(u32 id, struct fib_table *alias);

static inline void fib_combine_itag(u32 *itag, const struct fib_result *res)
{
#ifdef CONFIG_IP_ROUTE_CLASSID
#ifdef CONFIG_IP_MULTIPLE_TABLES
	u32 rtag;
#endif
	*itag = FIB_RES_NH(*res).nh_tclassid<<16;
#ifdef CONFIG_IP_MULTIPLE_TABLES
	rtag = res->tclassid;
	if (*itag == 0)
		*itag = (rtag<<16);
	*itag |= (rtag>>16);
#endif
#endif
}

void free_fib_info(struct fib_info *fi);

static inline void fib_info_hold(struct fib_info *fi)
{
	refcount_inc(&fi->fib_clntref);
}

static inline void fib_info_put(struct fib_info *fi)
{
	if (refcount_dec_and_test(&fi->fib_clntref))
		free_fib_info(fi);
}

#ifdef CONFIG_PROC_FS
int __net_init fib_proc_init(struct net *net);
void __net_exit fib_proc_exit(struct net *net);
#else
static inline int fib_proc_init(struct net *net)
{
	return 0;
}
static inline void fib_proc_exit(struct net *net)
{
}
#endif

u32 ip_mtu_from_fib_result(struct fib_result *res, __be32 daddr);

#endif  
