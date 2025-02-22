<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * VRFB Rotation Engine
 *
 * Copyright (C) 2009 Nokia Corporation
 * Author: Tomi Valkeinen <tomi.valkeinen@nokia.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
=======
>>>>>>> upstream/android-13
 */

#ifndef __OMAP_VRFB_H__
#define __OMAP_VRFB_H__

#define OMAP_VRFB_LINE_LEN 2048

struct vrfb {
	u8 context;
	void __iomem *vaddr[4];
	unsigned long paddr[4];
	u16 xres;
	u16 yres;
	u16 xoffset;
	u16 yoffset;
	u8 bytespp;
	bool yuv_mode;
};

#ifdef CONFIG_OMAP2_VRFB
extern bool omap_vrfb_supported(void);
extern int omap_vrfb_request_ctx(struct vrfb *vrfb);
extern void omap_vrfb_release_ctx(struct vrfb *vrfb);
extern void omap_vrfb_adjust_size(u16 *width, u16 *height,
		u8 bytespp);
extern u32 omap_vrfb_min_phys_size(u16 width, u16 height, u8 bytespp);
extern u16 omap_vrfb_max_height(u32 phys_size, u16 width, u8 bytespp);
extern void omap_vrfb_setup(struct vrfb *vrfb, unsigned long paddr,
		u16 width, u16 height,
		unsigned bytespp, bool yuv_mode);
extern int omap_vrfb_map_angle(struct vrfb *vrfb, u16 height, u8 rot);
extern void omap_vrfb_restore_context(void);

#else
static inline bool omap_vrfb_supported(void) { return false; }
static inline int omap_vrfb_request_ctx(struct vrfb *vrfb) { return 0; }
static inline void omap_vrfb_release_ctx(struct vrfb *vrfb) {}
static inline void omap_vrfb_adjust_size(u16 *width, u16 *height,
		u8 bytespp) {}
static inline u32 omap_vrfb_min_phys_size(u16 width, u16 height, u8 bytespp)
		{ return 0; }
static inline u16 omap_vrfb_max_height(u32 phys_size, u16 width, u8 bytespp)
		{ return 0; }
static inline void omap_vrfb_setup(struct vrfb *vrfb, unsigned long paddr,
		u16 width, u16 height, unsigned bytespp, bool yuv_mode) {}
static inline int omap_vrfb_map_angle(struct vrfb *vrfb, u16 height, u8 rot)
		{ return 0; }
static inline void omap_vrfb_restore_context(void) {}
#endif
#endif /* __VRFB_H */
