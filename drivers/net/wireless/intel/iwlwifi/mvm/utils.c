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
 * Copyright (C) 2015 - 2017 Intel Deutschland GmbH
 * Copyright(c) 2018 Intel Corporation
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
 * Copyright (C) 2015 - 2017 Intel Deutschland GmbH
 * Copyright(c) 2018 Intel Corporation
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
 * Copyright (C) 2012-2014, 2018-2021 Intel Corporation
 * Copyright (C) 2013-2014 Intel Mobile Communications GmbH
 * Copyright (C) 2015-2017 Intel Deutschland GmbH
 */
>>>>>>> upstream/android-13
#include <net/mac80211.h>

#include "iwl-debug.h"
#include "iwl-io.h"
#include "iwl-prph.h"
#include "iwl-csr.h"
#include "mvm.h"
#include "fw/api/rs.h"
<<<<<<< HEAD
=======
#include "fw/img.h"
>>>>>>> upstream/android-13

/*
 * Will return 0 even if the cmd failed when RFKILL is asserted unless
 * CMD_WANT_SKB is set in cmd->flags.
 */
int iwl_mvm_send_cmd(struct iwl_mvm *mvm, struct iwl_host_cmd *cmd)
{
	int ret;

#if defined(CONFIG_IWLWIFI_DEBUGFS) && defined(CONFIG_PM_SLEEP)
	if (WARN_ON(mvm->d3_test_active))
		return -EIO;
#endif

	/*
	 * Synchronous commands from this op-mode must hold
	 * the mutex, this ensures we don't try to send two
	 * (or more) synchronous commands at a time.
	 */
<<<<<<< HEAD
	if (!(cmd->flags & CMD_ASYNC)) {
		lockdep_assert_held(&mvm->mutex);
		if (!(cmd->flags & CMD_SEND_IN_IDLE))
			iwl_mvm_ref(mvm, IWL_MVM_REF_SENDING_CMD);
	}

	ret = iwl_trans_send_cmd(mvm->trans, cmd);

	if (!(cmd->flags & (CMD_ASYNC | CMD_SEND_IN_IDLE)))
		iwl_mvm_unref(mvm, IWL_MVM_REF_SENDING_CMD);

=======
	if (!(cmd->flags & CMD_ASYNC))
		lockdep_assert_held(&mvm->mutex);

	ret = iwl_trans_send_cmd(mvm->trans, cmd);

>>>>>>> upstream/android-13
	/*
	 * If the caller wants the SKB, then don't hide any problems, the
	 * caller might access the response buffer which will be NULL if
	 * the command failed.
	 */
	if (cmd->flags & CMD_WANT_SKB)
		return ret;

<<<<<<< HEAD
	/* Silently ignore failures if RFKILL is asserted */
	if (!ret || ret == -ERFKILL)
=======
	/*
	 * Silently ignore failures if RFKILL is asserted or
	 * we are in suspend\resume process
	 */
	if (!ret || ret == -ERFKILL || ret == -EHOSTDOWN)
>>>>>>> upstream/android-13
		return 0;
	return ret;
}

int iwl_mvm_send_cmd_pdu(struct iwl_mvm *mvm, u32 id,
			 u32 flags, u16 len, const void *data)
{
	struct iwl_host_cmd cmd = {
		.id = id,
		.len = { len, },
		.data = { data, },
		.flags = flags,
	};

	return iwl_mvm_send_cmd(mvm, &cmd);
}

/*
 * We assume that the caller set the status to the success value
 */
int iwl_mvm_send_cmd_status(struct iwl_mvm *mvm, struct iwl_host_cmd *cmd,
			    u32 *status)
{
	struct iwl_rx_packet *pkt;
	struct iwl_cmd_response *resp;
	int ret, resp_len;

	lockdep_assert_held(&mvm->mutex);

#if defined(CONFIG_IWLWIFI_DEBUGFS) && defined(CONFIG_PM_SLEEP)
	if (WARN_ON(mvm->d3_test_active))
		return -EIO;
#endif

	/*
	 * Only synchronous commands can wait for status,
	 * we use WANT_SKB so the caller can't.
	 */
	if (WARN_ONCE(cmd->flags & (CMD_ASYNC | CMD_WANT_SKB),
		      "cmd flags %x", cmd->flags))
		return -EINVAL;

	cmd->flags |= CMD_WANT_SKB;

	ret = iwl_trans_send_cmd(mvm->trans, cmd);
	if (ret == -ERFKILL) {
		/*
		 * The command failed because of RFKILL, don't update
		 * the status, leave it as success and return 0.
		 */
		return 0;
	} else if (ret) {
		return ret;
	}

	pkt = cmd->resp_pkt;

	resp_len = iwl_rx_packet_payload_len(pkt);
	if (WARN_ON_ONCE(resp_len != sizeof(*resp))) {
		ret = -EIO;
		goto out_free_resp;
	}

	resp = (void *)pkt->data;
	*status = le32_to_cpu(resp->status);
 out_free_resp:
	iwl_free_resp(cmd);
	return ret;
}

/*
 * We assume that the caller set the status to the sucess value
 */
int iwl_mvm_send_cmd_pdu_status(struct iwl_mvm *mvm, u32 id, u16 len,
				const void *data, u32 *status)
{
	struct iwl_host_cmd cmd = {
		.id = id,
		.len = { len, },
		.data = { data, },
	};

	return iwl_mvm_send_cmd_status(mvm, &cmd, status);
}

#define IWL_DECLARE_RATE_INFO(r) \
	[IWL_RATE_##r##M_INDEX] = IWL_RATE_##r##M_PLCP

/*
 * Translate from fw_rate_index (IWL_RATE_XXM_INDEX) to PLCP
 */
static const u8 fw_rate_idx_to_plcp[IWL_RATE_COUNT] = {
	IWL_DECLARE_RATE_INFO(1),
	IWL_DECLARE_RATE_INFO(2),
	IWL_DECLARE_RATE_INFO(5),
	IWL_DECLARE_RATE_INFO(11),
	IWL_DECLARE_RATE_INFO(6),
	IWL_DECLARE_RATE_INFO(9),
	IWL_DECLARE_RATE_INFO(12),
	IWL_DECLARE_RATE_INFO(18),
	IWL_DECLARE_RATE_INFO(24),
	IWL_DECLARE_RATE_INFO(36),
	IWL_DECLARE_RATE_INFO(48),
	IWL_DECLARE_RATE_INFO(54),
};

int iwl_mvm_legacy_rate_to_mac80211_idx(u32 rate_n_flags,
					enum nl80211_band band)
{
	int rate = rate_n_flags & RATE_LEGACY_RATE_MSK;
	int idx;
	int band_offset = 0;

	/* Legacy rate format, search for match in table */
<<<<<<< HEAD
	if (band == NL80211_BAND_5GHZ)
=======
	if (band != NL80211_BAND_2GHZ)
>>>>>>> upstream/android-13
		band_offset = IWL_FIRST_OFDM_RATE;
	for (idx = band_offset; idx < IWL_RATE_COUNT_LEGACY; idx++)
		if (fw_rate_idx_to_plcp[idx] == rate)
			return idx - band_offset;

	return -1;
}

u8 iwl_mvm_mac80211_idx_to_hwrate(int rate_idx)
{
	/* Get PLCP rate for tx_cmd->rate_n_flags */
	return fw_rate_idx_to_plcp[rate_idx];
}

<<<<<<< HEAD
=======
u8 iwl_mvm_mac80211_ac_to_ucode_ac(enum ieee80211_ac_numbers ac)
{
	static const u8 mac80211_ac_to_ucode_ac[] = {
		AC_VO,
		AC_VI,
		AC_BE,
		AC_BK
	};

	return mac80211_ac_to_ucode_ac[ac];
}

>>>>>>> upstream/android-13
void iwl_mvm_rx_fw_error(struct iwl_mvm *mvm, struct iwl_rx_cmd_buffer *rxb)
{
	struct iwl_rx_packet *pkt = rxb_addr(rxb);
	struct iwl_error_resp *err_resp = (void *)pkt->data;

	IWL_ERR(mvm, "FW Error notification: type 0x%08X cmd_id 0x%02X\n",
		le32_to_cpu(err_resp->error_type), err_resp->cmd_id);
	IWL_ERR(mvm, "FW Error notification: seq 0x%04X service 0x%08X\n",
		le16_to_cpu(err_resp->bad_cmd_seq_num),
		le32_to_cpu(err_resp->error_service));
<<<<<<< HEAD
	IWL_ERR(mvm, "FW Error notification: timestamp 0x%16llX\n",
=======
	IWL_ERR(mvm, "FW Error notification: timestamp 0x%016llX\n",
>>>>>>> upstream/android-13
		le64_to_cpu(err_resp->timestamp));
}

/*
 * Returns the first antenna as ANT_[ABC], as defined in iwl-config.h.
 * The parameter should also be a combination of ANT_[ABC].
 */
u8 first_antenna(u8 mask)
{
	BUILD_BUG_ON(ANT_A != BIT(0)); /* using ffs is wrong if not */
	if (WARN_ON_ONCE(!mask)) /* ffs will return 0 if mask is zeroed */
		return BIT(0);
	return BIT(ffs(mask) - 1);
}

/*
 * Toggles between TX antennas to send the probe request on.
 * Receives the bitmask of valid TX antennas and the *index* used
 * for the last TX, and returns the next valid *index* to use.
 * In order to set it in the tx_cmd, must do BIT(idx).
 */
u8 iwl_mvm_next_antenna(struct iwl_mvm *mvm, u8 valid, u8 last_idx)
{
	u8 ind = last_idx;
	int i;

	for (i = 0; i < MAX_ANT_NUM; i++) {
		ind = (ind + 1) % MAX_ANT_NUM;
		if (valid & BIT(ind))
			return ind;
	}

	WARN_ONCE(1, "Failed to toggle between antennas 0x%x", valid);
	return last_idx;
}

<<<<<<< HEAD
static const struct {
	const char *name;
	u8 num;
} advanced_lookup[] = {
	{ "NMI_INTERRUPT_WDG", 0x34 },
	{ "SYSASSERT", 0x35 },
	{ "UCODE_VERSION_MISMATCH", 0x37 },
	{ "BAD_COMMAND", 0x38 },
	{ "NMI_INTERRUPT_DATA_ACTION_PT", 0x3C },
	{ "FATAL_ERROR", 0x3D },
	{ "NMI_TRM_HW_ERR", 0x46 },
	{ "NMI_INTERRUPT_TRM", 0x4C },
	{ "NMI_INTERRUPT_BREAK_POINT", 0x54 },
	{ "NMI_INTERRUPT_WDG_RXF_FULL", 0x5C },
	{ "NMI_INTERRUPT_WDG_NO_RBD_RXF_FULL", 0x64 },
	{ "NMI_INTERRUPT_HOST", 0x66 },
	{ "NMI_INTERRUPT_ACTION_PT", 0x7C },
	{ "NMI_INTERRUPT_UNKNOWN", 0x84 },
	{ "NMI_INTERRUPT_INST_ACTION_PT", 0x86 },
	{ "ADVANCED_SYSASSERT", 0 },
};

static const char *desc_lookup(u32 num)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(advanced_lookup) - 1; i++)
		if (advanced_lookup[i].num == num)
			return advanced_lookup[i].name;

	/* No entry matches 'num', so it is the last: ADVANCED_SYSASSERT */
	return advanced_lookup[i].name;
}

