// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
/******************************************************************************
 *
 *
 * Module:	rtl8192c_rf6052.c	(Source C File)
 *
 * Note:	Provide RF 6052 series relative API.
 *
 * Function:
 *
 * Export:
 *
 * Abbrev:
 *
 * History:
 * Data			Who		Remark
 *
 * 09/25/2008	MHC		Create initial version.
 * 11/05/2008	MHC		Add API for tw power setting.
 *
 *
******************************************************************************/

#include <rtl8723b_hal.h>

/*---------------------------Define Local Constant---------------------------*/
/*---------------------------Define Local Constant---------------------------*/


/*------------------------Define global variable-----------------------------*/
/*------------------------Define global variable-----------------------------*/


/*------------------------Define local variable------------------------------*/
/*  2008/11/20 MH For Debug only, RF */
/*------------------------Define local variable------------------------------*/

/*-----------------------------------------------------------------------------
 * Function:    PHY_RF6052SetBandwidth()
 *
 * Overview:    This function is called by SetBWModeCallback8190Pci() only
 *
 * Input:       struct adapter *			Adapter
 *		WIRELESS_BANDWIDTH_E	Bandwidth	20M or 40M
 *
 * Output:      NONE
 *
 * Return:      NONE
 *
 * Note:		For RF type 0222D
 *---------------------------------------------------------------------------*/
void PHY_RF6052SetBandwidth8723B(
<<<<<<< HEAD
	struct adapter *Adapter, enum CHANNEL_WIDTH Bandwidth
=======
	struct adapter *Adapter, enum channel_width Bandwidth
>>>>>>> upstream/android-13
) /* 20M or 40M */
{
	struct hal_com_data *pHalData = GET_HAL_DATA(Adapter);

	switch (Bandwidth) {
	case CHANNEL_WIDTH_20:
		pHalData->RfRegChnlVal[0] = ((pHalData->RfRegChnlVal[0] & 0xfffff3ff) | BIT10 | BIT11);
<<<<<<< HEAD
		PHY_SetRFReg(Adapter, ODM_RF_PATH_A, RF_CHNLBW, bRFRegOffsetMask, pHalData->RfRegChnlVal[0]);
		PHY_SetRFReg(Adapter, ODM_RF_PATH_B, RF_CHNLBW, bRFRegOffsetMask, pHalData->RfRegChnlVal[0]);
=======
		PHY_SetRFReg(Adapter, RF_PATH_A, RF_CHNLBW, bRFRegOffsetMask, pHalData->RfRegChnlVal[0]);
		PHY_SetRFReg(Adapter, RF_PATH_B, RF_CHNLBW, bRFRegOffsetMask, pHalData->RfRegChnlVal[0]);
>>>>>>> upstream/android-13
		break;

	case CHANNEL_WIDTH_40:
		pHalData->RfRegChnlVal[0] = ((pHalData->RfRegChnlVal[0] & 0xfffff3ff) | BIT10);
<<<<<<< HEAD
		PHY_SetRFReg(Adapter, ODM_RF_PATH_A, RF_CHNLBW, bRFRegOffsetMask, pHalData->RfRegChnlVal[0]);
		PHY_SetRFReg(Adapter, ODM_RF_PATH_B, RF_CHNLBW, bRFRegOffsetMask, pHalData->RfRegChnlVal[0]);
		break;

	default:
		/* RT_TRACE(COMP_DBG, DBG_LOUD, ("PHY_SetRF8225Bandwidth(): unknown Bandwidth: %#X\n", Bandwidth)); */
=======
		PHY_SetRFReg(Adapter, RF_PATH_A, RF_CHNLBW, bRFRegOffsetMask, pHalData->RfRegChnlVal[0]);
		PHY_SetRFReg(Adapter, RF_PATH_B, RF_CHNLBW, bRFRegOffsetMask, pHalData->RfRegChnlVal[0]);
		break;

	default:
>>>>>>> upstream/android-13
		break;
	}

}

