<<<<<<< HEAD
/*
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
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

#include <drm/drm_crtc.h>
#include <drm/drm_crtc_helper.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 */

#include <drm/drm_crtc.h>
#include <drm/drm_probe_helper.h>
>>>>>>> upstream/android-13

#include "mdp4_kms.h"

struct mdp4_dtv_encoder {
	struct drm_encoder base;
	struct clk *hdmi_clk;
	struct clk *mdp_clk;
	unsigned long int pixclock;
	bool enabled;
	uint32_t bsc;
};
#define to_mdp4_dtv_encoder(x) container_of(x, struct mdp4_dtv_encoder, base)

static struct mdp4_kms *get_kms(struct drm_encoder *encoder)
{
	struct msm_drm_private *priv = encoder->dev->dev_private;
	return to_mdp4_kms(to_mdp_kms(priv->kms));
}

<<<<<<< HEAD
#ifdef DOWNSTREAM_CONFIG_MSM_BUS_SCALING
#include <mach/board.h>
/* not ironically named at all.. no, really.. */
static void bs_init(struct mdp4_dtv_encoder *mdp4_dtv_encoder)
{
	struct drm_device *dev = mdp4_dtv_encoder->base.dev;
	struct lcdc_platform_data *dtv_pdata = mdp4_find_pdata("dtv.0");

	if (!dtv_pdata) {
		dev_err(dev->dev, "could not find dtv pdata\n");
		return;
	}

	if (dtv_pdata->bus_scale_table) {
		mdp4_dtv_encoder->bsc = msm_bus_scale_register_client(
				dtv_pdata->bus_scale_table);
		DBG("bus scale client: %08x", mdp4_dtv_encoder->bsc);
		DBG("lcdc_power_save: %p", dtv_pdata->lcdc_power_save);
		if (dtv_pdata->lcdc_power_save)
			dtv_pdata->lcdc_power_save(1);
	}
}

static void bs_fini(struct mdp4_dtv_encoder *mdp4_dtv_encoder)
{
	if (mdp4_dtv_encoder->bsc) {
		msm_bus_scale_unregister_client(mdp4_dtv_encoder->bsc);
		mdp4_dtv_encoder->bsc = 0;
	}
}

static void bs_set(struct mdp4_dtv_encoder *mdp4_dtv_encoder, int idx)
{
	if (mdp4_dtv_encoder->bsc) {
		DBG("set bus scaling: %d", idx);
		msm_bus_scale_client_update_request(mdp4_dtv_encoder->bsc, idx);
	}
}
#else
static void bs_init(struct mdp4_dtv_encoder *mdp4_dtv_encoder) {}
static void bs_fini(struct mdp4_dtv_encoder *mdp4_dtv_encoder) {}
static void bs_set(struct mdp4_dtv_encoder *mdp4_dtv_encoder, int idx) {}
#endif

