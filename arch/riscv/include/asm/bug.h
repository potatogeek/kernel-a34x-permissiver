<<<<<<< HEAD
/*
 * Copyright (C) 2012 Regents of the University of California
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 Regents of the University of California
>>>>>>> upstream/android-13
 */

#ifndef _ASM_RISCV_BUG_H
#define _ASM_RISCV_BUG_H

#include <linux/compiler.h>
#include <linux/const.h>
#include <linux/types.h>

#include <asm/asm.h>

<<<<<<< HEAD
#ifdef CONFIG_GENERIC_BUG
#define __BUG_INSN	_AC(0x00100073, UL) /* ebreak */

#ifndef __ASSEMBLY__
=======
#define __INSN_LENGTH_MASK  _UL(0x3)
#define __INSN_LENGTH_32    _UL(0x3)
#define __COMPRESSED_INSN_MASK	_UL(0xffff)

#define __BUG_INSN_32	_UL(0x00100073) /* ebreak */
#define __BUG_INSN_16	_UL(0x9002) /* c.ebreak */

#define GET_INSN_LENGTH(insn)						\
({									\
	unsigned long __len;						\
	__len = ((insn & __INSN_LENGTH_MASK) == __INSN_LENGTH_32) ?	\
		4UL : 2UL;						\
	__len;								\
})

>>>>>>> upstream/android-13
typedef u32 bug_insn_t;

#ifdef CONFIG_GENERIC_BUG_RELATIVE_POINTERS
#define __BUG_ENTRY_ADDR	RISCV_INT " 1b - 2b"
#define __BUG_ENTRY_FILE	RISCV_INT " %0 - 2b"
#else
#define __BUG_ENTRY_ADDR	RISCV_PTR " 1b"
#define __BUG_ENTRY_FILE	RISCV_PTR " %0"
#endif

#ifdef CONFIG_DEBUG_BUGVERBOSE
#define __BUG_ENTRY			\
	__BUG_ENTRY_ADDR "\n\t"		\
	__BUG_ENTRY_FILE "\n\t"		\
<<<<<<< HEAD
	RISCV_SHORT " %1"
#else
#define __BUG_ENTRY			\
	__BUG_ENTRY_ADDR
#endif

#define BUG()							\
=======
	RISCV_SHORT " %1\n\t"		\
	RISCV_SHORT " %2"
#else
#define __BUG_ENTRY			\
	__BUG_ENTRY_ADDR "\n\t"		\
	RISCV_SHORT " %2"
#endif

#ifdef CONFIG_GENERIC_BUG
#define __BUG_FLAGS(flags)					\
>>>>>>> upstream/android-13
do {								\
	__asm__ __volatile__ (					\
		"1:\n\t"					\
			"ebreak\n"				\
<<<<<<< HEAD
			".pushsection __bug_table,\"a\"\n\t"	\
		"2:\n\t"					\
			__BUG_ENTRY "\n\t"			\
			".org 2b + %2\n\t"			\
			".popsection"				\
		:						\
		: "i" (__FILE__), "i" (__LINE__),		\
		  "i" (sizeof(struct bug_entry)));		\
	unreachable();						\
} while (0)
#endif /* !__ASSEMBLY__ */
#else /* CONFIG_GENERIC_BUG */
#ifndef __ASSEMBLY__
#define BUG()							\
do {								\
	__asm__ __volatile__ ("ebreak\n");			\
	unreachable();						\
} while (0)
#endif /* !__ASSEMBLY__ */
#endif /* CONFIG_GENERIC_BUG */
=======
			".pushsection __bug_table,\"aw\"\n\t"	\
		"2:\n\t"					\
			__BUG_ENTRY "\n\t"			\
			".org 2b + %3\n\t"                      \
			".popsection"				\
		:						\
		: "i" (__FILE__), "i" (__LINE__),		\
		  "i" (flags),					\
		  "i" (sizeof(struct bug_entry)));              \
} while (0)
#else /* CONFIG_GENERIC_BUG */
#define __BUG_FLAGS(flags) do {					\
	__asm__ __volatile__ ("ebreak\n");			\
} while (0)
#endif /* CONFIG_GENERIC_BUG */

#define BUG() do {						\
	__BUG_FLAGS(0);						\
	unreachable();						\
} while (0)

#define __WARN_FLAGS(flags) __BUG_FLAGS(BUGFLAG_WARNING|(flags))
>>>>>>> upstream/android-13

#define HAVE_ARCH_BUG

#include <asm-generic/bug.h>

<<<<<<< HEAD
#ifndef __ASSEMBLY__

struct pt_regs;
struct task_struct;

extern void die(struct pt_regs *regs, const char *str);
extern void do_trap(struct pt_regs *regs, int signo, int code,
	unsigned long addr, struct task_struct *tsk);

#endif /* !__ASSEMBLY__ */
=======
struct pt_regs;
struct task_struct;

void __show_regs(struct pt_regs *regs);
void die(struct pt_regs *regs, const char *str);
void do_trap(struct pt_regs *regs, int signo, int code, unsigned long addr);
>>>>>>> upstream/android-13

#endif /* _ASM_RISCV_BUG_H */
