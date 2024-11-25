<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Uniprocessor-only support functions.  The counterpart to kernel/smp.c
 */

#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/smp.h>
#include <linux/hypervisor.h>

int smp_call_function_single(int cpu, void (*func) (void *info), void *info,
				int wait)
{
	unsigned long flags;

<<<<<<< HEAD
	WARN_ON(cpu != 0);
=======
	if (cpu != 0)
		return -ENXIO;
>>>>>>> upstream/android-13

	local_irq_save(flags);
	func(info);
	local_irq_restore(flags);

	return 0;
}
EXPORT_SYMBOL(smp_call_function_single);

int smp_call_function_single_async(int cpu, struct __call_single_data *csd)
{
	unsigned long flags;

	local_irq_save(flags);
	csd->func(csd->info);
	local_irq_restore(flags);
	return 0;
}
EXPORT_SYMBOL(smp_call_function_single_async);

<<<<<<< HEAD
int on_each_cpu(smp_call_func_t func, void *info, int wait)
{
	unsigned long flags;

	local_irq_save(flags);
	func(info);
	local_irq_restore(flags);
	return 0;
}
EXPORT_SYMBOL(on_each_cpu);

/*
 * Note we still need to test the mask even for UP
 * because we actually can get an empty mask from
 * code that on SMP might call us without the local
 * CPU in the mask.
 */
void on_each_cpu_mask(const struct cpumask *mask,
		      smp_call_func_t func, void *info, bool wait)
{
	unsigned long flags;

	if (cpumask_test_cpu(0, mask)) {
		local_irq_save(flags);
		func(info);
		local_irq_restore(flags);
	}
}
EXPORT_SYMBOL(on_each_cpu_mask);

/*
 * Preemption is disabled here to make sure the cond_func is called under the
 * same condtions in UP and SMP.
 */
void on_each_cpu_cond(bool (*cond_func)(int cpu, void *info),
		      smp_call_func_t func, void *info, bool wait,
		      gfp_t gfp_flags)
=======
/*
 * Preemption is disabled here to make sure the cond_func is called under the
 * same conditions in UP and SMP.
 */
void on_each_cpu_cond_mask(smp_cond_func_t cond_func, smp_call_func_t func,
			   void *info, bool wait, const struct cpumask *mask)
>>>>>>> upstream/android-13
{
	unsigned long flags;

	preempt_disable();
<<<<<<< HEAD
	if (cond_func(0, info)) {
=======
	if ((!cond_func || cond_func(0, info)) && cpumask_test_cpu(0, mask)) {
>>>>>>> upstream/android-13
		local_irq_save(flags);
		func(info);
		local_irq_restore(flags);
	}
	preempt_enable();
}
<<<<<<< HEAD
EXPORT_SYMBOL(on_each_cpu_cond);
=======
EXPORT_SYMBOL(on_each_cpu_cond_mask);
>>>>>>> upstream/android-13

int smp_call_on_cpu(unsigned int cpu, int (*func)(void *), void *par, bool phys)
{
	int ret;

	if (cpu != 0)
		return -ENXIO;

	if (phys)
		hypervisor_pin_vcpu(0);
	ret = func(par);
	if (phys)
		hypervisor_pin_vcpu(-1);

	return ret;
}
EXPORT_SYMBOL_GPL(smp_call_on_cpu);
