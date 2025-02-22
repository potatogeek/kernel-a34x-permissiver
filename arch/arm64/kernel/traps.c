<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Based on arch/arm/kernel/traps.c
 *
 * Copyright (C) 1995-2009 Russell King
 * Copyright (C) 2012 ARM Ltd.
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/bug.h>
#include <linux/signal.h>
#include <linux/personality.h>
#include <linux/kallsyms.h>
=======
 */

#include <linux/bug.h>
#include <linux/context_tracking.h>
#include <linux/signal.h>
#include <linux/personality.h>
#include <linux/kallsyms.h>
#include <linux/kprobes.h>
>>>>>>> upstream/android-13
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/hardirq.h>
#include <linux/kdebug.h>
#include <linux/module.h>
#include <linux/kexec.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched/debug.h>
#include <linux/sched/task_stack.h>
#include <linux/sizes.h>
#include <linux/syscalls.h>
#include <linux/mm_types.h>
#include <linux/kasan.h>

#include <asm/atomic.h>
#include <asm/bug.h>
#include <asm/cpufeature.h>
#include <asm/daifflags.h>
#include <asm/debug-monitors.h>
#include <asm/esr.h>
<<<<<<< HEAD
#include <asm/insn.h>
=======
#include <asm/exception.h>
#include <asm/extable.h>
#include <asm/insn.h>
#include <asm/kprobes.h>
#include <asm/patching.h>
>>>>>>> upstream/android-13
#include <asm/traps.h>
#include <asm/smp.h>
#include <asm/stack_pointer.h>
#include <asm/stacktrace.h>
<<<<<<< HEAD
#include <asm/exception.h>
#include <asm/system_misc.h>
#include <asm/sysreg.h>

#ifdef CONFIG_SEC_DEBUG
#include <linux/sec_debug.h>
#endif

