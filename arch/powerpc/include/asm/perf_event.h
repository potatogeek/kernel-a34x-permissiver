<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Performance event support - hardware-specific disambiguation
 *
 * For now this is a compile-time decision, but eventually it should be
 * runtime.  This would allow multiplatform perf event support for e300 (fsl
 * embedded perf counters) plus server/classic, and would accommodate
 * devices other than the core which provide their own performance counters.
 *
 * Copyright 2010 Freescale Semiconductor, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifdef CONFIG_PPC_PERF_CTRS
#include <asm/perf_event_server.h>
<<<<<<< HEAD
=======
#else
static inline bool is_sier_available(void) { return false; }
static inline unsigned long get_pmcs_ext_regs(int idx) { return 0; }
>>>>>>> upstream/android-13
#endif

#ifdef CONFIG_FSL_EMB_PERF_EVENT
#include <asm/perf_event_fsl_emb.h>
#endif

#ifdef CONFIG_PERF_EVENTS
#include <asm/ptrace.h>
#include <asm/reg.h>

<<<<<<< HEAD
=======
#define perf_arch_bpf_user_pt_regs(regs) &regs->user_regs

>>>>>>> upstream/android-13
/*
 * Overload regs->result to specify whether we should use the MSR (result
 * is zero) or the SIAR (result is non zero).
 */
#define perf_arch_fetch_caller_regs(regs, __ip)			\
	do {							\
		(regs)->result = 0;				\
		(regs)->nip = __ip;				\
<<<<<<< HEAD
		(regs)->gpr[1] = current_stack_pointer();	\
		asm volatile("mfmsr %0" : "=r" ((regs)->msr));	\
	} while (0)
=======
		(regs)->gpr[1] = current_stack_frame();		\
		asm volatile("mfmsr %0" : "=r" ((regs)->msr));	\
	} while (0)

/* To support perf_regs sier update */
extern bool is_sier_available(void);
extern unsigned long get_pmcs_ext_regs(int idx);
/* To define perf extended regs mask value */
extern u64 PERF_REG_EXTENDED_MASK;
#define PERF_REG_EXTENDED_MASK	PERF_REG_EXTENDED_MASK
>>>>>>> upstream/android-13
#endif
