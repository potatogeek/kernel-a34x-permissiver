/*
 * Copyright 2018 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#include "wndw.h"
#include "wimm.h"
<<<<<<< HEAD
=======
#include "handles.h"
>>>>>>> upstream/android-13

#include <nvif/class.h>
#include <nvif/cl0002.h>

<<<<<<< HEAD
#include <drm/drm_atomic_helper.h>
#include "nouveau_bo.h"
=======
#include <nvhw/class/cl507c.h>
#include <nvhw/class/cl507e.h>
#include <nvhw/class/clc37e.h>

#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_fourcc.h>

#include "nouveau_bo.h"
#include "nouveau_gem.h"
>>>>>>> upstream/android-13

static void
nv50_wndw_ctxdma_del(struct nv50_wndw_ctxdma *ctxdma)
{
<<<<<<< HEAD
	nvif_object_fini(&ctxdma->object);
=======
	nvif_object_dtor(&ctxdma->object);
>>>>>>> upstream/android-13
	list_del(&ctxdma->head);
	kfree(ctxdma);
}

static struct nv50_wndw_ctxdma *
<<<<<<< HEAD
nv50_wndw_ctxdma_new(struct nv50_wndw *wndw, struct nouveau_framebuffer *fb)
{
	struct nouveau_drm *drm = nouveau_drm(fb->base.dev);
	struct nv50_wndw_ctxdma *ctxdma;
	const u8    kind = fb->nvbo->kind;
	const u32 handle = 0xfb000000 | kind;
=======
nv50_wndw_ctxdma_new(struct nv50_wndw *wndw, struct drm_framebuffer *fb)
{
	struct nouveau_drm *drm = nouveau_drm(fb->dev);
	struct nv50_wndw_ctxdma *ctxdma;
	u32 handle;
	u32 unused;
	u8  kind;
>>>>>>> upstream/android-13
	struct {
		struct nv_dma_v0 base;
		union {
			struct nv50_dma_v0 nv50;
			struct gf100_dma_v0 gf100;
			struct gf119_dma_v0 gf119;
		};
	} args = {};
	u32 argc = sizeof(args.base);
	int ret;

<<<<<<< HEAD
=======
	nouveau_framebuffer_get_layout(fb, &unused, &kind);
	handle = NV50_DISP_HANDLE_WNDW_CTX(kind);

>>>>>>> upstream/android-13
	list_for_each_entry(ctxdma, &wndw->ctxdma.list, head) {
		if (ctxdma->object.handle == handle)
			return ctxdma;
	}

	if (!(ctxdma = kzalloc(sizeof(*ctxdma), GFP_KERNEL)))
		return ERR_PTR(-ENOMEM);
	list_add(&ctxdma->head, &wndw->ctxdma.list);

	args.base.target = NV_DMA_V0_TARGET_VRAM;
	args.base.access = NV_DMA_V0_ACCESS_RDWR;
	args.base.start  = 0;
	args.base.limit  = drm->client.device.info.ram_user - 1;

	if (drm->client.device.info.chipset < 0x80) {
		args.nv50.part = NV50_DMA_V0_PART_256;
		argc += sizeof(args.nv50);
	} else
	if (drm->client.device.info.chipset < 0xc0) {
		args.nv50.part = NV50_DMA_V0_PART_256;
		args.nv50.kind = kind;
		argc += sizeof(args.nv50);
	} else
	if (drm->client.device.info.chipset < 0xd0) {
		args.gf100.kind = kind;
		argc += sizeof(args.gf100);
	} else {
		args.gf119.page = GF119_DMA_V0_PAGE_LP;
		args.gf119.kind = kind;
		argc += sizeof(args.gf119);
	}

<<<<<<< HEAD
	ret = nvif_object_init(wndw->ctxdma.parent, handle, NV_DMA_IN_MEMORY,
			       &args, argc, &ctxdma->object);
=======
	ret = nvif_object_ctor(wndw->ctxdma.parent, "kmsFbCtxDma", handle,
			       NV_DMA_IN_MEMORY, &args, argc, &ctxdma->object);
>>>>>>> upstream/android-13
	if (ret) {
		nv50_wndw_ctxdma_del(ctxdma);
		return ERR_PTR(ret);
	}

	return ctxdma;
}

int
nv50_wndw_wait_armed(struct nv50_wndw *wndw, struct nv50_wndw_atom *asyw)
{
	struct nv50_disp *disp = nv50_disp(wndw->plane.dev);
	if (asyw->set.ntfy) {
		return wndw->func->ntfy_wait_begun(disp->sync,
						   asyw->ntfy.offset,
						   wndw->wndw.base.device);
	}
	return 0;
}

void
nv50_wndw_flush_clr(struct nv50_wndw *wndw, u32 *interlock, bool flush,
		    struct nv50_wndw_atom *asyw)
{
	union nv50_wndw_atom_mask clr = {
		.mask = asyw->clr.mask & ~(flush ? 0 : asyw->set.mask),
	};
	if (clr.sema ) wndw->func-> sema_clr(wndw);
	if (clr.ntfy ) wndw->func-> ntfy_clr(wndw);
	if (clr.xlut ) wndw->func-> xlut_clr(wndw);
<<<<<<< HEAD
=======
	if (clr.csc  ) wndw->func->  csc_clr(wndw);
>>>>>>> upstream/android-13
	if (clr.image) wndw->func->image_clr(wndw);

	interlock[wndw->interlock.type] |= wndw->interlock.data;
}

void
nv50_wndw_flush_set(struct nv50_wndw *wndw, u32 *interlock,
		    struct nv50_wndw_atom *asyw)
{
<<<<<<< HEAD
	if (interlock) {
		asyw->image.mode = 0;
=======
	if (interlock[NV50_DISP_INTERLOCK_CORE]) {
		asyw->image.mode = NV507C_SET_PRESENT_CONTROL_BEGIN_MODE_NON_TEARING;
>>>>>>> upstream/android-13
		asyw->image.interval = 1;
	}

	if (asyw->set.sema ) wndw->func->sema_set (wndw, asyw);
	if (asyw->set.ntfy ) wndw->func->ntfy_set (wndw, asyw);
	if (asyw->set.image) wndw->func->image_set(wndw, asyw);

	if (asyw->set.xlut ) {
		if (asyw->ilut) {
			asyw->xlut.i.offset =
<<<<<<< HEAD
				nv50_lut_load(&wndw->ilut,
					      asyw->xlut.i.mode <= 1,
					      asyw->xlut.i.buffer,
					      asyw->ilut);
=======
				nv50_lut_load(&wndw->ilut, asyw->xlut.i.buffer,
					      asyw->ilut, asyw->xlut.i.load);
>>>>>>> upstream/android-13
		}
		wndw->func->xlut_set(wndw, asyw);
	}

<<<<<<< HEAD
	if (asyw->set.scale) wndw->func->scale_set(wndw, asyw);
=======
	if (asyw->set.csc  ) wndw->func->csc_set  (wndw, asyw);
	if (asyw->set.scale) wndw->func->scale_set(wndw, asyw);
	if (asyw->set.blend) wndw->func->blend_set(wndw, asyw);
>>>>>>> upstream/android-13
	if (asyw->set.point) {
		if (asyw->set.point = false, asyw->set.mask)
			interlock[wndw->interlock.type] |= wndw->interlock.data;
		interlock[NV50_DISP_INTERLOCK_WIMM] |= wndw->interlock.wimm;

		wndw->immd->point(wndw, asyw);
		wndw->immd->update(wndw, interlock);
	} else {
		interlock[wndw->interlock.type] |= wndw->interlock.data;
	}
}

void
nv50_wndw_ntfy_enable(struct nv50_wndw *wndw, struct nv50_wndw_atom *asyw)
{
	struct nv50_disp *disp = nv50_disp(wndw->plane.dev);

	asyw->ntfy.handle = wndw->wndw.sync.handle;
	asyw->ntfy.offset = wndw->ntfy;
	asyw->ntfy.awaken = false;
	asyw->set.ntfy = true;

	wndw->func->ntfy_reset(disp->sync, wndw->ntfy);
	wndw->ntfy ^= 0x10;
}

static void
nv50_wndw_atomic_check_release(struct nv50_wndw *wndw,
			       struct nv50_wndw_atom *asyw,
			       struct nv50_head_atom *asyh)
{
	struct nouveau_drm *drm = nouveau_drm(wndw->plane.dev);
	NV_ATOMIC(drm, "%s release\n", wndw->plane.name);
	wndw->func->release(wndw, asyw, asyh);
	asyw->ntfy.handle = 0;
	asyw->sema.handle = 0;
<<<<<<< HEAD
=======
	asyw->xlut.handle = 0;
	memset(asyw->image.handle, 0x00, sizeof(asyw->image.handle));
>>>>>>> upstream/android-13
}

static int
nv50_wndw_atomic_check_acquire_yuv(struct nv50_wndw_atom *asyw)
{
	switch (asyw->state.fb->format->format) {
<<<<<<< HEAD
	case DRM_FORMAT_YUYV: asyw->image.format = 0x28; break;
	case DRM_FORMAT_UYVY: asyw->image.format = 0x29; break;
=======
	case DRM_FORMAT_YUYV:
		asyw->image.format = NV507E_SURFACE_SET_PARAMS_FORMAT_VE8YO8UE8YE8;
		break;
	case DRM_FORMAT_UYVY:
		asyw->image.format = NV507E_SURFACE_SET_PARAMS_FORMAT_YO8VE8YE8UE8;
		break;
>>>>>>> upstream/android-13
	default:
		WARN_ON(1);
		return -EINVAL;
	}
<<<<<<< HEAD
	asyw->image.colorspace = 1;
=======

	asyw->image.colorspace = NV507E_SURFACE_SET_PARAMS_COLOR_SPACE_YUV_601;
>>>>>>> upstream/android-13
	return 0;
}

static int
nv50_wndw_atomic_check_acquire_rgb(struct nv50_wndw_atom *asyw)
{
	switch (asyw->state.fb->format->format) {
<<<<<<< HEAD
	case DRM_FORMAT_C8         : asyw->image.format = 0x1e; break;
	case DRM_FORMAT_XRGB8888   :
	case DRM_FORMAT_ARGB8888   : asyw->image.format = 0xcf; break;
	case DRM_FORMAT_RGB565     : asyw->image.format = 0xe8; break;
	case DRM_FORMAT_XRGB1555   :
	case DRM_FORMAT_ARGB1555   : asyw->image.format = 0xe9; break;
	case DRM_FORMAT_XBGR2101010:
	case DRM_FORMAT_ABGR2101010: asyw->image.format = 0xd1; break;
	case DRM_FORMAT_XBGR8888   :
	case DRM_FORMAT_ABGR8888   : asyw->image.format = 0xd5; break;
	case DRM_FORMAT_XRGB2101010:
	case DRM_FORMAT_ARGB2101010: asyw->image.format = 0xdf; break;
	default:
		return -EINVAL;
	}
	asyw->image.colorspace = 0;
=======
	case DRM_FORMAT_C8:
		asyw->image.format = NV507C_SURFACE_SET_PARAMS_FORMAT_I8;
		break;
	case DRM_FORMAT_XRGB8888:
	case DRM_FORMAT_ARGB8888:
		asyw->image.format = NV507C_SURFACE_SET_PARAMS_FORMAT_A8R8G8B8;
		break;
	case DRM_FORMAT_RGB565:
		asyw->image.format = NV507C_SURFACE_SET_PARAMS_FORMAT_R5G6B5;
		break;
	case DRM_FORMAT_XRGB1555:
	case DRM_FORMAT_ARGB1555:
		asyw->image.format = NV507C_SURFACE_SET_PARAMS_FORMAT_A1R5G5B5;
		break;
	case DRM_FORMAT_XBGR2101010:
	case DRM_FORMAT_ABGR2101010:
		asyw->image.format = NV507C_SURFACE_SET_PARAMS_FORMAT_A2B10G10R10;
		break;
	case DRM_FORMAT_XBGR8888:
	case DRM_FORMAT_ABGR8888:
		asyw->image.format = NV507C_SURFACE_SET_PARAMS_FORMAT_A8B8G8R8;
		break;
	case DRM_FORMAT_XRGB2101010:
	case DRM_FORMAT_ARGB2101010:
		asyw->image.format = NVC37E_SET_PARAMS_FORMAT_A2R10G10B10;
		break;
	case DRM_FORMAT_XBGR16161616F:
	case DRM_FORMAT_ABGR16161616F:
		asyw->image.format = NV507C_SURFACE_SET_PARAMS_FORMAT_RF16_GF16_BF16_AF16;
		break;
	default:
		return -EINVAL;
	}

	asyw->image.colorspace = NV507E_SURFACE_SET_PARAMS_COLOR_SPACE_RGB;
>>>>>>> upstream/android-13
	return 0;
}

static int
nv50_wndw_atomic_check_acquire(struct nv50_wndw *wndw, bool modeset,
			       struct nv50_wndw_atom *armw,
			       struct nv50_wndw_atom *asyw,
			       struct nv50_head_atom *asyh)
{
<<<<<<< HEAD
	struct nouveau_framebuffer *fb = nouveau_framebuffer(asyw->state.fb);
	struct nouveau_drm *drm = nouveau_drm(wndw->plane.dev);
=======
	struct drm_framebuffer *fb = asyw->state.fb;
	struct nouveau_drm *drm = nouveau_drm(wndw->plane.dev);
	uint8_t kind;
	uint32_t tile_mode;
>>>>>>> upstream/android-13
	int ret;

	NV_ATOMIC(drm, "%s acquire\n", wndw->plane.name);

<<<<<<< HEAD
	if (asyw->state.fb != armw->state.fb || !armw->visible || modeset) {
		asyw->image.w = fb->base.width;
		asyw->image.h = fb->base.height;
		asyw->image.kind = fb->nvbo->kind;
=======
	if (fb != armw->state.fb || !armw->visible || modeset) {
		nouveau_framebuffer_get_layout(fb, &tile_mode, &kind);

		asyw->image.w = fb->width;
		asyw->image.h = fb->height;
		asyw->image.kind = kind;
>>>>>>> upstream/android-13

		ret = nv50_wndw_atomic_check_acquire_rgb(asyw);
		if (ret) {
			ret = nv50_wndw_atomic_check_acquire_yuv(asyw);
			if (ret)
				return ret;
		}

		if (asyw->image.kind) {
<<<<<<< HEAD
			asyw->image.layout = 0;
			if (drm->client.device.info.chipset >= 0xc0)
				asyw->image.blockh = fb->nvbo->mode >> 4;
			else
				asyw->image.blockh = fb->nvbo->mode;
			asyw->image.blocks[0] = fb->base.pitches[0] / 64;
			asyw->image.pitch[0] = 0;
		} else {
			asyw->image.layout = 1;
			asyw->image.blockh = 0;
			asyw->image.blocks[0] = 0;
			asyw->image.pitch[0] = fb->base.pitches[0];
		}

		if (!(asyh->state.pageflip_flags & DRM_MODE_PAGE_FLIP_ASYNC))
			asyw->image.interval = 1;
		else
			asyw->image.interval = 0;
		asyw->image.mode = asyw->image.interval ? 0 : 1;
=======
			asyw->image.layout = NV507C_SURFACE_SET_STORAGE_MEMORY_LAYOUT_BLOCKLINEAR;
			if (drm->client.device.info.chipset >= 0xc0)
				asyw->image.blockh = tile_mode >> 4;
			else
				asyw->image.blockh = tile_mode;
			asyw->image.blocks[0] = fb->pitches[0] / 64;
			asyw->image.pitch[0] = 0;
		} else {
			asyw->image.layout = NV507C_SURFACE_SET_STORAGE_MEMORY_LAYOUT_PITCH;
			asyw->image.blockh = NV507C_SURFACE_SET_STORAGE_BLOCK_HEIGHT_ONE_GOB;
			asyw->image.blocks[0] = 0;
			asyw->image.pitch[0] = fb->pitches[0];
		}

		if (!asyh->state.async_flip)
			asyw->image.interval = 1;
		else
			asyw->image.interval = 0;

		if (asyw->image.interval)
			asyw->image.mode = NV507C_SET_PRESENT_CONTROL_BEGIN_MODE_NON_TEARING;
		else
			asyw->image.mode = NV507C_SET_PRESENT_CONTROL_BEGIN_MODE_IMMEDIATE;

>>>>>>> upstream/android-13
		asyw->set.image = wndw->func->image_set != NULL;
	}

	if (wndw->func->scale_set) {
		asyw->scale.sx = asyw->state.src_x >> 16;
		asyw->scale.sy = asyw->state.src_y >> 16;
		asyw->scale.sw = asyw->state.src_w >> 16;
		asyw->scale.sh = asyw->state.src_h >> 16;
		asyw->scale.dw = asyw->state.crtc_w;
		asyw->scale.dh = asyw->state.crtc_h;
		if (memcmp(&armw->scale, &asyw->scale, sizeof(asyw->scale)))
			asyw->set.scale = true;
	}

<<<<<<< HEAD
=======
	if (wndw->func->blend_set) {
		asyw->blend.depth = 255 - asyw->state.normalized_zpos;
		asyw->blend.k1 = asyw->state.alpha >> 8;
		switch (asyw->state.pixel_blend_mode) {
		case DRM_MODE_BLEND_PREMULTI:
			asyw->blend.src_color = NVC37E_SET_COMPOSITION_FACTOR_SELECT_SRC_COLOR_FACTOR_MATCH_SELECT_K1;
			asyw->blend.dst_color = NVC37E_SET_COMPOSITION_FACTOR_SELECT_DST_COLOR_FACTOR_MATCH_SELECT_NEG_K1_TIMES_SRC;
			break;
		case DRM_MODE_BLEND_COVERAGE:
			asyw->blend.src_color = NVC37E_SET_COMPOSITION_FACTOR_SELECT_SRC_COLOR_FACTOR_MATCH_SELECT_K1_TIMES_SRC;
			asyw->blend.dst_color = NVC37E_SET_COMPOSITION_FACTOR_SELECT_DST_COLOR_FACTOR_MATCH_SELECT_NEG_K1_TIMES_SRC;
			break;
		case DRM_MODE_BLEND_PIXEL_NONE:
		default:
			asyw->blend.src_color = NVC37E_SET_COMPOSITION_FACTOR_SELECT_SRC_COLOR_FACTOR_MATCH_SELECT_K1;
			asyw->blend.dst_color = NVC37E_SET_COMPOSITION_FACTOR_SELECT_DST_COLOR_FACTOR_MATCH_SELECT_NEG_K1;
			break;
		}
		if (memcmp(&armw->blend, &asyw->blend, sizeof(asyw->blend)))
			asyw->set.blend = true;
	}

>>>>>>> upstream/android-13
	if (wndw->immd) {
		asyw->point.x = asyw->state.crtc_x;
		asyw->point.y = asyw->state.crtc_y;
		if (memcmp(&armw->point, &asyw->point, sizeof(asyw->point)))
			asyw->set.point = true;
	}

	return wndw->func->acquire(wndw, asyw, asyh);
}

<<<<<<< HEAD
static void
=======
static int
>>>>>>> upstream/android-13
nv50_wndw_atomic_check_lut(struct nv50_wndw *wndw,
			   struct nv50_wndw_atom *armw,
			   struct nv50_wndw_atom *asyw,
			   struct nv50_head_atom *asyh)
{
	struct drm_property_blob *ilut = asyh->state.degamma_lut;

	/* I8 format without an input LUT makes no sense, and the
	 * HW error-checks for this.
	 *
	 * In order to handle legacy gamma, when there's no input
	 * LUT we need to steal the output LUT and use it instead.
	 */
	if (!ilut && asyw->state.fb->format->format == DRM_FORMAT_C8) {
		/* This should be an error, but there's legacy clients
		 * that do a modeset before providing a gamma table.
		 *
		 * We keep the window disabled to avoid angering HW.
		 */
		if (!(ilut = asyh->state.gamma_lut)) {
			asyw->visible = false;
<<<<<<< HEAD
			return;
=======
			return 0;
>>>>>>> upstream/android-13
		}

		if (wndw->func->ilut)
			asyh->wndw.olut |= BIT(wndw->id);
	} else {
		asyh->wndw.olut &= ~BIT(wndw->id);
	}

