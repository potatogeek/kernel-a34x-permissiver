// SPDX-License-Identifier: GPL-1.0+
/*
 * Renesas USB driver
 *
 * Copyright (C) 2011 Renesas Solutions Corp.
<<<<<<< HEAD
 * Kuninori Morimoto <kuninori.morimoto.gx@renesas.com>
 */
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/pm_runtime.h>
=======
 * Copyright (C) 2019 Renesas Electronics Corporation
 * Kuninori Morimoto <kuninori.morimoto.gx@renesas.com>
 */
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/gpio/consumer.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/pm_runtime.h>
#include <linux/reset.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>
#include <linux/sysfs.h>
#include "common.h"
#include "rcar2.h"
#include "rcar3.h"
#include "rza.h"

/*
 *		image of renesas_usbhs
 *
 * ex) gadget case

 * mod.c
 * mod_gadget.c
 * mod_host.c		pipe.c		fifo.c
 *
 *			+-------+	+-----------+
 *			| pipe0 |------>| fifo pio  |
 * +------------+	+-------+	+-----------+
 * | mod_gadget |=====> | pipe1 |--+
 * +------------+	+-------+  |	+-----------+
 *			| pipe2 |  |  +-| fifo dma0 |
 * +------------+	+-------+  |  |	+-----------+
 * | mod_host   |	| pipe3 |<-|--+
 * +------------+	+-------+  |	+-----------+
 *			| ....  |  +--->| fifo dma1 |
 *			| ....  |	+-----------+
 */

<<<<<<< HEAD

#define USBHSF_RUNTIME_PWCTRL	(1 << 0)

/* status */
#define usbhsc_flags_init(p)   do {(p)->flags = 0; } while (0)
#define usbhsc_flags_set(p, b) ((p)->flags |=  (b))
#define usbhsc_flags_clr(p, b) ((p)->flags &= ~(b))
#define usbhsc_flags_has(p, b) ((p)->flags &   (b))

=======
>>>>>>> upstream/android-13
/*
 * platform call back
 *
 * renesas usb support platform callback function.
 * Below macro call it.
 * if platform doesn't have callback, it return 0 (no error)
 */
#define usbhs_platform_call(priv, func, args...)\
	(!(priv) ? -ENODEV :			\
<<<<<<< HEAD
	 !((priv)->pfunc.func) ? 0 :		\
	 (priv)->pfunc.func(args))
=======
	 !((priv)->pfunc->func) ? 0 :		\
	 (priv)->pfunc->func(args))
>>>>>>> upstream/android-13

/*
 *		common functions
 */
u16 usbhs_read(struct usbhs_priv *priv, u32 reg)
{
	return ioread16(priv->base + reg);
}

void usbhs_write(struct usbhs_priv *priv, u32 reg, u16 data)
{
	iowrite16(data, priv->base + reg);
}

void usbhs_bset(struct usbhs_priv *priv, u32 reg, u16 mask, u16 data)
{
	u16 val = usbhs_read(priv, reg);

	val &= ~mask;
	val |= data & mask;

	usbhs_write(priv, reg, val);
}

struct usbhs_priv *usbhs_pdev_to_priv(struct platform_device *pdev)
{
	return dev_get_drvdata(&pdev->dev);
}

<<<<<<< HEAD
=======
int usbhs_get_id_as_gadget(struct platform_device *pdev)
{
	return USBHS_GADGET;
}

>>>>>>> upstream/android-13
/*
 *		syscfg functions
 */
static void usbhs_sys_clock_ctrl(struct usbhs_priv *priv, int enable)
{
	usbhs_bset(priv, SYSCFG, SCKE, enable ? SCKE : 0);
}

void usbhs_sys_host_ctrl(struct usbhs_priv *priv, int enable)
{
	u16 mask = DCFM | DRPD | DPRPU | HSE | USBE;
	u16 val  = DCFM | DRPD | HSE | USBE;
<<<<<<< HEAD
	int has_otg = usbhs_get_dparam(priv, has_otg);

	if (has_otg)
		usbhs_bset(priv, DVSTCTR, (EXTLP | PWEN), (EXTLP | PWEN));
=======
>>>>>>> upstream/android-13

	/*
	 * if enable
	 *
	 * - select Host mode
	 * - D+ Line/D- Line Pull-down
	 */
	usbhs_bset(priv, SYSCFG, mask, enable ? val : 0);
}

