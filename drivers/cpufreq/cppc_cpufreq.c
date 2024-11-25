<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * CPPC (Collaborative Processor Performance Control) driver for
 * interfacing with the CPUfreq layer and governors. See
 * cppc_acpi.c for CPPC specific methods.
 *
 * (C) Copyright 2014, 2015 Linaro Ltd.
 * Author: Ashwin Chaugule <ashwin.chaugule@linaro.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
=======
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt)	"CPPC Cpufreq:"	fmt

<<<<<<< HEAD
=======
#include <linux/arch_topology.h>
>>>>>>> upstream/android-13
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/cpu.h>
#include <linux/cpufreq.h>
#include <linux/dmi.h>
<<<<<<< HEAD
#include <linux/time.h>
#include <linux/vmalloc.h>
=======
#include <linux/irq_work.h>
#include <linux/kthread.h>
#include <linux/time.h>
#include <linux/vmalloc.h>
#include <uapi/linux/sched/types.h>
>>>>>>> upstream/android-13

#include <asm/unaligned.h>

#include <acpi/cppc_acpi.h>

/* Minimum struct length needed for the DMI processor entry we want */
#define DMI_ENTRY_PROCESSOR_MIN_LENGTH	48

<<<<<<< HEAD
/* Offest in the DMI processor structure for the max frequency */
#define DMI_PROCESSOR_MAX_SPEED  0x14

/*
 * These structs contain information parsed from per CPU
 * ACPI _CPC structures.
 * e.g. For each CPU the highest, lowest supported
 * performance capabilities, desired performance level
 * requested etc.
 */
static struct cppc_cpudata **all_cpu_data;
=======
/* Offset in the DMI processor structure for the max frequency */
#define DMI_PROCESSOR_MAX_SPEED		0x14

/*
 * This list contains information parsed from per CPU ACPI _CPC and _PSD
 * structures: e.g. the highest and lowest supported performance, capabilities,
 * desired performance, level requested etc. Depending on the share_type, not
 * all CPUs will have an entry in the list.
 */
static LIST_HEAD(cpu_data_list);

static bool boost_supported;

struct cppc_workaround_oem_info {
	char oem_id[ACPI_OEM_ID_SIZE + 1];
	char oem_table_id[ACPI_OEM_TABLE_ID_SIZE + 1];
	u32 oem_revision;
};

static struct cppc_workaround_oem_info wa_info[] = {
	{
		.oem_id		= "HISI  ",
		.oem_table_id	= "HIP07   ",
		.oem_revision	= 0,
	}, {
		.oem_id		= "HISI  ",
		.oem_table_id	= "HIP08   ",
		.oem_revision	= 0,
	}
};

#ifdef CONFIG_ACPI_CPPC_CPUFREQ_FIE

/* Frequency invariance support */
struct cppc_freq_invariance {
	int cpu;
	struct irq_work irq_work;
	struct kthread_work work;
	struct cppc_perf_fb_ctrs prev_perf_fb_ctrs;
	struct cppc_cpudata *cpu_data;
};

static DEFINE_PER_CPU(struct cppc_freq_invariance, cppc_freq_inv);
static struct kthread_worker *kworker_fie;

static struct cpufreq_driver cppc_cpufreq_driver;
static unsigned int hisi_cppc_cpufreq_get_rate(unsigned int cpu);
static int cppc_perf_from_fbctrs(struct cppc_cpudata *cpu_data,
				 struct cppc_perf_fb_ctrs *fb_ctrs_t0,
				 struct cppc_perf_fb_ctrs *fb_ctrs_t1);

/**
 * cppc_scale_freq_workfn - CPPC arch_freq_scale updater for frequency invariance
 * @work: The work item.
 *
 * The CPPC driver register itself with the topology core to provide its own
 * implementation (cppc_scale_freq_tick()) of topology_scale_freq_tick() which
 * gets called by the scheduler on every tick.
 *
 * Note that the arch specific counters have higher priority than CPPC counters,
 * if available, though the CPPC driver doesn't need to have any special
 * handling for that.
 *
 * On an invocation of cppc_scale_freq_tick(), we schedule an irq work (since we
 * reach here from hard-irq context), which then schedules a normal work item
 * and cppc_scale_freq_workfn() updates the per_cpu arch_freq_scale variable
 * based on the counter updates since the last tick.
 */
