<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * arch/arm/include/asm/unwind.h
 *
 * Copyright (C) 2008 ARM Limited
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
>>>>>>> upstream/android-13
 */

#ifndef __ASM_UNWIND_H
#define __ASM_UNWIND_H

#ifndef __ASSEMBLY__

/* Unwind reason code according the the ARM EABI documents */
enum unwind_reason_code {
	URC_OK = 0,			/* operation completed successfully */
	URC_CONTINUE_UNWIND = 8,
	URC_FAILURE = 9			/* unspecified failure of some kind */
};

struct unwind_idx {
	unsigned long addr_offset;
	unsigned long insn;
};

struct unwind_table {
	struct list_head list;
	const struct unwind_idx *start;
	const struct unwind_idx *origin;
	const struct unwind_idx *stop;
	unsigned long begin_addr;
	unsigned long end_addr;
};

extern struct unwind_table *unwind_table_add(unsigned long start,
					     unsigned long size,
					     unsigned long text_addr,
					     unsigned long text_size);
extern void unwind_table_del(struct unwind_table *tab);
<<<<<<< HEAD
extern void unwind_backtrace(struct pt_regs *regs, struct task_struct *tsk);
=======
extern void unwind_backtrace(struct pt_regs *regs, struct task_struct *tsk,
			     const char *loglvl);
>>>>>>> upstream/android-13

#endif	/* !__ASSEMBLY__ */

#ifdef CONFIG_ARM_UNWIND
#define UNWIND(code...)		code
#else
#define UNWIND(code...)
#endif

#endif	/* __ASM_UNWIND_H */
