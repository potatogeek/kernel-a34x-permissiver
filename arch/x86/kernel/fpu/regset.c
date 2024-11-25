// SPDX-License-Identifier: GPL-2.0
/*
 * FPU register's regset abstraction, for ptrace, core dumps, etc.
 */
<<<<<<< HEAD
=======
#include <linux/sched/task_stack.h>
#include <linux/vmalloc.h>

>>>>>>> upstream/android-13
#include <asm/fpu/internal.h>
#include <asm/fpu/signal.h>
#include <asm/fpu/regset.h>
#include <asm/fpu/xstate.h>
<<<<<<< HEAD
#include <linux/sched/task_stack.h>
=======
>>>>>>> upstream/android-13

/*
 * The xstateregs_active() routine is the same as the regset_fpregs_active() routine,
 * as the "regset->n" for the xstate regset will be updated based on the feature
 * capabilities supported by the xsave.
 */
int regset_fpregs_active(struct task_struct *target, const struct user_regset *regset)
{
<<<<<<< HEAD
	struct fpu *target_fpu = &target->thread.fpu;

	return target_fpu->initialized ? regset->n : 0;
=======
	return regset->n;
>>>>>>> upstream/android-13
}

int regset_xregset_fpregs_active(struct task_struct *target, const struct user_regset *regset)
{
<<<<<<< HEAD
	struct fpu *target_fpu = &target->thread.fpu;

	if (boot_cpu_has(X86_FEATURE_FXSR) && target_fpu->initialized)
=======
	if (boot_cpu_has(X86_FEATURE_FXSR))
>>>>>>> upstream/android-13
		return regset->n;
	else
		return 0;
}

<<<<<<< HEAD
int xfpregs_get(struct task_struct *target, const struct user_regset *regset,
		unsigned int pos, unsigned int count,
		void *kbuf, void __user *ubuf)
{
	struct fpu *fpu = &target->thread.fpu;

	if (!boot_cpu_has(X86_FEATURE_FXSR))
		return -ENODEV;

	fpu__prepare_read(fpu);
	fpstate_sanitize_xstate(fpu);

	return user_regset_copyout(&pos, &count, &kbuf, &ubuf,
				   &fpu->state.fxsave, 0, -1);
=======
/*
 * The regset get() functions are invoked from:
 *
 *   - coredump to dump the current task's fpstate. If the current task
 *     owns the FPU then the memory state has to be synchronized and the
 *     FPU register state preserved. Otherwise fpstate is already in sync.
 *
 *   - ptrace to dump fpstate of a stopped task, in which case the registers
 *     have already been saved to fpstate on context switch.
 */
static void sync_fpstate(struct fpu *fpu)
{
	if (fpu == &current->thread.fpu)
		fpu_sync_fpstate(fpu);
}

/*
 * Invalidate cached FPU registers before modifying the stopped target
 * task's fpstate.
 *
 * This forces the target task on resume to restore the FPU registers from
 * modified fpstate. Otherwise the task might skip the restore and operate
 * with the cached FPU registers which discards the modifications.
 */
static void fpu_force_restore(struct fpu *fpu)
{
	/*
	 * Only stopped child tasks can be used to modify the FPU
	 * state in the fpstate buffer:
	 */
	WARN_ON_FPU(fpu == &current->thread.fpu);

	__fpu_invalidate_fpregs_state(fpu);
}

int xfpregs_get(struct task_struct *target, const struct user_regset *regset,
		struct membuf to)
{
	struct fpu *fpu = &target->thread.fpu;

	if (!cpu_feature_enabled(X86_FEATURE_FXSR))
		return -ENODEV;

	sync_fpstate(fpu);

	if (!use_xsave()) {
		return membuf_write(&to, &fpu->state.fxsave,
				    sizeof(fpu->state.fxsave));
	}

	copy_xstate_to_uabi_buf(to, target, XSTATE_COPY_FX);
	return 0;
>>>>>>> upstream/android-13
}

int xfpregs_set(struct task_struct *target, const struct user_regset *regset,
		unsigned int pos, unsigned int count,
		const void *kbuf, const void __user *ubuf)
{
	struct fpu *fpu = &target->thread.fpu;
<<<<<<< HEAD
	int ret;

	if (!boot_cpu_has(X86_FEATURE_FXSR))
		return -ENODEV;

	fpu__prepare_write(fpu);
	fpstate_sanitize_xstate(fpu);

	ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				 &fpu->state.fxsave, 0, -1);

	/*
	 * mxcsr reserved bits must be masked to zero for security reasons.
	 */
	fpu->state.fxsave.mxcsr &= mxcsr_feature_mask;

	/*
	 * update the header bits in the xsave header, indicating the
	 * presence of FP and SSE state.
	 */
	if (boot_cpu_has(X86_FEATURE_XSAVE))
		fpu->state.xsave.header.xfeatures |= XFEATURE_MASK_FPSSE;

	return ret;
}

