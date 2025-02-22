/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI__IP_SET_HASH_H
#define _UAPI__IP_SET_HASH_H

#include <linux/netfilter/ipset/ip_set.h>


enum {
	
	IPSET_ERR_HASH_FULL = IPSET_ERR_TYPE_SPECIFIC,
	
	IPSET_ERR_HASH_ELEM,
	
	IPSET_ERR_INVALID_PROTO,
	
	IPSET_ERR_MISSING_PROTO,
	
	IPSET_ERR_HASH_RANGE_UNSUPPORTED,
	
	IPSET_ERR_HASH_RANGE,
};


#endif 
