<<<<<<< HEAD
/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
>>>>>>> upstream/android-13
 * Amit Bhor, Kanika Nema: Codito Technologies 2004
 */

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/sched/task_stack.h>

#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/unistd.h>
#include <linux/ptrace.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/elf.h>
#include <linux/tick.h>

<<<<<<< HEAD
=======
#include <asm/fpu.h>

>>>>>>> upstream/android-13
SYSCALL_DEFINE1(arc_settls, void *, user_tls_data_ptr)
{
	task_thread_info(current)->thr_ptr = (unsigned int)user_tls_data_ptr;
	return 0;
}

/*
 * We return the user space TLS data ptr as sys-call return code
 * Ideally it should be copy to user.
 * However we can cheat by the fact that some sys-calls do return
 * absurdly high values
 * Since the tls dat aptr is not going to be in range of 0xFFFF_xxxx
 * it won't be considered a sys-call error
 * and it will be loads better than copy-to-user, which is a definite
 * D-TLB Miss
 */
SYSCALL_DEFINE0(arc_gettls)
{
	return task_thread_info(current)->thr_ptr;
}

<<<<<<< HEAD
SYSCALL_DEFINE3(arc_usr_cmpxchg, int *, uaddr, int, expected, int, new)
=======
SYSCALL_DEFINE3(arc_usr_cmpxchg, int __user *, uaddr, int, expected, int, new)
>>>>>>> upstream/android-13
{
	struct pt_regs *regs = current_pt_regs();
	u32 uval;
	int ret;

	/*
<<<<<<< HEAD
	 * This is only for old cores lacking LLOCK/SCOND, which by defintion
=======
	 * This is only for old cores lacking LLOCK/SCOND, which by definition
>>>>>>> upstream/android-13
	 * can't possibly be SMP. Thus doesn't need to be SMP safe.
	 * And this also helps reduce the overhead for serializing in
	 * the UP case
	 */
	WARN_ON_ONCE(IS_ENABLED(CONFIG_SMP));

<<<<<<< HEAD
	/* Z indicates to userspace if operation succeded */
	regs->status32 &= ~STATUS_Z_MASK;

	ret = access_ok(VERIFY_WRITE, uaddr, sizeof(*uaddr));
=======
	/* Z indicates to userspace if operation succeeded */
	regs->status32 &= ~STATUS_Z_MASK;

	ret = access_ok(uaddr, sizeof(*uaddr));
>>>>>>> upstream/android-13
	if (!ret)
		 goto fail;

again:
	preempt_disable();

	ret = __get_user(uval, uaddr);
	if (ret)
		 goto fault;

	if (uval != expected)
		 goto out;

	ret = __put_user(new, uaddr);
	if (ret)
		 goto fault;

	regs->status32 |= STATUS_Z_MASK;

out:
	preempt_enable();
	return uval;

fault:
	preempt_enable();

	if (unlikely(ret != -EFAULT))
		 goto fail;

<<<<<<< HEAD
	down_read(&current->mm->mmap_sem);
	ret = fixup_user_fault(current, current->mm, (unsigned long) uaddr,
			       FAULT_FLAG_WRITE, NULL);
	up_read(&current->mm->mmap_sem);
=======
	mmap_read_lock(current->mm);
	ret = fixup_user_fault(current->mm, (unsigned long) uaddr,
			       FAULT_FLAG_WRITE, NULL);
	mmap_read_unlock(current->mm);
>>>>>>> upstream/android-13

	if (likely(!ret))
		 goto again;

fail:
<<<<<<< HEAD
	force_sig(SIGSEGV, current);
=======
	force_sig(SIGSEGV);
>>>>>>> upstream/android-13
	return ret;
}

#ifdef CONFIG_ISA_ARCV2

void arch_cpu_idle(void)
{
<<<<<<< HEAD
	/* Re-enable interrupts <= default irq priority before commiting SLEEP */
=======
	/* Re-enable interrupts <= default irq priority before committing SLEEP */
>>>>>>> upstream/android-13
	const unsigned int arg = 0x10 | ARCV2_IRQ_DEF_PRIO;

	__asm__ __volatile__(
		"sleep %0	\n"
		:
		:"I"(arg)); /* can't be "r" has to be embedded const */
}

