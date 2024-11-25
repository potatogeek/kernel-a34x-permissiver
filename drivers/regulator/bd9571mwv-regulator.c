<<<<<<< HEAD
/*
 * ROHM BD9571MWV-M regulator driver
 *
 * Copyright (C) 2017 Marek Vasut <marek.vasut+renesas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether expressed or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * ROHM BD9571MWV-M and BD9574MWF-M regulator driver
 *
 * Copyright (C) 2017 Marek Vasut <marek.vasut+renesas@gmail.com>
 *
>>>>>>> upstream/android-13
 * Based on the TPS65086 driver
 *
 * NOTE: VD09 is missing
 */

<<<<<<< HEAD
=======
#include <linux/mfd/rohm-generic.h>
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>

#include <linux/mfd/bd9571mwv.h>

struct bd9571mwv_reg {
<<<<<<< HEAD
	struct bd9571mwv *bd;
=======
	struct regmap *regmap;
>>>>>>> upstream/android-13

	/* DDR Backup Power */
	u8 bkup_mode_cnt_keepon;	/* from "rohm,ddr-backup-power" */
	u8 bkup_mode_cnt_saved;
	bool bkup_mode_enabled;

	/* Power switch type */
	bool rstbmode_level;
	bool rstbmode_pulse;
};

enum bd9571mwv_regulators { VD09, VD18, VD25, VD33, DVFS };

#define BD9571MWV_REG(_name, _of, _id, _ops, _vr, _vm, _nv, _min, _step, _lmin)\
	{							\
		.name			= _name,		\
		.of_match		= of_match_ptr(_of),	\
		.regulators_node	= "regulators",		\
		.id			= _id,			\
		.ops			= &_ops,		\
		.n_voltages		= _nv,			\
		.type			= REGULATOR_VOLTAGE,	\
		.owner			= THIS_MODULE,		\
		.vsel_reg		= _vr,			\
		.vsel_mask		= _vm,			\
		.min_uV			= _min,			\
		.uV_step		= _step,		\
		.linear_min_sel		= _lmin,		\
	}

static int bd9571mwv_avs_get_moni_state(struct regulator_dev *rdev)
{
	unsigned int val;
	int ret;

	ret = regmap_read(rdev->regmap, BD9571MWV_AVS_SET_MONI, &val);
	if (ret != 0)
		return ret;

	return val & BD9571MWV_AVS_SET_MONI_MASK;
}

static int bd9571mwv_avs_set_voltage_sel_regmap(struct regulator_dev *rdev,
						unsigned int sel)
{
	int ret;

	ret = bd9571mwv_avs_get_moni_state(rdev);
	if (ret < 0)
		return ret;

	return regmap_write_bits(rdev->regmap, BD9571MWV_AVS_VD09_VID(ret),
				 rdev->desc->vsel_mask, sel);
}

static int bd9571mwv_avs_get_voltage_sel_regmap(struct regulator_dev *rdev)
{
	unsigned int val;
	int ret;

	ret = bd9571mwv_avs_get_moni_state(rdev);
	if (ret < 0)
		return ret;

	ret = regmap_read(rdev->regmap, BD9571MWV_AVS_VD09_VID(ret), &val);
	if (ret != 0)
		return ret;

	val &= rdev->desc->vsel_mask;
	val >>= ffs(rdev->desc->vsel_mask) - 1;

	return val;
}

static int bd9571mwv_reg_set_voltage_sel_regmap(struct regulator_dev *rdev,
						unsigned int sel)
{
	return regmap_write_bits(rdev->regmap, BD9571MWV_DVFS_SETVID,
				 rdev->desc->vsel_mask, sel);
}

