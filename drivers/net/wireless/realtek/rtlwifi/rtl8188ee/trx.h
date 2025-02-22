<<<<<<< HEAD
/******************************************************************************
 *
 * Copyright(c) 2009-2013  Realtek Corporation.
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
/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright(c) 2009-2013  Realtek Corporation.*/
>>>>>>> upstream/android-13

#ifndef __RTL92CE_TRX_H__
#define __RTL92CE_TRX_H__

#define TX_DESC_SIZE					64
#define TX_DESC_AGGR_SUBFRAME_SIZE		32

#define RX_DESC_SIZE					32
#define RX_DRV_INFO_SIZE_UNIT			8

#define	TX_DESC_NEXT_DESC_OFFSET		40
#define USB_HWDESC_HEADER_LEN			32
#define CRCLENGTH						4

<<<<<<< HEAD
#define SET_TX_DESC_PKT_SIZE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc, 0, 16, __val)
#define SET_TX_DESC_OFFSET(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc, 16, 8, __val)
#define SET_TX_DESC_BMC(__pdesc, __val)				\
	SET_BITS_TO_LE_4BYTE(__pdesc, 24, 1, __val)
#define SET_TX_DESC_HTC(__pdesc, __val)				\
	SET_BITS_TO_LE_4BYTE(__pdesc, 25, 1, __val)
#define SET_TX_DESC_LAST_SEG(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc, 26, 1, __val)
#define SET_TX_DESC_FIRST_SEG(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc, 27, 1, __val)
#define SET_TX_DESC_LINIP(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc, 28, 1, __val)
#define SET_TX_DESC_NO_ACM(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc, 29, 1, __val)
#define SET_TX_DESC_GF(__pdesc, __val)				\
	SET_BITS_TO_LE_4BYTE(__pdesc, 30, 1, __val)
#define SET_TX_DESC_OWN(__pdesc, __val)				\
	SET_BITS_TO_LE_4BYTE(__pdesc, 31, 1, __val)

#define GET_TX_DESC_PKT_SIZE(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc, 0, 16)
#define GET_TX_DESC_OFFSET(__pdesc)					\
	LE_BITS_TO_4BYTE(__pdesc, 16, 8)
#define GET_TX_DESC_BMC(__pdesc)					\
	LE_BITS_TO_4BYTE(__pdesc, 24, 1)
#define GET_TX_DESC_HTC(__pdesc)					\
	LE_BITS_TO_4BYTE(__pdesc, 25, 1)
#define GET_TX_DESC_LAST_SEG(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc, 26, 1)
#define GET_TX_DESC_FIRST_SEG(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc, 27, 1)
#define GET_TX_DESC_LINIP(__pdesc)					\
	LE_BITS_TO_4BYTE(__pdesc, 28, 1)
#define GET_TX_DESC_NO_ACM(__pdesc)					\
	LE_BITS_TO_4BYTE(__pdesc, 29, 1)
#define GET_TX_DESC_GF(__pdesc)						\
	LE_BITS_TO_4BYTE(__pdesc, 30, 1)
#define GET_TX_DESC_OWN(__pdesc)					\
	LE_BITS_TO_4BYTE(__pdesc, 31, 1)

#define SET_TX_DESC_MACID(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 0, 6, __val)
#define SET_TX_DESC_QUEUE_SEL(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 8, 5, __val)
#define SET_TX_DESC_RDG_NAV_EXT(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 13, 1, __val)
#define SET_TX_DESC_LSIG_TXOP_EN(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 14, 1, __val)
#define SET_TX_DESC_PIFS(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 15, 1, __val)
#define SET_TX_DESC_RATE_ID(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 16, 4, __val)
#define SET_TX_DESC_NAV_USE_HDR(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 20, 1, __val)
#define SET_TX_DESC_EN_DESC_ID(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 21, 1, __val)
#define SET_TX_DESC_SEC_TYPE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 22, 2, __val)
#define SET_TX_DESC_PKT_OFFSET(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 26, 5, __val)
#define SET_TX_DESC_PADDING_LEN(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 24, 8, __val)

#define GET_TX_DESC_MACID(__pdesc)					\
	LE_BITS_TO_4BYTE(__pdesc+4, 0, 5)
#define GET_TX_DESC_AGG_ENABLE(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+4, 5, 1)
#define GET_TX_DESC_AGG_BREAK(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+4, 6, 1)
#define GET_TX_DESC_RDG_ENABLE(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+4, 7, 1)
#define GET_TX_DESC_QUEUE_SEL(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+4, 8, 5)
#define GET_TX_DESC_RDG_NAV_EXT(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 13, 1)
#define GET_TX_DESC_LSIG_TXOP_EN(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 14, 1)
#define GET_TX_DESC_PIFS(__pdesc)					\
	LE_BITS_TO_4BYTE(__pdesc+4, 15, 1)
#define GET_TX_DESC_RATE_ID(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+4, 16, 4)
#define GET_TX_DESC_NAV_USE_HDR(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 20, 1)
#define GET_TX_DESC_EN_DESC_ID(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+4, 21, 1)
#define GET_TX_DESC_SEC_TYPE(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+4, 22, 2)
#define GET_TX_DESC_PKT_OFFSET(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+4, 24, 8)

