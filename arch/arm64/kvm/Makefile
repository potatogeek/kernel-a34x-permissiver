# SPDX-License-Identifier: GPL-2.0
#
# Makefile for Kernel-based Virtual Machine module
#

<<<<<<< HEAD
ccflags-y += -Iarch/arm64/kvm -Ivirt/kvm/arm/vgic
CFLAGS_arm.o := -I.
CFLAGS_mmu.o := -I.

KVM=../../../virt/kvm

obj-$(CONFIG_KVM_ARM_HOST) += kvm.o
obj-$(CONFIG_KVM_ARM_HOST) += hyp/

kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/kvm_main.o $(KVM)/coalesced_mmio.o $(KVM)/eventfd.o $(KVM)/vfio.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/arm.o $(KVM)/arm/mmu.o $(KVM)/arm/mmio.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/psci.o $(KVM)/arm/perf.o

kvm-$(CONFIG_KVM_ARM_HOST) += inject_fault.o regmap.o va_layout.o
kvm-$(CONFIG_KVM_ARM_HOST) += hyp.o hyp-init.o handle_exit.o
kvm-$(CONFIG_KVM_ARM_HOST) += guest.o debug.o reset.o sys_regs.o sys_regs_generic_v8.o
kvm-$(CONFIG_KVM_ARM_HOST) += vgic-sys-reg-v3.o fpsimd.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/aarch32.o

kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/vgic/vgic.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/vgic/vgic-init.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/vgic/vgic-irqfd.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/vgic/vgic-v2.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/vgic/vgic-v3.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/vgic/vgic-v4.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/vgic/vgic-mmio.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/vgic/vgic-mmio-v2.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/vgic/vgic-mmio-v3.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/vgic/vgic-kvm-device.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/vgic/vgic-its.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/vgic/vgic-debug.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/irqchip.o
kvm-$(CONFIG_KVM_ARM_HOST) += $(KVM)/arm/arch_timer.o
kvm-$(CONFIG_KVM_ARM_PMU) += $(KVM)/arm/pmu.o
=======
ccflags-y += -I $(srctree)/$(src)

KVM=../../../virt/kvm

obj-$(CONFIG_KVM) += kvm.o
obj-$(CONFIG_KVM) += hyp/ iommu/

kvm-y := $(KVM)/kvm_main.o $(KVM)/coalesced_mmio.o $(KVM)/eventfd.o \
	 $(KVM)/vfio.o $(KVM)/irqchip.o $(KVM)/binary_stats.o iommu.o \
	 arm.o mmu.o mmio.o psci.o perf.o hypercalls.o pvtime.o \
	 inject_fault.o va_layout.o handle_exit.o \
	 guest.o debug.o reset.o sys_regs.o \
	 vgic-sys-reg-v3.o fpsimd.o pmu.o pkvm.o \
	 arch_timer.o trng.o\
	 vgic/vgic.o vgic/vgic-init.o \
	 vgic/vgic-irqfd.o vgic/vgic-v2.o \
	 vgic/vgic-v3.o vgic/vgic-v4.o \
	 vgic/vgic-mmio.o vgic/vgic-mmio-v2.o \
	 vgic/vgic-mmio-v3.o vgic/vgic-kvm-device.o \
	 vgic/vgic-its.o vgic/vgic-debug.o

kvm-$(CONFIG_HW_PERF_EVENTS)  += pmu-emul.o

always-y := hyp_constants.h hyp-constants.s

define rule_gen_hyp_constants
	$(call filechk,offsets,__HYP_CONSTANTS_H__)
endef

CFLAGS_hyp-constants.o = -I $(srctree)/$(src)/hyp/include -D__KVM_NVHE_HYPERVISOR__=1
$(obj)/hyp-constants.s: $(src)/hyp/hyp-constants.c FORCE
	$(call if_changed_dep,cc_s_c)

$(obj)/hyp_constants.h: $(obj)/hyp-constants.s FORCE
	$(call if_changed_rule,gen_hyp_constants)

obj-kvm := $(addprefix $(obj)/, $(kvm-y))
$(obj-kvm): $(obj)/hyp_constants.h
>>>>>>> upstream/android-13
