<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> upstream/android-13
/*
 * rcar_du_drv.h  --  R-Car Display Unit DRM driver
 *
 * Copyright (C) 2013-2015 Renesas Electronics Corporation
 *
 * Contact: Laurent Pinchart (laurent.pinchart@ideasonboard.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __RCAR_DU_DRV_H__
#define __RCAR_DU_DRV_H__

#include <linux/kernel.h>
#include <linux/wait.h>

<<<<<<< HEAD
=======
#include <drm/drm_device.h>

#include "rcar_cmm.h"
>>>>>>> upstream/android-13
#include "rcar_du_crtc.h"
#include "rcar_du_group.h"
#include "rcar_du_vsp.h"

struct clk;
struct device;
<<<<<<< HEAD
struct drm_device;
struct drm_fbdev_cma;
struct rcar_du_device;

#define RCAR_DU_FEATURE_CRTC_IRQ_CLOCK	(1 << 0)	/* Per-CRTC IRQ and clock */
#define RCAR_DU_FEATURE_EXT_CTRL_REGS	(1 << 1)	/* Has extended control registers */
#define RCAR_DU_FEATURE_VSP1_SOURCE	(1 << 2)	/* Has inputs from VSP1 */

#define RCAR_DU_QUIRK_ALIGN_128B	(1 << 0)	/* Align pitches to 128 bytes */
=======
struct drm_bridge;
struct drm_property;
struct rcar_du_device;

#define RCAR_DU_FEATURE_CRTC_IRQ_CLOCK	BIT(0)	/* Per-CRTC IRQ and clock */
#define RCAR_DU_FEATURE_VSP1_SOURCE	BIT(1)	/* Has inputs from VSP1 */
#define RCAR_DU_FEATURE_INTERLACED	BIT(2)	/* HW supports interlaced */
#define RCAR_DU_FEATURE_TVM_SYNC	BIT(3)	/* Has TV switch/sync modes */

#define RCAR_DU_QUIRK_ALIGN_128B	BIT(0)	/* Align pitches to 128 bytes */
>>>>>>> upstream/android-13

/*
 * struct rcar_du_output_routing - Output routing specification
 * @possible_crtcs: bitmask of possible CRTCs for the output
 * @port: device tree port number corresponding to this output route
 *
 * The DU has 5 possible outputs (DPAD0/1, LVDS0/1, TCON). Output routing data
 * specify the valid SoC outputs, which CRTCs can drive the output, and the type
 * of in-SoC encoder for the output.
 */
struct rcar_du_output_routing {
	unsigned int possible_crtcs;
	unsigned int port;
};

/*
 * struct rcar_du_device_info - DU model-specific information
 * @gen: device generation (2 or 3)
 * @features: device features (RCAR_DU_FEATURE_*)
 * @quirks: device quirks (RCAR_DU_QUIRK_*)
 * @channels_mask: bit mask of available DU channels
 * @routes: array of CRTC to output routes, indexed by output (RCAR_DU_OUTPUT_*)
 * @num_lvds: number of internal LVDS encoders
<<<<<<< HEAD
=======
 * @dpll_mask: bit mask of DU channels equipped with a DPLL
 * @lvds_clk_mask: bitmask of channels that can use the LVDS clock as dot clock
>>>>>>> upstream/android-13
 */
struct rcar_du_device_info {
	unsigned int gen;
	unsigned int features;
	unsigned int quirks;
	unsigned int channels_mask;
	struct rcar_du_output_routing routes[RCAR_DU_OUTPUT_MAX];
	unsigned int num_lvds;
<<<<<<< HEAD
	unsigned int dpll_ch;
=======
	unsigned int dpll_mask;
	unsigned int lvds_clk_mask;
>>>>>>> upstream/android-13
};

#define RCAR_DU_MAX_CRTCS		4
#define RCAR_DU_MAX_GROUPS		DIV_ROUND_UP(RCAR_DU_MAX_CRTCS, 2)
#define RCAR_DU_MAX_VSPS		4
<<<<<<< HEAD
=======
#define RCAR_DU_MAX_LVDS		2
>>>>>>> upstream/android-13

struct rcar_du_device {
	struct device *dev;
	const struct rcar_du_device_info *info;

	void __iomem *mmio;

<<<<<<< HEAD
	struct drm_device *ddev;
	struct drm_fbdev_cma *fbdev;
	struct drm_atomic_state *suspend_state;
=======
	struct drm_device ddev;
>>>>>>> upstream/android-13

	struct rcar_du_crtc crtcs[RCAR_DU_MAX_CRTCS];
	unsigned int num_crtcs;

	struct rcar_du_group groups[RCAR_DU_MAX_GROUPS];
<<<<<<< HEAD
	struct rcar_du_vsp vsps[RCAR_DU_MAX_VSPS];
=======
	struct platform_device *cmms[RCAR_DU_MAX_CRTCS];
	struct rcar_du_vsp vsps[RCAR_DU_MAX_VSPS];
	struct drm_bridge *lvds[RCAR_DU_MAX_LVDS];
>>>>>>> upstream/android-13

	struct {
		struct drm_property *colorkey;
	} props;

	unsigned int dpad0_source;
<<<<<<< HEAD
	unsigned int vspd1_sink;
};

=======
	unsigned int dpad1_source;
	unsigned int vspd1_sink;
};

static inline struct rcar_du_device *to_rcar_du_device(struct drm_device *dev)
{
	return container_of(dev, struct rcar_du_device, ddev);
}

>>>>>>> upstream/android-13
static inline bool rcar_du_has(struct rcar_du_device *rcdu,
			       unsigned int feature)
{
	return rcdu->info->features & feature;
}

static inline bool rcar_du_needs(struct rcar_du_device *rcdu,
				 unsigned int quirk)
{
	return rcdu->info->quirks & quirk;
}

static inline u32 rcar_du_read(struct rcar_du_device *rcdu, u32 reg)
{
	return ioread32(rcdu->mmio + reg);
}

static inline void rcar_du_write(struct rcar_du_device *rcdu, u32 reg, u32 data)
{
	iowrite32(data, rcdu->mmio + reg);
}

#endif /* __RCAR_DU_DRV_H__ */
