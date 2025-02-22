<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Testsuite for eBPF verifier
 *
 * Copyright (c) 2014 PLUMgrid, http://plumgrid.com
 * Copyright (c) 2017 Facebook
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
=======
 * Copyright (c) 2018 Covalent IO, Inc. http://covalent.io
>>>>>>> upstream/android-13
 */

#include <endian.h>
#include <asm/types.h>
#include <linux/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <sched.h>
#include <limits.h>
<<<<<<< HEAD
=======
#include <assert.h>
>>>>>>> upstream/android-13

#include <sys/capability.h>

#include <linux/unistd.h>
#include <linux/filter.h>
#include <linux/bpf_perf_event.h>
#include <linux/bpf.h>
#include <linux/if_ether.h>
<<<<<<< HEAD

#include <bpf/bpf.h>
=======
#include <linux/btf.h>

#include <bpf/bpf.h>
#include <bpf/libbpf.h>
>>>>>>> upstream/android-13

#ifdef HAVE_GENHDR
# include "autoconf.h"
#else
# if defined(__i386) || defined(__x86_64) || defined(__s390x__) || defined(__aarch64__)
#  define CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS 1
# endif
#endif
#include "bpf_rlimit.h"
#include "bpf_rand.h"
#include "bpf_util.h"
<<<<<<< HEAD
#include "../../../include/linux/filter.h"

#define MAX_INSNS	BPF_MAXINSNS
#define MAX_FIXUPS	8
#define MAX_NR_MAPS	8
=======
#include "test_btf.h"
#include "../../../include/linux/filter.h"

#define MAX_INSNS	BPF_MAXINSNS
#define MAX_TEST_INSNS	1000000
#define MAX_FIXUPS	8
#define MAX_NR_MAPS	21
#define MAX_TEST_RUNS	8
>>>>>>> upstream/android-13
#define POINTER_VALUE	0xcafe4all
#define TEST_DATA_LEN	64

#define F_NEEDS_EFFICIENT_UNALIGNED_ACCESS	(1 << 0)
#define F_LOAD_WITH_STRICT_ALIGNMENT		(1 << 1)

#define UNPRIV_SYSCTL "kernel/unprivileged_bpf_disabled"
static bool unpriv_disabled = false;
<<<<<<< HEAD
=======
static int skips;
static bool verbose = false;
>>>>>>> upstream/android-13

struct bpf_test {
	const char *descr;
	struct bpf_insn	insns[MAX_INSNS];
<<<<<<< HEAD
	int fixup_map1[MAX_FIXUPS];
	int fixup_map2[MAX_FIXUPS];
	int fixup_map3[MAX_FIXUPS];
	int fixup_map4[MAX_FIXUPS];
=======
	struct bpf_insn	*fill_insns;
	int fixup_map_hash_8b[MAX_FIXUPS];
	int fixup_map_hash_48b[MAX_FIXUPS];
	int fixup_map_hash_16b[MAX_FIXUPS];
	int fixup_map_array_48b[MAX_FIXUPS];
	int fixup_map_sockmap[MAX_FIXUPS];
	int fixup_map_sockhash[MAX_FIXUPS];
	int fixup_map_xskmap[MAX_FIXUPS];
	int fixup_map_stacktrace[MAX_FIXUPS];
>>>>>>> upstream/android-13
	int fixup_prog1[MAX_FIXUPS];
	int fixup_prog2[MAX_FIXUPS];
	int fixup_map_in_map[MAX_FIXUPS];
	int fixup_cgroup_storage[MAX_FIXUPS];
<<<<<<< HEAD
	const char *errstr;
	const char *errstr_unpriv;
	uint32_t retval;
	enum {
		UNDEF,
		ACCEPT,
		REJECT
	} result, result_unpriv;
	enum bpf_prog_type prog_type;
	uint8_t flags;
	__u8 data[TEST_DATA_LEN];
	void (*fill_helper)(struct bpf_test *self);
=======
	int fixup_percpu_cgroup_storage[MAX_FIXUPS];
	int fixup_map_spin_lock[MAX_FIXUPS];
	int fixup_map_array_ro[MAX_FIXUPS];
	int fixup_map_array_wo[MAX_FIXUPS];
	int fixup_map_array_small[MAX_FIXUPS];
	int fixup_sk_storage_map[MAX_FIXUPS];
	int fixup_map_event_output[MAX_FIXUPS];
	int fixup_map_reuseport_array[MAX_FIXUPS];
	int fixup_map_ringbuf[MAX_FIXUPS];
	/* Expected verifier log output for result REJECT or VERBOSE_ACCEPT.
	 * Can be a tab-separated sequence of expected strings. An empty string
	 * means no log verification.
	 */
	const char *errstr;
	const char *errstr_unpriv;
	uint32_t insn_processed;
	int prog_len;
	enum {
		UNDEF,
		ACCEPT,
		REJECT,
		VERBOSE_ACCEPT,
	} result, result_unpriv;
	enum bpf_prog_type prog_type;
	uint8_t flags;
	void (*fill_helper)(struct bpf_test *self);
	int runs;
#define bpf_testdata_struct_t					\
	struct {						\
		uint32_t retval, retval_unpriv;			\
		union {						\
			__u8 data[TEST_DATA_LEN];		\
			__u64 data64[TEST_DATA_LEN / 8];	\
		};						\
	}
	union {
		bpf_testdata_struct_t;
		bpf_testdata_struct_t retvals[MAX_TEST_RUNS];
	};
	enum bpf_attach_type expected_attach_type;
	const char *kfunc;
>>>>>>> upstream/android-13
};

/* Note we want this to be 64 bit aligned so that the end of our array is
 * actually the end of the structure.
 */
#define MAX_ENTRIES 11

struct test_val {
	unsigned int index;
	int foo[MAX_ENTRIES];
};

struct other_val {
	long long foo;
	long long bar;
};

static void bpf_fill_ld_abs_vlan_push_pop(struct bpf_test *self)
{
<<<<<<< HEAD
	/* test: {skb->data[0], vlan_push} x 68 + {skb->data[0], vlan_pop} x 68 */
#define PUSH_CNT 51
	unsigned int len = BPF_MAXINSNS;
	struct bpf_insn *insn = self->insns;
=======
	/* test: {skb->data[0], vlan_push} x 51 + {skb->data[0], vlan_pop} x 51 */
#define PUSH_CNT 51
	/* jump range is limited to 16 bit. PUSH_CNT of ld_abs needs room */
	unsigned int len = (1 << 15) - PUSH_CNT * 2 * 5 * 6;
	struct bpf_insn *insn = self->fill_insns;
>>>>>>> upstream/android-13
	int i = 0, j, k = 0;

	insn[i++] = BPF_MOV64_REG(BPF_REG_6, BPF_REG_1);
loop:
	for (j = 0; j < PUSH_CNT; j++) {
		insn[i++] = BPF_LD_ABS(BPF_B, 0);
<<<<<<< HEAD
		insn[i] = BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0x34, len - i - 2);
=======
		/* jump to error label */
		insn[i] = BPF_JMP32_IMM(BPF_JNE, BPF_REG_0, 0x34, len - i - 3);
>>>>>>> upstream/android-13
		i++;
		insn[i++] = BPF_MOV64_REG(BPF_REG_1, BPF_REG_6);
		insn[i++] = BPF_MOV64_IMM(BPF_REG_2, 1);
		insn[i++] = BPF_MOV64_IMM(BPF_REG_3, 2);
		insn[i++] = BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
					 BPF_FUNC_skb_vlan_push),
<<<<<<< HEAD
		insn[i] = BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, len - i - 2);
=======
		insn[i] = BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, len - i - 3);
>>>>>>> upstream/android-13
		i++;
	}

	for (j = 0; j < PUSH_CNT; j++) {
		insn[i++] = BPF_LD_ABS(BPF_B, 0);
<<<<<<< HEAD
		insn[i] = BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0x34, len - i - 2);
=======
		insn[i] = BPF_JMP32_IMM(BPF_JNE, BPF_REG_0, 0x34, len - i - 3);
>>>>>>> upstream/android-13
		i++;
		insn[i++] = BPF_MOV64_REG(BPF_REG_1, BPF_REG_6);
		insn[i++] = BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
					 BPF_FUNC_skb_vlan_pop),
<<<<<<< HEAD
		insn[i] = BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, len - i - 2);
=======
		insn[i] = BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, len - i - 3);
>>>>>>> upstream/android-13
		i++;
	}
	if (++k < 5)
		goto loop;

<<<<<<< HEAD
	for (; i < len - 1; i++)
		insn[i] = BPF_ALU32_IMM(BPF_MOV, BPF_REG_0, 0xbef);
	insn[len - 1] = BPF_EXIT_INSN();
=======
	for (; i < len - 3; i++)
		insn[i] = BPF_ALU64_IMM(BPF_MOV, BPF_REG_0, 0xbef);
	insn[len - 3] = BPF_JMP_A(1);
	/* error label */
	insn[len - 2] = BPF_MOV32_IMM(BPF_REG_0, 0);
	insn[len - 1] = BPF_EXIT_INSN();
	self->prog_len = len;
>>>>>>> upstream/android-13
}

static void bpf_fill_jump_around_ld_abs(struct bpf_test *self)
{
<<<<<<< HEAD
	struct bpf_insn *insn = self->insns;
	unsigned int len = BPF_MAXINSNS;
=======
	struct bpf_insn *insn = self->fill_insns;
	/* jump range is limited to 16 bit. every ld_abs is replaced by 6 insns,
	 * but on arches like arm, ppc etc, there will be one BPF_ZEXT inserted
	 * to extend the error value of the inlined ld_abs sequence which then
	 * contains 7 insns. so, set the dividend to 7 so the testcase could
	 * work on all arches.
	 */
	unsigned int len = (1 << 15) / 7;
>>>>>>> upstream/android-13
	int i = 0;

	insn[i++] = BPF_MOV64_REG(BPF_REG_6, BPF_REG_1);
	insn[i++] = BPF_LD_ABS(BPF_B, 0);
	insn[i] = BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 10, len - i - 2);
	i++;
	while (i < len - 1)
		insn[i++] = BPF_LD_ABS(BPF_B, 1);
	insn[i] = BPF_EXIT_INSN();
<<<<<<< HEAD
=======
	self->prog_len = i + 1;
>>>>>>> upstream/android-13
}

static void bpf_fill_rand_ld_dw(struct bpf_test *self)
{
<<<<<<< HEAD
	struct bpf_insn *insn = self->insns;
=======
	struct bpf_insn *insn = self->fill_insns;
>>>>>>> upstream/android-13
	uint64_t res = 0;
	int i = 0;

	insn[i++] = BPF_MOV32_IMM(BPF_REG_0, 0);
	while (i < self->retval) {
		uint64_t val = bpf_semi_rand_get();
		struct bpf_insn tmp[2] = { BPF_LD_IMM64(BPF_REG_1, val) };

		res ^= val;
		insn[i++] = tmp[0];
		insn[i++] = tmp[1];
		insn[i++] = BPF_ALU64_REG(BPF_XOR, BPF_REG_0, BPF_REG_1);
	}
	insn[i++] = BPF_MOV64_REG(BPF_REG_1, BPF_REG_0);
	insn[i++] = BPF_ALU64_IMM(BPF_RSH, BPF_REG_1, 32);
	insn[i++] = BPF_ALU64_REG(BPF_XOR, BPF_REG_0, BPF_REG_1);
	insn[i] = BPF_EXIT_INSN();
<<<<<<< HEAD
=======
	self->prog_len = i + 1;
>>>>>>> upstream/android-13
	res ^= (res >> 32);
	self->retval = (uint32_t)res;
}

<<<<<<< HEAD
static struct bpf_test tests[] = {
	{
		"add+sub+mul",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 1),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 2),
			BPF_MOV64_IMM(BPF_REG_2, 3),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -1),
			BPF_ALU64_IMM(BPF_MUL, BPF_REG_1, 3),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = -3,
	},
	{
		"DIV32 by 0, zero check 1",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_0, 42),
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_MOV32_IMM(BPF_REG_2, 1),
			BPF_ALU32_REG(BPF_DIV, BPF_REG_2, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 42,
	},
	{
		"DIV32 by 0, zero check 2",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_0, 42),
			BPF_LD_IMM64(BPF_REG_1, 0xffffffff00000000LL),
			BPF_MOV32_IMM(BPF_REG_2, 1),
			BPF_ALU32_REG(BPF_DIV, BPF_REG_2, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 42,
	},
	{
		"DIV64 by 0, zero check",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_0, 42),
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_MOV32_IMM(BPF_REG_2, 1),
			BPF_ALU64_REG(BPF_DIV, BPF_REG_2, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 42,
	},
	{
		"MOD32 by 0, zero check 1",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_0, 42),
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_MOV32_IMM(BPF_REG_2, 1),
			BPF_ALU32_REG(BPF_MOD, BPF_REG_2, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 42,
	},
	{
		"MOD32 by 0, zero check 2",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_0, 42),
			BPF_LD_IMM64(BPF_REG_1, 0xffffffff00000000LL),
			BPF_MOV32_IMM(BPF_REG_2, 1),
			BPF_ALU32_REG(BPF_MOD, BPF_REG_2, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 42,
	},
	{
		"MOD64 by 0, zero check",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_0, 42),
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_MOV32_IMM(BPF_REG_2, 1),
			BPF_ALU64_REG(BPF_MOD, BPF_REG_2, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 42,
	},
	{
		"DIV32 by 0, zero check ok, cls",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_0, 42),
			BPF_MOV32_IMM(BPF_REG_1, 2),
			BPF_MOV32_IMM(BPF_REG_2, 16),
			BPF_ALU32_REG(BPF_DIV, BPF_REG_2, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 8,
	},
	{
		"DIV32 by 0, zero check 1, cls",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_MOV32_IMM(BPF_REG_0, 1),
			BPF_ALU32_REG(BPF_DIV, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 0,
	},
	{
		"DIV32 by 0, zero check 2, cls",
		.insns = {
			BPF_LD_IMM64(BPF_REG_1, 0xffffffff00000000LL),
			BPF_MOV32_IMM(BPF_REG_0, 1),
			BPF_ALU32_REG(BPF_DIV, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 0,
	},
	{
		"DIV64 by 0, zero check, cls",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_MOV32_IMM(BPF_REG_0, 1),
			BPF_ALU64_REG(BPF_DIV, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 0,
	},
	{
		"MOD32 by 0, zero check ok, cls",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_0, 42),
			BPF_MOV32_IMM(BPF_REG_1, 3),
			BPF_MOV32_IMM(BPF_REG_2, 5),
			BPF_ALU32_REG(BPF_MOD, BPF_REG_2, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 2,
	},
	{
		"MOD32 by 0, zero check 1, cls",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_MOV32_IMM(BPF_REG_0, 1),
			BPF_ALU32_REG(BPF_MOD, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"MOD32 by 0, zero check 2, cls",
		.insns = {
			BPF_LD_IMM64(BPF_REG_1, 0xffffffff00000000LL),
			BPF_MOV32_IMM(BPF_REG_0, 1),
			BPF_ALU32_REG(BPF_MOD, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"MOD64 by 0, zero check 1, cls",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_MOV32_IMM(BPF_REG_0, 2),
			BPF_ALU64_REG(BPF_MOD, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 2,
	},
	{
		"MOD64 by 0, zero check 2, cls",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_MOV32_IMM(BPF_REG_0, -1),
			BPF_ALU64_REG(BPF_MOD, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = -1,
	},
	/* Just make sure that JITs used udiv/umod as otherwise we get
	 * an exception from INT_MIN/-1 overflow similarly as with div
	 * by zero.
	 */
	{
		"DIV32 overflow, check 1",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_1, -1),
			BPF_MOV32_IMM(BPF_REG_0, INT_MIN),
			BPF_ALU32_REG(BPF_DIV, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 0,
	},
	{
		"DIV32 overflow, check 2",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_0, INT_MIN),
			BPF_ALU32_IMM(BPF_DIV, BPF_REG_0, -1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 0,
	},
	{
		"DIV64 overflow, check 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, -1),
			BPF_LD_IMM64(BPF_REG_0, LLONG_MIN),
			BPF_ALU64_REG(BPF_DIV, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 0,
	},
	{
		"DIV64 overflow, check 2",
		.insns = {
			BPF_LD_IMM64(BPF_REG_0, LLONG_MIN),
			BPF_ALU64_IMM(BPF_DIV, BPF_REG_0, -1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 0,
	},
	{
		"MOD32 overflow, check 1",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_1, -1),
			BPF_MOV32_IMM(BPF_REG_0, INT_MIN),
			BPF_ALU32_REG(BPF_MOD, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = INT_MIN,
	},
	{
		"MOD32 overflow, check 2",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_0, INT_MIN),
			BPF_ALU32_IMM(BPF_MOD, BPF_REG_0, -1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = INT_MIN,
	},
	{
		"MOD64 overflow, check 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, -1),
			BPF_LD_IMM64(BPF_REG_2, LLONG_MIN),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_2),
			BPF_ALU64_REG(BPF_MOD, BPF_REG_2, BPF_REG_1),
			BPF_MOV32_IMM(BPF_REG_0, 0),
			BPF_JMP_REG(BPF_JNE, BPF_REG_3, BPF_REG_2, 1),
			BPF_MOV32_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"MOD64 overflow, check 2",
		.insns = {
			BPF_LD_IMM64(BPF_REG_2, LLONG_MIN),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_2),
			BPF_ALU64_IMM(BPF_MOD, BPF_REG_2, -1),
			BPF_MOV32_IMM(BPF_REG_0, 0),
			BPF_JMP_REG(BPF_JNE, BPF_REG_3, BPF_REG_2, 1),
			BPF_MOV32_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"xor32 zero extend check",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_2, -1),
			BPF_ALU64_IMM(BPF_LSH, BPF_REG_2, 32),
			BPF_ALU64_IMM(BPF_OR, BPF_REG_2, 0xffff),
			BPF_ALU32_REG(BPF_XOR, BPF_REG_2, BPF_REG_2),
			BPF_MOV32_IMM(BPF_REG_0, 2),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_2, 0, 1),
			BPF_MOV32_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"empty prog",
		.insns = {
		},
		.errstr = "unknown opcode 00",
		.result = REJECT,
	},
	{
		"only exit insn",
		.insns = {
			BPF_EXIT_INSN(),
		},
		.errstr = "R0 !read_ok",
		.result = REJECT,
	},
	{
		"unreachable",
		.insns = {
			BPF_EXIT_INSN(),
			BPF_EXIT_INSN(),
		},
		.errstr = "unreachable",
		.result = REJECT,
	},
	{
		"unreachable2",
		.insns = {
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "unreachable",
		.result = REJECT,
	},
	{
		"out of range jump",
		.insns = {
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_EXIT_INSN(),
		},
		.errstr = "jump out of range",
		.result = REJECT,
	},
	{
		"out of range jump2",
		.insns = {
			BPF_JMP_IMM(BPF_JA, 0, 0, -2),
			BPF_EXIT_INSN(),
		},
		.errstr = "jump out of range",
		.result = REJECT,
	},
	{
		"test1 ld_imm64",
		.insns = {
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 1),
			BPF_LD_IMM64(BPF_REG_0, 0),
			BPF_LD_IMM64(BPF_REG_0, 0),
			BPF_LD_IMM64(BPF_REG_0, 1),
			BPF_LD_IMM64(BPF_REG_0, 1),
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid BPF_LD_IMM insn",
		.errstr_unpriv = "R1 pointer comparison",
		.result = REJECT,
	},
	{
		"test2 ld_imm64",
		.insns = {
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 1),
			BPF_LD_IMM64(BPF_REG_0, 0),
			BPF_LD_IMM64(BPF_REG_0, 0),
			BPF_LD_IMM64(BPF_REG_0, 1),
			BPF_LD_IMM64(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid BPF_LD_IMM insn",
		.errstr_unpriv = "R1 pointer comparison",
		.result = REJECT,
	},
	{
		"test3 ld_imm64",
		.insns = {
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 1),
			BPF_RAW_INSN(BPF_LD | BPF_IMM | BPF_DW, 0, 0, 0, 0),
			BPF_LD_IMM64(BPF_REG_0, 0),
			BPF_LD_IMM64(BPF_REG_0, 0),
			BPF_LD_IMM64(BPF_REG_0, 1),
			BPF_LD_IMM64(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_ld_imm64 insn",
		.result = REJECT,
	},
	{
		"test4 ld_imm64",
		.insns = {
			BPF_RAW_INSN(BPF_LD | BPF_IMM | BPF_DW, 0, 0, 0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_ld_imm64 insn",
		.result = REJECT,
	},
	{
		"test5 ld_imm64",
		.insns = {
			BPF_RAW_INSN(BPF_LD | BPF_IMM | BPF_DW, 0, 0, 0, 0),
		},
		.errstr = "invalid bpf_ld_imm64 insn",
		.result = REJECT,
	},
	{
		"test6 ld_imm64",
		.insns = {
			BPF_RAW_INSN(BPF_LD | BPF_IMM | BPF_DW, 0, 0, 0, 0),
			BPF_RAW_INSN(0, 0, 0, 0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"test7 ld_imm64",
		.insns = {
			BPF_RAW_INSN(BPF_LD | BPF_IMM | BPF_DW, 0, 0, 0, 1),
			BPF_RAW_INSN(0, 0, 0, 0, 1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"test8 ld_imm64",
		.insns = {
			BPF_RAW_INSN(BPF_LD | BPF_IMM | BPF_DW, 0, 0, 1, 1),
			BPF_RAW_INSN(0, 0, 0, 0, 1),
			BPF_EXIT_INSN(),
		},
		.errstr = "uses reserved fields",
		.result = REJECT,
	},
	{
		"test9 ld_imm64",
		.insns = {
			BPF_RAW_INSN(BPF_LD | BPF_IMM | BPF_DW, 0, 0, 0, 1),
			BPF_RAW_INSN(0, 0, 0, 1, 1),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_ld_imm64 insn",
		.result = REJECT,
	},
	{
		"test10 ld_imm64",
		.insns = {
			BPF_RAW_INSN(BPF_LD | BPF_IMM | BPF_DW, 0, 0, 0, 1),
			BPF_RAW_INSN(0, BPF_REG_1, 0, 0, 1),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_ld_imm64 insn",
		.result = REJECT,
	},
	{
		"test11 ld_imm64",
		.insns = {
			BPF_RAW_INSN(BPF_LD | BPF_IMM | BPF_DW, 0, 0, 0, 1),
			BPF_RAW_INSN(0, 0, BPF_REG_1, 0, 1),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_ld_imm64 insn",
		.result = REJECT,
	},
	{
		"test12 ld_imm64",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_LD | BPF_IMM | BPF_DW, 0, BPF_REG_1, 0, 1),
			BPF_RAW_INSN(0, 0, 0, 0, 1),
			BPF_EXIT_INSN(),
		},
		.errstr = "not pointing to valid bpf_map",
		.result = REJECT,
	},
	{
		"test13 ld_imm64",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_LD | BPF_IMM | BPF_DW, 0, BPF_REG_1, 0, 1),
			BPF_RAW_INSN(0, 0, BPF_REG_1, 0, 1),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_ld_imm64 insn",
		.result = REJECT,
	},
	{
		"arsh32 on imm",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_ALU32_IMM(BPF_ARSH, BPF_REG_0, 5),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "unknown opcode c4",
	},
	{
		"arsh32 on reg",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_MOV64_IMM(BPF_REG_1, 5),
			BPF_ALU32_REG(BPF_ARSH, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "unknown opcode cc",
	},
	{
		"arsh64 on imm",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_ALU64_IMM(BPF_ARSH, BPF_REG_0, 5),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"arsh64 on reg",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_MOV64_IMM(BPF_REG_1, 5),
			BPF_ALU64_REG(BPF_ARSH, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"no bpf_exit",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_0, BPF_REG_2),
		},
		.errstr = "not an exit",
		.result = REJECT,
	},
	{
		"loop (back-edge)",
		.insns = {
			BPF_JMP_IMM(BPF_JA, 0, 0, -1),
			BPF_EXIT_INSN(),
		},
		.errstr = "back-edge",
		.result = REJECT,
	},
	{
		"loop2 (back-edge)",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_0),
			BPF_JMP_IMM(BPF_JA, 0, 0, -4),
			BPF_EXIT_INSN(),
		},
		.errstr = "back-edge",
		.result = REJECT,
	},
	{
		"conditional loop",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_0),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, -3),
			BPF_EXIT_INSN(),
		},
		.errstr = "back-edge",
		.result = REJECT,
	},
	{
		"read uninitialized register",
		.insns = {
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_EXIT_INSN(),
		},
		.errstr = "R2 !read_ok",
		.result = REJECT,
	},
	{
		"read invalid register",
		.insns = {
			BPF_MOV64_REG(BPF_REG_0, -1),
			BPF_EXIT_INSN(),
		},
		.errstr = "R15 is invalid",
		.result = REJECT,
	},
	{
		"program doesn't init R0 before exit",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_2, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.errstr = "R0 !read_ok",
		.result = REJECT,
	},
	{
		"program doesn't init R0 before exit in all branches",
		.insns = {
			BPF_JMP_IMM(BPF_JGE, BPF_REG_1, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.errstr = "R0 !read_ok",
		.errstr_unpriv = "R1 pointer comparison",
		.result = REJECT,
	},
	{
		"stack out of bounds",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, 8, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid stack",
		.result = REJECT,
	},
	{
		"invalid call insn1",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL | BPF_X, 0, 0, 0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "unknown opcode 8d",
		.result = REJECT,
	},
	{
		"invalid call insn2",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 1, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "BPF_CALL uses reserved",
		.result = REJECT,
	},
	{
		"invalid function call",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, 1234567),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid func unknown#1234567",
		.result = REJECT,
	},
	{
		"uninitialized stack1",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 2 },
		.errstr = "invalid indirect read from stack",
		.result = REJECT,
	},
	{
		"uninitialized stack2",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_2, -8),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid read from stack",
		.result = REJECT,
	},
	{
		"invalid fp arithmetic",
		/* If this gets ever changed, make sure JITs can deal with it. */
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_1, 8),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 subtraction from stack pointer",
		.result = REJECT,
	},
	{
		"non-invalid fp arithmetic",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"invalid argument register",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_cgroup_classid),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_cgroup_classid),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 !read_ok",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"non-invalid argument register",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_cgroup_classid),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_1, BPF_REG_6),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_cgroup_classid),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"check valid spill/fill",
		.insns = {
			/* spill R1(ctx) into stack */
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_1, -8),
			/* fill it back into R2 */
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_10, -8),
			/* should be able to access R0 = *(R2 + 8) */
			/* BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_2, 8), */
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "R0 leaks addr",
		.result = ACCEPT,
		.result_unpriv = REJECT,
		.retval = POINTER_VALUE,
	},
	{
		"check valid spill/fill, skb mark",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_1),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_6, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_10, -8),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_0,
				    offsetof(struct __sk_buff, mark)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.result_unpriv = ACCEPT,
	},
	{
		"check corrupted spill/fill",
		.insns = {
			/* spill R1(ctx) into stack */
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_1, -8),
			/* mess up with R1 pointer on stack */
			BPF_ST_MEM(BPF_B, BPF_REG_10, -7, 0x23),
			/* fill back into R0 should fail */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_10, -8),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "attempt to corrupt spilled",
		.errstr = "corrupted spill",
		.result = REJECT,
	},
	{
		"invalid src register in STX",
		.insns = {
			BPF_STX_MEM(BPF_B, BPF_REG_10, -1, -1),
			BPF_EXIT_INSN(),
		},
		.errstr = "R15 is invalid",
		.result = REJECT,
	},
	{
		"invalid dst register in STX",
		.insns = {
			BPF_STX_MEM(BPF_B, 14, BPF_REG_10, -1),
			BPF_EXIT_INSN(),
		},
		.errstr = "R14 is invalid",
		.result = REJECT,
	},
	{
		"invalid dst register in ST",
		.insns = {
			BPF_ST_MEM(BPF_B, 14, -1, -1),
			BPF_EXIT_INSN(),
		},
		.errstr = "R14 is invalid",
		.result = REJECT,
	},
	{
		"invalid src register in LDX",
		.insns = {
			BPF_LDX_MEM(BPF_B, BPF_REG_0, 12, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R12 is invalid",
		.result = REJECT,
	},
	{
		"invalid dst register in LDX",
		.insns = {
			BPF_LDX_MEM(BPF_B, 11, BPF_REG_1, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R11 is invalid",
		.result = REJECT,
	},
	{
		"junk insn",
		.insns = {
			BPF_RAW_INSN(0, 0, 0, 0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "unknown opcode 00",
		.result = REJECT,
	},
	{
		"junk insn2",
		.insns = {
			BPF_RAW_INSN(1, 0, 0, 0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "BPF_LDX uses reserved fields",
		.result = REJECT,
	},
	{
		"junk insn3",
		.insns = {
			BPF_RAW_INSN(-1, 0, 0, 0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "unknown opcode ff",
		.result = REJECT,
	},
	{
		"junk insn4",
		.insns = {
			BPF_RAW_INSN(-1, -1, -1, -1, -1),
			BPF_EXIT_INSN(),
		},
		.errstr = "unknown opcode ff",
		.result = REJECT,
	},
	{
		"junk insn5",
		.insns = {
			BPF_RAW_INSN(0x7f, -1, -1, -1, -1),
			BPF_EXIT_INSN(),
		},
		.errstr = "BPF_ALU uses reserved fields",
		.result = REJECT,
	},
	{
		"misaligned read from stack",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_2, -4),
			BPF_EXIT_INSN(),
		},
		.errstr = "misaligned stack access",
		.result = REJECT,
	},
	{
		"invalid map_fd for function call",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_delete_elem),
			BPF_EXIT_INSN(),
		},
		.errstr = "fd 0 is not pointing to valid bpf_map",
		.result = REJECT,
	},
	{
		"don't check return value before access",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "R0 invalid mem access 'map_value_or_null'",
		.result = REJECT,
	},
	{
		"access memory with incorrect alignment",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 4, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "misaligned value access",
		.result = REJECT,
		.flags = F_LOAD_WITH_STRICT_ALIGNMENT,
	},
	{
		"sometimes access memory with incorrect alignment",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),
			BPF_EXIT_INSN(),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 1),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "R0 invalid mem access",
		.errstr_unpriv = "R0 leaks addr",
		.result = REJECT,
		.flags = F_LOAD_WITH_STRICT_ALIGNMENT,
	},
	{
		"jump test 1",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1, -8),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -8, 0),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 1, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -16, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 2, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -8, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 3, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -16, 3),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 4, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -8, 4),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 5, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -32, 5),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "R1 pointer comparison",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"jump test 2",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 2),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -8, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 14),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 1, 2),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -16, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 11),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 2, 2),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -32, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 8),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 3, 2),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -40, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 5),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 4, 2),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -48, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 5, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -56, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "R1 pointer comparison",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"jump test 3",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 3),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -8, 0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_JMP_IMM(BPF_JA, 0, 0, 19),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 1, 3),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -16, 0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -16),
			BPF_JMP_IMM(BPF_JA, 0, 0, 15),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 2, 3),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -32, 0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -32),
			BPF_JMP_IMM(BPF_JA, 0, 0, 11),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 3, 3),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -40, 0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -40),
			BPF_JMP_IMM(BPF_JA, 0, 0, 7),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 4, 3),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -48, 0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -48),
			BPF_JMP_IMM(BPF_JA, 0, 0, 3),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 5, 0),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, -56, 0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -56),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_delete_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 24 },
		.errstr_unpriv = "R1 pointer comparison",
		.result_unpriv = REJECT,
		.result = ACCEPT,
		.retval = -ENOENT,
	},
	{
		"jump test 4",
		.insns = {
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 3),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 4),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 3),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 4),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 3),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 4),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 3),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 4),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 3),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 4),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 3),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 4),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 3),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 4),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 3),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 4),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 3),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 4),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 0),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 0),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 0),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, BPF_REG_10, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "R1 pointer comparison",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"jump test 5",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_2),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_1, 0, 2),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_3, -8),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_2, -8),
			BPF_JMP_IMM(BPF_JA, 0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_1, 0, 2),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_3, -8),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_2, -8),
			BPF_JMP_IMM(BPF_JA, 0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_1, 0, 2),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_3, -8),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_2, -8),
			BPF_JMP_IMM(BPF_JA, 0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_1, 0, 2),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_3, -8),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_2, -8),
			BPF_JMP_IMM(BPF_JA, 0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_1, 0, 2),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_3, -8),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_2, -8),
			BPF_JMP_IMM(BPF_JA, 0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "R1 pointer comparison",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"access skb fields ok",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, len)),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_0, 0, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_0, 0, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, pkt_type)),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_0, 0, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, queue_mapping)),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_0, 0, 0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, protocol)),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_0, 0, 0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, vlan_present)),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_0, 0, 0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, vlan_tci)),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_0, 0, 0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, napi_id)),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_0, 0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"access skb fields bad1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, -4),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"access skb fields bad2",
		.insns = {
			BPF_JMP_IMM(BPF_JGE, BPF_REG_1, 0, 9),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, pkt_type)),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 4 },
		.errstr = "different pointers",
		.errstr_unpriv = "R1 pointer comparison",
		.result = REJECT,
	},
	{
		"access skb fields bad3",
		.insns = {
			BPF_JMP_IMM(BPF_JGE, BPF_REG_1, 0, 2),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, pkt_type)),
			BPF_EXIT_INSN(),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_JMP_IMM(BPF_JA, 0, 0, -12),
		},
		.fixup_map1 = { 6 },
		.errstr = "different pointers",
		.errstr_unpriv = "R1 pointer comparison",
		.result = REJECT,
	},
	{
		"access skb fields bad4",
		.insns = {
			BPF_JMP_IMM(BPF_JGE, BPF_REG_1, 0, 3),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_1,
				    offsetof(struct __sk_buff, len)),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_JMP_IMM(BPF_JA, 0, 0, -13),
		},
		.fixup_map1 = { 7 },
		.errstr = "different pointers",
		.errstr_unpriv = "R1 pointer comparison",
		.result = REJECT,
	},
	{
		"invalid access __sk_buff family",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, family)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"invalid access __sk_buff remote_ip4",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, remote_ip4)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"invalid access __sk_buff local_ip4",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, local_ip4)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"invalid access __sk_buff remote_ip6",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, remote_ip6)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"invalid access __sk_buff local_ip6",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, local_ip6)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"invalid access __sk_buff remote_port",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, remote_port)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"invalid access __sk_buff remote_port",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, local_port)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"valid access __sk_buff family",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, family)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"valid access __sk_buff remote_ip4",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, remote_ip4)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"valid access __sk_buff local_ip4",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, local_ip4)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"valid access __sk_buff remote_ip6",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, remote_ip6[0])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, remote_ip6[1])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, remote_ip6[2])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, remote_ip6[3])),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"valid access __sk_buff local_ip6",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, local_ip6[0])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, local_ip6[1])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, local_ip6[2])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, local_ip6[3])),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"valid access __sk_buff remote_port",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, remote_port)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"valid access __sk_buff remote_port",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, local_port)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"invalid access of tc_classid for SK_SKB",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, tc_classid)),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
		.errstr = "invalid bpf_context access",
	},
	{
		"invalid access of skb->mark for SK_SKB",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_EXIT_INSN(),
		},
		.result =  REJECT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
		.errstr = "invalid bpf_context access",
	},
	{
		"check skb->mark is not writeable by SK_SKB",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, mark)),
			BPF_EXIT_INSN(),
		},
		.result =  REJECT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
		.errstr = "invalid bpf_context access",
	},
	{
		"check skb->tc_index is writeable by SK_SKB",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, tc_index)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"check skb->priority is writeable by SK_SKB",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, priority)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"direct packet read for SK_SKB",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"direct packet write for SK_SKB",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_STX_MEM(BPF_B, BPF_REG_2, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"overlapping checks for direct packet access SK_SKB",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 6),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_2, 6),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"valid access family in SK_MSG",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct sk_msg_md, family)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_MSG,
	},
	{
		"valid access remote_ip4 in SK_MSG",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct sk_msg_md, remote_ip4)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_MSG,
	},
	{
		"valid access local_ip4 in SK_MSG",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct sk_msg_md, local_ip4)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_MSG,
	},
	{
		"valid access remote_port in SK_MSG",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct sk_msg_md, remote_port)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_MSG,
	},
	{
		"valid access local_port in SK_MSG",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct sk_msg_md, local_port)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_MSG,
	},
	{
		"valid access remote_ip6 in SK_MSG",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct sk_msg_md, remote_ip6[0])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct sk_msg_md, remote_ip6[1])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct sk_msg_md, remote_ip6[2])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct sk_msg_md, remote_ip6[3])),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"valid access local_ip6 in SK_MSG",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct sk_msg_md, local_ip6[0])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct sk_msg_md, local_ip6[1])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct sk_msg_md, local_ip6[2])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct sk_msg_md, local_ip6[3])),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_SKB,
	},
	{
		"invalid 64B read of family in SK_MSG",
		.insns = {
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1,
				    offsetof(struct sk_msg_md, family)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SK_MSG,
	},
	{
		"invalid read past end of SK_MSG",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct sk_msg_md, local_port) + 4),
			BPF_EXIT_INSN(),
		},
		.errstr = "R0 !read_ok",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SK_MSG,
	},
	{
		"invalid read offset in SK_MSG",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct sk_msg_md, family) + 1),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SK_MSG,
	},
	{
		"direct packet read for SK_MSG",
		.insns = {
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1,
				    offsetof(struct sk_msg_md, data)),
			BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_1,
				    offsetof(struct sk_msg_md, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_MSG,
	},
	{
		"direct packet write for SK_MSG",
		.insns = {
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1,
				    offsetof(struct sk_msg_md, data)),
			BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_1,
				    offsetof(struct sk_msg_md, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_STX_MEM(BPF_B, BPF_REG_2, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_MSG,
	},
	{
		"overlapping checks for direct packet access SK_MSG",
		.insns = {
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1,
				    offsetof(struct sk_msg_md, data)),
			BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_1,
				    offsetof(struct sk_msg_md, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 6),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_2, 6),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SK_MSG,
	},
	{
		"check skb->mark is not writeable by sockets",
		.insns = {
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.errstr_unpriv = "R1 leaks addr",
		.result = REJECT,
	},
	{
		"check skb->tc_index is not writeable by sockets",
		.insns = {
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_1,
				    offsetof(struct __sk_buff, tc_index)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.errstr_unpriv = "R1 leaks addr",
		.result = REJECT,
	},
	{
		"check cb access: byte",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0]) + 1),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0]) + 2),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0]) + 3),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[1])),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[1]) + 1),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[1]) + 2),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[1]) + 3),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[2])),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[2]) + 1),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[2]) + 2),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[2]) + 3),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[3])),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[3]) + 1),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[3]) + 2),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[3]) + 3),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[4])),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[4]) + 1),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[4]) + 2),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[4]) + 3),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[0]) + 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[0]) + 2),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[0]) + 3),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[1])),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[1]) + 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[1]) + 2),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[1]) + 3),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[2])),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[2]) + 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[2]) + 2),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[2]) + 3),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[3])),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[3]) + 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[3]) + 2),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[3]) + 3),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[4])),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[4]) + 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[4]) + 2),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[4]) + 3),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"__sk_buff->hash, offset 0, byte store not permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, hash)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"__sk_buff->tc_index, offset 3, byte store not permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, tc_index) + 3),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"check skb->hash byte load permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
