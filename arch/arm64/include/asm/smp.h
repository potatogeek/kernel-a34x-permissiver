<<<<<<< HEAD
/*
 * Copyright (C) 2012 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 ARM Ltd.
>>>>>>> upstream/android-13
 */
#ifndef __ASM_SMP_H
#define __ASM_SMP_H

<<<<<<< HEAD
/* Values for secondary_data.status */

#define CPU_MMU_OFF		(-1)
#define CPU_BOOT_SUCCESS	(0)
/* The cpu invoked ops->cpu_die, synchronise it with cpu_kill */
#define CPU_KILL_ME		(1)
/* The cpu couldn't die gracefully and is looping in the kernel */
#define CPU_STUCK_IN_KERNEL	(2)
/* Fatal system error detected by secondary CPU, crash the system */
#define CPU_PANIC_KERNEL	(3)
=======
#include <linux/const.h>

/* Values for secondary_data.status */
#define CPU_STUCK_REASON_SHIFT		(8)
#define CPU_BOOT_STATUS_MASK		((UL(1) << CPU_STUCK_REASON_SHIFT) - 1)

#define CPU_MMU_OFF			(-1)
#define CPU_BOOT_SUCCESS		(0)
/* The cpu invoked ops->cpu_die, synchronise it with cpu_kill */
#define CPU_KILL_ME			(1)
/* The cpu couldn't die gracefully and is looping in the kernel */
#define CPU_STUCK_IN_KERNEL		(2)
/* Fatal system error detected by secondary CPU, crash the system */
#define CPU_PANIC_KERNEL		(3)

#define CPU_STUCK_REASON_52_BIT_VA	(UL(1) << CPU_STUCK_REASON_SHIFT)
#define CPU_STUCK_REASON_NO_GRAN	(UL(2) << CPU_STUCK_REASON_SHIFT)
>>>>>>> upstream/android-13

#ifndef __ASSEMBLY__

#include <asm/percpu.h>

#include <linux/threads.h>
#include <linux/cpumask.h>
#include <linux/thread_info.h>

DECLARE_PER_CPU_READ_MOSTLY(int, cpu_number);

/*
 * We don't use this_cpu_read(cpu_number) as that has implicit writes to
 * preempt_count, and associated (compiler) barriers, that we'd like to avoid
 * the expense of. If we're preemptible, the value can be stale at use anyway.
 * And we can't use this_cpu_ptr() either, as that winds up recursing back
 * here under CONFIG_DEBUG_PREEMPT=y.
 */
#define raw_smp_processor_id() (*raw_cpu_ptr(&cpu_number))

<<<<<<< HEAD
struct seq_file;

/*
 * generate IPI list text
 */
extern void show_ipi_list(struct seq_file *p, int prec);

/*
 * Called from C code, this handles an IPI.
 */
extern void handle_IPI(int ipinr, struct pt_regs *regs);

/*
=======
/*
 * Logical CPU mapping.
 */
extern u64 __cpu_logical_map[NR_CPUS];
extern u64 cpu_logical_map(unsigned int cpu);

static inline void set_cpu_logical_map(unsigned int cpu, u64 hwid)
{
	__cpu_logical_map[cpu] = hwid;
}

struct seq_file;

/*
>>>>>>> upstream/android-13
 * Discover the set of possible CPUs and determine their
 * SMP operations.
 */
extern void smp_init_cpus(void);

/*
<<<<<<< HEAD
 * Provide a function to raise an IPI cross call on CPUs in callmap.
 */
extern void set_smp_cross_call(void (*)(const struct cpumask *, unsigned int));

extern void (*__smp_cross_call)(const struct cpumask *, unsigned int);

/*
 * Provide a function to set a callback function pointer for updating the ipi
 * history.
 */
extern void set_update_ipi_history_callback(void (*fn)(int));
=======
 * Register IPI interrupts with the arch SMP code
 */
extern void set_smp_ipi_range(int ipi_base, int nr_ipi);
>>>>>>> upstream/android-13

/*
 * Called from the secondary holding pen, this is the secondary CPU entry point.
 */
asmlinkage void secondary_start_kernel(void);

/*
 * Initial data for bringing up a secondary CPU.
<<<<<<< HEAD
 * @stack  - sp for the secondary CPU
=======
>>>>>>> upstream/android-13
 * @status - Result passed back from the secondary CPU to
 *           indicate failure.
 */
struct secondary_data {
<<<<<<< HEAD
	void *stack;
=======
>>>>>>> upstream/android-13
	struct task_struct *task;
	long status;
};

extern struct secondary_data secondary_data;
extern long __early_cpu_boot_status;
extern void secondary_entry(void);

extern void arch_send_call_function_single_ipi(int cpu);
extern void arch_send_call_function_ipi_mask(const struct cpumask *mask);
<<<<<<< HEAD
=======
extern int nr_ipi_get(void);
extern struct irq_desc **ipi_desc_get(void);
>>>>>>> upstream/android-13

#ifdef CONFIG_ARM64_ACPI_PARKING_PROTOCOL
extern void arch_send_wakeup_ipi_mask(const struct cpumask *mask);
#else
static inline void arch_send_wakeup_ipi_mask(const struct cpumask *mask)
{
	BUILD_BUG();
}
#endif

extern int __cpu_disable(void);

extern void __cpu_die(unsigned int cpu);
extern void cpu_die(void);
extern void cpu_die_early(void);

static inline void cpu_park_loop(void)
{
	for (;;) {
		wfe();
		wfi();
	}
}

static inline void update_cpu_boot_status(int val)
{
	WRITE_ONCE(secondary_data.status, val);
	/* Ensure the visibility of the status update */
	dsb(ishst);
}

/*
 * The calling secondary CPU has detected serious configuration mismatch,
 * which calls for a kernel panic. Update the boot status and park the calling
 * CPU.
 */
static inline void cpu_panic_kernel(void)
{
	update_cpu_boot_status(CPU_PANIC_KERNEL);
	cpu_park_loop();
}

/*
 * If a secondary CPU enters the kernel but fails to come online,
 * (e.g. due to mismatched features), and cannot exit the kernel,
 * we increment cpus_stuck_in_kernel and leave the CPU in a
 * quiesecent loop within the kernel text. The memory containing
 * this loop must not be re-used for anything else as the 'stuck'
 * core is executing it.
 *
 * This function is used to inhibit features like kexec and hibernate.
 */
bool cpus_are_stuck_in_kernel(void);

extern void crash_smp_send_stop(void);
extern bool smp_crash_stop_failed(void);
<<<<<<< HEAD
=======
extern void panic_smp_self_stop(void);
>>>>>>> upstream/android-13

#endif /* ifndef __ASSEMBLY__ */

#endif /* ifndef __ASM_SMP_H */