/*
 * Note: This structure is read from the device with IO accesses,
 * and the reading already does the endian conversion. As it is
 * read with u32-sized accesses, any members with a different size
 * need to be ordered correctly though!
 */
struct iwl_error_event_table_v1 {
	u32 valid;		/* (nonzero) valid, (0) log is empty */
	u32 error_id;		/* type of error */
	u32 pc;			/* program counter */
	u32 blink1;		/* branch link */
	u32 blink2;		/* branch link */
	u32 ilink1;		/* interrupt link */
	u32 ilink2;		/* interrupt link */
	u32 data1;		/* error-specific data */
	u32 data2;		/* error-specific data */
	u32 data3;		/* error-specific data */
	u32 bcon_time;		/* beacon timer */
	u32 tsf_low;		/* network timestamp function timer */
	u32 tsf_hi;		/* network timestamp function timer */
	u32 gp1;		/* GP1 timer register */
	u32 gp2;		/* GP2 timer register */
	u32 gp3;		/* GP3 timer register */
	u32 ucode_ver;		/* uCode version */
	u32 hw_ver;		/* HW Silicon version */
	u32 brd_ver;		/* HW board version */
	u32 log_pc;		/* log program counter */
	u32 frame_ptr;		/* frame pointer */
	u32 stack_ptr;		/* stack pointer */
	u32 hcmd;		/* last host command header */
	u32 isr0;		/* isr status register LMPM_NIC_ISR0:
				 * rxtx_flag */
	u32 isr1;		/* isr status register LMPM_NIC_ISR1:
				 * host_flag */
	u32 isr2;		/* isr status register LMPM_NIC_ISR2:
				 * enc_flag */
	u32 isr3;		/* isr status register LMPM_NIC_ISR3:
				 * time_flag */
	u32 isr4;		/* isr status register LMPM_NIC_ISR4:
				 * wico interrupt */
	u32 isr_pref;		/* isr status register LMPM_NIC_PREF_STAT */
	u32 wait_event;		/* wait event() caller address */
	u32 l2p_control;	/* L2pControlField */
	u32 l2p_duration;	/* L2pDurationField */
	u32 l2p_mhvalid;	/* L2pMhValidBits */
	u32 l2p_addr_match;	/* L2pAddrMatchStat */
	u32 lmpm_pmg_sel;	/* indicate which clocks are turned on
				 * (LMPM_PMG_SEL) */
	u32 u_timestamp;	/* indicate when the date and time of the
				 * compilation */
	u32 flow_handler;	/* FH read/write pointers, RX credit */
} __packed /* LOG_ERROR_TABLE_API_S_VER_1 */;

struct iwl_error_event_table {
	u32 valid;		/* (nonzero) valid, (0) log is empty */
	u32 error_id;		/* type of error */
	u32 trm_hw_status0;	/* TRM HW status */
	u32 trm_hw_status1;	/* TRM HW status */
	u32 blink2;		/* branch link */
	u32 ilink1;		/* interrupt link */
	u32 ilink2;		/* interrupt link */
	u32 data1;		/* error-specific data */
	u32 data2;		/* error-specific data */
	u32 data3;		/* error-specific data */
	u32 bcon_time;		/* beacon timer */
	u32 tsf_low;		/* network timestamp function timer */
	u32 tsf_hi;		/* network timestamp function timer */
	u32 gp1;		/* GP1 timer register */
	u32 gp2;		/* GP2 timer register */
	u32 fw_rev_type;	/* firmware revision type */
	u32 major;		/* uCode version major */
	u32 minor;		/* uCode version minor */
	u32 hw_ver;		/* HW Silicon version */
	u32 brd_ver;		/* HW board version */
	u32 log_pc;		/* log program counter */
	u32 frame_ptr;		/* frame pointer */
	u32 stack_ptr;		/* stack pointer */
	u32 hcmd;		/* last host command header */
	u32 isr0;		/* isr status register LMPM_NIC_ISR0:
				 * rxtx_flag */
	u32 isr1;		/* isr status register LMPM_NIC_ISR1:
				 * host_flag */
	u32 isr2;		/* isr status register LMPM_NIC_ISR2:
				 * enc_flag */
	u32 isr3;		/* isr status register LMPM_NIC_ISR3:
				 * time_flag */
	u32 isr4;		/* isr status register LMPM_NIC_ISR4:
				 * wico interrupt */
	u32 last_cmd_id;	/* last HCMD id handled by the firmware */
	u32 wait_event;		/* wait event() caller address */
	u32 l2p_control;	/* L2pControlField */
	u32 l2p_duration;	/* L2pDurationField */
	u32 l2p_mhvalid;	/* L2pMhValidBits */
	u32 l2p_addr_match;	/* L2pAddrMatchStat */
	u32 lmpm_pmg_sel;	/* indicate which clocks are turned on
				 * (LMPM_PMG_SEL) */
	u32 u_timestamp;	/* indicate when the date and time of the
				 * compilation */
	u32 flow_handler;	/* FH read/write pointers, RX credit */
} __packed /* LOG_ERROR_TABLE_API_S_VER_3 */;

/*
 * UMAC error struct - relevant starting from family 8000 chip.
 * Note: This structure is read from the device with IO accesses,
 * and the reading already does the endian conversion. As it is
 * read with u32-sized accesses, any members with a different size
 * need to be ordered correctly though!
 */
struct iwl_umac_error_event_table {
	u32 valid;		/* (nonzero) valid, (0) log is empty */
	u32 error_id;		/* type of error */
	u32 blink1;		/* branch link */
	u32 blink2;		/* branch link */
	u32 ilink1;		/* interrupt link */
	u32 ilink2;		/* interrupt link */
	u32 data1;		/* error-specific data */
	u32 data2;		/* error-specific data */
	u32 data3;		/* error-specific data */
	u32 umac_major;
	u32 umac_minor;
	u32 frame_pointer;	/* core register 27*/
	u32 stack_pointer;	/* core register 28 */
	u32 cmd_header;		/* latest host cmd sent to UMAC */
	u32 nic_isr_pref;	/* ISR status register */
} __packed;

#define ERROR_START_OFFSET  (1 * sizeof(u32))
#define ERROR_ELEM_SIZE     (7 * sizeof(u32))

static void iwl_mvm_dump_umac_error_log(struct iwl_mvm *mvm)
{
	struct iwl_trans *trans = mvm->trans;
	struct iwl_umac_error_event_table table;

	if (!mvm->support_umac_log)
		return;

	iwl_trans_read_mem_bytes(trans, mvm->umac_error_event_table, &table,
				 sizeof(table));

	if (ERROR_START_OFFSET <= table.valid * ERROR_ELEM_SIZE) {
		IWL_ERR(trans, "Start IWL Error Log Dump:\n");
		IWL_ERR(trans, "Status: 0x%08lX, count: %d\n",
			mvm->status, table.valid);
	}

	IWL_ERR(mvm, "0x%08X | %s\n", table.error_id,
		desc_lookup(table.error_id));
	IWL_ERR(mvm, "0x%08X | umac branchlink1\n", table.blink1);
	IWL_ERR(mvm, "0x%08X | umac branchlink2\n", table.blink2);
	IWL_ERR(mvm, "0x%08X | umac interruptlink1\n", table.ilink1);
	IWL_ERR(mvm, "0x%08X | umac interruptlink2\n", table.ilink2);
	IWL_ERR(mvm, "0x%08X | umac data1\n", table.data1);
	IWL_ERR(mvm, "0x%08X | umac data2\n", table.data2);
	IWL_ERR(mvm, "0x%08X | umac data3\n", table.data3);
	IWL_ERR(mvm, "0x%08X | umac major\n", table.umac_major);
	IWL_ERR(mvm, "0x%08X | umac minor\n", table.umac_minor);
	IWL_ERR(mvm, "0x%08X | frame pointer\n", table.frame_pointer);
	IWL_ERR(mvm, "0x%08X | stack pointer\n", table.stack_pointer);
	IWL_ERR(mvm, "0x%08X | last host cmd\n", table.cmd_header);
	IWL_ERR(mvm, "0x%08X | isr status reg\n", table.nic_isr_pref);
}

