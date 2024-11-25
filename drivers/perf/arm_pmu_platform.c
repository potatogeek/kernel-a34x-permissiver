// SPDX-License-Identifier: GPL-2.0
/*
 * platform_device probing code for ARM performance counters.
 *
 * Copyright (C) 2009 picoChip Designs, Ltd., Jamie Iles
 * Copyright (C) 2010 ARM Ltd., Will Deacon <will.deacon@arm.com>
 */
#define pr_fmt(fmt) "hw perfevents: " fmt
<<<<<<< HEAD
=======
#define dev_fmt pr_fmt
>>>>>>> upstream/android-13

#include <linux/bug.h>
#include <linux/cpumask.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/irq.h>
#include <linux/irqdesc.h>
#include <linux/kconfig.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/percpu.h>
#include <linux/perf/arm_pmu.h>
#include <linux/platform_device.h>
#include <linux/printk.h>
#include <linux/smp.h>

static int probe_current_pmu(struct arm_pmu *pmu,
			     const struct pmu_probe_info *info)
{
	int cpu = get_cpu();
	unsigned int cpuid = read_cpuid_id();
	int ret = -ENODEV;

	pr_info("probing PMU on CPU %d\n", cpu);

	for (; info->init != NULL; info++) {
		if ((cpuid & info->mask) != info->cpuid)
			continue;
		ret = info->init(pmu);
		break;
	}

	put_cpu();
	return ret;
}

static int pmu_parse_percpu_irq(struct arm_pmu *pmu, int irq)
{
	int cpu, ret;
	struct pmu_hw_events __percpu *hw_events = pmu->hw_events;

	ret = irq_get_percpu_devid_partition(irq, &pmu->supported_cpus);
	if (ret)
		return ret;

	for_each_cpu(cpu, &pmu->supported_cpus)
		per_cpu(hw_events->irq, cpu) = irq;

	return 0;
}

static bool pmu_has_irq_affinity(struct device_node *node)
{
	return !!of_find_property(node, "interrupt-affinity", NULL);
}

