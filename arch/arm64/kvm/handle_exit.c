<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2012,2013 - ARM Ltd
 * Author: Marc Zyngier <marc.zyngier@arm.com>
 *
 * Derived from arch/arm/kvm/handle_exit.c:
 * Copyright (C) 2012 - Virtual Open Systems and Columbia University
 * Author: Christoffer Dall <c.dall@virtualopensystems.com>
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
=======
>>>>>>> upstream/android-13
 */

#include <linux/kvm.h>
#include <linux/kvm_host.h>

<<<<<<< HEAD
#include <kvm/arm_psci.h>

#include <asm/esr.h>
#include <asm/exception.h>
#include <asm/kvm_asm.h>
#include <asm/kvm_coproc.h>
=======
#include <asm/esr.h>
#include <asm/exception.h>
#include <asm/kvm_asm.h>
>>>>>>> upstream/android-13
#include <asm/kvm_emulate.h>
#include <asm/kvm_mmu.h>
#include <asm/debug-monitors.h>
#include <asm/traps.h>

<<<<<<< HEAD
#define CREATE_TRACE_POINTS
#include "trace.h"

typedef int (*exit_handle_fn)(struct kvm_vcpu *, struct kvm_run *);
=======
#include <kvm/arm_hypercalls.h>

#define CREATE_TRACE_POINTS
#include "trace_handle_exit.h"

typedef int (*exit_handle_fn)(struct kvm_vcpu *);
>>>>>>> upstream/android-13

static void kvm_handle_guest_serror(struct kvm_vcpu *vcpu, u32 esr)
{
	if (!arm64_is_ras_serror(esr) || arm64_is_fatal_ras_serror(NULL, esr))
		kvm_inject_vabt(vcpu);
}

<<<<<<< HEAD
static int handle_hvc(struct kvm_vcpu *vcpu, struct kvm_run *run)
=======
static int handle_hvc(struct kvm_vcpu *vcpu)
>>>>>>> upstream/android-13
{
	int ret;

	trace_kvm_hvc_arm64(*vcpu_pc(vcpu), vcpu_get_reg(vcpu, 0),
			    kvm_vcpu_hvc_get_imm(vcpu));
	vcpu->stat.hvc_exit_stat++;

	ret = kvm_hvc_call_handler(vcpu);
	if (ret < 0) {
		vcpu_set_reg(vcpu, 0, ~0UL);
		return 1;
	}

	return ret;
}

<<<<<<< HEAD
static int handle_smc(struct kvm_vcpu *vcpu, struct kvm_run *run)
=======
static int handle_smc(struct kvm_vcpu *vcpu)
>>>>>>> upstream/android-13
{
	/*
	 * "If an SMC instruction executed at Non-secure EL1 is
	 * trapped to EL2 because HCR_EL2.TSC is 1, the exception is a
	 * Trap exception, not a Secure Monitor Call exception [...]"
	 *
	 * We need to advance the PC after the trap, as it would
	 * otherwise return to the same address...
	 */
	vcpu_set_reg(vcpu, 0, ~0UL);
<<<<<<< HEAD
	kvm_skip_instr(vcpu, kvm_vcpu_trap_il_is32bit(vcpu));
=======
	kvm_incr_pc(vcpu);
>>>>>>> upstream/android-13
	return 1;
}

/*
 * Guest access to FP/ASIMD registers are routed to this handler only
 * when the system doesn't support FP/ASIMD.
 */
<<<<<<< HEAD
static int handle_no_fpsimd(struct kvm_vcpu *vcpu, struct kvm_run *run)
=======
static int handle_no_fpsimd(struct kvm_vcpu *vcpu)
>>>>>>> upstream/android-13
{
	kvm_inject_undefined(vcpu);
	return 1;
}

/**
 * kvm_handle_wfx - handle a wait-for-interrupts or wait-for-event
 *		    instruction executed by a guest
 *
 * @vcpu:	the vcpu pointer
 *
 * WFE: Yield the CPU and come back to this vcpu when the scheduler
 * decides to.
 * WFI: Simply call kvm_vcpu_block(), which will halt execution of
 * world-switches and schedule other host processes until there is an
 * incoming IRQ or FIQ to the VM.
 */
