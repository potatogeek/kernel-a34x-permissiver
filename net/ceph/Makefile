# SPDX-License-Identifier: GPL-2.0
#
# Makefile for CEPH filesystem.
#
obj-$(CONFIG_CEPH_LIB) += libceph.o

libceph-y := ceph_common.o messenger.o msgpool.o buffer.o pagelist.o \
<<<<<<< HEAD
	mon_client.o \
=======
	mon_client.o decode.o \
>>>>>>> upstream/android-13
	cls_lock_client.o \
	osd_client.o osdmap.o crush/crush.o crush/mapper.o crush/hash.o \
	striper.o \
	debugfs.o \
	auth.o auth_none.o \
	crypto.o armor.o \
	auth_x.o \
<<<<<<< HEAD
	ceph_fs.o ceph_strings.o ceph_hash.o \
	pagevec.o snapshot.o string_table.o
=======
	ceph_strings.o ceph_hash.o \
	pagevec.o snapshot.o string_table.o \
	messenger_v1.o messenger_v2.o
>>>>>>> upstream/android-13