<<<<<<< HEAD
static int pmu_parse_irq_affinity(struct device_node *node, int i)
=======
static int pmu_parse_irq_affinity(struct device *dev, int i)
>>>>>>> upstream/android-13
{
	struct device_node *dn;
	int cpu;

	/*
	 * If we don't have an interrupt-affinity property, we guess irq
	 * affinity matches our logical CPU order, as we used to assume.
	 * This is fragile, so we'll warn in pmu_parse_irqs().
	 */
<<<<<<< HEAD
	if (!pmu_has_irq_affinity(node))
		return i;

	dn = of_parse_phandle(node, "interrupt-affinity", i);
	if (!dn) {
		pr_warn("failed to parse interrupt-affinity[%d] for %s\n",
			i, node->name);
=======
	if (!pmu_has_irq_affinity(dev->of_node))
		return i;

	dn = of_parse_phandle(dev->of_node, "interrupt-affinity", i);
	if (!dn) {
		dev_warn(dev, "failed to parse interrupt-affinity[%d]\n", i);
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	cpu = of_cpu_node_to_id(dn);
	if (cpu < 0) {
<<<<<<< HEAD
		pr_warn("failed to find logical CPU for %s\n", dn->name);
=======
		dev_warn(dev, "failed to find logical CPU for %pOFn\n", dn);
>>>>>>> upstream/android-13
		cpu = nr_cpu_ids;
	}

	of_node_put(dn);

	return cpu;
}

static int pmu_parse_irqs(struct arm_pmu *pmu)
{
	int i = 0, num_irqs;
	struct platform_device *pdev = pmu->plat_device;
	struct pmu_hw_events __percpu *hw_events = pmu->hw_events;
<<<<<<< HEAD

	num_irqs = platform_irq_count(pdev);
	if (num_irqs < 0) {
		pr_err("unable to count PMU IRQs\n");
		return num_irqs;
	}
=======
	struct device *dev = &pdev->dev;

	num_irqs = platform_irq_count(pdev);
	if (num_irqs < 0)
		return dev_err_probe(dev, num_irqs, "unable to count PMU IRQs\n");
>>>>>>> upstream/android-13

	/*
	 * In this case we have no idea which CPUs are covered by the PMU.
	 * To match our prior behaviour, we assume all CPUs in this case.
	 */
	if (num_irqs == 0) {
<<<<<<< HEAD
		pr_warn("no irqs for PMU, sampling events not supported\n");
=======
		dev_warn(dev, "no irqs for PMU, sampling events not supported\n");
>>>>>>> upstream/android-13
		pmu->pmu.capabilities |= PERF_PMU_CAP_NO_INTERRUPT;
		cpumask_setall(&pmu->supported_cpus);
		return 0;
	}

	if (num_irqs == 1) {
		int irq = platform_get_irq(pdev, 0);
		if (irq && irq_is_percpu_devid(irq))
			return pmu_parse_percpu_irq(pmu, irq);
	}

<<<<<<< HEAD
	if (nr_cpu_ids != 1 && !pmu_has_irq_affinity(pdev->dev.of_node)) {
		pr_warn("no interrupt-affinity property for %pOF, guessing.\n",
			pdev->dev.of_node);
	}
=======
	if (nr_cpu_ids != 1 && !pmu_has_irq_affinity(dev->of_node))
		dev_warn(dev, "no interrupt-affinity property, guessing.\n");
>>>>>>> upstream/android-13

	for (i = 0; i < num_irqs; i++) {
		int cpu, irq;

		irq = platform_get_irq(pdev, i);
		if (WARN_ON(irq <= 0))
			continue;

		if (irq_is_percpu_devid(irq)) {
<<<<<<< HEAD
			pr_warn("multiple PPIs or mismatched SPI/PPI detected\n");
			return -EINVAL;
		}

		cpu = pmu_parse_irq_affinity(pdev->dev.of_node, i);
=======
			dev_warn(dev, "multiple PPIs or mismatched SPI/PPI detected\n");
			return -EINVAL;
		}

		cpu = pmu_parse_irq_affinity(dev, i);
>>>>>>> upstream/android-13
		if (cpu < 0)
			return cpu;
		if (cpu >= nr_cpu_ids)
			continue;

		if (per_cpu(hw_events->irq, cpu)) {
<<<<<<< HEAD
			pr_warn("multiple PMU IRQs for the same CPU detected\n");
=======
			dev_warn(dev, "multiple PMU IRQs for the same CPU detected\n");
>>>>>>> upstream/android-13
			return -EINVAL;
		}

		per_cpu(hw_events->irq, cpu) = irq;
		cpumask_set_cpu(cpu, &pmu->supported_cpus);
	}

	return 0;
}

static int armpmu_request_irqs(struct arm_pmu *armpmu)
{
	struct pmu_hw_events __percpu *hw_events = armpmu->hw_events;
	int cpu, err = 0;

	for_each_cpu(cpu, &armpmu->supported_cpus) {
		int irq = per_cpu(hw_events->irq, cpu);
		if (!irq)
			continue;

		err = armpmu_request_irq(irq, cpu);
		if (err)
			break;
	}

	return err;
}

static void armpmu_free_irqs(struct arm_pmu *armpmu)
{
	int cpu;
	struct pmu_hw_events __percpu *hw_events = armpmu->hw_events;

	for_each_cpu(cpu, &armpmu->supported_cpus) {
		int irq = per_cpu(hw_events->irq, cpu);

		armpmu_free_irq(irq, cpu);
	}
}

int arm_pmu_device_probe(struct platform_device *pdev,
			 const struct of_device_id *of_table,
			 const struct pmu_probe_info *probe_table)
{
<<<<<<< HEAD
	const struct of_device_id *of_id;
	armpmu_init_fn init_fn;
	struct device_node *node = pdev->dev.of_node;
=======
	armpmu_init_fn init_fn;
	struct device *dev = &pdev->dev;
>>>>>>> upstream/android-13
	struct arm_pmu *pmu;
	int ret = -ENODEV;

	pmu = armpmu_alloc();
	if (!pmu)
		return -ENOMEM;

	pmu->plat_device = pdev;

	ret = pmu_parse_irqs(pmu);
	if (ret)
		goto out_free;

<<<<<<< HEAD
	if (node && (of_id = of_match_node(of_table, pdev->dev.of_node))) {
		init_fn = of_id->data;

		pmu->secure_access = of_property_read_bool(pdev->dev.of_node,
=======
	init_fn = of_device_get_match_data(dev);
	if (init_fn) {
		pmu->secure_access = of_property_read_bool(dev->of_node,
>>>>>>> upstream/android-13
							   "secure-reg-access");

		/* arm64 systems boot only as non-secure */
		if (IS_ENABLED(CONFIG_ARM64) && pmu->secure_access) {
<<<<<<< HEAD
			pr_warn("ignoring \"secure-reg-access\" property for arm64\n");
=======
			dev_warn(dev, "ignoring \"secure-reg-access\" property for arm64\n");
>>>>>>> upstream/android-13
			pmu->secure_access = false;
		}

		ret = init_fn(pmu);
	} else if (probe_table) {
		cpumask_setall(&pmu->supported_cpus);
		ret = probe_current_pmu(pmu, probe_table);
	}

	if (ret) {
<<<<<<< HEAD
		pr_info("%pOF: failed to probe PMU!\n", node);
=======
		dev_err(dev, "failed to probe PMU!\n");
>>>>>>> upstream/android-13
		goto out_free;
	}

	ret = armpmu_request_irqs(pmu);
	if (ret)
		goto out_free_irqs;

	ret = armpmu_register(pmu);
<<<<<<< HEAD
	if (ret)
		goto out_free_irqs;
=======
	if (ret) {
		dev_err(dev, "failed to register PMU devices!\n");
		goto out_free_irqs;
	}
>>>>>>> upstream/android-13

	return 0;

out_free_irqs:
	armpmu_free_irqs(pmu);
out_free:
<<<<<<< HEAD
	pr_info("%pOF: failed to register PMU devices!\n", node);
=======
>>>>>>> upstream/android-13
	armpmu_free(pmu);
	return ret;
}