<<<<<<< HEAD
	/* Recalculate LUT state. */
	memset(&asyw->xlut, 0x00, sizeof(asyw->xlut));
	if ((asyw->ilut = wndw->func->ilut ? ilut : NULL)) {
		wndw->func->ilut(wndw, asyw);
		asyw->xlut.handle = wndw->wndw.vram.handle;
		asyw->xlut.i.buffer = !asyw->xlut.i.buffer;
		asyw->set.xlut = true;
=======
	if (!ilut && wndw->func->ilut_identity &&
	    asyw->state.fb->format->format != DRM_FORMAT_XBGR16161616F &&
	    asyw->state.fb->format->format != DRM_FORMAT_ABGR16161616F) {
		static struct drm_property_blob dummy = {};
		ilut = &dummy;
	}

	/* Recalculate LUT state. */
	memset(&asyw->xlut, 0x00, sizeof(asyw->xlut));
	if ((asyw->ilut = wndw->func->ilut ? ilut : NULL)) {
		if (!wndw->func->ilut(wndw, asyw, drm_color_lut_size(ilut))) {
			DRM_DEBUG_KMS("Invalid ilut\n");
			return -EINVAL;
		}
		asyw->xlut.handle = wndw->wndw.vram.handle;
		asyw->xlut.i.buffer = !asyw->xlut.i.buffer;
		asyw->set.xlut = true;
	} else {
		asyw->clr.xlut = armw->xlut.handle != 0;
>>>>>>> upstream/android-13
	}

	/* Handle setting base SET_OUTPUT_LUT_LO_ENABLE_USE_CORE_LUT. */
	if (wndw->func->olut_core &&
	    (!armw->visible || (armw->xlut.handle && !asyw->xlut.handle)))
		asyw->set.xlut = true;

<<<<<<< HEAD
	/* Can't do an immediate flip while changing the LUT. */
	asyh->state.pageflip_flags &= ~DRM_MODE_PAGE_FLIP_ASYNC;
}

