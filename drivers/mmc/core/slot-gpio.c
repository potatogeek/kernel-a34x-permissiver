<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Generic GPIO card-detect helper
 *
 * Copyright (C) 2011, Guennadi Liakhovetski <g.liakhovetski@gmx.de>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/err.h>
#include <linux/gpio.h>
=======
 */

#include <linux/err.h>
>>>>>>> upstream/android-13
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/mmc/host.h>
#include <linux/mmc/slot-gpio.h>
#include <linux/module.h>
#include <linux/slab.h>

<<<<<<< HEAD
#include "slot-gpio.h"
#include "core.h"

extern int mmc_get_cd_level(struct mmc_host *mmc);
=======
#include <trace/hooks/mmc.h>

#include "slot-gpio.h"
>>>>>>> upstream/android-13

struct mmc_gpio {
	struct gpio_desc *ro_gpio;
	struct gpio_desc *cd_gpio;
<<<<<<< HEAD
	bool override_ro_active_level;
	bool override_cd_active_level;
	bool status;
	irqreturn_t (*cd_gpio_isr)(int irq, void *dev_id);
	char *ro_label;
	u32 cd_debounce_delay_ms;
	char cd_label[];
=======
	irqreturn_t (*cd_gpio_isr)(int irq, void *dev_id);
	char *ro_label;
	char *cd_label;
	u32 cd_debounce_delay_ms;
>>>>>>> upstream/android-13
};

static irqreturn_t mmc_gpio_cd_irqt(int irq, void *dev_id)
{
	/* Schedule a card detection after a debounce timeout */
	struct mmc_host *host = dev_id;
	struct mmc_gpio *ctx = host->slot.handler_priv;
<<<<<<< HEAD
	bool status;

	if (host->card_detect_cnt < UINT_MAX)
		host->card_detect_cnt++;

	status = mmc_gpio_get_cd(host) ? true : false;

	if (status ^ ctx->status) {
		pr_err("%s: slot status change detected (%d -> %d), GPIO_ACTIVE_%s\n",
				mmc_hostname(host), ctx->status, status,
				(host->caps2 & MMC_CAP2_CD_ACTIVE_HIGH) ? "HIGH" : "LOW");
		ST_LOG("%s: slot status change detected (%d -> %d), GPIO_ACTIVE_%s\n",
				mmc_hostname(host), ctx->status, status,
				(host->caps2 & MMC_CAP2_CD_ACTIVE_HIGH) ? "HIGH" : "LOW");
		ctx->status = status;
		host->trigger_card_event = true;

#ifdef CONFIG_SEC_FACTORY
		if (status)
			mmc_detect_change(host, msecs_to_jiffies(50));
		else
			mmc_detect_change(host, 0);
#else
		mmc_detect_change(host, msecs_to_jiffies(200));
#endif
	}
=======
	bool allow = true;

	trace_android_vh_mmc_gpio_cd_irqt(host, &allow);
	if (!allow)
		return IRQ_HANDLED;

	host->trigger_card_event = true;
	mmc_detect_change(host, msecs_to_jiffies(ctx->cd_debounce_delay_ms));
>>>>>>> upstream/android-13

	return IRQ_HANDLED;
}

