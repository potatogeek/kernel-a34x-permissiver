<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * cros_ec_baro - Driver for barometer sensor behind CrosEC.
 *
 * Copyright (C) 2017 Google, Inc
<<<<<<< HEAD
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/delay.h>
=======
 */

>>>>>>> upstream/android-13
#include <linux/device.h>
#include <linux/iio/buffer.h>
#include <linux/iio/common/cros_ec_sensors_core.h>
#include <linux/iio/iio.h>
#include <linux/iio/kfifo_buf.h>
#include <linux/iio/trigger.h>
#include <linux/iio/triggered_buffer.h>
#include <linux/iio/trigger_consumer.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/mfd/cros_ec.h>
#include <linux/mfd/cros_ec_commands.h>
#include <linux/module.h>
#include <linux/slab.h>
=======
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/platform_data/cros_ec_commands.h>
#include <linux/platform_data/cros_ec_proto.h>
>>>>>>> upstream/android-13
#include <linux/platform_device.h>

/*
 * One channel for pressure, the other for timestamp.
 */
#define CROS_EC_BARO_MAX_CHANNELS (1 + 1)

/* State data for ec_sensors iio driver. */
struct cros_ec_baro_state {
	/* Shared by all sensors */
	struct cros_ec_sensors_core_state core;

	struct iio_chan_spec channels[CROS_EC_BARO_MAX_CHANNELS];
};

static int cros_ec_baro_read(struct iio_dev *indio_dev,
			     struct iio_chan_spec const *chan,
			     int *val, int *val2, long mask)
{
	struct cros_ec_baro_state *st = iio_priv(indio_dev);
	u16 data = 0;
<<<<<<< HEAD
	int ret = IIO_VAL_INT;
=======
	int ret;
>>>>>>> upstream/android-13
	int idx = chan->scan_index;

	mutex_lock(&st->core.cmd_lock);

	switch (mask) {
	case IIO_CHAN_INFO_RAW:
<<<<<<< HEAD
		if (cros_ec_sensors_read_cmd(indio_dev, 1 << idx,
					     (s16 *)&data) < 0)
			ret = -EIO;
		*val = data;
=======
		ret = cros_ec_sensors_read_cmd(indio_dev, 1 << idx,
					     (s16 *)&data);
		if (ret)
			break;

		*val = data;
		ret = IIO_VAL_INT;
>>>>>>> upstream/android-13
		break;
	case IIO_CHAN_INFO_SCALE:
		st->core.param.cmd = MOTIONSENSE_CMD_SENSOR_RANGE;
		st->core.param.sensor_range.data = EC_MOTION_SENSE_NO_VALUE;

<<<<<<< HEAD
		if (cros_ec_motion_send_host_cmd(&st->core, 0)) {
			ret = -EIO;
			break;
		}
=======
		ret = cros_ec_motion_send_host_cmd(&st->core, 0);
		if (ret)
			break;

>>>>>>> upstream/android-13
		*val = st->core.resp->sensor_range.ret;

		/* scale * in_pressure_raw --> kPa */
		*val2 = 10 << CROS_EC_SENSOR_BITS;
		ret = IIO_VAL_FRACTIONAL;
		break;
	default:
		ret = cros_ec_sensors_core_read(&st->core, chan, val, val2,
						mask);
		break;
	}

	mutex_unlock(&st->core.cmd_lock);

	return ret;
}

static int cros_ec_baro_write(struct iio_dev *indio_dev,
			      struct iio_chan_spec const *chan,
			      int val, int val2, long mask)
{
	struct cros_ec_baro_state *st = iio_priv(indio_dev);
	int ret = 0;

	mutex_lock(&st->core.cmd_lock);

	switch (mask) {
	case IIO_CHAN_INFO_SCALE:
		st->core.param.cmd = MOTIONSENSE_CMD_SENSOR_RANGE;
		st->core.param.sensor_range.data = val;

		/* Always roundup, so caller gets at least what it asks for. */
		st->core.param.sensor_range.roundup = 1;

<<<<<<< HEAD
		if (cros_ec_motion_send_host_cmd(&st->core, 0))
			ret = -EIO;
=======
		ret = cros_ec_motion_send_host_cmd(&st->core, 0);
		if (ret == 0) {
			st->core.range_updated = true;
			st->core.curr_range = val;
		}
>>>>>>> upstream/android-13
		break;
	default:
		ret = cros_ec_sensors_core_write(&st->core, chan, val, val2,
						 mask);
		break;
	}

	mutex_unlock(&st->core.cmd_lock);

	return ret;
}

