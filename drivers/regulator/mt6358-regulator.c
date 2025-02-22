<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2016 MediaTek Inc.
 */


#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/mfd/mt6358/core.h>
#include <linux/mfd/mt6358/registers.h>
=======
// SPDX-License-Identifier: GPL-2.0
//
// Copyright (c) 2019 MediaTek Inc.

#include <linux/mfd/mt6358/registers.h>
#include <linux/mfd/mt6397/core.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
>>>>>>> upstream/android-13
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/mt6358-regulator.h>
#include <linux/regulator/of_regulator.h>

<<<<<<< HEAD
#define MT6358_BUCK_MODE_AUTO		0
#define MT6358_BUCK_MODE_FORCE_PWM	1

#define DEF_OC_IRQ_ENABLE_DELAY_MS	10

=======
#define MT6358_BUCK_MODE_AUTO	0
#define MT6358_BUCK_MODE_FORCE_PWM	1

>>>>>>> upstream/android-13
/*
 * MT6358 regulators' information
 *
 * @desc: standard fields of regulator description.
<<<<<<< HEAD
 * @constraints: standard fields of regulator constraints.
 * @da_reg: for query status of regulators.
 * @qi: Mask for query enable signal status of regulators.
 * @modeset_reg: for operating AUTO/PWM mode register.
 * @modeset_mask: MASK for operating modeset register.
 * @modeset_shift: SHIFT for operating modeset register.
 */
struct mt6358_regulator_info {
	int irq;
	int oc_irq_enable_delay_ms;
	struct delayed_work oc_work;
	struct regulator_desc desc;
	struct regulation_constraints constraints;
	u32 da_vsel_reg;
	u32 da_vsel_mask;
	u32 da_vsel_shift;
	u32 da_reg;
	u32 qi;
	u32 modeset_reg;
	u32 modeset_mask;
	u32 modeset_shift;
};

/*
 * MTK regulators' init data
 *
 * @id: chip id
 * @size: num of regulators
 * @regulator_info: regulator info.
 */
struct mt_regulator_init_data {
	u32 id;
	u32 size;
	struct mt6358_regulator_info *regulator_info;
};

#define MT_BUCK_EN		(REGULATOR_CHANGE_STATUS)
#define MT_BUCK_VOL		(REGULATOR_CHANGE_VOLTAGE)
#define MT_BUCK_VOL_EN		(REGULATOR_CHANGE_STATUS |	\
				 REGULATOR_CHANGE_VOLTAGE)
#define MT_BUCK_VOL_EN_MODE	(REGULATOR_CHANGE_STATUS |	\
				 REGULATOR_CHANGE_VOLTAGE |	\
				 REGULATOR_CHANGE_MODE)

#define MT_LDO_EN		(REGULATOR_CHANGE_STATUS)
#define MT_LDO_VOL		(REGULATOR_CHANGE_VOLTAGE)
#define MT_LDO_VOL_EN		(REGULATOR_CHANGE_STATUS |	\
				 REGULATOR_CHANGE_VOLTAGE)

#define MT_BUCK(match, _name, min, max, step,				\
		min_sel, volt_ranges, _enable_reg,			\
		_da_reg,						\
		_da_vsel_reg, _da_vsel_mask, _da_vsel_shift,		\
		_vsel_reg, _vsel_mask,					\
		mode, _modeset_reg, _modeset_shift)			\
[MT6358_ID_##_name] = {							\
	.desc = {							\
		.name = #_name,						\
		.of_match = of_match_ptr(match),			\
		.ops = &mt6358_volt_range_ops,				\
		.type = REGULATOR_VOLTAGE,				\
		.id = MT6358_ID_##_name,				\
		.owner = THIS_MODULE,					\
		.uV_step = (step),					\
		.linear_min_sel = (min_sel),				\
		.n_voltages = ((max) - (min)) / (step) + 1,		\
		.min_uV = (min),					\
		.linear_ranges = volt_ranges,				\
		.n_linear_ranges = ARRAY_SIZE(volt_ranges),		\
		.vsel_reg = _vsel_reg,					\
		.vsel_mask = _vsel_mask,				\
		.enable_reg = _enable_reg,				\
		.enable_mask = BIT(0),					\
	},								\
	.constraints = {						\
		.valid_ops_mask = (mode),				\
		.valid_modes_mask =					\
			(REGULATOR_MODE_NORMAL | REGULATOR_MODE_FAST),	\
	},								\
	.da_vsel_reg = _da_vsel_reg,					\
	.da_vsel_mask = _da_vsel_mask,					\
	.da_vsel_shift = _da_vsel_shift,				\
	.da_reg = _da_reg,						\
	.qi = BIT(0),							\
	.modeset_reg = _modeset_reg,					\
	.modeset_mask = BIT(_modeset_shift),				\
	.modeset_shift = _modeset_shift					\
}

#define MT_LDO_REGULAR(match, _name, min, max, step,			\
		       min_sel, volt_ranges, _enable_reg,		\
		       _da_reg,						\
		       _da_vsel_reg, _da_vsel_mask, _da_vsel_shift,	\
		       _vsel_reg, _vsel_mask,				\
		       mode)						\
[MT6358_ID_##_name] = {							\
	.desc = {							\
		.name = #_name,						\
		.of_match = of_match_ptr(match),			\
		.ops = &mt6358_volt_range_ops,				\
		.type = REGULATOR_VOLTAGE,				\
		.id = MT6358_ID_##_name,				\
		.owner = THIS_MODULE,					\
		.uV_step = (step),					\
		.linear_min_sel = (min_sel),				\
		.n_voltages = ((max) - (min)) / (step) + 1,		\
		.min_uV = (min),					\
		.linear_ranges = volt_ranges,				\
		.n_linear_ranges = ARRAY_SIZE(volt_ranges),		\
		.vsel_reg = _vsel_reg,					\
		.vsel_mask = _vsel_mask,				\
		.enable_reg = _enable_reg,				\
		.enable_mask = BIT(0),					\
	},								\
	.constraints = {						\
		.valid_ops_mask = (mode),				\
	},								\
	.da_vsel_reg = _da_vsel_reg,					\
	.da_vsel_mask = _da_vsel_mask,					\
	.da_vsel_shift = _da_vsel_shift,				\
	.da_reg = _da_reg,						\
	.qi = BIT(0),							\
}

#define MT_LDO_NON_REGULAR(match, _name,				\
			   _volt_table, _enable_reg,			\
			   _da_reg,					\
			   _vsel_reg, _vsel_mask,			\
			   mode)					\
[MT6358_ID_##_name] = {							\
	.desc = {							\
		.name = #_name,						\
		.of_match = of_match_ptr(match),			\
		.ops = &mt6358_volt_table_ops,				\
		.type = REGULATOR_VOLTAGE,				\
		.id = MT6358_ID_##_name,				\
		.owner = THIS_MODULE,					\
		.n_voltages = ARRAY_SIZE(_volt_table),			\
		.volt_table = _volt_table,				\
		.vsel_reg = _vsel_reg,					\
		.vsel_mask = _vsel_mask,				\
		.enable_reg = _enable_reg,				\
		.enable_mask = BIT(0),					\
	},								\
	.constraints = {						\
		.valid_ops_mask = (mode),				\
	},								\
	.da_reg = _da_reg,						\
	.qi = BIT(15),							\
}

#define MT_REG_FIXED(match, _name, _enable_reg,				\
		     _da_reg,						\
		     _fixed_volt, mode)					\
[MT6358_ID_##_name] = {							\
	.desc = {							\
		.name = #_name,						\
		.of_match = of_match_ptr(match),			\
		.ops = &mt6358_volt_fixed_ops,				\
		.type = REGULATOR_VOLTAGE,				\
		.id = MT6358_ID_##_name,				\
		.owner = THIS_MODULE,					\
		.n_voltages = 1,					\
		.enable_reg = _enable_reg,				\
		.enable_mask = BIT(0),					\
		.fixed_uV = (_fixed_volt),				\
	},								\
	.constraints = {						\
		.valid_ops_mask = (mode),				\
	},								\
	.da_reg = _da_reg,						\
	.qi = BIT(15),							\
}

#define MT6358_LDO_VMC_DESC(match, _name, volt_ranges,	\
	_enable_reg, _da_reg, _vsel_reg, _vsel_mask, mode)\
[MT6358_ID_##_name] = {					\
	.desc = {					\
		.name = #_name,				\
		.of_match = of_match_ptr(match),	\
		.ops = &mt6358_vmc_ops,			\
		.type = REGULATOR_VOLTAGE,		\
		.id = MT6358_ID_##_name,		\
		.owner = THIS_MODULE,			\
		.n_voltages = 0xd01,			\
		.linear_ranges = volt_ranges,		\
		.n_linear_ranges = ARRAY_SIZE(volt_ranges),\
		.vsel_reg = _vsel_reg,			\
		.vsel_mask = _vsel_mask,		\
		.enable_reg = _enable_reg,		\
		.enable_mask = BIT(0),			\
	},						\
	.constraints = {				\
		.valid_ops_mask = (mode),		\
	},						\
	.da_reg = _da_reg,				\
	.qi = BIT(15),					\
}

