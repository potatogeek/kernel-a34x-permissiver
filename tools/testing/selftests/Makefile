# SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
TARGETS = android
=======
TARGETS = arm64
>>>>>>> upstream/android-13
TARGETS += bpf
TARGETS += breakpoints
TARGETS += capabilities
TARGETS += cgroup
<<<<<<< HEAD
TARGETS += cpufreq
TARGETS += cpu-hotplug
TARGETS += efivarfs
TARGETS += exec
TARGETS += filesystems
TARGETS += firmware
=======
TARGETS += clone3
TARGETS += core
TARGETS += cpufreq
TARGETS += cpu-hotplug
TARGETS += drivers/dma-buf
TARGETS += efivarfs
TARGETS += exec
TARGETS += filesystems
TARGETS += filesystems/binderfs
TARGETS += filesystems/epoll
TARGETS += firmware
TARGETS += fpu
>>>>>>> upstream/android-13
TARGETS += ftrace
TARGETS += futex
TARGETS += gpio
TARGETS += intel_pstate
TARGETS += ipc
<<<<<<< HEAD
TARGETS += kcmp
TARGETS += kvm
TARGETS += lib
TARGETS += membarrier
TARGETS += memfd
TARGETS += memory-hotplug
TARGETS += mount
TARGETS += mqueue
TARGETS += net
TARGETS += netfilter
TARGETS += nsfs
=======
TARGETS += ir
TARGETS += kcmp
TARGETS += kexec
TARGETS += kvm
TARGETS += landlock
TARGETS += lib
TARGETS += livepatch
TARGETS += lkdtm
TARGETS += membarrier
TARGETS += memfd
TARGETS += memory-hotplug
TARGETS += mincore
TARGETS += mount
TARGETS += mount_setattr
TARGETS += move_mount_set_group
TARGETS += mqueue
TARGETS += nci
TARGETS += net
TARGETS += net/af_unix
TARGETS += net/forwarding
TARGETS += net/mptcp
TARGETS += netfilter
TARGETS += nsfs
TARGETS += pidfd
TARGETS += pid_namespace
>>>>>>> upstream/android-13
TARGETS += powerpc
TARGETS += proc
TARGETS += pstore
TARGETS += ptrace
<<<<<<< HEAD
TARGETS += rseq
TARGETS += rtc
TARGETS += seccomp
=======
TARGETS += openat2
TARGETS += rlimits
TARGETS += rseq
TARGETS += rtc
TARGETS += seccomp
TARGETS += sgx
>>>>>>> upstream/android-13
TARGETS += sigaltstack
TARGETS += size
TARGETS += sparc64
TARGETS += splice
TARGETS += static_keys
TARGETS += sync
<<<<<<< HEAD
TARGETS += sysctl
ifneq (1, $(quicktest))
TARGETS += timers
endif
TARGETS += user
=======
TARGETS += syscall_user_dispatch
TARGETS += sysctl
TARGETS += tc-testing
TARGETS += timens
ifneq (1, $(quicktest))
TARGETS += timers
endif
TARGETS += tmpfs
TARGETS += tpm2
TARGETS += user
TARGETS += vDSO
>>>>>>> upstream/android-13
TARGETS += vm
TARGETS += x86
TARGETS += zram
#Please keep the TARGETS list alphabetically sorted
# Run "make quicktest=1 run_tests" or
# "make quicktest=1 kselftest" from top level Makefile

TARGETS_HOTPLUG = cpu-hotplug
TARGETS_HOTPLUG += memory-hotplug

<<<<<<< HEAD
# Clear LDFLAGS and MAKEFLAGS if called from main
# Makefile to avoid test build failures when test
# Makefile doesn't have explicit build rules.
ifeq (1,$(MAKELEVEL))
=======
# User can optionally provide a TARGETS skiplist.  By default we skip
# BPF since it has cutting edge build time dependencies which require
# more effort to install.
SKIP_TARGETS ?= bpf
ifneq ($(SKIP_TARGETS),)
	TMP := $(filter-out $(SKIP_TARGETS), $(TARGETS))
	override TARGETS := $(TMP)
