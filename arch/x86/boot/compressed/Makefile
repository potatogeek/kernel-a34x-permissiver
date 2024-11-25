# SPDX-License-Identifier: GPL-2.0
#
# linux/arch/x86/boot/compressed/Makefile
#
# create a compressed vmlinux image from the original vmlinux
#
# vmlinuz is:
#	decompression code (*.o)
#	asm globals (piggy.S), including:
#		vmlinux.bin.(gz|bz2|lzma|...)
#
# vmlinux.bin is:
#	vmlinux stripped of debugging and comments
# vmlinux.bin.all is:
#	vmlinux.bin + vmlinux.relocs
# vmlinux.bin.(gz|bz2|lzma|...) is:
#	(see scripts/Makefile.lib size_append)
#	compressed vmlinux.bin.all + u32 size of vmlinux.bin.all

<<<<<<< HEAD
KASAN_SANITIZE			:= n
=======
# Sanitizer runtimes are unavailable and cannot be linked for early boot code.
KASAN_SANITIZE			:= n
KCSAN_SANITIZE			:= n
>>>>>>> upstream/android-13
OBJECT_FILES_NON_STANDARD	:= y

# Prevents link failures: __sanitizer_cov_trace_pc() is not linked in.
KCOV_INSTRUMENT		:= n

targets := vmlinux vmlinux.bin vmlinux.bin.gz vmlinux.bin.bz2 vmlinux.bin.lzma \
<<<<<<< HEAD
	vmlinux.bin.xz vmlinux.bin.lzo vmlinux.bin.lz4

KBUILD_CFLAGS := -m$(BITS) -O2
KBUILD_CFLAGS += -fno-strict-aliasing $(call cc-option, -fPIE, -fPIC)
KBUILD_CFLAGS += -DDISABLE_BRANCH_PROFILING
cflags-$(CONFIG_X86_32) := -march=i386
cflags-$(CONFIG_X86_64) := -mcmodel=small
KBUILD_CFLAGS += $(cflags-y)
KBUILD_CFLAGS += -mno-mmx -mno-sse
KBUILD_CFLAGS += $(call cc-option,-ffreestanding)
KBUILD_CFLAGS += $(call cc-option,-fno-stack-protector)
KBUILD_CFLAGS += $(call cc-disable-warning, address-of-packed-member)
KBUILD_CFLAGS += $(call cc-disable-warning, gnu)
KBUILD_CFLAGS += -Wno-pointer-sign
# Disable relocation relaxation in case the link is not PIE.
KBUILD_CFLAGS += $(call as-option,-Wa$(comma)-mrelax-relocations=no)
=======
	vmlinux.bin.xz vmlinux.bin.lzo vmlinux.bin.lz4 vmlinux.bin.zst

# CLANG_FLAGS must come before any cc-disable-warning or cc-option calls in
# case of cross compiling, as it has the '--target=' flag, which is needed to
# avoid errors with '-march=i386', and future flags may depend on the target to
# be valid.
KBUILD_CFLAGS := -m$(BITS) -O2 $(CLANG_FLAGS)
KBUILD_CFLAGS += -fno-strict-aliasing -fPIE
KBUILD_CFLAGS += -Wundef
KBUILD_CFLAGS += -DDISABLE_BRANCH_PROFILING
cflags-$(CONFIG_X86_32) := -march=i386
cflags-$(CONFIG_X86_64) := -mcmodel=small -mno-red-zone
KBUILD_CFLAGS += $(cflags-y)
KBUILD_CFLAGS += -mno-mmx -mno-sse
KBUILD_CFLAGS += -ffreestanding -fshort-wchar
KBUILD_CFLAGS += -fno-stack-protector
KBUILD_CFLAGS += $(call cc-disable-warning, address-of-packed-member)
KBUILD_CFLAGS += $(call cc-disable-warning, gnu)
KBUILD_CFLAGS += -Wno-pointer-sign
KBUILD_CFLAGS += $(call cc-option,-fmacro-prefix-map=$(srctree)/=)
KBUILD_CFLAGS += -fno-asynchronous-unwind-tables
KBUILD_CFLAGS += -D__DISABLE_EXPORTS
# Disable relocation relaxation in case the link is not PIE.
KBUILD_CFLAGS += $(call as-option,-Wa$(comma)-mrelax-relocations=no)
KBUILD_CFLAGS += -include $(srctree)/include/linux/hidden.h

