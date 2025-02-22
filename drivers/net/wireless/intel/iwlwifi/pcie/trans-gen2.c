<<<<<<< HEAD
/******************************************************************************
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * Copyright(c) 2017 Intel Deutschland GmbH
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
 * Copyright(c) 2017 Intel Deutschland GmbH
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
 * Copyright (C) 2017 Intel Deutschland GmbH
 * Copyright (C) 2018-2021 Intel Corporation
 */
>>>>>>> upstream/android-13
#include "iwl-trans.h"
#include "iwl-prph.h"
#include "iwl-context-info.h"
#include "iwl-context-info-gen3.h"
#include "internal.h"
<<<<<<< HEAD
=======
#include "fw/dbg.h"

#define FW_RESET_TIMEOUT (HZ / 5)
>>>>>>> upstream/android-13

/*
 * Start up NIC's basic functionality after it has been reset
 * (e.g. after platform boot, or shutdown via iwl_pcie_apm_stop())
 * NOTE:  This does not load uCode nor start the embedded processor
 */
<<<<<<< HEAD
static int iwl_pcie_gen2_apm_init(struct iwl_trans *trans)
=======
int iwl_pcie_gen2_apm_init(struct iwl_trans *trans)
>>>>>>> upstream/android-13
{
	int ret = 0;

	IWL_DEBUG_INFO(trans, "Init card's basic functions\n");

	/*
	 * Use "set_bit" below rather than "write", to preserve any hardware
	 * bits already set by default after reset.
	 */

	/*
	 * Disable L0s without affecting L1;
	 * don't wait for ICH L0s (ICH bug W/A)
	 */
	iwl_set_bit(trans, CSR_GIO_CHICKEN_BITS,
		    CSR_GIO_CHICKEN_BITS_REG_BIT_L1A_NO_L0S_RX);

	/* Set FH wait threshold to maximum (HW error during stress W/A) */
	iwl_set_bit(trans, CSR_DBG_HPET_MEM_REG, CSR_DBG_HPET_MEM_REG_VAL);

	/*
	 * Enable HAP INTA (interrupt from management bus) to
	 * wake device's PCI Express link L1a -> L0s
	 */
	iwl_set_bit(trans, CSR_HW_IF_CONFIG_REG,
		    CSR_HW_IF_CONFIG_REG_BIT_HAP_WAKE_L1A);

	iwl_pcie_apm_config(trans);

<<<<<<< HEAD
	/*
	 * Set "initialization complete" bit to move adapter from
	 * D0U* --> D0A* (powered-up active) state.
	 */
	iwl_set_bit(trans, CSR_GP_CNTRL,
		    BIT(trans->cfg->csr->flag_init_done));

	/*
	 * Wait for clock stabilization; once stabilized, access to
	 * device-internal resources is supported, e.g. iwl_write_prph()
	 * and accesses to uCode SRAM.
	 */
	ret = iwl_poll_bit(trans, CSR_GP_CNTRL,
			   BIT(trans->cfg->csr->flag_mac_clock_ready),
			   BIT(trans->cfg->csr->flag_mac_clock_ready),
			   25000);
	if (ret < 0) {
		IWL_DEBUG_INFO(trans, "Failed to init the card\n");
		return ret;
	}
=======
	ret = iwl_finish_nic_init(trans, trans->trans_cfg);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	set_bit(STATUS_DEVICE_ENABLED, &trans->status);

	return 0;
}

static void iwl_pcie_gen2_apm_stop(struct iwl_trans *trans, bool op_mode_leave)
{
	IWL_DEBUG_INFO(trans, "Stop card, put in low power state\n");

	if (op_mode_leave) {
		if (!test_bit(STATUS_DEVICE_ENABLED, &trans->status))
			iwl_pcie_gen2_apm_init(trans);

		/* inform ME that we are leaving */
		iwl_set_bit(trans, CSR_DBG_LINK_PWR_MGMT_REG,
			    CSR_RESET_LINK_PWR_MGMT_DISABLED);
		iwl_set_bit(trans, CSR_HW_IF_CONFIG_REG,
			    CSR_HW_IF_CONFIG_REG_PREPARE |
			    CSR_HW_IF_CONFIG_REG_ENABLE_PME);
		mdelay(1);
		iwl_clear_bit(trans, CSR_DBG_LINK_PWR_MGMT_REG,
			      CSR_RESET_LINK_PWR_MGMT_DISABLED);
		mdelay(5);
	}

	clear_bit(STATUS_DEVICE_ENABLED, &trans->status);

	/* Stop device's DMA activity */
	iwl_pcie_apm_stop_master(trans);

	iwl_trans_sw_reset(trans);

	/*
	 * Clear "initialization complete" bit to move adapter from
	 * D0A* (powered-up Active) --> D0U* (Uninitialized) state.
	 */
<<<<<<< HEAD
	iwl_clear_bit(trans, CSR_GP_CNTRL,
		      BIT(trans->cfg->csr->flag_init_done));
}