endif

# User can set FORCE_TARGETS to 1 to require all targets to be successfully
# built; make will fail if any of the targets cannot be built. If
# FORCE_TARGETS is not set (the default), make will succeed if at least one
# of the targets gets built.
FORCE_TARGETS ?=

# Clear LDFLAGS and MAKEFLAGS when implicit rules are missing.  This provides
# implicit rules to sub-test Makefiles which avoids build failures in test
# Makefile that don't have explicit build rules.
ifeq (,$(LINK.c))
>>>>>>> upstream/android-13
override LDFLAGS =
override MAKEFLAGS =
endif

<<<<<<< HEAD
ifneq ($(KBUILD_SRC),)
override LDFLAGS =
endif

BUILD := $(O)
ifndef BUILD
  BUILD := $(KBUILD_OUTPUT)
endif
ifndef BUILD
  BUILD := $(shell pwd)
endif

# KSFT_TAP_LEVEL is used from KSFT framework to prevent nested TAP header
# printing from tests. Applicable to run_tests case where run_tests adds
# TAP header prior running tests and when a test program invokes another
# with system() call. Export it here to cover override RUN_TESTS defines.
export KSFT_TAP_LEVEL=`echo 1`

export BUILD
all:
	@for TARGET in $(TARGETS); do		\
		BUILD_TARGET=$$BUILD/$$TARGET;	\
		mkdir $$BUILD_TARGET  -p;	\
		make OUTPUT=$$BUILD_TARGET -C $$TARGET;\
	done;
=======
# Append kselftest to KBUILD_OUTPUT and O to avoid cluttering
# KBUILD_OUTPUT with selftest objects and headers installed
# by selftests Makefile or lib.mk.
ifdef building_out_of_srctree
override LDFLAGS =
endif

ifneq ($(O),)
	BUILD := $(O)/kselftest
else
	ifneq ($(KBUILD_OUTPUT),)
		BUILD := $(KBUILD_OUTPUT)/kselftest
	else
		BUILD := $(shell pwd)
		DEFAULT_INSTALL_HDR_PATH := 1
	endif
endif

# Prepare for headers install
top_srcdir ?= ../../..
include $(top_srcdir)/scripts/subarch.include
ARCH           ?= $(SUBARCH)
export KSFT_KHDR_INSTALL_DONE := 1
export BUILD

# set default goal to all, so make without a target runs all, even when
# all isn't the first target in the file.
.DEFAULT_GOAL := all

# Install headers here once for all tests. KSFT_KHDR_INSTALL_DONE
# is used to avoid running headers_install from lib.mk.
# Invoke headers install with --no-builtin-rules to avoid circular
# dependency in "make kselftest" case. In this case, second level
# make inherits builtin-rules which will use the rule generate
# Makefile.o and runs into
# "Circular Makefile.o <- prepare dependency dropped."
# and headers_install fails and test compile fails.
#
# O= KBUILD_OUTPUT cases don't run into this error, since main Makefile
# invokes them as sub-makes and --no-builtin-rules is not necessary,
# but doesn't cause any failures. Keep it simple and use the same
# flags in both cases.
# Local build cases: "make kselftest", "make -C" - headers are installed
# in the default INSTALL_HDR_PATH usr/include.
khdr:
ifeq (1,$(DEFAULT_INSTALL_HDR_PATH))
	$(MAKE) --no-builtin-rules ARCH=$(ARCH) -C $(top_srcdir) headers_install
else
	$(MAKE) --no-builtin-rules INSTALL_HDR_PATH=$$BUILD/usr \
		ARCH=$(ARCH) -C $(top_srcdir) headers_install
endif

all: khdr
	@ret=1;							\
	for TARGET in $(TARGETS); do				\
		BUILD_TARGET=$$BUILD/$$TARGET;			\
		mkdir $$BUILD_TARGET  -p;			\
		$(MAKE) OUTPUT=$$BUILD_TARGET -C $$TARGET	\
				$(if $(FORCE_TARGETS),|| exit);	\
		ret=$$((ret * $$?));				\
	done; exit $$ret;
