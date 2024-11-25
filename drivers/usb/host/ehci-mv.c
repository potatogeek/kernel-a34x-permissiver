// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2011 Marvell International Ltd. All rights reserved.
 * Author: Chao Xie <chao.xie@marvell.com>
 *        Neil Zhang <zhangwm@marvell.com>
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/usb/otg.h>
<<<<<<< HEAD
#include <linux/platform_data/mv_usb.h>

#define CAPLENGTH_MASK         (0xff)

struct ehci_hcd_mv {
	struct usb_hcd *hcd;

	/* Which mode does this ehci running OTG/Host ? */
	int mode;

	void __iomem *phy_regs;
=======
#include <linux/usb/of.h>
#include <linux/platform_data/mv_usb.h>
#include <linux/io.h>

#include <linux/usb/hcd.h>

#include "ehci.h"

/* registers */
#define U2x_CAPREGS_OFFSET       0x100

#define CAPLENGTH_MASK         (0xff)

#define hcd_to_ehci_hcd_mv(h) ((struct ehci_hcd_mv *)hcd_to_ehci(h)->priv)

struct ehci_hcd_mv {
	/* Which mode does this ehci running OTG/Host ? */
	int mode;

	void __iomem *base;
>>>>>>> upstream/android-13
	void __iomem *cap_regs;
	void __iomem *op_regs;

	struct usb_phy *otg;
<<<<<<< HEAD

	struct mv_usb_platform_data *pdata;

	struct clk *clk;
};

static void ehci_clock_enable(struct ehci_hcd_mv *ehci_mv)
{
	clk_prepare_enable(ehci_mv->clk);
}

static void ehci_clock_disable(struct ehci_hcd_mv *ehci_mv)
{
	clk_disable_unprepare(ehci_mv->clk);
}

=======
	struct clk *clk;

	struct phy *phy;

	int (*set_vbus)(unsigned int vbus);
};

>>>>>>> upstream/android-13
static int mv_ehci_enable(struct ehci_hcd_mv *ehci_mv)
{
	int retval;

<<<<<<< HEAD
	ehci_clock_enable(ehci_mv);
	if (ehci_mv->pdata->phy_init) {
		retval = ehci_mv->pdata->phy_init(ehci_mv->phy_regs);
		if (retval)
			return retval;
	}

	return 0;
=======
	retval = clk_prepare_enable(ehci_mv->clk);
	if (retval)
		return retval;

	retval = phy_init(ehci_mv->phy);
	if (retval)
		clk_disable_unprepare(ehci_mv->clk);

	return retval;
>>>>>>> upstream/android-13
}

static void mv_ehci_disable(struct ehci_hcd_mv *ehci_mv)
{
<<<<<<< HEAD
	if (ehci_mv->pdata->phy_deinit)
		ehci_mv->pdata->phy_deinit(ehci_mv->phy_regs);
	ehci_clock_disable(ehci_mv);
=======
	phy_exit(ehci_mv->phy);
	clk_disable_unprepare(ehci_mv->clk);
>>>>>>> upstream/android-13
}

static int mv_ehci_reset(struct usb_hcd *hcd)
{
	struct device *dev = hcd->self.controller;
<<<<<<< HEAD
	struct ehci_hcd_mv *ehci_mv = dev_get_drvdata(dev);
=======
	struct ehci_hcd_mv *ehci_mv = hcd_to_ehci_hcd_mv(hcd);
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	u32 status;
>>>>>>> upstream/android-13
	int retval;

	if (ehci_mv == NULL) {
		dev_err(dev, "Can not find private ehci data\n");
		return -ENODEV;
	}

	hcd->has_tt = 1;

	retval = ehci_setup(hcd);
	if (retval)
		dev_err(dev, "ehci_setup failed %d\n", retval);

<<<<<<< HEAD
	return retval;
}

