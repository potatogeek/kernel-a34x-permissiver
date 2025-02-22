/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/

#ifndef	__ODMCFOTRACK_H__
#define    __ODMCFOTRACK_H__

#define		CFO_TH_XTAL_HIGH		20		/*  kHz */
#define		CFO_TH_XTAL_LOW			10		/*  kHz */
#define		CFO_TH_ATC			80		/*  kHz */

<<<<<<< HEAD
typedef struct _CFO_TRACKING_ {
=======
struct cfo_tracking {
>>>>>>> upstream/android-13
	bool bATCStatus;
	bool largeCFOHit;
	bool bAdjust;
	u8 CrystalCap;
	u8 DefXCap;
	int CFO_tail[2];
	int CFO_ave_pre;
	u32 packetCount;
	u32 packetCount_pre;

	bool bForceXtalCap;
	bool bReset;
<<<<<<< HEAD
} CFO_TRACKING, *PCFO_TRACKING;
=======
};
>>>>>>> upstream/android-13

void ODM_CfoTrackingReset(void *pDM_VOID
);

void ODM_CfoTrackingInit(void *pDM_VOID);

void ODM_CfoTracking(void *pDM_VOID);

<<<<<<< HEAD
void ODM_ParsingCFO(void *pDM_VOID, void *pPktinfo_VOID, s8 *pcfotail);
=======
void odm_parsing_cfo(void *pDM_VOID, void *pPktinfo_VOID, s8 *pcfotail);
>>>>>>> upstream/android-13

#endif
