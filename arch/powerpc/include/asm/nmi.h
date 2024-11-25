/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_NMI_H
#define _ASM_NMI_H

#ifdef CONFIG_PPC_WATCHDOG
extern void arch_touch_nmi_watchdog(void);
<<<<<<< HEAD
=======
long soft_nmi_interrupt(struct pt_regs *regs);
>>>>>>> upstream/android-13
#else
static inline void arch_touch_nmi_watchdog(void) {}
#endif

#if defined(CONFIG_NMI_IPI) && defined(CONFIG_STACKTRACE)
extern void arch_trigger_cpumask_backtrace(const cpumask_t *mask,
					   bool exclude_self);
#define arch_trigger_cpumask_backtrace arch_trigger_cpumask_backtrace
#endif

<<<<<<< HEAD
=======
extern void hv_nmi_check_nonrecoverable(struct pt_regs *regs);

>>>>>>> upstream/android-13
#endif /* _ASM_NMI_H */
