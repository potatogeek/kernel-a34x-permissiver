<<<<<<< HEAD
/*
 * intel_pt_log.h: Intel Processor Trace support
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
 * intel_pt_log.h: Intel Processor Trace support
 * Copyright (c) 2013-2014, Intel Corporation.
>>>>>>> upstream/android-13
 */

#ifndef INCLUDE__INTEL_PT_LOG_H__
#define INCLUDE__INTEL_PT_LOG_H__

#include <linux/compiler.h>
#include <stdint.h>
#include <inttypes.h>

struct intel_pt_pkt;

<<<<<<< HEAD
=======
void *intel_pt_log_fp(void);
>>>>>>> upstream/android-13
void intel_pt_log_enable(void);
void intel_pt_log_disable(void);
void intel_pt_log_set_name(const char *name);

void __intel_pt_log_packet(const struct intel_pt_pkt *packet, int pkt_len,
			   uint64_t pos, const unsigned char *buf);

struct intel_pt_insn;

void __intel_pt_log_insn(struct intel_pt_insn *intel_pt_insn, uint64_t ip);
void __intel_pt_log_insn_no_data(struct intel_pt_insn *intel_pt_insn,
				 uint64_t ip);

void __intel_pt_log(const char *fmt, ...) __printf(1, 2);

#define intel_pt_log(fmt, ...) \
	do { \
		if (intel_pt_enable_logging) \
			__intel_pt_log(fmt, ##__VA_ARGS__); \
	} while (0)

#define intel_pt_log_packet(arg, ...) \
	do { \
		if (intel_pt_enable_logging) \
			__intel_pt_log_packet(arg, ##__VA_ARGS__); \
	} while (0)

#define intel_pt_log_insn(arg, ...) \
	do { \
		if (intel_pt_enable_logging) \
			__intel_pt_log_insn(arg, ##__VA_ARGS__); \
	} while (0)

#define intel_pt_log_insn_no_data(arg, ...) \
	do { \
		if (intel_pt_enable_logging) \
			__intel_pt_log_insn_no_data(arg, ##__VA_ARGS__); \
	} while (0)

#define x64_fmt "0x%" PRIx64

extern bool intel_pt_enable_logging;

static inline void intel_pt_log_at(const char *msg, uint64_t u)
{
	intel_pt_log("%s at " x64_fmt "\n", msg, u);
}

static inline void intel_pt_log_to(const char *msg, uint64_t u)
{
	intel_pt_log("%s to " x64_fmt "\n", msg, u);
}

<<<<<<< HEAD
=======
#define intel_pt_log_var(var, fmt) intel_pt_log("%s: " #var " " fmt "\n", __func__, var)

#define intel_pt_log_x32(var) intel_pt_log_var(var, "%#x")
#define intel_pt_log_x64(var) intel_pt_log_var(var, "%#" PRIx64)

>>>>>>> upstream/android-13
#endif