#define SET_TX_DESC_RTS_RC(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 0, 6, __val)
#define SET_TX_DESC_DATA_RC(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 6, 6, __val)
#define SET_TX_DESC_AGG_ENABLE(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 12, 1, __val)
#define SET_TX_DESC_RDG_ENABLE(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 13, 1, __val)
#define SET_TX_DESC_BAR_RTY_TH(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 14, 2, __val)
#define SET_TX_DESC_AGG_BREAK(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 16, 1, __val)
#define SET_TX_DESC_MORE_FRAG(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 17, 1, __val)
#define SET_TX_DESC_RAW(__pdesc, __val)				\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 18, 1, __val)
#define SET_TX_DESC_CCX(__pdesc, __val)				\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 19, 1, __val)
#define SET_TX_DESC_AMPDU_DENSITY(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 20, 3, __val)
#define SET_TX_DESC_BT_INT(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 23, 1, __val)
#define SET_TX_DESC_ANTSEL_A(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 24, 1, __val)
#define SET_TX_DESC_ANTSEL_B(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 25, 1, __val)
#define SET_TX_DESC_TX_ANT_CCK(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 26, 2, __val)
#define SET_TX_DESC_TX_ANTL(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 28, 2, __val)
#define SET_TX_DESC_TX_ANT_HT(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 30, 2, __val)

#define GET_TX_DESC_RTS_RC(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+8, 0, 6)
#define GET_TX_DESC_DATA_RC(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+8, 6, 6)
#define GET_TX_DESC_BAR_RTY_TH(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+8, 14, 2)
#define GET_TX_DESC_MORE_FRAG(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+8, 17, 1)
#define GET_TX_DESC_RAW(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+8, 18, 1)
#define GET_TX_DESC_CCX(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+8, 19, 1)
#define GET_TX_DESC_AMPDU_DENSITY(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+8, 20, 3)
#define GET_TX_DESC_ANTSEL_A(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+8, 24, 1)
#define GET_TX_DESC_ANTSEL_B(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+8, 25, 1)
#define GET_TX_DESC_TX_ANT_CCK(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+8, 26, 2)
#define GET_TX_DESC_TX_ANTL(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+8, 28, 2)
#define GET_TX_DESC_TX_ANT_HT(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+8, 30, 2)

#define SET_TX_DESC_NEXT_HEAP_PAGE(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 0, 8, __val)
#define SET_TX_DESC_TAIL_PAGE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 8, 8, __val)
#define SET_TX_DESC_SEQ(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 16, 12, __val)
#define SET_TX_DESC_CPU_HANDLE(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 28, 1, __val)
#define SET_TX_DESC_TAG1(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 29, 1, __val)
#define SET_TX_DESC_TRIGGER_INT(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 30, 1, __val)
#define SET_TX_DESC_HWSEQ_EN(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 31, 1, __val)

#define GET_TX_DESC_NEXT_HEAP_PAGE(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+12, 0, 8)
#define GET_TX_DESC_TAIL_PAGE(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+12, 8, 8)
#define GET_TX_DESC_SEQ(__pdesc)					\
	LE_BITS_TO_4BYTE(__pdesc+12, 16, 12)

#define SET_TX_DESC_RTS_RATE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 0, 5, __val)
#define SET_TX_DESC_AP_DCFE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 5, 1, __val)
#define SET_TX_DESC_QOS(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 6, 1, __val)
#define SET_TX_DESC_HWSEQ_SSN(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 7, 1, __val)
#define SET_TX_DESC_USE_RATE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 8, 1, __val)
#define SET_TX_DESC_DISABLE_RTS_FB(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 9, 1, __val)
#define SET_TX_DESC_DISABLE_FB(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 10, 1, __val)
#define SET_TX_DESC_CTS2SELF(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 11, 1, __val)
#define SET_TX_DESC_RTS_ENABLE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 12, 1, __val)
#define SET_TX_DESC_HW_RTS_ENABLE(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 13, 1, __val)
#define SET_TX_DESC_PORT_ID(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 14, 1, __val)
#define SET_TX_DESC_PWR_STATUS(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 15, 3, __val)
#define SET_TX_DESC_WAIT_DCTS(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 18, 1, __val)
#define SET_TX_DESC_CTS2AP_EN(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 19, 1, __val)
#define SET_TX_DESC_TX_SUB_CARRIER(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 20, 2, __val)
#define SET_TX_DESC_TX_STBC(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 22, 2, __val)
#define SET_TX_DESC_DATA_SHORT(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 24, 1, __val)
#define SET_TX_DESC_DATA_BW(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 25, 1, __val)
#define SET_TX_DESC_RTS_SHORT(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 26, 1, __val)
#define SET_TX_DESC_RTS_BW(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 27, 1, __val)
#define SET_TX_DESC_RTS_SC(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 28, 2, __val)
#define SET_TX_DESC_RTS_STBC(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 30, 2, __val)

