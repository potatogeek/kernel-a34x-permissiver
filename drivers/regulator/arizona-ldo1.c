<<<<<<< HEAD
/*
 * arizona-ldo1.c  --  LDO1 supply for Arizona devices
 *
 * Copyright 2012 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 */
=======
// SPDX-License-Identifier: GPL-2.0+
//
// arizona-ldo1.c  --  LDO1 supply for Arizona devices
//
// Copyright 2012 Wolfson Microelectronics PLC.
//
// Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
>>>>>>> upstream/android-13

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/of_regulator.h>
#include <linux/slab.h>

#include <linux/regulator/arizona-ldo1.h>

#include <linux/mfd/arizona/core.h>
#include <linux/mfd/arizona/pdata.h>
#include <linux/mfd/arizona/registers.h>

<<<<<<< HEAD
=======
#include <linux/mfd/madera/core.h>
#include <linux/mfd/madera/pdata.h>
#include <linux/mfd/madera/registers.h>

>>>>>>> upstream/android-13
struct arizona_ldo1 {
	struct regulator_dev *regulator;
	struct regmap *regmap;

	struct regulator_consumer_supply supply;
	struct regulator_init_data init_data;

	struct gpio_desc *ena_gpiod;
};

<<<<<<< HEAD
static int arizona_ldo1_hc_list_voltage(struct regulator_dev *rdev,
					unsigned int selector)
{
	if (selector >= rdev->desc->n_voltages)
		return -EINVAL;

	if (selector == rdev->desc->n_voltages - 1)
		return 1800000;
	else
		return rdev->desc->min_uV + (rdev->desc->uV_step * selector);
}

static int arizona_ldo1_hc_map_voltage(struct regulator_dev *rdev,
				       int min_uV, int max_uV)
{
	int sel;

	sel = DIV_ROUND_UP(min_uV - rdev->desc->min_uV, rdev->desc->uV_step);
	if (sel >= rdev->desc->n_voltages)
		sel = rdev->desc->n_voltages - 1;

	return sel;
}

