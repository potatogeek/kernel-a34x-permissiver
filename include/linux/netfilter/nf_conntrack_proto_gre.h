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

<<<<<<< HEAD
#ifdef __KERNEL__
=======
>>>>>>> upstream/android-13
#include <net/netfilter/nf_conntrack_tuple.h>

struct nf_conn;

/* structure for original <-> reply keymap */
struct nf_ct_gre_keymap {
	struct list_head list;
	struct nf_conntrack_tuple tuple;
<<<<<<< HEAD
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
=======
	struct rcu_head rcu;
>>>>>>> upstream/android-13
};

/* add new tuple->key_reply pair to keymap */
int nf_ct_gre_keymap_add(struct nf_conn *ct, enum ip_conntrack_dir dir,
			 struct nf_conntrack_tuple *t);

<<<<<<< HEAD
/* delete keymap entries */
void nf_ct_gre_keymap_destroy(struct nf_conn *ct);

void nf_nat_need_gre(void);

#endif /* __KERNEL__ */
=======
void nf_ct_gre_keymap_flush(struct net *net);
/* delete keymap entries */
void nf_ct_gre_keymap_destroy(struct nf_conn *ct);

bool gre_pkt_to_tuple(const struct sk_buff *skb, unsigned int dataoff,
		      struct net *net, struct nf_conntrack_tuple *tuple);
>>>>>>> upstream/android-13
#endif /* _CONNTRACK_PROTO_GRE_H */
