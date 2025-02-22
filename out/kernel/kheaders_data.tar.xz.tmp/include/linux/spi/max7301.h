/* SPDX-License-Identifier: GPL-2.0 */
#ifndef LINUX_SPI_MAX7301_H
#define LINUX_SPI_MAX7301_H

#include <linux/gpio.h>


struct max7301 {
	struct mutex	lock;
	u8		port_config[8];	
	u32		out_level;	
	u32		input_pullup_active;
	struct gpio_chip chip;
	struct device *dev;
	int (*write)(struct device *dev, unsigned int reg, unsigned int val);
	int (*read)(struct device *dev, unsigned int reg);
};

struct max7301_platform_data {
	
	unsigned	base;
	
	u32		input_pullup_active;
};

extern int __max730x_remove(struct device *dev);
extern int __max730x_probe(struct max7301 *ts);
#endif
