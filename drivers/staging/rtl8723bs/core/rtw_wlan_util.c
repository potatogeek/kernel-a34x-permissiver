// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
<<<<<<< HEAD
#define _RTW_WLAN_UTIL_C_
=======
>>>>>>> upstream/android-13

#include <drv_types.h>
#include <rtw_debug.h>
#include <hal_com_h2c.h>

<<<<<<< HEAD
#if defined(CONFIG_WOWLAN) || defined(CONFIG_AP_WOWLAN)
#include <linux/inetdevice.h>
#endif

=======
>>>>>>> upstream/android-13
static unsigned char ARTHEROS_OUI1[] = {0x00, 0x03, 0x7f};
static unsigned char ARTHEROS_OUI2[] = {0x00, 0x13, 0x74};

static unsigned char BROADCOM_OUI1[] = {0x00, 0x10, 0x18};
static unsigned char BROADCOM_OUI2[] = {0x00, 0x0a, 0xf7};
static unsigned char BROADCOM_OUI3[] = {0x00, 0x05, 0xb5};

static unsigned char CISCO_OUI[] = {0x00, 0x40, 0x96};
static unsigned char MARVELL_OUI[] = {0x00, 0x50, 0x43};
static unsigned char RALINK_OUI[] = {0x00, 0x0c, 0x43};
static unsigned char REALTEK_OUI[] = {0x00, 0xe0, 0x4c};
static unsigned char AIRGOCAP_OUI[] = {0x00, 0x0a, 0xf5};
static unsigned char RSN_TKIP_CIPHER[4] = {0x00, 0x0f, 0xac, 0x02};
static unsigned char WPA_TKIP_CIPHER[4] = {0x00, 0x50, 0xf2, 0x02};

<<<<<<< HEAD
extern unsigned char RTW_WPA_OUI[];
extern unsigned char WPA_TKIP_CIPHER[4];

#define R2T_PHY_DELAY	(0)

=======
>>>>>>> upstream/android-13
/* define WAIT_FOR_BCN_TO_MIN	(3000) */
#define WAIT_FOR_BCN_TO_MIN	(6000)
#define WAIT_FOR_BCN_TO_MAX	(20000)

#define DISCONNECT_BY_CHK_BCN_FAIL_OBSERV_PERIOD_IN_MS 1000
#define DISCONNECT_BY_CHK_BCN_FAIL_THRESHOLD 3

static u8 rtw_basic_rate_cck[4] = {
	IEEE80211_CCK_RATE_1MB | IEEE80211_BASIC_RATE_MASK,
	IEEE80211_CCK_RATE_2MB | IEEE80211_BASIC_RATE_MASK,
	IEEE80211_CCK_RATE_5MB | IEEE80211_BASIC_RATE_MASK,
	IEEE80211_CCK_RATE_11MB | IEEE80211_BASIC_RATE_MASK
};

static u8 rtw_basic_rate_ofdm[3] = {
	IEEE80211_OFDM_RATE_6MB | IEEE80211_BASIC_RATE_MASK,
	IEEE80211_OFDM_RATE_12MB | IEEE80211_BASIC_RATE_MASK,
	IEEE80211_OFDM_RATE_24MB | IEEE80211_BASIC_RATE_MASK
};

<<<<<<< HEAD
int cckrates_included(unsigned char *rate, int ratelen)
{
	int	i;

	for (i = 0; i < ratelen; i++) {
		if  ((((rate[i]) & 0x7f) == 2)	|| (((rate[i]) & 0x7f) == 4) ||
		     (((rate[i]) & 0x7f) == 11)  || (((rate[i]) & 0x7f) == 22))
			return true;
	}

	return false;
}

int cckratesonly_included(unsigned char *rate, int ratelen)
{
	int	i;

	for (i = 0; i < ratelen; i++) {
		if  ((((rate[i]) & 0x7f) != 2) && (((rate[i]) & 0x7f) != 4) &&
		     (((rate[i]) & 0x7f) != 11)  && (((rate[i]) & 0x7f) != 22))
			return false;
	}

	return true;
}

u8 networktype_to_raid_ex(struct adapter *adapter, struct sta_info *psta)
{
	u8 raid, cur_rf_type, rf_type = RF_1T1R;

	rtw_hal_get_hwreg(adapter, HW_VAR_RF_TYPE, (u8 *)(&cur_rf_type));

	if (cur_rf_type == RF_1T1R) {
		rf_type = RF_1T1R;
	} else if (IsSupportedVHT(psta->wireless_mode)) {
		if (psta->ra_mask & 0xffc00000)
			rf_type = RF_2T2R;
	} else if (IsSupportedHT(psta->wireless_mode)) {
		if (psta->ra_mask & 0xfff00000)
			rf_type = RF_2T2R;
	}
=======
u8 networktype_to_raid_ex(struct adapter *adapter, struct sta_info *psta)
{
	u8 raid;
>>>>>>> upstream/android-13

	switch (psta->wireless_mode) {
	case WIRELESS_11B:
		raid = RATEID_IDX_B;
		break;
<<<<<<< HEAD
	case WIRELESS_11A:
=======
>>>>>>> upstream/android-13
	case WIRELESS_11G:
		raid = RATEID_IDX_G;
		break;
	case WIRELESS_11BG:
		raid = RATEID_IDX_BG;
		break;
	case WIRELESS_11_24N:
<<<<<<< HEAD
	case WIRELESS_11_5N:
	case WIRELESS_11A_5N:
	case WIRELESS_11G_24N:
		if (rf_type == RF_2T2R)
			raid = RATEID_IDX_GN_N2SS;
		else
			raid = RATEID_IDX_GN_N1SS;
=======
	case WIRELESS_11G_24N:
		raid = RATEID_IDX_GN_N1SS;
>>>>>>> upstream/android-13
		break;
	case WIRELESS_11B_24N:
	case WIRELESS_11BG_24N:
		if (psta->bw_mode == CHANNEL_WIDTH_20) {
<<<<<<< HEAD
			if (rf_type == RF_2T2R)
				raid = RATEID_IDX_BGN_20M_2SS_BN;
			else
				raid = RATEID_IDX_BGN_20M_1SS_BN;
		} else {
			if (rf_type == RF_2T2R)
				raid = RATEID_IDX_BGN_40M_2SS;
			else
				raid = RATEID_IDX_BGN_40M_1SS;
=======
			raid = RATEID_IDX_BGN_20M_1SS_BN;
		} else {
			raid = RATEID_IDX_BGN_40M_1SS;
>>>>>>> upstream/android-13
		}
		break;
	default:
		raid = RATEID_IDX_BGN_40M_2SS;
		break;
	}
	return raid;
}

unsigned char ratetbl_val_2wifirate(unsigned char rate);
unsigned char ratetbl_val_2wifirate(unsigned char rate)
{
	switch (rate & 0x7f) {
	case 0:
		return IEEE80211_CCK_RATE_1MB;
	case 1:
		return IEEE80211_CCK_RATE_2MB;
	case 2:
		return IEEE80211_CCK_RATE_5MB;
	case 3:
		return IEEE80211_CCK_RATE_11MB;
	case 4:
		return IEEE80211_OFDM_RATE_6MB;
	case 5:
		return IEEE80211_OFDM_RATE_9MB;
	case 6:
		return IEEE80211_OFDM_RATE_12MB;
	case 7:
		return IEEE80211_OFDM_RATE_18MB;
	case 8:
		return IEEE80211_OFDM_RATE_24MB;
	case 9:
		return IEEE80211_OFDM_RATE_36MB;
	case 10:
		return IEEE80211_OFDM_RATE_48MB;
	case 11:
		return IEEE80211_OFDM_RATE_54MB;
	default:
		return 0;
	}
}

int is_basicrate(struct adapter *padapter, unsigned char rate);
int is_basicrate(struct adapter *padapter, unsigned char rate)
{
	int i;
	unsigned char val;
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;

	for (i = 0; i < NumRates; i++) {
		val = pmlmeext->basicrate[i];

		if ((val != 0xff) && (val != 0xfe))
			if (rate == ratetbl_val_2wifirate(val))
				return true;
	}

	return false;
}

unsigned int ratetbl2rateset(struct adapter *padapter, unsigned char *rateset);
unsigned int ratetbl2rateset(struct adapter *padapter, unsigned char *rateset)
{
	int i;
	unsigned char rate;
	unsigned int	len = 0;
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;

	for (i = 0; i < NumRates; i++) {
		rate = pmlmeext->datarate[i];

		switch (rate) {
		case 0xff:
			return len;

		case 0xfe:
			continue;

		default:
			rate = ratetbl_val_2wifirate(rate);

			if (is_basicrate(padapter, rate) == true)
				rate |= IEEE80211_BASIC_RATE_MASK;

			rateset[len] = rate;
			len++;
			break;
		}
	}
	return len;
}

void get_rate_set(struct adapter *padapter, unsigned char *pbssrate, int *bssrate_len)
{
	unsigned char supportedrates[NumRates];

	memset(supportedrates, 0, NumRates);
	*bssrate_len = ratetbl2rateset(padapter, supportedrates);
	memcpy(pbssrate, supportedrates, *bssrate_len);
}

void set_mcs_rate_by_mask(u8 *mcs_set, u32 mask)
{
	u8 mcs_rate_1r = (u8)(mask&0xff);
	u8 mcs_rate_2r = (u8)((mask>>8)&0xff);
	u8 mcs_rate_3r = (u8)((mask>>16)&0xff);
	u8 mcs_rate_4r = (u8)((mask>>24)&0xff);

	mcs_set[0] &= mcs_rate_1r;
	mcs_set[1] &= mcs_rate_2r;
	mcs_set[2] &= mcs_rate_3r;
	mcs_set[3] &= mcs_rate_4r;
}

void UpdateBrateTbl(struct adapter *Adapter, u8 *mBratesOS)
{
	u8 i;
	u8 rate;

	/*  1M, 2M, 5.5M, 11M, 6M, 12M, 24M are mandatory. */
	for (i = 0; i < NDIS_802_11_LENGTH_RATES_EX; i++) {
		rate = mBratesOS[i] & 0x7f;
		switch (rate) {
		case IEEE80211_CCK_RATE_1MB:
		case IEEE80211_CCK_RATE_2MB:
		case IEEE80211_CCK_RATE_5MB:
		case IEEE80211_CCK_RATE_11MB:
		case IEEE80211_OFDM_RATE_6MB:
		case IEEE80211_OFDM_RATE_12MB:
		case IEEE80211_OFDM_RATE_24MB:
			mBratesOS[i] |= IEEE80211_BASIC_RATE_MASK;
			break;
		}
	}
}

void UpdateBrateTblForSoftAP(u8 *bssrateset, u32 bssratelen)
{
	u8 i;
	u8 rate;

	for (i = 0; i < bssratelen; i++) {
		rate = bssrateset[i] & 0x7f;
		switch (rate) {
		case IEEE80211_CCK_RATE_1MB:
		case IEEE80211_CCK_RATE_2MB:
		case IEEE80211_CCK_RATE_5MB:
		case IEEE80211_CCK_RATE_11MB:
			bssrateset[i] |= IEEE80211_BASIC_RATE_MASK;
			break;
		}
	}
}

void Save_DM_Func_Flag(struct adapter *padapter)
{
	u8 bSaveFlag = true;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	rtw_hal_set_hwreg(padapter, HW_VAR_DM_FUNC_OP, (u8 *)(&bSaveFlag));
}

void Restore_DM_Func_Flag(struct adapter *padapter)
{
	u8 bSaveFlag = false;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	rtw_hal_set_hwreg(padapter, HW_VAR_DM_FUNC_OP, (u8 *)(&bSaveFlag));
}

void Switch_DM_Func(struct adapter *padapter, u32 mode, u8 enable)
{
	if (enable == true)
		rtw_hal_set_hwreg(padapter, HW_VAR_DM_FUNC_SET, (u8 *)(&mode));
	else
		rtw_hal_set_hwreg(padapter, HW_VAR_DM_FUNC_CLR, (u8 *)(&mode));
}

<<<<<<< HEAD
static void Set_NETYPE0_MSR(struct adapter *padapter, u8 type)
{
	rtw_hal_set_hwreg(padapter, HW_VAR_MEDIA_STATUS, (u8 *)(&type));
}

void Set_MSR(struct adapter *padapter, u8 type)
{
	Set_NETYPE0_MSR(padapter, type);
=======
void Set_MSR(struct adapter *padapter, u8 type)
{
	rtw_hal_set_hwreg(padapter, HW_VAR_MEDIA_STATUS, (u8 *)(&type));
>>>>>>> upstream/android-13
}

inline u8 rtw_get_oper_ch(struct adapter *adapter)
{
	return adapter_to_dvobj(adapter)->oper_channel;
}

inline void rtw_set_oper_ch(struct adapter *adapter, u8 ch)
{
#ifdef DBG_CH_SWITCH
	const int len = 128;
	char msg[128] = {0};
	int cnt = 0;
	int i = 0;
#endif  /* DBG_CH_SWITCH */
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);

	if (dvobj->oper_channel != ch) {
		dvobj->on_oper_ch_time = jiffies;

#ifdef DBG_CH_SWITCH
<<<<<<< HEAD
		cnt += snprintf(msg+cnt, len-cnt, "switch to ch %3u", ch);

		for (i = 0; i < dvobj->iface_nums; i++) {
			struct adapter *iface = dvobj->padapters[i];
			cnt += snprintf(msg+cnt, len-cnt, " ["ADPT_FMT":", ADPT_ARG(iface));
			if (iface->mlmeextpriv.cur_channel == ch)
				cnt += snprintf(msg+cnt, len-cnt, "C");
			else
				cnt += snprintf(msg+cnt, len-cnt, "_");
			if (iface->wdinfo.listen_channel == ch && !rtw_p2p_chk_state(&iface->wdinfo, P2P_STATE_NONE))
				cnt += snprintf(msg+cnt, len-cnt, "L");
			else
				cnt += snprintf(msg+cnt, len-cnt, "_");
			cnt += snprintf(msg+cnt, len-cnt, "]");
		}

		DBG_871X(FUNC_ADPT_FMT" %s\n", FUNC_ADPT_ARG(adapter), msg);
=======
		cnt += scnprintf(msg+cnt, len-cnt, "switch to ch %3u", ch);

		for (i = 0; i < dvobj->iface_nums; i++) {
			struct adapter *iface = dvobj->padapters[i];

			cnt += scnprintf(msg+cnt, len-cnt, " [%s:", ADPT_ARG(iface));
			if (iface->mlmeextpriv.cur_channel == ch)
				cnt += scnprintf(msg+cnt, len-cnt, "C");
			else
				cnt += scnprintf(msg+cnt, len-cnt, "_");
			if (iface->wdinfo.listen_channel == ch && !rtw_p2p_chk_state(&iface->wdinfo, P2P_STATE_NONE))
				cnt += scnprintf(msg+cnt, len-cnt, "L");
			else
				cnt += scnprintf(msg+cnt, len-cnt, "_");
			cnt += scnprintf(msg+cnt, len-cnt, "]");
		}

>>>>>>> upstream/android-13
#endif /* DBG_CH_SWITCH */
	}

	dvobj->oper_channel = ch;
}

inline u8 rtw_get_oper_bw(struct adapter *adapter)
{
	return adapter_to_dvobj(adapter)->oper_bwmode;
}

inline void rtw_set_oper_bw(struct adapter *adapter, u8 bw)
{
	adapter_to_dvobj(adapter)->oper_bwmode = bw;
}

inline u8 rtw_get_oper_choffset(struct adapter *adapter)
{
	return adapter_to_dvobj(adapter)->oper_ch_offset;
}

inline void rtw_set_oper_choffset(struct adapter *adapter, u8 offset)
{
	adapter_to_dvobj(adapter)->oper_ch_offset = offset;
}

u8 rtw_get_center_ch(u8 channel, u8 chnl_bw, u8 chnl_offset)
{
	u8 center_ch = channel;

<<<<<<< HEAD
	if (chnl_bw == CHANNEL_WIDTH_80) {
		if ((channel == 36) || (channel == 40) || (channel == 44) || (channel == 48))
			center_ch = 42;
		if ((channel == 52) || (channel == 56) || (channel == 60) || (channel == 64))
			center_ch = 58;
		if ((channel == 100) || (channel == 104) || (channel == 108) || (channel == 112))
			center_ch = 106;
		if ((channel == 116) || (channel == 120) || (channel == 124) || (channel == 128))
			center_ch = 122;
		if ((channel == 132) || (channel == 136) || (channel == 140) || (channel == 144))
			center_ch = 138;
		if ((channel == 149) || (channel == 153) || (channel == 157) || (channel == 161))
			center_ch = 155;
		else if (channel <= 14)
			center_ch = 7;
	} else if (chnl_bw == CHANNEL_WIDTH_40) {
=======
	if (chnl_bw == CHANNEL_WIDTH_40) {
>>>>>>> upstream/android-13
		if (chnl_offset == HAL_PRIME_CHNL_OFFSET_LOWER)
			center_ch = channel + 2;
		else
			center_ch = channel - 2;
	}

	return center_ch;
}

inline unsigned long rtw_get_on_cur_ch_time(struct adapter *adapter)
{
	if (adapter->mlmeextpriv.cur_channel == adapter_to_dvobj(adapter)->oper_channel)
		return adapter_to_dvobj(adapter)->on_oper_ch_time;
	else
		return 0;
}

void SelectChannel(struct adapter *padapter, unsigned char channel)
{
	if (mutex_lock_interruptible(&(adapter_to_dvobj(padapter)->setch_mutex)))
		return;

	/* saved channel info */
	rtw_set_oper_ch(padapter, channel);

	rtw_hal_set_chan(padapter, channel);

	mutex_unlock(&(adapter_to_dvobj(padapter)->setch_mutex));
}

void set_channel_bwmode(struct adapter *padapter, unsigned char channel, unsigned char channel_offset, unsigned short bwmode)
{
	u8 center_ch, chnl_offset80 = HAL_PRIME_CHNL_OFFSET_DONT_CARE;

<<<<<<< HEAD
	if (padapter->bNotifyChannelChange)
		DBG_871X("[%s] ch = %d, offset = %d, bwmode = %d\n", __func__, channel, channel_offset, bwmode);

	center_ch = rtw_get_center_ch(channel, bwmode, channel_offset);

	if (bwmode == CHANNEL_WIDTH_80) {
		if (center_ch > channel)
			chnl_offset80 = HAL_PRIME_CHNL_OFFSET_LOWER;
		else if (center_ch < channel)
			chnl_offset80 = HAL_PRIME_CHNL_OFFSET_UPPER;
		else
			chnl_offset80 = HAL_PRIME_CHNL_OFFSET_DONT_CARE;
	}
=======
	center_ch = rtw_get_center_ch(channel, bwmode, channel_offset);

>>>>>>> upstream/android-13

	/* set Channel */
	if (mutex_lock_interruptible(&(adapter_to_dvobj(padapter)->setch_mutex)))
		return;

	/* saved channel/bw info */
	rtw_set_oper_ch(padapter, channel);
	rtw_set_oper_bw(padapter, bwmode);
	rtw_set_oper_choffset(padapter, channel_offset);

	rtw_hal_set_chnl_bw(padapter, center_ch, bwmode, channel_offset, chnl_offset80); /*  set center channel */

	mutex_unlock(&(adapter_to_dvobj(padapter)->setch_mutex));
}

<<<<<<< HEAD
__inline u8 *get_my_bssid(struct wlan_bssid_ex *pnetwork)
{
	return pnetwork->MacAddress;
=======
inline u8 *get_my_bssid(struct wlan_bssid_ex *pnetwork)
{
	return pnetwork->mac_address;
>>>>>>> upstream/android-13
}

u16 get_beacon_interval(struct wlan_bssid_ex *bss)
{
	__le16 val;
<<<<<<< HEAD
	memcpy((unsigned char *)&val, rtw_get_beacon_interval_from_ie(bss->IEs), 2);
=======

	memcpy((unsigned char *)&val, rtw_get_beacon_interval_from_ie(bss->ies), 2);
>>>>>>> upstream/android-13

	return le16_to_cpu(val);
}

int is_client_associated_to_ap(struct adapter *padapter)
{
	struct mlme_ext_priv *pmlmeext;
	struct mlme_ext_info *pmlmeinfo;

	if (!padapter)
		return _FAIL;

	pmlmeext = &padapter->mlmeextpriv;
	pmlmeinfo = &(pmlmeext->mlmext_info);

	if ((pmlmeinfo->state & WIFI_FW_ASSOC_SUCCESS) && ((pmlmeinfo->state&0x03) == WIFI_FW_STATION_STATE))
		return true;
	else
		return _FAIL;
}

int is_client_associated_to_ibss(struct adapter *padapter)
{
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);

	if ((pmlmeinfo->state & WIFI_FW_ASSOC_SUCCESS) && ((pmlmeinfo->state&0x03) == WIFI_FW_ADHOC_STATE))
		return true;
	else
		return _FAIL;
}

int is_IBSS_empty(struct adapter *padapter)
{
	unsigned int i;
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);

	for (i = IBSS_START_MAC_ID; i < NUM_STA; i++) {
		if (pmlmeinfo->FW_sta_info[i].status == 1)
			return _FAIL;
	}

	return true;
}

unsigned int decide_wait_for_beacon_timeout(unsigned int bcn_interval)
{
	if ((bcn_interval << 2) < WAIT_FOR_BCN_TO_MIN)
		return WAIT_FOR_BCN_TO_MIN;
	else if ((bcn_interval << 2) > WAIT_FOR_BCN_TO_MAX)
		return WAIT_FOR_BCN_TO_MAX;
	else
<<<<<<< HEAD
		return ((bcn_interval << 2));
=======
		return bcn_interval << 2;
>>>>>>> upstream/android-13
}

void invalidate_cam_all(struct adapter *padapter)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(padapter);
	struct cam_ctl_t *cam_ctl = &dvobj->cam_ctl;

	rtw_hal_set_hwreg(padapter, HW_VAR_CAM_INVALID_ALL, NULL);

	spin_lock_bh(&cam_ctl->lock);
	cam_ctl->bitmap = 0;
	memset(dvobj->cam_cache, 0, sizeof(struct cam_entry_cache)*TOTAL_CAM_ENTRY);
	spin_unlock_bh(&cam_ctl->lock);
}

