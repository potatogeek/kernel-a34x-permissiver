<<<<<<< HEAD
/*
 * Copyright (c) 2012-2012 Quantenna Communications, Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
=======
// SPDX-License-Identifier: GPL-2.0+
/* Copyright (c) 2015-2016 Quantenna Communications. All rights reserved. */
>>>>>>> upstream/android-13

#include <linux/kernel.h>
#include <linux/etherdevice.h>
#include <linux/vmalloc.h>
#include <linux/ieee80211.h>
#include <net/cfg80211.h>
#include <net/netlink.h>

#include "cfg80211.h"
#include "commands.h"
#include "core.h"
#include "util.h"
#include "bus.h"

/* Supported rates to be advertised to the cfg80211 */
static struct ieee80211_rate qtnf_rates_2g[] = {
	{.bitrate = 10, .hw_value = 2, },
	{.bitrate = 20, .hw_value = 4, },
	{.bitrate = 55, .hw_value = 11, },
	{.bitrate = 110, .hw_value = 22, },
	{.bitrate = 60, .hw_value = 12, },
	{.bitrate = 90, .hw_value = 18, },
	{.bitrate = 120, .hw_value = 24, },
	{.bitrate = 180, .hw_value = 36, },
	{.bitrate = 240, .hw_value = 48, },
	{.bitrate = 360, .hw_value = 72, },
	{.bitrate = 480, .hw_value = 96, },
	{.bitrate = 540, .hw_value = 108, },
};

/* Supported rates to be advertised to the cfg80211 */
static struct ieee80211_rate qtnf_rates_5g[] = {
	{.bitrate = 60, .hw_value = 12, },
	{.bitrate = 90, .hw_value = 18, },
	{.bitrate = 120, .hw_value = 24, },
	{.bitrate = 180, .hw_value = 36, },
	{.bitrate = 240, .hw_value = 48, },
	{.bitrate = 360, .hw_value = 72, },
	{.bitrate = 480, .hw_value = 96, },
	{.bitrate = 540, .hw_value = 108, },
};

/* Supported crypto cipher suits to be advertised to cfg80211 */
static const u32 qtnf_cipher_suites[] = {
	WLAN_CIPHER_SUITE_TKIP,
	WLAN_CIPHER_SUITE_CCMP,
	WLAN_CIPHER_SUITE_AES_CMAC,
};

/* Supported mgmt frame types to be advertised to cfg80211 */
static const struct ieee80211_txrx_stypes
qtnf_mgmt_stypes[NUM_NL80211_IFTYPES] = {
	[NL80211_IFTYPE_STATION] = {
<<<<<<< HEAD
		.tx = BIT(IEEE80211_STYPE_ACTION >> 4),
		.rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
		      BIT(IEEE80211_STYPE_PROBE_REQ >> 4),
	},
	[NL80211_IFTYPE_AP] = {
		.tx = BIT(IEEE80211_STYPE_ACTION >> 4),
=======
		.tx = BIT(IEEE80211_STYPE_ACTION >> 4) |
		      BIT(IEEE80211_STYPE_AUTH >> 4),
		.rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
		      BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
		      BIT(IEEE80211_STYPE_AUTH >> 4),
	},
	[NL80211_IFTYPE_AP] = {
		.tx = BIT(IEEE80211_STYPE_ACTION >> 4) |
		      BIT(IEEE80211_STYPE_AUTH >> 4),
>>>>>>> upstream/android-13
		.rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
		      BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
		      BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |
		      BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
		      BIT(IEEE80211_STYPE_AUTH >> 4),
	},
};

static int
qtnf_validate_iface_combinations(struct wiphy *wiphy,
				 struct qtnf_vif *change_vif,
				 enum nl80211_iftype new_type)
{
	struct qtnf_wmac *mac;
	struct qtnf_vif *vif;
	int i;
	int ret = 0;
	struct iface_combination_params params = {
		.num_different_channels = 1,
	};

	mac = wiphy_priv(wiphy);
	if (!mac)
		return -EFAULT;

	for (i = 0; i < QTNF_MAX_INTF; i++) {
		vif = &mac->iflist[i];
		if (vif->wdev.iftype != NL80211_IFTYPE_UNSPECIFIED)
			params.iftype_num[vif->wdev.iftype]++;
	}

	if (change_vif) {
		params.iftype_num[new_type]++;
		params.iftype_num[change_vif->wdev.iftype]--;
	} else {
		params.iftype_num[new_type]++;
	}

	ret = cfg80211_check_combinations(wiphy, &params);

<<<<<<< HEAD
=======
	if (ret)
		return ret;

	/* Check repeater interface combination: primary VIF should be STA only.
	 * STA (primary) + AP (secondary) is OK.
	 * AP (primary) + STA (secondary) is not supported.
	 */
	vif = qtnf_mac_get_base_vif(mac);
	if (vif && vif->wdev.iftype == NL80211_IFTYPE_AP &&
	    vif != change_vif && new_type == NL80211_IFTYPE_STATION) {
		ret = -EINVAL;
		pr_err("MAC%u invalid combination: AP as primary repeater interface is not supported\n",
		       mac->macid);
	}

>>>>>>> upstream/android-13
	return ret;
}

static int
qtnf_change_virtual_intf(struct wiphy *wiphy,
			 struct net_device *dev,
			 enum nl80211_iftype type,
			 struct vif_params *params)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
<<<<<<< HEAD
	u8 *mac_addr;
=======
	u8 *mac_addr = NULL;
	int use4addr = 0;
>>>>>>> upstream/android-13
	int ret;

	ret = qtnf_validate_iface_combinations(wiphy, vif, type);
	if (ret) {
		pr_err("VIF%u.%u combination check: failed to set type %d\n",
		       vif->mac->macid, vif->vifid, type);
		return ret;
	}

<<<<<<< HEAD
	if (params)
		mac_addr = params->macaddr;
	else
		mac_addr = NULL;

	qtnf_scan_done(vif->mac, true);

	ret = qtnf_cmd_send_change_intf_type(vif, type, mac_addr);
	if (ret) {
		pr_err("VIF%u.%u: failed to change VIF type: %d\n",
		       vif->mac->macid, vif->vifid, ret);
=======
	if (params) {
		mac_addr = params->macaddr;
		use4addr = params->use_4addr;
	}

	qtnf_scan_done(vif->mac, true);

	ret = qtnf_cmd_send_change_intf_type(vif, type, use4addr, mac_addr);
	if (ret) {
		pr_err("VIF%u.%u: failed to change type to %d\n",
		       vif->mac->macid, vif->vifid, type);
>>>>>>> upstream/android-13
		return ret;
	}

	vif->wdev.iftype = type;
	return 0;
}

int qtnf_del_virtual_intf(struct wiphy *wiphy, struct wireless_dev *wdev)
{
	struct net_device *netdev =  wdev->netdev;
	struct qtnf_vif *vif;
<<<<<<< HEAD
=======
	struct sk_buff *skb;
>>>>>>> upstream/android-13

	if (WARN_ON(!netdev))
		return -EFAULT;

	vif = qtnf_netdev_get_priv(wdev->netdev);

	qtnf_scan_done(vif->mac, true);

	/* Stop data */
	netif_tx_stop_all_queues(netdev);
	if (netif_carrier_ok(netdev))
		netif_carrier_off(netdev);

<<<<<<< HEAD
	if (netdev->reg_state == NETREG_REGISTERED)
		unregister_netdevice(netdev);
=======
	while ((skb = skb_dequeue(&vif->high_pri_tx_queue)))
		dev_kfree_skb_any(skb);

	cancel_work_sync(&vif->high_pri_tx_work);

	if (netdev->reg_state == NETREG_REGISTERED)
		cfg80211_unregister_netdevice(netdev);
>>>>>>> upstream/android-13

	if (qtnf_cmd_send_del_intf(vif))
		pr_err("VIF%u.%u: failed to delete VIF\n", vif->mac->macid,
		       vif->vifid);

	vif->netdev->ieee80211_ptr = NULL;
	vif->netdev = NULL;
	vif->wdev.iftype = NL80211_IFTYPE_UNSPECIFIED;

	return 0;
}

