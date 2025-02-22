/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/

#ifndef __ODMDIG_H__
#define __ODMDIG_H__

<<<<<<< HEAD
typedef struct _Dynamic_Initial_Gain_Threshold_ {
=======
struct dig_t { /* _Dynamic_Initial_Gain_Threshold_ */
>>>>>>> upstream/android-13
	bool bStopDIG;
	bool bPSDInProgress;

	u8 Dig_Enable_Flag;
	u8 Dig_Ext_Port_Stage;

	int RssiLowThresh;
	int RssiHighThresh;

	u32 FALowThresh;
	u32 FAHighThresh;

	u8 CurSTAConnectState;
	u8 PreSTAConnectState;
	u8 CurMultiSTAConnectState;

	u8 PreIGValue;
	u8 CurIGValue;
	u8 BackupIGValue;		/* MP DIG */
	u8 BT30_CurIGI;
	u8 IGIBackup;

	s8 BackoffVal;
	s8 BackoffVal_range_max;
	s8 BackoffVal_range_min;
	u8 rx_gain_range_max;
	u8 rx_gain_range_min;
	u8 Rssi_val_min;

	u8 PreCCK_CCAThres;
	u8 CurCCK_CCAThres;
	u8 PreCCKPDState;
	u8 CurCCKPDState;
	u8 CCKPDBackup;

	u8 LargeFAHit;
	u8 ForbiddenIGI;
	u32 Recover_cnt;

	u8 DIG_Dynamic_MIN_0;
	u8 DIG_Dynamic_MIN_1;
	bool bMediaConnect_0;
	bool bMediaConnect_1;

	u32 AntDiv_RSSI_max;
	u32 RSSI_max;

	u8 *pbP2pLinkInProgress;
<<<<<<< HEAD
} DIG_T, *pDIG_T;

typedef struct false_ALARM_STATISTICS {
=======
};

struct  false_ALARM_STATISTICS {
>>>>>>> upstream/android-13
	u32 Cnt_Parity_Fail;
	u32 Cnt_Rate_Illegal;
	u32 Cnt_Crc8_fail;
	u32 Cnt_Mcs_fail;
	u32 Cnt_Ofdm_fail;
	u32 Cnt_Ofdm_fail_pre; /* For RTL8881A */
	u32 Cnt_Cck_fail;
	u32 Cnt_all;
	u32 Cnt_Fast_Fsync;
	u32 Cnt_SB_Search_fail;
	u32 Cnt_OFDM_CCA;
	u32 Cnt_CCK_CCA;
	u32 Cnt_CCA_all;
	u32 Cnt_BW_USC; /* Gary */
	u32 Cnt_BW_LSC; /* Gary */
<<<<<<< HEAD
} false_ALARM_STATISTICS, *Pfalse_ALARM_STATISTICS;

typedef enum tag_Dynamic_Init_Gain_Operation_Type_Definition {
	DIG_TYPE_THRESH_HIGH = 0,
	DIG_TYPE_THRESH_LOW = 1,
	DIG_TYPE_BACKOFF = 2,
	DIG_TYPE_RX_GAIN_MIN = 3,
	DIG_TYPE_RX_GAIN_MAX = 4,
	DIG_TYPE_ENABLE = 5,
	DIG_TYPE_DISABLE = 6,
	DIG_OP_TYPE_MAX
} DM_DIG_OP_E;

typedef enum tag_ODM_PauseDIG_Type {
	ODM_PAUSE_DIG = BIT0,
	ODM_RESUME_DIG = BIT1
} ODM_Pause_DIG_TYPE;

typedef enum tag_ODM_PauseCCKPD_Type {
	ODM_PAUSE_CCKPD = BIT0,
	ODM_RESUME_CCKPD = BIT1
} ODM_Pause_CCKPD_TYPE;
=======
};

