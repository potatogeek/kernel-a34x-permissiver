// SPDX-License-Identifier: GPL-2.0
/*
 * arch/arm64/kvm/fpsimd.c: Guest/host FPSIMD context coordination helpers
 *
 * Copyright 2018 Arm Limited
 * Author: Dave Martin <Dave.Martin@arm.com>
 */
#include <linux/irqflags.h>
#include <linux/sched.h>
<<<<<<< HEAD
#include <linux/thread_info.h>
#include <linux/kvm_host.h>
#include <asm/kvm_asm.h>
#include <asm/kvm_host.h>
#include <asm/kvm_mmu.h>
#include <asm/sysreg.h>

=======
#include <linux/kvm_host.h>
#include <asm/fpsimd.h>
#include <asm/kvm_asm.h>
#include <asm/kvm_hyp.h>
#include <asm/kvm_mmu.h>
#include <asm/sysreg.h>

void kvm_vcpu_unshare_task_fp(struct kvm_vcpu *vcpu)
{
	struct task_struct *p = vcpu->arch.parent_task;
	struct user_fpsimd_state *fpsimd;

	if (!is_protected_kvm_enabled() || !p)
		return;

	fpsimd = &p->thread.uw.fpsimd_state;
	kvm_unshare_hyp(fpsimd, fpsimd + 1);
	put_task_struct(p);
}

>>>>>>> upstream/android-13
/*
 * Called on entry to KVM_RUN unless this vcpu previously ran at least
 * once and the most recent prior KVM_RUN for this vcpu was called from
 * the same task as current (highly likely).
 *
 * This is guaranteed to execute before kvm_arch_vcpu_load_fp(vcpu),
 * such that on entering hyp the relevant parts of current are already
 * mapped.
 */
int kvm_arch_vcpu_run_map_fp(struct kvm_vcpu *vcpu)
{
	int ret;

<<<<<<< HEAD
	struct thread_info *ti = &current->thread_info;
	struct user_fpsimd_state *fpsimd = &current->thread.uw.fpsimd_state;

	/*
	 * Make sure the host task thread flags and fpsimd state are
	 * visible to hyp:
	 */
	ret = create_hyp_mappings(ti, ti + 1, PAGE_HYP);
	if (ret)
		goto error;

	ret = create_hyp_mappings(fpsimd, fpsimd + 1, PAGE_HYP);
	if (ret)
		goto error;

	vcpu->arch.host_thread_info = kern_hyp_va(ti);
	vcpu->arch.host_fpsimd_state = kern_hyp_va(fpsimd);
error:
	return ret;
=======
	struct user_fpsimd_state *fpsimd = &current->thread.uw.fpsimd_state;

	kvm_vcpu_unshare_task_fp(vcpu);

	/* Make sure the host task fpsimd state is visible to hyp: */
	ret = kvm_share_hyp(fpsimd, fpsimd + 1);
	if (ret)
		return ret;

	vcpu->arch.host_fpsimd_state = kern_hyp_va(fpsimd);

	/*
	 * We need to keep current's task_struct pinned until its data has been
	 * unshared with the hypervisor to make sure it is not re-used by the
	 * kernel and donated to someone else while already shared -- see
	 * kvm_vcpu_unshare_task_fp() for the matching put_task_struct().
	 */
	if (is_protected_kvm_enabled()) {
		get_task_struct(current);
		vcpu->arch.parent_task = current;
	}

	return 0;
>>>>>>> upstream/android-13
}

/*
 * Prepare vcpu for saving the host's FPSIMD state and loading the guest's.
 * The actual loading is done by the FPSIMD access trap taken to hyp.
 *
 * Here, we just set the correct metadata to indicate that the FPSIMD
 * state in the cpu regs (if any) belongs to current on the host.
<<<<<<< HEAD
 *
 * TIF_SVE is backed up here, since it may get clobbered with guest state.
 * This flag is restored by kvm_arch_vcpu_put_fp(vcpu).
=======
>>>>>>> upstream/android-13
 */
