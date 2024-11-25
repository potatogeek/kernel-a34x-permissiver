<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * MX35 CPU type detection
 *
 * Copyright (c) 2009 Daniel Mack <daniel@caiaq.de>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include <linux/module.h>
=======
 */
#include <linux/module.h>
#include <linux/of_address.h>
>>>>>>> upstream/android-13
#include <linux/io.h>

#include "hardware.h"
#include "iim.h"

static int mx35_cpu_rev = -1;

static int mx35_read_cpu_rev(void)
{
<<<<<<< HEAD
	u32 rev;

	rev = imx_readl(MX35_IO_ADDRESS(MX35_IIM_BASE_ADDR + MXC_IIMSREV));
=======
	void __iomem *iim_base;
	struct device_node *np;
	u32 rev;

	np = of_find_compatible_node(NULL, NULL, "fsl,imx35-iim");
	iim_base = of_iomap(np, 0);
	BUG_ON(!iim_base);

	rev = imx_readl(iim_base + MXC_IIMSREV);
>>>>>>> upstream/android-13
	switch (rev) {
	case 0x00:
		return IMX_CHIP_REVISION_1_0;
	case 0x10:
		return IMX_CHIP_REVISION_2_0;
	case 0x11:
		return IMX_CHIP_REVISION_2_1;
	default:
		return IMX_CHIP_REVISION_UNKNOWN;
	}
}

int mx35_revision(void)
{
	if (mx35_cpu_rev == -1)
		mx35_cpu_rev = mx35_read_cpu_rev();

	return mx35_cpu_rev;
}
EXPORT_SYMBOL(mx35_revision);
