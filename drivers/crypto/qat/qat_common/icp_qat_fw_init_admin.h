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
/* SPDX-License-Identifier: (BSD-3-Clause OR GPL-2.0-only) */
/* Copyright(c) 2014 - 2020 Intel Corporation */
>>>>>>> upstream/android-13
#ifndef _ICP_QAT_FW_INIT_ADMIN_H_
#define _ICP_QAT_FW_INIT_ADMIN_H_

#include "icp_qat_fw.h"

enum icp_qat_fw_init_admin_cmd_id {
<<<<<<< HEAD
	ICP_QAT_FW_INIT_ME = 0,
=======
	ICP_QAT_FW_INIT_AE = 0,
>>>>>>> upstream/android-13
	ICP_QAT_FW_TRNG_ENABLE = 1,
	ICP_QAT_FW_TRNG_DISABLE = 2,
	ICP_QAT_FW_CONSTANTS_CFG = 3,
	ICP_QAT_FW_STATUS_GET = 4,
	ICP_QAT_FW_COUNTERS_GET = 5,
	ICP_QAT_FW_LOOPBACK = 6,
	ICP_QAT_FW_HEARTBEAT_SYNC = 7,
	ICP_QAT_FW_HEARTBEAT_GET = 8
};

enum icp_qat_fw_init_admin_resp_status {
	ICP_QAT_FW_INIT_RESP_STATUS_SUCCESS = 0,
	ICP_QAT_FW_INIT_RESP_STATUS_FAIL
};

struct icp_qat_fw_init_admin_req {
<<<<<<< HEAD
	uint16_t init_cfg_sz;
	uint8_t resrvd1;
	uint8_t init_admin_cmd_id;
	uint32_t resrvd2;
	uint64_t opaque_data;
	uint64_t init_cfg_ptr;
	uint64_t resrvd3;
};

struct icp_qat_fw_init_admin_resp_hdr {
	uint8_t flags;
	uint8_t resrvd1;
	uint8_t status;
	uint8_t init_admin_cmd_id;
};

struct icp_qat_fw_init_admin_resp_pars {
	union {
		uint32_t resrvd1[ICP_QAT_FW_NUM_LONGWORDS_4];
		struct {
			uint32_t version_patch_num;
			uint8_t context_id;
			uint8_t ae_id;
			uint16_t resrvd1;
			uint64_t resrvd2;
		} s1;
		struct {
			uint64_t req_rec_count;
			uint64_t resp_sent_count;
		} s2;
	} u;
};

struct icp_qat_fw_init_admin_resp {
	struct icp_qat_fw_init_admin_resp_hdr init_resp_hdr;
	union {
		uint32_t resrvd2;
		struct {
			uint16_t version_minor_num;
			uint16_t version_major_num;
		} s;
	} u;
	uint64_t opaque_data;
	struct icp_qat_fw_init_admin_resp_pars init_resp_pars;
};
=======
	__u16 init_cfg_sz;
	__u8 resrvd1;
	__u8 cmd_id;
	__u32 resrvd2;
	__u64 opaque_data;
	__u64 init_cfg_ptr;

	union {
		struct {
			__u16 ibuf_size_in_kb;
			__u16 resrvd3;
		};
		__u32 idle_filter;
	};

	__u32 resrvd4;
} __packed;

struct icp_qat_fw_init_admin_resp {
	__u8 flags;
	__u8 resrvd1;
	__u8 status;
	__u8 cmd_id;
	union {
		__u32 resrvd2;
		struct {
			__u16 version_minor_num;
			__u16 version_major_num;
		};
	};
	__u64 opaque_data;
	union {
		__u32 resrvd3[ICP_QAT_FW_NUM_LONGWORDS_4];
		struct {
			__u32 version_patch_num;
			__u8 context_id;
			__u8 ae_id;
			__u16 resrvd4;
			__u64 resrvd5;
		};
		struct {
			__u64 req_rec_count;
			__u64 resp_sent_count;
		};
		struct {
			__u16 compression_algos;
			__u16 checksum_algos;
			__u32 deflate_capabilities;
			__u32 resrvd6;
			__u32 lzs_capabilities;
		};
		struct {
			__u32 cipher_algos;
			__u32 hash_algos;
			__u16 keygen_algos;
			__u16 other;
			__u16 public_key_algos;
			__u16 prime_algos;
		};
		struct {
			__u64 timestamp;
			__u64 resrvd7;
		};
		struct {
			__u32 successful_count;
			__u32 unsuccessful_count;
			__u64 resrvd8;
		};
	};
} __packed;
>>>>>>> upstream/android-13

#define ICP_QAT_FW_COMN_HEARTBEAT_OK 0
#define ICP_QAT_FW_COMN_HEARTBEAT_BLOCKED 1
#define ICP_QAT_FW_COMN_HEARTBEAT_FLAG_BITPOS 0
#define ICP_QAT_FW_COMN_HEARTBEAT_FLAG_MASK 0x1
#define ICP_QAT_FW_COMN_STATUS_RESRVD_FLD_MASK 0xFE
#define ICP_QAT_FW_COMN_HEARTBEAT_HDR_FLAG_GET(hdr_t) \
	ICP_QAT_FW_COMN_HEARTBEAT_FLAG_GET(hdr_t.flags)

#define ICP_QAT_FW_COMN_HEARTBEAT_HDR_FLAG_SET(hdr_t, val) \
	ICP_QAT_FW_COMN_HEARTBEAT_FLAG_SET(hdr_t, val)

#define ICP_QAT_FW_COMN_HEARTBEAT_FLAG_GET(flags) \
	QAT_FIELD_GET(flags, \
		 ICP_QAT_FW_COMN_HEARTBEAT_FLAG_BITPOS, \
		 ICP_QAT_FW_COMN_HEARTBEAT_FLAG_MASK)
#endif
