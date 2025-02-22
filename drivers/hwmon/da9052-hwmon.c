<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * HWMON Driver for Dialog DA9052
 *
 * Copyright(c) 2012 Dialog Semiconductor Ltd.
 *
 * Author: David Dajun Chen <dchen@diasemi.com>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/err.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/property.h>

#include <linux/mfd/da9052/da9052.h>
#include <linux/mfd/da9052/reg.h>
#include <linux/regulator/consumer.h>

struct da9052_hwmon {
	struct da9052		*da9052;
	struct mutex		hwmon_lock;
	bool			tsi_as_adc;
	int			tsiref_mv;
	struct regulator	*tsiref;
	struct completion	tsidone;
};

static const char * const input_names[] = {
	[DA9052_ADC_VDDOUT]	=	"VDDOUT",
	[DA9052_ADC_ICH]	=	"CHARGING CURRENT",
	[DA9052_ADC_TBAT]	=	"BATTERY TEMP",
	[DA9052_ADC_VBAT]	=	"BATTERY VOLTAGE",
	[DA9052_ADC_IN4]	=	"ADC IN4",
	[DA9052_ADC_IN5]	=	"ADC IN5",
	[DA9052_ADC_IN6]	=	"ADC IN6",
	[DA9052_ADC_TSI_XP]	=	"ADC TS X+",
	[DA9052_ADC_TSI_YP]	=	"ADC TS Y+",
	[DA9052_ADC_TSI_XN]	=	"ADC TS X-",
	[DA9052_ADC_TSI_YN]	=	"ADC TS Y-",
	[DA9052_ADC_TJUNC]	=	"BATTERY JUNCTION TEMP",
	[DA9052_ADC_VBBAT]	=	"BACK-UP BATTERY VOLTAGE",
};

/* Conversion function for VDDOUT and VBAT */
static inline int volt_reg_to_mv(int value)
{
	return DIV_ROUND_CLOSEST(value * 2000, 1023) + 2500;
}

/* Conversion function for ADC channels 4, 5 and 6 */
static inline int input_reg_to_mv(int value)
{
	return DIV_ROUND_CLOSEST(value * 2500, 1023);
}

/* Conversion function for VBBAT */
static inline int vbbat_reg_to_mv(int value)
{
	return DIV_ROUND_CLOSEST(value * 5000, 1023);
}

static inline int input_tsireg_to_mv(struct da9052_hwmon *hwmon, int value)
{
	return DIV_ROUND_CLOSEST(value * hwmon->tsiref_mv, 1023);
}

static inline int da9052_enable_vddout_channel(struct da9052 *da9052)
{
	return da9052_reg_update(da9052, DA9052_ADC_CONT_REG,
				 DA9052_ADCCONT_AUTOVDDEN,
				 DA9052_ADCCONT_AUTOVDDEN);
}

static inline int da9052_disable_vddout_channel(struct da9052 *da9052)
{
	return da9052_reg_update(da9052, DA9052_ADC_CONT_REG,
				 DA9052_ADCCONT_AUTOVDDEN, 0);
}

