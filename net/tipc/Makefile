# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the Linux TIPC layer
#

obj-$(CONFIG_TIPC) := tipc.o

tipc-y	+= addr.o bcast.o bearer.o \
	   core.o link.o discover.o msg.o  \
	   name_distr.o  subscr.o monitor.o name_table.o net.o  \
	   netlink.o netlink_compat.o node.o socket.o eth_media.o \
<<<<<<< HEAD
	   topsrv.o socket.o group.o
=======
	   topsrv.o group.o trace.o

CFLAGS_trace.o += -I$(src)
>>>>>>> upstream/android-13

tipc-$(CONFIG_TIPC_MEDIA_UDP)	+= udp_media.o
tipc-$(CONFIG_TIPC_MEDIA_IB)	+= ib_media.o
tipc-$(CONFIG_SYSCTL)		+= sysctl.o
<<<<<<< HEAD


obj-$(CONFIG_TIPC_DIAG)	+= diag.o

tipc_diag-y	:= diag.o
=======
tipc-$(CONFIG_TIPC_CRYPTO)	+= crypto.o


obj-$(CONFIG_TIPC_DIAG)	+= diag.o
>>>>>>> upstream/android-13
