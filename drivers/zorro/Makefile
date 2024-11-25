# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the Zorro bus specific drivers.
#

obj-$(CONFIG_ZORRO)	+= zorro.o zorro-driver.o zorro-sysfs.o
obj-$(CONFIG_PROC_FS)	+= proc.o
obj-$(CONFIG_ZORRO_NAMES) +=  names.o

<<<<<<< HEAD
hostprogs-y 		:= gen-devlist
=======
hostprogs		:= gen-devlist
>>>>>>> upstream/android-13

# Files generated that shall be removed upon make clean
clean-files := devlist.h

# Dependencies on generated files need to be listed explicitly
$(obj)/names.o: $(obj)/devlist.h

# And that's how to generate them
quiet_cmd_devlist = DEVLIST $@
      cmd_devlist = ( cd $(obj); ./gen-devlist ) < $<
$(obj)/devlist.h: $(src)/zorro.ids $(obj)/gen-devlist
	$(call cmd,devlist)

