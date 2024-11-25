<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *      intel-mid_wdt: generic Intel MID SCU watchdog driver
 *
 *      Copyright (C) 2014 Intel Corporation. All rights reserved.
 *      Contact: David Cohen <david.a.cohen@linux.intel.com>
<<<<<<< HEAD
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of version 2 of the GNU General
 *      Public License as published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __INTEL_MID_WDT_H__
#define __INTEL_MID_WDT_H__

#include <linux/platform_device.h>

struct intel_mid_wdt_pdata {
	int irq;
	int (*probe)(struct platform_device *pdev);
};

#endif /*__INTEL_MID_WDT_H__*/
