# SPDX-License-Identifier: GPL-2.0

obj-$(CONFIG_NETDEVSIM) += netdevsim.o

netdevsim-objs := \
<<<<<<< HEAD
	netdev.o \
=======
	netdev.o dev.o ethtool.o fib.o bus.o health.o udp_tunnels.o
>>>>>>> upstream/android-13

ifeq ($(CONFIG_BPF_SYSCALL),y)
netdevsim-objs += \
	bpf.o
endif

<<<<<<< HEAD
ifneq ($(CONFIG_NET_DEVLINK),)
netdevsim-objs += devlink.o fib.o
endif

ifneq ($(CONFIG_XFRM_OFFLOAD),)
netdevsim-objs += ipsec.o
endif
=======
ifneq ($(CONFIG_XFRM_OFFLOAD),)
netdevsim-objs += ipsec.o
endif

ifneq ($(CONFIG_PSAMPLE),)
netdevsim-objs += psample.o
endif
>>>>>>> upstream/android-13
