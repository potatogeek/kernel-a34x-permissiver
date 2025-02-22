// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/

/* include "Mp_Precomp.h" */
#include "odm_precomp.h"

<<<<<<< HEAD

#define CALCULATE_SWINGTALBE_OFFSET(_offset, _direction, _size, _deltaThermal) \
	do {\
		for (_offset = 0; _offset < _size; _offset++) {\
			if (_deltaThermal < thermalThreshold[_direction][_offset]) {\
				if (_offset != 0)\
					_offset--;\
				break;\
			} \
		} \
		if (_offset >= _size)\
			_offset = _size-1;\
	} while (0)


void ConfigureTxpowerTrack(PDM_ODM_T pDM_Odm, PTXPWRTRACK_CFG pConfig)
=======
void ConfigureTxpowerTrack(struct dm_odm_t *pDM_Odm, struct txpwrtrack_cfg *pConfig)
>>>>>>> upstream/android-13
{
	ConfigureTxpowerTrack_8723B(pConfig);
}

/*  */
/*  <20121113, Kordan> This function should be called when TxAGC changed. */
/*  Otherwise the previous compensation is gone, because we record the */
/*  delta of temperature between two TxPowerTracking watch dogs. */
/*  */
/*  NOTE: If Tx BB swing or Tx scaling is varified during run-time, still */
/*        need to call this function. */
/*  */
<<<<<<< HEAD
void ODM_ClearTxPowerTrackingState(PDM_ODM_T pDM_Odm)
=======
void ODM_ClearTxPowerTrackingState(struct dm_odm_t *pDM_Odm)
>>>>>>> upstream/android-13
{
	struct hal_com_data *pHalData = GET_HAL_DATA(pDM_Odm->Adapter);
	u8 p = 0;

	pDM_Odm->BbSwingIdxCckBase = pDM_Odm->DefaultCckIndex;
	pDM_Odm->BbSwingIdxCck = pDM_Odm->DefaultCckIndex;
	pDM_Odm->RFCalibrateInfo.CCK_index = 0;

<<<<<<< HEAD
	for (p = ODM_RF_PATH_A; p < MAX_RF_PATH; ++p) {
=======
	for (p = RF_PATH_A; p < MAX_RF_PATH; ++p) {
>>>>>>> upstream/android-13
		pDM_Odm->BbSwingIdxOfdmBase[p] = pDM_Odm->DefaultOfdmIndex;
		pDM_Odm->BbSwingIdxOfdm[p] = pDM_Odm->DefaultOfdmIndex;
		pDM_Odm->RFCalibrateInfo.OFDM_index[p] = pDM_Odm->DefaultOfdmIndex;

		pDM_Odm->RFCalibrateInfo.PowerIndexOffset[p] = 0;
		pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[p] = 0;
		pDM_Odm->RFCalibrateInfo.DeltaPowerIndexLast[p] = 0;
		pDM_Odm->RFCalibrateInfo.PowerIndexOffset[p] = 0;

		/*  Initial Mix mode power tracking */
		pDM_Odm->Absolute_OFDMSwingIdx[p] = 0;
		pDM_Odm->Remnant_OFDMSwingIdx[p] = 0;
	}

	/* Initial at Modify Tx Scaling Mode */
	pDM_Odm->Modify_TxAGC_Flag_PathA = false;
	/* Initial at Modify Tx Scaling Mode */
	pDM_Odm->Modify_TxAGC_Flag_PathB = false;
	pDM_Odm->Remnant_CCKSwingIdx = 0;
	pDM_Odm->RFCalibrateInfo.ThermalValue = pHalData->EEPROMThermalMeter;
	pDM_Odm->RFCalibrateInfo.ThermalValue_IQK = pHalData->EEPROMThermalMeter;
	pDM_Odm->RFCalibrateInfo.ThermalValue_LCK = pHalData->EEPROMThermalMeter;
}

void ODM_TXPowerTrackingCallback_ThermalMeter(struct adapter *Adapter)
{

	struct hal_com_data *pHalData = GET_HAL_DATA(Adapter);
<<<<<<< HEAD
	PDM_ODM_T pDM_Odm = &pHalData->odmpriv;

	u8 ThermalValue = 0, delta, delta_LCK, delta_IQK, p = 0, i = 0;
=======
	struct dm_odm_t *pDM_Odm = &pHalData->odmpriv;

	u8 ThermalValue = 0, delta, delta_LCK, p = 0, i = 0;
>>>>>>> upstream/android-13
	u8 ThermalValue_AVG_count = 0;
	u32 ThermalValue_AVG = 0;

	u8 OFDM_min_index = 0;  /*  OFDM BB Swing should be less than +3.0dB, which is required by Arthur */
	u8 Indexforchannel = 0; /*  GetRightChnlPlaceforIQK(pHalData->CurrentChannel) */

<<<<<<< HEAD
	TXPWRTRACK_CFG c;
=======
	struct txpwrtrack_cfg c;
>>>>>>> upstream/android-13


	/* 4 1. The following TWO tables decide the final index of OFDM/CCK swing table. */
	u8 *deltaSwingTableIdx_TUP_A;
	u8 *deltaSwingTableIdx_TDOWN_A;
	u8 *deltaSwingTableIdx_TUP_B;
	u8 *deltaSwingTableIdx_TDOWN_B;

<<<<<<< HEAD
	/* 4 2. Initilization (7 steps in total) */
=======
	/* 4 2. Initialization (7 steps in total) */
>>>>>>> upstream/android-13

	ConfigureTxpowerTrack(pDM_Odm, &c);

	(*c.GetDeltaSwingTable)(
		pDM_Odm,
		(u8 **)&deltaSwingTableIdx_TUP_A,
		(u8 **)&deltaSwingTableIdx_TDOWN_A,
		(u8 **)&deltaSwingTableIdx_TUP_B,
		(u8 **)&deltaSwingTableIdx_TDOWN_B
	);

	/* cosa add for debug */
	pDM_Odm->RFCalibrateInfo.TXPowerTrackingCallbackCnt++;
	pDM_Odm->RFCalibrateInfo.bTXPowerTrackingInit = true;

<<<<<<< HEAD
	ODM_RT_TRACE(
		pDM_Odm,
		ODM_COMP_TX_PWR_TRACK,
		ODM_DBG_LOUD,
		(
			"===>ODM_TXPowerTrackingCallback_ThermalMeter,\npDM_Odm->BbSwingIdxCckBase: %d, pDM_Odm->BbSwingIdxOfdmBase[A]: %d, pDM_Odm->DefaultOfdmIndex: %d\n",
			pDM_Odm->BbSwingIdxCckBase,
			pDM_Odm->BbSwingIdxOfdmBase[ODM_RF_PATH_A],
			pDM_Odm->DefaultOfdmIndex
		)
	);

	ThermalValue = (u8)PHY_QueryRFReg(pDM_Odm->Adapter, ODM_RF_PATH_A, c.ThermalRegAddr, 0xfc00);	/* 0x42: RF Reg[15:10] 88E */
=======
	ThermalValue = (u8)PHY_QueryRFReg(pDM_Odm->Adapter, RF_PATH_A, c.ThermalRegAddr, 0xfc00);	/* 0x42: RF Reg[15:10] 88E */
>>>>>>> upstream/android-13
	if (
		!pDM_Odm->RFCalibrateInfo.TxPowerTrackControl ||
		pHalData->EEPROMThermalMeter == 0 ||
		pHalData->EEPROMThermalMeter == 0xFF
	)
		return;

	/* 4 3. Initialize ThermalValues of RFCalibrateInfo */

<<<<<<< HEAD
	if (pDM_Odm->RFCalibrateInfo.bReloadtxpowerindex)
		ODM_RT_TRACE(
			pDM_Odm,
			ODM_COMP_TX_PWR_TRACK, ODM_DBG_LOUD,
			("reload ofdm index for band switch\n")
		);

=======
>>>>>>> upstream/android-13
	/* 4 4. Calculate average thermal meter */

	pDM_Odm->RFCalibrateInfo.ThermalValue_AVG[pDM_Odm->RFCalibrateInfo.ThermalValue_AVG_index] = ThermalValue;
	pDM_Odm->RFCalibrateInfo.ThermalValue_AVG_index++;
	if (pDM_Odm->RFCalibrateInfo.ThermalValue_AVG_index == c.AverageThermalNum)   /* Average times =  c.AverageThermalNum */
		pDM_Odm->RFCalibrateInfo.ThermalValue_AVG_index = 0;

	for (i = 0; i < c.AverageThermalNum; i++) {
		if (pDM_Odm->RFCalibrateInfo.ThermalValue_AVG[i]) {
			ThermalValue_AVG += pDM_Odm->RFCalibrateInfo.ThermalValue_AVG[i];
			ThermalValue_AVG_count++;
		}
	}

	/* Calculate Average ThermalValue after average enough times */
	if (ThermalValue_AVG_count) {
		ThermalValue = (u8)(ThermalValue_AVG / ThermalValue_AVG_count);
<<<<<<< HEAD
		ODM_RT_TRACE(
			pDM_Odm,
			ODM_COMP_TX_PWR_TRACK,
			ODM_DBG_LOUD,
			(
				"AVG Thermal Meter = 0x%X, EFUSE Thermal Base = 0x%X\n",
				ThermalValue,
				pHalData->EEPROMThermalMeter
			)
		);
	}

	/* 4 5. Calculate delta, delta_LCK, delta_IQK. */
=======
	}

	/* 4 5. Calculate delta, delta_LCK */
>>>>>>> upstream/android-13
	/* delta" here is used to determine whether thermal value changes or not. */
	delta =
		(ThermalValue > pDM_Odm->RFCalibrateInfo.ThermalValue) ?
		(ThermalValue - pDM_Odm->RFCalibrateInfo.ThermalValue) :
		(pDM_Odm->RFCalibrateInfo.ThermalValue - ThermalValue);
	delta_LCK =
		(ThermalValue > pDM_Odm->RFCalibrateInfo.ThermalValue_LCK) ?
		(ThermalValue - pDM_Odm->RFCalibrateInfo.ThermalValue_LCK) :
		(pDM_Odm->RFCalibrateInfo.ThermalValue_LCK - ThermalValue);
<<<<<<< HEAD
	delta_IQK =
		(ThermalValue > pDM_Odm->RFCalibrateInfo.ThermalValue_IQK) ?
		(ThermalValue - pDM_Odm->RFCalibrateInfo.ThermalValue_IQK) :
		(pDM_Odm->RFCalibrateInfo.ThermalValue_IQK - ThermalValue);

	ODM_RT_TRACE(
		pDM_Odm,
		ODM_COMP_TX_PWR_TRACK,
		ODM_DBG_LOUD,
		(
			"(delta, delta_LCK, delta_IQK) = (%d, %d, %d)\n",
			delta,
			delta_LCK,
			delta_IQK
		)
	);
=======
>>>>>>> upstream/android-13

	/* 4 6. If necessary, do LCK. */
	/*  Delta temperature is equal to or larger than 20 centigrade. */
	if (delta_LCK >= c.Threshold_IQK) {
<<<<<<< HEAD
		ODM_RT_TRACE(
			pDM_Odm,
			ODM_COMP_TX_PWR_TRACK,
			ODM_DBG_LOUD,
			(
				"delta_LCK(%d) >= Threshold_IQK(%d)\n",
				delta_LCK,
				c.Threshold_IQK
			)
		);
=======
>>>>>>> upstream/android-13
		pDM_Odm->RFCalibrateInfo.ThermalValue_LCK = ThermalValue;
		if (c.PHY_LCCalibrate)
			(*c.PHY_LCCalibrate)(pDM_Odm);
	}

	/* 3 7. If necessary, move the index of swing table to adjust Tx power. */
	if (delta > 0 && pDM_Odm->RFCalibrateInfo.TxPowerTrackControl) {
<<<<<<< HEAD
		/* delta" here is used to record the absolute value of differrence. */
=======
		/* delta" here is used to record the absolute value of difference. */
>>>>>>> upstream/android-13
		delta =
			ThermalValue > pHalData->EEPROMThermalMeter ?
			(ThermalValue - pHalData->EEPROMThermalMeter) :
			(pHalData->EEPROMThermalMeter - ThermalValue);

		if (delta >= TXPWR_TRACK_TABLE_SIZE)
			delta = TXPWR_TRACK_TABLE_SIZE - 1;

		/* 4 7.1 The Final Power Index = BaseIndex + PowerIndexOffset */
		if (ThermalValue > pHalData->EEPROMThermalMeter) {
<<<<<<< HEAD
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				(
					"deltaSwingTableIdx_TUP_A[%d] = %d\n",
					delta,
					deltaSwingTableIdx_TUP_A[delta]
				)
			);
			pDM_Odm->RFCalibrateInfo.DeltaPowerIndexLast[ODM_RF_PATH_A] =
				pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[ODM_RF_PATH_A];
			pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[ODM_RF_PATH_A] =
				deltaSwingTableIdx_TUP_A[delta];

			/*  Record delta swing for mix mode power tracking */
			pDM_Odm->Absolute_OFDMSwingIdx[ODM_RF_PATH_A] =
				deltaSwingTableIdx_TUP_A[delta];

			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				(
					"******Temp is higher and pDM_Odm->Absolute_OFDMSwingIdx[ODM_RF_PATH_A] = %d\n",
					pDM_Odm->Absolute_OFDMSwingIdx[ODM_RF_PATH_A]
				)
			);

			if (c.RfPathCount > 1) {
				ODM_RT_TRACE(
					pDM_Odm,
					ODM_COMP_TX_PWR_TRACK,
					ODM_DBG_LOUD,
					(
						"deltaSwingTableIdx_TUP_B[%d] = %d\n",
						delta,
						deltaSwingTableIdx_TUP_B[delta]
					)
				);
				pDM_Odm->RFCalibrateInfo.DeltaPowerIndexLast[ODM_RF_PATH_B] =
					pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[ODM_RF_PATH_B];
				pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[ODM_RF_PATH_B] =
					deltaSwingTableIdx_TUP_B[delta];

				/*  Record delta swing for mix mode power tracking */
				pDM_Odm->Absolute_OFDMSwingIdx[ODM_RF_PATH_B] =
					deltaSwingTableIdx_TUP_B[delta];
				ODM_RT_TRACE(
					pDM_Odm,
					ODM_COMP_TX_PWR_TRACK,
					ODM_DBG_LOUD,
					(
						"******Temp is higher and pDM_Odm->Absolute_OFDMSwingIdx[ODM_RF_PATH_B] = %d\n",
						pDM_Odm->Absolute_OFDMSwingIdx[ODM_RF_PATH_B]
					)
				);
			}

		} else {
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				(
					"deltaSwingTableIdx_TDOWN_A[%d] = %d\n",
					delta,
					deltaSwingTableIdx_TDOWN_A[delta]
				)
			);

			pDM_Odm->RFCalibrateInfo.DeltaPowerIndexLast[ODM_RF_PATH_A] =
				pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[ODM_RF_PATH_A];
			pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[ODM_RF_PATH_A] =
				-1 * deltaSwingTableIdx_TDOWN_A[delta];

			/*  Record delta swing for mix mode power tracking */
			pDM_Odm->Absolute_OFDMSwingIdx[ODM_RF_PATH_A] =
				-1 * deltaSwingTableIdx_TDOWN_A[delta];

			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				(
					"******Temp is lower and pDM_Odm->Absolute_OFDMSwingIdx[ODM_RF_PATH_A] = %d\n",
					pDM_Odm->Absolute_OFDMSwingIdx[ODM_RF_PATH_A]
				)
			);

			if (c.RfPathCount > 1) {
				ODM_RT_TRACE(
					pDM_Odm,
					ODM_COMP_TX_PWR_TRACK,
					ODM_DBG_LOUD,
					(
						"deltaSwingTableIdx_TDOWN_B[%d] = %d\n",
						delta,
						deltaSwingTableIdx_TDOWN_B[delta]
					)
				);

				pDM_Odm->RFCalibrateInfo.DeltaPowerIndexLast[ODM_RF_PATH_B] =
					pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[ODM_RF_PATH_B];
				pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[ODM_RF_PATH_B] =
					-1 * deltaSwingTableIdx_TDOWN_B[delta];

				 /*  Record delta swing for mix mode power tracking */
				pDM_Odm->Absolute_OFDMSwingIdx[ODM_RF_PATH_B] =
					-1 * deltaSwingTableIdx_TDOWN_B[delta];

				ODM_RT_TRACE(
					pDM_Odm,
					ODM_COMP_TX_PWR_TRACK,
					ODM_DBG_LOUD,
					(
						"******Temp is lower and pDM_Odm->Absolute_OFDMSwingIdx[ODM_RF_PATH_B] = %d\n",
						pDM_Odm->Absolute_OFDMSwingIdx[ODM_RF_PATH_B]
					)
				);
			}
		}

		for (p = ODM_RF_PATH_A; p < c.RfPathCount; p++) {
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				(
					"\n\n ================================ [Path-%c] Calculating PowerIndexOffset ================================\n",
					(p == ODM_RF_PATH_A ? 'A' : 'B')
				)
			);

