<<<<<<< HEAD
/*
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 * Author: Rob Clark <rob@ti.com>
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
 */

#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_mode.h>
#include <drm/drm_plane_helper.h>
#include <linux/math64.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2011 Texas Instruments Incorporated - https://www.ti.com/
 * Author: Rob Clark <rob@ti.com>
 */

#include <linux/math64.h>

#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc.h>
#include <drm/drm_mode.h>
#include <drm/drm_plane_helper.h>
#include <drm/drm_vblank.h>
>>>>>>> upstream/android-13

#include "omap_drv.h"

#define to_omap_crtc_state(x) container_of(x, struct omap_crtc_state, base)

struct omap_crtc_state {
	/* Must be first. */
	struct drm_crtc_state base;
	/* Shadow values for legacy userspace support. */
	unsigned int rotation;
	unsigned int zpos;
<<<<<<< HEAD
=======
	bool manually_updated;
>>>>>>> upstream/android-13
};

#define to_omap_crtc(x) container_of(x, struct omap_crtc, base)

struct omap_crtc {
	struct drm_crtc base;

	const char *name;
<<<<<<< HEAD
=======
	struct omap_drm_pipeline *pipe;
>>>>>>> upstream/android-13
	enum omap_channel channel;

	struct videomode vm;

	bool ignore_digit_sync_lost;

	bool enabled;
	bool pending;
	wait_queue_head_t pending_wait;
	struct drm_pending_vblank_event *event;
<<<<<<< HEAD
=======
	struct delayed_work update_work;

	void (*framedone_handler)(void *);
	void *framedone_handler_data;
>>>>>>> upstream/android-13
};

/* -----------------------------------------------------------------------------
 * Helper Functions
 */

struct videomode *omap_crtc_timings(struct drm_crtc *crtc)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	return &omap_crtc->vm;
}

enum omap_channel omap_crtc_channel(struct drm_crtc *crtc)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	return omap_crtc->channel;
}

static bool omap_crtc_is_pending(struct drm_crtc *crtc)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	unsigned long flags;
	bool pending;

	spin_lock_irqsave(&crtc->dev->event_lock, flags);
	pending = omap_crtc->pending;
	spin_unlock_irqrestore(&crtc->dev->event_lock, flags);

	return pending;
}

int omap_crtc_wait_pending(struct drm_crtc *crtc)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);

	/*
	 * Timeout is set to a "sufficiently" high value, which should cover
	 * a single frame refresh even on slower displays.
	 */
	return wait_event_timeout(omap_crtc->pending_wait,
				  !omap_crtc_is_pending(crtc),
				  msecs_to_jiffies(250));
}

/* -----------------------------------------------------------------------------
 * DSS Manager Functions
 */

/*
 * Manager-ops, callbacks from output when they need to configure
 * the upstream part of the video pipe.
<<<<<<< HEAD
 *
 * Most of these we can ignore until we add support for command-mode
 * panels.. for video-mode the crtc-helpers already do an adequate
 * job of sequencing the setup of the video pipe in the proper order
 */

/* ovl-mgr-id -> crtc */
static struct omap_crtc *omap_crtcs[8];
static struct omap_dss_device *omap_crtc_output[8];

/* we can probably ignore these until we support command-mode panels: */
static int omap_crtc_dss_connect(struct omap_drm_private *priv,
		enum omap_channel channel,
		struct omap_dss_device *dst)
{
	const struct dispc_ops *dispc_ops = priv->dispc_ops;
	struct dispc_device *dispc = priv->dispc;

	if (omap_crtc_output[channel])
		return -EINVAL;

	if (!(dispc_ops->mgr_get_supported_outputs(dispc, channel) & dst->id))
		return -EINVAL;

	omap_crtc_output[channel] = dst;
	dst->dispc_channel_connected = true;

	return 0;
}

static void omap_crtc_dss_disconnect(struct omap_drm_private *priv,
		enum omap_channel channel,
		struct omap_dss_device *dst)
{
	omap_crtc_output[channel] = NULL;
	dst->dispc_channel_connected = false;
}

static void omap_crtc_dss_start_update(struct omap_drm_private *priv,
				       enum omap_channel channel)
{
}

