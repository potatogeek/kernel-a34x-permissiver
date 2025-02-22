<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * OMAP4 Keypad Driver
 *
 * Copyright (C) 2010 Texas Instruments
 *
 * Author: Abraham Arce <x0066660@ti.com>
 * Initial Code: Syed Rafiuddin <rafiuddin.syed@ti.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/errno.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/input.h>
#include <linux/input/matrix_keypad.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>
<<<<<<< HEAD
=======
#include <linux/pm_wakeirq.h>
>>>>>>> upstream/android-13

/* OMAP4 registers */
#define OMAP4_KBD_REVISION		0x00
#define OMAP4_KBD_SYSCONFIG		0x10
#define OMAP4_KBD_SYSSTATUS		0x14
#define OMAP4_KBD_IRQSTATUS		0x18
#define OMAP4_KBD_IRQENABLE		0x1C
#define OMAP4_KBD_WAKEUPENABLE		0x20
#define OMAP4_KBD_PENDING		0x24
#define OMAP4_KBD_CTRL			0x28
#define OMAP4_KBD_DEBOUNCINGTIME	0x2C
#define OMAP4_KBD_LONGKEYTIME		0x30
#define OMAP4_KBD_TIMEOUT		0x34
#define OMAP4_KBD_STATEMACHINE		0x38
#define OMAP4_KBD_ROWINPUTS		0x3C
#define OMAP4_KBD_COLUMNOUTPUTS		0x40
#define OMAP4_KBD_FULLCODE31_0		0x44
#define OMAP4_KBD_FULLCODE63_32		0x48

/* OMAP4 bit definitions */
#define OMAP4_DEF_IRQENABLE_EVENTEN	BIT(0)
#define OMAP4_DEF_IRQENABLE_LONGKEY	BIT(1)
#define OMAP4_DEF_WUP_EVENT_ENA		BIT(0)
#define OMAP4_DEF_WUP_LONG_KEY_ENA	BIT(1)
#define OMAP4_DEF_CTRL_NOSOFTMODE	BIT(1)
#define OMAP4_DEF_CTRL_PTV_SHIFT	2

/* OMAP4 values */
#define OMAP4_VAL_IRQDISABLE		0x0

/*
 * Errata i689: If a key is released for a time shorter than debounce time,
 * the keyboard will idle and never detect the key release. The workaround
 * is to use at least a 12ms debounce time. See omap5432 TRM chapter
 * "26.4.6.2 Keyboard Controller Timer" for more information.
 */
#define OMAP4_KEYPAD_PTV_DIV_128        0x6
#define OMAP4_KEYPAD_DEBOUNCINGTIME_MS(dbms, ptv)     \
	((((dbms) * 1000) / ((1 << ((ptv) + 1)) * (1000000 / 32768))) - 1)
#define OMAP4_VAL_DEBOUNCINGTIME_16MS					\
	OMAP4_KEYPAD_DEBOUNCINGTIME_MS(16, OMAP4_KEYPAD_PTV_DIV_128)
<<<<<<< HEAD
=======
#define OMAP4_KEYPAD_AUTOIDLE_MS	50	/* Approximate measured time */
#define OMAP4_KEYPAD_IDLE_CHECK_MS	(OMAP4_KEYPAD_AUTOIDLE_MS / 2)
>>>>>>> upstream/android-13

enum {
	KBD_REVISION_OMAP4 = 0,
	KBD_REVISION_OMAP5,
};

struct omap4_keypad {
	struct input_dev *input;

	void __iomem *base;
<<<<<<< HEAD
	bool irq_wake_enabled;
	unsigned int irq;
=======
	unsigned int irq;
	struct mutex lock;		/* for key scan */
>>>>>>> upstream/android-13

	unsigned int rows;
	unsigned int cols;
	u32 reg_offset;
	u32 irqreg_offset;
	unsigned int row_shift;
	bool no_autorepeat;
<<<<<<< HEAD
	unsigned char key_state[8];
=======
	u64 keys;
>>>>>>> upstream/android-13
	unsigned short *keymap;
};

