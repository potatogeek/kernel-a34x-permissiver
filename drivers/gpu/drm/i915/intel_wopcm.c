<<<<<<< HEAD
/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright © 2017-2018 Intel Corporation
=======
// SPDX-License-Identifier: MIT
/*
 * Copyright © 2017-2019 Intel Corporation
>>>>>>> upstream/android-13
 */

#include "intel_wopcm.h"
#include "i915_drv.h"

/**
 * DOC: WOPCM Layout
 *
 * The layout of the WOPCM will be fixed after writing to GuC WOPCM size and
 * offset registers whose values are calculated and determined by HuC/GuC
 * firmware size and set of hardware requirements/restrictions as shown below:
 *
 * ::
 *
 *    +=========> +====================+ <== WOPCM Top
 *    ^           |  HW contexts RSVD  |
 *    |     +===> +====================+ <== GuC WOPCM Top
 *    |     ^     |                    |
 *    |     |     |                    |
 *    |     |     |                    |
 *    |    GuC    |                    |
 *    |   WOPCM   |                    |
 *    |    Size   +--------------------+
 *  WOPCM   |     |    GuC FW RSVD     |
 *    |     |     +--------------------+
 *    |     |     |   GuC Stack RSVD   |
 *    |     |     +------------------- +
 *    |     v     |   GuC WOPCM RSVD   |
 *    |     +===> +====================+ <== GuC WOPCM base
 *    |           |     WOPCM RSVD     |
 *    |           +------------------- + <== HuC Firmware Top
 *    v           |      HuC FW        |
 *    +=========> +====================+ <== WOPCM Base
 *
 * GuC accessible WOPCM starts at GuC WOPCM base and ends at GuC WOPCM top.
 * The top part of the WOPCM is reserved for hardware contexts (e.g. RC6
 * context).
 */

<<<<<<< HEAD
/* Default WOPCM size 1MB. */
#define GEN9_WOPCM_SIZE			(1024 * 1024)
/* 16KB WOPCM (RSVD WOPCM) is reserved from HuC firmware top. */
#define WOPCM_RESERVED_SIZE		(16 * 1024)

/* 16KB reserved at the beginning of GuC WOPCM. */
#define GUC_WOPCM_RESERVED		(16 * 1024)
/* 8KB from GUC_WOPCM_RESERVED is reserved for GuC stack. */
#define GUC_WOPCM_STACK_RESERVED	(8 * 1024)
=======
/* Default WOPCM size is 2MB from Gen11, 1MB on previous platforms */
#define GEN11_WOPCM_SIZE		SZ_2M
#define GEN9_WOPCM_SIZE			SZ_1M
/* 16KB WOPCM (RSVD WOPCM) is reserved from HuC firmware top. */
#define WOPCM_RESERVED_SIZE		SZ_16K

/* 16KB reserved at the beginning of GuC WOPCM. */
#define GUC_WOPCM_RESERVED		SZ_16K
/* 8KB from GUC_WOPCM_RESERVED is reserved for GuC stack. */
#define GUC_WOPCM_STACK_RESERVED	SZ_8K
>>>>>>> upstream/android-13

/* GuC WOPCM Offset value needs to be aligned to 16KB. */
#define GUC_WOPCM_OFFSET_ALIGNMENT	(1UL << GUC_WOPCM_OFFSET_SHIFT)

/* 24KB at the end of WOPCM is reserved for RC6 CTX on BXT. */
<<<<<<< HEAD
#define BXT_WOPCM_RC6_CTX_RESERVED	(24 * 1024)
/* 36KB WOPCM reserved at the end of WOPCM on CNL. */
#define CNL_WOPCM_HW_CTX_RESERVED	(36 * 1024)

/* 128KB from GUC_WOPCM_RESERVED is reserved for FW on Gen9. */
#define GEN9_GUC_FW_RESERVED	(128 * 1024)
#define GEN9_GUC_WOPCM_OFFSET	(GUC_WOPCM_RESERVED + GEN9_GUC_FW_RESERVED)

=======
#define BXT_WOPCM_RC6_CTX_RESERVED	(SZ_16K + SZ_8K)
/* 36KB WOPCM reserved at the end of WOPCM on ICL. */
#define ICL_WOPCM_HW_CTX_RESERVED	(SZ_32K + SZ_4K)