void usbhs_sys_function_ctrl(struct usbhs_priv *priv, int enable)
{
	u16 mask = DCFM | DRPD | DPRPU | HSE | USBE;
	u16 val  = HSE | USBE;

<<<<<<< HEAD
=======
	/* CNEN bit is required for function operation */
	if (usbhs_get_dparam(priv, has_cnen)) {
		mask |= CNEN;
		val  |= CNEN;
	}

>>>>>>> upstream/android-13
	/*
	 * if enable
	 *
	 * - select Function mode
	 * - D+ Line Pull-up is disabled
	 *      When D+ Line Pull-up is enabled,
	 *      calling usbhs_sys_function_pullup(,1)
	 */
	usbhs_bset(priv, SYSCFG, mask, enable ? val : 0);
}

void usbhs_sys_function_pullup(struct usbhs_priv *priv, int enable)
{
	usbhs_bset(priv, SYSCFG, DPRPU, enable ? DPRPU : 0);
}

void usbhs_sys_set_test_mode(struct usbhs_priv *priv, u16 mode)
{
	usbhs_write(priv, TESTMODE, mode);
}

/*
 *		frame functions
 */
int usbhs_frame_get_num(struct usbhs_priv *priv)
{
	return usbhs_read(priv, FRMNUM) & FRNM_MASK;
}

/*
 *		usb request functions
 */
void usbhs_usbreq_get_val(struct usbhs_priv *priv, struct usb_ctrlrequest *req)
{
	u16 val;

	val = usbhs_read(priv, USBREQ);
	req->bRequest		= (val >> 8) & 0xFF;
	req->bRequestType	= (val >> 0) & 0xFF;

<<<<<<< HEAD
	req->wValue	= usbhs_read(priv, USBVAL);
	req->wIndex	= usbhs_read(priv, USBINDX);
	req->wLength	= usbhs_read(priv, USBLENG);
=======
	req->wValue	= cpu_to_le16(usbhs_read(priv, USBVAL));
	req->wIndex	= cpu_to_le16(usbhs_read(priv, USBINDX));
	req->wLength	= cpu_to_le16(usbhs_read(priv, USBLENG));
>>>>>>> upstream/android-13
}

void usbhs_usbreq_set_val(struct usbhs_priv *priv, struct usb_ctrlrequest *req)
{
	usbhs_write(priv, USBREQ,  (req->bRequest << 8) | req->bRequestType);
<<<<<<< HEAD
	usbhs_write(priv, USBVAL,  req->wValue);
	usbhs_write(priv, USBINDX, req->wIndex);
	usbhs_write(priv, USBLENG, req->wLength);
=======
	usbhs_write(priv, USBVAL,  le16_to_cpu(req->wValue));
	usbhs_write(priv, USBINDX, le16_to_cpu(req->wIndex));
	usbhs_write(priv, USBLENG, le16_to_cpu(req->wLength));
>>>>>>> upstream/android-13

	usbhs_bset(priv, DCPCTR, SUREQ, SUREQ);
}

/*
 *		bus/vbus functions
 */
void usbhs_bus_send_sof_enable(struct usbhs_priv *priv)
{
	u16 status = usbhs_read(priv, DVSTCTR) & (USBRST | UACT);

	if (status != USBRST) {
		struct device *dev = usbhs_priv_to_dev(priv);
		dev_err(dev, "usbhs should be reset\n");
	}

	usbhs_bset(priv, DVSTCTR, (USBRST | UACT), UACT);
}

void usbhs_bus_send_reset(struct usbhs_priv *priv)
{
	usbhs_bset(priv, DVSTCTR, (USBRST | UACT), USBRST);
}

int usbhs_bus_get_speed(struct usbhs_priv *priv)
{
	u16 dvstctr = usbhs_read(priv, DVSTCTR);

	switch (RHST & dvstctr) {
	case RHST_LOW_SPEED:
		return USB_SPEED_LOW;
	case RHST_FULL_SPEED:
		return USB_SPEED_FULL;
	case RHST_HIGH_SPEED:
		return USB_SPEED_HIGH;
	}

	return USB_SPEED_UNKNOWN;
}

int usbhs_vbus_ctrl(struct usbhs_priv *priv, int enable)
{
	struct platform_device *pdev = usbhs_priv_to_pdev(priv);

	return usbhs_platform_call(priv, set_vbus, pdev, enable);
}

static void usbhsc_bus_init(struct usbhs_priv *priv)
{
	usbhs_write(priv, DVSTCTR, 0);

	usbhs_vbus_ctrl(priv, 0);
}

/*
 *		device configuration
 */
