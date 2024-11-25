<<<<<<< HEAD
/*
 * Copyright 2010 Matt Turner.
 * Copyright 2012 Red Hat 
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License version 2. See the file COPYING in the main
 * directory of this archive for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2010 Matt Turner.
 * Copyright 2012 Red Hat
>>>>>>> upstream/android-13
 *
 * Authors: Matthew Garrett
 * 	    Matt Turner
 *	    Dave Airlie
 */
#ifndef __MGAG200_DRV_H__
#define __MGAG200_DRV_H__

<<<<<<< HEAD
=======
#include <linux/i2c-algo-bit.h>
#include <linux/i2c.h>

>>>>>>> upstream/android-13
#include <video/vga.h>

#include <drm/drm_encoder.h>
#include <drm/drm_fb_helper.h>
<<<<<<< HEAD
#include <drm/ttm/ttm_bo_api.h>
#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_memory.h>
#include <drm/ttm/ttm_module.h>

#include <drm/drm_gem.h>

#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
=======
#include <drm/drm_gem.h>
#include <drm/drm_gem_shmem_helper.h>
#include <drm/drm_simple_kms_helper.h>
>>>>>>> upstream/android-13

#include "mgag200_reg.h"

#define DRIVER_AUTHOR		"Matthew Garrett"

#define DRIVER_NAME		"mgag200"
#define DRIVER_DESC		"MGA G200 SE"
#define DRIVER_DATE		"20110418"

#define DRIVER_MAJOR		1
#define DRIVER_MINOR		0
#define DRIVER_PATCHLEVEL	0

<<<<<<< HEAD
#define MGAG200FB_CONN_LIMIT 1

=======
>>>>>>> upstream/android-13
#define RREG8(reg) ioread8(((void __iomem *)mdev->rmmio) + (reg))
#define WREG8(reg, v) iowrite8(v, ((void __iomem *)mdev->rmmio) + (reg))
#define RREG32(reg) ioread32(((void __iomem *)mdev->rmmio) + (reg))
#define WREG32(reg, v) iowrite32(v, ((void __iomem *)mdev->rmmio) + (reg))

<<<<<<< HEAD
#define ATTR_INDEX 0x1fc0
#define ATTR_DATA 0x1fc1

=======
#define MGA_BIOS_OFFSET		0x7ffc

#define ATTR_INDEX 0x1fc0
#define ATTR_DATA 0x1fc1

#define WREG_MISC(v)						\
	WREG8(MGA_MISC_OUT, v)

#define RREG_MISC(v)						\
	((v) = RREG8(MGA_MISC_IN))

#define WREG_MISC_MASKED(v, mask)				\
	do {							\
		u8 misc_;					\
		u8 mask_ = (mask);				\
		RREG_MISC(misc_);				\
		misc_ &= ~mask_;				\
		misc_ |= ((v) & mask_);				\
		WREG_MISC(misc_);				\
	} while (0)

>>>>>>> upstream/android-13
#define WREG_ATTR(reg, v)					\
	do {							\
		RREG8(0x1fda);					\
		WREG8(ATTR_INDEX, reg);				\
		WREG8(ATTR_DATA, v);				\
	} while (0)						\

<<<<<<< HEAD
=======
#define RREG_SEQ(reg, v)					\
	do {							\
		WREG8(MGAREG_SEQ_INDEX, reg);			\
		v = RREG8(MGAREG_SEQ_DATA);			\
	} while (0)						\

>>>>>>> upstream/android-13
#define WREG_SEQ(reg, v)					\
	do {							\
		WREG8(MGAREG_SEQ_INDEX, reg);			\
		WREG8(MGAREG_SEQ_DATA, v);			\
	} while (0)						\

<<<<<<< HEAD
=======
#define RREG_CRT(reg, v)					\
	do {							\
		WREG8(MGAREG_CRTC_INDEX, reg);			\
		v = RREG8(MGAREG_CRTC_DATA);			\
	} while (0)						\

>>>>>>> upstream/android-13
#define WREG_CRT(reg, v)					\
	do {							\
		WREG8(MGAREG_CRTC_INDEX, reg);			\
		WREG8(MGAREG_CRTC_DATA, v);			\
	} while (0)						\

<<<<<<< HEAD
=======
#define RREG_ECRT(reg, v)					\
	do {							\
		WREG8(MGAREG_CRTCEXT_INDEX, reg);		\
		v = RREG8(MGAREG_CRTCEXT_DATA);			\
	} while (0)						\
>>>>>>> upstream/android-13

#define WREG_ECRT(reg, v)					\
	do {							\
		WREG8(MGAREG_CRTCEXT_INDEX, reg);				\
		WREG8(MGAREG_CRTCEXT_DATA, v);				\
	} while (0)						\

#define GFX_INDEX 0x1fce
#define GFX_DATA 0x1fcf

#define WREG_GFX(reg, v)					\
	do {							\
		WREG8(GFX_INDEX, reg);				\
		WREG8(GFX_DATA, v);				\
	} while (0)						\

#define DAC_INDEX 0x3c00
#define DAC_DATA 0x3c0a