int xstateregs_get(struct task_struct *target, const struct user_regset *regset,
		unsigned int pos, unsigned int count,
		void *kbuf, void __user *ubuf)
{
	struct fpu *fpu = &target->thread.fpu;
	struct xregs_state *xsave;
	int ret;

	if (!boot_cpu_has(X86_FEATURE_XSAVE))
		return -ENODEV;

	xsave = &fpu->state.xsave;

	fpu__prepare_read(fpu);

	if (using_compacted_format()) {
		if (kbuf)
			ret = copy_xstate_to_kernel(kbuf, xsave, pos, count);
		else
			ret = copy_xstate_to_user(ubuf, xsave, pos, count);
	} else {
		fpstate_sanitize_xstate(fpu);
		/*
		 * Copy the 48 bytes defined by the software into the xsave
		 * area in the thread struct, so that we can copy the whole
		 * area to user using one user_regset_copyout().
		 */
		memcpy(&xsave->i387.sw_reserved, xstate_fx_sw_bytes, sizeof(xstate_fx_sw_bytes));

		/*
		 * Copy the xstate memory layout.
		 */
		ret = user_regset_copyout(&pos, &count, &kbuf, &ubuf, xsave, 0, -1);
	}
	return ret;
=======
	struct fxregs_state newstate;
	int ret;

	if (!cpu_feature_enabled(X86_FEATURE_FXSR))
		return -ENODEV;

	/* No funny business with partial or oversized writes is permitted. */
	if (pos != 0 || count != sizeof(newstate))
		return -EINVAL;

	ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf, &newstate, 0, -1);
	if (ret)
		return ret;

	/* Do not allow an invalid MXCSR value. */
	if (newstate.mxcsr & ~mxcsr_feature_mask)
		return -EINVAL;

	fpu_force_restore(fpu);

	/* Copy the state  */
	memcpy(&fpu->state.fxsave, &newstate, sizeof(newstate));

	/* Clear xmm8..15 for 32-bit callers */
	BUILD_BUG_ON(sizeof(fpu->state.fxsave.xmm_space) != 16 * 16);
	if (in_ia32_syscall())
		memset(&fpu->state.fxsave.xmm_space[8*4], 0, 8 * 16);

	/* Mark FP and SSE as in use when XSAVE is enabled */
	if (use_xsave())
		fpu->state.xsave.header.xfeatures |= XFEATURE_MASK_FPSSE;

	return 0;
}

int xstateregs_get(struct task_struct *target, const struct user_regset *regset,
		struct membuf to)
{
	if (!cpu_feature_enabled(X86_FEATURE_XSAVE))
		return -ENODEV;

	sync_fpstate(&target->thread.fpu);

	copy_xstate_to_uabi_buf(to, target, XSTATE_COPY_XSAVE);
	return 0;
>>>>>>> upstream/android-13
}