static struct wireless_dev *qtnf_add_virtual_intf(struct wiphy *wiphy,
						  const char *name,
						  unsigned char name_assign_t,
						  enum nl80211_iftype type,
						  struct vif_params *params)
{
	struct qtnf_wmac *mac;
	struct qtnf_vif *vif;
	u8 *mac_addr = NULL;
<<<<<<< HEAD
=======
	int use4addr = 0;
>>>>>>> upstream/android-13
	int ret;

	mac = wiphy_priv(wiphy);

	if (!mac)
		return ERR_PTR(-EFAULT);

	ret = qtnf_validate_iface_combinations(wiphy, NULL, type);
	if (ret) {
		pr_err("MAC%u invalid combination: failed to add type %d\n",
		       mac->macid, type);
		return ERR_PTR(ret);
	}

	switch (type) {
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_AP:
		vif = qtnf_mac_get_free_vif(mac);
		if (!vif) {
			pr_err("MAC%u: no free VIF available\n", mac->macid);
			return ERR_PTR(-EFAULT);
		}

		eth_zero_addr(vif->mac_addr);
		eth_zero_addr(vif->bssid);
		vif->bss_priority = QTNF_DEF_BSS_PRIORITY;
<<<<<<< HEAD
		vif->sta_state = QTNF_STA_DISCONNECTED;
=======
>>>>>>> upstream/android-13
		memset(&vif->wdev, 0, sizeof(vif->wdev));
		vif->wdev.wiphy = wiphy;
		vif->wdev.iftype = type;
		break;
	default:
		pr_err("MAC%u: unsupported IF type %d\n", mac->macid, type);
		return ERR_PTR(-ENOTSUPP);
	}

<<<<<<< HEAD
	if (params)
		mac_addr = params->macaddr;

	if (qtnf_cmd_send_add_intf(vif, type, mac_addr)) {
		pr_err("VIF%u.%u: failed to add VIF\n", mac->macid, vif->vifid);
=======
	if (params) {
		mac_addr = params->macaddr;
		use4addr = params->use_4addr;
	}

	ret = qtnf_cmd_send_add_intf(vif, type, use4addr, mac_addr);
	if (ret) {
		pr_err("VIF%u.%u: failed to add VIF %pM\n",
		       mac->macid, vif->vifid, mac_addr);
>>>>>>> upstream/android-13
		goto err_cmd;
	}

	if (!is_valid_ether_addr(vif->mac_addr)) {
		pr_err("VIF%u.%u: FW reported bad MAC: %pM\n",
		       mac->macid, vif->vifid, vif->mac_addr);
<<<<<<< HEAD
		goto err_mac;
	}

	if (qtnf_core_net_attach(mac, vif, name, name_assign_t)) {
		pr_err("VIF%u.%u: failed to attach netdev\n", mac->macid,
		       vif->vifid);
		goto err_net;
=======
		ret = -EINVAL;
		goto error_del_vif;
	}

	ret = qtnf_core_net_attach(mac, vif, name, name_assign_t);
	if (ret) {
		pr_err("VIF%u.%u: failed to attach netdev\n", mac->macid,
		       vif->vifid);
		goto error_del_vif;
	}

	if (qtnf_hwcap_is_set(&mac->bus->hw_info, QLINK_HW_CAPAB_HW_BRIDGE)) {
		ret = qtnf_cmd_netdev_changeupper(vif, vif->netdev->ifindex);
		if (ret) {
			cfg80211_unregister_netdevice(vif->netdev);
			vif->netdev = NULL;
			goto error_del_vif;
		}
>>>>>>> upstream/android-13
	}

	vif->wdev.netdev = vif->netdev;
	return &vif->wdev;

<<<<<<< HEAD
err_net:
	vif->netdev = NULL;
err_mac:
=======
error_del_vif:
>>>>>>> upstream/android-13
	qtnf_cmd_send_del_intf(vif);
err_cmd:
	vif->wdev.iftype = NL80211_IFTYPE_UNSPECIFIED;

<<<<<<< HEAD
	return ERR_PTR(-EFAULT);
=======
	return ERR_PTR(ret);
>>>>>>> upstream/android-13
}

static int qtnf_mgmt_set_appie(struct qtnf_vif *vif,
			       const struct cfg80211_beacon_data *info)
{
	int ret = 0;

	if (!info->beacon_ies || !info->beacon_ies_len) {
		ret = qtnf_cmd_send_mgmt_set_appie(vif, QLINK_IE_SET_BEACON_IES,
						   NULL, 0);
	} else {
		ret = qtnf_cmd_send_mgmt_set_appie(vif, QLINK_IE_SET_BEACON_IES,
						   info->beacon_ies,
						   info->beacon_ies_len);
	}

	if (ret)
		goto out;

	if (!info->proberesp_ies || !info->proberesp_ies_len) {
		ret = qtnf_cmd_send_mgmt_set_appie(vif,
						   QLINK_IE_SET_PROBE_RESP_IES,
						   NULL, 0);
	} else {
		ret = qtnf_cmd_send_mgmt_set_appie(vif,
						   QLINK_IE_SET_PROBE_RESP_IES,
						   info->proberesp_ies,
						   info->proberesp_ies_len);
	}

	if (ret)
		goto out;

	if (!info->assocresp_ies || !info->assocresp_ies_len) {
		ret = qtnf_cmd_send_mgmt_set_appie(vif,
						   QLINK_IE_SET_ASSOC_RESP,
						   NULL, 0);
	} else {
		ret = qtnf_cmd_send_mgmt_set_appie(vif,
						   QLINK_IE_SET_ASSOC_RESP,
						   info->assocresp_ies,
						   info->assocresp_ies_len);
	}

out:
	return ret;
}

static int qtnf_change_beacon(struct wiphy *wiphy, struct net_device *dev,
			      struct cfg80211_beacon_data *info)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);

	return qtnf_mgmt_set_appie(vif, info);
}

static int qtnf_start_ap(struct wiphy *wiphy, struct net_device *dev,
			 struct cfg80211_ap_settings *settings)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	ret = qtnf_cmd_send_start_ap(vif, settings);
	if (ret)
		pr_err("VIF%u.%u: failed to start AP\n", vif->mac->macid,
		       vif->vifid);

	return ret;
}

<<<<<<< HEAD
static int qtnf_stop_ap(struct wiphy *wiphy, struct net_device *dev)
=======
static int qtnf_stop_ap(struct wiphy *wiphy, struct net_device *dev,
			unsigned int link_id)
>>>>>>> upstream/android-13
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	qtnf_scan_done(vif->mac, true);

	ret = qtnf_cmd_send_stop_ap(vif);
<<<<<<< HEAD
	if (ret) {
		pr_err("VIF%u.%u: failed to stop AP operation in FW\n",
		       vif->mac->macid, vif->vifid);

		netif_carrier_off(vif->netdev);
	}
=======
	if (ret)
		pr_err("VIF%u.%u: failed to stop AP operation in FW\n",
		       vif->mac->macid, vif->vifid);

	netif_carrier_off(vif->netdev);
>>>>>>> upstream/android-13

	return ret;
}