static int kbd_readl(struct omap4_keypad *keypad_data, u32 offset)
{
	return __raw_readl(keypad_data->base +
				keypad_data->reg_offset + offset);
}

static void kbd_writel(struct omap4_keypad *keypad_data, u32 offset, u32 value)
{
	__raw_writel(value,
		     keypad_data->base + keypad_data->reg_offset + offset);
}

static int kbd_read_irqreg(struct omap4_keypad *keypad_data, u32 offset)
{
	return __raw_readl(keypad_data->base +
				keypad_data->irqreg_offset + offset);
}

static void kbd_write_irqreg(struct omap4_keypad *keypad_data,
			     u32 offset, u32 value)
{
	__raw_writel(value,
		     keypad_data->base + keypad_data->irqreg_offset + offset);
}

<<<<<<< HEAD
=======
static int omap4_keypad_report_keys(struct omap4_keypad *keypad_data,
				    u64 keys, bool down)
{
	struct input_dev *input_dev = keypad_data->input;
	unsigned int col, row, code;
	DECLARE_BITMAP(mask, 64);
	unsigned long bit;
	int events = 0;

	bitmap_from_u64(mask, keys);

	for_each_set_bit(bit, mask, keypad_data->rows * BITS_PER_BYTE) {
		row = bit / BITS_PER_BYTE;
		col = bit % BITS_PER_BYTE;
		code = MATRIX_SCAN_CODE(row, col, keypad_data->row_shift);

		input_event(input_dev, EV_MSC, MSC_SCAN, code);
		input_report_key(input_dev, keypad_data->keymap[code], down);

		events++;
	}

	if (events)
		input_sync(input_dev);

	return events;
}

static void omap4_keypad_scan_keys(struct omap4_keypad *keypad_data, u64 keys)
{
	u64 changed;

	mutex_lock(&keypad_data->lock);

	changed = keys ^ keypad_data->keys;

	/*
	 * Report key up events separately and first. This matters in case we
	 * lost key-up interrupt and just now catching up.
	 */
	omap4_keypad_report_keys(keypad_data, changed & ~keys, false);

	/* Report key down events */
	omap4_keypad_report_keys(keypad_data, changed & keys, true);

	keypad_data->keys = keys;

	mutex_unlock(&keypad_data->lock);
}
>>>>>>> upstream/android-13

/* Interrupt handlers */
static irqreturn_t omap4_keypad_irq_handler(int irq, void *dev_id)
{
	struct omap4_keypad *keypad_data = dev_id;

	if (kbd_read_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS))
		return IRQ_WAKE_THREAD;

	return IRQ_NONE;
}

static irqreturn_t omap4_keypad_irq_thread_fn(int irq, void *dev_id)
{
	struct omap4_keypad *keypad_data = dev_id;
<<<<<<< HEAD
	struct input_dev *input_dev = keypad_data->input;
	unsigned char key_state[ARRAY_SIZE(keypad_data->key_state)];
	unsigned int col, row, code, changed;
	u32 *new_state = (u32 *) key_state;

	*new_state = kbd_readl(keypad_data, OMAP4_KBD_FULLCODE31_0);
	*(new_state + 1) = kbd_readl(keypad_data, OMAP4_KBD_FULLCODE63_32);

	for (row = 0; row < keypad_data->rows; row++) {
		changed = key_state[row] ^ keypad_data->key_state[row];
		if (!changed)
			continue;

		for (col = 0; col < keypad_data->cols; col++) {
			if (changed & (1 << col)) {
				code = MATRIX_SCAN_CODE(row, col,
						keypad_data->row_shift);
				input_event(input_dev, EV_MSC, MSC_SCAN, code);
				input_report_key(input_dev,
						 keypad_data->keymap[code],
						 key_state[row] & (1 << col));
			}
		}
	}

	input_sync(input_dev);

	memcpy(keypad_data->key_state, key_state,
		sizeof(keypad_data->key_state));
=======
	struct device *dev = keypad_data->input->dev.parent;
	u32 low, high;
	int error;
	u64 keys;

	error = pm_runtime_get_sync(dev);
	if (error < 0) {
		pm_runtime_put_noidle(dev);
		return IRQ_NONE;
	}

	low = kbd_readl(keypad_data, OMAP4_KBD_FULLCODE31_0);
	high = kbd_readl(keypad_data, OMAP4_KBD_FULLCODE63_32);
	keys = low | (u64)high << 32;

	omap4_keypad_scan_keys(keypad_data, keys);
>>>>>>> upstream/android-13

	/* clear pending interrupts */
	kbd_write_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS,
			 kbd_read_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS));

