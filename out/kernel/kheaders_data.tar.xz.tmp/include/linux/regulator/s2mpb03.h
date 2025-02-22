

#ifndef __LINUX_MFD_S2MPB03_H
#define __LINUX_MFD_S2MPB03_H
#include <linux/platform_device.h>
#include <linux/regmap.h>

#define MFD_DEV_NAME "s2mpb03"



struct s2mpb03_dev {
	struct device *dev;
	struct i2c_client *i2c;
	struct mutex i2c_lock;

	int type;
	u8 rev_num; 
	bool wakeup;

	struct s2mpb03_platform_data *pdata;
};

struct s2mpb03_regulator_data {
	int id;
	struct regulator_init_data *initdata;
	struct device_node *reg_node;
};

struct s2mpb03_platform_data {
	bool wakeup;
	bool need_recovery;
	int num_regulators;
	int num_rdata;
	struct	s2mpb03_regulator_data *regulators;
	int	device_type;
};

struct s2mpb03 {
	struct regmap *regmap;
};



enum S2MPB03_reg {
	S2MPB03_REG_PMIC_ID,
	S2MPB03_REG_STATUS,
	S2MPB03_REG_CTRL,
	S2MPB03_REG_LDO1_CTRL,
	S2MPB03_REG_LDO2_CTRL,
	S2MPB03_REG_LDO3_CTRL,
	S2MPB03_REG_LDO4_CTRL,
	S2MPB03_REG_LDO5_CTRL,
	S2MPB03_REG_LDO6_CTRL,
	S2MPB03_REG_LDO7_CTRL,
	S2MPB03_REG_LDO_SLEW1,
	S2MPB03_REG_LDO_SLEW2,
};


enum S2MPB03_regulators {
	S2MPB03_LDO1,
	S2MPB03_LDO2,
	S2MPB03_LDO3,
	S2MPB03_LDO4,
	S2MPB03_LDO5,
	S2MPB03_LDO6,
	S2MPB03_LDO7,
	S2MPB03_REG_MAX,
};

#define S2MPB03_LDO_MIN1	700000
#define S2MPB03_LDO_MIN2	1800000
#define S2MPB03_LDO_STEP1	25000
#define S2MPB03_LDO_STEP2	12500
#define S2MPB03_LDO_VSEL_MASK	0x3F
#define S2MPB03_LDO_ENABLE_MASK	0x80

#define S2MPB03_RAMP_DELAY	12000

#define S2MPB03_ENABLE_TIME_LDO		150

#define S2MPB03_ENABLE_SHIFT	0x07
#define S2MPB03_LDO_N_VOLTAGES	(S2MPB03_LDO_VSEL_MASK + 1)

#define S2MPB03_REGULATOR_MAX (S2MPB03_REG_MAX)

#endif 
