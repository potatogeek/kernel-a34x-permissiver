<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * STMicroelectronics sensors i2c library driver
 *
 * Copyright 2012-2013 STMicroelectronics Inc.
 *
 * Denis Ciocca <denis.ciocca@st.com>
<<<<<<< HEAD
 *
 * Licensed under the GPL-2.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/iio/iio.h>
#include <linux/of_device.h>
#include <linux/acpi.h>

#include <linux/iio/common/st_sensors_i2c.h>


#define ST_SENSORS_I2C_MULTIREAD	0x80

static unsigned int st_sensors_i2c_get_irq(struct iio_dev *indio_dev)
{
	struct st_sensor_data *sdata = iio_priv(indio_dev);

	return to_i2c_client(sdata->dev)->irq;
}

static int st_sensors_i2c_read_byte(struct st_sensor_transfer_buffer *tb,
				struct device *dev, u8 reg_addr, u8 *res_byte)
{
	int err;

	err = i2c_smbus_read_byte_data(to_i2c_client(dev), reg_addr);
	if (err < 0)
		goto st_accel_i2c_read_byte_error;

	*res_byte = err & 0xff;

st_accel_i2c_read_byte_error:
	return err < 0 ? err : 0;
}

static int st_sensors_i2c_read_multiple_byte(
		struct st_sensor_transfer_buffer *tb, struct device *dev,
			u8 reg_addr, int len, u8 *data, bool multiread_bit)
{
	if (multiread_bit)
		reg_addr |= ST_SENSORS_I2C_MULTIREAD;

	return i2c_smbus_read_i2c_block_data_or_emulated(to_i2c_client(dev),
							 reg_addr, len, data);
}

static int st_sensors_i2c_write_byte(struct st_sensor_transfer_buffer *tb,
				struct device *dev, u8 reg_addr, u8 data)
{
	return i2c_smbus_write_byte_data(to_i2c_client(dev), reg_addr, data);
}

static const struct st_sensor_transfer_function st_sensors_tf_i2c = {
	.read_byte = st_sensors_i2c_read_byte,
	.write_byte = st_sensors_i2c_write_byte,
	.read_multiple_byte = st_sensors_i2c_read_multiple_byte,
};

void st_sensors_i2c_configure(struct iio_dev *indio_dev,
		struct i2c_client *client, struct st_sensor_data *sdata)
{
	i2c_set_clientdata(client, indio_dev);

	indio_dev->dev.parent = &client->dev;
	indio_dev->name = client->name;

	sdata->dev = &client->dev;
	sdata->tf = &st_sensors_tf_i2c;
	sdata->get_irq_data_ready = st_sensors_i2c_get_irq;
}
EXPORT_SYMBOL(st_sensors_i2c_configure);

#ifdef CONFIG_ACPI
int st_sensors_match_acpi_device(struct device *dev)
{
	const struct acpi_device_id *acpi_id;
	kernel_ulong_t driver_data = 0;

	if (ACPI_HANDLE(dev)) {
		acpi_id = acpi_match_device(dev->driver->acpi_match_table, dev);
		if (!acpi_id) {
			dev_err(dev, "No driver data\n");
			return -EINVAL;
		}
		driver_data = acpi_id->driver_data;
	}
	return driver_data;
}
EXPORT_SYMBOL(st_sensors_match_acpi_device);
#endif

=======
 */

#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/iio/iio.h>
#include <linux/regmap.h>

#include <linux/iio/common/st_sensors_i2c.h>

#define ST_SENSORS_I2C_MULTIREAD	0x80

static const struct regmap_config st_sensors_i2c_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
};

static const struct regmap_config st_sensors_i2c_regmap_multiread_bit_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.read_flag_mask = ST_SENSORS_I2C_MULTIREAD,
};

/*
 * st_sensors_i2c_configure() - configure I2C interface
 * @indio_dev: IIO device reference.
 * @client: i2c client reference.
 *
 * Return: 0 on success, else a negative error code.
 */
int st_sensors_i2c_configure(struct iio_dev *indio_dev,
			     struct i2c_client *client)
{
	struct st_sensor_data *sdata = iio_priv(indio_dev);
	const struct regmap_config *config;

	if (sdata->sensor_settings->multi_read_bit)
		config = &st_sensors_i2c_regmap_multiread_bit_config;
	else
		config = &st_sensors_i2c_regmap_config;

	sdata->regmap = devm_regmap_init_i2c(client, config);
	if (IS_ERR(sdata->regmap)) {
		dev_err(&client->dev, "Failed to register i2c regmap (%ld)\n",
			PTR_ERR(sdata->regmap));
		return PTR_ERR(sdata->regmap);
	}

	i2c_set_clientdata(client, indio_dev);

	indio_dev->name = client->name;

	sdata->dev = &client->dev;
	sdata->irq = client->irq;

	return 0;
}
EXPORT_SYMBOL(st_sensors_i2c_configure);

>>>>>>> upstream/android-13
MODULE_AUTHOR("Denis Ciocca <denis.ciocca@st.com>");
MODULE_DESCRIPTION("STMicroelectronics ST-sensors i2c driver");
MODULE_LICENSE("GPL v2");
