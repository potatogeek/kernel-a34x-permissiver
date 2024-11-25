# SPDX-License-Identifier: GPL-2.0

<<<<<<< HEAD
=======
ccflags-y				+= -I$(src)

>>>>>>> upstream/android-13
obj-$(CONFIG_NVME_TARGET)		+= nvmet.o
obj-$(CONFIG_NVME_TARGET_LOOP)		+= nvme-loop.o
obj-$(CONFIG_NVME_TARGET_RDMA)		+= nvmet-rdma.o
obj-$(CONFIG_NVME_TARGET_FC)		+= nvmet-fc.o
obj-$(CONFIG_NVME_TARGET_FCLOOP)	+= nvme-fcloop.o
<<<<<<< HEAD

nvmet-y		+= core.o configfs.o admin-cmd.o fabrics-cmd.o \
			discovery.o io-cmd-file.o io-cmd-bdev.o
=======
obj-$(CONFIG_NVME_TARGET_TCP)		+= nvmet-tcp.o

nvmet-y		+= core.o configfs.o admin-cmd.o fabrics-cmd.o \
			discovery.o io-cmd-file.o io-cmd-bdev.o
nvmet-$(CONFIG_NVME_TARGET_PASSTHRU)	+= passthru.o
nvmet-$(CONFIG_BLK_DEV_ZONED)		+= zns.o
>>>>>>> upstream/android-13
nvme-loop-y	+= loop.o
nvmet-rdma-y	+= rdma.o
nvmet-fc-y	+= fc.o
nvme-fcloop-y	+= fcloop.o
<<<<<<< HEAD
=======
nvmet-tcp-y	+= tcp.o
nvmet-$(CONFIG_TRACING)	+= trace.o
>>>>>>> upstream/android-13
