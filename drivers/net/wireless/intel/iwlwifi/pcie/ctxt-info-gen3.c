<<<<<<< HEAD
/******************************************************************************
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
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
 * BSD LICENSE
 *
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
 * Copyright (C) 2018-2021 Intel Corporation
 */
>>>>>>> upstream/android-13
#include "iwl-trans.h"
#include "iwl-fh.h"
#include "iwl-context-info-gen3.h"
#include "internal.h"
#include "iwl-prph.h"

<<<<<<< HEAD
=======
static void
iwl_pcie_ctxt_info_dbg_enable(struct iwl_trans *trans,
			      struct iwl_prph_scratch_hwm_cfg *dbg_cfg,
			      u32 *control_flags)
{
	enum iwl_fw_ini_allocation_id alloc_id = IWL_FW_INI_ALLOCATION_ID_DBGC1;
	struct iwl_fw_ini_allocation_tlv *fw_mon_cfg;
	u32 dbg_flags = 0;

	if (!iwl_trans_dbg_ini_valid(trans)) {
		struct iwl_dram_data *fw_mon = &trans->dbg.fw_mon;

		iwl_pcie_alloc_fw_monitor(trans, 0);

		if (fw_mon->size) {
			dbg_flags |= IWL_PRPH_SCRATCH_EDBG_DEST_DRAM;

			IWL_DEBUG_FW(trans,
				     "WRT: Applying DRAM buffer destination\n");

			dbg_cfg->hwm_base_addr = cpu_to_le64(fw_mon->physical);
			dbg_cfg->hwm_size = cpu_to_le32(fw_mon->size);
		}

		goto out;
	}

	fw_mon_cfg = &trans->dbg.fw_mon_cfg[alloc_id];

	switch (le32_to_cpu(fw_mon_cfg->buf_location)) {
	case IWL_FW_INI_LOCATION_SRAM_PATH:
		dbg_flags |= IWL_PRPH_SCRATCH_EDBG_DEST_INTERNAL;
		IWL_DEBUG_FW(trans,
				"WRT: Applying SMEM buffer destination\n");
		break;

	case IWL_FW_INI_LOCATION_NPK_PATH:
		dbg_flags |= IWL_PRPH_SCRATCH_EDBG_DEST_TB22DTF;
		IWL_DEBUG_FW(trans,
			     "WRT: Applying NPK buffer destination\n");
		break;

	case IWL_FW_INI_LOCATION_DRAM_PATH:
		if (trans->dbg.fw_mon_ini[alloc_id].num_frags) {
			struct iwl_dram_data *frag =
				&trans->dbg.fw_mon_ini[alloc_id].frags[0];
			dbg_flags |= IWL_PRPH_SCRATCH_EDBG_DEST_DRAM;
			dbg_cfg->hwm_base_addr = cpu_to_le64(frag->physical);
			dbg_cfg->hwm_size = cpu_to_le32(frag->size);
			IWL_DEBUG_FW(trans,
				     "WRT: Applying DRAM destination (alloc_id=%u, num_frags=%u)\n",
				     alloc_id,
				     trans->dbg.fw_mon_ini[alloc_id].num_frags);
		}
		break;
	default:
		IWL_ERR(trans, "WRT: Invalid buffer destination\n");
	}
out:
	if (dbg_flags)
		*control_flags |= IWL_PRPH_SCRATCH_EARLY_DEBUG_EN | dbg_flags;
}

