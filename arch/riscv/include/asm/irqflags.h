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


#ifndef _ASM_RISCV_IRQFLAGS_H
#define _ASM_RISCV_IRQFLAGS_H

#include <asm/processor.h>
#include <asm/csr.h>

/* read interrupt enabled status */
static inline unsigned long arch_local_save_flags(void)
{
<<<<<<< HEAD
	return csr_read(sstatus);
=======
	return csr_read(CSR_STATUS);
>>>>>>> upstream/android-13
}

/* unconditionally enable interrupts */
static inline void arch_local_irq_enable(void)
{
<<<<<<< HEAD
	csr_set(sstatus, SR_SIE);
=======
	csr_set(CSR_STATUS, SR_IE);
>>>>>>> upstream/android-13
}

/* unconditionally disable interrupts */
static inline void arch_local_irq_disable(void)
{
<<<<<<< HEAD
	csr_clear(sstatus, SR_SIE);
=======
	csr_clear(CSR_STATUS, SR_IE);
>>>>>>> upstream/android-13
}

/* get status and disable interrupts */
static inline unsigned long arch_local_irq_save(void)
{
<<<<<<< HEAD
	return csr_read_clear(sstatus, SR_SIE);
=======
	return csr_read_clear(CSR_STATUS, SR_IE);
>>>>>>> upstream/android-13
}

/* test flags */
static inline int arch_irqs_disabled_flags(unsigned long flags)
{
<<<<<<< HEAD
	return !(flags & SR_SIE);
=======
	return !(flags & SR_IE);
>>>>>>> upstream/android-13
}

/* test hardware interrupt enable bit */
static inline int arch_irqs_disabled(void)
{
	return arch_irqs_disabled_flags(arch_local_save_flags());
}

/* set interrupt enabled status */
static inline void arch_local_irq_restore(unsigned long flags)
{
<<<<<<< HEAD
	csr_set(sstatus, flags & SR_SIE);
=======
	csr_set(CSR_STATUS, flags & SR_IE);
>>>>>>> upstream/android-13
}

#endif /* _ASM_RISCV_IRQFLAGS_H */
