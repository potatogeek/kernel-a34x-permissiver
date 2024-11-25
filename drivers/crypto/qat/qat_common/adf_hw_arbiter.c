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
=======
// SPDX-License-Identifier: (BSD-3-Clause OR GPL-2.0-only)
/* Copyright(c) 2014 - 2020 Intel Corporation */
>>>>>>> upstream/android-13
#include "adf_accel_devices.h"
#include "adf_common_drv.h"
#include "adf_transport_internal.h"

#define ADF_ARB_NUM 4
#define ADF_ARB_REG_SIZE 0x4
<<<<<<< HEAD
#define ADF_ARB_WTR_SIZE 0x20
#define ADF_ARB_OFFSET 0x30000
#define ADF_ARB_REG_SLOT 0x1000
#define ADF_ARB_WTR_OFFSET 0x010
#define ADF_ARB_RO_EN_OFFSET 0x090
#define ADF_ARB_WQCFG_OFFSET 0x100
#define ADF_ARB_WRK_2_SER_MAP_OFFSET 0x180
#define ADF_ARB_RINGSRVARBEN_OFFSET 0x19C

#define WRITE_CSR_ARB_RINGSRVARBEN(csr_addr, index, value) \
	ADF_CSR_WR(csr_addr, ADF_ARB_RINGSRVARBEN_OFFSET + \
	(ADF_ARB_REG_SLOT * index), value)

#define WRITE_CSR_ARB_SARCONFIG(csr_addr, index, value) \
	ADF_CSR_WR(csr_addr, ADF_ARB_OFFSET + \
	(ADF_ARB_REG_SIZE * index), value)

#define WRITE_CSR_ARB_WRK_2_SER_MAP(csr_addr, index, value) \
	ADF_CSR_WR(csr_addr, (ADF_ARB_OFFSET + \
	ADF_ARB_WRK_2_SER_MAP_OFFSET) + \
	(ADF_ARB_REG_SIZE * index), value)

#define WRITE_CSR_ARB_WQCFG(csr_addr, index, value) \
	ADF_CSR_WR(csr_addr, (ADF_ARB_OFFSET + \
	ADF_ARB_WQCFG_OFFSET) + (ADF_ARB_REG_SIZE * index), value)
=======

#define WRITE_CSR_ARB_SARCONFIG(csr_addr, arb_offset, index, value) \
	ADF_CSR_WR(csr_addr, (arb_offset) + \
	(ADF_ARB_REG_SIZE * (index)), value)

#define WRITE_CSR_ARB_WT2SAM(csr_addr, arb_offset, wt_offset, index, value) \
	ADF_CSR_WR(csr_addr, ((arb_offset) + (wt_offset)) + \
	(ADF_ARB_REG_SIZE * (index)), value)
>>>>>>> upstream/android-13

int adf_init_arb(struct adf_accel_dev *accel_dev)
{
	struct adf_hw_device_data *hw_data = accel_dev->hw_device;
	void __iomem *csr = accel_dev->transport->banks[0].csr_addr;
<<<<<<< HEAD
	u32 arb_cfg = 0x1 << 31 | 0x4 << 4 | 0x1;
	u32 arb, i;
	const u32 *thd_2_arb_cfg;
=======
	unsigned long ae_mask = hw_data->ae_mask;
	u32 arb_off, wt_off, arb_cfg;
	const u32 *thd_2_arb_cfg;
	struct arb_info info;
	int arb, i;

	hw_data->get_arb_info(&info);
	arb_cfg = info.arb_cfg;
	arb_off = info.arb_offset;
	wt_off = info.wt2sam_offset;
>>>>>>> upstream/android-13

	/* Service arb configured for 32 bytes responses and
	 * ring flow control check enabled. */
	for (arb = 0; arb < ADF_ARB_NUM; arb++)
<<<<<<< HEAD
		WRITE_CSR_ARB_SARCONFIG(csr, arb, arb_cfg);

	/* Setup worker queue registers */
	for (i = 0; i < hw_data->num_engines; i++)
		WRITE_CSR_ARB_WQCFG(csr, i, i);

	/* Map worker threads to service arbiters */
	hw_data->get_arb_mapping(accel_dev, &thd_2_arb_cfg);

	if (!thd_2_arb_cfg)
		return -EFAULT;

	for (i = 0; i < hw_data->num_engines; i++)
		WRITE_CSR_ARB_WRK_2_SER_MAP(csr, i, *(thd_2_arb_cfg + i));
=======
		WRITE_CSR_ARB_SARCONFIG(csr, arb_off, arb, arb_cfg);

	/* Map worker threads to service arbiters */
	thd_2_arb_cfg = hw_data->get_arb_mapping();

	for_each_set_bit(i, &ae_mask, hw_data->num_engines)
		WRITE_CSR_ARB_WT2SAM(csr, arb_off, wt_off, i, thd_2_arb_cfg[i]);
>>>>>>> upstream/android-13

	return 0;
}
EXPORT_SYMBOL_GPL(adf_init_arb);