#define GET_TX_DESC_RTS_RATE(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 0, 5)
#define GET_TX_DESC_AP_DCFE(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 5, 1)
#define GET_TX_DESC_QOS(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 6, 1)
#define GET_TX_DESC_HWSEQ_EN(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 7, 1)
#define GET_TX_DESC_USE_RATE(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 8, 1)
#define GET_TX_DESC_DISABLE_RTS_FB(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+16, 9, 1)
#define GET_TX_DESC_DISABLE_FB(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 10, 1)
#define GET_TX_DESC_CTS2SELF(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 11, 1)
#define GET_TX_DESC_RTS_ENABLE(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 12, 1)
#define GET_TX_DESC_HW_RTS_ENABLE(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+16, 13, 1)
#define GET_TX_DESC_PORT_ID(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 14, 1)
#define GET_TX_DESC_WAIT_DCTS(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 18, 1)
#define GET_TX_DESC_CTS2AP_EN(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 19, 1)
#define GET_TX_DESC_TX_SUB_CARRIER(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+16, 20, 2)
#define GET_TX_DESC_TX_STBC(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 22, 2)
#define GET_TX_DESC_DATA_SHORT(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 24, 1)
#define GET_TX_DESC_DATA_BW(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 25, 1)
#define GET_TX_DESC_RTS_SHORT(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 26, 1)
#define GET_TX_DESC_RTS_BW(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 27, 1)
#define GET_TX_DESC_RTS_SC(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 28, 2)
#define GET_TX_DESC_RTS_STBC(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 30, 2)

#define SET_TX_DESC_TX_RATE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 0, 6, __val)
#define SET_TX_DESC_DATA_SHORTGI(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 6, 1, __val)
#define SET_TX_DESC_CCX_TAG(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 7, 1, __val)
#define SET_TX_DESC_DATA_RATE_FB_LIMIT(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 8, 5, __val)
#define SET_TX_DESC_RTS_RATE_FB_LIMIT(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 13, 4, __val)
#define SET_TX_DESC_RETRY_LIMIT_ENABLE(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 17, 1, __val)
#define SET_TX_DESC_DATA_RETRY_LIMIT(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 18, 6, __val)
#define SET_TX_DESC_USB_TXAGG_NUM(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 24, 8, __val)

#define GET_TX_DESC_TX_RATE(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+20, 0, 6)
#define GET_TX_DESC_DATA_SHORTGI(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+20, 6, 1)
#define GET_TX_DESC_CCX_TAG(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+20, 7, 1)
#define GET_TX_DESC_DATA_RATE_FB_LIMIT(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+20, 8, 5)
#define GET_TX_DESC_RTS_RATE_FB_LIMIT(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+20, 13, 4)
#define GET_TX_DESC_RETRY_LIMIT_ENABLE(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+20, 17, 1)
#define GET_TX_DESC_DATA_RETRY_LIMIT(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+20, 18, 6)
#define GET_TX_DESC_USB_TXAGG_NUM(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+20, 24, 8)

#define SET_TX_DESC_TXAGC_A(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+24, 0, 5, __val)
#define SET_TX_DESC_TXAGC_B(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+24, 5, 5, __val)
#define SET_TX_DESC_USE_MAX_LEN(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+24, 10, 1, __val)
#define SET_TX_DESC_MAX_AGG_NUM(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+24, 11, 5, __val)
#define SET_TX_DESC_MCSG1_MAX_LEN(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+24, 16, 4, __val)
#define SET_TX_DESC_MCSG2_MAX_LEN(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+24, 20, 4, __val)
#define SET_TX_DESC_MCSG3_MAX_LEN(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+24, 24, 4, __val)
#define SET_TX_DESC_MCS7_SGI_MAX_LEN(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+24, 28, 4, __val)

#define GET_TX_DESC_TXAGC_A(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+24, 0, 5)
#define GET_TX_DESC_TXAGC_B(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+24, 5, 5)
#define GET_TX_DESC_USE_MAX_LEN(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+24, 10, 1)
#define GET_TX_DESC_MAX_AGG_NUM(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+24, 11, 5)
#define GET_TX_DESC_MCSG1_MAX_LEN(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+24, 16, 4)
#define GET_TX_DESC_MCSG2_MAX_LEN(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+24, 20, 4)
#define GET_TX_DESC_MCSG3_MAX_LEN(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+24, 24, 4)
#define GET_TX_DESC_MCS7_SGI_MAX_LEN(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+24, 28, 4)

#define SET_TX_DESC_TX_BUFFER_SIZE(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+28, 0, 16, __val)
#define SET_TX_DESC_SW_OFFSET30(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+28, 16, 8, __val)
#define SET_TX_DESC_SW_OFFSET31(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+28, 24, 4, __val)
#define SET_TX_DESC_ANTSEL_C(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+28, 29, 1, __val)
#define SET_TX_DESC_NULL_0(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+28, 30, 1, __val)
#define SET_TX_DESC_NULL_1(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+28, 30, 1, __val)

#define GET_TX_DESC_TX_BUFFER_SIZE(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+28, 0, 16)

#define SET_TX_DESC_TX_BUFFER_ADDRESS(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+32, 0, 32, __val)
#define SET_TX_DESC_TX_BUFFER_ADDRESS64(__pdesc, __val) \
	SET_BITS_TO_LE_4BYTE(__pdesc+36, 0, 32, __val)

