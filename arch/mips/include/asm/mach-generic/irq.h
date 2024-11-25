/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2003 by Ralf Baechle
 */
#ifndef __ASM_MACH_GENERIC_IRQ_H
#define __ASM_MACH_GENERIC_IRQ_H

#ifndef NR_IRQS
<<<<<<< HEAD
#define NR_IRQS 128
=======
#define NR_IRQS 256
>>>>>>> upstream/android-13
#endif

#ifdef CONFIG_I8259
#ifndef I8259A_IRQ_BASE
#define I8259A_IRQ_BASE 0
#endif
#endif

#ifdef CONFIG_IRQ_MIPS_CPU

#ifndef MIPS_CPU_IRQ_BASE
#ifdef CONFIG_I8259
#define MIPS_CPU_IRQ_BASE 16
#else
#define MIPS_CPU_IRQ_BASE 0
#endif /* CONFIG_I8259 */
#endif

<<<<<<< HEAD
#ifdef CONFIG_IRQ_CPU_RM7K
#ifndef RM7K_CPU_IRQ_BASE
#define RM7K_CPU_IRQ_BASE (MIPS_CPU_IRQ_BASE+8)
#endif
#endif

#endif /* CONFIG_IRQ_MIPS_CPU */

#ifdef CONFIG_MIPS_GIC
#ifndef MIPS_GIC_IRQ_BASE
#define MIPS_GIC_IRQ_BASE (MIPS_CPU_IRQ_BASE + 8)
#endif
#endif /* CONFIG_MIPS_GIC */

=======
#endif /* CONFIG_IRQ_MIPS_CPU */

>>>>>>> upstream/android-13
#endif /* __ASM_MACH_GENERIC_IRQ_H */
