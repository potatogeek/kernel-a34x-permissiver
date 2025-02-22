<<<<<<< HEAD
/******************************************************************************
 *
 * Copyright(c) 2009-2014  Realtek Corporation.
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
/* Copyright(c) 2009-2014  Realtek Corporation.*/
>>>>>>> upstream/android-13

#ifndef __RTL92E_TRX_H__
#define __RTL92E_TRX_H__

#define TX_DESC_SIZE					64

#define RX_DRV_INFO_SIZE_UNIT				8

#define	TX_DESC_NEXT_DESC_OFFSET			40
#define USB_HWDESC_HEADER_LEN				40

#define RX_DESC_SIZE					24
#define MAX_RECEIVE_BUFFER_SIZE				8192

<<<<<<< HEAD
#define SET_TX_DESC_PKT_SIZE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc, 0, 16, __val)
#define SET_TX_DESC_OFFSET(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc, 16, 8, __val)
#define SET_TX_DESC_BMC(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc, 24, 1, __val)
#define SET_TX_DESC_HTC(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc, 25, 1, __val)
#define SET_TX_DESC_LAST_SEG(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc, 26, 1, __val)
#define SET_TX_DESC_FIRST_SEG(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc, 27, 1, __val)
#define SET_TX_DESC_LINIP(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc, 28, 1, __val)
#define SET_TX_DESC_NO_ACM(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc, 29, 1, __val)
#define SET_TX_DESC_GF(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc, 30, 1, __val)
#define SET_TX_DESC_OWN(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc, 31, 1, __val)

#define GET_TX_DESC_PKT_SIZE(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 0, 16)
#define GET_TX_DESC_OFFSET(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 16, 8)
#define GET_TX_DESC_BMC(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 24, 1)
#define GET_TX_DESC_HTC(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 25, 1)
#define GET_TX_DESC_LAST_SEG(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 26, 1)
#define GET_TX_DESC_FIRST_SEG(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 27, 1)
#define GET_TX_DESC_LINIP(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 28, 1)
#define GET_TX_DESC_NO_ACM(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 29, 1)
#define GET_TX_DESC_GF(__pdesc)				\
	LE_BITS_TO_4BYTE(__pdesc, 30, 1)
#define GET_TX_DESC_OWN(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc, 31, 1)

#define SET_TX_DESC_MACID(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 0, 7, __val)
#define SET_TX_DESC_QUEUE_SEL(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 8, 5, __val)
#define SET_TX_DESC_RDG_NAV_EXT(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 13, 1, __val)
#define SET_TX_DESC_LSIG_TXOP_EN(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 14, 1, __val)
#define SET_TX_DESC_PIFS(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 15, 1, __val)
#define SET_TX_DESC_RATE_ID(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 16, 5, __val)
#define SET_TX_DESC_EN_DESC_ID(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 21, 1, __val)
#define SET_TX_DESC_SEC_TYPE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 22, 2, __val)
#define SET_TX_DESC_PKT_OFFSET(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 24, 5, __val)
#define SET_TX_DESC_MORE_DATA(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 29, 1, __val)
#define SET_TX_DESC_TXOP_PS_CAP(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 30, 1, __val)
#define SET_TX_DESC_TXOP_PS_MODE(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+4, 31, 1, __val)

#define GET_TX_DESC_MACID(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 0, 5)
#define GET_TX_DESC_AGG_ENABLE(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 5, 1)
#define GET_TX_DESC_AGG_BREAK(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 6, 1)
#define GET_TX_DESC_RDG_ENABLE(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 7, 1)
#define GET_TX_DESC_QUEUE_SEL(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 8, 5)
#define GET_TX_DESC_RDG_NAV_EXT(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+4, 13, 1)
#define GET_TX_DESC_LSIG_TXOP_EN(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+4, 14, 1)
#define GET_TX_DESC_PIFS(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 15, 1)
#define GET_TX_DESC_RATE_ID(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 16, 4)
#define GET_TX_DESC_NAV_USE_HDR(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+4, 20, 1)
#define GET_TX_DESC_EN_DESC_ID(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 21, 1)
#define GET_TX_DESC_SEC_TYPE(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 22, 2)
#define GET_TX_DESC_PKT_OFFSET(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 24, 5)

