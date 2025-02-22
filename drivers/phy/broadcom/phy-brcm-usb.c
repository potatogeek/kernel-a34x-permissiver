<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * phy-brcm-usb.c - Broadcom USB Phy Driver
 *
 * Copyright (C) 2015-2017 Broadcom
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
=======
>>>>>>> upstream/android-13
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
<<<<<<< HEAD
=======
#include <linux/of_device.h>
>>>>>>> upstream/android-13
#include <linux/phy/phy.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/soc/brcmstb/brcmstb.h>
#include <dt-bindings/phy/phy.h>
<<<<<<< HEAD
=======
#include <linux/mfd/syscon.h>
#include <linux/suspend.h>
>>>>>>> upstream/android-13

#include "phy-brcm-usb-init.h"

static DEFINE_MUTEX(sysfs_lock);

enum brcm_usb_phy_id {
	BRCM_USB_PHY_2_0 = 0,
	BRCM_USB_PHY_3_0,
	BRCM_USB_PHY_ID_MAX
};

struct value_to_name_map {
	int value;
	const char *name;
};

<<<<<<< HEAD
static struct value_to_name_map brcm_dr_mode_to_name[] = {
=======
struct match_chip_info {
	void (*init_func)(struct brcm_usb_init_params *params);
	u8 required_regs[BRCM_REGS_MAX + 1];
	u8 optional_reg;
};

static const struct value_to_name_map brcm_dr_mode_to_name[] = {
>>>>>>> upstream/android-13
	{ USB_CTLR_MODE_HOST, "host" },
	{ USB_CTLR_MODE_DEVICE, "peripheral" },
	{ USB_CTLR_MODE_DRD, "drd" },
	{ USB_CTLR_MODE_TYPEC_PD, "typec-pd" }
};

<<<<<<< HEAD
static struct value_to_name_map brcm_dual_mode_to_name[] = {
=======
static const struct value_to_name_map brcm_dual_mode_to_name[] = {
>>>>>>> upstream/android-13
	{ 0, "host" },
	{ 1, "device" },
	{ 2, "auto" },
};

struct brcm_usb_phy {
	struct phy *phy;
	unsigned int id;
	bool inited;
};

struct brcm_usb_phy_data {
	struct  brcm_usb_init_params ini;
	bool			has_eohci;
	bool			has_xhci;
	struct clk		*usb_20_clk;
	struct clk		*usb_30_clk;
<<<<<<< HEAD
	struct mutex		mutex;	/* serialize phy init */
	int			init_count;
	struct brcm_usb_phy	phys[BRCM_USB_PHY_ID_MAX];
};

=======
	struct clk		*suspend_clk;
	struct mutex		mutex;	/* serialize phy init */
	int			init_count;
	int			wake_irq;
	struct brcm_usb_phy	phys[BRCM_USB_PHY_ID_MAX];
	struct notifier_block	pm_notifier;
	bool			pm_active;
};

static s8 *node_reg_names[BRCM_REGS_MAX] = {
	"crtl", "xhci_ec", "xhci_gbl", "usb_phy", "usb_mdio", "bdc_ec"
};

static int brcm_pm_notifier(struct notifier_block *notifier,
			    unsigned long pm_event,
			    void *unused)
{
	struct brcm_usb_phy_data *priv =
		container_of(notifier, struct brcm_usb_phy_data, pm_notifier);

	switch (pm_event) {
	case PM_HIBERNATION_PREPARE:
	case PM_SUSPEND_PREPARE:
		priv->pm_active = true;
		break;
	case PM_POST_RESTORE:
	case PM_POST_HIBERNATION:
	case PM_POST_SUSPEND:
		priv->pm_active = false;
		break;
	}
	return NOTIFY_DONE;
}

static irqreturn_t brcm_usb_phy_wake_isr(int irq, void *dev_id)
{
	struct phy *gphy = dev_id;

	pm_wakeup_event(&gphy->dev, 0);

	return IRQ_HANDLED;
}

>>>>>>> upstream/android-13
static int brcm_usb_phy_init(struct phy *gphy)
{
	struct brcm_usb_phy *phy = phy_get_drvdata(gphy);
	struct brcm_usb_phy_data *priv =
		container_of(phy, struct brcm_usb_phy_data, phys[phy->id]);

<<<<<<< HEAD
=======
	if (priv->pm_active)
		return 0;

>>>>>>> upstream/android-13
	/*
	 * Use a lock to make sure a second caller waits until
	 * the base phy is inited before using it.
	 */
	mutex_lock(&priv->mutex);
	if (priv->init_count++ == 0) {
<<<<<<< HEAD
		clk_enable(priv->usb_20_clk);
		clk_enable(priv->usb_30_clk);
=======
		clk_prepare_enable(priv->usb_20_clk);
		clk_prepare_enable(priv->usb_30_clk);
		clk_prepare_enable(priv->suspend_clk);
>>>>>>> upstream/android-13
		brcm_usb_init_common(&priv->ini);
	}
	mutex_unlock(&priv->mutex);
	if (phy->id == BRCM_USB_PHY_2_0)
		brcm_usb_init_eohci(&priv->ini);
	else if (phy->id == BRCM_USB_PHY_3_0)
		brcm_usb_init_xhci(&priv->ini);
	phy->inited = true;
	dev_dbg(&gphy->dev, "INIT, id: %d, total: %d\n", phy->id,
		priv->init_count);

	return 0;
}

static int brcm_usb_phy_exit(struct phy *gphy)
{
	struct brcm_usb_phy *phy = phy_get_drvdata(gphy);
	struct brcm_usb_phy_data *priv =
		container_of(phy, struct brcm_usb_phy_data, phys[phy->id]);

<<<<<<< HEAD
=======
	if (priv->pm_active)
		return 0;

>>>>>>> upstream/android-13
	dev_dbg(&gphy->dev, "EXIT\n");
	if (phy->id == BRCM_USB_PHY_2_0)
		brcm_usb_uninit_eohci(&priv->ini);
	if (phy->id == BRCM_USB_PHY_3_0)
		brcm_usb_uninit_xhci(&priv->ini);

	/* If both xhci and eohci are gone, reset everything else */
	mutex_lock(&priv->mutex);
	if (--priv->init_count == 0) {
		brcm_usb_uninit_common(&priv->ini);
<<<<<<< HEAD
		clk_disable(priv->usb_20_clk);
		clk_disable(priv->usb_30_clk);
=======
		clk_disable_unprepare(priv->usb_20_clk);
		clk_disable_unprepare(priv->usb_30_clk);
		clk_disable_unprepare(priv->suspend_clk);
>>>>>>> upstream/android-13
	}
	mutex_unlock(&priv->mutex);
	phy->inited = false;
	return 0;
}

<<<<<<< HEAD
static struct phy_ops brcm_usb_phy_ops = {
=======
static const struct phy_ops brcm_usb_phy_ops = {
>>>>>>> upstream/android-13
	.init		= brcm_usb_phy_init,
	.exit		= brcm_usb_phy_exit,
	.owner		= THIS_MODULE,
};

static struct phy *brcm_usb_phy_xlate(struct device *dev,
				      struct of_phandle_args *args)
{
	struct brcm_usb_phy_data *data = dev_get_drvdata(dev);