#define GET_TX_DESC_TX_BUFFER_ADDRESS(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+32, 0, 32)
#define GET_TX_DESC_TX_BUFFER_ADDRESS64(__pdesc)	\
	LE_BITS_TO_4BYTE(__pdesc+36, 0, 32)

#define SET_TX_DESC_NEXT_DESC_ADDRESS(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+40, 0, 32, __val)
#define SET_TX_DESC_NEXT_DESC_ADDRESS64(__pdesc, __val) \
	SET_BITS_TO_LE_4BYTE(__pdesc+44, 0, 32, __val)

#define GET_TX_DESC_NEXT_DESC_ADDRESS(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+40, 0, 32)
#define GET_TX_DESC_NEXT_DESC_ADDRESS64(__pdesc)	\
	LE_BITS_TO_4BYTE(__pdesc+44, 0, 32)

#define GET_RX_DESC_PKT_LEN(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 0, 14)
#define GET_RX_DESC_CRC32(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 14, 1)
#define GET_RX_DESC_ICV(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 15, 1)
#define GET_RX_DESC_DRV_INFO_SIZE(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc, 16, 4)
#define GET_RX_DESC_SECURITY(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 20, 3)
#define GET_RX_DESC_QOS(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 23, 1)
#define GET_RX_DESC_SHIFT(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 24, 2)
#define GET_RX_DESC_PHYST(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 26, 1)
#define GET_RX_DESC_SWDEC(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 27, 1)
#define GET_RX_DESC_LS(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc, 28, 1)
#define GET_RX_DESC_FS(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc, 29, 1)
#define GET_RX_DESC_EOR(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 30, 1)
#define GET_RX_DESC_OWN(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 31, 1)

#define SET_RX_DESC_PKT_LEN(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc, 0, 14, __val)
#define SET_RX_DESC_EOR(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc, 30, 1, __val)
#define SET_RX_DESC_OWN(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc, 31, 1, __val)

#define GET_RX_DESC_MACID(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 0, 6)
#define GET_RX_DESC_PAGGR(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 14, 1)
#define GET_RX_DESC_FAGGR(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 15, 1)
#define GET_RX_DESC_A1_FIT(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 16, 4)
#define GET_RX_DESC_A2_FIT(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 20, 4)
#define GET_RX_DESC_PAM(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 24, 1)
#define GET_RX_DESC_PWR(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 25, 1)
#define GET_RX_DESC_MD(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+4, 26, 1)
#define GET_RX_DESC_MF(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+4, 27, 1)
#define GET_RX_DESC_TYPE(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 28, 2)
#define GET_RX_DESC_MC(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+4, 30, 1)
#define GET_RX_DESC_BC(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+4, 31, 1)
#define GET_RX_DESC_SEQ(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+8, 0, 12)
#define GET_RX_DESC_FRAG(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+8, 12, 4)

#define GET_RX_DESC_RXMCS(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+12, 0, 6)
#define GET_RX_DESC_RXHT(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+12, 6, 1)
#define GET_RX_STATUS_DESC_RX_GF(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+12, 7, 1)
#define GET_RX_DESC_SPLCP(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+12, 8, 1)
#define GET_RX_DESC_BW(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc+12, 9, 1)
#define GET_RX_DESC_HTC(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+12, 10, 1)
#define GET_RX_STATUS_DESC_EOSP(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+12, 11, 1)
#define GET_RX_STATUS_DESC_BSSID_FIT(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+12, 12, 2)
#define GET_RX_STATUS_DESC_RPT_SEL(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+12, 14, 2)

#define GET_RX_STATUS_DESC_PATTERN_MATCH(__pdesc)	\
	LE_BITS_TO_4BYTE(__pdesc+12, 29, 1)
#define GET_RX_STATUS_DESC_UNICAST_MATCH(__pdesc)	\
	LE_BITS_TO_4BYTE(__pdesc+12, 30, 1)
#define GET_RX_STATUS_DESC_MAGIC_MATCH(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+12, 31, 1)

#define GET_RX_DESC_IV1(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+16, 0, 32)
#define GET_RX_DESC_TSFL(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+20, 0, 32)

#define GET_RX_DESC_BUFF_ADDR(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+24, 0, 32)
#define GET_RX_DESC_BUFF_ADDR64(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+28, 0, 32)

#define SET_RX_DESC_BUFF_ADDR(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+24, 0, 32, __val)
#define SET_RX_DESC_BUFF_ADDR64(__pdesc, __val) \
	SET_BITS_TO_LE_4BYTE(__pdesc+28, 0, 32, __val)

/* TX report 2 format in Rx desc*/

#define GET_RX_RPT2_DESC_PKT_LEN(__status)	\
	LE_BITS_TO_4BYTE(__status, 0, 9)
#define GET_RX_RPT2_DESC_MACID_VALID_1(__status)	\
	LE_BITS_TO_4BYTE(__status+16, 0, 32)
#define GET_RX_RPT2_DESC_MACID_VALID_2(__status)	\
	LE_BITS_TO_4BYTE(__status+20, 0, 32)