/* Called only from the encoder enable/disable and suspend/resume handlers. */
static void omap_crtc_set_enabled(struct drm_crtc *crtc, bool enable)
{
=======
 */

void omap_crtc_dss_start_update(struct omap_drm_private *priv,
				       enum omap_channel channel)
{
	dispc_mgr_enable(priv->dispc, channel, true);
}

/* Called only from the encoder enable/disable and suspend/resume handlers. */
void omap_crtc_set_enabled(struct drm_crtc *crtc, bool enable)
{
	struct omap_crtc_state *omap_state = to_omap_crtc_state(crtc->state);
>>>>>>> upstream/android-13
	struct drm_device *dev = crtc->dev;
	struct omap_drm_private *priv = dev->dev_private;
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	enum omap_channel channel = omap_crtc->channel;
	struct omap_irq_wait *wait;
	u32 framedone_irq, vsync_irq;
	int ret;

	if (WARN_ON(omap_crtc->enabled == enable))
		return;

<<<<<<< HEAD
	if (omap_crtc_output[channel]->output_type == OMAP_DISPLAY_TYPE_HDMI) {
		priv->dispc_ops->mgr_enable(priv->dispc, channel, enable);
=======
	if (omap_state->manually_updated) {
		omap_irq_enable_framedone(crtc, enable);
		omap_crtc->enabled = enable;
		return;
	}

	if (omap_crtc->pipe->output->type == OMAP_DISPLAY_TYPE_HDMI) {
		dispc_mgr_enable(priv->dispc, channel, enable);
>>>>>>> upstream/android-13
		omap_crtc->enabled = enable;
		return;
	}

	if (omap_crtc->channel == OMAP_DSS_CHANNEL_DIGIT) {
		/*
		 * Digit output produces some sync lost interrupts during the
		 * first frame when enabling, so we need to ignore those.
		 */
		omap_crtc->ignore_digit_sync_lost = true;
	}

<<<<<<< HEAD
	framedone_irq = priv->dispc_ops->mgr_get_framedone_irq(priv->dispc,
							       channel);
	vsync_irq = priv->dispc_ops->mgr_get_vsync_irq(priv->dispc, channel);
=======
	framedone_irq = dispc_mgr_get_framedone_irq(priv->dispc,
							       channel);
	vsync_irq = dispc_mgr_get_vsync_irq(priv->dispc, channel);
>>>>>>> upstream/android-13

	if (enable) {
		wait = omap_irq_wait_init(dev, vsync_irq, 1);
	} else {
		/*
		 * When we disable the digit output, we need to wait for
		 * FRAMEDONE to know that DISPC has finished with the output.
		 *
		 * OMAP2/3 does not have FRAMEDONE irq for digit output, and in
		 * that case we need to use vsync interrupt, and wait for both
		 * even and odd frames.
		 */

		if (framedone_irq)
			wait = omap_irq_wait_init(dev, framedone_irq, 1);
		else
			wait = omap_irq_wait_init(dev, vsync_irq, 2);
	}

<<<<<<< HEAD
	priv->dispc_ops->mgr_enable(priv->dispc, channel, enable);
=======
	dispc_mgr_enable(priv->dispc, channel, enable);
>>>>>>> upstream/android-13
	omap_crtc->enabled = enable;

	ret = omap_irq_wait(dev, wait, msecs_to_jiffies(100));
	if (ret) {
		dev_err(dev->dev, "%s: timeout waiting for %s\n",
				omap_crtc->name, enable ? "enable" : "disable");
	}

	if (omap_crtc->channel == OMAP_DSS_CHANNEL_DIGIT) {
		omap_crtc->ignore_digit_sync_lost = false;
		/* make sure the irq handler sees the value above */
		mb();
	}
}


<<<<<<< HEAD
static int omap_crtc_dss_enable(struct omap_drm_private *priv,
				enum omap_channel channel)
{
	struct omap_crtc *omap_crtc = omap_crtcs[channel];

	priv->dispc_ops->mgr_set_timings(priv->dispc, omap_crtc->channel,
=======
int omap_crtc_dss_enable(struct omap_drm_private *priv, enum omap_channel channel)
{
	struct drm_crtc *crtc = priv->channels[channel]->crtc;
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);

	dispc_mgr_set_timings(priv->dispc, omap_crtc->channel,
>>>>>>> upstream/android-13
					 &omap_crtc->vm);
	omap_crtc_set_enabled(&omap_crtc->base, true);

	return 0;
}

<<<<<<< HEAD
static void omap_crtc_dss_disable(struct omap_drm_private *priv,
				  enum omap_channel channel)
{
	struct omap_crtc *omap_crtc = omap_crtcs[channel];
=======
void omap_crtc_dss_disable(struct omap_drm_private *priv, enum omap_channel channel)
{
	struct drm_crtc *crtc = priv->channels[channel]->crtc;
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
>>>>>>> upstream/android-13

	omap_crtc_set_enabled(&omap_crtc->base, false);
}

<<<<<<< HEAD
static void omap_crtc_dss_set_timings(struct omap_drm_private *priv,
		enum omap_channel channel,
		const struct videomode *vm)
{
	struct omap_crtc *omap_crtc = omap_crtcs[channel];
=======
void omap_crtc_dss_set_timings(struct omap_drm_private *priv,
		enum omap_channel channel,
		const struct videomode *vm)
{
	struct drm_crtc *crtc = priv->channels[channel]->crtc;
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);

>>>>>>> upstream/android-13
	DBG("%s", omap_crtc->name);
	omap_crtc->vm = *vm;
}

<<<<<<< HEAD
static void omap_crtc_dss_set_lcd_config(struct omap_drm_private *priv,
		enum omap_channel channel,
		const struct dss_lcd_mgr_config *config)
{
	struct omap_crtc *omap_crtc = omap_crtcs[channel];

	DBG("%s", omap_crtc->name);
	priv->dispc_ops->mgr_set_lcd_config(priv->dispc, omap_crtc->channel,
					    config);
}

static int omap_crtc_dss_register_framedone(
		struct omap_drm_private *priv, enum omap_channel channel,
		void (*handler)(void *), void *data)
{
	return 0;
}

static void omap_crtc_dss_unregister_framedone(
		struct omap_drm_private *priv, enum omap_channel channel,
		void (*handler)(void *), void *data)
{
}

static const struct dss_mgr_ops mgr_ops = {
	.connect = omap_crtc_dss_connect,
	.disconnect = omap_crtc_dss_disconnect,
	.start_update = omap_crtc_dss_start_update,
	.enable = omap_crtc_dss_enable,
	.disable = omap_crtc_dss_disable,
	.set_timings = omap_crtc_dss_set_timings,
	.set_lcd_config = omap_crtc_dss_set_lcd_config,
	.register_framedone_handler = omap_crtc_dss_register_framedone,
	.unregister_framedone_handler = omap_crtc_dss_unregister_framedone,
};
=======
void omap_crtc_dss_set_lcd_config(struct omap_drm_private *priv,
		enum omap_channel channel,
		const struct dss_lcd_mgr_config *config)
{
	struct drm_crtc *crtc = priv->channels[channel]->crtc;
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);

	DBG("%s", omap_crtc->name);
	dispc_mgr_set_lcd_config(priv->dispc, omap_crtc->channel,
					    config);
}

int omap_crtc_dss_register_framedone(
		struct omap_drm_private *priv, enum omap_channel channel,
		void (*handler)(void *), void *data)
{
	struct drm_crtc *crtc = priv->channels[channel]->crtc;
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	struct drm_device *dev = omap_crtc->base.dev;

	if (omap_crtc->framedone_handler)
		return -EBUSY;

	dev_dbg(dev->dev, "register framedone %s", omap_crtc->name);

	omap_crtc->framedone_handler = handler;
	omap_crtc->framedone_handler_data = data;

	return 0;
}

void omap_crtc_dss_unregister_framedone(
		struct omap_drm_private *priv, enum omap_channel channel,
		void (*handler)(void *), void *data)
{
	struct drm_crtc *crtc = priv->channels[channel]->crtc;
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	struct drm_device *dev = omap_crtc->base.dev;

	dev_dbg(dev->dev, "unregister framedone %s", omap_crtc->name);

	WARN_ON(omap_crtc->framedone_handler != handler);
	WARN_ON(omap_crtc->framedone_handler_data != data);

	omap_crtc->framedone_handler = NULL;
	omap_crtc->framedone_handler_data = NULL;
}
>>>>>>> upstream/android-13

/* -----------------------------------------------------------------------------
 * Setup, Flush and Page Flip
 */

void omap_crtc_error_irq(struct drm_crtc *crtc, u32 irqstatus)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);

	if (omap_crtc->ignore_digit_sync_lost) {
		irqstatus &= ~DISPC_IRQ_SYNC_LOST_DIGIT;
		if (!irqstatus)
			return;
	}

	DRM_ERROR_RATELIMITED("%s: errors: %08x\n", omap_crtc->name, irqstatus);
}

void omap_crtc_vblank_irq(struct drm_crtc *crtc)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	struct drm_device *dev = omap_crtc->base.dev;
	struct omap_drm_private *priv = dev->dev_private;
	bool pending;

	spin_lock(&crtc->dev->event_lock);
	/*
	 * If the dispc is busy we're racing the flush operation. Try again on
	 * the next vblank interrupt.
	 */
<<<<<<< HEAD
	if (priv->dispc_ops->mgr_go_busy(priv->dispc, omap_crtc->channel)) {
=======
	if (dispc_mgr_go_busy(priv->dispc, omap_crtc->channel)) {
>>>>>>> upstream/android-13
		spin_unlock(&crtc->dev->event_lock);
		return;
	}

	/* Send the vblank event if one has been requested. */
	if (omap_crtc->event) {
		drm_crtc_send_vblank_event(crtc, omap_crtc->event);
		omap_crtc->event = NULL;
	}

	pending = omap_crtc->pending;
	omap_crtc->pending = false;
	spin_unlock(&crtc->dev->event_lock);

	if (pending)
		drm_crtc_vblank_put(crtc);

	/* Wake up omap_atomic_complete. */
	wake_up(&omap_crtc->pending_wait);

	DBG("%s: apply done", omap_crtc->name);
}

<<<<<<< HEAD
=======
void omap_crtc_framedone_irq(struct drm_crtc *crtc, uint32_t irqstatus)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);

	if (!omap_crtc->framedone_handler)
		return;

	omap_crtc->framedone_handler(omap_crtc->framedone_handler_data);

	spin_lock(&crtc->dev->event_lock);
	/* Send the vblank event if one has been requested. */
	if (omap_crtc->event) {
		drm_crtc_send_vblank_event(crtc, omap_crtc->event);
		omap_crtc->event = NULL;
	}
	omap_crtc->pending = false;
	spin_unlock(&crtc->dev->event_lock);

	/* Wake up omap_atomic_complete. */
	wake_up(&omap_crtc->pending_wait);
}

