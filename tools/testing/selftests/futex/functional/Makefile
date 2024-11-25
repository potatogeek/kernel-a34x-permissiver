# SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
INCLUDES := -I../include -I../../
CFLAGS := $(CFLAGS) -g -O2 -Wall -D_GNU_SOURCE -pthread $(INCLUDES)
LDFLAGS := $(LDFLAGS) -pthread -lrt
=======
INCLUDES := -I../include -I../../ -I../../../../../usr/include/ \
	-I$(KBUILD_OUTPUT)/kselftest/usr/include
CFLAGS := $(CFLAGS) -g -O2 -Wall -D_GNU_SOURCE -pthread $(INCLUDES)
LDLIBS := -lpthread -lrt
>>>>>>> upstream/android-13

HEADERS := \
	../include/futextest.h \
	../include/atomic.h \
	../include/logging.h
TEST_GEN_FILES := \
	futex_wait_timeout \
	futex_wait_wouldblock \
	futex_requeue_pi \
	futex_requeue_pi_signal_restart \
	futex_requeue_pi_mismatched_ops \
	futex_wait_uninitialized_heap \
<<<<<<< HEAD
	futex_wait_private_mapped_file
=======
	futex_wait_private_mapped_file \
	futex_wait \
	futex_requeue
>>>>>>> upstream/android-13

TEST_PROGS := run.sh

top_srcdir = ../../../../..
KSFT_KHDR_INSTALL := 1
include ../../lib.mk

$(TEST_GEN_FILES): $(HEADERS)
