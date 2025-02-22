/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_MACH_LOONGSON64_IRQ_H_
#define __ASM_MACH_LOONGSON64_IRQ_H_

<<<<<<< HEAD
#include <boot_param.h>

#ifdef CONFIG_CPU_LOONGSON3

/* cpu core interrupt numbers */
#define MIPS_CPU_IRQ_BASE 56

#define LOONGSON_UART_IRQ   (MIPS_CPU_IRQ_BASE + 2) /* UART */
#define LOONGSON_BRIDGE_IRQ (MIPS_CPU_IRQ_BASE + 3) /* CASCADE */
#define LOONGSON_TIMER_IRQ  (MIPS_CPU_IRQ_BASE + 7) /* CPU Timer */

#define LOONGSON_HT1_CFG_BASE		loongson_sysconf.ht_control_base
#define LOONGSON_HT1_INT_VECTOR_BASE	(LOONGSON_HT1_CFG_BASE + 0x80)
#define LOONGSON_HT1_INT_EN_BASE	(LOONGSON_HT1_CFG_BASE + 0xa0)
#define LOONGSON_HT1_INT_VECTOR(n)	\
		LOONGSON3_REG32(LOONGSON_HT1_INT_VECTOR_BASE, 4 * (n))
#define LOONGSON_HT1_INTN_EN(n)		\
		LOONGSON3_REG32(LOONGSON_HT1_INT_EN_BASE, 4 * (n))

#define LOONGSON_INT_ROUTER_OFFSET	0x1400
#define LOONGSON_INT_ROUTER_INTEN	\
	  LOONGSON3_REG32(LOONGSON3_REG_BASE, LOONGSON_INT_ROUTER_OFFSET + 0x24)
#define LOONGSON_INT_ROUTER_INTENSET	\
	  LOONGSON3_REG32(LOONGSON3_REG_BASE, LOONGSON_INT_ROUTER_OFFSET + 0x28)
#define LOONGSON_INT_ROUTER_INTENCLR	\
	  LOONGSON3_REG32(LOONGSON3_REG_BASE, LOONGSON_INT_ROUTER_OFFSET + 0x2c)
#define LOONGSON_INT_ROUTER_ENTRY(n)	\
	  LOONGSON3_REG8(LOONGSON3_REG_BASE, LOONGSON_INT_ROUTER_OFFSET + n)
#define LOONGSON_INT_ROUTER_LPC		LOONGSON_INT_ROUTER_ENTRY(0x0a)
#define LOONGSON_INT_ROUTER_HT1(n)	LOONGSON_INT_ROUTER_ENTRY(n + 0x18)

#define LOONGSON_INT_COREx_INTy(x, y)	(1<<(x) | 1<<(y+4))	/* route to int y of core x */

#endif

extern void fixup_irqs(void);
extern void loongson3_ipi_interrupt(struct pt_regs *regs);

#include_next <irq.h>
=======
/* cpu core interrupt numbers */
#define NR_IRQS_LEGACY		16
#define NR_MIPS_CPU_IRQS	8
#define NR_MAX_CHAINED_IRQS	40 /* Chained IRQs means those not directly used by devices */
#define NR_IRQS			(NR_IRQS_LEGACY + NR_MIPS_CPU_IRQS + NR_MAX_CHAINED_IRQS + 256)

#define MIPS_CPU_IRQ_BASE 	NR_IRQS_LEGACY

#include <asm/mach-generic/irq.h>

>>>>>>> upstream/android-13
#endif /* __ASM_MACH_LOONGSON64_IRQ_H_ */