static int
nv50_wndw_atomic_check(struct drm_plane *plane, struct drm_plane_state *state)
{
	struct nouveau_drm *drm = nouveau_drm(plane->dev);
	struct nv50_wndw *wndw = nv50_wndw(plane);
	struct nv50_wndw_atom *armw = nv50_wndw_atom(wndw->plane.state);
	struct nv50_wndw_atom *asyw = nv50_wndw_atom(state);
=======
	if (wndw->func->csc && asyh->state.ctm) {
		const struct drm_color_ctm *ctm = asyh->state.ctm->data;
		wndw->func->csc(wndw, asyw, ctm);
		asyw->csc.valid = true;
		asyw->set.csc = true;
	} else {
		asyw->csc.valid = false;
		asyw->clr.csc = armw->csc.valid;
	}

	/* Can't do an immediate flip while changing the LUT. */
	asyh->state.async_flip = false;
	return 0;
}

static int
nv50_wndw_atomic_check(struct drm_plane *plane,
		       struct drm_atomic_state *state)
{
	struct drm_plane_state *new_plane_state = drm_atomic_get_new_plane_state(state,
										 plane);
	struct nouveau_drm *drm = nouveau_drm(plane->dev);
	struct nv50_wndw *wndw = nv50_wndw(plane);
	struct nv50_wndw_atom *armw = nv50_wndw_atom(wndw->plane.state);
	struct nv50_wndw_atom *asyw = nv50_wndw_atom(new_plane_state);
>>>>>>> upstream/android-13
	struct nv50_head_atom *harm = NULL, *asyh = NULL;
	bool modeset = false;
	int ret;

