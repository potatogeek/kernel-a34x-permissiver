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
// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
>>>>>>> upstream/android-13
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 ******************************************************************************/
#include "dot11d.h"

struct channel_list {
<<<<<<< HEAD
	u8      Channel[32];
	u8      Len;
};

static struct channel_list ChannelPlan[] = {
=======
	u8      channel[32];
	u8      len;
};

static struct channel_list channel_array[] = {
>>>>>>> upstream/android-13
	{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 36, 40, 44, 48, 52, 56, 60, 64,
	  149, 153, 157, 161, 165}, 24},
	{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 11},
	{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 36, 40, 44, 48, 52, 56,
	  60, 64}, 21},
	{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 36, 40, 44, 48, 52,
	  56, 60, 64}, 22},
	{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 36, 40, 44, 48, 52,
	  56, 60, 64}, 22},
	{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 36, 40, 44, 48, 52,
	  56, 60, 64}, 22},
	{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 36, 40, 44, 48, 52,
	 56, 60, 64}, 22},
	{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}, 14},
	{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 36, 40, 44, 48, 52,
	  56, 60, 64}, 21}
};

void dot11d_init(struct rtllib_device *ieee)
{
<<<<<<< HEAD
	struct rt_dot11d_info *pDot11dInfo = GET_DOT11D_INFO(ieee);

	pDot11dInfo->bEnabled = false;

	pDot11dInfo->State = DOT11D_STATE_NONE;
	pDot11dInfo->CountryIeLen = 0;
	memset(pDot11dInfo->channel_map, 0, MAX_CHANNEL_NUMBER + 1);
	memset(pDot11dInfo->MaxTxPwrDbmList, 0xFF, MAX_CHANNEL_NUMBER + 1);
=======
	struct rt_dot11d_info *dot11d_info = GET_DOT11D_INFO(ieee);

	dot11d_info->enabled = false;

	dot11d_info->state = DOT11D_STATE_NONE;
	dot11d_info->country_len = 0;
	memset(dot11d_info->channel_map, 0, MAX_CHANNEL_NUMBER + 1);
	memset(dot11d_info->max_tx_power_list, 0xFF, MAX_CHANNEL_NUMBER + 1);
>>>>>>> upstream/android-13
	RESET_CIE_WATCHDOG(ieee);
}
EXPORT_SYMBOL(dot11d_init);

