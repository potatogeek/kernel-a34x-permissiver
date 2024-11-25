<<<<<<< HEAD
/*
 * dptf_power:  DPTF platform power driver
 * Copyright (c) 2016, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * dptf_power:  DPTF platform power driver
 * Copyright (c) 2016, Intel Corporation.
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/acpi.h>
#include <linux/platform_device.h>

/*
<<<<<<< HEAD
 * Presentation of attributes which are defined for INT3407. They are:
=======
 * Presentation of attributes which are defined for INT3407 and INT3532.
 * They are:
>>>>>>> upstream/android-13
 * PMAX : Maximum platform powe
 * PSRC : Platform power source
 * ARTG : Adapter rating
 * CTYP : Charger type
 * PBSS : Battery steady power
<<<<<<< HEAD
=======
 * PROP : Rest of worst case platform Power
 * PBSS : Power Battery Steady State
 * PBSS : Power Battery Steady State
 * RBHF : High Frequency Impedance
 * VBNL : Instantaneous No-Load Voltage
 * CMPP : Current Discharge Capability
>>>>>>> upstream/android-13
 */
#define DPTF_POWER_SHOW(name, object) \
static ssize_t name##_show(struct device *dev,\
			   struct device_attribute *attr,\
			   char *buf)\
{\
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);\
	struct acpi_device *acpi_dev = platform_get_drvdata(pdev);\
=======
	struct acpi_device *acpi_dev = dev_get_drvdata(dev);\
>>>>>>> upstream/android-13
	unsigned long long val;\
	acpi_status status;\
\
	status = acpi_evaluate_integer(acpi_dev->handle, #object,\
				       NULL, &val);\
	if (ACPI_SUCCESS(status))\
		return sprintf(buf, "%d\n", (int)val);\
	else \
		return -EINVAL;\
}

DPTF_POWER_SHOW(max_platform_power_mw, PMAX)
DPTF_POWER_SHOW(platform_power_source, PSRC)
DPTF_POWER_SHOW(adapter_rating_mw, ARTG)
DPTF_POWER_SHOW(battery_steady_power_mw, PBSS)
DPTF_POWER_SHOW(charger_type, CTYP)
<<<<<<< HEAD
=======
DPTF_POWER_SHOW(rest_of_platform_power_mw, PROP)
DPTF_POWER_SHOW(max_steady_state_power_mw, PBSS)
DPTF_POWER_SHOW(high_freq_impedance_mohm, RBHF)
DPTF_POWER_SHOW(no_load_voltage_mv, VBNL)
DPTF_POWER_SHOW(current_discharge_capbility_ma, CMPP);
>>>>>>> upstream/android-13

static DEVICE_ATTR_RO(max_platform_power_mw);
static DEVICE_ATTR_RO(platform_power_source);
static DEVICE_ATTR_RO(adapter_rating_mw);
static DEVICE_ATTR_RO(battery_steady_power_mw);
static DEVICE_ATTR_RO(charger_type);
<<<<<<< HEAD
=======
static DEVICE_ATTR_RO(rest_of_platform_power_mw);
static DEVICE_ATTR_RO(max_steady_state_power_mw);
static DEVICE_ATTR_RO(high_freq_impedance_mohm);
static DEVICE_ATTR_RO(no_load_voltage_mv);
static DEVICE_ATTR_RO(current_discharge_capbility_ma);

static ssize_t prochot_confirm_store(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	struct acpi_device *acpi_dev = dev_get_drvdata(dev);
	acpi_status status;
	int seq_no;

	if (kstrtouint(buf, 0, &seq_no) < 0)
		return -EINVAL;

	status = acpi_execute_simple_method(acpi_dev->handle, "PBOK", seq_no);
	if (ACPI_SUCCESS(status))
		return count;

	return -EINVAL;
}

static DEVICE_ATTR_WO(prochot_confirm);
>>>>>>> upstream/android-13

static struct attribute *dptf_power_attrs[] = {
	&dev_attr_max_platform_power_mw.attr,
	&dev_attr_platform_power_source.attr,
	&dev_attr_adapter_rating_mw.attr,
	&dev_attr_battery_steady_power_mw.attr,
	&dev_attr_charger_type.attr,
<<<<<<< HEAD
=======
	&dev_attr_rest_of_platform_power_mw.attr,
	&dev_attr_prochot_confirm.attr,
>>>>>>> upstream/android-13
	NULL
};

static const struct attribute_group dptf_power_attribute_group = {
	.attrs = dptf_power_attrs,
	.name = "dptf_power"
};