<<<<<<< HEAD
=======
	pm_runtime_mark_last_busy(dev);
	pm_runtime_put_autosuspend(dev);

>>>>>>> upstream/android-13
	return IRQ_HANDLED;
}

static int omap4_keypad_open(struct input_dev *input)
{
	struct omap4_keypad *keypad_data = input_get_drvdata(input);
<<<<<<< HEAD

	pm_runtime_get_sync(input->dev.parent);
=======
	struct device *dev = input->dev.parent;
	int error;

	error = pm_runtime_get_sync(dev);
	if (error < 0) {
		pm_runtime_put_noidle(dev);
		return error;
	}
>>>>>>> upstream/android-13

	disable_irq(keypad_data->irq);

	kbd_writel(keypad_data, OMAP4_KBD_CTRL,
			OMAP4_DEF_CTRL_NOSOFTMODE |
			(OMAP4_KEYPAD_PTV_DIV_128 << OMAP4_DEF_CTRL_PTV_SHIFT));
	kbd_writel(keypad_data, OMAP4_KBD_DEBOUNCINGTIME,
			OMAP4_VAL_DEBOUNCINGTIME_16MS);
	/* clear pending interrupts */
	kbd_write_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS,
			 kbd_read_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS));
	kbd_write_irqreg(keypad_data, OMAP4_KBD_IRQENABLE,
<<<<<<< HEAD
			OMAP4_DEF_IRQENABLE_EVENTEN |
				OMAP4_DEF_IRQENABLE_LONGKEY);
	kbd_writel(keypad_data, OMAP4_KBD_WAKEUPENABLE,
			OMAP4_DEF_WUP_EVENT_ENA | OMAP4_DEF_WUP_LONG_KEY_ENA);

	enable_irq(keypad_data->irq);

=======
			OMAP4_DEF_IRQENABLE_EVENTEN);
	kbd_writel(keypad_data, OMAP4_KBD_WAKEUPENABLE,
			OMAP4_DEF_WUP_EVENT_ENA);

	enable_irq(keypad_data->irq);

	pm_runtime_mark_last_busy(dev);
	pm_runtime_put_autosuspend(dev);

>>>>>>> upstream/android-13
	return 0;
}

static void omap4_keypad_stop(struct omap4_keypad *keypad_data)
{
	/* Disable interrupts and wake-up events */
	kbd_write_irqreg(keypad_data, OMAP4_KBD_IRQENABLE,
			 OMAP4_VAL_IRQDISABLE);
	kbd_writel(keypad_data, OMAP4_KBD_WAKEUPENABLE, 0);

	/* clear pending interrupts */
	kbd_write_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS,
			 kbd_read_irqreg(keypad_data, OMAP4_KBD_IRQSTATUS));
}

static void omap4_keypad_close(struct input_dev *input)
{
<<<<<<< HEAD
	struct omap4_keypad *keypad_data;

	keypad_data = input_get_drvdata(input);
=======
	struct omap4_keypad *keypad_data = input_get_drvdata(input);
	struct device *dev = input->dev.parent;
	int error;

	error = pm_runtime_get_sync(dev);
	if (error < 0)
		pm_runtime_put_noidle(dev);

>>>>>>> upstream/android-13
	disable_irq(keypad_data->irq);
	omap4_keypad_stop(keypad_data);
	enable_irq(keypad_data->irq);

<<<<<<< HEAD
	pm_runtime_put_sync(input->dev.parent);
=======
	pm_runtime_mark_last_busy(dev);
	pm_runtime_put_autosuspend(dev);
>>>>>>> upstream/android-13
}

