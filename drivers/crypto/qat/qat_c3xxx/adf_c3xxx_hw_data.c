<<<<<<< HEAD
/*
  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY
  Copyright(c) 2014 Intel Corporation.
  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  qat-linux@intel.com

  BSD LICENSE
  Copyright(c) 2014 Intel Corporation.
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in
	  the documentation and/or other materials provided with the
	  distribution.
	* Neither the name of Intel Corporation nor the names of its
	  contributors may be used to endorse or promote products derived
	  from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <adf_accel_devices.h>
#include <adf_common_drv.h>
#include <adf_pf2vf_msg.h>
#include "adf_c3xxx_hw_data.h"

/* Worker thread to service arbiter mappings based on dev SKUs */
static const u32 thrd_to_arb_map_6_me_sku[] = {
=======
// SPDX-License-Identifier: (BSD-3-Clause OR GPL-2.0-only)
/* Copyright(c) 2014 - 2020 Intel Corporation */
#include <adf_accel_devices.h>
#include <adf_common_drv.h>
#include <adf_pf2vf_msg.h>
#include <adf_gen2_hw_data.h>
#include "adf_c3xxx_hw_data.h"
#include "icp_qat_hw.h"

/* Worker thread to service arbiter mappings */
static const u32 thrd_to_arb_map[ADF_C3XXX_MAX_ACCELENGINES] = {
>>>>>>> upstream/android-13
	0x12222AAA, 0x11222AAA, 0x12222AAA,
	0x11222AAA, 0x12222AAA, 0x11222AAA
};

static struct adf_hw_device_class c3xxx_class = {
	.name = ADF_C3XXX_DEVICE_NAME,
	.type = DEV_C3XXX,
	.instances = 0
};

<<<<<<< HEAD
static u32 get_accel_mask(u32 fuse)
{
	return (~fuse) >> ADF_C3XXX_ACCELERATORS_REG_OFFSET &
		ADF_C3XXX_ACCELERATORS_MASK;
}

static u32 get_ae_mask(u32 fuse)
{
	return (~fuse) & ADF_C3XXX_ACCELENGINES_MASK;
=======
static u32 get_accel_mask(struct adf_hw_device_data *self)
{
	u32 straps = self->straps;
	u32 fuses = self->fuses;
	u32 accel;

	accel = ~(fuses | straps) >> ADF_C3XXX_ACCELERATORS_REG_OFFSET;
	accel &= ADF_C3XXX_ACCELERATORS_MASK;

	return accel;
}

static u32 get_ae_mask(struct adf_hw_device_data *self)
{
	u32 straps = self->straps;
	u32 fuses = self->fuses;
	unsigned long disabled;
	u32 ae_disable;
	int accel;

	/* If an accel is disabled, then disable the corresponding two AEs */
	disabled = ~get_accel_mask(self) & ADF_C3XXX_ACCELERATORS_MASK;
	ae_disable = BIT(1) | BIT(0);
	for_each_set_bit(accel, &disabled, ADF_C3XXX_MAX_ACCELERATORS)
		straps |= ae_disable << (accel << 1);

	return ~(fuses | straps) & ADF_C3XXX_ACCELENGINES_MASK;
>>>>>>> upstream/android-13
}

static u32 get_num_accels(struct adf_hw_device_data *self)
{
	u32 i, ctr = 0;

	if (!self || !self->accel_mask)
		return 0;

	for (i = 0; i < ADF_C3XXX_MAX_ACCELERATORS; i++) {
		if (self->accel_mask & (1 << i))
			ctr++;
	}
	return ctr;
}

static u32 get_num_aes(struct adf_hw_device_data *self)
{
	u32 i, ctr = 0;

	if (!self || !self->ae_mask)
		return 0;

	for (i = 0; i < ADF_C3XXX_MAX_ACCELENGINES; i++) {
		if (self->ae_mask & (1 << i))
			ctr++;
	}
	return ctr;
}

static u32 get_misc_bar_id(struct adf_hw_device_data *self)
{
	return ADF_C3XXX_PMISC_BAR;
}

static u32 get_etr_bar_id(struct adf_hw_device_data *self)
{
	return ADF_C3XXX_ETR_BAR;
}

static u32 get_sram_bar_id(struct adf_hw_device_data *self)
{
	return 0;
}

static enum dev_sku_info get_sku(struct adf_hw_device_data *self)
{
	int aes = get_num_aes(self);

	if (aes == 6)
		return DEV_SKU_4;

	return DEV_SKU_UNKNOWN;
}

<<<<<<< HEAD
static void adf_get_arbiter_mapping(struct adf_accel_dev *accel_dev,
				    u32 const **arb_map_config)
{
	switch (accel_dev->accel_pci_dev.sku) {
	case DEV_SKU_4:
		*arb_map_config = thrd_to_arb_map_6_me_sku;
		break;
	default:
		dev_err(&GET_DEV(accel_dev),
			"The configuration doesn't match any SKU");
		*arb_map_config = NULL;
	}
=======
static const u32 *adf_get_arbiter_mapping(void)
{
	return thrd_to_arb_map;
>>>>>>> upstream/android-13
}

static u32 get_pf2vf_offset(u32 i)
{
	return ADF_C3XXX_PF2VF_OFFSET(i);
}

<<<<<<< HEAD
static u32 get_vintmsk_offset(u32 i)
{
	return ADF_C3XXX_VINTMSK_OFFSET(i);
}

=======
>>>>>>> upstream/android-13
static void adf_enable_error_correction(struct adf_accel_dev *accel_dev)
{
	struct adf_hw_device_data *hw_device = accel_dev->hw_device;
	struct adf_bar *misc_bar = &GET_BARS(accel_dev)[ADF_C3XXX_PMISC_BAR];
<<<<<<< HEAD
=======
	unsigned long accel_mask = hw_device->accel_mask;
	unsigned long ae_mask = hw_device->ae_mask;
>>>>>>> upstream/android-13
	void __iomem *csr = misc_bar->virt_addr;
	unsigned int val, i;

	/* Enable Accel Engine error detection & correction */
<<<<<<< HEAD
	for (i = 0; i < hw_device->get_num_aes(hw_device); i++) {
=======
	for_each_set_bit(i, &ae_mask, GET_MAX_ACCELENGINES(accel_dev)) {
>>>>>>> upstream/android-13
		val = ADF_CSR_RD(csr, ADF_C3XXX_AE_CTX_ENABLES(i));
		val |= ADF_C3XXX_ENABLE_AE_ECC_ERR;
		ADF_CSR_WR(csr, ADF_C3XXX_AE_CTX_ENABLES(i), val);
		val = ADF_CSR_RD(csr, ADF_C3XXX_AE_MISC_CONTROL(i));
		val |= ADF_C3XXX_ENABLE_AE_ECC_PARITY_CORR;
		ADF_CSR_WR(csr, ADF_C3XXX_AE_MISC_CONTROL(i), val);
	}

	/* Enable shared memory error detection & correction */
<<<<<<< HEAD
	for (i = 0; i < hw_device->get_num_accels(hw_device); i++) {
=======
	for_each_set_bit(i, &accel_mask, ADF_C3XXX_MAX_ACCELERATORS) {
>>>>>>> upstream/android-13
		val = ADF_CSR_RD(csr, ADF_C3XXX_UERRSSMSH(i));
		val |= ADF_C3XXX_ERRSSMSH_EN;
		ADF_CSR_WR(csr, ADF_C3XXX_UERRSSMSH(i), val);
		val = ADF_CSR_RD(csr, ADF_C3XXX_CERRSSMSH(i));
		val |= ADF_C3XXX_ERRSSMSH_EN;
		ADF_CSR_WR(csr, ADF_C3XXX_CERRSSMSH(i), val);
	}
}

static void adf_enable_ints(struct adf_accel_dev *accel_dev)
{
	void __iomem *addr;

	addr = (&GET_BARS(accel_dev)[ADF_C3XXX_PMISC_BAR])->virt_addr;

	/* Enable bundle and misc interrupts */
	ADF_CSR_WR(addr, ADF_C3XXX_SMIAPF0_MASK_OFFSET,
		   ADF_C3XXX_SMIA0_MASK);
	ADF_CSR_WR(addr, ADF_C3XXX_SMIAPF1_MASK_OFFSET,
		   ADF_C3XXX_SMIA1_MASK);
}

<<<<<<< HEAD
static int adf_pf_enable_vf2pf_comms(struct adf_accel_dev *accel_dev)
{
	return 0;
}

=======
static int adf_enable_pf2vf_comms(struct adf_accel_dev *accel_dev)
{
	spin_lock_init(&accel_dev->pf.vf2pf_ints_lock);

	return 0;
}

static void configure_iov_threads(struct adf_accel_dev *accel_dev, bool enable)
{
	adf_gen2_cfg_iov_thds(accel_dev, enable,
			      ADF_C3XXX_AE2FUNC_MAP_GRP_A_NUM_REGS,
			      ADF_C3XXX_AE2FUNC_MAP_GRP_B_NUM_REGS);
}

>>>>>>> upstream/android-13
void adf_init_hw_data_c3xxx(struct adf_hw_device_data *hw_data)
{
	hw_data->dev_class = &c3xxx_class;
	hw_data->instance_id = c3xxx_class.instances++;
	hw_data->num_banks = ADF_C3XXX_ETR_MAX_BANKS;
<<<<<<< HEAD
=======
	hw_data->num_rings_per_bank = ADF_ETR_MAX_RINGS_PER_BANK;
>>>>>>> upstream/android-13
	hw_data->num_accel = ADF_C3XXX_MAX_ACCELERATORS;
	hw_data->num_logical_accel = 1;
	hw_data->num_engines = ADF_C3XXX_MAX_ACCELENGINES;
	hw_data->tx_rx_gap = ADF_C3XXX_RX_RINGS_OFFSET;
	hw_data->tx_rings_mask = ADF_C3XXX_TX_RINGS_MASK;
	hw_data->alloc_irq = adf_isr_resource_alloc;
	hw_data->free_irq = adf_isr_resource_free;
	hw_data->enable_error_correction = adf_enable_error_correction;
	hw_data->get_accel_mask = get_accel_mask;
	hw_data->get_ae_mask = get_ae_mask;
<<<<<<< HEAD
=======
	hw_data->get_accel_cap = adf_gen2_get_accel_cap;
>>>>>>> upstream/android-13
	hw_data->get_num_accels = get_num_accels;
	hw_data->get_num_aes = get_num_aes;
	hw_data->get_sram_bar_id = get_sram_bar_id;
	hw_data->get_etr_bar_id = get_etr_bar_id;
	hw_data->get_misc_bar_id = get_misc_bar_id;
<<<<<<< HEAD
	hw_data->get_pf2vf_offset = get_pf2vf_offset;
	hw_data->get_vintmsk_offset = get_vintmsk_offset;
=======
	hw_data->get_admin_info = adf_gen2_get_admin_info;
	hw_data->get_arb_info = adf_gen2_get_arb_info;
>>>>>>> upstream/android-13
	hw_data->get_sku = get_sku;
	hw_data->fw_name = ADF_C3XXX_FW;
	hw_data->fw_mmp_name = ADF_C3XXX_MMP;
	hw_data->init_admin_comms = adf_init_admin_comms;
	hw_data->exit_admin_comms = adf_exit_admin_comms;
<<<<<<< HEAD
	hw_data->disable_iov = adf_disable_sriov;
=======
	hw_data->configure_iov_threads = configure_iov_threads;
>>>>>>> upstream/android-13
	hw_data->send_admin_init = adf_send_admin_init;
	hw_data->init_arb = adf_init_arb;
	hw_data->exit_arb = adf_exit_arb;
	hw_data->get_arb_mapping = adf_get_arbiter_mapping;
	hw_data->enable_ints = adf_enable_ints;
<<<<<<< HEAD
	hw_data->enable_vf2pf_comms = adf_pf_enable_vf2pf_comms;
	hw_data->reset_device = adf_reset_flr;
	hw_data->min_iov_compat_ver = ADF_PFVF_COMPATIBILITY_VERSION;
=======
	hw_data->reset_device = adf_reset_flr;
	hw_data->set_ssm_wdtimer = adf_gen2_set_ssm_wdtimer;
	hw_data->get_pf2vf_offset = get_pf2vf_offset;
	hw_data->enable_pfvf_comms = adf_enable_pf2vf_comms;
	hw_data->disable_iov = adf_disable_sriov;
	hw_data->min_iov_compat_ver = ADF_PFVF_COMPAT_THIS_VERSION;

	adf_gen2_init_hw_csr_ops(&hw_data->csr_ops);
>>>>>>> upstream/android-13
}

void adf_clean_hw_data_c3xxx(struct adf_hw_device_data *hw_data)
{
	hw_data->dev_class->instances--;
}