#define SET_TX_DESC_PAID(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 0, 9, __val)
#define SET_TX_DESC_CCA_RTS(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 10, 2, __val)
#define SET_TX_DESC_AGG_ENABLE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 12, 1, __val)
#define SET_TX_DESC_RDG_ENABLE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 13, 1, __val)
#define SET_TX_DESC_NULL_0(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 14, 1, __val)
#define SET_TX_DESC_NULL_1(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE((__pdesc) + 8, 15, 1, __val)
#define SET_TX_DESC_BK(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 16, 1, __val)
#define SET_TX_DESC_MORE_FRAG(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 17, 1, __val)
#define SET_TX_DESC_RAW(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 18, 1, __val)
#define SET_TX_DESC_SPE_RPT(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE((__pdesc) + 8, 19, 1, __val)
#define SET_TX_DESC_AMPDU_DENSITY(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 20, 3, __val)
#define SET_TX_DESC_BT_NULL(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 23, 1, __val)
#define SET_TX_DESC_GID(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+8, 24, 6, __val)

#define SET_TX_DESC_WHEADER_LEN(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 0, 4, __val)
#define SET_TX_DESC_CHK_EN(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 4, 1, __val)
#define SET_TX_DESC_EARLY_RATE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 5, 1, __val)
#define SET_TX_DESC_HWSEQ_SEL(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 6, 2, __val)
#define SET_TX_DESC_USE_RATE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 8, 1, __val)
#define SET_TX_DESC_DISABLE_RTS_FB(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 9, 1, __val)
#define SET_TX_DESC_DISABLE_FB(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 10, 1, __val)
#define SET_TX_DESC_CTS2SELF(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 11, 1, __val)
#define SET_TX_DESC_RTS_ENABLE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 12, 1, __val)
#define SET_TX_DESC_HW_RTS_ENABLE(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 13, 1, __val)
#define SET_TX_DESC_HW_PORT_ID(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 14, 1, __val)
#define SET_TX_DESC_NAV_USE_HDR(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 15, 1, __val)
#define SET_TX_DESC_USE_MAX_LEN(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 16, 1, __val)
#define SET_TX_DESC_MAX_AGG_NUM(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 17, 5, __val)
#define SET_TX_DESC_NDPA(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 22, 2, __val)
#define SET_TX_DESC_AMPDU_MAX_TIME(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+12, 24, 8, __val)

/* Dword 4 */
#define SET_TX_DESC_TX_RATE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 0, 7, __val)
#define SET_TX_DESC_TRY_RATE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 7, 1, __val)
#define SET_TX_DESC_DATA_RATE_FB_LIMIT(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 8, 5, __val)
#define SET_TX_DESC_RTS_RATE_FB_LIMIT(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 13, 4, __val)
#define SET_TX_DESC_RETRY_LIMIT_ENABLE(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 17, 1, __val)
#define SET_TX_DESC_DATA_RETRY_LIMIT(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 18, 6, __val)
#define SET_TX_DESC_RTS_RATE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 24, 5, __val)
#define SET_TX_DESC_PCTS_ENABLE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 29, 1, __val)
#define SET_TX_DESC_PCTS_MASK_IDX(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+16, 30, 2, __val)

/* Dword 5 */
#define SET_TX_DESC_TX_SUB_CARRIER(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 0, 4, __val)
#define SET_TX_DESC_DATA_SHORT(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 4, 1, __val)
#define SET_TX_DESC_DATA_BW(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 5, 2, __val)
#define SET_TX_DESC_DATA_LDPC(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 7, 1, __val)
#define SET_TX_DESC_DATA_STBC(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 8, 2, __val)
#define SET_TX_DESC_VCS_STBC(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 10, 2, __val)
#define SET_TX_DESC_RTS_SHORT(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 12, 1, __val)
#define SET_TX_DESC_RTS_SC(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 13, 4, __val)
#define SET_TX_DESC_TX_ANT(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 24, 4, __val)
#define SET_TX_DESC_TX_POWER_0_PSET(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+20, 28, 3, __val)

/* Dword 6 */
#define SET_TX_DESC_SW_DEFINE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE((__pdesc) + 24, 0, 12, __val)
#define SET_TX_DESC_ANTSEL_A(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE((__pdesc) + 24, 16, 3, __val)
#define SET_TX_DESC_ANTSEL_B(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE((__pdesc) + 24, 19, 3, __val)
#define SET_TX_DESC_ANTSEL_C(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE((__pdesc) + 24, 22, 3, __val)
#define SET_TX_DESC_ANTSEL_D(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE((__pdesc) + 24, 25, 3, __val)

