<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * AXP20x PMIC USB power supply status driver
 *
 * Copyright (C) 2015 Hans de Goede <hdegoede@redhat.com>
 * Copyright (C) 2014 Bruno Prémont <bonbons@linux-vserver.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under  the terms of the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the License, or (at your
 * option) any later version.
 */

#include <linux/device.h>
=======
 */

#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/devm-helpers.h>
>>>>>>> upstream/android-13
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/mfd/axp20x.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
=======
#include <linux/pm.h>
>>>>>>> upstream/android-13
#include <linux/power_supply.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <linux/iio/consumer.h>
<<<<<<< HEAD
=======
#include <linux/workqueue.h>
>>>>>>> upstream/android-13

#define DRVNAME "axp20x-usb-power-supply"

#define AXP20X_PWR_STATUS_VBUS_PRESENT	BIT(5)
#define AXP20X_PWR_STATUS_VBUS_USED	BIT(4)

#define AXP20X_USB_STATUS_VBUS_VALID	BIT(2)

<<<<<<< HEAD
=======
#define AXP20X_VBUS_PATH_SEL		BIT(7)
#define AXP20X_VBUS_PATH_SEL_OFFSET	7

>>>>>>> upstream/android-13
#define AXP20X_VBUS_VHOLD_uV(b)		(4000000 + (((b) >> 3) & 7) * 100000)
#define AXP20X_VBUS_VHOLD_MASK		GENMASK(5, 3)
#define AXP20X_VBUS_VHOLD_OFFSET	3
#define AXP20X_VBUS_CLIMIT_MASK		3
<<<<<<< HEAD
#define AXP20X_VBUC_CLIMIT_900mA	0
#define AXP20X_VBUC_CLIMIT_500mA	1
#define AXP20X_VBUC_CLIMIT_100mA	2
#define AXP20X_VBUC_CLIMIT_NONE		3
=======
#define AXP20X_VBUS_CLIMIT_900mA	0
#define AXP20X_VBUS_CLIMIT_500mA	1
#define AXP20X_VBUS_CLIMIT_100mA	2
#define AXP20X_VBUS_CLIMIT_NONE		3

#define AXP813_VBUS_CLIMIT_900mA	0
#define AXP813_VBUS_CLIMIT_1500mA	1
#define AXP813_VBUS_CLIMIT_2000mA	2
#define AXP813_VBUS_CLIMIT_2500mA	3
>>>>>>> upstream/android-13

#define AXP20X_ADC_EN1_VBUS_CURR	BIT(2)
#define AXP20X_ADC_EN1_VBUS_VOLT	BIT(3)

#define AXP20X_VBUS_MON_VBUS_VALID	BIT(3)

<<<<<<< HEAD
struct axp20x_usb_power {
	struct device_node *np;
=======
#define AXP813_BC_EN		BIT(0)

/*
 * Note do not raise the debounce time, we must report Vusb high within
 * 100ms otherwise we get Vbus errors in musb.
 */
#define DEBOUNCE_TIME			msecs_to_jiffies(50)

struct axp20x_usb_power {
>>>>>>> upstream/android-13
	struct regmap *regmap;
	struct power_supply *supply;
	enum axp20x_variants axp20x_id;
	struct iio_channel *vbus_v;
	struct iio_channel *vbus_i;
<<<<<<< HEAD
};

=======
	struct delayed_work vbus_detect;
	unsigned int old_status;
	unsigned int online;
	unsigned int num_irqs;
	unsigned int irqs[];
};

static bool axp20x_usb_vbus_needs_polling(struct axp20x_usb_power *power)
{
	/*
	 * Polling is only necessary while VBUS is offline. While online, a
	 * present->absent transition implies an online->offline transition
	 * and will trigger the VBUS_REMOVAL IRQ.
	 */
	if (power->axp20x_id >= AXP221_ID && !power->online)
		return true;

	return false;
}

>>>>>>> upstream/android-13
static irqreturn_t axp20x_usb_power_irq(int irq, void *devid)
{
	struct axp20x_usb_power *power = devid;

	power_supply_changed(power->supply);

<<<<<<< HEAD
	return IRQ_HANDLED;
}