<<<<<<< HEAD
static int kvm_handle_wfx(struct kvm_vcpu *vcpu, struct kvm_run *run)
{
	if (kvm_vcpu_get_hsr(vcpu) & ESR_ELx_WFx_ISS_WFE) {
=======
static int kvm_handle_wfx(struct kvm_vcpu *vcpu)
{
	if (kvm_vcpu_get_esr(vcpu) & ESR_ELx_WFx_ISS_WFE) {
>>>>>>> upstream/android-13
		trace_kvm_wfx_arm64(*vcpu_pc(vcpu), true);
		vcpu->stat.wfe_exit_stat++;
		kvm_vcpu_on_spin(vcpu, vcpu_mode_priv(vcpu));
	} else {
		trace_kvm_wfx_arm64(*vcpu_pc(vcpu), false);
		vcpu->stat.wfi_exit_stat++;
		kvm_vcpu_block(vcpu);
		kvm_clear_request(KVM_REQ_UNHALT, vcpu);
	}

<<<<<<< HEAD
	kvm_skip_instr(vcpu, kvm_vcpu_trap_il_is32bit(vcpu));
=======
	kvm_incr_pc(vcpu);
>>>>>>> upstream/android-13

	return 1;
}

/**
 * kvm_handle_guest_debug - handle a debug exception instruction
 *
 * @vcpu:	the vcpu pointer
<<<<<<< HEAD
 * @run:	access to the kvm_run structure for results
=======
>>>>>>> upstream/android-13
 *
 * We route all debug exceptions through the same handler. If both the
 * guest and host are using the same debug facilities it will be up to
 * userspace to re-inject the correct exception for guest delivery.
 *
<<<<<<< HEAD
 * @return: 0 (while setting run->exit_reason), -1 for error
 */
static int kvm_handle_guest_debug(struct kvm_vcpu *vcpu, struct kvm_run *run)
{
	u32 hsr = kvm_vcpu_get_hsr(vcpu);
	int ret = 0;

	run->exit_reason = KVM_EXIT_DEBUG;
	run->debug.arch.hsr = hsr;

	switch (ESR_ELx_EC(hsr)) {
	case ESR_ELx_EC_WATCHPT_LOW:
		run->debug.arch.far = vcpu->arch.fault.far_el2;
		/* fall through */
	case ESR_ELx_EC_SOFTSTP_LOW:
	case ESR_ELx_EC_BREAKPT_LOW:
	case ESR_ELx_EC_BKPT32:
	case ESR_ELx_EC_BRK64:
		break;
	default:
		kvm_err("%s: un-handled case hsr: %#08x\n",
			__func__, (unsigned int) hsr);
		ret = -1;
		break;
	}

	return ret;
}

static int kvm_handle_unknown_ec(struct kvm_vcpu *vcpu, struct kvm_run *run)
{
	u32 hsr = kvm_vcpu_get_hsr(vcpu);

	kvm_pr_unimpl("Unknown exception class: hsr: %#08x -- %s\n",
		      hsr, esr_get_class_string(hsr));
=======
 * @return: 0 (while setting vcpu->run->exit_reason)
 */
static int kvm_handle_guest_debug(struct kvm_vcpu *vcpu)
{
	struct kvm_run *run = vcpu->run;
	u32 esr = kvm_vcpu_get_esr(vcpu);

	run->exit_reason = KVM_EXIT_DEBUG;
	run->debug.arch.hsr = esr;

	if (ESR_ELx_EC(esr) == ESR_ELx_EC_WATCHPT_LOW)
		run->debug.arch.far = vcpu->arch.fault.far_el2;

	return 0;
}

static int kvm_handle_unknown_ec(struct kvm_vcpu *vcpu)
{
	u32 esr = kvm_vcpu_get_esr(vcpu);

	kvm_pr_unimpl("Unknown exception class: esr: %#08x -- %s\n",
		      esr, esr_get_class_string(esr));
>>>>>>> upstream/android-13

	kvm_inject_undefined(vcpu);
	return 1;
}

<<<<<<< HEAD
static int handle_sve(struct kvm_vcpu *vcpu, struct kvm_run *run)
=======
static int handle_sve(struct kvm_vcpu *vcpu)
>>>>>>> upstream/android-13
{
	/* Until SVE is supported for guests: */
	kvm_inject_undefined(vcpu);
	return 1;
}

<<<<<<< HEAD
=======
/*
 * Guest usage of a ptrauth instruction (which the guest EL1 did not turn into
 * a NOP). If we get here, it is that we didn't fixup ptrauth on exit, and all
 * that we can do is give the guest an UNDEF.
 */
static int kvm_handle_ptrauth(struct kvm_vcpu *vcpu)
{
	kvm_inject_undefined(vcpu);
	return 1;
}

>>>>>>> upstream/android-13
static exit_handle_fn arm_exit_handlers[] = {
	[0 ... ESR_ELx_EC_MAX]	= kvm_handle_unknown_ec,
	[ESR_ELx_EC_WFx]	= kvm_handle_wfx,
	[ESR_ELx_EC_CP15_32]	= kvm_handle_cp15_32,
	[ESR_ELx_EC_CP15_64]	= kvm_handle_cp15_64,
	[ESR_ELx_EC_CP14_MR]	= kvm_handle_cp14_32,
	[ESR_ELx_EC_CP14_LS]	= kvm_handle_cp14_load_store,
	[ESR_ELx_EC_CP14_64]	= kvm_handle_cp14_64,
	[ESR_ELx_EC_HVC32]	= handle_hvc,
	[ESR_ELx_EC_SMC32]	= handle_smc,
	[ESR_ELx_EC_HVC64]	= handle_hvc,
	[ESR_ELx_EC_SMC64]	= handle_smc,
	[ESR_ELx_EC_SYS64]	= kvm_handle_sys_reg,
	[ESR_ELx_EC_SVE]	= handle_sve,
	[ESR_ELx_EC_IABT_LOW]	= kvm_handle_guest_abort,
	[ESR_ELx_EC_DABT_LOW]	= kvm_handle_guest_abort,
	[ESR_ELx_EC_SOFTSTP_LOW]= kvm_handle_guest_debug,
	[ESR_ELx_EC_WATCHPT_LOW]= kvm_handle_guest_debug,
	[ESR_ELx_EC_BREAKPT_LOW]= kvm_handle_guest_debug,
	[ESR_ELx_EC_BKPT32]	= kvm_handle_guest_debug,
	[ESR_ELx_EC_BRK64]	= kvm_handle_guest_debug,
	[ESR_ELx_EC_FP_ASIMD]	= handle_no_fpsimd,
<<<<<<< HEAD
=======
	[ESR_ELx_EC_PAC]	= kvm_handle_ptrauth,
>>>>>>> upstream/android-13
};

static exit_handle_fn kvm_get_exit_handler(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	u32 hsr = kvm_vcpu_get_hsr(vcpu);
	u8 hsr_ec = ESR_ELx_EC(hsr);

	return arm_exit_handlers[hsr_ec];
=======
	u32 esr = kvm_vcpu_get_esr(vcpu);
	u8 esr_ec = ESR_ELx_EC(esr);

	return arm_exit_handlers[esr_ec];
>>>>>>> upstream/android-13
}

/*
 * We may be single-stepping an emulated instruction. If the emulation
 * has been completed in the kernel, we can return to userspace with a
 * KVM_EXIT_DEBUG, otherwise userspace needs to complete its
 * emulation first.
 */
<<<<<<< HEAD
static int handle_trap_exceptions(struct kvm_vcpu *vcpu, struct kvm_run *run)
=======
static int handle_trap_exceptions(struct kvm_vcpu *vcpu)
>>>>>>> upstream/android-13
{
	int handled;

	/*
<<<<<<< HEAD
=======
	 * If we run a non-protected VM when protection is enabled
	 * system-wide, resync the state from the hypervisor and mark
	 * it as dirty on the host side if it wasn't dirty already
	 * (which could happen if preemption has taken place).
	 */
	if (is_protected_kvm_enabled() && !kvm_vm_is_protected(vcpu->kvm)) {
		preempt_disable();
		if (!(vcpu->arch.flags & KVM_ARM64_PKVM_STATE_DIRTY)) {
			kvm_call_hyp_nvhe(__pkvm_vcpu_sync_state);
			vcpu->arch.flags |= KVM_ARM64_PKVM_STATE_DIRTY;
		}
		preempt_enable();
	}

	/*
>>>>>>> upstream/android-13
	 * See ARM ARM B1.14.1: "Hyp traps on instructions
	 * that fail their condition code check"
	 */
	if (!kvm_condition_valid(vcpu)) {
<<<<<<< HEAD
		kvm_skip_instr(vcpu, kvm_vcpu_trap_il_is32bit(vcpu));
=======
		kvm_incr_pc(vcpu);
>>>>>>> upstream/android-13
		handled = 1;
	} else {
		exit_handle_fn exit_handler;

		exit_handler = kvm_get_exit_handler(vcpu);
<<<<<<< HEAD
		handled = exit_handler(vcpu, run);
	}

	/*
	 * kvm_arm_handle_step_debug() sets the exit_reason on the kvm_run
	 * structure if we need to return to userspace.
	 */
	if (handled > 0 && kvm_arm_handle_step_debug(vcpu, run))
		handled = 0;

=======
		handled = exit_handler(vcpu);
	}

>>>>>>> upstream/android-13
	return handled;
}

/*
 * Return > 0 to return to guest, < 0 on error, 0 (and set exit_reason) on
 * proper exit to userspace.
 */
<<<<<<< HEAD
int handle_exit(struct kvm_vcpu *vcpu, struct kvm_run *run,
		       int exception_index)
{
	if (ARM_SERROR_PENDING(exception_index)) {
		u8 hsr_ec = ESR_ELx_EC(kvm_vcpu_get_hsr(vcpu));

		/*
		 * HVC/SMC already have an adjusted PC, which we need
		 * to correct in order to return to after having
		 * injected the SError.
		 */
		if (hsr_ec == ESR_ELx_EC_HVC32 || hsr_ec == ESR_ELx_EC_HVC64 ||
		    hsr_ec == ESR_ELx_EC_SMC32 || hsr_ec == ESR_ELx_EC_SMC64) {
			u32 adj =  kvm_vcpu_trap_il_is32bit(vcpu) ? 4 : 2;
			*vcpu_pc(vcpu) -= adj;
		}

=======
int handle_exit(struct kvm_vcpu *vcpu, int exception_index)
{
	struct kvm_run *run = vcpu->run;

	if (ARM_SERROR_PENDING(exception_index)) {
		/*
		 * The SError is handled by handle_exit_early(). If the guest
		 * survives it will re-execute the original instruction.
		 */
>>>>>>> upstream/android-13
		return 1;
	}

	exception_index = ARM_EXCEPTION_CODE(exception_index);

	switch (exception_index) {
	case ARM_EXCEPTION_IRQ:
		return 1;
	case ARM_EXCEPTION_EL1_SERROR:
<<<<<<< HEAD
		/* We may still need to return for single-step */
		if (!(*vcpu_cpsr(vcpu) & DBG_SPSR_SS)
			&& kvm_arm_handle_step_debug(vcpu, run))
			return 0;
		else
			return 1;
	case ARM_EXCEPTION_TRAP:
		return handle_trap_exceptions(vcpu, run);
=======
		return 1;
	case ARM_EXCEPTION_TRAP:
		return handle_trap_exceptions(vcpu);
>>>>>>> upstream/android-13
	case ARM_EXCEPTION_HYP_GONE:
		/*
		 * EL2 has been reset to the hyp-stub. This happens when a guest
		 * is pre-empted by kvm_reboot()'s shutdown call.
		 */
		run->exit_reason = KVM_EXIT_FAIL_ENTRY;
		return 0;
<<<<<<< HEAD
=======
	case ARM_EXCEPTION_IL:
		/*
		 * We attempted an illegal exception return.  Guest state must
		 * have been corrupted somehow.  Give up.
		 */
		run->exit_reason = KVM_EXIT_FAIL_ENTRY;
		return -EINVAL;
>>>>>>> upstream/android-13
	default:
		kvm_pr_unimpl("Unsupported exception type: %d",
			      exception_index);
		run->exit_reason = KVM_EXIT_INTERNAL_ERROR;
		return 0;
	}
}

/* For exit types that need handling before we can be preempted */
<<<<<<< HEAD
void handle_exit_early(struct kvm_vcpu *vcpu, struct kvm_run *run,
		       int exception_index)
{
=======
void handle_exit_early(struct kvm_vcpu *vcpu, int exception_index)
{
	/*
	 * We just exited, so the state is clean from a hypervisor
	 * perspective.
	 */
	if (is_protected_kvm_enabled())
		vcpu->arch.flags &= ~KVM_ARM64_PKVM_STATE_DIRTY;

>>>>>>> upstream/android-13
	if (ARM_SERROR_PENDING(exception_index)) {
		if (this_cpu_has_cap(ARM64_HAS_RAS_EXTN)) {
			u64 disr = kvm_vcpu_get_disr(vcpu);

			kvm_handle_guest_serror(vcpu, disr_to_esr(disr));
		} else {
			kvm_inject_vabt(vcpu);
		}

		return;
	}

	exception_index = ARM_EXCEPTION_CODE(exception_index);

	if (exception_index == ARM_EXCEPTION_EL1_SERROR)
<<<<<<< HEAD
		kvm_handle_guest_serror(vcpu, kvm_vcpu_get_hsr(vcpu));
=======
		kvm_handle_guest_serror(vcpu, kvm_vcpu_get_esr(vcpu));
}

void __noreturn __cold nvhe_hyp_panic_handler(u64 esr, u64 spsr,
					      u64 elr_virt, u64 elr_phys,
					      u64 par, uintptr_t vcpu,
					      u64 far, u64 hpfar) {
	u64 elr_in_kimg = __phys_to_kimg(elr_phys);
	u64 hyp_offset = elr_in_kimg - kaslr_offset() - elr_virt;
	u64 mode = spsr & PSR_MODE_MASK;

	/*
	 * The nVHE hyp symbols are not included by kallsyms to avoid issues
	 * with aliasing. That means that the symbols cannot be printed with the
	 * "%pS" format specifier, so fall back to the vmlinux address if
	 * there's no better option.
	 */
	if (mode != PSR_MODE_EL2t && mode != PSR_MODE_EL2h) {
		kvm_err("Invalid host exception to nVHE hyp!\n");
	} else if (ESR_ELx_EC(esr) == ESR_ELx_EC_BRK64 &&
		   (esr & ESR_ELx_BRK64_ISS_COMMENT_MASK) == BUG_BRK_IMM) {
		const char *file = NULL;
		unsigned int line = 0;

		/* All hyp bugs, including warnings, are treated as fatal. */
		if (!is_protected_kvm_enabled() ||
		    IS_ENABLED(CONFIG_NVHE_EL2_DEBUG)) {
			struct bug_entry *bug = find_bug(elr_in_kimg);

			if (bug)
				bug_get_file_line(bug, &file, &line);
		}

		if (file)
			kvm_err("nVHE hyp BUG at: %s:%u!\n", file, line);
		else
			kvm_err("nVHE hyp BUG at: %016llx!\n", elr_virt + hyp_offset);
	} else {
		kvm_err("nVHE hyp panic at: %016llx!\n", elr_virt + hyp_offset);
	}

	/*
	 * Hyp has panicked and we're going to handle that by panicking the
	 * kernel. The kernel offset will be revealed in the panic so we're
	 * also safe to reveal the hyp offset as a debugging aid for translating
	 * hyp VAs to vmlinux addresses.
	 */
	kvm_err("Hyp Offset: 0x%llx\n", hyp_offset);

	panic("HYP panic:\nPS:%08llx PC:%016llx ESR:%08llx\nFAR:%016llx HPFAR:%016llx PAR:%016llx\nVCPU:%016lx\n",
	      spsr, elr_virt, esr, far, hpfar, par, vcpu);
>>>>>>> upstream/android-13
}
