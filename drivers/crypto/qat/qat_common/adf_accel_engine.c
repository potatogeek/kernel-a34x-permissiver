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
#include <linux/firmware.h>
#include <linux/pci.h>
#include "adf_cfg.h"
#include "adf_accel_devices.h"
#include "adf_common_drv.h"
#include "icp_qat_uclo.h"

<<<<<<< HEAD
=======
static int adf_ae_fw_load_images(struct adf_accel_dev *accel_dev, void *fw_addr,
				 u32 fw_size)
{
	struct adf_fw_loader_data *loader_data = accel_dev->fw_loader;
	struct adf_hw_device_data *hw_device = accel_dev->hw_device;
	struct icp_qat_fw_loader_handle *loader;
	char *obj_name;
	u32 num_objs;
	u32 ae_mask;
	int i;

	loader = loader_data->fw_loader;
	num_objs = hw_device->uof_get_num_objs();

	for (i = 0; i < num_objs; i++) {
		obj_name = hw_device->uof_get_name(i);
		ae_mask = hw_device->uof_get_ae_mask(i);

		if (qat_uclo_set_cfg_ae_mask(loader, ae_mask)) {
			dev_err(&GET_DEV(accel_dev),
				"Invalid mask for UOF image\n");
			goto out_err;
		}
		if (qat_uclo_map_obj(loader, fw_addr, fw_size, obj_name)) {
			dev_err(&GET_DEV(accel_dev),
				"Failed to map UOF firmware\n");
			goto out_err;
		}
		if (qat_uclo_wr_all_uimage(loader)) {
			dev_err(&GET_DEV(accel_dev),
				"Failed to load UOF firmware\n");
			goto out_err;
		}
		qat_uclo_del_obj(loader);
	}

	return 0;

out_err:
	adf_ae_fw_release(accel_dev);
	return -EFAULT;
}