=======
	mod_delayed_work(system_power_efficient_wq, &power->vbus_detect, DEBOUNCE_TIME);

	return IRQ_HANDLED;
}

static void axp20x_usb_power_poll_vbus(struct work_struct *work)
{
	struct axp20x_usb_power *power =
		container_of(work, struct axp20x_usb_power, vbus_detect.work);
	unsigned int val;
	int ret;

	ret = regmap_read(power->regmap, AXP20X_PWR_INPUT_STATUS, &val);
	if (ret)
		goto out;

	val &= (AXP20X_PWR_STATUS_VBUS_PRESENT | AXP20X_PWR_STATUS_VBUS_USED);
	if (val != power->old_status)
		power_supply_changed(power->supply);

	power->old_status = val;
	power->online = val & AXP20X_PWR_STATUS_VBUS_USED;

out:
	if (axp20x_usb_vbus_needs_polling(power))
		mod_delayed_work(system_power_efficient_wq, &power->vbus_detect, DEBOUNCE_TIME);
}

static int axp20x_get_current_max(struct axp20x_usb_power *power, int *val)
{
	unsigned int v;
	int ret = regmap_read(power->regmap, AXP20X_VBUS_IPSOUT_MGMT, &v);

	if (ret)
		return ret;

	switch (v & AXP20X_VBUS_CLIMIT_MASK) {
	case AXP20X_VBUS_CLIMIT_100mA:
		if (power->axp20x_id == AXP221_ID)
			*val = -1; /* No 100mA limit */
		else
			*val = 100000;
		break;
	case AXP20X_VBUS_CLIMIT_500mA:
		*val = 500000;
		break;
	case AXP20X_VBUS_CLIMIT_900mA:
		*val = 900000;
		break;
	case AXP20X_VBUS_CLIMIT_NONE:
		*val = -1;
		break;
	}

	return 0;
}

static int axp813_get_current_max(struct axp20x_usb_power *power, int *val)
{
	unsigned int v;
	int ret = regmap_read(power->regmap, AXP20X_VBUS_IPSOUT_MGMT, &v);

	if (ret)
		return ret;

	switch (v & AXP20X_VBUS_CLIMIT_MASK) {
	case AXP813_VBUS_CLIMIT_900mA:
		*val = 900000;
		break;
	case AXP813_VBUS_CLIMIT_1500mA:
		*val = 1500000;
		break;
	case AXP813_VBUS_CLIMIT_2000mA:
		*val = 2000000;
		break;
	case AXP813_VBUS_CLIMIT_2500mA:
		*val = 2500000;
		break;
	}
	return 0;
}

>>>>>>> upstream/android-13
static int axp20x_usb_power_get_property(struct power_supply *psy,
	enum power_supply_property psp, union power_supply_propval *val)
{
	struct axp20x_usb_power *power = power_supply_get_drvdata(psy);
	unsigned int input, v;
	int ret;

