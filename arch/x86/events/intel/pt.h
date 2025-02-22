<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Intel(R) Processor Trace PMU driver for perf
 * Copyright (c) 2013-2014, Intel Corporation.
 *
<<<<<<< HEAD
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
>>>>>>> upstream/android-13
 * Intel PT is specified in the Intel Architecture Instruction Set Extensions
 * Programming Reference:
 * http://software.intel.com/en-us/intel-isa-extensions
 */

#ifndef __INTEL_PT_H__
#define __INTEL_PT_H__

/*
<<<<<<< HEAD
 * PT MSR bit definitions
 */
#define RTIT_CTL_TRACEEN		BIT(0)
#define RTIT_CTL_CYCLEACC		BIT(1)
#define RTIT_CTL_OS			BIT(2)
#define RTIT_CTL_USR			BIT(3)
#define RTIT_CTL_PWR_EVT_EN		BIT(4)
#define RTIT_CTL_FUP_ON_PTW		BIT(5)
#define RTIT_CTL_CR3EN			BIT(7)
#define RTIT_CTL_TOPA			BIT(8)
#define RTIT_CTL_MTC_EN			BIT(9)
#define RTIT_CTL_TSC_EN			BIT(10)
#define RTIT_CTL_DISRETC		BIT(11)
#define RTIT_CTL_PTW_EN			BIT(12)
#define RTIT_CTL_BRANCH_EN		BIT(13)
#define RTIT_CTL_MTC_RANGE_OFFSET	14
#define RTIT_CTL_MTC_RANGE		(0x0full << RTIT_CTL_MTC_RANGE_OFFSET)
#define RTIT_CTL_CYC_THRESH_OFFSET	19
#define RTIT_CTL_CYC_THRESH		(0x0full << RTIT_CTL_CYC_THRESH_OFFSET)
#define RTIT_CTL_PSB_FREQ_OFFSET	24
#define RTIT_CTL_PSB_FREQ      		(0x0full << RTIT_CTL_PSB_FREQ_OFFSET)
#define RTIT_CTL_ADDR0_OFFSET		32
#define RTIT_CTL_ADDR0      		(0x0full << RTIT_CTL_ADDR0_OFFSET)
#define RTIT_CTL_ADDR1_OFFSET		36
#define RTIT_CTL_ADDR1      		(0x0full << RTIT_CTL_ADDR1_OFFSET)
#define RTIT_CTL_ADDR2_OFFSET		40
#define RTIT_CTL_ADDR2      		(0x0full << RTIT_CTL_ADDR2_OFFSET)
#define RTIT_CTL_ADDR3_OFFSET		44
#define RTIT_CTL_ADDR3      		(0x0full << RTIT_CTL_ADDR3_OFFSET)
#define RTIT_STATUS_FILTEREN		BIT(0)
#define RTIT_STATUS_CONTEXTEN		BIT(1)
#define RTIT_STATUS_TRIGGEREN		BIT(2)
#define RTIT_STATUS_BUFFOVF		BIT(3)
#define RTIT_STATUS_ERROR		BIT(4)
#define RTIT_STATUS_STOPPED		BIT(5)

/*
=======
>>>>>>> upstream/android-13
 * Single-entry ToPA: when this close to region boundary, switch
 * buffers to avoid losing data.
 */
#define TOPA_PMI_MARGIN 512

#define TOPA_SHIFT 12

static inline unsigned int sizes(unsigned int tsz)
{
	return 1 << (tsz + TOPA_SHIFT);
};

struct topa_entry {
	u64	end	: 1;
	u64	rsvd0	: 1;
	u64	intr	: 1;
	u64	rsvd1	: 1;
	u64	stop	: 1;
	u64	rsvd2	: 1;
	u64	size	: 4;
	u64	rsvd3	: 2;
	u64	base	: 36;
	u64	rsvd4	: 16;
};

<<<<<<< HEAD
#define PT_CPUID_LEAVES		2
#define PT_CPUID_REGS_NUM	4 /* number of regsters (eax, ebx, ecx, edx) */

/* TSC to Core Crystal Clock Ratio */
#define CPUID_TSC_LEAF		0x15

enum pt_capabilities {
	PT_CAP_max_subleaf = 0,
	PT_CAP_cr3_filtering,
	PT_CAP_psb_cyc,
	PT_CAP_ip_filtering,
	PT_CAP_mtc,
	PT_CAP_ptwrite,
	PT_CAP_power_event_trace,
	PT_CAP_topa_output,
	PT_CAP_topa_multiple_entries,
	PT_CAP_single_range_output,
	PT_CAP_payloads_lip,
	PT_CAP_num_address_ranges,
	PT_CAP_mtc_periods,
	PT_CAP_cycle_thresholds,
	PT_CAP_psb_periods,
};

