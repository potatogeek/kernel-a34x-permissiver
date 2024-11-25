# SPDX-License-Identifier: GPL-2.0
bttv-objs      :=      bttv-driver.o bttv-cards.o bttv-if.o \
		       bttv-risc.o bttv-vbi.o bttv-i2c.o bttv-gpio.o \
		       bttv-input.o bttv-audio-hook.o btcx-risc.o

obj-$(CONFIG_VIDEO_BT848) += bttv.o
obj-$(CONFIG_DVB_BT8XX) += bt878.o dvb-bt8xx.o dst.o dst_ca.o

<<<<<<< HEAD
ccflags-y += -Idrivers/media/dvb-frontends
ccflags-y += -Idrivers/media/common
ccflags-y += -Idrivers/media/tuners
=======
ccflags-y += -I $(srctree)/drivers/media/dvb-frontends
ccflags-y += -I $(srctree)/drivers/media/tuners
>>>>>>> upstream/android-13
