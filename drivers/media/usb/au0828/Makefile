# SPDX-License-Identifier: GPL-2.0
au0828-objs	:= au0828-core.o au0828-i2c.o au0828-cards.o au0828-dvb.o

ifeq ($(CONFIG_VIDEO_AU0828_V4L2),y)
  au0828-objs   += au0828-video.o au0828-vbi.o
endif

ifeq ($(CONFIG_VIDEO_AU0828_RC),y)
  au0828-objs   += au0828-input.o
endif

obj-$(CONFIG_VIDEO_AU0828) += au0828.o

<<<<<<< HEAD
ccflags-y += -Idrivers/media/tuners
ccflags-y += -Idrivers/media/dvb-frontends
=======
ccflags-y += -I $(srctree)/drivers/media/tuners
ccflags-y += -I $(srctree)/drivers/media/dvb-frontends
>>>>>>> upstream/android-13

ccflags-y += $(extra-cflags-y) $(extra-cflags-m)