static void iwl_mvm_dump_lmac_error_log(struct iwl_mvm *mvm, u32 base)
{
	struct iwl_trans *trans = mvm->trans;
	struct iwl_error_event_table table;
	u32 val;

	if (mvm->fwrt.cur_fw_img == IWL_UCODE_INIT) {
		if (!base)
			base = mvm->fw->init_errlog_ptr;
	} else {
		if (!base)
			base = mvm->fw->inst_errlog_ptr;
	}

	if (base < 0x400000) {
		IWL_ERR(mvm,
			"Not valid error log pointer 0x%08X for %s uCode\n",
			base,
			(mvm->fwrt.cur_fw_img == IWL_UCODE_INIT)
			? "Init" : "RT");
		return;
	}

	/* check if there is a HW error */
	val = iwl_trans_read_mem32(trans, base);
	if (((val & ~0xf) == 0xa5a5a5a0) || ((val & ~0xf) == 0x5a5a5a50)) {
		int err;

		IWL_ERR(trans, "HW error, resetting before reading\n");

		/* reset the device */
		iwl_trans_sw_reset(trans);

		/* set INIT_DONE flag */
		iwl_set_bit(trans, CSR_GP_CNTRL,
			    BIT(trans->cfg->csr->flag_init_done));

		/* and wait for clock stabilization */
		if (trans->cfg->device_family == IWL_DEVICE_FAMILY_8000)
			udelay(2);

		err = iwl_poll_bit(trans, CSR_GP_CNTRL,
				   BIT(trans->cfg->csr->flag_mac_clock_ready),
				   BIT(trans->cfg->csr->flag_mac_clock_ready),
				   25000);
		if (err < 0) {
			IWL_DEBUG_INFO(trans,
				       "Failed to reset the card for the dump\n");
			return;
		}
	}

	iwl_trans_read_mem_bytes(trans, base, &table, sizeof(table));

	if (ERROR_START_OFFSET <= table.valid * ERROR_ELEM_SIZE) {
		IWL_ERR(trans, "Start IWL Error Log Dump:\n");
		IWL_ERR(trans, "Status: 0x%08lX, count: %d\n",
			mvm->status, table.valid);
	}

	/* Do not change this output - scripts rely on it */

	IWL_ERR(mvm, "Loaded firmware version: %s\n", mvm->fw->fw_version);

	trace_iwlwifi_dev_ucode_error(trans->dev, &table, table.hw_ver, table.brd_ver);
	IWL_ERR(mvm, "0x%08X | %-28s\n", table.error_id,
		desc_lookup(table.error_id));
	IWL_ERR(mvm, "0x%08X | trm_hw_status0\n", table.trm_hw_status0);
	IWL_ERR(mvm, "0x%08X | trm_hw_status1\n", table.trm_hw_status1);
	IWL_ERR(mvm, "0x%08X | branchlink2\n", table.blink2);
	IWL_ERR(mvm, "0x%08X | interruptlink1\n", table.ilink1);
	IWL_ERR(mvm, "0x%08X | interruptlink2\n", table.ilink2);
	IWL_ERR(mvm, "0x%08X | data1\n", table.data1);
	IWL_ERR(mvm, "0x%08X | data2\n", table.data2);
	IWL_ERR(mvm, "0x%08X | data3\n", table.data3);
	IWL_ERR(mvm, "0x%08X | beacon time\n", table.bcon_time);
	IWL_ERR(mvm, "0x%08X | tsf low\n", table.tsf_low);
	IWL_ERR(mvm, "0x%08X | tsf hi\n", table.tsf_hi);
	IWL_ERR(mvm, "0x%08X | time gp1\n", table.gp1);
	IWL_ERR(mvm, "0x%08X | time gp2\n", table.gp2);
	IWL_ERR(mvm, "0x%08X | uCode revision type\n", table.fw_rev_type);
	IWL_ERR(mvm, "0x%08X | uCode version major\n", table.major);
	IWL_ERR(mvm, "0x%08X | uCode version minor\n", table.minor);
	IWL_ERR(mvm, "0x%08X | hw version\n", table.hw_ver);
	IWL_ERR(mvm, "0x%08X | board version\n", table.brd_ver);
	IWL_ERR(mvm, "0x%08X | hcmd\n", table.hcmd);
	IWL_ERR(mvm, "0x%08X | isr0\n", table.isr0);
	IWL_ERR(mvm, "0x%08X | isr1\n", table.isr1);
	IWL_ERR(mvm, "0x%08X | isr2\n", table.isr2);
	IWL_ERR(mvm, "0x%08X | isr3\n", table.isr3);
	IWL_ERR(mvm, "0x%08X | isr4\n", table.isr4);
	IWL_ERR(mvm, "0x%08X | last cmd Id\n", table.last_cmd_id);
	IWL_ERR(mvm, "0x%08X | wait_event\n", table.wait_event);
	IWL_ERR(mvm, "0x%08X | l2p_control\n", table.l2p_control);
	IWL_ERR(mvm, "0x%08X | l2p_duration\n", table.l2p_duration);
	IWL_ERR(mvm, "0x%08X | l2p_mhvalid\n", table.l2p_mhvalid);
	IWL_ERR(mvm, "0x%08X | l2p_addr_match\n", table.l2p_addr_match);
	IWL_ERR(mvm, "0x%08X | lmpm_pmg_sel\n", table.lmpm_pmg_sel);
	IWL_ERR(mvm, "0x%08X | timestamp\n", table.u_timestamp);
	IWL_ERR(mvm, "0x%08X | flow_handler\n", table.flow_handler);
}

void iwl_mvm_dump_nic_error_log(struct iwl_mvm *mvm)
{
	if (!test_bit(STATUS_DEVICE_ENABLED, &mvm->trans->status)) {
		IWL_ERR(mvm,
			"DEVICE_ENABLED bit is not set. Aborting dump.\n");
		return;
	}

	iwl_mvm_dump_lmac_error_log(mvm, mvm->error_event_table[0]);

	if (mvm->error_event_table[1])
		iwl_mvm_dump_lmac_error_log(mvm, mvm->error_event_table[1]);

	iwl_mvm_dump_umac_error_log(mvm);
}

int iwl_mvm_find_free_queue(struct iwl_mvm *mvm, u8 sta_id, u8 minq, u8 maxq)
{
	int i;

	lockdep_assert_held(&mvm->queue_info_lock);

	/* This should not be hit with new TX path */
	if (WARN_ON(iwl_mvm_has_new_tx_api(mvm)))
		return -ENOSPC;

	/* Start by looking for a free queue */
	for (i = minq; i <= maxq; i++)
		if (mvm->queue_info[i].hw_queue_refcount == 0 &&
		    mvm->queue_info[i].status == IWL_MVM_QUEUE_FREE)
			return i;

	/*
	 * If no free queue found - settle for an inactive one to reconfigure
	 * Make sure that the inactive queue either already belongs to this STA,
	 * or that if it belongs to another one - it isn't the reserved queue
	 */
	for (i = minq; i <= maxq; i++)
		if (mvm->queue_info[i].status == IWL_MVM_QUEUE_INACTIVE &&
		    (sta_id == mvm->queue_info[i].ra_sta_id ||
		     !mvm->queue_info[i].reserved))
			return i;

	return -ENOSPC;
}

=======
>>>>>>> upstream/android-13
int iwl_mvm_reconfig_scd(struct iwl_mvm *mvm, int queue, int fifo, int sta_id,
			 int tid, int frame_limit, u16 ssn)
{
	struct iwl_scd_txq_cfg_cmd cmd = {
		.scd_queue = queue,
		.action = SCD_CFG_ENABLE_QUEUE,
		.window = frame_limit,
		.sta_id = sta_id,
		.ssn = cpu_to_le16(ssn),
		.tx_fifo = fifo,
		.aggregate = (queue >= IWL_MVM_DQA_MIN_DATA_QUEUE ||
			      queue == IWL_MVM_DQA_BSS_CLIENT_QUEUE),
		.tid = tid,
	};
	int ret;

	if (WARN_ON(iwl_mvm_has_new_tx_api(mvm)))
		return -EINVAL;

<<<<<<< HEAD
	spin_lock_bh(&mvm->queue_info_lock);
	if (WARN(mvm->queue_info[queue].hw_queue_refcount == 0,
		 "Trying to reconfig unallocated queue %d\n", queue)) {
		spin_unlock_bh(&mvm->queue_info_lock);
		return -ENXIO;
	}
	spin_unlock_bh(&mvm->queue_info_lock);
=======
	if (WARN(mvm->queue_info[queue].tid_bitmap == 0,
		 "Trying to reconfig unallocated queue %d\n", queue))
		return -ENXIO;
>>>>>>> upstream/android-13

	IWL_DEBUG_TX_QUEUES(mvm, "Reconfig SCD for TXQ #%d\n", queue);

	ret = iwl_mvm_send_cmd_pdu(mvm, SCD_QUEUE_CFG, 0, sizeof(cmd), &cmd);
	WARN_ONCE(ret, "Failed to re-configure queue %d on FIFO %d, ret=%d\n",
		  queue, fifo, ret);

	return ret;
}

<<<<<<< HEAD
static bool iwl_mvm_update_txq_mapping(struct iwl_mvm *mvm, int queue,
				       int mac80211_queue, u8 sta_id, u8 tid)
{
	bool enable_queue = true;

	spin_lock_bh(&mvm->queue_info_lock);

	/* Make sure this TID isn't already enabled */
	if (mvm->queue_info[queue].tid_bitmap & BIT(tid)) {
		spin_unlock_bh(&mvm->queue_info_lock);
		IWL_ERR(mvm, "Trying to enable TXQ %d with existing TID %d\n",
			queue, tid);
		return false;
	}

	/* Update mappings and refcounts */
	if (mvm->queue_info[queue].hw_queue_refcount > 0)
		enable_queue = false;

	if (mac80211_queue != IEEE80211_INVAL_HW_QUEUE) {
		WARN(mac80211_queue >=
		     BITS_PER_BYTE * sizeof(mvm->hw_queue_to_mac80211[0]),
		     "cannot track mac80211 queue %d (queue %d, sta %d, tid %d)\n",
		     mac80211_queue, queue, sta_id, tid);
		mvm->hw_queue_to_mac80211[queue] |= BIT(mac80211_queue);
	}

	mvm->queue_info[queue].hw_queue_refcount++;
	mvm->queue_info[queue].tid_bitmap |= BIT(tid);
	mvm->queue_info[queue].ra_sta_id = sta_id;

	if (enable_queue) {
		if (tid != IWL_MAX_TID_COUNT)
			mvm->queue_info[queue].mac80211_ac =
				tid_to_mac80211_ac[tid];
		else
			mvm->queue_info[queue].mac80211_ac = IEEE80211_AC_VO;

		mvm->queue_info[queue].txq_tid = tid;
	}

	IWL_DEBUG_TX_QUEUES(mvm,
			    "Enabling TXQ #%d refcount=%d (mac80211 map:0x%x)\n",
			    queue, mvm->queue_info[queue].hw_queue_refcount,
			    mvm->hw_queue_to_mac80211[queue]);

	spin_unlock_bh(&mvm->queue_info_lock);

	return enable_queue;
}

int iwl_mvm_tvqm_enable_txq(struct iwl_mvm *mvm, int mac80211_queue,
			    u8 sta_id, u8 tid, unsigned int timeout)
{
	struct iwl_tx_queue_cfg_cmd cmd = {
		.flags = cpu_to_le16(TX_QUEUE_CFG_ENABLE_QUEUE),
		.sta_id = sta_id,
		.tid = tid,
	};
	int queue, size = IWL_DEFAULT_QUEUE_SIZE;

	if (cmd.tid == IWL_MAX_TID_COUNT) {
		cmd.tid = IWL_MGMT_TID;
		size = IWL_MGMT_QUEUE_SIZE;
	}
	queue = iwl_trans_txq_alloc(mvm->trans, (void *)&cmd,
				    SCD_QUEUE_CFG, size, timeout);

	if (queue < 0) {
		IWL_DEBUG_TX_QUEUES(mvm,
				    "Failed allocating TXQ for sta %d tid %d, ret: %d\n",
				    sta_id, tid, queue);
		return queue;
	}

	IWL_DEBUG_TX_QUEUES(mvm, "Enabling TXQ #%d for sta %d tid %d\n",
			    queue, sta_id, tid);

	mvm->hw_queue_to_mac80211[queue] |= BIT(mac80211_queue);
	IWL_DEBUG_TX_QUEUES(mvm,
			    "Enabling TXQ #%d (mac80211 map:0x%x)\n",
			    queue, mvm->hw_queue_to_mac80211[queue]);

	return queue;
}

