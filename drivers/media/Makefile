# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the kernel multimedia device drivers.
#

<<<<<<< HEAD
media-objs	:= media-device.o media-devnode.o media-entity.o \
		   media-request.o

=======
>>>>>>> upstream/android-13
#
# I2C drivers should come before other drivers, otherwise they'll fail
# when compiled as builtin drivers
#
obj-y += i2c/ tuners/
obj-$(CONFIG_DVB_CORE)  += dvb-frontends/

#
<<<<<<< HEAD
# Now, let's link-in the media core
#
ifeq ($(CONFIG_MEDIA_CONTROLLER),y)
  obj-$(CONFIG_MEDIA_SUPPORT) += media.o
=======
# Now, let's link-in the media controller core
#
ifeq ($(CONFIG_MEDIA_CONTROLLER),y)
  obj-$(CONFIG_MEDIA_SUPPORT) += mc/
>>>>>>> upstream/android-13
endif

obj-$(CONFIG_VIDEO_DEV) += v4l2-core/
obj-$(CONFIG_DVB_CORE)  += dvb-core/

# There are both core and drivers at RC subtree - merge before drivers
obj-y += rc/

obj-$(CONFIG_CEC_CORE) += cec/

#
# Finally, merge the drivers that require the core
#

<<<<<<< HEAD
obj-y += common/ platform/ pci/ usb/ mmc/ firewire/ spi/
=======
obj-y += common/ platform/ pci/ usb/ mmc/ firewire/ spi/ test-drivers/
>>>>>>> upstream/android-13
obj-$(CONFIG_VIDEO_DEV) += radio/