#define SET_EARLYMODE_PKTNUM(__paddr, __value)	\
	SET_BITS_TO_LE_4BYTE(__paddr, 0, 4, __value)
#define SET_EARLYMODE_LEN0(__paddr, __value)	\
	SET_BITS_TO_LE_4BYTE(__paddr, 4, 12, __value)
#define SET_EARLYMODE_LEN1(__paddr, __value)	\
	SET_BITS_TO_LE_4BYTE(__paddr, 16, 12, __value)
#define SET_EARLYMODE_LEN2_1(__paddr, __value)	\
	SET_BITS_TO_LE_4BYTE(__paddr, 28, 4, __value)
#define SET_EARLYMODE_LEN2_2(__paddr, __value)	\
	SET_BITS_TO_LE_4BYTE(__paddr+4, 0, 8, __value)
#define SET_EARLYMODE_LEN3(__paddr, __value)	\
	SET_BITS_TO_LE_4BYTE(__paddr+4, 8, 12, __value)
#define SET_EARLYMODE_LEN4(__paddr, __value)	\
	SET_BITS_TO_LE_4BYTE(__paddr+4, 20, 12, __value)

#define CLEAR_PCI_TX_DESC_CONTENT(__pdesc, _size)		\
do {								\
	if (_size > TX_DESC_NEXT_DESC_OFFSET)			\
		memset(__pdesc, 0, TX_DESC_NEXT_DESC_OFFSET);	\
	else							\
		memset(__pdesc, 0, _size);			\
} while (0)
=======
static inline void set_tx_desc_pkt_size(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, GENMASK(15, 0));
}

static inline void set_tx_desc_offset(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, GENMASK(23, 16));
}

static inline void set_tx_desc_bmc(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, BIT(24));
}

static inline void set_tx_desc_htc(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, BIT(25));
}

static inline void set_tx_desc_last_seg(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, BIT(26));
}

static inline void set_tx_desc_first_seg(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, BIT(27));
}

static inline void set_tx_desc_linip(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, BIT(28));
}

static inline void set_tx_desc_own(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, BIT(31));
}

static inline int get_tx_desc_own(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), BIT(31));
}

static inline void set_tx_desc_macid(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 1, __val, GENMASK(5, 0));
}

static inline void set_tx_desc_queue_sel(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 1, __val, GENMASK(12, 8));
}

static inline void set_tx_desc_rate_id(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 1, __val, GENMASK(19, 16));
}

static inline void set_tx_desc_nav_use_hdr(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 1, __val, BIT(20));
}

static inline void set_tx_desc_sec_type(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 1, __val, GENMASK(23, 22));
}

static inline void set_tx_desc_pkt_offset(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 1, __val, GENMASK(30, 26));
}

static inline void set_tx_desc_agg_enable(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 2, __val, BIT(12));
}

static inline void set_tx_desc_rdg_enable(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 2, __val, BIT(13));
}

static inline void set_tx_desc_more_frag(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 2, __val, BIT(17));
}

static inline void set_tx_desc_ampdu_density(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 2, __val, GENMASK(22, 20));
}

static inline void set_tx_desc_antsel_a(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 2, __val, BIT(24));
}

static inline void set_tx_desc_antsel_b(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 2, __val, BIT(25));
}

static inline void set_tx_desc_seq(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 3, __val, GENMASK(27, 16));
}

static inline void set_tx_desc_hwseq_en(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 3, __val, BIT(31));
}

static inline void set_tx_desc_rts_rate(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, GENMASK(4, 0));
}

static inline void set_tx_desc_qos(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, BIT(6));
}

static inline void set_tx_desc_use_rate(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, BIT(8));
}

static inline void set_tx_desc_disable_fb(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, BIT(10));
}

static inline void set_tx_desc_cts2self(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, BIT(11));
}

static inline void set_tx_desc_rts_enable(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, BIT(12));
}

static inline void set_tx_desc_hw_rts_enable(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, BIT(13));
}

static inline void set_tx_desc_tx_sub_carrier(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, GENMASK(21, 20));
}

static inline void set_tx_desc_tx_stbc(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, GENMASK(23, 22));
}

static inline void set_tx_desc_data_bw(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, BIT(25));
}

static inline void set_tx_desc_rts_short(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, BIT(26));
}

static inline void set_tx_desc_rts_bw(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, BIT(27));
}

static inline void set_tx_desc_rts_sc(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, GENMASK(29, 28));
}

static inline void set_tx_desc_rts_stbc(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 4, __val, GENMASK(31, 30));
}

static inline void set_tx_desc_tx_rate(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 5, __val, GENMASK(5, 0));
}

static inline void set_tx_desc_data_shortgi(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 5, __val, BIT(6));
}

static inline void set_tx_desc_data_rate_fb_limit(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 5, __val, GENMASK(12, 8));
}

static inline void set_tx_desc_rts_rate_fb_limit(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 5, __val, GENMASK(16, 13));
}

static inline void set_tx_desc_max_agg_num(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 6, __val, GENMASK(15, 11));
}

static inline void set_tx_desc_antsel_c(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 7, __val, BIT(29));
}

static inline void set_tx_desc_tx_buffer_size(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc + 7, __val, GENMASK(15, 0));
}

