<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * cros_ec_dev - expose the Chrome OS Embedded Controller to user-space
 *
 * Copyright (C) 2014 Google, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/fs.h>
#include <linux/mfd/core.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "cros_ec_dev.h"

#define DRV_NAME "cros-ec-dev"

/* Device variables */
#define CROS_MAX_DEV 128
static int ec_major;

static const struct attribute_group *cros_ec_groups[] = {
	&cros_ec_attr_group,
	&cros_ec_lightbar_attr_group,
	&cros_ec_vbc_attr_group,
	NULL,
};

static struct class cros_class = {
	.owner          = THIS_MODULE,
	.name           = "chromeos",
	.dev_groups     = cros_ec_groups,
};

/* Basic communication */
static int ec_get_version(struct cros_ec_dev *ec, char *str, int maxlen)
{
	struct ec_response_get_version *resp;
	static const char * const current_image_name[] = {
		"unknown", "read-only", "read-write", "invalid",
	};
	struct cros_ec_command *msg;
	int ret;

	msg = kmalloc(sizeof(*msg) + sizeof(*resp), GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

	msg->version = 0;
	msg->command = EC_CMD_GET_VERSION + ec->cmd_offset;
	msg->insize = sizeof(*resp);
	msg->outsize = 0;

	ret = cros_ec_cmd_xfer(ec->ec_dev, msg);
	if (ret < 0)
		goto exit;

	if (msg->result != EC_RES_SUCCESS) {
		snprintf(str, maxlen,
			 "%s\nUnknown EC version: EC returned %d\n",
			 CROS_EC_DEV_VERSION, msg->result);
		ret = -EINVAL;
		goto exit;
	}

	resp = (struct ec_response_get_version *)msg->data;
	if (resp->current_image >= ARRAY_SIZE(current_image_name))
		resp->current_image = 3; /* invalid */

	snprintf(str, maxlen, "%s\n%s\n%s\n%s\n", CROS_EC_DEV_VERSION,
		 resp->version_string_ro, resp->version_string_rw,
		 current_image_name[resp->current_image]);

	ret = 0;
exit:
	kfree(msg);
	return ret;
}

static int cros_ec_check_features(struct cros_ec_dev *ec, int feature)
{
	struct cros_ec_command *msg;
	int ret;

	if (ec->features[0] == -1U && ec->features[1] == -1U) {
		/* features bitmap not read yet */

		msg = kmalloc(sizeof(*msg) + sizeof(ec->features), GFP_KERNEL);
		if (!msg)
			return -ENOMEM;

		msg->version = 0;
		msg->command = EC_CMD_GET_FEATURES + ec->cmd_offset;
		msg->insize = sizeof(ec->features);
		msg->outsize = 0;

		ret = cros_ec_cmd_xfer(ec->ec_dev, msg);
		if (ret < 0 || msg->result != EC_RES_SUCCESS) {
			dev_warn(ec->dev, "cannot get EC features: %d/%d\n",
				 ret, msg->result);
			memset(ec->features, 0, sizeof(ec->features));
		} else {
			memcpy(ec->features, msg->data, sizeof(ec->features));
		}

		dev_dbg(ec->dev, "EC features %08x %08x\n",
			ec->features[0], ec->features[1]);

		kfree(msg);
	}

	return ec->features[feature / 32] & EC_FEATURE_MASK_0(feature);
}

/* Device file ops */
static int ec_device_open(struct inode *inode, struct file *filp)
{
	struct cros_ec_dev *ec = container_of(inode->i_cdev,
					      struct cros_ec_dev, cdev);
	filp->private_data = ec;
	nonseekable_open(inode, filp);
	return 0;
}

static int ec_device_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t ec_device_read(struct file *filp, char __user *buffer,
			      size_t length, loff_t *offset)
{
	struct cros_ec_dev *ec = filp->private_data;
	char msg[sizeof(struct ec_response_get_version) +
		 sizeof(CROS_EC_DEV_VERSION)];
	size_t count;
	int ret;

	if (*offset != 0)
		return 0;

	ret = ec_get_version(ec, msg, sizeof(msg));
	if (ret)
		return ret;

	count = min(length, strlen(msg));

	if (copy_to_user(buffer, msg, count))
		return -EFAULT;

	*offset = count;
	return count;
}

/* Ioctls */
static long ec_device_ioctl_xcmd(struct cros_ec_dev *ec, void __user *arg)
{
	long ret;
	struct cros_ec_command u_cmd;
	struct cros_ec_command *s_cmd;

	if (copy_from_user(&u_cmd, arg, sizeof(u_cmd)))
		return -EFAULT;

	if ((u_cmd.outsize > EC_MAX_MSG_BYTES) ||
	    (u_cmd.insize > EC_MAX_MSG_BYTES))
		return -EINVAL;

	s_cmd = kmalloc(sizeof(*s_cmd) + max(u_cmd.outsize, u_cmd.insize),
			GFP_KERNEL);
	if (!s_cmd)
		return -ENOMEM;

	if (copy_from_user(s_cmd, arg, sizeof(*s_cmd) + u_cmd.outsize)) {
		ret = -EFAULT;
		goto exit;
	}

	if (u_cmd.outsize != s_cmd->outsize ||
	    u_cmd.insize != s_cmd->insize) {
		ret = -EINVAL;
		goto exit;
	}

	s_cmd->command += ec->cmd_offset;
	ret = cros_ec_cmd_xfer(ec->ec_dev, s_cmd);
	/* Only copy data to userland if data was received. */
	if (ret < 0)
		goto exit;

	if (copy_to_user(arg, s_cmd, sizeof(*s_cmd) + s_cmd->insize))
		ret = -EFAULT;
exit:
	kfree(s_cmd);
	return ret;
}

static long ec_device_ioctl_readmem(struct cros_ec_dev *ec, void __user *arg)
{
	struct cros_ec_device *ec_dev = ec->ec_dev;
	struct cros_ec_readmem s_mem = { };
	long num;

	/* Not every platform supports direct reads */
	if (!ec_dev->cmd_readmem)
		return -ENOTTY;

	if (copy_from_user(&s_mem, arg, sizeof(s_mem)))
		return -EFAULT;

	num = ec_dev->cmd_readmem(ec_dev, s_mem.offset, s_mem.bytes,
				  s_mem.buffer);
	if (num <= 0)
		return num;

	if (copy_to_user((void __user *)arg, &s_mem, sizeof(s_mem)))
		return -EFAULT;

	return 0;
}

static long ec_device_ioctl(struct file *filp, unsigned int cmd,
			    unsigned long arg)
{
	struct cros_ec_dev *ec = filp->private_data;

	if (_IOC_TYPE(cmd) != CROS_EC_DEV_IOC)
		return -ENOTTY;

	switch (cmd) {
	case CROS_EC_DEV_IOCXCMD:
		return ec_device_ioctl_xcmd(ec, (void __user *)arg);
	case CROS_EC_DEV_IOCRDMEM:
		return ec_device_ioctl_readmem(ec, (void __user *)arg);
	}

	return -ENOTTY;
}

/* Module initialization */
static const struct file_operations fops = {
	.open = ec_device_open,
	.release = ec_device_release,
	.read = ec_device_read,
	.unlocked_ioctl = ec_device_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = ec_device_ioctl,
#endif
=======
 */

#include <linux/dmi.h>
#include <linux/kconfig.h>
#include <linux/mfd/core.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/platform_data/cros_ec_chardev.h>
#include <linux/platform_data/cros_ec_commands.h>
#include <linux/platform_data/cros_ec_proto.h>
#include <linux/slab.h>

#define DRV_NAME "cros-ec-dev"

static struct class cros_class = {
	.owner          = THIS_MODULE,
	.name           = "chromeos",
};

/**
 * struct cros_feature_to_name - CrOS feature id to name/short description.
 * @id: The feature identifier.
 * @name: Device name associated with the feature id.
 * @desc: Short name that will be displayed.
 */
struct cros_feature_to_name {
	unsigned int id;
	const char *name;
	const char *desc;
};

/**
 * struct cros_feature_to_cells - CrOS feature id to mfd cells association.
 * @id: The feature identifier.
 * @mfd_cells: Pointer to the array of mfd cells that needs to be added.
 * @num_cells: Number of mfd cells into the array.
 */
struct cros_feature_to_cells {
	unsigned int id;
	const struct mfd_cell *mfd_cells;
	unsigned int num_cells;
};

static const struct cros_feature_to_name cros_mcu_devices[] = {
	{
		.id	= EC_FEATURE_FINGERPRINT,
		.name	= CROS_EC_DEV_FP_NAME,
		.desc	= "Fingerprint",
	},
	{
		.id	= EC_FEATURE_ISH,
		.name	= CROS_EC_DEV_ISH_NAME,
		.desc	= "Integrated Sensor Hub",
	},
	{
		.id	= EC_FEATURE_SCP,
		.name	= CROS_EC_DEV_SCP_NAME,
		.desc	= "System Control Processor",
	},
	{
		.id	= EC_FEATURE_TOUCHPAD,
		.name	= CROS_EC_DEV_TP_NAME,
		.desc	= "Touchpad",
	},
};

static const struct mfd_cell cros_ec_cec_cells[] = {
	{ .name = "cros-ec-cec", },
};

static const struct mfd_cell cros_ec_rtc_cells[] = {
	{ .name = "cros-ec-rtc", },
};

static const struct mfd_cell cros_ec_sensorhub_cells[] = {
	{ .name = "cros-ec-sensorhub", },
};

static const struct mfd_cell cros_usbpd_charger_cells[] = {
	{ .name = "cros-usbpd-charger", },
	{ .name = "cros-usbpd-logger", },
};

static const struct mfd_cell cros_usbpd_notify_cells[] = {
	{ .name = "cros-usbpd-notify", },
};

static const struct cros_feature_to_cells cros_subdevices[] = {
	{
		.id		= EC_FEATURE_CEC,
		.mfd_cells	= cros_ec_cec_cells,
		.num_cells	= ARRAY_SIZE(cros_ec_cec_cells),
	},
	{
		.id		= EC_FEATURE_RTC,
		.mfd_cells	= cros_ec_rtc_cells,
		.num_cells	= ARRAY_SIZE(cros_ec_rtc_cells),
	},
	{
		.id		= EC_FEATURE_USB_PD,
		.mfd_cells	= cros_usbpd_charger_cells,
		.num_cells	= ARRAY_SIZE(cros_usbpd_charger_cells),
	},
};

static const struct mfd_cell cros_ec_platform_cells[] = {
	{ .name = "cros-ec-chardev", },
	{ .name = "cros-ec-debugfs", },
	{ .name = "cros-ec-sysfs", },
	{ .name = "cros-ec-pchg", },
};

static const struct mfd_cell cros_ec_lightbar_cells[] = {
	{ .name = "cros-ec-lightbar", }
};

static const struct mfd_cell cros_ec_vbc_cells[] = {
	{ .name = "cros-ec-vbc", }
>>>>>>> upstream/android-13
};

static void cros_ec_class_release(struct device *dev)
{
	kfree(to_cros_ec_dev(dev));
}

<<<<<<< HEAD
static void cros_ec_sensors_register(struct cros_ec_dev *ec)
{
	/*
	 * Issue a command to get the number of sensor reported.
	 * Build an array of sensors driver and register them all.
	 */
	int ret, i, id, sensor_num;
	struct mfd_cell *sensor_cells;
	struct cros_ec_sensor_platform *sensor_platforms;
	int sensor_type[MOTIONSENSE_TYPE_MAX];
	struct ec_params_motion_sense *params;
	struct ec_response_motion_sense *resp;
	struct cros_ec_command *msg;

	msg = kzalloc(sizeof(struct cros_ec_command) +
		      max(sizeof(*params), sizeof(*resp)), GFP_KERNEL);
	if (msg == NULL)
		return;

	msg->version = 2;
	msg->command = EC_CMD_MOTION_SENSE_CMD + ec->cmd_offset;
	msg->outsize = sizeof(*params);
	msg->insize = sizeof(*resp);

	params = (struct ec_params_motion_sense *)msg->data;
	params->cmd = MOTIONSENSE_CMD_DUMP;

	ret = cros_ec_cmd_xfer(ec->ec_dev, msg);
	if (ret < 0 || msg->result != EC_RES_SUCCESS) {
		dev_warn(ec->dev, "cannot get EC sensor information: %d/%d\n",
			 ret, msg->result);
		goto error;
	}

	resp = (struct ec_response_motion_sense *)msg->data;
	sensor_num = resp->dump.sensor_count;
	/* Allocate 1 extra sensors in FIFO are needed */
	sensor_cells = kcalloc(sensor_num + 1, sizeof(struct mfd_cell),
			       GFP_KERNEL);
	if (sensor_cells == NULL)
		goto error;

	sensor_platforms = kcalloc(sensor_num + 1,
				   sizeof(struct cros_ec_sensor_platform),
				   GFP_KERNEL);
	if (sensor_platforms == NULL)
		goto error_platforms;

	memset(sensor_type, 0, sizeof(sensor_type));
	id = 0;
	for (i = 0; i < sensor_num; i++) {
		params->cmd = MOTIONSENSE_CMD_INFO;
		params->info.sensor_num = i;
		ret = cros_ec_cmd_xfer(ec->ec_dev, msg);
		if (ret < 0 || msg->result != EC_RES_SUCCESS) {
			dev_warn(ec->dev, "no info for EC sensor %d : %d/%d\n",
				 i, ret, msg->result);
			continue;
		}
		switch (resp->info.type) {
		case MOTIONSENSE_TYPE_ACCEL:
			sensor_cells[id].name = "cros-ec-accel";
			break;
		case MOTIONSENSE_TYPE_BARO:
			sensor_cells[id].name = "cros-ec-baro";
			break;
		case MOTIONSENSE_TYPE_GYRO:
			sensor_cells[id].name = "cros-ec-gyro";
			break;
		case MOTIONSENSE_TYPE_MAG:
			sensor_cells[id].name = "cros-ec-mag";
			break;
		case MOTIONSENSE_TYPE_PROX:
			sensor_cells[id].name = "cros-ec-prox";
			break;
		case MOTIONSENSE_TYPE_LIGHT:
			sensor_cells[id].name = "cros-ec-light";
			break;
		case MOTIONSENSE_TYPE_ACTIVITY:
			sensor_cells[id].name = "cros-ec-activity";
			break;
		default:
			dev_warn(ec->dev, "unknown type %d\n", resp->info.type);
			continue;
		}
		sensor_platforms[id].sensor_num = i;
		sensor_cells[id].id = sensor_type[resp->info.type];
		sensor_cells[id].platform_data = &sensor_platforms[id];
		sensor_cells[id].pdata_size =
			sizeof(struct cros_ec_sensor_platform);

		sensor_type[resp->info.type]++;
		id++;
	}

	if (sensor_type[MOTIONSENSE_TYPE_ACCEL] >= 2)
		ec->has_kb_wake_angle = true;

	if (cros_ec_check_features(ec, EC_FEATURE_MOTION_SENSE_FIFO)) {
		sensor_cells[id].name = "cros-ec-ring";
		id++;
	}

	ret = mfd_add_devices(ec->dev, 0, sensor_cells, id,
			      NULL, 0, NULL);
	if (ret)
		dev_err(ec->dev, "failed to add EC sensors\n");

	kfree(sensor_platforms);
error_platforms:
	kfree(sensor_cells);
error:
	kfree(msg);
}

static const struct mfd_cell cros_ec_cec_cells[] = {
	{ .name = "cros-ec-cec" }
};

static const struct mfd_cell cros_ec_rtc_cells[] = {
	{ .name = "cros-ec-rtc" }
};

static const struct mfd_cell cros_usbpd_charger_cells[] = {
	{ .name = "cros-usbpd-charger" }
};

static int ec_device_probe(struct platform_device *pdev)
{
	int retval = -ENOMEM;
	struct device *dev = &pdev->dev;
	struct cros_ec_platform *ec_platform = dev_get_platdata(dev);
	struct cros_ec_dev *ec = kzalloc(sizeof(*ec), GFP_KERNEL);
=======
static int ec_device_probe(struct platform_device *pdev)
{
	int retval = -ENOMEM;
	struct device_node *node;
	struct device *dev = &pdev->dev;
	struct cros_ec_platform *ec_platform = dev_get_platdata(dev);
	struct cros_ec_dev *ec = kzalloc(sizeof(*ec), GFP_KERNEL);
	int i;
>>>>>>> upstream/android-13

	if (!ec)
		return retval;

	dev_set_drvdata(dev, ec);
	ec->ec_dev = dev_get_drvdata(dev->parent);
	ec->dev = dev;
	ec->cmd_offset = ec_platform->cmd_offset;
	ec->features[0] = -1U; /* Not cached yet */
	ec->features[1] = -1U; /* Not cached yet */
	device_initialize(&ec->class_dev);
<<<<<<< HEAD
	cdev_init(&ec->cdev, &fops);

	/*
	 * Add the class device
	 * Link to the character device for creating the /dev entry
	 * in devtmpfs.
	 */
	ec->class_dev.devt = MKDEV(ec_major, pdev->id);
=======

	for (i = 0; i < ARRAY_SIZE(cros_mcu_devices); i++) {
		/*
		 * Check whether this is actually a dedicated MCU rather
		 * than an standard EC.
		 */
		if (cros_ec_check_features(ec, cros_mcu_devices[i].id)) {
			dev_info(dev, "CrOS %s MCU detected\n",
				 cros_mcu_devices[i].desc);
			/*
			 * Help userspace differentiating ECs from other MCU,
			 * regardless of the probing order.
			 */
			ec_platform->ec_name = cros_mcu_devices[i].name;
			break;
		}
	}

	/*
	 * Add the class device
	 */
>>>>>>> upstream/android-13
	ec->class_dev.class = &cros_class;
	ec->class_dev.parent = dev;
	ec->class_dev.release = cros_ec_class_release;

	retval = dev_set_name(&ec->class_dev, "%s", ec_platform->ec_name);
	if (retval) {
		dev_err(dev, "dev_set_name failed => %d\n", retval);
		goto failed;
	}

<<<<<<< HEAD
	/* check whether this EC is a sensor hub. */
	if (cros_ec_check_features(ec, EC_FEATURE_MOTION_SENSE))
		cros_ec_sensors_register(ec);

	/* Check whether this EC instance has CEC host command support */
	if (cros_ec_check_features(ec, EC_FEATURE_CEC)) {
		retval = mfd_add_devices(ec->dev, PLATFORM_DEVID_AUTO,
					 cros_ec_cec_cells,
					 ARRAY_SIZE(cros_ec_cec_cells),
					 NULL, 0, NULL);
		if (retval)
			dev_err(ec->dev,
				"failed to add cros-ec-cec device: %d\n",
				retval);
	}

	/* Check whether this EC instance has RTC host command support */
	if (cros_ec_check_features(ec, EC_FEATURE_RTC)) {
		retval = mfd_add_devices(ec->dev, PLATFORM_DEVID_AUTO,
					 cros_ec_rtc_cells,
					 ARRAY_SIZE(cros_ec_rtc_cells),
					 NULL, 0, NULL);
		if (retval)
			dev_err(ec->dev,
				"failed to add cros-ec-rtc device: %d\n",
				retval);
	}

	/* Check whether this EC instance has the PD charge manager */
	if (cros_ec_check_features(ec, EC_FEATURE_USB_PD)) {
		retval = mfd_add_devices(ec->dev, PLATFORM_DEVID_AUTO,
					 cros_usbpd_charger_cells,
					 ARRAY_SIZE(cros_usbpd_charger_cells),
					 NULL, 0, NULL);
		if (retval)
			dev_err(ec->dev,
				"failed to add cros-usbpd-charger device: %d\n",
				retval);
	}

	/* Take control of the lightbar from the EC. */
	lb_manual_suspend_ctrl(ec, 1);

	/* We can now add the sysfs class, we know which parameter to show */
	retval = cdev_device_add(&ec->cdev, &ec->class_dev);
	if (retval) {
		dev_err(dev, "cdev_device_add failed => %d\n", retval);
		goto failed;
	}

	if (cros_ec_debugfs_init(ec))
		dev_warn(dev, "failed to create debugfs directory\n");
=======
	retval = device_add(&ec->class_dev);
	if (retval)
		goto failed;

	/* check whether this EC is a sensor hub. */
	if (cros_ec_get_sensor_count(ec) > 0) {
		retval = mfd_add_hotplug_devices(ec->dev,
				cros_ec_sensorhub_cells,
				ARRAY_SIZE(cros_ec_sensorhub_cells));
		if (retval)
			dev_err(ec->dev, "failed to add %s subdevice: %d\n",
				cros_ec_sensorhub_cells->name, retval);
	}

	/*
	 * The following subdevices can be detected by sending the
	 * EC_FEATURE_GET_CMD Embedded Controller device.
	 */
	for (i = 0; i < ARRAY_SIZE(cros_subdevices); i++) {
		if (cros_ec_check_features(ec, cros_subdevices[i].id)) {
			retval = mfd_add_hotplug_devices(ec->dev,
						cros_subdevices[i].mfd_cells,
						cros_subdevices[i].num_cells);
			if (retval)
				dev_err(ec->dev,
					"failed to add %s subdevice: %d\n",
					cros_subdevices[i].mfd_cells->name,
					retval);
		}
	}

	/*
	 * Lightbar is a special case. Newer devices support autodetection,
	 * but older ones do not.
	 */
	if (cros_ec_check_features(ec, EC_FEATURE_LIGHTBAR) ||
	    dmi_match(DMI_PRODUCT_NAME, "Link")) {
		retval = mfd_add_hotplug_devices(ec->dev,
					cros_ec_lightbar_cells,
					ARRAY_SIZE(cros_ec_lightbar_cells));
		if (retval)
			dev_warn(ec->dev, "failed to add lightbar: %d\n",
				 retval);
	}

	/*
	 * The PD notifier driver cell is separate since it only needs to be
	 * explicitly added on platforms that don't have the PD notifier ACPI
	 * device entry defined.
	 */
	if (IS_ENABLED(CONFIG_OF) && ec->ec_dev->dev->of_node) {
		if (cros_ec_check_features(ec, EC_FEATURE_USB_PD)) {
			retval = mfd_add_hotplug_devices(ec->dev,
					cros_usbpd_notify_cells,
					ARRAY_SIZE(cros_usbpd_notify_cells));
			if (retval)
				dev_err(ec->dev,
					"failed to add PD notify devices: %d\n",
					retval);
		}
	}

	/*
	 * The following subdevices cannot be detected by sending the
	 * EC_FEATURE_GET_CMD to the Embedded Controller device.
	 */
	retval = mfd_add_hotplug_devices(ec->dev, cros_ec_platform_cells,
					 ARRAY_SIZE(cros_ec_platform_cells));
	if (retval)
		dev_warn(ec->dev,
			 "failed to add cros-ec platform devices: %d\n",
			 retval);

	/* Check whether this EC instance has a VBC NVRAM */
	node = ec->ec_dev->dev->of_node;
	if (of_property_read_bool(node, "google,has-vbc-nvram")) {
		retval = mfd_add_hotplug_devices(ec->dev, cros_ec_vbc_cells,
						ARRAY_SIZE(cros_ec_vbc_cells));
		if (retval)
			dev_warn(ec->dev, "failed to add VBC devices: %d\n",
				 retval);
	}
>>>>>>> upstream/android-13

	return 0;

failed:
	put_device(&ec->class_dev);
	return retval;
}

static int ec_device_remove(struct platform_device *pdev)
{
	struct cros_ec_dev *ec = dev_get_drvdata(&pdev->dev);

<<<<<<< HEAD
	/* Let the EC take over the lightbar again. */
	lb_manual_suspend_ctrl(ec, 0);

	cros_ec_debugfs_remove(ec);

	mfd_remove_devices(ec->dev);
	cdev_del(&ec->cdev);
=======
	mfd_remove_devices(ec->dev);
>>>>>>> upstream/android-13
	device_unregister(&ec->class_dev);
	return 0;
}

<<<<<<< HEAD
static void ec_device_shutdown(struct platform_device *pdev)
{
	struct cros_ec_dev *ec = dev_get_drvdata(&pdev->dev);

	/* Be sure to clear up debugfs delayed works */
	cros_ec_debugfs_remove(ec);
}

=======
>>>>>>> upstream/android-13
static const struct platform_device_id cros_ec_id[] = {
	{ DRV_NAME, 0 },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(platform, cros_ec_id);

<<<<<<< HEAD
static __maybe_unused int ec_device_suspend(struct device *dev)
{
	struct cros_ec_dev *ec = dev_get_drvdata(dev);

	cros_ec_debugfs_suspend(ec);

	lb_suspend(ec);

	return 0;
}

static __maybe_unused int ec_device_resume(struct device *dev)
{
	struct cros_ec_dev *ec = dev_get_drvdata(dev);

	cros_ec_debugfs_resume(ec);

	lb_resume(ec);

	return 0;
}

static const struct dev_pm_ops cros_ec_dev_pm_ops = {
#ifdef CONFIG_PM_SLEEP
	.suspend = ec_device_suspend,
	.resume = ec_device_resume,
#endif
};

static struct platform_driver cros_ec_dev_driver = {
	.driver = {
		.name = DRV_NAME,
		.pm = &cros_ec_dev_pm_ops,
	},
	.probe = ec_device_probe,
	.remove = ec_device_remove,
	.shutdown = ec_device_shutdown,
=======
static struct platform_driver cros_ec_dev_driver = {
	.driver = {
		.name = DRV_NAME,
	},
	.id_table = cros_ec_id,
	.probe = ec_device_probe,
	.remove = ec_device_remove,
>>>>>>> upstream/android-13
};

static int __init cros_ec_dev_init(void)
{
	int ret;
<<<<<<< HEAD
	dev_t dev = 0;
=======
>>>>>>> upstream/android-13

	ret  = class_register(&cros_class);
	if (ret) {
		pr_err(CROS_EC_DEV_NAME ": failed to register device class\n");
		return ret;
	}

<<<<<<< HEAD
	/* Get a range of minor numbers (starting with 0) to work with */
	ret = alloc_chrdev_region(&dev, 0, CROS_MAX_DEV, CROS_EC_DEV_NAME);
	if (ret < 0) {
		pr_err(CROS_EC_DEV_NAME ": alloc_chrdev_region() failed\n");
		goto failed_chrdevreg;
	}
	ec_major = MAJOR(dev);

=======
>>>>>>> upstream/android-13
	/* Register the driver */
	ret = platform_driver_register(&cros_ec_dev_driver);
	if (ret < 0) {
		pr_warn(CROS_EC_DEV_NAME ": can't register driver: %d\n", ret);
		goto failed_devreg;
	}
	return 0;

failed_devreg:
<<<<<<< HEAD
	unregister_chrdev_region(MKDEV(ec_major, 0), CROS_MAX_DEV);
failed_chrdevreg:
=======
>>>>>>> upstream/android-13
	class_unregister(&cros_class);
	return ret;
}

static void __exit cros_ec_dev_exit(void)
{
	platform_driver_unregister(&cros_ec_dev_driver);
<<<<<<< HEAD
	unregister_chrdev(ec_major, CROS_EC_DEV_NAME);
=======
>>>>>>> upstream/android-13
	class_unregister(&cros_class);
}

module_init(cros_ec_dev_init);
module_exit(cros_ec_dev_exit);

MODULE_ALIAS("platform:" DRV_NAME);
MODULE_AUTHOR("Bill Richardson <wfrichar@chromium.org>");
MODULE_DESCRIPTION("Userspace interface to the Chrome OS Embedded Controller");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");
