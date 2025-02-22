<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * ARM64 CPU idle arch support
 *
 * Copyright (C) 2014 ARM Ltd.
 * Author: Lorenzo Pieralisi <lorenzo.pieralisi@arm.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/acpi.h>
#include <linux/cpuidle.h>
#include <linux/cpu_pm.h>
#include <linux/of.h>
#include <linux/of_device.h>
<<<<<<< HEAD
=======
#include <linux/psci.h>
>>>>>>> upstream/android-13

#include <asm/cpuidle.h>
#include <asm/cpu_ops.h>

int arm_cpuidle_init(unsigned int cpu)
{
<<<<<<< HEAD
	int ret = -EOPNOTSUPP;

	if (cpu_ops[cpu] && cpu_ops[cpu]->cpu_suspend &&
			cpu_ops[cpu]->cpu_init_idle)
		ret = cpu_ops[cpu]->cpu_init_idle(cpu);
=======
	const struct cpu_operations *ops = get_cpu_ops(cpu);
	int ret = -EOPNOTSUPP;

	if (ops && ops->cpu_suspend && ops->cpu_init_idle)
		ret = ops->cpu_init_idle(cpu);
>>>>>>> upstream/android-13

	return ret;
}

/**
 * arm_cpuidle_suspend() - function to enter a low-power idle state
<<<<<<< HEAD
 * @arg: argument to pass to CPU suspend operations
=======
 * @index: argument to pass to CPU suspend operations
>>>>>>> upstream/android-13
 *
 * Return: 0 on success, -EOPNOTSUPP if CPU suspend hook not initialized, CPU
 * operations back-end error code otherwise.
 */
int arm_cpuidle_suspend(int index)
{
	int cpu = smp_processor_id();
<<<<<<< HEAD

	return cpu_ops[cpu]->cpu_suspend(index);
}
EXPORT_SYMBOL_GPL(arm_cpuidle_suspend);
=======
	const struct cpu_operations *ops = get_cpu_ops(cpu);

	return ops->cpu_suspend(index);
}
>>>>>>> upstream/android-13

#ifdef CONFIG_ACPI

#include <acpi/processor.h>

#define ARM64_LPI_IS_RETENTION_STATE(arch_flags) (!(arch_flags))

<<<<<<< HEAD
int acpi_processor_ffh_lpi_probe(unsigned int cpu)
{
	return arm_cpuidle_init(cpu);
=======
static int psci_acpi_cpu_init_idle(unsigned int cpu)
{
	int i, count;
	struct acpi_lpi_state *lpi;
	struct acpi_processor *pr = per_cpu(processors, cpu);

	if (unlikely(!pr || !pr->flags.has_lpi))
		return -EINVAL;

	/*
	 * If the PSCI cpu_suspend function hook has not been initialized
	 * idle states must not be enabled, so bail out
	 */
	if (!psci_ops.cpu_suspend)
		return -EOPNOTSUPP;

	count = pr->power.count - 1;
	if (count <= 0)
		return -ENODEV;

	for (i = 0; i < count; i++) {
		u32 state;

		lpi = &pr->power.lpi_states[i + 1];
		/*
		 * Only bits[31:0] represent a PSCI power_state while
		 * bits[63:32] must be 0x0 as per ARM ACPI FFH Specification
		 */
		state = lpi->address;
		if (!psci_power_state_is_valid(state)) {
			pr_warn("Invalid PSCI power state %#x\n", state);
			return -EINVAL;
		}
	}

	return 0;
}

int acpi_processor_ffh_lpi_probe(unsigned int cpu)
{
	return psci_acpi_cpu_init_idle(cpu);
>>>>>>> upstream/android-13
}

int acpi_processor_ffh_lpi_enter(struct acpi_lpi_state *lpi)
{
<<<<<<< HEAD
	if (ARM64_LPI_IS_RETENTION_STATE(lpi->arch_flags))
		return CPU_PM_CPU_IDLE_ENTER_RETENTION(arm_cpuidle_suspend,
						lpi->index);
	else
		return CPU_PM_CPU_IDLE_ENTER(arm_cpuidle_suspend, lpi->index);
=======
	u32 state = lpi->address;

	if (ARM64_LPI_IS_RETENTION_STATE(lpi->arch_flags))
		return CPU_PM_CPU_IDLE_ENTER_RETENTION_PARAM(psci_cpu_suspend_enter,
						lpi->index, state);
	else
		return CPU_PM_CPU_IDLE_ENTER_PARAM(psci_cpu_suspend_enter,
					     lpi->index, state);
>>>>>>> upstream/android-13
}
#endif
