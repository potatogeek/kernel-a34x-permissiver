<<<<<<< HEAD
/*
 * intel_pt_decoder.h: Intel Processor Trace support
 * Copyright (c) 2013-2014, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * intel_pt_decoder.h: Intel Processor Trace support
 * Copyright (c) 2013-2014, Intel Corporation.
>>>>>>> upstream/android-13
 */

#ifndef INCLUDE__INTEL_PT_DECODER_H__
#define INCLUDE__INTEL_PT_DECODER_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

<<<<<<< HEAD
=======
#include <linux/rbtree.h>

>>>>>>> upstream/android-13
#include "intel-pt-insn-decoder.h"

#define INTEL_PT_IN_TX		(1 << 0)
#define INTEL_PT_ABORT_TX	(1 << 1)
#define INTEL_PT_ASYNC		(1 << 2)
#define INTEL_PT_FUP_IP		(1 << 3)
<<<<<<< HEAD
=======
#define INTEL_PT_SAMPLE_IPC	(1 << 4)
>>>>>>> upstream/android-13

enum intel_pt_sample_type {
	INTEL_PT_BRANCH		= 1 << 0,
	INTEL_PT_INSTRUCTION	= 1 << 1,
	INTEL_PT_TRANSACTION	= 1 << 2,
	INTEL_PT_PTW		= 1 << 3,
	INTEL_PT_MWAIT_OP	= 1 << 4,
	INTEL_PT_PWR_ENTRY	= 1 << 5,
	INTEL_PT_EX_STOP	= 1 << 6,
	INTEL_PT_PWR_EXIT	= 1 << 7,
	INTEL_PT_CBR_CHG	= 1 << 8,
<<<<<<< HEAD
=======
	INTEL_PT_TRACE_BEGIN	= 1 << 9,
	INTEL_PT_TRACE_END	= 1 << 10,
	INTEL_PT_BLK_ITEMS	= 1 << 11,
	INTEL_PT_PSB_EVT	= 1 << 12,
>>>>>>> upstream/android-13
};

enum intel_pt_period_type {
	INTEL_PT_PERIOD_NONE,
	INTEL_PT_PERIOD_INSTRUCTIONS,
	INTEL_PT_PERIOD_TICKS,
	INTEL_PT_PERIOD_MTC,
};

enum {
	INTEL_PT_ERR_NOMEM = 1,
	INTEL_PT_ERR_INTERN,
	INTEL_PT_ERR_BADPKT,
	INTEL_PT_ERR_NODATA,
	INTEL_PT_ERR_NOINSN,
	INTEL_PT_ERR_MISMAT,
	INTEL_PT_ERR_OVR,
	INTEL_PT_ERR_LOST,
	INTEL_PT_ERR_UNK,
	INTEL_PT_ERR_NELOOP,
	INTEL_PT_ERR_MAX,
};

enum intel_pt_param_flags {
	/*
	 * FUP packet can contain next linear instruction pointer instead of
	 * current linear instruction pointer.
	 */
	INTEL_PT_FUP_WITH_NLIP	= 1 << 0,
};