/* Dword 7 */
#define SET_TX_DESC_TX_BUFFER_SIZE(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+28, 0, 16, __val)
#define SET_TX_DESC_USB_TXAGG_NUM(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+28, 24, 8, __val)

/* Dword 8 */
#define SET_TX_DESC_RTS_RC(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+32, 0, 6, __val)
#define SET_TX_DESC_BAR_RTY_TH(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+32, 6, 2, __val)
#define SET_TX_DESC_DATA_RC(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+32, 8, 6, __val)
#define SET_TX_DESC_ENABLE_HW_SELECT(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+32, 15, 1, __val)
#define SET_TX_DESC_NEXT_HEAD_PAGE(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+32, 16, 8, __val)
#define SET_TX_DESC_TAIL_PAGE(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+32, 24, 8, __val)

/* Dword 9 */
#define SET_TX_DESC_PADDING_LENGTH(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+36, 0, 11, __val)
#define SET_TX_DESC_TXBF_PATH(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+36, 11, 1, __val)
#define SET_TX_DESC_SEQ(__pdesc, __val)			\
	SET_BITS_TO_LE_4BYTE(__pdesc+36, 12, 12, __val)
#define SET_TX_DESC_FINAL_DATA_RATE(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+36, 24, 8, __val)

/* Dword 10 */
#define SET_TX_DESC_TX_BUFFER_ADDRESS(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+40, 0, 32, __val)

/* Dword 11*/
#define SET_TX_DESC_NEXT_DESC_ADDRESS(__pdesc, __val)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+48, 0, 32, __val)

#define SET_EARLYMODE_PKTNUM(__paddr, __val)		\
	SET_BITS_TO_LE_4BYTE(__paddr, 0, 4, __val)
#define SET_EARLYMODE_LEN0(__paddr, __val)		\
	SET_BITS_TO_LE_4BYTE(__paddr, 4, 15, __val)
#define SET_EARLYMODE_LEN1(__paddr, __val)		\
	SET_BITS_TO_LE_4BYTE(__paddr, 16, 2, __val)
#define SET_EARLYMODE_LEN1_1(__paddr, __val)		\
	SET_BITS_TO_LE_4BYTE(__paddr, 19, 13, __val)
#define SET_EARLYMODE_LEN1_2(__paddr, __val)		\
	SET_BITS_TO_LE_4BYTE(__paddr+4, 0, 2, __val)
#define SET_EARLYMODE_LEN2(__paddr, __val)		\
	SET_BITS_TO_LE_4BYTE(__paddr+4, 2, 15,  __val)
#define SET_EARLYMODE_LEN2_1(__paddr, __val)		\
	SET_BITS_TO_LE_4BYTE(__paddr, 2, 4,  __val)
#define SET_EARLYMODE_LEN2_2(__paddr, __val)		\
	SET_BITS_TO_LE_4BYTE(__paddr+4, 0, 8,  __val)
#define SET_EARLYMODE_LEN3(__paddr, __val)		\
	SET_BITS_TO_LE_4BYTE(__paddr+4, 17, 15, __val)
#define SET_EARLYMODE_LEN4(__paddr, __val)		\
	SET_BITS_TO_LE_4BYTE(__paddr+4, 20, 12, __val)

/* TX/RX buffer descriptor */

#define SET_TX_EXTBUFF_DESC_LEN(__pdesc, __val, __set)	\
	SET_BITS_TO_LE_4BYTE(__pdesc+(__set*16), 0, 16, __val)
#define SET_TX_EXTBUFF_DESC_ADDR_LOW(__pdesc, __val, __set)\
	SET_BITS_TO_LE_4BYTE(__pdesc+(__set*16)+4, 0, 32, __val)
#define SET_TX_EXTBUFF_DESC_ADDR_HIGH(__pdesc, __val, __set)\
	SET_BITS_TO_LE_4BYTE(__pdesc+(__set*16)+8, 0, 32, __val)

/* for Txfilldescroptor92ee, fill the desc content. */
#define SET_TXBUFFER_DESC_LEN_WITH_OFFSET(__pdesc, __offset, __val)            \
	SET_BITS_TO_LE_4BYTE((__pdesc) + ((__offset) * 16), 0, 16, __val)
#define SET_TXBUFFER_DESC_AMSDU_WITH_OFFSET(__pdesc, __offset, __val)          \
	SET_BITS_TO_LE_4BYTE((__pdesc) + ((__offset) * 16), 31, 1, __val)