static int omap4_keypad_parse_dt(struct device *dev,
				 struct omap4_keypad *keypad_data)
{
	struct device_node *np = dev->of_node;
	int err;

	err = matrix_keypad_parse_properties(dev, &keypad_data->rows,
					     &keypad_data->cols);
	if (err)
		return err;

	if (of_get_property(np, "linux,input-no-autorepeat", NULL))
		keypad_data->no_autorepeat = true;

	return 0;
}

static int omap4_keypad_check_revision(struct device *dev,
				       struct omap4_keypad *keypad_data)
{
	unsigned int rev;

	rev = __raw_readl(keypad_data->base + OMAP4_KBD_REVISION);
	rev &= 0x03 << 30;
	rev >>= 30;
	switch (rev) {
	case KBD_REVISION_OMAP4:
		keypad_data->reg_offset = 0x00;
		keypad_data->irqreg_offset = 0x00;
		break;
	case KBD_REVISION_OMAP5:
		keypad_data->reg_offset = 0x10;
		keypad_data->irqreg_offset = 0x0c;
		break;
	default:
		dev_err(dev, "Keypad reports unsupported revision %d", rev);
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD
static int omap4_keypad_probe(struct platform_device *pdev)
{
=======
/*
 * Errata ID i689 "1.32 Keyboard Key Up Event Can Be Missed".
 * Interrupt may not happen for key-up events. We must clear stuck
 * key-up events after the keyboard hardware has auto-idled.
 */
static int __maybe_unused omap4_keypad_runtime_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct omap4_keypad *keypad_data = platform_get_drvdata(pdev);
	u32 active;

	active = kbd_readl(keypad_data, OMAP4_KBD_STATEMACHINE);
	if (active) {
		pm_runtime_mark_last_busy(dev);
		return -EBUSY;
	}

	omap4_keypad_scan_keys(keypad_data, 0);

	return 0;
}

static const struct dev_pm_ops omap4_keypad_pm_ops = {
	SET_RUNTIME_PM_OPS(omap4_keypad_runtime_suspend, NULL, NULL)
};

static void omap4_disable_pm(void *d)
{
	pm_runtime_dont_use_autosuspend(d);
	pm_runtime_disable(d);
}

static int omap4_keypad_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
>>>>>>> upstream/android-13
	struct omap4_keypad *keypad_data;
	struct input_dev *input_dev;
	struct resource *res;
	unsigned int max_keys;
	int irq;
	int error;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "no base address specified\n");
		return -EINVAL;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

<<<<<<< HEAD
	keypad_data = kzalloc(sizeof(struct omap4_keypad), GFP_KERNEL);
	if (!keypad_data) {
		dev_err(&pdev->dev, "keypad_data memory allocation failed\n");
=======
	keypad_data = devm_kzalloc(dev, sizeof(*keypad_data), GFP_KERNEL);
	if (!keypad_data) {
		dev_err(dev, "keypad_data memory allocation failed\n");
>>>>>>> upstream/android-13
		return -ENOMEM;
	}

	keypad_data->irq = irq;
<<<<<<< HEAD

	error = omap4_keypad_parse_dt(&pdev->dev, keypad_data);
	if (error)
		goto err_free_keypad;

	res = request_mem_region(res->start, resource_size(res), pdev->name);
	if (!res) {
		dev_err(&pdev->dev, "can't request mem region\n");
		error = -EBUSY;
		goto err_free_keypad;
	}

	keypad_data->base = ioremap(res->start, resource_size(res));
	if (!keypad_data->base) {
		dev_err(&pdev->dev, "can't ioremap mem resource\n");
		error = -ENOMEM;
		goto err_release_mem;
	}

	pm_runtime_enable(&pdev->dev);

=======
	mutex_init(&keypad_data->lock);
	platform_set_drvdata(pdev, keypad_data);

	error = omap4_keypad_parse_dt(dev, keypad_data);
	if (error)
		return error;

	keypad_data->base = devm_ioremap_resource(dev, res);
	if (IS_ERR(keypad_data->base))
		return PTR_ERR(keypad_data->base);

	pm_runtime_use_autosuspend(dev);
	pm_runtime_set_autosuspend_delay(dev, OMAP4_KEYPAD_IDLE_CHECK_MS);
	pm_runtime_enable(dev);

	error = devm_add_action_or_reset(dev, omap4_disable_pm, dev);
	if (error) {
		dev_err(dev, "unable to register cleanup action\n");
		return error;
	}

>>>>>>> upstream/android-13
	/*
	 * Enable clocks for the keypad module so that we can read
	 * revision register.
	 */
<<<<<<< HEAD
	error = pm_runtime_get_sync(&pdev->dev);
	if (error) {
		dev_err(&pdev->dev, "pm_runtime_get_sync() failed\n");
		pm_runtime_put_noidle(&pdev->dev);
	} else {
		error = omap4_keypad_check_revision(&pdev->dev,
						    keypad_data);
		if (!error) {
			/* Ensure device does not raise interrupts */
			omap4_keypad_stop(keypad_data);
		}
		pm_runtime_put_sync(&pdev->dev);
	}
	if (error)
		goto err_pm_disable;

	/* input device allocation */
	keypad_data->input = input_dev = input_allocate_device();
	if (!input_dev) {
		error = -ENOMEM;
		goto err_pm_disable;
	}

	input_dev->name = pdev->name;
	input_dev->dev.parent = &pdev->dev;
=======
	error = pm_runtime_get_sync(dev);
	if (error < 0) {
		dev_err(dev, "pm_runtime_get_sync() failed\n");
		pm_runtime_put_noidle(dev);
		return error;
	}

	error = omap4_keypad_check_revision(dev, keypad_data);
	if (!error) {
		/* Ensure device does not raise interrupts */
		omap4_keypad_stop(keypad_data);
	}

	pm_runtime_mark_last_busy(dev);
	pm_runtime_put_autosuspend(dev);
	if (error)
		return error;

	/* input device allocation */
	keypad_data->input = input_dev = devm_input_allocate_device(dev);
	if (!input_dev)
		return -ENOMEM;

	input_dev->name = pdev->name;
>>>>>>> upstream/android-13
	input_dev->id.bustype = BUS_HOST;
	input_dev->id.vendor = 0x0001;
	input_dev->id.product = 0x0001;
	input_dev->id.version = 0x0001;

	input_dev->open = omap4_keypad_open;
	input_dev->close = omap4_keypad_close;

	input_set_capability(input_dev, EV_MSC, MSC_SCAN);
	if (!keypad_data->no_autorepeat)
		__set_bit(EV_REP, input_dev->evbit);

	input_set_drvdata(input_dev, keypad_data);

	keypad_data->row_shift = get_count_order(keypad_data->cols);
	max_keys = keypad_data->rows << keypad_data->row_shift;
<<<<<<< HEAD
	keypad_data->keymap = kcalloc(max_keys,
				      sizeof(keypad_data->keymap[0]),
				      GFP_KERNEL);
	if (!keypad_data->keymap) {
		dev_err(&pdev->dev, "Not enough memory for keymap\n");
		error = -ENOMEM;
		goto err_free_input;
=======
	keypad_data->keymap = devm_kcalloc(dev,
					   max_keys,
					   sizeof(keypad_data->keymap[0]),
					   GFP_KERNEL);
	if (!keypad_data->keymap) {
		dev_err(dev, "Not enough memory for keymap\n");
		return -ENOMEM;
>>>>>>> upstream/android-13
	}

	error = matrix_keypad_build_keymap(NULL, NULL,
					   keypad_data->rows, keypad_data->cols,
					   keypad_data->keymap, input_dev);
	if (error) {
<<<<<<< HEAD
		dev_err(&pdev->dev, "failed to build keymap\n");
		goto err_free_keymap;
	}

	error = request_threaded_irq(keypad_data->irq, omap4_keypad_irq_handler,
				     omap4_keypad_irq_thread_fn, IRQF_ONESHOT,
				     "omap4-keypad", keypad_data);
	if (error) {
		dev_err(&pdev->dev, "failed to register interrupt\n");
		goto err_free_keymap;
	}

	error = input_register_device(keypad_data->input);
	if (error < 0) {
		dev_err(&pdev->dev, "failed to register input device\n");
		goto err_free_irq;
	}

	device_init_wakeup(&pdev->dev, true);
	platform_set_drvdata(pdev, keypad_data);

	return 0;

err_free_irq:
	free_irq(keypad_data->irq, keypad_data);
err_free_keymap:
	kfree(keypad_data->keymap);
err_free_input:
	input_free_device(input_dev);
err_pm_disable:
	pm_runtime_disable(&pdev->dev);
	iounmap(keypad_data->base);
err_release_mem:
	release_mem_region(res->start, resource_size(res));
err_free_keypad:
	kfree(keypad_data);
	return error;
=======
		dev_err(dev, "failed to build keymap\n");
		return error;
	}

	error = devm_request_threaded_irq(dev, keypad_data->irq,
					  omap4_keypad_irq_handler,
					  omap4_keypad_irq_thread_fn,
					  IRQF_ONESHOT,
					  "omap4-keypad", keypad_data);
	if (error) {
		dev_err(dev, "failed to register interrupt\n");
		return error;
	}

	error = input_register_device(keypad_data->input);
	if (error) {
		dev_err(dev, "failed to register input device\n");
		return error;
	}

	device_init_wakeup(dev, true);
	error = dev_pm_set_wake_irq(dev, keypad_data->irq);
	if (error)
		dev_warn(dev, "failed to set up wakeup irq: %d\n", error);

	return 0;
>>>>>>> upstream/android-13
}

static int omap4_keypad_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct omap4_keypad *keypad_data = platform_get_drvdata(pdev);
	struct resource *res;

