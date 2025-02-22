<<<<<<< HEAD
/* Hwmon client for industrial I/O devices
 *
 * Copyright (c) 2011 Jonathan Cameron
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Hwmon client for industrial I/O devices
 *
 * Copyright (c) 2011 Jonathan Cameron
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/hwmon.h>
#include <linux/of.h>
#include <linux/hwmon-sysfs.h>
#include <linux/iio/consumer.h>
#include <linux/iio/types.h>

/**
 * struct iio_hwmon_state - device instance state
 * @channels:		filled with array of channels from iio
 * @num_channels:	number of channels in channels (saves counting twice)
 * @attr_group:		the group of attributes
 * @groups:		null terminated array of attribute groups
 * @attrs:		null terminated array of attribute pointers.
 */
struct iio_hwmon_state {
	struct iio_channel *channels;
	int num_channels;
	struct attribute_group attr_group;
	const struct attribute_group *groups[2];
	struct attribute **attrs;
};

/*
 * Assumes that IIO and hwmon operate in the same base units.
 * This is supposed to be true, but needs verification for
 * new channel types.
 */
static ssize_t iio_hwmon_read_val(struct device *dev,
				  struct device_attribute *attr,
				  char *buf)
{
	int result;
	int ret;
	struct sensor_device_attribute *sattr = to_sensor_dev_attr(attr);
	struct iio_hwmon_state *state = dev_get_drvdata(dev);
<<<<<<< HEAD

	ret = iio_read_channel_processed(&state->channels[sattr->index],
					&result);
	if (ret < 0)
		return ret;

=======
	struct iio_channel *chan = &state->channels[sattr->index];
	enum iio_chan_type type;

	ret = iio_read_channel_processed(chan, &result);
	if (ret < 0)
		return ret;

	ret = iio_get_channel_type(chan, &type);
	if (ret < 0)
		return ret;

	if (type == IIO_POWER)
		result *= 1000; /* mili-Watts to micro-Watts conversion */

>>>>>>> upstream/android-13
	return sprintf(buf, "%d\n", result);
}

static int iio_hwmon_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct iio_hwmon_state *st;
	struct sensor_device_attribute *a;
	int ret, i;
<<<<<<< HEAD
	int in_i = 1, temp_i = 1, curr_i = 1, humidity_i = 1;
	enum iio_chan_type type;
	struct iio_channel *channels;
	const char *name = "iio_hwmon";
	struct device *hwmon_dev;
	char *sname;

	if (dev->of_node && dev->of_node->name)
		name = dev->of_node->name;

=======
	int in_i = 1, temp_i = 1, curr_i = 1, humidity_i = 1, power_i = 1;
	enum iio_chan_type type;
	struct iio_channel *channels;
	struct device *hwmon_dev;
	char *sname;

>>>>>>> upstream/android-13
	channels = devm_iio_channel_get_all(dev);
	if (IS_ERR(channels)) {
		if (PTR_ERR(channels) == -ENODEV)
			return -EPROBE_DEFER;
		return PTR_ERR(channels);
	}

	st = devm_kzalloc(dev, sizeof(*st), GFP_KERNEL);
	if (st == NULL)
		return -ENOMEM;

	st->channels = channels;

	/* count how many attributes we have */
	while (st->channels[st->num_channels].indio_dev)
		st->num_channels++;

	st->attrs = devm_kcalloc(dev,
				 st->num_channels + 1, sizeof(*st->attrs),
				 GFP_KERNEL);
	if (st->attrs == NULL)
		return -ENOMEM;

	for (i = 0; i < st->num_channels; i++) {
<<<<<<< HEAD
=======
		const char *prefix;
		int n;

>>>>>>> upstream/android-13
		a = devm_kzalloc(dev, sizeof(*a), GFP_KERNEL);
		if (a == NULL)
			return -ENOMEM;

		sysfs_attr_init(&a->dev_attr.attr);
		ret = iio_get_channel_type(&st->channels[i], &type);
		if (ret < 0)
			return ret;

		switch (type) {
		case IIO_VOLTAGE:
<<<<<<< HEAD
			a->dev_attr.attr.name = devm_kasprintf(dev, GFP_KERNEL,
							       "in%d_input",
							       in_i++);
			break;
		case IIO_TEMP:
			a->dev_attr.attr.name = devm_kasprintf(dev, GFP_KERNEL,
							       "temp%d_input",
							       temp_i++);
			break;
		case IIO_CURRENT:
			a->dev_attr.attr.name = devm_kasprintf(dev, GFP_KERNEL,
							       "curr%d_input",
							       curr_i++);
			break;
		case IIO_HUMIDITYRELATIVE:
			a->dev_attr.attr.name = devm_kasprintf(dev, GFP_KERNEL,
							       "humidity%d_input",
							       humidity_i++);
=======
			n = in_i++;
			prefix = "in";
			break;
		case IIO_TEMP:
			n = temp_i++;
			prefix = "temp";
			break;
		case IIO_CURRENT:
			n = curr_i++;
			prefix = "curr";
			break;
		case IIO_POWER:
			n = power_i++;
			prefix = "power";
			break;
		case IIO_HUMIDITYRELATIVE:
			n = humidity_i++;
			prefix = "humidity";
>>>>>>> upstream/android-13
			break;
		default:
			return -EINVAL;
		}
<<<<<<< HEAD
=======

		a->dev_attr.attr.name = devm_kasprintf(dev, GFP_KERNEL,
						       "%s%d_input",
						       prefix, n);
>>>>>>> upstream/android-13
		if (a->dev_attr.attr.name == NULL)
			return -ENOMEM;

		a->dev_attr.show = iio_hwmon_read_val;
<<<<<<< HEAD
		a->dev_attr.attr.mode = S_IRUGO;
=======
		a->dev_attr.attr.mode = 0444;
>>>>>>> upstream/android-13
		a->index = i;
		st->attrs[i] = &a->dev_attr.attr;
	}

	st->attr_group.attrs = st->attrs;
	st->groups[0] = &st->attr_group;

<<<<<<< HEAD
	sname = devm_kstrdup(dev, name, GFP_KERNEL);
	if (!sname)
		return -ENOMEM;

	strreplace(sname, '-', '_');
=======
	if (dev->of_node) {
		sname = devm_kasprintf(dev, GFP_KERNEL, "%pOFn", dev->of_node);
		if (!sname)
			return -ENOMEM;
		strreplace(sname, '-', '_');
	} else {
		sname = "iio_hwmon";
	}

>>>>>>> upstream/android-13
	hwmon_dev = devm_hwmon_device_register_with_groups(dev, sname, st,
							   st->groups);
	return PTR_ERR_OR_ZERO(hwmon_dev);
}

static const struct of_device_id iio_hwmon_of_match[] = {
	{ .compatible = "iio-hwmon", },
	{ }
};
MODULE_DEVICE_TABLE(of, iio_hwmon_of_match);

<<<<<<< HEAD
static struct platform_driver __refdata iio_hwmon_driver = {
=======
static struct platform_driver iio_hwmon_driver = {
>>>>>>> upstream/android-13
	.driver = {
		.name = "iio_hwmon",
		.of_match_table = iio_hwmon_of_match,
	},
	.probe = iio_hwmon_probe,
};

module_platform_driver(iio_hwmon_driver);

MODULE_AUTHOR("Jonathan Cameron <jic23@kernel.org>");
MODULE_DESCRIPTION("IIO to hwmon driver");
MODULE_LICENSE("GPL v2");
