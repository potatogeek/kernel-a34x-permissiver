

#ifndef ST_SENSORS_I2C_H
#define ST_SENSORS_I2C_H

#include <linux/i2c.h>
#include <linux/iio/common/st_sensors.h>
#include <linux/of.h>

void st_sensors_i2c_configure(struct iio_dev *indio_dev,
		struct i2c_client *client, struct st_sensor_data *sdata);

#ifdef CONFIG_ACPI
int st_sensors_match_acpi_device(struct device *dev);
#else
static inline int st_sensors_match_acpi_device(struct device *dev)
{
	return -ENODEV;
}
#endif

#endif 
