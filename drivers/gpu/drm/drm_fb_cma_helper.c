<<<<<<< HEAD
/*
 * drm kms/fb cma (contiguous memory allocator) helper functions
 *
 * Copyright (C) 2012 Analog Device Inc.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * drm kms/fb cma (contiguous memory allocator) helper functions
 *
 * Copyright (C) 2012 Analog Devices Inc.
>>>>>>> upstream/android-13
 *   Author: Lars-Peter Clausen <lars@metafoo.de>
 *
 * Based on udl_fbdev.c
 *  Copyright (C) 2012 Red Hat
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <drm/drmP.h>
#include <drm/drm_client.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_framebuffer.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_print.h>
#include <linux/module.h>

struct drm_fbdev_cma {
	struct drm_fb_helper	fb_helper;
};

=======
 */

#include <drm/drm_damage_helper.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_framebuffer.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_plane.h>
#include <linux/dma-mapping.h>
#include <linux/module.h>

>>>>>>> upstream/android-13
/**
 * DOC: framebuffer cma helper functions
 *
 * Provides helper functions for creating a cma (contiguous memory allocator)
 * backed framebuffer.
 *
 * drm_gem_fb_create() is used in the &drm_mode_config_funcs.fb_create
 * callback function to create a cma backed framebuffer.
<<<<<<< HEAD
 *
 * An fbdev framebuffer backed by cma is also available by calling
 * drm_fb_cma_fbdev_init(). drm_fb_cma_fbdev_fini() tears it down.
 */

static inline struct drm_fbdev_cma *to_fbdev_cma(struct drm_fb_helper *helper)
{
	return container_of(helper, struct drm_fbdev_cma, fb_helper);
}

=======
 */

>>>>>>> upstream/android-13
/**
 * drm_fb_cma_get_gem_obj() - Get CMA GEM object for framebuffer
 * @fb: The framebuffer
 * @plane: Which plane
 *
 * Return the CMA GEM object for given framebuffer.
 *
 * This function will usually be called from the CRTC callback functions.
 */
struct drm_gem_cma_object *drm_fb_cma_get_gem_obj(struct drm_framebuffer *fb,
						  unsigned int plane)
{
	struct drm_gem_object *gem;

	gem = drm_gem_fb_get_obj(fb, plane);
	if (!gem)
		return NULL;

	return to_drm_gem_cma_obj(gem);
}
EXPORT_SYMBOL_GPL(drm_fb_cma_get_gem_obj);

/**
<<<<<<< HEAD
 * drm_fb_cma_get_gem_addr() - Get physical address for framebuffer
=======
 * drm_fb_cma_get_gem_addr() - Get physical address for framebuffer, for pixel
 * formats where values are grouped in blocks this will get you the beginning of
 * the block
>>>>>>> upstream/android-13
 * @fb: The framebuffer
 * @state: Which state of drm plane
 * @plane: Which plane
 * Return the CMA GEM address for given framebuffer.
 *
 * This function will usually be called from the PLANE callback functions.
 */
dma_addr_t drm_fb_cma_get_gem_addr(struct drm_framebuffer *fb,
				   struct drm_plane_state *state,
				   unsigned int plane)
{
	struct drm_gem_cma_object *obj;
	dma_addr_t paddr;
<<<<<<< HEAD
=======
	u8 h_div = 1, v_div = 1;
	u32 block_w = drm_format_info_block_width(fb->format, plane);
	u32 block_h = drm_format_info_block_height(fb->format, plane);
	u32 block_size = fb->format->char_per_block[plane];
	u32 sample_x;
	u32 sample_y;
	u32 block_start_y;
	u32 num_hblocks;
>>>>>>> upstream/android-13

	obj = drm_fb_cma_get_gem_obj(fb, plane);
	if (!obj)
		return 0;

	paddr = obj->paddr + fb->offsets[plane];
<<<<<<< HEAD
	paddr += fb->format->cpp[plane] * (state->src_x >> 16);
	paddr += fb->pitches[plane] * (state->src_y >> 16);
=======

	if (plane > 0) {
		h_div = fb->format->hsub;
		v_div = fb->format->vsub;
	}

	sample_x = (state->src_x >> 16) / h_div;
	sample_y = (state->src_y >> 16) / v_div;
	block_start_y = (sample_y / block_h) * block_h;
	num_hblocks = sample_x / block_w;

	paddr += fb->pitches[plane] * block_start_y;
	paddr += block_size * num_hblocks;
>>>>>>> upstream/android-13

	return paddr;
}
EXPORT_SYMBOL_GPL(drm_fb_cma_get_gem_addr);

