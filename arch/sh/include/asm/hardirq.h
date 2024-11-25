/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SH_HARDIRQ_H
#define __ASM_SH_HARDIRQ_H

<<<<<<< HEAD
#include <linux/threads.h>
#include <linux/irq.h>

typedef struct {
	unsigned int __softirq_pending;
	unsigned int __nmi_count;		/* arch dependent */
} ____cacheline_aligned irq_cpustat_t;

#include <linux/irq_cpustat.h>	/* Standard mappings for irq_cpustat_t above */

extern void ack_bad_irq(unsigned int irq);
=======
extern void ack_bad_irq(unsigned int irq);
#define ack_bad_irq ack_bad_irq
#define ARCH_WANTS_NMI_IRQSTAT

#include <asm-generic/hardirq.h>
>>>>>>> upstream/android-13

#endif /* __ASM_SH_HARDIRQ_H */
