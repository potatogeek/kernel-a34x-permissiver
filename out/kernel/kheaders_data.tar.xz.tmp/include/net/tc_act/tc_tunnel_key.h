

#ifndef __NET_TC_TUNNEL_KEY_H
#define __NET_TC_TUNNEL_KEY_H

#include <net/act_api.h>
#include <linux/tc_act/tc_tunnel_key.h>
#include <net/dst_metadata.h>

struct tcf_tunnel_key_params {
	struct rcu_head		rcu;
	int			tcft_action;
	struct metadata_dst     *tcft_enc_metadata;
};

struct tcf_tunnel_key {
	struct tc_action	      common;
	struct tcf_tunnel_key_params __rcu *params;
};

#define to_tunnel_key(a) ((struct tcf_tunnel_key *)a)

static inline bool is_tcf_tunnel_set(const struct tc_action *a)
{
#ifdef CONFIG_NET_CLS_ACT
	struct tcf_tunnel_key *t = to_tunnel_key(a);
	struct tcf_tunnel_key_params *params = rtnl_dereference(t->params);

	if (a->ops && a->ops->type == TCA_ACT_TUNNEL_KEY)
		return params->tcft_action == TCA_TUNNEL_KEY_ACT_SET;
#endif
	return false;
}

static inline bool is_tcf_tunnel_release(const struct tc_action *a)
{
#ifdef CONFIG_NET_CLS_ACT
	struct tcf_tunnel_key *t = to_tunnel_key(a);
	struct tcf_tunnel_key_params *params = rtnl_dereference(t->params);

	if (a->ops && a->ops->type == TCA_ACT_TUNNEL_KEY)
		return params->tcft_action == TCA_TUNNEL_KEY_ACT_RELEASE;
#endif
	return false;
}

static inline struct ip_tunnel_info *tcf_tunnel_info(const struct tc_action *a)
{
#ifdef CONFIG_NET_CLS_ACT
	struct tcf_tunnel_key *t = to_tunnel_key(a);
	struct tcf_tunnel_key_params *params = rtnl_dereference(t->params);

	return &params->tcft_enc_metadata->u.tun_info;
#else
	return NULL;
#endif
}
#endif 