static u32 _ReadCAM(struct adapter *padapter, u32 addr)
{
	u32 count = 0, cmd;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	cmd = CAM_POLLINIG | addr;
	rtw_write32(padapter, RWCAM, cmd);

	do {
		if (0 == (rtw_read32(padapter, REG_CAMCMD) & CAM_POLLINIG))
			break;
	} while (count++ < 100);

	return rtw_read32(padapter, REG_CAMREAD);
}
<<<<<<< HEAD
void read_cam(struct adapter *padapter, u8 entry, u8 *get_key)
{
	u32 j, addr, cmd;
	addr = entry << 3;

	/* DBG_8192C("********* DUMP CAM Entry_#%02d***************\n", entry); */
	for (j = 0; j < 6; j++) {
		cmd = _ReadCAM(padapter, addr+j);
		/* DBG_8192C("offset:0x%02x => 0x%08x\n", addr+j, cmd); */
		if (j > 1) /* get key from cam */
			memcpy(get_key+(j-2)*4, &cmd, 4);
	}
	/* DBG_8192C("*********************************\n"); */
=======

void read_cam(struct adapter *padapter, u8 entry, u8 *get_key)
{
	u32 j, addr, cmd;

	addr = entry << 3;

	for (j = 0; j < 6; j++) {
		cmd = _ReadCAM(padapter, addr+j);
		if (j > 1) /* get key from cam */
			memcpy(get_key+(j-2)*4, &cmd, 4);
	}
>>>>>>> upstream/android-13
}

void _write_cam(struct adapter *padapter, u8 entry, u16 ctrl, u8 *mac, u8 *key)
{
	unsigned int i, val, addr;
	int j;
	u32 cam_val[2];

	addr = entry << 3;

	for (j = 5; j >= 0; j--) {
		switch (j) {
		case 0:
			val = (ctrl | (mac[0] << 16) | (mac[1] << 24));
			break;
		case 1:
			val = (mac[2] | (mac[3] << 8) | (mac[4] << 16) | (mac[5] << 24));
			break;
		default:
			i = (j - 2) << 2;
			val = (key[i] | (key[i+1] << 8) | (key[i+2] << 16) | (key[i+3] << 24));
			break;
		}

		cam_val[0] = val;
		cam_val[1] = addr + (unsigned int)j;

		rtw_hal_set_hwreg(padapter, HW_VAR_CAM_WRITE, (u8 *)cam_val);
	}
}

void _clear_cam_entry(struct adapter *padapter, u8 entry)
{
	unsigned char null_sta[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char null_key[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	_write_cam(padapter, entry, 0, null_sta, null_key);
}

inline void write_cam(struct adapter *adapter, u8 id, u16 ctrl, u8 *mac, u8 *key)
{
	_write_cam(adapter, id, ctrl, mac, key);
	write_cam_cache(adapter, id, ctrl, mac, key);
}

inline void clear_cam_entry(struct adapter *adapter, u8 id)
{
	_clear_cam_entry(adapter, id);
	clear_cam_cache(adapter, id);
}

<<<<<<< HEAD
inline void write_cam_from_cache(struct adapter *adapter, u8 id)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);
	struct cam_ctl_t *cam_ctl = &dvobj->cam_ctl;
	struct cam_entry_cache cache;

	spin_lock_bh(&cam_ctl->lock);
	memcpy(&cache, &dvobj->cam_cache[id], sizeof(struct cam_entry_cache));
	spin_unlock_bh(&cam_ctl->lock);

	_write_cam(adapter, id, cache.ctrl, cache.mac, cache.key);
}

=======
>>>>>>> upstream/android-13
void write_cam_cache(struct adapter *adapter, u8 id, u16 ctrl, u8 *mac, u8 *key)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);
	struct cam_ctl_t *cam_ctl = &dvobj->cam_ctl;

	spin_lock_bh(&cam_ctl->lock);

	dvobj->cam_cache[id].ctrl = ctrl;
	memcpy(dvobj->cam_cache[id].mac, mac, ETH_ALEN);
	memcpy(dvobj->cam_cache[id].key, key, 16);

	spin_unlock_bh(&cam_ctl->lock);
}

void clear_cam_cache(struct adapter *adapter, u8 id)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);
	struct cam_ctl_t *cam_ctl = &dvobj->cam_ctl;

	spin_lock_bh(&cam_ctl->lock);

	memset(&(dvobj->cam_cache[id]), 0, sizeof(struct cam_entry_cache));

	spin_unlock_bh(&cam_ctl->lock);
}

static bool _rtw_camid_is_gk(struct adapter *adapter, u8 cam_id)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);
	struct cam_ctl_t *cam_ctl = &dvobj->cam_ctl;
	bool ret = false;

	if (cam_id >= TOTAL_CAM_ENTRY)
		goto exit;

	if (!(cam_ctl->bitmap & BIT(cam_id)))
		goto exit;

	ret = (dvobj->cam_cache[cam_id].ctrl&BIT6)?true:false;

exit:
	return ret;
}

static s16 _rtw_camid_search(struct adapter *adapter, u8 *addr, s16 kid)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);
	int i;
	s16 cam_id = -1;

	for (i = 0; i < TOTAL_CAM_ENTRY; i++) {
		if (addr && memcmp(dvobj->cam_cache[i].mac, addr, ETH_ALEN))
			continue;
		if (kid >= 0 && kid != (dvobj->cam_cache[i].ctrl&0x03))
			continue;

		cam_id = i;
		break;
	}

<<<<<<< HEAD
	if (addr)
		DBG_871X(FUNC_ADPT_FMT" addr:"MAC_FMT" kid:%d, return cam_id:%d\n"
			 , FUNC_ADPT_ARG(adapter), MAC_ARG(addr), kid, cam_id);
	else
		DBG_871X(FUNC_ADPT_FMT" addr:%p kid:%d, return cam_id:%d\n"
			 , FUNC_ADPT_ARG(adapter), addr, kid, cam_id);

=======
>>>>>>> upstream/android-13
	return cam_id;
}

s16 rtw_camid_search(struct adapter *adapter, u8 *addr, s16 kid)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);
	struct cam_ctl_t *cam_ctl = &dvobj->cam_ctl;
	s16 cam_id = -1;

	spin_lock_bh(&cam_ctl->lock);
	cam_id = _rtw_camid_search(adapter, addr, kid);
	spin_unlock_bh(&cam_ctl->lock);

	return cam_id;
}

