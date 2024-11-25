# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the x86 low level entry code
#

<<<<<<< HEAD
OBJECT_FILES_NON_STANDARD_entry_64_compat.o := y

CFLAGS_syscall_64.o		+= $(call cc-option,-Wno-override-init,)
CFLAGS_syscall_32.o		+= $(call cc-option,-Wno-override-init,)
=======
KASAN_SANITIZE := n
UBSAN_SANITIZE := n
KCOV_INSTRUMENT := n

CFLAGS_REMOVE_common.o		= $(CC_FLAGS_FTRACE)

CFLAGS_common.o			+= -fno-stack-protector

>>>>>>> upstream/android-13
obj-y				:= entry_$(BITS).o thunk_$(BITS).o syscall_$(BITS).o
obj-y				+= common.o

obj-y				+= vdso/
obj-y				+= vsyscall/

obj-$(CONFIG_IA32_EMULATION)	+= entry_64_compat.o syscall_32.o
<<<<<<< HEAD
=======
obj-$(CONFIG_X86_X32_ABI)	+= syscall_x32.o
>>>>>>> upstream/android-13