static const struct hc_driver mv_ehci_hc_driver = {
	.description = hcd_name,
	.product_desc = "Marvell EHCI",
	.hcd_priv_size = sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq = ehci_irq,
	.flags = HCD_MEMORY | HCD_USB2 | HCD_BH,

	/*
	 * basic lifecycle operations
	 */
	.reset = mv_ehci_reset,
	.start = ehci_run,
	.stop = ehci_stop,
	.shutdown = ehci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue = ehci_urb_enqueue,
	.urb_dequeue = ehci_urb_dequeue,
	.endpoint_disable = ehci_endpoint_disable,
	.endpoint_reset = ehci_endpoint_reset,
	.clear_tt_buffer_complete = ehci_clear_tt_buffer_complete,

	/*
	 * scheduling support
	 */
	.get_frame_number = ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data = ehci_hub_status_data,
	.hub_control = ehci_hub_control,
	.bus_suspend = ehci_bus_suspend,
	.bus_resume = ehci_bus_resume,
=======
	if (of_usb_get_phy_mode(dev->of_node) == USBPHY_INTERFACE_MODE_HSIC) {
		status = ehci_readl(ehci, &ehci->regs->port_status[0]);
		status |= PORT_TEST_FORCE;
		ehci_writel(ehci, status, &ehci->regs->port_status[0]);
		status &= ~PORT_TEST_FORCE;
		ehci_writel(ehci, status, &ehci->regs->port_status[0]);
	}

	return retval;
}

static struct hc_driver __read_mostly ehci_platform_hc_driver;

static const struct ehci_driver_overrides platform_overrides __initconst = {
	.reset =		mv_ehci_reset,
	.extra_priv_size =	sizeof(struct ehci_hcd_mv),
>>>>>>> upstream/android-13
};

static int mv_ehci_probe(struct platform_device *pdev)
{
	struct mv_usb_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct usb_hcd *hcd;
	struct ehci_hcd *ehci;
	struct ehci_hcd_mv *ehci_mv;
	struct resource *r;
<<<<<<< HEAD
	int retval = -ENODEV;
	u32 offset;

	if (!pdata) {
		dev_err(&pdev->dev, "missing platform_data\n");
		return -ENODEV;
	}
=======
	int retval;
	u32 offset;
	u32 status;
>>>>>>> upstream/android-13

	if (usb_disabled())
		return -ENODEV;

<<<<<<< HEAD
	hcd = usb_create_hcd(&mv_ehci_hc_driver, &pdev->dev, "mv ehci");
	if (!hcd)
		return -ENOMEM;

	ehci_mv = devm_kzalloc(&pdev->dev, sizeof(*ehci_mv), GFP_KERNEL);
	if (ehci_mv == NULL) {
		retval = -ENOMEM;
		goto err_put_hcd;
	}

	platform_set_drvdata(pdev, ehci_mv);
	ehci_mv->pdata = pdata;
	ehci_mv->hcd = hcd;
=======
	hcd = usb_create_hcd(&ehci_platform_hc_driver, &pdev->dev, dev_name(&pdev->dev));
	if (!hcd)
		return -ENOMEM;

	platform_set_drvdata(pdev, hcd);
	ehci_mv = hcd_to_ehci_hcd_mv(hcd);

	ehci_mv->mode = MV_USB_MODE_HOST;
	if (pdata) {
		ehci_mv->mode = pdata->mode;
		ehci_mv->set_vbus = pdata->set_vbus;
	}

	ehci_mv->phy = devm_phy_optional_get(&pdev->dev, "usb");
	if (IS_ERR(ehci_mv->phy)) {
		retval = PTR_ERR(ehci_mv->phy);
		if (retval != -EPROBE_DEFER)
			dev_err(&pdev->dev, "Failed to get phy.\n");
		goto err_put_hcd;
	}
>>>>>>> upstream/android-13

	ehci_mv->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(ehci_mv->clk)) {
		dev_err(&pdev->dev, "error getting clock\n");
		retval = PTR_ERR(ehci_mv->clk);
		goto err_put_hcd;
	}

