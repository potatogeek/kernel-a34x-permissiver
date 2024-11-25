// SPDX-License-Identifier: GPL-2.0
/*
 * System Control and Power Interface (SCMI) based CPUFreq Interface driver
 *
<<<<<<< HEAD
 * Copyright (C) 2018 ARM Ltd.
=======
 * Copyright (C) 2018-2021 ARM Ltd.
>>>>>>> upstream/android-13
 * Sudeep Holla <sudeep.holla@arm.com>
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

<<<<<<< HEAD
#include <linux/cpu.h>
#include <linux/cpufreq.h>
#include <linux/cpumask.h>
#include <linux/cpu_cooling.h>
=======
#include <linux/clk-provider.h>
#include <linux/cpu.h>
#include <linux/cpufreq.h>
#include <linux/cpumask.h>
>>>>>>> upstream/android-13
#include <linux/energy_model.h>
#include <linux/export.h>
#include <linux/module.h>
#include <linux/pm_opp.h>
#include <linux/slab.h>
#include <linux/scmi_protocol.h>
#include <linux/types.h>

struct scmi_data {
	int domain_id;
<<<<<<< HEAD
	struct device *cpu_dev;
	struct thermal_cooling_device *cdev;
};

static const struct scmi_handle *handle;
=======
	int nr_opp;
	struct device *cpu_dev;
	cpumask_var_t opp_shared_cpus;
};

static struct scmi_protocol_handle *ph;
static const struct scmi_perf_proto_ops *perf_ops;
>>>>>>> upstream/android-13

static unsigned int scmi_cpufreq_get_rate(unsigned int cpu)
{
	struct cpufreq_policy *policy = cpufreq_cpu_get_raw(cpu);
<<<<<<< HEAD
	struct scmi_perf_ops *perf_ops = handle->perf_ops;
=======
>>>>>>> upstream/android-13
	struct scmi_data *priv = policy->driver_data;
	unsigned long rate;
	int ret;

<<<<<<< HEAD
	ret = perf_ops->freq_get(handle, priv->domain_id, &rate, false);
=======
	ret = perf_ops->freq_get(ph, priv->domain_id, &rate, false);
>>>>>>> upstream/android-13
	if (ret)
		return 0;
	return rate / 1000;
}

/*
 * perf_ops->freq_set is not a synchronous, the actual OPP change will
 * happen asynchronously and can get notified if the events are
 * subscribed for by the SCMI firmware
 */
static int
scmi_cpufreq_set_target(struct cpufreq_policy *policy, unsigned int index)
{
<<<<<<< HEAD
	int ret;
	struct scmi_data *priv = policy->driver_data;
	struct scmi_perf_ops *perf_ops = handle->perf_ops;
	u64 freq = policy->freq_table[index].frequency;

	ret = perf_ops->freq_set(handle, priv->domain_id, freq * 1000, false);
	if (!ret)
		arch_set_freq_scale(policy->related_cpus, freq,
				    policy->cpuinfo.max_freq);
	return ret;
=======
	struct scmi_data *priv = policy->driver_data;
	u64 freq = policy->freq_table[index].frequency;

	return perf_ops->freq_set(ph, priv->domain_id, freq * 1000, false);
>>>>>>> upstream/android-13
}

static unsigned int scmi_cpufreq_fast_switch(struct cpufreq_policy *policy,
					     unsigned int target_freq)
{
	struct scmi_data *priv = policy->driver_data;
<<<<<<< HEAD
	struct scmi_perf_ops *perf_ops = handle->perf_ops;

	if (!perf_ops->freq_set(handle, priv->domain_id,
				target_freq * 1000, true)) {
		arch_set_freq_scale(policy->related_cpus, target_freq,
				    policy->cpuinfo.max_freq);
		return target_freq;
	}
=======

	if (!perf_ops->freq_set(ph, priv->domain_id,
				target_freq * 1000, true))
		return target_freq;
>>>>>>> upstream/android-13

	return 0;
}

