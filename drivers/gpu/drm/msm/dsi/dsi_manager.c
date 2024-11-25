<<<<<<< HEAD
/*
 * Copyright (c) 2015, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2015, The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */

#include "msm_kms.h"
#include "dsi.h"

#define DSI_CLOCK_MASTER	DSI_0
#define DSI_CLOCK_SLAVE		DSI_1

#define DSI_LEFT		DSI_0
#define DSI_RIGHT		DSI_1

/* According to the current drm framework sequence, take the encoder of
 * DSI_1 as master encoder
 */
#define DSI_ENCODER_MASTER	DSI_1
#define DSI_ENCODER_SLAVE	DSI_0

struct msm_dsi_manager {
	struct msm_dsi *dsi[DSI_MAX];

<<<<<<< HEAD
	bool is_dual_dsi;
=======
	bool is_bonded_dsi;
>>>>>>> upstream/android-13
	bool is_sync_needed;
	int master_dsi_link_id;
};

static struct msm_dsi_manager msm_dsim_glb;

<<<<<<< HEAD
#define IS_DUAL_DSI()		(msm_dsim_glb.is_dual_dsi)
=======
#define IS_BONDED_DSI()		(msm_dsim_glb.is_bonded_dsi)
>>>>>>> upstream/android-13
#define IS_SYNC_NEEDED()	(msm_dsim_glb.is_sync_needed)
#define IS_MASTER_DSI_LINK(id)	(msm_dsim_glb.master_dsi_link_id == id)

static inline struct msm_dsi *dsi_mgr_get_dsi(int id)
{
	return msm_dsim_glb.dsi[id];
}

static inline struct msm_dsi *dsi_mgr_get_other_dsi(int id)
{
	return msm_dsim_glb.dsi[(id + 1) % DSI_MAX];
}

