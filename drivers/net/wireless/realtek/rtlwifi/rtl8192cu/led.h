<<<<<<< HEAD
/******************************************************************************
 *
 * Copyright(c) 2009-2012  Realtek Corporation. All rights reserved.
 *
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
 * Realtek Corporation, No. 2, Innovation Road II, Hsinchu Science Park,
 * Hsinchu 300, Taiwan.
 *
 *****************************************************************************/
=======
/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright(c) 2009-2012  Realtek Corporation.*/
>>>>>>> upstream/android-13

#ifndef __RTL92CU_LED_H__
#define __RTL92CU_LED_H__

void rtl92cu_init_sw_leds(struct ieee80211_hw *hw);
void rtl92cu_deinit_sw_leds(struct ieee80211_hw *hw);
void rtl92cu_sw_led_on(struct ieee80211_hw *hw, struct rtl_led *pled);
void rtl92cu_sw_led_off(struct ieee80211_hw *hw, struct rtl_led *pled);
void rtl92cu_led_control(struct ieee80211_hw *hw, enum led_ctl_mode ledaction);

#endif