void omap_crtc_flush(struct drm_crtc *crtc)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	struct omap_crtc_state *omap_state = to_omap_crtc_state(crtc->state);

	if (!omap_state->manually_updated)
		return;

	if (!delayed_work_pending(&omap_crtc->update_work))
		schedule_delayed_work(&omap_crtc->update_work, 0);
}

static void omap_crtc_manual_display_update(struct work_struct *data)
{
	struct omap_crtc *omap_crtc =
			container_of(data, struct omap_crtc, update_work.work);
	struct omap_dss_device *dssdev = omap_crtc->pipe->output;
	struct drm_device *dev = omap_crtc->base.dev;
	int ret;

	if (!dssdev || !dssdev->dsi_ops || !dssdev->dsi_ops->update)
		return;

	ret = dssdev->dsi_ops->update(dssdev);
	if (ret < 0) {
		spin_lock_irq(&dev->event_lock);
		omap_crtc->pending = false;
		spin_unlock_irq(&dev->event_lock);
		wake_up(&omap_crtc->pending_wait);
	}
}

static s16 omap_crtc_s31_32_to_s2_8(s64 coef)
{
	u64 sign_bit = 1ULL << 63;
	u64 cbits = (u64)coef;

	s16 ret = clamp_val(((cbits & ~sign_bit) >> 24), 0, 0x1ff);

	if (cbits & sign_bit)
		ret = -ret;

	return ret;
}

