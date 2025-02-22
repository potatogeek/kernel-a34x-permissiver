/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KERNEL_VTIME_H
#define _LINUX_KERNEL_VTIME_H

#include <linux/context_tracking_state.h>
<<<<<<< HEAD
=======
#include <linux/sched.h>

>>>>>>> upstream/android-13
#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
#include <asm/vtime.h>
#endif

<<<<<<< HEAD

struct task_struct;

/*
 * vtime_accounting_cpu_enabled() definitions/declarations
 */
#if defined(CONFIG_VIRT_CPU_ACCOUNTING_NATIVE)
static inline bool vtime_accounting_cpu_enabled(void) { return true; }
#elif defined(CONFIG_VIRT_CPU_ACCOUNTING_GEN)
/*
 * Checks if vtime is enabled on some CPU. Cputime readers want to be careful
 * in that case and compute the tickless cputime.
 * For now vtime state is tied to context tracking. We might want to decouple
 * those later if necessary.
 */
static inline bool vtime_accounting_enabled(void)
{
	return context_tracking_is_enabled();
}

static inline bool vtime_accounting_cpu_enabled(void)
{
	if (vtime_accounting_enabled()) {
		if (context_tracking_cpu_is_enabled())
			return true;
	}

	return false;
}
#else /* !CONFIG_VIRT_CPU_ACCOUNTING */
static inline bool vtime_accounting_cpu_enabled(void) { return false; }
#endif


=======
>>>>>>> upstream/android-13
/*
 * Common vtime APIs
 */
#ifdef CONFIG_VIRT_CPU_ACCOUNTING
<<<<<<< HEAD

#ifdef __ARCH_HAS_VTIME_TASK_SWITCH
extern void vtime_task_switch(struct task_struct *prev);
#else
extern void vtime_common_task_switch(struct task_struct *prev);
static inline void vtime_task_switch(struct task_struct *prev)
{
	if (vtime_accounting_cpu_enabled())
		vtime_common_task_switch(prev);
}
#endif /* __ARCH_HAS_VTIME_TASK_SWITCH */

extern void vtime_account_system(struct task_struct *tsk);
extern void vtime_account_idle(struct task_struct *tsk);

#else /* !CONFIG_VIRT_CPU_ACCOUNTING */

static inline void vtime_task_switch(struct task_struct *prev) { }
static inline void vtime_account_system(struct task_struct *tsk) { }
=======
extern void vtime_account_kernel(struct task_struct *tsk);
extern void vtime_account_idle(struct task_struct *tsk);
>>>>>>> upstream/android-13
#endif /* !CONFIG_VIRT_CPU_ACCOUNTING */

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
extern void arch_vtime_task_switch(struct task_struct *tsk);
extern void vtime_user_enter(struct task_struct *tsk);
extern void vtime_user_exit(struct task_struct *tsk);
extern void vtime_guest_enter(struct task_struct *tsk);
extern void vtime_guest_exit(struct task_struct *tsk);
extern void vtime_init_idle(struct task_struct *tsk, int cpu);
#else /* !CONFIG_VIRT_CPU_ACCOUNTING_GEN  */
static inline void vtime_user_enter(struct task_struct *tsk) { }
static inline void vtime_user_exit(struct task_struct *tsk) { }
static inline void vtime_guest_enter(struct task_struct *tsk) { }
static inline void vtime_guest_exit(struct task_struct *tsk) { }
static inline void vtime_init_idle(struct task_struct *tsk, int cpu) { }
#endif

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
<<<<<<< HEAD
extern void vtime_account_irq_enter(struct task_struct *tsk);
static inline void vtime_account_irq_exit(struct task_struct *tsk)
{
	/* On hard|softirq exit we always account to hard|softirq cputime */
	vtime_account_system(tsk);
}
extern void vtime_flush(struct task_struct *tsk);
#else /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */
static inline void vtime_account_irq_enter(struct task_struct *tsk) { }
static inline void vtime_account_irq_exit(struct task_struct *tsk) { }
static inline void vtime_flush(struct task_struct *tsk) { }
#endif


#ifdef CONFIG_IRQ_TIME_ACCOUNTING
extern void irqtime_account_irq(struct task_struct *tsk);
#else
static inline void irqtime_account_irq(struct task_struct *tsk) { }
#endif

