<<<<<<< HEAD
/*
 * Copyright (C) 2017 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2017 Spreadtrum Communications Inc.
>>>>>>> upstream/android-13
 */

#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mfd/core.h>
<<<<<<< HEAD
#include <linux/of_device.h>
#include <linux/regmap.h>
#include <linux/spi/spi.h>
=======
#include <linux/mfd/sc27xx-pmic.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/regmap.h>
#include <linux/spi/spi.h>
#include <uapi/linux/usb/charger.h>
>>>>>>> upstream/android-13

#define SPRD_PMIC_INT_MASK_STATUS	0x0
#define SPRD_PMIC_INT_RAW_STATUS	0x4
#define SPRD_PMIC_INT_EN		0x8

#define SPRD_SC2731_IRQ_BASE		0x140
#define SPRD_SC2731_IRQ_NUMS		16
<<<<<<< HEAD
=======
#define SPRD_SC2731_CHG_DET		0xedc

/* PMIC charger detection definition */
#define SPRD_PMIC_CHG_DET_DELAY_US	200000
#define SPRD_PMIC_CHG_DET_TIMEOUT	2000000
#define SPRD_PMIC_CHG_DET_DONE		BIT(11)
#define SPRD_PMIC_SDP_TYPE		BIT(7)
#define SPRD_PMIC_DCP_TYPE		BIT(6)
#define SPRD_PMIC_CDP_TYPE		BIT(5)
#define SPRD_PMIC_CHG_TYPE_MASK		GENMASK(7, 5)
>>>>>>> upstream/android-13

struct sprd_pmic {
	struct regmap *regmap;
	struct device *dev;
	struct regmap_irq *irqs;
	struct regmap_irq_chip irq_chip;
	struct regmap_irq_chip_data *irq_data;
<<<<<<< HEAD
=======
	const struct sprd_pmic_data *pdata;
>>>>>>> upstream/android-13
	int irq;
};

struct sprd_pmic_data {
	u32 irq_base;
	u32 num_irqs;
<<<<<<< HEAD
=======
	u32 charger_det;
>>>>>>> upstream/android-13
};

/*
 * Since different PMICs of SC27xx series can have different interrupt
 * base address and irq number, we should save irq number and irq base
 * in the device data structure.
 */
static const struct sprd_pmic_data sc2731_data = {
	.irq_base = SPRD_SC2731_IRQ_BASE,
	.num_irqs = SPRD_SC2731_IRQ_NUMS,
<<<<<<< HEAD
};

static const struct mfd_cell sprd_pmic_devs[] = {
	{
		.name = "sc27xx-wdt",
		.of_compatible = "sprd,sc27xx-wdt",
	}, {
		.name = "sc27xx-rtc",
		.of_compatible = "sprd,sc27xx-rtc",
	}, {
		.name = "sc27xx-charger",
		.of_compatible = "sprd,sc27xx-charger",
	}, {
		.name = "sc27xx-chg-timer",
		.of_compatible = "sprd,sc27xx-chg-timer",
	}, {
		.name = "sc27xx-fast-chg",
		.of_compatible = "sprd,sc27xx-fast-chg",
	}, {
		.name = "sc27xx-chg-wdt",
		.of_compatible = "sprd,sc27xx-chg-wdt",
	}, {
		.name = "sc27xx-typec",
		.of_compatible = "sprd,sc27xx-typec",
	}, {
		.name = "sc27xx-flash",
		.of_compatible = "sprd,sc27xx-flash",
	}, {
		.name = "sc27xx-eic",
		.of_compatible = "sprd,sc27xx-eic",
	}, {
		.name = "sc27xx-efuse",
		.of_compatible = "sprd,sc27xx-efuse",
	}, {
		.name = "sc27xx-thermal",
		.of_compatible = "sprd,sc27xx-thermal",
	}, {
		.name = "sc27xx-adc",
		.of_compatible = "sprd,sc27xx-adc",
	}, {
		.name = "sc27xx-audio-codec",
		.of_compatible = "sprd,sc27xx-audio-codec",
	}, {
		.name = "sc27xx-regulator",
		.of_compatible = "sprd,sc27xx-regulator",
	}, {
		.name = "sc27xx-vibrator",
		.of_compatible = "sprd,sc27xx-vibrator",
	}, {
		.name = "sc27xx-keypad-led",
		.of_compatible = "sprd,sc27xx-keypad-led",
	}, {
		.name = "sc27xx-bltc",
		.of_compatible = "sprd,sc27xx-bltc",
	}, {
		.name = "sc27xx-fgu",
		.of_compatible = "sprd,sc27xx-fgu",
	}, {
		.name = "sc27xx-7sreset",
		.of_compatible = "sprd,sc27xx-7sreset",
	}, {
		.name = "sc27xx-poweroff",
		.of_compatible = "sprd,sc27xx-poweroff",
	}, {
		.name = "sc27xx-syscon",
		.of_compatible = "sprd,sc27xx-syscon",
	},
};
=======
	.charger_det = SPRD_SC2731_CHG_DET,
};

