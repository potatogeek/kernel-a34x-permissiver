# SPDX-License-Identifier: GPL-2.0
noarg:
	$(MAKE) -C ../

<<<<<<< HEAD
TEST_GEN_PROGS := hugetlb_vs_thp_test subpage_prot prot_sao segv_errors
=======
TEST_GEN_PROGS := hugetlb_vs_thp_test subpage_prot prot_sao segv_errors wild_bctr \
		  large_vm_fork_separation bad_accesses pkey_exec_prot \
		  pkey_siginfo stack_expansion_signal stack_expansion_ldst
TEST_PROGS := stress_code_patching.sh

>>>>>>> upstream/android-13
TEST_GEN_PROGS_EXTENDED := tlbie_test
TEST_GEN_FILES := tempfile

top_srcdir = ../../../../..
include ../../lib.mk

<<<<<<< HEAD
$(TEST_GEN_PROGS): ../harness.c

$(OUTPUT)/prot_sao: ../utils.c

=======
$(TEST_GEN_PROGS): ../harness.c ../utils.c

$(OUTPUT)/prot_sao: ../utils.c

$(OUTPUT)/wild_bctr: CFLAGS += -m64
$(OUTPUT)/large_vm_fork_separation: CFLAGS += -m64
$(OUTPUT)/bad_accesses: CFLAGS += -m64
$(OUTPUT)/pkey_exec_prot: CFLAGS += -m64
$(OUTPUT)/pkey_siginfo: CFLAGS += -m64

$(OUTPUT)/stack_expansion_signal: ../utils.c ../pmu/lib.c

$(OUTPUT)/stack_expansion_ldst: CFLAGS += -fno-stack-protector
$(OUTPUT)/stack_expansion_ldst: ../utils.c

>>>>>>> upstream/android-13
$(OUTPUT)/tempfile:
	dd if=/dev/zero of=$@ bs=64k count=1

$(OUTPUT)/tlbie_test: LDLIBS += -lpthread
<<<<<<< HEAD
=======
$(OUTPUT)/pkey_siginfo: LDLIBS += -lpthread
>>>>>>> upstream/android-13
