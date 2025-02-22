<<<<<<< HEAD
/*
 * Copyright (C) 2016 Pengutronix
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2016 Pengutronix
>>>>>>> upstream/android-13
 */

#ifndef IMX_VDOA_H
#define IMX_VDOA_H

struct vdoa_data;
struct vdoa_ctx;

#if (defined CONFIG_VIDEO_IMX_VDOA || defined CONFIG_VIDEO_IMX_VDOA_MODULE)

struct vdoa_ctx *vdoa_context_create(struct vdoa_data *vdoa);
int vdoa_context_configure(struct vdoa_ctx *ctx,
			   unsigned int width, unsigned int height,
			   u32 pixelformat);
void vdoa_context_destroy(struct vdoa_ctx *ctx);

void vdoa_device_run(struct vdoa_ctx *ctx, dma_addr_t dst, dma_addr_t src);
int vdoa_wait_for_completion(struct vdoa_ctx *ctx);

#else

static inline struct vdoa_ctx *vdoa_context_create(struct vdoa_data *vdoa)
{
	return NULL;
}

static inline int vdoa_context_configure(struct vdoa_ctx *ctx,
					 unsigned int width,
					 unsigned int height,
					 u32 pixelformat)
{
	return 0;
}

static inline void vdoa_context_destroy(struct vdoa_ctx *ctx) { };

static inline void vdoa_device_run(struct vdoa_ctx *ctx,
				   dma_addr_t dst, dma_addr_t src) { };

static inline int vdoa_wait_for_completion(struct vdoa_ctx *ctx)
{
	return 0;
};

#endif

#endif /* IMX_VDOA_H */