static inline void account_irq_enter_time(struct task_struct *tsk)
{
	vtime_account_irq_enter(tsk);
	irqtime_account_irq(tsk);
}

static inline void account_irq_exit_time(struct task_struct *tsk)
{
	vtime_account_irq_exit(tsk);
	irqtime_account_irq(tsk);
=======
extern void vtime_account_irq(struct task_struct *tsk, unsigned int offset);
extern void vtime_account_softirq(struct task_struct *tsk);
extern void vtime_account_hardirq(struct task_struct *tsk);
extern void vtime_flush(struct task_struct *tsk);
#else /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */
static inline void vtime_account_irq(struct task_struct *tsk, unsigned int offset) { }
static inline void vtime_account_softirq(struct task_struct *tsk) { }
static inline void vtime_account_hardirq(struct task_struct *tsk) { }
static inline void vtime_flush(struct task_struct *tsk) { }
#endif

/*
 * vtime_accounting_enabled_this_cpu() definitions/declarations
 */
#if defined(CONFIG_VIRT_CPU_ACCOUNTING_NATIVE)

static inline bool vtime_accounting_enabled_this_cpu(void) { return true; }
extern void vtime_task_switch(struct task_struct *prev);

static __always_inline void vtime_account_guest_enter(void)
{
	vtime_account_kernel(current);
	current->flags |= PF_VCPU;
}

static __always_inline void vtime_account_guest_exit(void)
{
	vtime_account_kernel(current);
	current->flags &= ~PF_VCPU;
}

#elif defined(CONFIG_VIRT_CPU_ACCOUNTING_GEN)

/*
 * Checks if vtime is enabled on some CPU. Cputime readers want to be careful
 * in that case and compute the tickless cputime.
 * For now vtime state is tied to context tracking. We might want to decouple
 * those later if necessary.
 */
static inline bool vtime_accounting_enabled(void)
{
	return context_tracking_enabled();
}

static inline bool vtime_accounting_enabled_cpu(int cpu)
{
	return context_tracking_enabled_cpu(cpu);
}

static inline bool vtime_accounting_enabled_this_cpu(void)
{
	return context_tracking_enabled_this_cpu();
}

extern void vtime_task_switch_generic(struct task_struct *prev);

static inline void vtime_task_switch(struct task_struct *prev)
{
	if (vtime_accounting_enabled_this_cpu())
		vtime_task_switch_generic(prev);
}

static __always_inline void vtime_account_guest_enter(void)
{
	if (vtime_accounting_enabled_this_cpu())
		vtime_guest_enter(current);
	else
		current->flags |= PF_VCPU;
}

static __always_inline void vtime_account_guest_exit(void)
{
	if (vtime_accounting_enabled_this_cpu())
		vtime_guest_exit(current);
	else
		current->flags &= ~PF_VCPU;
}

#else /* !CONFIG_VIRT_CPU_ACCOUNTING */

static inline bool vtime_accounting_enabled_this_cpu(void) { return false; }
static inline void vtime_task_switch(struct task_struct *prev) { }

static __always_inline void vtime_account_guest_enter(void)
{
	current->flags |= PF_VCPU;
}

static __always_inline void vtime_account_guest_exit(void)
{
	current->flags &= ~PF_VCPU;
}

#endif


#ifdef CONFIG_IRQ_TIME_ACCOUNTING
extern void irqtime_account_irq(struct task_struct *tsk, unsigned int offset);
#else
static inline void irqtime_account_irq(struct task_struct *tsk, unsigned int offset) { }
#endif

static inline void account_softirq_enter(struct task_struct *tsk)
{
	vtime_account_irq(tsk, SOFTIRQ_OFFSET);
	irqtime_account_irq(tsk, SOFTIRQ_OFFSET);
}

static inline void account_softirq_exit(struct task_struct *tsk)
{
	vtime_account_softirq(tsk);
	irqtime_account_irq(tsk, 0);
}

static inline void account_hardirq_enter(struct task_struct *tsk)
{
	vtime_account_irq(tsk, HARDIRQ_OFFSET);
	irqtime_account_irq(tsk, HARDIRQ_OFFSET);
}

static inline void account_hardirq_exit(struct task_struct *tsk)
{
	vtime_account_hardirq(tsk);
	irqtime_account_irq(tsk, 0);
>>>>>>> upstream/android-13
}

#endif /* _LINUX_KERNEL_VTIME_H */