/* Operations permitted on AVS voltage regulator */
<<<<<<< HEAD
static struct regulator_ops avs_ops = {
=======
static const struct regulator_ops avs_ops = {
>>>>>>> upstream/android-13
	.set_voltage_sel	= bd9571mwv_avs_set_voltage_sel_regmap,
	.map_voltage		= regulator_map_voltage_linear,
	.get_voltage_sel	= bd9571mwv_avs_get_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear,
};

/* Operations permitted on voltage regulators */
<<<<<<< HEAD
static struct regulator_ops reg_ops = {
=======
static const struct regulator_ops reg_ops = {
>>>>>>> upstream/android-13
	.set_voltage_sel	= bd9571mwv_reg_set_voltage_sel_regmap,
	.map_voltage		= regulator_map_voltage_linear,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear,
};

/* Operations permitted on voltage monitors */
<<<<<<< HEAD
static struct regulator_ops vid_ops = {
=======
static const struct regulator_ops vid_ops = {
>>>>>>> upstream/android-13
	.map_voltage		= regulator_map_voltage_linear,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.list_voltage		= regulator_list_voltage_linear,
};

<<<<<<< HEAD
static struct regulator_desc regulators[] = {
=======
static const struct regulator_desc regulators[] = {
>>>>>>> upstream/android-13
	BD9571MWV_REG("VD09", "vd09", VD09, avs_ops, 0, 0x7f,
		      0x6f, 600000, 10000, 0x3c),
	BD9571MWV_REG("VD18", "vd18", VD18, vid_ops, BD9571MWV_VD18_VID, 0xf,
		      16, 1625000, 25000, 0),
	BD9571MWV_REG("VD25", "vd25", VD25, vid_ops, BD9571MWV_VD25_VID, 0xf,
		      16, 2150000, 50000, 0),
	BD9571MWV_REG("VD33", "vd33", VD33, vid_ops, BD9571MWV_VD33_VID, 0xf,
		      11, 2800000, 100000, 0),
	BD9571MWV_REG("DVFS", "dvfs", DVFS, reg_ops,
		      BD9571MWV_DVFS_MONIVDAC, 0x7f,
		      0x6f, 600000, 10000, 0x3c),
};

#ifdef CONFIG_PM_SLEEP
<<<<<<< HEAD
static int bd9571mwv_bkup_mode_read(struct bd9571mwv *bd, unsigned int *mode)
{
	int ret;

	ret = regmap_read(bd->regmap, BD9571MWV_BKUP_MODE_CNT, mode);
	if (ret) {
		dev_err(bd->dev, "failed to read backup mode (%d)\n", ret);
=======
static int bd9571mwv_bkup_mode_read(struct bd9571mwv_reg *bdreg,
				    unsigned int *mode)
{
	int ret;

	ret = regmap_read(bdreg->regmap, BD9571MWV_BKUP_MODE_CNT, mode);
	if (ret) {
		dev_err(regmap_get_device(bdreg->regmap),
			"failed to read backup mode (%d)\n", ret);
>>>>>>> upstream/android-13
		return ret;
	}

	return 0;
}

<<<<<<< HEAD
static int bd9571mwv_bkup_mode_write(struct bd9571mwv *bd, unsigned int mode)
{
	int ret;

	ret = regmap_write(bd->regmap, BD9571MWV_BKUP_MODE_CNT, mode);
	if (ret) {
		dev_err(bd->dev, "failed to configure backup mode 0x%x (%d)\n",
=======
static int bd9571mwv_bkup_mode_write(struct bd9571mwv_reg *bdreg,
				     unsigned int mode)
{
	int ret;

	ret = regmap_write(bdreg->regmap, BD9571MWV_BKUP_MODE_CNT, mode);
	if (ret) {
		dev_err(regmap_get_device(bdreg->regmap),
			"failed to configure backup mode 0x%x (%d)\n",
>>>>>>> upstream/android-13
			mode, ret);
		return ret;
	}

	return 0;
}

static ssize_t backup_mode_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct bd9571mwv_reg *bdreg = dev_get_drvdata(dev);

<<<<<<< HEAD
	return sprintf(buf, "%s\n", bdreg->bkup_mode_enabled ? "on" : "off");
=======
	return sysfs_emit(buf, "%s\n", bdreg->bkup_mode_enabled ? "on" : "off");
>>>>>>> upstream/android-13
}

static ssize_t backup_mode_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
{
	struct bd9571mwv_reg *bdreg = dev_get_drvdata(dev);
	unsigned int mode;
	int ret;

	if (!count)
		return 0;

	ret = kstrtobool(buf, &bdreg->bkup_mode_enabled);
	if (ret)
		return ret;

	if (!bdreg->rstbmode_level)
		return count;

	/*
	 * Configure DDR Backup Mode, to change the role of the accessory power
	 * switch from a power switch to a wake-up switch, or vice versa
	 */
<<<<<<< HEAD
	ret = bd9571mwv_bkup_mode_read(bdreg->bd, &mode);
=======
	ret = bd9571mwv_bkup_mode_read(bdreg, &mode);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	mode &= ~BD9571MWV_BKUP_MODE_CNT_KEEPON_MASK;
	if (bdreg->bkup_mode_enabled)
		mode |= bdreg->bkup_mode_cnt_keepon;

<<<<<<< HEAD
	ret = bd9571mwv_bkup_mode_write(bdreg->bd, mode);
=======
	ret = bd9571mwv_bkup_mode_write(bdreg, mode);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	return count;
}

static DEVICE_ATTR_RW(backup_mode);

static int bd9571mwv_suspend(struct device *dev)
{
	struct bd9571mwv_reg *bdreg = dev_get_drvdata(dev);
	unsigned int mode;
	int ret;

	if (!bdreg->bkup_mode_enabled)
		return 0;

	/* Save DDR Backup Mode */
<<<<<<< HEAD
	ret = bd9571mwv_bkup_mode_read(bdreg->bd, &mode);
=======
	ret = bd9571mwv_bkup_mode_read(bdreg, &mode);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	bdreg->bkup_mode_cnt_saved = mode;

	if (!bdreg->rstbmode_pulse)
		return 0;

	/* Enable DDR Backup Mode */
	mode &= ~BD9571MWV_BKUP_MODE_CNT_KEEPON_MASK;
	mode |= bdreg->bkup_mode_cnt_keepon;

	if (mode != bdreg->bkup_mode_cnt_saved)
<<<<<<< HEAD
		return bd9571mwv_bkup_mode_write(bdreg->bd, mode);
=======
		return bd9571mwv_bkup_mode_write(bdreg, mode);
>>>>>>> upstream/android-13

	return 0;
}

static int bd9571mwv_resume(struct device *dev)
{
	struct bd9571mwv_reg *bdreg = dev_get_drvdata(dev);

	if (!bdreg->bkup_mode_enabled)
		return 0;

	/* Restore DDR Backup Mode */
<<<<<<< HEAD
	return bd9571mwv_bkup_mode_write(bdreg->bd, bdreg->bkup_mode_cnt_saved);
=======
	return bd9571mwv_bkup_mode_write(bdreg, bdreg->bkup_mode_cnt_saved);
>>>>>>> upstream/android-13
}

static const struct dev_pm_ops bd9571mwv_pm  = {
	SET_SYSTEM_SLEEP_PM_OPS(bd9571mwv_suspend, bd9571mwv_resume)
};

static int bd9571mwv_regulator_remove(struct platform_device *pdev)
{
	device_remove_file(&pdev->dev, &dev_attr_backup_mode);
	return 0;
}
#define DEV_PM_OPS	&bd9571mwv_pm
#else
#define DEV_PM_OPS	NULL
#define bd9571mwv_regulator_remove	NULL
#endif /* CONFIG_PM_SLEEP */

static int bd9571mwv_regulator_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct bd9571mwv *bd = dev_get_drvdata(pdev->dev.parent);
=======
>>>>>>> upstream/android-13
	struct regulator_config config = { };
	struct bd9571mwv_reg *bdreg;
	struct regulator_dev *rdev;
	unsigned int val;
	int i;
<<<<<<< HEAD
=======
	enum rohm_chip_type chip = platform_get_device_id(pdev)->driver_data;
>>>>>>> upstream/android-13

	bdreg = devm_kzalloc(&pdev->dev, sizeof(*bdreg), GFP_KERNEL);
	if (!bdreg)
		return -ENOMEM;

<<<<<<< HEAD
	bdreg->bd = bd;
=======
	bdreg->regmap = dev_get_regmap(pdev->dev.parent, NULL);
>>>>>>> upstream/android-13

	platform_set_drvdata(pdev, bdreg);

	config.dev = &pdev->dev;
<<<<<<< HEAD
	config.dev->of_node = bd->dev->of_node;
	config.driver_data = bd;
	config.regmap = bd->regmap;

	for (i = 0; i < ARRAY_SIZE(regulators); i++) {
		rdev = devm_regulator_register(&pdev->dev, &regulators[i],
					       &config);
		if (IS_ERR(rdev)) {
			dev_err(bd->dev, "failed to register %s regulator\n",
				pdev->name);
=======
	config.dev->of_node = pdev->dev.parent->of_node;
	config.driver_data = bdreg;
	config.regmap = bdreg->regmap;

	for (i = 0; i < ARRAY_SIZE(regulators); i++) {
		/* BD9574MWF supports DVFS only */
		if (chip == ROHM_CHIP_TYPE_BD9574 && regulators[i].id != DVFS)
			continue;
		rdev = devm_regulator_register(&pdev->dev, &regulators[i],
					       &config);
		if (IS_ERR(rdev)) {
			dev_err(&pdev->dev, "failed to register %s regulator\n",
				regulators[i].name);
>>>>>>> upstream/android-13
			return PTR_ERR(rdev);
		}
	}

	val = 0;
<<<<<<< HEAD
	of_property_read_u32(bd->dev->of_node, "rohm,ddr-backup-power", &val);
	if (val & ~BD9571MWV_BKUP_MODE_CNT_KEEPON_MASK) {
		dev_err(bd->dev, "invalid %s mode %u\n",
=======
	of_property_read_u32(config.dev->of_node, "rohm,ddr-backup-power", &val);
	if (val & ~BD9571MWV_BKUP_MODE_CNT_KEEPON_MASK) {
		dev_err(&pdev->dev, "invalid %s mode %u\n",
>>>>>>> upstream/android-13
			"rohm,ddr-backup-power", val);
		return -EINVAL;
	}
	bdreg->bkup_mode_cnt_keepon = val;

<<<<<<< HEAD
	bdreg->rstbmode_level = of_property_read_bool(bd->dev->of_node,
						      "rohm,rstbmode-level");
	bdreg->rstbmode_pulse = of_property_read_bool(bd->dev->of_node,
						      "rohm,rstbmode-pulse");
	if (bdreg->rstbmode_level && bdreg->rstbmode_pulse) {
		dev_err(bd->dev, "only one rohm,rstbmode-* may be specified");
=======
	bdreg->rstbmode_level = of_property_read_bool(config.dev->of_node,
						      "rohm,rstbmode-level");
	bdreg->rstbmode_pulse = of_property_read_bool(config.dev->of_node,
						      "rohm,rstbmode-pulse");
	if (bdreg->rstbmode_level && bdreg->rstbmode_pulse) {
		dev_err(&pdev->dev, "only one rohm,rstbmode-* may be specified");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

#ifdef CONFIG_PM_SLEEP
	if (bdreg->bkup_mode_cnt_keepon) {
		int ret;

		/*
		 * Backup mode is enabled by default in pulse mode, but needs
		 * explicit user setup in level mode.
		 */
		bdreg->bkup_mode_enabled = bdreg->rstbmode_pulse;

		ret = device_create_file(&pdev->dev, &dev_attr_backup_mode);
		if (ret)
			return ret;
	}
#endif /* CONFIG_PM_SLEEP */

	return 0;
}

static const struct platform_device_id bd9571mwv_regulator_id_table[] = {
<<<<<<< HEAD
	{ "bd9571mwv-regulator", },
=======
	{ "bd9571mwv-regulator", ROHM_CHIP_TYPE_BD9571 },
	{ "bd9574mwf-regulator", ROHM_CHIP_TYPE_BD9574 },
>>>>>>> upstream/android-13
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(platform, bd9571mwv_regulator_id_table);

static struct platform_driver bd9571mwv_regulator_driver = {
	.driver = {
		.name = "bd9571mwv-regulator",
		.pm = DEV_PM_OPS,
	},
	.probe = bd9571mwv_regulator_probe,
	.remove = bd9571mwv_regulator_remove,
	.id_table = bd9571mwv_regulator_id_table,
};
module_platform_driver(bd9571mwv_regulator_driver);

MODULE_AUTHOR("Marek Vasut <marek.vasut+renesas@gmail.com>");
MODULE_DESCRIPTION("BD9571MWV Regulator driver");
MODULE_LICENSE("GPL v2");
