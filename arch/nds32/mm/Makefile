<<<<<<< HEAD
obj-y				:= extable.o tlb.o \
				   fault.o init.o ioremap.o mmap.o \
                                   mm-nds32.o cacheflush.o proc.o

obj-$(CONFIG_ALIGNMENT_TRAP)	+= alignment.o
obj-$(CONFIG_HIGHMEM)           += highmem.o
=======
# SPDX-License-Identifier: GPL-2.0-only
obj-y				:= extable.o tlb.o fault.o init.o mmap.o \
                                   mm-nds32.o cacheflush.o proc.o

obj-$(CONFIG_ALIGNMENT_TRAP)	+= alignment.o
>>>>>>> upstream/android-13

ifdef CONFIG_FUNCTION_TRACER
CFLAGS_REMOVE_proc.o     = $(CC_FLAGS_FTRACE)
endif
CFLAGS_proc.o              += -fomit-frame-pointer
