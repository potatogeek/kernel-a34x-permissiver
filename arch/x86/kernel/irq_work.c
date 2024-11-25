// SPDX-License-Identifier: GPL-2.0
/*
 * x86 specific code for irq_work
 *
 * Copyright (C) 2010 Red Hat, Inc., Peter Zijlstra
 */

#include <linux/kernel.h>
#include <linux/irq_work.h>
#include <linux/hardirq.h>
#include <asm/apic.h>
<<<<<<< HEAD
=======
#include <asm/idtentry.h>
>>>>>>> upstream/android-13
#include <asm/trace/irq_vectors.h>
#include <linux/interrupt.h>

#ifdef CONFIG_X86_LOCAL_APIC
<<<<<<< HEAD
__visible void __irq_entry smp_irq_work_interrupt(struct pt_regs *regs)
{
	ipi_entering_ack_irq();
=======
DEFINE_IDTENTRY_SYSVEC(sysvec_irq_work)
{
	ack_APIC_irq();
>>>>>>> upstream/android-13
	trace_irq_work_entry(IRQ_WORK_VECTOR);
	inc_irq_stat(apic_irq_work_irqs);
	irq_work_run();
	trace_irq_work_exit(IRQ_WORK_VECTOR);
<<<<<<< HEAD
	exiting_irq();
=======
>>>>>>> upstream/android-13
}

void arch_irq_work_raise(void)
{
	if (!arch_irq_work_has_interrupt())
		return;

	apic->send_IPI_self(IRQ_WORK_VECTOR);
	apic_wait_icr_idle();
}
#endif
