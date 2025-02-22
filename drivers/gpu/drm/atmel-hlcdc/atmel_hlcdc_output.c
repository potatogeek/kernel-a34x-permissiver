<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2014 Traphandler
 * Copyright (C) 2014 Free Electrons
 * Copyright (C) 2014 Atmel
 *
 * Author: Jean-Jacques Hiblot <jjhiblot@traphandler.com>
 * Author: Boris BREZILLON <boris.brezillon@free-electrons.com>
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
 */

#include <linux/of_graph.h>

#include <drm/drmP.h>
#include <drm/drm_of.h>
#include <drm/drm_bridge.h>

#include "atmel_hlcdc_dc.h"

static const struct drm_encoder_funcs atmel_hlcdc_panel_encoder_funcs = {
	.destroy = drm_encoder_cleanup,
};

static int atmel_hlcdc_attach_endpoint(struct drm_device *dev, int endpoint)
{
	struct drm_encoder *encoder;
=======
 */

#include <linux/media-bus-format.h>
#include <linux/of_graph.h>

#include <drm/drm_bridge.h>
#include <drm/drm_encoder.h>
#include <drm/drm_of.h>
#include <drm/drm_simple_kms_helper.h>

#include "atmel_hlcdc_dc.h"

struct atmel_hlcdc_rgb_output {
	struct drm_encoder encoder;
	int bus_fmt;
};

static struct atmel_hlcdc_rgb_output *
atmel_hlcdc_encoder_to_rgb_output(struct drm_encoder *encoder)
{
	return container_of(encoder, struct atmel_hlcdc_rgb_output, encoder);
}

int atmel_hlcdc_encoder_get_bus_fmt(struct drm_encoder *encoder)
{
	struct atmel_hlcdc_rgb_output *output;

	output = atmel_hlcdc_encoder_to_rgb_output(encoder);

	return output->bus_fmt;
}

static int atmel_hlcdc_of_bus_fmt(const struct device_node *ep)
{
	u32 bus_width;
	int ret;

	ret = of_property_read_u32(ep, "bus-width", &bus_width);
	if (ret == -EINVAL)
		return 0;
	if (ret)
		return ret;

	switch (bus_width) {
	case 12:
		return MEDIA_BUS_FMT_RGB444_1X12;
	case 16:
		return MEDIA_BUS_FMT_RGB565_1X16;
	case 18:
		return MEDIA_BUS_FMT_RGB666_1X18;
	case 24:
		return MEDIA_BUS_FMT_RGB888_1X24;
	default:
		return -EINVAL;
	}
}

static int atmel_hlcdc_attach_endpoint(struct drm_device *dev, int endpoint)
{
	struct atmel_hlcdc_rgb_output *output;
	struct device_node *ep;
>>>>>>> upstream/android-13
	struct drm_panel *panel;
	struct drm_bridge *bridge;
	int ret;

<<<<<<< HEAD
	ret = drm_of_find_panel_or_bridge(dev->dev->of_node, 0, endpoint,
					  &panel, &bridge);
	if (ret)
		return ret;

	encoder = devm_kzalloc(dev->dev, sizeof(*encoder), GFP_KERNEL);
	if (!encoder)
		return -EINVAL;

	ret = drm_encoder_init(dev, encoder,
			       &atmel_hlcdc_panel_encoder_funcs,
			       DRM_MODE_ENCODER_NONE, NULL);
	if (ret)
		return ret;

	encoder->possible_crtcs = 0x1;

	if (panel) {
		bridge = drm_panel_bridge_add(panel, DRM_MODE_CONNECTOR_Unknown);
=======
	ep = of_graph_get_endpoint_by_regs(dev->dev->of_node, 0, endpoint);
	if (!ep)
		return -ENODEV;

	ret = drm_of_find_panel_or_bridge(dev->dev->of_node, 0, endpoint,
					  &panel, &bridge);
	if (ret) {
		of_node_put(ep);
		return ret;
	}

	output = devm_kzalloc(dev->dev, sizeof(*output), GFP_KERNEL);
	if (!output) {
		of_node_put(ep);
		return -ENOMEM;
	}

	output->bus_fmt = atmel_hlcdc_of_bus_fmt(ep);
	of_node_put(ep);
	if (output->bus_fmt < 0) {
		dev_err(dev->dev, "endpoint %d: invalid bus width\n", endpoint);
		return -EINVAL;
	}

	ret = drm_simple_encoder_init(dev, &output->encoder,
				      DRM_MODE_ENCODER_NONE);
	if (ret)
		return ret;

	output->encoder.possible_crtcs = 0x1;

	if (panel) {
		bridge = drm_panel_bridge_add_typed(panel,
						    DRM_MODE_CONNECTOR_Unknown);
>>>>>>> upstream/android-13
		if (IS_ERR(bridge))
			return PTR_ERR(bridge);
	}

	if (bridge) {
<<<<<<< HEAD
		ret = drm_bridge_attach(encoder, bridge, NULL);
=======
		ret = drm_bridge_attach(&output->encoder, bridge, NULL, 0);
>>>>>>> upstream/android-13
		if (!ret)
			return 0;

		if (panel)
			drm_panel_bridge_remove(bridge);
	}

<<<<<<< HEAD
	drm_encoder_cleanup(encoder);
=======
	drm_encoder_cleanup(&output->encoder);
>>>>>>> upstream/android-13

	return ret;
}

int atmel_hlcdc_create_outputs(struct drm_device *dev)
{
	int endpoint, ret = 0;
<<<<<<< HEAD

	for (endpoint = 0; !ret; endpoint++)
		ret = atmel_hlcdc_attach_endpoint(dev, endpoint);

	/* At least one device was successfully attached.*/
	if (ret == -ENODEV && endpoint)
=======
	int attached = 0;

	/*
	 * Always scan the first few endpoints even if we get -ENODEV,
	 * but keep going after that as long as we keep getting hits.
	 */
	for (endpoint = 0; !ret || endpoint < 4; endpoint++) {
		ret = atmel_hlcdc_attach_endpoint(dev, endpoint);
		if (ret == -ENODEV)
			continue;
		if (ret)
			break;
		attached++;
	}

	/* At least one device was successfully attached.*/
	if (ret == -ENODEV && attached)
>>>>>>> upstream/android-13
		return 0;

	return ret;
}