# sev.c indirectly inludes inat-table.h which is generated during
# compilation and stored in $(objtree). Add the directory to the includes so
# that the compiler finds it even with out-of-tree builds (make O=/some/path).
CFLAGS_sev.o += -I$(objtree)/arch/x86/lib/
>>>>>>> upstream/android-13

KBUILD_AFLAGS  := $(KBUILD_CFLAGS) -D__ASSEMBLY__
GCOV_PROFILE := n
UBSAN_SANITIZE :=n

KBUILD_LDFLAGS := -m elf_$(UTS_MACHINE)
<<<<<<< HEAD
# Compressed kernel should be built as PIE since it may be loaded at any
# address by the bootloader.
ifeq ($(CONFIG_X86_32),y)
KBUILD_LDFLAGS += $(call ld-option, -pie) $(call ld-option, --no-dynamic-linker)
else
# To build 64-bit compressed kernel as PIE, we disable relocation
# overflow check to avoid relocation overflow error with a new linker
# command-line option, -z noreloc-overflow.
KBUILD_LDFLAGS += $(shell $(LD) --help 2>&1 | grep -q "\-z noreloc-overflow" \
	&& echo "-z noreloc-overflow -pie --no-dynamic-linker")
endif
LDFLAGS_vmlinux := -T

hostprogs-y	:= mkpiggy
=======
KBUILD_LDFLAGS += $(call ld-option,--no-ld-generated-unwind-info)
# Compressed kernel should be built as PIE since it may be loaded at any
# address by the bootloader.
LDFLAGS_vmlinux := -pie $(call ld-option, --no-dynamic-linker)
ifdef CONFIG_LD_ORPHAN_WARN
LDFLAGS_vmlinux += --orphan-handling=warn
endif
LDFLAGS_vmlinux += -T

hostprogs	:= mkpiggy
>>>>>>> upstream/android-13
HOST_EXTRACFLAGS += -I$(srctree)/tools/include

sed-voffset := -e 's/^\([0-9a-fA-F]*\) [ABCDGRSTVW] \(_text\|__bss_start\|_end\)$$/\#define VO_\2 _AC(0x\1,UL)/p'

quiet_cmd_voffset = VOFFSET $@
      cmd_voffset = $(NM) $< | sed -n $(sed-voffset) > $@

targets += ../voffset.h

$(obj)/../voffset.h: vmlinux FORCE
	$(call if_changed,voffset)

$(obj)/misc.o: $(obj)/../voffset.h

<<<<<<< HEAD
vmlinux-objs-y := $(obj)/vmlinux.lds $(obj)/head_$(BITS).o $(obj)/misc.o \
	$(obj)/string.o $(obj)/cmdline.o $(obj)/error.o \
=======
vmlinux-objs-y := $(obj)/vmlinux.lds $(obj)/kernel_info.o $(obj)/head_$(BITS).o \
	$(obj)/misc.o $(obj)/string.o $(obj)/cmdline.o $(obj)/error.o \
>>>>>>> upstream/android-13
	$(obj)/piggy.o $(obj)/cpuflags.o

vmlinux-objs-$(CONFIG_EARLY_PRINTK) += $(obj)/early_serial_console.o
vmlinux-objs-$(CONFIG_RANDOMIZE_BASE) += $(obj)/kaslr.o
ifdef CONFIG_X86_64
<<<<<<< HEAD
	vmlinux-objs-$(CONFIG_RANDOMIZE_BASE) += $(obj)/kaslr_64.o
	vmlinux-objs-y += $(obj)/mem_encrypt.o
	vmlinux-objs-y += $(obj)/pgtable_64.o
endif

$(obj)/eboot.o: KBUILD_CFLAGS += -fshort-wchar -mno-red-zone

vmlinux-objs-$(CONFIG_EFI_STUB) += $(obj)/eboot.o $(obj)/efi_stub_$(BITS).o \
	$(objtree)/drivers/firmware/efi/libstub/lib.a
vmlinux-objs-$(CONFIG_EFI_MIXED) += $(obj)/efi_thunk_$(BITS).o

# The compressed kernel is built with -fPIC/-fPIE so that a boot loader
# can place it anywhere in memory and it will still run. However, since
# it is executed as-is without any ELF relocation processing performed
# (and has already had all relocation sections stripped from the binary),
# none of the code can use data relocations (e.g. static assignments of
# pointer values), since they will be meaningless at runtime. This check
# will refuse to link the vmlinux if any of these relocations are found.
quiet_cmd_check_data_rel = DATAREL $@
define cmd_check_data_rel
	for obj in $(filter %.o,$^); do \
		$(READELF) -S $$obj | grep -qF .rel.local && { \
			echo "error: $$obj has data relocations!" >&2; \
			exit 1; \
		} || true; \
	done
