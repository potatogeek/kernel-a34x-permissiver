<<<<<<< HEAD
/*
 * Copyright (C) 2012,2013 - ARM Ltd
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
 * Copyright (C) 2012,2013 - ARM Ltd
 * Author: Marc Zyngier <marc.zyngier@arm.com>
>>>>>>> upstream/android-13
 */

#ifndef __KVM_ARM_PSCI_H__
#define __KVM_ARM_PSCI_H__

#include <linux/kvm_host.h>
#include <uapi/linux/psci.h>

#define KVM_ARM_PSCI_0_1	PSCI_VERSION(0, 1)
#define KVM_ARM_PSCI_0_2	PSCI_VERSION(0, 2)
#define KVM_ARM_PSCI_1_0	PSCI_VERSION(1, 0)
<<<<<<< HEAD

#define KVM_ARM_PSCI_LATEST	KVM_ARM_PSCI_1_0
=======
#define KVM_ARM_PSCI_1_1	PSCI_VERSION(1, 1)

#define KVM_ARM_PSCI_LATEST	KVM_ARM_PSCI_1_1
>>>>>>> upstream/android-13

/*
 * We need the KVM pointer independently from the vcpu as we can call
 * this from HYP, and need to apply kern_hyp_va on it...
 */
static inline int kvm_psci_version(struct kvm_vcpu *vcpu, struct kvm *kvm)
{
	/*
	 * Our PSCI implementation stays the same across versions from
	 * v0.2 onward, only adding the few mandatory functions (such
	 * as FEATURES with 1.0) that are required by newer
	 * revisions. It is thus safe to return the latest, unless
	 * userspace has instructed us otherwise.
	 */
	if (test_bit(KVM_ARM_VCPU_PSCI_0_2, vcpu->arch.features)) {
		if (vcpu->kvm->arch.psci_version)
			return vcpu->kvm->arch.psci_version;

		return KVM_ARM_PSCI_LATEST;
	}

	return KVM_ARM_PSCI_0_1;
}


<<<<<<< HEAD
int kvm_hvc_call_handler(struct kvm_vcpu *vcpu);
=======
int kvm_psci_call(struct kvm_vcpu *vcpu);
>>>>>>> upstream/android-13

struct kvm_one_reg;

int kvm_arm_get_fw_num_regs(struct kvm_vcpu *vcpu);
int kvm_arm_copy_fw_reg_indices(struct kvm_vcpu *vcpu, u64 __user *uindices);
int kvm_arm_get_fw_reg(struct kvm_vcpu *vcpu, const struct kvm_one_reg *reg);
int kvm_arm_set_fw_reg(struct kvm_vcpu *vcpu, const struct kvm_one_reg *reg);

#endif /* __KVM_ARM_PSCI_H__ */
