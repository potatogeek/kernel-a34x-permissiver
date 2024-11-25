# SPDX-License-Identifier: GPL-2.0
# Makefile for the Linux sound card driver
#

obj-$(CONFIG_SOUND) += soundcore.o
obj-$(CONFIG_DMASOUND) += oss/dmasound/
obj-$(CONFIG_SND) += core/ i2c/ drivers/ isa/ pci/ ppc/ arm/ sh/ synth/ usb/ \
<<<<<<< HEAD
	firewire/ sparc/ spi/ parisc/ pcmcia/ mips/ soc/ atmel/ hda/ x86/ xen/
=======
	firewire/ sparc/ spi/ parisc/ pcmcia/ mips/ soc/ atmel/ hda/ x86/ xen/ \
	virtio/
>>>>>>> upstream/android-13
obj-$(CONFIG_SND_AOA) += aoa/

# This one must be compilable even if sound is configured out
obj-$(CONFIG_AC97_BUS) += ac97_bus.o
obj-$(CONFIG_AC97_BUS_NEW) += ac97/

ifeq ($(CONFIG_SND),y)
  obj-y += last.o
endif

soundcore-objs  := sound_core.o