<<<<<<< HEAD
static int dptf_power_add(struct platform_device *pdev)
{
	struct acpi_device *acpi_dev;
	acpi_status status;
=======
static struct attribute *dptf_battery_attrs[] = {
	&dev_attr_max_platform_power_mw.attr,
	&dev_attr_max_steady_state_power_mw.attr,
	&dev_attr_high_freq_impedance_mohm.attr,
	&dev_attr_no_load_voltage_mv.attr,
	&dev_attr_current_discharge_capbility_ma.attr,
	NULL
};

static const struct attribute_group dptf_battery_attribute_group = {
	.attrs = dptf_battery_attrs,
	.name = "dptf_battery"
};

#define MAX_POWER_CHANGED		0x80
#define POWER_STATE_CHANGED		0x81
#define STEADY_STATE_POWER_CHANGED	0x83
#define POWER_PROP_CHANGE_EVENT	0x84
#define IMPEDANCED_CHNGED		0x85
#define VOLTAGE_CURRENT_CHANGED	0x86

static long long dptf_participant_type(acpi_handle handle)
{
	unsigned long long ptype;
	acpi_status status;

	status = acpi_evaluate_integer(handle, "PTYP", NULL, &ptype);
	if (ACPI_FAILURE(status))
		return -ENODEV;

	return ptype;
}

static void dptf_power_notify(acpi_handle handle, u32 event, void *data)
{
	struct platform_device *pdev = data;
	char *attr;

	switch (event) {
	case POWER_STATE_CHANGED:
		attr = "platform_power_source";
		break;
	case POWER_PROP_CHANGE_EVENT:
		attr = "rest_of_platform_power_mw";
		break;
	case MAX_POWER_CHANGED:
		attr = "max_platform_power_mw";
		break;
	case STEADY_STATE_POWER_CHANGED:
		attr = "max_steady_state_power_mw";
		break;
	case VOLTAGE_CURRENT_CHANGED:
		attr = "no_load_voltage_mv";
		break;
	default:
		dev_err(&pdev->dev, "Unsupported event [0x%x]\n", event);
		return;
	}

	/*
	 * Notify that an attribute is changed, so that user space can read
	 * again.
	 */
	if (dptf_participant_type(handle) == 0x0CULL)
		sysfs_notify(&pdev->dev.kobj, "dptf_battery", attr);
	else
		sysfs_notify(&pdev->dev.kobj, "dptf_power", attr);
}

static int dptf_power_add(struct platform_device *pdev)
{
	const struct attribute_group *attr_group;
	struct acpi_device *acpi_dev;
>>>>>>> upstream/android-13
	unsigned long long ptype;
	int result;

	acpi_dev = ACPI_COMPANION(&(pdev->dev));
	if (!acpi_dev)
		return -ENODEV;

<<<<<<< HEAD
	status = acpi_evaluate_integer(acpi_dev->handle, "PTYP", NULL, &ptype);
	if (ACPI_FAILURE(status))
		return -ENODEV;

	if (ptype != 0x11)
		return -ENODEV;

	result = sysfs_create_group(&pdev->dev.kobj,
				    &dptf_power_attribute_group);
	if (result)
		return result;

=======
	ptype = dptf_participant_type(acpi_dev->handle);
	if (ptype == 0x11)
		attr_group = &dptf_power_attribute_group;
	else if (ptype == 0x0C)
		attr_group = &dptf_battery_attribute_group;
	else
		return -ENODEV;

	result = acpi_install_notify_handler(acpi_dev->handle,
					     ACPI_DEVICE_NOTIFY,
					     dptf_power_notify,
					     (void *)pdev);
	if (result)
		return result;

	result = sysfs_create_group(&pdev->dev.kobj,
				    attr_group);
	if (result) {
		acpi_remove_notify_handler(acpi_dev->handle,
					   ACPI_DEVICE_NOTIFY,
					   dptf_power_notify);
		return result;
	}

>>>>>>> upstream/android-13
	platform_set_drvdata(pdev, acpi_dev);

	return 0;
}

static int dptf_power_remove(struct platform_device *pdev)
{
<<<<<<< HEAD

	sysfs_remove_group(&pdev->dev.kobj, &dptf_power_attribute_group);
=======
	struct acpi_device *acpi_dev = platform_get_drvdata(pdev);

	acpi_remove_notify_handler(acpi_dev->handle,
				   ACPI_DEVICE_NOTIFY,
				   dptf_power_notify);

	if (dptf_participant_type(acpi_dev->handle) == 0x0CULL)
		sysfs_remove_group(&pdev->dev.kobj, &dptf_battery_attribute_group);
	else
		sysfs_remove_group(&pdev->dev.kobj, &dptf_power_attribute_group);
>>>>>>> upstream/android-13

	return 0;
}

static const struct acpi_device_id int3407_device_ids[] = {
	{"INT3407", 0},
<<<<<<< HEAD
=======
	{"INT3532", 0},
	{"INTC1047", 0},
	{"INTC1050", 0},
	{"INTC1060", 0},
	{"INTC1061", 0},
>>>>>>> upstream/android-13
	{"", 0},
};
MODULE_DEVICE_TABLE(acpi, int3407_device_ids);

static struct platform_driver dptf_power_driver = {
	.probe = dptf_power_add,
	.remove = dptf_power_remove,
	.driver = {
<<<<<<< HEAD
		.name = "DPTF Platform Power",
=======
		.name = "dptf_power",
>>>>>>> upstream/android-13
		.acpi_match_table = int3407_device_ids,
	},
};

module_platform_driver(dptf_power_driver);

MODULE_AUTHOR("Srinivas Pandruvada <srinivas.pandruvada@linux.intel.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("ACPI DPTF platform power driver");