=======
			pDM_Odm->RFCalibrateInfo.DeltaPowerIndexLast[RF_PATH_A] =
				pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[RF_PATH_A];
			pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[RF_PATH_A] =
				deltaSwingTableIdx_TUP_A[delta];

			/*  Record delta swing for mix mode power tracking */
			pDM_Odm->Absolute_OFDMSwingIdx[RF_PATH_A] =
				deltaSwingTableIdx_TUP_A[delta];

			if (c.RfPathCount > 1) {
				pDM_Odm->RFCalibrateInfo.DeltaPowerIndexLast[RF_PATH_B] =
					pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[RF_PATH_B];
				pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[RF_PATH_B] =
					deltaSwingTableIdx_TUP_B[delta];

				/*  Record delta swing for mix mode power tracking */
				pDM_Odm->Absolute_OFDMSwingIdx[RF_PATH_B] =
					deltaSwingTableIdx_TUP_B[delta];
			}

		} else {
			pDM_Odm->RFCalibrateInfo.DeltaPowerIndexLast[RF_PATH_A] =
				pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[RF_PATH_A];
			pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[RF_PATH_A] =
				-1 * deltaSwingTableIdx_TDOWN_A[delta];

			/*  Record delta swing for mix mode power tracking */
			pDM_Odm->Absolute_OFDMSwingIdx[RF_PATH_A] =
				-1 * deltaSwingTableIdx_TDOWN_A[delta];

			if (c.RfPathCount > 1) {
				pDM_Odm->RFCalibrateInfo.DeltaPowerIndexLast[RF_PATH_B] =
					pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[RF_PATH_B];
				pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[RF_PATH_B] =
					-1 * deltaSwingTableIdx_TDOWN_B[delta];

				 /*  Record delta swing for mix mode power tracking */
				pDM_Odm->Absolute_OFDMSwingIdx[RF_PATH_B] =
					-1 * deltaSwingTableIdx_TDOWN_B[delta];
			}
		}

		for (p = RF_PATH_A; p < c.RfPathCount; p++) {
>>>>>>> upstream/android-13
			if (
				pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[p] ==
				pDM_Odm->RFCalibrateInfo.DeltaPowerIndexLast[p]
			) /*  If Thermal value changes but lookup table value still the same */
				pDM_Odm->RFCalibrateInfo.PowerIndexOffset[p] = 0;
			else
				pDM_Odm->RFCalibrateInfo.PowerIndexOffset[p] = pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[p] - pDM_Odm->RFCalibrateInfo.DeltaPowerIndexLast[p];      /*  Power Index Diff between 2 times Power Tracking */

<<<<<<< HEAD
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				(
					"[Path-%c] PowerIndexOffset(%d) = DeltaPowerIndex(%d) - DeltaPowerIndexLast(%d)\n",
					(
						p == ODM_RF_PATH_A ? 'A' : 'B'),
						pDM_Odm->RFCalibrateInfo.PowerIndexOffset[p],
						pDM_Odm->RFCalibrateInfo.DeltaPowerIndex[p],
						pDM_Odm->RFCalibrateInfo.DeltaPowerIndexLast[p]
					)
				);

