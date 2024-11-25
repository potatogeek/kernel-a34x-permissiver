// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

=======
//
// Copyright (c) 2020 MediaTek Inc.
>>>>>>> upstream/android-13

#include <linux/err.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/of_regulator.h>
#include <linux/soc/mediatek/mtk_dvfsrc.h>

#define DVFSRC_ID_VCORE		0
#define DVFSRC_ID_VSCP		1

<<<<<<< HEAD
=======
#define MT_DVFSRC_REGULAR(match, _name,	_volt_table)	\
[DVFSRC_ID_##_name] = {					\
	.desc = {					\
		.name = match,				\
		.of_match = of_match_ptr(match),	\
		.ops = &dvfsrc_vcore_ops,		\
		.type = REGULATOR_VOLTAGE,		\
		.id = DVFSRC_ID_##_name,		\
		.owner = THIS_MODULE,			\
		.n_voltages = ARRAY_SIZE(_volt_table),	\
		.volt_table = _volt_table,		\
	},	\
}
>>>>>>> upstream/android-13

/*
 * DVFSRC regulators' information
 *
 * @desc: standard fields of regulator description.
 * @voltage_selector:  Selector used for get_voltage_sel() and
 *			   set_voltage_sel() callbacks
 */
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
struct dvfsrc_regulator {
	struct regulator_desc	desc;
};

/*
 * MTK DVFSRC regulators' init data
 *
 * @size: num of regulators
 * @regulator_info: regulator info.
 */
struct dvfsrc_regulator_init_data {
	u32 size;
	struct dvfsrc_regulator *regulator_info;
};

static inline struct device *to_dvfsrc_dev(struct regulator_dev *rdev)
{
<<<<<<< HEAD
	return rdev_get_dev(rdev)->parent->parent;
}

static int dvfsrc_set_voltage_sel(struct regulator_dev *rdev,
				   unsigned int selector)
=======
	return rdev_get_dev(rdev)->parent;
}

static int dvfsrc_set_voltage_sel(struct regulator_dev *rdev,
				  unsigned int selector)
>>>>>>> upstream/android-13
{
	struct device *dvfsrc_dev = to_dvfsrc_dev(rdev);
	int id = rdev_get_id(rdev);

<<<<<<< HEAD
	switch (id) {
	case DVFSRC_ID_VCORE:
		mtk_dvfsrc_send_request(dvfsrc_dev,
					MTK_DVFSRC_CMD_VCORE_REQUEST,
					selector);
	break;
	case DVFSRC_ID_VSCP:
		mtk_dvfsrc_send_request(dvfsrc_dev,
					MTK_DVFSRC_CMD_VSCP_REQUEST,
					selector);
	break;
	default:
		return -EINVAL;
	}
=======
	if (id == DVFSRC_ID_VCORE)
		mtk_dvfsrc_send_request(dvfsrc_dev,
					MTK_DVFSRC_CMD_VCORE_REQUEST,
					selector);
	else if (id == DVFSRC_ID_VSCP)
		mtk_dvfsrc_send_request(dvfsrc_dev,
					MTK_DVFSRC_CMD_VSCP_REQUEST,
					selector);
	else
		return -EINVAL;
>>>>>>> upstream/android-13

	return 0;
}

static int dvfsrc_get_voltage_sel(struct regulator_dev *rdev)
{
	struct device *dvfsrc_dev = to_dvfsrc_dev(rdev);
	int id = rdev_get_id(rdev);
	int val, ret;

<<<<<<< HEAD
	switch (id) {
	case DVFSRC_ID_VCORE:
		ret = mtk_dvfsrc_query_info(dvfsrc_dev,
				MTK_DVFSRC_CMD_VCORE_QUERY, &val);
	break;
	case DVFSRC_ID_VSCP:
		ret = mtk_dvfsrc_query_info(dvfsrc_dev,
				MTK_DVFSRC_CMD_VCP_QUERY, &val);
	break;
	default:
		return -EINVAL;
	}
=======
	if (id == DVFSRC_ID_VCORE)
		ret = mtk_dvfsrc_query_info(dvfsrc_dev,
					    MTK_DVFSRC_CMD_VCORE_LEVEL_QUERY,
					    &val);
	else if (id == DVFSRC_ID_VSCP)
		ret = mtk_dvfsrc_query_info(dvfsrc_dev,
					    MTK_DVFSRC_CMD_VSCP_LEVEL_QUERY,
					    &val);
	else
		return -EINVAL;
>>>>>>> upstream/android-13

	if (ret != 0)
		return ret;

	return val;
}

