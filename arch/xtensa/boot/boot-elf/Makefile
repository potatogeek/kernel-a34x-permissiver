#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#

<<<<<<< HEAD
ifeq ($(BIG_ENDIAN),1)
OBJCOPY_ARGS    := -O elf32-xtensa-be
else
OBJCOPY_ARGS    := -O elf32-xtensa-le
endif

export OBJCOPY_ARGS
export CPPFLAGS_boot.lds += -P -C
export KBUILD_AFLAGS += -mtext-section-literals

boot-y		:= bootstrap.o

OBJS		:= $(addprefix $(obj)/,$(boot-y))

$(obj)/Image.o: vmlinux.bin $(OBJS)
	$(Q)$(OBJCOPY) $(OBJCOPY_ARGS) -R .comment \
		--add-section image=vmlinux.bin \
=======
OBJCOPY_ARGS := -O $(if $(CONFIG_CPU_BIG_ENDIAN),elf32-xtensa-be,elf32-xtensa-le)

CPPFLAGS_boot.lds += -P -C
KBUILD_AFLAGS += -mtext-section-literals

boot-y		:= bootstrap.o
targets		+= $(boot-y) boot.lds

OBJS		:= $(addprefix $(obj)/,$(boot-y))

$(obj)/Image.o: $(obj)/../vmlinux.bin $(OBJS)
	$(Q)$(OBJCOPY) $(OBJCOPY_ARGS) -R .comment \
		--add-section image=$< \
>>>>>>> upstream/android-13
		--set-section-flags image=contents,alloc,load,load,data \
		$(OBJS) $@

$(obj)/../Image.elf: $(obj)/Image.o $(obj)/boot.lds
<<<<<<< HEAD
	$(Q)$(LD) $(KBUILD_LDFLAGS) $(LDFLAGS_vmlinux) \
=======
	$(Q)$(LD) $(KBUILD_LDFLAGS) \
>>>>>>> upstream/android-13
		-T $(obj)/boot.lds \
		--build-id=none \
		-o $@ $(obj)/Image.o
	$(Q)$(kecho) '  Kernel: $@ is ready'

all Image: $(obj)/../Image.elf