>>>>>>> upstream/android-13

run_tests: all
	@for TARGET in $(TARGETS); do \
		BUILD_TARGET=$$BUILD/$$TARGET;	\
<<<<<<< HEAD
		make OUTPUT=$$BUILD_TARGET -C $$TARGET run_tests;\
=======
		$(MAKE) OUTPUT=$$BUILD_TARGET -C $$TARGET run_tests;\
>>>>>>> upstream/android-13
	done;

hotplug:
	@for TARGET in $(TARGETS_HOTPLUG); do \
		BUILD_TARGET=$$BUILD/$$TARGET;	\
<<<<<<< HEAD
		make OUTPUT=$$BUILD_TARGET -C $$TARGET;\
=======
		$(MAKE) OUTPUT=$$BUILD_TARGET -C $$TARGET;\
>>>>>>> upstream/android-13
	done;

run_hotplug: hotplug
	@for TARGET in $(TARGETS_HOTPLUG); do \
		BUILD_TARGET=$$BUILD/$$TARGET;	\
<<<<<<< HEAD
		make OUTPUT=$$BUILD_TARGET -C $$TARGET run_full_test;\
=======
		$(MAKE) OUTPUT=$$BUILD_TARGET -C $$TARGET run_full_test;\
>>>>>>> upstream/android-13
	done;

clean_hotplug:
	@for TARGET in $(TARGETS_HOTPLUG); do \
		BUILD_TARGET=$$BUILD/$$TARGET;	\
<<<<<<< HEAD
		make OUTPUT=$$BUILD_TARGET -C $$TARGET clean;\
	done;

run_pstore_crash:
	make -C pstore run_crash

INSTALL_PATH ?= install
INSTALL_PATH := $(abspath $(INSTALL_PATH))
ALL_SCRIPT := $(INSTALL_PATH)/run_kselftest.sh

install:
ifdef INSTALL_PATH
	@# Ask all targets to install their files
	mkdir -p $(INSTALL_PATH)
	@for TARGET in $(TARGETS); do \
		BUILD_TARGET=$$BUILD/$$TARGET;	\
		make OUTPUT=$$BUILD_TARGET -C $$TARGET INSTALL_PATH=$(INSTALL_PATH)/$$TARGET install; \
	done;

	@# Ask all targets to emit their test scripts
	echo "#!/bin/sh" > $(ALL_SCRIPT)
	echo "BASE_DIR=\$$(realpath \$$(dirname \$$0))" >> $(ALL_SCRIPT)
	echo "cd \$$BASE_DIR" >> $(ALL_SCRIPT)
	echo "ROOT=\$$PWD" >> $(ALL_SCRIPT)
	echo "if [ \"\$$1\" = \"--summary\" ]; then" >> $(ALL_SCRIPT)
	echo "  OUTPUT=\$$BASE_DIR/output.log" >> $(ALL_SCRIPT)
	echo "  cat /dev/null > \$$OUTPUT" >> $(ALL_SCRIPT)
	echo "else" >> $(ALL_SCRIPT)
	echo "  OUTPUT=/dev/stdout" >> $(ALL_SCRIPT)
	echo "fi" >> $(ALL_SCRIPT)
	echo "export KSFT_TAP_LEVEL=1" >> $(ALL_SCRIPT)
	echo "export skip=4" >> $(ALL_SCRIPT)

	for TARGET in $(TARGETS); do \
		BUILD_TARGET=$$BUILD/$$TARGET;	\
		echo "echo ; echo TAP version 13" >> $(ALL_SCRIPT);	\
		echo "echo Running tests in $$TARGET" >> $(ALL_SCRIPT); \
		echo "echo ========================================" >> $(ALL_SCRIPT); \
		echo "[ -w /dev/kmsg ] && echo \"kselftest: Running tests in $$TARGET\" >> /dev/kmsg" >> $(ALL_SCRIPT); \
		echo "cd $$TARGET" >> $(ALL_SCRIPT); \
		make -s --no-print-directory OUTPUT=$$BUILD_TARGET -C $$TARGET emit_tests >> $(ALL_SCRIPT); \
		echo "cd \$$ROOT" >> $(ALL_SCRIPT); \
	done;

	chmod u+x $(ALL_SCRIPT)