/* 128KB from GUC_WOPCM_RESERVED is reserved for FW on Gen9. */
#define GEN9_GUC_FW_RESERVED	SZ_128K
#define GEN9_GUC_WOPCM_OFFSET	(GUC_WOPCM_RESERVED + GEN9_GUC_FW_RESERVED)

static inline struct drm_i915_private *wopcm_to_i915(struct intel_wopcm *wopcm)
{
	return container_of(wopcm, struct drm_i915_private, wopcm);
}

>>>>>>> upstream/android-13
/**
 * intel_wopcm_init_early() - Early initialization of the WOPCM.
 * @wopcm: pointer to intel_wopcm.
 *
 * Setup the size of WOPCM which will be used by later on WOPCM partitioning.
 */
void intel_wopcm_init_early(struct intel_wopcm *wopcm)
{
<<<<<<< HEAD
	wopcm->size = GEN9_WOPCM_SIZE;

	DRM_DEBUG_DRIVER("WOPCM size: %uKiB\n", wopcm->size / 1024);
}

static inline u32 context_reserved_size(struct drm_i915_private *i915)
{
	if (IS_GEN9_LP(i915))
		return BXT_WOPCM_RC6_CTX_RESERVED;
	else if (INTEL_GEN(i915) >= 10)
		return CNL_WOPCM_HW_CTX_RESERVED;
=======
	struct drm_i915_private *i915 = wopcm_to_i915(wopcm);

	if (!HAS_GT_UC(i915))
		return;

	if (GRAPHICS_VER(i915) >= 11)
		wopcm->size = GEN11_WOPCM_SIZE;
	else
		wopcm->size = GEN9_WOPCM_SIZE;

	drm_dbg(&i915->drm, "WOPCM: %uK\n", wopcm->size / 1024);
}

static u32 context_reserved_size(struct drm_i915_private *i915)
{
	if (IS_GEN9_LP(i915))
		return BXT_WOPCM_RC6_CTX_RESERVED;
	else if (GRAPHICS_VER(i915) >= 11)
		return ICL_WOPCM_HW_CTX_RESERVED;
>>>>>>> upstream/android-13
	else
		return 0;
}

<<<<<<< HEAD
static inline int gen9_check_dword_gap(u32 guc_wopcm_base, u32 guc_wopcm_size)
=======
static bool gen9_check_dword_gap(struct drm_i915_private *i915,
				 u32 guc_wopcm_base, u32 guc_wopcm_size)
>>>>>>> upstream/android-13
{
	u32 offset;

	/*
	 * GuC WOPCM size shall be at least a dword larger than the offset from
	 * WOPCM base (GuC WOPCM offset from WOPCM base + GEN9_GUC_WOPCM_OFFSET)
	 * due to hardware limitation on Gen9.
	 */
	offset = guc_wopcm_base + GEN9_GUC_WOPCM_OFFSET;
	if (offset > guc_wopcm_size ||
	    (guc_wopcm_size - offset) < sizeof(u32)) {
<<<<<<< HEAD
		DRM_ERROR("GuC WOPCM size %uKiB is too small. %uKiB needed.\n",
			  guc_wopcm_size / 1024,
			  (u32)(offset + sizeof(u32)) / 1024);
		return -E2BIG;
	}

	return 0;
}

static inline int gen9_check_huc_fw_fits(u32 guc_wopcm_size, u32 huc_fw_size)
{
	/*
	 * On Gen9 & CNL A0, hardware requires the total available GuC WOPCM
=======
		drm_err(&i915->drm,
			"WOPCM: invalid GuC region size: %uK < %uK\n",
			guc_wopcm_size / SZ_1K,
			(u32)(offset + sizeof(u32)) / SZ_1K);
		return false;
	}

	return true;
}

static bool gen9_check_huc_fw_fits(struct drm_i915_private *i915,
				   u32 guc_wopcm_size, u32 huc_fw_size)
{
	/*
	 * On Gen9, hardware requires the total available GuC WOPCM
>>>>>>> upstream/android-13
	 * size to be larger than or equal to HuC firmware size. Otherwise,
	 * firmware uploading would fail.
	 */
	if (huc_fw_size > guc_wopcm_size - GUC_WOPCM_RESERVED) {
<<<<<<< HEAD
		DRM_ERROR("HuC FW (%uKiB) won't fit in GuC WOPCM (%uKiB).\n",
			  huc_fw_size / 1024,
			  (guc_wopcm_size - GUC_WOPCM_RESERVED) / 1024);
		return -E2BIG;
	}

	return 0;
}

