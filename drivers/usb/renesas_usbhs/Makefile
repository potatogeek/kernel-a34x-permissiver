# SPDX-License-Identifier: GPL-2.0
#
# for Renesas USB
#

obj-$(CONFIG_USB_RENESAS_USBHS)	+= renesas_usbhs.o

<<<<<<< HEAD
renesas_usbhs-y			:= common.o mod.o pipe.o fifo.o rcar2.o rcar3.o rza.o
=======
renesas_usbhs-y			:= common.o mod.o pipe.o fifo.o rcar2.o rcar3.o rza.o rza2.o
>>>>>>> upstream/android-13

ifneq ($(CONFIG_USB_RENESAS_USBHS_HCD),)
	renesas_usbhs-y		+= mod_host.o
endif

ifneq ($(CONFIG_USB_RENESAS_USBHS_UDC),)
	renesas_usbhs-y		+= mod_gadget.o
endif
