<<<<<<< HEAD
/*
 * (C) 2008 Krzysztof Piotr Oledzki <ole@ans.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * (C) 2008 Krzysztof Piotr Oledzki <ole@ans.pl>
>>>>>>> upstream/android-13
 */

#ifndef _NF_CONNTRACK_ACCT_H
#define _NF_CONNTRACK_ACCT_H
#include <net/net_namespace.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_extend.h>

struct nf_conn_counter {
	atomic64_t packets;
	atomic64_t bytes;
};

struct nf_conn_acct {
	struct nf_conn_counter counter[IP_CT_DIR_MAX];
};

static inline
struct nf_conn_acct *nf_conn_acct_find(const struct nf_conn *ct)
{
	return nf_ct_ext_find(ct, NF_CT_EXT_ACCT);
}

static inline
struct nf_conn_acct *nf_ct_acct_ext_add(struct nf_conn *ct, gfp_t gfp)
{
<<<<<<< HEAD
=======
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
>>>>>>> upstream/android-13
	struct net *net = nf_ct_net(ct);
	struct nf_conn_acct *acct;

	if (!net->ct.sysctl_acct)
		return NULL;

	acct = nf_ct_ext_add(ct, NF_CT_EXT_ACCT, gfp);
	if (!acct)
		pr_debug("failed to add accounting extension area");


	return acct;
<<<<<<< HEAD
};

unsigned int seq_print_acct(struct seq_file *s, const struct nf_conn *ct,
			    int dir);
=======
#else
	return NULL;
#endif
}
>>>>>>> upstream/android-13

/* Check if connection tracking accounting is enabled */
static inline bool nf_ct_acct_enabled(struct net *net)
{
<<<<<<< HEAD
	return net->ct.sysctl_acct != 0;
=======
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
	return net->ct.sysctl_acct != 0;
#else
	return false;
#endif
>>>>>>> upstream/android-13
}

/* Enable/disable connection tracking accounting */
static inline void nf_ct_set_acct(struct net *net, bool enable)
{
<<<<<<< HEAD
	net->ct.sysctl_acct = enable;
}

int nf_conntrack_acct_pernet_init(struct net *net);
void nf_conntrack_acct_pernet_fini(struct net *net);

int nf_conntrack_acct_init(void);
void nf_conntrack_acct_fini(void);
=======
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
	net->ct.sysctl_acct = enable;
#endif
}

void nf_ct_acct_add(struct nf_conn *ct, u32 dir, unsigned int packets,
		    unsigned int bytes);

static inline void nf_ct_acct_update(struct nf_conn *ct, u32 dir,
				     unsigned int bytes)
{
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
	nf_ct_acct_add(ct, dir, 1, bytes);
#endif
}

void nf_conntrack_acct_pernet_init(struct net *net);

int nf_conntrack_acct_init(void);
void nf_conntrack_acct_fini(void);

>>>>>>> upstream/android-13
#endif /* _NF_CONNTRACK_ACCT_H */
