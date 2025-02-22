<<<<<<< HEAD
/******************************************************************************
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * Copyright(c) 2012 - 2014 Intel Corporation. All rights reserved.
 * Copyright(c) 2013 - 2015 Intel Mobile Communications GmbH
 * Copyright(c) 2016 - 2017 Intel Deutschland GmbH
 * Copyright(c) 2018 - 2019 Intel Corporation
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
 * Copyright(c) 2013 - 2015 Intel Mobile Communications GmbH
 * Copyright(c) 2016 - 2017 Intel Deutschland GmbH
 * Copyright(c) 2018 - 2019 Intel Corporation
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
 * Copyright (C) 2012-2014, 2018-2020 Intel Corporation
 * Copyright (C) 2013-2015 Intel Mobile Communications GmbH
 * Copyright (C) 2016-2017 Intel Deutschland GmbH
 */
>>>>>>> upstream/android-13
#include "iwl-drv.h"
#include "runtime.h"
#include "fw/api/commands.h"

static void iwl_parse_shared_mem_22000(struct iwl_fw_runtime *fwrt,
				       struct iwl_rx_packet *pkt)
{
	struct iwl_shared_mem_cfg *mem_cfg = (void *)pkt->data;
	int i, lmac;
	int lmac_num = le32_to_cpu(mem_cfg->lmac_num);
<<<<<<< HEAD
=======
	u8 api_ver = iwl_fw_lookup_notif_ver(fwrt->fw, SYSTEM_GROUP,
					     SHARED_MEM_CFG_CMD, 0);
>>>>>>> upstream/android-13

	if (WARN_ON(lmac_num > ARRAY_SIZE(mem_cfg->lmac_smem)))
		return;

	fwrt->smem_cfg.num_lmacs = lmac_num;
	fwrt->smem_cfg.num_txfifo_entries =
		ARRAY_SIZE(mem_cfg->lmac_smem[0].txfifo_size);
	fwrt->smem_cfg.rxfifo2_size = le32_to_cpu(mem_cfg->rxfifo2_size);

<<<<<<< HEAD
=======
	if (api_ver >= 4 &&
	    !WARN_ON_ONCE(iwl_rx_packet_payload_len(pkt) < sizeof(*mem_cfg))) {
		fwrt->smem_cfg.rxfifo2_control_size =
			le32_to_cpu(mem_cfg->rxfifo2_control_size);
	}

>>>>>>> upstream/android-13
	for (lmac = 0; lmac < lmac_num; lmac++) {
		struct iwl_shared_mem_lmac_cfg *lmac_cfg =
			&mem_cfg->lmac_smem[lmac];

		for (i = 0; i < ARRAY_SIZE(lmac_cfg->txfifo_size); i++)
			fwrt->smem_cfg.lmac[lmac].txfifo_size[i] =
				le32_to_cpu(lmac_cfg->txfifo_size[i]);
		fwrt->smem_cfg.lmac[lmac].rxfifo1_size =
			le32_to_cpu(lmac_cfg->rxfifo1_size);
	}
}

static void iwl_parse_shared_mem(struct iwl_fw_runtime *fwrt,
				 struct iwl_rx_packet *pkt)
{
	struct iwl_shared_mem_cfg_v2 *mem_cfg = (void *)pkt->data;
	int i;

	fwrt->smem_cfg.num_lmacs = 1;

	fwrt->smem_cfg.num_txfifo_entries = ARRAY_SIZE(mem_cfg->txfifo_size);
	for (i = 0; i < ARRAY_SIZE(mem_cfg->txfifo_size); i++)
		fwrt->smem_cfg.lmac[0].txfifo_size[i] =
			le32_to_cpu(mem_cfg->txfifo_size[i]);

	fwrt->smem_cfg.lmac[0].rxfifo1_size =
		le32_to_cpu(mem_cfg->rxfifo_size[0]);
	fwrt->smem_cfg.rxfifo2_size = le32_to_cpu(mem_cfg->rxfifo_size[1]);

	/* new API has more data, from rxfifo_addr field and on */
	if (fw_has_capa(&fwrt->fw->ucode_capa,
			IWL_UCODE_TLV_CAPA_EXTEND_SHARED_MEM_CFG)) {
		BUILD_BUG_ON(sizeof(fwrt->smem_cfg.internal_txfifo_size) !=
			     sizeof(mem_cfg->internal_txfifo_size));

		fwrt->smem_cfg.internal_txfifo_addr =
			le32_to_cpu(mem_cfg->internal_txfifo_addr);

		for (i = 0;
		     i < ARRAY_SIZE(fwrt->smem_cfg.internal_txfifo_size);
		     i++)
			fwrt->smem_cfg.internal_txfifo_size[i] =
				le32_to_cpu(mem_cfg->internal_txfifo_size[i]);
	}
}

void iwl_get_shared_mem_conf(struct iwl_fw_runtime *fwrt)
{
	struct iwl_host_cmd cmd = {
		.flags = CMD_WANT_SKB,
		.data = { NULL, },
		.len = { 0, },
	};
	struct iwl_rx_packet *pkt;
	int ret;

	if (fw_has_capa(&fwrt->fw->ucode_capa,
			IWL_UCODE_TLV_CAPA_EXTEND_SHARED_MEM_CFG))
		cmd.id = iwl_cmd_id(SHARED_MEM_CFG_CMD, SYSTEM_GROUP, 0);
	else
		cmd.id = SHARED_MEM_CFG;

	ret = iwl_trans_send_cmd(fwrt->trans, &cmd);

	if (ret) {
		WARN(ret != -ERFKILL,
		     "Could not send the SMEM command: %d\n", ret);
		return;
	}

	pkt = cmd.resp_pkt;
<<<<<<< HEAD
	if (fwrt->trans->cfg->device_family >= IWL_DEVICE_FAMILY_22000)
=======
	if (fwrt->trans->trans_cfg->device_family >= IWL_DEVICE_FAMILY_22000)
>>>>>>> upstream/android-13
		iwl_parse_shared_mem_22000(fwrt, pkt);
	else
		iwl_parse_shared_mem(fwrt, pkt);

	IWL_DEBUG_INFO(fwrt, "SHARED MEM CFG: got memory offsets/sizes\n");

	iwl_free_resp(&cmd);
}
IWL_EXPORT_SYMBOL(iwl_get_shared_mem_conf);
