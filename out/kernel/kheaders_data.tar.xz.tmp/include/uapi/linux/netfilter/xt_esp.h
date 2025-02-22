/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _XT_ESP_H
#define _XT_ESP_H

#include <linux/types.h>

struct xt_esp {
	__u32 spis[2];	
	__u8  invflags;	
};


#define XT_ESP_INV_SPI	0x01	
#define XT_ESP_INV_MASK	0x01	

#endif 