<<<<<<< HEAD
#elif defined(CONFIG_EZNPS_MTM_EXT)	/* ARC700 variant in NPS */

void arch_cpu_idle(void)
{
	/* only the calling HW thread needs to sleep */
	__asm__ __volatile__(
		".word %0	\n"
		:
		:"i"(CTOP_INST_HWSCHD_WFT_IE12));
}

=======
>>>>>>> upstream/android-13
#else	/* ARC700 */

void arch_cpu_idle(void)
{
<<<<<<< HEAD
	/* sleep, but enable both set E1/E2 (levels of interrutps) before committing */
=======
	/* sleep, but enable both set E1/E2 (levels of interrupts) before committing */
>>>>>>> upstream/android-13
	__asm__ __volatile__("sleep 0x3	\n");
}

#endif

asmlinkage void ret_from_fork(void);

/*
 * Copy architecture-specific thread state
 *
 * Layout of Child kernel mode stack as setup at the end of this function is
 *
 * |     ...        |
 * |     ...        |
 * |    unused      |
 * |                |
 * ------------------
 * |     r25        |   <==== top of Stack (thread.ksp)
 * ~                ~
 * |    --to--      |   (CALLEE Regs of kernel mode)
 * |     r13        |
 * ------------------
 * |     fp         |
 * |    blink       |   @ret_from_fork
 * ------------------
 * |                |
 * ~                ~
 * ~                ~
 * |                |
 * ------------------
 * |     r12        |
 * ~                ~
 * |    --to--      |   (scratch Regs of user mode)
 * |     r0         |
 * ------------------
 * |      SP        |
 * |    orig_r0     |
 * |    event/ECR   |
 * |    user_r25    |
 * ------------------  <===== END of PAGE
 */
<<<<<<< HEAD
int copy_thread(unsigned long clone_flags,
		unsigned long usp, unsigned long kthread_arg,
		struct task_struct *p)
=======
int copy_thread(unsigned long clone_flags, unsigned long usp,
		unsigned long kthread_arg, struct task_struct *p,
		unsigned long tls)
