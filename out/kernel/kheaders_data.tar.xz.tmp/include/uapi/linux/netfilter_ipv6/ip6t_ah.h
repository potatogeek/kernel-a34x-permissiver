/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _IP6T_AH_H
#define _IP6T_AH_H

#include <linux/types.h>

struct ip6t_ah {
	__u32 spis[2];			
	__u32 hdrlen;			
	__u8  hdrres;			
	__u8  invflags;			
};

#define IP6T_AH_SPI 0x01
#define IP6T_AH_LEN 0x02
#define IP6T_AH_RES 0x04


#define IP6T_AH_INV_SPI		0x01	
#define IP6T_AH_INV_LEN		0x02	
#define IP6T_AH_INV_MASK	0x03	

#endif 
