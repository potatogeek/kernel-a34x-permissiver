<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2017 MediaTek, Inc.
 *
 * Author: Chen Zhong <chen.zhong@mediatek.com>
<<<<<<< HEAD
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/regmap.h>
#include <linux/mfd/mt6323/registers.h>
#include <linux/mfd/mt6359/registers.h>
#include <linux/mfd/mt6397/registers.h>
#include <linux/mfd/mt6397/core.h>
#include <linux/mfd/mt6357/registers.h>
#include <linux/mfd/mt6357/core.h>
=======
 */

#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/mfd/mt6323/registers.h>
#include <linux/mfd/mt6397/core.h>
#include <linux/mfd/mt6397/registers.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>

#define MTK_PMIC_PWRKEY_RST_EN_MASK	0x1
#define MTK_PMIC_PWRKEY_RST_EN_SHIFT	6
#define MTK_PMIC_HOMEKEY_RST_EN_MASK	0x1
#define MTK_PMIC_HOMEKEY_RST_EN_SHIFT	5
#define MTK_PMIC_RST_DU_MASK		0x3
#define MTK_PMIC_RST_DU_SHIFT		8

#define MTK_PMIC_PWRKEY_RST		\
	(MTK_PMIC_PWRKEY_RST_EN_MASK << MTK_PMIC_PWRKEY_RST_EN_SHIFT)
#define MTK_PMIC_HOMEKEY_RST		\
	(MTK_PMIC_HOMEKEY_RST_EN_MASK << MTK_PMIC_HOMEKEY_RST_EN_SHIFT)
>>>>>>> upstream/android-13

#define MTK_PMIC_PWRKEY_INDEX	0
#define MTK_PMIC_HOMEKEY_INDEX	1
#define MTK_PMIC_MAX_KEY_COUNT	2

<<<<<<< HEAD
#define MT6397_PWRKEY_RST_SHIFT		6
#define MT6397_HOMEKEY_RST_SHIFT	5
#define MT6397_RST_DU_SHIFT		8

#define MT6357_PWRKEY_RST_SHIFT		9
#define MT6357_HOMEKEY_RST_SHIFT	8
#define MT6357_RST_DU_SHIFT		    12

#define MT6359_PWRKEY_RST_SHIFT		9
#define MT6359_HOMEKEY_RST_SHIFT	8
#define MT6359_RST_DU_SHIFT			12

#define PWRKEY_RST_EN			0x1
#define HOMEKEY_RST_EN			0x1
#define RST_DU_MASK				0x3
#define INVALID_VALUE			0

=======
>>>>>>> upstream/android-13
struct mtk_pmic_keys_regs {
	u32 deb_reg;
	u32 deb_mask;
	u32 intsel_reg;
	u32 intsel_mask;
};

#define MTK_PMIC_KEYS_REGS(_deb_reg, _deb_mask,		\
	_intsel_reg, _intsel_mask)			\
{							\
	.deb_reg		= _deb_reg,		\
	.deb_mask		= _deb_mask,		\
	.intsel_reg		= _intsel_reg,		\
	.intsel_mask		= _intsel_mask,		\
}

<<<<<<< HEAD
#define RELEASE_IRQ_INTERVAL	2

