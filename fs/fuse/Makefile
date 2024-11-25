<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
#
# Makefile for the FUSE filesystem.
#

obj-$(CONFIG_FUSE_FS) += fuse.o
obj-$(CONFIG_CUSE) += cuse.o
<<<<<<< HEAD

fuse-objs := dev.o dir.o file.o inode.o control.o xattr.o acl.o
fuse-objs += passthrough.o
=======
obj-$(CONFIG_VIRTIO_FS) += virtiofs.o

fuse-y := dev.o dir.o file.o inode.o control.o xattr.o acl.o readdir.o ioctl.o
fuse-y += passthrough.o
fuse-$(CONFIG_FUSE_DAX) += dax.o

virtiofs-y := virtio_fs.o
>>>>>>> upstream/android-13
