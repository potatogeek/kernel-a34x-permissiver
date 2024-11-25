# SPDX-License-Identifier: GPL-2.0
# Any varying coverage in these files is non-deterministic
# and is generally not a function of system call inputs.
KCOV_INSTRUMENT := n

<<<<<<< HEAD
=======
ifeq ($(CONFIG_KCSAN),y)
KBUILD_CFLAGS += -g -fno-omit-frame-pointer
endif

>>>>>>> upstream/android-13
obj-y += update.o sync.o
obj-$(CONFIG_TREE_SRCU) += srcutree.o
obj-$(CONFIG_TINY_SRCU) += srcutiny.o
obj-$(CONFIG_RCU_TORTURE_TEST) += rcutorture.o
<<<<<<< HEAD
obj-$(CONFIG_RCU_PERF_TEST) += rcuperf.o
obj-$(CONFIG_TREE_RCU) += tree.o
obj-$(CONFIG_PREEMPT_RCU) += tree.o
=======
obj-$(CONFIG_RCU_SCALE_TEST) += rcuscale.o
obj-$(CONFIG_RCU_REF_SCALE_TEST) += refscale.o
obj-$(CONFIG_TREE_RCU) += tree.o
>>>>>>> upstream/android-13
obj-$(CONFIG_TINY_RCU) += tiny.o
obj-$(CONFIG_RCU_NEED_SEGCBLIST) += rcu_segcblist.o
