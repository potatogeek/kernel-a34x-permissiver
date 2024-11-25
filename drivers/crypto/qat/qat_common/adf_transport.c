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
#include <linux/delay.h>
=======
// SPDX-License-Identifier: (BSD-3-Clause OR GPL-2.0-only)
/* Copyright(c) 2014 - 2020 Intel Corporation */
#include <linux/delay.h>
#include <linux/nospec.h>
>>>>>>> upstream/android-13
#include "adf_accel_devices.h"
#include "adf_transport_internal.h"
#include "adf_transport_access_macros.h"
#include "adf_cfg.h"
#include "adf_common_drv.h"

<<<<<<< HEAD
static inline uint32_t adf_modulo(uint32_t data, uint32_t shift)
{
	uint32_t div = data >> shift;
	uint32_t mult = div << shift;
=======
static inline u32 adf_modulo(u32 data, u32 shift)
{
	u32 div = data >> shift;
	u32 mult = div << shift;
>>>>>>> upstream/android-13

	return data - mult;
}

<<<<<<< HEAD
static inline int adf_check_ring_alignment(uint64_t addr, uint64_t size)
=======
static inline int adf_check_ring_alignment(u64 addr, u64 size)
>>>>>>> upstream/android-13
{
	if (((size - 1) & addr) != 0)
		return -EFAULT;
	return 0;
}

<<<<<<< HEAD
static int adf_verify_ring_size(uint32_t msg_size, uint32_t msg_num)
=======
static int adf_verify_ring_size(u32 msg_size, u32 msg_num)
>>>>>>> upstream/android-13
{
	int i = ADF_MIN_RING_SIZE;

	for (; i <= ADF_MAX_RING_SIZE; i++)
		if ((msg_size * msg_num) == ADF_SIZE_TO_RING_SIZE_IN_BYTES(i))
			return i;

	return ADF_DEFAULT_RING_SIZE;
}

<<<<<<< HEAD
static int adf_reserve_ring(struct adf_etr_bank_data *bank, uint32_t ring)
=======
static int adf_reserve_ring(struct adf_etr_bank_data *bank, u32 ring)
>>>>>>> upstream/android-13
{
	spin_lock(&bank->lock);
	if (bank->ring_mask & (1 << ring)) {
		spin_unlock(&bank->lock);
		return -EFAULT;
	}
	bank->ring_mask |= (1 << ring);
	spin_unlock(&bank->lock);
	return 0;
}

<<<<<<< HEAD
static void adf_unreserve_ring(struct adf_etr_bank_data *bank, uint32_t ring)
=======
static void adf_unreserve_ring(struct adf_etr_bank_data *bank, u32 ring)
>>>>>>> upstream/android-13
{
	spin_lock(&bank->lock);
	bank->ring_mask &= ~(1 << ring);
	spin_unlock(&bank->lock);
}

<<<<<<< HEAD
static void adf_enable_ring_irq(struct adf_etr_bank_data *bank, uint32_t ring)
{
	spin_lock_bh(&bank->lock);
	bank->irq_mask |= (1 << ring);
	spin_unlock_bh(&bank->lock);
	WRITE_CSR_INT_COL_EN(bank->csr_addr, bank->bank_number, bank->irq_mask);
	WRITE_CSR_INT_COL_CTL(bank->csr_addr, bank->bank_number,
			      bank->irq_coalesc_timer);
}

static void adf_disable_ring_irq(struct adf_etr_bank_data *bank, uint32_t ring)
{
	spin_lock_bh(&bank->lock);
	bank->irq_mask &= ~(1 << ring);
	spin_unlock_bh(&bank->lock);
	WRITE_CSR_INT_COL_EN(bank->csr_addr, bank->bank_number, bank->irq_mask);
}

int adf_send_message(struct adf_etr_ring_data *ring, uint32_t *msg)
{
=======
static void adf_enable_ring_irq(struct adf_etr_bank_data *bank, u32 ring)
{
	struct adf_hw_csr_ops *csr_ops = GET_CSR_OPS(bank->accel_dev);

	spin_lock_bh(&bank->lock);
	bank->irq_mask |= (1 << ring);
	spin_unlock_bh(&bank->lock);
	csr_ops->write_csr_int_col_en(bank->csr_addr, bank->bank_number,
				      bank->irq_mask);
	csr_ops->write_csr_int_col_ctl(bank->csr_addr, bank->bank_number,
				       bank->irq_coalesc_timer);
}

static void adf_disable_ring_irq(struct adf_etr_bank_data *bank, u32 ring)
{
	struct adf_hw_csr_ops *csr_ops = GET_CSR_OPS(bank->accel_dev);

	spin_lock_bh(&bank->lock);
	bank->irq_mask &= ~(1 << ring);
	spin_unlock_bh(&bank->lock);
	csr_ops->write_csr_int_col_en(bank->csr_addr, bank->bank_number,
				      bank->irq_mask);
}

int adf_send_message(struct adf_etr_ring_data *ring, u32 *msg)
{
	struct adf_hw_csr_ops *csr_ops = GET_CSR_OPS(ring->bank->accel_dev);

>>>>>>> upstream/android-13
	if (atomic_add_return(1, ring->inflights) >
	    ADF_MAX_INFLIGHTS(ring->ring_size, ring->msg_size)) {
		atomic_dec(ring->inflights);
		return -EAGAIN;
	}
	spin_lock_bh(&ring->lock);
	memcpy((void *)((uintptr_t)ring->base_addr + ring->tail), msg,
	       ADF_MSG_SIZE_TO_BYTES(ring->msg_size));

	ring->tail = adf_modulo(ring->tail +
				ADF_MSG_SIZE_TO_BYTES(ring->msg_size),
				ADF_RING_SIZE_MODULO(ring->ring_size));
<<<<<<< HEAD
	WRITE_CSR_RING_TAIL(ring->bank->csr_addr, ring->bank->bank_number,
			    ring->ring_number, ring->tail);
	spin_unlock_bh(&ring->lock);
=======
	csr_ops->write_csr_ring_tail(ring->bank->csr_addr,
				     ring->bank->bank_number, ring->ring_number,
				     ring->tail);
	spin_unlock_bh(&ring->lock);

>>>>>>> upstream/android-13
	return 0;
}

static int adf_handle_response(struct adf_etr_ring_data *ring)
{
<<<<<<< HEAD
	uint32_t msg_counter = 0;
	uint32_t *msg = (uint32_t *)((uintptr_t)ring->base_addr + ring->head);

	while (*msg != ADF_RING_EMPTY_SIG) {
		ring->callback((uint32_t *)msg);
=======
	struct adf_hw_csr_ops *csr_ops = GET_CSR_OPS(ring->bank->accel_dev);
	u32 msg_counter = 0;
	u32 *msg = (u32 *)((uintptr_t)ring->base_addr + ring->head);

	while (*msg != ADF_RING_EMPTY_SIG) {
		ring->callback((u32 *)msg);
>>>>>>> upstream/android-13
		atomic_dec(ring->inflights);
		*msg = ADF_RING_EMPTY_SIG;
		ring->head = adf_modulo(ring->head +
					ADF_MSG_SIZE_TO_BYTES(ring->msg_size),
					ADF_RING_SIZE_MODULO(ring->ring_size));
		msg_counter++;
<<<<<<< HEAD
		msg = (uint32_t *)((uintptr_t)ring->base_addr + ring->head);
	}
	if (msg_counter > 0)
		WRITE_CSR_RING_HEAD(ring->bank->csr_addr,
				    ring->bank->bank_number,
				    ring->ring_number, ring->head);
=======
		msg = (u32 *)((uintptr_t)ring->base_addr + ring->head);
	}
	if (msg_counter > 0) {
		csr_ops->write_csr_ring_head(ring->bank->csr_addr,
					     ring->bank->bank_number,
					     ring->ring_number, ring->head);
	}
>>>>>>> upstream/android-13
	return 0;
}

static void adf_configure_tx_ring(struct adf_etr_ring_data *ring)
{
<<<<<<< HEAD
	uint32_t ring_config = BUILD_RING_CONFIG(ring->ring_size);

	WRITE_CSR_RING_CONFIG(ring->bank->csr_addr, ring->bank->bank_number,
			      ring->ring_number, ring_config);
=======
	struct adf_hw_csr_ops *csr_ops = GET_CSR_OPS(ring->bank->accel_dev);
	u32 ring_config = BUILD_RING_CONFIG(ring->ring_size);

	csr_ops->write_csr_ring_config(ring->bank->csr_addr,
				       ring->bank->bank_number,
				       ring->ring_number, ring_config);

>>>>>>> upstream/android-13
}

static void adf_configure_rx_ring(struct adf_etr_ring_data *ring)
{
<<<<<<< HEAD
	uint32_t ring_config =
=======
	struct adf_hw_csr_ops *csr_ops = GET_CSR_OPS(ring->bank->accel_dev);
	u32 ring_config =
>>>>>>> upstream/android-13
			BUILD_RESP_RING_CONFIG(ring->ring_size,
					       ADF_RING_NEAR_WATERMARK_512,
					       ADF_RING_NEAR_WATERMARK_0);

<<<<<<< HEAD
	WRITE_CSR_RING_CONFIG(ring->bank->csr_addr, ring->bank->bank_number,
			      ring->ring_number, ring_config);
=======
	csr_ops->write_csr_ring_config(ring->bank->csr_addr,
				       ring->bank->bank_number,
				       ring->ring_number, ring_config);
>>>>>>> upstream/android-13
}

static int adf_init_ring(struct adf_etr_ring_data *ring)
{
	struct adf_etr_bank_data *bank = ring->bank;
	struct adf_accel_dev *accel_dev = bank->accel_dev;
	struct adf_hw_device_data *hw_data = accel_dev->hw_device;
<<<<<<< HEAD
	uint64_t ring_base;
	uint32_t ring_size_bytes =
=======
	struct adf_hw_csr_ops *csr_ops = GET_CSR_OPS(accel_dev);
	u64 ring_base;
	u32 ring_size_bytes =
>>>>>>> upstream/android-13
			ADF_SIZE_TO_RING_SIZE_IN_BYTES(ring->ring_size);

	ring_size_bytes = ADF_RING_SIZE_BYTES_MIN(ring_size_bytes);
	ring->base_addr = dma_alloc_coherent(&GET_DEV(accel_dev),
					     ring_size_bytes, &ring->dma_addr,
					     GFP_KERNEL);
	if (!ring->base_addr)
		return -ENOMEM;

	memset(ring->base_addr, 0x7F, ring_size_bytes);
	/* The base_addr has to be aligned to the size of the buffer */
	if (adf_check_ring_alignment(ring->dma_addr, ring_size_bytes)) {
		dev_err(&GET_DEV(accel_dev), "Ring address not aligned\n");
		dma_free_coherent(&GET_DEV(accel_dev), ring_size_bytes,
				  ring->base_addr, ring->dma_addr);
		ring->base_addr = NULL;
		return -EFAULT;
	}

	if (hw_data->tx_rings_mask & (1 << ring->ring_number))
		adf_configure_tx_ring(ring);

	else
		adf_configure_rx_ring(ring);

<<<<<<< HEAD
	ring_base = BUILD_RING_BASE_ADDR(ring->dma_addr, ring->ring_size);
	WRITE_CSR_RING_BASE(ring->bank->csr_addr, ring->bank->bank_number,
			    ring->ring_number, ring_base);
=======
	ring_base = csr_ops->build_csr_ring_base_addr(ring->dma_addr,
						      ring->ring_size);

	csr_ops->write_csr_ring_base(ring->bank->csr_addr,
				     ring->bank->bank_number, ring->ring_number,
				     ring_base);
>>>>>>> upstream/android-13
	spin_lock_init(&ring->lock);
	return 0;
}

static void adf_cleanup_ring(struct adf_etr_ring_data *ring)
{
<<<<<<< HEAD
	uint32_t ring_size_bytes =
=======
	u32 ring_size_bytes =
>>>>>>> upstream/android-13
			ADF_SIZE_TO_RING_SIZE_IN_BYTES(ring->ring_size);
	ring_size_bytes = ADF_RING_SIZE_BYTES_MIN(ring_size_bytes);

	if (ring->base_addr) {
		memset(ring->base_addr, 0x7F, ring_size_bytes);
		dma_free_coherent(&GET_DEV(ring->bank->accel_dev),
				  ring_size_bytes, ring->base_addr,
				  ring->dma_addr);
	}
}

int adf_create_ring(struct adf_accel_dev *accel_dev, const char *section,
<<<<<<< HEAD
		    uint32_t bank_num, uint32_t num_msgs,
		    uint32_t msg_size, const char *ring_name,
=======
		    u32 bank_num, u32 num_msgs,
		    u32 msg_size, const char *ring_name,
>>>>>>> upstream/android-13
		    adf_callback_fn callback, int poll_mode,
		    struct adf_etr_ring_data **ring_ptr)
{
	struct adf_etr_data *transport_data = accel_dev->transport;
<<<<<<< HEAD
	struct adf_etr_bank_data *bank;
	struct adf_etr_ring_data *ring;
	char val[ADF_CFG_MAX_VAL_LEN_IN_BYTES];
	uint32_t ring_num;
=======
	u8 num_rings_per_bank = GET_NUM_RINGS_PER_BANK(accel_dev);
	struct adf_etr_bank_data *bank;
	struct adf_etr_ring_data *ring;
	char val[ADF_CFG_MAX_VAL_LEN_IN_BYTES];
	u32 ring_num;
>>>>>>> upstream/android-13
	int ret;

	if (bank_num >= GET_MAX_BANKS(accel_dev)) {
		dev_err(&GET_DEV(accel_dev), "Invalid bank number\n");
		return -EFAULT;
	}
	if (msg_size > ADF_MSG_SIZE_TO_BYTES(ADF_MAX_MSG_SIZE)) {
		dev_err(&GET_DEV(accel_dev), "Invalid msg size\n");
		return -EFAULT;
	}
	if (ADF_MAX_INFLIGHTS(adf_verify_ring_size(msg_size, num_msgs),
			      ADF_BYTES_TO_MSG_SIZE(msg_size)) < 2) {
		dev_err(&GET_DEV(accel_dev),
			"Invalid ring size for given msg size\n");
		return -EFAULT;
	}
	if (adf_cfg_get_param_value(accel_dev, section, ring_name, val)) {
		dev_err(&GET_DEV(accel_dev), "Section %s, no such entry : %s\n",
			section, ring_name);
		return -EFAULT;
	}
	if (kstrtouint(val, 10, &ring_num)) {
		dev_err(&GET_DEV(accel_dev), "Can't get ring number\n");
		return -EFAULT;
	}
<<<<<<< HEAD
	if (ring_num >= ADF_ETR_MAX_RINGS_PER_BANK) {
=======
	if (ring_num >= num_rings_per_bank) {
>>>>>>> upstream/android-13
		dev_err(&GET_DEV(accel_dev), "Invalid ring number\n");
		return -EFAULT;
	}

<<<<<<< HEAD
=======
	ring_num = array_index_nospec(ring_num, num_rings_per_bank);
>>>>>>> upstream/android-13
	bank = &transport_data->banks[bank_num];
	if (adf_reserve_ring(bank, ring_num)) {
		dev_err(&GET_DEV(accel_dev), "Ring %d, %s already exists.\n",
			ring_num, ring_name);
		return -EFAULT;
	}
	ring = &bank->rings[ring_num];
	ring->ring_number = ring_num;
	ring->bank = bank;
	ring->callback = callback;
	ring->msg_size = ADF_BYTES_TO_MSG_SIZE(msg_size);
	ring->ring_size = adf_verify_ring_size(msg_size, num_msgs);
	ring->head = 0;
	ring->tail = 0;
	atomic_set(ring->inflights, 0);
	ret = adf_init_ring(ring);
	if (ret)
		goto err;

	/* Enable HW arbitration for the given ring */
	adf_update_ring_arb(ring);

	if (adf_ring_debugfs_add(ring, ring_name)) {
		dev_err(&GET_DEV(accel_dev),
			"Couldn't add ring debugfs entry\n");
		ret = -EFAULT;
		goto err;
	}

	/* Enable interrupts if needed */
	if (callback && (!poll_mode))
		adf_enable_ring_irq(bank, ring->ring_number);
	*ring_ptr = ring;
	return 0;
err:
	adf_cleanup_ring(ring);
	adf_unreserve_ring(bank, ring_num);
	adf_update_ring_arb(ring);
	return ret;
}

void adf_remove_ring(struct adf_etr_ring_data *ring)
{
	struct adf_etr_bank_data *bank = ring->bank;
<<<<<<< HEAD
=======
	struct adf_hw_csr_ops *csr_ops = GET_CSR_OPS(bank->accel_dev);
>>>>>>> upstream/android-13

	/* Disable interrupts for the given ring */
	adf_disable_ring_irq(bank, ring->ring_number);

	/* Clear PCI config space */
<<<<<<< HEAD
	WRITE_CSR_RING_CONFIG(bank->csr_addr, bank->bank_number,
			      ring->ring_number, 0);
	WRITE_CSR_RING_BASE(bank->csr_addr, bank->bank_number,
			    ring->ring_number, 0);
=======

	csr_ops->write_csr_ring_config(bank->csr_addr, bank->bank_number,
				       ring->ring_number, 0);
	csr_ops->write_csr_ring_base(bank->csr_addr, bank->bank_number,
				     ring->ring_number, 0);
>>>>>>> upstream/android-13
	adf_ring_debugfs_rm(ring);
	adf_unreserve_ring(bank, ring->ring_number);
	/* Disable HW arbitration for the given ring */
	adf_update_ring_arb(ring);
	adf_cleanup_ring(ring);
}

static void adf_ring_response_handler(struct adf_etr_bank_data *bank)
{
<<<<<<< HEAD
	uint32_t empty_rings, i;

	empty_rings = READ_CSR_E_STAT(bank->csr_addr, bank->bank_number);
	empty_rings = ~empty_rings & bank->irq_mask;

	for (i = 0; i < ADF_ETR_MAX_RINGS_PER_BANK; ++i) {
		if (empty_rings & (1 << i))
			adf_handle_response(&bank->rings[i]);
	}
=======
	struct adf_accel_dev *accel_dev = bank->accel_dev;
	u8 num_rings_per_bank = GET_NUM_RINGS_PER_BANK(accel_dev);
	struct adf_hw_csr_ops *csr_ops = GET_CSR_OPS(accel_dev);
	unsigned long empty_rings;
	int i;

	empty_rings = csr_ops->read_csr_e_stat(bank->csr_addr,
					       bank->bank_number);
	empty_rings = ~empty_rings & bank->irq_mask;

	for_each_set_bit(i, &empty_rings, num_rings_per_bank)
		adf_handle_response(&bank->rings[i]);
>>>>>>> upstream/android-13
}

void adf_response_handler(uintptr_t bank_addr)
{
	struct adf_etr_bank_data *bank = (void *)bank_addr;
<<<<<<< HEAD

	/* Handle all the responses and reenable IRQs */
	adf_ring_response_handler(bank);
	WRITE_CSR_INT_FLAG_AND_COL(bank->csr_addr, bank->bank_number,
				   bank->irq_mask);
=======
	struct adf_hw_csr_ops *csr_ops = GET_CSR_OPS(bank->accel_dev);

	/* Handle all the responses and reenable IRQs */
	adf_ring_response_handler(bank);

	csr_ops->write_csr_int_flag_and_col(bank->csr_addr, bank->bank_number,
					    bank->irq_mask);
>>>>>>> upstream/android-13
}

static inline int adf_get_cfg_int(struct adf_accel_dev *accel_dev,
				  const char *section, const char *format,
<<<<<<< HEAD
				  uint32_t key, uint32_t *value)
=======
				  u32 key, u32 *value)
>>>>>>> upstream/android-13
{
	char key_buf[ADF_CFG_MAX_KEY_LEN_IN_BYTES];
	char val_buf[ADF_CFG_MAX_VAL_LEN_IN_BYTES];

	snprintf(key_buf, ADF_CFG_MAX_KEY_LEN_IN_BYTES, format, key);

	if (adf_cfg_get_param_value(accel_dev, section, key_buf, val_buf))
		return -EFAULT;

	if (kstrtouint(val_buf, 10, value))
		return -EFAULT;
	return 0;
}

static void adf_get_coalesc_timer(struct adf_etr_bank_data *bank,
				  const char *section,
<<<<<<< HEAD
				  uint32_t bank_num_in_accel)
=======
				  u32 bank_num_in_accel)
>>>>>>> upstream/android-13
{
	if (adf_get_cfg_int(bank->accel_dev, section,
			    ADF_ETRMGR_COALESCE_TIMER_FORMAT,
			    bank_num_in_accel, &bank->irq_coalesc_timer))
		bank->irq_coalesc_timer = ADF_COALESCING_DEF_TIME;

	if (ADF_COALESCING_MAX_TIME < bank->irq_coalesc_timer ||
	    ADF_COALESCING_MIN_TIME > bank->irq_coalesc_timer)
		bank->irq_coalesc_timer = ADF_COALESCING_DEF_TIME;
}

static int adf_init_bank(struct adf_accel_dev *accel_dev,
			 struct adf_etr_bank_data *bank,
<<<<<<< HEAD
			 uint32_t bank_num, void __iomem *csr_addr)
{
	struct adf_hw_device_data *hw_data = accel_dev->hw_device;
	struct adf_etr_ring_data *ring;
	struct adf_etr_ring_data *tx_ring;
	uint32_t i, coalesc_enabled = 0;
=======
			 u32 bank_num, void __iomem *csr_addr)
{
	struct adf_hw_device_data *hw_data = accel_dev->hw_device;
	u8 num_rings_per_bank = hw_data->num_rings_per_bank;
	struct adf_hw_csr_ops *csr_ops = &hw_data->csr_ops;
	u32 irq_mask = BIT(num_rings_per_bank) - 1;
	struct adf_etr_ring_data *ring;
	struct adf_etr_ring_data *tx_ring;
	u32 i, coalesc_enabled = 0;
	unsigned long ring_mask;
	int size;
>>>>>>> upstream/android-13

	memset(bank, 0, sizeof(*bank));
	bank->bank_number = bank_num;
	bank->csr_addr = csr_addr;
	bank->accel_dev = accel_dev;
	spin_lock_init(&bank->lock);

<<<<<<< HEAD
=======
	/* Allocate the rings in the bank */
	size = num_rings_per_bank * sizeof(struct adf_etr_ring_data);
	bank->rings = kzalloc_node(size, GFP_KERNEL,
				   dev_to_node(&GET_DEV(accel_dev)));
	if (!bank->rings)
		return -ENOMEM;

>>>>>>> upstream/android-13
	/* Enable IRQ coalescing always. This will allow to use
	 * the optimised flag and coalesc register.
	 * If it is disabled in the config file just use min time value */
	if ((adf_get_cfg_int(accel_dev, "Accelerator0",
			     ADF_ETRMGR_COALESCING_ENABLED_FORMAT, bank_num,
			     &coalesc_enabled) == 0) && coalesc_enabled)
		adf_get_coalesc_timer(bank, "Accelerator0", bank_num);
	else
		bank->irq_coalesc_timer = ADF_COALESCING_MIN_TIME;

<<<<<<< HEAD
	for (i = 0; i < ADF_ETR_MAX_RINGS_PER_BANK; i++) {
		WRITE_CSR_RING_CONFIG(csr_addr, bank_num, i, 0);
		WRITE_CSR_RING_BASE(csr_addr, bank_num, i, 0);
=======
	for (i = 0; i < num_rings_per_bank; i++) {
		csr_ops->write_csr_ring_config(csr_addr, bank_num, i, 0);
		csr_ops->write_csr_ring_base(csr_addr, bank_num, i, 0);

>>>>>>> upstream/android-13
		ring = &bank->rings[i];
		if (hw_data->tx_rings_mask & (1 << i)) {
			ring->inflights =
				kzalloc_node(sizeof(atomic_t),
					     GFP_KERNEL,
					     dev_to_node(&GET_DEV(accel_dev)));
			if (!ring->inflights)
				goto err;
		} else {
			if (i < hw_data->tx_rx_gap) {
				dev_err(&GET_DEV(accel_dev),
					"Invalid tx rings mask config\n");
				goto err;
			}
			tx_ring = &bank->rings[i - hw_data->tx_rx_gap];
			ring->inflights = tx_ring->inflights;
		}
	}
	if (adf_bank_debugfs_add(bank)) {
		dev_err(&GET_DEV(accel_dev),
			"Failed to add bank debugfs entry\n");
		goto err;
	}

<<<<<<< HEAD
	WRITE_CSR_INT_FLAG(csr_addr, bank_num, ADF_BANK_INT_FLAG_CLEAR_MASK);
	WRITE_CSR_INT_SRCSEL(csr_addr, bank_num);
	return 0;
err:
	for (i = 0; i < ADF_ETR_MAX_RINGS_PER_BANK; i++) {
		ring = &bank->rings[i];
		if (hw_data->tx_rings_mask & (1 << i))
			kfree(ring->inflights);
	}
=======
	csr_ops->write_csr_int_flag(csr_addr, bank_num, irq_mask);
	csr_ops->write_csr_int_srcsel(csr_addr, bank_num);

	return 0;
err:
	ring_mask = hw_data->tx_rings_mask;
	for_each_set_bit(i, &ring_mask, num_rings_per_bank) {
		ring = &bank->rings[i];
		kfree(ring->inflights);
		ring->inflights = NULL;
	}
	kfree(bank->rings);
>>>>>>> upstream/android-13
	return -ENOMEM;
}

/**
 * adf_init_etr_data() - Initialize transport rings for acceleration device
 * @accel_dev:  Pointer to acceleration device.
 *
 * Function is the initializes the communications channels (rings) to the
 * acceleration device accel_dev.
 * To be used by QAT device specific drivers.
 *
 * Return: 0 on success, error code otherwise.
 */
int adf_init_etr_data(struct adf_accel_dev *accel_dev)
{
	struct adf_etr_data *etr_data;
	struct adf_hw_device_data *hw_data = accel_dev->hw_device;
	void __iomem *csr_addr;
<<<<<<< HEAD
	uint32_t size;
	uint32_t num_banks = 0;
=======
	u32 size;
	u32 num_banks = 0;
>>>>>>> upstream/android-13
	int i, ret;

	etr_data = kzalloc_node(sizeof(*etr_data), GFP_KERNEL,
				dev_to_node(&GET_DEV(accel_dev)));
	if (!etr_data)
		return -ENOMEM;

	num_banks = GET_MAX_BANKS(accel_dev);
	size = num_banks * sizeof(struct adf_etr_bank_data);
	etr_data->banks = kzalloc_node(size, GFP_KERNEL,
				       dev_to_node(&GET_DEV(accel_dev)));
	if (!etr_data->banks) {
		ret = -ENOMEM;
		goto err_bank;
	}

	accel_dev->transport = etr_data;
	i = hw_data->get_etr_bar_id(hw_data);
	csr_addr = accel_dev->accel_pci_dev.pci_bars[i].virt_addr;

	/* accel_dev->debugfs_dir should always be non-NULL here */
	etr_data->debug = debugfs_create_dir("transport",
					     accel_dev->debugfs_dir);
<<<<<<< HEAD
	if (!etr_data->debug) {
		dev_err(&GET_DEV(accel_dev),
			"Unable to create transport debugfs entry\n");
		ret = -ENOENT;
		goto err_bank_debug;
	}
=======
>>>>>>> upstream/android-13

	for (i = 0; i < num_banks; i++) {
		ret = adf_init_bank(accel_dev, &etr_data->banks[i], i,
				    csr_addr);
		if (ret)
			goto err_bank_all;
	}

	return 0;

err_bank_all:
	debugfs_remove(etr_data->debug);
<<<<<<< HEAD
err_bank_debug:
=======
>>>>>>> upstream/android-13
	kfree(etr_data->banks);
err_bank:
	kfree(etr_data);
	accel_dev->transport = NULL;
	return ret;
}
EXPORT_SYMBOL_GPL(adf_init_etr_data);

static void cleanup_bank(struct adf_etr_bank_data *bank)
{
<<<<<<< HEAD
	uint32_t i;

	for (i = 0; i < ADF_ETR_MAX_RINGS_PER_BANK; i++) {
		struct adf_accel_dev *accel_dev = bank->accel_dev;
		struct adf_hw_device_data *hw_data = accel_dev->hw_device;
=======
	struct adf_accel_dev *accel_dev = bank->accel_dev;
	struct adf_hw_device_data *hw_data = accel_dev->hw_device;
	u8 num_rings_per_bank = hw_data->num_rings_per_bank;
	u32 i;

	for (i = 0; i < num_rings_per_bank; i++) {
>>>>>>> upstream/android-13
		struct adf_etr_ring_data *ring = &bank->rings[i];

		if (bank->ring_mask & (1 << i))
			adf_cleanup_ring(ring);

		if (hw_data->tx_rings_mask & (1 << i))
			kfree(ring->inflights);
	}
<<<<<<< HEAD
=======
	kfree(bank->rings);
>>>>>>> upstream/android-13
	adf_bank_debugfs_rm(bank);
	memset(bank, 0, sizeof(*bank));
}

static void adf_cleanup_etr_handles(struct adf_accel_dev *accel_dev)
{
	struct adf_etr_data *etr_data = accel_dev->transport;
<<<<<<< HEAD
	uint32_t i, num_banks = GET_MAX_BANKS(accel_dev);
=======
	u32 i, num_banks = GET_MAX_BANKS(accel_dev);
>>>>>>> upstream/android-13

	for (i = 0; i < num_banks; i++)
		cleanup_bank(&etr_data->banks[i]);
}

/**
 * adf_cleanup_etr_data() - Clear transport rings for acceleration device
 * @accel_dev:  Pointer to acceleration device.
 *
 * Function is the clears the communications channels (rings) of the
 * acceleration device accel_dev.
 * To be used by QAT device specific drivers.
 *
 * Return: void
 */
void adf_cleanup_etr_data(struct adf_accel_dev *accel_dev)
{
	struct adf_etr_data *etr_data = accel_dev->transport;

	if (etr_data) {
		adf_cleanup_etr_handles(accel_dev);
		debugfs_remove(etr_data->debug);
<<<<<<< HEAD
=======
		kfree(etr_data->banks->rings);
>>>>>>> upstream/android-13
		kfree(etr_data->banks);
		kfree(etr_data);
		accel_dev->transport = NULL;
	}
}
EXPORT_SYMBOL_GPL(adf_cleanup_etr_data);