void _iwl_trans_pcie_gen2_stop_device(struct iwl_trans *trans, bool low_power)
=======
	if (trans->trans_cfg->device_family >= IWL_DEVICE_FAMILY_BZ)
		iwl_clear_bit(trans, CSR_GP_CNTRL,
			      CSR_GP_CNTRL_REG_FLAG_MAC_INIT);
	else
		iwl_clear_bit(trans, CSR_GP_CNTRL,
			      CSR_GP_CNTRL_REG_FLAG_INIT_DONE);
}

static void iwl_trans_pcie_fw_reset_handshake(struct iwl_trans *trans)
{
	struct iwl_trans_pcie *trans_pcie = IWL_TRANS_GET_PCIE_TRANS(trans);
	int ret;

	trans_pcie->fw_reset_state = FW_RESET_REQUESTED;

	if (trans->trans_cfg->device_family < IWL_DEVICE_FAMILY_AX210)
		iwl_write_umac_prph(trans, UREG_NIC_SET_NMI_DRIVER,
				    UREG_NIC_SET_NMI_DRIVER_RESET_HANDSHAKE);
	else
		iwl_write_umac_prph(trans, UREG_DOORBELL_TO_ISR6,
				    UREG_DOORBELL_TO_ISR6_RESET_HANDSHAKE);

	/* wait 200ms */
	ret = wait_event_timeout(trans_pcie->fw_reset_waitq,
				 trans_pcie->fw_reset_state != FW_RESET_REQUESTED,
				 FW_RESET_TIMEOUT);
	if (!ret || trans_pcie->fw_reset_state == FW_RESET_ERROR) {
		IWL_INFO(trans,
			 "firmware didn't ACK the reset - continue anyway\n");
		iwl_trans_fw_error(trans, true);
	}

	trans_pcie->fw_reset_state = FW_RESET_IDLE;
}