int xstateregs_set(struct task_struct *target, const struct user_regset *regset,
		  unsigned int pos, unsigned int count,
		  const void *kbuf, const void __user *ubuf)
{
	struct fpu *fpu = &target->thread.fpu;
<<<<<<< HEAD
	struct xregs_state *xsave;
	int ret;

	if (!boot_cpu_has(X86_FEATURE_XSAVE))
=======
	struct xregs_state *tmpbuf = NULL;
	int ret;

	if (!cpu_feature_enabled(X86_FEATURE_XSAVE))
>>>>>>> upstream/android-13
		return -ENODEV;

	/*
	 * A whole standard-format XSAVE buffer is needed:
	 */
<<<<<<< HEAD
	if ((pos != 0) || (count < fpu_user_xstate_size))
		return -EFAULT;

	xsave = &fpu->state.xsave;

	fpu__prepare_write(fpu);

	if (using_compacted_format()) {
		if (kbuf)
			ret = copy_kernel_to_xstate(xsave, kbuf);
		else
			ret = copy_user_to_xstate(xsave, ubuf);
	} else {
		ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf, xsave, 0, -1);
		if (!ret)
			ret = validate_xstate_header(&xsave->header);
	}

	/*
	 * mxcsr reserved bits must be masked to zero for security reasons.
	 */
	xsave->i387.mxcsr &= mxcsr_feature_mask;

	/*
	 * In case of failure, mark all states as init:
	 */
	if (ret)
		fpstate_init(&fpu->state);

=======
	if (pos != 0 || count != fpu_user_xstate_size)
		return -EFAULT;

	if (!kbuf) {
		tmpbuf = vmalloc(count);
		if (!tmpbuf)
			return -ENOMEM;

		if (copy_from_user(tmpbuf, ubuf, count)) {
			ret = -EFAULT;
			goto out;
		}
	}

	fpu_force_restore(fpu);
	ret = copy_uabi_from_kernel_to_xstate(&fpu->state.xsave, kbuf ?: tmpbuf);

out:
	vfree(tmpbuf);
>>>>>>> upstream/android-13
	return ret;
}

#if defined CONFIG_X86_32 || defined CONFIG_IA32_EMULATION

/*
 * FPU tag word conversions.
 */

static inline unsigned short twd_i387_to_fxsr(unsigned short twd)
{
	unsigned int tmp; /* to avoid 16 bit prefixes in the code */

	/* Transform each pair of bits into 01 (valid) or 00 (empty) */
	tmp = ~twd;
	tmp = (tmp | (tmp>>1)) & 0x5555; /* 0V0V0V0V0V0V0V0V */
	/* and move the valid bits to the lower byte. */
	tmp = (tmp | (tmp >> 1)) & 0x3333; /* 00VV00VV00VV00VV */
	tmp = (tmp | (tmp >> 2)) & 0x0f0f; /* 0000VVVV0000VVVV */
	tmp = (tmp | (tmp >> 4)) & 0x00ff; /* 00000000VVVVVVVV */

	return tmp;
}

#define FPREG_ADDR(f, n)	((void *)&(f)->st_space + (n) * 16)
#define FP_EXP_TAG_VALID	0
#define FP_EXP_TAG_ZERO		1
#define FP_EXP_TAG_SPECIAL	2
#define FP_EXP_TAG_EMPTY	3

static inline u32 twd_fxsr_to_i387(struct fxregs_state *fxsave)
{
	struct _fpxreg *st;
	u32 tos = (fxsave->swd >> 11) & 7;
	u32 twd = (unsigned long) fxsave->twd;
	u32 tag;
	u32 ret = 0xffff0000u;
	int i;

	for (i = 0; i < 8; i++, twd >>= 1) {
		if (twd & 0x1) {
			st = FPREG_ADDR(fxsave, (i - tos) & 7);

			switch (st->exponent & 0x7fff) {
			case 0x7fff:
				tag = FP_EXP_TAG_SPECIAL;
				break;
			case 0x0000:
				if (!st->significand[0] &&
				    !st->significand[1] &&
				    !st->significand[2] &&
				    !st->significand[3])
					tag = FP_EXP_TAG_ZERO;
				else
					tag = FP_EXP_TAG_SPECIAL;
				break;
			default:
				if (st->significand[3] & 0x8000)
					tag = FP_EXP_TAG_VALID;
				else
					tag = FP_EXP_TAG_SPECIAL;
				break;
			}
		} else {
			tag = FP_EXP_TAG_EMPTY;
		}
		ret |= tag << (2 * i);
	}
	return ret;
}

/*
 * FXSR floating point environment conversions.
 */

