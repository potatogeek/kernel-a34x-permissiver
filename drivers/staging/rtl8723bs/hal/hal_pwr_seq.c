// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/

/*
*
This file includes all kinds of Power Action event for RTL8723B
and corresponding hardware configurtions which are released from HW SD.

Major Change History:
	When       Who               What
	---------- ---------------   -------------------------------
	2011-08-08 Roger            Create.

*/

#include "hal_pwr_seq.h"

/* drivers should parse below arrays and do the corresponding actions */
/* 3 Power on  Array */
<<<<<<< HEAD
WLAN_PWR_CFG rtl8723B_power_on_flow[
=======
struct wlan_pwr_cfg rtl8723B_power_on_flow[
>>>>>>> upstream/android-13
	RTL8723B_TRANS_CARDEMU_TO_ACT_STEPS+
	RTL8723B_TRANS_END_STEPS
] = {
	RTL8723B_TRANS_CARDEMU_TO_ACT
	RTL8723B_TRANS_END
};

/* 3Radio off GPIO Array */
<<<<<<< HEAD
WLAN_PWR_CFG rtl8723B_radio_off_flow[
=======
struct wlan_pwr_cfg rtl8723B_radio_off_flow[
>>>>>>> upstream/android-13
	RTL8723B_TRANS_ACT_TO_CARDEMU_STEPS+
	RTL8723B_TRANS_END_STEPS
] = {
	RTL8723B_TRANS_ACT_TO_CARDEMU
	RTL8723B_TRANS_END
};

/* 3Card Disable Array */
<<<<<<< HEAD
WLAN_PWR_CFG rtl8723B_card_disable_flow[
=======
struct wlan_pwr_cfg rtl8723B_card_disable_flow[
>>>>>>> upstream/android-13
	RTL8723B_TRANS_ACT_TO_CARDEMU_STEPS+
	RTL8723B_TRANS_CARDEMU_TO_PDN_STEPS+
	RTL8723B_TRANS_END_STEPS
] = {
	RTL8723B_TRANS_ACT_TO_CARDEMU
	RTL8723B_TRANS_CARDEMU_TO_CARDDIS
	RTL8723B_TRANS_END
};

/* 3 Card Enable Array */
<<<<<<< HEAD
WLAN_PWR_CFG rtl8723B_card_enable_flow[
=======
struct wlan_pwr_cfg rtl8723B_card_enable_flow[
>>>>>>> upstream/android-13
	RTL8723B_TRANS_ACT_TO_CARDEMU_STEPS+
	RTL8723B_TRANS_CARDEMU_TO_PDN_STEPS+
	RTL8723B_TRANS_END_STEPS
] = {
	RTL8723B_TRANS_CARDDIS_TO_CARDEMU
	RTL8723B_TRANS_CARDEMU_TO_ACT
	RTL8723B_TRANS_END
};

/* 3Suspend Array */
<<<<<<< HEAD
WLAN_PWR_CFG rtl8723B_suspend_flow[
=======
struct wlan_pwr_cfg rtl8723B_suspend_flow[
>>>>>>> upstream/android-13
	RTL8723B_TRANS_ACT_TO_CARDEMU_STEPS+
	RTL8723B_TRANS_CARDEMU_TO_SUS_STEPS+
	RTL8723B_TRANS_END_STEPS
] = {
	RTL8723B_TRANS_ACT_TO_CARDEMU
	RTL8723B_TRANS_CARDEMU_TO_SUS
	RTL8723B_TRANS_END
};

/* 3 Resume Array */
<<<<<<< HEAD
WLAN_PWR_CFG rtl8723B_resume_flow[
=======
struct wlan_pwr_cfg rtl8723B_resume_flow[
>>>>>>> upstream/android-13
	RTL8723B_TRANS_ACT_TO_CARDEMU_STEPS+
	RTL8723B_TRANS_CARDEMU_TO_SUS_STEPS+
	RTL8723B_TRANS_END_STEPS
] = {
	RTL8723B_TRANS_SUS_TO_CARDEMU
	RTL8723B_TRANS_CARDEMU_TO_ACT
	RTL8723B_TRANS_END
};

/* 3HWPDN Array */
<<<<<<< HEAD
WLAN_PWR_CFG rtl8723B_hwpdn_flow[
=======
struct wlan_pwr_cfg rtl8723B_hwpdn_flow[
>>>>>>> upstream/android-13
	RTL8723B_TRANS_ACT_TO_CARDEMU_STEPS+
	RTL8723B_TRANS_CARDEMU_TO_PDN_STEPS+
	RTL8723B_TRANS_END_STEPS
] = {
	RTL8723B_TRANS_ACT_TO_CARDEMU
	RTL8723B_TRANS_CARDEMU_TO_PDN
	RTL8723B_TRANS_END
};

/* 3 Enter LPS */
<<<<<<< HEAD
WLAN_PWR_CFG rtl8723B_enter_lps_flow[
=======
struct wlan_pwr_cfg rtl8723B_enter_lps_flow[
>>>>>>> upstream/android-13
	RTL8723B_TRANS_ACT_TO_LPS_STEPS+RTL8723B_TRANS_END_STEPS
] = {
	/* FW behavior */
	RTL8723B_TRANS_ACT_TO_LPS
	RTL8723B_TRANS_END
};

/* 3 Leave LPS */
<<<<<<< HEAD
WLAN_PWR_CFG rtl8723B_leave_lps_flow[
=======
struct wlan_pwr_cfg rtl8723B_leave_lps_flow[
>>>>>>> upstream/android-13
	RTL8723B_TRANS_LPS_TO_ACT_STEPS+RTL8723B_TRANS_END_STEPS
] = {
	/* FW behavior */
	RTL8723B_TRANS_LPS_TO_ACT
	RTL8723B_TRANS_END
};

/* 3 Enter SW LPS */
<<<<<<< HEAD
WLAN_PWR_CFG rtl8723B_enter_swlps_flow[
=======
struct wlan_pwr_cfg rtl8723B_enter_swlps_flow[
>>>>>>> upstream/android-13
	RTL8723B_TRANS_ACT_TO_SWLPS_STEPS+RTL8723B_TRANS_END_STEPS
] = {
	/* SW behavior */
	RTL8723B_TRANS_ACT_TO_SWLPS
	RTL8723B_TRANS_END
};

/* 3 Leave SW LPS */
<<<<<<< HEAD
WLAN_PWR_CFG rtl8723B_leave_swlps_flow[
=======
struct wlan_pwr_cfg rtl8723B_leave_swlps_flow[
>>>>>>> upstream/android-13
	RTL8723B_TRANS_SWLPS_TO_ACT_STEPS+RTL8723B_TRANS_END_STEPS
] = {
	/* SW behavior */
	RTL8723B_TRANS_SWLPS_TO_ACT
	RTL8723B_TRANS_END
};
