<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (C) STMicroelectronics SA 2017
 *
 * Authors: Philippe Cornu <philippe.cornu@st.com>
 *          Yannick Fertre <yannick.fertre@st.com>
<<<<<<< HEAD
 *
 * License terms:  GNU General Public License (GPL), version 2
=======
>>>>>>> upstream/android-13
 */

#ifndef __DW_MIPI_DSI__
#define __DW_MIPI_DSI__

<<<<<<< HEAD
struct dw_mipi_dsi;

struct dw_mipi_dsi_phy_ops {
	int (*init)(void *priv_data);
	int (*get_lane_mbps)(void *priv_data, struct drm_display_mode *mode,
			     unsigned long mode_flags, u32 lanes, u32 format,
			     unsigned int *lane_mbps);
=======
#include <linux/types.h>

#include <drm/drm_modes.h>

struct drm_display_mode;
struct drm_encoder;
struct dw_mipi_dsi;
struct mipi_dsi_device;
struct platform_device;

struct dw_mipi_dsi_dphy_timing {
	u16 data_hs2lp;
	u16 data_lp2hs;
	u16 clk_hs2lp;
	u16 clk_lp2hs;
};

struct dw_mipi_dsi_phy_ops {
	int (*init)(void *priv_data);
	void (*power_on)(void *priv_data);
	void (*power_off)(void *priv_data);
	int (*get_lane_mbps)(void *priv_data,
			     const struct drm_display_mode *mode,
			     unsigned long mode_flags, u32 lanes, u32 format,
			     unsigned int *lane_mbps);
	int (*get_timing)(void *priv_data, unsigned int lane_mbps,
			  struct dw_mipi_dsi_dphy_timing *timing);
	int (*get_esc_clk_rate)(void *priv_data, unsigned int *esc_clk_rate);
};

struct dw_mipi_dsi_host_ops {
	int (*attach)(void *priv_data,
		      struct mipi_dsi_device *dsi);
	int (*detach)(void *priv_data,
		      struct mipi_dsi_device *dsi);
>>>>>>> upstream/android-13
};

struct dw_mipi_dsi_plat_data {
	void __iomem *base;
	unsigned int max_data_lanes;

	enum drm_mode_status (*mode_valid)(void *priv_data,
					   const struct drm_display_mode *mode);

	const struct dw_mipi_dsi_phy_ops *phy_ops;
<<<<<<< HEAD
=======
	const struct dw_mipi_dsi_host_ops *host_ops;
>>>>>>> upstream/android-13

	void *priv_data;
};

struct dw_mipi_dsi *dw_mipi_dsi_probe(struct platform_device *pdev,
				      const struct dw_mipi_dsi_plat_data
				      *plat_data);
void dw_mipi_dsi_remove(struct dw_mipi_dsi *dsi);
<<<<<<< HEAD
struct dw_mipi_dsi *dw_mipi_dsi_bind(struct platform_device *pdev,
				     struct drm_encoder *encoder,
				     const struct dw_mipi_dsi_plat_data
				     *plat_data);
void dw_mipi_dsi_unbind(struct dw_mipi_dsi *dsi);
=======
int dw_mipi_dsi_bind(struct dw_mipi_dsi *dsi, struct drm_encoder *encoder);
void dw_mipi_dsi_unbind(struct dw_mipi_dsi *dsi);
void dw_mipi_dsi_set_slave(struct dw_mipi_dsi *dsi, struct dw_mipi_dsi *slave);
>>>>>>> upstream/android-13

#endif /* __DW_MIPI_DSI__ */
