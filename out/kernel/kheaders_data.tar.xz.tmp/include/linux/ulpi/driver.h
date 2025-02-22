/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_ULPI_DRIVER_H
#define __LINUX_ULPI_DRIVER_H

#include <linux/mod_devicetable.h>

#include <linux/device.h>

struct ulpi_ops;


struct ulpi {
	struct ulpi_device_id id;
	const struct ulpi_ops *ops;
	struct device dev;
};

#define to_ulpi_dev(d) container_of(d, struct ulpi, dev)

static inline void ulpi_set_drvdata(struct ulpi *ulpi, void *data)
{
	dev_set_drvdata(&ulpi->dev, data);
}

static inline void *ulpi_get_drvdata(struct ulpi *ulpi)
{
	return dev_get_drvdata(&ulpi->dev);
}


struct ulpi_driver {
	const struct ulpi_device_id *id_table;
	int (*probe)(struct ulpi *ulpi);
	void (*remove)(struct ulpi *ulpi);
	struct device_driver driver;
};

#define to_ulpi_driver(d) container_of(d, struct ulpi_driver, driver)


#define ulpi_register_driver(drv) __ulpi_register_driver(drv, THIS_MODULE)
int __ulpi_register_driver(struct ulpi_driver *drv, struct module *module);
void ulpi_unregister_driver(struct ulpi_driver *drv);

#define module_ulpi_driver(__ulpi_driver) \
	module_driver(__ulpi_driver, ulpi_register_driver, \
		      ulpi_unregister_driver)

int ulpi_read(struct ulpi *ulpi, u8 addr);
int ulpi_write(struct ulpi *ulpi, u8 addr, u8 val);

#endif 