bool iwl_mvm_enable_txq(struct iwl_mvm *mvm, int queue, int mac80211_queue,
			u16 ssn, const struct iwl_trans_txq_scd_cfg *cfg,
			unsigned int wdg_timeout)
{
	struct iwl_scd_txq_cfg_cmd cmd = {
		.scd_queue = queue,
		.action = SCD_CFG_ENABLE_QUEUE,
		.window = cfg->frame_limit,
		.sta_id = cfg->sta_id,
		.ssn = cpu_to_le16(ssn),
		.tx_fifo = cfg->fifo,
		.aggregate = cfg->aggregate,
		.tid = cfg->tid,
	};
	bool inc_ssn;

	if (WARN_ON(iwl_mvm_has_new_tx_api(mvm)))
		return false;

	/* Send the enabling command if we need to */
	if (!iwl_mvm_update_txq_mapping(mvm, queue, mac80211_queue,
					cfg->sta_id, cfg->tid))
		return false;

	inc_ssn = iwl_trans_txq_enable_cfg(mvm->trans, queue, ssn,
					   NULL, wdg_timeout);
	if (inc_ssn)
		le16_add_cpu(&cmd.ssn, 1);

	WARN(iwl_mvm_send_cmd_pdu(mvm, SCD_QUEUE_CFG, 0, sizeof(cmd), &cmd),
	     "Failed to configure queue %d on FIFO %d\n", queue, cfg->fifo);

	return inc_ssn;
}

int iwl_mvm_disable_txq(struct iwl_mvm *mvm, int queue, int mac80211_queue,
			u8 tid, u8 flags)
{
	struct iwl_scd_txq_cfg_cmd cmd = {
		.scd_queue = queue,
		.action = SCD_CFG_DISABLE_QUEUE,
	};
	bool remove_mac_queue = mac80211_queue != IEEE80211_INVAL_HW_QUEUE;
	int ret;

	if (WARN_ON(remove_mac_queue && mac80211_queue >= IEEE80211_MAX_QUEUES))
		return -EINVAL;

	if (iwl_mvm_has_new_tx_api(mvm)) {
		spin_lock_bh(&mvm->queue_info_lock);

		if (remove_mac_queue)
			mvm->hw_queue_to_mac80211[queue] &=
				~BIT(mac80211_queue);

		spin_unlock_bh(&mvm->queue_info_lock);

		iwl_trans_txq_free(mvm->trans, queue);

		return 0;
	}

	spin_lock_bh(&mvm->queue_info_lock);

	if (WARN_ON(mvm->queue_info[queue].hw_queue_refcount == 0)) {
		spin_unlock_bh(&mvm->queue_info_lock);
		return 0;
	}

	mvm->queue_info[queue].tid_bitmap &= ~BIT(tid);

	/*
	 * If there is another TID with the same AC - don't remove the MAC queue
	 * from the mapping
	 */
	if (tid < IWL_MAX_TID_COUNT) {
		unsigned long tid_bitmap =
			mvm->queue_info[queue].tid_bitmap;
		int ac = tid_to_mac80211_ac[tid];
		int i;

		for_each_set_bit(i, &tid_bitmap, IWL_MAX_TID_COUNT) {
			if (tid_to_mac80211_ac[i] == ac)
				remove_mac_queue = false;
		}
	}

	if (remove_mac_queue)
		mvm->hw_queue_to_mac80211[queue] &=
			~BIT(mac80211_queue);
	mvm->queue_info[queue].hw_queue_refcount--;

	cmd.action = mvm->queue_info[queue].hw_queue_refcount ?
		SCD_CFG_ENABLE_QUEUE : SCD_CFG_DISABLE_QUEUE;
	if (cmd.action == SCD_CFG_DISABLE_QUEUE)
		mvm->queue_info[queue].status = IWL_MVM_QUEUE_FREE;

	IWL_DEBUG_TX_QUEUES(mvm,
			    "Disabling TXQ #%d refcount=%d (mac80211 map:0x%x)\n",
			    queue,
			    mvm->queue_info[queue].hw_queue_refcount,
			    mvm->hw_queue_to_mac80211[queue]);

	/* If the queue is still enabled - nothing left to do in this func */
	if (cmd.action == SCD_CFG_ENABLE_QUEUE) {
		spin_unlock_bh(&mvm->queue_info_lock);
		return 0;
	}

	cmd.sta_id = mvm->queue_info[queue].ra_sta_id;
	cmd.tid = mvm->queue_info[queue].txq_tid;

	/* Make sure queue info is correct even though we overwrite it */
	WARN(mvm->queue_info[queue].hw_queue_refcount ||
	     mvm->queue_info[queue].tid_bitmap ||
	     mvm->hw_queue_to_mac80211[queue],
	     "TXQ #%d info out-of-sync - refcount=%d, mac map=0x%x, tid=0x%x\n",
	     queue, mvm->queue_info[queue].hw_queue_refcount,
	     mvm->hw_queue_to_mac80211[queue],
	     mvm->queue_info[queue].tid_bitmap);

	/* If we are here - the queue is freed and we can zero out these vals */
	mvm->queue_info[queue].hw_queue_refcount = 0;
	mvm->queue_info[queue].tid_bitmap = 0;
	mvm->hw_queue_to_mac80211[queue] = 0;

	/* Regardless if this is a reserved TXQ for a STA - mark it as false */
	mvm->queue_info[queue].reserved = false;

	spin_unlock_bh(&mvm->queue_info_lock);

	iwl_trans_txq_disable(mvm->trans, queue, false);
	ret = iwl_mvm_send_cmd_pdu(mvm, SCD_QUEUE_CFG, flags,
				   sizeof(struct iwl_scd_txq_cfg_cmd), &cmd);

	if (ret)
		IWL_ERR(mvm, "Failed to disable queue %d (ret=%d)\n",
			queue, ret);
	return ret;
}

/**
 * iwl_mvm_send_lq_cmd() - Send link quality command
 * @sync: This command can be sent synchronously.
=======
/**
 * iwl_mvm_send_lq_cmd() - Send link quality command
 * @mvm: Driver data.
 * @lq: Link quality command to send.
>>>>>>> upstream/android-13
 *
 * The link quality command is sent as the last step of station creation.
 * This is the special case in which init is set and we call a callback in
 * this case to clear the state indicating that station creation is in
 * progress.
 */
<<<<<<< HEAD
int iwl_mvm_send_lq_cmd(struct iwl_mvm *mvm, struct iwl_lq_cmd *lq, bool sync)
=======
int iwl_mvm_send_lq_cmd(struct iwl_mvm *mvm, struct iwl_lq_cmd *lq)
>>>>>>> upstream/android-13
{
	struct iwl_host_cmd cmd = {
		.id = LQ_CMD,
		.len = { sizeof(struct iwl_lq_cmd), },
<<<<<<< HEAD
		.flags = sync ? 0 : CMD_ASYNC,
=======
		.flags = CMD_ASYNC,
>>>>>>> upstream/android-13
		.data = { lq, },
	};

	if (WARN_ON(lq->sta_id == IWL_MVM_INVALID_STA ||
		    iwl_mvm_has_tlc_offload(mvm)))
		return -EINVAL;

	return iwl_mvm_send_cmd(mvm, &cmd);
}

/**
 * iwl_mvm_update_smps - Get a request to change the SMPS mode
<<<<<<< HEAD
 * @req_type: The part of the driver who call for a change.
 * @smps_requests: The request to change the SMPS mode.
=======
 * @mvm: Driver data.
 * @vif: Pointer to the ieee80211_vif structure
 * @req_type: The part of the driver who call for a change.
 * @smps_request: The request to change the SMPS mode.
>>>>>>> upstream/android-13
 *
 * Get a requst to change the SMPS mode,
 * and change it according to all other requests in the driver.
 */
void iwl_mvm_update_smps(struct iwl_mvm *mvm, struct ieee80211_vif *vif,
			 enum iwl_mvm_smps_type_request req_type,
			 enum ieee80211_smps_mode smps_request)
{
	struct iwl_mvm_vif *mvmvif;
<<<<<<< HEAD
	enum ieee80211_smps_mode smps_mode;
=======
	enum ieee80211_smps_mode smps_mode = IEEE80211_SMPS_AUTOMATIC;
>>>>>>> upstream/android-13
	int i;

	lockdep_assert_held(&mvm->mutex);

	/* SMPS is irrelevant for NICs that don't have at least 2 RX antenna */
	if (num_of_ant(iwl_mvm_get_valid_rx_ant(mvm)) == 1)
		return;

<<<<<<< HEAD
	if (vif->type == NL80211_IFTYPE_AP)
		smps_mode = IEEE80211_SMPS_OFF;
	else
		smps_mode = IEEE80211_SMPS_AUTOMATIC;
=======
	if (vif->type != NL80211_IFTYPE_STATION)
		return;
>>>>>>> upstream/android-13

	mvmvif = iwl_mvm_vif_from_mac80211(vif);
	mvmvif->smps_requests[req_type] = smps_request;
	for (i = 0; i < NUM_IWL_MVM_SMPS_REQ; i++) {
		if (mvmvif->smps_requests[i] == IEEE80211_SMPS_STATIC) {
			smps_mode = IEEE80211_SMPS_STATIC;
			break;
		}
		if (mvmvif->smps_requests[i] == IEEE80211_SMPS_DYNAMIC)
			smps_mode = IEEE80211_SMPS_DYNAMIC;
	}

	ieee80211_request_smps(vif, smps_mode);
}

int iwl_mvm_request_statistics(struct iwl_mvm *mvm, bool clear)
{
	struct iwl_statistics_cmd scmd = {
		.flags = clear ? cpu_to_le32(IWL_STATISTICS_FLG_CLEAR) : 0,
	};
	struct iwl_host_cmd cmd = {
		.id = STATISTICS_CMD,
		.len[0] = sizeof(scmd),
		.data[0] = &scmd,
		.flags = CMD_WANT_SKB,
	};
	int ret;

	ret = iwl_mvm_send_cmd(mvm, &cmd);
	if (ret)
		return ret;

	iwl_mvm_handle_rx_statistics(mvm, cmd.resp_pkt);
	iwl_free_resp(&cmd);

	if (clear)
		iwl_mvm_accu_radio_stats(mvm);

	return 0;
}

