/*
 * Copyright © 2012 Intel Corporation
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
 * Authors:
 *    Eugeni Dodonov <eugeni.dodonov@intel.com>
 *
 */

<<<<<<< HEAD
#include <linux/cpufreq.h>
#include <drm/drm_plane_helper.h>
#include "i915_drv.h"
#include "intel_drv.h"
#include "../../../platform/x86/intel_ips.h"
#include <linux/module.h>
#include <drm/drm_atomic_helper.h>

/**
 * DOC: RC6
 *
 * RC6 is a special power stage which allows the GPU to enter an very
 * low-voltage mode when idle, using down to 0V while at this stage.  This
 * stage is entered automatically when the GPU is idle when RC6 support is
 * enabled, and as soon as new workload arises GPU wakes up automatically as well.
 *
 * There are different RC6 modes available in Intel GPU, which differentiate
 * among each other with the latency required to enter and leave RC6 and
 * voltage consumed by the GPU in different states.
 *
 * The combination of the following flags define which states GPU is allowed
 * to enter, while RC6 is the normal RC6 state, RC6p is the deep RC6, and
 * RC6pp is deepest RC6. Their support by hardware varies according to the
 * GPU, BIOS, chipset and platform. RC6 is usually the safest one and the one
 * which brings the most power savings; deeper states save more power, but
 * require higher latency to switch to and wake up.
 */

static void gen9_init_clock_gating(struct drm_i915_private *dev_priv)
{
=======
#include <linux/module.h>
#include <linux/pm_runtime.h>

#include <drm/drm_atomic_helper.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_plane_helper.h>

#include "display/intel_atomic.h"
#include "display/intel_atomic_plane.h"
#include "display/intel_bw.h"
#include "display/intel_de.h"
#include "display/intel_display_types.h"
#include "display/intel_fbc.h"
#include "display/intel_sprite.h"
#include "display/skl_universal_plane.h"

#include "gt/intel_llc.h"

#include "i915_drv.h"
#include "i915_fixed.h"
#include "i915_irq.h"
#include "i915_trace.h"
#include "intel_pm.h"
#include "intel_sideband.h"
#include "../../../platform/x86/intel_ips.h"

/* Stores plane specific WM parameters */
struct skl_wm_params {
	bool x_tiled, y_tiled;
	bool rc_surface;
	bool is_planar;
	u32 width;
	u8 cpp;
	u32 plane_pixel_rate;
	u32 y_min_scanlines;
	u32 plane_bytes_per_line;
	uint_fixed_16_16_t plane_blocks_per_line;
	uint_fixed_16_16_t y_tile_minimum;
	u32 linetime_us;
	u32 dbuf_block_size;
};

/* used in computing the new watermarks state */
struct intel_wm_config {
	unsigned int num_pipes_active;
	bool sprites_enabled;
	bool sprites_scaled;
};

static void gen9_init_clock_gating(struct drm_i915_private *dev_priv)
{
	enum pipe pipe;

>>>>>>> upstream/android-13
	if (HAS_LLC(dev_priv)) {
		/*
		 * WaCompressedResourceDisplayNewHashMode:skl,kbl
		 * Display WA #0390: skl,kbl
		 *
		 * Must match Sampler, Pixel Back End, and Media. See
		 * WaCompressedResourceSamplerPbeMediaNewHashMode.
		 */
<<<<<<< HEAD
		I915_WRITE(CHICKEN_PAR1_1,
			   I915_READ(CHICKEN_PAR1_1) |
			   SKL_DE_COMPRESSED_HASH_MODE);
	}

	/* See Bspec note for PSR2_CTL bit 31, Wa#828:skl,bxt,kbl,cfl */
	I915_WRITE(CHICKEN_PAR1_1,
		   I915_READ(CHICKEN_PAR1_1) | SKL_EDP_PSR_FIX_RDWRAP);

	/* WaEnableChickenDCPR:skl,bxt,kbl,glk,cfl */
	I915_WRITE(GEN8_CHICKEN_DCPR_1,
		   I915_READ(GEN8_CHICKEN_DCPR_1) | MASK_WAKEMEM);

	/* WaFbcTurnOffFbcWatermark:skl,bxt,kbl,cfl */
	/* WaFbcWakeMemOn:skl,bxt,kbl,glk,cfl */
	I915_WRITE(DISP_ARB_CTL, I915_READ(DISP_ARB_CTL) |
		   DISP_FBC_WM_DIS |
		   DISP_FBC_MEMORY_WAKE);

	/* WaFbcHighMemBwCorruptionAvoidance:skl,bxt,kbl,cfl */
	I915_WRITE(ILK_DPFC_CHICKEN, I915_READ(ILK_DPFC_CHICKEN) |
		   ILK_DPFC_DISABLE_DUMMY0);

	if (IS_SKYLAKE(dev_priv)) {
		/* WaDisableDopClockGating */
		I915_WRITE(GEN7_MISCCPCTL, I915_READ(GEN7_MISCCPCTL)
			   & ~GEN7_DOP_CLOCK_GATE_ENABLE);
	}
=======
		intel_uncore_write(&dev_priv->uncore, CHICKEN_PAR1_1,
			   intel_uncore_read(&dev_priv->uncore, CHICKEN_PAR1_1) |
			   SKL_DE_COMPRESSED_HASH_MODE);
	}

	for_each_pipe(dev_priv, pipe) {
		/*
		 * "Plane N strech max must be programmed to 11b (x1)
		 *  when Async flips are enabled on that plane."
		 */
		if (!IS_GEMINILAKE(dev_priv) && intel_vtd_active())
			intel_uncore_rmw(&dev_priv->uncore, CHICKEN_PIPESL_1(pipe),
					 SKL_PLANE1_STRETCH_MAX_MASK, SKL_PLANE1_STRETCH_MAX_X1);
	}

	/* See Bspec note for PSR2_CTL bit 31, Wa#828:skl,bxt,kbl,cfl */
	intel_uncore_write(&dev_priv->uncore, CHICKEN_PAR1_1,
		   intel_uncore_read(&dev_priv->uncore, CHICKEN_PAR1_1) | SKL_EDP_PSR_FIX_RDWRAP);

	/* WaEnableChickenDCPR:skl,bxt,kbl,glk,cfl */
	intel_uncore_write(&dev_priv->uncore, GEN8_CHICKEN_DCPR_1,
		   intel_uncore_read(&dev_priv->uncore, GEN8_CHICKEN_DCPR_1) | MASK_WAKEMEM);

	/*
	 * WaFbcWakeMemOn:skl,bxt,kbl,glk,cfl
	 * Display WA #0859: skl,bxt,kbl,glk,cfl
	 */
	intel_uncore_write(&dev_priv->uncore, DISP_ARB_CTL, intel_uncore_read(&dev_priv->uncore, DISP_ARB_CTL) |
		   DISP_FBC_MEMORY_WAKE);
>>>>>>> upstream/android-13
}

static void bxt_init_clock_gating(struct drm_i915_private *dev_priv)
{
	gen9_init_clock_gating(dev_priv);

	/* WaDisableSDEUnitClockGating:bxt */
<<<<<<< HEAD
	I915_WRITE(GEN8_UCGCTL6, I915_READ(GEN8_UCGCTL6) |
=======
	intel_uncore_write(&dev_priv->uncore, GEN8_UCGCTL6, intel_uncore_read(&dev_priv->uncore, GEN8_UCGCTL6) |
>>>>>>> upstream/android-13
		   GEN8_SDEUNIT_CLOCK_GATE_DISABLE);

	/*
	 * FIXME:
	 * GEN8_HDCUNIT_CLOCK_GATE_DISABLE_HDCREQ applies on 3x6 GT SKUs only.
	 */
<<<<<<< HEAD
	I915_WRITE(GEN8_UCGCTL6, I915_READ(GEN8_UCGCTL6) |
=======
	intel_uncore_write(&dev_priv->uncore, GEN8_UCGCTL6, intel_uncore_read(&dev_priv->uncore, GEN8_UCGCTL6) |
>>>>>>> upstream/android-13
		   GEN8_HDCUNIT_CLOCK_GATE_DISABLE_HDCREQ);

	/*
	 * Wa: Backlight PWM may stop in the asserted state, causing backlight
	 * to stay fully on.
	 */
<<<<<<< HEAD
	I915_WRITE(GEN9_CLKGATE_DIS_0, I915_READ(GEN9_CLKGATE_DIS_0) |
=======
	intel_uncore_write(&dev_priv->uncore, GEN9_CLKGATE_DIS_0, intel_uncore_read(&dev_priv->uncore, GEN9_CLKGATE_DIS_0) |
>>>>>>> upstream/android-13
		   PWM1_GATING_DIS | PWM2_GATING_DIS);

	/*
	 * Lower the display internal timeout.
	 * This is needed to avoid any hard hangs when DSI port PLL
	 * is off and a MMIO access is attempted by any privilege
	 * application, using batch buffers or any other means.
	 */
<<<<<<< HEAD
	I915_WRITE(RM_TIMEOUT, MMIO_TIMEOUT_US(950));
=======
	intel_uncore_write(&dev_priv->uncore, RM_TIMEOUT, MMIO_TIMEOUT_US(950));

	/*
	 * WaFbcTurnOffFbcWatermark:bxt
	 * Display WA #0562: bxt
	 */
	intel_uncore_write(&dev_priv->uncore, DISP_ARB_CTL, intel_uncore_read(&dev_priv->uncore, DISP_ARB_CTL) |
		   DISP_FBC_WM_DIS);

	/*
	 * WaFbcHighMemBwCorruptionAvoidance:bxt
	 * Display WA #0883: bxt
	 */
	intel_uncore_write(&dev_priv->uncore, ILK_DPFC_CHICKEN, intel_uncore_read(&dev_priv->uncore, ILK_DPFC_CHICKEN) |
		   ILK_DPFC_DISABLE_DUMMY0);
>>>>>>> upstream/android-13
}

static void glk_init_clock_gating(struct drm_i915_private *dev_priv)
{
	gen9_init_clock_gating(dev_priv);

	/*
	 * WaDisablePWMClockGating:glk
	 * Backlight PWM may stop in the asserted state, causing backlight
	 * to stay fully on.
	 */
<<<<<<< HEAD
	I915_WRITE(GEN9_CLKGATE_DIS_0, I915_READ(GEN9_CLKGATE_DIS_0) |
		   PWM1_GATING_DIS | PWM2_GATING_DIS);

	/* WaDDIIOTimeout:glk */
	if (IS_GLK_REVID(dev_priv, 0, GLK_REVID_A1)) {
		u32 val = I915_READ(CHICKEN_MISC_2);
		val &= ~(GLK_CL0_PWR_DOWN |
			 GLK_CL1_PWR_DOWN |
			 GLK_CL2_PWR_DOWN);
		I915_WRITE(CHICKEN_MISC_2, val);
	}

}

static void i915_pineview_get_mem_freq(struct drm_i915_private *dev_priv)
{
	u32 tmp;

	tmp = I915_READ(CLKCFG);
=======
	intel_uncore_write(&dev_priv->uncore, GEN9_CLKGATE_DIS_0, intel_uncore_read(&dev_priv->uncore, GEN9_CLKGATE_DIS_0) |
		   PWM1_GATING_DIS | PWM2_GATING_DIS);
}

static void pnv_get_mem_freq(struct drm_i915_private *dev_priv)
{
	u32 tmp;

	tmp = intel_uncore_read(&dev_priv->uncore, CLKCFG);
>>>>>>> upstream/android-13

	switch (tmp & CLKCFG_FSB_MASK) {
	case CLKCFG_FSB_533:
		dev_priv->fsb_freq = 533; /* 133*4 */
		break;
	case CLKCFG_FSB_800:
		dev_priv->fsb_freq = 800; /* 200*4 */
		break;
	case CLKCFG_FSB_667:
		dev_priv->fsb_freq =  667; /* 167*4 */
		break;
	case CLKCFG_FSB_400:
		dev_priv->fsb_freq = 400; /* 100*4 */
		break;
	}

	switch (tmp & CLKCFG_MEM_MASK) {
	case CLKCFG_MEM_533:
		dev_priv->mem_freq = 533;
		break;
	case CLKCFG_MEM_667:
		dev_priv->mem_freq = 667;
		break;
	case CLKCFG_MEM_800:
		dev_priv->mem_freq = 800;
		break;
	}

	/* detect pineview DDR3 setting */
<<<<<<< HEAD
	tmp = I915_READ(CSHRDDR3CTL);
	dev_priv->is_ddr3 = (tmp & CSHRDDR3CTL_DDR3) ? 1 : 0;
}

static void i915_ironlake_get_mem_freq(struct drm_i915_private *dev_priv)
{
	u16 ddrpll, csipll;

	ddrpll = I915_READ16(DDRMPLL1);
	csipll = I915_READ16(CSIPLL0);
=======
	tmp = intel_uncore_read(&dev_priv->uncore, CSHRDDR3CTL);
	dev_priv->is_ddr3 = (tmp & CSHRDDR3CTL_DDR3) ? 1 : 0;
}

static void ilk_get_mem_freq(struct drm_i915_private *dev_priv)
{
	u16 ddrpll, csipll;

	ddrpll = intel_uncore_read16(&dev_priv->uncore, DDRMPLL1);
	csipll = intel_uncore_read16(&dev_priv->uncore, CSIPLL0);
>>>>>>> upstream/android-13

	switch (ddrpll & 0xff) {
	case 0xc:
		dev_priv->mem_freq = 800;
		break;
	case 0x10:
		dev_priv->mem_freq = 1066;
		break;
	case 0x14:
		dev_priv->mem_freq = 1333;
		break;
	case 0x18:
		dev_priv->mem_freq = 1600;
		break;
	default:
<<<<<<< HEAD
		DRM_DEBUG_DRIVER("unknown memory frequency 0x%02x\n",
				 ddrpll & 0xff);
=======
		drm_dbg(&dev_priv->drm, "unknown memory frequency 0x%02x\n",
			ddrpll & 0xff);
>>>>>>> upstream/android-13
		dev_priv->mem_freq = 0;
		break;
	}

<<<<<<< HEAD
	dev_priv->ips.r_t = dev_priv->mem_freq;

=======
>>>>>>> upstream/android-13
	switch (csipll & 0x3ff) {
	case 0x00c:
		dev_priv->fsb_freq = 3200;
		break;
	case 0x00e:
		dev_priv->fsb_freq = 3733;
		break;
	case 0x010:
		dev_priv->fsb_freq = 4266;
		break;
	case 0x012:
		dev_priv->fsb_freq = 4800;
		break;
	case 0x014:
		dev_priv->fsb_freq = 5333;
		break;
	case 0x016:
		dev_priv->fsb_freq = 5866;
		break;
	case 0x018:
		dev_priv->fsb_freq = 6400;
		break;
	default:
<<<<<<< HEAD
		DRM_DEBUG_DRIVER("unknown fsb frequency 0x%04x\n",
				 csipll & 0x3ff);
		dev_priv->fsb_freq = 0;
		break;
	}

	if (dev_priv->fsb_freq == 3200) {
		dev_priv->ips.c_m = 0;
	} else if (dev_priv->fsb_freq > 3200 && dev_priv->fsb_freq <= 4800) {
		dev_priv->ips.c_m = 1;
	} else {
		dev_priv->ips.c_m = 2;
	}
=======
		drm_dbg(&dev_priv->drm, "unknown fsb frequency 0x%04x\n",
			csipll & 0x3ff);
		dev_priv->fsb_freq = 0;
		break;
	}
>>>>>>> upstream/android-13
}

static const struct cxsr_latency cxsr_latency_table[] = {
	{1, 0, 800, 400, 3382, 33382, 3983, 33983},    /* DDR2-400 SC */
	{1, 0, 800, 667, 3354, 33354, 3807, 33807},    /* DDR2-667 SC */
	{1, 0, 800, 800, 3347, 33347, 3763, 33763},    /* DDR2-800 SC */
	{1, 1, 800, 667, 6420, 36420, 6873, 36873},    /* DDR3-667 SC */
	{1, 1, 800, 800, 5902, 35902, 6318, 36318},    /* DDR3-800 SC */

	{1, 0, 667, 400, 3400, 33400, 4021, 34021},    /* DDR2-400 SC */
	{1, 0, 667, 667, 3372, 33372, 3845, 33845},    /* DDR2-667 SC */
	{1, 0, 667, 800, 3386, 33386, 3822, 33822},    /* DDR2-800 SC */
	{1, 1, 667, 667, 6438, 36438, 6911, 36911},    /* DDR3-667 SC */
	{1, 1, 667, 800, 5941, 35941, 6377, 36377},    /* DDR3-800 SC */

	{1, 0, 400, 400, 3472, 33472, 4173, 34173},    /* DDR2-400 SC */
	{1, 0, 400, 667, 3443, 33443, 3996, 33996},    /* DDR2-667 SC */
	{1, 0, 400, 800, 3430, 33430, 3946, 33946},    /* DDR2-800 SC */
	{1, 1, 400, 667, 6509, 36509, 7062, 37062},    /* DDR3-667 SC */
	{1, 1, 400, 800, 5985, 35985, 6501, 36501},    /* DDR3-800 SC */

	{0, 0, 800, 400, 3438, 33438, 4065, 34065},    /* DDR2-400 SC */
	{0, 0, 800, 667, 3410, 33410, 3889, 33889},    /* DDR2-667 SC */
	{0, 0, 800, 800, 3403, 33403, 3845, 33845},    /* DDR2-800 SC */
	{0, 1, 800, 667, 6476, 36476, 6955, 36955},    /* DDR3-667 SC */
	{0, 1, 800, 800, 5958, 35958, 6400, 36400},    /* DDR3-800 SC */

	{0, 0, 667, 400, 3456, 33456, 4103, 34106},    /* DDR2-400 SC */
	{0, 0, 667, 667, 3428, 33428, 3927, 33927},    /* DDR2-667 SC */
	{0, 0, 667, 800, 3443, 33443, 3905, 33905},    /* DDR2-800 SC */
	{0, 1, 667, 667, 6494, 36494, 6993, 36993},    /* DDR3-667 SC */
	{0, 1, 667, 800, 5998, 35998, 6460, 36460},    /* DDR3-800 SC */

	{0, 0, 400, 400, 3528, 33528, 4255, 34255},    /* DDR2-400 SC */
	{0, 0, 400, 667, 3500, 33500, 4079, 34079},    /* DDR2-667 SC */
	{0, 0, 400, 800, 3487, 33487, 4029, 34029},    /* DDR2-800 SC */
	{0, 1, 400, 667, 6566, 36566, 7145, 37145},    /* DDR3-667 SC */
	{0, 1, 400, 800, 6042, 36042, 6584, 36584},    /* DDR3-800 SC */
};

static const struct cxsr_latency *intel_get_cxsr_latency(bool is_desktop,
							 bool is_ddr3,
							 int fsb,
							 int mem)
{
	const struct cxsr_latency *latency;
	int i;

	if (fsb == 0 || mem == 0)
		return NULL;

	for (i = 0; i < ARRAY_SIZE(cxsr_latency_table); i++) {
		latency = &cxsr_latency_table[i];
		if (is_desktop == latency->is_desktop &&
		    is_ddr3 == latency->is_ddr3 &&
		    fsb == latency->fsb_freq && mem == latency->mem_freq)
			return latency;
	}

	DRM_DEBUG_KMS("Unknown FSB/MEM found, disable CxSR\n");

	return NULL;
}

static void chv_set_memory_dvfs(struct drm_i915_private *dev_priv, bool enable)
{
	u32 val;

<<<<<<< HEAD
	mutex_lock(&dev_priv->pcu_lock);
=======
	vlv_punit_get(dev_priv);
>>>>>>> upstream/android-13

	val = vlv_punit_read(dev_priv, PUNIT_REG_DDR_SETUP2);
	if (enable)
		val &= ~FORCE_DDR_HIGH_FREQ;
	else
		val |= FORCE_DDR_HIGH_FREQ;
	val &= ~FORCE_DDR_LOW_FREQ;
	val |= FORCE_DDR_FREQ_REQ_ACK;
	vlv_punit_write(dev_priv, PUNIT_REG_DDR_SETUP2, val);

	if (wait_for((vlv_punit_read(dev_priv, PUNIT_REG_DDR_SETUP2) &
		      FORCE_DDR_FREQ_REQ_ACK) == 0, 3))
<<<<<<< HEAD
		DRM_ERROR("timed out waiting for Punit DDR DVFS request\n");

	mutex_unlock(&dev_priv->pcu_lock);
=======
		drm_err(&dev_priv->drm,
			"timed out waiting for Punit DDR DVFS request\n");

	vlv_punit_put(dev_priv);
>>>>>>> upstream/android-13
}

static void chv_set_memory_pm5(struct drm_i915_private *dev_priv, bool enable)
{
	u32 val;

<<<<<<< HEAD
	mutex_lock(&dev_priv->pcu_lock);

	val = vlv_punit_read(dev_priv, PUNIT_REG_DSPFREQ);
=======
	vlv_punit_get(dev_priv);

	val = vlv_punit_read(dev_priv, PUNIT_REG_DSPSSPM);
>>>>>>> upstream/android-13
	if (enable)
		val |= DSP_MAXFIFO_PM5_ENABLE;
	else
		val &= ~DSP_MAXFIFO_PM5_ENABLE;
<<<<<<< HEAD
	vlv_punit_write(dev_priv, PUNIT_REG_DSPFREQ, val);

	mutex_unlock(&dev_priv->pcu_lock);
=======
	vlv_punit_write(dev_priv, PUNIT_REG_DSPSSPM, val);

	vlv_punit_put(dev_priv);
>>>>>>> upstream/android-13
}

#define FW_WM(value, plane) \
	(((value) << DSPFW_ ## plane ## _SHIFT) & DSPFW_ ## plane ## _MASK)

static bool _intel_set_memory_cxsr(struct drm_i915_private *dev_priv, bool enable)
{
	bool was_enabled;
	u32 val;

	if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv)) {
<<<<<<< HEAD
		was_enabled = I915_READ(FW_BLC_SELF_VLV) & FW_CSPWRDWNEN;
		I915_WRITE(FW_BLC_SELF_VLV, enable ? FW_CSPWRDWNEN : 0);
		POSTING_READ(FW_BLC_SELF_VLV);
	} else if (IS_G4X(dev_priv) || IS_I965GM(dev_priv)) {
		was_enabled = I915_READ(FW_BLC_SELF) & FW_BLC_SELF_EN;
		I915_WRITE(FW_BLC_SELF, enable ? FW_BLC_SELF_EN : 0);
		POSTING_READ(FW_BLC_SELF);
	} else if (IS_PINEVIEW(dev_priv)) {
		val = I915_READ(DSPFW3);
=======
		was_enabled = intel_uncore_read(&dev_priv->uncore, FW_BLC_SELF_VLV) & FW_CSPWRDWNEN;
		intel_uncore_write(&dev_priv->uncore, FW_BLC_SELF_VLV, enable ? FW_CSPWRDWNEN : 0);
		intel_uncore_posting_read(&dev_priv->uncore, FW_BLC_SELF_VLV);
	} else if (IS_G4X(dev_priv) || IS_I965GM(dev_priv)) {
		was_enabled = intel_uncore_read(&dev_priv->uncore, FW_BLC_SELF) & FW_BLC_SELF_EN;
		intel_uncore_write(&dev_priv->uncore, FW_BLC_SELF, enable ? FW_BLC_SELF_EN : 0);
		intel_uncore_posting_read(&dev_priv->uncore, FW_BLC_SELF);
	} else if (IS_PINEVIEW(dev_priv)) {
		val = intel_uncore_read(&dev_priv->uncore, DSPFW3);
>>>>>>> upstream/android-13
		was_enabled = val & PINEVIEW_SELF_REFRESH_EN;
		if (enable)
			val |= PINEVIEW_SELF_REFRESH_EN;
		else
			val &= ~PINEVIEW_SELF_REFRESH_EN;
<<<<<<< HEAD
		I915_WRITE(DSPFW3, val);
		POSTING_READ(DSPFW3);
	} else if (IS_I945G(dev_priv) || IS_I945GM(dev_priv)) {
		was_enabled = I915_READ(FW_BLC_SELF) & FW_BLC_SELF_EN;
		val = enable ? _MASKED_BIT_ENABLE(FW_BLC_SELF_EN) :
			       _MASKED_BIT_DISABLE(FW_BLC_SELF_EN);
		I915_WRITE(FW_BLC_SELF, val);
		POSTING_READ(FW_BLC_SELF);
=======
		intel_uncore_write(&dev_priv->uncore, DSPFW3, val);
		intel_uncore_posting_read(&dev_priv->uncore, DSPFW3);
	} else if (IS_I945G(dev_priv) || IS_I945GM(dev_priv)) {
		was_enabled = intel_uncore_read(&dev_priv->uncore, FW_BLC_SELF) & FW_BLC_SELF_EN;
		val = enable ? _MASKED_BIT_ENABLE(FW_BLC_SELF_EN) :
			       _MASKED_BIT_DISABLE(FW_BLC_SELF_EN);
		intel_uncore_write(&dev_priv->uncore, FW_BLC_SELF, val);
		intel_uncore_posting_read(&dev_priv->uncore, FW_BLC_SELF);
>>>>>>> upstream/android-13
	} else if (IS_I915GM(dev_priv)) {
		/*
		 * FIXME can't find a bit like this for 915G, and
		 * and yet it does have the related watermark in
		 * FW_BLC_SELF. What's going on?
		 */
<<<<<<< HEAD
		was_enabled = I915_READ(INSTPM) & INSTPM_SELF_EN;
		val = enable ? _MASKED_BIT_ENABLE(INSTPM_SELF_EN) :
			       _MASKED_BIT_DISABLE(INSTPM_SELF_EN);
		I915_WRITE(INSTPM, val);
		POSTING_READ(INSTPM);
=======
		was_enabled = intel_uncore_read(&dev_priv->uncore, INSTPM) & INSTPM_SELF_EN;
		val = enable ? _MASKED_BIT_ENABLE(INSTPM_SELF_EN) :
			       _MASKED_BIT_DISABLE(INSTPM_SELF_EN);
		intel_uncore_write(&dev_priv->uncore, INSTPM, val);
		intel_uncore_posting_read(&dev_priv->uncore, INSTPM);
>>>>>>> upstream/android-13
	} else {
		return false;
	}

	trace_intel_memory_cxsr(dev_priv, was_enabled, enable);

<<<<<<< HEAD
	DRM_DEBUG_KMS("memory self-refresh is %s (was %s)\n",
		      enableddisabled(enable),
		      enableddisabled(was_enabled));
=======
	drm_dbg_kms(&dev_priv->drm, "memory self-refresh is %s (was %s)\n",
		    enableddisabled(enable),
		    enableddisabled(was_enabled));
>>>>>>> upstream/android-13

	return was_enabled;
}

/**
 * intel_set_memory_cxsr - Configure CxSR state
 * @dev_priv: i915 device
 * @enable: Allow vs. disallow CxSR
 *
 * Allow or disallow the system to enter a special CxSR
 * (C-state self refresh) state. What typically happens in CxSR mode
 * is that several display FIFOs may get combined into a single larger
 * FIFO for a particular plane (so called max FIFO mode) to allow the
 * system to defer memory fetches longer, and the memory will enter
 * self refresh.
 *
 * Note that enabling CxSR does not guarantee that the system enter
 * this special mode, nor does it guarantee that the system stays
 * in that mode once entered. So this just allows/disallows the system
 * to autonomously utilize the CxSR mode. Other factors such as core
 * C-states will affect when/if the system actually enters/exits the
 * CxSR mode.
 *
 * Note that on VLV/CHV this actually only controls the max FIFO mode,
 * and the system is free to enter/exit memory self refresh at any time
 * even when the use of CxSR has been disallowed.
 *
 * While the system is actually in the CxSR/max FIFO mode, some plane
 * control registers will not get latched on vblank. Thus in order to
 * guarantee the system will respond to changes in the plane registers
 * we must always disallow CxSR prior to making changes to those registers.
 * Unfortunately the system will re-evaluate the CxSR conditions at
 * frame start which happens after vblank start (which is when the plane
 * registers would get latched), so we can't proceed with the plane update
 * during the same frame where we disallowed CxSR.
 *
 * Certain platforms also have a deeper HPLL SR mode. Fortunately the
 * HPLL SR mode depends on CxSR itself, so we don't have to hand hold
 * the hardware w.r.t. HPLL SR when writing to plane registers.
 * Disallowing just CxSR is sufficient.
 */
bool intel_set_memory_cxsr(struct drm_i915_private *dev_priv, bool enable)
{
	bool ret;

	mutex_lock(&dev_priv->wm.wm_mutex);
	ret = _intel_set_memory_cxsr(dev_priv, enable);
	if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv))
		dev_priv->wm.vlv.cxsr = enable;
	else if (IS_G4X(dev_priv))
		dev_priv->wm.g4x.cxsr = enable;
	mutex_unlock(&dev_priv->wm.wm_mutex);

	return ret;
}

/*
 * Latency for FIFO fetches is dependent on several factors:
 *   - memory configuration (speed, channels)
 *   - chipset
 *   - current MCH state
 * It can be fairly high in some situations, so here we assume a fairly
 * pessimal value.  It's a tradeoff between extra memory fetches (if we
 * set this value too high, the FIFO will fetch frequently to stay full)
 * and power consumption (set it too low to save power and we might see
 * FIFO underruns and display "flicker").
 *
 * A value of 5us seems to be a good balance; safe for very low end
 * platforms but not overly aggressive on lower latency configs.
 */
static const int pessimal_latency_ns = 5000;

#define VLV_FIFO_START(dsparb, dsparb2, lo_shift, hi_shift) \
	((((dsparb) >> (lo_shift)) & 0xff) | ((((dsparb2) >> (hi_shift)) & 0x1) << 8))

static void vlv_get_fifo_size(struct intel_crtc_state *crtc_state)
{
<<<<<<< HEAD
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->base.crtc);
=======
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->uapi.crtc);
>>>>>>> upstream/android-13
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	struct vlv_fifo_state *fifo_state = &crtc_state->wm.vlv.fifo_state;
	enum pipe pipe = crtc->pipe;
	int sprite0_start, sprite1_start;
<<<<<<< HEAD

	switch (pipe) {
		uint32_t dsparb, dsparb2, dsparb3;
	case PIPE_A:
		dsparb = I915_READ(DSPARB);
		dsparb2 = I915_READ(DSPARB2);
=======
	u32 dsparb, dsparb2, dsparb3;

	switch (pipe) {
	case PIPE_A:
		dsparb = intel_uncore_read(&dev_priv->uncore, DSPARB);
		dsparb2 = intel_uncore_read(&dev_priv->uncore, DSPARB2);
>>>>>>> upstream/android-13
		sprite0_start = VLV_FIFO_START(dsparb, dsparb2, 0, 0);
		sprite1_start = VLV_FIFO_START(dsparb, dsparb2, 8, 4);
		break;
	case PIPE_B:
<<<<<<< HEAD
		dsparb = I915_READ(DSPARB);
		dsparb2 = I915_READ(DSPARB2);
=======
		dsparb = intel_uncore_read(&dev_priv->uncore, DSPARB);
		dsparb2 = intel_uncore_read(&dev_priv->uncore, DSPARB2);
>>>>>>> upstream/android-13
		sprite0_start = VLV_FIFO_START(dsparb, dsparb2, 16, 8);
		sprite1_start = VLV_FIFO_START(dsparb, dsparb2, 24, 12);
		break;
	case PIPE_C:
<<<<<<< HEAD
		dsparb2 = I915_READ(DSPARB2);
		dsparb3 = I915_READ(DSPARB3);
=======
		dsparb2 = intel_uncore_read(&dev_priv->uncore, DSPARB2);
		dsparb3 = intel_uncore_read(&dev_priv->uncore, DSPARB3);
>>>>>>> upstream/android-13
		sprite0_start = VLV_FIFO_START(dsparb3, dsparb2, 0, 16);
		sprite1_start = VLV_FIFO_START(dsparb3, dsparb2, 8, 20);
		break;
	default:
		MISSING_CASE(pipe);
		return;
	}

	fifo_state->plane[PLANE_PRIMARY] = sprite0_start;
	fifo_state->plane[PLANE_SPRITE0] = sprite1_start - sprite0_start;
	fifo_state->plane[PLANE_SPRITE1] = 511 - sprite1_start;
	fifo_state->plane[PLANE_CURSOR] = 63;
}

static int i9xx_get_fifo_size(struct drm_i915_private *dev_priv,
			      enum i9xx_plane_id i9xx_plane)
{
<<<<<<< HEAD
	uint32_t dsparb = I915_READ(DSPARB);
=======
	u32 dsparb = intel_uncore_read(&dev_priv->uncore, DSPARB);
>>>>>>> upstream/android-13
	int size;

	size = dsparb & 0x7f;
	if (i9xx_plane == PLANE_B)
		size = ((dsparb >> DSPARB_CSTART_SHIFT) & 0x7f) - size;

<<<<<<< HEAD
	DRM_DEBUG_KMS("FIFO size - (0x%08x) %c: %d\n",
		      dsparb, plane_name(i9xx_plane), size);
=======
	drm_dbg_kms(&dev_priv->drm, "FIFO size - (0x%08x) %c: %d\n",
		    dsparb, plane_name(i9xx_plane), size);
>>>>>>> upstream/android-13

	return size;
}

static int i830_get_fifo_size(struct drm_i915_private *dev_priv,
			      enum i9xx_plane_id i9xx_plane)
{
<<<<<<< HEAD
	uint32_t dsparb = I915_READ(DSPARB);
=======
	u32 dsparb = intel_uncore_read(&dev_priv->uncore, DSPARB);
>>>>>>> upstream/android-13
	int size;

	size = dsparb & 0x1ff;
	if (i9xx_plane == PLANE_B)
		size = ((dsparb >> DSPARB_BEND_SHIFT) & 0x1ff) - size;
	size >>= 1; /* Convert to cachelines */

<<<<<<< HEAD
	DRM_DEBUG_KMS("FIFO size - (0x%08x) %c: %d\n",
		      dsparb, plane_name(i9xx_plane), size);
=======
	drm_dbg_kms(&dev_priv->drm, "FIFO size - (0x%08x) %c: %d\n",
		    dsparb, plane_name(i9xx_plane), size);
>>>>>>> upstream/android-13

	return size;
}

static int i845_get_fifo_size(struct drm_i915_private *dev_priv,
			      enum i9xx_plane_id i9xx_plane)
{
<<<<<<< HEAD
	uint32_t dsparb = I915_READ(DSPARB);
=======
	u32 dsparb = intel_uncore_read(&dev_priv->uncore, DSPARB);
>>>>>>> upstream/android-13
	int size;

	size = dsparb & 0x7f;
	size >>= 2; /* Convert to cachelines */

<<<<<<< HEAD
	DRM_DEBUG_KMS("FIFO size - (0x%08x) %c: %d\n",
		      dsparb, plane_name(i9xx_plane), size);
=======
	drm_dbg_kms(&dev_priv->drm, "FIFO size - (0x%08x) %c: %d\n",
		    dsparb, plane_name(i9xx_plane), size);
>>>>>>> upstream/android-13

	return size;
}

/* Pineview has different values for various configs */
<<<<<<< HEAD
static const struct intel_watermark_params pineview_display_wm = {
=======
static const struct intel_watermark_params pnv_display_wm = {
>>>>>>> upstream/android-13
	.fifo_size = PINEVIEW_DISPLAY_FIFO,
	.max_wm = PINEVIEW_MAX_WM,
	.default_wm = PINEVIEW_DFT_WM,
	.guard_size = PINEVIEW_GUARD_WM,
	.cacheline_size = PINEVIEW_FIFO_LINE_SIZE,
};
<<<<<<< HEAD
static const struct intel_watermark_params pineview_display_hplloff_wm = {
=======

static const struct intel_watermark_params pnv_display_hplloff_wm = {
>>>>>>> upstream/android-13
	.fifo_size = PINEVIEW_DISPLAY_FIFO,
	.max_wm = PINEVIEW_MAX_WM,
	.default_wm = PINEVIEW_DFT_HPLLOFF_WM,
	.guard_size = PINEVIEW_GUARD_WM,
	.cacheline_size = PINEVIEW_FIFO_LINE_SIZE,
};
<<<<<<< HEAD
static const struct intel_watermark_params pineview_cursor_wm = {
=======

static const struct intel_watermark_params pnv_cursor_wm = {
>>>>>>> upstream/android-13
	.fifo_size = PINEVIEW_CURSOR_FIFO,
	.max_wm = PINEVIEW_CURSOR_MAX_WM,
	.default_wm = PINEVIEW_CURSOR_DFT_WM,
	.guard_size = PINEVIEW_CURSOR_GUARD_WM,
	.cacheline_size = PINEVIEW_FIFO_LINE_SIZE,
};
<<<<<<< HEAD
static const struct intel_watermark_params pineview_cursor_hplloff_wm = {
=======

static const struct intel_watermark_params pnv_cursor_hplloff_wm = {
>>>>>>> upstream/android-13
	.fifo_size = PINEVIEW_CURSOR_FIFO,
	.max_wm = PINEVIEW_CURSOR_MAX_WM,
	.default_wm = PINEVIEW_CURSOR_DFT_WM,
	.guard_size = PINEVIEW_CURSOR_GUARD_WM,
	.cacheline_size = PINEVIEW_FIFO_LINE_SIZE,
};
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
static const struct intel_watermark_params i965_cursor_wm_info = {
	.fifo_size = I965_CURSOR_FIFO,
	.max_wm = I965_CURSOR_MAX_WM,
	.default_wm = I965_CURSOR_DFT_WM,
	.guard_size = 2,
	.cacheline_size = I915_FIFO_LINE_SIZE,
};
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
static const struct intel_watermark_params i945_wm_info = {
	.fifo_size = I945_FIFO_SIZE,
	.max_wm = I915_MAX_WM,
	.default_wm = 1,
	.guard_size = 2,
	.cacheline_size = I915_FIFO_LINE_SIZE,
};
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
static const struct intel_watermark_params i915_wm_info = {
	.fifo_size = I915_FIFO_SIZE,
	.max_wm = I915_MAX_WM,
	.default_wm = 1,
	.guard_size = 2,
	.cacheline_size = I915_FIFO_LINE_SIZE,
};
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
static const struct intel_watermark_params i830_a_wm_info = {
	.fifo_size = I855GM_FIFO_SIZE,
	.max_wm = I915_MAX_WM,
	.default_wm = 1,
	.guard_size = 2,
	.cacheline_size = I830_FIFO_LINE_SIZE,
};
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
static const struct intel_watermark_params i830_bc_wm_info = {
	.fifo_size = I855GM_FIFO_SIZE,
	.max_wm = I915_MAX_WM/2,
	.default_wm = 1,
	.guard_size = 2,
	.cacheline_size = I830_FIFO_LINE_SIZE,
};
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
static const struct intel_watermark_params i845_wm_info = {
	.fifo_size = I830_FIFO_SIZE,
	.max_wm = I915_MAX_WM,
	.default_wm = 1,
	.guard_size = 2,
	.cacheline_size = I830_FIFO_LINE_SIZE,
};

/**
 * intel_wm_method1 - Method 1 / "small buffer" watermark formula
 * @pixel_rate: Pipe pixel rate in kHz
 * @cpp: Plane bytes per pixel
 * @latency: Memory wakeup latency in 0.1us units
 *
 * Compute the watermark using the method 1 or "small buffer"
 * formula. The caller may additonally add extra cachelines
 * to account for TLB misses and clock crossings.
 *
 * This method is concerned with the short term drain rate
 * of the FIFO, ie. it does not account for blanking periods
 * which would effectively reduce the average drain rate across
 * a longer period. The name "small" refers to the fact the
 * FIFO is relatively small compared to the amount of data
 * fetched.
 *
 * The FIFO level vs. time graph might look something like:
 *
 *   |\   |\
 *   | \  | \
 * __---__---__ (- plane active, _ blanking)
 * -> time
 *
 * or perhaps like this:
 *
 *   |\|\  |\|\
 * __----__----__ (- plane active, _ blanking)
 * -> time
 *
 * Returns:
 * The watermark in bytes
 */
static unsigned int intel_wm_method1(unsigned int pixel_rate,
				     unsigned int cpp,
				     unsigned int latency)
{
<<<<<<< HEAD
	uint64_t ret;

	ret = (uint64_t) pixel_rate * cpp * latency;
=======
	u64 ret;

	ret = mul_u32_u32(pixel_rate, cpp * latency);
>>>>>>> upstream/android-13
	ret = DIV_ROUND_UP_ULL(ret, 10000);

	return ret;
}

/**
 * intel_wm_method2 - Method 2 / "large buffer" watermark formula
 * @pixel_rate: Pipe pixel rate in kHz
 * @htotal: Pipe horizontal total
 * @width: Plane width in pixels
 * @cpp: Plane bytes per pixel
 * @latency: Memory wakeup latency in 0.1us units
 *
 * Compute the watermark using the method 2 or "large buffer"
 * formula. The caller may additonally add extra cachelines
 * to account for TLB misses and clock crossings.
 *
 * This method is concerned with the long term drain rate
 * of the FIFO, ie. it does account for blanking periods
 * which effectively reduce the average drain rate across
 * a longer period. The name "large" refers to the fact the
 * FIFO is relatively large compared to the amount of data
 * fetched.
 *
 * The FIFO level vs. time graph might look something like:
 *
 *    |\___       |\___
 *    |    \___   |    \___
 *    |        \  |        \
 * __ --__--__--__--__--__--__ (- plane active, _ blanking)
 * -> time
 *
 * Returns:
 * The watermark in bytes
 */
static unsigned int intel_wm_method2(unsigned int pixel_rate,
				     unsigned int htotal,
				     unsigned int width,
				     unsigned int cpp,
				     unsigned int latency)
{
	unsigned int ret;

	/*
	 * FIXME remove once all users are computing
	 * watermarks in the correct place.
	 */
	if (WARN_ON_ONCE(htotal == 0))
		htotal = 1;

	ret = (latency * pixel_rate) / (htotal * 10000);
	ret = (ret + 1) * width * cpp;

	return ret;
}

/**
 * intel_calculate_wm - calculate watermark level
 * @pixel_rate: pixel clock
 * @wm: chip FIFO params
 * @fifo_size: size of the FIFO buffer
 * @cpp: bytes per pixel
 * @latency_ns: memory latency for the platform
 *
 * Calculate the watermark level (the level at which the display plane will
 * start fetching from memory again).  Each chip has a different display
 * FIFO size and allocation, so the caller needs to figure that out and pass
 * in the correct intel_watermark_params structure.
 *
 * As the pixel clock runs, the FIFO will be drained at a rate that depends
 * on the pixel size.  When it reaches the watermark level, it'll start
 * fetching FIFO line sized based chunks from memory until the FIFO fills
 * past the watermark point.  If the FIFO drains completely, a FIFO underrun
 * will occur, and a display engine hang could result.
 */
static unsigned int intel_calculate_wm(int pixel_rate,
				       const struct intel_watermark_params *wm,
				       int fifo_size, int cpp,
				       unsigned int latency_ns)
{
	int entries, wm_size;

	/*
	 * Note: we need to make sure we don't overflow for various clock &
	 * latency values.
	 * clocks go from a few thousand to several hundred thousand.
	 * latency is usually a few thousand
	 */
	entries = intel_wm_method1(pixel_rate, cpp,
				   latency_ns / 100);
	entries = DIV_ROUND_UP(entries, wm->cacheline_size) +
		wm->guard_size;
	DRM_DEBUG_KMS("FIFO entries required for mode: %d\n", entries);

	wm_size = fifo_size - entries;
	DRM_DEBUG_KMS("FIFO watermark level: %d\n", wm_size);

	/* Don't promote wm_size to unsigned... */
	if (wm_size > wm->max_wm)
		wm_size = wm->max_wm;
	if (wm_size <= 0)
		wm_size = wm->default_wm;

	/*
	 * Bspec seems to indicate that the value shouldn't be lower than
	 * 'burst size + 1'. Certainly 830 is quite unhappy with low values.
	 * Lets go for 8 which is the burst size since certain platforms
	 * already use a hardcoded 8 (which is what the spec says should be
	 * done).
	 */
	if (wm_size <= 8)
		wm_size = 8;

	return wm_size;
}

static bool is_disabling(int old, int new, int threshold)
{
	return old >= threshold && new < threshold;
}

static bool is_enabling(int old, int new, int threshold)
{
	return old < threshold && new >= threshold;
}

static int intel_wm_num_levels(struct drm_i915_private *dev_priv)
{
	return dev_priv->wm.max_level + 1;
}

static bool intel_wm_plane_visible(const struct intel_crtc_state *crtc_state,
				   const struct intel_plane_state *plane_state)
{
<<<<<<< HEAD
	struct intel_plane *plane = to_intel_plane(plane_state->base.plane);

	/* FIXME check the 'enable' instead */
	if (!crtc_state->base.active)
=======
	struct intel_plane *plane = to_intel_plane(plane_state->uapi.plane);

	/* FIXME check the 'enable' instead */
	if (!crtc_state->hw.active)
>>>>>>> upstream/android-13
		return false;

	/*
	 * Treat cursor with fb as always visible since cursor updates
	 * can happen faster than the vrefresh rate, and the current
	 * watermark code doesn't handle that correctly. Cursor updates
	 * which set/clear the fb or change the cursor size are going
	 * to get throttled by intel_legacy_cursor_update() to work
	 * around this problem with the watermark code.
	 */
	if (plane->id == PLANE_CURSOR)
<<<<<<< HEAD
		return plane_state->base.fb != NULL;
	else
		return plane_state->base.visible;
=======
		return plane_state->hw.fb != NULL;
	else
		return plane_state->uapi.visible;
}

static bool intel_crtc_active(struct intel_crtc *crtc)
{
	/* Be paranoid as we can arrive here with only partial
	 * state retrieved from the hardware during setup.
	 *
	 * We can ditch the adjusted_mode.crtc_clock check as soon
	 * as Haswell has gained clock readout/fastboot support.
	 *
	 * We can ditch the crtc->primary->state->fb check as soon as we can
	 * properly reconstruct framebuffers.
	 *
	 * FIXME: The intel_crtc->active here should be switched to
	 * crtc->state->active once we have proper CRTC states wired up
	 * for atomic.
	 */
	return crtc->active && crtc->base.primary->state->fb &&
		crtc->config->hw.adjusted_mode.crtc_clock;
>>>>>>> upstream/android-13
}

static struct intel_crtc *single_enabled_crtc(struct drm_i915_private *dev_priv)
{
	struct intel_crtc *crtc, *enabled = NULL;

	for_each_intel_crtc(&dev_priv->drm, crtc) {
		if (intel_crtc_active(crtc)) {
			if (enabled)
				return NULL;
			enabled = crtc;
		}
	}

	return enabled;
}

<<<<<<< HEAD
static void pineview_update_wm(struct intel_crtc *unused_crtc)
=======
static void pnv_update_wm(struct intel_crtc *unused_crtc)
>>>>>>> upstream/android-13
{
	struct drm_i915_private *dev_priv = to_i915(unused_crtc->base.dev);
	struct intel_crtc *crtc;
	const struct cxsr_latency *latency;
	u32 reg;
	unsigned int wm;

<<<<<<< HEAD
	latency = intel_get_cxsr_latency(IS_PINEVIEW_G(dev_priv),
=======
	latency = intel_get_cxsr_latency(!IS_MOBILE(dev_priv),
>>>>>>> upstream/android-13
					 dev_priv->is_ddr3,
					 dev_priv->fsb_freq,
					 dev_priv->mem_freq);
	if (!latency) {
<<<<<<< HEAD
		DRM_DEBUG_KMS("Unknown FSB/MEM found, disable CxSR\n");
=======
		drm_dbg_kms(&dev_priv->drm,
			    "Unknown FSB/MEM found, disable CxSR\n");
>>>>>>> upstream/android-13
		intel_set_memory_cxsr(dev_priv, false);
		return;
	}

	crtc = single_enabled_crtc(dev_priv);
	if (crtc) {
<<<<<<< HEAD
		const struct drm_display_mode *adjusted_mode =
			&crtc->config->base.adjusted_mode;
		const struct drm_framebuffer *fb =
			crtc->base.primary->state->fb;
		int cpp = fb->format->cpp[0];
		int clock = adjusted_mode->crtc_clock;

		/* Display SR */
		wm = intel_calculate_wm(clock, &pineview_display_wm,
					pineview_display_wm.fifo_size,
					cpp, latency->display_sr);
		reg = I915_READ(DSPFW1);
		reg &= ~DSPFW_SR_MASK;
		reg |= FW_WM(wm, SR);
		I915_WRITE(DSPFW1, reg);
		DRM_DEBUG_KMS("DSPFW1 register is %x\n", reg);

		/* cursor SR */
		wm = intel_calculate_wm(clock, &pineview_cursor_wm,
					pineview_display_wm.fifo_size,
					4, latency->cursor_sr);
		reg = I915_READ(DSPFW3);
		reg &= ~DSPFW_CURSOR_SR_MASK;
		reg |= FW_WM(wm, CURSOR_SR);
		I915_WRITE(DSPFW3, reg);

		/* Display HPLL off SR */
		wm = intel_calculate_wm(clock, &pineview_display_hplloff_wm,
					pineview_display_hplloff_wm.fifo_size,
					cpp, latency->display_hpll_disable);
		reg = I915_READ(DSPFW3);
		reg &= ~DSPFW_HPLL_SR_MASK;
		reg |= FW_WM(wm, HPLL_SR);
		I915_WRITE(DSPFW3, reg);

		/* cursor HPLL off SR */
		wm = intel_calculate_wm(clock, &pineview_cursor_hplloff_wm,
					pineview_display_hplloff_wm.fifo_size,
					4, latency->cursor_hpll_disable);
		reg = I915_READ(DSPFW3);
		reg &= ~DSPFW_HPLL_CURSOR_MASK;
		reg |= FW_WM(wm, HPLL_CURSOR);
		I915_WRITE(DSPFW3, reg);
		DRM_DEBUG_KMS("DSPFW3 register is %x\n", reg);
=======
		const struct drm_display_mode *pipe_mode =
			&crtc->config->hw.pipe_mode;
		const struct drm_framebuffer *fb =
			crtc->base.primary->state->fb;
		int cpp = fb->format->cpp[0];
		int clock = pipe_mode->crtc_clock;

		/* Display SR */
		wm = intel_calculate_wm(clock, &pnv_display_wm,
					pnv_display_wm.fifo_size,
					cpp, latency->display_sr);
		reg = intel_uncore_read(&dev_priv->uncore, DSPFW1);
		reg &= ~DSPFW_SR_MASK;
		reg |= FW_WM(wm, SR);
		intel_uncore_write(&dev_priv->uncore, DSPFW1, reg);
		drm_dbg_kms(&dev_priv->drm, "DSPFW1 register is %x\n", reg);

		/* cursor SR */
		wm = intel_calculate_wm(clock, &pnv_cursor_wm,
					pnv_display_wm.fifo_size,
					4, latency->cursor_sr);
		reg = intel_uncore_read(&dev_priv->uncore, DSPFW3);
		reg &= ~DSPFW_CURSOR_SR_MASK;
		reg |= FW_WM(wm, CURSOR_SR);
		intel_uncore_write(&dev_priv->uncore, DSPFW3, reg);

		/* Display HPLL off SR */
		wm = intel_calculate_wm(clock, &pnv_display_hplloff_wm,
					pnv_display_hplloff_wm.fifo_size,
					cpp, latency->display_hpll_disable);
		reg = intel_uncore_read(&dev_priv->uncore, DSPFW3);
		reg &= ~DSPFW_HPLL_SR_MASK;
		reg |= FW_WM(wm, HPLL_SR);
		intel_uncore_write(&dev_priv->uncore, DSPFW3, reg);

		/* cursor HPLL off SR */
		wm = intel_calculate_wm(clock, &pnv_cursor_hplloff_wm,
					pnv_display_hplloff_wm.fifo_size,
					4, latency->cursor_hpll_disable);
		reg = intel_uncore_read(&dev_priv->uncore, DSPFW3);
		reg &= ~DSPFW_HPLL_CURSOR_MASK;
		reg |= FW_WM(wm, HPLL_CURSOR);
		intel_uncore_write(&dev_priv->uncore, DSPFW3, reg);
		drm_dbg_kms(&dev_priv->drm, "DSPFW3 register is %x\n", reg);
>>>>>>> upstream/android-13

		intel_set_memory_cxsr(dev_priv, true);
	} else {
		intel_set_memory_cxsr(dev_priv, false);
	}
}

/*
 * Documentation says:
 * "If the line size is small, the TLB fetches can get in the way of the
 *  data fetches, causing some lag in the pixel data return which is not
 *  accounted for in the above formulas. The following adjustment only
 *  needs to be applied if eight whole lines fit in the buffer at once.
 *  The WM is adjusted upwards by the difference between the FIFO size
 *  and the size of 8 whole lines. This adjustment is always performed
 *  in the actual pixel depth regardless of whether FBC is enabled or not."
 */
static unsigned int g4x_tlb_miss_wa(int fifo_size, int width, int cpp)
{
	int tlb_miss = fifo_size * 64 - width * cpp * 8;

	return max(0, tlb_miss);
}

static void g4x_write_wm_values(struct drm_i915_private *dev_priv,
				const struct g4x_wm_values *wm)
{
	enum pipe pipe;

	for_each_pipe(dev_priv, pipe)
		trace_g4x_wm(intel_get_crtc_for_pipe(dev_priv, pipe), wm);

<<<<<<< HEAD
	I915_WRITE(DSPFW1,
=======
	intel_uncore_write(&dev_priv->uncore, DSPFW1,
>>>>>>> upstream/android-13
		   FW_WM(wm->sr.plane, SR) |
		   FW_WM(wm->pipe[PIPE_B].plane[PLANE_CURSOR], CURSORB) |
		   FW_WM(wm->pipe[PIPE_B].plane[PLANE_PRIMARY], PLANEB) |
		   FW_WM(wm->pipe[PIPE_A].plane[PLANE_PRIMARY], PLANEA));
<<<<<<< HEAD
	I915_WRITE(DSPFW2,
=======
	intel_uncore_write(&dev_priv->uncore, DSPFW2,
>>>>>>> upstream/android-13
		   (wm->fbc_en ? DSPFW_FBC_SR_EN : 0) |
		   FW_WM(wm->sr.fbc, FBC_SR) |
		   FW_WM(wm->hpll.fbc, FBC_HPLL_SR) |
		   FW_WM(wm->pipe[PIPE_B].plane[PLANE_SPRITE0], SPRITEB) |
		   FW_WM(wm->pipe[PIPE_A].plane[PLANE_CURSOR], CURSORA) |
		   FW_WM(wm->pipe[PIPE_A].plane[PLANE_SPRITE0], SPRITEA));
<<<<<<< HEAD
	I915_WRITE(DSPFW3,
=======
	intel_uncore_write(&dev_priv->uncore, DSPFW3,
>>>>>>> upstream/android-13
		   (wm->hpll_en ? DSPFW_HPLL_SR_EN : 0) |
		   FW_WM(wm->sr.cursor, CURSOR_SR) |
		   FW_WM(wm->hpll.cursor, HPLL_CURSOR) |
		   FW_WM(wm->hpll.plane, HPLL_SR));

<<<<<<< HEAD
	POSTING_READ(DSPFW1);
=======
	intel_uncore_posting_read(&dev_priv->uncore, DSPFW1);
>>>>>>> upstream/android-13
}

#define FW_WM_VLV(value, plane) \
	(((value) << DSPFW_ ## plane ## _SHIFT) & DSPFW_ ## plane ## _MASK_VLV)

static void vlv_write_wm_values(struct drm_i915_private *dev_priv,
				const struct vlv_wm_values *wm)
{
	enum pipe pipe;

	for_each_pipe(dev_priv, pipe) {
		trace_vlv_wm(intel_get_crtc_for_pipe(dev_priv, pipe), wm);

<<<<<<< HEAD
		I915_WRITE(VLV_DDL(pipe),
=======
		intel_uncore_write(&dev_priv->uncore, VLV_DDL(pipe),
>>>>>>> upstream/android-13
			   (wm->ddl[pipe].plane[PLANE_CURSOR] << DDL_CURSOR_SHIFT) |
			   (wm->ddl[pipe].plane[PLANE_SPRITE1] << DDL_SPRITE_SHIFT(1)) |
			   (wm->ddl[pipe].plane[PLANE_SPRITE0] << DDL_SPRITE_SHIFT(0)) |
			   (wm->ddl[pipe].plane[PLANE_PRIMARY] << DDL_PLANE_SHIFT));
	}

	/*
	 * Zero the (unused) WM1 watermarks, and also clear all the
	 * high order bits so that there are no out of bounds values
	 * present in the registers during the reprogramming.
	 */
<<<<<<< HEAD
	I915_WRITE(DSPHOWM, 0);
	I915_WRITE(DSPHOWM1, 0);
	I915_WRITE(DSPFW4, 0);
	I915_WRITE(DSPFW5, 0);
	I915_WRITE(DSPFW6, 0);

	I915_WRITE(DSPFW1,
=======
	intel_uncore_write(&dev_priv->uncore, DSPHOWM, 0);
	intel_uncore_write(&dev_priv->uncore, DSPHOWM1, 0);
	intel_uncore_write(&dev_priv->uncore, DSPFW4, 0);
	intel_uncore_write(&dev_priv->uncore, DSPFW5, 0);
	intel_uncore_write(&dev_priv->uncore, DSPFW6, 0);

	intel_uncore_write(&dev_priv->uncore, DSPFW1,
>>>>>>> upstream/android-13
		   FW_WM(wm->sr.plane, SR) |
		   FW_WM(wm->pipe[PIPE_B].plane[PLANE_CURSOR], CURSORB) |
		   FW_WM_VLV(wm->pipe[PIPE_B].plane[PLANE_PRIMARY], PLANEB) |
		   FW_WM_VLV(wm->pipe[PIPE_A].plane[PLANE_PRIMARY], PLANEA));
<<<<<<< HEAD
	I915_WRITE(DSPFW2,
		   FW_WM_VLV(wm->pipe[PIPE_A].plane[PLANE_SPRITE1], SPRITEB) |
		   FW_WM(wm->pipe[PIPE_A].plane[PLANE_CURSOR], CURSORA) |
		   FW_WM_VLV(wm->pipe[PIPE_A].plane[PLANE_SPRITE0], SPRITEA));
	I915_WRITE(DSPFW3,
		   FW_WM(wm->sr.cursor, CURSOR_SR));

	if (IS_CHERRYVIEW(dev_priv)) {
		I915_WRITE(DSPFW7_CHV,
			   FW_WM_VLV(wm->pipe[PIPE_B].plane[PLANE_SPRITE1], SPRITED) |
			   FW_WM_VLV(wm->pipe[PIPE_B].plane[PLANE_SPRITE0], SPRITEC));
		I915_WRITE(DSPFW8_CHV,
			   FW_WM_VLV(wm->pipe[PIPE_C].plane[PLANE_SPRITE1], SPRITEF) |
			   FW_WM_VLV(wm->pipe[PIPE_C].plane[PLANE_SPRITE0], SPRITEE));
		I915_WRITE(DSPFW9_CHV,
			   FW_WM_VLV(wm->pipe[PIPE_C].plane[PLANE_PRIMARY], PLANEC) |
			   FW_WM(wm->pipe[PIPE_C].plane[PLANE_CURSOR], CURSORC));
		I915_WRITE(DSPHOWM,
=======
	intel_uncore_write(&dev_priv->uncore, DSPFW2,
		   FW_WM_VLV(wm->pipe[PIPE_A].plane[PLANE_SPRITE1], SPRITEB) |
		   FW_WM(wm->pipe[PIPE_A].plane[PLANE_CURSOR], CURSORA) |
		   FW_WM_VLV(wm->pipe[PIPE_A].plane[PLANE_SPRITE0], SPRITEA));
	intel_uncore_write(&dev_priv->uncore, DSPFW3,
		   FW_WM(wm->sr.cursor, CURSOR_SR));

	if (IS_CHERRYVIEW(dev_priv)) {
		intel_uncore_write(&dev_priv->uncore, DSPFW7_CHV,
			   FW_WM_VLV(wm->pipe[PIPE_B].plane[PLANE_SPRITE1], SPRITED) |
			   FW_WM_VLV(wm->pipe[PIPE_B].plane[PLANE_SPRITE0], SPRITEC));
		intel_uncore_write(&dev_priv->uncore, DSPFW8_CHV,
			   FW_WM_VLV(wm->pipe[PIPE_C].plane[PLANE_SPRITE1], SPRITEF) |
			   FW_WM_VLV(wm->pipe[PIPE_C].plane[PLANE_SPRITE0], SPRITEE));
		intel_uncore_write(&dev_priv->uncore, DSPFW9_CHV,
			   FW_WM_VLV(wm->pipe[PIPE_C].plane[PLANE_PRIMARY], PLANEC) |
			   FW_WM(wm->pipe[PIPE_C].plane[PLANE_CURSOR], CURSORC));
		intel_uncore_write(&dev_priv->uncore, DSPHOWM,
>>>>>>> upstream/android-13
			   FW_WM(wm->sr.plane >> 9, SR_HI) |
			   FW_WM(wm->pipe[PIPE_C].plane[PLANE_SPRITE1] >> 8, SPRITEF_HI) |
			   FW_WM(wm->pipe[PIPE_C].plane[PLANE_SPRITE0] >> 8, SPRITEE_HI) |
			   FW_WM(wm->pipe[PIPE_C].plane[PLANE_PRIMARY] >> 8, PLANEC_HI) |
			   FW_WM(wm->pipe[PIPE_B].plane[PLANE_SPRITE1] >> 8, SPRITED_HI) |
			   FW_WM(wm->pipe[PIPE_B].plane[PLANE_SPRITE0] >> 8, SPRITEC_HI) |
			   FW_WM(wm->pipe[PIPE_B].plane[PLANE_PRIMARY] >> 8, PLANEB_HI) |
			   FW_WM(wm->pipe[PIPE_A].plane[PLANE_SPRITE1] >> 8, SPRITEB_HI) |
			   FW_WM(wm->pipe[PIPE_A].plane[PLANE_SPRITE0] >> 8, SPRITEA_HI) |
			   FW_WM(wm->pipe[PIPE_A].plane[PLANE_PRIMARY] >> 8, PLANEA_HI));
	} else {
<<<<<<< HEAD
		I915_WRITE(DSPFW7,
			   FW_WM_VLV(wm->pipe[PIPE_B].plane[PLANE_SPRITE1], SPRITED) |
			   FW_WM_VLV(wm->pipe[PIPE_B].plane[PLANE_SPRITE0], SPRITEC));
		I915_WRITE(DSPHOWM,
=======
		intel_uncore_write(&dev_priv->uncore, DSPFW7,
			   FW_WM_VLV(wm->pipe[PIPE_B].plane[PLANE_SPRITE1], SPRITED) |
			   FW_WM_VLV(wm->pipe[PIPE_B].plane[PLANE_SPRITE0], SPRITEC));
		intel_uncore_write(&dev_priv->uncore, DSPHOWM,
>>>>>>> upstream/android-13
			   FW_WM(wm->sr.plane >> 9, SR_HI) |
			   FW_WM(wm->pipe[PIPE_B].plane[PLANE_SPRITE1] >> 8, SPRITED_HI) |
			   FW_WM(wm->pipe[PIPE_B].plane[PLANE_SPRITE0] >> 8, SPRITEC_HI) |
			   FW_WM(wm->pipe[PIPE_B].plane[PLANE_PRIMARY] >> 8, PLANEB_HI) |
			   FW_WM(wm->pipe[PIPE_A].plane[PLANE_SPRITE1] >> 8, SPRITEB_HI) |
			   FW_WM(wm->pipe[PIPE_A].plane[PLANE_SPRITE0] >> 8, SPRITEA_HI) |
			   FW_WM(wm->pipe[PIPE_A].plane[PLANE_PRIMARY] >> 8, PLANEA_HI));
	}

<<<<<<< HEAD
	POSTING_READ(DSPFW1);
=======
	intel_uncore_posting_read(&dev_priv->uncore, DSPFW1);
>>>>>>> upstream/android-13
}

#undef FW_WM_VLV

static void g4x_setup_wm_latency(struct drm_i915_private *dev_priv)
{
	/* all latencies in usec */
	dev_priv->wm.pri_latency[G4X_WM_LEVEL_NORMAL] = 5;
	dev_priv->wm.pri_latency[G4X_WM_LEVEL_SR] = 12;
	dev_priv->wm.pri_latency[G4X_WM_LEVEL_HPLL] = 35;

	dev_priv->wm.max_level = G4X_WM_LEVEL_HPLL;
}

static int g4x_plane_fifo_size(enum plane_id plane_id, int level)
{
	/*
	 * DSPCNTR[13] supposedly controls whether the
	 * primary plane can use the FIFO space otherwise
	 * reserved for the sprite plane. It's not 100% clear
	 * what the actual FIFO size is, but it looks like we
	 * can happily set both primary and sprite watermarks
	 * up to 127 cachelines. So that would seem to mean
	 * that either DSPCNTR[13] doesn't do anything, or that
	 * the total FIFO is >= 256 cachelines in size. Either
	 * way, we don't seem to have to worry about this
	 * repartitioning as the maximum watermark value the
	 * register can hold for each plane is lower than the
	 * minimum FIFO size.
	 */
	switch (plane_id) {
	case PLANE_CURSOR:
		return 63;
	case PLANE_PRIMARY:
		return level == G4X_WM_LEVEL_NORMAL ? 127 : 511;
	case PLANE_SPRITE0:
		return level == G4X_WM_LEVEL_NORMAL ? 127 : 0;
	default:
		MISSING_CASE(plane_id);
		return 0;
	}
}

static int g4x_fbc_fifo_size(int level)
{
	switch (level) {
	case G4X_WM_LEVEL_SR:
		return 7;
	case G4X_WM_LEVEL_HPLL:
		return 15;
	default:
		MISSING_CASE(level);
		return 0;
	}
}

<<<<<<< HEAD
static uint16_t g4x_compute_wm(const struct intel_crtc_state *crtc_state,
			       const struct intel_plane_state *plane_state,
			       int level)
{
	struct intel_plane *plane = to_intel_plane(plane_state->base.plane);
	struct drm_i915_private *dev_priv = to_i915(plane->base.dev);
	const struct drm_display_mode *adjusted_mode =
		&crtc_state->base.adjusted_mode;
=======
static u16 g4x_compute_wm(const struct intel_crtc_state *crtc_state,
			  const struct intel_plane_state *plane_state,
			  int level)
{
	struct intel_plane *plane = to_intel_plane(plane_state->uapi.plane);
	struct drm_i915_private *dev_priv = to_i915(plane->base.dev);
	const struct drm_display_mode *pipe_mode =
		&crtc_state->hw.pipe_mode;
>>>>>>> upstream/android-13
	unsigned int latency = dev_priv->wm.pri_latency[level] * 10;
	unsigned int clock, htotal, cpp, width, wm;

	if (latency == 0)
		return USHRT_MAX;

	if (!intel_wm_plane_visible(crtc_state, plane_state))
		return 0;

<<<<<<< HEAD
=======
	cpp = plane_state->hw.fb->format->cpp[0];

>>>>>>> upstream/android-13
	/*
	 * Not 100% sure which way ELK should go here as the
	 * spec only says CL/CTG should assume 32bpp and BW
	 * doesn't need to. But as these things followed the
	 * mobile vs. desktop lines on gen3 as well, let's
	 * assume ELK doesn't need this.
	 *
	 * The spec also fails to list such a restriction for
	 * the HPLL watermark, which seems a little strange.
	 * Let's use 32bpp for the HPLL watermark as well.
	 */
	if (IS_GM45(dev_priv) && plane->id == PLANE_PRIMARY &&
	    level != G4X_WM_LEVEL_NORMAL)
<<<<<<< HEAD
		cpp = 4;
	else
		cpp = plane_state->base.fb->format->cpp[0];

	clock = adjusted_mode->crtc_clock;
	htotal = adjusted_mode->crtc_htotal;

	if (plane->id == PLANE_CURSOR)
		width = plane_state->base.crtc_w;
	else
		width = drm_rect_width(&plane_state->base.dst);
=======
		cpp = max(cpp, 4u);

	clock = pipe_mode->crtc_clock;
	htotal = pipe_mode->crtc_htotal;

	width = drm_rect_width(&plane_state->uapi.dst);
>>>>>>> upstream/android-13

	if (plane->id == PLANE_CURSOR) {
		wm = intel_wm_method2(clock, htotal, width, cpp, latency);
	} else if (plane->id == PLANE_PRIMARY &&
		   level == G4X_WM_LEVEL_NORMAL) {
		wm = intel_wm_method1(clock, cpp, latency);
	} else {
		unsigned int small, large;

		small = intel_wm_method1(clock, cpp, latency);
		large = intel_wm_method2(clock, htotal, width, cpp, latency);

		wm = min(small, large);
	}

	wm += g4x_tlb_miss_wa(g4x_plane_fifo_size(plane->id, level),
			      width, cpp);

	wm = DIV_ROUND_UP(wm, 64) + 2;

	return min_t(unsigned int, wm, USHRT_MAX);
}

static bool g4x_raw_plane_wm_set(struct intel_crtc_state *crtc_state,
				 int level, enum plane_id plane_id, u16 value)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = to_i915(crtc_state->base.crtc->dev);
=======
	struct drm_i915_private *dev_priv = to_i915(crtc_state->uapi.crtc->dev);
>>>>>>> upstream/android-13
	bool dirty = false;

	for (; level < intel_wm_num_levels(dev_priv); level++) {
		struct g4x_pipe_wm *raw = &crtc_state->wm.g4x.raw[level];

		dirty |= raw->plane[plane_id] != value;
		raw->plane[plane_id] = value;
	}

	return dirty;
}

static bool g4x_raw_fbc_wm_set(struct intel_crtc_state *crtc_state,
			       int level, u16 value)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = to_i915(crtc_state->base.crtc->dev);
=======
	struct drm_i915_private *dev_priv = to_i915(crtc_state->uapi.crtc->dev);
>>>>>>> upstream/android-13
	bool dirty = false;

	/* NORMAL level doesn't have an FBC watermark */
	level = max(level, G4X_WM_LEVEL_SR);

	for (; level < intel_wm_num_levels(dev_priv); level++) {
		struct g4x_pipe_wm *raw = &crtc_state->wm.g4x.raw[level];

		dirty |= raw->fbc != value;
		raw->fbc = value;
	}

	return dirty;
}

<<<<<<< HEAD
static uint32_t ilk_compute_fbc_wm(const struct intel_crtc_state *cstate,
				   const struct intel_plane_state *pstate,
				   uint32_t pri_val);
=======
static u32 ilk_compute_fbc_wm(const struct intel_crtc_state *crtc_state,
			      const struct intel_plane_state *plane_state,
			      u32 pri_val);
>>>>>>> upstream/android-13

static bool g4x_raw_plane_wm_compute(struct intel_crtc_state *crtc_state,
				     const struct intel_plane_state *plane_state)
{
<<<<<<< HEAD
	struct intel_plane *plane = to_intel_plane(plane_state->base.plane);
=======
	struct intel_plane *plane = to_intel_plane(plane_state->uapi.plane);
	struct drm_i915_private *dev_priv = to_i915(crtc_state->uapi.crtc->dev);
>>>>>>> upstream/android-13
	int num_levels = intel_wm_num_levels(to_i915(plane->base.dev));
	enum plane_id plane_id = plane->id;
	bool dirty = false;
	int level;

	if (!intel_wm_plane_visible(crtc_state, plane_state)) {
		dirty |= g4x_raw_plane_wm_set(crtc_state, 0, plane_id, 0);
		if (plane_id == PLANE_PRIMARY)
			dirty |= g4x_raw_fbc_wm_set(crtc_state, 0, 0);
		goto out;
	}

	for (level = 0; level < num_levels; level++) {
		struct g4x_pipe_wm *raw = &crtc_state->wm.g4x.raw[level];
		int wm, max_wm;

		wm = g4x_compute_wm(crtc_state, plane_state, level);
		max_wm = g4x_plane_fifo_size(plane_id, level);

		if (wm > max_wm)
			break;

		dirty |= raw->plane[plane_id] != wm;
		raw->plane[plane_id] = wm;

		if (plane_id != PLANE_PRIMARY ||
		    level == G4X_WM_LEVEL_NORMAL)
			continue;

		wm = ilk_compute_fbc_wm(crtc_state, plane_state,
					raw->plane[plane_id]);
		max_wm = g4x_fbc_fifo_size(level);

		/*
		 * FBC wm is not mandatory as we
		 * can always just disable its use.
		 */
		if (wm > max_wm)
			wm = USHRT_MAX;

		dirty |= raw->fbc != wm;
		raw->fbc = wm;
	}

	/* mark watermarks as invalid */
	dirty |= g4x_raw_plane_wm_set(crtc_state, level, plane_id, USHRT_MAX);

	if (plane_id == PLANE_PRIMARY)
		dirty |= g4x_raw_fbc_wm_set(crtc_state, level, USHRT_MAX);

 out:
	if (dirty) {
<<<<<<< HEAD
		DRM_DEBUG_KMS("%s watermarks: normal=%d, SR=%d, HPLL=%d\n",
			      plane->base.name,
			      crtc_state->wm.g4x.raw[G4X_WM_LEVEL_NORMAL].plane[plane_id],
			      crtc_state->wm.g4x.raw[G4X_WM_LEVEL_SR].plane[plane_id],
			      crtc_state->wm.g4x.raw[G4X_WM_LEVEL_HPLL].plane[plane_id]);

		if (plane_id == PLANE_PRIMARY)
			DRM_DEBUG_KMS("FBC watermarks: SR=%d, HPLL=%d\n",
				      crtc_state->wm.g4x.raw[G4X_WM_LEVEL_SR].fbc,
				      crtc_state->wm.g4x.raw[G4X_WM_LEVEL_HPLL].fbc);
=======
		drm_dbg_kms(&dev_priv->drm,
			    "%s watermarks: normal=%d, SR=%d, HPLL=%d\n",
			    plane->base.name,
			    crtc_state->wm.g4x.raw[G4X_WM_LEVEL_NORMAL].plane[plane_id],
			    crtc_state->wm.g4x.raw[G4X_WM_LEVEL_SR].plane[plane_id],
			    crtc_state->wm.g4x.raw[G4X_WM_LEVEL_HPLL].plane[plane_id]);

		if (plane_id == PLANE_PRIMARY)
			drm_dbg_kms(&dev_priv->drm,
				    "FBC watermarks: SR=%d, HPLL=%d\n",
				    crtc_state->wm.g4x.raw[G4X_WM_LEVEL_SR].fbc,
				    crtc_state->wm.g4x.raw[G4X_WM_LEVEL_HPLL].fbc);
>>>>>>> upstream/android-13
	}

	return dirty;
}

static bool g4x_raw_plane_wm_is_valid(const struct intel_crtc_state *crtc_state,
				      enum plane_id plane_id, int level)
{
	const struct g4x_pipe_wm *raw = &crtc_state->wm.g4x.raw[level];

	return raw->plane[plane_id] <= g4x_plane_fifo_size(plane_id, level);
}

static bool g4x_raw_crtc_wm_is_valid(const struct intel_crtc_state *crtc_state,
				     int level)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = to_i915(crtc_state->base.crtc->dev);
=======
	struct drm_i915_private *dev_priv = to_i915(crtc_state->uapi.crtc->dev);
>>>>>>> upstream/android-13

	if (level > dev_priv->wm.max_level)
		return false;

	return g4x_raw_plane_wm_is_valid(crtc_state, PLANE_PRIMARY, level) &&
		g4x_raw_plane_wm_is_valid(crtc_state, PLANE_SPRITE0, level) &&
		g4x_raw_plane_wm_is_valid(crtc_state, PLANE_CURSOR, level);
}

/* mark all levels starting from 'level' as invalid */
static void g4x_invalidate_wms(struct intel_crtc *crtc,
			       struct g4x_wm_state *wm_state, int level)
{
	if (level <= G4X_WM_LEVEL_NORMAL) {
		enum plane_id plane_id;

		for_each_plane_id_on_crtc(crtc, plane_id)
			wm_state->wm.plane[plane_id] = USHRT_MAX;
	}

	if (level <= G4X_WM_LEVEL_SR) {
		wm_state->cxsr = false;
		wm_state->sr.cursor = USHRT_MAX;
		wm_state->sr.plane = USHRT_MAX;
		wm_state->sr.fbc = USHRT_MAX;
	}

	if (level <= G4X_WM_LEVEL_HPLL) {
		wm_state->hpll_en = false;
		wm_state->hpll.cursor = USHRT_MAX;
		wm_state->hpll.plane = USHRT_MAX;
		wm_state->hpll.fbc = USHRT_MAX;
	}
}

<<<<<<< HEAD
static int g4x_compute_pipe_wm(struct intel_crtc_state *crtc_state)
{
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->base.crtc);
	struct intel_atomic_state *state =
		to_intel_atomic_state(crtc_state->base.state);
	struct g4x_wm_state *wm_state = &crtc_state->wm.g4x.optimal;
	int num_active_planes = hweight32(crtc_state->active_planes &
					  ~BIT(PLANE_CURSOR));
=======
static bool g4x_compute_fbc_en(const struct g4x_wm_state *wm_state,
			       int level)
{
	if (level < G4X_WM_LEVEL_SR)
		return false;

	if (level >= G4X_WM_LEVEL_SR &&
	    wm_state->sr.fbc > g4x_fbc_fifo_size(G4X_WM_LEVEL_SR))
		return false;

	if (level >= G4X_WM_LEVEL_HPLL &&
	    wm_state->hpll.fbc > g4x_fbc_fifo_size(G4X_WM_LEVEL_HPLL))
		return false;

	return true;
}

static int g4x_compute_pipe_wm(struct intel_atomic_state *state,
			       struct intel_crtc *crtc)
{
	struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
	struct g4x_wm_state *wm_state = &crtc_state->wm.g4x.optimal;
	int num_active_planes = hweight8(crtc_state->active_planes &
					 ~BIT(PLANE_CURSOR));
>>>>>>> upstream/android-13
	const struct g4x_pipe_wm *raw;
	const struct intel_plane_state *old_plane_state;
	const struct intel_plane_state *new_plane_state;
	struct intel_plane *plane;
	enum plane_id plane_id;
	int i, level;
	unsigned int dirty = 0;

	for_each_oldnew_intel_plane_in_state(state, plane,
					     old_plane_state,
					     new_plane_state, i) {
<<<<<<< HEAD
		if (new_plane_state->base.crtc != &crtc->base &&
		    old_plane_state->base.crtc != &crtc->base)
=======
		if (new_plane_state->hw.crtc != &crtc->base &&
		    old_plane_state->hw.crtc != &crtc->base)
>>>>>>> upstream/android-13
			continue;

		if (g4x_raw_plane_wm_compute(crtc_state, new_plane_state))
			dirty |= BIT(plane->id);
	}

	if (!dirty)
		return 0;

	level = G4X_WM_LEVEL_NORMAL;
	if (!g4x_raw_crtc_wm_is_valid(crtc_state, level))
		goto out;

	raw = &crtc_state->wm.g4x.raw[level];
	for_each_plane_id_on_crtc(crtc, plane_id)
		wm_state->wm.plane[plane_id] = raw->plane[plane_id];

	level = G4X_WM_LEVEL_SR;
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
	if (!g4x_raw_crtc_wm_is_valid(crtc_state, level))
		goto out;

	raw = &crtc_state->wm.g4x.raw[level];
	wm_state->sr.plane = raw->plane[PLANE_PRIMARY];
	wm_state->sr.cursor = raw->plane[PLANE_CURSOR];
	wm_state->sr.fbc = raw->fbc;

	wm_state->cxsr = num_active_planes == BIT(PLANE_PRIMARY);

	level = G4X_WM_LEVEL_HPLL;
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
	if (!g4x_raw_crtc_wm_is_valid(crtc_state, level))
		goto out;

	raw = &crtc_state->wm.g4x.raw[level];
	wm_state->hpll.plane = raw->plane[PLANE_PRIMARY];
	wm_state->hpll.cursor = raw->plane[PLANE_CURSOR];
	wm_state->hpll.fbc = raw->fbc;

	wm_state->hpll_en = wm_state->cxsr;

	level++;

 out:
	if (level == G4X_WM_LEVEL_NORMAL)
		return -EINVAL;

	/* invalidate the higher levels */
	g4x_invalidate_wms(crtc, wm_state, level);

	/*
	 * Determine if the FBC watermark(s) can be used. IF
	 * this isn't the case we prefer to disable the FBC
<<<<<<< HEAD
	 ( watermark(s) rather than disable the SR/HPLL
	 * level(s) entirely.
	 */
	wm_state->fbc_en = level > G4X_WM_LEVEL_NORMAL;

	if (level >= G4X_WM_LEVEL_SR &&
	    wm_state->sr.fbc > g4x_fbc_fifo_size(G4X_WM_LEVEL_SR))
		wm_state->fbc_en = false;
	else if (level >= G4X_WM_LEVEL_HPLL &&
		 wm_state->hpll.fbc > g4x_fbc_fifo_size(G4X_WM_LEVEL_HPLL))
		wm_state->fbc_en = false;
=======
	 * watermark(s) rather than disable the SR/HPLL
	 * level(s) entirely. 'level-1' is the highest valid
	 * level here.
	 */
	wm_state->fbc_en = g4x_compute_fbc_en(wm_state, level - 1);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int g4x_compute_intermediate_wm(struct drm_device *dev,
				       struct intel_crtc *crtc,
				       struct intel_crtc_state *new_crtc_state)
{
	struct g4x_wm_state *intermediate = &new_crtc_state->wm.g4x.intermediate;
	const struct g4x_wm_state *optimal = &new_crtc_state->wm.g4x.optimal;
	struct intel_atomic_state *intel_state =
		to_intel_atomic_state(new_crtc_state->base.state);
	const struct intel_crtc_state *old_crtc_state =
		intel_atomic_get_old_crtc_state(intel_state, crtc);
	const struct g4x_wm_state *active = &old_crtc_state->wm.g4x.optimal;
	enum plane_id plane_id;

	if (!new_crtc_state->base.active || drm_atomic_crtc_needs_modeset(&new_crtc_state->base)) {
=======
static int g4x_compute_intermediate_wm(struct intel_atomic_state *state,
				       struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	struct intel_crtc_state *new_crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
	const struct intel_crtc_state *old_crtc_state =
		intel_atomic_get_old_crtc_state(state, crtc);
	struct g4x_wm_state *intermediate = &new_crtc_state->wm.g4x.intermediate;
	const struct g4x_wm_state *optimal = &new_crtc_state->wm.g4x.optimal;
	const struct g4x_wm_state *active = &old_crtc_state->wm.g4x.optimal;
	enum plane_id plane_id;

	if (!new_crtc_state->hw.active ||
	    drm_atomic_crtc_needs_modeset(&new_crtc_state->uapi)) {
>>>>>>> upstream/android-13
		*intermediate = *optimal;

		intermediate->cxsr = false;
		intermediate->hpll_en = false;
		goto out;
	}

	intermediate->cxsr = optimal->cxsr && active->cxsr &&
		!new_crtc_state->disable_cxsr;
	intermediate->hpll_en = optimal->hpll_en && active->hpll_en &&
		!new_crtc_state->disable_cxsr;
	intermediate->fbc_en = optimal->fbc_en && active->fbc_en;

	for_each_plane_id_on_crtc(crtc, plane_id) {
		intermediate->wm.plane[plane_id] =
			max(optimal->wm.plane[plane_id],
			    active->wm.plane[plane_id]);

<<<<<<< HEAD
		WARN_ON(intermediate->wm.plane[plane_id] >
			g4x_plane_fifo_size(plane_id, G4X_WM_LEVEL_NORMAL));
=======
		drm_WARN_ON(&dev_priv->drm, intermediate->wm.plane[plane_id] >
			    g4x_plane_fifo_size(plane_id, G4X_WM_LEVEL_NORMAL));
>>>>>>> upstream/android-13
	}

	intermediate->sr.plane = max(optimal->sr.plane,
				     active->sr.plane);
	intermediate->sr.cursor = max(optimal->sr.cursor,
				      active->sr.cursor);
	intermediate->sr.fbc = max(optimal->sr.fbc,
				   active->sr.fbc);

	intermediate->hpll.plane = max(optimal->hpll.plane,
				       active->hpll.plane);
	intermediate->hpll.cursor = max(optimal->hpll.cursor,
					active->hpll.cursor);
	intermediate->hpll.fbc = max(optimal->hpll.fbc,
				     active->hpll.fbc);

<<<<<<< HEAD
	WARN_ON((intermediate->sr.plane >
		 g4x_plane_fifo_size(PLANE_PRIMARY, G4X_WM_LEVEL_SR) ||
		 intermediate->sr.cursor >
		 g4x_plane_fifo_size(PLANE_CURSOR, G4X_WM_LEVEL_SR)) &&
		intermediate->cxsr);
	WARN_ON((intermediate->sr.plane >
		 g4x_plane_fifo_size(PLANE_PRIMARY, G4X_WM_LEVEL_HPLL) ||
		 intermediate->sr.cursor >
		 g4x_plane_fifo_size(PLANE_CURSOR, G4X_WM_LEVEL_HPLL)) &&
		intermediate->hpll_en);

	WARN_ON(intermediate->sr.fbc > g4x_fbc_fifo_size(1) &&
		intermediate->fbc_en && intermediate->cxsr);
	WARN_ON(intermediate->hpll.fbc > g4x_fbc_fifo_size(2) &&
		intermediate->fbc_en && intermediate->hpll_en);
=======
	drm_WARN_ON(&dev_priv->drm,
		    (intermediate->sr.plane >
		     g4x_plane_fifo_size(PLANE_PRIMARY, G4X_WM_LEVEL_SR) ||
		     intermediate->sr.cursor >
		     g4x_plane_fifo_size(PLANE_CURSOR, G4X_WM_LEVEL_SR)) &&
		    intermediate->cxsr);
	drm_WARN_ON(&dev_priv->drm,
		    (intermediate->sr.plane >
		     g4x_plane_fifo_size(PLANE_PRIMARY, G4X_WM_LEVEL_HPLL) ||
		     intermediate->sr.cursor >
		     g4x_plane_fifo_size(PLANE_CURSOR, G4X_WM_LEVEL_HPLL)) &&
		    intermediate->hpll_en);

	drm_WARN_ON(&dev_priv->drm,
		    intermediate->sr.fbc > g4x_fbc_fifo_size(1) &&
		    intermediate->fbc_en && intermediate->cxsr);
	drm_WARN_ON(&dev_priv->drm,
		    intermediate->hpll.fbc > g4x_fbc_fifo_size(2) &&
		    intermediate->fbc_en && intermediate->hpll_en);
>>>>>>> upstream/android-13

out:
	/*
	 * If our intermediate WM are identical to the final WM, then we can
	 * omit the post-vblank programming; only update if it's different.
	 */
	if (memcmp(intermediate, optimal, sizeof(*intermediate)) != 0)
		new_crtc_state->wm.need_postvbl_update = true;

	return 0;
}

static void g4x_merge_wm(struct drm_i915_private *dev_priv,
			 struct g4x_wm_values *wm)
{
	struct intel_crtc *crtc;
<<<<<<< HEAD
	int num_active_crtcs = 0;
=======
	int num_active_pipes = 0;
>>>>>>> upstream/android-13

	wm->cxsr = true;
	wm->hpll_en = true;
	wm->fbc_en = true;

	for_each_intel_crtc(&dev_priv->drm, crtc) {
		const struct g4x_wm_state *wm_state = &crtc->wm.active.g4x;

		if (!crtc->active)
			continue;

		if (!wm_state->cxsr)
			wm->cxsr = false;
		if (!wm_state->hpll_en)
			wm->hpll_en = false;
		if (!wm_state->fbc_en)
			wm->fbc_en = false;

<<<<<<< HEAD
		num_active_crtcs++;
	}

	if (num_active_crtcs != 1) {
=======
		num_active_pipes++;
	}

	if (num_active_pipes != 1) {
>>>>>>> upstream/android-13
		wm->cxsr = false;
		wm->hpll_en = false;
		wm->fbc_en = false;
	}

	for_each_intel_crtc(&dev_priv->drm, crtc) {
		const struct g4x_wm_state *wm_state = &crtc->wm.active.g4x;
		enum pipe pipe = crtc->pipe;

		wm->pipe[pipe] = wm_state->wm;
		if (crtc->active && wm->cxsr)
			wm->sr = wm_state->sr;
		if (crtc->active && wm->hpll_en)
			wm->hpll = wm_state->hpll;
	}
}

static void g4x_program_watermarks(struct drm_i915_private *dev_priv)
{
	struct g4x_wm_values *old_wm = &dev_priv->wm.g4x;
	struct g4x_wm_values new_wm = {};

	g4x_merge_wm(dev_priv, &new_wm);

	if (memcmp(old_wm, &new_wm, sizeof(new_wm)) == 0)
		return;

	if (is_disabling(old_wm->cxsr, new_wm.cxsr, true))
		_intel_set_memory_cxsr(dev_priv, false);

	g4x_write_wm_values(dev_priv, &new_wm);

	if (is_enabling(old_wm->cxsr, new_wm.cxsr, true))
		_intel_set_memory_cxsr(dev_priv, true);

	*old_wm = new_wm;
}

static void g4x_initial_watermarks(struct intel_atomic_state *state,
<<<<<<< HEAD
				   struct intel_crtc_state *crtc_state)
{
	struct drm_i915_private *dev_priv = to_i915(crtc_state->base.crtc->dev);
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->base.crtc);
=======
				   struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	const struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
>>>>>>> upstream/android-13

	mutex_lock(&dev_priv->wm.wm_mutex);
	crtc->wm.active.g4x = crtc_state->wm.g4x.intermediate;
	g4x_program_watermarks(dev_priv);
	mutex_unlock(&dev_priv->wm.wm_mutex);
}

static void g4x_optimize_watermarks(struct intel_atomic_state *state,
<<<<<<< HEAD
				    struct intel_crtc_state *crtc_state)
{
	struct drm_i915_private *dev_priv = to_i915(crtc_state->base.crtc->dev);
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc_state->base.crtc);
=======
				    struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	const struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
>>>>>>> upstream/android-13

	if (!crtc_state->wm.need_postvbl_update)
		return;

	mutex_lock(&dev_priv->wm.wm_mutex);
<<<<<<< HEAD
	intel_crtc->wm.active.g4x = crtc_state->wm.g4x.optimal;
=======
	crtc->wm.active.g4x = crtc_state->wm.g4x.optimal;
>>>>>>> upstream/android-13
	g4x_program_watermarks(dev_priv);
	mutex_unlock(&dev_priv->wm.wm_mutex);
}

/* latency must be in 0.1us units. */
static unsigned int vlv_wm_method2(unsigned int pixel_rate,
				   unsigned int htotal,
				   unsigned int width,
				   unsigned int cpp,
				   unsigned int latency)
{
	unsigned int ret;

	ret = intel_wm_method2(pixel_rate, htotal,
			       width, cpp, latency);
	ret = DIV_ROUND_UP(ret, 64);

	return ret;
}

static void vlv_setup_wm_latency(struct drm_i915_private *dev_priv)
{
	/* all latencies in usec */
	dev_priv->wm.pri_latency[VLV_WM_LEVEL_PM2] = 3;

	dev_priv->wm.max_level = VLV_WM_LEVEL_PM2;

	if (IS_CHERRYVIEW(dev_priv)) {
		dev_priv->wm.pri_latency[VLV_WM_LEVEL_PM5] = 12;
		dev_priv->wm.pri_latency[VLV_WM_LEVEL_DDR_DVFS] = 33;

		dev_priv->wm.max_level = VLV_WM_LEVEL_DDR_DVFS;
	}
}

<<<<<<< HEAD
static uint16_t vlv_compute_wm_level(const struct intel_crtc_state *crtc_state,
				     const struct intel_plane_state *plane_state,
				     int level)
{
	struct intel_plane *plane = to_intel_plane(plane_state->base.plane);
	struct drm_i915_private *dev_priv = to_i915(plane->base.dev);
	const struct drm_display_mode *adjusted_mode =
		&crtc_state->base.adjusted_mode;
=======
static u16 vlv_compute_wm_level(const struct intel_crtc_state *crtc_state,
				const struct intel_plane_state *plane_state,
				int level)
{
	struct intel_plane *plane = to_intel_plane(plane_state->uapi.plane);
	struct drm_i915_private *dev_priv = to_i915(plane->base.dev);
	const struct drm_display_mode *pipe_mode =
		&crtc_state->hw.pipe_mode;
>>>>>>> upstream/android-13
	unsigned int clock, htotal, cpp, width, wm;

	if (dev_priv->wm.pri_latency[level] == 0)
		return USHRT_MAX;

	if (!intel_wm_plane_visible(crtc_state, plane_state))
		return 0;

<<<<<<< HEAD
	cpp = plane_state->base.fb->format->cpp[0];
	clock = adjusted_mode->crtc_clock;
	htotal = adjusted_mode->crtc_htotal;
=======
	cpp = plane_state->hw.fb->format->cpp[0];
	clock = pipe_mode->crtc_clock;
	htotal = pipe_mode->crtc_htotal;
>>>>>>> upstream/android-13
	width = crtc_state->pipe_src_w;

	if (plane->id == PLANE_CURSOR) {
		/*
		 * FIXME the formula gives values that are
		 * too big for the cursor FIFO, and hence we
		 * would never be able to use cursors. For
		 * now just hardcode the watermark.
		 */
		wm = 63;
	} else {
		wm = vlv_wm_method2(clock, htotal, width, cpp,
				    dev_priv->wm.pri_latency[level] * 10);
	}

	return min_t(unsigned int, wm, USHRT_MAX);
}

static bool vlv_need_sprite0_fifo_workaround(unsigned int active_planes)
{
	return (active_planes & (BIT(PLANE_SPRITE0) |
				 BIT(PLANE_SPRITE1))) == BIT(PLANE_SPRITE1);
}

static int vlv_compute_fifo(struct intel_crtc_state *crtc_state)
{
<<<<<<< HEAD
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->base.crtc);
=======
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->uapi.crtc);
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
>>>>>>> upstream/android-13
	const struct g4x_pipe_wm *raw =
		&crtc_state->wm.vlv.raw[VLV_WM_LEVEL_PM2];
	struct vlv_fifo_state *fifo_state = &crtc_state->wm.vlv.fifo_state;
	unsigned int active_planes = crtc_state->active_planes & ~BIT(PLANE_CURSOR);
<<<<<<< HEAD
	int num_active_planes = hweight32(active_planes);
=======
	int num_active_planes = hweight8(active_planes);
>>>>>>> upstream/android-13
	const int fifo_size = 511;
	int fifo_extra, fifo_left = fifo_size;
	int sprite0_fifo_extra = 0;
	unsigned int total_rate;
	enum plane_id plane_id;

	/*
	 * When enabling sprite0 after sprite1 has already been enabled
	 * we tend to get an underrun unless sprite0 already has some
	 * FIFO space allcoated. Hence we always allocate at least one
	 * cacheline for sprite0 whenever sprite1 is enabled.
	 *
	 * All other plane enable sequences appear immune to this problem.
	 */
	if (vlv_need_sprite0_fifo_workaround(active_planes))
		sprite0_fifo_extra = 1;

	total_rate = raw->plane[PLANE_PRIMARY] +
		raw->plane[PLANE_SPRITE0] +
		raw->plane[PLANE_SPRITE1] +
		sprite0_fifo_extra;

	if (total_rate > fifo_size)
		return -EINVAL;

	if (total_rate == 0)
		total_rate = 1;

	for_each_plane_id_on_crtc(crtc, plane_id) {
		unsigned int rate;

		if ((active_planes & BIT(plane_id)) == 0) {
			fifo_state->plane[plane_id] = 0;
			continue;
		}

		rate = raw->plane[plane_id];
		fifo_state->plane[plane_id] = fifo_size * rate / total_rate;
		fifo_left -= fifo_state->plane[plane_id];
	}

	fifo_state->plane[PLANE_SPRITE0] += sprite0_fifo_extra;
	fifo_left -= sprite0_fifo_extra;

	fifo_state->plane[PLANE_CURSOR] = 63;

	fifo_extra = DIV_ROUND_UP(fifo_left, num_active_planes ?: 1);

	/* spread the remainder evenly */
	for_each_plane_id_on_crtc(crtc, plane_id) {
		int plane_extra;

		if (fifo_left == 0)
			break;

		if ((active_planes & BIT(plane_id)) == 0)
			continue;

		plane_extra = min(fifo_extra, fifo_left);
		fifo_state->plane[plane_id] += plane_extra;
		fifo_left -= plane_extra;
	}

<<<<<<< HEAD
	WARN_ON(active_planes != 0 && fifo_left != 0);

	/* give it all to the first plane if none are active */
	if (active_planes == 0) {
		WARN_ON(fifo_left != fifo_size);
=======
	drm_WARN_ON(&dev_priv->drm, active_planes != 0 && fifo_left != 0);

	/* give it all to the first plane if none are active */
	if (active_planes == 0) {
		drm_WARN_ON(&dev_priv->drm, fifo_left != fifo_size);
>>>>>>> upstream/android-13
		fifo_state->plane[PLANE_PRIMARY] = fifo_left;
	}

	return 0;
}

/* mark all levels starting from 'level' as invalid */
static void vlv_invalidate_wms(struct intel_crtc *crtc,
			       struct vlv_wm_state *wm_state, int level)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);

	for (; level < intel_wm_num_levels(dev_priv); level++) {
		enum plane_id plane_id;

		for_each_plane_id_on_crtc(crtc, plane_id)
			wm_state->wm[level].plane[plane_id] = USHRT_MAX;

		wm_state->sr[level].cursor = USHRT_MAX;
		wm_state->sr[level].plane = USHRT_MAX;
	}
}

static u16 vlv_invert_wm_value(u16 wm, u16 fifo_size)
{
	if (wm > fifo_size)
		return USHRT_MAX;
	else
		return fifo_size - wm;
}

/*
 * Starting from 'level' set all higher
 * levels to 'value' in the "raw" watermarks.
 */
static bool vlv_raw_plane_wm_set(struct intel_crtc_state *crtc_state,
				 int level, enum plane_id plane_id, u16 value)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = to_i915(crtc_state->base.crtc->dev);
=======
	struct drm_i915_private *dev_priv = to_i915(crtc_state->uapi.crtc->dev);
>>>>>>> upstream/android-13
	int num_levels = intel_wm_num_levels(dev_priv);
	bool dirty = false;

	for (; level < num_levels; level++) {
		struct g4x_pipe_wm *raw = &crtc_state->wm.vlv.raw[level];

		dirty |= raw->plane[plane_id] != value;
		raw->plane[plane_id] = value;
	}

	return dirty;
}

static bool vlv_raw_plane_wm_compute(struct intel_crtc_state *crtc_state,
				     const struct intel_plane_state *plane_state)
{
<<<<<<< HEAD
	struct intel_plane *plane = to_intel_plane(plane_state->base.plane);
=======
	struct intel_plane *plane = to_intel_plane(plane_state->uapi.plane);
	struct drm_i915_private *dev_priv = to_i915(crtc_state->uapi.crtc->dev);
>>>>>>> upstream/android-13
	enum plane_id plane_id = plane->id;
	int num_levels = intel_wm_num_levels(to_i915(plane->base.dev));
	int level;
	bool dirty = false;

	if (!intel_wm_plane_visible(crtc_state, plane_state)) {
		dirty |= vlv_raw_plane_wm_set(crtc_state, 0, plane_id, 0);
		goto out;
	}

	for (level = 0; level < num_levels; level++) {
		struct g4x_pipe_wm *raw = &crtc_state->wm.vlv.raw[level];
		int wm = vlv_compute_wm_level(crtc_state, plane_state, level);
		int max_wm = plane_id == PLANE_CURSOR ? 63 : 511;

		if (wm > max_wm)
			break;

		dirty |= raw->plane[plane_id] != wm;
		raw->plane[plane_id] = wm;
	}

	/* mark all higher levels as invalid */
	dirty |= vlv_raw_plane_wm_set(crtc_state, level, plane_id, USHRT_MAX);

out:
	if (dirty)
<<<<<<< HEAD
		DRM_DEBUG_KMS("%s watermarks: PM2=%d, PM5=%d, DDR DVFS=%d\n",
			      plane->base.name,
			      crtc_state->wm.vlv.raw[VLV_WM_LEVEL_PM2].plane[plane_id],
			      crtc_state->wm.vlv.raw[VLV_WM_LEVEL_PM5].plane[plane_id],
			      crtc_state->wm.vlv.raw[VLV_WM_LEVEL_DDR_DVFS].plane[plane_id]);
=======
		drm_dbg_kms(&dev_priv->drm,
			    "%s watermarks: PM2=%d, PM5=%d, DDR DVFS=%d\n",
			    plane->base.name,
			    crtc_state->wm.vlv.raw[VLV_WM_LEVEL_PM2].plane[plane_id],
			    crtc_state->wm.vlv.raw[VLV_WM_LEVEL_PM5].plane[plane_id],
			    crtc_state->wm.vlv.raw[VLV_WM_LEVEL_DDR_DVFS].plane[plane_id]);
>>>>>>> upstream/android-13

	return dirty;
}

static bool vlv_raw_plane_wm_is_valid(const struct intel_crtc_state *crtc_state,
				      enum plane_id plane_id, int level)
{
	const struct g4x_pipe_wm *raw =
		&crtc_state->wm.vlv.raw[level];
	const struct vlv_fifo_state *fifo_state =
		&crtc_state->wm.vlv.fifo_state;

	return raw->plane[plane_id] <= fifo_state->plane[plane_id];
}

static bool vlv_raw_crtc_wm_is_valid(const struct intel_crtc_state *crtc_state, int level)
{
	return vlv_raw_plane_wm_is_valid(crtc_state, PLANE_PRIMARY, level) &&
		vlv_raw_plane_wm_is_valid(crtc_state, PLANE_SPRITE0, level) &&
		vlv_raw_plane_wm_is_valid(crtc_state, PLANE_SPRITE1, level) &&
		vlv_raw_plane_wm_is_valid(crtc_state, PLANE_CURSOR, level);
}

<<<<<<< HEAD
static int vlv_compute_pipe_wm(struct intel_crtc_state *crtc_state)
{
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->base.crtc);
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	struct intel_atomic_state *state =
		to_intel_atomic_state(crtc_state->base.state);
	struct vlv_wm_state *wm_state = &crtc_state->wm.vlv.optimal;
	const struct vlv_fifo_state *fifo_state =
		&crtc_state->wm.vlv.fifo_state;
	int num_active_planes = hweight32(crtc_state->active_planes &
					  ~BIT(PLANE_CURSOR));
	bool needs_modeset = drm_atomic_crtc_needs_modeset(&crtc_state->base);
=======
static int vlv_compute_pipe_wm(struct intel_atomic_state *state,
			       struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
	struct vlv_wm_state *wm_state = &crtc_state->wm.vlv.optimal;
	const struct vlv_fifo_state *fifo_state =
		&crtc_state->wm.vlv.fifo_state;
	int num_active_planes = hweight8(crtc_state->active_planes &
					 ~BIT(PLANE_CURSOR));
	bool needs_modeset = drm_atomic_crtc_needs_modeset(&crtc_state->uapi);
>>>>>>> upstream/android-13
	const struct intel_plane_state *old_plane_state;
	const struct intel_plane_state *new_plane_state;
	struct intel_plane *plane;
	enum plane_id plane_id;
	int level, ret, i;
	unsigned int dirty = 0;

	for_each_oldnew_intel_plane_in_state(state, plane,
					     old_plane_state,
					     new_plane_state, i) {
<<<<<<< HEAD
		if (new_plane_state->base.crtc != &crtc->base &&
		    old_plane_state->base.crtc != &crtc->base)
=======
		if (new_plane_state->hw.crtc != &crtc->base &&
		    old_plane_state->hw.crtc != &crtc->base)
>>>>>>> upstream/android-13
			continue;

		if (vlv_raw_plane_wm_compute(crtc_state, new_plane_state))
			dirty |= BIT(plane->id);
	}

	/*
	 * DSPARB registers may have been reset due to the
	 * power well being turned off. Make sure we restore
	 * them to a consistent state even if no primary/sprite
	 * planes are initially active.
	 */
	if (needs_modeset)
		crtc_state->fifo_changed = true;

	if (!dirty)
		return 0;

	/* cursor changes don't warrant a FIFO recompute */
	if (dirty & ~BIT(PLANE_CURSOR)) {
		const struct intel_crtc_state *old_crtc_state =
			intel_atomic_get_old_crtc_state(state, crtc);
		const struct vlv_fifo_state *old_fifo_state =
			&old_crtc_state->wm.vlv.fifo_state;

		ret = vlv_compute_fifo(crtc_state);
		if (ret)
			return ret;

		if (needs_modeset ||
		    memcmp(old_fifo_state, fifo_state,
			   sizeof(*fifo_state)) != 0)
			crtc_state->fifo_changed = true;
	}

	/* initially allow all levels */
	wm_state->num_levels = intel_wm_num_levels(dev_priv);
	/*
	 * Note that enabling cxsr with no primary/sprite planes
	 * enabled can wedge the pipe. Hence we only allow cxsr
	 * with exactly one enabled primary/sprite plane.
	 */
	wm_state->cxsr = crtc->pipe != PIPE_C && num_active_planes == 1;

	for (level = 0; level < wm_state->num_levels; level++) {
		const struct g4x_pipe_wm *raw = &crtc_state->wm.vlv.raw[level];
<<<<<<< HEAD
		const int sr_fifo_size = INTEL_INFO(dev_priv)->num_pipes * 512 - 1;
=======
		const int sr_fifo_size = INTEL_NUM_PIPES(dev_priv) * 512 - 1;
>>>>>>> upstream/android-13

		if (!vlv_raw_crtc_wm_is_valid(crtc_state, level))
			break;

		for_each_plane_id_on_crtc(crtc, plane_id) {
			wm_state->wm[level].plane[plane_id] =
				vlv_invert_wm_value(raw->plane[plane_id],
						    fifo_state->plane[plane_id]);
		}

		wm_state->sr[level].plane =
			vlv_invert_wm_value(max3(raw->plane[PLANE_PRIMARY],
						 raw->plane[PLANE_SPRITE0],
						 raw->plane[PLANE_SPRITE1]),
					    sr_fifo_size);

		wm_state->sr[level].cursor =
			vlv_invert_wm_value(raw->plane[PLANE_CURSOR],
					    63);
	}

	if (level == 0)
		return -EINVAL;

	/* limit to only levels we can actually handle */
	wm_state->num_levels = level;

	/* invalidate the higher levels */
	vlv_invalidate_wms(crtc, wm_state, level);

	return 0;
}

#define VLV_FIFO(plane, value) \
	(((value) << DSPARB_ ## plane ## _SHIFT_VLV) & DSPARB_ ## plane ## _MASK_VLV)

static void vlv_atomic_update_fifo(struct intel_atomic_state *state,
<<<<<<< HEAD
				   struct intel_crtc_state *crtc_state)
{
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->base.crtc);
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	const struct vlv_fifo_state *fifo_state =
		&crtc_state->wm.vlv.fifo_state;
	int sprite0_start, sprite1_start, fifo_size;
=======
				   struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	struct intel_uncore *uncore = &dev_priv->uncore;
	const struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
	const struct vlv_fifo_state *fifo_state =
		&crtc_state->wm.vlv.fifo_state;
	int sprite0_start, sprite1_start, fifo_size;
	u32 dsparb, dsparb2, dsparb3;
>>>>>>> upstream/android-13

	if (!crtc_state->fifo_changed)
		return;

	sprite0_start = fifo_state->plane[PLANE_PRIMARY];
	sprite1_start = fifo_state->plane[PLANE_SPRITE0] + sprite0_start;
	fifo_size = fifo_state->plane[PLANE_SPRITE1] + sprite1_start;

<<<<<<< HEAD
	WARN_ON(fifo_state->plane[PLANE_CURSOR] != 63);
	WARN_ON(fifo_size != 511);
=======
	drm_WARN_ON(&dev_priv->drm, fifo_state->plane[PLANE_CURSOR] != 63);
	drm_WARN_ON(&dev_priv->drm, fifo_size != 511);
>>>>>>> upstream/android-13

	trace_vlv_fifo_size(crtc, sprite0_start, sprite1_start, fifo_size);

	/*
	 * uncore.lock serves a double purpose here. It allows us to
	 * use the less expensive I915_{READ,WRITE}_FW() functions, and
	 * it protects the DSPARB registers from getting clobbered by
	 * parallel updates from multiple pipes.
	 *
	 * intel_pipe_update_start() has already disabled interrupts
	 * for us, so a plain spin_lock() is sufficient here.
	 */
<<<<<<< HEAD
	spin_lock(&dev_priv->uncore.lock);

	switch (crtc->pipe) {
		uint32_t dsparb, dsparb2, dsparb3;
	case PIPE_A:
		dsparb = I915_READ_FW(DSPARB);
		dsparb2 = I915_READ_FW(DSPARB2);
=======
	spin_lock(&uncore->lock);

	switch (crtc->pipe) {
	case PIPE_A:
		dsparb = intel_uncore_read_fw(uncore, DSPARB);
		dsparb2 = intel_uncore_read_fw(uncore, DSPARB2);
>>>>>>> upstream/android-13

		dsparb &= ~(VLV_FIFO(SPRITEA, 0xff) |
			    VLV_FIFO(SPRITEB, 0xff));
		dsparb |= (VLV_FIFO(SPRITEA, sprite0_start) |
			   VLV_FIFO(SPRITEB, sprite1_start));

		dsparb2 &= ~(VLV_FIFO(SPRITEA_HI, 0x1) |
			     VLV_FIFO(SPRITEB_HI, 0x1));
		dsparb2 |= (VLV_FIFO(SPRITEA_HI, sprite0_start >> 8) |
			   VLV_FIFO(SPRITEB_HI, sprite1_start >> 8));

<<<<<<< HEAD
		I915_WRITE_FW(DSPARB, dsparb);
		I915_WRITE_FW(DSPARB2, dsparb2);
		break;
	case PIPE_B:
		dsparb = I915_READ_FW(DSPARB);
		dsparb2 = I915_READ_FW(DSPARB2);
=======
		intel_uncore_write_fw(uncore, DSPARB, dsparb);
		intel_uncore_write_fw(uncore, DSPARB2, dsparb2);
		break;
	case PIPE_B:
		dsparb = intel_uncore_read_fw(uncore, DSPARB);
		dsparb2 = intel_uncore_read_fw(uncore, DSPARB2);
>>>>>>> upstream/android-13

		dsparb &= ~(VLV_FIFO(SPRITEC, 0xff) |
			    VLV_FIFO(SPRITED, 0xff));
		dsparb |= (VLV_FIFO(SPRITEC, sprite0_start) |
			   VLV_FIFO(SPRITED, sprite1_start));

		dsparb2 &= ~(VLV_FIFO(SPRITEC_HI, 0xff) |
			     VLV_FIFO(SPRITED_HI, 0xff));
		dsparb2 |= (VLV_FIFO(SPRITEC_HI, sprite0_start >> 8) |
			   VLV_FIFO(SPRITED_HI, sprite1_start >> 8));

<<<<<<< HEAD
		I915_WRITE_FW(DSPARB, dsparb);
		I915_WRITE_FW(DSPARB2, dsparb2);
		break;
	case PIPE_C:
		dsparb3 = I915_READ_FW(DSPARB3);
		dsparb2 = I915_READ_FW(DSPARB2);
=======
		intel_uncore_write_fw(uncore, DSPARB, dsparb);
		intel_uncore_write_fw(uncore, DSPARB2, dsparb2);
		break;
	case PIPE_C:
		dsparb3 = intel_uncore_read_fw(uncore, DSPARB3);
		dsparb2 = intel_uncore_read_fw(uncore, DSPARB2);
>>>>>>> upstream/android-13

		dsparb3 &= ~(VLV_FIFO(SPRITEE, 0xff) |
			     VLV_FIFO(SPRITEF, 0xff));
		dsparb3 |= (VLV_FIFO(SPRITEE, sprite0_start) |
			    VLV_FIFO(SPRITEF, sprite1_start));

		dsparb2 &= ~(VLV_FIFO(SPRITEE_HI, 0xff) |
			     VLV_FIFO(SPRITEF_HI, 0xff));
		dsparb2 |= (VLV_FIFO(SPRITEE_HI, sprite0_start >> 8) |
			   VLV_FIFO(SPRITEF_HI, sprite1_start >> 8));

<<<<<<< HEAD
		I915_WRITE_FW(DSPARB3, dsparb3);
		I915_WRITE_FW(DSPARB2, dsparb2);
=======
		intel_uncore_write_fw(uncore, DSPARB3, dsparb3);
		intel_uncore_write_fw(uncore, DSPARB2, dsparb2);
>>>>>>> upstream/android-13
		break;
	default:
		break;
	}

<<<<<<< HEAD
	POSTING_READ_FW(DSPARB);

	spin_unlock(&dev_priv->uncore.lock);
=======
	intel_uncore_posting_read_fw(uncore, DSPARB);

	spin_unlock(&uncore->lock);
>>>>>>> upstream/android-13
}

#undef VLV_FIFO

<<<<<<< HEAD
static int vlv_compute_intermediate_wm(struct drm_device *dev,
				       struct intel_crtc *crtc,
				       struct intel_crtc_state *new_crtc_state)
{
	struct vlv_wm_state *intermediate = &new_crtc_state->wm.vlv.intermediate;
	const struct vlv_wm_state *optimal = &new_crtc_state->wm.vlv.optimal;
	struct intel_atomic_state *intel_state =
		to_intel_atomic_state(new_crtc_state->base.state);
	const struct intel_crtc_state *old_crtc_state =
		intel_atomic_get_old_crtc_state(intel_state, crtc);
	const struct vlv_wm_state *active = &old_crtc_state->wm.vlv.optimal;
	int level;

	if (!new_crtc_state->base.active || drm_atomic_crtc_needs_modeset(&new_crtc_state->base)) {
=======
static int vlv_compute_intermediate_wm(struct intel_atomic_state *state,
				       struct intel_crtc *crtc)
{
	struct intel_crtc_state *new_crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
	const struct intel_crtc_state *old_crtc_state =
		intel_atomic_get_old_crtc_state(state, crtc);
	struct vlv_wm_state *intermediate = &new_crtc_state->wm.vlv.intermediate;
	const struct vlv_wm_state *optimal = &new_crtc_state->wm.vlv.optimal;
	const struct vlv_wm_state *active = &old_crtc_state->wm.vlv.optimal;
	int level;

	if (!new_crtc_state->hw.active ||
	    drm_atomic_crtc_needs_modeset(&new_crtc_state->uapi)) {
>>>>>>> upstream/android-13
		*intermediate = *optimal;

		intermediate->cxsr = false;
		goto out;
	}

	intermediate->num_levels = min(optimal->num_levels, active->num_levels);
	intermediate->cxsr = optimal->cxsr && active->cxsr &&
		!new_crtc_state->disable_cxsr;

	for (level = 0; level < intermediate->num_levels; level++) {
		enum plane_id plane_id;

		for_each_plane_id_on_crtc(crtc, plane_id) {
			intermediate->wm[level].plane[plane_id] =
				min(optimal->wm[level].plane[plane_id],
				    active->wm[level].plane[plane_id]);
		}

		intermediate->sr[level].plane = min(optimal->sr[level].plane,
						    active->sr[level].plane);
		intermediate->sr[level].cursor = min(optimal->sr[level].cursor,
						     active->sr[level].cursor);
	}

	vlv_invalidate_wms(crtc, intermediate, level);

out:
	/*
	 * If our intermediate WM are identical to the final WM, then we can
	 * omit the post-vblank programming; only update if it's different.
	 */
	if (memcmp(intermediate, optimal, sizeof(*intermediate)) != 0)
		new_crtc_state->wm.need_postvbl_update = true;

	return 0;
}

static void vlv_merge_wm(struct drm_i915_private *dev_priv,
			 struct vlv_wm_values *wm)
{
	struct intel_crtc *crtc;
<<<<<<< HEAD
	int num_active_crtcs = 0;
=======
	int num_active_pipes = 0;
>>>>>>> upstream/android-13

	wm->level = dev_priv->wm.max_level;
	wm->cxsr = true;

	for_each_intel_crtc(&dev_priv->drm, crtc) {
		const struct vlv_wm_state *wm_state = &crtc->wm.active.vlv;

		if (!crtc->active)
			continue;

		if (!wm_state->cxsr)
			wm->cxsr = false;

<<<<<<< HEAD
		num_active_crtcs++;
		wm->level = min_t(int, wm->level, wm_state->num_levels - 1);
	}

	if (num_active_crtcs != 1)
		wm->cxsr = false;

	if (num_active_crtcs > 1)
=======
		num_active_pipes++;
		wm->level = min_t(int, wm->level, wm_state->num_levels - 1);
	}

	if (num_active_pipes != 1)
		wm->cxsr = false;

	if (num_active_pipes > 1)
>>>>>>> upstream/android-13
		wm->level = VLV_WM_LEVEL_PM2;

	for_each_intel_crtc(&dev_priv->drm, crtc) {
		const struct vlv_wm_state *wm_state = &crtc->wm.active.vlv;
		enum pipe pipe = crtc->pipe;

		wm->pipe[pipe] = wm_state->wm[wm->level];
		if (crtc->active && wm->cxsr)
			wm->sr = wm_state->sr[wm->level];

		wm->ddl[pipe].plane[PLANE_PRIMARY] = DDL_PRECISION_HIGH | 2;
		wm->ddl[pipe].plane[PLANE_SPRITE0] = DDL_PRECISION_HIGH | 2;
		wm->ddl[pipe].plane[PLANE_SPRITE1] = DDL_PRECISION_HIGH | 2;
		wm->ddl[pipe].plane[PLANE_CURSOR] = DDL_PRECISION_HIGH | 2;
	}
}

static void vlv_program_watermarks(struct drm_i915_private *dev_priv)
{
	struct vlv_wm_values *old_wm = &dev_priv->wm.vlv;
	struct vlv_wm_values new_wm = {};

	vlv_merge_wm(dev_priv, &new_wm);

	if (memcmp(old_wm, &new_wm, sizeof(new_wm)) == 0)
		return;

	if (is_disabling(old_wm->level, new_wm.level, VLV_WM_LEVEL_DDR_DVFS))
		chv_set_memory_dvfs(dev_priv, false);

	if (is_disabling(old_wm->level, new_wm.level, VLV_WM_LEVEL_PM5))
		chv_set_memory_pm5(dev_priv, false);

	if (is_disabling(old_wm->cxsr, new_wm.cxsr, true))
		_intel_set_memory_cxsr(dev_priv, false);

	vlv_write_wm_values(dev_priv, &new_wm);

	if (is_enabling(old_wm->cxsr, new_wm.cxsr, true))
		_intel_set_memory_cxsr(dev_priv, true);

	if (is_enabling(old_wm->level, new_wm.level, VLV_WM_LEVEL_PM5))
		chv_set_memory_pm5(dev_priv, true);

	if (is_enabling(old_wm->level, new_wm.level, VLV_WM_LEVEL_DDR_DVFS))
		chv_set_memory_dvfs(dev_priv, true);

	*old_wm = new_wm;
}

static void vlv_initial_watermarks(struct intel_atomic_state *state,
<<<<<<< HEAD
				   struct intel_crtc_state *crtc_state)
{
	struct drm_i915_private *dev_priv = to_i915(crtc_state->base.crtc->dev);
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->base.crtc);
=======
				   struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	const struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
>>>>>>> upstream/android-13

	mutex_lock(&dev_priv->wm.wm_mutex);
	crtc->wm.active.vlv = crtc_state->wm.vlv.intermediate;
	vlv_program_watermarks(dev_priv);
	mutex_unlock(&dev_priv->wm.wm_mutex);
}

static void vlv_optimize_watermarks(struct intel_atomic_state *state,
<<<<<<< HEAD
				    struct intel_crtc_state *crtc_state)
{
	struct drm_i915_private *dev_priv = to_i915(crtc_state->base.crtc->dev);
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc_state->base.crtc);
=======
				    struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	const struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
>>>>>>> upstream/android-13

	if (!crtc_state->wm.need_postvbl_update)
		return;

	mutex_lock(&dev_priv->wm.wm_mutex);
<<<<<<< HEAD
	intel_crtc->wm.active.vlv = crtc_state->wm.vlv.optimal;
=======
	crtc->wm.active.vlv = crtc_state->wm.vlv.optimal;
>>>>>>> upstream/android-13
	vlv_program_watermarks(dev_priv);
	mutex_unlock(&dev_priv->wm.wm_mutex);
}

static void i965_update_wm(struct intel_crtc *unused_crtc)
{
	struct drm_i915_private *dev_priv = to_i915(unused_crtc->base.dev);
	struct intel_crtc *crtc;
	int srwm = 1;
	int cursor_sr = 16;
	bool cxsr_enabled;

	/* Calc sr entries for one plane configs */
	crtc = single_enabled_crtc(dev_priv);
	if (crtc) {
		/* self-refresh has much higher latency */
		static const int sr_latency_ns = 12000;
<<<<<<< HEAD
		const struct drm_display_mode *adjusted_mode =
			&crtc->config->base.adjusted_mode;
		const struct drm_framebuffer *fb =
			crtc->base.primary->state->fb;
		int clock = adjusted_mode->crtc_clock;
		int htotal = adjusted_mode->crtc_htotal;
=======
		const struct drm_display_mode *pipe_mode =
			&crtc->config->hw.pipe_mode;
		const struct drm_framebuffer *fb =
			crtc->base.primary->state->fb;
		int clock = pipe_mode->crtc_clock;
		int htotal = pipe_mode->crtc_htotal;
>>>>>>> upstream/android-13
		int hdisplay = crtc->config->pipe_src_w;
		int cpp = fb->format->cpp[0];
		int entries;

		entries = intel_wm_method2(clock, htotal,
					   hdisplay, cpp, sr_latency_ns / 100);
		entries = DIV_ROUND_UP(entries, I915_FIFO_LINE_SIZE);
		srwm = I965_FIFO_SIZE - entries;
		if (srwm < 0)
			srwm = 1;
		srwm &= 0x1ff;
<<<<<<< HEAD
		DRM_DEBUG_KMS("self-refresh entries: %d, wm: %d\n",
			      entries, srwm);
=======
		drm_dbg_kms(&dev_priv->drm,
			    "self-refresh entries: %d, wm: %d\n",
			    entries, srwm);
>>>>>>> upstream/android-13

		entries = intel_wm_method2(clock, htotal,
					   crtc->base.cursor->state->crtc_w, 4,
					   sr_latency_ns / 100);
		entries = DIV_ROUND_UP(entries,
				       i965_cursor_wm_info.cacheline_size) +
			i965_cursor_wm_info.guard_size;

		cursor_sr = i965_cursor_wm_info.fifo_size - entries;
		if (cursor_sr > i965_cursor_wm_info.max_wm)
			cursor_sr = i965_cursor_wm_info.max_wm;

<<<<<<< HEAD
		DRM_DEBUG_KMS("self-refresh watermark: display plane %d "
			      "cursor %d\n", srwm, cursor_sr);
=======
		drm_dbg_kms(&dev_priv->drm,
			    "self-refresh watermark: display plane %d "
			    "cursor %d\n", srwm, cursor_sr);
>>>>>>> upstream/android-13

		cxsr_enabled = true;
	} else {
		cxsr_enabled = false;
		/* Turn off self refresh if both pipes are enabled */
		intel_set_memory_cxsr(dev_priv, false);
	}

<<<<<<< HEAD
	DRM_DEBUG_KMS("Setting FIFO watermarks - A: 8, B: 8, C: 8, SR %d\n",
		      srwm);

	/* 965 has limitations... */
	I915_WRITE(DSPFW1, FW_WM(srwm, SR) |
		   FW_WM(8, CURSORB) |
		   FW_WM(8, PLANEB) |
		   FW_WM(8, PLANEA));
	I915_WRITE(DSPFW2, FW_WM(8, CURSORA) |
		   FW_WM(8, PLANEC_OLD));
	/* update cursor SR watermark */
	I915_WRITE(DSPFW3, FW_WM(cursor_sr, CURSOR_SR));
=======
	drm_dbg_kms(&dev_priv->drm,
		    "Setting FIFO watermarks - A: 8, B: 8, C: 8, SR %d\n",
		    srwm);

	/* 965 has limitations... */
	intel_uncore_write(&dev_priv->uncore, DSPFW1, FW_WM(srwm, SR) |
		   FW_WM(8, CURSORB) |
		   FW_WM(8, PLANEB) |
		   FW_WM(8, PLANEA));
	intel_uncore_write(&dev_priv->uncore, DSPFW2, FW_WM(8, CURSORA) |
		   FW_WM(8, PLANEC_OLD));
	/* update cursor SR watermark */
	intel_uncore_write(&dev_priv->uncore, DSPFW3, FW_WM(cursor_sr, CURSOR_SR));
>>>>>>> upstream/android-13

	if (cxsr_enabled)
		intel_set_memory_cxsr(dev_priv, true);
}

#undef FW_WM

static void i9xx_update_wm(struct intel_crtc *unused_crtc)
{
	struct drm_i915_private *dev_priv = to_i915(unused_crtc->base.dev);
	const struct intel_watermark_params *wm_info;
<<<<<<< HEAD
	uint32_t fwater_lo;
	uint32_t fwater_hi;
=======
	u32 fwater_lo;
	u32 fwater_hi;
>>>>>>> upstream/android-13
	int cwm, srwm = 1;
	int fifo_size;
	int planea_wm, planeb_wm;
	struct intel_crtc *crtc, *enabled = NULL;

	if (IS_I945GM(dev_priv))
		wm_info = &i945_wm_info;
<<<<<<< HEAD
	else if (!IS_GEN2(dev_priv))
=======
	else if (DISPLAY_VER(dev_priv) != 2)
>>>>>>> upstream/android-13
		wm_info = &i915_wm_info;
	else
		wm_info = &i830_a_wm_info;

	fifo_size = dev_priv->display.get_fifo_size(dev_priv, PLANE_A);
	crtc = intel_get_crtc_for_plane(dev_priv, PLANE_A);
	if (intel_crtc_active(crtc)) {
<<<<<<< HEAD
		const struct drm_display_mode *adjusted_mode =
			&crtc->config->base.adjusted_mode;
=======
		const struct drm_display_mode *pipe_mode =
			&crtc->config->hw.pipe_mode;
>>>>>>> upstream/android-13
		const struct drm_framebuffer *fb =
			crtc->base.primary->state->fb;
		int cpp;

<<<<<<< HEAD
		if (IS_GEN2(dev_priv))
=======
		if (DISPLAY_VER(dev_priv) == 2)
>>>>>>> upstream/android-13
			cpp = 4;
		else
			cpp = fb->format->cpp[0];

<<<<<<< HEAD
		planea_wm = intel_calculate_wm(adjusted_mode->crtc_clock,
=======
		planea_wm = intel_calculate_wm(pipe_mode->crtc_clock,
>>>>>>> upstream/android-13
					       wm_info, fifo_size, cpp,
					       pessimal_latency_ns);
		enabled = crtc;
	} else {
		planea_wm = fifo_size - wm_info->guard_size;
		if (planea_wm > (long)wm_info->max_wm)
			planea_wm = wm_info->max_wm;
	}

<<<<<<< HEAD
	if (IS_GEN2(dev_priv))
=======
	if (DISPLAY_VER(dev_priv) == 2)
>>>>>>> upstream/android-13
		wm_info = &i830_bc_wm_info;

	fifo_size = dev_priv->display.get_fifo_size(dev_priv, PLANE_B);
	crtc = intel_get_crtc_for_plane(dev_priv, PLANE_B);
	if (intel_crtc_active(crtc)) {
<<<<<<< HEAD
		const struct drm_display_mode *adjusted_mode =
			&crtc->config->base.adjusted_mode;
=======
		const struct drm_display_mode *pipe_mode =
			&crtc->config->hw.pipe_mode;
>>>>>>> upstream/android-13
		const struct drm_framebuffer *fb =
			crtc->base.primary->state->fb;
		int cpp;

<<<<<<< HEAD
		if (IS_GEN2(dev_priv))
=======
		if (DISPLAY_VER(dev_priv) == 2)
>>>>>>> upstream/android-13
			cpp = 4;
		else
			cpp = fb->format->cpp[0];

<<<<<<< HEAD
		planeb_wm = intel_calculate_wm(adjusted_mode->crtc_clock,
=======
		planeb_wm = intel_calculate_wm(pipe_mode->crtc_clock,
>>>>>>> upstream/android-13
					       wm_info, fifo_size, cpp,
					       pessimal_latency_ns);
		if (enabled == NULL)
			enabled = crtc;
		else
			enabled = NULL;
	} else {
		planeb_wm = fifo_size - wm_info->guard_size;
		if (planeb_wm > (long)wm_info->max_wm)
			planeb_wm = wm_info->max_wm;
	}

<<<<<<< HEAD
	DRM_DEBUG_KMS("FIFO watermarks - A: %d, B: %d\n", planea_wm, planeb_wm);
=======
	drm_dbg_kms(&dev_priv->drm,
		    "FIFO watermarks - A: %d, B: %d\n", planea_wm, planeb_wm);
>>>>>>> upstream/android-13

	if (IS_I915GM(dev_priv) && enabled) {
		struct drm_i915_gem_object *obj;

		obj = intel_fb_obj(enabled->base.primary->state->fb);

		/* self-refresh seems busted with untiled */
		if (!i915_gem_object_is_tiled(obj))
			enabled = NULL;
	}

	/*
	 * Overlay gets an aggressive default since video jitter is bad.
	 */
	cwm = 2;

	/* Play safe and disable self-refresh before adjusting watermarks. */
	intel_set_memory_cxsr(dev_priv, false);

	/* Calc sr entries for one plane configs */
	if (HAS_FW_BLC(dev_priv) && enabled) {
		/* self-refresh has much higher latency */
		static const int sr_latency_ns = 6000;
<<<<<<< HEAD
		const struct drm_display_mode *adjusted_mode =
			&enabled->config->base.adjusted_mode;
		const struct drm_framebuffer *fb =
			enabled->base.primary->state->fb;
		int clock = adjusted_mode->crtc_clock;
		int htotal = adjusted_mode->crtc_htotal;
=======
		const struct drm_display_mode *pipe_mode =
			&enabled->config->hw.pipe_mode;
		const struct drm_framebuffer *fb =
			enabled->base.primary->state->fb;
		int clock = pipe_mode->crtc_clock;
		int htotal = pipe_mode->crtc_htotal;
>>>>>>> upstream/android-13
		int hdisplay = enabled->config->pipe_src_w;
		int cpp;
		int entries;

		if (IS_I915GM(dev_priv) || IS_I945GM(dev_priv))
			cpp = 4;
		else
			cpp = fb->format->cpp[0];

		entries = intel_wm_method2(clock, htotal, hdisplay, cpp,
					   sr_latency_ns / 100);
		entries = DIV_ROUND_UP(entries, wm_info->cacheline_size);
<<<<<<< HEAD
		DRM_DEBUG_KMS("self-refresh entries: %d\n", entries);
=======
		drm_dbg_kms(&dev_priv->drm,
			    "self-refresh entries: %d\n", entries);
>>>>>>> upstream/android-13
		srwm = wm_info->fifo_size - entries;
		if (srwm < 0)
			srwm = 1;

		if (IS_I945G(dev_priv) || IS_I945GM(dev_priv))
<<<<<<< HEAD
			I915_WRITE(FW_BLC_SELF,
				   FW_BLC_SELF_FIFO_MASK | (srwm & 0xff));
		else
			I915_WRITE(FW_BLC_SELF, srwm & 0x3f);
	}

	DRM_DEBUG_KMS("Setting FIFO watermarks - A: %d, B: %d, C: %d, SR %d\n",
		      planea_wm, planeb_wm, cwm, srwm);
=======
			intel_uncore_write(&dev_priv->uncore, FW_BLC_SELF,
				   FW_BLC_SELF_FIFO_MASK | (srwm & 0xff));
		else
			intel_uncore_write(&dev_priv->uncore, FW_BLC_SELF, srwm & 0x3f);
	}

	drm_dbg_kms(&dev_priv->drm,
		    "Setting FIFO watermarks - A: %d, B: %d, C: %d, SR %d\n",
		     planea_wm, planeb_wm, cwm, srwm);
>>>>>>> upstream/android-13

	fwater_lo = ((planeb_wm & 0x3f) << 16) | (planea_wm & 0x3f);
	fwater_hi = (cwm & 0x1f);

	/* Set request length to 8 cachelines per fetch */
	fwater_lo = fwater_lo | (1 << 24) | (1 << 8);
	fwater_hi = fwater_hi | (1 << 8);

<<<<<<< HEAD
	I915_WRITE(FW_BLC, fwater_lo);
	I915_WRITE(FW_BLC2, fwater_hi);
=======
	intel_uncore_write(&dev_priv->uncore, FW_BLC, fwater_lo);
	intel_uncore_write(&dev_priv->uncore, FW_BLC2, fwater_hi);
>>>>>>> upstream/android-13

	if (enabled)
		intel_set_memory_cxsr(dev_priv, true);
}

static void i845_update_wm(struct intel_crtc *unused_crtc)
{
	struct drm_i915_private *dev_priv = to_i915(unused_crtc->base.dev);
	struct intel_crtc *crtc;
<<<<<<< HEAD
	const struct drm_display_mode *adjusted_mode;
	uint32_t fwater_lo;
=======
	const struct drm_display_mode *pipe_mode;
	u32 fwater_lo;
>>>>>>> upstream/android-13
	int planea_wm;

	crtc = single_enabled_crtc(dev_priv);
	if (crtc == NULL)
		return;

<<<<<<< HEAD
	adjusted_mode = &crtc->config->base.adjusted_mode;
	planea_wm = intel_calculate_wm(adjusted_mode->crtc_clock,
				       &i845_wm_info,
				       dev_priv->display.get_fifo_size(dev_priv, PLANE_A),
				       4, pessimal_latency_ns);
	fwater_lo = I915_READ(FW_BLC) & ~0xfff;
	fwater_lo |= (3<<8) | planea_wm;

	DRM_DEBUG_KMS("Setting FIFO watermarks - A: %d\n", planea_wm);

	I915_WRITE(FW_BLC, fwater_lo);
=======
	pipe_mode = &crtc->config->hw.pipe_mode;
	planea_wm = intel_calculate_wm(pipe_mode->crtc_clock,
				       &i845_wm_info,
				       dev_priv->display.get_fifo_size(dev_priv, PLANE_A),
				       4, pessimal_latency_ns);
	fwater_lo = intel_uncore_read(&dev_priv->uncore, FW_BLC) & ~0xfff;
	fwater_lo |= (3<<8) | planea_wm;

	drm_dbg_kms(&dev_priv->drm,
		    "Setting FIFO watermarks - A: %d\n", planea_wm);

	intel_uncore_write(&dev_priv->uncore, FW_BLC, fwater_lo);
>>>>>>> upstream/android-13
}

/* latency must be in 0.1us units. */
static unsigned int ilk_wm_method1(unsigned int pixel_rate,
				   unsigned int cpp,
				   unsigned int latency)
{
	unsigned int ret;

	ret = intel_wm_method1(pixel_rate, cpp, latency);
	ret = DIV_ROUND_UP(ret, 64) + 2;

	return ret;
}

/* latency must be in 0.1us units. */
static unsigned int ilk_wm_method2(unsigned int pixel_rate,
				   unsigned int htotal,
				   unsigned int width,
				   unsigned int cpp,
				   unsigned int latency)
{
	unsigned int ret;

	ret = intel_wm_method2(pixel_rate, htotal,
			       width, cpp, latency);
	ret = DIV_ROUND_UP(ret, 64) + 2;

	return ret;
}

<<<<<<< HEAD
static uint32_t ilk_wm_fbc(uint32_t pri_val, uint32_t horiz_pixels,
			   uint8_t cpp)
=======
static u32 ilk_wm_fbc(u32 pri_val, u32 horiz_pixels, u8 cpp)
>>>>>>> upstream/android-13
{
	/*
	 * Neither of these should be possible since this function shouldn't be
	 * called if the CRTC is off or the plane is invisible.  But let's be
	 * extra paranoid to avoid a potential divide-by-zero if we screw up
	 * elsewhere in the driver.
	 */
	if (WARN_ON(!cpp))
		return 0;
	if (WARN_ON(!horiz_pixels))
		return 0;

	return DIV_ROUND_UP(pri_val * 64, horiz_pixels * cpp) + 2;
}

struct ilk_wm_maximums {
<<<<<<< HEAD
	uint16_t pri;
	uint16_t spr;
	uint16_t cur;
	uint16_t fbc;
=======
	u16 pri;
	u16 spr;
	u16 cur;
	u16 fbc;
>>>>>>> upstream/android-13
};

/*
 * For both WM_PIPE and WM_LP.
 * mem_value must be in 0.1us units.
 */
<<<<<<< HEAD
static uint32_t ilk_compute_pri_wm(const struct intel_crtc_state *cstate,
				   const struct intel_plane_state *pstate,
				   uint32_t mem_value,
				   bool is_lp)
{
	uint32_t method1, method2;
=======
static u32 ilk_compute_pri_wm(const struct intel_crtc_state *crtc_state,
			      const struct intel_plane_state *plane_state,
			      u32 mem_value, bool is_lp)
{
	u32 method1, method2;
>>>>>>> upstream/android-13
	int cpp;

	if (mem_value == 0)
		return U32_MAX;

<<<<<<< HEAD
	if (!intel_wm_plane_visible(cstate, pstate))
		return 0;

	cpp = pstate->base.fb->format->cpp[0];

	method1 = ilk_wm_method1(cstate->pixel_rate, cpp, mem_value);
=======
	if (!intel_wm_plane_visible(crtc_state, plane_state))
		return 0;

	cpp = plane_state->hw.fb->format->cpp[0];

	method1 = ilk_wm_method1(crtc_state->pixel_rate, cpp, mem_value);
>>>>>>> upstream/android-13

	if (!is_lp)
		return method1;

<<<<<<< HEAD
	method2 = ilk_wm_method2(cstate->pixel_rate,
				 cstate->base.adjusted_mode.crtc_htotal,
				 drm_rect_width(&pstate->base.dst),
=======
	method2 = ilk_wm_method2(crtc_state->pixel_rate,
				 crtc_state->hw.pipe_mode.crtc_htotal,
				 drm_rect_width(&plane_state->uapi.dst),
>>>>>>> upstream/android-13
				 cpp, mem_value);

	return min(method1, method2);
}

/*
 * For both WM_PIPE and WM_LP.
 * mem_value must be in 0.1us units.
 */
<<<<<<< HEAD
static uint32_t ilk_compute_spr_wm(const struct intel_crtc_state *cstate,
				   const struct intel_plane_state *pstate,
				   uint32_t mem_value)
{
	uint32_t method1, method2;
=======
static u32 ilk_compute_spr_wm(const struct intel_crtc_state *crtc_state,
			      const struct intel_plane_state *plane_state,
			      u32 mem_value)
{
	u32 method1, method2;
>>>>>>> upstream/android-13
	int cpp;

	if (mem_value == 0)
		return U32_MAX;

<<<<<<< HEAD
	if (!intel_wm_plane_visible(cstate, pstate))
		return 0;

	cpp = pstate->base.fb->format->cpp[0];

	method1 = ilk_wm_method1(cstate->pixel_rate, cpp, mem_value);
	method2 = ilk_wm_method2(cstate->pixel_rate,
				 cstate->base.adjusted_mode.crtc_htotal,
				 drm_rect_width(&pstate->base.dst),
=======
	if (!intel_wm_plane_visible(crtc_state, plane_state))
		return 0;

	cpp = plane_state->hw.fb->format->cpp[0];

	method1 = ilk_wm_method1(crtc_state->pixel_rate, cpp, mem_value);
	method2 = ilk_wm_method2(crtc_state->pixel_rate,
				 crtc_state->hw.pipe_mode.crtc_htotal,
				 drm_rect_width(&plane_state->uapi.dst),
>>>>>>> upstream/android-13
				 cpp, mem_value);
	return min(method1, method2);
}

/*
 * For both WM_PIPE and WM_LP.
 * mem_value must be in 0.1us units.
 */
<<<<<<< HEAD
static uint32_t ilk_compute_cur_wm(const struct intel_crtc_state *cstate,
				   const struct intel_plane_state *pstate,
				   uint32_t mem_value)
=======
static u32 ilk_compute_cur_wm(const struct intel_crtc_state *crtc_state,
			      const struct intel_plane_state *plane_state,
			      u32 mem_value)
>>>>>>> upstream/android-13
{
	int cpp;

	if (mem_value == 0)
		return U32_MAX;

<<<<<<< HEAD
	if (!intel_wm_plane_visible(cstate, pstate))
		return 0;

	cpp = pstate->base.fb->format->cpp[0];

	return ilk_wm_method2(cstate->pixel_rate,
			      cstate->base.adjusted_mode.crtc_htotal,
			      pstate->base.crtc_w, cpp, mem_value);
}

/* Only for WM_LP. */
static uint32_t ilk_compute_fbc_wm(const struct intel_crtc_state *cstate,
				   const struct intel_plane_state *pstate,
				   uint32_t pri_val)
{
	int cpp;

	if (!intel_wm_plane_visible(cstate, pstate))
		return 0;

	cpp = pstate->base.fb->format->cpp[0];

	return ilk_wm_fbc(pri_val, drm_rect_width(&pstate->base.dst), cpp);
=======
	if (!intel_wm_plane_visible(crtc_state, plane_state))
		return 0;

	cpp = plane_state->hw.fb->format->cpp[0];

	return ilk_wm_method2(crtc_state->pixel_rate,
			      crtc_state->hw.pipe_mode.crtc_htotal,
			      drm_rect_width(&plane_state->uapi.dst),
			      cpp, mem_value);
}

/* Only for WM_LP. */
static u32 ilk_compute_fbc_wm(const struct intel_crtc_state *crtc_state,
			      const struct intel_plane_state *plane_state,
			      u32 pri_val)
{
	int cpp;

	if (!intel_wm_plane_visible(crtc_state, plane_state))
		return 0;

	cpp = plane_state->hw.fb->format->cpp[0];

	return ilk_wm_fbc(pri_val, drm_rect_width(&plane_state->uapi.dst),
			  cpp);
>>>>>>> upstream/android-13
}

static unsigned int
ilk_display_fifo_size(const struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	if (INTEL_GEN(dev_priv) >= 8)
		return 3072;
	else if (INTEL_GEN(dev_priv) >= 7)
=======
	if (DISPLAY_VER(dev_priv) >= 8)
		return 3072;
	else if (DISPLAY_VER(dev_priv) >= 7)
>>>>>>> upstream/android-13
		return 768;
	else
		return 512;
}

static unsigned int
ilk_plane_wm_reg_max(const struct drm_i915_private *dev_priv,
		     int level, bool is_sprite)
{
<<<<<<< HEAD
	if (INTEL_GEN(dev_priv) >= 8)
		/* BDW primary/sprite plane watermarks */
		return level == 0 ? 255 : 2047;
	else if (INTEL_GEN(dev_priv) >= 7)
=======
	if (DISPLAY_VER(dev_priv) >= 8)
		/* BDW primary/sprite plane watermarks */
		return level == 0 ? 255 : 2047;
	else if (DISPLAY_VER(dev_priv) >= 7)
>>>>>>> upstream/android-13
		/* IVB/HSW primary/sprite plane watermarks */
		return level == 0 ? 127 : 1023;
	else if (!is_sprite)
		/* ILK/SNB primary plane watermarks */
		return level == 0 ? 127 : 511;
	else
		/* ILK/SNB sprite plane watermarks */
		return level == 0 ? 63 : 255;
}

static unsigned int
ilk_cursor_wm_reg_max(const struct drm_i915_private *dev_priv, int level)
{
<<<<<<< HEAD
	if (INTEL_GEN(dev_priv) >= 7)
=======
	if (DISPLAY_VER(dev_priv) >= 7)
>>>>>>> upstream/android-13
		return level == 0 ? 63 : 255;
	else
		return level == 0 ? 31 : 63;
}

static unsigned int ilk_fbc_wm_reg_max(const struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	if (INTEL_GEN(dev_priv) >= 8)
=======
	if (DISPLAY_VER(dev_priv) >= 8)
>>>>>>> upstream/android-13
		return 31;
	else
		return 15;
}

/* Calculate the maximum primary/sprite plane watermark */
<<<<<<< HEAD
static unsigned int ilk_plane_wm_max(const struct drm_device *dev,
=======
static unsigned int ilk_plane_wm_max(const struct drm_i915_private *dev_priv,
>>>>>>> upstream/android-13
				     int level,
				     const struct intel_wm_config *config,
				     enum intel_ddb_partitioning ddb_partitioning,
				     bool is_sprite)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = to_i915(dev);
=======
>>>>>>> upstream/android-13
	unsigned int fifo_size = ilk_display_fifo_size(dev_priv);

	/* if sprites aren't enabled, sprites get nothing */
	if (is_sprite && !config->sprites_enabled)
		return 0;

	/* HSW allows LP1+ watermarks even with multiple pipes */
	if (level == 0 || config->num_pipes_active > 1) {
<<<<<<< HEAD
		fifo_size /= INTEL_INFO(dev_priv)->num_pipes;
=======
		fifo_size /= INTEL_NUM_PIPES(dev_priv);
>>>>>>> upstream/android-13

		/*
		 * For some reason the non self refresh
		 * FIFO size is only half of the self
		 * refresh FIFO size on ILK/SNB.
		 */
<<<<<<< HEAD
		if (INTEL_GEN(dev_priv) <= 6)
=======
		if (DISPLAY_VER(dev_priv) <= 6)
>>>>>>> upstream/android-13
			fifo_size /= 2;
	}

	if (config->sprites_enabled) {
		/* level 0 is always calculated with 1:1 split */
		if (level > 0 && ddb_partitioning == INTEL_DDB_PART_5_6) {
			if (is_sprite)
				fifo_size *= 5;
			fifo_size /= 6;
		} else {
			fifo_size /= 2;
		}
	}

	/* clamp to max that the registers can hold */
	return min(fifo_size, ilk_plane_wm_reg_max(dev_priv, level, is_sprite));
}

/* Calculate the maximum cursor plane watermark */
<<<<<<< HEAD
static unsigned int ilk_cursor_wm_max(const struct drm_device *dev,
=======
static unsigned int ilk_cursor_wm_max(const struct drm_i915_private *dev_priv,
>>>>>>> upstream/android-13
				      int level,
				      const struct intel_wm_config *config)
{
	/* HSW LP1+ watermarks w/ multiple pipes */
	if (level > 0 && config->num_pipes_active > 1)
		return 64;

	/* otherwise just report max that registers can hold */
<<<<<<< HEAD
	return ilk_cursor_wm_reg_max(to_i915(dev), level);
}

static void ilk_compute_wm_maximums(const struct drm_device *dev,
=======
	return ilk_cursor_wm_reg_max(dev_priv, level);
}

static void ilk_compute_wm_maximums(const struct drm_i915_private *dev_priv,
>>>>>>> upstream/android-13
				    int level,
				    const struct intel_wm_config *config,
				    enum intel_ddb_partitioning ddb_partitioning,
				    struct ilk_wm_maximums *max)
{
<<<<<<< HEAD
	max->pri = ilk_plane_wm_max(dev, level, config, ddb_partitioning, false);
	max->spr = ilk_plane_wm_max(dev, level, config, ddb_partitioning, true);
	max->cur = ilk_cursor_wm_max(dev, level, config);
	max->fbc = ilk_fbc_wm_reg_max(to_i915(dev));
=======
	max->pri = ilk_plane_wm_max(dev_priv, level, config, ddb_partitioning, false);
	max->spr = ilk_plane_wm_max(dev_priv, level, config, ddb_partitioning, true);
	max->cur = ilk_cursor_wm_max(dev_priv, level, config);
	max->fbc = ilk_fbc_wm_reg_max(dev_priv);
>>>>>>> upstream/android-13
}

static void ilk_compute_wm_reg_maximums(const struct drm_i915_private *dev_priv,
					int level,
					struct ilk_wm_maximums *max)
{
	max->pri = ilk_plane_wm_reg_max(dev_priv, level, false);
	max->spr = ilk_plane_wm_reg_max(dev_priv, level, true);
	max->cur = ilk_cursor_wm_reg_max(dev_priv, level);
	max->fbc = ilk_fbc_wm_reg_max(dev_priv);
}

static bool ilk_validate_wm_level(int level,
				  const struct ilk_wm_maximums *max,
				  struct intel_wm_level *result)
{
	bool ret;

	/* already determined to be invalid? */
	if (!result->enable)
		return false;

	result->enable = result->pri_val <= max->pri &&
			 result->spr_val <= max->spr &&
			 result->cur_val <= max->cur;

	ret = result->enable;

	/*
	 * HACK until we can pre-compute everything,
	 * and thus fail gracefully if LP0 watermarks
	 * are exceeded...
	 */
	if (level == 0 && !result->enable) {
		if (result->pri_val > max->pri)
			DRM_DEBUG_KMS("Primary WM%d too large %u (max %u)\n",
				      level, result->pri_val, max->pri);
		if (result->spr_val > max->spr)
			DRM_DEBUG_KMS("Sprite WM%d too large %u (max %u)\n",
				      level, result->spr_val, max->spr);
		if (result->cur_val > max->cur)
			DRM_DEBUG_KMS("Cursor WM%d too large %u (max %u)\n",
				      level, result->cur_val, max->cur);

<<<<<<< HEAD
		result->pri_val = min_t(uint32_t, result->pri_val, max->pri);
		result->spr_val = min_t(uint32_t, result->spr_val, max->spr);
		result->cur_val = min_t(uint32_t, result->cur_val, max->cur);
=======
		result->pri_val = min_t(u32, result->pri_val, max->pri);
		result->spr_val = min_t(u32, result->spr_val, max->spr);
		result->cur_val = min_t(u32, result->cur_val, max->cur);
>>>>>>> upstream/android-13
		result->enable = true;
	}

	return ret;
}

static void ilk_compute_wm_level(const struct drm_i915_private *dev_priv,
<<<<<<< HEAD
				 const struct intel_crtc *intel_crtc,
				 int level,
				 struct intel_crtc_state *cstate,
=======
				 const struct intel_crtc *crtc,
				 int level,
				 struct intel_crtc_state *crtc_state,
>>>>>>> upstream/android-13
				 const struct intel_plane_state *pristate,
				 const struct intel_plane_state *sprstate,
				 const struct intel_plane_state *curstate,
				 struct intel_wm_level *result)
{
<<<<<<< HEAD
	uint16_t pri_latency = dev_priv->wm.pri_latency[level];
	uint16_t spr_latency = dev_priv->wm.spr_latency[level];
	uint16_t cur_latency = dev_priv->wm.cur_latency[level];
=======
	u16 pri_latency = dev_priv->wm.pri_latency[level];
	u16 spr_latency = dev_priv->wm.spr_latency[level];
	u16 cur_latency = dev_priv->wm.cur_latency[level];
>>>>>>> upstream/android-13

	/* WM1+ latency values stored in 0.5us units */
	if (level > 0) {
		pri_latency *= 5;
		spr_latency *= 5;
		cur_latency *= 5;
	}

	if (pristate) {
<<<<<<< HEAD
		result->pri_val = ilk_compute_pri_wm(cstate, pristate,
						     pri_latency, level);
		result->fbc_val = ilk_compute_fbc_wm(cstate, pristate, result->pri_val);
	}

	if (sprstate)
		result->spr_val = ilk_compute_spr_wm(cstate, sprstate, spr_latency);

	if (curstate)
		result->cur_val = ilk_compute_cur_wm(cstate, curstate, cur_latency);
=======
		result->pri_val = ilk_compute_pri_wm(crtc_state, pristate,
						     pri_latency, level);
		result->fbc_val = ilk_compute_fbc_wm(crtc_state, pristate, result->pri_val);
	}

	if (sprstate)
		result->spr_val = ilk_compute_spr_wm(crtc_state, sprstate, spr_latency);

	if (curstate)
		result->cur_val = ilk_compute_cur_wm(crtc_state, curstate, cur_latency);
>>>>>>> upstream/android-13

	result->enable = true;
}

<<<<<<< HEAD
static uint32_t
hsw_compute_linetime_wm(const struct intel_crtc_state *cstate)
{
	const struct intel_atomic_state *intel_state =
		to_intel_atomic_state(cstate->base.state);
	const struct drm_display_mode *adjusted_mode =
		&cstate->base.adjusted_mode;
	u32 linetime, ips_linetime;

	if (!cstate->base.active)
		return 0;
	if (WARN_ON(adjusted_mode->crtc_clock == 0))
		return 0;
	if (WARN_ON(intel_state->cdclk.logical.cdclk == 0))
		return 0;

	/* The WM are computed with base on how long it takes to fill a single
	 * row at the given clock rate, multiplied by 8.
	 * */
	linetime = DIV_ROUND_CLOSEST(adjusted_mode->crtc_htotal * 1000 * 8,
				     adjusted_mode->crtc_clock);
	ips_linetime = DIV_ROUND_CLOSEST(adjusted_mode->crtc_htotal * 1000 * 8,
					 intel_state->cdclk.logical.cdclk);

	return PIPE_WM_LINETIME_IPS_LINETIME(ips_linetime) |
	       PIPE_WM_LINETIME_TIME(linetime);
}

static void intel_read_wm_latency(struct drm_i915_private *dev_priv,
				  uint16_t wm[8])
{
	if (INTEL_GEN(dev_priv) >= 9) {
		uint32_t val;
=======
static void intel_read_wm_latency(struct drm_i915_private *dev_priv,
				  u16 wm[8])
{
	struct intel_uncore *uncore = &dev_priv->uncore;

	if (DISPLAY_VER(dev_priv) >= 9) {
		u32 val;
>>>>>>> upstream/android-13
		int ret, i;
		int level, max_level = ilk_wm_max_level(dev_priv);

		/* read the first set of memory latencies[0:3] */
		val = 0; /* data0 to be programmed to 0 for first set */
<<<<<<< HEAD
		mutex_lock(&dev_priv->pcu_lock);
		ret = sandybridge_pcode_read(dev_priv,
					     GEN9_PCODE_READ_MEM_LATENCY,
					     &val);
		mutex_unlock(&dev_priv->pcu_lock);

		if (ret) {
			DRM_ERROR("SKL Mailbox read error = %d\n", ret);
=======
		ret = sandybridge_pcode_read(dev_priv,
					     GEN9_PCODE_READ_MEM_LATENCY,
					     &val, NULL);

		if (ret) {
			drm_err(&dev_priv->drm,
				"SKL Mailbox read error = %d\n", ret);
>>>>>>> upstream/android-13
			return;
		}

		wm[0] = val & GEN9_MEM_LATENCY_LEVEL_MASK;
		wm[1] = (val >> GEN9_MEM_LATENCY_LEVEL_1_5_SHIFT) &
				GEN9_MEM_LATENCY_LEVEL_MASK;
		wm[2] = (val >> GEN9_MEM_LATENCY_LEVEL_2_6_SHIFT) &
				GEN9_MEM_LATENCY_LEVEL_MASK;
		wm[3] = (val >> GEN9_MEM_LATENCY_LEVEL_3_7_SHIFT) &
				GEN9_MEM_LATENCY_LEVEL_MASK;

		/* read the second set of memory latencies[4:7] */
		val = 1; /* data0 to be programmed to 1 for second set */
<<<<<<< HEAD
		mutex_lock(&dev_priv->pcu_lock);
		ret = sandybridge_pcode_read(dev_priv,
					     GEN9_PCODE_READ_MEM_LATENCY,
					     &val);
		mutex_unlock(&dev_priv->pcu_lock);
		if (ret) {
			DRM_ERROR("SKL Mailbox read error = %d\n", ret);
=======
		ret = sandybridge_pcode_read(dev_priv,
					     GEN9_PCODE_READ_MEM_LATENCY,
					     &val, NULL);
		if (ret) {
			drm_err(&dev_priv->drm,
				"SKL Mailbox read error = %d\n", ret);
>>>>>>> upstream/android-13
			return;
		}

		wm[4] = val & GEN9_MEM_LATENCY_LEVEL_MASK;
		wm[5] = (val >> GEN9_MEM_LATENCY_LEVEL_1_5_SHIFT) &
				GEN9_MEM_LATENCY_LEVEL_MASK;
		wm[6] = (val >> GEN9_MEM_LATENCY_LEVEL_2_6_SHIFT) &
				GEN9_MEM_LATENCY_LEVEL_MASK;
		wm[7] = (val >> GEN9_MEM_LATENCY_LEVEL_3_7_SHIFT) &
				GEN9_MEM_LATENCY_LEVEL_MASK;

		/*
		 * If a level n (n > 1) has a 0us latency, all levels m (m >= n)
		 * need to be disabled. We make sure to sanitize the values out
		 * of the punit to satisfy this requirement.
		 */
		for (level = 1; level <= max_level; level++) {
			if (wm[level] == 0) {
				for (i = level + 1; i <= max_level; i++)
					wm[i] = 0;
<<<<<<< HEAD
=======

				max_level = level - 1;

>>>>>>> upstream/android-13
				break;
			}
		}

		/*
<<<<<<< HEAD
		 * WaWmMemoryReadLatency:skl+,glk
		 *
		 * punit doesn't take into account the read latency so we need
		 * to add 2us to the various latency levels we retrieve from the
		 * punit when level 0 response data us 0us.
		 */
		if (wm[0] == 0) {
			wm[0] += 2;
			for (level = 1; level <= max_level; level++) {
				if (wm[level] == 0)
					break;
				wm[level] += 2;
			}
		}

	} else if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv)) {
		uint64_t sskpd = I915_READ64(MCH_SSKPD);
=======
		 * WaWmMemoryReadLatency
		 *
		 * punit doesn't take into account the read latency so we need
		 * to add proper adjustement to each valid level we retrieve
		 * from the punit when level 0 response data is 0us.
		 */
		if (wm[0] == 0) {
			u8 adjust = DISPLAY_VER(dev_priv) >= 12 ? 3 : 2;

			for (level = 0; level <= max_level; level++)
				wm[level] += adjust;
		}

		/*
		 * WA Level-0 adjustment for 16GB DIMMs: SKL+
		 * If we could not get dimm info enable this WA to prevent from
		 * any underrun. If not able to get Dimm info assume 16GB dimm
		 * to avoid any underrun.
		 */
		if (dev_priv->dram_info.wm_lv_0_adjust_needed)
			wm[0] += 1;
	} else if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv)) {
		u64 sskpd = intel_uncore_read64(uncore, MCH_SSKPD);
>>>>>>> upstream/android-13

		wm[0] = (sskpd >> 56) & 0xFF;
		if (wm[0] == 0)
			wm[0] = sskpd & 0xF;
		wm[1] = (sskpd >> 4) & 0xFF;
		wm[2] = (sskpd >> 12) & 0xFF;
		wm[3] = (sskpd >> 20) & 0x1FF;
		wm[4] = (sskpd >> 32) & 0x1FF;
<<<<<<< HEAD
	} else if (INTEL_GEN(dev_priv) >= 6) {
		uint32_t sskpd = I915_READ(MCH_SSKPD);
=======
	} else if (DISPLAY_VER(dev_priv) >= 6) {
		u32 sskpd = intel_uncore_read(uncore, MCH_SSKPD);
>>>>>>> upstream/android-13

		wm[0] = (sskpd >> SSKPD_WM0_SHIFT) & SSKPD_WM_MASK;
		wm[1] = (sskpd >> SSKPD_WM1_SHIFT) & SSKPD_WM_MASK;
		wm[2] = (sskpd >> SSKPD_WM2_SHIFT) & SSKPD_WM_MASK;
		wm[3] = (sskpd >> SSKPD_WM3_SHIFT) & SSKPD_WM_MASK;
<<<<<<< HEAD
	} else if (INTEL_GEN(dev_priv) >= 5) {
		uint32_t mltr = I915_READ(MLTR_ILK);
=======
	} else if (DISPLAY_VER(dev_priv) >= 5) {
		u32 mltr = intel_uncore_read(uncore, MLTR_ILK);
>>>>>>> upstream/android-13

		/* ILK primary LP0 latency is 700 ns */
		wm[0] = 7;
		wm[1] = (mltr >> MLTR_WM1_SHIFT) & ILK_SRLT_MASK;
		wm[2] = (mltr >> MLTR_WM2_SHIFT) & ILK_SRLT_MASK;
	} else {
		MISSING_CASE(INTEL_DEVID(dev_priv));
	}
}

static void intel_fixup_spr_wm_latency(struct drm_i915_private *dev_priv,
<<<<<<< HEAD
				       uint16_t wm[5])
{
	/* ILK sprite LP0 latency is 1300 ns */
	if (IS_GEN5(dev_priv))
=======
				       u16 wm[5])
{
	/* ILK sprite LP0 latency is 1300 ns */
	if (DISPLAY_VER(dev_priv) == 5)
>>>>>>> upstream/android-13
		wm[0] = 13;
}

static void intel_fixup_cur_wm_latency(struct drm_i915_private *dev_priv,
<<<<<<< HEAD
				       uint16_t wm[5])
{
	/* ILK cursor LP0 latency is 1300 ns */
	if (IS_GEN5(dev_priv))
=======
				       u16 wm[5])
{
	/* ILK cursor LP0 latency is 1300 ns */
	if (DISPLAY_VER(dev_priv) == 5)
>>>>>>> upstream/android-13
		wm[0] = 13;
}

int ilk_wm_max_level(const struct drm_i915_private *dev_priv)
{
	/* how many WM levels are we expecting */
<<<<<<< HEAD
	if (INTEL_GEN(dev_priv) >= 9)
		return 7;
	else if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv))
		return 4;
	else if (INTEL_GEN(dev_priv) >= 6)
=======
	if (HAS_HW_SAGV_WM(dev_priv))
		return 5;
	else if (DISPLAY_VER(dev_priv) >= 9)
		return 7;
	else if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv))
		return 4;
	else if (DISPLAY_VER(dev_priv) >= 6)
>>>>>>> upstream/android-13
		return 3;
	else
		return 2;
}

static void intel_print_wm_latency(struct drm_i915_private *dev_priv,
				   const char *name,
<<<<<<< HEAD
				   const uint16_t wm[])
=======
				   const u16 wm[])
>>>>>>> upstream/android-13
{
	int level, max_level = ilk_wm_max_level(dev_priv);

	for (level = 0; level <= max_level; level++) {
		unsigned int latency = wm[level];

		if (latency == 0) {
<<<<<<< HEAD
			DRM_DEBUG_KMS("%s WM%d latency not provided\n",
				      name, level);
=======
			drm_dbg_kms(&dev_priv->drm,
				    "%s WM%d latency not provided\n",
				    name, level);
>>>>>>> upstream/android-13
			continue;
		}

		/*
		 * - latencies are in us on gen9.
		 * - before then, WM1+ latency values are in 0.5us units
		 */
<<<<<<< HEAD
		if (INTEL_GEN(dev_priv) >= 9)
=======
		if (DISPLAY_VER(dev_priv) >= 9)
>>>>>>> upstream/android-13
			latency *= 10;
		else if (level > 0)
			latency *= 5;

<<<<<<< HEAD
		DRM_DEBUG_KMS("%s WM%d latency %u (%u.%u usec)\n",
			      name, level, wm[level],
			      latency / 10, latency % 10);
=======
		drm_dbg_kms(&dev_priv->drm,
			    "%s WM%d latency %u (%u.%u usec)\n", name, level,
			    wm[level], latency / 10, latency % 10);
>>>>>>> upstream/android-13
	}
}

static bool ilk_increase_wm_latency(struct drm_i915_private *dev_priv,
<<<<<<< HEAD
				    uint16_t wm[5], uint16_t min)
=======
				    u16 wm[5], u16 min)
>>>>>>> upstream/android-13
{
	int level, max_level = ilk_wm_max_level(dev_priv);

	if (wm[0] >= min)
		return false;

	wm[0] = max(wm[0], min);
	for (level = 1; level <= max_level; level++)
<<<<<<< HEAD
		wm[level] = max_t(uint16_t, wm[level], DIV_ROUND_UP(min, 5));
=======
		wm[level] = max_t(u16, wm[level], DIV_ROUND_UP(min, 5));
>>>>>>> upstream/android-13

	return true;
}

static void snb_wm_latency_quirk(struct drm_i915_private *dev_priv)
{
	bool changed;

	/*
	 * The BIOS provided WM memory latency values are often
	 * inadequate for high resolution displays. Adjust them.
	 */
<<<<<<< HEAD
	changed = ilk_increase_wm_latency(dev_priv, dev_priv->wm.pri_latency, 12) |
		ilk_increase_wm_latency(dev_priv, dev_priv->wm.spr_latency, 12) |
		ilk_increase_wm_latency(dev_priv, dev_priv->wm.cur_latency, 12);
=======
	changed = ilk_increase_wm_latency(dev_priv, dev_priv->wm.pri_latency, 12);
	changed |= ilk_increase_wm_latency(dev_priv, dev_priv->wm.spr_latency, 12);
	changed |= ilk_increase_wm_latency(dev_priv, dev_priv->wm.cur_latency, 12);
>>>>>>> upstream/android-13

	if (!changed)
		return;

<<<<<<< HEAD
	DRM_DEBUG_KMS("WM latency values increased to avoid potential underruns\n");
=======
	drm_dbg_kms(&dev_priv->drm,
		    "WM latency values increased to avoid potential underruns\n");
>>>>>>> upstream/android-13
	intel_print_wm_latency(dev_priv, "Primary", dev_priv->wm.pri_latency);
	intel_print_wm_latency(dev_priv, "Sprite", dev_priv->wm.spr_latency);
	intel_print_wm_latency(dev_priv, "Cursor", dev_priv->wm.cur_latency);
}

static void snb_wm_lp3_irq_quirk(struct drm_i915_private *dev_priv)
{
	/*
	 * On some SNB machines (Thinkpad X220 Tablet at least)
	 * LP3 usage can cause vblank interrupts to be lost.
	 * The DEIIR bit will go high but it looks like the CPU
	 * never gets interrupted.
	 *
	 * It's not clear whether other interrupt source could
	 * be affected or if this is somehow limited to vblank
	 * interrupts only. To play it safe we disable LP3
	 * watermarks entirely.
	 */
	if (dev_priv->wm.pri_latency[3] == 0 &&
	    dev_priv->wm.spr_latency[3] == 0 &&
	    dev_priv->wm.cur_latency[3] == 0)
		return;

	dev_priv->wm.pri_latency[3] = 0;
	dev_priv->wm.spr_latency[3] = 0;
	dev_priv->wm.cur_latency[3] = 0;

<<<<<<< HEAD
	DRM_DEBUG_KMS("LP3 watermarks disabled due to potential for lost interrupts\n");
=======
	drm_dbg_kms(&dev_priv->drm,
		    "LP3 watermarks disabled due to potential for lost interrupts\n");
>>>>>>> upstream/android-13
	intel_print_wm_latency(dev_priv, "Primary", dev_priv->wm.pri_latency);
	intel_print_wm_latency(dev_priv, "Sprite", dev_priv->wm.spr_latency);
	intel_print_wm_latency(dev_priv, "Cursor", dev_priv->wm.cur_latency);
}

static void ilk_setup_wm_latency(struct drm_i915_private *dev_priv)
{
	intel_read_wm_latency(dev_priv, dev_priv->wm.pri_latency);

	memcpy(dev_priv->wm.spr_latency, dev_priv->wm.pri_latency,
	       sizeof(dev_priv->wm.pri_latency));
	memcpy(dev_priv->wm.cur_latency, dev_priv->wm.pri_latency,
	       sizeof(dev_priv->wm.pri_latency));

	intel_fixup_spr_wm_latency(dev_priv, dev_priv->wm.spr_latency);
	intel_fixup_cur_wm_latency(dev_priv, dev_priv->wm.cur_latency);

	intel_print_wm_latency(dev_priv, "Primary", dev_priv->wm.pri_latency);
	intel_print_wm_latency(dev_priv, "Sprite", dev_priv->wm.spr_latency);
	intel_print_wm_latency(dev_priv, "Cursor", dev_priv->wm.cur_latency);

<<<<<<< HEAD
	if (IS_GEN6(dev_priv)) {
=======
	if (DISPLAY_VER(dev_priv) == 6) {
>>>>>>> upstream/android-13
		snb_wm_latency_quirk(dev_priv);
		snb_wm_lp3_irq_quirk(dev_priv);
	}
}

static void skl_setup_wm_latency(struct drm_i915_private *dev_priv)
{
	intel_read_wm_latency(dev_priv, dev_priv->wm.skl_latency);
	intel_print_wm_latency(dev_priv, "Gen9 Plane", dev_priv->wm.skl_latency);
}

<<<<<<< HEAD
static bool ilk_validate_pipe_wm(struct drm_device *dev,
=======
static bool ilk_validate_pipe_wm(const struct drm_i915_private *dev_priv,
>>>>>>> upstream/android-13
				 struct intel_pipe_wm *pipe_wm)
{
	/* LP0 watermark maximums depend on this pipe alone */
	const struct intel_wm_config config = {
		.num_pipes_active = 1,
		.sprites_enabled = pipe_wm->sprites_enabled,
		.sprites_scaled = pipe_wm->sprites_scaled,
	};
	struct ilk_wm_maximums max;

	/* LP0 watermarks always use 1/2 DDB partitioning */
<<<<<<< HEAD
	ilk_compute_wm_maximums(dev, 0, &config, INTEL_DDB_PART_1_2, &max);

	/* At least LP0 must be valid */
	if (!ilk_validate_wm_level(0, &max, &pipe_wm->wm[0])) {
		DRM_DEBUG_KMS("LP0 watermark invalid\n");
=======
	ilk_compute_wm_maximums(dev_priv, 0, &config, INTEL_DDB_PART_1_2, &max);

	/* At least LP0 must be valid */
	if (!ilk_validate_wm_level(0, &max, &pipe_wm->wm[0])) {
		drm_dbg_kms(&dev_priv->drm, "LP0 watermark invalid\n");
>>>>>>> upstream/android-13
		return false;
	}

	return true;
}

/* Compute new watermarks for the pipe */
<<<<<<< HEAD
static int ilk_compute_pipe_wm(struct intel_crtc_state *cstate)
{
	struct drm_atomic_state *state = cstate->base.state;
	struct intel_crtc *intel_crtc = to_intel_crtc(cstate->base.crtc);
	struct intel_pipe_wm *pipe_wm;
	struct drm_device *dev = state->dev;
	const struct drm_i915_private *dev_priv = to_i915(dev);
	struct drm_plane *plane;
	const struct drm_plane_state *plane_state;
=======
static int ilk_compute_pipe_wm(struct intel_atomic_state *state,
			       struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(state->base.dev);
	struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
	struct intel_pipe_wm *pipe_wm;
	struct intel_plane *plane;
	const struct intel_plane_state *plane_state;
>>>>>>> upstream/android-13
	const struct intel_plane_state *pristate = NULL;
	const struct intel_plane_state *sprstate = NULL;
	const struct intel_plane_state *curstate = NULL;
	int level, max_level = ilk_wm_max_level(dev_priv), usable_level;
	struct ilk_wm_maximums max;

<<<<<<< HEAD
	pipe_wm = &cstate->wm.ilk.optimal;

	drm_atomic_crtc_state_for_each_plane_state(plane, plane_state, &cstate->base) {
		const struct intel_plane_state *ps = to_intel_plane_state(plane_state);

		if (plane->type == DRM_PLANE_TYPE_PRIMARY)
			pristate = ps;
		else if (plane->type == DRM_PLANE_TYPE_OVERLAY)
			sprstate = ps;
		else if (plane->type == DRM_PLANE_TYPE_CURSOR)
			curstate = ps;
	}

	pipe_wm->pipe_enabled = cstate->base.active;
	if (sprstate) {
		pipe_wm->sprites_enabled = sprstate->base.visible;
		pipe_wm->sprites_scaled = sprstate->base.visible &&
			(drm_rect_width(&sprstate->base.dst) != drm_rect_width(&sprstate->base.src) >> 16 ||
			 drm_rect_height(&sprstate->base.dst) != drm_rect_height(&sprstate->base.src) >> 16);
=======
	pipe_wm = &crtc_state->wm.ilk.optimal;

	intel_atomic_crtc_state_for_each_plane_state(plane, plane_state, crtc_state) {
		if (plane->base.type == DRM_PLANE_TYPE_PRIMARY)
			pristate = plane_state;
		else if (plane->base.type == DRM_PLANE_TYPE_OVERLAY)
			sprstate = plane_state;
		else if (plane->base.type == DRM_PLANE_TYPE_CURSOR)
			curstate = plane_state;
	}

	pipe_wm->pipe_enabled = crtc_state->hw.active;
	if (sprstate) {
		pipe_wm->sprites_enabled = sprstate->uapi.visible;
		pipe_wm->sprites_scaled = sprstate->uapi.visible &&
			(drm_rect_width(&sprstate->uapi.dst) != drm_rect_width(&sprstate->uapi.src) >> 16 ||
			 drm_rect_height(&sprstate->uapi.dst) != drm_rect_height(&sprstate->uapi.src) >> 16);
>>>>>>> upstream/android-13
	}

	usable_level = max_level;

	/* ILK/SNB: LP2+ watermarks only w/o sprites */
<<<<<<< HEAD
	if (INTEL_GEN(dev_priv) <= 6 && pipe_wm->sprites_enabled)
=======
	if (DISPLAY_VER(dev_priv) <= 6 && pipe_wm->sprites_enabled)
>>>>>>> upstream/android-13
		usable_level = 1;

	/* ILK/SNB/IVB: LP1+ watermarks only w/o scaling */
	if (pipe_wm->sprites_scaled)
		usable_level = 0;

	memset(&pipe_wm->wm, 0, sizeof(pipe_wm->wm));
<<<<<<< HEAD
	ilk_compute_wm_level(dev_priv, intel_crtc, 0, cstate,
			     pristate, sprstate, curstate, &pipe_wm->wm[0]);

	if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv))
		pipe_wm->linetime = hsw_compute_linetime_wm(cstate);

	if (!ilk_validate_pipe_wm(dev, pipe_wm))
=======
	ilk_compute_wm_level(dev_priv, crtc, 0, crtc_state,
			     pristate, sprstate, curstate, &pipe_wm->wm[0]);

	if (!ilk_validate_pipe_wm(dev_priv, pipe_wm))
>>>>>>> upstream/android-13
		return -EINVAL;

	ilk_compute_wm_reg_maximums(dev_priv, 1, &max);

	for (level = 1; level <= usable_level; level++) {
		struct intel_wm_level *wm = &pipe_wm->wm[level];

<<<<<<< HEAD
		ilk_compute_wm_level(dev_priv, intel_crtc, level, cstate,
=======
		ilk_compute_wm_level(dev_priv, crtc, level, crtc_state,
>>>>>>> upstream/android-13
				     pristate, sprstate, curstate, wm);

		/*
		 * Disable any watermark level that exceeds the
		 * register maximums since such watermarks are
		 * always invalid.
		 */
		if (!ilk_validate_wm_level(level, &max, wm)) {
			memset(wm, 0, sizeof(*wm));
			break;
		}
	}

	return 0;
}

/*
 * Build a set of 'intermediate' watermark values that satisfy both the old
 * state and the new state.  These can be programmed to the hardware
 * immediately.
 */
<<<<<<< HEAD
static int ilk_compute_intermediate_wm(struct drm_device *dev,
				       struct intel_crtc *intel_crtc,
				       struct intel_crtc_state *newstate)
{
	struct intel_pipe_wm *a = &newstate->wm.ilk.intermediate;
	struct intel_atomic_state *intel_state =
		to_intel_atomic_state(newstate->base.state);
	const struct intel_crtc_state *oldstate =
		intel_atomic_get_old_crtc_state(intel_state, intel_crtc);
	const struct intel_pipe_wm *b = &oldstate->wm.ilk.optimal;
	int level, max_level = ilk_wm_max_level(to_i915(dev));
=======
static int ilk_compute_intermediate_wm(struct intel_atomic_state *state,
				       struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	struct intel_crtc_state *new_crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
	const struct intel_crtc_state *old_crtc_state =
		intel_atomic_get_old_crtc_state(state, crtc);
	struct intel_pipe_wm *a = &new_crtc_state->wm.ilk.intermediate;
	const struct intel_pipe_wm *b = &old_crtc_state->wm.ilk.optimal;
	int level, max_level = ilk_wm_max_level(dev_priv);
>>>>>>> upstream/android-13

	/*
	 * Start with the final, target watermarks, then combine with the
	 * currently active watermarks to get values that are safe both before
	 * and after the vblank.
	 */
<<<<<<< HEAD
	*a = newstate->wm.ilk.optimal;
	if (!newstate->base.active || drm_atomic_crtc_needs_modeset(&newstate->base))
=======
	*a = new_crtc_state->wm.ilk.optimal;
	if (!new_crtc_state->hw.active ||
	    drm_atomic_crtc_needs_modeset(&new_crtc_state->uapi) ||
	    state->skip_intermediate_wm)
>>>>>>> upstream/android-13
		return 0;

	a->pipe_enabled |= b->pipe_enabled;
	a->sprites_enabled |= b->sprites_enabled;
	a->sprites_scaled |= b->sprites_scaled;

	for (level = 0; level <= max_level; level++) {
		struct intel_wm_level *a_wm = &a->wm[level];
		const struct intel_wm_level *b_wm = &b->wm[level];

		a_wm->enable &= b_wm->enable;
		a_wm->pri_val = max(a_wm->pri_val, b_wm->pri_val);
		a_wm->spr_val = max(a_wm->spr_val, b_wm->spr_val);
		a_wm->cur_val = max(a_wm->cur_val, b_wm->cur_val);
		a_wm->fbc_val = max(a_wm->fbc_val, b_wm->fbc_val);
	}

	/*
	 * We need to make sure that these merged watermark values are
	 * actually a valid configuration themselves.  If they're not,
	 * there's no safe way to transition from the old state to
	 * the new state, so we need to fail the atomic transaction.
	 */
<<<<<<< HEAD
	if (!ilk_validate_pipe_wm(dev, a))
=======
	if (!ilk_validate_pipe_wm(dev_priv, a))
>>>>>>> upstream/android-13
		return -EINVAL;

	/*
	 * If our intermediate WM are identical to the final WM, then we can
	 * omit the post-vblank programming; only update if it's different.
	 */
<<<<<<< HEAD
	if (memcmp(a, &newstate->wm.ilk.optimal, sizeof(*a)) != 0)
		newstate->wm.need_postvbl_update = true;
=======
	if (memcmp(a, &new_crtc_state->wm.ilk.optimal, sizeof(*a)) != 0)
		new_crtc_state->wm.need_postvbl_update = true;
>>>>>>> upstream/android-13

	return 0;
}

/*
 * Merge the watermarks from all active pipes for a specific level.
 */
<<<<<<< HEAD
static void ilk_merge_wm_level(struct drm_device *dev,
			       int level,
			       struct intel_wm_level *ret_wm)
{
	const struct intel_crtc *intel_crtc;

	ret_wm->enable = true;

	for_each_intel_crtc(dev, intel_crtc) {
		const struct intel_pipe_wm *active = &intel_crtc->wm.active.ilk;
=======
static void ilk_merge_wm_level(struct drm_i915_private *dev_priv,
			       int level,
			       struct intel_wm_level *ret_wm)
{
	const struct intel_crtc *crtc;

	ret_wm->enable = true;

	for_each_intel_crtc(&dev_priv->drm, crtc) {
		const struct intel_pipe_wm *active = &crtc->wm.active.ilk;
>>>>>>> upstream/android-13
		const struct intel_wm_level *wm = &active->wm[level];

		if (!active->pipe_enabled)
			continue;

		/*
		 * The watermark values may have been used in the past,
		 * so we must maintain them in the registers for some
		 * time even if the level is now disabled.
		 */
		if (!wm->enable)
			ret_wm->enable = false;

		ret_wm->pri_val = max(ret_wm->pri_val, wm->pri_val);
		ret_wm->spr_val = max(ret_wm->spr_val, wm->spr_val);
		ret_wm->cur_val = max(ret_wm->cur_val, wm->cur_val);
		ret_wm->fbc_val = max(ret_wm->fbc_val, wm->fbc_val);
	}
}

/*
 * Merge all low power watermarks for all active pipes.
 */
<<<<<<< HEAD
static void ilk_wm_merge(struct drm_device *dev,
=======
static void ilk_wm_merge(struct drm_i915_private *dev_priv,
>>>>>>> upstream/android-13
			 const struct intel_wm_config *config,
			 const struct ilk_wm_maximums *max,
			 struct intel_pipe_wm *merged)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = to_i915(dev);
=======
>>>>>>> upstream/android-13
	int level, max_level = ilk_wm_max_level(dev_priv);
	int last_enabled_level = max_level;

	/* ILK/SNB/IVB: LP1+ watermarks only w/ single pipe */
<<<<<<< HEAD
	if ((INTEL_GEN(dev_priv) <= 6 || IS_IVYBRIDGE(dev_priv)) &&
=======
	if ((DISPLAY_VER(dev_priv) <= 6 || IS_IVYBRIDGE(dev_priv)) &&
>>>>>>> upstream/android-13
	    config->num_pipes_active > 1)
		last_enabled_level = 0;

	/* ILK: FBC WM must be disabled always */
<<<<<<< HEAD
	merged->fbc_wm_enabled = INTEL_GEN(dev_priv) >= 6;
=======
	merged->fbc_wm_enabled = DISPLAY_VER(dev_priv) >= 6;
>>>>>>> upstream/android-13

	/* merge each WM1+ level */
	for (level = 1; level <= max_level; level++) {
		struct intel_wm_level *wm = &merged->wm[level];

<<<<<<< HEAD
		ilk_merge_wm_level(dev, level, wm);
=======
		ilk_merge_wm_level(dev_priv, level, wm);
>>>>>>> upstream/android-13

		if (level > last_enabled_level)
			wm->enable = false;
		else if (!ilk_validate_wm_level(level, max, wm))
			/* make sure all following levels get disabled */
			last_enabled_level = level - 1;

		/*
		 * The spec says it is preferred to disable
		 * FBC WMs instead of disabling a WM level.
		 */
		if (wm->fbc_val > max->fbc) {
			if (wm->enable)
				merged->fbc_wm_enabled = false;
			wm->fbc_val = 0;
		}
	}

	/* ILK: LP2+ must be disabled when FBC WM is disabled but FBC enabled */
	/*
	 * FIXME this is racy. FBC might get enabled later.
	 * What we should check here is whether FBC can be
	 * enabled sometime later.
	 */
<<<<<<< HEAD
	if (IS_GEN5(dev_priv) && !merged->fbc_wm_enabled &&
=======
	if (DISPLAY_VER(dev_priv) == 5 && !merged->fbc_wm_enabled &&
>>>>>>> upstream/android-13
	    intel_fbc_is_active(dev_priv)) {
		for (level = 2; level <= max_level; level++) {
			struct intel_wm_level *wm = &merged->wm[level];

			wm->enable = false;
		}
	}
}

static int ilk_wm_lp_to_level(int wm_lp, const struct intel_pipe_wm *pipe_wm)
{
	/* LP1,LP2,LP3 levels are either 1,2,3 or 1,3,4 */
	return wm_lp + (wm_lp >= 2 && pipe_wm->wm[4].enable);
}

/* The value we need to program into the WM_LPx latency field */
<<<<<<< HEAD
static unsigned int ilk_wm_lp_latency(struct drm_device *dev, int level)
{
	struct drm_i915_private *dev_priv = to_i915(dev);

=======
static unsigned int ilk_wm_lp_latency(struct drm_i915_private *dev_priv,
				      int level)
{
>>>>>>> upstream/android-13
	if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv))
		return 2 * level;
	else
		return dev_priv->wm.pri_latency[level];
}

<<<<<<< HEAD
static void ilk_compute_wm_results(struct drm_device *dev,
=======
static void ilk_compute_wm_results(struct drm_i915_private *dev_priv,
>>>>>>> upstream/android-13
				   const struct intel_pipe_wm *merged,
				   enum intel_ddb_partitioning partitioning,
				   struct ilk_wm_values *results)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct intel_crtc *intel_crtc;
=======
	struct intel_crtc *crtc;
>>>>>>> upstream/android-13
	int level, wm_lp;

	results->enable_fbc_wm = merged->fbc_wm_enabled;
	results->partitioning = partitioning;

	/* LP1+ register values */
	for (wm_lp = 1; wm_lp <= 3; wm_lp++) {
		const struct intel_wm_level *r;

		level = ilk_wm_lp_to_level(wm_lp, merged);

		r = &merged->wm[level];

		/*
		 * Maintain the watermark values even if the level is
		 * disabled. Doing otherwise could cause underruns.
		 */
		results->wm_lp[wm_lp - 1] =
<<<<<<< HEAD
			(ilk_wm_lp_latency(dev, level) << WM1_LP_LATENCY_SHIFT) |
=======
			(ilk_wm_lp_latency(dev_priv, level) << WM1_LP_LATENCY_SHIFT) |
>>>>>>> upstream/android-13
			(r->pri_val << WM1_LP_SR_SHIFT) |
			r->cur_val;

		if (r->enable)
			results->wm_lp[wm_lp - 1] |= WM1_LP_SR_EN;

<<<<<<< HEAD
		if (INTEL_GEN(dev_priv) >= 8)
=======
		if (DISPLAY_VER(dev_priv) >= 8)
>>>>>>> upstream/android-13
			results->wm_lp[wm_lp - 1] |=
				r->fbc_val << WM1_LP_FBC_SHIFT_BDW;
		else
			results->wm_lp[wm_lp - 1] |=
				r->fbc_val << WM1_LP_FBC_SHIFT;

		/*
		 * Always set WM1S_LP_EN when spr_val != 0, even if the
		 * level is disabled. Doing otherwise could cause underruns.
		 */
<<<<<<< HEAD
		if (INTEL_GEN(dev_priv) <= 6 && r->spr_val) {
			WARN_ON(wm_lp != 1);
=======
		if (DISPLAY_VER(dev_priv) <= 6 && r->spr_val) {
			drm_WARN_ON(&dev_priv->drm, wm_lp != 1);
>>>>>>> upstream/android-13
			results->wm_lp_spr[wm_lp - 1] = WM1S_LP_EN | r->spr_val;
		} else
			results->wm_lp_spr[wm_lp - 1] = r->spr_val;
	}

	/* LP0 register values */
<<<<<<< HEAD
	for_each_intel_crtc(dev, intel_crtc) {
		enum pipe pipe = intel_crtc->pipe;
		const struct intel_wm_level *r =
			&intel_crtc->wm.active.ilk.wm[0];

		if (WARN_ON(!r->enable))
			continue;

		results->wm_linetime[pipe] = intel_crtc->wm.active.ilk.linetime;

=======
	for_each_intel_crtc(&dev_priv->drm, crtc) {
		enum pipe pipe = crtc->pipe;
		const struct intel_pipe_wm *pipe_wm = &crtc->wm.active.ilk;
		const struct intel_wm_level *r = &pipe_wm->wm[0];

		if (drm_WARN_ON(&dev_priv->drm, !r->enable))
			continue;

>>>>>>> upstream/android-13
		results->wm_pipe[pipe] =
			(r->pri_val << WM0_PIPE_PLANE_SHIFT) |
			(r->spr_val << WM0_PIPE_SPRITE_SHIFT) |
			r->cur_val;
	}
}

/* Find the result with the highest level enabled. Check for enable_fbc_wm in
 * case both are at the same level. Prefer r1 in case they're the same. */
<<<<<<< HEAD
static struct intel_pipe_wm *ilk_find_best_result(struct drm_device *dev,
						  struct intel_pipe_wm *r1,
						  struct intel_pipe_wm *r2)
{
	int level, max_level = ilk_wm_max_level(to_i915(dev));
=======
static struct intel_pipe_wm *
ilk_find_best_result(struct drm_i915_private *dev_priv,
		     struct intel_pipe_wm *r1,
		     struct intel_pipe_wm *r2)
{
	int level, max_level = ilk_wm_max_level(dev_priv);
>>>>>>> upstream/android-13
	int level1 = 0, level2 = 0;

	for (level = 1; level <= max_level; level++) {
		if (r1->wm[level].enable)
			level1 = level;
		if (r2->wm[level].enable)
			level2 = level;
	}

	if (level1 == level2) {
		if (r2->fbc_wm_enabled && !r1->fbc_wm_enabled)
			return r2;
		else
			return r1;
	} else if (level1 > level2) {
		return r1;
	} else {
		return r2;
	}
}

/* dirty bits used to track which watermarks need changes */
#define WM_DIRTY_PIPE(pipe) (1 << (pipe))
<<<<<<< HEAD
#define WM_DIRTY_LINETIME(pipe) (1 << (8 + (pipe)))
=======
>>>>>>> upstream/android-13
#define WM_DIRTY_LP(wm_lp) (1 << (15 + (wm_lp)))
#define WM_DIRTY_LP_ALL (WM_DIRTY_LP(1) | WM_DIRTY_LP(2) | WM_DIRTY_LP(3))
#define WM_DIRTY_FBC (1 << 24)
#define WM_DIRTY_DDB (1 << 25)

static unsigned int ilk_compute_wm_dirty(struct drm_i915_private *dev_priv,
					 const struct ilk_wm_values *old,
					 const struct ilk_wm_values *new)
{
	unsigned int dirty = 0;
	enum pipe pipe;
	int wm_lp;

	for_each_pipe(dev_priv, pipe) {
<<<<<<< HEAD
		if (old->wm_linetime[pipe] != new->wm_linetime[pipe]) {
			dirty |= WM_DIRTY_LINETIME(pipe);
			/* Must disable LP1+ watermarks too */
			dirty |= WM_DIRTY_LP_ALL;
		}

=======
>>>>>>> upstream/android-13
		if (old->wm_pipe[pipe] != new->wm_pipe[pipe]) {
			dirty |= WM_DIRTY_PIPE(pipe);
			/* Must disable LP1+ watermarks too */
			dirty |= WM_DIRTY_LP_ALL;
		}
	}

	if (old->enable_fbc_wm != new->enable_fbc_wm) {
		dirty |= WM_DIRTY_FBC;
		/* Must disable LP1+ watermarks too */
		dirty |= WM_DIRTY_LP_ALL;
	}

	if (old->partitioning != new->partitioning) {
		dirty |= WM_DIRTY_DDB;
		/* Must disable LP1+ watermarks too */
		dirty |= WM_DIRTY_LP_ALL;
	}

	/* LP1+ watermarks already deemed dirty, no need to continue */
	if (dirty & WM_DIRTY_LP_ALL)
		return dirty;

	/* Find the lowest numbered LP1+ watermark in need of an update... */
	for (wm_lp = 1; wm_lp <= 3; wm_lp++) {
		if (old->wm_lp[wm_lp - 1] != new->wm_lp[wm_lp - 1] ||
		    old->wm_lp_spr[wm_lp - 1] != new->wm_lp_spr[wm_lp - 1])
			break;
	}

	/* ...and mark it and all higher numbered LP1+ watermarks as dirty */
	for (; wm_lp <= 3; wm_lp++)
		dirty |= WM_DIRTY_LP(wm_lp);

	return dirty;
}

static bool _ilk_disable_lp_wm(struct drm_i915_private *dev_priv,
			       unsigned int dirty)
{
	struct ilk_wm_values *previous = &dev_priv->wm.hw;
	bool changed = false;

	if (dirty & WM_DIRTY_LP(3) && previous->wm_lp[2] & WM1_LP_SR_EN) {
		previous->wm_lp[2] &= ~WM1_LP_SR_EN;
<<<<<<< HEAD
		I915_WRITE(WM3_LP_ILK, previous->wm_lp[2]);
=======
		intel_uncore_write(&dev_priv->uncore, WM3_LP_ILK, previous->wm_lp[2]);
>>>>>>> upstream/android-13
		changed = true;
	}
	if (dirty & WM_DIRTY_LP(2) && previous->wm_lp[1] & WM1_LP_SR_EN) {
		previous->wm_lp[1] &= ~WM1_LP_SR_EN;
<<<<<<< HEAD
		I915_WRITE(WM2_LP_ILK, previous->wm_lp[1]);
=======
		intel_uncore_write(&dev_priv->uncore, WM2_LP_ILK, previous->wm_lp[1]);
>>>>>>> upstream/android-13
		changed = true;
	}
	if (dirty & WM_DIRTY_LP(1) && previous->wm_lp[0] & WM1_LP_SR_EN) {
		previous->wm_lp[0] &= ~WM1_LP_SR_EN;
<<<<<<< HEAD
		I915_WRITE(WM1_LP_ILK, previous->wm_lp[0]);
=======
		intel_uncore_write(&dev_priv->uncore, WM1_LP_ILK, previous->wm_lp[0]);
>>>>>>> upstream/android-13
		changed = true;
	}

	/*
	 * Don't touch WM1S_LP_EN here.
	 * Doing so could cause underruns.
	 */

	return changed;
}

/*
 * The spec says we shouldn't write when we don't need, because every write
 * causes WMs to be re-evaluated, expending some power.
 */
static void ilk_write_wm_values(struct drm_i915_private *dev_priv,
				struct ilk_wm_values *results)
{
	struct ilk_wm_values *previous = &dev_priv->wm.hw;
	unsigned int dirty;
<<<<<<< HEAD
	uint32_t val;
=======
	u32 val;
>>>>>>> upstream/android-13

	dirty = ilk_compute_wm_dirty(dev_priv, previous, results);
	if (!dirty)
		return;

	_ilk_disable_lp_wm(dev_priv, dirty);

	if (dirty & WM_DIRTY_PIPE(PIPE_A))
<<<<<<< HEAD
		I915_WRITE(WM0_PIPEA_ILK, results->wm_pipe[0]);
	if (dirty & WM_DIRTY_PIPE(PIPE_B))
		I915_WRITE(WM0_PIPEB_ILK, results->wm_pipe[1]);
	if (dirty & WM_DIRTY_PIPE(PIPE_C))
		I915_WRITE(WM0_PIPEC_IVB, results->wm_pipe[2]);

	if (dirty & WM_DIRTY_LINETIME(PIPE_A))
		I915_WRITE(PIPE_WM_LINETIME(PIPE_A), results->wm_linetime[0]);
	if (dirty & WM_DIRTY_LINETIME(PIPE_B))
		I915_WRITE(PIPE_WM_LINETIME(PIPE_B), results->wm_linetime[1]);
	if (dirty & WM_DIRTY_LINETIME(PIPE_C))
		I915_WRITE(PIPE_WM_LINETIME(PIPE_C), results->wm_linetime[2]);

	if (dirty & WM_DIRTY_DDB) {
		if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv)) {
			val = I915_READ(WM_MISC);
=======
		intel_uncore_write(&dev_priv->uncore, WM0_PIPE_ILK(PIPE_A), results->wm_pipe[0]);
	if (dirty & WM_DIRTY_PIPE(PIPE_B))
		intel_uncore_write(&dev_priv->uncore, WM0_PIPE_ILK(PIPE_B), results->wm_pipe[1]);
	if (dirty & WM_DIRTY_PIPE(PIPE_C))
		intel_uncore_write(&dev_priv->uncore, WM0_PIPE_ILK(PIPE_C), results->wm_pipe[2]);

	if (dirty & WM_DIRTY_DDB) {
		if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv)) {
			val = intel_uncore_read(&dev_priv->uncore, WM_MISC);
>>>>>>> upstream/android-13
			if (results->partitioning == INTEL_DDB_PART_1_2)
				val &= ~WM_MISC_DATA_PARTITION_5_6;
			else
				val |= WM_MISC_DATA_PARTITION_5_6;
<<<<<<< HEAD
			I915_WRITE(WM_MISC, val);
		} else {
			val = I915_READ(DISP_ARB_CTL2);
=======
			intel_uncore_write(&dev_priv->uncore, WM_MISC, val);
		} else {
			val = intel_uncore_read(&dev_priv->uncore, DISP_ARB_CTL2);
>>>>>>> upstream/android-13
			if (results->partitioning == INTEL_DDB_PART_1_2)
				val &= ~DISP_DATA_PARTITION_5_6;
			else
				val |= DISP_DATA_PARTITION_5_6;
<<<<<<< HEAD
			I915_WRITE(DISP_ARB_CTL2, val);
=======
			intel_uncore_write(&dev_priv->uncore, DISP_ARB_CTL2, val);
>>>>>>> upstream/android-13
		}
	}

	if (dirty & WM_DIRTY_FBC) {
<<<<<<< HEAD
		val = I915_READ(DISP_ARB_CTL);
=======
		val = intel_uncore_read(&dev_priv->uncore, DISP_ARB_CTL);
>>>>>>> upstream/android-13
		if (results->enable_fbc_wm)
			val &= ~DISP_FBC_WM_DIS;
		else
			val |= DISP_FBC_WM_DIS;
<<<<<<< HEAD
		I915_WRITE(DISP_ARB_CTL, val);
=======
		intel_uncore_write(&dev_priv->uncore, DISP_ARB_CTL, val);
>>>>>>> upstream/android-13
	}

	if (dirty & WM_DIRTY_LP(1) &&
	    previous->wm_lp_spr[0] != results->wm_lp_spr[0])
<<<<<<< HEAD
		I915_WRITE(WM1S_LP_ILK, results->wm_lp_spr[0]);

	if (INTEL_GEN(dev_priv) >= 7) {
		if (dirty & WM_DIRTY_LP(2) && previous->wm_lp_spr[1] != results->wm_lp_spr[1])
			I915_WRITE(WM2S_LP_IVB, results->wm_lp_spr[1]);
		if (dirty & WM_DIRTY_LP(3) && previous->wm_lp_spr[2] != results->wm_lp_spr[2])
			I915_WRITE(WM3S_LP_IVB, results->wm_lp_spr[2]);
	}

	if (dirty & WM_DIRTY_LP(1) && previous->wm_lp[0] != results->wm_lp[0])
		I915_WRITE(WM1_LP_ILK, results->wm_lp[0]);
	if (dirty & WM_DIRTY_LP(2) && previous->wm_lp[1] != results->wm_lp[1])
		I915_WRITE(WM2_LP_ILK, results->wm_lp[1]);
	if (dirty & WM_DIRTY_LP(3) && previous->wm_lp[2] != results->wm_lp[2])
		I915_WRITE(WM3_LP_ILK, results->wm_lp[2]);
=======
		intel_uncore_write(&dev_priv->uncore, WM1S_LP_ILK, results->wm_lp_spr[0]);

	if (DISPLAY_VER(dev_priv) >= 7) {
		if (dirty & WM_DIRTY_LP(2) && previous->wm_lp_spr[1] != results->wm_lp_spr[1])
			intel_uncore_write(&dev_priv->uncore, WM2S_LP_IVB, results->wm_lp_spr[1]);
		if (dirty & WM_DIRTY_LP(3) && previous->wm_lp_spr[2] != results->wm_lp_spr[2])
			intel_uncore_write(&dev_priv->uncore, WM3S_LP_IVB, results->wm_lp_spr[2]);
	}

	if (dirty & WM_DIRTY_LP(1) && previous->wm_lp[0] != results->wm_lp[0])
		intel_uncore_write(&dev_priv->uncore, WM1_LP_ILK, results->wm_lp[0]);
	if (dirty & WM_DIRTY_LP(2) && previous->wm_lp[1] != results->wm_lp[1])
		intel_uncore_write(&dev_priv->uncore, WM2_LP_ILK, results->wm_lp[1]);
	if (dirty & WM_DIRTY_LP(3) && previous->wm_lp[2] != results->wm_lp[2])
		intel_uncore_write(&dev_priv->uncore, WM3_LP_ILK, results->wm_lp[2]);
>>>>>>> upstream/android-13

	dev_priv->wm.hw = *results;
}

<<<<<<< HEAD
bool ilk_disable_lp_wm(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = to_i915(dev);

	return _ilk_disable_lp_wm(dev_priv, WM_DIRTY_LP_ALL);
}

static u8 intel_enabled_dbuf_slices_num(struct drm_i915_private *dev_priv)
{
	u8 enabled_slices;

	/* Slice 1 will always be enabled */
	enabled_slices = 1;

	/* Gen prior to GEN11 have only one DBuf slice */
	if (INTEL_GEN(dev_priv) < 11)
		return enabled_slices;

	if (I915_READ(DBUF_CTL_S2) & DBUF_POWER_STATE)
		enabled_slices++;
=======
bool ilk_disable_lp_wm(struct drm_i915_private *dev_priv)
{
	return _ilk_disable_lp_wm(dev_priv, WM_DIRTY_LP_ALL);
}

u8 intel_enabled_dbuf_slices_mask(struct drm_i915_private *dev_priv)
{
	u8 enabled_slices = 0;
	enum dbuf_slice slice;

	for_each_dbuf_slice(dev_priv, slice) {
		if (intel_uncore_read(&dev_priv->uncore,
				      DBUF_CTL_S(slice)) & DBUF_POWER_STATE)
			enabled_slices |= BIT(slice);
	}
>>>>>>> upstream/android-13

	return enabled_slices;
}

/*
 * FIXME: We still don't have the proper code detect if we need to apply the WA,
 * so assume we'll always need it in order to avoid underruns.
 */
<<<<<<< HEAD
static bool skl_needs_memory_bw_wa(struct intel_atomic_state *state)
{
	struct drm_i915_private *dev_priv = to_i915(state->base.dev);

	if (IS_GEN9_BC(dev_priv) || IS_BROXTON(dev_priv))
		return true;

	return false;
=======
static bool skl_needs_memory_bw_wa(struct drm_i915_private *dev_priv)
{
	return DISPLAY_VER(dev_priv) == 9;
>>>>>>> upstream/android-13
}

static bool
intel_has_sagv(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	if (IS_KABYLAKE(dev_priv) || IS_COFFEELAKE(dev_priv) ||
	    IS_CANNONLAKE(dev_priv))
		return true;

	if (IS_SKYLAKE(dev_priv) &&
	    dev_priv->sagv_status != I915_SAGV_NOT_CONTROLLED)
		return true;

	return false;
=======
	return DISPLAY_VER(dev_priv) >= 9 && !IS_LP(dev_priv) &&
		dev_priv->sagv_status != I915_SAGV_NOT_CONTROLLED;
}

static void
skl_setup_sagv_block_time(struct drm_i915_private *dev_priv)
{
	if (DISPLAY_VER(dev_priv) >= 12) {
		u32 val = 0;
		int ret;

		ret = sandybridge_pcode_read(dev_priv,
					     GEN12_PCODE_READ_SAGV_BLOCK_TIME_US,
					     &val, NULL);
		if (!ret) {
			dev_priv->sagv_block_time_us = val;
			return;
		}

		drm_dbg(&dev_priv->drm, "Couldn't read SAGV block time!\n");
	} else if (DISPLAY_VER(dev_priv) == 11) {
		dev_priv->sagv_block_time_us = 10;
		return;
	} else if (DISPLAY_VER(dev_priv) == 10) {
		dev_priv->sagv_block_time_us = 20;
		return;
	} else if (DISPLAY_VER(dev_priv) == 9) {
		dev_priv->sagv_block_time_us = 30;
		return;
	} else {
		MISSING_CASE(DISPLAY_VER(dev_priv));
	}

	dev_priv->sagv_block_time_us = 0;
>>>>>>> upstream/android-13
}

/*
 * SAGV dynamically adjusts the system agent voltage and clock frequencies
 * depending on power and performance requirements. The display engine access
 * to system memory is blocked during the adjustment time. Because of the
 * blocking time, having this enabled can cause full system hangs and/or pipe
 * underruns if we don't meet all of the following requirements:
 *
 *  - <= 1 pipe enabled
 *  - All planes can enable watermarks for latencies >= SAGV engine block time
 *  - We're not using an interlaced display configuration
 */
<<<<<<< HEAD
int
=======
static int
>>>>>>> upstream/android-13
intel_enable_sagv(struct drm_i915_private *dev_priv)
{
	int ret;

	if (!intel_has_sagv(dev_priv))
		return 0;

	if (dev_priv->sagv_status == I915_SAGV_ENABLED)
		return 0;

<<<<<<< HEAD
	DRM_DEBUG_KMS("Enabling the SAGV\n");
	mutex_lock(&dev_priv->pcu_lock);

	ret = sandybridge_pcode_write(dev_priv, GEN9_PCODE_SAGV_CONTROL,
				      GEN9_SAGV_ENABLE);

	/* We don't need to wait for the SAGV when enabling */
	mutex_unlock(&dev_priv->pcu_lock);

	/*
	 * Some skl systems, pre-release machines in particular,
	 * don't actually have an SAGV.
	 */
	if (IS_SKYLAKE(dev_priv) && ret == -ENXIO) {
		DRM_DEBUG_DRIVER("No SAGV found on system, ignoring\n");
		dev_priv->sagv_status = I915_SAGV_NOT_CONTROLLED;
		return 0;
	} else if (ret < 0) {
		DRM_ERROR("Failed to enable the SAGV\n");
=======
	drm_dbg_kms(&dev_priv->drm, "Enabling SAGV\n");
	ret = sandybridge_pcode_write(dev_priv, GEN9_PCODE_SAGV_CONTROL,
				      GEN9_SAGV_ENABLE);

	/* We don't need to wait for SAGV when enabling */

	/*
	 * Some skl systems, pre-release machines in particular,
	 * don't actually have SAGV.
	 */
	if (IS_SKYLAKE(dev_priv) && ret == -ENXIO) {
		drm_dbg(&dev_priv->drm, "No SAGV found on system, ignoring\n");
		dev_priv->sagv_status = I915_SAGV_NOT_CONTROLLED;
		return 0;
	} else if (ret < 0) {
		drm_err(&dev_priv->drm, "Failed to enable SAGV\n");
>>>>>>> upstream/android-13
		return ret;
	}

	dev_priv->sagv_status = I915_SAGV_ENABLED;
	return 0;
}

<<<<<<< HEAD
int
=======
static int
>>>>>>> upstream/android-13
intel_disable_sagv(struct drm_i915_private *dev_priv)
{
	int ret;

	if (!intel_has_sagv(dev_priv))
		return 0;

	if (dev_priv->sagv_status == I915_SAGV_DISABLED)
		return 0;

<<<<<<< HEAD
	DRM_DEBUG_KMS("Disabling the SAGV\n");
	mutex_lock(&dev_priv->pcu_lock);

=======
	drm_dbg_kms(&dev_priv->drm, "Disabling SAGV\n");
>>>>>>> upstream/android-13
	/* bspec says to keep retrying for at least 1 ms */
	ret = skl_pcode_request(dev_priv, GEN9_PCODE_SAGV_CONTROL,
				GEN9_SAGV_DISABLE,
				GEN9_SAGV_IS_DISABLED, GEN9_SAGV_IS_DISABLED,
				1);
<<<<<<< HEAD
	mutex_unlock(&dev_priv->pcu_lock);

	/*
	 * Some skl systems, pre-release machines in particular,
	 * don't actually have an SAGV.
	 */
	if (IS_SKYLAKE(dev_priv) && ret == -ENXIO) {
		DRM_DEBUG_DRIVER("No SAGV found on system, ignoring\n");
		dev_priv->sagv_status = I915_SAGV_NOT_CONTROLLED;
		return 0;
	} else if (ret < 0) {
		DRM_ERROR("Failed to disable the SAGV (%d)\n", ret);
=======
	/*
	 * Some skl systems, pre-release machines in particular,
	 * don't actually have SAGV.
	 */
	if (IS_SKYLAKE(dev_priv) && ret == -ENXIO) {
		drm_dbg(&dev_priv->drm, "No SAGV found on system, ignoring\n");
		dev_priv->sagv_status = I915_SAGV_NOT_CONTROLLED;
		return 0;
	} else if (ret < 0) {
		drm_err(&dev_priv->drm, "Failed to disable SAGV (%d)\n", ret);
>>>>>>> upstream/android-13
		return ret;
	}

	dev_priv->sagv_status = I915_SAGV_DISABLED;
	return 0;
}

<<<<<<< HEAD
bool intel_can_enable_sagv(struct drm_atomic_state *state)
{
	struct drm_device *dev = state->dev;
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct intel_atomic_state *intel_state = to_intel_atomic_state(state);
	struct intel_crtc *crtc;
	struct intel_plane *plane;
	struct intel_crtc_state *cstate;
	enum pipe pipe;
	int level, latency;
	int sagv_block_time_us;
=======
void intel_sagv_pre_plane_update(struct intel_atomic_state *state)
{
	struct drm_i915_private *dev_priv = to_i915(state->base.dev);
	const struct intel_bw_state *new_bw_state;
	const struct intel_bw_state *old_bw_state;
	u32 new_mask = 0;

	/*
	 * Just return if we can't control SAGV or don't have it.
	 * This is different from situation when we have SAGV but just can't
	 * afford it due to DBuf limitation - in case if SAGV is completely
	 * disabled in a BIOS, we are not even allowed to send a PCode request,
	 * as it will throw an error. So have to check it here.
	 */
	if (!intel_has_sagv(dev_priv))
		return;

	new_bw_state = intel_atomic_get_new_bw_state(state);
	if (!new_bw_state)
		return;

	if (DISPLAY_VER(dev_priv) < 11 && !intel_can_enable_sagv(dev_priv, new_bw_state)) {
		intel_disable_sagv(dev_priv);
		return;
	}

	old_bw_state = intel_atomic_get_old_bw_state(state);
	/*
	 * Nothing to mask
	 */
	if (new_bw_state->qgv_points_mask == old_bw_state->qgv_points_mask)
		return;

	new_mask = old_bw_state->qgv_points_mask | new_bw_state->qgv_points_mask;

	/*
	 * If new mask is zero - means there is nothing to mask,
	 * we can only unmask, which should be done in unmask.
	 */
	if (!new_mask)
		return;

	/*
	 * Restrict required qgv points before updating the configuration.
	 * According to BSpec we can't mask and unmask qgv points at the same
	 * time. Also masking should be done before updating the configuration
	 * and unmasking afterwards.
	 */
	icl_pcode_restrict_qgv_points(dev_priv, new_mask);
}

void intel_sagv_post_plane_update(struct intel_atomic_state *state)
{
	struct drm_i915_private *dev_priv = to_i915(state->base.dev);
	const struct intel_bw_state *new_bw_state;
	const struct intel_bw_state *old_bw_state;
	u32 new_mask = 0;

	/*
	 * Just return if we can't control SAGV or don't have it.
	 * This is different from situation when we have SAGV but just can't
	 * afford it due to DBuf limitation - in case if SAGV is completely
	 * disabled in a BIOS, we are not even allowed to send a PCode request,
	 * as it will throw an error. So have to check it here.
	 */
	if (!intel_has_sagv(dev_priv))
		return;

	new_bw_state = intel_atomic_get_new_bw_state(state);
	if (!new_bw_state)
		return;

	if (DISPLAY_VER(dev_priv) < 11 && intel_can_enable_sagv(dev_priv, new_bw_state)) {
		intel_enable_sagv(dev_priv);
		return;
	}

	old_bw_state = intel_atomic_get_old_bw_state(state);
	/*
	 * Nothing to unmask
	 */
	if (new_bw_state->qgv_points_mask == old_bw_state->qgv_points_mask)
		return;

	new_mask = new_bw_state->qgv_points_mask;

	/*
	 * Allow required qgv points after updating the configuration.
	 * According to BSpec we can't mask and unmask qgv points at the same
	 * time. Also masking should be done before updating the configuration
	 * and unmasking afterwards.
	 */
	icl_pcode_restrict_qgv_points(dev_priv, new_mask);
}

static bool skl_crtc_can_enable_sagv(const struct intel_crtc_state *crtc_state)
{
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->uapi.crtc);
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	enum plane_id plane_id;
	int max_level = INT_MAX;
>>>>>>> upstream/android-13

	if (!intel_has_sagv(dev_priv))
		return false;

<<<<<<< HEAD
	if (IS_GEN9(dev_priv))
		sagv_block_time_us = 30;
	else if (IS_GEN10(dev_priv))
		sagv_block_time_us = 20;
	else
		sagv_block_time_us = 10;

	/*
	 * SKL+ workaround: bspec recommends we disable the SAGV when we have
	 * more then one pipe enabled
	 *
	 * If there are no active CRTCs, no additional checks need be performed
	 */
	if (hweight32(intel_state->active_crtcs) == 0)
		return true;
	else if (hweight32(intel_state->active_crtcs) > 1)
		return false;

	/* Since we're now guaranteed to only have one active CRTC... */
	pipe = ffs(intel_state->active_crtcs) - 1;
	crtc = intel_get_crtc_for_pipe(dev_priv, pipe);
	cstate = to_intel_crtc_state(crtc->base.state);

	if (crtc->base.state->adjusted_mode.flags & DRM_MODE_FLAG_INTERLACE)
		return false;

	for_each_intel_plane_on_crtc(dev, crtc, plane) {
		struct skl_plane_wm *wm =
			&cstate->wm.skl.optimal.planes[plane->id];

		/* Skip this plane if it's not enabled */
		if (!wm->wm[0].plane_en)
=======
	if (!crtc_state->hw.active)
		return true;

	if (crtc_state->hw.pipe_mode.flags & DRM_MODE_FLAG_INTERLACE)
		return false;

	for_each_plane_id_on_crtc(crtc, plane_id) {
		const struct skl_plane_wm *wm =
			&crtc_state->wm.skl.optimal.planes[plane_id];
		int level;

		/* Skip this plane if it's not enabled */
		if (!wm->wm[0].enable)
>>>>>>> upstream/android-13
			continue;

		/* Find the highest enabled wm level for this plane */
		for (level = ilk_wm_max_level(dev_priv);
<<<<<<< HEAD
		     !wm->wm[level].plane_en; --level)
		     { }

		latency = dev_priv->wm.skl_latency[level];

		if (skl_needs_memory_bw_wa(intel_state) &&
		    plane->base.state->fb->modifier ==
		    I915_FORMAT_MOD_X_TILED)
			latency += 15;

		/*
		 * If any of the planes on this pipe don't enable wm levels that
		 * incur memory latencies higher than sagv_block_time_us we
		 * can't enable the SAGV.
		 */
		if (latency < sagv_block_time_us)
=======
		     !wm->wm[level].enable; --level)
		     { }

		/* Highest common enabled wm level for all planes */
		max_level = min(level, max_level);
	}

	/* No enabled planes? */
	if (max_level == INT_MAX)
		return true;

	for_each_plane_id_on_crtc(crtc, plane_id) {
		const struct skl_plane_wm *wm =
			&crtc_state->wm.skl.optimal.planes[plane_id];

		/*
		 * All enabled planes must have enabled a common wm level that
		 * can tolerate memory latencies higher than sagv_block_time_us
		 */
		if (wm->wm[0].enable && !wm->wm[max_level].can_sagv)
>>>>>>> upstream/android-13
			return false;
	}

	return true;
}

<<<<<<< HEAD
static unsigned int intel_get_ddb_size(struct drm_i915_private *dev_priv,
				       const struct intel_crtc_state *cstate,
				       const unsigned int total_data_rate,
				       const int num_active,
				       struct skl_ddb_allocation *ddb)
{
	const struct drm_display_mode *adjusted_mode;
	u64 total_data_bw;
	u16 ddb_size = INTEL_INFO(dev_priv)->ddb_size;

	WARN_ON(ddb_size == 0);

	if (INTEL_GEN(dev_priv) < 11)
		return ddb_size - 4; /* 4 blocks for bypass path allocation */

	adjusted_mode = &cstate->base.adjusted_mode;
	total_data_bw = (u64)total_data_rate * drm_mode_vrefresh(adjusted_mode);

	/*
	 * 12GB/s is maximum BW supported by single DBuf slice.
	 */
	if (total_data_bw >= GBps(12) || num_active > 1) {
		ddb->enabled_slices = 2;
	} else {
		ddb->enabled_slices = 1;
		ddb_size /= 2;
	}

	return ddb_size;
}

static void
skl_ddb_get_pipe_allocation_limits(struct drm_device *dev,
				   const struct intel_crtc_state *cstate,
				   const unsigned int total_data_rate,
				   struct skl_ddb_allocation *ddb,
				   struct skl_ddb_entry *alloc, /* out */
				   int *num_active /* out */)
{
	struct drm_atomic_state *state = cstate->base.state;
	struct intel_atomic_state *intel_state = to_intel_atomic_state(state);
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct drm_crtc *for_crtc = cstate->base.crtc;
	unsigned int pipe_size, ddb_size;
	int nth_active_pipe;

	if (WARN_ON(!state) || !cstate->base.active) {
		alloc->start = 0;
		alloc->end = 0;
		*num_active = hweight32(dev_priv->active_crtcs);
		return;
	}

	if (intel_state->active_pipe_changes)
		*num_active = hweight32(intel_state->active_crtcs);
	else
		*num_active = hweight32(dev_priv->active_crtcs);

	ddb_size = intel_get_ddb_size(dev_priv, cstate, total_data_rate,
				      *num_active, ddb);

	/*
	 * If the state doesn't change the active CRTC's, then there's
	 * no need to recalculate; the existing pipe allocation limits
	 * should remain unchanged.  Note that we're safe from racing
	 * commits since any racing commit that changes the active CRTC
	 * list would need to grab _all_ crtc locks, including the one
	 * we currently hold.
	 */
	if (!intel_state->active_pipe_changes) {
		/*
		 * alloc may be cleared by clear_intel_crtc_state,
		 * copy from old state to be sure
		 */
		*alloc = to_intel_crtc_state(for_crtc->state)->wm.skl.ddb;
		return;
	}

	nth_active_pipe = hweight32(intel_state->active_crtcs &
				    (drm_crtc_mask(for_crtc) - 1));
	pipe_size = ddb_size / hweight32(intel_state->active_crtcs);
	alloc->start = nth_active_pipe * ddb_size / *num_active;
	alloc->end = alloc->start + pipe_size;
}

static unsigned int skl_cursor_allocation(int num_active)
{
	if (num_active == 1)
		return 32;

	return 8;
=======
static bool tgl_crtc_can_enable_sagv(const struct intel_crtc_state *crtc_state)
{
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->uapi.crtc);
	enum plane_id plane_id;

	if (!crtc_state->hw.active)
		return true;

	for_each_plane_id_on_crtc(crtc, plane_id) {
		const struct skl_plane_wm *wm =
			&crtc_state->wm.skl.optimal.planes[plane_id];

		if (wm->wm[0].enable && !wm->sagv.wm0.enable)
			return false;
	}

	return true;
}

static bool intel_crtc_can_enable_sagv(const struct intel_crtc_state *crtc_state)
{
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->uapi.crtc);
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);

	if (DISPLAY_VER(dev_priv) >= 12)
		return tgl_crtc_can_enable_sagv(crtc_state);
	else
		return skl_crtc_can_enable_sagv(crtc_state);
}

bool intel_can_enable_sagv(struct drm_i915_private *dev_priv,
			   const struct intel_bw_state *bw_state)
{
	if (DISPLAY_VER(dev_priv) < 11 &&
	    bw_state->active_pipes && !is_power_of_2(bw_state->active_pipes))
		return false;

	return bw_state->pipe_sagv_reject == 0;
}

static int intel_compute_sagv_mask(struct intel_atomic_state *state)
{
	struct drm_i915_private *dev_priv = to_i915(state->base.dev);
	int ret;
	struct intel_crtc *crtc;
	struct intel_crtc_state *new_crtc_state;
	struct intel_bw_state *new_bw_state = NULL;
	const struct intel_bw_state *old_bw_state = NULL;
	int i;

	for_each_new_intel_crtc_in_state(state, crtc,
					 new_crtc_state, i) {
		new_bw_state = intel_atomic_get_bw_state(state);
		if (IS_ERR(new_bw_state))
			return PTR_ERR(new_bw_state);

		old_bw_state = intel_atomic_get_old_bw_state(state);

		if (intel_crtc_can_enable_sagv(new_crtc_state))
			new_bw_state->pipe_sagv_reject &= ~BIT(crtc->pipe);
		else
			new_bw_state->pipe_sagv_reject |= BIT(crtc->pipe);
	}

	if (!new_bw_state)
		return 0;

	new_bw_state->active_pipes =
		intel_calc_active_pipes(state, old_bw_state->active_pipes);

	if (new_bw_state->active_pipes != old_bw_state->active_pipes) {
		ret = intel_atomic_lock_global_state(&new_bw_state->base);
		if (ret)
			return ret;
	}

	if (intel_can_enable_sagv(dev_priv, new_bw_state) !=
	    intel_can_enable_sagv(dev_priv, old_bw_state)) {
		ret = intel_atomic_serialize_global_state(&new_bw_state->base);
		if (ret)
			return ret;
	} else if (new_bw_state->pipe_sagv_reject != old_bw_state->pipe_sagv_reject) {
		ret = intel_atomic_lock_global_state(&new_bw_state->base);
		if (ret)
			return ret;
	}

	for_each_new_intel_crtc_in_state(state, crtc,
					 new_crtc_state, i) {
		struct skl_pipe_wm *pipe_wm = &new_crtc_state->wm.skl.optimal;

		/*
		 * We store use_sagv_wm in the crtc state rather than relying on
		 * that bw state since we have no convenient way to get at the
		 * latter from the plane commit hooks (especially in the legacy
		 * cursor case)
		 */
		pipe_wm->use_sagv_wm = !HAS_HW_SAGV_WM(dev_priv) &&
			DISPLAY_VER(dev_priv) >= 12 &&
			intel_can_enable_sagv(dev_priv, new_bw_state);
	}

	return 0;
}

static int intel_dbuf_slice_size(struct drm_i915_private *dev_priv)
{
	return INTEL_INFO(dev_priv)->dbuf.size /
		hweight8(INTEL_INFO(dev_priv)->dbuf.slice_mask);
}

static void
skl_ddb_entry_for_slices(struct drm_i915_private *dev_priv, u8 slice_mask,
			 struct skl_ddb_entry *ddb)
{
	int slice_size = intel_dbuf_slice_size(dev_priv);

	if (!slice_mask) {
		ddb->start = 0;
		ddb->end = 0;
		return;
	}

	ddb->start = (ffs(slice_mask) - 1) * slice_size;
	ddb->end = fls(slice_mask) * slice_size;

	WARN_ON(ddb->start >= ddb->end);
	WARN_ON(ddb->end > INTEL_INFO(dev_priv)->dbuf.size);
}

static unsigned int mbus_ddb_offset(struct drm_i915_private *i915, u8 slice_mask)
{
	struct skl_ddb_entry ddb;

	if (slice_mask & (BIT(DBUF_S1) | BIT(DBUF_S2)))
		slice_mask = BIT(DBUF_S1);
	else if (slice_mask & (BIT(DBUF_S3) | BIT(DBUF_S4)))
		slice_mask = BIT(DBUF_S3);

	skl_ddb_entry_for_slices(i915, slice_mask, &ddb);

	return ddb.start;
}

u32 skl_ddb_dbuf_slice_mask(struct drm_i915_private *dev_priv,
			    const struct skl_ddb_entry *entry)
{
	int slice_size = intel_dbuf_slice_size(dev_priv);
	enum dbuf_slice start_slice, end_slice;
	u8 slice_mask = 0;

	if (!skl_ddb_entry_size(entry))
		return 0;

	start_slice = entry->start / slice_size;
	end_slice = (entry->end - 1) / slice_size;

	/*
	 * Per plane DDB entry can in a really worst case be on multiple slices
	 * but single entry is anyway contigious.
	 */
	while (start_slice <= end_slice) {
		slice_mask |= BIT(start_slice);
		start_slice++;
	}

	return slice_mask;
}

static unsigned int intel_crtc_ddb_weight(const struct intel_crtc_state *crtc_state)
{
	const struct drm_display_mode *pipe_mode = &crtc_state->hw.pipe_mode;
	int hdisplay, vdisplay;

	if (!crtc_state->hw.active)
		return 0;

	/*
	 * Watermark/ddb requirement highly depends upon width of the
	 * framebuffer, So instead of allocating DDB equally among pipes
	 * distribute DDB based on resolution/width of the display.
	 */
	drm_mode_get_hv_timing(pipe_mode, &hdisplay, &vdisplay);

	return hdisplay;
}

static void intel_crtc_dbuf_weights(const struct intel_dbuf_state *dbuf_state,
				    enum pipe for_pipe,
				    unsigned int *weight_start,
				    unsigned int *weight_end,
				    unsigned int *weight_total)
{
	struct drm_i915_private *dev_priv =
		to_i915(dbuf_state->base.state->base.dev);
	enum pipe pipe;

	*weight_start = 0;
	*weight_end = 0;
	*weight_total = 0;

	for_each_pipe(dev_priv, pipe) {
		int weight = dbuf_state->weight[pipe];

		/*
		 * Do not account pipes using other slice sets
		 * luckily as of current BSpec slice sets do not partially
		 * intersect(pipes share either same one slice or same slice set
		 * i.e no partial intersection), so it is enough to check for
		 * equality for now.
		 */
		if (dbuf_state->slices[pipe] != dbuf_state->slices[for_pipe])
			continue;

		*weight_total += weight;
		if (pipe < for_pipe) {
			*weight_start += weight;
			*weight_end += weight;
		} else if (pipe == for_pipe) {
			*weight_end += weight;
		}
	}
}

static int
skl_crtc_allocate_ddb(struct intel_atomic_state *state, struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	unsigned int weight_total, weight_start, weight_end;
	const struct intel_dbuf_state *old_dbuf_state =
		intel_atomic_get_old_dbuf_state(state);
	struct intel_dbuf_state *new_dbuf_state =
		intel_atomic_get_new_dbuf_state(state);
	struct intel_crtc_state *crtc_state;
	struct skl_ddb_entry ddb_slices;
	enum pipe pipe = crtc->pipe;
	unsigned int mbus_offset = 0;
	u32 ddb_range_size;
	u32 dbuf_slice_mask;
	u32 start, end;
	int ret;

	if (new_dbuf_state->weight[pipe] == 0) {
		new_dbuf_state->ddb[pipe].start = 0;
		new_dbuf_state->ddb[pipe].end = 0;
		goto out;
	}

	dbuf_slice_mask = new_dbuf_state->slices[pipe];

	skl_ddb_entry_for_slices(dev_priv, dbuf_slice_mask, &ddb_slices);
	mbus_offset = mbus_ddb_offset(dev_priv, dbuf_slice_mask);
	ddb_range_size = skl_ddb_entry_size(&ddb_slices);

	intel_crtc_dbuf_weights(new_dbuf_state, pipe,
				&weight_start, &weight_end, &weight_total);

	start = ddb_range_size * weight_start / weight_total;
	end = ddb_range_size * weight_end / weight_total;

	new_dbuf_state->ddb[pipe].start = ddb_slices.start - mbus_offset + start;
	new_dbuf_state->ddb[pipe].end = ddb_slices.start - mbus_offset + end;
out:
	if (old_dbuf_state->slices[pipe] == new_dbuf_state->slices[pipe] &&
	    skl_ddb_entry_equal(&old_dbuf_state->ddb[pipe],
				&new_dbuf_state->ddb[pipe]))
		return 0;

	ret = intel_atomic_lock_global_state(&new_dbuf_state->base);
	if (ret)
		return ret;

	crtc_state = intel_atomic_get_crtc_state(&state->base, crtc);
	if (IS_ERR(crtc_state))
		return PTR_ERR(crtc_state);

	/*
	 * Used for checking overlaps, so we need absolute
	 * offsets instead of MBUS relative offsets.
	 */
	crtc_state->wm.skl.ddb.start = mbus_offset + new_dbuf_state->ddb[pipe].start;
	crtc_state->wm.skl.ddb.end = mbus_offset + new_dbuf_state->ddb[pipe].end;

	drm_dbg_kms(&dev_priv->drm,
		    "[CRTC:%d:%s] dbuf slices 0x%x -> 0x%x, ddb (%d - %d) -> (%d - %d), active pipes 0x%x -> 0x%x\n",
		    crtc->base.base.id, crtc->base.name,
		    old_dbuf_state->slices[pipe], new_dbuf_state->slices[pipe],
		    old_dbuf_state->ddb[pipe].start, old_dbuf_state->ddb[pipe].end,
		    new_dbuf_state->ddb[pipe].start, new_dbuf_state->ddb[pipe].end,
		    old_dbuf_state->active_pipes, new_dbuf_state->active_pipes);

	return 0;
}

static int skl_compute_wm_params(const struct intel_crtc_state *crtc_state,
				 int width, const struct drm_format_info *format,
				 u64 modifier, unsigned int rotation,
				 u32 plane_pixel_rate, struct skl_wm_params *wp,
				 int color_plane);
static void skl_compute_plane_wm(const struct intel_crtc_state *crtc_state,
				 int level,
				 unsigned int latency,
				 const struct skl_wm_params *wp,
				 const struct skl_wm_level *result_prev,
				 struct skl_wm_level *result /* out */);

static unsigned int
skl_cursor_allocation(const struct intel_crtc_state *crtc_state,
		      int num_active)
{
	struct drm_i915_private *dev_priv = to_i915(crtc_state->uapi.crtc->dev);
	int level, max_level = ilk_wm_max_level(dev_priv);
	struct skl_wm_level wm = {};
	int ret, min_ddb_alloc = 0;
	struct skl_wm_params wp;

	ret = skl_compute_wm_params(crtc_state, 256,
				    drm_format_info(DRM_FORMAT_ARGB8888),
				    DRM_FORMAT_MOD_LINEAR,
				    DRM_MODE_ROTATE_0,
				    crtc_state->pixel_rate, &wp, 0);
	drm_WARN_ON(&dev_priv->drm, ret);

	for (level = 0; level <= max_level; level++) {
		unsigned int latency = dev_priv->wm.skl_latency[level];

		skl_compute_plane_wm(crtc_state, level, latency, &wp, &wm, &wm);
		if (wm.min_ddb_alloc == U16_MAX)
			break;

		min_ddb_alloc = wm.min_ddb_alloc;
	}

	return max(num_active == 1 ? 32 : 8, min_ddb_alloc);
>>>>>>> upstream/android-13
}

static void skl_ddb_entry_init_from_hw(struct drm_i915_private *dev_priv,
				       struct skl_ddb_entry *entry, u32 reg)
{
<<<<<<< HEAD
	u16 mask;

	if (INTEL_GEN(dev_priv) >= 11)
		mask = ICL_DDB_ENTRY_MASK;
	else
		mask = SKL_DDB_ENTRY_MASK;
	entry->start = reg & mask;
	entry->end = (reg >> DDB_ENTRY_END_SHIFT) & mask;
=======
	entry->start = reg & DDB_ENTRY_MASK;
	entry->end = (reg >> DDB_ENTRY_END_SHIFT) & DDB_ENTRY_MASK;
>>>>>>> upstream/android-13

	if (entry->end)
		entry->end += 1;
}

static void
skl_ddb_get_hw_plane_state(struct drm_i915_private *dev_priv,
			   const enum pipe pipe,
			   const enum plane_id plane_id,
<<<<<<< HEAD
			   struct skl_ddb_allocation *ddb /* out */)
{
	u32 val, val2 = 0;
	int fourcc, pixel_format;

	/* Cursor doesn't support NV12/planar, so no extra calculation needed */
	if (plane_id == PLANE_CURSOR) {
		val = I915_READ(CUR_BUF_CFG(pipe));
		skl_ddb_entry_init_from_hw(dev_priv,
					   &ddb->plane[pipe][plane_id], val);
		return;
	}

	val = I915_READ(PLANE_CTL(pipe, plane_id));

	/* No DDB allocated for disabled planes */
	if (!(val & PLANE_CTL_ENABLE))
		return;

	pixel_format = val & PLANE_CTL_FORMAT_MASK;
	fourcc = skl_format_to_fourcc(pixel_format,
				      val & PLANE_CTL_ORDER_RGBX,
				      val & PLANE_CTL_ALPHA_MASK);

	val = I915_READ(PLANE_BUF_CFG(pipe, plane_id));
	val2 = I915_READ(PLANE_NV12_BUF_CFG(pipe, plane_id));

	if (fourcc == DRM_FORMAT_NV12) {
		skl_ddb_entry_init_from_hw(dev_priv,
					   &ddb->plane[pipe][plane_id], val2);
		skl_ddb_entry_init_from_hw(dev_priv,
					   &ddb->uv_plane[pipe][plane_id], val);
	} else {
		skl_ddb_entry_init_from_hw(dev_priv,
					   &ddb->plane[pipe][plane_id], val);
	}
}

void skl_ddb_get_hw_state(struct drm_i915_private *dev_priv,
			  struct skl_ddb_allocation *ddb /* out */)
{
	struct intel_crtc *crtc;

	memset(ddb, 0, sizeof(*ddb));

	ddb->enabled_slices = intel_enabled_dbuf_slices_num(dev_priv);

	for_each_intel_crtc(&dev_priv->drm, crtc) {
		enum intel_display_power_domain power_domain;
		enum plane_id plane_id;
		enum pipe pipe = crtc->pipe;

		power_domain = POWER_DOMAIN_PIPE(pipe);
		if (!intel_display_power_get_if_enabled(dev_priv, power_domain))
			continue;

		for_each_plane_id_on_crtc(crtc, plane_id)
			skl_ddb_get_hw_plane_state(dev_priv, pipe,
						   plane_id, ddb);

		intel_display_power_put(dev_priv, power_domain);
	}
=======
			   struct skl_ddb_entry *ddb_y,
			   struct skl_ddb_entry *ddb_uv)
{
	u32 val, val2;
	u32 fourcc = 0;

	/* Cursor doesn't support NV12/planar, so no extra calculation needed */
	if (plane_id == PLANE_CURSOR) {
		val = intel_uncore_read(&dev_priv->uncore, CUR_BUF_CFG(pipe));
		skl_ddb_entry_init_from_hw(dev_priv, ddb_y, val);
		return;
	}

	val = intel_uncore_read(&dev_priv->uncore, PLANE_CTL(pipe, plane_id));

	/* No DDB allocated for disabled planes */
	if (val & PLANE_CTL_ENABLE)
		fourcc = skl_format_to_fourcc(val & PLANE_CTL_FORMAT_MASK,
					      val & PLANE_CTL_ORDER_RGBX,
					      val & PLANE_CTL_ALPHA_MASK);

	if (DISPLAY_VER(dev_priv) >= 11) {
		val = intel_uncore_read(&dev_priv->uncore, PLANE_BUF_CFG(pipe, plane_id));
		skl_ddb_entry_init_from_hw(dev_priv, ddb_y, val);
	} else {
		val = intel_uncore_read(&dev_priv->uncore, PLANE_BUF_CFG(pipe, plane_id));
		val2 = intel_uncore_read(&dev_priv->uncore, PLANE_NV12_BUF_CFG(pipe, plane_id));

		if (fourcc &&
		    drm_format_info_is_yuv_semiplanar(drm_format_info(fourcc)))
			swap(val, val2);

		skl_ddb_entry_init_from_hw(dev_priv, ddb_y, val);
		skl_ddb_entry_init_from_hw(dev_priv, ddb_uv, val2);
	}
}

void skl_pipe_ddb_get_hw_state(struct intel_crtc *crtc,
			       struct skl_ddb_entry *ddb_y,
			       struct skl_ddb_entry *ddb_uv)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	enum intel_display_power_domain power_domain;
	enum pipe pipe = crtc->pipe;
	intel_wakeref_t wakeref;
	enum plane_id plane_id;

	power_domain = POWER_DOMAIN_PIPE(pipe);
	wakeref = intel_display_power_get_if_enabled(dev_priv, power_domain);
	if (!wakeref)
		return;

	for_each_plane_id_on_crtc(crtc, plane_id)
		skl_ddb_get_hw_plane_state(dev_priv, pipe,
					   plane_id,
					   &ddb_y[plane_id],
					   &ddb_uv[plane_id]);

	intel_display_power_put(dev_priv, power_domain, wakeref);
>>>>>>> upstream/android-13
}

/*
 * Determines the downscale amount of a plane for the purposes of watermark calculations.
 * The bspec defines downscale amount as:
 *
 * """
 * Horizontal down scale amount = maximum[1, Horizontal source size /
 *                                           Horizontal destination size]
 * Vertical down scale amount = maximum[1, Vertical source size /
 *                                         Vertical destination size]
 * Total down scale amount = Horizontal down scale amount *
 *                           Vertical down scale amount
 * """
 *
 * Return value is provided in 16.16 fixed point form to retain fractional part.
 * Caller should take care of dividing & rounding off the value.
 */
static uint_fixed_16_16_t
<<<<<<< HEAD
skl_plane_downscale_amount(const struct intel_crtc_state *cstate,
			   const struct intel_plane_state *pstate)
{
	struct intel_plane *plane = to_intel_plane(pstate->base.plane);
	uint32_t src_w, src_h, dst_w, dst_h;
	uint_fixed_16_16_t fp_w_ratio, fp_h_ratio;
	uint_fixed_16_16_t downscale_h, downscale_w;

	if (WARN_ON(!intel_wm_plane_visible(cstate, pstate)))
		return u32_to_fixed16(0);

	/* n.b., src is 16.16 fixed point, dst is whole integer */
	if (plane->id == PLANE_CURSOR) {
		/*
		 * Cursors only support 0/180 degree rotation,
		 * hence no need to account for rotation here.
		 */
		src_w = pstate->base.src_w >> 16;
		src_h = pstate->base.src_h >> 16;
		dst_w = pstate->base.crtc_w;
		dst_h = pstate->base.crtc_h;
	} else {
		/*
		 * Src coordinates are already rotated by 270 degrees for
		 * the 90/270 degree plane rotation cases (to match the
		 * GTT mapping), hence no need to account for rotation here.
		 */
		src_w = drm_rect_width(&pstate->base.src) >> 16;
		src_h = drm_rect_height(&pstate->base.src) >> 16;
		dst_w = drm_rect_width(&pstate->base.dst);
		dst_h = drm_rect_height(&pstate->base.dst);
	}
=======
skl_plane_downscale_amount(const struct intel_crtc_state *crtc_state,
			   const struct intel_plane_state *plane_state)
{
	struct drm_i915_private *dev_priv = to_i915(crtc_state->uapi.crtc->dev);
	u32 src_w, src_h, dst_w, dst_h;
	uint_fixed_16_16_t fp_w_ratio, fp_h_ratio;
	uint_fixed_16_16_t downscale_h, downscale_w;

	if (drm_WARN_ON(&dev_priv->drm,
			!intel_wm_plane_visible(crtc_state, plane_state)))
		return u32_to_fixed16(0);

	/*
	 * Src coordinates are already rotated by 270 degrees for
	 * the 90/270 degree plane rotation cases (to match the
	 * GTT mapping), hence no need to account for rotation here.
	 *
	 * n.b., src is 16.16 fixed point, dst is whole integer.
	 */
	src_w = drm_rect_width(&plane_state->uapi.src) >> 16;
	src_h = drm_rect_height(&plane_state->uapi.src) >> 16;
	dst_w = drm_rect_width(&plane_state->uapi.dst);
	dst_h = drm_rect_height(&plane_state->uapi.dst);
>>>>>>> upstream/android-13

	fp_w_ratio = div_fixed16(src_w, dst_w);
	fp_h_ratio = div_fixed16(src_h, dst_h);
	downscale_w = max_fixed16(fp_w_ratio, u32_to_fixed16(1));
	downscale_h = max_fixed16(fp_h_ratio, u32_to_fixed16(1));

	return mul_fixed16(downscale_w, downscale_h);
}

<<<<<<< HEAD
static uint_fixed_16_16_t
skl_pipe_downscale_amount(const struct intel_crtc_state *crtc_state)
{
	uint_fixed_16_16_t pipe_downscale = u32_to_fixed16(1);

	if (!crtc_state->base.enable)
		return pipe_downscale;

	if (crtc_state->pch_pfit.enabled) {
		uint32_t src_w, src_h, dst_w, dst_h;
		uint32_t pfit_size = crtc_state->pch_pfit.size;
		uint_fixed_16_16_t fp_w_ratio, fp_h_ratio;
		uint_fixed_16_16_t downscale_h, downscale_w;

		src_w = crtc_state->pipe_src_w;
		src_h = crtc_state->pipe_src_h;
		dst_w = pfit_size >> 16;
		dst_h = pfit_size & 0xffff;

		if (!dst_w || !dst_h)
			return pipe_downscale;

		fp_w_ratio = div_fixed16(src_w, dst_w);
		fp_h_ratio = div_fixed16(src_h, dst_h);
		downscale_w = max_fixed16(fp_w_ratio, u32_to_fixed16(1));
		downscale_h = max_fixed16(fp_h_ratio, u32_to_fixed16(1));

		pipe_downscale = mul_fixed16(downscale_w, downscale_h);
	}

	return pipe_downscale;
}

int skl_check_pipe_max_pixel_rate(struct intel_crtc *intel_crtc,
				  struct intel_crtc_state *cstate)
{
	struct drm_i915_private *dev_priv = to_i915(intel_crtc->base.dev);
	struct drm_crtc_state *crtc_state = &cstate->base;
	struct drm_atomic_state *state = crtc_state->state;
	struct drm_plane *plane;
	const struct drm_plane_state *pstate;
	struct intel_plane_state *intel_pstate;
	int crtc_clock, dotclk;
	uint32_t pipe_max_pixel_rate;
	uint_fixed_16_16_t pipe_downscale;
	uint_fixed_16_16_t max_downscale = u32_to_fixed16(1);

	if (!cstate->base.enable)
		return 0;

	drm_atomic_crtc_state_for_each_plane_state(plane, pstate, crtc_state) {
		uint_fixed_16_16_t plane_downscale;
		uint_fixed_16_16_t fp_9_div_8 = div_fixed16(9, 8);
		int bpp;

		if (!intel_wm_plane_visible(cstate,
					    to_intel_plane_state(pstate)))
			continue;

		if (WARN_ON(!pstate->fb))
			return -EINVAL;

		intel_pstate = to_intel_plane_state(pstate);
		plane_downscale = skl_plane_downscale_amount(cstate,
							     intel_pstate);
		bpp = pstate->fb->format->cpp[0] * 8;
		if (bpp == 64)
			plane_downscale = mul_fixed16(plane_downscale,
						      fp_9_div_8);

		max_downscale = max_fixed16(plane_downscale, max_downscale);
	}
	pipe_downscale = skl_pipe_downscale_amount(cstate);

	pipe_downscale = mul_fixed16(pipe_downscale, max_downscale);

	crtc_clock = crtc_state->adjusted_mode.crtc_clock;
	dotclk = to_intel_atomic_state(state)->cdclk.logical.cdclk;

	if (IS_GEMINILAKE(dev_priv) || INTEL_GEN(dev_priv) >= 10)
		dotclk *= 2;

	pipe_max_pixel_rate = div_round_up_u32_fixed16(dotclk, pipe_downscale);

	if (pipe_max_pixel_rate < crtc_clock) {
		DRM_DEBUG_KMS("Max supported pixel clock with scaling exceeded\n");
		return -EINVAL;
	}

	return 0;
}

static unsigned int
skl_plane_relative_data_rate(const struct intel_crtc_state *cstate,
			     const struct drm_plane_state *pstate,
			     const int plane)
{
	struct intel_plane *intel_plane = to_intel_plane(pstate->plane);
	struct intel_plane_state *intel_pstate = to_intel_plane_state(pstate);
	uint32_t data_rate;
	uint32_t width = 0, height = 0;
	struct drm_framebuffer *fb;
	u32 format;
	uint_fixed_16_16_t down_scale_amount;

	if (!intel_pstate->base.visible)
		return 0;

	fb = pstate->fb;
	format = fb->format->format;

	if (intel_plane->id == PLANE_CURSOR)
		return 0;
	if (plane == 1 && format != DRM_FORMAT_NV12)
=======
struct dbuf_slice_conf_entry {
	u8 active_pipes;
	u8 dbuf_mask[I915_MAX_PIPES];
	bool join_mbus;
};

/*
 * Table taken from Bspec 12716
 * Pipes do have some preferred DBuf slice affinity,
 * plus there are some hardcoded requirements on how
 * those should be distributed for multipipe scenarios.
 * For more DBuf slices algorithm can get even more messy
 * and less readable, so decided to use a table almost
 * as is from BSpec itself - that way it is at least easier
 * to compare, change and check.
 */
static const struct dbuf_slice_conf_entry icl_allowed_dbufs[] =
/* Autogenerated with igt/tools/intel_dbuf_map tool: */
{
	{
		.active_pipes = BIT(PIPE_A),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
		},
	},
	{
		.active_pipes = BIT(PIPE_B),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S1),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
			[PIPE_B] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_C] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
			[PIPE_C] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_B) | BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S1),
			[PIPE_C] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
			[PIPE_B] = BIT(DBUF_S1),
			[PIPE_C] = BIT(DBUF_S2),
		},
	},
	{}
};

/*
 * Table taken from Bspec 49255
 * Pipes do have some preferred DBuf slice affinity,
 * plus there are some hardcoded requirements on how
 * those should be distributed for multipipe scenarios.
 * For more DBuf slices algorithm can get even more messy
 * and less readable, so decided to use a table almost
 * as is from BSpec itself - that way it is at least easier
 * to compare, change and check.
 */
static const struct dbuf_slice_conf_entry tgl_allowed_dbufs[] =
/* Autogenerated with igt/tools/intel_dbuf_map tool: */
{
	{
		.active_pipes = BIT(PIPE_A),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_B),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S1) | BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S2),
			[PIPE_B] = BIT(DBUF_S1),
		},
	},
	{
		.active_pipes = BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_C] = BIT(DBUF_S2) | BIT(DBUF_S1),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
			[PIPE_C] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_B) | BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S1),
			[PIPE_C] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
			[PIPE_B] = BIT(DBUF_S1),
			[PIPE_C] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_D] = BIT(DBUF_S2) | BIT(DBUF_S1),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
			[PIPE_D] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_B) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S1),
			[PIPE_D] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
			[PIPE_B] = BIT(DBUF_S1),
			[PIPE_D] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_C) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_C] = BIT(DBUF_S1),
			[PIPE_D] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_C) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
			[PIPE_C] = BIT(DBUF_S2),
			[PIPE_D] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_B) | BIT(PIPE_C) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S1),
			[PIPE_C] = BIT(DBUF_S2),
			[PIPE_D] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_C) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
			[PIPE_B] = BIT(DBUF_S1),
			[PIPE_C] = BIT(DBUF_S2),
			[PIPE_D] = BIT(DBUF_S2),
		},
	},
	{}
};

static const struct dbuf_slice_conf_entry dg2_allowed_dbufs[] = {
	{
		.active_pipes = BIT(PIPE_A),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_B),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S1) | BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
			[PIPE_B] = BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_C] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2),
			[PIPE_C] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_B) | BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S1) | BIT(DBUF_S2),
			[PIPE_C] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
			[PIPE_B] = BIT(DBUF_S2),
			[PIPE_C] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_D] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2),
			[PIPE_D] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_B) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S1) | BIT(DBUF_S2),
			[PIPE_D] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
			[PIPE_B] = BIT(DBUF_S2),
			[PIPE_D] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_C) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_C] = BIT(DBUF_S3),
			[PIPE_D] = BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_C) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2),
			[PIPE_C] = BIT(DBUF_S3),
			[PIPE_D] = BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_B) | BIT(PIPE_C) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S1) | BIT(DBUF_S2),
			[PIPE_C] = BIT(DBUF_S3),
			[PIPE_D] = BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_C) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1),
			[PIPE_B] = BIT(DBUF_S2),
			[PIPE_C] = BIT(DBUF_S3),
			[PIPE_D] = BIT(DBUF_S4),
		},
	},
	{}
};

static const struct dbuf_slice_conf_entry adlp_allowed_dbufs[] = {
	/*
	 * Keep the join_mbus cases first so check_mbus_joined()
	 * will prefer them over the !join_mbus cases.
	 */
	{
		.active_pipes = BIT(PIPE_A),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2) | BIT(DBUF_S3) | BIT(DBUF_S4),
		},
		.join_mbus = true,
	},
	{
		.active_pipes = BIT(PIPE_B),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S1) | BIT(DBUF_S2) | BIT(DBUF_S3) | BIT(DBUF_S4),
		},
		.join_mbus = true,
	},
	{
		.active_pipes = BIT(PIPE_A),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2),
		},
		.join_mbus = false,
	},
	{
		.active_pipes = BIT(PIPE_B),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
		.join_mbus = false,
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2),
			[PIPE_B] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_C] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2),
			[PIPE_C] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_B) | BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S3) | BIT(DBUF_S4),
			[PIPE_C] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_C),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2),
			[PIPE_B] = BIT(DBUF_S3) | BIT(DBUF_S4),
			[PIPE_C] = BIT(DBUF_S3) | BIT(DBUF_S4),
		},
	},
	{
		.active_pipes = BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_D] = BIT(DBUF_S1) | BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2),
			[PIPE_D] = BIT(DBUF_S1) | BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_B) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S3) | BIT(DBUF_S4),
			[PIPE_D] = BIT(DBUF_S1) | BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2),
			[PIPE_B] = BIT(DBUF_S3) | BIT(DBUF_S4),
			[PIPE_D] = BIT(DBUF_S1) | BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_C) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_C] = BIT(DBUF_S3) | BIT(DBUF_S4),
			[PIPE_D] = BIT(DBUF_S1) | BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_C) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2),
			[PIPE_C] = BIT(DBUF_S3) | BIT(DBUF_S4),
			[PIPE_D] = BIT(DBUF_S1) | BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_B) | BIT(PIPE_C) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_B] = BIT(DBUF_S3) | BIT(DBUF_S4),
			[PIPE_C] = BIT(DBUF_S3) | BIT(DBUF_S4),
			[PIPE_D] = BIT(DBUF_S1) | BIT(DBUF_S2),
		},
	},
	{
		.active_pipes = BIT(PIPE_A) | BIT(PIPE_B) | BIT(PIPE_C) | BIT(PIPE_D),
		.dbuf_mask = {
			[PIPE_A] = BIT(DBUF_S1) | BIT(DBUF_S2),
			[PIPE_B] = BIT(DBUF_S3) | BIT(DBUF_S4),
			[PIPE_C] = BIT(DBUF_S3) | BIT(DBUF_S4),
			[PIPE_D] = BIT(DBUF_S1) | BIT(DBUF_S2),
		},
	},
	{}

};

static bool check_mbus_joined(u8 active_pipes,
			      const struct dbuf_slice_conf_entry *dbuf_slices)
{
	int i;

	for (i = 0; dbuf_slices[i].active_pipes != 0; i++) {
		if (dbuf_slices[i].active_pipes == active_pipes)
			return dbuf_slices[i].join_mbus;
	}
	return false;
}

static bool adlp_check_mbus_joined(u8 active_pipes)
{
	return check_mbus_joined(active_pipes, adlp_allowed_dbufs);
}

static u8 compute_dbuf_slices(enum pipe pipe, u8 active_pipes, bool join_mbus,
			      const struct dbuf_slice_conf_entry *dbuf_slices)
{
	int i;

	for (i = 0; dbuf_slices[i].active_pipes != 0; i++) {
		if (dbuf_slices[i].active_pipes == active_pipes &&
		    dbuf_slices[i].join_mbus == join_mbus)
			return dbuf_slices[i].dbuf_mask[pipe];
	}
	return 0;
}

/*
 * This function finds an entry with same enabled pipe configuration and
 * returns correspondent DBuf slice mask as stated in BSpec for particular
 * platform.
 */
static u8 icl_compute_dbuf_slices(enum pipe pipe, u8 active_pipes, bool join_mbus)
{
	/*
	 * FIXME: For ICL this is still a bit unclear as prev BSpec revision
	 * required calculating "pipe ratio" in order to determine
	 * if one or two slices can be used for single pipe configurations
	 * as additional constraint to the existing table.
	 * However based on recent info, it should be not "pipe ratio"
	 * but rather ratio between pixel_rate and cdclk with additional
	 * constants, so for now we are using only table until this is
	 * clarified. Also this is the reason why crtc_state param is
	 * still here - we will need it once those additional constraints
	 * pop up.
	 */
	return compute_dbuf_slices(pipe, active_pipes, join_mbus,
				   icl_allowed_dbufs);
}

static u8 tgl_compute_dbuf_slices(enum pipe pipe, u8 active_pipes, bool join_mbus)
{
	return compute_dbuf_slices(pipe, active_pipes, join_mbus,
				   tgl_allowed_dbufs);
}

static u8 adlp_compute_dbuf_slices(enum pipe pipe, u8 active_pipes, bool join_mbus)
{
	return compute_dbuf_slices(pipe, active_pipes, join_mbus,
				   adlp_allowed_dbufs);
}

static u8 dg2_compute_dbuf_slices(enum pipe pipe, u8 active_pipes, bool join_mbus)
{
	return compute_dbuf_slices(pipe, active_pipes, join_mbus,
				   dg2_allowed_dbufs);
}

static u8 skl_compute_dbuf_slices(struct intel_crtc *crtc, u8 active_pipes, bool join_mbus)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	enum pipe pipe = crtc->pipe;

	if (IS_DG2(dev_priv))
		return dg2_compute_dbuf_slices(pipe, active_pipes, join_mbus);
	else if (IS_ALDERLAKE_P(dev_priv))
		return adlp_compute_dbuf_slices(pipe, active_pipes, join_mbus);
	else if (DISPLAY_VER(dev_priv) == 12)
		return tgl_compute_dbuf_slices(pipe, active_pipes, join_mbus);
	else if (DISPLAY_VER(dev_priv) == 11)
		return icl_compute_dbuf_slices(pipe, active_pipes, join_mbus);
	/*
	 * For anything else just return one slice yet.
	 * Should be extended for other platforms.
	 */
	return active_pipes & BIT(pipe) ? BIT(DBUF_S1) : 0;
}

static u64
skl_plane_relative_data_rate(const struct intel_crtc_state *crtc_state,
			     const struct intel_plane_state *plane_state,
			     int color_plane)
{
	struct intel_plane *plane = to_intel_plane(plane_state->uapi.plane);
	const struct drm_framebuffer *fb = plane_state->hw.fb;
	u32 data_rate;
	u32 width = 0, height = 0;
	uint_fixed_16_16_t down_scale_amount;
	u64 rate;

	if (!plane_state->uapi.visible)
		return 0;

	if (plane->id == PLANE_CURSOR)
		return 0;

	if (color_plane == 1 &&
	    !intel_format_info_is_yuv_semiplanar(fb->format, fb->modifier))
>>>>>>> upstream/android-13
		return 0;

	/*
	 * Src coordinates are already rotated by 270 degrees for
	 * the 90/270 degree plane rotation cases (to match the
	 * GTT mapping), hence no need to account for rotation here.
	 */
<<<<<<< HEAD
	width = drm_rect_width(&intel_pstate->base.src) >> 16;
	height = drm_rect_height(&intel_pstate->base.src) >> 16;

	/* UV plane does 1/2 pixel sub-sampling */
	if (plane == 1 && format == DRM_FORMAT_NV12) {
=======
	width = drm_rect_width(&plane_state->uapi.src) >> 16;
	height = drm_rect_height(&plane_state->uapi.src) >> 16;

	/* UV plane does 1/2 pixel sub-sampling */
	if (color_plane == 1) {
>>>>>>> upstream/android-13
		width /= 2;
		height /= 2;
	}

<<<<<<< HEAD
	data_rate = width * height * fb->format->cpp[plane];

	down_scale_amount = skl_plane_downscale_amount(cstate, intel_pstate);

	return mul_round_up_u32_fixed16(data_rate, down_scale_amount);
}

/*
 * We don't overflow 32 bits. Worst case is 3 planes enabled, each fetching
 * a 8192x4096@32bpp framebuffer:
 *   3 * 4096 * 8192  * 4 < 2^32
 */
static unsigned int
skl_get_total_relative_data_rate(struct intel_crtc_state *intel_cstate,
				 unsigned int *plane_data_rate,
				 unsigned int *uv_plane_data_rate)
{
	struct drm_crtc_state *cstate = &intel_cstate->base;
	struct drm_atomic_state *state = cstate->state;
	struct drm_plane *plane;
	const struct drm_plane_state *pstate;
	unsigned int total_data_rate = 0;

	if (WARN_ON(!state))
		return 0;

	/* Calculate and cache data rate for each plane */
	drm_atomic_crtc_state_for_each_plane_state(plane, pstate, cstate) {
		enum plane_id plane_id = to_intel_plane(plane)->id;
		unsigned int rate;

		/* packed/y */
		rate = skl_plane_relative_data_rate(intel_cstate,
						    pstate, 0);
		plane_data_rate[plane_id] = rate;

		total_data_rate += rate;

		/* uv-plane */
		rate = skl_plane_relative_data_rate(intel_cstate,
						    pstate, 1);
		uv_plane_data_rate[plane_id] = rate;

		total_data_rate += rate;
=======
	data_rate = width * height;

	down_scale_amount = skl_plane_downscale_amount(crtc_state, plane_state);

	rate = mul_round_up_u32_fixed16(data_rate, down_scale_amount);

	rate *= fb->format->cpp[color_plane];
	return rate;
}

static u64
skl_get_total_relative_data_rate(struct intel_atomic_state *state,
				 struct intel_crtc *crtc)
{
	struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
	const struct intel_plane_state *plane_state;
	struct intel_plane *plane;
	u64 total_data_rate = 0;
	enum plane_id plane_id;
	int i;

	/* Calculate and cache data rate for each plane */
	for_each_new_intel_plane_in_state(state, plane, plane_state, i) {
		if (plane->pipe != crtc->pipe)
			continue;

		plane_id = plane->id;

		/* packed/y */
		crtc_state->plane_data_rate[plane_id] =
			skl_plane_relative_data_rate(crtc_state, plane_state, 0);

		/* uv-plane */
		crtc_state->uv_plane_data_rate[plane_id] =
			skl_plane_relative_data_rate(crtc_state, plane_state, 1);
	}

	for_each_plane_id_on_crtc(crtc, plane_id) {
		total_data_rate += crtc_state->plane_data_rate[plane_id];
		total_data_rate += crtc_state->uv_plane_data_rate[plane_id];
>>>>>>> upstream/android-13
	}

	return total_data_rate;
}

<<<<<<< HEAD
static uint16_t
skl_ddb_min_alloc(const struct drm_plane_state *pstate, const int plane)
{
	struct drm_framebuffer *fb = pstate->fb;
	struct intel_plane_state *intel_pstate = to_intel_plane_state(pstate);
	uint32_t src_w, src_h;
	uint32_t min_scanlines = 8;
	uint8_t plane_bpp;

	if (WARN_ON(!fb))
		return 0;

	/* For packed formats, and uv-plane, return 0 */
	if (plane == 1 && fb->format->format != DRM_FORMAT_NV12)
		return 0;

	/* For Non Y-tile return 8-blocks */
	if (fb->modifier != I915_FORMAT_MOD_Y_TILED &&
	    fb->modifier != I915_FORMAT_MOD_Yf_TILED &&
	    fb->modifier != I915_FORMAT_MOD_Y_TILED_CCS &&
	    fb->modifier != I915_FORMAT_MOD_Yf_TILED_CCS)
		return 8;

	/*
	 * Src coordinates are already rotated by 270 degrees for
	 * the 90/270 degree plane rotation cases (to match the
	 * GTT mapping), hence no need to account for rotation here.
	 */
	src_w = drm_rect_width(&intel_pstate->base.src) >> 16;
	src_h = drm_rect_height(&intel_pstate->base.src) >> 16;

	/* Halve UV plane width and height for NV12 */
	if (plane == 1) {
		src_w /= 2;
		src_h /= 2;
	}

	plane_bpp = fb->format->cpp[plane];

	if (drm_rotation_90_or_270(pstate->rotation)) {
		switch (plane_bpp) {
		case 1:
			min_scanlines = 32;
			break;
		case 2:
			min_scanlines = 16;
			break;
		case 4:
			min_scanlines = 8;
			break;
		case 8:
			min_scanlines = 4;
			break;
		default:
			WARN(1, "Unsupported pixel depth %u for rotation",
			     plane_bpp);
			min_scanlines = 32;
		}
	}

	return DIV_ROUND_UP((4 * src_w * plane_bpp), 512) * min_scanlines/4 + 3;
}

static void
skl_ddb_calc_min(const struct intel_crtc_state *cstate, int num_active,
		 uint16_t *minimum, uint16_t *uv_minimum)
{
	const struct drm_plane_state *pstate;
	struct drm_plane *plane;

	drm_atomic_crtc_state_for_each_plane_state(plane, pstate, &cstate->base) {
		enum plane_id plane_id = to_intel_plane(plane)->id;

		if (plane_id == PLANE_CURSOR)
			continue;

		if (!pstate->visible)
			continue;

		minimum[plane_id] = skl_ddb_min_alloc(pstate, 0);
		uv_minimum[plane_id] = skl_ddb_min_alloc(pstate, 1);
	}

	minimum[PLANE_CURSOR] = skl_cursor_allocation(num_active);
}

static int
skl_allocate_pipe_ddb(struct intel_crtc_state *cstate,
		      struct skl_ddb_allocation *ddb /* out */)
{
	struct drm_atomic_state *state = cstate->base.state;
	struct drm_crtc *crtc = cstate->base.crtc;
	struct drm_device *dev = crtc->dev;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	enum pipe pipe = intel_crtc->pipe;
	struct skl_ddb_entry *alloc = &cstate->wm.skl.ddb;
	uint16_t alloc_size, start;
	uint16_t minimum[I915_MAX_PLANES] = {};
	uint16_t uv_minimum[I915_MAX_PLANES] = {};
	unsigned int total_data_rate;
	enum plane_id plane_id;
	int num_active;
	unsigned int plane_data_rate[I915_MAX_PLANES] = {};
	unsigned int uv_plane_data_rate[I915_MAX_PLANES] = {};
	uint16_t total_min_blocks = 0;

	/* Clear the partitioning for disabled planes. */
	memset(ddb->plane[pipe], 0, sizeof(ddb->plane[pipe]));
	memset(ddb->uv_plane[pipe], 0, sizeof(ddb->uv_plane[pipe]));

	if (WARN_ON(!state))
		return 0;

	if (!cstate->base.active) {
		alloc->start = alloc->end = 0;
		return 0;
	}

	total_data_rate = skl_get_total_relative_data_rate(cstate,
							   plane_data_rate,
							   uv_plane_data_rate);
	skl_ddb_get_pipe_allocation_limits(dev, cstate, total_data_rate, ddb,
					   alloc, &num_active);
=======
static u64
icl_get_total_relative_data_rate(struct intel_atomic_state *state,
				 struct intel_crtc *crtc)
{
	struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
	const struct intel_plane_state *plane_state;
	struct intel_plane *plane;
	u64 total_data_rate = 0;
	enum plane_id plane_id;
	int i;

	/* Calculate and cache data rate for each plane */
	for_each_new_intel_plane_in_state(state, plane, plane_state, i) {
		if (plane->pipe != crtc->pipe)
			continue;

		plane_id = plane->id;

		if (!plane_state->planar_linked_plane) {
			crtc_state->plane_data_rate[plane_id] =
				skl_plane_relative_data_rate(crtc_state, plane_state, 0);
		} else {
			enum plane_id y_plane_id;

			/*
			 * The slave plane might not iterate in
			 * intel_atomic_crtc_state_for_each_plane_state(),
			 * and needs the master plane state which may be
			 * NULL if we try get_new_plane_state(), so we
			 * always calculate from the master.
			 */
			if (plane_state->planar_slave)
				continue;

			/* Y plane rate is calculated on the slave */
			y_plane_id = plane_state->planar_linked_plane->id;
			crtc_state->plane_data_rate[y_plane_id] =
				skl_plane_relative_data_rate(crtc_state, plane_state, 0);

			crtc_state->plane_data_rate[plane_id] =
				skl_plane_relative_data_rate(crtc_state, plane_state, 1);
		}
	}

	for_each_plane_id_on_crtc(crtc, plane_id)
		total_data_rate += crtc_state->plane_data_rate[plane_id];

	return total_data_rate;
}

const struct skl_wm_level *
skl_plane_wm_level(const struct skl_pipe_wm *pipe_wm,
		   enum plane_id plane_id,
		   int level)
{
	const struct skl_plane_wm *wm = &pipe_wm->planes[plane_id];

	if (level == 0 && pipe_wm->use_sagv_wm)
		return &wm->sagv.wm0;

	return &wm->wm[level];
}

const struct skl_wm_level *
skl_plane_trans_wm(const struct skl_pipe_wm *pipe_wm,
		   enum plane_id plane_id)
{
	const struct skl_plane_wm *wm = &pipe_wm->planes[plane_id];

	if (pipe_wm->use_sagv_wm)
		return &wm->sagv.trans_wm;

	return &wm->trans_wm;
}

/*
 * We only disable the watermarks for each plane if
 * they exceed the ddb allocation of said plane. This
 * is done so that we don't end up touching cursor
 * watermarks needlessly when some other plane reduces
 * our max possible watermark level.
 *
 * Bspec has this to say about the PLANE_WM enable bit:
 * "All the watermarks at this level for all enabled
 *  planes must be enabled before the level will be used."
 * So this is actually safe to do.
 */
static void
skl_check_wm_level(struct skl_wm_level *wm, u64 total)
{
	if (wm->min_ddb_alloc > total)
		memset(wm, 0, sizeof(*wm));
}

static void
skl_check_nv12_wm_level(struct skl_wm_level *wm, struct skl_wm_level *uv_wm,
			u64 total, u64 uv_total)
{
	if (wm->min_ddb_alloc > total ||
	    uv_wm->min_ddb_alloc > uv_total) {
		memset(wm, 0, sizeof(*wm));
		memset(uv_wm, 0, sizeof(*uv_wm));
	}
}

static int
skl_allocate_plane_ddb(struct intel_atomic_state *state,
		       struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
	const struct intel_dbuf_state *dbuf_state =
		intel_atomic_get_new_dbuf_state(state);
	const struct skl_ddb_entry *alloc = &dbuf_state->ddb[crtc->pipe];
	int num_active = hweight8(dbuf_state->active_pipes);
	u16 alloc_size, start = 0;
	u16 total[I915_MAX_PLANES] = {};
	u16 uv_total[I915_MAX_PLANES] = {};
	u64 total_data_rate;
	enum plane_id plane_id;
	u32 blocks;
	int level;

	/* Clear the partitioning for disabled planes. */
	memset(crtc_state->wm.skl.plane_ddb_y, 0, sizeof(crtc_state->wm.skl.plane_ddb_y));
	memset(crtc_state->wm.skl.plane_ddb_uv, 0, sizeof(crtc_state->wm.skl.plane_ddb_uv));

	if (!crtc_state->hw.active)
		return 0;

	if (DISPLAY_VER(dev_priv) >= 11)
		total_data_rate =
			icl_get_total_relative_data_rate(state, crtc);
	else
		total_data_rate =
			skl_get_total_relative_data_rate(state, crtc);

>>>>>>> upstream/android-13
	alloc_size = skl_ddb_entry_size(alloc);
	if (alloc_size == 0)
		return 0;

<<<<<<< HEAD
	skl_ddb_calc_min(cstate, num_active, minimum, uv_minimum);

	/*
	 * 1. Allocate the mininum required blocks for each active plane
	 * and allocate the cursor, it doesn't require extra allocation
	 * proportional to the data rate.
	 */

	for_each_plane_id_on_crtc(intel_crtc, plane_id) {
		total_min_blocks += minimum[plane_id];
		total_min_blocks += uv_minimum[plane_id];
	}

	if (total_min_blocks > alloc_size) {
		DRM_DEBUG_KMS("Requested display configuration exceeds system DDB limitations");
		DRM_DEBUG_KMS("minimum required %d/%d\n", total_min_blocks,
							alloc_size);
		return -EINVAL;
	}

	alloc_size -= total_min_blocks;
	ddb->plane[pipe][PLANE_CURSOR].start = alloc->end - minimum[PLANE_CURSOR];
	ddb->plane[pipe][PLANE_CURSOR].end = alloc->end;

	/*
	 * 2. Distribute the remaining space in proportion to the amount of
	 * data each plane needs to fetch from memory.
	 *
	 * FIXME: we may not allocate every single block here.
	 */
	if (total_data_rate == 0)
		return 0;

	start = alloc->start;
	for_each_plane_id_on_crtc(intel_crtc, plane_id) {
		unsigned int data_rate, uv_data_rate;
		uint16_t plane_blocks, uv_plane_blocks;
=======
	/* Allocate fixed number of blocks for cursor. */
	total[PLANE_CURSOR] = skl_cursor_allocation(crtc_state, num_active);
	alloc_size -= total[PLANE_CURSOR];
	crtc_state->wm.skl.plane_ddb_y[PLANE_CURSOR].start =
		alloc->end - total[PLANE_CURSOR];
	crtc_state->wm.skl.plane_ddb_y[PLANE_CURSOR].end = alloc->end;

	if (total_data_rate == 0)
		return 0;

	/*
	 * Find the highest watermark level for which we can satisfy the block
	 * requirement of active planes.
	 */
	for (level = ilk_wm_max_level(dev_priv); level >= 0; level--) {
		blocks = 0;
		for_each_plane_id_on_crtc(crtc, plane_id) {
			const struct skl_plane_wm *wm =
				&crtc_state->wm.skl.optimal.planes[plane_id];

			if (plane_id == PLANE_CURSOR) {
				if (wm->wm[level].min_ddb_alloc > total[PLANE_CURSOR]) {
					drm_WARN_ON(&dev_priv->drm,
						    wm->wm[level].min_ddb_alloc != U16_MAX);
					blocks = U32_MAX;
					break;
				}
				continue;
			}

			blocks += wm->wm[level].min_ddb_alloc;
			blocks += wm->uv_wm[level].min_ddb_alloc;
		}

		if (blocks <= alloc_size) {
			alloc_size -= blocks;
			break;
		}
	}

	if (level < 0) {
		drm_dbg_kms(&dev_priv->drm,
			    "Requested display configuration exceeds system DDB limitations");
		drm_dbg_kms(&dev_priv->drm, "minimum required %d/%d\n",
			    blocks, alloc_size);
		return -EINVAL;
	}

	/*
	 * Grant each plane the blocks it requires at the highest achievable
	 * watermark level, plus an extra share of the leftover blocks
	 * proportional to its relative data rate.
	 */
	for_each_plane_id_on_crtc(crtc, plane_id) {
		const struct skl_plane_wm *wm =
			&crtc_state->wm.skl.optimal.planes[plane_id];
		u64 rate;
		u16 extra;
>>>>>>> upstream/android-13

		if (plane_id == PLANE_CURSOR)
			continue;

<<<<<<< HEAD
		data_rate = plane_data_rate[plane_id];

		/*
		 * allocation for (packed formats) or (uv-plane part of planar format):
		 * promote the expression to 64 bits to avoid overflowing, the
		 * result is < available as data_rate / total_data_rate < 1
		 */
		plane_blocks = minimum[plane_id];
		plane_blocks += div_u64((uint64_t)alloc_size * data_rate,
					total_data_rate);

		/* Leave disabled planes at (0,0) */
		if (data_rate) {
			ddb->plane[pipe][plane_id].start = start;
			ddb->plane[pipe][plane_id].end = start + plane_blocks;
		}

		start += plane_blocks;

		/* Allocate DDB for UV plane for planar format/NV12 */
		uv_data_rate = uv_plane_data_rate[plane_id];

		uv_plane_blocks = uv_minimum[plane_id];
		uv_plane_blocks += div_u64((uint64_t)alloc_size * uv_data_rate,
					   total_data_rate);

		if (uv_data_rate) {
			ddb->uv_plane[pipe][plane_id].start = start;
			ddb->uv_plane[pipe][plane_id].end =
				start + uv_plane_blocks;
		}

		start += uv_plane_blocks;
=======
		/*
		 * We've accounted for all active planes; remaining planes are
		 * all disabled.
		 */
		if (total_data_rate == 0)
			break;

		rate = crtc_state->plane_data_rate[plane_id];
		extra = min_t(u16, alloc_size,
			      DIV64_U64_ROUND_UP(alloc_size * rate,
						 total_data_rate));
		total[plane_id] = wm->wm[level].min_ddb_alloc + extra;
		alloc_size -= extra;
		total_data_rate -= rate;

		if (total_data_rate == 0)
			break;

		rate = crtc_state->uv_plane_data_rate[plane_id];
		extra = min_t(u16, alloc_size,
			      DIV64_U64_ROUND_UP(alloc_size * rate,
						 total_data_rate));
		uv_total[plane_id] = wm->uv_wm[level].min_ddb_alloc + extra;
		alloc_size -= extra;
		total_data_rate -= rate;
	}
	drm_WARN_ON(&dev_priv->drm, alloc_size != 0 || total_data_rate != 0);

	/* Set the actual DDB start/end points for each plane */
	start = alloc->start;
	for_each_plane_id_on_crtc(crtc, plane_id) {
		struct skl_ddb_entry *plane_alloc =
			&crtc_state->wm.skl.plane_ddb_y[plane_id];
		struct skl_ddb_entry *uv_plane_alloc =
			&crtc_state->wm.skl.plane_ddb_uv[plane_id];

		if (plane_id == PLANE_CURSOR)
			continue;

		/* Gen11+ uses a separate plane for UV watermarks */
		drm_WARN_ON(&dev_priv->drm,
			    DISPLAY_VER(dev_priv) >= 11 && uv_total[plane_id]);

		/* Leave disabled planes at (0,0) */
		if (total[plane_id]) {
			plane_alloc->start = start;
			start += total[plane_id];
			plane_alloc->end = start;
		}

		if (uv_total[plane_id]) {
			uv_plane_alloc->start = start;
			start += uv_total[plane_id];
			uv_plane_alloc->end = start;
		}
	}

	/*
	 * When we calculated watermark values we didn't know how high
	 * of a level we'd actually be able to hit, so we just marked
	 * all levels as "enabled."  Go back now and disable the ones
	 * that aren't actually possible.
	 */
	for (level++; level <= ilk_wm_max_level(dev_priv); level++) {
		for_each_plane_id_on_crtc(crtc, plane_id) {
			struct skl_plane_wm *wm =
				&crtc_state->wm.skl.optimal.planes[plane_id];

			skl_check_nv12_wm_level(&wm->wm[level], &wm->uv_wm[level],
						total[plane_id], uv_total[plane_id]);

			/*
			 * Wa_1408961008:icl, ehl
			 * Underruns with WM1+ disabled
			 */
			if (DISPLAY_VER(dev_priv) == 11 &&
			    level == 1 && wm->wm[0].enable) {
				wm->wm[level].blocks = wm->wm[0].blocks;
				wm->wm[level].lines = wm->wm[0].lines;
				wm->wm[level].ignore_lines = wm->wm[0].ignore_lines;
			}
		}
	}

	/*
	 * Go back and disable the transition and SAGV watermarks
	 * if it turns out we don't have enough DDB blocks for them.
	 */
	for_each_plane_id_on_crtc(crtc, plane_id) {
		struct skl_plane_wm *wm =
			&crtc_state->wm.skl.optimal.planes[plane_id];

		skl_check_wm_level(&wm->trans_wm, total[plane_id]);
		skl_check_wm_level(&wm->sagv.wm0, total[plane_id]);
		skl_check_wm_level(&wm->sagv.trans_wm, total[plane_id]);
>>>>>>> upstream/android-13
	}

	return 0;
}

/*
 * The max latency should be 257 (max the punit can code is 255 and we add 2us
 * for the read latency) and cpp should always be <= 8, so that
 * should allow pixel_rate up to ~2 GHz which seems sufficient since max
 * 2xcdclk is 1350 MHz and the pixel rate should never exceed that.
*/
static uint_fixed_16_16_t
<<<<<<< HEAD
skl_wm_method1(const struct drm_i915_private *dev_priv, uint32_t pixel_rate,
	       uint8_t cpp, uint32_t latency, uint32_t dbuf_block_size)
{
	uint32_t wm_intermediate_val;
=======
skl_wm_method1(const struct drm_i915_private *dev_priv, u32 pixel_rate,
	       u8 cpp, u32 latency, u32 dbuf_block_size)
{
	u32 wm_intermediate_val;
>>>>>>> upstream/android-13
	uint_fixed_16_16_t ret;

	if (latency == 0)
		return FP_16_16_MAX;

	wm_intermediate_val = latency * pixel_rate * cpp;
	ret = div_fixed16(wm_intermediate_val, 1000 * dbuf_block_size);

<<<<<<< HEAD
	if (INTEL_GEN(dev_priv) >= 10)
=======
	if (DISPLAY_VER(dev_priv) >= 10)
>>>>>>> upstream/android-13
		ret = add_fixed16_u32(ret, 1);

	return ret;
}

<<<<<<< HEAD
static uint_fixed_16_16_t skl_wm_method2(uint32_t pixel_rate,
			uint32_t pipe_htotal,
			uint32_t latency,
			uint_fixed_16_16_t plane_blocks_per_line)
{
	uint32_t wm_intermediate_val;
=======
static uint_fixed_16_16_t
skl_wm_method2(u32 pixel_rate, u32 pipe_htotal, u32 latency,
	       uint_fixed_16_16_t plane_blocks_per_line)
{
	u32 wm_intermediate_val;
>>>>>>> upstream/android-13
	uint_fixed_16_16_t ret;

	if (latency == 0)
		return FP_16_16_MAX;

	wm_intermediate_val = latency * pixel_rate;
	wm_intermediate_val = DIV_ROUND_UP(wm_intermediate_val,
					   pipe_htotal * 1000);
	ret = mul_u32_fixed16(wm_intermediate_val, plane_blocks_per_line);
	return ret;
}

static uint_fixed_16_16_t
<<<<<<< HEAD
intel_get_linetime_us(struct intel_crtc_state *cstate)
{
	uint32_t pixel_rate;
	uint32_t crtc_htotal;
	uint_fixed_16_16_t linetime_us;

	if (!cstate->base.active)
		return u32_to_fixed16(0);

	pixel_rate = cstate->pixel_rate;

	if (WARN_ON(pixel_rate == 0))
		return u32_to_fixed16(0);

	crtc_htotal = cstate->base.adjusted_mode.crtc_htotal;
=======
intel_get_linetime_us(const struct intel_crtc_state *crtc_state)
{
	struct drm_i915_private *dev_priv = to_i915(crtc_state->uapi.crtc->dev);
	u32 pixel_rate;
	u32 crtc_htotal;
	uint_fixed_16_16_t linetime_us;

	if (!crtc_state->hw.active)
		return u32_to_fixed16(0);

	pixel_rate = crtc_state->pixel_rate;

	if (drm_WARN_ON(&dev_priv->drm, pixel_rate == 0))
		return u32_to_fixed16(0);

	crtc_htotal = crtc_state->hw.pipe_mode.crtc_htotal;
>>>>>>> upstream/android-13
	linetime_us = div_fixed16(crtc_htotal * 1000, pixel_rate);

	return linetime_us;
}

<<<<<<< HEAD
static uint32_t
skl_adjusted_plane_pixel_rate(const struct intel_crtc_state *cstate,
			      const struct intel_plane_state *pstate)
{
	uint64_t adjusted_pixel_rate;
	uint_fixed_16_16_t downscale_amount;

	/* Shouldn't reach here on disabled planes... */
	if (WARN_ON(!intel_wm_plane_visible(cstate, pstate)))
		return 0;

	/*
	 * Adjusted plane pixel rate is just the pipe's adjusted pixel rate
	 * with additional adjustments for plane-specific scaling.
	 */
	adjusted_pixel_rate = cstate->pixel_rate;
	downscale_amount = skl_plane_downscale_amount(cstate, pstate);

	return mul_round_up_u32_fixed16(adjusted_pixel_rate,
					    downscale_amount);
}

static int
skl_compute_plane_wm_params(const struct drm_i915_private *dev_priv,
			    struct intel_crtc_state *cstate,
			    const struct intel_plane_state *intel_pstate,
			    struct skl_wm_params *wp, int plane_id)
{
	struct intel_plane *plane = to_intel_plane(intel_pstate->base.plane);
	const struct drm_plane_state *pstate = &intel_pstate->base;
	const struct drm_framebuffer *fb = pstate->fb;
	uint32_t interm_pbpl;
	struct intel_atomic_state *state =
		to_intel_atomic_state(cstate->base.state);
	bool apply_memory_bw_wa = skl_needs_memory_bw_wa(state);

	if (!intel_wm_plane_visible(cstate, intel_pstate))
		return 0;

	/* only NV12 format has two planes */
	if (plane_id == 1 && fb->format->format != DRM_FORMAT_NV12) {
		DRM_DEBUG_KMS("Non NV12 format have single plane\n");
		return -EINVAL;
	}

	wp->y_tiled = fb->modifier == I915_FORMAT_MOD_Y_TILED ||
		      fb->modifier == I915_FORMAT_MOD_Yf_TILED ||
		      fb->modifier == I915_FORMAT_MOD_Y_TILED_CCS ||
		      fb->modifier == I915_FORMAT_MOD_Yf_TILED_CCS;
	wp->x_tiled = fb->modifier == I915_FORMAT_MOD_X_TILED;
	wp->rc_surface = fb->modifier == I915_FORMAT_MOD_Y_TILED_CCS ||
			 fb->modifier == I915_FORMAT_MOD_Yf_TILED_CCS;
	wp->is_planar = fb->format->format == DRM_FORMAT_NV12;

	if (plane->id == PLANE_CURSOR) {
		wp->width = intel_pstate->base.crtc_w;
	} else {
		/*
		 * Src coordinates are already rotated by 270 degrees for
		 * the 90/270 degree plane rotation cases (to match the
		 * GTT mapping), hence no need to account for rotation here.
		 */
		wp->width = drm_rect_width(&intel_pstate->base.src) >> 16;
	}

	if (plane_id == 1 && wp->is_planar)
		wp->width /= 2;

	wp->cpp = fb->format->cpp[plane_id];
	wp->plane_pixel_rate = skl_adjusted_plane_pixel_rate(cstate,
							     intel_pstate);

	if (INTEL_GEN(dev_priv) >= 11 &&
	    fb->modifier == I915_FORMAT_MOD_Yf_TILED && wp->cpp == 8)
=======
static int
skl_compute_wm_params(const struct intel_crtc_state *crtc_state,
		      int width, const struct drm_format_info *format,
		      u64 modifier, unsigned int rotation,
		      u32 plane_pixel_rate, struct skl_wm_params *wp,
		      int color_plane)
{
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->uapi.crtc);
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	u32 interm_pbpl;

	/* only planar format has two planes */
	if (color_plane == 1 &&
	    !intel_format_info_is_yuv_semiplanar(format, modifier)) {
		drm_dbg_kms(&dev_priv->drm,
			    "Non planar format have single plane\n");
		return -EINVAL;
	}

	wp->y_tiled = modifier == I915_FORMAT_MOD_Y_TILED ||
		      modifier == I915_FORMAT_MOD_Yf_TILED ||
		      modifier == I915_FORMAT_MOD_Y_TILED_CCS ||
		      modifier == I915_FORMAT_MOD_Yf_TILED_CCS;
	wp->x_tiled = modifier == I915_FORMAT_MOD_X_TILED;
	wp->rc_surface = modifier == I915_FORMAT_MOD_Y_TILED_CCS ||
			 modifier == I915_FORMAT_MOD_Yf_TILED_CCS;
	wp->is_planar = intel_format_info_is_yuv_semiplanar(format, modifier);

	wp->width = width;
	if (color_plane == 1 && wp->is_planar)
		wp->width /= 2;

	wp->cpp = format->cpp[color_plane];
	wp->plane_pixel_rate = plane_pixel_rate;

	if (DISPLAY_VER(dev_priv) >= 11 &&
	    modifier == I915_FORMAT_MOD_Yf_TILED  && wp->cpp == 1)
>>>>>>> upstream/android-13
		wp->dbuf_block_size = 256;
	else
		wp->dbuf_block_size = 512;

<<<<<<< HEAD
	if (drm_rotation_90_or_270(pstate->rotation)) {

=======
	if (drm_rotation_90_or_270(rotation)) {
>>>>>>> upstream/android-13
		switch (wp->cpp) {
		case 1:
			wp->y_min_scanlines = 16;
			break;
		case 2:
			wp->y_min_scanlines = 8;
			break;
		case 4:
			wp->y_min_scanlines = 4;
			break;
		default:
			MISSING_CASE(wp->cpp);
			return -EINVAL;
		}
	} else {
		wp->y_min_scanlines = 4;
	}

<<<<<<< HEAD
	if (apply_memory_bw_wa)
=======
	if (skl_needs_memory_bw_wa(dev_priv))
>>>>>>> upstream/android-13
		wp->y_min_scanlines *= 2;

	wp->plane_bytes_per_line = wp->width * wp->cpp;
	if (wp->y_tiled) {
		interm_pbpl = DIV_ROUND_UP(wp->plane_bytes_per_line *
					   wp->y_min_scanlines,
					   wp->dbuf_block_size);

<<<<<<< HEAD
		if (INTEL_GEN(dev_priv) >= 10)
=======
		if (DISPLAY_VER(dev_priv) >= 10)
>>>>>>> upstream/android-13
			interm_pbpl++;

		wp->plane_blocks_per_line = div_fixed16(interm_pbpl,
							wp->y_min_scanlines);
<<<<<<< HEAD
	} else if (wp->x_tiled && IS_GEN9(dev_priv)) {
		interm_pbpl = DIV_ROUND_UP(wp->plane_bytes_per_line,
					   wp->dbuf_block_size);
		wp->plane_blocks_per_line = u32_to_fixed16(interm_pbpl);
	} else {
		interm_pbpl = DIV_ROUND_UP(wp->plane_bytes_per_line,
					   wp->dbuf_block_size) + 1;
=======
	} else {
		interm_pbpl = DIV_ROUND_UP(wp->plane_bytes_per_line,
					   wp->dbuf_block_size);

		if (!wp->x_tiled || DISPLAY_VER(dev_priv) >= 10)
			interm_pbpl++;

>>>>>>> upstream/android-13
		wp->plane_blocks_per_line = u32_to_fixed16(interm_pbpl);
	}

	wp->y_tile_minimum = mul_u32_fixed16(wp->y_min_scanlines,
					     wp->plane_blocks_per_line);
<<<<<<< HEAD
	wp->linetime_us = fixed16_to_u32_round_up(
					intel_get_linetime_us(cstate));
=======

	wp->linetime_us = fixed16_to_u32_round_up(
					intel_get_linetime_us(crtc_state));
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int skl_compute_plane_wm(const struct drm_i915_private *dev_priv,
				struct intel_crtc_state *cstate,
				const struct intel_plane_state *intel_pstate,
				uint16_t ddb_allocation,
				int level,
				const struct skl_wm_params *wp,
				const struct skl_wm_level *result_prev,
				struct skl_wm_level *result /* out */)
{
	const struct drm_plane_state *pstate = &intel_pstate->base;
	uint32_t latency = dev_priv->wm.skl_latency[level];
	uint_fixed_16_16_t method1, method2;
	uint_fixed_16_16_t selected_result;
	uint32_t res_blocks, res_lines;
	struct intel_atomic_state *state =
		to_intel_atomic_state(cstate->base.state);
	bool apply_memory_bw_wa = skl_needs_memory_bw_wa(state);
	uint32_t min_disp_buf_needed;

	if (latency == 0 ||
	    !intel_wm_plane_visible(cstate, intel_pstate)) {
		result->plane_en = false;
		return 0;
	}

	/* Display WA #1141: kbl,cfl */
	if ((IS_KABYLAKE(dev_priv) || IS_COFFEELAKE(dev_priv) ||
	    IS_CNL_REVID(dev_priv, CNL_REVID_A0, CNL_REVID_B0)) &&
	    dev_priv->ipc_enabled)
		latency += 4;

	if (apply_memory_bw_wa && wp->x_tiled)
=======
static int
skl_compute_plane_wm_params(const struct intel_crtc_state *crtc_state,
			    const struct intel_plane_state *plane_state,
			    struct skl_wm_params *wp, int color_plane)
{
	const struct drm_framebuffer *fb = plane_state->hw.fb;
	int width;

	/*
	 * Src coordinates are already rotated by 270 degrees for
	 * the 90/270 degree plane rotation cases (to match the
	 * GTT mapping), hence no need to account for rotation here.
	 */
	width = drm_rect_width(&plane_state->uapi.src) >> 16;

	return skl_compute_wm_params(crtc_state, width,
				     fb->format, fb->modifier,
				     plane_state->hw.rotation,
				     intel_plane_pixel_rate(crtc_state, plane_state),
				     wp, color_plane);
}

static bool skl_wm_has_lines(struct drm_i915_private *dev_priv, int level)
{
	if (DISPLAY_VER(dev_priv) >= 10)
		return true;

	/* The number of lines are ignored for the level 0 watermark. */
	return level > 0;
}

static int skl_wm_max_lines(struct drm_i915_private *dev_priv)
{
	if (DISPLAY_VER(dev_priv) >= 13)
		return 255;
	else
		return 31;
}

static void skl_compute_plane_wm(const struct intel_crtc_state *crtc_state,
				 int level,
				 unsigned int latency,
				 const struct skl_wm_params *wp,
				 const struct skl_wm_level *result_prev,
				 struct skl_wm_level *result /* out */)
{
	struct drm_i915_private *dev_priv = to_i915(crtc_state->uapi.crtc->dev);
	uint_fixed_16_16_t method1, method2;
	uint_fixed_16_16_t selected_result;
	u32 blocks, lines, min_ddb_alloc = 0;

	if (latency == 0) {
		/* reject it */
		result->min_ddb_alloc = U16_MAX;
		return;
	}

	/*
	 * WaIncreaseLatencyIPCEnabled: kbl,cfl
	 * Display WA #1141: kbl,cfl
	 */
	if ((IS_KABYLAKE(dev_priv) ||
	     IS_COFFEELAKE(dev_priv) ||
	     IS_COMETLAKE(dev_priv)) &&
	    dev_priv->ipc_enabled)
		latency += 4;

	if (skl_needs_memory_bw_wa(dev_priv) && wp->x_tiled)
>>>>>>> upstream/android-13
		latency += 15;

	method1 = skl_wm_method1(dev_priv, wp->plane_pixel_rate,
				 wp->cpp, latency, wp->dbuf_block_size);
	method2 = skl_wm_method2(wp->plane_pixel_rate,
<<<<<<< HEAD
				 cstate->base.adjusted_mode.crtc_htotal,
=======
				 crtc_state->hw.pipe_mode.crtc_htotal,
>>>>>>> upstream/android-13
				 latency,
				 wp->plane_blocks_per_line);

	if (wp->y_tiled) {
		selected_result = max_fixed16(method2, wp->y_tile_minimum);
	} else {
<<<<<<< HEAD
		if ((wp->cpp * cstate->base.adjusted_mode.crtc_htotal /
		     wp->dbuf_block_size < 1) &&
		     (wp->plane_bytes_per_line / wp->dbuf_block_size < 1))
			selected_result = method2;
		else if (ddb_allocation >=
			 fixed16_to_u32_round_up(wp->plane_blocks_per_line))
			selected_result = min_fixed16(method1, method2);
		else if (latency >= wp->linetime_us)
			selected_result = min_fixed16(method1, method2);
		else
			selected_result = method1;
	}

	res_blocks = fixed16_to_u32_round_up(selected_result) + 1;
	res_lines = div_round_up_fixed16(selected_result,
					 wp->plane_blocks_per_line);

	/* Display WA #1125: skl,bxt,kbl,glk */
	if (level == 0 && wp->rc_surface)
		res_blocks += fixed16_to_u32_round_up(wp->y_tile_minimum);

	/* Display WA #1126: skl,bxt,kbl,glk */
	if (level >= 1 && level <= 7) {
		if (wp->y_tiled) {
			res_blocks += fixed16_to_u32_round_up(
							wp->y_tile_minimum);
			res_lines += wp->y_min_scanlines;
		} else {
			res_blocks++;
		}

		/*
		 * Make sure result blocks for higher latency levels are atleast
		 * as high as level below the current level.
		 * Assumption in DDB algorithm optimization for special cases.
		 * Also covers Display WA #1125 for RC.
		 */
		if (result_prev->plane_res_b > res_blocks)
			res_blocks = result_prev->plane_res_b;
	}

	if (INTEL_GEN(dev_priv) >= 11) {
		if (wp->y_tiled) {
			uint32_t extra_lines;
			uint_fixed_16_16_t fp_min_disp_buf_needed;

			if (res_lines % wp->y_min_scanlines == 0)
				extra_lines = wp->y_min_scanlines;
			else
				extra_lines = wp->y_min_scanlines * 2 -
					      res_lines % wp->y_min_scanlines;

			fp_min_disp_buf_needed = mul_u32_fixed16(res_lines +
						extra_lines,
						wp->plane_blocks_per_line);
			min_disp_buf_needed = fixed16_to_u32_round_up(
						fp_min_disp_buf_needed);
		} else {
			min_disp_buf_needed = DIV_ROUND_UP(res_blocks * 11, 10);
		}
	} else {
		min_disp_buf_needed = res_blocks;
	}

	if ((level > 0 && res_lines > 31) ||
	    res_blocks >= ddb_allocation ||
	    min_disp_buf_needed >= ddb_allocation) {
		result->plane_en = false;

		/*
		 * If there are no valid level 0 watermarks, then we can't
		 * support this display configuration.
		 */
		if (level) {
			return 0;
		} else {
			struct drm_plane *plane = pstate->plane;

			DRM_DEBUG_KMS("Requested display configuration exceeds system watermark limitations\n");
			DRM_DEBUG_KMS("[PLANE:%d:%s] blocks required = %u/%u, lines required = %u/31\n",
				      plane->base.id, plane->name,
				      res_blocks, ddb_allocation, res_lines);
			return -EINVAL;
		}
	}

	/*
	 * Display WA #826 (SKL:ALL, BXT:ALL) & #1059 (CNL:A)
	 * disable wm level 1-7 on NV12 planes
	 */
	if (wp->is_planar && level >= 1 &&
	    (IS_SKYLAKE(dev_priv) || IS_BROXTON(dev_priv) ||
	     IS_CNL_REVID(dev_priv, CNL_REVID_A0, CNL_REVID_A0))) {
		result->plane_en = false;
		return 0;
	}

	/* The number of lines are ignored for the level 0 watermark. */
	result->plane_res_b = res_blocks;
	result->plane_res_l = res_lines;
	result->plane_en = true;

	return 0;
}

static int
skl_compute_wm_levels(const struct drm_i915_private *dev_priv,
		      struct skl_ddb_allocation *ddb,
		      struct intel_crtc_state *cstate,
		      const struct intel_plane_state *intel_pstate,
		      const struct skl_wm_params *wm_params,
		      struct skl_plane_wm *wm,
		      int plane_id)
{
	struct intel_crtc *intel_crtc = to_intel_crtc(cstate->base.crtc);
	struct drm_plane *plane = intel_pstate->base.plane;
	struct intel_plane *intel_plane = to_intel_plane(plane);
	uint16_t ddb_blocks;
	enum pipe pipe = intel_crtc->pipe;
	int level, max_level = ilk_wm_max_level(dev_priv);
	enum plane_id intel_plane_id = intel_plane->id;
	int ret;

	if (WARN_ON(!intel_pstate->base.fb))
		return -EINVAL;

	ddb_blocks = plane_id ?
		     skl_ddb_entry_size(&ddb->uv_plane[pipe][intel_plane_id]) :
		     skl_ddb_entry_size(&ddb->plane[pipe][intel_plane_id]);

	for (level = 0; level <= max_level; level++) {
		struct skl_wm_level *result = plane_id ? &wm->uv_wm[level] :
							  &wm->wm[level];
		struct skl_wm_level *result_prev;

		if (level)
			result_prev = plane_id ? &wm->uv_wm[level - 1] :
						  &wm->wm[level - 1];
		else
			result_prev = plane_id ? &wm->uv_wm[0] : &wm->wm[0];

		ret = skl_compute_plane_wm(dev_priv,
					   cstate,
					   intel_pstate,
					   ddb_blocks,
					   level,
					   wm_params,
					   result_prev,
					   result);
		if (ret)
			return ret;
	}

	if (intel_pstate->base.fb->format->format == DRM_FORMAT_NV12)
		wm->is_planar = true;

	return 0;
}

static uint32_t
skl_compute_linetime_wm(struct intel_crtc_state *cstate)
{
	struct drm_atomic_state *state = cstate->base.state;
	struct drm_i915_private *dev_priv = to_i915(state->dev);
	uint_fixed_16_16_t linetime_us;
	uint32_t linetime_wm;

	linetime_us = intel_get_linetime_us(cstate);

	if (is_fixed16_zero(linetime_us))
		return 0;

	linetime_wm = fixed16_to_u32_round_up(mul_u32_fixed16(8, linetime_us));

	/* Display WA #1135: bxt:ALL GLK:ALL */
	if ((IS_BROXTON(dev_priv) || IS_GEMINILAKE(dev_priv)) &&
	    dev_priv->ipc_enabled)
		linetime_wm /= 2;

	return linetime_wm;
}

static void skl_compute_transition_wm(struct intel_crtc_state *cstate,
				      struct skl_wm_params *wp,
				      struct skl_wm_level *wm_l0,
				      uint16_t ddb_allocation,
				      struct skl_wm_level *trans_wm /* out */)
{
	struct drm_device *dev = cstate->base.crtc->dev;
	const struct drm_i915_private *dev_priv = to_i915(dev);
	uint16_t trans_min, trans_y_tile_min;
	const uint16_t trans_amount = 10; /* This is configurable amount */
	uint16_t trans_offset_b, res_blocks;

	if (!cstate->base.active)
		goto exit;

	/* Transition WM are not recommended by HW team for GEN9 */
	if (INTEL_GEN(dev_priv) <= 9)
		goto exit;

	/* Transition WM don't make any sense if ipc is disabled */
	if (!dev_priv->ipc_enabled)
		goto exit;

	trans_min = 0;
	if (INTEL_GEN(dev_priv) >= 10)
		trans_min = 4;

	trans_offset_b = trans_min + trans_amount;

	if (wp->y_tiled) {
		trans_y_tile_min = (uint16_t) mul_round_up_u32_fixed16(2,
							wp->y_tile_minimum);
		res_blocks = max(wm_l0->plane_res_b, trans_y_tile_min) +
				trans_offset_b;
	} else {
		res_blocks = wm_l0->plane_res_b + trans_offset_b;

		/* WA BUG:1938466 add one block for non y-tile planes */
		if (IS_CNL_REVID(dev_priv, CNL_REVID_A0, CNL_REVID_A0))
			res_blocks += 1;

	}

	res_blocks += 1;

	if (res_blocks < ddb_allocation) {
		trans_wm->plane_res_b = res_blocks;
		trans_wm->plane_en = true;
		return;
	}

exit:
	trans_wm->plane_en = false;
}

static int skl_build_pipe_wm(struct intel_crtc_state *cstate,
			     struct skl_ddb_allocation *ddb,
			     struct skl_pipe_wm *pipe_wm)
{
	struct drm_device *dev = cstate->base.crtc->dev;
	struct drm_crtc_state *crtc_state = &cstate->base;
	const struct drm_i915_private *dev_priv = to_i915(dev);
	struct drm_plane *plane;
	const struct drm_plane_state *pstate;
	struct skl_plane_wm *wm;
	int ret;

	/*
	 * We'll only calculate watermarks for planes that are actually
	 * enabled, so make sure all other planes are set as disabled.
	 */
	memset(pipe_wm->planes, 0, sizeof(pipe_wm->planes));

	drm_atomic_crtc_state_for_each_plane_state(plane, pstate, crtc_state) {
		const struct intel_plane_state *intel_pstate =
						to_intel_plane_state(pstate);
		enum plane_id plane_id = to_intel_plane(plane)->id;
		struct skl_wm_params wm_params;
		enum pipe pipe = to_intel_crtc(cstate->base.crtc)->pipe;
		uint16_t ddb_blocks;

		wm = &pipe_wm->planes[plane_id];
		ddb_blocks = skl_ddb_entry_size(&ddb->plane[pipe][plane_id]);

		ret = skl_compute_plane_wm_params(dev_priv, cstate,
						  intel_pstate, &wm_params, 0);
		if (ret)
			return ret;

		ret = skl_compute_wm_levels(dev_priv, ddb, cstate,
					    intel_pstate, &wm_params, wm, 0);
		if (ret)
			return ret;

		skl_compute_transition_wm(cstate, &wm_params, &wm->wm[0],
					  ddb_blocks, &wm->trans_wm);

		/* uv plane watermarks must also be validated for NV12/Planar */
		if (wm_params.is_planar) {
			memset(&wm_params, 0, sizeof(struct skl_wm_params));
			wm->is_planar = true;

			ret = skl_compute_plane_wm_params(dev_priv, cstate,
							  intel_pstate,
							  &wm_params, 1);
			if (ret)
				return ret;

			ret = skl_compute_wm_levels(dev_priv, ddb, cstate,
						    intel_pstate, &wm_params,
						    wm, 1);
			if (ret)
				return ret;
		}
	}

	pipe_wm->linetime = skl_compute_linetime_wm(cstate);
=======
		if ((wp->cpp * crtc_state->hw.pipe_mode.crtc_htotal /
		     wp->dbuf_block_size < 1) &&
		     (wp->plane_bytes_per_line / wp->dbuf_block_size < 1)) {
			selected_result = method2;
		} else if (latency >= wp->linetime_us) {
			if (DISPLAY_VER(dev_priv) == 9)
				selected_result = min_fixed16(method1, method2);
			else
				selected_result = method2;
		} else {
			selected_result = method1;
		}
	}

	blocks = fixed16_to_u32_round_up(selected_result) + 1;
	lines = div_round_up_fixed16(selected_result,
				     wp->plane_blocks_per_line);

	if (DISPLAY_VER(dev_priv) == 9) {
		/* Display WA #1125: skl,bxt,kbl */
		if (level == 0 && wp->rc_surface)
			blocks += fixed16_to_u32_round_up(wp->y_tile_minimum);

		/* Display WA #1126: skl,bxt,kbl */
		if (level >= 1 && level <= 7) {
			if (wp->y_tiled) {
				blocks += fixed16_to_u32_round_up(wp->y_tile_minimum);
				lines += wp->y_min_scanlines;
			} else {
				blocks++;
			}

			/*
			 * Make sure result blocks for higher latency levels are
			 * atleast as high as level below the current level.
			 * Assumption in DDB algorithm optimization for special
			 * cases. Also covers Display WA #1125 for RC.
			 */
			if (result_prev->blocks > blocks)
				blocks = result_prev->blocks;
		}
	}

	if (DISPLAY_VER(dev_priv) >= 11) {
		if (wp->y_tiled) {
			int extra_lines;

			if (lines % wp->y_min_scanlines == 0)
				extra_lines = wp->y_min_scanlines;
			else
				extra_lines = wp->y_min_scanlines * 2 -
					lines % wp->y_min_scanlines;

			min_ddb_alloc = mul_round_up_u32_fixed16(lines + extra_lines,
								 wp->plane_blocks_per_line);
		} else {
			min_ddb_alloc = blocks + DIV_ROUND_UP(blocks, 10);
		}
	}

	if (!skl_wm_has_lines(dev_priv, level))
		lines = 0;

	if (lines > skl_wm_max_lines(dev_priv)) {
		/* reject it */
		result->min_ddb_alloc = U16_MAX;
		return;
	}

	/*
	 * If lines is valid, assume we can use this watermark level
	 * for now.  We'll come back and disable it after we calculate the
	 * DDB allocation if it turns out we don't actually have enough
	 * blocks to satisfy it.
	 */
	result->blocks = blocks;
	result->lines = lines;
	/* Bspec says: value >= plane ddb allocation -> invalid, hence the +1 here */
	result->min_ddb_alloc = max(min_ddb_alloc, blocks) + 1;
	result->enable = true;

	if (DISPLAY_VER(dev_priv) < 12 && dev_priv->sagv_block_time_us)
		result->can_sagv = latency >= dev_priv->sagv_block_time_us;
}

static void
skl_compute_wm_levels(const struct intel_crtc_state *crtc_state,
		      const struct skl_wm_params *wm_params,
		      struct skl_wm_level *levels)
{
	struct drm_i915_private *dev_priv = to_i915(crtc_state->uapi.crtc->dev);
	int level, max_level = ilk_wm_max_level(dev_priv);
	struct skl_wm_level *result_prev = &levels[0];

	for (level = 0; level <= max_level; level++) {
		struct skl_wm_level *result = &levels[level];
		unsigned int latency = dev_priv->wm.skl_latency[level];

		skl_compute_plane_wm(crtc_state, level, latency,
				     wm_params, result_prev, result);

		result_prev = result;
	}
}

static void tgl_compute_sagv_wm(const struct intel_crtc_state *crtc_state,
				const struct skl_wm_params *wm_params,
				struct skl_plane_wm *plane_wm)
{
	struct drm_i915_private *dev_priv = to_i915(crtc_state->uapi.crtc->dev);
	struct skl_wm_level *sagv_wm = &plane_wm->sagv.wm0;
	struct skl_wm_level *levels = plane_wm->wm;
	unsigned int latency = 0;

	if (dev_priv->sagv_block_time_us)
		latency = dev_priv->sagv_block_time_us + dev_priv->wm.skl_latency[0];

	skl_compute_plane_wm(crtc_state, 0, latency,
			     wm_params, &levels[0],
			     sagv_wm);
}

static void skl_compute_transition_wm(struct drm_i915_private *dev_priv,
				      struct skl_wm_level *trans_wm,
				      const struct skl_wm_level *wm0,
				      const struct skl_wm_params *wp)
{
	u16 trans_min, trans_amount, trans_y_tile_min;
	u16 wm0_blocks, trans_offset, blocks;

	/* Transition WM don't make any sense if ipc is disabled */
	if (!dev_priv->ipc_enabled)
		return;

	/*
	 * WaDisableTWM:skl,kbl,cfl,bxt
	 * Transition WM are not recommended by HW team for GEN9
	 */
	if (DISPLAY_VER(dev_priv) == 9)
		return;

	if (DISPLAY_VER(dev_priv) >= 11)
		trans_min = 4;
	else
		trans_min = 14;

	/* Display WA #1140: glk,cnl */
	if (DISPLAY_VER(dev_priv) == 10)
		trans_amount = 0;
	else
		trans_amount = 10; /* This is configurable amount */

	trans_offset = trans_min + trans_amount;

	/*
	 * The spec asks for Selected Result Blocks for wm0 (the real value),
	 * not Result Blocks (the integer value). Pay attention to the capital
	 * letters. The value wm_l0->blocks is actually Result Blocks, but
	 * since Result Blocks is the ceiling of Selected Result Blocks plus 1,
	 * and since we later will have to get the ceiling of the sum in the
	 * transition watermarks calculation, we can just pretend Selected
	 * Result Blocks is Result Blocks minus 1 and it should work for the
	 * current platforms.
	 */
	wm0_blocks = wm0->blocks - 1;

	if (wp->y_tiled) {
		trans_y_tile_min =
			(u16)mul_round_up_u32_fixed16(2, wp->y_tile_minimum);
		blocks = max(wm0_blocks, trans_y_tile_min) + trans_offset;
	} else {
		blocks = wm0_blocks + trans_offset;
	}
	blocks++;

	/*
	 * Just assume we can enable the transition watermark.  After
	 * computing the DDB we'll come back and disable it if that
	 * assumption turns out to be false.
	 */
	trans_wm->blocks = blocks;
	trans_wm->min_ddb_alloc = max_t(u16, wm0->min_ddb_alloc, blocks + 1);
	trans_wm->enable = true;
}

static int skl_build_plane_wm_single(struct intel_crtc_state *crtc_state,
				     const struct intel_plane_state *plane_state,
				     enum plane_id plane_id, int color_plane)
{
	struct intel_crtc *crtc = to_intel_crtc(crtc_state->uapi.crtc);
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	struct skl_plane_wm *wm = &crtc_state->wm.skl.raw.planes[plane_id];
	struct skl_wm_params wm_params;
	int ret;

	ret = skl_compute_plane_wm_params(crtc_state, plane_state,
					  &wm_params, color_plane);
	if (ret)
		return ret;

	skl_compute_wm_levels(crtc_state, &wm_params, wm->wm);

	skl_compute_transition_wm(dev_priv, &wm->trans_wm,
				  &wm->wm[0], &wm_params);

	if (DISPLAY_VER(dev_priv) >= 12) {
		tgl_compute_sagv_wm(crtc_state, &wm_params, wm);

		skl_compute_transition_wm(dev_priv, &wm->sagv.trans_wm,
					  &wm->sagv.wm0, &wm_params);
	}

	return 0;
}

static int skl_build_plane_wm_uv(struct intel_crtc_state *crtc_state,
				 const struct intel_plane_state *plane_state,
				 enum plane_id plane_id)
{
	struct skl_plane_wm *wm = &crtc_state->wm.skl.raw.planes[plane_id];
	struct skl_wm_params wm_params;
	int ret;

	wm->is_planar = true;

	/* uv plane watermarks must also be validated for NV12/Planar */
	ret = skl_compute_plane_wm_params(crtc_state, plane_state,
					  &wm_params, 1);
	if (ret)
		return ret;

	skl_compute_wm_levels(crtc_state, &wm_params, wm->uv_wm);

	return 0;
}

static int skl_build_plane_wm(struct intel_crtc_state *crtc_state,
			      const struct intel_plane_state *plane_state)
{
	struct intel_plane *plane = to_intel_plane(plane_state->uapi.plane);
	enum plane_id plane_id = plane->id;
	struct skl_plane_wm *wm = &crtc_state->wm.skl.raw.planes[plane_id];
	const struct drm_framebuffer *fb = plane_state->hw.fb;
	int ret;

	memset(wm, 0, sizeof(*wm));

	if (!intel_wm_plane_visible(crtc_state, plane_state))
		return 0;

	ret = skl_build_plane_wm_single(crtc_state, plane_state,
					plane_id, 0);
	if (ret)
		return ret;

	if (fb->format->is_yuv && fb->format->num_planes > 1) {
		ret = skl_build_plane_wm_uv(crtc_state, plane_state,
					    plane_id);
		if (ret)
			return ret;
	}

	return 0;
}

static int icl_build_plane_wm(struct intel_crtc_state *crtc_state,
			      const struct intel_plane_state *plane_state)
{
	struct intel_plane *plane = to_intel_plane(plane_state->uapi.plane);
	struct drm_i915_private *dev_priv = to_i915(plane->base.dev);
	enum plane_id plane_id = plane->id;
	struct skl_plane_wm *wm = &crtc_state->wm.skl.raw.planes[plane_id];
	int ret;

	/* Watermarks calculated in master */
	if (plane_state->planar_slave)
		return 0;

	memset(wm, 0, sizeof(*wm));

	if (plane_state->planar_linked_plane) {
		const struct drm_framebuffer *fb = plane_state->hw.fb;
		enum plane_id y_plane_id = plane_state->planar_linked_plane->id;

		drm_WARN_ON(&dev_priv->drm,
			    !intel_wm_plane_visible(crtc_state, plane_state));
		drm_WARN_ON(&dev_priv->drm, !fb->format->is_yuv ||
			    fb->format->num_planes == 1);

		ret = skl_build_plane_wm_single(crtc_state, plane_state,
						y_plane_id, 0);
		if (ret)
			return ret;

		ret = skl_build_plane_wm_single(crtc_state, plane_state,
						plane_id, 1);
		if (ret)
			return ret;
	} else if (intel_wm_plane_visible(crtc_state, plane_state)) {
		ret = skl_build_plane_wm_single(crtc_state, plane_state,
						plane_id, 0);
		if (ret)
			return ret;
	}

	return 0;
}

static int skl_build_pipe_wm(struct intel_atomic_state *state,
			     struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
	const struct intel_plane_state *plane_state;
	struct intel_plane *plane;
	int ret, i;

	for_each_new_intel_plane_in_state(state, plane, plane_state, i) {
		/*
		 * FIXME should perhaps check {old,new}_plane_crtc->hw.crtc
		 * instead but we don't populate that correctly for NV12 Y
		 * planes so for now hack this.
		 */
		if (plane->pipe != crtc->pipe)
			continue;

		if (DISPLAY_VER(dev_priv) >= 11)
			ret = icl_build_plane_wm(crtc_state, plane_state);
		else
			ret = skl_build_plane_wm(crtc_state, plane_state);
		if (ret)
			return ret;
	}

	crtc_state->wm.skl.optimal = crtc_state->wm.skl.raw;
>>>>>>> upstream/android-13

	return 0;
}

static void skl_ddb_entry_write(struct drm_i915_private *dev_priv,
				i915_reg_t reg,
				const struct skl_ddb_entry *entry)
{
	if (entry->end)
<<<<<<< HEAD
		I915_WRITE(reg, (entry->end - 1) << 16 | entry->start);
	else
		I915_WRITE(reg, 0);
=======
		intel_de_write_fw(dev_priv, reg,
				  (entry->end - 1) << 16 | entry->start);
	else
		intel_de_write_fw(dev_priv, reg, 0);
>>>>>>> upstream/android-13
}

static void skl_write_wm_level(struct drm_i915_private *dev_priv,
			       i915_reg_t reg,
			       const struct skl_wm_level *level)
{
<<<<<<< HEAD
	uint32_t val = 0;

	if (level->plane_en) {
		val |= PLANE_WM_EN;
		val |= level->plane_res_b;
		val |= level->plane_res_l << PLANE_WM_LINES_SHIFT;
	}

	I915_WRITE(reg, val);
}

static void skl_write_plane_wm(struct intel_crtc *intel_crtc,
			       const struct skl_plane_wm *wm,
			       const struct skl_ddb_allocation *ddb,
			       enum plane_id plane_id)
{
	struct drm_crtc *crtc = &intel_crtc->base;
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = to_i915(dev);
	int level, max_level = ilk_wm_max_level(dev_priv);
	enum pipe pipe = intel_crtc->pipe;

	for (level = 0; level <= max_level; level++) {
		skl_write_wm_level(dev_priv, PLANE_WM(pipe, plane_id, level),
				   &wm->wm[level]);
	}
	skl_write_wm_level(dev_priv, PLANE_WM_TRANS(pipe, plane_id),
			   &wm->trans_wm);

	skl_ddb_entry_write(dev_priv, PLANE_BUF_CFG(pipe, plane_id),
			    &ddb->plane[pipe][plane_id]);
	if (INTEL_GEN(dev_priv) >= 11)
		return skl_ddb_entry_write(dev_priv,
					   PLANE_BUF_CFG(pipe, plane_id),
					   &ddb->plane[pipe][plane_id]);
	if (wm->is_planar) {
		skl_ddb_entry_write(dev_priv, PLANE_BUF_CFG(pipe, plane_id),
				    &ddb->uv_plane[pipe][plane_id]);
		skl_ddb_entry_write(dev_priv,
				    PLANE_NV12_BUF_CFG(pipe, plane_id),
				    &ddb->plane[pipe][plane_id]);
	} else {
		skl_ddb_entry_write(dev_priv, PLANE_BUF_CFG(pipe, plane_id),
				    &ddb->plane[pipe][plane_id]);
		I915_WRITE(PLANE_NV12_BUF_CFG(pipe, plane_id), 0x0);
	}
}

static void skl_write_cursor_wm(struct intel_crtc *intel_crtc,
				const struct skl_plane_wm *wm,
				const struct skl_ddb_allocation *ddb)
{
	struct drm_crtc *crtc = &intel_crtc->base;
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = to_i915(dev);
	int level, max_level = ilk_wm_max_level(dev_priv);
	enum pipe pipe = intel_crtc->pipe;

	for (level = 0; level <= max_level; level++) {
		skl_write_wm_level(dev_priv, CUR_WM(pipe, level),
				   &wm->wm[level]);
	}
	skl_write_wm_level(dev_priv, CUR_WM_TRANS(pipe), &wm->trans_wm);

	skl_ddb_entry_write(dev_priv, CUR_BUF_CFG(pipe),
			    &ddb->plane[pipe][PLANE_CURSOR]);
=======
	u32 val = 0;

	if (level->enable)
		val |= PLANE_WM_EN;
	if (level->ignore_lines)
		val |= PLANE_WM_IGNORE_LINES;
	val |= level->blocks;
	val |= REG_FIELD_PREP(PLANE_WM_LINES_MASK, level->lines);

	intel_de_write_fw(dev_priv, reg, val);
}

void skl_write_plane_wm(struct intel_plane *plane,
			const struct intel_crtc_state *crtc_state)
{
	struct drm_i915_private *dev_priv = to_i915(plane->base.dev);
	int level, max_level = ilk_wm_max_level(dev_priv);
	enum plane_id plane_id = plane->id;
	enum pipe pipe = plane->pipe;
	const struct skl_pipe_wm *pipe_wm = &crtc_state->wm.skl.optimal;
	const struct skl_plane_wm *wm = &pipe_wm->planes[plane_id];
	const struct skl_ddb_entry *ddb_y =
		&crtc_state->wm.skl.plane_ddb_y[plane_id];
	const struct skl_ddb_entry *ddb_uv =
		&crtc_state->wm.skl.plane_ddb_uv[plane_id];

	for (level = 0; level <= max_level; level++)
		skl_write_wm_level(dev_priv, PLANE_WM(pipe, plane_id, level),
				   skl_plane_wm_level(pipe_wm, plane_id, level));

	skl_write_wm_level(dev_priv, PLANE_WM_TRANS(pipe, plane_id),
			   skl_plane_trans_wm(pipe_wm, plane_id));

	if (HAS_HW_SAGV_WM(dev_priv)) {
		skl_write_wm_level(dev_priv, PLANE_WM_SAGV(pipe, plane_id),
				   &wm->sagv.wm0);
		skl_write_wm_level(dev_priv, PLANE_WM_SAGV_TRANS(pipe, plane_id),
				   &wm->sagv.trans_wm);
	}

	if (DISPLAY_VER(dev_priv) >= 11) {
		skl_ddb_entry_write(dev_priv,
				    PLANE_BUF_CFG(pipe, plane_id), ddb_y);
		return;
	}

	if (wm->is_planar)
		swap(ddb_y, ddb_uv);

	skl_ddb_entry_write(dev_priv,
			    PLANE_BUF_CFG(pipe, plane_id), ddb_y);
	skl_ddb_entry_write(dev_priv,
			    PLANE_NV12_BUF_CFG(pipe, plane_id), ddb_uv);
}

void skl_write_cursor_wm(struct intel_plane *plane,
			 const struct intel_crtc_state *crtc_state)
{
	struct drm_i915_private *dev_priv = to_i915(plane->base.dev);
	int level, max_level = ilk_wm_max_level(dev_priv);
	enum plane_id plane_id = plane->id;
	enum pipe pipe = plane->pipe;
	const struct skl_pipe_wm *pipe_wm = &crtc_state->wm.skl.optimal;
	const struct skl_ddb_entry *ddb =
		&crtc_state->wm.skl.plane_ddb_y[plane_id];

	for (level = 0; level <= max_level; level++)
		skl_write_wm_level(dev_priv, CUR_WM(pipe, level),
				   skl_plane_wm_level(pipe_wm, plane_id, level));

	skl_write_wm_level(dev_priv, CUR_WM_TRANS(pipe),
			   skl_plane_trans_wm(pipe_wm, plane_id));

	if (HAS_HW_SAGV_WM(dev_priv)) {
		const struct skl_plane_wm *wm = &pipe_wm->planes[plane_id];

		skl_write_wm_level(dev_priv, CUR_WM_SAGV(pipe),
				   &wm->sagv.wm0);
		skl_write_wm_level(dev_priv, CUR_WM_SAGV_TRANS(pipe),
				   &wm->sagv.trans_wm);
	}

	skl_ddb_entry_write(dev_priv, CUR_BUF_CFG(pipe), ddb);
>>>>>>> upstream/android-13
}

bool skl_wm_level_equals(const struct skl_wm_level *l1,
			 const struct skl_wm_level *l2)
{
<<<<<<< HEAD
	if (l1->plane_en != l2->plane_en)
		return false;

	/* If both planes aren't enabled, the rest shouldn't matter */
	if (!l1->plane_en)
		return true;

	return (l1->plane_res_l == l2->plane_res_l &&
		l1->plane_res_b == l2->plane_res_b);
}

static inline bool skl_ddb_entries_overlap(const struct skl_ddb_entry *a,
					   const struct skl_ddb_entry *b)
=======
	return l1->enable == l2->enable &&
		l1->ignore_lines == l2->ignore_lines &&
		l1->lines == l2->lines &&
		l1->blocks == l2->blocks;
}

static bool skl_plane_wm_equals(struct drm_i915_private *dev_priv,
				const struct skl_plane_wm *wm1,
				const struct skl_plane_wm *wm2)
{
	int level, max_level = ilk_wm_max_level(dev_priv);

	for (level = 0; level <= max_level; level++) {
		/*
		 * We don't check uv_wm as the hardware doesn't actually
		 * use it. It only gets used for calculating the required
		 * ddb allocation.
		 */
		if (!skl_wm_level_equals(&wm1->wm[level], &wm2->wm[level]))
			return false;
	}

	return skl_wm_level_equals(&wm1->trans_wm, &wm2->trans_wm) &&
		skl_wm_level_equals(&wm1->sagv.wm0, &wm2->sagv.wm0) &&
		skl_wm_level_equals(&wm1->sagv.trans_wm, &wm2->sagv.trans_wm);
}

static bool skl_ddb_entries_overlap(const struct skl_ddb_entry *a,
				    const struct skl_ddb_entry *b)
>>>>>>> upstream/android-13
{
	return a->start < b->end && b->start < a->end;
}

<<<<<<< HEAD
bool skl_ddb_allocation_overlaps(struct drm_i915_private *dev_priv,
				 const struct skl_ddb_entry **entries,
				 const struct skl_ddb_entry *ddb,
				 int ignore)
{
	enum pipe pipe;

	for_each_pipe(dev_priv, pipe) {
		if (pipe != ignore && entries[pipe] &&
		    skl_ddb_entries_overlap(ddb, entries[pipe]))
=======
static void skl_ddb_entry_union(struct skl_ddb_entry *a,
				const struct skl_ddb_entry *b)
{
	if (a->end && b->end) {
		a->start = min(a->start, b->start);
		a->end = max(a->end, b->end);
	} else if (b->end) {
		a->start = b->start;
		a->end = b->end;
	}
}

bool skl_ddb_allocation_overlaps(const struct skl_ddb_entry *ddb,
				 const struct skl_ddb_entry *entries,
				 int num_entries, int ignore_idx)
{
	int i;

	for (i = 0; i < num_entries; i++) {
		if (i != ignore_idx &&
		    skl_ddb_entries_overlap(ddb, &entries[i]))
>>>>>>> upstream/android-13
			return true;
	}

	return false;
}

<<<<<<< HEAD
static int skl_update_pipe_wm(struct drm_crtc_state *cstate,
			      const struct skl_pipe_wm *old_pipe_wm,
			      struct skl_pipe_wm *pipe_wm, /* out */
			      struct skl_ddb_allocation *ddb, /* out */
			      bool *changed /* out */)
{
	struct intel_crtc_state *intel_cstate = to_intel_crtc_state(cstate);
	int ret;

	ret = skl_build_pipe_wm(intel_cstate, ddb, pipe_wm);
	if (ret)
		return ret;

	if (!memcmp(old_pipe_wm, pipe_wm, sizeof(*pipe_wm)))
		*changed = false;
	else
		*changed = true;

	return 0;
}

static uint32_t
pipes_modified(struct drm_atomic_state *state)
{
	struct drm_crtc *crtc;
	struct drm_crtc_state *cstate;
	uint32_t i, ret = 0;

	for_each_new_crtc_in_state(state, crtc, cstate, i)
		ret |= drm_crtc_mask(crtc);

	return ret;
}

static int
skl_ddb_add_affected_planes(struct intel_crtc_state *cstate)
{
	struct drm_atomic_state *state = cstate->base.state;
	struct drm_device *dev = state->dev;
	struct drm_crtc *crtc = cstate->base.crtc;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct intel_atomic_state *intel_state = to_intel_atomic_state(state);
	struct skl_ddb_allocation *new_ddb = &intel_state->wm_results.ddb;
	struct skl_ddb_allocation *cur_ddb = &dev_priv->wm.skl_hw.ddb;
	struct drm_plane_state *plane_state;
	struct drm_plane *plane;
	enum pipe pipe = intel_crtc->pipe;

	drm_for_each_plane_mask(plane, dev, cstate->base.plane_mask) {
		enum plane_id plane_id = to_intel_plane(plane)->id;

		if (skl_ddb_entry_equal(&cur_ddb->plane[pipe][plane_id],
					&new_ddb->plane[pipe][plane_id]) &&
		    skl_ddb_entry_equal(&cur_ddb->uv_plane[pipe][plane_id],
					&new_ddb->uv_plane[pipe][plane_id]))
			continue;

		plane_state = drm_atomic_get_plane_state(state, plane);
		if (IS_ERR(plane_state))
			return PTR_ERR(plane_state);
=======
static int
skl_ddb_add_affected_planes(const struct intel_crtc_state *old_crtc_state,
			    struct intel_crtc_state *new_crtc_state)
{
	struct intel_atomic_state *state = to_intel_atomic_state(new_crtc_state->uapi.state);
	struct intel_crtc *crtc = to_intel_crtc(new_crtc_state->uapi.crtc);
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	struct intel_plane *plane;

	for_each_intel_plane_on_crtc(&dev_priv->drm, crtc, plane) {
		struct intel_plane_state *plane_state;
		enum plane_id plane_id = plane->id;

		if (skl_ddb_entry_equal(&old_crtc_state->wm.skl.plane_ddb_y[plane_id],
					&new_crtc_state->wm.skl.plane_ddb_y[plane_id]) &&
		    skl_ddb_entry_equal(&old_crtc_state->wm.skl.plane_ddb_uv[plane_id],
					&new_crtc_state->wm.skl.plane_ddb_uv[plane_id]))
			continue;

		plane_state = intel_atomic_get_plane_state(state, plane);
		if (IS_ERR(plane_state))
			return PTR_ERR(plane_state);

		new_crtc_state->update_planes |= BIT(plane_id);
>>>>>>> upstream/android-13
	}

	return 0;
}

<<<<<<< HEAD
static int
skl_compute_ddb(struct drm_atomic_state *state)
{
	const struct drm_i915_private *dev_priv = to_i915(state->dev);
	struct intel_atomic_state *intel_state = to_intel_atomic_state(state);
	struct skl_ddb_allocation *ddb = &intel_state->wm_results.ddb;
	struct intel_crtc *crtc;
	struct intel_crtc_state *cstate;
	int ret, i;

	memcpy(ddb, &dev_priv->wm.skl_hw.ddb, sizeof(*ddb));

	for_each_new_intel_crtc_in_state(intel_state, crtc, cstate, i) {
		ret = skl_allocate_pipe_ddb(cstate, ddb);
		if (ret)
			return ret;

		ret = skl_ddb_add_affected_planes(cstate);
=======
static u8 intel_dbuf_enabled_slices(const struct intel_dbuf_state *dbuf_state)
{
	struct drm_i915_private *dev_priv = to_i915(dbuf_state->base.state->base.dev);
	u8 enabled_slices;
	enum pipe pipe;

	/*
	 * FIXME: For now we always enable slice S1 as per
	 * the Bspec display initialization sequence.
	 */
	enabled_slices = BIT(DBUF_S1);

	for_each_pipe(dev_priv, pipe)
		enabled_slices |= dbuf_state->slices[pipe];

	return enabled_slices;
}

static int
skl_compute_ddb(struct intel_atomic_state *state)
{
	struct drm_i915_private *dev_priv = to_i915(state->base.dev);
	const struct intel_dbuf_state *old_dbuf_state;
	struct intel_dbuf_state *new_dbuf_state = NULL;
	const struct intel_crtc_state *old_crtc_state;
	struct intel_crtc_state *new_crtc_state;
	struct intel_crtc *crtc;
	int ret, i;

	for_each_new_intel_crtc_in_state(state, crtc, new_crtc_state, i) {
		new_dbuf_state = intel_atomic_get_dbuf_state(state);
		if (IS_ERR(new_dbuf_state))
			return PTR_ERR(new_dbuf_state);

		old_dbuf_state = intel_atomic_get_old_dbuf_state(state);
		break;
	}

	if (!new_dbuf_state)
		return 0;

	new_dbuf_state->active_pipes =
		intel_calc_active_pipes(state, old_dbuf_state->active_pipes);

	if (old_dbuf_state->active_pipes != new_dbuf_state->active_pipes) {
		ret = intel_atomic_lock_global_state(&new_dbuf_state->base);
		if (ret)
			return ret;
	}

	if (IS_ALDERLAKE_P(dev_priv))
		new_dbuf_state->joined_mbus =
			adlp_check_mbus_joined(new_dbuf_state->active_pipes);

	for_each_intel_crtc(&dev_priv->drm, crtc) {
		enum pipe pipe = crtc->pipe;

		new_dbuf_state->slices[pipe] =
			skl_compute_dbuf_slices(crtc, new_dbuf_state->active_pipes,
						new_dbuf_state->joined_mbus);

		if (old_dbuf_state->slices[pipe] == new_dbuf_state->slices[pipe])
			continue;

		ret = intel_atomic_lock_global_state(&new_dbuf_state->base);
		if (ret)
			return ret;
	}

	new_dbuf_state->enabled_slices = intel_dbuf_enabled_slices(new_dbuf_state);

	if (old_dbuf_state->enabled_slices != new_dbuf_state->enabled_slices ||
	    old_dbuf_state->joined_mbus != new_dbuf_state->joined_mbus) {
		ret = intel_atomic_serialize_global_state(&new_dbuf_state->base);
		if (ret)
			return ret;

		if (old_dbuf_state->joined_mbus != new_dbuf_state->joined_mbus) {
			/* TODO: Implement vblank synchronized MBUS joining changes */
			ret = intel_modeset_all_pipes(state);
			if (ret)
				return ret;
		}

		drm_dbg_kms(&dev_priv->drm,
			    "Enabled dbuf slices 0x%x -> 0x%x (total dbuf slices 0x%x), mbus joined? %s->%s\n",
			    old_dbuf_state->enabled_slices,
			    new_dbuf_state->enabled_slices,
			    INTEL_INFO(dev_priv)->dbuf.slice_mask,
			    yesno(old_dbuf_state->joined_mbus),
			    yesno(new_dbuf_state->joined_mbus));
	}

	for_each_new_intel_crtc_in_state(state, crtc, new_crtc_state, i) {
		enum pipe pipe = crtc->pipe;

		new_dbuf_state->weight[pipe] = intel_crtc_ddb_weight(new_crtc_state);

		if (old_dbuf_state->weight[pipe] == new_dbuf_state->weight[pipe])
			continue;

		ret = intel_atomic_lock_global_state(&new_dbuf_state->base);
		if (ret)
			return ret;
	}

	for_each_intel_crtc(&dev_priv->drm, crtc) {
		ret = skl_crtc_allocate_ddb(state, crtc);
		if (ret)
			return ret;
	}

	for_each_oldnew_intel_crtc_in_state(state, crtc, old_crtc_state,
					    new_crtc_state, i) {
		ret = skl_allocate_plane_ddb(state, crtc);
		if (ret)
			return ret;

		ret = skl_ddb_add_affected_planes(old_crtc_state,
						  new_crtc_state);
>>>>>>> upstream/android-13
		if (ret)
			return ret;
	}

	return 0;
}

<<<<<<< HEAD
static void
skl_copy_ddb_for_pipe(struct skl_ddb_values *dst,
		      struct skl_ddb_values *src,
		      enum pipe pipe)
{
	memcpy(dst->ddb.uv_plane[pipe], src->ddb.uv_plane[pipe],
	       sizeof(dst->ddb.uv_plane[pipe]));
	memcpy(dst->ddb.plane[pipe], src->ddb.plane[pipe],
	       sizeof(dst->ddb.plane[pipe]));
}

static void
skl_print_wm_changes(const struct drm_atomic_state *state)
{
	const struct drm_device *dev = state->dev;
	const struct drm_i915_private *dev_priv = to_i915(dev);
	const struct intel_atomic_state *intel_state =
		to_intel_atomic_state(state);
	const struct drm_crtc *crtc;
	const struct drm_crtc_state *cstate;
	const struct intel_plane *intel_plane;
	const struct skl_ddb_allocation *old_ddb = &dev_priv->wm.skl_hw.ddb;
	const struct skl_ddb_allocation *new_ddb = &intel_state->wm_results.ddb;
	int i;

	for_each_new_crtc_in_state(state, crtc, cstate, i) {
		const struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
		enum pipe pipe = intel_crtc->pipe;

		for_each_intel_plane_on_crtc(dev, intel_crtc, intel_plane) {
			enum plane_id plane_id = intel_plane->id;
			const struct skl_ddb_entry *old, *new;

			old = &old_ddb->plane[pipe][plane_id];
			new = &new_ddb->plane[pipe][plane_id];
=======
static char enast(bool enable)
{
	return enable ? '*' : ' ';
}

static void
skl_print_wm_changes(struct intel_atomic_state *state)
{
	struct drm_i915_private *dev_priv = to_i915(state->base.dev);
	const struct intel_crtc_state *old_crtc_state;
	const struct intel_crtc_state *new_crtc_state;
	struct intel_plane *plane;
	struct intel_crtc *crtc;
	int i;

	if (!drm_debug_enabled(DRM_UT_KMS))
		return;

	for_each_oldnew_intel_crtc_in_state(state, crtc, old_crtc_state,
					    new_crtc_state, i) {
		const struct skl_pipe_wm *old_pipe_wm, *new_pipe_wm;

		old_pipe_wm = &old_crtc_state->wm.skl.optimal;
		new_pipe_wm = &new_crtc_state->wm.skl.optimal;

		for_each_intel_plane_on_crtc(&dev_priv->drm, crtc, plane) {
			enum plane_id plane_id = plane->id;
			const struct skl_ddb_entry *old, *new;

			old = &old_crtc_state->wm.skl.plane_ddb_y[plane_id];
			new = &new_crtc_state->wm.skl.plane_ddb_y[plane_id];
>>>>>>> upstream/android-13

			if (skl_ddb_entry_equal(old, new))
				continue;

<<<<<<< HEAD
			DRM_DEBUG_ATOMIC("[PLANE:%d:%s] ddb (%d - %d) -> (%d - %d)\n",
					 intel_plane->base.base.id,
					 intel_plane->base.name,
					 old->start, old->end,
					 new->start, new->end);
=======
			drm_dbg_kms(&dev_priv->drm,
				    "[PLANE:%d:%s] ddb (%4d - %4d) -> (%4d - %4d), size %4d -> %4d\n",
				    plane->base.base.id, plane->base.name,
				    old->start, old->end, new->start, new->end,
				    skl_ddb_entry_size(old), skl_ddb_entry_size(new));
		}

		for_each_intel_plane_on_crtc(&dev_priv->drm, crtc, plane) {
			enum plane_id plane_id = plane->id;
			const struct skl_plane_wm *old_wm, *new_wm;

			old_wm = &old_pipe_wm->planes[plane_id];
			new_wm = &new_pipe_wm->planes[plane_id];

			if (skl_plane_wm_equals(dev_priv, old_wm, new_wm))
				continue;

			drm_dbg_kms(&dev_priv->drm,
				    "[PLANE:%d:%s]   level %cwm0,%cwm1,%cwm2,%cwm3,%cwm4,%cwm5,%cwm6,%cwm7,%ctwm,%cswm,%cstwm"
				    " -> %cwm0,%cwm1,%cwm2,%cwm3,%cwm4,%cwm5,%cwm6,%cwm7,%ctwm,%cswm,%cstwm\n",
				    plane->base.base.id, plane->base.name,
				    enast(old_wm->wm[0].enable), enast(old_wm->wm[1].enable),
				    enast(old_wm->wm[2].enable), enast(old_wm->wm[3].enable),
				    enast(old_wm->wm[4].enable), enast(old_wm->wm[5].enable),
				    enast(old_wm->wm[6].enable), enast(old_wm->wm[7].enable),
				    enast(old_wm->trans_wm.enable),
				    enast(old_wm->sagv.wm0.enable),
				    enast(old_wm->sagv.trans_wm.enable),
				    enast(new_wm->wm[0].enable), enast(new_wm->wm[1].enable),
				    enast(new_wm->wm[2].enable), enast(new_wm->wm[3].enable),
				    enast(new_wm->wm[4].enable), enast(new_wm->wm[5].enable),
				    enast(new_wm->wm[6].enable), enast(new_wm->wm[7].enable),
				    enast(new_wm->trans_wm.enable),
				    enast(new_wm->sagv.wm0.enable),
				    enast(new_wm->sagv.trans_wm.enable));

			drm_dbg_kms(&dev_priv->drm,
				    "[PLANE:%d:%s]   lines %c%3d,%c%3d,%c%3d,%c%3d,%c%3d,%c%3d,%c%3d,%c%3d,%c%3d,%c%3d,%c%4d"
				      " -> %c%3d,%c%3d,%c%3d,%c%3d,%c%3d,%c%3d,%c%3d,%c%3d,%c%3d,%c%3d,%c%4d\n",
				    plane->base.base.id, plane->base.name,
				    enast(old_wm->wm[0].ignore_lines), old_wm->wm[0].lines,
				    enast(old_wm->wm[1].ignore_lines), old_wm->wm[1].lines,
				    enast(old_wm->wm[2].ignore_lines), old_wm->wm[2].lines,
				    enast(old_wm->wm[3].ignore_lines), old_wm->wm[3].lines,
				    enast(old_wm->wm[4].ignore_lines), old_wm->wm[4].lines,
				    enast(old_wm->wm[5].ignore_lines), old_wm->wm[5].lines,
				    enast(old_wm->wm[6].ignore_lines), old_wm->wm[6].lines,
				    enast(old_wm->wm[7].ignore_lines), old_wm->wm[7].lines,
				    enast(old_wm->trans_wm.ignore_lines), old_wm->trans_wm.lines,
				    enast(old_wm->sagv.wm0.ignore_lines), old_wm->sagv.wm0.lines,
				    enast(old_wm->sagv.trans_wm.ignore_lines), old_wm->sagv.trans_wm.lines,
				    enast(new_wm->wm[0].ignore_lines), new_wm->wm[0].lines,
				    enast(new_wm->wm[1].ignore_lines), new_wm->wm[1].lines,
				    enast(new_wm->wm[2].ignore_lines), new_wm->wm[2].lines,
				    enast(new_wm->wm[3].ignore_lines), new_wm->wm[3].lines,
				    enast(new_wm->wm[4].ignore_lines), new_wm->wm[4].lines,
				    enast(new_wm->wm[5].ignore_lines), new_wm->wm[5].lines,
				    enast(new_wm->wm[6].ignore_lines), new_wm->wm[6].lines,
				    enast(new_wm->wm[7].ignore_lines), new_wm->wm[7].lines,
				    enast(new_wm->trans_wm.ignore_lines), new_wm->trans_wm.lines,
				    enast(new_wm->sagv.wm0.ignore_lines), new_wm->sagv.wm0.lines,
				    enast(new_wm->sagv.trans_wm.ignore_lines), new_wm->sagv.trans_wm.lines);

			drm_dbg_kms(&dev_priv->drm,
				    "[PLANE:%d:%s]  blocks %4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%5d"
				    " -> %4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%5d\n",
				    plane->base.base.id, plane->base.name,
				    old_wm->wm[0].blocks, old_wm->wm[1].blocks,
				    old_wm->wm[2].blocks, old_wm->wm[3].blocks,
				    old_wm->wm[4].blocks, old_wm->wm[5].blocks,
				    old_wm->wm[6].blocks, old_wm->wm[7].blocks,
				    old_wm->trans_wm.blocks,
				    old_wm->sagv.wm0.blocks,
				    old_wm->sagv.trans_wm.blocks,
				    new_wm->wm[0].blocks, new_wm->wm[1].blocks,
				    new_wm->wm[2].blocks, new_wm->wm[3].blocks,
				    new_wm->wm[4].blocks, new_wm->wm[5].blocks,
				    new_wm->wm[6].blocks, new_wm->wm[7].blocks,
				    new_wm->trans_wm.blocks,
				    new_wm->sagv.wm0.blocks,
				    new_wm->sagv.trans_wm.blocks);

			drm_dbg_kms(&dev_priv->drm,
				    "[PLANE:%d:%s] min_ddb %4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%5d"
				    " -> %4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%5d\n",
				    plane->base.base.id, plane->base.name,
				    old_wm->wm[0].min_ddb_alloc, old_wm->wm[1].min_ddb_alloc,
				    old_wm->wm[2].min_ddb_alloc, old_wm->wm[3].min_ddb_alloc,
				    old_wm->wm[4].min_ddb_alloc, old_wm->wm[5].min_ddb_alloc,
				    old_wm->wm[6].min_ddb_alloc, old_wm->wm[7].min_ddb_alloc,
				    old_wm->trans_wm.min_ddb_alloc,
				    old_wm->sagv.wm0.min_ddb_alloc,
				    old_wm->sagv.trans_wm.min_ddb_alloc,
				    new_wm->wm[0].min_ddb_alloc, new_wm->wm[1].min_ddb_alloc,
				    new_wm->wm[2].min_ddb_alloc, new_wm->wm[3].min_ddb_alloc,
				    new_wm->wm[4].min_ddb_alloc, new_wm->wm[5].min_ddb_alloc,
				    new_wm->wm[6].min_ddb_alloc, new_wm->wm[7].min_ddb_alloc,
				    new_wm->trans_wm.min_ddb_alloc,
				    new_wm->sagv.wm0.min_ddb_alloc,
				    new_wm->sagv.trans_wm.min_ddb_alloc);
>>>>>>> upstream/android-13
		}
	}
}

<<<<<<< HEAD
static int
skl_ddb_add_affected_pipes(struct drm_atomic_state *state, bool *changed)
{
	struct drm_device *dev = state->dev;
	const struct drm_i915_private *dev_priv = to_i915(dev);
	const struct drm_crtc *crtc;
	const struct drm_crtc_state *cstate;
	struct intel_crtc *intel_crtc;
	struct intel_atomic_state *intel_state = to_intel_atomic_state(state);
	uint32_t realloc_pipes = pipes_modified(state);
	int ret, i;

	/*
	 * When we distrust bios wm we always need to recompute to set the
	 * expected DDB allocations for each CRTC.
	 */
	if (dev_priv->wm.distrust_bios_wm)
		(*changed) = true;

	/*
	 * If this transaction isn't actually touching any CRTC's, don't
	 * bother with watermark calculation.  Note that if we pass this
	 * test, we're guaranteed to hold at least one CRTC state mutex,
	 * which means we can safely use values like dev_priv->active_crtcs
	 * since any racing commits that want to update them would need to
	 * hold _all_ CRTC state mutexes.
	 */
	for_each_new_crtc_in_state(state, crtc, cstate, i)
		(*changed) = true;

	if (!*changed)
		return 0;

	/*
	 * If this is our first atomic update following hardware readout,
	 * we can't trust the DDB that the BIOS programmed for us.  Let's
	 * pretend that all pipes switched active status so that we'll
	 * ensure a full DDB recompute.
	 */
	if (dev_priv->wm.distrust_bios_wm) {
		ret = drm_modeset_lock(&dev->mode_config.connection_mutex,
				       state->acquire_ctx);
		if (ret)
			return ret;

		intel_state->active_pipe_changes = ~0;

		/*
		 * We usually only initialize intel_state->active_crtcs if we
		 * we're doing a modeset; make sure this field is always
		 * initialized during the sanitization process that happens
		 * on the first commit too.
		 */
		if (!intel_state->modeset)
			intel_state->active_crtcs = dev_priv->active_crtcs;
	}

	/*
	 * If the modeset changes which CRTC's are active, we need to
	 * recompute the DDB allocation for *all* active pipes, even
	 * those that weren't otherwise being modified in any way by this
	 * atomic commit.  Due to the shrinking of the per-pipe allocations
	 * when new active CRTC's are added, it's possible for a pipe that
	 * we were already using and aren't changing at all here to suddenly
	 * become invalid if its DDB needs exceeds its new allocation.
	 *
	 * Note that if we wind up doing a full DDB recompute, we can't let
	 * any other display updates race with this transaction, so we need
	 * to grab the lock on *all* CRTC's.
	 */
	if (intel_state->active_pipe_changes) {
		realloc_pipes = ~0;
		intel_state->wm_results.dirty_pipes = ~0;
	}

	/*
	 * We're not recomputing for the pipes not included in the commit, so
	 * make sure we start with the current state.
	 */
	for_each_intel_crtc_mask(dev, intel_crtc, realloc_pipes) {
		struct intel_crtc_state *cstate;

		cstate = intel_atomic_get_crtc_state(state, intel_crtc);
		if (IS_ERR(cstate))
			return PTR_ERR(cstate);
=======
static bool skl_plane_selected_wm_equals(struct intel_plane *plane,
					 const struct skl_pipe_wm *old_pipe_wm,
					 const struct skl_pipe_wm *new_pipe_wm)
{
	struct drm_i915_private *i915 = to_i915(plane->base.dev);
	int level, max_level = ilk_wm_max_level(i915);

	for (level = 0; level <= max_level; level++) {
		/*
		 * We don't check uv_wm as the hardware doesn't actually
		 * use it. It only gets used for calculating the required
		 * ddb allocation.
		 */
		if (!skl_wm_level_equals(skl_plane_wm_level(old_pipe_wm, plane->id, level),
					 skl_plane_wm_level(new_pipe_wm, plane->id, level)))
			return false;
	}

	if (HAS_HW_SAGV_WM(i915)) {
		const struct skl_plane_wm *old_wm = &old_pipe_wm->planes[plane->id];
		const struct skl_plane_wm *new_wm = &new_pipe_wm->planes[plane->id];

		if (!skl_wm_level_equals(&old_wm->sagv.wm0, &new_wm->sagv.wm0) ||
		    !skl_wm_level_equals(&old_wm->sagv.trans_wm, &new_wm->sagv.trans_wm))
			return false;
	}

	return skl_wm_level_equals(skl_plane_trans_wm(old_pipe_wm, plane->id),
				   skl_plane_trans_wm(new_pipe_wm, plane->id));
}

/*
 * To make sure the cursor watermark registers are always consistent
 * with our computed state the following scenario needs special
 * treatment:
 *
 * 1. enable cursor
 * 2. move cursor entirely offscreen
 * 3. disable cursor
 *
 * Step 2. does call .disable_plane() but does not zero the watermarks
 * (since we consider an offscreen cursor still active for the purposes
 * of watermarks). Step 3. would not normally call .disable_plane()
 * because the actual plane visibility isn't changing, and we don't
 * deallocate the cursor ddb until the pipe gets disabled. So we must
 * force step 3. to call .disable_plane() to update the watermark
 * registers properly.
 *
 * Other planes do not suffer from this issues as their watermarks are
 * calculated based on the actual plane visibility. The only time this
 * can trigger for the other planes is during the initial readout as the
 * default value of the watermarks registers is not zero.
 */
static int skl_wm_add_affected_planes(struct intel_atomic_state *state,
				      struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	const struct intel_crtc_state *old_crtc_state =
		intel_atomic_get_old_crtc_state(state, crtc);
	struct intel_crtc_state *new_crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);
	struct intel_plane *plane;

	for_each_intel_plane_on_crtc(&dev_priv->drm, crtc, plane) {
		struct intel_plane_state *plane_state;
		enum plane_id plane_id = plane->id;

		/*
		 * Force a full wm update for every plane on modeset.
		 * Required because the reset value of the wm registers
		 * is non-zero, whereas we want all disabled planes to
		 * have zero watermarks. So if we turn off the relevant
		 * power well the hardware state will go out of sync
		 * with the software state.
		 */
		if (!drm_atomic_crtc_needs_modeset(&new_crtc_state->uapi) &&
		    skl_plane_selected_wm_equals(plane,
						 &old_crtc_state->wm.skl.optimal,
						 &new_crtc_state->wm.skl.optimal))
			continue;

		plane_state = intel_atomic_get_plane_state(state, plane);
		if (IS_ERR(plane_state))
			return PTR_ERR(plane_state);

		new_crtc_state->update_planes |= BIT(plane_id);
>>>>>>> upstream/android-13
	}

	return 0;
}

static int
<<<<<<< HEAD
skl_compute_wm(struct drm_atomic_state *state)
{
	struct drm_crtc *crtc;
	struct drm_crtc_state *cstate;
	struct intel_atomic_state *intel_state = to_intel_atomic_state(state);
	struct skl_ddb_values *results = &intel_state->wm_results;
	struct skl_pipe_wm *pipe_wm;
	bool changed = false;
	int ret, i;

	/* Clear all dirty flags */
	results->dirty_pipes = 0;

	ret = skl_ddb_add_affected_pipes(state, &changed);
	if (ret || !changed)
		return ret;
=======
skl_compute_wm(struct intel_atomic_state *state)
{
	struct intel_crtc *crtc;
	struct intel_crtc_state *new_crtc_state;
	int ret, i;

	for_each_new_intel_crtc_in_state(state, crtc, new_crtc_state, i) {
		ret = skl_build_pipe_wm(state, crtc);
		if (ret)
			return ret;
	}
>>>>>>> upstream/android-13

	ret = skl_compute_ddb(state);
	if (ret)
		return ret;

<<<<<<< HEAD
	/*
	 * Calculate WM's for all pipes that are part of this transaction.
	 * Note that the DDB allocation above may have added more CRTC's that
	 * weren't otherwise being modified (and set bits in dirty_pipes) if
	 * pipe allocations had to change.
	 *
	 * FIXME:  Now that we're doing this in the atomic check phase, we
	 * should allow skl_update_pipe_wm() to return failure in cases where
	 * no suitable watermark values can be found.
	 */
	for_each_new_crtc_in_state(state, crtc, cstate, i) {
		struct intel_crtc_state *intel_cstate =
			to_intel_crtc_state(cstate);
		const struct skl_pipe_wm *old_pipe_wm =
			&to_intel_crtc_state(crtc->state)->wm.skl.optimal;

		pipe_wm = &intel_cstate->wm.skl.optimal;
		ret = skl_update_pipe_wm(cstate, old_pipe_wm, pipe_wm,
					 &results->ddb, &changed);
		if (ret)
			return ret;

		if (changed)
			results->dirty_pipes |= drm_crtc_mask(crtc);

		if ((results->dirty_pipes & drm_crtc_mask(crtc)) == 0)
			/* This pipe's WM's did not change */
			continue;

		intel_cstate->update_wm_pre = true;
=======
	ret = intel_compute_sagv_mask(state);
	if (ret)
		return ret;

	/*
	 * skl_compute_ddb() will have adjusted the final watermarks
	 * based on how much ddb is available. Now we can actually
	 * check if the final watermarks changed.
	 */
	for_each_new_intel_crtc_in_state(state, crtc, new_crtc_state, i) {
		ret = skl_wm_add_affected_planes(state, crtc);
		if (ret)
			return ret;
>>>>>>> upstream/android-13
	}

	skl_print_wm_changes(state);

	return 0;
}

<<<<<<< HEAD
static void skl_atomic_update_crtc_wm(struct intel_atomic_state *state,
				      struct intel_crtc_state *cstate)
{
	struct intel_crtc *crtc = to_intel_crtc(cstate->base.crtc);
	struct drm_i915_private *dev_priv = to_i915(state->base.dev);
	struct skl_pipe_wm *pipe_wm = &cstate->wm.skl.optimal;
	const struct skl_ddb_allocation *ddb = &state->wm_results.ddb;
	enum pipe pipe = crtc->pipe;
	enum plane_id plane_id;

	if (!(state->wm_results.dirty_pipes & drm_crtc_mask(&crtc->base)))
		return;

	I915_WRITE(PIPE_WM_LINETIME(pipe), pipe_wm->linetime);

	for_each_plane_id_on_crtc(crtc, plane_id) {
		if (plane_id != PLANE_CURSOR)
			skl_write_plane_wm(crtc, &pipe_wm->planes[plane_id],
					   ddb, plane_id);
		else
			skl_write_cursor_wm(crtc, &pipe_wm->planes[plane_id],
					    ddb);
	}
}

static void skl_initial_wm(struct intel_atomic_state *state,
			   struct intel_crtc_state *cstate)
{
	struct intel_crtc *intel_crtc = to_intel_crtc(cstate->base.crtc);
	struct drm_device *dev = intel_crtc->base.dev;
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct skl_ddb_values *results = &state->wm_results;
	struct skl_ddb_values *hw_vals = &dev_priv->wm.skl_hw;
	enum pipe pipe = intel_crtc->pipe;

	if ((results->dirty_pipes & drm_crtc_mask(&intel_crtc->base)) == 0)
		return;

	mutex_lock(&dev_priv->wm.wm_mutex);

	if (cstate->base.active_changed)
		skl_atomic_update_crtc_wm(state, cstate);

	skl_copy_ddb_for_pipe(hw_vals, results, pipe);

	mutex_unlock(&dev_priv->wm.wm_mutex);
}

static void ilk_compute_wm_config(struct drm_device *dev,
=======
static void ilk_compute_wm_config(struct drm_i915_private *dev_priv,
>>>>>>> upstream/android-13
				  struct intel_wm_config *config)
{
	struct intel_crtc *crtc;

	/* Compute the currently _active_ config */
<<<<<<< HEAD
	for_each_intel_crtc(dev, crtc) {
=======
	for_each_intel_crtc(&dev_priv->drm, crtc) {
>>>>>>> upstream/android-13
		const struct intel_pipe_wm *wm = &crtc->wm.active.ilk;

		if (!wm->pipe_enabled)
			continue;

		config->sprites_enabled |= wm->sprites_enabled;
		config->sprites_scaled |= wm->sprites_scaled;
		config->num_pipes_active++;
	}
}

static void ilk_program_watermarks(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	struct drm_device *dev = &dev_priv->drm;
=======
>>>>>>> upstream/android-13
	struct intel_pipe_wm lp_wm_1_2 = {}, lp_wm_5_6 = {}, *best_lp_wm;
	struct ilk_wm_maximums max;
	struct intel_wm_config config = {};
	struct ilk_wm_values results = {};
	enum intel_ddb_partitioning partitioning;

<<<<<<< HEAD
	ilk_compute_wm_config(dev, &config);

	ilk_compute_wm_maximums(dev, 1, &config, INTEL_DDB_PART_1_2, &max);
	ilk_wm_merge(dev, &config, &max, &lp_wm_1_2);

	/* 5/6 split only in single pipe config on IVB+ */
	if (INTEL_GEN(dev_priv) >= 7 &&
	    config.num_pipes_active == 1 && config.sprites_enabled) {
		ilk_compute_wm_maximums(dev, 1, &config, INTEL_DDB_PART_5_6, &max);
		ilk_wm_merge(dev, &config, &max, &lp_wm_5_6);

		best_lp_wm = ilk_find_best_result(dev, &lp_wm_1_2, &lp_wm_5_6);
=======
	ilk_compute_wm_config(dev_priv, &config);

	ilk_compute_wm_maximums(dev_priv, 1, &config, INTEL_DDB_PART_1_2, &max);
	ilk_wm_merge(dev_priv, &config, &max, &lp_wm_1_2);

	/* 5/6 split only in single pipe config on IVB+ */
	if (DISPLAY_VER(dev_priv) >= 7 &&
	    config.num_pipes_active == 1 && config.sprites_enabled) {
		ilk_compute_wm_maximums(dev_priv, 1, &config, INTEL_DDB_PART_5_6, &max);
		ilk_wm_merge(dev_priv, &config, &max, &lp_wm_5_6);

		best_lp_wm = ilk_find_best_result(dev_priv, &lp_wm_1_2, &lp_wm_5_6);
>>>>>>> upstream/android-13
	} else {
		best_lp_wm = &lp_wm_1_2;
	}

	partitioning = (best_lp_wm == &lp_wm_1_2) ?
		       INTEL_DDB_PART_1_2 : INTEL_DDB_PART_5_6;

<<<<<<< HEAD
	ilk_compute_wm_results(dev, best_lp_wm, partitioning, &results);
=======
	ilk_compute_wm_results(dev_priv, best_lp_wm, partitioning, &results);
>>>>>>> upstream/android-13

	ilk_write_wm_values(dev_priv, &results);
}

static void ilk_initial_watermarks(struct intel_atomic_state *state,
<<<<<<< HEAD
				   struct intel_crtc_state *cstate)
{
	struct drm_i915_private *dev_priv = to_i915(cstate->base.crtc->dev);
	struct intel_crtc *intel_crtc = to_intel_crtc(cstate->base.crtc);

	mutex_lock(&dev_priv->wm.wm_mutex);
	intel_crtc->wm.active.ilk = cstate->wm.ilk.intermediate;
=======
				   struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	const struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);

	mutex_lock(&dev_priv->wm.wm_mutex);
	crtc->wm.active.ilk = crtc_state->wm.ilk.intermediate;
>>>>>>> upstream/android-13
	ilk_program_watermarks(dev_priv);
	mutex_unlock(&dev_priv->wm.wm_mutex);
}

static void ilk_optimize_watermarks(struct intel_atomic_state *state,
<<<<<<< HEAD
				    struct intel_crtc_state *cstate)
{
	struct drm_i915_private *dev_priv = to_i915(cstate->base.crtc->dev);
	struct intel_crtc *intel_crtc = to_intel_crtc(cstate->base.crtc);

	mutex_lock(&dev_priv->wm.wm_mutex);
	if (cstate->wm.need_postvbl_update) {
		intel_crtc->wm.active.ilk = cstate->wm.ilk.optimal;
		ilk_program_watermarks(dev_priv);
	}
	mutex_unlock(&dev_priv->wm.wm_mutex);
}

static inline void skl_wm_level_from_reg_val(uint32_t val,
					     struct skl_wm_level *level)
{
	level->plane_en = val & PLANE_WM_EN;
	level->plane_res_b = val & PLANE_WM_BLOCKS_MASK;
	level->plane_res_l = (val >> PLANE_WM_LINES_SHIFT) &
		PLANE_WM_LINES_MASK;
}

void skl_pipe_wm_get_hw_state(struct drm_crtc *crtc,
			      struct skl_pipe_wm *out)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->dev);
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	enum pipe pipe = intel_crtc->pipe;
	int level, max_level;
	enum plane_id plane_id;
	uint32_t val;

	max_level = ilk_wm_max_level(dev_priv);

	for_each_plane_id_on_crtc(intel_crtc, plane_id) {
=======
				    struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	const struct intel_crtc_state *crtc_state =
		intel_atomic_get_new_crtc_state(state, crtc);

	if (!crtc_state->wm.need_postvbl_update)
		return;

	mutex_lock(&dev_priv->wm.wm_mutex);
	crtc->wm.active.ilk = crtc_state->wm.ilk.optimal;
	ilk_program_watermarks(dev_priv);
	mutex_unlock(&dev_priv->wm.wm_mutex);
}

static void skl_wm_level_from_reg_val(u32 val, struct skl_wm_level *level)
{
	level->enable = val & PLANE_WM_EN;
	level->ignore_lines = val & PLANE_WM_IGNORE_LINES;
	level->blocks = val & PLANE_WM_BLOCKS_MASK;
	level->lines = REG_FIELD_GET(PLANE_WM_LINES_MASK, val);
}

void skl_pipe_wm_get_hw_state(struct intel_crtc *crtc,
			      struct skl_pipe_wm *out)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);
	enum pipe pipe = crtc->pipe;
	int level, max_level;
	enum plane_id plane_id;
	u32 val;

	max_level = ilk_wm_max_level(dev_priv);

	for_each_plane_id_on_crtc(crtc, plane_id) {
>>>>>>> upstream/android-13
		struct skl_plane_wm *wm = &out->planes[plane_id];

		for (level = 0; level <= max_level; level++) {
			if (plane_id != PLANE_CURSOR)
<<<<<<< HEAD
				val = I915_READ(PLANE_WM(pipe, plane_id, level));
			else
				val = I915_READ(CUR_WM(pipe, level));
=======
				val = intel_uncore_read(&dev_priv->uncore, PLANE_WM(pipe, plane_id, level));
			else
				val = intel_uncore_read(&dev_priv->uncore, CUR_WM(pipe, level));
>>>>>>> upstream/android-13

			skl_wm_level_from_reg_val(val, &wm->wm[level]);
		}

		if (plane_id != PLANE_CURSOR)
<<<<<<< HEAD
			val = I915_READ(PLANE_WM_TRANS(pipe, plane_id));
		else
			val = I915_READ(CUR_WM_TRANS(pipe));

		skl_wm_level_from_reg_val(val, &wm->trans_wm);
	}

	if (!intel_crtc->active)
		return;

	out->linetime = I915_READ(PIPE_WM_LINETIME(pipe));
}

void skl_wm_get_hw_state(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct skl_ddb_values *hw = &dev_priv->wm.skl_hw;
	struct skl_ddb_allocation *ddb = &dev_priv->wm.skl_hw.ddb;
	struct drm_crtc *crtc;
	struct intel_crtc *intel_crtc;
	struct intel_crtc_state *cstate;

	skl_ddb_get_hw_state(dev_priv, ddb);
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		intel_crtc = to_intel_crtc(crtc);
		cstate = to_intel_crtc_state(crtc->state);

		skl_pipe_wm_get_hw_state(crtc, &cstate->wm.skl.optimal);

		if (intel_crtc->active)
			hw->dirty_pipes |= drm_crtc_mask(crtc);
	}

	if (dev_priv->active_crtcs) {
		/* Fully recompute DDB on first atomic commit */
		dev_priv->wm.distrust_bios_wm = true;
	} else {
		/*
		 * Easy/common case; just sanitize DDB now if everything off
		 * Keep dbuf slice info intact
		 */
		memset(ddb->plane, 0, sizeof(ddb->plane));
		memset(ddb->uv_plane, 0, sizeof(ddb->uv_plane));
	}
}

static void ilk_pipe_wm_get_hw_state(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct ilk_wm_values *hw = &dev_priv->wm.hw;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_crtc_state *cstate = to_intel_crtc_state(crtc->state);
	struct intel_pipe_wm *active = &cstate->wm.ilk.optimal;
	enum pipe pipe = intel_crtc->pipe;
	static const i915_reg_t wm0_pipe_reg[] = {
		[PIPE_A] = WM0_PIPEA_ILK,
		[PIPE_B] = WM0_PIPEB_ILK,
		[PIPE_C] = WM0_PIPEC_IVB,
	};

	hw->wm_pipe[pipe] = I915_READ(wm0_pipe_reg[pipe]);
	if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv))
		hw->wm_linetime[pipe] = I915_READ(PIPE_WM_LINETIME(pipe));

	memset(active, 0, sizeof(*active));

	active->pipe_enabled = intel_crtc->active;
=======
			val = intel_uncore_read(&dev_priv->uncore, PLANE_WM_TRANS(pipe, plane_id));
		else
			val = intel_uncore_read(&dev_priv->uncore, CUR_WM_TRANS(pipe));

		skl_wm_level_from_reg_val(val, &wm->trans_wm);

		if (HAS_HW_SAGV_WM(dev_priv)) {
			if (plane_id != PLANE_CURSOR)
				val = intel_uncore_read(&dev_priv->uncore,
							PLANE_WM_SAGV(pipe, plane_id));
			else
				val = intel_uncore_read(&dev_priv->uncore,
							CUR_WM_SAGV(pipe));

			skl_wm_level_from_reg_val(val, &wm->sagv.wm0);

			if (plane_id != PLANE_CURSOR)
				val = intel_uncore_read(&dev_priv->uncore,
							PLANE_WM_SAGV_TRANS(pipe, plane_id));
			else
				val = intel_uncore_read(&dev_priv->uncore,
							CUR_WM_SAGV_TRANS(pipe));

			skl_wm_level_from_reg_val(val, &wm->sagv.trans_wm);
		} else if (DISPLAY_VER(dev_priv) >= 12) {
			wm->sagv.wm0 = wm->wm[0];
			wm->sagv.trans_wm = wm->trans_wm;
		}
	}
}

void skl_wm_get_hw_state(struct drm_i915_private *dev_priv)
{
	struct intel_dbuf_state *dbuf_state =
		to_intel_dbuf_state(dev_priv->dbuf.obj.state);
	struct intel_crtc *crtc;

	if (IS_ALDERLAKE_P(dev_priv))
		dbuf_state->joined_mbus = intel_de_read(dev_priv, MBUS_CTL) & MBUS_JOIN;

	for_each_intel_crtc(&dev_priv->drm, crtc) {
		struct intel_crtc_state *crtc_state =
			to_intel_crtc_state(crtc->base.state);
		enum pipe pipe = crtc->pipe;
		unsigned int mbus_offset;
		enum plane_id plane_id;
		u8 slices;

		skl_pipe_wm_get_hw_state(crtc, &crtc_state->wm.skl.optimal);
		crtc_state->wm.skl.raw = crtc_state->wm.skl.optimal;

		memset(&dbuf_state->ddb[pipe], 0, sizeof(dbuf_state->ddb[pipe]));

		for_each_plane_id_on_crtc(crtc, plane_id) {
			struct skl_ddb_entry *ddb_y =
				&crtc_state->wm.skl.plane_ddb_y[plane_id];
			struct skl_ddb_entry *ddb_uv =
				&crtc_state->wm.skl.plane_ddb_uv[plane_id];

			skl_ddb_get_hw_plane_state(dev_priv, crtc->pipe,
						   plane_id, ddb_y, ddb_uv);

			skl_ddb_entry_union(&dbuf_state->ddb[pipe], ddb_y);
			skl_ddb_entry_union(&dbuf_state->ddb[pipe], ddb_uv);
		}

		dbuf_state->weight[pipe] = intel_crtc_ddb_weight(crtc_state);

		/*
		 * Used for checking overlaps, so we need absolute
		 * offsets instead of MBUS relative offsets.
		 */
		slices = skl_compute_dbuf_slices(crtc, dbuf_state->active_pipes,
						 dbuf_state->joined_mbus);
		mbus_offset = mbus_ddb_offset(dev_priv, slices);
		crtc_state->wm.skl.ddb.start = mbus_offset + dbuf_state->ddb[pipe].start;
		crtc_state->wm.skl.ddb.end = mbus_offset + dbuf_state->ddb[pipe].end;

		/* The slices actually used by the planes on the pipe */
		dbuf_state->slices[pipe] =
			skl_ddb_dbuf_slice_mask(dev_priv, &crtc_state->wm.skl.ddb);

		drm_dbg_kms(&dev_priv->drm,
			    "[CRTC:%d:%s] dbuf slices 0x%x, ddb (%d - %d), active pipes 0x%x, mbus joined: %s\n",
			    crtc->base.base.id, crtc->base.name,
			    dbuf_state->slices[pipe], dbuf_state->ddb[pipe].start,
			    dbuf_state->ddb[pipe].end, dbuf_state->active_pipes,
			    yesno(dbuf_state->joined_mbus));
	}

	dbuf_state->enabled_slices = dev_priv->dbuf.enabled_slices;
}

static bool skl_dbuf_is_misconfigured(struct drm_i915_private *i915)
{
	const struct intel_dbuf_state *dbuf_state =
		to_intel_dbuf_state(i915->dbuf.obj.state);
	struct skl_ddb_entry entries[I915_MAX_PIPES] = {};
	struct intel_crtc *crtc;

	for_each_intel_crtc(&i915->drm, crtc) {
		const struct intel_crtc_state *crtc_state =
			to_intel_crtc_state(crtc->base.state);

		entries[crtc->pipe] = crtc_state->wm.skl.ddb;
	}

	for_each_intel_crtc(&i915->drm, crtc) {
		const struct intel_crtc_state *crtc_state =
			to_intel_crtc_state(crtc->base.state);
		u8 slices;

		slices = skl_compute_dbuf_slices(crtc, dbuf_state->active_pipes,
						 dbuf_state->joined_mbus);
		if (dbuf_state->slices[crtc->pipe] & ~slices)
			return true;

		if (skl_ddb_allocation_overlaps(&crtc_state->wm.skl.ddb, entries,
						I915_MAX_PIPES, crtc->pipe))
			return true;
	}

	return false;
}

void skl_wm_sanitize(struct drm_i915_private *i915)
{
	struct intel_crtc *crtc;

	/*
	 * On TGL/RKL (at least) the BIOS likes to assign the planes
	 * to the wrong DBUF slices. This will cause an infinite loop
	 * in skl_commit_modeset_enables() as it can't find a way to
	 * transition between the old bogus DBUF layout to the new
	 * proper DBUF layout without DBUF allocation overlaps between
	 * the planes (which cannot be allowed or else the hardware
	 * may hang). If we detect a bogus DBUF layout just turn off
	 * all the planes so that skl_commit_modeset_enables() can
	 * simply ignore them.
	 */
	if (!skl_dbuf_is_misconfigured(i915))
		return;

	drm_dbg_kms(&i915->drm, "BIOS has misprogrammed the DBUF, disabling all planes\n");

	for_each_intel_crtc(&i915->drm, crtc) {
		struct intel_plane *plane = to_intel_plane(crtc->base.primary);
		const struct intel_plane_state *plane_state =
			to_intel_plane_state(plane->base.state);
		struct intel_crtc_state *crtc_state =
			to_intel_crtc_state(crtc->base.state);

		if (plane_state->uapi.visible)
			intel_plane_disable_noatomic(crtc, plane);

		drm_WARN_ON(&i915->drm, crtc_state->active_planes != 0);

		memset(&crtc_state->wm.skl.ddb, 0, sizeof(crtc_state->wm.skl.ddb));
	}
}

static void ilk_pipe_wm_get_hw_state(struct intel_crtc *crtc)
{
	struct drm_device *dev = crtc->base.dev;
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct ilk_wm_values *hw = &dev_priv->wm.hw;
	struct intel_crtc_state *crtc_state = to_intel_crtc_state(crtc->base.state);
	struct intel_pipe_wm *active = &crtc_state->wm.ilk.optimal;
	enum pipe pipe = crtc->pipe;

	hw->wm_pipe[pipe] = intel_uncore_read(&dev_priv->uncore, WM0_PIPE_ILK(pipe));

	memset(active, 0, sizeof(*active));

	active->pipe_enabled = crtc->active;
>>>>>>> upstream/android-13

	if (active->pipe_enabled) {
		u32 tmp = hw->wm_pipe[pipe];

		/*
		 * For active pipes LP0 watermark is marked as
		 * enabled, and LP1+ watermaks as disabled since
		 * we can't really reverse compute them in case
		 * multiple pipes are active.
		 */
		active->wm[0].enable = true;
		active->wm[0].pri_val = (tmp & WM0_PIPE_PLANE_MASK) >> WM0_PIPE_PLANE_SHIFT;
		active->wm[0].spr_val = (tmp & WM0_PIPE_SPRITE_MASK) >> WM0_PIPE_SPRITE_SHIFT;
		active->wm[0].cur_val = tmp & WM0_PIPE_CURSOR_MASK;
<<<<<<< HEAD
		active->linetime = hw->wm_linetime[pipe];
=======
>>>>>>> upstream/android-13
	} else {
		int level, max_level = ilk_wm_max_level(dev_priv);

		/*
		 * For inactive pipes, all watermark levels
		 * should be marked as enabled but zeroed,
		 * which is what we'd compute them to.
		 */
		for (level = 0; level <= max_level; level++)
			active->wm[level].enable = true;
	}

<<<<<<< HEAD
	intel_crtc->wm.active.ilk = *active;
=======
	crtc->wm.active.ilk = *active;
>>>>>>> upstream/android-13
}

#define _FW_WM(value, plane) \
	(((value) & DSPFW_ ## plane ## _MASK) >> DSPFW_ ## plane ## _SHIFT)
#define _FW_WM_VLV(value, plane) \
	(((value) & DSPFW_ ## plane ## _MASK_VLV) >> DSPFW_ ## plane ## _SHIFT)

static void g4x_read_wm_values(struct drm_i915_private *dev_priv,
			       struct g4x_wm_values *wm)
{
<<<<<<< HEAD
	uint32_t tmp;

	tmp = I915_READ(DSPFW1);
=======
	u32 tmp;

	tmp = intel_uncore_read(&dev_priv->uncore, DSPFW1);
>>>>>>> upstream/android-13
	wm->sr.plane = _FW_WM(tmp, SR);
	wm->pipe[PIPE_B].plane[PLANE_CURSOR] = _FW_WM(tmp, CURSORB);
	wm->pipe[PIPE_B].plane[PLANE_PRIMARY] = _FW_WM(tmp, PLANEB);
	wm->pipe[PIPE_A].plane[PLANE_PRIMARY] = _FW_WM(tmp, PLANEA);

<<<<<<< HEAD
	tmp = I915_READ(DSPFW2);
=======
	tmp = intel_uncore_read(&dev_priv->uncore, DSPFW2);
>>>>>>> upstream/android-13
	wm->fbc_en = tmp & DSPFW_FBC_SR_EN;
	wm->sr.fbc = _FW_WM(tmp, FBC_SR);
	wm->hpll.fbc = _FW_WM(tmp, FBC_HPLL_SR);
	wm->pipe[PIPE_B].plane[PLANE_SPRITE0] = _FW_WM(tmp, SPRITEB);
	wm->pipe[PIPE_A].plane[PLANE_CURSOR] = _FW_WM(tmp, CURSORA);
	wm->pipe[PIPE_A].plane[PLANE_SPRITE0] = _FW_WM(tmp, SPRITEA);

<<<<<<< HEAD
	tmp = I915_READ(DSPFW3);
=======
	tmp = intel_uncore_read(&dev_priv->uncore, DSPFW3);
>>>>>>> upstream/android-13
	wm->hpll_en = tmp & DSPFW_HPLL_SR_EN;
	wm->sr.cursor = _FW_WM(tmp, CURSOR_SR);
	wm->hpll.cursor = _FW_WM(tmp, HPLL_CURSOR);
	wm->hpll.plane = _FW_WM(tmp, HPLL_SR);
}

static void vlv_read_wm_values(struct drm_i915_private *dev_priv,
			       struct vlv_wm_values *wm)
{
	enum pipe pipe;
<<<<<<< HEAD
	uint32_t tmp;

	for_each_pipe(dev_priv, pipe) {
		tmp = I915_READ(VLV_DDL(pipe));
=======
	u32 tmp;

	for_each_pipe(dev_priv, pipe) {
		tmp = intel_uncore_read(&dev_priv->uncore, VLV_DDL(pipe));
>>>>>>> upstream/android-13

		wm->ddl[pipe].plane[PLANE_PRIMARY] =
			(tmp >> DDL_PLANE_SHIFT) & (DDL_PRECISION_HIGH | DRAIN_LATENCY_MASK);
		wm->ddl[pipe].plane[PLANE_CURSOR] =
			(tmp >> DDL_CURSOR_SHIFT) & (DDL_PRECISION_HIGH | DRAIN_LATENCY_MASK);
		wm->ddl[pipe].plane[PLANE_SPRITE0] =
			(tmp >> DDL_SPRITE_SHIFT(0)) & (DDL_PRECISION_HIGH | DRAIN_LATENCY_MASK);
		wm->ddl[pipe].plane[PLANE_SPRITE1] =
			(tmp >> DDL_SPRITE_SHIFT(1)) & (DDL_PRECISION_HIGH | DRAIN_LATENCY_MASK);
	}

<<<<<<< HEAD
	tmp = I915_READ(DSPFW1);
=======
	tmp = intel_uncore_read(&dev_priv->uncore, DSPFW1);
>>>>>>> upstream/android-13
	wm->sr.plane = _FW_WM(tmp, SR);
	wm->pipe[PIPE_B].plane[PLANE_CURSOR] = _FW_WM(tmp, CURSORB);
	wm->pipe[PIPE_B].plane[PLANE_PRIMARY] = _FW_WM_VLV(tmp, PLANEB);
	wm->pipe[PIPE_A].plane[PLANE_PRIMARY] = _FW_WM_VLV(tmp, PLANEA);

<<<<<<< HEAD
	tmp = I915_READ(DSPFW2);
=======
	tmp = intel_uncore_read(&dev_priv->uncore, DSPFW2);
>>>>>>> upstream/android-13
	wm->pipe[PIPE_A].plane[PLANE_SPRITE1] = _FW_WM_VLV(tmp, SPRITEB);
	wm->pipe[PIPE_A].plane[PLANE_CURSOR] = _FW_WM(tmp, CURSORA);
	wm->pipe[PIPE_A].plane[PLANE_SPRITE0] = _FW_WM_VLV(tmp, SPRITEA);

<<<<<<< HEAD
	tmp = I915_READ(DSPFW3);
	wm->sr.cursor = _FW_WM(tmp, CURSOR_SR);

	if (IS_CHERRYVIEW(dev_priv)) {
		tmp = I915_READ(DSPFW7_CHV);
		wm->pipe[PIPE_B].plane[PLANE_SPRITE1] = _FW_WM_VLV(tmp, SPRITED);
		wm->pipe[PIPE_B].plane[PLANE_SPRITE0] = _FW_WM_VLV(tmp, SPRITEC);

		tmp = I915_READ(DSPFW8_CHV);
		wm->pipe[PIPE_C].plane[PLANE_SPRITE1] = _FW_WM_VLV(tmp, SPRITEF);
		wm->pipe[PIPE_C].plane[PLANE_SPRITE0] = _FW_WM_VLV(tmp, SPRITEE);

		tmp = I915_READ(DSPFW9_CHV);
		wm->pipe[PIPE_C].plane[PLANE_PRIMARY] = _FW_WM_VLV(tmp, PLANEC);
		wm->pipe[PIPE_C].plane[PLANE_CURSOR] = _FW_WM(tmp, CURSORC);

		tmp = I915_READ(DSPHOWM);
=======
	tmp = intel_uncore_read(&dev_priv->uncore, DSPFW3);
	wm->sr.cursor = _FW_WM(tmp, CURSOR_SR);

	if (IS_CHERRYVIEW(dev_priv)) {
		tmp = intel_uncore_read(&dev_priv->uncore, DSPFW7_CHV);
		wm->pipe[PIPE_B].plane[PLANE_SPRITE1] = _FW_WM_VLV(tmp, SPRITED);
		wm->pipe[PIPE_B].plane[PLANE_SPRITE0] = _FW_WM_VLV(tmp, SPRITEC);

		tmp = intel_uncore_read(&dev_priv->uncore, DSPFW8_CHV);
		wm->pipe[PIPE_C].plane[PLANE_SPRITE1] = _FW_WM_VLV(tmp, SPRITEF);
		wm->pipe[PIPE_C].plane[PLANE_SPRITE0] = _FW_WM_VLV(tmp, SPRITEE);

		tmp = intel_uncore_read(&dev_priv->uncore, DSPFW9_CHV);
		wm->pipe[PIPE_C].plane[PLANE_PRIMARY] = _FW_WM_VLV(tmp, PLANEC);
		wm->pipe[PIPE_C].plane[PLANE_CURSOR] = _FW_WM(tmp, CURSORC);

		tmp = intel_uncore_read(&dev_priv->uncore, DSPHOWM);
>>>>>>> upstream/android-13
		wm->sr.plane |= _FW_WM(tmp, SR_HI) << 9;
		wm->pipe[PIPE_C].plane[PLANE_SPRITE1] |= _FW_WM(tmp, SPRITEF_HI) << 8;
		wm->pipe[PIPE_C].plane[PLANE_SPRITE0] |= _FW_WM(tmp, SPRITEE_HI) << 8;
		wm->pipe[PIPE_C].plane[PLANE_PRIMARY] |= _FW_WM(tmp, PLANEC_HI) << 8;
		wm->pipe[PIPE_B].plane[PLANE_SPRITE1] |= _FW_WM(tmp, SPRITED_HI) << 8;
		wm->pipe[PIPE_B].plane[PLANE_SPRITE0] |= _FW_WM(tmp, SPRITEC_HI) << 8;
		wm->pipe[PIPE_B].plane[PLANE_PRIMARY] |= _FW_WM(tmp, PLANEB_HI) << 8;
		wm->pipe[PIPE_A].plane[PLANE_SPRITE1] |= _FW_WM(tmp, SPRITEB_HI) << 8;
		wm->pipe[PIPE_A].plane[PLANE_SPRITE0] |= _FW_WM(tmp, SPRITEA_HI) << 8;
		wm->pipe[PIPE_A].plane[PLANE_PRIMARY] |= _FW_WM(tmp, PLANEA_HI) << 8;
	} else {
<<<<<<< HEAD
		tmp = I915_READ(DSPFW7);
		wm->pipe[PIPE_B].plane[PLANE_SPRITE1] = _FW_WM_VLV(tmp, SPRITED);
		wm->pipe[PIPE_B].plane[PLANE_SPRITE0] = _FW_WM_VLV(tmp, SPRITEC);

		tmp = I915_READ(DSPHOWM);
=======
		tmp = intel_uncore_read(&dev_priv->uncore, DSPFW7);
		wm->pipe[PIPE_B].plane[PLANE_SPRITE1] = _FW_WM_VLV(tmp, SPRITED);
		wm->pipe[PIPE_B].plane[PLANE_SPRITE0] = _FW_WM_VLV(tmp, SPRITEC);

		tmp = intel_uncore_read(&dev_priv->uncore, DSPHOWM);
>>>>>>> upstream/android-13
		wm->sr.plane |= _FW_WM(tmp, SR_HI) << 9;
		wm->pipe[PIPE_B].plane[PLANE_SPRITE1] |= _FW_WM(tmp, SPRITED_HI) << 8;
		wm->pipe[PIPE_B].plane[PLANE_SPRITE0] |= _FW_WM(tmp, SPRITEC_HI) << 8;
		wm->pipe[PIPE_B].plane[PLANE_PRIMARY] |= _FW_WM(tmp, PLANEB_HI) << 8;
		wm->pipe[PIPE_A].plane[PLANE_SPRITE1] |= _FW_WM(tmp, SPRITEB_HI) << 8;
		wm->pipe[PIPE_A].plane[PLANE_SPRITE0] |= _FW_WM(tmp, SPRITEA_HI) << 8;
		wm->pipe[PIPE_A].plane[PLANE_PRIMARY] |= _FW_WM(tmp, PLANEA_HI) << 8;
	}
}

#undef _FW_WM
#undef _FW_WM_VLV

<<<<<<< HEAD
void g4x_wm_get_hw_state(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
=======
void g4x_wm_get_hw_state(struct drm_i915_private *dev_priv)
{
>>>>>>> upstream/android-13
	struct g4x_wm_values *wm = &dev_priv->wm.g4x;
	struct intel_crtc *crtc;

	g4x_read_wm_values(dev_priv, wm);

<<<<<<< HEAD
	wm->cxsr = I915_READ(FW_BLC_SELF) & FW_BLC_SELF_EN;

	for_each_intel_crtc(dev, crtc) {
=======
	wm->cxsr = intel_uncore_read(&dev_priv->uncore, FW_BLC_SELF) & FW_BLC_SELF_EN;

	for_each_intel_crtc(&dev_priv->drm, crtc) {
>>>>>>> upstream/android-13
		struct intel_crtc_state *crtc_state =
			to_intel_crtc_state(crtc->base.state);
		struct g4x_wm_state *active = &crtc->wm.active.g4x;
		struct g4x_pipe_wm *raw;
		enum pipe pipe = crtc->pipe;
		enum plane_id plane_id;
		int level, max_level;

		active->cxsr = wm->cxsr;
		active->hpll_en = wm->hpll_en;
		active->fbc_en = wm->fbc_en;

		active->sr = wm->sr;
		active->hpll = wm->hpll;

		for_each_plane_id_on_crtc(crtc, plane_id) {
			active->wm.plane[plane_id] =
				wm->pipe[pipe].plane[plane_id];
		}

		if (wm->cxsr && wm->hpll_en)
			max_level = G4X_WM_LEVEL_HPLL;
		else if (wm->cxsr)
			max_level = G4X_WM_LEVEL_SR;
		else
			max_level = G4X_WM_LEVEL_NORMAL;

		level = G4X_WM_LEVEL_NORMAL;
		raw = &crtc_state->wm.g4x.raw[level];
		for_each_plane_id_on_crtc(crtc, plane_id)
			raw->plane[plane_id] = active->wm.plane[plane_id];

		if (++level > max_level)
			goto out;

		raw = &crtc_state->wm.g4x.raw[level];
		raw->plane[PLANE_PRIMARY] = active->sr.plane;
		raw->plane[PLANE_CURSOR] = active->sr.cursor;
		raw->plane[PLANE_SPRITE0] = 0;
		raw->fbc = active->sr.fbc;

		if (++level > max_level)
			goto out;

		raw = &crtc_state->wm.g4x.raw[level];
		raw->plane[PLANE_PRIMARY] = active->hpll.plane;
		raw->plane[PLANE_CURSOR] = active->hpll.cursor;
		raw->plane[PLANE_SPRITE0] = 0;
		raw->fbc = active->hpll.fbc;

	out:
		for_each_plane_id_on_crtc(crtc, plane_id)
			g4x_raw_plane_wm_set(crtc_state, level,
					     plane_id, USHRT_MAX);
		g4x_raw_fbc_wm_set(crtc_state, level, USHRT_MAX);

		crtc_state->wm.g4x.optimal = *active;
		crtc_state->wm.g4x.intermediate = *active;

<<<<<<< HEAD
		DRM_DEBUG_KMS("Initial watermarks: pipe %c, plane=%d, cursor=%d, sprite=%d\n",
			      pipe_name(pipe),
			      wm->pipe[pipe].plane[PLANE_PRIMARY],
			      wm->pipe[pipe].plane[PLANE_CURSOR],
			      wm->pipe[pipe].plane[PLANE_SPRITE0]);
	}

	DRM_DEBUG_KMS("Initial SR watermarks: plane=%d, cursor=%d fbc=%d\n",
		      wm->sr.plane, wm->sr.cursor, wm->sr.fbc);
	DRM_DEBUG_KMS("Initial HPLL watermarks: plane=%d, SR cursor=%d fbc=%d\n",
		      wm->hpll.plane, wm->hpll.cursor, wm->hpll.fbc);
	DRM_DEBUG_KMS("Initial SR=%s HPLL=%s FBC=%s\n",
		      yesno(wm->cxsr), yesno(wm->hpll_en), yesno(wm->fbc_en));
=======
		drm_dbg_kms(&dev_priv->drm,
			    "Initial watermarks: pipe %c, plane=%d, cursor=%d, sprite=%d\n",
			    pipe_name(pipe),
			    wm->pipe[pipe].plane[PLANE_PRIMARY],
			    wm->pipe[pipe].plane[PLANE_CURSOR],
			    wm->pipe[pipe].plane[PLANE_SPRITE0]);
	}

	drm_dbg_kms(&dev_priv->drm,
		    "Initial SR watermarks: plane=%d, cursor=%d fbc=%d\n",
		    wm->sr.plane, wm->sr.cursor, wm->sr.fbc);
	drm_dbg_kms(&dev_priv->drm,
		    "Initial HPLL watermarks: plane=%d, SR cursor=%d fbc=%d\n",
		    wm->hpll.plane, wm->hpll.cursor, wm->hpll.fbc);
	drm_dbg_kms(&dev_priv->drm, "Initial SR=%s HPLL=%s FBC=%s\n",
		    yesno(wm->cxsr), yesno(wm->hpll_en), yesno(wm->fbc_en));
>>>>>>> upstream/android-13
}

void g4x_wm_sanitize(struct drm_i915_private *dev_priv)
{
	struct intel_plane *plane;
	struct intel_crtc *crtc;

	mutex_lock(&dev_priv->wm.wm_mutex);

	for_each_intel_plane(&dev_priv->drm, plane) {
		struct intel_crtc *crtc =
			intel_get_crtc_for_pipe(dev_priv, plane->pipe);
		struct intel_crtc_state *crtc_state =
			to_intel_crtc_state(crtc->base.state);
		struct intel_plane_state *plane_state =
			to_intel_plane_state(plane->base.state);
		struct g4x_wm_state *wm_state = &crtc_state->wm.g4x.optimal;
		enum plane_id plane_id = plane->id;
		int level;

<<<<<<< HEAD
		if (plane_state->base.visible)
=======
		if (plane_state->uapi.visible)
>>>>>>> upstream/android-13
			continue;

		for (level = 0; level < 3; level++) {
			struct g4x_pipe_wm *raw =
				&crtc_state->wm.g4x.raw[level];

			raw->plane[plane_id] = 0;
			wm_state->wm.plane[plane_id] = 0;
		}

		if (plane_id == PLANE_PRIMARY) {
			for (level = 0; level < 3; level++) {
				struct g4x_pipe_wm *raw =
					&crtc_state->wm.g4x.raw[level];
				raw->fbc = 0;
			}

			wm_state->sr.fbc = 0;
			wm_state->hpll.fbc = 0;
			wm_state->fbc_en = false;
		}
	}

	for_each_intel_crtc(&dev_priv->drm, crtc) {
		struct intel_crtc_state *crtc_state =
			to_intel_crtc_state(crtc->base.state);

		crtc_state->wm.g4x.intermediate =
			crtc_state->wm.g4x.optimal;
		crtc->wm.active.g4x = crtc_state->wm.g4x.optimal;
	}

	g4x_program_watermarks(dev_priv);

	mutex_unlock(&dev_priv->wm.wm_mutex);
}

<<<<<<< HEAD
void vlv_wm_get_hw_state(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
=======
void vlv_wm_get_hw_state(struct drm_i915_private *dev_priv)
{
>>>>>>> upstream/android-13
	struct vlv_wm_values *wm = &dev_priv->wm.vlv;
	struct intel_crtc *crtc;
	u32 val;

	vlv_read_wm_values(dev_priv, wm);

<<<<<<< HEAD
	wm->cxsr = I915_READ(FW_BLC_SELF_VLV) & FW_CSPWRDWNEN;
	wm->level = VLV_WM_LEVEL_PM2;

	if (IS_CHERRYVIEW(dev_priv)) {
		mutex_lock(&dev_priv->pcu_lock);

		val = vlv_punit_read(dev_priv, PUNIT_REG_DSPFREQ);
=======
	wm->cxsr = intel_uncore_read(&dev_priv->uncore, FW_BLC_SELF_VLV) & FW_CSPWRDWNEN;
	wm->level = VLV_WM_LEVEL_PM2;

	if (IS_CHERRYVIEW(dev_priv)) {
		vlv_punit_get(dev_priv);

		val = vlv_punit_read(dev_priv, PUNIT_REG_DSPSSPM);
>>>>>>> upstream/android-13
		if (val & DSP_MAXFIFO_PM5_ENABLE)
			wm->level = VLV_WM_LEVEL_PM5;

		/*
		 * If DDR DVFS is disabled in the BIOS, Punit
		 * will never ack the request. So if that happens
		 * assume we don't have to enable/disable DDR DVFS
		 * dynamically. To test that just set the REQ_ACK
		 * bit to poke the Punit, but don't change the
		 * HIGH/LOW bits so that we don't actually change
		 * the current state.
		 */
		val = vlv_punit_read(dev_priv, PUNIT_REG_DDR_SETUP2);
		val |= FORCE_DDR_FREQ_REQ_ACK;
		vlv_punit_write(dev_priv, PUNIT_REG_DDR_SETUP2, val);

		if (wait_for((vlv_punit_read(dev_priv, PUNIT_REG_DDR_SETUP2) &
			      FORCE_DDR_FREQ_REQ_ACK) == 0, 3)) {
<<<<<<< HEAD
			DRM_DEBUG_KMS("Punit not acking DDR DVFS request, "
				      "assuming DDR DVFS is disabled\n");
=======
			drm_dbg_kms(&dev_priv->drm,
				    "Punit not acking DDR DVFS request, "
				    "assuming DDR DVFS is disabled\n");
>>>>>>> upstream/android-13
			dev_priv->wm.max_level = VLV_WM_LEVEL_PM5;
		} else {
			val = vlv_punit_read(dev_priv, PUNIT_REG_DDR_SETUP2);
			if ((val & FORCE_DDR_HIGH_FREQ) == 0)
				wm->level = VLV_WM_LEVEL_DDR_DVFS;
		}

<<<<<<< HEAD
		mutex_unlock(&dev_priv->pcu_lock);
	}

	for_each_intel_crtc(dev, crtc) {
=======
		vlv_punit_put(dev_priv);
	}

	for_each_intel_crtc(&dev_priv->drm, crtc) {
>>>>>>> upstream/android-13
		struct intel_crtc_state *crtc_state =
			to_intel_crtc_state(crtc->base.state);
		struct vlv_wm_state *active = &crtc->wm.active.vlv;
		const struct vlv_fifo_state *fifo_state =
			&crtc_state->wm.vlv.fifo_state;
		enum pipe pipe = crtc->pipe;
		enum plane_id plane_id;
		int level;

		vlv_get_fifo_size(crtc_state);

		active->num_levels = wm->level + 1;
		active->cxsr = wm->cxsr;

		for (level = 0; level < active->num_levels; level++) {
			struct g4x_pipe_wm *raw =
				&crtc_state->wm.vlv.raw[level];

			active->sr[level].plane = wm->sr.plane;
			active->sr[level].cursor = wm->sr.cursor;

			for_each_plane_id_on_crtc(crtc, plane_id) {
				active->wm[level].plane[plane_id] =
					wm->pipe[pipe].plane[plane_id];

				raw->plane[plane_id] =
					vlv_invert_wm_value(active->wm[level].plane[plane_id],
							    fifo_state->plane[plane_id]);
			}
		}

		for_each_plane_id_on_crtc(crtc, plane_id)
			vlv_raw_plane_wm_set(crtc_state, level,
					     plane_id, USHRT_MAX);
		vlv_invalidate_wms(crtc, active, level);

		crtc_state->wm.vlv.optimal = *active;
		crtc_state->wm.vlv.intermediate = *active;

<<<<<<< HEAD
		DRM_DEBUG_KMS("Initial watermarks: pipe %c, plane=%d, cursor=%d, sprite0=%d, sprite1=%d\n",
			      pipe_name(pipe),
			      wm->pipe[pipe].plane[PLANE_PRIMARY],
			      wm->pipe[pipe].plane[PLANE_CURSOR],
			      wm->pipe[pipe].plane[PLANE_SPRITE0],
			      wm->pipe[pipe].plane[PLANE_SPRITE1]);
	}

	DRM_DEBUG_KMS("Initial watermarks: SR plane=%d, SR cursor=%d level=%d cxsr=%d\n",
		      wm->sr.plane, wm->sr.cursor, wm->level, wm->cxsr);
=======
		drm_dbg_kms(&dev_priv->drm,
			    "Initial watermarks: pipe %c, plane=%d, cursor=%d, sprite0=%d, sprite1=%d\n",
			    pipe_name(pipe),
			    wm->pipe[pipe].plane[PLANE_PRIMARY],
			    wm->pipe[pipe].plane[PLANE_CURSOR],
			    wm->pipe[pipe].plane[PLANE_SPRITE0],
			    wm->pipe[pipe].plane[PLANE_SPRITE1]);
	}

	drm_dbg_kms(&dev_priv->drm,
		    "Initial watermarks: SR plane=%d, SR cursor=%d level=%d cxsr=%d\n",
		    wm->sr.plane, wm->sr.cursor, wm->level, wm->cxsr);
>>>>>>> upstream/android-13
}

void vlv_wm_sanitize(struct drm_i915_private *dev_priv)
{
	struct intel_plane *plane;
	struct intel_crtc *crtc;

	mutex_lock(&dev_priv->wm.wm_mutex);

	for_each_intel_plane(&dev_priv->drm, plane) {
		struct intel_crtc *crtc =
			intel_get_crtc_for_pipe(dev_priv, plane->pipe);
		struct intel_crtc_state *crtc_state =
			to_intel_crtc_state(crtc->base.state);
		struct intel_plane_state *plane_state =
			to_intel_plane_state(plane->base.state);
		struct vlv_wm_state *wm_state = &crtc_state->wm.vlv.optimal;
		const struct vlv_fifo_state *fifo_state =
			&crtc_state->wm.vlv.fifo_state;
		enum plane_id plane_id = plane->id;
		int level;

<<<<<<< HEAD
		if (plane_state->base.visible)
=======
		if (plane_state->uapi.visible)
>>>>>>> upstream/android-13
			continue;

		for (level = 0; level < wm_state->num_levels; level++) {
			struct g4x_pipe_wm *raw =
				&crtc_state->wm.vlv.raw[level];

			raw->plane[plane_id] = 0;

			wm_state->wm[level].plane[plane_id] =
				vlv_invert_wm_value(raw->plane[plane_id],
						    fifo_state->plane[plane_id]);
		}
	}

	for_each_intel_crtc(&dev_priv->drm, crtc) {
		struct intel_crtc_state *crtc_state =
			to_intel_crtc_state(crtc->base.state);

		crtc_state->wm.vlv.intermediate =
			crtc_state->wm.vlv.optimal;
		crtc->wm.active.vlv = crtc_state->wm.vlv.optimal;
	}

	vlv_program_watermarks(dev_priv);

	mutex_unlock(&dev_priv->wm.wm_mutex);
}

/*
 * FIXME should probably kill this and improve
 * the real watermark readout/sanitation instead
 */
static void ilk_init_lp_watermarks(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	I915_WRITE(WM3_LP_ILK, I915_READ(WM3_LP_ILK) & ~WM1_LP_SR_EN);
	I915_WRITE(WM2_LP_ILK, I915_READ(WM2_LP_ILK) & ~WM1_LP_SR_EN);
	I915_WRITE(WM1_LP_ILK, I915_READ(WM1_LP_ILK) & ~WM1_LP_SR_EN);
=======
	intel_uncore_write(&dev_priv->uncore, WM3_LP_ILK, intel_uncore_read(&dev_priv->uncore, WM3_LP_ILK) & ~WM1_LP_SR_EN);
	intel_uncore_write(&dev_priv->uncore, WM2_LP_ILK, intel_uncore_read(&dev_priv->uncore, WM2_LP_ILK) & ~WM1_LP_SR_EN);
	intel_uncore_write(&dev_priv->uncore, WM1_LP_ILK, intel_uncore_read(&dev_priv->uncore, WM1_LP_ILK) & ~WM1_LP_SR_EN);
>>>>>>> upstream/android-13

	/*
	 * Don't touch WM1S_LP_EN here.
	 * Doing so could cause underruns.
	 */
}

<<<<<<< HEAD
void ilk_wm_get_hw_state(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct ilk_wm_values *hw = &dev_priv->wm.hw;
	struct drm_crtc *crtc;

	ilk_init_lp_watermarks(dev_priv);

	for_each_crtc(dev, crtc)
		ilk_pipe_wm_get_hw_state(crtc);

	hw->wm_lp[0] = I915_READ(WM1_LP_ILK);
	hw->wm_lp[1] = I915_READ(WM2_LP_ILK);
	hw->wm_lp[2] = I915_READ(WM3_LP_ILK);

	hw->wm_lp_spr[0] = I915_READ(WM1S_LP_ILK);
	if (INTEL_GEN(dev_priv) >= 7) {
		hw->wm_lp_spr[1] = I915_READ(WM2S_LP_IVB);
		hw->wm_lp_spr[2] = I915_READ(WM3S_LP_IVB);
	}

	if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv))
		hw->partitioning = (I915_READ(WM_MISC) & WM_MISC_DATA_PARTITION_5_6) ?
			INTEL_DDB_PART_5_6 : INTEL_DDB_PART_1_2;
	else if (IS_IVYBRIDGE(dev_priv))
		hw->partitioning = (I915_READ(DISP_ARB_CTL2) & DISP_DATA_PARTITION_5_6) ?
			INTEL_DDB_PART_5_6 : INTEL_DDB_PART_1_2;

	hw->enable_fbc_wm =
		!(I915_READ(DISP_ARB_CTL) & DISP_FBC_WM_DIS);
=======
void ilk_wm_get_hw_state(struct drm_i915_private *dev_priv)
{
	struct ilk_wm_values *hw = &dev_priv->wm.hw;
	struct intel_crtc *crtc;

	ilk_init_lp_watermarks(dev_priv);

	for_each_intel_crtc(&dev_priv->drm, crtc)
		ilk_pipe_wm_get_hw_state(crtc);

	hw->wm_lp[0] = intel_uncore_read(&dev_priv->uncore, WM1_LP_ILK);
	hw->wm_lp[1] = intel_uncore_read(&dev_priv->uncore, WM2_LP_ILK);
	hw->wm_lp[2] = intel_uncore_read(&dev_priv->uncore, WM3_LP_ILK);

	hw->wm_lp_spr[0] = intel_uncore_read(&dev_priv->uncore, WM1S_LP_ILK);
	if (DISPLAY_VER(dev_priv) >= 7) {
		hw->wm_lp_spr[1] = intel_uncore_read(&dev_priv->uncore, WM2S_LP_IVB);
		hw->wm_lp_spr[2] = intel_uncore_read(&dev_priv->uncore, WM3S_LP_IVB);
	}

	if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv))
		hw->partitioning = (intel_uncore_read(&dev_priv->uncore, WM_MISC) & WM_MISC_DATA_PARTITION_5_6) ?
			INTEL_DDB_PART_5_6 : INTEL_DDB_PART_1_2;
	else if (IS_IVYBRIDGE(dev_priv))
		hw->partitioning = (intel_uncore_read(&dev_priv->uncore, DISP_ARB_CTL2) & DISP_DATA_PARTITION_5_6) ?
			INTEL_DDB_PART_5_6 : INTEL_DDB_PART_1_2;

	hw->enable_fbc_wm =
		!(intel_uncore_read(&dev_priv->uncore, DISP_ARB_CTL) & DISP_FBC_WM_DIS);
>>>>>>> upstream/android-13
}

/**
 * intel_update_watermarks - update FIFO watermark values based on current modes
 * @crtc: the #intel_crtc on which to compute the WM
 *
 * Calculate watermark values for the various WM regs based on current mode
 * and plane configuration.
 *
 * There are several cases to deal with here:
 *   - normal (i.e. non-self-refresh)
 *   - self-refresh (SR) mode
 *   - lines are large relative to FIFO size (buffer can hold up to 2)
 *   - lines are small relative to FIFO size (buffer can hold more than 2
 *     lines), so need to account for TLB latency
 *
 *   The normal calculation is:
 *     watermark = dotclock * bytes per pixel * latency
 *   where latency is platform & configuration dependent (we assume pessimal
 *   values here).
 *
 *   The SR calculation is:
 *     watermark = (trunc(latency/line time)+1) * surface width *
 *       bytes per pixel
 *   where
 *     line time = htotal / dotclock
 *     surface width = hdisplay for normal plane and 64 for cursor
 *   and latency is assumed to be high, as above.
 *
 * The final value programmed to the register should always be rounded up,
 * and include an extra 2 entries to account for clock crossings.
 *
 * We don't use the sprite, so we can ignore that.  And on Crestline we have
 * to set the non-SR watermarks to 8.
 */
void intel_update_watermarks(struct intel_crtc *crtc)
{
	struct drm_i915_private *dev_priv = to_i915(crtc->base.dev);

	if (dev_priv->display.update_wm)
		dev_priv->display.update_wm(crtc);
}

void intel_enable_ipc(struct drm_i915_private *dev_priv)
{
	u32 val;

<<<<<<< HEAD
	/* Display WA #0477 WaDisableIPC: skl */
	if (IS_SKYLAKE(dev_priv)) {
		dev_priv->ipc_enabled = false;
		return;
	}

	val = I915_READ(DISP_ARB_CTL2);
=======
	if (!HAS_IPC(dev_priv))
		return;

	val = intel_uncore_read(&dev_priv->uncore, DISP_ARB_CTL2);
>>>>>>> upstream/android-13

	if (dev_priv->ipc_enabled)
		val |= DISP_IPC_ENABLE;
	else
		val &= ~DISP_IPC_ENABLE;

<<<<<<< HEAD
	I915_WRITE(DISP_ARB_CTL2, val);
=======
	intel_uncore_write(&dev_priv->uncore, DISP_ARB_CTL2, val);
}

static bool intel_can_enable_ipc(struct drm_i915_private *dev_priv)
{
	/* Display WA #0477 WaDisableIPC: skl */
	if (IS_SKYLAKE(dev_priv))
		return false;

	/* Display WA #1141: SKL:all KBL:all CFL */
	if (IS_KABYLAKE(dev_priv) ||
	    IS_COFFEELAKE(dev_priv) ||
	    IS_COMETLAKE(dev_priv))
		return dev_priv->dram_info.symmetric_memory;

	return true;
>>>>>>> upstream/android-13
}

void intel_init_ipc(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	dev_priv->ipc_enabled = false;
	if (!HAS_IPC(dev_priv))
		return;

	dev_priv->ipc_enabled = true;
	intel_enable_ipc(dev_priv);
}

/*
 * Lock protecting IPS related data structures
 */
DEFINE_SPINLOCK(mchdev_lock);

/* Global for IPS driver to get at the current i915 device. Protected by
 * mchdev_lock. */
static struct drm_i915_private *i915_mch_dev;

bool ironlake_set_drps(struct drm_i915_private *dev_priv, u8 val)
{
	u16 rgvswctl;

	lockdep_assert_held(&mchdev_lock);

	rgvswctl = I915_READ16(MEMSWCTL);
	if (rgvswctl & MEMCTL_CMD_STS) {
		DRM_DEBUG("gpu busy, RCS change rejected\n");
		return false; /* still busy with another command */
	}

	rgvswctl = (MEMCTL_CMD_CHFREQ << MEMCTL_CMD_SHIFT) |
		(val << MEMCTL_FREQ_SHIFT) | MEMCTL_SFCAVM;
	I915_WRITE16(MEMSWCTL, rgvswctl);
	POSTING_READ16(MEMSWCTL);

	rgvswctl |= MEMCTL_CMD_STS;
	I915_WRITE16(MEMSWCTL, rgvswctl);

	return true;
}

static void ironlake_enable_drps(struct drm_i915_private *dev_priv)
{
	u32 rgvmodectl;
	u8 fmax, fmin, fstart, vstart;

	spin_lock_irq(&mchdev_lock);

	rgvmodectl = I915_READ(MEMMODECTL);

	/* Enable temp reporting */
	I915_WRITE16(PMMISC, I915_READ(PMMISC) | MCPPCE_EN);
	I915_WRITE16(TSC1, I915_READ(TSC1) | TSE);

	/* 100ms RC evaluation intervals */
	I915_WRITE(RCUPEI, 100000);
	I915_WRITE(RCDNEI, 100000);

	/* Set max/min thresholds to 90ms and 80ms respectively */
	I915_WRITE(RCBMAXAVG, 90000);
	I915_WRITE(RCBMINAVG, 80000);

	I915_WRITE(MEMIHYST, 1);

	/* Set up min, max, and cur for interrupt handling */
	fmax = (rgvmodectl & MEMMODE_FMAX_MASK) >> MEMMODE_FMAX_SHIFT;
	fmin = (rgvmodectl & MEMMODE_FMIN_MASK);
	fstart = (rgvmodectl & MEMMODE_FSTART_MASK) >>
		MEMMODE_FSTART_SHIFT;

	vstart = (I915_READ(PXVFREQ(fstart)) & PXVFREQ_PX_MASK) >>
		PXVFREQ_PX_SHIFT;

	dev_priv->ips.fmax = fmax; /* IPS callback will increase this */
	dev_priv->ips.fstart = fstart;

	dev_priv->ips.max_delay = fstart;
	dev_priv->ips.min_delay = fmin;
	dev_priv->ips.cur_delay = fstart;

	DRM_DEBUG_DRIVER("fmax: %d, fmin: %d, fstart: %d\n",
			 fmax, fmin, fstart);

	I915_WRITE(MEMINTREN, MEMINT_CX_SUPR_EN | MEMINT_EVAL_CHG_EN);

	/*
	 * Interrupts will be enabled in ironlake_irq_postinstall
	 */

	I915_WRITE(VIDSTART, vstart);
	POSTING_READ(VIDSTART);

	rgvmodectl |= MEMMODE_SWMODE_EN;
	I915_WRITE(MEMMODECTL, rgvmodectl);

	if (wait_for_atomic((I915_READ(MEMSWCTL) & MEMCTL_CMD_STS) == 0, 10))
		DRM_ERROR("stuck trying to change perf mode\n");
	mdelay(1);

	ironlake_set_drps(dev_priv, fstart);

	dev_priv->ips.last_count1 = I915_READ(DMIEC) +
		I915_READ(DDREC) + I915_READ(CSIEC);
	dev_priv->ips.last_time1 = jiffies_to_msecs(jiffies);
	dev_priv->ips.last_count2 = I915_READ(GFXEC);
	dev_priv->ips.last_time2 = ktime_get_raw_ns();

	spin_unlock_irq(&mchdev_lock);
}

static void ironlake_disable_drps(struct drm_i915_private *dev_priv)
{
	u16 rgvswctl;

	spin_lock_irq(&mchdev_lock);

	rgvswctl = I915_READ16(MEMSWCTL);

	/* Ack interrupts, disable EFC interrupt */
	I915_WRITE(MEMINTREN, I915_READ(MEMINTREN) & ~MEMINT_EVAL_CHG_EN);
	I915_WRITE(MEMINTRSTS, MEMINT_EVAL_CHG);
	I915_WRITE(DEIER, I915_READ(DEIER) & ~DE_PCU_EVENT);
	I915_WRITE(DEIIR, DE_PCU_EVENT);
	I915_WRITE(DEIMR, I915_READ(DEIMR) | DE_PCU_EVENT);

	/* Go back to the starting frequency */
	ironlake_set_drps(dev_priv, dev_priv->ips.fstart);
	mdelay(1);
	rgvswctl |= MEMCTL_CMD_STS;
	I915_WRITE(MEMSWCTL, rgvswctl);
	mdelay(1);

	spin_unlock_irq(&mchdev_lock);
}

/* There's a funny hw issue where the hw returns all 0 when reading from
 * GEN6_RP_INTERRUPT_LIMITS. Hence we always need to compute the desired value
 * ourselves, instead of doing a rmw cycle (which might result in us clearing
 * all limits and the gpu stuck at whatever frequency it is at atm).
 */
static u32 intel_rps_limits(struct drm_i915_private *dev_priv, u8 val)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
	u32 limits;

	/* Only set the down limit when we've reached the lowest level to avoid
	 * getting more interrupts, otherwise leave this clear. This prevents a
	 * race in the hw when coming out of rc6: There's a tiny window where
	 * the hw runs at the minimal clock before selecting the desired
	 * frequency, if the down threshold expires in that window we will not
	 * receive a down interrupt. */
	if (INTEL_GEN(dev_priv) >= 9) {
		limits = (rps->max_freq_softlimit) << 23;
		if (val <= rps->min_freq_softlimit)
			limits |= (rps->min_freq_softlimit) << 14;
	} else {
		limits = rps->max_freq_softlimit << 24;
		if (val <= rps->min_freq_softlimit)
			limits |= rps->min_freq_softlimit << 16;
	}

	return limits;
}

static void rps_set_power(struct drm_i915_private *dev_priv, int new_power)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
	u32 threshold_up = 0, threshold_down = 0; /* in % */
	u32 ei_up = 0, ei_down = 0;

	lockdep_assert_held(&rps->power.mutex);

	if (new_power == rps->power.mode)
		return;

	/* Note the units here are not exactly 1us, but 1280ns. */
	switch (new_power) {
	case LOW_POWER:
		/* Upclock if more than 95% busy over 16ms */
		ei_up = 16000;
		threshold_up = 95;

		/* Downclock if less than 85% busy over 32ms */
		ei_down = 32000;
		threshold_down = 85;
		break;

	case BETWEEN:
		/* Upclock if more than 90% busy over 13ms */
		ei_up = 13000;
		threshold_up = 90;

		/* Downclock if less than 75% busy over 32ms */
		ei_down = 32000;
		threshold_down = 75;
		break;

	case HIGH_POWER:
		/* Upclock if more than 85% busy over 10ms */
		ei_up = 10000;
		threshold_up = 85;

		/* Downclock if less than 60% busy over 32ms */
		ei_down = 32000;
		threshold_down = 60;
		break;
	}

	/* When byt can survive without system hang with dynamic
	 * sw freq adjustments, this restriction can be lifted.
	 */
	if (IS_VALLEYVIEW(dev_priv))
		goto skip_hw_write;

	I915_WRITE(GEN6_RP_UP_EI,
		   GT_INTERVAL_FROM_US(dev_priv, ei_up));
	I915_WRITE(GEN6_RP_UP_THRESHOLD,
		   GT_INTERVAL_FROM_US(dev_priv,
				       ei_up * threshold_up / 100));

	I915_WRITE(GEN6_RP_DOWN_EI,
		   GT_INTERVAL_FROM_US(dev_priv, ei_down));
	I915_WRITE(GEN6_RP_DOWN_THRESHOLD,
		   GT_INTERVAL_FROM_US(dev_priv,
				       ei_down * threshold_down / 100));

	I915_WRITE(GEN6_RP_CONTROL,
		   GEN6_RP_MEDIA_TURBO |
		   GEN6_RP_MEDIA_HW_NORMAL_MODE |
		   GEN6_RP_MEDIA_IS_GFX |
		   GEN6_RP_ENABLE |
		   GEN6_RP_UP_BUSY_AVG |
		   GEN6_RP_DOWN_IDLE_AVG);

skip_hw_write:
	rps->power.mode = new_power;
	rps->power.up_threshold = threshold_up;
	rps->power.down_threshold = threshold_down;
}

static void gen6_set_rps_thresholds(struct drm_i915_private *dev_priv, u8 val)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
	int new_power;

	new_power = rps->power.mode;
	switch (rps->power.mode) {
	case LOW_POWER:
		if (val > rps->efficient_freq + 1 &&
		    val > rps->cur_freq)
			new_power = BETWEEN;
		break;

	case BETWEEN:
		if (val <= rps->efficient_freq &&
		    val < rps->cur_freq)
			new_power = LOW_POWER;
		else if (val >= rps->rp0_freq &&
			 val > rps->cur_freq)
			new_power = HIGH_POWER;
		break;

	case HIGH_POWER:
		if (val < (rps->rp1_freq + rps->rp0_freq) >> 1 &&
		    val < rps->cur_freq)
			new_power = BETWEEN;
		break;
	}
	/* Max/min bins are special */
	if (val <= rps->min_freq_softlimit)
		new_power = LOW_POWER;
	if (val >= rps->max_freq_softlimit)
		new_power = HIGH_POWER;

	mutex_lock(&rps->power.mutex);
	if (rps->power.interactive)
		new_power = HIGH_POWER;
	rps_set_power(dev_priv, new_power);
	mutex_unlock(&rps->power.mutex);
	rps->last_adj = 0;
}

void intel_rps_mark_interactive(struct drm_i915_private *i915, bool interactive)
{
	struct intel_rps *rps = &i915->gt_pm.rps;

	if (INTEL_GEN(i915) < 6)
		return;

	mutex_lock(&rps->power.mutex);
	if (interactive) {
		if (!rps->power.interactive++ && READ_ONCE(i915->gt.awake))
			rps_set_power(i915, HIGH_POWER);
	} else {
		GEM_BUG_ON(!rps->power.interactive);
		rps->power.interactive--;
	}
	mutex_unlock(&rps->power.mutex);
}

static u32 gen6_rps_pm_mask(struct drm_i915_private *dev_priv, u8 val)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
	u32 mask = 0;

	/* We use UP_EI_EXPIRED interupts for both up/down in manual mode */
	if (val > rps->min_freq_softlimit)
		mask |= GEN6_PM_RP_UP_EI_EXPIRED | GEN6_PM_RP_DOWN_THRESHOLD | GEN6_PM_RP_DOWN_TIMEOUT;
	if (val < rps->max_freq_softlimit)
		mask |= GEN6_PM_RP_UP_EI_EXPIRED | GEN6_PM_RP_UP_THRESHOLD;

	mask &= dev_priv->pm_rps_events;

	return gen6_sanitize_rps_pm_mask(dev_priv, ~mask);
}

/* gen6_set_rps is called to update the frequency request, but should also be
 * called when the range (min_delay and max_delay) is modified so that we can
 * update the GEN6_RP_INTERRUPT_LIMITS register accordingly. */
static int gen6_set_rps(struct drm_i915_private *dev_priv, u8 val)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;

	/* min/max delay may still have been modified so be sure to
	 * write the limits value.
	 */
	if (val != rps->cur_freq) {
		gen6_set_rps_thresholds(dev_priv, val);

		if (INTEL_GEN(dev_priv) >= 9)
			I915_WRITE(GEN6_RPNSWREQ,
				   GEN9_FREQUENCY(val));
		else if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv))
			I915_WRITE(GEN6_RPNSWREQ,
				   HSW_FREQUENCY(val));
		else
			I915_WRITE(GEN6_RPNSWREQ,
				   GEN6_FREQUENCY(val) |
				   GEN6_OFFSET(0) |
				   GEN6_AGGRESSIVE_TURBO);
	}

	/* Make sure we continue to get interrupts
	 * until we hit the minimum or maximum frequencies.
	 */
	I915_WRITE(GEN6_RP_INTERRUPT_LIMITS, intel_rps_limits(dev_priv, val));
	I915_WRITE(GEN6_PMINTRMSK, gen6_rps_pm_mask(dev_priv, val));

	rps->cur_freq = val;
	trace_intel_gpu_freq_change(intel_gpu_freq(dev_priv, val));

	return 0;
}

static int valleyview_set_rps(struct drm_i915_private *dev_priv, u8 val)
{
	int err;

	if (WARN_ONCE(IS_CHERRYVIEW(dev_priv) && (val & 1),
		      "Odd GPU freq value\n"))
		val &= ~1;

	I915_WRITE(GEN6_PMINTRMSK, gen6_rps_pm_mask(dev_priv, val));

	if (val != dev_priv->gt_pm.rps.cur_freq) {
		err = vlv_punit_write(dev_priv, PUNIT_REG_GPU_FREQ_REQ, val);
		if (err)
			return err;

		gen6_set_rps_thresholds(dev_priv, val);
	}

	dev_priv->gt_pm.rps.cur_freq = val;
	trace_intel_gpu_freq_change(intel_gpu_freq(dev_priv, val));

	return 0;
}

/* vlv_set_rps_idle: Set the frequency to idle, if Gfx clocks are down
 *
 * * If Gfx is Idle, then
 * 1. Forcewake Media well.
 * 2. Request idle freq.
 * 3. Release Forcewake of Media well.
*/
static void vlv_set_rps_idle(struct drm_i915_private *dev_priv)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
	u32 val = rps->idle_freq;
	int err;

	if (rps->cur_freq <= val)
		return;

	/* The punit delays the write of the frequency and voltage until it
	 * determines the GPU is awake. During normal usage we don't want to
	 * waste power changing the frequency if the GPU is sleeping (rc6).
	 * However, the GPU and driver is now idle and we do not want to delay
	 * switching to minimum voltage (reducing power whilst idle) as we do
	 * not expect to be woken in the near future and so must flush the
	 * change by waking the device.
	 *
	 * We choose to take the media powerwell (either would do to trick the
	 * punit into committing the voltage change) as that takes a lot less
	 * power than the render powerwell.
	 */
	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_MEDIA);
	err = valleyview_set_rps(dev_priv, val);
	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_MEDIA);

	if (err)
		DRM_ERROR("Failed to set RPS for idle\n");
}

void gen6_rps_busy(struct drm_i915_private *dev_priv)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;

	mutex_lock(&dev_priv->pcu_lock);
	if (rps->enabled) {
		u8 freq;

		if (dev_priv->pm_rps_events & GEN6_PM_RP_UP_EI_EXPIRED)
			gen6_rps_reset_ei(dev_priv);
		I915_WRITE(GEN6_PMINTRMSK,
			   gen6_rps_pm_mask(dev_priv, rps->cur_freq));

		gen6_enable_rps_interrupts(dev_priv);

		/* Use the user's desired frequency as a guide, but for better
		 * performance, jump directly to RPe as our starting frequency.
		 */
		freq = max(rps->cur_freq,
			   rps->efficient_freq);

		if (intel_set_rps(dev_priv,
				  clamp(freq,
					rps->min_freq_softlimit,
					rps->max_freq_softlimit)))
			DRM_DEBUG_DRIVER("Failed to set idle frequency\n");
	}
	mutex_unlock(&dev_priv->pcu_lock);
}

void gen6_rps_idle(struct drm_i915_private *dev_priv)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;

	/* Flush our bottom-half so that it does not race with us
	 * setting the idle frequency and so that it is bounded by
	 * our rpm wakeref. And then disable the interrupts to stop any
	 * futher RPS reclocking whilst we are asleep.
	 */
	gen6_disable_rps_interrupts(dev_priv);

	mutex_lock(&dev_priv->pcu_lock);
	if (rps->enabled) {
		if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv))
			vlv_set_rps_idle(dev_priv);
		else
			gen6_set_rps(dev_priv, rps->idle_freq);
		rps->last_adj = 0;
		I915_WRITE(GEN6_PMINTRMSK,
			   gen6_sanitize_rps_pm_mask(dev_priv, ~0));
	}
	mutex_unlock(&dev_priv->pcu_lock);
}

void gen6_rps_boost(struct i915_request *rq,
		    struct intel_rps_client *rps_client)
{
	struct intel_rps *rps = &rq->i915->gt_pm.rps;
	unsigned long flags;
	bool boost;

	/* This is intentionally racy! We peek at the state here, then
	 * validate inside the RPS worker.
	 */
	if (!rps->enabled)
		return;

	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &rq->fence.flags))
		return;

	/* Serializes with i915_request_retire() */
	boost = false;
	spin_lock_irqsave(&rq->lock, flags);
	if (!rq->waitboost && !dma_fence_is_signaled_locked(&rq->fence)) {
		boost = !atomic_fetch_inc(&rps->num_waiters);
		rq->waitboost = true;
	}
	spin_unlock_irqrestore(&rq->lock, flags);
	if (!boost)
		return;

	if (READ_ONCE(rps->cur_freq) < rps->boost_freq)
		schedule_work(&rps->work);

	atomic_inc(rps_client ? &rps_client->boosts : &rps->boosts);
}

int intel_set_rps(struct drm_i915_private *dev_priv, u8 val)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
	int err;

	lockdep_assert_held(&dev_priv->pcu_lock);
	GEM_BUG_ON(val > rps->max_freq);
	GEM_BUG_ON(val < rps->min_freq);

	if (!rps->enabled) {
		rps->cur_freq = val;
		return 0;
	}

	if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv))
		err = valleyview_set_rps(dev_priv, val);
	else
		err = gen6_set_rps(dev_priv, val);

	return err;
}

static void gen9_disable_rc6(struct drm_i915_private *dev_priv)
{
	I915_WRITE(GEN6_RC_CONTROL, 0);
	I915_WRITE(GEN9_PG_ENABLE, 0);
}

static void gen9_disable_rps(struct drm_i915_private *dev_priv)
{
	I915_WRITE(GEN6_RP_CONTROL, 0);
}

static void gen6_disable_rc6(struct drm_i915_private *dev_priv)
{
	I915_WRITE(GEN6_RC_CONTROL, 0);
}

static void gen6_disable_rps(struct drm_i915_private *dev_priv)
{
	I915_WRITE(GEN6_RPNSWREQ, 1 << 31);
	I915_WRITE(GEN6_RP_CONTROL, 0);
}

static void cherryview_disable_rc6(struct drm_i915_private *dev_priv)
{
	I915_WRITE(GEN6_RC_CONTROL, 0);
}

static void cherryview_disable_rps(struct drm_i915_private *dev_priv)
{
	I915_WRITE(GEN6_RP_CONTROL, 0);
}

static void valleyview_disable_rc6(struct drm_i915_private *dev_priv)
{
	/* We're doing forcewake before Disabling RC6,
	 * This what the BIOS expects when going into suspend */
	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);

	I915_WRITE(GEN6_RC_CONTROL, 0);

	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
}

static void valleyview_disable_rps(struct drm_i915_private *dev_priv)
{
	I915_WRITE(GEN6_RP_CONTROL, 0);
}

static bool bxt_check_bios_rc6_setup(struct drm_i915_private *dev_priv)
{
	bool enable_rc6 = true;
	unsigned long rc6_ctx_base;
	u32 rc_ctl;
	int rc_sw_target;

	rc_ctl = I915_READ(GEN6_RC_CONTROL);
	rc_sw_target = (I915_READ(GEN6_RC_STATE) & RC_SW_TARGET_STATE_MASK) >>
		       RC_SW_TARGET_STATE_SHIFT;
	DRM_DEBUG_DRIVER("BIOS enabled RC states: "
			 "HW_CTRL %s HW_RC6 %s SW_TARGET_STATE %x\n",
			 onoff(rc_ctl & GEN6_RC_CTL_HW_ENABLE),
			 onoff(rc_ctl & GEN6_RC_CTL_RC6_ENABLE),
			 rc_sw_target);

	if (!(I915_READ(RC6_LOCATION) & RC6_CTX_IN_DRAM)) {
		DRM_DEBUG_DRIVER("RC6 Base location not set properly.\n");
		enable_rc6 = false;
	}

	/*
	 * The exact context size is not known for BXT, so assume a page size
	 * for this check.
	 */
	rc6_ctx_base = I915_READ(RC6_CTX_BASE) & RC6_CTX_BASE_MASK;
	if (!((rc6_ctx_base >= dev_priv->dsm_reserved.start) &&
	      (rc6_ctx_base + PAGE_SIZE < dev_priv->dsm_reserved.end))) {
		DRM_DEBUG_DRIVER("RC6 Base address not as expected.\n");
		enable_rc6 = false;
	}

	if (!(((I915_READ(PWRCTX_MAXCNT_RCSUNIT) & IDLE_TIME_MASK) > 1) &&
	      ((I915_READ(PWRCTX_MAXCNT_VCSUNIT0) & IDLE_TIME_MASK) > 1) &&
	      ((I915_READ(PWRCTX_MAXCNT_BCSUNIT) & IDLE_TIME_MASK) > 1) &&
	      ((I915_READ(PWRCTX_MAXCNT_VECSUNIT) & IDLE_TIME_MASK) > 1))) {
		DRM_DEBUG_DRIVER("Engine Idle wait time not set properly.\n");
		enable_rc6 = false;
	}

	if (!I915_READ(GEN8_PUSHBUS_CONTROL) ||
	    !I915_READ(GEN8_PUSHBUS_ENABLE) ||
	    !I915_READ(GEN8_PUSHBUS_SHIFT)) {
		DRM_DEBUG_DRIVER("Pushbus not setup properly.\n");
		enable_rc6 = false;
	}

	if (!I915_READ(GEN6_GFXPAUSE)) {
		DRM_DEBUG_DRIVER("GFX pause not setup properly.\n");
		enable_rc6 = false;
	}

	if (!I915_READ(GEN8_MISC_CTRL0)) {
		DRM_DEBUG_DRIVER("GPM control not setup properly.\n");
		enable_rc6 = false;
	}

	return enable_rc6;
}

static bool sanitize_rc6(struct drm_i915_private *i915)
{
	struct intel_device_info *info = mkwrite_device_info(i915);

	/* Powersaving is controlled by the host when inside a VM */
	if (intel_vgpu_active(i915))
		info->has_rc6 = 0;

	if (info->has_rc6 &&
	    IS_GEN9_LP(i915) && !bxt_check_bios_rc6_setup(i915)) {
		DRM_INFO("RC6 disabled by BIOS\n");
		info->has_rc6 = 0;
	}

	/*
	 * We assume that we do not have any deep rc6 levels if we don't have
	 * have the previous rc6 level supported, i.e. we use HAS_RC6()
	 * as the initial coarse check for rc6 in general, moving on to
	 * progressively finer/deeper levels.
	 */
	if (!info->has_rc6 && info->has_rc6p)
		info->has_rc6p = 0;

	return info->has_rc6;
}

static void gen6_init_rps_frequencies(struct drm_i915_private *dev_priv)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;

	/* All of these values are in units of 50MHz */

	/* static values from HW: RP0 > RP1 > RPn (min_freq) */
	if (IS_GEN9_LP(dev_priv)) {
		u32 rp_state_cap = I915_READ(BXT_RP_STATE_CAP);
		rps->rp0_freq = (rp_state_cap >> 16) & 0xff;
		rps->rp1_freq = (rp_state_cap >>  8) & 0xff;
		rps->min_freq = (rp_state_cap >>  0) & 0xff;
	} else {
		u32 rp_state_cap = I915_READ(GEN6_RP_STATE_CAP);
		rps->rp0_freq = (rp_state_cap >>  0) & 0xff;
		rps->rp1_freq = (rp_state_cap >>  8) & 0xff;
		rps->min_freq = (rp_state_cap >> 16) & 0xff;
	}
	/* hw_max = RP0 until we check for overclocking */
	rps->max_freq = rps->rp0_freq;

	rps->efficient_freq = rps->rp1_freq;
	if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv) ||
	    IS_GEN9_BC(dev_priv) || INTEL_GEN(dev_priv) >= 10) {
		u32 ddcc_status = 0;

		if (sandybridge_pcode_read(dev_priv,
					   HSW_PCODE_DYNAMIC_DUTY_CYCLE_CONTROL,
					   &ddcc_status) == 0)
			rps->efficient_freq =
				clamp_t(u8,
					((ddcc_status >> 8) & 0xff),
					rps->min_freq,
					rps->max_freq);
	}

	if (IS_GEN9_BC(dev_priv) || INTEL_GEN(dev_priv) >= 10) {
		/* Store the frequency values in 16.66 MHZ units, which is
		 * the natural hardware unit for SKL
		 */
		rps->rp0_freq *= GEN9_FREQ_SCALER;
		rps->rp1_freq *= GEN9_FREQ_SCALER;
		rps->min_freq *= GEN9_FREQ_SCALER;
		rps->max_freq *= GEN9_FREQ_SCALER;
		rps->efficient_freq *= GEN9_FREQ_SCALER;
	}
}

static void reset_rps(struct drm_i915_private *dev_priv,
		      int (*set)(struct drm_i915_private *, u8))
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
	u8 freq = rps->cur_freq;

	/* force a reset */
	rps->power.mode = -1;
	rps->cur_freq = -1;

	if (set(dev_priv, freq))
		DRM_ERROR("Failed to reset RPS to initial values\n");
}

/* See the Gen9_GT_PM_Programming_Guide doc for the below */
static void gen9_enable_rps(struct drm_i915_private *dev_priv)
{
	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);

	/* Program defaults and thresholds for RPS */
	if (IS_GEN9(dev_priv))
		I915_WRITE(GEN6_RC_VIDEO_FREQ,
			GEN9_FREQUENCY(dev_priv->gt_pm.rps.rp1_freq));

	/* 1 second timeout*/
	I915_WRITE(GEN6_RP_DOWN_TIMEOUT,
		GT_INTERVAL_FROM_US(dev_priv, 1000000));

	I915_WRITE(GEN6_RP_IDLE_HYSTERSIS, 0xa);

	/* Leaning on the below call to gen6_set_rps to program/setup the
	 * Up/Down EI & threshold registers, as well as the RP_CONTROL,
	 * RP_INTERRUPT_LIMITS & RPNSWREQ registers */
	reset_rps(dev_priv, gen6_set_rps);

	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
}

static void gen9_enable_rc6(struct drm_i915_private *dev_priv)
{
	struct intel_engine_cs *engine;
	enum intel_engine_id id;
	u32 rc6_mode;

	/* 1a: Software RC state - RC0 */
	I915_WRITE(GEN6_RC_STATE, 0);

	/* 1b: Get forcewake during program sequence. Although the driver
	 * hasn't enabled a state yet where we need forcewake, BIOS may have.*/
	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);

	/* 2a: Disable RC states. */
	I915_WRITE(GEN6_RC_CONTROL, 0);

	/* 2b: Program RC6 thresholds.*/
	if (INTEL_GEN(dev_priv) >= 10) {
		I915_WRITE(GEN6_RC6_WAKE_RATE_LIMIT, 54 << 16 | 85);
		I915_WRITE(GEN10_MEDIA_WAKE_RATE_LIMIT, 150);
	} else if (IS_SKYLAKE(dev_priv)) {
		/*
		 * WaRsDoubleRc6WrlWithCoarsePowerGating:skl Doubling WRL only
		 * when CPG is enabled
		 */
		I915_WRITE(GEN6_RC6_WAKE_RATE_LIMIT, 108 << 16);
	} else {
		I915_WRITE(GEN6_RC6_WAKE_RATE_LIMIT, 54 << 16);
	}

	I915_WRITE(GEN6_RC_EVALUATION_INTERVAL, 125000); /* 12500 * 1280ns */
	I915_WRITE(GEN6_RC_IDLE_HYSTERSIS, 25); /* 25 * 1280ns */
	for_each_engine(engine, dev_priv, id)
		I915_WRITE(RING_MAX_IDLE(engine->mmio_base), 10);

	if (HAS_GUC(dev_priv))
		I915_WRITE(GUC_MAX_IDLE_COUNT, 0xA);

	I915_WRITE(GEN6_RC_SLEEP, 0);

	/*
	 * 2c: Program Coarse Power Gating Policies.
	 *
	 * Bspec's guidance is to use 25us (really 25 * 1280ns) here. What we
	 * use instead is a more conservative estimate for the maximum time
	 * it takes us to service a CS interrupt and submit a new ELSP - that
	 * is the time which the GPU is idle waiting for the CPU to select the
	 * next request to execute. If the idle hysteresis is less than that
	 * interrupt service latency, the hardware will automatically gate
	 * the power well and we will then incur the wake up cost on top of
	 * the service latency. A similar guide from intel_pstate is that we
	 * do not want the enable hysteresis to less than the wakeup latency.
	 *
	 * igt/gem_exec_nop/sequential provides a rough estimate for the
	 * service latency, and puts it around 10us for Broadwell (and other
	 * big core) and around 40us for Broxton (and other low power cores).
	 * [Note that for legacy ringbuffer submission, this is less than 1us!]
	 * However, the wakeup latency on Broxton is closer to 100us. To be
	 * conservative, we have to factor in a context switch on top (due
	 * to ksoftirqd).
	 */
	I915_WRITE(GEN9_MEDIA_PG_IDLE_HYSTERESIS, 250);
	I915_WRITE(GEN9_RENDER_PG_IDLE_HYSTERESIS, 250);

	/* 3a: Enable RC6 */
	I915_WRITE(GEN6_RC6_THRESHOLD, 37500); /* 37.5/125ms per EI */

	/* WaRsUseTimeoutMode:cnl (pre-prod) */
	if (IS_CNL_REVID(dev_priv, CNL_REVID_A0, CNL_REVID_C0))
		rc6_mode = GEN7_RC_CTL_TO_MODE;
	else
		rc6_mode = GEN6_RC_CTL_EI_MODE(1);

	I915_WRITE(GEN6_RC_CONTROL,
		   GEN6_RC_CTL_HW_ENABLE |
		   GEN6_RC_CTL_RC6_ENABLE |
		   rc6_mode);

	/*
	 * 3b: Enable Coarse Power Gating only when RC6 is enabled.
	 * WaRsDisableCoarsePowerGating:skl,cnl - Render/Media PG need to be disabled with RC6.
	 */
	if (NEEDS_WaRsDisableCoarsePowerGating(dev_priv))
		I915_WRITE(GEN9_PG_ENABLE, 0);
	else
		I915_WRITE(GEN9_PG_ENABLE,
			   GEN9_RENDER_PG_ENABLE | GEN9_MEDIA_PG_ENABLE);

	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
}

static void gen8_enable_rc6(struct drm_i915_private *dev_priv)
{
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	/* 1a: Software RC state - RC0 */
	I915_WRITE(GEN6_RC_STATE, 0);

	/* 1b: Get forcewake during program sequence. Although the driver
	 * hasn't enabled a state yet where we need forcewake, BIOS may have.*/
	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);

	/* 2a: Disable RC states. */
	I915_WRITE(GEN6_RC_CONTROL, 0);

	/* 2b: Program RC6 thresholds.*/
	I915_WRITE(GEN6_RC6_WAKE_RATE_LIMIT, 40 << 16);
	I915_WRITE(GEN6_RC_EVALUATION_INTERVAL, 125000); /* 12500 * 1280ns */
	I915_WRITE(GEN6_RC_IDLE_HYSTERSIS, 25); /* 25 * 1280ns */
	for_each_engine(engine, dev_priv, id)
		I915_WRITE(RING_MAX_IDLE(engine->mmio_base), 10);
	I915_WRITE(GEN6_RC_SLEEP, 0);
	I915_WRITE(GEN6_RC6_THRESHOLD, 625); /* 800us/1.28 for TO */

	/* 3: Enable RC6 */

	I915_WRITE(GEN6_RC_CONTROL,
		   GEN6_RC_CTL_HW_ENABLE |
		   GEN7_RC_CTL_TO_MODE |
		   GEN6_RC_CTL_RC6_ENABLE);

	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
}

static void gen8_enable_rps(struct drm_i915_private *dev_priv)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;

	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);

	/* 1 Program defaults and thresholds for RPS*/
	I915_WRITE(GEN6_RPNSWREQ,
		   HSW_FREQUENCY(rps->rp1_freq));
	I915_WRITE(GEN6_RC_VIDEO_FREQ,
		   HSW_FREQUENCY(rps->rp1_freq));
	/* NB: Docs say 1s, and 1000000 - which aren't equivalent */
	I915_WRITE(GEN6_RP_DOWN_TIMEOUT, 100000000 / 128); /* 1 second timeout */

	/* Docs recommend 900MHz, and 300 MHz respectively */
	I915_WRITE(GEN6_RP_INTERRUPT_LIMITS,
		   rps->max_freq_softlimit << 24 |
		   rps->min_freq_softlimit << 16);

	I915_WRITE(GEN6_RP_UP_THRESHOLD, 7600000 / 128); /* 76ms busyness per EI, 90% */
	I915_WRITE(GEN6_RP_DOWN_THRESHOLD, 31300000 / 128); /* 313ms busyness per EI, 70%*/
	I915_WRITE(GEN6_RP_UP_EI, 66000); /* 84.48ms, XXX: random? */
	I915_WRITE(GEN6_RP_DOWN_EI, 350000); /* 448ms, XXX: random? */

	I915_WRITE(GEN6_RP_IDLE_HYSTERSIS, 10);

	/* 2: Enable RPS */
	I915_WRITE(GEN6_RP_CONTROL,
		   GEN6_RP_MEDIA_TURBO |
		   GEN6_RP_MEDIA_HW_NORMAL_MODE |
		   GEN6_RP_MEDIA_IS_GFX |
		   GEN6_RP_ENABLE |
		   GEN6_RP_UP_BUSY_AVG |
		   GEN6_RP_DOWN_IDLE_AVG);

	reset_rps(dev_priv, gen6_set_rps);

	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
}

static void gen6_enable_rc6(struct drm_i915_private *dev_priv)
{
	struct intel_engine_cs *engine;
	enum intel_engine_id id;
	u32 rc6vids, rc6_mask;
	u32 gtfifodbg;
	int ret;

	I915_WRITE(GEN6_RC_STATE, 0);

	/* Clear the DBG now so we don't confuse earlier errors */
	gtfifodbg = I915_READ(GTFIFODBG);
	if (gtfifodbg) {
		DRM_ERROR("GT fifo had a previous error %x\n", gtfifodbg);
		I915_WRITE(GTFIFODBG, gtfifodbg);
	}

	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);

	/* disable the counters and set deterministic thresholds */
	I915_WRITE(GEN6_RC_CONTROL, 0);

	I915_WRITE(GEN6_RC1_WAKE_RATE_LIMIT, 1000 << 16);
	I915_WRITE(GEN6_RC6_WAKE_RATE_LIMIT, 40 << 16 | 30);
	I915_WRITE(GEN6_RC6pp_WAKE_RATE_LIMIT, 30);
	I915_WRITE(GEN6_RC_EVALUATION_INTERVAL, 125000);
	I915_WRITE(GEN6_RC_IDLE_HYSTERSIS, 25);

	for_each_engine(engine, dev_priv, id)
		I915_WRITE(RING_MAX_IDLE(engine->mmio_base), 10);

	I915_WRITE(GEN6_RC_SLEEP, 0);
	I915_WRITE(GEN6_RC1e_THRESHOLD, 1000);
	if (IS_IVYBRIDGE(dev_priv))
		I915_WRITE(GEN6_RC6_THRESHOLD, 125000);
	else
		I915_WRITE(GEN6_RC6_THRESHOLD, 50000);
	I915_WRITE(GEN6_RC6p_THRESHOLD, 150000);
	I915_WRITE(GEN6_RC6pp_THRESHOLD, 64000); /* unused */

	/* We don't use those on Haswell */
	rc6_mask = GEN6_RC_CTL_RC6_ENABLE;
	if (HAS_RC6p(dev_priv))
		rc6_mask |= GEN6_RC_CTL_RC6p_ENABLE;
	if (HAS_RC6pp(dev_priv))
		rc6_mask |= GEN6_RC_CTL_RC6pp_ENABLE;
	I915_WRITE(GEN6_RC_CONTROL,
		   rc6_mask |
		   GEN6_RC_CTL_EI_MODE(1) |
		   GEN6_RC_CTL_HW_ENABLE);

	rc6vids = 0;
	ret = sandybridge_pcode_read(dev_priv, GEN6_PCODE_READ_RC6VIDS, &rc6vids);
	if (IS_GEN6(dev_priv) && ret) {
		DRM_DEBUG_DRIVER("Couldn't check for BIOS workaround\n");
	} else if (IS_GEN6(dev_priv) && (GEN6_DECODE_RC6_VID(rc6vids & 0xff) < 450)) {
		DRM_DEBUG_DRIVER("You should update your BIOS. Correcting minimum rc6 voltage (%dmV->%dmV)\n",
			  GEN6_DECODE_RC6_VID(rc6vids & 0xff), 450);
		rc6vids &= 0xffff00;
		rc6vids |= GEN6_ENCODE_RC6_VID(450);
		ret = sandybridge_pcode_write(dev_priv, GEN6_PCODE_WRITE_RC6VIDS, rc6vids);
		if (ret)
			DRM_ERROR("Couldn't fix incorrect rc6 voltage\n");
	}

	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
}

static void gen6_enable_rps(struct drm_i915_private *dev_priv)
{
	/* Here begins a magic sequence of register writes to enable
	 * auto-downclocking.
	 *
	 * Perhaps there might be some value in exposing these to
	 * userspace...
	 */
	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);

	/* Power down if completely idle for over 50ms */
	I915_WRITE(GEN6_RP_DOWN_TIMEOUT, 50000);
	I915_WRITE(GEN6_RP_IDLE_HYSTERSIS, 10);

	reset_rps(dev_priv, gen6_set_rps);

	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
}

static void gen6_update_ring_freq(struct drm_i915_private *dev_priv)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
	const int min_freq = 15;
	const int scaling_factor = 180;
	unsigned int gpu_freq;
	unsigned int max_ia_freq, min_ring_freq;
	unsigned int max_gpu_freq, min_gpu_freq;
	struct cpufreq_policy *policy;

	WARN_ON(!mutex_is_locked(&dev_priv->pcu_lock));

	if (rps->max_freq <= rps->min_freq)
		return;

	policy = cpufreq_cpu_get(0);
	if (policy) {
		max_ia_freq = policy->cpuinfo.max_freq;
		cpufreq_cpu_put(policy);
	} else {
		/*
		 * Default to measured freq if none found, PCU will ensure we
		 * don't go over
		 */
		max_ia_freq = tsc_khz;
	}

	/* Convert from kHz to MHz */
	max_ia_freq /= 1000;

	min_ring_freq = I915_READ(DCLK) & 0xf;
	/* convert DDR frequency from units of 266.6MHz to bandwidth */
	min_ring_freq = mult_frac(min_ring_freq, 8, 3);

	min_gpu_freq = rps->min_freq;
	max_gpu_freq = rps->max_freq;
	if (IS_GEN9_BC(dev_priv) || INTEL_GEN(dev_priv) >= 10) {
		/* Convert GT frequency to 50 HZ units */
		min_gpu_freq /= GEN9_FREQ_SCALER;
		max_gpu_freq /= GEN9_FREQ_SCALER;
	}

	/*
	 * For each potential GPU frequency, load a ring frequency we'd like
	 * to use for memory access.  We do this by specifying the IA frequency
	 * the PCU should use as a reference to determine the ring frequency.
	 */
	for (gpu_freq = max_gpu_freq; gpu_freq >= min_gpu_freq; gpu_freq--) {
		const int diff = max_gpu_freq - gpu_freq;
		unsigned int ia_freq = 0, ring_freq = 0;

		if (IS_GEN9_BC(dev_priv) || INTEL_GEN(dev_priv) >= 10) {
			/*
			 * ring_freq = 2 * GT. ring_freq is in 100MHz units
			 * No floor required for ring frequency on SKL.
			 */
			ring_freq = gpu_freq;
		} else if (INTEL_GEN(dev_priv) >= 8) {
			/* max(2 * GT, DDR). NB: GT is 50MHz units */
			ring_freq = max(min_ring_freq, gpu_freq);
		} else if (IS_HASWELL(dev_priv)) {
			ring_freq = mult_frac(gpu_freq, 5, 4);
			ring_freq = max(min_ring_freq, ring_freq);
			/* leave ia_freq as the default, chosen by cpufreq */
		} else {
			/* On older processors, there is no separate ring
			 * clock domain, so in order to boost the bandwidth
			 * of the ring, we need to upclock the CPU (ia_freq).
			 *
			 * For GPU frequencies less than 750MHz,
			 * just use the lowest ring freq.
			 */
			if (gpu_freq < min_freq)
				ia_freq = 800;
			else
				ia_freq = max_ia_freq - ((diff * scaling_factor) / 2);
			ia_freq = DIV_ROUND_CLOSEST(ia_freq, 100);
		}

		sandybridge_pcode_write(dev_priv,
					GEN6_PCODE_WRITE_MIN_FREQ_TABLE,
					ia_freq << GEN6_PCODE_FREQ_IA_RATIO_SHIFT |
					ring_freq << GEN6_PCODE_FREQ_RING_RATIO_SHIFT |
					gpu_freq);
	}
}

static int cherryview_rps_max_freq(struct drm_i915_private *dev_priv)
{
	u32 val, rp0;

	val = vlv_punit_read(dev_priv, FB_GFX_FMAX_AT_VMAX_FUSE);

	switch (INTEL_INFO(dev_priv)->sseu.eu_total) {
	case 8:
		/* (2 * 4) config */
		rp0 = (val >> FB_GFX_FMAX_AT_VMAX_2SS4EU_FUSE_SHIFT);
		break;
	case 12:
		/* (2 * 6) config */
		rp0 = (val >> FB_GFX_FMAX_AT_VMAX_2SS6EU_FUSE_SHIFT);
		break;
	case 16:
		/* (2 * 8) config */
	default:
		/* Setting (2 * 8) Min RP0 for any other combination */
		rp0 = (val >> FB_GFX_FMAX_AT_VMAX_2SS8EU_FUSE_SHIFT);
		break;
	}

	rp0 = (rp0 & FB_GFX_FREQ_FUSE_MASK);

	return rp0;
}

static int cherryview_rps_rpe_freq(struct drm_i915_private *dev_priv)
{
	u32 val, rpe;

	val = vlv_punit_read(dev_priv, PUNIT_GPU_DUTYCYCLE_REG);
	rpe = (val >> PUNIT_GPU_DUTYCYCLE_RPE_FREQ_SHIFT) & PUNIT_GPU_DUTYCYCLE_RPE_FREQ_MASK;

	return rpe;
}

static int cherryview_rps_guar_freq(struct drm_i915_private *dev_priv)
{
	u32 val, rp1;

	val = vlv_punit_read(dev_priv, FB_GFX_FMAX_AT_VMAX_FUSE);
	rp1 = (val & FB_GFX_FREQ_FUSE_MASK);

	return rp1;
}

static u32 cherryview_rps_min_freq(struct drm_i915_private *dev_priv)
{
	u32 val, rpn;

	val = vlv_punit_read(dev_priv, FB_GFX_FMIN_AT_VMIN_FUSE);
	rpn = ((val >> FB_GFX_FMIN_AT_VMIN_FUSE_SHIFT) &
		       FB_GFX_FREQ_FUSE_MASK);

	return rpn;
}

static int valleyview_rps_guar_freq(struct drm_i915_private *dev_priv)
{
	u32 val, rp1;

	val = vlv_nc_read(dev_priv, IOSF_NC_FB_GFX_FREQ_FUSE);

	rp1 = (val & FB_GFX_FGUARANTEED_FREQ_FUSE_MASK) >> FB_GFX_FGUARANTEED_FREQ_FUSE_SHIFT;

	return rp1;
}

static int valleyview_rps_max_freq(struct drm_i915_private *dev_priv)
{
	u32 val, rp0;

	val = vlv_nc_read(dev_priv, IOSF_NC_FB_GFX_FREQ_FUSE);

	rp0 = (val & FB_GFX_MAX_FREQ_FUSE_MASK) >> FB_GFX_MAX_FREQ_FUSE_SHIFT;
	/* Clamp to max */
	rp0 = min_t(u32, rp0, 0xea);

	return rp0;
}

static int valleyview_rps_rpe_freq(struct drm_i915_private *dev_priv)
{
	u32 val, rpe;

	val = vlv_nc_read(dev_priv, IOSF_NC_FB_GFX_FMAX_FUSE_LO);
	rpe = (val & FB_FMAX_VMIN_FREQ_LO_MASK) >> FB_FMAX_VMIN_FREQ_LO_SHIFT;
	val = vlv_nc_read(dev_priv, IOSF_NC_FB_GFX_FMAX_FUSE_HI);
	rpe |= (val & FB_FMAX_VMIN_FREQ_HI_MASK) << 5;

	return rpe;
}

static int valleyview_rps_min_freq(struct drm_i915_private *dev_priv)
{
	u32 val;

	val = vlv_punit_read(dev_priv, PUNIT_REG_GPU_LFM) & 0xff;
	/*
	 * According to the BYT Punit GPU turbo HAS 1.1.6.3 the minimum value
	 * for the minimum frequency in GPLL mode is 0xc1. Contrary to this on
	 * a BYT-M B0 the above register contains 0xbf. Moreover when setting
	 * a frequency Punit will not allow values below 0xc0. Clamp it 0xc0
	 * to make sure it matches what Punit accepts.
	 */
	return max_t(u32, val, 0xc0);
}

/* Check that the pctx buffer wasn't move under us. */
static void valleyview_check_pctx(struct drm_i915_private *dev_priv)
{
	unsigned long pctx_addr = I915_READ(VLV_PCBR) & ~4095;

	WARN_ON(pctx_addr != dev_priv->dsm.start +
			     dev_priv->vlv_pctx->stolen->start);
}


/* Check that the pcbr address is not empty. */
static void cherryview_check_pctx(struct drm_i915_private *dev_priv)
{
	unsigned long pctx_addr = I915_READ(VLV_PCBR) & ~4095;

	WARN_ON((pctx_addr >> VLV_PCBR_ADDR_SHIFT) == 0);
}

static void cherryview_setup_pctx(struct drm_i915_private *dev_priv)
{
	resource_size_t pctx_paddr, paddr;
	resource_size_t pctx_size = 32*1024;
	u32 pcbr;

	pcbr = I915_READ(VLV_PCBR);
	if ((pcbr >> VLV_PCBR_ADDR_SHIFT) == 0) {
		DRM_DEBUG_DRIVER("BIOS didn't set up PCBR, fixing up\n");
		paddr = dev_priv->dsm.end + 1 - pctx_size;
		GEM_BUG_ON(paddr > U32_MAX);

		pctx_paddr = (paddr & (~4095));
		I915_WRITE(VLV_PCBR, pctx_paddr);
	}

	DRM_DEBUG_DRIVER("PCBR: 0x%08x\n", I915_READ(VLV_PCBR));
}

static void valleyview_setup_pctx(struct drm_i915_private *dev_priv)
{
	struct drm_i915_gem_object *pctx;
	resource_size_t pctx_paddr;
	resource_size_t pctx_size = 24*1024;
	u32 pcbr;

	pcbr = I915_READ(VLV_PCBR);
	if (pcbr) {
		/* BIOS set it up already, grab the pre-alloc'd space */
		resource_size_t pcbr_offset;

		pcbr_offset = (pcbr & (~4095)) - dev_priv->dsm.start;
		pctx = i915_gem_object_create_stolen_for_preallocated(dev_priv,
								      pcbr_offset,
								      I915_GTT_OFFSET_NONE,
								      pctx_size);
		goto out;
	}

	DRM_DEBUG_DRIVER("BIOS didn't set up PCBR, fixing up\n");

	/*
	 * From the Gunit register HAS:
	 * The Gfx driver is expected to program this register and ensure
	 * proper allocation within Gfx stolen memory.  For example, this
	 * register should be programmed such than the PCBR range does not
	 * overlap with other ranges, such as the frame buffer, protected
	 * memory, or any other relevant ranges.
	 */
	pctx = i915_gem_object_create_stolen(dev_priv, pctx_size);
	if (!pctx) {
		DRM_DEBUG("not enough stolen space for PCTX, disabling\n");
		goto out;
	}

	GEM_BUG_ON(range_overflows_t(u64,
				     dev_priv->dsm.start,
				     pctx->stolen->start,
				     U32_MAX));
	pctx_paddr = dev_priv->dsm.start + pctx->stolen->start;
	I915_WRITE(VLV_PCBR, pctx_paddr);

out:
	DRM_DEBUG_DRIVER("PCBR: 0x%08x\n", I915_READ(VLV_PCBR));
	dev_priv->vlv_pctx = pctx;
}

static void valleyview_cleanup_pctx(struct drm_i915_private *dev_priv)
{
	struct drm_i915_gem_object *pctx;

	pctx = fetch_and_zero(&dev_priv->vlv_pctx);
	if (pctx)
		i915_gem_object_put(pctx);
}

static void vlv_init_gpll_ref_freq(struct drm_i915_private *dev_priv)
{
	dev_priv->gt_pm.rps.gpll_ref_freq =
		vlv_get_cck_clock(dev_priv, "GPLL ref",
				  CCK_GPLL_CLOCK_CONTROL,
				  dev_priv->czclk_freq);

	DRM_DEBUG_DRIVER("GPLL reference freq: %d kHz\n",
			 dev_priv->gt_pm.rps.gpll_ref_freq);
}

static void valleyview_init_gt_powersave(struct drm_i915_private *dev_priv)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
	u32 val;

	valleyview_setup_pctx(dev_priv);

	vlv_init_gpll_ref_freq(dev_priv);

	val = vlv_punit_read(dev_priv, PUNIT_REG_GPU_FREQ_STS);
	switch ((val >> 6) & 3) {
	case 0:
	case 1:
		dev_priv->mem_freq = 800;
		break;
	case 2:
		dev_priv->mem_freq = 1066;
		break;
	case 3:
		dev_priv->mem_freq = 1333;
		break;
	}
	DRM_DEBUG_DRIVER("DDR speed: %d MHz\n", dev_priv->mem_freq);

	rps->max_freq = valleyview_rps_max_freq(dev_priv);
	rps->rp0_freq = rps->max_freq;
	DRM_DEBUG_DRIVER("max GPU freq: %d MHz (%u)\n",
			 intel_gpu_freq(dev_priv, rps->max_freq),
			 rps->max_freq);

	rps->efficient_freq = valleyview_rps_rpe_freq(dev_priv);
	DRM_DEBUG_DRIVER("RPe GPU freq: %d MHz (%u)\n",
			 intel_gpu_freq(dev_priv, rps->efficient_freq),
			 rps->efficient_freq);

	rps->rp1_freq = valleyview_rps_guar_freq(dev_priv);
	DRM_DEBUG_DRIVER("RP1(Guar Freq) GPU freq: %d MHz (%u)\n",
			 intel_gpu_freq(dev_priv, rps->rp1_freq),
			 rps->rp1_freq);

	rps->min_freq = valleyview_rps_min_freq(dev_priv);
	DRM_DEBUG_DRIVER("min GPU freq: %d MHz (%u)\n",
			 intel_gpu_freq(dev_priv, rps->min_freq),
			 rps->min_freq);
}

static void cherryview_init_gt_powersave(struct drm_i915_private *dev_priv)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;
	u32 val;

	cherryview_setup_pctx(dev_priv);

	vlv_init_gpll_ref_freq(dev_priv);

	mutex_lock(&dev_priv->sb_lock);
	val = vlv_cck_read(dev_priv, CCK_FUSE_REG);
	mutex_unlock(&dev_priv->sb_lock);

	switch ((val >> 2) & 0x7) {
	case 3:
		dev_priv->mem_freq = 2000;
		break;
	default:
		dev_priv->mem_freq = 1600;
		break;
	}
	DRM_DEBUG_DRIVER("DDR speed: %d MHz\n", dev_priv->mem_freq);

	rps->max_freq = cherryview_rps_max_freq(dev_priv);
	rps->rp0_freq = rps->max_freq;
	DRM_DEBUG_DRIVER("max GPU freq: %d MHz (%u)\n",
			 intel_gpu_freq(dev_priv, rps->max_freq),
			 rps->max_freq);

	rps->efficient_freq = cherryview_rps_rpe_freq(dev_priv);
	DRM_DEBUG_DRIVER("RPe GPU freq: %d MHz (%u)\n",
			 intel_gpu_freq(dev_priv, rps->efficient_freq),
			 rps->efficient_freq);

	rps->rp1_freq = cherryview_rps_guar_freq(dev_priv);
	DRM_DEBUG_DRIVER("RP1(Guar) GPU freq: %d MHz (%u)\n",
			 intel_gpu_freq(dev_priv, rps->rp1_freq),
			 rps->rp1_freq);

	rps->min_freq = cherryview_rps_min_freq(dev_priv);
	DRM_DEBUG_DRIVER("min GPU freq: %d MHz (%u)\n",
			 intel_gpu_freq(dev_priv, rps->min_freq),
			 rps->min_freq);

	WARN_ONCE((rps->max_freq | rps->efficient_freq | rps->rp1_freq |
		   rps->min_freq) & 1,
		  "Odd GPU freq values\n");
}

static void valleyview_cleanup_gt_powersave(struct drm_i915_private *dev_priv)
{
	valleyview_cleanup_pctx(dev_priv);
}

static void cherryview_enable_rc6(struct drm_i915_private *dev_priv)
{
	struct intel_engine_cs *engine;
	enum intel_engine_id id;
	u32 gtfifodbg, rc6_mode, pcbr;

	gtfifodbg = I915_READ(GTFIFODBG) & ~(GT_FIFO_SBDEDICATE_FREE_ENTRY_CHV |
					     GT_FIFO_FREE_ENTRIES_CHV);
	if (gtfifodbg) {
		DRM_DEBUG_DRIVER("GT fifo had a previous error %x\n",
				 gtfifodbg);
		I915_WRITE(GTFIFODBG, gtfifodbg);
	}

	cherryview_check_pctx(dev_priv);

	/* 1a & 1b: Get forcewake during program sequence. Although the driver
	 * hasn't enabled a state yet where we need forcewake, BIOS may have.*/
	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);

	/*  Disable RC states. */
	I915_WRITE(GEN6_RC_CONTROL, 0);

	/* 2a: Program RC6 thresholds.*/
	I915_WRITE(GEN6_RC6_WAKE_RATE_LIMIT, 40 << 16);
	I915_WRITE(GEN6_RC_EVALUATION_INTERVAL, 125000); /* 12500 * 1280ns */
	I915_WRITE(GEN6_RC_IDLE_HYSTERSIS, 25); /* 25 * 1280ns */

	for_each_engine(engine, dev_priv, id)
		I915_WRITE(RING_MAX_IDLE(engine->mmio_base), 10);
	I915_WRITE(GEN6_RC_SLEEP, 0);

	/* TO threshold set to 500 us ( 0x186 * 1.28 us) */
	I915_WRITE(GEN6_RC6_THRESHOLD, 0x186);

	/* Allows RC6 residency counter to work */
	I915_WRITE(VLV_COUNTER_CONTROL,
		   _MASKED_BIT_ENABLE(VLV_COUNT_RANGE_HIGH |
				      VLV_MEDIA_RC6_COUNT_EN |
				      VLV_RENDER_RC6_COUNT_EN));

	/* For now we assume BIOS is allocating and populating the PCBR  */
	pcbr = I915_READ(VLV_PCBR);

	/* 3: Enable RC6 */
	rc6_mode = 0;
	if (pcbr >> VLV_PCBR_ADDR_SHIFT)
		rc6_mode = GEN7_RC_CTL_TO_MODE;
	I915_WRITE(GEN6_RC_CONTROL, rc6_mode);

	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
}

static void cherryview_enable_rps(struct drm_i915_private *dev_priv)
{
	u32 val;

	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);

	/* 1: Program defaults and thresholds for RPS*/
	I915_WRITE(GEN6_RP_DOWN_TIMEOUT, 1000000);
	I915_WRITE(GEN6_RP_UP_THRESHOLD, 59400);
	I915_WRITE(GEN6_RP_DOWN_THRESHOLD, 245000);
	I915_WRITE(GEN6_RP_UP_EI, 66000);
	I915_WRITE(GEN6_RP_DOWN_EI, 350000);

	I915_WRITE(GEN6_RP_IDLE_HYSTERSIS, 10);

	/* 2: Enable RPS */
	I915_WRITE(GEN6_RP_CONTROL,
		   GEN6_RP_MEDIA_HW_NORMAL_MODE |
		   GEN6_RP_MEDIA_IS_GFX |
		   GEN6_RP_ENABLE |
		   GEN6_RP_UP_BUSY_AVG |
		   GEN6_RP_DOWN_IDLE_AVG);

	/* Setting Fixed Bias */
	val = VLV_OVERRIDE_EN |
		  VLV_SOC_TDP_EN |
		  CHV_BIAS_CPU_50_SOC_50;
	vlv_punit_write(dev_priv, VLV_TURBO_SOC_OVERRIDE, val);

	val = vlv_punit_read(dev_priv, PUNIT_REG_GPU_FREQ_STS);

	/* RPS code assumes GPLL is used */
	WARN_ONCE((val & GPLLENABLE) == 0, "GPLL not enabled\n");

	DRM_DEBUG_DRIVER("GPLL enabled? %s\n", yesno(val & GPLLENABLE));
	DRM_DEBUG_DRIVER("GPU status: 0x%08x\n", val);

	reset_rps(dev_priv, valleyview_set_rps);

	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
}

static void valleyview_enable_rc6(struct drm_i915_private *dev_priv)
{
	struct intel_engine_cs *engine;
	enum intel_engine_id id;
	u32 gtfifodbg;

	valleyview_check_pctx(dev_priv);

	gtfifodbg = I915_READ(GTFIFODBG);
	if (gtfifodbg) {
		DRM_DEBUG_DRIVER("GT fifo had a previous error %x\n",
				 gtfifodbg);
		I915_WRITE(GTFIFODBG, gtfifodbg);
	}

	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);

	/*  Disable RC states. */
	I915_WRITE(GEN6_RC_CONTROL, 0);

	I915_WRITE(GEN6_RC6_WAKE_RATE_LIMIT, 0x00280000);
	I915_WRITE(GEN6_RC_EVALUATION_INTERVAL, 125000);
	I915_WRITE(GEN6_RC_IDLE_HYSTERSIS, 25);

	for_each_engine(engine, dev_priv, id)
		I915_WRITE(RING_MAX_IDLE(engine->mmio_base), 10);

	I915_WRITE(GEN6_RC6_THRESHOLD, 0x557);

	/* Allows RC6 residency counter to work */
	I915_WRITE(VLV_COUNTER_CONTROL,
		   _MASKED_BIT_ENABLE(VLV_COUNT_RANGE_HIGH |
				      VLV_MEDIA_RC0_COUNT_EN |
				      VLV_RENDER_RC0_COUNT_EN |
				      VLV_MEDIA_RC6_COUNT_EN |
				      VLV_RENDER_RC6_COUNT_EN));

	I915_WRITE(GEN6_RC_CONTROL,
		   GEN7_RC_CTL_TO_MODE | VLV_RC_CTL_CTX_RST_PARALLEL);

	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
}

static void valleyview_enable_rps(struct drm_i915_private *dev_priv)
{
	u32 val;

	intel_uncore_forcewake_get(dev_priv, FORCEWAKE_ALL);

	I915_WRITE(GEN6_RP_DOWN_TIMEOUT, 1000000);
	I915_WRITE(GEN6_RP_UP_THRESHOLD, 59400);
	I915_WRITE(GEN6_RP_DOWN_THRESHOLD, 245000);
	I915_WRITE(GEN6_RP_UP_EI, 66000);
	I915_WRITE(GEN6_RP_DOWN_EI, 350000);

	I915_WRITE(GEN6_RP_IDLE_HYSTERSIS, 10);

	I915_WRITE(GEN6_RP_CONTROL,
		   GEN6_RP_MEDIA_TURBO |
		   GEN6_RP_MEDIA_HW_NORMAL_MODE |
		   GEN6_RP_MEDIA_IS_GFX |
		   GEN6_RP_ENABLE |
		   GEN6_RP_UP_BUSY_AVG |
		   GEN6_RP_DOWN_IDLE_CONT);

	/* Setting Fixed Bias */
	val = VLV_OVERRIDE_EN |
		  VLV_SOC_TDP_EN |
		  VLV_BIAS_CPU_125_SOC_875;
	vlv_punit_write(dev_priv, VLV_TURBO_SOC_OVERRIDE, val);

	val = vlv_punit_read(dev_priv, PUNIT_REG_GPU_FREQ_STS);

	/* RPS code assumes GPLL is used */
	WARN_ONCE((val & GPLLENABLE) == 0, "GPLL not enabled\n");

	DRM_DEBUG_DRIVER("GPLL enabled? %s\n", yesno(val & GPLLENABLE));
	DRM_DEBUG_DRIVER("GPU status: 0x%08x\n", val);

	reset_rps(dev_priv, valleyview_set_rps);

	intel_uncore_forcewake_put(dev_priv, FORCEWAKE_ALL);
}

static unsigned long intel_pxfreq(u32 vidfreq)
{
	unsigned long freq;
	int div = (vidfreq & 0x3f0000) >> 16;
	int post = (vidfreq & 0x3000) >> 12;
	int pre = (vidfreq & 0x7);

	if (!pre)
		return 0;

	freq = ((div * 133333) / ((1<<post) * pre));

	return freq;
}

static const struct cparams {
	u16 i;
	u16 t;
	u16 m;
	u16 c;
} cparams[] = {
	{ 1, 1333, 301, 28664 },
	{ 1, 1066, 294, 24460 },
	{ 1, 800, 294, 25192 },
	{ 0, 1333, 276, 27605 },
	{ 0, 1066, 276, 27605 },
	{ 0, 800, 231, 23784 },
};

static unsigned long __i915_chipset_val(struct drm_i915_private *dev_priv)
{
	u64 total_count, diff, ret;
	u32 count1, count2, count3, m = 0, c = 0;
	unsigned long now = jiffies_to_msecs(jiffies), diff1;
	int i;

	lockdep_assert_held(&mchdev_lock);

	diff1 = now - dev_priv->ips.last_time1;

	/* Prevent division-by-zero if we are asking too fast.
	 * Also, we don't get interesting results if we are polling
	 * faster than once in 10ms, so just return the saved value
	 * in such cases.
	 */
	if (diff1 <= 10)
		return dev_priv->ips.chipset_power;

	count1 = I915_READ(DMIEC);
	count2 = I915_READ(DDREC);
	count3 = I915_READ(CSIEC);

	total_count = count1 + count2 + count3;

	/* FIXME: handle per-counter overflow */
	if (total_count < dev_priv->ips.last_count1) {
		diff = ~0UL - dev_priv->ips.last_count1;
		diff += total_count;
	} else {
		diff = total_count - dev_priv->ips.last_count1;
	}

	for (i = 0; i < ARRAY_SIZE(cparams); i++) {
		if (cparams[i].i == dev_priv->ips.c_m &&
		    cparams[i].t == dev_priv->ips.r_t) {
			m = cparams[i].m;
			c = cparams[i].c;
			break;
		}
	}

	diff = div_u64(diff, diff1);
	ret = ((m * diff) + c);
	ret = div_u64(ret, 10);

	dev_priv->ips.last_count1 = total_count;
	dev_priv->ips.last_time1 = now;

	dev_priv->ips.chipset_power = ret;

	return ret;
}

unsigned long i915_chipset_val(struct drm_i915_private *dev_priv)
{
	unsigned long val;

	if (!IS_GEN5(dev_priv))
		return 0;

	spin_lock_irq(&mchdev_lock);

	val = __i915_chipset_val(dev_priv);

	spin_unlock_irq(&mchdev_lock);

	return val;
}

unsigned long i915_mch_val(struct drm_i915_private *dev_priv)
{
	unsigned long m, x, b;
	u32 tsfs;

	tsfs = I915_READ(TSFS);

	m = ((tsfs & TSFS_SLOPE_MASK) >> TSFS_SLOPE_SHIFT);
	x = I915_READ8(TR1);

	b = tsfs & TSFS_INTR_MASK;

	return ((m * x) / 127) - b;
}

static int _pxvid_to_vd(u8 pxvid)
{
	if (pxvid == 0)
		return 0;

	if (pxvid >= 8 && pxvid < 31)
		pxvid = 31;

	return (pxvid + 2) * 125;
}

static u32 pvid_to_extvid(struct drm_i915_private *dev_priv, u8 pxvid)
{
	const int vd = _pxvid_to_vd(pxvid);
	const int vm = vd - 1125;

	if (INTEL_INFO(dev_priv)->is_mobile)
		return vm > 0 ? vm : 0;

	return vd;
}

static void __i915_update_gfx_val(struct drm_i915_private *dev_priv)
{
	u64 now, diff, diffms;
	u32 count;

	lockdep_assert_held(&mchdev_lock);

	now = ktime_get_raw_ns();
	diffms = now - dev_priv->ips.last_time2;
	do_div(diffms, NSEC_PER_MSEC);

	/* Don't divide by 0 */
	if (!diffms)
		return;

	count = I915_READ(GFXEC);

	if (count < dev_priv->ips.last_count2) {
		diff = ~0UL - dev_priv->ips.last_count2;
		diff += count;
	} else {
		diff = count - dev_priv->ips.last_count2;
	}

	dev_priv->ips.last_count2 = count;
	dev_priv->ips.last_time2 = now;

	/* More magic constants... */
	diff = diff * 1181;
	diff = div_u64(diff, diffms * 10);
	dev_priv->ips.gfx_power = diff;
}

void i915_update_gfx_val(struct drm_i915_private *dev_priv)
{
	if (!IS_GEN5(dev_priv))
		return;

	spin_lock_irq(&mchdev_lock);

	__i915_update_gfx_val(dev_priv);

	spin_unlock_irq(&mchdev_lock);
}

static unsigned long __i915_gfx_val(struct drm_i915_private *dev_priv)
{
	unsigned long t, corr, state1, corr2, state2;
	u32 pxvid, ext_v;

	lockdep_assert_held(&mchdev_lock);

	pxvid = I915_READ(PXVFREQ(dev_priv->gt_pm.rps.cur_freq));
	pxvid = (pxvid >> 24) & 0x7f;
	ext_v = pvid_to_extvid(dev_priv, pxvid);

	state1 = ext_v;

	t = i915_mch_val(dev_priv);

	/* Revel in the empirically derived constants */

	/* Correction factor in 1/100000 units */
	if (t > 80)
		corr = ((t * 2349) + 135940);
	else if (t >= 50)
		corr = ((t * 964) + 29317);
	else /* < 50 */
		corr = ((t * 301) + 1004);

	corr = corr * ((150142 * state1) / 10000 - 78642);
	corr /= 100000;
	corr2 = (corr * dev_priv->ips.corr);

	state2 = (corr2 * state1) / 10000;
	state2 /= 100; /* convert to mW */

	__i915_update_gfx_val(dev_priv);

	return dev_priv->ips.gfx_power + state2;
}

unsigned long i915_gfx_val(struct drm_i915_private *dev_priv)
{
	unsigned long val;

	if (!IS_GEN5(dev_priv))
		return 0;

	spin_lock_irq(&mchdev_lock);

	val = __i915_gfx_val(dev_priv);

	spin_unlock_irq(&mchdev_lock);

	return val;
}

/**
 * i915_read_mch_val - return value for IPS use
 *
 * Calculate and return a value for the IPS driver to use when deciding whether
 * we have thermal and power headroom to increase CPU or GPU power budget.
 */
unsigned long i915_read_mch_val(void)
{
	struct drm_i915_private *dev_priv;
	unsigned long chipset_val, graphics_val, ret = 0;

	spin_lock_irq(&mchdev_lock);
	if (!i915_mch_dev)
		goto out_unlock;
	dev_priv = i915_mch_dev;

	chipset_val = __i915_chipset_val(dev_priv);
	graphics_val = __i915_gfx_val(dev_priv);

	ret = chipset_val + graphics_val;

out_unlock:
	spin_unlock_irq(&mchdev_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(i915_read_mch_val);

/**
 * i915_gpu_raise - raise GPU frequency limit
 *
 * Raise the limit; IPS indicates we have thermal headroom.
 */
bool i915_gpu_raise(void)
{
	struct drm_i915_private *dev_priv;
	bool ret = true;

	spin_lock_irq(&mchdev_lock);
	if (!i915_mch_dev) {
		ret = false;
		goto out_unlock;
	}
	dev_priv = i915_mch_dev;

	if (dev_priv->ips.max_delay > dev_priv->ips.fmax)
		dev_priv->ips.max_delay--;

out_unlock:
	spin_unlock_irq(&mchdev_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(i915_gpu_raise);

/**
 * i915_gpu_lower - lower GPU frequency limit
 *
 * IPS indicates we're close to a thermal limit, so throttle back the GPU
 * frequency maximum.
 */
bool i915_gpu_lower(void)
{
	struct drm_i915_private *dev_priv;
	bool ret = true;

	spin_lock_irq(&mchdev_lock);
	if (!i915_mch_dev) {
		ret = false;
		goto out_unlock;
	}
	dev_priv = i915_mch_dev;

	if (dev_priv->ips.max_delay < dev_priv->ips.min_delay)
		dev_priv->ips.max_delay++;

out_unlock:
	spin_unlock_irq(&mchdev_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(i915_gpu_lower);

/**
 * i915_gpu_busy - indicate GPU business to IPS
 *
 * Tell the IPS driver whether or not the GPU is busy.
 */
bool i915_gpu_busy(void)
{
	bool ret = false;

	spin_lock_irq(&mchdev_lock);
	if (i915_mch_dev)
		ret = i915_mch_dev->gt.awake;
	spin_unlock_irq(&mchdev_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(i915_gpu_busy);

/**
 * i915_gpu_turbo_disable - disable graphics turbo
 *
 * Disable graphics turbo by resetting the max frequency and setting the
 * current frequency to the default.
 */
bool i915_gpu_turbo_disable(void)
{
	struct drm_i915_private *dev_priv;
	bool ret = true;

	spin_lock_irq(&mchdev_lock);
	if (!i915_mch_dev) {
		ret = false;
		goto out_unlock;
	}
	dev_priv = i915_mch_dev;

	dev_priv->ips.max_delay = dev_priv->ips.fstart;

	if (!ironlake_set_drps(dev_priv, dev_priv->ips.fstart))
		ret = false;

out_unlock:
	spin_unlock_irq(&mchdev_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(i915_gpu_turbo_disable);

/**
 * Tells the intel_ips driver that the i915 driver is now loaded, if
 * IPS got loaded first.
 *
 * This awkward dance is so that neither module has to depend on the
 * other in order for IPS to do the appropriate communication of
 * GPU turbo limits to i915.
 */
static void
ips_ping_for_i915_load(void)
{
	void (*link)(void);

	link = symbol_get(ips_link_to_i915_driver);
	if (link) {
		link();
		symbol_put(ips_link_to_i915_driver);
	}
}

void intel_gpu_ips_init(struct drm_i915_private *dev_priv)
{
	/* We only register the i915 ips part with intel-ips once everything is
	 * set up, to avoid intel-ips sneaking in and reading bogus values. */
	spin_lock_irq(&mchdev_lock);
	i915_mch_dev = dev_priv;
	spin_unlock_irq(&mchdev_lock);

	ips_ping_for_i915_load();
}

void intel_gpu_ips_teardown(void)
{
	spin_lock_irq(&mchdev_lock);
	i915_mch_dev = NULL;
	spin_unlock_irq(&mchdev_lock);
}

static void intel_init_emon(struct drm_i915_private *dev_priv)
{
	u32 lcfuse;
	u8 pxw[16];
	int i;

	/* Disable to program */
	I915_WRITE(ECR, 0);
	POSTING_READ(ECR);

	/* Program energy weights for various events */
	I915_WRITE(SDEW, 0x15040d00);
	I915_WRITE(CSIEW0, 0x007f0000);
	I915_WRITE(CSIEW1, 0x1e220004);
	I915_WRITE(CSIEW2, 0x04000004);

	for (i = 0; i < 5; i++)
		I915_WRITE(PEW(i), 0);
	for (i = 0; i < 3; i++)
		I915_WRITE(DEW(i), 0);

	/* Program P-state weights to account for frequency power adjustment */
	for (i = 0; i < 16; i++) {
		u32 pxvidfreq = I915_READ(PXVFREQ(i));
		unsigned long freq = intel_pxfreq(pxvidfreq);
		unsigned long vid = (pxvidfreq & PXVFREQ_PX_MASK) >>
			PXVFREQ_PX_SHIFT;
		unsigned long val;

		val = vid * vid;
		val *= (freq / 1000);
		val *= 255;
		val /= (127*127*900);
		if (val > 0xff)
			DRM_ERROR("bad pxval: %ld\n", val);
		pxw[i] = val;
	}
	/* Render standby states get 0 weight */
	pxw[14] = 0;
	pxw[15] = 0;

	for (i = 0; i < 4; i++) {
		u32 val = (pxw[i*4] << 24) | (pxw[(i*4)+1] << 16) |
			(pxw[(i*4)+2] << 8) | (pxw[(i*4)+3]);
		I915_WRITE(PXW(i), val);
	}

	/* Adjust magic regs to magic values (more experimental results) */
	I915_WRITE(OGW0, 0);
	I915_WRITE(OGW1, 0);
	I915_WRITE(EG0, 0x00007f00);
	I915_WRITE(EG1, 0x0000000e);
	I915_WRITE(EG2, 0x000e0000);
	I915_WRITE(EG3, 0x68000300);
	I915_WRITE(EG4, 0x42000000);
	I915_WRITE(EG5, 0x00140031);
	I915_WRITE(EG6, 0);
	I915_WRITE(EG7, 0);

	for (i = 0; i < 8; i++)
		I915_WRITE(PXWL(i), 0);

	/* Enable PMON + select events */
	I915_WRITE(ECR, 0x80000019);

	lcfuse = I915_READ(LCFUSE02);

	dev_priv->ips.corr = (lcfuse & LCFUSE_HIV_MASK);
}

static bool i915_rc6_ctx_corrupted(struct drm_i915_private *dev_priv)
{
	return !I915_READ(GEN8_RC6_CTX_INFO);
}

static void i915_rc6_ctx_wa_init(struct drm_i915_private *i915)
{
	if (!NEEDS_RC6_CTX_CORRUPTION_WA(i915))
		return;

	if (i915_rc6_ctx_corrupted(i915)) {
		DRM_INFO("RC6 context corrupted, disabling runtime power management\n");
		i915->gt_pm.rc6.ctx_corrupted = true;
		intel_runtime_pm_get(i915);
	}
}

static void i915_rc6_ctx_wa_cleanup(struct drm_i915_private *i915)
{
	if (i915->gt_pm.rc6.ctx_corrupted) {
		intel_runtime_pm_put(i915);
		i915->gt_pm.rc6.ctx_corrupted = false;
	}
}

/**
 * i915_rc6_ctx_wa_suspend - system suspend sequence for the RC6 CTX WA
 * @i915: i915 device
 *
 * Perform any steps needed to clean up the RC6 CTX WA before system suspend.
 */
void i915_rc6_ctx_wa_suspend(struct drm_i915_private *i915)
{
	if (i915->gt_pm.rc6.ctx_corrupted)
		intel_runtime_pm_put(i915);
}

/**
 * i915_rc6_ctx_wa_resume - system resume sequence for the RC6 CTX WA
 * @i915: i915 device
 *
 * Perform any steps needed to re-init the RC6 CTX WA after system resume.
 */
void i915_rc6_ctx_wa_resume(struct drm_i915_private *i915)
{
	if (!i915->gt_pm.rc6.ctx_corrupted)
		return;

	if (i915_rc6_ctx_corrupted(i915)) {
		intel_runtime_pm_get(i915);
		return;
	}

	DRM_INFO("RC6 context restored, re-enabling runtime power management\n");
	i915->gt_pm.rc6.ctx_corrupted = false;
}

static void intel_disable_rc6(struct drm_i915_private *dev_priv);

/**
 * i915_rc6_ctx_wa_check - check for a new RC6 CTX corruption
 * @i915: i915 device
 *
 * Check if an RC6 CTX corruption has happened since the last check and if so
 * disable RC6 and runtime power management.
 *
 * Return false if no context corruption has happened since the last call of
 * this function, true otherwise.
*/
bool i915_rc6_ctx_wa_check(struct drm_i915_private *i915)
{
	if (!NEEDS_RC6_CTX_CORRUPTION_WA(i915))
		return false;

	if (i915->gt_pm.rc6.ctx_corrupted)
		return false;

	if (!i915_rc6_ctx_corrupted(i915))
		return false;

	DRM_NOTE("RC6 context corruption, disabling runtime power management\n");

	intel_disable_rc6(i915);
	i915->gt_pm.rc6.ctx_corrupted = true;
	intel_runtime_pm_get_noresume(i915);

	return true;
}

void intel_init_gt_powersave(struct drm_i915_private *dev_priv)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;

	/*
	 * RPM depends on RC6 to save restore the GT HW context, so make RC6 a
	 * requirement.
	 */
	if (!sanitize_rc6(dev_priv)) {
		DRM_INFO("RC6 disabled, disabling runtime PM support\n");
		intel_runtime_pm_get(dev_priv);
	}

	mutex_lock(&dev_priv->pcu_lock);

	i915_rc6_ctx_wa_init(dev_priv);

	/* Initialize RPS limits (for userspace) */
	if (IS_CHERRYVIEW(dev_priv))
		cherryview_init_gt_powersave(dev_priv);
	else if (IS_VALLEYVIEW(dev_priv))
		valleyview_init_gt_powersave(dev_priv);
	else if (INTEL_GEN(dev_priv) >= 6)
		gen6_init_rps_frequencies(dev_priv);

	/* Derive initial user preferences/limits from the hardware limits */
	rps->idle_freq = rps->min_freq;
	rps->cur_freq = rps->idle_freq;

	rps->max_freq_softlimit = rps->max_freq;
	rps->min_freq_softlimit = rps->min_freq;

	if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv))
		rps->min_freq_softlimit =
			max_t(int,
			      rps->efficient_freq,
			      intel_freq_opcode(dev_priv, 450));

	/* After setting max-softlimit, find the overclock max freq */
	if (IS_GEN6(dev_priv) ||
	    IS_IVYBRIDGE(dev_priv) || IS_HASWELL(dev_priv)) {
		u32 params = 0;

		sandybridge_pcode_read(dev_priv, GEN6_READ_OC_PARAMS, &params);
		if (params & BIT(31)) { /* OC supported */
			DRM_DEBUG_DRIVER("Overclocking supported, max: %dMHz, overclock: %dMHz\n",
					 (rps->max_freq & 0xff) * 50,
					 (params & 0xff) * 50);
			rps->max_freq = params & 0xff;
		}
	}

	/* Finally allow us to boost to max by default */
	rps->boost_freq = rps->max_freq;

	mutex_unlock(&dev_priv->pcu_lock);
}

void intel_cleanup_gt_powersave(struct drm_i915_private *dev_priv)
{
	if (IS_VALLEYVIEW(dev_priv))
		valleyview_cleanup_gt_powersave(dev_priv);

	i915_rc6_ctx_wa_cleanup(dev_priv);

	if (!HAS_RC6(dev_priv))
		intel_runtime_pm_put(dev_priv);
}

/**
 * intel_suspend_gt_powersave - suspend PM work and helper threads
 * @dev_priv: i915 device
 *
 * We don't want to disable RC6 or other features here, we just want
 * to make sure any work we've queued has finished and won't bother
 * us while we're suspended.
 */
void intel_suspend_gt_powersave(struct drm_i915_private *dev_priv)
{
	if (INTEL_GEN(dev_priv) < 6)
		return;

	/* gen6_rps_idle() will be called later to disable interrupts */
}

void intel_sanitize_gt_powersave(struct drm_i915_private *dev_priv)
{
	dev_priv->gt_pm.rps.enabled = true; /* force RPS disabling */
	dev_priv->gt_pm.rc6.enabled = true; /* force RC6 disabling */
	intel_disable_gt_powersave(dev_priv);

	if (INTEL_GEN(dev_priv) >= 11)
		gen11_reset_rps_interrupts(dev_priv);
	else
		gen6_reset_rps_interrupts(dev_priv);
}

static inline void intel_disable_llc_pstate(struct drm_i915_private *i915)
{
	lockdep_assert_held(&i915->pcu_lock);

	if (!i915->gt_pm.llc_pstate.enabled)
		return;

	/* Currently there is no HW configuration to be done to disable. */

	i915->gt_pm.llc_pstate.enabled = false;
}

static void __intel_disable_rc6(struct drm_i915_private *dev_priv)
{
	lockdep_assert_held(&dev_priv->pcu_lock);

	if (!dev_priv->gt_pm.rc6.enabled)
		return;

	if (INTEL_GEN(dev_priv) >= 9)
		gen9_disable_rc6(dev_priv);
	else if (IS_CHERRYVIEW(dev_priv))
		cherryview_disable_rc6(dev_priv);
	else if (IS_VALLEYVIEW(dev_priv))
		valleyview_disable_rc6(dev_priv);
	else if (INTEL_GEN(dev_priv) >= 6)
		gen6_disable_rc6(dev_priv);

	dev_priv->gt_pm.rc6.enabled = false;
}

static void intel_disable_rc6(struct drm_i915_private *dev_priv)
{
	mutex_lock(&dev_priv->pcu_lock);
	__intel_disable_rc6(dev_priv);
	mutex_unlock(&dev_priv->pcu_lock);
}

static void intel_disable_rps(struct drm_i915_private *dev_priv)
{
	lockdep_assert_held(&dev_priv->pcu_lock);

	if (!dev_priv->gt_pm.rps.enabled)
		return;

	if (INTEL_GEN(dev_priv) >= 9)
		gen9_disable_rps(dev_priv);
	else if (IS_CHERRYVIEW(dev_priv))
		cherryview_disable_rps(dev_priv);
	else if (IS_VALLEYVIEW(dev_priv))
		valleyview_disable_rps(dev_priv);
	else if (INTEL_GEN(dev_priv) >= 6)
		gen6_disable_rps(dev_priv);
	else if (IS_IRONLAKE_M(dev_priv))
		ironlake_disable_drps(dev_priv);

	dev_priv->gt_pm.rps.enabled = false;
}

void intel_disable_gt_powersave(struct drm_i915_private *dev_priv)
{
	mutex_lock(&dev_priv->pcu_lock);

	__intel_disable_rc6(dev_priv);
	intel_disable_rps(dev_priv);
	if (HAS_LLC(dev_priv))
		intel_disable_llc_pstate(dev_priv);

	mutex_unlock(&dev_priv->pcu_lock);
}

static inline void intel_enable_llc_pstate(struct drm_i915_private *i915)
{
	lockdep_assert_held(&i915->pcu_lock);

	if (i915->gt_pm.llc_pstate.enabled)
		return;

	gen6_update_ring_freq(i915);

	i915->gt_pm.llc_pstate.enabled = true;
}

static void intel_enable_rc6(struct drm_i915_private *dev_priv)
{
	lockdep_assert_held(&dev_priv->pcu_lock);

	if (dev_priv->gt_pm.rc6.enabled)
		return;

	if (dev_priv->gt_pm.rc6.ctx_corrupted)
		return;

	if (IS_CHERRYVIEW(dev_priv))
		cherryview_enable_rc6(dev_priv);
	else if (IS_VALLEYVIEW(dev_priv))
		valleyview_enable_rc6(dev_priv);
	else if (INTEL_GEN(dev_priv) >= 9)
		gen9_enable_rc6(dev_priv);
	else if (IS_BROADWELL(dev_priv))
		gen8_enable_rc6(dev_priv);
	else if (INTEL_GEN(dev_priv) >= 6)
		gen6_enable_rc6(dev_priv);

	dev_priv->gt_pm.rc6.enabled = true;
}

static void intel_enable_rps(struct drm_i915_private *dev_priv)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;

	lockdep_assert_held(&dev_priv->pcu_lock);

	if (rps->enabled)
		return;

	if (IS_CHERRYVIEW(dev_priv)) {
		cherryview_enable_rps(dev_priv);
	} else if (IS_VALLEYVIEW(dev_priv)) {
		valleyview_enable_rps(dev_priv);
	} else if (INTEL_GEN(dev_priv) >= 9) {
		gen9_enable_rps(dev_priv);
	} else if (IS_BROADWELL(dev_priv)) {
		gen8_enable_rps(dev_priv);
	} else if (INTEL_GEN(dev_priv) >= 6) {
		gen6_enable_rps(dev_priv);
	} else if (IS_IRONLAKE_M(dev_priv)) {
		ironlake_enable_drps(dev_priv);
		intel_init_emon(dev_priv);
	}

	WARN_ON(rps->max_freq < rps->min_freq);
	WARN_ON(rps->idle_freq > rps->max_freq);

	WARN_ON(rps->efficient_freq < rps->min_freq);
	WARN_ON(rps->efficient_freq > rps->max_freq);

	rps->enabled = true;
}

void intel_enable_gt_powersave(struct drm_i915_private *dev_priv)
{
	/* Powersaving is controlled by the host when inside a VM */
	if (intel_vgpu_active(dev_priv))
		return;

	mutex_lock(&dev_priv->pcu_lock);

	if (HAS_RC6(dev_priv))
		intel_enable_rc6(dev_priv);
	intel_enable_rps(dev_priv);
	if (HAS_LLC(dev_priv))
		intel_enable_llc_pstate(dev_priv);

	mutex_unlock(&dev_priv->pcu_lock);
}

=======
	if (!HAS_IPC(dev_priv))
		return;

	dev_priv->ipc_enabled = intel_can_enable_ipc(dev_priv);

	intel_enable_ipc(dev_priv);
}

>>>>>>> upstream/android-13
static void ibx_init_clock_gating(struct drm_i915_private *dev_priv)
{
	/*
	 * On Ibex Peak and Cougar Point, we need to disable clock
	 * gating for the panel power sequencer or it will fail to
	 * start up when no ports are active.
	 */
<<<<<<< HEAD
	I915_WRITE(SOUTH_DSPCLK_GATE_D, PCH_DPLSUNIT_CLOCK_GATE_DISABLE);
=======
	intel_uncore_write(&dev_priv->uncore, SOUTH_DSPCLK_GATE_D, PCH_DPLSUNIT_CLOCK_GATE_DISABLE);
>>>>>>> upstream/android-13
}

static void g4x_disable_trickle_feed(struct drm_i915_private *dev_priv)
{
	enum pipe pipe;

	for_each_pipe(dev_priv, pipe) {
<<<<<<< HEAD
		I915_WRITE(DSPCNTR(pipe),
			   I915_READ(DSPCNTR(pipe)) |
			   DISPPLANE_TRICKLE_FEED_DISABLE);

		I915_WRITE(DSPSURF(pipe), I915_READ(DSPSURF(pipe)));
		POSTING_READ(DSPSURF(pipe));
=======
		intel_uncore_write(&dev_priv->uncore, DSPCNTR(pipe),
			   intel_uncore_read(&dev_priv->uncore, DSPCNTR(pipe)) |
			   DISPPLANE_TRICKLE_FEED_DISABLE);

		intel_uncore_write(&dev_priv->uncore, DSPSURF(pipe), intel_uncore_read(&dev_priv->uncore, DSPSURF(pipe)));
		intel_uncore_posting_read(&dev_priv->uncore, DSPSURF(pipe));
>>>>>>> upstream/android-13
	}
}

static void ilk_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	uint32_t dspclk_gate = ILK_VRHUNIT_CLOCK_GATE_DISABLE;
=======
	u32 dspclk_gate = ILK_VRHUNIT_CLOCK_GATE_DISABLE;
>>>>>>> upstream/android-13

	/*
	 * Required for FBC
	 * WaFbcDisableDpfcClockGating:ilk
	 */
	dspclk_gate |= ILK_DPFCRUNIT_CLOCK_GATE_DISABLE |
		   ILK_DPFCUNIT_CLOCK_GATE_DISABLE |
		   ILK_DPFDUNIT_CLOCK_GATE_ENABLE;

<<<<<<< HEAD
	I915_WRITE(PCH_3DCGDIS0,
		   MARIUNIT_CLOCK_GATE_DISABLE |
		   SVSMUNIT_CLOCK_GATE_DISABLE);
	I915_WRITE(PCH_3DCGDIS1,
=======
	intel_uncore_write(&dev_priv->uncore, PCH_3DCGDIS0,
		   MARIUNIT_CLOCK_GATE_DISABLE |
		   SVSMUNIT_CLOCK_GATE_DISABLE);
	intel_uncore_write(&dev_priv->uncore, PCH_3DCGDIS1,
>>>>>>> upstream/android-13
		   VFMUNIT_CLOCK_GATE_DISABLE);

	/*
	 * According to the spec the following bits should be set in
	 * order to enable memory self-refresh
	 * The bit 22/21 of 0x42004
	 * The bit 5 of 0x42020
	 * The bit 15 of 0x45000
	 */
<<<<<<< HEAD
	I915_WRITE(ILK_DISPLAY_CHICKEN2,
		   (I915_READ(ILK_DISPLAY_CHICKEN2) |
		    ILK_DPARB_GATE | ILK_VSDPFD_FULL));
	dspclk_gate |= ILK_DPARBUNIT_CLOCK_GATE_ENABLE;
	I915_WRITE(DISP_ARB_CTL,
		   (I915_READ(DISP_ARB_CTL) |
=======
	intel_uncore_write(&dev_priv->uncore, ILK_DISPLAY_CHICKEN2,
		   (intel_uncore_read(&dev_priv->uncore, ILK_DISPLAY_CHICKEN2) |
		    ILK_DPARB_GATE | ILK_VSDPFD_FULL));
	dspclk_gate |= ILK_DPARBUNIT_CLOCK_GATE_ENABLE;
	intel_uncore_write(&dev_priv->uncore, DISP_ARB_CTL,
		   (intel_uncore_read(&dev_priv->uncore, DISP_ARB_CTL) |
>>>>>>> upstream/android-13
		    DISP_FBC_WM_DIS));

	/*
	 * Based on the document from hardware guys the following bits
	 * should be set unconditionally in order to enable FBC.
	 * The bit 22 of 0x42000
	 * The bit 22 of 0x42004
	 * The bit 7,8,9 of 0x42020.
	 */
	if (IS_IRONLAKE_M(dev_priv)) {
		/* WaFbcAsynchFlipDisableFbcQueue:ilk */
<<<<<<< HEAD
		I915_WRITE(ILK_DISPLAY_CHICKEN1,
			   I915_READ(ILK_DISPLAY_CHICKEN1) |
			   ILK_FBCQ_DIS);
		I915_WRITE(ILK_DISPLAY_CHICKEN2,
			   I915_READ(ILK_DISPLAY_CHICKEN2) |
			   ILK_DPARB_GATE);
	}

	I915_WRITE(ILK_DSPCLK_GATE_D, dspclk_gate);

	I915_WRITE(ILK_DISPLAY_CHICKEN2,
		   I915_READ(ILK_DISPLAY_CHICKEN2) |
		   ILK_ELPIN_409_SELECT);
	I915_WRITE(_3D_CHICKEN2,
		   _3D_CHICKEN2_WM_READ_PIPELINED << 16 |
		   _3D_CHICKEN2_WM_READ_PIPELINED);

	/* WaDisableRenderCachePipelinedFlush:ilk */
	I915_WRITE(CACHE_MODE_0,
		   _MASKED_BIT_ENABLE(CM0_PIPELINED_RENDER_FLUSH_DISABLE));

	/* WaDisable_RenderCache_OperationalFlush:ilk */
	I915_WRITE(CACHE_MODE_0, _MASKED_BIT_DISABLE(RC_OP_FLUSH_ENABLE));
=======
		intel_uncore_write(&dev_priv->uncore, ILK_DISPLAY_CHICKEN1,
			   intel_uncore_read(&dev_priv->uncore, ILK_DISPLAY_CHICKEN1) |
			   ILK_FBCQ_DIS);
		intel_uncore_write(&dev_priv->uncore, ILK_DISPLAY_CHICKEN2,
			   intel_uncore_read(&dev_priv->uncore, ILK_DISPLAY_CHICKEN2) |
			   ILK_DPARB_GATE);
	}

	intel_uncore_write(&dev_priv->uncore, ILK_DSPCLK_GATE_D, dspclk_gate);

	intel_uncore_write(&dev_priv->uncore, ILK_DISPLAY_CHICKEN2,
		   intel_uncore_read(&dev_priv->uncore, ILK_DISPLAY_CHICKEN2) |
		   ILK_ELPIN_409_SELECT);
>>>>>>> upstream/android-13

	g4x_disable_trickle_feed(dev_priv);

	ibx_init_clock_gating(dev_priv);
}

static void cpt_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	int pipe;
	uint32_t val;
=======
	enum pipe pipe;
	u32 val;
>>>>>>> upstream/android-13

	/*
	 * On Ibex Peak and Cougar Point, we need to disable clock
	 * gating for the panel power sequencer or it will fail to
	 * start up when no ports are active.
	 */
<<<<<<< HEAD
	I915_WRITE(SOUTH_DSPCLK_GATE_D, PCH_DPLSUNIT_CLOCK_GATE_DISABLE |
		   PCH_DPLUNIT_CLOCK_GATE_DISABLE |
		   PCH_CPUNIT_CLOCK_GATE_DISABLE);
	I915_WRITE(SOUTH_CHICKEN2, I915_READ(SOUTH_CHICKEN2) |
=======
	intel_uncore_write(&dev_priv->uncore, SOUTH_DSPCLK_GATE_D, PCH_DPLSUNIT_CLOCK_GATE_DISABLE |
		   PCH_DPLUNIT_CLOCK_GATE_DISABLE |
		   PCH_CPUNIT_CLOCK_GATE_DISABLE);
	intel_uncore_write(&dev_priv->uncore, SOUTH_CHICKEN2, intel_uncore_read(&dev_priv->uncore, SOUTH_CHICKEN2) |
>>>>>>> upstream/android-13
		   DPLS_EDP_PPS_FIX_DIS);
	/* The below fixes the weird display corruption, a few pixels shifted
	 * downward, on (only) LVDS of some HP laptops with IVY.
	 */
	for_each_pipe(dev_priv, pipe) {
<<<<<<< HEAD
		val = I915_READ(TRANS_CHICKEN2(pipe));
=======
		val = intel_uncore_read(&dev_priv->uncore, TRANS_CHICKEN2(pipe));
>>>>>>> upstream/android-13
		val |= TRANS_CHICKEN2_TIMING_OVERRIDE;
		val &= ~TRANS_CHICKEN2_FDI_POLARITY_REVERSED;
		if (dev_priv->vbt.fdi_rx_polarity_inverted)
			val |= TRANS_CHICKEN2_FDI_POLARITY_REVERSED;
<<<<<<< HEAD
		val &= ~TRANS_CHICKEN2_FRAME_START_DELAY_MASK;
		val &= ~TRANS_CHICKEN2_DISABLE_DEEP_COLOR_COUNTER;
		val &= ~TRANS_CHICKEN2_DISABLE_DEEP_COLOR_MODESWITCH;
		I915_WRITE(TRANS_CHICKEN2(pipe), val);
	}
	/* WADP0ClockGatingDisable */
	for_each_pipe(dev_priv, pipe) {
		I915_WRITE(TRANS_CHICKEN1(pipe),
=======
		val &= ~TRANS_CHICKEN2_DISABLE_DEEP_COLOR_COUNTER;
		val &= ~TRANS_CHICKEN2_DISABLE_DEEP_COLOR_MODESWITCH;
		intel_uncore_write(&dev_priv->uncore, TRANS_CHICKEN2(pipe), val);
	}
	/* WADP0ClockGatingDisable */
	for_each_pipe(dev_priv, pipe) {
		intel_uncore_write(&dev_priv->uncore, TRANS_CHICKEN1(pipe),
>>>>>>> upstream/android-13
			   TRANS_CHICKEN1_DP0UNIT_GC_DISABLE);
	}
}

static void gen6_check_mch_setup(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	uint32_t tmp;

	tmp = I915_READ(MCH_SSKPD);
	if ((tmp & MCH_SSKPD_WM0_MASK) != MCH_SSKPD_WM0_VAL)
		DRM_DEBUG_KMS("Wrong MCH_SSKPD value: 0x%08x This can cause underruns.\n",
			      tmp);
=======
	u32 tmp;

	tmp = intel_uncore_read(&dev_priv->uncore, MCH_SSKPD);
	if ((tmp & MCH_SSKPD_WM0_MASK) != MCH_SSKPD_WM0_VAL)
		drm_dbg_kms(&dev_priv->drm,
			    "Wrong MCH_SSKPD value: 0x%08x This can cause underruns.\n",
			    tmp);
>>>>>>> upstream/android-13
}

static void gen6_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	uint32_t dspclk_gate = ILK_VRHUNIT_CLOCK_GATE_DISABLE;

	I915_WRITE(ILK_DSPCLK_GATE_D, dspclk_gate);

	I915_WRITE(ILK_DISPLAY_CHICKEN2,
		   I915_READ(ILK_DISPLAY_CHICKEN2) |
		   ILK_ELPIN_409_SELECT);

	/* WaDisableHiZPlanesWhenMSAAEnabled:snb */
	I915_WRITE(_3D_CHICKEN,
		   _MASKED_BIT_ENABLE(_3D_CHICKEN_HIZ_PLANE_DISABLE_MSAA_4X_SNB));

	/* WaDisable_RenderCache_OperationalFlush:snb */
	I915_WRITE(CACHE_MODE_0, _MASKED_BIT_DISABLE(RC_OP_FLUSH_ENABLE));

	/*
	 * BSpec recoomends 8x4 when MSAA is used,
	 * however in practice 16x4 seems fastest.
	 *
	 * Note that PS/WM thread counts depend on the WIZ hashing
	 * disable bit, which we don't touch here, but it's good
	 * to keep in mind (see 3DSTATE_PS and 3DSTATE_WM).
	 */
	I915_WRITE(GEN6_GT_MODE,
		   _MASKED_FIELD(GEN6_WIZ_HASHING_MASK, GEN6_WIZ_HASHING_16x4));

	I915_WRITE(CACHE_MODE_0,
		   _MASKED_BIT_DISABLE(CM0_STC_EVICT_DISABLE_LRA_SNB));

	I915_WRITE(GEN6_UCGCTL1,
		   I915_READ(GEN6_UCGCTL1) |
=======
	u32 dspclk_gate = ILK_VRHUNIT_CLOCK_GATE_DISABLE;

	intel_uncore_write(&dev_priv->uncore, ILK_DSPCLK_GATE_D, dspclk_gate);

	intel_uncore_write(&dev_priv->uncore, ILK_DISPLAY_CHICKEN2,
		   intel_uncore_read(&dev_priv->uncore, ILK_DISPLAY_CHICKEN2) |
		   ILK_ELPIN_409_SELECT);

	intel_uncore_write(&dev_priv->uncore, GEN6_UCGCTL1,
		   intel_uncore_read(&dev_priv->uncore, GEN6_UCGCTL1) |
>>>>>>> upstream/android-13
		   GEN6_BLBUNIT_CLOCK_GATE_DISABLE |
		   GEN6_CSUNIT_CLOCK_GATE_DISABLE);

	/* According to the BSpec vol1g, bit 12 (RCPBUNIT) clock
	 * gating disable must be set.  Failure to set it results in
	 * flickering pixels due to Z write ordering failures after
	 * some amount of runtime in the Mesa "fire" demo, and Unigine
	 * Sanctuary and Tropics, and apparently anything else with
	 * alpha test or pixel discard.
	 *
	 * According to the spec, bit 11 (RCCUNIT) must also be set,
	 * but we didn't debug actual testcases to find it out.
	 *
	 * WaDisableRCCUnitClockGating:snb
	 * WaDisableRCPBUnitClockGating:snb
	 */
<<<<<<< HEAD
	I915_WRITE(GEN6_UCGCTL2,
		   GEN6_RCPBUNIT_CLOCK_GATE_DISABLE |
		   GEN6_RCCUNIT_CLOCK_GATE_DISABLE);

	/* WaStripsFansDisableFastClipPerformanceFix:snb */
	I915_WRITE(_3D_CHICKEN3,
		   _MASKED_BIT_ENABLE(_3D_CHICKEN3_SF_DISABLE_FASTCLIP_CULL));

	/*
	 * Bspec says:
	 * "This bit must be set if 3DSTATE_CLIP clip mode is set to normal and
	 * 3DSTATE_SF number of SF output attributes is more than 16."
	 */
	I915_WRITE(_3D_CHICKEN3,
		   _MASKED_BIT_ENABLE(_3D_CHICKEN3_SF_DISABLE_PIPELINED_ATTR_FETCH));

=======
	intel_uncore_write(&dev_priv->uncore, GEN6_UCGCTL2,
		   GEN6_RCPBUNIT_CLOCK_GATE_DISABLE |
		   GEN6_RCCUNIT_CLOCK_GATE_DISABLE);

>>>>>>> upstream/android-13
	/*
	 * According to the spec the following bits should be
	 * set in order to enable memory self-refresh and fbc:
	 * The bit21 and bit22 of 0x42000
	 * The bit21 and bit22 of 0x42004
	 * The bit5 and bit7 of 0x42020
	 * The bit14 of 0x70180
	 * The bit14 of 0x71180
	 *
	 * WaFbcAsynchFlipDisableFbcQueue:snb
	 */
<<<<<<< HEAD
	I915_WRITE(ILK_DISPLAY_CHICKEN1,
		   I915_READ(ILK_DISPLAY_CHICKEN1) |
		   ILK_FBCQ_DIS | ILK_PABSTRETCH_DIS);
	I915_WRITE(ILK_DISPLAY_CHICKEN2,
		   I915_READ(ILK_DISPLAY_CHICKEN2) |
		   ILK_DPARB_GATE | ILK_VSDPFD_FULL);
	I915_WRITE(ILK_DSPCLK_GATE_D,
		   I915_READ(ILK_DSPCLK_GATE_D) |
=======
	intel_uncore_write(&dev_priv->uncore, ILK_DISPLAY_CHICKEN1,
		   intel_uncore_read(&dev_priv->uncore, ILK_DISPLAY_CHICKEN1) |
		   ILK_FBCQ_DIS | ILK_PABSTRETCH_DIS);
	intel_uncore_write(&dev_priv->uncore, ILK_DISPLAY_CHICKEN2,
		   intel_uncore_read(&dev_priv->uncore, ILK_DISPLAY_CHICKEN2) |
		   ILK_DPARB_GATE | ILK_VSDPFD_FULL);
	intel_uncore_write(&dev_priv->uncore, ILK_DSPCLK_GATE_D,
		   intel_uncore_read(&dev_priv->uncore, ILK_DSPCLK_GATE_D) |
>>>>>>> upstream/android-13
		   ILK_DPARBUNIT_CLOCK_GATE_ENABLE  |
		   ILK_DPFDUNIT_CLOCK_GATE_ENABLE);

	g4x_disable_trickle_feed(dev_priv);

	cpt_init_clock_gating(dev_priv);

	gen6_check_mch_setup(dev_priv);
}

<<<<<<< HEAD
static void gen7_setup_fixed_func_scheduler(struct drm_i915_private *dev_priv)
{
	uint32_t reg = I915_READ(GEN7_FF_THREAD_MODE);

	/*
	 * WaVSThreadDispatchOverride:ivb,vlv
	 *
	 * This actually overrides the dispatch
	 * mode for all thread types.
	 */
	reg &= ~GEN7_FF_SCHED_MASK;
	reg |= GEN7_FF_TS_SCHED_HW;
	reg |= GEN7_FF_VS_SCHED_HW;
	reg |= GEN7_FF_DS_SCHED_HW;

	I915_WRITE(GEN7_FF_THREAD_MODE, reg);
}

=======
>>>>>>> upstream/android-13
static void lpt_init_clock_gating(struct drm_i915_private *dev_priv)
{
	/*
	 * TODO: this bit should only be enabled when really needed, then
	 * disabled when not needed anymore in order to save power.
	 */
	if (HAS_PCH_LPT_LP(dev_priv))
<<<<<<< HEAD
		I915_WRITE(SOUTH_DSPCLK_GATE_D,
			   I915_READ(SOUTH_DSPCLK_GATE_D) |
			   PCH_LP_PARTITION_LEVEL_DISABLE);

	/* WADPOClockGatingDisable:hsw */
	I915_WRITE(TRANS_CHICKEN1(PIPE_A),
		   I915_READ(TRANS_CHICKEN1(PIPE_A)) |
=======
		intel_uncore_write(&dev_priv->uncore, SOUTH_DSPCLK_GATE_D,
			   intel_uncore_read(&dev_priv->uncore, SOUTH_DSPCLK_GATE_D) |
			   PCH_LP_PARTITION_LEVEL_DISABLE);

	/* WADPOClockGatingDisable:hsw */
	intel_uncore_write(&dev_priv->uncore, TRANS_CHICKEN1(PIPE_A),
		   intel_uncore_read(&dev_priv->uncore, TRANS_CHICKEN1(PIPE_A)) |
>>>>>>> upstream/android-13
		   TRANS_CHICKEN1_DP0UNIT_GC_DISABLE);
}

static void lpt_suspend_hw(struct drm_i915_private *dev_priv)
{
	if (HAS_PCH_LPT_LP(dev_priv)) {
<<<<<<< HEAD
		uint32_t val = I915_READ(SOUTH_DSPCLK_GATE_D);

		val &= ~PCH_LP_PARTITION_LEVEL_DISABLE;
		I915_WRITE(SOUTH_DSPCLK_GATE_D, val);
=======
		u32 val = intel_uncore_read(&dev_priv->uncore, SOUTH_DSPCLK_GATE_D);

		val &= ~PCH_LP_PARTITION_LEVEL_DISABLE;
		intel_uncore_write(&dev_priv->uncore, SOUTH_DSPCLK_GATE_D, val);
>>>>>>> upstream/android-13
	}
}

static void gen8_set_l3sqc_credits(struct drm_i915_private *dev_priv,
				   int general_prio_credits,
				   int high_prio_credits)
{
	u32 misccpctl;
	u32 val;

	/* WaTempDisableDOPClkGating:bdw */
<<<<<<< HEAD
	misccpctl = I915_READ(GEN7_MISCCPCTL);
	I915_WRITE(GEN7_MISCCPCTL, misccpctl & ~GEN7_DOP_CLOCK_GATE_ENABLE);

	val = I915_READ(GEN8_L3SQCREG1);
	val &= ~L3_PRIO_CREDITS_MASK;
	val |= L3_GENERAL_PRIO_CREDITS(general_prio_credits);
	val |= L3_HIGH_PRIO_CREDITS(high_prio_credits);
	I915_WRITE(GEN8_L3SQCREG1, val);
=======
	misccpctl = intel_uncore_read(&dev_priv->uncore, GEN7_MISCCPCTL);
	intel_uncore_write(&dev_priv->uncore, GEN7_MISCCPCTL, misccpctl & ~GEN7_DOP_CLOCK_GATE_ENABLE);

	val = intel_uncore_read(&dev_priv->uncore, GEN8_L3SQCREG1);
	val &= ~L3_PRIO_CREDITS_MASK;
	val |= L3_GENERAL_PRIO_CREDITS(general_prio_credits);
	val |= L3_HIGH_PRIO_CREDITS(high_prio_credits);
	intel_uncore_write(&dev_priv->uncore, GEN8_L3SQCREG1, val);
>>>>>>> upstream/android-13

	/*
	 * Wait at least 100 clocks before re-enabling clock gating.
	 * See the definition of L3SQCREG1 in BSpec.
	 */
<<<<<<< HEAD
	POSTING_READ(GEN8_L3SQCREG1);
	udelay(1);
	I915_WRITE(GEN7_MISCCPCTL, misccpctl);
=======
	intel_uncore_posting_read(&dev_priv->uncore, GEN8_L3SQCREG1);
	udelay(1);
	intel_uncore_write(&dev_priv->uncore, GEN7_MISCCPCTL, misccpctl);
>>>>>>> upstream/android-13
}

static void icl_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	/* This is not an Wa. Enable to reduce Sampler power */
	I915_WRITE(GEN10_DFR_RATIO_EN_AND_CHICKEN,
		   I915_READ(GEN10_DFR_RATIO_EN_AND_CHICKEN) & ~DFR_DISABLE);
=======
	/* Wa_1409120013:icl,ehl */
	intel_uncore_write(&dev_priv->uncore, ILK_DPFC_CHICKEN,
		   ILK_DPFC_CHICKEN_COMP_DUMMY_PIXEL);

	/*Wa_14010594013:icl, ehl */
	intel_uncore_rmw(&dev_priv->uncore, GEN8_CHICKEN_DCPR_1,
			 0, ICL_DELAY_PMRSP);
}

static void gen12lp_init_clock_gating(struct drm_i915_private *dev_priv)
{
	/* Wa_1409120013:tgl,rkl,adl-s,dg1 */
	if (IS_TIGERLAKE(dev_priv) || IS_ROCKETLAKE(dev_priv) ||
	    IS_ALDERLAKE_S(dev_priv) || IS_DG1(dev_priv))
		intel_uncore_write(&dev_priv->uncore, ILK_DPFC_CHICKEN,
				   ILK_DPFC_CHICKEN_COMP_DUMMY_PIXEL);

	/* Wa_1409825376:tgl (pre-prod)*/
	if (IS_TGL_DISPLAY_STEP(dev_priv, STEP_A0, STEP_C0))
		intel_uncore_write(&dev_priv->uncore, GEN9_CLKGATE_DIS_3, intel_uncore_read(&dev_priv->uncore, GEN9_CLKGATE_DIS_3) |
			   TGL_VRH_GATING_DIS);

	/* Wa_14013723622:tgl,rkl,dg1,adl-s */
	if (DISPLAY_VER(dev_priv) == 12)
		intel_uncore_rmw(&dev_priv->uncore, CLKREQ_POLICY,
				 CLKREQ_POLICY_MEM_UP_OVRD, 0);
}

static void adlp_init_clock_gating(struct drm_i915_private *dev_priv)
{
	gen12lp_init_clock_gating(dev_priv);

	/* Wa_22011091694:adlp */
	intel_de_rmw(dev_priv, GEN9_CLKGATE_DIS_5, 0, DPCE_GATING_DIS);
}

static void dg1_init_clock_gating(struct drm_i915_private *dev_priv)
{
	gen12lp_init_clock_gating(dev_priv);

	/* Wa_1409836686:dg1[a0] */
	if (IS_DG1_GT_STEP(dev_priv, STEP_A0, STEP_B0))
		intel_uncore_write(&dev_priv->uncore, GEN9_CLKGATE_DIS_3, intel_uncore_read(&dev_priv->uncore, GEN9_CLKGATE_DIS_3) |
			   DPT_GATING_DIS);
>>>>>>> upstream/android-13
}

static void cnp_init_clock_gating(struct drm_i915_private *dev_priv)
{
	if (!HAS_PCH_CNP(dev_priv))
		return;

	/* Display WA #1181 WaSouthDisplayDisablePWMCGEGating: cnp */
<<<<<<< HEAD
	I915_WRITE(SOUTH_DSPCLK_GATE_D, I915_READ(SOUTH_DSPCLK_GATE_D) |
		   CNP_PWM_CGE_GATING_DISABLE);
}

static void cnl_init_clock_gating(struct drm_i915_private *dev_priv)
{
	u32 val;
	cnp_init_clock_gating(dev_priv);

	/* This is not an Wa. Enable for better image quality */
	I915_WRITE(_3D_CHICKEN3,
		   _MASKED_BIT_ENABLE(_3D_CHICKEN3_AA_LINE_QUALITY_FIX_ENABLE));

	/* WaEnableChickenDCPR:cnl */
	I915_WRITE(GEN8_CHICKEN_DCPR_1,
		   I915_READ(GEN8_CHICKEN_DCPR_1) | MASK_WAKEMEM);

	/* WaFbcWakeMemOn:cnl */
	I915_WRITE(DISP_ARB_CTL, I915_READ(DISP_ARB_CTL) |
		   DISP_FBC_MEMORY_WAKE);

	val = I915_READ(SLICE_UNIT_LEVEL_CLKGATE);
	/* ReadHitWriteOnlyDisable:cnl */
	val |= RCCUNIT_CLKGATE_DIS;
	/* WaSarbUnitClockGatingDisable:cnl (pre-prod) */
	if (IS_CNL_REVID(dev_priv, CNL_REVID_A0, CNL_REVID_B0))
		val |= SARBUNIT_CLKGATE_DIS;
	I915_WRITE(SLICE_UNIT_LEVEL_CLKGATE, val);

	/* Wa_2201832410:cnl */
	val = I915_READ(SUBSLICE_UNIT_LEVEL_CLKGATE);
	val |= GWUNIT_CLKGATE_DIS;
	I915_WRITE(SUBSLICE_UNIT_LEVEL_CLKGATE, val);

	/* WaDisableVFclkgate:cnl */
	/* WaVFUnitClockGatingDisable:cnl */
	val = I915_READ(UNSLICE_UNIT_LEVEL_CLKGATE);
	val |= VFUNIT_CLKGATE_DIS;
	I915_WRITE(UNSLICE_UNIT_LEVEL_CLKGATE, val);
}

=======
	intel_uncore_write(&dev_priv->uncore, SOUTH_DSPCLK_GATE_D, intel_uncore_read(&dev_priv->uncore, SOUTH_DSPCLK_GATE_D) |
		   CNP_PWM_CGE_GATING_DISABLE);
}

>>>>>>> upstream/android-13
static void cfl_init_clock_gating(struct drm_i915_private *dev_priv)
{
	cnp_init_clock_gating(dev_priv);
	gen9_init_clock_gating(dev_priv);

<<<<<<< HEAD
	/* WaFbcNukeOnHostModify:cfl */
	I915_WRITE(ILK_DPFC_CHICKEN, I915_READ(ILK_DPFC_CHICKEN) |
=======
	/* WAC6entrylatency:cfl */
	intel_uncore_write(&dev_priv->uncore, FBC_LLC_READ_CTRL, intel_uncore_read(&dev_priv->uncore, FBC_LLC_READ_CTRL) |
		   FBC_LLC_FULLY_OPEN);

	/*
	 * WaFbcTurnOffFbcWatermark:cfl
	 * Display WA #0562: cfl
	 */
	intel_uncore_write(&dev_priv->uncore, DISP_ARB_CTL, intel_uncore_read(&dev_priv->uncore, DISP_ARB_CTL) |
		   DISP_FBC_WM_DIS);

	/*
	 * WaFbcNukeOnHostModify:cfl
	 * Display WA #0873: cfl
	 */
	intel_uncore_write(&dev_priv->uncore, ILK_DPFC_CHICKEN, intel_uncore_read(&dev_priv->uncore, ILK_DPFC_CHICKEN) |
>>>>>>> upstream/android-13
		   ILK_DPFC_NUKE_ON_ANY_MODIFICATION);
}

static void kbl_init_clock_gating(struct drm_i915_private *dev_priv)
{
	gen9_init_clock_gating(dev_priv);

<<<<<<< HEAD
	/* WaDisableSDEUnitClockGating:kbl */
	if (IS_KBL_REVID(dev_priv, 0, KBL_REVID_B0))
		I915_WRITE(GEN8_UCGCTL6, I915_READ(GEN8_UCGCTL6) |
			   GEN8_SDEUNIT_CLOCK_GATE_DISABLE);

	/* WaDisableGamClockGating:kbl */
	if (IS_KBL_REVID(dev_priv, 0, KBL_REVID_B0))
		I915_WRITE(GEN6_UCGCTL1, I915_READ(GEN6_UCGCTL1) |
			   GEN6_GAMUNIT_CLOCK_GATE_DISABLE);

	/* WaFbcNukeOnHostModify:kbl */
	I915_WRITE(ILK_DPFC_CHICKEN, I915_READ(ILK_DPFC_CHICKEN) |
=======
	/* WAC6entrylatency:kbl */
	intel_uncore_write(&dev_priv->uncore, FBC_LLC_READ_CTRL, intel_uncore_read(&dev_priv->uncore, FBC_LLC_READ_CTRL) |
		   FBC_LLC_FULLY_OPEN);

	/* WaDisableSDEUnitClockGating:kbl */
	if (IS_KBL_GT_STEP(dev_priv, 0, STEP_C0))
		intel_uncore_write(&dev_priv->uncore, GEN8_UCGCTL6, intel_uncore_read(&dev_priv->uncore, GEN8_UCGCTL6) |
			   GEN8_SDEUNIT_CLOCK_GATE_DISABLE);

	/* WaDisableGamClockGating:kbl */
	if (IS_KBL_GT_STEP(dev_priv, 0, STEP_C0))
		intel_uncore_write(&dev_priv->uncore, GEN6_UCGCTL1, intel_uncore_read(&dev_priv->uncore, GEN6_UCGCTL1) |
			   GEN6_GAMUNIT_CLOCK_GATE_DISABLE);

	/*
	 * WaFbcTurnOffFbcWatermark:kbl
	 * Display WA #0562: kbl
	 */
	intel_uncore_write(&dev_priv->uncore, DISP_ARB_CTL, intel_uncore_read(&dev_priv->uncore, DISP_ARB_CTL) |
		   DISP_FBC_WM_DIS);

	/*
	 * WaFbcNukeOnHostModify:kbl
	 * Display WA #0873: kbl
	 */
	intel_uncore_write(&dev_priv->uncore, ILK_DPFC_CHICKEN, intel_uncore_read(&dev_priv->uncore, ILK_DPFC_CHICKEN) |
>>>>>>> upstream/android-13
		   ILK_DPFC_NUKE_ON_ANY_MODIFICATION);
}

static void skl_init_clock_gating(struct drm_i915_private *dev_priv)
{
	gen9_init_clock_gating(dev_priv);

<<<<<<< HEAD
	/* WAC6entrylatency:skl */
	I915_WRITE(FBC_LLC_READ_CTRL, I915_READ(FBC_LLC_READ_CTRL) |
		   FBC_LLC_FULLY_OPEN);

	/* WaFbcNukeOnHostModify:skl */
	I915_WRITE(ILK_DPFC_CHICKEN, I915_READ(ILK_DPFC_CHICKEN) |
		   ILK_DPFC_NUKE_ON_ANY_MODIFICATION);
=======
	/* WaDisableDopClockGating:skl */
	intel_uncore_write(&dev_priv->uncore, GEN7_MISCCPCTL, intel_uncore_read(&dev_priv->uncore, GEN7_MISCCPCTL) &
		   ~GEN7_DOP_CLOCK_GATE_ENABLE);

	/* WAC6entrylatency:skl */
	intel_uncore_write(&dev_priv->uncore, FBC_LLC_READ_CTRL, intel_uncore_read(&dev_priv->uncore, FBC_LLC_READ_CTRL) |
		   FBC_LLC_FULLY_OPEN);

	/*
	 * WaFbcTurnOffFbcWatermark:skl
	 * Display WA #0562: skl
	 */
	intel_uncore_write(&dev_priv->uncore, DISP_ARB_CTL, intel_uncore_read(&dev_priv->uncore, DISP_ARB_CTL) |
		   DISP_FBC_WM_DIS);

	/*
	 * WaFbcNukeOnHostModify:skl
	 * Display WA #0873: skl
	 */
	intel_uncore_write(&dev_priv->uncore, ILK_DPFC_CHICKEN, intel_uncore_read(&dev_priv->uncore, ILK_DPFC_CHICKEN) |
		   ILK_DPFC_NUKE_ON_ANY_MODIFICATION);

	/*
	 * WaFbcHighMemBwCorruptionAvoidance:skl
	 * Display WA #0883: skl
	 */
	intel_uncore_write(&dev_priv->uncore, ILK_DPFC_CHICKEN, intel_uncore_read(&dev_priv->uncore, ILK_DPFC_CHICKEN) |
		   ILK_DPFC_DISABLE_DUMMY0);
>>>>>>> upstream/android-13
}

static void bdw_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	/* The GTT cache must be disabled if the system is using 2M pages. */
	bool can_use_gtt_cache = !HAS_PAGE_SIZES(dev_priv,
						 I915_GTT_PAGE_SIZE_2M);
	enum pipe pipe;

	/* WaSwitchSolVfFArbitrationPriority:bdw */
	I915_WRITE(GAM_ECOCHK, I915_READ(GAM_ECOCHK) | HSW_ECOCHK_ARB_PRIO_SOL);

	/* WaPsrDPAMaskVBlankInSRD:bdw */
	I915_WRITE(CHICKEN_PAR1_1,
		   I915_READ(CHICKEN_PAR1_1) | DPA_MASK_VBLANK_SRD);

	/* WaPsrDPRSUnmaskVBlankInSRD:bdw */
	for_each_pipe(dev_priv, pipe) {
		I915_WRITE(CHICKEN_PIPESL_1(pipe),
			   I915_READ(CHICKEN_PIPESL_1(pipe)) |
			   BDW_DPRS_MASK_VBLANK_SRD);
=======
	enum pipe pipe;

	/* WaFbcAsynchFlipDisableFbcQueue:hsw,bdw */
	intel_uncore_write(&dev_priv->uncore, CHICKEN_PIPESL_1(PIPE_A),
		   intel_uncore_read(&dev_priv->uncore, CHICKEN_PIPESL_1(PIPE_A)) |
		   HSW_FBCQ_DIS);

	/* WaSwitchSolVfFArbitrationPriority:bdw */
	intel_uncore_write(&dev_priv->uncore, GAM_ECOCHK, intel_uncore_read(&dev_priv->uncore, GAM_ECOCHK) | HSW_ECOCHK_ARB_PRIO_SOL);

	/* WaPsrDPAMaskVBlankInSRD:bdw */
	intel_uncore_write(&dev_priv->uncore, CHICKEN_PAR1_1,
		   intel_uncore_read(&dev_priv->uncore, CHICKEN_PAR1_1) | DPA_MASK_VBLANK_SRD);

	for_each_pipe(dev_priv, pipe) {
		/* WaPsrDPRSUnmaskVBlankInSRD:bdw */
		intel_uncore_write(&dev_priv->uncore, CHICKEN_PIPESL_1(pipe),
			   intel_uncore_read(&dev_priv->uncore, CHICKEN_PIPESL_1(pipe)) |
			   BDW_DPRS_MASK_VBLANK_SRD);

		/* Undocumented but fixes async flip + VT-d corruption */
		if (intel_vtd_active())
			intel_uncore_rmw(&dev_priv->uncore, CHICKEN_PIPESL_1(pipe),
					 HSW_PRI_STRETCH_MAX_MASK, HSW_PRI_STRETCH_MAX_X1);
>>>>>>> upstream/android-13
	}

	/* WaVSRefCountFullforceMissDisable:bdw */
	/* WaDSRefCountFullforceMissDisable:bdw */
<<<<<<< HEAD
	I915_WRITE(GEN7_FF_THREAD_MODE,
		   I915_READ(GEN7_FF_THREAD_MODE) &
		   ~(GEN8_FF_DS_REF_CNT_FFME | GEN7_FF_VS_REF_CNT_FFME));

	I915_WRITE(GEN6_RC_SLEEP_PSMI_CONTROL,
		   _MASKED_BIT_ENABLE(GEN8_RC_SEMA_IDLE_MSG_DISABLE));

	/* WaDisableSDEUnitClockGating:bdw */
	I915_WRITE(GEN8_UCGCTL6, I915_READ(GEN8_UCGCTL6) |
=======
	intel_uncore_write(&dev_priv->uncore, GEN7_FF_THREAD_MODE,
		   intel_uncore_read(&dev_priv->uncore, GEN7_FF_THREAD_MODE) &
		   ~(GEN8_FF_DS_REF_CNT_FFME | GEN7_FF_VS_REF_CNT_FFME));

	intel_uncore_write(&dev_priv->uncore, GEN6_RC_SLEEP_PSMI_CONTROL,
		   _MASKED_BIT_ENABLE(GEN8_RC_SEMA_IDLE_MSG_DISABLE));

	/* WaDisableSDEUnitClockGating:bdw */
	intel_uncore_write(&dev_priv->uncore, GEN8_UCGCTL6, intel_uncore_read(&dev_priv->uncore, GEN8_UCGCTL6) |
>>>>>>> upstream/android-13
		   GEN8_SDEUNIT_CLOCK_GATE_DISABLE);

	/* WaProgramL3SqcReg1Default:bdw */
	gen8_set_l3sqc_credits(dev_priv, 30, 2);

<<<<<<< HEAD
	/* WaGttCachingOffByDefault:bdw */
	I915_WRITE(HSW_GTT_CACHE_EN, can_use_gtt_cache ? GTT_CACHE_EN_ALL : 0);

	/* WaKVMNotificationOnConfigChange:bdw */
	I915_WRITE(CHICKEN_PAR2_1, I915_READ(CHICKEN_PAR2_1)
=======
	/* WaKVMNotificationOnConfigChange:bdw */
	intel_uncore_write(&dev_priv->uncore, CHICKEN_PAR2_1, intel_uncore_read(&dev_priv->uncore, CHICKEN_PAR2_1)
>>>>>>> upstream/android-13
		   | KVM_CONFIG_CHANGE_NOTIFICATION_SELECT);

	lpt_init_clock_gating(dev_priv);

	/* WaDisableDopClockGating:bdw
	 *
	 * Also see the CHICKEN2 write in bdw_init_workarounds() to disable DOP
	 * clock gating.
	 */
<<<<<<< HEAD
	I915_WRITE(GEN6_UCGCTL1,
		   I915_READ(GEN6_UCGCTL1) | GEN6_EU_TCUNIT_CLOCK_GATE_DISABLE);
=======
	intel_uncore_write(&dev_priv->uncore, GEN6_UCGCTL1,
		   intel_uncore_read(&dev_priv->uncore, GEN6_UCGCTL1) | GEN6_EU_TCUNIT_CLOCK_GATE_DISABLE);
>>>>>>> upstream/android-13
}

static void hsw_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	/* L3 caching of data atomics doesn't work -- disable it. */
	I915_WRITE(HSW_SCRATCH1, HSW_SCRATCH1_L3_DATA_ATOMICS_DISABLE);
	I915_WRITE(HSW_ROW_CHICKEN3,
		   _MASKED_BIT_ENABLE(HSW_ROW_CHICKEN3_L3_GLOBAL_ATOMICS_DISABLE));

	/* This is required by WaCatErrorRejectionIssue:hsw */
	I915_WRITE(GEN7_SQ_CHICKEN_MBCUNIT_CONFIG,
			I915_READ(GEN7_SQ_CHICKEN_MBCUNIT_CONFIG) |
			GEN7_SQ_CHICKEN_MBCUNIT_SQINTMOB);

	/* WaVSRefCountFullforceMissDisable:hsw */
	I915_WRITE(GEN7_FF_THREAD_MODE,
		   I915_READ(GEN7_FF_THREAD_MODE) & ~GEN7_FF_VS_REF_CNT_FFME);

	/* WaDisable_RenderCache_OperationalFlush:hsw */
	I915_WRITE(CACHE_MODE_0_GEN7, _MASKED_BIT_DISABLE(RC_OP_FLUSH_ENABLE));

	/* enable HiZ Raw Stall Optimization */
	I915_WRITE(CACHE_MODE_0_GEN7,
		   _MASKED_BIT_DISABLE(HIZ_RAW_STALL_OPT_DISABLE));

	/* WaDisable4x2SubspanOptimization:hsw */
	I915_WRITE(CACHE_MODE_1,
		   _MASKED_BIT_ENABLE(PIXEL_SUBSPAN_COLLECT_OPT_DISABLE));

	/*
	 * BSpec recommends 8x4 when MSAA is used,
	 * however in practice 16x4 seems fastest.
	 *
	 * Note that PS/WM thread counts depend on the WIZ hashing
	 * disable bit, which we don't touch here, but it's good
	 * to keep in mind (see 3DSTATE_PS and 3DSTATE_WM).
	 */
	I915_WRITE(GEN7_GT_MODE,
		   _MASKED_FIELD(GEN6_WIZ_HASHING_MASK, GEN6_WIZ_HASHING_16x4));

	/* WaSampleCChickenBitEnable:hsw */
	I915_WRITE(HALF_SLICE_CHICKEN3,
		   _MASKED_BIT_ENABLE(HSW_SAMPLE_C_PERFORMANCE));

	/* WaSwitchSolVfFArbitrationPriority:hsw */
	I915_WRITE(GAM_ECOCHK, I915_READ(GAM_ECOCHK) | HSW_ECOCHK_ARB_PRIO_SOL);
=======
	enum pipe pipe;

	/* WaFbcAsynchFlipDisableFbcQueue:hsw,bdw */
	intel_uncore_write(&dev_priv->uncore, CHICKEN_PIPESL_1(PIPE_A),
		   intel_uncore_read(&dev_priv->uncore, CHICKEN_PIPESL_1(PIPE_A)) |
		   HSW_FBCQ_DIS);

	for_each_pipe(dev_priv, pipe) {
		/* Undocumented but fixes async flip + VT-d corruption */
		if (intel_vtd_active())
			intel_uncore_rmw(&dev_priv->uncore, CHICKEN_PIPESL_1(pipe),
					 HSW_PRI_STRETCH_MAX_MASK, HSW_PRI_STRETCH_MAX_X1);
	}

	/* This is required by WaCatErrorRejectionIssue:hsw */
	intel_uncore_write(&dev_priv->uncore, GEN7_SQ_CHICKEN_MBCUNIT_CONFIG,
		   intel_uncore_read(&dev_priv->uncore, GEN7_SQ_CHICKEN_MBCUNIT_CONFIG) |
		   GEN7_SQ_CHICKEN_MBCUNIT_SQINTMOB);

	/* WaSwitchSolVfFArbitrationPriority:hsw */
	intel_uncore_write(&dev_priv->uncore, GAM_ECOCHK, intel_uncore_read(&dev_priv->uncore, GAM_ECOCHK) | HSW_ECOCHK_ARB_PRIO_SOL);
>>>>>>> upstream/android-13

	lpt_init_clock_gating(dev_priv);
}

static void ivb_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	uint32_t snpcr;

	I915_WRITE(ILK_DSPCLK_GATE_D, ILK_VRHUNIT_CLOCK_GATE_DISABLE);

	/* WaDisableEarlyCull:ivb */
	I915_WRITE(_3D_CHICKEN3,
		   _MASKED_BIT_ENABLE(_3D_CHICKEN_SF_DISABLE_OBJEND_CULL));

	/* WaDisableBackToBackFlipFix:ivb */
	I915_WRITE(IVB_CHICKEN3,
		   CHICKEN3_DGMG_REQ_OUT_FIX_DISABLE |
		   CHICKEN3_DGMG_DONE_FIX_DISABLE);

	/* WaDisablePSDDualDispatchEnable:ivb */
	if (IS_IVB_GT1(dev_priv))
		I915_WRITE(GEN7_HALF_SLICE_CHICKEN1,
			   _MASKED_BIT_ENABLE(GEN7_PSD_SINGLE_PORT_DISPATCH_ENABLE));

	/* WaDisable_RenderCache_OperationalFlush:ivb */
	I915_WRITE(CACHE_MODE_0_GEN7, _MASKED_BIT_DISABLE(RC_OP_FLUSH_ENABLE));

	/* Apply the WaDisableRHWOOptimizationForRenderHang:ivb workaround. */
	I915_WRITE(GEN7_COMMON_SLICE_CHICKEN1,
		   GEN7_CSC1_RHWO_OPT_DISABLE_IN_RCC);

	/* WaApplyL3ControlAndL3ChickenMode:ivb */
	I915_WRITE(GEN7_L3CNTLREG1,
			GEN7_WA_FOR_GEN7_L3_CONTROL);
	I915_WRITE(GEN7_L3_CHICKEN_MODE_REGISTER,
		   GEN7_WA_L3_CHICKEN_MODE);
	if (IS_IVB_GT1(dev_priv))
		I915_WRITE(GEN7_ROW_CHICKEN2,
			   _MASKED_BIT_ENABLE(DOP_CLOCK_GATING_DISABLE));
	else {
		/* must write both registers */
		I915_WRITE(GEN7_ROW_CHICKEN2,
			   _MASKED_BIT_ENABLE(DOP_CLOCK_GATING_DISABLE));
		I915_WRITE(GEN7_ROW_CHICKEN2_GT2,
			   _MASKED_BIT_ENABLE(DOP_CLOCK_GATING_DISABLE));
	}

	/* WaForceL3Serialization:ivb */
	I915_WRITE(GEN7_L3SQCREG4, I915_READ(GEN7_L3SQCREG4) &
		   ~L3SQ_URB_READ_CAM_MATCH_DISABLE);

=======
	u32 snpcr;

	intel_uncore_write(&dev_priv->uncore, ILK_DSPCLK_GATE_D, ILK_VRHUNIT_CLOCK_GATE_DISABLE);

	/* WaFbcAsynchFlipDisableFbcQueue:ivb */
	intel_uncore_write(&dev_priv->uncore, ILK_DISPLAY_CHICKEN1,
		   intel_uncore_read(&dev_priv->uncore, ILK_DISPLAY_CHICKEN1) |
		   ILK_FBCQ_DIS);

	/* WaDisableBackToBackFlipFix:ivb */
	intel_uncore_write(&dev_priv->uncore, IVB_CHICKEN3,
		   CHICKEN3_DGMG_REQ_OUT_FIX_DISABLE |
		   CHICKEN3_DGMG_DONE_FIX_DISABLE);

	if (IS_IVB_GT1(dev_priv))
		intel_uncore_write(&dev_priv->uncore, GEN7_ROW_CHICKEN2,
			   _MASKED_BIT_ENABLE(DOP_CLOCK_GATING_DISABLE));
	else {
		/* must write both registers */
		intel_uncore_write(&dev_priv->uncore, GEN7_ROW_CHICKEN2,
			   _MASKED_BIT_ENABLE(DOP_CLOCK_GATING_DISABLE));
		intel_uncore_write(&dev_priv->uncore, GEN7_ROW_CHICKEN2_GT2,
			   _MASKED_BIT_ENABLE(DOP_CLOCK_GATING_DISABLE));
	}

>>>>>>> upstream/android-13
	/*
	 * According to the spec, bit 13 (RCZUNIT) must be set on IVB.
	 * This implements the WaDisableRCZUnitClockGating:ivb workaround.
	 */
<<<<<<< HEAD
	I915_WRITE(GEN6_UCGCTL2,
		   GEN6_RCZUNIT_CLOCK_GATE_DISABLE);

	/* This is required by WaCatErrorRejectionIssue:ivb */
	I915_WRITE(GEN7_SQ_CHICKEN_MBCUNIT_CONFIG,
			I915_READ(GEN7_SQ_CHICKEN_MBCUNIT_CONFIG) |
=======
	intel_uncore_write(&dev_priv->uncore, GEN6_UCGCTL2,
		   GEN6_RCZUNIT_CLOCK_GATE_DISABLE);

	/* This is required by WaCatErrorRejectionIssue:ivb */
	intel_uncore_write(&dev_priv->uncore, GEN7_SQ_CHICKEN_MBCUNIT_CONFIG,
			intel_uncore_read(&dev_priv->uncore, GEN7_SQ_CHICKEN_MBCUNIT_CONFIG) |
>>>>>>> upstream/android-13
			GEN7_SQ_CHICKEN_MBCUNIT_SQINTMOB);

	g4x_disable_trickle_feed(dev_priv);

<<<<<<< HEAD
	gen7_setup_fixed_func_scheduler(dev_priv);

	if (0) { /* causes HiZ corruption on ivb:gt1 */
		/* enable HiZ Raw Stall Optimization */
		I915_WRITE(CACHE_MODE_0_GEN7,
			   _MASKED_BIT_DISABLE(HIZ_RAW_STALL_OPT_DISABLE));
	}

	/* WaDisable4x2SubspanOptimization:ivb */
	I915_WRITE(CACHE_MODE_1,
		   _MASKED_BIT_ENABLE(PIXEL_SUBSPAN_COLLECT_OPT_DISABLE));

	/*
	 * BSpec recommends 8x4 when MSAA is used,
	 * however in practice 16x4 seems fastest.
	 *
	 * Note that PS/WM thread counts depend on the WIZ hashing
	 * disable bit, which we don't touch here, but it's good
	 * to keep in mind (see 3DSTATE_PS and 3DSTATE_WM).
	 */
	I915_WRITE(GEN7_GT_MODE,
		   _MASKED_FIELD(GEN6_WIZ_HASHING_MASK, GEN6_WIZ_HASHING_16x4));

	snpcr = I915_READ(GEN6_MBCUNIT_SNPCR);
	snpcr &= ~GEN6_MBC_SNPCR_MASK;
	snpcr |= GEN6_MBC_SNPCR_MED;
	I915_WRITE(GEN6_MBCUNIT_SNPCR, snpcr);
=======
	snpcr = intel_uncore_read(&dev_priv->uncore, GEN6_MBCUNIT_SNPCR);
	snpcr &= ~GEN6_MBC_SNPCR_MASK;
	snpcr |= GEN6_MBC_SNPCR_MED;
	intel_uncore_write(&dev_priv->uncore, GEN6_MBCUNIT_SNPCR, snpcr);
>>>>>>> upstream/android-13

	if (!HAS_PCH_NOP(dev_priv))
		cpt_init_clock_gating(dev_priv);

	gen6_check_mch_setup(dev_priv);
}

static void vlv_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	/* WaDisableEarlyCull:vlv */
	I915_WRITE(_3D_CHICKEN3,
		   _MASKED_BIT_ENABLE(_3D_CHICKEN_SF_DISABLE_OBJEND_CULL));

	/* WaDisableBackToBackFlipFix:vlv */
	I915_WRITE(IVB_CHICKEN3,
		   CHICKEN3_DGMG_REQ_OUT_FIX_DISABLE |
		   CHICKEN3_DGMG_DONE_FIX_DISABLE);

	/* WaPsdDispatchEnable:vlv */
	/* WaDisablePSDDualDispatchEnable:vlv */
	I915_WRITE(GEN7_HALF_SLICE_CHICKEN1,
		   _MASKED_BIT_ENABLE(GEN7_MAX_PS_THREAD_DEP |
				      GEN7_PSD_SINGLE_PORT_DISPATCH_ENABLE));

	/* WaDisable_RenderCache_OperationalFlush:vlv */
	I915_WRITE(CACHE_MODE_0_GEN7, _MASKED_BIT_DISABLE(RC_OP_FLUSH_ENABLE));

	/* WaForceL3Serialization:vlv */
	I915_WRITE(GEN7_L3SQCREG4, I915_READ(GEN7_L3SQCREG4) &
		   ~L3SQ_URB_READ_CAM_MATCH_DISABLE);

	/* WaDisableDopClockGating:vlv */
	I915_WRITE(GEN7_ROW_CHICKEN2,
		   _MASKED_BIT_ENABLE(DOP_CLOCK_GATING_DISABLE));

	/* This is required by WaCatErrorRejectionIssue:vlv */
	I915_WRITE(GEN7_SQ_CHICKEN_MBCUNIT_CONFIG,
		   I915_READ(GEN7_SQ_CHICKEN_MBCUNIT_CONFIG) |
		   GEN7_SQ_CHICKEN_MBCUNIT_SQINTMOB);

	gen7_setup_fixed_func_scheduler(dev_priv);

=======
	/* WaDisableBackToBackFlipFix:vlv */
	intel_uncore_write(&dev_priv->uncore, IVB_CHICKEN3,
		   CHICKEN3_DGMG_REQ_OUT_FIX_DISABLE |
		   CHICKEN3_DGMG_DONE_FIX_DISABLE);

	/* WaDisableDopClockGating:vlv */
	intel_uncore_write(&dev_priv->uncore, GEN7_ROW_CHICKEN2,
		   _MASKED_BIT_ENABLE(DOP_CLOCK_GATING_DISABLE));

	/* This is required by WaCatErrorRejectionIssue:vlv */
	intel_uncore_write(&dev_priv->uncore, GEN7_SQ_CHICKEN_MBCUNIT_CONFIG,
		   intel_uncore_read(&dev_priv->uncore, GEN7_SQ_CHICKEN_MBCUNIT_CONFIG) |
		   GEN7_SQ_CHICKEN_MBCUNIT_SQINTMOB);

>>>>>>> upstream/android-13
	/*
	 * According to the spec, bit 13 (RCZUNIT) must be set on IVB.
	 * This implements the WaDisableRCZUnitClockGating:vlv workaround.
	 */
<<<<<<< HEAD
	I915_WRITE(GEN6_UCGCTL2,
=======
	intel_uncore_write(&dev_priv->uncore, GEN6_UCGCTL2,
>>>>>>> upstream/android-13
		   GEN6_RCZUNIT_CLOCK_GATE_DISABLE);

	/* WaDisableL3Bank2xClockGate:vlv
	 * Disabling L3 clock gating- MMIO 940c[25] = 1
	 * Set bit 25, to disable L3_BANK_2x_CLK_GATING */
<<<<<<< HEAD
	I915_WRITE(GEN7_UCGCTL4,
		   I915_READ(GEN7_UCGCTL4) | GEN7_L3BANK2X_CLOCK_GATE_DISABLE);

	/*
	 * BSpec says this must be set, even though
	 * WaDisable4x2SubspanOptimization isn't listed for VLV.
	 */
	I915_WRITE(CACHE_MODE_1,
		   _MASKED_BIT_ENABLE(PIXEL_SUBSPAN_COLLECT_OPT_DISABLE));

	/*
	 * BSpec recommends 8x4 when MSAA is used,
	 * however in practice 16x4 seems fastest.
	 *
	 * Note that PS/WM thread counts depend on the WIZ hashing
	 * disable bit, which we don't touch here, but it's good
	 * to keep in mind (see 3DSTATE_PS and 3DSTATE_WM).
	 */
	I915_WRITE(GEN7_GT_MODE,
		   _MASKED_FIELD(GEN6_WIZ_HASHING_MASK, GEN6_WIZ_HASHING_16x4));

	/*
	 * WaIncreaseL3CreditsForVLVB0:vlv
	 * This is the hardware default actually.
	 */
	I915_WRITE(GEN7_L3SQCREG1, VLV_B0_WA_L3SQCREG1_VALUE);
=======
	intel_uncore_write(&dev_priv->uncore, GEN7_UCGCTL4,
		   intel_uncore_read(&dev_priv->uncore, GEN7_UCGCTL4) | GEN7_L3BANK2X_CLOCK_GATE_DISABLE);
>>>>>>> upstream/android-13

	/*
	 * WaDisableVLVClockGating_VBIIssue:vlv
	 * Disable clock gating on th GCFG unit to prevent a delay
	 * in the reporting of vblank events.
	 */
<<<<<<< HEAD
	I915_WRITE(VLV_GUNIT_CLOCK_GATE, GCFG_DIS);
=======
	intel_uncore_write(&dev_priv->uncore, VLV_GUNIT_CLOCK_GATE, GCFG_DIS);
>>>>>>> upstream/android-13
}

static void chv_init_clock_gating(struct drm_i915_private *dev_priv)
{
	/* WaVSRefCountFullforceMissDisable:chv */
	/* WaDSRefCountFullforceMissDisable:chv */
<<<<<<< HEAD
	I915_WRITE(GEN7_FF_THREAD_MODE,
		   I915_READ(GEN7_FF_THREAD_MODE) &
		   ~(GEN8_FF_DS_REF_CNT_FFME | GEN7_FF_VS_REF_CNT_FFME));

	/* WaDisableSemaphoreAndSyncFlipWait:chv */
	I915_WRITE(GEN6_RC_SLEEP_PSMI_CONTROL,
		   _MASKED_BIT_ENABLE(GEN8_RC_SEMA_IDLE_MSG_DISABLE));

	/* WaDisableCSUnitClockGating:chv */
	I915_WRITE(GEN6_UCGCTL1, I915_READ(GEN6_UCGCTL1) |
		   GEN6_CSUNIT_CLOCK_GATE_DISABLE);

	/* WaDisableSDEUnitClockGating:chv */
	I915_WRITE(GEN8_UCGCTL6, I915_READ(GEN8_UCGCTL6) |
=======
	intel_uncore_write(&dev_priv->uncore, GEN7_FF_THREAD_MODE,
		   intel_uncore_read(&dev_priv->uncore, GEN7_FF_THREAD_MODE) &
		   ~(GEN8_FF_DS_REF_CNT_FFME | GEN7_FF_VS_REF_CNT_FFME));

	/* WaDisableSemaphoreAndSyncFlipWait:chv */
	intel_uncore_write(&dev_priv->uncore, GEN6_RC_SLEEP_PSMI_CONTROL,
		   _MASKED_BIT_ENABLE(GEN8_RC_SEMA_IDLE_MSG_DISABLE));

	/* WaDisableCSUnitClockGating:chv */
	intel_uncore_write(&dev_priv->uncore, GEN6_UCGCTL1, intel_uncore_read(&dev_priv->uncore, GEN6_UCGCTL1) |
		   GEN6_CSUNIT_CLOCK_GATE_DISABLE);

	/* WaDisableSDEUnitClockGating:chv */
	intel_uncore_write(&dev_priv->uncore, GEN8_UCGCTL6, intel_uncore_read(&dev_priv->uncore, GEN8_UCGCTL6) |
>>>>>>> upstream/android-13
		   GEN8_SDEUNIT_CLOCK_GATE_DISABLE);

	/*
	 * WaProgramL3SqcReg1Default:chv
	 * See gfxspecs/Related Documents/Performance Guide/
	 * LSQC Setting Recommendations.
	 */
	gen8_set_l3sqc_credits(dev_priv, 38, 2);
<<<<<<< HEAD

	/*
	 * GTT cache may not work with big pages, so if those
	 * are ever enabled GTT cache may need to be disabled.
	 */
	I915_WRITE(HSW_GTT_CACHE_EN, GTT_CACHE_EN_ALL);
=======
>>>>>>> upstream/android-13
}

static void g4x_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	uint32_t dspclk_gate;

	I915_WRITE(RENCLK_GATE_D1, 0);
	I915_WRITE(RENCLK_GATE_D2, VF_UNIT_CLOCK_GATE_DISABLE |
		   GS_UNIT_CLOCK_GATE_DISABLE |
		   CL_UNIT_CLOCK_GATE_DISABLE);
	I915_WRITE(RAMCLK_GATE_D, 0);
=======
	u32 dspclk_gate;

	intel_uncore_write(&dev_priv->uncore, RENCLK_GATE_D1, 0);
	intel_uncore_write(&dev_priv->uncore, RENCLK_GATE_D2, VF_UNIT_CLOCK_GATE_DISABLE |
		   GS_UNIT_CLOCK_GATE_DISABLE |
		   CL_UNIT_CLOCK_GATE_DISABLE);
	intel_uncore_write(&dev_priv->uncore, RAMCLK_GATE_D, 0);
>>>>>>> upstream/android-13
	dspclk_gate = VRHUNIT_CLOCK_GATE_DISABLE |
		OVRUNIT_CLOCK_GATE_DISABLE |
		OVCUNIT_CLOCK_GATE_DISABLE;
	if (IS_GM45(dev_priv))
		dspclk_gate |= DSSUNIT_CLOCK_GATE_DISABLE;
<<<<<<< HEAD
	I915_WRITE(DSPCLK_GATE_D, dspclk_gate);

	/* WaDisableRenderCachePipelinedFlush */
	I915_WRITE(CACHE_MODE_0,
		   _MASKED_BIT_ENABLE(CM0_PIPELINED_RENDER_FLUSH_DISABLE));

	/* WaDisable_RenderCache_OperationalFlush:g4x */
	I915_WRITE(CACHE_MODE_0, _MASKED_BIT_DISABLE(RC_OP_FLUSH_ENABLE));
=======
	intel_uncore_write(&dev_priv->uncore, DSPCLK_GATE_D, dspclk_gate);
>>>>>>> upstream/android-13

	g4x_disable_trickle_feed(dev_priv);
}

static void i965gm_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	I915_WRITE(RENCLK_GATE_D1, I965_RCC_CLOCK_GATE_DISABLE);
	I915_WRITE(RENCLK_GATE_D2, 0);
	I915_WRITE(DSPCLK_GATE_D, 0);
	I915_WRITE(RAMCLK_GATE_D, 0);
	I915_WRITE16(DEUC, 0);
	I915_WRITE(MI_ARB_STATE,
		   _MASKED_BIT_ENABLE(MI_ARB_DISPLAY_TRICKLE_FEED_DISABLE));

	/* WaDisable_RenderCache_OperationalFlush:gen4 */
	I915_WRITE(CACHE_MODE_0, _MASKED_BIT_DISABLE(RC_OP_FLUSH_ENABLE));
=======
	struct intel_uncore *uncore = &dev_priv->uncore;

	intel_uncore_write(uncore, RENCLK_GATE_D1, I965_RCC_CLOCK_GATE_DISABLE);
	intel_uncore_write(uncore, RENCLK_GATE_D2, 0);
	intel_uncore_write(uncore, DSPCLK_GATE_D, 0);
	intel_uncore_write(uncore, RAMCLK_GATE_D, 0);
	intel_uncore_write16(uncore, DEUC, 0);
	intel_uncore_write(uncore,
			   MI_ARB_STATE,
			   _MASKED_BIT_ENABLE(MI_ARB_DISPLAY_TRICKLE_FEED_DISABLE));
>>>>>>> upstream/android-13
}

static void i965g_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	I915_WRITE(RENCLK_GATE_D1, I965_RCZ_CLOCK_GATE_DISABLE |
=======
	intel_uncore_write(&dev_priv->uncore, RENCLK_GATE_D1, I965_RCZ_CLOCK_GATE_DISABLE |
>>>>>>> upstream/android-13
		   I965_RCC_CLOCK_GATE_DISABLE |
		   I965_RCPB_CLOCK_GATE_DISABLE |
		   I965_ISC_CLOCK_GATE_DISABLE |
		   I965_FBC_CLOCK_GATE_DISABLE);
<<<<<<< HEAD
	I915_WRITE(RENCLK_GATE_D2, 0);
	I915_WRITE(MI_ARB_STATE,
		   _MASKED_BIT_ENABLE(MI_ARB_DISPLAY_TRICKLE_FEED_DISABLE));

	/* WaDisable_RenderCache_OperationalFlush:gen4 */
	I915_WRITE(CACHE_MODE_0, _MASKED_BIT_DISABLE(RC_OP_FLUSH_ENABLE));
=======
	intel_uncore_write(&dev_priv->uncore, RENCLK_GATE_D2, 0);
	intel_uncore_write(&dev_priv->uncore, MI_ARB_STATE,
		   _MASKED_BIT_ENABLE(MI_ARB_DISPLAY_TRICKLE_FEED_DISABLE));
>>>>>>> upstream/android-13
}

static void gen3_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	u32 dstate = I915_READ(D_STATE);

	dstate |= DSTATE_PLL_D3_OFF | DSTATE_GFX_CLOCK_GATING |
		DSTATE_DOT_CLOCK_GATING;
	I915_WRITE(D_STATE, dstate);

	if (IS_PINEVIEW(dev_priv))
		I915_WRITE(ECOSKPD, _MASKED_BIT_ENABLE(ECO_GATING_CX_ONLY));

	/* IIR "flip pending" means done if this bit is set */
	I915_WRITE(ECOSKPD, _MASKED_BIT_DISABLE(ECO_FLIP_DONE));

	/* interrupts should cause a wake up from C3 */
	I915_WRITE(INSTPM, _MASKED_BIT_ENABLE(INSTPM_AGPBUSY_INT_EN));

	/* On GEN3 we really need to make sure the ARB C3 LP bit is set */
	I915_WRITE(MI_ARB_STATE, _MASKED_BIT_ENABLE(MI_ARB_C3_LP_WRITE_ENABLE));

	I915_WRITE(MI_ARB_STATE,
=======
	u32 dstate = intel_uncore_read(&dev_priv->uncore, D_STATE);

	dstate |= DSTATE_PLL_D3_OFF | DSTATE_GFX_CLOCK_GATING |
		DSTATE_DOT_CLOCK_GATING;
	intel_uncore_write(&dev_priv->uncore, D_STATE, dstate);

	if (IS_PINEVIEW(dev_priv))
		intel_uncore_write(&dev_priv->uncore, ECOSKPD, _MASKED_BIT_ENABLE(ECO_GATING_CX_ONLY));

	/* IIR "flip pending" means done if this bit is set */
	intel_uncore_write(&dev_priv->uncore, ECOSKPD, _MASKED_BIT_DISABLE(ECO_FLIP_DONE));

	/* interrupts should cause a wake up from C3 */
	intel_uncore_write(&dev_priv->uncore, INSTPM, _MASKED_BIT_ENABLE(INSTPM_AGPBUSY_INT_EN));

	/* On GEN3 we really need to make sure the ARB C3 LP bit is set */
	intel_uncore_write(&dev_priv->uncore, MI_ARB_STATE, _MASKED_BIT_ENABLE(MI_ARB_C3_LP_WRITE_ENABLE));

	intel_uncore_write(&dev_priv->uncore, MI_ARB_STATE,
>>>>>>> upstream/android-13
		   _MASKED_BIT_ENABLE(MI_ARB_DISPLAY_TRICKLE_FEED_DISABLE));
}

static void i85x_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	I915_WRITE(RENCLK_GATE_D1, SV_CLOCK_GATE_DISABLE);

	/* interrupts should cause a wake up from C3 */
	I915_WRITE(MI_STATE, _MASKED_BIT_ENABLE(MI_AGPBUSY_INT_EN) |
		   _MASKED_BIT_DISABLE(MI_AGPBUSY_830_MODE));

	I915_WRITE(MEM_MODE,
		   _MASKED_BIT_ENABLE(MEM_DISPLAY_TRICKLE_FEED_DISABLE));
=======
	intel_uncore_write(&dev_priv->uncore, RENCLK_GATE_D1, SV_CLOCK_GATE_DISABLE);

	/* interrupts should cause a wake up from C3 */
	intel_uncore_write(&dev_priv->uncore, MI_STATE, _MASKED_BIT_ENABLE(MI_AGPBUSY_INT_EN) |
		   _MASKED_BIT_DISABLE(MI_AGPBUSY_830_MODE));

	intel_uncore_write(&dev_priv->uncore, MEM_MODE,
		   _MASKED_BIT_ENABLE(MEM_DISPLAY_TRICKLE_FEED_DISABLE));

	/*
	 * Have FBC ignore 3D activity since we use software
	 * render tracking, and otherwise a pure 3D workload
	 * (even if it just renders a single frame and then does
	 * abosultely nothing) would not allow FBC to recompress
	 * until a 2D blit occurs.
	 */
	intel_uncore_write(&dev_priv->uncore, SCPD0,
		   _MASKED_BIT_ENABLE(SCPD_FBC_IGNORE_3D));
>>>>>>> upstream/android-13
}

static void i830_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	I915_WRITE(MEM_MODE,
=======
	intel_uncore_write(&dev_priv->uncore, MEM_MODE,
>>>>>>> upstream/android-13
		   _MASKED_BIT_ENABLE(MEM_DISPLAY_A_TRICKLE_FEED_DISABLE) |
		   _MASKED_BIT_ENABLE(MEM_DISPLAY_B_TRICKLE_FEED_DISABLE));
}

void intel_init_clock_gating(struct drm_i915_private *dev_priv)
{
	dev_priv->display.init_clock_gating(dev_priv);
}

void intel_suspend_hw(struct drm_i915_private *dev_priv)
{
	if (HAS_PCH_LPT(dev_priv))
		lpt_suspend_hw(dev_priv);
}

static void nop_init_clock_gating(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	DRM_DEBUG_KMS("No clock gating settings or workarounds applied.\n");
=======
	drm_dbg_kms(&dev_priv->drm,
		    "No clock gating settings or workarounds applied.\n");
>>>>>>> upstream/android-13
}

/**
 * intel_init_clock_gating_hooks - setup the clock gating hooks
 * @dev_priv: device private
 *
 * Setup the hooks that configure which clocks of a given platform can be
 * gated and also apply various GT and display specific workarounds for these
 * platforms. Note that some GT specific workarounds are applied separately
 * when GPU contexts or batchbuffers start their execution.
 */
void intel_init_clock_gating_hooks(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	if (IS_ICELAKE(dev_priv))
		dev_priv->display.init_clock_gating = icl_init_clock_gating;
	else if (IS_CANNONLAKE(dev_priv))
		dev_priv->display.init_clock_gating = cnl_init_clock_gating;
	else if (IS_COFFEELAKE(dev_priv))
=======
	if (IS_ALDERLAKE_P(dev_priv))
		dev_priv->display.init_clock_gating = adlp_init_clock_gating;
	else if (IS_DG1(dev_priv))
		dev_priv->display.init_clock_gating = dg1_init_clock_gating;
	else if (GRAPHICS_VER(dev_priv) == 12)
		dev_priv->display.init_clock_gating = gen12lp_init_clock_gating;
	else if (GRAPHICS_VER(dev_priv) == 11)
		dev_priv->display.init_clock_gating = icl_init_clock_gating;
	else if (IS_COFFEELAKE(dev_priv) || IS_COMETLAKE(dev_priv))
>>>>>>> upstream/android-13
		dev_priv->display.init_clock_gating = cfl_init_clock_gating;
	else if (IS_SKYLAKE(dev_priv))
		dev_priv->display.init_clock_gating = skl_init_clock_gating;
	else if (IS_KABYLAKE(dev_priv))
		dev_priv->display.init_clock_gating = kbl_init_clock_gating;
	else if (IS_BROXTON(dev_priv))
		dev_priv->display.init_clock_gating = bxt_init_clock_gating;
	else if (IS_GEMINILAKE(dev_priv))
		dev_priv->display.init_clock_gating = glk_init_clock_gating;
	else if (IS_BROADWELL(dev_priv))
		dev_priv->display.init_clock_gating = bdw_init_clock_gating;
	else if (IS_CHERRYVIEW(dev_priv))
		dev_priv->display.init_clock_gating = chv_init_clock_gating;
	else if (IS_HASWELL(dev_priv))
		dev_priv->display.init_clock_gating = hsw_init_clock_gating;
	else if (IS_IVYBRIDGE(dev_priv))
		dev_priv->display.init_clock_gating = ivb_init_clock_gating;
	else if (IS_VALLEYVIEW(dev_priv))
		dev_priv->display.init_clock_gating = vlv_init_clock_gating;
<<<<<<< HEAD
	else if (IS_GEN6(dev_priv))
		dev_priv->display.init_clock_gating = gen6_init_clock_gating;
	else if (IS_GEN5(dev_priv))
=======
	else if (GRAPHICS_VER(dev_priv) == 6)
		dev_priv->display.init_clock_gating = gen6_init_clock_gating;
	else if (GRAPHICS_VER(dev_priv) == 5)
>>>>>>> upstream/android-13
		dev_priv->display.init_clock_gating = ilk_init_clock_gating;
	else if (IS_G4X(dev_priv))
		dev_priv->display.init_clock_gating = g4x_init_clock_gating;
	else if (IS_I965GM(dev_priv))
		dev_priv->display.init_clock_gating = i965gm_init_clock_gating;
	else if (IS_I965G(dev_priv))
		dev_priv->display.init_clock_gating = i965g_init_clock_gating;
<<<<<<< HEAD
	else if (IS_GEN3(dev_priv))
		dev_priv->display.init_clock_gating = gen3_init_clock_gating;
	else if (IS_I85X(dev_priv) || IS_I865G(dev_priv))
		dev_priv->display.init_clock_gating = i85x_init_clock_gating;
	else if (IS_GEN2(dev_priv))
=======
	else if (GRAPHICS_VER(dev_priv) == 3)
		dev_priv->display.init_clock_gating = gen3_init_clock_gating;
	else if (IS_I85X(dev_priv) || IS_I865G(dev_priv))
		dev_priv->display.init_clock_gating = i85x_init_clock_gating;
	else if (GRAPHICS_VER(dev_priv) == 2)
>>>>>>> upstream/android-13
		dev_priv->display.init_clock_gating = i830_init_clock_gating;
	else {
		MISSING_CASE(INTEL_DEVID(dev_priv));
		dev_priv->display.init_clock_gating = nop_init_clock_gating;
	}
}

/* Set up chip specific power management-related functions */
void intel_init_pm(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	intel_fbc_init(dev_priv);

	/* For cxsr */
	if (IS_PINEVIEW(dev_priv))
		i915_pineview_get_mem_freq(dev_priv);
	else if (IS_GEN5(dev_priv))
		i915_ironlake_get_mem_freq(dev_priv);

	/* For FIFO watermark updates */
	if (INTEL_GEN(dev_priv) >= 9) {
		skl_setup_wm_latency(dev_priv);
		dev_priv->display.initial_watermarks = skl_initial_wm;
		dev_priv->display.atomic_update_watermarks = skl_atomic_update_crtc_wm;
=======
	/* For cxsr */
	if (IS_PINEVIEW(dev_priv))
		pnv_get_mem_freq(dev_priv);
	else if (GRAPHICS_VER(dev_priv) == 5)
		ilk_get_mem_freq(dev_priv);

	if (intel_has_sagv(dev_priv))
		skl_setup_sagv_block_time(dev_priv);

	/* For FIFO watermark updates */
	if (DISPLAY_VER(dev_priv) >= 9) {
		skl_setup_wm_latency(dev_priv);
>>>>>>> upstream/android-13
		dev_priv->display.compute_global_watermarks = skl_compute_wm;
	} else if (HAS_PCH_SPLIT(dev_priv)) {
		ilk_setup_wm_latency(dev_priv);

<<<<<<< HEAD
		if ((IS_GEN5(dev_priv) && dev_priv->wm.pri_latency[1] &&
		     dev_priv->wm.spr_latency[1] && dev_priv->wm.cur_latency[1]) ||
		    (!IS_GEN5(dev_priv) && dev_priv->wm.pri_latency[0] &&
=======
		if ((DISPLAY_VER(dev_priv) == 5 && dev_priv->wm.pri_latency[1] &&
		     dev_priv->wm.spr_latency[1] && dev_priv->wm.cur_latency[1]) ||
		    (DISPLAY_VER(dev_priv) != 5 && dev_priv->wm.pri_latency[0] &&
>>>>>>> upstream/android-13
		     dev_priv->wm.spr_latency[0] && dev_priv->wm.cur_latency[0])) {
			dev_priv->display.compute_pipe_wm = ilk_compute_pipe_wm;
			dev_priv->display.compute_intermediate_wm =
				ilk_compute_intermediate_wm;
			dev_priv->display.initial_watermarks =
				ilk_initial_watermarks;
			dev_priv->display.optimize_watermarks =
				ilk_optimize_watermarks;
		} else {
<<<<<<< HEAD
			DRM_DEBUG_KMS("Failed to read display plane latency. "
				      "Disable CxSR\n");
=======
			drm_dbg_kms(&dev_priv->drm,
				    "Failed to read display plane latency. "
				    "Disable CxSR\n");
>>>>>>> upstream/android-13
		}
	} else if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv)) {
		vlv_setup_wm_latency(dev_priv);
		dev_priv->display.compute_pipe_wm = vlv_compute_pipe_wm;
		dev_priv->display.compute_intermediate_wm = vlv_compute_intermediate_wm;
		dev_priv->display.initial_watermarks = vlv_initial_watermarks;
		dev_priv->display.optimize_watermarks = vlv_optimize_watermarks;
		dev_priv->display.atomic_update_watermarks = vlv_atomic_update_fifo;
	} else if (IS_G4X(dev_priv)) {
		g4x_setup_wm_latency(dev_priv);
		dev_priv->display.compute_pipe_wm = g4x_compute_pipe_wm;
		dev_priv->display.compute_intermediate_wm = g4x_compute_intermediate_wm;
		dev_priv->display.initial_watermarks = g4x_initial_watermarks;
		dev_priv->display.optimize_watermarks = g4x_optimize_watermarks;
	} else if (IS_PINEVIEW(dev_priv)) {
<<<<<<< HEAD
		if (!intel_get_cxsr_latency(IS_PINEVIEW_G(dev_priv),
					    dev_priv->is_ddr3,
					    dev_priv->fsb_freq,
					    dev_priv->mem_freq)) {
			DRM_INFO("failed to find known CxSR latency "
=======
		if (!intel_get_cxsr_latency(!IS_MOBILE(dev_priv),
					    dev_priv->is_ddr3,
					    dev_priv->fsb_freq,
					    dev_priv->mem_freq)) {
			drm_info(&dev_priv->drm,
				 "failed to find known CxSR latency "
>>>>>>> upstream/android-13
				 "(found ddr%s fsb freq %d, mem freq %d), "
				 "disabling CxSR\n",
				 (dev_priv->is_ddr3 == 1) ? "3" : "2",
				 dev_priv->fsb_freq, dev_priv->mem_freq);
			/* Disable CxSR and never update its watermark again */
			intel_set_memory_cxsr(dev_priv, false);
			dev_priv->display.update_wm = NULL;
		} else
<<<<<<< HEAD
			dev_priv->display.update_wm = pineview_update_wm;
	} else if (IS_GEN4(dev_priv)) {
		dev_priv->display.update_wm = i965_update_wm;
	} else if (IS_GEN3(dev_priv)) {
		dev_priv->display.update_wm = i9xx_update_wm;
		dev_priv->display.get_fifo_size = i9xx_get_fifo_size;
	} else if (IS_GEN2(dev_priv)) {
		if (INTEL_INFO(dev_priv)->num_pipes == 1) {
=======
			dev_priv->display.update_wm = pnv_update_wm;
	} else if (DISPLAY_VER(dev_priv) == 4) {
		dev_priv->display.update_wm = i965_update_wm;
	} else if (DISPLAY_VER(dev_priv) == 3) {
		dev_priv->display.update_wm = i9xx_update_wm;
		dev_priv->display.get_fifo_size = i9xx_get_fifo_size;
	} else if (DISPLAY_VER(dev_priv) == 2) {
		if (INTEL_NUM_PIPES(dev_priv) == 1) {
>>>>>>> upstream/android-13
			dev_priv->display.update_wm = i845_update_wm;
			dev_priv->display.get_fifo_size = i845_get_fifo_size;
		} else {
			dev_priv->display.update_wm = i9xx_update_wm;
			dev_priv->display.get_fifo_size = i830_get_fifo_size;
		}
	} else {
<<<<<<< HEAD
		DRM_ERROR("unexpected fall-through in intel_init_pm\n");
	}
}

static inline int gen6_check_mailbox_status(struct drm_i915_private *dev_priv)
{
	uint32_t flags =
		I915_READ_FW(GEN6_PCODE_MAILBOX) & GEN6_PCODE_ERROR_MASK;

	switch (flags) {
	case GEN6_PCODE_SUCCESS:
		return 0;
	case GEN6_PCODE_UNIMPLEMENTED_CMD:
		return -ENODEV;
	case GEN6_PCODE_ILLEGAL_CMD:
		return -ENXIO;
	case GEN6_PCODE_MIN_FREQ_TABLE_GT_RATIO_OUT_OF_RANGE:
	case GEN7_PCODE_MIN_FREQ_TABLE_GT_RATIO_OUT_OF_RANGE:
		return -EOVERFLOW;
	case GEN6_PCODE_TIMEOUT:
		return -ETIMEDOUT;
	default:
		MISSING_CASE(flags);
		return 0;
	}
}

static inline int gen7_check_mailbox_status(struct drm_i915_private *dev_priv)
{
	uint32_t flags =
		I915_READ_FW(GEN6_PCODE_MAILBOX) & GEN6_PCODE_ERROR_MASK;

	switch (flags) {
	case GEN6_PCODE_SUCCESS:
		return 0;
	case GEN6_PCODE_ILLEGAL_CMD:
		return -ENXIO;
	case GEN7_PCODE_TIMEOUT:
		return -ETIMEDOUT;
	case GEN7_PCODE_ILLEGAL_DATA:
		return -EINVAL;
	case GEN7_PCODE_MIN_FREQ_TABLE_GT_RATIO_OUT_OF_RANGE:
		return -EOVERFLOW;
	default:
		MISSING_CASE(flags);
		return 0;
	}
}

int sandybridge_pcode_read(struct drm_i915_private *dev_priv, u32 mbox, u32 *val)
{
	int status;

	WARN_ON(!mutex_is_locked(&dev_priv->pcu_lock));

	/* GEN6_PCODE_* are outside of the forcewake domain, we can
	 * use te fw I915_READ variants to reduce the amount of work
	 * required when reading/writing.
	 */

	if (I915_READ_FW(GEN6_PCODE_MAILBOX) & GEN6_PCODE_READY) {
		DRM_DEBUG_DRIVER("warning: pcode (read from mbox %x) mailbox access failed for %ps\n",
				 mbox, __builtin_return_address(0));
		return -EAGAIN;
	}

	I915_WRITE_FW(GEN6_PCODE_DATA, *val);
	I915_WRITE_FW(GEN6_PCODE_DATA1, 0);
	I915_WRITE_FW(GEN6_PCODE_MAILBOX, GEN6_PCODE_READY | mbox);

	if (__intel_wait_for_register_fw(dev_priv,
					 GEN6_PCODE_MAILBOX, GEN6_PCODE_READY, 0,
					 500, 0, NULL)) {
		DRM_ERROR("timeout waiting for pcode read (from mbox %x) to finish for %ps\n",
			  mbox, __builtin_return_address(0));
		return -ETIMEDOUT;
	}

	*val = I915_READ_FW(GEN6_PCODE_DATA);
	I915_WRITE_FW(GEN6_PCODE_DATA, 0);

	if (INTEL_GEN(dev_priv) > 6)
		status = gen7_check_mailbox_status(dev_priv);
	else
		status = gen6_check_mailbox_status(dev_priv);

	if (status) {
		DRM_DEBUG_DRIVER("warning: pcode (read from mbox %x) mailbox access failed for %ps: %d\n",
				 mbox, __builtin_return_address(0), status);
		return status;
	}

	return 0;
}

int sandybridge_pcode_write_timeout(struct drm_i915_private *dev_priv,
				    u32 mbox, u32 val,
				    int fast_timeout_us, int slow_timeout_ms)
{
	int status;

	WARN_ON(!mutex_is_locked(&dev_priv->pcu_lock));

	/* GEN6_PCODE_* are outside of the forcewake domain, we can
	 * use te fw I915_READ variants to reduce the amount of work
	 * required when reading/writing.
	 */

	if (I915_READ_FW(GEN6_PCODE_MAILBOX) & GEN6_PCODE_READY) {
		DRM_DEBUG_DRIVER("warning: pcode (write of 0x%08x to mbox %x) mailbox access failed for %ps\n",
				 val, mbox, __builtin_return_address(0));
		return -EAGAIN;
	}

	I915_WRITE_FW(GEN6_PCODE_DATA, val);
	I915_WRITE_FW(GEN6_PCODE_DATA1, 0);
	I915_WRITE_FW(GEN6_PCODE_MAILBOX, GEN6_PCODE_READY | mbox);

	if (__intel_wait_for_register_fw(dev_priv,
					 GEN6_PCODE_MAILBOX, GEN6_PCODE_READY, 0,
					 fast_timeout_us, slow_timeout_ms,
					 NULL)) {
		DRM_ERROR("timeout waiting for pcode write of 0x%08x to mbox %x to finish for %ps\n",
			  val, mbox, __builtin_return_address(0));
		return -ETIMEDOUT;
	}

	I915_WRITE_FW(GEN6_PCODE_DATA, 0);

	if (INTEL_GEN(dev_priv) > 6)
		status = gen7_check_mailbox_status(dev_priv);
	else
		status = gen6_check_mailbox_status(dev_priv);

	if (status) {
		DRM_DEBUG_DRIVER("warning: pcode (write of 0x%08x to mbox %x) mailbox access failed for %ps: %d\n",
				 val, mbox, __builtin_return_address(0), status);
		return status;
	}

	return 0;
}

static bool skl_pcode_try_request(struct drm_i915_private *dev_priv, u32 mbox,
				  u32 request, u32 reply_mask, u32 reply,
				  u32 *status)
{
	u32 val = request;

	*status = sandybridge_pcode_read(dev_priv, mbox, &val);

	return *status || ((val & reply_mask) == reply);
}

/**
 * skl_pcode_request - send PCODE request until acknowledgment
 * @dev_priv: device private
 * @mbox: PCODE mailbox ID the request is targeted for
 * @request: request ID
 * @reply_mask: mask used to check for request acknowledgment
 * @reply: value used to check for request acknowledgment
 * @timeout_base_ms: timeout for polling with preemption enabled
 *
 * Keep resending the @request to @mbox until PCODE acknowledges it, PCODE
 * reports an error or an overall timeout of @timeout_base_ms+50 ms expires.
 * The request is acknowledged once the PCODE reply dword equals @reply after
 * applying @reply_mask. Polling is first attempted with preemption enabled
 * for @timeout_base_ms and if this times out for another 50 ms with
 * preemption disabled.
 *
 * Returns 0 on success, %-ETIMEDOUT in case of a timeout, <0 in case of some
 * other error as reported by PCODE.
 */
int skl_pcode_request(struct drm_i915_private *dev_priv, u32 mbox, u32 request,
		      u32 reply_mask, u32 reply, int timeout_base_ms)
{
	u32 status;
	int ret;

	WARN_ON(!mutex_is_locked(&dev_priv->pcu_lock));

#define COND skl_pcode_try_request(dev_priv, mbox, request, reply_mask, reply, \
				   &status)

	/*
	 * Prime the PCODE by doing a request first. Normally it guarantees
	 * that a subsequent request, at most @timeout_base_ms later, succeeds.
	 * _wait_for() doesn't guarantee when its passed condition is evaluated
	 * first, so send the first request explicitly.
	 */
	if (COND) {
		ret = 0;
		goto out;
	}
	ret = _wait_for(COND, timeout_base_ms * 1000, 10, 10);
	if (!ret)
		goto out;

	/*
	 * The above can time out if the number of requests was low (2 in the
	 * worst case) _and_ PCODE was busy for some reason even after a
	 * (queued) request and @timeout_base_ms delay. As a workaround retry
	 * the poll with preemption disabled to maximize the number of
	 * requests. Increase the timeout from @timeout_base_ms to 50ms to
	 * account for interrupts that could reduce the number of these
	 * requests, and for any quirks of the PCODE firmware that delays
	 * the request completion.
	 */
	DRM_DEBUG_KMS("PCODE timeout, retrying with preemption disabled\n");
	WARN_ON_ONCE(timeout_base_ms > 3);
	preempt_disable();
	ret = wait_for_atomic(COND, 50);
	preempt_enable();

out:
	return ret ? ret : status;
#undef COND
}

static int byt_gpu_freq(struct drm_i915_private *dev_priv, int val)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;

	/*
	 * N = val - 0xb7
	 * Slow = Fast = GPLL ref * N
	 */
	return DIV_ROUND_CLOSEST(rps->gpll_ref_freq * (val - 0xb7), 1000);
}

static int byt_freq_opcode(struct drm_i915_private *dev_priv, int val)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;

	return DIV_ROUND_CLOSEST(1000 * val, rps->gpll_ref_freq) + 0xb7;
}

static int chv_gpu_freq(struct drm_i915_private *dev_priv, int val)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;

	/*
	 * N = val / 2
	 * CU (slow) = CU2x (fast) / 2 = GPLL ref * N / 2
	 */
	return DIV_ROUND_CLOSEST(rps->gpll_ref_freq * val, 2 * 2 * 1000);
}

static int chv_freq_opcode(struct drm_i915_private *dev_priv, int val)
{
	struct intel_rps *rps = &dev_priv->gt_pm.rps;

	/* CHV needs even values */
	return DIV_ROUND_CLOSEST(2 * 1000 * val, rps->gpll_ref_freq) * 2;
}

int intel_gpu_freq(struct drm_i915_private *dev_priv, int val)
{
	if (INTEL_GEN(dev_priv) >= 9)
		return DIV_ROUND_CLOSEST(val * GT_FREQUENCY_MULTIPLIER,
					 GEN9_FREQ_SCALER);
	else if (IS_CHERRYVIEW(dev_priv))
		return chv_gpu_freq(dev_priv, val);
	else if (IS_VALLEYVIEW(dev_priv))
		return byt_gpu_freq(dev_priv, val);
	else
		return val * GT_FREQUENCY_MULTIPLIER;
}

int intel_freq_opcode(struct drm_i915_private *dev_priv, int val)
{
	if (INTEL_GEN(dev_priv) >= 9)
		return DIV_ROUND_CLOSEST(val * GEN9_FREQ_SCALER,
					 GT_FREQUENCY_MULTIPLIER);
	else if (IS_CHERRYVIEW(dev_priv))
		return chv_freq_opcode(dev_priv, val);
	else if (IS_VALLEYVIEW(dev_priv))
		return byt_freq_opcode(dev_priv, val);
	else
		return DIV_ROUND_CLOSEST(val, GT_FREQUENCY_MULTIPLIER);
}

void intel_pm_setup(struct drm_i915_private *dev_priv)
{
	mutex_init(&dev_priv->pcu_lock);
	mutex_init(&dev_priv->gt_pm.rps.power.mutex);

	atomic_set(&dev_priv->gt_pm.rps.num_waiters, 0);

=======
		drm_err(&dev_priv->drm,
			"unexpected fall-through in %s\n", __func__);
	}
}

void intel_pm_setup(struct drm_i915_private *dev_priv)
{
>>>>>>> upstream/android-13
	dev_priv->runtime_pm.suspended = false;
	atomic_set(&dev_priv->runtime_pm.wakeref_count, 0);
}

<<<<<<< HEAD
static u64 vlv_residency_raw(struct drm_i915_private *dev_priv,
			     const i915_reg_t reg)
{
	u32 lower, upper, tmp;
	int loop = 2;

	/*
	 * The register accessed do not need forcewake. We borrow
	 * uncore lock to prevent concurrent access to range reg.
	 */
	lockdep_assert_held(&dev_priv->uncore.lock);

	/*
	 * vlv and chv residency counters are 40 bits in width.
	 * With a control bit, we can choose between upper or lower
	 * 32bit window into this counter.
	 *
	 * Although we always use the counter in high-range mode elsewhere,
	 * userspace may attempt to read the value before rc6 is initialised,
	 * before we have set the default VLV_COUNTER_CONTROL value. So always
	 * set the high bit to be safe.
	 */
	I915_WRITE_FW(VLV_COUNTER_CONTROL,
		      _MASKED_BIT_ENABLE(VLV_COUNT_RANGE_HIGH));
	upper = I915_READ_FW(reg);
	do {
		tmp = upper;

		I915_WRITE_FW(VLV_COUNTER_CONTROL,
			      _MASKED_BIT_DISABLE(VLV_COUNT_RANGE_HIGH));
		lower = I915_READ_FW(reg);

		I915_WRITE_FW(VLV_COUNTER_CONTROL,
			      _MASKED_BIT_ENABLE(VLV_COUNT_RANGE_HIGH));
		upper = I915_READ_FW(reg);
	} while (upper != tmp && --loop);

	/*
	 * Everywhere else we always use VLV_COUNTER_CONTROL with the
	 * VLV_COUNT_RANGE_HIGH bit set - so it is safe to leave it set
	 * now.
	 */

	return lower | (u64)upper << 8;
}

u64 intel_rc6_residency_ns(struct drm_i915_private *dev_priv,
			   const i915_reg_t reg)
{
	u64 time_hw, prev_hw, overflow_hw;
	unsigned int fw_domains;
	unsigned long flags;
	unsigned int i;
	u32 mul, div;

	if (!HAS_RC6(dev_priv))
		return 0;

	/*
	 * Store previous hw counter values for counter wrap-around handling.
	 *
	 * There are only four interesting registers and they live next to each
	 * other so we can use the relative address, compared to the smallest
	 * one as the index into driver storage.
	 */
	i = (i915_mmio_reg_offset(reg) -
	     i915_mmio_reg_offset(GEN6_GT_GFX_RC6_LOCKED)) / sizeof(u32);
	if (WARN_ON_ONCE(i >= ARRAY_SIZE(dev_priv->gt_pm.rc6.cur_residency)))
		return 0;

	fw_domains = intel_uncore_forcewake_for_reg(dev_priv, reg, FW_REG_READ);

	spin_lock_irqsave(&dev_priv->uncore.lock, flags);
	intel_uncore_forcewake_get__locked(dev_priv, fw_domains);

	/* On VLV and CHV, residency time is in CZ units rather than 1.28us */
	if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv)) {
		mul = 1000000;
		div = dev_priv->czclk_freq;
		overflow_hw = BIT_ULL(40);
		time_hw = vlv_residency_raw(dev_priv, reg);
	} else {
		/* 833.33ns units on Gen9LP, 1.28us elsewhere. */
		if (IS_GEN9_LP(dev_priv)) {
			mul = 10000;
			div = 12;
		} else {
			mul = 1280;
			div = 1;
		}

		overflow_hw = BIT_ULL(32);
		time_hw = I915_READ_FW(reg);
	}

	/*
	 * Counter wrap handling.
	 *
	 * But relying on a sufficient frequency of queries otherwise counters
	 * can still wrap.
	 */
	prev_hw = dev_priv->gt_pm.rc6.prev_hw_residency[i];
	dev_priv->gt_pm.rc6.prev_hw_residency[i] = time_hw;

	/* RC6 delta from last sample. */
	if (time_hw >= prev_hw)
		time_hw -= prev_hw;
	else
		time_hw += overflow_hw - prev_hw;

	/* Add delta to RC6 extended raw driver copy. */
	time_hw += dev_priv->gt_pm.rc6.cur_residency[i];
	dev_priv->gt_pm.rc6.cur_residency[i] = time_hw;

	intel_uncore_forcewake_put__locked(dev_priv, fw_domains);
	spin_unlock_irqrestore(&dev_priv->uncore.lock, flags);

	return mul_u64_u32_div(time_hw, mul, div);
}

u32 intel_get_cagf(struct drm_i915_private *dev_priv, u32 rpstat)
{
	u32 cagf;

	if (INTEL_GEN(dev_priv) >= 9)
		cagf = (rpstat & GEN9_CAGF_MASK) >> GEN9_CAGF_SHIFT;
	else if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv))
		cagf = (rpstat & HSW_CAGF_MASK) >> HSW_CAGF_SHIFT;
	else
		cagf = (rpstat & GEN6_CAGF_MASK) >> GEN6_CAGF_SHIFT;

	return  cagf;
=======
static struct intel_global_state *intel_dbuf_duplicate_state(struct intel_global_obj *obj)
{
	struct intel_dbuf_state *dbuf_state;

	dbuf_state = kmemdup(obj->state, sizeof(*dbuf_state), GFP_KERNEL);
	if (!dbuf_state)
		return NULL;

	return &dbuf_state->base;
}

static void intel_dbuf_destroy_state(struct intel_global_obj *obj,
				     struct intel_global_state *state)
{
	kfree(state);
}

static const struct intel_global_state_funcs intel_dbuf_funcs = {
	.atomic_duplicate_state = intel_dbuf_duplicate_state,
	.atomic_destroy_state = intel_dbuf_destroy_state,
};

struct intel_dbuf_state *
intel_atomic_get_dbuf_state(struct intel_atomic_state *state)
{
	struct drm_i915_private *dev_priv = to_i915(state->base.dev);
	struct intel_global_state *dbuf_state;

	dbuf_state = intel_atomic_get_global_obj_state(state, &dev_priv->dbuf.obj);
	if (IS_ERR(dbuf_state))
		return ERR_CAST(dbuf_state);

	return to_intel_dbuf_state(dbuf_state);
}

int intel_dbuf_init(struct drm_i915_private *dev_priv)
{
	struct intel_dbuf_state *dbuf_state;

	dbuf_state = kzalloc(sizeof(*dbuf_state), GFP_KERNEL);
	if (!dbuf_state)
		return -ENOMEM;

	intel_atomic_global_obj_init(dev_priv, &dev_priv->dbuf.obj,
				     &dbuf_state->base, &intel_dbuf_funcs);

	return 0;
}

/*
 * Configure MBUS_CTL and all DBUF_CTL_S of each slice to join_mbus state before
 * update the request state of all DBUS slices.
 */
static void update_mbus_pre_enable(struct intel_atomic_state *state)
{
	struct drm_i915_private *dev_priv = to_i915(state->base.dev);
	u32 mbus_ctl, dbuf_min_tracker_val;
	enum dbuf_slice slice;
	const struct intel_dbuf_state *dbuf_state =
		intel_atomic_get_new_dbuf_state(state);

	if (!IS_ALDERLAKE_P(dev_priv))
		return;

	/*
	 * TODO: Implement vblank synchronized MBUS joining changes.
	 * Must be properly coordinated with dbuf reprogramming.
	 */
	if (dbuf_state->joined_mbus) {
		mbus_ctl = MBUS_HASHING_MODE_1x4 | MBUS_JOIN |
			MBUS_JOIN_PIPE_SELECT_NONE;
		dbuf_min_tracker_val = DBUF_MIN_TRACKER_STATE_SERVICE(3);
	} else {
		mbus_ctl = MBUS_HASHING_MODE_2x2 |
			MBUS_JOIN_PIPE_SELECT_NONE;
		dbuf_min_tracker_val = DBUF_MIN_TRACKER_STATE_SERVICE(1);
	}

	intel_de_rmw(dev_priv, MBUS_CTL,
		     MBUS_HASHING_MODE_MASK | MBUS_JOIN |
		     MBUS_JOIN_PIPE_SELECT_MASK, mbus_ctl);

	for_each_dbuf_slice(dev_priv, slice)
		intel_de_rmw(dev_priv, DBUF_CTL_S(slice),
			     DBUF_MIN_TRACKER_STATE_SERVICE_MASK,
			     dbuf_min_tracker_val);
}

void intel_dbuf_pre_plane_update(struct intel_atomic_state *state)
{
	struct drm_i915_private *dev_priv = to_i915(state->base.dev);
	const struct intel_dbuf_state *new_dbuf_state =
		intel_atomic_get_new_dbuf_state(state);
	const struct intel_dbuf_state *old_dbuf_state =
		intel_atomic_get_old_dbuf_state(state);

	if (!new_dbuf_state ||
	    ((new_dbuf_state->enabled_slices == old_dbuf_state->enabled_slices)
	    && (new_dbuf_state->joined_mbus == old_dbuf_state->joined_mbus)))
		return;

	WARN_ON(!new_dbuf_state->base.changed);

	update_mbus_pre_enable(state);
	gen9_dbuf_slices_update(dev_priv,
				old_dbuf_state->enabled_slices |
				new_dbuf_state->enabled_slices);
}

void intel_dbuf_post_plane_update(struct intel_atomic_state *state)
{
	struct drm_i915_private *dev_priv = to_i915(state->base.dev);
	const struct intel_dbuf_state *new_dbuf_state =
		intel_atomic_get_new_dbuf_state(state);
	const struct intel_dbuf_state *old_dbuf_state =
		intel_atomic_get_old_dbuf_state(state);

	if (!new_dbuf_state ||
	    ((new_dbuf_state->enabled_slices == old_dbuf_state->enabled_slices)
	    && (new_dbuf_state->joined_mbus == old_dbuf_state->joined_mbus)))
		return;

	WARN_ON(!new_dbuf_state->base.changed);

	gen9_dbuf_slices_update(dev_priv,
				new_dbuf_state->enabled_slices);
>>>>>>> upstream/android-13
}