static void omap_crtc_cpr_coefs_from_ctm(const struct drm_color_ctm *ctm,
					 struct omap_dss_cpr_coefs *cpr)
{
	cpr->rr = omap_crtc_s31_32_to_s2_8(ctm->matrix[0]);
	cpr->rg = omap_crtc_s31_32_to_s2_8(ctm->matrix[1]);
	cpr->rb = omap_crtc_s31_32_to_s2_8(ctm->matrix[2]);
	cpr->gr = omap_crtc_s31_32_to_s2_8(ctm->matrix[3]);
	cpr->gg = omap_crtc_s31_32_to_s2_8(ctm->matrix[4]);
	cpr->gb = omap_crtc_s31_32_to_s2_8(ctm->matrix[5]);
	cpr->br = omap_crtc_s31_32_to_s2_8(ctm->matrix[6]);
	cpr->bg = omap_crtc_s31_32_to_s2_8(ctm->matrix[7]);
	cpr->bb = omap_crtc_s31_32_to_s2_8(ctm->matrix[8]);
}

>>>>>>> upstream/android-13
static void omap_crtc_write_crtc_properties(struct drm_crtc *crtc)
{
	struct omap_drm_private *priv = crtc->dev->dev_private;
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	struct omap_overlay_manager_info info;

	memset(&info, 0, sizeof(info));

	info.default_color = 0x000000;
	info.trans_enabled = false;
	info.partial_alpha_enabled = false;
<<<<<<< HEAD
	info.cpr_enable = false;

	priv->dispc_ops->mgr_setup(priv->dispc, omap_crtc->channel, &info);
=======

	if (crtc->state->ctm) {
		struct drm_color_ctm *ctm = crtc->state->ctm->data;

		info.cpr_enable = true;
		omap_crtc_cpr_coefs_from_ctm(ctm, &info.cpr_coefs);
	} else {
		info.cpr_enable = false;
	}

	dispc_mgr_setup(priv->dispc, omap_crtc->channel, &info);
>>>>>>> upstream/android-13
}

/* -----------------------------------------------------------------------------
 * CRTC Functions
 */

static void omap_crtc_destroy(struct drm_crtc *crtc)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);

	DBG("%s", omap_crtc->name);

	drm_crtc_cleanup(crtc);

	kfree(omap_crtc);
}

static void omap_crtc_arm_event(struct drm_crtc *crtc)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);

	WARN_ON(omap_crtc->pending);
	omap_crtc->pending = true;

	if (crtc->state->event) {
		omap_crtc->event = crtc->state->event;
		crtc->state->event = NULL;
	}
}

static void omap_crtc_atomic_enable(struct drm_crtc *crtc,
<<<<<<< HEAD
				    struct drm_crtc_state *old_state)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
=======
				    struct drm_atomic_state *state)
{
	struct omap_drm_private *priv = crtc->dev->dev_private;
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	struct omap_crtc_state *omap_state = to_omap_crtc_state(crtc->state);
>>>>>>> upstream/android-13
	int ret;

	DBG("%s", omap_crtc->name);

<<<<<<< HEAD
	spin_lock_irq(&crtc->dev->event_lock);
	drm_crtc_vblank_on(crtc);
	ret = drm_crtc_vblank_get(crtc);
	WARN_ON(ret != 0);

=======
	dispc_runtime_get(priv->dispc);