static void mdp4_dtv_encoder_destroy(struct drm_encoder *encoder)
{
	struct mdp4_dtv_encoder *mdp4_dtv_encoder = to_mdp4_dtv_encoder(encoder);
	bs_fini(mdp4_dtv_encoder);
=======
static void mdp4_dtv_encoder_destroy(struct drm_encoder *encoder)
{
	struct mdp4_dtv_encoder *mdp4_dtv_encoder = to_mdp4_dtv_encoder(encoder);
>>>>>>> upstream/android-13
	drm_encoder_cleanup(encoder);
	kfree(mdp4_dtv_encoder);
}

static const struct drm_encoder_funcs mdp4_dtv_encoder_funcs = {
	.destroy = mdp4_dtv_encoder_destroy,
};

static void mdp4_dtv_encoder_mode_set(struct drm_encoder *encoder,
		struct drm_display_mode *mode,
		struct drm_display_mode *adjusted_mode)
{
	struct mdp4_dtv_encoder *mdp4_dtv_encoder = to_mdp4_dtv_encoder(encoder);
	struct mdp4_kms *mdp4_kms = get_kms(encoder);
	uint32_t dtv_hsync_skew, vsync_period, vsync_len, ctrl_pol;
	uint32_t display_v_start, display_v_end;
	uint32_t hsync_start_x, hsync_end_x;

	mode = adjusted_mode;

<<<<<<< HEAD
	DBG("set mode: %d:\"%s\" %d %d %d %d %d %d %d %d %d %d 0x%x 0x%x",
			mode->base.id, mode->name,
			mode->vrefresh, mode->clock,
			mode->hdisplay, mode->hsync_start,
			mode->hsync_end, mode->htotal,
			mode->vdisplay, mode->vsync_start,
			mode->vsync_end, mode->vtotal,
			mode->type, mode->flags);
=======
	DBG("set mode: " DRM_MODE_FMT, DRM_MODE_ARG(mode));
>>>>>>> upstream/android-13

	mdp4_dtv_encoder->pixclock = mode->clock * 1000;

	DBG("pixclock=%lu", mdp4_dtv_encoder->pixclock);

	ctrl_pol = 0;
	if (mode->flags & DRM_MODE_FLAG_NHSYNC)
		ctrl_pol |= MDP4_DTV_CTRL_POLARITY_HSYNC_LOW;
	if (mode->flags & DRM_MODE_FLAG_NVSYNC)
		ctrl_pol |= MDP4_DTV_CTRL_POLARITY_VSYNC_LOW;
	/* probably need to get DATA_EN polarity from panel.. */

	dtv_hsync_skew = 0;  /* get this from panel? */

	hsync_start_x = (mode->htotal - mode->hsync_start);
	hsync_end_x = mode->htotal - (mode->hsync_start - mode->hdisplay) - 1;

	vsync_period = mode->vtotal * mode->htotal;
	vsync_len = (mode->vsync_end - mode->vsync_start) * mode->htotal;
	display_v_start = (mode->vtotal - mode->vsync_start) * mode->htotal + dtv_hsync_skew;
	display_v_end = vsync_period - ((mode->vsync_start - mode->vdisplay) * mode->htotal) + dtv_hsync_skew - 1;

	mdp4_write(mdp4_kms, REG_MDP4_DTV_HSYNC_CTRL,
			MDP4_DTV_HSYNC_CTRL_PULSEW(mode->hsync_end - mode->hsync_start) |
			MDP4_DTV_HSYNC_CTRL_PERIOD(mode->htotal));
	mdp4_write(mdp4_kms, REG_MDP4_DTV_VSYNC_PERIOD, vsync_period);
	mdp4_write(mdp4_kms, REG_MDP4_DTV_VSYNC_LEN, vsync_len);
	mdp4_write(mdp4_kms, REG_MDP4_DTV_DISPLAY_HCTRL,
			MDP4_DTV_DISPLAY_HCTRL_START(hsync_start_x) |
			MDP4_DTV_DISPLAY_HCTRL_END(hsync_end_x));
	mdp4_write(mdp4_kms, REG_MDP4_DTV_DISPLAY_VSTART, display_v_start);
	mdp4_write(mdp4_kms, REG_MDP4_DTV_DISPLAY_VEND, display_v_end);
	mdp4_write(mdp4_kms, REG_MDP4_DTV_BORDER_CLR, 0);
	mdp4_write(mdp4_kms, REG_MDP4_DTV_UNDERFLOW_CLR,
			MDP4_DTV_UNDERFLOW_CLR_ENABLE_RECOVERY |
			MDP4_DTV_UNDERFLOW_CLR_COLOR(0xff));
	mdp4_write(mdp4_kms, REG_MDP4_DTV_HSYNC_SKEW, dtv_hsync_skew);
	mdp4_write(mdp4_kms, REG_MDP4_DTV_CTRL_POLARITY, ctrl_pol);
	mdp4_write(mdp4_kms, REG_MDP4_DTV_ACTIVE_HCTL,
			MDP4_DTV_ACTIVE_HCTL_START(0) |
			MDP4_DTV_ACTIVE_HCTL_END(0));
	mdp4_write(mdp4_kms, REG_MDP4_DTV_ACTIVE_VSTART, 0);
	mdp4_write(mdp4_kms, REG_MDP4_DTV_ACTIVE_VEND, 0);
}

static void mdp4_dtv_encoder_disable(struct drm_encoder *encoder)
{
	struct mdp4_dtv_encoder *mdp4_dtv_encoder = to_mdp4_dtv_encoder(encoder);
	struct mdp4_kms *mdp4_kms = get_kms(encoder);

	if (WARN_ON(!mdp4_dtv_encoder->enabled))
		return;

	mdp4_write(mdp4_kms, REG_MDP4_DTV_ENABLE, 0);

	/*
	 * Wait for a vsync so we know the ENABLE=0 latched before
	 * the (connector) source of the vsync's gets disabled,
	 * otherwise we end up in a funny state if we re-enable
	 * before the disable latches, which results that some of
	 * the settings changes for the new modeset (like new
	 * scanout buffer) don't latch properly..
	 */
	mdp_irq_wait(&mdp4_kms->base, MDP4_IRQ_EXTERNAL_VSYNC);

	clk_disable_unprepare(mdp4_dtv_encoder->hdmi_clk);
	clk_disable_unprepare(mdp4_dtv_encoder->mdp_clk);

<<<<<<< HEAD
	bs_set(mdp4_dtv_encoder, 0);

=======
>>>>>>> upstream/android-13
	mdp4_dtv_encoder->enabled = false;
}

static void mdp4_dtv_encoder_enable(struct drm_encoder *encoder)
{
	struct drm_device *dev = encoder->dev;
	struct mdp4_dtv_encoder *mdp4_dtv_encoder = to_mdp4_dtv_encoder(encoder);
	struct mdp4_kms *mdp4_kms = get_kms(encoder);
	unsigned long pc = mdp4_dtv_encoder->pixclock;
	int ret;

	if (WARN_ON(mdp4_dtv_encoder->enabled))
		return;

	mdp4_crtc_set_config(encoder->crtc,
			MDP4_DMA_CONFIG_R_BPC(BPC8) |
			MDP4_DMA_CONFIG_G_BPC(BPC8) |
			MDP4_DMA_CONFIG_B_BPC(BPC8) |
			MDP4_DMA_CONFIG_PACK(0x21));
	mdp4_crtc_set_intf(encoder->crtc, INTF_LCDC_DTV, 1);

<<<<<<< HEAD
	bs_set(mdp4_dtv_encoder, 1);

=======
>>>>>>> upstream/android-13
	DBG("setting mdp_clk=%lu", pc);

	ret = clk_set_rate(mdp4_dtv_encoder->mdp_clk, pc);
	if (ret)
<<<<<<< HEAD
		dev_err(dev->dev, "failed to set mdp_clk to %lu: %d\n",
=======
		DRM_DEV_ERROR(dev->dev, "failed to set mdp_clk to %lu: %d\n",
>>>>>>> upstream/android-13
			pc, ret);

	ret = clk_prepare_enable(mdp4_dtv_encoder->mdp_clk);
	if (ret)
<<<<<<< HEAD
		dev_err(dev->dev, "failed to enabled mdp_clk: %d\n", ret);

	ret = clk_prepare_enable(mdp4_dtv_encoder->hdmi_clk);
	if (ret)
		dev_err(dev->dev, "failed to enable hdmi_clk: %d\n", ret);
=======
		DRM_DEV_ERROR(dev->dev, "failed to enabled mdp_clk: %d\n", ret);

	ret = clk_prepare_enable(mdp4_dtv_encoder->hdmi_clk);
	if (ret)
		DRM_DEV_ERROR(dev->dev, "failed to enable hdmi_clk: %d\n", ret);
>>>>>>> upstream/android-13

	mdp4_write(mdp4_kms, REG_MDP4_DTV_ENABLE, 1);

	mdp4_dtv_encoder->enabled = true;
}

static const struct drm_encoder_helper_funcs mdp4_dtv_encoder_helper_funcs = {
	.mode_set = mdp4_dtv_encoder_mode_set,
	.enable = mdp4_dtv_encoder_enable,
	.disable = mdp4_dtv_encoder_disable,
};

long mdp4_dtv_round_pixclk(struct drm_encoder *encoder, unsigned long rate)
{
	struct mdp4_dtv_encoder *mdp4_dtv_encoder = to_mdp4_dtv_encoder(encoder);
	return clk_round_rate(mdp4_dtv_encoder->mdp_clk, rate);
}

/* initialize encoder */
struct drm_encoder *mdp4_dtv_encoder_init(struct drm_device *dev)
{
	struct drm_encoder *encoder = NULL;
	struct mdp4_dtv_encoder *mdp4_dtv_encoder;
	int ret;

	mdp4_dtv_encoder = kzalloc(sizeof(*mdp4_dtv_encoder), GFP_KERNEL);
	if (!mdp4_dtv_encoder) {
		ret = -ENOMEM;
		goto fail;
	}

	encoder = &mdp4_dtv_encoder->base;

	drm_encoder_init(dev, encoder, &mdp4_dtv_encoder_funcs,
			 DRM_MODE_ENCODER_TMDS, NULL);
	drm_encoder_helper_add(encoder, &mdp4_dtv_encoder_helper_funcs);

	mdp4_dtv_encoder->hdmi_clk = devm_clk_get(dev->dev, "hdmi_clk");
	if (IS_ERR(mdp4_dtv_encoder->hdmi_clk)) {
<<<<<<< HEAD
		dev_err(dev->dev, "failed to get hdmi_clk\n");
=======
		DRM_DEV_ERROR(dev->dev, "failed to get hdmi_clk\n");
>>>>>>> upstream/android-13
		ret = PTR_ERR(mdp4_dtv_encoder->hdmi_clk);
		goto fail;
	}

	mdp4_dtv_encoder->mdp_clk = devm_clk_get(dev->dev, "tv_clk");
	if (IS_ERR(mdp4_dtv_encoder->mdp_clk)) {
<<<<<<< HEAD
		dev_err(dev->dev, "failed to get tv_clk\n");
=======
		DRM_DEV_ERROR(dev->dev, "failed to get tv_clk\n");
>>>>>>> upstream/android-13
		ret = PTR_ERR(mdp4_dtv_encoder->mdp_clk);
		goto fail;
	}

<<<<<<< HEAD
	bs_init(mdp4_dtv_encoder);

=======
>>>>>>> upstream/android-13
	return encoder;

fail:
	if (encoder)
		mdp4_dtv_encoder_destroy(encoder);

	return ERR_PTR(ret);
}