void adf_update_ring_arb(struct adf_etr_ring_data *ring)
{
<<<<<<< HEAD
	WRITE_CSR_ARB_RINGSRVARBEN(ring->bank->csr_addr,
				   ring->bank->bank_number,
				   ring->bank->ring_mask & 0xFF);
=======
	struct adf_accel_dev *accel_dev = ring->bank->accel_dev;
	struct adf_hw_device_data *hw_data = accel_dev->hw_device;
	struct adf_hw_csr_ops *csr_ops = GET_CSR_OPS(accel_dev);
	u32 tx_ring_mask = hw_data->tx_rings_mask;
	u32 shift = hw_data->tx_rx_gap;
	u32 arben, arben_tx, arben_rx;
	u32 rx_ring_mask;

	/*
	 * Enable arbitration on a ring only if the TX half of the ring mask
	 * matches the RX part. This results in writes to CSR on both TX and
	 * RX update - only one is necessary, but both are done for
	 * simplicity.
	 */
	rx_ring_mask = tx_ring_mask << shift;
	arben_tx = (ring->bank->ring_mask & tx_ring_mask) >> 0;
	arben_rx = (ring->bank->ring_mask & rx_ring_mask) >> shift;
	arben = arben_tx & arben_rx;

	csr_ops->write_csr_ring_srv_arb_en(ring->bank->csr_addr,
					   ring->bank->bank_number, arben);
>>>>>>> upstream/android-13
}

void adf_exit_arb(struct adf_accel_dev *accel_dev)
{
	struct adf_hw_device_data *hw_data = accel_dev->hw_device;
<<<<<<< HEAD
	void __iomem *csr;
	unsigned int i;

=======
	struct adf_hw_csr_ops *csr_ops = GET_CSR_OPS(accel_dev);
	u32 arb_off, wt_off;
	struct arb_info info;
	void __iomem *csr;
	unsigned int i;

	hw_data->get_arb_info(&info);
	arb_off = info.arb_offset;
	wt_off = info.wt2sam_offset;

>>>>>>> upstream/android-13
	if (!accel_dev->transport)
		return;

	csr = accel_dev->transport->banks[0].csr_addr;

<<<<<<< HEAD
	/* Reset arbiter configuration */
	for (i = 0; i < ADF_ARB_NUM; i++)
		WRITE_CSR_ARB_SARCONFIG(csr, i, 0);

	/* Shutdown work queue */
	for (i = 0; i < hw_data->num_engines; i++)
		WRITE_CSR_ARB_WQCFG(csr, i, 0);

	/* Unmap worker threads to service arbiters */
	for (i = 0; i < hw_data->num_engines; i++)
		WRITE_CSR_ARB_WRK_2_SER_MAP(csr, i, 0);

	/* Disable arbitration on all rings */
	for (i = 0; i < GET_MAX_BANKS(accel_dev); i++)
		WRITE_CSR_ARB_RINGSRVARBEN(csr, i, 0);
=======
	hw_data->get_arb_info(&info);

	/* Reset arbiter configuration */
	for (i = 0; i < ADF_ARB_NUM; i++)
		WRITE_CSR_ARB_SARCONFIG(csr, arb_off, i, 0);

	/* Unmap worker threads to service arbiters */
	for (i = 0; i < hw_data->num_engines; i++)
		WRITE_CSR_ARB_WT2SAM(csr, arb_off, wt_off, i, 0);

	/* Disable arbitration on all rings */
	for (i = 0; i < GET_MAX_BANKS(accel_dev); i++)
		csr_ops->write_csr_ring_srv_arb_en(csr, i, 0);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(adf_exit_arb);
