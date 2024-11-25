<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/* include/video/platform_lcd.h
 *
 * Copyright 2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * Generic platform-device LCD power control interface.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
*/

struct plat_lcd_data;
struct fb_info;

struct plat_lcd_data {
	int	(*probe)(struct plat_lcd_data *);
	void	(*set_power)(struct plat_lcd_data *, unsigned int power);
	int	(*match_fb)(struct plat_lcd_data *, struct fb_info *);
};