s16 rtw_camid_alloc(struct adapter *adapter, struct sta_info *sta, u8 kid)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);
	struct cam_ctl_t *cam_ctl = &dvobj->cam_ctl;
	s16 cam_id = -1;
	struct mlme_ext_info *mlmeinfo;

	spin_lock_bh(&cam_ctl->lock);

	mlmeinfo = &adapter->mlmeextpriv.mlmext_info;

	if ((((mlmeinfo->state&0x03) == WIFI_FW_AP_STATE) || ((mlmeinfo->state&0x03) == WIFI_FW_ADHOC_STATE))
		&& !sta) {
		/* AP/Ad-hoc mode group key: static alloction to default key by key ID */
		if (kid > 3) {
<<<<<<< HEAD
			DBG_871X_LEVEL(_drv_always_, FUNC_ADPT_FMT" group key with invalid key id:%u\n"
				, FUNC_ADPT_ARG(adapter), kid);
=======
			netdev_dbg(adapter->pnetdev,
				   FUNC_ADPT_FMT " group key with invalid key id:%u\n",
				   FUNC_ADPT_ARG(adapter), kid);
>>>>>>> upstream/android-13
			rtw_warn_on(1);
			goto bitmap_handle;
		}

		cam_id = kid;
	} else {
		int i;
		u8 *addr = sta?sta->hwaddr:NULL;

		if (!sta) {
			if (!(mlmeinfo->state & WIFI_FW_ASSOC_SUCCESS)) {
				/* bypass STA mode group key setting before connected(ex:WEP) because bssid is not ready */
				goto bitmap_handle;
			}

			addr = get_bssid(&adapter->mlmepriv);
		}

		i = _rtw_camid_search(adapter, addr, kid);
		if (i >= 0) {
			/* Fix issue that pairwise and group key have same key id. Pairwise key first, group key can overwirte group only(ex: rekey) */
<<<<<<< HEAD
			if (sta || _rtw_camid_is_gk(adapter, i) == true)
				cam_id = i;
			else
				DBG_871X_LEVEL(_drv_always_, FUNC_ADPT_FMT" group key id:%u the same key id as pairwise key\n"
					, FUNC_ADPT_ARG(adapter), kid);
=======
			if (sta || _rtw_camid_is_gk(adapter, i))
				cam_id = i;
			else
				netdev_dbg(adapter->pnetdev,
					   FUNC_ADPT_FMT " group key id:%u the same key id as pairwise key\n",
					   FUNC_ADPT_ARG(adapter), kid);
>>>>>>> upstream/android-13
			goto bitmap_handle;
		}

		for (i = 4; i < TOTAL_CAM_ENTRY; i++)
			if (!(cam_ctl->bitmap & BIT(i)))
				break;

		if (i == TOTAL_CAM_ENTRY) {
			if (sta)
<<<<<<< HEAD
				DBG_871X_LEVEL(_drv_always_, FUNC_ADPT_FMT" pairwise key with "MAC_FMT" id:%u no room\n"
				, FUNC_ADPT_ARG(adapter), MAC_ARG(sta->hwaddr), kid);
			else
				DBG_871X_LEVEL(_drv_always_, FUNC_ADPT_FMT" group key id:%u no room\n"
				, FUNC_ADPT_ARG(adapter), kid);
=======
				netdev_dbg(adapter->pnetdev,
					   FUNC_ADPT_FMT " pairwise key with %pM id:%u no room\n",
					   FUNC_ADPT_ARG(adapter),
					   MAC_ARG(sta->hwaddr), kid);
			else
				netdev_dbg(adapter->pnetdev,
					   FUNC_ADPT_FMT " group key id:%u no room\n",
					   FUNC_ADPT_ARG(adapter), kid);
>>>>>>> upstream/android-13
			rtw_warn_on(1);
			goto bitmap_handle;
		}

		cam_id = i;
	}

bitmap_handle:
	if (cam_id >= 0 && cam_id < 32)
		cam_ctl->bitmap |= BIT(cam_id);

	spin_unlock_bh(&cam_ctl->lock);

	return cam_id;
}

void rtw_camid_free(struct adapter *adapter, u8 cam_id)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);
	struct cam_ctl_t *cam_ctl = &dvobj->cam_ctl;

	spin_lock_bh(&cam_ctl->lock);

	if (cam_id < TOTAL_CAM_ENTRY)
		cam_ctl->bitmap &= ~(BIT(cam_id));

	spin_unlock_bh(&cam_ctl->lock);
}

int allocate_fw_sta_entry(struct adapter *padapter)
{
	unsigned int mac_id;
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);

	for (mac_id = IBSS_START_MAC_ID; mac_id < NUM_STA; mac_id++) {
		if (pmlmeinfo->FW_sta_info[mac_id].status == 0) {
			pmlmeinfo->FW_sta_info[mac_id].status = 1;
			pmlmeinfo->FW_sta_info[mac_id].retry = 0;
			break;
		}
	}

	return mac_id;
}

void flush_all_cam_entry(struct adapter *padapter)
{
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);

	invalidate_cam_all(padapter);
	/* clear default key related key search setting */
	rtw_hal_set_hwreg(padapter, HW_VAR_SEC_DK_CFG, (u8 *)false);

	memset((u8 *)(pmlmeinfo->FW_sta_info), 0, sizeof(pmlmeinfo->FW_sta_info));
}

int WMM_param_handler(struct adapter *padapter, struct ndis_80211_var_ie *pIE)
{
	/* struct registry_priv *pregpriv = &padapter->registrypriv; */
	struct mlme_priv *pmlmepriv = &(padapter->mlmepriv);
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);

	if (pmlmepriv->qospriv.qos_option == 0) {
		pmlmeinfo->WMM_enable = 0;
		return false;
	}

	if (!memcmp(&(pmlmeinfo->WMM_param), (pIE->data + 6), sizeof(struct WMM_para_element)))
		return false;
	else
		memcpy(&(pmlmeinfo->WMM_param), (pIE->data + 6), sizeof(struct WMM_para_element));

	pmlmeinfo->WMM_enable = 1;
	return true;
}

<<<<<<< HEAD
=======
static void sort_wmm_ac_params(u32 *inx, u32 *edca)
{
	u32 i, j, change_inx = false;

	/* entry indx: 0->vo, 1->vi, 2->be, 3->bk. */
	for (i = 0; i < 4; i++) {
		for (j = i + 1; j < 4; j++) {
			/* compare CW and AIFS */
			if ((edca[j] & 0xFFFF) < (edca[i] & 0xFFFF)) {
				change_inx = true;
			} else if ((edca[j] & 0xFFFF) == (edca[i] & 0xFFFF)) {
				/* compare TXOP */
				if ((edca[j] >> 16) > (edca[i] >> 16))
					change_inx = true;
			}

			if (change_inx) {
				swap(edca[i], edca[j]);
				swap(inx[i], inx[j]);

				change_inx = false;
			}
		}
	}
}

>>>>>>> upstream/android-13
void WMMOnAssocRsp(struct adapter *padapter)
{
	u8 ACI, ACM, AIFS, ECWMin, ECWMax, aSifsTime;
	u8 acm_mask;
	u16 TXOP;
	u32 acParm, i;
	u32 edca[4], inx[4];
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);
<<<<<<< HEAD
	struct xmit_priv 	*pxmitpriv = &padapter->xmitpriv;
=======
	struct xmit_priv *pxmitpriv = &padapter->xmitpriv;
>>>>>>> upstream/android-13
	struct registry_priv *pregpriv = &padapter->registrypriv;

	acm_mask = 0;

	if (pmlmeext->cur_wireless_mode & WIRELESS_11_24N)
		aSifsTime = 16;
	else
		aSifsTime = 10;

	if (pmlmeinfo->WMM_enable == 0) {
		padapter->mlmepriv.acm_mask = 0;

		AIFS = aSifsTime + (2 * pmlmeinfo->slotTime);

<<<<<<< HEAD
		if (pmlmeext->cur_wireless_mode & (WIRELESS_11G | WIRELESS_11A)) {
=======
		if (pmlmeext->cur_wireless_mode & WIRELESS_11G) {
>>>>>>> upstream/android-13
			ECWMin = 4;
			ECWMax = 10;
		} else if (pmlmeext->cur_wireless_mode & WIRELESS_11B) {
			ECWMin = 5;
			ECWMax = 10;
		} else {
			ECWMin = 4;
			ECWMax = 10;
		}

		TXOP = 0;
		acParm = AIFS | (ECWMin << 8) | (ECWMax << 12) | (TXOP << 16);
		rtw_hal_set_hwreg(padapter, HW_VAR_AC_PARAM_BE, (u8 *)(&acParm));
		rtw_hal_set_hwreg(padapter, HW_VAR_AC_PARAM_BK, (u8 *)(&acParm));
		rtw_hal_set_hwreg(padapter, HW_VAR_AC_PARAM_VI, (u8 *)(&acParm));

		ECWMin = 2;
		ECWMax = 3;
		TXOP = 0x2f;
		acParm = AIFS | (ECWMin << 8) | (ECWMax << 12) | (TXOP << 16);
		rtw_hal_set_hwreg(padapter, HW_VAR_AC_PARAM_VO, (u8 *)(&acParm));
<<<<<<< HEAD
	} else{
=======
	} else {
>>>>>>> upstream/android-13
		edca[0] = edca[1] = edca[2] = edca[3] = 0;

		for (i = 0; i < 4; i++) {
			ACI = (pmlmeinfo->WMM_param.ac_param[i].ACI_AIFSN >> 5) & 0x03;
			ACM = (pmlmeinfo->WMM_param.ac_param[i].ACI_AIFSN >> 4) & 0x01;

			/* AIFS = AIFSN * slot time + SIFS - r2t phy delay */
			AIFS = (pmlmeinfo->WMM_param.ac_param[i].ACI_AIFSN & 0x0f) * pmlmeinfo->slotTime + aSifsTime;

			ECWMin = (pmlmeinfo->WMM_param.ac_param[i].CW & 0x0f);
			ECWMax = (pmlmeinfo->WMM_param.ac_param[i].CW & 0xf0) >> 4;
			TXOP = le16_to_cpu(pmlmeinfo->WMM_param.ac_param[i].TXOP_limit);

			acParm = AIFS | (ECWMin << 8) | (ECWMax << 12) | (TXOP << 16);

			switch (ACI) {
			case 0x0:
				rtw_hal_set_hwreg(padapter, HW_VAR_AC_PARAM_BE, (u8 *)(&acParm));
				acm_mask |= (ACM ? BIT(1):0);
				edca[XMIT_BE_QUEUE] = acParm;
				break;

			case 0x1:
				rtw_hal_set_hwreg(padapter, HW_VAR_AC_PARAM_BK, (u8 *)(&acParm));
				/* acm_mask |= (ACM? BIT(0):0); */
				edca[XMIT_BK_QUEUE] = acParm;
				break;

			case 0x2:
				rtw_hal_set_hwreg(padapter, HW_VAR_AC_PARAM_VI, (u8 *)(&acParm));
				acm_mask |= (ACM ? BIT(2):0);
				edca[XMIT_VI_QUEUE] = acParm;
				break;

			case 0x3:
				rtw_hal_set_hwreg(padapter, HW_VAR_AC_PARAM_VO, (u8 *)(&acParm));
				acm_mask |= (ACM ? BIT(3):0);
				edca[XMIT_VO_QUEUE] = acParm;
				break;
			}
<<<<<<< HEAD

			DBG_871X("WMM(%x): %x, %x\n", ACI, ACM, acParm);
=======
>>>>>>> upstream/android-13
		}

		if (padapter->registrypriv.acm_method == 1)
			rtw_hal_set_hwreg(padapter, HW_VAR_ACM_CTRL, (u8 *)(&acm_mask));
		else
			padapter->mlmepriv.acm_mask = acm_mask;

		inx[0] = 0; inx[1] = 1; inx[2] = 2; inx[3] = 3;

<<<<<<< HEAD
		if (pregpriv->wifi_spec == 1) {
			u32 j, tmp, change_inx = false;

			/* entry indx: 0->vo, 1->vi, 2->be, 3->bk. */
			for (i = 0; i < 4; i++) {
				for (j = i+1; j < 4; j++) {
					/* compare CW and AIFS */
					if ((edca[j] & 0xFFFF) < (edca[i] & 0xFFFF)) {
						change_inx = true;
					} else if ((edca[j] & 0xFFFF) == (edca[i] & 0xFFFF)) {
						/* compare TXOP */
						if ((edca[j] >> 16) > (edca[i] >> 16))
							change_inx = true;
					}

					if (change_inx) {
						tmp = edca[i];
						edca[i] = edca[j];
						edca[j] = tmp;

						tmp = inx[i];
						inx[i] = inx[j];
						inx[j] = tmp;

						change_inx = false;
					}
				}
			}
		}

		for (i = 0; i < 4; i++) {
			pxmitpriv->wmm_para_seq[i] = inx[i];
			DBG_871X("wmm_para_seq(%d): %d\n", i, pxmitpriv->wmm_para_seq[i]);
		}
=======
		if (pregpriv->wifi_spec == 1)
			sort_wmm_ac_params(inx, edca);

		for (i = 0; i < 4; i++)
			pxmitpriv->wmm_para_seq[i] = inx[i];
>>>>>>> upstream/android-13
	}
}

static void bwmode_update_check(struct adapter *padapter, struct ndis_80211_var_ie *pIE)
{
	unsigned char  new_bwmode;
	unsigned char  new_ch_offset;
	struct HT_info_element	 *pHT_info;
	struct mlme_priv *pmlmepriv = &(padapter->mlmepriv);
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);
	struct registry_priv *pregistrypriv = &padapter->registrypriv;
<<<<<<< HEAD
	struct ht_priv 		*phtpriv = &pmlmepriv->htpriv;
=======
	struct ht_priv *phtpriv = &pmlmepriv->htpriv;
>>>>>>> upstream/android-13
	u8 cbw40_enable = 0;

	if (!pIE)
		return;

	if (phtpriv->ht_option == false)
		return;

<<<<<<< HEAD
	if (pmlmeext->cur_bwmode >= CHANNEL_WIDTH_80)
		return;

	if (pIE->Length > sizeof(struct HT_info_element))
=======
	if (pIE->length > sizeof(struct HT_info_element))
>>>>>>> upstream/android-13
		return;

	pHT_info = (struct HT_info_element *)pIE->data;

	if (pmlmeext->cur_channel > 14) {
		if ((pregistrypriv->bw_mode & 0xf0) > 0)
			cbw40_enable = 1;
<<<<<<< HEAD
	} else
		if ((pregistrypriv->bw_mode & 0x0f) > 0)
			cbw40_enable = 1;
=======
	} else {
		if ((pregistrypriv->bw_mode & 0x0f) > 0)
			cbw40_enable = 1;
	}
>>>>>>> upstream/android-13

	if ((pHT_info->infos[0] & BIT(2)) && cbw40_enable) {
		new_bwmode = CHANNEL_WIDTH_40;

		switch (pHT_info->infos[0] & 0x3) {
		case 1:
			new_ch_offset = HAL_PRIME_CHNL_OFFSET_LOWER;
			break;

		case 3:
			new_ch_offset = HAL_PRIME_CHNL_OFFSET_UPPER;
			break;

		default:
			new_bwmode = CHANNEL_WIDTH_20;
			new_ch_offset = HAL_PRIME_CHNL_OFFSET_DONT_CARE;
			break;
		}
<<<<<<< HEAD
	} else{
=======
	} else {
>>>>>>> upstream/android-13
		new_bwmode = CHANNEL_WIDTH_20;
		new_ch_offset = HAL_PRIME_CHNL_OFFSET_DONT_CARE;
	}

	if ((new_bwmode != pmlmeext->cur_bwmode) || (new_ch_offset != pmlmeext->cur_ch_offset)) {
		pmlmeinfo->bwmode_updated = true;

		pmlmeext->cur_bwmode = new_bwmode;
		pmlmeext->cur_ch_offset = new_ch_offset;

		/* update HT info also */
		HT_info_handler(padapter, pIE);
<<<<<<< HEAD
	} else
		pmlmeinfo->bwmode_updated = false;