void iwl_mvm_accu_radio_stats(struct iwl_mvm *mvm)
{
	mvm->accu_radio_stats.rx_time += mvm->radio_stats.rx_time;
	mvm->accu_radio_stats.tx_time += mvm->radio_stats.tx_time;
	mvm->accu_radio_stats.on_time_rf += mvm->radio_stats.on_time_rf;
	mvm->accu_radio_stats.on_time_scan += mvm->radio_stats.on_time_scan;
}

<<<<<<< HEAD
=======
struct iwl_mvm_diversity_iter_data {
	struct iwl_mvm_phy_ctxt *ctxt;
	bool result;
};

>>>>>>> upstream/android-13
static void iwl_mvm_diversity_iter(void *_data, u8 *mac,
				   struct ieee80211_vif *vif)
{
	struct iwl_mvm_vif *mvmvif = iwl_mvm_vif_from_mac80211(vif);
<<<<<<< HEAD
	bool *result = _data;
	int i;

	for (i = 0; i < NUM_IWL_MVM_SMPS_REQ; i++) {
		if (mvmvif->smps_requests[i] == IEEE80211_SMPS_STATIC ||
		    mvmvif->smps_requests[i] == IEEE80211_SMPS_DYNAMIC)
			*result = false;
	}
}

bool iwl_mvm_rx_diversity_allowed(struct iwl_mvm *mvm)
{
	bool result = true;

	lockdep_assert_held(&mvm->mutex);

=======
	struct iwl_mvm_diversity_iter_data *data = _data;
	int i;

	if (mvmvif->phy_ctxt != data->ctxt)
		return;

	for (i = 0; i < NUM_IWL_MVM_SMPS_REQ; i++) {
		if (mvmvif->smps_requests[i] == IEEE80211_SMPS_STATIC ||
		    mvmvif->smps_requests[i] == IEEE80211_SMPS_DYNAMIC) {
			data->result = false;
			break;
		}
	}
}

bool iwl_mvm_rx_diversity_allowed(struct iwl_mvm *mvm,
				  struct iwl_mvm_phy_ctxt *ctxt)
{
	struct iwl_mvm_diversity_iter_data data = {
		.ctxt = ctxt,
		.result = true,
	};

	lockdep_assert_held(&mvm->mutex);

	if (iwlmvm_mod_params.power_scheme != IWL_POWER_SCHEME_CAM)
		return false;

>>>>>>> upstream/android-13
	if (num_of_ant(iwl_mvm_get_valid_rx_ant(mvm)) == 1)
		return false;

	if (mvm->cfg->rx_with_siso_diversity)
		return false;

	ieee80211_iterate_active_interfaces_atomic(
			mvm->hw, IEEE80211_IFACE_ITER_NORMAL,
<<<<<<< HEAD
			iwl_mvm_diversity_iter, &result);

	return result;
=======
			iwl_mvm_diversity_iter, &data);

	return data.result;
}

void iwl_mvm_send_low_latency_cmd(struct iwl_mvm *mvm,
				  bool low_latency, u16 mac_id)
{
	struct iwl_mac_low_latency_cmd cmd = {
		.mac_id = cpu_to_le32(mac_id)
	};

	if (!fw_has_capa(&mvm->fw->ucode_capa,
			 IWL_UCODE_TLV_CAPA_DYNAMIC_QUOTA))
		return;

	if (low_latency) {
		/* currently we don't care about the direction */
		cmd.low_latency_rx = 1;
		cmd.low_latency_tx = 1;
	}

	if (iwl_mvm_send_cmd_pdu(mvm, iwl_cmd_id(LOW_LATENCY_CMD,
						 MAC_CONF_GROUP, 0),
				 0, sizeof(cmd), &cmd))
		IWL_ERR(mvm, "Failed to send low latency command\n");
>>>>>>> upstream/android-13
}

int iwl_mvm_update_low_latency(struct iwl_mvm *mvm, struct ieee80211_vif *vif,
			       bool low_latency,
			       enum iwl_mvm_low_latency_cause cause)
{
	struct iwl_mvm_vif *mvmvif = iwl_mvm_vif_from_mac80211(vif);
	int res;
	bool prev;

	lockdep_assert_held(&mvm->mutex);

	prev = iwl_mvm_vif_low_latency(mvmvif);
	iwl_mvm_vif_set_low_latency(mvmvif, low_latency, cause);

	low_latency = iwl_mvm_vif_low_latency(mvmvif);

	if (low_latency == prev)
		return 0;

<<<<<<< HEAD
	if (fw_has_capa(&mvm->fw->ucode_capa,
			IWL_UCODE_TLV_CAPA_DYNAMIC_QUOTA)) {
		struct iwl_mac_low_latency_cmd cmd = {
			.mac_id = cpu_to_le32(mvmvif->id)
		};

		if (low_latency) {
			/* currently we don't care about the direction */
			cmd.low_latency_rx = 1;
			cmd.low_latency_tx = 1;
		}
		res = iwl_mvm_send_cmd_pdu(mvm,
					   iwl_cmd_id(LOW_LATENCY_CMD,
						      MAC_CONF_GROUP, 0),
					   0, sizeof(cmd), &cmd);
		if (res)
			IWL_ERR(mvm, "Failed to send low latency command\n");
	}
=======
	iwl_mvm_send_low_latency_cmd(mvm, low_latency, mvmvif->id);
>>>>>>> upstream/android-13

	res = iwl_mvm_update_quotas(mvm, false, NULL);
	if (res)
		return res;

	iwl_mvm_bt_coex_vif_change(mvm);

	return iwl_mvm_power_update_mac(mvm);
}

struct iwl_mvm_low_latency_iter {
	bool result;
	bool result_per_band[NUM_NL80211_BANDS];
};

static void iwl_mvm_ll_iter(void *_data, u8 *mac, struct ieee80211_vif *vif)
{
	struct iwl_mvm_low_latency_iter *result = _data;
	struct iwl_mvm_vif *mvmvif = iwl_mvm_vif_from_mac80211(vif);
	enum nl80211_band band;

	if (iwl_mvm_vif_low_latency(mvmvif)) {
		result->result = true;

		if (!mvmvif->phy_ctxt)
			return;

		band = mvmvif->phy_ctxt->channel->band;
		result->result_per_band[band] = true;
	}
}

bool iwl_mvm_low_latency(struct iwl_mvm *mvm)
{
	struct iwl_mvm_low_latency_iter data = {};

	ieee80211_iterate_active_interfaces_atomic(
			mvm->hw, IEEE80211_IFACE_ITER_NORMAL,
			iwl_mvm_ll_iter, &data);

	return data.result;
}

bool iwl_mvm_low_latency_band(struct iwl_mvm *mvm, enum nl80211_band band)
{
	struct iwl_mvm_low_latency_iter data = {};

	ieee80211_iterate_active_interfaces_atomic(
			mvm->hw, IEEE80211_IFACE_ITER_NORMAL,
			iwl_mvm_ll_iter, &data);

	return data.result_per_band[band];
}

struct iwl_bss_iter_data {
	struct ieee80211_vif *vif;
	bool error;
};

static void iwl_mvm_bss_iface_iterator(void *_data, u8 *mac,
				       struct ieee80211_vif *vif)
{
	struct iwl_bss_iter_data *data = _data;

	if (vif->type != NL80211_IFTYPE_STATION || vif->p2p)
		return;

	if (data->vif) {
		data->error = true;
		return;
	}

	data->vif = vif;
}

struct ieee80211_vif *iwl_mvm_get_bss_vif(struct iwl_mvm *mvm)
{
	struct iwl_bss_iter_data bss_iter_data = {};

	ieee80211_iterate_active_interfaces_atomic(
		mvm->hw, IEEE80211_IFACE_ITER_NORMAL,
		iwl_mvm_bss_iface_iterator, &bss_iter_data);

	if (bss_iter_data.error) {
		IWL_ERR(mvm, "More than one managed interface active!\n");
		return ERR_PTR(-EINVAL);
	}

	return bss_iter_data.vif;
}

<<<<<<< HEAD
=======
struct iwl_bss_find_iter_data {
	struct ieee80211_vif *vif;
	u32 macid;
};

static void iwl_mvm_bss_find_iface_iterator(void *_data, u8 *mac,
					    struct ieee80211_vif *vif)
{
	struct iwl_bss_find_iter_data *data = _data;
	struct iwl_mvm_vif *mvmvif = iwl_mvm_vif_from_mac80211(vif);

	if (mvmvif->id == data->macid)
		data->vif = vif;
}

struct ieee80211_vif *iwl_mvm_get_vif_by_macid(struct iwl_mvm *mvm, u32 macid)
{
	struct iwl_bss_find_iter_data data = {
		.macid = macid,
	};

	lockdep_assert_held(&mvm->mutex);

	ieee80211_iterate_active_interfaces_atomic(
		mvm->hw, IEEE80211_IFACE_ITER_NORMAL,
		iwl_mvm_bss_find_iface_iterator, &data);

	return data.vif;
}

>>>>>>> upstream/android-13
struct iwl_sta_iter_data {
	bool assoc;
};

static void iwl_mvm_sta_iface_iterator(void *_data, u8 *mac,
				       struct ieee80211_vif *vif)
{
	struct iwl_sta_iter_data *data = _data;

	if (vif->type != NL80211_IFTYPE_STATION)
		return;

	if (vif->bss_conf.assoc)
		data->assoc = true;
}

bool iwl_mvm_is_vif_assoc(struct iwl_mvm *mvm)
{
	struct iwl_sta_iter_data data = {
		.assoc = false,
	};

	ieee80211_iterate_active_interfaces_atomic(mvm->hw,
						   IEEE80211_IFACE_ITER_NORMAL,
						   iwl_mvm_sta_iface_iterator,
						   &data);
	return data.assoc;
}