static int phy_RF6052_Config_ParaFile(struct adapter *Adapter)
{
	u32 u4RegValue = 0;
	u8 eRFPath;
	struct bb_register_def *pPhyReg;
<<<<<<< HEAD

	int rtStatus = _SUCCESS;
	struct hal_com_data *pHalData = GET_HAL_DATA(Adapter);

	static char sz8723RadioAFile[] = RTL8723B_PHY_RADIO_A;
	static char sz8723RadioBFile[] = RTL8723B_PHY_RADIO_B;
	static s8 sz8723BTxPwrTrackFile[] = RTL8723B_TXPWR_TRACK;
	char *pszRadioAFile, *pszRadioBFile, *pszTxPwrTrackFile;

	pszRadioAFile = sz8723RadioAFile;
	pszRadioBFile = sz8723RadioBFile;
	pszTxPwrTrackFile = sz8723BTxPwrTrackFile;

=======
	struct hal_com_data *pHalData = GET_HAL_DATA(Adapter);

>>>>>>> upstream/android-13
	/* 3----------------------------------------------------------------- */
	/* 3 <2> Initialize RF */
	/* 3----------------------------------------------------------------- */
	/* for (eRFPath = RF_PATH_A; eRFPath <pHalData->NumTotalRFPath; eRFPath++) */
	for (eRFPath = 0; eRFPath < pHalData->NumTotalRFPath; eRFPath++) {

		pPhyReg = &pHalData->PHYRegDef[eRFPath];

		/*----Store original RFENV control type----*/
		switch (eRFPath) {
		case RF_PATH_A:
<<<<<<< HEAD
		case RF_PATH_C:
			u4RegValue = PHY_QueryBBReg(Adapter, pPhyReg->rfintfs, bRFSI_RFENV);
			break;
		case RF_PATH_B:
		case RF_PATH_D:
			u4RegValue = PHY_QueryBBReg(Adapter, pPhyReg->rfintfs, bRFSI_RFENV<<16);
=======
			u4RegValue = PHY_QueryBBReg(Adapter, pPhyReg->rfintfs, bRFSI_RFENV);
			break;
		case RF_PATH_B:
			u4RegValue = PHY_QueryBBReg(Adapter, pPhyReg->rfintfs, bRFSI_RFENV << 16);
>>>>>>> upstream/android-13
			break;
		}

		/*----Set RF_ENV enable----*/
<<<<<<< HEAD
		PHY_SetBBReg(Adapter, pPhyReg->rfintfe, bRFSI_RFENV<<16, 0x1);
=======
		PHY_SetBBReg(Adapter, pPhyReg->rfintfe, bRFSI_RFENV << 16, 0x1);
>>>>>>> upstream/android-13
		udelay(1);/* PlatformStallExecution(1); */

		/*----Set RF_ENV output high----*/
		PHY_SetBBReg(Adapter, pPhyReg->rfintfo, bRFSI_RFENV, 0x1);
		udelay(1);/* PlatformStallExecution(1); */

		/* Set bit number of Address and Data for RF register */
		PHY_SetBBReg(Adapter, pPhyReg->rfHSSIPara2, b3WireAddressLength, 0x0);	/*  Set 1 to 4 bits for 8255 */
		udelay(1);/* PlatformStallExecution(1); */

		PHY_SetBBReg(Adapter, pPhyReg->rfHSSIPara2, b3WireDataLength, 0x0);	/*  Set 0 to 12  bits for 8255 */
		udelay(1);/* PlatformStallExecution(1); */

		/*----Initialize RF fom connfiguration file----*/
		switch (eRFPath) {
		case RF_PATH_A:
<<<<<<< HEAD
			if (PHY_ConfigRFWithParaFile(Adapter, pszRadioAFile,
						     eRFPath) == _FAIL) {
				if (HAL_STATUS_FAILURE == ODM_ConfigRFWithHeaderFile(&pHalData->odmpriv, CONFIG_RF_RADIO, (ODM_RF_RADIO_PATH_E)eRFPath))
					rtStatus = _FAIL;
			}
			break;
		case RF_PATH_B:
			if (PHY_ConfigRFWithParaFile(Adapter, pszRadioBFile,
						     eRFPath) == _FAIL) {
				if (HAL_STATUS_FAILURE == ODM_ConfigRFWithHeaderFile(&pHalData->odmpriv, CONFIG_RF_RADIO, (ODM_RF_RADIO_PATH_E)eRFPath))
					rtStatus = _FAIL;
			}
			break;
		case RF_PATH_C:
			break;
		case RF_PATH_D:
=======
		case RF_PATH_B:
			ODM_ConfigRFWithHeaderFile(&pHalData->odmpriv,
						   CONFIG_RF_RADIO, eRFPath);
>>>>>>> upstream/android-13
			break;
		}

		/*----Restore RFENV control type----*/
		switch (eRFPath) {
		case RF_PATH_A:
<<<<<<< HEAD
		case RF_PATH_C:
			PHY_SetBBReg(Adapter, pPhyReg->rfintfs, bRFSI_RFENV, u4RegValue);
			break;
		case RF_PATH_B:
		case RF_PATH_D:
			PHY_SetBBReg(Adapter, pPhyReg->rfintfs, bRFSI_RFENV<<16, u4RegValue);
			break;
		}

		if (rtStatus != _SUCCESS) {
			/* RT_TRACE(COMP_FPGA, DBG_LOUD, ("phy_RF6052_Config_ParaFile():Radio[%d] Fail!!", eRFPath)); */
			goto phy_RF6052_Config_ParaFile_Fail;
		}

=======
			PHY_SetBBReg(Adapter, pPhyReg->rfintfs, bRFSI_RFENV, u4RegValue);
			break;
		case RF_PATH_B:
			PHY_SetBBReg(Adapter, pPhyReg->rfintfs, bRFSI_RFENV << 16, u4RegValue);
			break;
		}
>>>>>>> upstream/android-13
	}

	/* 3 ----------------------------------------------------------------- */
	/* 3 Configuration of Tx Power Tracking */
	/* 3 ----------------------------------------------------------------- */