struct mtk_pmic_regs {
	const struct mtk_pmic_keys_regs keys_regs[MTK_PMIC_MAX_KEY_COUNT];
	bool release_irq;
	u32 pmic_rst_reg;
	u32 pwrkey_rst_shift;
	u32 homekey_rst_shift;
	u32 rst_du_shift;
=======
struct mtk_pmic_regs {
	const struct mtk_pmic_keys_regs keys_regs[MTK_PMIC_MAX_KEY_COUNT];
	u32 pmic_rst_reg;
>>>>>>> upstream/android-13
};

static const struct mtk_pmic_regs mt6397_regs = {
	.keys_regs[MTK_PMIC_PWRKEY_INDEX] =
		MTK_PMIC_KEYS_REGS(MT6397_CHRSTATUS,
		0x8, MT6397_INT_RSV, 0x10),
	.keys_regs[MTK_PMIC_HOMEKEY_INDEX] =
		MTK_PMIC_KEYS_REGS(MT6397_OCSTATUS2,
		0x10, MT6397_INT_RSV, 0x8),
<<<<<<< HEAD
	.release_irq = false,
	.pmic_rst_reg = MT6397_TOP_RST_MISC,
	.pwrkey_rst_shift = MT6397_PWRKEY_RST_SHIFT,
	.homekey_rst_shift = MT6397_HOMEKEY_RST_SHIFT,
	.rst_du_shift = MT6397_RST_DU_SHIFT,
=======
	.pmic_rst_reg = MT6397_TOP_RST_MISC,
>>>>>>> upstream/android-13
};

static const struct mtk_pmic_regs mt6323_regs = {
	.keys_regs[MTK_PMIC_PWRKEY_INDEX] =
		MTK_PMIC_KEYS_REGS(MT6323_CHRSTATUS,
		0x2, MT6323_INT_MISC_CON, 0x10),
	.keys_regs[MTK_PMIC_HOMEKEY_INDEX] =
		MTK_PMIC_KEYS_REGS(MT6323_CHRSTATUS,
		0x4, MT6323_INT_MISC_CON, 0x8),
<<<<<<< HEAD
	.release_irq = false,
	.pmic_rst_reg = MT6323_TOP_RST_MISC,
	.pwrkey_rst_shift = MT6397_PWRKEY_RST_SHIFT,
	.homekey_rst_shift = MT6397_HOMEKEY_RST_SHIFT,
	.rst_du_shift = MT6397_RST_DU_SHIFT,
};

static const struct mtk_pmic_regs mt6359_regs = {
	.keys_regs[MTK_PMIC_PWRKEY_INDEX] =
		MTK_PMIC_KEYS_REGS(MT6359_TOPSTATUS,
		0x2, MT6359_PSC_TOP_INT_CON0, 0x1),
	.keys_regs[MTK_PMIC_HOMEKEY_INDEX] =
		MTK_PMIC_KEYS_REGS(MT6359_TOPSTATUS,
		0x8, MT6359_PSC_TOP_INT_CON0, 0x2),
	.release_irq = true,
	.pmic_rst_reg = MT6359_TOP_RST_MISC,
	.pwrkey_rst_shift = MT6359_PWRKEY_RST_SHIFT,
	.homekey_rst_shift = MT6359_HOMEKEY_RST_SHIFT,
	.rst_du_shift = MT6359_RST_DU_SHIFT,
};

static const struct mtk_pmic_regs mt6357_regs = {
	.keys_regs[MTK_PMIC_PWRKEY_INDEX] =
		MTK_PMIC_KEYS_REGS(MT6357_TOPSTATUS,
		0x2,
		MT6357_PSC_TOP_INT_CON0,
		MT6357_RG_INT_EN_PWRKEY_MASK),
	.keys_regs[MTK_PMIC_HOMEKEY_INDEX] =
		MTK_PMIC_KEYS_REGS(MT6357_TOPSTATUS,
		MT6357_HOMEKEY_DEB_MASK,
		MT6357_PSC_TOP_INT_CON0,
		MT6357_RG_INT_EN_HOMEKEY_MASK),
	.release_irq = true,
	.pmic_rst_reg = MT6357_TOP_RST_MISC,
	.pwrkey_rst_shift = MT6357_PWRKEY_RST_SHIFT,
	.homekey_rst_shift = MT6357_HOMEKEY_RST_SHIFT,
	.rst_du_shift = MT6357_RST_DU_SHIFT,
};


=======
	.pmic_rst_reg = MT6323_TOP_RST_MISC,
};

>>>>>>> upstream/android-13
struct mtk_pmic_keys_info {
	struct mtk_pmic_keys *keys;
	const struct mtk_pmic_keys_regs *regs;
	unsigned int keycode;
	int irq;
<<<<<<< HEAD
	int release_irq_num;
=======
>>>>>>> upstream/android-13
	bool wakeup:1;
};

struct mtk_pmic_keys {
	struct input_dev *input_dev;
	struct device *dev;
	struct regmap *regmap;
	struct mtk_pmic_keys_info keys[MTK_PMIC_MAX_KEY_COUNT];
};

enum mtk_pmic_keys_lp_mode {
	LP_DISABLE,
	LP_ONEKEY,
	LP_TWOKEY,
};

<<<<<<< HEAD
struct mtk_pmic_keys *keys;

