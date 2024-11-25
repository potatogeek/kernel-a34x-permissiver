# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the OneNAND MTD
#

# Core functionality.
obj-$(CONFIG_MTD_ONENAND)		+= onenand.o

# Board specific.
obj-$(CONFIG_MTD_ONENAND_GENERIC)	+= generic.o
<<<<<<< HEAD
obj-$(CONFIG_MTD_ONENAND_OMAP2)		+= omap2.o
obj-$(CONFIG_MTD_ONENAND_SAMSUNG)       += samsung.o
=======
obj-$(CONFIG_MTD_ONENAND_OMAP2)		+= onenand_omap2.o
obj-$(CONFIG_MTD_ONENAND_SAMSUNG)       += onenand_samsung.o
>>>>>>> upstream/android-13

onenand-objs = onenand_base.o onenand_bbt.o
