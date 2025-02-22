/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_CONNTRACK_COMMON_H
#define _NF_CONNTRACK_COMMON_H

<<<<<<< HEAD
=======
#include <linux/refcount.h>
>>>>>>> upstream/android-13
#include <uapi/linux/netfilter/nf_conntrack_common.h>

struct ip_conntrack_stat {
	unsigned int found;
	unsigned int invalid;
<<<<<<< HEAD
	unsigned int ignore;
	unsigned int insert;
	unsigned int insert_failed;
=======
	unsigned int insert;
	unsigned int insert_failed;
	unsigned int clash_resolve;
>>>>>>> upstream/android-13
	unsigned int drop;
	unsigned int early_drop;
	unsigned int error;
	unsigned int expect_new;
	unsigned int expect_create;
	unsigned int expect_delete;
	unsigned int search_restart;
<<<<<<< HEAD
};

/* call to create an explicit dependency on nf_conntrack. */
void need_conntrack(void);
=======
	unsigned int chaintoolong;
};

#define NFCT_INFOMASK	7UL
#define NFCT_PTRMASK	~(NFCT_INFOMASK)

struct nf_conntrack {
	refcount_t use;
};

void nf_conntrack_destroy(struct nf_conntrack *nfct);

/* like nf_ct_put, but without module dependency on nf_conntrack */
static inline void nf_conntrack_put(struct nf_conntrack *nfct)
{
	if (nfct && refcount_dec_and_test(&nfct->use))
		nf_conntrack_destroy(nfct);
}
static inline void nf_conntrack_get(struct nf_conntrack *nfct)
{
	if (nfct)
		refcount_inc(&nfct->use);
}
>>>>>>> upstream/android-13

#endif /* _NF_CONNTRACK_COMMON_H */