unsigned int iwl_mvm_get_wd_timeout(struct iwl_mvm *mvm,
				    struct ieee80211_vif *vif,
				    bool tdls, bool cmd_q)
{
	struct iwl_fw_dbg_trigger_tlv *trigger;
	struct iwl_fw_dbg_trigger_txq_timer *txq_timer;
<<<<<<< HEAD
	unsigned int default_timeout =
		cmd_q ? IWL_DEF_WD_TIMEOUT : mvm->cfg->base_params->wd_timeout;
=======
	unsigned int default_timeout = cmd_q ?
		IWL_DEF_WD_TIMEOUT :
		mvm->trans->trans_cfg->base_params->wd_timeout;
>>>>>>> upstream/android-13

	if (!iwl_fw_dbg_trigger_enabled(mvm->fw, FW_DBG_TRIGGER_TXQ_TIMERS)) {
		/*
		 * We can't know when the station is asleep or awake, so we
		 * must disable the queue hang detection.
		 */
		if (fw_has_capa(&mvm->fw->ucode_capa,
				IWL_UCODE_TLV_CAPA_STA_PM_NOTIF) &&
		    vif && vif->type == NL80211_IFTYPE_AP)
			return IWL_WATCHDOG_DISABLED;
<<<<<<< HEAD
		return iwlmvm_mod_params.tfd_q_hang_detect ?
			default_timeout : IWL_WATCHDOG_DISABLED;
=======
		return default_timeout;
>>>>>>> upstream/android-13
	}

	trigger = iwl_fw_dbg_get_trigger(mvm->fw, FW_DBG_TRIGGER_TXQ_TIMERS);
	txq_timer = (void *)trigger->data;

	if (tdls)
		return le32_to_cpu(txq_timer->tdls);

	if (cmd_q)
		return le32_to_cpu(txq_timer->command_queue);

	if (WARN_ON(!vif))
		return default_timeout;

	switch (ieee80211_vif_type_p2p(vif)) {
	case NL80211_IFTYPE_ADHOC:
		return le32_to_cpu(txq_timer->ibss);
	case NL80211_IFTYPE_STATION:
		return le32_to_cpu(txq_timer->bss);
	case NL80211_IFTYPE_AP:
		return le32_to_cpu(txq_timer->softap);
	case NL80211_IFTYPE_P2P_CLIENT:
		return le32_to_cpu(txq_timer->p2p_client);
	case NL80211_IFTYPE_P2P_GO:
		return le32_to_cpu(txq_timer->p2p_go);
	case NL80211_IFTYPE_P2P_DEVICE:
		return le32_to_cpu(txq_timer->p2p_device);
	case NL80211_IFTYPE_MONITOR:
		return default_timeout;
	default:
		WARN_ON(1);
<<<<<<< HEAD
		return mvm->cfg->base_params->wd_timeout;
=======
		return mvm->trans->trans_cfg->base_params->wd_timeout;
>>>>>>> upstream/android-13
	}
}

void iwl_mvm_connection_loss(struct iwl_mvm *mvm, struct ieee80211_vif *vif,
			     const char *errmsg)
{
	struct iwl_fw_dbg_trigger_tlv *trig;
	struct iwl_fw_dbg_trigger_mlme *trig_mlme;

<<<<<<< HEAD
	if (!iwl_fw_dbg_trigger_enabled(mvm->fw, FW_DBG_TRIGGER_MLME))
		goto out;

	trig = iwl_fw_dbg_get_trigger(mvm->fw, FW_DBG_TRIGGER_MLME);
	trig_mlme = (void *)trig->data;
	if (!iwl_fw_dbg_trigger_check_stop(&mvm->fwrt,
					   ieee80211_vif_to_wdev(vif), trig))
		goto out;
=======
	trig = iwl_fw_dbg_trigger_on(&mvm->fwrt, ieee80211_vif_to_wdev(vif),
				     FW_DBG_TRIGGER_MLME);
	if (!trig)
		goto out;

	trig_mlme = (void *)trig->data;
>>>>>>> upstream/android-13

	if (trig_mlme->stop_connection_loss &&
	    --trig_mlme->stop_connection_loss)
		goto out;

	iwl_fw_dbg_collect_trig(&mvm->fwrt, trig, "%s", errmsg);

out:
	ieee80211_connection_loss(vif);
}

<<<<<<< HEAD
/*
 * Remove inactive TIDs of a given queue.
 * If all queue TIDs are inactive - mark the queue as inactive
 * If only some the queue TIDs are inactive - unmap them from the queue
 */
static void iwl_mvm_remove_inactive_tids(struct iwl_mvm *mvm,
					 struct iwl_mvm_sta *mvmsta, int queue,
					 unsigned long tid_bitmap)
{
	int tid;

	lockdep_assert_held(&mvmsta->lock);
	lockdep_assert_held(&mvm->queue_info_lock);

	if (WARN_ON(iwl_mvm_has_new_tx_api(mvm)))
		return;

	/* Go over all non-active TIDs, incl. IWL_MAX_TID_COUNT (for mgmt) */
	for_each_set_bit(tid, &tid_bitmap, IWL_MAX_TID_COUNT + 1) {
		/* If some TFDs are still queued - don't mark TID as inactive */
		if (iwl_mvm_tid_queued(mvm, &mvmsta->tid_data[tid]))
			tid_bitmap &= ~BIT(tid);

		/* Don't mark as inactive any TID that has an active BA */
		if (mvmsta->tid_data[tid].state != IWL_AGG_OFF)
			tid_bitmap &= ~BIT(tid);
	}

	/* If all TIDs in the queue are inactive - mark queue as inactive. */
	if (tid_bitmap == mvm->queue_info[queue].tid_bitmap) {
		mvm->queue_info[queue].status = IWL_MVM_QUEUE_INACTIVE;

		for_each_set_bit(tid, &tid_bitmap, IWL_MAX_TID_COUNT + 1)
			mvmsta->tid_data[tid].is_tid_active = false;

		IWL_DEBUG_TX_QUEUES(mvm, "Queue %d marked as inactive\n",
				    queue);
		return;
	}

	/*
	 * If we are here, this is a shared queue and not all TIDs timed-out.
	 * Remove the ones that did.
	 */
	for_each_set_bit(tid, &tid_bitmap, IWL_MAX_TID_COUNT + 1) {
		int mac_queue = mvmsta->vif->hw_queue[tid_to_mac80211_ac[tid]];

		mvmsta->tid_data[tid].txq_id = IWL_MVM_INVALID_QUEUE;
		mvm->hw_queue_to_mac80211[queue] &= ~BIT(mac_queue);
		mvm->queue_info[queue].hw_queue_refcount--;
		mvm->queue_info[queue].tid_bitmap &= ~BIT(tid);
		mvmsta->tid_data[tid].is_tid_active = false;

		IWL_DEBUG_TX_QUEUES(mvm,
				    "Removing inactive TID %d from shared Q:%d\n",
				    tid, queue);
	}

	IWL_DEBUG_TX_QUEUES(mvm,
			    "TXQ #%d left with tid bitmap 0x%x\n", queue,
			    mvm->queue_info[queue].tid_bitmap);

	/*
	 * There may be different TIDs with the same mac queues, so make
	 * sure all TIDs have existing corresponding mac queues enabled
	 */
	tid_bitmap = mvm->queue_info[queue].tid_bitmap;
	for_each_set_bit(tid, &tid_bitmap, IWL_MAX_TID_COUNT + 1) {
		mvm->hw_queue_to_mac80211[queue] |=
			BIT(mvmsta->vif->hw_queue[tid_to_mac80211_ac[tid]]);
	}

	/* If the queue is marked as shared - "unshare" it */
	if (mvm->queue_info[queue].hw_queue_refcount == 1 &&
	    mvm->queue_info[queue].status == IWL_MVM_QUEUE_SHARED) {
		mvm->queue_info[queue].status = IWL_MVM_QUEUE_RECONFIGURING;
		IWL_DEBUG_TX_QUEUES(mvm, "Marking Q:%d for reconfig\n",
				    queue);
	}
}

void iwl_mvm_inactivity_check(struct iwl_mvm *mvm)
{
	unsigned long timeout_queues_map = 0;
	unsigned long now = jiffies;
	int i;

	if (iwl_mvm_has_new_tx_api(mvm))
		return;

	spin_lock_bh(&mvm->queue_info_lock);
	for (i = 0; i < IWL_MAX_HW_QUEUES; i++)
		if (mvm->queue_info[i].hw_queue_refcount > 0)
			timeout_queues_map |= BIT(i);
	spin_unlock_bh(&mvm->queue_info_lock);

	rcu_read_lock();

	/*
	 * If a queue time outs - mark it as INACTIVE (don't remove right away
	 * if we don't have to.) This is an optimization in case traffic comes
	 * later, and we don't HAVE to use a currently-inactive queue
	 */
	for_each_set_bit(i, &timeout_queues_map, IWL_MAX_HW_QUEUES) {
		struct ieee80211_sta *sta;
		struct iwl_mvm_sta *mvmsta;
		u8 sta_id;
		int tid;
		unsigned long inactive_tid_bitmap = 0;
		unsigned long queue_tid_bitmap;

		spin_lock_bh(&mvm->queue_info_lock);
		queue_tid_bitmap = mvm->queue_info[i].tid_bitmap;

		/* If TXQ isn't in active use anyway - nothing to do here... */
		if (mvm->queue_info[i].status != IWL_MVM_QUEUE_READY &&
		    mvm->queue_info[i].status != IWL_MVM_QUEUE_SHARED) {
			spin_unlock_bh(&mvm->queue_info_lock);
			continue;
		}

		/* Check to see if there are inactive TIDs on this queue */
		for_each_set_bit(tid, &queue_tid_bitmap,
				 IWL_MAX_TID_COUNT + 1) {
			if (time_after(mvm->queue_info[i].last_frame_time[tid] +
				       IWL_MVM_DQA_QUEUE_TIMEOUT, now))
				continue;

			inactive_tid_bitmap |= BIT(tid);
		}
		spin_unlock_bh(&mvm->queue_info_lock);

		/* If all TIDs are active - finish check on this queue */
		if (!inactive_tid_bitmap)
			continue;

		/*
		 * If we are here - the queue hadn't been served recently and is
		 * in use
		 */

		sta_id = mvm->queue_info[i].ra_sta_id;
		sta = rcu_dereference(mvm->fw_id_to_mac_id[sta_id]);

		/*
		 * If the STA doesn't exist anymore, it isn't an error. It could
		 * be that it was removed since getting the queues, and in this
		 * case it should've inactivated its queues anyway.
		 */
		if (IS_ERR_OR_NULL(sta))
			continue;

		mvmsta = iwl_mvm_sta_from_mac80211(sta);

		spin_lock_bh(&mvmsta->lock);
		spin_lock(&mvm->queue_info_lock);
		iwl_mvm_remove_inactive_tids(mvm, mvmsta, i,
					     inactive_tid_bitmap);
		spin_unlock(&mvm->queue_info_lock);
		spin_unlock_bh(&mvmsta->lock);
	}

	rcu_read_unlock();
}

