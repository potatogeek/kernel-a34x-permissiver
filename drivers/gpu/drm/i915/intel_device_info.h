/*
 * Copyright © 2014-2017 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#ifndef _INTEL_DEVICE_INFO_H_
#define _INTEL_DEVICE_INFO_H_

<<<<<<< HEAD
#include "intel_display.h"
=======
#include <uapi/drm/i915_drm.h>

#include "intel_step.h"

#include "display/intel_display.h"

#include "gt/intel_engine_types.h"
#include "gt/intel_context_types.h"
#include "gt/intel_sseu.h"
>>>>>>> upstream/android-13

struct drm_printer;
struct drm_i915_private;

/* Keep in gen based order, and chronological order within a gen */
enum intel_platform {
	INTEL_PLATFORM_UNINITIALIZED = 0,
	/* gen2 */
	INTEL_I830,
	INTEL_I845G,
	INTEL_I85X,
	INTEL_I865G,
	/* gen3 */
	INTEL_I915G,
	INTEL_I915GM,
	INTEL_I945G,
	INTEL_I945GM,
	INTEL_G33,
	INTEL_PINEVIEW,
	/* gen4 */
	INTEL_I965G,
	INTEL_I965GM,
	INTEL_G45,
	INTEL_GM45,
	/* gen5 */
	INTEL_IRONLAKE,
	/* gen6 */
	INTEL_SANDYBRIDGE,
	/* gen7 */
	INTEL_IVYBRIDGE,
	INTEL_VALLEYVIEW,
	INTEL_HASWELL,
	/* gen8 */
	INTEL_BROADWELL,
	INTEL_CHERRYVIEW,
	/* gen9 */
	INTEL_SKYLAKE,
	INTEL_BROXTON,
	INTEL_KABYLAKE,
	INTEL_GEMINILAKE,
	INTEL_COFFEELAKE,
<<<<<<< HEAD
	/* gen10 */
	INTEL_CANNONLAKE,
	/* gen11 */
	INTEL_ICELAKE,
	INTEL_MAX_PLATFORMS
};

#define DEV_INFO_FOR_EACH_FLAG(func) \
	func(is_mobile); \
	func(is_lp); \
	func(is_alpha_support); \
	/* Keep has_* in alphabetical order */ \
	func(has_64bit_reloc); \
	func(has_aliasing_ppgtt); \
	func(has_csr); \
	func(has_ddi); \
	func(has_dp_mst); \
	func(has_reset_engine); \
	func(has_fbc); \
	func(has_fpga_dbg); \
	func(has_full_ppgtt); \
	func(has_full_48bit_ppgtt); \
	func(has_gmch_display); \
	func(has_guc); \
	func(has_guc_ct); \
	func(has_hotplug); \
=======
	INTEL_COMETLAKE,
	/* gen11 */
	INTEL_ICELAKE,
	INTEL_ELKHARTLAKE,
	INTEL_JASPERLAKE,
	/* gen12 */
	INTEL_TIGERLAKE,
	INTEL_ROCKETLAKE,
	INTEL_DG1,
	INTEL_ALDERLAKE_S,
	INTEL_ALDERLAKE_P,
	INTEL_XEHPSDV,
	INTEL_DG2,
	INTEL_MAX_PLATFORMS
};

/*
 * Subplatform bits share the same namespace per parent platform. In other words
 * it is fine for the same bit to be used on multiple parent platforms.
 */

#define INTEL_SUBPLATFORM_BITS (2)
#define INTEL_SUBPLATFORM_MASK (BIT(INTEL_SUBPLATFORM_BITS) - 1)

/* HSW/BDW/SKL/KBL/CFL */
#define INTEL_SUBPLATFORM_ULT	(0)
#define INTEL_SUBPLATFORM_ULX	(1)

/* ICL */
#define INTEL_SUBPLATFORM_PORTF	(0)

/* DG2 */
#define INTEL_SUBPLATFORM_G10	0
#define INTEL_SUBPLATFORM_G11	1

enum intel_ppgtt_type {
	INTEL_PPGTT_NONE = I915_GEM_PPGTT_NONE,
	INTEL_PPGTT_ALIASING = I915_GEM_PPGTT_ALIASING,
	INTEL_PPGTT_FULL = I915_GEM_PPGTT_FULL,
};

#define DEV_INFO_FOR_EACH_FLAG(func) \
	func(is_mobile); \
	func(is_lp); \
	func(require_force_probe); \
	func(is_dgfx); \
	/* Keep has_* in alphabetical order */ \
	func(has_64bit_reloc); \
	func(gpu_reset_clobbers_display); \
	func(has_reset_engine); \
	func(has_global_mocs); \
	func(has_gt_uc); \
