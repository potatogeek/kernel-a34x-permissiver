/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_ARM_PARAVIRT_H
#define _ASM_ARM_PARAVIRT_H

#ifdef CONFIG_PARAVIRT
<<<<<<< HEAD
=======
#include <linux/static_call_types.h>

>>>>>>> upstream/android-13
struct static_key;
extern struct static_key paravirt_steal_enabled;
extern struct static_key paravirt_steal_rq_enabled;

<<<<<<< HEAD
struct pv_time_ops {
	unsigned long long (*steal_clock)(int cpu);
};
extern struct pv_time_ops pv_time_ops;

static inline u64 paravirt_steal_clock(int cpu)
{
	return pv_time_ops.steal_clock(cpu);
=======
u64 dummy_steal_clock(int cpu);

DECLARE_STATIC_CALL(pv_steal_clock, dummy_steal_clock);

static inline u64 paravirt_steal_clock(int cpu)
{
	return static_call(pv_steal_clock)(cpu);
>>>>>>> upstream/android-13
}
#endif

#endif
