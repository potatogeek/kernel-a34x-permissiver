<<<<<<< HEAD
/*
 * Regulator device driver for DA9061 and DA9062.
 * Copyright (C) 2015-2017  Dialog Semiconductor
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
=======
// SPDX-License-Identifier: GPL-2.0+
//
// Regulator device driver for DA9061 and DA9062.
// Copyright (C) 2015-2017  Dialog Semiconductor

>>>>>>> upstream/android-13
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/of_regulator.h>
#include <linux/mfd/da9062/core.h>
#include <linux/mfd/da9062/registers.h>
<<<<<<< HEAD
=======
#include <dt-bindings/regulator/dlg,da9063-regulator.h>
>>>>>>> upstream/android-13

/* Regulator IDs */
enum {
	DA9061_ID_BUCK1,
	DA9061_ID_BUCK2,
	DA9061_ID_BUCK3,
	DA9061_ID_LDO1,
	DA9061_ID_LDO2,
	DA9061_ID_LDO3,
	DA9061_ID_LDO4,
	DA9061_MAX_REGULATORS,
};

enum {
	DA9062_ID_BUCK1,
	DA9062_ID_BUCK2,
	DA9062_ID_BUCK3,
	DA9062_ID_BUCK4,
	DA9062_ID_LDO1,
	DA9062_ID_LDO2,
	DA9062_ID_LDO3,
	DA9062_ID_LDO4,
	DA9062_MAX_REGULATORS,
};

/* Regulator capabilities and registers description */
struct da9062_regulator_info {
	struct regulator_desc desc;
<<<<<<< HEAD
	/* Current limiting */
	unsigned int n_current_limits;
	const int *current_limits;
=======
>>>>>>> upstream/android-13
	/* Main register fields */
	struct reg_field mode;
	struct reg_field suspend;
	struct reg_field sleep;
	struct reg_field suspend_sleep;
	unsigned int suspend_vsel_reg;
<<<<<<< HEAD
	struct reg_field ilimit;
=======
>>>>>>> upstream/android-13
	/* Event detection bit */
	struct reg_field oc_event;
};

/* Single regulator settings */
struct da9062_regulator {
	struct regulator_desc			desc;
	struct regulator_dev			*rdev;
	struct da9062				*hw;
	const struct da9062_regulator_info	*info;

	struct regmap_field			*mode;
	struct regmap_field			*suspend;
	struct regmap_field			*sleep;
	struct regmap_field			*suspend_sleep;
<<<<<<< HEAD
	struct regmap_field			*ilimit;
=======
>>>>>>> upstream/android-13
};

/* Encapsulates all information for the regulators driver */
struct da9062_regulators {
	int					irq_ldo_lim;
	unsigned				n_regulators;
	/* Array size to be defined during init. Keep at end. */
<<<<<<< HEAD
	struct da9062_regulator			regulator[0];
};

/* BUCK modes */
enum {
	BUCK_MODE_MANUAL,	/* 0 */
	BUCK_MODE_SLEEP,	/* 1 */
	BUCK_MODE_SYNC,		/* 2 */
	BUCK_MODE_AUTO		/* 3 */
=======
	struct da9062_regulator			regulator[];
>>>>>>> upstream/android-13
};

/* Regulator operations */

/* Current limits array (in uA)
 * - DA9061_ID_[BUCK1|BUCK3]
 * - DA9062_ID_[BUCK1|BUCK2|BUCK4]
 * Entry indexes corresponds to register values.
 */