	/* manual updated display will not trigger vsync irq */
	if (omap_state->manually_updated)
		return;

	drm_crtc_vblank_on(crtc);

	ret = drm_crtc_vblank_get(crtc);
	WARN_ON(ret != 0);

	spin_lock_irq(&crtc->dev->event_lock);
>>>>>>> upstream/android-13
	omap_crtc_arm_event(crtc);
	spin_unlock_irq(&crtc->dev->event_lock);
}

static void omap_crtc_atomic_disable(struct drm_crtc *crtc,
<<<<<<< HEAD
				     struct drm_crtc_state *old_state)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
=======
				     struct drm_atomic_state *state)
{
	struct omap_drm_private *priv = crtc->dev->dev_private;
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	struct drm_device *dev = crtc->dev;
>>>>>>> upstream/android-13

	DBG("%s", omap_crtc->name);

	spin_lock_irq(&crtc->dev->event_lock);
	if (crtc->state->event) {
		drm_crtc_send_vblank_event(crtc, crtc->state->event);
		crtc->state->event = NULL;
	}
	spin_unlock_irq(&crtc->dev->event_lock);

<<<<<<< HEAD
	drm_crtc_vblank_off(crtc);
=======
	cancel_delayed_work(&omap_crtc->update_work);

	if (!omap_crtc_wait_pending(crtc))
		dev_warn(dev->dev, "manual display update did not finish!");

	drm_crtc_vblank_off(crtc);

	dispc_runtime_put(priv->dispc);
>>>>>>> upstream/android-13
}

static enum drm_mode_status omap_crtc_mode_valid(struct drm_crtc *crtc,
					const struct drm_display_mode *mode)
{
	struct omap_drm_private *priv = crtc->dev->dev_private;
<<<<<<< HEAD
=======
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	struct videomode vm = {0};
	int r;

	drm_display_mode_to_videomode(mode, &vm);

	/*
	 * DSI might not call this, since the supplied mode is not a
	 * valid DISPC mode. DSI will calculate and configure the
	 * proper DISPC mode later.
	 */
	if (omap_crtc->pipe->output->type != OMAP_DISPLAY_TYPE_DSI) {
		r = dispc_mgr_check_timings(priv->dispc,
						       omap_crtc->channel,
						       &vm);
		if (r)
			return r;
	}
>>>>>>> upstream/android-13

	/* Check for bandwidth limit */
	if (priv->max_bandwidth) {
		/*
		 * Estimation for the bandwidth need of a given mode with one
		 * full screen plane:
		 * bandwidth = resolution * 32bpp * (pclk / (vtotal * htotal))
		 *					^^ Refresh rate ^^
		 *
		 * The interlaced mode is taken into account by using the
		 * pixelclock in the calculation.
		 *
		 * The equation is rearranged for 64bit arithmetic.
		 */
		uint64_t bandwidth = mode->clock * 1000;
		unsigned int bpp = 4;

		bandwidth = bandwidth * mode->hdisplay * mode->vdisplay * bpp;
		bandwidth = div_u64(bandwidth, mode->htotal * mode->vtotal);

		/*
		 * Reject modes which would need more bandwidth if used with one
		 * full resolution plane (most common use case).
		 */
		if (priv->max_bandwidth < bandwidth)
			return MODE_BAD;
	}

	return MODE_OK;
}

static void omap_crtc_mode_set_nofb(struct drm_crtc *crtc)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	struct drm_display_mode *mode = &crtc->state->adjusted_mode;
<<<<<<< HEAD
	struct omap_drm_private *priv = crtc->dev->dev_private;
	const u32 flags_mask = DISPLAY_FLAGS_DE_HIGH | DISPLAY_FLAGS_DE_LOW |
		DISPLAY_FLAGS_PIXDATA_POSEDGE | DISPLAY_FLAGS_PIXDATA_NEGEDGE |
		DISPLAY_FLAGS_SYNC_POSEDGE | DISPLAY_FLAGS_SYNC_NEGEDGE;
	unsigned int i;

	DBG("%s: set mode: %d:\"%s\" %d %d %d %d %d %d %d %d %d %d 0x%x 0x%x",
	    omap_crtc->name, mode->base.id, mode->name,
	    mode->vrefresh, mode->clock,
	    mode->hdisplay, mode->hsync_start, mode->hsync_end, mode->htotal,
	    mode->vdisplay, mode->vsync_start, mode->vsync_end, mode->vtotal,
	    mode->type, mode->flags);

	drm_display_mode_to_videomode(mode, &omap_crtc->vm);

	/*
	 * HACK: This fixes the vm flags.
	 * struct drm_display_mode does not contain the VSYNC/HSYNC/DE flags
	 * and they get lost when converting back and forth between
	 * struct drm_display_mode and struct videomode. The hack below
	 * goes and fetches the missing flags from the panel drivers.
	 *
	 * Correct solution would be to use DRM's bus-flags, but that's not
	 * easily possible before the omapdrm's panel/encoder driver model
	 * has been changed to the DRM model.
	 */

	for (i = 0; i < priv->num_encoders; ++i) {
		struct drm_encoder *encoder = priv->encoders[i];

		if (encoder->crtc == crtc) {
			struct omap_dss_device *dssdev;

			dssdev = omap_encoder_get_dssdev(encoder);

			if (dssdev) {
				struct videomode vm = {0};

				dssdev->driver->get_timings(dssdev, &vm);

				omap_crtc->vm.flags |= vm.flags & flags_mask;
			}

			break;
		}
	}
}