=======
	} else {
		pmlmeinfo->bwmode_updated = false;
	}
>>>>>>> upstream/android-13

	if (true == pmlmeinfo->bwmode_updated) {
		struct sta_info *psta;
		struct wlan_bssid_ex	*cur_network = &(pmlmeinfo->network);
		struct sta_priv *pstapriv = &padapter->stapriv;

		/* set_channel_bwmode(padapter, pmlmeext->cur_channel, pmlmeext->cur_ch_offset, pmlmeext->cur_bwmode); */

		/* update ap's stainfo */
<<<<<<< HEAD
		psta = rtw_get_stainfo(pstapriv, cur_network->MacAddress);
=======
		psta = rtw_get_stainfo(pstapriv, cur_network->mac_address);
>>>>>>> upstream/android-13
		if (psta) {
			struct ht_priv *phtpriv_sta = &psta->htpriv;

			if (phtpriv_sta->ht_option) {
				/*  bwmode */
				psta->bw_mode = pmlmeext->cur_bwmode;
				phtpriv_sta->ch_offset = pmlmeext->cur_ch_offset;
<<<<<<< HEAD
			} else{
=======
			} else {
>>>>>>> upstream/android-13
				psta->bw_mode = CHANNEL_WIDTH_20;
				phtpriv_sta->ch_offset = HAL_PRIME_CHNL_OFFSET_DONT_CARE;
			}

			rtw_dm_ra_mask_wk_cmd(padapter, (u8 *)psta);
		}
	}
}

void HT_caps_handler(struct adapter *padapter, struct ndis_80211_var_ie *pIE)
{
	unsigned int	i;
<<<<<<< HEAD
	u8 rf_type;
=======
>>>>>>> upstream/android-13
	u8 max_AMPDU_len, min_MPDU_spacing;
	u8 cur_ldpc_cap = 0, cur_stbc_cap = 0;
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);
<<<<<<< HEAD
	struct mlme_priv 	*pmlmepriv = &padapter->mlmepriv;
	struct ht_priv 		*phtpriv = &pmlmepriv->htpriv;
=======
	struct mlme_priv *pmlmepriv = &padapter->mlmepriv;
	struct ht_priv *phtpriv = &pmlmepriv->htpriv;
>>>>>>> upstream/android-13

	if (!pIE)
		return;

	if (phtpriv->ht_option == false)
		return;

	pmlmeinfo->HT_caps_enable = 1;

<<<<<<< HEAD
	for (i = 0; i < (pIE->Length); i++) {
		if (i != 2) {
			/* 	Commented by Albert 2010/07/12 */
			/* 	Got the endian issue here. */
			pmlmeinfo->HT_caps.u.HT_cap[i] &= (pIE->data[i]);
		} else{
=======
	for (i = 0; i < (pIE->length); i++) {
		if (i != 2) {
			/* Commented by Albert 2010/07/12 */
			/* Got the endian issue here. */
			pmlmeinfo->HT_caps.u.HT_cap[i] &= (pIE->data[i]);
		} else {
>>>>>>> upstream/android-13
			/* modify from  fw by Thomas 2010/11/17 */
			if ((pmlmeinfo->HT_caps.u.HT_cap_element.AMPDU_para & 0x3) > (pIE->data[i] & 0x3))
				max_AMPDU_len = (pIE->data[i] & 0x3);
			else
				max_AMPDU_len = (pmlmeinfo->HT_caps.u.HT_cap_element.AMPDU_para & 0x3);

			if ((pmlmeinfo->HT_caps.u.HT_cap_element.AMPDU_para & 0x1c) > (pIE->data[i] & 0x1c))
				min_MPDU_spacing = (pmlmeinfo->HT_caps.u.HT_cap_element.AMPDU_para & 0x1c);
			else
				min_MPDU_spacing = (pIE->data[i] & 0x1c);

			pmlmeinfo->HT_caps.u.HT_cap_element.AMPDU_para = max_AMPDU_len | min_MPDU_spacing;
		}
	}
<<<<<<< HEAD
	rtw_hal_get_hwreg(padapter, HW_VAR_RF_TYPE, (u8 *)(&rf_type));
=======
>>>>>>> upstream/android-13

	/* update the MCS set */
	for (i = 0; i < 16; i++)
		pmlmeinfo->HT_caps.u.HT_cap_element.MCS_rate[i] &= pmlmeext->default_supported_mcs_set[i];

	/* update the MCS rates */
<<<<<<< HEAD
	switch (rf_type) {
	case RF_1T1R:
	case RF_1T2R:
		set_mcs_rate_by_mask(pmlmeinfo->HT_caps.u.HT_cap_element.MCS_rate, MCS_RATE_1R);
		break;
	case RF_2T2R:
	default:
#ifdef CONFIG_DISABLE_MCS13TO15
		if (pmlmeext->cur_bwmode == CHANNEL_WIDTH_40 && pregistrypriv->wifi_spec != 1)
			set_mcs_rate_by_mask(pmlmeinfo->HT_caps.u.HT_cap_element.MCS_rate, MCS_RATE_2R_13TO15_OFF);
		else
			set_mcs_rate_by_mask(pmlmeinfo->HT_caps.u.HT_cap_element.MCS_rate, MCS_RATE_2R);
#else /* CONFIG_DISABLE_MCS13TO15 */
		set_mcs_rate_by_mask(pmlmeinfo->HT_caps.u.HT_cap_element.MCS_rate, MCS_RATE_2R);
#endif /* CONFIG_DISABLE_MCS13TO15 */
	}

	if (check_fwstate(pmlmepriv, WIFI_AP_STATE)) {
		/*  Config STBC setting */
		if (TEST_FLAG(phtpriv->stbc_cap, STBC_HT_ENABLE_TX) && GET_HT_CAPABILITY_ELE_TX_STBC(pIE->data)) {
			SET_FLAG(cur_stbc_cap, STBC_HT_ENABLE_TX);
			DBG_871X("Enable HT Tx STBC !\n");
		}
		phtpriv->stbc_cap = cur_stbc_cap;
	} else {
		/*  Config LDPC Coding Capability */
		if (TEST_FLAG(phtpriv->ldpc_cap, LDPC_HT_ENABLE_TX) && GET_HT_CAPABILITY_ELE_LDPC_CAP(pIE->data)) {
			SET_FLAG(cur_ldpc_cap, (LDPC_HT_ENABLE_TX | LDPC_HT_CAP_TX));
			DBG_871X("Enable HT Tx LDPC!\n");
		}
		phtpriv->ldpc_cap = cur_ldpc_cap;

		/*  Config STBC setting */
		if (TEST_FLAG(phtpriv->stbc_cap, STBC_HT_ENABLE_TX) && GET_HT_CAPABILITY_ELE_RX_STBC(pIE->data)) {
			SET_FLAG(cur_stbc_cap, (STBC_HT_ENABLE_TX | STBC_HT_CAP_TX));
			DBG_871X("Enable HT Tx STBC!\n");
		}
=======
	set_mcs_rate_by_mask(pmlmeinfo->HT_caps.u.HT_cap_element.MCS_rate, MCS_RATE_1R);

	if (check_fwstate(pmlmepriv, WIFI_AP_STATE)) {
		/*  Config STBC setting */
		if (TEST_FLAG(phtpriv->stbc_cap, STBC_HT_ENABLE_TX) &&
		    GET_HT_CAPABILITY_ELE_TX_STBC(pIE->data))
			SET_FLAG(cur_stbc_cap, STBC_HT_ENABLE_TX);

		phtpriv->stbc_cap = cur_stbc_cap;
	} else {
		/*  Config LDPC Coding Capability */
		if (TEST_FLAG(phtpriv->ldpc_cap, LDPC_HT_ENABLE_TX) &&
		    GET_HT_CAPABILITY_ELE_LDPC_CAP(pIE->data))
			SET_FLAG(cur_ldpc_cap, (LDPC_HT_ENABLE_TX | LDPC_HT_CAP_TX));

		phtpriv->ldpc_cap = cur_ldpc_cap;

		/*  Config STBC setting */
		if (TEST_FLAG(phtpriv->stbc_cap, STBC_HT_ENABLE_TX) &&
		    GET_HT_CAPABILITY_ELE_RX_STBC(pIE->data))
			SET_FLAG(cur_stbc_cap, (STBC_HT_ENABLE_TX | STBC_HT_CAP_TX));

>>>>>>> upstream/android-13
		phtpriv->stbc_cap = cur_stbc_cap;
	}
}

void HT_info_handler(struct adapter *padapter, struct ndis_80211_var_ie *pIE)
{
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);
<<<<<<< HEAD
	struct mlme_priv 	*pmlmepriv = &padapter->mlmepriv;
	struct ht_priv 		*phtpriv = &pmlmepriv->htpriv;
=======
	struct mlme_priv *pmlmepriv = &padapter->mlmepriv;
	struct ht_priv *phtpriv = &pmlmepriv->htpriv;
>>>>>>> upstream/android-13

	if (!pIE)
		return;

	if (phtpriv->ht_option == false)
		return;

<<<<<<< HEAD
	if (pIE->Length > sizeof(struct HT_info_element))
		return;

	pmlmeinfo->HT_info_enable = 1;
	memcpy(&(pmlmeinfo->HT_info), pIE->data, pIE->Length);

	return;
=======
	if (pIE->length > sizeof(struct HT_info_element))
		return;

	pmlmeinfo->HT_info_enable = 1;
	memcpy(&(pmlmeinfo->HT_info), pIE->data, pIE->length);
>>>>>>> upstream/android-13
}

void HTOnAssocRsp(struct adapter *padapter)
{
<<<<<<< HEAD
	unsigned char 	max_AMPDU_len;
	unsigned char 	min_MPDU_spacing;
=======
	unsigned char max_AMPDU_len;
	unsigned char min_MPDU_spacing;
>>>>>>> upstream/android-13
	/* struct registry_priv  *pregpriv = &padapter->registrypriv; */
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);

<<<<<<< HEAD
	DBG_871X("%s\n", __func__);

	if ((pmlmeinfo->HT_info_enable) && (pmlmeinfo->HT_caps_enable)) {
		pmlmeinfo->HT_enable = 1;
	} else{
=======
	if ((pmlmeinfo->HT_info_enable) && (pmlmeinfo->HT_caps_enable)) {
		pmlmeinfo->HT_enable = 1;
	} else {
>>>>>>> upstream/android-13
		pmlmeinfo->HT_enable = 0;
		/* set_channel_bwmode(padapter, pmlmeext->cur_channel, pmlmeext->cur_ch_offset, pmlmeext->cur_bwmode); */
		return;
	}

	/* handle A-MPDU parameter field */
	/*
		AMPDU_para [1:0]:Max AMPDU Len => 0:8k , 1:16k, 2:32k, 3:64k
		AMPDU_para [4:2]:Min MPDU Start Spacing
	*/
	max_AMPDU_len = pmlmeinfo->HT_caps.u.HT_cap_element.AMPDU_para & 0x03;

	min_MPDU_spacing = (pmlmeinfo->HT_caps.u.HT_cap_element.AMPDU_para & 0x1c) >> 2;

	rtw_hal_set_hwreg(padapter, HW_VAR_AMPDU_MIN_SPACE, (u8 *)(&min_MPDU_spacing));

	rtw_hal_set_hwreg(padapter, HW_VAR_AMPDU_FACTOR, (u8 *)(&max_AMPDU_len));
}

void ERP_IE_handler(struct adapter *padapter, struct ndis_80211_var_ie *pIE)
{
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);

<<<<<<< HEAD
	if (pIE->Length > 1)
		return;

	pmlmeinfo->ERP_enable = 1;
	memcpy(&(pmlmeinfo->ERP_IE), pIE->data, pIE->Length);
=======
	if (pIE->length > 1)
		return;

	pmlmeinfo->ERP_enable = 1;
	memcpy(&(pmlmeinfo->ERP_IE), pIE->data, pIE->length);
>>>>>>> upstream/android-13
}

void VCS_update(struct adapter *padapter, struct sta_info *psta)
{
	struct registry_priv  *pregpriv = &padapter->registrypriv;
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);

	switch (pregpriv->vrtl_carrier_sense) {/* 0:off 1:on 2:auto */
	case 0: /* off */
		psta->rtsen = 0;
		psta->cts2self = 0;
		break;

	case 1: /* on */
		if (pregpriv->vcs_type == 1) { /* 1:RTS/CTS 2:CTS to self */
			psta->rtsen = 1;
			psta->cts2self = 0;
<<<<<<< HEAD
		} else{
=======
		} else {
>>>>>>> upstream/android-13
			psta->rtsen = 0;
			psta->cts2self = 1;
		}
		break;

	case 2: /* auto */
	default:
		if ((pmlmeinfo->ERP_enable) && (pmlmeinfo->ERP_IE & BIT(1))) {
			if (pregpriv->vcs_type == 1) {
				psta->rtsen = 1;
				psta->cts2self = 0;
<<<<<<< HEAD
			} else{
				psta->rtsen = 0;
				psta->cts2self = 1;
			}
		} else{
=======
			} else {
				psta->rtsen = 0;
				psta->cts2self = 1;
			}
		} else {
>>>>>>> upstream/android-13
			psta->rtsen = 0;
			psta->cts2self = 0;
		}
		break;
	}
}

void update_ldpc_stbc_cap(struct sta_info *psta)
{
	if (psta->htpriv.ht_option) {
		if (TEST_FLAG(psta->htpriv.ldpc_cap, LDPC_HT_ENABLE_TX))
			psta->ldpc = 1;

		if (TEST_FLAG(psta->htpriv.stbc_cap, STBC_HT_ENABLE_TX))
			psta->stbc = 1;
	} else {
		psta->ldpc = 0;
		psta->stbc = 0;
	}
}