void _iwl_trans_pcie_gen2_stop_device(struct iwl_trans *trans)
>>>>>>> upstream/android-13
{
	struct iwl_trans_pcie *trans_pcie = IWL_TRANS_GET_PCIE_TRANS(trans);

	lockdep_assert_held(&trans_pcie->mutex);

	if (trans_pcie->is_down)
		return;

<<<<<<< HEAD
	trans_pcie->is_down = true;

	/* Stop dbgc before stopping device */
	iwl_write_prph(trans, DBGC_IN_SAMPLE, 0);
	udelay(100);
	iwl_write_prph(trans, DBGC_OUT_CTRL, 0);
=======
	if (trans->state >= IWL_TRANS_FW_STARTED) {
		if (trans->trans_cfg->device_family >= IWL_DEVICE_FAMILY_BZ) {
			iwl_set_bit(trans, CSR_GP_CNTRL,
				    CSR_GP_CNTRL_REG_FLAG_BUS_MASTER_DISABLE_REQ);
			iwl_poll_bit(trans, CSR_GP_CNTRL,
				     CSR_GP_CNTRL_REG_FLAG_BUS_MASTER_DISABLE_STATUS,
				     CSR_GP_CNTRL_REG_FLAG_BUS_MASTER_DISABLE_STATUS,
				     5000);
			msleep(100);
			iwl_set_bit(trans, CSR_GP_CNTRL,
				    CSR_GP_CNTRL_REG_FLAG_SW_RESET);
		} else if (trans_pcie->fw_reset_handshake) {
			iwl_trans_pcie_fw_reset_handshake(trans);
		}
	}

	trans_pcie->is_down = true;
>>>>>>> upstream/android-13

	/* tell the device to stop sending interrupts */
	iwl_disable_interrupts(trans);

	/* device going down, Stop using ICT table */
	iwl_pcie_disable_ict(trans);

	/*
	 * If a HW restart happens during firmware loading,
	 * then the firmware loading might call this function
	 * and later it might be called again due to the
	 * restart. So don't process again if the device is
	 * already dead.
	 */
	if (test_and_clear_bit(STATUS_DEVICE_ENABLED, &trans->status)) {
		IWL_DEBUG_INFO(trans,
			       "DEVICE_ENABLED bit was set and is now cleared\n");
<<<<<<< HEAD
		iwl_pcie_gen2_tx_stop(trans);
=======
		iwl_txq_gen2_tx_free(trans);
>>>>>>> upstream/android-13
		iwl_pcie_rx_stop(trans);
	}

	iwl_pcie_ctxt_info_free_paging(trans);
<<<<<<< HEAD
	if (trans->cfg->device_family == IWL_DEVICE_FAMILY_22560)
		iwl_pcie_ctxt_info_gen3_free(trans);
=======
	if (trans->trans_cfg->device_family >= IWL_DEVICE_FAMILY_AX210)
		iwl_pcie_ctxt_info_gen3_free(trans, false);
>>>>>>> upstream/android-13
	else
		iwl_pcie_ctxt_info_free(trans);

	/* Make sure (redundant) we've released our request to stay awake */
<<<<<<< HEAD
	iwl_clear_bit(trans, CSR_GP_CNTRL,
		      BIT(trans->cfg->csr->flag_mac_access_req));

=======
	if (trans->trans_cfg->device_family >= IWL_DEVICE_FAMILY_BZ)
		iwl_clear_bit(trans, CSR_GP_CNTRL,
			      CSR_GP_CNTRL_REG_FLAG_BZ_MAC_ACCESS_REQ);
	else
		iwl_clear_bit(trans, CSR_GP_CNTRL,
			      CSR_GP_CNTRL_REG_FLAG_MAC_ACCESS_REQ);

	if (trans->trans_cfg->device_family >= IWL_DEVICE_FAMILY_BZ) {
		iwl_set_bit(trans, CSR_GP_CNTRL,
			    CSR_GP_CNTRL_REG_FLAG_SW_RESET);
	}
>>>>>>> upstream/android-13
	/* Stop the device, and put it in low power state */
	iwl_pcie_gen2_apm_stop(trans, false);

	iwl_trans_sw_reset(trans);

	/*
	 * Upon stop, the IVAR table gets erased, so msi-x won't
	 * work. This causes a bug in RF-KILL flows, since the interrupt
	 * that enables radio won't fire on the correct irq, and the
	 * driver won't be able to handle the interrupt.
	 * Configure the IVAR table again after reset.
	 */
	iwl_pcie_conf_msix_hw(trans_pcie);

	/*
	 * Upon stop, the APM issues an interrupt if HW RF kill is set.
	 * This is a bug in certain verions of the hardware.
	 * Certain devices also keep sending HW RF kill interrupt all
	 * the time, unless the interrupt is ACKed even if the interrupt
	 * should be masked. Re-ACK all the interrupts here.
	 */
	iwl_disable_interrupts(trans);

	/* clear all status bits */
	clear_bit(STATUS_SYNC_HCMD_ACTIVE, &trans->status);
	clear_bit(STATUS_INT_ENABLED, &trans->status);
	clear_bit(STATUS_TPOWER_PMI, &trans->status);

	/*
	 * Even if we stop the HW, we still want the RF kill
	 * interrupt
	 */
	iwl_enable_rfkill_int(trans);

	/* re-take ownership to prevent other users from stealing the device */
	iwl_pcie_prepare_card_hw(trans);
}

