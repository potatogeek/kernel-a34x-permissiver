<<<<<<< HEAD
# SPDX-License-Identifier: GPL-2.0
# Most of this file is copied from tools/lib/traceevent/Makefile

BPF_VERSION = 0
BPF_PATCHLEVEL = 0
BPF_EXTRAVERSION = 1

MAKEFLAGS += --no-print-directory

ifeq ($(srctree),)
=======
# SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause)
# Most of this file is copied from tools/lib/traceevent/Makefile

RM ?= rm
srctree = $(abs_srctree)

VERSION_SCRIPT := libbpf.map
LIBBPF_VERSION := $(shell \
	grep -oE '^LIBBPF_([0-9.]+)' $(VERSION_SCRIPT) | \
	sort -rV | head -n1 | cut -d'_' -f2)
LIBBPF_MAJOR_VERSION := $(firstword $(subst ., ,$(LIBBPF_VERSION)))

MAKEFLAGS += --no-print-directory

# This will work when bpf is built in tools env. where srctree
# isn't set and when invoked from selftests build, where srctree
# is a ".". building_out_of_srctree is undefined for in srctree
# builds
ifndef building_out_of_srctree
>>>>>>> upstream/android-13
srctree := $(patsubst %/,%,$(dir $(CURDIR)))
srctree := $(patsubst %/,%,$(dir $(srctree)))
srctree := $(patsubst %/,%,$(dir $(srctree)))
#$(info Determined 'srctree' to be $(srctree))
endif

<<<<<<< HEAD
# Makefiles suck: This macro sets a default value of $(2) for the
# variable named by $(1), unless the variable has been set by
# environment or command line. This is necessary for CC and AR
# because make sets default values, so the simpler ?= approach
# won't work as expected.
define allow-override
  $(if $(or $(findstring environment,$(origin $(1))),\
            $(findstring command line,$(origin $(1)))),,\
    $(eval $(1) = $(2)))
endef

# Allow setting CC and AR, or setting CROSS_COMPILE as a prefix.
$(call allow-override,CC,$(CROSS_COMPILE)gcc)
$(call allow-override,AR,$(CROSS_COMPILE)ar)

=======
>>>>>>> upstream/android-13
INSTALL = install

# Use DESTDIR for installing into a different root directory.
# This is useful for building a package. The program will be
# installed in this directory as if it was the root directory.
# Then the build tool can move it later.
DESTDIR ?=
DESTDIR_SQ = '$(subst ','\'',$(DESTDIR))'

include $(srctree)/tools/scripts/Makefile.arch

ifeq ($(LP64), 1)
  libdir_relative = lib64
else
  libdir_relative = lib
endif

prefix ?= /usr/local
libdir = $(prefix)/$(libdir_relative)
man_dir = $(prefix)/share/man
man_dir_SQ = '$(subst ','\'',$(man_dir))'

export man_dir man_dir_SQ INSTALL
export DESTDIR DESTDIR_SQ

<<<<<<< HEAD
include ../../scripts/Makefile.include
=======
include $(srctree)/tools/scripts/Makefile.include
>>>>>>> upstream/android-13

# copy a bit from Linux kbuild

ifeq ("$(origin V)", "command line")
  VERBOSE = $(V)
endif
ifndef VERBOSE
  VERBOSE = 0
endif

<<<<<<< HEAD
FEATURE_USER = .libbpf
FEATURE_TESTS = libelf libelf-mmap bpf reallocarray
FEATURE_DISPLAY = libelf bpf

INCLUDES = -I. -I$(srctree)/tools/include -I$(srctree)/tools/arch/$(ARCH)/include/uapi -I$(srctree)/tools/include/uapi -I$(srctree)/tools/perf
FEATURE_CHECK_CFLAGS-bpf = $(INCLUDES)

check_feat := 1
NON_CHECK_FEAT_TARGETS := clean TAGS tags cscope help
ifdef MAKECMDGOALS
ifeq ($(filter-out $(NON_CHECK_FEAT_TARGETS),$(MAKECMDGOALS)),)
  check_feat := 0