int rtw_check_bcn_info(struct adapter *Adapter, u8 *pframe, u32 packet_len)
{
<<<<<<< HEAD
	unsigned int		len;
	unsigned char 	*p;
=======
	unsigned int len;
	unsigned char *p;
>>>>>>> upstream/android-13
	unsigned short	val16, subtype;
	struct wlan_network *cur_network = &(Adapter->mlmepriv.cur_network);
	/* u8 wpa_ie[255], rsn_ie[255]; */
	u16 wpa_len = 0, rsn_len = 0;
	u8 encryp_protocol = 0;
	struct wlan_bssid_ex *bssid;
	int group_cipher = 0, pairwise_cipher = 0, is_8021x = 0;
	unsigned char *pbuf;
	u32 wpa_ielen = 0;
	u8 *pbssid = GetAddr3Ptr(pframe);
<<<<<<< HEAD
	u32 hidden_ssid = 0;
	struct HT_info_element *pht_info = NULL;
	struct rtw_ieee80211_ht_cap *pht_cap = NULL;
=======
	struct HT_info_element *pht_info = NULL;
	struct ieee80211_ht_cap *pht_cap = NULL;
>>>>>>> upstream/android-13
	u32 bcn_channel;
	unsigned short	ht_cap_info;
	unsigned char ht_info_infos_0;
	struct mlme_priv *pmlmepriv = &Adapter->mlmepriv;
<<<<<<< HEAD
=======
	int ssid_len;
>>>>>>> upstream/android-13

	if (is_client_associated_to_ap(Adapter) == false)
		return true;

	len = packet_len - sizeof(struct ieee80211_hdr_3addr);

<<<<<<< HEAD
	if (len > MAX_IE_SZ) {
		DBG_871X("%s IE too long for survey event\n", __func__);
		return _FAIL;
	}

	if (memcmp(cur_network->network.MacAddress, pbssid, 6)) {
		DBG_871X("Oops: rtw_check_network_encrypt linked but recv other bssid bcn\n" MAC_FMT MAC_FMT,
				MAC_ARG(pbssid), MAC_ARG(cur_network->network.MacAddress));
		return true;
	}

	bssid = rtw_zmalloc(sizeof(struct wlan_bssid_ex));
	if (!bssid) {
		DBG_871X("%s rtw_zmalloc fail !!!\n", __func__);
		return true;
	}
=======
	if (len > MAX_IE_SZ)
		return _FAIL;

	if (memcmp(cur_network->network.mac_address, pbssid, 6))
		return true;

	bssid = rtw_zmalloc(sizeof(struct wlan_bssid_ex));
	if (!bssid)
		return true;
>>>>>>> upstream/android-13

	if ((pmlmepriv->timeBcnInfoChkStart != 0) && (jiffies_to_msecs(jiffies - pmlmepriv->timeBcnInfoChkStart) > DISCONNECT_BY_CHK_BCN_FAIL_OBSERV_PERIOD_IN_MS)) {
		pmlmepriv->timeBcnInfoChkStart = 0;
		pmlmepriv->NumOfBcnInfoChkFail = 0;
	}

	subtype = GetFrameSubType(pframe) >> 4;

	if (subtype == WIFI_BEACON)
<<<<<<< HEAD
		bssid->Reserved[0] = 1;

	bssid->Length = sizeof(struct wlan_bssid_ex) - MAX_IE_SZ + len;

	/* below is to copy the information element */
	bssid->IELength = len;
	memcpy(bssid->IEs, (pframe + sizeof(struct ieee80211_hdr_3addr)), bssid->IELength);

	/* check bw and channel offset */
	/* parsing HT_CAP_IE */
	p = rtw_get_ie(bssid->IEs + _FIXED_IE_LENGTH_, _HT_CAPABILITY_IE_, &len, bssid->IELength - _FIXED_IE_LENGTH_);
	if (p && len > 0) {
			pht_cap = (struct rtw_ieee80211_ht_cap *)(p + 2);
			ht_cap_info = le16_to_cpu(pht_cap->cap_info);
	} else {
			ht_cap_info = 0;
	}
	/* parsing HT_INFO_IE */
	p = rtw_get_ie(bssid->IEs + _FIXED_IE_LENGTH_, _HT_ADD_INFO_IE_, &len, bssid->IELength - _FIXED_IE_LENGTH_);
	if (p && len > 0) {
			pht_info = (struct HT_info_element *)(p + 2);
			ht_info_infos_0 = pht_info->infos[0];
	} else {
			ht_info_infos_0 = 0;
	}
	if (ht_cap_info != cur_network->BcnInfo.ht_cap_info ||
		((ht_info_infos_0&0x03) != (cur_network->BcnInfo.ht_info_infos_0&0x03))) {
			DBG_871X("%s bcn now: ht_cap_info:%x ht_info_infos_0:%x\n", __func__,
							ht_cap_info, ht_info_infos_0);
			DBG_871X("%s bcn link: ht_cap_info:%x ht_info_infos_0:%x\n", __func__,
							cur_network->BcnInfo.ht_cap_info, cur_network->BcnInfo.ht_info_infos_0);
			DBG_871X("%s bw mode change\n", __func__);
			{
				/* bcn_info_update */
				cur_network->BcnInfo.ht_cap_info = ht_cap_info;
				cur_network->BcnInfo.ht_info_infos_0 = ht_info_infos_0;
				/* to do : need to check that whether modify related register of BB or not */
			}
			/* goto _mismatch; */
	}

	/* Checking for channel */
	p = rtw_get_ie(bssid->IEs + _FIXED_IE_LENGTH_, _DSSET_IE_, &len, bssid->IELength - _FIXED_IE_LENGTH_);
	if (p) {
			bcn_channel = *(p + 2);
	} else {/* In 5G, some ap do not have DSSET IE checking HT info for channel */
			rtw_get_ie(bssid->IEs + _FIXED_IE_LENGTH_, _HT_ADD_INFO_IE_, &len, bssid->IELength - _FIXED_IE_LENGTH_);
			if (pht_info) {
					bcn_channel = pht_info->primary_channel;
			} else { /* we don't find channel IE, so don't check it */
					/* DBG_871X("Oops: %s we don't find channel IE, so don't check it\n", __func__); */
					bcn_channel = Adapter->mlmeextpriv.cur_channel;
			}
	}
	if (bcn_channel != Adapter->mlmeextpriv.cur_channel) {
			DBG_871X("%s beacon channel:%d cur channel:%d disconnect\n", __func__,
						   bcn_channel, Adapter->mlmeextpriv.cur_channel);
			goto _mismatch;
	}

	/* checking SSID */
	p = rtw_get_ie(bssid->IEs + _FIXED_IE_LENGTH_, _SSID_IE_, &len, bssid->IELength - _FIXED_IE_LENGTH_);
	if (!p) {
		DBG_871X("%s marc: cannot find SSID for survey event\n", __func__);
		hidden_ssid = true;
	} else {
		hidden_ssid = false;
	}

	if ((NULL != p) && (false == hidden_ssid && (*(p + 1)))) {
		memcpy(bssid->Ssid.Ssid, (p + 2), *(p + 1));
		bssid->Ssid.SsidLength = *(p + 1);
	} else {
		bssid->Ssid.SsidLength = 0;
		bssid->Ssid.Ssid[0] = '\0';
	}

	RT_TRACE(_module_rtl871x_mlme_c_, _drv_info_, ("%s bssid.Ssid.Ssid:%s bssid.Ssid.SsidLength:%d "
				"cur_network->network.Ssid.Ssid:%s len:%d\n", __func__, bssid->Ssid.Ssid,
				bssid->Ssid.SsidLength, cur_network->network.Ssid.Ssid,
				cur_network->network.Ssid.SsidLength));

	if (memcmp(bssid->Ssid.Ssid, cur_network->network.Ssid.Ssid, 32) ||
			bssid->Ssid.SsidLength != cur_network->network.Ssid.SsidLength) {
		if (bssid->Ssid.Ssid[0] != '\0' && bssid->Ssid.SsidLength != 0) { /* not hidden ssid */
			DBG_871X("%s(), SSID is not match\n", __func__);
			goto _mismatch;
		}
	}
=======
		bssid->reserved[0] = 1;

	bssid->length = sizeof(struct wlan_bssid_ex) - MAX_IE_SZ + len;

	/* below is to copy the information element */
	bssid->ie_length = len;
	memcpy(bssid->ies, (pframe + sizeof(struct ieee80211_hdr_3addr)), bssid->ie_length);

	/* check bw and channel offset */
	/* parsing HT_CAP_IE */
	p = rtw_get_ie(bssid->ies + _FIXED_IE_LENGTH_, WLAN_EID_HT_CAPABILITY, &len, bssid->ie_length - _FIXED_IE_LENGTH_);
	if (p && len > 0) {
		pht_cap = (struct ieee80211_ht_cap *)(p + 2);
		ht_cap_info = le16_to_cpu(pht_cap->cap_info);
	} else {
		ht_cap_info = 0;
	}
	/* parsing HT_INFO_IE */
	p = rtw_get_ie(bssid->ies + _FIXED_IE_LENGTH_, WLAN_EID_HT_OPERATION, &len, bssid->ie_length - _FIXED_IE_LENGTH_);
	if (p && len > 0) {
		pht_info = (struct HT_info_element *)(p + 2);
		ht_info_infos_0 = pht_info->infos[0];
	} else {
		ht_info_infos_0 = 0;
	}
	if (ht_cap_info != cur_network->bcn_info.ht_cap_info ||
	    ((ht_info_infos_0&0x03) != (cur_network->bcn_info.ht_info_infos_0&0x03))) {
		{
			/* bcn_info_update */
			cur_network->bcn_info.ht_cap_info = ht_cap_info;
			cur_network->bcn_info.ht_info_infos_0 = ht_info_infos_0;
			/* to do : need to check that whether modify related register of BB or not */
		}
		/* goto _mismatch; */
	}

	/* Checking for channel */
	p = rtw_get_ie(bssid->ies + _FIXED_IE_LENGTH_, WLAN_EID_DS_PARAMS, &len, bssid->ie_length - _FIXED_IE_LENGTH_);
	if (p) {
		bcn_channel = *(p + 2);
	} else {/* In 5G, some ap do not have DSSET IE checking HT info for channel */
		rtw_get_ie(bssid->ies + _FIXED_IE_LENGTH_, WLAN_EID_HT_OPERATION,
			   &len, bssid->ie_length - _FIXED_IE_LENGTH_);
		if (pht_info)
			bcn_channel = pht_info->primary_channel;
		else /* we don't find channel IE, so don't check it */
			bcn_channel = Adapter->mlmeextpriv.cur_channel;
	}

	if (bcn_channel != Adapter->mlmeextpriv.cur_channel)
		goto _mismatch;

	/* checking SSID */
	ssid_len = 0;
	p = rtw_get_ie(bssid->ies + _FIXED_IE_LENGTH_, WLAN_EID_SSID, &len, bssid->ie_length - _FIXED_IE_LENGTH_);
	if (p) {
		ssid_len = *(p + 1);
		if (ssid_len > NDIS_802_11_LENGTH_SSID)
			ssid_len = 0;
	}
	memcpy(bssid->ssid.ssid, (p + 2), ssid_len);
	bssid->ssid.ssid_length = ssid_len;

	if (memcmp(bssid->ssid.ssid, cur_network->network.ssid.ssid, 32) ||
			bssid->ssid.ssid_length != cur_network->network.ssid.ssid_length)
		if (bssid->ssid.ssid[0] != '\0' &&
		    bssid->ssid.ssid_length != 0) /* not hidden ssid */
			goto _mismatch;
>>>>>>> upstream/android-13

	/* check encryption info */
	val16 = rtw_get_capability((struct wlan_bssid_ex *)bssid);

	if (val16 & BIT(4))
<<<<<<< HEAD
		bssid->Privacy = 1;
	else
		bssid->Privacy = 0;

	RT_TRACE(_module_rtl871x_mlme_c_, _drv_info_,
			("%s(): cur_network->network.Privacy is %d, bssid.Privacy is %d\n",
			 __func__, cur_network->network.Privacy, bssid->Privacy));
	if (cur_network->network.Privacy != bssid->Privacy) {
		DBG_871X("%s(), privacy is not match\n", __func__);
		goto _mismatch;
	}

	rtw_get_sec_ie(bssid->IEs, bssid->IELength, NULL, &rsn_len, NULL, &wpa_len);

	if (rsn_len > 0) {
		encryp_protocol = ENCRYP_PROTOCOL_WPA2;
	} else if (wpa_len > 0) {
		encryp_protocol = ENCRYP_PROTOCOL_WPA;
	} else {
		if (bssid->Privacy)
			encryp_protocol = ENCRYP_PROTOCOL_WEP;
	}

	if (cur_network->BcnInfo.encryp_protocol != encryp_protocol) {
		DBG_871X("%s(): enctyp is not match\n", __func__);
		goto _mismatch;
	}

	if (encryp_protocol == ENCRYP_PROTOCOL_WPA || encryp_protocol == ENCRYP_PROTOCOL_WPA2) {
		pbuf = rtw_get_wpa_ie(&bssid->IEs[12], &wpa_ielen, bssid->IELength-12);
		if (pbuf && (wpa_ielen > 0)) {
			if (_SUCCESS == rtw_parse_wpa_ie(pbuf, wpa_ielen+2, &group_cipher, &pairwise_cipher, &is_8021x)) {
				RT_TRACE(_module_rtl871x_mlme_c_, _drv_info_,
						("%s pnetwork->pairwise_cipher: %d, group_cipher is %d, is_8021x is %d\n", __func__,
						 pairwise_cipher, group_cipher, is_8021x));
			}
		} else {
			pbuf = rtw_get_wpa2_ie(&bssid->IEs[12], &wpa_ielen, bssid->IELength-12);

			if (pbuf && (wpa_ielen > 0)) {
				if (_SUCCESS == rtw_parse_wpa2_ie(pbuf, wpa_ielen+2, &group_cipher, &pairwise_cipher, &is_8021x)) {
					RT_TRACE(_module_rtl871x_mlme_c_, _drv_info_,
							("%s pnetwork->pairwise_cipher: %d, pnetwork->group_cipher is %d, is_802x is %d\n",
							 __func__, pairwise_cipher, group_cipher, is_8021x));
				}
			}
		}

		RT_TRACE(_module_rtl871x_mlme_c_, _drv_err_,
				("%s cur_network->group_cipher is %d: %d\n", __func__, cur_network->BcnInfo.group_cipher, group_cipher));
		if (pairwise_cipher != cur_network->BcnInfo.pairwise_cipher || group_cipher != cur_network->BcnInfo.group_cipher) {
			DBG_871X("%s pairwise_cipher(%x:%x) or group_cipher(%x:%x) is not match\n", __func__,
					pairwise_cipher, cur_network->BcnInfo.pairwise_cipher,
					group_cipher, cur_network->BcnInfo.group_cipher);
			goto _mismatch;
		}

		if (is_8021x != cur_network->BcnInfo.is_8021x) {
			DBG_871X("%s authentication is not match\n", __func__);
			goto _mismatch;
		}
	}

	kfree((u8 *)bssid);
	return _SUCCESS;

_mismatch:
	kfree((u8 *)bssid);
=======
		bssid->privacy = 1;
	else
		bssid->privacy = 0;

	if (cur_network->network.privacy != bssid->privacy)
		goto _mismatch;

	rtw_get_sec_ie(bssid->ies, bssid->ie_length, NULL, &rsn_len, NULL, &wpa_len);

	if (rsn_len > 0)
		encryp_protocol = ENCRYP_PROTOCOL_WPA2;
	else if (wpa_len > 0)
		encryp_protocol = ENCRYP_PROTOCOL_WPA;
	else
		if (bssid->privacy)
			encryp_protocol = ENCRYP_PROTOCOL_WEP;

	if (cur_network->bcn_info.encryp_protocol != encryp_protocol)
		goto _mismatch;

	if (encryp_protocol == ENCRYP_PROTOCOL_WPA || encryp_protocol == ENCRYP_PROTOCOL_WPA2) {
		pbuf = rtw_get_wpa_ie(&bssid->ies[12], &wpa_ielen, bssid->ie_length-12);
		if (pbuf && (wpa_ielen > 0)) {
			rtw_parse_wpa_ie(pbuf, wpa_ielen + 2, &group_cipher,
					 &pairwise_cipher, &is_8021x);
		} else {
			pbuf = rtw_get_wpa2_ie(&bssid->ies[12], &wpa_ielen, bssid->ie_length-12);

			if (pbuf && (wpa_ielen > 0))
				rtw_parse_wpa2_ie(pbuf, wpa_ielen + 2, &group_cipher,
						  &pairwise_cipher, &is_8021x);
		}

		if (pairwise_cipher != cur_network->bcn_info.pairwise_cipher ||
		    group_cipher != cur_network->bcn_info.group_cipher)
			goto _mismatch;

		if (is_8021x != cur_network->bcn_info.is_8021x)
			goto _mismatch;
	}

	kfree(bssid);
	return _SUCCESS;

_mismatch:
	kfree(bssid);
>>>>>>> upstream/android-13

	if (pmlmepriv->NumOfBcnInfoChkFail == 0)
		pmlmepriv->timeBcnInfoChkStart = jiffies;

	pmlmepriv->NumOfBcnInfoChkFail++;
<<<<<<< HEAD
	DBG_871X("%s by "ADPT_FMT" - NumOfChkFail = %d (SeqNum of this Beacon frame = %d).\n", __func__, ADPT_ARG(Adapter), pmlmepriv->NumOfBcnInfoChkFail, GetSequence(pframe));

	if ((pmlmepriv->timeBcnInfoChkStart != 0) && (jiffies_to_msecs(jiffies - pmlmepriv->timeBcnInfoChkStart) <= DISCONNECT_BY_CHK_BCN_FAIL_OBSERV_PERIOD_IN_MS)
		&& (pmlmepriv->NumOfBcnInfoChkFail >= DISCONNECT_BY_CHK_BCN_FAIL_THRESHOLD)) {
		DBG_871X("%s by "ADPT_FMT" - NumOfChkFail = %d >= threshold : %d (in %d ms), return FAIL.\n", __func__, ADPT_ARG(Adapter), pmlmepriv->NumOfBcnInfoChkFail,
			DISCONNECT_BY_CHK_BCN_FAIL_THRESHOLD, jiffies_to_msecs(jiffies - pmlmepriv->timeBcnInfoChkStart));
=======

	if ((pmlmepriv->timeBcnInfoChkStart != 0) && (jiffies_to_msecs(jiffies - pmlmepriv->timeBcnInfoChkStart) <= DISCONNECT_BY_CHK_BCN_FAIL_OBSERV_PERIOD_IN_MS)
		&& (pmlmepriv->NumOfBcnInfoChkFail >= DISCONNECT_BY_CHK_BCN_FAIL_THRESHOLD)) {
>>>>>>> upstream/android-13
		pmlmepriv->timeBcnInfoChkStart = 0;
		pmlmepriv->NumOfBcnInfoChkFail = 0;
		return _FAIL;
	}

	return _SUCCESS;
}

void update_beacon_info(struct adapter *padapter, u8 *pframe, uint pkt_len, struct sta_info *psta)
{
	unsigned int i;
	unsigned int len;
	struct ndis_80211_var_ie *pIE;

	len = pkt_len - (_BEACON_IE_OFFSET_ + WLAN_HDR_A3_LEN);

	for (i = 0; i < len;) {
		pIE = (struct ndis_80211_var_ie *)(pframe + (_BEACON_IE_OFFSET_ + WLAN_HDR_A3_LEN) + i);

<<<<<<< HEAD
		switch (pIE->ElementID) {
		case _VENDOR_SPECIFIC_IE_:
			/* to update WMM paramter set while receiving beacon */
			if (!memcmp(pIE->data, WMM_PARA_OUI, 6) && pIE->Length == WLAN_WMM_LEN)	/* WMM */
=======
		switch (pIE->element_id) {
		case WLAN_EID_VENDOR_SPECIFIC:
			/* to update WMM parameter set while receiving beacon */
			if (!memcmp(pIE->data, WMM_PARA_OUI, 6) && pIE->length == WLAN_WMM_LEN)	/* WMM */
>>>>>>> upstream/android-13
				if (WMM_param_handler(padapter, pIE))
					report_wmm_edca_update(padapter);

			break;

<<<<<<< HEAD
		case _HT_EXTRA_INFO_IE_:	/* HT info */
=======
		case WLAN_EID_HT_OPERATION:	/* HT info */
>>>>>>> upstream/android-13
			/* HT_info_handler(padapter, pIE); */
			bwmode_update_check(padapter, pIE);
			break;

<<<<<<< HEAD
		case _ERPINFO_IE_:
=======
		case WLAN_EID_ERP_INFO:
>>>>>>> upstream/android-13
			ERP_IE_handler(padapter, pIE);
			VCS_update(padapter, psta);
			break;

		default:
			break;
		}

<<<<<<< HEAD
		i += (pIE->Length + 2);
=======
		i += (pIE->length + 2);
>>>>>>> upstream/android-13
	}
}

unsigned int is_ap_in_tkip(struct adapter *padapter)
{
	u32 i;
	struct ndis_80211_var_ie *pIE;
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);
	struct wlan_bssid_ex		*cur_network = &(pmlmeinfo->network);

	if (rtw_get_capability((struct wlan_bssid_ex *)cur_network) & WLAN_CAPABILITY_PRIVACY) {
<<<<<<< HEAD
		for (i = sizeof(struct ndis_802_11_fix_ie); i < pmlmeinfo->network.IELength;) {
			pIE = (struct ndis_80211_var_ie *)(pmlmeinfo->network.IEs + i);

			switch (pIE->ElementID) {
			case _VENDOR_SPECIFIC_IE_:
=======
		for (i = sizeof(struct ndis_802_11_fix_ie); i < pmlmeinfo->network.ie_length;) {
			pIE = (struct ndis_80211_var_ie *)(pmlmeinfo->network.ies + i);

			switch (pIE->element_id) {
			case WLAN_EID_VENDOR_SPECIFIC:
>>>>>>> upstream/android-13
				if ((!memcmp(pIE->data, RTW_WPA_OUI, 4)) && (!memcmp((pIE->data + 12), WPA_TKIP_CIPHER, 4)))
					return true;

				break;

<<<<<<< HEAD
			case _RSN_IE_2_:
				if (!memcmp((pIE->data + 8), RSN_TKIP_CIPHER, 4))
					return true;
=======
			case WLAN_EID_RSN:
				if (!memcmp((pIE->data + 8), RSN_TKIP_CIPHER, 4))
					return true;
				break;
>>>>>>> upstream/android-13

			default:
				break;
			}

<<<<<<< HEAD
			i += (pIE->Length + 2);
		}

		return false;
	} else
		return false;
=======
			i += (pIE->length + 2);
		}

		return false;
	} else {
		return false;
	}
>>>>>>> upstream/android-13
}

int support_short_GI(struct adapter *padapter, struct HT_caps_element *pHT_caps, u8 bwmode)
{
<<<<<<< HEAD
	unsigned char 				bit_offset;
=======
	unsigned char bit_offset;
>>>>>>> upstream/android-13
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);

	if (!(pmlmeinfo->HT_enable))
		return _FAIL;

	bit_offset = (bwmode & CHANNEL_WIDTH_40) ? 6 : 5;

	if (le16_to_cpu(pHT_caps->u.HT_cap_element.HT_caps_info) & (0x1 << bit_offset))
		return _SUCCESS;
	else
		return _FAIL;
}

