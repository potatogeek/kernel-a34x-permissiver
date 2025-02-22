// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 * Copyright (C) 2017-2018 Bootlin
 *
 * Maxime Ripard <maxime.ripard@bootlin.com>
 */

#ifndef _SUN6I_MIPI_DSI_H_
#define _SUN6I_MIPI_DSI_H_

#include <drm/drm_connector.h>
#include <drm/drm_encoder.h>
#include <drm/drm_mipi_dsi.h>

<<<<<<< HEAD
struct sun6i_dphy {
	struct clk		*bus_clk;
	struct clk		*mod_clk;
	struct regmap		*regs;
	struct reset_control	*reset;
};
=======
#define SUN6I_DSI_TCON_DIV	4
>>>>>>> upstream/android-13

struct sun6i_dsi {
	struct drm_connector	connector;
	struct drm_encoder	encoder;
	struct mipi_dsi_host	host;

	struct clk		*bus_clk;
	struct clk		*mod_clk;
	struct regmap		*regs;
<<<<<<< HEAD
	struct reset_control	*reset;
	struct sun6i_dphy	*dphy;

	struct device		*dev;
	struct sun4i_drv	*drv;
	struct mipi_dsi_device	*device;
=======
	struct regulator	*regulator;
	struct reset_control	*reset;
	struct phy		*dphy;

	struct device		*dev;
	struct mipi_dsi_device	*device;
	struct drm_device	*drm;
>>>>>>> upstream/android-13
	struct drm_panel	*panel;
};

static inline struct sun6i_dsi *host_to_sun6i_dsi(struct mipi_dsi_host *host)
{
	return container_of(host, struct sun6i_dsi, host);
};

static inline struct sun6i_dsi *connector_to_sun6i_dsi(struct drm_connector *connector)
{
	return container_of(connector, struct sun6i_dsi, connector);
};

static inline struct sun6i_dsi *encoder_to_sun6i_dsi(const struct drm_encoder *encoder)
{
	return container_of(encoder, struct sun6i_dsi, encoder);
};

<<<<<<< HEAD
int sun6i_dphy_probe(struct sun6i_dsi *dsi, struct device_node *node);
int sun6i_dphy_remove(struct sun6i_dsi *dsi);

int sun6i_dphy_init(struct sun6i_dphy *dphy, unsigned int lanes);
int sun6i_dphy_power_on(struct sun6i_dphy *dphy, unsigned int lanes);
int sun6i_dphy_power_off(struct sun6i_dphy *dphy);
int sun6i_dphy_exit(struct sun6i_dphy *dphy);

=======
>>>>>>> upstream/android-13
#endif /* _SUN6I_MIPI_DSI_H_ */