static const char *handler[]= {
	"Synchronous Abort",
	"IRQ",
	"FIQ",
	"Error"
=======
#include <asm/system_misc.h>
#include <asm/sysreg.h>

#include <trace/hooks/traps.h>

static bool __kprobes __check_eq(unsigned long pstate)
{
	return (pstate & PSR_Z_BIT) != 0;
}

static bool __kprobes __check_ne(unsigned long pstate)
{
	return (pstate & PSR_Z_BIT) == 0;
}

static bool __kprobes __check_cs(unsigned long pstate)
{
	return (pstate & PSR_C_BIT) != 0;
}

static bool __kprobes __check_cc(unsigned long pstate)
{
	return (pstate & PSR_C_BIT) == 0;
}

static bool __kprobes __check_mi(unsigned long pstate)
{
	return (pstate & PSR_N_BIT) != 0;
}

static bool __kprobes __check_pl(unsigned long pstate)
{
	return (pstate & PSR_N_BIT) == 0;
}

static bool __kprobes __check_vs(unsigned long pstate)
{
	return (pstate & PSR_V_BIT) != 0;
}

static bool __kprobes __check_vc(unsigned long pstate)
{
	return (pstate & PSR_V_BIT) == 0;
}

static bool __kprobes __check_hi(unsigned long pstate)
{
	pstate &= ~(pstate >> 1);	/* PSR_C_BIT &= ~PSR_Z_BIT */
	return (pstate & PSR_C_BIT) != 0;
}

static bool __kprobes __check_ls(unsigned long pstate)
{
	pstate &= ~(pstate >> 1);	/* PSR_C_BIT &= ~PSR_Z_BIT */
	return (pstate & PSR_C_BIT) == 0;
}

static bool __kprobes __check_ge(unsigned long pstate)
{
	pstate ^= (pstate << 3);	/* PSR_N_BIT ^= PSR_V_BIT */
	return (pstate & PSR_N_BIT) == 0;
}

static bool __kprobes __check_lt(unsigned long pstate)
{
	pstate ^= (pstate << 3);	/* PSR_N_BIT ^= PSR_V_BIT */
	return (pstate & PSR_N_BIT) != 0;
}

static bool __kprobes __check_gt(unsigned long pstate)
{
	/*PSR_N_BIT ^= PSR_V_BIT */
	unsigned long temp = pstate ^ (pstate << 3);

	temp |= (pstate << 1);	/*PSR_N_BIT |= PSR_Z_BIT */
	return (temp & PSR_N_BIT) == 0;
}

static bool __kprobes __check_le(unsigned long pstate)
{
	/*PSR_N_BIT ^= PSR_V_BIT */
	unsigned long temp = pstate ^ (pstate << 3);

	temp |= (pstate << 1);	/*PSR_N_BIT |= PSR_Z_BIT */
	return (temp & PSR_N_BIT) != 0;
}

static bool __kprobes __check_al(unsigned long pstate)
{
	return true;
}

/*
 * Note that the ARMv8 ARM calls condition code 0b1111 "nv", but states that
 * it behaves identically to 0b1110 ("al").
 */
pstate_check_t * const aarch32_opcode_cond_checks[16] = {
	__check_eq, __check_ne, __check_cs, __check_cc,
	__check_mi, __check_pl, __check_vs, __check_vc,
	__check_hi, __check_ls, __check_ge, __check_lt,
	__check_gt, __check_le, __check_al, __check_al
>>>>>>> upstream/android-13
};

int show_unhandled_signals = 0;

<<<<<<< HEAD
static void dump_backtrace_entry(unsigned long where)
{
	/*
	 * Note that 'where' can have a physical address, but it's not handled.
	 */
	print_ip_sym(where);
}

#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
static void dump_backtrace_entry_auto_comment(unsigned long where)
{
	/*
	 * Note that 'where' can have a physical address, but it's not handled.
	 */
	pr_auto(ASL2, "[<%px>] %pS\n", (void *) where, (void *) where);
}
#endif

static void __dump_instr(const char *lvl, struct pt_regs *regs)
=======
static void dump_kernel_instr(const char *lvl, struct pt_regs *regs)
>>>>>>> upstream/android-13
{
	unsigned long addr = instruction_pointer(regs);
	char str[sizeof("00000000 ") * 5 + 2 + 1], *p = str;
	int i;

<<<<<<< HEAD
	for (i = -4; i < 1; i++) {
		unsigned int val, bad;

		bad = get_user(val, &((u32 *)addr)[i]);
=======
	if (user_mode(regs))
		return;

	for (i = -4; i < 1; i++) {
		unsigned int val, bad;

		bad = aarch64_insn_read(&((u32 *)addr)[i], &val);
>>>>>>> upstream/android-13

		if (!bad)
			p += sprintf(p, i == 0 ? "(%08x) " : "%08x ", val);
		else {
			p += sprintf(p, "bad PC value");
			break;
		}
	}
<<<<<<< HEAD
	printk("%sCode: %s\n", lvl, str);
}

static void dump_instr(const char *lvl, struct pt_regs *regs)
{
	if (!user_mode(regs)) {
		mm_segment_t fs = get_fs();
		set_fs(KERNEL_DS);
		__dump_instr(lvl, regs);
		set_fs(fs);
	} else {
		__dump_instr(lvl, regs);
	}
}

#ifdef CONFIG_SEC_DEBUG_LIMIT_BACKTRACE
#define MAX_UNWINDING_LOOP 256 /* maximum number of unwind frame */
#endif

void dump_backtrace(struct pt_regs *regs, struct task_struct *tsk)
{
	struct stackframe frame;
	int skip = 0;
	int cnt = 0;

	pr_debug("%s(regs = %p tsk = %p)\n", __func__, regs, tsk);

	if (regs) {
		if (user_mode(regs))
			return;
		skip = 1;
	}

	if (!tsk)
		tsk = current;

	if (!try_get_task_stack(tsk))
		return;

	if (tsk == current) {
		frame.fp = (unsigned long)__builtin_frame_address(0);
		frame.pc = (unsigned long)dump_backtrace;
	} else {
		/*
		 * task blocked in __switch_to
		 */
		frame.fp = thread_saved_fp(tsk);
		frame.pc = thread_saved_pc(tsk);
	}
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	frame.graph = tsk->curr_ret_stack;
#endif

	printk("Call trace:\n");
	do {
#ifdef CONFIG_SEC_DEBUG_LIMIT_BACKTRACE
		if (MAX_UNWINDING_LOOP < cnt) {
				pr_info("%s: Forcely break dump_backtrace to avoid infinity backtrace\n", __func__);
				break;
		}
#endif

		/* skip until specified stack frame */
		if (!skip) {
			dump_backtrace_entry(frame.pc);
		} else if (frame.fp == regs->regs[29]) {
			skip = 0;
			/*
			 * Mostly, this is the case where this function is
			 * called in panic/abort. As exception handler's
			 * stack frame does not contain the corresponding pc
			 * at which an exception has taken place, use regs->pc
			 * instead.
			 */
			dump_backtrace_entry(regs->pc);
		}
		cnt++;
	} while (!unwind_frame(tsk, &frame));

	put_task_stack(tsk);
}

#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
void dump_backtrace_auto_comment(struct pt_regs *regs, struct task_struct *tsk)
{
	struct stackframe frame;
	int skip = 0;
	int cnt = 0;

	pr_debug("%s(regs = %p tsk = %p)\n", __func__, regs, tsk);

	if (regs) {
		if (user_mode(regs))
			return;
		skip = 1;
	}

	if (!tsk)
		tsk = current;

	if (!try_get_task_stack(tsk))
		return;

	if (tsk == current) {
		frame.fp = (unsigned long)__builtin_frame_address(0);
		frame.pc = (unsigned long)dump_backtrace_auto_comment;
	} else {
		/*
		 * task blocked in __switch_to
		 */
		frame.fp = thread_saved_fp(tsk);
		frame.pc = thread_saved_pc(tsk);
	}
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	frame.graph = tsk->curr_ret_stack;
#endif

	pr_auto_once(2);
	pr_auto(ASL2, "Call trace:\n");
	do {

#ifdef CONFIG_SEC_DEBUG_LIMIT_BACKTRACE
		if (MAX_UNWINDING_LOOP < cnt) {
				pr_info("%s: Forcely break dump_backtrace to avoid infinity backtrace\n", __func__);
				break;
		}
#endif

		/* skip until specified stack frame */
		if (!skip) {
			dump_backtrace_entry_auto_comment(frame.pc);
		} else if (frame.fp == regs->regs[29]) {
			skip = 0;
			/*
			 * Mostly, this is the case where this function is
			 * called in panic/abort. As exception handler's
			 * stack frame does not contain the corresponding pc
			 * at which an exception has taken place, use regs->pc
			 * instead.
			 */
			dump_backtrace_entry_auto_comment(regs->pc);
		}
		cnt++;
	} while (!unwind_frame(tsk, &frame));

	put_task_stack(tsk);
}
#endif

void show_stack(struct task_struct *tsk, unsigned long *sp)
{
	dump_backtrace(NULL, tsk);
	barrier();
}

#ifdef CONFIG_PREEMPT
#define S_PREEMPT " PREEMPT"
#else
#define S_PREEMPT ""
#endif
=======

	printk("%sCode: %s\n", lvl, str);
}

#if IS_ENABLED(CONFIG_SEC_DEBUG)
static unsigned long __get_par_to_addr(u64 par, unsigned long vaddr)
{
	u64 tmp, rpar;

	tmp = (u64)vaddr & (0xfff);
	rpar = ((par & ~(0xfffUL << 52)) & ~(0xfff));
	rpar = rpar | tmp;

	return rpar;
}

static unsigned long secdbg_get_pa_from_mmu(unsigned long vaddr)
{
	unsigned long flags;
	u64 par, dfsc;

	local_irq_save(flags);
	asm volatile("at s1e1r, %0" :: "r" (vaddr));
	isb();
	par = read_sysreg_par();
	local_irq_restore(flags);

	if (par & SYS_PAR_EL1_F) {
		dfsc = FIELD_GET(SYS_PAR_EL1_FST, par);
		pr_err("failed to get v:0x%lx, p:0x%lx, fsc:0x%lx\n",
			vaddr, par, dfsc);
		return 0;
	}

	return __get_par_to_addr(par, vaddr);
}

static int _dump_kernel_hex_line(unsigned long start_addr, unsigned int lines,
					unsigned long paddr, unsigned long pc_val)
{
	unsigned int val, bad;
	u32 *addr = (u32 *)start_addr;
	char str[sizeof(" 00000000 ") * 4 + 1];
	int i;
	char *p = str;
	char *end = p + sizeof(str);

	for (i = 0; i < 4; i++) {
		bool ispc = (addr + i == (u32 *)pc_val);

		bad = aarch64_insn_read(addr + i, &val);

		if (!bad)
			p += snprintf(p, end - p, ispc ? "(%08x)" : " %08x ", val);
		else {
			p += snprintf(p, end - p, "bad PC value");
			break;
		}
	}

	pr_info(" %9x :%s\n", paddr, str);

	return bad;
}

static void secdbg_dump_kernel_instr_ext(struct pt_regs *regs)
{
	unsigned long pc_val = instruction_pointer(regs);
	unsigned long start_addr = (pc_val & ~(0x40 - 1)) - 0x40;
	unsigned long paddr = 0x0;
	int i;

	if (user_mode(regs))
		return;

	for (i = 0; i < 12; i++) {
		unsigned int bad;
		u32 *addr = (u32 *)start_addr + 4 * i;

		if (i == 0 || offset_in_page(addr) == 0)
			paddr = secdbg_get_pa_from_mmu((unsigned long)addr);
		else
			paddr += 0x10;

		if (i && (i % 4) == 0)
			pr_info("\n");

		bad = _dump_kernel_hex_line((unsigned long)addr, 4, paddr, pc_val);
		if (bad)
			break;
	}
}
#else
static inline void secdbg_dump_kernel_instr_ext(struct pt_regs *regs)
{
}
#endif

#ifdef CONFIG_PREEMPT
#define S_PREEMPT " PREEMPT"
#elif defined(CONFIG_PREEMPT_RT)
#define S_PREEMPT " PREEMPT_RT"
#else
#define S_PREEMPT ""
#endif

>>>>>>> upstream/android-13
#define S_SMP " SMP"

static int __die(const char *str, int err, struct pt_regs *regs)
{
<<<<<<< HEAD
	struct task_struct *tsk = current;
=======
>>>>>>> upstream/android-13
	static int die_counter;
	int ret;

	pr_emerg("Internal error: %s: %x [#%d]" S_PREEMPT S_SMP "\n",
		 str, err, ++die_counter);

	/* trap and error numbers are mostly meaningless on ARM */
	ret = notify_die(DIE_OOPS, str, regs, err, 0, SIGSEGV);
	if (ret == NOTIFY_STOP)
		return ret;

	print_modules();
<<<<<<< HEAD
	pr_emerg("Process %.*s (pid: %d, stack limit = 0x%p)\n",
		 TASK_COMM_LEN, tsk->comm, task_pid_nr(tsk),
		 end_of_stack(tsk));
	show_regs(regs);

	if (!user_mode(regs))
		dump_instr(KERN_EMERG, regs);
=======
#if IS_ENABLED(CONFIG_SEC_DEBUG_AUTO_COMMENT)
	__show_regs(regs);
	dump_backtrace_auto_comment(regs, NULL);
#else
	show_regs(regs);
#endif

	dump_kernel_instr(KERN_EMERG, regs);
>>>>>>> upstream/android-13

	return ret;
}

static DEFINE_RAW_SPINLOCK(die_lock);

/*
 * This function is protected against re-entrancy.
 */
void die(const char *str, struct pt_regs *regs, int err)
{
	int ret;
	unsigned long flags;

	raw_spin_lock_irqsave(&die_lock, flags);

	oops_enter();

<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG_EXTRA_INFO
	if (regs && (!user_mode(regs)))
		sec_debug_set_extra_info_backtrace(regs);
#endif

=======
>>>>>>> upstream/android-13
	console_verbose();
	bust_spinlocks(1);
	ret = __die(str, err, regs);

	if (regs && kexec_should_crash(current))
		crash_kexec(regs);

	bust_spinlocks(0);
	add_taint(TAINT_DIE, LOCKDEP_NOW_UNRELIABLE);
	oops_exit();

<<<<<<< HEAD
#ifdef CONFIG_SEC_DEBUG
	if (in_interrupt()) {
		if (regs)
			panic("%s\nPC is at %pS\nLR is at %pS",
				  "Fatal exception in interrupt", (void *)(regs)->pc,
				  (compat_user_mode(regs)) ? (void *)regs->compat_lr : (void *)regs->regs[30]);
		else
			panic("Fatal exception in interrupt");
	}
	if (panic_on_oops) {
		if (regs)
			panic("%s\nPC is at %pS\nLR is at %pS",
				  "Fatal exception", (void *)(regs)->pc,
				  (compat_user_mode(regs)) ? (void *)regs->compat_lr : (void *)regs->regs[30]);
		else
			panic("Fatal exception");
	}
#else
	if (in_interrupt())
		panic("Fatal exception in interrupt");
	if (panic_on_oops)
		panic("Fatal exception");
#endif
=======
	if (in_interrupt())
		panic("%s: Fatal exception in interrupt", str);
	if (panic_on_oops)
		panic("%s: Fatal exception", str);
>>>>>>> upstream/android-13

	raw_spin_unlock_irqrestore(&die_lock, flags);

	if (ret != NOTIFY_STOP)
		do_exit(SIGSEGV);
}

<<<<<<< HEAD
static bool show_unhandled_signals_ratelimited(void)
{
	static DEFINE_RATELIMIT_STATE(rs, DEFAULT_RATELIMIT_INTERVAL,
				      DEFAULT_RATELIMIT_BURST);
	return show_unhandled_signals && __ratelimit(&rs);
}

void arm64_force_sig_info(struct siginfo *info, const char *str,
			  struct task_struct *tsk)
{
	unsigned int esr = tsk->thread.fault_code;
	struct pt_regs *regs = task_pt_regs(tsk);

	if (!unhandled_signal(tsk, info->si_signo))
		goto send_sig;

	if (!show_unhandled_signals_ratelimited())
		goto send_sig;
=======
static void arm64_show_signal(int signo, const char *str)
{
	static DEFINE_RATELIMIT_STATE(rs, DEFAULT_RATELIMIT_INTERVAL,
				      DEFAULT_RATELIMIT_BURST);
	struct task_struct *tsk = current;
	unsigned int esr = tsk->thread.fault_code;
	struct pt_regs *regs = task_pt_regs(tsk);

	/* Leave if the signal won't be shown */
	if (!show_unhandled_signals ||
	    !unhandled_signal(tsk, signo) ||
	    !__ratelimit(&rs))
		return;
>>>>>>> upstream/android-13

	pr_info("%s[%d]: unhandled exception: ", tsk->comm, task_pid_nr(tsk));
	if (esr)
		pr_cont("%s, ESR 0x%08x, ", esr_get_class_string(esr), esr);

	pr_cont("%s", str);
	print_vma_addr(KERN_CONT " in ", regs->pc);
	pr_cont("\n");
	__show_regs(regs);
<<<<<<< HEAD

send_sig:
	force_sig_info(info->si_signo, info, tsk);
}

void arm64_notify_die(const char *str, struct pt_regs *regs,
		      struct siginfo *info, int err)
=======
}

void arm64_force_sig_fault(int signo, int code, unsigned long far,
			   const char *str)
{
	arm64_show_signal(signo, str);
	if (signo == SIGKILL)
		force_sig(SIGKILL);
	else
		force_sig_fault(signo, code, (void __user *)far);
}

void arm64_force_sig_mceerr(int code, unsigned long far, short lsb,
			    const char *str)
{
	arm64_show_signal(SIGBUS, str);
	force_sig_mceerr(code, (void __user *)far, lsb);
}

void arm64_force_sig_ptrace_errno_trap(int errno, unsigned long far,
				       const char *str)
{
	arm64_show_signal(SIGTRAP, str);
	force_sig_ptrace_errno_trap(errno, (void __user *)far);
}

void arm64_notify_die(const char *str, struct pt_regs *regs,
		      int signo, int sicode, unsigned long far,
		      int err)
>>>>>>> upstream/android-13
{
	if (user_mode(regs)) {
		WARN_ON(regs != current_pt_regs());
		current->thread.fault_address = 0;
		current->thread.fault_code = err;
<<<<<<< HEAD
		arm64_force_sig_info(info, str, current);
=======

		arm64_force_sig_fault(signo, sicode, far, str);
>>>>>>> upstream/android-13
	} else {
		die(str, regs, err);
	}
}

<<<<<<< HEAD
=======
#ifdef CONFIG_COMPAT
#define PSTATE_IT_1_0_SHIFT	25
#define PSTATE_IT_1_0_MASK	(0x3 << PSTATE_IT_1_0_SHIFT)
#define PSTATE_IT_7_2_SHIFT	10
#define PSTATE_IT_7_2_MASK	(0x3f << PSTATE_IT_7_2_SHIFT)

static u32 compat_get_it_state(struct pt_regs *regs)
{
	u32 it, pstate = regs->pstate;

	it  = (pstate & PSTATE_IT_1_0_MASK) >> PSTATE_IT_1_0_SHIFT;
	it |= ((pstate & PSTATE_IT_7_2_MASK) >> PSTATE_IT_7_2_SHIFT) << 2;

	return it;
}

static void compat_set_it_state(struct pt_regs *regs, u32 it)
{
	u32 pstate_it;

	pstate_it  = (it << PSTATE_IT_1_0_SHIFT) & PSTATE_IT_1_0_MASK;
	pstate_it |= ((it >> 2) << PSTATE_IT_7_2_SHIFT) & PSTATE_IT_7_2_MASK;

	regs->pstate &= ~PSR_AA32_IT_MASK;
	regs->pstate |= pstate_it;
}

static void advance_itstate(struct pt_regs *regs)
{
	u32 it;

	/* ARM mode */
	if (!(regs->pstate & PSR_AA32_T_BIT) ||
	    !(regs->pstate & PSR_AA32_IT_MASK))
		return;

	it  = compat_get_it_state(regs);

	/*
	 * If this is the last instruction of the block, wipe the IT
	 * state. Otherwise advance it.
	 */
	if (!(it & 7))
		it = 0;
	else
		it = (it & 0xe0) | ((it << 1) & 0x1f);

	compat_set_it_state(regs, it);
}
#else
static void advance_itstate(struct pt_regs *regs)
{
}
#endif

>>>>>>> upstream/android-13
void arm64_skip_faulting_instruction(struct pt_regs *regs, unsigned long size)
{
	regs->pc += size;

	/*
	 * If we were single stepping, we want to get the step exception after
	 * we return from the trap.
	 */
	if (user_mode(regs))
		user_fastforward_single_step(current);
<<<<<<< HEAD
=======

	if (compat_user_mode(regs))
		advance_itstate(regs);
	else
		regs->pstate &= ~PSR_BTYPE_MASK;
>>>>>>> upstream/android-13
}

static LIST_HEAD(undef_hook);
static DEFINE_RAW_SPINLOCK(undef_lock);

void register_undef_hook(struct undef_hook *hook)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&undef_lock, flags);
	list_add(&hook->node, &undef_hook);
	raw_spin_unlock_irqrestore(&undef_lock, flags);
}