int mmc_gpio_alloc(struct mmc_host *host)
{
<<<<<<< HEAD
	size_t len = strlen(dev_name(host->parent)) + 4;
	struct mmc_gpio *ctx = devm_kzalloc(host->parent,
				sizeof(*ctx) + 2 * len,	GFP_KERNEL);

	if (ctx) {
		ctx->ro_label = ctx->cd_label + len;
		ctx->cd_debounce_delay_ms = 200;
		snprintf(ctx->cd_label, len, "%s cd", dev_name(host->parent));
		snprintf(ctx->ro_label, len, "%s ro", dev_name(host->parent));
=======
	struct mmc_gpio *ctx = devm_kzalloc(host->parent,
					    sizeof(*ctx), GFP_KERNEL);

	if (ctx) {
		ctx->cd_debounce_delay_ms = 200;
		ctx->cd_label = devm_kasprintf(host->parent, GFP_KERNEL,
				"%s cd", dev_name(host->parent));
		if (!ctx->cd_label)
			return -ENOMEM;
		ctx->ro_label = devm_kasprintf(host->parent, GFP_KERNEL,
				"%s ro", dev_name(host->parent));
		if (!ctx->ro_label)
			return -ENOMEM;
>>>>>>> upstream/android-13
		host->slot.handler_priv = ctx;
		host->slot.cd_irq = -EINVAL;
	}

	return ctx ? 0 : -ENOMEM;
}

int mmc_gpio_get_ro(struct mmc_host *host)
{
	struct mmc_gpio *ctx = host->slot.handler_priv;

	if (!ctx || !ctx->ro_gpio)
		return -ENOSYS;

<<<<<<< HEAD
	if (ctx->override_ro_active_level)
		return !gpiod_get_raw_value_cansleep(ctx->ro_gpio) ^
			!!(host->caps2 & MMC_CAP2_RO_ACTIVE_HIGH);

=======
>>>>>>> upstream/android-13
	return gpiod_get_value_cansleep(ctx->ro_gpio);
}
EXPORT_SYMBOL(mmc_gpio_get_ro);

int mmc_gpio_get_cd(struct mmc_host *host)
{
	struct mmc_gpio *ctx = host->slot.handler_priv;
	int cansleep;

	if (!ctx || !ctx->cd_gpio)
		return -ENOSYS;

	cansleep = gpiod_cansleep(ctx->cd_gpio);
<<<<<<< HEAD
	if (ctx->override_cd_active_level) {
		int value = cansleep ?
				gpiod_get_raw_value_cansleep(ctx->cd_gpio) :
				gpiod_get_raw_value(ctx->cd_gpio);
		return !value ^ !!(host->caps2 & MMC_CAP2_CD_ACTIVE_HIGH);
	}

=======
>>>>>>> upstream/android-13
	return cansleep ?
		gpiod_get_value_cansleep(ctx->cd_gpio) :
		gpiod_get_value(ctx->cd_gpio);
}
EXPORT_SYMBOL(mmc_gpio_get_cd);

<<<<<<< HEAD
/**
 * mmc_gpio_request_ro - request a gpio for write-protection
 * @host: mmc host
 * @gpio: gpio number requested
 *
 * As devm_* managed functions are used in mmc_gpio_request_ro(), client
 * drivers do not need to worry about freeing up memory.
 *
 * Returns zero on success, else an error.
 */
int mmc_gpio_request_ro(struct mmc_host *host, unsigned int gpio)
{
	struct mmc_gpio *ctx = host->slot.handler_priv;
	int ret;

	if (!gpio_is_valid(gpio))
		return -EINVAL;

	ret = devm_gpio_request_one(host->parent, gpio, GPIOF_DIR_IN,
				    ctx->ro_label);
	if (ret < 0)
		return ret;

	ctx->override_ro_active_level = true;
	ctx->ro_gpio = gpio_to_desc(gpio);

	return 0;
}
EXPORT_SYMBOL(mmc_gpio_request_ro);

=======
>>>>>>> upstream/android-13
void mmc_gpiod_request_cd_irq(struct mmc_host *host)
{
	struct mmc_gpio *ctx = host->slot.handler_priv;
	int irq = -EINVAL;
	int ret;

	if (host->slot.cd_irq >= 0 || !ctx || !ctx->cd_gpio)
		return;

	/*
	 * Do not use IRQ if the platform prefers to poll, e.g., because that
	 * IRQ number is already used by another unit and cannot be shared.
	 */
	if (!(host->caps & MMC_CAP_NEEDS_POLL))
		irq = gpiod_to_irq(ctx->cd_gpio);

<<<<<<< HEAD
	ctx->status = mmc_gpio_get_cd(host) ? true : false;

=======
>>>>>>> upstream/android-13
	if (irq >= 0) {
		if (!ctx->cd_gpio_isr)
			ctx->cd_gpio_isr = mmc_gpio_cd_irqt;
		ret = devm_request_threaded_irq(host->parent, irq,
			NULL, ctx->cd_gpio_isr,
			IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
			ctx->cd_label, host);
		if (ret < 0)
			irq = ret;
	}

	host->slot.cd_irq = irq;

	if (irq < 0)
		host->caps |= MMC_CAP_NEEDS_POLL;
}
EXPORT_SYMBOL(mmc_gpiod_request_cd_irq);

int mmc_gpio_set_cd_wake(struct mmc_host *host, bool on)
{
	int ret = 0;

	if (!(host->caps & MMC_CAP_CD_WAKE) ||
	    host->slot.cd_irq < 0 ||
	    on == host->slot.cd_wake_enabled)
		return 0;

	if (on) {
		ret = enable_irq_wake(host->slot.cd_irq);
		host->slot.cd_wake_enabled = !ret;
	} else {
		disable_irq_wake(host->slot.cd_irq);
		host->slot.cd_wake_enabled = false;
	}

	return ret;
}
EXPORT_SYMBOL(mmc_gpio_set_cd_wake);

/* Register an alternate interrupt service routine for
 * the card-detect GPIO.
 */
void mmc_gpio_set_cd_isr(struct mmc_host *host,
			 irqreturn_t (*isr)(int irq, void *dev_id))
{
	struct mmc_gpio *ctx = host->slot.handler_priv;

	WARN_ON(ctx->cd_gpio_isr);
	ctx->cd_gpio_isr = isr;
}
EXPORT_SYMBOL(mmc_gpio_set_cd_isr);

/**
<<<<<<< HEAD
 * mmc_gpio_request_cd - request a gpio for card-detection
 * @host: mmc host
 * @gpio: gpio number requested
 * @debounce: debounce time in microseconds
 *
 * As devm_* managed functions are used in mmc_gpio_request_cd(), client
 * drivers do not need to worry about freeing up memory.
 *
 * If GPIO debouncing is desired, set the debounce parameter to a non-zero
 * value. The caller is responsible for ensuring that the GPIO driver associated
 * with the GPIO supports debouncing, otherwise an error will be returned.
 *
 * Returns zero on success, else an error.
 */
int mmc_gpio_request_cd(struct mmc_host *host, unsigned int gpio,
			unsigned int debounce)
{
	struct mmc_gpio *ctx = host->slot.handler_priv;
	int ret;

	ret = devm_gpio_request_one(host->parent, gpio, GPIOF_DIR_IN,
				    ctx->cd_label);
	if (ret < 0)
		/*
		 * don't bother freeing memory. It might still get used by other
		 * slot functions, in any case it will be freed, when the device
		 * is destroyed.
		 */
		return ret;

	if (debounce) {
		ret = gpio_set_debounce(gpio, debounce);
		if (ret < 0)
			return ret;
	}

	ctx->override_cd_active_level = true;
	ctx->cd_gpio = gpio_to_desc(gpio);

	return 0;
}
EXPORT_SYMBOL(mmc_gpio_request_cd);

/**
=======
>>>>>>> upstream/android-13
 * mmc_gpiod_request_cd - request a gpio descriptor for card-detection
 * @host: mmc host
 * @con_id: function within the GPIO consumer
 * @idx: index of the GPIO to obtain in the consumer
 * @override_active_level: ignore %GPIO_ACTIVE_LOW flag
 * @debounce: debounce time in microseconds
<<<<<<< HEAD
 * @gpio_invert: will return whether the GPIO line is inverted or not, set
 * to NULL to ignore
 *
 * Use this function in place of mmc_gpio_request_cd() to use the GPIO
 * descriptor API.  Note that it must be called prior to mmc_add_host()
=======
 *
 * Note that this must be called prior to mmc_add_host()
>>>>>>> upstream/android-13
 * otherwise the caller must also call mmc_gpiod_request_cd_irq().
 *
 * Returns zero on success, else an error.
 */
int mmc_gpiod_request_cd(struct mmc_host *host, const char *con_id,
			 unsigned int idx, bool override_active_level,
<<<<<<< HEAD
			 unsigned int debounce, bool *gpio_invert)
=======
			 unsigned int debounce)
