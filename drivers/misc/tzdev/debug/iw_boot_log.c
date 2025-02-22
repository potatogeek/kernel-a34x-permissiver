/*
<<<<<<< HEAD
 * Copyright (C) 2013-2019 Samsung Electronics, Inc.
=======
 * Copyright (c) 2013 Samsung Electronics Co., Ltd All Rights Reserved
>>>>>>> upstream/android-13
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/atomic.h>
#include <linux/kernel.h>
<<<<<<< HEAD
=======
#include <linux/limits.h>
>>>>>>> upstream/android-13
#include <linux/mm.h>
#include <linux/printk.h>
#include <linux/string.h>

#include "tzdev_internal.h"
<<<<<<< HEAD
=======
#include "tzdev_limits.h"
>>>>>>> upstream/android-13
#include "core/cdev.h"
#include "core/log.h"
#include "debug/iw_boot_log.h"

#define TZ_BOOT_LOG_PREFIX		KERN_DEFAULT "SW_BOOT> "

static atomic_t tz_iw_boot_log_already_read = ATOMIC_INIT(0);

static void tz_iw_boot_log_print(char *buf, unsigned long nbytes)
{
	unsigned long printed;
	char *p;

	while (nbytes) {
		p = memchr(buf, '\n', nbytes);

		if (p) {
			*p = '\0';
			printed = p - buf + 1;
		} else {
			printed = nbytes;
		}

		printk(TZ_BOOT_LOG_PREFIX "%.*s\n", (int)printed, buf);

		nbytes -= printed;
		buf += printed;
	}
}

void tz_iw_boot_log_read(void)
{
	struct page *pages;
	unsigned int order;
<<<<<<< HEAD
=======
	unsigned long pfn;
>>>>>>> upstream/android-13
	int nbytes;

	if (atomic_cmpxchg(&tz_iw_boot_log_already_read, 0, 1))
		return;

	order = order_base_2(CONFIG_TZ_BOOT_LOG_PG_CNT);

	pages = alloc_pages(GFP_KERNEL, order);
	if (!pages)
		return;

<<<<<<< HEAD
	nbytes = tzdev_smc_boot_log_read(page_to_pfn(pages), CONFIG_TZ_BOOT_LOG_PG_CNT);
=======
	pfn = page_to_pfn(pages);

	BUG_ON(pfn > U32_MAX);

	nbytes = tzdev_smc_boot_log_read((unsigned int)pfn, CONFIG_TZ_BOOT_LOG_PG_CNT);
>>>>>>> upstream/android-13
	if (nbytes > 0)
		tz_iw_boot_log_print(page_address(pages), nbytes);

	__free_pages(pages, order);
}
