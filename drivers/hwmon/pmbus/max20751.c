<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Hardware monitoring driver for Maxim MAX20751
 *
 * Copyright (c) 2015 Guenter Roeck
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
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include "pmbus.h"

static struct pmbus_driver_info max20751_info = {
	.pages = 1,
	.format[PSC_VOLTAGE_IN] = linear,
	.format[PSC_VOLTAGE_OUT] = vid,
<<<<<<< HEAD
	.vrm_version = vr12,
=======
	.vrm_version[0] = vr12,
>>>>>>> upstream/android-13
	.format[PSC_TEMPERATURE] = linear,
	.format[PSC_CURRENT_OUT] = linear,
	.format[PSC_POWER] = linear,
	.func[0] = PMBUS_HAVE_VIN | PMBUS_HAVE_VOUT | PMBUS_HAVE_STATUS_VOUT |
		PMBUS_HAVE_IOUT | PMBUS_HAVE_STATUS_IOUT |
		PMBUS_HAVE_TEMP | PMBUS_HAVE_STATUS_TEMP |
		PMBUS_HAVE_POUT,
};

<<<<<<< HEAD
static int max20751_probe(struct i2c_client *client,
			  const struct i2c_device_id *id)
{
	return pmbus_do_probe(client, id, &max20751_info);
=======
static int max20751_probe(struct i2c_client *client)
{
	return pmbus_do_probe(client, &max20751_info);
>>>>>>> upstream/android-13
}

static const struct i2c_device_id max20751_id[] = {
	{"max20751", 0},
	{}
};

MODULE_DEVICE_TABLE(i2c, max20751_id);

static struct i2c_driver max20751_driver = {
	.driver = {
		   .name = "max20751",
		   },
<<<<<<< HEAD
	.probe = max20751_probe,
	.remove = pmbus_do_remove,
=======
	.probe_new = max20751_probe,
>>>>>>> upstream/android-13
	.id_table = max20751_id,
};

module_i2c_driver(max20751_driver);

MODULE_AUTHOR("Guenter Roeck <linux@roeck-us.net>");
MODULE_DESCRIPTION("PMBus driver for Maxim MAX20751");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_IMPORT_NS(PMBUS);
>>>>>>> upstream/android-13
