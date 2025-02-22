// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) 2010, 2014 The Linux Foundation. All rights reserved.  */

<<<<<<< HEAD
#include <linux/init.h>
=======
#include <linux/console.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
>>>>>>> upstream/android-13

#include <asm/dcc.h>
#include <asm/processor.h>

#include "hvc_console.h"

<<<<<<< HEAD
=======
/*
 * Disable DCC driver at runtime. Want driver enabled for GKI, but some devices
 * do not support the registers and crash when driver pokes the registers
 */
static bool enable;
module_param(enable, bool, 0444);

>>>>>>> upstream/android-13
/* DCC Status Bits */
#define DCC_STATUS_RX		(1 << 30)
#define DCC_STATUS_TX		(1 << 29)

<<<<<<< HEAD
=======
static void dcc_uart_console_putchar(struct uart_port *port, int ch)
{
	while (__dcc_getstatus() & DCC_STATUS_TX)
		cpu_relax();

	__dcc_putchar(ch);
}

static void dcc_early_write(struct console *con, const char *s, unsigned n)
{
	struct earlycon_device *dev = con->data;

	uart_console_write(&dev->port, s, n, dcc_uart_console_putchar);
}

static int __init dcc_early_console_setup(struct earlycon_device *device,
					  const char *opt)
{
	device->con->write = dcc_early_write;

	return 0;
}

EARLYCON_DECLARE(dcc, dcc_early_console_setup);

>>>>>>> upstream/android-13
static int hvc_dcc_put_chars(uint32_t vt, const char *buf, int count)
{
	int i;

	for (i = 0; i < count; i++) {
		while (__dcc_getstatus() & DCC_STATUS_TX)
			cpu_relax();

		__dcc_putchar(buf[i]);
	}

	return count;
}

static int hvc_dcc_get_chars(uint32_t vt, char *buf, int count)
{
	int i;

	for (i = 0; i < count; ++i)
		if (__dcc_getstatus() & DCC_STATUS_RX)
			buf[i] = __dcc_getchar();
		else
			break;

	return i;
}

static bool hvc_dcc_check(void)
{
	unsigned long time = jiffies + (HZ / 10);

	/* Write a test character to check if it is handled */
	__dcc_putchar('\n');

	while (time_is_after_jiffies(time)) {
		if (!(__dcc_getstatus() & DCC_STATUS_TX))
			return true;
	}

	return false;
}

static const struct hv_ops hvc_dcc_get_put_ops = {
	.get_chars = hvc_dcc_get_chars,
	.put_chars = hvc_dcc_put_chars,
};

static int __init hvc_dcc_console_init(void)
{
	int ret;

<<<<<<< HEAD
	if (!hvc_dcc_check())
=======
	if (!enable || !hvc_dcc_check())
>>>>>>> upstream/android-13
		return -ENODEV;

	/* Returns -1 if error */
	ret = hvc_instantiate(0, 0, &hvc_dcc_get_put_ops);

	return ret < 0 ? -ENODEV : 0;
}
console_initcall(hvc_dcc_console_init);

static int __init hvc_dcc_init(void)
{
	struct hvc_struct *p;

<<<<<<< HEAD
	if (!hvc_dcc_check())
=======
	if (!enable || !hvc_dcc_check())
>>>>>>> upstream/android-13
		return -ENODEV;

	p = hvc_alloc(0, 0, &hvc_dcc_get_put_ops, 128);

	return PTR_ERR_OR_ZERO(p);
}
device_initcall(hvc_dcc_init);