enum ODM_Pause_DIG_TYPE {
	ODM_PAUSE_DIG = BIT0,
	ODM_RESUME_DIG = BIT1
};
>>>>>>> upstream/android-13

#define		DM_DIG_THRESH_HIGH			40
#define		DM_DIG_THRESH_LOW			35

#define		DMfalseALARM_THRESH_LOW	400
#define		DMfalseALARM_THRESH_HIGH	1000

#define		DM_DIG_MAX_NIC				0x3e
#define		DM_DIG_MIN_NIC				0x1e /* 0x22//0x1c */
#define		DM_DIG_MAX_OF_MIN_NIC		0x3e

#define		DM_DIG_MAX_AP					0x3e
#define		DM_DIG_MIN_AP					0x1c
#define		DM_DIG_MAX_OF_MIN			0x2A	/* 0x32 */
#define		DM_DIG_MIN_AP_DFS				0x20

#define		DM_DIG_MAX_NIC_HP			0x46
#define		DM_DIG_MIN_NIC_HP				0x2e

#define		DM_DIG_MAX_AP_HP				0x42
#define		DM_DIG_MIN_AP_HP				0x30

#define		DM_DIG_FA_TH0				0x200/* 0x20 */

#define		DM_DIG_FA_TH1					0x300
#define		DM_DIG_FA_TH2					0x400
/* this is for 92d */
#define		DM_DIG_FA_TH0_92D				0x100
#define		DM_DIG_FA_TH1_92D				0x400
#define		DM_DIG_FA_TH2_92D				0x600

#define		DM_DIG_BACKOFF_MAX			12
#define		DM_DIG_BACKOFF_MIN			-4
#define		DM_DIG_BACKOFF_DEFAULT		10

#define			DM_DIG_FA_TH0_LPS				4 /*  4 in lps */
#define			DM_DIG_FA_TH1_LPS				15 /*  15 lps */
#define			DM_DIG_FA_TH2_LPS				30 /*  30 lps */
#define			RSSI_OFFSET_DIG				0x05

void odm_NHMCounterStatisticsInit(void *pDM_VOID);

void odm_NHMCounterStatistics(void *pDM_VOID);

void odm_NHMBBInit(void *pDM_VOID);

void odm_NHMBB(void *pDM_VOID);

void odm_NHMCounterStatisticsReset(void *pDM_VOID);

void odm_GetNHMCounterStatistics(void *pDM_VOID);

void odm_SearchPwdBLowerBound(void *pDM_VOID, u8 IGI_target);

void odm_AdaptivityInit(void *pDM_VOID);

void odm_Adaptivity(void *pDM_VOID, u8 IGI);

void ODM_Write_DIG(void *pDM_VOID, u8 CurrentIGI);

<<<<<<< HEAD
void odm_PauseDIG(void *pDM_VOID, ODM_Pause_DIG_TYPE PauseType, u8 IGIValue);
=======
void odm_PauseDIG(void *pDM_VOID, enum ODM_Pause_DIG_TYPE PauseType, u8 IGIValue);
>>>>>>> upstream/android-13

void odm_DIGInit(void *pDM_VOID);

void odm_DIG(void *pDM_VOID);

void odm_DIGbyRSSI_LPS(void *pDM_VOID);

void odm_FalseAlarmCounterStatistics(void *pDM_VOID);

void odm_FAThresholdCheck(
	void *pDM_VOID,
	bool bDFSBand,
	bool bPerformance,
	u32 RxTp,
	u32 TxTp,
	u32 *dm_FA_thres
);

u8 odm_ForbiddenIGICheck(void *pDM_VOID, u8 DIG_Dynamic_MIN, u8 CurrentIGI);

bool odm_DigAbort(void *pDM_VOID);

void odm_CCKPacketDetectionThresh(void *pDM_VOID);

void ODM_Write_CCK_CCA_Thres(void *pDM_VOID, u8 CurCCK_CCAThres);

#endif