=======
>>>>>>> upstream/android-13
			pDM_Odm->RFCalibrateInfo.OFDM_index[p] =
				pDM_Odm->BbSwingIdxOfdmBase[p] +
				pDM_Odm->RFCalibrateInfo.PowerIndexOffset[p];

			pDM_Odm->RFCalibrateInfo.CCK_index =
				pDM_Odm->BbSwingIdxCckBase +
				pDM_Odm->RFCalibrateInfo.PowerIndexOffset[p];

			pDM_Odm->BbSwingIdxCck =
				pDM_Odm->RFCalibrateInfo.CCK_index;

			pDM_Odm->BbSwingIdxOfdm[p] =
				pDM_Odm->RFCalibrateInfo.OFDM_index[p];

<<<<<<< HEAD
			/*  *************Print BB Swing Base and Index Offset************* */
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				(
					"The 'CCK' final index(%d) = BaseIndex(%d) + PowerIndexOffset(%d)\n",
					pDM_Odm->BbSwingIdxCck,
					pDM_Odm->BbSwingIdxCckBase,
					pDM_Odm->RFCalibrateInfo.PowerIndexOffset[p]
				)
			);
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				(
					"The 'OFDM' final index(%d) = BaseIndex[%c](%d) + PowerIndexOffset(%d)\n",
					pDM_Odm->BbSwingIdxOfdm[p],
					(p == ODM_RF_PATH_A ? 'A' : 'B'),
					pDM_Odm->BbSwingIdxOfdmBase[p],
					pDM_Odm->RFCalibrateInfo.PowerIndexOffset[p]
				)
			);

