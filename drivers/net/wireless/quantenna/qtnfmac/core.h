<<<<<<< HEAD
/*
 * Copyright (c) 2015-2016 Quantenna Communications, Inc.
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
/* SPDX-License-Identifier: GPL-2.0+ */
/* Copyright (c) 2015-2016 Quantenna Communications. All rights reserved. */
>>>>>>> upstream/android-13

#ifndef _QTN_FMAC_CORE_H_
#define _QTN_FMAC_CORE_H_

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/if_arp.h>
#include <linux/etherdevice.h>
#include <net/sock.h>
#include <net/lib80211.h>
#include <net/cfg80211.h>
#include <linux/vmalloc.h>
#include <linux/firmware.h>
#include <linux/ctype.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
<<<<<<< HEAD

#include "qlink.h"
#include "trans.h"
=======
#include <linux/platform_device.h>

#include "qlink.h"
#include "trans.h"
#include "qlink_util.h"
>>>>>>> upstream/android-13

#undef pr_fmt
#define pr_fmt(fmt)	KBUILD_MODNAME ": %s: " fmt, __func__

#define QTNF_MAX_VSIE_LEN		255
#define QTNF_MAX_INTF			8
#define QTNF_MAX_EVENT_QUEUE_LEN	255
#define QTNF_SCAN_TIMEOUT_SEC		15

#define QTNF_DEF_BSS_PRIORITY		0
#define QTNF_DEF_WDOG_TIMEOUT		5
#define QTNF_TX_TIMEOUT_TRSHLD		100

extern const struct net_device_ops qtnf_netdev_ops;

struct qtnf_bus;
struct qtnf_vif;

struct qtnf_sta_node {
	struct list_head list;
	u8 mac_addr[ETH_ALEN];
};

struct qtnf_sta_list {
	struct list_head head;
	atomic_t size;
};

<<<<<<< HEAD
enum qtnf_sta_state {
	QTNF_STA_DISCONNECTED,
	QTNF_STA_CONNECTING,
	QTNF_STA_CONNECTED
};

=======
>>>>>>> upstream/android-13
struct qtnf_vif {
	struct wireless_dev wdev;
	u8 bssid[ETH_ALEN];
	u8 mac_addr[ETH_ALEN];
	u8 vifid;
	u8 bss_priority;
	u8 bss_status;
<<<<<<< HEAD
	enum qtnf_sta_state sta_state;
=======
>>>>>>> upstream/android-13
	u16 mgmt_frames_bitmask;
	struct net_device *netdev;
	struct qtnf_wmac *mac;

	struct work_struct reset_work;
<<<<<<< HEAD
	struct qtnf_sta_list sta_list;
	unsigned long cons_tx_timeout_cnt;
	int generation;

	struct pcpu_sw_netstats __percpu *stats64;
=======
	struct work_struct high_pri_tx_work;
	struct sk_buff_head high_pri_tx_queue;
	struct qtnf_sta_list sta_list;
	unsigned long cons_tx_timeout_cnt;
	int generation;
>>>>>>> upstream/android-13
};

struct qtnf_mac_info {
	u8 bands_cap;
<<<<<<< HEAD
	u8 dev_mac[ETH_ALEN];
=======
>>>>>>> upstream/android-13
	u8 num_tx_chain;
	u8 num_rx_chain;
	u16 max_ap_assoc_sta;
	u32 frag_thr;
	u32 rts_thr;
	u8 lretry_limit;
	u8 sretry_limit;
	u8 coverage_class;
	u8 radar_detect_widths;
<<<<<<< HEAD
	u32 max_acl_mac_addrs;
=======
	u8 max_scan_ssids;
	u16 max_acl_mac_addrs;
>>>>>>> upstream/android-13
	struct ieee80211_ht_cap ht_cap_mod_mask;
	struct ieee80211_vht_cap vht_cap_mod_mask;
	struct ieee80211_iface_combination *if_comb;
	size_t n_if_comb;
	u8 *extended_capabilities;
	u8 *extended_capabilities_mask;
	u8 extended_capabilities_len;
	struct wiphy_wowlan_support *wowlan;
};

<<<<<<< HEAD
struct qtnf_chan_stats {
	u32 chan_num;
	u32 cca_tx;
	u32 cca_rx;
	u32 cca_busy;
	u32 cca_try;
	s8 chan_noise;
};

