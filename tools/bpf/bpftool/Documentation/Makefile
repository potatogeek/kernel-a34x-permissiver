<<<<<<< HEAD
include ../../../scripts/Makefile.include
include ../../../scripts/utilities.mak
=======
# SPDX-License-Identifier: GPL-2.0-only
include ../../../scripts/Makefile.include
>>>>>>> upstream/android-13

INSTALL ?= install
RM ?= rm -f
RMDIR ?= rmdir --ignore-fail-on-non-empty

ifeq ($(V),1)
  Q =
else
  Q = @
endif

prefix ?= /usr/local
mandir ?= $(prefix)/man
man8dir = $(mandir)/man8

<<<<<<< HEAD
# Load targets for building eBPF helpers man page.
include ../../Makefile.helpers

MAN8_RST = $(filter-out $(HELPERS_RST),$(wildcard *.rst))
=======
MAN8_RST = $(wildcard bpftool*.rst)
>>>>>>> upstream/android-13

_DOC_MAN8 = $(patsubst %.rst,%.8,$(MAN8_RST))
DOC_MAN8 = $(addprefix $(OUTPUT),$(_DOC_MAN8))

<<<<<<< HEAD
man: man8 helpers
man8: $(DOC_MAN8)

RST2MAN_DEP := $(shell command -v rst2man 2>/dev/null)
=======
man: man8
man8: $(DOC_MAN8)

RST2MAN_DEP := $(shell command -v rst2man 2>/dev/null)
RST2MAN_OPTS += --verbose

list_pages = $(sort $(basename $(filter-out $(1),$(MAN8_RST))))
see_also = $(subst " ",, \
	"\n" \
	"SEE ALSO\n" \
	"========\n" \
	"\t**bpf**\ (2),\n" \
	"\t**bpf-helpers**\\ (7)" \
	$(foreach page,$(call list_pages,$(1)),",\n\t**$(page)**\\ (8)") \
	"\n")
>>>>>>> upstream/android-13

$(OUTPUT)%.8: %.rst
ifndef RST2MAN_DEP
	$(error "rst2man not found, but required to generate man pages")
endif
<<<<<<< HEAD
	$(QUIET_GEN)rst2man $< > $@

clean: helpers-clean
	$(call QUIET_CLEAN, Documentation)
	$(Q)$(RM) $(DOC_MAN8)

install: man helpers-install
=======
	$(QUIET_GEN)( cat $< ; printf "%b" $(call see_also,$<) ) | rst2man $(RST2MAN_OPTS) > $@

clean:
	$(call QUIET_CLEAN, Documentation)
	$(Q)$(RM) $(DOC_MAN8)

install: man
>>>>>>> upstream/android-13
	$(call QUIET_INSTALL, Documentation-man)
	$(Q)$(INSTALL) -d -m 755 $(DESTDIR)$(man8dir)
	$(Q)$(INSTALL) -m 644 $(DOC_MAN8) $(DESTDIR)$(man8dir)

<<<<<<< HEAD
uninstall: helpers-uninstall
=======
uninstall:
>>>>>>> upstream/android-13
	$(call QUIET_UNINST, Documentation-man)
	$(Q)$(RM) $(addprefix $(DESTDIR)$(man8dir)/,$(_DOC_MAN8))
	$(Q)$(RMDIR) $(DESTDIR)$(man8dir)

.PHONY: man man8 clean install uninstall
.DEFAULT_GOAL := man