>>>>>>> upstream/android-13
int adf_ae_fw_load(struct adf_accel_dev *accel_dev)
{
	struct adf_fw_loader_data *loader_data = accel_dev->fw_loader;
	struct adf_hw_device_data *hw_device = accel_dev->hw_device;
<<<<<<< HEAD
	void *uof_addr, *mmp_addr;
	u32 uof_size, mmp_size;
=======
	void *fw_addr, *mmp_addr;
	u32 fw_size, mmp_size;
>>>>>>> upstream/android-13

	if (!hw_device->fw_name)
		return 0;

	if (request_firmware(&loader_data->mmp_fw, hw_device->fw_mmp_name,
			     &accel_dev->accel_pci_dev.pci_dev->dev)) {
		dev_err(&GET_DEV(accel_dev), "Failed to load MMP firmware %s\n",
			hw_device->fw_mmp_name);
		return -EFAULT;
	}
	if (request_firmware(&loader_data->uof_fw, hw_device->fw_name,
			     &accel_dev->accel_pci_dev.pci_dev->dev)) {
		dev_err(&GET_DEV(accel_dev), "Failed to load UOF firmware %s\n",
			hw_device->fw_name);
		goto out_err;
	}

<<<<<<< HEAD
	uof_size = loader_data->uof_fw->size;
	uof_addr = (void *)loader_data->uof_fw->data;
	mmp_size = loader_data->mmp_fw->size;
	mmp_addr = (void *)loader_data->mmp_fw->data;
=======
	fw_size = loader_data->uof_fw->size;
	fw_addr = (void *)loader_data->uof_fw->data;
	mmp_size = loader_data->mmp_fw->size;
	mmp_addr = (void *)loader_data->mmp_fw->data;

>>>>>>> upstream/android-13
	if (qat_uclo_wr_mimage(loader_data->fw_loader, mmp_addr, mmp_size)) {
		dev_err(&GET_DEV(accel_dev), "Failed to load MMP\n");
		goto out_err;
	}
<<<<<<< HEAD
	if (qat_uclo_map_obj(loader_data->fw_loader, uof_addr, uof_size)) {
=======

	if (hw_device->uof_get_num_objs)
		return adf_ae_fw_load_images(accel_dev, fw_addr, fw_size);

	if (qat_uclo_map_obj(loader_data->fw_loader, fw_addr, fw_size, NULL)) {
>>>>>>> upstream/android-13
		dev_err(&GET_DEV(accel_dev), "Failed to map FW\n");
		goto out_err;
	}
	if (qat_uclo_wr_all_uimage(loader_data->fw_loader)) {
		dev_err(&GET_DEV(accel_dev), "Failed to load UOF\n");
		goto out_err;
	}
	return 0;

out_err:
	adf_ae_fw_release(accel_dev);
	return -EFAULT;
}

void adf_ae_fw_release(struct adf_accel_dev *accel_dev)
{
	struct adf_fw_loader_data *loader_data = accel_dev->fw_loader;
	struct adf_hw_device_data *hw_device = accel_dev->hw_device;

	if (!hw_device->fw_name)
		return;

<<<<<<< HEAD
	qat_uclo_del_uof_obj(loader_data->fw_loader);
=======
	qat_uclo_del_obj(loader_data->fw_loader);
>>>>>>> upstream/android-13
	qat_hal_deinit(loader_data->fw_loader);
	release_firmware(loader_data->uof_fw);
	release_firmware(loader_data->mmp_fw);
	loader_data->uof_fw = NULL;
	loader_data->mmp_fw = NULL;
	loader_data->fw_loader = NULL;
}

int adf_ae_start(struct adf_accel_dev *accel_dev)
{
	struct adf_fw_loader_data *loader_data = accel_dev->fw_loader;
	struct adf_hw_device_data *hw_data = accel_dev->hw_device;
<<<<<<< HEAD
	uint32_t ae_ctr, ae, max_aes = GET_MAX_ACCELENGINES(accel_dev);
=======
	u32 ae_ctr;
>>>>>>> upstream/android-13

	if (!hw_data->fw_name)
		return 0;

<<<<<<< HEAD
	for (ae = 0, ae_ctr = 0; ae < max_aes; ae++) {
		if (hw_data->ae_mask & (1 << ae)) {
			qat_hal_start(loader_data->fw_loader, ae, 0xFF);
			ae_ctr++;
		}
	}
=======
	ae_ctr = qat_hal_start(loader_data->fw_loader);
>>>>>>> upstream/android-13
	dev_info(&GET_DEV(accel_dev),
		 "qat_dev%d started %d acceleration engines\n",
		 accel_dev->accel_id, ae_ctr);
	return 0;
}

int adf_ae_stop(struct adf_accel_dev *accel_dev)
{
	struct adf_fw_loader_data *loader_data = accel_dev->fw_loader;
	struct adf_hw_device_data *hw_data = accel_dev->hw_device;
<<<<<<< HEAD
	uint32_t ae_ctr, ae, max_aes = GET_MAX_ACCELENGINES(accel_dev);
=======
	u32 ae_ctr, ae, max_aes = GET_MAX_ACCELENGINES(accel_dev);
>>>>>>> upstream/android-13

	if (!hw_data->fw_name)
		return 0;

	for (ae = 0, ae_ctr = 0; ae < max_aes; ae++) {
		if (hw_data->ae_mask & (1 << ae)) {
			qat_hal_stop(loader_data->fw_loader, ae, 0xFF);
			ae_ctr++;
		}
	}
	dev_info(&GET_DEV(accel_dev),
		 "qat_dev%d stopped %d acceleration engines\n",
		 accel_dev->accel_id, ae_ctr);
	return 0;
}

static int adf_ae_reset(struct adf_accel_dev *accel_dev, int ae)
{
	struct adf_fw_loader_data *loader_data = accel_dev->fw_loader;

	qat_hal_reset(loader_data->fw_loader);
	if (qat_hal_clr_reset(loader_data->fw_loader))
		return -EFAULT;

	return 0;
}

int adf_ae_init(struct adf_accel_dev *accel_dev)
{
	struct adf_fw_loader_data *loader_data;
	struct adf_hw_device_data *hw_device = accel_dev->hw_device;

	if (!hw_device->fw_name)
		return 0;

	loader_data = kzalloc(sizeof(*loader_data), GFP_KERNEL);
	if (!loader_data)
		return -ENOMEM;

	accel_dev->fw_loader = loader_data;
	if (qat_hal_init(accel_dev)) {
		dev_err(&GET_DEV(accel_dev), "Failed to init the AEs\n");
		kfree(loader_data);
		return -EFAULT;
	}
	if (adf_ae_reset(accel_dev, 0)) {
		dev_err(&GET_DEV(accel_dev), "Failed to reset the AEs\n");
		qat_hal_deinit(loader_data->fw_loader);
		kfree(loader_data);
		return -EFAULT;
	}
	return 0;
}

int adf_ae_shutdown(struct adf_accel_dev *accel_dev)
{
	struct adf_fw_loader_data *loader_data = accel_dev->fw_loader;
	struct adf_hw_device_data *hw_device = accel_dev->hw_device;

	if (!hw_device->fw_name)
		return 0;

	qat_hal_deinit(loader_data->fw_loader);
	kfree(accel_dev->fw_loader);
	accel_dev->fw_loader = NULL;
	return 0;
}
