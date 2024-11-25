<<<<<<< HEAD
/*
 * Copyright (C) 2015 - ARM Ltd
 * Author: Marc Zyngier <marc.zyngier@arm.com>
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
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2015 - ARM Ltd
 * Author: Marc Zyngier <marc.zyngier@arm.com>
>>>>>>> upstream/android-13
 */

#ifndef __ARM64_KVM_HYP_H__
#define __ARM64_KVM_HYP_H__

#include <linux/compiler.h>
#include <linux/kvm_host.h>
<<<<<<< HEAD
#include <asm/sysreg.h>

#define __hyp_text __section(.hyp.text) notrace
=======
#include <asm/alternative.h>
#include <asm/sysreg.h>

DECLARE_PER_CPU(struct kvm_cpu_context, kvm_hyp_ctxt);
DECLARE_PER_CPU(unsigned long, kvm_hyp_vector);
DECLARE_PER_CPU(struct kvm_nvhe_init_params, kvm_init_params);
>>>>>>> upstream/android-13

#define read_sysreg_elx(r,nvh,vh)					\
	({								\
		u64 reg;						\
<<<<<<< HEAD
		asm volatile(ALTERNATIVE("mrs %0, " __stringify(r##nvh),\
=======
		asm volatile(ALTERNATIVE(__mrs_s("%0", r##nvh),	\
>>>>>>> upstream/android-13
					 __mrs_s("%0", r##vh),		\
					 ARM64_HAS_VIRT_HOST_EXTN)	\
			     : "=r" (reg));				\
		reg;							\
	})

#define write_sysreg_elx(v,r,nvh,vh)					\
	do {								\
		u64 __val = (u64)(v);					\
<<<<<<< HEAD
		asm volatile(ALTERNATIVE("msr " __stringify(r##nvh) ", %x0",\
=======
		asm volatile(ALTERNATIVE(__msr_s(r##nvh, "%x0"),	\
>>>>>>> upstream/android-13
					 __msr_s(r##vh, "%x0"),		\
					 ARM64_HAS_VIRT_HOST_EXTN)	\
					 : : "rZ" (__val));		\
	} while (0)

/*
 * Unified accessors for registers that have a different encoding
 * between VHE and non-VHE. They must be specified without their "ELx"
<<<<<<< HEAD
 * encoding.
 */
#define read_sysreg_el2(r)						\
	({								\
		u64 reg;						\
		asm volatile(ALTERNATIVE("mrs %0, " __stringify(r##_EL2),\
					 "mrs %0, " __stringify(r##_EL1),\
					 ARM64_HAS_VIRT_HOST_EXTN)	\
			     : "=r" (reg));				\
		reg;							\
	})

#define write_sysreg_el2(v,r)						\
	do {								\
		u64 __val = (u64)(v);					\
		asm volatile(ALTERNATIVE("msr " __stringify(r##_EL2) ", %x0",\
					 "msr " __stringify(r##_EL1) ", %x0",\
					 ARM64_HAS_VIRT_HOST_EXTN)	\
					 : : "rZ" (__val));		\
	} while (0)
=======
 * encoding, but with the SYS_ prefix, as defined in asm/sysreg.h.
 */
>>>>>>> upstream/android-13

#define read_sysreg_el0(r)	read_sysreg_elx(r, _EL0, _EL02)
#define write_sysreg_el0(v,r)	write_sysreg_elx(v, r, _EL0, _EL02)
#define read_sysreg_el1(r)	read_sysreg_elx(r, _EL1, _EL12)
#define write_sysreg_el1(v,r)	write_sysreg_elx(v, r, _EL1, _EL12)
<<<<<<< HEAD

/* The VHE specific system registers and their encoding */
#define sctlr_EL12              sys_reg(3, 5, 1, 0, 0)
#define cpacr_EL12              sys_reg(3, 5, 1, 0, 2)
#define ttbr0_EL12              sys_reg(3, 5, 2, 0, 0)
#define ttbr1_EL12              sys_reg(3, 5, 2, 0, 1)
#define tcr_EL12                sys_reg(3, 5, 2, 0, 2)
#define afsr0_EL12              sys_reg(3, 5, 5, 1, 0)
#define afsr1_EL12              sys_reg(3, 5, 5, 1, 1)
#define esr_EL12                sys_reg(3, 5, 5, 2, 0)
#define far_EL12                sys_reg(3, 5, 6, 0, 0)
#define mair_EL12               sys_reg(3, 5, 10, 2, 0)
#define amair_EL12              sys_reg(3, 5, 10, 3, 0)
#define vbar_EL12               sys_reg(3, 5, 12, 0, 0)
#define contextidr_EL12         sys_reg(3, 5, 13, 0, 1)
#define cntkctl_EL12            sys_reg(3, 5, 14, 1, 0)
#define cntp_tval_EL02          sys_reg(3, 5, 14, 2, 0)
#define cntp_ctl_EL02           sys_reg(3, 5, 14, 2, 1)
#define cntp_cval_EL02          sys_reg(3, 5, 14, 2, 2)
#define cntv_tval_EL02          sys_reg(3, 5, 14, 3, 0)
#define cntv_ctl_EL02           sys_reg(3, 5, 14, 3, 1)
#define cntv_cval_EL02          sys_reg(3, 5, 14, 3, 2)
#define spsr_EL12               sys_reg(3, 5, 4, 0, 0)
#define elr_EL12                sys_reg(3, 5, 4, 0, 1)

/**
 * hyp_alternate_select - Generates patchable code sequences that are
 * used to switch between two implementations of a function, depending
 * on the availability of a feature.
 *
 * @fname: a symbol name that will be defined as a function returning a
 * function pointer whose type will match @orig and @alt
 * @orig: A pointer to the default function, as returned by @fname when
 * @cond doesn't hold
 * @alt: A pointer to the alternate function, as returned by @fname
 * when @cond holds
 * @cond: a CPU feature (as described in asm/cpufeature.h)
 */
#define hyp_alternate_select(fname, orig, alt, cond)			\
typeof(orig) * __hyp_text fname(void)					\
{									\
	typeof(alt) *val = orig;					\
	asm volatile(ALTERNATIVE("nop		\n",			\
				 "mov	%0, %1	\n",			\
				 cond)					\
		     : "+r" (val) : "r" (alt));				\
	return val;							\
}

int __vgic_v2_perform_cpuif_access(struct kvm_vcpu *vcpu);

void __vgic_v3_save_state(struct kvm_vcpu *vcpu);
void __vgic_v3_restore_state(struct kvm_vcpu *vcpu);
void __vgic_v3_activate_traps(struct kvm_vcpu *vcpu);
void __vgic_v3_deactivate_traps(struct kvm_vcpu *vcpu);
void __vgic_v3_save_aprs(struct kvm_vcpu *vcpu);
void __vgic_v3_restore_aprs(struct kvm_vcpu *vcpu);
int __vgic_v3_perform_cpuif_access(struct kvm_vcpu *vcpu);

void __timer_enable_traps(struct kvm_vcpu *vcpu);
void __timer_disable_traps(struct kvm_vcpu *vcpu);

void __sysreg_save_state_nvhe(struct kvm_cpu_context *ctxt);
void __sysreg_restore_state_nvhe(struct kvm_cpu_context *ctxt);
=======
#define read_sysreg_el2(r)	read_sysreg_elx(r, _EL2, _EL1)
#define write_sysreg_el2(v,r)	write_sysreg_elx(v, r, _EL2, _EL1)

/*
 * Without an __arch_swab32(), we fall back to ___constant_swab32(), but the
 * static inline can allow the compiler to out-of-line this. KVM always wants
 * the macro version as its always inlined.
 */
#define __kvm_swab32(x)	___constant_swab32(x)

int __vgic_v2_perform_cpuif_access(struct kvm_vcpu *vcpu);

void __vgic_v3_save_state(struct vgic_v3_cpu_if *cpu_if);
void __vgic_v3_restore_state(struct vgic_v3_cpu_if *cpu_if);
void __vgic_v3_activate_traps(struct vgic_v3_cpu_if *cpu_if);
void __vgic_v3_deactivate_traps(struct vgic_v3_cpu_if *cpu_if);
void __vgic_v3_save_vmcr_aprs(struct vgic_v3_cpu_if *cpu_if);
void __vgic_v3_restore_vmcr_aprs(struct vgic_v3_cpu_if *cpu_if);
int __vgic_v3_perform_cpuif_access(struct kvm_vcpu *vcpu);

#ifdef __KVM_NVHE_HYPERVISOR__
void __timer_enable_traps(struct kvm_vcpu *vcpu);
void __timer_disable_traps(struct kvm_vcpu *vcpu);
#endif

#ifdef __KVM_NVHE_HYPERVISOR__
void __sysreg_save_state_nvhe(struct kvm_cpu_context *ctxt);
void __sysreg_restore_state_nvhe(struct kvm_cpu_context *ctxt);
#else
>>>>>>> upstream/android-13
void sysreg_save_host_state_vhe(struct kvm_cpu_context *ctxt);
void sysreg_restore_host_state_vhe(struct kvm_cpu_context *ctxt);
void sysreg_save_guest_state_vhe(struct kvm_cpu_context *ctxt);
void sysreg_restore_guest_state_vhe(struct kvm_cpu_context *ctxt);
<<<<<<< HEAD
void __sysreg32_save_state(struct kvm_vcpu *vcpu);
void __sysreg32_restore_state(struct kvm_vcpu *vcpu);

void __debug_switch_to_guest(struct kvm_vcpu *vcpu);
void __debug_switch_to_host(struct kvm_vcpu *vcpu);
void __debug_save_host_buffers_nvhe(struct kvm_vcpu *vcpu);
void __debug_restore_host_buffers_nvhe(struct kvm_vcpu *vcpu);


void __fpsimd_save_state(struct user_fpsimd_state *fp_regs);
void __fpsimd_restore_state(struct user_fpsimd_state *fp_regs);
bool __fpsimd_enabled(void);

void activate_traps_vhe_load(struct kvm_vcpu *vcpu);
void deactivate_traps_vhe_put(void);

u64 __guest_enter(struct kvm_vcpu *vcpu, struct kvm_cpu_context *host_ctxt);
void __noreturn __hyp_do_panic(unsigned long, ...);

#endif /* __ARM64_KVM_HYP_H__ */

=======
#endif

void __debug_switch_to_guest(struct kvm_vcpu *vcpu);
void __debug_switch_to_host(struct kvm_vcpu *vcpu);

#ifdef __KVM_NVHE_HYPERVISOR__
void __debug_save_host_buffers_nvhe(struct kvm_vcpu *vcpu);
void __debug_restore_host_buffers_nvhe(struct kvm_vcpu *vcpu);
#endif

void __fpsimd_save_state(struct user_fpsimd_state *fp_regs);
void __fpsimd_restore_state(struct user_fpsimd_state *fp_regs);
void __sve_restore_state(void *sve_pffr, u32 *fpsr);

#ifndef __KVM_NVHE_HYPERVISOR__
void activate_traps_vhe_load(struct kvm_vcpu *vcpu);
void deactivate_traps_vhe_put(struct kvm_vcpu *vcpu);
#endif

u64 __guest_enter(struct kvm_vcpu *vcpu);

bool kvm_host_psci_handler(struct kvm_cpu_context *host_ctxt);

#ifdef __KVM_NVHE_HYPERVISOR__
void __noreturn __hyp_do_panic(struct kvm_cpu_context *host_ctxt, u64 spsr,
			       u64 elr, u64 par);
#endif

#ifdef __KVM_NVHE_HYPERVISOR__
void __pkvm_init_switch_pgd(phys_addr_t phys, unsigned long size,
			    phys_addr_t pgd, void *sp, void *cont_fn);
int __pkvm_init(phys_addr_t phys, unsigned long size, unsigned long nr_cpus,
		unsigned long *per_cpu_base, u32 hyp_va_bits);
void __noreturn __host_enter(struct kvm_cpu_context *host_ctxt);
#endif

extern u64 kvm_nvhe_sym(id_aa64pfr0_el1_sys_val);
extern u64 kvm_nvhe_sym(id_aa64pfr1_el1_sys_val);
extern u64 kvm_nvhe_sym(id_aa64isar0_el1_sys_val);
extern u64 kvm_nvhe_sym(id_aa64isar1_el1_sys_val);
extern u64 kvm_nvhe_sym(id_aa64mmfr0_el1_sys_val);
extern u64 kvm_nvhe_sym(id_aa64mmfr1_el1_sys_val);
extern u64 kvm_nvhe_sym(id_aa64mmfr2_el1_sys_val);

extern unsigned long kvm_nvhe_sym(__icache_flags);
extern bool kvm_nvhe_sym(smccc_trng_available);
#endif /* __ARM64_KVM_HYP_H__ */
>>>>>>> upstream/android-13