unsigned char get_highest_rate_idx(u32 mask)
{
	int i;
	unsigned char rate_idx = 0;

	for (i = 31; i >= 0; i--) {
		if (mask & BIT(i)) {
			rate_idx = i;
			break;
		}
	}

	return rate_idx;
}

void Update_RA_Entry(struct adapter *padapter, struct sta_info *psta)
{
	rtw_hal_update_ra_mask(psta, 0);
}

<<<<<<< HEAD
void enable_rate_adaptive(struct adapter *padapter, struct sta_info *psta);
void enable_rate_adaptive(struct adapter *padapter, struct sta_info *psta)
{
	Update_RA_Entry(padapter, psta);
}

void set_sta_rate(struct adapter *padapter, struct sta_info *psta)
{
	/* rate adaptive */
	enable_rate_adaptive(padapter, psta);
=======
void set_sta_rate(struct adapter *padapter, struct sta_info *psta)
{
	/* rate adaptive */
	Update_RA_Entry(padapter, psta);
}

static u32 get_realtek_assoc_AP_vender(struct ndis_80211_var_ie *pIE)
{
	u32 Vender = HT_IOT_PEER_REALTEK;

	if (pIE->length >= 5) {
		if (pIE->data[4] == 1)
			/* if (pIE->data[5] & RT_HT_CAP_USE_LONG_PREAMBLE) */
			/* bssDesc->BssHT.RT2RT_HT_Mode |= RT_HT_CAP_USE_LONG_PREAMBLE; */
			if (pIE->data[5] & RT_HT_CAP_USE_92SE)
				/* bssDesc->BssHT.RT2RT_HT_Mode |= RT_HT_CAP_USE_92SE; */
				Vender = HT_IOT_PEER_REALTEK_92SE;

		if (pIE->data[5] & RT_HT_CAP_USE_SOFTAP)
			Vender = HT_IOT_PEER_REALTEK_SOFTAP;

		if (pIE->data[4] == 2) {
			if (pIE->data[6] & RT_HT_CAP_USE_JAGUAR_BCUT)
				Vender = HT_IOT_PEER_REALTEK_JAGUAR_BCUTAP;

			if (pIE->data[6] & RT_HT_CAP_USE_JAGUAR_CCUT)
				Vender = HT_IOT_PEER_REALTEK_JAGUAR_CCUTAP;
		}
	}

	return Vender;
>>>>>>> upstream/android-13
}

unsigned char check_assoc_AP(u8 *pframe, uint len)
{
	unsigned int	i;
	struct ndis_80211_var_ie *pIE;

	for (i = sizeof(struct ndis_802_11_fix_ie); i < len;) {
		pIE = (struct ndis_80211_var_ie *)(pframe + i);

<<<<<<< HEAD
		switch (pIE->ElementID) {
		case _VENDOR_SPECIFIC_IE_:
			if ((!memcmp(pIE->data, ARTHEROS_OUI1, 3)) || (!memcmp(pIE->data, ARTHEROS_OUI2, 3))) {
				DBG_871X("link to Artheros AP\n");
				return HT_IOT_PEER_ATHEROS;
			} else if ((!memcmp(pIE->data, BROADCOM_OUI1, 3))
						|| (!memcmp(pIE->data, BROADCOM_OUI2, 3))
						|| (!memcmp(pIE->data, BROADCOM_OUI3, 3))) {
				DBG_871X("link to Broadcom AP\n");
				return HT_IOT_PEER_BROADCOM;
			} else if (!memcmp(pIE->data, MARVELL_OUI, 3)) {
				DBG_871X("link to Marvell AP\n");
				return HT_IOT_PEER_MARVELL;
			} else if (!memcmp(pIE->data, RALINK_OUI, 3)) {
				DBG_871X("link to Ralink AP\n");
				return HT_IOT_PEER_RALINK;
			} else if (!memcmp(pIE->data, CISCO_OUI, 3)) {
				DBG_871X("link to Cisco AP\n");
				return HT_IOT_PEER_CISCO;
			} else if (!memcmp(pIE->data, REALTEK_OUI, 3)) {
				u32 Vender = HT_IOT_PEER_REALTEK;

				if (pIE->Length >= 5) {
					if (pIE->data[4] == 1)
						/* if (pIE->data[5] & RT_HT_CAP_USE_LONG_PREAMBLE) */
						/* 	bssDesc->BssHT.RT2RT_HT_Mode |= RT_HT_CAP_USE_LONG_PREAMBLE; */
						if (pIE->data[5] & RT_HT_CAP_USE_92SE)
							/* bssDesc->BssHT.RT2RT_HT_Mode |= RT_HT_CAP_USE_92SE; */
							Vender = HT_IOT_PEER_REALTEK_92SE;

					if (pIE->data[5] & RT_HT_CAP_USE_SOFTAP)
						Vender = HT_IOT_PEER_REALTEK_SOFTAP;

					if (pIE->data[4] == 2) {
						if (pIE->data[6] & RT_HT_CAP_USE_JAGUAR_BCUT) {
							Vender = HT_IOT_PEER_REALTEK_JAGUAR_BCUTAP;
							DBG_871X("link to Realtek JAGUAR_BCUTAP\n");
						}
						if (pIE->data[6] & RT_HT_CAP_USE_JAGUAR_CCUT) {
							Vender = HT_IOT_PEER_REALTEK_JAGUAR_CCUTAP;
							DBG_871X("link to Realtek JAGUAR_CCUTAP\n");
						}
					}
				}

				DBG_871X("link to Realtek AP\n");
				return Vender;
			} else if (!memcmp(pIE->data, AIRGOCAP_OUI, 3)) {
				DBG_871X("link to Airgo Cap\n");
				return HT_IOT_PEER_AIRGO;
			} else
=======
		switch (pIE->element_id) {
		case WLAN_EID_VENDOR_SPECIFIC:
			if ((!memcmp(pIE->data, ARTHEROS_OUI1, 3)) || (!memcmp(pIE->data, ARTHEROS_OUI2, 3)))
				return HT_IOT_PEER_ATHEROS;
			else if ((!memcmp(pIE->data, BROADCOM_OUI1, 3)) ||
				 (!memcmp(pIE->data, BROADCOM_OUI2, 3)) ||
				 (!memcmp(pIE->data, BROADCOM_OUI3, 3)))
				return HT_IOT_PEER_BROADCOM;
			else if (!memcmp(pIE->data, MARVELL_OUI, 3))
				return HT_IOT_PEER_MARVELL;
			else if (!memcmp(pIE->data, RALINK_OUI, 3))
				return HT_IOT_PEER_RALINK;
			else if (!memcmp(pIE->data, CISCO_OUI, 3))
				return HT_IOT_PEER_CISCO;
			else if (!memcmp(pIE->data, REALTEK_OUI, 3))
				return get_realtek_assoc_AP_vender(pIE);
			else if (!memcmp(pIE->data, AIRGOCAP_OUI, 3))
				return HT_IOT_PEER_AIRGO;
			else
>>>>>>> upstream/android-13
				break;

		default:
			break;
		}

<<<<<<< HEAD
		i += (pIE->Length + 2);
	}

	DBG_871X("link to new AP\n");
=======
		i += (pIE->length + 2);
	}

>>>>>>> upstream/android-13
	return HT_IOT_PEER_UNKNOWN;
}

void update_IOT_info(struct adapter *padapter)
{
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);

	switch (pmlmeinfo->assoc_AP_vendor) {
	case HT_IOT_PEER_MARVELL:
		pmlmeinfo->turboMode_cts2self = 1;
		pmlmeinfo->turboMode_rtsen = 0;
		break;

	case HT_IOT_PEER_RALINK:
		pmlmeinfo->turboMode_cts2self = 0;
		pmlmeinfo->turboMode_rtsen = 1;
		/* disable high power */
		Switch_DM_Func(padapter, (~DYNAMIC_BB_DYNAMIC_TXPWR), false);
		break;
	case HT_IOT_PEER_REALTEK:
		/* rtw_write16(padapter, 0x4cc, 0xffff); */
		/* rtw_write16(padapter, 0x546, 0x01c0); */
		/* disable high power */
		Switch_DM_Func(padapter, (~DYNAMIC_BB_DYNAMIC_TXPWR), false);
		break;
	default:
		pmlmeinfo->turboMode_cts2self = 0;
		pmlmeinfo->turboMode_rtsen = 1;
		break;
	}
}