static inline int check_hw_restriction(struct drm_i915_private *i915,
				       u32 guc_wopcm_base, u32 guc_wopcm_size,
				       u32 huc_fw_size)
{
	int err = 0;

	if (IS_GEN9(i915))
		err = gen9_check_dword_gap(guc_wopcm_base, guc_wopcm_size);

	if (!err &&
	    (IS_GEN9(i915) || IS_CNL_REVID(i915, CNL_REVID_A0, CNL_REVID_A0)))
		err = gen9_check_huc_fw_fits(guc_wopcm_size, huc_fw_size);

	return err;
=======
		drm_err(&i915->drm, "WOPCM: no space for %s: %uK < %uK\n",
			intel_uc_fw_type_repr(INTEL_UC_FW_TYPE_HUC),
			(guc_wopcm_size - GUC_WOPCM_RESERVED) / SZ_1K,
			huc_fw_size / 1024);
		return false;
	}

	return true;
}

static bool check_hw_restrictions(struct drm_i915_private *i915,
				  u32 guc_wopcm_base, u32 guc_wopcm_size,
				  u32 huc_fw_size)
{
	if (GRAPHICS_VER(i915) == 9 && !gen9_check_dword_gap(i915, guc_wopcm_base,
							     guc_wopcm_size))
		return false;

	if (GRAPHICS_VER(i915) == 9 &&
	    !gen9_check_huc_fw_fits(i915, guc_wopcm_size, huc_fw_size))
		return false;

	return true;
}

static bool __check_layout(struct drm_i915_private *i915, u32 wopcm_size,
			   u32 guc_wopcm_base, u32 guc_wopcm_size,
			   u32 guc_fw_size, u32 huc_fw_size)
{
	const u32 ctx_rsvd = context_reserved_size(i915);
	u32 size;

	size = wopcm_size - ctx_rsvd;
	if (unlikely(range_overflows(guc_wopcm_base, guc_wopcm_size, size))) {
		drm_err(&i915->drm,
			"WOPCM: invalid GuC region layout: %uK + %uK > %uK\n",
			guc_wopcm_base / SZ_1K, guc_wopcm_size / SZ_1K,
			size / SZ_1K);
		return false;
	}

	size = guc_fw_size + GUC_WOPCM_RESERVED + GUC_WOPCM_STACK_RESERVED;
	if (unlikely(guc_wopcm_size < size)) {
		drm_err(&i915->drm, "WOPCM: no space for %s: %uK < %uK\n",
			intel_uc_fw_type_repr(INTEL_UC_FW_TYPE_GUC),
			guc_wopcm_size / SZ_1K, size / SZ_1K);
		return false;
	}

	size = huc_fw_size + WOPCM_RESERVED_SIZE;
	if (unlikely(guc_wopcm_base < size)) {
		drm_err(&i915->drm, "WOPCM: no space for %s: %uK < %uK\n",
			intel_uc_fw_type_repr(INTEL_UC_FW_TYPE_HUC),
			guc_wopcm_base / SZ_1K, size / SZ_1K);
		return false;
	}

	return check_hw_restrictions(i915, guc_wopcm_base, guc_wopcm_size,
				     huc_fw_size);
}

static bool __wopcm_regs_locked(struct intel_uncore *uncore,
				u32 *guc_wopcm_base, u32 *guc_wopcm_size)
{
	u32 reg_base = intel_uncore_read(uncore, DMA_GUC_WOPCM_OFFSET);
	u32 reg_size = intel_uncore_read(uncore, GUC_WOPCM_SIZE);

	if (!(reg_size & GUC_WOPCM_SIZE_LOCKED) ||
	    !(reg_base & GUC_WOPCM_OFFSET_VALID))
		return false;

	*guc_wopcm_base = reg_base & GUC_WOPCM_OFFSET_MASK;
	*guc_wopcm_size = reg_size & GUC_WOPCM_SIZE_MASK;
	return true;
>>>>>>> upstream/android-13
}

