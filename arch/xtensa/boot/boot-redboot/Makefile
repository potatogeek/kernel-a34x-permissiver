#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#

<<<<<<< HEAD
ifeq ($(BIG_ENDIAN),1)
OBJCOPY_ARGS 	:= -O elf32-xtensa-be
else
OBJCOPY_ARGS 	:= -O elf32-xtensa-le
endif
=======
OBJCOPY_ARGS := -O $(if $(CONFIG_CPU_BIG_ENDIAN),elf32-xtensa-be,elf32-xtensa-le)
>>>>>>> upstream/android-13

LD_ARGS	= -T $(srctree)/$(obj)/boot.ld

boot-y	:= bootstrap.o
<<<<<<< HEAD
=======
targets	+= $(boot-y)
>>>>>>> upstream/android-13

OBJS	:= $(addprefix $(obj)/,$(boot-y))
LIBS	:= arch/xtensa/boot/lib/lib.a arch/xtensa/lib/lib.a

LIBGCC := $(shell $(CC) $(KBUILD_CFLAGS) -print-libgcc-file-name)

<<<<<<< HEAD
$(obj)/zImage.o: vmlinux.bin.gz $(OBJS)
	$(Q)$(OBJCOPY) $(OBJCOPY_ARGS) -R .comment \
		--add-section image=vmlinux.bin.gz \
=======
$(obj)/zImage.o: $(obj)/../vmlinux.bin.gz $(OBJS)
	$(Q)$(OBJCOPY) $(OBJCOPY_ARGS) -R .comment \
		--add-section image=$< \
>>>>>>> upstream/android-13
		--set-section-flags image=contents,alloc,load,load,data \
		$(OBJS) $@

$(obj)/zImage.elf: $(obj)/zImage.o $(LIBS)
	$(Q)$(LD) $(LD_ARGS) -o $@ $^ -L/xtensa-elf/lib $(LIBGCC)

$(obj)/../zImage.redboot: $(obj)/zImage.elf
	$(Q)$(OBJCOPY) -S -O binary $< $@
	$(Q)$(kecho) '  Kernel: $@ is ready'

all zImage: $(obj)/../zImage.redboot