void update_capinfo(struct adapter *Adapter, u16 updateCap)
{
	struct mlme_ext_priv *pmlmeext = &Adapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);
	bool		ShortPreamble;

	/*  Check preamble mode, 2005.01.06, by rcnjko. */
	/*  Mark to update preamble value forever, 2008.03.18 by lanhsin */
	/* if (pMgntInfo->RegPreambleMode == PREAMBLE_AUTO) */
	{
		if (updateCap & cShortPreamble) {
			/*  Short Preamble */
			if (pmlmeinfo->preamble_mode != PREAMBLE_SHORT) { /*  PREAMBLE_LONG or PREAMBLE_AUTO */
				ShortPreamble = true;
				pmlmeinfo->preamble_mode = PREAMBLE_SHORT;
				rtw_hal_set_hwreg(Adapter, HW_VAR_ACK_PREAMBLE, (u8 *)&ShortPreamble);
			}
<<<<<<< HEAD
		} else{
=======
		} else {
>>>>>>> upstream/android-13
			/*  Long Preamble */
			if (pmlmeinfo->preamble_mode != PREAMBLE_LONG) { /*  PREAMBLE_SHORT or PREAMBLE_AUTO */
				ShortPreamble = false;
				pmlmeinfo->preamble_mode = PREAMBLE_LONG;
				rtw_hal_set_hwreg(Adapter, HW_VAR_ACK_PREAMBLE, (u8 *)&ShortPreamble);
			}
		}
	}

<<<<<<< HEAD
	if (updateCap & cIBSS)
		/* Filen: See 802.11-2007 p.91 */
		pmlmeinfo->slotTime = NON_SHORT_SLOT_TIME;
	else {
		/* Filen: See 802.11-2007 p.90 */
		if (pmlmeext->cur_wireless_mode & (WIRELESS_11_24N | WIRELESS_11A | WIRELESS_11_5N | WIRELESS_11AC))
			pmlmeinfo->slotTime = SHORT_SLOT_TIME;
		else if (pmlmeext->cur_wireless_mode & (WIRELESS_11G)) {
=======
	if (updateCap & cIBSS) {
		/* Filen: See 802.11-2007 p.91 */
		pmlmeinfo->slotTime = NON_SHORT_SLOT_TIME;
	} else {
		/* Filen: See 802.11-2007 p.90 */
		if (pmlmeext->cur_wireless_mode & (WIRELESS_11_24N)) {
			pmlmeinfo->slotTime = SHORT_SLOT_TIME;
		} else if (pmlmeext->cur_wireless_mode & (WIRELESS_11G)) {
>>>>>>> upstream/android-13
			if ((updateCap & cShortSlotTime) /* && (!(pMgntInfo->pHTInfo->RT2RT_HT_Mode & RT_HT_CAP_USE_LONG_PREAMBLE)) */)
				/*  Short Slot Time */
				pmlmeinfo->slotTime = SHORT_SLOT_TIME;
			else
				/*  Long Slot Time */
				pmlmeinfo->slotTime = NON_SHORT_SLOT_TIME;
<<<<<<< HEAD
		} else
			/* B Mode */
			pmlmeinfo->slotTime = NON_SHORT_SLOT_TIME;
=======
		} else {
			/* B Mode */
			pmlmeinfo->slotTime = NON_SHORT_SLOT_TIME;
		}
>>>>>>> upstream/android-13
	}

	rtw_hal_set_hwreg(Adapter, HW_VAR_SLOT_TIME, &pmlmeinfo->slotTime);
}

void update_wireless_mode(struct adapter *padapter)
{
<<<<<<< HEAD
	int ratelen, network_type = 0;
	u32 SIFS_Timer;
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);
	struct wlan_bssid_ex		*cur_network = &(pmlmeinfo->network);
	unsigned char 		*rate = cur_network->SupportedRates;

	ratelen = rtw_get_rateset_len(cur_network->SupportedRates);
=======
	int network_type = 0;
	u32 SIFS_Timer;
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);
	struct wlan_bssid_ex *cur_network = &(pmlmeinfo->network);
	unsigned char *rate = cur_network->supported_rates;
>>>>>>> upstream/android-13

	if ((pmlmeinfo->HT_info_enable) && (pmlmeinfo->HT_caps_enable))
		pmlmeinfo->HT_enable = 1;

<<<<<<< HEAD
	if (pmlmeext->cur_channel > 14) {
		if (pmlmeinfo->VHT_enable)
			network_type = WIRELESS_11AC;
		else if (pmlmeinfo->HT_enable)
			network_type = WIRELESS_11_5N;

		network_type |= WIRELESS_11A;
	} else{
		if (pmlmeinfo->VHT_enable)
			network_type = WIRELESS_11AC;
		else if (pmlmeinfo->HT_enable)
			network_type = WIRELESS_11_24N;

		if ((cckratesonly_included(rate, ratelen)) == true)
			network_type |= WIRELESS_11B;
		else if ((cckrates_included(rate, ratelen)) == true)
			network_type |= WIRELESS_11BG;
		else
			network_type |= WIRELESS_11G;
	}
=======
	if (pmlmeinfo->HT_enable)
		network_type = WIRELESS_11_24N;

	if (rtw_is_cckratesonly_included(rate))
		network_type |= WIRELESS_11B;
	else if (rtw_is_cckrates_included(rate))
		network_type |= WIRELESS_11BG;
	else
		network_type |= WIRELESS_11G;
>>>>>>> upstream/android-13

	pmlmeext->cur_wireless_mode = network_type & padapter->registrypriv.wireless_mode;

	SIFS_Timer = 0x0a0a0808; /* 0x0808 -> for CCK, 0x0a0a -> for OFDM */
													/* change this value if having IOT issues. */

	padapter->HalFunc.SetHwRegHandler(padapter, HW_VAR_RESP_SIFS,  (u8 *)&SIFS_Timer);

	padapter->HalFunc.SetHwRegHandler(padapter, HW_VAR_WIRELESS_MODE,  (u8 *)&(pmlmeext->cur_wireless_mode));

	if (pmlmeext->cur_wireless_mode & WIRELESS_11B)
		update_mgnt_tx_rate(padapter, IEEE80211_CCK_RATE_1MB);
<<<<<<< HEAD
	 else
=======
	else
>>>>>>> upstream/android-13
		update_mgnt_tx_rate(padapter, IEEE80211_OFDM_RATE_6MB);
}

void update_sta_basic_rate(struct sta_info *psta, u8 wireless_mode)
{
<<<<<<< HEAD
	if (IsSupportedTxCCK(wireless_mode)) {
		/*  Only B, B/G, and B/G/N AP could use CCK rate */
		memcpy(psta->bssrateset, rtw_basic_rate_cck, 4);
		psta->bssratelen = 4;
	} else{
=======
	if (is_supported_tx_cck(wireless_mode)) {
		/*  Only B, B/G, and B/G/N AP could use CCK rate */
		memcpy(psta->bssrateset, rtw_basic_rate_cck, 4);
		psta->bssratelen = 4;
	} else {
>>>>>>> upstream/android-13
		memcpy(psta->bssrateset, rtw_basic_rate_ofdm, 3);
		psta->bssratelen = 3;
	}
}

int update_sta_support_rate(struct adapter *padapter, u8 *pvar_ie, uint var_ie_len, int cam_idx)
{
	unsigned int	ie_len;
	struct ndis_80211_var_ie *pIE;
	int	supportRateNum = 0;
	struct mlme_ext_priv *pmlmeext = &(padapter->mlmeextpriv);
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);

<<<<<<< HEAD
	pIE = (struct ndis_80211_var_ie *)rtw_get_ie(pvar_ie, _SUPPORTEDRATES_IE_, &ie_len, var_ie_len);
=======
	pIE = (struct ndis_80211_var_ie *)rtw_get_ie(pvar_ie, WLAN_EID_SUPP_RATES, &ie_len, var_ie_len);
>>>>>>> upstream/android-13
	if (!pIE)
		return _FAIL;
	if (ie_len > sizeof(pmlmeinfo->FW_sta_info[cam_idx].SupportedRates))
		return _FAIL;

	memcpy(pmlmeinfo->FW_sta_info[cam_idx].SupportedRates, pIE->data, ie_len);
	supportRateNum = ie_len;

<<<<<<< HEAD
	pIE = (struct ndis_80211_var_ie *)rtw_get_ie(pvar_ie, _EXT_SUPPORTEDRATES_IE_, &ie_len, var_ie_len);
=======
	pIE = (struct ndis_80211_var_ie *)rtw_get_ie(pvar_ie, WLAN_EID_EXT_SUPP_RATES, &ie_len, var_ie_len);
>>>>>>> upstream/android-13
	if (pIE && (ie_len <= sizeof(pmlmeinfo->FW_sta_info[cam_idx].SupportedRates) - supportRateNum))
		memcpy((pmlmeinfo->FW_sta_info[cam_idx].SupportedRates + supportRateNum), pIE->data, ie_len);

	return _SUCCESS;
}

void process_addba_req(struct adapter *padapter, u8 *paddba_req, u8 *addr)
{
	struct sta_info *psta;
<<<<<<< HEAD
	u16 tid, start_seq, param;
=======
	u16 tid, param;
>>>>>>> upstream/android-13
	struct recv_reorder_ctrl *preorder_ctrl;
	struct sta_priv *pstapriv = &padapter->stapriv;
	struct ADDBA_request *preq = (struct ADDBA_request *)paddba_req;
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);

	psta = rtw_get_stainfo(pstapriv, addr);

	if (psta) {
<<<<<<< HEAD
		start_seq = le16_to_cpu(preq->BA_starting_seqctrl) >> 4;

=======
>>>>>>> upstream/android-13
		param = le16_to_cpu(preq->BA_para_set);
		tid = (param>>2)&0x0f;

		preorder_ctrl = &psta->recvreorder_ctrl[tid];

<<<<<<< HEAD
		#ifdef CONFIG_UPDATE_INDICATE_SEQ_WHILE_PROCESS_ADDBA_REQ
		preorder_ctrl->indicate_seq = start_seq;
		#ifdef DBG_RX_SEQ
		DBG_871X("DBG_RX_SEQ %s:%d IndicateSeq: %d, start_seq: %d\n", __func__, __LINE__,
			preorder_ctrl->indicate_seq, start_seq);
		#endif
		#else
		preorder_ctrl->indicate_seq = 0xffff;
		#endif
=======
		preorder_ctrl->indicate_seq = 0xffff;
>>>>>>> upstream/android-13

		preorder_ctrl->enable = pmlmeinfo->accept_addba_req;
	}
}

void update_TSF(struct mlme_ext_priv *pmlmeext, u8 *pframe, uint len)
{
	u8 *pIE;
	__le32 *pbuf;

	pIE = pframe + sizeof(struct ieee80211_hdr_3addr);
	pbuf = (__le32 *)pIE;

	pmlmeext->TSFValue = le32_to_cpu(*(pbuf+1));

	pmlmeext->TSFValue = pmlmeext->TSFValue << 32;

	pmlmeext->TSFValue |= le32_to_cpu(*pbuf);
}

void correct_TSF(struct adapter *padapter, struct mlme_ext_priv *pmlmeext)
{
	rtw_hal_set_hwreg(padapter, HW_VAR_CORRECT_TSF, NULL);
}

void adaptive_early_32k(struct mlme_ext_priv *pmlmeext, u8 *pframe, uint len)
{
	int i;
	u8 *pIE;
	__le32 *pbuf;
	u64 tsf = 0;
	u32 delay_ms;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);

	pmlmeext->bcn_cnt++;

	pIE = pframe + sizeof(struct ieee80211_hdr_3addr);
	pbuf = (__le32 *)pIE;

	tsf = le32_to_cpu(*(pbuf+1));
	tsf = tsf << 32;
	tsf |= le32_to_cpu(*pbuf);

<<<<<<< HEAD
	/* DBG_871X("%s(): tsf_upper = 0x%08x, tsf_lower = 0x%08x\n", __func__, (u32)(tsf>>32), (u32)tsf); */

	/* delay = (timestamp mod 1024*100)/1000 (unit: ms) */
	/* delay_ms = do_div(tsf, (pmlmeinfo->bcn_interval*1024))/1000; */
	delay_ms = rtw_modular64(tsf, (pmlmeinfo->bcn_interval*1024));
=======
	/* delay = (timestamp mod 1024*100)/1000 (unit: ms) */
	/* delay_ms = do_div(tsf, (pmlmeinfo->bcn_interval*1024))/1000; */
	delay_ms = do_div(tsf, (pmlmeinfo->bcn_interval*1024));
>>>>>>> upstream/android-13
	delay_ms = delay_ms/1000;

	if (delay_ms >= 8)
		pmlmeext->bcn_delay_cnt[8]++;
		/* pmlmeext->bcn_delay_ratio[8] = (pmlmeext->bcn_delay_cnt[8] * 100) /pmlmeext->bcn_cnt; */
	else
		pmlmeext->bcn_delay_cnt[delay_ms]++;
		/* pmlmeext->bcn_delay_ratio[delay_ms] = (pmlmeext->bcn_delay_cnt[delay_ms] * 100) /pmlmeext->bcn_cnt; */