static int omap_crtc_atomic_check(struct drm_crtc *crtc,
				struct drm_crtc_state *state)
{
	struct drm_plane_state *pri_state;

	if (state->color_mgmt_changed && state->gamma_lut) {
		unsigned int length = state->gamma_lut->length /
=======

	DBG("%s: set mode: " DRM_MODE_FMT,
	    omap_crtc->name, DRM_MODE_ARG(mode));

	drm_display_mode_to_videomode(mode, &omap_crtc->vm);
}

static bool omap_crtc_is_manually_updated(struct drm_crtc *crtc)
{
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	struct omap_dss_device *dssdev = omap_crtc->pipe->output;

	if (!dssdev || !dssdev->dsi_ops || !dssdev->dsi_ops->is_video_mode)
		return false;

	if (dssdev->dsi_ops->is_video_mode(dssdev))
		return false;

	DBG("detected manually updated display!");
	return true;
}

static int omap_crtc_atomic_check(struct drm_crtc *crtc,
				struct drm_atomic_state *state)
{
	struct drm_crtc_state *crtc_state = drm_atomic_get_new_crtc_state(state,
									  crtc);
	struct drm_plane_state *pri_state;

	if (crtc_state->color_mgmt_changed && crtc_state->degamma_lut) {
		unsigned int length = crtc_state->degamma_lut->length /
>>>>>>> upstream/android-13
			sizeof(struct drm_color_lut);

		if (length < 2)
			return -EINVAL;
	}

<<<<<<< HEAD
	pri_state = drm_atomic_get_new_plane_state(state->state, crtc->primary);
	if (pri_state) {
		struct omap_crtc_state *omap_crtc_state =
			to_omap_crtc_state(state);
=======
	pri_state = drm_atomic_get_new_plane_state(state,
						   crtc->primary);
	if (pri_state) {
		struct omap_crtc_state *omap_crtc_state =
			to_omap_crtc_state(crtc_state);
>>>>>>> upstream/android-13

		/* Mirror new values for zpos and rotation in omap_crtc_state */
		omap_crtc_state->zpos = pri_state->zpos;
		omap_crtc_state->rotation = pri_state->rotation;
<<<<<<< HEAD
=======

		/* Check if this CRTC is for a manually updated display */
		omap_crtc_state->manually_updated = omap_crtc_is_manually_updated(crtc);
>>>>>>> upstream/android-13
	}

	return 0;
}

static void omap_crtc_atomic_begin(struct drm_crtc *crtc,
<<<<<<< HEAD
				   struct drm_crtc_state *old_crtc_state)
=======
				   struct drm_atomic_state *state)
>>>>>>> upstream/android-13
{
}

static void omap_crtc_atomic_flush(struct drm_crtc *crtc,
<<<<<<< HEAD
				   struct drm_crtc_state *old_crtc_state)
{
	struct omap_drm_private *priv = crtc->dev->dev_private;
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
=======
				   struct drm_atomic_state *state)
{
	struct omap_drm_private *priv = crtc->dev->dev_private;
	struct omap_crtc *omap_crtc = to_omap_crtc(crtc);
	struct omap_crtc_state *omap_crtc_state = to_omap_crtc_state(crtc->state);
>>>>>>> upstream/android-13
	int ret;