	switch (psp) {
	case POWER_SUPPLY_PROP_VOLTAGE_MIN:
		ret = regmap_read(power->regmap, AXP20X_VBUS_IPSOUT_MGMT, &v);
		if (ret)
			return ret;

		val->intval = AXP20X_VBUS_VHOLD_uV(v);
		return 0;
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		if (IS_ENABLED(CONFIG_AXP20X_ADC)) {
			ret = iio_read_channel_processed(power->vbus_v,
							 &val->intval);
			if (ret)
				return ret;

			/*
			 * IIO framework gives mV but Power Supply framework
			 * gives uV.
			 */
			val->intval *= 1000;
			return 0;
		}

		ret = axp20x_read_variable_width(power->regmap,
						 AXP20X_VBUS_V_ADC_H, 12);
		if (ret < 0)
			return ret;

		val->intval = ret * 1700; /* 1 step = 1.7 mV */
		return 0;
	case POWER_SUPPLY_PROP_CURRENT_MAX:
<<<<<<< HEAD
		ret = regmap_read(power->regmap, AXP20X_VBUS_IPSOUT_MGMT, &v);
		if (ret)
			return ret;

		switch (v & AXP20X_VBUS_CLIMIT_MASK) {
		case AXP20X_VBUC_CLIMIT_100mA:
			if (power->axp20x_id == AXP221_ID)
				val->intval = -1; /* No 100mA limit */
			else
				val->intval = 100000;
			break;
		case AXP20X_VBUC_CLIMIT_500mA:
			val->intval = 500000;
			break;
		case AXP20X_VBUC_CLIMIT_900mA:
			val->intval = 900000;
			break;
		case AXP20X_VBUC_CLIMIT_NONE:
			val->intval = -1;
			break;
		}
		return 0;
=======
		if (power->axp20x_id == AXP813_ID)
			return axp813_get_current_max(power, &val->intval);
		return axp20x_get_current_max(power, &val->intval);
>>>>>>> upstream/android-13
	case POWER_SUPPLY_PROP_CURRENT_NOW:
		if (IS_ENABLED(CONFIG_AXP20X_ADC)) {
			ret = iio_read_channel_processed(power->vbus_i,
							 &val->intval);
			if (ret)
				return ret;

			/*
			 * IIO framework gives mA but Power Supply framework
			 * gives uA.
			 */
			val->intval *= 1000;
			return 0;
		}

		ret = axp20x_read_variable_width(power->regmap,
						 AXP20X_VBUS_I_ADC_H, 12);
		if (ret < 0)
			return ret;

		val->intval = ret * 375; /* 1 step = 0.375 mA */
		return 0;
	default:
		break;
	}

	/* All the properties below need the input-status reg value */
	ret = regmap_read(power->regmap, AXP20X_PWR_INPUT_STATUS, &input);
	if (ret)
		return ret;