>>>>>>> upstream/android-13
{
	struct mmc_gpio *ctx = host->slot.handler_priv;
	struct gpio_desc *desc;
	int ret;

	desc = devm_gpiod_get_index(host->parent, con_id, idx, GPIOD_IN);
	if (IS_ERR(desc))
		return PTR_ERR(desc);

	if (debounce) {
		ret = gpiod_set_debounce(desc, debounce);
		if (ret < 0)
			ctx->cd_debounce_delay_ms = debounce / 1000;
	}

<<<<<<< HEAD
	if (gpio_invert)
		*gpio_invert = !gpiod_is_active_low(desc);

	ctx->override_cd_active_level = override_active_level;
=======
	/* override forces default (active-low) polarity ... */
	if (override_active_level && !gpiod_is_active_low(desc))
		gpiod_toggle_active_low(desc);

	/* ... or active-high */
	if (host->caps2 & MMC_CAP2_CD_ACTIVE_HIGH)
		gpiod_toggle_active_low(desc);

>>>>>>> upstream/android-13
	ctx->cd_gpio = desc;

	return 0;
}
EXPORT_SYMBOL(mmc_gpiod_request_cd);

bool mmc_can_gpio_cd(struct mmc_host *host)
{
	struct mmc_gpio *ctx = host->slot.handler_priv;

	return ctx->cd_gpio ? true : false;
}
EXPORT_SYMBOL(mmc_can_gpio_cd);