<<<<<<< HEAD
void
convert_from_fxsr(struct user_i387_ia32_struct *env, struct task_struct *tsk)
{
	struct fxregs_state *fxsave = &tsk->thread.fpu.state.fxsave;
=======
static void __convert_from_fxsr(struct user_i387_ia32_struct *env,
				struct task_struct *tsk,
				struct fxregs_state *fxsave)
{
>>>>>>> upstream/android-13
	struct _fpreg *to = (struct _fpreg *) &env->st_space[0];
	struct _fpxreg *from = (struct _fpxreg *) &fxsave->st_space[0];
	int i;

	env->cwd = fxsave->cwd | 0xffff0000u;
	env->swd = fxsave->swd | 0xffff0000u;
	env->twd = twd_fxsr_to_i387(fxsave);

#ifdef CONFIG_X86_64
	env->fip = fxsave->rip;
	env->foo = fxsave->rdp;
	/*
	 * should be actually ds/cs at fpu exception time, but
	 * that information is not available in 64bit mode.
	 */
	env->fcs = task_pt_regs(tsk)->cs;
	if (tsk == current) {
		savesegment(ds, env->fos);
	} else {
		env->fos = tsk->thread.ds;
	}
	env->fos |= 0xffff0000;
#else
	env->fip = fxsave->fip;
	env->fcs = (u16) fxsave->fcs | ((u32) fxsave->fop << 16);
	env->foo = fxsave->foo;
	env->fos = fxsave->fos;
#endif

	for (i = 0; i < 8; ++i)
		memcpy(&to[i], &from[i], sizeof(to[0]));
}

<<<<<<< HEAD
void convert_to_fxsr(struct task_struct *tsk,
		     const struct user_i387_ia32_struct *env)

{
	struct fxregs_state *fxsave = &tsk->thread.fpu.state.fxsave;
=======
void
convert_from_fxsr(struct user_i387_ia32_struct *env, struct task_struct *tsk)
{
	__convert_from_fxsr(env, tsk, &tsk->thread.fpu.state.fxsave);
}

void convert_to_fxsr(struct fxregs_state *fxsave,
		     const struct user_i387_ia32_struct *env)

{
>>>>>>> upstream/android-13
	struct _fpreg *from = (struct _fpreg *) &env->st_space[0];
	struct _fpxreg *to = (struct _fpxreg *) &fxsave->st_space[0];
	int i;

	fxsave->cwd = env->cwd;
	fxsave->swd = env->swd;
	fxsave->twd = twd_i387_to_fxsr(env->twd);
	fxsave->fop = (u16) ((u32) env->fcs >> 16);
#ifdef CONFIG_X86_64
	fxsave->rip = env->fip;
	fxsave->rdp = env->foo;
	/* cs and ds ignored */
#else
	fxsave->fip = env->fip;
	fxsave->fcs = (env->fcs & 0xffff);
	fxsave->foo = env->foo;
	fxsave->fos = env->fos;
#endif

	for (i = 0; i < 8; ++i)
		memcpy(&to[i], &from[i], sizeof(from[0]));
}

int fpregs_get(struct task_struct *target, const struct user_regset *regset,
<<<<<<< HEAD
	       unsigned int pos, unsigned int count,
	       void *kbuf, void __user *ubuf)
{
	struct fpu *fpu = &target->thread.fpu;
	struct user_i387_ia32_struct env;

	fpu__prepare_read(fpu);

	if (!boot_cpu_has(X86_FEATURE_FPU))
		return fpregs_soft_get(target, regset, pos, count, kbuf, ubuf);

	if (!boot_cpu_has(X86_FEATURE_FXSR))
		return user_regset_copyout(&pos, &count, &kbuf, &ubuf,
					   &fpu->state.fsave, 0,
					   -1);

	fpstate_sanitize_xstate(fpu);

	if (kbuf && pos == 0 && count == sizeof(env)) {
		convert_from_fxsr(kbuf, target);
		return 0;
	}

	convert_from_fxsr(&env, target);

	return user_regset_copyout(&pos, &count, &kbuf, &ubuf, &env, 0, -1);
=======
	       struct membuf to)
{
	struct fpu *fpu = &target->thread.fpu;
	struct user_i387_ia32_struct env;
	struct fxregs_state fxsave, *fx;

	sync_fpstate(fpu);

	if (!cpu_feature_enabled(X86_FEATURE_FPU))
		return fpregs_soft_get(target, regset, to);

	if (!cpu_feature_enabled(X86_FEATURE_FXSR)) {
		return membuf_write(&to, &fpu->state.fsave,
				    sizeof(struct fregs_state));
	}

	if (use_xsave()) {
		struct membuf mb = { .p = &fxsave, .left = sizeof(fxsave) };

		/* Handle init state optimized xstate correctly */
		copy_xstate_to_uabi_buf(mb, target, XSTATE_COPY_FP);
		fx = &fxsave;
	} else {
		fx = &fpu->state.fxsave;
	}

	__convert_from_fxsr(&env, target, fx);
	return membuf_write(&to, &env, sizeof(env));
>>>>>>> upstream/android-13
}

int fpregs_set(struct task_struct *target, const struct user_regset *regset,
	       unsigned int pos, unsigned int count,
	       const void *kbuf, const void __user *ubuf)
{
	struct fpu *fpu = &target->thread.fpu;
	struct user_i387_ia32_struct env;
	int ret;

<<<<<<< HEAD
	fpu__prepare_write(fpu);
	fpstate_sanitize_xstate(fpu);

	if (!boot_cpu_has(X86_FEATURE_FPU))
		return fpregs_soft_set(target, regset, pos, count, kbuf, ubuf);

	if (!boot_cpu_has(X86_FEATURE_FXSR))
		return user_regset_copyin(&pos, &count, &kbuf, &ubuf,
					  &fpu->state.fsave, 0,
					  -1);

	if (pos > 0 || count < sizeof(env))
		convert_from_fxsr(&env, target);

	ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf, &env, 0, -1);
	if (!ret)
		convert_to_fxsr(target, &env);

	/*
	 * update the header bit in the xsave header, indicating the
	 * presence of FP.
	 */
	if (boot_cpu_has(X86_FEATURE_XSAVE))
		fpu->state.xsave.header.xfeatures |= XFEATURE_MASK_FP;
	return ret;
}

/*
 * FPU state for core dumps.
 * This is only used for a.out dumps now.
 * It is declared generically using elf_fpregset_t (which is
 * struct user_i387_struct) but is in fact only used for 32-bit
 * dumps, so on 64-bit it is really struct user_i387_ia32_struct.
 */
int dump_fpu(struct pt_regs *regs, struct user_i387_struct *ufpu)
{
	struct task_struct *tsk = current;
	struct fpu *fpu = &tsk->thread.fpu;
	int fpvalid;

	fpvalid = fpu->initialized;
	if (fpvalid)
		fpvalid = !fpregs_get(tsk, NULL,
				      0, sizeof(struct user_i387_ia32_struct),
				      ufpu, NULL);

	return fpvalid;
}
EXPORT_SYMBOL(dump_fpu);

=======
	/* No funny business with partial or oversized writes is permitted. */
	if (pos != 0 || count != sizeof(struct user_i387_ia32_struct))
		return -EINVAL;

	if (!cpu_feature_enabled(X86_FEATURE_FPU))
		return fpregs_soft_set(target, regset, pos, count, kbuf, ubuf);

	ret = user_regset_copyin(&pos, &count, &kbuf, &ubuf, &env, 0, -1);
	if (ret)
		return ret;

	fpu_force_restore(fpu);

	if (cpu_feature_enabled(X86_FEATURE_FXSR))
		convert_to_fxsr(&fpu->state.fxsave, &env);
	else
		memcpy(&fpu->state.fsave, &env, sizeof(env));

	/*
	 * Update the header bit in the xsave header, indicating the
	 * presence of FP.
	 */
	if (cpu_feature_enabled(X86_FEATURE_XSAVE))
		fpu->state.xsave.header.xfeatures |= XFEATURE_MASK_FP;

	return 0;
}

>>>>>>> upstream/android-13
#endif	/* CONFIG_X86_32 || CONFIG_IA32_EMULATION */
