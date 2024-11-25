/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_CONTEXT_TRACKING_H
#define _LINUX_CONTEXT_TRACKING_H

#include <linux/sched.h>
#include <linux/vtime.h>
#include <linux/context_tracking_state.h>
<<<<<<< HEAD
=======
#include <linux/instrumentation.h>

>>>>>>> upstream/android-13
#include <asm/ptrace.h>


#ifdef CONFIG_CONTEXT_TRACKING
extern void context_tracking_cpu_set(int cpu);

/* Called with interrupts disabled.  */
extern void __context_tracking_enter(enum ctx_state state);
extern void __context_tracking_exit(enum ctx_state state);

extern void context_tracking_enter(enum ctx_state state);
extern void context_tracking_exit(enum ctx_state state);
extern void context_tracking_user_enter(void);
extern void context_tracking_user_exit(void);

static inline void user_enter(void)
{
<<<<<<< HEAD
	if (context_tracking_is_enabled())
=======
	if (context_tracking_enabled())
>>>>>>> upstream/android-13
		context_tracking_enter(CONTEXT_USER);

}
static inline void user_exit(void)
{
<<<<<<< HEAD
	if (context_tracking_is_enabled())
=======
	if (context_tracking_enabled())
>>>>>>> upstream/android-13
		context_tracking_exit(CONTEXT_USER);
}

/* Called with interrupts disabled.  */
<<<<<<< HEAD
static inline void user_enter_irqoff(void)
{
	if (context_tracking_is_enabled())
		__context_tracking_enter(CONTEXT_USER);

}
static inline void user_exit_irqoff(void)
{
	if (context_tracking_is_enabled())
=======
static __always_inline void user_enter_irqoff(void)
{
	if (context_tracking_enabled())
		__context_tracking_enter(CONTEXT_USER);

}
static __always_inline void user_exit_irqoff(void)
{
	if (context_tracking_enabled())
>>>>>>> upstream/android-13
		__context_tracking_exit(CONTEXT_USER);
}

static inline enum ctx_state exception_enter(void)
{
	enum ctx_state prev_ctx;

<<<<<<< HEAD
	if (!context_tracking_is_enabled())
=======
	if (IS_ENABLED(CONFIG_HAVE_CONTEXT_TRACKING_OFFSTACK) ||
	    !context_tracking_enabled())
>>>>>>> upstream/android-13
		return 0;

	prev_ctx = this_cpu_read(context_tracking.state);
	if (prev_ctx != CONTEXT_KERNEL)
		context_tracking_exit(prev_ctx);

	return prev_ctx;
}

static inline void exception_exit(enum ctx_state prev_ctx)
{
<<<<<<< HEAD
	if (context_tracking_is_enabled()) {
=======
	if (!IS_ENABLED(CONFIG_HAVE_CONTEXT_TRACKING_OFFSTACK) &&
	    context_tracking_enabled()) {
>>>>>>> upstream/android-13
		if (prev_ctx != CONTEXT_KERNEL)
			context_tracking_enter(prev_ctx);
	}
}

<<<<<<< HEAD
=======
static __always_inline bool context_tracking_guest_enter(void)
{
	if (context_tracking_enabled())
		__context_tracking_enter(CONTEXT_GUEST);

	return context_tracking_enabled_this_cpu();
}

static __always_inline void context_tracking_guest_exit(void)
{
	if (context_tracking_enabled())
		__context_tracking_exit(CONTEXT_GUEST);
}
>>>>>>> upstream/android-13

/**
 * ct_state() - return the current context tracking state if known
 *
 * Returns the current cpu's context tracking state if context tracking
 * is enabled.  If context tracking is disabled, returns
 * CONTEXT_DISABLED.  This should be used primarily for debugging.
 */
<<<<<<< HEAD
static inline enum ctx_state ct_state(void)
{
	return context_tracking_is_enabled() ?
=======
static __always_inline enum ctx_state ct_state(void)
{
	return context_tracking_enabled() ?
>>>>>>> upstream/android-13
		this_cpu_read(context_tracking.state) : CONTEXT_DISABLED;
}
#else
static inline void user_enter(void) { }
static inline void user_exit(void) { }
static inline void user_enter_irqoff(void) { }
static inline void user_exit_irqoff(void) { }
static inline enum ctx_state exception_enter(void) { return 0; }
static inline void exception_exit(enum ctx_state prev_ctx) { }
static inline enum ctx_state ct_state(void) { return CONTEXT_DISABLED; }
<<<<<<< HEAD
#endif /* !CONFIG_CONTEXT_TRACKING */

#define CT_WARN_ON(cond) WARN_ON(context_tracking_is_enabled() && (cond))
=======
static inline bool context_tracking_guest_enter(void) { return false; }
static inline void context_tracking_guest_exit(void) { }

#endif /* !CONFIG_CONTEXT_TRACKING */

#define CT_WARN_ON(cond) WARN_ON(context_tracking_enabled() && (cond))
>>>>>>> upstream/android-13

#ifdef CONFIG_CONTEXT_TRACKING_FORCE
extern void context_tracking_init(void);
#else
static inline void context_tracking_init(void) { }
#endif /* CONFIG_CONTEXT_TRACKING_FORCE */

<<<<<<< HEAD

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
/* must be called with irqs disabled */
static inline void guest_enter_irqoff(void)
{
	if (vtime_accounting_cpu_enabled())
		vtime_guest_enter(current);
	else
		current->flags |= PF_VCPU;

	if (context_tracking_is_enabled())
		__context_tracking_enter(CONTEXT_GUEST);

	/* KVM does not hold any references to rcu protected data when it
	 * switches CPU into a guest mode. In fact switching to a guest mode
	 * is very similar to exiting to userspace from rcu point of view. In
	 * addition CPU may stay in a guest mode for quite a long time (up to
	 * one time slice). Lets treat guest mode as quiescent state, just like
	 * we do with user-mode execution.
	 */
	if (!context_tracking_cpu_is_enabled())
		rcu_virt_note_context_switch(smp_processor_id());
}

static inline void guest_exit_irqoff(void)
{
	if (context_tracking_is_enabled())
		__context_tracking_exit(CONTEXT_GUEST);

	if (vtime_accounting_cpu_enabled())
		vtime_guest_exit(current);
	else
		current->flags &= ~PF_VCPU;
}

#else
static inline void guest_enter_irqoff(void)
{
	/*
	 * This is running in ioctl context so its safe
	 * to assume that it's the stime pending cputime
	 * to flush.
	 */
	vtime_account_system(current);
	current->flags |= PF_VCPU;
	rcu_virt_note_context_switch(smp_processor_id());
}

static inline void guest_exit_irqoff(void)
{
	/* Flush the guest cputime we spent on the guest */
	vtime_account_system(current);
	current->flags &= ~PF_VCPU;
}
#endif /* CONFIG_VIRT_CPU_ACCOUNTING_GEN */

static inline void guest_enter(void)
{
	unsigned long flags;

	local_irq_save(flags);
	guest_enter_irqoff();
	local_irq_restore(flags);
}

static inline void guest_exit(void)
{
	unsigned long flags;

	local_irq_save(flags);
	guest_exit_irqoff();
	local_irq_restore(flags);
}

=======
>>>>>>> upstream/android-13
#endif