=======
>>>>>>> upstream/android-13
struct qtnf_wmac {
	u8 macid;
	u8 wiphy_registered;
	u8 macaddr[ETH_ALEN];
	struct qtnf_bus *bus;
	struct qtnf_mac_info macinfo;
	struct qtnf_vif iflist[QTNF_MAX_INTF];
	struct cfg80211_scan_request *scan_req;
	struct mutex mac_lock;	/* lock during wmac speicific ops */
	struct delayed_work scan_timeout;
<<<<<<< HEAD
};

struct qtnf_hw_info {
	u16 ql_proto_ver;
	u8 num_mac;
	u8 mac_bitmap;
	u32 fw_ver;
	u32 hw_capab;
	struct ieee80211_regdomain *rd;
=======
	struct ieee80211_regdomain *rd;
	struct platform_device *pdev;
};

struct qtnf_hw_info {
	u32 ql_proto_ver;
	u8 num_mac;
	u8 mac_bitmap;
	u32 fw_ver;
>>>>>>> upstream/android-13
	u8 total_tx_chain;
	u8 total_rx_chain;
	char fw_version[ETHTOOL_FWVERS_LEN];
	u32 hw_version;
<<<<<<< HEAD
	u8 max_scan_ssids;
=======
	u8 hw_capab[QLINK_HW_CAPAB_NUM / BITS_PER_BYTE + 1];
>>>>>>> upstream/android-13
};

struct qtnf_vif *qtnf_mac_get_free_vif(struct qtnf_wmac *mac);
struct qtnf_vif *qtnf_mac_get_base_vif(struct qtnf_wmac *mac);
void qtnf_mac_iface_comb_free(struct qtnf_wmac *mac);
void qtnf_mac_ext_caps_free(struct qtnf_wmac *mac);
<<<<<<< HEAD
struct wiphy *qtnf_wiphy_allocate(struct qtnf_bus *bus);
=======
bool qtnf_slave_radar_get(void);
bool qtnf_dfs_offload_get(void);
struct wiphy *qtnf_wiphy_allocate(struct qtnf_bus *bus,
				  struct platform_device *pdev);
>>>>>>> upstream/android-13
int qtnf_core_net_attach(struct qtnf_wmac *mac, struct qtnf_vif *priv,
			 const char *name, unsigned char name_assign_type);
void qtnf_main_work_queue(struct work_struct *work);
int qtnf_cmd_send_update_phy_params(struct qtnf_wmac *mac, u32 changed);
<<<<<<< HEAD
int qtnf_cmd_send_get_phy_params(struct qtnf_wmac *mac);
=======
>>>>>>> upstream/android-13

struct qtnf_wmac *qtnf_core_get_mac(const struct qtnf_bus *bus, u8 macid);
struct net_device *qtnf_classify_skb(struct qtnf_bus *bus, struct sk_buff *skb);
void qtnf_wake_all_queues(struct net_device *ndev);
<<<<<<< HEAD
void qtnf_update_rx_stats(struct net_device *ndev, const struct sk_buff *skb);
void qtnf_update_tx_stats(struct net_device *ndev, const struct sk_buff *skb);
=======
>>>>>>> upstream/android-13

void qtnf_virtual_intf_cleanup(struct net_device *ndev);

void qtnf_netdev_updown(struct net_device *ndev, bool up);
void qtnf_scan_done(struct qtnf_wmac *mac, bool aborted);
<<<<<<< HEAD
=======
struct dentry *qtnf_get_debugfs_dir(void);
bool qtnf_netdev_is_qtn(const struct net_device *ndev);
>>>>>>> upstream/android-13

static inline struct qtnf_vif *qtnf_netdev_get_priv(struct net_device *dev)
{
	return *((void **)netdev_priv(dev));
}

<<<<<<< HEAD
=======
static inline bool qtnf_hwcap_is_set(const struct qtnf_hw_info *info,
				     unsigned int bit)
{
	return qtnf_utils_is_bit_set(info->hw_capab, bit,
				     sizeof(info->hw_capab));
}

>>>>>>> upstream/android-13
#endif /* _QTN_FMAC_CORE_H_ */