/**
<<<<<<< HEAD
 * drm_fb_cma_fbdev_init() - Allocate and initialize fbdev emulation
 * @dev: DRM device
 * @preferred_bpp: Preferred bits per pixel for the device.
 *                 @dev->mode_config.preferred_depth is used if this is zero.
 * @max_conn_count: Maximum number of connectors.
 *                  @dev->mode_config.num_connector is used if this is zero.
 *
 * Returns:
 * Zero on success or negative error code on failure.
 */
int drm_fb_cma_fbdev_init(struct drm_device *dev, unsigned int preferred_bpp,
			  unsigned int max_conn_count)
{
	struct drm_fbdev_cma *fbdev_cma;

	/* dev->fb_helper will indirectly point to fbdev_cma after this call */
	fbdev_cma = drm_fbdev_cma_init(dev, preferred_bpp, max_conn_count);
	if (IS_ERR(fbdev_cma))
		return PTR_ERR(fbdev_cma);

	return 0;
}
EXPORT_SYMBOL_GPL(drm_fb_cma_fbdev_init);

/**
 * drm_fb_cma_fbdev_fini() - Teardown fbdev emulation
 * @dev: DRM device
 */
void drm_fb_cma_fbdev_fini(struct drm_device *dev)
{
	if (dev->fb_helper)
		drm_fbdev_cma_fini(to_fbdev_cma(dev->fb_helper));
}
EXPORT_SYMBOL_GPL(drm_fb_cma_fbdev_fini);

static const struct drm_fb_helper_funcs drm_fb_cma_helper_funcs = {
	.fb_probe = drm_fb_helper_generic_probe,
};

/**
 * drm_fbdev_cma_init() - Allocate and initializes a drm_fbdev_cma struct
 * @dev: DRM device
 * @preferred_bpp: Preferred bits per pixel for the device
 * @max_conn_count: Maximum number of connectors
 *
 * Returns a newly allocated drm_fbdev_cma struct or a ERR_PTR.
 */
struct drm_fbdev_cma *drm_fbdev_cma_init(struct drm_device *dev,
	unsigned int preferred_bpp, unsigned int max_conn_count)
{
	struct drm_fbdev_cma *fbdev_cma;
	struct drm_fb_helper *fb_helper;
	int ret;

	fbdev_cma = kzalloc(sizeof(*fbdev_cma), GFP_KERNEL);
	if (!fbdev_cma)
		return ERR_PTR(-ENOMEM);

	fb_helper = &fbdev_cma->fb_helper;

	ret = drm_client_init(dev, &fb_helper->client, "fbdev", NULL);
	if (ret)
		goto err_free;

	ret = drm_fb_helper_fbdev_setup(dev, fb_helper, &drm_fb_cma_helper_funcs,
					preferred_bpp, max_conn_count);
	if (ret)
		goto err_client_put;

	drm_client_add(&fb_helper->client);

	return fbdev_cma;

err_client_put:
	drm_client_release(&fb_helper->client);
err_free:
	kfree(fbdev_cma);

	return ERR_PTR(ret);
}
EXPORT_SYMBOL_GPL(drm_fbdev_cma_init);

/**
 * drm_fbdev_cma_fini() - Free drm_fbdev_cma struct
 * @fbdev_cma: The drm_fbdev_cma struct
 */
void drm_fbdev_cma_fini(struct drm_fbdev_cma *fbdev_cma)
{
	drm_fb_helper_unregister_fbi(&fbdev_cma->fb_helper);
	/* All resources have now been freed by drm_fbdev_fb_destroy() */
}
EXPORT_SYMBOL_GPL(drm_fbdev_cma_fini);

/**
 * drm_fbdev_cma_restore_mode() - Restores initial framebuffer mode
 * @fbdev_cma: The drm_fbdev_cma struct, may be NULL
 *
 * This function is usually called from the &drm_driver.lastclose callback.
 */
