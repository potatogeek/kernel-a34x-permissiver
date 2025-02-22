<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13

/*
 * acpi_lpit.c - LPIT table processing functions
 *
 * Copyright (C) 2017 Intel Corporation. All rights reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#include <linux/cpu.h>
#include <linux/acpi.h>
#include <asm/msr.h>
#include <asm/tsc.h>

struct lpit_residency_info {
	struct acpi_generic_address gaddr;
	u64 frequency;
	void __iomem *iomem_addr;
};

/* Storage for an memory mapped and FFH based entries */
static struct lpit_residency_info residency_info_mem;
static struct lpit_residency_info residency_info_ffh;

static int lpit_read_residency_counter_us(u64 *counter, bool io_mem)
{
	int err;

	if (io_mem) {
		u64 count = 0;
		int error;

		error = acpi_os_read_iomem(residency_info_mem.iomem_addr, &count,
					   residency_info_mem.gaddr.bit_width);
		if (error)
			return error;

		*counter = div64_u64(count * 1000000ULL, residency_info_mem.frequency);
		return 0;
	}

	err = rdmsrl_safe(residency_info_ffh.gaddr.address, counter);
	if (!err) {
		u64 mask = GENMASK_ULL(residency_info_ffh.gaddr.bit_offset +
				       residency_info_ffh.gaddr. bit_width - 1,
				       residency_info_ffh.gaddr.bit_offset);

		*counter &= mask;
		*counter >>= residency_info_ffh.gaddr.bit_offset;
		*counter = div64_u64(*counter * 1000000ULL, residency_info_ffh.frequency);
		return 0;
	}

	return -ENODATA;
}

static ssize_t low_power_idle_system_residency_us_show(struct device *dev,
						       struct device_attribute *attr,
						       char *buf)
{
	u64 counter;
	int ret;

	ret = lpit_read_residency_counter_us(&counter, true);
	if (ret)
		return ret;

	return sprintf(buf, "%llu\n", counter);
}
static DEVICE_ATTR_RO(low_power_idle_system_residency_us);

static ssize_t low_power_idle_cpu_residency_us_show(struct device *dev,
						    struct device_attribute *attr,
						    char *buf)
{
	u64 counter;
	int ret;

	ret = lpit_read_residency_counter_us(&counter, false);
	if (ret)
		return ret;

	return sprintf(buf, "%llu\n", counter);
}
static DEVICE_ATTR_RO(low_power_idle_cpu_residency_us);

int lpit_read_residency_count_address(u64 *address)
{
	if (!residency_info_mem.gaddr.address)
		return -EINVAL;

	*address = residency_info_mem.gaddr.address;

	return 0;
}
EXPORT_SYMBOL_GPL(lpit_read_residency_count_address);

static void lpit_update_residency(struct lpit_residency_info *info,
				 struct acpi_lpit_native *lpit_native)
{
	info->frequency = lpit_native->counter_frequency ?
				lpit_native->counter_frequency : tsc_khz * 1000;
	if (!info->frequency)
		info->frequency = 1;

	info->gaddr = lpit_native->residency_counter;
	if (info->gaddr.space_id == ACPI_ADR_SPACE_SYSTEM_MEMORY) {
<<<<<<< HEAD
		info->iomem_addr = ioremap_nocache(info->gaddr.address,
=======
		info->iomem_addr = ioremap(info->gaddr.address,
>>>>>>> upstream/android-13
						   info->gaddr.bit_width / 8);
		if (!info->iomem_addr)
			return;

		if (!(acpi_gbl_FADT.flags & ACPI_FADT_LOW_POWER_S0))
			return;

		/* Silently fail, if cpuidle attribute group is not present */
		sysfs_add_file_to_group(&cpu_subsys.dev_root->kobj,
					&dev_attr_low_power_idle_system_residency_us.attr,
					"cpuidle");
	} else if (info->gaddr.space_id == ACPI_ADR_SPACE_FIXED_HARDWARE) {
		if (!(acpi_gbl_FADT.flags & ACPI_FADT_LOW_POWER_S0))
			return;

		/* Silently fail, if cpuidle attribute group is not present */
		sysfs_add_file_to_group(&cpu_subsys.dev_root->kobj,
					&dev_attr_low_power_idle_cpu_residency_us.attr,
					"cpuidle");
	}
}

static void lpit_process(u64 begin, u64 end)
{
<<<<<<< HEAD
	while (begin + sizeof(struct acpi_lpit_native) < end) {
=======
	while (begin + sizeof(struct acpi_lpit_native) <= end) {
>>>>>>> upstream/android-13
		struct acpi_lpit_native *lpit_native = (struct acpi_lpit_native *)begin;

		if (!lpit_native->header.type && !lpit_native->header.flags) {
			if (lpit_native->residency_counter.space_id == ACPI_ADR_SPACE_SYSTEM_MEMORY &&
			    !residency_info_mem.gaddr.address) {
				lpit_update_residency(&residency_info_mem, lpit_native);
			} else if (lpit_native->residency_counter.space_id == ACPI_ADR_SPACE_FIXED_HARDWARE &&
				   !residency_info_ffh.gaddr.address) {
				lpit_update_residency(&residency_info_ffh, lpit_native);
			}
		}
		begin += lpit_native->header.length;
	}
}

void acpi_init_lpit(void)
{
	acpi_status status;
<<<<<<< HEAD
	u64 lpit_begin;
	struct acpi_table_lpit *lpit;

	status = acpi_get_table(ACPI_SIG_LPIT, 0, (struct acpi_table_header **)&lpit);

	if (ACPI_FAILURE(status))
		return;

	lpit_begin = (u64)lpit + sizeof(*lpit);
	lpit_process(lpit_begin, lpit_begin + lpit->header.length);
=======
	struct acpi_table_lpit *lpit;

	status = acpi_get_table(ACPI_SIG_LPIT, 0, (struct acpi_table_header **)&lpit);
	if (ACPI_FAILURE(status))
		return;

	lpit_process((u64)lpit + sizeof(*lpit),
		     (u64)lpit + lpit->header.length);

	acpi_put_table((struct acpi_table_header *)lpit);
>>>>>>> upstream/android-13
}
