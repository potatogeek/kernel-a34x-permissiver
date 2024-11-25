<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * Mainly by David Woodhouse, somewhat modified by Jordan Crouse
 *
 * Copyright © 2006-2007  Red Hat, Inc.
 * Copyright © 2006-2007  Advanced Micro Devices, Inc.
 * Copyright © 2009       VIA Technology, Inc.
 * Copyright (c) 2010  Andres Salomon <dilinger@queued.net>
<<<<<<< HEAD
 *
 * This program is free software.  You can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/cs5535.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/delay.h>
=======
#include <linux/gpio/consumer.h>
#include <linux/delay.h>
#include <linux/i2c.h>
>>>>>>> upstream/android-13
#include <asm/olpc.h>

#include "olpc_dcon.h"

<<<<<<< HEAD
static int dcon_init_xo_1(struct dcon_priv *dcon)
{
	unsigned char lob;

	if (gpio_request(OLPC_GPIO_DCON_STAT0, "OLPC-DCON")) {
		pr_err("failed to request STAT0 GPIO\n");
		return -EIO;
	}
	if (gpio_request(OLPC_GPIO_DCON_STAT1, "OLPC-DCON")) {
		pr_err("failed to request STAT1 GPIO\n");
		goto err_gp_stat1;
	}
	if (gpio_request(OLPC_GPIO_DCON_IRQ, "OLPC-DCON")) {
		pr_err("failed to request IRQ GPIO\n");
		goto err_gp_irq;
	}
	if (gpio_request(OLPC_GPIO_DCON_LOAD, "OLPC-DCON")) {
		pr_err("failed to request LOAD GPIO\n");
		goto err_gp_load;
	}
	if (gpio_request(OLPC_GPIO_DCON_BLANK, "OLPC-DCON")) {
		pr_err("failed to request BLANK GPIO\n");
		goto err_gp_blank;
=======
enum dcon_gpios {
	OLPC_DCON_STAT0,
	OLPC_DCON_STAT1,
	OLPC_DCON_IRQ,
	OLPC_DCON_LOAD,
	OLPC_DCON_BLANK,
};

static const struct dcon_gpio gpios_asis[] = {
	[OLPC_DCON_STAT0] = { .name = "dcon_stat0", .flags = GPIOD_ASIS },
	[OLPC_DCON_STAT1] = { .name = "dcon_stat1", .flags = GPIOD_ASIS },
	[OLPC_DCON_IRQ] = { .name = "dcon_irq", .flags = GPIOD_ASIS },
	[OLPC_DCON_LOAD] = { .name = "dcon_load", .flags = GPIOD_ASIS },
	[OLPC_DCON_BLANK] = { .name = "dcon_blank", .flags = GPIOD_ASIS },
};

static struct gpio_desc *gpios[5];

static int dcon_init_xo_1(struct dcon_priv *dcon)
{
	unsigned char lob;
	int ret, i;
	const struct dcon_gpio *pin = &gpios_asis[0];

	for (i = 0; i < ARRAY_SIZE(gpios_asis); i++) {
		gpios[i] = devm_gpiod_get(&dcon->client->dev, pin[i].name,
					  pin[i].flags);
		if (IS_ERR(gpios[i])) {
			ret = PTR_ERR(gpios[i]);
			pr_err("failed to request %s GPIO: %d\n", pin[i].name,
			       ret);
			return ret;
		}
>>>>>>> upstream/android-13
	}

	/* Turn off the event enable for GPIO7 just to be safe */
	cs5535_gpio_clear(OLPC_GPIO_DCON_IRQ, GPIO_EVENTS_ENABLE);

	/*
	 * Determine the current state by reading the GPIO bit; earlier
	 * stages of the boot process have established the state.
	 *
	 * Note that we read GPIO_OUTPUT_VAL rather than GPIO_READ_BACK here;
	 * this is because OFW will disable input for the pin and set a value..
	 * READ_BACK will only contain a valid value if input is enabled and
	 * then a value is set.  So, future readings of the pin can use
	 * READ_BACK, but the first one cannot.  Awesome, huh?
	 */
	dcon->curr_src = cs5535_gpio_isset(OLPC_GPIO_DCON_LOAD, GPIO_OUTPUT_VAL)
		? DCON_SOURCE_CPU
		: DCON_SOURCE_DCON;
	dcon->pending_src = dcon->curr_src;

	/* Set the directions for the GPIO pins */
<<<<<<< HEAD
	gpio_direction_input(OLPC_GPIO_DCON_STAT0);
	gpio_direction_input(OLPC_GPIO_DCON_STAT1);
	gpio_direction_input(OLPC_GPIO_DCON_IRQ);
	gpio_direction_input(OLPC_GPIO_DCON_BLANK);
	gpio_direction_output(OLPC_GPIO_DCON_LOAD,
			      dcon->curr_src == DCON_SOURCE_CPU);
=======
	gpiod_direction_input(gpios[OLPC_DCON_STAT0]);
	gpiod_direction_input(gpios[OLPC_DCON_STAT1]);
	gpiod_direction_input(gpios[OLPC_DCON_IRQ]);
	gpiod_direction_input(gpios[OLPC_DCON_BLANK]);
	gpiod_direction_output(gpios[OLPC_DCON_LOAD],
			       dcon->curr_src == DCON_SOURCE_CPU);
>>>>>>> upstream/android-13

	/* Set up the interrupt mappings */

	/* Set the IRQ to pair 2 */
	cs5535_gpio_setup_event(OLPC_GPIO_DCON_IRQ, 2, 0);

	/* Enable group 2 to trigger the DCON interrupt */
	cs5535_gpio_set_irq(2, DCON_IRQ);

	/* Select edge level for interrupt (in PIC) */
	lob = inb(0x4d0);
	lob &= ~(1 << DCON_IRQ);
	outb(lob, 0x4d0);

	/* Register the interrupt handler */
	if (request_irq(DCON_IRQ, &dcon_interrupt, 0, "DCON", dcon)) {
		pr_err("failed to request DCON's irq\n");
<<<<<<< HEAD
		goto err_req_irq;
=======
		return -EIO;
>>>>>>> upstream/android-13
	}

	/* Clear INV_EN for GPIO7 (DCONIRQ) */
	cs5535_gpio_clear(OLPC_GPIO_DCON_IRQ, GPIO_INPUT_INVERT);

	/* Enable filter for GPIO12 (DCONBLANK) */
	cs5535_gpio_set(OLPC_GPIO_DCON_BLANK, GPIO_INPUT_FILTER);

	/* Disable filter for GPIO7 */
	cs5535_gpio_clear(OLPC_GPIO_DCON_IRQ, GPIO_INPUT_FILTER);

	/* Disable event counter for GPIO7 (DCONIRQ) and GPIO12 (DCONBLANK) */
	cs5535_gpio_clear(OLPC_GPIO_DCON_IRQ, GPIO_INPUT_EVENT_COUNT);
	cs5535_gpio_clear(OLPC_GPIO_DCON_BLANK, GPIO_INPUT_EVENT_COUNT);

	/* Add GPIO12 to the Filter Event Pair #7 */
	cs5535_gpio_set(OLPC_GPIO_DCON_BLANK, GPIO_FE7_SEL);

	/* Turn off negative Edge Enable for GPIO12 */
	cs5535_gpio_clear(OLPC_GPIO_DCON_BLANK, GPIO_NEGATIVE_EDGE_EN);

	/* Enable negative Edge Enable for GPIO7 */
	cs5535_gpio_set(OLPC_GPIO_DCON_IRQ, GPIO_NEGATIVE_EDGE_EN);

	/* Zero the filter amount for Filter Event Pair #7 */
	cs5535_gpio_set(0, GPIO_FLTR7_AMOUNT);

	/* Clear the negative edge status for GPIO7 and GPIO12 */
	cs5535_gpio_set(OLPC_GPIO_DCON_IRQ, GPIO_NEGATIVE_EDGE_STS);
	cs5535_gpio_set(OLPC_GPIO_DCON_BLANK, GPIO_NEGATIVE_EDGE_STS);

	/* FIXME:  Clear the positive status as well, just to be sure */
	cs5535_gpio_set(OLPC_GPIO_DCON_IRQ, GPIO_POSITIVE_EDGE_STS);
	cs5535_gpio_set(OLPC_GPIO_DCON_BLANK, GPIO_POSITIVE_EDGE_STS);

	/* Enable events for GPIO7 (DCONIRQ) and GPIO12 (DCONBLANK) */
	cs5535_gpio_set(OLPC_GPIO_DCON_IRQ, GPIO_EVENTS_ENABLE);
	cs5535_gpio_set(OLPC_GPIO_DCON_BLANK, GPIO_EVENTS_ENABLE);

	return 0;
<<<<<<< HEAD

err_req_irq:
	gpio_free(OLPC_GPIO_DCON_BLANK);
err_gp_blank:
	gpio_free(OLPC_GPIO_DCON_LOAD);
err_gp_load:
	gpio_free(OLPC_GPIO_DCON_IRQ);
err_gp_irq:
	gpio_free(OLPC_GPIO_DCON_STAT1);
err_gp_stat1:
	gpio_free(OLPC_GPIO_DCON_STAT0);
	return -EIO;
=======
>>>>>>> upstream/android-13
}

static void dcon_wiggle_xo_1(void)
{
	int x;

	/*
	 * According to HiMax, when powering the DCON up we should hold
	 * SMB_DATA high for 8 SMB_CLK cycles.  This will force the DCON
	 * state machine to reset to a (sane) initial state.  Mitch Bradley
	 * did some testing and discovered that holding for 16 SMB_CLK cycles
	 * worked a lot more reliably, so that's what we do here.
	 *
	 * According to the cs5536 spec, to set GPIO14 to SMB_CLK we must
	 * simultaneously set AUX1 IN/OUT to GPIO14; ditto for SMB_DATA and
	 * GPIO15.
	 */
	cs5535_gpio_set(OLPC_GPIO_SMB_CLK, GPIO_OUTPUT_VAL);
	cs5535_gpio_set(OLPC_GPIO_SMB_DATA, GPIO_OUTPUT_VAL);
	cs5535_gpio_set(OLPC_GPIO_SMB_CLK, GPIO_OUTPUT_ENABLE);
	cs5535_gpio_set(OLPC_GPIO_SMB_DATA, GPIO_OUTPUT_ENABLE);
	cs5535_gpio_clear(OLPC_GPIO_SMB_CLK, GPIO_OUTPUT_AUX1);
	cs5535_gpio_clear(OLPC_GPIO_SMB_DATA, GPIO_OUTPUT_AUX1);
	cs5535_gpio_clear(OLPC_GPIO_SMB_CLK, GPIO_OUTPUT_AUX2);
	cs5535_gpio_clear(OLPC_GPIO_SMB_DATA, GPIO_OUTPUT_AUX2);
	cs5535_gpio_clear(OLPC_GPIO_SMB_CLK, GPIO_INPUT_AUX1);
	cs5535_gpio_clear(OLPC_GPIO_SMB_DATA, GPIO_INPUT_AUX1);

	for (x = 0; x < 16; x++) {
		udelay(5);
		cs5535_gpio_clear(OLPC_GPIO_SMB_CLK, GPIO_OUTPUT_VAL);
		udelay(5);
		cs5535_gpio_set(OLPC_GPIO_SMB_CLK, GPIO_OUTPUT_VAL);
	}
	udelay(5);
	cs5535_gpio_set(OLPC_GPIO_SMB_CLK, GPIO_OUTPUT_AUX1);
	cs5535_gpio_set(OLPC_GPIO_SMB_DATA, GPIO_OUTPUT_AUX1);
	cs5535_gpio_set(OLPC_GPIO_SMB_CLK, GPIO_INPUT_AUX1);
	cs5535_gpio_set(OLPC_GPIO_SMB_DATA, GPIO_INPUT_AUX1);
}

static void dcon_set_dconload_1(int val)
{
<<<<<<< HEAD
	gpio_set_value(OLPC_GPIO_DCON_LOAD, val);
=======
	gpiod_set_value(gpios[OLPC_DCON_LOAD], val);
>>>>>>> upstream/android-13
}

static int dcon_read_status_xo_1(u8 *status)
{
<<<<<<< HEAD
	*status = gpio_get_value(OLPC_GPIO_DCON_STAT0);
	*status |= gpio_get_value(OLPC_GPIO_DCON_STAT1) << 1;
=======
	*status = gpiod_get_value(gpios[OLPC_DCON_STAT0]);
	*status |= gpiod_get_value(gpios[OLPC_DCON_STAT1]) << 1;
>>>>>>> upstream/android-13

	/* Clear the negative edge status for GPIO7 */
	cs5535_gpio_set(OLPC_GPIO_DCON_IRQ, GPIO_NEGATIVE_EDGE_STS);

	return 0;
}

struct dcon_platform_data dcon_pdata_xo_1 = {
	.init = dcon_init_xo_1,
	.bus_stabilize_wiggle = dcon_wiggle_xo_1,
	.set_dconload = dcon_set_dconload_1,
	.read_status = dcon_read_status_xo_1,
};
