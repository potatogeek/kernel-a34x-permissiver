# SPDX-License-Identifier: GPL-2.0+ OR MIT
<<<<<<< HEAD
CFLAGS += -O2 -Wall -g -I./ -I../../../../usr/include/ -L./ -Wl,-rpath=./
=======

ifneq ($(shell $(CC) --version 2>&1 | head -n 1 | grep clang),)
CLANG_FLAGS += -no-integrated-as
endif

CFLAGS += -O2 -Wall -g -I./ -I../../../../usr/include/ -L$(OUTPUT) -Wl,-rpath=./ \
	  $(CLANG_FLAGS)
>>>>>>> upstream/android-13
LDLIBS += -lpthread

# Own dependencies because we only want to build against 1st prerequisite, but
# still track changes to header files and depend on shared object.
OVERRIDE_TARGETS = 1

TEST_GEN_PROGS = basic_test basic_percpu_ops_test param_test \
		param_test_benchmark param_test_compare_twice

TEST_GEN_PROGS_EXTENDED = librseq.so

TEST_PROGS = run_param_test.sh

<<<<<<< HEAD
=======
TEST_FILES := settings

>>>>>>> upstream/android-13
include ../lib.mk

$(OUTPUT)/librseq.so: rseq.c rseq.h rseq-*.h
	$(CC) $(CFLAGS) -shared -fPIC $< $(LDLIBS) -o $@

$(OUTPUT)/%: %.c $(TEST_GEN_PROGS_EXTENDED) rseq.h rseq-*.h
	$(CC) $(CFLAGS) $< $(LDLIBS) -lrseq -o $@

$(OUTPUT)/param_test_benchmark: param_test.c $(TEST_GEN_PROGS_EXTENDED) \
					rseq.h rseq-*.h
	$(CC) $(CFLAGS) -DBENCHMARK $< $(LDLIBS) -lrseq -o $@

$(OUTPUT)/param_test_compare_twice: param_test.c $(TEST_GEN_PROGS_EXTENDED) \
					rseq.h rseq-*.h
	$(CC) $(CFLAGS) -DRSEQ_COMPARE_TWICE $< $(LDLIBS) -lrseq -o $@
