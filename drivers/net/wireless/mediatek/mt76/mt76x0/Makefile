<<<<<<< HEAD
obj-$(CONFIG_MT76x0U)    += mt76x0.o

mt76x0-objs	= \
	usb.o init.o main.o mcu.o trace.o dma.o eeprom.o phy.o \
	mac.o util.o debugfs.o tx.o core.o
# ccflags-y := -DDEBUG
CFLAGS_trace.o := -I$(src)
=======
# SPDX-License-Identifier: GPL-2.0-only
obj-$(CONFIG_MT76x0U) += mt76x0u.o
obj-$(CONFIG_MT76x0E) += mt76x0e.o
obj-$(CONFIG_MT76x0_COMMON) += mt76x0-common.o

mt76x0-common-y := init.o main.o eeprom.o phy.o

mt76x0u-y := usb.o usb_mcu.o
mt76x0e-y := pci.o pci_mcu.o

# ccflags-y := -DDEBUG
>>>>>>> upstream/android-13
