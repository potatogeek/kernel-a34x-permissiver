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
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2011 Texas Instruments Incorporated - https://www.ti.com/
 * Author: Rob Clark <rob@ti.com>
>>>>>>> upstream/android-13
 */

#include <linux/list.h>

<<<<<<< HEAD
#include <drm/drm_crtc.h>
#include <drm/drm_crtc_helper.h>
=======
#include <drm/drm_bridge.h>
#include <drm/drm_crtc.h>
#include <drm/drm_modeset_helper_vtables.h>
>>>>>>> upstream/android-13
#include <drm/drm_edid.h>

#include "omap_drv.h"

/*
 * encoder funcs
 */

#define to_omap_encoder(x) container_of(x, struct omap_encoder, base)

/* The encoder and connector both map to same dssdev.. the encoder
 * handles the 'active' parts, ie. anything the modifies the state
 * of the hw, and the connector handles the 'read-only' parts, like
 * detecting connection and reading edid.
 */
struct omap_encoder {
	struct drm_encoder base;
<<<<<<< HEAD
	struct omap_dss_device *dssdev;
};

struct omap_dss_device *omap_encoder_get_dssdev(struct drm_encoder *encoder)
{
	struct omap_encoder *omap_encoder = to_omap_encoder(encoder);

	return omap_encoder->dssdev;
}

=======
	struct omap_dss_device *output;
};

>>>>>>> upstream/android-13
static void omap_encoder_destroy(struct drm_encoder *encoder)
{
	struct omap_encoder *omap_encoder = to_omap_encoder(encoder);

	drm_encoder_cleanup(encoder);
	kfree(omap_encoder);
}

static const struct drm_encoder_funcs omap_encoder_funcs = {
	.destroy = omap_encoder_destroy,
};

<<<<<<< HEAD
static void omap_encoder_mode_set(struct drm_encoder *encoder,
				struct drm_display_mode *mode,
				struct drm_display_mode *adjusted_mode)
{
	struct drm_device *dev = encoder->dev;
	struct omap_encoder *omap_encoder = to_omap_encoder(encoder);
	struct omap_dss_device *dssdev = omap_encoder->dssdev;
	struct drm_connector *connector;
	bool hdmi_mode;
	int r;

	hdmi_mode = false;
	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		if (connector->encoder == encoder) {
			hdmi_mode = omap_connector_get_hdmi_mode(connector);
			break;
		}
	}

	if (dssdev->driver->set_hdmi_mode)
		dssdev->driver->set_hdmi_mode(dssdev, hdmi_mode);

	if (hdmi_mode && dssdev->driver->set_hdmi_infoframe) {
		struct hdmi_avi_infoframe avi;

		r = drm_hdmi_avi_infoframe_from_display_mode(&avi, adjusted_mode,
							     false);
		if (r == 0)
			dssdev->driver->set_hdmi_infoframe(dssdev, &avi);
	}
}

static void omap_encoder_disable(struct drm_encoder *encoder)
{
	struct omap_encoder *omap_encoder = to_omap_encoder(encoder);
	struct omap_dss_device *dssdev = omap_encoder->dssdev;
	struct omap_dss_driver *dssdrv = dssdev->driver;

	dssdrv->disable(dssdev);
}

static int omap_encoder_update(struct drm_encoder *encoder,
			       enum omap_channel channel,
			       struct videomode *vm)
{
	struct drm_device *dev = encoder->dev;
	struct omap_encoder *omap_encoder = to_omap_encoder(encoder);
	struct omap_dss_device *dssdev = omap_encoder->dssdev;
	struct omap_dss_driver *dssdrv = dssdev->driver;
	int ret;

	if (dssdrv->check_timings) {
		ret = dssdrv->check_timings(dssdev, vm);
	} else {
		struct videomode t = {0};

		dssdrv->get_timings(dssdev, &t);

		if (memcmp(vm, &t, sizeof(*vm)))
			ret = -EINVAL;
		else
			ret = 0;
	}

	if (ret) {
		dev_err(dev->dev, "could not set timings: %d\n", ret);
		return ret;
	}

	if (dssdrv->set_timings)
		dssdrv->set_timings(dssdev, vm);

	return 0;
}

static void omap_encoder_enable(struct drm_encoder *encoder)
{
	struct omap_encoder *omap_encoder = to_omap_encoder(encoder);
	struct omap_dss_device *dssdev = omap_encoder->dssdev;
	struct omap_dss_driver *dssdrv = dssdev->driver;
	int r;

	omap_encoder_update(encoder, omap_crtc_channel(encoder->crtc),
			    omap_crtc_timings(encoder->crtc));

	r = dssdrv->enable(dssdev);
	if (r)
		dev_err(encoder->dev->dev,
			"Failed to enable display '%s': %d\n",
			dssdev->name, r);
}

