<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/*
 * Backtrace support for Microblaze
 *
 * Copyright (C) 2010  Digital Design Corporation
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef __MICROBLAZE_UNWIND_H
#define __MICROBLAZE_UNWIND_H

struct stack_trace;

struct trap_handler_info {
	unsigned long start_addr;
	unsigned long end_addr;
	const char    *trap_name;
};
extern struct trap_handler_info microblaze_trap_handlers;

extern const char _hw_exception_handler;
extern const char ex_handler_unhandled;

<<<<<<< HEAD
void microblaze_unwind(struct task_struct *task, struct stack_trace *trace);
=======
void microblaze_unwind(struct task_struct *task, struct stack_trace *trace,
		       const char *loglvl);
>>>>>>> upstream/android-13

#endif	/* __MICROBLAZE_UNWIND_H */

