# SPDX-License-Identifier: GPL-2.0
#
# Makefile for Red Hat Linux AFS client.
#

afs-cache-$(CONFIG_AFS_FSCACHE) := cache.o

kafs-y := \
	$(afs-cache-y) \
	addr_list.o \
	callback.o \
	cell.o \
	cmservice.o \
	dir.o \
	dir_edit.o \
<<<<<<< HEAD
=======
	dir_silly.o \
>>>>>>> upstream/android-13
	dynroot.o \
	file.o \
	flock.o \
	fsclient.o \
<<<<<<< HEAD
=======
	fs_operation.o \
	fs_probe.o \
>>>>>>> upstream/android-13
	inode.o \
	main.o \
	misc.o \
	mntpt.o \
	rotate.o \
	rxrpc.o \
	security.o \
	server.o \
	server_list.o \
	super.o \
<<<<<<< HEAD
	netdevices.o \
	vlclient.o \
	volume.o \
	write.o \
	xattr.o
=======
	vlclient.o \
	vl_alias.o \
	vl_list.o \
	vl_probe.o \
	vl_rotate.o \
	volume.o \
	write.o \
	xattr.o \
	yfsclient.o
>>>>>>> upstream/android-13

kafs-$(CONFIG_PROC_FS) += proc.o
obj-$(CONFIG_AFS_FS)  := kafs.o
