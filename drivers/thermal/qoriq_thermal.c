// SPDX-License-Identifier: GPL-2.0
//
// Copyright 2016 Freescale Semiconductor, Inc.

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/thermal.h>

#include "thermal_core.h"

#define SITES_MAX	16

/*
 * QorIQ TMU Registers
 */
struct qoriq_tmu_site_regs {
	u32 tritsr;		/* Immediate Temperature Site Register */
	u32 tratsr;		/* Average Temperature Site Register */
	u8 res0[0x8];
};

struct qoriq_tmu_regs {
	u32 tmr;		/* Mode Register */
#define TMR_DISABLE	0x0
#define TMR_ME		0x80000000
#define TMR_ALPF	0x0c000000
	u32 tsr;		/* Status Register */
	u32 tmtmir;		/* Temperature measurement interval Register */
#define TMTMIR_DEFAULT	0x0000000f
	u8 res0[0x14];
	u32 tier;		/* Interrupt Enable Register */
#define TIER_DISABLE	0x0
	u32 tidr;		/* Interrupt Detect Register */
	u32 tiscr;		/* Interrupt Site Capture Register */
	u32 ticscr;		/* Interrupt Critical Site Capture Register */
	u8 res1[0x10];
	u32 tmhtcrh;		/* High Temperature Capture Register */
	u32 tmhtcrl;		/* Low Temperature Capture Register */
	u8 res2[0x8];
	u32 tmhtitr;		/* High Temperature Immediate Threshold */
	u32 tmhtatr;		/* High Temperature Average Threshold */
	u32 tmhtactr;	/* High Temperature Average Crit Threshold */
	u8 res3[0x24];
	u32 ttcfgr;		/* Temperature Configuration Register */
	u32 tscfgr;		/* Sensor Configuration Register */
	u8 res4[0x78];
	struct qoriq_tmu_site_regs site[SITES_MAX];
	u8 res5[0x9f8];
	u32 ipbrr0;		/* IP Block Revision Register 0 */
	u32 ipbrr1;		/* IP Block Revision Register 1 */
	u8 res6[0x310];
	u32 ttr0cr;		/* Temperature Range 0 Control Register */
	u32 ttr1cr;		/* Temperature Range 1 Control Register */
	u32 ttr2cr;		/* Temperature Range 2 Control Register */
	u32 ttr3cr;		/* Temperature Range 3 Control Register */
};
=======
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/sizes.h>
#include <linux/thermal.h>
#include <linux/units.h>

#include "thermal_core.h"
#include "thermal_hwmon.h"

#define SITES_MAX		16
#define TMR_DISABLE		0x0
#define TMR_ME			0x80000000
#define TMR_ALPF		0x0c000000
#define TMR_ALPF_V2		0x03000000
#define TMTMIR_DEFAULT	0x0000000f
#define TIER_DISABLE	0x0
#define TEUMR0_V2		0x51009c00
#define TMSARA_V2		0xe
#define TMU_VER1		0x1
#define TMU_VER2		0x2

#define REGS_TMR	0x000	/* Mode Register */
#define TMR_DISABLE	0x0
#define TMR_ME		0x80000000
#define TMR_ALPF	0x0c000000
#define TMR_MSITE_ALL	GENMASK(15, 0)

#define REGS_TMTMIR	0x008	/* Temperature measurement interval Register */
#define TMTMIR_DEFAULT	0x0000000f

#define REGS_V2_TMSR	0x008	/* monitor site register */

#define REGS_V2_TMTMIR	0x00c	/* Temperature measurement interval Register */

#define REGS_TIER	0x020	/* Interrupt Enable Register */
#define TIER_DISABLE	0x0


#define REGS_TTCFGR	0x080	/* Temperature Configuration Register */
#define REGS_TSCFGR	0x084	/* Sensor Configuration Register */

#define REGS_TRITSR(n)	(0x100 + 16 * (n)) /* Immediate Temperature
					    * Site Register
					    */
#define TRITSR_V	BIT(31)
#define REGS_V2_TMSAR(n)	(0x304 + 16 * (n))	/* TMU monitoring
						* site adjustment register
						*/
#define REGS_TTRnCR(n)	(0xf10 + 4 * (n)) /* Temperature Range n
					   * Control Register
					   */
#define REGS_IPBRR(n)		(0xbf8 + 4 * (n)) /* IP Block Revision
						   * Register n
						   */
#define REGS_V2_TEUMR(n)	(0xf00 + 4 * (n))
>>>>>>> upstream/android-13

