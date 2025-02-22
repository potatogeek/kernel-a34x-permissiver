<<<<<<< HEAD
/******************************************************************************
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * Copyright(c) 2007 - 2014 Intel Corporation. All rights reserved.
 * Copyright(c) 2018 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;
 *
 * The full GNU General Public License is included in this distribution
 * in the file called COPYING.
 *
 * Contact Information:
 *  Intel Linux Wireless <linuxwifi@intel.com>
 * Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497
 *
 * BSD LICENSE
 *
 * Copyright(c) 2005 - 2014 Intel Corporation. All rights reserved.
 * Copyright(c) 2018 Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/
=======
/* SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause */
/*
 * Copyright (C) 2005-2014, 2018-2020 Intel Corporation
 */
>>>>>>> upstream/android-13
#ifndef __iwl_modparams_h__
#define __iwl_modparams_h__

#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/gfp.h>

extern struct iwl_mod_params iwlwifi_mod_params;

enum iwl_power_level {
	IWL_POWER_INDEX_1,
	IWL_POWER_INDEX_2,
	IWL_POWER_INDEX_3,
	IWL_POWER_INDEX_4,
	IWL_POWER_INDEX_5,
	IWL_POWER_NUM
};

enum iwl_disable_11n {
	IWL_DISABLE_HT_ALL	 = BIT(0),
	IWL_DISABLE_HT_TXAGG	 = BIT(1),
	IWL_DISABLE_HT_RXAGG	 = BIT(2),
	IWL_ENABLE_HT_TXAGG	 = BIT(3),
};

enum iwl_amsdu_size {
	IWL_AMSDU_DEF = 0,
	IWL_AMSDU_4K = 1,
	IWL_AMSDU_8K = 2,
	IWL_AMSDU_12K = 3,
	/* Add 2K at the end to avoid breaking current API */
	IWL_AMSDU_2K = 4,
};

enum iwl_uapsd_disable {
	IWL_DISABLE_UAPSD_BSS		= BIT(0),
	IWL_DISABLE_UAPSD_P2P_CLIENT	= BIT(1),
};

/**
 * struct iwl_mod_params
 *
 * Holds the module parameters
 *
 * @swcrypto: using hardware encryption, default = 0
 * @disable_11n: disable 11n capabilities, default = 0,
 *	use IWL_[DIS,EN]ABLE_HT_* constants
 * @amsdu_size: See &enum iwl_amsdu_size.
 * @fw_restart: restart firmware, default = 1
 * @bt_coex_active: enable bt coex, default = true
 * @led_mode: system default, default = 0
 * @power_save: enable power save, default = false
 * @power_level: power level, default = 1
 * @debug_level: levels are IWL_DL_*
<<<<<<< HEAD
 * @antenna_coupling: antenna coupling in dB, default = 0
 * @nvm_file: specifies a external NVM file
 * @uapsd_disable: disable U-APSD, see &enum iwl_uapsd_disable, default =
 *	IWL_DISABLE_UAPSD_BSS | IWL_DISABLE_UAPSD_P2P_CLIENT
 * @d0i3_disable: disable d0i3, default = 1,
 * @d0i3_timeout: time to wait after no refs are taken before
 *	entering D0i3 (in msecs)
 * @lar_disable: disable LAR (regulatory), default = 0
 * @fw_monitor: allow to use firmware monitor
 * @disable_11ac: disable VHT capabilities, default = false.
 * @remove_when_gone: remove an inaccessible device from the PCIe bus.
=======
 * @nvm_file: specifies a external NVM file
 * @uapsd_disable: disable U-APSD, see &enum iwl_uapsd_disable, default =
 *	IWL_DISABLE_UAPSD_BSS | IWL_DISABLE_UAPSD_P2P_CLIENT
 * @disable_11ac: disable VHT capabilities, default = false.
 * @remove_when_gone: remove an inaccessible device from the PCIe bus.
 * @enable_ini: enable new FW debug infratructure (INI TLVs)
>>>>>>> upstream/android-13
 */
struct iwl_mod_params {
	int swcrypto;
	unsigned int disable_11n;
	int amsdu_size;
	bool fw_restart;
	bool bt_coex_active;
	int led_mode;
	bool power_save;
	int power_level;
#ifdef CONFIG_IWLWIFI_DEBUG
	u32 debug_level;
#endif
<<<<<<< HEAD
	int antenna_coupling;
	char *nvm_file;
	u32 uapsd_disable;
	bool d0i3_disable;
	unsigned int d0i3_timeout;
	bool lar_disable;
	bool fw_monitor;
=======
	char *nvm_file;
	u32 uapsd_disable;
>>>>>>> upstream/android-13
	bool disable_11ac;
	/**
	 * @disable_11ax: disable HE capabilities, default = false
	 */
	bool disable_11ax;
	bool remove_when_gone;
<<<<<<< HEAD
};

=======
	bool enable_ini;
};

static inline bool iwl_enable_rx_ampdu(void)
{
	if (iwlwifi_mod_params.disable_11n & IWL_DISABLE_HT_RXAGG)
		return false;
	return true;
}

static inline bool iwl_enable_tx_ampdu(void)
{
	if (iwlwifi_mod_params.disable_11n & IWL_DISABLE_HT_TXAGG)
		return false;
	if (iwlwifi_mod_params.disable_11n & IWL_ENABLE_HT_TXAGG)
		return true;

	/* enabled by default */
	return true;
}

>>>>>>> upstream/android-13
#endif /* #__iwl_modparams_h__ */