=======
>>>>>>> upstream/android-13
void iwl_mvm_event_frame_timeout_callback(struct iwl_mvm *mvm,
					  struct ieee80211_vif *vif,
					  const struct ieee80211_sta *sta,
					  u16 tid)
{
	struct iwl_fw_dbg_trigger_tlv *trig;
	struct iwl_fw_dbg_trigger_ba *ba_trig;

<<<<<<< HEAD
	if (!iwl_fw_dbg_trigger_enabled(mvm->fw, FW_DBG_TRIGGER_BA))
		return;

	trig = iwl_fw_dbg_get_trigger(mvm->fw, FW_DBG_TRIGGER_BA);
	ba_trig = (void *)trig->data;
	if (!iwl_fw_dbg_trigger_check_stop(&mvm->fwrt,
					   ieee80211_vif_to_wdev(vif), trig))
		return;
=======
	trig = iwl_fw_dbg_trigger_on(&mvm->fwrt, ieee80211_vif_to_wdev(vif),
				     FW_DBG_TRIGGER_BA);
	if (!trig)
		return;

	ba_trig = (void *)trig->data;
>>>>>>> upstream/android-13

	if (!(le16_to_cpu(ba_trig->frame_timeout) & BIT(tid)))
		return;

	iwl_fw_dbg_collect_trig(&mvm->fwrt, trig,
				"Frame from %pM timed out, tid %d",
				sta->addr, tid);
}

u8 iwl_mvm_tcm_load_percentage(u32 airtime, u32 elapsed)
{
	if (!elapsed)
		return 0;

	return (100 * airtime / elapsed) / USEC_PER_MSEC;
}

static enum iwl_mvm_traffic_load
iwl_mvm_tcm_load(struct iwl_mvm *mvm, u32 airtime, unsigned long elapsed)
{
	u8 load = iwl_mvm_tcm_load_percentage(airtime, elapsed);

	if (load > IWL_MVM_TCM_LOAD_HIGH_THRESH)
		return IWL_MVM_TRAFFIC_HIGH;
	if (load > IWL_MVM_TCM_LOAD_MEDIUM_THRESH)
		return IWL_MVM_TRAFFIC_MEDIUM;

	return IWL_MVM_TRAFFIC_LOW;
}

<<<<<<< HEAD
struct iwl_mvm_tcm_iter_data {
	struct iwl_mvm *mvm;
	bool any_sent;
};

static void iwl_mvm_tcm_iter(void *_data, u8 *mac, struct ieee80211_vif *vif)
{
	struct iwl_mvm_tcm_iter_data *data = _data;
	struct iwl_mvm *mvm = data->mvm;
=======
static void iwl_mvm_tcm_iter(void *_data, u8 *mac, struct ieee80211_vif *vif)
{
	struct iwl_mvm *mvm = _data;
>>>>>>> upstream/android-13
	struct iwl_mvm_vif *mvmvif = iwl_mvm_vif_from_mac80211(vif);
	bool low_latency, prev = mvmvif->low_latency & LOW_LATENCY_TRAFFIC;

	if (mvmvif->id >= NUM_MAC_INDEX_DRIVER)
		return;

	low_latency = mvm->tcm.result.low_latency[mvmvif->id];

	if (!mvm->tcm.result.change[mvmvif->id] &&
	    prev == low_latency) {
		iwl_mvm_update_quotas(mvm, false, NULL);
		return;
	}

	if (prev != low_latency) {
		/* this sends traffic load and updates quota as well */
		iwl_mvm_update_low_latency(mvm, vif, low_latency,
					   LOW_LATENCY_TRAFFIC);
	} else {
		iwl_mvm_update_quotas(mvm, false, NULL);
	}
<<<<<<< HEAD

	data->any_sent = true;
=======
>>>>>>> upstream/android-13
}

static void iwl_mvm_tcm_results(struct iwl_mvm *mvm)
{
<<<<<<< HEAD
	struct iwl_mvm_tcm_iter_data data = {
		.mvm = mvm,
		.any_sent = false,
	};

=======
>>>>>>> upstream/android-13
	mutex_lock(&mvm->mutex);

	ieee80211_iterate_active_interfaces(
		mvm->hw, IEEE80211_IFACE_ITER_NORMAL,
<<<<<<< HEAD
		iwl_mvm_tcm_iter, &data);
=======
		iwl_mvm_tcm_iter, mvm);
>>>>>>> upstream/android-13

	if (fw_has_capa(&mvm->fw->ucode_capa, IWL_UCODE_TLV_CAPA_UMAC_SCAN))
		iwl_mvm_config_scan(mvm);

	mutex_unlock(&mvm->mutex);
}

static void iwl_mvm_tcm_uapsd_nonagg_detected_wk(struct work_struct *wk)
{
	struct iwl_mvm *mvm;
	struct iwl_mvm_vif *mvmvif;
	struct ieee80211_vif *vif;

	mvmvif = container_of(wk, struct iwl_mvm_vif,
			      uapsd_nonagg_detected_wk.work);
	vif = container_of((void *)mvmvif, struct ieee80211_vif, drv_priv);
	mvm = mvmvif->mvm;

	if (mvm->tcm.data[mvmvif->id].opened_rx_ba_sessions)
		return;

	/* remember that this AP is broken */
	memcpy(mvm->uapsd_noagg_bssids[mvm->uapsd_noagg_bssid_write_idx].addr,
	       vif->bss_conf.bssid, ETH_ALEN);
	mvm->uapsd_noagg_bssid_write_idx++;
	if (mvm->uapsd_noagg_bssid_write_idx >= IWL_MVM_UAPSD_NOAGG_LIST_LEN)
		mvm->uapsd_noagg_bssid_write_idx = 0;

	iwl_mvm_connection_loss(mvm, vif,
				"AP isn't using AMPDU with uAPSD enabled");
}

<<<<<<< HEAD
static void iwl_mvm_uapsd_agg_disconnect_iter(void *data, u8 *mac,
					      struct ieee80211_vif *vif)
{
	struct iwl_mvm_vif *mvmvif = iwl_mvm_vif_from_mac80211(vif);
	struct iwl_mvm *mvm = mvmvif->mvm;
	int *mac_id = data;
=======
static void iwl_mvm_uapsd_agg_disconnect(struct iwl_mvm *mvm,
					 struct ieee80211_vif *vif)
{
	struct iwl_mvm_vif *mvmvif = iwl_mvm_vif_from_mac80211(vif);
>>>>>>> upstream/android-13

	if (vif->type != NL80211_IFTYPE_STATION)
		return;

<<<<<<< HEAD
	if (mvmvif->id != *mac_id)
		return;

=======
>>>>>>> upstream/android-13
	if (!vif->bss_conf.assoc)
		return;

	if (!mvmvif->queue_params[IEEE80211_AC_VO].uapsd &&
	    !mvmvif->queue_params[IEEE80211_AC_VI].uapsd &&
	    !mvmvif->queue_params[IEEE80211_AC_BE].uapsd &&
	    !mvmvif->queue_params[IEEE80211_AC_BK].uapsd)
		return;

<<<<<<< HEAD
	if (mvm->tcm.data[*mac_id].uapsd_nonagg_detect.detected)
		return;

	mvm->tcm.data[*mac_id].uapsd_nonagg_detect.detected = true;
=======
	if (mvm->tcm.data[mvmvif->id].uapsd_nonagg_detect.detected)
		return;

	mvm->tcm.data[mvmvif->id].uapsd_nonagg_detect.detected = true;
>>>>>>> upstream/android-13
	IWL_INFO(mvm,
		 "detected AP should do aggregation but isn't, likely due to U-APSD\n");
	schedule_delayed_work(&mvmvif->uapsd_nonagg_detected_wk, 15 * HZ);
}

static void iwl_mvm_check_uapsd_agg_expected_tpt(struct iwl_mvm *mvm,
						 unsigned int elapsed,
						 int mac)
{
	u64 bytes = mvm->tcm.data[mac].uapsd_nonagg_detect.rx_bytes;
	u64 tpt;
	unsigned long rate;
<<<<<<< HEAD
=======
	struct ieee80211_vif *vif;
>>>>>>> upstream/android-13

	rate = ewma_rate_read(&mvm->tcm.data[mac].uapsd_nonagg_detect.rate);

	if (!rate || mvm->tcm.data[mac].opened_rx_ba_sessions ||
	    mvm->tcm.data[mac].uapsd_nonagg_detect.detected)
		return;

	if (iwl_mvm_has_new_rx_api(mvm)) {
		tpt = 8 * bytes; /* kbps */
		do_div(tpt, elapsed);
		rate *= 1000; /* kbps */
		if (tpt < 22 * rate / 100)
			return;
	} else {
		/*
		 * the rate here is actually the threshold, in 100Kbps units,
		 * so do the needed conversion from bytes to 100Kbps:
		 * 100kb = bits / (100 * 1000),
		 * 100kbps = 100kb / (msecs / 1000) ==
		 *           (bits / (100 * 1000)) / (msecs / 1000) ==
		 *           bits / (100 * msecs)
		 */
		tpt = (8 * bytes);
		do_div(tpt, elapsed * 100);
		if (tpt < rate)
			return;
	}

<<<<<<< HEAD
	ieee80211_iterate_active_interfaces_atomic(
		mvm->hw, IEEE80211_IFACE_ITER_NORMAL,
		iwl_mvm_uapsd_agg_disconnect_iter, &mac);
=======
	rcu_read_lock();
	vif = rcu_dereference(mvm->vif_id_to_mac[mac]);
	if (vif)
		iwl_mvm_uapsd_agg_disconnect(mvm, vif);
	rcu_read_unlock();
>>>>>>> upstream/android-13
}

static void iwl_mvm_tcm_iterator(void *_data, u8 *mac,
				 struct ieee80211_vif *vif)
{
	struct iwl_mvm_vif *mvmvif = iwl_mvm_vif_from_mac80211(vif);
	u32 *band = _data;

	if (!mvmvif->phy_ctxt)
		return;

