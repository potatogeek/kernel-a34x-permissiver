/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _IP6T_FRAG_H
#define _IP6T_FRAG_H

#include <linux/types.h>

struct ip6t_frag {
	__u32 ids[2];			
	__u32 hdrlen;			
	__u8  flags;			
	__u8  invflags;			
};

#define IP6T_FRAG_IDS 		0x01
#define IP6T_FRAG_LEN 		0x02
#define IP6T_FRAG_RES 		0x04
#define IP6T_FRAG_FST 		0x08
#define IP6T_FRAG_MF  		0x10
#define IP6T_FRAG_NMF  		0x20


#define IP6T_FRAG_INV_IDS	0x01	
#define IP6T_FRAG_INV_LEN	0x02	
#define IP6T_FRAG_INV_MASK	0x03	

#endif 