	/*
	 * values 0 and 1 are for backward compatibility with
	 * device tree nodes from older bootloaders.
	 */
	switch (args->args[0]) {
	case 0:
	case PHY_TYPE_USB2:
		if (data->phys[BRCM_USB_PHY_2_0].phy)
			return data->phys[BRCM_USB_PHY_2_0].phy;
		dev_warn(dev, "Error, 2.0 Phy not found\n");
		break;
	case 1:
	case PHY_TYPE_USB3:
		if (data->phys[BRCM_USB_PHY_3_0].phy)
			return data->phys[BRCM_USB_PHY_3_0].phy;
		dev_warn(dev, "Error, 3.0 Phy not found\n");
		break;
	}
	return ERR_PTR(-ENODEV);
}

<<<<<<< HEAD
static int name_to_value(struct value_to_name_map *table, int count,
=======
static int name_to_value(const struct value_to_name_map *table, int count,
>>>>>>> upstream/android-13
			 const char *name, int *value)
{
	int x;

	*value = 0;
	for (x = 0; x < count; x++) {
		if (sysfs_streq(name, table[x].name)) {
			*value = x;
			return 0;
		}
	}
	return -EINVAL;
}

<<<<<<< HEAD
static const char *value_to_name(struct value_to_name_map *table, int count,
=======
static const char *value_to_name(const struct value_to_name_map *table, int count,
>>>>>>> upstream/android-13
				 int value)
{
	if (value >= count)
		return "unknown";
	return table[value].name;
}

static ssize_t dr_mode_show(struct device *dev,
			    struct device_attribute *attr,
			    char *buf)
{
	struct brcm_usb_phy_data *priv = dev_get_drvdata(dev);

	return sprintf(buf, "%s\n",
		value_to_name(&brcm_dr_mode_to_name[0],
			      ARRAY_SIZE(brcm_dr_mode_to_name),
			      priv->ini.mode));
}
static DEVICE_ATTR_RO(dr_mode);

static ssize_t dual_select_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t len)
{
	struct brcm_usb_phy_data *priv = dev_get_drvdata(dev);
	int value;
	int res;

	mutex_lock(&sysfs_lock);
	res = name_to_value(&brcm_dual_mode_to_name[0],
			    ARRAY_SIZE(brcm_dual_mode_to_name), buf, &value);
	if (!res) {
<<<<<<< HEAD
		brcm_usb_init_set_dual_select(&priv->ini, value);
=======
		brcm_usb_set_dual_select(&priv->ini, value);
>>>>>>> upstream/android-13
		res = len;
	}
	mutex_unlock(&sysfs_lock);
	return res;
}

static ssize_t dual_select_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	struct brcm_usb_phy_data *priv = dev_get_drvdata(dev);
	int value;

	mutex_lock(&sysfs_lock);
<<<<<<< HEAD
	value = brcm_usb_init_get_dual_select(&priv->ini);
=======
	value = brcm_usb_get_dual_select(&priv->ini);
>>>>>>> upstream/android-13
	mutex_unlock(&sysfs_lock);
	return sprintf(buf, "%s\n",
		value_to_name(&brcm_dual_mode_to_name[0],
			      ARRAY_SIZE(brcm_dual_mode_to_name),
			      value));
}
static DEVICE_ATTR_RW(dual_select);

static struct attribute *brcm_usb_phy_attrs[] = {
	&dev_attr_dr_mode.attr,
	&dev_attr_dual_select.attr,
	NULL
};

static const struct attribute_group brcm_usb_phy_group = {
	.attrs = brcm_usb_phy_attrs,
};

<<<<<<< HEAD
static int brcm_usb_phy_dvr_init(struct device *dev,
				 struct brcm_usb_phy_data *priv,
				 struct device_node *dn)
{
	struct phy *gphy;
=======
static const struct match_chip_info chip_info_4908 = {
	.init_func = &brcm_usb_dvr_init_4908,
	.required_regs = {
		BRCM_REGS_CTRL,
		BRCM_REGS_XHCI_EC,
		-1,
	},
};

static const struct match_chip_info chip_info_7216 = {
	.init_func = &brcm_usb_dvr_init_7216,
	.required_regs = {
		BRCM_REGS_CTRL,
		BRCM_REGS_XHCI_EC,
		BRCM_REGS_XHCI_GBL,
		-1,
	},
};

static const struct match_chip_info chip_info_7211b0 = {
	.init_func = &brcm_usb_dvr_init_7211b0,
	.required_regs = {
		BRCM_REGS_CTRL,
		BRCM_REGS_XHCI_EC,
		BRCM_REGS_XHCI_GBL,
		BRCM_REGS_USB_PHY,
		BRCM_REGS_USB_MDIO,
		-1,
	},
	.optional_reg = BRCM_REGS_BDC_EC,
};

static const struct match_chip_info chip_info_7445 = {
	.init_func = &brcm_usb_dvr_init_7445,
	.required_regs = {
		BRCM_REGS_CTRL,
		BRCM_REGS_XHCI_EC,
		-1,
	},
};

static const struct of_device_id brcm_usb_dt_ids[] = {
	{
		.compatible = "brcm,bcm4908-usb-phy",
		.data = &chip_info_4908,
	},
	{
		.compatible = "brcm,bcm7216-usb-phy",
		.data = &chip_info_7216,
	},
	{
		.compatible = "brcm,bcm7211-usb-phy",
		.data = &chip_info_7211b0,
	},
	{
		.compatible = "brcm,brcmstb-usb-phy",
		.data = &chip_info_7445,
	},
	{ /* sentinel */ }
};

static int brcm_usb_get_regs(struct platform_device *pdev,
			     enum brcmusb_reg_sel regs,
			     struct  brcm_usb_init_params *ini,
			     bool optional)
{
	struct resource *res;

	/* Older DT nodes have ctrl and optional xhci_ec by index only */
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM,
						node_reg_names[regs]);
	if (res == NULL) {
		if (regs == BRCM_REGS_CTRL) {
			res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
		} else if (regs == BRCM_REGS_XHCI_EC) {
			res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
			/* XHCI_EC registers are optional */
			if (res == NULL)
				return 0;
		}
		if (res == NULL) {
			if (optional) {
				dev_dbg(&pdev->dev,
					"Optional reg %s not found\n",
					node_reg_names[regs]);
				return 0;
			}
			dev_err(&pdev->dev, "can't get %s base addr\n",
				node_reg_names[regs]);
			return 1;
		}
	}
	ini->regs[regs] = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(ini->regs[regs])) {
		dev_err(&pdev->dev, "can't map %s register space\n",
			node_reg_names[regs]);
		return 1;
	}
	return 0;
}

static int brcm_usb_phy_dvr_init(struct platform_device *pdev,
				 struct brcm_usb_phy_data *priv,
				 struct device_node *dn)
{
	struct device *dev = &pdev->dev;
	struct phy *gphy = NULL;
>>>>>>> upstream/android-13
	int err;

