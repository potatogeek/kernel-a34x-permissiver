<<<<<<< HEAD
/*
 * cros_ec_vbc - Expose the vboot context nvram to userspace
 *
 * Copyright (C) 2015 Collabora Ltd.
 *
 * based on vendor driver,
 *
 * Copyright (C) 2012 The Chromium OS Authors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/mfd/cros_ec.h>
#include <linux/mfd/cros_ec_commands.h>
#include <linux/slab.h>

=======
// SPDX-License-Identifier: GPL-2.0+
// Expose the vboot context nvram to userspace
//
// Copyright (C) 2012 Google, Inc.
// Copyright (C) 2015 Collabora Ltd.

#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/platform_data/cros_ec_commands.h>
#include <linux/platform_data/cros_ec_proto.h>
#include <linux/slab.h>

#define DRV_NAME "cros-ec-vbc"

>>>>>>> upstream/android-13
static ssize_t vboot_context_read(struct file *filp, struct kobject *kobj,
				  struct bin_attribute *att, char *buf,
				  loff_t pos, size_t count)
{
<<<<<<< HEAD
	struct device *dev = container_of(kobj, struct device, kobj);
=======
	struct device *dev = kobj_to_dev(kobj);
>>>>>>> upstream/android-13
	struct cros_ec_dev *ec = to_cros_ec_dev(dev);
	struct cros_ec_device *ecdev = ec->ec_dev;
	struct ec_params_vbnvcontext *params;
	struct cros_ec_command *msg;
	int err;
	const size_t para_sz = sizeof(params->op);
	const size_t resp_sz = sizeof(struct ec_response_vbnvcontext);
	const size_t payload = max(para_sz, resp_sz);

	msg = kmalloc(sizeof(*msg) + payload, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

	/* NB: we only kmalloc()ated enough space for the op field */
	params = (struct ec_params_vbnvcontext *)msg->data;
	params->op = EC_VBNV_CONTEXT_OP_READ;

	msg->version = EC_VER_VBNV_CONTEXT;
	msg->command = EC_CMD_VBNV_CONTEXT;
	msg->outsize = para_sz;
	msg->insize = resp_sz;

<<<<<<< HEAD
	err = cros_ec_cmd_xfer(ecdev, msg);
=======
	err = cros_ec_cmd_xfer_status(ecdev, msg);
>>>>>>> upstream/android-13
	if (err < 0) {
		dev_err(dev, "Error sending read request: %d\n", err);
		kfree(msg);
		return err;
	}

	memcpy(buf, msg->data, resp_sz);

	kfree(msg);
	return resp_sz;
}

static ssize_t vboot_context_write(struct file *filp, struct kobject *kobj,
				   struct bin_attribute *attr, char *buf,
				   loff_t pos, size_t count)
{
<<<<<<< HEAD
	struct device *dev = container_of(kobj, struct device, kobj);
=======
	struct device *dev = kobj_to_dev(kobj);
>>>>>>> upstream/android-13
	struct cros_ec_dev *ec = to_cros_ec_dev(dev);
	struct cros_ec_device *ecdev = ec->ec_dev;
	struct ec_params_vbnvcontext *params;
	struct cros_ec_command *msg;
	int err;
	const size_t para_sz = sizeof(*params);
	const size_t data_sz = sizeof(params->block);

	/* Only write full values */
	if (count != data_sz)
		return -EINVAL;

	msg = kmalloc(sizeof(*msg) + para_sz, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

	params = (struct ec_params_vbnvcontext *)msg->data;
	params->op = EC_VBNV_CONTEXT_OP_WRITE;
	memcpy(params->block, buf, data_sz);

	msg->version = EC_VER_VBNV_CONTEXT;
	msg->command = EC_CMD_VBNV_CONTEXT;
	msg->outsize = para_sz;
	msg->insize = 0;

<<<<<<< HEAD
	err = cros_ec_cmd_xfer(ecdev, msg);
=======
	err = cros_ec_cmd_xfer_status(ecdev, msg);
>>>>>>> upstream/android-13
	if (err < 0) {
		dev_err(dev, "Error sending write request: %d\n", err);
		kfree(msg);
		return err;
	}

	kfree(msg);
	return data_sz;
}

<<<<<<< HEAD
static umode_t cros_ec_vbc_is_visible(struct kobject *kobj,
				      struct bin_attribute *a, int n)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct cros_ec_dev *ec = to_cros_ec_dev(dev);
	struct device_node *np = ec->ec_dev->dev->of_node;

	if (IS_ENABLED(CONFIG_OF) && np) {
		if (of_property_read_bool(np, "google,has-vbc-nvram"))
			return a->attr.mode;
	}

	return 0;
}

=======
>>>>>>> upstream/android-13
static BIN_ATTR_RW(vboot_context, 16);

static struct bin_attribute *cros_ec_vbc_bin_attrs[] = {
	&bin_attr_vboot_context,
	NULL
};

<<<<<<< HEAD
struct attribute_group cros_ec_vbc_attr_group = {
	.name = "vbc",
	.bin_attrs = cros_ec_vbc_bin_attrs,
	.is_bin_visible = cros_ec_vbc_is_visible,
};
EXPORT_SYMBOL(cros_ec_vbc_attr_group);
=======
static const struct attribute_group cros_ec_vbc_attr_group = {
	.name = "vbc",
	.bin_attrs = cros_ec_vbc_bin_attrs,
};

static int cros_ec_vbc_probe(struct platform_device *pd)
{
	struct cros_ec_dev *ec_dev = dev_get_drvdata(pd->dev.parent);
	struct device *dev = &pd->dev;
	int ret;

	ret = sysfs_create_group(&ec_dev->class_dev.kobj,
				 &cros_ec_vbc_attr_group);
	if (ret < 0)
		dev_err(dev, "failed to create %s attributes. err=%d\n",
			cros_ec_vbc_attr_group.name, ret);

	return ret;
}

static int cros_ec_vbc_remove(struct platform_device *pd)
{
	struct cros_ec_dev *ec_dev = dev_get_drvdata(pd->dev.parent);

	sysfs_remove_group(&ec_dev->class_dev.kobj,
			   &cros_ec_vbc_attr_group);

	return 0;
}

static struct platform_driver cros_ec_vbc_driver = {
	.driver = {
		.name = DRV_NAME,
	},
	.probe = cros_ec_vbc_probe,
	.remove = cros_ec_vbc_remove,
};

module_platform_driver(cros_ec_vbc_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Expose the vboot context nvram to userspace");
MODULE_ALIAS("platform:" DRV_NAME);
>>>>>>> upstream/android-13