<<<<<<< HEAD
	r = platform_get_resource_byname(pdev, IORESOURCE_MEM, "phyregs");
	ehci_mv->phy_regs = devm_ioremap_resource(&pdev->dev, r);
	if (IS_ERR(ehci_mv->phy_regs)) {
		retval = PTR_ERR(ehci_mv->phy_regs);
		goto err_put_hcd;
	}

	r = platform_get_resource_byname(pdev, IORESOURCE_MEM, "capregs");
	ehci_mv->cap_regs = devm_ioremap_resource(&pdev->dev, r);
	if (IS_ERR(ehci_mv->cap_regs)) {
		retval = PTR_ERR(ehci_mv->cap_regs);
=======
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	ehci_mv->base = devm_ioremap_resource(&pdev->dev, r);
	if (IS_ERR(ehci_mv->base)) {
		retval = PTR_ERR(ehci_mv->base);
>>>>>>> upstream/android-13
		goto err_put_hcd;
	}

	retval = mv_ehci_enable(ehci_mv);
	if (retval) {
		dev_err(&pdev->dev, "init phy error %d\n", retval);
		goto err_put_hcd;
	}

<<<<<<< HEAD
=======
	ehci_mv->cap_regs =
		(void __iomem *) ((unsigned long) ehci_mv->base + U2x_CAPREGS_OFFSET);
>>>>>>> upstream/android-13
	offset = readl(ehci_mv->cap_regs) & CAPLENGTH_MASK;
	ehci_mv->op_regs =
		(void __iomem *) ((unsigned long) ehci_mv->cap_regs + offset);

	hcd->rsrc_start = r->start;
	hcd->rsrc_len = resource_size(r);
	hcd->regs = ehci_mv->op_regs;

	retval = platform_get_irq(pdev, 0);
	if (retval < 0)
		goto err_disable_clk;
	hcd->irq = retval;

	ehci = hcd_to_ehci(hcd);
<<<<<<< HEAD
	ehci->caps = (struct ehci_caps *) ehci_mv->cap_regs;

	ehci_mv->mode = pdata->mode;
=======
	ehci->caps = (struct ehci_caps __iomem *) ehci_mv->cap_regs;

>>>>>>> upstream/android-13
	if (ehci_mv->mode == MV_USB_MODE_OTG) {
		ehci_mv->otg = devm_usb_get_phy(&pdev->dev, USB_PHY_TYPE_USB2);
		if (IS_ERR(ehci_mv->otg)) {
			retval = PTR_ERR(ehci_mv->otg);

			if (retval == -ENXIO)
				dev_info(&pdev->dev, "MV_USB_MODE_OTG "
						"must have CONFIG_USB_PHY enabled\n");
			else
				dev_err(&pdev->dev,
						"unable to find transceiver\n");
			goto err_disable_clk;
		}

		retval = otg_set_host(ehci_mv->otg->otg, &hcd->self);
		if (retval < 0) {
			dev_err(&pdev->dev,
				"unable to register with transceiver\n");
			retval = -ENODEV;
			goto err_disable_clk;
		}
		/* otg will enable clock before use as host */
		mv_ehci_disable(ehci_mv);
	} else {
<<<<<<< HEAD
		if (pdata->set_vbus)
			pdata->set_vbus(1);
=======
		if (ehci_mv->set_vbus)
			ehci_mv->set_vbus(1);
>>>>>>> upstream/android-13

		retval = usb_add_hcd(hcd, hcd->irq, IRQF_SHARED);
		if (retval) {
			dev_err(&pdev->dev,
				"failed to add hcd with err %d\n", retval);
			goto err_set_vbus;
		}
		device_wakeup_enable(hcd->self.controller);
	}

<<<<<<< HEAD
	if (pdata->private_init)
		pdata->private_init(ehci_mv->op_regs, ehci_mv->phy_regs);
=======
	if (of_usb_get_phy_mode(pdev->dev.of_node) == USBPHY_INTERFACE_MODE_HSIC) {
		status = ehci_readl(ehci, &ehci->regs->port_status[0]);
		/* These "reserved" bits actually enable HSIC mode. */
		status |= BIT(25);
		status &= ~GENMASK(31, 30);
		ehci_writel(ehci, status, &ehci->regs->port_status[0]);
	}
>>>>>>> upstream/android-13

	dev_info(&pdev->dev,
		 "successful find EHCI device with regs 0x%p irq %d"
		 " working in %s mode\n", hcd->regs, hcd->irq,
		 ehci_mv->mode == MV_USB_MODE_OTG ? "OTG" : "Host");

	return 0;

err_set_vbus:
<<<<<<< HEAD
	if (pdata->set_vbus)
		pdata->set_vbus(0);
=======
	if (ehci_mv->set_vbus)
		ehci_mv->set_vbus(0);
>>>>>>> upstream/android-13
err_disable_clk:
	mv_ehci_disable(ehci_mv);
err_put_hcd:
	usb_put_hcd(hcd);

	return retval;
}