static void mtk_pmic_keys_lp_reset_setup(struct mtk_pmic_keys *keys,
		const struct mtk_pmic_regs *pmic_regs)
{
	int ret;
	u32 long_press_mode, long_press_debounce;
	u32 pmic_rst_reg = pmic_regs->pmic_rst_reg;
	u32 pwrkey_rst = PWRKEY_RST_EN << pmic_regs->pwrkey_rst_shift;
	u32 homekey_rst =
		HOMEKEY_RST_EN << pmic_regs->homekey_rst_shift;
=======
static void mtk_pmic_keys_lp_reset_setup(struct mtk_pmic_keys *keys,
		u32 pmic_rst_reg)
{
	int ret;
	u32 long_press_mode, long_press_debounce;
>>>>>>> upstream/android-13

	ret = of_property_read_u32(keys->dev->of_node,
		"power-off-time-sec", &long_press_debounce);
	if (ret)
		long_press_debounce = 0;

	regmap_update_bits(keys->regmap, pmic_rst_reg,
<<<<<<< HEAD
			   RST_DU_MASK << pmic_regs->rst_du_shift,
			   long_press_debounce << pmic_regs->rst_du_shift);
=======
			   MTK_PMIC_RST_DU_MASK << MTK_PMIC_RST_DU_SHIFT,
			   long_press_debounce << MTK_PMIC_RST_DU_SHIFT);
>>>>>>> upstream/android-13

	ret = of_property_read_u32(keys->dev->of_node,
		"mediatek,long-press-mode", &long_press_mode);
	if (ret)
		long_press_mode = LP_DISABLE;

	switch (long_press_mode) {
	case LP_ONEKEY:
		regmap_update_bits(keys->regmap, pmic_rst_reg,
<<<<<<< HEAD
				   pwrkey_rst,
				   pwrkey_rst);
		regmap_update_bits(keys->regmap, pmic_rst_reg,
				   homekey_rst,
=======
				   MTK_PMIC_PWRKEY_RST,
				   MTK_PMIC_PWRKEY_RST);
		regmap_update_bits(keys->regmap, pmic_rst_reg,
				   MTK_PMIC_HOMEKEY_RST,
>>>>>>> upstream/android-13
				   0);
		break;
	case LP_TWOKEY:
		regmap_update_bits(keys->regmap, pmic_rst_reg,
<<<<<<< HEAD
				   pwrkey_rst,
				   pwrkey_rst);
		regmap_update_bits(keys->regmap, pmic_rst_reg,
				   homekey_rst,
				   homekey_rst);
		break;
	case LP_DISABLE:
		regmap_update_bits(keys->regmap, pmic_rst_reg,
				   pwrkey_rst,
				   0);
		regmap_update_bits(keys->regmap, pmic_rst_reg,
				   homekey_rst,
=======
				   MTK_PMIC_PWRKEY_RST,
				   MTK_PMIC_PWRKEY_RST);
		regmap_update_bits(keys->regmap, pmic_rst_reg,
				   MTK_PMIC_HOMEKEY_RST,
				   MTK_PMIC_HOMEKEY_RST);
		break;
	case LP_DISABLE:
		regmap_update_bits(keys->regmap, pmic_rst_reg,
				   MTK_PMIC_PWRKEY_RST,
				   0);
		regmap_update_bits(keys->regmap, pmic_rst_reg,
				   MTK_PMIC_HOMEKEY_RST,
>>>>>>> upstream/android-13
				   0);
		break;
	default:
		break;
	}
}

<<<<<<< HEAD
static irqreturn_t mtk_pmic_keys_release_irq_handler_thread(
				int irq, void *data)
{
	struct mtk_pmic_keys_info *info = data;

	input_report_key(info->keys->input_dev, info->keycode, 0);
	input_sync(info->keys->input_dev);

	dev_dbg(info->keys->dev, "release key =%d using PMIC\n",
			info->keycode);

	return IRQ_HANDLED;
}

=======
>>>>>>> upstream/android-13
static irqreturn_t mtk_pmic_keys_irq_handler_thread(int irq, void *data)
{
	struct mtk_pmic_keys_info *info = data;
	u32 key_deb, pressed;

<<<<<<< HEAD
	if (info->release_irq_num > 0) {
		pressed = 1;
	} else {
		regmap_read(info->keys->regmap, info->regs->deb_reg, &key_deb);
		key_deb &= info->regs->deb_mask;
		pressed = !key_deb;
	}
=======
	regmap_read(info->keys->regmap, info->regs->deb_reg, &key_deb);

	key_deb &= info->regs->deb_mask;

	pressed = !key_deb;
>>>>>>> upstream/android-13

	input_report_key(info->keys->input_dev, info->keycode, pressed);
	input_sync(info->keys->input_dev);

	dev_dbg(info->keys->dev, "(%s) key =%d using PMIC\n",
		 pressed ? "pressed" : "released", info->keycode);

	return IRQ_HANDLED;
}

static int mtk_pmic_key_setup(struct mtk_pmic_keys *keys,
		struct mtk_pmic_keys_info *info)
{
	int ret;

	info->keys = keys;