	free_irq(keypad_data->irq, keypad_data);

	pm_runtime_disable(&pdev->dev);

	input_unregister_device(keypad_data->input);

	iounmap(keypad_data->base);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	release_mem_region(res->start, resource_size(res));

	kfree(keypad_data->keymap);
	kfree(keypad_data);
=======
	dev_pm_clear_wake_irq(&pdev->dev);
>>>>>>> upstream/android-13

	return 0;
}

static const struct of_device_id omap_keypad_dt_match[] = {
	{ .compatible = "ti,omap4-keypad" },
	{},
};
MODULE_DEVICE_TABLE(of, omap_keypad_dt_match);

<<<<<<< HEAD
#ifdef CONFIG_PM_SLEEP
static int omap4_keypad_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct omap4_keypad *keypad_data = platform_get_drvdata(pdev);
	int error;

	if (device_may_wakeup(&pdev->dev)) {
		error = enable_irq_wake(keypad_data->irq);
		if (!error)
			keypad_data->irq_wake_enabled = true;
	}

	return 0;
}

static int omap4_keypad_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct omap4_keypad *keypad_data = platform_get_drvdata(pdev);

	if (device_may_wakeup(&pdev->dev) && keypad_data->irq_wake_enabled) {
		disable_irq_wake(keypad_data->irq);
		keypad_data->irq_wake_enabled = false;
	}

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(omap4_keypad_pm_ops,
			 omap4_keypad_suspend, omap4_keypad_resume);

=======
>>>>>>> upstream/android-13
static struct platform_driver omap4_keypad_driver = {
	.probe		= omap4_keypad_probe,
	.remove		= omap4_keypad_remove,
	.driver		= {
		.name	= "omap4-keypad",
<<<<<<< HEAD
		.pm	= &omap4_keypad_pm_ops,
		.of_match_table = omap_keypad_dt_match,
=======
		.of_match_table = omap_keypad_dt_match,
		.pm = &omap4_keypad_pm_ops,
>>>>>>> upstream/android-13
	},
};
module_platform_driver(omap4_keypad_driver);

MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("OMAP4 Keypad Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:omap4-keypad");
