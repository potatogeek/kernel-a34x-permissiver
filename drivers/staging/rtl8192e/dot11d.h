<<<<<<< HEAD
/******************************************************************************
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
=======
/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
>>>>>>> upstream/android-13
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 ******************************************************************************/
#ifndef __INC_DOT11D_H
#define __INC_DOT11D_H

#include "rtllib.h"

struct chnl_txpow_triple {
<<<<<<< HEAD
	u8 FirstChnl;
	u8  NumChnls;
	u8  MaxTxPowerInDbm;
=======
	u8 first_channel;
	u8  num_channels;
	u8  max_tx_power;
>>>>>>> upstream/android-13
};

enum dot11d_state {
	DOT11D_STATE_NONE = 0,
	DOT11D_STATE_LEARNED,
	DOT11D_STATE_DONE,
};

/**
<<<<<<< HEAD
 * struct rt_dot11d_info * @CountryIeLen: value greater than 0 if
 *		  @CountryIeBuf contains valid country information element.
=======
 * struct rt_dot11d_info * @country_len: value greater than 0 if
 *		  @country_buffer contains valid country information element.
>>>>>>> upstream/android-13
 * @channel_map: holds channel values
 *		0 - invalid,
 *		1 - valid (active scan),
 *		2 - valid (passive scan)
<<<<<<< HEAD
 * @CountryIeSrcAddr - Source AP of the country IE
 */

struct rt_dot11d_info {
	bool bEnabled;

	u16 CountryIeLen;
	u8  CountryIeBuf[MAX_IE_LEN];
	u8  CountryIeSrcAddr[6];
	u8  CountryIeWatchdog;

	u8  channel_map[MAX_CHANNEL_NUMBER + 1];
	u8  MaxTxPwrDbmList[MAX_CHANNEL_NUMBER + 1];

	enum dot11d_state State;
};

static inline void cpMacAddr(unsigned char *des, unsigned char *src)
=======
 * @country_src_addr - Source AP of the country IE
 */

struct rt_dot11d_info {
	bool enabled;

	u16 country_len;
	u8  country_buffer[MAX_IE_LEN];
	u8  country_src_addr[6];
	u8  country_watchdog;

	u8  channel_map[MAX_CHANNEL_NUMBER + 1];
	u8  max_tx_power_list[MAX_CHANNEL_NUMBER + 1];

	enum dot11d_state state;
};

static inline void copy_mac_addr(unsigned char *des, unsigned char *src)
>>>>>>> upstream/android-13
{
	memcpy(des, src, 6);
}

<<<<<<< HEAD
#define GET_DOT11D_INFO(__pIeeeDev)			\
	 ((struct rt_dot11d_info *)((__pIeeeDev)->pDot11dInfo))

#define IS_DOT11D_ENABLE(__pIeeeDev)			\
	 (GET_DOT11D_INFO(__pIeeeDev)->bEnabled)
#define IS_COUNTRY_IE_VALID(__pIeeeDev)			\
	(GET_DOT11D_INFO(__pIeeeDev)->CountryIeLen > 0)

#define IS_EQUAL_CIE_SRC(__pIeeeDev, __pTa)		\
	 ether_addr_equal_unaligned( \
		GET_DOT11D_INFO(__pIeeeDev)->CountryIeSrcAddr, __pTa)
#define UPDATE_CIE_SRC(__pIeeeDev, __pTa)		\
	cpMacAddr(GET_DOT11D_INFO(__pIeeeDev)->CountryIeSrcAddr, __pTa)

#define GET_CIE_WATCHDOG(__pIeeeDev)				\
	 (GET_DOT11D_INFO(__pIeeeDev)->CountryIeWatchdog)
static inline void RESET_CIE_WATCHDOG(struct rtllib_device *__pIeeeDev)
{
	GET_CIE_WATCHDOG(__pIeeeDev) = 0;
}

#define UPDATE_CIE_WATCHDOG(__pIeeeDev) (++GET_CIE_WATCHDOG(__pIeeeDev))

void dot11d_init(struct rtllib_device *dev);
void Dot11d_Channelmap(u8 channel_plan, struct rtllib_device *ieee);
void Dot11d_Reset(struct rtllib_device *dev);
void Dot11d_UpdateCountryIe(struct rtllib_device *dev, u8 *pTaddr,
			    u16 CoutryIeLen, u8 *pCoutryIe);
void DOT11D_ScanComplete(struct rtllib_device *dev);
=======
#define GET_DOT11D_INFO(__ieee_dev)			\
	 ((struct rt_dot11d_info *)((__ieee_dev)->dot11d_info))

#define IS_DOT11D_ENABLE(__ieee_dev)			\
	 (GET_DOT11D_INFO(__ieee_dev)->enabled)
#define IS_COUNTRY_IE_VALID(__ieee_dev)			\
	(GET_DOT11D_INFO(__ieee_dev)->country_len > 0)

#define IS_EQUAL_CIE_SRC(__ieee_dev, __address)		\
	 ether_addr_equal_unaligned( \
		GET_DOT11D_INFO(__ieee_dev)->country_src_addr, __address)
#define UPDATE_CIE_SRC(__ieee_dev, __address)		\
	copy_mac_addr(GET_DOT11D_INFO(__ieee_dev)->country_src_addr, __address)

#define GET_CIE_WATCHDOG(__ieee_dev)				\
	 (GET_DOT11D_INFO(__ieee_dev)->country_watchdog)
static inline void RESET_CIE_WATCHDOG(struct rtllib_device *__ieee_dev)
{
	GET_CIE_WATCHDOG(__ieee_dev) = 0;
}

#define UPDATE_CIE_WATCHDOG(__ieee_dev) (++GET_CIE_WATCHDOG(__ieee_dev))

void dot11d_init(struct rtllib_device *dev);
void dot11d_channel_map(u8 channel_plan, struct rtllib_device *ieee);
void dot11d_reset(struct rtllib_device *dev);
void dot11d_update_country(struct rtllib_device *dev, u8 *address,
			   u16 country_len, u8 *country);
void dot11d_scan_complete(struct rtllib_device *dev);
>>>>>>> upstream/android-13

#endif