#define SET_TXBUFFER_DESC_ADD_LOW_WITH_OFFSET(__pdesc, __offset, __val)        \
	SET_BITS_TO_LE_4BYTE((__pdesc) + ((__offset) * 16) + 4, 0, 32, __val)
#define SET_TXBUFFER_DESC_ADD_HIGH_WITH_OFFSET(pbd, off, val, dma64)	       \
	(dma64 ? SET_BITS_TO_LE_4BYTE((pbd) + ((off) * 16) + 8, 0, 32, val) : 0)
#define GET_TXBUFFER_DESC_ADDR_LOW(__pdesc, __offset)                          \
	LE_BITS_TO_4BYTE((__pdesc) + ((__offset) * 16) + 4, 0, 32)
#define GET_TXBUFFER_DESC_ADDR_HIGH(pbd, off, dma64)			       \
	(dma64 ? LE_BITS_TO_4BYTE((pbd) + ((off) * 16) + 8, 0, 32) : 0)

/* Dword 0 */
#define SET_TX_BUFF_DESC_LEN_0(__pdesc, __val)                                 \
	SET_BITS_TO_LE_4BYTE(__pdesc, 0, 14, __val)
#define SET_TX_BUFF_DESC_PSB(__pdesc, __val)                                   \
	SET_BITS_TO_LE_4BYTE(__pdesc, 16, 15, __val)
#define SET_TX_BUFF_DESC_OWN(__pdesc, __val)                                   \
	SET_BITS_TO_LE_4BYTE(__pdesc, 31, 1, __val)

/* Dword 1 */
#define SET_TX_BUFF_DESC_ADDR_LOW_0(__pdesc, __val)                            \
	SET_BITS_TO_LE_4BYTE((__pdesc) + 4, 0, 32, __val)
/* Dword 2 */
#define SET_TX_BUFF_DESC_ADDR_HIGH_0(bdesc, val, dma64)			       \
	SET_TXBUFFER_DESC_ADD_HIGH_WITH_OFFSET(bdesc, 0, val, dma64)
/* Dword 3 / RESERVED 0 */
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
	le32p_replace_bits((__pdesc + 1), __val, GENMASK(6, 0));
}

static inline void set_tx_desc_queue_sel(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 1), __val, GENMASK(12, 8));
}

static inline void set_tx_desc_rate_id(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 1), __val, GENMASK(20, 16));
}

static inline void set_tx_desc_sec_type(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 1), __val, GENMASK(23, 22));
}

static inline void set_tx_desc_pkt_offset(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 1), __val, GENMASK(28, 24));
}

static inline void set_tx_desc_agg_enable(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 2), __val, BIT(12));
}

static inline void set_tx_desc_rdg_enable(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 2), __val, BIT(13));
}

static inline void set_tx_desc_more_frag(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 2), __val, BIT(17));
}

static inline void set_tx_desc_ampdu_density(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 2), __val, GENMASK(22, 20));
}

static inline void set_tx_desc_use_rate(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 3), __val, BIT(8));
}

static inline void set_tx_desc_disable_fb(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 3), __val, BIT(10));
}

static inline void set_tx_desc_cts2self(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 3), __val, BIT(11));
}

static inline void set_tx_desc_rts_enable(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 3), __val, BIT(12));
}

static inline void set_tx_desc_hw_rts_enable(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 3), __val, BIT(13));
}

static inline void set_tx_desc_nav_use_hdr(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 3), __val, BIT(15));
}

static inline void set_tx_desc_max_agg_num(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 3), __val, GENMASK(21, 17));
}

/* Dword 4 */
static inline void set_tx_desc_tx_rate(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 4), __val, GENMASK(6, 0));
}

static inline void set_tx_desc_data_rate_fb_limit(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 4), __val, GENMASK(12, 8));
}

static inline void set_tx_desc_rts_rate_fb_limit(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 4), __val, GENMASK(16, 13));
}

static inline void set_tx_desc_rts_rate(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 4), __val, GENMASK(28, 24));
}

/* Dword 5 */
static inline void set_tx_desc_tx_sub_carrier(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 5), __val, GENMASK(3, 0));
}

static inline void set_tx_desc_data_bw(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 4), __val, GENMASK(6, 5));
}

static inline void set_tx_desc_rts_short(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 5), __val, BIT(12));
}

static inline void set_tx_desc_rts_sc(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 5), __val, GENMASK(16, 13));
}

/* Dword 7 */
static inline void set_tx_desc_tx_buffer_size(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 7), __val, GENMASK(15, 0));
}

