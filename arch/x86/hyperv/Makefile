<<<<<<< HEAD
obj-y			:= hv_init.o mmu.o nested.o
obj-$(CONFIG_X86_64)	+= hv_apic.o
=======
# SPDX-License-Identifier: GPL-2.0-only
obj-y			:= hv_init.o mmu.o nested.o irqdomain.o
obj-$(CONFIG_X86_64)	+= hv_apic.o hv_proc.o

ifdef CONFIG_X86_64
obj-$(CONFIG_PARAVIRT_SPINLOCKS)	+= hv_spinlock.o
endif
>>>>>>> upstream/android-13
