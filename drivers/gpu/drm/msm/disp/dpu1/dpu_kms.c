<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2014-2018, The Linux Foundation. All rights reserved.
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

#define pr_fmt(fmt)	"[drm:%s:%d] " fmt, __func__, __LINE__

<<<<<<< HEAD
#include <drm/drm_crtc.h>
#include <linux/debugfs.h>
#include <linux/of_irq.h>
#include <linux/dma-buf.h>
=======
#include <linux/debugfs.h>
#include <linux/dma-buf.h>
#include <linux/of_irq.h>
#include <linux/pm_opp.h>

#include <drm/drm_crtc.h>
#include <drm/drm_file.h>
#include <drm/drm_vblank.h>
>>>>>>> upstream/android-13

#include "msm_drv.h"
#include "msm_mmu.h"
#include "msm_gem.h"
<<<<<<< HEAD
=======
#include "disp/msm_disp_snapshot.h"
>>>>>>> upstream/android-13

#include "dpu_kms.h"
#include "dpu_core_irq.h"
#include "dpu_formats.h"
#include "dpu_hw_vbif.h"
#include "dpu_vbif.h"
#include "dpu_encoder.h"
#include "dpu_plane.h"
#include "dpu_crtc.h"

#define CREATE_TRACE_POINTS
#include "dpu_trace.h"

<<<<<<< HEAD
static const char * const iommu_ports[] = {
		"mdp_0",
};

=======
>>>>>>> upstream/android-13
/*
 * To enable overall DRM driver logging
 * # echo 0x2 > /sys/module/drm/parameters/debug
 *
 * To enable DRM driver h/w logging
 * # echo <mask> > /sys/kernel/debug/dri/0/debug/hw_log_mask
 *
 * See dpu_hw_mdss.h for h/w logging mask definitions (search for DPU_DBG_MASK_)
 */
#define DPU_DEBUGFS_DIR "msm_dpu"
#define DPU_DEBUGFS_HWMASKNAME "hw_log_mask"

<<<<<<< HEAD
static int dpu_kms_hw_init(struct msm_kms *kms);
static int _dpu_kms_mmu_destroy(struct dpu_kms *dpu_kms);

static unsigned long dpu_iomap_size(struct platform_device *pdev,
				    const char *name)
{
	struct resource *res;

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, name);
	if (!res) {
		DRM_ERROR("failed to get memory resource: %s\n", name);
		return 0;
	}

	return resource_size(res);
}
=======
#define MIN_IB_BW	400000000ULL /* Min ib vote 400MB */

static int dpu_kms_hw_init(struct msm_kms *kms);
static void _dpu_kms_mmu_destroy(struct dpu_kms *dpu_kms);
>>>>>>> upstream/android-13