#define WREG_DAC(reg, v)					\
	do {							\
		WREG8(DAC_INDEX, reg);				\
		WREG8(DAC_DATA, v);				\
	} while (0)						\

#define MGA_MISC_OUT 0x1fc2
#define MGA_MISC_IN 0x1fcc

#define MGAG200_MAX_FB_HEIGHT 4096
#define MGAG200_MAX_FB_WIDTH 4096

<<<<<<< HEAD
#define MATROX_DPMS_CLEARED (-1)

#define to_mga_crtc(x) container_of(x, struct mga_crtc, base)
#define to_mga_encoder(x) container_of(x, struct mga_encoder, base)
#define to_mga_connector(x) container_of(x, struct mga_connector, base)
#define to_mga_framebuffer(x) container_of(x, struct mga_framebuffer, base)

struct mga_framebuffer {
	struct drm_framebuffer base;
	struct drm_gem_object *obj;
};

struct mga_fbdev {
	struct drm_fb_helper helper;
	struct mga_framebuffer mfb;
	void *sysram;
	int size;
	struct ttm_bo_kmap_obj mapping;
	int x1, y1, x2, y2; /* dirty rect */
	spinlock_t dirty_lock;
};

struct mga_crtc {
	struct drm_crtc base;
	u8 lut_r[256], lut_g[256], lut_b[256];
	int last_dpms;
	bool enabled;
};

struct mga_mode_info {
	bool mode_config_initialized;
	struct mga_crtc *crtc;
};

struct mga_encoder {
	struct drm_encoder base;
	int last_dpms;
};

=======
struct mga_device;
struct mgag200_pll;

/*
 * Stores parameters for programming the PLLs
 *
 * Fref: reference frequency (A: 25.175 Mhz, B: 28.361, C: XX Mhz)
 * Fo: output frequency
 * Fvco = Fref * (N / M)
 * Fo = Fvco / P
 *
 * S = [0..3]
 */
struct mgag200_pll_values {
	unsigned int m;
	unsigned int n;
	unsigned int p;
	unsigned int s;
};

struct mgag200_pll_funcs {
	int (*compute)(struct mgag200_pll *pll, long clock, struct mgag200_pll_values *pllc);
	void (*update)(struct mgag200_pll *pll, const struct mgag200_pll_values *pllc);
};

struct mgag200_pll {
	struct mga_device *mdev;

	const struct mgag200_pll_funcs *funcs;
};

struct mgag200_crtc_state {
	struct drm_crtc_state base;

	struct mgag200_pll_values pixpllc;
};

static inline struct mgag200_crtc_state *to_mgag200_crtc_state(struct drm_crtc_state *base)
{
	return container_of(base, struct mgag200_crtc_state, base);
}

#define to_mga_connector(x) container_of(x, struct mga_connector, base)
>>>>>>> upstream/android-13

struct mga_i2c_chan {
	struct i2c_adapter adapter;
	struct drm_device *dev;
	struct i2c_algo_bit_data bit;
	int data, clock;
};

struct mga_connector {
	struct drm_connector base;
	struct mga_i2c_chan *i2c;
};

<<<<<<< HEAD
struct mga_cursor {
	/*
	   We have to have 2 buffers for the cursor to avoid occasional
	   corruption while switching cursor icons.
	   If either of these is NULL, then don't do hardware cursors, and
	   fall back to software.
	*/
	struct mgag200_bo *pixels_1;
	struct mgag200_bo *pixels_2;
	u64 pixels_1_gpu_addr, pixels_2_gpu_addr;
	/* The currently displayed icon, this points to one of pixels_1, or pixels_2 */
	struct mgag200_bo *pixels_current;
	/* The previously displayed icon */
	struct mgag200_bo *pixels_prev;
};

=======
>>>>>>> upstream/android-13
struct mga_mc {
	resource_size_t			vram_size;
	resource_size_t			vram_base;
	resource_size_t			vram_window;
};

enum mga_type {
<<<<<<< HEAD
=======
	G200_PCI,
	G200_AGP,
>>>>>>> upstream/android-13
	G200_SE_A,
	G200_SE_B,
	G200_WB,
	G200_EV,
	G200_EH,
	G200_EH3,
	G200_ER,
	G200_EW3,
};

<<<<<<< HEAD
#define IS_G200_SE(mdev) (mdev->type == G200_SE_A || mdev->type == G200_SE_B)

struct mga_device {
	struct drm_device		*dev;
=======
/* HW does not handle 'startadd' field correct. */
#define MGAG200_FLAG_HW_BUG_NO_STARTADD	(1ul << 8)

#define MGAG200_TYPE_MASK	(0x000000ff)
#define MGAG200_FLAG_MASK	(0x00ffff00)

#define IS_G200_SE(mdev) (mdev->type == G200_SE_A || mdev->type == G200_SE_B)

struct mga_device {
	struct drm_device		base;
>>>>>>> upstream/android-13
	unsigned long			flags;

	resource_size_t			rmmio_base;
	resource_size_t			rmmio_size;
	void __iomem			*rmmio;

	struct mga_mc			mc;
<<<<<<< HEAD
	struct mga_mode_info		mode_info;