#if __BYTE_ORDER == __LITTLE_ENDIAN
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, hash)),
#else
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, hash) + 3),
#endif
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"check skb->hash byte load not permitted 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, hash) + 1),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"check skb->hash byte load not permitted 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, hash) + 2),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"check skb->hash byte load not permitted 3",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
#if __BYTE_ORDER == __LITTLE_ENDIAN
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, hash) + 3),
#else
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, hash)),
#endif
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"check cb access: byte, wrong type",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_CGROUP_SOCK,
	},
	{
		"check cb access: half",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0]) + 2),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[1])),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[1]) + 2),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[2])),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[2]) + 2),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[3])),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[3]) + 2),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[4])),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[4]) + 2),
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[0]) + 2),
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[1])),
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[1]) + 2),
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[2])),
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[2]) + 2),
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[3])),
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[3]) + 2),
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[4])),
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[4]) + 2),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"check cb access: half, unaligned",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0]) + 1),
			BPF_EXIT_INSN(),
		},
		.errstr = "misaligned context access",
		.result = REJECT,
		.flags = F_LOAD_WITH_STRICT_ALIGNMENT,
	},
	{
		"check __sk_buff->hash, offset 0, half store not permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, hash)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"check __sk_buff->tc_index, offset 2, half store not permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, tc_index) + 2),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"check skb->hash half load permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