<<<<<<< HEAD
static struct regulator_ops dvfsrc_vcore_ops = {
=======
static const struct regulator_ops dvfsrc_vcore_ops = {
>>>>>>> upstream/android-13
	.list_voltage = regulator_list_voltage_table,
	.get_voltage_sel = dvfsrc_get_voltage_sel,
	.set_voltage_sel = dvfsrc_set_voltage_sel,
};

<<<<<<< HEAD
#define MT_DVFSRC_REGULAR(match, _name,	_volt_table)	\
[DVFSRC_ID_##_name] = {					\
	.desc = {					\
		.name = match,				\
		.of_match = of_match_ptr(match),	\
		.ops = &dvfsrc_vcore_ops,		\
		.type = REGULATOR_VOLTAGE,		\
		.id = DVFSRC_ID_##_name,		\
		.owner = THIS_MODULE,			\
		.n_voltages = ARRAY_SIZE(_volt_table),	\
		.volt_table = _volt_table,		\
	},	\
}

static const unsigned int mt6779_voltages[] = {
	650000,
	725000,
	825000,
};

static struct dvfsrc_regulator mt6779_regulators[] = {
	MT_DVFSRC_REGULAR("dvfsrc-vcore", VCORE,
		mt6779_voltages),
	MT_DVFSRC_REGULAR("dvfsrc-vscp", VSCP,
		mt6779_voltages),
};

static const struct dvfsrc_regulator_init_data regulator_mt6779_data = {
	.size = ARRAY_SIZE(mt6779_regulators),
	.regulator_info = &mt6779_regulators[0],
};

static const unsigned int mt6761_voltages[] = {
	650000,
	0,
	700000,
	800000,
};

static struct dvfsrc_regulator mt6761_regulators[] = {
	MT_DVFSRC_REGULAR("dvfsrc-vcore", VCORE,
		mt6761_voltages),
	MT_DVFSRC_REGULAR("dvfsrc-vscp", VSCP,
		mt6761_voltages),
};

static const struct dvfsrc_regulator_init_data regulator_mt6761_data = {
	.size = ARRAY_SIZE(mt6761_regulators),
	.regulator_info = &mt6761_regulators[0],
};

static int dvfsrc_vcore_regulator_probe(struct platform_device *pdev)
{
=======
static const unsigned int mt8183_voltages[] = {
	725000,
	800000,
};

static struct dvfsrc_regulator mt8183_regulators[] = {
	MT_DVFSRC_REGULAR("dvfsrc-vcore", VCORE,
			  mt8183_voltages),
};

static const struct dvfsrc_regulator_init_data regulator_mt8183_data = {
	.size = ARRAY_SIZE(mt8183_regulators),
	.regulator_info = &mt8183_regulators[0],
};

static const unsigned int mt6873_voltages[] = {
	575000,
	600000,
	650000,
	725000,
};

static struct dvfsrc_regulator mt6873_regulators[] = {
	MT_DVFSRC_REGULAR("dvfsrc-vcore", VCORE,
			  mt6873_voltages),
	MT_DVFSRC_REGULAR("dvfsrc-vscp", VSCP,
			  mt6873_voltages),
};

static const struct dvfsrc_regulator_init_data regulator_mt6873_data = {
	.size = ARRAY_SIZE(mt6873_regulators),
	.regulator_info = &mt6873_regulators[0],
};

static const struct of_device_id mtk_dvfsrc_regulator_match[] = {
	{
		.compatible = "mediatek,mt8183-dvfsrc",
		.data = &regulator_mt8183_data,
	}, {
		.compatible = "mediatek,mt8192-dvfsrc",
		.data = &regulator_mt6873_data,
	}, {
		.compatible = "mediatek,mt6873-dvfsrc",
		.data = &regulator_mt6873_data,
	}, {
		/* sentinel */
	},
};
MODULE_DEVICE_TABLE(of, mtk_dvfsrc_regulator_match);

static int dvfsrc_vcore_regulator_probe(struct platform_device *pdev)
{
	const struct of_device_id *match;
	struct device *dev = &pdev->dev;
>>>>>>> upstream/android-13
	struct regulator_config config = { };
	struct regulator_dev *rdev;
	const struct dvfsrc_regulator_init_data *regulator_init_data;
	struct dvfsrc_regulator *mt_regulators;
	int i;

<<<<<<< HEAD
	regulator_init_data = of_device_get_match_data(&pdev->dev);
	if (!regulator_init_data)
		return -EINVAL;

	mt_regulators = regulator_init_data->regulator_info;

	for (i = 0; i < regulator_init_data->size; i++) {
		config.dev = &pdev->dev;
		config.driver_data = (mt_regulators + i);
		rdev = devm_regulator_register(&pdev->dev,
				&(mt_regulators + i)->desc, &config);
		if (IS_ERR(rdev)) {
			dev_notice(&pdev->dev, "failed to register %s\n",
=======
	match = of_match_node(mtk_dvfsrc_regulator_match, dev->parent->of_node);

	if (!match) {
		dev_err(dev, "invalid compatible string\n");
		return -ENODEV;
	}

	regulator_init_data = match->data;

	mt_regulators = regulator_init_data->regulator_info;
	for (i = 0; i < regulator_init_data->size; i++) {
		config.dev = dev->parent;
		config.driver_data = (mt_regulators + i);
		rdev = devm_regulator_register(dev, &(mt_regulators + i)->desc,
					       &config);
		if (IS_ERR(rdev)) {
			dev_err(dev, "failed to register %s\n",
>>>>>>> upstream/android-13
				(mt_regulators + i)->desc.name);
			return PTR_ERR(rdev);
		}
	}

<<<<<<< HEAD
	dev_info(&pdev->dev, "initialized mtk,dvfsrc regulator\n");
	return 0;
}

static const struct of_device_id mtk_dvfsrc_regulator_match[] = {
	{ .compatible = "mediatek,dvfsrc-mt6779-regulator",
		.data = &regulator_mt6779_data },
	{ .compatible = "mediatek,dvfsrc-mt6761-regulator",
		.data = &regulator_mt6761_data },
	{},
};

static struct platform_driver mtk_dvfsrc_regulator_driver = {
	.driver = {
		.name  = "mtk_dvfsrc-regulator",
		.of_match_table = mtk_dvfsrc_regulator_match,
=======
	return 0;
}

static struct platform_driver mtk_dvfsrc_regulator_driver = {
	.driver = {
		.name  = "mtk-dvfsrc-regulator",
>>>>>>> upstream/android-13
	},
	.probe = dvfsrc_vcore_regulator_probe,
};

static int __init mtk_dvfsrc_regulator_init(void)
{
	return platform_driver_register(&mtk_dvfsrc_regulator_driver);
}
subsys_initcall(mtk_dvfsrc_regulator_init);

static void __exit mtk_dvfsrc_regulator_exit(void)
{
	platform_driver_unregister(&mtk_dvfsrc_regulator_driver);
}
module_exit(mtk_dvfsrc_regulator_exit);

<<<<<<< HEAD

MODULE_AUTHOR("Arvin wang <arvin.wang@mediatek.com>");
MODULE_LICENSE("GPL v2");

=======
MODULE_AUTHOR("Arvin wang <arvin.wang@mediatek.com>");
MODULE_LICENSE("GPL v2");
>>>>>>> upstream/android-13
