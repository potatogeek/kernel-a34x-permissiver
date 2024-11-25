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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/interrupt.h>
#include <linux/smp.h>
#include <linux/sched.h>
=======
 */

#include <linux/cpu.h>
#include <linux/clockchips.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/profile.h>
#include <linux/smp.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
#include <linux/irq_work.h>
>>>>>>> upstream/android-13

#include <asm/sbi.h>
#include <asm/tlbflush.h>
#include <asm/cacheflush.h>

<<<<<<< HEAD
/* A collection of single bit ipi messages.  */
static struct {
	unsigned long bits ____cacheline_aligned;
} ipi_data[NR_CPUS] __cacheline_aligned;

enum ipi_message_type {
	IPI_RESCHEDULE,
	IPI_CALL_FUNC,
	IPI_MAX
};

=======
enum ipi_message_type {
	IPI_RESCHEDULE,
	IPI_CALL_FUNC,
	IPI_CPU_STOP,
	IPI_IRQ_WORK,
	IPI_TIMER,
	IPI_MAX
};

unsigned long __cpuid_to_hartid_map[NR_CPUS] __ro_after_init = {
	[0 ... NR_CPUS-1] = INVALID_HARTID
};

void __init smp_setup_processor_id(void)
{
	cpuid_to_hartid_map(0) = boot_cpu_hartid;
}

/* A collection of single bit ipi messages.  */
static struct {
	unsigned long stats[IPI_MAX] ____cacheline_aligned;
	unsigned long bits ____cacheline_aligned;
} ipi_data[NR_CPUS] __cacheline_aligned;

int riscv_hartid_to_cpuid(int hartid)
{
	int i;

	for (i = 0; i < NR_CPUS; i++)
		if (cpuid_to_hartid_map(i) == hartid)
			return i;

	pr_err("Couldn't find cpu id for hartid [%d]\n", hartid);
	return -ENOENT;
}

bool arch_match_cpu_phys_id(int cpu, u64 phys_id)
{
	return phys_id == cpuid_to_hartid_map(cpu);
}
>>>>>>> upstream/android-13

/* Unsupported */
int setup_profiling_timer(unsigned int multiplier)
{
	return -EINVAL;
}