/**
 * intel_wopcm_init() - Initialize the WOPCM structure.
 * @wopcm: pointer to intel_wopcm.
 *
 * This function will partition WOPCM space based on GuC and HuC firmware sizes
 * and will allocate max remaining for use by GuC. This function will also
 * enforce platform dependent hardware restrictions on GuC WOPCM offset and
<<<<<<< HEAD
 * size. It will fail the WOPCM init if any of these checks were failed, so that
 * the following GuC firmware uploading would be aborted.
 *
 * Return: 0 on success, non-zero error code on failure.
 */
int intel_wopcm_init(struct intel_wopcm *wopcm)
{
	struct drm_i915_private *i915 = wopcm_to_i915(wopcm);
	u32 guc_fw_size = intel_uc_fw_get_upload_size(&i915->guc.fw);
	u32 huc_fw_size = intel_uc_fw_get_upload_size(&i915->huc.fw);
	u32 ctx_rsvd = context_reserved_size(i915);
	u32 guc_wopcm_base;
	u32 guc_wopcm_size;
	u32 guc_wopcm_rsvd;
	int err;

	GEM_BUG_ON(!wopcm->size);

	if (guc_fw_size >= wopcm->size) {
		DRM_ERROR("GuC FW (%uKiB) is too big to fit in WOPCM.",
			  guc_fw_size / 1024);
		return -E2BIG;
	}

	if (huc_fw_size >= wopcm->size) {
		DRM_ERROR("HuC FW (%uKiB) is too big to fit in WOPCM.",
			  huc_fw_size / 1024);
		return -E2BIG;
	}

	guc_wopcm_base = ALIGN(huc_fw_size + WOPCM_RESERVED_SIZE,
			       GUC_WOPCM_OFFSET_ALIGNMENT);
	if ((guc_wopcm_base + ctx_rsvd) >= wopcm->size) {
		DRM_ERROR("GuC WOPCM base (%uKiB) is too big.\n",
			  guc_wopcm_base / 1024);
		return -E2BIG;
	}

	guc_wopcm_size = wopcm->size - guc_wopcm_base - ctx_rsvd;
	guc_wopcm_size &= GUC_WOPCM_SIZE_MASK;

	DRM_DEBUG_DRIVER("Calculated GuC WOPCM Region: [%uKiB, %uKiB)\n",
			 guc_wopcm_base / 1024, guc_wopcm_size / 1024);

	guc_wopcm_rsvd = GUC_WOPCM_RESERVED + GUC_WOPCM_STACK_RESERVED;
	if ((guc_fw_size + guc_wopcm_rsvd) > guc_wopcm_size) {
		DRM_ERROR("Need %uKiB WOPCM for GuC, %uKiB available.\n",
			  (guc_fw_size + guc_wopcm_rsvd) / 1024,
			  guc_wopcm_size / 1024);
		return -E2BIG;
	}

	err = check_hw_restriction(i915, guc_wopcm_base, guc_wopcm_size,
				   huc_fw_size);
	if (err)
		return err;

	wopcm->guc.base = guc_wopcm_base;
	wopcm->guc.size = guc_wopcm_size;

	return 0;
}

static inline int write_and_verify(struct drm_i915_private *dev_priv,
				   i915_reg_t reg, u32 val, u32 mask,
				   u32 locked_bit)
{
	u32 reg_val;

	GEM_BUG_ON(val & ~mask);

	I915_WRITE(reg, val);

	reg_val = I915_READ(reg);

	return (reg_val & mask) != (val | locked_bit) ? -EIO : 0;
}

/**
 * intel_wopcm_init_hw() - Setup GuC WOPCM registers.
 * @wopcm: pointer to intel_wopcm.
 *
 * Setup the GuC WOPCM size and offset registers with the calculated values. It
 * will verify the register values to make sure the registers are locked with
 * correct values.
 *
 * Return: 0 on success. -EIO if registers were locked with incorrect values.
 */
