/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _BAYCOM_H
#define _BAYCOM_H




struct baycom_debug_data {
	unsigned long debug1;
	unsigned long debug2;
	long debug3;
};

struct baycom_ioctl {
	int cmd;
	union {
		struct baycom_debug_data dbg;
	} data;
};




#define BAYCOMCTL_GETDEBUG       0x92



#endif 