static inline int get_tx_desc_tx_buffer_size(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 7), GENMASK(15, 0));
}

static inline void set_tx_desc_tx_buffer_address(__le32 *__pdesc, u32 __val)
{
	*(__pdesc + 8) = cpu_to_le32(__val);
}

static inline u32 get_tx_desc_tx_buffer_address(__le32 *__pdesc)
{
	return le32_to_cpu(*(__pdesc + 8));
}

static inline void set_tx_desc_next_desc_address(__le32 *__pdesc, u32 __val)
{
	*(__pdesc + 10) = cpu_to_le32(__val);
}

static inline int get_rx_desc_pkt_len(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), GENMASK(13, 0));
}

static inline int get_rx_desc_crc32(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), BIT(14));
}

static inline int get_rx_desc_icv(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), BIT(15));
}

static inline int get_rx_desc_drv_info_size(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), GENMASK(19, 16));
}

static inline int get_rx_desc_security(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), GENMASK(22, 20));
}

static inline int get_rx_desc_qos(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), BIT(23));
}

static inline int get_rx_desc_shift(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), GENMASK(25, 24));
}

static inline int get_rx_desc_physt(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), BIT(26));
}

static inline int get_rx_desc_swdec(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), BIT(27));
}

static inline int get_rx_desc_ls(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), BIT(28));
}

static inline int get_rx_desc_fs(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), BIT(29));
}

static inline int get_rx_desc_eor(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), BIT(30));
}

static inline int get_rx_desc_own(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc), BIT(31));
}

static inline void set_rx_desc_pkt_len(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, GENMASK(13, 0));
}

static inline void set_rx_desc_eor(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, BIT(30));
}

static inline void set_rx_desc_own(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, BIT(31));
}

static inline int get_rx_desc_macid(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), GENMASK(5, 0));
}

static inline int get_rx_desc_paggr(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), BIT(14));
}

static inline int get_rx_desc_faggr(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), BIT(15));
}

static inline int get_rx_desc_a1_fit(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), GENMASK(19, 16));
}

static inline int get_rx_desc_a2_fit(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), GENMASK(23, 20));
}

static inline int get_rx_desc_pam(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), BIT(24));
}

static inline int get_rx_desc_pwr(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), BIT(25));
}

static inline int get_rx_desc_md(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), BIT(26));
}

static inline int get_rx_desc_mf(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), BIT(27));
}

static inline int get_rx_desc_type(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), GENMASK(29, 28));
}

static inline int get_rx_desc_mc(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), BIT(30));
}

static inline int get_rx_desc_bc(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), BIT(31));
}

static inline int get_rx_desc_seq(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 2), GENMASK(11, 0));
}

static inline int get_rx_desc_frag(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 2), GENMASK(15, 12));
}

static inline int get_rx_desc_rxmcs(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 3), GENMASK(5, 0));
}

static inline int get_rx_desc_rxht(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 3), BIT(6));
}

static inline int get_rx_status_desc_rx_gf(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 3), BIT(7));
}

static inline int get_rx_desc_splcp(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 3), BIT(8));
}

static inline int get_rx_desc_bw(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 3), BIT(9));
}

static inline int get_rx_desc_htc(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 3), BIT(10));
}

static inline int get_rx_status_desc_eosp(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 3), BIT(11));
}

static inline int get_rx_status_desc_bssid_fit(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 3), GENMASK(13, 12));
}

static inline int get_rx_status_desc_rpt_sel(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 3), GENMASK(15, 14));
}

static inline int get_rx_status_desc_pattern_match(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 3), BIT(29));
}

static inline int get_rx_status_desc_unicast_match(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 3), BIT(30));
}

static inline int get_rx_status_desc_magic_match(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 3), BIT(31));
}

static inline u32 get_rx_desc_iv1(__le32 *__pdesc)
{
	return le32_to_cpu(*(__pdesc + 4));
}

static inline u32 get_rx_desc_tsfl(__le32 *__pdesc)
{
	return le32_to_cpu(*(__pdesc + 5));
}

static inline u32 get_rx_desc_buff_addr(__le32 *__pdesc)
{
	return le32_to_cpu(*(__pdesc + 6));
}

static inline u32 get_rx_desc_buff_addr64(__le32 *__pdesc)
{
	return le32_to_cpu(*(__pdesc + 7));
}

static inline void set_rx_desc_buff_addr(__le32 *__pdesc, u32 __val)
{
	*(__pdesc + 6) = cpu_to_le32(__val);
}

static inline void set_rx_desc_buff_addr64(__le32 *__pdesc, u32 __val)
{
	*(__pdesc + 7) = cpu_to_le32(__val);
}

/* TX report 2 format in Rx desc*/

static inline int get_rx_rpt2_desc_pkt_len(__le32 *__status)
{
	return le32_get_bits(*(__status), GENMASK(8, 0));
}

static inline u32 get_rx_rpt2_desc_macid_valid_1(__le32 *__status)
{
	return le32_to_cpu(*(__status + 4));
}

static inline u32 get_rx_rpt2_desc_macid_valid_2(__le32 *__status)
{
	return le32_to_cpu(*(__status + 5));
}