static int arizona_ldo1_hc_set_voltage_sel(struct regulator_dev *rdev,
					   unsigned sel)
{
	struct arizona_ldo1 *ldo = rdev_get_drvdata(rdev);
	struct regmap *regmap = ldo->regmap;
=======
static int arizona_ldo1_hc_set_voltage_sel(struct regulator_dev *rdev,
					   unsigned sel)
{
	struct regmap *regmap = rdev_get_regmap(rdev);
>>>>>>> upstream/android-13
	unsigned int val;
	int ret;

	if (sel == rdev->desc->n_voltages - 1)
		val = ARIZONA_LDO1_HI_PWR;
	else
		val = 0;

	ret = regmap_update_bits(regmap, ARIZONA_LDO1_CONTROL_2,
				 ARIZONA_LDO1_HI_PWR, val);
	if (ret != 0)
		return ret;

	if (val)
		return 0;

<<<<<<< HEAD
	val = sel << ARIZONA_LDO1_VSEL_SHIFT;

	return regmap_update_bits(regmap, ARIZONA_LDO1_CONTROL_1,
				  ARIZONA_LDO1_VSEL_MASK, val);
=======
	return regulator_set_voltage_sel_regmap(rdev, sel);
>>>>>>> upstream/android-13
}

static int arizona_ldo1_hc_get_voltage_sel(struct regulator_dev *rdev)
{
<<<<<<< HEAD
	struct arizona_ldo1 *ldo = rdev_get_drvdata(rdev);
	struct regmap *regmap = ldo->regmap;
=======
	struct regmap *regmap = rdev_get_regmap(rdev);
>>>>>>> upstream/android-13
	unsigned int val;
	int ret;

	ret = regmap_read(regmap, ARIZONA_LDO1_CONTROL_2, &val);
	if (ret != 0)
		return ret;

	if (val & ARIZONA_LDO1_HI_PWR)
		return rdev->desc->n_voltages - 1;

<<<<<<< HEAD
	ret = regmap_read(regmap, ARIZONA_LDO1_CONTROL_1, &val);
	if (ret != 0)
		return ret;

	return (val & ARIZONA_LDO1_VSEL_MASK) >> ARIZONA_LDO1_VSEL_SHIFT;
}

static const struct regulator_ops arizona_ldo1_hc_ops = {
	.list_voltage = arizona_ldo1_hc_list_voltage,
	.map_voltage = arizona_ldo1_hc_map_voltage,
=======
	return regulator_get_voltage_sel_regmap(rdev);
}

static const struct regulator_ops arizona_ldo1_hc_ops = {
	.list_voltage = regulator_list_voltage_linear_range,
	.map_voltage = regulator_map_voltage_linear_range,
>>>>>>> upstream/android-13
	.get_voltage_sel = arizona_ldo1_hc_get_voltage_sel,
	.set_voltage_sel = arizona_ldo1_hc_set_voltage_sel,
	.get_bypass = regulator_get_bypass_regmap,
	.set_bypass = regulator_set_bypass_regmap,
};

<<<<<<< HEAD
=======
static const struct linear_range arizona_ldo1_hc_ranges[] = {
	REGULATOR_LINEAR_RANGE(900000, 0, 0x6, 50000),
	REGULATOR_LINEAR_RANGE(1800000, 0x7, 0x7, 0),
};

>>>>>>> upstream/android-13
static const struct regulator_desc arizona_ldo1_hc = {
	.name = "LDO1",
	.supply_name = "LDOVDD",
	.type = REGULATOR_VOLTAGE,
	.ops = &arizona_ldo1_hc_ops,

<<<<<<< HEAD
	.bypass_reg = ARIZONA_LDO1_CONTROL_1,
	.bypass_mask = ARIZONA_LDO1_BYPASS,
	.min_uV = 900000,
	.uV_step = 50000,
=======
	.vsel_reg = ARIZONA_LDO1_CONTROL_1,
	.vsel_mask = ARIZONA_LDO1_VSEL_MASK,
	.bypass_reg = ARIZONA_LDO1_CONTROL_1,
	.bypass_mask = ARIZONA_LDO1_BYPASS,
	.linear_ranges = arizona_ldo1_hc_ranges,
	.n_linear_ranges = ARRAY_SIZE(arizona_ldo1_hc_ranges),
>>>>>>> upstream/android-13
	.n_voltages = 8,
	.enable_time = 1500,
	.ramp_delay = 24000,

	.owner = THIS_MODULE,
};

static const struct regulator_ops arizona_ldo1_ops = {
	.list_voltage = regulator_list_voltage_linear,
	.map_voltage = regulator_map_voltage_linear,
	.get_voltage_sel = regulator_get_voltage_sel_regmap,
	.set_voltage_sel = regulator_set_voltage_sel_regmap,
};

static const struct regulator_desc arizona_ldo1 = {
	.name = "LDO1",
	.supply_name = "LDOVDD",
	.type = REGULATOR_VOLTAGE,
	.ops = &arizona_ldo1_ops,

	.vsel_reg = ARIZONA_LDO1_CONTROL_1,
	.vsel_mask = ARIZONA_LDO1_VSEL_MASK,
	.min_uV = 900000,
	.uV_step = 25000,
	.n_voltages = 13,
	.enable_time = 500,
	.ramp_delay = 24000,

	.owner = THIS_MODULE,
};

static const struct regulator_init_data arizona_ldo1_dvfs = {
	.constraints = {
		.min_uV = 1200000,
		.max_uV = 1800000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
				  REGULATOR_CHANGE_VOLTAGE,
	},
	.num_consumer_supplies = 1,
};

static const struct regulator_init_data arizona_ldo1_default = {
	.constraints = {
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies = 1,
};

static const struct regulator_init_data arizona_ldo1_wm5110 = {
	.constraints = {
		.min_uV = 1175000,
		.max_uV = 1200000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
				  REGULATOR_CHANGE_VOLTAGE,
	},
	.num_consumer_supplies = 1,
};

<<<<<<< HEAD
=======
static const struct regulator_desc madera_ldo1 = {
	.name = "LDO1",
	.supply_name = "LDOVDD",
	.type = REGULATOR_VOLTAGE,
	.ops = &arizona_ldo1_ops,

	.vsel_reg = MADERA_LDO1_CONTROL_1,
	.vsel_mask = MADERA_LDO1_VSEL_MASK,
	.min_uV = 900000,
	.uV_step = 25000,
	.n_voltages = 13,
	.enable_time = 3000,

	.owner = THIS_MODULE,
};

static const struct regulator_init_data madera_ldo1_default = {
	.constraints = {
		.min_uV = 1200000,
		.max_uV = 1200000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies = 1,
};

>>>>>>> upstream/android-13
static int arizona_ldo1_of_get_pdata(struct arizona_ldo1_pdata *pdata,
				     struct regulator_config *config,
				     const struct regulator_desc *desc,
				     bool *external_dcvdd)
{
	struct arizona_ldo1 *ldo1 = config->driver_data;
	struct device_node *np = config->dev->of_node;
	struct device_node *init_node, *dcvdd_node;
	struct regulator_init_data *init_data;

	init_node = of_get_child_by_name(np, "ldo1");
	dcvdd_node = of_parse_phandle(np, "DCVDD-supply", 0);

	if (init_node) {
		config->of_node = init_node;

		init_data = of_get_regulator_init_data(config->dev, init_node,
						       desc);
		if (init_data) {
			init_data->consumer_supplies = &ldo1->supply;
			init_data->num_consumer_supplies = 1;

			if (dcvdd_node && dcvdd_node != init_node)
				*external_dcvdd = true;

			pdata->init_data = init_data;
		}
	} else if (dcvdd_node) {
		*external_dcvdd = true;
	}

	of_node_put(dcvdd_node);

	return 0;
}

static int arizona_ldo1_common_init(struct platform_device *pdev,
				    struct arizona_ldo1 *ldo1,
				    const struct regulator_desc *desc,
				    struct arizona_ldo1_pdata *pdata,
				    bool *external_dcvdd)
{
	struct device *parent_dev = pdev->dev.parent;
	struct regulator_config config = { };
	int ret;

	*external_dcvdd = false;

	ldo1->supply.supply = "DCVDD";
	ldo1->init_data.consumer_supplies = &ldo1->supply;
	ldo1->supply.dev_name = dev_name(parent_dev);

	config.dev = parent_dev;
	config.driver_data = ldo1;
	config.regmap = ldo1->regmap;

	if (IS_ENABLED(CONFIG_OF)) {
		if (!dev_get_platdata(parent_dev)) {
			ret = arizona_ldo1_of_get_pdata(pdata,
							&config, desc,
							external_dcvdd);
			if (ret < 0)
				return ret;
		}
	}

	/* We assume that high output = regulator off
	 * Don't use devm, since we need to get against the parent device
	 * so clean up would happen at the wrong time
	 */
	config.ena_gpiod = gpiod_get_optional(parent_dev, "wlf,ldoena",
<<<<<<< HEAD
					      GPIOD_OUT_LOW);
=======
				GPIOD_OUT_LOW | GPIOD_FLAGS_BIT_NONEXCLUSIVE);
>>>>>>> upstream/android-13
	if (IS_ERR(config.ena_gpiod))
		return PTR_ERR(config.ena_gpiod);

	ldo1->ena_gpiod = config.ena_gpiod;

	if (pdata->init_data)
		config.init_data = pdata->init_data;
	else
		config.init_data = &ldo1->init_data;

	/*
	 * LDO1 can only be used to supply DCVDD so if it has no
	 * consumers then DCVDD is supplied externally.
	 */
	if (config.init_data->num_consumer_supplies == 0)
		*external_dcvdd = true;

	ldo1->regulator = devm_regulator_register(&pdev->dev, desc, &config);

	of_node_put(config.of_node);

	if (IS_ERR(ldo1->regulator)) {
<<<<<<< HEAD
		if (config.ena_gpiod)
			gpiod_put(config.ena_gpiod);

=======
>>>>>>> upstream/android-13
		ret = PTR_ERR(ldo1->regulator);
		dev_err(&pdev->dev, "Failed to register LDO1 supply: %d\n",
			ret);
		return ret;
	}

	platform_set_drvdata(pdev, ldo1);

	return 0;
}

static int arizona_ldo1_probe(struct platform_device *pdev)
{
	struct arizona *arizona = dev_get_drvdata(pdev->dev.parent);
	struct arizona_ldo1 *ldo1;
	const struct regulator_desc *desc;
	bool external_dcvdd;
	int ret;

	ldo1 = devm_kzalloc(&pdev->dev, sizeof(*ldo1), GFP_KERNEL);
	if (!ldo1)
		return -ENOMEM;

	ldo1->regmap = arizona->regmap;

	/*
	 * Since the chip usually supplies itself we provide some
	 * default init_data for it.  This will be overridden with
	 * platform data if provided.
	 */
	switch (arizona->type) {
	case WM5102:
	case WM8997:
	case WM8998:
	case WM1814:
		desc = &arizona_ldo1_hc;
		ldo1->init_data = arizona_ldo1_dvfs;
		break;
	case WM5110:
	case WM8280:
		desc = &arizona_ldo1;
		ldo1->init_data = arizona_ldo1_wm5110;
		break;
	default:
		desc = &arizona_ldo1;
		ldo1->init_data = arizona_ldo1_default;
		break;
	}

	ret = arizona_ldo1_common_init(pdev, ldo1, desc,
				       &arizona->pdata.ldo1,
				       &external_dcvdd);
	if (ret == 0)
		arizona->external_dcvdd = external_dcvdd;

	return ret;
}

static int arizona_ldo1_remove(struct platform_device *pdev)
{
	struct arizona_ldo1 *ldo1 = platform_get_drvdata(pdev);

	if (ldo1->ena_gpiod)
		gpiod_put(ldo1->ena_gpiod);

	return 0;
}

<<<<<<< HEAD
=======
static int madera_ldo1_probe(struct platform_device *pdev)
{
	struct madera *madera = dev_get_drvdata(pdev->dev.parent);
	struct arizona_ldo1 *ldo1;
	bool external_dcvdd;
	int ret;

	ldo1 = devm_kzalloc(&pdev->dev, sizeof(*ldo1), GFP_KERNEL);
	if (!ldo1)
		return -ENOMEM;

	ldo1->regmap = madera->regmap;

	ldo1->init_data = madera_ldo1_default;

	ret = arizona_ldo1_common_init(pdev, ldo1, &madera_ldo1,
				       &madera->pdata.ldo1,
				       &external_dcvdd);
	if (ret)
		return ret;

	madera->internal_dcvdd = !external_dcvdd;

	return 0;
}

>>>>>>> upstream/android-13
static struct platform_driver arizona_ldo1_driver = {
	.probe = arizona_ldo1_probe,
	.remove = arizona_ldo1_remove,
	.driver		= {
		.name	= "arizona-ldo1",
	},
};

<<<<<<< HEAD
module_platform_driver(arizona_ldo1_driver);
=======
static struct platform_driver madera_ldo1_driver = {
	.probe = madera_ldo1_probe,
	.remove = arizona_ldo1_remove,
	.driver		= {
		.name	= "madera-ldo1",
	},
};

static struct platform_driver * const madera_ldo1_drivers[] = {
	&arizona_ldo1_driver,
	&madera_ldo1_driver,
};

static int __init arizona_ldo1_init(void)
{
	return platform_register_drivers(madera_ldo1_drivers,
					 ARRAY_SIZE(madera_ldo1_drivers));
}
module_init(arizona_ldo1_init);

static void __exit madera_ldo1_exit(void)
{
	platform_unregister_drivers(madera_ldo1_drivers,
				    ARRAY_SIZE(madera_ldo1_drivers));
}
module_exit(madera_ldo1_exit);
>>>>>>> upstream/android-13

/* Module information */
MODULE_AUTHOR("Mark Brown <broonie@opensource.wolfsonmicro.com>");
MODULE_DESCRIPTION("Arizona LDO1 driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:arizona-ldo1");
<<<<<<< HEAD
=======
MODULE_ALIAS("platform:madera-ldo1");
>>>>>>> upstream/android-13
