<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  PS3 time and rtc routines.
 *
 *  Copyright (C) 2006 Sony Computer Entertainment Inc.
 *  Copyright 2006 Sony Corp.
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/rtc.h>

#include <asm/firmware.h>
#include <asm/lv1call.h>
#include <asm/ps3.h>

#include "platform.h"

void __init ps3_calibrate_decr(void)
{
	int result;
	u64 tmp;

	result = ps3_repository_read_be_tb_freq(0, &tmp);
	BUG_ON(result);

	ppc_tb_freq = tmp;
	ppc_proc_freq = ppc_tb_freq * 40;
}

static u64 read_rtc(void)
{
	int result;
	u64 rtc_val;
	u64 tb_val;

	result = lv1_get_rtc(&rtc_val, &tb_val);
	BUG_ON(result);

	return rtc_val;
}

time64_t __init ps3_get_boot_time(void)
{
	return read_rtc() + ps3_os_area_get_rtc_diff();
}

static int __init ps3_rtc_init(void)
{
	struct platform_device *pdev;

	if (!firmware_has_feature(FW_FEATURE_PS3_LV1))
		return -ENODEV;

	pdev = platform_device_register_simple("rtc-ps3", -1, NULL, 0);

	return PTR_ERR_OR_ZERO(pdev);
}
device_initcall(ps3_rtc_init);