=======
/* TSC to Core Crystal Clock Ratio */
#define CPUID_TSC_LEAF		0x15

>>>>>>> upstream/android-13
struct pt_pmu {
	struct pmu		pmu;
	u32			caps[PT_CPUID_REGS_NUM * PT_CPUID_LEAVES];
	bool			vmx;
	bool			branch_en_always_on;
	unsigned long		max_nonturbo_ratio;
	unsigned int		tsc_art_num;
	unsigned int		tsc_art_den;
};

/**
 * struct pt_buffer - buffer configuration; one buffer per task_struct or
 *		cpu, depending on perf event configuration
<<<<<<< HEAD
 * @cpu:	cpu for per-cpu allocation
=======
>>>>>>> upstream/android-13
 * @tables:	list of ToPA tables in this buffer
 * @first:	shorthand for first topa table
 * @last:	shorthand for last topa table
 * @cur:	current topa table
 * @nr_pages:	buffer size in pages
 * @cur_idx:	current output region's index within @cur table
 * @output_off:	offset within the current output region
 * @data_size:	running total of the amount of data in this buffer
 * @lost:	if data was lost/truncated
 * @head:	logical write offset inside the buffer
 * @snapshot:	if this is for a snapshot/overwrite counter
<<<<<<< HEAD
 * @stop_pos:	STOP topa entry in the buffer
 * @intr_pos:	INT topa entry in the buffer
=======
 * @single:	use Single Range Output instead of ToPA
 * @stop_pos:	STOP topa entry index
 * @intr_pos:	INT topa entry index
 * @stop_te:	STOP topa entry pointer
 * @intr_te:	INT topa entry pointer
>>>>>>> upstream/android-13
 * @data_pages:	array of pages from perf
 * @topa_index:	table of topa entries indexed by page offset
 */
struct pt_buffer {
<<<<<<< HEAD
	int			cpu;
=======
>>>>>>> upstream/android-13
	struct list_head	tables;
	struct topa		*first, *last, *cur;
	unsigned int		cur_idx;
	size_t			output_off;
	unsigned long		nr_pages;
	local_t			data_size;
	local64_t		head;
	bool			snapshot;
<<<<<<< HEAD
	unsigned long		stop_pos, intr_pos;
	void			**data_pages;
	struct topa_entry	*topa_index[0];
=======
	bool			single;
	long			stop_pos, intr_pos;
	struct topa_entry	*stop_te, *intr_te;
	void			**data_pages;
>>>>>>> upstream/android-13
};

#define PT_FILTERS_NUM	4

/**
 * struct pt_filter - IP range filter configuration
 * @msr_a:	range start, goes to RTIT_ADDRn_A
 * @msr_b:	range end, goes to RTIT_ADDRn_B
 * @config:	4-bit field in RTIT_CTL
 */
struct pt_filter {
	unsigned long	msr_a;
	unsigned long	msr_b;
	unsigned long	config;
};

/**
 * struct pt_filters - IP range filtering context
 * @filter:	filters defined for this context
 * @nr_filters:	number of defined filters in the @filter array
 */
struct pt_filters {
	struct pt_filter	filter[PT_FILTERS_NUM];
	unsigned int		nr_filters;
};

/**
 * struct pt - per-cpu pt context
<<<<<<< HEAD
 * @handle:	perf output handle
 * @filters:		last configured filters
 * @handle_nmi:	do handle PT PMI on this cpu, there's an active event
 * @vmx_on:	1 if VMX is ON on this cpu
=======
 * @handle:		perf output handle
 * @filters:		last configured filters
 * @handle_nmi:		do handle PT PMI on this cpu, there's an active event
 * @vmx_on:		1 if VMX is ON on this cpu
 * @output_base:	cached RTIT_OUTPUT_BASE MSR value
 * @output_mask:	cached RTIT_OUTPUT_MASK MSR value
>>>>>>> upstream/android-13
 */
struct pt {
	struct perf_output_handle handle;
	struct pt_filters	filters;
	int			handle_nmi;
	int			vmx_on;
<<<<<<< HEAD
=======
	u64			output_base;
	u64			output_mask;
>>>>>>> upstream/android-13
};

#endif /* __INTEL_PT_H__ */
