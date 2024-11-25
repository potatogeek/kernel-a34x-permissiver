// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/

#include "odm_precomp.h"

static void odm_SetCrystalCap(void *pDM_VOID, u8 CrystalCap)
{
<<<<<<< HEAD
	PDM_ODM_T pDM_Odm = (PDM_ODM_T)pDM_VOID;
	PCFO_TRACKING pCfoTrack = &pDM_Odm->DM_CfoTrack;
	bool bEEPROMCheck;
	struct adapter *Adapter = pDM_Odm->Adapter;
	struct hal_com_data *pHalData = GET_HAL_DATA(Adapter);

	bEEPROMCheck = pHalData->EEPROMVersion >= 0x01;
=======
	struct dm_odm_t *pDM_Odm = (struct dm_odm_t *)pDM_VOID;
	struct cfo_tracking *pCfoTrack = &pDM_Odm->DM_CfoTrack;
>>>>>>> upstream/android-13

	if (pCfoTrack->CrystalCap == CrystalCap)
		return;

	pCfoTrack->CrystalCap = CrystalCap;

	/*  0x2C[23:18] = 0x2C[17:12] = CrystalCap */
	CrystalCap = CrystalCap & 0x3F;
	PHY_SetBBReg(
		pDM_Odm->Adapter,
		REG_MAC_PHY_CTRL,
		0x00FFF000,
		(CrystalCap | (CrystalCap << 6))
	);
<<<<<<< HEAD

	ODM_RT_TRACE(
		pDM_Odm,
		ODM_COMP_CFO_TRACKING,
		ODM_DBG_LOUD,
		(
			"odm_SetCrystalCap(): CrystalCap = 0x%x\n",
			CrystalCap
		)
	);
=======
>>>>>>> upstream/android-13
}

static u8 odm_GetDefaultCrytaltalCap(void *pDM_VOID)
{
<<<<<<< HEAD
	PDM_ODM_T pDM_Odm = (PDM_ODM_T)pDM_VOID;
	u8 CrystalCap = 0x20;
=======
	struct dm_odm_t *pDM_Odm = (struct dm_odm_t *)pDM_VOID;
>>>>>>> upstream/android-13

	struct adapter *Adapter = pDM_Odm->Adapter;
	struct hal_com_data *pHalData = GET_HAL_DATA(Adapter);

<<<<<<< HEAD
	CrystalCap = pHalData->CrystalCap;

	CrystalCap = CrystalCap & 0x3f;

	return CrystalCap;
=======
	return pHalData->CrystalCap & 0x3f;
>>>>>>> upstream/android-13
}

static void odm_SetATCStatus(void *pDM_VOID, bool ATCStatus)
{
<<<<<<< HEAD
	PDM_ODM_T pDM_Odm = (PDM_ODM_T)pDM_VOID;
	PCFO_TRACKING pCfoTrack = &pDM_Odm->DM_CfoTrack;
=======
	struct dm_odm_t *pDM_Odm = (struct dm_odm_t *)pDM_VOID;
	struct cfo_tracking *pCfoTrack = &pDM_Odm->DM_CfoTrack;
>>>>>>> upstream/android-13

	if (pCfoTrack->bATCStatus == ATCStatus)
		return;

	PHY_SetBBReg(
		pDM_Odm->Adapter,
		ODM_REG(BB_ATC, pDM_Odm),
		ODM_BIT(BB_ATC, pDM_Odm),
		ATCStatus
	);
	pCfoTrack->bATCStatus = ATCStatus;
}

static bool odm_GetATCStatus(void *pDM_VOID)
{
	bool ATCStatus;
<<<<<<< HEAD
	PDM_ODM_T pDM_Odm = (PDM_ODM_T)pDM_VOID;
=======
	struct dm_odm_t *pDM_Odm = (struct dm_odm_t *)pDM_VOID;
>>>>>>> upstream/android-13

	ATCStatus = (bool)PHY_QueryBBReg(
		pDM_Odm->Adapter,
		ODM_REG(BB_ATC, pDM_Odm),
		ODM_BIT(BB_ATC, pDM_Odm)
	);
	return ATCStatus;
}