endef

# We need to run two commands under "if_changed", so merge them into a
# single invocation.
quiet_cmd_check-and-link-vmlinux = LD      $@
      cmd_check-and-link-vmlinux = $(cmd_check_data_rel); $(cmd_ld)

$(obj)/vmlinux: $(vmlinux-objs-y) FORCE
	$(call if_changed,check-and-link-vmlinux)
=======
	vmlinux-objs-y += $(obj)/ident_map_64.o
	vmlinux-objs-y += $(obj)/idt_64.o $(obj)/idt_handlers_64.o
	vmlinux-objs-y += $(obj)/mem_encrypt.o
	vmlinux-objs-y += $(obj)/pgtable_64.o
	vmlinux-objs-$(CONFIG_AMD_MEM_ENCRYPT) += $(obj)/sev.o
endif

vmlinux-objs-$(CONFIG_ACPI) += $(obj)/acpi.o

vmlinux-objs-$(CONFIG_EFI_MIXED) += $(obj)/efi_thunk_$(BITS).o
efi-obj-$(CONFIG_EFI_STUB) = $(objtree)/drivers/firmware/efi/libstub/lib.a

$(obj)/vmlinux: $(vmlinux-objs-y) $(efi-obj-y) FORCE
	$(call if_changed,ld)
>>>>>>> upstream/android-13

OBJCOPYFLAGS_vmlinux.bin :=  -R .comment -S
$(obj)/vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)

targets += $(patsubst $(obj)/%,%,$(vmlinux-objs-y)) vmlinux.bin.all vmlinux.relocs

CMD_RELOCS = arch/x86/tools/relocs
quiet_cmd_relocs = RELOCS  $@
      cmd_relocs = $(CMD_RELOCS) $< > $@;$(CMD_RELOCS) --abs-relocs $<
$(obj)/vmlinux.relocs: vmlinux FORCE
	$(call if_changed,relocs)

vmlinux.bin.all-y := $(obj)/vmlinux.bin
vmlinux.bin.all-$(CONFIG_X86_NEED_RELOCS) += $(obj)/vmlinux.relocs

$(obj)/vmlinux.bin.gz: $(vmlinux.bin.all-y) FORCE
	$(call if_changed,gzip)
$(obj)/vmlinux.bin.bz2: $(vmlinux.bin.all-y) FORCE
	$(call if_changed,bzip2)
$(obj)/vmlinux.bin.lzma: $(vmlinux.bin.all-y) FORCE
	$(call if_changed,lzma)
$(obj)/vmlinux.bin.xz: $(vmlinux.bin.all-y) FORCE
	$(call if_changed,xzkern)
$(obj)/vmlinux.bin.lzo: $(vmlinux.bin.all-y) FORCE
	$(call if_changed,lzo)
$(obj)/vmlinux.bin.lz4: $(vmlinux.bin.all-y) FORCE
	$(call if_changed,lz4)
<<<<<<< HEAD
=======
$(obj)/vmlinux.bin.zst: $(vmlinux.bin.all-y) FORCE
	$(call if_changed,zstd22)
>>>>>>> upstream/android-13

suffix-$(CONFIG_KERNEL_GZIP)	:= gz
suffix-$(CONFIG_KERNEL_BZIP2)	:= bz2
suffix-$(CONFIG_KERNEL_LZMA)	:= lzma
suffix-$(CONFIG_KERNEL_XZ)	:= xz
suffix-$(CONFIG_KERNEL_LZO) 	:= lzo
suffix-$(CONFIG_KERNEL_LZ4) 	:= lz4
<<<<<<< HEAD

quiet_cmd_mkpiggy = MKPIGGY $@
      cmd_mkpiggy = $(obj)/mkpiggy $< > $@ || ( rm -f $@ ; false )
=======
suffix-$(CONFIG_KERNEL_ZSTD)	:= zst

quiet_cmd_mkpiggy = MKPIGGY $@
      cmd_mkpiggy = $(obj)/mkpiggy $< > $@
>>>>>>> upstream/android-13

targets += piggy.S
$(obj)/piggy.S: $(obj)/vmlinux.bin.$(suffix-y) $(obj)/mkpiggy FORCE
	$(call if_changed,mkpiggy)
