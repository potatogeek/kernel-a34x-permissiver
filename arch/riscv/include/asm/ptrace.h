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

#ifndef _ASM_RISCV_PTRACE_H
#define _ASM_RISCV_PTRACE_H

#include <uapi/asm/ptrace.h>
#include <asm/csr.h>
<<<<<<< HEAD
=======
#include <linux/compiler.h>
>>>>>>> upstream/android-13

#ifndef __ASSEMBLY__

struct pt_regs {
<<<<<<< HEAD
	unsigned long sepc;
=======
	unsigned long epc;
>>>>>>> upstream/android-13
	unsigned long ra;
	unsigned long sp;
	unsigned long gp;
	unsigned long tp;
	unsigned long t0;
	unsigned long t1;
	unsigned long t2;
	unsigned long s0;
	unsigned long s1;
	unsigned long a0;
	unsigned long a1;
	unsigned long a2;
	unsigned long a3;
	unsigned long a4;
	unsigned long a5;
	unsigned long a6;
	unsigned long a7;
	unsigned long s2;
	unsigned long s3;
	unsigned long s4;
	unsigned long s5;
	unsigned long s6;
	unsigned long s7;
	unsigned long s8;
	unsigned long s9;
	unsigned long s10;
	unsigned long s11;
	unsigned long t3;
	unsigned long t4;
	unsigned long t5;
	unsigned long t6;
<<<<<<< HEAD
	/* Supervisor CSRs */
	unsigned long sstatus;
	unsigned long sbadaddr;
	unsigned long scause;
        /* a0 value before the syscall */
        unsigned long orig_a0;
=======
	/* Supervisor/Machine CSRs */
	unsigned long status;
	unsigned long badaddr;
	unsigned long cause;
	/* a0 value before the syscall */
	unsigned long orig_a0;
>>>>>>> upstream/android-13
};

#ifdef CONFIG_64BIT
#define REG_FMT "%016lx"
#else
#define REG_FMT "%08lx"
#endif

<<<<<<< HEAD
#define user_mode(regs) (((regs)->sstatus & SR_SPP) == 0)


/* Helpers for working with the instruction pointer */
#define GET_IP(regs) ((regs)->sepc)
#define SET_IP(regs, val) (GET_IP(regs) = (val))

static inline unsigned long instruction_pointer(struct pt_regs *regs)
{
	return GET_IP(regs);
=======
#define user_mode(regs) (((regs)->status & SR_PP) == 0)

#define MAX_REG_OFFSET offsetof(struct pt_regs, orig_a0)

/* Helpers for working with the instruction pointer */
static inline unsigned long instruction_pointer(struct pt_regs *regs)
{
	return regs->epc;
>>>>>>> upstream/android-13
}
static inline void instruction_pointer_set(struct pt_regs *regs,
					   unsigned long val)
{
<<<<<<< HEAD
	SET_IP(regs, val);
=======
	regs->epc = val;
>>>>>>> upstream/android-13
}

#define profile_pc(regs) instruction_pointer(regs)

/* Helpers for working with the user stack pointer */
<<<<<<< HEAD
#define GET_USP(regs) ((regs)->sp)
#define SET_USP(regs, val) (GET_USP(regs) = (val))

static inline unsigned long user_stack_pointer(struct pt_regs *regs)
{
	return GET_USP(regs);
=======
static inline unsigned long user_stack_pointer(struct pt_regs *regs)
{
	return regs->sp;
>>>>>>> upstream/android-13
}
static inline void user_stack_pointer_set(struct pt_regs *regs,
					  unsigned long val)
{
<<<<<<< HEAD
	SET_USP(regs, val);
}

/* Helpers for working with the frame pointer */
#define GET_FP(regs) ((regs)->s0)
#define SET_FP(regs, val) (GET_FP(regs) = (val))

static inline unsigned long frame_pointer(struct pt_regs *regs)
{
	return GET_FP(regs);
=======
	regs->sp =  val;
}

/* Valid only for Kernel mode traps. */
static inline unsigned long kernel_stack_pointer(struct pt_regs *regs)
{
	return regs->sp;
}

/* Helpers for working with the frame pointer */
static inline unsigned long frame_pointer(struct pt_regs *regs)
{
	return regs->s0;
>>>>>>> upstream/android-13
}
static inline void frame_pointer_set(struct pt_regs *regs,
				     unsigned long val)
{
<<<<<<< HEAD
	SET_FP(regs, val);
=======
	regs->s0 = val;
}

static inline unsigned long regs_return_value(struct pt_regs *regs)
{
	return regs->a0;
}

static inline void regs_set_return_value(struct pt_regs *regs,
					 unsigned long val)
{
	regs->a0 = val;
}

extern int regs_query_register_offset(const char *name);
extern unsigned long regs_get_kernel_stack_nth(struct pt_regs *regs,
					       unsigned int n);

void prepare_ftrace_return(unsigned long *parent, unsigned long self_addr,
			   unsigned long frame_pointer);
int do_syscall_trace_enter(struct pt_regs *regs);
void do_syscall_trace_exit(struct pt_regs *regs);

/**
 * regs_get_register() - get register value from its offset
 * @regs:	pt_regs from which register value is gotten
 * @offset:	offset of the register.
 *
 * regs_get_register returns the value of a register whose offset from @regs.
 * The @offset is the offset of the register in struct pt_regs.
 * If @offset is bigger than MAX_REG_OFFSET, this returns 0.
 */
static inline unsigned long regs_get_register(struct pt_regs *regs,
					      unsigned int offset)
{
	if (unlikely(offset > MAX_REG_OFFSET))
		return 0;

	return *(unsigned long *)((unsigned long)regs + offset);
}

/**
 * regs_get_kernel_argument() - get Nth function argument in kernel
 * @regs:       pt_regs of that context
 * @n:          function argument number (start from 0)
 *
 * regs_get_argument() returns @n th argument of the function call.
 *
 * Note you can get the parameter correctly if the function has no
 * more than eight arguments.
 */
static inline unsigned long regs_get_kernel_argument(struct pt_regs *regs,
						unsigned int n)
{
	static const int nr_reg_arguments = 8;
	static const unsigned int argument_offs[] = {
		offsetof(struct pt_regs, a0),
		offsetof(struct pt_regs, a1),
		offsetof(struct pt_regs, a2),
		offsetof(struct pt_regs, a3),
		offsetof(struct pt_regs, a4),
		offsetof(struct pt_regs, a5),
		offsetof(struct pt_regs, a6),
		offsetof(struct pt_regs, a7),
	};

	if (n < nr_reg_arguments)
		return regs_get_register(regs, argument_offs[n]);
	return 0;
>>>>>>> upstream/android-13
}

#endif /* __ASSEMBLY__ */

#endif /* _ASM_RISCV_PTRACE_H */
