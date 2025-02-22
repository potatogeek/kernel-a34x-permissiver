<<<<<<< HEAD
/******************************************************************************
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * Copyright(c) 2012 - 2014 Intel Corporation. All rights reserved.
 * Copyright(c) 2013 - 2014 Intel Mobile Communications GmbH
 * Copyright(c) 2016 - 2017 Intel Deutschland GmbH
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110,
 * USA
 *
 * The full GNU General Public License is included in this distribution
 * in the file called COPYING.
 *
 * Contact Information:
 *  Intel Linux Wireless <linuxwifi@intel.com>
 * Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497
 *
 * BSD LICENSE
 *
 * Copyright(c) 2012 - 2014 Intel Corporation. All rights reserved.
 * Copyright(c) 2013 - 2014 Intel Mobile Communications GmbH
 * Copyright(c) 2016 - 2017 Intel Deutschland GmbH
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

=======
// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/*
 * Copyright (C) 2012-2014, 2018 Intel Corporation
 * Copyright (C) 2013-2014 Intel Mobile Communications GmbH
 * Copyright (C) 2016-2017 Intel Deutschland GmbH
 */
>>>>>>> upstream/android-13
#include <net/mac80211.h>
#include "fw-api.h"
#include "mvm.h"

#define QUOTA_100	IWL_MVM_MAX_QUOTA
#define QUOTA_LOWLAT_MIN ((QUOTA_100 * IWL_MVM_LOWLAT_QUOTA_MIN_PERCENT) / 100)

struct iwl_mvm_quota_iterator_data {
	int n_interfaces[MAX_BINDINGS];
	int colors[MAX_BINDINGS];
	int low_latency[MAX_BINDINGS];
#ifdef CONFIG_IWLWIFI_DEBUGFS
	int dbgfs_min[MAX_BINDINGS];
#endif
	int n_low_latency_bindings;
	struct ieee80211_vif *disabled_vif;
};

static void iwl_mvm_quota_iterator(void *_data, u8 *mac,
				   struct ieee80211_vif *vif)
{
	struct iwl_mvm_quota_iterator_data *data = _data;
	struct iwl_mvm_vif *mvmvif = iwl_mvm_vif_from_mac80211(vif);
	u16 id;

	/* skip disabled interfaces here immediately */
	if (vif == data->disabled_vif)
		return;

	if (!mvmvif->phy_ctxt)
		return;

	/* currently, PHY ID == binding ID */
	id = mvmvif->phy_ctxt->id;

	/* need at least one binding per PHY */
	BUILD_BUG_ON(NUM_PHY_CTX > MAX_BINDINGS);

	if (WARN_ON_ONCE(id >= MAX_BINDINGS))
		return;

	switch (vif->type) {
	case NL80211_IFTYPE_STATION:
		if (vif->bss_conf.assoc)
			break;
		return;
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_ADHOC:
		if (mvmvif->ap_ibss_active)
			break;
		return;
	case NL80211_IFTYPE_MONITOR:
		if (mvmvif->monitor_active)
			break;
		return;
	case NL80211_IFTYPE_P2P_DEVICE:
		return;
	default:
		WARN_ON_ONCE(1);
		return;
	}

	if (data->colors[id] < 0)
		data->colors[id] = mvmvif->phy_ctxt->color;
	else
		WARN_ON_ONCE(data->colors[id] != mvmvif->phy_ctxt->color);

	data->n_interfaces[id]++;

#ifdef CONFIG_IWLWIFI_DEBUGFS
	if (mvmvif->dbgfs_quota_min)
		data->dbgfs_min[id] = max(data->dbgfs_min[id],
					  mvmvif->dbgfs_quota_min);
#endif

	if (iwl_mvm_vif_low_latency(mvmvif) && !data->low_latency[id]) {
		data->n_low_latency_bindings++;
		data->low_latency[id] = true;
	}
}