=======
>>>>>>> upstream/android-13
			/* 4 7.1 Handle boundary conditions of index. */
			if (pDM_Odm->RFCalibrateInfo.OFDM_index[p] > c.SwingTableSize_OFDM-1)
				pDM_Odm->RFCalibrateInfo.OFDM_index[p] = c.SwingTableSize_OFDM-1;
			else if (pDM_Odm->RFCalibrateInfo.OFDM_index[p] < OFDM_min_index)
				pDM_Odm->RFCalibrateInfo.OFDM_index[p] = OFDM_min_index;
		}
<<<<<<< HEAD
		ODM_RT_TRACE(
			pDM_Odm,
			ODM_COMP_TX_PWR_TRACK,
			ODM_DBG_LOUD,
			("\n\n ========================================================================================================\n")
		);
=======
>>>>>>> upstream/android-13
		if (pDM_Odm->RFCalibrateInfo.CCK_index > c.SwingTableSize_CCK-1)
			pDM_Odm->RFCalibrateInfo.CCK_index = c.SwingTableSize_CCK-1;
		/* else if (pDM_Odm->RFCalibrateInfo.CCK_index < 0) */
			/* pDM_Odm->RFCalibrateInfo.CCK_index = 0; */
	} else {
<<<<<<< HEAD
		ODM_RT_TRACE(
			pDM_Odm,
			ODM_COMP_TX_PWR_TRACK,
			ODM_DBG_LOUD,
			(
				"The thermal meter is unchanged or TxPowerTracking OFF(%d): ThermalValue: %d , pDM_Odm->RFCalibrateInfo.ThermalValue: %d\n",
				pDM_Odm->RFCalibrateInfo.TxPowerTrackControl,
				ThermalValue,
				pDM_Odm->RFCalibrateInfo.ThermalValue
			)
		);

			for (p = ODM_RF_PATH_A; p < c.RfPathCount; p++)
				pDM_Odm->RFCalibrateInfo.PowerIndexOffset[p] = 0;
	}
	ODM_RT_TRACE(
		pDM_Odm,
		ODM_COMP_TX_PWR_TRACK,
		ODM_DBG_LOUD,
		(
			"TxPowerTracking: [CCK] Swing Current Index: %d, Swing Base Index: %d\n",
			pDM_Odm->RFCalibrateInfo.CCK_index,
			pDM_Odm->BbSwingIdxCckBase
		)
	);

	/* Print Swing base & current */
	for (p = ODM_RF_PATH_A; p < c.RfPathCount; p++) {
		ODM_RT_TRACE(
			pDM_Odm,
			ODM_COMP_TX_PWR_TRACK,
			ODM_DBG_LOUD,
			(
				"TxPowerTracking: [OFDM] Swing Current Index: %d, Swing Base Index[%c]: %d\n",
				pDM_Odm->RFCalibrateInfo.OFDM_index[p],
				(p == ODM_RF_PATH_A ? 'A' : 'B'),
				pDM_Odm->BbSwingIdxOfdmBase[p]
			)
		);
	}

	if (
		(pDM_Odm->RFCalibrateInfo.PowerIndexOffset[ODM_RF_PATH_A] != 0 ||
		 pDM_Odm->RFCalibrateInfo.PowerIndexOffset[ODM_RF_PATH_B] != 0) &&
=======
			for (p = RF_PATH_A; p < c.RfPathCount; p++)
				pDM_Odm->RFCalibrateInfo.PowerIndexOffset[p] = 0;
	}

	/* Print Swing base & current */
	for (p = RF_PATH_A; p < c.RfPathCount; p++) {
	}

	if (
		(pDM_Odm->RFCalibrateInfo.PowerIndexOffset[RF_PATH_A] != 0 ||
		 pDM_Odm->RFCalibrateInfo.PowerIndexOffset[RF_PATH_B] != 0) &&
>>>>>>> upstream/android-13
		 pDM_Odm->RFCalibrateInfo.TxPowerTrackControl
	 ) {
		/* 4 7.2 Configure the Swing Table to adjust Tx Power. */

		pDM_Odm->RFCalibrateInfo.bTxPowerChanged = true; /*  Always true after Tx Power is adjusted by power tracking. */
		/*  */
		/*  2012/04/23 MH According to Luke's suggestion, we can not write BB digital */
		/*  to increase TX power. Otherwise, EVM will be bad. */
		/*  */
		/*  2012/04/25 MH Add for tx power tracking to set tx power in tx agc for 88E. */
<<<<<<< HEAD
		if (ThermalValue > pDM_Odm->RFCalibrateInfo.ThermalValue) {
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				(
					"Temperature Increasing(A): delta_pi: %d , delta_t: %d, Now_t: %d, EFUSE_t: %d, Last_t: %d\n",
					pDM_Odm->RFCalibrateInfo.PowerIndexOffset[ODM_RF_PATH_A],
					delta,
					ThermalValue,
					pHalData->EEPROMThermalMeter,
					pDM_Odm->RFCalibrateInfo.ThermalValue
				)
			);

			if (c.RfPathCount > 1)
				ODM_RT_TRACE(
					pDM_Odm,
					ODM_COMP_TX_PWR_TRACK,
					ODM_DBG_LOUD,
					(
						"Temperature Increasing(B): delta_pi: %d , delta_t: %d, Now_t: %d, EFUSE_t: %d, Last_t: %d\n",
						pDM_Odm->RFCalibrateInfo.PowerIndexOffset[ODM_RF_PATH_B],
						delta,
						ThermalValue,
						pHalData->EEPROMThermalMeter,
						pDM_Odm->RFCalibrateInfo.ThermalValue
					)
				);

		} else if (ThermalValue < pDM_Odm->RFCalibrateInfo.ThermalValue) { /*  Low temperature */
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				(
					"Temperature Decreasing(A): delta_pi: %d , delta_t: %d, Now_t: %d, EFUSE_t: %d, Last_t: %d\n",
					pDM_Odm->RFCalibrateInfo.PowerIndexOffset[ODM_RF_PATH_A],
					delta,
					ThermalValue,
					pHalData->EEPROMThermalMeter,
					pDM_Odm->RFCalibrateInfo.ThermalValue
				)
			);

			if (c.RfPathCount > 1)
				ODM_RT_TRACE(
					pDM_Odm,
					ODM_COMP_TX_PWR_TRACK,
					ODM_DBG_LOUD,
					(
						"Temperature Decreasing(B): delta_pi: %d , delta_t: %d, Now_t: %d, EFUSE_t: %d, Last_t: %d\n",
						pDM_Odm->RFCalibrateInfo.PowerIndexOffset[ODM_RF_PATH_B],
						delta,
						ThermalValue,
						pHalData->EEPROMThermalMeter,
						pDM_Odm->RFCalibrateInfo.ThermalValue
					)
				);

		}

		if (ThermalValue > pHalData->EEPROMThermalMeter) {
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				(
					"Temperature(%d) higher than PG value(%d)\n",
					ThermalValue,
					pHalData->EEPROMThermalMeter
				)
			);

			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				("**********Enter POWER Tracking MIX_MODE**********\n")
			);
			for (p = ODM_RF_PATH_A; p < c.RfPathCount; p++)
					(*c.ODM_TxPwrTrackSetPwr)(pDM_Odm, MIX_MODE, p, 0);
		} else {
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				(
					"Temperature(%d) lower than PG value(%d)\n",
					ThermalValue,
					pHalData->EEPROMThermalMeter
				)
			);

			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_TX_PWR_TRACK,
				ODM_DBG_LOUD,
				("**********Enter POWER Tracking MIX_MODE**********\n")
			);
			for (p = ODM_RF_PATH_A; p < c.RfPathCount; p++)