static int qtnf_set_wiphy_params(struct wiphy *wiphy, u32 changed)
{
	struct qtnf_wmac *mac = wiphy_priv(wiphy);
	struct qtnf_vif *vif;
	int ret;

	vif = qtnf_mac_get_base_vif(mac);
	if (!vif) {
		pr_err("MAC%u: primary VIF is not configured\n", mac->macid);
		return -EFAULT;
	}

<<<<<<< HEAD
	if (changed & (WIPHY_PARAM_RETRY_LONG | WIPHY_PARAM_RETRY_SHORT)) {
		pr_err("MAC%u: can't modify retry params\n", mac->macid);
		return -EOPNOTSUPP;
	}

=======
>>>>>>> upstream/android-13
	ret = qtnf_cmd_send_update_phy_params(mac, changed);
	if (ret)
		pr_err("MAC%u: failed to update PHY params\n", mac->macid);

	return ret;
}

static void
<<<<<<< HEAD
qtnf_mgmt_frame_register(struct wiphy *wiphy, struct wireless_dev *wdev,
			 u16 frame_type, bool reg)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(wdev->netdev);
	u16 mgmt_type;
	u16 new_mask;
	u16 qlink_frame_type = 0;

	mgmt_type = (frame_type & IEEE80211_FCTL_STYPE) >> 4;

	if (reg)
		new_mask = vif->mgmt_frames_bitmask | BIT(mgmt_type);
	else
		new_mask = vif->mgmt_frames_bitmask & ~BIT(mgmt_type);

	if (new_mask == vif->mgmt_frames_bitmask)
		return;

	switch (frame_type & IEEE80211_FCTL_STYPE) {
	case IEEE80211_STYPE_REASSOC_REQ:
	case IEEE80211_STYPE_ASSOC_REQ:
		qlink_frame_type = QLINK_MGMT_FRAME_ASSOC_REQ;
		break;
	case IEEE80211_STYPE_AUTH:
		qlink_frame_type = QLINK_MGMT_FRAME_AUTH;
		break;
	case IEEE80211_STYPE_PROBE_REQ:
		qlink_frame_type = QLINK_MGMT_FRAME_PROBE_REQ;
		break;
	case IEEE80211_STYPE_ACTION:
		qlink_frame_type = QLINK_MGMT_FRAME_ACTION;
		break;
	default:
		pr_warn("VIF%u.%u: unsupported frame type: %X\n",
			vif->mac->macid, vif->vifid,
			(frame_type & IEEE80211_FCTL_STYPE) >> 4);
		return;
	}

	if (qtnf_cmd_send_register_mgmt(vif, qlink_frame_type, reg)) {
		pr_warn("VIF%u.%u: failed to %sregister mgmt frame type 0x%x\n",
			vif->mac->macid, vif->vifid, reg ? "" : "un",
			frame_type);
		return;
	}

	vif->mgmt_frames_bitmask = new_mask;
	pr_debug("VIF%u.%u: %sregistered mgmt frame type 0x%x\n",
		 vif->mac->macid, vif->vifid, reg ? "" : "un", frame_type);
=======
qtnf_update_mgmt_frame_registrations(struct wiphy *wiphy,
				     struct wireless_dev *wdev,
				     struct mgmt_frame_regs *upd)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(wdev->netdev);
	u16 new_mask = upd->interface_stypes;
	u16 old_mask = vif->mgmt_frames_bitmask;
	static const struct {
		u16 mask, qlink_type;
	} updates[] = {
		{
			.mask = BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
				BIT(IEEE80211_STYPE_ASSOC_REQ >> 4),
			.qlink_type = QLINK_MGMT_FRAME_ASSOC_REQ,
		},
		{
			.mask = BIT(IEEE80211_STYPE_AUTH >> 4),
			.qlink_type = QLINK_MGMT_FRAME_AUTH,
		},
		{
			.mask = BIT(IEEE80211_STYPE_PROBE_REQ >> 4),
			.qlink_type = QLINK_MGMT_FRAME_PROBE_REQ,
		},
		{
			.mask = BIT(IEEE80211_STYPE_ACTION >> 4),
			.qlink_type = QLINK_MGMT_FRAME_ACTION,
		},
	};
	unsigned int i;

	if (new_mask == old_mask)
		return;

	for (i = 0; i < ARRAY_SIZE(updates); i++) {
		u16 mask = updates[i].mask;
		u16 qlink_frame_type = updates[i].qlink_type;
		bool reg;

		/* the ! are here due to the assoc/reassoc merge */
		if (!(new_mask & mask) == !(old_mask & mask))
			continue;

		reg = new_mask & mask;

		if (qtnf_cmd_send_register_mgmt(vif, qlink_frame_type, reg))
			pr_warn("VIF%u.%u: failed to %sregister qlink frame type 0x%x\n",
				vif->mac->macid, vif->vifid, reg ? "" : "un",
				qlink_frame_type);
	}

	vif->mgmt_frames_bitmask = new_mask;
>>>>>>> upstream/android-13
}

static int
qtnf_mgmt_tx(struct wiphy *wiphy, struct wireless_dev *wdev,
	     struct cfg80211_mgmt_tx_params *params, u64 *cookie)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(wdev->netdev);
	const struct ieee80211_mgmt *mgmt_frame = (void *)params->buf;
	u32 short_cookie = prandom_u32();
	u16 flags = 0;
	u16 freq;

	*cookie = short_cookie;

	if (params->offchan)
<<<<<<< HEAD
		flags |= QLINK_MGMT_FRAME_TX_FLAG_OFFCHAN;

	if (params->no_cck)
		flags |= QLINK_MGMT_FRAME_TX_FLAG_NO_CCK;

	if (params->dont_wait_for_ack)
		flags |= QLINK_MGMT_FRAME_TX_FLAG_ACK_NOWAIT;
=======
		flags |= QLINK_FRAME_TX_FLAG_OFFCHAN;

	if (params->no_cck)
		flags |= QLINK_FRAME_TX_FLAG_NO_CCK;

	if (params->dont_wait_for_ack)
		flags |= QLINK_FRAME_TX_FLAG_ACK_NOWAIT;
>>>>>>> upstream/android-13

	/* If channel is not specified, pass "freq = 0" to tell device
	 * firmware to use current channel.
	 */
	if (params->chan)
		freq = params->chan->center_freq;
	else
		freq = 0;

	pr_debug("%s freq:%u; FC:%.4X; DA:%pM; len:%zu; C:%.8X; FL:%.4X\n",
		 wdev->netdev->name, freq,
		 le16_to_cpu(mgmt_frame->frame_control), mgmt_frame->da,
		 params->len, short_cookie, flags);

<<<<<<< HEAD
	return qtnf_cmd_send_mgmt_frame(vif, short_cookie, flags,
					freq,
					params->buf, params->len);
=======
	return qtnf_cmd_send_frame(vif, short_cookie, flags,
				   freq, params->buf, params->len);
>>>>>>> upstream/android-13
}

static int
qtnf_get_station(struct wiphy *wiphy, struct net_device *dev,
		 const u8 *mac, struct station_info *sinfo)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);

	sinfo->generation = vif->generation;
	return qtnf_cmd_get_sta_info(vif, mac, sinfo);
}

static int
qtnf_dump_station(struct wiphy *wiphy, struct net_device *dev,
		  int idx, u8 *mac, struct station_info *sinfo)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	const struct qtnf_sta_node *sta_node;
	int ret;