void ODM_CfoTrackingReset(void *pDM_VOID)
{
<<<<<<< HEAD
	PDM_ODM_T pDM_Odm = (PDM_ODM_T)pDM_VOID;
	PCFO_TRACKING pCfoTrack = &pDM_Odm->DM_CfoTrack;
=======
	struct dm_odm_t *pDM_Odm = (struct dm_odm_t *)pDM_VOID;
	struct cfo_tracking *pCfoTrack = &pDM_Odm->DM_CfoTrack;
>>>>>>> upstream/android-13

	pCfoTrack->DefXCap = odm_GetDefaultCrytaltalCap(pDM_Odm);
	pCfoTrack->bAdjust = true;

	odm_SetCrystalCap(pDM_Odm, pCfoTrack->DefXCap);
	odm_SetATCStatus(pDM_Odm, true);
}

void ODM_CfoTrackingInit(void *pDM_VOID)
{
<<<<<<< HEAD
	PDM_ODM_T pDM_Odm = (PDM_ODM_T)pDM_VOID;
	PCFO_TRACKING pCfoTrack = &pDM_Odm->DM_CfoTrack;
=======
	struct dm_odm_t *pDM_Odm = (struct dm_odm_t *)pDM_VOID;
	struct cfo_tracking *pCfoTrack = &pDM_Odm->DM_CfoTrack;
>>>>>>> upstream/android-13

	pCfoTrack->DefXCap =
		pCfoTrack->CrystalCap = odm_GetDefaultCrytaltalCap(pDM_Odm);
	pCfoTrack->bATCStatus = odm_GetATCStatus(pDM_Odm);
	pCfoTrack->bAdjust = true;
<<<<<<< HEAD
	ODM_RT_TRACE(
		pDM_Odm,
		ODM_COMP_CFO_TRACKING,
		ODM_DBG_LOUD,
		("ODM_CfoTracking_init() =========>\n")
	);
	ODM_RT_TRACE(
		pDM_Odm,
		ODM_COMP_CFO_TRACKING,
		ODM_DBG_LOUD,
		(
			"ODM_CfoTracking_init(): bATCStatus = %d, CrystalCap = 0x%x\n",
			pCfoTrack->bATCStatus,
			pCfoTrack->DefXCap
		)
	);
=======
>>>>>>> upstream/android-13
}

