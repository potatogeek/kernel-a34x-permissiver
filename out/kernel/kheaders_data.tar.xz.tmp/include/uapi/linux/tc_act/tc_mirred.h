/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef __LINUX_TC_MIR_H
#define __LINUX_TC_MIR_H

#include <linux/types.h>
#include <linux/pkt_cls.h>

#define TCA_ACT_MIRRED 8
#define TCA_EGRESS_REDIR 1  
#define TCA_EGRESS_MIRROR 2 
#define TCA_INGRESS_REDIR 3  
#define TCA_INGRESS_MIRROR 4 

struct tc_mirred {
	tc_gen;
	int                     eaction;   
	__u32                   ifindex;  
};

enum {
	TCA_MIRRED_UNSPEC,
	TCA_MIRRED_TM,
	TCA_MIRRED_PARMS,
	TCA_MIRRED_PAD,
	__TCA_MIRRED_MAX
};
#define TCA_MIRRED_MAX (__TCA_MIRRED_MAX - 1)

#endif
