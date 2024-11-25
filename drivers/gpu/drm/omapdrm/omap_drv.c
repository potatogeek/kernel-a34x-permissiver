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

=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2011 Texas Instruments Incorporated - https://www.ti.com/
 * Author: Rob Clark <rob@ti.com>
 */

#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/sort.h>
>>>>>>> upstream/android-13
#include <linux/sys_soc.h>

#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
<<<<<<< HEAD
#include <drm/drm_crtc_helper.h>
#include <drm/drm_fb_helper.h>
=======
#include <drm/drm_bridge.h>
#include <drm/drm_bridge_connector.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_file.h>
#include <drm/drm_ioctl.h>
#include <drm/drm_panel.h>
#include <drm/drm_prime.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_vblank.h>
>>>>>>> upstream/android-13

#include "omap_dmm_tiler.h"
#include "omap_drv.h"

#define DRIVER_NAME		MODULE_NAME
#define DRIVER_DESC		"OMAP DRM"
#define DRIVER_DATE		"20110917"
#define DRIVER_MAJOR		1
#define DRIVER_MINOR		0
#define DRIVER_PATCHLEVEL	0

/*
 * mode config funcs
 */

/* Notes about mapping DSS and DRM entities:
 *    CRTC:        overlay
 *    encoder:     manager.. with some extension to allow one primary CRTC
 *                 and zero or more video CRTC's to be mapped to one encoder?
 *    connector:   dssdev.. manager can be attached/detached from different
 *                 devices
 */

static void omap_atomic_wait_for_completion(struct drm_device *dev,
					    struct drm_atomic_state *old_state)
{
	struct drm_crtc_state *new_crtc_state;
	struct drm_crtc *crtc;
	unsigned int i;
	int ret;

	for_each_new_crtc_in_state(old_state, crtc, new_crtc_state, i) {
		if (!new_crtc_state->active)
			continue;

		ret = omap_crtc_wait_pending(crtc);

		if (!ret)
			dev_warn(dev->dev,
				 "atomic complete timeout (pipe %u)!\n", i);
	}
}

static void omap_atomic_commit_tail(struct drm_atomic_state *old_state)
{
	struct drm_device *dev = old_state->dev;
	struct omap_drm_private *priv = dev->dev_private;
<<<<<<< HEAD

	priv->dispc_ops->runtime_get(priv->dispc);
=======
	bool fence_cookie = dma_fence_begin_signalling();

	dispc_runtime_get(priv->dispc);
>>>>>>> upstream/android-13

	/* Apply the atomic update. */
	drm_atomic_helper_commit_modeset_disables(dev, old_state);

	if (priv->omaprev != 0x3430) {
		/* With the current dss dispc implementation we have to enable
		 * the new modeset before we can commit planes. The dispc ovl
		 * configuration relies on the video mode configuration been
		 * written into the HW when the ovl configuration is
		 * calculated.
		 *
		 * This approach is not ideal because after a mode change the
		 * plane update is executed only after the first vblank
		 * interrupt. The dispc implementation should be fixed so that
		 * it is able use uncommitted drm state information.
		 */
		drm_atomic_helper_commit_modeset_enables(dev, old_state);
		omap_atomic_wait_for_completion(dev, old_state);

		drm_atomic_helper_commit_planes(dev, old_state, 0);
<<<<<<< HEAD

		drm_atomic_helper_commit_hw_done(old_state);
=======
>>>>>>> upstream/android-13
	} else {
		/*
		 * OMAP3 DSS seems to have issues with the work-around above,
		 * resulting in endless sync losts if a crtc is enabled without
		 * a plane. For now, skip the WA for OMAP3.
		 */
		drm_atomic_helper_commit_planes(dev, old_state, 0);

		drm_atomic_helper_commit_modeset_enables(dev, old_state);
<<<<<<< HEAD

		drm_atomic_helper_commit_hw_done(old_state);
	}

=======
	}

	drm_atomic_helper_commit_hw_done(old_state);

	dma_fence_end_signalling(fence_cookie);

>>>>>>> upstream/android-13
	/*
	 * Wait for completion of the page flips to ensure that old buffers
	 * can't be touched by the hardware anymore before cleaning up planes.
	 */
	omap_atomic_wait_for_completion(dev, old_state);

	drm_atomic_helper_cleanup_planes(dev, old_state);

<<<<<<< HEAD
	priv->dispc_ops->runtime_put(priv->dispc);
=======
	dispc_runtime_put(priv->dispc);
>>>>>>> upstream/android-13
}

static const struct drm_mode_config_helper_funcs omap_mode_config_helper_funcs = {
	.atomic_commit_tail = omap_atomic_commit_tail,
};