<<<<<<< HEAD
	sta_node = qtnf_sta_list_lookup_index(&vif->sta_list, idx);

	if (unlikely(!sta_node))
		return -ENOENT;

	ether_addr_copy(mac, sta_node->mac_addr);

	ret = qtnf_cmd_get_sta_info(vif, sta_node->mac_addr, sinfo);

	if (unlikely(ret == -ENOENT)) {
		qtnf_sta_list_del(vif, mac);
		cfg80211_del_sta(vif->netdev, mac, GFP_KERNEL);
		sinfo->filled = 0;
=======
	switch (vif->wdev.iftype) {
	case NL80211_IFTYPE_STATION:
		if (idx != 0 || !vif->wdev.connected)
			return -ENOENT;

		ether_addr_copy(mac, vif->bssid);
		break;
	case NL80211_IFTYPE_AP:
		sta_node = qtnf_sta_list_lookup_index(&vif->sta_list, idx);
		if (unlikely(!sta_node))
			return -ENOENT;

		ether_addr_copy(mac, sta_node->mac_addr);
		break;
	default:
		return -ENOTSUPP;
	}

	ret = qtnf_cmd_get_sta_info(vif, mac, sinfo);

	if (vif->wdev.iftype == NL80211_IFTYPE_AP) {
		if (ret == -ENOENT) {
			cfg80211_del_sta(vif->netdev, mac, GFP_KERNEL);
			sinfo->filled = 0;
		}
>>>>>>> upstream/android-13
	}

	sinfo->generation = vif->generation;

	return ret;
}

static int qtnf_add_key(struct wiphy *wiphy, struct net_device *dev,
<<<<<<< HEAD
			u8 key_index, bool pairwise, const u8 *mac_addr,
			struct key_params *params)
=======
			int link_id, u8 key_index, bool pairwise,
			const u8 *mac_addr, struct key_params *params)
>>>>>>> upstream/android-13
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	ret = qtnf_cmd_send_add_key(vif, key_index, pairwise, mac_addr, params);
	if (ret)
		pr_err("VIF%u.%u: failed to add key: cipher=%x idx=%u pw=%u\n",
		       vif->mac->macid, vif->vifid, params->cipher, key_index,
		       pairwise);

	return ret;
}

static int qtnf_del_key(struct wiphy *wiphy, struct net_device *dev,
<<<<<<< HEAD
			u8 key_index, bool pairwise, const u8 *mac_addr)
=======
			int link_id, u8 key_index, bool pairwise,
			const u8 *mac_addr)
>>>>>>> upstream/android-13
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	ret = qtnf_cmd_send_del_key(vif, key_index, pairwise, mac_addr);
	if (ret) {
		if (ret == -ENOENT) {
			pr_debug("VIF%u.%u: key index %d out of bounds\n",
				 vif->mac->macid, vif->vifid, key_index);
		} else {
			pr_err("VIF%u.%u: failed to delete key: idx=%u pw=%u\n",
			       vif->mac->macid, vif->vifid,
			       key_index, pairwise);
		}
	}

	return ret;
}

static int qtnf_set_default_key(struct wiphy *wiphy, struct net_device *dev,
<<<<<<< HEAD
				u8 key_index, bool unicast, bool multicast)
=======
				int link_id, u8 key_index, bool unicast,
				bool multicast)
>>>>>>> upstream/android-13
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	ret = qtnf_cmd_send_set_default_key(vif, key_index, unicast, multicast);
	if (ret)
		pr_err("VIF%u.%u: failed to set dflt key: idx=%u uc=%u mc=%u\n",
		       vif->mac->macid, vif->vifid, key_index, unicast,
		       multicast);

	return ret;
}

static int
qtnf_set_default_mgmt_key(struct wiphy *wiphy, struct net_device *dev,
<<<<<<< HEAD
			  u8 key_index)
=======
			  int link_id, u8 key_index)
>>>>>>> upstream/android-13
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	ret = qtnf_cmd_send_set_default_mgmt_key(vif, key_index);
	if (ret)
		pr_err("VIF%u.%u: failed to set default MGMT key: idx=%u\n",
		       vif->mac->macid, vif->vifid, key_index);

	return ret;
}

static int
qtnf_change_station(struct wiphy *wiphy, struct net_device *dev,
		    const u8 *mac, struct station_parameters *params)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	ret = qtnf_cmd_send_change_sta(vif, mac, params);
	if (ret)
		pr_err("VIF%u.%u: failed to change STA %pM\n",
		       vif->mac->macid, vif->vifid, mac);

	return ret;
}

static int
qtnf_del_station(struct wiphy *wiphy, struct net_device *dev,
		 struct station_del_parameters *params)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	if (params->mac &&
	    (vif->wdev.iftype == NL80211_IFTYPE_AP) &&
	    !is_broadcast_ether_addr(params->mac) &&
	    !qtnf_sta_list_lookup(&vif->sta_list, params->mac))
		return 0;

	ret = qtnf_cmd_send_del_sta(vif, params);
	if (ret)
		pr_err("VIF%u.%u: failed to delete STA %pM\n",
		       vif->mac->macid, vif->vifid, params->mac);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	return ret;
}

static int
qtnf_scan(struct wiphy *wiphy, struct cfg80211_scan_request *request)
{
	struct qtnf_wmac *mac = wiphy_priv(wiphy);
<<<<<<< HEAD
=======
	int ret;
>>>>>>> upstream/android-13

	cancel_delayed_work_sync(&mac->scan_timeout);

	mac->scan_req = request;

<<<<<<< HEAD
	if (qtnf_cmd_send_scan(mac)) {
		pr_err("MAC%u: failed to start scan\n", mac->macid);
		mac->scan_req = NULL;
		return -EFAULT;
	}

	queue_delayed_work(mac->bus->workqueue, &mac->scan_timeout,
			   QTNF_SCAN_TIMEOUT_SEC * HZ);

	return 0;
=======
	ret = qtnf_cmd_send_scan(mac);
	if (ret) {
		pr_err("MAC%u: failed to start scan\n", mac->macid);
		mac->scan_req = NULL;
		goto out;
	}

	pr_debug("MAC%u: scan started\n", mac->macid);
	queue_delayed_work(mac->bus->workqueue, &mac->scan_timeout,
			   QTNF_SCAN_TIMEOUT_SEC * HZ);

out:
	return ret;
>>>>>>> upstream/android-13
}

static int
qtnf_connect(struct wiphy *wiphy, struct net_device *dev,
	     struct cfg80211_connect_params *sme)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	if (vif->wdev.iftype != NL80211_IFTYPE_STATION)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (vif->sta_state != QTNF_STA_DISCONNECTED)
		return -EBUSY;
=======
	if (sme->auth_type == NL80211_AUTHTYPE_SAE &&
	    !(sme->flags & CONNECT_REQ_EXTERNAL_AUTH_SUPPORT)) {
		pr_err("can not offload authentication to userspace\n");
		return -EOPNOTSUPP;
	}
>>>>>>> upstream/android-13

	if (sme->bssid)
		ether_addr_copy(vif->bssid, sme->bssid);
	else
		eth_zero_addr(vif->bssid);

	ret = qtnf_cmd_send_connect(vif, sme);
<<<<<<< HEAD
	if (ret) {
		pr_err("VIF%u.%u: failed to connect\n", vif->mac->macid,
		       vif->vifid);
		return ret;
	}

	vif->sta_state = QTNF_STA_CONNECTING;
	return 0;
=======
	if (ret)
		pr_err("VIF%u.%u: failed to connect\n",
		       vif->mac->macid, vif->vifid);

	return ret;
}

static int
qtnf_external_auth(struct wiphy *wiphy, struct net_device *dev,
		   struct cfg80211_external_auth_params *auth)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	if (vif->wdev.iftype == NL80211_IFTYPE_STATION &&
	    !ether_addr_equal(vif->bssid, auth->bssid))
		pr_warn("unexpected bssid: %pM", auth->bssid);

	ret = qtnf_cmd_send_external_auth(vif, auth);
	if (ret)
		pr_err("VIF%u.%u: failed to report external auth\n",
		       vif->mac->macid, vif->vifid);

	return ret;
>>>>>>> upstream/android-13
}