int usbhs_set_device_config(struct usbhs_priv *priv, int devnum,
			   u16 upphub, u16 hubport, u16 speed)
{
	struct device *dev = usbhs_priv_to_dev(priv);
	u16 usbspd = 0;
	u32 reg = DEVADD0 + (2 * devnum);

	if (devnum > 10) {
		dev_err(dev, "cannot set speed to unknown device %d\n", devnum);
		return -EIO;
	}

	if (upphub > 0xA) {
		dev_err(dev, "unsupported hub number %d\n", upphub);
		return -EIO;
	}

	switch (speed) {
	case USB_SPEED_LOW:
		usbspd = USBSPD_SPEED_LOW;
		break;
	case USB_SPEED_FULL:
		usbspd = USBSPD_SPEED_FULL;
		break;
	case USB_SPEED_HIGH:
		usbspd = USBSPD_SPEED_HIGH;
		break;
	default:
		dev_err(dev, "unsupported speed %d\n", speed);
		return -EIO;
	}

	usbhs_write(priv, reg,	UPPHUB(upphub)	|
				HUBPORT(hubport)|
				USBSPD(usbspd));

	return 0;
}

/*
 *		interrupt functions
 */
void usbhs_xxxsts_clear(struct usbhs_priv *priv, u16 sts_reg, u16 bit)
{
	u16 pipe_mask = (u16)GENMASK(usbhs_get_dparam(priv, pipe_size), 0);

	usbhs_write(priv, sts_reg, ~(1 << bit) & pipe_mask);
}

/*
 *		local functions
 */
static void usbhsc_set_buswait(struct usbhs_priv *priv)
{
	int wait = usbhs_get_dparam(priv, buswait_bwait);

	/* set bus wait if platform have */
	if (wait)
		usbhs_bset(priv, BUSWAIT, 0x000F, wait);
}

<<<<<<< HEAD
=======
static bool usbhsc_is_multi_clks(struct usbhs_priv *priv)
{
	return priv->dparam.multi_clks;
}

static int usbhsc_clk_get(struct device *dev, struct usbhs_priv *priv)
{
	if (!usbhsc_is_multi_clks(priv))
		return 0;

	/* The first clock should exist */
	priv->clks[0] = of_clk_get(dev_of_node(dev), 0);
	if (IS_ERR(priv->clks[0]))
		return PTR_ERR(priv->clks[0]);

	/*
	 * To backward compatibility with old DT, this driver checks the return
	 * value if it's -ENOENT or not.
	 */
	priv->clks[1] = of_clk_get(dev_of_node(dev), 1);
	if (PTR_ERR(priv->clks[1]) == -ENOENT)
		priv->clks[1] = NULL;
	else if (IS_ERR(priv->clks[1]))
		return PTR_ERR(priv->clks[1]);

	return 0;
}

static void usbhsc_clk_put(struct usbhs_priv *priv)
{
	int i;

	if (!usbhsc_is_multi_clks(priv))
		return;

	for (i = 0; i < ARRAY_SIZE(priv->clks); i++)
		clk_put(priv->clks[i]);
}

static int usbhsc_clk_prepare_enable(struct usbhs_priv *priv)
{
	int i, ret;

	if (!usbhsc_is_multi_clks(priv))
		return 0;

	for (i = 0; i < ARRAY_SIZE(priv->clks); i++) {
		ret = clk_prepare_enable(priv->clks[i]);
		if (ret) {
			while (--i >= 0)
				clk_disable_unprepare(priv->clks[i]);
			return ret;
		}
	}

	return ret;
}

static void usbhsc_clk_disable_unprepare(struct usbhs_priv *priv)
{
	int i;

	if (!usbhsc_is_multi_clks(priv))
		return;

	for (i = 0; i < ARRAY_SIZE(priv->clks); i++)
		clk_disable_unprepare(priv->clks[i]);
}

>>>>>>> upstream/android-13
/*
 *		platform default param
 */

/* commonly used on old SH-Mobile SoCs */
static struct renesas_usbhs_driver_pipe_config usbhsc_default_pipe[] = {
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_CONTROL, 64, 0x00, false),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_ISOC, 1024, 0x08, false),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_ISOC, 1024, 0x18, false),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_BULK, 512, 0x28, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_BULK, 512, 0x38, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_BULK, 512, 0x48, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_INT, 64, 0x04, false),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_INT, 64, 0x05, false),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_INT, 64, 0x06, false),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_INT, 64, 0x07, false),
};

/* commonly used on newer SH-Mobile and R-Car SoCs */
static struct renesas_usbhs_driver_pipe_config usbhsc_new_pipe[] = {
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_CONTROL, 64, 0x00, false),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_ISOC, 1024, 0x08, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_ISOC, 1024, 0x28, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_BULK, 512, 0x48, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_BULK, 512, 0x58, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_BULK, 512, 0x68, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_INT, 64, 0x04, false),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_INT, 64, 0x05, false),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_INT, 64, 0x06, false),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_BULK, 512, 0x78, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_BULK, 512, 0x88, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_BULK, 512, 0x98, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_BULK, 512, 0xa8, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_BULK, 512, 0xb8, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_BULK, 512, 0xc8, true),
	RENESAS_USBHS_PIPE(USB_ENDPOINT_XFER_BULK, 512, 0xd8, true),
};