int intel_wopcm_init_hw(struct intel_wopcm *wopcm)
{
	struct drm_i915_private *dev_priv = wopcm_to_i915(wopcm);
	u32 huc_agent;
	u32 mask;
	int err;

	if (!USES_GUC(dev_priv))
		return 0;

	GEM_BUG_ON(!HAS_GUC(dev_priv));
	GEM_BUG_ON(!wopcm->guc.size);
	GEM_BUG_ON(!wopcm->guc.base);

	err = write_and_verify(dev_priv, GUC_WOPCM_SIZE, wopcm->guc.size,
			       GUC_WOPCM_SIZE_MASK | GUC_WOPCM_SIZE_LOCKED,
			       GUC_WOPCM_SIZE_LOCKED);
	if (err)
		goto err_out;

	huc_agent = USES_HUC(dev_priv) ? HUC_LOADING_AGENT_GUC : 0;
	mask = GUC_WOPCM_OFFSET_MASK | GUC_WOPCM_OFFSET_VALID | huc_agent;
	err = write_and_verify(dev_priv, DMA_GUC_WOPCM_OFFSET,
			       wopcm->guc.base | huc_agent, mask,
			       GUC_WOPCM_OFFSET_VALID);
	if (err)
		goto err_out;

	return 0;

err_out:
	DRM_ERROR("Failed to init WOPCM registers:\n");
	DRM_ERROR("DMA_GUC_WOPCM_OFFSET=%#x\n",
		  I915_READ(DMA_GUC_WOPCM_OFFSET));
	DRM_ERROR("GUC_WOPCM_SIZE=%#x\n", I915_READ(GUC_WOPCM_SIZE));

	return err;
=======
 * size. It will fail the WOPCM init if any of these checks fail, so that the
 * following WOPCM registers setup and GuC firmware uploading would be aborted.
 */
void intel_wopcm_init(struct intel_wopcm *wopcm)
{
	struct drm_i915_private *i915 = wopcm_to_i915(wopcm);
	struct intel_gt *gt = &i915->gt;
	u32 guc_fw_size = intel_uc_fw_get_upload_size(&gt->uc.guc.fw);
	u32 huc_fw_size = intel_uc_fw_get_upload_size(&gt->uc.huc.fw);
	u32 ctx_rsvd = context_reserved_size(i915);
	u32 guc_wopcm_base;
	u32 guc_wopcm_size;

	if (!guc_fw_size)
		return;

	GEM_BUG_ON(!wopcm->size);
	GEM_BUG_ON(wopcm->guc.base);
	GEM_BUG_ON(wopcm->guc.size);
	GEM_BUG_ON(guc_fw_size >= wopcm->size);
	GEM_BUG_ON(huc_fw_size >= wopcm->size);
	GEM_BUG_ON(ctx_rsvd + WOPCM_RESERVED_SIZE >= wopcm->size);

	if (i915_inject_probe_failure(i915))
		return;

	if (__wopcm_regs_locked(gt->uncore, &guc_wopcm_base, &guc_wopcm_size)) {
		drm_dbg(&i915->drm, "GuC WOPCM is already locked [%uK, %uK)\n",
			guc_wopcm_base / SZ_1K, guc_wopcm_size / SZ_1K);
		goto check;
	}

	/*
	 * Aligned value of guc_wopcm_base will determine available WOPCM space
	 * for HuC firmware and mandatory reserved area.
	 */
	guc_wopcm_base = huc_fw_size + WOPCM_RESERVED_SIZE;
	guc_wopcm_base = ALIGN(guc_wopcm_base, GUC_WOPCM_OFFSET_ALIGNMENT);

	/*
	 * Need to clamp guc_wopcm_base now to make sure the following math is
	 * correct. Formal check of whole WOPCM layout will be done below.
	 */
	guc_wopcm_base = min(guc_wopcm_base, wopcm->size - ctx_rsvd);

	/* Aligned remainings of usable WOPCM space can be assigned to GuC. */
	guc_wopcm_size = wopcm->size - ctx_rsvd - guc_wopcm_base;
	guc_wopcm_size &= GUC_WOPCM_SIZE_MASK;

	drm_dbg(&i915->drm, "Calculated GuC WOPCM [%uK, %uK)\n",
		guc_wopcm_base / SZ_1K, guc_wopcm_size / SZ_1K);

check:
	if (__check_layout(i915, wopcm->size, guc_wopcm_base, guc_wopcm_size,
			   guc_fw_size, huc_fw_size)) {
		wopcm->guc.base = guc_wopcm_base;
		wopcm->guc.size = guc_wopcm_size;
		GEM_BUG_ON(!wopcm->guc.base);
		GEM_BUG_ON(!wopcm->guc.size);
	}
>>>>>>> upstream/android-13
}