#if __BYTE_ORDER == __LITTLE_ENDIAN
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, hash)),
#else
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, hash) + 2),
#endif
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"check skb->hash half load not permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
#if __BYTE_ORDER == __LITTLE_ENDIAN
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, hash) + 2),
#else
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, hash)),
#endif
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"check cb access: half, wrong type",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_H, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_CGROUP_SOCK,
	},
	{
		"check cb access: word",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[1])),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[2])),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[3])),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[4])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[1])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[2])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[3])),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[4])),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"check cb access: word, unaligned 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0]) + 2),
			BPF_EXIT_INSN(),
		},
		.errstr = "misaligned context access",
		.result = REJECT,
		.flags = F_LOAD_WITH_STRICT_ALIGNMENT,
	},
	{
		"check cb access: word, unaligned 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[4]) + 1),
			BPF_EXIT_INSN(),
		},
		.errstr = "misaligned context access",
		.result = REJECT,
		.flags = F_LOAD_WITH_STRICT_ALIGNMENT,
	},
	{
		"check cb access: word, unaligned 3",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[4]) + 2),
			BPF_EXIT_INSN(),
		},
		.errstr = "misaligned context access",
		.result = REJECT,
		.flags = F_LOAD_WITH_STRICT_ALIGNMENT,
	},
	{
		"check cb access: word, unaligned 4",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[4]) + 3),
			BPF_EXIT_INSN(),
		},
		.errstr = "misaligned context access",
		.result = REJECT,
		.flags = F_LOAD_WITH_STRICT_ALIGNMENT,
	},
	{
		"check cb access: double",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[2])),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[2])),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"check cb access: double, unaligned 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[1])),
			BPF_EXIT_INSN(),
		},
		.errstr = "misaligned context access",
		.result = REJECT,
		.flags = F_LOAD_WITH_STRICT_ALIGNMENT,
	},
	{
		"check cb access: double, unaligned 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[3])),
			BPF_EXIT_INSN(),
		},
		.errstr = "misaligned context access",
		.result = REJECT,
		.flags = F_LOAD_WITH_STRICT_ALIGNMENT,
	},
	{
		"check cb access: double, oob 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[4])),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"check cb access: double, oob 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[4])),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"check __sk_buff->ifindex dw store not permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, ifindex)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"check __sk_buff->ifindex dw load not permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, ifindex)),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"check cb access: double, wrong type",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_CGROUP_SOCK,
	},
	{
		"check out of range skb->cb access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[0]) + 256),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access",
		.errstr_unpriv = "",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_ACT,
	},
	{
		"write skb fields from socket prog",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[4])),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_0, 0, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, tc_index)),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_0, 0, 1),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[2])),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.errstr_unpriv = "R1 leaks addr",
		.result_unpriv = REJECT,
	},
	{
		"write skb fields from tc_cls_act prog",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, mark)),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, tc_index)),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, tc_index)),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[3])),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "",
		.result_unpriv = REJECT,
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"PTR_TO_STACK store/load",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -10),
			BPF_ST_MEM(BPF_DW, BPF_REG_1, 2, 0xfaceb00c),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, 2),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 0xfaceb00c,
	},
	{
		"PTR_TO_STACK store/load - bad alignment on off",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_1, 2, 0xfaceb00c),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, 2),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "misaligned stack access off (0x0; 0x0)+-8+2 size 8",
	},
	{
		"PTR_TO_STACK store/load - bad alignment on reg",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -10),
			BPF_ST_MEM(BPF_DW, BPF_REG_1, 8, 0xfaceb00c),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, 8),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "misaligned stack access off (0x0; 0x0)+-10+8 size 8",
	},
	{
		"PTR_TO_STACK store/load - out of bounds low",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -80000),
			BPF_ST_MEM(BPF_DW, BPF_REG_1, 8, 0xfaceb00c),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, 8),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid stack off=-79992 size=8",
	},
	{
		"PTR_TO_STACK store/load - out of bounds high",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_1, 8, 0xfaceb00c),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, 8),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid stack off=0 size=8",
	},
	{
		"unpriv: return pointer",
		.insns = {
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_10),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.result_unpriv = REJECT,
		.errstr_unpriv = "R0 leaks addr",
		.retval = POINTER_VALUE,
	},
	{
		"unpriv: add const to pointer",
		.insns = {
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"unpriv: add pointer to pointer",
		.insns = {
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_10),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R1 pointer += pointer",
	},
	{
		"unpriv: neg pointer",
		.insns = {
			BPF_ALU64_IMM(BPF_NEG, BPF_REG_1, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.result_unpriv = REJECT,
		.errstr_unpriv = "R1 pointer arithmetic",
	},
	{
		"unpriv: cmp pointer with const",
		.insns = {
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.result_unpriv = REJECT,
		.errstr_unpriv = "R1 pointer comparison",
	},
	{
		"unpriv: cmp pointer with pointer",
		.insns = {
			BPF_JMP_REG(BPF_JEQ, BPF_REG_1, BPF_REG_10, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.result_unpriv = REJECT,
		.errstr_unpriv = "R10 pointer comparison",
	},
	{
		"unpriv: check that printk is disallowed",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_2, 8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_trace_printk),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "unknown func bpf_trace_printk#6",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"unpriv: pass pointer to helper function",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_2),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_update_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr_unpriv = "R4 leaks addr",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"unpriv: indirectly pass pointer on stack to helper function",
		.insns = {
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_10, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "invalid indirect read from stack off -8+0 size 8",
		.result = REJECT,
	},
	{
		"unpriv: mangle pointer on stack 1",
		.insns = {
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_10, -8),
			BPF_ST_MEM(BPF_W, BPF_REG_10, -8, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "attempt to corrupt spilled",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"unpriv: mangle pointer on stack 2",
		.insns = {
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_10, -8),
			BPF_ST_MEM(BPF_B, BPF_REG_10, -1, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "attempt to corrupt spilled",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"unpriv: read pointer from stack in small chunks",
		.insns = {
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_10, -8),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_10, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid size",
		.result = REJECT,
	},
	{
		"unpriv: write pointer into ctx",
		.insns = {
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_1, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "R1 leaks addr",
		.result_unpriv = REJECT,
		.errstr = "invalid bpf_context access",
		.result = REJECT,
	},
	{
		"unpriv: spill/fill of ctx",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_6, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"unpriv: spill/fill of ctx 2",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_6, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_hash_recalc),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"unpriv: spill/fill of ctx 3",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_10, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_6, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_hash_recalc),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R1 type=fp expected=ctx",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"unpriv: spill/fill of ctx 4",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1, 0),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_RAW_INSN(BPF_STX | BPF_XADD | BPF_DW, BPF_REG_10,
				     BPF_REG_0, -8, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_6, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_hash_recalc),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R1 type=inv expected=ctx",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"unpriv: spill/fill of different pointers stx",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_3, 42),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 3),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -16),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_2, 0),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_1, 0, 1),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_6, 0),
			BPF_STX_MEM(BPF_W, BPF_REG_1, BPF_REG_3,
				    offsetof(struct __sk_buff, mark)),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "same insn cannot be used with different pointers",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"unpriv: spill/fill of different pointers ldx",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 3),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2,
				      -(__s32)offsetof(struct bpf_perf_event_data,
						       sample_period) - 8),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_2, 0),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_1, 0, 1),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_6, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_1,
				    offsetof(struct bpf_perf_event_data,
					     sample_period)),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "same insn cannot be used with different pointers",
		.prog_type = BPF_PROG_TYPE_PERF_EVENT,
	},
	{
		"unpriv: write pointer into map elem value",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			BPF_STX_MEM(BPF_DW, BPF_REG_0, BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr_unpriv = "R0 leaks addr",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"alu32: mov u32 const",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_7, 0),
			BPF_ALU32_IMM(BPF_AND, BPF_REG_7, 1),
			BPF_MOV32_REG(BPF_REG_0, BPF_REG_7),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_7, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 0,
	},
	{
		"unpriv: partial copy of pointer",
		.insns = {
			BPF_MOV32_REG(BPF_REG_1, BPF_REG_10),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "R10 partial copy",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"unpriv: pass pointer to tail_call",
		.insns = {
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_1),
			BPF_LD_MAP_FD(BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_tail_call),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_prog1 = { 1 },
		.errstr_unpriv = "R3 leaks addr into helper",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"unpriv: cmp map pointer with zero",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 1 },
		.errstr_unpriv = "R1 pointer comparison",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"unpriv: write into frame pointer",
		.insns = {
			BPF_MOV64_REG(BPF_REG_10, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "frame pointer is read only",
		.result = REJECT,
	},
	{
		"unpriv: spill/fill frame pointer",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_10, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_10, BPF_REG_6, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "frame pointer is read only",
		.result = REJECT,
	},
	{
		"unpriv: cmp of frame pointer",
		.insns = {
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_10, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "R10 pointer comparison",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"unpriv: adding of fp",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_1, 0),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_10),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0, -8),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"unpriv: cmp of stack pointer",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_2, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "R2 pointer comparison",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"runtime/jit: tail_call within bounds, prog once",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_LD_MAP_FD(BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_tail_call),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.fixup_prog1 = { 1 },
		.result = ACCEPT,
		.retval = 42,
	},
	{
		"runtime/jit: tail_call within bounds, prog loop",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_3, 1),
			BPF_LD_MAP_FD(BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_tail_call),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.fixup_prog1 = { 1 },
		.result = ACCEPT,
		.retval = 41,
	},
	{
		"runtime/jit: tail_call within bounds, no prog",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_3, 2),
			BPF_LD_MAP_FD(BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_tail_call),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.fixup_prog1 = { 1 },
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"runtime/jit: tail_call out of bounds",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_3, 256),
			BPF_LD_MAP_FD(BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_tail_call),
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.fixup_prog1 = { 1 },
		.result = ACCEPT,
		.retval = 2,
	},
	{
		"runtime/jit: pass negative index to tail_call",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_3, -1),
			BPF_LD_MAP_FD(BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_tail_call),
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.fixup_prog1 = { 1 },
		.result = ACCEPT,
		.retval = 2,
	},
	{
		"runtime/jit: pass > 32bit index to tail_call",
		.insns = {
			BPF_LD_IMM64(BPF_REG_3, 0x100000000ULL),
			BPF_LD_MAP_FD(BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_tail_call),
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.fixup_prog1 = { 2 },
		.result = ACCEPT,
		.retval = 42,
	},
	{
		"stack pointer arithmetic",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 4),
			BPF_JMP_IMM(BPF_JA, 0, 0, 0),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_7, -10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_7, -10),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_7),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_2, BPF_REG_1),
			BPF_ST_MEM(0, BPF_REG_2, 4, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_7),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, 8),
			BPF_ST_MEM(0, BPF_REG_2, 4, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"raw_stack: no skb_load_bytes",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 8),
			/* Call to skb_load_bytes() omitted. */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid read from stack off -8+0 size 8",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, negative len",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, -8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R4 min value is negative",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, negative len 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, ~0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R4 min value is negative",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, zero len",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid stack type R3",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, no init",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, init",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_6, 0, 0xcafe),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, spilled regs around bounds",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -16),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1,  8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_6,  8),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_0,
				    offsetof(struct __sk_buff, mark)),
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_2,
				    offsetof(struct __sk_buff, priority)),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_2),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, spilled regs corruption",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1, 0),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_0,
				    offsetof(struct __sk_buff, mark)),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R0 invalid mem access 'inv'",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, spilled regs corruption 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -16),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1,  0),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1,  8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_6,  8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_6,  0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_0,
				    offsetof(struct __sk_buff, mark)),
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_2,
				    offsetof(struct __sk_buff, priority)),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_2),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_3,
				    offsetof(struct __sk_buff, pkt_type)),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_3),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R3 invalid mem access 'inv'",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, spilled regs + data",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -16),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1,  0),
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1,  8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_6,  8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_6,  0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_0,
				    offsetof(struct __sk_buff, mark)),
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_2,
				    offsetof(struct __sk_buff, priority)),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_3),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, invalid access 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -513),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid stack type R3 off=-513 access_size=8",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, invalid access 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -1),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid stack type R3 off=-1 access_size=8",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, invalid access 3",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 0xffffffff),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 0xffffffff),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R4 min value is negative",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, invalid access 4",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -1),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 0x7fffffff),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R4 unbounded memory access, use 'var &= const' or 'if (var < const)'",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, invalid access 5",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -512),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 0x7fffffff),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R4 unbounded memory access, use 'var &= const' or 'if (var < const)'",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, invalid access 6",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -512),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid stack type R3 off=-512 access_size=0",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"raw_stack: skb_load_bytes, large access",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, -512),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_4, 512),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"context stores via ST",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_ST_MEM(BPF_DW, BPF_REG_1, offsetof(struct __sk_buff, mark), 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "BPF_ST stores into R1 context is not allowed",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"context stores via XADD",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_RAW_INSN(BPF_STX | BPF_XADD | BPF_W, BPF_REG_1,
				     BPF_REG_0, offsetof(struct __sk_buff, mark), 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "BPF_XADD stores into R1 context is not allowed",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_4, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_3),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_5, 14),
			BPF_JMP_REG(BPF_JGT, BPF_REG_5, BPF_REG_4, 15),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_3, 7),
			BPF_LDX_MEM(BPF_B, BPF_REG_4, BPF_REG_3, 12),
			BPF_ALU64_IMM(BPF_MUL, BPF_REG_4, 14),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_3, BPF_REG_4),
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, len)),
			BPF_ALU64_IMM(BPF_LSH, BPF_REG_2, 49),
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_2, 49),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_3, BPF_REG_2),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_3),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, 8),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_JMP_REG(BPF_JGT, BPF_REG_2, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_1, BPF_REG_3, 4),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test3",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid bpf_context access off=76",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SOCKET_FILTER,
	},
	{
		"direct packet access: test4 (write)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_STX_MEM(BPF_B, BPF_REG_2, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test5 (pkt_end >= reg, good access)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_3, BPF_REG_0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test6 (pkt_end >= reg, bad access)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_3, BPF_REG_0, 3),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid access to packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test7 (pkt_end >= reg, both accesses)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_3, BPF_REG_0, 3),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid access to packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test8 (double test, variant 1)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_3, BPF_REG_0, 4),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test9 (double test, variant 2)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_3, BPF_REG_0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test10 (write invalid)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_STX_MEM(BPF_B, BPF_REG_2, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid access to packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test11 (shift, good access)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 22),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 8),
			BPF_MOV64_IMM(BPF_REG_3, 144),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_3),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_5, 23),
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_5, 3),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_6, BPF_REG_5),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.retval = 1,
	},
	{
		"direct packet access: test12 (and, good access)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 22),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 8),
			BPF_MOV64_IMM(BPF_REG_3, 144),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_3),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_5, 23),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_5, 15),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_6, BPF_REG_5),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.retval = 1,
	},
	{
		"direct packet access: test13 (branches, good access)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 22),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 13),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_MOV64_IMM(BPF_REG_4, 1),
			BPF_JMP_REG(BPF_JGT, BPF_REG_3, BPF_REG_4, 2),
			BPF_MOV64_IMM(BPF_REG_3, 14),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_MOV64_IMM(BPF_REG_3, 24),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_3),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_5, 23),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_5, 15),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_6, BPF_REG_5),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.retval = 1,
	},
	{
		"direct packet access: test14 (pkt_ptr += 0, CONST_IMM, good access)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 22),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 7),
			BPF_MOV64_IMM(BPF_REG_5, 12),
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_5, 4),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_6, BPF_REG_5),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_6, 0),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.retval = 1,
	},
	{
		"direct packet access: test15 (spill with xadd)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 8),
			BPF_MOV64_IMM(BPF_REG_5, 4096),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_STX_XADD(BPF_DW, BPF_REG_4, BPF_REG_5, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_4, 0),
			BPF_STX_MEM(BPF_W, BPF_REG_2, BPF_REG_5, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R2 invalid mem access 'inv'",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test16 (arith on data_end)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_3, 16),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_STX_MEM(BPF_B, BPF_REG_2, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R3 pointer arithmetic on PTR_TO_PACKET_END",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test17 (pruning, alignment)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 14),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_7, 1, 4),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_STX_MEM(BPF_W, BPF_REG_0, BPF_REG_0, -4),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 1),
			BPF_JMP_A(-6),
		},
		.errstr = "misaligned packet access off 2+(0x0; 0x0)+15+-4 size 4",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.flags = F_LOAD_WITH_STRICT_ALIGNMENT,
	},
	{
		"direct packet access: test18 (imm += pkt_ptr, 1)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_IMM(BPF_REG_0, 8),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_2),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_STX_MEM(BPF_B, BPF_REG_2, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test19 (imm += pkt_ptr, 2)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 3),
			BPF_MOV64_IMM(BPF_REG_4, 4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_4, BPF_REG_2),
			BPF_STX_MEM(BPF_B, BPF_REG_4, BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test20 (x += pkt_ptr, 1)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_IMM(BPF_REG_0, 0xffffffff),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_10, -8),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_0, 0x7fff),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_0),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_4, BPF_REG_2),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_4),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 0x7fff - 1),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_3, 1),
			BPF_STX_MEM(BPF_DW, BPF_REG_5, BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
	},
	{
		"direct packet access: test21 (x += pkt_ptr, 2)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 9),
			BPF_MOV64_IMM(BPF_REG_4, 0xffffffff),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_4, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_4, BPF_REG_10, -8),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_4, 0x7fff),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_4, BPF_REG_2),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_4),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 0x7fff - 1),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_3, 1),
			BPF_STX_MEM(BPF_DW, BPF_REG_5, BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
	},
	{
		"direct packet access: test22 (x += pkt_ptr, 3)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_2, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_3, -16),
			BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_10, -16),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 11),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_10, -8),
			BPF_MOV64_IMM(BPF_REG_4, 0xffffffff),
			BPF_STX_XADD(BPF_DW, BPF_REG_10, BPF_REG_4, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_4, BPF_REG_10, -8),
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_4, 49),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_4, BPF_REG_2),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_4),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 2),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 2),
			BPF_MOV64_IMM(BPF_REG_2, 1),
			BPF_STX_MEM(BPF_H, BPF_REG_4, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
	},
	{
		"direct packet access: test23 (x += pkt_ptr, 4)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_IMM(BPF_REG_0, 0xffffffff),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_10, -8),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_0, 0xffff),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_0, 31),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_2),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 0xffff - 1),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_STX_MEM(BPF_DW, BPF_REG_5, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = REJECT,
		.errstr = "invalid access to packet, off=0 size=8, R5(id=1,off=0,r=0)",
	},
	{
		"direct packet access: test24 (x += pkt_ptr, 5)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_IMM(BPF_REG_0, 0xffffffff),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_10, -8),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_0, 0xff),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_0, 64),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_2),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 0x7fff - 1),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_STX_MEM(BPF_DW, BPF_REG_5, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
	},
	{
		"direct packet access: test25 (marking on <, good access)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_0, BPF_REG_3, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, -4),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test26 (marking on <, bad access)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_0, BPF_REG_3, 3),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JA, 0, 0, -3),
		},
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"direct packet access: test27 (marking on <=, good access)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_3, BPF_REG_0, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.retval = 1,
	},
	{
		"direct packet access: test28 (marking on <=, bad access)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_3, BPF_REG_0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, -4),
		},
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test1, valid packet_ptr range",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_3, 5),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_2),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_update_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 5 },
		.result_unpriv = ACCEPT,
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"helper access to packet: test2, unchecked packet_ptr",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 1 },
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"helper access to packet: test3, variable add",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
					offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
					offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_3, 10),
			BPF_LDX_MEM(BPF_B, BPF_REG_5, BPF_REG_2, 0),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_4, BPF_REG_5),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_4),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_5, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_5, BPF_REG_3, 4),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_4),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 11 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"helper access to packet: test4, packet_ptr with bad range",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 4),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_3, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 7 },
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"helper access to packet: test5, packet_ptr with too short range",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, 1),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 7),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_3, 3),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 6 },
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"helper access to packet: test6, cls valid packet_ptr range",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_3, 5),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_2),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_update_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 5 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test7, cls unchecked packet_ptr",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 1 },
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test8, cls variable add",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
					offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
					offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_3, 10),
			BPF_LDX_MEM(BPF_B, BPF_REG_5, BPF_REG_2, 0),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_4, BPF_REG_5),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_4),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_5, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_5, BPF_REG_3, 4),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_4),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 11 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test9, cls packet_ptr with bad range",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 4),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_3, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 7 },
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test10, cls packet_ptr with too short range",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, 1),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 7),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_3, 3),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 6 },
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test11, cls unsuitable helper 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 1),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_3, 7),
			BPF_JMP_REG(BPF_JGT, BPF_REG_3, BPF_REG_7, 4),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_4, 42),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_store_bytes),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "helper access to the packet",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test12, cls unsuitable helper 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_6, BPF_REG_7, 3),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_4, 4),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "helper access to the packet",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test13, cls helper ok",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 1),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 7),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_7, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_csum_diff),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test14, cls helper ok sub",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 1),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 7),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_7, 6),
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_1, 4),
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_csum_diff),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test15, cls helper fail sub",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 1),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 7),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_7, 6),
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_1, 12),
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_csum_diff),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test16, cls helper fail range 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 1),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 7),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_7, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_2, 8),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_csum_diff),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test17, cls helper fail range 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 1),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 7),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_7, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_2, -9),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_csum_diff),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R2 min value is negative",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test18, cls helper fail range 3",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 1),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 7),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_7, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_2, ~0),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_csum_diff),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R2 min value is negative",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test19, cls helper range zero",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 1),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 7),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_7, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_csum_diff),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test20, pkt end as input",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 1),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 7),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_7, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_7),
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_csum_diff),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R1 type=pkt_end expected=fp",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to packet: test21, wrong reg",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 1),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 7),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_7, 6),
			BPF_MOV64_IMM(BPF_REG_2, 4),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_csum_diff),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"valid map access into an array with a constant",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0,
				   offsetof(struct test_val, foo)),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R0 leaks addr",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"valid map access into an array with a register",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_IMM(BPF_REG_1, 4),
			BPF_ALU64_IMM(BPF_LSH, BPF_REG_1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0,
				   offsetof(struct test_val, foo)),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R0 leaks addr",
		.result_unpriv = REJECT,
		.result = ACCEPT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"valid map access into an array with a variable",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_1, MAX_ENTRIES, 3),
			BPF_ALU64_IMM(BPF_LSH, BPF_REG_1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0,
				   offsetof(struct test_val, foo)),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R0 leaks addr",
		.result_unpriv = REJECT,
		.result = ACCEPT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"valid map access into an array with a signed variable",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 9),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_1, 0xffffffff, 1),
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_MOV32_IMM(BPF_REG_2, MAX_ENTRIES),
			BPF_JMP_REG(BPF_JSGT, BPF_REG_2, BPF_REG_1, 1),
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_ALU32_IMM(BPF_LSH, BPF_REG_1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0,
				   offsetof(struct test_val, foo)),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R0 leaks addr",
		.result_unpriv = REJECT,
		.result = ACCEPT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"invalid map access into an array with a constant",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, (MAX_ENTRIES + 1) << 2,
				   offsetof(struct test_val, foo)),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "invalid access to map value, value_size=48 off=48 size=8",
		.result = REJECT,
	},
	{
		"invalid map access into an array with a register",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_IMM(BPF_REG_1, MAX_ENTRIES + 1),
			BPF_ALU64_IMM(BPF_LSH, BPF_REG_1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0,
				   offsetof(struct test_val, foo)),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R0 min value is outside of the array range",
		.result = REJECT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"invalid map access into an array with a variable",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, 0),
			BPF_ALU64_IMM(BPF_LSH, BPF_REG_1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0,
				   offsetof(struct test_val, foo)),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R0 unbounded memory access, make sure to bounds check any array access into a map",
		.result = REJECT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"invalid map access into an array with no floor check",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0, 0),
			BPF_MOV32_IMM(BPF_REG_2, MAX_ENTRIES),
			BPF_JMP_REG(BPF_JSGT, BPF_REG_2, BPF_REG_1, 1),
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_ALU32_IMM(BPF_LSH, BPF_REG_1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0,
				   offsetof(struct test_val, foo)),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R0 leaks addr",
		.errstr = "R0 unbounded memory access",
		.result_unpriv = REJECT,
		.result = REJECT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"invalid map access into an array with a invalid max check",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, 0),
			BPF_MOV32_IMM(BPF_REG_2, MAX_ENTRIES + 1),
			BPF_JMP_REG(BPF_JGT, BPF_REG_2, BPF_REG_1, 1),
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_ALU32_IMM(BPF_LSH, BPF_REG_1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0,
				   offsetof(struct test_val, foo)),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R0 leaks addr",
		.errstr = "invalid access to map value, value_size=48 off=44 size=8",
		.result_unpriv = REJECT,
		.result = REJECT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"invalid map access into an array with a invalid max check",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 10),
			BPF_MOV64_REG(BPF_REG_8, BPF_REG_0),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_8),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_0,
				    offsetof(struct test_val, foo)),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3, 11 },
		.errstr = "R0 pointer += pointer",
		.result = REJECT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"valid cgroup storage access",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_local_storage),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, 0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_1),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.fixup_cgroup_storage = { 1 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_CGROUP_SKB,
	},
	{
		"invalid cgroup storage access 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_local_storage),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, 0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_1),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 1 },
		.result = REJECT,
		.errstr = "cannot pass map_type 1 into func bpf_get_local_storage",
		.prog_type = BPF_PROG_TYPE_CGROUP_SKB,
	},
	{
		"invalid cgroup storage access 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_local_storage),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "fd 1 is not pointing to valid bpf_map",
		.prog_type = BPF_PROG_TYPE_CGROUP_SKB,
	},
	{
		"invalid per-cgroup storage access 3",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_local_storage),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, 256),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 1),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_cgroup_storage = { 1 },
		.result = REJECT,
		.errstr = "invalid access to map value, value_size=64 off=256 size=4",
		.prog_type = BPF_PROG_TYPE_CGROUP_SKB,
	},
	{
		"invalid cgroup storage access 4",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_local_storage),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, -2),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_1),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 1),
			BPF_EXIT_INSN(),
		},
		.fixup_cgroup_storage = { 1 },
		.result = REJECT,
		.errstr = "invalid access to map value, value_size=64 off=-2 size=4",
		.prog_type = BPF_PROG_TYPE_CGROUP_SKB,
	},
	{
		"invalid cgroup storage access 5",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 7),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_local_storage),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, 0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_1),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.fixup_cgroup_storage = { 1 },
		.result = REJECT,
		.errstr = "get_local_storage() doesn't support non-zero flags",
		.prog_type = BPF_PROG_TYPE_CGROUP_SKB,
	},
	{
		"invalid cgroup storage access 6",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_1),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_local_storage),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, 0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_1),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.fixup_cgroup_storage = { 1 },
		.result = REJECT,
		.errstr = "get_local_storage() doesn't support non-zero flags",
		.prog_type = BPF_PROG_TYPE_CGROUP_SKB,
	},
	{
		"multiple registers share map_lookup_elem result",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 10),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_0),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_4, 0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 4 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS
	},
	{
		"alu ops on ptr_to_map_value_or_null, 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 10),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, -2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_4, 0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 4 },
		.errstr = "R4 pointer arithmetic on PTR_TO_MAP_VALUE_OR_NULL",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS
	},
	{
		"alu ops on ptr_to_map_value_or_null, 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 10),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_0),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_4, -1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_4, 0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 4 },
		.errstr = "R4 pointer arithmetic on PTR_TO_MAP_VALUE_OR_NULL",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS
	},
	{
		"alu ops on ptr_to_map_value_or_null, 3",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 10),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_0),
			BPF_ALU64_IMM(BPF_LSH, BPF_REG_4, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_4, 0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 4 },
		.errstr = "R4 pointer arithmetic on PTR_TO_MAP_VALUE_OR_NULL",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS
	},
	{
		"invalid memory access with multiple map_lookup_elem calls",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 10),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_MOV64_REG(BPF_REG_8, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_2),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_7),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_4, 0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 4 },
		.result = REJECT,
		.errstr = "R4 !read_ok",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS
	},
	{
		"valid indirect map_lookup_elem access with 2nd lookup in branch",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 10),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_MOV64_REG(BPF_REG_8, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_2),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_2, 10),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_2, 0, 3),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_7),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_0),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			BPF_ST_MEM(BPF_DW, BPF_REG_4, 0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 4 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS
	},
	{
		"invalid map access from else condition",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_1, MAX_ENTRIES-1, 1),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 1),
			BPF_ALU64_IMM(BPF_LSH, BPF_REG_1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, offsetof(struct test_val, foo)),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R0 unbounded memory access",
		.result = REJECT,
		.errstr_unpriv = "R0 leaks addr",
		.result_unpriv = REJECT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"constant register |= constant should keep constant type",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -48),
			BPF_MOV64_IMM(BPF_REG_2, 34),
			BPF_ALU64_IMM(BPF_OR, BPF_REG_2, 13),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"constant register |= constant should not bypass stack boundary checks",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -48),
			BPF_MOV64_IMM(BPF_REG_2, 34),
			BPF_ALU64_IMM(BPF_OR, BPF_REG_2, 24),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid stack type R1 off=-48 access_size=58",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"constant register |= constant register should keep constant type",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -48),
			BPF_MOV64_IMM(BPF_REG_2, 34),
			BPF_MOV64_IMM(BPF_REG_4, 13),
			BPF_ALU64_REG(BPF_OR, BPF_REG_2, BPF_REG_4),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"constant register |= constant register should not bypass stack boundary checks",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -48),
			BPF_MOV64_IMM(BPF_REG_2, 34),
			BPF_MOV64_IMM(BPF_REG_4, 24),
			BPF_ALU64_REG(BPF_OR, BPF_REG_2, BPF_REG_4),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid stack type R1 off=-48 access_size=58",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"invalid direct packet write for LWT_IN",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_STX_MEM(BPF_B, BPF_REG_2, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "cannot write into packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_LWT_IN,
	},
	{
		"invalid direct packet write for LWT_OUT",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_STX_MEM(BPF_B, BPF_REG_2, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "cannot write into packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_LWT_OUT,
	},
	{
		"direct packet write for LWT_XMIT",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_STX_MEM(BPF_B, BPF_REG_2, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_LWT_XMIT,
	},
	{
		"direct packet read for LWT_IN",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_LWT_IN,
	},
	{
		"direct packet read for LWT_OUT",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_LWT_OUT,
	},
	{
		"direct packet read for LWT_XMIT",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_LWT_XMIT,
	},
	{
		"overlapping checks for direct packet access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 6),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_2, 6),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_LWT_XMIT,
	},
	{
		"make headroom for LWT_XMIT",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_2, 34),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_skb_change_head),
			/* split for s390 to succeed */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_2, 42),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_skb_change_head),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_LWT_XMIT,
	},
	{
		"invalid access of tc_classid for LWT_IN",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, tc_classid)),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid bpf_context access",
	},
	{
		"invalid access of tc_classid for LWT_OUT",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, tc_classid)),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid bpf_context access",
	},
	{
		"invalid access of tc_classid for LWT_XMIT",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, tc_classid)),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid bpf_context access",
	},
	{
		"leak pointer into ctx 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_LD_MAP_FD(BPF_REG_2, 0),
			BPF_STX_XADD(BPF_DW, BPF_REG_1, BPF_REG_2,
				      offsetof(struct __sk_buff, cb[0])),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 2 },
		.errstr_unpriv = "R2 leaks addr into mem",
		.result_unpriv = REJECT,
		.result = REJECT,
		.errstr = "BPF_XADD stores into R1 context is not allowed",
	},
	{
		"leak pointer into ctx 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0,
				    offsetof(struct __sk_buff, cb[0])),
			BPF_STX_XADD(BPF_DW, BPF_REG_1, BPF_REG_10,
				      offsetof(struct __sk_buff, cb[0])),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "R10 leaks addr into mem",
		.result_unpriv = REJECT,
		.result = REJECT,
		.errstr = "BPF_XADD stores into R1 context is not allowed",
	},
	{
		"leak pointer into ctx 3",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_LD_MAP_FD(BPF_REG_2, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_2,
				      offsetof(struct __sk_buff, cb[0])),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 1 },
		.errstr_unpriv = "R2 leaks addr into ctx",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"leak pointer into map val",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 3),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_0, BPF_REG_3, 0),
			BPF_STX_XADD(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 4 },
		.errstr_unpriv = "R6 leaks addr into mem",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
	{
		"helper access to map: full range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_2, sizeof(struct test_val)),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: partial range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_2, 8),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: empty range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 3),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_EMIT_CALL(BPF_FUNC_trace_printk),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "invalid access to map value, value_size=48 off=0 size=0",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: out-of-bound range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_2, sizeof(struct test_val) + 8),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "invalid access to map value, value_size=48 off=0 size=56",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: negative range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_2, -8),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R2 min value is negative",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via const imm): full range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1,
				offsetof(struct test_val, foo)),
			BPF_MOV64_IMM(BPF_REG_2,
				sizeof(struct test_val) -
				offsetof(struct test_val, foo)),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via const imm): partial range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1,
				offsetof(struct test_val, foo)),
			BPF_MOV64_IMM(BPF_REG_2, 8),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via const imm): empty range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1,
				offsetof(struct test_val, foo)),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_EMIT_CALL(BPF_FUNC_trace_printk),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "invalid access to map value, value_size=48 off=4 size=0",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via const imm): out-of-bound range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1,
				offsetof(struct test_val, foo)),
			BPF_MOV64_IMM(BPF_REG_2,
				sizeof(struct test_val) -
				offsetof(struct test_val, foo) + 8),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "invalid access to map value, value_size=48 off=4 size=52",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via const imm): negative range (> adjustment)",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1,
				offsetof(struct test_val, foo)),
			BPF_MOV64_IMM(BPF_REG_2, -8),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R2 min value is negative",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via const imm): negative range (< adjustment)",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1,
				offsetof(struct test_val, foo)),
			BPF_MOV64_IMM(BPF_REG_2, -1),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R2 min value is negative",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via const reg): full range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_3,
				offsetof(struct test_val, foo)),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_MOV64_IMM(BPF_REG_2,
				sizeof(struct test_val) -
				offsetof(struct test_val, foo)),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via const reg): partial range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_3,
				offsetof(struct test_val, foo)),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_MOV64_IMM(BPF_REG_2, 8),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via const reg): empty range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_EMIT_CALL(BPF_FUNC_trace_printk),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R1 min value is outside of the array range",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via const reg): out-of-bound range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_3,
				offsetof(struct test_val, foo)),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_MOV64_IMM(BPF_REG_2,
				sizeof(struct test_val) -
				offsetof(struct test_val, foo) + 8),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "invalid access to map value, value_size=48 off=4 size=52",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via const reg): negative range (> adjustment)",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_3,
				offsetof(struct test_val, foo)),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_MOV64_IMM(BPF_REG_2, -8),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R2 min value is negative",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via const reg): negative range (< adjustment)",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_3,
				offsetof(struct test_val, foo)),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_MOV64_IMM(BPF_REG_2, -1),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R2 min value is negative",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via variable): full range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_3,
				offsetof(struct test_val, foo), 4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_MOV64_IMM(BPF_REG_2,
				sizeof(struct test_val) -
				offsetof(struct test_val, foo)),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via variable): partial range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_3,
				offsetof(struct test_val, foo), 4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_MOV64_IMM(BPF_REG_2, 8),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via variable): empty range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_3,
				offsetof(struct test_val, foo), 3),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_EMIT_CALL(BPF_FUNC_trace_printk),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R1 min value is outside of the array range",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via variable): no max check",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_MOV64_IMM(BPF_REG_2, 1),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R1 unbounded memory access",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to adjusted map (via variable): wrong max check",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_3,
				offsetof(struct test_val, foo), 4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_MOV64_IMM(BPF_REG_2,
				sizeof(struct test_val) -
				offsetof(struct test_val, foo) + 1),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "invalid access to map value, value_size=48 off=4 size=45",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: bounds check using <, good access",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JLT, BPF_REG_3, 32, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_ST_MEM(BPF_B, BPF_REG_1, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: bounds check using <, bad access",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JLT, BPF_REG_3, 32, 4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_ST_MEM(BPF_B, BPF_REG_1, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = REJECT,
		.errstr = "R1 unbounded memory access",
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: bounds check using <=, good access",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JLE, BPF_REG_3, 32, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_ST_MEM(BPF_B, BPF_REG_1, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: bounds check using <=, bad access",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JLE, BPF_REG_3, 32, 4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_ST_MEM(BPF_B, BPF_REG_1, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = REJECT,
		.errstr = "R1 unbounded memory access",
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: bounds check using s<, good access",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JSLT, BPF_REG_3, 32, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JSLT, BPF_REG_3, 0, -3),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_ST_MEM(BPF_B, BPF_REG_1, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: bounds check using s<, good access 2",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JSLT, BPF_REG_3, 32, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JSLT, BPF_REG_3, -3, -3),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_ST_MEM(BPF_B, BPF_REG_1, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: bounds check using s<, bad access",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JSLT, BPF_REG_3, 32, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JSLT, BPF_REG_3, -3, -3),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_ST_MEM(BPF_B, BPF_REG_1, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = REJECT,
		.errstr = "R1 min value is negative",
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: bounds check using s<=, good access",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JSLE, BPF_REG_3, 32, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JSLE, BPF_REG_3, 0, -3),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_ST_MEM(BPF_B, BPF_REG_1, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: bounds check using s<=, good access 2",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JSLE, BPF_REG_3, 32, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JSLE, BPF_REG_3, -3, -3),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_ST_MEM(BPF_B, BPF_REG_1, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to map: bounds check using s<=, bad access",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JSLE, BPF_REG_3, 32, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JSLE, BPF_REG_3, -3, -3),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_ST_MEM(BPF_B, BPF_REG_1, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = REJECT,
		.errstr = "R1 min value is negative",
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"map lookup helper access to map",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map3 = { 3, 8 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"map update helper access to map",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_update_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map3 = { 3, 10 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"map update helper access to map: wrong size",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_update_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.fixup_map3 = { 10 },
		.result = REJECT,
		.errstr = "invalid access to map value, value_size=8 off=0 size=16",
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"map helper access to adjusted map (via const imm)",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2,
				      offsetof(struct other_val, bar)),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map3 = { 3, 9 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"map helper access to adjusted map (via const imm): out-of-bound 1",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2,
				      sizeof(struct other_val) - 4),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map3 = { 3, 9 },
		.result = REJECT,
		.errstr = "invalid access to map value, value_size=16 off=12 size=8",
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"map helper access to adjusted map (via const imm): out-of-bound 2",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -4),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map3 = { 3, 9 },
		.result = REJECT,
		.errstr = "invalid access to map value, value_size=16 off=-4 size=8",
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"map helper access to adjusted map (via const reg)",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_3,
				      offsetof(struct other_val, bar)),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_2, BPF_REG_3),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map3 = { 3, 10 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"map helper access to adjusted map (via const reg): out-of-bound 1",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_3,
				      sizeof(struct other_val) - 4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_2, BPF_REG_3),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map3 = { 3, 10 },
		.result = REJECT,
		.errstr = "invalid access to map value, value_size=16 off=12 size=8",
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"map helper access to adjusted map (via const reg): out-of-bound 2",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_3, -4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_2, BPF_REG_3),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map3 = { 3, 10 },
		.result = REJECT,
		.errstr = "invalid access to map value, value_size=16 off=-4 size=8",
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"map helper access to adjusted map (via variable)",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_3,
				    offsetof(struct other_val, bar), 4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_2, BPF_REG_3),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map3 = { 3, 11 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"map helper access to adjusted map (via variable): no max check",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_2, BPF_REG_3),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map3 = { 3, 10 },
		.result = REJECT,
		.errstr = "R2 unbounded memory access, make sure to bounds check any array access into a map",
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"map helper access to adjusted map (via variable): wrong max check",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_3,
				    offsetof(struct other_val, bar) + 1, 4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_2, BPF_REG_3),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map3 = { 3, 11 },
		.result = REJECT,
		.errstr = "invalid access to map value, value_size=16 off=9 size=8",
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"map element value is preserved across register spilling",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 42),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -184),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_1, 0),
			BPF_ST_MEM(BPF_DW, BPF_REG_3, 0, 42),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R0 leaks addr",
		.result = ACCEPT,
		.result_unpriv = REJECT,
	},
	{
		"map element value or null is marked on register spilling",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -152),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
			BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_1, 0),
			BPF_ST_MEM(BPF_DW, BPF_REG_3, 0, 42),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R0 leaks addr",
		.result = ACCEPT,
		.result_unpriv = REJECT,
	},
	{
		"map element value store of cleared call register",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			BPF_STX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R1 !read_ok",
		.errstr = "R1 !read_ok",
		.result = REJECT,
		.result_unpriv = REJECT,
	},
	{
		"map element value with unaligned store",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 17),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 3),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 42),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 2, 43),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, -2, 44),
			BPF_MOV64_REG(BPF_REG_8, BPF_REG_0),
			BPF_ST_MEM(BPF_DW, BPF_REG_8, 0, 32),
			BPF_ST_MEM(BPF_DW, BPF_REG_8, 2, 33),
			BPF_ST_MEM(BPF_DW, BPF_REG_8, -2, 34),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_8, 5),
			BPF_ST_MEM(BPF_DW, BPF_REG_8, 0, 22),
			BPF_ST_MEM(BPF_DW, BPF_REG_8, 4, 23),
			BPF_ST_MEM(BPF_DW, BPF_REG_8, -7, 24),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_8),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_7, 3),
			BPF_ST_MEM(BPF_DW, BPF_REG_7, 0, 22),
			BPF_ST_MEM(BPF_DW, BPF_REG_7, 4, 23),
			BPF_ST_MEM(BPF_DW, BPF_REG_7, -4, 24),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R0 leaks addr",
		.result = ACCEPT,
		.result_unpriv = REJECT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"map element value with unaligned load",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 11),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_1, MAX_ENTRIES, 9),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 3),
			BPF_LDX_MEM(BPF_DW, BPF_REG_7, BPF_REG_0, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_7, BPF_REG_0, 2),
			BPF_MOV64_REG(BPF_REG_8, BPF_REG_0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_7, BPF_REG_8, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_7, BPF_REG_8, 2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 5),
			BPF_LDX_MEM(BPF_DW, BPF_REG_7, BPF_REG_0, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_7, BPF_REG_0, 4),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R0 leaks addr",
		.result = ACCEPT,
		.result_unpriv = REJECT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"map element value illegal alu op, 1",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_0, 8),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 22),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R0 bitwise operator &= on pointer",
		.result = REJECT,
	},
	{
		"map element value illegal alu op, 2",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
			BPF_ALU32_IMM(BPF_ADD, BPF_REG_0, 0),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 22),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R0 32-bit pointer arithmetic prohibited",
		.result = REJECT,
	},
	{
		"map element value illegal alu op, 3",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
			BPF_ALU64_IMM(BPF_DIV, BPF_REG_0, 42),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 22),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R0 pointer arithmetic with /= operator",
		.result = REJECT,
	},
	{
		"map element value illegal alu op, 4",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
			BPF_ENDIAN(BPF_FROM_BE, BPF_REG_0, 64),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 22),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R0 pointer arithmetic prohibited",
		.errstr = "invalid mem access 'inv'",
		.result = REJECT,
		.result_unpriv = REJECT,
	},
	{
		"map element value illegal alu op, 5",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_MOV64_IMM(BPF_REG_3, 4096),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_0, 0),
			BPF_STX_XADD(BPF_DW, BPF_REG_2, BPF_REG_3, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_2, 0),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 22),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R0 invalid mem access 'inv'",
		.result = REJECT,
	},
	{
		"map element value is preserved across register spilling",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0,
				offsetof(struct test_val, foo)),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 42),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -184),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_1, 0),
			BPF_ST_MEM(BPF_DW, BPF_REG_3, 0, 42),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R0 leaks addr",
		.result = ACCEPT,
		.result_unpriv = REJECT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"helper access to variable memory: stack, bitwise AND + JMP, correct bounds",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -64),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -64),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -56),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -48),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -40),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -32),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -24),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -16),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_MOV64_IMM(BPF_REG_2, 16),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1, -128),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_2, 64),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_JMP_REG(BPF_JGE, BPF_REG_4, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: stack, bitwise AND, zero included",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -64),
			BPF_MOV64_IMM(BPF_REG_2, 16),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1, -128),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_2, 64),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid indirect read from stack off -64+0 size 64",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: stack, bitwise AND + JMP, wrong max",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -64),
			BPF_MOV64_IMM(BPF_REG_2, 16),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1, -128),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_2, 65),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_JMP_REG(BPF_JGE, BPF_REG_4, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid stack type R1 off=-64 access_size=65",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: stack, JMP, correct bounds",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -64),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -64),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -56),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -48),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -40),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -32),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -24),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -16),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_MOV64_IMM(BPF_REG_2, 16),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1, -128),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_2, 64, 4),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_JMP_REG(BPF_JGE, BPF_REG_4, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: stack, JMP (signed), correct bounds",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -64),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -64),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -56),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -48),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -40),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -32),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -24),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -16),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_MOV64_IMM(BPF_REG_2, 16),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1, -128),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_2, 64, 4),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_JMP_REG(BPF_JSGE, BPF_REG_4, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: stack, JMP, bounds + offset",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -64),
			BPF_MOV64_IMM(BPF_REG_2, 16),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1, -128),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_2, 64, 5),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_JMP_REG(BPF_JGE, BPF_REG_4, BPF_REG_2, 3),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, 1),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid stack type R1 off=-64 access_size=65",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: stack, JMP, wrong max",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -64),
			BPF_MOV64_IMM(BPF_REG_2, 16),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1, -128),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_2, 65, 4),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_JMP_REG(BPF_JGE, BPF_REG_4, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid stack type R1 off=-64 access_size=65",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: stack, JMP, no max check",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -64),
			BPF_MOV64_IMM(BPF_REG_2, 16),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1, -128),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_JMP_REG(BPF_JGE, BPF_REG_4, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		/* because max wasn't checked, signed min is negative */
		.errstr = "R2 min value is negative, either use unsigned or 'var &= const'",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: stack, JMP, no min check",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -64),
			BPF_MOV64_IMM(BPF_REG_2, 16),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1, -128),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_2, 64, 3),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid indirect read from stack off -64+0 size 64",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: stack, JMP (signed), no min check",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -64),
			BPF_MOV64_IMM(BPF_REG_2, 16),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_1, -128),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_2, 64, 3),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R2 min value is negative",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: map, JMP, correct bounds",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 10),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_2, sizeof(struct test_val)),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_10, -128),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_2,
				sizeof(struct test_val), 4),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_JMP_REG(BPF_JSGE, BPF_REG_4, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: map, JMP, wrong max",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 10),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_2, sizeof(struct test_val)),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_10, -128),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_2,
				sizeof(struct test_val) + 1, 4),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_JMP_REG(BPF_JSGE, BPF_REG_4, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "invalid access to map value, value_size=48 off=0 size=49",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: map adjusted, JMP, correct bounds",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 11),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 20),
			BPF_MOV64_IMM(BPF_REG_2, sizeof(struct test_val)),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_10, -128),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_2,
				sizeof(struct test_val) - 20, 4),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_JMP_REG(BPF_JSGE, BPF_REG_4, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: map adjusted, JMP, wrong max",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 11),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 20),
			BPF_MOV64_IMM(BPF_REG_2, sizeof(struct test_val)),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_10, -128),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_2,
				sizeof(struct test_val) - 19, 4),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_JMP_REG(BPF_JSGE, BPF_REG_4, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R1 min value is outside of the array range",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: size = 0 allowed on NULL (ARG_PTR_TO_MEM_OR_NULL)",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 0),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_EMIT_CALL(BPF_FUNC_csum_diff),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to variable memory: size > 0 not allowed on NULL (ARG_PTR_TO_MEM_OR_NULL)",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 0),
			BPF_MOV64_IMM(BPF_REG_2, 1),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_10, -128),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_2, 64),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_EMIT_CALL(BPF_FUNC_csum_diff),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 type=inv expected=fp",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to variable memory: size = 0 allowed on != NULL stack pointer (ARG_PTR_TO_MEM_OR_NULL)",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_2, 0),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_2, 8),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_EMIT_CALL(BPF_FUNC_csum_diff),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to variable memory: size = 0 allowed on != NULL map pointer (ARG_PTR_TO_MEM_OR_NULL)",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_EMIT_CALL(BPF_FUNC_csum_diff),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to variable memory: size possible = 0 allowed on != NULL stack pointer (ARG_PTR_TO_MEM_OR_NULL)",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 9),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_2, 8, 7),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_EMIT_CALL(BPF_FUNC_csum_diff),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to variable memory: size possible = 0 allowed on != NULL map pointer (ARG_PTR_TO_MEM_OR_NULL)",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_2, 8, 4),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_EMIT_CALL(BPF_FUNC_csum_diff),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"helper access to variable memory: size possible = 0 allowed on != NULL packet pointer (ARG_PTR_TO_MEM_OR_NULL)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 7),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_6, 0),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_2, 8, 4),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_EMIT_CALL(BPF_FUNC_csum_diff),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.retval = 0 /* csum_diff of 64-byte packet */,
	},
	{
		"helper access to variable memory: size = 0 not allowed on NULL (!ARG_PTR_TO_MEM_OR_NULL)",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 0),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 type=inv expected=fp",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: size > 0 not allowed on NULL (!ARG_PTR_TO_MEM_OR_NULL)",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_1, 0),
			BPF_MOV64_IMM(BPF_REG_2, 1),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 type=inv expected=fp",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: size = 0 allowed on != NULL stack pointer (!ARG_PTR_TO_MEM_OR_NULL)",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: size = 0 allowed on != NULL map pointer (!ARG_PTR_TO_MEM_OR_NULL)",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: size possible = 0 allowed on != NULL stack pointer (!ARG_PTR_TO_MEM_OR_NULL)",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_2, 8, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: size possible = 0 allowed on != NULL map pointer (!ARG_PTR_TO_MEM_OR_NULL)",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_2, 8, 2),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: 8 bytes leak",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -64),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -64),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -56),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -48),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -40),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -24),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -16),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_MOV64_IMM(BPF_REG_2, 1),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_2, -128),
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_10, -128),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_2, 63),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, 1),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_EXIT_INSN(),
		},
		.errstr = "invalid indirect read from stack off -64+32 size 64",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"helper access to variable memory: 8 bytes no leak (init memory)",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -64),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -56),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -48),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -40),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -32),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -24),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -16),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -64),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_2, 32),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, 32),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_EMIT_CALL(BPF_FUNC_probe_read),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"invalid and of negative number",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_LDX_MEM(BPF_B, BPF_REG_1, BPF_REG_0, 0),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_1, -4),
			BPF_ALU64_IMM(BPF_LSH, BPF_REG_1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0,
				   offsetof(struct test_val, foo)),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R0 max value is outside of the array range",
		.result = REJECT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"invalid range check",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 12),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_9, 1),
			BPF_ALU32_IMM(BPF_MOD, BPF_REG_1, 2),
			BPF_ALU32_IMM(BPF_ADD, BPF_REG_1, 1),
			BPF_ALU32_REG(BPF_AND, BPF_REG_9, BPF_REG_1),
			BPF_ALU32_IMM(BPF_ADD, BPF_REG_9, 1),
			BPF_ALU32_IMM(BPF_RSH, BPF_REG_9, 1),
			BPF_MOV32_IMM(BPF_REG_3, 1),
			BPF_ALU32_REG(BPF_SUB, BPF_REG_3, BPF_REG_9),
			BPF_ALU32_IMM(BPF_MUL, BPF_REG_3, 0x10000000),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_3),
			BPF_STX_MEM(BPF_W, BPF_REG_0, BPF_REG_3, 0),
			BPF_MOV64_REG(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr = "R0 max value is outside of the array range",
		.result = REJECT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"map in map access",
		.insns = {
			BPF_ST_MEM(0, BPF_REG_10, -4, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -4),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
			BPF_ST_MEM(0, BPF_REG_10, -4, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map_in_map = { 3 },
		.result = ACCEPT,
	},
	{
		"invalid inner map pointer",
		.insns = {
			BPF_ST_MEM(0, BPF_REG_10, -4, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -4),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_ST_MEM(0, BPF_REG_10, -4, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map_in_map = { 3 },
		.errstr = "R1 pointer arithmetic on CONST_PTR_TO_MAP prohibited",
		.result = REJECT,
	},
	{
		"forgot null checking on the inner map pointer",
		.insns = {
			BPF_ST_MEM(0, BPF_REG_10, -4, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -4),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_ST_MEM(0, BPF_REG_10, -4, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map_in_map = { 3 },
		.errstr = "R1 type=map_value_or_null expected=map_ptr",
		.result = REJECT,
	},
	{
		"ld_abs: check calling conv, r1",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_1, 0),
			BPF_LD_ABS(BPF_W, -0x200000),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 !read_ok",
		.result = REJECT,
	},
	{
		"ld_abs: check calling conv, r2",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_LD_ABS(BPF_W, -0x200000),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_EXIT_INSN(),
		},
		.errstr = "R2 !read_ok",
		.result = REJECT,
	},
	{
		"ld_abs: check calling conv, r3",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_LD_ABS(BPF_W, -0x200000),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_3),
			BPF_EXIT_INSN(),
		},
		.errstr = "R3 !read_ok",
		.result = REJECT,
	},
	{
		"ld_abs: check calling conv, r4",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_LD_ABS(BPF_W, -0x200000),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_4),
			BPF_EXIT_INSN(),
		},
		.errstr = "R4 !read_ok",
		.result = REJECT,
	},
	{
		"ld_abs: check calling conv, r5",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_LD_ABS(BPF_W, -0x200000),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_5),
			BPF_EXIT_INSN(),
		},
		.errstr = "R5 !read_ok",
		.result = REJECT,
	},
	{
		"ld_abs: check calling conv, r7",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_7, 0),
			BPF_LD_ABS(BPF_W, -0x200000),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_7),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"ld_abs: tests on r6 and skb data reload helper",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_LD_ABS(BPF_B, 0),
			BPF_LD_ABS(BPF_H, 0),
			BPF_LD_ABS(BPF_W, 0),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_6, 0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_7),
			BPF_MOV64_IMM(BPF_REG_2, 1),
			BPF_MOV64_IMM(BPF_REG_3, 2),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_vlan_push),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_7),
			BPF_LD_ABS(BPF_B, 0),
			BPF_LD_ABS(BPF_H, 0),
			BPF_LD_ABS(BPF_W, 0),
			BPF_MOV64_IMM(BPF_REG_0, 42),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 42 /* ultimate return value */,
	},
	{
		"ld_ind: check calling conv, r1",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_1, 1),
			BPF_LD_IND(BPF_W, BPF_REG_1, -0x200000),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 !read_ok",
		.result = REJECT,
	},
	{
		"ld_ind: check calling conv, r2",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_2, 1),
			BPF_LD_IND(BPF_W, BPF_REG_2, -0x200000),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_EXIT_INSN(),
		},
		.errstr = "R2 !read_ok",
		.result = REJECT,
	},
	{
		"ld_ind: check calling conv, r3",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_3, 1),
			BPF_LD_IND(BPF_W, BPF_REG_3, -0x200000),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_3),
			BPF_EXIT_INSN(),
		},
		.errstr = "R3 !read_ok",
		.result = REJECT,
	},
	{
		"ld_ind: check calling conv, r4",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_4, 1),
			BPF_LD_IND(BPF_W, BPF_REG_4, -0x200000),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_4),
			BPF_EXIT_INSN(),
		},
		.errstr = "R4 !read_ok",
		.result = REJECT,
	},
	{
		"ld_ind: check calling conv, r5",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_5, 1),
			BPF_LD_IND(BPF_W, BPF_REG_5, -0x200000),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_5),
			BPF_EXIT_INSN(),
		},
		.errstr = "R5 !read_ok",
		.result = REJECT,
	},
	{
		"ld_ind: check calling conv, r7",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_IMM(BPF_REG_7, 1),
			BPF_LD_IND(BPF_W, BPF_REG_7, -0x200000),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_7),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"check bpf_perf_event_data->sample_period byte load permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
