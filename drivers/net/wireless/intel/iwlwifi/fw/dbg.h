<<<<<<< HEAD
/******************************************************************************
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * Copyright(c) 2008 - 2014 Intel Corporation. All rights reserved.
 * Copyright(c) 2013 - 2015 Intel Mobile Communications GmbH
 * Copyright(c) 2015 - 2017 Intel Deutschland GmbH
 * Copyright(c) 2018        Intel Corporation
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
 * along with this program;
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
 * Copyright(c) 2005 - 2014 Intel Corporation. All rights reserved.
 * Copyright(c) 2013 - 2015 Intel Mobile Communications GmbH
 * Copyright(c) 2015 - 2017 Intel Deutschland GmbH
 * Copyright(c) 2018        Intel Corporation
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
/* SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause */
/*
 * Copyright (C) 2005-2014, 2018-2019, 2021 Intel Corporation
 * Copyright (C) 2013-2015 Intel Mobile Communications GmbH
 * Copyright (C) 2015-2017 Intel Deutschland GmbH
 */
>>>>>>> upstream/android-13
#ifndef __iwl_fw_dbg_h__
#define __iwl_fw_dbg_h__
#include <linux/workqueue.h>
#include <net/cfg80211.h>
#include "runtime.h"
#include "iwl-prph.h"
#include "iwl-io.h"
#include "file.h"
#include "error-dump.h"
<<<<<<< HEAD
=======
#include "api/commands.h"
#include "api/dbg-tlv.h"
#include "api/alive.h"
>>>>>>> upstream/android-13

/**
 * struct iwl_fw_dump_desc - describes the dump
 * @len: length of trig_desc->data
 * @trig_desc: the description of the dump
 */
struct iwl_fw_dump_desc {
	size_t len;
	/* must be last */
	struct iwl_fw_error_dump_trigger_desc trig_desc;
};

<<<<<<< HEAD
extern const struct iwl_fw_dump_desc iwl_dump_desc_assert;

static inline void iwl_fw_free_dump_desc(struct iwl_fw_runtime *fwrt)
{
	if (fwrt->dump.desc != &iwl_dump_desc_assert)
		kfree(fwrt->dump.desc);
	fwrt->dump.desc = NULL;
	fwrt->dump.trig = NULL;
}

void iwl_fw_error_dump(struct iwl_fw_runtime *fwrt);
int iwl_fw_dbg_collect_desc(struct iwl_fw_runtime *fwrt,
			    const struct iwl_fw_dump_desc *desc,
			    const struct iwl_fw_dbg_trigger_tlv *trigger);
int iwl_fw_dbg_collect(struct iwl_fw_runtime *fwrt,
		       enum iwl_fw_dbg_trigger trig,
		       const char *str, size_t len,
		       const struct iwl_fw_dbg_trigger_tlv *trigger);
=======
/**
 * struct iwl_fw_dbg_params - register values to restore
 * @in_sample: DBGC_IN_SAMPLE value
 * @out_ctrl: DBGC_OUT_CTRL value
 */
struct iwl_fw_dbg_params {
	u32 in_sample;
	u32 out_ctrl;
};

extern const struct iwl_fw_dump_desc iwl_dump_desc_assert;

int iwl_fw_dbg_collect_desc(struct iwl_fw_runtime *fwrt,
			    const struct iwl_fw_dump_desc *desc,
			    bool monitor_only, unsigned int delay);
int iwl_fw_dbg_error_collect(struct iwl_fw_runtime *fwrt,
			     enum iwl_fw_dbg_trigger trig_type);
int iwl_fw_dbg_ini_collect(struct iwl_fw_runtime *fwrt,
			   struct iwl_fwrt_dump_data *dump_data,
			   bool sync);
int iwl_fw_dbg_collect(struct iwl_fw_runtime *fwrt,
		       enum iwl_fw_dbg_trigger trig, const char *str,
		       size_t len, struct iwl_fw_dbg_trigger_tlv *trigger);
>>>>>>> upstream/android-13
int iwl_fw_dbg_collect_trig(struct iwl_fw_runtime *fwrt,
			    struct iwl_fw_dbg_trigger_tlv *trigger,
			    const char *fmt, ...) __printf(3, 4);
int iwl_fw_start_dbg_conf(struct iwl_fw_runtime *fwrt, u8 id);

#define iwl_fw_dbg_trigger_enabled(fw, id) ({			\
<<<<<<< HEAD
	void *__dbg_trigger = (fw)->dbg_trigger_tlv[(id)];	\