/*
<<<<<<< HEAD
	DBG_871X("%s(): (a)bcn_cnt = %d\n", __func__, pmlmeext->bcn_cnt);


	for (i = 0; i<9; i++)
	{
		DBG_871X("%s():bcn_delay_cnt[%d]=%d,  bcn_delay_ratio[%d]=%d\n", __func__, i,
=======

	for (i = 0; i<9; i++)
	{
>>>>>>> upstream/android-13
			pmlmeext->bcn_delay_cnt[i] , i, pmlmeext->bcn_delay_ratio[i]);
	}
*/

	/* dump for  adaptive_early_32k */
	if (pmlmeext->bcn_cnt > 100 && (pmlmeext->adaptive_tsf_done == true)) {
		u8 ratio_20_delay, ratio_80_delay;
		u8 DrvBcnEarly, DrvBcnTimeOut;

		ratio_20_delay = 0;
		ratio_80_delay = 0;
		DrvBcnEarly = 0xff;
		DrvBcnTimeOut = 0xff;

<<<<<<< HEAD
		DBG_871X("%s(): bcn_cnt = %d\n", __func__, pmlmeext->bcn_cnt);

		for (i = 0; i < 9; i++) {
			pmlmeext->bcn_delay_ratio[i] = (pmlmeext->bcn_delay_cnt[i] * 100) / pmlmeext->bcn_cnt;

			DBG_871X("%s():bcn_delay_cnt[%d]=%d,  bcn_delay_ratio[%d]=%d\n", __func__, i,
				pmlmeext->bcn_delay_cnt[i], i, pmlmeext->bcn_delay_ratio[i]);

			ratio_20_delay += pmlmeext->bcn_delay_ratio[i];
			ratio_80_delay += pmlmeext->bcn_delay_ratio[i];

			if (ratio_20_delay > 20 && DrvBcnEarly == 0xff) {
				DrvBcnEarly = i;
				DBG_871X("%s(): DrvBcnEarly = %d\n", __func__, DrvBcnEarly);
			}

			if (ratio_80_delay > 80 && DrvBcnTimeOut == 0xff) {
				DrvBcnTimeOut = i;
				DBG_871X("%s(): DrvBcnTimeOut = %d\n", __func__, DrvBcnTimeOut);
			}
=======
		for (i = 0; i < 9; i++) {
			pmlmeext->bcn_delay_ratio[i] = (pmlmeext->bcn_delay_cnt[i] * 100) / pmlmeext->bcn_cnt;

			ratio_20_delay += pmlmeext->bcn_delay_ratio[i];
			ratio_80_delay += pmlmeext->bcn_delay_ratio[i];

			if (ratio_20_delay > 20 && DrvBcnEarly == 0xff)
				DrvBcnEarly = i;

			if (ratio_80_delay > 80 && DrvBcnTimeOut == 0xff)
				DrvBcnTimeOut = i;
>>>>>>> upstream/android-13

			/* reset adaptive_early_32k cnt */
			pmlmeext->bcn_delay_cnt[i] = 0;
			pmlmeext->bcn_delay_ratio[i] = 0;
		}

		pmlmeext->DrvBcnEarly = DrvBcnEarly;
		pmlmeext->DrvBcnTimeOut = DrvBcnTimeOut;

		pmlmeext->bcn_cnt = 0;
	}
}

<<<<<<< HEAD
void beacon_timing_control(struct adapter *padapter)
{
	rtw_hal_bcn_related_reg_setting(padapter);
}

=======
>>>>>>> upstream/android-13
void rtw_alloc_macid(struct adapter *padapter, struct sta_info *psta)
{
	int i;
	u8 bc_addr[ETH_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	struct dvobj_priv *pdvobj = adapter_to_dvobj(padapter);

	if (!memcmp(psta->hwaddr, bc_addr, ETH_ALEN))
		return;

	if (!memcmp(psta->hwaddr, myid(&padapter->eeprompriv), ETH_ALEN)) {
		psta->mac_id = NUM_STA;
		return;
	}

	spin_lock_bh(&pdvobj->lock);
	for (i = 0; i < NUM_STA; i++) {
		if (pdvobj->macid[i] == false) {
			pdvobj->macid[i]  = true;
			break;
		}
	}
	spin_unlock_bh(&pdvobj->lock);

<<<<<<< HEAD
	if (i > (NUM_STA-1)) {
		psta->mac_id = NUM_STA;
		DBG_871X("  no room for more MACIDs\n");
	} else{
		psta->mac_id = i;
		DBG_871X("%s = %d\n", __func__, psta->mac_id);
	}
=======
	if (i > (NUM_STA - 1))
		psta->mac_id = NUM_STA;
	else
		psta->mac_id = i;
>>>>>>> upstream/android-13
}

void rtw_release_macid(struct adapter *padapter, struct sta_info *psta)
{
	u8 bc_addr[ETH_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	struct dvobj_priv *pdvobj = adapter_to_dvobj(padapter);

	if (!memcmp(psta->hwaddr, bc_addr, ETH_ALEN))
		return;

	if (!memcmp(psta->hwaddr, myid(&padapter->eeprompriv), ETH_ALEN))
		return;

	spin_lock_bh(&pdvobj->lock);
	if (psta->mac_id < NUM_STA && psta->mac_id != 1) {
		if (pdvobj->macid[psta->mac_id] == true) {
<<<<<<< HEAD
			DBG_871X("%s = %d\n", __func__, psta->mac_id);
=======
>>>>>>> upstream/android-13
			pdvobj->macid[psta->mac_id] = false;
			psta->mac_id = NUM_STA;
		}
	}
	spin_unlock_bh(&pdvobj->lock);
}
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
/* For 8188E RA */
u8 rtw_search_max_mac_id(struct adapter *padapter)
{
	u8 max_mac_id = 0;
	struct dvobj_priv *pdvobj = adapter_to_dvobj(padapter);
	int i;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	spin_lock_bh(&pdvobj->lock);
	for (i = (NUM_STA-1); i >= 0 ; i--) {
		if (pdvobj->macid[i] == true)
			break;
	}
	max_mac_id = i;
	spin_unlock_bh(&pdvobj->lock);

	return max_mac_id;
}

struct adapter *dvobj_get_port0_adapter(struct dvobj_priv *dvobj)
{
	if (get_iface_type(dvobj->padapters[i]) != IFACE_PORT0)
		return NULL;

	return dvobj->padapters;
}
<<<<<<< HEAD

#ifdef CONFIG_GPIO_API
int rtw_get_gpio(struct net_device *netdev, int gpio_num)
{
	u8 value;
	u8 direction;
	struct adapter *adapter = (struct adapter *)rtw_netdev_priv(netdev);
	struct pwrctrl_priv *pwrpriv = adapter_to_pwrctl(adapter);

	rtw_ps_deny(adapter, PS_DENY_IOCTL);

	DBG_871X("rf_pwrstate = 0x%02x\n", pwrpriv->rf_pwrstate);
	LeaveAllPowerSaveModeDirect(adapter);

	/* Read GPIO Direction */
	direction = (rtw_read8(adapter, REG_GPIO_PIN_CTRL + 2) & BIT(gpio_num)) >> gpio_num;

	/* According the direction to read register value */
	if (direction)
		value = (rtw_read8(adapter, REG_GPIO_PIN_CTRL + 1) & BIT(gpio_num)) >> gpio_num;
	else
		value = (rtw_read8(adapter, REG_GPIO_PIN_CTRL) & BIT(gpio_num)) >> gpio_num;

	rtw_ps_deny_cancel(adapter, PS_DENY_IOCTL);
	DBG_871X("%s direction =%d value =%d\n", __func__, direction, value);

	return value;
}
EXPORT_SYMBOL(rtw_get_gpio);

int  rtw_set_gpio_output_value(struct net_device *netdev, int gpio_num, bool isHigh)
{
	u8 direction = 0;
	u8 res = -1;
	struct adapter *adapter = (struct adapter *)rtw_netdev_priv(netdev);

	/* Check GPIO is 4~7 */
	if (gpio_num > 7 || gpio_num < 4) {
		DBG_871X("%s The gpio number does not included 4~7.\n", __func__);
		return -1;
	}

	rtw_ps_deny(adapter, PS_DENY_IOCTL);

	LeaveAllPowerSaveModeDirect(adapter);

	/* Read GPIO direction */
	direction = (rtw_read8(adapter, REG_GPIO_PIN_CTRL + 2) & BIT(gpio_num)) >> gpio_num;

	/* If GPIO is output direction, setting value. */
	if (direction) {
		if (isHigh)
			rtw_write8(adapter, REG_GPIO_PIN_CTRL + 1, rtw_read8(adapter, REG_GPIO_PIN_CTRL + 1) | BIT(gpio_num));
		else
			rtw_write8(adapter, REG_GPIO_PIN_CTRL + 1, rtw_read8(adapter, REG_GPIO_PIN_CTRL + 1) & ~BIT(gpio_num));

		DBG_871X("%s Set gpio %x[%d]=%d\n", __func__, REG_GPIO_PIN_CTRL+1, gpio_num, isHigh);
		res = 0;
	} else{
		DBG_871X("%s The gpio is input, not be set!\n", __func__);
		res = -1;
	}

	rtw_ps_deny_cancel(adapter, PS_DENY_IOCTL);
	return res;
}
EXPORT_SYMBOL(rtw_set_gpio_output_value);

int rtw_config_gpio(struct net_device *netdev, int gpio_num, bool isOutput)
{
	struct adapter *adapter = (struct adapter *)rtw_netdev_priv(netdev);

	if (gpio_num > 7 || gpio_num < 4) {
		DBG_871X("%s The gpio number does not included 4~7.\n", __func__);
		return -1;
	}

	DBG_871X("%s gpio_num =%d direction =%d\n", __func__, gpio_num, isOutput);

	rtw_ps_deny(adapter, PS_DENY_IOCTL);

	LeaveAllPowerSaveModeDirect(adapter);

	if (isOutput)
		rtw_write8(adapter, REG_GPIO_PIN_CTRL + 2, rtw_read8(adapter, REG_GPIO_PIN_CTRL + 2) | BIT(gpio_num));
	else
		rtw_write8(adapter, REG_GPIO_PIN_CTRL + 2, rtw_read8(adapter, REG_GPIO_PIN_CTRL + 2) & ~BIT(gpio_num));

	rtw_ps_deny_cancel(adapter, PS_DENY_IOCTL);

	return 0;
}
EXPORT_SYMBOL(rtw_config_gpio);
#endif

#if defined(CONFIG_WOWLAN) || defined(CONFIG_AP_WOWLAN)
void rtw_get_current_ip_address(struct adapter *padapter, u8 *pcurrentip)
{
	struct mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &(pmlmeext->mlmext_info);
	struct in_device *my_ip_ptr = padapter->pnetdev->ip_ptr;
	u8 ipaddress[4];

	if ((pmlmeinfo->state & WIFI_FW_LINKING_STATE) ||
			pmlmeinfo->state & WIFI_FW_AP_STATE) {
		if (my_ip_ptr) {
			struct in_ifaddr *my_ifa_list = my_ip_ptr->ifa_list;
			if (my_ifa_list) {
				ipaddress[0] = my_ifa_list->ifa_address & 0xFF;
				ipaddress[1] = (my_ifa_list->ifa_address >> 8) & 0xFF;
				ipaddress[2] = (my_ifa_list->ifa_address >> 16) & 0xFF;
				ipaddress[3] = my_ifa_list->ifa_address >> 24;
				DBG_871X("%s: %d.%d.%d.%d ==========\n", __func__,
						ipaddress[0], ipaddress[1], ipaddress[2], ipaddress[3]);
				memcpy(pcurrentip, ipaddress, 4);
			}
		}
	}
}
#endif
#ifdef CONFIG_WOWLAN
void rtw_get_sec_iv(struct adapter *padapter, u8 *pcur_dot11txpn, u8 *StaAddr)
{
	struct sta_info 	*psta;
	struct security_priv *psecpriv = &padapter->securitypriv;

	memset(pcur_dot11txpn, 0, 8);
	if (NULL == StaAddr)
		return;
	psta = rtw_get_stainfo(&padapter->stapriv, StaAddr);
	DBG_871X("%s(): StaAddr: %02x %02x %02x %02x %02x %02x\n",
		__func__, StaAddr[0], StaAddr[1], StaAddr[2],
		StaAddr[3], StaAddr[4], StaAddr[5]);

	if (psta) {
		if (psecpriv->dot11PrivacyAlgrthm != _NO_PRIVACY_ && psta->dot11txpn.val > 0)
			psta->dot11txpn.val--;
		AES_IV(pcur_dot11txpn, psta->dot11txpn, 0);

		DBG_871X("%s(): CurrentIV: %02x %02x %02x %02x %02x %02x %02x %02x\n"
		, __func__, pcur_dot11txpn[0], pcur_dot11txpn[1],
		pcur_dot11txpn[2], pcur_dot11txpn[3], pcur_dot11txpn[4],
		pcur_dot11txpn[5], pcur_dot11txpn[6], pcur_dot11txpn[7]);
	}
}
void rtw_set_sec_pn(struct adapter *padapter)
{
		struct sta_info         *psta;
		struct mlme_ext_priv    *pmlmeext = &(padapter->mlmeextpriv);
		struct mlme_ext_info    *pmlmeinfo = &(pmlmeext->mlmext_info);
		struct pwrctrl_priv *pwrpriv = adapter_to_pwrctl(padapter);
		struct security_priv *psecpriv = &padapter->securitypriv;

		psta = rtw_get_stainfo(&padapter->stapriv,
		get_my_bssid(&pmlmeinfo->network));

		if (psta) {
			if (pwrpriv->wowlan_fw_iv > psta->dot11txpn.val) {
				if (psecpriv->dot11PrivacyAlgrthm != _NO_PRIVACY_)
					psta->dot11txpn.val = pwrpriv->wowlan_fw_iv + 2;
			} else {
				DBG_871X("%s(): FW IV is smaller than driver\n", __func__);
				psta->dot11txpn.val += 2;
			}
			DBG_871X("%s: dot11txpn: 0x%016llx\n", __func__, psta->dot11txpn.val);
		}
}
#endif /* CONFIG_WOWLAN */

#ifdef CONFIG_PNO_SUPPORT
#define	CSCAN_TLV_TYPE_SSID_IE	'S'
#define CIPHER_IE "key_mgmt ="
#define CIPHER_NONE "NONE"
#define CIPHER_WPA_PSK "WPA-PSK"
#define CIPHER_WPA_EAP "WPA-EAP IEEE8021X"

#endif /* CONFIG_PNO_SUPPORT */
=======
>>>>>>> upstream/android-13
