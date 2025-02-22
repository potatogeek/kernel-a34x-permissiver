<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * PS3 RTC Driver
 *
 * Copyright 2009 Sony Corporation
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/rtc.h>

#include <asm/lv1call.h>
#include <asm/ps3.h>


static u64 read_rtc(void)
{
	int result;
	u64 rtc_val;
	u64 tb_val;

	result = lv1_get_rtc(&rtc_val, &tb_val);
	BUG_ON(result);

	return rtc_val;
}

static int ps3_get_time(struct device *dev, struct rtc_time *tm)
{
<<<<<<< HEAD
	rtc_time_to_tm(read_rtc() + ps3_os_area_get_rtc_diff(), tm);
=======
	rtc_time64_to_tm(read_rtc() + ps3_os_area_get_rtc_diff(), tm);
>>>>>>> upstream/android-13
	return 0;
}

static int ps3_set_time(struct device *dev, struct rtc_time *tm)
{
<<<<<<< HEAD
	unsigned long now;

	rtc_tm_to_time(tm, &now);
	ps3_os_area_set_rtc_diff(now - read_rtc());
=======
	ps3_os_area_set_rtc_diff(rtc_tm_to_time64(tm) - read_rtc());
>>>>>>> upstream/android-13
	return 0;
}

static const struct rtc_class_ops ps3_rtc_ops = {
	.read_time = ps3_get_time,
	.set_time = ps3_set_time,
};

static int __init ps3_rtc_probe(struct platform_device *dev)
{
	struct rtc_device *rtc;

<<<<<<< HEAD
	rtc = devm_rtc_device_register(&dev->dev, "rtc-ps3", &ps3_rtc_ops,
				  THIS_MODULE);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	platform_set_drvdata(dev, rtc);
	return 0;
=======
	rtc = devm_rtc_allocate_device(&dev->dev);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	rtc->ops = &ps3_rtc_ops;
	rtc->range_max = U64_MAX;

	platform_set_drvdata(dev, rtc);

	return devm_rtc_register_device(rtc);
>>>>>>> upstream/android-13
}

static struct platform_driver ps3_rtc_driver = {
	.driver = {
		.name = "rtc-ps3",
	},
};

module_platform_driver_probe(ps3_rtc_driver, ps3_rtc_probe);

MODULE_AUTHOR("Sony Corporation");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ps3 RTC driver");
MODULE_ALIAS("platform:rtc-ps3");
