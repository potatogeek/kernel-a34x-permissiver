<<<<<<< HEAD
/*
 * Common time accounting prototypes and such for all ppc machines.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Common time accounting prototypes and such for all ppc machines.
>>>>>>> upstream/android-13
 */

#ifndef __POWERPC_ACCOUNTING_H
#define __POWERPC_ACCOUNTING_H

/* Stuff for accurate time accounting */
struct cpu_accounting_data {
	/* Accumulated cputime values to flush on ticks*/
	unsigned long utime;
	unsigned long stime;
<<<<<<< HEAD
	unsigned long utime_scaled;
	unsigned long stime_scaled;
=======
#ifdef CONFIG_ARCH_HAS_SCALED_CPUTIME
	unsigned long utime_scaled;
	unsigned long stime_scaled;
#endif
>>>>>>> upstream/android-13
	unsigned long gtime;
	unsigned long hardirq_time;
	unsigned long softirq_time;
	unsigned long steal_time;
	unsigned long idle_time;
	/* Internal counters */
	unsigned long starttime;	/* TB value snapshot */
	unsigned long starttime_user;	/* TB value on exit to usermode */
<<<<<<< HEAD
	unsigned long startspurr;	/* SPURR value snapshot */
	unsigned long utime_sspurr;	/* ->user_time when ->startspurr set */
=======
#ifdef CONFIG_ARCH_HAS_SCALED_CPUTIME
	unsigned long startspurr;	/* SPURR value snapshot */
	unsigned long utime_sspurr;	/* ->user_time when ->startspurr set */
#endif
>>>>>>> upstream/android-13
};

#endif