static const struct drm_mode_config_funcs omap_mode_config_funcs = {
	.fb_create = omap_framebuffer_create,
	.output_poll_changed = drm_fb_helper_output_poll_changed,
	.atomic_check = drm_atomic_helper_check,
	.atomic_commit = drm_atomic_helper_commit,
};

<<<<<<< HEAD
static int get_connector_type(struct omap_dss_device *dssdev)
{
	switch (dssdev->type) {
	case OMAP_DISPLAY_TYPE_HDMI:
		return DRM_MODE_CONNECTOR_HDMIA;
	case OMAP_DISPLAY_TYPE_DVI:
		return DRM_MODE_CONNECTOR_DVID;
	case OMAP_DISPLAY_TYPE_DSI:
		return DRM_MODE_CONNECTOR_DSI;
	case OMAP_DISPLAY_TYPE_DPI:
	case OMAP_DISPLAY_TYPE_DBI:
		return DRM_MODE_CONNECTOR_DPI;
	case OMAP_DISPLAY_TYPE_VENC:
		/* TODO: This could also be composite */
		return DRM_MODE_CONNECTOR_SVIDEO;
	case OMAP_DISPLAY_TYPE_SDI:
		return DRM_MODE_CONNECTOR_LVDS;
	default:
		return DRM_MODE_CONNECTOR_Unknown;
	}
}

static void omap_disconnect_dssdevs(void)
{
	struct omap_dss_device *dssdev = NULL;

	for_each_dss_dev(dssdev)
		dssdev->driver->disconnect(dssdev);
}