void drm_fbdev_cma_restore_mode(struct drm_fbdev_cma *fbdev_cma)
{
	if (fbdev_cma)
		drm_fb_helper_restore_fbdev_mode_unlocked(&fbdev_cma->fb_helper);
}
EXPORT_SYMBOL_GPL(drm_fbdev_cma_restore_mode);

/**
 * drm_fbdev_cma_hotplug_event() - Poll for hotpulug events
 * @fbdev_cma: The drm_fbdev_cma struct, may be NULL
 *
 * This function is usually called from the &drm_mode_config.output_poll_changed
 * callback.
 */
void drm_fbdev_cma_hotplug_event(struct drm_fbdev_cma *fbdev_cma)
{
	if (fbdev_cma)
		drm_fb_helper_hotplug_event(&fbdev_cma->fb_helper);
}
EXPORT_SYMBOL_GPL(drm_fbdev_cma_hotplug_event);

/**
 * drm_fbdev_cma_set_suspend - wrapper around drm_fb_helper_set_suspend
 * @fbdev_cma: The drm_fbdev_cma struct, may be NULL
 * @state: desired state, zero to resume, non-zero to suspend
 *
 * Calls drm_fb_helper_set_suspend, which is a wrapper around
 * fb_set_suspend implemented by fbdev core.
 */
void drm_fbdev_cma_set_suspend(struct drm_fbdev_cma *fbdev_cma, bool state)
{
	if (fbdev_cma)
		drm_fb_helper_set_suspend(&fbdev_cma->fb_helper, state);
}
EXPORT_SYMBOL(drm_fbdev_cma_set_suspend);

/**
 * drm_fbdev_cma_set_suspend_unlocked - wrapper around
 *                                      drm_fb_helper_set_suspend_unlocked
 * @fbdev_cma: The drm_fbdev_cma struct, may be NULL
 * @state: desired state, zero to resume, non-zero to suspend
 *
 * Calls drm_fb_helper_set_suspend, which is a wrapper around
 * fb_set_suspend implemented by fbdev core.
 */
void drm_fbdev_cma_set_suspend_unlocked(struct drm_fbdev_cma *fbdev_cma,
					bool state)
{
	if (fbdev_cma)
		drm_fb_helper_set_suspend_unlocked(&fbdev_cma->fb_helper,
						   state);
}
EXPORT_SYMBOL(drm_fbdev_cma_set_suspend_unlocked);
=======
 * drm_fb_cma_sync_non_coherent - Sync GEM object to non-coherent backing
 *	memory
 * @drm: DRM device
 * @old_state: Old plane state
 * @state: New plane state
 *
 * This function can be used by drivers that use damage clips and have
 * CMA GEM objects backed by non-coherent memory. Calling this function
 * in a plane's .atomic_update ensures that all the data in the backing
 * memory have been written to RAM.
 */
void drm_fb_cma_sync_non_coherent(struct drm_device *drm,
				  struct drm_plane_state *old_state,
				  struct drm_plane_state *state)
{
	const struct drm_format_info *finfo = state->fb->format;
	struct drm_atomic_helper_damage_iter iter;
	const struct drm_gem_cma_object *cma_obj;
	unsigned int offset, i;
	struct drm_rect clip;
	dma_addr_t daddr;
	size_t nb_bytes;

	for (i = 0; i < finfo->num_planes; i++) {
		cma_obj = drm_fb_cma_get_gem_obj(state->fb, i);
		if (!cma_obj->map_noncoherent)
			continue;

		daddr = drm_fb_cma_get_gem_addr(state->fb, state, i);
		drm_atomic_helper_damage_iter_init(&iter, old_state, state);

		drm_atomic_for_each_plane_damage(&iter, &clip) {
			/* Ignore x1/x2 values, invalidate complete lines */
			offset = clip.y1 * state->fb->pitches[i];

			nb_bytes = (clip.y2 - clip.y1) * state->fb->pitches[i];
			dma_sync_single_for_device(drm->dev, daddr + offset,
						   nb_bytes, DMA_TO_DEVICE);
		}
	}
}
EXPORT_SYMBOL_GPL(drm_fb_cma_sync_non_coherent);
>>>>>>> upstream/android-13