#if __BYTE_ORDER == __LITTLE_ENDIAN
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct bpf_perf_event_data, sample_period)),
#else
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_1,
				    offsetof(struct bpf_perf_event_data, sample_period) + 7),
#endif
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_PERF_EVENT,
	},
	{
		"check bpf_perf_event_data->sample_period half load permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
#if __BYTE_ORDER == __LITTLE_ENDIAN
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct bpf_perf_event_data, sample_period)),
#else
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct bpf_perf_event_data, sample_period) + 6),
#endif
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_PERF_EVENT,
	},
	{
		"check bpf_perf_event_data->sample_period word load permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
#if __BYTE_ORDER == __LITTLE_ENDIAN
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct bpf_perf_event_data, sample_period)),
#else
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct bpf_perf_event_data, sample_period) + 4),
#endif
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_PERF_EVENT,
	},
	{
		"check bpf_perf_event_data->sample_period dword load permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1,
				    offsetof(struct bpf_perf_event_data, sample_period)),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_PERF_EVENT,
	},
	{
		"check skb->data half load not permitted",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
#if __BYTE_ORDER == __LITTLE_ENDIAN
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
#else
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, data) + 2),
#endif
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid bpf_context access",
	},
	{
		"check skb->tc_classid half load not permitted for lwt prog",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
#if __BYTE_ORDER == __LITTLE_ENDIAN
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, tc_classid)),
#else
			BPF_LDX_MEM(BPF_H, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, tc_classid) + 2),
