// SPDX-License-Identifier: GPL-2.0
/*
 * FPU signal frame handling routines.
 */

#include <linux/compat.h>
#include <linux/cpu.h>
<<<<<<< HEAD
=======
#include <linux/pagemap.h>
>>>>>>> upstream/android-13

#include <asm/fpu/internal.h>
#include <asm/fpu/signal.h>
#include <asm/fpu/regset.h>
#include <asm/fpu/xstate.h>

#include <asm/sigframe.h>
#include <asm/trace/fpu.h>

<<<<<<< HEAD
static struct _fpx_sw_bytes fx_sw_reserved, fx_sw_reserved_ia32;
=======
static struct _fpx_sw_bytes fx_sw_reserved __ro_after_init;
static struct _fpx_sw_bytes fx_sw_reserved_ia32 __ro_after_init;
>>>>>>> upstream/android-13

/*
 * Check for the presence of extended state information in the
 * user fpstate pointer in the sigcontext.
 */
<<<<<<< HEAD
static inline int check_for_xstate(struct fxregs_state __user *buf,
				   void __user *fpstate,
				   struct _fpx_sw_bytes *fx_sw)
{
	int min_xstate_size = sizeof(struct fxregs_state) +
			      sizeof(struct xstate_header);
	unsigned int magic2;

	if (__copy_from_user(fx_sw, &buf->sw_reserved[0], sizeof(*fx_sw)))
		return -1;
=======
static inline int check_xstate_in_sigframe(struct fxregs_state __user *fxbuf,
					   struct _fpx_sw_bytes *fx_sw)
{
	int min_xstate_size = sizeof(struct fxregs_state) +
			      sizeof(struct xstate_header);
	void __user *fpstate = fxbuf;
	unsigned int magic2;

	if (__copy_from_user(fx_sw, &fxbuf->sw_reserved[0], sizeof(*fx_sw)))
		return -EFAULT;
>>>>>>> upstream/android-13

	/* Check for the first magic field and other error scenarios. */
	if (fx_sw->magic1 != FP_XSTATE_MAGIC1 ||
	    fx_sw->xstate_size < min_xstate_size ||
	    fx_sw->xstate_size > fpu_user_xstate_size ||
	    fx_sw->xstate_size > fx_sw->extended_size)
<<<<<<< HEAD
		return -1;
=======
		goto setfx;
>>>>>>> upstream/android-13

	/*
	 * Check for the presence of second magic word at the end of memory
	 * layout. This detects the case where the user just copied the legacy
	 * fpstate layout with out copying the extended state information
	 * in the memory layout.
	 */
<<<<<<< HEAD
	if (__get_user(magic2, (__u32 __user *)(fpstate + fx_sw->xstate_size))
	    || magic2 != FP_XSTATE_MAGIC2)
		return -1;

=======
	if (__get_user(magic2, (__u32 __user *)(fpstate + fx_sw->xstate_size)))
		return -EFAULT;

	if (likely(magic2 == FP_XSTATE_MAGIC2))
		return 0;
setfx:
	trace_x86_fpu_xstate_check_failed(&current->thread.fpu);