/*
 * Thermal zone data
 */
<<<<<<< HEAD
struct qoriq_tmu_data {
	struct thermal_zone_device *tz;
	struct qoriq_tmu_regs __iomem *regs;
	int sensor_id;
	bool little_endian;
};

static void tmu_write(struct qoriq_tmu_data *p, u32 val, void __iomem *addr)
{
	if (p->little_endian)
		iowrite32(val, addr);
	else
		iowrite32be(val, addr);
}

static u32 tmu_read(struct qoriq_tmu_data *p, void __iomem *addr)
{
	if (p->little_endian)
		return ioread32(addr);
	else
		return ioread32be(addr);
=======
struct qoriq_sensor {
	int				id;
};

struct qoriq_tmu_data {
	int ver;
	struct regmap *regmap;
	struct clk *clk;
	struct qoriq_sensor	sensor[SITES_MAX];
};

static struct qoriq_tmu_data *qoriq_sensor_to_data(struct qoriq_sensor *s)
{
	return container_of(s, struct qoriq_tmu_data, sensor[s->id]);
>>>>>>> upstream/android-13
}

static int tmu_get_temp(void *p, int *temp)
{
<<<<<<< HEAD
	u32 val;
	struct qoriq_tmu_data *data = p;

	val = tmu_read(data, &data->regs->site[data->sensor_id].tritsr);
	*temp = (val & 0xff) * 1000;
=======
	struct qoriq_sensor *qsensor = p;
	struct qoriq_tmu_data *qdata = qoriq_sensor_to_data(qsensor);
	u32 val;
	/*
	 * REGS_TRITSR(id) has the following layout:
	 *
	 * For TMU Rev1:
	 * 31  ... 7 6 5 4 3 2 1 0
	 *  V          TEMP
	 *
	 * Where V bit signifies if the measurement is ready and is
	 * within sensor range. TEMP is an 8 bit value representing
	 * temperature in Celsius.

	 * For TMU Rev2:
	 * 31  ... 8 7 6 5 4 3 2 1 0
	 *  V          TEMP
	 *
	 * Where V bit signifies if the measurement is ready and is
	 * within sensor range. TEMP is an 9 bit value representing
	 * temperature in KelVin.
	 */
	if (regmap_read_poll_timeout(qdata->regmap,
				     REGS_TRITSR(qsensor->id),
				     val,
				     val & TRITSR_V,
				     USEC_PER_MSEC,
				     10 * USEC_PER_MSEC))
		return -ENODATA;

	if (qdata->ver == TMU_VER1)
		*temp = (val & GENMASK(7, 0)) * MILLIDEGREE_PER_DEGREE;
	else
		*temp = kelvin_to_millicelsius(val & GENMASK(8, 0));
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int qoriq_tmu_get_sensor_id(void)
{
	int ret, id;
	struct of_phandle_args sensor_specs;
	struct device_node *np, *sensor_np;

	np = of_find_node_by_name(NULL, "thermal-zones");
	if (!np)
		return -ENODEV;

	sensor_np = of_get_next_child(np, NULL);
	ret = of_parse_phandle_with_args(sensor_np, "thermal-sensors",
			"#thermal-sensor-cells",
			0, &sensor_specs);
	if (ret) {
		of_node_put(np);
		of_node_put(sensor_np);
		return ret;
	}

	if (sensor_specs.args_count >= 1) {
		id = sensor_specs.args[0];
		WARN(sensor_specs.args_count > 1,
				"%s: too many cells in sensor specifier %d\n",
				sensor_specs.np->name, sensor_specs.args_count);
	} else {
		id = 0;
	}

	of_node_put(np);
	of_node_put(sensor_np);

	return id;
}

static int qoriq_tmu_calibration(struct platform_device *pdev)
=======
static const struct thermal_zone_of_device_ops tmu_tz_ops = {
	.get_temp = tmu_get_temp,
};

static int qoriq_tmu_register_tmu_zone(struct device *dev,
				       struct qoriq_tmu_data *qdata)
{
	int id;

	if (qdata->ver == TMU_VER1) {
		regmap_write(qdata->regmap, REGS_TMR,
			     TMR_MSITE_ALL | TMR_ME | TMR_ALPF);
	} else {
		regmap_write(qdata->regmap, REGS_V2_TMSR, TMR_MSITE_ALL);
		regmap_write(qdata->regmap, REGS_TMR, TMR_ME | TMR_ALPF_V2);
	}

	for (id = 0; id < SITES_MAX; id++) {
		struct thermal_zone_device *tzd;
		struct qoriq_sensor *sensor = &qdata->sensor[id];
		int ret;

		sensor->id = id;

		tzd = devm_thermal_zone_of_sensor_register(dev, id,
							   sensor,
							   &tmu_tz_ops);
		ret = PTR_ERR_OR_ZERO(tzd);
		if (ret) {
			if (ret == -ENODEV)
				continue;

			regmap_write(qdata->regmap, REGS_TMR, TMR_DISABLE);
			return ret;
		}

		if (devm_thermal_add_hwmon_sysfs(tzd))
			dev_warn(dev,
				 "Failed to add hwmon sysfs attributes\n");

	}

	return 0;
}

static int qoriq_tmu_calibration(struct device *dev,
				 struct qoriq_tmu_data *data)
>>>>>>> upstream/android-13
{
	int i, val, len;
	u32 range[4];
	const u32 *calibration;
<<<<<<< HEAD
	struct device_node *np = pdev->dev.of_node;
	struct qoriq_tmu_data *data = platform_get_drvdata(pdev);

	if (of_property_read_u32_array(np, "fsl,tmu-range", range, 4)) {
		dev_err(&pdev->dev, "missing calibration range.\n");
		return -ENODEV;
	}

	/* Init temperature range registers */
	tmu_write(data, range[0], &data->regs->ttr0cr);
	tmu_write(data, range[1], &data->regs->ttr1cr);
	tmu_write(data, range[2], &data->regs->ttr2cr);
	tmu_write(data, range[3], &data->regs->ttr3cr);

	calibration = of_get_property(np, "fsl,tmu-calibration", &len);
	if (calibration == NULL || len % 8) {
		dev_err(&pdev->dev, "invalid calibration data.\n");
=======
	struct device_node *np = dev->of_node;

	len = of_property_count_u32_elems(np, "fsl,tmu-range");
	if (len < 0 || len > 4) {
		dev_err(dev, "invalid range data.\n");
		return len;
	}

	val = of_property_read_u32_array(np, "fsl,tmu-range", range, len);
	if (val != 0) {
		dev_err(dev, "failed to read range data.\n");
		return val;
	}

	/* Init temperature range registers */
	for (i = 0; i < len; i++)
		regmap_write(data->regmap, REGS_TTRnCR(i), range[i]);

	calibration = of_get_property(np, "fsl,tmu-calibration", &len);
	if (calibration == NULL || len % 8) {
		dev_err(dev, "invalid calibration data.\n");
>>>>>>> upstream/android-13
		return -ENODEV;
	}

	for (i = 0; i < len; i += 8, calibration += 2) {
		val = of_read_number(calibration, 1);
<<<<<<< HEAD
		tmu_write(data, val, &data->regs->ttcfgr);
		val = of_read_number(calibration + 1, 1);
		tmu_write(data, val, &data->regs->tscfgr);
=======
		regmap_write(data->regmap, REGS_TTCFGR, val);
		val = of_read_number(calibration + 1, 1);
		regmap_write(data->regmap, REGS_TSCFGR, val);
>>>>>>> upstream/android-13
	}

	return 0;
}

static void qoriq_tmu_init_device(struct qoriq_tmu_data *data)
{
<<<<<<< HEAD
	/* Disable interrupt, using polling instead */
	tmu_write(data, TIER_DISABLE, &data->regs->tier);

	/* Set update_interval */
	tmu_write(data, TMTMIR_DEFAULT, &data->regs->tmtmir);

	/* Disable monitoring */
	tmu_write(data, TMR_DISABLE, &data->regs->tmr);
}

static const struct thermal_zone_of_device_ops tmu_tz_ops = {
	.get_temp = tmu_get_temp,
};

static int qoriq_tmu_probe(struct platform_device *pdev)
{
	int ret;
	struct qoriq_tmu_data *data;
	struct device_node *np = pdev->dev.of_node;
	u32 site;

	if (!np) {
		dev_err(&pdev->dev, "Device OF-Node is NULL");
		return -ENODEV;
	}

	data = devm_kzalloc(&pdev->dev, sizeof(struct qoriq_tmu_data),
=======
	int i;

	/* Disable interrupt, using polling instead */
	regmap_write(data->regmap, REGS_TIER, TIER_DISABLE);

	/* Set update_interval */

	if (data->ver == TMU_VER1) {
		regmap_write(data->regmap, REGS_TMTMIR, TMTMIR_DEFAULT);
	} else {
		regmap_write(data->regmap, REGS_V2_TMTMIR, TMTMIR_DEFAULT);
		regmap_write(data->regmap, REGS_V2_TEUMR(0), TEUMR0_V2);
		for (i = 0; i < SITES_MAX; i++)
			regmap_write(data->regmap, REGS_V2_TMSAR(i), TMSARA_V2);
	}

	/* Disable monitoring */
	regmap_write(data->regmap, REGS_TMR, TMR_DISABLE);
}

static const struct regmap_range qoriq_yes_ranges[] = {
	regmap_reg_range(REGS_TMR, REGS_TSCFGR),
	regmap_reg_range(REGS_TTRnCR(0), REGS_TTRnCR(3)),
	regmap_reg_range(REGS_V2_TEUMR(0), REGS_V2_TEUMR(2)),
	regmap_reg_range(REGS_V2_TMSAR(0), REGS_V2_TMSAR(15)),
	regmap_reg_range(REGS_IPBRR(0), REGS_IPBRR(1)),
	/* Read only registers below */
	regmap_reg_range(REGS_TRITSR(0), REGS_TRITSR(15)),
};

static const struct regmap_access_table qoriq_wr_table = {
	.yes_ranges	= qoriq_yes_ranges,
	.n_yes_ranges	= ARRAY_SIZE(qoriq_yes_ranges) - 1,
};

static const struct regmap_access_table qoriq_rd_table = {
	.yes_ranges	= qoriq_yes_ranges,
	.n_yes_ranges	= ARRAY_SIZE(qoriq_yes_ranges),
};

static void qoriq_tmu_action(void *p)
{
	struct qoriq_tmu_data *data = p;

	regmap_write(data->regmap, REGS_TMR, TMR_DISABLE);
	clk_disable_unprepare(data->clk);
}

static int qoriq_tmu_probe(struct platform_device *pdev)
{
	int ret;
	u32 ver;
	struct qoriq_tmu_data *data;
	struct device_node *np = pdev->dev.of_node;
	struct device *dev = &pdev->dev;
	const bool little_endian = of_property_read_bool(np, "little-endian");
	const enum regmap_endian format_endian =
		little_endian ? REGMAP_ENDIAN_LITTLE : REGMAP_ENDIAN_BIG;
	const struct regmap_config regmap_config = {
		.reg_bits		= 32,
		.val_bits		= 32,
		.reg_stride		= 4,
		.rd_table		= &qoriq_rd_table,
		.wr_table		= &qoriq_wr_table,
		.val_format_endian	= format_endian,
		.max_register		= SZ_4K,
	};
	void __iomem *base;

	data = devm_kzalloc(dev, sizeof(struct qoriq_tmu_data),
>>>>>>> upstream/android-13
			    GFP_KERNEL);
	if (!data)
		return -ENOMEM;

<<<<<<< HEAD
	platform_set_drvdata(pdev, data);

	data->little_endian = of_property_read_bool(np, "little-endian");

	data->sensor_id = qoriq_tmu_get_sensor_id();
	if (data->sensor_id < 0) {
		dev_err(&pdev->dev, "Failed to get sensor id\n");
		ret = -ENODEV;
		goto err_iomap;
	}

	data->regs = of_iomap(np, 0);
	if (!data->regs) {
		dev_err(&pdev->dev, "Failed to get memory region\n");
		ret = -ENODEV;
		goto err_iomap;
	}

	qoriq_tmu_init_device(data);	/* TMU initialization */

	ret = qoriq_tmu_calibration(pdev);	/* TMU calibration */
	if (ret < 0)
		goto err_tmu;

	data->tz = devm_thermal_zone_of_sensor_register(&pdev->dev,
							data->sensor_id,
							data, &tmu_tz_ops);
	if (IS_ERR(data->tz)) {
		ret = PTR_ERR(data->tz);
		dev_err(&pdev->dev,
			"Failed to register thermal zone device %d\n", ret);
		goto err_tmu;
	}

	/* Enable monitoring */
	site = 0x1 << (15 - data->sensor_id);
	tmu_write(data, site | TMR_ME | TMR_ALPF, &data->regs->tmr);

	return 0;

err_tmu:
	iounmap(data->regs);

err_iomap:
	platform_set_drvdata(pdev, NULL);

	return ret;
}

static int qoriq_tmu_remove(struct platform_device *pdev)
{
	struct qoriq_tmu_data *data = platform_get_drvdata(pdev);

	/* Disable monitoring */
	tmu_write(data, TMR_DISABLE, &data->regs->tmr);

	iounmap(data->regs);
	platform_set_drvdata(pdev, NULL);
=======
	base = devm_platform_ioremap_resource(pdev, 0);
	ret = PTR_ERR_OR_ZERO(base);
	if (ret) {
		dev_err(dev, "Failed to get memory region\n");
		return ret;
	}

	data->regmap = devm_regmap_init_mmio(dev, base, &regmap_config);
	ret = PTR_ERR_OR_ZERO(data->regmap);
	if (ret) {
		dev_err(dev, "Failed to init regmap (%d)\n", ret);
		return ret;
	}

	data->clk = devm_clk_get_optional(dev, NULL);
	if (IS_ERR(data->clk))
		return PTR_ERR(data->clk);

	ret = clk_prepare_enable(data->clk);
	if (ret) {
		dev_err(dev, "Failed to enable clock\n");
		return ret;
	}

	ret = devm_add_action_or_reset(dev, qoriq_tmu_action, data);
	if (ret)
		return ret;

	/* version register offset at: 0xbf8 on both v1 and v2 */
	ret = regmap_read(data->regmap, REGS_IPBRR(0), &ver);
	if (ret) {
		dev_err(&pdev->dev, "Failed to read IP block version\n");
		return ret;
	}
	data->ver = (ver >> 8) & 0xff;

	qoriq_tmu_init_device(data);	/* TMU initialization */

	ret = qoriq_tmu_calibration(dev, data);	/* TMU calibration */
	if (ret < 0)
		return ret;

	ret = qoriq_tmu_register_tmu_zone(dev, data);
	if (ret < 0) {
		dev_err(dev, "Failed to register sensors\n");
		return ret;
	}

	platform_set_drvdata(pdev, data);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM_SLEEP
static int qoriq_tmu_suspend(struct device *dev)
{
	u32 tmr;
	struct qoriq_tmu_data *data = dev_get_drvdata(dev);

	/* Disable monitoring */
	tmr = tmu_read(data, &data->regs->tmr);
	tmr &= ~TMR_ME;
	tmu_write(data, tmr, &data->regs->tmr);

	return 0;
}

static int qoriq_tmu_resume(struct device *dev)
{
	u32 tmr;
	struct qoriq_tmu_data *data = dev_get_drvdata(dev);

	/* Enable monitoring */
	tmr = tmu_read(data, &data->regs->tmr);
	tmr |= TMR_ME;
	tmu_write(data, tmr, &data->regs->tmr);

	return 0;
}
#endif
=======
static int __maybe_unused qoriq_tmu_suspend(struct device *dev)
{
	struct qoriq_tmu_data *data = dev_get_drvdata(dev);
	int ret;

	ret = regmap_update_bits(data->regmap, REGS_TMR, TMR_ME, 0);
	if (ret)
		return ret;

	clk_disable_unprepare(data->clk);

	return 0;
}

static int __maybe_unused qoriq_tmu_resume(struct device *dev)
{
	int ret;
	struct qoriq_tmu_data *data = dev_get_drvdata(dev);

	ret = clk_prepare_enable(data->clk);
	if (ret)
		return ret;

	/* Enable monitoring */
	return regmap_update_bits(data->regmap, REGS_TMR, TMR_ME, TMR_ME);
}
>>>>>>> upstream/android-13

static SIMPLE_DEV_PM_OPS(qoriq_tmu_pm_ops,
			 qoriq_tmu_suspend, qoriq_tmu_resume);

static const struct of_device_id qoriq_tmu_match[] = {
	{ .compatible = "fsl,qoriq-tmu", },
<<<<<<< HEAD
=======
	{ .compatible = "fsl,imx8mq-tmu", },
>>>>>>> upstream/android-13
	{},
};
MODULE_DEVICE_TABLE(of, qoriq_tmu_match);

static struct platform_driver qoriq_tmu = {
	.driver	= {
		.name		= "qoriq_thermal",
		.pm		= &qoriq_tmu_pm_ops,
		.of_match_table	= qoriq_tmu_match,
	},
	.probe	= qoriq_tmu_probe,
<<<<<<< HEAD
	.remove	= qoriq_tmu_remove,
=======
>>>>>>> upstream/android-13
};
module_platform_driver(qoriq_tmu);

MODULE_AUTHOR("Jia Hongtao <hongtao.jia@nxp.com>");
MODULE_DESCRIPTION("QorIQ Thermal Monitoring Unit driver");
MODULE_LICENSE("GPL v2");
