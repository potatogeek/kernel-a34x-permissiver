<<<<<<< HEAD
/*
 * Copyright (C) 2015 Imagination Technologies
 * Author: Paul Burton <paul.burton@mips.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2015 Imagination Technologies
 * Author: Paul Burton <paul.burton@mips.com>
>>>>>>> upstream/android-13
 */

#include <linux/clk-provider.h>
#include <linux/init.h>
#include <linux/of_fdt.h>
#include <linux/of_platform.h>

void __init device_tree_init(void)
{
	unflatten_and_copy_device_tree();
}