/*
 *		power control
 */
static void usbhsc_power_ctrl(struct usbhs_priv *priv, int enable)
{
	struct platform_device *pdev = usbhs_priv_to_pdev(priv);
	struct device *dev = usbhs_priv_to_dev(priv);

	if (enable) {
		/* enable PM */
		pm_runtime_get_sync(dev);

<<<<<<< HEAD
=======
		/* enable clks */
		if (usbhsc_clk_prepare_enable(priv))
			return;

>>>>>>> upstream/android-13
		/* enable platform power */
		usbhs_platform_call(priv, power_ctrl, pdev, priv->base, enable);

		/* USB on */
		usbhs_sys_clock_ctrl(priv, enable);
	} else {
		/* USB off */
		usbhs_sys_clock_ctrl(priv, enable);

		/* disable platform power */
		usbhs_platform_call(priv, power_ctrl, pdev, priv->base, enable);

<<<<<<< HEAD
=======
		/* disable clks */
		usbhsc_clk_disable_unprepare(priv);

>>>>>>> upstream/android-13
		/* disable PM */
		pm_runtime_put_sync(dev);
	}
}

/*
 *		hotplug
 */
static void usbhsc_hotplug(struct usbhs_priv *priv)
{
	struct platform_device *pdev = usbhs_priv_to_pdev(priv);
	struct usbhs_mod *mod = usbhs_mod_get_current(priv);
	int id;
	int enable;
	int cable;
	int ret;

	/*
	 * get vbus status from platform
	 */
<<<<<<< HEAD
	enable = usbhs_platform_call(priv, get_vbus, pdev);
=======
	enable = usbhs_mod_info_call(priv, get_vbus, pdev);
>>>>>>> upstream/android-13

	/*
	 * get id from platform
	 */
	id = usbhs_platform_call(priv, get_id, pdev);

	if (enable && !mod) {
		if (priv->edev) {
			cable = extcon_get_state(priv->edev, EXTCON_USB_HOST);
			if ((cable > 0 && id != USBHS_HOST) ||
			    (!cable && id != USBHS_GADGET)) {
				dev_info(&pdev->dev,
					 "USB cable plugged in doesn't match the selected role!\n");
				return;
			}
		}

		ret = usbhs_mod_change(priv, id);
		if (ret < 0)
			return;

		dev_dbg(&pdev->dev, "%s enable\n", __func__);

		/* power on */
<<<<<<< HEAD
		if (usbhsc_flags_has(priv, USBHSF_RUNTIME_PWCTRL))
=======
		if (usbhs_get_dparam(priv, runtime_pwctrl))
>>>>>>> upstream/android-13
			usbhsc_power_ctrl(priv, enable);

		/* bus init */
		usbhsc_set_buswait(priv);
		usbhsc_bus_init(priv);

		/* module start */
		usbhs_mod_call(priv, start, priv);

	} else if (!enable && mod) {
		dev_dbg(&pdev->dev, "%s disable\n", __func__);

		/* module stop */
		usbhs_mod_call(priv, stop, priv);

		/* bus init */
		usbhsc_bus_init(priv);

		/* power off */
<<<<<<< HEAD
		if (usbhsc_flags_has(priv, USBHSF_RUNTIME_PWCTRL))
=======
		if (usbhs_get_dparam(priv, runtime_pwctrl))
>>>>>>> upstream/android-13
			usbhsc_power_ctrl(priv, enable);

		usbhs_mod_change(priv, -1);

		/* reset phy for next connection */
		usbhs_platform_call(priv, phy_reset, pdev);
	}
}

/*
 *		notify hotplug
 */
static void usbhsc_notify_hotplug(struct work_struct *work)
{
	struct usbhs_priv *priv = container_of(work,
					       struct usbhs_priv,
					       notify_hotplug_work.work);
	usbhsc_hotplug(priv);
}

<<<<<<< HEAD
static int usbhsc_drvcllbck_notify_hotplug(struct platform_device *pdev)
=======
int usbhsc_schedule_notify_hotplug(struct platform_device *pdev)
>>>>>>> upstream/android-13
{
	struct usbhs_priv *priv = usbhs_pdev_to_priv(pdev);
	int delay = usbhs_get_dparam(priv, detection_delay);

	/*
	 * This functions will be called in interrupt.
	 * To make sure safety context,
	 * use workqueue for usbhs_notify_hotplug
	 */
	schedule_delayed_work(&priv->notify_hotplug_work,
			      msecs_to_jiffies(delay));
	return 0;
}

/*
 *		platform functions
 */
