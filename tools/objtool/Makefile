# SPDX-License-Identifier: GPL-2.0
include ../scripts/Makefile.include
include ../scripts/Makefile.arch

<<<<<<< HEAD
ifeq ($(ARCH),x86_64)
ARCH := x86
endif

=======
>>>>>>> upstream/android-13
# always use the host compiler
AR	 = $(HOSTAR)
CC	 = $(HOSTCC)
LD	 = $(HOSTLD)

ifeq ($(srctree),)
srctree := $(patsubst %/,%,$(dir $(CURDIR)))
srctree := $(patsubst %/,%,$(dir $(srctree)))
endif

SUBCMD_SRCDIR		= $(srctree)/tools/lib/subcmd/
LIBSUBCMD_OUTPUT	= $(if $(OUTPUT),$(OUTPUT),$(CURDIR)/)
LIBSUBCMD		= $(LIBSUBCMD_OUTPUT)libsubcmd.a

OBJTOOL    := $(OUTPUT)objtool
OBJTOOL_IN := $(OBJTOOL)-in.o

LIBELF_FLAGS := $(shell pkg-config libelf --cflags 2>/dev/null)
LIBELF_LIBS  := $(shell pkg-config libelf --libs 2>/dev/null || echo -lelf)

all: $(OBJTOOL)

INCLUDES := -I$(srctree)/tools/include \
	    -I$(srctree)/tools/arch/$(HOSTARCH)/include/uapi \
<<<<<<< HEAD
	    -I$(srctree)/tools/objtool/arch/$(ARCH)/include
WARNINGS := $(EXTRA_WARNINGS) -Wno-switch-default -Wno-switch-enum -Wno-packed
=======
	    -I$(srctree)/tools/arch/$(SRCARCH)/include	\
	    -I$(srctree)/tools/objtool/include \
	    -I$(srctree)/tools/objtool/arch/$(SRCARCH)/include
WARNINGS := $(EXTRA_WARNINGS) -Wno-switch-default -Wno-switch-enum -Wno-packed -Wno-nested-externs
>>>>>>> upstream/android-13
CFLAGS   := -Werror $(WARNINGS) $(KBUILD_HOSTCFLAGS) -g $(INCLUDES) $(LIBELF_FLAGS)
LDFLAGS  += $(LIBELF_LIBS) $(LIBSUBCMD) $(KBUILD_HOSTLDFLAGS)

# Allow old libelf to be used:
elfshdr := $(shell echo '$(pound)include <libelf.h>' | $(CC) $(CFLAGS) -x c -E - | grep elf_getshdr)
CFLAGS += $(if $(elfshdr),,-DLIBELF_USE_DEPRECATED)

AWK = awk
<<<<<<< HEAD
=======

SUBCMD_CHECK := n
SUBCMD_ORC := n

ifeq ($(SRCARCH),x86)
	SUBCMD_CHECK := y
	SUBCMD_ORC := y
endif

export SUBCMD_CHECK SUBCMD_ORC
>>>>>>> upstream/android-13
export srctree OUTPUT CFLAGS SRCARCH AWK
include $(srctree)/tools/build/Makefile.include

$(OBJTOOL_IN): fixdep FORCE
<<<<<<< HEAD
	@$(MAKE) $(build)=objtool

$(OBJTOOL): $(LIBSUBCMD) $(OBJTOOL_IN)
	@$(CONFIG_SHELL) ./sync-check.sh
=======
	@$(CONFIG_SHELL) ./sync-check.sh
	@$(MAKE) $(build)=objtool

$(OBJTOOL): $(LIBSUBCMD) $(OBJTOOL_IN)
>>>>>>> upstream/android-13
	$(QUIET_LINK)$(CC) $(OBJTOOL_IN) $(LDFLAGS) -o $@


$(LIBSUBCMD): fixdep FORCE
	$(Q)$(MAKE) -C $(SUBCMD_SRCDIR) OUTPUT=$(LIBSUBCMD_OUTPUT)

clean:
	$(call QUIET_CLEAN, objtool) $(RM) $(OBJTOOL)
	$(Q)find $(OUTPUT) -name '*.o' -delete -o -name '\.*.cmd' -delete -o -name '\.*.d' -delete
<<<<<<< HEAD
	$(Q)$(RM) $(OUTPUT)arch/x86/lib/inat-tables.c $(OUTPUT)fixdep
=======
	$(Q)$(RM) $(OUTPUT)arch/x86/inat-tables.c $(OUTPUT)fixdep
>>>>>>> upstream/android-13

FORCE:

.PHONY: clean FORCE
