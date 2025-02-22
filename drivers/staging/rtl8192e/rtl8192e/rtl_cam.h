<<<<<<< HEAD
/******************************************************************************
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
>>>>>>> upstream/android-13
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Based on the r8180 driver, which is:
 * Copyright 2004-2005 Andrea Merello <andrea.merello@gmail.com>, et al.
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 *****************************************************************************/
=======
 *
 * Contact Information: wlanfae <wlanfae@realtek.com>
 */
>>>>>>> upstream/android-13
#ifndef _RTL_CAM_H
#define _RTL_CAM_H

#include <linux/types.h>
struct net_device;

void rtl92e_cam_reset(struct net_device *dev);
void rtl92e_enable_hw_security_config(struct net_device *dev);
void rtl92e_set_key(struct net_device *dev, u8 EntryNo, u8 KeyIndex,
		    u16 KeyType, const u8 *MacAddr, u8 DefaultKey,
		    u32 *KeyContent);
void rtl92e_set_swcam(struct net_device *dev, u8 EntryNo, u8 KeyIndex,
		      u16 KeyType, const u8 *MacAddr, u8 DefaultKey,
		      u32 *KeyContent, u8 is_mesh);
void rtl92e_cam_restore(struct net_device *dev);

#endif