	NV_ATOMIC(drm, "%s atomic_check\n", plane->name);

	/* Fetch the assembly state for the head the window will belong to,
	 * and determine whether the window will be visible.
	 */
	if (asyw->state.crtc) {
		asyh = nv50_head_atom_get(asyw->state.state, asyw->state.crtc);
		if (IS_ERR(asyh))
			return PTR_ERR(asyh);
		modeset = drm_atomic_crtc_needs_modeset(&asyh->state);
		asyw->visible = asyh->state.active;
	} else {
		asyw->visible = false;
	}

	/* Fetch assembly state for the head the window used to belong to. */
	if (armw->state.crtc) {
		harm = nv50_head_atom_get(asyw->state.state, armw->state.crtc);
		if (IS_ERR(harm))
			return PTR_ERR(harm);
	}

	/* LUT configuration can potentially cause the window to be disabled. */
	if (asyw->visible && wndw->func->xlut_set &&
	    (!armw->visible ||
	     asyh->state.color_mgmt_changed ||
	     asyw->state.fb->format->format !=
<<<<<<< HEAD
	     armw->state.fb->format->format))
		nv50_wndw_atomic_check_lut(wndw, armw, asyw, asyh);
=======
	     armw->state.fb->format->format)) {
		ret = nv50_wndw_atomic_check_lut(wndw, armw, asyw, asyh);
		if (ret)
			return ret;
	}
