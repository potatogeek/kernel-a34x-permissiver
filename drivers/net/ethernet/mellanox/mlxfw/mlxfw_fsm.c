<<<<<<< HEAD
/*
 * drivers/net/ethernet/mellanox/mlxfw/mlxfw.c
 * Copyright (c) 2017 Mellanox Technologies. All rights reserved.
 * Copyright (c) 2017 Yotam Gigi <yotamg@mellanox.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
=======
// SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0
/* Copyright (c) 2017-2019 Mellanox Technologies. All rights reserved */
>>>>>>> upstream/android-13

#define pr_fmt(fmt) "mlxfw: " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>

#include "mlxfw.h"
#include "mlxfw_mfa2.h"

#define MLXFW_FSM_STATE_WAIT_CYCLE_MS 200
#define MLXFW_FSM_STATE_WAIT_TIMEOUT_MS 30000
#define MLXFW_FSM_STATE_WAIT_ROUNDS \
	(MLXFW_FSM_STATE_WAIT_TIMEOUT_MS / MLXFW_FSM_STATE_WAIT_CYCLE_MS)
#define MLXFW_FSM_MAX_COMPONENT_SIZE (10 * (1 << 20))

<<<<<<< HEAD
static const char * const mlxfw_fsm_state_err_str[] = {
	[MLXFW_FSM_STATE_ERR_ERROR] =
		"general error",
	[MLXFW_FSM_STATE_ERR_REJECTED_DIGEST_ERR] =
		"component hash mismatch",
	[MLXFW_FSM_STATE_ERR_REJECTED_NOT_APPLICABLE] =
		"component not applicable",
	[MLXFW_FSM_STATE_ERR_REJECTED_UNKNOWN_KEY] =
		"unknown key",
	[MLXFW_FSM_STATE_ERR_REJECTED_AUTH_FAILED] =
		"authentication failed",
	[MLXFW_FSM_STATE_ERR_REJECTED_UNSIGNED] =
		"component was not signed",
	[MLXFW_FSM_STATE_ERR_REJECTED_KEY_NOT_APPLICABLE] =
		"key not applicable",
	[MLXFW_FSM_STATE_ERR_REJECTED_BAD_FORMAT] =
		"bad format",
	[MLXFW_FSM_STATE_ERR_BLOCKED_PENDING_RESET] =
		"pending reset",
	[MLXFW_FSM_STATE_ERR_MAX] =
		"unknown error"
};

static int mlxfw_fsm_state_wait(struct mlxfw_dev *mlxfw_dev, u32 fwhandle,
				enum mlxfw_fsm_state fsm_state)
=======
static const int mlxfw_fsm_state_errno[] = {
	[MLXFW_FSM_STATE_ERR_ERROR] = -EIO,
	[MLXFW_FSM_STATE_ERR_REJECTED_DIGEST_ERR] = -EBADMSG,
	[MLXFW_FSM_STATE_ERR_REJECTED_NOT_APPLICABLE] = -ENOENT,
	[MLXFW_FSM_STATE_ERR_REJECTED_UNKNOWN_KEY] = -ENOKEY,
	[MLXFW_FSM_STATE_ERR_REJECTED_AUTH_FAILED] = -EACCES,
	[MLXFW_FSM_STATE_ERR_REJECTED_UNSIGNED] = -EKEYREVOKED,
	[MLXFW_FSM_STATE_ERR_REJECTED_KEY_NOT_APPLICABLE] = -EKEYREJECTED,
	[MLXFW_FSM_STATE_ERR_REJECTED_BAD_FORMAT] = -ENOEXEC,
	[MLXFW_FSM_STATE_ERR_BLOCKED_PENDING_RESET] = -EBUSY,
	[MLXFW_FSM_STATE_ERR_MAX] = -EINVAL
};

#define MLXFW_ERR_PRFX "Firmware flash failed: "
#define MLXFW_ERR_MSG(fwdev, extack, msg, err) do { \
	mlxfw_err(fwdev, "%s, err (%d)\n", MLXFW_ERR_PRFX msg, err); \
	NL_SET_ERR_MSG_MOD(extack, MLXFW_ERR_PRFX msg); \
} while (0)

