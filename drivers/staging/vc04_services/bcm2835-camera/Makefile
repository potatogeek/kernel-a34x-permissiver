# SPDX-License-Identifier: GPL-2.0
bcm2835-v4l2-$(CONFIG_VIDEO_BCM2835) := \
	bcm2835-camera.o \
<<<<<<< HEAD
	controls.o \
	mmal-vchiq.o
=======
	controls.o
>>>>>>> upstream/android-13

obj-$(CONFIG_VIDEO_BCM2835) += bcm2835-v4l2.o

ccflags-y += \
<<<<<<< HEAD
	-Idrivers/staging/vc04_services \
=======
	-I $(srctree)/$(src)/.. \
	-I $(srctree)/$(src)/../vchiq-mmal/ \
>>>>>>> upstream/android-13
	-D__VCCOREVER__=0x04000000
