<<<<<<< HEAD
/*
 * Copyright (C) 2012 ARM Ltd.
 * Author: Marc Zyngier <marc.zyngier@arm.com>
 *
 * This program is free software: you can redistribute it and/or modify
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
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 ARM Ltd.
 * Author: Marc Zyngier <marc.zyngier@arm.com>
>>>>>>> upstream/android-13
 */

#ifndef __ASM__VIRT_H
#define __ASM__VIRT_H

/*
 * The arm64 hcall implementation uses x0 to specify the hcall
 * number. A value less than HVC_STUB_HCALL_NR indicates a special
 * hcall, such as set vector. Any other value is handled in a
 * hypervisor specific way.
 *
 * The hypercall is allowed to clobber any of the caller-saved
 * registers (x0-x18), so it is advisable to use it through the
 * indirection of a function call (as implemented in hyp-stub.S).
 */

/*
 * HVC_SET_VECTORS - Set the value of the vbar_el2 register.
 *
 * @x1: Physical address of the new vector table.
 */
#define HVC_SET_VECTORS 0

/*
 * HVC_SOFT_RESTART - CPU soft reset, used by the cpu_soft_restart routine.
 */
#define HVC_SOFT_RESTART 1

/*
 * HVC_RESET_VECTORS - Restore the vectors to the original HYP stubs
 */
#define HVC_RESET_VECTORS 2

<<<<<<< HEAD
/* Max number of HYP stub hypercalls */
#define HVC_STUB_HCALL_NR 3
=======
/*
 * HVC_VHE_RESTART - Upgrade the CPU from EL1 to EL2, if possible
 */
#define HVC_VHE_RESTART	3

/* Max number of HYP stub hypercalls */
#define HVC_STUB_HCALL_NR 4
>>>>>>> upstream/android-13

/* Error returned when an invalid stub number is passed into x0 */
#define HVC_STUB_ERR	0xbadca11

#define BOOT_CPU_MODE_EL1	(0xe11)
#define BOOT_CPU_MODE_EL2	(0xe12)

#ifndef __ASSEMBLY__

#include <asm/ptrace.h>
#include <asm/sections.h>
#include <asm/sysreg.h>
#include <asm/cpufeature.h>

/*
 * __boot_cpu_mode records what mode CPUs were booted in.
 * A correctly-implemented bootloader must start all CPUs in the same mode:
 * In this case, both 32bit halves of __boot_cpu_mode will contain the
 * same value (either 0 if booted in EL1, BOOT_CPU_MODE_EL2 if booted in EL2).
 *
 * Should the bootloader fail to do this, the two values will be different.
 * This allows the kernel to flag an error when the secondaries have come up.
 */
extern u32 __boot_cpu_mode[2];

void __hyp_set_vectors(phys_addr_t phys_vector_base);
void __hyp_reset_vectors(void);

<<<<<<< HEAD
/* Reports the availability of HYP mode */
static inline bool is_hyp_mode_available(void)
{
=======
DECLARE_STATIC_KEY_FALSE(kvm_protected_mode_initialized);

static inline bool is_pkvm_initialized(void)
{
	return IS_ENABLED(CONFIG_KVM) &&
	       static_branch_likely(&kvm_protected_mode_initialized);
}

/* Reports the availability of HYP mode */
static inline bool is_hyp_mode_available(void)
{
	/*
	 * If KVM protected mode is initialized, all CPUs must have been booted
	 * in EL2. Avoid checking __boot_cpu_mode as CPUs now come up in EL1.
	 */
	if (is_pkvm_initialized())
		return true;

>>>>>>> upstream/android-13
	return (__boot_cpu_mode[0] == BOOT_CPU_MODE_EL2 &&
		__boot_cpu_mode[1] == BOOT_CPU_MODE_EL2);
}

/* Check if the bootloader has booted CPUs in different modes */
static inline bool is_hyp_mode_mismatched(void)
{
<<<<<<< HEAD
=======
	/*
	 * If KVM protected mode is initialized, all CPUs must have been booted
	 * in EL2. Avoid checking __boot_cpu_mode as CPUs now come up in EL1.
	 */
	if (is_pkvm_initialized())
		return false;

>>>>>>> upstream/android-13
	return __boot_cpu_mode[0] != __boot_cpu_mode[1];
}

static inline bool is_kernel_in_hyp_mode(void)
{
	return read_sysreg(CurrentEL) == CurrentEL_EL2;
}

<<<<<<< HEAD
static inline bool has_vhe(void)
{
	if (cpus_have_const_cap(ARM64_HAS_VIRT_HOST_EXTN))
		return true;

	return false;
=======
static __always_inline bool has_vhe(void)
{
	/*
	 * Code only run in VHE/NVHE hyp context can assume VHE is present or
	 * absent. Otherwise fall back to caps.
	 * This allows the compiler to discard VHE-specific code from the
	 * nVHE object, reducing the number of external symbol references
	 * needed to link.
	 */
	if (is_vhe_hyp_code())
		return true;
	else if (is_nvhe_hyp_code())
		return false;
	else
		return cpus_have_final_cap(ARM64_HAS_VIRT_HOST_EXTN);
}

static __always_inline bool is_protected_kvm_enabled(void)
{
	if (is_vhe_hyp_code())
		return false;
	else
		return cpus_have_final_cap(ARM64_KVM_PROTECTED_MODE);
>>>>>>> upstream/android-13
}

#endif /* __ASSEMBLY__ */

#endif /* ! __ASM__VIRT_H */
