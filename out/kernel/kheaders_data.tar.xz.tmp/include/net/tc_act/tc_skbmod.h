

#ifndef __NET_TC_SKBMOD_H
#define __NET_TC_SKBMOD_H

#include <net/act_api.h>
#include <linux/tc_act/tc_skbmod.h>

struct tcf_skbmod_params {
	struct rcu_head	rcu;
	u64	flags; 
	u8	eth_dst[ETH_ALEN];
	u16	eth_type;
	u8	eth_src[ETH_ALEN];
};

struct tcf_skbmod {
	struct tc_action	common;
	struct tcf_skbmod_params __rcu *skbmod_p;
};
#define to_skbmod(a) ((struct tcf_skbmod *)a)

#endif 
