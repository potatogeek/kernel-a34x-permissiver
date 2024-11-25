/*
<<<<<<< HEAD
 * Copyright (C) 2012-2017, Samsung Electronics Co., Ltd.
=======
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TZ_CRED_H__
#define __TZ_CRED_H__

#include <linux/kconfig.h>
#include <tz_common.h>

#define UUID_SIZE		16

#define UUID_TIME_HI_MASK	0xFFF
#define UUID_VERSION		5
#define UUID_VERSION_SHIFT	12

<<<<<<< HEAD
#if IS_MODULE(CONFIG_TZDEV)
int tz_cred_init_notifier(void);
#endif

=======
>>>>>>> upstream/android-13
int tz_format_cred(struct tz_cred *cred, int is_kern);

#endif /* __TZ_CRED_H__ */