	/* Set the parameters for fx only state */
	fx_sw->magic1 = 0;
	fx_sw->xstate_size = sizeof(struct fxregs_state);
	fx_sw->xfeatures = XFEATURE_MASK_FPSSE;
>>>>>>> upstream/android-13
	return 0;
}

/*
 * Signal frame handlers.
 */
static inline int save_fsave_header(struct task_struct *tsk, void __user *buf)
{
	if (use_fxsr()) {
		struct xregs_state *xsave = &tsk->thread.fpu.state.xsave;
		struct user_i387_ia32_struct env;
		struct _fpstate_32 __user *fp = buf;

<<<<<<< HEAD
=======
		fpregs_lock();
		if (!test_thread_flag(TIF_NEED_FPU_LOAD))
			fxsave(&tsk->thread.fpu.state.fxsave);
		fpregs_unlock();

>>>>>>> upstream/android-13
		convert_from_fxsr(&env, tsk);

		if (__copy_to_user(buf, &env, sizeof(env)) ||
		    __put_user(xsave->i387.swd, &fp->status) ||
		    __put_user(X86_FXSR_MAGIC, &fp->magic))
			return -1;
	} else {
		struct fregs_state __user *fp = buf;
		u32 swd;
		if (__get_user(swd, &fp->swd) || __put_user(swd, &fp->status))
			return -1;
	}

	return 0;
}

static inline int save_xstate_epilog(void __user *buf, int ia32_frame)
{
	struct xregs_state __user *x = buf;
	struct _fpx_sw_bytes *sw_bytes;
	u32 xfeatures;
	int err;

	/* Setup the bytes not touched by the [f]xsave and reserved for SW. */
	sw_bytes = ia32_frame ? &fx_sw_reserved_ia32 : &fx_sw_reserved;
	err = __copy_to_user(&x->i387.sw_reserved, sw_bytes, sizeof(*sw_bytes));

	if (!use_xsave())
		return err;

	err |= __put_user(FP_XSTATE_MAGIC2,
<<<<<<< HEAD
			  (__u32 *)(buf + fpu_user_xstate_size));
=======
			  (__u32 __user *)(buf + fpu_user_xstate_size));
>>>>>>> upstream/android-13

	/*
	 * Read the xfeatures which we copied (directly from the cpu or
	 * from the state in task struct) to the user buffers.
	 */
<<<<<<< HEAD
	err |= __get_user(xfeatures, (__u32 *)&x->header.xfeatures);
=======
	err |= __get_user(xfeatures, (__u32 __user *)&x->header.xfeatures);
>>>>>>> upstream/android-13

	/*
	 * For legacy compatible, we always set FP/SSE bits in the bit
	 * vector while saving the state to the user context. This will
	 * enable us capturing any changes(during sigreturn) to
	 * the FP/SSE bits by the legacy applications which don't touch
	 * xfeatures in the xsave header.
	 *
	 * xsave aware apps can change the xfeatures in the xsave
	 * header as well as change any contents in the memory layout.
	 * xrestore as part of sigreturn will capture all the changes.
	 */
	xfeatures |= XFEATURE_MASK_FPSSE;

<<<<<<< HEAD
	err |= __put_user(xfeatures, (__u32 *)&x->header.xfeatures);
=======
	err |= __put_user(xfeatures, (__u32 __user *)&x->header.xfeatures);
>>>>>>> upstream/android-13

	return err;
}

static inline int copy_fpregs_to_sigframe(struct xregs_state __user *buf)
{
	int err;

	if (use_xsave())
<<<<<<< HEAD
		err = copy_xregs_to_user(buf);
	else if (use_fxsr())
		err = copy_fxregs_to_user((struct fxregs_state __user *) buf);
	else
		err = copy_fregs_to_user((struct fregs_state __user *) buf);
=======
		err = xsave_to_user_sigframe(buf);
	else if (use_fxsr())
		err = fxsave_to_user_sigframe((struct fxregs_state __user *) buf);
	else
		err = fnsave_to_user_sigframe((struct fregs_state __user *) buf);
>>>>>>> upstream/android-13

	if (unlikely(err) && __clear_user(buf, fpu_user_xstate_size))
		err = -EFAULT;
	return err;
}

/*
 * Save the fpu, extended register state to the user signal frame.
 *
 * 'buf_fx' is the 64-byte aligned pointer at which the [f|fx|x]save
 *  state is copied.
 *  'buf' points to the 'buf_fx' or to the fsave header followed by 'buf_fx'.
 *
 *	buf == buf_fx for 64-bit frames and 32-bit fsave frame.
 *	buf != buf_fx for 32-bit frames with fxstate.
 *
<<<<<<< HEAD
 * If the fpu, extended register state is live, save the state directly
 * to the user frame pointed by the aligned pointer 'buf_fx'. Otherwise,
 * copy the thread's fpu state to the user frame starting at 'buf_fx'.
=======
 * Try to save it directly to the user frame with disabled page fault handler.
 * If this fails then do the slow path where the FPU state is first saved to
 * task's fpu->state and then copy it to the user frame pointed to by the
 * aligned pointer 'buf_fx'.
>>>>>>> upstream/android-13
 *
 * If this is a 32-bit frame with fxstate, put a fsave header before
 * the aligned state at 'buf_fx'.
 *
 * For [f]xsave state, update the SW reserved fields in the [f]xsave frame
 * indicating the absence/presence of the extended state to the user.
 */
