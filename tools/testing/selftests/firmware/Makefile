<<<<<<< HEAD
# Makefile for firmware loading selftests

# No binaries, but make sure arg-less "make" doesn't trigger "run_tests"
all:

TEST_PROGS := fw_run_tests.sh
TEST_FILES := fw_fallback.sh fw_filesystem.sh fw_lib.sh

include ../lib.mk

# Nothing to clean up.
clean:
=======
# SPDX-License-Identifier: GPL-2.0-only
# Makefile for firmware loading selftests
CFLAGS = -Wall \
         -O2

TEST_PROGS := fw_run_tests.sh
TEST_FILES := fw_fallback.sh fw_filesystem.sh fw_lib.sh
TEST_GEN_FILES := fw_namespace

include ../lib.mk
>>>>>>> upstream/android-13
