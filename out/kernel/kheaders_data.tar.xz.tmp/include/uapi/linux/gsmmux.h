/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _LINUX_GSMMUX_H
#define _LINUX_GSMMUX_H

#include <linux/if.h>
#include <linux/ioctl.h>
#include <linux/types.h>

struct gsm_config
{
	unsigned int adaption;
	unsigned int encapsulation;
	unsigned int initiator;
	unsigned int t1;
	unsigned int t2;
	unsigned int t3;
	unsigned int n2;
	unsigned int mru;
	unsigned int mtu;
	unsigned int k;
	unsigned int i;
	unsigned int unused[8];		
};

#define GSMIOC_GETCONF		_IOR('G', 0, struct gsm_config)
#define GSMIOC_SETCONF		_IOW('G', 1, struct gsm_config)

struct gsm_netconfig {
	unsigned int adaption;  
	unsigned short protocol;
	unsigned short unused2;
	char if_name[IFNAMSIZ];	
	__u8 unused[28];        
};

#define GSMIOC_ENABLE_NET      _IOW('G', 2, struct gsm_netconfig)
#define GSMIOC_DISABLE_NET     _IO('G', 3)


#endif