>>>>>>> upstream/android-13
int iwl_pcie_ctxt_info_gen3_init(struct iwl_trans *trans,
				 const struct fw_img *fw)
{
	struct iwl_trans_pcie *trans_pcie = IWL_TRANS_GET_PCIE_TRANS(trans);
	struct iwl_context_info_gen3 *ctxt_info_gen3;
	struct iwl_prph_scratch *prph_scratch;
	struct iwl_prph_scratch_ctrl_cfg *prph_sc_ctrl;
	struct iwl_prph_info *prph_info;
<<<<<<< HEAD
	void *iml_img;
	u32 control_flags = 0;
	int ret;
=======
	u32 control_flags = 0;
	int ret;
	int cmdq_size = max_t(u32, IWL_CMD_QUEUE_SIZE,
			      trans->cfg->min_txq_size);

	switch (trans_pcie->rx_buf_size) {
	case IWL_AMSDU_DEF:
		return -EINVAL;
	case IWL_AMSDU_2K:
		break;
	case IWL_AMSDU_4K:
		control_flags |= IWL_PRPH_SCRATCH_RB_SIZE_4K;
		break;
	case IWL_AMSDU_8K:
		control_flags |= IWL_PRPH_SCRATCH_RB_SIZE_4K;
		/* if firmware supports the ext size, tell it */
		control_flags |= IWL_PRPH_SCRATCH_RB_SIZE_EXT_8K;
		break;
	case IWL_AMSDU_12K:
		control_flags |= IWL_PRPH_SCRATCH_RB_SIZE_4K;
		/* if firmware supports the ext size, tell it */
		control_flags |= IWL_PRPH_SCRATCH_RB_SIZE_EXT_16K;
		break;
	}
>>>>>>> upstream/android-13

	/* Allocate prph scratch */
	prph_scratch = dma_alloc_coherent(trans->dev, sizeof(*prph_scratch),
					  &trans_pcie->prph_scratch_dma_addr,
					  GFP_KERNEL);
	if (!prph_scratch)
		return -ENOMEM;

	prph_sc_ctrl = &prph_scratch->ctrl_cfg;

	prph_sc_ctrl->version.version = 0;
	prph_sc_ctrl->version.mac_id =
		cpu_to_le16((u16)iwl_read32(trans, CSR_HW_REV));
	prph_sc_ctrl->version.size = cpu_to_le16(sizeof(*prph_scratch) / 4);

<<<<<<< HEAD
	control_flags = IWL_PRPH_SCRATCH_RB_SIZE_4K |
			IWL_PRPH_SCRATCH_MTR_MODE |
			(IWL_PRPH_MTR_FORMAT_256B &
			 IWL_PRPH_SCRATCH_MTR_FORMAT) |
			IWL_PRPH_SCRATCH_EARLY_DEBUG_EN |
			IWL_PRPH_SCRATCH_EDBG_DEST_DRAM;
	prph_sc_ctrl->control.control_flags = cpu_to_le32(control_flags);
=======
	control_flags |= IWL_PRPH_SCRATCH_MTR_MODE;
	control_flags |= IWL_PRPH_MTR_FORMAT_256B & IWL_PRPH_SCRATCH_MTR_FORMAT;
>>>>>>> upstream/android-13

	/* initialize RX default queue */
	prph_sc_ctrl->rbd_cfg.free_rbd_addr =
		cpu_to_le64(trans_pcie->rxq->bd_dma);

<<<<<<< HEAD
	/* Configure debug, for integration */
	iwl_pcie_alloc_fw_monitor(trans, 0);
	prph_sc_ctrl->hwm_cfg.hwm_base_addr =
		cpu_to_le64(trans_pcie->fw_mon_phys);
	prph_sc_ctrl->hwm_cfg.hwm_size =
		cpu_to_le32(trans_pcie->fw_mon_size);
=======
	iwl_pcie_ctxt_info_dbg_enable(trans, &prph_sc_ctrl->hwm_cfg,
				      &control_flags);
	prph_sc_ctrl->control.control_flags = cpu_to_le32(control_flags);
>>>>>>> upstream/android-13

	/* allocate ucode sections in dram and set addresses */
	ret = iwl_pcie_init_fw_sec(trans, fw, &prph_scratch->dram);
	if (ret)
		goto err_free_prph_scratch;


	/* Allocate prph information
	 * currently we don't assign to the prph info anything, but it would get
<<<<<<< HEAD
	 * assigned later */
	prph_info = dma_alloc_coherent(trans->dev, sizeof(*prph_info),
=======
	 * assigned later
	 *
	 * We also use the second half of this page to give the device some
	 * dummy TR/CR tail pointers - which shouldn't be necessary as we don't
	 * use this, but the hardware still reads/writes there and we can't let
	 * it go do that with a NULL pointer.
	 */
	BUILD_BUG_ON(sizeof(*prph_info) > PAGE_SIZE / 2);
	prph_info = dma_alloc_coherent(trans->dev, PAGE_SIZE,
>>>>>>> upstream/android-13
				       &trans_pcie->prph_info_dma_addr,
				       GFP_KERNEL);
	if (!prph_info) {
		ret = -ENOMEM;
		goto err_free_prph_scratch;
	}

	/* Allocate context info */
	ctxt_info_gen3 = dma_alloc_coherent(trans->dev,
					    sizeof(*ctxt_info_gen3),
					    &trans_pcie->ctxt_info_dma_addr,
					    GFP_KERNEL);
	if (!ctxt_info_gen3) {
		ret = -ENOMEM;
		goto err_free_prph_info;
	}

	ctxt_info_gen3->prph_info_base_addr =
		cpu_to_le64(trans_pcie->prph_info_dma_addr);
	ctxt_info_gen3->prph_scratch_base_addr =
		cpu_to_le64(trans_pcie->prph_scratch_dma_addr);
	ctxt_info_gen3->prph_scratch_size =
		cpu_to_le32(sizeof(*prph_scratch));
	ctxt_info_gen3->cr_head_idx_arr_base_addr =
		cpu_to_le64(trans_pcie->rxq->rb_stts_dma);
	ctxt_info_gen3->tr_tail_idx_arr_base_addr =
<<<<<<< HEAD
		cpu_to_le64(trans_pcie->rxq->tr_tail_dma);
	ctxt_info_gen3->cr_tail_idx_arr_base_addr =
		cpu_to_le64(trans_pcie->rxq->cr_tail_dma);
	ctxt_info_gen3->cr_idx_arr_size =
		cpu_to_le16(IWL_NUM_OF_COMPLETION_RINGS);
	ctxt_info_gen3->tr_idx_arr_size =
		cpu_to_le16(IWL_NUM_OF_TRANSFER_RINGS);
	ctxt_info_gen3->mtr_base_addr =
		cpu_to_le64(trans_pcie->txq[trans_pcie->cmd_queue]->dma_addr);
	ctxt_info_gen3->mcr_base_addr =
		cpu_to_le64(trans_pcie->rxq->used_bd_dma);
	ctxt_info_gen3->mtr_size =
		cpu_to_le16(TFD_QUEUE_CB_SIZE(TFD_CMD_SLOTS));
	ctxt_info_gen3->mcr_size =
		cpu_to_le16(RX_QUEUE_CB_SIZE(MQ_RX_TABLE_SIZE));
=======
		cpu_to_le64(trans_pcie->prph_info_dma_addr + PAGE_SIZE / 2);
	ctxt_info_gen3->cr_tail_idx_arr_base_addr =
		cpu_to_le64(trans_pcie->prph_info_dma_addr + 3 * PAGE_SIZE / 4);
	ctxt_info_gen3->mtr_base_addr =
		cpu_to_le64(trans->txqs.txq[trans->txqs.cmd.q_id]->dma_addr);
	ctxt_info_gen3->mcr_base_addr =
		cpu_to_le64(trans_pcie->rxq->used_bd_dma);
	ctxt_info_gen3->mtr_size =
		cpu_to_le16(TFD_QUEUE_CB_SIZE(cmdq_size));
	ctxt_info_gen3->mcr_size =
		cpu_to_le16(RX_QUEUE_CB_SIZE(trans->cfg->num_rbds));
>>>>>>> upstream/android-13

	trans_pcie->ctxt_info_gen3 = ctxt_info_gen3;
	trans_pcie->prph_info = prph_info;
	trans_pcie->prph_scratch = prph_scratch;

	/* Allocate IML */
<<<<<<< HEAD
	iml_img = dma_alloc_coherent(trans->dev, trans->iml_len,
				     &trans_pcie->iml_dma_addr, GFP_KERNEL);
	if (!iml_img) {
=======
	trans_pcie->iml = dma_alloc_coherent(trans->dev, trans->iml_len,
					     &trans_pcie->iml_dma_addr,
					     GFP_KERNEL);
	if (!trans_pcie->iml) {
>>>>>>> upstream/android-13
		ret = -ENOMEM;
		goto err_free_ctxt_info;
	}

<<<<<<< HEAD
	memcpy(iml_img, trans->iml, trans->iml_len);
=======
	memcpy(trans_pcie->iml, trans->iml, trans->iml_len);
>>>>>>> upstream/android-13

	iwl_enable_fw_load_int_ctx_info(trans);

	/* kick FW self load */
	iwl_write64(trans, CSR_CTXT_INFO_ADDR,
		    trans_pcie->ctxt_info_dma_addr);
	iwl_write64(trans, CSR_IML_DATA_ADDR,
		    trans_pcie->iml_dma_addr);
	iwl_write32(trans, CSR_IML_SIZE_ADDR, trans->iml_len);
<<<<<<< HEAD
	iwl_set_bit(trans, CSR_CTXT_INFO_BOOT_CTRL, CSR_AUTO_FUNC_BOOT_ENA);
	iwl_set_bit(trans, CSR_GP_CNTRL, CSR_AUTO_FUNC_INIT);
=======

	iwl_set_bit(trans, CSR_CTXT_INFO_BOOT_CTRL,
		    CSR_AUTO_FUNC_BOOT_ENA);
>>>>>>> upstream/android-13

	return 0;

err_free_ctxt_info:
	dma_free_coherent(trans->dev, sizeof(*trans_pcie->ctxt_info_gen3),
			  trans_pcie->ctxt_info_gen3,
			  trans_pcie->ctxt_info_dma_addr);
	trans_pcie->ctxt_info_gen3 = NULL;
err_free_prph_info:
<<<<<<< HEAD
	dma_free_coherent(trans->dev,
			  sizeof(*prph_info),
			prph_info,
			trans_pcie->prph_info_dma_addr);
=======
	dma_free_coherent(trans->dev, PAGE_SIZE, prph_info,
			  trans_pcie->prph_info_dma_addr);
>>>>>>> upstream/android-13

err_free_prph_scratch:
	dma_free_coherent(trans->dev,
			  sizeof(*prph_scratch),
			prph_scratch,
			trans_pcie->prph_scratch_dma_addr);
	return ret;

}

<<<<<<< HEAD
void iwl_pcie_ctxt_info_gen3_free(struct iwl_trans *trans)
{
	struct iwl_trans_pcie *trans_pcie = IWL_TRANS_GET_PCIE_TRANS(trans);

	if (!trans_pcie->ctxt_info_gen3)
		return;

=======
void iwl_pcie_ctxt_info_gen3_free(struct iwl_trans *trans, bool alive)
{
	struct iwl_trans_pcie *trans_pcie = IWL_TRANS_GET_PCIE_TRANS(trans);

	if (trans_pcie->iml) {
		dma_free_coherent(trans->dev, trans->iml_len, trans_pcie->iml,
				  trans_pcie->iml_dma_addr);
		trans_pcie->iml_dma_addr = 0;
		trans_pcie->iml = NULL;
	}

	iwl_pcie_ctxt_info_free_fw_img(trans);

	if (alive)
		return;

	if (!trans_pcie->ctxt_info_gen3)
		return;

	/* ctxt_info_gen3 and prph_scratch are still needed for PNVM load */
>>>>>>> upstream/android-13
	dma_free_coherent(trans->dev, sizeof(*trans_pcie->ctxt_info_gen3),
			  trans_pcie->ctxt_info_gen3,
			  trans_pcie->ctxt_info_dma_addr);
	trans_pcie->ctxt_info_dma_addr = 0;
	trans_pcie->ctxt_info_gen3 = NULL;

<<<<<<< HEAD
	iwl_pcie_ctxt_info_free_fw_img(trans);

=======
>>>>>>> upstream/android-13
	dma_free_coherent(trans->dev, sizeof(*trans_pcie->prph_scratch),
			  trans_pcie->prph_scratch,
			  trans_pcie->prph_scratch_dma_addr);
	trans_pcie->prph_scratch_dma_addr = 0;
	trans_pcie->prph_scratch = NULL;

<<<<<<< HEAD
	dma_free_coherent(trans->dev, sizeof(*trans_pcie->prph_info),
			  trans_pcie->prph_info,
=======
	/* this is needed for the entire lifetime */
	dma_free_coherent(trans->dev, PAGE_SIZE, trans_pcie->prph_info,
>>>>>>> upstream/android-13
			  trans_pcie->prph_info_dma_addr);
	trans_pcie->prph_info_dma_addr = 0;
	trans_pcie->prph_info = NULL;
}
<<<<<<< HEAD
=======

int iwl_trans_pcie_ctx_info_gen3_set_pnvm(struct iwl_trans *trans,
					  const void *data, u32 len)
{
	struct iwl_trans_pcie *trans_pcie = IWL_TRANS_GET_PCIE_TRANS(trans);
	struct iwl_prph_scratch_ctrl_cfg *prph_sc_ctrl =
		&trans_pcie->prph_scratch->ctrl_cfg;
	int ret;

	if (trans->trans_cfg->device_family < IWL_DEVICE_FAMILY_AX210)
		return 0;

	/* only allocate the DRAM if not allocated yet */
	if (!trans->pnvm_loaded) {
		if (WARN_ON(prph_sc_ctrl->pnvm_cfg.pnvm_size))
			return -EBUSY;

		ret = iwl_pcie_ctxt_info_alloc_dma(trans, data, len,
						   &trans_pcie->pnvm_dram);
		if (ret < 0) {
			IWL_DEBUG_FW(trans, "Failed to allocate PNVM DMA %d.\n",
				     ret);
			return ret;
		}
	}

	prph_sc_ctrl->pnvm_cfg.pnvm_base_addr =
		cpu_to_le64(trans_pcie->pnvm_dram.physical);
	prph_sc_ctrl->pnvm_cfg.pnvm_size =
		cpu_to_le32(trans_pcie->pnvm_dram.size);

	return 0;
}

int iwl_trans_pcie_ctx_info_gen3_set_reduce_power(struct iwl_trans *trans,
						  const void *data, u32 len)
{
	struct iwl_trans_pcie *trans_pcie = IWL_TRANS_GET_PCIE_TRANS(trans);
	struct iwl_prph_scratch_ctrl_cfg *prph_sc_ctrl =
		&trans_pcie->prph_scratch->ctrl_cfg;
	int ret;

	if (trans->trans_cfg->device_family < IWL_DEVICE_FAMILY_AX210)
		return 0;

	/* only allocate the DRAM if not allocated yet */
	if (!trans->reduce_power_loaded) {
		if (WARN_ON(prph_sc_ctrl->reduce_power_cfg.size))
			return -EBUSY;

		ret = iwl_pcie_ctxt_info_alloc_dma(trans, data, len,
					   &trans_pcie->reduce_power_dram);
		if (ret < 0) {
			IWL_DEBUG_FW(trans,
				     "Failed to allocate reduce power DMA %d.\n",
				     ret);
			return ret;
		}
	}

	prph_sc_ctrl->reduce_power_cfg.base_addr =
		cpu_to_le64(trans_pcie->reduce_power_dram.physical);
	prph_sc_ctrl->reduce_power_cfg.size =
		cpu_to_le32(trans_pcie->reduce_power_dram.size);

	return 0;
}
>>>>>>> upstream/android-13