static int
qtnf_disconnect(struct wiphy *wiphy, struct net_device *dev,
		u16 reason_code)
{
	struct qtnf_wmac *mac = wiphy_priv(wiphy);
	struct qtnf_vif *vif;
	int ret = 0;

	vif = qtnf_mac_get_base_vif(mac);
	if (!vif) {
		pr_err("MAC%u: primary VIF is not configured\n", mac->macid);
		return -EFAULT;
	}

	if (vif->wdev.iftype != NL80211_IFTYPE_STATION) {
<<<<<<< HEAD
		ret = -EOPNOTSUPP;
		goto out;
	}

	qtnf_scan_done(mac, true);

	if (vif->sta_state == QTNF_STA_DISCONNECTED)
		goto out;

	ret = qtnf_cmd_send_disconnect(vif, reason_code);
	if (ret) {
		pr_err("VIF%u.%u: failed to disconnect\n", mac->macid,
		       vif->vifid);
		goto out;
	}

out:
	if (vif->sta_state == QTNF_STA_CONNECTING)
		vif->sta_state = QTNF_STA_DISCONNECTED;
=======
		return -EOPNOTSUPP;
	}

	ret = qtnf_cmd_send_disconnect(vif, reason_code);
	if (ret)
		pr_err("VIF%u.%u: failed to disconnect\n",
		       mac->macid, vif->vifid);

	if (vif->wdev.connected) {
		netif_carrier_off(vif->netdev);
		cfg80211_disconnected(vif->netdev, reason_code,
				      NULL, 0, true, GFP_KERNEL);
	}
>>>>>>> upstream/android-13

	return ret;
}

static int
qtnf_dump_survey(struct wiphy *wiphy, struct net_device *dev,
		 int idx, struct survey_info *survey)
{
	struct qtnf_wmac *mac = wiphy_priv(wiphy);
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct ieee80211_supported_band *sband;
<<<<<<< HEAD
	const struct cfg80211_chan_def *chandef = &wdev->chandef;
	struct ieee80211_channel *chan;
	struct qtnf_chan_stats stats;
	struct qtnf_vif *vif;
	int ret;

	vif = qtnf_netdev_get_priv(dev);
=======
	const struct cfg80211_chan_def *chandef = wdev_chandef(wdev, 0);
	struct ieee80211_channel *chan;
	int ret;

>>>>>>> upstream/android-13

	sband = wiphy->bands[NL80211_BAND_2GHZ];
	if (sband && idx >= sband->n_channels) {
		idx -= sband->n_channels;
		sband = NULL;
	}

	if (!sband)
		sband = wiphy->bands[NL80211_BAND_5GHZ];

	if (!sband || idx >= sband->n_channels)
		return -ENOENT;

	chan = &sband->channels[idx];
<<<<<<< HEAD
	memset(&stats, 0, sizeof(stats));

	survey->channel = chan;
	survey->filled = 0x0;

	if (chandef->chan) {
		if (chan->hw_value == chandef->chan->hw_value)
			survey->filled = SURVEY_INFO_IN_USE;
	}

	ret = qtnf_cmd_get_chan_stats(mac, chan->hw_value, &stats);
	switch (ret) {
	case 0:
		if (unlikely(stats.chan_num != chan->hw_value)) {
			pr_err("received stats for channel %d instead of %d\n",
			       stats.chan_num, chan->hw_value);
			ret = -EINVAL;
			break;
		}

		survey->filled |= SURVEY_INFO_TIME |
				 SURVEY_INFO_TIME_SCAN |
				 SURVEY_INFO_TIME_BUSY |
				 SURVEY_INFO_TIME_RX |
				 SURVEY_INFO_TIME_TX |
				 SURVEY_INFO_NOISE_DBM;

		survey->time_scan = stats.cca_try;
		survey->time = stats.cca_try;
		survey->time_tx = stats.cca_tx;
		survey->time_rx = stats.cca_rx;
		survey->time_busy = stats.cca_busy;
		survey->noise = stats.chan_noise;
		break;
	case -ENOENT:
		pr_debug("no stats for channel %u\n", chan->hw_value);
		ret = 0;
		break;
	default:
		pr_debug("failed to get chan(%d) stats from card\n",
			 chan->hw_value);
		ret = -EINVAL;
		break;
	}
=======
	survey->channel = chan;
	survey->filled = 0x0;

	if (chandef && chan == chandef->chan)
		survey->filled = SURVEY_INFO_IN_USE;

	ret = qtnf_cmd_get_chan_stats(mac, chan->center_freq, survey);
	if (ret)
		pr_debug("failed to get chan(%d) stats from card\n",
			 chan->hw_value);
>>>>>>> upstream/android-13

	return ret;
}

static int
qtnf_get_channel(struct wiphy *wiphy, struct wireless_dev *wdev,
<<<<<<< HEAD
		 struct cfg80211_chan_def *chandef)
=======
		 unsigned int link_id, struct cfg80211_chan_def *chandef)
>>>>>>> upstream/android-13
{
	struct net_device *ndev = wdev->netdev;
	struct qtnf_vif *vif;
	int ret;

	if (!ndev)
		return -ENODEV;

	vif = qtnf_netdev_get_priv(wdev->netdev);

	ret = qtnf_cmd_get_channel(vif, chandef);
	if (ret) {
		pr_err("%s: failed to get channel: %d\n", ndev->name, ret);
<<<<<<< HEAD
=======
		ret = -ENODATA;
>>>>>>> upstream/android-13
		goto out;
	}

	if (!cfg80211_chandef_valid(chandef)) {
		pr_err("%s: bad channel freq=%u cf1=%u cf2=%u bw=%u\n",
		       ndev->name, chandef->chan->center_freq,
		       chandef->center_freq1, chandef->center_freq2,
		       chandef->width);
		ret = -ENODATA;
<<<<<<< HEAD
=======
		goto out;
>>>>>>> upstream/android-13
	}

out:
	return ret;
}

static int qtnf_channel_switch(struct wiphy *wiphy, struct net_device *dev,
			       struct cfg80211_csa_settings *params)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	pr_debug("%s: chan(%u) count(%u) radar(%u) block_tx(%u)\n", dev->name,
		 params->chandef.chan->hw_value, params->count,
		 params->radar_required, params->block_tx);

	if (!cfg80211_chandef_valid(&params->chandef)) {
		pr_err("%s: invalid channel\n", dev->name);
		return -EINVAL;
	}

	ret = qtnf_cmd_send_chan_switch(vif, params);
	if (ret)
		pr_warn("%s: failed to switch to channel (%u)\n",
			dev->name, params->chandef.chan->hw_value);

	return ret;
}

static int qtnf_start_radar_detection(struct wiphy *wiphy,
				      struct net_device *ndev,
				      struct cfg80211_chan_def *chandef,
				      u32 cac_time_ms)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(ndev);
	int ret;

	if (wiphy_ext_feature_isset(wiphy, NL80211_EXT_FEATURE_DFS_OFFLOAD))
		return -ENOTSUPP;

	ret = qtnf_cmd_start_cac(vif, chandef, cac_time_ms);
	if (ret)
		pr_err("%s: failed to start CAC ret=%d\n", ndev->name, ret);

	return ret;
}

static int qtnf_set_mac_acl(struct wiphy *wiphy,
			    struct net_device *dev,
			    const struct cfg80211_acl_data *params)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	ret = qtnf_cmd_set_mac_acl(vif, params);
	if (ret)
		pr_err("%s: failed to set mac ACL ret=%d\n", dev->name, ret);

	return ret;
}

static int qtnf_set_power_mgmt(struct wiphy *wiphy, struct net_device *dev,
			       bool enabled, int timeout)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	ret = qtnf_cmd_send_pm_set(vif, enabled ? QLINK_PM_AUTO_STANDBY :
				   QLINK_PM_OFF, timeout);
