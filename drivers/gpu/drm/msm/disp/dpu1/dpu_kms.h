<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2015-2018, The Linux Foundation. All rights reserved.
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#ifndef __DPU_KMS_H__
#define __DPU_KMS_H__

<<<<<<< HEAD
=======
#include <linux/interconnect.h>

#include <drm/drm_drv.h>

>>>>>>> upstream/android-13
#include "msm_drv.h"
#include "msm_kms.h"
#include "msm_mmu.h"
#include "msm_gem.h"
<<<<<<< HEAD
#include "dpu_dbg.h"
=======
>>>>>>> upstream/android-13
#include "dpu_hw_catalog.h"
#include "dpu_hw_ctl.h"
#include "dpu_hw_lm.h"
#include "dpu_hw_interrupts.h"
#include "dpu_hw_top.h"
<<<<<<< HEAD
#include "dpu_rm.h"
#include "dpu_power_handle.h"
#include "dpu_irq.h"
=======
#include "dpu_io_util.h"
#include "dpu_rm.h"
>>>>>>> upstream/android-13
#include "dpu_core_perf.h"

#define DRMID(x) ((x) ? (x)->base.id : -1)

/**
 * DPU_DEBUG - macro for kms/plane/crtc/encoder/connector logs
 * @fmt: Pointer to format string
 */
