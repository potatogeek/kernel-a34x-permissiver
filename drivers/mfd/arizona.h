<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * wm5102.h  --  WM5102 MFD internals
 *
 * Copyright 2012 Wolfson Microelectronics plc
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef _WM5102_H
#define _WM5102_H

#include <linux/of.h>
#include <linux/regmap.h>
#include <linux/pm.h>

extern const struct regmap_config wm5102_i2c_regmap;
extern const struct regmap_config wm5102_spi_regmap;

extern const struct regmap_config wm5110_i2c_regmap;
extern const struct regmap_config wm5110_spi_regmap;

extern const struct regmap_config cs47l24_spi_regmap;

extern const struct regmap_config wm8997_i2c_regmap;

extern const struct regmap_config wm8998_i2c_regmap;

extern const struct dev_pm_ops arizona_pm_ops;

extern const struct of_device_id arizona_of_match[];

extern const struct regmap_irq_chip wm5102_aod;
extern const struct regmap_irq_chip wm5102_irq;

extern const struct regmap_irq_chip wm5110_aod;
extern const struct regmap_irq_chip wm5110_irq;
extern const struct regmap_irq_chip wm5110_revd_irq;

extern const struct regmap_irq_chip cs47l24_irq;

extern const struct regmap_irq_chip wm8997_aod;
extern const struct regmap_irq_chip wm8997_irq;

extern struct regmap_irq_chip wm8998_aod;
extern struct regmap_irq_chip wm8998_irq;

int arizona_dev_init(struct arizona *arizona);
int arizona_dev_exit(struct arizona *arizona);
int arizona_irq_init(struct arizona *arizona);
int arizona_irq_exit(struct arizona *arizona);

<<<<<<< HEAD
#ifdef CONFIG_OF
unsigned long arizona_of_get_type(struct device *dev);
#else
static inline unsigned long arizona_of_get_type(struct device *dev)
{
	return 0;
}
#endif

=======
>>>>>>> upstream/android-13
#endif
