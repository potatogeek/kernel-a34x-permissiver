<<<<<<< HEAD
# 
# Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
# Licensed under the GPL
=======
# SPDX-License-Identifier: GPL-2.0
# 
# Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
>>>>>>> upstream/android-13
#

# Don't instrument UML-specific code
KCOV_INSTRUMENT                := n

<<<<<<< HEAD
obj-y = aio.o execvp.o file.o helper.o irq.o main.o mem.o process.o \
	registers.o sigio.o signal.o start_up.o time.o tty.o \
	umid.o user_syms.o util.o drivers/ skas/

obj-$(CONFIG_ARCH_REUSE_HOST_VSYSCALL_AREA) += elf_aux.o

USER_OBJS := $(user-objs-y) aio.o elf_aux.o execvp.o file.o helper.o irq.o \
	main.o mem.o process.o registers.o sigio.o signal.o start_up.o time.o \
	tty.o umid.o util.o

HAVE_AIO_ABI := $(shell [ -r /usr/include/linux/aio_abi.h ] && \
	echo -DHAVE_AIO_ABI )
CFLAGS_aio.o += $(HAVE_AIO_ABI)

=======
obj-y = execvp.o file.o helper.o irq.o main.o mem.o process.o \
	registers.o sigio.o signal.o start_up.o time.o tty.o \
	umid.o user_syms.o util.o drivers/ skas/

CFLAGS_signal.o += -Wframe-larger-than=4096

obj-$(CONFIG_ARCH_REUSE_HOST_VSYSCALL_AREA) += elf_aux.o

USER_OBJS := $(user-objs-y) elf_aux.o execvp.o file.o helper.o irq.o \
	main.o mem.o process.o registers.o sigio.o signal.o start_up.o time.o \
	tty.o umid.o util.o

>>>>>>> upstream/android-13
include arch/um/scripts/Makefile.rules
