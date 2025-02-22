<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  Copyright (C) 1994 Linus Torvalds
 *
 *  Pentium III FXSR, SSE support
 *  General FPU state handling cleanups
 *	Gareth Hughes <gareth@valinux.com>, May 2000
 */
#include <asm/fpu/internal.h>
#include <asm/fpu/regset.h>
#include <asm/fpu/signal.h>
#include <asm/fpu/types.h>
#include <asm/traps.h>
#include <asm/irq_regs.h>

#include <linux/hardirq.h>
#include <linux/pkeys.h>

#define CREATE_TRACE_POINTS
#include <asm/trace/fpu.h>

/*
 * Represents the initial FPU state. It's mostly (but not completely) zeroes,
 * depending on the FPU hardware format:
 */
<<<<<<< HEAD
union fpregs_state init_fpstate __read_mostly;

/*
 * Track whether the kernel is using the FPU state
 * currently.
 *
 * This flag is used:
 *
 *   - by IRQ context code to potentially use the FPU
 *     if it's unused.
 *
 *   - to debug kernel_fpu_begin()/end() correctness
 */
=======
union fpregs_state init_fpstate __ro_after_init;

/* Track in-kernel FPU usage */
>>>>>>> upstream/android-13
static DEFINE_PER_CPU(bool, in_kernel_fpu);

/*
 * Track which context is using the FPU on the CPU:
 */
DEFINE_PER_CPU(struct fpu *, fpu_fpregs_owner_ctx);

<<<<<<< HEAD
static void kernel_fpu_disable(void)
{
	WARN_ON_FPU(this_cpu_read(in_kernel_fpu));
	this_cpu_write(in_kernel_fpu, true);
}

static void kernel_fpu_enable(void)
{
	WARN_ON_FPU(!this_cpu_read(in_kernel_fpu));
	this_cpu_write(in_kernel_fpu, false);
}

static bool kernel_fpu_disabled(void)
{
	return this_cpu_read(in_kernel_fpu);
}

static bool interrupted_kernel_fpu_idle(void)
{
	return !kernel_fpu_disabled();
}

/*
 * Were we in user mode (or vm86 mode) when we were
 * interrupted?
 *
 * Doing kernel_fpu_begin/end() is ok if we are running
 * in an interrupt context from user mode - we'll just
 * save the FPU state as required.
 */
static bool interrupted_user_mode(void)
{
	struct pt_regs *regs = get_irq_regs();
	return regs && user_mode(regs);
}

/*
 * Can we use the FPU in kernel mode with the
 * whole "kernel_fpu_begin/end()" sequence?
 *
 * It's always ok in process context (ie "not interrupt")
 * but it is sometimes ok even from an irq.
 */
bool irq_fpu_usable(void)
{
	return !in_interrupt() ||
		interrupted_user_mode() ||
		interrupted_kernel_fpu_idle();
}
EXPORT_SYMBOL(irq_fpu_usable);

static void __kernel_fpu_begin(void)
{
	struct fpu *fpu = &current->thread.fpu;

	WARN_ON_FPU(!irq_fpu_usable());

	kernel_fpu_disable();

	if (fpu->initialized) {
		/*
		 * Ignore return value -- we don't care if reg state
		 * is clobbered.
		 */
		copy_fpregs_to_fpstate(fpu);
	} else {
		__cpu_invalidate_fpregs_state();
	}
}

static void __kernel_fpu_end(void)
{
	struct fpu *fpu = &current->thread.fpu;

	if (fpu->initialized)
		copy_kernel_to_fpregs(&fpu->state);

	kernel_fpu_enable();
}

void kernel_fpu_begin(void)
{
	preempt_disable();
	__kernel_fpu_begin();
}
EXPORT_SYMBOL_GPL(kernel_fpu_begin);