void unregister_undef_hook(struct undef_hook *hook)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&undef_lock, flags);
	list_del(&hook->node);
	raw_spin_unlock_irqrestore(&undef_lock, flags);
}

static int call_undef_hook(struct pt_regs *regs)
{
	struct undef_hook *hook;
	unsigned long flags;
	u32 instr;
	int (*fn)(struct pt_regs *regs, u32 instr) = NULL;
	void __user *pc = (void __user *)instruction_pointer(regs);

	if (!user_mode(regs)) {
		__le32 instr_le;
<<<<<<< HEAD
		if (probe_kernel_address((__force __le32 *)pc, instr_le))
=======
		if (get_kernel_nofault(instr_le, (__force __le32 *)pc))
>>>>>>> upstream/android-13
			goto exit;
		instr = le32_to_cpu(instr_le);
	} else if (compat_thumb_mode(regs)) {
		/* 16-bit Thumb instruction */
		__le16 instr_le;
		if (get_user(instr_le, (__le16 __user *)pc))
			goto exit;
		instr = le16_to_cpu(instr_le);
		if (aarch32_insn_is_wide(instr)) {
			u32 instr2;

			if (get_user(instr_le, (__le16 __user *)(pc + 2)))
				goto exit;
			instr2 = le16_to_cpu(instr_le);
			instr = (instr << 16) | instr2;
		}
	} else {
		/* 32-bit ARM instruction */
		__le32 instr_le;
		if (get_user(instr_le, (__le32 __user *)pc))
			goto exit;
		instr = le32_to_cpu(instr_le);
	}

	raw_spin_lock_irqsave(&undef_lock, flags);
	list_for_each_entry(hook, &undef_hook, node)
		if ((instr & hook->instr_mask) == hook->instr_val &&
			(regs->pstate & hook->pstate_mask) == hook->pstate_val)
			fn = hook->fn;

	raw_spin_unlock_irqrestore(&undef_lock, flags);
exit:
	return fn ? fn(regs, instr) : 1;
}