static const struct iio_info cros_ec_baro_info = {
	.read_raw = &cros_ec_baro_read,
	.write_raw = &cros_ec_baro_write,
<<<<<<< HEAD
=======
	.read_avail = &cros_ec_sensors_core_read_avail,
>>>>>>> upstream/android-13
};

static int cros_ec_baro_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct cros_ec_dev *ec_dev = dev_get_drvdata(dev->parent);
	struct iio_dev *indio_dev;
	struct cros_ec_baro_state *state;
	struct iio_chan_spec *channel;
	int ret;

	if (!ec_dev || !ec_dev->ec_dev) {
		dev_warn(dev, "No CROS EC device found.\n");
		return -EINVAL;
	}

	indio_dev = devm_iio_device_alloc(dev, sizeof(*state));
	if (!indio_dev)
		return -ENOMEM;

<<<<<<< HEAD
	ret = cros_ec_sensors_core_init(pdev, indio_dev, true);
=======
	ret = cros_ec_sensors_core_init(pdev, indio_dev, true,
					cros_ec_sensors_capture,
					cros_ec_sensors_push_data);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	indio_dev->info = &cros_ec_baro_info;
	state = iio_priv(indio_dev);
	state->core.type = state->core.resp->info.type;
	state->core.loc = state->core.resp->info.location;
	channel = state->channels;
	/* Common part */
	channel->info_mask_separate = BIT(IIO_CHAN_INFO_RAW);
	channel->info_mask_shared_by_all =
		BIT(IIO_CHAN_INFO_SCALE) |
<<<<<<< HEAD
		BIT(IIO_CHAN_INFO_SAMP_FREQ) |
		BIT(IIO_CHAN_INFO_FREQUENCY);
=======
		BIT(IIO_CHAN_INFO_SAMP_FREQ);
	channel->info_mask_shared_by_all_available =
		BIT(IIO_CHAN_INFO_SAMP_FREQ);
>>>>>>> upstream/android-13
	channel->scan_type.realbits = CROS_EC_SENSOR_BITS;
	channel->scan_type.storagebits = CROS_EC_SENSOR_BITS;
	channel->scan_type.shift = 0;
	channel->scan_index = 0;
	channel->ext_info = cros_ec_sensors_ext_info;
	channel->scan_type.sign = 'u';

<<<<<<< HEAD
	state->core.calib[0] = 0;

=======
>>>>>>> upstream/android-13
	/* Sensor specific */
	switch (state->core.type) {
	case MOTIONSENSE_TYPE_BARO:
		channel->type = IIO_PRESSURE;
		break;
	default:
		dev_warn(dev, "Unknown motion sensor\n");
		return -EINVAL;
	}

	/* Timestamp */
	channel++;
	channel->type = IIO_TIMESTAMP;
	channel->channel = -1;
	channel->scan_index = 1;
	channel->scan_type.sign = 's';
	channel->scan_type.realbits = 64;
	channel->scan_type.storagebits = 64;

	indio_dev->channels = state->channels;
	indio_dev->num_channels = CROS_EC_BARO_MAX_CHANNELS;

	state->core.read_ec_sensors_data = cros_ec_sensors_read_cmd;

<<<<<<< HEAD
	ret = devm_iio_triggered_buffer_setup(dev, indio_dev, NULL,
					      cros_ec_sensors_capture, NULL);
	if (ret)
		return ret;

=======
>>>>>>> upstream/android-13
	return devm_iio_device_register(dev, indio_dev);
}

static const struct platform_device_id cros_ec_baro_ids[] = {
	{
		.name = "cros-ec-baro",
	},
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(platform, cros_ec_baro_ids);

static struct platform_driver cros_ec_baro_platform_driver = {
	.driver = {
		.name	= "cros-ec-baro",
<<<<<<< HEAD
=======
		.pm	= &cros_ec_sensors_pm_ops,
>>>>>>> upstream/android-13
	},
	.probe		= cros_ec_baro_probe,
	.id_table	= cros_ec_baro_ids,
};
module_platform_driver(cros_ec_baro_platform_driver);

MODULE_DESCRIPTION("ChromeOS EC barometer sensor driver");
MODULE_LICENSE("GPL v2");
