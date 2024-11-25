# SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
OBJECT_FILES_NON_STANDARD_xen-asm_$(BITS).o := y
OBJECT_FILES_NON_STANDARD_xen-pvh.o := y
=======
>>>>>>> upstream/android-13

ifdef CONFIG_FUNCTION_TRACER
# Do not profile debug and lowlevel utilities
CFLAGS_REMOVE_spinlock.o = -pg
CFLAGS_REMOVE_time.o = -pg
CFLAGS_REMOVE_irq.o = -pg
endif

# Make sure early boot has no stackprotector
<<<<<<< HEAD
nostackp := $(call cc-option, -fno-stack-protector)
CFLAGS_enlighten_pv.o		:= $(nostackp)
CFLAGS_mmu_pv.o		:= $(nostackp)

obj-y		:= enlighten.o multicalls.o mmu.o irq.o \
			time.o xen-asm.o xen-asm_$(BITS).o \
			grant-table.o suspend.o platform-pci-unplug.o

obj-$(CONFIG_XEN_PVHVM)		+= enlighten_hvm.o mmu_hvm.o suspend_hvm.o
obj-$(CONFIG_XEN_PV)			+= setup.o apic.o pmu.o suspend_pv.o \
						p2m.o enlighten_pv.o mmu_pv.o
obj-$(CONFIG_XEN_PVH)			+= enlighten_pvh.o

obj-$(CONFIG_EVENT_TRACING) += trace.o
=======
CFLAGS_enlighten_pv.o		:= -fno-stack-protector
CFLAGS_mmu_pv.o			:= -fno-stack-protector

obj-y				+= enlighten.o
obj-y				+= mmu.o
obj-y				+= time.o
obj-y				+= grant-table.o
obj-y				+= suspend.o

obj-$(CONFIG_XEN_PVHVM)		+= enlighten_hvm.o
obj-$(CONFIG_XEN_PVHVM)		+= mmu_hvm.o
obj-$(CONFIG_XEN_PVHVM)		+= suspend_hvm.o
obj-$(CONFIG_XEN_PVHVM)		+= platform-pci-unplug.o

obj-$(CONFIG_XEN_PV)		+= setup.o
obj-$(CONFIG_XEN_PV)		+= apic.o
obj-$(CONFIG_XEN_PV)		+= pmu.o
obj-$(CONFIG_XEN_PV)		+= suspend_pv.o
obj-$(CONFIG_XEN_PV)		+= p2m.o
obj-$(CONFIG_XEN_PV)		+= enlighten_pv.o
obj-$(CONFIG_XEN_PV)		+= mmu_pv.o
obj-$(CONFIG_XEN_PV)		+= irq.o
obj-$(CONFIG_XEN_PV)		+= multicalls.o
obj-$(CONFIG_XEN_PV)		+= xen-asm.o

obj-$(CONFIG_XEN_PVH)		+= enlighten_pvh.o

obj-$(CONFIG_EVENT_TRACING)	+= trace.o
>>>>>>> upstream/android-13

obj-$(CONFIG_SMP)		+= smp.o
obj-$(CONFIG_XEN_PV_SMP)  	+= smp_pv.o
obj-$(CONFIG_XEN_PVHVM_SMP)  	+= smp_hvm.o
<<<<<<< HEAD
obj-$(CONFIG_PARAVIRT_SPINLOCKS)+= spinlock.o
obj-$(CONFIG_XEN_DEBUG_FS)	+= debugfs.o
obj-$(CONFIG_XEN_DOM0)		+= vga.o
obj-$(CONFIG_SWIOTLB_XEN)	+= pci-swiotlb-xen.o
obj-$(CONFIG_XEN_EFI)		+= efi.o
obj-$(CONFIG_XEN_PVH)	 	+= xen-pvh.o
=======

obj-$(CONFIG_PARAVIRT_SPINLOCKS)+= spinlock.o

obj-$(CONFIG_XEN_DEBUG_FS)	+= debugfs.o

obj-$(CONFIG_XEN_PV_DOM0)	+= vga.o

obj-$(CONFIG_SWIOTLB_XEN)	+= pci-swiotlb-xen.o

obj-$(CONFIG_XEN_EFI)		+= efi.o
>>>>>>> upstream/android-13