>>>>>>> upstream/android-13
{
	struct pt_regs *c_regs;        /* child's pt_regs */
	unsigned long *childksp;       /* to unwind out of __switch_to() */
	struct callee_regs *c_callee;  /* child's callee regs */
	struct callee_regs *parent_callee;  /* paren't callee */
	struct pt_regs *regs = current_pt_regs();

	/* Mark the specific anchors to begin with (see pic above) */
	c_regs = task_pt_regs(p);
	childksp = (unsigned long *)c_regs - 2;  /* 2 words for FP/BLINK */
	c_callee = ((struct callee_regs *)childksp) - 1;

	/*
	 * __switch_to() uses thread.ksp to start unwinding stack
	 * For kernel threads we don't need to create callee regs, the
	 * stack layout nevertheless needs to remain the same.
	 * Also, since __switch_to anyways unwinds callee regs, we use
	 * this to populate kernel thread entry-pt/args into callee regs,
	 * so that ret_from_kernel_thread() becomes simpler.
	 */
	p->thread.ksp = (unsigned long)c_callee;	/* THREAD_KSP */

	/* __switch_to expects FP(0), BLINK(return addr) at top */
	childksp[0] = 0;			/* fp */
	childksp[1] = (unsigned long)ret_from_fork; /* blink */

<<<<<<< HEAD
	if (unlikely(p->flags & PF_KTHREAD)) {
=======
	if (unlikely(p->flags & (PF_KTHREAD | PF_IO_WORKER))) {
>>>>>>> upstream/android-13
		memset(c_regs, 0, sizeof(struct pt_regs));

		c_callee->r13 = kthread_arg;
		c_callee->r14 = usp;  /* function */

		return 0;
	}

	/*--------- User Task Only --------------*/

	/* __switch_to expects FP(0), BLINK(return addr) at top of stack */
	childksp[0] = 0;				/* for POP fp */
	childksp[1] = (unsigned long)ret_from_fork;	/* for POP blink */

	/* Copy parents pt regs on child's kernel mode stack */
	*c_regs = *regs;

	if (usp)
		c_regs->sp = usp;

	c_regs->r0 = 0;		/* fork returns 0 in child */

	parent_callee = ((struct callee_regs *)regs) - 1;
	*c_callee = *parent_callee;

	if (unlikely(clone_flags & CLONE_SETTLS)) {
		/*
		 * set task's userland tls data ptr from 4th arg
		 * clone C-lib call is difft from clone sys-call
		 */
<<<<<<< HEAD
		task_thread_info(p)->thr_ptr = regs->r3;
=======
		task_thread_info(p)->thr_ptr = tls;
>>>>>>> upstream/android-13
	} else {
		/* Normal fork case: set parent's TLS ptr in child */
		task_thread_info(p)->thr_ptr =
		task_thread_info(current)->thr_ptr;
	}


	/*
	 * setup usermode thread pointer #1:
	 * when child is picked by scheduler, __switch_to() uses @c_callee to
	 * populate usermode callee regs: this works (despite being in a kernel
	 * function) since special return path for child @ret_from_fork()
	 * ensures those regs are not clobbered all the way to RTIE to usermode
	 */
	c_callee->r25 = task_thread_info(p)->thr_ptr;

#ifdef CONFIG_ARC_CURR_IN_REG
	/*
	 * setup usermode thread pointer #2:
	 * however for this special use of r25 in kernel, __switch_to() sets
	 * r25 for kernel needs and only in the final return path is usermode
	 * r25 setup, from pt_regs->user_r25. So set that up as well
	 */
	c_regs->user_r25 = c_callee->r25;
#endif

	return 0;
}

/*
 * Do necessary setup to start up a new user task
 */
<<<<<<< HEAD
void start_thread(struct pt_regs * regs, unsigned long pc, unsigned long usp)
=======
void start_thread(struct pt_regs *regs, unsigned long pc, unsigned long usp)
>>>>>>> upstream/android-13
{
	regs->sp = usp;
	regs->ret = pc;

	/*
	 * [U]ser Mode bit set
	 * [L] ZOL loop inhibited to begin with - cleared by a LP insn
	 * Interrupts enabled
	 */
	regs->status32 = STATUS_U_MASK | STATUS_L_MASK | ISA_INIT_STATUS_BITS;

<<<<<<< HEAD
#ifdef CONFIG_EZNPS_MTM_EXT
	regs->eflags = 0;
#endif
=======
	fpu_init_task(regs);
>>>>>>> upstream/android-13

	/* bogus seed values for debugging */
	regs->lp_start = 0x10;
	regs->lp_end = 0x80;
}

/*
 * Some archs flush debug and FPU info here
 */
void flush_thread(void)
{
}

<<<<<<< HEAD
int dump_fpu(struct pt_regs *regs, elf_fpregset_t *fpu)
{
	return 0;
}

=======
>>>>>>> upstream/android-13
int elf_check_arch(const struct elf32_hdr *x)
{
	unsigned int eflags;

	if (x->e_machine != EM_ARC_INUSE) {
		pr_err("ELF not built for %s ISA\n",
			is_isa_arcompact() ? "ARCompact":"ARCv2");
		return 0;
	}

	eflags = x->e_flags;
	if ((eflags & EF_ARC_OSABI_MSK) != EF_ARC_OSABI_CURRENT) {
		pr_err("ABI mismatch - you need newer toolchain\n");
<<<<<<< HEAD
		force_sigsegv(SIGSEGV, current);
=======
		force_fatal_sig(SIGSEGV);
>>>>>>> upstream/android-13
		return 0;
	}

	return 1;
}
EXPORT_SYMBOL(elf_check_arch);
