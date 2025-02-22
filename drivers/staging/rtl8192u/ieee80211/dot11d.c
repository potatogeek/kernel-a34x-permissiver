// SPDX-License-Identifier: GPL-2.0
/* Implement 802.11d. */

#include "dot11d.h"

<<<<<<< HEAD
void Dot11d_Init(struct ieee80211_device *ieee)
{
	struct rt_dot11d_info *pDot11dInfo = GET_DOT11D_INFO(ieee);

	pDot11dInfo->enabled = false;

	pDot11dInfo->state = DOT11D_STATE_NONE;
	pDot11dInfo->country_ie_len = 0;
	memset(pDot11dInfo->channel_map, 0, MAX_CHANNEL_NUMBER + 1);
	memset(pDot11dInfo->max_tx_pwr_dbm_list, 0xFF, MAX_CHANNEL_NUMBER+1);
	RESET_CIE_WATCHDOG(ieee);

	netdev_info(ieee->dev, "Dot11d_Init()\n");
}
EXPORT_SYMBOL(Dot11d_Init);

/* Reset to the state as we are just entering a regulatory domain. */
void Dot11d_Reset(struct ieee80211_device *ieee)
{
	u32 i;
	struct rt_dot11d_info *pDot11dInfo = GET_DOT11D_INFO(ieee);
	/* Clear old channel map */
	memset(pDot11dInfo->channel_map, 0, MAX_CHANNEL_NUMBER+1);
	memset(pDot11dInfo->max_tx_pwr_dbm_list, 0xFF, MAX_CHANNEL_NUMBER+1);
	/* Set new channel map */
	for (i = 1; i <= 11; i++)
		(pDot11dInfo->channel_map)[i] = 1;

	for (i = 12; i <= 14; i++)
		(pDot11dInfo->channel_map)[i] = 2;

	pDot11dInfo->state = DOT11D_STATE_NONE;
	pDot11dInfo->country_ie_len = 0;
	RESET_CIE_WATCHDOG(ieee);
}
EXPORT_SYMBOL(Dot11d_Reset);
=======
void rtl8192u_dot11d_init(struct ieee80211_device *ieee)
{
	struct rt_dot11d_info *dot11d_info = GET_DOT11D_INFO(ieee);

	dot11d_info->dot11d_enabled = false;

	dot11d_info->state = DOT11D_STATE_NONE;
	dot11d_info->country_ie_len = 0;
	memset(dot11d_info->channel_map, 0, MAX_CHANNEL_NUMBER + 1);
	memset(dot11d_info->max_tx_pwr_dbm_list, 0xFF, MAX_CHANNEL_NUMBER + 1);
	RESET_CIE_WATCHDOG(ieee);
}
EXPORT_SYMBOL(rtl8192u_dot11d_init);

/* Reset to the state as we are just entering a regulatory domain. */
void dot11d_reset(struct ieee80211_device *ieee)
{
	u32 i;
	struct rt_dot11d_info *dot11d_info = GET_DOT11D_INFO(ieee);
	/* Clear old channel map */
	memset(dot11d_info->channel_map, 0, MAX_CHANNEL_NUMBER + 1);
	memset(dot11d_info->max_tx_pwr_dbm_list, 0xFF, MAX_CHANNEL_NUMBER + 1);
	/* Set new channel map */
	for (i = 1; i <= 11; i++)
		(dot11d_info->channel_map)[i] = 1;

	for (i = 12; i <= 14; i++)
		(dot11d_info->channel_map)[i] = 2;

	dot11d_info->state = DOT11D_STATE_NONE;
	dot11d_info->country_ie_len = 0;
	RESET_CIE_WATCHDOG(ieee);
}
EXPORT_SYMBOL(dot11d_reset);
>>>>>>> upstream/android-13

/*
 * Update country IE from Beacon or Probe Resopnse and configure PHY for
 * operation in the regulatory domain.
 *
 * TODO: Configure Tx power.
 * Assumption:
 * 1. IS_DOT11D_ENABLE() is TRUE.
 * 2. Input IE is an valid one.
 */