enum usb_charger_type sprd_pmic_detect_charger_type(struct device *dev)
{
	struct spi_device *spi = to_spi_device(dev);
	struct sprd_pmic *ddata = spi_get_drvdata(spi);
	const struct sprd_pmic_data *pdata = ddata->pdata;
	enum usb_charger_type type;
	u32 val;
	int ret;

	ret = regmap_read_poll_timeout(ddata->regmap, pdata->charger_det, val,
				       (val & SPRD_PMIC_CHG_DET_DONE),
				       SPRD_PMIC_CHG_DET_DELAY_US,
				       SPRD_PMIC_CHG_DET_TIMEOUT);
	if (ret) {
		dev_err(&spi->dev, "failed to detect charger type\n");
		return UNKNOWN_TYPE;
	}

	switch (val & SPRD_PMIC_CHG_TYPE_MASK) {
	case SPRD_PMIC_CDP_TYPE:
		type = CDP_TYPE;
		break;
	case SPRD_PMIC_DCP_TYPE:
		type = DCP_TYPE;
		break;
	case SPRD_PMIC_SDP_TYPE:
		type = SDP_TYPE;
		break;
	default:
		type = UNKNOWN_TYPE;
		break;
	}

	return type;
}
EXPORT_SYMBOL_GPL(sprd_pmic_detect_charger_type);
>>>>>>> upstream/android-13

static int sprd_pmic_spi_write(void *context, const void *data, size_t count)
{
	struct device *dev = context;
	struct spi_device *spi = to_spi_device(dev);

	return spi_write(spi, data, count);
}

static int sprd_pmic_spi_read(void *context,
			      const void *reg, size_t reg_size,
			      void *val, size_t val_size)
{
	struct device *dev = context;
	struct spi_device *spi = to_spi_device(dev);
	u32 rx_buf[2] = { 0 };
	int ret;

	/* Now we only support one PMIC register to read every time. */
	if (reg_size != sizeof(u32) || val_size != sizeof(u32))
		return -EINVAL;

	/* Copy address to read from into first element of SPI buffer. */
	memcpy(rx_buf, reg, sizeof(u32));
	ret = spi_read(spi, rx_buf, 1);
	if (ret < 0)
		return ret;

	memcpy(val, rx_buf, val_size);
	return 0;
}

static struct regmap_bus sprd_pmic_regmap = {
	.write = sprd_pmic_spi_write,
	.read = sprd_pmic_spi_read,
	.reg_format_endian_default = REGMAP_ENDIAN_NATIVE,
	.val_format_endian_default = REGMAP_ENDIAN_NATIVE,
};

static const struct regmap_config sprd_pmic_config = {
	.reg_bits = 32,
	.val_bits = 32,
	.reg_stride = 4,
	.max_register = 0xffff,
};