#ifdef CONFIG_DEBUG_FS
static int _dpu_danger_signal_status(struct seq_file *s,
		bool danger_status)
{
	struct dpu_kms *kms = (struct dpu_kms *)s->private;
<<<<<<< HEAD
	struct msm_drm_private *priv;
	struct dpu_danger_safe_status status;
	int i;

	if (!kms || !kms->dev || !kms->dev->dev_private || !kms->hw_mdp) {
=======
	struct dpu_danger_safe_status status;
	int i;

	if (!kms->hw_mdp) {
>>>>>>> upstream/android-13
		DPU_ERROR("invalid arg(s)\n");
		return 0;
	}

<<<<<<< HEAD
	priv = kms->dev->dev_private;
=======
>>>>>>> upstream/android-13
	memset(&status, 0, sizeof(struct dpu_danger_safe_status));

	pm_runtime_get_sync(&kms->pdev->dev);
	if (danger_status) {
		seq_puts(s, "\nDanger signal status:\n");
		if (kms->hw_mdp->ops.get_danger_status)
			kms->hw_mdp->ops.get_danger_status(kms->hw_mdp,
					&status);
	} else {
		seq_puts(s, "\nSafe signal status:\n");
<<<<<<< HEAD
		if (kms->hw_mdp->ops.get_danger_status)
			kms->hw_mdp->ops.get_danger_status(kms->hw_mdp,
=======
		if (kms->hw_mdp->ops.get_safe_status)
			kms->hw_mdp->ops.get_safe_status(kms->hw_mdp,
>>>>>>> upstream/android-13
					&status);
	}
	pm_runtime_put_sync(&kms->pdev->dev);

	seq_printf(s, "MDP     :  0x%x\n", status.mdp);

	for (i = SSPP_VIG0; i < SSPP_MAX; i++)
		seq_printf(s, "SSPP%d   :  0x%x  \t", i - SSPP_VIG0,
				status.sspp[i]);
	seq_puts(s, "\n");

	return 0;
}

<<<<<<< HEAD
#define DEFINE_DPU_DEBUGFS_SEQ_FOPS(__prefix)				\
static int __prefix ## _open(struct inode *inode, struct file *file)	\
{									\
	return single_open(file, __prefix ## _show, inode->i_private);	\
}									\
static const struct file_operations __prefix ## _fops = {		\
	.owner = THIS_MODULE,						\
	.open = __prefix ## _open,					\
	.release = single_release,					\
	.read = seq_read,						\
	.llseek = seq_lseek,						\
}

=======
>>>>>>> upstream/android-13
static int dpu_debugfs_danger_stats_show(struct seq_file *s, void *v)
{
	return _dpu_danger_signal_status(s, true);
}
<<<<<<< HEAD
DEFINE_DPU_DEBUGFS_SEQ_FOPS(dpu_debugfs_danger_stats);
=======
DEFINE_SHOW_ATTRIBUTE(dpu_debugfs_danger_stats);
>>>>>>> upstream/android-13

static int dpu_debugfs_safe_stats_show(struct seq_file *s, void *v)
{
	return _dpu_danger_signal_status(s, false);
}
<<<<<<< HEAD
DEFINE_DPU_DEBUGFS_SEQ_FOPS(dpu_debugfs_safe_stats);

static void dpu_debugfs_danger_destroy(struct dpu_kms *dpu_kms)
{
	debugfs_remove_recursive(dpu_kms->debugfs_danger);
	dpu_kms->debugfs_danger = NULL;
}

static int dpu_debugfs_danger_init(struct dpu_kms *dpu_kms,
		struct dentry *parent)
{
	dpu_kms->debugfs_danger = debugfs_create_dir("danger",
			parent);
	if (!dpu_kms->debugfs_danger) {
		DPU_ERROR("failed to create danger debugfs\n");
		return -EINVAL;
	}

	debugfs_create_file("danger_status", 0600, dpu_kms->debugfs_danger,
			dpu_kms, &dpu_debugfs_danger_stats_fops);
	debugfs_create_file("safe_status", 0600, dpu_kms->debugfs_danger,
			dpu_kms, &dpu_debugfs_safe_stats_fops);

	return 0;
=======
DEFINE_SHOW_ATTRIBUTE(dpu_debugfs_safe_stats);

static void dpu_debugfs_danger_init(struct dpu_kms *dpu_kms,
		struct dentry *parent)
{
	struct dentry *entry = debugfs_create_dir("danger", parent);

	debugfs_create_file("danger_status", 0600, entry,
			dpu_kms, &dpu_debugfs_danger_stats_fops);
	debugfs_create_file("safe_status", 0600, entry,
			dpu_kms, &dpu_debugfs_safe_stats_fops);
>>>>>>> upstream/android-13
}

static int _dpu_debugfs_show_regset32(struct seq_file *s, void *data)
{
<<<<<<< HEAD
	struct dpu_debugfs_regset32 *regset;
	struct dpu_kms *dpu_kms;
	struct drm_device *dev;
	struct msm_drm_private *priv;
	void __iomem *base;
	uint32_t i, addr;

	if (!s || !s->private)
		return 0;

	regset = s->private;

	dpu_kms = regset->dpu_kms;
	if (!dpu_kms || !dpu_kms->mmio)
		return 0;

	dev = dpu_kms->dev;
	if (!dev)
		return 0;

	priv = dev->dev_private;
	if (!priv)
=======
	struct dpu_debugfs_regset32 *regset = s->private;
	struct dpu_kms *dpu_kms = regset->dpu_kms;
	void __iomem *base;
	uint32_t i, addr;

	if (!dpu_kms->mmio)
>>>>>>> upstream/android-13
		return 0;

	base = dpu_kms->mmio + regset->offset;

	/* insert padding spaces, if needed */
	if (regset->offset & 0xF) {
		seq_printf(s, "[%x]", regset->offset & ~0xF);
		for (i = 0; i < (regset->offset & 0xF); i += 4)
			seq_puts(s, "         ");
	}

	pm_runtime_get_sync(&dpu_kms->pdev->dev);

	/* main register output */
	for (i = 0; i < regset->blk_len; i += 4) {
		addr = regset->offset + i;
		if ((addr & 0xF) == 0x0)
			seq_printf(s, i ? "\n[%x]" : "[%x]", addr);
		seq_printf(s, " %08x", readl_relaxed(base + i));
	}
	seq_puts(s, "\n");
	pm_runtime_put_sync(&dpu_kms->pdev->dev);

	return 0;
}

static int dpu_debugfs_open_regset32(struct inode *inode,
		struct file *file)
{
	return single_open(file, _dpu_debugfs_show_regset32, inode->i_private);
}

static const struct file_operations dpu_fops_regset32 = {
	.open =		dpu_debugfs_open_regset32,
	.read =		seq_read,
	.llseek =	seq_lseek,
	.release =	single_release,
};

void dpu_debugfs_setup_regset32(struct dpu_debugfs_regset32 *regset,
		uint32_t offset, uint32_t length, struct dpu_kms *dpu_kms)
{
	if (regset) {
		regset->offset = offset;
		regset->blk_len = length;
		regset->dpu_kms = dpu_kms;
	}
}

<<<<<<< HEAD
void *dpu_debugfs_create_regset32(const char *name, umode_t mode,
		void *parent, struct dpu_debugfs_regset32 *regset)
{
	if (!name || !regset || !regset->dpu_kms || !regset->blk_len)
		return NULL;
=======
void dpu_debugfs_create_regset32(const char *name, umode_t mode,
		void *parent, struct dpu_debugfs_regset32 *regset)
{
	if (!name || !regset || !regset->dpu_kms || !regset->blk_len)
		return;
>>>>>>> upstream/android-13

	/* make sure offset is a multiple of 4 */
	regset->offset = round_down(regset->offset, 4);

<<<<<<< HEAD
	return debugfs_create_file(name, mode, parent,
			regset, &dpu_fops_regset32);
}

static int _dpu_debugfs_init(struct dpu_kms *dpu_kms)
{
	void *p;
	int rc;

	p = dpu_hw_util_get_log_mask_ptr();

	if (!dpu_kms || !p)
		return -EINVAL;

	dpu_kms->debugfs_root = debugfs_create_dir("debug",
					   dpu_kms->dev->primary->debugfs_root);
	if (IS_ERR_OR_NULL(dpu_kms->debugfs_root)) {
		DRM_ERROR("debugfs create_dir failed %ld\n",
			  PTR_ERR(dpu_kms->debugfs_root));
		return PTR_ERR(dpu_kms->debugfs_root);
	}

	rc = dpu_dbg_debugfs_register(dpu_kms->debugfs_root);
	if (rc) {
		DRM_ERROR("failed to reg dpu dbg debugfs: %d\n", rc);
		return rc;
	}

	/* allow root to be NULL */
	debugfs_create_x32(DPU_DEBUGFS_HWMASKNAME, 0600, dpu_kms->debugfs_root, p);

	(void) dpu_debugfs_danger_init(dpu_kms, dpu_kms->debugfs_root);
	(void) dpu_debugfs_vbif_init(dpu_kms, dpu_kms->debugfs_root);
	(void) dpu_debugfs_core_irq_init(dpu_kms, dpu_kms->debugfs_root);

	rc = dpu_core_perf_debugfs_init(&dpu_kms->perf, dpu_kms->debugfs_root);
	if (rc) {
		DPU_ERROR("failed to init perf %d\n", rc);
		return rc;
	}

	return 0;
}

static void _dpu_debugfs_destroy(struct dpu_kms *dpu_kms)
{
	/* don't need to NULL check debugfs_root */
	if (dpu_kms) {
		dpu_debugfs_vbif_destroy(dpu_kms);
		dpu_debugfs_danger_destroy(dpu_kms);
		dpu_debugfs_core_irq_destroy(dpu_kms);
		debugfs_remove_recursive(dpu_kms->debugfs_root);
	}
}
#else
static void _dpu_debugfs_destroy(struct dpu_kms *dpu_kms)
{
}
#endif
=======
	debugfs_create_file(name, mode, parent, regset, &dpu_fops_regset32);
}

static int dpu_kms_debugfs_init(struct msm_kms *kms, struct drm_minor *minor)
{
	struct dpu_kms *dpu_kms = to_dpu_kms(kms);
	void *p = dpu_hw_util_get_log_mask_ptr();
	struct dentry *entry;
	struct drm_device *dev;
	struct msm_drm_private *priv;

	if (!p)
		return -EINVAL;

	dev = dpu_kms->dev;
	priv = dev->dev_private;

	entry = debugfs_create_dir("debug", minor->debugfs_root);

	debugfs_create_x32(DPU_DEBUGFS_HWMASKNAME, 0600, entry, p);

	dpu_debugfs_danger_init(dpu_kms, entry);
	dpu_debugfs_vbif_init(dpu_kms, entry);
	dpu_debugfs_core_irq_init(dpu_kms, entry);

	if (priv->dp)
		msm_dp_debugfs_init(priv->dp, minor);

	return dpu_core_perf_debugfs_init(dpu_kms, entry);
}
#endif

/* Global/shared object state funcs */

/*
 * This is a helper that returns the private state currently in operation.
 * Note that this would return the "old_state" if called in the atomic check
 * path, and the "new_state" after the atomic swap has been done.
 */
struct dpu_global_state *
dpu_kms_get_existing_global_state(struct dpu_kms *dpu_kms)
{
	return to_dpu_global_state(dpu_kms->global_state.state);
}

/*
 * This acquires the modeset lock set aside for global state, creates
 * a new duplicated private object state.
 */
struct dpu_global_state *dpu_kms_get_global_state(struct drm_atomic_state *s)
{
	struct msm_drm_private *priv = s->dev->dev_private;
	struct dpu_kms *dpu_kms = to_dpu_kms(priv->kms);
	struct drm_private_state *priv_state;
	int ret;

	ret = drm_modeset_lock(&dpu_kms->global_state_lock, s->acquire_ctx);
	if (ret)
		return ERR_PTR(ret);

	priv_state = drm_atomic_get_private_obj_state(s,
						&dpu_kms->global_state);
	if (IS_ERR(priv_state))
		return ERR_CAST(priv_state);

	return to_dpu_global_state(priv_state);
}

static struct drm_private_state *
dpu_kms_global_duplicate_state(struct drm_private_obj *obj)
{
	struct dpu_global_state *state;

	state = kmemdup(obj->state, sizeof(*state), GFP_KERNEL);
	if (!state)
		return NULL;

	__drm_atomic_helper_private_obj_duplicate_state(obj, &state->base);

	return &state->base;
}

static void dpu_kms_global_destroy_state(struct drm_private_obj *obj,
				      struct drm_private_state *state)
{
	struct dpu_global_state *dpu_state = to_dpu_global_state(state);

	kfree(dpu_state);
}

static const struct drm_private_state_funcs dpu_kms_global_state_funcs = {
	.atomic_duplicate_state = dpu_kms_global_duplicate_state,
	.atomic_destroy_state = dpu_kms_global_destroy_state,
};

static int dpu_kms_global_obj_init(struct dpu_kms *dpu_kms)
{
	struct dpu_global_state *state;

	drm_modeset_lock_init(&dpu_kms->global_state_lock);

	state = kzalloc(sizeof(*state), GFP_KERNEL);
	if (!state)
		return -ENOMEM;

	drm_atomic_private_obj_init(dpu_kms->dev, &dpu_kms->global_state,
				    &state->base,
				    &dpu_kms_global_state_funcs);
	return 0;
}

static int dpu_kms_parse_data_bus_icc_path(struct dpu_kms *dpu_kms)
{
	struct icc_path *path0;
	struct icc_path *path1;
	struct drm_device *dev = dpu_kms->dev;

	path0 = of_icc_get(dev->dev, "mdp0-mem");
	path1 = of_icc_get(dev->dev, "mdp1-mem");

	if (IS_ERR_OR_NULL(path0))
		return PTR_ERR_OR_ZERO(path0);

	dpu_kms->path[0] = path0;
	dpu_kms->num_paths = 1;

	if (!IS_ERR_OR_NULL(path1)) {
		dpu_kms->path[1] = path1;
		dpu_kms->num_paths++;
	}
	return 0;
}
>>>>>>> upstream/android-13

static int dpu_kms_enable_vblank(struct msm_kms *kms, struct drm_crtc *crtc)
{
	return dpu_crtc_vblank(crtc, true);
}

static void dpu_kms_disable_vblank(struct msm_kms *kms, struct drm_crtc *crtc)
{
	dpu_crtc_vblank(crtc, false);
}

<<<<<<< HEAD
static void dpu_kms_prepare_commit(struct msm_kms *kms,
		struct drm_atomic_state *state)
{
	struct dpu_kms *dpu_kms;
	struct msm_drm_private *priv;
	struct drm_device *dev;
	struct drm_encoder *encoder;

	if (!kms)
		return;
	dpu_kms = to_dpu_kms(kms);
	dev = dpu_kms->dev;

	if (!dev || !dev->dev_private)
		return;
	priv = dev->dev_private;
	pm_runtime_get_sync(&dpu_kms->pdev->dev);

	list_for_each_entry(encoder, &dev->mode_config.encoder_list, head)
		if (encoder->crtc != NULL)
			dpu_encoder_prepare_commit(encoder);
=======
static void dpu_kms_enable_commit(struct msm_kms *kms)
{
	struct dpu_kms *dpu_kms = to_dpu_kms(kms);
	pm_runtime_get_sync(&dpu_kms->pdev->dev);
}

static void dpu_kms_disable_commit(struct msm_kms *kms)
{
	struct dpu_kms *dpu_kms = to_dpu_kms(kms);
	pm_runtime_put_sync(&dpu_kms->pdev->dev);
}

static ktime_t dpu_kms_vsync_time(struct msm_kms *kms, struct drm_crtc *crtc)
{
	struct drm_encoder *encoder;

	drm_for_each_encoder_mask(encoder, crtc->dev, crtc->state->encoder_mask) {
		ktime_t vsync_time;

		if (dpu_encoder_vsync_time(encoder, &vsync_time) == 0)
			return vsync_time;
	}

	return ktime_get();
}

static void dpu_kms_prepare_commit(struct msm_kms *kms,
		struct drm_atomic_state *state)
{
	struct drm_crtc *crtc;
	struct drm_crtc_state *crtc_state;
	struct drm_encoder *encoder;
	int i;

	if (!kms)
		return;

	/* Call prepare_commit for all affected encoders */
	for_each_new_crtc_in_state(state, crtc, crtc_state, i) {
		drm_for_each_encoder_mask(encoder, crtc->dev,
					  crtc_state->encoder_mask) {
			dpu_encoder_prepare_commit(encoder);
		}
	}
}

static void dpu_kms_flush_commit(struct msm_kms *kms, unsigned crtc_mask)
{
	struct dpu_kms *dpu_kms = to_dpu_kms(kms);
	struct drm_crtc *crtc;

	for_each_crtc_mask(dpu_kms->dev, crtc, crtc_mask) {
		if (!crtc->state->active)
			continue;

		trace_dpu_kms_commit(DRMID(crtc));
		dpu_crtc_commit_kickoff(crtc);
	}
>>>>>>> upstream/android-13
}

/*
 * Override the encoder enable since we need to setup the inline rotator and do
 * some crtc magic before enabling any bridge that might be present.
 */
void dpu_kms_encoder_enable(struct drm_encoder *encoder)
{
	const struct drm_encoder_helper_funcs *funcs = encoder->helper_private;
<<<<<<< HEAD
	struct drm_crtc *crtc = encoder->crtc;
=======
	struct drm_device *dev = encoder->dev;
	struct drm_crtc *crtc;
>>>>>>> upstream/android-13

	/* Forward this enable call to the commit hook */
	if (funcs && funcs->commit)
		funcs->commit(encoder);

<<<<<<< HEAD
	if (crtc && crtc->state->active) {
		trace_dpu_kms_enc_enable(DRMID(crtc));
		dpu_crtc_commit_kickoff(crtc);
	}
}

static void dpu_kms_commit(struct msm_kms *kms, struct drm_atomic_state *state)
{
	struct drm_crtc *crtc;
	struct drm_crtc_state *crtc_state;
	int i;

	for_each_new_crtc_in_state(state, crtc, crtc_state, i) {
		/* If modeset is required, kickoff is run in encoder_enable */
		if (drm_atomic_crtc_needs_modeset(crtc_state))
			continue;

		if (crtc->state->active) {
			trace_dpu_kms_commit(DRMID(crtc));
			dpu_crtc_commit_kickoff(crtc);
		}
	}
}

static void dpu_kms_complete_commit(struct msm_kms *kms,
		struct drm_atomic_state *old_state)
{
	struct dpu_kms *dpu_kms;
	struct msm_drm_private *priv;
	struct drm_crtc *crtc;
	struct drm_crtc_state *old_crtc_state;
	int i;

	if (!kms || !old_state)
		return;
	dpu_kms = to_dpu_kms(kms);

	if (!dpu_kms->dev || !dpu_kms->dev->dev_private)
		return;
	priv = dpu_kms->dev->dev_private;

	DPU_ATRACE_BEGIN("kms_complete_commit");

	for_each_old_crtc_in_state(old_state, crtc, old_crtc_state, i)
		dpu_crtc_complete_commit(crtc, old_crtc_state);

	pm_runtime_put_sync(&dpu_kms->pdev->dev);
=======
	drm_for_each_crtc(crtc, dev) {
		if (!(crtc->state->encoder_mask & drm_encoder_mask(encoder)))
			continue;

		trace_dpu_kms_enc_enable(DRMID(crtc));
	}
}

static void dpu_kms_complete_commit(struct msm_kms *kms, unsigned crtc_mask)
{
	struct dpu_kms *dpu_kms = to_dpu_kms(kms);
	struct drm_crtc *crtc;

	DPU_ATRACE_BEGIN("kms_complete_commit");

	for_each_crtc_mask(dpu_kms->dev, crtc, crtc_mask)
		dpu_crtc_complete_commit(crtc);
>>>>>>> upstream/android-13

	DPU_ATRACE_END("kms_complete_commit");
}

static void dpu_kms_wait_for_commit_done(struct msm_kms *kms,
		struct drm_crtc *crtc)
{
	struct drm_encoder *encoder;
	struct drm_device *dev;
	int ret;

	if (!kms || !crtc || !crtc->state) {
		DPU_ERROR("invalid params\n");
		return;
	}

	dev = crtc->dev;

	if (!crtc->state->enable) {
		DPU_DEBUG("[crtc:%d] not enable\n", crtc->base.id);
		return;
	}

	if (!crtc->state->active) {
		DPU_DEBUG("[crtc:%d] not active\n", crtc->base.id);
		return;
	}

	list_for_each_entry(encoder, &dev->mode_config.encoder_list, head) {
		if (encoder->crtc != crtc)
			continue;
		/*
		 * Wait for post-flush if necessary to delay before
		 * plane_cleanup. For example, wait for vsync in case of video
		 * mode panels. This may be a no-op for command mode panels.
		 */
		trace_dpu_kms_wait_for_commit_done(DRMID(crtc));
		ret = dpu_encoder_wait_for_event(encoder, MSM_ENC_COMMIT_DONE);
		if (ret && ret != -EWOULDBLOCK) {
			DPU_ERROR("wait for commit done returned %d\n", ret);
			break;
		}
	}
}

<<<<<<< HEAD
=======
static void dpu_kms_wait_flush(struct msm_kms *kms, unsigned crtc_mask)
{
	struct dpu_kms *dpu_kms = to_dpu_kms(kms);
	struct drm_crtc *crtc;

	for_each_crtc_mask(dpu_kms->dev, crtc, crtc_mask)
		dpu_kms_wait_for_commit_done(kms, crtc);
}

>>>>>>> upstream/android-13
static int _dpu_kms_initialize_dsi(struct drm_device *dev,
				    struct msm_drm_private *priv,
				    struct dpu_kms *dpu_kms)
{
	struct drm_encoder *encoder = NULL;
<<<<<<< HEAD
=======
	struct msm_display_info info;
>>>>>>> upstream/android-13
	int i, rc = 0;

	if (!(priv->dsi[0] || priv->dsi[1]))
		return rc;

<<<<<<< HEAD
	/*TODO: Support two independent DSI connectors */
	encoder = dpu_encoder_init(dev, DRM_MODE_ENCODER_DSI);
	if (IS_ERR(encoder)) {
		DPU_ERROR("encoder init failed for dsi display\n");
		return PTR_ERR(encoder);
	}

	priv->encoders[priv->num_encoders++] = encoder;

	for (i = 0; i < ARRAY_SIZE(priv->dsi); i++) {
		if (!priv->dsi[i])
			continue;

=======
	/*
	 * We support following confiurations:
	 * - Single DSI host (dsi0 or dsi1)
	 * - Two independent DSI hosts
	 * - Bonded DSI0 and DSI1 hosts
	 *
	 * TODO: Support swapping DSI0 and DSI1 in the bonded setup.
	 */
	for (i = 0; i < ARRAY_SIZE(priv->dsi); i++) {
		int other = (i + 1) % 2;

		if (!priv->dsi[i])
			continue;

		if (msm_dsi_is_bonded_dsi(priv->dsi[i]) &&
		    !msm_dsi_is_master_dsi(priv->dsi[i]))
			continue;

		encoder = dpu_encoder_init(dev, DRM_MODE_ENCODER_DSI);
		if (IS_ERR(encoder)) {
			DPU_ERROR("encoder init failed for dsi display\n");
			return PTR_ERR(encoder);
		}

		priv->encoders[priv->num_encoders++] = encoder;

		memset(&info, 0, sizeof(info));
		info.intf_type = encoder->encoder_type;

>>>>>>> upstream/android-13
		rc = msm_dsi_modeset_init(priv->dsi[i], dev, encoder);
		if (rc) {
			DPU_ERROR("modeset_init failed for dsi[%d], rc = %d\n",
				i, rc);
			break;
		}
<<<<<<< HEAD
=======

		info.h_tile_instance[info.num_of_h_tiles++] = i;
		info.capabilities = msm_dsi_is_cmd_mode(priv->dsi[i]) ?
			MSM_DISPLAY_CAP_CMD_MODE :
			MSM_DISPLAY_CAP_VID_MODE;

		if (msm_dsi_is_bonded_dsi(priv->dsi[i]) && priv->dsi[other]) {
			rc = msm_dsi_modeset_init(priv->dsi[other], dev, encoder);
			if (rc) {
				DPU_ERROR("modeset_init failed for dsi[%d], rc = %d\n",
					other, rc);
				break;
			}

			info.h_tile_instance[info.num_of_h_tiles++] = other;
		}

		rc = dpu_encoder_setup(dev, encoder, &info);
		if (rc)
			DPU_ERROR("failed to setup DPU encoder %d: rc:%d\n",
				  encoder->base.id, rc);
>>>>>>> upstream/android-13
	}

	return rc;
}

<<<<<<< HEAD
=======
static int _dpu_kms_initialize_displayport(struct drm_device *dev,
					    struct msm_drm_private *priv,
					    struct dpu_kms *dpu_kms)
{
	struct drm_encoder *encoder = NULL;
	struct msm_display_info info;
	int rc = 0;

	if (!priv->dp)
		return rc;

	encoder = dpu_encoder_init(dev, DRM_MODE_ENCODER_TMDS);
	if (IS_ERR(encoder)) {
		DPU_ERROR("encoder init failed for dsi display\n");
		return PTR_ERR(encoder);
	}

	memset(&info, 0, sizeof(info));
	rc = msm_dp_modeset_init(priv->dp, dev, encoder);
	if (rc) {
		DPU_ERROR("modeset_init failed for DP, rc = %d\n", rc);
		drm_encoder_cleanup(encoder);
		return rc;
	}

	priv->encoders[priv->num_encoders++] = encoder;

	info.num_of_h_tiles = 1;
	info.capabilities = MSM_DISPLAY_CAP_VID_MODE;
	info.intf_type = encoder->encoder_type;
	rc = dpu_encoder_setup(dev, encoder, &info);
	if (rc)
		DPU_ERROR("failed to setup DPU encoder %d: rc:%d\n",
			  encoder->base.id, rc);
	return rc;
}

>>>>>>> upstream/android-13
/**
 * _dpu_kms_setup_displays - create encoders, bridges and connectors
 *                           for underlying displays
 * @dev:        Pointer to drm device structure
 * @priv:       Pointer to private drm device data
 * @dpu_kms:    Pointer to dpu kms structure
 * Returns:     Zero on success
 */
static int _dpu_kms_setup_displays(struct drm_device *dev,
				    struct msm_drm_private *priv,
				    struct dpu_kms *dpu_kms)
{
<<<<<<< HEAD
	/**
	 * Extend this function to initialize other
	 * types of displays
	 */

	return _dpu_kms_initialize_dsi(dev, priv, dpu_kms);
=======
	int rc = 0;

	rc = _dpu_kms_initialize_dsi(dev, priv, dpu_kms);
	if (rc) {
		DPU_ERROR("initialize_dsi failed, rc = %d\n", rc);
		return rc;
	}

	rc = _dpu_kms_initialize_displayport(dev, priv, dpu_kms);
	if (rc) {
		DPU_ERROR("initialize_DP failed, rc = %d\n", rc);
		return rc;
	}

	return rc;
>>>>>>> upstream/android-13
}

static void _dpu_kms_drm_obj_destroy(struct dpu_kms *dpu_kms)
{
	struct msm_drm_private *priv;
	int i;

<<<<<<< HEAD
	if (!dpu_kms) {
		DPU_ERROR("invalid dpu_kms\n");
		return;
	} else if (!dpu_kms->dev) {
		DPU_ERROR("invalid dev\n");
		return;
	} else if (!dpu_kms->dev->dev_private) {
		DPU_ERROR("invalid dev_private\n");
		return;
	}
=======
>>>>>>> upstream/android-13
	priv = dpu_kms->dev->dev_private;

	for (i = 0; i < priv->num_crtcs; i++)
		priv->crtcs[i]->funcs->destroy(priv->crtcs[i]);
	priv->num_crtcs = 0;

	for (i = 0; i < priv->num_planes; i++)
		priv->planes[i]->funcs->destroy(priv->planes[i]);
	priv->num_planes = 0;

	for (i = 0; i < priv->num_connectors; i++)
		priv->connectors[i]->funcs->destroy(priv->connectors[i]);
	priv->num_connectors = 0;

	for (i = 0; i < priv->num_encoders; i++)
		priv->encoders[i]->funcs->destroy(priv->encoders[i]);
	priv->num_encoders = 0;
}

static int _dpu_kms_drm_obj_init(struct dpu_kms *dpu_kms)
{
	struct drm_device *dev;
	struct drm_plane *primary_planes[MAX_PLANES], *plane;
<<<<<<< HEAD
=======
	struct drm_plane *cursor_planes[MAX_PLANES] = { NULL };
>>>>>>> upstream/android-13
	struct drm_crtc *crtc;

	struct msm_drm_private *priv;
	struct dpu_mdss_cfg *catalog;

<<<<<<< HEAD
	int primary_planes_idx = 0, i, ret;
	int max_crtc_count;

	if (!dpu_kms || !dpu_kms->dev || !dpu_kms->dev->dev) {
		DPU_ERROR("invalid dpu_kms\n");
		return -EINVAL;
	}

=======
	int primary_planes_idx = 0, cursor_planes_idx = 0, i, ret;
	int max_crtc_count;
>>>>>>> upstream/android-13
	dev = dpu_kms->dev;
	priv = dev->dev_private;
	catalog = dpu_kms->catalog;

	/*
	 * Create encoder and query display drivers to create
	 * bridges and connectors
	 */
	ret = _dpu_kms_setup_displays(dev, priv, dpu_kms);
	if (ret)
		goto fail;

	max_crtc_count = min(catalog->mixer_count, priv->num_encoders);

<<<<<<< HEAD
	/* Create the planes */
	for (i = 0; i < catalog->sspp_count; i++) {
		bool primary = true;

		if (catalog->sspp[i].features & BIT(DPU_SSPP_CURSOR)
			|| primary_planes_idx >= max_crtc_count)
			primary = false;

		plane = dpu_plane_init(dev, catalog->sspp[i].id, primary,
				(1UL << max_crtc_count) - 1, 0);
=======
	/* Create the planes, keeping track of one primary/cursor per crtc */
	for (i = 0; i < catalog->sspp_count; i++) {
		enum drm_plane_type type;

		if ((catalog->sspp[i].features & BIT(DPU_SSPP_CURSOR))
			&& cursor_planes_idx < max_crtc_count)
			type = DRM_PLANE_TYPE_CURSOR;
		else if (primary_planes_idx < max_crtc_count)
			type = DRM_PLANE_TYPE_PRIMARY;
		else
			type = DRM_PLANE_TYPE_OVERLAY;

		DPU_DEBUG("Create plane type %d with features %lx (cur %lx)\n",
			  type, catalog->sspp[i].features,
			  catalog->sspp[i].features & BIT(DPU_SSPP_CURSOR));

		plane = dpu_plane_init(dev, catalog->sspp[i].id, type,
				       (1UL << max_crtc_count) - 1, 0);
>>>>>>> upstream/android-13
		if (IS_ERR(plane)) {
			DPU_ERROR("dpu_plane_init failed\n");
			ret = PTR_ERR(plane);
			goto fail;
		}
		priv->planes[priv->num_planes++] = plane;

<<<<<<< HEAD
		if (primary)
=======
		if (type == DRM_PLANE_TYPE_CURSOR)
			cursor_planes[cursor_planes_idx++] = plane;
		else if (type == DRM_PLANE_TYPE_PRIMARY)
>>>>>>> upstream/android-13
			primary_planes[primary_planes_idx++] = plane;
	}

	max_crtc_count = min(max_crtc_count, primary_planes_idx);

	/* Create one CRTC per encoder */
	for (i = 0; i < max_crtc_count; i++) {
<<<<<<< HEAD
		crtc = dpu_crtc_init(dev, primary_planes[i]);
=======
		crtc = dpu_crtc_init(dev, primary_planes[i], cursor_planes[i]);
>>>>>>> upstream/android-13
		if (IS_ERR(crtc)) {
			ret = PTR_ERR(crtc);
			goto fail;
		}
		priv->crtcs[priv->num_crtcs++] = crtc;
	}

	/* All CRTCs are compatible with all encoders */
	for (i = 0; i < priv->num_encoders; i++)
		priv->encoders[i]->possible_crtcs = (1 << priv->num_crtcs) - 1;

	return 0;
fail:
	_dpu_kms_drm_obj_destroy(dpu_kms);
	return ret;
}

<<<<<<< HEAD
#ifdef CONFIG_DEBUG_FS
static int dpu_kms_debugfs_init(struct msm_kms *kms, struct drm_minor *minor)
{
	struct dpu_kms *dpu_kms = to_dpu_kms(kms);
	struct drm_device *dev;
	int rc;

	if (!dpu_kms || !dpu_kms->dev || !dpu_kms->dev->dev) {
		DPU_ERROR("invalid dpu_kms\n");
		return -EINVAL;
	}

	dev = dpu_kms->dev;

	rc = _dpu_debugfs_init(dpu_kms);
	if (rc)
		DPU_ERROR("dpu_debugfs init failed: %d\n", rc);

	return rc;
}
#endif

=======
>>>>>>> upstream/android-13
static long dpu_kms_round_pixclk(struct msm_kms *kms, unsigned long rate,
		struct drm_encoder *encoder)
{
	return rate;
}

static void _dpu_kms_hw_destroy(struct dpu_kms *dpu_kms)
{
<<<<<<< HEAD
	struct drm_device *dev;
	int i;

	dev = dpu_kms->dev;
	if (!dev)
		return;

=======
	int i;

>>>>>>> upstream/android-13
	if (dpu_kms->hw_intr)
		dpu_hw_intr_destroy(dpu_kms->hw_intr);
	dpu_kms->hw_intr = NULL;

<<<<<<< HEAD
	if (dpu_kms->power_event)
		dpu_power_handle_unregister_event(
				&dpu_kms->phandle, dpu_kms->power_event);

	/* safe to call these more than once during shutdown */
	_dpu_debugfs_destroy(dpu_kms);
=======
	/* safe to call these more than once during shutdown */
>>>>>>> upstream/android-13
	_dpu_kms_mmu_destroy(dpu_kms);

	if (dpu_kms->catalog) {
		for (i = 0; i < dpu_kms->catalog->vbif_count; i++) {
			u32 vbif_idx = dpu_kms->catalog->vbif[i].id;

			if ((vbif_idx < VBIF_MAX) && dpu_kms->hw_vbif[vbif_idx])
				dpu_hw_vbif_destroy(dpu_kms->hw_vbif[vbif_idx]);
		}
	}

	if (dpu_kms->rm_init)
		dpu_rm_destroy(&dpu_kms->rm);
	dpu_kms->rm_init = false;

	if (dpu_kms->catalog)
		dpu_hw_catalog_deinit(dpu_kms->catalog);
	dpu_kms->catalog = NULL;

<<<<<<< HEAD
	if (dpu_kms->core_client)
		dpu_power_client_destroy(&dpu_kms->phandle,
			dpu_kms->core_client);
	dpu_kms->core_client = NULL;

=======
>>>>>>> upstream/android-13
	if (dpu_kms->vbif[VBIF_NRT])
		devm_iounmap(&dpu_kms->pdev->dev, dpu_kms->vbif[VBIF_NRT]);
	dpu_kms->vbif[VBIF_NRT] = NULL;

	if (dpu_kms->vbif[VBIF_RT])
		devm_iounmap(&dpu_kms->pdev->dev, dpu_kms->vbif[VBIF_RT]);
	dpu_kms->vbif[VBIF_RT] = NULL;

<<<<<<< HEAD
=======
	if (dpu_kms->hw_mdp)
		dpu_hw_mdp_destroy(dpu_kms->hw_mdp);
	dpu_kms->hw_mdp = NULL;

>>>>>>> upstream/android-13
	if (dpu_kms->mmio)
		devm_iounmap(&dpu_kms->pdev->dev, dpu_kms->mmio);
	dpu_kms->mmio = NULL;
}

static void dpu_kms_destroy(struct msm_kms *kms)
{
	struct dpu_kms *dpu_kms;

	if (!kms) {
		DPU_ERROR("invalid kms\n");
		return;
	}

	dpu_kms = to_dpu_kms(kms);

<<<<<<< HEAD
	dpu_dbg_destroy();
	_dpu_kms_hw_destroy(dpu_kms);
}

static int dpu_kms_pm_suspend(struct device *dev)
{
	struct drm_device *ddev;
	struct drm_modeset_acquire_ctx ctx;
	struct drm_atomic_state *state;
	struct dpu_kms *dpu_kms;
	int ret = 0, num_crtcs = 0;

	if (!dev)
		return -EINVAL;

	ddev = dev_get_drvdata(dev);
	if (!ddev || !ddev_to_msm_kms(ddev))
		return -EINVAL;

	dpu_kms = to_dpu_kms(ddev_to_msm_kms(ddev));

	/* disable hot-plug polling */
	drm_kms_helper_poll_disable(ddev);

	/* acquire modeset lock(s) */
	drm_modeset_acquire_init(&ctx, 0);

retry:
	DPU_ATRACE_BEGIN("kms_pm_suspend");

	ret = drm_modeset_lock_all_ctx(ddev, &ctx);
	if (ret)
		goto unlock;

	/* save current state for resume */
	if (dpu_kms->suspend_state)
		drm_atomic_state_put(dpu_kms->suspend_state);
	dpu_kms->suspend_state = drm_atomic_helper_duplicate_state(ddev, &ctx);
	if (IS_ERR_OR_NULL(dpu_kms->suspend_state)) {
		DRM_ERROR("failed to back up suspend state\n");
		dpu_kms->suspend_state = NULL;
		goto unlock;
	}

	/* create atomic state to disable all CRTCs */
	state = drm_atomic_state_alloc(ddev);
	if (IS_ERR_OR_NULL(state)) {
		DRM_ERROR("failed to allocate crtc disable state\n");
		goto unlock;
	}

	state->acquire_ctx = &ctx;

	/* check for nothing to do */
	if (num_crtcs == 0) {
		DRM_DEBUG("all crtcs are already in the off state\n");
		drm_atomic_state_put(state);
		goto suspended;
	}

	/* commit the "disable all" state */
	ret = drm_atomic_commit(state);
	if (ret < 0) {
		DRM_ERROR("failed to disable crtcs, %d\n", ret);
		drm_atomic_state_put(state);
		goto unlock;
	}

suspended:
	dpu_kms->suspend_block = true;

unlock:
	if (ret == -EDEADLK) {
		drm_modeset_backoff(&ctx);
		goto retry;
	}
	drm_modeset_drop_locks(&ctx);
	drm_modeset_acquire_fini(&ctx);

	DPU_ATRACE_END("kms_pm_suspend");
	return 0;
}

static int dpu_kms_pm_resume(struct device *dev)
{
	struct drm_device *ddev;
	struct dpu_kms *dpu_kms;
	int ret;

	if (!dev)
		return -EINVAL;

	ddev = dev_get_drvdata(dev);
	if (!ddev || !ddev_to_msm_kms(ddev))
		return -EINVAL;

	dpu_kms = to_dpu_kms(ddev_to_msm_kms(ddev));

	DPU_ATRACE_BEGIN("kms_pm_resume");

	drm_mode_config_reset(ddev);

	drm_modeset_lock_all(ddev);

	dpu_kms->suspend_block = false;

	if (dpu_kms->suspend_state) {
		dpu_kms->suspend_state->acquire_ctx =
			ddev->mode_config.acquire_ctx;
		ret = drm_atomic_commit(dpu_kms->suspend_state);
		if (ret < 0) {
			DRM_ERROR("failed to restore state, %d\n", ret);
			drm_atomic_state_put(dpu_kms->suspend_state);
		}
		dpu_kms->suspend_state = NULL;
	}
	drm_modeset_unlock_all(ddev);

	/* enable hot-plug polling */
	drm_kms_helper_poll_enable(ddev);

	DPU_ATRACE_END("kms_pm_resume");
	return 0;
}

static void _dpu_kms_set_encoder_mode(struct msm_kms *kms,
				 struct drm_encoder *encoder,
				 bool cmd_mode)
{
	struct msm_display_info info;
	struct msm_drm_private *priv = encoder->dev->dev_private;
	int i, rc = 0;

	memset(&info, 0, sizeof(info));

	info.intf_type = encoder->encoder_type;
	info.capabilities = cmd_mode ? MSM_DISPLAY_CAP_CMD_MODE :
			MSM_DISPLAY_CAP_VID_MODE;

	/* TODO: No support for DSI swap */
	for (i = 0; i < ARRAY_SIZE(priv->dsi); i++) {
		if (priv->dsi[i]) {
			info.h_tile_instance[info.num_of_h_tiles] = i;
			info.num_of_h_tiles++;
		}
	}

	rc = dpu_encoder_setup(encoder->dev, encoder, &info);
	if (rc)
		DPU_ERROR("failed to setup DPU encoder %d: rc:%d\n",
			encoder->base.id, rc);
=======
	_dpu_kms_hw_destroy(dpu_kms);

	msm_kms_destroy(&dpu_kms->base);
}

static irqreturn_t dpu_irq(struct msm_kms *kms)
{
	struct dpu_kms *dpu_kms = to_dpu_kms(kms);

	return dpu_core_irq(dpu_kms);
}

static void dpu_irq_preinstall(struct msm_kms *kms)
{
	struct dpu_kms *dpu_kms = to_dpu_kms(kms);

	dpu_core_irq_preinstall(dpu_kms);
}

static int dpu_irq_postinstall(struct msm_kms *kms)
{
	struct msm_drm_private *priv;
	struct dpu_kms *dpu_kms = to_dpu_kms(kms);

	if (!dpu_kms || !dpu_kms->dev)
		return -EINVAL;

	priv = dpu_kms->dev->dev_private;
	if (!priv)
		return -EINVAL;

	msm_dp_irq_postinstall(priv->dp);

	return 0;
}

static void dpu_irq_uninstall(struct msm_kms *kms)
{
	struct dpu_kms *dpu_kms = to_dpu_kms(kms);

	dpu_core_irq_uninstall(dpu_kms);
}

static void dpu_kms_mdp_snapshot(struct msm_disp_state *disp_state, struct msm_kms *kms)
{
	int i;
	struct dpu_kms *dpu_kms;
	struct dpu_mdss_cfg *cat;
	struct dpu_hw_mdp *top;

	dpu_kms = to_dpu_kms(kms);

	cat = dpu_kms->catalog;
	top = dpu_kms->hw_mdp;

	pm_runtime_get_sync(&dpu_kms->pdev->dev);

	/* dump CTL sub-blocks HW regs info */
	for (i = 0; i < cat->ctl_count; i++)
		msm_disp_snapshot_add_block(disp_state, cat->ctl[i].len,
				dpu_kms->mmio + cat->ctl[i].base, "ctl_%d", i);

	/* dump DSPP sub-blocks HW regs info */
	for (i = 0; i < cat->dspp_count; i++)
		msm_disp_snapshot_add_block(disp_state, cat->dspp[i].len,
				dpu_kms->mmio + cat->dspp[i].base, "dspp_%d", i);

	/* dump INTF sub-blocks HW regs info */
	for (i = 0; i < cat->intf_count; i++)
		msm_disp_snapshot_add_block(disp_state, cat->intf[i].len,
				dpu_kms->mmio + cat->intf[i].base, "intf_%d", i);

	/* dump PP sub-blocks HW regs info */
	for (i = 0; i < cat->pingpong_count; i++)
		msm_disp_snapshot_add_block(disp_state, cat->pingpong[i].len,
				dpu_kms->mmio + cat->pingpong[i].base, "pingpong_%d", i);

	/* dump SSPP sub-blocks HW regs info */
	for (i = 0; i < cat->sspp_count; i++)
		msm_disp_snapshot_add_block(disp_state, cat->sspp[i].len,
				dpu_kms->mmio + cat->sspp[i].base, "sspp_%d", i);

	msm_disp_snapshot_add_block(disp_state, top->hw.length,
			dpu_kms->mmio + top->hw.blk_off, "top");

	pm_runtime_put_sync(&dpu_kms->pdev->dev);
>>>>>>> upstream/android-13
}

static const struct msm_kms_funcs kms_funcs = {
	.hw_init         = dpu_kms_hw_init,
	.irq_preinstall  = dpu_irq_preinstall,
	.irq_postinstall = dpu_irq_postinstall,
	.irq_uninstall   = dpu_irq_uninstall,
	.irq             = dpu_irq,
<<<<<<< HEAD
	.prepare_commit  = dpu_kms_prepare_commit,
	.commit          = dpu_kms_commit,
	.complete_commit = dpu_kms_complete_commit,
	.wait_for_crtc_commit_done = dpu_kms_wait_for_commit_done,
=======
	.enable_commit   = dpu_kms_enable_commit,
	.disable_commit  = dpu_kms_disable_commit,
	.vsync_time      = dpu_kms_vsync_time,
	.prepare_commit  = dpu_kms_prepare_commit,
	.flush_commit    = dpu_kms_flush_commit,
	.wait_flush      = dpu_kms_wait_flush,
	.complete_commit = dpu_kms_complete_commit,
>>>>>>> upstream/android-13
	.enable_vblank   = dpu_kms_enable_vblank,
	.disable_vblank  = dpu_kms_disable_vblank,
	.check_modified_format = dpu_format_check_modified_format,
	.get_format      = dpu_get_msm_format,
	.round_pixclk    = dpu_kms_round_pixclk,
<<<<<<< HEAD
	.pm_suspend      = dpu_kms_pm_suspend,
	.pm_resume       = dpu_kms_pm_resume,
	.destroy         = dpu_kms_destroy,
	.set_encoder_mode = _dpu_kms_set_encoder_mode,
=======
	.destroy         = dpu_kms_destroy,
	.snapshot        = dpu_kms_mdp_snapshot,
>>>>>>> upstream/android-13
#ifdef CONFIG_DEBUG_FS
	.debugfs_init    = dpu_kms_debugfs_init,
#endif
};

<<<<<<< HEAD
/* the caller api needs to turn on clock before calling it */
static inline void _dpu_kms_core_hw_rev_init(struct dpu_kms *dpu_kms)
{
	dpu_kms->core_rev = readl_relaxed(dpu_kms->mmio + 0x0);
}

static int _dpu_kms_mmu_destroy(struct dpu_kms *dpu_kms)
{
	struct msm_mmu *mmu;

	mmu = dpu_kms->base.aspace->mmu;

	mmu->funcs->detach(mmu, (const char **)iommu_ports,
			ARRAY_SIZE(iommu_ports));
	msm_gem_address_space_put(dpu_kms->base.aspace);

	return 0;
=======
static void _dpu_kms_mmu_destroy(struct dpu_kms *dpu_kms)
{
	struct msm_mmu *mmu;

	if (!dpu_kms->base.aspace)
		return;

	mmu = dpu_kms->base.aspace->mmu;

	mmu->funcs->detach(mmu);
	msm_gem_address_space_put(dpu_kms->base.aspace);

	dpu_kms->base.aspace = NULL;
>>>>>>> upstream/android-13
}

static int _dpu_kms_mmu_init(struct dpu_kms *dpu_kms)
{
	struct iommu_domain *domain;
	struct msm_gem_address_space *aspace;
<<<<<<< HEAD
	int ret;
=======
	struct msm_mmu *mmu;
>>>>>>> upstream/android-13

	domain = iommu_domain_alloc(&platform_bus_type);
	if (!domain)
		return 0;

<<<<<<< HEAD
	aspace = msm_gem_address_space_create(dpu_kms->dev->dev,
			domain, "dpu1");
	if (IS_ERR(aspace)) {
		ret = PTR_ERR(aspace);
		goto fail;
	}

	dpu_kms->base.aspace = aspace;

	ret = aspace->mmu->funcs->attach(aspace->mmu, iommu_ports,
			ARRAY_SIZE(iommu_ports));
	if (ret) {
		DPU_ERROR("failed to attach iommu %d\n", ret);
		msm_gem_address_space_put(aspace);
		goto fail;
	}

	return 0;
fail:
	_dpu_kms_mmu_destroy(dpu_kms);

	return ret;
=======
	mmu = msm_iommu_new(dpu_kms->dev->dev, domain);
	if (IS_ERR(mmu)) {
		iommu_domain_free(domain);
		return PTR_ERR(mmu);
	}
	aspace = msm_gem_address_space_create(mmu, "dpu1",
		0x1000, 0x100000000 - 0x1000);

	if (IS_ERR(aspace)) {
		mmu->funcs->destroy(mmu);
		return PTR_ERR(aspace);
	}

	dpu_kms->base.aspace = aspace;
	return 0;
>>>>>>> upstream/android-13
}

static struct dss_clk *_dpu_kms_get_clk(struct dpu_kms *dpu_kms,
		char *clock_name)
{
	struct dss_module_power *mp = &dpu_kms->mp;
	int i;

	for (i = 0; i < mp->num_clk; i++) {
		if (!strcmp(mp->clk_config[i].clk_name, clock_name))
			return &mp->clk_config[i];
	}

	return NULL;
}

u64 dpu_kms_get_clk_rate(struct dpu_kms *dpu_kms, char *clock_name)
{
	struct dss_clk *clk;

	clk = _dpu_kms_get_clk(dpu_kms, clock_name);
	if (!clk)
		return -EINVAL;

	return clk_get_rate(clk->clk);
}

<<<<<<< HEAD
static void dpu_kms_handle_power_event(u32 event_type, void *usr)
{
	struct dpu_kms *dpu_kms = usr;

	if (!dpu_kms)
		return;

	if (event_type == DPU_POWER_EVENT_POST_ENABLE)
		dpu_vbif_init_memtypes(dpu_kms);
}

=======
>>>>>>> upstream/android-13
static int dpu_kms_hw_init(struct msm_kms *kms)
{
	struct dpu_kms *dpu_kms;
	struct drm_device *dev;
<<<<<<< HEAD
	struct msm_drm_private *priv;
=======
>>>>>>> upstream/android-13
	int i, rc = -EINVAL;

	if (!kms) {
		DPU_ERROR("invalid kms\n");
<<<<<<< HEAD
		goto end;
=======
		return rc;
>>>>>>> upstream/android-13
	}

	dpu_kms = to_dpu_kms(kms);
	dev = dpu_kms->dev;
<<<<<<< HEAD
	if (!dev) {
		DPU_ERROR("invalid device\n");
		goto end;
	}

	rc = dpu_dbg_init(&dpu_kms->pdev->dev);
	if (rc) {
		DRM_ERROR("failed to init dpu dbg: %d\n", rc);
		goto end;
	}

	priv = dev->dev_private;
	if (!priv) {
		DPU_ERROR("invalid private data\n");
		goto dbg_destroy;
	}
=======

	rc = dpu_kms_global_obj_init(dpu_kms);
	if (rc)
		return rc;

	atomic_set(&dpu_kms->bandwidth_ref, 0);
>>>>>>> upstream/android-13

	dpu_kms->mmio = msm_ioremap(dpu_kms->pdev, "mdp", "mdp");
	if (IS_ERR(dpu_kms->mmio)) {
		rc = PTR_ERR(dpu_kms->mmio);
		DPU_ERROR("mdp register memory map failed: %d\n", rc);
		dpu_kms->mmio = NULL;
		goto error;
	}
	DRM_DEBUG("mapped dpu address space @%pK\n", dpu_kms->mmio);
<<<<<<< HEAD
	dpu_kms->mmio_len = dpu_iomap_size(dpu_kms->pdev, "mdp");
=======
>>>>>>> upstream/android-13

	dpu_kms->vbif[VBIF_RT] = msm_ioremap(dpu_kms->pdev, "vbif", "vbif");
	if (IS_ERR(dpu_kms->vbif[VBIF_RT])) {
		rc = PTR_ERR(dpu_kms->vbif[VBIF_RT]);
		DPU_ERROR("vbif register memory map failed: %d\n", rc);
		dpu_kms->vbif[VBIF_RT] = NULL;
		goto error;
	}
<<<<<<< HEAD
	dpu_kms->vbif_len[VBIF_RT] = dpu_iomap_size(dpu_kms->pdev, "vbif");
	dpu_kms->vbif[VBIF_NRT] = msm_ioremap(dpu_kms->pdev, "vbif_nrt", "vbif_nrt");
	if (IS_ERR(dpu_kms->vbif[VBIF_NRT])) {
		dpu_kms->vbif[VBIF_NRT] = NULL;
		DPU_DEBUG("VBIF NRT is not defined");
	} else {
		dpu_kms->vbif_len[VBIF_NRT] = dpu_iomap_size(dpu_kms->pdev,
							     "vbif_nrt");
	}

	dpu_kms->reg_dma = msm_ioremap(dpu_kms->pdev, "regdma", "regdma");
	if (IS_ERR(dpu_kms->reg_dma)) {
		dpu_kms->reg_dma = NULL;
		DPU_DEBUG("REG_DMA is not defined");
	} else {
		dpu_kms->reg_dma_len = dpu_iomap_size(dpu_kms->pdev, "regdma");
	}

	dpu_kms->core_client = dpu_power_client_create(&dpu_kms->phandle,
					"core");
	if (IS_ERR_OR_NULL(dpu_kms->core_client)) {
		rc = PTR_ERR(dpu_kms->core_client);
		if (!dpu_kms->core_client)
			rc = -EINVAL;
		DPU_ERROR("dpu power client create failed: %d\n", rc);
		dpu_kms->core_client = NULL;
		goto error;
	}

	pm_runtime_get_sync(&dpu_kms->pdev->dev);

	_dpu_kms_core_hw_rev_init(dpu_kms);
=======
	dpu_kms->vbif[VBIF_NRT] = msm_ioremap_quiet(dpu_kms->pdev, "vbif_nrt", "vbif_nrt");
	if (IS_ERR(dpu_kms->vbif[VBIF_NRT])) {
		dpu_kms->vbif[VBIF_NRT] = NULL;
		DPU_DEBUG("VBIF NRT is not defined");
	}

	dpu_kms->reg_dma = msm_ioremap_quiet(dpu_kms->pdev, "regdma", "regdma");
	if (IS_ERR(dpu_kms->reg_dma)) {
		dpu_kms->reg_dma = NULL;
		DPU_DEBUG("REG_DMA is not defined");
	}

	dpu_kms_parse_data_bus_icc_path(dpu_kms);

	pm_runtime_get_sync(&dpu_kms->pdev->dev);

	dpu_kms->core_rev = readl_relaxed(dpu_kms->mmio + 0x0);
>>>>>>> upstream/android-13

	pr_info("dpu hardware revision:0x%x\n", dpu_kms->core_rev);

	dpu_kms->catalog = dpu_hw_catalog_init(dpu_kms->core_rev);
	if (IS_ERR_OR_NULL(dpu_kms->catalog)) {
		rc = PTR_ERR(dpu_kms->catalog);
		if (!dpu_kms->catalog)
			rc = -EINVAL;
		DPU_ERROR("catalog init failed: %d\n", rc);
		dpu_kms->catalog = NULL;
		goto power_error;
	}

<<<<<<< HEAD
	dpu_dbg_init_dbg_buses(dpu_kms->core_rev);

=======
>>>>>>> upstream/android-13
	/*
	 * Now we need to read the HW catalog and initialize resources such as
	 * clocks, regulators, GDSC/MMAGIC, ioremap the register ranges etc
	 */
	rc = _dpu_kms_mmu_init(dpu_kms);
	if (rc) {
		DPU_ERROR("dpu_kms_mmu_init failed: %d\n", rc);
		goto power_error;
	}

<<<<<<< HEAD
	rc = dpu_rm_init(&dpu_kms->rm, dpu_kms->catalog, dpu_kms->mmio,
			dpu_kms->dev);
=======
	rc = dpu_rm_init(&dpu_kms->rm, dpu_kms->catalog, dpu_kms->mmio);
>>>>>>> upstream/android-13
	if (rc) {
		DPU_ERROR("rm init failed: %d\n", rc);
		goto power_error;
	}

	dpu_kms->rm_init = true;

<<<<<<< HEAD
	dpu_kms->hw_mdp = dpu_rm_get_mdp(&dpu_kms->rm);
	if (IS_ERR_OR_NULL(dpu_kms->hw_mdp)) {
		rc = PTR_ERR(dpu_kms->hw_mdp);
		if (!dpu_kms->hw_mdp)
			rc = -EINVAL;
=======
	dpu_kms->hw_mdp = dpu_hw_mdptop_init(MDP_TOP, dpu_kms->mmio,
					     dpu_kms->catalog);
	if (IS_ERR(dpu_kms->hw_mdp)) {
		rc = PTR_ERR(dpu_kms->hw_mdp);
>>>>>>> upstream/android-13
		DPU_ERROR("failed to get hw_mdp: %d\n", rc);
		dpu_kms->hw_mdp = NULL;
		goto power_error;
	}

	for (i = 0; i < dpu_kms->catalog->vbif_count; i++) {
		u32 vbif_idx = dpu_kms->catalog->vbif[i].id;

		dpu_kms->hw_vbif[i] = dpu_hw_vbif_init(vbif_idx,
				dpu_kms->vbif[vbif_idx], dpu_kms->catalog);
		if (IS_ERR_OR_NULL(dpu_kms->hw_vbif[vbif_idx])) {
			rc = PTR_ERR(dpu_kms->hw_vbif[vbif_idx]);
			if (!dpu_kms->hw_vbif[vbif_idx])
				rc = -EINVAL;
			DPU_ERROR("failed to init vbif %d: %d\n", vbif_idx, rc);
			dpu_kms->hw_vbif[vbif_idx] = NULL;
			goto power_error;
		}
	}

	rc = dpu_core_perf_init(&dpu_kms->perf, dev, dpu_kms->catalog,
<<<<<<< HEAD
			&dpu_kms->phandle,
=======
>>>>>>> upstream/android-13
			_dpu_kms_get_clk(dpu_kms, "core"));
	if (rc) {
		DPU_ERROR("failed to init perf %d\n", rc);
		goto perf_err;
	}

	dpu_kms->hw_intr = dpu_hw_intr_init(dpu_kms->mmio, dpu_kms->catalog);
	if (IS_ERR_OR_NULL(dpu_kms->hw_intr)) {
		rc = PTR_ERR(dpu_kms->hw_intr);
		DPU_ERROR("hw_intr init failed: %d\n", rc);
		dpu_kms->hw_intr = NULL;
		goto hw_intr_init_err;
	}

<<<<<<< HEAD
	/*
	 * _dpu_kms_drm_obj_init should create the DRM related objects
	 * i.e. CRTCs, planes, encoders, connectors and so forth
	 */
	rc = _dpu_kms_drm_obj_init(dpu_kms);
	if (rc) {
		DPU_ERROR("modeset init failed: %d\n", rc);
		goto drm_obj_init_err;
	}

=======
>>>>>>> upstream/android-13
	dev->mode_config.min_width = 0;
	dev->mode_config.min_height = 0;

	/*
	 * max crtc width is equal to the max mixer width * 2 and max height is
	 * is 4K
	 */
	dev->mode_config.max_width =
			dpu_kms->catalog->caps->max_mixer_width * 2;
	dev->mode_config.max_height = 4096;

<<<<<<< HEAD
	/*
	 * Support format modifiers for compression etc.
	 */
	dev->mode_config.allow_fb_modifiers = true;

	/*
	 * Handle (re)initializations during power enable
	 */
	dpu_kms_handle_power_event(DPU_POWER_EVENT_POST_ENABLE, dpu_kms);
	dpu_kms->power_event = dpu_power_handle_register_event(
			&dpu_kms->phandle,
			DPU_POWER_EVENT_POST_ENABLE,
			dpu_kms_handle_power_event, dpu_kms, "kms");
=======
	dev->max_vblank_count = 0xffffffff;
	/* Disable vblank irqs aggressively for power-saving */
	dev->vblank_disable_immediate = true;

	/*
	 * _dpu_kms_drm_obj_init should create the DRM related objects
	 * i.e. CRTCs, planes, encoders, connectors and so forth
	 */
	rc = _dpu_kms_drm_obj_init(dpu_kms);
	if (rc) {
		DPU_ERROR("modeset init failed: %d\n", rc);
		goto drm_obj_init_err;
	}

	dpu_vbif_init_memtypes(dpu_kms);
>>>>>>> upstream/android-13

	pm_runtime_put_sync(&dpu_kms->pdev->dev);

	return 0;

drm_obj_init_err:
	dpu_core_perf_destroy(&dpu_kms->perf);
hw_intr_init_err:
perf_err:
power_error:
	pm_runtime_put_sync(&dpu_kms->pdev->dev);
error:
	_dpu_kms_hw_destroy(dpu_kms);
<<<<<<< HEAD
dbg_destroy:
	dpu_dbg_destroy();
end:
=======

>>>>>>> upstream/android-13
	return rc;
}

struct msm_kms *dpu_kms_init(struct drm_device *dev)
{
	struct msm_drm_private *priv;
	struct dpu_kms *dpu_kms;
	int irq;

<<<<<<< HEAD
	if (!dev || !dev->dev_private) {
=======
	if (!dev) {
>>>>>>> upstream/android-13
		DPU_ERROR("drm device node invalid\n");
		return ERR_PTR(-EINVAL);
	}

	priv = dev->dev_private;
	dpu_kms = to_dpu_kms(priv->kms);

	irq = irq_of_parse_and_map(dpu_kms->pdev->dev.of_node, 0);
	if (irq < 0) {
		DPU_ERROR("failed to get irq: %d\n", irq);
		return ERR_PTR(irq);
	}
	dpu_kms->base.irq = irq;

	return &dpu_kms->base;
}

static int dpu_bind(struct device *dev, struct device *master, void *data)
{
	struct drm_device *ddev = dev_get_drvdata(master);
	struct platform_device *pdev = to_platform_device(dev);
	struct msm_drm_private *priv = ddev->dev_private;
	struct dpu_kms *dpu_kms;
	struct dss_module_power *mp;
	int ret = 0;

	dpu_kms = devm_kzalloc(&pdev->dev, sizeof(*dpu_kms), GFP_KERNEL);
	if (!dpu_kms)
		return -ENOMEM;

<<<<<<< HEAD
=======
	ret = devm_pm_opp_set_clkname(dev, "core");
	if (ret)
		return ret;
	/* OPP table is optional */
	ret = devm_pm_opp_of_add_table(dev);
	if (ret && ret != -ENODEV) {
		dev_err(dev, "invalid OPP table in device tree\n");
		return ret;
	}

>>>>>>> upstream/android-13
	mp = &dpu_kms->mp;
	ret = msm_dss_parse_clock(pdev, mp);
	if (ret) {
		DPU_ERROR("failed to parse clocks, ret=%d\n", ret);
		return ret;
	}

<<<<<<< HEAD
	dpu_power_resource_init(pdev, &dpu_kms->phandle);

	platform_set_drvdata(pdev, dpu_kms);

	msm_kms_init(&dpu_kms->base, &kms_funcs);
=======
	platform_set_drvdata(pdev, dpu_kms);

	ret = msm_kms_init(&dpu_kms->base, &kms_funcs);
	if (ret) {
		DPU_ERROR("failed to init kms, ret=%d\n", ret);
		return ret;
	}
>>>>>>> upstream/android-13
	dpu_kms->dev = ddev;
	dpu_kms->pdev = pdev;

	pm_runtime_enable(&pdev->dev);
	dpu_kms->rpm_enabled = true;

	priv->kms = &dpu_kms->base;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	return ret;
}

static void dpu_unbind(struct device *dev, struct device *master, void *data)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct dpu_kms *dpu_kms = platform_get_drvdata(pdev);
	struct dss_module_power *mp = &dpu_kms->mp;

<<<<<<< HEAD
	dpu_power_resource_deinit(pdev, &dpu_kms->phandle);
=======
>>>>>>> upstream/android-13
	msm_dss_put_clk(mp->clk_config, mp->num_clk);
	devm_kfree(&pdev->dev, mp->clk_config);
	mp->num_clk = 0;

	if (dpu_kms->rpm_enabled)
		pm_runtime_disable(&pdev->dev);
}

static const struct component_ops dpu_ops = {
	.bind   = dpu_bind,
	.unbind = dpu_unbind,
};

static int dpu_dev_probe(struct platform_device *pdev)
{
	return component_add(&pdev->dev, &dpu_ops);
}

static int dpu_dev_remove(struct platform_device *pdev)
{
	component_del(&pdev->dev, &dpu_ops);
	return 0;
}

static int __maybe_unused dpu_runtime_suspend(struct device *dev)
{
<<<<<<< HEAD
	int rc = -1;
	struct platform_device *pdev = to_platform_device(dev);
	struct dpu_kms *dpu_kms = platform_get_drvdata(pdev);
	struct drm_device *ddev;
	struct dss_module_power *mp = &dpu_kms->mp;

	ddev = dpu_kms->dev;
	if (!ddev) {
		DPU_ERROR("invalid drm_device\n");
		goto exit;
	}

	rc = dpu_power_resource_enable(&dpu_kms->phandle,
			dpu_kms->core_client, false);
	if (rc)
		DPU_ERROR("resource disable failed: %d\n", rc);

=======
	int i, rc = -1;
	struct platform_device *pdev = to_platform_device(dev);
	struct dpu_kms *dpu_kms = platform_get_drvdata(pdev);
	struct dss_module_power *mp = &dpu_kms->mp;

	/* Drop the performance state vote */
	dev_pm_opp_set_rate(dev, 0);
>>>>>>> upstream/android-13
	rc = msm_dss_enable_clk(mp->clk_config, mp->num_clk, false);
	if (rc)
		DPU_ERROR("clock disable failed rc:%d\n", rc);

<<<<<<< HEAD
exit:
=======
	for (i = 0; i < dpu_kms->num_paths; i++)
		icc_set_bw(dpu_kms->path[i], 0, 0);

>>>>>>> upstream/android-13
	return rc;
}

static int __maybe_unused dpu_runtime_resume(struct device *dev)
{
	int rc = -1;
	struct platform_device *pdev = to_platform_device(dev);
	struct dpu_kms *dpu_kms = platform_get_drvdata(pdev);
<<<<<<< HEAD
	struct drm_device *ddev;
	struct dss_module_power *mp = &dpu_kms->mp;

	ddev = dpu_kms->dev;
	if (!ddev) {
		DPU_ERROR("invalid drm_device\n");
		goto exit;
	}
=======
	struct drm_encoder *encoder;
	struct drm_device *ddev;
	struct dss_module_power *mp = &dpu_kms->mp;
	int i;

	ddev = dpu_kms->dev;

	WARN_ON(!(dpu_kms->num_paths));
	/* Min vote of BW is required before turning on AXI clk */
	for (i = 0; i < dpu_kms->num_paths; i++)
		icc_set_bw(dpu_kms->path[i], 0, Bps_to_icc(MIN_IB_BW));
>>>>>>> upstream/android-13

	rc = msm_dss_enable_clk(mp->clk_config, mp->num_clk, true);
	if (rc) {
		DPU_ERROR("clock enable failed rc:%d\n", rc);
<<<<<<< HEAD
		goto exit;
	}

	rc = dpu_power_resource_enable(&dpu_kms->phandle,
			dpu_kms->core_client, true);
	if (rc)
		DPU_ERROR("resource enable failed: %d\n", rc);

exit:
=======
		return rc;
	}

	dpu_vbif_init_memtypes(dpu_kms);

	drm_for_each_encoder(encoder, ddev)
		dpu_encoder_virt_runtime_resume(encoder);

>>>>>>> upstream/android-13
	return rc;
}

static const struct dev_pm_ops dpu_pm_ops = {
	SET_RUNTIME_PM_OPS(dpu_runtime_suspend, dpu_runtime_resume, NULL)
<<<<<<< HEAD
=======
	SET_SYSTEM_SLEEP_PM_OPS(pm_runtime_force_suspend,
				pm_runtime_force_resume)
>>>>>>> upstream/android-13
};

static const struct of_device_id dpu_dt_match[] = {
	{ .compatible = "qcom,sdm845-dpu", },
<<<<<<< HEAD
=======
	{ .compatible = "qcom,sc7180-dpu", },
	{ .compatible = "qcom,sc7280-dpu", },
	{ .compatible = "qcom,sm8150-dpu", },
	{ .compatible = "qcom,sm8250-dpu", },
>>>>>>> upstream/android-13
	{}
};
MODULE_DEVICE_TABLE(of, dpu_dt_match);

static struct platform_driver dpu_driver = {
	.probe = dpu_dev_probe,
	.remove = dpu_dev_remove,
	.driver = {
		.name = "msm_dpu",
		.of_match_table = dpu_dt_match,
		.pm = &dpu_pm_ops,
	},
};

void __init msm_dpu_register(void)
{
	platform_driver_register(&dpu_driver);
}

void __exit msm_dpu_unregister(void)
{
	platform_driver_unregister(&dpu_driver);
}