static int mlxfw_fsm_state_err(struct mlxfw_dev *mlxfw_dev,
			       struct netlink_ext_ack *extack,
			       enum mlxfw_fsm_state_err err)
{
	enum mlxfw_fsm_state_err fsm_state_err;

	fsm_state_err = min_t(enum mlxfw_fsm_state_err, err,
			      MLXFW_FSM_STATE_ERR_MAX);

	switch (fsm_state_err) {
	case MLXFW_FSM_STATE_ERR_ERROR:
		MLXFW_ERR_MSG(mlxfw_dev, extack, "general error", err);
		break;
	case MLXFW_FSM_STATE_ERR_REJECTED_DIGEST_ERR:
		MLXFW_ERR_MSG(mlxfw_dev, extack, "component hash mismatch", err);
		break;
	case MLXFW_FSM_STATE_ERR_REJECTED_NOT_APPLICABLE:
		MLXFW_ERR_MSG(mlxfw_dev, extack, "component not applicable", err);
		break;
	case MLXFW_FSM_STATE_ERR_REJECTED_UNKNOWN_KEY:
		MLXFW_ERR_MSG(mlxfw_dev, extack, "unknown key", err);
		break;
	case MLXFW_FSM_STATE_ERR_REJECTED_AUTH_FAILED:
		MLXFW_ERR_MSG(mlxfw_dev, extack, "authentication failed", err);
		break;
	case MLXFW_FSM_STATE_ERR_REJECTED_UNSIGNED:
		MLXFW_ERR_MSG(mlxfw_dev, extack, "component was not signed", err);
		break;
	case MLXFW_FSM_STATE_ERR_REJECTED_KEY_NOT_APPLICABLE:
		MLXFW_ERR_MSG(mlxfw_dev, extack, "key not applicable", err);
		break;
	case MLXFW_FSM_STATE_ERR_REJECTED_BAD_FORMAT:
		MLXFW_ERR_MSG(mlxfw_dev, extack, "bad format", err);
		break;
	case MLXFW_FSM_STATE_ERR_BLOCKED_PENDING_RESET:
		MLXFW_ERR_MSG(mlxfw_dev, extack, "pending reset", err);
		break;
	case MLXFW_FSM_STATE_ERR_OK:
	case MLXFW_FSM_STATE_ERR_MAX:
		MLXFW_ERR_MSG(mlxfw_dev, extack, "unknown error", err);
		break;
	}

	return mlxfw_fsm_state_errno[fsm_state_err];
};

