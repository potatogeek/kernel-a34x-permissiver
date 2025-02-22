<<<<<<< HEAD
/*
 * Copyright (c) 2016, Amir Vadai <amir@vadai.me>
 * Copyright (c) 2016, Mellanox Technologies. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (c) 2016, Amir Vadai <amir@vadai.me>
 * Copyright (c) 2016, Mellanox Technologies. All rights reserved.
>>>>>>> upstream/android-13
 */

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
<<<<<<< HEAD
	struct tcf_tunnel_key_params *params = rtnl_dereference(t->params);

	if (a->ops && a->ops->type == TCA_ACT_TUNNEL_KEY)
=======
	struct tcf_tunnel_key_params *params;

	params = rcu_dereference_protected(t->params,
					   lockdep_is_held(&a->tcfa_lock));
	if (a->ops && a->ops->id == TCA_ID_TUNNEL_KEY)
>>>>>>> upstream/android-13
		return params->tcft_action == TCA_TUNNEL_KEY_ACT_SET;
#endif
	return false;
}

static inline bool is_tcf_tunnel_release(const struct tc_action *a)
{
#ifdef CONFIG_NET_CLS_ACT
	struct tcf_tunnel_key *t = to_tunnel_key(a);
<<<<<<< HEAD
	struct tcf_tunnel_key_params *params = rtnl_dereference(t->params);

	if (a->ops && a->ops->type == TCA_ACT_TUNNEL_KEY)
=======
	struct tcf_tunnel_key_params *params;

	params = rcu_dereference_protected(t->params,
					   lockdep_is_held(&a->tcfa_lock));
	if (a->ops && a->ops->id == TCA_ID_TUNNEL_KEY)
>>>>>>> upstream/android-13
		return params->tcft_action == TCA_TUNNEL_KEY_ACT_RELEASE;
#endif
	return false;
}

static inline struct ip_tunnel_info *tcf_tunnel_info(const struct tc_action *a)
{
#ifdef CONFIG_NET_CLS_ACT
	struct tcf_tunnel_key *t = to_tunnel_key(a);
<<<<<<< HEAD
	struct tcf_tunnel_key_params *params = rtnl_dereference(t->params);
=======
	struct tcf_tunnel_key_params *params;

	params = rcu_dereference_protected(t->params,
					   lockdep_is_held(&a->tcfa_lock));
>>>>>>> upstream/android-13

	return &params->tcft_enc_metadata->u.tun_info;
#else
	return NULL;
#endif
}
<<<<<<< HEAD
=======

static inline struct ip_tunnel_info *
tcf_tunnel_info_copy(const struct tc_action *a)
{
#ifdef CONFIG_NET_CLS_ACT
	struct ip_tunnel_info *tun = tcf_tunnel_info(a);

	if (tun) {
		size_t tun_size = sizeof(*tun) + tun->options_len;
		struct ip_tunnel_info *tun_copy = kmemdup(tun, tun_size,
							  GFP_ATOMIC);

		return tun_copy;
	}
#endif
	return NULL;
}
>>>>>>> upstream/android-13
#endif /* __NET_TC_TUNNEL_KEY_H */