static const struct regulator_linear_range mt_volt_range1[] = {
	REGULATOR_LINEAR_RANGE(500000, 0, 0x7f, 6250),
};

static const struct regulator_linear_range mt_volt_range2[] = {
	REGULATOR_LINEAR_RANGE(500000, 0, 0x7f, 12500),
};

static const struct regulator_linear_range mt_volt_range3[] = {
	REGULATOR_LINEAR_RANGE(500000, 0, 0x3f, 50000),
};

static const struct regulator_linear_range mt_volt_range4[] = {
	REGULATOR_LINEAR_RANGE(1000000, 0, 0x7f, 12500),
};

/* for vmc voltage calibration: 1.86V */
static const struct regulator_linear_range mt_volt_range5[] = {
	REGULATOR_LINEAR_RANGE(1800000, 0x400, 0x400, 0),
	REGULATOR_LINEAR_RANGE(1810000, 0x401, 0x40a, 10000),
	REGULATOR_LINEAR_RANGE(2900000, 0xa00, 0xa00, 0),
	REGULATOR_LINEAR_RANGE(2910000, 0xa01, 0xa09, 10000),
	REGULATOR_LINEAR_RANGE(3000000, 0xb00, 0xb00, 0),
	REGULATOR_LINEAR_RANGE(3010000, 0xb01, 0xb0a, 10000),
	REGULATOR_LINEAR_RANGE(3300000, 0xd00, 0xd00, 0),
};

static const u32 vdram2_voltages[] = {
	600000,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1800000,
};

static const u32 vsim1_voltages[] = {
	0,
	0,
	0,
	1700000,
	1800000,
	0,
	0,
	0,
	2700000,
	0,
	0,
	3000000,
	3100000,
};

static const u32 vibr_voltages[] = {
	1200000,
	1300000,
	1500000,
	0,
	1800000,
	2000000,
	0,
	0,
	0,
	2800000,
	0,
	3000000,
	0,
	3300000,
};

static const u32 vrf12_voltages[] = {
	1200000,
};

static const u32 vio18_voltages[] = {
	1800000,
};

static const u32 vusb_voltages[] = {
	0,
	0,
	0,
	3000000,
	3100000,
};

#if !USE_PMIC_MT6366
static const u32 vcamio_voltages[] = {
	1800000,
};

static const u32 vcamd_voltages[] = {
	0,
	0,
	0,
	900000,
	1000000,
	1100000,
	1200000,
	1300000,
	0,
	1500000,
	0,
	0,
	1800000,
};
#endif

static const u32 vcn18_voltages[] = {
	1800000,
};

static const u32 vfe28_voltages[] = {
	2800000,
};

static const u32 vcn28_voltages[] = {
	2800000,
};

static const u32 vxo22_voltages[] = {
	2200000,
};

static const u32 vefuse_voltages[] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1700000,
	1800000,
	1900000,
};

static const u32 vaux18_voltages[] = {
	1800000,
};

static const u32 vmch_voltages[] = {
	0,
	0,
	2900000,
	3000000,
	0,
	3300000,
};

static const u32 vbif28_voltages[] = {
	2800000,
};

#if !USE_PMIC_MT6366
static const u32 vcama1_voltages[] = {
	1800000,
	0,
	0,
	0,
	0,
	0,
	0,
	2500000,
	0,
	2700000,
	2800000,
	2900000,
	3000000,
};
#endif

static const u32 vemc_voltages[] = {
	0,
	0,
	2900000,
	3000000,
	0,
	3300000,
};

static const u32 vio28_voltages[] = {
	2800000,
};

static const u32 va12_voltages[] = {
	1200000,
};

static const u32 vrf18_voltages[] = {
	1800000,
};

static const u32 vcn33_bt_voltages[] = {
	0,
	3300000,
	3400000,
	3500000,
};

static const u32 vcn33_wifi_voltages[] = {
	0,
	3300000,
	3400000,
	3500000,
};

#if !USE_PMIC_MT6366
static const u32 vcama2_voltages[] = {
	1800000,
	0,
	0,
	0,
	0,
	0,
	0,
	2500000,
	0,
	2700000,
	2800000,
	2900000,
	3000000,
};

static const u32 vldo28_voltages[] = {
	0,
	2800000,
	0,
	3000000,
};
#endif

static const u32 vaud28_voltages[] = {
	2800000,
};

static const u32 vsim2_voltages[] = {
	0,
	0,
	0,
	1700000,
	1800000,
	0,
	0,
	0,
	2700000,
	0,
	0,
	3000000,
	3100000,
};


static int mt6358_regulator_disable(struct regulator_dev *rdev)
{
	int ret = 0;

	if (rdev->use_count == 0) {
		dev_notice(&rdev->dev, "%s:%s should not be disable.(use_count=0)\n"
			, __func__
			, rdev->desc->name);
		ret = -1;
	} else {
		ret = regulator_disable_regmap(rdev);
	}
=======
 * @qi: Mask for query enable signal status of regulators
 */
struct mt6358_regulator_info {
	struct regulator_desc desc;
	u32 status_reg;
	u32 qi;
	const u32 *index_table;
	unsigned int n_table;
	u32 da_vsel_reg;
	u32 da_vsel_mask;
	u32 modeset_reg;
	u32 modeset_mask;
};

#define MT6358_BUCK(match, vreg, min, max, step,		\
	volt_ranges, vosel_mask, _da_vsel_reg, _da_vsel_mask,	\
	_modeset_reg, _modeset_shift)		\
[MT6358_ID_##vreg] = {	\
	.desc = {	\
		.name = #vreg,	\
		.of_match = of_match_ptr(match),	\
		.ops = &mt6358_volt_range_ops,	\
		.type = REGULATOR_VOLTAGE,	\
		.id = MT6358_ID_##vreg,		\
		.owner = THIS_MODULE,		\
		.n_voltages = ((max) - (min)) / (step) + 1,	\
		.linear_ranges = volt_ranges,		\
		.n_linear_ranges = ARRAY_SIZE(volt_ranges),	\
		.vsel_reg = MT6358_BUCK_##vreg##_ELR0,	\
		.vsel_mask = vosel_mask,	\
		.enable_reg = MT6358_BUCK_##vreg##_CON0,	\
		.enable_mask = BIT(0),	\
		.of_map_mode = mt6358_map_mode,	\
	},	\
	.status_reg = MT6358_BUCK_##vreg##_DBG1,	\
	.qi = BIT(0),	\
	.da_vsel_reg = _da_vsel_reg,	\
	.da_vsel_mask = _da_vsel_mask,	\
	.modeset_reg = _modeset_reg,	\
	.modeset_mask = BIT(_modeset_shift),	\
}

#define MT6358_LDO(match, vreg, ldo_volt_table,	\
	ldo_index_table, enreg, enbit, vosel,	\
	vosel_mask)	\
[MT6358_ID_##vreg] = {	\
	.desc = {	\
		.name = #vreg,	\
		.of_match = of_match_ptr(match),	\
		.ops = &mt6358_volt_table_ops,	\
		.type = REGULATOR_VOLTAGE,	\
		.id = MT6358_ID_##vreg,	\
		.owner = THIS_MODULE,	\
		.n_voltages = ARRAY_SIZE(ldo_volt_table),	\
		.volt_table = ldo_volt_table,	\
		.vsel_reg = vosel,	\
		.vsel_mask = vosel_mask,	\
		.enable_reg = enreg,	\
		.enable_mask = BIT(enbit),	\
	},	\
	.status_reg = MT6358_LDO_##vreg##_CON1,	\
	.qi = BIT(15),	\
	.index_table = ldo_index_table,	\
	.n_table = ARRAY_SIZE(ldo_index_table),	\
}

#define MT6358_LDO1(match, vreg, min, max, step,	\
	volt_ranges, _da_vsel_reg, _da_vsel_mask,	\
	vosel, vosel_mask)	\
[MT6358_ID_##vreg] = {	\
	.desc = {	\
		.name = #vreg,	\
		.of_match = of_match_ptr(match),	\
		.ops = &mt6358_volt_range_ops,	\
		.type = REGULATOR_VOLTAGE,	\
		.id = MT6358_ID_##vreg,	\
		.owner = THIS_MODULE,	\
		.n_voltages = ((max) - (min)) / (step) + 1,	\
		.linear_ranges = volt_ranges,	\
		.n_linear_ranges = ARRAY_SIZE(volt_ranges),	\
		.vsel_reg = vosel,	\
		.vsel_mask = vosel_mask,	\
		.enable_reg = MT6358_LDO_##vreg##_CON0,	\
		.enable_mask = BIT(0),	\
	},	\
	.da_vsel_reg = _da_vsel_reg,	\
	.da_vsel_mask = _da_vsel_mask,	\
	.status_reg = MT6358_LDO_##vreg##_DBG1,	\
	.qi = BIT(0),	\
}

