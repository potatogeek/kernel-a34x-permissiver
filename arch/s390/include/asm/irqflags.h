/* SPDX-License-Identifier: GPL-2.0 */
/*
 *    Copyright IBM Corp. 2006, 2010
 *    Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>
 */

#ifndef __ASM_IRQFLAGS_H
#define __ASM_IRQFLAGS_H

#include <linux/types.h>

#define ARCH_IRQ_ENABLED	(3UL << (BITS_PER_LONG - 8))

/* store then OR system mask. */
#define __arch_local_irq_stosm(__or)					\
({									\
	unsigned long __mask;						\
	asm volatile(							\
		"	stosm	%0,%1"					\
		: "=Q" (__mask) : "i" (__or) : "memory");		\
	__mask;								\
})

/* store then AND system mask. */
#define __arch_local_irq_stnsm(__and)					\
({									\
	unsigned long __mask;						\
	asm volatile(							\
		"	stnsm	%0,%1"					\
		: "=Q" (__mask) : "i" (__and) : "memory");		\
	__mask;								\
})

/* set system mask. */
<<<<<<< HEAD
static inline notrace void __arch_local_irq_ssm(unsigned long flags)
=======
static __always_inline void __arch_local_irq_ssm(unsigned long flags)
>>>>>>> upstream/android-13
{
	asm volatile("ssm   %0" : : "Q" (flags) : "memory");
}

<<<<<<< HEAD
static inline notrace unsigned long arch_local_save_flags(void)
=======
static __always_inline unsigned long arch_local_save_flags(void)
>>>>>>> upstream/android-13
{
	return __arch_local_irq_stnsm(0xff);
}

<<<<<<< HEAD
static inline notrace unsigned long arch_local_irq_save(void)
=======
static __always_inline unsigned long arch_local_irq_save(void)
>>>>>>> upstream/android-13
{
	return __arch_local_irq_stnsm(0xfc);
}

<<<<<<< HEAD
static inline notrace void arch_local_irq_disable(void)
=======
static __always_inline void arch_local_irq_disable(void)
>>>>>>> upstream/android-13
{
	arch_local_irq_save();
}

<<<<<<< HEAD
static inline notrace void arch_local_irq_enable(void)
=======
static __always_inline void arch_local_irq_enable(void)
>>>>>>> upstream/android-13
{
	__arch_local_irq_stosm(0x03);
}

/* This only restores external and I/O interrupt state */
<<<<<<< HEAD
static inline notrace void arch_local_irq_restore(unsigned long flags)
=======
static __always_inline void arch_local_irq_restore(unsigned long flags)
>>>>>>> upstream/android-13
{
	/* only disabled->disabled and disabled->enabled is valid */
	if (flags & ARCH_IRQ_ENABLED)
		arch_local_irq_enable();
}

<<<<<<< HEAD
static inline notrace bool arch_irqs_disabled_flags(unsigned long flags)
=======
static __always_inline bool arch_irqs_disabled_flags(unsigned long flags)
>>>>>>> upstream/android-13
{
	return !(flags & ARCH_IRQ_ENABLED);
}

<<<<<<< HEAD
static inline notrace bool arch_irqs_disabled(void)
=======
static __always_inline bool arch_irqs_disabled(void)
>>>>>>> upstream/android-13
{
	return arch_irqs_disabled_flags(arch_local_save_flags());
}

#endif /* __ASM_IRQFLAGS_H */