	ret = regmap_update_bits(keys->regmap, info->regs->intsel_reg,
				 info->regs->intsel_mask,
				 info->regs->intsel_mask);
	if (ret < 0)
		return ret;

	ret = devm_request_threaded_irq(keys->dev, info->irq, NULL,
					mtk_pmic_keys_irq_handler_thread,
					IRQF_ONESHOT | IRQF_TRIGGER_HIGH,
					"mtk-pmic-keys", info);
	if (ret) {
<<<<<<< HEAD
		dev_dbg(keys->dev, "Failed to request IRQ: %d: %d\n",
=======
		dev_err(keys->dev, "Failed to request IRQ: %d: %d\n",
>>>>>>> upstream/android-13
			info->irq, ret);
		return ret;
	}

<<<<<<< HEAD
	if (info->release_irq_num > 0) {
		ret = devm_request_threaded_irq(keys->dev,
				info->release_irq_num,
				NULL, mtk_pmic_keys_release_irq_handler_thread,
				IRQF_ONESHOT | IRQF_TRIGGER_HIGH,
				"mtk-pmic-keys", info);
		if (ret) {
			dev_dbg(keys->dev, "Failed to request IRQ: %d: %d\n",
				info->release_irq_num, ret);
			return ret;
		}
	}

=======
>>>>>>> upstream/android-13
	input_set_capability(keys->input_dev, EV_KEY, info->keycode);

	return 0;
}

<<<<<<< HEAD
int mtk_pmic_pwrkey_status(void)
{
	struct mtk_pmic_keys_info *pwrkey;
	const struct mtk_pmic_keys_regs *regs;
	u32 key_deb, pressed;

	if (!keys)
		return -EINVAL;

	pwrkey = &keys->keys[MTK_PMIC_PWRKEY_INDEX];
	regs = pwrkey->regs;

	regmap_read(keys->regmap, regs->deb_reg, &key_deb);
	dev_info(keys->dev, "Read register 0x%x and mask 0x%x and value: 0x%x\n",
		 regs->deb_reg, regs->deb_mask, key_deb);
	key_deb &= regs->deb_mask;
	pressed = !key_deb;

	dev_info(keys->dev, "%s power key\n", pressed ? "pressed" : "released");

	return pressed;
}
EXPORT_SYMBOL(mtk_pmic_pwrkey_status);

#ifdef CONFIG_SEC_DEBUG
int mtk_pmic_homekey_status(void)
{
	struct mtk_pmic_keys_info *homekey;
	const struct mtk_pmic_keys_regs *regs;
	u32 key_deb, pressed;

	if (!keys)
		return -EINVAL;

	homekey = &keys->keys[MTK_PMIC_HOMEKEY_INDEX];
	regs = homekey->regs;

	regmap_read(keys->regmap, regs->deb_reg, &key_deb);
	dev_info(keys->dev, "Read register 0x%x and mask 0x%x and value: 0x%x\n",
		 regs->deb_reg, regs->deb_mask, key_deb);
	key_deb &= regs->deb_mask;
	pressed = !key_deb;

	dev_info(keys->dev, "%s home key\n", pressed ? "pressed" : "released");

	return pressed;
}
EXPORT_SYMBOL(mtk_pmic_homekey_status);
#endif

=======
>>>>>>> upstream/android-13
static int __maybe_unused mtk_pmic_keys_suspend(struct device *dev)
{
	struct mtk_pmic_keys *keys = dev_get_drvdata(dev);
	int index;

	for (index = 0; index < MTK_PMIC_MAX_KEY_COUNT; index++) {
		if (keys->keys[index].wakeup)
			enable_irq_wake(keys->keys[index].irq);
	}

	return 0;
}

static int __maybe_unused mtk_pmic_keys_resume(struct device *dev)
{
	struct mtk_pmic_keys *keys = dev_get_drvdata(dev);
	int index;

	for (index = 0; index < MTK_PMIC_MAX_KEY_COUNT; index++) {
		if (keys->keys[index].wakeup)
			disable_irq_wake(keys->keys[index].irq);
	}

	return 0;
}

static SIMPLE_DEV_PM_OPS(mtk_pmic_keys_pm_ops, mtk_pmic_keys_suspend,
			mtk_pmic_keys_resume);

static const struct of_device_id of_mtk_pmic_keys_match_tbl[] = {
	{
		.compatible = "mediatek,mt6397-keys",
		.data = &mt6397_regs,
	}, {
		.compatible = "mediatek,mt6323-keys",
		.data = &mt6323_regs,
	}, {
<<<<<<< HEAD
		.compatible = "mediatek,mt6359-keys",
		.data = &mt6359_regs,
	}, {
		.compatible = "mediatek,mt6357-keys",
		.data = &mt6357_regs,
	}, {
=======
>>>>>>> upstream/android-13
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(of, of_mtk_pmic_keys_match_tbl);

static int mtk_pmic_keys_probe(struct platform_device *pdev)
{
	int error, index = 0;
	unsigned int keycount;
	struct mt6397_chip *pmic_chip = dev_get_drvdata(pdev->dev.parent);
	struct device_node *node = pdev->dev.of_node, *child;
<<<<<<< HEAD
=======
	struct mtk_pmic_keys *keys;
>>>>>>> upstream/android-13
	const struct mtk_pmic_regs *mtk_pmic_regs;
	struct input_dev *input_dev;
	const struct of_device_id *of_id =
		of_match_device(of_mtk_pmic_keys_match_tbl, &pdev->dev);

<<<<<<< HEAD
	if (!of_id)
		return -EINVAL;

=======
>>>>>>> upstream/android-13
	keys = devm_kzalloc(&pdev->dev, sizeof(*keys), GFP_KERNEL);
	if (!keys)
		return -ENOMEM;

	keys->dev = &pdev->dev;
	keys->regmap = pmic_chip->regmap;
	mtk_pmic_regs = of_id->data;

	keys->input_dev = input_dev = devm_input_allocate_device(keys->dev);
	if (!input_dev) {
		dev_err(keys->dev, "input allocate device fail.\n");
		return -ENOMEM;
	}

	input_dev->name = "mtk-pmic-keys";
	input_dev->id.bustype = BUS_HOST;
	input_dev->id.vendor = 0x0001;
	input_dev->id.product = 0x0001;
	input_dev->id.version = 0x0001;

<<<<<<< HEAD
	__set_bit(EV_KEY, input_dev->evbit);
=======
>>>>>>> upstream/android-13
	keycount = of_get_available_child_count(node);
	if (keycount > MTK_PMIC_MAX_KEY_COUNT) {
		dev_err(keys->dev, "too many keys defined (%d)\n", keycount);
		return -EINVAL;
	}

	for_each_child_of_node(node, child) {
		keys->keys[index].regs = &mtk_pmic_regs->keys_regs[index];

		keys->keys[index].irq = platform_get_irq(pdev, index);
<<<<<<< HEAD
		if (keys->keys[index].irq < 0)
			return keys->keys[index].irq;

		if (mtk_pmic_regs->release_irq) {
			keys->keys[index].release_irq_num = platform_get_irq(
						pdev,
						index + RELEASE_IRQ_INTERVAL);
			if (keys->keys[index].release_irq_num < 0)
				return keys->keys[index].release_irq_num;
=======
		if (keys->keys[index].irq < 0) {
			of_node_put(child);
			return keys->keys[index].irq;
>>>>>>> upstream/android-13
		}

		error = of_property_read_u32(child,
			"linux,keycodes", &keys->keys[index].keycode);
		if (error) {
			dev_err(keys->dev,
				"failed to read key:%d linux,keycode property: %d\n",
				index, error);
<<<<<<< HEAD
=======
			of_node_put(child);
>>>>>>> upstream/android-13
			return error;
		}

		if (of_property_read_bool(child, "wakeup-source"))
			keys->keys[index].wakeup = true;

		error = mtk_pmic_key_setup(keys, &keys->keys[index]);
		if (error) {
<<<<<<< HEAD
			pr_info("Set key index = %d error.\n", index);
=======
			of_node_put(child);
>>>>>>> upstream/android-13
			return error;
		}

		index++;
	}

	error = input_register_device(input_dev);
	if (error) {
		dev_err(&pdev->dev,
			"register input device failed (%d)\n", error);
		return error;
	}

<<<<<<< HEAD
	mtk_pmic_keys_lp_reset_setup(keys, mtk_pmic_regs);
=======
	mtk_pmic_keys_lp_reset_setup(keys, mtk_pmic_regs->pmic_rst_reg);
>>>>>>> upstream/android-13

	platform_set_drvdata(pdev, keys);

	return 0;
}

static struct platform_driver pmic_keys_pdrv = {
	.probe = mtk_pmic_keys_probe,
	.driver = {
		   .name = "mtk-pmic-keys",
		   .of_match_table = of_mtk_pmic_keys_match_tbl,
		   .pm = &mtk_pmic_keys_pm_ops,
	},
};

module_platform_driver(pmic_keys_pdrv);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Chen Zhong <chen.zhong@mediatek.com>");
MODULE_DESCRIPTION("MTK pmic-keys driver v0.1");