=======
	void *__dbg_trigger = (fw)->dbg.trigger_tlv[(id)];	\
>>>>>>> upstream/android-13
	unlikely(__dbg_trigger);				\
})

static inline struct iwl_fw_dbg_trigger_tlv*
_iwl_fw_dbg_get_trigger(const struct iwl_fw *fw, enum iwl_fw_dbg_trigger id)
{
<<<<<<< HEAD
	return fw->dbg_trigger_tlv[id];
=======
	return fw->dbg.trigger_tlv[id];
>>>>>>> upstream/android-13
}

#define iwl_fw_dbg_get_trigger(fw, id) ({			\
	BUILD_BUG_ON(!__builtin_constant_p(id));		\
	BUILD_BUG_ON((id) >= FW_DBG_TRIGGER_MAX);		\
	_iwl_fw_dbg_get_trigger((fw), (id));			\
})

static inline bool
iwl_fw_dbg_trigger_vif_match(struct iwl_fw_dbg_trigger_tlv *trig,
			     struct wireless_dev *wdev)
{
	u32 trig_vif = le32_to_cpu(trig->vif_type);

	return trig_vif == IWL_FW_DBG_CONF_VIF_ANY ||
	       wdev->iftype == trig_vif;
}

static inline bool
iwl_fw_dbg_trigger_stop_conf_match(struct iwl_fw_runtime *fwrt,
				   struct iwl_fw_dbg_trigger_tlv *trig)
{
	return ((trig->mode & IWL_FW_DBG_TRIGGER_STOP) &&
		(fwrt->dump.conf == FW_DBG_INVALID ||
		(BIT(fwrt->dump.conf) & le32_to_cpu(trig->stop_conf_ids))));
}

static inline bool
<<<<<<< HEAD
iwl_fw_dbg_no_trig_window(struct iwl_fw_runtime *fwrt,
			  struct iwl_fw_dbg_trigger_tlv *trig)
{
	unsigned long wind_jiff =
		msecs_to_jiffies(le16_to_cpu(trig->trig_dis_ms));
	u32 id = le32_to_cpu(trig->id);
=======
iwl_fw_dbg_no_trig_window(struct iwl_fw_runtime *fwrt, u32 id, u32 dis_usec)
{
	unsigned long wind_jiff = usecs_to_jiffies(dis_usec);
>>>>>>> upstream/android-13

	/* If this is the first event checked, jump to update start ts */
	if (fwrt->dump.non_collect_ts_start[id] &&
	    (time_after(fwrt->dump.non_collect_ts_start[id] + wind_jiff,
			jiffies)))
		return true;

	fwrt->dump.non_collect_ts_start[id] = jiffies;
	return false;
}

static inline bool
iwl_fw_dbg_trigger_check_stop(struct iwl_fw_runtime *fwrt,
			      struct wireless_dev *wdev,
			      struct iwl_fw_dbg_trigger_tlv *trig)
{
<<<<<<< HEAD
	if (wdev && !iwl_fw_dbg_trigger_vif_match(trig, wdev))
		return false;

	if (iwl_fw_dbg_no_trig_window(fwrt, trig)) {
=======
	u32 usec = le16_to_cpu(trig->trig_dis_ms) * USEC_PER_MSEC;

	if (wdev && !iwl_fw_dbg_trigger_vif_match(trig, wdev))
		return false;

	if (iwl_fw_dbg_no_trig_window(fwrt, le32_to_cpu(trig->id), usec)) {
>>>>>>> upstream/android-13
		IWL_WARN(fwrt, "Trigger %d occurred while no-collect window.\n",
			 trig->id);
		return false;
	}

	return iwl_fw_dbg_trigger_stop_conf_match(fwrt, trig);
}

<<<<<<< HEAD
=======
static inline struct iwl_fw_dbg_trigger_tlv*
_iwl_fw_dbg_trigger_on(struct iwl_fw_runtime *fwrt,
		       struct wireless_dev *wdev,
		       const enum iwl_fw_dbg_trigger id)
{
	struct iwl_fw_dbg_trigger_tlv *trig;

	if (iwl_trans_dbg_ini_valid(fwrt->trans))
		return NULL;

	if (!iwl_fw_dbg_trigger_enabled(fwrt->fw, id))
		return NULL;

	trig = _iwl_fw_dbg_get_trigger(fwrt->fw, id);

	if (!iwl_fw_dbg_trigger_check_stop(fwrt, wdev, trig))
		return NULL;

	return trig;
}

#define iwl_fw_dbg_trigger_on(fwrt, wdev, id) ({		\
	BUILD_BUG_ON(!__builtin_constant_p(id));		\
	BUILD_BUG_ON((id) >= FW_DBG_TRIGGER_MAX);		\
	_iwl_fw_dbg_trigger_on((fwrt), (wdev), (id));		\
})