static void iwl_mvm_adjust_quota_for_noa(struct iwl_mvm *mvm,
					 struct iwl_time_quota_cmd *cmd)
{
#ifdef CONFIG_NL80211_TESTMODE
	struct iwl_mvm_vif *mvmvif;
	int i, phy_id = -1, beacon_int = 0;

	if (!mvm->noa_duration || !mvm->noa_vif)
		return;

	mvmvif = iwl_mvm_vif_from_mac80211(mvm->noa_vif);
	if (!mvmvif->ap_ibss_active)
		return;

	phy_id = mvmvif->phy_ctxt->id;
	beacon_int = mvm->noa_vif->bss_conf.beacon_int;

	for (i = 0; i < MAX_BINDINGS; i++) {
		struct iwl_time_quota_data *data =
					iwl_mvm_quota_cmd_get_quota(mvm, cmd,
								    i);
		u32 id_n_c = le32_to_cpu(data->id_and_color);
		u32 id = (id_n_c & FW_CTXT_ID_MSK) >> FW_CTXT_ID_POS;
		u32 quota = le32_to_cpu(data->quota);

		if (id != phy_id)
			continue;

		quota *= (beacon_int - mvm->noa_duration);
		quota /= beacon_int;

		IWL_DEBUG_QUOTA(mvm, "quota: adjust for NoA from %d to %d\n",
				le32_to_cpu(data->quota), quota);

		data->quota = cpu_to_le32(quota);
	}
#endif
}

int iwl_mvm_update_quotas(struct iwl_mvm *mvm,
			  bool force_update,
			  struct ieee80211_vif *disabled_vif)
{
	struct iwl_time_quota_cmd cmd = {};
	int i, idx, err, num_active_macs, quota, quota_rem, n_non_lowlat;
	struct iwl_mvm_quota_iterator_data data = {
		.n_interfaces = {},
		.colors = { -1, -1, -1, -1 },
		.disabled_vif = disabled_vif,
	};
	struct iwl_time_quota_cmd *last = &mvm->last_quota_cmd;
	struct iwl_time_quota_data *qdata, *last_data;
	bool send = false;

	lockdep_assert_held(&mvm->mutex);

	if (fw_has_capa(&mvm->fw->ucode_capa,
			IWL_UCODE_TLV_CAPA_DYNAMIC_QUOTA))
		return 0;

	/* update all upon completion */
	if (test_bit(IWL_MVM_STATUS_IN_HW_RESTART, &mvm->status))
		return 0;

	/* iterator data above must match */
	BUILD_BUG_ON(MAX_BINDINGS != 4);

	ieee80211_iterate_active_interfaces_atomic(
		mvm->hw, IEEE80211_IFACE_ITER_NORMAL,
		iwl_mvm_quota_iterator, &data);

	/*
	 * The FW's scheduling session consists of
	 * IWL_MVM_MAX_QUOTA fragments. Divide these fragments
	 * equally between all the bindings that require quota
	 */
	num_active_macs = 0;
	for (i = 0; i < MAX_BINDINGS; i++) {
		qdata = iwl_mvm_quota_cmd_get_quota(mvm, &cmd, i);
		qdata->id_and_color = cpu_to_le32(FW_CTXT_INVALID);
		num_active_macs += data.n_interfaces[i];
	}

	n_non_lowlat = num_active_macs;

	if (data.n_low_latency_bindings == 1) {
		for (i = 0; i < MAX_BINDINGS; i++) {
			if (data.low_latency[i]) {
				n_non_lowlat -= data.n_interfaces[i];
				break;
			}
		}
	}

	if (data.n_low_latency_bindings == 1 && n_non_lowlat) {
		/*
		 * Reserve quota for the low latency binding in case that
		 * there are several data bindings but only a single
		 * low latency one. Split the rest of the quota equally
		 * between the other data interfaces.
		 */
		quota = (QUOTA_100 - QUOTA_LOWLAT_MIN) / n_non_lowlat;
		quota_rem = QUOTA_100 - n_non_lowlat * quota -
			    QUOTA_LOWLAT_MIN;
		IWL_DEBUG_QUOTA(mvm,
				"quota: low-latency binding active, remaining quota per other binding: %d\n",
				quota);
	} else if (num_active_macs) {
		/*
		 * There are 0 or more than 1 low latency bindings, or all the
		 * data interfaces belong to the single low latency binding.
		 * Split the quota equally between the data interfaces.
		 */
		quota = QUOTA_100 / num_active_macs;
		quota_rem = QUOTA_100 % num_active_macs;
		IWL_DEBUG_QUOTA(mvm,
				"quota: splitting evenly per binding: %d\n",
				quota);
	} else {
		/* values don't really matter - won't be used */
		quota = 0;
		quota_rem = 0;
	}