	if (crtc->state->color_mgmt_changed) {
		struct drm_color_lut *lut = NULL;
		unsigned int length = 0;

<<<<<<< HEAD
		if (crtc->state->gamma_lut) {
			lut = (struct drm_color_lut *)
				crtc->state->gamma_lut->data;
			length = crtc->state->gamma_lut->length /
				sizeof(*lut);
		}
		priv->dispc_ops->mgr_set_gamma(priv->dispc, omap_crtc->channel,
=======
		if (crtc->state->degamma_lut) {
			lut = (struct drm_color_lut *)
				crtc->state->degamma_lut->data;
			length = crtc->state->degamma_lut->length /
				sizeof(*lut);
		}
		dispc_mgr_set_gamma(priv->dispc, omap_crtc->channel,
>>>>>>> upstream/android-13
					       lut, length);
	}

	omap_crtc_write_crtc_properties(crtc);

	/* Only flush the CRTC if it is currently enabled. */
	if (!omap_crtc->enabled)
		return;

	DBG("%s: GO", omap_crtc->name);

<<<<<<< HEAD
=======
	if (omap_crtc_state->manually_updated) {
		/* send new image for page flips and modeset changes */
		spin_lock_irq(&crtc->dev->event_lock);
		omap_crtc_flush(crtc);
		omap_crtc_arm_event(crtc);
		spin_unlock_irq(&crtc->dev->event_lock);
		return;
	}

>>>>>>> upstream/android-13
	ret = drm_crtc_vblank_get(crtc);
	WARN_ON(ret != 0);

	spin_lock_irq(&crtc->dev->event_lock);
<<<<<<< HEAD
	priv->dispc_ops->mgr_go(priv->dispc, omap_crtc->channel);
=======
	dispc_mgr_go(priv->dispc, omap_crtc->channel);
>>>>>>> upstream/android-13
	omap_crtc_arm_event(crtc);
	spin_unlock_irq(&crtc->dev->event_lock);
}

static int omap_crtc_atomic_set_property(struct drm_crtc *crtc,
					 struct drm_crtc_state *state,
					 struct drm_property *property,
					 u64 val)
{
	struct omap_drm_private *priv = crtc->dev->dev_private;
	struct drm_plane_state *plane_state;

	/*
	 * Delegate property set to the primary plane. Get the plane state and
	 * set the property directly, the shadow copy will be assigned in the
	 * omap_crtc_atomic_check callback. This way updates to plane state will
	 * always be mirrored in the crtc state correctly.
	 */
	plane_state = drm_atomic_get_plane_state(state->state, crtc->primary);
	if (IS_ERR(plane_state))
		return PTR_ERR(plane_state);

	if (property == crtc->primary->rotation_property)
		plane_state->rotation = val;
	else if (property == priv->zorder_prop)
		plane_state->zpos = val;
	else
		return -EINVAL;

	return 0;
}

static int omap_crtc_atomic_get_property(struct drm_crtc *crtc,
					 const struct drm_crtc_state *state,
					 struct drm_property *property,
					 u64 *val)
{
	struct omap_drm_private *priv = crtc->dev->dev_private;
	struct omap_crtc_state *omap_state = to_omap_crtc_state(state);

	if (property == crtc->primary->rotation_property)
		*val = omap_state->rotation;
	else if (property == priv->zorder_prop)
		*val = omap_state->zpos;
	else
		return -EINVAL;

	return 0;
}

static void omap_crtc_reset(struct drm_crtc *crtc)
{
<<<<<<< HEAD
=======
	struct omap_crtc_state *state;

>>>>>>> upstream/android-13
	if (crtc->state)
		__drm_atomic_helper_crtc_destroy_state(crtc->state);

	kfree(crtc->state);
<<<<<<< HEAD
	crtc->state = kzalloc(sizeof(struct omap_crtc_state), GFP_KERNEL);

	if (crtc->state)
		crtc->state->crtc = crtc;
=======

	state = kzalloc(sizeof(*state), GFP_KERNEL);
	if (state)
		__drm_atomic_helper_crtc_reset(crtc, &state->base);
>>>>>>> upstream/android-13
}

static struct drm_crtc_state *
omap_crtc_duplicate_state(struct drm_crtc *crtc)
{
	struct omap_crtc_state *state, *current_state;

	if (WARN_ON(!crtc->state))
		return NULL;

	current_state = to_omap_crtc_state(crtc->state);

	state = kmalloc(sizeof(*state), GFP_KERNEL);
	if (!state)
		return NULL;

	__drm_atomic_helper_crtc_duplicate_state(crtc, &state->base);

	state->zpos = current_state->zpos;
	state->rotation = current_state->rotation;
<<<<<<< HEAD
=======
	state->manually_updated = current_state->manually_updated;
>>>>>>> upstream/android-13

	return &state->base;
}

static const struct drm_crtc_funcs omap_crtc_funcs = {
	.reset = omap_crtc_reset,
	.set_config = drm_atomic_helper_set_config,
	.destroy = omap_crtc_destroy,
	.page_flip = drm_atomic_helper_page_flip,
<<<<<<< HEAD
	.gamma_set = drm_atomic_helper_legacy_gamma_set,
=======
>>>>>>> upstream/android-13
	.atomic_duplicate_state = omap_crtc_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_crtc_destroy_state,
	.atomic_set_property = omap_crtc_atomic_set_property,
	.atomic_get_property = omap_crtc_atomic_get_property,
	.enable_vblank = omap_irq_enable_vblank,
	.disable_vblank = omap_irq_disable_vblank,
};

static const struct drm_crtc_helper_funcs omap_crtc_helper_funcs = {
	.mode_set_nofb = omap_crtc_mode_set_nofb,
	.atomic_check = omap_crtc_atomic_check,
	.atomic_begin = omap_crtc_atomic_begin,
	.atomic_flush = omap_crtc_atomic_flush,
	.atomic_enable = omap_crtc_atomic_enable,
	.atomic_disable = omap_crtc_atomic_disable,
	.mode_valid = omap_crtc_mode_valid,
};

/* -----------------------------------------------------------------------------
 * Init and Cleanup
 */

static const char *channel_names[] = {
	[OMAP_DSS_CHANNEL_LCD] = "lcd",
	[OMAP_DSS_CHANNEL_DIGIT] = "tv",
	[OMAP_DSS_CHANNEL_LCD2] = "lcd2",
	[OMAP_DSS_CHANNEL_LCD3] = "lcd3",
};

<<<<<<< HEAD
void omap_crtc_pre_init(struct omap_drm_private *priv)
{
	memset(omap_crtcs, 0, sizeof(omap_crtcs));

	dss_install_mgr_ops(&mgr_ops, priv);
}

void omap_crtc_pre_uninit(void)
{
	dss_uninstall_mgr_ops();
}

/* initialize crtc */
struct drm_crtc *omap_crtc_init(struct drm_device *dev,
		struct drm_plane *plane, struct omap_dss_device *dssdev)
=======
/* initialize crtc */
struct drm_crtc *omap_crtc_init(struct drm_device *dev,
				struct omap_drm_pipeline *pipe,
				struct drm_plane *plane)
>>>>>>> upstream/android-13
{
	struct omap_drm_private *priv = dev->dev_private;
	struct drm_crtc *crtc = NULL;
	struct omap_crtc *omap_crtc;
	enum omap_channel channel;
<<<<<<< HEAD
	struct omap_dss_device *out;
	int ret;

	out = omapdss_find_output_from_display(dssdev);
	channel = out->dispc_channel;
	omap_dss_put_device(out);

	DBG("%s", channel_names[channel]);

	/* Multiple displays on same channel is not allowed */
	if (WARN_ON(omap_crtcs[channel] != NULL))
		return ERR_PTR(-EINVAL);

=======
	int ret;

	channel = pipe->output->dispc_channel;

	DBG("%s", channel_names[channel]);

>>>>>>> upstream/android-13
	omap_crtc = kzalloc(sizeof(*omap_crtc), GFP_KERNEL);
	if (!omap_crtc)
		return ERR_PTR(-ENOMEM);

	crtc = &omap_crtc->base;

	init_waitqueue_head(&omap_crtc->pending_wait);

<<<<<<< HEAD
	omap_crtc->channel = channel;
	omap_crtc->name = channel_names[channel];

=======
	omap_crtc->pipe = pipe;
	omap_crtc->channel = channel;
	omap_crtc->name = channel_names[channel];

	/*
	 * We want to refresh manually updated displays from dirty callback,
	 * which is called quite often (e.g. for each drawn line). This will
	 * be used to do the display update asynchronously to avoid blocking
	 * the rendering process and merges multiple dirty calls into one
	 * update if they arrive very fast. We also call this function for
	 * atomic display updates (e.g. for page flips), which means we do
	 * not need extra locking. Atomic updates should be synchronous, but
	 * need to wait for the framedone interrupt anyways.
	 */
	INIT_DELAYED_WORK(&omap_crtc->update_work,
			  omap_crtc_manual_display_update);

>>>>>>> upstream/android-13
	ret = drm_crtc_init_with_planes(dev, crtc, plane, NULL,
					&omap_crtc_funcs, NULL);
	if (ret < 0) {
		dev_err(dev->dev, "%s(): could not init crtc for: %s\n",
<<<<<<< HEAD
			__func__, dssdev->name);
=======
			__func__, pipe->output->name);
>>>>>>> upstream/android-13
		kfree(omap_crtc);
		return ERR_PTR(ret);
	}