<<<<<<< HEAD
static ssize_t da9052_read_vddout(struct device *dev,
=======
static ssize_t da9052_vddout_show(struct device *dev,
>>>>>>> upstream/android-13
				  struct device_attribute *devattr, char *buf)
{
	struct da9052_hwmon *hwmon = dev_get_drvdata(dev);
	int ret, vdd;

	mutex_lock(&hwmon->hwmon_lock);

	ret = da9052_enable_vddout_channel(hwmon->da9052);
	if (ret < 0)
		goto hwmon_err;

	vdd = da9052_reg_read(hwmon->da9052, DA9052_VDD_RES_REG);
	if (vdd < 0) {
		ret = vdd;
		goto hwmon_err_release;
	}

	ret = da9052_disable_vddout_channel(hwmon->da9052);
	if (ret < 0)
		goto hwmon_err;

	mutex_unlock(&hwmon->hwmon_lock);
	return sprintf(buf, "%d\n", volt_reg_to_mv(vdd));

hwmon_err_release:
	da9052_disable_vddout_channel(hwmon->da9052);
hwmon_err:
	mutex_unlock(&hwmon->hwmon_lock);
	return ret;
}

<<<<<<< HEAD
static ssize_t da9052_read_ich(struct device *dev,
=======
static ssize_t da9052_ich_show(struct device *dev,
>>>>>>> upstream/android-13
			       struct device_attribute *devattr, char *buf)
{
	struct da9052_hwmon *hwmon = dev_get_drvdata(dev);
	int ret;

	ret = da9052_reg_read(hwmon->da9052, DA9052_ICHG_AV_REG);
	if (ret < 0)
		return ret;

	/* Equivalent to 3.9mA/bit in register ICHG_AV */
	return sprintf(buf, "%d\n", DIV_ROUND_CLOSEST(ret * 39, 10));
}

<<<<<<< HEAD
static ssize_t da9052_read_tbat(struct device *dev,
=======
static ssize_t da9052_tbat_show(struct device *dev,
>>>>>>> upstream/android-13
				struct device_attribute *devattr, char *buf)
{
	struct da9052_hwmon *hwmon = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", da9052_adc_read_temp(hwmon->da9052));
}

<<<<<<< HEAD
static ssize_t da9052_read_vbat(struct device *dev,
=======
static ssize_t da9052_vbat_show(struct device *dev,
>>>>>>> upstream/android-13
				struct device_attribute *devattr, char *buf)
{
	struct da9052_hwmon *hwmon = dev_get_drvdata(dev);
	int ret;

	ret = da9052_adc_manual_read(hwmon->da9052, DA9052_ADC_VBAT);
	if (ret < 0)
		return ret;

	return sprintf(buf, "%d\n", volt_reg_to_mv(ret));
}

<<<<<<< HEAD
static ssize_t da9052_read_misc_channel(struct device *dev,
=======
static ssize_t da9052_misc_channel_show(struct device *dev,
>>>>>>> upstream/android-13
					struct device_attribute *devattr,
					char *buf)
{
	struct da9052_hwmon *hwmon = dev_get_drvdata(dev);
	int channel = to_sensor_dev_attr(devattr)->index;
	int ret;

	ret = da9052_adc_manual_read(hwmon->da9052, channel);
	if (ret < 0)
		return ret;

	return sprintf(buf, "%d\n", input_reg_to_mv(ret));
}

static int da9052_request_tsi_read(struct da9052_hwmon *hwmon, int channel)
{
	u8 val = DA9052_TSICONTB_TSIMAN;

	switch (channel) {
	case DA9052_ADC_TSI_XP:
		val |= DA9052_TSICONTB_TSIMUX_XP;
		break;
	case DA9052_ADC_TSI_YP:
		val |= DA9052_TSICONTB_TSIMUX_YP;
		break;
	case DA9052_ADC_TSI_XN:
		val |= DA9052_TSICONTB_TSIMUX_XN;
		break;
	case DA9052_ADC_TSI_YN:
		val |= DA9052_TSICONTB_TSIMUX_YN;
		break;
	}

	return da9052_reg_write(hwmon->da9052, DA9052_TSI_CONT_B_REG, val);
}

static int da9052_get_tsi_result(struct da9052_hwmon *hwmon, int channel)
{
	u8 regs[3];
	int msb, lsb, err;

	/* block read to avoid separation of MSB and LSB */
	err = da9052_group_read(hwmon->da9052, DA9052_TSI_X_MSB_REG,
				ARRAY_SIZE(regs), regs);
	if (err)
		return err;

	switch (channel) {
	case DA9052_ADC_TSI_XP:
	case DA9052_ADC_TSI_XN:
		msb = regs[0] << DA9052_TSILSB_TSIXL_BITS;
		lsb = regs[2] & DA9052_TSILSB_TSIXL;
		lsb >>= DA9052_TSILSB_TSIXL_SHIFT;
		break;
	case DA9052_ADC_TSI_YP:
	case DA9052_ADC_TSI_YN:
		msb = regs[1] << DA9052_TSILSB_TSIYL_BITS;
		lsb = regs[2] & DA9052_TSILSB_TSIYL;
		lsb >>= DA9052_TSILSB_TSIYL_SHIFT;
		break;
	default:
		return -EINVAL;
	}

	return msb | lsb;
}


static ssize_t __da9052_read_tsi(struct device *dev, int channel)
{
	struct da9052_hwmon *hwmon = dev_get_drvdata(dev);
	int ret;

	reinit_completion(&hwmon->tsidone);

	ret = da9052_request_tsi_read(hwmon, channel);
	if (ret < 0)
		return ret;

	/* Wait for an conversion done interrupt */
	if (!wait_for_completion_timeout(&hwmon->tsidone,
					 msecs_to_jiffies(500)))
		return -ETIMEDOUT;

	return da9052_get_tsi_result(hwmon, channel);
}

<<<<<<< HEAD
static ssize_t da9052_read_tsi(struct device *dev,
			       struct device_attribute *devattr,
			       char *buf)
=======
static ssize_t da9052_tsi_show(struct device *dev,
			       struct device_attribute *devattr, char *buf)
>>>>>>> upstream/android-13
{
	struct da9052_hwmon *hwmon = dev_get_drvdata(dev);
	int channel = to_sensor_dev_attr(devattr)->index;
	int ret;

	mutex_lock(&hwmon->da9052->auxadc_lock);
	ret = __da9052_read_tsi(dev, channel);
	mutex_unlock(&hwmon->da9052->auxadc_lock);

	if (ret < 0)
		return ret;
	else
		return sprintf(buf, "%d\n", input_tsireg_to_mv(hwmon, ret));
}

<<<<<<< HEAD
static ssize_t da9052_read_tjunc(struct device *dev,
=======
static ssize_t da9052_tjunc_show(struct device *dev,
>>>>>>> upstream/android-13
				 struct device_attribute *devattr, char *buf)
{
	struct da9052_hwmon *hwmon = dev_get_drvdata(dev);
	int tjunc;
	int toffset;

	tjunc = da9052_reg_read(hwmon->da9052, DA9052_TJUNC_RES_REG);
	if (tjunc < 0)
		return tjunc;

	toffset = da9052_reg_read(hwmon->da9052, DA9052_T_OFFSET_REG);
	if (toffset < 0)
		return toffset;

	/*
	 * Degrees celsius = 1.708 * (TJUNC_RES - T_OFFSET) - 108.8
	 * T_OFFSET is a trim value used to improve accuracy of the result
	 */
	return sprintf(buf, "%d\n", 1708 * (tjunc - toffset) - 108800);
}

<<<<<<< HEAD
static ssize_t da9052_read_vbbat(struct device *dev,
=======
static ssize_t da9052_vbbat_show(struct device *dev,
>>>>>>> upstream/android-13
				 struct device_attribute *devattr, char *buf)
{
	struct da9052_hwmon *hwmon = dev_get_drvdata(dev);
	int ret;

	ret = da9052_adc_manual_read(hwmon->da9052, DA9052_ADC_VBBAT);
	if (ret < 0)
		return ret;

	return sprintf(buf, "%d\n", vbbat_reg_to_mv(ret));
}

<<<<<<< HEAD
static ssize_t show_label(struct device *dev,
=======
static ssize_t label_show(struct device *dev,
>>>>>>> upstream/android-13
			  struct device_attribute *devattr, char *buf)
{
	return sprintf(buf, "%s\n",
		       input_names[to_sensor_dev_attr(devattr)->index]);
}

static umode_t da9052_channel_is_visible(struct kobject *kobj,
					 struct attribute *attr, int index)
{
<<<<<<< HEAD
	struct device *dev = container_of(kobj, struct device, kobj);
=======
	struct device *dev = kobj_to_dev(kobj);
>>>>>>> upstream/android-13
	struct da9052_hwmon *hwmon = dev_get_drvdata(dev);
	struct device_attribute *dattr = container_of(attr,
				struct device_attribute, attr);
	struct sensor_device_attribute *sattr = to_sensor_dev_attr(dattr);

	if (!hwmon->tsi_as_adc) {
		switch (sattr->index) {
		case DA9052_ADC_TSI_XP:
		case DA9052_ADC_TSI_YP:
		case DA9052_ADC_TSI_XN:
		case DA9052_ADC_TSI_YN:
			return 0;
		}
	}

	return attr->mode;
}

<<<<<<< HEAD
static SENSOR_DEVICE_ATTR(in0_input, 0444, da9052_read_vddout, NULL,
			  DA9052_ADC_VDDOUT);
static SENSOR_DEVICE_ATTR(in0_label, 0444, show_label, NULL,
			  DA9052_ADC_VDDOUT);
static SENSOR_DEVICE_ATTR(in3_input, 0444, da9052_read_vbat, NULL,
			  DA9052_ADC_VBAT);
static SENSOR_DEVICE_ATTR(in3_label, 0444, show_label, NULL,
			  DA9052_ADC_VBAT);
static SENSOR_DEVICE_ATTR(in4_input, 0444, da9052_read_misc_channel, NULL,
			  DA9052_ADC_IN4);
static SENSOR_DEVICE_ATTR(in4_label, 0444, show_label, NULL,
			  DA9052_ADC_IN4);
static SENSOR_DEVICE_ATTR(in5_input, 0444, da9052_read_misc_channel, NULL,
			  DA9052_ADC_IN5);
static SENSOR_DEVICE_ATTR(in5_label, 0444, show_label, NULL,
			  DA9052_ADC_IN5);
static SENSOR_DEVICE_ATTR(in6_input, 0444, da9052_read_misc_channel, NULL,
			  DA9052_ADC_IN6);
static SENSOR_DEVICE_ATTR(in6_label, 0444, show_label, NULL,
			  DA9052_ADC_IN6);
static SENSOR_DEVICE_ATTR(in9_input, 0444, da9052_read_vbbat, NULL,
			  DA9052_ADC_VBBAT);
static SENSOR_DEVICE_ATTR(in9_label, 0444, show_label, NULL,
			  DA9052_ADC_VBBAT);

static SENSOR_DEVICE_ATTR(in70_input, 0444, da9052_read_tsi, NULL,
			  DA9052_ADC_TSI_XP);
static SENSOR_DEVICE_ATTR(in70_label, 0444, show_label, NULL,
			  DA9052_ADC_TSI_XP);
static SENSOR_DEVICE_ATTR(in71_input, 0444, da9052_read_tsi, NULL,
			  DA9052_ADC_TSI_XN);
static SENSOR_DEVICE_ATTR(in71_label, 0444, show_label, NULL,
			  DA9052_ADC_TSI_XN);
static SENSOR_DEVICE_ATTR(in72_input, 0444, da9052_read_tsi, NULL,
			  DA9052_ADC_TSI_YP);
static SENSOR_DEVICE_ATTR(in72_label, 0444, show_label, NULL,
			  DA9052_ADC_TSI_YP);
static SENSOR_DEVICE_ATTR(in73_input, 0444, da9052_read_tsi, NULL,
			  DA9052_ADC_TSI_YN);
static SENSOR_DEVICE_ATTR(in73_label, 0444, show_label, NULL,
			  DA9052_ADC_TSI_YN);

static SENSOR_DEVICE_ATTR(curr1_input, 0444, da9052_read_ich, NULL,
			  DA9052_ADC_ICH);
static SENSOR_DEVICE_ATTR(curr1_label, 0444, show_label, NULL,
			  DA9052_ADC_ICH);

static SENSOR_DEVICE_ATTR(temp2_input, 0444, da9052_read_tbat, NULL,
			  DA9052_ADC_TBAT);
static SENSOR_DEVICE_ATTR(temp2_label, 0444, show_label, NULL,
			  DA9052_ADC_TBAT);
static SENSOR_DEVICE_ATTR(temp8_input, 0444, da9052_read_tjunc, NULL,
			  DA9052_ADC_TJUNC);
static SENSOR_DEVICE_ATTR(temp8_label, 0444, show_label, NULL,
			  DA9052_ADC_TJUNC);
=======
static SENSOR_DEVICE_ATTR_RO(in0_input, da9052_vddout, DA9052_ADC_VDDOUT);
static SENSOR_DEVICE_ATTR_RO(in0_label, label, DA9052_ADC_VDDOUT);
static SENSOR_DEVICE_ATTR_RO(in3_input, da9052_vbat, DA9052_ADC_VBAT);
static SENSOR_DEVICE_ATTR_RO(in3_label, label, DA9052_ADC_VBAT);
static SENSOR_DEVICE_ATTR_RO(in4_input, da9052_misc_channel, DA9052_ADC_IN4);
static SENSOR_DEVICE_ATTR_RO(in4_label, label, DA9052_ADC_IN4);
static SENSOR_DEVICE_ATTR_RO(in5_input, da9052_misc_channel, DA9052_ADC_IN5);
static SENSOR_DEVICE_ATTR_RO(in5_label, label, DA9052_ADC_IN5);
static SENSOR_DEVICE_ATTR_RO(in6_input, da9052_misc_channel, DA9052_ADC_IN6);
static SENSOR_DEVICE_ATTR_RO(in6_label, label, DA9052_ADC_IN6);
static SENSOR_DEVICE_ATTR_RO(in9_input, da9052_vbbat, DA9052_ADC_VBBAT);
static SENSOR_DEVICE_ATTR_RO(in9_label, label, DA9052_ADC_VBBAT);

static SENSOR_DEVICE_ATTR_RO(in70_input, da9052_tsi, DA9052_ADC_TSI_XP);
static SENSOR_DEVICE_ATTR_RO(in70_label, label, DA9052_ADC_TSI_XP);
static SENSOR_DEVICE_ATTR_RO(in71_input, da9052_tsi, DA9052_ADC_TSI_XN);
static SENSOR_DEVICE_ATTR_RO(in71_label, label, DA9052_ADC_TSI_XN);
static SENSOR_DEVICE_ATTR_RO(in72_input, da9052_tsi, DA9052_ADC_TSI_YP);
static SENSOR_DEVICE_ATTR_RO(in72_label, label, DA9052_ADC_TSI_YP);
static SENSOR_DEVICE_ATTR_RO(in73_input, da9052_tsi, DA9052_ADC_TSI_YN);
static SENSOR_DEVICE_ATTR_RO(in73_label, label, DA9052_ADC_TSI_YN);

static SENSOR_DEVICE_ATTR_RO(curr1_input, da9052_ich, DA9052_ADC_ICH);
static SENSOR_DEVICE_ATTR_RO(curr1_label, label, DA9052_ADC_ICH);

static SENSOR_DEVICE_ATTR_RO(temp2_input, da9052_tbat, DA9052_ADC_TBAT);
static SENSOR_DEVICE_ATTR_RO(temp2_label, label, DA9052_ADC_TBAT);
static SENSOR_DEVICE_ATTR_RO(temp8_input, da9052_tjunc, DA9052_ADC_TJUNC);
static SENSOR_DEVICE_ATTR_RO(temp8_label, label, DA9052_ADC_TJUNC);
>>>>>>> upstream/android-13

static struct attribute *da9052_attrs[] = {
	&sensor_dev_attr_in0_input.dev_attr.attr,
	&sensor_dev_attr_in0_label.dev_attr.attr,
	&sensor_dev_attr_in3_input.dev_attr.attr,
	&sensor_dev_attr_in3_label.dev_attr.attr,
	&sensor_dev_attr_in4_input.dev_attr.attr,
	&sensor_dev_attr_in4_label.dev_attr.attr,
	&sensor_dev_attr_in5_input.dev_attr.attr,
	&sensor_dev_attr_in5_label.dev_attr.attr,
	&sensor_dev_attr_in6_input.dev_attr.attr,
	&sensor_dev_attr_in6_label.dev_attr.attr,
	&sensor_dev_attr_in70_input.dev_attr.attr,
	&sensor_dev_attr_in70_label.dev_attr.attr,
	&sensor_dev_attr_in71_input.dev_attr.attr,
	&sensor_dev_attr_in71_label.dev_attr.attr,
	&sensor_dev_attr_in72_input.dev_attr.attr,
	&sensor_dev_attr_in72_label.dev_attr.attr,
	&sensor_dev_attr_in73_input.dev_attr.attr,
	&sensor_dev_attr_in73_label.dev_attr.attr,
	&sensor_dev_attr_in9_input.dev_attr.attr,
	&sensor_dev_attr_in9_label.dev_attr.attr,
	&sensor_dev_attr_curr1_input.dev_attr.attr,
	&sensor_dev_attr_curr1_label.dev_attr.attr,
	&sensor_dev_attr_temp2_input.dev_attr.attr,
	&sensor_dev_attr_temp2_label.dev_attr.attr,
	&sensor_dev_attr_temp8_input.dev_attr.attr,
	&sensor_dev_attr_temp8_label.dev_attr.attr,
	NULL
};

static const struct attribute_group da9052_group = {
	.attrs = da9052_attrs,
	.is_visible = da9052_channel_is_visible,
};
__ATTRIBUTE_GROUPS(da9052);

static irqreturn_t da9052_tsi_datardy_irq(int irq, void *data)
{
	struct da9052_hwmon *hwmon = data;

	complete(&hwmon->tsidone);
	return IRQ_HANDLED;
}

static int da9052_hwmon_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct da9052_hwmon *hwmon;
	struct device *hwmon_dev;
	int err;

	hwmon = devm_kzalloc(dev, sizeof(struct da9052_hwmon), GFP_KERNEL);
	if (!hwmon)
		return -ENOMEM;

	platform_set_drvdata(pdev, hwmon);

	mutex_init(&hwmon->hwmon_lock);
	hwmon->da9052 = dev_get_drvdata(pdev->dev.parent);

	init_completion(&hwmon->tsidone);

	hwmon->tsi_as_adc =
		device_property_read_bool(pdev->dev.parent, "dlg,tsi-as-adc");

	if (hwmon->tsi_as_adc) {
		hwmon->tsiref = devm_regulator_get(pdev->dev.parent, "tsiref");
		if (IS_ERR(hwmon->tsiref)) {
			err = PTR_ERR(hwmon->tsiref);
			dev_err(&pdev->dev, "failed to get tsiref: %d", err);
			return err;
		}

		err = regulator_enable(hwmon->tsiref);
		if (err)
			return err;

		hwmon->tsiref_mv = regulator_get_voltage(hwmon->tsiref);
		if (hwmon->tsiref_mv < 0) {
			err = hwmon->tsiref_mv;
			goto exit_regulator;
		}

		/* convert from microvolt (DT) to millivolt (hwmon) */
		hwmon->tsiref_mv /= 1000;

		/* TSIREF limits from datasheet */
		if (hwmon->tsiref_mv < 1800 || hwmon->tsiref_mv > 2600) {
			dev_err(hwmon->da9052->dev, "invalid TSIREF voltage: %d",
				hwmon->tsiref_mv);
			err = -ENXIO;
			goto exit_regulator;
		}

		/* disable touchscreen features */
		da9052_reg_write(hwmon->da9052, DA9052_TSI_CONT_A_REG, 0x00);

		/* Sample every 1ms */
		da9052_reg_update(hwmon->da9052, DA9052_ADC_CONT_REG,
					  DA9052_ADCCONT_ADCMODE,
					  DA9052_ADCCONT_ADCMODE);

		err = da9052_request_irq(hwmon->da9052, DA9052_IRQ_TSIREADY,
					 "tsiready-irq", da9052_tsi_datardy_irq,
					 hwmon);
		if (err) {
			dev_err(&pdev->dev, "Failed to register TSIRDY IRQ: %d",
				err);
			goto exit_regulator;
		}
	}

	hwmon_dev = devm_hwmon_device_register_with_groups(dev, "da9052",
							   hwmon,
							   da9052_groups);
	err = PTR_ERR_OR_ZERO(hwmon_dev);
	if (err)
		goto exit_irq;

	return 0;

exit_irq:
	if (hwmon->tsi_as_adc)
		da9052_free_irq(hwmon->da9052, DA9052_IRQ_TSIREADY, hwmon);
exit_regulator:
	if (hwmon->tsiref)
		regulator_disable(hwmon->tsiref);

	return err;
}

static int da9052_hwmon_remove(struct platform_device *pdev)
{
	struct da9052_hwmon *hwmon = platform_get_drvdata(pdev);

	if (hwmon->tsi_as_adc) {
		da9052_free_irq(hwmon->da9052, DA9052_IRQ_TSIREADY, hwmon);
		regulator_disable(hwmon->tsiref);
	}

	return 0;
}

static struct platform_driver da9052_hwmon_driver = {
	.probe = da9052_hwmon_probe,
	.remove = da9052_hwmon_remove,
	.driver = {
		.name = "da9052-hwmon",
	},
};

module_platform_driver(da9052_hwmon_driver);

MODULE_AUTHOR("David Dajun Chen <dchen@diasemi.com>");
MODULE_DESCRIPTION("DA9052 HWMON driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:da9052-hwmon");
