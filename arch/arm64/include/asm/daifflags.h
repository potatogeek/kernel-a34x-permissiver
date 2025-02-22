<<<<<<< HEAD
/*
 * Copyright (C) 2017 ARM Ltd.
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
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2017 ARM Ltd.
>>>>>>> upstream/android-13
 */
#ifndef __ASM_DAIFFLAGS_H
#define __ASM_DAIFFLAGS_H

#include <linux/irqflags.h>

<<<<<<< HEAD
#define DAIF_PROCCTX		0
#define DAIF_PROCCTX_NOIRQ	PSR_I_BIT
=======
#include <asm/arch_gicv3.h>
#include <asm/barrier.h>
#include <asm/cpufeature.h>
#include <asm/ptrace.h>

#define DAIF_PROCCTX		0
#define DAIF_PROCCTX_NOIRQ	(PSR_I_BIT | PSR_F_BIT)
#define DAIF_ERRCTX		(PSR_A_BIT | PSR_I_BIT | PSR_F_BIT)
#define DAIF_MASK		(PSR_D_BIT | PSR_A_BIT | PSR_I_BIT | PSR_F_BIT)

>>>>>>> upstream/android-13

/* mask/save/unmask/restore all exceptions, including interrupts. */
static inline void local_daif_mask(void)
{
<<<<<<< HEAD
=======
	WARN_ON(system_has_prio_mask_debugging() &&
		(read_sysreg_s(SYS_ICC_PMR_EL1) == (GIC_PRIO_IRQOFF |
						    GIC_PRIO_PSR_I_SET)));

>>>>>>> upstream/android-13
	asm volatile(
		"msr	daifset, #0xf		// local_daif_mask\n"
		:
		:
		: "memory");
<<<<<<< HEAD
	trace_hardirqs_off();
}

=======

	/* Don't really care for a dsb here, we don't intend to enable IRQs */
	if (system_uses_irq_prio_masking())
		gic_write_pmr(GIC_PRIO_IRQON | GIC_PRIO_PSR_I_SET);

	trace_hardirqs_off();
}

static inline unsigned long local_daif_save_flags(void)
{
	unsigned long flags;

	flags = read_sysreg(daif);

	if (system_uses_irq_prio_masking()) {
		/* If IRQs are masked with PMR, reflect it in the flags */
		if (read_sysreg_s(SYS_ICC_PMR_EL1) != GIC_PRIO_IRQON)
			flags |= PSR_I_BIT | PSR_F_BIT;
	}

	return flags;
}

>>>>>>> upstream/android-13
static inline unsigned long local_daif_save(void)
{
	unsigned long flags;

<<<<<<< HEAD
	asm volatile(
		"mrs	%0, daif		// local_daif_save\n"
		: "=r" (flags)
		:
		: "memory");
=======
	flags = local_daif_save_flags();

>>>>>>> upstream/android-13
	local_daif_mask();

	return flags;
}

<<<<<<< HEAD
static inline void local_daif_unmask(void)
{
	trace_hardirqs_on();
	asm volatile(
		"msr	daifclr, #0xf		// local_daif_unmask"
		:
		:
		: "memory");
}

static inline void local_daif_restore(unsigned long flags)
{
	if (!arch_irqs_disabled_flags(flags))
		trace_hardirqs_on();
	asm volatile(
		"msr	daif, %0		// local_daif_restore"
		:
		: "r" (flags)
		: "memory");
	if (arch_irqs_disabled_flags(flags))
		trace_hardirqs_off();
}

=======
static inline void local_daif_restore(unsigned long flags)
{
	bool irq_disabled = flags & PSR_I_BIT;

	WARN_ON(system_has_prio_mask_debugging() &&
		(read_sysreg(daif) & (PSR_I_BIT | PSR_F_BIT)) != (PSR_I_BIT | PSR_F_BIT));

	if (!irq_disabled) {
		trace_hardirqs_on();

		if (system_uses_irq_prio_masking()) {
			gic_write_pmr(GIC_PRIO_IRQON);
			pmr_sync();
		}
	} else if (system_uses_irq_prio_masking()) {
		u64 pmr;

		if (!(flags & PSR_A_BIT)) {
			/*
			 * If interrupts are disabled but we can take
			 * asynchronous errors, we can take NMIs
			 */
			flags &= ~(PSR_I_BIT | PSR_F_BIT);
			pmr = GIC_PRIO_IRQOFF;
		} else {
			pmr = GIC_PRIO_IRQON | GIC_PRIO_PSR_I_SET;
		}

		/*
		 * There has been concern that the write to daif
		 * might be reordered before this write to PMR.
		 * From the ARM ARM DDI 0487D.a, section D1.7.1
		 * "Accessing PSTATE fields":
		 *   Writes to the PSTATE fields have side-effects on
		 *   various aspects of the PE operation. All of these
		 *   side-effects are guaranteed:
		 *     - Not to be visible to earlier instructions in
		 *       the execution stream.
		 *     - To be visible to later instructions in the
		 *       execution stream
		 *
		 * Also, writes to PMR are self-synchronizing, so no
		 * interrupts with a lower priority than PMR is signaled
		 * to the PE after the write.
		 *
		 * So we don't need additional synchronization here.
		 */
		gic_write_pmr(pmr);
	}

	write_sysreg(flags, daif);

	if (irq_disabled)
		trace_hardirqs_off();
}

/*
 * Called by synchronous exception handlers to restore the DAIF bits that were
 * modified by taking an exception.
 */
static inline void local_daif_inherit(struct pt_regs *regs)
{
	unsigned long flags = regs->pstate & DAIF_MASK;

	if (interrupts_enabled(regs))
		trace_hardirqs_on();

	if (system_uses_irq_prio_masking())
		gic_write_pmr(regs->pmr_save);

	/*
	 * We can't use local_daif_restore(regs->pstate) here as
	 * system_has_prio_mask_debugging() won't restore the I bit if it can
	 * use the pmr instead.
	 */
	write_sysreg(flags, daif);
}
>>>>>>> upstream/android-13
#endif
