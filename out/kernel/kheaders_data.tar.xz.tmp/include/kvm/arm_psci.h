

#ifndef __KVM_ARM_PSCI_H__
#define __KVM_ARM_PSCI_H__

#include <linux/kvm_host.h>
#include <uapi/linux/psci.h>

#define KVM_ARM_PSCI_0_1	PSCI_VERSION(0, 1)
#define KVM_ARM_PSCI_0_2	PSCI_VERSION(0, 2)
#define KVM_ARM_PSCI_1_0	PSCI_VERSION(1, 0)

#define KVM_ARM_PSCI_LATEST	KVM_ARM_PSCI_1_0


static inline int kvm_psci_version(struct kvm_vcpu *vcpu, struct kvm *kvm)
{
	
	if (test_bit(KVM_ARM_VCPU_PSCI_0_2, vcpu->arch.features)) {
		if (vcpu->kvm->arch.psci_version)
			return vcpu->kvm->arch.psci_version;

		return KVM_ARM_PSCI_LATEST;
	}

	return KVM_ARM_PSCI_0_1;
}


int kvm_hvc_call_handler(struct kvm_vcpu *vcpu);

struct kvm_one_reg;

int kvm_arm_get_fw_num_regs(struct kvm_vcpu *vcpu);
int kvm_arm_copy_fw_reg_indices(struct kvm_vcpu *vcpu, u64 __user *uindices);
int kvm_arm_get_fw_reg(struct kvm_vcpu *vcpu, const struct kvm_one_reg *reg);
int kvm_arm_set_fw_reg(struct kvm_vcpu *vcpu, const struct kvm_one_reg *reg);

#endif 