/* Dword 9 */
static inline void set_tx_desc_seq(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits((__pdesc + 9), __val, GENMASK(23, 12));
}

/* Dword 10 */
static inline void set_tx_desc_tx_buffer_address(__le32 *__pdesc, u32 __val)
{
	*(__pdesc + 10) = cpu_to_le32(__val);
}

/* Dword 11*/
static inline void set_tx_desc_next_desc_address(__le32 *__pdesc, u32 __val)
{
	*(__pdesc + 12) = cpu_to_le32(__val);
}

static inline void set_earlymode_pktnum(__le32 *__paddr, u32 __val)
{
	le32p_replace_bits(__paddr, __val, GENMASK(3, 0));
}

static inline void set_earlymode_len0(__le32 *__paddr, u32 __val)
{
	le32p_replace_bits(__paddr, __val, GENMASK(18, 4));
}

static inline void set_earlymode_len1(__le32 *__paddr, u32 __val)
{
	le32p_replace_bits(__paddr, __val, GENMASK(17, 16));
}

static inline void set_earlymode_len2_1(__le32 *__paddr, u32 __val)
{
	le32p_replace_bits(__paddr, __val, GENMASK(5, 2));
}

static inline void set_earlymode_len2_2(__le32 *__paddr, u32 __val)
{
	le32p_replace_bits((__paddr + 1), __val, GENMASK(7, 0));
}

static inline void set_earlymode_len3(__le32 *__paddr, u32 __val)
{
	le32p_replace_bits((__paddr + 1), __val, GENMASK(31, 17));
}

static inline void set_earlymode_len4(__le32 *__paddr, u32 __val)
{
	le32p_replace_bits((__paddr + 1), __val, GENMASK(31, 20));
}

/* TX/RX buffer descriptor */

/* for Txfilldescroptor92ee, fill the desc content. */
static inline void set_txbuffer_desc_len_with_offset(__le32 *__pdesc,
						     u8 __offset, u32 __val)
{
	le32p_replace_bits((__pdesc + 4 * __offset), __val,
			   GENMASK(15, 0));
}

static inline void set_txbuffer_desc_amsdu_with_offset(__le32 *__pdesc,
						       u8 __offset, u32 __val)
{
	le32p_replace_bits((__pdesc + 4 * __offset), __val, BIT(31));
}

static inline void set_txbuffer_desc_add_low_with_offset(__le32 *__pdesc,
							 u8 __offset,
							 u32 __val)
{
	*(__pdesc + 4 * __offset + 1) = cpu_to_le32(__val);
}

static inline void set_txbuffer_desc_add_high_with_offset(__le32 *pbd, u8 off,
							  u32 val, bool dma64)
{
	if (dma64)
		*(pbd + 4 * off + 2) = cpu_to_le32(val);
	else
		*(pbd + 4 * off + 2) = 0;
}

static inline u32 get_txbuffer_desc_addr_low(__le32 *__pdesc, u8 __offset)
{
	return le32_to_cpu(*((__pdesc + 4 * __offset + 1)));
}

static inline u32 get_txbuffer_desc_addr_high(__le32 *pbd, u32 off, bool dma64)
{
	if (dma64)
		return le32_to_cpu(*((pbd + 4 * off + 2)));
	return 0;
}

/* Dword 0 */
static inline void set_tx_buff_desc_len_0(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, GENMASK(13, 0));
}

static inline void set_tx_buff_desc_psb(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, GENMASK(30, 16));
}

static inline void set_tx_buff_desc_own(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, BIT(31));
}

/* Dword 1 */
static inline void set_tx_buff_desc_addr_low_0(__le32 *__pdesc, u32 __val)
{
	*(__pdesc + 1) = cpu_to_le32(__val);
}

/* Dword 2 */
static inline void set_tx_buff_desc_addr_high_0(__le32 *pdesc, u32 val,
						bool dma64)
{
	if (dma64)
		*(pdesc + 2) = cpu_to_le32(val);
	else
		*(pdesc + 2) = 0;
}
>>>>>>> upstream/android-13

/* RX buffer  */

/* DWORD 0 */
<<<<<<< HEAD
#define SET_RX_BUFFER_DESC_DATA_LENGTH(__status, __val)	\
	SET_BITS_TO_LE_4BYTE(__status, 0, 14, __val)
#define SET_RX_BUFFER_DESC_LS(__status, __val)		\
	SET_BITS_TO_LE_4BYTE(__status, 15, 1, __val)