<<<<<<< HEAD
void iwl_trans_pcie_gen2_stop_device(struct iwl_trans *trans, bool low_power)
=======
void iwl_trans_pcie_gen2_stop_device(struct iwl_trans *trans)
>>>>>>> upstream/android-13
{
	struct iwl_trans_pcie *trans_pcie = IWL_TRANS_GET_PCIE_TRANS(trans);
	bool was_in_rfkill;

<<<<<<< HEAD
	mutex_lock(&trans_pcie->mutex);
	trans_pcie->opmode_down = true;
	was_in_rfkill = test_bit(STATUS_RFKILL_OPMODE, &trans->status);
	_iwl_trans_pcie_gen2_stop_device(trans, low_power);
=======
	iwl_op_mode_time_point(trans->op_mode,
			       IWL_FW_INI_TIME_POINT_HOST_DEVICE_DISABLE,
			       NULL);

	mutex_lock(&trans_pcie->mutex);
	trans_pcie->opmode_down = true;
	was_in_rfkill = test_bit(STATUS_RFKILL_OPMODE, &trans->status);
	_iwl_trans_pcie_gen2_stop_device(trans);
>>>>>>> upstream/android-13
	iwl_trans_pcie_handle_stop_rfkill(trans, was_in_rfkill);
	mutex_unlock(&trans_pcie->mutex);
}

static int iwl_pcie_gen2_nic_init(struct iwl_trans *trans)
{
	struct iwl_trans_pcie *trans_pcie = IWL_TRANS_GET_PCIE_TRANS(trans);
<<<<<<< HEAD

	/* TODO: most of the logic can be removed in A0 - but not in Z0 */
	spin_lock(&trans_pcie->irq_lock);
	iwl_pcie_gen2_apm_init(trans);
	spin_unlock(&trans_pcie->irq_lock);
=======
	int queue_size = max_t(u32, IWL_CMD_QUEUE_SIZE,
			       trans->cfg->min_txq_size);

	/* TODO: most of the logic can be removed in A0 - but not in Z0 */
	spin_lock_bh(&trans_pcie->irq_lock);
	iwl_pcie_gen2_apm_init(trans);
	spin_unlock_bh(&trans_pcie->irq_lock);
>>>>>>> upstream/android-13

	iwl_op_mode_nic_config(trans->op_mode);

	/* Allocate the RX queue, or reset if it is already allocated */
	if (iwl_pcie_gen2_rx_init(trans))
		return -ENOMEM;

	/* Allocate or reset and init all Tx and Command queues */
<<<<<<< HEAD
	if (iwl_pcie_gen2_tx_init(trans))
=======
	if (iwl_txq_gen2_init(trans, trans->txqs.cmd.q_id, queue_size))
>>>>>>> upstream/android-13
		return -ENOMEM;

	/* enable shadow regs in HW */
	iwl_set_bit(trans, CSR_MAC_SHADOW_REG_CTRL, 0x800FFFFF);
	IWL_DEBUG_INFO(trans, "Enabling shadow registers in device\n");

	return 0;
}

<<<<<<< HEAD
=======
static void iwl_pcie_get_rf_name(struct iwl_trans *trans)
{
	struct iwl_trans_pcie *trans_pcie = IWL_TRANS_GET_PCIE_TRANS(trans);
	char *buf = trans_pcie->rf_name;
	size_t buflen = sizeof(trans_pcie->rf_name);
	size_t pos;
	u32 version;

	if (buf[0])
		return;

	switch (CSR_HW_RFID_TYPE(trans->hw_rf_id)) {
	case CSR_HW_RFID_TYPE(CSR_HW_RF_ID_TYPE_JF):
		pos = scnprintf(buf, buflen, "JF");
		break;
	case CSR_HW_RFID_TYPE(CSR_HW_RF_ID_TYPE_GF):
		pos = scnprintf(buf, buflen, "GF");
		break;
	case CSR_HW_RFID_TYPE(CSR_HW_RF_ID_TYPE_GF4):
		pos = scnprintf(buf, buflen, "GF4");
		break;
	case CSR_HW_RFID_TYPE(CSR_HW_RF_ID_TYPE_HR):
		pos = scnprintf(buf, buflen, "HR");
		break;
	case CSR_HW_RFID_TYPE(CSR_HW_RF_ID_TYPE_HR1):
		pos = scnprintf(buf, buflen, "HR1");
		break;
	case CSR_HW_RFID_TYPE(CSR_HW_RF_ID_TYPE_HRCDB):
		pos = scnprintf(buf, buflen, "HRCDB");
		break;
	default:
		return;
	}

	switch (CSR_HW_RFID_TYPE(trans->hw_rf_id)) {
	case CSR_HW_RFID_TYPE(CSR_HW_RF_ID_TYPE_HR):
	case CSR_HW_RFID_TYPE(CSR_HW_RF_ID_TYPE_HR1):
	case CSR_HW_RFID_TYPE(CSR_HW_RF_ID_TYPE_HRCDB):
		version = iwl_read_prph(trans, CNVI_MBOX_C);
		switch (version) {
		case 0x20000:
			pos += scnprintf(buf + pos, buflen - pos, " B3");
			break;
		case 0x120000:
			pos += scnprintf(buf + pos, buflen - pos, " B5");
			break;
		default:
			pos += scnprintf(buf + pos, buflen - pos,
					 " (0x%x)", version);
			break;
		}
		break;
	default:
		break;
	}

	pos += scnprintf(buf + pos, buflen - pos, ", rfid=0x%x",
			 trans->hw_rf_id);

	IWL_INFO(trans, "Detected RF %s\n", buf);

	/*
	 * also add a \n for debugfs - need to do it after printing
	 * since our IWL_INFO machinery wants to see a static \n at
	 * the end of the string
	 */
	pos += scnprintf(buf + pos, buflen - pos, "\n");
}