>>>>>>> upstream/android-13
	func(has_l3_dpf); \
	func(has_llc); \
	func(has_logical_ring_contexts); \
	func(has_logical_ring_elsq); \
<<<<<<< HEAD
	func(has_logical_ring_preemption); \
	func(has_overlay); \
	func(has_pooled_eu); \
	func(has_psr); \
	func(has_rc6); \
	func(has_rc6p); \
	func(has_resource_streamer); \
	func(has_runtime_pm); \
	func(has_snoop); \
	func(unfenced_needs_alignment); \
	func(cursor_needs_physical); \
	func(hws_needs_physical); \
	func(overlay_needs_physical); \
	func(supports_tv); \
	func(has_ipc);

#define GEN_MAX_SLICES		(6) /* CNL upper bound */
#define GEN_MAX_SUBSLICES	(8) /* ICL upper bound */

struct sseu_dev_info {
	u8 slice_mask;
	u8 subslice_mask[GEN_MAX_SUBSLICES];
	u16 eu_total;
	u8 eu_per_subslice;
	u8 min_eu_in_pool;
	/* For each slice, which subslice(s) has(have) 7 EUs (bitfield)? */
	u8 subslice_7eu[3];
	u8 has_slice_pg:1;
	u8 has_subslice_pg:1;
	u8 has_eu_pg:1;

	/* Topology fields */
	u8 max_slices;
	u8 max_subslices;
	u8 max_eus_per_subslice;

	/* We don't have more than 8 eus per subslice at the moment and as we
	 * store eus enabled using bits, no need to multiply by eus per
	 * subslice.
	 */
	u8 eu_mask[GEN_MAX_SLICES * GEN_MAX_SUBSLICES];
};

typedef u8 intel_ring_mask_t;

struct intel_device_info {
	u16 device_id;
	u16 gen_mask;

	u8 gen;
	u8 gt; /* GT number, 0 if undefined */
	u8 num_rings;
	intel_ring_mask_t ring_mask; /* Rings supported by the HW */

	enum intel_platform platform;
	u32 platform_mask;

	unsigned int page_sizes; /* page sizes supported by the HW */

	u32 display_mmio_offset;

	u8 num_pipes;
	u8 num_sprites[I915_MAX_PIPES];
	u8 num_scalers[I915_MAX_PIPES];
=======
	func(has_mslices); \
	func(has_pooled_eu); \
	func(has_rc6); \
	func(has_rc6p); \
	func(has_rps); \
	func(has_runtime_pm); \
	func(has_snoop); \
	func(has_coherent_ggtt); \
	func(unfenced_needs_alignment); \
	func(hws_needs_physical);

#define DEV_INFO_DISPLAY_FOR_EACH_FLAG(func) \
	/* Keep in alphabetical order */ \
	func(cursor_needs_physical); \
	func(has_cdclk_crawl); \
	func(has_dmc); \
	func(has_ddi); \
	func(has_dp_mst); \
	func(has_dsb); \
	func(has_dsc); \
	func(has_fbc); \
	func(has_fpga_dbg); \
	func(has_gmch); \
	func(has_hdcp); \
	func(has_hotplug); \
	func(has_hti); \
	func(has_ipc); \
	func(has_modular_fia); \
	func(has_overlay); \
	func(has_psr); \
	func(has_psr_hw_tracking); \
	func(overlay_needs_physical); \
	func(supports_tv);

struct intel_device_info {
	u8 graphics_ver;
	u8 graphics_rel;
	u8 media_ver;
	u8 media_rel;

	intel_engine_mask_t platform_engine_mask; /* Engines supported by the HW */

	enum intel_platform platform;

	unsigned int dma_mask_size; /* available DMA address bits */

	enum intel_ppgtt_type ppgtt_type;
	unsigned int ppgtt_size; /* log2, e.g. 31/32/48 bits */

	unsigned int page_sizes; /* page sizes supported by the HW */

	u32 memory_regions; /* regions supported by the HW */

	u32 display_mmio_offset;

	u8 gt; /* GT number, 0 if undefined */

	u8 pipe_mask;
	u8 cpu_transcoder_mask;

	u8 abox_mask;
>>>>>>> upstream/android-13

#define DEFINE_FLAG(name) u8 name:1
	DEV_INFO_FOR_EACH_FLAG(DEFINE_FLAG);
#undef DEFINE_FLAG
<<<<<<< HEAD
	u16 ddb_size; /* in blocks */
=======

	struct {
		u8 ver;

#define DEFINE_FLAG(name) u8 name:1
		DEV_INFO_DISPLAY_FOR_EACH_FLAG(DEFINE_FLAG);
#undef DEFINE_FLAG
	} display;

