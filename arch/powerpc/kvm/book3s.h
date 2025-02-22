<<<<<<< HEAD
/*
 * Copyright IBM Corporation, 2013
 * Author Aneesh Kumar K.V <aneesh.kumar@linux.vnet.ibm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License or (at your optional) any later version of the license.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright IBM Corporation, 2013
 * Author Aneesh Kumar K.V <aneesh.kumar@linux.vnet.ibm.com>
>>>>>>> upstream/android-13
 */

#ifndef __POWERPC_KVM_BOOK3S_H__
#define __POWERPC_KVM_BOOK3S_H__

extern void kvmppc_core_flush_memslot_hv(struct kvm *kvm,
					 struct kvm_memory_slot *memslot);
<<<<<<< HEAD
extern int kvm_unmap_hva_range_hv(struct kvm *kvm, unsigned long start,
				  unsigned long end);
extern int kvm_age_hva_hv(struct kvm *kvm, unsigned long start,
			  unsigned long end);
extern int kvm_test_age_hva_hv(struct kvm *kvm, unsigned long hva);
extern void kvm_set_spte_hva_hv(struct kvm *kvm, unsigned long hva, pte_t pte);

extern void kvmppc_mmu_destroy_pr(struct kvm_vcpu *vcpu);
extern int kvmppc_core_emulate_op_pr(struct kvm_run *run, struct kvm_vcpu *vcpu,
=======
extern bool kvm_unmap_gfn_range_hv(struct kvm *kvm, struct kvm_gfn_range *range);
extern bool kvm_age_gfn_hv(struct kvm *kvm, struct kvm_gfn_range *range);
extern bool kvm_test_age_gfn_hv(struct kvm *kvm, struct kvm_gfn_range *range);
extern bool kvm_set_spte_gfn_hv(struct kvm *kvm, struct kvm_gfn_range *range);

extern int kvmppc_mmu_init_pr(struct kvm_vcpu *vcpu);
extern void kvmppc_mmu_destroy_pr(struct kvm_vcpu *vcpu);
extern int kvmppc_core_emulate_op_pr(struct kvm_vcpu *vcpu,
>>>>>>> upstream/android-13
				     unsigned int inst, int *advance);
extern int kvmppc_core_emulate_mtspr_pr(struct kvm_vcpu *vcpu,
					int sprn, ulong spr_val);
extern int kvmppc_core_emulate_mfspr_pr(struct kvm_vcpu *vcpu,
					int sprn, ulong *spr_val);
extern int kvmppc_book3s_init_pr(void);
<<<<<<< HEAD
extern void kvmppc_book3s_exit_pr(void);
=======
void kvmppc_book3s_exit_pr(void);
extern int kvmppc_handle_exit_pr(struct kvm_vcpu *vcpu, unsigned int exit_nr);
>>>>>>> upstream/android-13

#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
extern void kvmppc_emulate_tabort(struct kvm_vcpu *vcpu, int ra_val);
#else
static inline void kvmppc_emulate_tabort(struct kvm_vcpu *vcpu, int ra_val) {}
#endif

<<<<<<< HEAD
=======
extern void kvmppc_set_msr_hv(struct kvm_vcpu *vcpu, u64 msr);
extern void kvmppc_inject_interrupt_hv(struct kvm_vcpu *vcpu, int vec, u64 srr1_flags);

>>>>>>> upstream/android-13
#endif
