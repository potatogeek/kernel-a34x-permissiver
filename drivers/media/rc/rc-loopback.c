<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Loopback driver for rc-core,
 *
 * Copyright (c) 2010 David Härdeman <david@hardeman.nu>
 *
 * This driver receives TX data and passes it back as RX data,
 * which is useful for (scripted) debugging of rc-core without
 * having to use actual hardware.
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
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/device.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <media/rc-core.h>

<<<<<<< HEAD
#define DRIVER_NAME	"rc-loopback"
#define dprintk(x...)	if (debug) printk(KERN_INFO DRIVER_NAME ": " x)
#define RXMASK_REGULAR	0x1
#define RXMASK_LEARNING	0x2

static bool debug;
=======
#define DRIVER_NAME		"rc-loopback"
#define RXMASK_NARROWBAND	0x1
#define RXMASK_WIDEBAND		0x2
>>>>>>> upstream/android-13

struct loopback_dev {
	struct rc_dev *dev;
	u32 txmask;
	u32 txcarrier;
	u32 txduty;
	bool idle;
<<<<<<< HEAD
	bool learning;
=======
	bool wideband;
>>>>>>> upstream/android-13
	bool carrierreport;
	u32 rxcarriermin;
	u32 rxcarriermax;
};

static struct loopback_dev loopdev;

static int loop_set_tx_mask(struct rc_dev *dev, u32 mask)
{
	struct loopback_dev *lodev = dev->priv;

<<<<<<< HEAD
	if ((mask & (RXMASK_REGULAR | RXMASK_LEARNING)) != mask) {
		dprintk("invalid tx mask: %u\n", mask);
		return -EINVAL;
	}

	dprintk("setting tx mask: %u\n", mask);
=======
	if ((mask & (RXMASK_NARROWBAND | RXMASK_WIDEBAND)) != mask) {
		dev_dbg(&dev->dev, "invalid tx mask: %u\n", mask);
		return 2;
	}

	dev_dbg(&dev->dev, "setting tx mask: %u\n", mask);
>>>>>>> upstream/android-13
	lodev->txmask = mask;
	return 0;
}

static int loop_set_tx_carrier(struct rc_dev *dev, u32 carrier)
{
	struct loopback_dev *lodev = dev->priv;

<<<<<<< HEAD
	dprintk("setting tx carrier: %u\n", carrier);
=======
	dev_dbg(&dev->dev, "setting tx carrier: %u\n", carrier);
>>>>>>> upstream/android-13
	lodev->txcarrier = carrier;
	return 0;
}

static int loop_set_tx_duty_cycle(struct rc_dev *dev, u32 duty_cycle)
{
	struct loopback_dev *lodev = dev->priv;

	if (duty_cycle < 1 || duty_cycle > 99) {
<<<<<<< HEAD
		dprintk("invalid duty cycle: %u\n", duty_cycle);
		return -EINVAL;
	}

	dprintk("setting duty cycle: %u\n", duty_cycle);
=======
		dev_dbg(&dev->dev, "invalid duty cycle: %u\n", duty_cycle);
		return -EINVAL;
	}

	dev_dbg(&dev->dev, "setting duty cycle: %u\n", duty_cycle);
>>>>>>> upstream/android-13
	lodev->txduty = duty_cycle;
	return 0;
}

static int loop_set_rx_carrier_range(struct rc_dev *dev, u32 min, u32 max)
{
	struct loopback_dev *lodev = dev->priv;

	if (min < 1 || min > max) {
<<<<<<< HEAD
		dprintk("invalid rx carrier range %u to %u\n", min, max);
		return -EINVAL;
	}

	dprintk("setting rx carrier range %u to %u\n", min, max);
=======
		dev_dbg(&dev->dev, "invalid rx carrier range %u to %u\n", min, max);
		return -EINVAL;
	}

	dev_dbg(&dev->dev, "setting rx carrier range %u to %u\n", min, max);
>>>>>>> upstream/android-13
	lodev->rxcarriermin = min;
	lodev->rxcarriermax = max;
	return 0;
}