static int mlxfw_fsm_state_wait(struct mlxfw_dev *mlxfw_dev, u32 fwhandle,
				enum mlxfw_fsm_state fsm_state,
				struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	enum mlxfw_fsm_state_err fsm_state_err;
	enum mlxfw_fsm_state curr_fsm_state;
	int times;
	int err;

	times = MLXFW_FSM_STATE_WAIT_ROUNDS;
retry:
	err = mlxfw_dev->ops->fsm_query_state(mlxfw_dev, fwhandle,
					      &curr_fsm_state, &fsm_state_err);
<<<<<<< HEAD
	if (err)
		return err;

	if (fsm_state_err != MLXFW_FSM_STATE_ERR_OK) {
		fsm_state_err = min_t(enum mlxfw_fsm_state_err,
				      fsm_state_err, MLXFW_FSM_STATE_ERR_MAX);
		pr_err("Firmware flash failed: %s\n",
		       mlxfw_fsm_state_err_str[fsm_state_err]);
		return -EINVAL;
	}
	if (curr_fsm_state != fsm_state) {
		if (--times == 0) {
			pr_err("Timeout reached on FSM state change");
=======
	if (err) {
		MLXFW_ERR_MSG(mlxfw_dev, extack, "FSM state query failed", err);
		return err;
	}

	if (fsm_state_err != MLXFW_FSM_STATE_ERR_OK)
		return mlxfw_fsm_state_err(mlxfw_dev, extack, fsm_state_err);

	if (curr_fsm_state != fsm_state) {
		if (--times == 0) {
			MLXFW_ERR_MSG(mlxfw_dev, extack,
				      "Timeout reached on FSM state change", -ETIMEDOUT);
>>>>>>> upstream/android-13
			return -ETIMEDOUT;
		}
		msleep(MLXFW_FSM_STATE_WAIT_CYCLE_MS);
		goto retry;
	}
	return 0;
}

<<<<<<< HEAD
=======
static int
mlxfw_fsm_reactivate_err(struct mlxfw_dev *mlxfw_dev,
			 struct netlink_ext_ack *extack, u8 err)
{
	enum mlxfw_fsm_reactivate_status status;

#define MXFW_REACT_PRFX "Reactivate FSM: "
#define MLXFW_REACT_ERR(msg, err) \
	MLXFW_ERR_MSG(mlxfw_dev, extack, MXFW_REACT_PRFX msg, err)

	status = min_t(enum mlxfw_fsm_reactivate_status, err,
		       MLXFW_FSM_REACTIVATE_STATUS_MAX);

	switch (status) {
	case MLXFW_FSM_REACTIVATE_STATUS_BUSY:
		MLXFW_REACT_ERR("busy", err);
		break;
	case MLXFW_FSM_REACTIVATE_STATUS_PROHIBITED_FW_VER_ERR:
		MLXFW_REACT_ERR("prohibited fw ver", err);
		break;
	case MLXFW_FSM_REACTIVATE_STATUS_FIRST_PAGE_COPY_FAILED:
		MLXFW_REACT_ERR("first page copy failed", err);
		break;
	case MLXFW_FSM_REACTIVATE_STATUS_FIRST_PAGE_ERASE_FAILED:
		MLXFW_REACT_ERR("first page erase failed", err);
		break;
	case MLXFW_FSM_REACTIVATE_STATUS_FIRST_PAGE_RESTORE_FAILED:
		MLXFW_REACT_ERR("first page restore failed", err);
		break;
	case MLXFW_FSM_REACTIVATE_STATUS_CANDIDATE_FW_DEACTIVATION_FAILED:
		MLXFW_REACT_ERR("candidate fw deactivation failed", err);
		break;
	case MLXFW_FSM_REACTIVATE_STATUS_ERR_DEVICE_RESET_REQUIRED:
		MLXFW_REACT_ERR("device reset required", err);
		break;
	case MLXFW_FSM_REACTIVATE_STATUS_ERR_FW_PROGRAMMING_NEEDED:
		MLXFW_REACT_ERR("fw programming needed", err);
		break;
	case MLXFW_FSM_REACTIVATE_STATUS_FW_ALREADY_ACTIVATED:
		MLXFW_REACT_ERR("fw already activated", err);
		break;
	case MLXFW_FSM_REACTIVATE_STATUS_OK:
	case MLXFW_FSM_REACTIVATE_STATUS_MAX:
		MLXFW_REACT_ERR("unexpected error", err);
		break;
	}
	return -EREMOTEIO;
};

static int mlxfw_fsm_reactivate(struct mlxfw_dev *mlxfw_dev,
				struct netlink_ext_ack *extack,
				bool *supported)
{
	u8 status;
	int err;

	if (!mlxfw_dev->ops->fsm_reactivate)
		return 0;

	err = mlxfw_dev->ops->fsm_reactivate(mlxfw_dev, &status);
	if (err == -EOPNOTSUPP) {
		*supported = false;
		return 0;
	}

	if (err) {
		MLXFW_ERR_MSG(mlxfw_dev, extack,
			      "Could not reactivate firmware flash", err);
		return err;
	}

	if (status == MLXFW_FSM_REACTIVATE_STATUS_OK ||
	    status == MLXFW_FSM_REACTIVATE_STATUS_FW_ALREADY_ACTIVATED)
		return 0;

	return mlxfw_fsm_reactivate_err(mlxfw_dev, extack, status);
}

static void mlxfw_status_notify(struct mlxfw_dev *mlxfw_dev,
				const char *msg, const char *comp_name,
				u32 done_bytes, u32 total_bytes)
{
	devlink_flash_update_status_notify(mlxfw_dev->devlink, msg, comp_name,
					   done_bytes, total_bytes);
}

>>>>>>> upstream/android-13
#define MLXFW_ALIGN_DOWN(x, align_bits) ((x) & ~((1 << (align_bits)) - 1))
#define MLXFW_ALIGN_UP(x, align_bits) \
		MLXFW_ALIGN_DOWN((x) + ((1 << (align_bits)) - 1), (align_bits))

static int mlxfw_flash_component(struct mlxfw_dev *mlxfw_dev,
				 u32 fwhandle,
<<<<<<< HEAD
				 struct mlxfw_mfa2_component *comp)
=======
				 struct mlxfw_mfa2_component *comp,
				 bool reactivate_supp,
				 struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	u16 comp_max_write_size;
	u8 comp_align_bits;
	u32 comp_max_size;
<<<<<<< HEAD
=======
	char comp_name[8];
>>>>>>> upstream/android-13
	u16 block_size;
	u8 *block_ptr;
	u32 offset;
	int err;

<<<<<<< HEAD
	err = mlxfw_dev->ops->component_query(mlxfw_dev, comp->index,
					      &comp_max_size, &comp_align_bits,
					      &comp_max_write_size);
	if (err)
		return err;

	comp_max_size = min_t(u32, comp_max_size, MLXFW_FSM_MAX_COMPONENT_SIZE);
	if (comp->data_size > comp_max_size) {
		pr_err("Component %d is of size %d which is bigger than limit %d\n",
		       comp->index, comp->data_size, comp_max_size);
=======
	sprintf(comp_name, "%u", comp->index);

	err = mlxfw_dev->ops->component_query(mlxfw_dev, comp->index,
					      &comp_max_size, &comp_align_bits,
					      &comp_max_write_size);
	if (err) {
		MLXFW_ERR_MSG(mlxfw_dev, extack, "FSM component query failed", err);
		return err;
	}

	comp_max_size = min_t(u32, comp_max_size, MLXFW_FSM_MAX_COMPONENT_SIZE);
	if (comp->data_size > comp_max_size) {
		MLXFW_ERR_MSG(mlxfw_dev, extack,
			      "Component size is bigger than limit", -EINVAL);
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	comp_max_write_size = MLXFW_ALIGN_DOWN(comp_max_write_size,
					       comp_align_bits);

<<<<<<< HEAD
	pr_debug("Component update\n");
	err = mlxfw_dev->ops->fsm_component_update(mlxfw_dev, fwhandle,
						   comp->index,
						   comp->data_size);
	if (err)
		return err;

	err = mlxfw_fsm_state_wait(mlxfw_dev, fwhandle,
				   MLXFW_FSM_STATE_DOWNLOAD);
	if (err)
		goto err_out;

	pr_debug("Component download\n");
=======
	mlxfw_dbg(mlxfw_dev, "Component update\n");
	mlxfw_status_notify(mlxfw_dev, "Updating component", comp_name, 0, 0);
	err = mlxfw_dev->ops->fsm_component_update(mlxfw_dev, fwhandle,
						   comp->index,
						   comp->data_size);
	if (err) {
		if (!reactivate_supp)
			MLXFW_ERR_MSG(mlxfw_dev, extack,
				      "FSM component update failed, FW reactivate is not supported",
				      err);
		else
			MLXFW_ERR_MSG(mlxfw_dev, extack,
				      "FSM component update failed", err);
		return err;
	}

	err = mlxfw_fsm_state_wait(mlxfw_dev, fwhandle,
				   MLXFW_FSM_STATE_DOWNLOAD, extack);
	if (err)
		goto err_out;

	mlxfw_dbg(mlxfw_dev, "Component download\n");
	mlxfw_status_notify(mlxfw_dev, "Downloading component",
			    comp_name, 0, comp->data_size);
>>>>>>> upstream/android-13
	for (offset = 0;
	     offset < MLXFW_ALIGN_UP(comp->data_size, comp_align_bits);
	     offset += comp_max_write_size) {
		block_ptr = comp->data + offset;
		block_size = (u16) min_t(u32, comp->data_size - offset,
					 comp_max_write_size);
		err = mlxfw_dev->ops->fsm_block_download(mlxfw_dev, fwhandle,
							 block_ptr, block_size,
							 offset);
<<<<<<< HEAD
		if (err)
			goto err_out;
	}

	pr_debug("Component verify\n");
	err = mlxfw_dev->ops->fsm_component_verify(mlxfw_dev, fwhandle,
						   comp->index);
	if (err)
		goto err_out;

	err = mlxfw_fsm_state_wait(mlxfw_dev, fwhandle, MLXFW_FSM_STATE_LOCKED);
=======
		if (err) {
			MLXFW_ERR_MSG(mlxfw_dev, extack,
				      "Component download failed", err);
			goto err_out;
		}
		mlxfw_status_notify(mlxfw_dev, "Downloading component",
				    comp_name, offset + block_size,
				    comp->data_size);
	}

	mlxfw_dbg(mlxfw_dev, "Component verify\n");
	mlxfw_status_notify(mlxfw_dev, "Verifying component", comp_name, 0, 0);
	err = mlxfw_dev->ops->fsm_component_verify(mlxfw_dev, fwhandle,
						   comp->index);
	if (err) {
		MLXFW_ERR_MSG(mlxfw_dev, extack,
			      "FSM component verify failed", err);
		goto err_out;
	}

	err = mlxfw_fsm_state_wait(mlxfw_dev, fwhandle,
				   MLXFW_FSM_STATE_LOCKED, extack);
>>>>>>> upstream/android-13
	if (err)
		goto err_out;
	return 0;

err_out:
	mlxfw_dev->ops->fsm_cancel(mlxfw_dev, fwhandle);
	return err;
}

static int mlxfw_flash_components(struct mlxfw_dev *mlxfw_dev, u32 fwhandle,
<<<<<<< HEAD
				  struct mlxfw_mfa2_file *mfa2_file)
=======
				  struct mlxfw_mfa2_file *mfa2_file,
				  bool reactivate_supp,
				  struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	u32 component_count;
	int err;
	int i;

	err = mlxfw_mfa2_file_component_count(mfa2_file, mlxfw_dev->psid,
					      mlxfw_dev->psid_size,
					      &component_count);
	if (err) {
<<<<<<< HEAD
		pr_err("Could not find device PSID in MFA2 file\n");
=======
		MLXFW_ERR_MSG(mlxfw_dev, extack,
			      "Could not find device PSID in MFA2 file", err);
>>>>>>> upstream/android-13
		return err;
	}

	for (i = 0; i < component_count; i++) {
		struct mlxfw_mfa2_component *comp;

		comp = mlxfw_mfa2_file_component_get(mfa2_file, mlxfw_dev->psid,
						     mlxfw_dev->psid_size, i);
<<<<<<< HEAD
		if (IS_ERR(comp))
			return PTR_ERR(comp);

		pr_info("Flashing component type %d\n", comp->index);
		err = mlxfw_flash_component(mlxfw_dev, fwhandle, comp);
=======
		if (IS_ERR(comp)) {
			err = PTR_ERR(comp);
			MLXFW_ERR_MSG(mlxfw_dev, extack,
				      "Failed to get MFA2 component", err);
			return err;
		}

		mlxfw_info(mlxfw_dev, "Flashing component type %d\n",
			   comp->index);
		err = mlxfw_flash_component(mlxfw_dev, fwhandle, comp,
					    reactivate_supp, extack);
>>>>>>> upstream/android-13
		mlxfw_mfa2_file_component_put(comp);
		if (err)
			return err;
	}
	return 0;
}

int mlxfw_firmware_flash(struct mlxfw_dev *mlxfw_dev,
<<<<<<< HEAD
			 const struct firmware *firmware)
{
	struct mlxfw_mfa2_file *mfa2_file;
=======
			 const struct firmware *firmware,
			 struct netlink_ext_ack *extack)
{
	struct mlxfw_mfa2_file *mfa2_file;
	bool reactivate_supp = true;
>>>>>>> upstream/android-13
	u32 fwhandle;
	int err;

	if (!mlxfw_mfa2_check(firmware)) {
<<<<<<< HEAD
		pr_err("Firmware file is not MFA2\n");
=======
		MLXFW_ERR_MSG(mlxfw_dev, extack,
			      "Firmware file is not MFA2", -EINVAL);
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	mfa2_file = mlxfw_mfa2_file_init(firmware);
<<<<<<< HEAD
	if (IS_ERR(mfa2_file))
		return PTR_ERR(mfa2_file);

	pr_info("Initialize firmware flash process\n");
	err = mlxfw_dev->ops->fsm_lock(mlxfw_dev, &fwhandle);
	if (err) {
		pr_err("Could not lock the firmware FSM\n");
=======
	if (IS_ERR(mfa2_file)) {
		err = PTR_ERR(mfa2_file);
		MLXFW_ERR_MSG(mlxfw_dev, extack,
			      "Failed to initialize MFA2 firmware file", err);
		return err;
	}

	mlxfw_info(mlxfw_dev, "Initialize firmware flash process\n");
	mlxfw_status_notify(mlxfw_dev, "Initializing firmware flash process",
			    NULL, 0, 0);
	err = mlxfw_dev->ops->fsm_lock(mlxfw_dev, &fwhandle);
	if (err) {
		MLXFW_ERR_MSG(mlxfw_dev, extack,
			      "Could not lock the firmware FSM", err);
>>>>>>> upstream/android-13
		goto err_fsm_lock;
	}

	err = mlxfw_fsm_state_wait(mlxfw_dev, fwhandle,
<<<<<<< HEAD
				   MLXFW_FSM_STATE_LOCKED);
	if (err)
		goto err_state_wait_idle_to_locked;

	err = mlxfw_flash_components(mlxfw_dev, fwhandle, mfa2_file);
	if (err)
		goto err_flash_components;

	pr_debug("Activate image\n");
	err = mlxfw_dev->ops->fsm_activate(mlxfw_dev, fwhandle);
	if (err) {
		pr_err("Could not activate the downloaded image\n");
		goto err_fsm_activate;
	}

	err = mlxfw_fsm_state_wait(mlxfw_dev, fwhandle, MLXFW_FSM_STATE_LOCKED);
	if (err)
		goto err_state_wait_activate_to_locked;

	pr_debug("Handle release\n");
	mlxfw_dev->ops->fsm_release(mlxfw_dev, fwhandle);

	pr_info("Firmware flash done.\n");
=======
				   MLXFW_FSM_STATE_LOCKED, extack);
	if (err)
		goto err_state_wait_idle_to_locked;

	err = mlxfw_fsm_reactivate(mlxfw_dev, extack, &reactivate_supp);
	if (err)
		goto err_fsm_reactivate;

	err = mlxfw_fsm_state_wait(mlxfw_dev, fwhandle,
				   MLXFW_FSM_STATE_LOCKED, extack);
	if (err)
		goto err_state_wait_reactivate_to_locked;

	err = mlxfw_flash_components(mlxfw_dev, fwhandle, mfa2_file,
				     reactivate_supp, extack);
	if (err)
		goto err_flash_components;

	mlxfw_dbg(mlxfw_dev, "Activate image\n");
	mlxfw_status_notify(mlxfw_dev, "Activating image", NULL, 0, 0);
	err = mlxfw_dev->ops->fsm_activate(mlxfw_dev, fwhandle);
	if (err) {
		MLXFW_ERR_MSG(mlxfw_dev, extack,
			      "Could not activate the downloaded image", err);
		goto err_fsm_activate;
	}

	err = mlxfw_fsm_state_wait(mlxfw_dev, fwhandle,
				   MLXFW_FSM_STATE_LOCKED, extack);
	if (err)
		goto err_state_wait_activate_to_locked;

	mlxfw_dbg(mlxfw_dev, "Handle release\n");
	mlxfw_dev->ops->fsm_release(mlxfw_dev, fwhandle);

	mlxfw_info(mlxfw_dev, "Firmware flash done\n");
	mlxfw_status_notify(mlxfw_dev, "Firmware flash done", NULL, 0, 0);
>>>>>>> upstream/android-13
	mlxfw_mfa2_file_fini(mfa2_file);
	return 0;

err_state_wait_activate_to_locked:
err_fsm_activate:
err_flash_components:
<<<<<<< HEAD
=======
err_state_wait_reactivate_to_locked:
err_fsm_reactivate:
>>>>>>> upstream/android-13
err_state_wait_idle_to_locked:
	mlxfw_dev->ops->fsm_release(mlxfw_dev, fwhandle);
err_fsm_lock:
	mlxfw_mfa2_file_fini(mfa2_file);
	return err;
}
EXPORT_SYMBOL(mlxfw_firmware_flash);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Yotam Gigi <yotamg@mellanox.com>");
MODULE_DESCRIPTION("Mellanox firmware flash lib");