>>>>>>> upstream/android-13

	/* Calculate new window state. */
	if (asyw->visible) {
		ret = nv50_wndw_atomic_check_acquire(wndw, modeset,
						     armw, asyw, asyh);
		if (ret)
			return ret;

		asyh->wndw.mask |= BIT(wndw->id);
	} else
	if (armw->visible) {
		nv50_wndw_atomic_check_release(wndw, asyw, harm);
		harm->wndw.mask &= ~BIT(wndw->id);
	} else {
		return 0;
	}

	/* Aside from the obvious case where the window is actively being
	 * disabled, we might also need to temporarily disable the window
	 * when performing certain modeset operations.
	 */
	if (!asyw->visible || modeset) {
		asyw->clr.ntfy = armw->ntfy.handle != 0;
		asyw->clr.sema = armw->sema.handle != 0;
		asyw->clr.xlut = armw->xlut.handle != 0;
		if (asyw->clr.xlut && asyw->visible)
			asyw->set.xlut = asyw->xlut.handle != 0;
<<<<<<< HEAD
=======
		asyw->clr.csc  = armw->csc.valid;
>>>>>>> upstream/android-13
		if (wndw->func->image_clr)
			asyw->clr.image = armw->image.handle[0] != 0;
	}

	return 0;
}

static void
nv50_wndw_cleanup_fb(struct drm_plane *plane, struct drm_plane_state *old_state)
{
<<<<<<< HEAD
	struct nouveau_framebuffer *fb = nouveau_framebuffer(old_state->fb);
	struct nouveau_drm *drm = nouveau_drm(plane->dev);
=======
	struct nouveau_drm *drm = nouveau_drm(plane->dev);
	struct nouveau_bo *nvbo;
>>>>>>> upstream/android-13

	NV_ATOMIC(drm, "%s cleanup: %p\n", plane->name, old_state->fb);
	if (!old_state->fb)
		return;

<<<<<<< HEAD
	nouveau_bo_unpin(fb->nvbo);
=======
	nvbo = nouveau_gem_object(old_state->fb->obj[0]);
	nouveau_bo_unpin(nvbo);
>>>>>>> upstream/android-13
}

