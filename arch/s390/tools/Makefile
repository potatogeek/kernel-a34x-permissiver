# SPDX-License-Identifier: GPL-2.0
#
# Makefile for s390 specific build tools
#

kapi := arch/$(ARCH)/include/generated/asm
kapi-hdrs-y := $(kapi)/facility-defs.h $(kapi)/dis-defs.h

<<<<<<< HEAD
targets += $(addprefix ../../../,$(kapi-hdrs-y))
=======
>>>>>>> upstream/android-13
PHONY += kapi

kapi:	$(kapi-hdrs-y)

<<<<<<< HEAD
hostprogs-y		    += gen_facilities
hostprogs-y		    += gen_opcode_table

HOSTCFLAGS_gen_facilities.o += -Wall $(LINUXINCLUDE)
HOSTCFLAGS_gen_opcode_table.o += -Wall $(LINUXINCLUDE)

# Ensure output directory exists
_dummy := $(shell [ -d '$(kapi)' ] || mkdir -p '$(kapi)')

define filechk_facility-defs.h
	$(obj)/gen_facilities
endef

define filechk_dis-defs.h
	( $(obj)/gen_opcode_table < $(srctree)/arch/$(ARCH)/tools/opcodes.txt )
endef
=======
hostprogs		    += gen_facilities
hostprogs		    += gen_opcode_table

HOSTCFLAGS_gen_facilities.o += $(LINUXINCLUDE)

filechk_facility-defs.h = $(obj)/gen_facilities

filechk_dis-defs.h = \
	$(obj)/gen_opcode_table < $(srctree)/arch/$(ARCH)/tools/opcodes.txt
>>>>>>> upstream/android-13

$(kapi)/facility-defs.h: $(obj)/gen_facilities FORCE
	$(call filechk,facility-defs.h)

$(kapi)/dis-defs.h: $(obj)/gen_opcode_table FORCE
	$(call filechk,dis-defs.h)