static int loop_tx_ir(struct rc_dev *dev, unsigned *txbuf, unsigned count)
{
	struct loopback_dev *lodev = dev->priv;
	u32 rxmask;
	unsigned i;
<<<<<<< HEAD
	DEFINE_IR_RAW_EVENT(rawir);

	if (lodev->txcarrier < lodev->rxcarriermin ||
	    lodev->txcarrier > lodev->rxcarriermax) {
		dprintk("ignoring tx, carrier out of range\n");
		goto out;
	}

	if (lodev->learning)
		rxmask = RXMASK_LEARNING;
	else
		rxmask = RXMASK_REGULAR;

	if (!(rxmask & lodev->txmask)) {
		dprintk("ignoring tx, rx mask mismatch\n");
=======
	struct ir_raw_event rawir = {};

	if (lodev->txcarrier < lodev->rxcarriermin ||
	    lodev->txcarrier > lodev->rxcarriermax) {
		dev_dbg(&dev->dev, "ignoring tx, carrier out of range\n");
		goto out;
	}

	if (lodev->wideband)
		rxmask = RXMASK_WIDEBAND;
	else
		rxmask = RXMASK_NARROWBAND;

	if (!(rxmask & lodev->txmask)) {
		dev_dbg(&dev->dev, "ignoring tx, rx mask mismatch\n");
>>>>>>> upstream/android-13
		goto out;
	}

	for (i = 0; i < count; i++) {
		rawir.pulse = i % 2 ? false : true;
<<<<<<< HEAD
		rawir.duration = txbuf[i] * 1000;
		if (rawir.duration)
			ir_raw_event_store_with_filter(dev, &rawir);
=======
		rawir.duration = txbuf[i];

		ir_raw_event_store_with_filter(dev, &rawir);
	}

	if (lodev->carrierreport) {
		rawir.pulse = false;
		rawir.carrier_report = true;
		rawir.carrier = lodev->txcarrier;

		ir_raw_event_store(dev, &rawir);
>>>>>>> upstream/android-13
	}

	/* Fake a silence long enough to cause us to go idle */
	rawir.pulse = false;
	rawir.duration = dev->timeout;
	ir_raw_event_store_with_filter(dev, &rawir);

	ir_raw_event_handle(dev);

out:
	return count;
}

static void loop_set_idle(struct rc_dev *dev, bool enable)
{
	struct loopback_dev *lodev = dev->priv;

	if (lodev->idle != enable) {
<<<<<<< HEAD
		dprintk("%sing idle mode\n", enable ? "enter" : "exit");
=======
		dev_dbg(&dev->dev, "%sing idle mode\n", enable ? "enter" : "exit");
>>>>>>> upstream/android-13
		lodev->idle = enable;
	}
}

<<<<<<< HEAD
static int loop_set_learning_mode(struct rc_dev *dev, int enable)
{
	struct loopback_dev *lodev = dev->priv;

	if (lodev->learning != enable) {
		dprintk("%sing learning mode\n", enable ? "enter" : "exit");
		lodev->learning = !!enable;
=======
static int loop_set_wideband_receiver(struct rc_dev *dev, int enable)
{
	struct loopback_dev *lodev = dev->priv;

	if (lodev->wideband != enable) {
		dev_dbg(&dev->dev, "using %sband receiver\n", enable ? "wide" : "narrow");
		lodev->wideband = !!enable;
>>>>>>> upstream/android-13
	}

	return 0;
}

static int loop_set_carrier_report(struct rc_dev *dev, int enable)
{
	struct loopback_dev *lodev = dev->priv;

	if (lodev->carrierreport != enable) {
<<<<<<< HEAD
		dprintk("%sabling carrier reports\n", enable ? "en" : "dis");
=======
		dev_dbg(&dev->dev, "%sabling carrier reports\n", enable ? "en" : "dis");
>>>>>>> upstream/android-13
		lodev->carrierreport = !!enable;
	}

	return 0;
}

static int loop_set_wakeup_filter(struct rc_dev *dev,
				  struct rc_scancode_filter *sc)
{
	static const unsigned int max = 512;
	struct ir_raw_event *raw;
	int ret;
	int i;

	/* fine to disable filter */
	if (!sc->mask)
		return 0;

	/* encode the specified filter and loop it back */
	raw = kmalloc_array(max, sizeof(*raw), GFP_KERNEL);
	if (!raw)
		return -ENOMEM;

	ret = ir_raw_encode_scancode(dev->wakeup_protocol, sc->data, raw, max);
	/* still loop back the partial raw IR even if it's incomplete */
	if (ret == -ENOBUFS)
		ret = max;
	if (ret >= 0) {
		/* do the loopback */
		for (i = 0; i < ret; ++i)
			ir_raw_event_store(dev, &raw[i]);
		ir_raw_event_handle(dev);

		ret = 0;
	}

	kfree(raw);

	return ret;
}

static int __init loop_init(void)
{
	struct rc_dev *rc;
	int ret;

	rc = rc_allocate_device(RC_DRIVER_IR_RAW);
<<<<<<< HEAD
	if (!rc) {
		printk(KERN_ERR DRIVER_NAME ": rc_dev allocation failed\n");
		return -ENOMEM;
	}
=======
	if (!rc)
		return -ENOMEM;
>>>>>>> upstream/android-13

	rc->device_name		= "rc-core loopback device";
	rc->input_phys		= "rc-core/virtual";
	rc->input_id.bustype	= BUS_VIRTUAL;
	rc->input_id.version	= 1;
	rc->driver_name		= DRIVER_NAME;
	rc->map_name		= RC_MAP_EMPTY;
	rc->priv		= &loopdev;
	rc->allowed_protocols	= RC_PROTO_BIT_ALL_IR_DECODER;
	rc->allowed_wakeup_protocols = RC_PROTO_BIT_ALL_IR_ENCODER;
	rc->encode_wakeup	= true;
<<<<<<< HEAD
	rc->timeout		= 100 * 1000 * 1000; /* 100 ms */
	rc->min_timeout		= 1;
	rc->max_timeout		= UINT_MAX;
	rc->rx_resolution	= 1000;
	rc->tx_resolution	= 1000;
=======
	rc->timeout		= IR_DEFAULT_TIMEOUT;
	rc->min_timeout		= 1;
	rc->max_timeout		= IR_MAX_TIMEOUT;
	rc->rx_resolution	= 1;
	rc->tx_resolution	= 1;
>>>>>>> upstream/android-13
	rc->s_tx_mask		= loop_set_tx_mask;
	rc->s_tx_carrier	= loop_set_tx_carrier;
	rc->s_tx_duty_cycle	= loop_set_tx_duty_cycle;
	rc->s_rx_carrier_range	= loop_set_rx_carrier_range;
	rc->tx_ir		= loop_tx_ir;
	rc->s_idle		= loop_set_idle;
<<<<<<< HEAD
	rc->s_learning_mode	= loop_set_learning_mode;
	rc->s_carrier_report	= loop_set_carrier_report;
	rc->s_wakeup_filter	= loop_set_wakeup_filter;

	loopdev.txmask		= RXMASK_REGULAR;
=======
	rc->s_wideband_receiver	= loop_set_wideband_receiver;
	rc->s_carrier_report	= loop_set_carrier_report;
	rc->s_wakeup_filter	= loop_set_wakeup_filter;

	loopdev.txmask		= RXMASK_NARROWBAND;
>>>>>>> upstream/android-13
	loopdev.txcarrier	= 36000;
	loopdev.txduty		= 50;
	loopdev.rxcarriermin	= 1;
	loopdev.rxcarriermax	= ~0;
	loopdev.idle		= true;
<<<<<<< HEAD
	loopdev.learning	= false;
=======
	loopdev.wideband	= false;
>>>>>>> upstream/android-13
	loopdev.carrierreport	= false;

	ret = rc_register_device(rc);
	if (ret < 0) {
<<<<<<< HEAD
		printk(KERN_ERR DRIVER_NAME ": rc_dev registration failed\n");
=======
		dev_err(&rc->dev, "rc_dev registration failed\n");
>>>>>>> upstream/android-13
		rc_free_device(rc);
		return ret;
	}

	loopdev.dev = rc;
	return 0;
}

static void __exit loop_exit(void)
{
	rc_unregister_device(loopdev.dev);
}

module_init(loop_init);
module_exit(loop_exit);

<<<<<<< HEAD
module_param(debug, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "Enable debug messages");

=======
>>>>>>> upstream/android-13
MODULE_DESCRIPTION("Loopback device for rc-core debugging");
MODULE_AUTHOR("David Härdeman <david@hardeman.nu>");
MODULE_LICENSE("GPL");
