# SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
# List of files in the vdso, has to be asm only for now

KCOV_INSTRUMENT := n

obj-vdso32 = gettimeofday.o clock_getres.o clock_gettime.o note.o getcpu.o
=======
# List of files in the vdso

KCOV_INSTRUMENT := n
ARCH_REL_TYPE_ABS := R_390_COPY|R_390_GLOB_DAT|R_390_JMP_SLOT|R_390_RELATIVE
ARCH_REL_TYPE_ABS += R_390_GOT|R_390_PLT

include $(srctree)/lib/vdso/Makefile
obj-vdso32 = vdso_user_wrapper-32.o note-32.o
>>>>>>> upstream/android-13

# Build rules

targets := $(obj-vdso32) vdso32.so vdso32.so.dbg
obj-vdso32 := $(addprefix $(obj)/, $(obj-vdso32))

KBUILD_AFLAGS += -DBUILD_VDSO
<<<<<<< HEAD
KBUILD_CFLAGS += -DBUILD_VDSO

KBUILD_AFLAGS_31 := $(filter-out -m64,$(KBUILD_AFLAGS))
KBUILD_AFLAGS_31 += -m31 -s

KBUILD_CFLAGS_31 := $(filter-out -m64,$(KBUILD_CFLAGS))
KBUILD_CFLAGS_31 += -m31 -fPIC -shared -fno-common -fno-builtin
KBUILD_CFLAGS_31 += -nostdlib -Wl,-soname=linux-vdso32.so.1 \
			$(call cc-ldoption, -Wl$(comma)--hash-style=both)

$(targets:%=$(obj)/%.dbg): KBUILD_CFLAGS = $(KBUILD_CFLAGS_31)
$(targets:%=$(obj)/%.dbg): KBUILD_AFLAGS = $(KBUILD_AFLAGS_31)

obj-y += vdso32_wrapper.o
extra-y += vdso32.lds
=======
KBUILD_CFLAGS += -DBUILD_VDSO -DDISABLE_BRANCH_PROFILING

KBUILD_AFLAGS_32 := $(filter-out -m64,$(KBUILD_AFLAGS))
KBUILD_AFLAGS_32 += -m31 -s

KBUILD_CFLAGS_32 := $(filter-out -m64,$(KBUILD_CFLAGS))
KBUILD_CFLAGS_32 += -m31 -fPIC -shared -fno-common -fno-builtin

LDFLAGS_vdso32.so.dbg += -fPIC -shared -nostdlib -soname=linux-vdso32.so.1 \
	--hash-style=both --build-id=sha1 -melf_s390 -T

$(targets:%=$(obj)/%.dbg): KBUILD_CFLAGS = $(KBUILD_CFLAGS_32)
$(targets:%=$(obj)/%.dbg): KBUILD_AFLAGS = $(KBUILD_AFLAGS_32)

obj-y += vdso32_wrapper.o
targets += vdso32.lds
>>>>>>> upstream/android-13
CPPFLAGS_vdso32.lds += -P -C -U$(ARCH)

# Disable gcov profiling, ubsan and kasan for VDSO code
GCOV_PROFILE := n
UBSAN_SANITIZE := n
KASAN_SANITIZE := n
<<<<<<< HEAD
=======
KCSAN_SANITIZE := n
>>>>>>> upstream/android-13

# Force dependency (incbin is bad)
$(obj)/vdso32_wrapper.o : $(obj)/vdso32.so

<<<<<<< HEAD
# link rule for the .so file, .lds has to be first
$(obj)/vdso32.so.dbg: $(src)/vdso32.lds $(obj-vdso32) FORCE
	$(call if_changed,vdso32ld)
=======
$(obj)/vdso32.so.dbg: $(src)/vdso32.lds $(obj-vdso32) FORCE
	$(call if_changed,ld)
>>>>>>> upstream/android-13

# strip rule for the .so file
$(obj)/%.so: OBJCOPYFLAGS := -S
$(obj)/%.so: $(obj)/%.so.dbg FORCE
	$(call if_changed,objcopy)

<<<<<<< HEAD
# assembly rules for the .S files
$(obj-vdso32): %.o: %.S FORCE
	$(call if_changed_dep,vdso32as)

# actual build commands
quiet_cmd_vdso32ld = VDSO32L $@
      cmd_vdso32ld = $(CC) $(c_flags) -Wl,-T $(filter %.lds %.o,$^) -o $@
quiet_cmd_vdso32as = VDSO32A $@
      cmd_vdso32as = $(CC) $(a_flags) -c -o $@ $<
=======
$(obj-vdso32): %-32.o: %.S FORCE
	$(call if_changed_dep,vdso32as)

# actual build commands
quiet_cmd_vdso32as = VDSO32A $@
      cmd_vdso32as = $(CC) $(a_flags) -c -o $@ $<
quiet_cmd_vdso32cc = VDSO32C $@
      cmd_vdso32cc = $(CC) $(c_flags) -c -o $@ $<
>>>>>>> upstream/android-13

# install commands for the unstripped file
quiet_cmd_vdso_install = INSTALL $@
      cmd_vdso_install = cp $(obj)/$@.dbg $(MODLIB)/vdso/$@

vdso32.so: $(obj)/vdso32.so.dbg
	@mkdir -p $(MODLIB)/vdso
	$(call cmd,vdso_install)

vdso_install: vdso32.so
<<<<<<< HEAD
=======

# Generate VDSO offsets using helper script
gen-vdsosym := $(srctree)/$(src)/gen_vdso_offsets.sh
quiet_cmd_vdsosym = VDSOSYM $@
	cmd_vdsosym = $(NM) $< | $(gen-vdsosym) | LC_ALL=C sort > $@

include/generated/vdso32-offsets.h: $(obj)/vdso32.so.dbg FORCE
	$(call if_changed,vdsosym)
>>>>>>> upstream/android-13
