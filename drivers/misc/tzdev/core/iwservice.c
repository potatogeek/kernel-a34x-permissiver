/*
<<<<<<< HEAD
 * Copyright (C) 2012-2019, Samsung Electronics Co., Ltd.
=======
 * Copyright (c) 2017 Samsung Electronics Co., Ltd All Rights Reserved
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

#include <linux/errno.h>
#include <linux/types.h>

#include "tzdev_internal.h"
#include "core/log.h"
#include "core/iwio.h"

struct iw_service_channel {
	uint32_t cpu_mask;
	uint32_t user_cpu_mask;
	uint32_t cpu_freq[NR_CPUS];
} __attribute__((__packed__));

static struct iw_service_channel *iw_channel;

unsigned long tz_iwservice_get_cpu_mask(void)
{
	if (!iw_channel)
		return 0;

<<<<<<< HEAD
	return (iw_channel->cpu_mask | iw_channel->user_cpu_mask);
=======
	return iw_channel->cpu_mask;
>>>>>>> upstream/android-13
}

unsigned long tz_iwservice_get_user_cpu_mask(void)
{
	if (!iw_channel)
		return 0;

	return iw_channel->user_cpu_mask;
}

void tz_iwservice_set_cpu_freq(unsigned int cpu, unsigned int cpu_freq)
{
	if (!iw_channel)
		return;

	iw_channel->cpu_freq[cpu] = cpu_freq;
}

int tz_iwservice_init(void)
{
	struct iw_service_channel *ch;

	ch = tz_iwio_alloc_iw_channel(TZ_IWIO_CONNECT_SERVICE, 1, NULL, NULL, NULL);
	if (!ch) {
		log_error(tzdev_iwservice, "Failed to initialize iw service channel\n");
		return -ENOMEM;
	}

	iw_channel = ch;

	log_info(tzdev_iwservice, "IW service initialization done.\n");

	return 0;
}