<<<<<<< HEAD
static int dsi_mgr_parse_dual_dsi(struct device_node *np, int id)
{
	struct msm_dsi_manager *msm_dsim = &msm_dsim_glb;

	/* We assume 2 dsi nodes have the same information of dual-dsi and
	 * sync-mode, and only one node specifies master in case of dual mode.
	 */
	if (!msm_dsim->is_dual_dsi)
		msm_dsim->is_dual_dsi = of_property_read_bool(
						np, "qcom,dual-dsi-mode");

	if (msm_dsim->is_dual_dsi) {
=======
static int dsi_mgr_parse_of(struct device_node *np, int id)
{
	struct msm_dsi_manager *msm_dsim = &msm_dsim_glb;

	/* We assume 2 dsi nodes have the same information of bonded dsi and
	 * sync-mode, and only one node specifies master in case of bonded mode.
	 */
	if (!msm_dsim->is_bonded_dsi)
		msm_dsim->is_bonded_dsi = of_property_read_bool(np, "qcom,dual-dsi-mode");

	if (msm_dsim->is_bonded_dsi) {
>>>>>>> upstream/android-13
		if (of_property_read_bool(np, "qcom,master-dsi"))
			msm_dsim->master_dsi_link_id = id;
		if (!msm_dsim->is_sync_needed)
			msm_dsim->is_sync_needed = of_property_read_bool(
					np, "qcom,sync-dual-dsi");
	}

	return 0;
}

static int dsi_mgr_setup_components(int id)
{
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct msm_dsi *other_dsi = dsi_mgr_get_other_dsi(id);
	struct msm_dsi *clk_master_dsi = dsi_mgr_get_dsi(DSI_CLOCK_MASTER);
	struct msm_dsi *clk_slave_dsi = dsi_mgr_get_dsi(DSI_CLOCK_SLAVE);
<<<<<<< HEAD
	struct msm_dsi_pll *src_pll;
	int ret;

	if (!IS_DUAL_DSI()) {
=======
	int ret;

	if (!IS_BONDED_DSI()) {
>>>>>>> upstream/android-13
		ret = msm_dsi_host_register(msm_dsi->host, true);
		if (ret)
			return ret;

		msm_dsi_phy_set_usecase(msm_dsi->phy, MSM_DSI_PHY_STANDALONE);
<<<<<<< HEAD
		src_pll = msm_dsi_phy_get_pll(msm_dsi->phy);
		if (IS_ERR(src_pll))
			return PTR_ERR(src_pll);
		ret = msm_dsi_host_set_src_pll(msm_dsi->host, src_pll);
=======
		ret = msm_dsi_host_set_src_pll(msm_dsi->host, msm_dsi->phy);
>>>>>>> upstream/android-13
	} else if (!other_dsi) {
		ret = 0;
	} else {
		struct msm_dsi *master_link_dsi = IS_MASTER_DSI_LINK(id) ?
							msm_dsi : other_dsi;
		struct msm_dsi *slave_link_dsi = IS_MASTER_DSI_LINK(id) ?
							other_dsi : msm_dsi;
		/* Register slave host first, so that slave DSI device
		 * has a chance to probe, and do not block the master
		 * DSI device's probe.
		 * Also, do not check defer for the slave host,
		 * because only master DSI device adds the panel to global
		 * panel list. The panel's device is the master DSI device.
		 */
		ret = msm_dsi_host_register(slave_link_dsi->host, false);
		if (ret)
			return ret;
		ret = msm_dsi_host_register(master_link_dsi->host, true);
		if (ret)
			return ret;

<<<<<<< HEAD
		/* PLL0 is to drive both 2 DSI link clocks in Dual DSI mode. */
=======
		/* PLL0 is to drive both 2 DSI link clocks in bonded DSI mode. */
>>>>>>> upstream/android-13
		msm_dsi_phy_set_usecase(clk_master_dsi->phy,
					MSM_DSI_PHY_MASTER);
		msm_dsi_phy_set_usecase(clk_slave_dsi->phy,
					MSM_DSI_PHY_SLAVE);
<<<<<<< HEAD
		src_pll = msm_dsi_phy_get_pll(clk_master_dsi->phy);
		if (IS_ERR(src_pll))
			return PTR_ERR(src_pll);
		ret = msm_dsi_host_set_src_pll(msm_dsi->host, src_pll);
		if (ret)
			return ret;
		ret = msm_dsi_host_set_src_pll(other_dsi->host, src_pll);
=======
		ret = msm_dsi_host_set_src_pll(msm_dsi->host, clk_master_dsi->phy);
		if (ret)
			return ret;
		ret = msm_dsi_host_set_src_pll(other_dsi->host, clk_master_dsi->phy);
>>>>>>> upstream/android-13
	}

	return ret;
}

<<<<<<< HEAD
static int enable_phy(struct msm_dsi *msm_dsi, int src_pll_id,
=======
static int enable_phy(struct msm_dsi *msm_dsi,
>>>>>>> upstream/android-13
		      struct msm_dsi_phy_shared_timings *shared_timings)
{
	struct msm_dsi_phy_clk_request clk_req;
	int ret;
<<<<<<< HEAD
	bool is_dual_dsi = IS_DUAL_DSI();

	msm_dsi_host_get_phy_clk_req(msm_dsi->host, &clk_req, is_dual_dsi);

	ret = msm_dsi_phy_enable(msm_dsi->phy, src_pll_id, &clk_req);
	msm_dsi_phy_get_shared_timings(msm_dsi->phy, shared_timings);
=======
	bool is_bonded_dsi = IS_BONDED_DSI();

	msm_dsi_host_get_phy_clk_req(msm_dsi->host, &clk_req, is_bonded_dsi);

	ret = msm_dsi_phy_enable(msm_dsi->phy, &clk_req, shared_timings);
>>>>>>> upstream/android-13

	return ret;
}

static int
dsi_mgr_phy_enable(int id,
		   struct msm_dsi_phy_shared_timings shared_timings[DSI_MAX])
{
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct msm_dsi *mdsi = dsi_mgr_get_dsi(DSI_CLOCK_MASTER);
	struct msm_dsi *sdsi = dsi_mgr_get_dsi(DSI_CLOCK_SLAVE);
<<<<<<< HEAD
	int src_pll_id = IS_DUAL_DSI() ? DSI_CLOCK_MASTER : id;
	int ret;

	/* In case of dual DSI, some registers in PHY1 have been programmed
=======
	int ret;

	/* In case of bonded DSI, some registers in PHY1 have been programmed
>>>>>>> upstream/android-13
	 * during PLL0 clock's set_rate. The PHY1 reset called by host1 here
	 * will silently reset those PHY1 registers. Therefore we need to reset
	 * and enable both PHYs before any PLL clock operation.
	 */
<<<<<<< HEAD
	if (IS_DUAL_DSI() && mdsi && sdsi) {
=======
	if (IS_BONDED_DSI() && mdsi && sdsi) {
>>>>>>> upstream/android-13
		if (!mdsi->phy_enabled && !sdsi->phy_enabled) {
			msm_dsi_host_reset_phy(mdsi->host);
			msm_dsi_host_reset_phy(sdsi->host);

<<<<<<< HEAD
			ret = enable_phy(mdsi, src_pll_id,
					 &shared_timings[DSI_CLOCK_MASTER]);
			if (ret)
				return ret;
			ret = enable_phy(sdsi, src_pll_id,
=======
			ret = enable_phy(mdsi,
					 &shared_timings[DSI_CLOCK_MASTER]);
			if (ret)
				return ret;
			ret = enable_phy(sdsi,
>>>>>>> upstream/android-13
					 &shared_timings[DSI_CLOCK_SLAVE]);
			if (ret) {
				msm_dsi_phy_disable(mdsi->phy);
				return ret;
			}
		}
	} else {
		msm_dsi_host_reset_phy(msm_dsi->host);
<<<<<<< HEAD
		ret = enable_phy(msm_dsi, src_pll_id, &shared_timings[id]);
=======
		ret = enable_phy(msm_dsi, &shared_timings[id]);
>>>>>>> upstream/android-13
		if (ret)
			return ret;
	}

	msm_dsi->phy_enabled = true;

	return 0;
}

static void dsi_mgr_phy_disable(int id)
{
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct msm_dsi *mdsi = dsi_mgr_get_dsi(DSI_CLOCK_MASTER);
	struct msm_dsi *sdsi = dsi_mgr_get_dsi(DSI_CLOCK_SLAVE);

	/* disable DSI phy
<<<<<<< HEAD
	 * In dual-dsi configuration, the phy should be disabled for the
	 * first controller only when the second controller is disabled.
	 */
	msm_dsi->phy_enabled = false;
	if (IS_DUAL_DSI() && mdsi && sdsi) {
=======
	 * In bonded dsi configuration, the phy should be disabled for the
	 * first controller only when the second controller is disabled.
	 */
	msm_dsi->phy_enabled = false;
	if (IS_BONDED_DSI() && mdsi && sdsi) {
>>>>>>> upstream/android-13
		if (!mdsi->phy_enabled && !sdsi->phy_enabled) {
			msm_dsi_phy_disable(sdsi->phy);
			msm_dsi_phy_disable(mdsi->phy);
		}
	} else {
		msm_dsi_phy_disable(msm_dsi->phy);
	}
}

struct dsi_connector {
	struct drm_connector base;
	int id;
};

struct dsi_bridge {
	struct drm_bridge base;
	int id;
};

#define to_dsi_connector(x) container_of(x, struct dsi_connector, base)
#define to_dsi_bridge(x) container_of(x, struct dsi_bridge, base)

static inline int dsi_mgr_connector_get_id(struct drm_connector *connector)
{
	struct dsi_connector *dsi_connector = to_dsi_connector(connector);
	return dsi_connector->id;
}

static int dsi_mgr_bridge_get_id(struct drm_bridge *bridge)
{
	struct dsi_bridge *dsi_bridge = to_dsi_bridge(bridge);
	return dsi_bridge->id;
}

<<<<<<< HEAD
=======
static int msm_dsi_manager_panel_init(struct drm_connector *conn, u8 id)
{
	struct msm_drm_private *priv = conn->dev->dev_private;
	struct msm_kms *kms = priv->kms;
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct msm_dsi *other_dsi = dsi_mgr_get_other_dsi(id);
	struct msm_dsi *master_dsi, *slave_dsi;
	struct drm_panel *panel;

	if (IS_BONDED_DSI() && !IS_MASTER_DSI_LINK(id)) {
		master_dsi = other_dsi;
		slave_dsi = msm_dsi;
	} else {
		master_dsi = msm_dsi;
		slave_dsi = other_dsi;
	}

	/*
	 * There is only 1 panel in the global panel list for bonded DSI mode.
	 * Therefore slave dsi should get the drm_panel instance from master
	 * dsi.
	 */
	panel = msm_dsi_host_get_panel(master_dsi->host);
	if (IS_ERR(panel)) {
		DRM_ERROR("Could not find panel for %u (%ld)\n", msm_dsi->id,
			  PTR_ERR(panel));
		return PTR_ERR(panel);
	}

	if (!panel || !IS_BONDED_DSI())
		goto out;

	drm_object_attach_property(&conn->base,
				   conn->dev->mode_config.tile_property, 0);

	/*
	 * Set split display info to kms once bonded DSI panel is connected to
	 * both hosts.
	 */
	if (other_dsi && other_dsi->panel && kms->funcs->set_split_display) {
		kms->funcs->set_split_display(kms, master_dsi->encoder,
					      slave_dsi->encoder,
					      msm_dsi_is_cmd_mode(msm_dsi));
	}

out:
	msm_dsi->panel = panel;
	return 0;
}

>>>>>>> upstream/android-13
static enum drm_connector_status dsi_mgr_connector_detect(
		struct drm_connector *connector, bool force)
{
	int id = dsi_mgr_connector_get_id(connector);
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
<<<<<<< HEAD
	struct msm_dsi *other_dsi = dsi_mgr_get_other_dsi(id);
	struct msm_drm_private *priv = connector->dev->dev_private;
	struct msm_kms *kms = priv->kms;

	DBG("id=%d", id);
	if (!msm_dsi->panel) {
		msm_dsi->panel = msm_dsi_host_get_panel(msm_dsi->host,
						&msm_dsi->device_flags);

		/* There is only 1 panel in the global panel list
		 * for dual DSI mode. Therefore slave dsi should get
		 * the drm_panel instance from master dsi, and
		 * keep using the panel flags got from the current DSI link.
		 */
		if (!msm_dsi->panel && IS_DUAL_DSI() &&
			!IS_MASTER_DSI_LINK(id) && other_dsi)
			msm_dsi->panel = msm_dsi_host_get_panel(
					other_dsi->host, NULL);


		if (msm_dsi->panel && kms->funcs->set_encoder_mode) {
			bool cmd_mode = !(msm_dsi->device_flags &
					  MIPI_DSI_MODE_VIDEO);
			struct drm_encoder *encoder =
					msm_dsi_get_encoder(msm_dsi);

			kms->funcs->set_encoder_mode(kms, encoder, cmd_mode);
		}

		if (msm_dsi->panel && IS_DUAL_DSI())
			drm_object_attach_property(&connector->base,
				connector->dev->mode_config.tile_property, 0);

		/* Set split display info to kms once dual DSI panel is
		 * connected to both hosts.
		 */
		if (msm_dsi->panel && IS_DUAL_DSI() &&
			other_dsi && other_dsi->panel) {
			bool cmd_mode = !(msm_dsi->device_flags &
						MIPI_DSI_MODE_VIDEO);
			struct drm_encoder *encoder = msm_dsi_get_encoder(
					dsi_mgr_get_dsi(DSI_ENCODER_MASTER));
			struct drm_encoder *slave_enc = msm_dsi_get_encoder(
					dsi_mgr_get_dsi(DSI_ENCODER_SLAVE));

			if (kms->funcs->set_split_display)
				kms->funcs->set_split_display(kms, encoder,
							slave_enc, cmd_mode);
			else
				pr_err("mdp does not support dual DSI\n");
		}
	}
=======
>>>>>>> upstream/android-13

	return msm_dsi->panel ? connector_status_connected :
		connector_status_disconnected;
}

static void dsi_mgr_connector_destroy(struct drm_connector *connector)
{
	struct dsi_connector *dsi_connector = to_dsi_connector(connector);

	DBG("");

	drm_connector_cleanup(connector);

	kfree(dsi_connector);
}

static int dsi_mgr_connector_get_modes(struct drm_connector *connector)
{
	int id = dsi_mgr_connector_get_id(connector);
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct drm_panel *panel = msm_dsi->panel;
	int num;

	if (!panel)
		return 0;

	/*
<<<<<<< HEAD
	 * In dual DSI mode, we have one connector that can be
	 * attached to the drm_panel.
	 */
	drm_panel_attach(panel, connector);
	num = drm_panel_get_modes(panel);
=======
	 * In bonded DSI mode, we have one connector that can be
	 * attached to the drm_panel.
	 */
	num = drm_panel_get_modes(panel, connector);
>>>>>>> upstream/android-13
	if (!num)
		return 0;

	return num;
}

static enum drm_mode_status dsi_mgr_connector_mode_valid(struct drm_connector *connector,
				struct drm_display_mode *mode)
{
	int id = dsi_mgr_connector_get_id(connector);
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct drm_encoder *encoder = msm_dsi_get_encoder(msm_dsi);
	struct msm_drm_private *priv = connector->dev->dev_private;
	struct msm_kms *kms = priv->kms;
	long actual, requested;

	DBG("");
	requested = 1000 * mode->clock;
	actual = kms->funcs->round_pixclk(kms, requested, encoder);

	DBG("requested=%ld, actual=%ld", requested, actual);
	if (actual != requested)
		return MODE_CLOCK_RANGE;

	return MODE_OK;
}

static struct drm_encoder *
dsi_mgr_connector_best_encoder(struct drm_connector *connector)
{
	int id = dsi_mgr_connector_get_id(connector);
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);

	DBG("");
	return msm_dsi_get_encoder(msm_dsi);
}

static void dsi_mgr_bridge_pre_enable(struct drm_bridge *bridge)
{
	int id = dsi_mgr_bridge_get_id(bridge);
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct msm_dsi *msm_dsi1 = dsi_mgr_get_dsi(DSI_1);
	struct mipi_dsi_host *host = msm_dsi->host;
	struct drm_panel *panel = msm_dsi->panel;
	struct msm_dsi_phy_shared_timings phy_shared_timings[DSI_MAX];
<<<<<<< HEAD
	bool is_dual_dsi = IS_DUAL_DSI();
=======
	bool is_bonded_dsi = IS_BONDED_DSI();
>>>>>>> upstream/android-13
	int ret;

	DBG("id=%d", id);
	if (!msm_dsi_device_connected(msm_dsi))
		return;

<<<<<<< HEAD
=======
	/* Do nothing with the host if it is slave-DSI in case of bonded DSI */
	if (is_bonded_dsi && !IS_MASTER_DSI_LINK(id))
		return;

>>>>>>> upstream/android-13
	ret = dsi_mgr_phy_enable(id, phy_shared_timings);
	if (ret)
		goto phy_en_fail;

<<<<<<< HEAD
	/* Do nothing with the host if it is slave-DSI in case of dual DSI */
	if (is_dual_dsi && !IS_MASTER_DSI_LINK(id))
		return;

	ret = msm_dsi_host_power_on(host, &phy_shared_timings[id], is_dual_dsi);
=======
	ret = msm_dsi_host_power_on(host, &phy_shared_timings[id], is_bonded_dsi, msm_dsi->phy);
>>>>>>> upstream/android-13
	if (ret) {
		pr_err("%s: power on host %d failed, %d\n", __func__, id, ret);
		goto host_on_fail;
	}

<<<<<<< HEAD
	if (is_dual_dsi && msm_dsi1) {
		ret = msm_dsi_host_power_on(msm_dsi1->host,
				&phy_shared_timings[DSI_1], is_dual_dsi);
=======
	if (is_bonded_dsi && msm_dsi1) {
		ret = msm_dsi_host_power_on(msm_dsi1->host,
				&phy_shared_timings[DSI_1], is_bonded_dsi, msm_dsi1->phy);
>>>>>>> upstream/android-13
		if (ret) {
			pr_err("%s: power on host1 failed, %d\n",
							__func__, ret);
			goto host1_on_fail;
		}
	}

<<<<<<< HEAD
=======
	/*
	 * Enable before preparing the panel, disable after unpreparing, so
	 * that the panel can communicate over the DSI link.
	 */
	msm_dsi_host_enable_irq(host);
	if (is_bonded_dsi && msm_dsi1)
		msm_dsi_host_enable_irq(msm_dsi1->host);

>>>>>>> upstream/android-13
	/* Always call panel functions once, because even for dual panels,
	 * there is only one drm_panel instance.
	 */
	if (panel) {
		ret = drm_panel_prepare(panel);
		if (ret) {
			pr_err("%s: prepare panel %d failed, %d\n", __func__,
								id, ret);
			goto panel_prep_fail;
		}
	}

	ret = msm_dsi_host_enable(host);
	if (ret) {
		pr_err("%s: enable host %d failed, %d\n", __func__, id, ret);
		goto host_en_fail;
	}

<<<<<<< HEAD
	if (is_dual_dsi && msm_dsi1) {
=======
	if (is_bonded_dsi && msm_dsi1) {
>>>>>>> upstream/android-13
		ret = msm_dsi_host_enable(msm_dsi1->host);
		if (ret) {
			pr_err("%s: enable host1 failed, %d\n", __func__, ret);
			goto host1_en_fail;
		}
	}

<<<<<<< HEAD
	if (panel) {
		ret = drm_panel_enable(panel);
		if (ret) {
			pr_err("%s: enable panel %d failed, %d\n", __func__, id,
									ret);
			goto panel_en_fail;
		}
	}

	return;

panel_en_fail:
	if (is_dual_dsi && msm_dsi1)
		msm_dsi_host_disable(msm_dsi1->host);
=======
	return;

>>>>>>> upstream/android-13
host1_en_fail:
	msm_dsi_host_disable(host);
host_en_fail:
	if (panel)
		drm_panel_unprepare(panel);
panel_prep_fail:
<<<<<<< HEAD
	if (is_dual_dsi && msm_dsi1)
=======
	msm_dsi_host_disable_irq(host);
	if (is_bonded_dsi && msm_dsi1)
		msm_dsi_host_disable_irq(msm_dsi1->host);

	if (is_bonded_dsi && msm_dsi1)
>>>>>>> upstream/android-13
		msm_dsi_host_power_off(msm_dsi1->host);
host1_on_fail:
	msm_dsi_host_power_off(host);
host_on_fail:
	dsi_mgr_phy_disable(id);
phy_en_fail:
	return;
}

<<<<<<< HEAD
static void dsi_mgr_bridge_enable(struct drm_bridge *bridge)
{
	DBG("");
=======
void msm_dsi_manager_tpg_enable(void)
{
	struct msm_dsi *m_dsi = dsi_mgr_get_dsi(DSI_0);
	struct msm_dsi *s_dsi = dsi_mgr_get_dsi(DSI_1);

	/* if dual dsi, trigger tpg on master first then slave */
	if (m_dsi) {
		msm_dsi_host_test_pattern_en(m_dsi->host);
		if (IS_BONDED_DSI() && s_dsi)
			msm_dsi_host_test_pattern_en(s_dsi->host);
	}
}

static void dsi_mgr_bridge_enable(struct drm_bridge *bridge)
{
	int id = dsi_mgr_bridge_get_id(bridge);
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct drm_panel *panel = msm_dsi->panel;
	bool is_bonded_dsi = IS_BONDED_DSI();
	int ret;

	DBG("id=%d", id);
	if (!msm_dsi_device_connected(msm_dsi))
		return;

	/* Do nothing with the host if it is slave-DSI in case of bonded DSI */
	if (is_bonded_dsi && !IS_MASTER_DSI_LINK(id))
		return;

	if (panel) {
		ret = drm_panel_enable(panel);
		if (ret) {
			pr_err("%s: enable panel %d failed, %d\n", __func__, id,
									ret);
		}
	}
>>>>>>> upstream/android-13
}

static void dsi_mgr_bridge_disable(struct drm_bridge *bridge)
{
<<<<<<< HEAD
	DBG("");
=======
	int id = dsi_mgr_bridge_get_id(bridge);
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct drm_panel *panel = msm_dsi->panel;
	bool is_bonded_dsi = IS_BONDED_DSI();
	int ret;

	DBG("id=%d", id);
	if (!msm_dsi_device_connected(msm_dsi))
		return;

	/* Do nothing with the host if it is slave-DSI in case of bonded DSI */
	if (is_bonded_dsi && !IS_MASTER_DSI_LINK(id))
		return;

	if (panel) {
		ret = drm_panel_disable(panel);
		if (ret)
			pr_err("%s: Panel %d OFF failed, %d\n", __func__, id,
									ret);
	}
>>>>>>> upstream/android-13
}

static void dsi_mgr_bridge_post_disable(struct drm_bridge *bridge)
{
	int id = dsi_mgr_bridge_get_id(bridge);
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct msm_dsi *msm_dsi1 = dsi_mgr_get_dsi(DSI_1);
	struct mipi_dsi_host *host = msm_dsi->host;
	struct drm_panel *panel = msm_dsi->panel;
<<<<<<< HEAD
	struct msm_dsi_pll *src_pll;
	bool is_dual_dsi = IS_DUAL_DSI();
=======
	bool is_bonded_dsi = IS_BONDED_DSI();
>>>>>>> upstream/android-13
	int ret;

	DBG("id=%d", id);

	if (!msm_dsi_device_connected(msm_dsi))
		return;

	/*
<<<<<<< HEAD
	 * Do nothing with the host if it is slave-DSI in case of dual DSI.
	 * It is safe to call dsi_mgr_phy_disable() here because a single PHY
	 * won't be diabled until both PHYs request disable.
	 */
	if (is_dual_dsi && !IS_MASTER_DSI_LINK(id))
		goto disable_phy;

	if (panel) {
		ret = drm_panel_disable(panel);
		if (ret)
			pr_err("%s: Panel %d OFF failed, %d\n", __func__, id,
									ret);
	}

=======
	 * Do nothing with the host if it is slave-DSI in case of bonded DSI.
	 * It is safe to call dsi_mgr_phy_disable() here because a single PHY
	 * won't be diabled until both PHYs request disable.
	 */
	if (is_bonded_dsi && !IS_MASTER_DSI_LINK(id))
		goto disable_phy;

>>>>>>> upstream/android-13
	ret = msm_dsi_host_disable(host);
	if (ret)
		pr_err("%s: host %d disable failed, %d\n", __func__, id, ret);

<<<<<<< HEAD
	if (is_dual_dsi && msm_dsi1) {
=======
	if (is_bonded_dsi && msm_dsi1) {
>>>>>>> upstream/android-13
		ret = msm_dsi_host_disable(msm_dsi1->host);
		if (ret)
			pr_err("%s: host1 disable failed, %d\n", __func__, ret);
	}

	if (panel) {
		ret = drm_panel_unprepare(panel);
		if (ret)
			pr_err("%s: Panel %d unprepare failed,%d\n", __func__,
								id, ret);
	}

<<<<<<< HEAD
	/* Save PLL status if it is a clock source */
	src_pll = msm_dsi_phy_get_pll(msm_dsi->phy);
	msm_dsi_pll_save_state(src_pll);
=======
	msm_dsi_host_disable_irq(host);
	if (is_bonded_dsi && msm_dsi1)
		msm_dsi_host_disable_irq(msm_dsi1->host);

	/* Save PHY status if it is a clock source */
	msm_dsi_phy_pll_save_state(msm_dsi->phy);
>>>>>>> upstream/android-13

	ret = msm_dsi_host_power_off(host);
	if (ret)
		pr_err("%s: host %d power off failed,%d\n", __func__, id, ret);

<<<<<<< HEAD
	if (is_dual_dsi && msm_dsi1) {
=======
	if (is_bonded_dsi && msm_dsi1) {
>>>>>>> upstream/android-13
		ret = msm_dsi_host_power_off(msm_dsi1->host);
		if (ret)
			pr_err("%s: host1 power off failed, %d\n",
								__func__, ret);
	}

disable_phy:
	dsi_mgr_phy_disable(id);
}

static void dsi_mgr_bridge_mode_set(struct drm_bridge *bridge,
<<<<<<< HEAD
		struct drm_display_mode *mode,
		struct drm_display_mode *adjusted_mode)
=======
		const struct drm_display_mode *mode,
		const struct drm_display_mode *adjusted_mode)
>>>>>>> upstream/android-13
{
	int id = dsi_mgr_bridge_get_id(bridge);
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct msm_dsi *other_dsi = dsi_mgr_get_other_dsi(id);
	struct mipi_dsi_host *host = msm_dsi->host;
<<<<<<< HEAD
	bool is_dual_dsi = IS_DUAL_DSI();

	DBG("set mode: %d:\"%s\" %d %d %d %d %d %d %d %d %d %d 0x%x 0x%x",
			mode->base.id, mode->name,
			mode->vrefresh, mode->clock,
			mode->hdisplay, mode->hsync_start,
			mode->hsync_end, mode->htotal,
			mode->vdisplay, mode->vsync_start,
			mode->vsync_end, mode->vtotal,
			mode->type, mode->flags);

	if (is_dual_dsi && !IS_MASTER_DSI_LINK(id))
		return;

	msm_dsi_host_set_display_mode(host, adjusted_mode);
	if (is_dual_dsi && other_dsi)
=======
	bool is_bonded_dsi = IS_BONDED_DSI();

	DBG("set mode: " DRM_MODE_FMT, DRM_MODE_ARG(mode));

	if (is_bonded_dsi && !IS_MASTER_DSI_LINK(id))
		return;

	msm_dsi_host_set_display_mode(host, adjusted_mode);
	if (is_bonded_dsi && other_dsi)
>>>>>>> upstream/android-13
		msm_dsi_host_set_display_mode(other_dsi->host, adjusted_mode);
}

static const struct drm_connector_funcs dsi_mgr_connector_funcs = {
	.detect = dsi_mgr_connector_detect,
	.fill_modes = drm_helper_probe_single_connector_modes,
	.destroy = dsi_mgr_connector_destroy,
	.reset = drm_atomic_helper_connector_reset,
	.atomic_duplicate_state = drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_connector_destroy_state,
};

static const struct drm_connector_helper_funcs dsi_mgr_conn_helper_funcs = {
	.get_modes = dsi_mgr_connector_get_modes,
	.mode_valid = dsi_mgr_connector_mode_valid,
	.best_encoder = dsi_mgr_connector_best_encoder,
};

static const struct drm_bridge_funcs dsi_mgr_bridge_funcs = {
	.pre_enable = dsi_mgr_bridge_pre_enable,
	.enable = dsi_mgr_bridge_enable,
	.disable = dsi_mgr_bridge_disable,
	.post_disable = dsi_mgr_bridge_post_disable,
	.mode_set = dsi_mgr_bridge_mode_set,
};

/* initialize connector when we're connected to a drm_panel */
struct drm_connector *msm_dsi_manager_connector_init(u8 id)
{
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct drm_connector *connector = NULL;
	struct dsi_connector *dsi_connector;
	int ret;

	dsi_connector = kzalloc(sizeof(*dsi_connector), GFP_KERNEL);
	if (!dsi_connector)
		return ERR_PTR(-ENOMEM);

	dsi_connector->id = id;

	connector = &dsi_connector->base;

	ret = drm_connector_init(msm_dsi->dev, connector,
			&dsi_mgr_connector_funcs, DRM_MODE_CONNECTOR_DSI);
	if (ret)
		return ERR_PTR(ret);

	drm_connector_helper_add(connector, &dsi_mgr_conn_helper_funcs);

	/* Enable HPD to let hpd event is handled
	 * when panel is attached to the host.
	 */
	connector->polled = DRM_CONNECTOR_POLL_HPD;

	/* Display driver doesn't support interlace now. */
	connector->interlace_allowed = 0;
	connector->doublescan_allowed = 0;

	drm_connector_attach_encoder(connector, msm_dsi->encoder);

<<<<<<< HEAD
	return connector;
}

bool msm_dsi_manager_validate_current_config(u8 id)
{
	bool is_dual_dsi = IS_DUAL_DSI();

	/*
	 * For dual DSI, we only have one drm panel. For this
	 * use case, we register only one bridge/connector.
	 * Skip bridge/connector initialisation if it is
	 * slave-DSI for dual DSI configuration.
	 */
	if (is_dual_dsi && !IS_MASTER_DSI_LINK(id)) {
		DBG("Skip bridge registration for slave DSI->id: %d\n", id);
		return false;
	}
	return true;
=======
	ret = msm_dsi_manager_panel_init(connector, id);
	if (ret) {
		DRM_DEV_ERROR(msm_dsi->dev->dev, "init panel failed %d\n", ret);
		goto fail;
	}

	return connector;

fail:
	connector->funcs->destroy(connector);
	return ERR_PTR(ret);
>>>>>>> upstream/android-13
}

/* initialize bridge */
struct drm_bridge *msm_dsi_manager_bridge_init(u8 id)
{
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct drm_bridge *bridge = NULL;
	struct dsi_bridge *dsi_bridge;
	struct drm_encoder *encoder;
	int ret;

	dsi_bridge = devm_kzalloc(msm_dsi->dev->dev,
				sizeof(*dsi_bridge), GFP_KERNEL);
	if (!dsi_bridge) {
		ret = -ENOMEM;
		goto fail;
	}

	dsi_bridge->id = id;

	encoder = msm_dsi->encoder;

	bridge = &dsi_bridge->base;
	bridge->funcs = &dsi_mgr_bridge_funcs;

<<<<<<< HEAD
	ret = drm_bridge_attach(encoder, bridge, NULL);
=======
	ret = drm_bridge_attach(encoder, bridge, NULL, 0);
>>>>>>> upstream/android-13
	if (ret)
		goto fail;

	return bridge;

fail:
	if (bridge)
		msm_dsi_manager_bridge_destroy(bridge);

	return ERR_PTR(ret);
}

struct drm_connector *msm_dsi_manager_ext_bridge_init(u8 id)
{
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct drm_device *dev = msm_dsi->dev;
	struct drm_encoder *encoder;
	struct drm_bridge *int_bridge, *ext_bridge;
	struct drm_connector *connector;
	struct list_head *connector_list;

	int_bridge = msm_dsi->bridge;
	ext_bridge = msm_dsi->external_bridge =
			msm_dsi_host_get_bridge(msm_dsi->host);

	encoder = msm_dsi->encoder;

	/* link the internal dsi bridge to the external bridge */
<<<<<<< HEAD
	drm_bridge_attach(encoder, ext_bridge, int_bridge);
=======
	drm_bridge_attach(encoder, ext_bridge, int_bridge, 0);
>>>>>>> upstream/android-13

	/*
	 * we need the drm_connector created by the external bridge
	 * driver (or someone else) to feed it to our driver's
	 * priv->connector[] list, mainly for msm_fbdev_init()
	 */
	connector_list = &dev->mode_config.connector_list;

	list_for_each_entry(connector, connector_list, head) {
		if (drm_connector_has_possible_encoder(connector, encoder))
			return connector;
	}

	return ERR_PTR(-ENODEV);
}

void msm_dsi_manager_bridge_destroy(struct drm_bridge *bridge)
{
}

int msm_dsi_manager_cmd_xfer(int id, const struct mipi_dsi_msg *msg)
{
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct msm_dsi *msm_dsi0 = dsi_mgr_get_dsi(DSI_0);
	struct mipi_dsi_host *host = msm_dsi->host;
	bool is_read = (msg->rx_buf && msg->rx_len);
	bool need_sync = (IS_SYNC_NEEDED() && !is_read);
	int ret;

	if (!msg->tx_buf || !msg->tx_len)
		return 0;

<<<<<<< HEAD
	/* In dual master case, panel requires the same commands sent to
=======
	/* In bonded master case, panel requires the same commands sent to
>>>>>>> upstream/android-13
	 * both DSI links. Host issues the command trigger to both links
	 * when DSI_1 calls the cmd transfer function, no matter it happens
	 * before or after DSI_0 cmd transfer.
	 */
	if (need_sync && (id == DSI_0))
		return is_read ? msg->rx_len : msg->tx_len;

	if (need_sync && msm_dsi0) {
		ret = msm_dsi_host_xfer_prepare(msm_dsi0->host, msg);
		if (ret) {
			pr_err("%s: failed to prepare non-trigger host, %d\n",
				__func__, ret);
			return ret;
		}
	}
	ret = msm_dsi_host_xfer_prepare(host, msg);
	if (ret) {
		pr_err("%s: failed to prepare host, %d\n", __func__, ret);
		goto restore_host0;
	}

	ret = is_read ? msm_dsi_host_cmd_rx(host, msg) :
			msm_dsi_host_cmd_tx(host, msg);

	msm_dsi_host_xfer_restore(host, msg);

restore_host0:
	if (need_sync && msm_dsi0)
		msm_dsi_host_xfer_restore(msm_dsi0->host, msg);

	return ret;
}

bool msm_dsi_manager_cmd_xfer_trigger(int id, u32 dma_base, u32 len)
{
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct msm_dsi *msm_dsi0 = dsi_mgr_get_dsi(DSI_0);
	struct mipi_dsi_host *host = msm_dsi->host;

	if (IS_SYNC_NEEDED() && (id == DSI_0))
		return false;

	if (IS_SYNC_NEEDED() && msm_dsi0)
		msm_dsi_host_cmd_xfer_commit(msm_dsi0->host, dma_base, len);

	msm_dsi_host_cmd_xfer_commit(host, dma_base, len);

	return true;
}

<<<<<<< HEAD
void msm_dsi_manager_attach_dsi_device(int id, u32 device_flags)
{
	struct msm_dsi *msm_dsi = dsi_mgr_get_dsi(id);
	struct drm_device *dev = msm_dsi->dev;
	struct msm_drm_private *priv;
	struct msm_kms *kms;
	struct drm_encoder *encoder;
	bool cmd_mode;

	/*
	 * drm_device pointer is assigned to msm_dsi only in the modeset_init
	 * path. If mipi_dsi_attach() happens in DSI driver's probe path
	 * (generally the case when we're connected to a drm_panel of the type
	 * mipi_dsi_device), this would be NULL. In such cases, try to set the
	 * encoder mode in the DSI connector's detect() op.
	 */
	if (!dev)
		return;

	priv = dev->dev_private;
	kms = priv->kms;
	encoder = msm_dsi_get_encoder(msm_dsi);
	cmd_mode = !(device_flags &
				 MIPI_DSI_MODE_VIDEO);

	if (encoder && kms->funcs->set_encoder_mode)
		kms->funcs->set_encoder_mode(kms, encoder, cmd_mode);
}

=======
>>>>>>> upstream/android-13
int msm_dsi_manager_register(struct msm_dsi *msm_dsi)
{
	struct msm_dsi_manager *msm_dsim = &msm_dsim_glb;
	int id = msm_dsi->id;
	int ret;

	if (id >= DSI_MAX) {
		pr_err("%s: invalid id %d\n", __func__, id);
		return -EINVAL;
	}

	if (msm_dsim->dsi[id]) {
		pr_err("%s: dsi%d already registered\n", __func__, id);
		return -EBUSY;
	}

	msm_dsim->dsi[id] = msm_dsi;

<<<<<<< HEAD
	ret = dsi_mgr_parse_dual_dsi(msm_dsi->pdev->dev.of_node, id);
	if (ret) {
		pr_err("%s: failed to parse dual DSI info\n", __func__);
=======
	ret = dsi_mgr_parse_of(msm_dsi->pdev->dev.of_node, id);
	if (ret) {
		pr_err("%s: failed to parse OF DSI info\n", __func__);
>>>>>>> upstream/android-13
		goto fail;
	}

	ret = dsi_mgr_setup_components(id);
	if (ret) {
<<<<<<< HEAD
		pr_err("%s: failed to register mipi dsi host for DSI %d\n",
			__func__, id);
=======
		pr_err("%s: failed to register mipi dsi host for DSI %d: %d\n",
			__func__, id, ret);
>>>>>>> upstream/android-13
		goto fail;
	}

	return 0;

fail:
	msm_dsim->dsi[id] = NULL;
	return ret;
}

void msm_dsi_manager_unregister(struct msm_dsi *msm_dsi)
{
	struct msm_dsi_manager *msm_dsim = &msm_dsim_glb;

	if (msm_dsi->host)
		msm_dsi_host_unregister(msm_dsi->host);
<<<<<<< HEAD
	msm_dsim->dsi[msm_dsi->id] = NULL;
}

=======

	if (msm_dsi->id >= 0)
		msm_dsim->dsi[msm_dsi->id] = NULL;
}

bool msm_dsi_is_bonded_dsi(struct msm_dsi *msm_dsi)
{
	return IS_BONDED_DSI();
}

bool msm_dsi_is_master_dsi(struct msm_dsi *msm_dsi)
{
	return IS_MASTER_DSI_LINK(msm_dsi->id);
}
>>>>>>> upstream/android-13