>>>>>>> upstream/android-13
void iwl_trans_pcie_gen2_fw_alive(struct iwl_trans *trans, u32 scd_addr)
{
	struct iwl_trans_pcie *trans_pcie = IWL_TRANS_GET_PCIE_TRANS(trans);

	iwl_pcie_reset_ict(trans);

	/* make sure all queue are not stopped/used */
<<<<<<< HEAD
	memset(trans_pcie->queue_stopped, 0, sizeof(trans_pcie->queue_stopped));
	memset(trans_pcie->queue_used, 0, sizeof(trans_pcie->queue_used));
=======
	memset(trans->txqs.queue_stopped, 0,
	       sizeof(trans->txqs.queue_stopped));
	memset(trans->txqs.queue_used, 0, sizeof(trans->txqs.queue_used));
>>>>>>> upstream/android-13

	/* now that we got alive we can free the fw image & the context info.
	 * paging memory cannot be freed included since FW will still use it
	 */
<<<<<<< HEAD
	iwl_pcie_ctxt_info_free(trans);
=======
	if (trans->trans_cfg->device_family >= IWL_DEVICE_FAMILY_AX210)
		iwl_pcie_ctxt_info_gen3_free(trans, true);
	else
		iwl_pcie_ctxt_info_free(trans);
>>>>>>> upstream/android-13

	/*
	 * Re-enable all the interrupts, including the RF-Kill one, now that
	 * the firmware is alive.
	 */
	iwl_enable_interrupts(trans);
	mutex_lock(&trans_pcie->mutex);
	iwl_pcie_check_hw_rf_kill(trans);
<<<<<<< HEAD
	mutex_unlock(&trans_pcie->mutex);
}

=======

	iwl_pcie_get_rf_name(trans);
	mutex_unlock(&trans_pcie->mutex);
}

static void iwl_pcie_set_ltr(struct iwl_trans *trans)
{
	u32 ltr_val = CSR_LTR_LONG_VAL_AD_NO_SNOOP_REQ |
		      u32_encode_bits(CSR_LTR_LONG_VAL_AD_SCALE_USEC,
				      CSR_LTR_LONG_VAL_AD_NO_SNOOP_SCALE) |
		      u32_encode_bits(250,
				      CSR_LTR_LONG_VAL_AD_NO_SNOOP_VAL) |
		      CSR_LTR_LONG_VAL_AD_SNOOP_REQ |
		      u32_encode_bits(CSR_LTR_LONG_VAL_AD_SCALE_USEC,
				      CSR_LTR_LONG_VAL_AD_SNOOP_SCALE) |
		      u32_encode_bits(250, CSR_LTR_LONG_VAL_AD_SNOOP_VAL);

	/*
	 * To workaround hardware latency issues during the boot process,
	 * initialize the LTR to ~250 usec (see ltr_val above).
	 * The firmware initializes this again later (to a smaller value).
	 */
	if ((trans->trans_cfg->device_family == IWL_DEVICE_FAMILY_AX210 ||
	     trans->trans_cfg->device_family == IWL_DEVICE_FAMILY_22000) &&
	    !trans->trans_cfg->integrated) {
		iwl_write32(trans, CSR_LTR_LONG_VAL_AD, ltr_val);
	} else if (trans->trans_cfg->integrated &&
		   trans->trans_cfg->device_family == IWL_DEVICE_FAMILY_22000) {
		iwl_write_prph(trans, HPM_MAC_LTR_CSR, HPM_MAC_LRT_ENABLE_ALL);
		iwl_write_prph(trans, HPM_UMAC_LTR, ltr_val);
	}
}

