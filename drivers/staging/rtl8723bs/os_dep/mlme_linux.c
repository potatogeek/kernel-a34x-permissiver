// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
<<<<<<< HEAD


#define _MLME_OSDEP_C_

=======
>>>>>>> upstream/android-13
#include <drv_types.h>
#include <rtw_debug.h>

static void _dynamic_check_timer_handler(struct timer_list *t)
{
	struct adapter *adapter =
		from_timer(adapter, t, mlmepriv.dynamic_chk_timer);

	rtw_dynamic_check_timer_handler(adapter);

	_set_timer(&adapter->mlmepriv.dynamic_chk_timer, 2000);
}

static void _rtw_set_scan_deny_timer_hdl(struct timer_list *t)
{
	struct adapter *adapter =
		from_timer(adapter, t, mlmepriv.set_scan_deny_timer);

<<<<<<< HEAD
	rtw_set_scan_deny_timer_hdl(adapter);
=======
	rtw_clear_scan_deny(adapter);
>>>>>>> upstream/android-13
}

void rtw_init_mlme_timer(struct adapter *padapter)
{
	struct	mlme_priv *pmlmepriv = &padapter->mlmepriv;

	timer_setup(&pmlmepriv->assoc_timer, _rtw_join_timeout_handler, 0);
	timer_setup(&pmlmepriv->scan_to_timer, rtw_scan_timeout_handler, 0);
	timer_setup(&pmlmepriv->dynamic_chk_timer,
		    _dynamic_check_timer_handler, 0);
	timer_setup(&pmlmepriv->set_scan_deny_timer,
		    _rtw_set_scan_deny_timer_hdl, 0);
}

void rtw_os_indicate_connect(struct adapter *adapter)
{
	struct mlme_priv *pmlmepriv = &(adapter->mlmepriv);

	if ((check_fwstate(pmlmepriv, WIFI_ADHOC_MASTER_STATE) == true) ||
<<<<<<< HEAD
		(check_fwstate(pmlmepriv, WIFI_ADHOC_STATE) == true))
	{
		rtw_cfg80211_ibss_indicate_connect(adapter);
	}
	else
		rtw_cfg80211_indicate_connect(adapter);

	rtw_indicate_wx_assoc_event(adapter);
=======
		(check_fwstate(pmlmepriv, WIFI_ADHOC_STATE) == true)) {
		rtw_cfg80211_ibss_indicate_connect(adapter);
	} else {
		rtw_cfg80211_indicate_connect(adapter);
	}

>>>>>>> upstream/android-13
	netif_carrier_on(adapter->pnetdev);

	if (adapter->pid[2] != 0)
		rtw_signal_process(adapter->pid[2], SIGALRM);
}

void rtw_os_indicate_scan_done(struct adapter *padapter, bool aborted)
{
	rtw_cfg80211_indicate_scan_done(padapter, aborted);
<<<<<<< HEAD
	indicate_wx_scan_complete_event(padapter);
}

static RT_PMKID_LIST   backupPMKIDList[ NUM_PMKID_CACHE ];
=======
}