static int
scmi_get_sharing_cpus(struct device *cpu_dev, struct cpumask *cpumask)
{
	int cpu, domain, tdomain;
	struct device *tcpu_dev;

<<<<<<< HEAD
	domain = handle->perf_ops->device_domain_id(cpu_dev);
=======
	domain = perf_ops->device_domain_id(cpu_dev);
>>>>>>> upstream/android-13
	if (domain < 0)
		return domain;

	for_each_possible_cpu(cpu) {
		if (cpu == cpu_dev->id)
			continue;

		tcpu_dev = get_cpu_device(cpu);
		if (!tcpu_dev)
			continue;

<<<<<<< HEAD
		tdomain = handle->perf_ops->device_domain_id(tcpu_dev);
=======
		tdomain = perf_ops->device_domain_id(tcpu_dev);
>>>>>>> upstream/android-13
		if (tdomain == domain)
			cpumask_set_cpu(cpu, cpumask);
	}

	return 0;
}

static int __maybe_unused
<<<<<<< HEAD
scmi_get_cpu_power(unsigned long *power, unsigned long *KHz, int cpu)
{
	struct device *cpu_dev = get_cpu_device(cpu);
	unsigned long Hz;
	int ret, domain;

	if (!cpu_dev) {
		pr_err("failed to get cpu%d device\n", cpu);
		return -ENODEV;
	}

	domain = handle->perf_ops->device_domain_id(cpu_dev);
=======
scmi_get_cpu_power(unsigned long *power, unsigned long *KHz,
		   struct device *cpu_dev)
{
	unsigned long Hz;
	int ret, domain;

	domain = perf_ops->device_domain_id(cpu_dev);
>>>>>>> upstream/android-13
	if (domain < 0)
		return domain;

	/* Get the power cost of the performance domain. */
	Hz = *KHz * 1000;
<<<<<<< HEAD
	ret = handle->perf_ops->est_power_get(handle, domain, &Hz, power);
=======
	ret = perf_ops->est_power_get(ph, domain, &Hz, power);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	/* The EM framework specifies the frequency in KHz. */
	*KHz = Hz / 1000;

	return 0;
}

static int scmi_cpufreq_init(struct cpufreq_policy *policy)
{
	int ret, nr_opp;
	unsigned int latency;
	struct device *cpu_dev;
	struct scmi_data *priv;
	struct cpufreq_frequency_table *freq_table;
<<<<<<< HEAD
	struct em_data_callback em_cb = EM_DATA_CB(scmi_get_cpu_power);
=======
>>>>>>> upstream/android-13

	cpu_dev = get_cpu_device(policy->cpu);
	if (!cpu_dev) {
		pr_err("failed to get cpu%d device\n", policy->cpu);
		return -ENODEV;
	}

<<<<<<< HEAD
	ret = handle->perf_ops->device_opps_add(handle, cpu_dev);
	if (ret) {
		dev_warn(cpu_dev, "failed to add opps to the device\n");
		return ret;
	}

	ret = scmi_get_sharing_cpus(cpu_dev, policy->cpus);
	if (ret) {
		dev_warn(cpu_dev, "failed to get sharing cpumask\n");
		return ret;
	}

	ret = dev_pm_opp_set_sharing_cpus(cpu_dev, policy->cpus);
	if (ret) {
		dev_err(cpu_dev, "%s: failed to mark OPPs as shared: %d\n",
			__func__, ret);
		return ret;
	}

	ret = dev_pm_opp_get_opp_count(cpu_dev);
	if (ret <= 0) {
		dev_dbg(cpu_dev, "OPP table is not ready, deferring probe\n");
		ret = -EPROBE_DEFER;
		goto out_free_opp;
	}
	nr_opp = ret;

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		ret = -ENOMEM;
		goto out_free_opp;
=======
	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	if (!zalloc_cpumask_var(&priv->opp_shared_cpus, GFP_KERNEL)) {
		ret = -ENOMEM;
		goto out_free_priv;
	}

	/* Obtain CPUs that share SCMI performance controls */
	ret = scmi_get_sharing_cpus(cpu_dev, policy->cpus);
	if (ret) {
		dev_warn(cpu_dev, "failed to get sharing cpumask\n");
		goto out_free_cpumask;
	}

	/*
	 * Obtain CPUs that share performance levels.
	 * The OPP 'sharing cpus' info may come from DT through an empty opp
	 * table and opp-shared.
	 */
	ret = dev_pm_opp_of_get_sharing_cpus(cpu_dev, priv->opp_shared_cpus);
	if (ret || !cpumask_weight(priv->opp_shared_cpus)) {
		/*
		 * Either opp-table is not set or no opp-shared was found.
		 * Use the CPU mask from SCMI to designate CPUs sharing an OPP
		 * table.
		 */
		cpumask_copy(priv->opp_shared_cpus, policy->cpus);
	}

	 /*
	  * A previous CPU may have marked OPPs as shared for a few CPUs, based on
	  * what OPP core provided. If the current CPU is part of those few, then
	  * there is no need to add OPPs again.
	  */
	nr_opp = dev_pm_opp_get_opp_count(cpu_dev);
	if (nr_opp <= 0) {
		ret = perf_ops->device_opps_add(ph, cpu_dev);
		if (ret) {
			dev_warn(cpu_dev, "failed to add opps to the device\n");
			goto out_free_cpumask;
		}

		nr_opp = dev_pm_opp_get_opp_count(cpu_dev);
		if (nr_opp <= 0) {
			dev_err(cpu_dev, "%s: No OPPs for this device: %d\n",
				__func__, nr_opp);

			ret = -ENODEV;
			goto out_free_opp;
		}

		ret = dev_pm_opp_set_sharing_cpus(cpu_dev, priv->opp_shared_cpus);
		if (ret) {
			dev_err(cpu_dev, "%s: failed to mark OPPs as shared: %d\n",
				__func__, ret);

			goto out_free_opp;
		}

		priv->nr_opp = nr_opp;
>>>>>>> upstream/android-13
	}

	ret = dev_pm_opp_init_cpufreq_table(cpu_dev, &freq_table);
	if (ret) {
		dev_err(cpu_dev, "failed to init cpufreq table: %d\n", ret);
<<<<<<< HEAD
		goto out_free_priv;
	}

	priv->cpu_dev = cpu_dev;
	priv->domain_id = handle->perf_ops->device_domain_id(cpu_dev);
=======
		goto out_free_opp;
	}

	priv->cpu_dev = cpu_dev;
	priv->domain_id = perf_ops->device_domain_id(cpu_dev);
>>>>>>> upstream/android-13

	policy->driver_data = priv;
	policy->freq_table = freq_table;

	/* SCMI allows DVFS request for any domain from any CPU */
	policy->dvfs_possible_from_any_cpu = true;

<<<<<<< HEAD
	latency = handle->perf_ops->transition_latency_get(handle, cpu_dev);
=======
	latency = perf_ops->transition_latency_get(ph, cpu_dev);
>>>>>>> upstream/android-13
	if (!latency)
		latency = CPUFREQ_ETERNAL;

	policy->cpuinfo.transition_latency = latency;

<<<<<<< HEAD
	policy->fast_switch_possible = true;

	em_register_perf_domain(policy->cpus, nr_opp, &em_cb);

	return 0;

out_free_priv:
	kfree(priv);
out_free_opp:
	dev_pm_opp_cpumask_remove_table(policy->cpus);
=======
	policy->fast_switch_possible =
		perf_ops->fast_switch_possible(ph, cpu_dev);

	return 0;

out_free_opp:
	dev_pm_opp_remove_all_dynamic(cpu_dev);

out_free_cpumask:
	free_cpumask_var(priv->opp_shared_cpus);

out_free_priv:
	kfree(priv);
>>>>>>> upstream/android-13

	return ret;
}

