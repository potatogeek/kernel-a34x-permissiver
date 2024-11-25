<<<<<<< HEAD
/*
 * Copyright (C) 2012 Regents of the University of California
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 Regents of the University of California
>>>>>>> upstream/android-13
 */

#ifndef _ASM_RISCV_SMP_H
#define _ASM_RISCV_SMP_H

<<<<<<< HEAD
/* This both needs asm-offsets.h and is used when generating it. */
#ifndef GENERATING_ASM_OFFSETS
#include <asm/asm-offsets.h>
#endif

#include <linux/cpumask.h>
#include <linux/irqreturn.h>

#ifdef CONFIG_SMP
=======
#include <linux/cpumask.h>
#include <linux/irqreturn.h>
#include <linux/thread_info.h>

#define INVALID_HARTID ULONG_MAX

struct seq_file;
extern unsigned long boot_cpu_hartid;

struct riscv_ipi_ops {
	void (*ipi_inject)(const struct cpumask *target);
	void (*ipi_clear)(void);
};

#ifdef CONFIG_SMP
/*
 * Mapping between linux logical cpu index and hartid.
 */
extern unsigned long __cpuid_to_hartid_map[NR_CPUS];
#define cpuid_to_hartid_map(cpu)    __cpuid_to_hartid_map[cpu]

/* print IPI stats */
void show_ipi_stats(struct seq_file *p, int prec);
>>>>>>> upstream/android-13

/* SMP initialization hook for setup_arch */
void __init setup_smp(void);

<<<<<<< HEAD
=======
/* Called from C code, this handles an IPI. */
void handle_IPI(struct pt_regs *regs);

>>>>>>> upstream/android-13
/* Hook for the generic smp_call_function_many() routine. */
void arch_send_call_function_ipi_mask(struct cpumask *mask);

/* Hook for the generic smp_call_function_single() routine. */
void arch_send_call_function_single_ipi(int cpu);

<<<<<<< HEAD
/*
 * This is particularly ugly: it appears we can't actually get the definition
 * of task_struct here, but we need access to the CPU this task is running on.
 * Instead of using C we're using asm-offsets.h to get the current processor
 * ID.
 */
#define raw_smp_processor_id() (*((int*)((char*)get_current() + TASK_TI_CPU)))

#endif /* CONFIG_SMP */

=======
int riscv_hartid_to_cpuid(int hartid);

/* Set custom IPI operations */
void riscv_set_ipi_ops(const struct riscv_ipi_ops *ops);

/* Clear IPI for current CPU */
void riscv_clear_ipi(void);

/* Secondary hart entry */
asmlinkage void smp_callin(void);

/*
 * Obtains the hart ID of the currently executing task.  This relies on
 * THREAD_INFO_IN_TASK, but we define that unconditionally.
 */
#define raw_smp_processor_id() (current_thread_info()->cpu)

#if defined CONFIG_HOTPLUG_CPU
int __cpu_disable(void);
void __cpu_die(unsigned int cpu);
void cpu_stop(void);
#else
#endif /* CONFIG_HOTPLUG_CPU */

#else

static inline void show_ipi_stats(struct seq_file *p, int prec)
{
}

static inline int riscv_hartid_to_cpuid(int hartid)
{
	if (hartid == boot_cpu_hartid)
		return 0;

	return -1;
}
static inline unsigned long cpuid_to_hartid_map(int cpu)
{
	return boot_cpu_hartid;
}

static inline void riscv_set_ipi_ops(const struct riscv_ipi_ops *ops)
{
}

static inline void riscv_clear_ipi(void)
{
}

#endif /* CONFIG_SMP */

void riscv_cpuid_to_hartid_mask(const struct cpumask *in, struct cpumask *out);

#if defined(CONFIG_HOTPLUG_CPU) && (CONFIG_SMP)
bool cpu_has_hotplug(unsigned int cpu);
#else
static inline bool cpu_has_hotplug(unsigned int cpu)
{
	return false;
}
#endif

>>>>>>> upstream/android-13
#endif /* _ASM_RISCV_SMP_H */