#define SET_RX_BUFFER_DESC_FS(__status, __val)		\
	SET_BITS_TO_LE_4BYTE(__status, 16, 1, __val)
#define SET_RX_BUFFER_DESC_TOTAL_LENGTH(__status, __val)	\
	SET_BITS_TO_LE_4BYTE(__status, 16, 15, __val)

#define GET_RX_BUFFER_DESC_OWN(__status)			\
	LE_BITS_TO_4BYTE(__status, 31, 1)
#define GET_RX_BUFFER_DESC_LS(__status)			\
	LE_BITS_TO_4BYTE(__status, 15, 1)
#define GET_RX_BUFFER_DESC_FS(__status)			\
	LE_BITS_TO_4BYTE(__status, 16, 1)
#define GET_RX_BUFFER_DESC_TOTAL_LENGTH(__status)	\
	LE_BITS_TO_4BYTE(__status, 16, 15)

/* DWORD 1 */
#define SET_RX_BUFFER_PHYSICAL_LOW(__status, __val)	\
	SET_BITS_TO_LE_4BYTE(__status+4, 0, 32, __val)

/* DWORD 2 */
#define SET_RX_BUFFER_PHYSICAL_HIGH(__rx_status_desc, __val, dma64)            \
	(dma64 ? SET_BITS_TO_LE_4BYTE((__rx_status_desc) + 8, 0, 32, __val) : 0)

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
	LE_BITS_TO_4BYTE(__pdesc+4, 0, 7)
#define GET_RX_DESC_TID(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 8, 4)
#define GET_RX_DESC_MACID_VLD(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 12, 1)
#define GET_RX_DESC_AMSDU(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 13, 1)
#define GET_RX_DESC_RXID_MATCH(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 14, 1)
#define GET_RX_DESC_PAGGR(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 15, 1)
#define GET_RX_DESC_A1_FIT(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+4, 16, 4)
#define GET_RX_DESC_TCPOFFLOAD_CHKERR(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+4, 20, 1)
#define GET_RX_DESC_TCPOFFLOAD_IPVER(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+4, 21, 1)
#define GET_RX_DESC_TCPOFFLOAD_IS_TCPUDP(__pdesc)	\
	LE_BITS_TO_4BYTE(__pdesc+4, 22, 1)
#define GET_RX_DESC_TCPOFFLOAD_CHK_VLD(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+4, 23, 1)
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
#define GET_RX_DESC_RX_IS_QOS(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+8, 16, 1)
#define GET_RX_STATUS_DESC_RPT_SEL(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+8, 28, 1)

#define GET_RX_DESC_RXMCS(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+12, 0, 7)
#define GET_RX_DESC_HTC(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+12, 10, 1)
#define GET_RX_STATUS_DESC_EOSP(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+12, 11, 1)
#define GET_RX_STATUS_DESC_BSSID_FIT(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+12, 12, 2)
#define GET_RX_STATUS_DESC_DMA_AGG_NUM(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+12, 16, 8)
#define GET_RX_STATUS_DESC_PATTERN_MATCH(__pdesc)	\
	LE_BITS_TO_4BYTE(__pdesc+12, 29, 1)
#define GET_RX_STATUS_DESC_UNICAST_MATCH(__pdesc)	\
	LE_BITS_TO_4BYTE(__pdesc+12, 30, 1)
#define GET_RX_STATUS_DESC_MAGIC_MATCH(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+12, 31, 1)

#define GET_RX_DESC_TSFL(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+20, 0, 32)

#define GET_RX_DESC_BUFF_ADDR(__pdesc)			\
	LE_BITS_TO_4BYTE(__pdesc+24, 0, 32)
#define GET_RX_DESC_BUFF_ADDR64(__pdesc)		\
	LE_BITS_TO_4BYTE(__pdesc+28, 0, 32)

#define SET_RX_DESC_BUFF_ADDR(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+24, 0, 32, __val)
#define SET_RX_DESC_BUFF_ADDR64(__pdesc, __val)		\
	SET_BITS_TO_LE_4BYTE(__pdesc+28, 0, 32, __val)

/* TX report 2 format in Rx desc*/

#define GET_RX_RPT2_DESC_PKT_LEN(__status)	\
	LE_BITS_TO_4BYTE(__status, 0, 9)
#define GET_RX_RPT2_DESC_MACID_VALID_1(__status)	\
	LE_BITS_TO_4BYTE(__status+16, 0, 32)