<<<<<<< HEAD
void Dot11d_Channelmap(u8 channel_plan, struct rtllib_device *ieee)
{
	int i, max_chan = 14, min_chan = 1;

	ieee->bGlobalDomain = false;

	if (ChannelPlan[channel_plan].Len != 0) {
		memset(GET_DOT11D_INFO(ieee)->channel_map, 0,
		       sizeof(GET_DOT11D_INFO(ieee)->channel_map));
		for (i = 0; i < ChannelPlan[channel_plan].Len; i++) {
			if (ChannelPlan[channel_plan].Channel[i] < min_chan ||
			    ChannelPlan[channel_plan].Channel[i] > max_chan)
				break;
			GET_DOT11D_INFO(ieee)->channel_map[ChannelPlan
					[channel_plan].Channel[i]] = 1;
=======
void dot11d_channel_map(u8 channel_plan, struct rtllib_device *ieee)
{
	int i, max_chan = 14, min_chan = 1;

	ieee->global_domain = false;

	if (channel_array[channel_plan].len != 0) {
		memset(GET_DOT11D_INFO(ieee)->channel_map, 0,
		       sizeof(GET_DOT11D_INFO(ieee)->channel_map));
		for (i = 0; i < channel_array[channel_plan].len; i++) {
			if (channel_array[channel_plan].channel[i] < min_chan ||
			    channel_array[channel_plan].channel[i] > max_chan)
				break;
			GET_DOT11D_INFO(ieee)->channel_map[channel_array
					[channel_plan].channel[i]] = 1;
>>>>>>> upstream/android-13
		}
	}

	switch (channel_plan) {
	case COUNTRY_CODE_GLOBAL_DOMAIN:
<<<<<<< HEAD
		ieee->bGlobalDomain = true;
		for (i = 12; i <= 14; i++)
			GET_DOT11D_INFO(ieee)->channel_map[i] = 2;
		ieee->IbssStartChnl = 10;
=======
		ieee->global_domain = true;
		for (i = 12; i <= 14; i++)
			GET_DOT11D_INFO(ieee)->channel_map[i] = 2;
		ieee->bss_start_channel = 10;
>>>>>>> upstream/android-13
		ieee->ibss_maxjoin_chal = 11;
		break;

	case COUNTRY_CODE_WORLD_WIDE_13:
		for (i = 12; i <= 13; i++)
			GET_DOT11D_INFO(ieee)->channel_map[i] = 2;
<<<<<<< HEAD
		ieee->IbssStartChnl = 10;
=======
		ieee->bss_start_channel = 10;
>>>>>>> upstream/android-13
		ieee->ibss_maxjoin_chal = 11;
		break;

	default:
<<<<<<< HEAD
		ieee->IbssStartChnl = 1;
=======
		ieee->bss_start_channel = 1;
>>>>>>> upstream/android-13
		ieee->ibss_maxjoin_chal = 14;
		break;
	}
}
<<<<<<< HEAD
EXPORT_SYMBOL(Dot11d_Channelmap);

void Dot11d_Reset(struct rtllib_device *ieee)
{
	struct rt_dot11d_info *pDot11dInfo = GET_DOT11D_INFO(ieee);
	u32 i;

	memset(pDot11dInfo->channel_map, 0, MAX_CHANNEL_NUMBER + 1);
	memset(pDot11dInfo->MaxTxPwrDbmList, 0xFF, MAX_CHANNEL_NUMBER + 1);
	for (i = 1; i <= 11; i++)
		(pDot11dInfo->channel_map)[i] = 1;
	for (i = 12; i <= 14; i++)
		(pDot11dInfo->channel_map)[i] = 2;
	pDot11dInfo->State = DOT11D_STATE_NONE;
	pDot11dInfo->CountryIeLen = 0;
	RESET_CIE_WATCHDOG(ieee);
}

void Dot11d_UpdateCountryIe(struct rtllib_device *dev, u8 *pTaddr,
			    u16 CoutryIeLen, u8 *pCoutryIe)
{
	struct rt_dot11d_info *pDot11dInfo = GET_DOT11D_INFO(dev);
	u8 i, j, NumTriples, MaxChnlNum;
	struct chnl_txpow_triple *pTriple;

	memset(pDot11dInfo->channel_map, 0, MAX_CHANNEL_NUMBER + 1);
	memset(pDot11dInfo->MaxTxPwrDbmList, 0xFF, MAX_CHANNEL_NUMBER + 1);
	MaxChnlNum = 0;
	NumTriples = (CoutryIeLen - 3) / 3;
	pTriple = (struct chnl_txpow_triple *)(pCoutryIe + 3);
	for (i = 0; i < NumTriples; i++) {
		if (MaxChnlNum >= pTriple->FirstChnl) {
=======
EXPORT_SYMBOL(dot11d_channel_map);

void dot11d_reset(struct rtllib_device *ieee)
{
	struct rt_dot11d_info *dot11d_info = GET_DOT11D_INFO(ieee);
	u32 i;

	memset(dot11d_info->channel_map, 0, MAX_CHANNEL_NUMBER + 1);
	memset(dot11d_info->max_tx_power_list, 0xFF, MAX_CHANNEL_NUMBER + 1);
	for (i = 1; i <= 11; i++)
		(dot11d_info->channel_map)[i] = 1;
	for (i = 12; i <= 14; i++)
		(dot11d_info->channel_map)[i] = 2;
	dot11d_info->state = DOT11D_STATE_NONE;
	dot11d_info->country_len = 0;
	RESET_CIE_WATCHDOG(ieee);
}

void dot11d_update_country(struct rtllib_device *dev, u8 *address,
			   u16 country_len, u8 *country)
{
	struct rt_dot11d_info *dot11d_info = GET_DOT11D_INFO(dev);
	u8 i, j, number_of_triples, max_channel_number;
	struct chnl_txpow_triple *triple;

	memset(dot11d_info->channel_map, 0, MAX_CHANNEL_NUMBER + 1);
	memset(dot11d_info->max_tx_power_list, 0xFF, MAX_CHANNEL_NUMBER + 1);
	max_channel_number = 0;
	number_of_triples = (country_len - 3) / 3;
	triple = (struct chnl_txpow_triple *)(country + 3);
	for (i = 0; i < number_of_triples; i++) {
		if (max_channel_number >= triple->first_channel) {
>>>>>>> upstream/android-13
			netdev_info(dev->dev,
				    "%s: Invalid country IE, skip it......1\n",
				    __func__);
			return;
		}
<<<<<<< HEAD
		if (MAX_CHANNEL_NUMBER < (pTriple->FirstChnl +
		    pTriple->NumChnls)) {
=======
		if (MAX_CHANNEL_NUMBER < (triple->first_channel +
		    triple->num_channels)) {
>>>>>>> upstream/android-13
			netdev_info(dev->dev,
				    "%s: Invalid country IE, skip it......2\n",
				    __func__);
			return;
		}

<<<<<<< HEAD
		for (j = 0; j < pTriple->NumChnls; j++) {
			pDot11dInfo->channel_map[pTriple->FirstChnl + j] = 1;
			pDot11dInfo->MaxTxPwrDbmList[pTriple->FirstChnl + j] =
						 pTriple->MaxTxPowerInDbm;
			MaxChnlNum = pTriple->FirstChnl + j;
		}

		pTriple = (struct chnl_txpow_triple *)((u8 *)pTriple + 3);
	}

	UPDATE_CIE_SRC(dev, pTaddr);

	pDot11dInfo->CountryIeLen = CoutryIeLen;
	memcpy(pDot11dInfo->CountryIeBuf, pCoutryIe, CoutryIeLen);
	pDot11dInfo->State = DOT11D_STATE_LEARNED;
}

void DOT11D_ScanComplete(struct rtllib_device *dev)
{
	struct rt_dot11d_info *pDot11dInfo = GET_DOT11D_INFO(dev);

	switch (pDot11dInfo->State) {
	case DOT11D_STATE_LEARNED:
		pDot11dInfo->State = DOT11D_STATE_DONE;
		break;
	case DOT11D_STATE_DONE:
		Dot11d_Reset(dev);
=======
		for (j = 0; j < triple->num_channels; j++) {
			dot11d_info->channel_map[triple->first_channel + j] = 1;
			dot11d_info->max_tx_power_list[triple->first_channel + j] =
						 triple->max_tx_power;
			max_channel_number = triple->first_channel + j;
		}

		triple = (struct chnl_txpow_triple *)((u8 *)triple + 3);
	}

	UPDATE_CIE_SRC(dev, address);

	dot11d_info->country_len = country_len;
	memcpy(dot11d_info->country_buffer, country, country_len);
	dot11d_info->state = DOT11D_STATE_LEARNED;
}

void dot11d_scan_complete(struct rtllib_device *dev)
{
	struct rt_dot11d_info *dot11d_info = GET_DOT11D_INFO(dev);

	switch (dot11d_info->state) {
	case DOT11D_STATE_LEARNED:
		dot11d_info->state = DOT11D_STATE_DONE;
		break;
	case DOT11D_STATE_DONE:
		dot11d_reset(dev);
>>>>>>> upstream/android-13
		break;
	case DOT11D_STATE_NONE:
		break;
	}
}
