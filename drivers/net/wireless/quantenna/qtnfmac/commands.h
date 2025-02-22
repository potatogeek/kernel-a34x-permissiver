<<<<<<< HEAD
/*
 * Copyright (c) 2016 Quantenna Communications, Inc.
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
/* Copyright (c) 2016 Quantenna Communications. All rights reserved. */
>>>>>>> upstream/android-13

#ifndef QLINK_COMMANDS_H_
#define QLINK_COMMANDS_H_

#include <linux/nl80211.h>

#include "core.h"
#include "bus.h"

int qtnf_cmd_send_init_fw(struct qtnf_bus *bus);
void qtnf_cmd_send_deinit_fw(struct qtnf_bus *bus);
int qtnf_cmd_get_hw_info(struct qtnf_bus *bus);
int qtnf_cmd_get_mac_info(struct qtnf_wmac *mac);
int qtnf_cmd_send_add_intf(struct qtnf_vif *vif, enum nl80211_iftype iftype,
<<<<<<< HEAD
			   u8 *mac_addr);
int qtnf_cmd_send_change_intf_type(struct qtnf_vif *vif,
				   enum nl80211_iftype iftype, u8 *mac_addr);
=======
			   int use4addr, u8 *mac_addr);
int qtnf_cmd_send_change_intf_type(struct qtnf_vif *vif,
				   enum nl80211_iftype iftype,
				   int use4addr,
				   u8 *mac_addr);
>>>>>>> upstream/android-13
int qtnf_cmd_send_del_intf(struct qtnf_vif *vif);
int qtnf_cmd_band_info_get(struct qtnf_wmac *mac,
			   struct ieee80211_supported_band *band);
int qtnf_cmd_send_regulatory_config(struct qtnf_wmac *mac, const char *alpha2);
int qtnf_cmd_send_start_ap(struct qtnf_vif *vif,
			   const struct cfg80211_ap_settings *s);
int qtnf_cmd_send_stop_ap(struct qtnf_vif *vif);
int qtnf_cmd_send_register_mgmt(struct qtnf_vif *vif, u16 frame_type, bool reg);
<<<<<<< HEAD
int qtnf_cmd_send_mgmt_frame(struct qtnf_vif *vif, u32 cookie, u16 flags,
			     u16 freq, const u8 *buf, size_t len);
=======
int qtnf_cmd_send_frame(struct qtnf_vif *vif, u32 cookie, u16 flags,
			u16 freq, const u8 *buf, size_t len);
>>>>>>> upstream/android-13
int qtnf_cmd_send_mgmt_set_appie(struct qtnf_vif *vif, u8 frame_type,
				 const u8 *buf, size_t len);
int qtnf_cmd_get_sta_info(struct qtnf_vif *vif, const u8 *sta_mac,
			  struct station_info *sinfo);
int qtnf_cmd_send_phy_params(struct qtnf_wmac *mac, u16 cmd_action,
			     void *data_buf);
int qtnf_cmd_send_add_key(struct qtnf_vif *vif, u8 key_index, bool pairwise,
			  const u8 *mac_addr, struct key_params *params);
int qtnf_cmd_send_del_key(struct qtnf_vif *vif, u8 key_index, bool pairwise,
			  const u8 *mac_addr);
int qtnf_cmd_send_set_default_key(struct qtnf_vif *vif, u8 key_index,
				  bool unicast, bool multicast);
int qtnf_cmd_send_set_default_mgmt_key(struct qtnf_vif *vif, u8 key_index);
int qtnf_cmd_send_add_sta(struct qtnf_vif *vif, const u8 *mac,
			  struct station_parameters *params);
int qtnf_cmd_send_change_sta(struct qtnf_vif *vif, const u8 *mac,
			     struct station_parameters *params);
int qtnf_cmd_send_del_sta(struct qtnf_vif *vif,
			  struct station_del_parameters *params);
int qtnf_cmd_send_scan(struct qtnf_wmac *mac);
int qtnf_cmd_send_connect(struct qtnf_vif *vif,
			  struct cfg80211_connect_params *sme);
<<<<<<< HEAD
=======
int qtnf_cmd_send_external_auth(struct qtnf_vif *vif,
				struct cfg80211_external_auth_params *auth);
>>>>>>> upstream/android-13
int qtnf_cmd_send_disconnect(struct qtnf_vif *vif,
			     u16 reason_code);
int qtnf_cmd_send_updown_intf(struct qtnf_vif *vif,
			      bool up);
<<<<<<< HEAD
int qtnf_cmd_reg_notify(struct qtnf_bus *bus, struct regulatory_request *req);
int qtnf_cmd_get_chan_stats(struct qtnf_wmac *mac, u16 channel,
			    struct qtnf_chan_stats *stats);
=======
int qtnf_cmd_reg_notify(struct qtnf_wmac *mac, struct regulatory_request *req,
			bool slave_radar, bool dfs_offload);
int qtnf_cmd_get_chan_stats(struct qtnf_wmac *mac, u32 chan_freq,
			    struct survey_info *survey);
>>>>>>> upstream/android-13
int qtnf_cmd_send_chan_switch(struct qtnf_vif *vif,
			      struct cfg80211_csa_settings *params);
int qtnf_cmd_get_channel(struct qtnf_vif *vif, struct cfg80211_chan_def *chdef);
int qtnf_cmd_start_cac(const struct qtnf_vif *vif,
		       const struct cfg80211_chan_def *chdef,
		       u32 cac_time_ms);
int qtnf_cmd_set_mac_acl(const struct qtnf_vif *vif,
			 const struct cfg80211_acl_data *params);
int qtnf_cmd_send_pm_set(const struct qtnf_vif *vif, u8 pm_mode, int timeout);
<<<<<<< HEAD
int qtnf_cmd_send_wowlan_set(const struct qtnf_vif *vif,
			     const struct cfg80211_wowlan *wowl);
=======
int qtnf_cmd_get_tx_power(const struct qtnf_vif *vif, int *dbm);
int qtnf_cmd_set_tx_power(const struct qtnf_vif *vif,
			  enum nl80211_tx_power_setting type, int mbm);
int qtnf_cmd_send_wowlan_set(const struct qtnf_vif *vif,
			     const struct cfg80211_wowlan *wowl);
int qtnf_cmd_netdev_changeupper(const struct qtnf_vif *vif, int br_domain);
int qtnf_cmd_send_update_owe(struct qtnf_vif *vif,
			     struct cfg80211_update_owe_info *owe);
>>>>>>> upstream/android-13

#endif /* QLINK_COMMANDS_H_ */