endif
endif

ifeq ($(check_feat),1)
ifeq ($(FEATURES_DUMP),)
include $(srctree)/tools/build/Makefile.feature
else
include $(FEATURES_DUMP)
endif
endif
=======
INCLUDES = -I. -I$(srctree)/tools/include -I$(srctree)/tools/include/uapi
>>>>>>> upstream/android-13

export prefix libdir src obj

# Shell quotes
libdir_SQ = $(subst ','\'',$(libdir))
libdir_relative_SQ = $(subst ','\'',$(libdir_relative))

<<<<<<< HEAD
LIB_FILE = libbpf.a libbpf.so

VERSION		= $(BPF_VERSION)
PATCHLEVEL	= $(BPF_PATCHLEVEL)
EXTRAVERSION	= $(BPF_EXTRAVERSION)

OBJ		= $@
N		=

LIBBPF_VERSION = $(BPF_VERSION).$(BPF_PATCHLEVEL).$(BPF_EXTRAVERSION)
=======
OBJ		= $@
N		=

LIB_TARGET	= libbpf.a libbpf.so.$(LIBBPF_VERSION)
LIB_FILE	= libbpf.a libbpf.so*
PC_FILE		= libbpf.pc
>>>>>>> upstream/android-13

# Set compile option CFLAGS
ifdef EXTRA_CFLAGS
  CFLAGS := $(EXTRA_CFLAGS)
else
<<<<<<< HEAD
  CFLAGS := -g -Wall
endif

ifeq ($(feature-libelf-mmap), 1)
  override CFLAGS += -DHAVE_LIBELF_MMAP_SUPPORT
endif

ifeq ($(feature-reallocarray), 0)
  override CFLAGS += -DCOMPAT_NEED_REALLOCARRAY
endif

# Append required CFLAGS
override CFLAGS += $(EXTRA_WARNINGS)
override CFLAGS += -Werror -Wall
override CFLAGS += -fPIC
override CFLAGS += $(INCLUDES)
=======
  CFLAGS := -g -O2
endif

# Append required CFLAGS
override CFLAGS += $(EXTRA_WARNINGS) -Wno-switch-enum
override CFLAGS += -Werror -Wall
override CFLAGS += $(INCLUDES)
override CFLAGS += -fvisibility=hidden
override CFLAGS += -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

# flags specific for shared library
SHLIB_FLAGS := -DSHARED -fPIC
>>>>>>> upstream/android-13

ifeq ($(VERBOSE),1)
  Q =
else
  Q = @
endif

# Disable command line variables (CFLAGS) override from top
# level Makefile (perf), otherwise build Makefile will get
# the same command line setup.
MAKEOVERRIDES=

all:

export srctree OUTPUT CC LD CFLAGS V
include $(srctree)/tools/build/Makefile.include

<<<<<<< HEAD
BPF_IN    := $(OUTPUT)libbpf-in.o
LIB_FILE := $(addprefix $(OUTPUT),$(LIB_FILE))

CMD_TARGETS = $(LIB_FILE)

TARGETS = $(CMD_TARGETS)
=======
SHARED_OBJDIR	:= $(OUTPUT)sharedobjs/
STATIC_OBJDIR	:= $(OUTPUT)staticobjs/
BPF_IN_SHARED	:= $(SHARED_OBJDIR)libbpf-in.o
BPF_IN_STATIC	:= $(STATIC_OBJDIR)libbpf-in.o
BPF_HELPER_DEFS	:= $(OUTPUT)bpf_helper_defs.h

LIB_TARGET	:= $(addprefix $(OUTPUT),$(LIB_TARGET))
LIB_FILE	:= $(addprefix $(OUTPUT),$(LIB_FILE))
PC_FILE		:= $(addprefix $(OUTPUT),$(PC_FILE))