	priv->usb_20_clk = of_clk_get_by_name(dn, "sw_usb");
	if (IS_ERR(priv->usb_20_clk)) {
<<<<<<< HEAD
=======
		if (PTR_ERR(priv->usb_20_clk) == -EPROBE_DEFER)
			return -EPROBE_DEFER;
>>>>>>> upstream/android-13
		dev_info(dev, "Clock not found in Device Tree\n");
		priv->usb_20_clk = NULL;
	}
	err = clk_prepare_enable(priv->usb_20_clk);
	if (err)
		return err;

	if (priv->has_eohci) {
		gphy = devm_phy_create(dev, NULL, &brcm_usb_phy_ops);
		if (IS_ERR(gphy)) {
			dev_err(dev, "failed to create EHCI/OHCI PHY\n");
			return PTR_ERR(gphy);
		}
		priv->phys[BRCM_USB_PHY_2_0].phy = gphy;
		priv->phys[BRCM_USB_PHY_2_0].id = BRCM_USB_PHY_2_0;
		phy_set_drvdata(gphy, &priv->phys[BRCM_USB_PHY_2_0]);
	}

	if (priv->has_xhci) {
		gphy = devm_phy_create(dev, NULL, &brcm_usb_phy_ops);
		if (IS_ERR(gphy)) {
			dev_err(dev, "failed to create XHCI PHY\n");
			return PTR_ERR(gphy);
		}
		priv->phys[BRCM_USB_PHY_3_0].phy = gphy;
		priv->phys[BRCM_USB_PHY_3_0].id = BRCM_USB_PHY_3_0;
		phy_set_drvdata(gphy, &priv->phys[BRCM_USB_PHY_3_0]);

		priv->usb_30_clk = of_clk_get_by_name(dn, "sw_usb3");
		if (IS_ERR(priv->usb_30_clk)) {
<<<<<<< HEAD
=======
			if (PTR_ERR(priv->usb_30_clk) == -EPROBE_DEFER)
				return -EPROBE_DEFER;
>>>>>>> upstream/android-13
			dev_info(dev,
				 "USB3.0 clock not found in Device Tree\n");
			priv->usb_30_clk = NULL;
		}
		err = clk_prepare_enable(priv->usb_30_clk);
		if (err)
			return err;
	}
<<<<<<< HEAD
=======