void ODM_CfoTracking(void *pDM_VOID)
{
<<<<<<< HEAD
	PDM_ODM_T pDM_Odm = (PDM_ODM_T)pDM_VOID;
	PCFO_TRACKING pCfoTrack = &pDM_Odm->DM_CfoTrack;
	int CFO_kHz_A, CFO_kHz_B, CFO_ave = 0;
=======
	struct dm_odm_t *pDM_Odm = (struct dm_odm_t *)pDM_VOID;
	struct cfo_tracking *pCfoTrack = &pDM_Odm->DM_CfoTrack;
	int CFO_kHz_A, CFO_ave = 0;
>>>>>>> upstream/android-13
	int CFO_ave_diff;
	int CrystalCap = (int)pCfoTrack->CrystalCap;
	u8 Adjust_Xtal = 1;

	/* 4 Support ability */
	if (!(pDM_Odm->SupportAbility & ODM_BB_CFO_TRACKING)) {
<<<<<<< HEAD
		ODM_RT_TRACE(
			pDM_Odm,
			ODM_COMP_CFO_TRACKING,
			ODM_DBG_LOUD,
			("ODM_CfoTracking(): Return: SupportAbility ODM_BB_CFO_TRACKING is disabled\n")
		);
		return;
	}

	ODM_RT_TRACE(
		pDM_Odm,
		ODM_COMP_CFO_TRACKING,
		ODM_DBG_LOUD,
		("ODM_CfoTracking() =========>\n")
	);

	if (!pDM_Odm->bLinked || !pDM_Odm->bOneEntryOnly) {
		/* 4 No link or more than one entry */
		ODM_CfoTrackingReset(pDM_Odm);
		ODM_RT_TRACE(
			pDM_Odm,
			ODM_COMP_CFO_TRACKING,
			ODM_DBG_LOUD,
			(
				"ODM_CfoTracking(): Reset: bLinked = %d, bOneEntryOnly = %d\n",
				pDM_Odm->bLinked,
				pDM_Odm->bOneEntryOnly
			)
		);
=======
		return;
	}

	if (!pDM_Odm->bLinked || !pDM_Odm->bOneEntryOnly) {
		/* 4 No link or more than one entry */
		ODM_CfoTrackingReset(pDM_Odm);
>>>>>>> upstream/android-13
	} else {
		/* 3 1. CFO Tracking */
		/* 4 1.1 No new packet */
		if (pCfoTrack->packetCount == pCfoTrack->packetCount_pre) {
<<<<<<< HEAD
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_CFO_TRACKING,
				ODM_DBG_LOUD,
				(
					"ODM_CfoTracking(): packet counter doesn't change\n"
				)
			);
=======
>>>>>>> upstream/android-13
			return;
		}
		pCfoTrack->packetCount_pre = pCfoTrack->packetCount;

		/* 4 1.2 Calculate CFO */
		CFO_kHz_A =  (int)(pCfoTrack->CFO_tail[0] * 3125)  / 1280;
<<<<<<< HEAD
		CFO_kHz_B =  (int)(pCfoTrack->CFO_tail[1] * 3125)  / 1280;

		if (pDM_Odm->RFType < ODM_2T2R)
			CFO_ave = CFO_kHz_A;
		else
			CFO_ave = (int)(CFO_kHz_A + CFO_kHz_B) >> 1;
		ODM_RT_TRACE(
			pDM_Odm,
			ODM_COMP_CFO_TRACKING,
			ODM_DBG_LOUD,
			(
				"ODM_CfoTracking(): CFO_kHz_A = %dkHz, CFO_kHz_B = %dkHz, CFO_ave = %dkHz\n",
				CFO_kHz_A,
				CFO_kHz_B,
				CFO_ave
			)
		);
=======

		CFO_ave = CFO_kHz_A;
>>>>>>> upstream/android-13

		/* 4 1.3 Avoid abnormal large CFO */
		CFO_ave_diff =
			(pCfoTrack->CFO_ave_pre >= CFO_ave) ?
			(pCfoTrack->CFO_ave_pre-CFO_ave) :
			(CFO_ave-pCfoTrack->CFO_ave_pre);

		if (
			CFO_ave_diff > 20 &&
			pCfoTrack->largeCFOHit == 0 &&
			!pCfoTrack->bAdjust
		) {
<<<<<<< HEAD
			ODM_RT_TRACE(pDM_Odm, ODM_COMP_CFO_TRACKING, ODM_DBG_LOUD, ("ODM_CfoTracking(): first large CFO hit\n"));
=======
>>>>>>> upstream/android-13
			pCfoTrack->largeCFOHit = 1;
			return;
		} else
			pCfoTrack->largeCFOHit = 0;
		pCfoTrack->CFO_ave_pre = CFO_ave;

		/* 4 1.4 Dynamic Xtal threshold */
		if (pCfoTrack->bAdjust == false) {
			if (CFO_ave > CFO_TH_XTAL_HIGH || CFO_ave < (-CFO_TH_XTAL_HIGH))
				pCfoTrack->bAdjust = true;
		} else {
			if (CFO_ave < CFO_TH_XTAL_LOW && CFO_ave > (-CFO_TH_XTAL_LOW))
				pCfoTrack->bAdjust = false;
		}

		/* 4 1.5 BT case: Disable CFO tracking */
		if (pDM_Odm->bBtEnabled) {
			pCfoTrack->bAdjust = false;
			odm_SetCrystalCap(pDM_Odm, pCfoTrack->DefXCap);
<<<<<<< HEAD
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_CFO_TRACKING,
				ODM_DBG_LOUD,
				("ODM_CfoTracking(): Disable CFO tracking for BT!!\n")
			);
=======
>>>>>>> upstream/android-13
		}

		/* 4 1.6 Big jump */
		if (pCfoTrack->bAdjust) {
			if (CFO_ave > CFO_TH_XTAL_LOW)
				Adjust_Xtal = Adjust_Xtal+((CFO_ave-CFO_TH_XTAL_LOW)>>2);
			else if (CFO_ave < (-CFO_TH_XTAL_LOW))
				Adjust_Xtal = Adjust_Xtal+((CFO_TH_XTAL_LOW-CFO_ave)>>2);
<<<<<<< HEAD

			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_CFO_TRACKING,
				ODM_DBG_LOUD,
				(
					"ODM_CfoTracking(): Crystal cap offset = %d\n",
					Adjust_Xtal
				)
			);