TAGS_PROG := $(if $(shell which etags 2>/dev/null),etags,ctags)

GLOBAL_SYM_COUNT = $(shell readelf -s --wide $(BPF_IN_SHARED) | \
			   cut -d "@" -f1 | sed 's/_v[0-9]_[0-9]_[0-9].*//' | \
			   sed 's/\[.*\]//' | \
			   awk '/GLOBAL/ && /DEFAULT/ && !/UND/ {print $$NF}' | \
			   sort -u | wc -l)
VERSIONED_SYM_COUNT = $(shell readelf --dyn-syms --wide $(OUTPUT)libbpf.so | \
			      sed 's/\[.*\]//' | \
			      awk '/GLOBAL/ && /DEFAULT/ && !/UND|ABS/ {print $$NF}' | \
			      grep -Eo '[^ ]+@LIBBPF_' | cut -d@ -f1 | sort -u | wc -l)

CMD_TARGETS = $(LIB_TARGET) $(PC_FILE)
>>>>>>> upstream/android-13

all: fixdep
	$(Q)$(MAKE) all_cmd

<<<<<<< HEAD
all_cmd: $(CMD_TARGETS)

$(BPF_IN): force elfdep bpfdep
=======
all_cmd: $(CMD_TARGETS) check

$(BPF_IN_SHARED): force $(BPF_HELPER_DEFS)
>>>>>>> upstream/android-13
	@(test -f ../../include/uapi/linux/bpf.h -a -f ../../../include/uapi/linux/bpf.h && ( \
	(diff -B ../../include/uapi/linux/bpf.h ../../../include/uapi/linux/bpf.h >/dev/null) || \
	echo "Warning: Kernel ABI header at 'tools/include/uapi/linux/bpf.h' differs from latest version at 'include/uapi/linux/bpf.h'" >&2 )) || true
	@(test -f ../../include/uapi/linux/bpf_common.h -a -f ../../../include/uapi/linux/bpf_common.h && ( \
	(diff -B ../../include/uapi/linux/bpf_common.h ../../../include/uapi/linux/bpf_common.h >/dev/null) || \
	echo "Warning: Kernel ABI header at 'tools/include/uapi/linux/bpf_common.h' differs from latest version at 'include/uapi/linux/bpf_common.h'" >&2 )) || true
	@(test -f ../../include/uapi/linux/netlink.h -a -f ../../../include/uapi/linux/netlink.h && ( \
	(diff -B ../../include/uapi/linux/netlink.h ../../../include/uapi/linux/netlink.h >/dev/null) || \
	echo "Warning: Kernel ABI header at 'tools/include/uapi/linux/netlink.h' differs from latest version at 'include/uapi/linux/netlink.h'" >&2 )) || true
	@(test -f ../../include/uapi/linux/if_link.h -a -f ../../../include/uapi/linux/if_link.h && ( \
	(diff -B ../../include/uapi/linux/if_link.h ../../../include/uapi/linux/if_link.h >/dev/null) || \
	echo "Warning: Kernel ABI header at 'tools/include/uapi/linux/if_link.h' differs from latest version at 'include/uapi/linux/if_link.h'" >&2 )) || true
<<<<<<< HEAD
	$(Q)$(MAKE) $(build)=libbpf

$(OUTPUT)libbpf.so: $(BPF_IN)
	$(QUIET_LINK)$(CC) --shared $^ -o $@

$(OUTPUT)libbpf.a: $(BPF_IN)
	$(QUIET_LINK)$(RM) $@; $(AR) rcs $@ $^
=======
	@(test -f ../../include/uapi/linux/if_xdp.h -a -f ../../../include/uapi/linux/if_xdp.h && ( \
	(diff -B ../../include/uapi/linux/if_xdp.h ../../../include/uapi/linux/if_xdp.h >/dev/null) || \
	echo "Warning: Kernel ABI header at 'tools/include/uapi/linux/if_xdp.h' differs from latest version at 'include/uapi/linux/if_xdp.h'" >&2 )) || true
	$(Q)$(MAKE) $(build)=libbpf OUTPUT=$(SHARED_OBJDIR) CFLAGS="$(CFLAGS) $(SHLIB_FLAGS)"