#endif
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid bpf_context access",
		.prog_type = BPF_PROG_TYPE_LWT_IN,
	},
	{
		"bounds checks mixing signed and unsigned, positive bounds",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, 2),
			BPF_JMP_REG(BPF_JGE, BPF_REG_2, BPF_REG_1, 3),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_1, 4, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_B, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "unbounded min value",
		.result = REJECT,
	},
	{
		"bounds checks mixing signed and unsigned",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, -1),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_2, 3),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_1, 1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_B, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "unbounded min value",
		.result = REJECT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 2",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 9),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, -1),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_2, 5),
			BPF_MOV64_IMM(BPF_REG_8, 0),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_8, BPF_REG_1),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_8, 1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_8),
			BPF_ST_MEM(BPF_B, BPF_REG_8, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "unbounded min value",
		.result = REJECT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 3",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 8),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, -1),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_2, 4),
			BPF_MOV64_REG(BPF_REG_8, BPF_REG_1),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_8, 1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_8),
			BPF_ST_MEM(BPF_B, BPF_REG_8, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "unbounded min value",
		.result = REJECT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 4",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, 1),
			BPF_ALU64_REG(BPF_AND, BPF_REG_1, BPF_REG_2),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_1, 1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_B, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 5",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 9),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, -1),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_2, 5),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_1, 1, 4),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 4),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_B, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "unbounded min value",
		.result = REJECT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 6",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_3, -512),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_4, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_6, -1),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_6, 5),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_4, 1, 4),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 1),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_ST_MEM(BPF_H, BPF_REG_10, -512, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_load_bytes),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R4 min value is negative, either use unsigned",
		.result = REJECT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 7",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 7),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, 1024 * 1024 * 1024),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_2, 3),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_1, 1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_B, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 8",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 9),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, -1),
			BPF_JMP_REG(BPF_JGT, BPF_REG_2, BPF_REG_1, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_1, 1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_B, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "unbounded min value",
		.result = REJECT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 9",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 10),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_LD_IMM64(BPF_REG_2, -9223372036854775808ULL),
			BPF_JMP_REG(BPF_JGT, BPF_REG_2, BPF_REG_1, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_1, 1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_B, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 10",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 9),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_JMP_REG(BPF_JGT, BPF_REG_2, BPF_REG_1, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_1, 1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_B, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "unbounded min value",
		.result = REJECT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 11",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 9),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, -1),
			BPF_JMP_REG(BPF_JGE, BPF_REG_2, BPF_REG_1, 2),
			/* Dead branch. */
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_1, 1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_B, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "unbounded min value",
		.result = REJECT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 12",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 9),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, -6),
			BPF_JMP_REG(BPF_JGE, BPF_REG_2, BPF_REG_1, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_1, 1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_B, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "unbounded min value",
		.result = REJECT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 13",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, 2),
			BPF_JMP_REG(BPF_JGE, BPF_REG_2, BPF_REG_1, 2),
			BPF_MOV64_IMM(BPF_REG_7, 1),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_7, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_7, BPF_REG_1),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_7, 4, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_7),
			BPF_ST_MEM(BPF_B, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "unbounded min value",
		.result = REJECT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 14",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_9, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 8),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, -1),
			BPF_MOV64_IMM(BPF_REG_8, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_9, 42, 6),
			BPF_JMP_REG(BPF_JSGT, BPF_REG_8, BPF_REG_1, 3),
			BPF_JMP_IMM(BPF_JSGT, BPF_REG_1, 1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_ST_MEM(BPF_B, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_2, -3),
			BPF_JMP_IMM(BPF_JA, 0, 0, -7),
		},
		.fixup_map1 = { 4 },
		.errstr = "R0 invalid mem access 'inv'",
		.result = REJECT,
	},
	{
		"bounds checks mixing signed and unsigned, variant 15",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_10, -16),
			BPF_MOV64_IMM(BPF_REG_2, -6),
			BPF_JMP_REG(BPF_JGE, BPF_REG_2, BPF_REG_1, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_0, 1, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_ST_MEM(BPF_B, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "unbounded min value",
		.result = REJECT,
		.result_unpriv = REJECT,
	},
	{
		"subtraction bounds (map value) variant 1",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 9),
			BPF_LDX_MEM(BPF_B, BPF_REG_1, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_1, 0xff, 7),
			BPF_LDX_MEM(BPF_B, BPF_REG_3, BPF_REG_0, 1),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_3, 0xff, 5),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_1, BPF_REG_3),
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_1, 56),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "R0 max value is outside of the array range",
		.result = REJECT,
	},
	{
		"subtraction bounds (map value) variant 2",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 8),
			BPF_LDX_MEM(BPF_B, BPF_REG_1, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_1, 0xff, 6),
			BPF_LDX_MEM(BPF_B, BPF_REG_3, BPF_REG_0, 1),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_3, 0xff, 4),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_1, BPF_REG_3),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "R0 min value is negative, either use unsigned index or do a if (index >=0) check.",
		.result = REJECT,
	},
	{
		"bounds check based on zero-extended MOV",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			/* r2 = 0x0000'0000'ffff'ffff */
			BPF_MOV32_IMM(BPF_REG_2, 0xffffffff),
			/* r2 = 0 */
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_2, 32),
			/* no-op */
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_2),
			/* access at offset 0 */
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			/* exit */
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT
	},
	{
		"bounds check based on sign-extended MOV. test1",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			/* r2 = 0xffff'ffff'ffff'ffff */
			BPF_MOV64_IMM(BPF_REG_2, 0xffffffff),
			/* r2 = 0xffff'ffff */
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_2, 32),
			/* r0 = <oob pointer> */
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_2),
			/* access to OOB pointer */
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			/* exit */
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "map_value pointer and 4294967295",
		.result = REJECT
	},
	{
		"bounds check based on sign-extended MOV. test2",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			/* r2 = 0xffff'ffff'ffff'ffff */
			BPF_MOV64_IMM(BPF_REG_2, 0xffffffff),
			/* r2 = 0xfff'ffff */
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_2, 36),
			/* r0 = <oob pointer> */
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_2),
			/* access to OOB pointer */
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			/* exit */
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "R0 min value is outside of the array range",
		.result = REJECT
	},
	{
		"bounds check based on reg_off + var_off + insn_off. test1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_6, 1),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, (1 << 29) - 1),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, (1 << 29) - 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 3),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 4 },
		.errstr = "value_size=8 off=1073741825",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"bounds check based on reg_off + var_off + insn_off. test2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 4),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_6, 1),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, (1 << 30) - 1),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, (1 << 29) - 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 3),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 4 },
		.errstr = "value 1073741823",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"bounds check after truncation of non-boundary-crossing range",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 9),
			/* r1 = [0x00, 0xff] */
			BPF_LDX_MEM(BPF_B, BPF_REG_1, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_2, 1),
			/* r2 = 0x10'0000'0000 */
			BPF_ALU64_IMM(BPF_LSH, BPF_REG_2, 36),
			/* r1 = [0x10'0000'0000, 0x10'0000'00ff] */
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_2),
			/* r1 = [0x10'7fff'ffff, 0x10'8000'00fe] */
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 0x7fffffff),
			/* r1 = [0x00, 0xff] */
			BPF_ALU32_IMM(BPF_SUB, BPF_REG_1, 0x7fffffff),
			/* r1 = 0 */
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_1, 8),
			/* no-op */
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			/* access at offset 0 */
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			/* exit */
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT
	},
	{
		"bounds check after truncation of boundary-crossing range (1)",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 9),
			/* r1 = [0x00, 0xff] */
			BPF_LDX_MEM(BPF_B, BPF_REG_1, BPF_REG_0, 0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 0xffffff80 >> 1),
			/* r1 = [0xffff'ff80, 0x1'0000'007f] */
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 0xffffff80 >> 1),
			/* r1 = [0xffff'ff80, 0xffff'ffff] or
			 *      [0x0000'0000, 0x0000'007f]
			 */
			BPF_ALU32_IMM(BPF_ADD, BPF_REG_1, 0),
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_1, 0xffffff80 >> 1),
			/* r1 = [0x00, 0xff] or
			 *      [0xffff'ffff'0000'0080, 0xffff'ffff'ffff'ffff]
			 */
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_1, 0xffffff80 >> 1),
			/* r1 = 0 or
			 *      [0x00ff'ffff'ff00'0000, 0x00ff'ffff'ffff'ffff]
			 */
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_1, 8),
			/* no-op or OOB pointer computation */
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			/* potentially OOB access */
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			/* exit */
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		/* not actually fully unbounded, but the bound is very high */
		.errstr = "R0 unbounded memory access",
		.result = REJECT
	},
	{
		"bounds check after truncation of boundary-crossing range (2)",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 9),
			/* r1 = [0x00, 0xff] */
			BPF_LDX_MEM(BPF_B, BPF_REG_1, BPF_REG_0, 0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 0xffffff80 >> 1),
			/* r1 = [0xffff'ff80, 0x1'0000'007f] */
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 0xffffff80 >> 1),
			/* r1 = [0xffff'ff80, 0xffff'ffff] or
			 *      [0x0000'0000, 0x0000'007f]
			 * difference to previous test: truncation via MOV32
			 * instead of ALU32.
			 */
			BPF_MOV32_REG(BPF_REG_1, BPF_REG_1),
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_1, 0xffffff80 >> 1),
			/* r1 = [0x00, 0xff] or
			 *      [0xffff'ffff'0000'0080, 0xffff'ffff'ffff'ffff]
			 */
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_1, 0xffffff80 >> 1),
			/* r1 = 0 or
			 *      [0x00ff'ffff'ff00'0000, 0x00ff'ffff'ffff'ffff]
			 */
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_1, 8),
			/* no-op or OOB pointer computation */
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			/* potentially OOB access */
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			/* exit */
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		/* not actually fully unbounded, but the bound is very high */
		.errstr = "R0 unbounded memory access",
		.result = REJECT
	},
	{
		"bounds check after wrapping 32-bit addition",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
			/* r1 = 0x7fff'ffff */
			BPF_MOV64_IMM(BPF_REG_1, 0x7fffffff),
			/* r1 = 0xffff'fffe */
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 0x7fffffff),
			/* r1 = 0 */
			BPF_ALU32_IMM(BPF_ADD, BPF_REG_1, 2),
			/* no-op */
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			/* access at offset 0 */
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			/* exit */
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT
	},
	{
		"bounds check after shift with oversized count operand",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			BPF_MOV64_IMM(BPF_REG_2, 32),
			BPF_MOV64_IMM(BPF_REG_1, 1),
			/* r1 = (u32)1 << (u32)32 = ? */
			BPF_ALU32_REG(BPF_LSH, BPF_REG_1, BPF_REG_2),
			/* r1 = [0x0000, 0xffff] */
			BPF_ALU64_IMM(BPF_AND, BPF_REG_1, 0xffff),
			/* computes unknown pointer, potentially OOB */
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			/* potentially OOB access */
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			/* exit */
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "R0 max value is outside of the array range",
		.result = REJECT
	},
	{
		"bounds check after right shift of maybe-negative number",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 6),
			/* r1 = [0x00, 0xff] */
			BPF_LDX_MEM(BPF_B, BPF_REG_1, BPF_REG_0, 0),
			/* r1 = [-0x01, 0xfe] */
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_1, 1),
			/* r1 = 0 or 0xff'ffff'ffff'ffff */
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_1, 8),
			/* r1 = 0 or 0xffff'ffff'ffff */
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_1, 8),
			/* computes unknown pointer, potentially OOB */
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			/* potentially OOB access */
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			/* exit */
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "R0 unbounded memory access",
		.result = REJECT
	},
	{
		"bounds check map access with off+size signed 32bit overflow. test1",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 1),
			BPF_EXIT_INSN(),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 0x7ffffffe),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_0, 0),
			BPF_JMP_A(0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "map_value pointer and 2147483646",
		.result = REJECT
	},
	{
		"bounds check map access with off+size signed 32bit overflow. test2",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 1),
			BPF_EXIT_INSN(),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 0x1fffffff),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 0x1fffffff),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 0x1fffffff),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_0, 0),
			BPF_JMP_A(0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "pointer offset 1073741822",
		.result = REJECT
	},
	{
		"bounds check map access with off+size signed 32bit overflow. test3",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 1),
			BPF_EXIT_INSN(),
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_0, 0x1fffffff),
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_0, 0x1fffffff),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_0, 2),
			BPF_JMP_A(0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "pointer offset -1073741822",
		.result = REJECT
	},
	{
		"bounds check map access with off+size signed 32bit overflow. test4",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_1, 1000000),
			BPF_ALU64_IMM(BPF_MUL, BPF_REG_1, 1000000),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_0, 2),
			BPF_JMP_A(0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "map_value pointer and 1000000000000",
		.result = REJECT
	},
	{
		"pointer/scalar confusion in state equality check (way 1)",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_0, 0),
			BPF_JMP_A(1),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_10),
			BPF_JMP_A(0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT,
		.retval = POINTER_VALUE,
		.result_unpriv = REJECT,
		.errstr_unpriv = "R0 leaks addr as return value"
	},
	{
		"pointer/scalar confusion in state equality check (way 2)",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_10),
			BPF_JMP_A(1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = ACCEPT,
		.retval = POINTER_VALUE,
		.result_unpriv = REJECT,
		.errstr_unpriv = "R0 leaks addr as return value"
	},
	{
		"variable-offset ctx access",
		.insns = {
			/* Get an unknown value */
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1, 0),
			/* Make it small and 4-byte aligned */
			BPF_ALU64_IMM(BPF_AND, BPF_REG_2, 4),
			/* add it to skb.  We now have either &skb->len or
			 * &skb->pkt_type, but we don't know which
			 */
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_2),
			/* dereference it */
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "variable ctx access var_off=(0x0; 0x4)",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_LWT_IN,
	},
	{
		"variable-offset stack access",
		.insns = {
			/* Fill the top 8 bytes of the stack */
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			/* Get an unknown value */
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1, 0),
			/* Make it small and 4-byte aligned */
			BPF_ALU64_IMM(BPF_AND, BPF_REG_2, 4),
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_2, 8),
			/* add it to fp.  We now have either fp-4 or fp-8, but
			 * we don't know which
			 */
			BPF_ALU64_REG(BPF_ADD, BPF_REG_2, BPF_REG_10),
			/* dereference it */
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_2, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "variable stack access var_off=(0xfffffffffffffff8; 0x4)",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_LWT_IN,
	},
	{
		"indirect variable-offset stack access",
		.insns = {
			/* Fill the top 8 bytes of the stack */
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			/* Get an unknown value */
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1, 0),
			/* Make it small and 4-byte aligned */
			BPF_ALU64_IMM(BPF_AND, BPF_REG_2, 4),
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_2, 8),
			/* add it to fp.  We now have either fp-4 or fp-8, but
			 * we don't know which
			 */
			BPF_ALU64_REG(BPF_ADD, BPF_REG_2, BPF_REG_10),
			/* dereference it indirectly */
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 5 },
		.errstr = "variable stack read R2",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_LWT_IN,
	},
	{
		"direct stack access with 32-bit wraparound. test1",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 0x7fffffff),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 0x7fffffff),
			BPF_MOV32_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0, 0),
			BPF_EXIT_INSN()
		},
		.errstr = "fp pointer and 2147483647",
		.result = REJECT
	},
	{
		"direct stack access with 32-bit wraparound. test2",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 0x3fffffff),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 0x3fffffff),
			BPF_MOV32_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0, 0),
			BPF_EXIT_INSN()
		},
		.errstr = "fp pointer and 1073741823",
		.result = REJECT
	},
	{
		"direct stack access with 32-bit wraparound. test3",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 0x1fffffff),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 0x1fffffff),
			BPF_MOV32_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_B, BPF_REG_1, BPF_REG_0, 0),
			BPF_EXIT_INSN()
		},
		.errstr = "fp pointer offset 1073741822",
		.result = REJECT
	},
	{
		"liveness pruning and write screening",
		.insns = {
			/* Get an unknown value */
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1, 0),
			/* branch conditions teach us nothing about R2 */
			BPF_JMP_IMM(BPF_JGE, BPF_REG_2, 0, 1),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JGE, BPF_REG_2, 0, 1),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R0 !read_ok",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_LWT_IN,
	},
	{
		"varlen_map_value_access pruning",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_0, 0),
			BPF_MOV32_IMM(BPF_REG_2, MAX_ENTRIES),
			BPF_JMP_REG(BPF_JSGT, BPF_REG_2, BPF_REG_1, 1),
			BPF_MOV32_IMM(BPF_REG_1, 0),
			BPF_ALU32_IMM(BPF_LSH, BPF_REG_1, 2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 0),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0,
				   offsetof(struct test_val, foo)),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 3 },
		.errstr_unpriv = "R0 leaks addr",
		.errstr = "R0 unbounded memory access",
		.result_unpriv = REJECT,
		.result = REJECT,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"invalid 64-bit BPF_END",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_0, 0),
			{
				.code  = BPF_ALU64 | BPF_END | BPF_TO_LE,
				.dst_reg = BPF_REG_0,
				.src_reg = 0,
				.off   = 0,
				.imm   = 32,
			},
			BPF_EXIT_INSN(),
		},
		.errstr = "unknown opcode d7",
		.result = REJECT,
	},
	{
		"XDP, using ifindex from netdev",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, ingress_ifindex)),
			BPF_JMP_IMM(BPF_JLT, BPF_REG_2, 1, 1),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.retval = 1,
	},
	{
		"meta access, test1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"meta access, test2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_0, 8),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid access to packet, off=-8",
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"meta access, test3",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"meta access, test4",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_LDX_MEM(BPF_W, BPF_REG_4, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_4),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"meta access, test5",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_4, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_3),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_4, 3),
			BPF_MOV64_IMM(BPF_REG_2, -8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_xdp_adjust_meta),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_3, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R3 !read_ok",
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"meta access, test6",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_3),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_0, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"meta access, test7",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_3),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"meta access, test8",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 0xFFFF),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"meta access, test9",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 0xFFFF),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 1),
			BPF_JMP_REG(BPF_JGT, BPF_REG_4, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"meta access, test10",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_4, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_IMM(BPF_REG_5, 42),
			BPF_MOV64_IMM(BPF_REG_6, 24),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_5, -8),
			BPF_STX_XADD(BPF_DW, BPF_REG_10, BPF_REG_6, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_5, BPF_REG_10, -8),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_5, 100, 6),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_3, BPF_REG_5),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_3),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_6, BPF_REG_5, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_2, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "invalid access to packet",
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"meta access, test11",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_IMM(BPF_REG_5, 42),
			BPF_MOV64_IMM(BPF_REG_6, 24),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_5, -8),
			BPF_STX_XADD(BPF_DW, BPF_REG_10, BPF_REG_6, -8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_5, BPF_REG_10, -8),
			BPF_JMP_IMM(BPF_JGT, BPF_REG_5, 100, 6),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_2, BPF_REG_5),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_2),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_6, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_5, BPF_REG_5, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"meta access, test12",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_4, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_3),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_5, 16),
			BPF_JMP_REG(BPF_JGT, BPF_REG_5, BPF_REG_4, 5),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_3, 0),
			BPF_MOV64_REG(BPF_REG_5, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_5, 16),
			BPF_JMP_REG(BPF_JGT, BPF_REG_5, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"arithmetic ops make PTR_TO_CTX unusable",
		.insns = {
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1,
				      offsetof(struct __sk_buff, data) -
				      offsetof(struct __sk_buff, mark)),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_EXIT_INSN(),
		},
		.errstr = "dereference of modified ctx ptr",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"pkt_end - pkt_start is allowed",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_0, BPF_REG_2),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = TEST_DATA_LEN,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"XDP pkt read, pkt_end mangling, bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_3, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R3 pointer arithmetic on PTR_TO_PACKET_END",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_end mangling, bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_ALU64_IMM(BPF_SUB, BPF_REG_3, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R3 pointer arithmetic on PTR_TO_PACKET_END",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data' > pkt_end, good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data' > pkt_end, bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -4),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_data' > pkt_end, bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_3, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_end > pkt_data', good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_3, BPF_REG_1, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, -5),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_end > pkt_data', bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_3, BPF_REG_1, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_end > pkt_data', bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_3, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data' < pkt_end, good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_1, BPF_REG_3, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, -5),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_data' < pkt_end, bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_1, BPF_REG_3, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data' < pkt_end, bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_end < pkt_data', good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_3, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_end < pkt_data', bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_3, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -4),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_end < pkt_data', bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_3, BPF_REG_1, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data' >= pkt_end, good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, -5),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_data' >= pkt_end, bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data' >= pkt_end, bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_1, BPF_REG_3, 0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, -5),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_end >= pkt_data', good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_3, BPF_REG_1, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_end >= pkt_data', bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_3, BPF_REG_1, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -4),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_end >= pkt_data', bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_3, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data' <= pkt_end, good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_1, BPF_REG_3, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data' <= pkt_end, bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_1, BPF_REG_3, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -4),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_data' <= pkt_end, bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_end <= pkt_data', good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_3, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, -5),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_end <= pkt_data', bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_3, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_end <= pkt_data', bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_3, BPF_REG_1, 0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, -5),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_meta' > pkt_data, good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_meta' > pkt_data, bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -4),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_meta' > pkt_data, bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_1, BPF_REG_3, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data > pkt_meta', good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_3, BPF_REG_1, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, -5),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_data > pkt_meta', bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_3, BPF_REG_1, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data > pkt_meta', bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_3, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_meta' < pkt_data, good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_1, BPF_REG_3, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, -5),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_meta' < pkt_data, bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_1, BPF_REG_3, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_meta' < pkt_data, bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data < pkt_meta', good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_3, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data < pkt_meta', bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_3, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -4),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_data < pkt_meta', bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_3, BPF_REG_1, 0),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_meta' >= pkt_data, good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, -5),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_meta' >= pkt_data, bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_meta' >= pkt_data, bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_1, BPF_REG_3, 0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, -5),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_data >= pkt_meta', good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_3, BPF_REG_1, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data >= pkt_meta', bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_3, BPF_REG_1, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -4),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_data >= pkt_meta', bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JGE, BPF_REG_3, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_meta' <= pkt_data, good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_1, BPF_REG_3, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_meta' <= pkt_data, bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_1, BPF_REG_3, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -4),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_meta' <= pkt_data, bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_1, BPF_REG_3, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data <= pkt_meta', good access",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_3, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, -5),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"XDP pkt read, pkt_data <= pkt_meta', bad access 1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_3, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, -8),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"XDP pkt read, pkt_data <= pkt_meta', bad access 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data_meta)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_3, BPF_REG_1, 0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, -5),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R1 offset is outside of the packet",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
		.flags = F_NEEDS_EFFICIENT_UNALIGNED_ACCESS,
	},
	{
		"check deducing bounds from const, 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_JMP_IMM(BPF_JSGE, BPF_REG_0, 1, 0),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R0 tried to subtract pointer from scalar",
	},
	{
		"check deducing bounds from const, 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_JMP_IMM(BPF_JSGE, BPF_REG_0, 1, 1),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JSLE, BPF_REG_0, 1, 1),
			BPF_EXIT_INSN(),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_1, BPF_REG_0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"check deducing bounds from const, 3",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JSLE, BPF_REG_0, 0, 0),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R0 tried to subtract pointer from scalar",
	},
	{
		"check deducing bounds from const, 4",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JSLE, BPF_REG_0, 0, 1),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JSGE, BPF_REG_0, 0, 1),
			BPF_EXIT_INSN(),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_1, BPF_REG_0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
	},
	{
		"check deducing bounds from const, 5",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JSGE, BPF_REG_0, 0, 1),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R0 tried to subtract pointer from scalar",
	},
	{
		"check deducing bounds from const, 6",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JSGE, BPF_REG_0, 0, 1),
			BPF_EXIT_INSN(),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R0 tried to subtract pointer from scalar",
	},
	{
		"check deducing bounds from const, 7",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, ~0),
			BPF_JMP_IMM(BPF_JSGE, BPF_REG_0, 0, 0),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "dereference of modified ctx ptr",
	},
	{
		"check deducing bounds from const, 8",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, ~0),
			BPF_JMP_IMM(BPF_JSGE, BPF_REG_0, 0, 1),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "dereference of modified ctx ptr",
	},
	{
		"check deducing bounds from const, 9",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JSGE, BPF_REG_0, 0, 0),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R0 tried to subtract pointer from scalar",
	},
	{
		"check deducing bounds from const, 10",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JSLE, BPF_REG_0, 0, 0),
			/* Marks reg as unknown. */
			BPF_ALU64_IMM(BPF_NEG, BPF_REG_0, 0),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "math between ctx pointer and register with unbounded min value is not allowed",
	},
	{
		"bpf_exit with invalid return code. test1",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, 0),
			BPF_EXIT_INSN(),
		},
		.errstr = "R0 has value (0x0; 0xffffffff)",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_CGROUP_SOCK,
	},
	{
		"bpf_exit with invalid return code. test2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, 0),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_CGROUP_SOCK,
	},
	{
		"bpf_exit with invalid return code. test3",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, 0),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_0, 3),
			BPF_EXIT_INSN(),
		},
		.errstr = "R0 has value (0x0; 0x3)",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_CGROUP_SOCK,
	},
	{
		"bpf_exit with invalid return code. test4",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_CGROUP_SOCK,
	},
	{
		"bpf_exit with invalid return code. test5",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.errstr = "R0 has value (0x2; 0x0)",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_CGROUP_SOCK,
	},
	{
		"bpf_exit with invalid return code. test6",
		.insns = {
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.errstr = "R0 is not a known value (ctx)",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_CGROUP_SOCK,
	},
	{
		"bpf_exit with invalid return code. test7",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, 0),
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1, 4),
			BPF_ALU64_REG(BPF_MUL, BPF_REG_0, BPF_REG_2),
			BPF_EXIT_INSN(),
		},
		.errstr = "R0 has unknown scalar value",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_CGROUP_SOCK,
	},
	{
		"calls: basic sanity",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.result = ACCEPT,
	},
	{
		"calls: not on unpriviledged",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "function calls to other bpf functions are allowed for root only",
		.result_unpriv = REJECT,
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"calls: div by 0 in subprog",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 8),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_2, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV32_IMM(BPF_REG_2, 0),
			BPF_MOV32_IMM(BPF_REG_3, 1),
			BPF_ALU32_REG(BPF_DIV, BPF_REG_3, BPF_REG_2),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"calls: multiple ret types in subprog 1",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 8),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_2, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 1),
			BPF_MOV32_IMM(BPF_REG_0, 42),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = REJECT,
		.errstr = "R0 invalid mem access 'inv'",
	},
	{
		"calls: multiple ret types in subprog 2",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 8),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_0),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_2, BPF_REG_1, 1),
			BPF_LDX_MEM(BPF_B, BPF_REG_0, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 9),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 1),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_6,
				    offsetof(struct __sk_buff, data)),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 64),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.fixup_map1 = { 16 },
		.result = REJECT,
		.errstr = "R0 min value is outside of the array range",
	},
	{
		"calls: overlapping caller/callee",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "last insn is not an exit or jmp",
		.result = REJECT,
	},
	{
		"calls: wrong recursive calls",
		.insns = {
			BPF_JMP_IMM(BPF_JA, 0, 0, 4),
			BPF_JMP_IMM(BPF_JA, 0, 0, 4),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, -2),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, -2),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, -2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "jump out of range",
		.result = REJECT,
	},
	{
		"calls: wrong src reg",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 2, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "BPF_CALL uses reserved fields",
		.result = REJECT,
	},
	{
		"calls: wrong off value",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, -1, 2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "BPF_CALL uses reserved fields",
		.result = REJECT,
	},
	{
		"calls: jump back loop",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, -1),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "back-edge from insn 0 to 0",
		.result = REJECT,
	},
	{
		"calls: conditional call",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 3),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "jump out of range",
		.result = REJECT,
	},
	{
		"calls: conditional call 2",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 3),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 4),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 3),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.result = ACCEPT,
	},
	{
		"calls: conditional call 3",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 3),
			BPF_JMP_IMM(BPF_JA, 0, 0, 4),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, -6),
			BPF_MOV64_IMM(BPF_REG_0, 3),
			BPF_JMP_IMM(BPF_JA, 0, 0, -6),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "back-edge from insn",
		.result = REJECT,
	},
	{
		"calls: conditional call 4",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 3),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 4),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, -5),
			BPF_MOV64_IMM(BPF_REG_0, 3),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.result = ACCEPT,
	},
	{
		"calls: conditional call 5",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 3),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 4),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, -6),
			BPF_MOV64_IMM(BPF_REG_0, 3),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "back-edge from insn",
		.result = REJECT,
	},
	{
		"calls: conditional call 6",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, -2),
			BPF_EXIT_INSN(),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "back-edge from insn",
		.result = REJECT,
	},
	{
		"calls: using r0 returned by callee",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.result = ACCEPT,
	},
	{
		"calls: using uninit r0 from callee",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "!read_ok",
		.result = REJECT,
	},
	{
		"calls: callee is using r1",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, len)),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_ACT,
		.result = ACCEPT,
		.retval = TEST_DATA_LEN,
	},
	{
		"calls: callee using args1",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_1),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "allowed for root only",
		.result_unpriv = REJECT,
		.result = ACCEPT,
		.retval = POINTER_VALUE,
	},
	{
		"calls: callee using wrong args2",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "R2 !read_ok",
		.result = REJECT,
	},
	{
		"calls: callee using two args",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_LDX_MEM(BPF_W, BPF_REG_1, BPF_REG_6,
				    offsetof(struct __sk_buff, len)),
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_6,
				    offsetof(struct __sk_buff, len)),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_1),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_2),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "allowed for root only",
		.result_unpriv = REJECT,
		.result = ACCEPT,
		.retval = TEST_DATA_LEN + TEST_DATA_LEN - ETH_HLEN - ETH_HLEN,
	},
	{
		"calls: callee changing pkt pointers",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_8, BPF_REG_6),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_8, 8),
			BPF_JMP_REG(BPF_JGT, BPF_REG_8, BPF_REG_7, 2),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			/* clear_all_pkt_pointers() has to walk all frames
			 * to make sure that pkt pointers in the caller
			 * are cleared when callee is calling a helper that
			 * adjusts packet size
			 */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_MOV32_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_xdp_adjust_head),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "R6 invalid mem access 'inv'",
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"calls: two calls with args",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 6),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_7, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_7),
			BPF_EXIT_INSN(),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, len)),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = TEST_DATA_LEN + TEST_DATA_LEN,
	},
	{
		"calls: calls with stack arith",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -64),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -64),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -64),
			BPF_MOV64_IMM(BPF_REG_0, 42),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 42,
	},
	{
		"calls: calls with misaligned stack access",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -63),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -61),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -63),
			BPF_MOV64_IMM(BPF_REG_0, 42),
			BPF_STX_MEM(BPF_DW, BPF_REG_2, BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.flags = F_LOAD_WITH_STRICT_ALIGNMENT,
		.errstr = "misaligned stack access",
		.result = REJECT,
	},
	{
		"calls: calls control flow, jump test",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 42),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 43),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_JMP_IMM(BPF_JA, 0, 0, -3),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 43,
	},
	{
		"calls: calls control flow, jump test 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 42),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 43),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, -3),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.errstr = "jump out of range from insn 1 to 4",
		.result = REJECT,
	},
	{
		"calls: two calls with bad jump",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 6),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_7, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_7),
			BPF_EXIT_INSN(),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, len)),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, -3),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "jump out of range from insn 11 to 9",
		.result = REJECT,
	},
	{
		"calls: recursive call. test1",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, -1),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "back-edge",
		.result = REJECT,
	},
	{
		"calls: recursive call. test2",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, -3),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "back-edge",
		.result = REJECT,
	},
	{
		"calls: unreachable code",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "unreachable insn 6",
		.result = REJECT,
	},
	{
		"calls: invalid call",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, -4),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "invalid destination",
		.result = REJECT,
	},
	{
		"calls: invalid call 2",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 0x7fffffff),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "invalid destination",
		.result = REJECT,
	},
	{
		"calls: jumping across function bodies. test1",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, -3),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "jump out of range",
		.result = REJECT,
	},
	{
		"calls: jumping across function bodies. test2",
		.insns = {
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 3),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "jump out of range",
		.result = REJECT,
	},
	{
		"calls: call without exit",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, -2),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "not an exit",
		.result = REJECT,
	},
	{
		"calls: call into middle of ld_imm64",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_LD_IMM64(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "last insn",
		.result = REJECT,
	},
	{
		"calls: call into middle of other call",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "last insn",
		.result = REJECT,
	},
	{
		"calls: ld_abs with changing ctx data in callee",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_LD_ABS(BPF_B, 0),
			BPF_LD_ABS(BPF_H, 0),
			BPF_LD_ABS(BPF_W, 0),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_6),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 5),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_7),
			BPF_LD_ABS(BPF_B, 0),
			BPF_LD_ABS(BPF_H, 0),
			BPF_LD_ABS(BPF_W, 0),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_2, 1),
			BPF_MOV64_IMM(BPF_REG_3, 2),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_vlan_push),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.errstr = "BPF_LD_[ABS|IND] instructions cannot be mixed",
		.result = REJECT,
	},
	{
		"calls: two calls with bad fallthrough",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 6),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_7, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_7),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_0),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1,
				    offsetof(struct __sk_buff, len)),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.errstr = "not an exit",
		.result = REJECT,
	},
	{
		"calls: two calls with stack read",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 6),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_7, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_7),
			BPF_EXIT_INSN(),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		.result = ACCEPT,
	},
	{
		"calls: two calls with stack write",
		.insns = {
			/* main prog */
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -16),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_10, -16),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_2),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 7),
			BPF_MOV64_REG(BPF_REG_8, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_8, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_8),
			/* write into stack frame of main prog */
			BPF_STX_MEM(BPF_DW, BPF_REG_7, BPF_REG_0, 0),
			BPF_EXIT_INSN(),

			/* subprog 2 */
			/* read from stack frame of main prog */
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_1, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		.result = ACCEPT,
	},
	{
		"calls: stack overflow using two frames (pre-call access)",
		.insns = {
			/* prog 1 */
			BPF_ST_MEM(BPF_B, BPF_REG_10, -300, 0),
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),

			/* prog 2 */
			BPF_ST_MEM(BPF_B, BPF_REG_10, -300, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		.errstr = "combined stack size",
		.result = REJECT,
	},
	{
		"calls: stack overflow using two frames (post-call access)",
		.insns = {
			/* prog 1 */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 2),
			BPF_ST_MEM(BPF_B, BPF_REG_10, -300, 0),
			BPF_EXIT_INSN(),

			/* prog 2 */
			BPF_ST_MEM(BPF_B, BPF_REG_10, -300, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		.errstr = "combined stack size",
		.result = REJECT,
	},
	{
		"calls: stack depth check using three frames. test1",
		.insns = {
			/* main */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 4), /* call A */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 5), /* call B */
			BPF_ST_MEM(BPF_B, BPF_REG_10, -32, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			/* A */
			BPF_ST_MEM(BPF_B, BPF_REG_10, -256, 0),
			BPF_EXIT_INSN(),
			/* B */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, -3), /* call A */
			BPF_ST_MEM(BPF_B, BPF_REG_10, -64, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		/* stack_main=32, stack_A=256, stack_B=64
		 * and max(main+A, main+A+B) < 512
		 */
		.result = ACCEPT,
	},
	{
		"calls: stack depth check using three frames. test2",
		.insns = {
			/* main */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 4), /* call A */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 5), /* call B */
			BPF_ST_MEM(BPF_B, BPF_REG_10, -32, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			/* A */
			BPF_ST_MEM(BPF_B, BPF_REG_10, -64, 0),
			BPF_EXIT_INSN(),
			/* B */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, -3), /* call A */
			BPF_ST_MEM(BPF_B, BPF_REG_10, -256, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		/* stack_main=32, stack_A=64, stack_B=256
		 * and max(main+A, main+A+B) < 512
		 */
		.result = ACCEPT,
	},
	{
		"calls: stack depth check using three frames. test3",
		.insns = {
			/* main */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 6), /* call A */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 8), /* call B */
			BPF_JMP_IMM(BPF_JGE, BPF_REG_6, 0, 1),
			BPF_ST_MEM(BPF_B, BPF_REG_10, -64, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			/* A */
			BPF_JMP_IMM(BPF_JLT, BPF_REG_1, 10, 1),
			BPF_EXIT_INSN(),
			BPF_ST_MEM(BPF_B, BPF_REG_10, -224, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, -3),
			/* B */
			BPF_JMP_IMM(BPF_JGT, BPF_REG_1, 2, 1),
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, -6), /* call A */
			BPF_ST_MEM(BPF_B, BPF_REG_10, -256, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		/* stack_main=64, stack_A=224, stack_B=256
		 * and max(main+A, main+A+B) > 512
		 */
		.errstr = "combined stack",
		.result = REJECT,
	},
	{
		"calls: stack depth check using three frames. test4",
		/* void main(void) {
		 *   func1(0);
		 *   func1(1);
		 *   func2(1);
		 * }
		 * void func1(int alloc_or_recurse) {
		 *   if (alloc_or_recurse) {
		 *     frame_pointer[-300] = 1;
		 *   } else {
		 *     func2(alloc_or_recurse);
		 *   }
		 * }
		 * void func2(int alloc_or_recurse) {
		 *   if (alloc_or_recurse) {
		 *     frame_pointer[-300] = 1;
		 *   }
		 * }
		 */
		.insns = {
			/* main */
			BPF_MOV64_IMM(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 6), /* call A */
			BPF_MOV64_IMM(BPF_REG_1, 1),
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 4), /* call A */
			BPF_MOV64_IMM(BPF_REG_1, 1),
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 7), /* call B */
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			/* A */
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 2),
			BPF_ST_MEM(BPF_B, BPF_REG_10, -300, 0),
			BPF_EXIT_INSN(),
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 1), /* call B */
			BPF_EXIT_INSN(),
			/* B */
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 1),
			BPF_ST_MEM(BPF_B, BPF_REG_10, -300, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		.result = REJECT,
		.errstr = "combined stack",
	},
	{
		"calls: stack depth check using three frames. test5",
		.insns = {
			/* main */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 1), /* call A */
			BPF_EXIT_INSN(),
			/* A */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 1), /* call B */
			BPF_EXIT_INSN(),
			/* B */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 1), /* call C */
			BPF_EXIT_INSN(),
			/* C */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 1), /* call D */
			BPF_EXIT_INSN(),
			/* D */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 1), /* call E */
			BPF_EXIT_INSN(),
			/* E */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 1), /* call F */
			BPF_EXIT_INSN(),
			/* F */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 1), /* call G */
			BPF_EXIT_INSN(),
			/* G */
			BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 1), /* call H */
			BPF_EXIT_INSN(),
			/* H */
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		.errstr = "call stack",
		.result = REJECT,
	},
	{
		"calls: spill into caller stack frame",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),
			BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_1, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		.errstr = "cannot spill",
		.result = REJECT,
	},
	{
		"calls: write into caller stack frame",
		.insns = {
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			BPF_EXIT_INSN(),
			BPF_ST_MEM(BPF_DW, BPF_REG_1, 0, 42),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		.result = ACCEPT,
		.retval = 42,
	},
	{
		"calls: write into callee stack frame",
		.insns = {
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 42),
			BPF_EXIT_INSN(),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, -8),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		.errstr = "cannot return stack pointer",
		.result = REJECT,
	},
	{
		"calls: two calls with stack write and void return",
		.insns = {
			/* main prog */
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -16),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_10, -16),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_2),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_7),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),

			/* subprog 2 */
			/* write into stack frame of main prog */
			BPF_ST_MEM(BPF_DW, BPF_REG_1, 0, 0),
			BPF_EXIT_INSN(), /* void return */
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		.result = ACCEPT,
	},
	{
		"calls: ambiguous return value",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 5),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_EXIT_INSN(),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 1),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.errstr_unpriv = "allowed for root only",
		.result_unpriv = REJECT,
		.errstr = "R0 !read_ok",
		.result = REJECT,
	},
	{
		"calls: two calls that return map_value",
		.insns = {
			/* main prog */
			/* pass fp-16, fp-8 into a function */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -16),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 8),

			/* fetch map_value_ptr from the stack of this function */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_10, -8),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),
			/* fetch secound map_value_ptr from the stack */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_10, -16),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			/* call 3rd function twice */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_2),
			/* first time with fp-8 */
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_7),
			/* second time with fp-16 */
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),

			/* subprog 2 */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			/* lookup from map */
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			/* write map_value_ptr into stack frame of main prog */
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(), /* return 0 */
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		.fixup_map1 = { 23 },
		.result = ACCEPT,
	},
	{
		"calls: two calls that return map_value with bool condition",
		.insns = {
			/* main prog */
			/* pass fp-16, fp-8 into a function */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -16),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			/* call 3rd function twice */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_2),
			/* first time with fp-8 */
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 9),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 1, 2),
			/* fetch map_value_ptr from the stack of this function */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_7),
			/* second time with fp-16 */
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 4),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 1, 2),
			/* fetch secound map_value_ptr from the stack */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_7, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),
			BPF_EXIT_INSN(),

			/* subprog 2 */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			/* lookup from map */
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(), /* return 0 */
			/* write map_value_ptr into stack frame of main prog */
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(), /* return 1 */
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		.fixup_map1 = { 23 },
		.result = ACCEPT,
	},
	{
		"calls: two calls that return map_value with incorrect bool check",
		.insns = {
			/* main prog */
			/* pass fp-16, fp-8 into a function */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -16),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			/* call 3rd function twice */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_2),
			/* first time with fp-8 */
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 9),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 1, 2),
			/* fetch map_value_ptr from the stack of this function */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_7),
			/* second time with fp-16 */
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 4),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			/* fetch secound map_value_ptr from the stack */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_7, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),
			BPF_EXIT_INSN(),

			/* subprog 2 */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			/* lookup from map */
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(), /* return 0 */
			/* write map_value_ptr into stack frame of main prog */
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(), /* return 1 */
		},
		.prog_type = BPF_PROG_TYPE_XDP,
		.fixup_map1 = { 23 },
		.result = REJECT,
		.errstr = "invalid read from stack off -16+0 size 8",
	},
	{
		"calls: two calls that receive map_value via arg=ptr_stack_of_caller. test1",
		.insns = {
			/* main prog */
			/* pass fp-16, fp-8 into a function */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -16),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_2),
			/* 1st lookup from map */
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_8, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			/* write map_value_ptr into stack frame of main prog at fp-8 */
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_8, 1),

			/* 2nd lookup from map */
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10), /* 20 */
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, /* 24 */
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_9, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			/* write map_value_ptr into stack frame of main prog at fp-16 */
			BPF_STX_MEM(BPF_DW, BPF_REG_7, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_9, 1),

			/* call 3rd func with fp-8, 0|1, fp-16, 0|1 */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6), /* 30 */
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_7),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_9),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),  /* 34 */
			BPF_EXIT_INSN(),

			/* subprog 2 */
			/* if arg2 == 1 do *arg1 = 0 */
			BPF_JMP_IMM(BPF_JNE, BPF_REG_2, 1, 2),
			/* fetch map_value_ptr from the stack of this function */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),

			/* if arg4 == 1 do *arg3 = 0 */
			BPF_JMP_IMM(BPF_JNE, BPF_REG_4, 1, 2),
			/* fetch map_value_ptr from the stack of this function */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_3, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 2, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.fixup_map1 = { 12, 22 },
		.result = REJECT,
		.errstr = "invalid access to map value, value_size=8 off=2 size=8",
	},
	{
		"calls: two calls that receive map_value via arg=ptr_stack_of_caller. test2",
		.insns = {
			/* main prog */
			/* pass fp-16, fp-8 into a function */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -16),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_2),
			/* 1st lookup from map */
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_8, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			/* write map_value_ptr into stack frame of main prog at fp-8 */
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_8, 1),

			/* 2nd lookup from map */
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10), /* 20 */
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, /* 24 */
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_9, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			/* write map_value_ptr into stack frame of main prog at fp-16 */
			BPF_STX_MEM(BPF_DW, BPF_REG_7, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_9, 1),

			/* call 3rd func with fp-8, 0|1, fp-16, 0|1 */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6), /* 30 */
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_7),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_9),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),  /* 34 */
			BPF_EXIT_INSN(),

			/* subprog 2 */
			/* if arg2 == 1 do *arg1 = 0 */
			BPF_JMP_IMM(BPF_JNE, BPF_REG_2, 1, 2),
			/* fetch map_value_ptr from the stack of this function */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),

			/* if arg4 == 1 do *arg3 = 0 */
			BPF_JMP_IMM(BPF_JNE, BPF_REG_4, 1, 2),
			/* fetch map_value_ptr from the stack of this function */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_3, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.fixup_map1 = { 12, 22 },
		.result = ACCEPT,
	},
	{
		"calls: two jumps that receive map_value via arg=ptr_stack_of_jumper. test3",
		.insns = {
			/* main prog */
			/* pass fp-16, fp-8 into a function */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -16),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_1, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_2),
			/* 1st lookup from map */
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -24, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -24),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_8, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			/* write map_value_ptr into stack frame of main prog at fp-8 */
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_8, 1),

			/* 2nd lookup from map */
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -24),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_9, 0),  // 26
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			/* write map_value_ptr into stack frame of main prog at fp-16 */
			BPF_STX_MEM(BPF_DW, BPF_REG_7, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_9, 1),

			/* call 3rd func with fp-8, 0|1, fp-16, 0|1 */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6), // 30
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_7),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_9),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_1, 0, 1), // 34
			BPF_JMP_IMM(BPF_JA, 0, 0, -30),

			/* subprog 2 */
			/* if arg2 == 1 do *arg1 = 0 */
			BPF_JMP_IMM(BPF_JNE, BPF_REG_2, 1, 2),
			/* fetch map_value_ptr from the stack of this function */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),

			/* if arg4 == 1 do *arg3 = 0 */
			BPF_JMP_IMM(BPF_JNE, BPF_REG_4, 1, 2),
			/* fetch map_value_ptr from the stack of this function */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_3, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 2, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, -8),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.fixup_map1 = { 12, 22 },
		.result = REJECT,
		.errstr = "invalid access to map value, value_size=8 off=2 size=8",
	},
	{
		"calls: two calls that receive map_value_ptr_or_null via arg. test1",
		.insns = {
			/* main prog */
			/* pass fp-16, fp-8 into a function */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -16),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_2),
			/* 1st lookup from map */
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			/* write map_value_ptr_or_null into stack frame of main prog at fp-8 */
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_8, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_MOV64_IMM(BPF_REG_8, 1),

			/* 2nd lookup from map */
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			/* write map_value_ptr_or_null into stack frame of main prog at fp-16 */
			BPF_STX_MEM(BPF_DW, BPF_REG_7, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_9, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_MOV64_IMM(BPF_REG_9, 1),

			/* call 3rd func with fp-8, 0|1, fp-16, 0|1 */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_7),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_9),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),

			/* subprog 2 */
			/* if arg2 == 1 do *arg1 = 0 */
			BPF_JMP_IMM(BPF_JNE, BPF_REG_2, 1, 2),
			/* fetch map_value_ptr from the stack of this function */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),

			/* if arg4 == 1 do *arg3 = 0 */
			BPF_JMP_IMM(BPF_JNE, BPF_REG_4, 1, 2),
			/* fetch map_value_ptr from the stack of this function */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_3, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.fixup_map1 = { 12, 22 },
		.result = ACCEPT,
	},
	{
		"calls: two calls that receive map_value_ptr_or_null via arg. test2",
		.insns = {
			/* main prog */
			/* pass fp-16, fp-8 into a function */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -16),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_2),
			/* 1st lookup from map */
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			/* write map_value_ptr_or_null into stack frame of main prog at fp-8 */
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_8, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_MOV64_IMM(BPF_REG_8, 1),

			/* 2nd lookup from map */
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			/* write map_value_ptr_or_null into stack frame of main prog at fp-16 */
			BPF_STX_MEM(BPF_DW, BPF_REG_7, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV64_IMM(BPF_REG_9, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_MOV64_IMM(BPF_REG_9, 1),

			/* call 3rd func with fp-8, 0|1, fp-16, 0|1 */
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_8),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_7),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_9),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),

			/* subprog 2 */
			/* if arg2 == 1 do *arg1 = 0 */
			BPF_JMP_IMM(BPF_JNE, BPF_REG_2, 1, 2),
			/* fetch map_value_ptr from the stack of this function */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_1, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),

			/* if arg4 == 0 do *arg3 = 0 */
			BPF_JMP_IMM(BPF_JNE, BPF_REG_4, 0, 2),
			/* fetch map_value_ptr from the stack of this function */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_3, 0),
			/* write into map value */
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.fixup_map1 = { 12, 22 },
		.result = REJECT,
		.errstr = "R0 invalid mem access 'inv'",
	},
	{
		"calls: pkt_ptr spill into caller stack",
		.insns = {
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, -8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 1),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			/* spill unchecked pkt_ptr into stack of caller */
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 2),
			/* now the pkt range is verified, read pkt_ptr from stack */
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_4, 0),
			/* write 4 bytes into packet */
			BPF_ST_MEM(BPF_W, BPF_REG_2, 0, 0),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.retval = POINTER_VALUE,
	},
	{
		"calls: pkt_ptr spill into caller stack 2",
		.insns = {
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, -8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			/* Marking is still kept, but not in all cases safe. */
			BPF_LDX_MEM(BPF_DW, BPF_REG_4, BPF_REG_10, -8),
			BPF_ST_MEM(BPF_W, BPF_REG_4, 0, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			/* spill unchecked pkt_ptr into stack of caller */
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 2),
			/* now the pkt range is verified, read pkt_ptr from stack */
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_4, 0),
			/* write 4 bytes into packet */
			BPF_ST_MEM(BPF_W, BPF_REG_2, 0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.errstr = "invalid access to packet",
		.result = REJECT,
	},
	{
		"calls: pkt_ptr spill into caller stack 3",
		.insns = {
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, -8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 4),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
			/* Marking is still kept and safe here. */
			BPF_LDX_MEM(BPF_DW, BPF_REG_4, BPF_REG_10, -8),
			BPF_ST_MEM(BPF_W, BPF_REG_4, 0, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			/* spill unchecked pkt_ptr into stack of caller */
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 3),
			BPF_MOV64_IMM(BPF_REG_5, 1),
			/* now the pkt range is verified, read pkt_ptr from stack */
			BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_4, 0),
			/* write 4 bytes into packet */
			BPF_ST_MEM(BPF_W, BPF_REG_2, 0, 0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_5),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"calls: pkt_ptr spill into caller stack 4",
		.insns = {
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, -8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 4),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
			/* Check marking propagated. */
			BPF_LDX_MEM(BPF_DW, BPF_REG_4, BPF_REG_10, -8),
			BPF_ST_MEM(BPF_W, BPF_REG_4, 0, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			/* spill unchecked pkt_ptr into stack of caller */
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 2),
			BPF_MOV64_IMM(BPF_REG_5, 1),
			/* don't read back pkt_ptr from stack here */
			/* write 4 bytes into packet */
			BPF_ST_MEM(BPF_W, BPF_REG_2, 0, 0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_5),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"calls: pkt_ptr spill into caller stack 5",
		.insns = {
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_LDX_MEM(BPF_DW, BPF_REG_4, BPF_REG_10, -8),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_4, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 3),
			/* spill checked pkt_ptr into stack of caller */
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_5, 1),
			/* don't read back pkt_ptr from stack here */
			/* write 4 bytes into packet */
			BPF_ST_MEM(BPF_W, BPF_REG_2, 0, 0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_5),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.errstr = "same insn cannot be used with different",
		.result = REJECT,
	},
	{
		"calls: pkt_ptr spill into caller stack 6",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_LDX_MEM(BPF_DW, BPF_REG_4, BPF_REG_10, -8),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_4, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 3),
			/* spill checked pkt_ptr into stack of caller */
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_5, 1),
			/* don't read back pkt_ptr from stack here */
			/* write 4 bytes into packet */
			BPF_ST_MEM(BPF_W, BPF_REG_2, 0, 0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_5),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.errstr = "R4 invalid mem access",
		.result = REJECT,
	},
	{
		"calls: pkt_ptr spill into caller stack 7",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_LDX_MEM(BPF_DW, BPF_REG_4, BPF_REG_10, -8),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_4, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 3),
			/* spill checked pkt_ptr into stack of caller */
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_5, 1),
			/* don't read back pkt_ptr from stack here */
			/* write 4 bytes into packet */
			BPF_ST_MEM(BPF_W, BPF_REG_2, 0, 0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_5),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.errstr = "R4 invalid mem access",
		.result = REJECT,
	},
	{
		"calls: pkt_ptr spill into caller stack 8",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_0, BPF_REG_3, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_LDX_MEM(BPF_DW, BPF_REG_4, BPF_REG_10, -8),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_4, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 3),
			/* spill checked pkt_ptr into stack of caller */
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_5, 1),
			/* don't read back pkt_ptr from stack here */
			/* write 4 bytes into packet */
			BPF_ST_MEM(BPF_W, BPF_REG_2, 0, 0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_5),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
	},
	{
		"calls: pkt_ptr spill into caller stack 9",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_JMP_REG(BPF_JLE, BPF_REG_0, BPF_REG_3, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_REG(BPF_REG_4, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, -8),
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 3),
			BPF_LDX_MEM(BPF_DW, BPF_REG_4, BPF_REG_10, -8),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_4, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct __sk_buff, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct __sk_buff, data_end)),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_0, 8),
			BPF_MOV64_IMM(BPF_REG_5, 0),
			/* spill unchecked pkt_ptr into stack of caller */
			BPF_STX_MEM(BPF_DW, BPF_REG_4, BPF_REG_2, 0),
			BPF_JMP_REG(BPF_JGT, BPF_REG_0, BPF_REG_3, 2),
			BPF_MOV64_IMM(BPF_REG_5, 1),
			/* don't read back pkt_ptr from stack here */
			/* write 4 bytes into packet */
			BPF_ST_MEM(BPF_W, BPF_REG_2, 0, 0),
			BPF_MOV64_REG(BPF_REG_0, BPF_REG_5),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.errstr = "invalid access to packet",
		.result = REJECT,
	},
	{
		"calls: caller stack init to zero or map_value_or_null",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 4),
			/* fetch map_value_or_null or const_zero from stack */
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_10, -8),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 1),
			/* store into map_value */
			BPF_ST_MEM(BPF_W, BPF_REG_0, 0, 0),
			BPF_EXIT_INSN(),

			/* subprog 1 */
			/* if (ctx == 0) return; */
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0, 8),
			/* else bpf_map_lookup() and *(fp - 8) = r0 */
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_2),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			/* write map_value_ptr_or_null into stack frame of main prog at fp-8 */
			BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 13 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"calls: stack init to zero and pruning",
		.insns = {
			/* first make allocated_stack 16 byte */
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -16, 0),
			/* now fork the execution such that the false branch
			 * of JGT insn will be verified second and it skisp zero
			 * init of fp-8 stack slot. If stack liveness marking
			 * is missing live_read marks from call map_lookup
			 * processing then pruning will incorrectly assume
			 * that fp-8 stack slot was unused in the fall-through
			 * branch and will accept the program incorrectly
			 */
			BPF_JMP_IMM(BPF_JGT, BPF_REG_1, 2, 2),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 6 },
		.errstr = "invalid indirect read from stack off -8+0 size 8",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"calls: two calls returning different map pointers for lookup (hash, array)",
		.insns = {
			/* main prog */
			BPF_JMP_IMM(BPF_JNE, BPF_REG_1, 0, 2),
			BPF_CALL_REL(11),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_CALL_REL(12),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0,
				   offsetof(struct test_val, foo)),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			/* subprog 1 */
			BPF_LD_MAP_FD(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			/* subprog 2 */
			BPF_LD_MAP_FD(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.fixup_map2 = { 13 },
		.fixup_map4 = { 16 },
		.result = ACCEPT,
		.retval = 1,
	},
	{
		"calls: two calls returning different map pointers for lookup (hash, map in map)",
		.insns = {
			/* main prog */
			BPF_JMP_IMM(BPF_JNE, BPF_REG_1, 0, 2),
			BPF_CALL_REL(11),
			BPF_JMP_IMM(BPF_JA, 0, 0, 1),
			BPF_CALL_REL(12),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
			BPF_ST_MEM(BPF_DW, BPF_REG_0, 0,
				   offsetof(struct test_val, foo)),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			/* subprog 1 */
			BPF_LD_MAP_FD(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
			/* subprog 2 */
			BPF_LD_MAP_FD(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.fixup_map_in_map = { 16 },
		.fixup_map4 = { 13 },
		.result = REJECT,
		.errstr = "R0 invalid mem access 'map_ptr'",
	},
	{
		"cond: two branches returning different map pointers for lookup (tail, tail)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_6, 0, 3),
			BPF_LD_MAP_FD(BPF_REG_2, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			BPF_LD_MAP_FD(BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_3, 7),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_tail_call),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.fixup_prog1 = { 5 },
		.fixup_prog2 = { 2 },
		.result_unpriv = REJECT,
		.errstr_unpriv = "tail_call abusing map_ptr",
		.result = ACCEPT,
		.retval = 42,
	},
	{
		"cond: two branches returning same map pointers for lookup (tail, tail)",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
				    offsetof(struct __sk_buff, mark)),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_6, 0, 3),
			BPF_LD_MAP_FD(BPF_REG_2, 0),
			BPF_JMP_IMM(BPF_JA, 0, 0, 2),
			BPF_LD_MAP_FD(BPF_REG_2, 0),
			BPF_MOV64_IMM(BPF_REG_3, 7),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_tail_call),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.fixup_prog2 = { 2, 5 },
		.result_unpriv = ACCEPT,
		.result = ACCEPT,
		.retval = 42,
	},
	{
		"search pruning: all branches should be verified (nop operation)",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 11),
			BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_0, 0),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_3, 0xbeef, 2),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_JMP_A(1),
			BPF_MOV64_IMM(BPF_REG_4, 1),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_4, -16),
			BPF_EMIT_CALL(BPF_FUNC_ktime_get_ns),
			BPF_LDX_MEM(BPF_DW, BPF_REG_5, BPF_REG_10, -16),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_5, 0, 2),
			BPF_MOV64_IMM(BPF_REG_6, 0),
			BPF_ST_MEM(BPF_DW, BPF_REG_6, 0, 0xdead),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "R6 invalid mem access 'inv'",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"search pruning: all branches should be verified (invalid stack access)",
		.insns = {
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_ST_MEM(BPF_DW, BPF_REG_2, 0, 0),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 8),
			BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_0, 0),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_3, 0xbeef, 2),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_4, -16),
			BPF_JMP_A(1),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_4, -24),
			BPF_EMIT_CALL(BPF_FUNC_ktime_get_ns),
			BPF_LDX_MEM(BPF_DW, BPF_REG_5, BPF_REG_10, -16),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.errstr = "invalid read from stack off -16+0 size 8",
		.result = REJECT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"jit: lsh, rsh, arsh by 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_MOV64_IMM(BPF_REG_1, 0xff),
			BPF_ALU64_IMM(BPF_LSH, BPF_REG_1, 1),
			BPF_ALU32_IMM(BPF_LSH, BPF_REG_1, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0x3fc, 1),
			BPF_EXIT_INSN(),
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_1, 1),
			BPF_ALU32_IMM(BPF_RSH, BPF_REG_1, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0xff, 1),
			BPF_EXIT_INSN(),
			BPF_ALU64_IMM(BPF_ARSH, BPF_REG_1, 1),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_1, 0x7f, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 2,
	},
	{
		"jit: mov32 for ldimm64, 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_LD_IMM64(BPF_REG_1, 0xfeffffffffffffffULL),
			BPF_ALU64_IMM(BPF_RSH, BPF_REG_1, 32),
			BPF_LD_IMM64(BPF_REG_2, 0xfeffffffULL),
			BPF_JMP_REG(BPF_JEQ, BPF_REG_1, BPF_REG_2, 1),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 2,
	},
	{
		"jit: mov32 for ldimm64, 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_LD_IMM64(BPF_REG_1, 0x1ffffffffULL),
			BPF_LD_IMM64(BPF_REG_2, 0xffffffffULL),
			BPF_JMP_REG(BPF_JEQ, BPF_REG_1, BPF_REG_2, 1),
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 2,
	},
	{
		"jit: various mul tests",
		.insns = {
			BPF_LD_IMM64(BPF_REG_2, 0xeeff0d413122ULL),
			BPF_LD_IMM64(BPF_REG_0, 0xfefefeULL),
			BPF_LD_IMM64(BPF_REG_1, 0xefefefULL),
			BPF_ALU64_REG(BPF_MUL, BPF_REG_0, BPF_REG_1),
			BPF_JMP_REG(BPF_JEQ, BPF_REG_0, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_LD_IMM64(BPF_REG_3, 0xfefefeULL),
			BPF_ALU64_REG(BPF_MUL, BPF_REG_3, BPF_REG_1),
			BPF_JMP_REG(BPF_JEQ, BPF_REG_3, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV32_REG(BPF_REG_2, BPF_REG_2),
			BPF_LD_IMM64(BPF_REG_0, 0xfefefeULL),
			BPF_ALU32_REG(BPF_MUL, BPF_REG_0, BPF_REG_1),
			BPF_JMP_REG(BPF_JEQ, BPF_REG_0, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_LD_IMM64(BPF_REG_3, 0xfefefeULL),
			BPF_ALU32_REG(BPF_MUL, BPF_REG_3, BPF_REG_1),
			BPF_JMP_REG(BPF_JEQ, BPF_REG_3, BPF_REG_2, 2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_LD_IMM64(BPF_REG_0, 0x952a7bbcULL),
			BPF_LD_IMM64(BPF_REG_1, 0xfefefeULL),
			BPF_LD_IMM64(BPF_REG_2, 0xeeff0d413122ULL),
			BPF_ALU32_REG(BPF_MUL, BPF_REG_2, BPF_REG_1),
			BPF_JMP_REG(BPF_JEQ, BPF_REG_2, BPF_REG_0, 2),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 2),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.retval = 2,
	},
	{
		"xadd/w check unaligned stack",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_STX_XADD(BPF_W, BPF_REG_10, BPF_REG_0, -7),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_10, -8),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "misaligned stack access off",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"xadd/w check unaligned map",
		.insns = {
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 1),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_1, 1),
			BPF_STX_XADD(BPF_W, BPF_REG_0, BPF_REG_1, 3),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_0, 3),
			BPF_EXIT_INSN(),
		},
		.fixup_map1 = { 3 },
		.result = REJECT,
		.errstr = "misaligned value access off",
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	},
	{
		"xadd/w check unaligned pkt",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,
				    offsetof(struct xdp_md, data)),
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,
				    offsetof(struct xdp_md, data_end)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_2),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 8),
			BPF_JMP_REG(BPF_JLT, BPF_REG_1, BPF_REG_3, 2),
			BPF_MOV64_IMM(BPF_REG_0, 99),
			BPF_JMP_IMM(BPF_JA, 0, 0, 6),
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_ST_MEM(BPF_W, BPF_REG_2, 0, 0),
			BPF_ST_MEM(BPF_W, BPF_REG_2, 3, 0),
			BPF_STX_XADD(BPF_W, BPF_REG_2, BPF_REG_0, 1),
			BPF_STX_XADD(BPF_W, BPF_REG_2, BPF_REG_0, 2),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_2, 1),
			BPF_EXIT_INSN(),
		},
		.result = REJECT,
		.errstr = "BPF_XADD stores into R2 packet",
		.prog_type = BPF_PROG_TYPE_XDP,
	},
	{
		"xadd/w check whether src/dst got mangled, 1",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_10),
			BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_STX_XADD(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_STX_XADD(BPF_DW, BPF_REG_10, BPF_REG_0, -8),
			BPF_JMP_REG(BPF_JNE, BPF_REG_6, BPF_REG_0, 3),
			BPF_JMP_REG(BPF_JNE, BPF_REG_7, BPF_REG_10, 2),
			BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_10, -8),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 42),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.retval = 3,
	},
	{
		"xadd/w check whether src/dst got mangled, 2",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_0, 1),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_0),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_10),
			BPF_STX_MEM(BPF_W, BPF_REG_10, BPF_REG_0, -8),
			BPF_STX_XADD(BPF_W, BPF_REG_10, BPF_REG_0, -8),
			BPF_STX_XADD(BPF_W, BPF_REG_10, BPF_REG_0, -8),
			BPF_JMP_REG(BPF_JNE, BPF_REG_6, BPF_REG_0, 3),
			BPF_JMP_REG(BPF_JNE, BPF_REG_7, BPF_REG_10, 2),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_10, -8),
			BPF_EXIT_INSN(),
			BPF_MOV64_IMM(BPF_REG_0, 42),
			BPF_EXIT_INSN(),
		},
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.retval = 3,
	},
	{
		"bpf_get_stack return R0 within range",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
			BPF_LD_MAP_FD(BPF_REG_1, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_map_lookup_elem),
			BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 28),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_0),
			BPF_MOV64_IMM(BPF_REG_9, sizeof(struct test_val)),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_7),
			BPF_MOV64_IMM(BPF_REG_3, sizeof(struct test_val)),
			BPF_MOV64_IMM(BPF_REG_4, 256),
			BPF_EMIT_CALL(BPF_FUNC_get_stack),
			BPF_MOV64_IMM(BPF_REG_1, 0),
			BPF_MOV64_REG(BPF_REG_8, BPF_REG_0),
			BPF_ALU64_IMM(BPF_LSH, BPF_REG_8, 32),
			BPF_ALU64_IMM(BPF_ARSH, BPF_REG_8, 32),
			BPF_JMP_REG(BPF_JSLT, BPF_REG_1, BPF_REG_8, 16),
			BPF_ALU64_REG(BPF_SUB, BPF_REG_9, BPF_REG_8),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_7),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_2, BPF_REG_8),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_9),
			BPF_ALU64_IMM(BPF_LSH, BPF_REG_1, 32),
			BPF_ALU64_IMM(BPF_ARSH, BPF_REG_1, 32),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_2),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_3, BPF_REG_1),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_7),
			BPF_MOV64_IMM(BPF_REG_5, sizeof(struct test_val)),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_5),
			BPF_JMP_REG(BPF_JGE, BPF_REG_3, BPF_REG_1, 4),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_MOV64_REG(BPF_REG_3, BPF_REG_9),
			BPF_MOV64_IMM(BPF_REG_4, 0),
			BPF_EMIT_CALL(BPF_FUNC_get_stack),
			BPF_EXIT_INSN(),
		},
		.fixup_map2 = { 4 },
		.result = ACCEPT,
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
	},
	{
		"ld_abs: invalid op 1",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_LD_ABS(BPF_DW, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = REJECT,
		.errstr = "unknown opcode",
	},
	{
		"ld_abs: invalid op 2",
		.insns = {
			BPF_MOV32_IMM(BPF_REG_0, 256),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_LD_IND(BPF_DW, BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = REJECT,
		.errstr = "unknown opcode",
	},
	{
		"ld_abs: nmap reduced",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_LD_ABS(BPF_H, 12),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0x806, 28),
			BPF_LD_ABS(BPF_H, 12),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0x806, 26),
			BPF_MOV32_IMM(BPF_REG_0, 18),
			BPF_STX_MEM(BPF_W, BPF_REG_10, BPF_REG_0, -64),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_10, -64),
			BPF_LD_IND(BPF_W, BPF_REG_7, 14),
			BPF_STX_MEM(BPF_W, BPF_REG_10, BPF_REG_0, -60),
			BPF_MOV32_IMM(BPF_REG_0, 280971478),
			BPF_STX_MEM(BPF_W, BPF_REG_10, BPF_REG_0, -56),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_10, -56),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_10, -60),
			BPF_ALU32_REG(BPF_SUB, BPF_REG_0, BPF_REG_7),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 15),
			BPF_LD_ABS(BPF_H, 12),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0x806, 13),
			BPF_MOV32_IMM(BPF_REG_0, 22),
			BPF_STX_MEM(BPF_W, BPF_REG_10, BPF_REG_0, -56),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_10, -56),
			BPF_LD_IND(BPF_H, BPF_REG_7, 14),
			BPF_STX_MEM(BPF_W, BPF_REG_10, BPF_REG_0, -52),
			BPF_MOV32_IMM(BPF_REG_0, 17366),
			BPF_STX_MEM(BPF_W, BPF_REG_10, BPF_REG_0, -48),
			BPF_LDX_MEM(BPF_W, BPF_REG_7, BPF_REG_10, -48),
			BPF_LDX_MEM(BPF_W, BPF_REG_0, BPF_REG_10, -52),
			BPF_ALU32_REG(BPF_SUB, BPF_REG_0, BPF_REG_7),
			BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
			BPF_MOV32_IMM(BPF_REG_0, 256),
			BPF_EXIT_INSN(),
			BPF_MOV32_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.data = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x08, 0x06, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0x10, 0xbf, 0x48, 0xd6, 0x43, 0xd6,
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 256,
	},
	{
		"ld_abs: div + abs, test 1",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_1),
			BPF_LD_ABS(BPF_B, 3),
			BPF_ALU64_IMM(BPF_MOV, BPF_REG_2, 2),
			BPF_ALU32_REG(BPF_DIV, BPF_REG_0, BPF_REG_2),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_8, BPF_REG_0),
			BPF_LD_ABS(BPF_B, 4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_8, BPF_REG_0),
			BPF_LD_IND(BPF_B, BPF_REG_8, -70),
			BPF_EXIT_INSN(),
		},
		.data = {
			10, 20, 30, 40, 50,
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 10,
	},
	{
		"ld_abs: div + abs, test 2",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_1),
			BPF_LD_ABS(BPF_B, 3),
			BPF_ALU64_IMM(BPF_MOV, BPF_REG_2, 2),
			BPF_ALU32_REG(BPF_DIV, BPF_REG_0, BPF_REG_2),
			BPF_ALU64_REG(BPF_MOV, BPF_REG_8, BPF_REG_0),
			BPF_LD_ABS(BPF_B, 128),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_8, BPF_REG_0),
			BPF_LD_IND(BPF_B, BPF_REG_8, -70),
			BPF_EXIT_INSN(),
		},
		.data = {
			10, 20, 30, 40, 50,
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 0,
	},
	{
		"ld_abs: div + abs, test 3",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_1),
			BPF_ALU64_IMM(BPF_MOV, BPF_REG_7, 0),
			BPF_LD_ABS(BPF_B, 3),
			BPF_ALU32_REG(BPF_DIV, BPF_REG_0, BPF_REG_7),
			BPF_EXIT_INSN(),
		},
		.data = {
			10, 20, 30, 40, 50,
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 0,
	},
	{
		"ld_abs: div + abs, test 4",
		.insns = {
			BPF_ALU64_REG(BPF_MOV, BPF_REG_6, BPF_REG_1),
			BPF_ALU64_IMM(BPF_MOV, BPF_REG_7, 0),
			BPF_LD_ABS(BPF_B, 256),
			BPF_ALU32_REG(BPF_DIV, BPF_REG_0, BPF_REG_7),
			BPF_EXIT_INSN(),
		},
		.data = {
			10, 20, 30, 40, 50,
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 0,
	},
	{
		"ld_abs: vlan + abs, test 1",
		.insns = { },
		.data = {
			0x34,
		},
		.fill_helper = bpf_fill_ld_abs_vlan_push_pop,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 0xbef,
	},
	{
		"ld_abs: vlan + abs, test 2",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_LD_ABS(BPF_B, 0),
			BPF_LD_ABS(BPF_H, 0),
			BPF_LD_ABS(BPF_W, 0),
			BPF_MOV64_REG(BPF_REG_7, BPF_REG_6),
			BPF_MOV64_IMM(BPF_REG_6, 0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_7),
			BPF_MOV64_IMM(BPF_REG_2, 1),
			BPF_MOV64_IMM(BPF_REG_3, 2),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_skb_vlan_push),
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_7),
			BPF_LD_ABS(BPF_B, 0),
			BPF_LD_ABS(BPF_H, 0),
			BPF_LD_ABS(BPF_W, 0),
			BPF_MOV64_IMM(BPF_REG_0, 42),
			BPF_EXIT_INSN(),
		},
		.data = {
			0x34,
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 42,
	},
	{
		"ld_abs: jump around ld_abs",
		.insns = { },
		.data = {
			10, 11,
		},
		.fill_helper = bpf_fill_jump_around_ld_abs,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 10,
	},
	{
		"ld_dw: xor semi-random 64 bit imms, test 1",
		.insns = { },
		.data = { },
		.fill_helper = bpf_fill_rand_ld_dw,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 4090,
	},
	{
		"ld_dw: xor semi-random 64 bit imms, test 2",
		.insns = { },
		.data = { },
		.fill_helper = bpf_fill_rand_ld_dw,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 2047,
	},
	{
		"ld_dw: xor semi-random 64 bit imms, test 3",
		.insns = { },
		.data = { },
		.fill_helper = bpf_fill_rand_ld_dw,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 511,
	},
	{
		"ld_dw: xor semi-random 64 bit imms, test 4",
		.insns = { },
		.data = { },
		.fill_helper = bpf_fill_rand_ld_dw,
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
		.retval = 5,
	},
	{
		"pass unmodified ctx pointer to helper",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_csum_update),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
	},
	{
		"pass modified ctx pointer to helper, 1",
		.insns = {
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -612),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_csum_update),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = REJECT,
		.errstr = "dereference of modified ctx ptr",
	},
	{
		"pass modified ctx pointer to helper, 2",
		.insns = {
			BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, -612),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_get_socket_cookie),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.result_unpriv = REJECT,
		.result = REJECT,
		.errstr_unpriv = "dereference of modified ctx ptr",
		.errstr = "dereference of modified ctx ptr",
	},
	{
		"pass modified ctx pointer to helper, 3",
		.insns = {
			BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1, 0),
			BPF_ALU64_IMM(BPF_AND, BPF_REG_3, 4),
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_3),
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
				     BPF_FUNC_csum_update),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = REJECT,
		.errstr = "variable ctx access var_off=(0x0; 0x4)",
	},
	{
		"mov64 src == dst",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_2, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_2),
			// Check bounds are OK
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
	},
	{
		"mov64 src != dst",
		.insns = {
			BPF_MOV64_IMM(BPF_REG_3, 0),
			BPF_MOV64_REG(BPF_REG_2, BPF_REG_3),
			// Check bounds are OK
			BPF_ALU64_REG(BPF_ADD, BPF_REG_1, BPF_REG_2),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.result = ACCEPT,
	},
	{
		"calls: ctx read at start of subprog",
		.insns = {
			BPF_MOV64_REG(BPF_REG_6, BPF_REG_1),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 5),
			BPF_JMP_REG(BPF_JSGT, BPF_REG_0, BPF_REG_0, 0),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_6),
			BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 1, 0, 2),
			BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
			BPF_EXIT_INSN(),
			BPF_LDX_MEM(BPF_B, BPF_REG_9, BPF_REG_1, 0),
			BPF_MOV64_IMM(BPF_REG_0, 0),
			BPF_EXIT_INSN(),
		},
		.prog_type = BPF_PROG_TYPE_SOCKET_FILTER,
		.errstr_unpriv = "function calls to other bpf functions are allowed for root only",
		.result_unpriv = REJECT,
		.result = ACCEPT,
	},
