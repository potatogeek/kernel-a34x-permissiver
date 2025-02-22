/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _IP6T_HL_H
#define _IP6T_HL_H

#include <linux/types.h>

enum {
	IP6T_HL_EQ = 0,		
	IP6T_HL_NE,		
	IP6T_HL_LT,		
	IP6T_HL_GT,		
};


struct ip6t_hl_info {
	__u8	mode;
	__u8	hop_limit;
};


#endif