static int mv_ehci_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct ehci_hcd_mv *ehci_mv = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = ehci_mv->hcd;
=======
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct ehci_hcd_mv *ehci_mv = hcd_to_ehci_hcd_mv(hcd);
>>>>>>> upstream/android-13

	if (hcd->rh_registered)
		usb_remove_hcd(hcd);

	if (!IS_ERR_OR_NULL(ehci_mv->otg))
		otg_set_host(ehci_mv->otg->otg, NULL);

	if (ehci_mv->mode == MV_USB_MODE_HOST) {
<<<<<<< HEAD
		if (ehci_mv->pdata->set_vbus)
			ehci_mv->pdata->set_vbus(0);
=======
		if (ehci_mv->set_vbus)
			ehci_mv->set_vbus(0);
>>>>>>> upstream/android-13

		mv_ehci_disable(ehci_mv);
	}

	usb_put_hcd(hcd);

	return 0;
}

MODULE_ALIAS("mv-ehci");

static const struct platform_device_id ehci_id_table[] = {
<<<<<<< HEAD
	{"pxa-u2oehci", PXA_U2OEHCI},
	{"pxa-sph", PXA_SPH},
	{"mmp3-hsic", MMP3_HSIC},
	{"mmp3-fsic", MMP3_FSIC},
=======
	{"pxa-u2oehci", 0},
	{"pxa-sph", 0},
>>>>>>> upstream/android-13
	{},
};

static void mv_ehci_shutdown(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct ehci_hcd_mv *ehci_mv = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = ehci_mv->hcd;
=======
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
>>>>>>> upstream/android-13

	if (!hcd->rh_registered)
		return;

	if (hcd->driver->shutdown)
		hcd->driver->shutdown(hcd);
}

<<<<<<< HEAD
=======
static const struct of_device_id ehci_mv_dt_ids[] = {
	{ .compatible = "marvell,pxau2o-ehci", },
	{},
};

>>>>>>> upstream/android-13
static struct platform_driver ehci_mv_driver = {
	.probe = mv_ehci_probe,
	.remove = mv_ehci_remove,
	.shutdown = mv_ehci_shutdown,
	.driver = {
<<<<<<< HEAD
		   .name = "mv-ehci",
		   .bus = &platform_bus_type,
		   },
	.id_table = ehci_id_table,
};
=======
		.name = "mv-ehci",
		.bus = &platform_bus_type,
		.of_match_table = ehci_mv_dt_ids,
	},
	.id_table = ehci_id_table,
};

static int __init ehci_platform_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	ehci_init_driver(&ehci_platform_hc_driver, &platform_overrides);
	return platform_driver_register(&ehci_mv_driver);
}
module_init(ehci_platform_init);

static void __exit ehci_platform_cleanup(void)
{
	platform_driver_unregister(&ehci_mv_driver);
}
module_exit(ehci_platform_cleanup);

MODULE_DESCRIPTION("Marvell EHCI driver");
MODULE_AUTHOR("Chao Xie <chao.xie@marvell.com>");
MODULE_AUTHOR("Neil Zhang <zhangwm@marvell.com>");
MODULE_ALIAS("mv-ehci");
MODULE_LICENSE("GPL");
MODULE_DEVICE_TABLE(of, ehci_mv_dt_ids);
>>>>>>> upstream/android-13
