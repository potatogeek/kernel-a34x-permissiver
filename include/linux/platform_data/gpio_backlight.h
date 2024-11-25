<<<<<<< HEAD
/*
 * gpio_backlight.h - Simple GPIO-controlled backlight
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * gpio_backlight.h - Simple GPIO-controlled backlight
>>>>>>> upstream/android-13
 */
#ifndef __GPIO_BACKLIGHT_H__
#define __GPIO_BACKLIGHT_H__

struct device;

struct gpio_backlight_platform_data {
	struct device *fbdev;
<<<<<<< HEAD
	int gpio;
	int def_value;
	const char *name;
=======
>>>>>>> upstream/android-13
};

#endif
