/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __LINUX_RESTART_BLOCK_H
#define __LINUX_RESTART_BLOCK_H

#include <linux/compiler.h>
#include <linux/types.h>
#include <linux/time64.h>

struct timespec;
struct compat_timespec;
struct pollfd;

enum timespec_type {
	TT_NONE		= 0,
	TT_NATIVE	= 1,
	TT_COMPAT	= 2,
};


struct restart_block {
	long (*fn)(struct restart_block *);
	union {
		
		struct {
			u32 __user *uaddr;
			u32 val;
			u32 flags;
			u32 bitset;
			u64 time;
			u32 __user *uaddr2;
		} futex;
		
		struct {
			clockid_t clockid;
			enum timespec_type type;
			union {
				struct __kernel_timespec __user *rmtp;
				struct compat_timespec __user *compat_rmtp;
			};
			u64 expires;
		} nanosleep;
		
		struct {
			struct pollfd __user *ufds;
			int nfds;
			int has_timeout;
			unsigned long tv_sec;
			unsigned long tv_nsec;
		} poll;
	};
};

extern long do_no_restart_syscall(struct restart_block *parm);

#endif 