#define MT6358_REG_FIXED(match, vreg,	\
	enreg, enbit, volt)	\
[MT6358_ID_##vreg] = {	\
	.desc = {	\
		.name = #vreg,	\
		.of_match = of_match_ptr(match),	\
		.ops = &mt6358_volt_fixed_ops,	\
		.type = REGULATOR_VOLTAGE,	\
		.id = MT6358_ID_##vreg,	\
		.owner = THIS_MODULE,	\
		.n_voltages = 1,	\
		.enable_reg = enreg,	\
		.enable_mask = BIT(enbit),	\
		.min_uV = volt,	\
	},	\
	.status_reg = MT6358_LDO_##vreg##_CON1,	\
	.qi = BIT(15),							\
}

static const struct linear_range buck_volt_range1[] = {
	REGULATOR_LINEAR_RANGE(500000, 0, 0x7f, 6250),
};

static const struct linear_range buck_volt_range2[] = {
	REGULATOR_LINEAR_RANGE(500000, 0, 0x7f, 12500),
};

static const struct linear_range buck_volt_range3[] = {
	REGULATOR_LINEAR_RANGE(500000, 0, 0x3f, 50000),
};

static const struct linear_range buck_volt_range4[] = {
	REGULATOR_LINEAR_RANGE(1000000, 0, 0x7f, 12500),
};

static const unsigned int vdram2_voltages[] = {
	600000, 1800000,
};

static const unsigned int vsim_voltages[] = {
	1700000, 1800000, 2700000, 3000000, 3100000,
};

static const unsigned int vibr_voltages[] = {
	1200000, 1300000, 1500000, 1800000,
	2000000, 2800000, 3000000, 3300000,
};

static const unsigned int vusb_voltages[] = {
	3000000, 3100000,
};

static const unsigned int vcamd_voltages[] = {
	900000, 1000000, 1100000, 1200000,
	1300000, 1500000, 1800000,
};

static const unsigned int vefuse_voltages[] = {
	1700000, 1800000, 1900000,
};

static const unsigned int vmch_vemc_voltages[] = {
	2900000, 3000000, 3300000,
};

static const unsigned int vcama_voltages[] = {
	1800000, 2500000, 2700000,
	2800000, 2900000, 3000000,
};

static const unsigned int vcn33_bt_wifi_voltages[] = {
	3300000, 3400000, 3500000,
};

static const unsigned int vmc_voltages[] = {
	1800000, 2900000, 3000000, 3300000,
};

static const unsigned int vldo28_voltages[] = {
	2800000, 3000000,
};

static const u32 vdram2_idx[] = {
	0, 12,
};

static const u32 vsim_idx[] = {
	3, 4, 8, 11, 12,
};

static const u32 vibr_idx[] = {
	0, 1, 2, 4, 5, 9, 11, 13,
};

static const u32 vusb_idx[] = {
	3, 4,
};

static const u32 vcamd_idx[] = {
	3, 4, 5, 6, 7, 9, 12,
};

static const u32 vefuse_idx[] = {
	11, 12, 13,
};

static const u32 vmch_vemc_idx[] = {
	2, 3, 5,
};

static const u32 vcama_idx[] = {
	0, 7, 9, 10, 11, 12,
};

static const u32 vcn33_bt_wifi_idx[] = {
	1, 2, 3,
};

static const u32 vmc_idx[] = {
	4, 10, 11, 13,
};

static const u32 vldo28_idx[] = {
	1, 3,
};

static unsigned int mt6358_map_mode(unsigned int mode)
{
	return mode == MT6358_BUCK_MODE_AUTO ?
		REGULATOR_MODE_NORMAL : REGULATOR_MODE_FAST;
}