/**
 * mmc_gpiod_request_ro - request a gpio descriptor for write protection
 * @host: mmc host
 * @con_id: function within the GPIO consumer
 * @idx: index of the GPIO to obtain in the consumer
<<<<<<< HEAD
 * @override_active_level: ignore %GPIO_ACTIVE_LOW flag
 * @debounce: debounce time in microseconds
 * @gpio_invert: will return whether the GPIO line is inverted or not,
 * set to NULL to ignore
 *
 * Use this function in place of mmc_gpio_request_ro() to use the GPIO
 * descriptor API.
=======
 * @debounce: debounce time in microseconds
>>>>>>> upstream/android-13
 *
 * Returns zero on success, else an error.
 */
int mmc_gpiod_request_ro(struct mmc_host *host, const char *con_id,
<<<<<<< HEAD
			 unsigned int idx, bool override_active_level,
			 unsigned int debounce, bool *gpio_invert)
=======
			 unsigned int idx, unsigned int debounce)
>>>>>>> upstream/android-13
{
	struct mmc_gpio *ctx = host->slot.handler_priv;
	struct gpio_desc *desc;
	int ret;

	desc = devm_gpiod_get_index(host->parent, con_id, idx, GPIOD_IN);
	if (IS_ERR(desc))
		return PTR_ERR(desc);

	if (debounce) {
		ret = gpiod_set_debounce(desc, debounce);
		if (ret < 0)
			return ret;
	}

<<<<<<< HEAD
	if (gpio_invert)
		*gpio_invert = !gpiod_is_active_low(desc);

	ctx->override_ro_active_level = override_active_level;
=======
	if (host->caps2 & MMC_CAP2_RO_ACTIVE_HIGH)
		gpiod_toggle_active_low(desc);

>>>>>>> upstream/android-13
	ctx->ro_gpio = desc;

	return 0;
}
EXPORT_SYMBOL(mmc_gpiod_request_ro);

bool mmc_can_gpio_ro(struct mmc_host *host)
{
	struct mmc_gpio *ctx = host->slot.handler_priv;

	return ctx->ro_gpio ? true : false;
}
EXPORT_SYMBOL(mmc_can_gpio_ro);