static void cppc_scale_freq_workfn(struct kthread_work *work)
{
	struct cppc_freq_invariance *cppc_fi;
	struct cppc_perf_fb_ctrs fb_ctrs = {0};
	struct cppc_cpudata *cpu_data;
	unsigned long local_freq_scale;
	u64 perf;

	cppc_fi = container_of(work, struct cppc_freq_invariance, work);
	cpu_data = cppc_fi->cpu_data;

	if (cppc_get_perf_ctrs(cppc_fi->cpu, &fb_ctrs)) {
		pr_warn("%s: failed to read perf counters\n", __func__);
		return;
	}

	perf = cppc_perf_from_fbctrs(cpu_data, &cppc_fi->prev_perf_fb_ctrs,
				     &fb_ctrs);
	cppc_fi->prev_perf_fb_ctrs = fb_ctrs;

	perf <<= SCHED_CAPACITY_SHIFT;
	local_freq_scale = div64_u64(perf, cpu_data->perf_caps.highest_perf);

	/* This can happen due to counter's overflow */
	if (unlikely(local_freq_scale > 1024))
		local_freq_scale = 1024;

	per_cpu(arch_freq_scale, cppc_fi->cpu) = local_freq_scale;
}

static void cppc_irq_work(struct irq_work *irq_work)
{
	struct cppc_freq_invariance *cppc_fi;

	cppc_fi = container_of(irq_work, struct cppc_freq_invariance, irq_work);
	kthread_queue_work(kworker_fie, &cppc_fi->work);
}

static void cppc_scale_freq_tick(void)
{
	struct cppc_freq_invariance *cppc_fi = &per_cpu(cppc_freq_inv, smp_processor_id());

	/*
	 * cppc_get_perf_ctrs() can potentially sleep, call that from the right
	 * context.
	 */
	irq_work_queue(&cppc_fi->irq_work);
}

static struct scale_freq_data cppc_sftd = {
	.source = SCALE_FREQ_SOURCE_CPPC,
	.set_freq_scale = cppc_scale_freq_tick,
};

static void cppc_cpufreq_cpu_fie_init(struct cpufreq_policy *policy)
{
	struct cppc_freq_invariance *cppc_fi;
	int cpu, ret;

	if (cppc_cpufreq_driver.get == hisi_cppc_cpufreq_get_rate)
		return;

	for_each_cpu(cpu, policy->cpus) {
		cppc_fi = &per_cpu(cppc_freq_inv, cpu);
		cppc_fi->cpu = cpu;
		cppc_fi->cpu_data = policy->driver_data;
		kthread_init_work(&cppc_fi->work, cppc_scale_freq_workfn);
		init_irq_work(&cppc_fi->irq_work, cppc_irq_work);

		ret = cppc_get_perf_ctrs(cpu, &cppc_fi->prev_perf_fb_ctrs);
		if (ret) {
			pr_warn("%s: failed to read perf counters for cpu:%d: %d\n",
				__func__, cpu, ret);

			/*
			 * Don't abort if the CPU was offline while the driver
			 * was getting registered.
			 */
			if (cpu_online(cpu))
				return;
		}
	}

	/* Register for freq-invariance */
	topology_set_scale_freq_source(&cppc_sftd, policy->cpus);
}

/*
 * We free all the resources on policy's removal and not on CPU removal as the
 * irq-work are per-cpu and the hotplug core takes care of flushing the pending
 * irq-works (hint: smpcfd_dying_cpu()) on CPU hotplug. Even if the kthread-work
 * fires on another CPU after the concerned CPU is removed, it won't harm.
 *
 * We just need to make sure to remove them all on policy->exit().
 */
static void cppc_cpufreq_cpu_fie_exit(struct cpufreq_policy *policy)
{
	struct cppc_freq_invariance *cppc_fi;
	int cpu;

	if (cppc_cpufreq_driver.get == hisi_cppc_cpufreq_get_rate)
		return;

	/* policy->cpus will be empty here, use related_cpus instead */
	topology_clear_scale_freq_source(SCALE_FREQ_SOURCE_CPPC, policy->related_cpus);

	for_each_cpu(cpu, policy->related_cpus) {
		cppc_fi = &per_cpu(cppc_freq_inv, cpu);
		irq_work_sync(&cppc_fi->irq_work);
		kthread_cancel_work_sync(&cppc_fi->work);
	}
}

static void __init cppc_freq_invariance_init(void)
{
	struct sched_attr attr = {
		.size		= sizeof(struct sched_attr),
		.sched_policy	= SCHED_DEADLINE,
		.sched_nice	= 0,
		.sched_priority	= 0,
		/*
		 * Fake (unused) bandwidth; workaround to "fix"
		 * priority inheritance.
		 */
		.sched_runtime	= 1000000,
		.sched_deadline = 10000000,
		.sched_period	= 10000000,
	};
	int ret;

	if (cppc_cpufreq_driver.get == hisi_cppc_cpufreq_get_rate)
		return;

	kworker_fie = kthread_create_worker(0, "cppc_fie");
	if (IS_ERR(kworker_fie))
		return;

	ret = sched_setattr_nocheck(kworker_fie->task, &attr);
	if (ret) {
		pr_warn("%s: failed to set SCHED_DEADLINE: %d\n", __func__,
			ret);
		kthread_destroy_worker(kworker_fie);
		return;
	}
}

static void cppc_freq_invariance_exit(void)
{
	if (cppc_cpufreq_driver.get == hisi_cppc_cpufreq_get_rate)
		return;

	kthread_destroy_worker(kworker_fie);
	kworker_fie = NULL;
}