$(BPF_IN_STATIC): force $(BPF_HELPER_DEFS)
	$(Q)$(MAKE) $(build)=libbpf OUTPUT=$(STATIC_OBJDIR)

$(BPF_HELPER_DEFS): $(srctree)/tools/include/uapi/linux/bpf.h
	$(QUIET_GEN)$(srctree)/scripts/bpf_doc.py --header \
		--file $(srctree)/tools/include/uapi/linux/bpf.h > $(BPF_HELPER_DEFS)

$(OUTPUT)libbpf.so: $(OUTPUT)libbpf.so.$(LIBBPF_VERSION)

$(OUTPUT)libbpf.so.$(LIBBPF_VERSION): $(BPF_IN_SHARED) $(VERSION_SCRIPT)
	$(QUIET_LINK)$(CC) $(LDFLAGS) \
		--shared -Wl,-soname,libbpf.so.$(LIBBPF_MAJOR_VERSION) \
		-Wl,--version-script=$(VERSION_SCRIPT) $< -lelf -lz -o $@
	@ln -sf $(@F) $(OUTPUT)libbpf.so
	@ln -sf $(@F) $(OUTPUT)libbpf.so.$(LIBBPF_MAJOR_VERSION)

$(OUTPUT)libbpf.a: $(BPF_IN_STATIC)
	$(QUIET_LINK)$(RM) -f $@; $(AR) rcs $@ $^

$(OUTPUT)libbpf.pc:
	$(QUIET_GEN)sed -e "s|@PREFIX@|$(prefix)|" \
		-e "s|@LIBDIR@|$(libdir_SQ)|" \
		-e "s|@VERSION@|$(LIBBPF_VERSION)|" \
		< libbpf.pc.template > $@

check: check_abi

check_abi: $(OUTPUT)libbpf.so $(VERSION_SCRIPT)
	@if [ "$(GLOBAL_SYM_COUNT)" != "$(VERSIONED_SYM_COUNT)" ]; then	 \
		echo "Warning: Num of global symbols in $(BPF_IN_SHARED)"	 \
		     "($(GLOBAL_SYM_COUNT)) does NOT match with num of"	 \
		     "versioned symbols in $^ ($(VERSIONED_SYM_COUNT))." \
		     "Please make sure all LIBBPF_API symbols are"	 \
		     "versioned in $(VERSION_SCRIPT)." >&2;		 \
		readelf -s --wide $(BPF_IN_SHARED) |			 \
		    cut -d "@" -f1 | sed 's/_v[0-9]_[0-9]_[0-9].*//' |	 \
		    sed 's/\[.*\]//' |					 \
		    awk '/GLOBAL/ && /DEFAULT/ && !/UND/ {print $$NF}'|  \
		    sort -u > $(OUTPUT)libbpf_global_syms.tmp;		 \
		readelf --dyn-syms --wide $(OUTPUT)libbpf.so |		 \
		    sed 's/\[.*\]//' |					 \
		    awk '/GLOBAL/ && /DEFAULT/ && !/UND|ABS/ {print $$NF}'|  \
		    grep -Eo '[^ ]+@LIBBPF_' | cut -d@ -f1 |		 \
		    sort -u > $(OUTPUT)libbpf_versioned_syms.tmp; 	 \
		diff -u $(OUTPUT)libbpf_global_syms.tmp			 \
		     $(OUTPUT)libbpf_versioned_syms.tmp;		 \
		rm $(OUTPUT)libbpf_global_syms.tmp			 \
		   $(OUTPUT)libbpf_versioned_syms.tmp;			 \
		exit 1;							 \
	fi

define do_install_mkdir
	if [ ! -d '$(DESTDIR_SQ)$1' ]; then		\
		$(INSTALL) -d -m 755 '$(DESTDIR_SQ)$1';	\
	fi