static inline void set_earlymode_pktnum(__le32 *__paddr, u32 __value)
{
	le32p_replace_bits(__paddr, __value, GENMASK(3, 0));
}

static inline void set_earlymode_len0(__le32 *__paddr, u32 __value)
{
	le32p_replace_bits(__paddr, __value, GENMASK(15, 4));
}

static inline void set_earlymode_len1(__le32 *__paddr, u32 __value)
{
	le32p_replace_bits(__paddr, __value, GENMASK(27, 16));
}

static inline void set_earlymode_len2_1(__le32 *__paddr, u32 __value)
{
	le32p_replace_bits(__paddr, __value, GENMASK(31, 28));
}

static inline void set_earlymode_len2_2(__le32 *__paddr, u32 __value)
{
	le32p_replace_bits(__paddr + 1, __value, GENMASK(7, 0));
}

static inline void set_earlymode_len3(__le32 *__paddr, u32 __value)
{
	le32p_replace_bits(__paddr + 1, __value, GENMASK(19, 8));
}

static inline void set_earlymode_len4(__le32 *__paddr, u32 __value)
{
	le32p_replace_bits(__paddr + 1, __value, GENMASK(31, 20));
}

static inline void clear_pci_tx_desc_content(__le32 *__pdesc, int _size)
{
	if (_size > TX_DESC_NEXT_DESC_OFFSET)
		memset(__pdesc, 0, TX_DESC_NEXT_DESC_OFFSET);
	else
		memset(__pdesc, 0, _size);
}
>>>>>>> upstream/android-13

#define RTL8188_RX_HAL_IS_CCK_RATE(rxmcs)\
	(rxmcs == DESC92C_RATE1M ||\
	 rxmcs == DESC92C_RATE2M ||\
	 rxmcs == DESC92C_RATE5_5M ||\
	 rxmcs == DESC92C_RATE11M)

<<<<<<< HEAD
#define IS_LITTLE_ENDIAN	1

struct phy_rx_agc_info_t {
	#if IS_LITTLE_ENDIAN
		u8	gain:7, trsw:1;
	#else
		u8	trsw:1, gain:7;
	#endif
};
struct phy_status_rpt {
	struct phy_rx_agc_info_t path_agc[2];
=======
struct phy_status_rpt {
	u8	padding[2];
>>>>>>> upstream/android-13
	u8	ch_corr[2];
	u8	cck_sig_qual_ofdm_pwdb_all;
	u8	cck_agc_rpt_ofdm_cfosho_a;
	u8	cck_rpt_b_ofdm_cfosho_b;
	u8	rsvd_1;/* ch_corr_msb; */
	u8	noise_power_db_msb;
	u8	path_cfotail[2];
	u8	pcts_mask[2];
	u8	stream_rxevm[2];
	u8	path_rxsnr[2];
	u8	noise_power_db_lsb;
	u8	rsvd_2[3];
	u8	stream_csi[2];
	u8	stream_target_csi[2];
	u8	sig_evm;
	u8	rsvd_3;
<<<<<<< HEAD
#if IS_LITTLE_ENDIAN
=======
#if defined(__LITTLE_ENDIAN)
>>>>>>> upstream/android-13
	u8	antsel_rx_keep_2:1;	/*ex_intf_flg:1;*/
	u8	sgi_en:1;
	u8	rxsc:2;
	u8	idle_long:1;
	u8	r_ant_train_en:1;
	u8	ant_sel_b:1;
	u8	ant_sel:1;
<<<<<<< HEAD
#else	/* _BIG_ENDIAN_	*/
=======
#else	/* __BIG_ENDIAN	*/
>>>>>>> upstream/android-13
	u8	ant_sel:1;
	u8	ant_sel_b:1;
	u8	r_ant_train_en:1;
	u8	idle_long:1;
	u8	rxsc:2;
	u8	sgi_en:1;
	u8	antsel_rx_keep_2:1;	/*ex_intf_flg:1;*/
#endif
} __packed;

struct rx_fwinfo_88e {
	u8 gain_trsw[4];
	u8 pwdb_all;
	u8 cfosho[4];
	u8 cfotail[4];
	s8 rxevm[2];
	s8 rxsnr[4];
	u8 pdsnr[2];
	u8 csi_current[2];
	u8 csi_target[2];
	u8 sigevm;
	u8 max_ex_pwr;
	u8 ex_intf_flag:1;
	u8 sgi_en:1;
	u8 rxsc:2;
	u8 reserve:4;
} __packed;

struct tx_desc_88e {
	u32 pktsize:16;
	u32 offset:8;
	u32 bmc:1;
	u32 htc:1;
	u32 lastseg:1;
	u32 firstseg:1;
	u32 linip:1;
	u32 noacm:1;
	u32 gf:1;
	u32 own:1;

	u32 macid:6;
	u32 rsvd0:2;
	u32 queuesel:5;
	u32 rd_nav_ext:1;
	u32 lsig_txop_en:1;
	u32 pifs:1;
	u32 rateid:4;
	u32 nav_usehdr:1;
	u32 en_descid:1;
	u32 sectype:2;
	u32 pktoffset:8;