>>>>>>> upstream/android-13
int iwl_trans_pcie_gen2_start_fw(struct iwl_trans *trans,
				 const struct fw_img *fw, bool run_in_rfkill)
{
	struct iwl_trans_pcie *trans_pcie = IWL_TRANS_GET_PCIE_TRANS(trans);
	bool hw_rfkill;
	int ret;

	/* This may fail if AMT took ownership of the device */
	if (iwl_pcie_prepare_card_hw(trans)) {
		IWL_WARN(trans, "Exit HW not ready\n");
<<<<<<< HEAD
		ret = -EIO;
		goto out;
=======
		return -EIO;
>>>>>>> upstream/android-13
	}

	iwl_enable_rfkill_int(trans);

	iwl_write32(trans, CSR_INT, 0xFFFFFFFF);

	/*
	 * We enabled the RF-Kill interrupt and the handler may very
	 * well be running. Disable the interrupts to make sure no other
	 * interrupt can be fired.
	 */
	iwl_disable_interrupts(trans);

	/* Make sure it finished running */
	iwl_pcie_synchronize_irqs(trans);

	mutex_lock(&trans_pcie->mutex);

	/* If platform's RF_KILL switch is NOT set to KILL */
	hw_rfkill = iwl_pcie_check_hw_rf_kill(trans);
	if (hw_rfkill && !run_in_rfkill) {
		ret = -ERFKILL;
		goto out;
	}

	/* Someone called stop_device, don't try to start_fw */
	if (trans_pcie->is_down) {
		IWL_WARN(trans,
			 "Can't start_fw since the HW hasn't been started\n");
		ret = -EIO;
		goto out;
	}

	/* make sure rfkill handshake bits are cleared */
	iwl_write32(trans, CSR_UCODE_DRV_GP1_CLR, CSR_UCODE_SW_BIT_RFKILL);
	iwl_write32(trans, CSR_UCODE_DRV_GP1_CLR,
		    CSR_UCODE_DRV_GP1_BIT_CMD_BLOCKED);

	/* clear (again), then enable host interrupts */
	iwl_write32(trans, CSR_INT, 0xFFFFFFFF);

	ret = iwl_pcie_gen2_nic_init(trans);
	if (ret) {
		IWL_ERR(trans, "Unable to init nic\n");
		goto out;
	}

<<<<<<< HEAD
	if (trans->cfg->device_family == IWL_DEVICE_FAMILY_22560)
=======
	if (trans->trans_cfg->device_family >= IWL_DEVICE_FAMILY_AX210)
>>>>>>> upstream/android-13
		ret = iwl_pcie_ctxt_info_gen3_init(trans, fw);
	else
		ret = iwl_pcie_ctxt_info_init(trans, fw);
	if (ret)
		goto out;

<<<<<<< HEAD
=======
	iwl_pcie_set_ltr(trans);

	if (trans->trans_cfg->device_family >= IWL_DEVICE_FAMILY_BZ)
		iwl_set_bit(trans, CSR_GP_CNTRL,
			    CSR_GP_CNTRL_REG_FLAG_ROM_START);
	else if (trans->trans_cfg->device_family >= IWL_DEVICE_FAMILY_AX210)
		iwl_write_umac_prph(trans, UREG_CPU_INIT_RUN, 1);
	else
		iwl_write_prph(trans, UREG_CPU_INIT_RUN, 1);

>>>>>>> upstream/android-13
	/* re-check RF-Kill state since we may have missed the interrupt */
	hw_rfkill = iwl_pcie_check_hw_rf_kill(trans);
	if (hw_rfkill && !run_in_rfkill)
		ret = -ERFKILL;

out:
	mutex_unlock(&trans_pcie->mutex);
	return ret;
}
