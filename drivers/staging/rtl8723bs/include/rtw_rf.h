/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#ifndef	__RTW_RF_H_
#define __RTW_RF_H_


#define OFDM_PHY		1
#define MIXED_PHY		2
#define CCK_PHY			3

#define NumRates		13

/*  slot time for 11g */
#define SHORT_SLOT_TIME		9
#define NON_SHORT_SLOT_TIME	20

#define RTL8711_RF_MAX_SENS	 6
#define RTL8711_RF_DEF_SENS	 4

<<<<<<< HEAD
/*  */
/*  We now define the following channels as the max channels in each channel plan. */
/*  2G, total 14 chnls */
/*  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14} */
/*  5G, total 24 chnls */
/*  {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120,
 *   124, 128, 132, 136, 140, 149, 153, 157, 161, 165} */
#define	MAX_CHANNEL_NUM_2G	14
#define	MAX_CHANNEL_NUM_5G	24
#define	MAX_CHANNEL_NUM		38/* 14+24 */
=======
/*
 * We now define the following channels as the max channels in each channel plan.
 * 2G, total 14 chnls
 * {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}
 */
#define	MAX_CHANNEL_NUM_2G	14
#define	MAX_CHANNEL_NUM		14
>>>>>>> upstream/android-13

#define NUM_REGULATORYS	1

/* Country codes */
#define USA			0x555320
#define EUROPE			0x1 /* temp, should be provided later */
#define JAPAN			0x2 /* temp, should be provided later */

struct	regulatory_class {
	u32 starting_freq;			/* MHz, */
	u8 channel_set[MAX_CHANNEL_NUM];
	u8 channel_cck_power[MAX_CHANNEL_NUM];/* dbm */
	u8 channel_ofdm_power[MAX_CHANNEL_NUM];/* dbm */
	u8 txpower_limit;			/* dbm */
	u8 channel_spacing;			/* MHz */
	u8 modem;
};

<<<<<<< HEAD
enum CAPABILITY {
=======
enum {
>>>>>>> upstream/android-13
	cESS			= 0x0001,
	cIBSS			= 0x0002,
	cPollable		= 0x0004,
	cPollReq		= 0x0008,
	cPrivacy		= 0x0010,
	cShortPreamble		= 0x0020,
	cPBCC			= 0x0040,
	cChannelAgility		= 0x0080,
	cSpectrumMgnt		= 0x0100,
	cQos			= 0x0200,	/*  For HCCA, use with CF-Pollable and CF-PollReq */
	cShortSlotTime		= 0x0400,
	cAPSD			= 0x0800,
	cRM			= 0x1000,	/*  RRM (Radio Request Measurement) */
	cDSSS_OFDM		= 0x2000,
	cDelayedBA		= 0x4000,
	cImmediateBA		= 0x8000,
};

<<<<<<< HEAD
enum	_REG_PREAMBLE_MODE {
=======
enum {
>>>>>>> upstream/android-13
	PREAMBLE_LONG	= 1,
	PREAMBLE_AUTO	= 2,
	PREAMBLE_SHORT	= 3,
};

<<<<<<< HEAD
enum _RTL8712_RF_MIMO_CONFIG_ {
	RTL8712_RFCONFIG_1T = 0x10,
	RTL8712_RFCONFIG_2T = 0x20,
	RTL8712_RFCONFIG_1R = 0x01,
	RTL8712_RFCONFIG_2R = 0x02,
	RTL8712_RFCONFIG_1T1R = 0x11,
	RTL8712_RFCONFIG_1T2R = 0x12,
	RTL8712_RFCONFIG_TURBO = 0x92,
	RTL8712_RFCONFIG_2T2R = 0x22
};

enum RF90_RADIO_PATH {
	RF90_PATH_A = 0,		/* Radio Path A */
	RF90_PATH_B = 1,		/* Radio Path B */
	RF90_PATH_C = 2,		/* Radio Path C */
	RF90_PATH_D = 3			/* Radio Path D */
};

=======
>>>>>>> upstream/android-13
/*  Bandwidth Offset */
#define HAL_PRIME_CHNL_OFFSET_DONT_CARE	0
#define HAL_PRIME_CHNL_OFFSET_LOWER	1
#define HAL_PRIME_CHNL_OFFSET_UPPER	2

/*  Represent Channel Width in HT Capabilities */
<<<<<<< HEAD
enum CHANNEL_WIDTH {
	CHANNEL_WIDTH_20 = 0,
	CHANNEL_WIDTH_40 = 1,
	CHANNEL_WIDTH_80 = 2,
	CHANNEL_WIDTH_160 = 3,
	CHANNEL_WIDTH_80_80 = 4,
	CHANNEL_WIDTH_MAX = 5,
=======
enum channel_width {
	CHANNEL_WIDTH_20 = 0,
	CHANNEL_WIDTH_40 = 1,
>>>>>>> upstream/android-13
};

/*  Represent Extension Channel Offset in HT Capabilities */
/*  This is available only in 40Mhz mode. */
<<<<<<< HEAD
enum EXTCHNL_OFFSET {
=======
enum extchnl_offset {
>>>>>>> upstream/android-13
	EXTCHNL_OFFSET_NO_EXT = 0,
	EXTCHNL_OFFSET_UPPER = 1,
	EXTCHNL_OFFSET_NO_DEF = 2,
	EXTCHNL_OFFSET_LOWER = 3,
};

<<<<<<< HEAD
enum VHT_DATA_SC {
	VHT_DATA_SC_DONOT_CARE = 0,
	VHT_DATA_SC_20_UPPER_OF_80MHZ = 1,
	VHT_DATA_SC_20_LOWER_OF_80MHZ = 2,
	VHT_DATA_SC_20_UPPERST_OF_80MHZ = 3,
	VHT_DATA_SC_20_LOWEST_OF_80MHZ = 4,
	VHT_DATA_SC_20_RECV1 = 5,
	VHT_DATA_SC_20_RECV2 = 6,
	VHT_DATA_SC_20_RECV3 = 7,
	VHT_DATA_SC_20_RECV4 = 8,
	VHT_DATA_SC_40_UPPER_OF_80MHZ = 9,
	VHT_DATA_SC_40_LOWER_OF_80MHZ = 10,
};

enum PROTECTION_MODE {
	PROTECTION_MODE_AUTO = 0,
	PROTECTION_MODE_FORCE_ENABLE = 1,
	PROTECTION_MODE_FORCE_DISABLE = 2,
};

/* 2007/11/15 MH Define different RF type. */
enum RT_RF_TYPE_DEFINITION {
	RF_1T2R = 0,
	RF_2T4R = 1,
	RF_2T2R = 2,
	RF_1T1R = 3,
	RF_2T2R_GREEN = 4,
	RF_MAX_TYPE = 5,
=======
enum {
	HT_DATA_SC_DONOT_CARE = 0,
	HT_DATA_SC_20_UPPER_OF_40MHZ = 1,
	HT_DATA_SC_20_LOWER_OF_40MHZ = 2,
>>>>>>> upstream/android-13
};

u32 rtw_ch2freq(u32 ch);

#endif /* _RTL8711_RF_H_ */