static int
nv50_wndw_prepare_fb(struct drm_plane *plane, struct drm_plane_state *state)
{
<<<<<<< HEAD
	struct nouveau_framebuffer *fb = nouveau_framebuffer(state->fb);
	struct nouveau_drm *drm = nouveau_drm(plane->dev);
	struct nv50_wndw *wndw = nv50_wndw(plane);
	struct nv50_wndw_atom *asyw = nv50_wndw_atom(state);
=======
	struct drm_framebuffer *fb = state->fb;
	struct nouveau_drm *drm = nouveau_drm(plane->dev);
	struct nv50_wndw *wndw = nv50_wndw(plane);
	struct nv50_wndw_atom *asyw = nv50_wndw_atom(state);
	struct nouveau_bo *nvbo;
>>>>>>> upstream/android-13
	struct nv50_head_atom *asyh;
	struct nv50_wndw_ctxdma *ctxdma;
	int ret;

<<<<<<< HEAD
	NV_ATOMIC(drm, "%s prepare: %p\n", plane->name, state->fb);
	if (!asyw->state.fb)
		return 0;

	ret = nouveau_bo_pin(fb->nvbo, TTM_PL_FLAG_VRAM, true);
=======
	NV_ATOMIC(drm, "%s prepare: %p\n", plane->name, fb);
	if (!asyw->state.fb)
		return 0;

	nvbo = nouveau_gem_object(fb->obj[0]);
	ret = nouveau_bo_pin(nvbo, NOUVEAU_GEM_DOMAIN_VRAM, true);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	if (wndw->ctxdma.parent) {
		ctxdma = nv50_wndw_ctxdma_new(wndw, fb);
		if (IS_ERR(ctxdma)) {
<<<<<<< HEAD
			nouveau_bo_unpin(fb->nvbo);
			return PTR_ERR(ctxdma);
		}

		asyw->image.handle[0] = ctxdma->object.handle;
	}

	asyw->state.fence = reservation_object_get_excl_rcu(fb->nvbo->bo.resv);
	asyw->image.offset[0] = fb->nvbo->bo.offset;
=======
			nouveau_bo_unpin(nvbo);
			return PTR_ERR(ctxdma);
		}

		if (asyw->visible)
			asyw->image.handle[0] = ctxdma->object.handle;
	}

	asyw->state.fence = dma_resv_get_excl_unlocked(nvbo->bo.base.resv);
	asyw->image.offset[0] = nvbo->offset;