>>>>>>> upstream/android-13
static inline void
_iwl_fw_dbg_trigger_simple_stop(struct iwl_fw_runtime *fwrt,
				struct wireless_dev *wdev,
				struct iwl_fw_dbg_trigger_tlv *trigger)
{
	if (!trigger)
		return;

	if (!iwl_fw_dbg_trigger_check_stop(fwrt, wdev, trigger))
		return;

	iwl_fw_dbg_collect_trig(fwrt, trigger, NULL);
}

#define iwl_fw_dbg_trigger_simple_stop(fwrt, wdev, trig)	\
	_iwl_fw_dbg_trigger_simple_stop((fwrt), (wdev),		\
					iwl_fw_dbg_get_trigger((fwrt)->fw,\
							       (trig)))
<<<<<<< HEAD

static inline void iwl_fw_dbg_stop_recording(struct iwl_fw_runtime *fwrt)
{
	if (fwrt->trans->cfg->device_family == IWL_DEVICE_FAMILY_7000) {
		iwl_set_bits_prph(fwrt->trans, MON_BUFF_SAMPLE_CTL, 0x100);
	} else {
		iwl_write_prph(fwrt->trans, DBGC_IN_SAMPLE, 0);
		udelay(100);
		iwl_write_prph(fwrt->trans, DBGC_OUT_CTRL, 0);
	}
}
=======
void iwl_fw_dbg_stop_restart_recording(struct iwl_fw_runtime *fwrt,
				       struct iwl_fw_dbg_params *params,
				       bool stop);

#ifdef CONFIG_IWLWIFI_DEBUGFS
static inline void iwl_fw_set_dbg_rec_on(struct iwl_fw_runtime *fwrt)
{
	if (fwrt->cur_fw_img == IWL_UCODE_REGULAR &&
	    (fwrt->fw->dbg.dest_tlv ||
	     fwrt->trans->dbg.ini_dest != IWL_FW_INI_LOCATION_INVALID))
		fwrt->trans->dbg.rec_on = true;
}
#endif
>>>>>>> upstream/android-13

static inline void iwl_fw_dump_conf_clear(struct iwl_fw_runtime *fwrt)
{
	fwrt->dump.conf = FW_DBG_INVALID;
}

void iwl_fw_error_dump_wk(struct work_struct *work);

<<<<<<< HEAD
static inline void iwl_fw_flush_dump(struct iwl_fw_runtime *fwrt)
{
	flush_delayed_work(&fwrt->dump.wk);
}

