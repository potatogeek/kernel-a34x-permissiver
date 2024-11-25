/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_UM_HARDIRQ_H
#define __ASM_UM_HARDIRQ_H

<<<<<<< HEAD
#include <linux/cache.h>
#include <linux/threads.h>

typedef struct {
	unsigned int __softirq_pending;
} ____cacheline_aligned irq_cpustat_t;

#include <linux/irq_cpustat.h>	/* Standard mappings for irq_cpustat_t above */
#include <linux/irq.h>

#ifndef ack_bad_irq
static inline void ack_bad_irq(unsigned int irq)
{
	printk(KERN_CRIT "unexpected IRQ trap at vector %02x\n", irq);
}
#endif
=======
#include <asm-generic/hardirq.h>
>>>>>>> upstream/android-13

#define __ARCH_IRQ_EXIT_IRQS_DISABLED 1

#endif /* __ASM_UM_HARDIRQ_H */