=======
>>>>>>> upstream/android-13
		}

		/* 4 1.7 Adjust Crystal Cap. */
		if (pCfoTrack->bAdjust) {
			if (CFO_ave > CFO_TH_XTAL_LOW)
				CrystalCap = CrystalCap + Adjust_Xtal;
			else if (CFO_ave < (-CFO_TH_XTAL_LOW))
				CrystalCap = CrystalCap - Adjust_Xtal;

			if (CrystalCap > 0x3f)
				CrystalCap = 0x3f;
			else if (CrystalCap < 0)
				CrystalCap = 0;

			odm_SetCrystalCap(pDM_Odm, (u8)CrystalCap);
		}
<<<<<<< HEAD
		ODM_RT_TRACE(
			pDM_Odm,
			ODM_COMP_CFO_TRACKING,
			ODM_DBG_LOUD,
			(
				"ODM_CfoTracking(): Crystal cap = 0x%x, Default Crystal cap = 0x%x\n",
				pCfoTrack->CrystalCap,
				pCfoTrack->DefXCap
			)
		);
=======
>>>>>>> upstream/android-13

		/* 3 2. Dynamic ATC switch */
		if (CFO_ave < CFO_TH_ATC && CFO_ave > -CFO_TH_ATC) {
			odm_SetATCStatus(pDM_Odm, false);
<<<<<<< HEAD
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_CFO_TRACKING,
				ODM_DBG_LOUD,
				("ODM_CfoTracking(): Disable ATC!!\n")
			);
		} else {
			odm_SetATCStatus(pDM_Odm, true);
			ODM_RT_TRACE(
				pDM_Odm,
				ODM_COMP_CFO_TRACKING,
				ODM_DBG_LOUD,
				("ODM_CfoTracking(): Enable ATC!!\n")
			);
=======
		} else {
			odm_SetATCStatus(pDM_Odm, true);
>>>>>>> upstream/android-13
		}
	}
}

<<<<<<< HEAD
void ODM_ParsingCFO(void *pDM_VOID, void *pPktinfo_VOID, s8 *pcfotail)
{
	PDM_ODM_T pDM_Odm = (PDM_ODM_T)pDM_VOID;
	struct odm_packet_info *pPktinfo = (struct odm_packet_info *)pPktinfo_VOID;
	PCFO_TRACKING pCfoTrack = &pDM_Odm->DM_CfoTrack;
	u8 i;

	if (!(pDM_Odm->SupportAbility & ODM_BB_CFO_TRACKING))
		return;

	if (pPktinfo->station_id != 0) {
		/* 3 Update CFO report for path-A & path-B */
		/*  Only paht-A and path-B have CFO tail and short CFO */
		for (i = ODM_RF_PATH_A; i <= ODM_RF_PATH_B; i++)
			pCfoTrack->CFO_tail[i] = (int)pcfotail[i];

		/* 3 Update packet counter */
		if (pCfoTrack->packetCount == 0xffffffff)
			pCfoTrack->packetCount = 0;
		else
			pCfoTrack->packetCount++;
=======
void odm_parsing_cfo(void *dm_void, void *pkt_info_void, s8 *cfotail)
{
	struct dm_odm_t *dm_odm = (struct dm_odm_t *)dm_void;
	struct odm_packet_info *pkt_info = pkt_info_void;
	struct cfo_tracking *cfo_track = &dm_odm->DM_CfoTrack;
	u8 i;

	if (!(dm_odm->SupportAbility & ODM_BB_CFO_TRACKING))
		return;

	if (pkt_info->station_id != 0) {
		/*
		 * 3 Update CFO report for path-A & path-B
		 * Only paht-A and path-B have CFO tail and short CFO
		 */
		for (i = RF_PATH_A; i <= RF_PATH_B; i++)
			cfo_track->CFO_tail[i] = (int)cfotail[i];

		/* 3 Update packet counter */
		if (cfo_track->packetCount == 0xffffffff)
			cfo_track->packetCount = 0;
		else
			cfo_track->packetCount++;
>>>>>>> upstream/android-13
	}
}
