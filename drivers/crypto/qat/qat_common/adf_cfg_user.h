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
#ifndef ADF_CFG_USER_H_
#define ADF_CFG_USER_H_

#include "adf_cfg_common.h"
#include "adf_cfg_strings.h"

struct adf_user_cfg_key_val {
	char key[ADF_CFG_MAX_KEY_LEN_IN_BYTES];
	char val[ADF_CFG_MAX_VAL_LEN_IN_BYTES];
	union {
		struct adf_user_cfg_key_val *next;
<<<<<<< HEAD
		uint64_t padding3;
=======
		__u64 padding3;
>>>>>>> upstream/android-13
	};
	enum adf_cfg_val_type type;
} __packed;

struct adf_user_cfg_section {
	char name[ADF_CFG_MAX_SECTION_LEN_IN_BYTES];
	union {
		struct adf_user_cfg_key_val *params;
<<<<<<< HEAD
		uint64_t padding1;
	};
	union {
		struct adf_user_cfg_section *next;
		uint64_t padding3;
=======
		__u64 padding1;
	};
	union {
		struct adf_user_cfg_section *next;
		__u64 padding3;
>>>>>>> upstream/android-13
	};
} __packed;

struct adf_user_cfg_ctl_data {
	union {
		struct adf_user_cfg_section *config_section;
<<<<<<< HEAD
		uint64_t padding;
	};
	uint8_t device_id;
=======
		__u64 padding;
	};
	__u8 device_id;
>>>>>>> upstream/android-13
} __packed;
#endif
