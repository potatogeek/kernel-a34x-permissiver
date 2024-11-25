/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_IRQ_WORK_H
#define __ASM_IRQ_WORK_H

<<<<<<< HEAD
#include <asm/smp.h>

static inline bool arch_irq_work_has_interrupt(void)
{
	return !!__smp_cross_call;
=======
extern void arch_irq_work_raise(void);

static inline bool arch_irq_work_has_interrupt(void)
{
	return true;
>>>>>>> upstream/android-13
}

#endif /* __ASM_IRQ_WORK_H */