	priv->suspend_clk = clk_get(dev, "usb0_freerun");
	if (IS_ERR(priv->suspend_clk)) {
		if (PTR_ERR(priv->suspend_clk) == -EPROBE_DEFER)
			return -EPROBE_DEFER;
		dev_err(dev, "Suspend Clock not found in Device Tree\n");
		priv->suspend_clk = NULL;
	}

	priv->wake_irq = platform_get_irq_byname(pdev, "wake");
	if (priv->wake_irq < 0)
		priv->wake_irq = platform_get_irq_byname(pdev, "wakeup");
	if (priv->wake_irq >= 0) {
		err = devm_request_irq(dev, priv->wake_irq,
				       brcm_usb_phy_wake_isr, 0,
				       dev_name(dev), gphy);
		if (err < 0)
			return err;
		device_set_wakeup_capable(dev, 1);
	} else {
		dev_info(dev,
			 "Wake interrupt missing, system wake not supported\n");
	}

>>>>>>> upstream/android-13
	return 0;
}

static int brcm_usb_phy_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct resource *res;
=======
>>>>>>> upstream/android-13
	struct device *dev = &pdev->dev;
	struct brcm_usb_phy_data *priv;
	struct phy_provider *phy_provider;
	struct device_node *dn = pdev->dev.of_node;
	int err;
	const char *mode;
<<<<<<< HEAD
=======
	const struct match_chip_info *info;
	struct regmap *rmap;
	int x;
>>>>>>> upstream/android-13

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;
	platform_set_drvdata(pdev, priv);