	struct {
		u16 size; /* in blocks */
		u8 slice_mask;
	} dbuf;
>>>>>>> upstream/android-13

	/* Register offsets for the various display pipes and transcoders */
	int pipe_offsets[I915_MAX_TRANSCODERS];
	int trans_offsets[I915_MAX_TRANSCODERS];
<<<<<<< HEAD
	int palette_offsets[I915_MAX_PIPES];
	int cursor_offsets[I915_MAX_PIPES];

	/* Slice/subslice/EU info */
	struct sseu_dev_info sseu;

	u32 cs_timestamp_frequency_khz;

	struct color_luts {
		u16 degamma_lut_size;
		u16 gamma_lut_size;
	} color;
};

=======
	int cursor_offsets[I915_MAX_PIPES];

	struct color_luts {
		u32 degamma_lut_size;
		u32 gamma_lut_size;
		u32 degamma_lut_tests;
		u32 gamma_lut_tests;
	} color;
};

struct intel_runtime_info {
	/*
	 * Platform mask is used for optimizing or-ed IS_PLATFORM calls into
	 * into single runtime conditionals, and also to provide groundwork
	 * for future per platform, or per SKU build optimizations.
	 *
	 * Array can be extended when necessary if the corresponding
	 * BUILD_BUG_ON is hit.
	 */
	u32 platform_mask[2];

	u16 device_id;

	u8 num_sprites[I915_MAX_PIPES];
	u8 num_scalers[I915_MAX_PIPES];

	u32 rawclk_freq;

	struct intel_step_info step;
};

>>>>>>> upstream/android-13
struct intel_driver_caps {
	unsigned int scheduler;
	bool has_logical_contexts:1;
};

<<<<<<< HEAD
static inline unsigned int sseu_subslice_total(const struct sseu_dev_info *sseu)
{
	unsigned int i, total = 0;

	for (i = 0; i < ARRAY_SIZE(sseu->subslice_mask); i++)
		total += hweight8(sseu->subslice_mask[i]);

	return total;
}

static inline int sseu_eu_idx(const struct sseu_dev_info *sseu,
			      int slice, int subslice)
{
	int subslice_stride = DIV_ROUND_UP(sseu->max_eus_per_subslice,
					   BITS_PER_BYTE);
	int slice_stride = sseu->max_subslices * subslice_stride;

	return slice * slice_stride + subslice * subslice_stride;
}

static inline u16 sseu_get_eus(const struct sseu_dev_info *sseu,
			       int slice, int subslice)
{
	int i, offset = sseu_eu_idx(sseu, slice, subslice);
	u16 eu_mask = 0;

	for (i = 0;
	     i < DIV_ROUND_UP(sseu->max_eus_per_subslice, BITS_PER_BYTE); i++) {
		eu_mask |= ((u16) sseu->eu_mask[offset + i]) <<
			(i * BITS_PER_BYTE);
	}

	return eu_mask;
}

static inline void sseu_set_eus(struct sseu_dev_info *sseu,
				int slice, int subslice, u16 eu_mask)
{
	int i, offset = sseu_eu_idx(sseu, slice, subslice);

	for (i = 0;
	     i < DIV_ROUND_UP(sseu->max_eus_per_subslice, BITS_PER_BYTE); i++) {
		sseu->eu_mask[offset + i] =
			(eu_mask >> (BITS_PER_BYTE * i)) & 0xff;
	}
}

const char *intel_platform_name(enum intel_platform platform);

void intel_device_info_runtime_init(struct intel_device_info *info);
void intel_device_info_dump(const struct intel_device_info *info,
			    struct drm_printer *p);
void intel_device_info_dump_flags(const struct intel_device_info *info,
				  struct drm_printer *p);
void intel_device_info_dump_runtime(const struct intel_device_info *info,
				    struct drm_printer *p);
void intel_device_info_dump_topology(const struct sseu_dev_info *sseu,
				     struct drm_printer *p);

void intel_device_info_init_mmio(struct drm_i915_private *dev_priv);
=======
const char *intel_platform_name(enum intel_platform platform);

void intel_device_info_subplatform_init(struct drm_i915_private *dev_priv);
void intel_device_info_runtime_init(struct drm_i915_private *dev_priv);

void intel_device_info_print_static(const struct intel_device_info *info,
				    struct drm_printer *p);
void intel_device_info_print_runtime(const struct intel_runtime_info *info,
				     struct drm_printer *p);
>>>>>>> upstream/android-13

void intel_driver_caps_print(const struct intel_driver_caps *caps,
			     struct drm_printer *p);

#endif