<<<<<<< HEAD
	if (ret) {
		pr_err("%s: failed to set PM mode ret=%d\n", dev->name, ret);
		return ret;
	}

=======
	if (ret)
		pr_err("%s: failed to set PM mode ret=%d\n", dev->name, ret);

	return ret;
}

static int qtnf_get_tx_power(struct wiphy *wiphy, struct wireless_dev *wdev,
			     int *dbm)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(wdev->netdev);
	int ret;

	ret = qtnf_cmd_get_tx_power(vif, dbm);
	if (ret)
		pr_err("MAC%u: failed to get Tx power\n", vif->mac->macid);

	return ret;
}

static int qtnf_set_tx_power(struct wiphy *wiphy, struct wireless_dev *wdev,
			     enum nl80211_tx_power_setting type, int mbm)
{
	struct qtnf_vif *vif;
	int ret;

	if (wdev) {
		vif = qtnf_netdev_get_priv(wdev->netdev);
	} else {
		struct qtnf_wmac *mac = wiphy_priv(wiphy);

		vif = qtnf_mac_get_base_vif(mac);
		if (!vif) {
			pr_err("MAC%u: primary VIF is not configured\n",
			       mac->macid);
			return -EFAULT;
		}
	}

	ret = qtnf_cmd_set_tx_power(vif, type, mbm);
	if (ret)
		pr_err("MAC%u: failed to set Tx power\n", vif->mac->macid);

	return ret;
}

static int qtnf_update_owe_info(struct wiphy *wiphy, struct net_device *dev,
				struct cfg80211_update_owe_info *owe_info)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(dev);
	int ret;

	if (vif->wdev.iftype != NL80211_IFTYPE_AP)
		return -EOPNOTSUPP;

	ret = qtnf_cmd_send_update_owe(vif, owe_info);
	if (ret)
		pr_err("VIF%u.%u: failed to update owe info\n",
		       vif->mac->macid, vif->vifid);

>>>>>>> upstream/android-13
	return ret;
}

#ifdef CONFIG_PM
static int qtnf_suspend(struct wiphy *wiphy, struct cfg80211_wowlan *wowlan)
{
	struct qtnf_wmac *mac = wiphy_priv(wiphy);
	struct qtnf_vif *vif;
	int ret = 0;

	vif = qtnf_mac_get_base_vif(mac);
	if (!vif) {
		pr_err("MAC%u: primary VIF is not configured\n", mac->macid);
		ret = -EFAULT;
		goto exit;
	}

	if (!wowlan) {
		pr_debug("WoWLAN triggers are not enabled\n");
		qtnf_virtual_intf_cleanup(vif->netdev);
		goto exit;
	}

	qtnf_scan_done(vif->mac, true);

	ret = qtnf_cmd_send_wowlan_set(vif, wowlan);
	if (ret) {
		pr_err("MAC%u: failed to set WoWLAN triggers\n",
		       mac->macid);
		goto exit;
	}

exit:
	return ret;
}

static int qtnf_resume(struct wiphy *wiphy)
{
	struct qtnf_wmac *mac = wiphy_priv(wiphy);
	struct qtnf_vif *vif;
	int ret = 0;

	vif = qtnf_mac_get_base_vif(mac);
	if (!vif) {
		pr_err("MAC%u: primary VIF is not configured\n", mac->macid);
<<<<<<< HEAD
		ret = -EFAULT;
		goto exit;
	}

	ret = qtnf_cmd_send_wowlan_set(vif, NULL);
	if (ret) {
		pr_err("MAC%u: failed to reset WoWLAN triggers\n",
		       mac->macid);
		goto exit;
	}

exit:
=======
		return -EFAULT;
	}

	ret = qtnf_cmd_send_wowlan_set(vif, NULL);
	if (ret)
		pr_err("MAC%u: failed to reset WoWLAN triggers\n",
		       mac->macid);

>>>>>>> upstream/android-13
	return ret;
}

static void qtnf_set_wakeup(struct wiphy *wiphy, bool enabled)
{
	struct qtnf_wmac *mac = wiphy_priv(wiphy);
	struct qtnf_bus *bus = mac->bus;

	device_set_wakeup_enable(bus->dev, enabled);
}
#endif

static struct cfg80211_ops qtn_cfg80211_ops = {
	.add_virtual_intf	= qtnf_add_virtual_intf,
	.change_virtual_intf	= qtnf_change_virtual_intf,
	.del_virtual_intf	= qtnf_del_virtual_intf,
	.start_ap		= qtnf_start_ap,
	.change_beacon		= qtnf_change_beacon,
	.stop_ap		= qtnf_stop_ap,
	.set_wiphy_params	= qtnf_set_wiphy_params,
<<<<<<< HEAD
	.mgmt_frame_register	= qtnf_mgmt_frame_register,
=======
	.update_mgmt_frame_registrations =
		qtnf_update_mgmt_frame_registrations,
>>>>>>> upstream/android-13
	.mgmt_tx		= qtnf_mgmt_tx,
	.change_station		= qtnf_change_station,
	.del_station		= qtnf_del_station,
	.get_station		= qtnf_get_station,
	.dump_station		= qtnf_dump_station,
	.add_key		= qtnf_add_key,
	.del_key		= qtnf_del_key,
	.set_default_key	= qtnf_set_default_key,
	.set_default_mgmt_key	= qtnf_set_default_mgmt_key,
	.scan			= qtnf_scan,
	.connect		= qtnf_connect,
<<<<<<< HEAD
=======
	.external_auth		= qtnf_external_auth,
>>>>>>> upstream/android-13
	.disconnect		= qtnf_disconnect,
	.dump_survey		= qtnf_dump_survey,
	.get_channel		= qtnf_get_channel,
	.channel_switch		= qtnf_channel_switch,
	.start_radar_detection	= qtnf_start_radar_detection,
	.set_mac_acl		= qtnf_set_mac_acl,
	.set_power_mgmt		= qtnf_set_power_mgmt,
<<<<<<< HEAD
=======
	.get_tx_power		= qtnf_get_tx_power,
	.set_tx_power		= qtnf_set_tx_power,
	.update_owe_info	= qtnf_update_owe_info,
>>>>>>> upstream/android-13
#ifdef CONFIG_PM
	.suspend		= qtnf_suspend,
	.resume			= qtnf_resume,
	.set_wakeup		= qtnf_set_wakeup,
#endif
};

<<<<<<< HEAD
static void qtnf_cfg80211_reg_notifier(struct wiphy *wiphy_in,
				       struct regulatory_request *req)
{
	struct qtnf_wmac *mac = wiphy_priv(wiphy_in);
	struct qtnf_bus *bus = mac->bus;
	struct wiphy *wiphy;
	unsigned int mac_idx;
=======
static void qtnf_cfg80211_reg_notifier(struct wiphy *wiphy,
				       struct regulatory_request *req)
{
	struct qtnf_wmac *mac = wiphy_priv(wiphy);
>>>>>>> upstream/android-13
	enum nl80211_band band;
	int ret;

	pr_debug("MAC%u: initiator=%d alpha=%c%c\n", mac->macid, req->initiator,
		 req->alpha2[0], req->alpha2[1]);

<<<<<<< HEAD
	ret = qtnf_cmd_reg_notify(bus, req);
	if (ret) {
		if (ret != -EOPNOTSUPP && ret != -EALREADY)
			pr_err("failed to update reg domain to %c%c\n",
			       req->alpha2[0], req->alpha2[1]);
		return;
	}