#define GET_RX_RPT2_DESC_MACID_VALID_2(__status)	\
	LE_BITS_TO_4BYTE(__status+20, 0, 32)

#define CLEAR_PCI_TX_DESC_CONTENT(__pdesc, _size)		\
do {								\
	if (_size > TX_DESC_NEXT_DESC_OFFSET)			\
		memset(__pdesc, 0, TX_DESC_NEXT_DESC_OFFSET);	\
	else							\
		memset(__pdesc, 0, _size);			\
} while (0)
=======
static inline void set_rx_buffer_desc_data_length(__le32 *__status, u32 __val)
{
	le32p_replace_bits(__status, __val, GENMASK(13, 0));
}

static inline void set_rx_buffer_desc_ls(__le32 *__status, u32 __val)
{
	le32p_replace_bits(__status, __val, BIT(15));
}

static inline void set_rx_buffer_desc_fs(__le32 *__status, u32 __val)
{
	le32p_replace_bits(__status, __val, BIT(16));
}

static inline void set_rx_buffer_desc_total_length(__le32 *__status, u32 __val)
{
	le32p_replace_bits(__status, __val, GENMASK(30, 16));
}

static inline int get_rx_buffer_desc_ls(__le32 *__status)
{
	return le32_get_bits(*(__status), BIT(15));
}

static inline int get_rx_buffer_desc_fs(__le32 *__status)
{
	return le32_get_bits(*(__status), BIT(16));
}

static inline int get_rx_buffer_desc_total_length(__le32 *__status)
{
	return le32_get_bits(*(__status), GENMASK(30, 16));
}

/* DWORD 1 */
static inline void set_rx_buffer_physical_low(__le32 *__status, u32 __val)
{
	*(__status + 1) = cpu_to_le32(__val);
}

/* DWORD 2 */
static inline void set_rx_buffer_physical_high(__le32 *__rx_status_desc,
					       u32 __val, bool dma64)
{
	if (dma64)
		*(__rx_status_desc + 2) = cpu_to_le32(__val);
	else
		*(__rx_status_desc + 2) = 0;
}

static inline int get_rx_desc_pkt_len(__le32 *__pdesc)
{
	return le32_get_bits(*__pdesc, GENMASK(13, 0));
}

static inline int get_rx_desc_crc32(__le32 *__pdesc)
{
	return le32_get_bits(*__pdesc, BIT(14));
}

static inline int get_rx_desc_icv(__le32 *__pdesc)
{
	return le32_get_bits(*__pdesc, BIT(15));
}

static inline int get_rx_desc_drv_info_size(__le32 *__pdesc)
{
	return le32_get_bits(*__pdesc, GENMASK(19, 16));
}

static inline int get_rx_desc_shift(__le32 *__pdesc)
{
	return le32_get_bits(*__pdesc, GENMASK(25, 24));
}

static inline int get_rx_desc_physt(__le32 *__pdesc)
{
	return le32_get_bits(*__pdesc, BIT(26));
}

static inline int get_rx_desc_swdec(__le32 *__pdesc)
{
	return le32_get_bits(*__pdesc, BIT(27));
}

static inline int get_rx_desc_own(__le32 *__pdesc)
{
	return le32_get_bits(*__pdesc, BIT(31));
}

static inline void set_rx_desc_eor(__le32 *__pdesc, u32 __val)
{
	le32p_replace_bits(__pdesc, __val, BIT(30));
}

static inline int get_rx_desc_macid(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), GENMASK(6, 0));
}

static inline int get_rx_desc_paggr(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 1), BIT(15));
}

static inline int get_rx_status_desc_rpt_sel(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 2), BIT(28));
}