<<<<<<< HEAD
static const int da9062_buck_a_limits[] = {
=======
static const unsigned int da9062_buck_a_limits[] = {
>>>>>>> upstream/android-13
	 500000,  600000,  700000,  800000,  900000, 1000000, 1100000, 1200000,
	1300000, 1400000, 1500000, 1600000, 1700000, 1800000, 1900000, 2000000
};

/* Current limits array (in uA)
 * - DA9061_ID_BUCK2
 * - DA9062_ID_BUCK3
 * Entry indexes corresponds to register values.
 */
<<<<<<< HEAD
static const int da9062_buck_b_limits[] = {
=======
static const unsigned int da9062_buck_b_limits[] = {
>>>>>>> upstream/android-13
	1500000, 1600000, 1700000, 1800000, 1900000, 2000000, 2100000, 2200000,
	2300000, 2400000, 2500000, 2600000, 2700000, 2800000, 2900000, 3000000
};

<<<<<<< HEAD
static int da9062_set_current_limit(struct regulator_dev *rdev,
				    int min_ua, int max_ua)
{
	struct da9062_regulator *regl = rdev_get_drvdata(rdev);
	const struct da9062_regulator_info *rinfo = regl->info;
	int n, tval;

	for (n = 0; n < rinfo->n_current_limits; n++) {
		tval = rinfo->current_limits[n];
		if (tval >= min_ua && tval <= max_ua)
			return regmap_field_write(regl->ilimit, n);
	}

	return -EINVAL;
}

static int da9062_get_current_limit(struct regulator_dev *rdev)
{
	struct da9062_regulator *regl = rdev_get_drvdata(rdev);
	const struct da9062_regulator_info *rinfo = regl->info;
	unsigned int sel;
	int ret;

	ret = regmap_field_read(regl->ilimit, &sel);
	if (ret < 0)
		return ret;

	if (sel >= rinfo->n_current_limits)
		sel = rinfo->n_current_limits - 1;

	return rinfo->current_limits[sel];
=======
static unsigned int da9062_map_buck_mode(unsigned int mode)
{
	switch (mode) {
	case DA9063_BUCK_MODE_SLEEP:
		return REGULATOR_MODE_STANDBY;
	case DA9063_BUCK_MODE_SYNC:
		return REGULATOR_MODE_FAST;
	case DA9063_BUCK_MODE_AUTO:
		return REGULATOR_MODE_NORMAL;
	default:
		return REGULATOR_MODE_INVALID;
	}
>>>>>>> upstream/android-13
}

static int da9062_buck_set_mode(struct regulator_dev *rdev, unsigned mode)
{
	struct da9062_regulator *regl = rdev_get_drvdata(rdev);
	unsigned val;

	switch (mode) {
	case REGULATOR_MODE_FAST:
<<<<<<< HEAD
		val = BUCK_MODE_SYNC;
		break;
	case REGULATOR_MODE_NORMAL:
		val = BUCK_MODE_AUTO;
		break;
	case REGULATOR_MODE_STANDBY:
		val = BUCK_MODE_SLEEP;
=======
		val = DA9063_BUCK_MODE_SYNC;
		break;
	case REGULATOR_MODE_NORMAL:
		val = DA9063_BUCK_MODE_AUTO;
		break;
	case REGULATOR_MODE_STANDBY:
		val = DA9063_BUCK_MODE_SLEEP;
>>>>>>> upstream/android-13
		break;
	default:
		return -EINVAL;
	}

	return regmap_field_write(regl->mode, val);
}

/*
 * Bucks use single mode register field for normal operation
 * and suspend state.
 * There are 3 modes to map to: FAST, NORMAL, and STANDBY.
 */

static unsigned da9062_buck_get_mode(struct regulator_dev *rdev)
{
	struct da9062_regulator *regl = rdev_get_drvdata(rdev);
<<<<<<< HEAD
	struct regmap_field *field;
	unsigned int val, mode = 0;
=======
	unsigned int val;
>>>>>>> upstream/android-13
	int ret;

	ret = regmap_field_read(regl->mode, &val);
	if (ret < 0)
		return ret;

	switch (val) {
	default:
<<<<<<< HEAD
	case BUCK_MODE_MANUAL:
		mode = REGULATOR_MODE_FAST | REGULATOR_MODE_STANDBY;
		/* Sleep flag bit decides the mode */
		break;
	case BUCK_MODE_SLEEP:
		return REGULATOR_MODE_STANDBY;
	case BUCK_MODE_SYNC:
		return REGULATOR_MODE_FAST;
	case BUCK_MODE_AUTO:
		return REGULATOR_MODE_NORMAL;
	}

	/* Detect current regulator state */
	ret = regmap_field_read(regl->suspend, &val);
	if (ret < 0)
		return 0;

	/* Read regulator mode from proper register, depending on state */
	if (val)
		field = regl->suspend_sleep;
	else
		field = regl->sleep;

	ret = regmap_field_read(field, &val);
=======
		/* Sleep flag bit decides the mode */
		break;
	case DA9063_BUCK_MODE_SLEEP:
		return REGULATOR_MODE_STANDBY;
	case DA9063_BUCK_MODE_SYNC:
		return REGULATOR_MODE_FAST;
	case DA9063_BUCK_MODE_AUTO:
		return REGULATOR_MODE_NORMAL;
	}

	ret = regmap_field_read(regl->sleep, &val);
>>>>>>> upstream/android-13
	if (ret < 0)
		return 0;

	if (val)
<<<<<<< HEAD
		mode &= REGULATOR_MODE_STANDBY;
	else
		mode &= REGULATOR_MODE_NORMAL | REGULATOR_MODE_FAST;

	return mode;
=======
		return REGULATOR_MODE_STANDBY;
	else
		return REGULATOR_MODE_FAST;
>>>>>>> upstream/android-13
}

/*
 * LDOs use sleep flags - one for normal and one for suspend state.
 * There are 2 modes to map to: NORMAL and STANDBY (sleep) for each state.
 */

static int da9062_ldo_set_mode(struct regulator_dev *rdev, unsigned mode)
{
	struct da9062_regulator *regl = rdev_get_drvdata(rdev);
	unsigned val;

	switch (mode) {
	case REGULATOR_MODE_NORMAL:
		val = 0;
		break;
	case REGULATOR_MODE_STANDBY:
		val = 1;
		break;
	default:
		return -EINVAL;
	}

	return regmap_field_write(regl->sleep, val);
}

static unsigned da9062_ldo_get_mode(struct regulator_dev *rdev)
{
	struct da9062_regulator *regl = rdev_get_drvdata(rdev);
<<<<<<< HEAD
	struct regmap_field *field;
	int ret, val;

	/* Detect current regulator state */
	ret = regmap_field_read(regl->suspend, &val);
	if (ret < 0)
		return 0;

	/* Read regulator mode from proper register, depending on state */
	if (val)
		field = regl->suspend_sleep;
	else
		field = regl->sleep;

	ret = regmap_field_read(field, &val);
=======
	int ret, val;

	ret = regmap_field_read(regl->sleep, &val);
>>>>>>> upstream/android-13
	if (ret < 0)
		return 0;

	if (val)
		return REGULATOR_MODE_STANDBY;
	else
		return REGULATOR_MODE_NORMAL;
}

static int da9062_buck_get_status(struct regulator_dev *rdev)
{
	int ret = regulator_is_enabled_regmap(rdev);

	if (ret == 0) {
		ret = REGULATOR_STATUS_OFF;
	} else if (ret > 0) {
		ret = da9062_buck_get_mode(rdev);
		if (ret > 0)
			ret = regulator_mode_to_status(ret);
		else if (ret == 0)
			ret = -EIO;
	}

	return ret;
}

static int da9062_ldo_get_status(struct regulator_dev *rdev)
{
	int ret = regulator_is_enabled_regmap(rdev);

	if (ret == 0) {
		ret = REGULATOR_STATUS_OFF;
	} else if (ret > 0) {
		ret = da9062_ldo_get_mode(rdev);
		if (ret > 0)
			ret = regulator_mode_to_status(ret);
		else if (ret == 0)
			ret = -EIO;
	}

	return ret;
}

static int da9062_set_suspend_voltage(struct regulator_dev *rdev, int uv)
{
	struct da9062_regulator *regl = rdev_get_drvdata(rdev);
	const struct da9062_regulator_info *rinfo = regl->info;
	int ret, sel;

	sel = regulator_map_voltage_linear(rdev, uv, uv);
	if (sel < 0)
		return sel;

	sel <<= ffs(rdev->desc->vsel_mask) - 1;

	ret = regmap_update_bits(regl->hw->regmap, rinfo->suspend_vsel_reg,
				 rdev->desc->vsel_mask, sel);

	return ret;
}

static int da9062_suspend_enable(struct regulator_dev *rdev)
{
	struct da9062_regulator *regl = rdev_get_drvdata(rdev);

	return regmap_field_write(regl->suspend, 1);
}

static int da9062_suspend_disable(struct regulator_dev *rdev)
{
	struct da9062_regulator *regl = rdev_get_drvdata(rdev);

	return regmap_field_write(regl->suspend, 0);
}

static int da9062_buck_set_suspend_mode(struct regulator_dev *rdev,
					unsigned mode)
{
	struct da9062_regulator *regl = rdev_get_drvdata(rdev);
	int val;

	switch (mode) {
	case REGULATOR_MODE_FAST:
<<<<<<< HEAD
		val = BUCK_MODE_SYNC;
		break;
	case REGULATOR_MODE_NORMAL:
		val = BUCK_MODE_AUTO;
		break;
	case REGULATOR_MODE_STANDBY:
		val = BUCK_MODE_SLEEP;
=======
		val = DA9063_BUCK_MODE_SYNC;
		break;
	case REGULATOR_MODE_NORMAL:
		val = DA9063_BUCK_MODE_AUTO;
		break;
	case REGULATOR_MODE_STANDBY:
		val = DA9063_BUCK_MODE_SLEEP;
>>>>>>> upstream/android-13
		break;
	default:
		return -EINVAL;
	}

	return regmap_field_write(regl->mode, val);
}

static int da9062_ldo_set_suspend_mode(struct regulator_dev *rdev,
						unsigned mode)
{
	struct da9062_regulator *regl = rdev_get_drvdata(rdev);
	unsigned val;

	switch (mode) {
	case REGULATOR_MODE_NORMAL:
		val = 0;
		break;
	case REGULATOR_MODE_STANDBY:
		val = 1;
		break;
	default:
		return -EINVAL;
	}

	return regmap_field_write(regl->suspend_sleep, val);
}

static const struct regulator_ops da9062_buck_ops = {
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.is_enabled		= regulator_is_enabled_regmap,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear,
<<<<<<< HEAD
	.set_current_limit	= da9062_set_current_limit,
	.get_current_limit	= da9062_get_current_limit,
=======
	.set_current_limit	= regulator_set_current_limit_regmap,
	.get_current_limit	= regulator_get_current_limit_regmap,
>>>>>>> upstream/android-13
	.set_mode		= da9062_buck_set_mode,
	.get_mode		= da9062_buck_get_mode,
	.get_status		= da9062_buck_get_status,
	.set_suspend_voltage	= da9062_set_suspend_voltage,
	.set_suspend_enable	= da9062_suspend_enable,
	.set_suspend_disable	= da9062_suspend_disable,
	.set_suspend_mode	= da9062_buck_set_suspend_mode,
};

static const struct regulator_ops da9062_ldo_ops = {
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.is_enabled		= regulator_is_enabled_regmap,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear,
	.set_mode		= da9062_ldo_set_mode,
	.get_mode		= da9062_ldo_get_mode,
	.get_status		= da9062_ldo_get_status,
	.set_suspend_voltage	= da9062_set_suspend_voltage,
	.set_suspend_enable	= da9062_suspend_enable,
	.set_suspend_disable	= da9062_suspend_disable,
	.set_suspend_mode	= da9062_ldo_set_suspend_mode,
};

/* DA9061 Regulator information */
static const struct da9062_regulator_info local_da9061_regulator_info[] = {
	{
		.desc.id = DA9061_ID_BUCK1,
		.desc.name = "DA9061 BUCK1",
		.desc.of_match = of_match_ptr("buck1"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_buck_ops,
		.desc.min_uV = (300) * 1000,
		.desc.uV_step = (10) * 1000,
		.desc.n_voltages = ((1570) - (300))/(10) + 1,
<<<<<<< HEAD
		.current_limits = da9062_buck_a_limits,
		.n_current_limits = ARRAY_SIZE(da9062_buck_a_limits),
=======
		.desc.curr_table = da9062_buck_a_limits,
		.desc.n_current_limits = ARRAY_SIZE(da9062_buck_a_limits),
		.desc.csel_reg = DA9062AA_BUCK_ILIM_C,
		.desc.csel_mask = DA9062AA_BUCK1_ILIM_MASK,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_BUCK1_CONT,
		.desc.enable_mask = DA9062AA_BUCK1_EN_MASK,
		.desc.vsel_reg = DA9062AA_VBUCK1_A,
		.desc.vsel_mask = DA9062AA_VBUCK1_A_MASK,
		.desc.linear_min_sel = 0,
<<<<<<< HEAD
=======
		.desc.of_map_mode = da9062_map_buck_mode,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VBUCK1_A,
			__builtin_ffs((int)DA9062AA_BUCK1_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK1_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VBUCK1_B,
			__builtin_ffs((int)DA9062AA_BUCK1_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK1_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VBUCK1_B,
		.mode = REG_FIELD(DA9062AA_BUCK1_CFG,
			__builtin_ffs((int)DA9062AA_BUCK1_MODE_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK1_MODE_MASK)) - 1),
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VBUCK1_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VBUCK1_SEL_MASK)) - 1),
		.ilimit = REG_FIELD(DA9062AA_BUCK_ILIM_C,
			__builtin_ffs((int)DA9062AA_BUCK1_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK1_ILIM_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_BUCK1_CONT,
			__builtin_ffs((int)DA9062AA_BUCK1_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_BUCK1_CONF_MASK) - 1),
>>>>>>> upstream/android-13
	},
	{
		.desc.id = DA9061_ID_BUCK2,
		.desc.name = "DA9061 BUCK2",
		.desc.of_match = of_match_ptr("buck2"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_buck_ops,
		.desc.min_uV = (800) * 1000,
		.desc.uV_step = (20) * 1000,
		.desc.n_voltages = ((3340) - (800))/(20) + 1,
<<<<<<< HEAD
		.current_limits = da9062_buck_b_limits,
		.n_current_limits = ARRAY_SIZE(da9062_buck_b_limits),
=======
		.desc.curr_table = da9062_buck_b_limits,
		.desc.n_current_limits = ARRAY_SIZE(da9062_buck_b_limits),
		.desc.csel_reg = DA9062AA_BUCK_ILIM_A,
		.desc.csel_mask = DA9062AA_BUCK3_ILIM_MASK,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_BUCK3_CONT,
		.desc.enable_mask = DA9062AA_BUCK3_EN_MASK,
		.desc.vsel_reg = DA9062AA_VBUCK3_A,
		.desc.vsel_mask = DA9062AA_VBUCK3_A_MASK,
		.desc.linear_min_sel = 0,
<<<<<<< HEAD
=======
		.desc.of_map_mode = da9062_map_buck_mode,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VBUCK3_A,
			__builtin_ffs((int)DA9062AA_BUCK3_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK3_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VBUCK3_B,
			__builtin_ffs((int)DA9062AA_BUCK3_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK3_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VBUCK3_B,
		.mode = REG_FIELD(DA9062AA_BUCK3_CFG,
			__builtin_ffs((int)DA9062AA_BUCK3_MODE_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK3_MODE_MASK)) - 1),
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VBUCK3_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VBUCK3_SEL_MASK)) - 1),
		.ilimit = REG_FIELD(DA9062AA_BUCK_ILIM_A,
			__builtin_ffs((int)DA9062AA_BUCK3_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK3_ILIM_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_BUCK3_CONT,
			__builtin_ffs((int)DA9062AA_BUCK3_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_BUCK3_CONF_MASK) - 1),
>>>>>>> upstream/android-13
	},
	{
		.desc.id = DA9061_ID_BUCK3,
		.desc.name = "DA9061 BUCK3",
		.desc.of_match = of_match_ptr("buck3"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_buck_ops,
		.desc.min_uV = (530) * 1000,
		.desc.uV_step = (10) * 1000,
		.desc.n_voltages = ((1800) - (530))/(10) + 1,
<<<<<<< HEAD
		.current_limits = da9062_buck_a_limits,
		.n_current_limits = ARRAY_SIZE(da9062_buck_a_limits),
=======
		.desc.curr_table = da9062_buck_a_limits,
		.desc.n_current_limits = ARRAY_SIZE(da9062_buck_a_limits),
		.desc.csel_reg = DA9062AA_BUCK_ILIM_B,
		.desc.csel_mask = DA9062AA_BUCK4_ILIM_MASK,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_BUCK4_CONT,
		.desc.enable_mask = DA9062AA_BUCK4_EN_MASK,
		.desc.vsel_reg = DA9062AA_VBUCK4_A,
		.desc.vsel_mask = DA9062AA_VBUCK4_A_MASK,
		.desc.linear_min_sel = 0,
<<<<<<< HEAD
=======
		.desc.of_map_mode = da9062_map_buck_mode,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VBUCK4_A,
			__builtin_ffs((int)DA9062AA_BUCK4_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK4_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VBUCK4_B,
			__builtin_ffs((int)DA9062AA_BUCK4_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK4_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VBUCK4_B,
		.mode = REG_FIELD(DA9062AA_BUCK4_CFG,
			__builtin_ffs((int)DA9062AA_BUCK4_MODE_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK4_MODE_MASK)) - 1),
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VBUCK4_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VBUCK4_SEL_MASK)) - 1),
		.ilimit = REG_FIELD(DA9062AA_BUCK_ILIM_B,
			__builtin_ffs((int)DA9062AA_BUCK4_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK4_ILIM_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_BUCK4_CONT,
			__builtin_ffs((int)DA9062AA_BUCK4_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_BUCK4_CONF_MASK) - 1),
>>>>>>> upstream/android-13
	},
	{
		.desc.id = DA9061_ID_LDO1,
		.desc.name = "DA9061 LDO1",
		.desc.of_match = of_match_ptr("ldo1"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_ldo_ops,
		.desc.min_uV = (900) * 1000,
		.desc.uV_step = (50) * 1000,
<<<<<<< HEAD
		.desc.n_voltages = ((3600) - (900))/(50) + 1,
=======
		.desc.n_voltages = ((3600) - (900))/(50) + 1
				+ DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_LDO1_CONT,
		.desc.enable_mask = DA9062AA_LDO1_EN_MASK,
		.desc.vsel_reg = DA9062AA_VLDO1_A,
		.desc.vsel_mask = DA9062AA_VLDO1_A_MASK,
<<<<<<< HEAD
		.desc.linear_min_sel = 0,
=======
		.desc.linear_min_sel = DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VLDO1_A,
			__builtin_ffs((int)DA9062AA_LDO1_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO1_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VLDO1_B,
			__builtin_ffs((int)DA9062AA_LDO1_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO1_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VLDO1_B,
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VLDO1_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VLDO1_SEL_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_LDO1_CONT,
			__builtin_ffs((int)DA9062AA_LDO1_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_LDO1_CONF_MASK) - 1),
>>>>>>> upstream/android-13
		.oc_event = REG_FIELD(DA9062AA_STATUS_D,
			__builtin_ffs((int)DA9062AA_LDO1_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO1_ILIM_MASK)) - 1),
	},
	{
		.desc.id = DA9061_ID_LDO2,
		.desc.name = "DA9061 LDO2",
		.desc.of_match = of_match_ptr("ldo2"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_ldo_ops,
		.desc.min_uV = (900) * 1000,
		.desc.uV_step = (50) * 1000,
<<<<<<< HEAD
		.desc.n_voltages = ((3600) - (600))/(50) + 1,
=======
		.desc.n_voltages = ((3600) - (900))/(50) + 1
				+ DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_LDO2_CONT,
		.desc.enable_mask = DA9062AA_LDO2_EN_MASK,
		.desc.vsel_reg = DA9062AA_VLDO2_A,
		.desc.vsel_mask = DA9062AA_VLDO2_A_MASK,
<<<<<<< HEAD
		.desc.linear_min_sel = 0,
=======
		.desc.linear_min_sel = DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VLDO2_A,
			__builtin_ffs((int)DA9062AA_LDO2_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO2_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VLDO2_B,
			__builtin_ffs((int)DA9062AA_LDO2_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO2_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VLDO2_B,
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VLDO2_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VLDO2_SEL_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_LDO2_CONT,
			__builtin_ffs((int)DA9062AA_LDO2_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_LDO2_CONF_MASK) - 1),
>>>>>>> upstream/android-13
		.oc_event = REG_FIELD(DA9062AA_STATUS_D,
			__builtin_ffs((int)DA9062AA_LDO2_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO2_ILIM_MASK)) - 1),
	},
	{
		.desc.id = DA9061_ID_LDO3,
		.desc.name = "DA9061 LDO3",
		.desc.of_match = of_match_ptr("ldo3"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_ldo_ops,
		.desc.min_uV = (900) * 1000,
		.desc.uV_step = (50) * 1000,
<<<<<<< HEAD
		.desc.n_voltages = ((3600) - (900))/(50) + 1,
=======
		.desc.n_voltages = ((3600) - (900))/(50) + 1
				+ DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_LDO3_CONT,
		.desc.enable_mask = DA9062AA_LDO3_EN_MASK,
		.desc.vsel_reg = DA9062AA_VLDO3_A,
		.desc.vsel_mask = DA9062AA_VLDO3_A_MASK,
<<<<<<< HEAD
		.desc.linear_min_sel = 0,
=======
		.desc.linear_min_sel = DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VLDO3_A,
			__builtin_ffs((int)DA9062AA_LDO3_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO3_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VLDO3_B,
			__builtin_ffs((int)DA9062AA_LDO3_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO3_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VLDO3_B,
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VLDO3_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VLDO3_SEL_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_LDO3_CONT,
			__builtin_ffs((int)DA9062AA_LDO3_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_LDO3_CONF_MASK) - 1),
>>>>>>> upstream/android-13
		.oc_event = REG_FIELD(DA9062AA_STATUS_D,
			__builtin_ffs((int)DA9062AA_LDO3_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO3_ILIM_MASK)) - 1),
	},
	{
		.desc.id = DA9061_ID_LDO4,
		.desc.name = "DA9061 LDO4",
		.desc.of_match = of_match_ptr("ldo4"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_ldo_ops,
		.desc.min_uV = (900) * 1000,
		.desc.uV_step = (50) * 1000,
<<<<<<< HEAD
		.desc.n_voltages = ((3600) - (900))/(50) + 1,
=======
		.desc.n_voltages = ((3600) - (900))/(50) + 1
				+ DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_LDO4_CONT,
		.desc.enable_mask = DA9062AA_LDO4_EN_MASK,
		.desc.vsel_reg = DA9062AA_VLDO4_A,
		.desc.vsel_mask = DA9062AA_VLDO4_A_MASK,
<<<<<<< HEAD
		.desc.linear_min_sel = 0,
=======
		.desc.linear_min_sel = DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VLDO4_A,
			__builtin_ffs((int)DA9062AA_LDO4_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO4_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VLDO4_B,
			__builtin_ffs((int)DA9062AA_LDO4_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO4_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VLDO4_B,
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VLDO4_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VLDO4_SEL_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_LDO4_CONT,
			__builtin_ffs((int)DA9062AA_LDO4_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_LDO4_CONF_MASK) - 1),
>>>>>>> upstream/android-13
		.oc_event = REG_FIELD(DA9062AA_STATUS_D,
			__builtin_ffs((int)DA9062AA_LDO4_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO4_ILIM_MASK)) - 1),
	},
};

/* DA9062 Regulator information */
static const struct da9062_regulator_info local_da9062_regulator_info[] = {
	{
		.desc.id = DA9062_ID_BUCK1,
		.desc.name = "DA9062 BUCK1",
		.desc.of_match = of_match_ptr("buck1"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_buck_ops,
		.desc.min_uV = (300) * 1000,
		.desc.uV_step = (10) * 1000,
		.desc.n_voltages = ((1570) - (300))/(10) + 1,
<<<<<<< HEAD
		.current_limits = da9062_buck_a_limits,
		.n_current_limits = ARRAY_SIZE(da9062_buck_a_limits),
=======
		.desc.curr_table = da9062_buck_a_limits,
		.desc.n_current_limits = ARRAY_SIZE(da9062_buck_a_limits),
		.desc.csel_reg = DA9062AA_BUCK_ILIM_C,
		.desc.csel_mask = DA9062AA_BUCK1_ILIM_MASK,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_BUCK1_CONT,
		.desc.enable_mask = DA9062AA_BUCK1_EN_MASK,
		.desc.vsel_reg = DA9062AA_VBUCK1_A,
		.desc.vsel_mask = DA9062AA_VBUCK1_A_MASK,
		.desc.linear_min_sel = 0,
<<<<<<< HEAD
=======
		.desc.of_map_mode = da9062_map_buck_mode,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VBUCK1_A,
			__builtin_ffs((int)DA9062AA_BUCK1_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK1_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VBUCK1_B,
			__builtin_ffs((int)DA9062AA_BUCK1_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK1_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VBUCK1_B,
		.mode = REG_FIELD(DA9062AA_BUCK1_CFG,
			__builtin_ffs((int)DA9062AA_BUCK1_MODE_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK1_MODE_MASK)) - 1),
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VBUCK1_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VBUCK1_SEL_MASK)) - 1),
		.ilimit = REG_FIELD(DA9062AA_BUCK_ILIM_C,
			__builtin_ffs((int)DA9062AA_BUCK1_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK1_ILIM_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_BUCK1_CONT,
			__builtin_ffs((int)DA9062AA_BUCK1_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_BUCK1_CONF_MASK) - 1),
>>>>>>> upstream/android-13
	},
	{
		.desc.id = DA9062_ID_BUCK2,
		.desc.name = "DA9062 BUCK2",
		.desc.of_match = of_match_ptr("buck2"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_buck_ops,
		.desc.min_uV = (300) * 1000,
		.desc.uV_step = (10) * 1000,
		.desc.n_voltages = ((1570) - (300))/(10) + 1,
<<<<<<< HEAD
		.current_limits = da9062_buck_a_limits,
		.n_current_limits = ARRAY_SIZE(da9062_buck_a_limits),
=======
		.desc.curr_table = da9062_buck_a_limits,
		.desc.n_current_limits = ARRAY_SIZE(da9062_buck_a_limits),
		.desc.csel_reg = DA9062AA_BUCK_ILIM_C,
		.desc.csel_mask = DA9062AA_BUCK2_ILIM_MASK,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_BUCK2_CONT,
		.desc.enable_mask = DA9062AA_BUCK2_EN_MASK,
		.desc.vsel_reg = DA9062AA_VBUCK2_A,
		.desc.vsel_mask = DA9062AA_VBUCK2_A_MASK,
		.desc.linear_min_sel = 0,
<<<<<<< HEAD
=======
		.desc.of_map_mode = da9062_map_buck_mode,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VBUCK2_A,
			__builtin_ffs((int)DA9062AA_BUCK2_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK2_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VBUCK2_B,
			__builtin_ffs((int)DA9062AA_BUCK2_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK2_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VBUCK2_B,
		.mode = REG_FIELD(DA9062AA_BUCK2_CFG,
			__builtin_ffs((int)DA9062AA_BUCK2_MODE_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK2_MODE_MASK)) - 1),
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VBUCK2_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VBUCK2_SEL_MASK)) - 1),
		.ilimit = REG_FIELD(DA9062AA_BUCK_ILIM_C,
			__builtin_ffs((int)DA9062AA_BUCK2_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK2_ILIM_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_BUCK2_CONT,
			__builtin_ffs((int)DA9062AA_BUCK2_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_BUCK2_CONF_MASK) - 1),
>>>>>>> upstream/android-13
	},
	{
		.desc.id = DA9062_ID_BUCK3,
		.desc.name = "DA9062 BUCK3",
		.desc.of_match = of_match_ptr("buck3"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_buck_ops,
		.desc.min_uV = (800) * 1000,
		.desc.uV_step = (20) * 1000,
		.desc.n_voltages = ((3340) - (800))/(20) + 1,
<<<<<<< HEAD
		.current_limits = da9062_buck_b_limits,
		.n_current_limits = ARRAY_SIZE(da9062_buck_b_limits),
=======
		.desc.curr_table = da9062_buck_b_limits,
		.desc.n_current_limits = ARRAY_SIZE(da9062_buck_b_limits),
		.desc.csel_reg = DA9062AA_BUCK_ILIM_A,
		.desc.csel_mask = DA9062AA_BUCK3_ILIM_MASK,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_BUCK3_CONT,
		.desc.enable_mask = DA9062AA_BUCK3_EN_MASK,
		.desc.vsel_reg = DA9062AA_VBUCK3_A,
		.desc.vsel_mask = DA9062AA_VBUCK3_A_MASK,
		.desc.linear_min_sel = 0,
<<<<<<< HEAD
=======
		.desc.of_map_mode = da9062_map_buck_mode,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VBUCK3_A,
			__builtin_ffs((int)DA9062AA_BUCK3_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK3_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VBUCK3_B,
			__builtin_ffs((int)DA9062AA_BUCK3_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK3_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VBUCK3_B,
		.mode = REG_FIELD(DA9062AA_BUCK3_CFG,
			__builtin_ffs((int)DA9062AA_BUCK3_MODE_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK3_MODE_MASK)) - 1),
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VBUCK3_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VBUCK3_SEL_MASK)) - 1),
		.ilimit = REG_FIELD(DA9062AA_BUCK_ILIM_A,
			__builtin_ffs((int)DA9062AA_BUCK3_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK3_ILIM_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_BUCK3_CONT,
			__builtin_ffs((int)DA9062AA_BUCK3_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_BUCK3_CONF_MASK) - 1),
>>>>>>> upstream/android-13
	},
	{
		.desc.id = DA9062_ID_BUCK4,
		.desc.name = "DA9062 BUCK4",
		.desc.of_match = of_match_ptr("buck4"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_buck_ops,
		.desc.min_uV = (530) * 1000,
		.desc.uV_step = (10) * 1000,
		.desc.n_voltages = ((1800) - (530))/(10) + 1,
<<<<<<< HEAD
		.current_limits = da9062_buck_a_limits,
		.n_current_limits = ARRAY_SIZE(da9062_buck_a_limits),
=======
		.desc.curr_table = da9062_buck_a_limits,
		.desc.n_current_limits = ARRAY_SIZE(da9062_buck_a_limits),
		.desc.csel_reg = DA9062AA_BUCK_ILIM_B,
		.desc.csel_mask = DA9062AA_BUCK4_ILIM_MASK,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_BUCK4_CONT,
		.desc.enable_mask = DA9062AA_BUCK4_EN_MASK,
		.desc.vsel_reg = DA9062AA_VBUCK4_A,
		.desc.vsel_mask = DA9062AA_VBUCK4_A_MASK,
		.desc.linear_min_sel = 0,
<<<<<<< HEAD
=======
		.desc.of_map_mode = da9062_map_buck_mode,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VBUCK4_A,
			__builtin_ffs((int)DA9062AA_BUCK4_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK4_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VBUCK4_B,
			__builtin_ffs((int)DA9062AA_BUCK4_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK4_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VBUCK4_B,
		.mode = REG_FIELD(DA9062AA_BUCK4_CFG,
			__builtin_ffs((int)DA9062AA_BUCK4_MODE_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK4_MODE_MASK)) - 1),
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VBUCK4_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VBUCK4_SEL_MASK)) - 1),
		.ilimit = REG_FIELD(DA9062AA_BUCK_ILIM_B,
			__builtin_ffs((int)DA9062AA_BUCK4_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_BUCK4_ILIM_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_BUCK4_CONT,
			__builtin_ffs((int)DA9062AA_BUCK4_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_BUCK4_CONF_MASK) - 1),
>>>>>>> upstream/android-13
	},
	{
		.desc.id = DA9062_ID_LDO1,
		.desc.name = "DA9062 LDO1",
		.desc.of_match = of_match_ptr("ldo1"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_ldo_ops,
		.desc.min_uV = (900) * 1000,
		.desc.uV_step = (50) * 1000,
<<<<<<< HEAD
		.desc.n_voltages = ((3600) - (900))/(50) + 1,
=======
		.desc.n_voltages = ((3600) - (900))/(50) + 1
				+ DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_LDO1_CONT,
		.desc.enable_mask = DA9062AA_LDO1_EN_MASK,
		.desc.vsel_reg = DA9062AA_VLDO1_A,
		.desc.vsel_mask = DA9062AA_VLDO1_A_MASK,
<<<<<<< HEAD
		.desc.linear_min_sel = 0,
=======
		.desc.linear_min_sel = DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VLDO1_A,
			__builtin_ffs((int)DA9062AA_LDO1_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO1_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VLDO1_B,
			__builtin_ffs((int)DA9062AA_LDO1_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO1_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VLDO1_B,
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VLDO1_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VLDO1_SEL_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_LDO1_CONT,
			__builtin_ffs((int)DA9062AA_LDO1_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_LDO1_CONF_MASK) - 1),
>>>>>>> upstream/android-13
		.oc_event = REG_FIELD(DA9062AA_STATUS_D,
			__builtin_ffs((int)DA9062AA_LDO1_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO1_ILIM_MASK)) - 1),
	},
	{
		.desc.id = DA9062_ID_LDO2,
		.desc.name = "DA9062 LDO2",
		.desc.of_match = of_match_ptr("ldo2"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_ldo_ops,
		.desc.min_uV = (900) * 1000,
		.desc.uV_step = (50) * 1000,
<<<<<<< HEAD
		.desc.n_voltages = ((3600) - (600))/(50) + 1,
=======
		.desc.n_voltages = ((3600) - (900))/(50) + 1
				+ DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_LDO2_CONT,
		.desc.enable_mask = DA9062AA_LDO2_EN_MASK,
		.desc.vsel_reg = DA9062AA_VLDO2_A,
		.desc.vsel_mask = DA9062AA_VLDO2_A_MASK,
<<<<<<< HEAD
		.desc.linear_min_sel = 0,
=======
		.desc.linear_min_sel = DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VLDO2_A,
			__builtin_ffs((int)DA9062AA_LDO2_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO2_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VLDO2_B,
			__builtin_ffs((int)DA9062AA_LDO2_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO2_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VLDO2_B,
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VLDO2_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VLDO2_SEL_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_LDO2_CONT,
			__builtin_ffs((int)DA9062AA_LDO2_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_LDO2_CONF_MASK) - 1),
>>>>>>> upstream/android-13
		.oc_event = REG_FIELD(DA9062AA_STATUS_D,
			__builtin_ffs((int)DA9062AA_LDO2_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO2_ILIM_MASK)) - 1),
	},
	{
		.desc.id = DA9062_ID_LDO3,
		.desc.name = "DA9062 LDO3",
		.desc.of_match = of_match_ptr("ldo3"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_ldo_ops,
		.desc.min_uV = (900) * 1000,
		.desc.uV_step = (50) * 1000,
<<<<<<< HEAD
		.desc.n_voltages = ((3600) - (900))/(50) + 1,
=======
		.desc.n_voltages = ((3600) - (900))/(50) + 1
				+ DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_LDO3_CONT,
		.desc.enable_mask = DA9062AA_LDO3_EN_MASK,
		.desc.vsel_reg = DA9062AA_VLDO3_A,
		.desc.vsel_mask = DA9062AA_VLDO3_A_MASK,
<<<<<<< HEAD
		.desc.linear_min_sel = 0,
=======
		.desc.linear_min_sel = DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VLDO3_A,
			__builtin_ffs((int)DA9062AA_LDO3_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO3_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VLDO3_B,
			__builtin_ffs((int)DA9062AA_LDO3_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO3_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VLDO3_B,
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VLDO3_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VLDO3_SEL_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_LDO3_CONT,
			__builtin_ffs((int)DA9062AA_LDO3_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_LDO3_CONF_MASK) - 1),
>>>>>>> upstream/android-13
		.oc_event = REG_FIELD(DA9062AA_STATUS_D,
			__builtin_ffs((int)DA9062AA_LDO3_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO3_ILIM_MASK)) - 1),
	},
	{
		.desc.id = DA9062_ID_LDO4,
		.desc.name = "DA9062 LDO4",
		.desc.of_match = of_match_ptr("ldo4"),
		.desc.regulators_node = of_match_ptr("regulators"),
		.desc.ops = &da9062_ldo_ops,
		.desc.min_uV = (900) * 1000,
		.desc.uV_step = (50) * 1000,
<<<<<<< HEAD
		.desc.n_voltages = ((3600) - (900))/(50) + 1,
=======
		.desc.n_voltages = ((3600) - (900))/(50) + 1
				+ DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.desc.enable_reg = DA9062AA_LDO4_CONT,
		.desc.enable_mask = DA9062AA_LDO4_EN_MASK,
		.desc.vsel_reg = DA9062AA_VLDO4_A,
		.desc.vsel_mask = DA9062AA_VLDO4_A_MASK,
<<<<<<< HEAD
		.desc.linear_min_sel = 0,
=======
		.desc.linear_min_sel = DA9062AA_VLDO_A_MIN_SEL,
>>>>>>> upstream/android-13
		.sleep = REG_FIELD(DA9062AA_VLDO4_A,
			__builtin_ffs((int)DA9062AA_LDO4_SL_A_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO4_SL_A_MASK)) - 1),
		.suspend_sleep = REG_FIELD(DA9062AA_VLDO4_B,
			__builtin_ffs((int)DA9062AA_LDO4_SL_B_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO4_SL_B_MASK)) - 1),
		.suspend_vsel_reg = DA9062AA_VLDO4_B,
<<<<<<< HEAD
		.suspend = REG_FIELD(DA9062AA_DVC_1,
			__builtin_ffs((int)DA9062AA_VLDO4_SEL_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_VLDO4_SEL_MASK)) - 1),
=======
		.suspend = REG_FIELD(DA9062AA_LDO4_CONT,
			__builtin_ffs((int)DA9062AA_LDO4_CONF_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz(DA9062AA_LDO4_CONF_MASK) - 1),
>>>>>>> upstream/android-13
		.oc_event = REG_FIELD(DA9062AA_STATUS_D,
			__builtin_ffs((int)DA9062AA_LDO4_ILIM_MASK) - 1,
			sizeof(unsigned int) * 8 -
			__builtin_clz((DA9062AA_LDO4_ILIM_MASK)) - 1),
	},
};

/* Regulator interrupt handlers */
static irqreturn_t da9062_ldo_lim_event(int irq, void *data)
{
	struct da9062_regulators *regulators = data;
	struct da9062 *hw = regulators->regulator[0].hw;
	struct da9062_regulator *regl;
	int handled = IRQ_NONE;
	int bits, i, ret;

	ret = regmap_read(hw->regmap, DA9062AA_STATUS_D, &bits);
	if (ret < 0) {
		dev_err(hw->dev,
			"Failed to read LDO overcurrent indicator\n");
		goto ldo_lim_error;
	}

	for (i = regulators->n_regulators - 1; i >= 0; i--) {
		regl = &regulators->regulator[i];
		if (regl->info->oc_event.reg != DA9062AA_STATUS_D)
			continue;

		if (BIT(regl->info->oc_event.lsb) & bits) {
			regulator_notifier_call_chain(regl->rdev,
					REGULATOR_EVENT_OVER_CURRENT, NULL);
			handled = IRQ_HANDLED;
		}
	}

ldo_lim_error:
	return handled;
}

static int da9062_regulator_probe(struct platform_device *pdev)
{
	struct da9062 *chip = dev_get_drvdata(pdev->dev.parent);
	struct da9062_regulators *regulators;
	struct da9062_regulator *regl;
	struct regulator_config config = { };
	const struct da9062_regulator_info *rinfo;
	int irq, n, ret;
<<<<<<< HEAD
	size_t size;
=======
>>>>>>> upstream/android-13
	int max_regulators;

	switch (chip->chip_type) {
	case COMPAT_TYPE_DA9061:
		max_regulators = DA9061_MAX_REGULATORS;
		rinfo = local_da9061_regulator_info;
		break;
	case COMPAT_TYPE_DA9062:
		max_regulators = DA9062_MAX_REGULATORS;
		rinfo = local_da9062_regulator_info;
		break;
	default:
		dev_err(chip->dev, "Unrecognised chip type\n");
		return -ENODEV;
	}

	/* Allocate memory required by usable regulators */
<<<<<<< HEAD
	size = sizeof(struct da9062_regulators) +
		max_regulators * sizeof(struct da9062_regulator);
	regulators = devm_kzalloc(&pdev->dev, size, GFP_KERNEL);
=======
	regulators = devm_kzalloc(&pdev->dev, struct_size(regulators, regulator,
				  max_regulators), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!regulators)
		return -ENOMEM;

	regulators->n_regulators = max_regulators;
	platform_set_drvdata(pdev, regulators);

<<<<<<< HEAD
	n = 0;
	while (n < regulators->n_regulators) {
=======
	for (n = 0; n < regulators->n_regulators; n++) {
>>>>>>> upstream/android-13
		/* Initialise regulator structure */
		regl = &regulators->regulator[n];
		regl->hw = chip;
		regl->info = &rinfo[n];
		regl->desc = regl->info->desc;
		regl->desc.type = REGULATOR_VOLTAGE;
		regl->desc.owner = THIS_MODULE;

<<<<<<< HEAD
		if (regl->info->mode.reg)
=======
		if (regl->info->mode.reg) {
>>>>>>> upstream/android-13
			regl->mode = devm_regmap_field_alloc(
					&pdev->dev,
					chip->regmap,
					regl->info->mode);
<<<<<<< HEAD
		if (regl->info->suspend.reg)
=======
			if (IS_ERR(regl->mode))
				return PTR_ERR(regl->mode);
		}

		if (regl->info->suspend.reg) {
>>>>>>> upstream/android-13
			regl->suspend = devm_regmap_field_alloc(
					&pdev->dev,
					chip->regmap,
					regl->info->suspend);
<<<<<<< HEAD
		if (regl->info->sleep.reg)
=======
			if (IS_ERR(regl->suspend))
				return PTR_ERR(regl->suspend);
		}

		if (regl->info->sleep.reg) {
>>>>>>> upstream/android-13
			regl->sleep = devm_regmap_field_alloc(
					&pdev->dev,
					chip->regmap,
					regl->info->sleep);
<<<<<<< HEAD
		if (regl->info->suspend_sleep.reg)
=======
			if (IS_ERR(regl->sleep))
				return PTR_ERR(regl->sleep);
		}

		if (regl->info->suspend_sleep.reg) {
>>>>>>> upstream/android-13
			regl->suspend_sleep = devm_regmap_field_alloc(
					&pdev->dev,
					chip->regmap,
					regl->info->suspend_sleep);
<<<<<<< HEAD
		if (regl->info->ilimit.reg)
			regl->ilimit = devm_regmap_field_alloc(
					&pdev->dev,
					chip->regmap,
					regl->info->ilimit);
=======
			if (IS_ERR(regl->suspend_sleep))
				return PTR_ERR(regl->suspend_sleep);
		}
>>>>>>> upstream/android-13

		/* Register regulator */
		memset(&config, 0, sizeof(config));
		config.dev = chip->dev;
		config.driver_data = regl;
		config.regmap = chip->regmap;

		regl->rdev = devm_regulator_register(&pdev->dev, &regl->desc,
						     &config);
		if (IS_ERR(regl->rdev)) {
			dev_err(&pdev->dev,
				"Failed to register %s regulator\n",
				regl->desc.name);
			return PTR_ERR(regl->rdev);
		}
<<<<<<< HEAD

		n++;
=======
>>>>>>> upstream/android-13
	}

	/* LDOs overcurrent event support */
	irq = platform_get_irq_byname(pdev, "LDO_LIM");
<<<<<<< HEAD
	if (irq < 0) {
		dev_err(&pdev->dev, "Failed to get IRQ.\n");
		return irq;
	}
=======
	if (irq < 0)
		return irq;
>>>>>>> upstream/android-13
	regulators->irq_ldo_lim = irq;

	ret = devm_request_threaded_irq(&pdev->dev, irq,
					NULL, da9062_ldo_lim_event,
					IRQF_TRIGGER_LOW | IRQF_ONESHOT,
					"LDO_LIM", regulators);
	if (ret) {
		dev_warn(&pdev->dev,
			 "Failed to request LDO_LIM IRQ.\n");
		regulators->irq_ldo_lim = -ENXIO;
	}

	return 0;
}

static struct platform_driver da9062_regulator_driver = {
	.driver = {
		.name = "da9062-regulators",
	},
	.probe = da9062_regulator_probe,
};

static int __init da9062_regulator_init(void)
{
	return platform_driver_register(&da9062_regulator_driver);
}
subsys_initcall(da9062_regulator_init);

static void __exit da9062_regulator_cleanup(void)
{
	platform_driver_unregister(&da9062_regulator_driver);
}
module_exit(da9062_regulator_cleanup);

/* Module information */
MODULE_AUTHOR("S Twiss <stwiss.opensource@diasemi.com>");
MODULE_DESCRIPTION("REGULATOR device driver for Dialog DA9062 and DA9061");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:da9062-regulators");