	priv->ini.family_id = brcmstb_get_family_id();
	priv->ini.product_id = brcmstb_get_product_id();
<<<<<<< HEAD
	brcm_usb_set_family_map(&priv->ini);
	dev_dbg(dev, "Best mapping table is for %s\n",
		priv->ini.family_name);
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "can't get USB_CTRL base address\n");
		return -EINVAL;
	}
	priv->ini.ctrl_regs = devm_ioremap_resource(dev, res);
	if (IS_ERR(priv->ini.ctrl_regs)) {
		dev_err(dev, "can't map CTRL register space\n");
		return -EINVAL;
	}

	/* The XHCI EC registers are optional */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (res) {
		priv->ini.xhci_ec_regs =
			devm_ioremap_resource(dev, res);
		if (IS_ERR(priv->ini.xhci_ec_regs)) {
			dev_err(dev, "can't map XHCI EC register space\n");
			return -EINVAL;
		}
	}
=======

	info = of_device_get_match_data(&pdev->dev);
	if (!info)
		return -ENOENT;

	info->init_func(&priv->ini);

	dev_dbg(dev, "Best mapping table is for %s\n",
		priv->ini.family_name);
>>>>>>> upstream/android-13

	of_property_read_u32(dn, "brcm,ipp", &priv->ini.ipp);
	of_property_read_u32(dn, "brcm,ioc", &priv->ini.ioc);

	priv->ini.mode = USB_CTLR_MODE_HOST;
	err = of_property_read_string(dn, "dr_mode", &mode);
	if (err == 0) {
		name_to_value(&brcm_dr_mode_to_name[0],
			      ARRAY_SIZE(brcm_dr_mode_to_name),
			mode, &priv->ini.mode);
	}
	if (of_property_read_bool(dn, "brcm,has-xhci"))
		priv->has_xhci = true;
	if (of_property_read_bool(dn, "brcm,has-eohci"))
		priv->has_eohci = true;

<<<<<<< HEAD
	err = brcm_usb_phy_dvr_init(dev, priv, dn);
	if (err)
		return err;

