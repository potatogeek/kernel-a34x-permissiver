/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/

 #ifndef __HAL_PHY_RF_H__
 #define __HAL_PHY_RF_H__

<<<<<<< HEAD
typedef enum _SPUR_CAL_METHOD {
	PLL_RESET,
	AFE_PHASE_SEL
} SPUR_CAL_METHOD;

typedef enum _PWRTRACK_CONTROL_METHOD {
	BBSWING,
	TXAGC,
	MIX_MODE
} PWRTRACK_METHOD;

typedef void (*FuncSetPwr)(PDM_ODM_T, PWRTRACK_METHOD, u8, u8);
typedef void (*FuncIQK)(PDM_ODM_T, u8, u8, u8);
typedef void (*FuncLCK)(PDM_ODM_T);
typedef void (*FuncSwing)(PDM_ODM_T, u8 **, u8 **, u8 **, u8 **);

typedef struct _TXPWRTRACK_CFG {
=======
enum pwrtrack_method {
	BBSWING,
	TXAGC,
	MIX_MODE
};

typedef void (*FuncSetPwr)(struct dm_odm_t *, enum pwrtrack_method, u8, u8);
typedef void (*FuncIQK)(struct dm_odm_t *, u8, u8, u8);
typedef void (*FuncLCK)(struct dm_odm_t *);
typedef void (*FuncSwing)(struct dm_odm_t *, u8 **, u8 **, u8 **, u8 **);

struct txpwrtrack_cfg {
>>>>>>> upstream/android-13
	u8 SwingTableSize_CCK;
	u8 SwingTableSize_OFDM;
	u8 Threshold_IQK;
	u8 AverageThermalNum;
	u8 RfPathCount;
	u32 ThermalRegAddr;
	FuncSetPwr ODM_TxPwrTrackSetPwr;
	FuncIQK DoIQK;
	FuncLCK PHY_LCCalibrate;
	FuncSwing GetDeltaSwingTable;
<<<<<<< HEAD
} TXPWRTRACK_CFG, *PTXPWRTRACK_CFG;

void ConfigureTxpowerTrack(PDM_ODM_T pDM_Odm, PTXPWRTRACK_CFG pConfig);


void ODM_ClearTxPowerTrackingState(PDM_ODM_T pDM_Odm);

void ODM_TXPowerTrackingCallback_ThermalMeter(struct adapter *Adapter);



#define ODM_TARGET_CHNL_NUM_2G_5G 59


u8 ODM_GetRightChnlPlaceforIQK(u8 chnl);


=======
};

void ConfigureTxpowerTrack(struct dm_odm_t *pDM_Odm, struct txpwrtrack_cfg *pConfig);


void ODM_ClearTxPowerTrackingState(struct dm_odm_t *pDM_Odm);

void ODM_TXPowerTrackingCallback_ThermalMeter(struct adapter *Adapter);

>>>>>>> upstream/android-13
#endif	/*  #ifndef __HAL_PHY_RF_H__ */
