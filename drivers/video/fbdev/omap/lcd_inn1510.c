<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * LCD panel support for the TI OMAP1510 Innovator board
 *
 * Copyright (C) 2004 Nokia Corporation
 * Author: Imre Deak <imre.deak@nokia.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#include <mach/hardware.h>

#include "omapfb.h"

static int innovator1510_panel_enable(struct lcd_panel *panel)
{
	__raw_writeb(0x7, OMAP1510_FPGA_LCD_PANEL_CONTROL);
	return 0;
}

static void innovator1510_panel_disable(struct lcd_panel *panel)
{
	__raw_writeb(0x0, OMAP1510_FPGA_LCD_PANEL_CONTROL);
}

static struct lcd_panel innovator1510_panel = {
	.name		= "inn1510",
	.config		= OMAP_LCDC_PANEL_TFT,

	.bpp		= 16,
	.data_lines	= 16,
	.x_res		= 240,
	.y_res		= 320,
	.pixel_clock	= 12500,
	.hsw		= 40,
	.hfp		= 40,
	.hbp		= 72,
	.vsw		= 1,
	.vfp		= 1,
	.vbp		= 0,
	.pcd		= 12,

	.enable		= innovator1510_panel_enable,
	.disable	= innovator1510_panel_disable,
};

static int innovator1510_panel_probe(struct platform_device *pdev)
{
	omapfb_register_panel(&innovator1510_panel);
	return 0;
}

static struct platform_driver innovator1510_panel_driver = {
	.probe		= innovator1510_panel_probe,
	.driver		= {
		.name	= "lcd_inn1510",
	},
};

module_platform_driver(innovator1510_panel_driver);

MODULE_AUTHOR("Imre Deak");
MODULE_DESCRIPTION("LCD panel support for the TI OMAP1510 Innovator board");
MODULE_LICENSE("GPL");