<<<<<<< HEAD
void Dot11d_UpdateCountryIe(struct ieee80211_device *dev, u8 *pTaddr,
			    u16 CoutryIeLen, u8 *pCoutryIe)
{
	struct rt_dot11d_info *pDot11dInfo = GET_DOT11D_INFO(dev);
	u8 i, j, NumTriples, MaxChnlNum;
	struct chnl_txpower_triple *pTriple;

	memset(pDot11dInfo->channel_map, 0, MAX_CHANNEL_NUMBER+1);
	memset(pDot11dInfo->max_tx_pwr_dbm_list, 0xFF, MAX_CHANNEL_NUMBER+1);
=======
void dot11d_update_country_ie(struct ieee80211_device *dev, u8 *pTaddr,
			    u16 CoutryIeLen, u8 *pCoutryIe)
{
	struct rt_dot11d_info *dot11d_info = GET_DOT11D_INFO(dev);
	u8 i, j, NumTriples, MaxChnlNum;
	struct chnl_txpower_triple *pTriple;

	memset(dot11d_info->channel_map, 0, MAX_CHANNEL_NUMBER + 1);
	memset(dot11d_info->max_tx_pwr_dbm_list, 0xFF, MAX_CHANNEL_NUMBER + 1);
>>>>>>> upstream/android-13
	MaxChnlNum = 0;
	NumTriples = (CoutryIeLen - 3) / 3; /* skip 3-byte country string. */
	pTriple = (struct chnl_txpower_triple *)(pCoutryIe + 3);
	for (i = 0; i < NumTriples; i++) {
		if (MaxChnlNum >= pTriple->first_channel) {
			/* It is not in a monotonically increasing order, so
			 * stop processing.
			 */
<<<<<<< HEAD
			netdev_err(dev->dev, "Dot11d_UpdateCountryIe(): Invalid country IE, skip it........1\n");
=======
			netdev_err(dev->dev, "%s: Invalid country IE, skip it 1\n", __func__);
>>>>>>> upstream/android-13
			return;
		}
		if (MAX_CHANNEL_NUMBER < (pTriple->first_channel + pTriple->num_channels)) {
			/* It is not a valid set of channel id, so stop
			 * processing.
			 */
<<<<<<< HEAD
			netdev_err(dev->dev, "Dot11d_UpdateCountryIe(): Invalid country IE, skip it........2\n");
=======
			netdev_err(dev->dev, "%s: Invalid country IE, skip it 2\n", __func__);
>>>>>>> upstream/android-13
			return;
		}

		for (j = 0; j < pTriple->num_channels; j++) {
<<<<<<< HEAD
			pDot11dInfo->channel_map[pTriple->first_channel + j] = 1;
			pDot11dInfo->max_tx_pwr_dbm_list[pTriple->first_channel + j] = pTriple->max_tx_pwr_dbm;
=======
			dot11d_info->channel_map[pTriple->first_channel + j] = 1;
			dot11d_info->max_tx_pwr_dbm_list[pTriple->first_channel + j] = pTriple->max_tx_pwr_dbm;
>>>>>>> upstream/android-13
			MaxChnlNum = pTriple->first_channel + j;
		}

		pTriple = (struct chnl_txpower_triple *)((u8 *)pTriple + 3);
	}
	netdev_info(dev->dev, "Channel List:");
	for (i = 1; i <= MAX_CHANNEL_NUMBER; i++)
<<<<<<< HEAD
		if (pDot11dInfo->channel_map[i] > 0)
=======
		if (dot11d_info->channel_map[i] > 0)
>>>>>>> upstream/android-13
			netdev_info(dev->dev, " %d", i);
	netdev_info(dev->dev, "\n");

	UPDATE_CIE_SRC(dev, pTaddr);

<<<<<<< HEAD
	pDot11dInfo->country_ie_len = CoutryIeLen;
	memcpy(pDot11dInfo->country_ie_buf, pCoutryIe, CoutryIeLen);
	pDot11dInfo->state = DOT11D_STATE_LEARNED;
}
EXPORT_SYMBOL(Dot11d_UpdateCountryIe);

u8 DOT11D_GetMaxTxPwrInDbm(struct ieee80211_device *dev, u8 Channel)
{
	struct rt_dot11d_info *pDot11dInfo = GET_DOT11D_INFO(dev);
	u8 MaxTxPwrInDbm = 255;

	if (Channel > MAX_CHANNEL_NUMBER) {
		netdev_err(dev->dev, "DOT11D_GetMaxTxPwrInDbm(): Invalid Channel\n");
		return MaxTxPwrInDbm;
	}
	if (pDot11dInfo->channel_map[Channel])
		MaxTxPwrInDbm = pDot11dInfo->max_tx_pwr_dbm_list[Channel];

	return MaxTxPwrInDbm;
}
EXPORT_SYMBOL(DOT11D_GetMaxTxPwrInDbm);

void DOT11D_ScanComplete(struct ieee80211_device *dev)
{
	struct rt_dot11d_info *pDot11dInfo = GET_DOT11D_INFO(dev);

	switch (pDot11dInfo->state) {
	case DOT11D_STATE_LEARNED:
		pDot11dInfo->state = DOT11D_STATE_DONE;
=======
	dot11d_info->country_ie_len = CoutryIeLen;
	memcpy(dot11d_info->country_ie_buf, pCoutryIe, CoutryIeLen);
	dot11d_info->state = DOT11D_STATE_LEARNED;
}
EXPORT_SYMBOL(dot11d_update_country_ie);

u8 dot11d_get_max_tx_pwr_in_dbm(struct ieee80211_device *dev, u8 Channel)
{
	struct rt_dot11d_info *dot11d_info = GET_DOT11D_INFO(dev);
	u8 MaxTxPwrInDbm = 255;

	if (Channel > MAX_CHANNEL_NUMBER) {
		netdev_err(dev->dev, "%s: Invalid Channel\n", __func__);
		return MaxTxPwrInDbm;
	}
	if (dot11d_info->channel_map[Channel])
		MaxTxPwrInDbm = dot11d_info->max_tx_pwr_dbm_list[Channel];

	return MaxTxPwrInDbm;
}
EXPORT_SYMBOL(dot11d_get_max_tx_pwr_in_dbm);

void dot11d_scan_complete(struct ieee80211_device *dev)
{
	struct rt_dot11d_info *dot11d_info = GET_DOT11D_INFO(dev);

	switch (dot11d_info->state) {
	case DOT11D_STATE_LEARNED:
		dot11d_info->state = DOT11D_STATE_DONE;
>>>>>>> upstream/android-13
		break;

	case DOT11D_STATE_DONE:
		if (GET_CIE_WATCHDOG(dev) == 0) {
			/* Reset country IE if previous one is gone. */
<<<<<<< HEAD
			Dot11d_Reset(dev);
=======
			dot11d_reset(dev);
>>>>>>> upstream/android-13
		}
		break;
	case DOT11D_STATE_NONE:
		break;
	}
}
<<<<<<< HEAD
EXPORT_SYMBOL(DOT11D_ScanComplete);

int IsLegalChannel(struct ieee80211_device *dev, u8 channel)
{
	struct rt_dot11d_info *pDot11dInfo = GET_DOT11D_INFO(dev);

	if (channel > MAX_CHANNEL_NUMBER) {
		netdev_err(dev->dev, "IsLegalChannel(): Invalid Channel\n");
		return 0;
	}
	if (pDot11dInfo->channel_map[channel] > 0)
		return 1;
	return 0;
}
EXPORT_SYMBOL(IsLegalChannel);

int ToLegalChannel(struct ieee80211_device *dev, u8 channel)
{
	struct rt_dot11d_info *pDot11dInfo = GET_DOT11D_INFO(dev);
=======
EXPORT_SYMBOL(dot11d_scan_complete);

int is_legal_channel(struct ieee80211_device *dev, u8 channel)
{
	struct rt_dot11d_info *dot11d_info = GET_DOT11D_INFO(dev);

	if (channel > MAX_CHANNEL_NUMBER) {
		netdev_err(dev->dev, "%s: Invalid Channel\n", __func__);
		return 0;
	}
	if (dot11d_info->channel_map[channel] > 0)
		return 1;
	return 0;
}
EXPORT_SYMBOL(is_legal_channel);

int to_legal_channel(struct ieee80211_device *dev, u8 channel)
{
	struct rt_dot11d_info *dot11d_info = GET_DOT11D_INFO(dev);
>>>>>>> upstream/android-13
	u8 default_chn = 0;
	u32 i = 0;

	for (i = 1; i <= MAX_CHANNEL_NUMBER; i++) {
<<<<<<< HEAD
		if (pDot11dInfo->channel_map[i] > 0) {
=======
		if (dot11d_info->channel_map[i] > 0) {
>>>>>>> upstream/android-13
			default_chn = i;
			break;
		}
	}

	if (channel > MAX_CHANNEL_NUMBER) {
<<<<<<< HEAD
		netdev_err(dev->dev, "IsLegalChannel(): Invalid Channel\n");
		return default_chn;
	}

	if (pDot11dInfo->channel_map[channel] > 0)
=======
		netdev_err(dev->dev, "%s: Invalid Channel\n", __func__);
		return default_chn;
	}

	if (dot11d_info->channel_map[channel] > 0)
>>>>>>> upstream/android-13
		return channel;

	return default_chn;
}
<<<<<<< HEAD
EXPORT_SYMBOL(ToLegalChannel);
=======
EXPORT_SYMBOL(to_legal_channel);
>>>>>>> upstream/android-13