<<<<<<< HEAD
struct intel_pt_state {
	enum intel_pt_sample_type type;
	int err;
	uint64_t from_ip;
	uint64_t to_ip;
	uint64_t cr3;
	uint64_t tot_insn_cnt;
=======
enum intel_pt_blk_type {
	INTEL_PT_GP_REGS	= 1,
	INTEL_PT_PEBS_BASIC	= 4,
	INTEL_PT_PEBS_MEM	= 5,
	INTEL_PT_LBR_0		= 8,
	INTEL_PT_LBR_1		= 9,
	INTEL_PT_LBR_2		= 10,
	INTEL_PT_XMM		= 16,
	INTEL_PT_BLK_TYPE_MAX
};

/*
 * The block type numbers are not sequential but here they are given sequential
 * positions to avoid wasting space for array placement.
 */
enum intel_pt_blk_type_pos {
	INTEL_PT_GP_REGS_POS,
	INTEL_PT_PEBS_BASIC_POS,
	INTEL_PT_PEBS_MEM_POS,
	INTEL_PT_LBR_0_POS,
	INTEL_PT_LBR_1_POS,
	INTEL_PT_LBR_2_POS,
	INTEL_PT_XMM_POS,
	INTEL_PT_BLK_TYPE_CNT
};

/* Get the array position for a block type */
static inline int intel_pt_blk_type_pos(enum intel_pt_blk_type blk_type)
{
#define BLK_TYPE(bt) [INTEL_PT_##bt] = INTEL_PT_##bt##_POS + 1
	const int map[INTEL_PT_BLK_TYPE_MAX] = {
		BLK_TYPE(GP_REGS),
		BLK_TYPE(PEBS_BASIC),
		BLK_TYPE(PEBS_MEM),
		BLK_TYPE(LBR_0),
		BLK_TYPE(LBR_1),
		BLK_TYPE(LBR_2),
		BLK_TYPE(XMM),
	};
#undef BLK_TYPE

	return blk_type < INTEL_PT_BLK_TYPE_MAX ? map[blk_type] - 1 : -1;
}

#define INTEL_PT_BLK_ITEM_ID_CNT	32

/*
 * Use unions so that the block items can be accessed by name or by array index.
 * There is an array of 32-bit masks for each block type, which indicate which
 * values are present. Then arrays of 32 64-bit values for each block type.
 */
struct intel_pt_blk_items {
	union {
		uint32_t mask[INTEL_PT_BLK_TYPE_CNT];
		struct {
			uint32_t has_rflags:1;
			uint32_t has_rip:1;
			uint32_t has_rax:1;
			uint32_t has_rcx:1;
			uint32_t has_rdx:1;
			uint32_t has_rbx:1;
			uint32_t has_rsp:1;
			uint32_t has_rbp:1;
			uint32_t has_rsi:1;
			uint32_t has_rdi:1;
			uint32_t has_r8:1;
			uint32_t has_r9:1;
			uint32_t has_r10:1;
			uint32_t has_r11:1;
			uint32_t has_r12:1;
			uint32_t has_r13:1;
			uint32_t has_r14:1;
			uint32_t has_r15:1;
			uint32_t has_unused_0:14;
			uint32_t has_ip:1;
			uint32_t has_applicable_counters:1;
			uint32_t has_timestamp:1;
			uint32_t has_unused_1:29;
			uint32_t has_mem_access_address:1;
			uint32_t has_mem_aux_info:1;
			uint32_t has_mem_access_latency:1;
			uint32_t has_tsx_aux_info:1;
			uint32_t has_unused_2:28;
			uint32_t has_lbr_0;
			uint32_t has_lbr_1;
			uint32_t has_lbr_2;
			uint32_t has_xmm;
		};
	};
	union {
		uint64_t val[INTEL_PT_BLK_TYPE_CNT][INTEL_PT_BLK_ITEM_ID_CNT];
		struct {
			struct {
				uint64_t rflags;
				uint64_t rip;
				uint64_t rax;
				uint64_t rcx;
				uint64_t rdx;
				uint64_t rbx;
				uint64_t rsp;
				uint64_t rbp;
				uint64_t rsi;
				uint64_t rdi;
				uint64_t r8;
				uint64_t r9;
				uint64_t r10;
				uint64_t r11;
				uint64_t r12;
				uint64_t r13;
				uint64_t r14;
				uint64_t r15;
				uint64_t unused_0[INTEL_PT_BLK_ITEM_ID_CNT - 18];
			};
			struct {
				uint64_t ip;
				uint64_t applicable_counters;
				uint64_t timestamp;
				uint64_t unused_1[INTEL_PT_BLK_ITEM_ID_CNT - 3];
			};
			struct {
				uint64_t mem_access_address;
				uint64_t mem_aux_info;
				uint64_t mem_access_latency;
				uint64_t tsx_aux_info;
				uint64_t unused_2[INTEL_PT_BLK_ITEM_ID_CNT - 4];
			};
			uint64_t lbr_0[INTEL_PT_BLK_ITEM_ID_CNT];
			uint64_t lbr_1[INTEL_PT_BLK_ITEM_ID_CNT];
			uint64_t lbr_2[INTEL_PT_BLK_ITEM_ID_CNT];
			uint64_t xmm[INTEL_PT_BLK_ITEM_ID_CNT];
		};
	};
	bool is_32_bit;
};

struct intel_pt_vmcs_info {
	struct rb_node rb_node;
	uint64_t vmcs;
	uint64_t tsc_offset;
	bool reliable;
	bool error_printed;
};

struct intel_pt_state {
	enum intel_pt_sample_type type;
	bool from_nr;
	bool to_nr;
	int err;
	uint64_t from_ip;
	uint64_t to_ip;
	uint64_t tot_insn_cnt;
	uint64_t tot_cyc_cnt;
>>>>>>> upstream/android-13
	uint64_t timestamp;
	uint64_t est_timestamp;
	uint64_t trace_nr;
	uint64_t ptw_payload;
	uint64_t mwait_payload;
	uint64_t pwre_payload;
	uint64_t pwrx_payload;
	uint64_t cbr_payload;
<<<<<<< HEAD
=======
	uint64_t psb_offset;
	uint32_t cbr;
>>>>>>> upstream/android-13
	uint32_t flags;
	enum intel_pt_insn_op insn_op;
	int insn_len;
	char insn[INTEL_PT_INSN_BUF_SZ];
<<<<<<< HEAD
=======
	struct intel_pt_blk_items items;
>>>>>>> upstream/android-13
};

struct intel_pt_insn;

struct intel_pt_buffer {
	const unsigned char *buf;
	size_t len;
	bool consecutive;
	uint64_t ref_timestamp;
	uint64_t trace_nr;
};

<<<<<<< HEAD
=======
typedef int (*intel_pt_lookahead_cb_t)(struct intel_pt_buffer *, void *);

>>>>>>> upstream/android-13
struct intel_pt_params {
	int (*get_trace)(struct intel_pt_buffer *buffer, void *data);
	int (*walk_insn)(struct intel_pt_insn *intel_pt_insn,
			 uint64_t *insn_cnt_ptr, uint64_t *ip, uint64_t to_ip,
			 uint64_t max_insn_cnt, void *data);
	bool (*pgd_ip)(uint64_t ip, void *data);
<<<<<<< HEAD
	void *data;
	bool return_compression;
	bool branch_enable;
=======
	int (*lookahead)(void *data, intel_pt_lookahead_cb_t cb, void *cb_data);
	struct intel_pt_vmcs_info *(*findnew_vmcs_info)(void *data, uint64_t vmcs);
	void *data;
	bool return_compression;
	bool branch_enable;
	bool vm_time_correlation;
	bool vm_tm_corr_dry_run;
	uint64_t first_timestamp;
	uint64_t ctl;
>>>>>>> upstream/android-13
	uint64_t period;
	enum intel_pt_period_type period_type;
	unsigned max_non_turbo_ratio;
	unsigned int mtc_period;
	uint32_t tsc_ctc_ratio_n;
	uint32_t tsc_ctc_ratio_d;
	enum intel_pt_param_flags flags;
<<<<<<< HEAD
=======
	unsigned int quick;
	int max_loops;
>>>>>>> upstream/android-13
};

struct intel_pt_decoder;

struct intel_pt_decoder *intel_pt_decoder_new(struct intel_pt_params *params);
void intel_pt_decoder_free(struct intel_pt_decoder *decoder);

const struct intel_pt_state *intel_pt_decode(struct intel_pt_decoder *decoder);

<<<<<<< HEAD
unsigned char *intel_pt_find_overlap(unsigned char *buf_a, size_t len_a,
				     unsigned char *buf_b, size_t len_b,
				     bool have_tsc, bool *consecutive);

int intel_pt__strerror(int code, char *buf, size_t buflen);

=======
int intel_pt_fast_forward(struct intel_pt_decoder *decoder, uint64_t timestamp);

unsigned char *intel_pt_find_overlap(unsigned char *buf_a, size_t len_a,
				     unsigned char *buf_b, size_t len_b,
				     bool have_tsc, bool *consecutive,
				     bool ooo_tsc);

int intel_pt__strerror(int code, char *buf, size_t buflen);

void intel_pt_set_first_timestamp(struct intel_pt_decoder *decoder,
				  uint64_t first_timestamp);

>>>>>>> upstream/android-13
#endif