static const struct of_device_id usbhs_of_match[] = {
	{
		.compatible = "renesas,usbhs-r8a774c0",
<<<<<<< HEAD
		.data = (void *)USBHS_TYPE_RCAR_GEN3_WITH_PLL,
	},
	{
		.compatible = "renesas,usbhs-r8a7790",
		.data = (void *)USBHS_TYPE_RCAR_GEN2,
	},
	{
		.compatible = "renesas,usbhs-r8a7791",
		.data = (void *)USBHS_TYPE_RCAR_GEN2,
	},
	{
		.compatible = "renesas,usbhs-r8a7794",
		.data = (void *)USBHS_TYPE_RCAR_GEN2,
	},
	{
		.compatible = "renesas,usbhs-r8a7795",
		.data = (void *)USBHS_TYPE_RCAR_GEN3,
	},
	{
		.compatible = "renesas,usbhs-r8a7796",
		.data = (void *)USBHS_TYPE_RCAR_GEN3,
	},
	{
		.compatible = "renesas,usbhs-r8a77995",
		.data = (void *)USBHS_TYPE_RCAR_GEN3_WITH_PLL,
	},
	{
		.compatible = "renesas,rcar-gen2-usbhs",
		.data = (void *)USBHS_TYPE_RCAR_GEN2,
	},
	{
		.compatible = "renesas,rcar-gen3-usbhs",
		.data = (void *)USBHS_TYPE_RCAR_GEN3,
	},
	{
		.compatible = "renesas,rza1-usbhs",
		.data = (void *)USBHS_TYPE_RZA1,
=======
		.data = &usbhs_rcar_gen3_with_pll_plat_info,
	},
	{
		.compatible = "renesas,usbhs-r8a7790",
		.data = &usbhs_rcar_gen2_plat_info,
	},
	{
		.compatible = "renesas,usbhs-r8a7791",
		.data = &usbhs_rcar_gen2_plat_info,
	},
	{
		.compatible = "renesas,usbhs-r8a7794",
		.data = &usbhs_rcar_gen2_plat_info,
	},
	{
		.compatible = "renesas,usbhs-r8a7795",
		.data = &usbhs_rcar_gen3_plat_info,
	},
	{
		.compatible = "renesas,usbhs-r8a7796",
		.data = &usbhs_rcar_gen3_plat_info,
	},
	{
		.compatible = "renesas,usbhs-r8a77990",
		.data = &usbhs_rcar_gen3_with_pll_plat_info,
	},
	{
		.compatible = "renesas,usbhs-r8a77995",
		.data = &usbhs_rcar_gen3_with_pll_plat_info,
	},
	{
		.compatible = "renesas,rcar-gen2-usbhs",
		.data = &usbhs_rcar_gen2_plat_info,
	},
	{
		.compatible = "renesas,rcar-gen3-usbhs",
		.data = &usbhs_rcar_gen3_plat_info,
	},
	{
		.compatible = "renesas,rza1-usbhs",
		.data = &usbhs_rza1_plat_info,
	},
	{
		.compatible = "renesas,rza2-usbhs",
		.data = &usbhs_rza2_plat_info,
>>>>>>> upstream/android-13
	},
	{ },
};
MODULE_DEVICE_TABLE(of, usbhs_of_match);

<<<<<<< HEAD
static struct renesas_usbhs_platform_info *usbhs_parse_dt(struct device *dev)
{
	struct renesas_usbhs_platform_info *info;
	struct renesas_usbhs_driver_param *dparam;
	u32 tmp;
	int gpio;

	info = devm_kzalloc(dev, sizeof(*info), GFP_KERNEL);
	if (!info)
		return NULL;

	dparam = &info->driver_param;
	dparam->type = (uintptr_t)of_device_get_match_data(dev);
	if (!of_property_read_u32(dev->of_node, "renesas,buswait", &tmp))
		dparam->buswait_bwait = tmp;
	gpio = of_get_named_gpio_flags(dev->of_node, "renesas,enable-gpio", 0,
				       NULL);
	if (gpio > 0)
		dparam->enable_gpio = gpio;

	if (dparam->type == USBHS_TYPE_RCAR_GEN2 ||
	    dparam->type == USBHS_TYPE_RCAR_GEN3 ||
	    dparam->type == USBHS_TYPE_RCAR_GEN3_WITH_PLL) {
		dparam->has_usb_dmac = 1;
		dparam->pipe_configs = usbhsc_new_pipe;
		dparam->pipe_size = ARRAY_SIZE(usbhsc_new_pipe);
	}

	if (dparam->type == USBHS_TYPE_RZA1) {
		dparam->pipe_configs = usbhsc_new_pipe;
		dparam->pipe_size = ARRAY_SIZE(usbhsc_new_pipe);
	}

	return info;
}