#else
static inline void cppc_cpufreq_cpu_fie_init(struct cpufreq_policy *policy)
{
}

static inline void cppc_cpufreq_cpu_fie_exit(struct cpufreq_policy *policy)
{
}

static inline void cppc_freq_invariance_init(void)
{
}

static inline void cppc_freq_invariance_exit(void)
{
}
#endif /* CONFIG_ACPI_CPPC_CPUFREQ_FIE */
>>>>>>> upstream/android-13

/* Callback function used to retrieve the max frequency from DMI */
static void cppc_find_dmi_mhz(const struct dmi_header *dm, void *private)
{
	const u8 *dmi_data = (const u8 *)dm;
	u16 *mhz = (u16 *)private;

	if (dm->type == DMI_ENTRY_PROCESSOR &&
	    dm->length >= DMI_ENTRY_PROCESSOR_MIN_LENGTH) {
		u16 val = (u16)get_unaligned((const u16 *)
				(dmi_data + DMI_PROCESSOR_MAX_SPEED));
		*mhz = val > *mhz ? val : *mhz;
	}
}

/* Look up the max frequency in DMI */
static u64 cppc_get_dmi_max_khz(void)
{
	u16 mhz = 0;

	dmi_walk(cppc_find_dmi_mhz, &mhz);

	/*
	 * Real stupid fallback value, just in case there is no
	 * actual value set.
	 */
	mhz = mhz ? mhz : 1;

	return (1000 * mhz);
}

/*
 * If CPPC lowest_freq and nominal_freq registers are exposed then we can
<<<<<<< HEAD
 * use them to convert perf to freq and vice versa
 *
 * If the perf/freq point lies between Nominal and Lowest, we can treat
 * (Low perf, Low freq) and (Nom Perf, Nom freq) as 2D co-ordinates of a line
 * and extrapolate the rest
 * For perf/freq > Nominal, we use the ratio perf:freq at Nominal for conversion
 */