endef
>>>>>>> upstream/android-13

define do_install
	if [ ! -d '$(DESTDIR_SQ)$2' ]; then		\
		$(INSTALL) -d -m 755 '$(DESTDIR_SQ)$2';	\
	fi;						\
	$(INSTALL) $(if $3,-m $3,) $1 '$(DESTDIR_SQ)$2'
endef

install_lib: all_cmd
<<<<<<< HEAD
	$(call QUIET_INSTALL, $(LIB_FILE)) \
		$(call do_install,$(LIB_FILE),$(libdir_SQ))

install_headers:
	$(call QUIET_INSTALL, headers) \
		$(call do_install,bpf.h,$(prefix)/include/bpf,644); \
		$(call do_install,libbpf.h,$(prefix)/include/bpf,644);
		$(call do_install,btf.h,$(prefix)/include/bpf,644);

install: install_lib

### Cleaning rules

config-clean:
	$(call QUIET_CLEAN, config)
	$(Q)$(MAKE) -C $(srctree)/tools/build/feature/ clean >/dev/null

clean:
	$(call QUIET_CLEAN, libbpf) $(RM) *.o *~ $(TARGETS) *.a *.so .*.d .*.cmd \
		$(RM) LIBBPF-CFLAGS
	$(call QUIET_CLEAN, core-gen) $(RM) $(OUTPUT)FEATURE-DUMP.libbpf



PHONY += force elfdep bpfdep
force:

elfdep:
	@if [ "$(feature-libelf)" != "1" ]; then echo "No libelf found"; exit 1 ; fi

bpfdep:
	@if [ "$(feature-bpf)" != "1" ]; then echo "BPF API too old"; exit 1 ; fi
=======
	$(call QUIET_INSTALL, $(LIB_TARGET)) \
		$(call do_install_mkdir,$(libdir_SQ)); \
		cp -fpR $(LIB_FILE) $(DESTDIR)$(libdir_SQ)

INSTALL_HEADERS = bpf.h libbpf.h btf.h libbpf_common.h libbpf_legacy.h xsk.h \
		  bpf_helpers.h $(BPF_HELPER_DEFS) bpf_tracing.h	     \
		  bpf_endian.h bpf_core_read.h skel_internal.h

install_headers: $(BPF_HELPER_DEFS)
	$(call QUIET_INSTALL, headers)					     \
		$(foreach hdr,$(INSTALL_HEADERS),			     \
			$(call do_install,$(hdr),$(prefix)/include/bpf,644);)

install_pkgconfig: $(PC_FILE)
	$(call QUIET_INSTALL, $(PC_FILE)) \
		$(call do_install,$(PC_FILE),$(libdir_SQ)/pkgconfig,644)

install: install_lib install_pkgconfig install_headers

clean:
	$(call QUIET_CLEAN, libbpf) $(RM) -rf $(CMD_TARGETS)		     \
		*~ .*.d .*.cmd LIBBPF-CFLAGS $(BPF_HELPER_DEFS)		     \
		$(SHARED_OBJDIR) $(STATIC_OBJDIR)			     \
		$(addprefix $(OUTPUT),					     \
			    *.o *.a *.so *.so.$(LIBBPF_MAJOR_VERSION) *.pc)

PHONY += force cscope tags
force:

cscope:
	ls *.c *.h > cscope.files
	cscope -b -q -I $(srctree)/include -f cscope.out

tags:
	$(RM) -f TAGS tags
	ls *.c *.h | xargs $(TAGS_PROG) -a
>>>>>>> upstream/android-13

# Declare the contents of the .PHONY variable as phony.  We keep that
# information in a variable so we can use it in if_changed and friends.
.PHONY: $(PHONY)
<<<<<<< HEAD
=======

# Delete partially updated (corrupted) files on error
.DELETE_ON_ERROR:
>>>>>>> upstream/android-13
