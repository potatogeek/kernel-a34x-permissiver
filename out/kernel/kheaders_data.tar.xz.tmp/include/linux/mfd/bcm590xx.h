

#ifndef __LINUX_MFD_BCM590XX_H
#define __LINUX_MFD_BCM590XX_H

#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/regmap.h>


#define BCM590XX_MAX_REGISTER_PRI	0xe7
#define BCM590XX_MAX_REGISTER_SEC	0xf0

struct bcm590xx {
	struct device *dev;
	struct i2c_client *i2c_pri;
	struct i2c_client *i2c_sec;
	struct regmap *regmap_pri;
	struct regmap *regmap_sec;
	unsigned int id;
};

#endif 
