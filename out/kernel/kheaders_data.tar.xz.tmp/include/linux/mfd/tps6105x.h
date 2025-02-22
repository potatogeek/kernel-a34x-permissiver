
#ifndef MFD_TPS6105X_H
#define MFD_TPS6105X_H

#include <linux/i2c.h>
#include <linux/regmap.h>
#include <linux/regulator/machine.h>


#define TPS6105X_REG_0			0x00
#define TPS6105X_REG0_MODE_SHIFT	6
#define TPS6105X_REG0_MODE_MASK		(0x03<<6)

#define TPS6105X_REG0_MODE_SHUTDOWN	0x00
#define TPS6105X_REG0_MODE_TORCH	0x01
#define TPS6105X_REG0_MODE_TORCH_FLASH	0x02
#define TPS6105X_REG0_MODE_VOLTAGE	0x03
#define TPS6105X_REG0_VOLTAGE_SHIFT	4
#define TPS6105X_REG0_VOLTAGE_MASK	(3<<4)
#define TPS6105X_REG0_VOLTAGE_450	0
#define TPS6105X_REG0_VOLTAGE_500	1
#define TPS6105X_REG0_VOLTAGE_525	2
#define TPS6105X_REG0_VOLTAGE_500_2	3
#define TPS6105X_REG0_DIMMING_SHIFT	3
#define TPS6105X_REG0_TORCHC_SHIFT	0
#define TPS6105X_REG0_TORCHC_MASK	(7<<0)
#define TPS6105X_REG0_TORCHC_0		0x00
#define TPS6105X_REG0_TORCHC_50		0x01
#define TPS6105X_REG0_TORCHC_75		0x02
#define TPS6105X_REG0_TORCHC_100	0x03
#define TPS6105X_REG0_TORCHC_150	0x04
#define TPS6105X_REG0_TORCHC_200	0x05
#define TPS6105X_REG0_TORCHC_250_400	0x06
#define TPS6105X_REG0_TORCHC_250_500	0x07
#define TPS6105X_REG_1			0x01
#define TPS6105X_REG1_MODE_SHIFT	6
#define TPS6105X_REG1_MODE_MASK		(0x03<<6)
#define TPS6105X_REG1_MODE_SHUTDOWN	0x00
#define TPS6105X_REG1_MODE_TORCH	0x01
#define TPS6105X_REG1_MODE_TORCH_FLASH	0x02
#define TPS6105X_REG1_MODE_VOLTAGE	0x03
#define TPS6105X_REG_2			0x02
#define TPS6105X_REG_3			0x03


enum tps6105x_mode {
	TPS6105X_MODE_SHUTDOWN,
	TPS6105X_MODE_TORCH,
	TPS6105X_MODE_TORCH_FLASH,
	TPS6105X_MODE_VOLTAGE,
};


struct tps6105x_platform_data {
	enum tps6105x_mode mode;
	struct regulator_init_data *regulator_data;
};


struct tps6105x {
	struct tps6105x_platform_data *pdata;
	struct i2c_client	*client;
	struct regulator_dev	*regulator;
	struct regmap		*regmap;
};

#endif