static unsigned int cppc_cpufreq_perf_to_khz(struct cppc_cpudata *cpu,
					unsigned int perf)
{
	static u64 max_khz;
	struct cppc_perf_caps *caps = &cpu->perf_caps;
	u64 mul, div;

	if (caps->lowest_freq && caps->nominal_freq) {
		if (perf >= caps->nominal_perf) {
			mul = caps->nominal_freq;
			div = caps->nominal_perf;
		} else {
			mul = caps->nominal_freq - caps->lowest_freq;
			div = caps->nominal_perf - caps->lowest_perf;
		}
=======
 * use them to convert perf to freq and vice versa. The conversion is
 * extrapolated as an affine function passing by the 2 points:
 *  - (Low perf, Low freq)
 *  - (Nominal perf, Nominal perf)
 */
static unsigned int cppc_cpufreq_perf_to_khz(struct cppc_cpudata *cpu_data,
					     unsigned int perf)
{
	struct cppc_perf_caps *caps = &cpu_data->perf_caps;
	s64 retval, offset = 0;
	static u64 max_khz;
	u64 mul, div;

	if (caps->lowest_freq && caps->nominal_freq) {
		mul = caps->nominal_freq - caps->lowest_freq;
		div = caps->nominal_perf - caps->lowest_perf;
		offset = caps->nominal_freq - div64_u64(caps->nominal_perf * mul, div);
>>>>>>> upstream/android-13
	} else {
		if (!max_khz)
			max_khz = cppc_get_dmi_max_khz();
		mul = max_khz;
<<<<<<< HEAD
		div = cpu->perf_caps.highest_perf;
	}
	return (u64)perf * mul / div;
}

static unsigned int cppc_cpufreq_khz_to_perf(struct cppc_cpudata *cpu,
					unsigned int freq)
{
	static u64 max_khz;
	struct cppc_perf_caps *caps = &cpu->perf_caps;
	u64  mul, div;

	if (caps->lowest_freq && caps->nominal_freq) {
		if (freq >= caps->nominal_freq) {
			mul = caps->nominal_perf;
			div = caps->nominal_freq;
		} else {
			mul = caps->lowest_perf;
			div = caps->lowest_freq;
		}
	} else {
		if (!max_khz)
			max_khz = cppc_get_dmi_max_khz();
		mul = cpu->perf_caps.highest_perf;
		div = max_khz;
	}

	return (u64)freq * mul / div;
}

static int cppc_cpufreq_set_target(struct cpufreq_policy *policy,
		unsigned int target_freq,
		unsigned int relation)
{
	struct cppc_cpudata *cpu;
=======
		div = caps->highest_perf;
	}

	retval = offset + div64_u64(perf * mul, div);
	if (retval >= 0)
		return retval;
	return 0;
}

static unsigned int cppc_cpufreq_khz_to_perf(struct cppc_cpudata *cpu_data,
					     unsigned int freq)
{
	struct cppc_perf_caps *caps = &cpu_data->perf_caps;
	s64 retval, offset = 0;
	static u64 max_khz;
	u64  mul, div;

	if (caps->lowest_freq && caps->nominal_freq) {
		mul = caps->nominal_perf - caps->lowest_perf;
		div = caps->nominal_freq - caps->lowest_freq;
		offset = caps->nominal_perf - div64_u64(caps->nominal_freq * mul, div);
	} else {
		if (!max_khz)
			max_khz = cppc_get_dmi_max_khz();
		mul = caps->highest_perf;
		div = max_khz;
	}

	retval = offset + div64_u64(freq * mul, div);
	if (retval >= 0)
		return retval;
	return 0;
}

static int cppc_cpufreq_set_target(struct cpufreq_policy *policy,
				   unsigned int target_freq,
				   unsigned int relation)

{
	struct cppc_cpudata *cpu_data = policy->driver_data;
	unsigned int cpu = policy->cpu;
>>>>>>> upstream/android-13
	struct cpufreq_freqs freqs;
	u32 desired_perf;
	int ret = 0;

<<<<<<< HEAD
	cpu = all_cpu_data[policy->cpu];

	desired_perf = cppc_cpufreq_khz_to_perf(cpu, target_freq);
	/* Return if it is exactly the same perf */
	if (desired_perf == cpu->perf_ctrls.desired_perf)
		return ret;

	cpu->perf_ctrls.desired_perf = desired_perf;
=======
	desired_perf = cppc_cpufreq_khz_to_perf(cpu_data, target_freq);
	/* Return if it is exactly the same perf */
	if (desired_perf == cpu_data->perf_ctrls.desired_perf)
		return ret;

	cpu_data->perf_ctrls.desired_perf = desired_perf;
>>>>>>> upstream/android-13
	freqs.old = policy->cur;
	freqs.new = target_freq;

	cpufreq_freq_transition_begin(policy, &freqs);
<<<<<<< HEAD
	ret = cppc_set_perf(cpu->cpu, &cpu->perf_ctrls);
=======
	ret = cppc_set_perf(cpu, &cpu_data->perf_ctrls);
>>>>>>> upstream/android-13
	cpufreq_freq_transition_end(policy, &freqs, ret != 0);

	if (ret)
		pr_debug("Failed to set target on CPU:%d. ret:%d\n",
<<<<<<< HEAD
				cpu->cpu, ret);
=======
			 cpu, ret);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static int cppc_verify_policy(struct cpufreq_policy *policy)
=======
static int cppc_verify_policy(struct cpufreq_policy_data *policy)
>>>>>>> upstream/android-13
{
	cpufreq_verify_within_cpu_limits(policy);
	return 0;
}

<<<<<<< HEAD
static void cppc_cpufreq_stop_cpu(struct cpufreq_policy *policy)
{
	int cpu_num = policy->cpu;
	struct cppc_cpudata *cpu = all_cpu_data[cpu_num];
	int ret;

	cpu->perf_ctrls.desired_perf = cpu->perf_caps.lowest_perf;

	ret = cppc_set_perf(cpu_num, &cpu->perf_ctrls);
	if (ret)
		pr_debug("Err setting perf value:%d on CPU:%d. ret:%d\n",
				cpu->perf_caps.lowest_perf, cpu_num, ret);
}

/*
 * The PCC subspace describes the rate at which platform can accept commands
 * on the shared PCC channel (including READs which do not count towards freq
 * trasition requests), so ideally we need to use the PCC values as a fallback
=======
/*
 * The PCC subspace describes the rate at which platform can accept commands
 * on the shared PCC channel (including READs which do not count towards freq
 * transition requests), so ideally we need to use the PCC values as a fallback
>>>>>>> upstream/android-13
 * if we don't have a platform specific transition_delay_us
 */
#ifdef CONFIG_ARM64
#include <asm/cputype.h>

<<<<<<< HEAD
static unsigned int cppc_cpufreq_get_transition_delay_us(int cpu)
{
	unsigned long implementor = read_cpuid_implementor();
	unsigned long part_num = read_cpuid_part_number();
	unsigned int delay_us = 0;
=======
static unsigned int cppc_cpufreq_get_transition_delay_us(unsigned int cpu)
{
	unsigned long implementor = read_cpuid_implementor();
	unsigned long part_num = read_cpuid_part_number();
>>>>>>> upstream/android-13

	switch (implementor) {
	case ARM_CPU_IMP_QCOM:
		switch (part_num) {
		case QCOM_CPU_PART_FALKOR_V1:
		case QCOM_CPU_PART_FALKOR:
<<<<<<< HEAD
			delay_us = 10000;
			break;
		default:
			delay_us = cppc_get_transition_latency(cpu) / NSEC_PER_USEC;
			break;
		}
		break;
	default:
		delay_us = cppc_get_transition_latency(cpu) / NSEC_PER_USEC;
		break;
	}

	return delay_us;
=======
			return 10000;
		}
	}
	return cppc_get_transition_latency(cpu) / NSEC_PER_USEC;
>>>>>>> upstream/android-13
}