static int mt6358_set_voltage_sel(struct regulator_dev *rdev,
				  unsigned int selector)
{
	int idx, ret;
	const u32 *pvol;
	struct mt6358_regulator_info *info = rdev_get_drvdata(rdev);

	pvol = info->index_table;

	idx = pvol[selector];
	idx <<= ffs(info->desc.vsel_mask) - 1;
	ret = regmap_update_bits(rdev->regmap, info->desc.vsel_reg,
				 info->desc.vsel_mask, idx);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static int mt6358_regulator_get_voltage_sel(struct regulator_dev *rdev)
{
	struct mt6358_regulator_info *info = rdev_get_drvdata(rdev);
	int ret, regval = 0;

	ret = regmap_read(rdev->regmap, info->da_vsel_reg, &regval);
	if (ret != 0) {
		dev_notice(&rdev->dev,
			"Failed to get mt6358 regulator voltage: %d\n", ret);
		return ret;
	}

	ret = (regval >> info->da_vsel_shift) & info->da_vsel_mask;
=======
static int mt6358_get_voltage_sel(struct regulator_dev *rdev)
{
	int idx, ret;
	u32 selector;
	struct mt6358_regulator_info *info = rdev_get_drvdata(rdev);
	const u32 *pvol;

	ret = regmap_read(rdev->regmap, info->desc.vsel_reg, &selector);
	if (ret != 0) {
		dev_info(&rdev->dev,
			 "Failed to get mt6358 %s vsel reg: %d\n",
			 info->desc.name, ret);
		return ret;
	}

	selector = (selector & info->desc.vsel_mask) >>
			(ffs(info->desc.vsel_mask) - 1);
	pvol = info->index_table;
	for (idx = 0; idx < info->desc.n_voltages; idx++) {
		if (pvol[idx] == selector)
			return idx;
	}

	return -EINVAL;
}

static int mt6358_get_buck_voltage_sel(struct regulator_dev *rdev)
{
	int ret, regval;
	struct mt6358_regulator_info *info = rdev_get_drvdata(rdev);

	ret = regmap_read(rdev->regmap, info->da_vsel_reg, &regval);
	if (ret != 0) {
		dev_err(&rdev->dev,
			"Failed to get mt6358 Buck %s vsel reg: %d\n",
			info->desc.name, ret);
		return ret;
	}

	ret = (regval & info->da_vsel_mask) >> (ffs(info->da_vsel_mask) - 1);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
=======
static int mt6358_get_status(struct regulator_dev *rdev)
{
	int ret;
	u32 regval;
	struct mt6358_regulator_info *info = rdev_get_drvdata(rdev);

	ret = regmap_read(rdev->regmap, info->status_reg, &regval);
	if (ret != 0) {
		dev_info(&rdev->dev, "Failed to get enable reg: %d\n", ret);
		return ret;
	}

	return (regval & info->qi) ? REGULATOR_STATUS_ON : REGULATOR_STATUS_OFF;
}

>>>>>>> upstream/android-13
static int mt6358_regulator_set_mode(struct regulator_dev *rdev,
				     unsigned int mode)
{
	struct mt6358_regulator_info *info = rdev_get_drvdata(rdev);
<<<<<<< HEAD
	int ret, val;
=======
	int val;
>>>>>>> upstream/android-13

	switch (mode) {
	case REGULATOR_MODE_FAST:
		val = MT6358_BUCK_MODE_FORCE_PWM;
		break;
	case REGULATOR_MODE_NORMAL:
		val = MT6358_BUCK_MODE_AUTO;
		break;
	default:
<<<<<<< HEAD
		ret = -EINVAL;
		goto err_mode;
	}

	dev_notice(&rdev->dev, "mt6358 buck set_mode %#x, %#x, %#x, %#x\n",
		info->modeset_reg, info->modeset_mask,
		info->modeset_shift, val);

	val <<= info->modeset_shift;
	ret = regmap_update_bits(rdev->regmap, info->modeset_reg,
				 info->modeset_mask, val);
err_mode:
	if (ret != 0) {
		dev_notice(&rdev->dev,
			"Failed to set mt6358 buck mode: %d\n", ret);
		return ret;
	}

	return 0;
=======
		return -EINVAL;
	}

	dev_dbg(&rdev->dev, "mt6358 buck set_mode %#x, %#x, %#x\n",
		info->modeset_reg, info->modeset_mask, val);

	val <<= ffs(info->modeset_mask) - 1;

	return regmap_update_bits(rdev->regmap, info->modeset_reg,
				  info->modeset_mask, val);
>>>>>>> upstream/android-13
}

static unsigned int mt6358_regulator_get_mode(struct regulator_dev *rdev)
{
	struct mt6358_regulator_info *info = rdev_get_drvdata(rdev);
<<<<<<< HEAD
	int ret, regval = 0;

	ret = regmap_read(rdev->regmap, info->modeset_reg, &regval);
	if (ret != 0) {
		dev_notice(&rdev->dev,
=======
	int ret, regval;

	ret = regmap_read(rdev->regmap, info->modeset_reg, &regval);
	if (ret != 0) {
		dev_err(&rdev->dev,
>>>>>>> upstream/android-13
			"Failed to get mt6358 buck mode: %d\n", ret);
		return ret;
	}

<<<<<<< HEAD
	switch ((regval & info->modeset_mask) >> info->modeset_shift) {
=======
	switch ((regval & info->modeset_mask) >> (ffs(info->modeset_mask) - 1)) {
>>>>>>> upstream/android-13
	case MT6358_BUCK_MODE_AUTO:
		return REGULATOR_MODE_NORMAL;
	case MT6358_BUCK_MODE_FORCE_PWM:
		return REGULATOR_MODE_FAST;
	default:
		return -EINVAL;
	}
}

<<<<<<< HEAD
static int mt6358_get_status(struct regulator_dev *rdev)
{
	int ret;
	u32 regval = 0;
	struct mt6358_regulator_info *info = rdev_get_drvdata(rdev);

	ret = regmap_read(rdev->regmap, info->da_reg, &regval);
	if (ret != 0) {
		dev_notice(&rdev->dev, "Failed to get enable reg: %d\n", ret);
		return ret;
	}

	return (regval & info->qi) ? REGULATOR_STATUS_ON : REGULATOR_STATUS_OFF;
}

/* Regulator EXT_PMIC2 enable */
static int pmic_regulator_ext2_enable(struct regulator_dev *rdev)
{
	unsigned int ext_en = 0, ext_sel = 0;
	unsigned int ext_en_mask = PMIC_RG_STRUP_EXT_PMIC_EN_MASK <<
				   PMIC_RG_STRUP_EXT_PMIC_EN_SHIFT;
	unsigned int ext_sel_mask = PMIC_RG_STRUP_EXT_PMIC_SEL_MASK <<
				    PMIC_RG_STRUP_EXT_PMIC_SEL_SHIFT;
	int ret = 0;

	dev_info(&rdev->dev, "regulator ext_pmic2 enable\n");
	ret = regmap_read(rdev->regmap,
			  PMIC_RG_STRUP_EXT_PMIC_EN_ADDR, &ext_en);
	if ((ext_en & 0x2) != 0x2) {
		ret = regmap_update_bits(rdev->regmap,
					 PMIC_RG_STRUP_EXT_PMIC_EN_ADDR,
					 ext_en_mask,
					 ext_en | 0x2);
	}
	ret = regmap_read(rdev->regmap,
			  PMIC_RG_STRUP_EXT_PMIC_SEL_ADDR, &ext_sel);
	if ((ext_sel & 0x20) != 0x20) {
		ret = regmap_update_bits(rdev->regmap,
					 PMIC_RG_STRUP_EXT_PMIC_SEL_ADDR,
					 ext_sel_mask,
					 ext_sel | 0x20);
	}
	return ret;
}

/* Regulator EXT_PMIC2 disable */
static int pmic_regulator_ext2_disable(struct regulator_dev *rdev)
{
	unsigned int ext_en = 0, ext_sel = 0;
	unsigned int ext_en_mask = PMIC_RG_STRUP_EXT_PMIC_EN_MASK <<
				   PMIC_RG_STRUP_EXT_PMIC_EN_SHIFT;
	unsigned int ext_sel_mask = PMIC_RG_STRUP_EXT_PMIC_SEL_MASK <<
				    PMIC_RG_STRUP_EXT_PMIC_SEL_SHIFT;
	int ret = 0;

	dev_info(&rdev->dev, "regulator ext_pmic2 disable\n");
	if (rdev->use_count == 0) {
		dev_notice(&rdev->dev, "%s:%s should not be disable.(use_count=0)\n"
			, __func__
			, rdev->desc->name);
		return -1;
	}
	ret = regmap_read(rdev->regmap,
			  PMIC_RG_STRUP_EXT_PMIC_SEL_ADDR, &ext_sel);
	if ((ext_sel & 0x20) != 0x20) {
		ret = regmap_update_bits(rdev->regmap,
					 PMIC_RG_STRUP_EXT_PMIC_SEL_ADDR,
					 ext_sel_mask,
					 ext_sel | 0x20);
	}
	ret = regmap_read(rdev->regmap,
			  PMIC_RG_STRUP_EXT_PMIC_EN_ADDR, &ext_en);
	if ((ext_en & 0x2) != 0x2) {
		ret = regmap_update_bits(rdev->regmap,
					 PMIC_RG_STRUP_EXT_PMIC_EN_ADDR,
					 ext_en_mask,
					 ext_en & ~0x2);
	}
	return ret;
}

=======
>>>>>>> upstream/android-13
static const struct regulator_ops mt6358_volt_range_ops = {
	.list_voltage = regulator_list_voltage_linear_range,
	.map_voltage = regulator_map_voltage_linear_range,
	.set_voltage_sel = regulator_set_voltage_sel_regmap,
<<<<<<< HEAD
	.get_voltage_sel = mt6358_regulator_get_voltage_sel,
	.set_voltage_time_sel = regulator_set_voltage_time_sel,
	.enable = regulator_enable_regmap,
	.disable = mt6358_regulator_disable,
=======
	.get_voltage_sel = mt6358_get_buck_voltage_sel,
	.set_voltage_time_sel = regulator_set_voltage_time_sel,
	.enable = regulator_enable_regmap,
	.disable = regulator_disable_regmap,
>>>>>>> upstream/android-13
	.is_enabled = regulator_is_enabled_regmap,
	.get_status = mt6358_get_status,
	.set_mode = mt6358_regulator_set_mode,
	.get_mode = mt6358_regulator_get_mode,
};

static const struct regulator_ops mt6358_volt_table_ops = {
	.list_voltage = regulator_list_voltage_table,
	.map_voltage = regulator_map_voltage_iterate,
<<<<<<< HEAD
	.set_voltage_sel = regulator_set_voltage_sel_regmap,
	.get_voltage_sel = regulator_get_voltage_sel_regmap,
	.set_voltage_time_sel = regulator_set_voltage_time_sel,
	.enable = regulator_enable_regmap,
	.disable = mt6358_regulator_disable,
=======
	.set_voltage_sel = mt6358_set_voltage_sel,
	.get_voltage_sel = mt6358_get_voltage_sel,
	.set_voltage_time_sel = regulator_set_voltage_time_sel,
	.enable = regulator_enable_regmap,
	.disable = regulator_disable_regmap,
>>>>>>> upstream/android-13
	.is_enabled = regulator_is_enabled_regmap,
	.get_status = mt6358_get_status,
};

static const struct regulator_ops mt6358_volt_fixed_ops = {
<<<<<<< HEAD
	.enable = regulator_enable_regmap,
	.disable = mt6358_regulator_disable,
=======
	.list_voltage = regulator_list_voltage_linear,
	.enable = regulator_enable_regmap,
	.disable = regulator_disable_regmap,
>>>>>>> upstream/android-13
	.is_enabled = regulator_is_enabled_regmap,
	.get_status = mt6358_get_status,
};

<<<<<<< HEAD
/* Regulator EXT_PMIC2 ops */
static struct regulator_ops pmic_regulator_ext2_ops = {
	.enable = pmic_regulator_ext2_enable,
	.disable = pmic_regulator_ext2_disable,
	.is_enabled = mt6358_get_status,
};

static const struct regulator_ops mt6358_vmc_ops = {
	.list_voltage = regulator_list_voltage_linear_range,
	.enable = regulator_enable_regmap,
	.disable = mt6358_regulator_disable,
	.is_enabled = regulator_is_enabled_regmap,
	.get_status = mt6358_get_status,
	.set_voltage_sel = regulator_set_voltage_sel_regmap,
	.get_voltage_sel = regulator_get_voltage_sel_regmap,
	.set_voltage_time_sel = regulator_set_voltage_time_sel,
};

/* The array is indexed by id(MT6358_ID_XXX) */
static struct mt6358_regulator_info mt6358_regulators[] = {
	MT_BUCK("buck_vdram1", VDRAM1, 500000, 2087500, 12500,
		0, mt_volt_range2, PMIC_RG_BUCK_VDRAM1_EN_ADDR,
		PMIC_DA_VDRAM1_EN_ADDR,
		PMIC_DA_VDRAM1_VOSEL_ADDR,
		PMIC_DA_VDRAM1_VOSEL_MASK,
		PMIC_DA_VDRAM1_VOSEL_SHIFT,
		PMIC_RG_BUCK_VDRAM1_VOSEL_ADDR,
		PMIC_RG_BUCK_VDRAM1_VOSEL_MASK <<
		PMIC_RG_BUCK_VDRAM1_VOSEL_SHIFT,
		MT_BUCK_VOL_EN,
		PMIC_RG_VDRAM1_FPWM_ADDR, PMIC_RG_VDRAM1_FPWM_SHIFT),
	MT_BUCK("buck_vcore", VCORE, 500000, 1293750, 6250,
		0, mt_volt_range1, PMIC_RG_BUCK_VCORE_EN_ADDR,
		PMIC_DA_VCORE_EN_ADDR,
		PMIC_DA_VCORE_VOSEL_ADDR,
		PMIC_DA_VCORE_VOSEL_MASK,
		PMIC_DA_VCORE_VOSEL_SHIFT,
		PMIC_RG_BUCK_VCORE_VOSEL_ADDR,
		PMIC_RG_BUCK_VCORE_VOSEL_MASK <<
		PMIC_RG_BUCK_VCORE_VOSEL_SHIFT,
		MT_BUCK_VOL_EN_MODE,
		PMIC_RG_VCORE_FPWM_ADDR, PMIC_RG_VCORE_FPWM_SHIFT),
	MT_BUCK("buck_vpa", VPA, 500000, 3650000, 50000,
		0, mt_volt_range3, PMIC_RG_BUCK_VPA_EN_ADDR,
		PMIC_DA_VPA_EN_ADDR,
		PMIC_DA_VPA_VOSEL_ADDR,
		PMIC_DA_VPA_VOSEL_MASK,
		PMIC_DA_VPA_VOSEL_SHIFT,
		PMIC_RG_BUCK_VPA_VOSEL_ADDR,
		PMIC_RG_BUCK_VPA_VOSEL_MASK <<
		PMIC_RG_BUCK_VPA_VOSEL_SHIFT,
		MT_BUCK_VOL_EN,
		PMIC_RG_VPA_MODESET_ADDR, PMIC_RG_VPA_MODESET_SHIFT),
	MT_BUCK("buck_vproc11", VPROC11, 500000, 1293750, 6250,
		0, mt_volt_range1, PMIC_RG_BUCK_VPROC11_EN_ADDR,
		PMIC_DA_VPROC11_EN_ADDR,
		PMIC_DA_VPROC11_VOSEL_ADDR,
		PMIC_DA_VPROC11_VOSEL_MASK,
		PMIC_DA_VPROC11_VOSEL_SHIFT,
		PMIC_RG_BUCK_VPROC11_VOSEL_ADDR,
		PMIC_RG_BUCK_VPROC11_VOSEL_MASK <<
		PMIC_RG_BUCK_VPROC11_VOSEL_SHIFT,
		MT_BUCK_VOL_EN_MODE,
		PMIC_RG_VPROC11_FPWM_ADDR, PMIC_RG_VPROC11_FPWM_SHIFT),
	MT_BUCK("buck_vproc12", VPROC12, 500000, 1293750, 6250,
		0, mt_volt_range1, PMIC_RG_BUCK_VPROC12_EN_ADDR,
		PMIC_DA_VPROC12_EN_ADDR,
		PMIC_DA_VPROC12_VOSEL_ADDR,
		PMIC_DA_VPROC12_VOSEL_MASK,
		PMIC_DA_VPROC12_VOSEL_SHIFT,
		PMIC_RG_BUCK_VPROC12_VOSEL_ADDR,
		PMIC_RG_BUCK_VPROC12_VOSEL_MASK <<
		PMIC_RG_BUCK_VPROC12_VOSEL_SHIFT,
		MT_BUCK_VOL_EN_MODE,
		PMIC_RG_VPROC12_FPWM_ADDR, PMIC_RG_VPROC12_FPWM_SHIFT),
	MT_BUCK("buck_vgpu", VGPU, 500000, 1293750, 6250,
		0, mt_volt_range1, PMIC_RG_BUCK_VGPU_EN_ADDR,
		PMIC_DA_VGPU_EN_ADDR,
		PMIC_DA_VGPU_VOSEL_ADDR,
		PMIC_DA_VGPU_VOSEL_MASK,
		PMIC_DA_VGPU_VOSEL_SHIFT,
		PMIC_RG_BUCK_VGPU_VOSEL_ADDR,
		PMIC_RG_BUCK_VGPU_VOSEL_MASK <<
		PMIC_RG_BUCK_VGPU_VOSEL_SHIFT,
		MT_BUCK_VOL_EN_MODE,
		PMIC_RG_VGPU_FPWM_ADDR, PMIC_RG_VGPU_FPWM_SHIFT),
	MT_BUCK("buck_vs2", VS2, 500000, 2087500, 12500,
		0, mt_volt_range2, PMIC_RG_BUCK_VS2_EN_ADDR,
		PMIC_DA_VS2_EN_ADDR,
		PMIC_DA_VS2_VOSEL_ADDR,
		PMIC_DA_VS2_VOSEL_MASK,
		PMIC_DA_VS2_VOSEL_SHIFT,
		PMIC_RG_BUCK_VS2_VOSEL_ADDR,
		PMIC_RG_BUCK_VS2_VOSEL_MASK <<
		PMIC_RG_BUCK_VS2_VOSEL_SHIFT,
		MT_BUCK_VOL_EN,
		PMIC_RG_VS2_FPWM_ADDR, PMIC_RG_VS2_FPWM_SHIFT),
	MT_BUCK("buck_vmodem", VMODEM, 500000, 1293750, 6250,
		0, mt_volt_range1, PMIC_RG_BUCK_VMODEM_EN_ADDR,
		PMIC_DA_VMODEM_EN_ADDR,
		PMIC_DA_VMODEM_VOSEL_ADDR,
		PMIC_DA_VMODEM_VOSEL_MASK,
		PMIC_DA_VMODEM_VOSEL_SHIFT,
		PMIC_RG_BUCK_VMODEM_VOSEL_ADDR,
		PMIC_RG_BUCK_VMODEM_VOSEL_MASK <<
		PMIC_RG_BUCK_VMODEM_VOSEL_SHIFT,
		MT_BUCK_VOL_EN,
		PMIC_RG_VMODEM_FPWM_ADDR, PMIC_RG_VMODEM_FPWM_SHIFT),
	MT_BUCK("buck_vs1", VS1, 1000000, 2587500, 12500,
		0, mt_volt_range4, PMIC_RG_BUCK_VS1_EN_ADDR,
		PMIC_DA_VS1_EN_ADDR,
		PMIC_DA_VS1_VOSEL_ADDR,
		PMIC_DA_VS1_VOSEL_MASK,
		PMIC_DA_VS1_VOSEL_SHIFT,
		PMIC_RG_BUCK_VS1_VOSEL_ADDR,
		PMIC_RG_BUCK_VS1_VOSEL_MASK <<
		PMIC_RG_BUCK_VS1_VOSEL_SHIFT,
		MT_BUCK_VOL_EN,
		PMIC_RG_VS1_FPWM_ADDR, PMIC_RG_VS1_FPWM_SHIFT),

	MT_LDO_NON_REGULAR("ldo_vdram2", VDRAM2,
		vdram2_voltages, PMIC_RG_LDO_VDRAM2_EN_ADDR,
		PMIC_DA_VDRAM2_EN_ADDR,
		PMIC_RG_LDO_VDRAM2_VOSEL_0_ADDR,
		PMIC_RG_LDO_VDRAM2_VOSEL_0_MASK <<
		PMIC_RG_LDO_VDRAM2_VOSEL_0_SHIFT,
		MT_LDO_VOL_EN),
	MT_LDO_NON_REGULAR("ldo_vsim1", VSIM1,
		vsim1_voltages, PMIC_RG_LDO_VSIM1_EN_ADDR,
		PMIC_DA_VSIM1_EN_ADDR,
		PMIC_RG_VSIM1_VOSEL_ADDR,
		PMIC_RG_VSIM1_VOSEL_MASK <<
		PMIC_RG_VSIM1_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
	MT_LDO_NON_REGULAR("ldo_vibr", VIBR,
		vibr_voltages, PMIC_RG_LDO_VIBR_EN_ADDR,
		PMIC_DA_VIBR_EN_ADDR,
		PMIC_RG_VIBR_VOSEL_ADDR,
		PMIC_RG_VIBR_VOSEL_MASK <<
		PMIC_RG_VIBR_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
	MT_REG_FIXED("ldo_vrf12", VRF12, PMIC_RG_LDO_VRF12_EN_ADDR,
		PMIC_DA_VRF12_EN_ADDR,
		1200000, MT_LDO_EN),
	MT_REG_FIXED("ldo_vio18", VIO18, PMIC_RG_LDO_VIO18_EN_ADDR,
		PMIC_DA_VIO18_EN_ADDR,
		1800000, MT_LDO_EN),
	MT_LDO_NON_REGULAR("ldo_vusb", VUSB,
		vusb_voltages, PMIC_RG_LDO_VUSB_EN_0_ADDR,
		PMIC_DA_VUSB_EN_ADDR,
		PMIC_RG_VUSB_VOSEL_ADDR,
		PMIC_RG_VUSB_VOSEL_MASK <<
		PMIC_RG_VUSB_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
#if !USE_PMIC_MT6366
	MT_REG_FIXED("ldo_vcamio", VCAMIO, PMIC_RG_LDO_VCAMIO_EN_ADDR,
		PMIC_DA_VCAMIO_EN_ADDR,
		1800000, MT_LDO_EN),
	MT_LDO_NON_REGULAR("ldo_vcamd", VCAMD,
		vcamd_voltages, PMIC_RG_LDO_VCAMD_EN_ADDR,
		PMIC_DA_VCAMD_EN_ADDR,
		PMIC_RG_VCAMD_VOSEL_ADDR,
		PMIC_RG_VCAMD_VOSEL_MASK <<
		PMIC_RG_VCAMD_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
#endif
	MT_REG_FIXED("ldo_vcn18", VCN18, PMIC_RG_LDO_VCN18_EN_ADDR,
		PMIC_DA_VCN18_EN_ADDR,
		1800000, MT_LDO_EN),
	MT_REG_FIXED("ldo_vfe28", VFE28, PMIC_RG_LDO_VFE28_EN_ADDR,
		PMIC_DA_VFE28_EN_ADDR,
		2800000, MT_LDO_EN),
	MT_LDO_REGULAR("ldo_vsram_proc11", VSRAM_PROC11, 500000, 1293750, 6250,
		0, mt_volt_range1, PMIC_RG_LDO_VSRAM_PROC11_EN_ADDR,
		PMIC_DA_VSRAM_PROC11_EN_ADDR,
		PMIC_DA_VSRAM_PROC11_VOSEL_ADDR,
		PMIC_DA_VSRAM_PROC11_VOSEL_MASK,
		PMIC_DA_VSRAM_PROC11_VOSEL_SHIFT,
		PMIC_RG_LDO_VSRAM_PROC11_VOSEL_ADDR,
		PMIC_RG_LDO_VSRAM_PROC11_VOSEL_MASK <<
		PMIC_RG_LDO_VSRAM_PROC11_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
	MT_REG_FIXED("ldo_vcn28", VCN28, PMIC_RG_LDO_VCN28_EN_ADDR,
		PMIC_DA_VCN28_EN_ADDR,
		2800000, MT_LDO_EN),
	MT_LDO_REGULAR("ldo_vsram_others", VSRAM_OTHERS, 500000, 1293750, 6250,
		0, mt_volt_range1, PMIC_RG_LDO_VSRAM_OTHERS_EN_ADDR,
		PMIC_DA_VSRAM_OTHERS_EN_ADDR,
		PMIC_DA_VSRAM_OTHERS_VOSEL_ADDR,
		PMIC_DA_VSRAM_OTHERS_VOSEL_MASK,
		PMIC_DA_VSRAM_OTHERS_VOSEL_SHIFT,
		PMIC_RG_LDO_VSRAM_OTHERS_VOSEL_ADDR,
		PMIC_RG_LDO_VSRAM_OTHERS_VOSEL_MASK <<
		PMIC_RG_LDO_VSRAM_OTHERS_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
	MT_LDO_REGULAR("ldo_vsram_gpu", VSRAM_GPU, 500000, 1293750, 6250,
		0, mt_volt_range1, PMIC_RG_LDO_VSRAM_GPU_EN_ADDR,
		PMIC_DA_VSRAM_GPU_EN_ADDR,
		PMIC_DA_VSRAM_GPU_VOSEL_ADDR,
		PMIC_DA_VSRAM_GPU_VOSEL_MASK,
		PMIC_DA_VSRAM_GPU_VOSEL_SHIFT,
		PMIC_RG_LDO_VSRAM_GPU_VOSEL_ADDR,
		PMIC_RG_LDO_VSRAM_GPU_VOSEL_MASK <<
		PMIC_RG_LDO_VSRAM_GPU_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
	MT_REG_FIXED("ldo_vxo22", VXO22, PMIC_RG_LDO_VXO22_EN_ADDR,
		PMIC_DA_VXO22_EN_ADDR,
		2200000, MT_LDO_EN),
	MT_LDO_NON_REGULAR("ldo_vefuse", VEFUSE,
		vefuse_voltages, PMIC_RG_LDO_VEFUSE_EN_ADDR,
		PMIC_DA_VEFUSE_EN_ADDR,
		PMIC_RG_VEFUSE_VOSEL_ADDR,
		PMIC_RG_VEFUSE_VOSEL_MASK <<
		PMIC_RG_VEFUSE_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
	MT_REG_FIXED("ldo_vaux18", VAUX18, PMIC_RG_LDO_VAUX18_EN_ADDR,
		PMIC_DA_VAUX18_EN_ADDR,
		1800000, MT_LDO_EN),
	MT_LDO_NON_REGULAR("ldo_vmch", VMCH,
		vmch_voltages, PMIC_RG_LDO_VMCH_EN_ADDR,
		PMIC_DA_VMCH_EN_ADDR,
		PMIC_RG_VMCH_VOSEL_ADDR,
		PMIC_RG_VMCH_VOSEL_MASK <<
		PMIC_RG_VMCH_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
	MT_REG_FIXED("ldo_vbif28", VBIF28, PMIC_RG_LDO_VBIF28_EN_ADDR,
		PMIC_DA_VBIF28_EN_ADDR,
		2800000, MT_LDO_EN),
	MT_LDO_REGULAR("ldo_vsram_proc12", VSRAM_PROC12, 500000, 1293750, 6250,
		0, mt_volt_range1, PMIC_RG_LDO_VSRAM_PROC12_EN_ADDR,
		PMIC_DA_VSRAM_PROC12_EN_ADDR,
		PMIC_DA_VSRAM_PROC12_VOSEL_ADDR,
		PMIC_DA_VSRAM_PROC12_VOSEL_MASK,
		PMIC_DA_VSRAM_PROC12_VOSEL_SHIFT,
		PMIC_RG_LDO_VSRAM_PROC12_VOSEL_ADDR,
		PMIC_RG_LDO_VSRAM_PROC12_VOSEL_MASK <<
		PMIC_RG_LDO_VSRAM_PROC12_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
#if !USE_PMIC_MT6366
	MT_LDO_NON_REGULAR("ldo_vcama1", VCAMA1,
		vcama1_voltages, PMIC_RG_LDO_VCAMA1_EN_ADDR,
		PMIC_DA_VCAMA1_EN_ADDR,
		PMIC_RG_VCAMA1_VOSEL_ADDR,
		PMIC_RG_VCAMA1_VOSEL_MASK <<
		PMIC_RG_VCAMA1_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
#endif
	MT_LDO_NON_REGULAR("ldo_vemc", VEMC,
		vemc_voltages, PMIC_RG_LDO_VEMC_EN_ADDR,
		PMIC_DA_VEMC_EN_ADDR,
		PMIC_RG_VEMC_VOSEL_ADDR,
		PMIC_RG_VEMC_VOSEL_MASK <<
		PMIC_RG_VEMC_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
	MT_REG_FIXED("ldo_vio28", VIO28, PMIC_RG_LDO_VIO28_EN_ADDR,
		PMIC_DA_VIO28_EN_ADDR,
		2800000, MT_LDO_EN),
	MT_REG_FIXED("ldo_va12", VA12, PMIC_RG_LDO_VA12_EN_ADDR,
		PMIC_DA_VA12_EN_ADDR,
		1200000, MT_LDO_EN),
	MT_REG_FIXED("ldo_vrf18", VRF18, PMIC_RG_LDO_VRF18_EN_ADDR,
		PMIC_DA_VRF18_EN_ADDR,
		1800000, MT_LDO_EN),
	MT_LDO_NON_REGULAR("ldo_vcn33_bt", VCN33_BT,
		vcn33_bt_voltages, PMIC_RG_LDO_VCN33_EN_0_ADDR,
		PMIC_DA_VCN33_EN_ADDR,
		PMIC_RG_VCN33_VOSEL_ADDR,
		PMIC_RG_VCN33_VOSEL_MASK <<
		PMIC_RG_VCN33_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
	MT_LDO_NON_REGULAR("ldo_vcn33_wifi", VCN33_WIFI,
		vcn33_wifi_voltages, PMIC_RG_LDO_VCN33_EN_1_ADDR,
		PMIC_DA_VCN33_EN_ADDR,
		PMIC_RG_VCN33_VOSEL_ADDR,
		PMIC_RG_VCN33_VOSEL_MASK <<
		PMIC_RG_VCN33_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
#if !USE_PMIC_MT6366
	MT_LDO_NON_REGULAR("ldo_vcama2", VCAMA2,
		vcama2_voltages, PMIC_RG_LDO_VCAMA2_EN_ADDR,
		PMIC_DA_VCAMA2_EN_ADDR,
		PMIC_RG_VCAMA2_VOSEL_ADDR,
		PMIC_RG_VCAMA2_VOSEL_MASK <<
		PMIC_RG_VCAMA2_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
#endif
	MT6358_LDO_VMC_DESC("ldo_vmc", VMC, mt_volt_range5,
			    PMIC_RG_LDO_VMC_EN_ADDR,
			    PMIC_DA_VMC_EN_ADDR, PMIC_RG_VMC_VOSEL_ADDR,
			    0xFFF, MT_LDO_VOL_EN),
#if !USE_PMIC_MT6366
	MT_LDO_NON_REGULAR("ldo_vldo28", VLDO28,
		vldo28_voltages, PMIC_RG_LDO_VLDO28_EN_0_ADDR,
		PMIC_DA_VLDO28_EN_ADDR,
		PMIC_RG_VLDO28_VOSEL_ADDR,
		PMIC_RG_VLDO28_VOSEL_MASK <<
		PMIC_RG_VLDO28_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
#endif
	MT_REG_FIXED("ldo_vaud28", VAUD28, PMIC_RG_LDO_VAUD28_EN_ADDR,
		PMIC_DA_VAUD28_EN_ADDR,
		2800000, MT_LDO_EN),
	MT_LDO_NON_REGULAR("ldo_vsim2", VSIM2,
		vsim2_voltages, PMIC_RG_LDO_VSIM2_EN_ADDR,
		PMIC_DA_VSIM2_EN_ADDR,
		PMIC_RG_VSIM2_VOSEL_ADDR,
		PMIC_RG_VSIM2_VOSEL_MASK <<
		PMIC_RG_VSIM2_VOSEL_SHIFT,
		MT_LDO_VOL_EN),
	{
		.desc = {
			.name = "VA09",
			.of_match = of_match_ptr("ldo_va09"),
			.ops = &pmic_regulator_ext2_ops,
			.type = REGULATOR_VOLTAGE,
			.id = MT6358_ID_VA09,
			.owner = THIS_MODULE,
			.n_voltages = 1,
			.fixed_uV = 900000,
		},
		.constraints = {
			.valid_ops_mask = MT_LDO_EN,
		},
		.da_reg = PMIC_DA_EXT_PMIC_EN2_ADDR,
		.qi = BIT(9),
	},
};

static const struct mt_regulator_init_data mt6358_regulator_init_data = {
	.id = MT6358_SWCID,
	.size = MT6358_MAX_REGULATOR,
	.regulator_info = &mt6358_regulators[0],
};
=======
/* The array is indexed by id(MT6358_ID_XXX) */
static struct mt6358_regulator_info mt6358_regulators[] = {
	MT6358_BUCK("buck_vdram1", VDRAM1, 500000, 2087500, 12500,
		    buck_volt_range2, 0x7f, MT6358_BUCK_VDRAM1_DBG0, 0x7f,
		    MT6358_VDRAM1_ANA_CON0, 8),
	MT6358_BUCK("buck_vcore", VCORE, 500000, 1293750, 6250,
		    buck_volt_range1, 0x7f, MT6358_BUCK_VCORE_DBG0, 0x7f,
		    MT6358_VCORE_VGPU_ANA_CON0, 1),
	MT6358_BUCK("buck_vpa", VPA, 500000, 3650000, 50000,
		    buck_volt_range3, 0x3f, MT6358_BUCK_VPA_DBG0, 0x3f,
		    MT6358_VPA_ANA_CON0, 3),
	MT6358_BUCK("buck_vproc11", VPROC11, 500000, 1293750, 6250,
		    buck_volt_range1, 0x7f, MT6358_BUCK_VPROC11_DBG0, 0x7f,
		    MT6358_VPROC_ANA_CON0, 1),
	MT6358_BUCK("buck_vproc12", VPROC12, 500000, 1293750, 6250,
		    buck_volt_range1, 0x7f, MT6358_BUCK_VPROC12_DBG0, 0x7f,
		    MT6358_VPROC_ANA_CON0, 2),
	MT6358_BUCK("buck_vgpu", VGPU, 500000, 1293750, 6250,
		    buck_volt_range1, 0x7f, MT6358_BUCK_VGPU_ELR0, 0x7f,
		    MT6358_VCORE_VGPU_ANA_CON0, 2),
	MT6358_BUCK("buck_vs2", VS2, 500000, 2087500, 12500,
		    buck_volt_range2, 0x7f, MT6358_BUCK_VS2_DBG0, 0x7f,
		    MT6358_VS2_ANA_CON0, 8),
	MT6358_BUCK("buck_vmodem", VMODEM, 500000, 1293750, 6250,
		    buck_volt_range1, 0x7f, MT6358_BUCK_VMODEM_DBG0, 0x7f,
		    MT6358_VMODEM_ANA_CON0, 8),
	MT6358_BUCK("buck_vs1", VS1, 1000000, 2587500, 12500,
		    buck_volt_range4, 0x7f, MT6358_BUCK_VS1_DBG0, 0x7f,
		    MT6358_VS1_ANA_CON0, 8),
	MT6358_REG_FIXED("ldo_vrf12", VRF12,
			 MT6358_LDO_VRF12_CON0, 0, 1200000),
	MT6358_REG_FIXED("ldo_vio18", VIO18,
			 MT6358_LDO_VIO18_CON0, 0, 1800000),
	MT6358_REG_FIXED("ldo_vcamio", VCAMIO,
			 MT6358_LDO_VCAMIO_CON0, 0, 1800000),
	MT6358_REG_FIXED("ldo_vcn18", VCN18, MT6358_LDO_VCN18_CON0, 0, 1800000),
	MT6358_REG_FIXED("ldo_vfe28", VFE28, MT6358_LDO_VFE28_CON0, 0, 2800000),
	MT6358_REG_FIXED("ldo_vcn28", VCN28, MT6358_LDO_VCN28_CON0, 0, 2800000),
	MT6358_REG_FIXED("ldo_vxo22", VXO22, MT6358_LDO_VXO22_CON0, 0, 2200000),
	MT6358_REG_FIXED("ldo_vaux18", VAUX18,
			 MT6358_LDO_VAUX18_CON0, 0, 1800000),
	MT6358_REG_FIXED("ldo_vbif28", VBIF28,
			 MT6358_LDO_VBIF28_CON0, 0, 2800000),
	MT6358_REG_FIXED("ldo_vio28", VIO28, MT6358_LDO_VIO28_CON0, 0, 2800000),
	MT6358_REG_FIXED("ldo_va12", VA12, MT6358_LDO_VA12_CON0, 0, 1200000),
	MT6358_REG_FIXED("ldo_vrf18", VRF18, MT6358_LDO_VRF18_CON0, 0, 1800000),
	MT6358_REG_FIXED("ldo_vaud28", VAUD28,
			 MT6358_LDO_VAUD28_CON0, 0, 2800000),
	MT6358_LDO("ldo_vdram2", VDRAM2, vdram2_voltages, vdram2_idx,
		   MT6358_LDO_VDRAM2_CON0, 0, MT6358_LDO_VDRAM2_ELR0, 0xf),
	MT6358_LDO("ldo_vsim1", VSIM1, vsim_voltages, vsim_idx,
		   MT6358_LDO_VSIM1_CON0, 0, MT6358_VSIM1_ANA_CON0, 0xf00),
	MT6358_LDO("ldo_vibr", VIBR, vibr_voltages, vibr_idx,
		   MT6358_LDO_VIBR_CON0, 0, MT6358_VIBR_ANA_CON0, 0xf00),
	MT6358_LDO("ldo_vusb", VUSB, vusb_voltages, vusb_idx,
		   MT6358_LDO_VUSB_CON0_0, 0, MT6358_VUSB_ANA_CON0, 0x700),
	MT6358_LDO("ldo_vcamd", VCAMD, vcamd_voltages, vcamd_idx,
		   MT6358_LDO_VCAMD_CON0, 0, MT6358_VCAMD_ANA_CON0, 0xf00),
	MT6358_LDO("ldo_vefuse", VEFUSE, vefuse_voltages, vefuse_idx,
		   MT6358_LDO_VEFUSE_CON0, 0, MT6358_VEFUSE_ANA_CON0, 0xf00),
	MT6358_LDO("ldo_vmch", VMCH, vmch_vemc_voltages, vmch_vemc_idx,
		   MT6358_LDO_VMCH_CON0, 0, MT6358_VMCH_ANA_CON0, 0x700),
	MT6358_LDO("ldo_vcama1", VCAMA1, vcama_voltages, vcama_idx,
		   MT6358_LDO_VCAMA1_CON0, 0, MT6358_VCAMA1_ANA_CON0, 0xf00),
	MT6358_LDO("ldo_vemc", VEMC, vmch_vemc_voltages, vmch_vemc_idx,
		   MT6358_LDO_VEMC_CON0, 0, MT6358_VEMC_ANA_CON0, 0x700),
	MT6358_LDO("ldo_vcn33_bt", VCN33_BT, vcn33_bt_wifi_voltages,
		   vcn33_bt_wifi_idx, MT6358_LDO_VCN33_CON0_0,
		   0, MT6358_VCN33_ANA_CON0, 0x300),
	MT6358_LDO("ldo_vcn33_wifi", VCN33_WIFI, vcn33_bt_wifi_voltages,
		   vcn33_bt_wifi_idx, MT6358_LDO_VCN33_CON0_1,
		   0, MT6358_VCN33_ANA_CON0, 0x300),
	MT6358_LDO("ldo_vcama2", VCAMA2, vcama_voltages, vcama_idx,
		   MT6358_LDO_VCAMA2_CON0, 0, MT6358_VCAMA2_ANA_CON0, 0xf00),
	MT6358_LDO("ldo_vmc", VMC, vmc_voltages, vmc_idx,
		   MT6358_LDO_VMC_CON0, 0, MT6358_VMC_ANA_CON0, 0xf00),
	MT6358_LDO("ldo_vldo28", VLDO28, vldo28_voltages, vldo28_idx,
		   MT6358_LDO_VLDO28_CON0_0, 0,
		   MT6358_VLDO28_ANA_CON0, 0x300),
	MT6358_LDO("ldo_vsim2", VSIM2, vsim_voltages, vsim_idx,
		   MT6358_LDO_VSIM2_CON0, 0, MT6358_VSIM2_ANA_CON0, 0xf00),
	MT6358_LDO1("ldo_vsram_proc11", VSRAM_PROC11, 500000, 1293750, 6250,
		    buck_volt_range1, MT6358_LDO_VSRAM_PROC11_DBG0, 0x7f00,
		    MT6358_LDO_VSRAM_CON0, 0x7f),
	MT6358_LDO1("ldo_vsram_others", VSRAM_OTHERS, 500000, 1293750, 6250,
		    buck_volt_range1, MT6358_LDO_VSRAM_OTHERS_DBG0, 0x7f00,
		    MT6358_LDO_VSRAM_CON2, 0x7f),
	MT6358_LDO1("ldo_vsram_gpu", VSRAM_GPU, 500000, 1293750, 6250,
		    buck_volt_range1, MT6358_LDO_VSRAM_GPU_DBG0, 0x7f00,
		    MT6358_LDO_VSRAM_CON3, 0x7f),
	MT6358_LDO1("ldo_vsram_proc12", VSRAM_PROC12, 500000, 1293750, 6250,
		    buck_volt_range1, MT6358_LDO_VSRAM_PROC12_DBG0, 0x7f00,
		    MT6358_LDO_VSRAM_CON1, 0x7f),
};

static int mt6358_regulator_probe(struct platform_device *pdev)
{
	struct mt6397_chip *mt6397 = dev_get_drvdata(pdev->dev.parent);
	struct regulator_config config = {};
	struct regulator_dev *rdev;
	int i;

	for (i = 0; i < MT6358_MAX_REGULATOR; i++) {
		config.dev = &pdev->dev;
		config.driver_data = &mt6358_regulators[i];
		config.regmap = mt6397->regmap;

		rdev = devm_regulator_register(&pdev->dev,
					       &mt6358_regulators[i].desc,
					       &config);
		if (IS_ERR(rdev)) {
			dev_err(&pdev->dev, "failed to register %s\n",
				mt6358_regulators[i].desc.name);
			return PTR_ERR(rdev);
		}
	}

	return 0;
}
>>>>>>> upstream/android-13

static const struct platform_device_id mt6358_platform_ids[] = {
	{"mt6358-regulator", 0},
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(platform, mt6358_platform_ids);

<<<<<<< HEAD
static const struct of_device_id mt6358_of_match[] = {
	{
		.compatible = "mediatek,mt6358-regulator",
		.data = &mt6358_regulator_init_data,
	}, {
		/* sentinel */
	},
};
MODULE_DEVICE_TABLE(of, mt6358_of_match);

static void mt6358_oc_irq_enable_work(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct mt6358_regulator_info *info
		= container_of(dwork, struct mt6358_regulator_info, oc_work);

	enable_irq(info->irq);
}

static irqreturn_t mt6358_oc_irq(int irq, void *data)
{
	struct regulator_dev *rdev = (struct regulator_dev *)data;
	struct mt6358_regulator_info *info = rdev_get_drvdata(rdev);

	disable_irq_nosync(info->irq);
	if (!regulator_is_enabled_regmap(rdev))
		goto delayed_enable;
	mutex_lock(&rdev->mutex);
	regulator_notifier_call_chain(rdev, REGULATOR_EVENT_OVER_CURRENT,
				      NULL);
	mutex_unlock(&rdev->mutex);
delayed_enable:
	schedule_delayed_work(&info->oc_work,
			      msecs_to_jiffies(info->oc_irq_enable_delay_ms));
	return IRQ_HANDLED;
}

static int mt6358_of_parse_cb(struct device_node *np,
			      const struct regulator_desc *desc,
			      struct regulator_config *config)
{
	int ret;
	struct mt6358_regulator_info *info = config->driver_data;

	ret = of_property_read_u32(np, "mediatek,oc-irq-enable-delay-ms",
				   &info->oc_irq_enable_delay_ms);
	if (ret || !info->oc_irq_enable_delay_ms)
		info->oc_irq_enable_delay_ms = DEF_OC_IRQ_ENABLE_DELAY_MS;

	return 0;
}

static int mt6358_regulator_probe(struct platform_device *pdev)
{
	const struct of_device_id *of_id;
	struct mt6358_chip *mt6358 = dev_get_drvdata(pdev->dev.parent);
	struct mt6358_regulator_info *mt_regulators;
	struct mt_regulator_init_data *regulator_init_data;
	struct regulator_config config = {};
	struct regulator_dev *rdev;
	struct regulation_constraints *c;
	int i, ret;
	u32 reg_value = 0;

	of_id = of_match_device(mt6358_of_match, &pdev->dev);
	if (!of_id || !of_id->data)
		return -ENODEV;
	regulator_init_data = (struct mt_regulator_init_data *)of_id->data;
	mt_regulators = regulator_init_data->regulator_info;

	/* Read PMIC chip revision to update constraints and voltage table */
	if (regmap_read(mt6358->regmap, MT6358_SWCID, &reg_value) < 0) {
		dev_notice(&pdev->dev, "Failed to read Chip ID\n");
		return -EIO;
	}
	dev_info(&pdev->dev, "Chip ID = 0x%x\n", reg_value);

	for (i = 0; i < regulator_init_data->size; i++, mt_regulators++) {
		mt_regulators->desc.of_parse_cb = mt6358_of_parse_cb;
		config.dev = &pdev->dev;
		config.driver_data = mt_regulators;
		config.regmap = mt6358->regmap;
		rdev = devm_regulator_register(&pdev->dev,
					       &mt_regulators->desc, &config);
		if (IS_ERR(rdev)) {
			dev_notice(&pdev->dev, "failed to register %s\n",
				   mt_regulators->desc.name);
			return PTR_ERR(rdev);
		}

		c = rdev->constraints;
		c->valid_ops_mask |=
			mt_regulators->constraints.valid_ops_mask;
		c->valid_modes_mask |=
			mt_regulators->constraints.valid_modes_mask;

		mt_regulators->irq =
			platform_get_irq_byname(pdev,
						mt_regulators->desc.name);
		if (mt_regulators->irq < 0)
			continue;
		ret = devm_request_threaded_irq(&pdev->dev, mt_regulators->irq,
						NULL, mt6358_oc_irq,
						IRQF_TRIGGER_HIGH,
						mt_regulators->desc.name,
						rdev);
		if (ret) {
			dev_notice(&pdev->dev, "Failed to request IRQ:%s,%d",
				   mt_regulators->desc.name, ret);
			continue;
		}
		INIT_DELAYED_WORK(&mt_regulators->oc_work,
				  mt6358_oc_irq_enable_work);
	}

	return 0;
}

static struct platform_driver mt6358_regulator_driver = {
	.driver = {
		.name = "mt6358-regulator",
		.of_match_table = of_match_ptr(mt6358_of_match),
=======
static struct platform_driver mt6358_regulator_driver = {
	.driver = {
		.name = "mt6358-regulator",
>>>>>>> upstream/android-13
	},
	.probe = mt6358_regulator_probe,
	.id_table = mt6358_platform_ids,
};

module_platform_driver(mt6358_regulator_driver);

<<<<<<< HEAD
MODULE_AUTHOR("Wen Su <wen.su@mediatek.com>");
MODULE_DESCRIPTION("Regulator Driver for MediaTek MT6358 PMIC");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:mt6358-regulator");
=======
MODULE_AUTHOR("Hsin-Hsiung Wang <hsin-hsiung.wang@mediatek.com>");
MODULE_DESCRIPTION("Regulator Driver for MediaTek MT6358 PMIC");
MODULE_LICENSE("GPL");
>>>>>>> upstream/android-13