=======
	for (x = 0; x < BRCM_REGS_MAX; x++) {
		if (info->required_regs[x] >= BRCM_REGS_MAX)
			break;

		err = brcm_usb_get_regs(pdev, info->required_regs[x],
					&priv->ini, false);
		if (err)
			return -EINVAL;
	}
	if (info->optional_reg) {
		err = brcm_usb_get_regs(pdev, info->optional_reg,
					&priv->ini, true);
		if (err)
			return -EINVAL;
	}

	err = brcm_usb_phy_dvr_init(pdev, priv, dn);
	if (err)
		return err;

	priv->pm_notifier.notifier_call = brcm_pm_notifier;
	register_pm_notifier(&priv->pm_notifier);

>>>>>>> upstream/android-13
	mutex_init(&priv->mutex);

	/* make sure invert settings are correct */
	brcm_usb_init_ipp(&priv->ini);

	/*
	 * Create sysfs entries for mode.
	 * Remove "dual_select" attribute if not in dual mode
	 */
	if (priv->ini.mode != USB_CTLR_MODE_DRD)
		brcm_usb_phy_attrs[1] = NULL;
	err = sysfs_create_group(&dev->kobj, &brcm_usb_phy_group);
	if (err)
		dev_warn(dev, "Error creating sysfs attributes\n");

<<<<<<< HEAD
=======
	/* Get piarbctl syscon if it exists */
	rmap = syscon_regmap_lookup_by_phandle(dev->of_node,
						 "syscon-piarbctl");
	if (IS_ERR(rmap))
		rmap = syscon_regmap_lookup_by_phandle(dev->of_node,
						       "brcm,syscon-piarbctl");
	if (!IS_ERR(rmap))
		priv->ini.syscon_piarbctl = rmap;

>>>>>>> upstream/android-13
	/* start with everything off */
	if (priv->has_xhci)
		brcm_usb_uninit_xhci(&priv->ini);
	if (priv->has_eohci)
		brcm_usb_uninit_eohci(&priv->ini);
	brcm_usb_uninit_common(&priv->ini);
<<<<<<< HEAD
	clk_disable(priv->usb_20_clk);
	clk_disable(priv->usb_30_clk);

	phy_provider = devm_of_phy_provider_register(dev, brcm_usb_phy_xlate);
	if (IS_ERR(phy_provider))
		return PTR_ERR(phy_provider);

	return 0;
=======
	clk_disable_unprepare(priv->usb_20_clk);
	clk_disable_unprepare(priv->usb_30_clk);

	phy_provider = devm_of_phy_provider_register(dev, brcm_usb_phy_xlate);

	return PTR_ERR_OR_ZERO(phy_provider);
>>>>>>> upstream/android-13
}

static int brcm_usb_phy_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	sysfs_remove_group(&pdev->dev.kobj, &brcm_usb_phy_group);
=======
	struct brcm_usb_phy_data *priv = dev_get_drvdata(&pdev->dev);

	sysfs_remove_group(&pdev->dev.kobj, &brcm_usb_phy_group);
	unregister_pm_notifier(&priv->pm_notifier);
>>>>>>> upstream/android-13

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int brcm_usb_phy_suspend(struct device *dev)
{
	struct brcm_usb_phy_data *priv = dev_get_drvdata(dev);

	if (priv->init_count) {
<<<<<<< HEAD
		clk_disable(priv->usb_20_clk);
		clk_disable(priv->usb_30_clk);
=======
		dev_dbg(dev, "SUSPEND\n");
		priv->ini.wake_enabled = device_may_wakeup(dev);
		if (priv->phys[BRCM_USB_PHY_3_0].inited)
			brcm_usb_uninit_xhci(&priv->ini);
		if (priv->phys[BRCM_USB_PHY_2_0].inited)
			brcm_usb_uninit_eohci(&priv->ini);
		brcm_usb_uninit_common(&priv->ini);

		/*
		 * Handle the clocks unless needed for wake. This has
		 * to work for both older XHCI->3.0-clks, EOHCI->2.0-clks
		 * and newer XHCI->2.0-clks/3.0-clks.
		 */

		if (!priv->ini.suspend_with_clocks) {
			if (priv->phys[BRCM_USB_PHY_3_0].inited)
				clk_disable_unprepare(priv->usb_30_clk);
			if (priv->phys[BRCM_USB_PHY_2_0].inited ||
			    !priv->has_eohci)
				clk_disable_unprepare(priv->usb_20_clk);
		}
		if (priv->wake_irq >= 0)
			enable_irq_wake(priv->wake_irq);
>>>>>>> upstream/android-13
	}
	return 0;
}

