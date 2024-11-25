<<<<<<< HEAD
/*
 * Copyright (C) 2017 Linaro Ltd. <ard.biesheuvel@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2017 Linaro Ltd. <ard.biesheuvel@linaro.org>
>>>>>>> upstream/android-13
 */

#ifndef __ASM_SIMD_H
#define __ASM_SIMD_H

#include <linux/compiler.h>
#include <linux/irqflags.h>
#include <linux/percpu.h>
#include <linux/preempt.h>
#include <linux/types.h>

<<<<<<< HEAD
#ifdef CONFIG_KERNEL_MODE_NEON

DECLARE_PER_CPU(bool, kernel_neon_busy);
=======
DECLARE_PER_CPU(bool, fpsimd_context_busy);

#ifdef CONFIG_KERNEL_MODE_NEON
>>>>>>> upstream/android-13

/*
 * may_use_simd - whether it is allowable at this time to issue SIMD
 *                instructions or access the SIMD register file
 *
 * Callers must not assume that the result remains true beyond the next
 * preempt_enable() or return from softirq context.
 */
static __must_check inline bool may_use_simd(void)
{
	/*
<<<<<<< HEAD
	 * kernel_neon_busy is only set while preemption is disabled,
	 * and is clear whenever preemption is enabled. Since
	 * this_cpu_read() is atomic w.r.t. preemption, kernel_neon_busy
=======
	 * We must make sure that the SVE has been initialized properly
	 * before using the SIMD in kernel.
	 * fpsimd_context_busy is only set while preemption is disabled,
	 * and is clear whenever preemption is enabled. Since
	 * this_cpu_read() is atomic w.r.t. preemption, fpsimd_context_busy
>>>>>>> upstream/android-13
	 * cannot change under our feet -- if it's set we cannot be
	 * migrated, and if it's clear we cannot be migrated to a CPU
	 * where it is set.
	 */
<<<<<<< HEAD
	return !in_irq() && !irqs_disabled() && !in_nmi() &&
		!this_cpu_read(kernel_neon_busy);
=======
	return !in_hardirq() && !irqs_disabled() && !in_nmi() &&
	       !this_cpu_read(fpsimd_context_busy);
>>>>>>> upstream/android-13
}

#else /* ! CONFIG_KERNEL_MODE_NEON */

static __must_check inline bool may_use_simd(void) {
	return false;
}

#endif /* ! CONFIG_KERNEL_MODE_NEON */

#endif
