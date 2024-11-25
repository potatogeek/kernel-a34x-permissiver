#
# arch/xtensa/boot/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
#


# KBUILD_CFLAGS used when building rest of boot (takes effect recursively)
KBUILD_CFLAGS	+= -fno-builtin -Iarch/$(ARCH)/boot/include
HOSTFLAGS	+= -Iarch/$(ARCH)/boot/include

<<<<<<< HEAD
BIG_ENDIAN	:= $(shell echo __XTENSA_EB__ | $(CC) -E - | grep -v "\#")

export ccflags-y
export BIG_ENDIAN

subdir-y	:= lib
=======
subdir-y	:= lib
targets		+= vmlinux.bin vmlinux.bin.gz
targets		+= uImage xipImage
>>>>>>> upstream/android-13

# Subdirs for the boot loader(s)

boot-$(CONFIG_XTENSA_PLATFORM_ISS)    += Image
boot-$(CONFIG_XTENSA_PLATFORM_XT2000) += Image zImage uImage
boot-$(CONFIG_XTENSA_PLATFORM_XTFPGA) += Image zImage uImage

all: $(boot-y)
Image: boot-elf
zImage: boot-redboot
uImage: $(obj)/uImage
<<<<<<< HEAD
=======
xipImage: $(obj)/xipImage
>>>>>>> upstream/android-13

boot-elf boot-redboot: $(addprefix $(obj)/,$(subdir-y))
	$(Q)$(MAKE) $(build)=$(obj)/$@ $(MAKECMDGOALS)

OBJCOPYFLAGS = --strip-all -R .comment -R .notes -O binary

<<<<<<< HEAD
vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)

vmlinux.bin.gz: vmlinux.bin FORCE
	$(call if_changed,gzip)

boot-elf: vmlinux.bin
boot-redboot: vmlinux.bin.gz
=======
$(obj)/vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)

$(obj)/vmlinux.bin.gz: $(obj)/vmlinux.bin FORCE
	$(call if_changed,gzip)

boot-elf: $(obj)/vmlinux.bin
boot-redboot: $(obj)/vmlinux.bin.gz
>>>>>>> upstream/android-13

UIMAGE_LOADADDR = $(CONFIG_KERNEL_LOAD_ADDRESS)
UIMAGE_COMPRESSION = gzip

<<<<<<< HEAD
$(obj)/uImage: vmlinux.bin.gz FORCE
	$(call if_changed,uimage)
	$(Q)$(kecho) '  Kernel: $@ is ready'
=======
$(obj)/uImage: $(obj)/vmlinux.bin.gz FORCE
	$(call if_changed,uimage)
	$(Q)$(kecho) '  Kernel: $@ is ready'

$(obj)/xipImage: vmlinux FORCE
	$(call if_changed,objcopy)
	$(Q)$(kecho) '  Kernel: $@ is ready'
>>>>>>> upstream/android-13