static int sprd_pmic_probe(struct spi_device *spi)
{
	struct sprd_pmic *ddata;
	const struct sprd_pmic_data *pdata;
	int ret, i;

	pdata = of_device_get_match_data(&spi->dev);
	if (!pdata) {
		dev_err(&spi->dev, "No matching driver data found\n");
		return -EINVAL;
	}

	ddata = devm_kzalloc(&spi->dev, sizeof(*ddata), GFP_KERNEL);
	if (!ddata)
		return -ENOMEM;

	ddata->regmap = devm_regmap_init(&spi->dev, &sprd_pmic_regmap,
					 &spi->dev, &sprd_pmic_config);
	if (IS_ERR(ddata->regmap)) {
		ret = PTR_ERR(ddata->regmap);
		dev_err(&spi->dev, "Failed to allocate register map %d\n", ret);
		return ret;
	}

	spi_set_drvdata(spi, ddata);
	ddata->dev = &spi->dev;
	ddata->irq = spi->irq;
<<<<<<< HEAD
=======
	ddata->pdata = pdata;
>>>>>>> upstream/android-13

	ddata->irq_chip.name = dev_name(&spi->dev);
	ddata->irq_chip.status_base =
		pdata->irq_base + SPRD_PMIC_INT_MASK_STATUS;
	ddata->irq_chip.mask_base = pdata->irq_base + SPRD_PMIC_INT_EN;
	ddata->irq_chip.ack_base = 0;
	ddata->irq_chip.num_regs = 1;
	ddata->irq_chip.num_irqs = pdata->num_irqs;
	ddata->irq_chip.mask_invert = true;

	ddata->irqs = devm_kcalloc(&spi->dev,
				   pdata->num_irqs, sizeof(struct regmap_irq),
				   GFP_KERNEL);
	if (!ddata->irqs)
		return -ENOMEM;

	ddata->irq_chip.irqs = ddata->irqs;
<<<<<<< HEAD
	for (i = 0; i < pdata->num_irqs; i++) {
		ddata->irqs[i].reg_offset = i / pdata->num_irqs;
		ddata->irqs[i].mask = BIT(i % pdata->num_irqs);
	}
=======
	for (i = 0; i < pdata->num_irqs; i++)
		ddata->irqs[i].mask = BIT(i);
>>>>>>> upstream/android-13

	ret = devm_regmap_add_irq_chip(&spi->dev, ddata->regmap, ddata->irq,
				       IRQF_ONESHOT, 0,
				       &ddata->irq_chip, &ddata->irq_data);
	if (ret) {
		dev_err(&spi->dev, "Failed to add PMIC irq chip %d\n", ret);
		return ret;
	}

<<<<<<< HEAD
	ret = devm_mfd_add_devices(&spi->dev, PLATFORM_DEVID_AUTO,
				   sprd_pmic_devs, ARRAY_SIZE(sprd_pmic_devs),
				   NULL, 0,
				   regmap_irq_get_domain(ddata->irq_data));
	if (ret) {
		dev_err(&spi->dev, "Failed to register device %d\n", ret);
=======
	ret = devm_of_platform_populate(&spi->dev);
	if (ret) {
		dev_err(&spi->dev, "Failed to populate sub-devices %d\n", ret);
>>>>>>> upstream/android-13
		return ret;
	}

	device_init_wakeup(&spi->dev, true);
	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int sprd_pmic_suspend(struct device *dev)
{
	struct sprd_pmic *ddata = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		enable_irq_wake(ddata->irq);

	return 0;
}

static int sprd_pmic_resume(struct device *dev)
{
	struct sprd_pmic *ddata = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		disable_irq_wake(ddata->irq);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(sprd_pmic_pm_ops, sprd_pmic_suspend, sprd_pmic_resume);

static const struct of_device_id sprd_pmic_match[] = {
	{ .compatible = "sprd,sc2731", .data = &sc2731_data },
	{},
};
MODULE_DEVICE_TABLE(of, sprd_pmic_match);

<<<<<<< HEAD
static struct spi_driver sprd_pmic_driver = {
	.driver = {
		.name = "sc27xx-pmic",
		.bus = &spi_bus_type,
=======
static const struct spi_device_id sprd_pmic_spi_ids[] = {
	{ .name = "sc2731", .driver_data = (unsigned long)&sc2731_data },
	{},
};
MODULE_DEVICE_TABLE(spi, sprd_pmic_spi_ids);

static struct spi_driver sprd_pmic_driver = {
	.driver = {
		.name = "sc27xx-pmic",
>>>>>>> upstream/android-13
		.of_match_table = sprd_pmic_match,
		.pm = &sprd_pmic_pm_ops,
	},
	.probe = sprd_pmic_probe,
<<<<<<< HEAD
=======
	.id_table = sprd_pmic_spi_ids,
>>>>>>> upstream/android-13
};

static int __init sprd_pmic_init(void)
{
	return spi_register_driver(&sprd_pmic_driver);
}
subsys_initcall(sprd_pmic_init);

static void __exit sprd_pmic_exit(void)
{
	spi_unregister_driver(&sprd_pmic_driver);
}
module_exit(sprd_pmic_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Spreadtrum SC27xx PMICs driver");
MODULE_AUTHOR("Baolin Wang <baolin.wang@spreadtrum.com>");