	for (mac_idx = 0; mac_idx < QTNF_MAX_MAC; ++mac_idx) {
		if (!(bus->hw_info.mac_bitmap & (1 << mac_idx)))
			continue;

		mac = bus->mac[mac_idx];
		if (!mac)
			continue;

		wiphy = priv_to_wiphy(mac);

		for (band = 0; band < NUM_NL80211_BANDS; ++band) {
			if (!wiphy->bands[band])
				continue;

			ret = qtnf_cmd_band_info_get(mac, wiphy->bands[band]);
			if (ret)
				pr_err("failed to get chan info for mac %u band %u\n",
				       mac_idx, band);
		}
	}
}

struct wiphy *qtnf_wiphy_allocate(struct qtnf_bus *bus)
{
	struct wiphy *wiphy;

	if (bus->hw_info.hw_capab & QLINK_HW_CAPAB_DFS_OFFLOAD)
		qtn_cfg80211_ops.start_radar_detection = NULL;

	if (!(bus->hw_info.hw_capab & QLINK_HW_CAPAB_PWR_MGMT))
=======
	ret = qtnf_cmd_reg_notify(mac, req, qtnf_slave_radar_get(),
				  qtnf_dfs_offload_get());
	if (ret) {
		pr_err("MAC%u: failed to update region to %c%c: %d\n",
		       mac->macid, req->alpha2[0], req->alpha2[1], ret);
		return;
	}

	for (band = 0; band < NUM_NL80211_BANDS; ++band) {
		if (!wiphy->bands[band])
			continue;

		ret = qtnf_cmd_band_info_get(mac, wiphy->bands[band]);
		if (ret)
			pr_err("MAC%u: failed to update band %u\n",
			       mac->macid, band);
	}
}

struct wiphy *qtnf_wiphy_allocate(struct qtnf_bus *bus,
				  struct platform_device *pdev)
{
	struct wiphy *wiphy;

	if (qtnf_dfs_offload_get() &&
	    qtnf_hwcap_is_set(&bus->hw_info, QLINK_HW_CAPAB_DFS_OFFLOAD))
		qtn_cfg80211_ops.start_radar_detection = NULL;

	if (!qtnf_hwcap_is_set(&bus->hw_info, QLINK_HW_CAPAB_PWR_MGMT))
>>>>>>> upstream/android-13
		qtn_cfg80211_ops.set_power_mgmt	= NULL;

	wiphy = wiphy_new(&qtn_cfg80211_ops, sizeof(struct qtnf_wmac));
	if (!wiphy)
		return NULL;

<<<<<<< HEAD
	set_wiphy_dev(wiphy, bus->dev);
=======
	if (pdev)
		set_wiphy_dev(wiphy, &pdev->dev);
	else
		set_wiphy_dev(wiphy, bus->dev);
>>>>>>> upstream/android-13