void kvm_arch_vcpu_load_fp(struct kvm_vcpu *vcpu)
{
	BUG_ON(!current->mm);
<<<<<<< HEAD

	vcpu->arch.flags &= ~(KVM_ARM64_FP_ENABLED |
			      KVM_ARM64_HOST_SVE_IN_USE |
			      KVM_ARM64_HOST_SVE_ENABLED);
	vcpu->arch.flags |= KVM_ARM64_FP_HOST;

	if (test_thread_flag(TIF_SVE))
		vcpu->arch.flags |= KVM_ARM64_HOST_SVE_IN_USE;

=======
	BUG_ON(test_thread_flag(TIF_SVE));

	vcpu->arch.flags &= ~KVM_ARM64_FP_ENABLED;
	vcpu->arch.flags |= KVM_ARM64_FP_HOST;

>>>>>>> upstream/android-13
	if (read_sysreg(cpacr_el1) & CPACR_EL1_ZEN_EL0EN)
		vcpu->arch.flags |= KVM_ARM64_HOST_SVE_ENABLED;
}

<<<<<<< HEAD
=======
void kvm_arch_vcpu_ctxflush_fp(struct kvm_vcpu *vcpu)
{
	if (test_thread_flag(TIF_FOREIGN_FPSTATE))
		vcpu->arch.flags |= KVM_ARM64_FP_FOREIGN_FPSTATE;
	else
		vcpu->arch.flags &= ~KVM_ARM64_FP_FOREIGN_FPSTATE;
}

>>>>>>> upstream/android-13
/*
 * If the guest FPSIMD state was loaded, update the host's context
 * tracking data mark the CPU FPSIMD regs as dirty and belonging to vcpu
 * so that they will be written back if the kernel clobbers them due to
 * kernel-mode NEON before re-entry into the guest.
 */
void kvm_arch_vcpu_ctxsync_fp(struct kvm_vcpu *vcpu)
{
	WARN_ON_ONCE(!irqs_disabled());

	if (vcpu->arch.flags & KVM_ARM64_FP_ENABLED) {
<<<<<<< HEAD
		fpsimd_bind_state_to_cpu(&vcpu->arch.ctxt.gp_regs.fp_regs);
		clear_thread_flag(TIF_FOREIGN_FPSTATE);
		clear_thread_flag(TIF_SVE);
=======
		fpsimd_bind_state_to_cpu(&vcpu->arch.ctxt.fp_regs,
					 vcpu->arch.sve_state,
					 vcpu->arch.sve_max_vl);

		clear_thread_flag(TIF_FOREIGN_FPSTATE);
		update_thread_flag(TIF_SVE, vcpu_has_sve(vcpu));
>>>>>>> upstream/android-13
	}
}

/*
 * Write back the vcpu FPSIMD regs if they are dirty, and invalidate the
 * cpu FPSIMD regs so that they can't be spuriously reused if this vcpu
 * disappears and another task or vcpu appears that recycles the same
 * struct fpsimd_state.
 */
void kvm_arch_vcpu_put_fp(struct kvm_vcpu *vcpu)
{
	unsigned long flags;

	local_irq_save(flags);

	if (vcpu->arch.flags & KVM_ARM64_FP_ENABLED) {
<<<<<<< HEAD
		/* Clean guest FP state to memory and invalidate cpu view */
		fpsimd_save();
		fpsimd_flush_cpu_state();
	} else if (system_supports_sve()) {
=======
		if (vcpu_has_sve(vcpu)) {
			__vcpu_sys_reg(vcpu, ZCR_EL1) = read_sysreg_el1(SYS_ZCR);

			/* Restore the VL that was saved when bound to the CPU */
			if (!has_vhe())
				sve_cond_update_zcr_vq(vcpu_sve_max_vq(vcpu) - 1,
						       SYS_ZCR_EL1);
		}

		fpsimd_save_and_flush_cpu_state();
	} else if (has_vhe() && system_supports_sve()) {
>>>>>>> upstream/android-13
		/*
		 * The FPSIMD/SVE state in the CPU has not been touched, and we
		 * have SVE (and VHE): CPACR_EL1 (alias CPTR_EL2) has been
		 * reset to CPACR_EL1_DEFAULT by the Hyp code, disabling SVE
		 * for EL0.  To avoid spurious traps, restore the trap state
		 * seen by kvm_arch_vcpu_load_fp():
		 */
		if (vcpu->arch.flags & KVM_ARM64_HOST_SVE_ENABLED)
			sysreg_clear_set(CPACR_EL1, 0, CPACR_EL1_ZEN_EL0EN);
		else
			sysreg_clear_set(CPACR_EL1, CPACR_EL1_ZEN_EL0EN, 0);
	}

<<<<<<< HEAD
	update_thread_flag(TIF_SVE,
			   vcpu->arch.flags & KVM_ARM64_HOST_SVE_IN_USE);
=======
	update_thread_flag(TIF_SVE, 0);
>>>>>>> upstream/android-13

	local_irq_restore(flags);
}
