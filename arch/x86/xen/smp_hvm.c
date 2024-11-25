// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
=======
#include <linux/thread_info.h>
>>>>>>> upstream/android-13
#include <asm/smp.h>

#include <xen/events.h>

#include "xen-ops.h"
#include "smp.h"


static void __init xen_hvm_smp_prepare_boot_cpu(void)
{
	BUG_ON(smp_processor_id() != 0);
	native_smp_prepare_boot_cpu();

	/*
	 * Setup vcpu_info for boot CPU. Secondary CPUs get their vcpu_info
	 * in xen_cpu_up_prepare_hvm().
	 */
	xen_vcpu_setup(0);

	/*
<<<<<<< HEAD
=======
	 * Called again in case the kernel boots on vcpu >= MAX_VIRT_CPUS.
	 * Refer to comments in xen_hvm_init_time_ops().
	 */
	xen_hvm_init_time_ops();

	/*
>>>>>>> upstream/android-13
	 * The alternative logic (which patches the unlock/lock) runs before
	 * the smp bootup up code is activated. Hence we need to set this up
	 * the core kernel is being patched. Otherwise we will have only
	 * modules patched but not core code.
	 */
	xen_init_spinlocks();
}

static void __init xen_hvm_smp_prepare_cpus(unsigned int max_cpus)
{
	int cpu;

	native_smp_prepare_cpus(max_cpus);
<<<<<<< HEAD
	WARN_ON(xen_smp_intr_init(0));

	xen_init_lock_cpu(0);
=======

	if (xen_have_vector_callback) {
		WARN_ON(xen_smp_intr_init(0));
		xen_init_lock_cpu(0);
	}
>>>>>>> upstream/android-13

	for_each_possible_cpu(cpu) {
		if (cpu == 0)
			continue;

		/* Set default vcpu_id to make sure that we don't use cpu-0's */
		per_cpu(xen_vcpu_id, cpu) = XEN_VCPU_ID_INVALID;
	}
}

#ifdef CONFIG_HOTPLUG_CPU
static void xen_hvm_cpu_die(unsigned int cpu)
{
	if (common_cpu_die(cpu) == 0) {
<<<<<<< HEAD
		xen_smp_intr_free(cpu);
		xen_uninit_lock_cpu(cpu);
		xen_teardown_timer(cpu);
=======
		if (xen_have_vector_callback) {
			xen_smp_intr_free(cpu);
			xen_uninit_lock_cpu(cpu);
			xen_teardown_timer(cpu);
		}
>>>>>>> upstream/android-13
	}
}
#else
static void xen_hvm_cpu_die(unsigned int cpu)
{
	BUG();
}
#endif

void __init xen_hvm_smp_init(void)
{
<<<<<<< HEAD
	if (!xen_have_vector_callback)
		return;

	smp_ops.smp_prepare_cpus = xen_hvm_smp_prepare_cpus;
	smp_ops.smp_send_reschedule = xen_smp_send_reschedule;
	smp_ops.cpu_die = xen_hvm_cpu_die;
	smp_ops.send_call_func_ipi = xen_smp_send_call_function_ipi;
	smp_ops.send_call_func_single_ipi = xen_smp_send_call_function_single_ipi;
	smp_ops.smp_prepare_boot_cpu = xen_hvm_smp_prepare_boot_cpu;
	smp_ops.smp_cpus_done = xen_smp_cpus_done;
=======
	smp_ops.smp_prepare_boot_cpu = xen_hvm_smp_prepare_boot_cpu;
	smp_ops.smp_prepare_cpus = xen_hvm_smp_prepare_cpus;
	smp_ops.smp_cpus_done = xen_smp_cpus_done;
	smp_ops.cpu_die = xen_hvm_cpu_die;

	if (!xen_have_vector_callback) {
#ifdef CONFIG_PARAVIRT_SPINLOCKS
		nopvspin = true;
#endif
		return;
	}

	smp_ops.smp_send_reschedule = xen_smp_send_reschedule;
	smp_ops.send_call_func_ipi = xen_smp_send_call_function_ipi;
	smp_ops.send_call_func_single_ipi = xen_smp_send_call_function_single_ipi;
>>>>>>> upstream/android-13
}