static int scmi_cpufreq_exit(struct cpufreq_policy *policy)
{
	struct scmi_data *priv = policy->driver_data;

<<<<<<< HEAD
	cpufreq_cooling_unregister(priv->cdev);
	dev_pm_opp_free_cpufreq_table(priv->cpu_dev, &policy->freq_table);
	kfree(priv);
	dev_pm_opp_cpumask_remove_table(policy->related_cpus);
=======
	dev_pm_opp_free_cpufreq_table(priv->cpu_dev, &policy->freq_table);
	dev_pm_opp_remove_all_dynamic(priv->cpu_dev);
	free_cpumask_var(priv->opp_shared_cpus);
	kfree(priv);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static void scmi_cpufreq_ready(struct cpufreq_policy *policy)
{
	struct scmi_data *priv = policy->driver_data;

	priv->cdev = of_cpufreq_cooling_register(policy);
=======
static void scmi_cpufreq_register_em(struct cpufreq_policy *policy)
{
	struct em_data_callback em_cb = EM_DATA_CB(scmi_get_cpu_power);
	bool power_scale_mw = perf_ops->power_scale_mw_get(ph);
	struct scmi_data *priv = policy->driver_data;

	/*
	 * This callback will be called for each policy, but we don't need to
	 * register with EM every time. Despite not being part of the same
	 * policy, some CPUs may still share their perf-domains, and a CPU from
	 * another policy may already have registered with EM on behalf of CPUs
	 * of this policy.
	 */
	if (!priv->nr_opp)
		return;

	em_dev_register_perf_domain(get_cpu_device(policy->cpu), priv->nr_opp,
				    &em_cb, priv->opp_shared_cpus,
				    power_scale_mw);
>>>>>>> upstream/android-13
}

static struct cpufreq_driver scmi_cpufreq_driver = {
	.name	= "scmi",
<<<<<<< HEAD
	.flags	= CPUFREQ_STICKY | CPUFREQ_HAVE_GOVERNOR_PER_POLICY |
		  CPUFREQ_NEED_INITIAL_FREQ_CHECK,
=======
	.flags	= CPUFREQ_HAVE_GOVERNOR_PER_POLICY |
		  CPUFREQ_NEED_INITIAL_FREQ_CHECK |
		  CPUFREQ_IS_COOLING_DEV,
>>>>>>> upstream/android-13
	.verify	= cpufreq_generic_frequency_table_verify,
	.attr	= cpufreq_generic_attr,
	.target_index	= scmi_cpufreq_set_target,
	.fast_switch	= scmi_cpufreq_fast_switch,
	.get	= scmi_cpufreq_get_rate,
	.init	= scmi_cpufreq_init,
	.exit	= scmi_cpufreq_exit,
<<<<<<< HEAD
	.ready	= scmi_cpufreq_ready,
=======
	.register_em	= scmi_cpufreq_register_em,
>>>>>>> upstream/android-13
};

static int scmi_cpufreq_probe(struct scmi_device *sdev)
{
	int ret;
<<<<<<< HEAD

	handle = sdev->handle;

	if (!handle || !handle->perf_ops)
		return -ENODEV;

	ret = cpufreq_register_driver(&scmi_cpufreq_driver);
	if (ret) {
		dev_err(&sdev->dev, "%s: registering cpufreq failed, err: %d\n",
=======
	struct device *dev = &sdev->dev;
	const struct scmi_handle *handle;

	handle = sdev->handle;

	if (!handle)
		return -ENODEV;

	perf_ops = handle->devm_protocol_get(sdev, SCMI_PROTOCOL_PERF, &ph);
	if (IS_ERR(perf_ops))
		return PTR_ERR(perf_ops);

#ifdef CONFIG_COMMON_CLK
	/* dummy clock provider as needed by OPP if clocks property is used */
	if (of_find_property(dev->of_node, "#clock-cells", NULL))
		devm_of_clk_add_hw_provider(dev, of_clk_hw_simple_get, NULL);
#endif

	ret = cpufreq_register_driver(&scmi_cpufreq_driver);
	if (ret) {
		dev_err(dev, "%s: registering cpufreq failed, err: %d\n",
>>>>>>> upstream/android-13
			__func__, ret);
	}

	return ret;
}

static void scmi_cpufreq_remove(struct scmi_device *sdev)
{
	cpufreq_unregister_driver(&scmi_cpufreq_driver);
}

static const struct scmi_device_id scmi_id_table[] = {
<<<<<<< HEAD
	{ SCMI_PROTOCOL_PERF },
=======
	{ SCMI_PROTOCOL_PERF, "cpufreq" },
>>>>>>> upstream/android-13
	{ },
};
MODULE_DEVICE_TABLE(scmi, scmi_id_table);

static struct scmi_driver scmi_cpufreq_drv = {
	.name		= "scmi-cpufreq",
	.probe		= scmi_cpufreq_probe,
	.remove		= scmi_cpufreq_remove,
	.id_table	= scmi_id_table,
};
module_scmi_driver(scmi_cpufreq_drv);

MODULE_AUTHOR("Sudeep Holla <sudeep.holla@arm.com>");
MODULE_DESCRIPTION("ARM SCMI CPUFreq interface driver");
MODULE_LICENSE("GPL v2");
