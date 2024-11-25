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
#ifndef _QAT_CRYPTO_INSTANCE_H_
#define _QAT_CRYPTO_INSTANCE_H_

=======
/* SPDX-License-Identifier: (BSD-3-Clause OR GPL-2.0-only) */
/* Copyright(c) 2014 - 2020 Intel Corporation */
#ifndef _QAT_CRYPTO_INSTANCE_H_
#define _QAT_CRYPTO_INSTANCE_H_

#include <crypto/aes.h>
>>>>>>> upstream/android-13
#include <linux/list.h>
#include <linux/slab.h>
#include "adf_accel_devices.h"
#include "icp_qat_fw_la.h"

struct qat_crypto_instance {
	struct adf_etr_ring_data *sym_tx;
	struct adf_etr_ring_data *sym_rx;
	struct adf_etr_ring_data *pke_tx;
	struct adf_etr_ring_data *pke_rx;
	struct adf_accel_dev *accel_dev;
	struct list_head list;
	unsigned long state;
	int id;
	atomic_t refctr;
};

struct qat_crypto_request_buffs {
	struct qat_alg_buf_list *bl;
	dma_addr_t blp;
	struct qat_alg_buf_list *blout;
	dma_addr_t bloutp;
	size_t sz;
	size_t sz_out;
};

struct qat_crypto_request;

struct qat_crypto_request {
	struct icp_qat_fw_la_bulk_req req;
	union {
		struct qat_alg_aead_ctx *aead_ctx;
<<<<<<< HEAD
		struct qat_alg_ablkcipher_ctx *ablkcipher_ctx;
	};
	union {
		struct aead_request *aead_req;
		struct ablkcipher_request *ablkcipher_req;
=======
		struct qat_alg_skcipher_ctx *skcipher_ctx;
	};
	union {
		struct aead_request *aead_req;
		struct skcipher_request *skcipher_req;
>>>>>>> upstream/android-13
	};
	struct qat_crypto_request_buffs buf;
	void (*cb)(struct icp_qat_fw_la_resp *resp,
		   struct qat_crypto_request *req);
<<<<<<< HEAD
};

=======
	union {
		struct {
			__be64 iv_hi;
			__be64 iv_lo;
		};
		u8 iv[AES_BLOCK_SIZE];
	};
	bool encryption;
};

static inline bool adf_hw_dev_has_crypto(struct adf_accel_dev *accel_dev)
{
	struct adf_hw_device_data *hw_device = accel_dev->hw_device;
	u32 mask = ~hw_device->accel_capabilities_mask;

	if (mask & ADF_ACCEL_CAPABILITIES_CRYPTO_SYMMETRIC)
		return false;
	if (mask & ADF_ACCEL_CAPABILITIES_CRYPTO_ASYMMETRIC)
		return false;
	if (mask & ADF_ACCEL_CAPABILITIES_AUTHENTICATION)
		return false;

	return true;
}

>>>>>>> upstream/android-13
#endif