<<<<<<< HEAD
void force_signal_inject(int signal, int code, unsigned long address)
{
	siginfo_t info;
	const char *desc;
	struct pt_regs *regs = current_pt_regs();

	clear_siginfo(&info);
=======
void force_signal_inject(int signal, int code, unsigned long address, unsigned int err)
{
	const char *desc;
	struct pt_regs *regs = current_pt_regs();

	if (WARN_ON(!user_mode(regs)))
		return;
>>>>>>> upstream/android-13

	switch (signal) {
	case SIGILL:
		desc = "undefined instruction";
		break;
	case SIGSEGV:
		desc = "illegal memory access";
		break;
	default:
		desc = "unknown or unrecoverable error";
		break;
	}

	/* Force signals we don't understand to SIGKILL */
	if (WARN_ON(signal != SIGKILL &&
		    siginfo_layout(signal, code) != SIL_FAULT)) {
		signal = SIGKILL;
	}

<<<<<<< HEAD
	info.si_signo = signal;
	info.si_errno = 0;
	info.si_code  = code;
	info.si_addr  = (void __user *)address;

	arm64_notify_die(desc, regs, &info, 0);
=======
	arm64_notify_die(desc, regs, signal, code, address, err);
>>>>>>> upstream/android-13
}

/*
 * Set up process info to signal segmentation fault - called on access error.
 */
void arm64_notify_segfault(unsigned long addr)
{
	int code;

<<<<<<< HEAD
	down_read(&current->mm->mmap_sem);
	if (find_vma(current->mm, addr) == NULL)
		code = SEGV_MAPERR;
	else
		code = SEGV_ACCERR;
	up_read(&current->mm->mmap_sem);

	force_signal_inject(SIGSEGV, code, addr);
}

asmlinkage void __exception do_undefinstr(struct pt_regs *regs)
{
#ifdef CONFIG_SEC_DEBUG_EXTRA_INFO
	if (!user_mode(regs)) {
		sec_debug_set_extra_info_fault((unsigned long)regs->pc, regs);
	}
#endif

=======
	mmap_read_lock(current->mm);
	if (find_vma(current->mm, untagged_addr(addr)) == NULL)
		code = SEGV_MAPERR;
	else
		code = SEGV_ACCERR;
	mmap_read_unlock(current->mm);

	force_signal_inject(SIGSEGV, code, addr, 0);
}

#ifdef CONFIG_S3C2410_BUILTIN_WATCHDOG
extern int s3c2410wdt_builtin_expire_watchdog(void);
static inline void do_s3c2410wdt_builtin_expire_watchdog(void)
{
	s3c2410wdt_builtin_expire_watchdog();
}
#else
static inline void do_s3c2410wdt_builtin_expire_watchdog(void)
{
}
#endif
void do_undefinstr(struct pt_regs *regs)
{
>>>>>>> upstream/android-13
	/* check for AArch32 breakpoint instructions */
	if (!aarch32_break_handler(regs))
		return;

	if (call_undef_hook(regs) == 0)
		return;

<<<<<<< HEAD
	force_signal_inject(SIGILL, ILL_ILLOPC, regs->pc);
	BUG_ON(!user_mode(regs));
}

void cpu_enable_cache_maint_trap(const struct arm64_cpu_capabilities *__unused)
{
	sysreg_clear_set(sctlr_el1, SCTLR_EL1_UCI, 0);
}
=======
	trace_android_rvh_do_undefinstr(regs);

	if (IS_ENABLED(CONFIG_SEC_DEBUG_FAULT_MSG_ADV) && !user_mode(regs)) {
		pr_auto(ASL1, "%s: pc=0x%016llx\n",
			"undefined instruction", regs->pc);
		dump_kernel_instr(KERN_INFO, regs);
		secdbg_dump_kernel_instr_ext(regs);
	}

	if (!user_mode(regs))
		do_s3c2410wdt_builtin_expire_watchdog();

	if (IS_ENABLED(CONFIG_SEC_DEBUG_FAULT_MSG_ADV) && !user_mode(regs))
		die("undefined instruction", regs, 0);

	BUG_ON(!user_mode(regs));
	force_signal_inject(SIGILL, ILL_ILLOPC, regs->pc, 0);
}
NOKPROBE_SYMBOL(do_undefinstr);

void do_bti(struct pt_regs *regs)
{
	BUG_ON(!user_mode(regs));
	force_signal_inject(SIGILL, ILL_ILLOPC, regs->pc, 0);
}
NOKPROBE_SYMBOL(do_bti);