=======
#define MAX_JMP_SEQ 8192

/* test the sequence of 8k jumps */
static void bpf_fill_scale1(struct bpf_test *self)
{
	struct bpf_insn *insn = self->fill_insns;
	int i = 0, k = 0;

	insn[i++] = BPF_MOV64_REG(BPF_REG_6, BPF_REG_1);
	/* test to check that the long sequence of jumps is acceptable */
	while (k++ < MAX_JMP_SEQ) {
		insn[i++] = BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
					 BPF_FUNC_get_prandom_u32);
		insn[i++] = BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, bpf_semi_rand_get(), 2);
		insn[i++] = BPF_MOV64_REG(BPF_REG_1, BPF_REG_10);
		insn[i++] = BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_6,
					-8 * (k % 64 + 1));
	}
	/* is_state_visited() doesn't allocate state for pruning for every jump.
	 * Hence multiply jmps by 4 to accommodate that heuristic
	 */
	while (i < MAX_TEST_INSNS - MAX_JMP_SEQ * 4)
		insn[i++] = BPF_ALU64_IMM(BPF_MOV, BPF_REG_0, 42);
	insn[i] = BPF_EXIT_INSN();
	self->prog_len = i + 1;
	self->retval = 42;
}

/* test the sequence of 8k jumps in inner most function (function depth 8)*/
static void bpf_fill_scale2(struct bpf_test *self)
{
	struct bpf_insn *insn = self->fill_insns;
	int i = 0, k = 0;

#define FUNC_NEST 7
	for (k = 0; k < FUNC_NEST; k++) {
		insn[i++] = BPF_CALL_REL(1);
		insn[i++] = BPF_EXIT_INSN();
	}
	insn[i++] = BPF_MOV64_REG(BPF_REG_6, BPF_REG_1);
	/* test to check that the long sequence of jumps is acceptable */
	k = 0;
	while (k++ < MAX_JMP_SEQ) {
		insn[i++] = BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
					 BPF_FUNC_get_prandom_u32);
		insn[i++] = BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, bpf_semi_rand_get(), 2);
		insn[i++] = BPF_MOV64_REG(BPF_REG_1, BPF_REG_10);
		insn[i++] = BPF_STX_MEM(BPF_DW, BPF_REG_1, BPF_REG_6,
					-8 * (k % (64 - 4 * FUNC_NEST) + 1));
	}
	while (i < MAX_TEST_INSNS - MAX_JMP_SEQ * 4)
		insn[i++] = BPF_ALU64_IMM(BPF_MOV, BPF_REG_0, 42);
	insn[i] = BPF_EXIT_INSN();
	self->prog_len = i + 1;
	self->retval = 42;
}