static int brcm_usb_phy_resume(struct device *dev)
{
	struct brcm_usb_phy_data *priv = dev_get_drvdata(dev);

<<<<<<< HEAD
	clk_enable(priv->usb_20_clk);
	clk_enable(priv->usb_30_clk);
=======
	clk_prepare_enable(priv->usb_20_clk);
	clk_prepare_enable(priv->usb_30_clk);
>>>>>>> upstream/android-13
	brcm_usb_init_ipp(&priv->ini);

	/*
	 * Initialize anything that was previously initialized.
	 * Uninitialize anything that wasn't previously initialized.
	 */
	if (priv->init_count) {
<<<<<<< HEAD
=======
		dev_dbg(dev, "RESUME\n");
		if (priv->wake_irq >= 0)
			disable_irq_wake(priv->wake_irq);
>>>>>>> upstream/android-13
		brcm_usb_init_common(&priv->ini);
		if (priv->phys[BRCM_USB_PHY_2_0].inited) {
			brcm_usb_init_eohci(&priv->ini);
		} else if (priv->has_eohci) {
			brcm_usb_uninit_eohci(&priv->ini);
<<<<<<< HEAD
			clk_disable(priv->usb_20_clk);
=======
			clk_disable_unprepare(priv->usb_20_clk);
>>>>>>> upstream/android-13
		}
		if (priv->phys[BRCM_USB_PHY_3_0].inited) {
			brcm_usb_init_xhci(&priv->ini);
		} else if (priv->has_xhci) {
			brcm_usb_uninit_xhci(&priv->ini);
<<<<<<< HEAD
			clk_disable(priv->usb_30_clk);
=======
			clk_disable_unprepare(priv->usb_30_clk);
			if (!priv->has_eohci)
				clk_disable_unprepare(priv->usb_20_clk);
>>>>>>> upstream/android-13
		}
	} else {
		if (priv->has_xhci)
			brcm_usb_uninit_xhci(&priv->ini);
		if (priv->has_eohci)
			brcm_usb_uninit_eohci(&priv->ini);
		brcm_usb_uninit_common(&priv->ini);
<<<<<<< HEAD
		clk_disable(priv->usb_20_clk);
		clk_disable(priv->usb_30_clk);
	}

=======
		clk_disable_unprepare(priv->usb_20_clk);
		clk_disable_unprepare(priv->usb_30_clk);
	}
	priv->ini.wake_enabled = false;
>>>>>>> upstream/android-13
	return 0;
}
#endif /* CONFIG_PM_SLEEP */

static const struct dev_pm_ops brcm_usb_phy_pm_ops = {
	SET_LATE_SYSTEM_SLEEP_PM_OPS(brcm_usb_phy_suspend, brcm_usb_phy_resume)
};

<<<<<<< HEAD
static const struct of_device_id brcm_usb_dt_ids[] = {
	{ .compatible = "brcm,brcmstb-usb-phy" },
	{ /* sentinel */ }
};

=======
>>>>>>> upstream/android-13
MODULE_DEVICE_TABLE(of, brcm_usb_dt_ids);

static struct platform_driver brcm_usb_driver = {
	.probe		= brcm_usb_phy_probe,
	.remove		= brcm_usb_phy_remove,
	.driver		= {
		.name	= "brcmstb-usb-phy",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
=======
>>>>>>> upstream/android-13
		.pm = &brcm_usb_phy_pm_ops,
		.of_match_table = brcm_usb_dt_ids,
	},
};

module_platform_driver(brcm_usb_driver);

MODULE_ALIAS("platform:brcmstb-usb-phy");
MODULE_AUTHOR("Al Cooper <acooper@broadcom.com>");
MODULE_DESCRIPTION("BRCM USB PHY driver");
MODULE_LICENSE("GPL v2");