<<<<<<< HEAD
	if (PHY_ConfigRFWithTxPwrTrackParaFile(Adapter, pszTxPwrTrackFile) ==
		_FAIL) {
		ODM_ConfigRFWithTxPwrTrackHeaderFile(&pHalData->odmpriv);
	}

	/* RT_TRACE(COMP_INIT, DBG_LOUD, ("<---phy_RF6052_Config_ParaFile()\n")); */
	return rtStatus;

phy_RF6052_Config_ParaFile_Fail:
	return rtStatus;
=======
	ODM_ConfigRFWithTxPwrTrackHeaderFile(&pHalData->odmpriv);

	return _SUCCESS;
>>>>>>> upstream/android-13
}


int PHY_RF6052_Config8723B(struct adapter *Adapter)
{
	struct hal_com_data *pHalData = GET_HAL_DATA(Adapter);
<<<<<<< HEAD
	int rtStatus = _SUCCESS;
=======
>>>>>>> upstream/android-13

	/*  */
	/*  Initialize general global value */
	/*  */
<<<<<<< HEAD
	/*  TODO: Extend RF_PATH_C and RF_PATH_D in the future */
	if (pHalData->rf_type == RF_1T1R)
		pHalData->NumTotalRFPath = 1;
	else
		pHalData->NumTotalRFPath = 2;
=======
	pHalData->NumTotalRFPath = 1;
>>>>>>> upstream/android-13

	/*  */
	/*  Config BB and RF */
	/*  */
<<<<<<< HEAD
	rtStatus = phy_RF6052_Config_ParaFile(Adapter);
	return rtStatus;
=======
	return phy_RF6052_Config_ParaFile(Adapter);
>>>>>>> upstream/android-13

}

/* End of HalRf6052.c */