>>>>>>> upstream/android-13

	if (wndw->func->prepare) {
		asyh = nv50_head_atom_get(asyw->state.state, asyw->state.crtc);
		if (IS_ERR(asyh))
			return PTR_ERR(asyh);

		wndw->func->prepare(wndw, asyh, asyw);
	}

	return 0;
}

static const struct drm_plane_helper_funcs
nv50_wndw_helper = {
	.prepare_fb = nv50_wndw_prepare_fb,
	.cleanup_fb = nv50_wndw_cleanup_fb,
	.atomic_check = nv50_wndw_atomic_check,
};

static void
nv50_wndw_atomic_destroy_state(struct drm_plane *plane,
			       struct drm_plane_state *state)
{
	struct nv50_wndw_atom *asyw = nv50_wndw_atom(state);
	__drm_atomic_helper_plane_destroy_state(&asyw->state);
	kfree(asyw);
}

static struct drm_plane_state *
nv50_wndw_atomic_duplicate_state(struct drm_plane *plane)
{
	struct nv50_wndw_atom *armw = nv50_wndw_atom(plane->state);
	struct nv50_wndw_atom *asyw;
	if (!(asyw = kmalloc(sizeof(*asyw), GFP_KERNEL)))
		return NULL;
	__drm_atomic_helper_plane_duplicate_state(plane, &asyw->state);
	asyw->sema = armw->sema;
	asyw->ntfy = armw->ntfy;
	asyw->ilut = NULL;
	asyw->xlut = armw->xlut;
<<<<<<< HEAD
=======
	asyw->csc  = armw->csc;
>>>>>>> upstream/android-13
	asyw->image = armw->image;
	asyw->point = armw->point;
	asyw->clr.mask = 0;
	asyw->set.mask = 0;
	return &asyw->state;
}

<<<<<<< HEAD
=======
static int
nv50_wndw_zpos_default(struct drm_plane *plane)
{
	return (plane->type == DRM_PLANE_TYPE_PRIMARY) ? 0 :
	       (plane->type == DRM_PLANE_TYPE_OVERLAY) ? 1 : 255;
}

>>>>>>> upstream/android-13
static void
nv50_wndw_reset(struct drm_plane *plane)
{
	struct nv50_wndw_atom *asyw;

	if (WARN_ON(!(asyw = kzalloc(sizeof(*asyw), GFP_KERNEL))))
		return;

	if (plane->state)
		plane->funcs->atomic_destroy_state(plane, plane->state);
<<<<<<< HEAD
	plane->state = &asyw->state;
	plane->state->plane = plane;
	plane->state->rotation = DRM_MODE_ROTATE_0;
=======

	__drm_atomic_helper_plane_reset(plane, &asyw->state);
	plane->state->zpos = nv50_wndw_zpos_default(plane);
	plane->state->normalized_zpos = nv50_wndw_zpos_default(plane);
>>>>>>> upstream/android-13
}

static void
nv50_wndw_destroy(struct drm_plane *plane)
{
	struct nv50_wndw *wndw = nv50_wndw(plane);
	struct nv50_wndw_ctxdma *ctxdma, *ctxtmp;

	list_for_each_entry_safe(ctxdma, ctxtmp, &wndw->ctxdma.list, head) {
		nv50_wndw_ctxdma_del(ctxdma);
	}

<<<<<<< HEAD
	nvif_notify_fini(&wndw->notify);
=======
	nvif_notify_dtor(&wndw->notify);
>>>>>>> upstream/android-13
	nv50_dmac_destroy(&wndw->wimm);
	nv50_dmac_destroy(&wndw->wndw);

	nv50_lut_fini(&wndw->ilut);

	drm_plane_cleanup(&wndw->plane);
	kfree(wndw);
}

<<<<<<< HEAD
=======
/* This function assumes the format has already been validated against the plane
 * and the modifier was validated against the device-wides modifier list at FB
 * creation time.
 */
static bool nv50_plane_format_mod_supported(struct drm_plane *plane,
					    u32 format, u64 modifier)
{
	struct nouveau_drm *drm = nouveau_drm(plane->dev);
	uint8_t i;

	if (drm->client.device.info.chipset < 0xc0) {
		const struct drm_format_info *info = drm_format_info(format);
		const uint8_t kind = (modifier >> 12) & 0xff;

		if (!format) return false;

		for (i = 0; i < info->num_planes; i++)
			if ((info->cpp[i] != 4) && kind != 0x70) return false;
	}

	return true;
}

>>>>>>> upstream/android-13
const struct drm_plane_funcs
nv50_wndw = {
	.update_plane = drm_atomic_helper_update_plane,
	.disable_plane = drm_atomic_helper_disable_plane,
	.destroy = nv50_wndw_destroy,
	.reset = nv50_wndw_reset,
	.atomic_duplicate_state = nv50_wndw_atomic_duplicate_state,
	.atomic_destroy_state = nv50_wndw_atomic_destroy_state,
<<<<<<< HEAD
=======
	.format_mod_supported = nv50_plane_format_mod_supported,
>>>>>>> upstream/android-13
};

