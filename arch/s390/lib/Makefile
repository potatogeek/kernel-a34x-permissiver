# SPDX-License-Identifier: GPL-2.0
#
# Makefile for s390-specific library files..
#

<<<<<<< HEAD
lib-y += delay.o string.o uaccess.o find.o
obj-y += mem.o xor.o
lib-$(CONFIG_SMP) += spinlock.o
=======
lib-y += delay.o string.o uaccess.o find.o spinlock.o
obj-y += mem.o xor.o
>>>>>>> upstream/android-13
lib-$(CONFIG_KPROBES) += probes.o
lib-$(CONFIG_UPROBES) += probes.o

# Instrumenting memory accesses to __user data (in different address space)
# produce false positives
KASAN_SANITIZE_uaccess.o := n

<<<<<<< HEAD
chkbss := mem.o
include $(srctree)/arch/s390/scripts/Makefile.chkbss
=======
obj-$(CONFIG_S390_UNWIND_SELFTEST) += test_unwind.o
CFLAGS_test_unwind.o += -fno-optimize-sibling-calls

lib-$(CONFIG_FUNCTION_ERROR_INJECTION) += error-inject.o
>>>>>>> upstream/android-13
