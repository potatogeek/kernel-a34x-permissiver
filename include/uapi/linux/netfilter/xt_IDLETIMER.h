<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
=======
>>>>>>> upstream/android-13
/*
 * linux/include/linux/netfilter/xt_IDLETIMER.h
 *
 * Header file for Xtables timer target module.
 *
 * Copyright (C) 2004, 2010 Nokia Corporation
<<<<<<< HEAD
 *
=======
>>>>>>> upstream/android-13
 * Written by Timo Teras <ext-timo.teras@nokia.com>
 *
 * Converted to x_tables and forward-ported to 2.6.34
 * by Luciano Coelho <luciano.coelho@nokia.com>
 *
 * Contact: Luciano Coelho <luciano.coelho@nokia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#ifndef _XT_IDLETIMER_H
#define _XT_IDLETIMER_H

#include <linux/types.h>

#define MAX_IDLETIMER_LABEL_SIZE 28
<<<<<<< HEAD
#define NLMSG_MAX_SIZE 64

#define NL_EVENT_TYPE_INACTIVE 0
#define NL_EVENT_TYPE_ACTIVE 1
=======
#define XT_IDLETIMER_ALARM 0x01
>>>>>>> upstream/android-13

struct idletimer_tg_info {
	__u32 timeout;

	char label[MAX_IDLETIMER_LABEL_SIZE];

<<<<<<< HEAD
	/* Use netlink messages for notification in addition to sysfs */
	__u8 send_nl_msg;

=======
>>>>>>> upstream/android-13
	/* for kernel module internal use only */
	struct idletimer_tg *timer __attribute__((aligned(8)));
};

<<<<<<< HEAD
=======
struct idletimer_tg_info_v1 {
	__u32 timeout;

	char label[MAX_IDLETIMER_LABEL_SIZE];

	__u8 send_nl_msg;
	__u8 timer_type;

	/* for kernel module internal use only */
	struct idletimer_tg *timer __attribute__((aligned(8)));
};
>>>>>>> upstream/android-13
#endif