=======
		$(MAKE) OUTPUT=$$BUILD_TARGET -C $$TARGET clean;\
	done;

run_pstore_crash:
	$(MAKE) -C pstore run_crash

# Use $BUILD as the default install root. $BUILD points to the
# right output location for the following cases:
# 1. output_dir=kernel_src
# 2. a separate output directory is specified using O= KBUILD_OUTPUT
# 3. a separate output directory is specified using KBUILD_OUTPUT
# Avoid conflict with INSTALL_PATH set by the main Makefile
#
KSFT_INSTALL_PATH ?= $(BUILD)/kselftest_install
KSFT_INSTALL_PATH := $(abspath $(KSFT_INSTALL_PATH))
# Avoid changing the rest of the logic here and lib.mk.
INSTALL_PATH := $(KSFT_INSTALL_PATH)
ALL_SCRIPT := $(INSTALL_PATH)/run_kselftest.sh
TEST_LIST := $(INSTALL_PATH)/kselftest-list.txt

install: all
ifdef INSTALL_PATH
	@# Ask all targets to install their files
	mkdir -p $(INSTALL_PATH)/kselftest
	install -m 744 kselftest/module.sh $(INSTALL_PATH)/kselftest/
	install -m 744 kselftest/runner.sh $(INSTALL_PATH)/kselftest/
	install -m 744 kselftest/prefix.pl $(INSTALL_PATH)/kselftest/
	install -m 744 run_kselftest.sh $(INSTALL_PATH)/
	rm -f $(TEST_LIST)
	@ret=1;	\
	for TARGET in $(TARGETS); do \
		BUILD_TARGET=$$BUILD/$$TARGET;	\
		$(MAKE) OUTPUT=$$BUILD_TARGET -C $$TARGET INSTALL_PATH=$(INSTALL_PATH)/$$TARGET install \
				$(if $(FORCE_TARGETS),|| exit);	\
		ret=$$((ret * $$?));		\
	done; exit $$ret;


	@# Ask all targets to emit their test scripts
	@# While building kselftest-list.text skip also non-existent TARGET dirs:
	@# they could be the result of a build failure and should NOT be
	@# included in the generated runlist.
	for TARGET in $(TARGETS); do \
		BUILD_TARGET=$$BUILD/$$TARGET;	\
		[ ! -d $(INSTALL_PATH)/$$TARGET ] && echo "Skipping non-existent dir: $$TARGET" && continue; \
		echo -n "Emit Tests for $$TARGET\n"; \
		$(MAKE) -s --no-print-directory OUTPUT=$$BUILD_TARGET COLLECTION=$$TARGET \
			-C $$TARGET emit_tests >> $(TEST_LIST); \
	done;
>>>>>>> upstream/android-13
else
	$(error Error: set INSTALL_PATH to use install)
endif

<<<<<<< HEAD
clean:
	@for TARGET in $(TARGETS); do \
		BUILD_TARGET=$$BUILD/$$TARGET;	\
		make OUTPUT=$$BUILD_TARGET -C $$TARGET clean;\
	done;

.PHONY: all run_tests hotplug run_hotplug clean_hotplug run_pstore_crash install clean
=======
FORMAT ?= .gz
TAR_PATH = $(abspath ${INSTALL_PATH}/kselftest-packages/kselftest.tar${FORMAT})
gen_tar: install
	@mkdir -p ${INSTALL_PATH}/kselftest-packages/
	@tar caf ${TAR_PATH} --exclude=kselftest-packages -C ${INSTALL_PATH} .
	@echo "Created ${TAR_PATH}"

clean:
	@for TARGET in $(TARGETS); do \
		BUILD_TARGET=$$BUILD/$$TARGET;	\
		$(MAKE) OUTPUT=$$BUILD_TARGET -C $$TARGET clean;\
	done;

.PHONY: khdr all run_tests hotplug run_hotplug clean_hotplug run_pstore_crash install clean gen_tar
>>>>>>> upstream/android-13
