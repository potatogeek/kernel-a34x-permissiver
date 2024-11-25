/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FTRACE_IRQ_H
#define _LINUX_FTRACE_IRQ_H

<<<<<<< HEAD

#ifdef CONFIG_FTRACE_NMI_ENTER
extern void arch_ftrace_nmi_enter(void);
extern void arch_ftrace_nmi_exit(void);
#else
static inline void arch_ftrace_nmi_enter(void) { }
static inline void arch_ftrace_nmi_exit(void) { }
#endif

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_HWLAT_TRACER
extern bool trace_hwlat_callback_enabled;
extern void trace_hwlat_callback(bool enter);
#endif

<<<<<<< HEAD
=======
#ifdef CONFIG_OSNOISE_TRACER
extern bool trace_osnoise_callback_enabled;
extern void trace_osnoise_callback(bool enter);
#endif

>>>>>>> upstream/android-13
static inline void ftrace_nmi_enter(void)
{
#ifdef CONFIG_HWLAT_TRACER
	if (trace_hwlat_callback_enabled)
		trace_hwlat_callback(true);
#endif
<<<<<<< HEAD
	arch_ftrace_nmi_enter();
=======
#ifdef CONFIG_OSNOISE_TRACER
	if (trace_osnoise_callback_enabled)
		trace_osnoise_callback(true);
#endif
>>>>>>> upstream/android-13
}

static inline void ftrace_nmi_exit(void)
{
<<<<<<< HEAD
	arch_ftrace_nmi_exit();
=======
>>>>>>> upstream/android-13
#ifdef CONFIG_HWLAT_TRACER
	if (trace_hwlat_callback_enabled)
		trace_hwlat_callback(false);
#endif
<<<<<<< HEAD
=======
#ifdef CONFIG_OSNOISE_TRACER
	if (trace_osnoise_callback_enabled)
		trace_osnoise_callback(false);
#endif
>>>>>>> upstream/android-13
}

#endif /* _LINUX_FTRACE_IRQ_H */