#define show_pac_key_single_kern(k, uk, kk)					\
do {										\
	u64 lo, hi;								\
	lo = read_sysreg_s(SYS_ ## k ## KEYLO_EL1);				\
	hi = read_sysreg_s(SYS_ ## k ## KEYHI_EL1);				\
	pr_auto(ASL1, #k "KEY_EL1: REG %016lx::%016lx, USER %016lx::%016lx, KERN %016lx::%016lx\n", \
		hi, lo, (uk).hi, (uk).lo, (kk).hi, (kk).lo);			\
} while (0)

#define show_pac_key_single(k, uk)						\
do {										\
	u64 lo, hi;								\
	lo = read_sysreg_s(SYS_ ## k ## KEYLO_EL1);				\
	hi = read_sysreg_s(SYS_ ## k ## KEYHI_EL1);				\
	pr_auto(ASL1, #k "KEY_EL1: REG %016lx::%016lx, USER %016lx::%016lx\n",		\
		hi, lo, (uk).hi, (uk).lo);					\
} while (0)

static void show_pac_keys(struct ptrauth_keys_user *userk, struct ptrauth_keys_kernel *kernk)
{
#ifdef CONFIG_ARM64_PTR_AUTH
	pr_crit("PtrAuth status: address auth? %d, generic auth? %d\n",
		system_supports_address_auth(), system_supports_generic_auth());

	if (!system_supports_address_auth() && !system_supports_generic_auth())
		return;

	if (system_supports_address_auth()) {
		show_pac_key_single_kern(APIA, userk->apia, kernk->apia);
		show_pac_key_single(APIB, userk->apib);
		show_pac_key_single(APDA, userk->apda);
		show_pac_key_single(APDB, userk->apdb);
	}

	if (system_supports_generic_auth())
		show_pac_key_single(APGA, userk->apga);
#endif
}

void do_ptrauth_fault(struct pt_regs *regs, unsigned int esr)
{
	/*
	 * Unexpected FPAC exception or pointer authentication failure in
	 * the kernel: kill the task before it does any more harm.
	 */
	trace_android_rvh_do_ptrauth_fault(regs, esr);

	if (IS_ENABLED(CONFIG_SEC_DEBUG_FAULT_MSG_ADV) && !user_mode(regs)) {
		pr_auto(ASL1, "Wrong PAC detected on CPU%d, LR 0x%010lx, code 0x%08x -- %s\n",
			smp_processor_id(), regs->regs[30], esr, esr_get_class_string(esr));
		show_pac_keys(&current->thread.keys_user, &current->thread.keys_kernel);
		die("ptrauth fault", regs, 0);
	}

	BUG_ON(!user_mode(regs));
	force_signal_inject(SIGILL, ILL_ILLOPN, regs->pc, esr);
}
NOKPROBE_SYMBOL(do_ptrauth_fault);
>>>>>>> upstream/android-13

#define __user_cache_maint(insn, address, res)			\
	if (address >= user_addr_max()) {			\
		res = -EFAULT;					\
	} else {						\
		uaccess_ttbr0_enable();				\
		asm volatile (					\
			"1:	" insn ", %1\n"			\
			"	mov	%w0, #0\n"		\
			"2:\n"					\
			"	.pushsection .fixup,\"ax\"\n"	\
			"	.align	2\n"			\
			"3:	mov	%w0, %w2\n"		\
			"	b	2b\n"			\
			"	.popsection\n"			\
			_ASM_EXTABLE(1b, 3b)			\
			: "=r" (res)				\
			: "r" (address), "i" (-EFAULT));	\
		uaccess_ttbr0_disable();			\
	}

static void user_cache_maint_handler(unsigned int esr, struct pt_regs *regs)
{
<<<<<<< HEAD
	unsigned long address;
	int rt = (esr & ESR_ELx_SYS64_ISS_RT_MASK) >> ESR_ELx_SYS64_ISS_RT_SHIFT;
	int crm = (esr & ESR_ELx_SYS64_ISS_CRM_MASK) >> ESR_ELx_SYS64_ISS_CRM_SHIFT;
	int ret = 0;

	address = untagged_addr(pt_regs_read_reg(regs, rt));
=======
	unsigned long tagged_address, address;
	int rt = ESR_ELx_SYS64_ISS_RT(esr);
	int crm = (esr & ESR_ELx_SYS64_ISS_CRM_MASK) >> ESR_ELx_SYS64_ISS_CRM_SHIFT;
	int ret = 0;

	tagged_address = pt_regs_read_reg(regs, rt);
	address = untagged_addr(tagged_address);
>>>>>>> upstream/android-13

	switch (crm) {
	case ESR_ELx_SYS64_ISS_CRM_DC_CVAU:	/* DC CVAU, gets promoted */
		__user_cache_maint("dc civac", address, ret);
		break;
	case ESR_ELx_SYS64_ISS_CRM_DC_CVAC:	/* DC CVAC, gets promoted */
		__user_cache_maint("dc civac", address, ret);
		break;
<<<<<<< HEAD
=======
	case ESR_ELx_SYS64_ISS_CRM_DC_CVADP:	/* DC CVADP */
		__user_cache_maint("sys 3, c7, c13, 1", address, ret);
		break;
>>>>>>> upstream/android-13
	case ESR_ELx_SYS64_ISS_CRM_DC_CVAP:	/* DC CVAP */
		__user_cache_maint("sys 3, c7, c12, 1", address, ret);
		break;
	case ESR_ELx_SYS64_ISS_CRM_DC_CIVAC:	/* DC CIVAC */
		__user_cache_maint("dc civac", address, ret);
		break;
	case ESR_ELx_SYS64_ISS_CRM_IC_IVAU:	/* IC IVAU */
		__user_cache_maint("ic ivau", address, ret);
		break;
	default:
<<<<<<< HEAD
		force_signal_inject(SIGILL, ILL_ILLOPC, regs->pc);
=======
		force_signal_inject(SIGILL, ILL_ILLOPC, regs->pc, 0);
>>>>>>> upstream/android-13
		return;
	}

	if (ret)
<<<<<<< HEAD
		arm64_notify_segfault(address);
=======
		arm64_notify_segfault(tagged_address);
>>>>>>> upstream/android-13
	else
		arm64_skip_faulting_instruction(regs, AARCH64_INSN_SIZE);
}

static void ctr_read_handler(unsigned int esr, struct pt_regs *regs)
{
<<<<<<< HEAD
	int rt = (esr & ESR_ELx_SYS64_ISS_RT_MASK) >> ESR_ELx_SYS64_ISS_RT_SHIFT;
=======
	int rt = ESR_ELx_SYS64_ISS_RT(esr);
>>>>>>> upstream/android-13
	unsigned long val = arm64_ftr_reg_user_value(&arm64_ftr_reg_ctrel0);

	if (cpus_have_const_cap(ARM64_WORKAROUND_1542419)) {
		/* Hide DIC so that we can trap the unnecessary maintenance...*/
		val &= ~BIT(CTR_DIC_SHIFT);

		/* ... and fake IminLine to reduce the number of traps. */
		val &= ~CTR_IMINLINE_MASK;
		val |= (PAGE_SHIFT - 2) & CTR_IMINLINE_MASK;
	}

	pt_regs_write_reg(regs, rt, val);

	arm64_skip_faulting_instruction(regs, AARCH64_INSN_SIZE);
}

static void cntvct_read_handler(unsigned int esr, struct pt_regs *regs)
{
<<<<<<< HEAD
	int rt = (esr & ESR_ELx_SYS64_ISS_RT_MASK) >> ESR_ELx_SYS64_ISS_RT_SHIFT;

	pt_regs_write_reg(regs, rt, arch_counter_get_cntvct());
=======
	int rt = ESR_ELx_SYS64_ISS_RT(esr);

	pt_regs_write_reg(regs, rt, arch_timer_read_counter());
>>>>>>> upstream/android-13
	arm64_skip_faulting_instruction(regs, AARCH64_INSN_SIZE);
}

static void cntfrq_read_handler(unsigned int esr, struct pt_regs *regs)
{
<<<<<<< HEAD
	int rt = (esr & ESR_ELx_SYS64_ISS_RT_MASK) >> ESR_ELx_SYS64_ISS_RT_SHIFT;
=======
	int rt = ESR_ELx_SYS64_ISS_RT(esr);
>>>>>>> upstream/android-13

	pt_regs_write_reg(regs, rt, arch_timer_get_rate());
	arm64_skip_faulting_instruction(regs, AARCH64_INSN_SIZE);
}

<<<<<<< HEAD
=======
static void mrs_handler(unsigned int esr, struct pt_regs *regs)
{
	u32 sysreg, rt;

	rt = ESR_ELx_SYS64_ISS_RT(esr);
	sysreg = esr_sys64_to_sysreg(esr);

	if (do_emulate_mrs(regs, sysreg, rt) != 0)
		force_signal_inject(SIGILL, ILL_ILLOPC, regs->pc, 0);
}

static void wfi_handler(unsigned int esr, struct pt_regs *regs)
{
	arm64_skip_faulting_instruction(regs, AARCH64_INSN_SIZE);
}

>>>>>>> upstream/android-13
struct sys64_hook {
	unsigned int esr_mask;
	unsigned int esr_val;
	void (*handler)(unsigned int esr, struct pt_regs *regs);
};

<<<<<<< HEAD
static struct sys64_hook sys64_hooks[] = {
=======
static const struct sys64_hook sys64_hooks[] = {
>>>>>>> upstream/android-13
	{
		.esr_mask = ESR_ELx_SYS64_ISS_EL0_CACHE_OP_MASK,
		.esr_val = ESR_ELx_SYS64_ISS_EL0_CACHE_OP_VAL,
		.handler = user_cache_maint_handler,
	},
	{
		/* Trap read access to CTR_EL0 */
		.esr_mask = ESR_ELx_SYS64_ISS_SYS_OP_MASK,
		.esr_val = ESR_ELx_SYS64_ISS_SYS_CTR_READ,
		.handler = ctr_read_handler,
	},
	{
		/* Trap read access to CNTVCT_EL0 */
		.esr_mask = ESR_ELx_SYS64_ISS_SYS_OP_MASK,
		.esr_val = ESR_ELx_SYS64_ISS_SYS_CNTVCT,
		.handler = cntvct_read_handler,
	},
	{
		/* Trap read access to CNTFRQ_EL0 */
		.esr_mask = ESR_ELx_SYS64_ISS_SYS_OP_MASK,
		.esr_val = ESR_ELx_SYS64_ISS_SYS_CNTFRQ,
		.handler = cntfrq_read_handler,
	},
<<<<<<< HEAD
	{},
};

asmlinkage void __exception do_sysinstr(unsigned int esr, struct pt_regs *regs)
{
	struct sys64_hook *hook;
=======
	{
		/* Trap read access to CPUID registers */
		.esr_mask = ESR_ELx_SYS64_ISS_SYS_MRS_OP_MASK,
		.esr_val = ESR_ELx_SYS64_ISS_SYS_MRS_OP_VAL,
		.handler = mrs_handler,
	},
	{
		/* Trap WFI instructions executed in userspace */
		.esr_mask = ESR_ELx_WFx_MASK,
		.esr_val = ESR_ELx_WFx_WFI_VAL,
		.handler = wfi_handler,
	},
	{},
};

#ifdef CONFIG_COMPAT
static bool cp15_cond_valid(unsigned int esr, struct pt_regs *regs)
{
	int cond;

	/* Only a T32 instruction can trap without CV being set */
	if (!(esr & ESR_ELx_CV)) {
		u32 it;

		it = compat_get_it_state(regs);
		if (!it)
			return true;

		cond = it >> 4;
	} else {
		cond = (esr & ESR_ELx_COND_MASK) >> ESR_ELx_COND_SHIFT;
	}

	return aarch32_opcode_cond_checks[cond](regs->pstate);
}

static void compat_cntfrq_read_handler(unsigned int esr, struct pt_regs *regs)
{
	int reg = (esr & ESR_ELx_CP15_32_ISS_RT_MASK) >> ESR_ELx_CP15_32_ISS_RT_SHIFT;

	pt_regs_write_reg(regs, reg, arch_timer_get_rate());
	arm64_skip_faulting_instruction(regs, 4);
}

static const struct sys64_hook cp15_32_hooks[] = {
	{
		.esr_mask = ESR_ELx_CP15_32_ISS_SYS_MASK,
		.esr_val = ESR_ELx_CP15_32_ISS_SYS_CNTFRQ,
		.handler = compat_cntfrq_read_handler,
	},
	{},
};

static void compat_cntvct_read_handler(unsigned int esr, struct pt_regs *regs)
{
	int rt = (esr & ESR_ELx_CP15_64_ISS_RT_MASK) >> ESR_ELx_CP15_64_ISS_RT_SHIFT;
	int rt2 = (esr & ESR_ELx_CP15_64_ISS_RT2_MASK) >> ESR_ELx_CP15_64_ISS_RT2_SHIFT;
	u64 val = arch_timer_read_counter();

	pt_regs_write_reg(regs, rt, lower_32_bits(val));
	pt_regs_write_reg(regs, rt2, upper_32_bits(val));
	arm64_skip_faulting_instruction(regs, 4);
}

static const struct sys64_hook cp15_64_hooks[] = {
	{
		.esr_mask = ESR_ELx_CP15_64_ISS_SYS_MASK,
		.esr_val = ESR_ELx_CP15_64_ISS_SYS_CNTVCT,
		.handler = compat_cntvct_read_handler,
	},
	{},
};

void do_cp15instr(unsigned int esr, struct pt_regs *regs)
{
	const struct sys64_hook *hook, *hook_base;

	if (!cp15_cond_valid(esr, regs)) {
		/*
		 * There is no T16 variant of a CP access, so we
		 * always advance PC by 4 bytes.
		 */
		arm64_skip_faulting_instruction(regs, 4);
		return;
	}

	switch (ESR_ELx_EC(esr)) {
	case ESR_ELx_EC_CP15_32:
		hook_base = cp15_32_hooks;
		break;
	case ESR_ELx_EC_CP15_64:
		hook_base = cp15_64_hooks;
		break;
	default:
		do_undefinstr(regs);
		return;
	}

	for (hook = hook_base; hook->handler; hook++)
		if ((hook->esr_mask & esr) == hook->esr_val) {
			hook->handler(esr, regs);
			return;
		}

	/*
	 * New cp15 instructions may previously have been undefined at
	 * EL0. Fall back to our usual undefined instruction handler
	 * so that we handle these consistently.
	 */
	do_undefinstr(regs);
}
NOKPROBE_SYMBOL(do_cp15instr);
#endif

void do_sysinstr(unsigned int esr, struct pt_regs *regs)
{
	const struct sys64_hook *hook;
>>>>>>> upstream/android-13

	for (hook = sys64_hooks; hook->handler; hook++)
		if ((hook->esr_mask & esr) == hook->esr_val) {
			hook->handler(esr, regs);
			return;
		}

	/*
	 * New SYS instructions may previously have been undefined at EL0. Fall
	 * back to our usual undefined instruction handler so that we handle
	 * these consistently.
	 */
	do_undefinstr(regs);
}
<<<<<<< HEAD
=======
NOKPROBE_SYMBOL(do_sysinstr);
>>>>>>> upstream/android-13

static const char *esr_class_str[] = {
	[0 ... ESR_ELx_EC_MAX]		= "UNRECOGNIZED EC",
	[ESR_ELx_EC_UNKNOWN]		= "Unknown/Uncategorized",
	[ESR_ELx_EC_WFx]		= "WFI/WFE",
	[ESR_ELx_EC_CP15_32]		= "CP15 MCR/MRC",
	[ESR_ELx_EC_CP15_64]		= "CP15 MCRR/MRRC",
	[ESR_ELx_EC_CP14_MR]		= "CP14 MCR/MRC",
	[ESR_ELx_EC_CP14_LS]		= "CP14 LDC/STC",
	[ESR_ELx_EC_FP_ASIMD]		= "ASIMD",
	[ESR_ELx_EC_CP10_ID]		= "CP10 MRC/VMRS",
<<<<<<< HEAD
	[ESR_ELx_EC_CP14_64]		= "CP14 MCRR/MRRC",
=======
	[ESR_ELx_EC_PAC]		= "PAC",
	[ESR_ELx_EC_CP14_64]		= "CP14 MCRR/MRRC",
	[ESR_ELx_EC_BTI]		= "BTI",
>>>>>>> upstream/android-13
	[ESR_ELx_EC_ILL]		= "PSTATE.IL",
	[ESR_ELx_EC_SVC32]		= "SVC (AArch32)",
	[ESR_ELx_EC_HVC32]		= "HVC (AArch32)",
	[ESR_ELx_EC_SMC32]		= "SMC (AArch32)",
	[ESR_ELx_EC_SVC64]		= "SVC (AArch64)",
	[ESR_ELx_EC_HVC64]		= "HVC (AArch64)",
	[ESR_ELx_EC_SMC64]		= "SMC (AArch64)",
	[ESR_ELx_EC_SYS64]		= "MSR/MRS (AArch64)",
	[ESR_ELx_EC_SVE]		= "SVE",
<<<<<<< HEAD
=======
	[ESR_ELx_EC_ERET]		= "ERET/ERETAA/ERETAB",
	[ESR_ELx_EC_FPAC]		= "FPAC",
>>>>>>> upstream/android-13
	[ESR_ELx_EC_IMP_DEF]		= "EL3 IMP DEF",
	[ESR_ELx_EC_IABT_LOW]		= "IABT (lower EL)",
	[ESR_ELx_EC_IABT_CUR]		= "IABT (current EL)",
	[ESR_ELx_EC_PC_ALIGN]		= "PC Alignment",
	[ESR_ELx_EC_DABT_LOW]		= "DABT (lower EL)",
	[ESR_ELx_EC_DABT_CUR]		= "DABT (current EL)",
	[ESR_ELx_EC_SP_ALIGN]		= "SP Alignment",
	[ESR_ELx_EC_FP_EXC32]		= "FP (AArch32)",
	[ESR_ELx_EC_FP_EXC64]		= "FP (AArch64)",
	[ESR_ELx_EC_SERROR]		= "SError",
	[ESR_ELx_EC_BREAKPT_LOW]	= "Breakpoint (lower EL)",
	[ESR_ELx_EC_BREAKPT_CUR]	= "Breakpoint (current EL)",
	[ESR_ELx_EC_SOFTSTP_LOW]	= "Software Step (lower EL)",
	[ESR_ELx_EC_SOFTSTP_CUR]	= "Software Step (current EL)",
	[ESR_ELx_EC_WATCHPT_LOW]	= "Watchpoint (lower EL)",
	[ESR_ELx_EC_WATCHPT_CUR]	= "Watchpoint (current EL)",
	[ESR_ELx_EC_BKPT32]		= "BKPT (AArch32)",
	[ESR_ELx_EC_VECTOR32]		= "Vector catch (AArch32)",
	[ESR_ELx_EC_BRK64]		= "BRK (AArch64)",
};

const char *esr_get_class_string(u32 esr)
{
	return esr_class_str[ESR_ELx_EC(esr)];
}

/*
<<<<<<< HEAD
 * bad_mode handles the impossible case in the exception vector. This is always
 * fatal.
 */
asmlinkage void bad_mode(struct pt_regs *regs, int reason, unsigned int esr)
{
	console_verbose();

#ifdef CONFIG_SEC_DEBUG_AUTO_COMMENT
	pr_auto(ASL1,"Bad mode in %s handler detected on CPU%d, code 0x%08x -- %s\n",
		handler[reason], smp_processor_id(), esr,
		esr_get_class_string(esr));
#else
	pr_crit("Bad mode in %s handler detected on CPU%d, code 0x%08x -- %s\n",
		handler[reason], smp_processor_id(), esr,
		esr_get_class_string(esr));
#endif

#ifdef CONFIG_SEC_DEBUG_EXTRA_INFO
	if (!user_mode(regs)) {
		sec_debug_set_extra_info_fault((unsigned long)regs->pc, regs);
		sec_debug_set_extra_info_esr(esr);
	}
#endif

	local_daif_mask();
	panic("bad mode");
}

/*
 * bad_el0_sync handles unexpected, but potentially recoverable synchronous
 * exceptions taken from EL0. Unlike bad_mode, this returns.
 */
asmlinkage void bad_el0_sync(struct pt_regs *regs, int reason, unsigned int esr)
{
	siginfo_t info;
	void __user *pc = (void __user *)instruction_pointer(regs);

#ifdef CONFIG_SEC_DEBUG_EXTRA_INFO
	if (!user_mode(regs)) {
		sec_debug_set_extra_info_fault(SEC_DEBUG_BADMODE_MAGIC, regs);
		sec_debug_set_extra_info_esr(esr);
	}
#endif

	clear_siginfo(&info);
	info.si_signo = SIGILL;
	info.si_errno = 0;
	info.si_code  = ILL_ILLOPC;
	info.si_addr  = pc;
=======
 * bad_el0_sync handles unexpected, but potentially recoverable synchronous
 * exceptions taken from EL0.
 */
void bad_el0_sync(struct pt_regs *regs, int reason, unsigned int esr)
{
	unsigned long pc = instruction_pointer(regs);
>>>>>>> upstream/android-13

	current->thread.fault_address = 0;
	current->thread.fault_code = esr;

<<<<<<< HEAD
	arm64_force_sig_info(&info, "Bad EL0 synchronous exception", current);
=======
	arm64_force_sig_fault(SIGILL, ILL_ILLOPC, pc,
			      "Bad EL0 synchronous exception");
>>>>>>> upstream/android-13
}

#ifdef CONFIG_VMAP_STACK

DEFINE_PER_CPU(unsigned long [OVERFLOW_STACK_SIZE/sizeof(long)], overflow_stack)
	__aligned(16);

<<<<<<< HEAD
asmlinkage void handle_bad_stack(struct pt_regs *regs)
=======
void panic_bad_stack(struct pt_regs *regs, unsigned int esr, unsigned long far)
>>>>>>> upstream/android-13
{
	unsigned long tsk_stk = (unsigned long)current->stack;
	unsigned long irq_stk = (unsigned long)this_cpu_read(irq_stack_ptr);
	unsigned long ovf_stk = (unsigned long)this_cpu_ptr(overflow_stack);
<<<<<<< HEAD
	unsigned int esr = read_sysreg(esr_el1);
	unsigned long far = read_sysreg(far_el1);
=======
>>>>>>> upstream/android-13

	console_verbose();
	pr_emerg("Insufficient stack space to handle exception!");

	pr_emerg("ESR: 0x%08x -- %s\n", esr, esr_get_class_string(esr));
	pr_emerg("FAR: 0x%016lx\n", far);

	pr_emerg("Task stack:     [0x%016lx..0x%016lx]\n",
		 tsk_stk, tsk_stk + THREAD_SIZE);
	pr_emerg("IRQ stack:      [0x%016lx..0x%016lx]\n",
<<<<<<< HEAD
		 irq_stk, irq_stk + THREAD_SIZE);
=======
		 irq_stk, irq_stk + IRQ_STACK_SIZE);
>>>>>>> upstream/android-13
	pr_emerg("Overflow stack: [0x%016lx..0x%016lx]\n",
		 ovf_stk, ovf_stk + OVERFLOW_STACK_SIZE);

	__show_regs(regs);

	/*
	 * We use nmi_panic to limit the potential for recusive overflows, and
	 * to get a better stack trace.
	 */
	nmi_panic(NULL, "kernel stack overflow");
	cpu_park_loop();
}
#endif

void __noreturn arm64_serror_panic(struct pt_regs *regs, u32 esr)
{
	console_verbose();

<<<<<<< HEAD
	pr_crit("SError Interrupt on CPU%d, code 0x%08x -- %s\n",
		smp_processor_id(), esr, esr_get_class_string(esr));
=======
	pr_auto(ASL1, "SError Interrupt on CPU%d, code 0x%08x -- %s\n",
		smp_processor_id(), esr, esr_get_class_string(esr));

	trace_android_rvh_arm64_serror_panic(regs, esr);
>>>>>>> upstream/android-13
	if (regs)
		__show_regs(regs);

	nmi_panic(regs, "Asynchronous SError Interrupt");

	cpu_park_loop();
	unreachable();
}

bool arm64_is_fatal_ras_serror(struct pt_regs *regs, unsigned int esr)
{
	u32 aet = arm64_ras_serror_get_severity(esr);

	switch (aet) {
	case ESR_ELx_AET_CE:	/* corrected error */
	case ESR_ELx_AET_UEO:	/* restartable, not yet consumed */
		/*
		 * The CPU can make progress. We may take UEO again as
		 * a more severe error.
		 */
		return false;

	case ESR_ELx_AET_UEU:	/* Uncorrected Unrecoverable */
	case ESR_ELx_AET_UER:	/* Uncorrected Recoverable */
		/*
		 * The CPU can't make progress. The exception may have
		 * been imprecise.
<<<<<<< HEAD
=======
		 *
		 * Neoverse-N1 #1349291 means a non-KVM SError reported as
		 * Unrecoverable should be treated as Uncontainable. We
		 * call arm64_serror_panic() in both cases.
>>>>>>> upstream/android-13
		 */
		return true;

	case ESR_ELx_AET_UC:	/* Uncontainable or Uncategorized error */
	default:
		/* Error has been silently propagated */
		arm64_serror_panic(regs, esr);
	}
}

<<<<<<< HEAD
asmlinkage void do_serror(struct pt_regs *regs, unsigned int esr)
{
	nmi_enter();

	/* non-RAS errors are not containable */
	if (!arm64_is_ras_serror(esr) || arm64_is_fatal_ras_serror(regs, esr))
		arm64_serror_panic(regs, esr);

	nmi_exit();
}

void __pte_error(const char *file, int line, unsigned long val)
{
	pr_err("%s:%d: bad pte %016lx.\n", file, line, val);
}

void __pmd_error(const char *file, int line, unsigned long val)
{
	pr_err("%s:%d: bad pmd %016lx.\n", file, line, val);
}

void __pud_error(const char *file, int line, unsigned long val)
{
	pr_err("%s:%d: bad pud %016lx.\n", file, line, val);
}

void __pgd_error(const char *file, int line, unsigned long val)
{
	pr_err("%s:%d: bad pgd %016lx.\n", file, line, val);
=======
void do_serror(struct pt_regs *regs, unsigned int esr)
{
	/* non-RAS errors are not containable */
	if (!arm64_is_ras_serror(esr) || arm64_is_fatal_ras_serror(regs, esr))
		arm64_serror_panic(regs, esr);
>>>>>>> upstream/android-13
}

/* GENERIC_BUG traps */

int is_valid_bugaddr(unsigned long addr)
{
	/*
	 * bug_handler() only called for BRK #BUG_BRK_IMM.
	 * So the answer is trivial -- any spurious instances with no
	 * bug table entry will be rejected by report_bug() and passed
	 * back to the debug-monitors code and handled as a fatal
	 * unexpected debug exception.
	 */
	return 1;
}

static int bug_handler(struct pt_regs *regs, unsigned int esr)
{
<<<<<<< HEAD
	if (user_mode(regs))
		return DBG_HOOK_ERROR;

	switch (report_bug(regs->pc, regs)) {
	case BUG_TRAP_TYPE_BUG:
#ifdef CONFIG_SEC_DEBUG_EXTRA_INFO
		sec_debug_set_extra_info_fault((unsigned long)regs->pc, regs);
		sec_debug_set_extra_info_esr(esr);
#endif

=======
	switch (report_bug(regs->pc, regs)) {
	case BUG_TRAP_TYPE_BUG:
>>>>>>> upstream/android-13
		die("Oops - BUG", regs, 0);
		break;

	case BUG_TRAP_TYPE_WARN:
		break;

	default:
		/* unknown/unrecognised bug trap type */
		return DBG_HOOK_ERROR;
	}

	/* If thread survives, skip over the BUG instruction and continue: */
	arm64_skip_faulting_instruction(regs, AARCH64_INSN_SIZE);
	return DBG_HOOK_HANDLED;
}

static struct break_hook bug_break_hook = {
<<<<<<< HEAD
	.esr_val = 0xf2000000 | BUG_BRK_IMM,
	.esr_mask = 0xffffffff,
	.fn = bug_handler,
=======
	.fn = bug_handler,
	.imm = BUG_BRK_IMM,
};

static int reserved_fault_handler(struct pt_regs *regs, unsigned int esr)
{
	pr_err("%s generated an invalid instruction at %pS!\n",
		"Kernel text patching",
		(void *)instruction_pointer(regs));

	/* We cannot handle this */
	return DBG_HOOK_ERROR;
}

static struct break_hook fault_break_hook = {
	.fn = reserved_fault_handler,
	.imm = FAULT_BRK_IMM,
>>>>>>> upstream/android-13
};

#ifdef CONFIG_KASAN_SW_TAGS

#define KASAN_ESR_RECOVER	0x20
#define KASAN_ESR_WRITE	0x10
#define KASAN_ESR_SIZE_MASK	0x0f
#define KASAN_ESR_SIZE(esr)	(1 << ((esr) & KASAN_ESR_SIZE_MASK))

static int kasan_handler(struct pt_regs *regs, unsigned int esr)
{
	bool recover = esr & KASAN_ESR_RECOVER;
	bool write = esr & KASAN_ESR_WRITE;
	size_t size = KASAN_ESR_SIZE(esr);
	u64 addr = regs->regs[0];
	u64 pc = regs->pc;

<<<<<<< HEAD
	if (user_mode(regs))
		return DBG_HOOK_ERROR;

=======
>>>>>>> upstream/android-13
	kasan_report(addr, size, write, pc);

	/*
	 * The instrumentation allows to control whether we can proceed after
	 * a crash was detected. This is done by passing the -recover flag to
	 * the compiler. Disabling recovery allows to generate more compact
	 * code.
	 *
	 * Unfortunately disabling recovery doesn't work for the kernel right
	 * now. KASAN reporting is disabled in some contexts (for example when
	 * the allocator accesses slab object metadata; this is controlled by
	 * current->kasan_depth). All these accesses are detected by the tool,
	 * even though the reports for them are not printed.
	 *
	 * This is something that might be fixed at some point in the future.
	 */
	if (!recover)
		die("Oops - KASAN", regs, 0);

	/* If thread survives, skip over the brk instruction and continue: */
	arm64_skip_faulting_instruction(regs, AARCH64_INSN_SIZE);
	return DBG_HOOK_HANDLED;
}

<<<<<<< HEAD
#define KASAN_ESR_VAL (0xf2000000 | KASAN_BRK_IMM)
#define KASAN_ESR_MASK 0xffffff00

static struct break_hook kasan_break_hook = {
	.esr_val = KASAN_ESR_VAL,
	.esr_mask = KASAN_ESR_MASK,
	.fn = kasan_handler,
=======
static struct break_hook kasan_break_hook = {
	.fn	= kasan_handler,
	.imm	= KASAN_BRK_IMM,
	.mask	= KASAN_BRK_MASK,
>>>>>>> upstream/android-13
};
#endif

/*
 * Initial handler for AArch64 BRK exceptions
 * This handler only used until debug_traps_init().
 */
int __init early_brk64(unsigned long addr, unsigned int esr,
		struct pt_regs *regs)
{
#ifdef CONFIG_KASAN_SW_TAGS
<<<<<<< HEAD
	if ((esr & KASAN_ESR_MASK) == KASAN_ESR_VAL)
=======
	unsigned int comment = esr & ESR_ELx_BRK64_ISS_COMMENT_MASK;

	if ((comment & ~KASAN_BRK_MASK) == KASAN_BRK_IMM)
>>>>>>> upstream/android-13
		return kasan_handler(regs, esr) != DBG_HOOK_HANDLED;
#endif
	return bug_handler(regs, esr) != DBG_HOOK_HANDLED;
}

<<<<<<< HEAD
/* This registration must happen early, before debug_traps_init(). */
void __init trap_init(void)
{
	register_break_hook(&bug_break_hook);
#ifdef CONFIG_KASAN_SW_TAGS
	register_break_hook(&kasan_break_hook);
#endif
=======
void __init trap_init(void)
{
	register_kernel_break_hook(&bug_break_hook);
	register_kernel_break_hook(&fault_break_hook);
#ifdef CONFIG_KASAN_SW_TAGS
	register_kernel_break_hook(&kasan_break_hook);
#endif
	debug_traps_init();
>>>>>>> upstream/android-13
}