int copy_fpstate_to_sigframe(void __user *buf, void __user *buf_fx, int size)
{
<<<<<<< HEAD
	struct fpu *fpu = &current->thread.fpu;
	struct xregs_state *xsave = &fpu->state.xsave;
	struct task_struct *tsk = current;
	int ia32_fxstate = (buf != buf_fx);
=======
	struct task_struct *tsk = current;
	int ia32_fxstate = (buf != buf_fx);
	int ret;
>>>>>>> upstream/android-13

	ia32_fxstate &= (IS_ENABLED(CONFIG_X86_32) ||
			 IS_ENABLED(CONFIG_IA32_EMULATION));

<<<<<<< HEAD
	if (!access_ok(VERIFY_WRITE, buf, size))
		return -EACCES;

	if (!static_cpu_has(X86_FEATURE_FPU))
		return fpregs_soft_get(current, NULL, 0,
			sizeof(struct user_i387_ia32_struct), NULL,
			(struct _fpstate_32 __user *) buf) ? -1 : 1;

	if (fpu->initialized || using_compacted_format()) {
		/* Save the live register state to the user directly. */
		if (copy_fpregs_to_sigframe(buf_fx))
			return -1;
		/* Update the thread's fxstate to save the fsave header. */
		if (ia32_fxstate)
			copy_fxregs_to_kernel(fpu);
	} else {
		/*
		 * It is a *bug* if kernel uses compacted-format for xsave
		 * area and we copy it out directly to a signal frame. It
		 * should have been handled above by saving the registers
		 * directly.
		 */
		if (boot_cpu_has(X86_FEATURE_XSAVES)) {
			WARN_ONCE(1, "x86/fpu: saving compacted-format xsave area to a signal frame!\n");
			return -1;
		}

		fpstate_sanitize_xstate(fpu);
		if (__copy_to_user(buf_fx, xsave, fpu_user_xstate_size))
			return -1;
=======
	if (!static_cpu_has(X86_FEATURE_FPU)) {
		struct user_i387_ia32_struct fp;
		fpregs_soft_get(current, NULL, (struct membuf){.p = &fp,
						.left = sizeof(fp)});
		return copy_to_user(buf, &fp, sizeof(fp)) ? -EFAULT : 0;
	}

	if (!access_ok(buf, size))
		return -EACCES;
retry:
	/*
	 * Load the FPU registers if they are not valid for the current task.
	 * With a valid FPU state we can attempt to save the state directly to
	 * userland's stack frame which will likely succeed. If it does not,
	 * resolve the fault in the user memory and try again.
	 */
	fpregs_lock();
	if (test_thread_flag(TIF_NEED_FPU_LOAD))
		fpregs_restore_userregs();

	pagefault_disable();
	ret = copy_fpregs_to_sigframe(buf_fx);
	pagefault_enable();
	fpregs_unlock();

	if (ret) {
		if (!fault_in_writeable(buf_fx, fpu_user_xstate_size))
			goto retry;
		return -EFAULT;
>>>>>>> upstream/android-13
	}

	/* Save the fsave header for the 32-bit frames. */
	if ((ia32_fxstate || !use_fxsr()) && save_fsave_header(tsk, buf))
		return -1;

	if (use_fxsr() && save_xstate_epilog(buf_fx, ia32_fxstate))
		return -1;

	return 0;
}

<<<<<<< HEAD
static inline void
sanitize_restored_xstate(struct task_struct *tsk,
			 struct user_i387_ia32_struct *ia32_env,
			 u64 xfeatures, int fx_only)
{
	struct xregs_state *xsave = &tsk->thread.fpu.state.xsave;
	struct xstate_header *header = &xsave->header;

	if (use_xsave()) {
		/*
		 * Note: we don't need to zero the reserved bits in the
		 * xstate_header here because we either didn't copy them at all,
		 * or we checked earlier that they aren't set.
		 */

		/*
		 * Init the state that is not present in the memory
		 * layout and not enabled by the OS.
		 */
		if (fx_only)
			header->xfeatures = XFEATURE_MASK_FPSSE;
		else
			header->xfeatures &= xfeatures;
	}

	if (use_fxsr()) {
		/*
		 * mscsr reserved bits must be masked to zero for security
		 * reasons.
		 */
		xsave->i387.mxcsr &= mxcsr_feature_mask;

		convert_to_fxsr(tsk, ia32_env);
=======
static int __restore_fpregs_from_user(void __user *buf, u64 xrestore,
				      bool fx_only)
{
	if (use_xsave()) {
		u64 init_bv = xfeatures_mask_uabi() & ~xrestore;
		int ret;

		if (likely(!fx_only))
			ret = xrstor_from_user_sigframe(buf, xrestore);
		else
			ret = fxrstor_from_user_sigframe(buf);

		if (!ret && unlikely(init_bv))
			os_xrstor(&init_fpstate.xsave, init_bv);
		return ret;
	} else if (use_fxsr()) {
		return fxrstor_from_user_sigframe(buf);
	} else {
		return frstor_from_user_sigframe(buf);
>>>>>>> upstream/android-13
	}
}

/*
<<<<<<< HEAD
 * Restore the extended state if present. Otherwise, restore the FP/SSE state.
 */
static inline int copy_user_to_fpregs_zeroing(void __user *buf, u64 xbv, int fx_only)
{
	if (use_xsave()) {
		if ((unsigned long)buf % 64 || fx_only) {
			u64 init_bv = xfeatures_mask & ~XFEATURE_MASK_FPSSE;
			copy_kernel_to_xregs(&init_fpstate.xsave, init_bv);
			return copy_user_to_fxregs(buf);
		} else {
			u64 init_bv = xfeatures_mask & ~xbv;
			if (unlikely(init_bv))
				copy_kernel_to_xregs(&init_fpstate.xsave, init_bv);
			return copy_user_to_xregs(buf, xbv);
		}
	} else if (use_fxsr()) {
		return copy_user_to_fxregs(buf);
	} else
		return copy_user_to_fregs(buf);
}

static int __fpu__restore_sig(void __user *buf, void __user *buf_fx, int size)
{
	int ia32_fxstate = (buf != buf_fx);
	struct task_struct *tsk = current;
	struct fpu *fpu = &tsk->thread.fpu;
	int state_size = fpu_kernel_xstate_size;
	u64 xfeatures = 0;
	int fx_only = 0;

	ia32_fxstate &= (IS_ENABLED(CONFIG_X86_32) ||
			 IS_ENABLED(CONFIG_IA32_EMULATION));

	if (!buf) {
		fpu__clear(fpu);
		return 0;
	}

	if (!access_ok(VERIFY_READ, buf, size))
		return -EACCES;

	fpu__initialize(fpu);

	if (!static_cpu_has(X86_FEATURE_FPU))
		return fpregs_soft_set(current, NULL,
				       0, sizeof(struct user_i387_ia32_struct),
				       NULL, buf) != 0;

	if (use_xsave()) {
		struct _fpx_sw_bytes fx_sw_user;
		if (unlikely(check_for_xstate(buf_fx, buf_fx, &fx_sw_user))) {
			/*
			 * Couldn't find the extended state information in the
			 * memory layout. Restore just the FP/SSE and init all
			 * the other extended state.
			 */
			state_size = sizeof(struct fxregs_state);
			fx_only = 1;
			trace_x86_fpu_xstate_check_failed(fpu);
		} else {
			state_size = fx_sw_user.xstate_size;
			xfeatures = fx_sw_user.xfeatures;
		}
	}

	if (ia32_fxstate) {
		/*
		 * For 32-bit frames with fxstate, copy the user state to the
		 * thread's fpu state, reconstruct fxstate from the fsave
		 * header. Validate and sanitize the copied state.
		 */
		struct user_i387_ia32_struct env;
		int err = 0;

		/*
		 * Drop the current fpu which clears fpu->initialized. This ensures
		 * that any context-switch during the copy of the new state,
		 * avoids the intermediate state from getting restored/saved.
		 * Thus avoiding the new restored state from getting corrupted.
		 * We will be ready to restore/save the state only after
		 * fpu->initialized is again set.
		 */
		fpu__drop(fpu);

		if (using_compacted_format()) {
			err = copy_user_to_xstate(&fpu->state.xsave, buf_fx);
		} else {
			err = __copy_from_user(&fpu->state.xsave, buf_fx, state_size);

			if (!err && state_size > offsetof(struct xregs_state, header))
				err = validate_xstate_header(&fpu->state.xsave.header);
		}

		if (err || __copy_from_user(&env, buf, sizeof(env))) {
			fpstate_init(&fpu->state);
			trace_x86_fpu_init_state(fpu);
			err = -1;
		} else {
			sanitize_restored_xstate(tsk, &env, xfeatures, fx_only);
		}

		local_bh_disable();
		fpu->initialized = 1;
		fpu__restore(fpu);
		local_bh_enable();

		return err;
	} else {
		/*
		 * For 64-bit frames and 32-bit fsave frames, restore the user
		 * state to the registers directly (with exceptions handled).
		 */
		user_fpu_begin();
		if (copy_user_to_fpregs_zeroing(buf_fx, xfeatures, fx_only)) {
			fpu__clear(fpu);
			return -1;
		}
	}

	return 0;
}

=======
 * Attempt to restore the FPU registers directly from user memory.
 * Pagefaults are handled and any errors returned are fatal.
 */
static int restore_fpregs_from_user(void __user *buf, u64 xrestore,
				    bool fx_only, unsigned int size)
{
	struct fpu *fpu = &current->thread.fpu;
	int ret;

retry:
	fpregs_lock();
	pagefault_disable();
	ret = __restore_fpregs_from_user(buf, xrestore, fx_only);
	pagefault_enable();

	if (unlikely(ret)) {
		/*
		 * The above did an FPU restore operation, restricted to
		 * the user portion of the registers, and failed, but the
		 * microcode might have modified the FPU registers
		 * nevertheless.
		 *
		 * If the FPU registers do not belong to current, then
		 * invalidate the FPU register state otherwise the task
		 * might preempt current and return to user space with
		 * corrupted FPU registers.
		 */
		if (test_thread_flag(TIF_NEED_FPU_LOAD))
			__cpu_invalidate_fpregs_state();
		fpregs_unlock();

		/* Try to handle #PF, but anything else is fatal. */
		if (ret != -EFAULT)
			return -EINVAL;

		if (!fault_in_readable(buf, size))
			goto retry;
		return -EFAULT;
	}

	/*
	 * Restore supervisor states: previous context switch etc has done
	 * XSAVES and saved the supervisor states in the kernel buffer from
	 * which they can be restored now.
	 *
	 * It would be optimal to handle this with a single XRSTORS, but
	 * this does not work because the rest of the FPU registers have
	 * been restored from a user buffer directly.
	 */
	if (test_thread_flag(TIF_NEED_FPU_LOAD) && xfeatures_mask_supervisor())
		os_xrstor(&fpu->state.xsave, xfeatures_mask_supervisor());

	fpregs_mark_activate();
	fpregs_unlock();
	return 0;
}

static int __fpu_restore_sig(void __user *buf, void __user *buf_fx,
			     bool ia32_fxstate)
{
	int state_size = fpu_kernel_xstate_size;
	struct task_struct *tsk = current;
	struct fpu *fpu = &tsk->thread.fpu;
	struct user_i387_ia32_struct env;
	u64 user_xfeatures = 0;
	bool fx_only = false;
	int ret;

	if (use_xsave()) {
		struct _fpx_sw_bytes fx_sw_user;

		ret = check_xstate_in_sigframe(buf_fx, &fx_sw_user);
		if (unlikely(ret))
			return ret;

		fx_only = !fx_sw_user.magic1;
		state_size = fx_sw_user.xstate_size;
		user_xfeatures = fx_sw_user.xfeatures;
	} else {
		user_xfeatures = XFEATURE_MASK_FPSSE;
	}

	if (likely(!ia32_fxstate)) {
		/*
		 * Attempt to restore the FPU registers directly from user
		 * memory. For that to succeed, the user access cannot cause page
		 * faults. If it does, fall back to the slow path below, going
		 * through the kernel buffer with the enabled pagefault handler.
		 */
		return restore_fpregs_from_user(buf_fx, user_xfeatures, fx_only,
						state_size);
	}

	/*
	 * Copy the legacy state because the FP portion of the FX frame has
	 * to be ignored for histerical raisins. The legacy state is folded
	 * in once the larger state has been copied.
	 */
	ret = __copy_from_user(&env, buf, sizeof(env));
	if (ret)
		return ret;

	/*
	 * By setting TIF_NEED_FPU_LOAD it is ensured that our xstate is
	 * not modified on context switch and that the xstate is considered
	 * to be loaded again on return to userland (overriding last_cpu avoids
	 * the optimisation).
	 */
	fpregs_lock();
	if (!test_thread_flag(TIF_NEED_FPU_LOAD)) {
		/*
		 * If supervisor states are available then save the
		 * hardware state in current's fpstate so that the
		 * supervisor state is preserved. Save the full state for
		 * simplicity. There is no point in optimizing this by only
		 * saving the supervisor states and then shuffle them to
		 * the right place in memory. It's ia32 mode. Shrug.
		 */
		if (xfeatures_mask_supervisor())
			os_xsave(&fpu->state.xsave);
		set_thread_flag(TIF_NEED_FPU_LOAD);
	}
	__fpu_invalidate_fpregs_state(fpu);
	__cpu_invalidate_fpregs_state();
	fpregs_unlock();

	if (use_xsave() && !fx_only) {
		ret = copy_sigframe_from_user_to_xstate(&fpu->state.xsave, buf_fx);
		if (ret)
			return ret;
	} else {
		if (__copy_from_user(&fpu->state.fxsave, buf_fx,
				     sizeof(fpu->state.fxsave)))
			return -EFAULT;

		if (IS_ENABLED(CONFIG_X86_64)) {
			/* Reject invalid MXCSR values. */
			if (fpu->state.fxsave.mxcsr & ~mxcsr_feature_mask)
				return -EINVAL;
		} else {
			/* Mask invalid bits out for historical reasons (broken hardware). */
			fpu->state.fxsave.mxcsr &= mxcsr_feature_mask;
		}

		/* Enforce XFEATURE_MASK_FPSSE when XSAVE is enabled */
		if (use_xsave())
			fpu->state.xsave.header.xfeatures |= XFEATURE_MASK_FPSSE;
	}

	/* Fold the legacy FP storage */
	convert_to_fxsr(&fpu->state.fxsave, &env);

	fpregs_lock();
	if (use_xsave()) {
		/*
		 * Remove all UABI feature bits not set in user_xfeatures
		 * from the memory xstate header which makes the full
		 * restore below bring them into init state. This works for
		 * fx_only mode as well because that has only FP and SSE
		 * set in user_xfeatures.
		 *
		 * Preserve supervisor states!
		 */
		u64 mask = user_xfeatures | xfeatures_mask_supervisor();

		fpu->state.xsave.header.xfeatures &= mask;
		ret = os_xrstor_safe(&fpu->state.xsave, xfeatures_mask_all);
	} else {
		ret = fxrstor_safe(&fpu->state.fxsave);
	}

	if (likely(!ret))
		fpregs_mark_activate();

	fpregs_unlock();
	return ret;
}
>>>>>>> upstream/android-13
static inline int xstate_sigframe_size(void)
{
	return use_xsave() ? fpu_user_xstate_size + FP_XSTATE_MAGIC2_SIZE :
			fpu_user_xstate_size;
}

/*
 * Restore FPU state from a sigframe:
 */
int fpu__restore_sig(void __user *buf, int ia32_frame)
{
<<<<<<< HEAD
	void __user *buf_fx = buf;
	int size = xstate_sigframe_size();

	if (ia32_frame && use_fxsr()) {
		buf_fx = buf + sizeof(struct fregs_state);
		size += sizeof(struct fregs_state);
	}

	return __fpu__restore_sig(buf, buf_fx, size);
=======
	unsigned int size = xstate_sigframe_size();
	struct fpu *fpu = &current->thread.fpu;
	void __user *buf_fx = buf;
	bool ia32_fxstate = false;
	int ret;

	if (unlikely(!buf)) {
		fpu__clear_user_states(fpu);
		return 0;
	}

	ia32_frame &= (IS_ENABLED(CONFIG_X86_32) ||
		       IS_ENABLED(CONFIG_IA32_EMULATION));

	/*
	 * Only FXSR enabled systems need the FX state quirk.
	 * FRSTOR does not need it and can use the fast path.
	 */
	if (ia32_frame && use_fxsr()) {
		buf_fx = buf + sizeof(struct fregs_state);
		size += sizeof(struct fregs_state);
		ia32_fxstate = true;
	}

	if (!access_ok(buf, size)) {
		ret = -EACCES;
		goto out;
	}

	if (!IS_ENABLED(CONFIG_X86_64) && !cpu_feature_enabled(X86_FEATURE_FPU)) {
		ret = fpregs_soft_set(current, NULL, 0,
				      sizeof(struct user_i387_ia32_struct),
				      NULL, buf);
	} else {
		ret = __fpu_restore_sig(buf, buf_fx, ia32_fxstate);
	}

out:
	if (unlikely(ret))
		fpu__clear_user_states(fpu);
	return ret;
>>>>>>> upstream/android-13
}

unsigned long
fpu__alloc_mathframe(unsigned long sp, int ia32_frame,
		     unsigned long *buf_fx, unsigned long *size)
{
	unsigned long frame_size = xstate_sigframe_size();

	*buf_fx = sp = round_down(sp - frame_size, 64);
	if (ia32_frame && use_fxsr()) {
		frame_size += sizeof(struct fregs_state);
		sp -= sizeof(struct fregs_state);
	}

	*size = frame_size;

	return sp;
}
<<<<<<< HEAD
=======

unsigned long fpu__get_fpstate_size(void)
{
	unsigned long ret = xstate_sigframe_size();

	/*
	 * This space is needed on (most) 32-bit kernels, or when a 32-bit
	 * app is running on a 64-bit kernel. To keep things simple, just
	 * assume the worst case and always include space for 'freg_state',
	 * even for 64-bit apps on 64-bit kernels. This wastes a bit of
	 * space, but keeps the code simple.
	 */
	if ((IS_ENABLED(CONFIG_IA32_EMULATION) ||
	     IS_ENABLED(CONFIG_X86_32)) && use_fxsr())
		ret += sizeof(struct fregs_state);

	return ret;
}

>>>>>>> upstream/android-13
/*
 * Prepare the SW reserved portion of the fxsave memory layout, indicating
 * the presence of the extended state information in the memory layout
 * pointed by the fpstate pointer in the sigcontext.
 * This will be saved when ever the FP and extended state context is
 * saved on the user stack during the signal handler delivery to the user.
 */
void fpu__init_prepare_fx_sw_frame(void)
{
	int size = fpu_user_xstate_size + FP_XSTATE_MAGIC2_SIZE;

	fx_sw_reserved.magic1 = FP_XSTATE_MAGIC1;
	fx_sw_reserved.extended_size = size;
<<<<<<< HEAD
	fx_sw_reserved.xfeatures = xfeatures_mask;
=======
	fx_sw_reserved.xfeatures = xfeatures_mask_uabi();
>>>>>>> upstream/android-13
	fx_sw_reserved.xstate_size = fpu_user_xstate_size;

	if (IS_ENABLED(CONFIG_IA32_EMULATION) ||
	    IS_ENABLED(CONFIG_X86_32)) {
		int fsave_header_size = sizeof(struct fregs_state);

		fx_sw_reserved_ia32 = fx_sw_reserved;
		fx_sw_reserved_ia32.extended_size = size + fsave_header_size;
	}
}