static int
nv50_wndw_notify(struct nvif_notify *notify)
{
	return NVIF_NOTIFY_KEEP;
}

void
nv50_wndw_fini(struct nv50_wndw *wndw)
{
	nvif_notify_put(&wndw->notify);
}

void
nv50_wndw_init(struct nv50_wndw *wndw)
{
	nvif_notify_get(&wndw->notify);
}

<<<<<<< HEAD
=======
static const u64 nv50_cursor_format_modifiers[] = {
	DRM_FORMAT_MOD_LINEAR,
	DRM_FORMAT_MOD_INVALID,
};

>>>>>>> upstream/android-13
int
nv50_wndw_new_(const struct nv50_wndw_func *func, struct drm_device *dev,
	       enum drm_plane_type type, const char *name, int index,
	       const u32 *format, u32 heads,
	       enum nv50_disp_interlock_type interlock_type, u32 interlock_data,
	       struct nv50_wndw **pwndw)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nvif_mmu *mmu = &drm->client.mmu;
	struct nv50_disp *disp = nv50_disp(dev);
	struct nv50_wndw *wndw;
<<<<<<< HEAD
=======
	const u64 *format_modifiers;
>>>>>>> upstream/android-13
	int nformat;
	int ret;

	if (!(wndw = *pwndw = kzalloc(sizeof(*wndw), GFP_KERNEL)))
		return -ENOMEM;
	wndw->func = func;
	wndw->id = index;
	wndw->interlock.type = interlock_type;
	wndw->interlock.data = interlock_data;

	wndw->ctxdma.parent = &wndw->wndw.base.user;
	INIT_LIST_HEAD(&wndw->ctxdma.list);

	for (nformat = 0; format[nformat]; nformat++);

<<<<<<< HEAD
	ret = drm_universal_plane_init(dev, &wndw->plane, heads, &nv50_wndw,
				       format, nformat, NULL,
				       type, "%s-%d", name, index);
=======
	if (type == DRM_PLANE_TYPE_CURSOR)
		format_modifiers = nv50_cursor_format_modifiers;
	else
		format_modifiers = nouveau_display(dev)->format_modifiers;

	ret = drm_universal_plane_init(dev, &wndw->plane, heads, &nv50_wndw, format, nformat,
				       format_modifiers, type, "%s-%d", name, index);
>>>>>>> upstream/android-13
	if (ret) {
		kfree(*pwndw);
		*pwndw = NULL;
		return ret;
	}

	drm_plane_helper_add(&wndw->plane, &nv50_wndw_helper);

	if (wndw->func->ilut) {
		ret = nv50_lut_init(disp, mmu, &wndw->ilut);
		if (ret)
			return ret;
	}

	wndw->notify.func = nv50_wndw_notify;
<<<<<<< HEAD
=======

	if (wndw->func->blend_set) {
		ret = drm_plane_create_zpos_property(&wndw->plane,
				nv50_wndw_zpos_default(&wndw->plane), 0, 254);
		if (ret)
			return ret;

		ret = drm_plane_create_alpha_property(&wndw->plane);
		if (ret)
			return ret;

		ret = drm_plane_create_blend_mode_property(&wndw->plane,
				BIT(DRM_MODE_BLEND_PIXEL_NONE) |
				BIT(DRM_MODE_BLEND_PREMULTI) |
				BIT(DRM_MODE_BLEND_COVERAGE));
		if (ret)
			return ret;
	} else {
		ret = drm_plane_create_zpos_immutable_property(&wndw->plane,
				nv50_wndw_zpos_default(&wndw->plane));
		if (ret)
			return ret;
	}

>>>>>>> upstream/android-13
	return 0;
}

int
nv50_wndw_new(struct nouveau_drm *drm, enum drm_plane_type type, int index,
	      struct nv50_wndw **pwndw)
{
	struct {
		s32 oclass;
		int version;
		int (*new)(struct nouveau_drm *, enum drm_plane_type,
			   int, s32, struct nv50_wndw **);
	} wndws[] = {
<<<<<<< HEAD
=======
		{ GA102_DISP_WINDOW_CHANNEL_DMA, 0, wndwc67e_new },
		{ TU102_DISP_WINDOW_CHANNEL_DMA, 0, wndwc57e_new },
>>>>>>> upstream/android-13
		{ GV100_DISP_WINDOW_CHANNEL_DMA, 0, wndwc37e_new },
		{}
	};
	struct nv50_disp *disp = nv50_disp(drm->dev);
	int cid, ret;

	cid = nvif_mclass(&disp->disp->object, wndws);
	if (cid < 0) {
		NV_ERROR(drm, "No supported window class\n");
		return cid;
	}

	ret = wndws[cid].new(drm, type, index, wndws[cid].oclass, pwndw);
	if (ret)
		return ret;

	return nv50_wimm_init(drm, *pwndw);
}