static void bpf_fill_scale(struct bpf_test *self)
{
	switch (self->retval) {
	case 1:
		return bpf_fill_scale1(self);
	case 2:
		return bpf_fill_scale2(self);
	default:
		self->prog_len = 0;
		break;
	}
}

static int bpf_fill_torturous_jumps_insn_1(struct bpf_insn *insn)
{
	unsigned int len = 259, hlen = 128;
	int i;

	insn[0] = BPF_EMIT_CALL(BPF_FUNC_get_prandom_u32);
	for (i = 1; i <= hlen; i++) {
		insn[i]        = BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, i, hlen);
		insn[i + hlen] = BPF_JMP_A(hlen - i);
	}
	insn[len - 2] = BPF_MOV64_IMM(BPF_REG_0, 1);
	insn[len - 1] = BPF_EXIT_INSN();

	return len;
}

static int bpf_fill_torturous_jumps_insn_2(struct bpf_insn *insn)
{
	unsigned int len = 4100, jmp_off = 2048;
	int i, j;

	insn[0] = BPF_EMIT_CALL(BPF_FUNC_get_prandom_u32);
	for (i = 1; i <= jmp_off; i++) {
		insn[i] = BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, i, jmp_off);
	}
	insn[i++] = BPF_JMP_A(jmp_off);
	for (; i <= jmp_off * 2 + 1; i+=16) {
		for (j = 0; j < 16; j++) {
			insn[i + j] = BPF_JMP_A(16 - j - 1);
		}
	}

	insn[len - 2] = BPF_MOV64_IMM(BPF_REG_0, 2);
	insn[len - 1] = BPF_EXIT_INSN();

	return len;
}

static void bpf_fill_torturous_jumps(struct bpf_test *self)
{
	struct bpf_insn *insn = self->fill_insns;
	int i = 0;

	switch (self->retval) {
	case 1:
		self->prog_len = bpf_fill_torturous_jumps_insn_1(insn);
		return;
	case 2:
		self->prog_len = bpf_fill_torturous_jumps_insn_2(insn);
		return;
	case 3:
		/* main */
		insn[i++] = BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 4);
		insn[i++] = BPF_RAW_INSN(BPF_JMP|BPF_CALL, 0, 1, 0, 262);
		insn[i++] = BPF_ST_MEM(BPF_B, BPF_REG_10, -32, 0);
		insn[i++] = BPF_MOV64_IMM(BPF_REG_0, 3);
		insn[i++] = BPF_EXIT_INSN();

		/* subprog 1 */
		i += bpf_fill_torturous_jumps_insn_1(insn + i);

		/* subprog 2 */
		i += bpf_fill_torturous_jumps_insn_2(insn + i);

		self->prog_len = i;
		return;
	default:
		self->prog_len = 0;
		break;
	}
}

/* BPF_SK_LOOKUP contains 13 instructions, if you need to fix up maps */
#define BPF_SK_LOOKUP(func)						\
	/* struct bpf_sock_tuple tuple = {} */				\
	BPF_MOV64_IMM(BPF_REG_2, 0),					\
	BPF_STX_MEM(BPF_W, BPF_REG_10, BPF_REG_2, -8),			\
	BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_2, -16),		\
	BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_2, -24),		\
	BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_2, -32),		\
	BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_2, -40),		\
	BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_2, -48),		\
	/* sk = func(ctx, &tuple, sizeof tuple, 0, 0) */		\
	BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),				\
	BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -48),				\
	BPF_MOV64_IMM(BPF_REG_3, sizeof(struct bpf_sock_tuple)),	\
	BPF_MOV64_IMM(BPF_REG_4, 0),					\
	BPF_MOV64_IMM(BPF_REG_5, 0),					\
	BPF_EMIT_CALL(BPF_FUNC_ ## func)

/* BPF_DIRECT_PKT_R2 contains 7 instructions, it initializes default return
 * value into 0 and does necessary preparation for direct packet access
 * through r2. The allowed access range is 8 bytes.
 */
#define BPF_DIRECT_PKT_R2						\
	BPF_MOV64_IMM(BPF_REG_0, 0),					\
	BPF_LDX_MEM(BPF_W, BPF_REG_2, BPF_REG_1,			\
		    offsetof(struct __sk_buff, data)),			\
	BPF_LDX_MEM(BPF_W, BPF_REG_3, BPF_REG_1,			\
		    offsetof(struct __sk_buff, data_end)),		\
	BPF_MOV64_REG(BPF_REG_4, BPF_REG_2),				\
	BPF_ALU64_IMM(BPF_ADD, BPF_REG_4, 8),				\
	BPF_JMP_REG(BPF_JLE, BPF_REG_4, BPF_REG_3, 1),			\
	BPF_EXIT_INSN()

/* BPF_RAND_UEXT_R7 contains 4 instructions, it initializes R7 into a random
 * positive u32, and zero-extend it into 64-bit.
 */
#define BPF_RAND_UEXT_R7						\
	BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,			\
		     BPF_FUNC_get_prandom_u32),				\
	BPF_MOV64_REG(BPF_REG_7, BPF_REG_0),				\
	BPF_ALU64_IMM(BPF_LSH, BPF_REG_7, 33),				\
	BPF_ALU64_IMM(BPF_RSH, BPF_REG_7, 33)

/* BPF_RAND_SEXT_R7 contains 5 instructions, it initializes R7 into a random
 * negative u32, and sign-extend it into 64-bit.
 */
#define BPF_RAND_SEXT_R7						\
	BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,			\
		     BPF_FUNC_get_prandom_u32),				\
	BPF_MOV64_REG(BPF_REG_7, BPF_REG_0),				\
	BPF_ALU64_IMM(BPF_OR, BPF_REG_7, 0x80000000),			\
	BPF_ALU64_IMM(BPF_LSH, BPF_REG_7, 32),				\
	BPF_ALU64_IMM(BPF_ARSH, BPF_REG_7, 32)

static struct bpf_test tests[] = {
#define FILL_ARRAY
#include <verifier/tests.h>
#undef FILL_ARRAY
>>>>>>> upstream/android-13
};

static int probe_filter_length(const struct bpf_insn *fp)
{
	int len;

	for (len = MAX_INSNS - 1; len > 0; --len)
		if (fp[len].code != 0 || fp[len].imm != 0)
			break;
	return len + 1;
}

<<<<<<< HEAD
static int create_map(uint32_t type, uint32_t size_key,
		      uint32_t size_value, uint32_t max_elem)
=======
static bool skip_unsupported_map(enum bpf_map_type map_type)
{
	if (!bpf_probe_map_type(map_type, 0)) {
		printf("SKIP (unsupported map type %d)\n", map_type);
		skips++;
		return true;
	}
	return false;
}

static int __create_map(uint32_t type, uint32_t size_key,
			uint32_t size_value, uint32_t max_elem,
			uint32_t extra_flags)
>>>>>>> upstream/android-13
{
	int fd;

	fd = bpf_create_map(type, size_key, size_value, max_elem,
<<<<<<< HEAD
			    type == BPF_MAP_TYPE_HASH ? BPF_F_NO_PREALLOC : 0);
	if (fd < 0)
		printf("Failed to create hash map '%s'!\n", strerror(errno));
=======
			    (type == BPF_MAP_TYPE_HASH ?
			     BPF_F_NO_PREALLOC : 0) | extra_flags);
	if (fd < 0) {
		if (skip_unsupported_map(type))
			return -1;
		printf("Failed to create hash map '%s'!\n", strerror(errno));
	}
>>>>>>> upstream/android-13

	return fd;
}

<<<<<<< HEAD
static int create_prog_dummy1(void)
{
	struct bpf_insn prog[] = {
		BPF_MOV64_IMM(BPF_REG_0, 42),
		BPF_EXIT_INSN(),
	};

	return bpf_load_program(BPF_PROG_TYPE_SOCKET_FILTER, prog,
				ARRAY_SIZE(prog), "GPL", 0, NULL, 0);
}

static int create_prog_dummy2(int mfd, int idx)
=======
static int create_map(uint32_t type, uint32_t size_key,
		      uint32_t size_value, uint32_t max_elem)
{
	return __create_map(type, size_key, size_value, max_elem, 0);
}

static void update_map(int fd, int index)
{
	struct test_val value = {
		.index = (6 + 1) * sizeof(int),
		.foo[6] = 0xabcdef12,
	};

	assert(!bpf_map_update_elem(fd, &index, &value, 0));
}

static int create_prog_dummy_simple(enum bpf_prog_type prog_type, int ret)
{
	struct bpf_insn prog[] = {
		BPF_MOV64_IMM(BPF_REG_0, ret),
		BPF_EXIT_INSN(),
	};

	return bpf_load_program(prog_type, prog,
				ARRAY_SIZE(prog), "GPL", 0, NULL, 0);
}

static int create_prog_dummy_loop(enum bpf_prog_type prog_type, int mfd,
				  int idx, int ret)
>>>>>>> upstream/android-13
{
	struct bpf_insn prog[] = {
		BPF_MOV64_IMM(BPF_REG_3, idx),
		BPF_LD_MAP_FD(BPF_REG_2, mfd),
		BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0,
			     BPF_FUNC_tail_call),
<<<<<<< HEAD
		BPF_MOV64_IMM(BPF_REG_0, 41),
		BPF_EXIT_INSN(),
	};

	return bpf_load_program(BPF_PROG_TYPE_SOCKET_FILTER, prog,
				ARRAY_SIZE(prog), "GPL", 0, NULL, 0);
}

static int create_prog_array(uint32_t max_elem, int p1key)
{
	int p2key = 1;
	int mfd, p1fd, p2fd;
=======
		BPF_MOV64_IMM(BPF_REG_0, ret),
		BPF_EXIT_INSN(),
	};

	return bpf_load_program(prog_type, prog,
				ARRAY_SIZE(prog), "GPL", 0, NULL, 0);
}

static int create_prog_array(enum bpf_prog_type prog_type, uint32_t max_elem,
			     int p1key, int p2key, int p3key)
{
	int mfd, p1fd, p2fd, p3fd;
>>>>>>> upstream/android-13

	mfd = bpf_create_map(BPF_MAP_TYPE_PROG_ARRAY, sizeof(int),
			     sizeof(int), max_elem, 0);
	if (mfd < 0) {
<<<<<<< HEAD
=======
		if (skip_unsupported_map(BPF_MAP_TYPE_PROG_ARRAY))
			return -1;
>>>>>>> upstream/android-13
		printf("Failed to create prog array '%s'!\n", strerror(errno));
		return -1;
	}

<<<<<<< HEAD
	p1fd = create_prog_dummy1();
	p2fd = create_prog_dummy2(mfd, p2key);
	if (p1fd < 0 || p2fd < 0)
		goto out;
	if (bpf_map_update_elem(mfd, &p1key, &p1fd, BPF_ANY) < 0)
		goto out;
	if (bpf_map_update_elem(mfd, &p2key, &p2fd, BPF_ANY) < 0)
		goto out;
	close(p2fd);
	close(p1fd);

	return mfd;
out:
	close(p2fd);
	close(p1fd);
	close(mfd);
	return -1;
=======
	p1fd = create_prog_dummy_simple(prog_type, 42);
	p2fd = create_prog_dummy_loop(prog_type, mfd, p2key, 41);
	p3fd = create_prog_dummy_simple(prog_type, 24);
	if (p1fd < 0 || p2fd < 0 || p3fd < 0)
		goto err;
	if (bpf_map_update_elem(mfd, &p1key, &p1fd, BPF_ANY) < 0)
		goto err;
	if (bpf_map_update_elem(mfd, &p2key, &p2fd, BPF_ANY) < 0)
		goto err;
	if (bpf_map_update_elem(mfd, &p3key, &p3fd, BPF_ANY) < 0) {
err:
		close(mfd);
		mfd = -1;
	}
	close(p3fd);
	close(p2fd);
	close(p1fd);
	return mfd;
>>>>>>> upstream/android-13
}

static int create_map_in_map(void)
{
	int inner_map_fd, outer_map_fd;

	inner_map_fd = bpf_create_map(BPF_MAP_TYPE_ARRAY, sizeof(int),
				      sizeof(int), 1, 0);
	if (inner_map_fd < 0) {
<<<<<<< HEAD
=======
		if (skip_unsupported_map(BPF_MAP_TYPE_ARRAY))
			return -1;
>>>>>>> upstream/android-13
		printf("Failed to create array '%s'!\n", strerror(errno));
		return inner_map_fd;
	}

	outer_map_fd = bpf_create_map_in_map(BPF_MAP_TYPE_ARRAY_OF_MAPS, NULL,
					     sizeof(int), inner_map_fd, 1, 0);
<<<<<<< HEAD
	if (outer_map_fd < 0)
		printf("Failed to create array of maps '%s'!\n",
		       strerror(errno));
=======
	if (outer_map_fd < 0) {
		if (skip_unsupported_map(BPF_MAP_TYPE_ARRAY_OF_MAPS))
			return -1;
		printf("Failed to create array of maps '%s'!\n",
		       strerror(errno));
	}
>>>>>>> upstream/android-13

	close(inner_map_fd);

	return outer_map_fd;
}