	return wiphy;
}

static int
qtnf_wiphy_setup_if_comb(struct wiphy *wiphy, struct qtnf_mac_info *mac_info)
{
	struct ieee80211_iface_combination *if_comb;
	size_t n_if_comb;
	u16 interface_modes = 0;
	size_t i, j;

	if_comb = mac_info->if_comb;
	n_if_comb = mac_info->n_if_comb;

	if (!if_comb || !n_if_comb)
		return -ENOENT;

	for (i = 0; i < n_if_comb; i++) {
		if_comb[i].radar_detect_widths = mac_info->radar_detect_widths;

		for (j = 0; j < if_comb[i].n_limits; j++)
			interface_modes |= if_comb[i].limits[j].types;
	}

	wiphy->iface_combinations = if_comb;
	wiphy->n_iface_combinations = n_if_comb;
	wiphy->interface_modes = interface_modes;

	return 0;
}

int qtnf_wiphy_register(struct qtnf_hw_info *hw_info, struct qtnf_wmac *mac)
{
	struct wiphy *wiphy = priv_to_wiphy(mac);
	struct qtnf_mac_info *macinfo = &mac->macinfo;
	int ret;
<<<<<<< HEAD
=======
	bool regdomain_is_known;
>>>>>>> upstream/android-13

	if (!wiphy) {
		pr_err("invalid wiphy pointer\n");
		return -EFAULT;
	}

	wiphy->frag_threshold = macinfo->frag_thr;
	wiphy->rts_threshold = macinfo->rts_thr;
	wiphy->retry_short = macinfo->sretry_limit;
	wiphy->retry_long = macinfo->lretry_limit;
	wiphy->coverage_class = macinfo->coverage_class;

	wiphy->max_scan_ssids =
<<<<<<< HEAD
		(hw_info->max_scan_ssids) ? hw_info->max_scan_ssids : 1;
=======
		(macinfo->max_scan_ssids) ? macinfo->max_scan_ssids : 1;
>>>>>>> upstream/android-13
	wiphy->max_scan_ie_len = QTNF_MAX_VSIE_LEN;
	wiphy->mgmt_stypes = qtnf_mgmt_stypes;
	wiphy->max_remain_on_channel_duration = 5000;
	wiphy->max_acl_mac_addrs = macinfo->max_acl_mac_addrs;
	wiphy->max_num_csa_counters = 2;

	ret = qtnf_wiphy_setup_if_comb(wiphy, macinfo);
	if (ret)
		goto out;

	/* Initialize cipher suits */
	wiphy->cipher_suites = qtnf_cipher_suites;
	wiphy->n_cipher_suites = ARRAY_SIZE(qtnf_cipher_suites);
	wiphy->signal_type = CFG80211_SIGNAL_TYPE_MBM;
	wiphy->flags |= WIPHY_FLAG_HAVE_AP_SME |
			WIPHY_FLAG_AP_PROBE_RESP_OFFLOAD |
			WIPHY_FLAG_AP_UAPSD |
<<<<<<< HEAD
			WIPHY_FLAG_HAS_CHANNEL_SWITCH;
	wiphy->flags &= ~WIPHY_FLAG_PS_ON_BY_DEFAULT;

	if (hw_info->hw_capab & QLINK_HW_CAPAB_DFS_OFFLOAD)
		wiphy_ext_feature_set(wiphy, NL80211_EXT_FEATURE_DFS_OFFLOAD);

=======
			WIPHY_FLAG_HAS_CHANNEL_SWITCH |
			WIPHY_FLAG_4ADDR_STATION |
			WIPHY_FLAG_NETNS_OK;
	wiphy->flags &= ~WIPHY_FLAG_PS_ON_BY_DEFAULT;

	if (qtnf_dfs_offload_get() &&
	    qtnf_hwcap_is_set(hw_info, QLINK_HW_CAPAB_DFS_OFFLOAD))
		wiphy_ext_feature_set(wiphy, NL80211_EXT_FEATURE_DFS_OFFLOAD);

	if (qtnf_hwcap_is_set(hw_info, QLINK_HW_CAPAB_SCAN_DWELL))
		wiphy_ext_feature_set(wiphy,
				      NL80211_EXT_FEATURE_SET_SCAN_DWELL);

>>>>>>> upstream/android-13
	wiphy->probe_resp_offload = NL80211_PROBE_RESP_OFFLOAD_SUPPORT_WPS |
				    NL80211_PROBE_RESP_OFFLOAD_SUPPORT_WPS2;

	wiphy->available_antennas_tx = macinfo->num_tx_chain;
	wiphy->available_antennas_rx = macinfo->num_rx_chain;

	wiphy->max_ap_assoc_sta = macinfo->max_ap_assoc_sta;
	wiphy->ht_capa_mod_mask = &macinfo->ht_cap_mod_mask;
	wiphy->vht_capa_mod_mask = &macinfo->vht_cap_mod_mask;

	ether_addr_copy(wiphy->perm_addr, mac->macaddr);

<<<<<<< HEAD
	if (hw_info->hw_capab & QLINK_HW_CAPAB_STA_INACT_TIMEOUT)
		wiphy->features |= NL80211_FEATURE_INACTIVITY_TIMER;

	if (hw_info->hw_capab & QLINK_HW_CAPAB_SCAN_RANDOM_MAC_ADDR)
		wiphy->features |= NL80211_FEATURE_SCAN_RANDOM_MAC_ADDR;

	if (!(hw_info->hw_capab & QLINK_HW_CAPAB_OBSS_SCAN))
		wiphy->features |= NL80211_FEATURE_NEED_OBSS_SCAN;

=======
	if (qtnf_hwcap_is_set(hw_info, QLINK_HW_CAPAB_STA_INACT_TIMEOUT))
		wiphy->features |= NL80211_FEATURE_INACTIVITY_TIMER;

	if (qtnf_hwcap_is_set(hw_info, QLINK_HW_CAPAB_SCAN_RANDOM_MAC_ADDR))
		wiphy->features |= NL80211_FEATURE_SCAN_RANDOM_MAC_ADDR;

	if (!qtnf_hwcap_is_set(hw_info, QLINK_HW_CAPAB_OBSS_SCAN))
		wiphy->features |= NL80211_FEATURE_NEED_OBSS_SCAN;

	if (qtnf_hwcap_is_set(hw_info, QLINK_HW_CAPAB_SAE))
		wiphy->features |= NL80211_FEATURE_SAE;

>>>>>>> upstream/android-13
#ifdef CONFIG_PM
	if (macinfo->wowlan)
		wiphy->wowlan = macinfo->wowlan;
#endif

<<<<<<< HEAD
	if (hw_info->hw_capab & QLINK_HW_CAPAB_REG_UPDATE) {
		wiphy->regulatory_flags |= REGULATORY_STRICT_REG |
			REGULATORY_CUSTOM_REG;
		wiphy->reg_notifier = qtnf_cfg80211_reg_notifier;
		wiphy_apply_custom_regulatory(wiphy, hw_info->rd);
=======
	regdomain_is_known = isalpha(mac->rd->alpha2[0]) &&
				isalpha(mac->rd->alpha2[1]);

	if (qtnf_hwcap_is_set(hw_info, QLINK_HW_CAPAB_REG_UPDATE)) {
		wiphy->reg_notifier = qtnf_cfg80211_reg_notifier;

		if (mac->rd->alpha2[0] == '9' && mac->rd->alpha2[1] == '9') {
			wiphy->regulatory_flags |= REGULATORY_CUSTOM_REG |
				REGULATORY_STRICT_REG;
			wiphy_apply_custom_regulatory(wiphy, mac->rd);
		} else if (regdomain_is_known) {
			wiphy->regulatory_flags |= REGULATORY_STRICT_REG;
		}
>>>>>>> upstream/android-13
	} else {
		wiphy->regulatory_flags |= REGULATORY_WIPHY_SELF_MANAGED;
	}

	if (mac->macinfo.extended_capabilities_len) {
		wiphy->extended_capabilities =
			mac->macinfo.extended_capabilities;
		wiphy->extended_capabilities_mask =
			mac->macinfo.extended_capabilities_mask;
		wiphy->extended_capabilities_len =
			mac->macinfo.extended_capabilities_len;
	}

	strlcpy(wiphy->fw_version, hw_info->fw_version,
		sizeof(wiphy->fw_version));
	wiphy->hw_version = hw_info->hw_version;

	ret = wiphy_register(wiphy);
	if (ret < 0)
		goto out;

	if (wiphy->regulatory_flags & REGULATORY_WIPHY_SELF_MANAGED)
<<<<<<< HEAD
		ret = regulatory_set_wiphy_regd(wiphy, hw_info->rd);
	else if (isalpha(hw_info->rd->alpha2[0]) &&
		 isalpha(hw_info->rd->alpha2[1]))
		ret = regulatory_hint(wiphy, hw_info->rd->alpha2);
=======
		ret = regulatory_set_wiphy_regd(wiphy, mac->rd);
	else if (regdomain_is_known)
		ret = regulatory_hint(wiphy, mac->rd->alpha2);
>>>>>>> upstream/android-13

out:
	return ret;
}

void qtnf_netdev_updown(struct net_device *ndev, bool up)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(ndev);

	if (qtnf_cmd_send_updown_intf(vif, up))
<<<<<<< HEAD
		pr_err("failed to send up/down command to FW\n");
=======
		pr_err("failed to send %s command to VIF%u.%u\n",
		       up ? "UP" : "DOWN", vif->mac->macid, vif->vifid);
>>>>>>> upstream/android-13
}

void qtnf_virtual_intf_cleanup(struct net_device *ndev)
{
	struct qtnf_vif *vif = qtnf_netdev_get_priv(ndev);
	struct qtnf_wmac *mac = wiphy_priv(vif->wdev.wiphy);

<<<<<<< HEAD
	if (vif->wdev.iftype == NL80211_IFTYPE_STATION) {
		switch (vif->sta_state) {
		case QTNF_STA_DISCONNECTED:
			break;
		case QTNF_STA_CONNECTING:
			cfg80211_connect_result(vif->netdev,
						vif->bssid, NULL, 0,
						NULL, 0,
						WLAN_STATUS_UNSPECIFIED_FAILURE,
						GFP_KERNEL);
			qtnf_disconnect(vif->wdev.wiphy, ndev,
					WLAN_REASON_DEAUTH_LEAVING);
			break;
		case QTNF_STA_CONNECTED:
			cfg80211_disconnected(vif->netdev,
					      WLAN_REASON_DEAUTH_LEAVING,
					      NULL, 0, 1, GFP_KERNEL);
			qtnf_disconnect(vif->wdev.wiphy, ndev,
					WLAN_REASON_DEAUTH_LEAVING);
			break;
		}

		vif->sta_state = QTNF_STA_DISCONNECTED;
	}
=======
	if (vif->wdev.iftype == NL80211_IFTYPE_STATION)
		qtnf_disconnect(vif->wdev.wiphy, ndev,
				WLAN_REASON_DEAUTH_LEAVING);
>>>>>>> upstream/android-13

	qtnf_scan_done(mac, true);
}

void qtnf_cfg80211_vif_reset(struct qtnf_vif *vif)
{
<<<<<<< HEAD
	if (vif->wdev.iftype == NL80211_IFTYPE_STATION) {
		switch (vif->sta_state) {
		case QTNF_STA_CONNECTING:
			cfg80211_connect_result(vif->netdev,
						vif->bssid, NULL, 0,
						NULL, 0,
						WLAN_STATUS_UNSPECIFIED_FAILURE,
						GFP_KERNEL);
			break;
		case QTNF_STA_CONNECTED:
			cfg80211_disconnected(vif->netdev,
					      WLAN_REASON_DEAUTH_LEAVING,
					      NULL, 0, 1, GFP_KERNEL);
			break;
		case QTNF_STA_DISCONNECTED:
			break;
		}
	}

	cfg80211_shutdown_all_interfaces(vif->wdev.wiphy);
	vif->sta_state = QTNF_STA_DISCONNECTED;
=======
	if (vif->wdev.iftype == NL80211_IFTYPE_STATION)
		cfg80211_disconnected(vif->netdev, WLAN_REASON_DEAUTH_LEAVING,
				      NULL, 0, 1, GFP_KERNEL);

	cfg80211_shutdown_all_interfaces(vif->wdev.wiphy);
>>>>>>> upstream/android-13
}

void qtnf_band_init_rates(struct ieee80211_supported_band *band)
{
	switch (band->band) {
	case NL80211_BAND_2GHZ:
		band->bitrates = qtnf_rates_2g;
		band->n_bitrates = ARRAY_SIZE(qtnf_rates_2g);
		break;
	case NL80211_BAND_5GHZ:
		band->bitrates = qtnf_rates_5g;
		band->n_bitrates = ARRAY_SIZE(qtnf_rates_5g);
		break;
	default:
		band->bitrates = NULL;
		band->n_bitrates = 0;
		break;
	}
}