	switch (psp) {
	case POWER_SUPPLY_PROP_HEALTH:
		if (!(input & AXP20X_PWR_STATUS_VBUS_PRESENT)) {
			val->intval = POWER_SUPPLY_HEALTH_UNKNOWN;
			break;
		}

		val->intval = POWER_SUPPLY_HEALTH_GOOD;

		if (power->axp20x_id == AXP202_ID) {
			ret = regmap_read(power->regmap,
					  AXP20X_USB_OTG_STATUS, &v);
			if (ret)
				return ret;

			if (!(v & AXP20X_USB_STATUS_VBUS_VALID))
				val->intval =
					POWER_SUPPLY_HEALTH_UNSPEC_FAILURE;
		}
		break;
	case POWER_SUPPLY_PROP_PRESENT:
		val->intval = !!(input & AXP20X_PWR_STATUS_VBUS_PRESENT);
		break;
	case POWER_SUPPLY_PROP_ONLINE:
		val->intval = !!(input & AXP20X_PWR_STATUS_VBUS_USED);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD
=======
static int axp813_usb_power_set_online(struct axp20x_usb_power *power,
				       int intval)
{
	int val = !intval << AXP20X_VBUS_PATH_SEL_OFFSET;

	return regmap_update_bits(power->regmap,
				  AXP20X_VBUS_IPSOUT_MGMT,
				  AXP20X_VBUS_PATH_SEL, val);
}

>>>>>>> upstream/android-13
static int axp20x_usb_power_set_voltage_min(struct axp20x_usb_power *power,
					    int intval)
{
	int val;

	switch (intval) {
	case 4000000:
	case 4100000:
	case 4200000:
	case 4300000:
	case 4400000:
	case 4500000:
	case 4600000:
	case 4700000:
		val = (intval - 4000000) / 100000;
		return regmap_update_bits(power->regmap,
					  AXP20X_VBUS_IPSOUT_MGMT,
					  AXP20X_VBUS_VHOLD_MASK,
					  val << AXP20X_VBUS_VHOLD_OFFSET);
	default:
		return -EINVAL;
	}

	return -EINVAL;
}

<<<<<<< HEAD
=======
static int axp813_usb_power_set_current_max(struct axp20x_usb_power *power,
					    int intval)
{
	int val;

	switch (intval) {
	case 900000:
		return regmap_update_bits(power->regmap,
					  AXP20X_VBUS_IPSOUT_MGMT,
					  AXP20X_VBUS_CLIMIT_MASK,
					  AXP813_VBUS_CLIMIT_900mA);
	case 1500000:
	case 2000000:
	case 2500000:
		val = (intval - 1000000) / 500000;
		return regmap_update_bits(power->regmap,
					  AXP20X_VBUS_IPSOUT_MGMT,
					  AXP20X_VBUS_CLIMIT_MASK, val);
	default:
		return -EINVAL;
	}

	return -EINVAL;
}

>>>>>>> upstream/android-13
static int axp20x_usb_power_set_current_max(struct axp20x_usb_power *power,
					    int intval)
{
	int val;

	switch (intval) {
	case 100000:
		if (power->axp20x_id == AXP221_ID)
			return -EINVAL;
<<<<<<< HEAD
		/* fall through */
=======
		fallthrough;
>>>>>>> upstream/android-13
	case 500000:
	case 900000:
		val = (900000 - intval) / 400000;
		return regmap_update_bits(power->regmap,
					  AXP20X_VBUS_IPSOUT_MGMT,
					  AXP20X_VBUS_CLIMIT_MASK, val);
	default:
		return -EINVAL;
	}

	return -EINVAL;
}

static int axp20x_usb_power_set_property(struct power_supply *psy,
					 enum power_supply_property psp,
					 const union power_supply_propval *val)
{
	struct axp20x_usb_power *power = power_supply_get_drvdata(psy);

	switch (psp) {
<<<<<<< HEAD
=======
	case POWER_SUPPLY_PROP_ONLINE:
		if (power->axp20x_id != AXP813_ID)
			return -EINVAL;
		return axp813_usb_power_set_online(power, val->intval);

>>>>>>> upstream/android-13
	case POWER_SUPPLY_PROP_VOLTAGE_MIN:
		return axp20x_usb_power_set_voltage_min(power, val->intval);

	case POWER_SUPPLY_PROP_CURRENT_MAX:
<<<<<<< HEAD
=======
		if (power->axp20x_id == AXP813_ID)
			return axp813_usb_power_set_current_max(power,
								val->intval);
>>>>>>> upstream/android-13
		return axp20x_usb_power_set_current_max(power, val->intval);

	default:
		return -EINVAL;
	}

	return -EINVAL;
}

static int axp20x_usb_power_prop_writeable(struct power_supply *psy,
					   enum power_supply_property psp)
{
<<<<<<< HEAD
=======
	struct axp20x_usb_power *power = power_supply_get_drvdata(psy);

	/*
	 * The VBUS path select flag works differently on AXP288 and newer:
	 *  - On AXP20x and AXP22x, the flag enables VBUS (ignoring N_VBUSEN).
	 *  - On AXP288 and AXP8xx, the flag disables VBUS (ignoring N_VBUSEN).
	 * We only expose the control on variants where it can be used to force
	 * the VBUS input offline.
	 */
	if (psp == POWER_SUPPLY_PROP_ONLINE)
		return power->axp20x_id == AXP813_ID;

>>>>>>> upstream/android-13
	return psp == POWER_SUPPLY_PROP_VOLTAGE_MIN ||
	       psp == POWER_SUPPLY_PROP_CURRENT_MAX;
}

static enum power_supply_property axp20x_usb_power_properties[] = {
	POWER_SUPPLY_PROP_HEALTH,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_VOLTAGE_MIN,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_CURRENT_MAX,
	POWER_SUPPLY_PROP_CURRENT_NOW,
};

static enum power_supply_property axp22x_usb_power_properties[] = {
	POWER_SUPPLY_PROP_HEALTH,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_VOLTAGE_MIN,
	POWER_SUPPLY_PROP_CURRENT_MAX,
};

static const struct power_supply_desc axp20x_usb_power_desc = {
	.name = "axp20x-usb",
	.type = POWER_SUPPLY_TYPE_USB,
	.properties = axp20x_usb_power_properties,
	.num_properties = ARRAY_SIZE(axp20x_usb_power_properties),
	.property_is_writeable = axp20x_usb_power_prop_writeable,
	.get_property = axp20x_usb_power_get_property,
	.set_property = axp20x_usb_power_set_property,
};

static const struct power_supply_desc axp22x_usb_power_desc = {
	.name = "axp20x-usb",
	.type = POWER_SUPPLY_TYPE_USB,
	.properties = axp22x_usb_power_properties,
	.num_properties = ARRAY_SIZE(axp22x_usb_power_properties),
	.property_is_writeable = axp20x_usb_power_prop_writeable,
	.get_property = axp20x_usb_power_get_property,
	.set_property = axp20x_usb_power_set_property,
};

<<<<<<< HEAD
=======
static const char * const axp20x_irq_names[] = {
	"VBUS_PLUGIN",
	"VBUS_REMOVAL",
	"VBUS_VALID",
	"VBUS_NOT_VALID",
};

static const char * const axp22x_irq_names[] = {
	"VBUS_PLUGIN",
	"VBUS_REMOVAL",
};

struct axp_data {
	const struct power_supply_desc	*power_desc;
	const char * const		*irq_names;
	unsigned int			num_irq_names;
	enum axp20x_variants		axp20x_id;
};

static const struct axp_data axp202_data = {
	.power_desc	= &axp20x_usb_power_desc,
	.irq_names	= axp20x_irq_names,
	.num_irq_names	= ARRAY_SIZE(axp20x_irq_names),
	.axp20x_id	= AXP202_ID,
};

static const struct axp_data axp221_data = {
	.power_desc	= &axp22x_usb_power_desc,
	.irq_names	= axp22x_irq_names,
	.num_irq_names	= ARRAY_SIZE(axp22x_irq_names),
	.axp20x_id	= AXP221_ID,
};

static const struct axp_data axp223_data = {
	.power_desc	= &axp22x_usb_power_desc,
	.irq_names	= axp22x_irq_names,
	.num_irq_names	= ARRAY_SIZE(axp22x_irq_names),
	.axp20x_id	= AXP223_ID,
};

static const struct axp_data axp813_data = {
	.power_desc	= &axp22x_usb_power_desc,
	.irq_names	= axp22x_irq_names,
	.num_irq_names	= ARRAY_SIZE(axp22x_irq_names),
	.axp20x_id	= AXP813_ID,
};

#ifdef CONFIG_PM_SLEEP
static int axp20x_usb_power_suspend(struct device *dev)
{
	struct axp20x_usb_power *power = dev_get_drvdata(dev);
	int i = 0;

	/*
	 * Allow wake via VBUS_PLUGIN only.
	 *
	 * As nested threaded IRQs are not automatically disabled during
	 * suspend, we must explicitly disable the remainder of the IRQs.
	 */
	if (device_may_wakeup(&power->supply->dev))
		enable_irq_wake(power->irqs[i++]);
	while (i < power->num_irqs)
		disable_irq(power->irqs[i++]);

	return 0;
}

static int axp20x_usb_power_resume(struct device *dev)
{
	struct axp20x_usb_power *power = dev_get_drvdata(dev);
	int i = 0;

	if (device_may_wakeup(&power->supply->dev))
		disable_irq_wake(power->irqs[i++]);
	while (i < power->num_irqs)
		enable_irq(power->irqs[i++]);

	mod_delayed_work(system_power_efficient_wq, &power->vbus_detect, DEBOUNCE_TIME);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(axp20x_usb_power_pm_ops, axp20x_usb_power_suspend,
						  axp20x_usb_power_resume);

>>>>>>> upstream/android-13
static int configure_iio_channels(struct platform_device *pdev,
				  struct axp20x_usb_power *power)
{
	power->vbus_v = devm_iio_channel_get(&pdev->dev, "vbus_v");
	if (IS_ERR(power->vbus_v)) {
		if (PTR_ERR(power->vbus_v) == -ENODEV)
			return -EPROBE_DEFER;
		return PTR_ERR(power->vbus_v);
	}

	power->vbus_i = devm_iio_channel_get(&pdev->dev, "vbus_i");
	if (IS_ERR(power->vbus_i)) {
		if (PTR_ERR(power->vbus_i) == -ENODEV)
			return -EPROBE_DEFER;
		return PTR_ERR(power->vbus_i);
	}

	return 0;
}

static int configure_adc_registers(struct axp20x_usb_power *power)
{
	/* Enable vbus voltage and current measurement */
	return regmap_update_bits(power->regmap, AXP20X_ADC_EN1,
				  AXP20X_ADC_EN1_VBUS_CURR |
				  AXP20X_ADC_EN1_VBUS_VOLT,
				  AXP20X_ADC_EN1_VBUS_CURR |
				  AXP20X_ADC_EN1_VBUS_VOLT);
}

static int axp20x_usb_power_probe(struct platform_device *pdev)
{
	struct axp20x_dev *axp20x = dev_get_drvdata(pdev->dev.parent);
	struct power_supply_config psy_cfg = {};
	struct axp20x_usb_power *power;
<<<<<<< HEAD
	static const char * const axp20x_irq_names[] = { "VBUS_PLUGIN",
		"VBUS_REMOVAL", "VBUS_VALID", "VBUS_NOT_VALID", NULL };
	static const char * const axp22x_irq_names[] = {
		"VBUS_PLUGIN", "VBUS_REMOVAL", NULL };
	const char * const *irq_names;
	const struct power_supply_desc *usb_power_desc;
=======
	const struct axp_data *axp_data;
>>>>>>> upstream/android-13
	int i, irq, ret;

	if (!of_device_is_available(pdev->dev.of_node))
		return -ENODEV;

	if (!axp20x) {
		dev_err(&pdev->dev, "Parent drvdata not set\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	power = devm_kzalloc(&pdev->dev, sizeof(*power), GFP_KERNEL);
	if (!power)
		return -ENOMEM;

	power->axp20x_id = (enum axp20x_variants)of_device_get_match_data(
								&pdev->dev);

	power->np = pdev->dev.of_node;
	power->regmap = axp20x->regmap;
=======
	axp_data = of_device_get_match_data(&pdev->dev);

	power = devm_kzalloc(&pdev->dev,
			     struct_size(power, irqs, axp_data->num_irq_names),
			     GFP_KERNEL);
	if (!power)
		return -ENOMEM;

	platform_set_drvdata(pdev, power);

	power->axp20x_id = axp_data->axp20x_id;
	power->regmap = axp20x->regmap;
	power->num_irqs = axp_data->num_irq_names;

	ret = devm_delayed_work_autocancel(&pdev->dev, &power->vbus_detect,
					   axp20x_usb_power_poll_vbus);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	if (power->axp20x_id == AXP202_ID) {
		/* Enable vbus valid checking */
		ret = regmap_update_bits(power->regmap, AXP20X_VBUS_MON,
					 AXP20X_VBUS_MON_VBUS_VALID,
					 AXP20X_VBUS_MON_VBUS_VALID);
		if (ret)
			return ret;

		if (IS_ENABLED(CONFIG_AXP20X_ADC))
			ret = configure_iio_channels(pdev, power);
		else
			ret = configure_adc_registers(power);

		if (ret)
			return ret;
<<<<<<< HEAD

		usb_power_desc = &axp20x_usb_power_desc;
		irq_names = axp20x_irq_names;
	} else if (power->axp20x_id == AXP221_ID ||
		   power->axp20x_id == AXP223_ID) {
		usb_power_desc = &axp22x_usb_power_desc;
		irq_names = axp22x_irq_names;
	} else {
		dev_err(&pdev->dev, "Unsupported AXP variant: %ld\n",
			axp20x->variant);
		return -EINVAL;
=======
	}

	if (power->axp20x_id == AXP813_ID) {
		/* Enable USB Battery Charging specification detection */
		ret = regmap_update_bits(axp20x->regmap, AXP288_BC_GLOBAL,
				   AXP813_BC_EN, AXP813_BC_EN);
		if (ret)
			return ret;
>>>>>>> upstream/android-13
	}

	psy_cfg.of_node = pdev->dev.of_node;
	psy_cfg.drv_data = power;

<<<<<<< HEAD
	power->supply = devm_power_supply_register(&pdev->dev, usb_power_desc,
=======
	power->supply = devm_power_supply_register(&pdev->dev,
						   axp_data->power_desc,
>>>>>>> upstream/android-13
						   &psy_cfg);
	if (IS_ERR(power->supply))
		return PTR_ERR(power->supply);

	/* Request irqs after registering, as irqs may trigger immediately */
<<<<<<< HEAD
	for (i = 0; irq_names[i]; i++) {
		irq = platform_get_irq_byname(pdev, irq_names[i]);
		if (irq < 0) {
			dev_warn(&pdev->dev, "No IRQ for %s: %d\n",
				 irq_names[i], irq);
			continue;
		}
		irq = regmap_irq_get_virq(axp20x->regmap_irqc, irq);
		ret = devm_request_any_context_irq(&pdev->dev, irq,
				axp20x_usb_power_irq, 0, DRVNAME, power);
		if (ret < 0)
			dev_warn(&pdev->dev, "Error requesting %s IRQ: %d\n",
				 irq_names[i], ret);
	}

=======
	for (i = 0; i < axp_data->num_irq_names; i++) {
		irq = platform_get_irq_byname(pdev, axp_data->irq_names[i]);
		if (irq < 0) {
			dev_err(&pdev->dev, "No IRQ for %s: %d\n",
				axp_data->irq_names[i], irq);
			return irq;
		}
		power->irqs[i] = regmap_irq_get_virq(axp20x->regmap_irqc, irq);
		ret = devm_request_any_context_irq(&pdev->dev, power->irqs[i],
						   axp20x_usb_power_irq, 0,
						   DRVNAME, power);
		if (ret < 0) {
			dev_err(&pdev->dev, "Error requesting %s IRQ: %d\n",
				axp_data->irq_names[i], ret);
			return ret;
		}
	}

	if (axp20x_usb_vbus_needs_polling(power))
		queue_delayed_work(system_power_efficient_wq, &power->vbus_detect, 0);

>>>>>>> upstream/android-13
	return 0;
}

static const struct of_device_id axp20x_usb_power_match[] = {
	{
		.compatible = "x-powers,axp202-usb-power-supply",
<<<<<<< HEAD
		.data = (void *)AXP202_ID,
	}, {
		.compatible = "x-powers,axp221-usb-power-supply",
		.data = (void *)AXP221_ID,
	}, {
		.compatible = "x-powers,axp223-usb-power-supply",
		.data = (void *)AXP223_ID,
=======
		.data = &axp202_data,
	}, {
		.compatible = "x-powers,axp221-usb-power-supply",
		.data = &axp221_data,
	}, {
		.compatible = "x-powers,axp223-usb-power-supply",
		.data = &axp223_data,
	}, {
		.compatible = "x-powers,axp813-usb-power-supply",
		.data = &axp813_data,
>>>>>>> upstream/android-13
	}, { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, axp20x_usb_power_match);

static struct platform_driver axp20x_usb_power_driver = {
	.probe = axp20x_usb_power_probe,
	.driver = {
<<<<<<< HEAD
		.name = DRVNAME,
		.of_match_table = axp20x_usb_power_match,
=======
		.name		= DRVNAME,
		.of_match_table	= axp20x_usb_power_match,
		.pm		= &axp20x_usb_power_pm_ops,
>>>>>>> upstream/android-13
	},
};

module_platform_driver(axp20x_usb_power_driver);

MODULE_AUTHOR("Hans de Goede <hdegoede@redhat.com>");
MODULE_DESCRIPTION("AXP20x PMIC USB power supply status driver");
MODULE_LICENSE("GPL");