static int omap_encoder_atomic_check(struct drm_encoder *encoder,
				     struct drm_crtc_state *crtc_state,
				     struct drm_connector_state *conn_state)
{
	return 0;
=======
static void omap_encoder_update_videomode_flags(struct videomode *vm,
						u32 bus_flags)
{
	if (!(vm->flags & (DISPLAY_FLAGS_DE_LOW |
			   DISPLAY_FLAGS_DE_HIGH))) {
		if (bus_flags & DRM_BUS_FLAG_DE_LOW)
			vm->flags |= DISPLAY_FLAGS_DE_LOW;
		else if (bus_flags & DRM_BUS_FLAG_DE_HIGH)
			vm->flags |= DISPLAY_FLAGS_DE_HIGH;
	}

	if (!(vm->flags & (DISPLAY_FLAGS_PIXDATA_POSEDGE |
			   DISPLAY_FLAGS_PIXDATA_NEGEDGE))) {
		if (bus_flags & DRM_BUS_FLAG_PIXDATA_DRIVE_POSEDGE)
			vm->flags |= DISPLAY_FLAGS_PIXDATA_POSEDGE;
		else if (bus_flags & DRM_BUS_FLAG_PIXDATA_DRIVE_NEGEDGE)
			vm->flags |= DISPLAY_FLAGS_PIXDATA_NEGEDGE;
	}

	if (!(vm->flags & (DISPLAY_FLAGS_SYNC_POSEDGE |
			   DISPLAY_FLAGS_SYNC_NEGEDGE))) {
		if (bus_flags & DRM_BUS_FLAG_SYNC_DRIVE_POSEDGE)
			vm->flags |= DISPLAY_FLAGS_SYNC_POSEDGE;
		else if (bus_flags & DRM_BUS_FLAG_SYNC_DRIVE_NEGEDGE)
			vm->flags |= DISPLAY_FLAGS_SYNC_NEGEDGE;
	}
}

static void omap_encoder_mode_set(struct drm_encoder *encoder,
				  struct drm_display_mode *mode,
				  struct drm_display_mode *adjusted_mode)
{
	struct omap_encoder *omap_encoder = to_omap_encoder(encoder);
	struct omap_dss_device *output = omap_encoder->output;
	struct drm_device *dev = encoder->dev;
	struct drm_connector *connector;
	struct drm_bridge *bridge;
	struct videomode vm = { 0 };
	u32 bus_flags;

	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		if (connector->encoder == encoder)
			break;
	}

	drm_display_mode_to_videomode(adjusted_mode, &vm);

	/*
	 * HACK: This fixes the vm flags.
	 * struct drm_display_mode does not contain the VSYNC/HSYNC/DE flags and
	 * they get lost when converting back and forth between struct
	 * drm_display_mode and struct videomode. The hack below goes and
	 * fetches the missing flags.
	 *
	 * A better solution is to use DRM's bus-flags through the whole driver.
	 */
	for (bridge = output->bridge; bridge;
	     bridge = drm_bridge_get_next_bridge(bridge)) {
		if (!bridge->timings)
			continue;

		bus_flags = bridge->timings->input_bus_flags;
		omap_encoder_update_videomode_flags(&vm, bus_flags);
	}

	bus_flags = connector->display_info.bus_flags;
	omap_encoder_update_videomode_flags(&vm, bus_flags);

	/* Set timings for all devices in the display pipeline. */
	dss_mgr_set_timings(output, &vm);
>>>>>>> upstream/android-13
}

static const struct drm_encoder_helper_funcs omap_encoder_helper_funcs = {
	.mode_set = omap_encoder_mode_set,
<<<<<<< HEAD
	.disable = omap_encoder_disable,
	.enable = omap_encoder_enable,
	.atomic_check = omap_encoder_atomic_check,
=======
>>>>>>> upstream/android-13
};

/* initialize encoder */
struct drm_encoder *omap_encoder_init(struct drm_device *dev,
<<<<<<< HEAD
		struct omap_dss_device *dssdev)
=======
				      struct omap_dss_device *output)
>>>>>>> upstream/android-13
{
	struct drm_encoder *encoder = NULL;
	struct omap_encoder *omap_encoder;

	omap_encoder = kzalloc(sizeof(*omap_encoder), GFP_KERNEL);
	if (!omap_encoder)
		goto fail;

<<<<<<< HEAD
	omap_encoder->dssdev = dssdev;
=======
	omap_encoder->output = output;
>>>>>>> upstream/android-13

	encoder = &omap_encoder->base;

	drm_encoder_init(dev, encoder, &omap_encoder_funcs,
			 DRM_MODE_ENCODER_TMDS, NULL);
	drm_encoder_helper_add(encoder, &omap_encoder_helper_funcs);

	return encoder;

fail:
	if (encoder)
		omap_encoder_destroy(encoder);

	return NULL;
}