static inline void iwl_fw_cancel_dump(struct iwl_fw_runtime *fwrt)
{
	cancel_delayed_work_sync(&fwrt->dump.wk);
=======
static inline bool iwl_fw_dbg_type_on(struct iwl_fw_runtime *fwrt, u32 type)
{
	return (fwrt->fw->dbg.dump_mask & BIT(type));
}

static inline bool iwl_fw_dbg_is_d3_debug_enabled(struct iwl_fw_runtime *fwrt)
{
	return fw_has_capa(&fwrt->fw->ucode_capa,
			   IWL_UCODE_TLV_CAPA_D3_DEBUG) &&
		fwrt->trans->cfg->d3_debug_data_length && fwrt->ops &&
		fwrt->ops->d3_debug_enable &&
		fwrt->ops->d3_debug_enable(fwrt->ops_ctx) &&
		iwl_fw_dbg_type_on(fwrt, IWL_FW_ERROR_DUMP_D3_DEBUG_DATA);
}

static inline bool iwl_fw_dbg_is_paging_enabled(struct iwl_fw_runtime *fwrt)
{
	return iwl_fw_dbg_type_on(fwrt, IWL_FW_ERROR_DUMP_PAGING) &&
		!fwrt->trans->trans_cfg->gen2 &&
		fwrt->cur_fw_img < IWL_UCODE_TYPE_MAX &&
		fwrt->fw->img[fwrt->cur_fw_img].paging_mem_size &&
		fwrt->fw_paging_db[0].fw_paging_block;
}

void iwl_fw_dbg_read_d3_debug_data(struct iwl_fw_runtime *fwrt);

static inline void iwl_fw_flush_dumps(struct iwl_fw_runtime *fwrt)
{
	int i;

	iwl_dbg_tlv_del_timers(fwrt->trans);
	for (i = 0; i < IWL_FW_RUNTIME_DUMP_WK_NUM; i++)
		flush_delayed_work(&fwrt->dump.wks[i].wk);
>>>>>>> upstream/android-13
}

#ifdef CONFIG_IWLWIFI_DEBUGFS
static inline void iwl_fw_cancel_timestamp(struct iwl_fw_runtime *fwrt)
{
	fwrt->timestamp.delay = 0;
	cancel_delayed_work_sync(&fwrt->timestamp.wk);
}

void iwl_fw_trigger_timestamp(struct iwl_fw_runtime *fwrt, u32 delay);

static inline void iwl_fw_suspend_timestamp(struct iwl_fw_runtime *fwrt)
{
	cancel_delayed_work_sync(&fwrt->timestamp.wk);
}

static inline void iwl_fw_resume_timestamp(struct iwl_fw_runtime *fwrt)
{
	if (!fwrt->timestamp.delay)
		return;

	schedule_delayed_work(&fwrt->timestamp.wk,
			      round_jiffies_relative(fwrt->timestamp.delay));
}

#else

static inline void iwl_fw_cancel_timestamp(struct iwl_fw_runtime *fwrt) {}

static inline void iwl_fw_trigger_timestamp(struct iwl_fw_runtime *fwrt,
					    u32 delay) {}

static inline void iwl_fw_suspend_timestamp(struct iwl_fw_runtime *fwrt) {}

static inline void iwl_fw_resume_timestamp(struct iwl_fw_runtime *fwrt) {}

#endif /* CONFIG_IWLWIFI_DEBUGFS */

<<<<<<< HEAD
=======
void iwl_fw_dbg_stop_sync(struct iwl_fw_runtime *fwrt);

static inline void iwl_fw_lmac1_set_alive_err_table(struct iwl_trans *trans,
						    u32 lmac_error_event_table)
{
	if (!(trans->dbg.error_event_table_tlv_status &
	      IWL_ERROR_EVENT_TABLE_LMAC1) ||
	    WARN_ON(trans->dbg.lmac_error_event_table[0] !=
		    lmac_error_event_table))
		trans->dbg.lmac_error_event_table[0] = lmac_error_event_table;
}

static inline void iwl_fw_umac_set_alive_err_table(struct iwl_trans *trans,
						   u32 umac_error_event_table)
{
	if (!(trans->dbg.error_event_table_tlv_status &
	      IWL_ERROR_EVENT_TABLE_UMAC) ||
	    WARN_ON(trans->dbg.umac_error_event_table !=
		    umac_error_event_table))
		trans->dbg.umac_error_event_table = umac_error_event_table;
}

static inline void iwl_fw_error_collect(struct iwl_fw_runtime *fwrt, bool sync)
{
	enum iwl_fw_ini_time_point tp_id;

	if (!iwl_trans_dbg_ini_valid(fwrt->trans)) {
		iwl_fw_dbg_collect_desc(fwrt, &iwl_dump_desc_assert, false, 0);
		return;
	}

	if (fwrt->trans->dbg.hw_error) {
		tp_id = IWL_FW_INI_TIME_POINT_FW_HW_ERROR;
		fwrt->trans->dbg.hw_error = false;
	} else {
		tp_id = IWL_FW_INI_TIME_POINT_FW_ASSERT;
	}

	_iwl_dbg_tlv_time_point(fwrt, tp_id, NULL, sync);
}

void iwl_fw_error_print_fseq_regs(struct iwl_fw_runtime *fwrt);

static inline void iwl_fwrt_update_fw_versions(struct iwl_fw_runtime *fwrt,
					       struct iwl_lmac_alive *lmac,
					       struct iwl_umac_alive *umac)
{
	if (lmac) {
		fwrt->dump.fw_ver.type = lmac->ver_type;
		fwrt->dump.fw_ver.subtype = lmac->ver_subtype;
		fwrt->dump.fw_ver.lmac_major = le32_to_cpu(lmac->ucode_major);
		fwrt->dump.fw_ver.lmac_minor = le32_to_cpu(lmac->ucode_minor);
	}

	if (umac) {
		fwrt->dump.fw_ver.umac_major = le32_to_cpu(umac->umac_major);
		fwrt->dump.fw_ver.umac_minor = le32_to_cpu(umac->umac_minor);
	}
}

void iwl_fwrt_dump_error_logs(struct iwl_fw_runtime *fwrt);
>>>>>>> upstream/android-13
#endif  /* __iwl_fw_dbg_h__ */