static int usbhs_probe(struct platform_device *pdev)
{
	struct renesas_usbhs_platform_info *info = renesas_usbhs_get_info(pdev);
	struct renesas_usbhs_driver_callback *dfunc;
	struct usbhs_priv *priv;
	struct resource *res, *irq_res;
	int ret;

	/* check device node */
	if (pdev->dev.of_node)
		info = pdev->dev.platform_data = usbhs_parse_dt(&pdev->dev);

	/* check platform information */
	if (!info) {
		dev_err(&pdev->dev, "no platform information\n");
=======
static int usbhs_probe(struct platform_device *pdev)
{
	const struct renesas_usbhs_platform_info *info;
	struct usbhs_priv *priv;
	struct resource *irq_res;
	struct device *dev = &pdev->dev;
	struct gpio_desc *gpiod;
	int ret;
	u32 tmp;

	/* check device node */
	if (dev_of_node(dev))
		info = of_device_get_match_data(dev);
	else
		info = renesas_usbhs_get_info(pdev);

	/* check platform information */
	if (!info) {
		dev_err(dev, "no platform information\n");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	/* platform data */
	irq_res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!irq_res) {
<<<<<<< HEAD
		dev_err(&pdev->dev, "Not enough Renesas USB platform resources.\n");
=======
		dev_err(dev, "Not enough Renesas USB platform resources.\n");
>>>>>>> upstream/android-13
		return -ENODEV;
	}

	/* usb private data */
<<<<<<< HEAD
	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	priv->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(priv->base))
		return PTR_ERR(priv->base);

	if (of_property_read_bool(pdev->dev.of_node, "extcon")) {
		priv->edev = extcon_get_edev_by_phandle(&pdev->dev, 0);
=======
	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(priv->base))
		return PTR_ERR(priv->base);

	if (of_property_read_bool(dev_of_node(dev), "extcon")) {
		priv->edev = extcon_get_edev_by_phandle(dev, 0);
>>>>>>> upstream/android-13
		if (IS_ERR(priv->edev))
			return PTR_ERR(priv->edev);
	}

<<<<<<< HEAD
=======
	priv->rsts = devm_reset_control_array_get_optional_shared(dev);
	if (IS_ERR(priv->rsts))
		return PTR_ERR(priv->rsts);

>>>>>>> upstream/android-13
	/*
	 * care platform info
	 */

<<<<<<< HEAD
	memcpy(&priv->dparam,
	       &info->driver_param,
	       sizeof(struct renesas_usbhs_driver_param));

	switch (priv->dparam.type) {
	case USBHS_TYPE_RCAR_GEN2:
		priv->pfunc = usbhs_rcar2_ops;
		break;
	case USBHS_TYPE_RCAR_GEN3:
		priv->pfunc = usbhs_rcar3_ops;
		break;
	case USBHS_TYPE_RCAR_GEN3_WITH_PLL:
		priv->pfunc = usbhs_rcar3_with_pll_ops;
		if (!IS_ERR_OR_NULL(priv->edev)) {
			priv->nb.notifier_call = priv->pfunc.notifier;
			ret = devm_extcon_register_notifier(&pdev->dev,
							    priv->edev,
							    EXTCON_USB_HOST,
							    &priv->nb);
			if (ret < 0)
				dev_err(&pdev->dev, "no notifier registered\n");
		}
		break;
	case USBHS_TYPE_RZA1:
		priv->pfunc = usbhs_rza1_ops;
		break;
	default:
		if (!info->platform_callback.get_id) {
			dev_err(&pdev->dev, "no platform callbacks");
			return -EINVAL;
		}
		memcpy(&priv->pfunc,
		       &info->platform_callback,
		       sizeof(struct renesas_usbhs_platform_callback));
		break;
	}

	/* set driver callback functions for platform */
	dfunc			= &info->driver_callback;
	dfunc->notify_hotplug	= usbhsc_drvcllbck_notify_hotplug;

	/* set default param if platform doesn't have */
	if (!priv->dparam.pipe_configs) {
=======
	priv->dparam = info->driver_param;

	if (!info->platform_callback.get_id) {
		dev_err(dev, "no platform callbacks\n");
		return -EINVAL;
	}
	priv->pfunc = &info->platform_callback;

	/* set default param if platform doesn't have */
	if (usbhs_get_dparam(priv, has_new_pipe_configs)) {
		priv->dparam.pipe_configs = usbhsc_new_pipe;
		priv->dparam.pipe_size = ARRAY_SIZE(usbhsc_new_pipe);
	} else if (!priv->dparam.pipe_configs) {
>>>>>>> upstream/android-13
		priv->dparam.pipe_configs = usbhsc_default_pipe;
		priv->dparam.pipe_size = ARRAY_SIZE(usbhsc_default_pipe);
	}
	if (!priv->dparam.pio_dma_border)
		priv->dparam.pio_dma_border = 64; /* 64byte */
<<<<<<< HEAD

	/* FIXME */
	/* runtime power control ? */
	if (priv->pfunc.get_vbus)
		usbhsc_flags_set(priv, USBHSF_RUNTIME_PWCTRL);
=======
	if (!of_property_read_u32(dev_of_node(dev), "renesas,buswait", &tmp))
		priv->dparam.buswait_bwait = tmp;
	gpiod = devm_gpiod_get_optional(dev, "renesas,enable", GPIOD_IN);
	if (IS_ERR(gpiod))
		return PTR_ERR(gpiod);

	/* FIXME */
	/* runtime power control ? */
	if (priv->pfunc->get_vbus)
		usbhs_get_dparam(priv, runtime_pwctrl) = 1;
>>>>>>> upstream/android-13

	/*
	 * priv settings
	 */
	priv->irq	= irq_res->start;
	if (irq_res->flags & IORESOURCE_IRQ_SHAREABLE)
		priv->irqflags = IRQF_SHARED;
	priv->pdev	= pdev;
	INIT_DELAYED_WORK(&priv->notify_hotplug_work, usbhsc_notify_hotplug);
	spin_lock_init(usbhs_priv_to_lock(priv));

	/* call pipe and module init */
	ret = usbhs_pipe_probe(priv);
	if (ret < 0)
		return ret;

	ret = usbhs_fifo_probe(priv);
	if (ret < 0)
		goto probe_end_pipe_exit;

	ret = usbhs_mod_probe(priv);
	if (ret < 0)
		goto probe_end_fifo_exit;

	/* dev_set_drvdata should be called after usbhs_mod_init */
	platform_set_drvdata(pdev, priv);

<<<<<<< HEAD
=======
	ret = reset_control_deassert(priv->rsts);
	if (ret)
		goto probe_fail_rst;

	ret = usbhsc_clk_get(dev, priv);
	if (ret)
		goto probe_fail_clks;

>>>>>>> upstream/android-13
	/*
	 * deviece reset here because
	 * USB device might be used in boot loader.
	 */
	usbhs_sys_clock_ctrl(priv, 0);

	/* check GPIO determining if USB function should be enabled */
<<<<<<< HEAD
	if (priv->dparam.enable_gpio) {
		gpio_request_one(priv->dparam.enable_gpio, GPIOF_IN, NULL);
		ret = !gpio_get_value(priv->dparam.enable_gpio);
		gpio_free(priv->dparam.enable_gpio);
		if (ret) {
			dev_warn(&pdev->dev,
				 "USB function not selected (GPIO %d)\n",
				 priv->dparam.enable_gpio);
=======
	if (gpiod) {
		ret = !gpiod_get_value(gpiod);
		if (ret) {
			dev_warn(dev, "USB function not selected (GPIO)\n");
>>>>>>> upstream/android-13
			ret = -ENOTSUPP;
			goto probe_end_mod_exit;
		}
	}

	/*
	 * platform call
	 *
	 * USB phy setup might depend on CPU/Board.
	 * If platform has its callback functions,
	 * call it here.
	 */
	ret = usbhs_platform_call(priv, hardware_init, pdev);
	if (ret < 0) {
<<<<<<< HEAD
		dev_err(&pdev->dev, "platform init failed.\n");
=======
		dev_err(dev, "platform init failed.\n");
>>>>>>> upstream/android-13
		goto probe_end_mod_exit;
	}

	/* reset phy for connection */
	usbhs_platform_call(priv, phy_reset, pdev);

	/* power control */
<<<<<<< HEAD
	pm_runtime_enable(&pdev->dev);
	if (!usbhsc_flags_has(priv, USBHSF_RUNTIME_PWCTRL)) {
		usbhsc_power_ctrl(priv, 1);
		usbhs_mod_autonomy_mode(priv);
=======
	pm_runtime_enable(dev);
	if (!usbhs_get_dparam(priv, runtime_pwctrl)) {
		usbhsc_power_ctrl(priv, 1);
		usbhs_mod_autonomy_mode(priv);
	} else {
		usbhs_mod_non_autonomy_mode(priv);
>>>>>>> upstream/android-13
	}

	/*
	 * manual call notify_hotplug for cold plug
	 */
<<<<<<< HEAD
	usbhsc_drvcllbck_notify_hotplug(pdev);

	dev_info(&pdev->dev, "probed\n");
=======
	usbhsc_schedule_notify_hotplug(pdev);

	dev_info(dev, "probed\n");
>>>>>>> upstream/android-13

	return ret;

probe_end_mod_exit:
<<<<<<< HEAD
=======
	usbhsc_clk_put(priv);
probe_fail_clks:
	reset_control_assert(priv->rsts);
probe_fail_rst:
>>>>>>> upstream/android-13
	usbhs_mod_remove(priv);
probe_end_fifo_exit:
	usbhs_fifo_remove(priv);
probe_end_pipe_exit:
	usbhs_pipe_remove(priv);

<<<<<<< HEAD
	dev_info(&pdev->dev, "probe failed (%d)\n", ret);
=======
	dev_info(dev, "probe failed (%d)\n", ret);
>>>>>>> upstream/android-13

	return ret;
}

static int usbhs_remove(struct platform_device *pdev)
{
	struct usbhs_priv *priv = usbhs_pdev_to_priv(pdev);
<<<<<<< HEAD
	struct renesas_usbhs_platform_info *info = renesas_usbhs_get_info(pdev);
	struct renesas_usbhs_driver_callback *dfunc = &info->driver_callback;

	dev_dbg(&pdev->dev, "usb remove\n");

	dfunc->notify_hotplug = NULL;

	/* power off */
	if (!usbhsc_flags_has(priv, USBHSF_RUNTIME_PWCTRL))
=======

	dev_dbg(&pdev->dev, "usb remove\n");

	/* power off */
	if (!usbhs_get_dparam(priv, runtime_pwctrl))
>>>>>>> upstream/android-13
		usbhsc_power_ctrl(priv, 0);

	pm_runtime_disable(&pdev->dev);

	usbhs_platform_call(priv, hardware_exit, pdev);
<<<<<<< HEAD
=======
	usbhsc_clk_put(priv);
	reset_control_assert(priv->rsts);
>>>>>>> upstream/android-13
	usbhs_mod_remove(priv);
	usbhs_fifo_remove(priv);
	usbhs_pipe_remove(priv);

	return 0;
}

<<<<<<< HEAD
static int usbhsc_suspend(struct device *dev)
=======
static __maybe_unused int usbhsc_suspend(struct device *dev)
>>>>>>> upstream/android-13
{
	struct usbhs_priv *priv = dev_get_drvdata(dev);
	struct usbhs_mod *mod = usbhs_mod_get_current(priv);

	if (mod) {
		usbhs_mod_call(priv, stop, priv);
		usbhs_mod_change(priv, -1);
	}

<<<<<<< HEAD
	if (mod || !usbhsc_flags_has(priv, USBHSF_RUNTIME_PWCTRL))
=======
	if (mod || !usbhs_get_dparam(priv, runtime_pwctrl))
>>>>>>> upstream/android-13
		usbhsc_power_ctrl(priv, 0);

	return 0;
}

<<<<<<< HEAD
static int usbhsc_resume(struct device *dev)
=======
static __maybe_unused int usbhsc_resume(struct device *dev)
>>>>>>> upstream/android-13
{
	struct usbhs_priv *priv = dev_get_drvdata(dev);
	struct platform_device *pdev = usbhs_priv_to_pdev(priv);

<<<<<<< HEAD
	if (!usbhsc_flags_has(priv, USBHSF_RUNTIME_PWCTRL)) {
=======
	if (!usbhs_get_dparam(priv, runtime_pwctrl)) {
>>>>>>> upstream/android-13
		usbhsc_power_ctrl(priv, 1);
		usbhs_mod_autonomy_mode(priv);
	}

	usbhs_platform_call(priv, phy_reset, pdev);

<<<<<<< HEAD
	usbhsc_drvcllbck_notify_hotplug(pdev);
=======
	usbhsc_schedule_notify_hotplug(pdev);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int usbhsc_runtime_nop(struct device *dev)
{
	/* Runtime PM callback shared between ->runtime_suspend()
	 * and ->runtime_resume(). Simply returns success.
	 *
	 * This driver re-initializes all registers after
	 * pm_runtime_get_sync() anyway so there is no need
	 * to save and restore registers here.
	 */
	return 0;
}

static const struct dev_pm_ops usbhsc_pm_ops = {
	.suspend		= usbhsc_suspend,
	.resume			= usbhsc_resume,
	.runtime_suspend	= usbhsc_runtime_nop,
	.runtime_resume		= usbhsc_runtime_nop,
};
=======
static SIMPLE_DEV_PM_OPS(usbhsc_pm_ops, usbhsc_suspend, usbhsc_resume);
>>>>>>> upstream/android-13

static struct platform_driver renesas_usbhs_driver = {
	.driver		= {
		.name	= "renesas_usbhs",
		.pm	= &usbhsc_pm_ops,
		.of_match_table = of_match_ptr(usbhs_of_match),
	},
	.probe		= usbhs_probe,
	.remove		= usbhs_remove,
};

module_platform_driver(renesas_usbhs_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Renesas USB driver");
MODULE_AUTHOR("Kuninori Morimoto <kuninori.morimoto.gx@renesas.com>");