	struct mga_fbdev *mfbdev;
	struct mga_cursor cursor;

	bool				suspended;
	int				num_crtc;
	enum mga_type			type;
	int				has_sdram;
	struct drm_display_mode		mode;

	int bpp_shifts[4];

	int fb_mtrr;

	struct {
		struct drm_global_reference mem_global_ref;
		struct ttm_bo_global_ref bo_global_ref;
		struct ttm_bo_device bdev;
	} ttm;

	/* SE model number stored in reg 0x1e24 */
	u32 unique_rev_id;
};


struct mgag200_bo {
	struct ttm_buffer_object bo;
	struct ttm_placement placement;
	struct ttm_bo_kmap_obj kmap;
	struct drm_gem_object gem;
	struct ttm_place placements[3];
	int pin_count;
};
#define gem_to_mga_bo(gobj) container_of((gobj), struct mgag200_bo, gem)

static inline struct mgag200_bo *
mgag200_bo(struct ttm_buffer_object *bo)
{
	return container_of(bo, struct mgag200_bo, bo);
=======

	void __iomem			*vram;
	size_t				vram_fb_available;

	enum mga_type			type;

	int fb_mtrr;

	union {
		struct {
			long ref_clk;
			long pclk_min;
			long pclk_max;
		} g200;
		struct {
			/* SE model number stored in reg 0x1e24 */
			u32 unique_rev_id;
		} g200se;
	} model;

	struct mga_connector connector;
	struct mgag200_pll pixpll;
	struct drm_simple_display_pipe display_pipe;
};

static inline struct mga_device *to_mga_device(struct drm_device *dev)
{
	return container_of(dev, struct mga_device, base);
>>>>>>> upstream/android-13
}

				/* mgag200_mode.c */
int mgag200_modeset_init(struct mga_device *mdev);
<<<<<<< HEAD
void mgag200_modeset_fini(struct mga_device *mdev);

				/* mgag200_fb.c */
int mgag200_fbdev_init(struct mga_device *mdev);
void mgag200_fbdev_fini(struct mga_device *mdev);

				/* mgag200_main.c */
int mgag200_framebuffer_init(struct drm_device *dev,
			     struct mga_framebuffer *mfb,
			     const struct drm_mode_fb_cmd2 *mode_cmd,
			     struct drm_gem_object *obj);


int mgag200_driver_load(struct drm_device *dev, unsigned long flags);
void mgag200_driver_unload(struct drm_device *dev);
int mgag200_gem_create(struct drm_device *dev,
		   u32 size, bool iskernel,
		       struct drm_gem_object **obj);
int mgag200_dumb_create(struct drm_file *file,
			struct drm_device *dev,
			struct drm_mode_create_dumb *args);
void mgag200_gem_free_object(struct drm_gem_object *obj);
int
mgag200_dumb_mmap_offset(struct drm_file *file,
			 struct drm_device *dev,
			 uint32_t handle,
			 uint64_t *offset);
=======

>>>>>>> upstream/android-13
				/* mgag200_i2c.c */
struct mga_i2c_chan *mgag200_i2c_create(struct drm_device *dev);
void mgag200_i2c_destroy(struct mga_i2c_chan *i2c);

<<<<<<< HEAD
#define DRM_FILE_PAGE_OFFSET (0x100000000ULL >> PAGE_SHIFT)
void mgag200_ttm_placement(struct mgag200_bo *bo, int domain);

static inline int mgag200_bo_reserve(struct mgag200_bo *bo, bool no_wait)
{
	int ret;

	ret = ttm_bo_reserve(&bo->bo, true, no_wait, NULL);
	if (ret) {
		if (ret != -ERESTARTSYS && ret != -EBUSY)
			DRM_ERROR("reserve failed %p\n", bo);
		return ret;
	}
	return 0;
}

static inline void mgag200_bo_unreserve(struct mgag200_bo *bo)
{
	ttm_bo_unreserve(&bo->bo);
}

int mgag200_bo_create(struct drm_device *dev, int size, int align,
		      uint32_t flags, struct mgag200_bo **pastbo);
int mgag200_mm_init(struct mga_device *mdev);
void mgag200_mm_fini(struct mga_device *mdev);
int mgag200_mmap(struct file *filp, struct vm_area_struct *vma);
int mgag200_bo_pin(struct mgag200_bo *bo, u32 pl_flag, u64 *gpu_addr);
int mgag200_bo_unpin(struct mgag200_bo *bo);
int mgag200_bo_push_sysram(struct mgag200_bo *bo);
			   /* mgag200_cursor.c */
int mga_crtc_cursor_set(struct drm_crtc *crtc, struct drm_file *file_priv,
						uint32_t handle, uint32_t width, uint32_t height);
int mga_crtc_cursor_move(struct drm_crtc *crtc, int x, int y);
=======
				/* mgag200_mm.c */
int mgag200_mm_init(struct mga_device *mdev);

				/* mgag200_pll.c */
int mgag200_pixpll_init(struct mgag200_pll *pixpll, struct mga_device *mdev);
>>>>>>> upstream/android-13

#endif				/* __MGAG200_DRV_H__ */
