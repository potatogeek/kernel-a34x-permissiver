<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * livepatch.h - powerpc-specific Kernel Live Patching Core
 *
 * Copyright (C) 2015-2016, SUSE, IBM Corp.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */
#ifndef _ASM_POWERPC_LIVEPATCH_H
#define _ASM_POWERPC_LIVEPATCH_H

#include <linux/module.h>
#include <linux/ftrace.h>
<<<<<<< HEAD

#ifdef CONFIG_LIVEPATCH
static inline int klp_check_compiler_support(void)
{
	return 0;
}

static inline void klp_arch_set_pc(struct pt_regs *regs, unsigned long ip)
{
	regs->nip = ip;
=======
#include <linux/sched/task_stack.h>

#ifdef CONFIG_LIVEPATCH
static inline void klp_arch_set_pc(struct ftrace_regs *fregs, unsigned long ip)
{
	struct pt_regs *regs = ftrace_get_regs(fregs);

	regs_set_return_ip(regs, ip);
>>>>>>> upstream/android-13
}

#define klp_get_ftrace_location klp_get_ftrace_location
static inline unsigned long klp_get_ftrace_location(unsigned long faddr)
{
	/*
	 * Live patch works only with -mprofile-kernel on PPC. In this case,
	 * the ftrace location is always within the first 16 bytes.
	 */
	return ftrace_location_range(faddr, faddr + 16);
}

<<<<<<< HEAD
static inline void klp_init_thread_info(struct thread_info *ti)
{
	/* + 1 to account for STACK_END_MAGIC */
	ti->livepatch_sp = (unsigned long *)(ti + 1) + 1;
}
#else
static void klp_init_thread_info(struct thread_info *ti) { }
=======
static inline void klp_init_thread_info(struct task_struct *p)
{
	/* + 1 to account for STACK_END_MAGIC */
	task_thread_info(p)->livepatch_sp = end_of_stack(p) + 1;
}
#else
static inline void klp_init_thread_info(struct task_struct *p) { }
>>>>>>> upstream/android-13
#endif /* CONFIG_LIVEPATCH */

#endif /* _ASM_POWERPC_LIVEPATCH_H */