=======

		if (ThermalValue > pHalData->EEPROMThermalMeter) {
			for (p = RF_PATH_A; p < c.RfPathCount; p++)
					(*c.ODM_TxPwrTrackSetPwr)(pDM_Odm, MIX_MODE, p, 0);
		} else {
			for (p = RF_PATH_A; p < c.RfPathCount; p++)
>>>>>>> upstream/android-13
				(*c.ODM_TxPwrTrackSetPwr)(pDM_Odm, MIX_MODE, p, Indexforchannel);
		}

		/*  Record last time Power Tracking result as base. */
		pDM_Odm->BbSwingIdxCckBase = pDM_Odm->BbSwingIdxCck;
<<<<<<< HEAD
		for (p = ODM_RF_PATH_A; p < c.RfPathCount; p++)
			pDM_Odm->BbSwingIdxOfdmBase[p] = pDM_Odm->BbSwingIdxOfdm[p];

		ODM_RT_TRACE(
			pDM_Odm,
			ODM_COMP_TX_PWR_TRACK, ODM_DBG_LOUD,
			(
				"pDM_Odm->RFCalibrateInfo.ThermalValue = %d ThermalValue = %d\n",
				pDM_Odm->RFCalibrateInfo.ThermalValue,
				ThermalValue
			)
		);

