/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_BH_H
#define _LINUX_BH_H

#include <linux/preempt.h>

<<<<<<< HEAD
#ifdef CONFIG_TRACE_IRQFLAGS
=======
#if defined(CONFIG_PREEMPT_RT) || defined(CONFIG_TRACE_IRQFLAGS)
>>>>>>> upstream/android-13
extern void __local_bh_disable_ip(unsigned long ip, unsigned int cnt);
#else
static __always_inline void __local_bh_disable_ip(unsigned long ip, unsigned int cnt)
{
	preempt_count_add(cnt);
	barrier();
}
#endif

static inline void local_bh_disable(void)
{
	__local_bh_disable_ip(_THIS_IP_, SOFTIRQ_DISABLE_OFFSET);
}

extern void _local_bh_enable(void);
extern void __local_bh_enable_ip(unsigned long ip, unsigned int cnt);

static inline void local_bh_enable_ip(unsigned long ip)
{
	__local_bh_enable_ip(ip, SOFTIRQ_DISABLE_OFFSET);
}

static inline void local_bh_enable(void)
{
	__local_bh_enable_ip(_THIS_IP_, SOFTIRQ_DISABLE_OFFSET);
}

<<<<<<< HEAD
=======
#ifdef CONFIG_PREEMPT_RT
extern bool local_bh_blocked(void);
#else
static inline bool local_bh_blocked(void) { return false; }
#endif

>>>>>>> upstream/android-13
#endif /* _LINUX_BH_H */
