/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _CONNTRACK_PROTO_GRE_H
#define _CONNTRACK_PROTO_GRE_H
#include <asm/byteorder.h>
#include <net/gre.h>
#include <net/pptp.h>

struct nf_ct_gre {
	unsigned int stream_timeout;
	unsigned int timeout;
};

#ifdef __KERNEL__
#include <net/netfilter/nf_conntrack_tuple.h>

struct nf_conn;


struct nf_ct_gre_keymap {
	struct list_head list;
	struct nf_conntrack_tuple tuple;
};

enum grep_conntrack {
	GRE_CT_UNREPLIED,
	GRE_CT_REPLIED,
	GRE_CT_MAX
};

struct netns_proto_gre {
	struct nf_proto_net	nf;
	rwlock_t		keymap_lock;
	struct list_head	keymap_list;
	unsigned int		gre_timeouts[GRE_CT_MAX];
};


int nf_ct_gre_keymap_add(struct nf_conn *ct, enum ip_conntrack_dir dir,
			 struct nf_conntrack_tuple *t);


void nf_ct_gre_keymap_destroy(struct nf_conn *ct);

void nf_nat_need_gre(void);

#endif 
#endif 
