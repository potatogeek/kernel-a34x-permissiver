/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __UM_IRQFLAGS_H
#define __UM_IRQFLAGS_H

<<<<<<< HEAD
extern int get_signals(void);
extern int set_signals(int enable);
extern void block_signals(void);
extern void unblock_signals(void);
=======
extern int signals_enabled;
int um_set_signals(int enable);
void block_signals(void);
void unblock_signals(void);
>>>>>>> upstream/android-13

#define arch_local_save_flags arch_local_save_flags
static inline unsigned long arch_local_save_flags(void)
{
<<<<<<< HEAD
	return get_signals();
=======
	return signals_enabled;
>>>>>>> upstream/android-13
}

#define arch_local_irq_restore arch_local_irq_restore
static inline void arch_local_irq_restore(unsigned long flags)
{
<<<<<<< HEAD
	set_signals(flags);
=======
	um_set_signals(flags);
>>>>>>> upstream/android-13
}

#define arch_local_irq_enable arch_local_irq_enable
static inline void arch_local_irq_enable(void)
{
	unblock_signals();
}

#define arch_local_irq_disable arch_local_irq_disable
static inline void arch_local_irq_disable(void)
{
	block_signals();
}

#define ARCH_IRQ_DISABLED	0
<<<<<<< HEAD
#define ARCh_IRQ_ENABLED	(SIGIO|SIGVTALRM)
=======
>>>>>>> upstream/android-13

#include <asm-generic/irqflags.h>

#endif
