/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _XT_SYNPROXY_H
#define _XT_SYNPROXY_H

<<<<<<< HEAD
#include <linux/types.h>

#define XT_SYNPROXY_OPT_MSS		0x01
#define XT_SYNPROXY_OPT_WSCALE		0x02
#define XT_SYNPROXY_OPT_SACK_PERM	0x04
#define XT_SYNPROXY_OPT_TIMESTAMP	0x08
#define XT_SYNPROXY_OPT_ECN		0x10

struct xt_synproxy_info {
	__u8	options;
	__u8	wscale;
	__u16	mss;
};
=======
#include <linux/netfilter/nf_synproxy.h>

#define XT_SYNPROXY_OPT_MSS		NF_SYNPROXY_OPT_MSS
#define XT_SYNPROXY_OPT_WSCALE		NF_SYNPROXY_OPT_WSCALE
#define XT_SYNPROXY_OPT_SACK_PERM	NF_SYNPROXY_OPT_SACK_PERM
#define XT_SYNPROXY_OPT_TIMESTAMP	NF_SYNPROXY_OPT_TIMESTAMP
#define XT_SYNPROXY_OPT_ECN		NF_SYNPROXY_OPT_ECN

#define xt_synproxy_info		nf_synproxy_info
>>>>>>> upstream/android-13

#endif /* _XT_SYNPROXY_H */