#define DPU_DEBUG(fmt, ...)                                                \
	do {                                                               \
<<<<<<< HEAD
		if (unlikely(drm_debug & DRM_UT_KMS))                      \
=======
		if (drm_debug_enabled(DRM_UT_KMS))                         \
>>>>>>> upstream/android-13
			DRM_DEBUG(fmt, ##__VA_ARGS__); \
		else                                                       \
			pr_debug(fmt, ##__VA_ARGS__);                      \
	} while (0)

/**
 * DPU_DEBUG_DRIVER - macro for hardware driver logging
 * @fmt: Pointer to format string
 */
#define DPU_DEBUG_DRIVER(fmt, ...)                                         \
	do {                                                               \
<<<<<<< HEAD
		if (unlikely(drm_debug & DRM_UT_DRIVER))                   \
=======
		if (drm_debug_enabled(DRM_UT_DRIVER))                      \
>>>>>>> upstream/android-13
			DRM_ERROR(fmt, ##__VA_ARGS__); \
		else                                                       \
			pr_debug(fmt, ##__VA_ARGS__);                      \
	} while (0)

#define DPU_ERROR(fmt, ...) pr_err("[dpu error]" fmt, ##__VA_ARGS__)

/**
 * ktime_compare_safe - compare two ktime structures
 *	This macro is similar to the standard ktime_compare() function, but
 *	attempts to also handle ktime overflows.
 * @A: First ktime value
 * @B: Second ktime value
 * Returns: -1 if A < B, 0 if A == B, 1 if A > B
 */
#define ktime_compare_safe(A, B) \
	ktime_compare(ktime_sub((A), (B)), ktime_set(0, 0))

#define DPU_NAME_SIZE  12

<<<<<<< HEAD
/* timeout in frames waiting for frame done */
#define DPU_FRAME_DONE_TIMEOUT	60

=======
>>>>>>> upstream/android-13
/*
 * struct dpu_irq_callback - IRQ callback handlers
 * @list: list to callback
 * @func: intr handler
 * @arg: argument for the handler
 */
struct dpu_irq_callback {
	struct list_head list;
	void (*func)(void *arg, int irq_idx);
	void *arg;
};

/**
 * struct dpu_irq: IRQ structure contains callback registration info
 * @total_irq:    total number of irq_idx obtained from HW interrupts mapping
 * @irq_cb_tbl:   array of IRQ callbacks setting
<<<<<<< HEAD
 * @enable_counts array of IRQ enable counts
 * @cb_lock:      callback lock
=======
>>>>>>> upstream/android-13
 * @debugfs_file: debugfs file for irq statistics
 */
struct dpu_irq {
	u32 total_irqs;
	struct list_head *irq_cb_tbl;
<<<<<<< HEAD
	atomic_t *enable_counts;
	atomic_t *irq_counts;
	spinlock_t cb_lock;
	struct dentry *debugfs_file;
=======
	atomic_t *irq_counts;
>>>>>>> upstream/android-13
};

struct dpu_kms {
	struct msm_kms base;
	struct drm_device *dev;
	int core_rev;
	struct dpu_mdss_cfg *catalog;

<<<<<<< HEAD
	struct dpu_power_handle phandle;
	struct dpu_power_client *core_client;
	struct dpu_power_event *power_event;

	/* directory entry for debugfs */
	struct dentry *debugfs_root;
	struct dentry *debugfs_danger;
	struct dentry *debugfs_vbif;

	/* io/register spaces: */
	void __iomem *mmio, *vbif[VBIF_MAX], *reg_dma;
	unsigned long mmio_len, vbif_len[VBIF_MAX], reg_dma_len;
=======
	/* io/register spaces: */
	void __iomem *mmio, *vbif[VBIF_MAX], *reg_dma;
>>>>>>> upstream/android-13

	struct regulator *vdd;
	struct regulator *mmagic;
	struct regulator *venus;

	struct dpu_hw_intr *hw_intr;
	struct dpu_irq irq_obj;

	struct dpu_core_perf perf;

<<<<<<< HEAD
	/* saved atomic state during system suspend */
	struct drm_atomic_state *suspend_state;
	bool suspend_block;
=======
	/*
	 * Global private object state, Do not access directly, use
	 * dpu_kms_global_get_state()
	 */
	struct drm_modeset_lock global_state_lock;
	struct drm_private_obj global_state;
>>>>>>> upstream/android-13

	struct dpu_rm rm;
	bool rm_init;

	struct dpu_hw_vbif *hw_vbif[VBIF_MAX];
	struct dpu_hw_mdp *hw_mdp;

	bool has_danger_ctrl;

	struct platform_device *pdev;
	bool rpm_enabled;
<<<<<<< HEAD
	struct dss_module_power mp;
=======

	struct dss_module_power mp;

	/* reference count bandwidth requests, so we know when we can
	 * release bandwidth.  Each atomic update increments, and frame-
	 * done event decrements.  Additionally, for video mode, the
	 * reference is incremented when crtc is enabled, and decremented
	 * when disabled.
	 */
	atomic_t bandwidth_ref;
	struct icc_path *path[2];
	u32 num_paths;
>>>>>>> upstream/android-13
};

struct vsync_info {
	u32 frame_count;
	u32 line_count;
};

#define to_dpu_kms(x) container_of(x, struct dpu_kms, base)

<<<<<<< HEAD
/* get struct msm_kms * from drm_device * */
#define ddev_to_msm_kms(D) ((D) && (D)->dev_private ? \
		((struct msm_drm_private *)((D)->dev_private))->kms : NULL)

/**
 * dpu_kms_is_suspend_state - whether or not the system is pm suspended
 * @dev: Pointer to drm device
 * Return: Suspend status
 */
static inline bool dpu_kms_is_suspend_state(struct drm_device *dev)
{
	if (!ddev_to_msm_kms(dev))
		return false;

	return to_dpu_kms(ddev_to_msm_kms(dev))->suspend_state != NULL;
}

/**
 * dpu_kms_is_suspend_blocked - whether or not commits are blocked due to pm
 *				suspend status
 * @dev: Pointer to drm device
 * Return: True if commits should be rejected due to pm suspend
 */
static inline bool dpu_kms_is_suspend_blocked(struct drm_device *dev)
{
	if (!dpu_kms_is_suspend_state(dev))
		return false;

	return to_dpu_kms(ddev_to_msm_kms(dev))->suspend_block;
}
=======
#define to_dpu_global_state(x) container_of(x, struct dpu_global_state, base)

/* Global private object state for tracking resources that are shared across
 * multiple kms objects (planes/crtcs/etc).
 */
struct dpu_global_state {
	struct drm_private_state base;

	uint32_t pingpong_to_enc_id[PINGPONG_MAX - PINGPONG_0];
	uint32_t mixer_to_enc_id[LM_MAX - LM_0];
	uint32_t ctl_to_enc_id[CTL_MAX - CTL_0];
	uint32_t intf_to_enc_id[INTF_MAX - INTF_0];
	uint32_t dspp_to_enc_id[DSPP_MAX - DSPP_0];
};

struct dpu_global_state
	*dpu_kms_get_existing_global_state(struct dpu_kms *dpu_kms);
struct dpu_global_state
	*__must_check dpu_kms_get_global_state(struct drm_atomic_state *s);
>>>>>>> upstream/android-13

/**
 * Debugfs functions - extra helper functions for debugfs support
 *
 * Main debugfs documentation is located at,
 *
<<<<<<< HEAD
 * Documentation/filesystems/debugfs.txt
=======
 * Documentation/filesystems/debugfs.rst
>>>>>>> upstream/android-13
 *
 * @dpu_debugfs_setup_regset32: Initialize data for dpu_debugfs_create_regset32
 * @dpu_debugfs_create_regset32: Create 32-bit register dump file
 * @dpu_debugfs_get_root: Get root dentry for DPU_KMS's debugfs node
 */

/**
 * Companion structure for dpu_debugfs_create_regset32. Do not initialize the
 * members of this structure explicitly; use dpu_debugfs_setup_regset32 instead.
 */
struct dpu_debugfs_regset32 {
	uint32_t offset;
	uint32_t blk_len;
	struct dpu_kms *dpu_kms;
};

/**
 * dpu_debugfs_setup_regset32 - Initialize register block definition for debugfs
 * This function is meant to initialize dpu_debugfs_regset32 structures for use
 * with dpu_debugfs_create_regset32.
 * @regset: opaque register definition structure
 * @offset: sub-block offset
 * @length: sub-block length, in bytes
 * @dpu_kms: pointer to dpu kms structure
 */
void dpu_debugfs_setup_regset32(struct dpu_debugfs_regset32 *regset,
		uint32_t offset, uint32_t length, struct dpu_kms *dpu_kms);

/**
 * dpu_debugfs_create_regset32 - Create register read back file for debugfs
 *
 * This function is almost identical to the standard debugfs_create_regset32()
 * function, with the main difference being that a list of register
 * names/offsets do not need to be provided. The 'read' function simply outputs
 * sequential register values over a specified range.
 *
 * Similar to the related debugfs_create_regset32 API, the structure pointed to
 * by regset needs to persist for the lifetime of the created file. The calling
 * code is responsible for initialization/management of this structure.
 *
 * The structure pointed to by regset is meant to be opaque. Please use
 * dpu_debugfs_setup_regset32 to initialize it.
 *
 * @name:   File name within debugfs
 * @mode:   File mode within debugfs
 * @parent: Parent directory entry within debugfs, can be NULL
 * @regset: Pointer to persistent register block definition
<<<<<<< HEAD
 *
 * Return: dentry pointer for newly created file, use either debugfs_remove()
 *         or debugfs_remove_recursive() (on a parent directory) to remove the
 *         file
 */
void *dpu_debugfs_create_regset32(const char *name, umode_t mode,
=======
 */
void dpu_debugfs_create_regset32(const char *name, umode_t mode,
>>>>>>> upstream/android-13
		void *parent, struct dpu_debugfs_regset32 *regset);

/**
 * dpu_debugfs_get_root - Return root directory entry for KMS's debugfs
 *
 * The return value should be passed as the 'parent' argument to subsequent
 * debugfs create calls.
 *
 * @dpu_kms: Pointer to DPU's KMS structure
 *
 * Return: dentry pointer for DPU's debugfs location
 */
void *dpu_debugfs_get_root(struct dpu_kms *dpu_kms);

/**
 * DPU info management functions
 * These functions/definitions allow for building up a 'dpu_info' structure
 * containing one or more "key=value\n" entries.
 */
#define DPU_KMS_INFO_MAX_SIZE	4096

/**
 * Vblank enable/disable functions
 */
int dpu_enable_vblank(struct msm_kms *kms, struct drm_crtc *crtc);
void dpu_disable_vblank(struct msm_kms *kms, struct drm_crtc *crtc);

void dpu_kms_encoder_enable(struct drm_encoder *encoder);

/**
 * dpu_kms_get_clk_rate() - get the clock rate
<<<<<<< HEAD
 * @dpu_kms:  poiner to dpu_kms structure
=======
 * @dpu_kms:  pointer to dpu_kms structure
>>>>>>> upstream/android-13
 * @clock_name: clock name to get the rate
 *
 * Return: current clock rate
 */
u64 dpu_kms_get_clk_rate(struct dpu_kms *dpu_kms, char *clock_name);

#endif /* __dpu_kms_H__ */