	drm_crtc_helper_add(crtc, &omap_crtc_helper_funcs);

	/* The dispc API adapts to what ever size, but the HW supports
	 * 256 element gamma table for LCDs and 1024 element table for
	 * OMAP_DSS_CHANNEL_DIGIT. X server assumes 256 element gamma
	 * tables so lets use that. Size of HW gamma table can be
	 * extracted with dispc_mgr_gamma_size(). If it returns 0
<<<<<<< HEAD
	 * gamma table is not supprted.
	 */
	if (priv->dispc_ops->mgr_gamma_size(priv->dispc, channel)) {
		unsigned int gamma_lut_size = 256;

		drm_crtc_enable_color_mgmt(crtc, 0, false, gamma_lut_size);
=======
	 * gamma table is not supported.
	 */
	if (dispc_mgr_gamma_size(priv->dispc, channel)) {
		unsigned int gamma_lut_size = 256;

		drm_crtc_enable_color_mgmt(crtc, gamma_lut_size, true, 0);
>>>>>>> upstream/android-13
		drm_mode_crtc_set_gamma_size(crtc, gamma_lut_size);
	}

	omap_plane_install_properties(crtc->primary, &crtc->base);

<<<<<<< HEAD
	omap_crtcs[channel] = omap_crtc;

=======
>>>>>>> upstream/android-13
	return crtc;
}