static inline int get_rx_desc_rxmcs(__le32 *__pdesc)
{
	return le32_get_bits(*(__pdesc + 3), GENMASK(6, 0));
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

static inline u32 get_rx_desc_tsfl(__le32 *__pdesc)
{
	return le32_to_cpu(*((__pdesc + 5)));
}

static inline u32 get_rx_desc_buff_addr(__le32 *__pdesc)
{
	return le32_to_cpu(*((__pdesc + 6)));
}

/* TX report 2 format in Rx desc*/

static inline u32 get_rx_rpt2_desc_macid_valid_1(__le32 *__status)
{
	return le32_to_cpu(*((__status + 4)));
}

static inline u32 get_rx_rpt2_desc_macid_valid_2(__le32 *__status)
{
	return le32_to_cpu(*((__status + 5)));
}

static inline void clear_pci_tx_desc_content(__le32 *__pdesc, int _size)
{
	if (_size > TX_DESC_NEXT_DESC_OFFSET)
		memset(__pdesc, 0, TX_DESC_NEXT_DESC_OFFSET);
	else
		memset(__pdesc, 0, _size);
}
>>>>>>> upstream/android-13

#define RTL92EE_RX_HAL_IS_CCK_RATE(rxmcs)\
	(rxmcs == DESC_RATE1M ||\
	 rxmcs == DESC_RATE2M ||\
	 rxmcs == DESC_RATE5_5M ||\
	 rxmcs == DESC_RATE11M)

#define IS_LITTLE_ENDIAN	1

struct phy_rx_agc_info_t {
	#if IS_LITTLE_ENDIAN
		u8 gain:7, trsw:1;
	#else
		u8 trsw:1, gain:7;
	#endif
};

struct phy_status_rpt {
	struct phy_rx_agc_info_t path_agc[2];
	u8 ch_corr[2];
	u8 cck_sig_qual_ofdm_pwdb_all;
	u8 cck_agc_rpt_ofdm_cfosho_a;
	u8 cck_rpt_b_ofdm_cfosho_b;
	u8 rsvd_1;
	u8 noise_power_db_msb;
	u8 path_cfotail[2];
	u8 pcts_mask[2];
	u8 stream_rxevm[2];
	u8 path_rxsnr[2];
	u8 noise_power_db_lsb;
	u8 rsvd_2[3];
	u8 stream_csi[2];
	u8 stream_target_csi[2];
	u8 sig_evm;
	u8 rsvd_3;
#if IS_LITTLE_ENDIAN
	u8 antsel_rx_keep_2:1;	/*ex_intf_flg:1;*/
	u8 sgi_en:1;
	u8 rxsc:2;
	u8 idle_long:1;
	u8 r_ant_train_en:1;
	u8 ant_sel_b:1;
	u8 ant_sel:1;
#else	/* _BIG_ENDIAN_	*/
	u8 ant_sel:1;
	u8 ant_sel_b:1;
	u8 r_ant_train_en:1;
	u8 idle_long:1;
	u8 rxsc:2;
	u8 sgi_en:1;
	u8 antsel_rx_keep_2:1;	/*ex_intf_flg:1;*/
#endif
} __packed;

struct rx_fwinfo {
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

struct tx_desc {
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

struct rx_desc {
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

void rtl92ee_rx_check_dma_ok(struct ieee80211_hw *hw, u8 *header_desc,
			     u8 queue_index);
u16	rtl92ee_rx_desc_buff_remained_cnt(struct ieee80211_hw *hw,
					  u8 queue_index);
u16 rtl92ee_get_available_desc(struct ieee80211_hw *hw, u8 queue_index);
void rtl92ee_pre_fill_tx_bd_desc(struct ieee80211_hw *hw,
				 u8 *tx_bd_desc, u8 *desc, u8 queue_index,
				 struct sk_buff *skb, dma_addr_t addr);

void rtl92ee_tx_fill_desc(struct ieee80211_hw *hw,
			  struct ieee80211_hdr *hdr, u8 *pdesc_tx,
			  u8 *pbd_desc_tx,
			  struct ieee80211_tx_info *info,
			  struct ieee80211_sta *sta,
			  struct sk_buff *skb,
			  u8 hw_queue, struct rtl_tcb_desc *ptcb_desc);
bool rtl92ee_rx_query_desc(struct ieee80211_hw *hw,
			   struct rtl_stats *status,
			   struct ieee80211_rx_status *rx_status,
			   u8 *pdesc, struct sk_buff *skb);
void rtl92ee_set_desc(struct ieee80211_hw *hw, u8 *pdesc, bool istx,
		      u8 desc_name, u8 *val);

u64 rtl92ee_get_desc(struct ieee80211_hw *hw,
		     u8 *pdesc, bool istx, u8 desc_name);
bool rtl92ee_is_tx_desc_closed(struct ieee80211_hw *hw, u8 hw_queue, u16 index);
void rtl92ee_tx_polling(struct ieee80211_hw *hw, u8 hw_queue);
void rtl92ee_tx_fill_cmddesc(struct ieee80211_hw *hw, u8 *pdesc,
			     bool firstseg, bool lastseg,
			     struct sk_buff *skb);
#endif