static int omap_connect_dssdevs(void)
{
	int r;
	struct omap_dss_device *dssdev = NULL;

	if (!omapdss_stack_is_ready())
		return -EPROBE_DEFER;

	for_each_dss_dev(dssdev) {
		r = dssdev->driver->connect(dssdev);
		if (r == -EPROBE_DEFER) {
			omap_dss_put_device(dssdev);
			goto cleanup;
		} else if (r) {
			dev_warn(dssdev->dev, "could not connect display: %s\n",
				dssdev->name);
=======
static void omap_disconnect_pipelines(struct drm_device *ddev)
{
	struct omap_drm_private *priv = ddev->dev_private;
	unsigned int i;

	for (i = 0; i < priv->num_pipes; i++) {
		struct omap_drm_pipeline *pipe = &priv->pipes[i];

		omapdss_device_disconnect(NULL, pipe->output);

		omapdss_device_put(pipe->output);
		pipe->output = NULL;
	}

	memset(&priv->channels, 0, sizeof(priv->channels));

	priv->num_pipes = 0;
}

static int omap_connect_pipelines(struct drm_device *ddev)
{
	struct omap_drm_private *priv = ddev->dev_private;
	struct omap_dss_device *output = NULL;
	int r;

	for_each_dss_output(output) {
		r = omapdss_device_connect(priv->dss, NULL, output);
		if (r == -EPROBE_DEFER) {
			omapdss_device_put(output);
			return r;
		} else if (r) {
			dev_warn(output->dev, "could not connect output %s\n",
				 output->name);
		} else {
			struct omap_drm_pipeline *pipe;

			pipe = &priv->pipes[priv->num_pipes++];
			pipe->output = omapdss_device_get(output);

			if (priv->num_pipes == ARRAY_SIZE(priv->pipes)) {
				/* To balance the 'for_each_dss_output' loop */
				omapdss_device_put(output);
				break;
			}
>>>>>>> upstream/android-13
		}
	}

	return 0;
<<<<<<< HEAD

cleanup:
	/*
	 * if we are deferring probe, we disconnect the devices we previously
	 * connected
	 */
	omap_disconnect_dssdevs();

	return r;
=======
}

static int omap_compare_pipelines(const void *a, const void *b)
{
	const struct omap_drm_pipeline *pipe1 = a;
	const struct omap_drm_pipeline *pipe2 = b;

	if (pipe1->alias_id > pipe2->alias_id)
		return 1;
	else if (pipe1->alias_id < pipe2->alias_id)
		return -1;
	return 0;
>>>>>>> upstream/android-13
}

static int omap_modeset_init_properties(struct drm_device *dev)
{
	struct omap_drm_private *priv = dev->dev_private;
<<<<<<< HEAD
	unsigned int num_planes = priv->dispc_ops->get_num_ovls(priv->dispc);
=======
	unsigned int num_planes = dispc_get_num_ovls(priv->dispc);
>>>>>>> upstream/android-13

	priv->zorder_prop = drm_property_create_range(dev, 0, "zorder", 0,
						      num_planes - 1);
	if (!priv->zorder_prop)
		return -ENOMEM;

	return 0;
}

<<<<<<< HEAD
static int omap_modeset_init(struct drm_device *dev)
{
	struct omap_drm_private *priv = dev->dev_private;
	struct omap_dss_device *dssdev = NULL;
	int num_ovls = priv->dispc_ops->get_num_ovls(priv->dispc);
	int num_mgrs = priv->dispc_ops->get_num_mgrs(priv->dispc);
	int num_crtcs, crtc_idx, plane_idx;
	int ret;
	u32 plane_crtc_mask;

=======
static int omap_display_id(struct omap_dss_device *output)
{
	struct device_node *node = NULL;

	if (output->bridge) {
		struct drm_bridge *bridge = output->bridge;

		while (drm_bridge_get_next_bridge(bridge))
			bridge = drm_bridge_get_next_bridge(bridge);

		node = bridge->of_node;
	}

	return node ? of_alias_get_id(node, "display") : -ENODEV;
}

static int omap_modeset_init(struct drm_device *dev)
{
	struct omap_drm_private *priv = dev->dev_private;
	int num_ovls = dispc_get_num_ovls(priv->dispc);
	int num_mgrs = dispc_get_num_mgrs(priv->dispc);
	unsigned int i;
	int ret;
	u32 plane_crtc_mask;

	if (!omapdss_stack_is_ready())
		return -EPROBE_DEFER;

>>>>>>> upstream/android-13
	drm_mode_config_init(dev);

	ret = omap_modeset_init_properties(dev);
	if (ret < 0)
		return ret;

	/*
	 * This function creates exactly one connector, encoder, crtc,
	 * and primary plane per each connected dss-device. Each
	 * connector->encoder->crtc chain is expected to be separate
	 * and each crtc is connect to a single dss-channel. If the
	 * configuration does not match the expectations or exceeds
	 * the available resources, the configuration is rejected.
	 */
<<<<<<< HEAD
	num_crtcs = 0;
	for_each_dss_dev(dssdev)
		if (omapdss_device_is_connected(dssdev))
			num_crtcs++;

	if (num_crtcs > num_mgrs || num_crtcs > num_ovls ||
	    num_crtcs > ARRAY_SIZE(priv->crtcs) ||
	    num_crtcs > ARRAY_SIZE(priv->planes) ||
	    num_crtcs > ARRAY_SIZE(priv->encoders) ||
	    num_crtcs > ARRAY_SIZE(priv->connectors)) {
=======
	ret = omap_connect_pipelines(dev);
	if (ret < 0)
		return ret;

	if (priv->num_pipes > num_mgrs || priv->num_pipes > num_ovls) {
>>>>>>> upstream/android-13
		dev_err(dev->dev, "%s(): Too many connected displays\n",
			__func__);
		return -EINVAL;
	}

<<<<<<< HEAD
	/* All planes can be put to any CRTC */
	plane_crtc_mask = (1 << num_crtcs) - 1;

	dssdev = NULL;

	crtc_idx = 0;
	plane_idx = 0;
	for_each_dss_dev(dssdev) {
		struct drm_connector *connector;
		struct drm_encoder *encoder;
		struct drm_plane *plane;
		struct drm_crtc *crtc;

		if (!omapdss_device_is_connected(dssdev))
			continue;

		encoder = omap_encoder_init(dev, dssdev);
		if (!encoder)
			return -ENOMEM;

		connector = omap_connector_init(dev,
				get_connector_type(dssdev), dssdev, encoder);
		if (!connector)
			return -ENOMEM;

		plane = omap_plane_init(dev, plane_idx, DRM_PLANE_TYPE_PRIMARY,
					plane_crtc_mask);
		if (IS_ERR(plane))
			return PTR_ERR(plane);

		crtc = omap_crtc_init(dev, plane, dssdev);
		if (IS_ERR(crtc))
			return PTR_ERR(crtc);

		drm_connector_attach_encoder(connector, encoder);
		encoder->possible_crtcs = (1 << crtc_idx);

		priv->crtcs[priv->num_crtcs++] = crtc;
		priv->planes[priv->num_planes++] = plane;
		priv->encoders[priv->num_encoders++] = encoder;
		priv->connectors[priv->num_connectors++] = connector;

		plane_idx++;
		crtc_idx++;
	}

	/*
	 * Create normal planes for the remaining overlays:
	 */
	for (; plane_idx < num_ovls; plane_idx++) {
=======
	/* Create all planes first. They can all be put to any CRTC. */
	plane_crtc_mask = (1 << priv->num_pipes) - 1;

	for (i = 0; i < num_ovls; i++) {
		enum drm_plane_type type = i < priv->num_pipes
					 ? DRM_PLANE_TYPE_PRIMARY
					 : DRM_PLANE_TYPE_OVERLAY;
>>>>>>> upstream/android-13
		struct drm_plane *plane;

		if (WARN_ON(priv->num_planes >= ARRAY_SIZE(priv->planes)))
			return -EINVAL;

<<<<<<< HEAD
		plane = omap_plane_init(dev, plane_idx, DRM_PLANE_TYPE_OVERLAY,
			plane_crtc_mask);
=======
		plane = omap_plane_init(dev, i, type, plane_crtc_mask);
>>>>>>> upstream/android-13
		if (IS_ERR(plane))
			return PTR_ERR(plane);

		priv->planes[priv->num_planes++] = plane;
	}

<<<<<<< HEAD
	DBG("registered %d planes, %d crtcs, %d encoders and %d connectors\n",
		priv->num_planes, priv->num_crtcs, priv->num_encoders,
		priv->num_connectors);
=======
	/*
	 * Create the encoders, attach the bridges and get the pipeline alias
	 * IDs.
	 */
	for (i = 0; i < priv->num_pipes; i++) {
		struct omap_drm_pipeline *pipe = &priv->pipes[i];
		int id;

		pipe->encoder = omap_encoder_init(dev, pipe->output);
		if (!pipe->encoder)
			return -ENOMEM;

		if (pipe->output->bridge) {
			ret = drm_bridge_attach(pipe->encoder,
						pipe->output->bridge, NULL,
						DRM_BRIDGE_ATTACH_NO_CONNECTOR);
			if (ret < 0)
				return ret;
		}

		id = omap_display_id(pipe->output);
		pipe->alias_id = id >= 0 ? id : i;
	}

	/* Sort the pipelines by DT aliases. */
	sort(priv->pipes, priv->num_pipes, sizeof(priv->pipes[0]),
	     omap_compare_pipelines, NULL);

	/*
	 * Populate the pipeline lookup table by DISPC channel. Only one display
	 * is allowed per channel.
	 */
	for (i = 0; i < priv->num_pipes; ++i) {
		struct omap_drm_pipeline *pipe = &priv->pipes[i];
		enum omap_channel channel = pipe->output->dispc_channel;

		if (WARN_ON(priv->channels[channel] != NULL))
			return -EINVAL;

		priv->channels[channel] = pipe;
	}

	/* Create the connectors and CRTCs. */
	for (i = 0; i < priv->num_pipes; i++) {
		struct omap_drm_pipeline *pipe = &priv->pipes[i];
		struct drm_encoder *encoder = pipe->encoder;
		struct drm_crtc *crtc;

		pipe->connector = drm_bridge_connector_init(dev, encoder);
		if (IS_ERR(pipe->connector)) {
			dev_err(priv->dev,
				"unable to create bridge connector for %s\n",
				pipe->output->name);
			return PTR_ERR(pipe->connector);
		}

		drm_connector_attach_encoder(pipe->connector, encoder);

		crtc = omap_crtc_init(dev, pipe, priv->planes[i]);
		if (IS_ERR(crtc))
			return PTR_ERR(crtc);

		encoder->possible_crtcs = 1 << i;
		pipe->crtc = crtc;
	}

	DBG("registered %u planes, %u crtcs/encoders/connectors\n",
	    priv->num_planes, priv->num_pipes);
>>>>>>> upstream/android-13

	dev->mode_config.min_width = 8;
	dev->mode_config.min_height = 2;

	/*
	 * Note: these values are used for multiple independent things:
	 * connector mode filtering, buffer sizes, crtc sizes...
	 * Use big enough values here to cover all use cases, and do more
	 * specific checking in the respective code paths.
	 */
	dev->mode_config.max_width = 8192;
	dev->mode_config.max_height = 8192;

	/* We want the zpos to be normalized */
	dev->mode_config.normalize_zpos = true;

	dev->mode_config.funcs = &omap_mode_config_funcs;
	dev->mode_config.helper_private = &omap_mode_config_helper_funcs;

	drm_mode_config_reset(dev);

	omap_drm_irq_install(dev);

	return 0;
}

<<<<<<< HEAD
/*
 * Enable the HPD in external components if supported
 */
static void omap_modeset_enable_external_hpd(void)
{
	struct omap_dss_device *dssdev = NULL;

	for_each_dss_dev(dssdev) {
		if (dssdev->driver->enable_hpd)
			dssdev->driver->enable_hpd(dssdev);
=======
static void omap_modeset_fini(struct drm_device *ddev)
{
	omap_drm_irq_uninstall(ddev);

	drm_mode_config_cleanup(ddev);
}

/*
 * Enable the HPD in external components if supported
 */
static void omap_modeset_enable_external_hpd(struct drm_device *ddev)
{
	struct omap_drm_private *priv = ddev->dev_private;
	unsigned int i;

	for (i = 0; i < priv->num_pipes; i++) {
		struct drm_connector *connector = priv->pipes[i].connector;

		if (!connector)
			continue;

		if (priv->pipes[i].output->bridge)
			drm_bridge_connector_enable_hpd(connector);
>>>>>>> upstream/android-13
	}
}

/*
 * Disable the HPD in external components if supported
 */
<<<<<<< HEAD
static void omap_modeset_disable_external_hpd(void)
{
	struct omap_dss_device *dssdev = NULL;

	for_each_dss_dev(dssdev) {
		if (dssdev->driver->disable_hpd)
			dssdev->driver->disable_hpd(dssdev);
=======
static void omap_modeset_disable_external_hpd(struct drm_device *ddev)
{
	struct omap_drm_private *priv = ddev->dev_private;
	unsigned int i;

	for (i = 0; i < priv->num_pipes; i++) {
		struct drm_connector *connector = priv->pipes[i].connector;

		if (!connector)
			continue;

		if (priv->pipes[i].output->bridge)
			drm_bridge_connector_disable_hpd(connector);
>>>>>>> upstream/android-13
	}
}

/*
 * drm ioctl funcs
 */


static int ioctl_get_param(struct drm_device *dev, void *data,
		struct drm_file *file_priv)
{
	struct omap_drm_private *priv = dev->dev_private;
	struct drm_omap_param *args = data;

	DBG("%p: param=%llu", dev, args->param);

	switch (args->param) {
	case OMAP_PARAM_CHIPSET_ID:
		args->value = priv->omaprev;
		break;
	default:
		DBG("unknown parameter %lld", args->param);
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD
static int ioctl_set_param(struct drm_device *dev, void *data,
		struct drm_file *file_priv)
{
	struct drm_omap_param *args = data;

	switch (args->param) {
	default:
		DBG("unknown parameter %lld", args->param);
		return -EINVAL;
	}

	return 0;
}

=======
>>>>>>> upstream/android-13
#define OMAP_BO_USER_MASK	0x00ffffff	/* flags settable by userspace */

static int ioctl_gem_new(struct drm_device *dev, void *data,
		struct drm_file *file_priv)
{
	struct drm_omap_gem_new *args = data;
	u32 flags = args->flags & OMAP_BO_USER_MASK;

	VERB("%p:%p: size=0x%08x, flags=%08x", dev, file_priv,
	     args->size.bytes, flags);

	return omap_gem_new_handle(dev, file_priv, args->size, flags,
				   &args->handle);
}

static int ioctl_gem_info(struct drm_device *dev, void *data,
		struct drm_file *file_priv)
{
	struct drm_omap_gem_info *args = data;
	struct drm_gem_object *obj;
	int ret = 0;

	VERB("%p:%p: handle=%d", dev, file_priv, args->handle);

	obj = drm_gem_object_lookup(file_priv, args->handle);
	if (!obj)
		return -ENOENT;

	args->size = omap_gem_mmap_size(obj);
	args->offset = omap_gem_mmap_offset(obj);

<<<<<<< HEAD
	drm_gem_object_unreference_unlocked(obj);
=======
	drm_gem_object_put(obj);
>>>>>>> upstream/android-13

	return ret;
}

static const struct drm_ioctl_desc ioctls[DRM_COMMAND_END - DRM_COMMAND_BASE] = {
	DRM_IOCTL_DEF_DRV(OMAP_GET_PARAM, ioctl_get_param,
<<<<<<< HEAD
			  DRM_AUTH | DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(OMAP_SET_PARAM, ioctl_set_param,
			  DRM_AUTH | DRM_MASTER | DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(OMAP_GEM_NEW, ioctl_gem_new,
			  DRM_AUTH | DRM_RENDER_ALLOW),
	/* Deprecated, to be removed. */
	DRM_IOCTL_DEF_DRV(OMAP_GEM_CPU_PREP, drm_noop,
			  DRM_AUTH | DRM_RENDER_ALLOW),
	/* Deprecated, to be removed. */
	DRM_IOCTL_DEF_DRV(OMAP_GEM_CPU_FINI, drm_noop,
			  DRM_AUTH | DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(OMAP_GEM_INFO, ioctl_gem_info,
			  DRM_AUTH | DRM_RENDER_ALLOW),
=======
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(OMAP_SET_PARAM, drm_invalid_op,
			  DRM_AUTH | DRM_MASTER | DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(OMAP_GEM_NEW, ioctl_gem_new,
			  DRM_RENDER_ALLOW),
	/* Deprecated, to be removed. */
	DRM_IOCTL_DEF_DRV(OMAP_GEM_CPU_PREP, drm_noop,
			  DRM_RENDER_ALLOW),
	/* Deprecated, to be removed. */
	DRM_IOCTL_DEF_DRV(OMAP_GEM_CPU_FINI, drm_noop,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(OMAP_GEM_INFO, ioctl_gem_info,
			  DRM_RENDER_ALLOW),
>>>>>>> upstream/android-13
};

/*
 * drm driver funcs
 */

static int dev_open(struct drm_device *dev, struct drm_file *file)
{
	file->driver_priv = NULL;

	DBG("open: dev=%p, file=%p", dev, file);

	return 0;
}

<<<<<<< HEAD
static const struct vm_operations_struct omap_gem_vm_ops = {
	.fault = omap_gem_fault,
	.open = drm_gem_vm_open,
	.close = drm_gem_vm_close,
};

=======
>>>>>>> upstream/android-13
static const struct file_operations omapdriver_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.unlocked_ioctl = drm_ioctl,
	.compat_ioctl = drm_compat_ioctl,
	.release = drm_release,
	.mmap = omap_gem_mmap,
	.poll = drm_poll,
	.read = drm_read,
	.llseek = noop_llseek,
};

<<<<<<< HEAD
static struct drm_driver omap_drm_driver = {
	.driver_features = DRIVER_MODESET | DRIVER_GEM  | DRIVER_PRIME |
=======
static const struct drm_driver omap_drm_driver = {
	.driver_features = DRIVER_MODESET | DRIVER_GEM  |
>>>>>>> upstream/android-13
		DRIVER_ATOMIC | DRIVER_RENDER,
	.open = dev_open,
	.lastclose = drm_fb_helper_lastclose,
#ifdef CONFIG_DEBUG_FS
	.debugfs_init = omap_debugfs_init,
#endif
	.prime_handle_to_fd = drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle = drm_gem_prime_fd_to_handle,
<<<<<<< HEAD
	.gem_prime_export = omap_gem_prime_export,
	.gem_prime_import = omap_gem_prime_import,
	.gem_free_object_unlocked = omap_gem_free_object,
	.gem_vm_ops = &omap_gem_vm_ops,
=======
	.gem_prime_import = omap_gem_prime_import,
>>>>>>> upstream/android-13
	.dumb_create = omap_gem_dumb_create,
	.dumb_map_offset = omap_gem_dumb_map_offset,
	.ioctls = ioctls,
	.num_ioctls = DRM_OMAP_NUM_IOCTLS,
	.fops = &omapdriver_fops,
	.name = DRIVER_NAME,
	.desc = DRIVER_DESC,
	.date = DRIVER_DATE,
	.major = DRIVER_MAJOR,
	.minor = DRIVER_MINOR,
	.patchlevel = DRIVER_PATCHLEVEL,
};

static const struct soc_device_attribute omapdrm_soc_devices[] = {
	{ .family = "OMAP3", .data = (void *)0x3430 },
	{ .family = "OMAP4", .data = (void *)0x4430 },
	{ .family = "OMAP5", .data = (void *)0x5430 },
	{ .family = "DRA7",  .data = (void *)0x0752 },
	{ /* sentinel */ }
};

static int omapdrm_init(struct omap_drm_private *priv, struct device *dev)
{
	const struct soc_device_attribute *soc;
<<<<<<< HEAD
	struct drm_device *ddev;
	unsigned int i;
=======
	struct dss_pdata *pdata = dev->platform_data;
	struct drm_device *ddev;
>>>>>>> upstream/android-13
	int ret;

	DBG("%s", dev_name(dev));

<<<<<<< HEAD
	priv->dev = dev;
	priv->dss = omapdss_get_dss();
	priv->dispc = dispc_get_dispc(priv->dss);
	priv->dispc_ops = dispc_get_ops(priv->dss);

	omap_crtc_pre_init(priv);

	ret = omap_connect_dssdevs();
	if (ret)
		goto err_crtc_uninit;
=======
	/* Allocate and initialize the DRM device. */
	ddev = drm_dev_alloc(&omap_drm_driver, dev);
	if (IS_ERR(ddev))
		return PTR_ERR(ddev);

	priv->ddev = ddev;
	ddev->dev_private = priv;

	priv->dev = dev;
	priv->dss = pdata->dss;
	priv->dispc = dispc_get_dispc(priv->dss);

	priv->dss->mgr_ops_priv = priv;
>>>>>>> upstream/android-13

	soc = soc_device_match(omapdrm_soc_devices);
	priv->omaprev = soc ? (unsigned int)soc->data : 0;
	priv->wq = alloc_ordered_workqueue("omapdrm", 0);

	mutex_init(&priv->list_lock);
	INIT_LIST_HEAD(&priv->obj_list);

<<<<<<< HEAD
	/* Allocate and initialize the DRM device. */
	ddev = drm_dev_alloc(&omap_drm_driver, priv->dev);
	if (IS_ERR(ddev)) {
		ret = PTR_ERR(ddev);
		goto err_destroy_wq;
	}

	priv->ddev = ddev;
	ddev->dev_private = priv;

	/* Get memory bandwidth limits */
	if (priv->dispc_ops->get_memory_bandwidth_limit)
		priv->max_bandwidth =
			priv->dispc_ops->get_memory_bandwidth_limit(priv->dispc);
=======
	/* Get memory bandwidth limits */
	priv->max_bandwidth = dispc_get_memory_bandwidth_limit(priv->dispc);
>>>>>>> upstream/android-13

	omap_gem_init(ddev);

	ret = omap_modeset_init(ddev);
	if (ret) {
		dev_err(priv->dev, "omap_modeset_init failed: ret=%d\n", ret);
<<<<<<< HEAD
		goto err_free_drm_dev;
	}

	/* Initialize vblank handling, start with all CRTCs disabled. */
	ret = drm_vblank_init(ddev, priv->num_crtcs);
=======
		goto err_gem_deinit;
	}

	/* Initialize vblank handling, start with all CRTCs disabled. */
	ret = drm_vblank_init(ddev, priv->num_pipes);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(priv->dev, "could not init vblank\n");
		goto err_cleanup_modeset;
	}

<<<<<<< HEAD
	for (i = 0; i < priv->num_crtcs; i++)
		drm_crtc_vblank_off(priv->crtcs[i]);

	omap_fbdev_init(ddev);

	drm_kms_helper_poll_init(ddev);
	omap_modeset_enable_external_hpd();
=======
	omap_fbdev_init(ddev);

	drm_kms_helper_poll_init(ddev);
	omap_modeset_enable_external_hpd(ddev);
>>>>>>> upstream/android-13

	/*
	 * Register the DRM device with the core and the connectors with
	 * sysfs.
	 */
	ret = drm_dev_register(ddev, 0);
	if (ret)
		goto err_cleanup_helpers;

	return 0;

err_cleanup_helpers:
<<<<<<< HEAD
	omap_modeset_disable_external_hpd();
=======
	omap_modeset_disable_external_hpd(ddev);
>>>>>>> upstream/android-13
	drm_kms_helper_poll_fini(ddev);

	omap_fbdev_fini(ddev);
err_cleanup_modeset:
<<<<<<< HEAD
	drm_mode_config_cleanup(ddev);
	omap_drm_irq_uninstall(ddev);
err_free_drm_dev:
	omap_gem_deinit(ddev);
	drm_dev_unref(ddev);
err_destroy_wq:
	destroy_workqueue(priv->wq);
	omap_disconnect_dssdevs();
err_crtc_uninit:
	omap_crtc_pre_uninit();
=======
	omap_modeset_fini(ddev);
err_gem_deinit:
	omap_gem_deinit(ddev);
	destroy_workqueue(priv->wq);
	omap_disconnect_pipelines(ddev);
	drm_dev_put(ddev);
>>>>>>> upstream/android-13
	return ret;
}

static void omapdrm_cleanup(struct omap_drm_private *priv)
{
	struct drm_device *ddev = priv->ddev;

	DBG("");

	drm_dev_unregister(ddev);

<<<<<<< HEAD
	omap_modeset_disable_external_hpd();
=======
	omap_modeset_disable_external_hpd(ddev);
>>>>>>> upstream/android-13
	drm_kms_helper_poll_fini(ddev);

	omap_fbdev_fini(ddev);

	drm_atomic_helper_shutdown(ddev);

<<<<<<< HEAD
	drm_mode_config_cleanup(ddev);

	omap_drm_irq_uninstall(ddev);
	omap_gem_deinit(ddev);

	drm_dev_unref(ddev);

	destroy_workqueue(priv->wq);

	omap_disconnect_dssdevs();
	omap_crtc_pre_uninit();
=======
	omap_modeset_fini(ddev);
	omap_gem_deinit(ddev);

	destroy_workqueue(priv->wq);

	omap_disconnect_pipelines(ddev);

	drm_dev_put(ddev);
>>>>>>> upstream/android-13
}

static int pdev_probe(struct platform_device *pdev)
{
	struct omap_drm_private *priv;
	int ret;

<<<<<<< HEAD
	if (omapdss_is_initialized() == false)
		return -EPROBE_DEFER;

	ret = dma_set_coherent_mask(&pdev->dev, DMA_BIT_MASK(32));
=======
	ret = dma_coerce_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(&pdev->dev, "Failed to set the DMA mask\n");
		return ret;
	}

	/* Allocate and initialize the driver private structure. */
	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	platform_set_drvdata(pdev, priv);

	ret = omapdrm_init(priv, &pdev->dev);
	if (ret < 0)
		kfree(priv);

	return ret;
}

static int pdev_remove(struct platform_device *pdev)
{
	struct omap_drm_private *priv = platform_get_drvdata(pdev);

	omapdrm_cleanup(priv);
	kfree(priv);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
<<<<<<< HEAD
static int omap_drm_suspend_all_displays(void)
{
	struct omap_dss_device *dssdev = NULL;

	for_each_dss_dev(dssdev) {
		if (!dssdev->driver)
			continue;

		if (dssdev->state == OMAP_DSS_DISPLAY_ACTIVE) {
			dssdev->driver->disable(dssdev);
			dssdev->activate_after_resume = true;
		} else {
			dssdev->activate_after_resume = false;
		}
	}

	return 0;
}

static int omap_drm_resume_all_displays(void)
{
	struct omap_dss_device *dssdev = NULL;

	for_each_dss_dev(dssdev) {
		if (!dssdev->driver)
			continue;

		if (dssdev->activate_after_resume) {
			dssdev->driver->enable(dssdev);
			dssdev->activate_after_resume = false;
		}
	}

	return 0;
}

=======
>>>>>>> upstream/android-13
static int omap_drm_suspend(struct device *dev)
{
	struct omap_drm_private *priv = dev_get_drvdata(dev);
	struct drm_device *drm_dev = priv->ddev;

<<<<<<< HEAD
	drm_kms_helper_poll_disable(drm_dev);

	drm_modeset_lock_all(drm_dev);
	omap_drm_suspend_all_displays();
	drm_modeset_unlock_all(drm_dev);

	return 0;
=======
	return drm_mode_config_helper_suspend(drm_dev);
>>>>>>> upstream/android-13
}

static int omap_drm_resume(struct device *dev)
{
	struct omap_drm_private *priv = dev_get_drvdata(dev);
	struct drm_device *drm_dev = priv->ddev;

<<<<<<< HEAD
	drm_modeset_lock_all(drm_dev);
	omap_drm_resume_all_displays();
	drm_modeset_unlock_all(drm_dev);

	drm_kms_helper_poll_enable(drm_dev);
=======
	drm_mode_config_helper_resume(drm_dev);
>>>>>>> upstream/android-13

	return omap_gem_resume(drm_dev);
}
#endif

static SIMPLE_DEV_PM_OPS(omapdrm_pm_ops, omap_drm_suspend, omap_drm_resume);

static struct platform_driver pdev = {
	.driver = {
		.name = "omapdrm",
		.pm = &omapdrm_pm_ops,
	},
	.probe = pdev_probe,
	.remove = pdev_remove,
};

static struct platform_driver * const drivers[] = {
	&omap_dmm_driver,
	&pdev,
};

static int __init omap_drm_init(void)
{
<<<<<<< HEAD
	DBG("init");

	return platform_register_drivers(drivers, ARRAY_SIZE(drivers));
=======
	int r;

	DBG("init");

	r = omap_dss_init();
	if (r)
		return r;

	r = platform_register_drivers(drivers, ARRAY_SIZE(drivers));
	if (r) {
		omap_dss_exit();
		return r;
	}

	return 0;
>>>>>>> upstream/android-13
}

static void __exit omap_drm_fini(void)
{
	DBG("fini");

	platform_unregister_drivers(drivers, ARRAY_SIZE(drivers));
<<<<<<< HEAD
}

/* need late_initcall() so we load after dss_driver's are loaded */
late_initcall(omap_drm_init);
module_exit(omap_drm_fini);

MODULE_AUTHOR("Rob Clark <rob@ti.com>");
=======

	omap_dss_exit();
}

module_init(omap_drm_init);
module_exit(omap_drm_fini);

MODULE_AUTHOR("Rob Clark <rob@ti.com>");
MODULE_AUTHOR("Tomi Valkeinen <tomi.valkeinen@ti.com>");
>>>>>>> upstream/android-13
MODULE_DESCRIPTION("OMAP DRM Display Driver");
MODULE_ALIAS("platform:" DRIVER_NAME);
MODULE_LICENSE("GPL v2");
