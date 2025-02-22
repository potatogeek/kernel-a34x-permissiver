<<<<<<< HEAD
/**
 * emac_arc.c - ARC EMAC specific glue layer
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/**
 * DOC: emac_arc.c - ARC EMAC specific glue layer
>>>>>>> upstream/android-13
 *
 * Copyright (C) 2014 Romain Perier
 *
 * Romain Perier  <romain.perier@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#include <linux/etherdevice.h>
#include <linux/module.h>
#include <linux/of_net.h>
#include <linux/platform_device.h>

#include "emac.h"

#define DRV_NAME    "emac_arc"
<<<<<<< HEAD
#define DRV_VERSION "1.0"
=======
>>>>>>> upstream/android-13

static int emac_arc_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
<<<<<<< HEAD
	struct net_device *ndev;
	struct arc_emac_priv *priv;
	int interface, err;
=======
	struct arc_emac_priv *priv;
	phy_interface_t interface;
	struct net_device *ndev;
	int err;
>>>>>>> upstream/android-13

	if (!dev->of_node)
		return -ENODEV;

	ndev = alloc_etherdev(sizeof(struct arc_emac_priv));
	if (!ndev)
		return -ENOMEM;
	platform_set_drvdata(pdev, ndev);
	SET_NETDEV_DEV(ndev, dev);

	priv = netdev_priv(ndev);
	priv->drv_name = DRV_NAME;
<<<<<<< HEAD
	priv->drv_version = DRV_VERSION;

	interface = of_get_phy_mode(dev->of_node);
	if (interface < 0)
		interface = PHY_INTERFACE_MODE_MII;
=======

	err = of_get_phy_mode(dev->of_node, &interface);
	if (err) {
		if (err == -ENODEV)
			interface = PHY_INTERFACE_MODE_MII;
		else
			goto out_netdev;
	}
>>>>>>> upstream/android-13

	priv->clk = devm_clk_get(dev, "hclk");
	if (IS_ERR(priv->clk)) {
		dev_err(dev, "failed to retrieve host clock from device tree\n");
		err = -EINVAL;
		goto out_netdev;
	}

	err = arc_emac_probe(ndev, interface);
out_netdev:
	if (err)
		free_netdev(ndev);
	return err;
}

static int emac_arc_remove(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	int err;

	err = arc_emac_remove(ndev);
	free_netdev(ndev);
	return err;
}

static const struct of_device_id emac_arc_dt_ids[] = {
	{ .compatible = "snps,arc-emac" },
	{ /* Sentinel */ }
};
MODULE_DEVICE_TABLE(of, emac_arc_dt_ids);

static struct platform_driver emac_arc_driver = {
	.probe = emac_arc_probe,
	.remove = emac_arc_remove,
	.driver = {
		.name = DRV_NAME,
		.of_match_table  = emac_arc_dt_ids,
	},
};

module_platform_driver(emac_arc_driver);

MODULE_AUTHOR("Romain Perier <romain.perier@gmail.com>");
MODULE_DESCRIPTION("ARC EMAC platform driver");
MODULE_LICENSE("GPL");
