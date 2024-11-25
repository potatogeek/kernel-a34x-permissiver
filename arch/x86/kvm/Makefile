# SPDX-License-Identifier: GPL-2.0

<<<<<<< HEAD
ccflags-y += -Iarch/x86/kvm

CFLAGS_x86.o := -I.
CFLAGS_svm.o := -I.
CFLAGS_vmx.o := -I.
=======
ccflags-y += -I $(srctree)/arch/x86/kvm
ccflags-$(CONFIG_KVM_WERROR) += -Werror

ifeq ($(CONFIG_FRAME_POINTER),y)
OBJECT_FILES_NON_STANDARD_vmenter.o := y
endif
>>>>>>> upstream/android-13

KVM := ../../../virt/kvm

kvm-y			+= $(KVM)/kvm_main.o $(KVM)/coalesced_mmio.o \
<<<<<<< HEAD
				$(KVM)/eventfd.o $(KVM)/irqchip.o $(KVM)/vfio.o
kvm-$(CONFIG_KVM_ASYNC_PF)	+= $(KVM)/async_pf.o

kvm-y			+= x86.o mmu.o emulate.o i8259.o irq.o lapic.o \
			   i8254.o ioapic.o irq_comm.o cpuid.o pmu.o mtrr.o \
			   hyperv.o page_track.o debugfs.o

kvm-intel-y		+= vmx.o pmu_intel.o
kvm-amd-y		+= svm.o pmu_amd.o
=======
				$(KVM)/eventfd.o $(KVM)/irqchip.o $(KVM)/vfio.o \
				$(KVM)/dirty_ring.o $(KVM)/binary_stats.o
kvm-$(CONFIG_KVM_ASYNC_PF)	+= $(KVM)/async_pf.o

kvm-y			+= x86.o emulate.o i8259.o irq.o lapic.o \
			   i8254.o ioapic.o irq_comm.o cpuid.o pmu.o mtrr.o \
			   hyperv.o debugfs.o mmu/mmu.o mmu/page_track.o \
			   mmu/spte.o

ifdef CONFIG_HYPERV
kvm-y			+= kvm_onhyperv.o
endif

kvm-$(CONFIG_X86_64) += mmu/tdp_iter.o mmu/tdp_mmu.o
kvm-$(CONFIG_KVM_XEN)	+= xen.o

kvm-intel-y		+= vmx/vmx.o vmx/vmenter.o vmx/pmu_intel.o vmx/vmcs12.o \
			   vmx/evmcs.o vmx/nested.o vmx/posted_intr.o
kvm-intel-$(CONFIG_X86_SGX_KVM)	+= vmx/sgx.o

kvm-amd-y		+= svm/svm.o svm/vmenter.o svm/pmu.o svm/nested.o svm/avic.o svm/sev.o

ifdef CONFIG_HYPERV
kvm-amd-y		+= svm/svm_onhyperv.o
endif
>>>>>>> upstream/android-13

obj-$(CONFIG_KVM)	+= kvm.o
obj-$(CONFIG_KVM_INTEL)	+= kvm-intel.o
obj-$(CONFIG_KVM_AMD)	+= kvm-amd.o
