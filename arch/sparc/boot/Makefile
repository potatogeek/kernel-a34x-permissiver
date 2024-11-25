# SPDX-License-Identifier: GPL-2.0
# Makefile for the Sparc boot stuff.
#
# Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
# Copyright (C) 1997,1998 Jakub Jelinek (jj@ultra.linux.cz)

ROOT_IMG	:= /usr/src/root.img
ELFTOAOUT	:= elftoaout

<<<<<<< HEAD
hostprogs-y	:= piggyback
=======
hostprogs	:= piggyback
>>>>>>> upstream/android-13
targets		:= tftpboot.img image zImage vmlinux.aout
clean-files	:= System.map

quiet_cmd_elftoaout	= ELFTOAOUT $@
      cmd_elftoaout	= $(ELFTOAOUT) $(obj)/image -o $@
quiet_cmd_piggy		= PIGGY   $@
      cmd_piggy		= $(obj)/piggyback $(BITS) $@ System.map $(ROOT_IMG)
quiet_cmd_strip		= STRIP   $@
      cmd_strip		= $(STRIP) -R .comment -R .note -K sun4u_init -K _end -K _start $< -o $@

ifeq ($(CONFIG_SPARC64),y)

# Actual linking

<<<<<<< HEAD
$(obj)/zImage: $(obj)/image
=======
$(obj)/zImage: $(obj)/image FORCE
>>>>>>> upstream/android-13
	$(call if_changed,gzip)
	@echo '  kernel: $@ is ready'

$(obj)/vmlinux.aout: vmlinux FORCE
	$(call if_changed,elftoaout)
	@echo '  kernel: $@ is ready'
else

<<<<<<< HEAD
$(obj)/zImage: $(obj)/image
=======
$(obj)/zImage: $(obj)/image FORCE
>>>>>>> upstream/android-13
	$(call if_changed,strip)
	@echo '  kernel: $@ is ready'

# The following lines make a readable image for U-Boot.
#  uImage   - Binary file read by U-boot
#  uImage.o - object file of uImage for loading with a
#             flash programmer understanding ELF.

OBJCOPYFLAGS_image.bin := -S -O binary -R .note -R .comment
$(obj)/image.bin: $(obj)/image FORCE
	$(call if_changed,objcopy)

<<<<<<< HEAD
$(obj)/image.gz: $(obj)/image.bin
=======
$(obj)/image.gz: $(obj)/image.bin FORCE
>>>>>>> upstream/android-13
	$(call if_changed,gzip)

UIMAGE_LOADADDR = $(CONFIG_UBOOT_LOAD_ADDR)
UIMAGE_ENTRYADDR = $(CONFIG_UBOOT_ENTRY_ADDR)
UIMAGE_COMPRESSION = gzip

quiet_cmd_uimage.o = UIMAGE.O $@
      cmd_uimage.o = $(LD) -Tdata $(CONFIG_UBOOT_FLASH_ADDR) \
                     -r -b binary $@ -o $@.o

targets += uImage
<<<<<<< HEAD
$(obj)/uImage: $(obj)/image.gz
=======
$(obj)/uImage: $(obj)/image.gz FORCE
>>>>>>> upstream/android-13
	$(call if_changed,uimage)
	$(call if_changed,uimage.o)
	@echo '  Image $@ is ready'

endif

$(obj)/image: vmlinux FORCE
	$(call if_changed,strip)
	@echo '  kernel: $@ is ready'

$(obj)/tftpboot.img: $(obj)/image $(obj)/piggyback System.map $(ROOT_IMG) FORCE
	$(call if_changed,elftoaout)
	$(call if_changed,piggy)
<<<<<<< HEAD

install:
	sh $(srctree)/$(src)/install.sh $(KERNELRELEASE) $(obj)/zImage \
		System.map "$(INSTALL_PATH)"
=======
>>>>>>> upstream/android-13