	band[mvmvif->id] = mvmvif->phy_ctxt->channel->band;
}

static unsigned long iwl_mvm_calc_tcm_stats(struct iwl_mvm *mvm,
					    unsigned long ts,
					    bool handle_uapsd)
{
	unsigned int elapsed = jiffies_to_msecs(ts - mvm->tcm.ts);
	unsigned int uapsd_elapsed =
		jiffies_to_msecs(ts - mvm->tcm.uapsd_nonagg_ts);
	u32 total_airtime = 0;
	u32 band_airtime[NUM_NL80211_BANDS] = {0};
	u32 band[NUM_MAC_INDEX_DRIVER] = {0};
	int ac, mac, i;
	bool low_latency = false;
	enum iwl_mvm_traffic_load load, band_load;
	bool handle_ll = time_after(ts, mvm->tcm.ll_ts + MVM_LL_PERIOD);

	if (handle_ll)
		mvm->tcm.ll_ts = ts;
	if (handle_uapsd)
		mvm->tcm.uapsd_nonagg_ts = ts;

	mvm->tcm.result.elapsed = elapsed;

	ieee80211_iterate_active_interfaces_atomic(mvm->hw,
						   IEEE80211_IFACE_ITER_NORMAL,
						   iwl_mvm_tcm_iterator,
						   &band);

	for (mac = 0; mac < NUM_MAC_INDEX_DRIVER; mac++) {
		struct iwl_mvm_tcm_mac *mdata = &mvm->tcm.data[mac];
		u32 vo_vi_pkts = 0;
		u32 airtime = mdata->rx.airtime + mdata->tx.airtime;

		total_airtime += airtime;
		band_airtime[band[mac]] += airtime;

		load = iwl_mvm_tcm_load(mvm, airtime, elapsed);
		mvm->tcm.result.change[mac] = load != mvm->tcm.result.load[mac];
		mvm->tcm.result.load[mac] = load;
		mvm->tcm.result.airtime[mac] = airtime;

		for (ac = IEEE80211_AC_VO; ac <= IEEE80211_AC_VI; ac++)
			vo_vi_pkts += mdata->rx.pkts[ac] +
				      mdata->tx.pkts[ac];

		/* enable immediately with enough packets but defer disabling */
		if (vo_vi_pkts > IWL_MVM_TCM_LOWLAT_ENABLE_THRESH)
			mvm->tcm.result.low_latency[mac] = true;
		else if (handle_ll)
			mvm->tcm.result.low_latency[mac] = false;

		if (handle_ll) {
			/* clear old data */
			memset(&mdata->rx.pkts, 0, sizeof(mdata->rx.pkts));
			memset(&mdata->tx.pkts, 0, sizeof(mdata->tx.pkts));
		}
		low_latency |= mvm->tcm.result.low_latency[mac];

		if (!mvm->tcm.result.low_latency[mac] && handle_uapsd)
			iwl_mvm_check_uapsd_agg_expected_tpt(mvm, uapsd_elapsed,
							     mac);
		/* clear old data */
		if (handle_uapsd)
			mdata->uapsd_nonagg_detect.rx_bytes = 0;
		memset(&mdata->rx.airtime, 0, sizeof(mdata->rx.airtime));
		memset(&mdata->tx.airtime, 0, sizeof(mdata->tx.airtime));
	}

	load = iwl_mvm_tcm_load(mvm, total_airtime, elapsed);
<<<<<<< HEAD
	mvm->tcm.result.global_change = load != mvm->tcm.result.global_load;
=======
>>>>>>> upstream/android-13
	mvm->tcm.result.global_load = load;

	for (i = 0; i < NUM_NL80211_BANDS; i++) {
		band_load = iwl_mvm_tcm_load(mvm, band_airtime[i], elapsed);
		mvm->tcm.result.band_load[i] = band_load;
	}

	/*
	 * If the current load isn't low we need to force re-evaluation
	 * in the TCM period, so that we can return to low load if there
	 * was no traffic at all (and thus iwl_mvm_recalc_tcm didn't get
	 * triggered by traffic).
	 */
	if (load != IWL_MVM_TRAFFIC_LOW)
		return MVM_TCM_PERIOD;
	/*
	 * If low-latency is active we need to force re-evaluation after
	 * (the longer) MVM_LL_PERIOD, so that we can disable low-latency
	 * when there's no traffic at all.
	 */
	if (low_latency)
		return MVM_LL_PERIOD;
	/*
	 * Otherwise, we don't need to run the work struct because we're
	 * in the default "idle" state - traffic indication is low (which
	 * also covers the "no traffic" case) and low-latency is disabled
	 * so there's no state that may need to be disabled when there's
	 * no traffic at all.
	 *
	 * Note that this has no impact on the regular scheduling of the
	 * updates triggered by traffic - those happen whenever one of the
	 * two timeouts expire (if there's traffic at all.)
	 */
	return 0;
}

void iwl_mvm_recalc_tcm(struct iwl_mvm *mvm)
{
	unsigned long ts = jiffies;
	bool handle_uapsd =
		time_after(ts, mvm->tcm.uapsd_nonagg_ts +
			       msecs_to_jiffies(IWL_MVM_UAPSD_NONAGG_PERIOD));

	spin_lock(&mvm->tcm.lock);
	if (mvm->tcm.paused || !time_after(ts, mvm->tcm.ts + MVM_TCM_PERIOD)) {
		spin_unlock(&mvm->tcm.lock);
		return;
	}
	spin_unlock(&mvm->tcm.lock);

	if (handle_uapsd && iwl_mvm_has_new_rx_api(mvm)) {
		mutex_lock(&mvm->mutex);
		if (iwl_mvm_request_statistics(mvm, true))
			handle_uapsd = false;
		mutex_unlock(&mvm->mutex);
	}

	spin_lock(&mvm->tcm.lock);
	/* re-check if somebody else won the recheck race */
	if (!mvm->tcm.paused && time_after(ts, mvm->tcm.ts + MVM_TCM_PERIOD)) {
		/* calculate statistics */
		unsigned long work_delay = iwl_mvm_calc_tcm_stats(mvm, ts,
								  handle_uapsd);

		/* the memset needs to be visible before the timestamp */
		smp_mb();
		mvm->tcm.ts = ts;
		if (work_delay)
			schedule_delayed_work(&mvm->tcm.work, work_delay);
	}
	spin_unlock(&mvm->tcm.lock);

	iwl_mvm_tcm_results(mvm);
}

void iwl_mvm_tcm_work(struct work_struct *work)
{
	struct delayed_work *delayed_work = to_delayed_work(work);
	struct iwl_mvm *mvm = container_of(delayed_work, struct iwl_mvm,
					   tcm.work);

	iwl_mvm_recalc_tcm(mvm);
}

void iwl_mvm_pause_tcm(struct iwl_mvm *mvm, bool with_cancel)
{
	spin_lock_bh(&mvm->tcm.lock);
	mvm->tcm.paused = true;
	spin_unlock_bh(&mvm->tcm.lock);
	if (with_cancel)
		cancel_delayed_work_sync(&mvm->tcm.work);
}

void iwl_mvm_resume_tcm(struct iwl_mvm *mvm)
{
	int mac;
	bool low_latency = false;

	spin_lock_bh(&mvm->tcm.lock);
	mvm->tcm.ts = jiffies;
	mvm->tcm.ll_ts = jiffies;
	for (mac = 0; mac < NUM_MAC_INDEX_DRIVER; mac++) {
		struct iwl_mvm_tcm_mac *mdata = &mvm->tcm.data[mac];

		memset(&mdata->rx.pkts, 0, sizeof(mdata->rx.pkts));
		memset(&mdata->tx.pkts, 0, sizeof(mdata->tx.pkts));
		memset(&mdata->rx.airtime, 0, sizeof(mdata->rx.airtime));
		memset(&mdata->tx.airtime, 0, sizeof(mdata->tx.airtime));

		if (mvm->tcm.result.low_latency[mac])
			low_latency = true;
	}
	/* The TCM data needs to be reset before "paused" flag changes */
	smp_mb();
	mvm->tcm.paused = false;

	/*
	 * if the current load is not low or low latency is active, force
	 * re-evaluation to cover the case of no traffic.
	 */
	if (mvm->tcm.result.global_load > IWL_MVM_TRAFFIC_LOW)
		schedule_delayed_work(&mvm->tcm.work, MVM_TCM_PERIOD);
	else if (low_latency)
		schedule_delayed_work(&mvm->tcm.work, MVM_LL_PERIOD);

	spin_unlock_bh(&mvm->tcm.lock);
}

void iwl_mvm_tcm_add_vif(struct iwl_mvm *mvm, struct ieee80211_vif *vif)
{
	struct iwl_mvm_vif *mvmvif = iwl_mvm_vif_from_mac80211(vif);

	INIT_DELAYED_WORK(&mvmvif->uapsd_nonagg_detected_wk,
			  iwl_mvm_tcm_uapsd_nonagg_detected_wk);
}

void iwl_mvm_tcm_rm_vif(struct iwl_mvm *mvm, struct ieee80211_vif *vif)
{
	struct iwl_mvm_vif *mvmvif = iwl_mvm_vif_from_mac80211(vif);

	cancel_delayed_work_sync(&mvmvif->uapsd_nonagg_detected_wk);
}

<<<<<<< HEAD

void iwl_mvm_get_sync_time(struct iwl_mvm *mvm, u32 *gp2, u64 *boottime)
=======
u32 iwl_mvm_get_systime(struct iwl_mvm *mvm)
{
	u32 reg_addr = DEVICE_SYSTEM_TIME_REG;

	if (mvm->trans->trans_cfg->device_family >= IWL_DEVICE_FAMILY_22000 &&
	    mvm->trans->cfg->gp2_reg_addr)
		reg_addr = mvm->trans->cfg->gp2_reg_addr;

	return iwl_read_prph(mvm->trans, reg_addr);
}

void iwl_mvm_get_sync_time(struct iwl_mvm *mvm, int clock_type,
			   u32 *gp2, u64 *boottime, ktime_t *realtime)
>>>>>>> upstream/android-13
{
	bool ps_disabled;

	lockdep_assert_held(&mvm->mutex);

	/* Disable power save when reading GP2 */
	ps_disabled = mvm->ps_disabled;
	if (!ps_disabled) {
		mvm->ps_disabled = true;
		iwl_mvm_power_update_device(mvm);
	}

<<<<<<< HEAD
	*gp2 = iwl_read_prph(mvm->trans, DEVICE_SYSTEM_TIME_REG);
	*boottime = ktime_get_boot_ns();
=======
	*gp2 = iwl_mvm_get_systime(mvm);

	if (clock_type == CLOCK_BOOTTIME && boottime)
		*boottime = ktime_get_boottime_ns();
	else if (clock_type == CLOCK_REALTIME && realtime)
		*realtime = ktime_get_real();
>>>>>>> upstream/android-13

	if (!ps_disabled) {
		mvm->ps_disabled = ps_disabled;
		iwl_mvm_power_update_device(mvm);
	}
}
