<<<<<<< HEAD
/******************************************************************************
 *
 * Copyright(c) 2009-2012  Realtek Corporation.
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
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 *****************************************************************************/
=======
// SPDX-License-Identifier: GPL-2.0
/* Copyright(c) 2009-2012  Realtek Corporation.*/
>>>>>>> upstream/android-13

#include "../wifi.h"
#include "../base.h"
#include "../pci.h"
#include "../core.h"
#include "reg.h"
#include "def.h"
#include "phy.h"
#include "dm.h"
#include "../rtl8192c/fw_common.h"

void rtl92ce_dm_dynamic_txpower(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_phy *rtlphy = &(rtlpriv->phy);
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	long undec_sm_pwdb;

	if (!rtlpriv->dm.dynamic_txpower_enable)
		return;

	if (rtlpriv->dm.dm_flag & HAL_DM_HIPWR_DISABLE) {
		rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_NORMAL;
		return;
	}

	if ((mac->link_state < MAC80211_LINKED) &&
	    (rtlpriv->dm.entry_min_undec_sm_pwdb == 0)) {
<<<<<<< HEAD
		RT_TRACE(rtlpriv, COMP_POWER, DBG_TRACE,
			 "Not connected to any\n");
=======
		rtl_dbg(rtlpriv, COMP_POWER, DBG_TRACE,
			"Not connected to any\n");
>>>>>>> upstream/android-13

		rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_NORMAL;

		rtlpriv->dm.last_dtp_lvl = TXHIGHPWRLEVEL_NORMAL;
		return;
	}

	if (mac->link_state >= MAC80211_LINKED) {
		if (mac->opmode == NL80211_IFTYPE_ADHOC) {
			undec_sm_pwdb = rtlpriv->dm.entry_min_undec_sm_pwdb;
<<<<<<< HEAD
			RT_TRACE(rtlpriv, COMP_POWER, DBG_LOUD,
				 "AP Client PWDB = 0x%lx\n",
				 undec_sm_pwdb);
		} else {
			undec_sm_pwdb = rtlpriv->dm.undec_sm_pwdb;
			RT_TRACE(rtlpriv, COMP_POWER, DBG_LOUD,
				 "STA Default Port PWDB = 0x%lx\n",
				 undec_sm_pwdb);
=======
			rtl_dbg(rtlpriv, COMP_POWER, DBG_LOUD,
				"AP Client PWDB = 0x%lx\n",
				undec_sm_pwdb);
		} else {
			undec_sm_pwdb = rtlpriv->dm.undec_sm_pwdb;
			rtl_dbg(rtlpriv, COMP_POWER, DBG_LOUD,
				"STA Default Port PWDB = 0x%lx\n",
				undec_sm_pwdb);
>>>>>>> upstream/android-13
		}
	} else {
		undec_sm_pwdb = rtlpriv->dm.entry_min_undec_sm_pwdb;

<<<<<<< HEAD
		RT_TRACE(rtlpriv, COMP_POWER, DBG_LOUD,
			 "AP Ext Port PWDB = 0x%lx\n",
			 undec_sm_pwdb);
=======
		rtl_dbg(rtlpriv, COMP_POWER, DBG_LOUD,
			"AP Ext Port PWDB = 0x%lx\n",
			undec_sm_pwdb);
>>>>>>> upstream/android-13
	}

	if (undec_sm_pwdb >= TX_POWER_NEAR_FIELD_THRESH_LVL2) {
		rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_LEVEL1;
<<<<<<< HEAD
		RT_TRACE(rtlpriv, COMP_POWER, DBG_LOUD,
			 "TXHIGHPWRLEVEL_LEVEL1 (TxPwr=0x0)\n");
=======
		rtl_dbg(rtlpriv, COMP_POWER, DBG_LOUD,
			"TXHIGHPWRLEVEL_LEVEL1 (TxPwr=0x0)\n");
>>>>>>> upstream/android-13
	} else if ((undec_sm_pwdb < (TX_POWER_NEAR_FIELD_THRESH_LVL2 - 3)) &&
		   (undec_sm_pwdb >= TX_POWER_NEAR_FIELD_THRESH_LVL1)) {

		rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_LEVEL1;
<<<<<<< HEAD
		RT_TRACE(rtlpriv, COMP_POWER, DBG_LOUD,
			 "TXHIGHPWRLEVEL_LEVEL1 (TxPwr=0x10)\n");
	} else if (undec_sm_pwdb < (TX_POWER_NEAR_FIELD_THRESH_LVL1 - 5)) {
		rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_NORMAL;
		RT_TRACE(rtlpriv, COMP_POWER, DBG_LOUD,
			 "TXHIGHPWRLEVEL_NORMAL\n");
	}

	if ((rtlpriv->dm.dynamic_txhighpower_lvl != rtlpriv->dm.last_dtp_lvl)) {
		RT_TRACE(rtlpriv, COMP_POWER, DBG_LOUD,
			 "PHY_SetTxPowerLevel8192S() Channel = %d\n",
			 rtlphy->current_channel);
=======
		rtl_dbg(rtlpriv, COMP_POWER, DBG_LOUD,
			"TXHIGHPWRLEVEL_LEVEL1 (TxPwr=0x10)\n");
	} else if (undec_sm_pwdb < (TX_POWER_NEAR_FIELD_THRESH_LVL1 - 5)) {
		rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_NORMAL;
		rtl_dbg(rtlpriv, COMP_POWER, DBG_LOUD,
			"TXHIGHPWRLEVEL_NORMAL\n");
	}

	if ((rtlpriv->dm.dynamic_txhighpower_lvl != rtlpriv->dm.last_dtp_lvl)) {
		rtl_dbg(rtlpriv, COMP_POWER, DBG_LOUD,
			"PHY_SetTxPowerLevel8192S() Channel = %d\n",
			rtlphy->current_channel);
>>>>>>> upstream/android-13
		rtl92c_phy_set_txpower_level(hw, rtlphy->current_channel);
	}

	rtlpriv->dm.last_dtp_lvl = rtlpriv->dm.dynamic_txhighpower_lvl;
}