<<<<<<< HEAD
void riscv_software_interrupt(void)
{
	unsigned long *pending_ipis = &ipi_data[smp_processor_id()].bits;

	/* Clear pending IPI */
	csr_clear(sip, SIE_SSIE);
=======
static void ipi_stop(void)
{
	set_cpu_online(smp_processor_id(), false);
	while (1)
		wait_for_interrupt();
}

static const struct riscv_ipi_ops *ipi_ops __ro_after_init;

void riscv_set_ipi_ops(const struct riscv_ipi_ops *ops)
{
	ipi_ops = ops;
}
EXPORT_SYMBOL_GPL(riscv_set_ipi_ops);

void riscv_clear_ipi(void)
{
	if (ipi_ops && ipi_ops->ipi_clear)
		ipi_ops->ipi_clear();

	csr_clear(CSR_IP, IE_SIE);
}
EXPORT_SYMBOL_GPL(riscv_clear_ipi);

static void send_ipi_mask(const struct cpumask *mask, enum ipi_message_type op)
{
	int cpu;

	smp_mb__before_atomic();
	for_each_cpu(cpu, mask)
		set_bit(op, &ipi_data[cpu].bits);
	smp_mb__after_atomic();

	if (ipi_ops && ipi_ops->ipi_inject)
		ipi_ops->ipi_inject(mask);
	else
		pr_warn("SMP: IPI inject method not available\n");
}

static void send_ipi_single(int cpu, enum ipi_message_type op)
{
	smp_mb__before_atomic();
	set_bit(op, &ipi_data[cpu].bits);
	smp_mb__after_atomic();

	if (ipi_ops && ipi_ops->ipi_inject)
		ipi_ops->ipi_inject(cpumask_of(cpu));
	else
		pr_warn("SMP: IPI inject method not available\n");
}

#ifdef CONFIG_IRQ_WORK
void arch_irq_work_raise(void)
{
	send_ipi_single(smp_processor_id(), IPI_IRQ_WORK);
}
#endif

void handle_IPI(struct pt_regs *regs)
{
	struct pt_regs *old_regs = set_irq_regs(regs);
	unsigned long *pending_ipis = &ipi_data[smp_processor_id()].bits;
	unsigned long *stats = ipi_data[smp_processor_id()].stats;

	irq_enter();

	riscv_clear_ipi();
>>>>>>> upstream/android-13

	while (true) {
		unsigned long ops;

		/* Order bit clearing and data access. */
		mb();

		ops = xchg(pending_ipis, 0);
		if (ops == 0)
<<<<<<< HEAD
			return;

		if (ops & (1 << IPI_RESCHEDULE))
			scheduler_ipi();

		if (ops & (1 << IPI_CALL_FUNC))
			generic_smp_call_function_interrupt();

=======
			goto done;

		if (ops & (1 << IPI_RESCHEDULE)) {
			stats[IPI_RESCHEDULE]++;
			scheduler_ipi();
		}

		if (ops & (1 << IPI_CALL_FUNC)) {
			stats[IPI_CALL_FUNC]++;
			generic_smp_call_function_interrupt();
		}

		if (ops & (1 << IPI_CPU_STOP)) {
			stats[IPI_CPU_STOP]++;
			ipi_stop();
		}

		if (ops & (1 << IPI_IRQ_WORK)) {
			stats[IPI_IRQ_WORK]++;
			irq_work_run();
		}

#ifdef CONFIG_GENERIC_CLOCKEVENTS_BROADCAST
		if (ops & (1 << IPI_TIMER)) {
			stats[IPI_TIMER]++;
			tick_receive_broadcast();
		}
#endif
>>>>>>> upstream/android-13
		BUG_ON((ops >> IPI_MAX) != 0);

		/* Order data access and bit testing. */
		mb();
	}
<<<<<<< HEAD
}

static void
send_ipi_message(const struct cpumask *to_whom, enum ipi_message_type operation)
{
	int i;

	mb();
	for_each_cpu(i, to_whom)
		set_bit(operation, &ipi_data[i].bits);

	mb();
	sbi_send_ipi(cpumask_bits(to_whom));
=======

done:
	irq_exit();
	set_irq_regs(old_regs);
}

static const char * const ipi_names[] = {
	[IPI_RESCHEDULE]	= "Rescheduling interrupts",
	[IPI_CALL_FUNC]		= "Function call interrupts",
	[IPI_CPU_STOP]		= "CPU stop interrupts",
	[IPI_IRQ_WORK]		= "IRQ work interrupts",
	[IPI_TIMER]		= "Timer broadcast interrupts",
};

void show_ipi_stats(struct seq_file *p, int prec)
{
	unsigned int cpu, i;

	for (i = 0; i < IPI_MAX; i++) {
		seq_printf(p, "%*s%u:%s", prec - 1, "IPI", i,
			   prec >= 4 ? " " : "");
		for_each_online_cpu(cpu)
			seq_printf(p, "%10lu ", ipi_data[cpu].stats[i]);
		seq_printf(p, " %s\n", ipi_names[i]);
	}
>>>>>>> upstream/android-13
}

void arch_send_call_function_ipi_mask(struct cpumask *mask)
{
<<<<<<< HEAD
	send_ipi_message(mask, IPI_CALL_FUNC);
=======
	send_ipi_mask(mask, IPI_CALL_FUNC);
>>>>>>> upstream/android-13
}

void arch_send_call_function_single_ipi(int cpu)
{
<<<<<<< HEAD
	send_ipi_message(cpumask_of(cpu), IPI_CALL_FUNC);
}

static void ipi_stop(void *unused)
{
	while (1)
		wait_for_interrupt();
}

void smp_send_stop(void)
{
	on_each_cpu(ipi_stop, NULL, 1);
=======
	send_ipi_single(cpu, IPI_CALL_FUNC);
}

#ifdef CONFIG_GENERIC_CLOCKEVENTS_BROADCAST
void tick_broadcast(const struct cpumask *mask)
{
	send_ipi_mask(mask, IPI_TIMER);
}
#endif

void smp_send_stop(void)
{
	unsigned long timeout;

	if (num_online_cpus() > 1) {
		cpumask_t mask;

		cpumask_copy(&mask, cpu_online_mask);
		cpumask_clear_cpu(smp_processor_id(), &mask);

		if (system_state <= SYSTEM_RUNNING)
			pr_crit("SMP: stopping secondary CPUs\n");
		send_ipi_mask(&mask, IPI_CPU_STOP);
	}

	/* Wait up to one second for other CPUs to stop */
	timeout = USEC_PER_SEC;
	while (num_online_cpus() > 1 && timeout--)
		udelay(1);

	if (num_online_cpus() > 1)
		pr_warn("SMP: failed to stop secondary CPUs %*pbl\n",
			   cpumask_pr_args(cpu_online_mask));
>>>>>>> upstream/android-13
}

void smp_send_reschedule(int cpu)
{
<<<<<<< HEAD
	send_ipi_message(cpumask_of(cpu), IPI_RESCHEDULE);
}

/*
 * Performs an icache flush for the given MM context.  RISC-V has no direct
 * mechanism for instruction cache shoot downs, so instead we send an IPI that
 * informs the remote harts they need to flush their local instruction caches.
 * To avoid pathologically slow behavior in a common case (a bunch of
 * single-hart processes on a many-hart machine, ie 'make -j') we avoid the
 * IPIs for harts that are not currently executing a MM context and instead
 * schedule a deferred local instruction cache flush to be performed before
 * execution resumes on each hart.
 */
void flush_icache_mm(struct mm_struct *mm, bool local)
{
	unsigned int cpu;
	cpumask_t others, *mask;

	preempt_disable();

	/* Mark every hart's icache as needing a flush for this MM. */
	mask = &mm->context.icache_stale_mask;
	cpumask_setall(mask);
	/* Flush this hart's I$ now, and mark it as flushed. */
	cpu = smp_processor_id();
	cpumask_clear_cpu(cpu, mask);
	local_flush_icache_all();

	/*
	 * Flush the I$ of other harts concurrently executing, and mark them as
	 * flushed.
	 */
	cpumask_andnot(&others, mm_cpumask(mm), cpumask_of(cpu));
	local |= cpumask_empty(&others);
	if (mm != current->active_mm || !local)
		sbi_remote_fence_i(others.bits);
	else {
		/*
		 * It's assumed that at least one strongly ordered operation is
		 * performed on this hart between setting a hart's cpumask bit
		 * and scheduling this MM context on that hart.  Sending an SBI
		 * remote message will do this, but in the case where no
		 * messages are sent we still need to order this hart's writes
		 * with flush_icache_deferred().
		 */
		smp_mb();
	}

	preempt_enable();
}
=======
	send_ipi_single(cpu, IPI_RESCHEDULE);
}
EXPORT_SYMBOL_GPL(smp_send_reschedule);
>>>>>>> upstream/android-13