void kernel_fpu_end(void)
{
	__kernel_fpu_end();
=======
/*
 * Can we use the FPU in kernel mode with the
 * whole "kernel_fpu_begin/end()" sequence?
 */
bool irq_fpu_usable(void)
{
	if (WARN_ON_ONCE(in_nmi()))
		return false;

	/* In kernel FPU usage already active? */
	if (this_cpu_read(in_kernel_fpu))
		return false;

	/*
	 * When not in NMI or hard interrupt context, FPU can be used in:
	 *
	 * - Task context except from within fpregs_lock()'ed critical
	 *   regions.
	 *
	 * - Soft interrupt processing context which cannot happen
	 *   while in a fpregs_lock()'ed critical region.
	 */
	if (!in_hardirq())
		return true;

	/*
	 * In hard interrupt context it's safe when soft interrupts
	 * are enabled, which means the interrupt did not hit in
	 * a fpregs_lock()'ed critical region.
	 */
	return !softirq_count();
}
EXPORT_SYMBOL(irq_fpu_usable);

/*
 * Save the FPU register state in fpu->state. The register state is
 * preserved.
 *
 * Must be called with fpregs_lock() held.
 *
 * The legacy FNSAVE instruction clears all FPU state unconditionally, so
 * register state has to be reloaded. That might be a pointless exercise
 * when the FPU is going to be used by another task right after that. But
 * this only affects 20+ years old 32bit systems and avoids conditionals all
 * over the place.
 *
 * FXSAVE and all XSAVE variants preserve the FPU register state.
 */
void save_fpregs_to_fpstate(struct fpu *fpu)
{
	if (likely(use_xsave())) {
		os_xsave(&fpu->state.xsave);

		/*
		 * AVX512 state is tracked here because its use is
		 * known to slow the max clock speed of the core.
		 */
		if (fpu->state.xsave.header.xfeatures & XFEATURE_MASK_AVX512)
			fpu->avx512_timestamp = jiffies;
		return;
	}

	if (likely(use_fxsr())) {
		fxsave(&fpu->state.fxsave);
		return;
	}

	/*
	 * Legacy FPU register saving, FNSAVE always clears FPU registers,
	 * so we have to reload them from the memory state.
	 */
	asm volatile("fnsave %[fp]; fwait" : [fp] "=m" (fpu->state.fsave));
	frstor(&fpu->state.fsave);
}
EXPORT_SYMBOL(save_fpregs_to_fpstate);

void __restore_fpregs_from_fpstate(union fpregs_state *fpstate, u64 mask)
{
	/*
	 * AMD K7/K8 and later CPUs up to Zen don't save/restore
	 * FDP/FIP/FOP unless an exception is pending. Clear the x87 state
	 * here by setting it to fixed values.  "m" is a random variable
	 * that should be in L1.
	 */
	if (unlikely(static_cpu_has_bug(X86_BUG_FXSAVE_LEAK))) {
		asm volatile(
			"fnclex\n\t"
			"emms\n\t"
			"fildl %P[addr]"	/* set F?P to defined value */
			: : [addr] "m" (fpstate));
	}

	if (use_xsave()) {
		os_xrstor(&fpstate->xsave, mask);
	} else {
		if (use_fxsr())
			fxrstor(&fpstate->fxsave);
		else
			frstor(&fpstate->fsave);
	}
}
EXPORT_SYMBOL_GPL(__restore_fpregs_from_fpstate);

void kernel_fpu_begin_mask(unsigned int kfpu_mask)
{
	preempt_disable();

	WARN_ON_FPU(!irq_fpu_usable());
	WARN_ON_FPU(this_cpu_read(in_kernel_fpu));

	this_cpu_write(in_kernel_fpu, true);

	if (!(current->flags & PF_KTHREAD) &&
	    !test_thread_flag(TIF_NEED_FPU_LOAD)) {
		set_thread_flag(TIF_NEED_FPU_LOAD);
		save_fpregs_to_fpstate(&current->thread.fpu);
	}
	__cpu_invalidate_fpregs_state();

	/* Put sane initial values into the control registers. */
	if (likely(kfpu_mask & KFPU_MXCSR) && boot_cpu_has(X86_FEATURE_XMM))
		ldmxcsr(MXCSR_DEFAULT);

	if (unlikely(kfpu_mask & KFPU_387) && boot_cpu_has(X86_FEATURE_FPU))
		asm volatile ("fninit");
}
EXPORT_SYMBOL_GPL(kernel_fpu_begin_mask);

void kernel_fpu_end(void)
{
	WARN_ON_FPU(!this_cpu_read(in_kernel_fpu));

	this_cpu_write(in_kernel_fpu, false);
>>>>>>> upstream/android-13
	preempt_enable();
}
EXPORT_SYMBOL_GPL(kernel_fpu_end);

/*
<<<<<<< HEAD
 * Save the FPU state (mark it for reload if necessary):
 *
 * This only ever gets called for the current task.
 */
void fpu__save(struct fpu *fpu)
{
	WARN_ON_FPU(fpu != &current->thread.fpu);

	preempt_disable();
	trace_x86_fpu_before_save(fpu);
	if (fpu->initialized) {
		if (!copy_fpregs_to_fpstate(fpu)) {
			copy_kernel_to_fpregs(&fpu->state);
		}
	}
	trace_x86_fpu_after_save(fpu);
	preempt_enable();
}
EXPORT_SYMBOL_GPL(fpu__save);
=======
 * Sync the FPU register state to current's memory register state when the
 * current task owns the FPU. The hardware register state is preserved.
 */
void fpu_sync_fpstate(struct fpu *fpu)
{
	WARN_ON_FPU(fpu != &current->thread.fpu);

	fpregs_lock();
	trace_x86_fpu_before_save(fpu);

	if (!test_thread_flag(TIF_NEED_FPU_LOAD))
		save_fpregs_to_fpstate(fpu);

	trace_x86_fpu_after_save(fpu);
	fpregs_unlock();
}

static inline void fpstate_init_xstate(struct xregs_state *xsave)
{
	/*
	 * XRSTORS requires these bits set in xcomp_bv, or it will
	 * trigger #GP:
	 */
	xsave->header.xcomp_bv = XCOMP_BV_COMPACTED_FORMAT | xfeatures_mask_all;
}

static inline void fpstate_init_fxstate(struct fxregs_state *fx)
{
	fx->cwd = 0x37f;
	fx->mxcsr = MXCSR_DEFAULT;
}
>>>>>>> upstream/android-13

/*
 * Legacy x87 fpstate state init:
 */
static inline void fpstate_init_fstate(struct fregs_state *fp)
{
	fp->cwd = 0xffff037fu;
	fp->swd = 0xffff0000u;
	fp->twd = 0xffffffffu;
	fp->fos = 0xffff0000u;
}

void fpstate_init(union fpregs_state *state)
{
	if (!static_cpu_has(X86_FEATURE_FPU)) {
		fpstate_init_soft(&state->soft);
		return;
	}

	memset(state, 0, fpu_kernel_xstate_size);

	if (static_cpu_has(X86_FEATURE_XSAVES))
		fpstate_init_xstate(&state->xsave);
	if (static_cpu_has(X86_FEATURE_FXSR))
		fpstate_init_fxstate(&state->fxsave);
	else
		fpstate_init_fstate(&state->fsave);
}
EXPORT_SYMBOL_GPL(fpstate_init);

<<<<<<< HEAD
int fpu__copy(struct fpu *dst_fpu, struct fpu *src_fpu)
{
	dst_fpu->last_cpu = -1;

	if (!src_fpu->initialized || !static_cpu_has(X86_FEATURE_FPU))
		return 0;

	WARN_ON_FPU(src_fpu != &current->thread.fpu);

=======
/* Clone current's FPU state on fork */
int fpu_clone(struct task_struct *dst)
{
	struct fpu *src_fpu = &current->thread.fpu;
	struct fpu *dst_fpu = &dst->thread.fpu;

	/* The new task's FPU state cannot be valid in the hardware. */
	dst_fpu->last_cpu = -1;

	if (!cpu_feature_enabled(X86_FEATURE_FPU))
		return 0;

>>>>>>> upstream/android-13
	/*
	 * Don't let 'init optimized' areas of the XSAVE area
	 * leak into the child task:
	 */
	memset(&dst_fpu->state.xsave, 0, fpu_kernel_xstate_size);

	/*
<<<<<<< HEAD
	 * Save current FPU registers directly into the child
	 * FPU context, without any memory-to-memory copying.
	 *
	 * ( The function 'fails' in the FNSAVE case, which destroys
	 *   register contents so we have to copy them back. )
	 */
	if (!copy_fpregs_to_fpstate(dst_fpu)) {
		memcpy(&src_fpu->state, &dst_fpu->state, fpu_kernel_xstate_size);
		copy_kernel_to_fpregs(&src_fpu->state);
	}
=======
	 * If the FPU registers are not owned by current just memcpy() the
	 * state.  Otherwise save the FPU registers directly into the
	 * child's FPU context, without any memory-to-memory copying.
	 */
	fpregs_lock();
	if (test_thread_flag(TIF_NEED_FPU_LOAD))
		memcpy(&dst_fpu->state, &src_fpu->state, fpu_kernel_xstate_size);

	else
		save_fpregs_to_fpstate(dst_fpu);
	fpregs_unlock();

	set_tsk_thread_flag(dst, TIF_NEED_FPU_LOAD);
>>>>>>> upstream/android-13

	trace_x86_fpu_copy_src(src_fpu);
	trace_x86_fpu_copy_dst(dst_fpu);

	return 0;
}

/*
<<<<<<< HEAD
 * Activate the current task's in-memory FPU context,
 * if it has not been used before:
 */
void fpu__initialize(struct fpu *fpu)
{
	WARN_ON_FPU(fpu != &current->thread.fpu);

	if (!fpu->initialized) {
		fpstate_init(&fpu->state);
		trace_x86_fpu_init_state(fpu);

		trace_x86_fpu_activate_state(fpu);
		/* Safe to do for the current task: */
		fpu->initialized = 1;
	}
}
EXPORT_SYMBOL_GPL(fpu__initialize);

/*
 * This function must be called before we read a task's fpstate.
 *
 * There's two cases where this gets called:
 *
 * - for the current task (when coredumping), in which case we have
 *   to save the latest FPU registers into the fpstate,
 *
 * - or it's called for stopped tasks (ptrace), in which case the
 *   registers were already saved by the context-switch code when
 *   the task scheduled out - we only have to initialize the registers
 *   if they've never been initialized.
 *
 * If the task has used the FPU before then save it.
 */
void fpu__prepare_read(struct fpu *fpu)
{
	if (fpu == &current->thread.fpu) {
		fpu__save(fpu);
	} else {
		if (!fpu->initialized) {
			fpstate_init(&fpu->state);
			trace_x86_fpu_init_state(fpu);

			trace_x86_fpu_activate_state(fpu);
			/* Safe to do for current and for stopped child tasks: */
			fpu->initialized = 1;
		}
	}
}

/*
 * This function must be called before we write a task's fpstate.
 *
 * If the task has used the FPU before then invalidate any cached FPU registers.
 * If the task has not used the FPU before then initialize its fpstate.
 *
 * After this function call, after registers in the fpstate are
 * modified and the child task has woken up, the child task will
 * restore the modified FPU state from the modified context. If we
 * didn't clear its cached status here then the cached in-registers
 * state pending on its former CPU could be restored, corrupting
 * the modifications.
 */
void fpu__prepare_write(struct fpu *fpu)
{
	/*
	 * Only stopped child tasks can be used to modify the FPU
	 * state in the fpstate buffer:
	 */
	WARN_ON_FPU(fpu == &current->thread.fpu);

	if (fpu->initialized) {
		/* Invalidate any cached state: */
		__fpu_invalidate_fpregs_state(fpu);
	} else {
		fpstate_init(&fpu->state);
		trace_x86_fpu_init_state(fpu);

		trace_x86_fpu_activate_state(fpu);
		/* Safe to do for stopped child tasks: */
		fpu->initialized = 1;
	}
}

/*
 * 'fpu__restore()' is called to copy FPU registers from
 * the FPU fpstate to the live hw registers and to activate
 * access to the hardware registers, so that FPU instructions
 * can be used afterwards.
 *
 * Must be called with kernel preemption disabled (for example
 * with local interrupts disabled, as it is in the case of
 * do_device_not_available()).
 */
void fpu__restore(struct fpu *fpu)
{
	fpu__initialize(fpu);

	/* Avoid __kernel_fpu_begin() right after fpregs_activate() */
	kernel_fpu_disable();
	trace_x86_fpu_before_restore(fpu);
	fpregs_activate(fpu);
	copy_kernel_to_fpregs(&fpu->state);
	trace_x86_fpu_after_restore(fpu);
	kernel_fpu_enable();
}
EXPORT_SYMBOL_GPL(fpu__restore);

/*
=======
>>>>>>> upstream/android-13
 * Drops current FPU state: deactivates the fpregs and
 * the fpstate. NOTE: it still leaves previous contents
 * in the fpregs in the eager-FPU case.
 *
 * This function can be used in cases where we know that
 * a state-restore is coming: either an explicit one,
 * or a reschedule.
 */
void fpu__drop(struct fpu *fpu)
{
	preempt_disable();

	if (fpu == &current->thread.fpu) {
<<<<<<< HEAD
		if (fpu->initialized) {
			/* Ignore delayed exceptions from user space */
			asm volatile("1: fwait\n"
				     "2:\n"
				     _ASM_EXTABLE(1b, 2b));
			fpregs_deactivate(fpu);
		}
	}

	fpu->initialized = 0;

=======
		/* Ignore delayed exceptions from user space */
		asm volatile("1: fwait\n"
			     "2:\n"
			     _ASM_EXTABLE(1b, 2b));
		fpregs_deactivate(fpu);
	}

>>>>>>> upstream/android-13
	trace_x86_fpu_dropped(fpu);

	preempt_enable();
}

/*
<<<<<<< HEAD
 * Clear FPU registers by setting them up from
 * the init fpstate:
 */
static inline void copy_init_fpstate_to_fpregs(void)
{
	if (use_xsave())
		copy_kernel_to_xregs(&init_fpstate.xsave, -1);
	else if (static_cpu_has(X86_FEATURE_FXSR))
		copy_kernel_to_fxregs(&init_fpstate.fxsave);
	else
		copy_kernel_to_fregs(&init_fpstate.fsave);

	if (boot_cpu_has(X86_FEATURE_OSPKE))
		copy_init_pkru_to_fpregs();
}

/*
 * Clear the FPU state back to init state.
 *
 * Called by sys_execve(), by the signal handler code and by various
 * error paths.
 */
void fpu__clear(struct fpu *fpu)
{
	WARN_ON_FPU(fpu != &current->thread.fpu); /* Almost certainly an anomaly */

	fpu__drop(fpu);

	/*
	 * Make sure fpstate is cleared and initialized.
	 */
	if (static_cpu_has(X86_FEATURE_FPU)) {
		preempt_disable();
		fpu__initialize(fpu);
		user_fpu_begin();
		copy_init_fpstate_to_fpregs();
		preempt_enable();
	}
}

=======
 * Clear FPU registers by setting them up from the init fpstate.
 * Caller must do fpregs_[un]lock() around it.
 */
static inline void restore_fpregs_from_init_fpstate(u64 features_mask)
{
	if (use_xsave())
		os_xrstor(&init_fpstate.xsave, features_mask);
	else if (use_fxsr())
		fxrstor(&init_fpstate.fxsave);
	else
		frstor(&init_fpstate.fsave);

	pkru_write_default();
}

static inline unsigned int init_fpstate_copy_size(void)
{
	if (!use_xsave())
		return fpu_kernel_xstate_size;

	/* XSAVE(S) just needs the legacy and the xstate header part */
	return sizeof(init_fpstate.xsave);
}

/*
 * Reset current->fpu memory state to the init values.
 */
static void fpu_reset_fpstate(void)
{
	struct fpu *fpu = &current->thread.fpu;

	fpregs_lock();
	fpu__drop(fpu);
	/*
	 * This does not change the actual hardware registers. It just
	 * resets the memory image and sets TIF_NEED_FPU_LOAD so a
	 * subsequent return to usermode will reload the registers from the
	 * task's memory image.
	 *
	 * Do not use fpstate_init() here. Just copy init_fpstate which has
	 * the correct content already except for PKRU.
	 *
	 * PKRU handling does not rely on the xstate when restoring for
	 * user space as PKRU is eagerly written in switch_to() and
	 * flush_thread().
	 */
	memcpy(&fpu->state, &init_fpstate, init_fpstate_copy_size());
	set_thread_flag(TIF_NEED_FPU_LOAD);
	fpregs_unlock();
}

/*
 * Reset current's user FPU states to the init states.  current's
 * supervisor states, if any, are not modified by this function.  The
 * caller guarantees that the XSTATE header in memory is intact.
 */
void fpu__clear_user_states(struct fpu *fpu)
{
	WARN_ON_FPU(fpu != &current->thread.fpu);

	fpregs_lock();
	if (!cpu_feature_enabled(X86_FEATURE_FPU)) {
		fpu_reset_fpstate();
		fpregs_unlock();
		return;
	}

	/*
	 * Ensure that current's supervisor states are loaded into their
	 * corresponding registers.
	 */
	if (xfeatures_mask_supervisor() &&
	    !fpregs_state_valid(fpu, smp_processor_id())) {
		os_xrstor(&fpu->state.xsave, xfeatures_mask_supervisor());
	}

	/* Reset user states in registers. */
	restore_fpregs_from_init_fpstate(xfeatures_mask_restore_user());

	/*
	 * Now all FPU registers have their desired values.  Inform the FPU
	 * state machine that current's FPU registers are in the hardware
	 * registers. The memory image does not need to be updated because
	 * any operation relying on it has to save the registers first when
	 * current's FPU is marked active.
	 */
	fpregs_mark_activate();
	fpregs_unlock();
}

void fpu_flush_thread(void)
{
	fpu_reset_fpstate();
}
/*
 * Load FPU context before returning to userspace.
 */
void switch_fpu_return(void)
{
	if (!static_cpu_has(X86_FEATURE_FPU))
		return;

	fpregs_restore_userregs();
}
EXPORT_SYMBOL_GPL(switch_fpu_return);

#ifdef CONFIG_X86_DEBUG_FPU
/*
 * If current FPU state according to its tracking (loaded FPU context on this
 * CPU) is not valid then we must have TIF_NEED_FPU_LOAD set so the context is
 * loaded on return to userland.
 */
void fpregs_assert_state_consistent(void)
{
	struct fpu *fpu = &current->thread.fpu;

	if (test_thread_flag(TIF_NEED_FPU_LOAD))
		return;

	WARN_ON_FPU(!fpregs_state_valid(fpu, smp_processor_id()));
}
EXPORT_SYMBOL_GPL(fpregs_assert_state_consistent);
#endif

void fpregs_mark_activate(void)
{
	struct fpu *fpu = &current->thread.fpu;

	fpregs_activate(fpu);
	fpu->last_cpu = smp_processor_id();
	clear_thread_flag(TIF_NEED_FPU_LOAD);
}
EXPORT_SYMBOL_GPL(fpregs_mark_activate);

>>>>>>> upstream/android-13
/*
 * x87 math exception handling:
 */

int fpu__exception_code(struct fpu *fpu, int trap_nr)
{
	int err;

	if (trap_nr == X86_TRAP_MF) {
		unsigned short cwd, swd;
		/*
		 * (~cwd & swd) will mask out exceptions that are not set to unmasked
		 * status.  0x3f is the exception bits in these regs, 0x200 is the
		 * C1 reg you need in case of a stack fault, 0x040 is the stack
		 * fault bit.  We should only be taking one exception at a time,
		 * so if this combination doesn't produce any single exception,
		 * then we have a bad program that isn't synchronizing its FPU usage
		 * and it will suffer the consequences since we won't be able to
		 * fully reproduce the context of the exception.
		 */
		if (boot_cpu_has(X86_FEATURE_FXSR)) {
			cwd = fpu->state.fxsave.cwd;
			swd = fpu->state.fxsave.swd;
		} else {
			cwd = (unsigned short)fpu->state.fsave.cwd;
			swd = (unsigned short)fpu->state.fsave.swd;
		}

		err = swd & ~cwd;
	} else {
		/*
		 * The SIMD FPU exceptions are handled a little differently, as there
		 * is only a single status/control register.  Thus, to determine which
		 * unmasked exception was caught we must mask the exception mask bits
		 * at 0x1f80, and then use these to mask the exception bits at 0x3f.
		 */
		unsigned short mxcsr = MXCSR_DEFAULT;

		if (boot_cpu_has(X86_FEATURE_XMM))
			mxcsr = fpu->state.fxsave.mxcsr;

		err = ~(mxcsr >> 7) & mxcsr;
	}

	if (err & 0x001) {	/* Invalid op */
		/*
		 * swd & 0x240 == 0x040: Stack Underflow
		 * swd & 0x240 == 0x240: Stack Overflow
		 * User must clear the SF bit (0x40) if set
		 */
		return FPE_FLTINV;
	} else if (err & 0x004) { /* Divide by Zero */
		return FPE_FLTDIV;
	} else if (err & 0x008) { /* Overflow */
		return FPE_FLTOVF;
	} else if (err & 0x012) { /* Denormal, Underflow */
		return FPE_FLTUND;
	} else if (err & 0x020) { /* Precision */
		return FPE_FLTRES;
	}

	/*
	 * If we're using IRQ 13, or supposedly even some trap
	 * X86_TRAP_MF implementations, it's possible
	 * we get a spurious trap, which is not an error.
	 */
	return 0;
}