<<<<<<< HEAD
static int create_cgroup_storage(void)
{
	int fd;

	fd = bpf_create_map(BPF_MAP_TYPE_CGROUP_STORAGE,
			    sizeof(struct bpf_cgroup_storage_key),
			    TEST_DATA_LEN, 0, 0);
	if (fd < 0)
		printf("Failed to create array '%s'!\n", strerror(errno));
=======
static int create_cgroup_storage(bool percpu)
{
	enum bpf_map_type type = percpu ? BPF_MAP_TYPE_PERCPU_CGROUP_STORAGE :
		BPF_MAP_TYPE_CGROUP_STORAGE;
	int fd;

	fd = bpf_create_map(type, sizeof(struct bpf_cgroup_storage_key),
			    TEST_DATA_LEN, 0, 0);
	if (fd < 0) {
		if (skip_unsupported_map(type))
			return -1;
		printf("Failed to create cgroup storage '%s'!\n",
		       strerror(errno));
	}
>>>>>>> upstream/android-13

	return fd;
}

<<<<<<< HEAD
static char bpf_vlog[UINT_MAX >> 8];

static void do_test_fixup(struct bpf_test *test, struct bpf_insn *prog,
			  int *map_fds)
{
	int *fixup_map1 = test->fixup_map1;
	int *fixup_map2 = test->fixup_map2;
	int *fixup_map3 = test->fixup_map3;
	int *fixup_map4 = test->fixup_map4;
=======
/* struct bpf_spin_lock {
 *   int val;
 * };
 * struct val {
 *   int cnt;
 *   struct bpf_spin_lock l;
 * };
 */
static const char btf_str_sec[] = "\0bpf_spin_lock\0val\0cnt\0l";
static __u32 btf_raw_types[] = {
	/* int */
	BTF_TYPE_INT_ENC(0, BTF_INT_SIGNED, 0, 32, 4),  /* [1] */
	/* struct bpf_spin_lock */                      /* [2] */
	BTF_TYPE_ENC(1, BTF_INFO_ENC(BTF_KIND_STRUCT, 0, 1), 4),
	BTF_MEMBER_ENC(15, 1, 0), /* int val; */
	/* struct val */                                /* [3] */
	BTF_TYPE_ENC(15, BTF_INFO_ENC(BTF_KIND_STRUCT, 0, 2), 8),
	BTF_MEMBER_ENC(19, 1, 0), /* int cnt; */
	BTF_MEMBER_ENC(23, 2, 32),/* struct bpf_spin_lock l; */
};

static int load_btf(void)
{
	struct btf_header hdr = {
		.magic = BTF_MAGIC,
		.version = BTF_VERSION,
		.hdr_len = sizeof(struct btf_header),
		.type_len = sizeof(btf_raw_types),
		.str_off = sizeof(btf_raw_types),
		.str_len = sizeof(btf_str_sec),
	};
	void *ptr, *raw_btf;
	int btf_fd;

	ptr = raw_btf = malloc(sizeof(hdr) + sizeof(btf_raw_types) +
			       sizeof(btf_str_sec));

	memcpy(ptr, &hdr, sizeof(hdr));
	ptr += sizeof(hdr);
	memcpy(ptr, btf_raw_types, hdr.type_len);
	ptr += hdr.type_len;
	memcpy(ptr, btf_str_sec, hdr.str_len);
	ptr += hdr.str_len;

	btf_fd = bpf_load_btf(raw_btf, ptr - raw_btf, 0, 0, 0);
	free(raw_btf);
	if (btf_fd < 0)
		return -1;
	return btf_fd;
}

static int create_map_spin_lock(void)
{
	struct bpf_create_map_attr attr = {
		.name = "test_map",
		.map_type = BPF_MAP_TYPE_ARRAY,
		.key_size = 4,
		.value_size = 8,
		.max_entries = 1,
		.btf_key_type_id = 1,
		.btf_value_type_id = 3,
	};
	int fd, btf_fd;

	btf_fd = load_btf();
	if (btf_fd < 0)
		return -1;
	attr.btf_fd = btf_fd;
	fd = bpf_create_map_xattr(&attr);
	if (fd < 0)
		printf("Failed to create map with spin_lock\n");
	return fd;
}

static int create_sk_storage_map(void)
{
	struct bpf_create_map_attr attr = {
		.name = "test_map",
		.map_type = BPF_MAP_TYPE_SK_STORAGE,
		.key_size = 4,
		.value_size = 8,
		.max_entries = 0,
		.map_flags = BPF_F_NO_PREALLOC,
		.btf_key_type_id = 1,
		.btf_value_type_id = 3,
	};
	int fd, btf_fd;

	btf_fd = load_btf();
	if (btf_fd < 0)
		return -1;
	attr.btf_fd = btf_fd;
	fd = bpf_create_map_xattr(&attr);
	close(attr.btf_fd);
	if (fd < 0)
		printf("Failed to create sk_storage_map\n");
	return fd;
}

static char bpf_vlog[UINT_MAX >> 8];

static void do_test_fixup(struct bpf_test *test, enum bpf_prog_type prog_type,
			  struct bpf_insn *prog, int *map_fds)
{
	int *fixup_map_hash_8b = test->fixup_map_hash_8b;
	int *fixup_map_hash_48b = test->fixup_map_hash_48b;
	int *fixup_map_hash_16b = test->fixup_map_hash_16b;
	int *fixup_map_array_48b = test->fixup_map_array_48b;
	int *fixup_map_sockmap = test->fixup_map_sockmap;
	int *fixup_map_sockhash = test->fixup_map_sockhash;
	int *fixup_map_xskmap = test->fixup_map_xskmap;
	int *fixup_map_stacktrace = test->fixup_map_stacktrace;
>>>>>>> upstream/android-13
	int *fixup_prog1 = test->fixup_prog1;
	int *fixup_prog2 = test->fixup_prog2;
	int *fixup_map_in_map = test->fixup_map_in_map;
	int *fixup_cgroup_storage = test->fixup_cgroup_storage;
<<<<<<< HEAD

	if (test->fill_helper)
		test->fill_helper(test);
=======
	int *fixup_percpu_cgroup_storage = test->fixup_percpu_cgroup_storage;
	int *fixup_map_spin_lock = test->fixup_map_spin_lock;
	int *fixup_map_array_ro = test->fixup_map_array_ro;
	int *fixup_map_array_wo = test->fixup_map_array_wo;
	int *fixup_map_array_small = test->fixup_map_array_small;
	int *fixup_sk_storage_map = test->fixup_sk_storage_map;
	int *fixup_map_event_output = test->fixup_map_event_output;
	int *fixup_map_reuseport_array = test->fixup_map_reuseport_array;
	int *fixup_map_ringbuf = test->fixup_map_ringbuf;

	if (test->fill_helper) {
		test->fill_insns = calloc(MAX_TEST_INSNS, sizeof(struct bpf_insn));
		test->fill_helper(test);
	}
>>>>>>> upstream/android-13

	/* Allocating HTs with 1 elem is fine here, since we only test
	 * for verifier and not do a runtime lookup, so the only thing
	 * that really matters is value size in this case.
	 */
<<<<<<< HEAD
	if (*fixup_map1) {
		map_fds[0] = create_map(BPF_MAP_TYPE_HASH, sizeof(long long),
					sizeof(long long), 1);
		do {
			prog[*fixup_map1].imm = map_fds[0];
			fixup_map1++;
		} while (*fixup_map1);
	}

	if (*fixup_map2) {
		map_fds[1] = create_map(BPF_MAP_TYPE_HASH, sizeof(long long),
					sizeof(struct test_val), 1);
		do {
			prog[*fixup_map2].imm = map_fds[1];
			fixup_map2++;
		} while (*fixup_map2);
	}

	if (*fixup_map3) {
		map_fds[2] = create_map(BPF_MAP_TYPE_HASH, sizeof(long long),
					sizeof(struct other_val), 1);
		do {
			prog[*fixup_map3].imm = map_fds[2];
			fixup_map3++;
		} while (*fixup_map3);
	}

	if (*fixup_map4) {
		map_fds[3] = create_map(BPF_MAP_TYPE_ARRAY, sizeof(int),
					sizeof(struct test_val), 1);
		do {
			prog[*fixup_map4].imm = map_fds[3];
			fixup_map4++;
		} while (*fixup_map4);
	}

	if (*fixup_prog1) {
		map_fds[4] = create_prog_array(4, 0);
=======
	if (*fixup_map_hash_8b) {
		map_fds[0] = create_map(BPF_MAP_TYPE_HASH, sizeof(long long),
					sizeof(long long), 1);
		do {
			prog[*fixup_map_hash_8b].imm = map_fds[0];
			fixup_map_hash_8b++;
		} while (*fixup_map_hash_8b);
	}

	if (*fixup_map_hash_48b) {
		map_fds[1] = create_map(BPF_MAP_TYPE_HASH, sizeof(long long),
					sizeof(struct test_val), 1);
		do {
			prog[*fixup_map_hash_48b].imm = map_fds[1];
			fixup_map_hash_48b++;
		} while (*fixup_map_hash_48b);
	}

	if (*fixup_map_hash_16b) {
		map_fds[2] = create_map(BPF_MAP_TYPE_HASH, sizeof(long long),
					sizeof(struct other_val), 1);
		do {
			prog[*fixup_map_hash_16b].imm = map_fds[2];
			fixup_map_hash_16b++;
		} while (*fixup_map_hash_16b);
	}

	if (*fixup_map_array_48b) {
		map_fds[3] = create_map(BPF_MAP_TYPE_ARRAY, sizeof(int),
					sizeof(struct test_val), 1);
		update_map(map_fds[3], 0);
		do {
			prog[*fixup_map_array_48b].imm = map_fds[3];
			fixup_map_array_48b++;
		} while (*fixup_map_array_48b);
	}

	if (*fixup_prog1) {
		map_fds[4] = create_prog_array(prog_type, 4, 0, 1, 2);
>>>>>>> upstream/android-13
		do {
			prog[*fixup_prog1].imm = map_fds[4];
			fixup_prog1++;
		} while (*fixup_prog1);
	}

	if (*fixup_prog2) {
<<<<<<< HEAD
		map_fds[5] = create_prog_array(8, 7);
=======
		map_fds[5] = create_prog_array(prog_type, 8, 7, 1, 2);
>>>>>>> upstream/android-13
		do {
			prog[*fixup_prog2].imm = map_fds[5];
			fixup_prog2++;
		} while (*fixup_prog2);
	}

	if (*fixup_map_in_map) {
		map_fds[6] = create_map_in_map();
		do {
			prog[*fixup_map_in_map].imm = map_fds[6];
			fixup_map_in_map++;
		} while (*fixup_map_in_map);
	}

	if (*fixup_cgroup_storage) {
<<<<<<< HEAD
		map_fds[7] = create_cgroup_storage();
=======
		map_fds[7] = create_cgroup_storage(false);
>>>>>>> upstream/android-13
		do {
			prog[*fixup_cgroup_storage].imm = map_fds[7];
			fixup_cgroup_storage++;
		} while (*fixup_cgroup_storage);
	}
<<<<<<< HEAD
=======

	if (*fixup_percpu_cgroup_storage) {
		map_fds[8] = create_cgroup_storage(true);
		do {
			prog[*fixup_percpu_cgroup_storage].imm = map_fds[8];
			fixup_percpu_cgroup_storage++;
		} while (*fixup_percpu_cgroup_storage);
	}
	if (*fixup_map_sockmap) {
		map_fds[9] = create_map(BPF_MAP_TYPE_SOCKMAP, sizeof(int),
					sizeof(int), 1);
		do {
			prog[*fixup_map_sockmap].imm = map_fds[9];
			fixup_map_sockmap++;
		} while (*fixup_map_sockmap);
	}
	if (*fixup_map_sockhash) {
		map_fds[10] = create_map(BPF_MAP_TYPE_SOCKHASH, sizeof(int),
					sizeof(int), 1);
		do {
			prog[*fixup_map_sockhash].imm = map_fds[10];
			fixup_map_sockhash++;
		} while (*fixup_map_sockhash);
	}
	if (*fixup_map_xskmap) {
		map_fds[11] = create_map(BPF_MAP_TYPE_XSKMAP, sizeof(int),
					sizeof(int), 1);
		do {
			prog[*fixup_map_xskmap].imm = map_fds[11];
			fixup_map_xskmap++;
		} while (*fixup_map_xskmap);
	}
	if (*fixup_map_stacktrace) {
		map_fds[12] = create_map(BPF_MAP_TYPE_STACK_TRACE, sizeof(u32),
					 sizeof(u64), 1);
		do {
			prog[*fixup_map_stacktrace].imm = map_fds[12];
			fixup_map_stacktrace++;
		} while (*fixup_map_stacktrace);
	}
	if (*fixup_map_spin_lock) {
		map_fds[13] = create_map_spin_lock();
		do {
			prog[*fixup_map_spin_lock].imm = map_fds[13];
			fixup_map_spin_lock++;
		} while (*fixup_map_spin_lock);
	}
	if (*fixup_map_array_ro) {
		map_fds[14] = __create_map(BPF_MAP_TYPE_ARRAY, sizeof(int),
					   sizeof(struct test_val), 1,
					   BPF_F_RDONLY_PROG);
		update_map(map_fds[14], 0);
		do {
			prog[*fixup_map_array_ro].imm = map_fds[14];
			fixup_map_array_ro++;
		} while (*fixup_map_array_ro);
	}
	if (*fixup_map_array_wo) {
		map_fds[15] = __create_map(BPF_MAP_TYPE_ARRAY, sizeof(int),
					   sizeof(struct test_val), 1,
					   BPF_F_WRONLY_PROG);
		update_map(map_fds[15], 0);
		do {
			prog[*fixup_map_array_wo].imm = map_fds[15];
			fixup_map_array_wo++;
		} while (*fixup_map_array_wo);
	}
	if (*fixup_map_array_small) {
		map_fds[16] = __create_map(BPF_MAP_TYPE_ARRAY, sizeof(int),
					   1, 1, 0);
		update_map(map_fds[16], 0);
		do {
			prog[*fixup_map_array_small].imm = map_fds[16];
			fixup_map_array_small++;
		} while (*fixup_map_array_small);
	}
	if (*fixup_sk_storage_map) {
		map_fds[17] = create_sk_storage_map();
		do {
			prog[*fixup_sk_storage_map].imm = map_fds[17];
			fixup_sk_storage_map++;
		} while (*fixup_sk_storage_map);
	}
	if (*fixup_map_event_output) {
		map_fds[18] = __create_map(BPF_MAP_TYPE_PERF_EVENT_ARRAY,
					   sizeof(int), sizeof(int), 1, 0);
		do {
			prog[*fixup_map_event_output].imm = map_fds[18];
			fixup_map_event_output++;
		} while (*fixup_map_event_output);
	}
	if (*fixup_map_reuseport_array) {
		map_fds[19] = __create_map(BPF_MAP_TYPE_REUSEPORT_SOCKARRAY,
					   sizeof(u32), sizeof(u64), 1, 0);
		do {
			prog[*fixup_map_reuseport_array].imm = map_fds[19];
			fixup_map_reuseport_array++;
		} while (*fixup_map_reuseport_array);
	}
	if (*fixup_map_ringbuf) {
		map_fds[20] = create_map(BPF_MAP_TYPE_RINGBUF, 0,
					   0, 4096);
		do {
			prog[*fixup_map_ringbuf].imm = map_fds[20];
			fixup_map_ringbuf++;
		} while (*fixup_map_ringbuf);
	}
}

struct libcap {
	struct __user_cap_header_struct hdr;
	struct __user_cap_data_struct data[2];
};

static int set_admin(bool admin)
{
	cap_t caps;
	/* need CAP_BPF, CAP_NET_ADMIN, CAP_PERFMON to load progs */
	const cap_value_t cap_net_admin = CAP_NET_ADMIN;
	const cap_value_t cap_sys_admin = CAP_SYS_ADMIN;
	struct libcap *cap;
	int ret = -1;

	caps = cap_get_proc();
	if (!caps) {
		perror("cap_get_proc");
		return -1;
	}
	cap = (struct libcap *)caps;
	if (cap_set_flag(caps, CAP_EFFECTIVE, 1, &cap_sys_admin, CAP_CLEAR)) {
		perror("cap_set_flag clear admin");
		goto out;
	}
	if (cap_set_flag(caps, CAP_EFFECTIVE, 1, &cap_net_admin,
				admin ? CAP_SET : CAP_CLEAR)) {
		perror("cap_set_flag set_or_clear net");
		goto out;
	}
	/* libcap is likely old and simply ignores CAP_BPF and CAP_PERFMON,
	 * so update effective bits manually
	 */
	if (admin) {
		cap->data[1].effective |= 1 << (38 /* CAP_PERFMON */ - 32);
		cap->data[1].effective |= 1 << (39 /* CAP_BPF */ - 32);
	} else {
		cap->data[1].effective &= ~(1 << (38 - 32));
		cap->data[1].effective &= ~(1 << (39 - 32));
	}
	if (cap_set_proc(caps)) {
		perror("cap_set_proc");
		goto out;
	}
	ret = 0;
out:
	if (cap_free(caps))
		perror("cap_free");
	return ret;
}

static int do_prog_test_run(int fd_prog, bool unpriv, uint32_t expected_val,
			    void *data, size_t size_data)
{
	__u8 tmp[TEST_DATA_LEN << 2];
	__u32 size_tmp = sizeof(tmp);
	uint32_t retval;
	int err, saved_errno;

	if (unpriv)
		set_admin(true);
	err = bpf_prog_test_run(fd_prog, 1, data, size_data,
				tmp, &size_tmp, &retval, NULL);
	saved_errno = errno;

	if (unpriv)
		set_admin(false);

	if (err) {
		switch (saved_errno) {
		case 524/*ENOTSUPP*/:
			printf("Did not run the program (not supported) ");
			return 0;
		case EPERM:
			if (unpriv) {
				printf("Did not run the program (no permission) ");
				return 0;
			}
			/* fallthrough; */
		default:
			printf("FAIL: Unexpected bpf_prog_test_run error (%s) ",
				strerror(saved_errno));
			return err;
		}
	}

	if (retval != expected_val &&
	    expected_val != POINTER_VALUE) {
		printf("FAIL retval %d != %d ", retval, expected_val);
		return 1;
	}

	return 0;
}

/* Returns true if every part of exp (tab-separated) appears in log, in order.
 *
 * If exp is an empty string, returns true.
 */
static bool cmp_str_seq(const char *log, const char *exp)
{
	char needle[200];
	const char *p, *q;
	int len;

	do {
		if (!strlen(exp))
			break;
		p = strchr(exp, '\t');
		if (!p)
			p = exp + strlen(exp);

		len = p - exp;
		if (len >= sizeof(needle) || !len) {
			printf("FAIL\nTestcase bug\n");
			return false;
		}
		strncpy(needle, exp, len);
		needle[len] = 0;
		q = strstr(log, needle);
		if (!q) {
			printf("FAIL\nUnexpected verifier log!\n"
			       "EXP: %s\nRES:\n", needle);
			return false;
		}
		log = q + len;
		exp = p + 1;
	} while (*p);
	return true;
>>>>>>> upstream/android-13
}

static void do_test_single(struct bpf_test *test, bool unpriv,
			   int *passes, int *errors)
{
<<<<<<< HEAD
	int fd_prog, expected_ret, reject_from_alignment;
	int prog_len, prog_type = test->prog_type;
	struct bpf_insn *prog = test->insns;
	int map_fds[MAX_NR_MAPS];
	const char *expected_err;
	uint32_t retval;
=======
	int fd_prog, expected_ret, alignment_prevented_execution;
	int prog_len, prog_type = test->prog_type;
	struct bpf_insn *prog = test->insns;
	struct bpf_load_program_attr attr;
	int run_errs, run_successes;
	int map_fds[MAX_NR_MAPS];
	const char *expected_err;
	int saved_errno;
	int fixup_skips;
	__u32 pflags;
>>>>>>> upstream/android-13
	int i, err;

	for (i = 0; i < MAX_NR_MAPS; i++)
		map_fds[i] = -1;

<<<<<<< HEAD
	do_test_fixup(test, prog, map_fds);
	prog_len = probe_filter_length(prog);

	fd_prog = bpf_verify_program(prog_type ? : BPF_PROG_TYPE_SOCKET_FILTER,
				     prog, prog_len, test->flags & F_LOAD_WITH_STRICT_ALIGNMENT,
				     "GPL", 0, bpf_vlog, sizeof(bpf_vlog), 1);
=======
	if (!prog_type)
		prog_type = BPF_PROG_TYPE_SOCKET_FILTER;
	fixup_skips = skips;
	do_test_fixup(test, prog_type, prog, map_fds);
	if (test->fill_insns) {
		prog = test->fill_insns;
		prog_len = test->prog_len;
	} else {
		prog_len = probe_filter_length(prog);
	}
	/* If there were some map skips during fixup due to missing bpf
	 * features, skip this test.
	 */
	if (fixup_skips != skips)
		return;

	pflags = BPF_F_TEST_RND_HI32;
	if (test->flags & F_LOAD_WITH_STRICT_ALIGNMENT)
		pflags |= BPF_F_STRICT_ALIGNMENT;
	if (test->flags & F_NEEDS_EFFICIENT_UNALIGNED_ACCESS)
		pflags |= BPF_F_ANY_ALIGNMENT;
	if (test->flags & ~3)
		pflags |= test->flags;
>>>>>>> upstream/android-13

	expected_ret = unpriv && test->result_unpriv != UNDEF ?
		       test->result_unpriv : test->result;
	expected_err = unpriv && test->errstr_unpriv ?
		       test->errstr_unpriv : test->errstr;
<<<<<<< HEAD

	reject_from_alignment = fd_prog < 0 &&
				(test->flags & F_NEEDS_EFFICIENT_UNALIGNED_ACCESS) &&
				strstr(bpf_vlog, "misaligned");
#ifdef CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS
	if (reject_from_alignment) {
		printf("FAIL\nFailed due to alignment despite having efficient unaligned access: '%s'!\n",
		       strerror(errno));
		goto fail_log;
	}
#endif
	if (expected_ret == ACCEPT) {
		if (fd_prog < 0 && !reject_from_alignment) {
			printf("FAIL\nFailed to load prog '%s'!\n",
			       strerror(errno));
=======
	memset(&attr, 0, sizeof(attr));
	attr.prog_type = prog_type;
	attr.expected_attach_type = test->expected_attach_type;
	attr.insns = prog;
	attr.insns_cnt = prog_len;
	attr.license = "GPL";
	if (verbose)
		attr.log_level = 1;
	else if (expected_ret == VERBOSE_ACCEPT)
		attr.log_level = 2;
	else
		attr.log_level = 4;
	attr.prog_flags = pflags;

	if (prog_type == BPF_PROG_TYPE_TRACING && test->kfunc) {
		attr.attach_btf_id = libbpf_find_vmlinux_btf_id(test->kfunc,
						attr.expected_attach_type);
		if (attr.attach_btf_id < 0) {
			printf("FAIL\nFailed to find BTF ID for '%s'!\n",
				test->kfunc);
			(*errors)++;
			return;
		}
	}

	fd_prog = bpf_load_program_xattr(&attr, bpf_vlog, sizeof(bpf_vlog));
	saved_errno = errno;

	/* BPF_PROG_TYPE_TRACING requires more setup and
	 * bpf_probe_prog_type won't give correct answer
	 */
	if (fd_prog < 0 && prog_type != BPF_PROG_TYPE_TRACING &&
	    !bpf_probe_prog_type(prog_type, 0)) {
		printf("SKIP (unsupported program type %d)\n", prog_type);
		skips++;
		goto close_fds;
	}

	alignment_prevented_execution = 0;

	if (expected_ret == ACCEPT || expected_ret == VERBOSE_ACCEPT) {
		if (fd_prog < 0) {
			printf("FAIL\nFailed to load prog '%s'!\n",
			       strerror(saved_errno));
			goto fail_log;
		}
#ifndef CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS
		if (fd_prog >= 0 &&
		    (test->flags & F_NEEDS_EFFICIENT_UNALIGNED_ACCESS))
			alignment_prevented_execution = 1;
#endif
		if (expected_ret == VERBOSE_ACCEPT && !cmp_str_seq(bpf_vlog, expected_err)) {
>>>>>>> upstream/android-13
			goto fail_log;
		}
	} else {
		if (fd_prog >= 0) {
			printf("FAIL\nUnexpected success to load!\n");
			goto fail_log;
		}
<<<<<<< HEAD
		if (!strstr(bpf_vlog, expected_err) && !reject_from_alignment) {
=======
		if (!expected_err || !cmp_str_seq(bpf_vlog, expected_err)) {
>>>>>>> upstream/android-13
			printf("FAIL\nUnexpected error message!\n\tEXP: %s\n\tRES: %s\n",
			      expected_err, bpf_vlog);
			goto fail_log;
		}
	}

<<<<<<< HEAD
	if (fd_prog >= 0) {
		__u8 tmp[TEST_DATA_LEN << 2];
		__u32 size_tmp = sizeof(tmp);

		err = bpf_prog_test_run(fd_prog, 1, test->data,
					sizeof(test->data), tmp, &size_tmp,
					&retval, NULL);
		if (err && errno != 524/*ENOTSUPP*/ && errno != EPERM) {
			printf("Unexpected bpf_prog_test_run error\n");
			goto fail_log;
		}
		if (!err && retval != test->retval &&
		    test->retval != POINTER_VALUE) {
			printf("FAIL retval %d != %d\n", retval, test->retval);
			goto fail_log;
		}
	}
	(*passes)++;
	printf("OK%s\n", reject_from_alignment ?
	       " (NOTE: reject due to unknown alignment)" : "");
close_fds:
=======
	if (!unpriv && test->insn_processed) {
		uint32_t insn_processed;
		char *proc;

		proc = strstr(bpf_vlog, "processed ");
		insn_processed = atoi(proc + 10);
		if (test->insn_processed != insn_processed) {
			printf("FAIL\nUnexpected insn_processed %u vs %u\n",
			       insn_processed, test->insn_processed);
			goto fail_log;
		}
	}

	if (verbose)
		printf(", verifier log:\n%s", bpf_vlog);

	run_errs = 0;
	run_successes = 0;
	if (!alignment_prevented_execution && fd_prog >= 0 && test->runs >= 0) {
		uint32_t expected_val;
		int i;

		if (!test->runs)
			test->runs = 1;

		for (i = 0; i < test->runs; i++) {
			if (unpriv && test->retvals[i].retval_unpriv)
				expected_val = test->retvals[i].retval_unpriv;
			else
				expected_val = test->retvals[i].retval;

			err = do_prog_test_run(fd_prog, unpriv, expected_val,
					       test->retvals[i].data,
					       sizeof(test->retvals[i].data));
			if (err) {
				printf("(run %d/%d) ", i + 1, test->runs);
				run_errs++;
			} else {
				run_successes++;
			}
		}
	}

	if (!run_errs) {
		(*passes)++;
		if (run_successes > 1)
			printf("%d cases ", run_successes);
		printf("OK");
		if (alignment_prevented_execution)
			printf(" (NOTE: not executed due to unknown alignment)");
		printf("\n");
	} else {
		printf("\n");
		goto fail_log;
	}
close_fds:
	if (test->fill_insns)
		free(test->fill_insns);
>>>>>>> upstream/android-13
	close(fd_prog);
	for (i = 0; i < MAX_NR_MAPS; i++)
		close(map_fds[i]);
	sched_yield();
	return;
fail_log:
	(*errors)++;
	printf("%s", bpf_vlog);
	goto close_fds;
}

static bool is_admin(void)
{
<<<<<<< HEAD
	cap_t caps;
	cap_flag_value_t sysadmin = CAP_CLEAR;
	const cap_value_t cap_val = CAP_SYS_ADMIN;
=======
	cap_flag_value_t net_priv = CAP_CLEAR;
	bool perfmon_priv = false;
	bool bpf_priv = false;
	struct libcap *cap;
	cap_t caps;
>>>>>>> upstream/android-13

#ifdef CAP_IS_SUPPORTED
	if (!CAP_IS_SUPPORTED(CAP_SETFCAP)) {
		perror("cap_get_flag");
		return false;
	}
#endif
	caps = cap_get_proc();
	if (!caps) {
		perror("cap_get_proc");
		return false;
	}
<<<<<<< HEAD
	if (cap_get_flag(caps, cap_val, CAP_EFFECTIVE, &sysadmin))
		perror("cap_get_flag");
	if (cap_free(caps))
		perror("cap_free");
	return (sysadmin == CAP_SET);
}

static int set_admin(bool admin)
{
	cap_t caps;
	const cap_value_t cap_val = CAP_SYS_ADMIN;
	int ret = -1;

	caps = cap_get_proc();
	if (!caps) {
		perror("cap_get_proc");
		return -1;
	}
	if (cap_set_flag(caps, CAP_EFFECTIVE, 1, &cap_val,
				admin ? CAP_SET : CAP_CLEAR)) {
		perror("cap_set_flag");
		goto out;
	}
	if (cap_set_proc(caps)) {
		perror("cap_set_proc");
		goto out;
	}
	ret = 0;
out:
	if (cap_free(caps))
		perror("cap_free");
	return ret;
=======
	cap = (struct libcap *)caps;
	bpf_priv = cap->data[1].effective & (1 << (39/* CAP_BPF */ - 32));
	perfmon_priv = cap->data[1].effective & (1 << (38/* CAP_PERFMON */ - 32));
	if (cap_get_flag(caps, CAP_NET_ADMIN, CAP_EFFECTIVE, &net_priv))
		perror("cap_get_flag NET");
	if (cap_free(caps))
		perror("cap_free");
	return bpf_priv && perfmon_priv && net_priv == CAP_SET;
>>>>>>> upstream/android-13
}

static void get_unpriv_disabled()
{
	char buf[2];
	FILE *fd;

	fd = fopen("/proc/sys/"UNPRIV_SYSCTL, "r");
	if (!fd) {
		perror("fopen /proc/sys/"UNPRIV_SYSCTL);
		unpriv_disabled = true;
		return;
	}
	if (fgets(buf, 2, fd) == buf && atoi(buf))
		unpriv_disabled = true;
	fclose(fd);
}

<<<<<<< HEAD
static int do_test(bool unpriv, unsigned int from, unsigned int to)
{
	int i, passes = 0, errors = 0, skips = 0;
=======
static bool test_as_unpriv(struct bpf_test *test)
{
#ifndef CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS
	/* Some architectures have strict alignment requirements. In
	 * that case, the BPF verifier detects if a program has
	 * unaligned accesses and rejects them. A user can pass
	 * BPF_F_ANY_ALIGNMENT to a program to override this
	 * check. That, however, will only work when a privileged user
	 * loads a program. An unprivileged user loading a program
	 * with this flag will be rejected prior entering the
	 * verifier.
	 */
	if (test->flags & F_NEEDS_EFFICIENT_UNALIGNED_ACCESS)
		return false;
#endif
	return !test->prog_type ||
	       test->prog_type == BPF_PROG_TYPE_SOCKET_FILTER ||
	       test->prog_type == BPF_PROG_TYPE_CGROUP_SKB;
}

static int do_test(bool unpriv, unsigned int from, unsigned int to)
{
	int i, passes = 0, errors = 0;
>>>>>>> upstream/android-13

	for (i = from; i < to; i++) {
		struct bpf_test *test = &tests[i];

		/* Program types that are not supported by non-root we
		 * skip right away.
		 */
<<<<<<< HEAD
		if (!test->prog_type && unpriv_disabled) {
			printf("#%d/u %s SKIP\n", i, test->descr);
			skips++;
		} else if (!test->prog_type) {
=======
		if (test_as_unpriv(test) && unpriv_disabled) {
			printf("#%d/u %s SKIP\n", i, test->descr);
			skips++;
		} else if (test_as_unpriv(test)) {
>>>>>>> upstream/android-13
			if (!unpriv)
				set_admin(false);
			printf("#%d/u %s ", i, test->descr);
			do_test_single(test, true, &passes, &errors);
			if (!unpriv)
				set_admin(true);
		}

		if (unpriv) {
			printf("#%d/p %s SKIP\n", i, test->descr);
			skips++;
		} else {
			printf("#%d/p %s ", i, test->descr);
			do_test_single(test, false, &passes, &errors);
		}
	}

	printf("Summary: %d PASSED, %d SKIPPED, %d FAILED\n", passes,
	       skips, errors);
	return errors ? EXIT_FAILURE : EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
	unsigned int from = 0, to = ARRAY_SIZE(tests);
	bool unpriv = !is_admin();
<<<<<<< HEAD

	if (argc == 3) {
		unsigned int l = atoi(argv[argc - 2]);
		unsigned int u = atoi(argv[argc - 1]);
=======
	int arg = 1;

	if (argc > 1 && strcmp(argv[1], "-v") == 0) {
		arg++;
		verbose = true;
		argc--;
	}

	if (argc == 3) {
		unsigned int l = atoi(argv[arg]);
		unsigned int u = atoi(argv[arg + 1]);
>>>>>>> upstream/android-13

		if (l < to && u < to) {
			from = l;
			to   = u + 1;
		}
	} else if (argc == 2) {
<<<<<<< HEAD
		unsigned int t = atoi(argv[argc - 1]);
=======
		unsigned int t = atoi(argv[arg]);
>>>>>>> upstream/android-13

		if (t < to) {
			from = t;
			to   = t + 1;
		}
	}

	get_unpriv_disabled();
	if (unpriv && unpriv_disabled) {
		printf("Cannot run as unprivileged user with sysctl %s.\n",
		       UNPRIV_SYSCTL);
		return EXIT_FAILURE;
	}

	bpf_semi_rand_init();
	return do_test(unpriv, from, to);
}
