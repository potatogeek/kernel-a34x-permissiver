/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */

 
#ifndef _LINUX_IF_PLIP_H
#define _LINUX_IF_PLIP_H

#include <linux/sockios.h>

#define	SIOCDEVPLIP	SIOCDEVPRIVATE

struct plipconf {
	unsigned short pcmd;
	unsigned long  nibble;
	unsigned long  trigger;
};

#define PLIP_GET_TIMEOUT	0x1
#define PLIP_SET_TIMEOUT	0x2

#endif