static struct rt_pmkid_list   backupPMKIDList[NUM_PMKID_CACHE];
>>>>>>> upstream/android-13
void rtw_reset_securitypriv(struct adapter *adapter)
{
	u8 backupPMKIDIndex = 0;
	u8 backupTKIPCountermeasure = 0x00;
	u32 backupTKIPcountermeasure_time = 0;
	/*  add for CONFIG_IEEE80211W, none 11w also can use */
	struct mlme_ext_priv *pmlmeext = &adapter->mlmeextpriv;

	spin_lock_bh(&adapter->security_key_mutex);

<<<<<<< HEAD
	if (adapter->securitypriv.dot11AuthAlgrthm == dot11AuthAlgrthm_8021X)/* 802.1x */
	{
=======
	if (adapter->securitypriv.dot11AuthAlgrthm == dot11AuthAlgrthm_8021X) {
		/* 802.1x */
>>>>>>> upstream/android-13
		/*  Added by Albert 2009/02/18 */
		/*  We have to backup the PMK information for WiFi PMK Caching test item. */
		/*  */
		/*  Backup the btkip_countermeasure information. */
		/*  When the countermeasure is trigger, the driver have to disconnect with AP for 60 seconds. */

<<<<<<< HEAD
		memcpy(&backupPMKIDList[ 0 ], &adapter->securitypriv.PMKIDList[ 0 ], sizeof(RT_PMKID_LIST) * NUM_PMKID_CACHE);
=======
		memcpy(&backupPMKIDList[0], &adapter->securitypriv.PMKIDList[0], sizeof(struct rt_pmkid_list) * NUM_PMKID_CACHE);
>>>>>>> upstream/android-13
		backupPMKIDIndex = adapter->securitypriv.PMKIDIndex;
		backupTKIPCountermeasure = adapter->securitypriv.btkip_countermeasure;
		backupTKIPcountermeasure_time = adapter->securitypriv.btkip_countermeasure_time;

		/* reset RX BIP packet number */
		pmlmeext->mgnt_80211w_IPN_rx = 0;

		memset((unsigned char *)&adapter->securitypriv, 0, sizeof(struct security_priv));

		/*  Added by Albert 2009/02/18 */
		/*  Restore the PMK information to securitypriv structure for the following connection. */
<<<<<<< HEAD
		memcpy(&adapter->securitypriv.PMKIDList[ 0 ], &backupPMKIDList[ 0 ], sizeof(RT_PMKID_LIST) * NUM_PMKID_CACHE);
=======
		memcpy(&adapter->securitypriv.PMKIDList[0], &backupPMKIDList[0], sizeof(struct rt_pmkid_list) * NUM_PMKID_CACHE);
>>>>>>> upstream/android-13
		adapter->securitypriv.PMKIDIndex = backupPMKIDIndex;
		adapter->securitypriv.btkip_countermeasure = backupTKIPCountermeasure;
		adapter->securitypriv.btkip_countermeasure_time = backupTKIPcountermeasure_time;

		adapter->securitypriv.ndisauthtype = Ndis802_11AuthModeOpen;
		adapter->securitypriv.ndisencryptstatus = Ndis802_11WEPDisabled;

<<<<<<< HEAD
	}
	else /* reset values in securitypriv */
	{
=======
	} else {
		/* reset values in securitypriv */
>>>>>>> upstream/android-13
		/* if (adapter->mlmepriv.fw_state & WIFI_STATION_STATE) */
		/*  */
		struct security_priv *psec_priv = &adapter->securitypriv;

		psec_priv->dot11AuthAlgrthm = dot11AuthAlgrthm_Open;  /* open system */
		psec_priv->dot11PrivacyAlgrthm = _NO_PRIVACY_;
		psec_priv->dot11PrivacyKeyIndex = 0;

		psec_priv->dot118021XGrpPrivacy = _NO_PRIVACY_;
		psec_priv->dot118021XGrpKeyid = 1;

		psec_priv->ndisauthtype = Ndis802_11AuthModeOpen;
		psec_priv->ndisencryptstatus = Ndis802_11WEPDisabled;
		/*  */
	}
	/*  add for CONFIG_IEEE80211W, none 11w also can use */
	spin_unlock_bh(&adapter->security_key_mutex);
}

void rtw_os_indicate_disconnect(struct adapter *adapter)
{
<<<<<<< HEAD
	/* RT_PMKID_LIST   backupPMKIDList[ NUM_PMKID_CACHE ]; */
=======
	/* struct rt_pmkid_list   backupPMKIDList[ NUM_PMKID_CACHE ]; */
>>>>>>> upstream/android-13

	netif_carrier_off(adapter->pnetdev); /*  Do it first for tx broadcast pkt after disconnection issue! */

	rtw_cfg80211_indicate_disconnect(adapter);

<<<<<<< HEAD
	rtw_indicate_wx_disassoc_event(adapter);

=======
>>>>>>> upstream/android-13
	/* modify for CONFIG_IEEE80211W, none 11w also can use the same command */
	rtw_reset_securitypriv_cmd(adapter);
}

void rtw_report_sec_ie(struct adapter *adapter, u8 authmode, u8 *sec_ie)
{
	uint	len;
	u8 *buff, *p, i;
	union iwreq_data wrqu;

<<<<<<< HEAD
	RT_TRACE(_module_mlme_osdep_c_, _drv_info_, ("+rtw_report_sec_ie, authmode =%d\n", authmode));

	buff = NULL;
	if (authmode == _WPA_IE_ID_)
	{
		RT_TRACE(_module_mlme_osdep_c_, _drv_info_, ("rtw_report_sec_ie, authmode =%d\n", authmode));

		buff = rtw_zmalloc(IW_CUSTOM_MAX);
		if (NULL == buff) {
			DBG_871X(FUNC_ADPT_FMT ": alloc memory FAIL!!\n",
				FUNC_ADPT_ARG(adapter));
			return;
		}
		p = buff;

		p += sprintf(p, "ASSOCINFO(ReqIEs =");
=======
	buff = NULL;
	if (authmode == WLAN_EID_VENDOR_SPECIFIC) {
		buff = rtw_zmalloc(IW_CUSTOM_MAX);
		if (!buff)
			return;

		p = buff;

		p += scnprintf(p, IW_CUSTOM_MAX - (p - buff), "ASSOCINFO(ReqIEs =");
>>>>>>> upstream/android-13

		len = sec_ie[1] + 2;
		len = (len < IW_CUSTOM_MAX) ? len : IW_CUSTOM_MAX;

<<<<<<< HEAD
		for (i = 0; i < len; i++) {
			p += sprintf(p, "%02x", sec_ie[i]);
		}

		p += sprintf(p, ")");
=======
		for (i = 0; i < len; i++)
			p += scnprintf(p, IW_CUSTOM_MAX - (p - buff), "%02x", sec_ie[i]);

		p += scnprintf(p, IW_CUSTOM_MAX - (p - buff), ")");
>>>>>>> upstream/android-13

		memset(&wrqu, 0, sizeof(wrqu));

		wrqu.data.length = p - buff;

		wrqu.data.length = (wrqu.data.length < IW_CUSTOM_MAX) ? wrqu.data.length : IW_CUSTOM_MAX;

		kfree(buff);
	}
}

void init_addba_retry_timer(struct adapter *padapter, struct sta_info *psta)
{
	timer_setup(&psta->addba_retry_timer, addba_timer_hdl, 0);
}

void init_mlme_ext_timer(struct adapter *padapter)
{
	struct	mlme_ext_priv *pmlmeext = &padapter->mlmeextpriv;

	timer_setup(&pmlmeext->survey_timer, survey_timer_hdl, 0);
	timer_setup(&pmlmeext->link_timer, link_timer_hdl, 0);
	timer_setup(&pmlmeext->sa_query_timer, sa_query_timer_hdl, 0);
}
