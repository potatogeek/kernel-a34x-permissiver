# SPDX-License-Identifier: GPL-2.0
OBJECT_FILES_NON_STANDARD := y
<<<<<<< HEAD
CFLAGS_empty.o += $(DISABLE_LTO)

hostprogs-y	:= modpost mk_elfconfig
always		:= $(hostprogs-y) empty.o
=======
CFLAGS_REMOVE_empty.o += $(CC_FLAGS_LTO)

hostprogs-always-y	+= modpost mk_elfconfig
always-y		+= empty.o
>>>>>>> upstream/android-13

modpost-objs	:= modpost.o file2alias.o sumversion.o

devicetable-offsets-file := devicetable-offsets.h

$(obj)/$(devicetable-offsets-file): $(obj)/devicetable-offsets.s FORCE
	$(call filechk,offsets,__DEVICETABLE_OFFSETS_H__)

targets += $(devicetable-offsets-file) devicetable-offsets.s

# dependencies on generated files need to be listed explicitly

$(obj)/modpost.o $(obj)/file2alias.o $(obj)/sumversion.o: $(obj)/elfconfig.h
$(obj)/file2alias.o: $(obj)/$(devicetable-offsets-file)

quiet_cmd_elfconfig = MKELF   $@
      cmd_elfconfig = $(obj)/mk_elfconfig < $< > $@

$(obj)/elfconfig.h: $(obj)/empty.o $(obj)/mk_elfconfig FORCE
	$(call if_changed,elfconfig)

targets += elfconfig.h