	u32 rts_rc:6;
	u32 data_rc:6;
	u32 agg_en:1;
	u32 rdg_en:1;
	u32 bar_retryht:2;
	u32 agg_break:1;
	u32 morefrag:1;
	u32 raw:1;
	u32 ccx:1;
	u32 ampdudensity:3;
	u32 bt_int:1;
	u32 ant_sela:1;
	u32 ant_selb:1;
	u32 txant_cck:2;
	u32 txant_l:2;
	u32 txant_ht:2;

	u32 nextheadpage:8;
	u32 tailpage:8;
	u32 seq:12;
	u32 cpu_handle:1;
	u32 tag1:1;
	u32 trigger_int:1;
	u32 hwseq_en:1;

	u32 rtsrate:5;
	u32 apdcfe:1;
	u32 qos:1;
	u32 hwseq_ssn:1;
	u32 userrate:1;
	u32 dis_rtsfb:1;
	u32 dis_datafb:1;
	u32 cts2self:1;
	u32 rts_en:1;
	u32 hwrts_en:1;
	u32 portid:1;
	u32 pwr_status:3;
	u32 waitdcts:1;
	u32 cts2ap_en:1;
	u32 txsc:2;
	u32 stbc:2;
	u32 txshort:1;
	u32 txbw:1;
	u32 rtsshort:1;
	u32 rtsbw:1;
	u32 rtssc:2;
	u32 rtsstbc:2;

	u32 txrate:6;
	u32 shortgi:1;
	u32 ccxt:1;
	u32 txrate_fb_lmt:5;
	u32 rtsrate_fb_lmt:4;
	u32 retrylmt_en:1;
	u32 txretrylmt:6;
	u32 usb_txaggnum:8;

	u32 txagca:5;
	u32 txagcb:5;
	u32 usemaxlen:1;
	u32 maxaggnum:5;
	u32 mcsg1maxlen:4;
	u32 mcsg2maxlen:4;
	u32 mcsg3maxlen:4;
	u32 mcs7sgimaxlen:4;

	u32 txbuffersize:16;
	u32 sw_offset30:8;
	u32 sw_offset31:4;
	u32 rsvd1:1;
	u32 antsel_c:1;
	u32 null_0:1;
	u32 null_1:1;

	u32 txbuffaddr;
	u32 txbufferaddr64;
	u32 nextdescaddress;
	u32 nextdescaddress64;

	u32 reserve_pass_pcie_mm_limit[4];
} __packed;

struct rx_desc_88e {
	u32 length:14;
	u32 crc32:1;
	u32 icverror:1;
	u32 drv_infosize:4;
	u32 security:3;
	u32 qos:1;
	u32 shift:2;
	u32 phystatus:1;
	u32 swdec:1;
	u32 lastseg:1;
	u32 firstseg:1;
	u32 eor:1;
	u32 own:1;

	u32 macid:6;
	u32 tid:4;
	u32 hwrsvd:5;
	u32 paggr:1;
	u32 faggr:1;
	u32 a1_fit:4;
	u32 a2_fit:4;
	u32 pam:1;
	u32 pwr:1;
	u32 moredata:1;
	u32 morefrag:1;
	u32 type:2;
	u32 mc:1;
	u32 bc:1;

	u32 seq:12;
	u32 frag:4;
	u32 nextpktlen:14;
	u32 nextind:1;
	u32 rsvd:1;

	u32 rxmcs:6;
	u32 rxht:1;
	u32 amsdu:1;
	u32 splcp:1;
	u32 bandwidth:1;
	u32 htc:1;
	u32 tcpchk_rpt:1;
	u32 ipcchk_rpt:1;
	u32 tcpchk_valid:1;
	u32 hwpcerr:1;
	u32 hwpcind:1;
	u32 iv0:16;

	u32 iv1;

	u32 tsfl;

	u32 bufferaddress;
	u32 bufferaddress64;

} __packed;

void rtl88ee_tx_fill_desc(struct ieee80211_hw *hw,
			  struct ieee80211_hdr *hdr, u8 *pdesc_tx,
			  u8 *txbd, struct ieee80211_tx_info *info,
			  struct ieee80211_sta *sta,
			  struct sk_buff *skb,
			  u8 hw_queue, struct rtl_tcb_desc *ptcb_desc);
bool rtl88ee_rx_query_desc(struct ieee80211_hw *hw,
			   struct rtl_stats *status,
			   struct ieee80211_rx_status *rx_status,
			   u8 *pdesc, struct sk_buff *skb);
void rtl88ee_set_desc(struct ieee80211_hw *hw, u8 *pdesc,
		      bool istx, u8 desc_name, u8 *val);
u64 rtl88ee_get_desc(struct ieee80211_hw *hw,
		     u8 *pdesc, bool istx, u8 desc_name);
bool rtl88ee_is_tx_desc_closed(struct ieee80211_hw *hw,
			       u8 hw_queue, u16 index);
void rtl88ee_tx_polling(struct ieee80211_hw *hw, u8 hw_queue);
void rtl88ee_tx_fill_cmddesc(struct ieee80211_hw *hw, u8 *pdesc,
			     bool firstseg, bool lastseg,
			     struct sk_buff *skb);
#endif