	for (idx = 0, i = 0; i < MAX_BINDINGS; i++) {
		if (data.colors[i] < 0)
			continue;

		qdata = iwl_mvm_quota_cmd_get_quota(mvm, &cmd, idx);

		qdata->id_and_color =
			cpu_to_le32(FW_CMD_ID_AND_COLOR(i, data.colors[i]));

		if (data.n_interfaces[i] <= 0)
			qdata->quota = cpu_to_le32(0);
#ifdef CONFIG_IWLWIFI_DEBUGFS
		else if (data.dbgfs_min[i])
			qdata->quota =
				cpu_to_le32(data.dbgfs_min[i] * QUOTA_100 / 100);
#endif
		else if (data.n_low_latency_bindings == 1 && n_non_lowlat &&
			 data.low_latency[i])
			/*
			 * There is more than one binding, but only one of the
			 * bindings is in low latency. For this case, allocate
			 * the minimal required quota for the low latency
			 * binding.
			 */
			qdata->quota = cpu_to_le32(QUOTA_LOWLAT_MIN);
		else
			qdata->quota =
				cpu_to_le32(quota * data.n_interfaces[i]);

		WARN_ONCE(le32_to_cpu(qdata->quota) > QUOTA_100,
			  "Binding=%d, quota=%u > max=%u\n",
			  idx, le32_to_cpu(qdata->quota), QUOTA_100);

		qdata->max_duration = cpu_to_le32(0);

		idx++;
	}

	/* Give the remainder of the session to the first data binding */
	for (i = 0; i < MAX_BINDINGS; i++) {
		qdata = iwl_mvm_quota_cmd_get_quota(mvm, &cmd, i);
		if (le32_to_cpu(qdata->quota) != 0) {
			le32_add_cpu(&qdata->quota, quota_rem);
			IWL_DEBUG_QUOTA(mvm,
					"quota: giving remainder of %d to binding %d\n",
					quota_rem, i);
			break;
		}
	}

	iwl_mvm_adjust_quota_for_noa(mvm, &cmd);

	/* check that we have non-zero quota for all valid bindings */
	for (i = 0; i < MAX_BINDINGS; i++) {
		qdata = iwl_mvm_quota_cmd_get_quota(mvm, &cmd, i);
		last_data = iwl_mvm_quota_cmd_get_quota(mvm, last, i);
		if (qdata->id_and_color != last_data->id_and_color)
			send = true;
		if (qdata->max_duration != last_data->max_duration)
			send = true;
		if (abs((int)le32_to_cpu(qdata->quota) -
			(int)le32_to_cpu(last_data->quota))
						> IWL_MVM_QUOTA_THRESHOLD)
			send = true;
		if (qdata->id_and_color == cpu_to_le32(FW_CTXT_INVALID))
			continue;
		WARN_ONCE(qdata->quota == 0,
			  "zero quota on binding %d\n", i);
	}

	if (!send && !force_update) {
		/* don't send a practically unchanged command, the firmware has
		 * to re-initialize a lot of state and that can have an adverse
		 * impact on it
		 */
		return 0;
	}

	err = iwl_mvm_send_cmd_pdu(mvm, TIME_QUOTA_CMD, 0,
				   iwl_mvm_quota_cmd_size(mvm), &cmd);

	if (err)
		IWL_ERR(mvm, "Failed to send quota: %d\n", err);
	else
		mvm->last_quota_cmd = cmd;
	return err;
}
