/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _XT_CONNLIMIT_H
#define _XT_CONNLIMIT_H

#include <linux/types.h>
#include <linux/netfilter.h>

struct xt_connlimit_data;

enum {
	XT_CONNLIMIT_INVERT = 1 << 0,
	XT_CONNLIMIT_DADDR  = 1 << 1,
};

struct xt_connlimit_info {
	union {
		union nf_inet_addr mask;
#ifndef __KERNEL__
		union {
			__be32 v4_mask;
			__be32 v6_mask[4];
		};
#endif
	};
	unsigned int limit;
	
	__u32 flags;

	
	struct nf_conncount_data *data __attribute__((aligned(8)));
};

#endif 