=======
		for (p = RF_PATH_A; p < c.RfPathCount; p++)
			pDM_Odm->BbSwingIdxOfdmBase[p] = pDM_Odm->BbSwingIdxOfdm[p];

>>>>>>> upstream/android-13
		/* Record last Power Tracking Thermal Value */
		pDM_Odm->RFCalibrateInfo.ThermalValue = ThermalValue;
	}

<<<<<<< HEAD
	ODM_RT_TRACE(
		pDM_Odm,
		ODM_COMP_TX_PWR_TRACK,
		ODM_DBG_LOUD,
		("<===ODM_TXPowerTrackingCallback_ThermalMeter\n")
	);

	pDM_Odm->RFCalibrateInfo.TXPowercount = 0;
}




/* 3 ============================================================ */
/* 3 IQ Calibration */
/* 3 ============================================================ */

u8 ODM_GetRightChnlPlaceforIQK(u8 chnl)
{
	u8 channel_all[ODM_TARGET_CHNL_NUM_2G_5G] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
		36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58,
		60, 62, 64, 100, 102, 104, 106, 108, 110, 112,
		114, 116, 118, 120, 122, 124, 126, 128, 130, 132,
		134, 136, 138, 140, 149, 151, 153, 155, 157, 159,
		161, 163, 165
	};
	u8 place = chnl;


	if (chnl > 14) {
		for (place = 14; place < sizeof(channel_all); place++) {
			if (channel_all[place] == chnl)
				return place-13;
		}
	}
	return 0;

}
=======
	pDM_Odm->RFCalibrateInfo.TXPowercount = 0;
}
>>>>>>> upstream/android-13