#else

<<<<<<< HEAD
static unsigned int cppc_cpufreq_get_transition_delay_us(int cpu)
=======
static unsigned int cppc_cpufreq_get_transition_delay_us(unsigned int cpu)
>>>>>>> upstream/android-13
{
	return cppc_get_transition_latency(cpu) / NSEC_PER_USEC;
}
#endif

<<<<<<< HEAD
static int cppc_cpufreq_cpu_init(struct cpufreq_policy *policy)
{
	struct cppc_cpudata *cpu;
	unsigned int cpu_num = policy->cpu;
	int ret = 0;

	cpu = all_cpu_data[policy->cpu];

	cpu->cpu = cpu_num;
	ret = cppc_get_perf_caps(policy->cpu, &cpu->perf_caps);

	if (ret) {
		pr_debug("Err reading CPU%d perf capabilities. ret:%d\n",
				cpu_num, ret);
		return ret;
	}

	/* Convert the lowest and nominal freq from MHz to KHz */
	cpu->perf_caps.lowest_freq *= 1000;
	cpu->perf_caps.nominal_freq *= 1000;
=======

static struct cppc_cpudata *cppc_cpufreq_get_cpu_data(unsigned int cpu)
{
	struct cppc_cpudata *cpu_data;
	int ret;

	cpu_data = kzalloc(sizeof(struct cppc_cpudata), GFP_KERNEL);
	if (!cpu_data)
		goto out;

	if (!zalloc_cpumask_var(&cpu_data->shared_cpu_map, GFP_KERNEL))
		goto free_cpu;

	ret = acpi_get_psd_map(cpu, cpu_data);
	if (ret) {
		pr_debug("Err parsing CPU%d PSD data: ret:%d\n", cpu, ret);
		goto free_mask;
	}

	ret = cppc_get_perf_caps(cpu, &cpu_data->perf_caps);
	if (ret) {
		pr_debug("Err reading CPU%d perf caps: ret:%d\n", cpu, ret);
		goto free_mask;
	}

	/* Convert the lowest and nominal freq from MHz to KHz */
	cpu_data->perf_caps.lowest_freq *= 1000;
	cpu_data->perf_caps.nominal_freq *= 1000;

	list_add(&cpu_data->node, &cpu_data_list);

	return cpu_data;

free_mask:
	free_cpumask_var(cpu_data->shared_cpu_map);
free_cpu:
	kfree(cpu_data);
out:
	return NULL;
}

static void cppc_cpufreq_put_cpu_data(struct cpufreq_policy *policy)
{
	struct cppc_cpudata *cpu_data = policy->driver_data;

	list_del(&cpu_data->node);
	free_cpumask_var(cpu_data->shared_cpu_map);
	kfree(cpu_data);
	policy->driver_data = NULL;
}

static int cppc_cpufreq_cpu_init(struct cpufreq_policy *policy)
{
	unsigned int cpu = policy->cpu;
	struct cppc_cpudata *cpu_data;
	struct cppc_perf_caps *caps;
	int ret;

	cpu_data = cppc_cpufreq_get_cpu_data(cpu);
	if (!cpu_data) {
		pr_err("Error in acquiring _CPC/_PSD data for CPU%d.\n", cpu);
		return -ENODEV;
	}
	caps = &cpu_data->perf_caps;
	policy->driver_data = cpu_data;
>>>>>>> upstream/android-13

	/*
	 * Set min to lowest nonlinear perf to avoid any efficiency penalty (see
	 * Section 8.4.7.1.1.5 of ACPI 6.1 spec)
	 */
<<<<<<< HEAD
	policy->min = cppc_cpufreq_perf_to_khz(cpu, cpu->perf_caps.lowest_nonlinear_perf);
	policy->max = cppc_cpufreq_perf_to_khz(cpu, cpu->perf_caps.highest_perf);
=======
	policy->min = cppc_cpufreq_perf_to_khz(cpu_data,
					       caps->lowest_nonlinear_perf);
	policy->max = cppc_cpufreq_perf_to_khz(cpu_data,
					       caps->nominal_perf);
>>>>>>> upstream/android-13

	/*
	 * Set cpuinfo.min_freq to Lowest to make the full range of performance
	 * available if userspace wants to use any perf between lowest & lowest
	 * nonlinear perf
	 */
<<<<<<< HEAD
	policy->cpuinfo.min_freq = cppc_cpufreq_perf_to_khz(cpu, cpu->perf_caps.lowest_perf);
	policy->cpuinfo.max_freq = cppc_cpufreq_perf_to_khz(cpu, cpu->perf_caps.highest_perf);

	policy->transition_delay_us = cppc_cpufreq_get_transition_delay_us(cpu_num);
	policy->shared_type = cpu->shared_type;

	if (policy->shared_type == CPUFREQ_SHARED_TYPE_ANY) {
		int i;

		cpumask_copy(policy->cpus, cpu->shared_cpu_map);

		for_each_cpu(i, policy->cpus) {
			if (unlikely(i == policy->cpu))
				continue;

			memcpy(&all_cpu_data[i]->perf_caps, &cpu->perf_caps,
			       sizeof(cpu->perf_caps));
		}
	} else if (policy->shared_type == CPUFREQ_SHARED_TYPE_ALL) {
		/* Support only SW_ANY for now. */
		pr_debug("Unsupported CPU co-ord type\n");
		return -EFAULT;
	}

	cpu->cur_policy = policy;

	/* Set policy->cur to max now. The governors will adjust later. */
	policy->cur = cppc_cpufreq_perf_to_khz(cpu,
					cpu->perf_caps.highest_perf);
	cpu->perf_ctrls.desired_perf = cpu->perf_caps.highest_perf;

	ret = cppc_set_perf(cpu_num, &cpu->perf_ctrls);
	if (ret)
		pr_debug("Err setting perf value:%d on CPU:%d. ret:%d\n",
				cpu->perf_caps.highest_perf, cpu_num, ret);

	return ret;
=======
	policy->cpuinfo.min_freq = cppc_cpufreq_perf_to_khz(cpu_data,
							    caps->lowest_perf);
	policy->cpuinfo.max_freq = cppc_cpufreq_perf_to_khz(cpu_data,
							    caps->nominal_perf);

	policy->transition_delay_us = cppc_cpufreq_get_transition_delay_us(cpu);
	policy->shared_type = cpu_data->shared_type;

	switch (policy->shared_type) {
	case CPUFREQ_SHARED_TYPE_HW:
	case CPUFREQ_SHARED_TYPE_NONE:
		/* Nothing to be done - we'll have a policy for each CPU */
		break;
	case CPUFREQ_SHARED_TYPE_ANY:
		/*
		 * All CPUs in the domain will share a policy and all cpufreq
		 * operations will use a single cppc_cpudata structure stored
		 * in policy->driver_data.
		 */
		cpumask_copy(policy->cpus, cpu_data->shared_cpu_map);
		break;
	default:
		pr_debug("Unsupported CPU co-ord type: %d\n",
			 policy->shared_type);
		ret = -EFAULT;
		goto out;
	}

	/*
	 * If 'highest_perf' is greater than 'nominal_perf', we assume CPU Boost
	 * is supported.
	 */
	if (caps->highest_perf > caps->nominal_perf)
		boost_supported = true;

	/* Set policy->cur to max now. The governors will adjust later. */
	policy->cur = cppc_cpufreq_perf_to_khz(cpu_data, caps->highest_perf);
	cpu_data->perf_ctrls.desired_perf =  caps->highest_perf;

	ret = cppc_set_perf(cpu, &cpu_data->perf_ctrls);
	if (ret) {
		pr_debug("Err setting perf value:%d on CPU:%d. ret:%d\n",
			 caps->highest_perf, cpu, ret);
		goto out;
	}

	cppc_cpufreq_cpu_fie_init(policy);
	return 0;

out:
	cppc_cpufreq_put_cpu_data(policy);
	return ret;
}

static int cppc_cpufreq_cpu_exit(struct cpufreq_policy *policy)
{
	struct cppc_cpudata *cpu_data = policy->driver_data;
	struct cppc_perf_caps *caps = &cpu_data->perf_caps;
	unsigned int cpu = policy->cpu;
	int ret;

	cppc_cpufreq_cpu_fie_exit(policy);

	cpu_data->perf_ctrls.desired_perf = caps->lowest_perf;

	ret = cppc_set_perf(cpu, &cpu_data->perf_ctrls);
	if (ret)
		pr_debug("Err setting perf value:%d on CPU:%d. ret:%d\n",
			 caps->lowest_perf, cpu, ret);

	cppc_cpufreq_put_cpu_data(policy);
	return 0;
>>>>>>> upstream/android-13
}

static inline u64 get_delta(u64 t1, u64 t0)
{
	if (t1 > t0 || t0 > ~(u32)0)
		return t1 - t0;

	return (u32)t1 - (u32)t0;
}

<<<<<<< HEAD
static int cppc_get_rate_from_fbctrs(struct cppc_cpudata *cpu,
				     struct cppc_perf_fb_ctrs fb_ctrs_t0,
				     struct cppc_perf_fb_ctrs fb_ctrs_t1)
{
	u64 delta_reference, delta_delivered;
	u64 reference_perf, delivered_perf;

	reference_perf = fb_ctrs_t0.reference_perf;

	delta_reference = get_delta(fb_ctrs_t1.reference,
				    fb_ctrs_t0.reference);
	delta_delivered = get_delta(fb_ctrs_t1.delivered,
				    fb_ctrs_t0.delivered);

	/* Check to avoid divide-by zero */
	if (delta_reference || delta_delivered)
		delivered_perf = (reference_perf * delta_delivered) /
					delta_reference;
	else
		delivered_perf = cpu->perf_ctrls.desired_perf;

	return cppc_cpufreq_perf_to_khz(cpu, delivered_perf);
}

static unsigned int cppc_cpufreq_get_rate(unsigned int cpunum)
{
	struct cppc_perf_fb_ctrs fb_ctrs_t0 = {0}, fb_ctrs_t1 = {0};
	struct cppc_cpudata *cpu = all_cpu_data[cpunum];
	int ret;

	ret = cppc_get_perf_ctrs(cpunum, &fb_ctrs_t0);
=======
static int cppc_perf_from_fbctrs(struct cppc_cpudata *cpu_data,
				 struct cppc_perf_fb_ctrs *fb_ctrs_t0,
				 struct cppc_perf_fb_ctrs *fb_ctrs_t1)
{
	u64 delta_reference, delta_delivered;
	u64 reference_perf;

	reference_perf = fb_ctrs_t0->reference_perf;

	delta_reference = get_delta(fb_ctrs_t1->reference,
				    fb_ctrs_t0->reference);
	delta_delivered = get_delta(fb_ctrs_t1->delivered,
				    fb_ctrs_t0->delivered);

	/* Check to avoid divide-by zero and invalid delivered_perf */
	if (!delta_reference || !delta_delivered)
		return cpu_data->perf_ctrls.desired_perf;

	return (reference_perf * delta_delivered) / delta_reference;
}

static unsigned int cppc_cpufreq_get_rate(unsigned int cpu)
{
	struct cppc_perf_fb_ctrs fb_ctrs_t0 = {0}, fb_ctrs_t1 = {0};
	struct cpufreq_policy *policy = cpufreq_cpu_get(cpu);
	struct cppc_cpudata *cpu_data = policy->driver_data;
	u64 delivered_perf;
	int ret;

	cpufreq_cpu_put(policy);

	ret = cppc_get_perf_ctrs(cpu, &fb_ctrs_t0);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	udelay(2); /* 2usec delay between sampling */

<<<<<<< HEAD
	ret = cppc_get_perf_ctrs(cpunum, &fb_ctrs_t1);
	if (ret)
		return ret;

	return cppc_get_rate_from_fbctrs(cpu, fb_ctrs_t0, fb_ctrs_t1);
}

=======
	ret = cppc_get_perf_ctrs(cpu, &fb_ctrs_t1);
	if (ret)
		return ret;

	delivered_perf = cppc_perf_from_fbctrs(cpu_data, &fb_ctrs_t0,
					       &fb_ctrs_t1);

	return cppc_cpufreq_perf_to_khz(cpu_data, delivered_perf);
}

static int cppc_cpufreq_set_boost(struct cpufreq_policy *policy, int state)
{
	struct cppc_cpudata *cpu_data = policy->driver_data;
	struct cppc_perf_caps *caps = &cpu_data->perf_caps;
	int ret;

	if (!boost_supported) {
		pr_err("BOOST not supported by CPU or firmware\n");
		return -EINVAL;
	}

	if (state)
		policy->max = cppc_cpufreq_perf_to_khz(cpu_data,
						       caps->highest_perf);
	else
		policy->max = cppc_cpufreq_perf_to_khz(cpu_data,
						       caps->nominal_perf);
	policy->cpuinfo.max_freq = policy->max;

	ret = freq_qos_update_request(policy->max_freq_req, policy->max);
	if (ret < 0)
		return ret;

	return 0;
}

static ssize_t show_freqdomain_cpus(struct cpufreq_policy *policy, char *buf)
{
	struct cppc_cpudata *cpu_data = policy->driver_data;

	return cpufreq_show_cpus(cpu_data->shared_cpu_map, buf);
}
cpufreq_freq_attr_ro(freqdomain_cpus);

static struct freq_attr *cppc_cpufreq_attr[] = {
	&freqdomain_cpus,
	NULL,
};

>>>>>>> upstream/android-13
static struct cpufreq_driver cppc_cpufreq_driver = {
	.flags = CPUFREQ_CONST_LOOPS,
	.verify = cppc_verify_policy,
	.target = cppc_cpufreq_set_target,
	.get = cppc_cpufreq_get_rate,
	.init = cppc_cpufreq_cpu_init,
<<<<<<< HEAD
	.stop_cpu = cppc_cpufreq_stop_cpu,
	.name = "cppc_cpufreq",
};

static int __init cppc_cpufreq_init(void)
{
	int i, ret = 0;
	struct cppc_cpudata *cpu;

	if (acpi_disabled)
		return -ENODEV;

	all_cpu_data = kcalloc(num_possible_cpus(), sizeof(void *),
			       GFP_KERNEL);
	if (!all_cpu_data)
		return -ENOMEM;

	for_each_possible_cpu(i) {
		all_cpu_data[i] = kzalloc(sizeof(struct cppc_cpudata), GFP_KERNEL);
		if (!all_cpu_data[i])
			goto out;

		cpu = all_cpu_data[i];
		if (!zalloc_cpumask_var(&cpu->shared_cpu_map, GFP_KERNEL))
			goto out;
	}

	ret = acpi_get_psd_map(all_cpu_data);
	if (ret) {
		pr_debug("Error parsing PSD data. Aborting cpufreq registration.\n");
		goto out;
	}

	ret = cpufreq_register_driver(&cppc_cpufreq_driver);
	if (ret)
		goto out;

	return ret;

out:
	for_each_possible_cpu(i) {
		cpu = all_cpu_data[i];
		if (!cpu)
			break;
		free_cpumask_var(cpu->shared_cpu_map);
		kfree(cpu);
	}

	kfree(all_cpu_data);
	return -ENODEV;
=======
	.exit = cppc_cpufreq_cpu_exit,
	.set_boost = cppc_cpufreq_set_boost,
	.attr = cppc_cpufreq_attr,
	.name = "cppc_cpufreq",
};

/*
 * HISI platform does not support delivered performance counter and
 * reference performance counter. It can calculate the performance using the
 * platform specific mechanism. We reuse the desired performance register to
 * store the real performance calculated by the platform.
 */
static unsigned int hisi_cppc_cpufreq_get_rate(unsigned int cpu)
{
	struct cpufreq_policy *policy = cpufreq_cpu_get(cpu);
	struct cppc_cpudata *cpu_data = policy->driver_data;
	u64 desired_perf;
	int ret;

	cpufreq_cpu_put(policy);

	ret = cppc_get_desired_perf(cpu, &desired_perf);
	if (ret < 0)
		return -EIO;

	return cppc_cpufreq_perf_to_khz(cpu_data, desired_perf);
}

static void cppc_check_hisi_workaround(void)
{
	struct acpi_table_header *tbl;
	acpi_status status = AE_OK;
	int i;

	status = acpi_get_table(ACPI_SIG_PCCT, 0, &tbl);
	if (ACPI_FAILURE(status) || !tbl)
		return;

	for (i = 0; i < ARRAY_SIZE(wa_info); i++) {
		if (!memcmp(wa_info[i].oem_id, tbl->oem_id, ACPI_OEM_ID_SIZE) &&
		    !memcmp(wa_info[i].oem_table_id, tbl->oem_table_id, ACPI_OEM_TABLE_ID_SIZE) &&
		    wa_info[i].oem_revision == tbl->oem_revision) {
			/* Overwrite the get() callback */
			cppc_cpufreq_driver.get = hisi_cppc_cpufreq_get_rate;
			break;
		}
	}

	acpi_put_table(tbl);
}

static int __init cppc_cpufreq_init(void)
{
	int ret;

	if ((acpi_disabled) || !acpi_cpc_valid())
		return -ENODEV;

	INIT_LIST_HEAD(&cpu_data_list);

	cppc_check_hisi_workaround();
	cppc_freq_invariance_init();

	ret = cpufreq_register_driver(&cppc_cpufreq_driver);
	if (ret)
		cppc_freq_invariance_exit();

	return ret;
}

static inline void free_cpu_data(void)
{
	struct cppc_cpudata *iter, *tmp;

	list_for_each_entry_safe(iter, tmp, &cpu_data_list, node) {
		free_cpumask_var(iter->shared_cpu_map);
		list_del(&iter->node);
		kfree(iter);
	}

>>>>>>> upstream/android-13
}

static void __exit cppc_cpufreq_exit(void)
{
<<<<<<< HEAD
	struct cppc_cpudata *cpu;
	int i;

	cpufreq_unregister_driver(&cppc_cpufreq_driver);

	for_each_possible_cpu(i) {
		cpu = all_cpu_data[i];
		free_cpumask_var(cpu->shared_cpu_map);
		kfree(cpu);
	}

	kfree(all_cpu_data);
=======
	cpufreq_unregister_driver(&cppc_cpufreq_driver);
	cppc_freq_invariance_exit();

	free_cpu_data();
>>>>>>> upstream/android-13
}

module_exit(cppc_cpufreq_exit);
MODULE_AUTHOR("Ashwin Chaugule");
MODULE_DESCRIPTION("CPUFreq driver based on the ACPI CPPC v5.0+ spec");
MODULE_LICENSE("GPL");

late_initcall(cppc_cpufreq_init);

<<<<<<< HEAD
static const struct acpi_device_id cppc_acpi_ids[] = {
=======
static const struct acpi_device_id cppc_acpi_ids[] __used = {
>>>>>>> upstream/android-13
	{ACPI_PROCESSOR_DEVICE_HID, },
	{}
};

MODULE_DEVICE_TABLE(acpi, cppc_acpi_ids);
