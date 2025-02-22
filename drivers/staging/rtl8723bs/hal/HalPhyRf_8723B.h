/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/

#ifndef __HAL_PHY_RF_8723B_H__
#define __HAL_PHY_RF_8723B_H__

/*--------------------------Define Parameters-------------------------------*/
#define	IQK_DELAY_TIME_8723B		20		/* ms */
#define IQK_DEFERRED_TIME_8723B		4
#define	index_mapping_NUM_8723B		15
#define AVG_THERMAL_NUM_8723B		4
#define	RF_T_METER_8723B					0x42	/*  */


<<<<<<< HEAD
void ConfigureTxpowerTrack_8723B(PTXPWRTRACK_CFG	pConfig);

void DoIQK_8723B(
	PDM_ODM_T pDM_Odm,
=======
void ConfigureTxpowerTrack_8723B(struct txpwrtrack_cfg *pConfig);

void DoIQK_8723B(
	struct dm_odm_t *pDM_Odm,
>>>>>>> upstream/android-13
	u8 DeltaThermalIndex,
	u8 ThermalValue,
	u8 Threshold
);

void ODM_TxPwrTrackSetPwr_8723B(
<<<<<<< HEAD
	PDM_ODM_T pDM_Odm,
	PWRTRACK_METHOD Method,
=======
	struct dm_odm_t *pDM_Odm,
	enum pwrtrack_method Method,
>>>>>>> upstream/android-13
	u8 RFPath,
	u8 ChannelMappedIndex
);

/* 1 7. IQK */
void PHY_IQCalibrate_8723B(
	struct adapter *Adapter,
	bool bReCovery,
	bool bRestore,
	bool Is2ant,
	u8 RF_Path
);

<<<<<<< HEAD
void ODM_SetIQCbyRFpath(PDM_ODM_T pDM_Odm, u32 RFpath);
=======
void ODM_SetIQCbyRFpath(struct dm_odm_t *pDM_Odm, u32 RFpath);
>>>>>>> upstream/android-13

/*  */
/*  LC calibrate */
/*  */
<<<<<<< HEAD
void PHY_LCCalibrate_8723B(PDM_ODM_T pDM_Odm);
=======
void PHY_LCCalibrate_8723B(struct dm_odm_t *pDM_Odm);
>>>>>>> upstream/android-13

/*  */
/*  AP calibrate */
/*  */
void PHY_DigitalPredistortion_8723B(struct adapter *padapter);


void _PHY_SaveADDARegisters_8723B(
	struct adapter *padapter,
	u32 *ADDAReg,
	u32 *ADDABackup,
	u32 RegisterNum
);

void _PHY_PathADDAOn_8723B(
	struct adapter *padapter,
	u32 *ADDAReg,
	bool isPathAOn,
	bool is2T
);

void _PHY_MACSettingCalibration_8723B(
	struct adapter *padapter, u32 *MACReg, u32 *MACBackup
);

#endif /*  #ifndef __HAL_PHY_RF_8188E_H__ */
