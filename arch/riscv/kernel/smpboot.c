<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * SMP initialisation and IPI support
 * Based on arch/arm64/kernel/smp.c
 *
 * Copyright (C) 2012 ARM Ltd.
 * Copyright (C) 2015 Regents of the University of California
 * Copyright (C) 2017 SiFive
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

=======
 */

#include <linux/arch_topology.h>
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/kernel_stat.h>
#include <linux/notifier.h>
#include <linux/cpu.h>
#include <linux/percpu.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/irq.h>
#include <linux/of.h>
#include <linux/sched/task_stack.h>
<<<<<<< HEAD
#include <asm/irq.h>
#include <asm/mmu_context.h>
#include <asm/tlbflush.h>
#include <asm/sections.h>
#include <asm/sbi.h>

void *__cpu_up_stack_pointer[NR_CPUS];
void *__cpu_up_task_pointer[NR_CPUS];

void __init smp_prepare_boot_cpu(void)
{
=======
#include <linux/sched/mm.h>
#include <asm/cpu_ops.h>
#include <asm/irq.h>
#include <asm/mmu_context.h>
#include <asm/numa.h>
#include <asm/tlbflush.h>
#include <asm/sections.h>
#include <asm/sbi.h>
#include <asm/smp.h>
#include <asm/alternative.h>

#include "head.h"

static DECLARE_COMPLETION(cpu_running);

void __init smp_prepare_boot_cpu(void)
{
	init_cpu_topology();
#ifdef CONFIG_RISCV_ERRATA_ALTERNATIVE
	apply_boot_alternatives();
#endif
>>>>>>> upstream/android-13
}

void __init smp_prepare_cpus(unsigned int max_cpus)
{
<<<<<<< HEAD
=======
	int cpuid;
	int ret;
	unsigned int curr_cpuid;

	curr_cpuid = smp_processor_id();
	numa_store_cpu_info(curr_cpuid);
	numa_add_cpu(curr_cpuid);

	/* This covers non-smp usecase mandated by "nosmp" option */
	if (max_cpus == 0)
		return;

	for_each_possible_cpu(cpuid) {
		if (cpuid == curr_cpuid)
			continue;
		if (cpu_ops[cpuid]->cpu_prepare) {
			ret = cpu_ops[cpuid]->cpu_prepare(cpuid);
			if (ret)
				continue;
		}
		set_cpu_present(cpuid, true);
		numa_store_cpu_info(cpuid);
	}
>>>>>>> upstream/android-13
}

void __init setup_smp(void)
{
<<<<<<< HEAD
	struct device_node *dn = NULL;
	int hart, im_okay_therefore_i_am = 0;

	while ((dn = of_find_node_by_type(dn, "cpu"))) {
		hart = riscv_of_processor_hart(dn);
		if (hart >= 0) {
			set_cpu_possible(hart, true);
			set_cpu_present(hart, true);
			if (hart == smp_processor_id()) {
				BUG_ON(im_okay_therefore_i_am);
				im_okay_therefore_i_am = 1;
			}
		}
	}

	BUG_ON(!im_okay_therefore_i_am);
=======
	struct device_node *dn;
	int hart;
	bool found_boot_cpu = false;
	int cpuid = 1;

	cpu_set_ops(0);

	for_each_of_cpu_node(dn) {
		hart = riscv_of_processor_hartid(dn);
		if (hart < 0)
			continue;

		if (hart == cpuid_to_hartid_map(0)) {
			BUG_ON(found_boot_cpu);
			found_boot_cpu = 1;
			early_map_cpu_to_node(0, of_node_to_nid(dn));
			continue;
		}
		if (cpuid >= NR_CPUS) {
			pr_warn("Invalid cpuid [%d] for hartid [%d]\n",
				cpuid, hart);
			break;
		}

		cpuid_to_hartid_map(cpuid) = hart;
		early_map_cpu_to_node(cpuid, of_node_to_nid(dn));
		cpuid++;
	}

	BUG_ON(!found_boot_cpu);

	if (cpuid > nr_cpu_ids)
		pr_warn("Total number of cpus [%d] is greater than nr_cpus option value [%d]\n",
			cpuid, nr_cpu_ids);

	for (cpuid = 1; cpuid < nr_cpu_ids; cpuid++) {
		if (cpuid_to_hartid_map(cpuid) != INVALID_HARTID) {
			cpu_set_ops(cpuid);
			set_cpu_possible(cpuid, true);
		}
	}
}

static int start_secondary_cpu(int cpu, struct task_struct *tidle)
{
	if (cpu_ops[cpu]->cpu_start)
		return cpu_ops[cpu]->cpu_start(cpu, tidle);

	return -EOPNOTSUPP;
>>>>>>> upstream/android-13
}

int __cpu_up(unsigned int cpu, struct task_struct *tidle)
{
<<<<<<< HEAD
	tidle->thread_info.cpu = cpu;

	/*
	 * On RISC-V systems, all harts boot on their own accord.  Our _start
	 * selects the first hart to boot the kernel and causes the remainder
	 * of the harts to spin in a loop waiting for their stack pointer to be
	 * setup by that main hart.  Writing __cpu_up_stack_pointer signals to
	 * the spinning harts that they can continue the boot process.
	 */
	smp_mb();
	__cpu_up_stack_pointer[cpu] = task_stack_page(tidle) + THREAD_SIZE;
	__cpu_up_task_pointer[cpu] = tidle;

	while (!cpu_online(cpu))
		cpu_relax();

	return 0;
=======
	int ret = 0;
	tidle->thread_info.cpu = cpu;

	ret = start_secondary_cpu(cpu, tidle);
	if (!ret) {
		wait_for_completion_timeout(&cpu_running,
					    msecs_to_jiffies(1000));

		if (!cpu_online(cpu)) {
			pr_crit("CPU%u: failed to come online\n", cpu);
			ret = -EIO;
		}
	} else {
		pr_crit("CPU%u: failed to start\n", cpu);
	}

	return ret;
>>>>>>> upstream/android-13
}

void __init smp_cpus_done(unsigned int max_cpus)
{
}

/*
 * C entry point for a secondary processor.
 */
<<<<<<< HEAD
asmlinkage void __init smp_callin(void)
{
	struct mm_struct *mm = &init_mm;

	/* All kernel threads share the same mm context.  */
	atomic_inc(&mm->mm_count);
	current->active_mm = mm;

	trap_init();
	notify_cpu_starting(smp_processor_id());
	set_cpu_online(smp_processor_id(), 1);
	local_flush_tlb_all();
	local_irq_enable();
	preempt_disable();
=======
asmlinkage __visible void smp_callin(void)
{
	struct mm_struct *mm = &init_mm;
	unsigned int curr_cpuid = smp_processor_id();

	riscv_clear_ipi();

	/* All kernel threads share the same mm context.  */
	mmgrab(mm);
	current->active_mm = mm;

	notify_cpu_starting(curr_cpuid);
	numa_add_cpu(curr_cpuid);
	update_siblings_masks(curr_cpuid);
	set_cpu_online(curr_cpuid, 1);

	/*
	 * Remote TLB flushes are ignored while the CPU is offline, so emit
	 * a local TLB flush right now just in case.
	 */
	local_flush_tlb_all();
	complete(&cpu_running);
	/*
	 * Disable preemption before enabling interrupts, so we don't try to
	 * schedule a CPU that hasn't actually started yet.
	 */
	local_irq_enable();
>>>>>>> upstream/android-13
	cpu_startup_entry(CPUHP_AP_ONLINE_IDLE);
}
