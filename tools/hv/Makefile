# SPDX-License-Identifier: GPL-2.0
# Makefile for Hyper-V tools
<<<<<<< HEAD

WARNINGS = -Wall -Wextra
CFLAGS = $(WARNINGS) -g $(shell getconf LFS_CFLAGS)

CFLAGS += -D__EXPORTED_HEADERS__ -I../../include/uapi -I../../include
=======
include ../scripts/Makefile.include
>>>>>>> upstream/android-13

sbindir ?= /usr/sbin
libexecdir ?= /usr/libexec
sharedstatedir ?= /var/lib

<<<<<<< HEAD
ALL_PROGRAMS := hv_kvp_daemon hv_vss_daemon hv_fcopy_daemon
=======
ifeq ($(srctree),)
srctree := $(patsubst %/,%,$(dir $(CURDIR)))
srctree := $(patsubst %/,%,$(dir $(srctree)))
endif

# Do not use make's built-in rules
# (this improves performance and avoids hard-to-debug behaviour);
MAKEFLAGS += -r

override CFLAGS += -O2 -Wall -g -D_GNU_SOURCE -I$(OUTPUT)include

ALL_TARGETS := hv_kvp_daemon hv_vss_daemon hv_fcopy_daemon
ALL_PROGRAMS := $(patsubst %,$(OUTPUT)%,$(ALL_TARGETS))
>>>>>>> upstream/android-13

ALL_SCRIPTS := hv_get_dhcp_info.sh hv_get_dns_info.sh hv_set_ifconfig.sh

all: $(ALL_PROGRAMS)

<<<<<<< HEAD
%: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) hv_kvp_daemon hv_vss_daemon hv_fcopy_daemon

install: all
=======
export srctree OUTPUT CC LD CFLAGS
include $(srctree)/tools/build/Makefile.include

HV_KVP_DAEMON_IN := $(OUTPUT)hv_kvp_daemon-in.o
$(HV_KVP_DAEMON_IN): FORCE
	$(Q)$(MAKE) $(build)=hv_kvp_daemon
$(OUTPUT)hv_kvp_daemon: $(HV_KVP_DAEMON_IN)
	$(QUIET_LINK)$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

HV_VSS_DAEMON_IN := $(OUTPUT)hv_vss_daemon-in.o
$(HV_VSS_DAEMON_IN): FORCE
	$(Q)$(MAKE) $(build)=hv_vss_daemon
$(OUTPUT)hv_vss_daemon: $(HV_VSS_DAEMON_IN)
	$(QUIET_LINK)$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

HV_FCOPY_DAEMON_IN := $(OUTPUT)hv_fcopy_daemon-in.o
$(HV_FCOPY_DAEMON_IN): FORCE
	$(Q)$(MAKE) $(build)=hv_fcopy_daemon
$(OUTPUT)hv_fcopy_daemon: $(HV_FCOPY_DAEMON_IN)
	$(QUIET_LINK)$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f $(ALL_PROGRAMS)
	find $(if $(OUTPUT),$(OUTPUT),.) -name '*.o' -delete -o -name '\.*.d' -delete

install: $(ALL_PROGRAMS)
>>>>>>> upstream/android-13
	install -d -m 755 $(DESTDIR)$(sbindir); \
	install -d -m 755 $(DESTDIR)$(libexecdir)/hypervkvpd; \
	install -d -m 755 $(DESTDIR)$(sharedstatedir); \
	for program in $(ALL_PROGRAMS); do \
		install $$program -m 755 $(DESTDIR)$(sbindir);	\
	done; \
	install -m 755 lsvmbus $(DESTDIR)$(sbindir); \
	for script in $(ALL_SCRIPTS); do \
		install $$script -m 755 $(DESTDIR)$(libexecdir)/hypervkvpd/$${script%.sh}; \
	done
<<<<<<< HEAD
=======

FORCE:

.PHONY: all install clean FORCE prepare
>>>>>>> upstream/android-13
