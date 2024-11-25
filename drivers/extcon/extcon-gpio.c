<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * extcon_gpio.c - Single-state GPIO extcon driver based on extcon class
 *
 * Copyright (C) 2008 Google, Inc.
 * Author: Mike Lockwood <lockwood@android.com>
 *
 * Modified by MyungJoo Ham <myungjoo.ham@samsung.com> to support extcon
 * (originally switch class is supported)
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
 */

=======
 */

#include <linux/devm-helpers.h>
>>>>>>> upstream/android-13
#include <linux/extcon-provider.h>
#include <linux/gpio/consumer.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/workqueue.h>

/**
 * struct gpio_extcon_data - A simple GPIO-controlled extcon device state container.
 * @edev:		Extcon device.
<<<<<<< HEAD
 * @irq:		Interrupt line for the external connector.
=======
>>>>>>> upstream/android-13
 * @work:		Work fired by the interrupt.
 * @debounce_jiffies:	Number of jiffies to wait for the GPIO to stabilize, from the debounce
 *			value.
 * @gpiod:		GPIO descriptor for this external connector.
 * @extcon_id:		The unique id of specific external connector.
 * @debounce:		Debounce time for GPIO IRQ in ms.
<<<<<<< HEAD
 * @irq_flags:		IRQ Flags (e.g., IRQF_TRIGGER_LOW).
=======
>>>>>>> upstream/android-13
 * @check_on_resume:	Boolean describing whether to check the state of gpio
 *			while resuming from sleep.
 */
struct gpio_extcon_data {
	struct extcon_dev *edev;
<<<<<<< HEAD
	int irq;
=======
>>>>>>> upstream/android-13
	struct delayed_work work;
	unsigned long debounce_jiffies;
	struct gpio_desc *gpiod;
	unsigned int extcon_id;
	unsigned long debounce;
<<<<<<< HEAD
	unsigned long irq_flags;
=======
>>>>>>> upstream/android-13
	bool check_on_resume;
};

static void gpio_extcon_work(struct work_struct *work)
{
	int state;
	struct gpio_extcon_data	*data =
		container_of(to_delayed_work(work), struct gpio_extcon_data,
			     work);

	state = gpiod_get_value_cansleep(data->gpiod);
	extcon_set_state_sync(data->edev, data->extcon_id, state);
}

static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
	struct gpio_extcon_data *data = dev_id;

	queue_delayed_work(system_power_efficient_wq, &data->work,
			      data->debounce_jiffies);
	return IRQ_HANDLED;
}

static int gpio_extcon_probe(struct platform_device *pdev)
{
	struct gpio_extcon_data *data;
	struct device *dev = &pdev->dev;
<<<<<<< HEAD
=======
	unsigned long irq_flags;
	int irq;
>>>>>>> upstream/android-13
	int ret;

	data = devm_kzalloc(dev, sizeof(struct gpio_extcon_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	/*
	 * FIXME: extcon_id represents the unique identifier of external
	 * connectors such as EXTCON_USB, EXTCON_DISP_HDMI and so on. extcon_id
	 * is necessary to register the extcon device. But, it's not yet
	 * developed to get the extcon id from device-tree or others.
	 * On later, it have to be solved.
	 */
<<<<<<< HEAD
	if (!data->irq_flags || data->extcon_id > EXTCON_NONE)
=======
	if (data->extcon_id > EXTCON_NONE)
>>>>>>> upstream/android-13
		return -EINVAL;

	data->gpiod = devm_gpiod_get(dev, "extcon", GPIOD_IN);
	if (IS_ERR(data->gpiod))
		return PTR_ERR(data->gpiod);
<<<<<<< HEAD
	data->irq = gpiod_to_irq(data->gpiod);
	if (data->irq <= 0)
		return data->irq;
=======
	irq = gpiod_to_irq(data->gpiod);
	if (irq <= 0)
		return irq;

	/*
	 * It is unlikely that this is an acknowledged interrupt that goes
	 * away after handling, what we are looking for are falling edges
	 * if the signal is active low, and rising edges if the signal is
	 * active high.
	 */
	if (gpiod_is_active_low(data->gpiod))
		irq_flags = IRQF_TRIGGER_FALLING;
	else
		irq_flags = IRQF_TRIGGER_RISING;
>>>>>>> upstream/android-13

	/* Allocate the memory of extcon devie and register extcon device */
	data->edev = devm_extcon_dev_allocate(dev, &data->extcon_id);
	if (IS_ERR(data->edev)) {
		dev_err(dev, "failed to allocate extcon device\n");
		return -ENOMEM;
	}

	ret = devm_extcon_dev_register(dev, data->edev);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	INIT_DELAYED_WORK(&data->work, gpio_extcon_work);
=======
	ret = devm_delayed_work_autocancel(dev, &data->work, gpio_extcon_work);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	/*
	 * Request the interrupt of gpio to detect whether external connector
	 * is attached or detached.
	 */
<<<<<<< HEAD
	ret = devm_request_any_context_irq(dev, data->irq,
					gpio_irq_handler, data->irq_flags,
=======
	ret = devm_request_any_context_irq(dev, irq,
					gpio_irq_handler, irq_flags,
>>>>>>> upstream/android-13
					pdev->name, data);
	if (ret < 0)
		return ret;

	platform_set_drvdata(pdev, data);
	/* Perform initial detection */
	gpio_extcon_work(&data->work.work);

	return 0;
}

<<<<<<< HEAD
static int gpio_extcon_remove(struct platform_device *pdev)
{
	struct gpio_extcon_data *data = platform_get_drvdata(pdev);

	cancel_delayed_work_sync(&data->work);

	return 0;
}

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_PM_SLEEP
static int gpio_extcon_resume(struct device *dev)
{
	struct gpio_extcon_data *data;

	data = dev_get_drvdata(dev);
	if (data->check_on_resume)
		queue_delayed_work(system_power_efficient_wq,
			&data->work, data->debounce_jiffies);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(gpio_extcon_pm_ops, NULL, gpio_extcon_resume);

static struct platform_driver gpio_extcon_driver = {
	.probe		= gpio_extcon_probe,
<<<<<<< HEAD
	.remove		= gpio_extcon_remove,
=======
>>>>>>> upstream/android-13
	.driver		= {
		.name	= "extcon-gpio",
		.pm	= &gpio_extcon_pm_ops,
	},
};

module_platform_driver(gpio_extcon_driver);

MODULE_AUTHOR("Mike Lockwood <lockwood@android.com>");
MODULE_DESCRIPTION("GPIO extcon driver");
MODULE_LICENSE("GPL");
