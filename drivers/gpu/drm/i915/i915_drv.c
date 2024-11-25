/* i915_drv.c -- i830,i845,i855,i865,i915 driver -*- linux-c -*-
 */
/*
 *
 * Copyright 2003 Tungsten Graphics, Inc., Cedar Park, Texas.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <linux/acpi.h>
#include <linux/device.h>
#include <linux/oom.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/pm.h>
#include <linux/pm_runtime.h>
#include <linux/pnp.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/vgaarb.h>
#include <linux/vga_switcheroo.h>
#include <linux/vt.h>
#include <acpi/video.h>

#include <drm/drmP.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_atomic_helper.h>
#include <drm/i915_drm.h>

#include "i915_drv.h"
#include "i915_trace.h"
#include "i915_pmu.h"
#include "i915_query.h"
#include "i915_vgpu.h"
#include "intel_drv.h"
#include "intel_uc.h"

static struct drm_driver driver;

#if IS_ENABLED(CONFIG_DRM_I915_DEBUG)
static unsigned int i915_load_fail_count;

bool __i915_inject_load_failure(const char *func, int line)
{
	if (i915_load_fail_count >= i915_modparams.inject_load_failure)
		return false;

	if (++i915_load_fail_count == i915_modparams.inject_load_failure) {
		DRM_INFO("Injecting failure at checkpoint %u [%s:%d]\n",
			 i915_modparams.inject_load_failure, func, line);
		i915_modparams.inject_load_failure = 0;
		return true;
	}

	return false;
}

bool i915_error_injected(void)
{
	return i915_load_fail_count && !i915_modparams.inject_load_failure;
}

#endif

#define FDO_BUG_URL "https://bugs.freedesktop.org/enter_bug.cgi?product=DRI"
#define FDO_BUG_MSG "Please file a bug at " FDO_BUG_URL " against DRM/Intel " \
		    "providing the dmesg log by booting with drm.debug=0xf"

void
__i915_printk(struct drm_i915_private *dev_priv, const char *level,
	      const char *fmt, ...)
{
	static bool shown_bug_once;
	struct device *kdev = dev_priv->drm.dev;
	bool is_error = level[1] <= KERN_ERR[1];
	bool is_debug = level[1] == KERN_DEBUG[1];
	struct va_format vaf;
	va_list args;

	if (is_debug && !(drm_debug & DRM_UT_DRIVER))
		return;

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	if (is_error)
		dev_printk(level, kdev, "%pV", &vaf);
	else
		dev_printk(level, kdev, "[" DRM_NAME ":%ps] %pV",
			   __builtin_return_address(0), &vaf);

	va_end(args);

	if (is_error && !shown_bug_once) {
		/*
		 * Ask the user to file a bug report for the error, except
		 * if they may have caused the bug by fiddling with unsafe
		 * module parameters.
		 */
		if (!test_taint(TAINT_USER))
			dev_notice(kdev, "%s", FDO_BUG_MSG);
		shown_bug_once = true;
	}
}

/* Map PCH device id to PCH type, or PCH_NONE if unknown. */
static enum intel_pch
intel_pch_type(const struct drm_i915_private *dev_priv, unsigned short id)
{
	switch (id) {
	case INTEL_PCH_IBX_DEVICE_ID_TYPE:
		DRM_DEBUG_KMS("Found Ibex Peak PCH\n");
		WARN_ON(!IS_GEN5(dev_priv));
		return PCH_IBX;
	case INTEL_PCH_CPT_DEVICE_ID_TYPE:
		DRM_DEBUG_KMS("Found CougarPoint PCH\n");
		WARN_ON(!IS_GEN6(dev_priv) && !IS_IVYBRIDGE(dev_priv));
		return PCH_CPT;
	case INTEL_PCH_PPT_DEVICE_ID_TYPE:
		DRM_DEBUG_KMS("Found PantherPoint PCH\n");
		WARN_ON(!IS_GEN6(dev_priv) && !IS_IVYBRIDGE(dev_priv));
		/* PantherPoint is CPT compatible */
		return PCH_CPT;
	case INTEL_PCH_LPT_DEVICE_ID_TYPE:
		DRM_DEBUG_KMS("Found LynxPoint PCH\n");
		WARN_ON(!IS_HASWELL(dev_priv) && !IS_BROADWELL(dev_priv));
		WARN_ON(IS_HSW_ULT(dev_priv) || IS_BDW_ULT(dev_priv));
		return PCH_LPT;
	case INTEL_PCH_LPT_LP_DEVICE_ID_TYPE:
		DRM_DEBUG_KMS("Found LynxPoint LP PCH\n");
		WARN_ON(!IS_HASWELL(dev_priv) && !IS_BROADWELL(dev_priv));
		WARN_ON(!IS_HSW_ULT(dev_priv) && !IS_BDW_ULT(dev_priv));
		return PCH_LPT;
	case INTEL_PCH_WPT_DEVICE_ID_TYPE:
		DRM_DEBUG_KMS("Found WildcatPoint PCH\n");
		WARN_ON(!IS_HASWELL(dev_priv) && !IS_BROADWELL(dev_priv));
		WARN_ON(IS_HSW_ULT(dev_priv) || IS_BDW_ULT(dev_priv));
		/* WildcatPoint is LPT compatible */
		return PCH_LPT;
	case INTEL_PCH_WPT_LP_DEVICE_ID_TYPE:
		DRM_DEBUG_KMS("Found WildcatPoint LP PCH\n");
		WARN_ON(!IS_HASWELL(dev_priv) && !IS_BROADWELL(dev_priv));
		WARN_ON(!IS_HSW_ULT(dev_priv) && !IS_BDW_ULT(dev_priv));
		/* WildcatPoint is LPT compatible */
		return PCH_LPT;
	case INTEL_PCH_SPT_DEVICE_ID_TYPE:
		DRM_DEBUG_KMS("Found SunrisePoint PCH\n");
		WARN_ON(!IS_SKYLAKE(dev_priv) && !IS_KABYLAKE(dev_priv));
		return PCH_SPT;
	case INTEL_PCH_SPT_LP_DEVICE_ID_TYPE:
		DRM_DEBUG_KMS("Found SunrisePoint LP PCH\n");
		WARN_ON(!IS_SKYLAKE(dev_priv) && !IS_KABYLAKE(dev_priv));
		return PCH_SPT;
	case INTEL_PCH_KBP_DEVICE_ID_TYPE:
		DRM_DEBUG_KMS("Found Kaby Lake PCH (KBP)\n");
		WARN_ON(!IS_SKYLAKE(dev_priv) && !IS_KABYLAKE(dev_priv) &&
			!IS_COFFEELAKE(dev_priv));
		return PCH_KBP;
	case INTEL_PCH_CNP_DEVICE_ID_TYPE:
		DRM_DEBUG_KMS("Found Cannon Lake PCH (CNP)\n");
		WARN_ON(!IS_CANNONLAKE(dev_priv) && !IS_COFFEELAKE(dev_priv));
		return PCH_CNP;
	case INTEL_PCH_CNP_LP_DEVICE_ID_TYPE:
		DRM_DEBUG_KMS("Found Cannon Lake LP PCH (CNP-LP)\n");
		WARN_ON(!IS_CANNONLAKE(dev_priv) && !IS_COFFEELAKE(dev_priv));
		return PCH_CNP;
	case INTEL_PCH_ICP_DEVICE_ID_TYPE:
		DRM_DEBUG_KMS("Found Ice Lake PCH\n");
		WARN_ON(!IS_ICELAKE(dev_priv));
		return PCH_ICP;
	default:
		return PCH_NONE;
	}
}

static bool intel_is_virt_pch(unsigned short id,
			      unsigned short svendor, unsigned short sdevice)
{
	return (id == INTEL_PCH_P2X_DEVICE_ID_TYPE ||
		id == INTEL_PCH_P3X_DEVICE_ID_TYPE ||
		(id == INTEL_PCH_QEMU_DEVICE_ID_TYPE &&
		 svendor == PCI_SUBVENDOR_ID_REDHAT_QUMRANET &&
		 sdevice == PCI_SUBDEVICE_ID_QEMU));
}

static unsigned short
intel_virt_detect_pch(const struct drm_i915_private *dev_priv)
{
	unsigned short id = 0;

	/*
	 * In a virtualized passthrough environment we can be in a
	 * setup where the ISA bridge is not able to be passed through.
	 * In this case, a south bridge can be emulated and we have to
	 * make an educated guess as to which PCH is really there.
	 */

	if (IS_GEN5(dev_priv))
		id = INTEL_PCH_IBX_DEVICE_ID_TYPE;
	else if (IS_GEN6(dev_priv) || IS_IVYBRIDGE(dev_priv))
		id = INTEL_PCH_CPT_DEVICE_ID_TYPE;
	else if (IS_HSW_ULT(dev_priv) || IS_BDW_ULT(dev_priv))
		id = INTEL_PCH_LPT_LP_DEVICE_ID_TYPE;
	else if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv))
		id = INTEL_PCH_LPT_DEVICE_ID_TYPE;
	else if (IS_SKYLAKE(dev_priv) || IS_KABYLAKE(dev_priv))
		id = INTEL_PCH_SPT_DEVICE_ID_TYPE;
	else if (IS_COFFEELAKE(dev_priv) || IS_CANNONLAKE(dev_priv))
		id = INTEL_PCH_CNP_DEVICE_ID_TYPE;
	else if (IS_ICELAKE(dev_priv))
		id = INTEL_PCH_ICP_DEVICE_ID_TYPE;

	if (id)
		DRM_DEBUG_KMS("Assuming PCH ID %04x\n", id);
	else
		DRM_DEBUG_KMS("Assuming no PCH\n");

	return id;
}

static void intel_detect_pch(struct drm_i915_private *dev_priv)
{
	struct pci_dev *pch = NULL;

	/*
	 * The reason to probe ISA bridge instead of Dev31:Fun0 is to
	 * make graphics device passthrough work easy for VMM, that only
	 * need to expose ISA bridge to let driver know the real hardware
	 * underneath. This is a requirement from virtualization team.
	 *
	 * In some virtualized environments (e.g. XEN), there is irrelevant
	 * ISA bridge in the system. To work reliably, we should scan trhough
	 * all the ISA bridge devices and check for the first match, instead
	 * of only checking the first one.
	 */
	while ((pch = pci_get_class(PCI_CLASS_BRIDGE_ISA << 8, pch))) {
		unsigned short id;
		enum intel_pch pch_type;

		if (pch->vendor != PCI_VENDOR_ID_INTEL)
			continue;

		id = pch->device & INTEL_PCH_DEVICE_ID_MASK;

		pch_type = intel_pch_type(dev_priv, id);
		if (pch_type != PCH_NONE) {
			dev_priv->pch_type = pch_type;
			dev_priv->pch_id = id;
			break;
		} else if (intel_is_virt_pch(id, pch->subsystem_vendor,
					 pch->subsystem_device)) {
			id = intel_virt_detect_pch(dev_priv);
			pch_type = intel_pch_type(dev_priv, id);

			/* Sanity check virtual PCH id */
			if (WARN_ON(id && pch_type == PCH_NONE))
				id = 0;

			dev_priv->pch_type = pch_type;
			dev_priv->pch_id = id;
			break;
		}
	}

	/*
	 * Use PCH_NOP (PCH but no South Display) for PCH platforms without
	 * display.
	 */
	if (pch && INTEL_INFO(dev_priv)->num_pipes == 0) {
		DRM_DEBUG_KMS("Display disabled, reverting to NOP PCH\n");
		dev_priv->pch_type = PCH_NOP;
		dev_priv->pch_id = 0;
	}

	if (!pch)
		DRM_DEBUG_KMS("No PCH found.\n");

	pci_dev_put(pch);
}

static int i915_getparam_ioctl(struct drm_device *dev, void *data,
			       struct drm_file *file_priv)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct pci_dev *pdev = dev_priv->drm.pdev;
	drm_i915_getparam_t *param = data;
	int value;

	switch (param->param) {
	case I915_PARAM_IRQ_ACTIVE:
	case I915_PARAM_ALLOW_BATCHBUFFER:
	case I915_PARAM_LAST_DISPATCH:
	case I915_PARAM_HAS_EXEC_CONSTANTS:
		/* Reject all old ums/dri params. */
		return -ENODEV;
	case I915_PARAM_CHIPSET_ID:
		value = pdev->device;
		break;
	case I915_PARAM_REVISION:
		value = pdev->revision;
		break;
	case I915_PARAM_NUM_FENCES_AVAIL:
		value = dev_priv->num_fence_regs;
		break;
	case I915_PARAM_HAS_OVERLAY:
		value = dev_priv->overlay ? 1 : 0;
		break;
	case I915_PARAM_HAS_BSD:
		value = !!dev_priv->engine[VCS];
		break;
	case I915_PARAM_HAS_BLT:
		value = !!dev_priv->engine[BCS];
		break;
	case I915_PARAM_HAS_VEBOX:
		value = !!dev_priv->engine[VECS];
		break;
	case I915_PARAM_HAS_BSD2:
		value = !!dev_priv->engine[VCS2];
		break;
	case I915_PARAM_HAS_LLC:
		value = HAS_LLC(dev_priv);
		break;
	case I915_PARAM_HAS_WT:
		value = HAS_WT(dev_priv);
		break;
	case I915_PARAM_HAS_ALIASING_PPGTT:
		value = USES_PPGTT(dev_priv);
		break;
	case I915_PARAM_HAS_SEMAPHORES:
		value = HAS_LEGACY_SEMAPHORES(dev_priv);
		break;
	case I915_PARAM_HAS_SECURE_BATCHES:
		value = HAS_SECURE_BATCHES(dev_priv) && capable(CAP_SYS_ADMIN);
		break;
	case I915_PARAM_CMD_PARSER_VERSION:
		value = i915_cmd_parser_get_version(dev_priv);
		break;
	case I915_PARAM_SUBSLICE_TOTAL:
		value = sseu_subslice_total(&INTEL_INFO(dev_priv)->sseu);
		if (!value)
			return -ENODEV;
		break;
	case I915_PARAM_EU_TOTAL:
		value = INTEL_INFO(dev_priv)->sseu.eu_total;
		if (!value)
			return -ENODEV;
		break;
	case I915_PARAM_HAS_GPU_RESET:
		value = i915_modparams.enable_hangcheck &&
			intel_has_gpu_reset(dev_priv);
		if (value && intel_has_reset_engine(dev_priv))
			value = 2;
		break;
	case I915_PARAM_HAS_RESOURCE_STREAMER:
		value = HAS_RESOURCE_STREAMER(dev_priv);
		break;
	case I915_PARAM_HAS_POOLED_EU:
		value = HAS_POOLED_EU(dev_priv);
		break;
	case I915_PARAM_MIN_EU_IN_POOL:
		value = INTEL_INFO(dev_priv)->sseu.min_eu_in_pool;
		break;
	case I915_PARAM_HUC_STATUS:
		value = intel_huc_check_status(&dev_priv->huc);
		if (value < 0)
			return value;
		break;
	case I915_PARAM_MMAP_GTT_VERSION:
		/* Though we've started our numbering from 1, and so class all
		 * earlier versions as 0, in effect their value is undefined as
		 * the ioctl will report EINVAL for the unknown param!
		 */
		value = i915_gem_mmap_gtt_version();
		break;
	case I915_PARAM_HAS_SCHEDULER:
		value = dev_priv->caps.scheduler;
		break;

	case I915_PARAM_MMAP_VERSION:
		/* Remember to bump this if the version changes! */
	case I915_PARAM_HAS_GEM:
	case I915_PARAM_HAS_PAGEFLIPPING:
	case I915_PARAM_HAS_EXECBUF2: /* depends on GEM */
	case I915_PARAM_HAS_RELAXED_FENCING:
	case I915_PARAM_HAS_COHERENT_RINGS:
	case I915_PARAM_HAS_RELAXED_DELTA:
	case I915_PARAM_HAS_GEN7_SOL_RESET:
	case I915_PARAM_HAS_WAIT_TIMEOUT:
	case I915_PARAM_HAS_PRIME_VMAP_FLUSH:
	case I915_PARAM_HAS_PINNED_BATCHES:
	case I915_PARAM_HAS_EXEC_NO_RELOC:
	case I915_PARAM_HAS_EXEC_HANDLE_LUT:
	case I915_PARAM_HAS_COHERENT_PHYS_GTT:
	case I915_PARAM_HAS_EXEC_SOFTPIN:
	case I915_PARAM_HAS_EXEC_ASYNC:
	case I915_PARAM_HAS_EXEC_FENCE:
	case I915_PARAM_HAS_EXEC_CAPTURE:
	case I915_PARAM_HAS_EXEC_BATCH_FIRST:
	case I915_PARAM_HAS_EXEC_FENCE_ARRAY:
		/* For the time being all of these are always true;
		 * if some supported hardware does not have one of these
		 * features this value needs to be provided from
		 * INTEL_INFO(), a feature macro, or similar.
		 */
		value = 1;
		break;
	case I915_PARAM_HAS_CONTEXT_ISOLATION:
		value = intel_engines_has_context_isolation(dev_priv);
		break;
	case I915_PARAM_SLICE_MASK:
		value = INTEL_INFO(dev_priv)->sseu.slice_mask;
		if (!value)
			return -ENODEV;
		break;
	case I915_PARAM_SUBSLICE_MASK:
		value = INTEL_INFO(dev_priv)->sseu.subslice_mask[0];
		if (!value)
			return -ENODEV;
		break;
	case I915_PARAM_CS_TIMESTAMP_FREQUENCY:
		value = 1000 * INTEL_INFO(dev_priv)->cs_timestamp_frequency_khz;
		break;
	default:
		DRM_DEBUG("Unknown parameter %d\n", param->param);
		return -EINVAL;
	}

	if (put_user(value, param->value))
		return -EFAULT;

	return 0;
}

static int i915_get_bridge_dev(struct drm_i915_private *dev_priv)
{
	int domain = pci_domain_nr(dev_priv->drm.pdev->bus);
=======
#include <linux/vga_switcheroo.h>
#include <linux/vt.h>

#include <drm/drm_aperture.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_ioctl.h>
#include <drm/drm_managed.h>
#include <drm/drm_probe_helper.h>

#include "display/intel_acpi.h"
#include "display/intel_bw.h"
#include "display/intel_cdclk.h"
#include "display/intel_dmc.h"
#include "display/intel_display_types.h"
#include "display/intel_dp.h"
#include "display/intel_fbdev.h"
#include "display/intel_hotplug.h"
#include "display/intel_overlay.h"
#include "display/intel_pipe_crc.h"
#include "display/intel_pps.h"
#include "display/intel_sprite.h"
#include "display/intel_vga.h"

#include "gem/i915_gem_context.h"
#include "gem/i915_gem_ioctls.h"
#include "gem/i915_gem_mman.h"
#include "gem/i915_gem_pm.h"
#include "gt/intel_gt.h"
#include "gt/intel_gt_pm.h"
#include "gt/intel_rc6.h"

#include "i915_debugfs.h"
#include "i915_drv.h"
#include "i915_ioc32.h"
#include "i915_irq.h"
#include "i915_memcpy.h"
#include "i915_perf.h"
#include "i915_query.h"
#include "i915_suspend.h"
#include "i915_switcheroo.h"
#include "i915_sysfs.h"
#include "i915_trace.h"
#include "i915_vgpu.h"
#include "intel_dram.h"
#include "intel_gvt.h"
#include "intel_memory_region.h"
#include "intel_pm.h"
#include "intel_region_ttm.h"
#include "intel_sideband.h"
#include "vlv_suspend.h"

static const struct drm_driver driver;

static int i915_get_bridge_dev(struct drm_i915_private *dev_priv)
{
	int domain = pci_domain_nr(to_pci_dev(dev_priv->drm.dev)->bus);
>>>>>>> upstream/android-13

	dev_priv->bridge_dev =
		pci_get_domain_bus_and_slot(domain, 0, PCI_DEVFN(0, 0));
	if (!dev_priv->bridge_dev) {
<<<<<<< HEAD
		DRM_ERROR("bridge device not found\n");
=======
		drm_err(&dev_priv->drm, "bridge device not found\n");
>>>>>>> upstream/android-13
		return -1;
	}
	return 0;
}

/* Allocate space for the MCH regs if needed, return nonzero on error */
static int
intel_alloc_mchbar_resource(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	int reg = INTEL_GEN(dev_priv) >= 4 ? MCHBAR_I965 : MCHBAR_I915;
=======
	int reg = GRAPHICS_VER(dev_priv) >= 4 ? MCHBAR_I965 : MCHBAR_I915;
>>>>>>> upstream/android-13
	u32 temp_lo, temp_hi = 0;
	u64 mchbar_addr;
	int ret;

<<<<<<< HEAD
	if (INTEL_GEN(dev_priv) >= 4)
=======
	if (GRAPHICS_VER(dev_priv) >= 4)
>>>>>>> upstream/android-13
		pci_read_config_dword(dev_priv->bridge_dev, reg + 4, &temp_hi);
	pci_read_config_dword(dev_priv->bridge_dev, reg, &temp_lo);
	mchbar_addr = ((u64)temp_hi << 32) | temp_lo;

	/* If ACPI doesn't have it, assume we need to allocate it ourselves */
#ifdef CONFIG_PNP
	if (mchbar_addr &&
	    pnp_range_reserved(mchbar_addr, mchbar_addr + MCHBAR_SIZE))
		return 0;
#endif

	/* Get some space for it */
	dev_priv->mch_res.name = "i915 MCHBAR";
	dev_priv->mch_res.flags = IORESOURCE_MEM;
	ret = pci_bus_alloc_resource(dev_priv->bridge_dev->bus,
				     &dev_priv->mch_res,
				     MCHBAR_SIZE, MCHBAR_SIZE,
				     PCIBIOS_MIN_MEM,
				     0, pcibios_align_resource,
				     dev_priv->bridge_dev);
	if (ret) {
<<<<<<< HEAD
		DRM_DEBUG_DRIVER("failed bus alloc: %d\n", ret);
=======
		drm_dbg(&dev_priv->drm, "failed bus alloc: %d\n", ret);
>>>>>>> upstream/android-13
		dev_priv->mch_res.start = 0;
		return ret;
	}

<<<<<<< HEAD
	if (INTEL_GEN(dev_priv) >= 4)
=======
	if (GRAPHICS_VER(dev_priv) >= 4)
>>>>>>> upstream/android-13
		pci_write_config_dword(dev_priv->bridge_dev, reg + 4,
				       upper_32_bits(dev_priv->mch_res.start));

	pci_write_config_dword(dev_priv->bridge_dev, reg,
			       lower_32_bits(dev_priv->mch_res.start));
	return 0;
}

/* Setup MCHBAR if possible, return true if we should disable it again */
static void
intel_setup_mchbar(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	int mchbar_reg = INTEL_GEN(dev_priv) >= 4 ? MCHBAR_I965 : MCHBAR_I915;
=======
	int mchbar_reg = GRAPHICS_VER(dev_priv) >= 4 ? MCHBAR_I965 : MCHBAR_I915;
>>>>>>> upstream/android-13
	u32 temp;
	bool enabled;

	if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv))
		return;

	dev_priv->mchbar_need_disable = false;

	if (IS_I915G(dev_priv) || IS_I915GM(dev_priv)) {
		pci_read_config_dword(dev_priv->bridge_dev, DEVEN, &temp);
		enabled = !!(temp & DEVEN_MCHBAR_EN);
	} else {
		pci_read_config_dword(dev_priv->bridge_dev, mchbar_reg, &temp);
		enabled = temp & 1;
	}

	/* If it's already enabled, don't have to do anything */
	if (enabled)
		return;

	if (intel_alloc_mchbar_resource(dev_priv))
		return;

	dev_priv->mchbar_need_disable = true;

	/* Space is allocated or reserved, so enable it. */
	if (IS_I915G(dev_priv) || IS_I915GM(dev_priv)) {
		pci_write_config_dword(dev_priv->bridge_dev, DEVEN,
				       temp | DEVEN_MCHBAR_EN);
	} else {
		pci_read_config_dword(dev_priv->bridge_dev, mchbar_reg, &temp);
		pci_write_config_dword(dev_priv->bridge_dev, mchbar_reg, temp | 1);
	}
}

static void
intel_teardown_mchbar(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	int mchbar_reg = INTEL_GEN(dev_priv) >= 4 ? MCHBAR_I965 : MCHBAR_I915;
=======
	int mchbar_reg = GRAPHICS_VER(dev_priv) >= 4 ? MCHBAR_I965 : MCHBAR_I915;
>>>>>>> upstream/android-13

	if (dev_priv->mchbar_need_disable) {
		if (IS_I915G(dev_priv) || IS_I915GM(dev_priv)) {
			u32 deven_val;

			pci_read_config_dword(dev_priv->bridge_dev, DEVEN,
					      &deven_val);
			deven_val &= ~DEVEN_MCHBAR_EN;
			pci_write_config_dword(dev_priv->bridge_dev, DEVEN,
					       deven_val);
		} else {
			u32 mchbar_val;

			pci_read_config_dword(dev_priv->bridge_dev, mchbar_reg,
					      &mchbar_val);
			mchbar_val &= ~1;
			pci_write_config_dword(dev_priv->bridge_dev, mchbar_reg,
					       mchbar_val);
		}
	}

	if (dev_priv->mch_res.start)
		release_resource(&dev_priv->mch_res);
}

<<<<<<< HEAD
/* true = enable decode, false = disable decoder */
static unsigned int i915_vga_set_decode(void *cookie, bool state)
{
	struct drm_i915_private *dev_priv = cookie;

	intel_modeset_vga_set_state(dev_priv, state);
	if (state)
		return VGA_RSRC_LEGACY_IO | VGA_RSRC_LEGACY_MEM |
		       VGA_RSRC_NORMAL_IO | VGA_RSRC_NORMAL_MEM;
	else
		return VGA_RSRC_NORMAL_IO | VGA_RSRC_NORMAL_MEM;
}

static int i915_resume_switcheroo(struct drm_device *dev);
static int i915_suspend_switcheroo(struct drm_device *dev, pm_message_t state);

static void i915_switcheroo_set_state(struct pci_dev *pdev, enum vga_switcheroo_state state)
{
	struct drm_device *dev = pci_get_drvdata(pdev);
	pm_message_t pmm = { .event = PM_EVENT_SUSPEND };

	if (state == VGA_SWITCHEROO_ON) {
		pr_info("switched on\n");
		dev->switch_power_state = DRM_SWITCH_POWER_CHANGING;
		/* i915 resume handler doesn't set to D0 */
		pci_set_power_state(pdev, PCI_D0);
		i915_resume_switcheroo(dev);
		dev->switch_power_state = DRM_SWITCH_POWER_ON;
	} else {
		pr_info("switched off\n");
		dev->switch_power_state = DRM_SWITCH_POWER_CHANGING;
		i915_suspend_switcheroo(dev, pmm);
		dev->switch_power_state = DRM_SWITCH_POWER_OFF;
	}
}

static bool i915_switcheroo_can_switch(struct pci_dev *pdev)
{
	struct drm_device *dev = pci_get_drvdata(pdev);

	/*
	 * FIXME: open_count is protected by drm_global_mutex but that would lead to
	 * locking inversion with the driver load path. And the access here is
	 * completely racy anyway. So don't bother with locking for now.
	 */
	return dev->open_count == 0;
}

static const struct vga_switcheroo_client_ops i915_switcheroo_ops = {
	.set_gpu_state = i915_switcheroo_set_state,
	.reprobe = NULL,
	.can_switch = i915_switcheroo_can_switch,
};

static int i915_load_modeset_init(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct pci_dev *pdev = dev_priv->drm.pdev;
	int ret;

	if (i915_inject_load_failure())
		return -ENODEV;

	intel_bios_init(dev_priv);

	/* If we have > 1 VGA cards, then we need to arbitrate access
	 * to the common VGA resources.
	 *
	 * If we are a secondary display controller (!PCI_DISPLAY_CLASS_VGA),
	 * then we do not take part in VGA arbitration and the
	 * vga_client_register() fails with -ENODEV.
	 */
	ret = vga_client_register(pdev, dev_priv, NULL, i915_vga_set_decode);
	if (ret && ret != -ENODEV)
		goto out;

	intel_register_dsm_handler();

	ret = vga_switcheroo_register_client(pdev, &i915_switcheroo_ops, false);
	if (ret)
		goto cleanup_vga_client;

	/* must happen before intel_power_domains_init_hw() on VLV/CHV */
	intel_update_rawclk(dev_priv);

	intel_power_domains_init_hw(dev_priv, false);

	intel_csr_ucode_init(dev_priv);

	ret = intel_irq_install(dev_priv);
	if (ret)
		goto cleanup_csr;

	intel_setup_gmbus(dev_priv);

	/* Important: The output setup functions called by modeset_init need
	 * working irqs for e.g. gmbus and dp aux transfers. */
	ret = intel_modeset_init(dev);
	if (ret)
		goto cleanup_irq;

	ret = i915_gem_init(dev_priv);
	if (ret)
		goto cleanup_modeset;

	intel_setup_overlay(dev_priv);

	if (INTEL_INFO(dev_priv)->num_pipes == 0)
		return 0;

	ret = intel_fbdev_init(dev);
	if (ret)
		goto cleanup_gem;

	/* Only enable hotplug handling once the fbdev is fully set up. */
	intel_hpd_init(dev_priv);

	return 0;

cleanup_gem:
	if (i915_gem_suspend(dev_priv))
		DRM_ERROR("failed to idle hardware; continuing to unload!\n");
	i915_gem_fini(dev_priv);
cleanup_modeset:
	intel_modeset_cleanup(dev);
cleanup_irq:
	drm_irq_uninstall(dev);
	intel_teardown_gmbus(dev_priv);
cleanup_csr:
	intel_csr_ucode_fini(dev_priv);
	intel_power_domains_fini(dev_priv);
	vga_switcheroo_unregister_client(pdev);
cleanup_vga_client:
	vga_client_register(pdev, NULL, NULL, NULL);
out:
	return ret;
}

static int i915_kick_out_firmware_fb(struct drm_i915_private *dev_priv)
{
	struct apertures_struct *ap;
	struct pci_dev *pdev = dev_priv->drm.pdev;
	struct i915_ggtt *ggtt = &dev_priv->ggtt;
	bool primary;
	int ret;

	ap = alloc_apertures(1);
	if (!ap)
		return -ENOMEM;

	ap->ranges[0].base = ggtt->gmadr.start;
	ap->ranges[0].size = ggtt->mappable_end;

	primary =
		pdev->resource[PCI_ROM_RESOURCE].flags & IORESOURCE_ROM_SHADOW;

	ret = drm_fb_helper_remove_conflicting_framebuffers(ap, "inteldrmfb", primary);

	kfree(ap);

	return ret;
}

#if !defined(CONFIG_VGA_CONSOLE)
static int i915_kick_out_vgacon(struct drm_i915_private *dev_priv)
{
	return 0;
}
#elif !defined(CONFIG_DUMMY_CONSOLE)
static int i915_kick_out_vgacon(struct drm_i915_private *dev_priv)
{
	return -ENODEV;
}
#else
static int i915_kick_out_vgacon(struct drm_i915_private *dev_priv)
{
	int ret = 0;

	DRM_INFO("Replacing VGA console driver\n");

	console_lock();
	if (con_is_bound(&vga_con))
		ret = do_take_over_console(&dummy_con, 0, MAX_NR_CONSOLES - 1, 1);
	if (ret == 0) {
		ret = do_unregister_con_driver(&vga_con);

		/* Ignore "already unregistered". */
		if (ret == -ENODEV)
			ret = 0;
	}
	console_unlock();

	return ret;
}
#endif

static void intel_init_dpio(struct drm_i915_private *dev_priv)
{
	/*
	 * IOSF_PORT_DPIO is used for VLV x2 PHY (DP/HDMI B and C),
	 * CHV x1 PHY (DP/HDMI D)
	 * IOSF_PORT_DPIO_2 is used for CHV x2 PHY (DP/HDMI B and C)
	 */
	if (IS_CHERRYVIEW(dev_priv)) {
		DPIO_PHY_IOSF_PORT(DPIO_PHY0) = IOSF_PORT_DPIO_2;
		DPIO_PHY_IOSF_PORT(DPIO_PHY1) = IOSF_PORT_DPIO;
	} else if (IS_VALLEYVIEW(dev_priv)) {
		DPIO_PHY_IOSF_PORT(DPIO_PHY0) = IOSF_PORT_DPIO;
	}
}

=======
>>>>>>> upstream/android-13
static int i915_workqueues_init(struct drm_i915_private *dev_priv)
{
	/*
	 * The i915 workqueue is primarily used for batched retirement of
	 * requests (and thus managing bo) once the task has been completed
	 * by the GPU. i915_retire_requests() is called directly when we
	 * need high-priority retirement, such as waiting for an explicit
	 * bo.
	 *
	 * It is also used for periodic low-priority events, such as
	 * idle-timers and recording error state.
	 *
	 * All tasks on the workqueue are expected to acquire the dev mutex
	 * so there is no point in running more than one instance of the
	 * workqueue at any time.  Use an ordered one.
	 */
	dev_priv->wq = alloc_ordered_workqueue("i915", 0);
	if (dev_priv->wq == NULL)
		goto out_err;

	dev_priv->hotplug.dp_wq = alloc_ordered_workqueue("i915-dp", 0);
	if (dev_priv->hotplug.dp_wq == NULL)
		goto out_free_wq;

	return 0;

out_free_wq:
	destroy_workqueue(dev_priv->wq);
out_err:
<<<<<<< HEAD
	DRM_ERROR("Failed to allocate workqueues.\n");
=======
	drm_err(&dev_priv->drm, "Failed to allocate workqueues.\n");
>>>>>>> upstream/android-13

	return -ENOMEM;
}

<<<<<<< HEAD
static void i915_engines_cleanup(struct drm_i915_private *i915)
{
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	for_each_engine(engine, i915, id)
		kfree(engine);
}

=======
>>>>>>> upstream/android-13
static void i915_workqueues_cleanup(struct drm_i915_private *dev_priv)
{
	destroy_workqueue(dev_priv->hotplug.dp_wq);
	destroy_workqueue(dev_priv->wq);
}

/*
 * We don't keep the workarounds for pre-production hardware, so we expect our
 * driver to fail on these machines in one way or another. A little warning on
 * dmesg may help both the user and the bug triagers.
 *
 * Our policy for removing pre-production workarounds is to keep the
 * current gen workarounds as a guide to the bring-up of the next gen
 * (workarounds have a habit of persisting!). Anything older than that
 * should be removed along with the complications they introduce.
 */
static void intel_detect_preproduction_hw(struct drm_i915_private *dev_priv)
{
	bool pre = false;

	pre |= IS_HSW_EARLY_SDV(dev_priv);
<<<<<<< HEAD
	pre |= IS_SKL_REVID(dev_priv, 0, SKL_REVID_F0);
	pre |= IS_BXT_REVID(dev_priv, 0, BXT_REVID_B_LAST);

	if (pre) {
		DRM_ERROR("This is a pre-production stepping. "
=======
	pre |= IS_SKYLAKE(dev_priv) && INTEL_REVID(dev_priv) < 0x6;
	pre |= IS_BROXTON(dev_priv) && INTEL_REVID(dev_priv) < 0xA;
	pre |= IS_KABYLAKE(dev_priv) && INTEL_REVID(dev_priv) < 0x1;
	pre |= IS_GEMINILAKE(dev_priv) && INTEL_REVID(dev_priv) < 0x3;
	pre |= IS_ICELAKE(dev_priv) && INTEL_REVID(dev_priv) < 0x7;

	if (pre) {
		drm_err(&dev_priv->drm, "This is a pre-production stepping. "
>>>>>>> upstream/android-13
			  "It may not be fully functional.\n");
		add_taint(TAINT_MACHINE_CHECK, LOCKDEP_STILL_OK);
	}
}

<<<<<<< HEAD
/**
 * i915_driver_init_early - setup state not requiring device access
 * @dev_priv: device private
 * @ent: the matching pci_device_id
=======
static void sanitize_gpu(struct drm_i915_private *i915)
{
	if (!INTEL_INFO(i915)->gpu_reset_clobbers_display)
		__intel_gt_reset(&i915->gt, ALL_ENGINES);
}

/**
 * i915_driver_early_probe - setup state not requiring device access
 * @dev_priv: device private
>>>>>>> upstream/android-13
 *
 * Initialize everything that is a "SW-only" state, that is state not
 * requiring accessing the device or exposing the driver via kernel internal
 * or userspace interfaces. Example steps belonging here: lock initialization,
 * system memory allocation, setting up device specific attributes and
 * function hooks not requiring accessing the device.
 */
<<<<<<< HEAD
static int i915_driver_init_early(struct drm_i915_private *dev_priv,
				  const struct pci_device_id *ent)
{
	const struct intel_device_info *match_info =
		(struct intel_device_info *)ent->driver_data;
	struct intel_device_info *device_info;
	int ret = 0;

	if (i915_inject_load_failure())
		return -ENODEV;

	/* Setup the write-once "constant" device info */
	device_info = mkwrite_device_info(dev_priv);
	memcpy(device_info, match_info, sizeof(*device_info));
	device_info->device_id = dev_priv->drm.pdev->device;

	BUILD_BUG_ON(INTEL_MAX_PLATFORMS >
		     sizeof(device_info->platform_mask) * BITS_PER_BYTE);
	BUG_ON(device_info->gen > sizeof(device_info->gen_mask) * BITS_PER_BYTE);
	spin_lock_init(&dev_priv->irq_lock);
	spin_lock_init(&dev_priv->gpu_error.lock);
	mutex_init(&dev_priv->backlight_lock);
	spin_lock_init(&dev_priv->uncore.lock);

	mutex_init(&dev_priv->sb_lock);
	mutex_init(&dev_priv->av_mutex);
	mutex_init(&dev_priv->wm.wm_mutex);
	mutex_init(&dev_priv->pps_mutex);

	i915_memcpy_init_early(dev_priv);

	ret = i915_workqueues_init(dev_priv);
	if (ret < 0)
		goto err_engines;

	ret = i915_gem_init_early(dev_priv);
	if (ret < 0)
		goto err_workqueues;

	/* This must be called before any calls to HAS_PCH_* */
	intel_detect_pch(dev_priv);

	intel_wopcm_init_early(&dev_priv->wopcm);
	intel_uc_init_early(dev_priv);
	intel_pm_setup(dev_priv);
	intel_init_dpio(dev_priv);
	intel_power_domains_init(dev_priv);
	intel_irq_init(dev_priv);
	intel_hangcheck_init(dev_priv);
	intel_init_display_hooks(dev_priv);
	intel_init_clock_gating_hooks(dev_priv);
	intel_init_audio_hooks(dev_priv);
	intel_display_crc_init(dev_priv);
=======
static int i915_driver_early_probe(struct drm_i915_private *dev_priv)
{
	int ret = 0;

	if (i915_inject_probe_failure(dev_priv))
		return -ENODEV;

	intel_device_info_subplatform_init(dev_priv);
	intel_step_init(dev_priv);

	intel_uncore_mmio_debug_init_early(&dev_priv->mmio_debug);
	intel_uncore_init_early(&dev_priv->uncore, dev_priv);

	spin_lock_init(&dev_priv->irq_lock);
	spin_lock_init(&dev_priv->gpu_error.lock);
	mutex_init(&dev_priv->backlight_lock);

	mutex_init(&dev_priv->sb_lock);
	cpu_latency_qos_add_request(&dev_priv->sb_qos, PM_QOS_DEFAULT_VALUE);

	mutex_init(&dev_priv->av_mutex);
	mutex_init(&dev_priv->wm.wm_mutex);
	mutex_init(&dev_priv->pps_mutex);
	mutex_init(&dev_priv->hdcp_comp_mutex);

	i915_memcpy_init_early(dev_priv);
	intel_runtime_pm_init_early(&dev_priv->runtime_pm);

	ret = i915_workqueues_init(dev_priv);
	if (ret < 0)
		return ret;

	ret = vlv_suspend_init(dev_priv);
	if (ret < 0)
		goto err_workqueues;

	ret = intel_region_ttm_device_init(dev_priv);
	if (ret)
		goto err_ttm;

	intel_wopcm_init_early(&dev_priv->wopcm);

	intel_gt_init_early(&dev_priv->gt, dev_priv);

	i915_gem_init_early(dev_priv);

	/* This must be called before any calls to HAS_PCH_* */
	intel_detect_pch(dev_priv);

	intel_pm_setup(dev_priv);
	ret = intel_power_domains_init(dev_priv);
	if (ret < 0)
		goto err_gem;
	intel_irq_init(dev_priv);
	intel_init_display_hooks(dev_priv);
	intel_init_clock_gating_hooks(dev_priv);
>>>>>>> upstream/android-13

	intel_detect_preproduction_hw(dev_priv);

	return 0;

<<<<<<< HEAD
err_workqueues:
	i915_workqueues_cleanup(dev_priv);
err_engines:
	i915_engines_cleanup(dev_priv);
=======
err_gem:
	i915_gem_cleanup_early(dev_priv);
	intel_gt_driver_late_release(&dev_priv->gt);
	intel_region_ttm_device_fini(dev_priv);
err_ttm:
	vlv_suspend_cleanup(dev_priv);
err_workqueues:
	i915_workqueues_cleanup(dev_priv);
>>>>>>> upstream/android-13
	return ret;
}

/**
<<<<<<< HEAD
 * i915_driver_cleanup_early - cleanup the setup done in i915_driver_init_early()
 * @dev_priv: device private
 */
static void i915_driver_cleanup_early(struct drm_i915_private *dev_priv)
{
	intel_irq_fini(dev_priv);
	intel_uc_cleanup_early(dev_priv);
	i915_gem_cleanup_early(dev_priv);
	i915_workqueues_cleanup(dev_priv);
	i915_engines_cleanup(dev_priv);
}

static int i915_mmio_setup(struct drm_i915_private *dev_priv)
{
	struct pci_dev *pdev = dev_priv->drm.pdev;
	int mmio_bar;
	int mmio_size;

	mmio_bar = IS_GEN2(dev_priv) ? 1 : 0;
	/*
	 * Before gen4, the registers and the GTT are behind different BARs.
	 * However, from gen4 onwards, the registers and the GTT are shared
	 * in the same BAR, so we want to restrict this ioremap from
	 * clobbering the GTT which we want ioremap_wc instead. Fortunately,
	 * the register BAR remains the same size for all the earlier
	 * generations up to Ironlake.
	 */
	if (INTEL_GEN(dev_priv) < 5)
		mmio_size = 512 * 1024;
	else
		mmio_size = 2 * 1024 * 1024;
	dev_priv->regs = pci_iomap(pdev, mmio_bar, mmio_size);
	if (dev_priv->regs == NULL) {
		DRM_ERROR("failed to map registers\n");

		return -EIO;
	}

	/* Try to make sure MCHBAR is enabled before poking at it */
	intel_setup_mchbar(dev_priv);

	return 0;
}

static void i915_mmio_cleanup(struct drm_i915_private *dev_priv)
{
	struct pci_dev *pdev = dev_priv->drm.pdev;

	intel_teardown_mchbar(dev_priv);
	pci_iounmap(pdev, dev_priv->regs);
}

/**
 * i915_driver_init_mmio - setup device MMIO
=======
 * i915_driver_late_release - cleanup the setup done in
 *			       i915_driver_early_probe()
 * @dev_priv: device private
 */
static void i915_driver_late_release(struct drm_i915_private *dev_priv)
{
	intel_irq_fini(dev_priv);
	intel_power_domains_cleanup(dev_priv);
	i915_gem_cleanup_early(dev_priv);
	intel_gt_driver_late_release(&dev_priv->gt);
	intel_region_ttm_device_fini(dev_priv);
	vlv_suspend_cleanup(dev_priv);
	i915_workqueues_cleanup(dev_priv);

	cpu_latency_qos_remove_request(&dev_priv->sb_qos);
	mutex_destroy(&dev_priv->sb_lock);

	i915_params_free(&dev_priv->params);
}

/**
 * i915_driver_mmio_probe - setup device MMIO
>>>>>>> upstream/android-13
 * @dev_priv: device private
 *
 * Setup minimal device state necessary for MMIO accesses later in the
 * initialization sequence. The setup here should avoid any other device-wide
 * side effects or exposing the driver via kernel internal or user space
 * interfaces.
 */
<<<<<<< HEAD
static int i915_driver_init_mmio(struct drm_i915_private *dev_priv)
{
	int ret;

	if (i915_inject_load_failure())
=======
static int i915_driver_mmio_probe(struct drm_i915_private *dev_priv)
{
	int ret;

	if (i915_inject_probe_failure(dev_priv))
>>>>>>> upstream/android-13
		return -ENODEV;

	if (i915_get_bridge_dev(dev_priv))
		return -EIO;

<<<<<<< HEAD
	ret = i915_mmio_setup(dev_priv);
	if (ret < 0)
		goto err_bridge;

	intel_uncore_init(dev_priv);

	intel_device_info_init_mmio(dev_priv);

	intel_uncore_prune(dev_priv);

	intel_uc_init_mmio(dev_priv);

	ret = intel_engines_init_mmio(dev_priv);
	if (ret)
		goto err_uncore;

	i915_gem_init_mmio(dev_priv);
=======
	ret = intel_uncore_init_mmio(&dev_priv->uncore);
	if (ret < 0)
		goto err_bridge;

	/* Try to make sure MCHBAR is enabled before poking at it */
	intel_setup_mchbar(dev_priv);
	intel_device_info_runtime_init(dev_priv);

	ret = intel_gt_init_mmio(&dev_priv->gt);
	if (ret)
		goto err_uncore;

	/* As early as possible, scrub existing GPU state before clobbering */
	sanitize_gpu(dev_priv);
>>>>>>> upstream/android-13

	return 0;

err_uncore:
<<<<<<< HEAD
	intel_uncore_fini(dev_priv);
=======
	intel_teardown_mchbar(dev_priv);
	intel_uncore_fini_mmio(&dev_priv->uncore);
>>>>>>> upstream/android-13
err_bridge:
	pci_dev_put(dev_priv->bridge_dev);

	return ret;
}

/**
<<<<<<< HEAD
 * i915_driver_cleanup_mmio - cleanup the setup done in i915_driver_init_mmio()
 * @dev_priv: device private
 */
static void i915_driver_cleanup_mmio(struct drm_i915_private *dev_priv)
{
	intel_uncore_fini(dev_priv);
	i915_mmio_cleanup(dev_priv);
=======
 * i915_driver_mmio_release - cleanup the setup done in i915_driver_mmio_probe()
 * @dev_priv: device private
 */
static void i915_driver_mmio_release(struct drm_i915_private *dev_priv)
{
	intel_teardown_mchbar(dev_priv);
	intel_uncore_fini_mmio(&dev_priv->uncore);
>>>>>>> upstream/android-13
	pci_dev_put(dev_priv->bridge_dev);
}

static void intel_sanitize_options(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	/*
	 * i915.enable_ppgtt is read-only, so do an early pass to validate the
	 * user's requested state against the hardware/driver capabilities.  We
	 * do this now so that we can print out any log messages once rather
	 * than every time we check intel_enable_ppgtt().
	 */
	i915_modparams.enable_ppgtt =
		intel_sanitize_enable_ppgtt(dev_priv,
					    i915_modparams.enable_ppgtt);
	DRM_DEBUG_DRIVER("ppgtt mode: %i\n", i915_modparams.enable_ppgtt);

=======
>>>>>>> upstream/android-13
	intel_gvt_sanitize_options(dev_priv);
}

/**
<<<<<<< HEAD
 * i915_driver_init_hw - setup state requiring device access
 * @dev_priv: device private
 *
 * Setup state that requires accessing the device, but doesn't require
 * exposing the driver via kernel internal or userspace interfaces.
 */
static int i915_driver_init_hw(struct drm_i915_private *dev_priv)
{
	struct pci_dev *pdev = dev_priv->drm.pdev;
	int ret;

	if (i915_inject_load_failure())
		return -ENODEV;

	intel_device_info_runtime_init(mkwrite_device_info(dev_priv));

	intel_sanitize_options(dev_priv);

	i915_perf_init(dev_priv);

	ret = i915_ggtt_probe_hw(dev_priv);
	if (ret)
		goto err_perf;

	/*
	 * WARNING: Apparently we must kick fbdev drivers before vgacon,
	 * otherwise the vga fbdev driver falls over.
	 */
	ret = i915_kick_out_firmware_fb(dev_priv);
	if (ret) {
		DRM_ERROR("failed to remove conflicting framebuffer drivers\n");
		goto err_ggtt;
	}

	ret = i915_kick_out_vgacon(dev_priv);
	if (ret) {
		DRM_ERROR("failed to remove conflicting VGA console\n");
		goto err_ggtt;
	}

	ret = i915_ggtt_init_hw(dev_priv);
	if (ret)
		goto err_ggtt;

	ret = i915_ggtt_enable_hw(dev_priv);
	if (ret) {
		DRM_ERROR("failed to enable GGTT\n");
		goto err_ggtt;
	}

	pci_set_master(pdev);
=======
 * i915_set_dma_info - set all relevant PCI dma info as configured for the
 * platform
 * @i915: valid i915 instance
 *
 * Set the dma max segment size, device and coherent masks.  The dma mask set
 * needs to occur before i915_ggtt_probe_hw.
 *
 * A couple of platforms have special needs.  Address them as well.
 *
 */
static int i915_set_dma_info(struct drm_i915_private *i915)
{
	unsigned int mask_size = INTEL_INFO(i915)->dma_mask_size;
	int ret;

	GEM_BUG_ON(!mask_size);
>>>>>>> upstream/android-13

	/*
	 * We don't have a max segment size, so set it to the max so sg's
	 * debugging layer doesn't complain
	 */
<<<<<<< HEAD
	dma_set_max_seg_size(&pdev->dev, UINT_MAX);

	/* overlay on gen2 is broken and can't address above 1G */
	if (IS_GEN2(dev_priv)) {
		ret = dma_set_coherent_mask(&pdev->dev, DMA_BIT_MASK(30));
		if (ret) {
			DRM_ERROR("failed to set DMA mask\n");

			goto err_ggtt;
		}
	}

	/* 965GM sometimes incorrectly writes to hardware status page (HWS)
=======
	dma_set_max_seg_size(i915->drm.dev, UINT_MAX);

	ret = dma_set_mask(i915->drm.dev, DMA_BIT_MASK(mask_size));
	if (ret)
		goto mask_err;

	/* overlay on gen2 is broken and can't address above 1G */
	if (GRAPHICS_VER(i915) == 2)
		mask_size = 30;

	/*
	 * 965GM sometimes incorrectly writes to hardware status page (HWS)
>>>>>>> upstream/android-13
	 * using 32bit addressing, overwriting memory if HWS is located
	 * above 4GB.
	 *
	 * The documentation also mentions an issue with undefined
	 * behaviour if any general state is accessed within a page above 4GB,
	 * which also needs to be handled carefully.
	 */
<<<<<<< HEAD
	if (IS_I965G(dev_priv) || IS_I965GM(dev_priv)) {
		ret = dma_set_coherent_mask(&pdev->dev, DMA_BIT_MASK(32));

		if (ret) {
			DRM_ERROR("failed to set DMA mask\n");

			goto err_ggtt;
		}
	}

	pm_qos_add_request(&dev_priv->pm_qos, PM_QOS_CPU_DMA_LATENCY,
			   PM_QOS_DEFAULT_VALUE);

	intel_uncore_sanitize(dev_priv);

	i915_gem_load_init_fences(dev_priv);
=======
	if (IS_I965G(i915) || IS_I965GM(i915))
		mask_size = 32;

	ret = dma_set_coherent_mask(i915->drm.dev, DMA_BIT_MASK(mask_size));
	if (ret)
		goto mask_err;

	return 0;

mask_err:
	drm_err(&i915->drm, "Can't set DMA mask/consistent mask (%d)\n", ret);
	return ret;
}

/**
 * i915_driver_hw_probe - setup state requiring device access
 * @dev_priv: device private
 *
 * Setup state that requires accessing the device, but doesn't require
 * exposing the driver via kernel internal or userspace interfaces.
 */
static int i915_driver_hw_probe(struct drm_i915_private *dev_priv)
{
	struct pci_dev *pdev = to_pci_dev(dev_priv->drm.dev);
	int ret;

	if (i915_inject_probe_failure(dev_priv))
		return -ENODEV;

	if (HAS_PPGTT(dev_priv)) {
		if (intel_vgpu_active(dev_priv) &&
		    !intel_vgpu_has_full_ppgtt(dev_priv)) {
			i915_report_error(dev_priv,
					  "incompatible vGPU found, support for isolated ppGTT required\n");
			return -ENXIO;
		}
	}

	if (HAS_EXECLISTS(dev_priv)) {
		/*
		 * Older GVT emulation depends upon intercepting CSB mmio,
		 * which we no longer use, preferring to use the HWSP cache
		 * instead.
		 */
		if (intel_vgpu_active(dev_priv) &&
		    !intel_vgpu_has_hwsp_emulation(dev_priv)) {
			i915_report_error(dev_priv,
					  "old vGPU host found, support for HWSP emulation required\n");
			return -ENXIO;
		}
	}

	intel_sanitize_options(dev_priv);

	/* needs to be done before ggtt probe */
	intel_dram_edram_detect(dev_priv);

	ret = i915_set_dma_info(dev_priv);
	if (ret)
		return ret;

	i915_perf_init(dev_priv);

	ret = i915_ggtt_probe_hw(dev_priv);
	if (ret)
		goto err_perf;

	ret = drm_aperture_remove_conflicting_pci_framebuffers(pdev, dev_priv->drm.driver);
	if (ret)
		goto err_ggtt;

	ret = i915_ggtt_init_hw(dev_priv);
	if (ret)
		goto err_ggtt;

	ret = intel_memory_regions_hw_probe(dev_priv);
	if (ret)
		goto err_ggtt;

	intel_gt_init_hw_early(&dev_priv->gt, &dev_priv->ggtt);

	ret = intel_gt_probe_lmem(&dev_priv->gt);
	if (ret)
		goto err_mem_regions;

	ret = i915_ggtt_enable_hw(dev_priv);
	if (ret) {
		drm_err(&dev_priv->drm, "failed to enable GGTT\n");
		goto err_mem_regions;
	}

	pci_set_master(pdev);

	intel_gt_init_workarounds(dev_priv);
>>>>>>> upstream/android-13

	/* On the 945G/GM, the chipset reports the MSI capability on the
	 * integrated graphics even though the support isn't actually there
	 * according to the published specs.  It doesn't appear to function
	 * correctly in testing on 945G.
	 * This may be a side effect of MSI having been made available for PEG
	 * and the registers being closely associated.
	 *
	 * According to chipset errata, on the 965GM, MSI interrupts may
	 * be lost or delayed, and was defeatured. MSI interrupts seem to
	 * get lost on g4x as well, and interrupt delivery seems to stay
	 * properly dead afterwards. So we'll just disable them for all
	 * pre-gen5 chipsets.
	 *
	 * dp aux and gmbus irq on gen4 seems to be able to generate legacy
	 * interrupts even when in MSI mode. This results in spurious
	 * interrupt warnings if the legacy irq no. is shared with another
	 * device. The kernel then disables that interrupt source and so
	 * prevents the other device from working properly.
	 */
<<<<<<< HEAD
	if (INTEL_GEN(dev_priv) >= 5) {
		if (pci_enable_msi(pdev) < 0)
			DRM_DEBUG_DRIVER("can't enable MSI");
=======
	if (GRAPHICS_VER(dev_priv) >= 5) {
		if (pci_enable_msi(pdev) < 0)
			drm_dbg(&dev_priv->drm, "can't enable MSI");
>>>>>>> upstream/android-13
	}

	ret = intel_gvt_init(dev_priv);
	if (ret)
		goto err_msi;

	intel_opregion_setup(dev_priv);

<<<<<<< HEAD
=======
	ret = intel_pcode_init(dev_priv);
	if (ret)
		goto err_msi;

	/*
	 * Fill the dram structure to get the system dram info. This will be
	 * used for memory latency calculation.
	 */
	intel_dram_detect(dev_priv);

	intel_bw_init_hw(dev_priv);

>>>>>>> upstream/android-13
	return 0;

err_msi:
	if (pdev->msi_enabled)
		pci_disable_msi(pdev);
<<<<<<< HEAD
	pm_qos_remove_request(&dev_priv->pm_qos);
err_ggtt:
	i915_ggtt_cleanup_hw(dev_priv);
=======
err_mem_regions:
	intel_memory_regions_driver_release(dev_priv);
err_ggtt:
	i915_ggtt_driver_release(dev_priv);
	i915_gem_drain_freed_objects(dev_priv);
	i915_ggtt_driver_late_release(dev_priv);
>>>>>>> upstream/android-13
err_perf:
	i915_perf_fini(dev_priv);
	return ret;
}

/**
<<<<<<< HEAD
 * i915_driver_cleanup_hw - cleanup the setup done in i915_driver_init_hw()
 * @dev_priv: device private
 */
static void i915_driver_cleanup_hw(struct drm_i915_private *dev_priv)
{
	struct pci_dev *pdev = dev_priv->drm.pdev;
=======
 * i915_driver_hw_remove - cleanup the setup done in i915_driver_hw_probe()
 * @dev_priv: device private
 */
static void i915_driver_hw_remove(struct drm_i915_private *dev_priv)
{
	struct pci_dev *pdev = to_pci_dev(dev_priv->drm.dev);
>>>>>>> upstream/android-13

	i915_perf_fini(dev_priv);

	if (pdev->msi_enabled)
		pci_disable_msi(pdev);
<<<<<<< HEAD

	pm_qos_remove_request(&dev_priv->pm_qos);
	i915_ggtt_cleanup_hw(dev_priv);
=======
>>>>>>> upstream/android-13
}

/**
 * i915_driver_register - register the driver with the rest of the system
 * @dev_priv: device private
 *
 * Perform any steps necessary to make the driver available via kernel
 * internal or userspace interfaces.
 */
static void i915_driver_register(struct drm_i915_private *dev_priv)
{
	struct drm_device *dev = &dev_priv->drm;

<<<<<<< HEAD
	i915_gem_shrinker_register(dev_priv);
	i915_pmu_register(dev_priv);

	/*
	 * Notify a valid surface after modesetting,
	 * when running inside a VM.
	 */
	if (intel_vgpu_active(dev_priv))
		I915_WRITE(vgtif_reg(display_ready), VGT_DRV_DISPLAY_READY);

	/* Reveal our presence to userspace */
	if (drm_dev_register(dev, 0) == 0) {
		i915_debugfs_register(dev_priv);
		i915_setup_sysfs(dev_priv);

		/* Depends on sysfs having been initialized */
		i915_perf_register(dev_priv);
	} else
		DRM_ERROR("Failed to register driver for userspace access!\n");

	if (INTEL_INFO(dev_priv)->num_pipes) {
		/* Must be done after probing outputs */
		intel_opregion_register(dev_priv);
		acpi_video_register();
	}

	if (IS_GEN5(dev_priv))
		intel_gpu_ips_init(dev_priv);

	intel_audio_init(dev_priv);

	/*
	 * Some ports require correctly set-up hpd registers for detection to
	 * work properly (leading to ghost connected connector status), e.g. VGA
	 * on gm45.  Hence we can only set up the initial fbdev config after hpd
	 * irqs are fully enabled. We do it last so that the async config
	 * cannot run before the connectors are registered.
	 */
	intel_fbdev_initial_config_async(dev);

	/*
	 * We need to coordinate the hotplugs with the asynchronous fbdev
	 * configuration, for which we use the fbdev->async_cookie.
	 */
	if (INTEL_INFO(dev_priv)->num_pipes)
		drm_kms_helper_poll_init(dev);
=======
	i915_gem_driver_register(dev_priv);
	i915_pmu_register(dev_priv);

	intel_vgpu_register(dev_priv);

	/* Reveal our presence to userspace */
	if (drm_dev_register(dev, 0)) {
		drm_err(&dev_priv->drm,
			"Failed to register driver for userspace access!\n");
		return;
	}

	i915_debugfs_register(dev_priv);
	i915_setup_sysfs(dev_priv);

	/* Depends on sysfs having been initialized */
	i915_perf_register(dev_priv);

	intel_gt_driver_register(&dev_priv->gt);

	intel_display_driver_register(dev_priv);

	intel_power_domains_enable(dev_priv);
	intel_runtime_pm_enable(&dev_priv->runtime_pm);

	intel_register_dsm_handler();

	if (i915_switcheroo_register(dev_priv))
		drm_err(&dev_priv->drm, "Failed to register vga switcheroo!\n");
>>>>>>> upstream/android-13
}

/**
 * i915_driver_unregister - cleanup the registration done in i915_driver_regiser()
 * @dev_priv: device private
 */
static void i915_driver_unregister(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	intel_fbdev_unregister(dev_priv);
	intel_audio_deinit(dev_priv);

	/*
	 * After flushing the fbdev (incl. a late async config which will
	 * have delayed queuing of a hotplug event), then flush the hotplug
	 * events.
	 */
	drm_kms_helper_poll_fini(&dev_priv->drm);

	intel_gpu_ips_teardown();
	acpi_video_unregister();
	intel_opregion_unregister(dev_priv);
=======
	i915_switcheroo_unregister(dev_priv);

	intel_unregister_dsm_handler();

	intel_runtime_pm_disable(&dev_priv->runtime_pm);
	intel_power_domains_disable(dev_priv);

	intel_display_driver_unregister(dev_priv);

	intel_gt_driver_unregister(&dev_priv->gt);
>>>>>>> upstream/android-13

	i915_perf_unregister(dev_priv);
	i915_pmu_unregister(dev_priv);

	i915_teardown_sysfs(dev_priv);
<<<<<<< HEAD
	drm_dev_unregister(&dev_priv->drm);

	i915_gem_shrinker_unregister(dev_priv);
=======
	drm_dev_unplug(&dev_priv->drm);

	i915_gem_driver_unregister(dev_priv);
>>>>>>> upstream/android-13
}

static void i915_welcome_messages(struct drm_i915_private *dev_priv)
{
<<<<<<< HEAD
	if (drm_debug & DRM_UT_DRIVER) {
		struct drm_printer p = drm_debug_printer("i915 device info:");

		intel_device_info_dump(&dev_priv->info, &p);
		intel_device_info_dump_runtime(&dev_priv->info, &p);
	}

	if (IS_ENABLED(CONFIG_DRM_I915_DEBUG))
		DRM_INFO("DRM_I915_DEBUG enabled\n");
	if (IS_ENABLED(CONFIG_DRM_I915_DEBUG_GEM))
		DRM_INFO("DRM_I915_DEBUG_GEM enabled\n");
}

/**
 * i915_driver_load - setup chip and create an initial config
 * @pdev: PCI device
 * @ent: matching PCI ID entry
 *
 * The driver load routine has to do several things:
=======
	if (drm_debug_enabled(DRM_UT_DRIVER)) {
		struct drm_printer p = drm_debug_printer("i915 device info:");

		drm_printf(&p, "pciid=0x%04x rev=0x%02x platform=%s (subplatform=0x%x) gen=%i\n",
			   INTEL_DEVID(dev_priv),
			   INTEL_REVID(dev_priv),
			   intel_platform_name(INTEL_INFO(dev_priv)->platform),
			   intel_subplatform(RUNTIME_INFO(dev_priv),
					     INTEL_INFO(dev_priv)->platform),
			   GRAPHICS_VER(dev_priv));

		intel_device_info_print_static(INTEL_INFO(dev_priv), &p);
		intel_device_info_print_runtime(RUNTIME_INFO(dev_priv), &p);
		intel_gt_info_print(&dev_priv->gt.info, &p);
	}

	if (IS_ENABLED(CONFIG_DRM_I915_DEBUG))
		drm_info(&dev_priv->drm, "DRM_I915_DEBUG enabled\n");
	if (IS_ENABLED(CONFIG_DRM_I915_DEBUG_GEM))
		drm_info(&dev_priv->drm, "DRM_I915_DEBUG_GEM enabled\n");
	if (IS_ENABLED(CONFIG_DRM_I915_DEBUG_RUNTIME_PM))
		drm_info(&dev_priv->drm,
			 "DRM_I915_DEBUG_RUNTIME_PM enabled\n");
}

static struct drm_i915_private *
i915_driver_create(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	const struct intel_device_info *match_info =
		(struct intel_device_info *)ent->driver_data;
	struct intel_device_info *device_info;
	struct drm_i915_private *i915;

	i915 = devm_drm_dev_alloc(&pdev->dev, &driver,
				  struct drm_i915_private, drm);
	if (IS_ERR(i915))
		return i915;

	pci_set_drvdata(pdev, i915);

	/* Device parameters start as a copy of module parameters. */
	i915_params_copy(&i915->params, &i915_modparams);

	/* Setup the write-once "constant" device info */
	device_info = mkwrite_device_info(i915);
	memcpy(device_info, match_info, sizeof(*device_info));
	RUNTIME_INFO(i915)->device_id = pdev->device;

	return i915;
}

/**
 * i915_driver_probe - setup chip and create an initial config
 * @pdev: PCI device
 * @ent: matching PCI ID entry
 *
 * The driver probe routine has to do several things:
>>>>>>> upstream/android-13
 *   - drive output discovery via intel_modeset_init()
 *   - initialize the memory manager
 *   - allocate initial config memory
 *   - setup the DRM framebuffer with the allocated memory
 */
<<<<<<< HEAD
int i915_driver_load(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	const struct intel_device_info *match_info =
		(struct intel_device_info *)ent->driver_data;
	struct drm_i915_private *dev_priv;
	int ret;

	/* Enable nuclear pageflip on ILK+ */
	if (!i915_modparams.nuclear_pageflip && match_info->gen < 5)
		driver.driver_features &= ~DRIVER_ATOMIC;

	ret = -ENOMEM;
	dev_priv = kzalloc(sizeof(*dev_priv), GFP_KERNEL);
	if (dev_priv)
		ret = drm_dev_init(&dev_priv->drm, &driver, &pdev->dev);
	if (ret) {
		DRM_DEV_ERROR(&pdev->dev, "allocation failed\n");
		goto out_free;
	}

	dev_priv->drm.pdev = pdev;
	dev_priv->drm.dev_private = dev_priv;
=======
int i915_driver_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	const struct intel_device_info *match_info =
		(struct intel_device_info *)ent->driver_data;
	struct drm_i915_private *i915;
	int ret;

	i915 = i915_driver_create(pdev, ent);
	if (IS_ERR(i915))
		return PTR_ERR(i915);

	/* Disable nuclear pageflip by default on pre-ILK */
	if (!i915->params.nuclear_pageflip && match_info->graphics_ver < 5)
		i915->drm.driver_features &= ~DRIVER_ATOMIC;

	/*
	 * Check if we support fake LMEM -- for now we only unleash this for
	 * the live selftests(test-and-exit).
	 */
#if IS_ENABLED(CONFIG_DRM_I915_SELFTEST)
	if (IS_ENABLED(CONFIG_DRM_I915_UNSTABLE_FAKE_LMEM)) {
		if (GRAPHICS_VER(i915) >= 9 && i915_selftest.live < 0 &&
		    i915->params.fake_lmem_start) {
			mkwrite_device_info(i915)->memory_regions =
				REGION_SMEM | REGION_LMEM | REGION_STOLEN_SMEM;
			GEM_BUG_ON(!HAS_LMEM(i915));
		}
	}
#endif
>>>>>>> upstream/android-13

	ret = pci_enable_device(pdev);
	if (ret)
		goto out_fini;

<<<<<<< HEAD
	pci_set_drvdata(pdev, &dev_priv->drm);
	/*
	 * Disable the system suspend direct complete optimization, which can
	 * leave the device suspended skipping the driver's suspend handlers
	 * if the device was already runtime suspended. This is needed due to
	 * the difference in our runtime and system suspend sequence and
	 * becaue the HDA driver may require us to enable the audio power
	 * domain during system suspend.
	 */
	dev_pm_set_driver_flags(&pdev->dev, DPM_FLAG_NEVER_SKIP);

	ret = i915_driver_init_early(dev_priv, ent);
	if (ret < 0)
		goto out_pci_disable;

	intel_runtime_pm_get(dev_priv);

	ret = i915_driver_init_mmio(dev_priv);
	if (ret < 0)
		goto out_runtime_pm_put;

	ret = i915_driver_init_hw(dev_priv);
	if (ret < 0)
		goto out_cleanup_mmio;

	/*
	 * TODO: move the vblank init and parts of modeset init steps into one
	 * of the i915_driver_init_/i915_driver_register functions according
	 * to the role/effect of the given init step.
	 */
	if (INTEL_INFO(dev_priv)->num_pipes) {
		ret = drm_vblank_init(&dev_priv->drm,
				      INTEL_INFO(dev_priv)->num_pipes);
		if (ret)
			goto out_cleanup_hw;
	}

	ret = i915_load_modeset_init(&dev_priv->drm);
	if (ret < 0)
		goto out_cleanup_hw;

	i915_driver_register(dev_priv);

	intel_runtime_pm_enable(dev_priv);

	intel_init_ipc(dev_priv);

	intel_runtime_pm_put(dev_priv);

	i915_welcome_messages(dev_priv);

	return 0;

out_cleanup_hw:
	i915_driver_cleanup_hw(dev_priv);
out_cleanup_mmio:
	i915_driver_cleanup_mmio(dev_priv);
out_runtime_pm_put:
	intel_runtime_pm_put(dev_priv);
	i915_driver_cleanup_early(dev_priv);
out_pci_disable:
	pci_disable_device(pdev);
out_fini:
	i915_load_error(dev_priv, "Device initialization failed (%d)\n", ret);
	drm_dev_fini(&dev_priv->drm);
out_free:
	kfree(dev_priv);
	pci_set_drvdata(pdev, NULL);
	return ret;
}

void i915_driver_unload(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct pci_dev *pdev = dev_priv->drm.pdev;

	i915_driver_unregister(dev_priv);

	if (i915_gem_suspend(dev_priv))
		DRM_ERROR("failed to idle hardware; continuing to unload!\n");

	intel_display_power_get(dev_priv, POWER_DOMAIN_INIT);

	drm_atomic_helper_shutdown(dev);

	intel_gvt_cleanup(dev_priv);

	intel_modeset_cleanup(dev);

	intel_bios_cleanup(dev_priv);

	vga_switcheroo_unregister_client(pdev);
	vga_client_register(pdev, NULL, NULL, NULL);

	intel_csr_ucode_fini(dev_priv);

	/* Free error state after interrupts are fully disabled. */
	cancel_delayed_work_sync(&dev_priv->gpu_error.hangcheck_work);
	i915_reset_error_state(dev_priv);

	i915_gem_fini(dev_priv);
	intel_fbc_cleanup_cfb(dev_priv);

	intel_power_domains_fini(dev_priv);

	i915_driver_cleanup_hw(dev_priv);
	i915_driver_cleanup_mmio(dev_priv);

	intel_display_power_put(dev_priv, POWER_DOMAIN_INIT);
=======
	ret = i915_driver_early_probe(i915);
	if (ret < 0)
		goto out_pci_disable;

	disable_rpm_wakeref_asserts(&i915->runtime_pm);

	intel_vgpu_detect(i915);

	ret = i915_driver_mmio_probe(i915);
	if (ret < 0)
		goto out_runtime_pm_put;

	ret = i915_driver_hw_probe(i915);
	if (ret < 0)
		goto out_cleanup_mmio;

	ret = intel_modeset_init_noirq(i915);
	if (ret < 0)
		goto out_cleanup_hw;

	ret = intel_irq_install(i915);
	if (ret)
		goto out_cleanup_modeset;

	ret = intel_modeset_init_nogem(i915);
	if (ret)
		goto out_cleanup_irq;

	ret = i915_gem_init(i915);
	if (ret)
		goto out_cleanup_modeset2;

	ret = intel_modeset_init(i915);
	if (ret)
		goto out_cleanup_gem;

	i915_driver_register(i915);

	enable_rpm_wakeref_asserts(&i915->runtime_pm);

	i915_welcome_messages(i915);

	i915->do_release = true;

	return 0;

out_cleanup_gem:
	i915_gem_suspend(i915);
	i915_gem_driver_remove(i915);
	i915_gem_driver_release(i915);
out_cleanup_modeset2:
	/* FIXME clean up the error path */
	intel_modeset_driver_remove(i915);
	intel_irq_uninstall(i915);
	intel_modeset_driver_remove_noirq(i915);
	goto out_cleanup_modeset;
out_cleanup_irq:
	intel_irq_uninstall(i915);
out_cleanup_modeset:
	intel_modeset_driver_remove_nogem(i915);
out_cleanup_hw:
	i915_driver_hw_remove(i915);
	intel_memory_regions_driver_release(i915);
	i915_ggtt_driver_release(i915);
	i915_gem_drain_freed_objects(i915);
	i915_ggtt_driver_late_release(i915);
out_cleanup_mmio:
	i915_driver_mmio_release(i915);
out_runtime_pm_put:
	enable_rpm_wakeref_asserts(&i915->runtime_pm);
	i915_driver_late_release(i915);
out_pci_disable:
	pci_disable_device(pdev);
out_fini:
	i915_probe_error(i915, "Device initialization failed (%d)\n", ret);
	return ret;
}

void i915_driver_remove(struct drm_i915_private *i915)
{
	disable_rpm_wakeref_asserts(&i915->runtime_pm);

	i915_driver_unregister(i915);

	/* Flush any external code that still may be under the RCU lock */
	synchronize_rcu();

	i915_gem_suspend(i915);

	intel_gvt_driver_remove(i915);

	intel_modeset_driver_remove(i915);

	intel_irq_uninstall(i915);

	intel_modeset_driver_remove_noirq(i915);

	i915_reset_error_state(i915);
	i915_gem_driver_remove(i915);

	intel_modeset_driver_remove_nogem(i915);

	i915_driver_hw_remove(i915);

	enable_rpm_wakeref_asserts(&i915->runtime_pm);
>>>>>>> upstream/android-13
}

static void i915_driver_release(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
<<<<<<< HEAD

	i915_driver_cleanup_early(dev_priv);
	drm_dev_fini(&dev_priv->drm);

	kfree(dev_priv);
=======
	struct intel_runtime_pm *rpm = &dev_priv->runtime_pm;

	if (!dev_priv->do_release)
		return;

	disable_rpm_wakeref_asserts(rpm);

	i915_gem_driver_release(dev_priv);

	intel_memory_regions_driver_release(dev_priv);
	i915_ggtt_driver_release(dev_priv);
	i915_gem_drain_freed_objects(dev_priv);
	i915_ggtt_driver_late_release(dev_priv);

	i915_driver_mmio_release(dev_priv);

	enable_rpm_wakeref_asserts(rpm);
	intel_runtime_pm_driver_release(rpm);

	i915_driver_late_release(dev_priv);
>>>>>>> upstream/android-13
}

static int i915_driver_open(struct drm_device *dev, struct drm_file *file)
{
	struct drm_i915_private *i915 = to_i915(dev);
	int ret;

	ret = i915_gem_open(i915, file);
	if (ret)
		return ret;

	return 0;
}

/**
 * i915_driver_lastclose - clean up after all DRM clients have exited
 * @dev: DRM device
 *
 * Take care of cleaning up after all DRM clients have exited.  In the
 * mode setting case, we want to restore the kernel's initial mode (just
 * in case the last client left us in a bad state).
 *
 * Additionally, in the non-mode setting case, we'll tear down the GTT
 * and DMA structures, since the kernel won't be using them, and clea
 * up any GEM state.
 */
static void i915_driver_lastclose(struct drm_device *dev)
{
<<<<<<< HEAD
	intel_fbdev_restore_mode(dev);
	vga_switcheroo_process_delayed_switch();
=======
	struct drm_i915_private *i915 = to_i915(dev);

	intel_fbdev_restore_mode(dev);

	if (HAS_DISPLAY(i915))
		vga_switcheroo_process_delayed_switch();
>>>>>>> upstream/android-13
}

static void i915_driver_postclose(struct drm_device *dev, struct drm_file *file)
{
	struct drm_i915_file_private *file_priv = file->driver_priv;

<<<<<<< HEAD
	mutex_lock(&dev->struct_mutex);
	i915_gem_context_close(file);
	i915_gem_release(dev, file);
	mutex_unlock(&dev->struct_mutex);

	kfree(file_priv);
=======
	i915_gem_context_close(file);

	kfree_rcu(file_priv, rcu);

	/* Catch up with all the deferred frees from "this" client */
	i915_gem_flush_free_objects(to_i915(dev));
>>>>>>> upstream/android-13
}

static void intel_suspend_encoders(struct drm_i915_private *dev_priv)
{
	struct drm_device *dev = &dev_priv->drm;
	struct intel_encoder *encoder;

<<<<<<< HEAD
=======
	if (!HAS_DISPLAY(dev_priv))
		return;

>>>>>>> upstream/android-13
	drm_modeset_lock_all(dev);
	for_each_intel_encoder(dev, encoder)
		if (encoder->suspend)
			encoder->suspend(encoder);
	drm_modeset_unlock_all(dev);
}

<<<<<<< HEAD
static int vlv_resume_prepare(struct drm_i915_private *dev_priv,
			      bool rpm_resume);
static int vlv_suspend_complete(struct drm_i915_private *dev_priv);
=======
static void intel_shutdown_encoders(struct drm_i915_private *dev_priv)
{
	struct drm_device *dev = &dev_priv->drm;
	struct intel_encoder *encoder;

	if (!HAS_DISPLAY(dev_priv))
		return;

	drm_modeset_lock_all(dev);
	for_each_intel_encoder(dev, encoder)
		if (encoder->shutdown)
			encoder->shutdown(encoder);
	drm_modeset_unlock_all(dev);
}

void i915_driver_shutdown(struct drm_i915_private *i915)
{
	disable_rpm_wakeref_asserts(&i915->runtime_pm);
	intel_runtime_pm_disable(&i915->runtime_pm);
	intel_power_domains_disable(i915);

	i915_gem_suspend(i915);

	if (HAS_DISPLAY(i915)) {
		drm_kms_helper_poll_disable(&i915->drm);

		drm_atomic_helper_shutdown(&i915->drm);
	}

	intel_dp_mst_suspend(i915);

	intel_runtime_pm_disable_interrupts(i915);
	intel_hpd_cancel_work(i915);

	intel_suspend_encoders(i915);
	intel_shutdown_encoders(i915);

	intel_dmc_ucode_suspend(i915);

	/*
	 * The only requirement is to reboot with display DC states disabled,
	 * for now leaving all display power wells in the INIT power domain
	 * enabled.
	 *
	 * TODO:
	 * - unify the pci_driver::shutdown sequence here with the
	 *   pci_driver.driver.pm.poweroff,poweroff_late sequence.
	 * - unify the driver remove and system/runtime suspend sequences with
	 *   the above unified shutdown/poweroff sequence.
	 */
	intel_power_domains_driver_remove(i915);
	enable_rpm_wakeref_asserts(&i915->runtime_pm);

	intel_runtime_pm_driver_release(&i915->runtime_pm);
}
>>>>>>> upstream/android-13

static bool suspend_to_idle(struct drm_i915_private *dev_priv)
{
#if IS_ENABLED(CONFIG_ACPI_SLEEP)
	if (acpi_target_system_state() < ACPI_STATE_S3)
		return true;
#endif
	return false;
}

static int i915_drm_prepare(struct drm_device *dev)
{
	struct drm_i915_private *i915 = to_i915(dev);
<<<<<<< HEAD
	int err;
=======
>>>>>>> upstream/android-13

	/*
	 * NB intel_display_suspend() may issue new requests after we've
	 * ostensibly marked the GPU as ready-to-sleep here. We need to
	 * split out that work and pull it forward so that after point,
	 * the GPU is not woken again.
	 */
<<<<<<< HEAD
	err = i915_gem_suspend(i915);
	if (err)
		dev_err(&i915->drm.pdev->dev,
			"GEM idle failed, suspend/resume might fail\n");

	return err;
=======
	i915_gem_suspend(i915);

	return 0;
>>>>>>> upstream/android-13
}

static int i915_drm_suspend(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
<<<<<<< HEAD
	struct pci_dev *pdev = dev_priv->drm.pdev;
	pci_power_t opregion_target_state;

	disable_rpm_wakeref_asserts(dev_priv);

	/* We do a lot of poking in a lot of registers, make sure they work
	 * properly. */
	intel_display_set_init_power(dev_priv, true);

	drm_kms_helper_poll_disable(dev);
=======
	struct pci_dev *pdev = to_pci_dev(dev_priv->drm.dev);
	pci_power_t opregion_target_state;

	disable_rpm_wakeref_asserts(&dev_priv->runtime_pm);

	/* We do a lot of poking in a lot of registers, make sure they work
	 * properly. */
	intel_power_domains_disable(dev_priv);
	if (HAS_DISPLAY(dev_priv))
		drm_kms_helper_poll_disable(dev);
>>>>>>> upstream/android-13

	pci_save_state(pdev);

	intel_display_suspend(dev);

	intel_dp_mst_suspend(dev_priv);

	intel_runtime_pm_disable_interrupts(dev_priv);
	intel_hpd_cancel_work(dev_priv);

	intel_suspend_encoders(dev_priv);

	intel_suspend_hw(dev_priv);

<<<<<<< HEAD
	i915_gem_suspend_gtt_mappings(dev_priv);

	i915_save_state(dev_priv);

	opregion_target_state = suspend_to_idle(dev_priv) ? PCI_D1 : PCI_D3cold;
	intel_opregion_notify_adapter(dev_priv, opregion_target_state);

	intel_opregion_unregister(dev_priv);
=======
	i915_ggtt_suspend(&dev_priv->ggtt);

	i915_save_display(dev_priv);

	opregion_target_state = suspend_to_idle(dev_priv) ? PCI_D1 : PCI_D3cold;
	intel_opregion_suspend(dev_priv, opregion_target_state);
>>>>>>> upstream/android-13

	intel_fbdev_set_suspend(dev, FBINFO_STATE_SUSPENDED, true);

	dev_priv->suspend_count++;

<<<<<<< HEAD
	intel_csr_ucode_suspend(dev_priv);

	enable_rpm_wakeref_asserts(dev_priv);
=======
	intel_dmc_ucode_suspend(dev_priv);

	enable_rpm_wakeref_asserts(&dev_priv->runtime_pm);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int i915_drm_suspend_late(struct drm_device *dev, bool hibernation)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct pci_dev *pdev = dev_priv->drm.pdev;
	int ret;

	disable_rpm_wakeref_asserts(dev_priv);

	i915_gem_suspend_late(dev_priv);

	intel_display_set_init_power(dev_priv, false);
	i915_rc6_ctx_wa_suspend(dev_priv);
	intel_uncore_suspend(dev_priv);

	/*
	 * In case of firmware assisted context save/restore don't manually
	 * deinit the power domains. This also means the CSR/DMC firmware will
	 * stay active, it will power down any HW resources as required and
	 * also enable deeper system power states that would be blocked if the
	 * firmware was inactive.
	 */
	if (IS_GEN9_LP(dev_priv) || hibernation || !suspend_to_idle(dev_priv) ||
	    dev_priv->csr.dmc_payload == NULL) {
		intel_power_domains_suspend(dev_priv);
		dev_priv->power_domains_suspended = true;
	}

	ret = 0;
	if (IS_GEN9_LP(dev_priv))
		bxt_enable_dc9(dev_priv);
	else if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv))
		hsw_enable_pc8(dev_priv);
	else if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv))
		ret = vlv_suspend_complete(dev_priv);

	if (ret) {
		DRM_ERROR("Suspend complete failed: %d\n", ret);
		if (dev_priv->power_domains_suspended) {
			intel_power_domains_init_hw(dev_priv, true);
			dev_priv->power_domains_suspended = false;
		}
=======
static enum i915_drm_suspend_mode
get_suspend_mode(struct drm_i915_private *dev_priv, bool hibernate)
{
	if (hibernate)
		return I915_DRM_SUSPEND_HIBERNATE;

	if (suspend_to_idle(dev_priv))
		return I915_DRM_SUSPEND_IDLE;

	return I915_DRM_SUSPEND_MEM;
}

static int i915_drm_suspend_late(struct drm_device *dev, bool hibernation)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct pci_dev *pdev = to_pci_dev(dev_priv->drm.dev);
	struct intel_runtime_pm *rpm = &dev_priv->runtime_pm;
	int ret;

	disable_rpm_wakeref_asserts(rpm);

	i915_gem_suspend_late(dev_priv);

	intel_uncore_suspend(&dev_priv->uncore);

	intel_power_domains_suspend(dev_priv,
				    get_suspend_mode(dev_priv, hibernation));

	intel_display_power_suspend_late(dev_priv);

	ret = vlv_suspend_complete(dev_priv);
	if (ret) {
		drm_err(&dev_priv->drm, "Suspend complete failed: %d\n", ret);
		intel_power_domains_resume(dev_priv);
>>>>>>> upstream/android-13

		goto out;
	}

	pci_disable_device(pdev);
	/*
	 * During hibernation on some platforms the BIOS may try to access
	 * the device even though it's already in D3 and hang the machine. So
	 * leave the device in D0 on those platforms and hope the BIOS will
	 * power down the device properly. The issue was seen on multiple old
	 * GENs with different BIOS vendors, so having an explicit blacklist
	 * is inpractical; apply the workaround on everything pre GEN6. The
	 * platforms where the issue was seen:
	 * Lenovo Thinkpad X301, X61s, X60, T60, X41
	 * Fujitsu FSC S7110
	 * Acer Aspire 1830T
	 */
<<<<<<< HEAD
	if (!(hibernation && INTEL_GEN(dev_priv) < 6))
		pci_set_power_state(pdev, PCI_D3hot);

out:
	enable_rpm_wakeref_asserts(dev_priv);
=======
	if (!(hibernation && GRAPHICS_VER(dev_priv) < 6))
		pci_set_power_state(pdev, PCI_D3hot);

out:
	enable_rpm_wakeref_asserts(rpm);
	if (!dev_priv->uncore.user_forcewake_count)
		intel_runtime_pm_driver_release(rpm);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static int i915_suspend_switcheroo(struct drm_device *dev, pm_message_t state)
{
	int error;

	if (!dev) {
		DRM_ERROR("dev: %p\n", dev);
		DRM_ERROR("DRM not initialized, aborting suspend.\n");
		return -ENODEV;
	}

	if (WARN_ON_ONCE(state.event != PM_EVENT_SUSPEND &&
			 state.event != PM_EVENT_FREEZE))
		return -EINVAL;

	if (dev->switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	error = i915_drm_suspend(dev);
	if (error)
		return error;

	return i915_drm_suspend_late(dev, false);
=======
int i915_suspend_switcheroo(struct drm_i915_private *i915, pm_message_t state)
{
	int error;

	if (drm_WARN_ON_ONCE(&i915->drm, state.event != PM_EVENT_SUSPEND &&
			     state.event != PM_EVENT_FREEZE))
		return -EINVAL;

	if (i915->drm.switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	error = i915_drm_suspend(&i915->drm);
	if (error)
		return error;

	return i915_drm_suspend_late(&i915->drm, false);
>>>>>>> upstream/android-13
}

static int i915_drm_resume(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
	int ret;

<<<<<<< HEAD
	disable_rpm_wakeref_asserts(dev_priv);
	intel_sanitize_gt_powersave(dev_priv);

	i915_gem_sanitize(dev_priv);

	ret = i915_ggtt_enable_hw(dev_priv);
	if (ret)
		DRM_ERROR("failed to re-enable GGTT\n");

	intel_csr_ucode_resume(dev_priv);

	i915_restore_state(dev_priv);
	intel_pps_unlock_regs_wa(dev_priv);
	intel_opregion_setup(dev_priv);
=======
	disable_rpm_wakeref_asserts(&dev_priv->runtime_pm);

	ret = intel_pcode_init(dev_priv);
	if (ret)
		return ret;

	sanitize_gpu(dev_priv);

	ret = i915_ggtt_enable_hw(dev_priv);
	if (ret)
		drm_err(&dev_priv->drm, "failed to re-enable GGTT\n");

	i915_ggtt_resume(&dev_priv->ggtt);

	intel_dmc_ucode_resume(dev_priv);

	i915_restore_display(dev_priv);
	intel_pps_unlock_regs_wa(dev_priv);
>>>>>>> upstream/android-13

	intel_init_pch_refclk(dev_priv);

	/*
	 * Interrupts have to be enabled before any batches are run. If not the
	 * GPU will hang. i915_gem_init_hw() will initiate batches to
	 * update/restore the context.
	 *
	 * drm_mode_config_reset() needs AUX interrupts.
	 *
	 * Modeset enabling in intel_modeset_init_hw() also needs working
	 * interrupts.
	 */
	intel_runtime_pm_enable_interrupts(dev_priv);

<<<<<<< HEAD
	drm_mode_config_reset(dev);

	i915_gem_resume(dev_priv);

	intel_modeset_init_hw(dev);
	intel_init_clock_gating(dev_priv);

	spin_lock_irq(&dev_priv->irq_lock);
	if (dev_priv->display.hpd_irq_setup)
		dev_priv->display.hpd_irq_setup(dev_priv);
	spin_unlock_irq(&dev_priv->irq_lock);

	intel_dp_mst_resume(dev_priv);

	intel_display_resume(dev);

	drm_kms_helper_poll_enable(dev);

	/*
	 * ... but also need to make sure that hotplug processing
	 * doesn't cause havoc. Like in the driver load code we don't
	 * bother with the tiny race here where we might loose hotplug
	 * notifications.
	 * */
	intel_hpd_init(dev_priv);

	intel_opregion_register(dev_priv);

	intel_fbdev_set_suspend(dev, FBINFO_STATE_RUNNING, false);

	intel_opregion_notify_adapter(dev_priv, PCI_D0);

	enable_rpm_wakeref_asserts(dev_priv);
=======
	if (HAS_DISPLAY(dev_priv))
		drm_mode_config_reset(dev);

	i915_gem_resume(dev_priv);

	intel_modeset_init_hw(dev_priv);
	intel_init_clock_gating(dev_priv);
	intel_hpd_init(dev_priv);

	/* MST sideband requires HPD interrupts enabled */
	intel_dp_mst_resume(dev_priv);
	intel_display_resume(dev);

	intel_hpd_poll_disable(dev_priv);
	if (HAS_DISPLAY(dev_priv))
		drm_kms_helper_poll_enable(dev);

	intel_opregion_resume(dev_priv);

	intel_fbdev_set_suspend(dev, FBINFO_STATE_RUNNING, false);

	intel_power_domains_enable(dev_priv);

	intel_gvt_resume(dev_priv);

	enable_rpm_wakeref_asserts(&dev_priv->runtime_pm);
>>>>>>> upstream/android-13

	return 0;
}

static int i915_drm_resume_early(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
<<<<<<< HEAD
	struct pci_dev *pdev = dev_priv->drm.pdev;
=======
	struct pci_dev *pdev = to_pci_dev(dev_priv->drm.dev);
>>>>>>> upstream/android-13
	int ret;

	/*
	 * We have a resume ordering issue with the snd-hda driver also
	 * requiring our device to be power up. Due to the lack of a
	 * parent/child relationship we currently solve this with an early
	 * resume hook.
	 *
	 * FIXME: This should be solved with a special hdmi sink device or
	 * similar so that power domains can be employed.
	 */

	/*
	 * Note that we need to set the power state explicitly, since we
	 * powered off the device during freeze and the PCI core won't power
	 * it back up for us during thaw. Powering off the device during
	 * freeze is not a hard requirement though, and during the
	 * suspend/resume phases the PCI core makes sure we get here with the
	 * device powered on. So in case we change our freeze logic and keep
	 * the device powered we can also remove the following set power state
	 * call.
	 */
	ret = pci_set_power_state(pdev, PCI_D0);
	if (ret) {
<<<<<<< HEAD
		DRM_ERROR("failed to set PCI D0 power state (%d)\n", ret);
		goto out;
=======
		drm_err(&dev_priv->drm,
			"failed to set PCI D0 power state (%d)\n", ret);
		return ret;
>>>>>>> upstream/android-13
	}

	/*
	 * Note that pci_enable_device() first enables any parent bridge
	 * device and only then sets the power state for this device. The
	 * bridge enabling is a nop though, since bridge devices are resumed
	 * first. The order of enabling power and enabling the device is
	 * imposed by the PCI core as described above, so here we preserve the
	 * same order for the freeze/thaw phases.
	 *
	 * TODO: eventually we should remove pci_disable_device() /
	 * pci_enable_enable_device() from suspend/resume. Due to how they
	 * depend on the device enable refcount we can't anyway depend on them
	 * disabling/enabling the device.
	 */
<<<<<<< HEAD
	if (pci_enable_device(pdev)) {
		ret = -EIO;
		goto out;
	}

	pci_set_master(pdev);

	disable_rpm_wakeref_asserts(dev_priv);

	if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv))
		ret = vlv_resume_prepare(dev_priv, false);
	if (ret)
		DRM_ERROR("Resume prepare failed: %d, continuing anyway\n",
			  ret);

	intel_uncore_resume_early(dev_priv);

	if (IS_GEN9_LP(dev_priv)) {
		gen9_sanitize_dc_state(dev_priv);
		bxt_disable_dc9(dev_priv);
	} else if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv)) {
		hsw_disable_pc8(dev_priv);
	}

	intel_uncore_sanitize(dev_priv);

	if (dev_priv->power_domains_suspended)
		intel_power_domains_init_hw(dev_priv, true);
	else
		intel_display_set_init_power(dev_priv, true);

	i915_rc6_ctx_wa_resume(dev_priv);

	intel_engines_sanitize(dev_priv);

	enable_rpm_wakeref_asserts(dev_priv);

out:
	dev_priv->power_domains_suspended = false;
=======
	if (pci_enable_device(pdev))
		return -EIO;

	pci_set_master(pdev);

	disable_rpm_wakeref_asserts(&dev_priv->runtime_pm);

	ret = vlv_resume_prepare(dev_priv, false);
	if (ret)
		drm_err(&dev_priv->drm,
			"Resume prepare failed: %d, continuing anyway\n", ret);

	intel_uncore_resume_early(&dev_priv->uncore);

	intel_gt_check_and_clear_faults(&dev_priv->gt);

	intel_display_power_resume_early(dev_priv);

	intel_power_domains_resume(dev_priv);

	enable_rpm_wakeref_asserts(&dev_priv->runtime_pm);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static int i915_resume_switcheroo(struct drm_device *dev)
{
	int ret;

	if (dev->switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	ret = i915_drm_resume_early(dev);
	if (ret)
		return ret;

	return i915_drm_resume(dev);
}

/**
 * i915_reset - reset chip after a hang
 * @i915: #drm_i915_private to reset
 * @stalled_mask: mask of the stalled engines with the guilty requests
 * @reason: user error message for why we are resetting
 *
 * Reset the chip.  Useful if a hang is detected. Marks the device as wedged
 * on failure.
 *
 * Caller must hold the struct_mutex.
 *
 * Procedure is fairly simple:
 *   - reset the chip using the reset reg
 *   - re-init context state
 *   - re-init hardware status page
 *   - re-init ring buffer
 *   - re-init interrupt state
 *   - re-init display
 */
void i915_reset(struct drm_i915_private *i915,
		unsigned int stalled_mask,
		const char *reason)
{
	struct i915_gpu_error *error = &i915->gpu_error;
	int ret;
	int i;

	GEM_TRACE("flags=%lx\n", error->flags);

	might_sleep();
	lockdep_assert_held(&i915->drm.struct_mutex);
	GEM_BUG_ON(!test_bit(I915_RESET_BACKOFF, &error->flags));

	if (!test_bit(I915_RESET_HANDOFF, &error->flags))
		return;

	/* Clear any previous failed attempts at recovery. Time to try again. */
	if (!i915_gem_unset_wedged(i915))
		goto wakeup;

	if (reason)
		dev_notice(i915->drm.dev, "Resetting chip for %s\n", reason);
	error->reset_count++;

	disable_irq(i915->drm.irq);
	ret = i915_gem_reset_prepare(i915);
	if (ret) {
		dev_err(i915->drm.dev, "GPU recovery failed\n");
		goto taint;
	}

	if (!intel_has_gpu_reset(i915)) {
		if (i915_modparams.reset)
			dev_err(i915->drm.dev, "GPU reset not supported\n");
		else
			DRM_DEBUG_DRIVER("GPU reset disabled\n");
		goto error;
	}

	for (i = 0; i < 3; i++) {
		ret = intel_gpu_reset(i915, ALL_ENGINES);
		if (ret == 0)
			break;

		msleep(100);
	}
	if (ret) {
		dev_err(i915->drm.dev, "Failed to reset chip\n");
		goto taint;
	}

	/* Ok, now get things going again... */

	/*
	 * Everything depends on having the GTT running, so we need to start
	 * there.
	 */
	ret = i915_ggtt_enable_hw(i915);
	if (ret) {
		DRM_ERROR("Failed to re-enable GGTT following reset (%d)\n",
			  ret);
		goto error;
	}

	i915_gem_reset(i915, stalled_mask);
	intel_overlay_reset(i915);

	/*
	 * Next we need to restore the context, but we don't use those
	 * yet either...
	 *
	 * Ring buffer needs to be re-initialized in the KMS case, or if X
	 * was running at the time of the reset (i.e. we weren't VT
	 * switched away).
	 */
	ret = i915_gem_init_hw(i915);
	if (ret) {
		DRM_ERROR("Failed to initialise HW following reset (%d)\n",
			  ret);
		goto error;
	}

	i915_queue_hangcheck(i915);

finish:
	i915_gem_reset_finish(i915);
	enable_irq(i915->drm.irq);

wakeup:
	clear_bit(I915_RESET_HANDOFF, &error->flags);
	wake_up_bit(&error->flags, I915_RESET_HANDOFF);
	return;

taint:
	/*
	 * History tells us that if we cannot reset the GPU now, we
	 * never will. This then impacts everything that is run
	 * subsequently. On failing the reset, we mark the driver
	 * as wedged, preventing further execution on the GPU.
	 * We also want to go one step further and add a taint to the
	 * kernel so that any subsequent faults can be traced back to
	 * this failure. This is important for CI, where if the
	 * GPU/driver fails we would like to reboot and restart testing
	 * rather than continue on into oblivion. For everyone else,
	 * the system should still plod along, but they have been warned!
	 */
	add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
error:
	i915_gem_set_wedged(i915);
	i915_retire_requests(i915);
	goto finish;
}

static inline int intel_gt_reset_engine(struct drm_i915_private *dev_priv,
					struct intel_engine_cs *engine)
{
	return intel_gpu_reset(dev_priv, intel_engine_flag(engine));
}

/**
 * i915_reset_engine - reset GPU engine to recover from a hang
 * @engine: engine to reset
 * @msg: reason for GPU reset; or NULL for no dev_notice()
 *
 * Reset a specific GPU engine. Useful if a hang is detected.
 * Returns zero on successful reset or otherwise an error code.
 *
 * Procedure is:
 *  - identifies the request that caused the hang and it is dropped
 *  - reset engine (which will force the engine to idle)
 *  - re-init/configure engine
 */
int i915_reset_engine(struct intel_engine_cs *engine, const char *msg)
{
	struct i915_gpu_error *error = &engine->i915->gpu_error;
	struct i915_request *active_request;
	int ret;

	GEM_TRACE("%s flags=%lx\n", engine->name, error->flags);
	GEM_BUG_ON(!test_bit(I915_RESET_ENGINE + engine->id, &error->flags));

	active_request = i915_gem_reset_prepare_engine(engine);
	if (IS_ERR_OR_NULL(active_request)) {
		/* Either the previous reset failed, or we pardon the reset. */
		ret = PTR_ERR(active_request);
		goto out;
	}

	if (msg)
		dev_notice(engine->i915->drm.dev,
			   "Resetting %s for %s\n", engine->name, msg);
	error->reset_engine_count[engine->id]++;

	if (!engine->i915->guc.execbuf_client)
		ret = intel_gt_reset_engine(engine->i915, engine);
	else
		ret = intel_guc_reset_engine(&engine->i915->guc, engine);
	if (ret) {
		/* If we fail here, we expect to fallback to a global reset */
		DRM_DEBUG_DRIVER("%sFailed to reset %s, ret=%d\n",
				 engine->i915->guc.execbuf_client ? "GuC " : "",
				 engine->name, ret);
		goto out;
	}

	/*
	 * The request that caused the hang is stuck on elsp, we know the
	 * active request and can drop it, adjust head to skip the offending
	 * request to resume executing remaining requests in the queue.
	 */
	i915_gem_reset_engine(engine, active_request, true);

	/*
	 * The engine and its registers (and workarounds in case of render)
	 * have been reset to their default values. Follow the init_ring
	 * process to program RING_MODE, HWSP and re-enable submission.
	 */
	ret = engine->init_hw(engine);
	if (ret)
		goto out;

out:
	i915_gem_reset_finish_engine(engine);
	return ret;
=======
int i915_resume_switcheroo(struct drm_i915_private *i915)
{
	int ret;

	if (i915->drm.switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	ret = i915_drm_resume_early(&i915->drm);
	if (ret)
		return ret;

	return i915_drm_resume(&i915->drm);
>>>>>>> upstream/android-13
}

static int i915_pm_prepare(struct device *kdev)
{
<<<<<<< HEAD
	struct pci_dev *pdev = to_pci_dev(kdev);
	struct drm_device *dev = pci_get_drvdata(pdev);

	if (!dev) {
=======
	struct drm_i915_private *i915 = kdev_to_i915(kdev);

	if (!i915) {
>>>>>>> upstream/android-13
		dev_err(kdev, "DRM not initialized, aborting suspend.\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	if (dev->switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	return i915_drm_prepare(dev);
=======
	if (i915->drm.switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	return i915_drm_prepare(&i915->drm);
>>>>>>> upstream/android-13
}

static int i915_pm_suspend(struct device *kdev)
{
<<<<<<< HEAD
	struct pci_dev *pdev = to_pci_dev(kdev);
	struct drm_device *dev = pci_get_drvdata(pdev);

	if (!dev) {
=======
	struct drm_i915_private *i915 = kdev_to_i915(kdev);

	if (!i915) {
>>>>>>> upstream/android-13
		dev_err(kdev, "DRM not initialized, aborting suspend.\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	if (dev->switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	return i915_drm_suspend(dev);
=======
	if (i915->drm.switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	return i915_drm_suspend(&i915->drm);
>>>>>>> upstream/android-13
}

static int i915_pm_suspend_late(struct device *kdev)
{
<<<<<<< HEAD
	struct drm_device *dev = &kdev_to_i915(kdev)->drm;
=======
	struct drm_i915_private *i915 = kdev_to_i915(kdev);
>>>>>>> upstream/android-13

	/*
	 * We have a suspend ordering issue with the snd-hda driver also
	 * requiring our device to be power up. Due to the lack of a
	 * parent/child relationship we currently solve this with an late
	 * suspend hook.
	 *
	 * FIXME: This should be solved with a special hdmi sink device or
	 * similar so that power domains can be employed.
	 */
<<<<<<< HEAD
	if (dev->switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	return i915_drm_suspend_late(dev, false);
=======
	if (i915->drm.switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	return i915_drm_suspend_late(&i915->drm, false);
>>>>>>> upstream/android-13
}

static int i915_pm_poweroff_late(struct device *kdev)
{
<<<<<<< HEAD
	struct drm_device *dev = &kdev_to_i915(kdev)->drm;

	if (dev->switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	return i915_drm_suspend_late(dev, true);
=======
	struct drm_i915_private *i915 = kdev_to_i915(kdev);

	if (i915->drm.switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	return i915_drm_suspend_late(&i915->drm, true);
>>>>>>> upstream/android-13
}

static int i915_pm_resume_early(struct device *kdev)
{
<<<<<<< HEAD
	struct drm_device *dev = &kdev_to_i915(kdev)->drm;

	if (dev->switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	return i915_drm_resume_early(dev);
=======
	struct drm_i915_private *i915 = kdev_to_i915(kdev);

	if (i915->drm.switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	return i915_drm_resume_early(&i915->drm);
>>>>>>> upstream/android-13
}

static int i915_pm_resume(struct device *kdev)
{
<<<<<<< HEAD
	struct drm_device *dev = &kdev_to_i915(kdev)->drm;

	if (dev->switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	return i915_drm_resume(dev);
=======
	struct drm_i915_private *i915 = kdev_to_i915(kdev);

	if (i915->drm.switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	return i915_drm_resume(&i915->drm);
>>>>>>> upstream/android-13
}

/* freeze: before creating the hibernation_image */
static int i915_pm_freeze(struct device *kdev)
{
<<<<<<< HEAD
	struct drm_device *dev = &kdev_to_i915(kdev)->drm;
	int ret;

	if (dev->switch_power_state != DRM_SWITCH_POWER_OFF) {
		ret = i915_drm_suspend(dev);
=======
	struct drm_i915_private *i915 = kdev_to_i915(kdev);
	int ret;

	if (i915->drm.switch_power_state != DRM_SWITCH_POWER_OFF) {
		ret = i915_drm_suspend(&i915->drm);
>>>>>>> upstream/android-13
		if (ret)
			return ret;
	}

<<<<<<< HEAD
	ret = i915_gem_freeze(kdev_to_i915(kdev));
=======
	ret = i915_gem_freeze(i915);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	return 0;
}

static int i915_pm_freeze_late(struct device *kdev)
{
<<<<<<< HEAD
	struct drm_device *dev = &kdev_to_i915(kdev)->drm;
	int ret;

	if (dev->switch_power_state != DRM_SWITCH_POWER_OFF) {
		ret = i915_drm_suspend_late(dev, true);
=======
	struct drm_i915_private *i915 = kdev_to_i915(kdev);
	int ret;

	if (i915->drm.switch_power_state != DRM_SWITCH_POWER_OFF) {
		ret = i915_drm_suspend_late(&i915->drm, true);
>>>>>>> upstream/android-13
		if (ret)
			return ret;
	}

<<<<<<< HEAD
	ret = i915_gem_freeze_late(kdev_to_i915(kdev));
=======
	ret = i915_gem_freeze_late(i915);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	return 0;
}

/* thaw: called after creating the hibernation image, but before turning off. */
static int i915_pm_thaw_early(struct device *kdev)
{
	return i915_pm_resume_early(kdev);
}

static int i915_pm_thaw(struct device *kdev)
{
	return i915_pm_resume(kdev);
}

/* restore: called after loading the hibernation image. */
static int i915_pm_restore_early(struct device *kdev)
{
	return i915_pm_resume_early(kdev);
}

static int i915_pm_restore(struct device *kdev)
{
	return i915_pm_resume(kdev);
}

<<<<<<< HEAD
/*
 * Save all Gunit registers that may be lost after a D3 and a subsequent
 * S0i[R123] transition. The list of registers needing a save/restore is
 * defined in the VLV2_S0IXRegs document. This documents marks all Gunit
 * registers in the following way:
 * - Driver: saved/restored by the driver
 * - Punit : saved/restored by the Punit firmware
 * - No, w/o marking: no need to save/restore, since the register is R/O or
 *                    used internally by the HW in a way that doesn't depend
 *                    keeping the content across a suspend/resume.
 * - Debug : used for debugging
 *
 * We save/restore all registers marked with 'Driver', with the following
 * exceptions:
 * - Registers out of use, including also registers marked with 'Debug'.
 *   These have no effect on the driver's operation, so we don't save/restore
 *   them to reduce the overhead.
 * - Registers that are fully setup by an initialization function called from
 *   the resume path. For example many clock gating and RPS/RC6 registers.
 * - Registers that provide the right functionality with their reset defaults.
 *
 * TODO: Except for registers that based on the above 3 criteria can be safely
 * ignored, we save/restore all others, practically treating the HW context as
 * a black-box for the driver. Further investigation is needed to reduce the
 * saved/restored registers even further, by following the same 3 criteria.
 */
static void vlv_save_gunit_s0ix_state(struct drm_i915_private *dev_priv)
{
	struct vlv_s0ix_state *s = &dev_priv->vlv_s0ix_state;
	int i;

	/* GAM 0x4000-0x4770 */
	s->wr_watermark		= I915_READ(GEN7_WR_WATERMARK);
	s->gfx_prio_ctrl	= I915_READ(GEN7_GFX_PRIO_CTRL);
	s->arb_mode		= I915_READ(ARB_MODE);
	s->gfx_pend_tlb0	= I915_READ(GEN7_GFX_PEND_TLB0);
	s->gfx_pend_tlb1	= I915_READ(GEN7_GFX_PEND_TLB1);

	for (i = 0; i < ARRAY_SIZE(s->lra_limits); i++)
		s->lra_limits[i] = I915_READ(GEN7_LRA_LIMITS(i));

	s->media_max_req_count	= I915_READ(GEN7_MEDIA_MAX_REQ_COUNT);
	s->gfx_max_req_count	= I915_READ(GEN7_GFX_MAX_REQ_COUNT);

	s->render_hwsp		= I915_READ(RENDER_HWS_PGA_GEN7);
	s->ecochk		= I915_READ(GAM_ECOCHK);
	s->bsd_hwsp		= I915_READ(BSD_HWS_PGA_GEN7);
	s->blt_hwsp		= I915_READ(BLT_HWS_PGA_GEN7);

	s->tlb_rd_addr		= I915_READ(GEN7_TLB_RD_ADDR);

	/* MBC 0x9024-0x91D0, 0x8500 */
	s->g3dctl		= I915_READ(VLV_G3DCTL);
	s->gsckgctl		= I915_READ(VLV_GSCKGCTL);
	s->mbctl		= I915_READ(GEN6_MBCTL);

	/* GCP 0x9400-0x9424, 0x8100-0x810C */
	s->ucgctl1		= I915_READ(GEN6_UCGCTL1);
	s->ucgctl3		= I915_READ(GEN6_UCGCTL3);
	s->rcgctl1		= I915_READ(GEN6_RCGCTL1);
	s->rcgctl2		= I915_READ(GEN6_RCGCTL2);
	s->rstctl		= I915_READ(GEN6_RSTCTL);
	s->misccpctl		= I915_READ(GEN7_MISCCPCTL);

	/* GPM 0xA000-0xAA84, 0x8000-0x80FC */
	s->gfxpause		= I915_READ(GEN6_GFXPAUSE);
	s->rpdeuhwtc		= I915_READ(GEN6_RPDEUHWTC);
	s->rpdeuc		= I915_READ(GEN6_RPDEUC);
	s->ecobus		= I915_READ(ECOBUS);
	s->pwrdwnupctl		= I915_READ(VLV_PWRDWNUPCTL);
	s->rp_down_timeout	= I915_READ(GEN6_RP_DOWN_TIMEOUT);
	s->rp_deucsw		= I915_READ(GEN6_RPDEUCSW);
	s->rcubmabdtmr		= I915_READ(GEN6_RCUBMABDTMR);
	s->rcedata		= I915_READ(VLV_RCEDATA);
	s->spare2gh		= I915_READ(VLV_SPAREG2H);

	/* Display CZ domain, 0x4400C-0x4402C, 0x4F000-0x4F11F */
	s->gt_imr		= I915_READ(GTIMR);
	s->gt_ier		= I915_READ(GTIER);
	s->pm_imr		= I915_READ(GEN6_PMIMR);
	s->pm_ier		= I915_READ(GEN6_PMIER);

	for (i = 0; i < ARRAY_SIZE(s->gt_scratch); i++)
		s->gt_scratch[i] = I915_READ(GEN7_GT_SCRATCH(i));

	/* GT SA CZ domain, 0x100000-0x138124 */
	s->tilectl		= I915_READ(TILECTL);
	s->gt_fifoctl		= I915_READ(GTFIFOCTL);
	s->gtlc_wake_ctrl	= I915_READ(VLV_GTLC_WAKE_CTRL);
	s->gtlc_survive		= I915_READ(VLV_GTLC_SURVIVABILITY_REG);
	s->pmwgicz		= I915_READ(VLV_PMWGICZ);

	/* Gunit-Display CZ domain, 0x182028-0x1821CF */
	s->gu_ctl0		= I915_READ(VLV_GU_CTL0);
	s->gu_ctl1		= I915_READ(VLV_GU_CTL1);
	s->pcbr			= I915_READ(VLV_PCBR);
	s->clock_gate_dis2	= I915_READ(VLV_GUNIT_CLOCK_GATE2);

	/*
	 * Not saving any of:
	 * DFT,		0x9800-0x9EC0
	 * SARB,	0xB000-0xB1FC
	 * GAC,		0x5208-0x524C, 0x14000-0x14C000
	 * PCI CFG
	 */
}

static void vlv_restore_gunit_s0ix_state(struct drm_i915_private *dev_priv)
{
	struct vlv_s0ix_state *s = &dev_priv->vlv_s0ix_state;
	u32 val;
	int i;

	/* GAM 0x4000-0x4770 */
	I915_WRITE(GEN7_WR_WATERMARK,	s->wr_watermark);
	I915_WRITE(GEN7_GFX_PRIO_CTRL,	s->gfx_prio_ctrl);
	I915_WRITE(ARB_MODE,		s->arb_mode | (0xffff << 16));
	I915_WRITE(GEN7_GFX_PEND_TLB0,	s->gfx_pend_tlb0);
	I915_WRITE(GEN7_GFX_PEND_TLB1,	s->gfx_pend_tlb1);

	for (i = 0; i < ARRAY_SIZE(s->lra_limits); i++)
		I915_WRITE(GEN7_LRA_LIMITS(i), s->lra_limits[i]);

	I915_WRITE(GEN7_MEDIA_MAX_REQ_COUNT, s->media_max_req_count);
	I915_WRITE(GEN7_GFX_MAX_REQ_COUNT, s->gfx_max_req_count);

	I915_WRITE(RENDER_HWS_PGA_GEN7,	s->render_hwsp);
	I915_WRITE(GAM_ECOCHK,		s->ecochk);
	I915_WRITE(BSD_HWS_PGA_GEN7,	s->bsd_hwsp);
	I915_WRITE(BLT_HWS_PGA_GEN7,	s->blt_hwsp);

	I915_WRITE(GEN7_TLB_RD_ADDR,	s->tlb_rd_addr);

	/* MBC 0x9024-0x91D0, 0x8500 */
	I915_WRITE(VLV_G3DCTL,		s->g3dctl);
	I915_WRITE(VLV_GSCKGCTL,	s->gsckgctl);
	I915_WRITE(GEN6_MBCTL,		s->mbctl);

	/* GCP 0x9400-0x9424, 0x8100-0x810C */
	I915_WRITE(GEN6_UCGCTL1,	s->ucgctl1);
	I915_WRITE(GEN6_UCGCTL3,	s->ucgctl3);
	I915_WRITE(GEN6_RCGCTL1,	s->rcgctl1);
	I915_WRITE(GEN6_RCGCTL2,	s->rcgctl2);
	I915_WRITE(GEN6_RSTCTL,		s->rstctl);
	I915_WRITE(GEN7_MISCCPCTL,	s->misccpctl);

	/* GPM 0xA000-0xAA84, 0x8000-0x80FC */
	I915_WRITE(GEN6_GFXPAUSE,	s->gfxpause);
	I915_WRITE(GEN6_RPDEUHWTC,	s->rpdeuhwtc);
	I915_WRITE(GEN6_RPDEUC,		s->rpdeuc);
	I915_WRITE(ECOBUS,		s->ecobus);
	I915_WRITE(VLV_PWRDWNUPCTL,	s->pwrdwnupctl);
	I915_WRITE(GEN6_RP_DOWN_TIMEOUT,s->rp_down_timeout);
	I915_WRITE(GEN6_RPDEUCSW,	s->rp_deucsw);
	I915_WRITE(GEN6_RCUBMABDTMR,	s->rcubmabdtmr);
	I915_WRITE(VLV_RCEDATA,		s->rcedata);
	I915_WRITE(VLV_SPAREG2H,	s->spare2gh);

	/* Display CZ domain, 0x4400C-0x4402C, 0x4F000-0x4F11F */
	I915_WRITE(GTIMR,		s->gt_imr);
	I915_WRITE(GTIER,		s->gt_ier);
	I915_WRITE(GEN6_PMIMR,		s->pm_imr);
	I915_WRITE(GEN6_PMIER,		s->pm_ier);

	for (i = 0; i < ARRAY_SIZE(s->gt_scratch); i++)
		I915_WRITE(GEN7_GT_SCRATCH(i), s->gt_scratch[i]);

	/* GT SA CZ domain, 0x100000-0x138124 */
	I915_WRITE(TILECTL,			s->tilectl);
	I915_WRITE(GTFIFOCTL,			s->gt_fifoctl);
	/*
	 * Preserve the GT allow wake and GFX force clock bit, they are not
	 * be restored, as they are used to control the s0ix suspend/resume
	 * sequence by the caller.
	 */
	val = I915_READ(VLV_GTLC_WAKE_CTRL);
	val &= VLV_GTLC_ALLOWWAKEREQ;
	val |= s->gtlc_wake_ctrl & ~VLV_GTLC_ALLOWWAKEREQ;
	I915_WRITE(VLV_GTLC_WAKE_CTRL, val);

	val = I915_READ(VLV_GTLC_SURVIVABILITY_REG);
	val &= VLV_GFX_CLK_FORCE_ON_BIT;
	val |= s->gtlc_survive & ~VLV_GFX_CLK_FORCE_ON_BIT;
	I915_WRITE(VLV_GTLC_SURVIVABILITY_REG, val);

	I915_WRITE(VLV_PMWGICZ,			s->pmwgicz);

	/* Gunit-Display CZ domain, 0x182028-0x1821CF */
	I915_WRITE(VLV_GU_CTL0,			s->gu_ctl0);
	I915_WRITE(VLV_GU_CTL1,			s->gu_ctl1);
	I915_WRITE(VLV_PCBR,			s->pcbr);
	I915_WRITE(VLV_GUNIT_CLOCK_GATE2,	s->clock_gate_dis2);
}

static int vlv_wait_for_pw_status(struct drm_i915_private *dev_priv,
				  u32 mask, u32 val)
{
	/* The HW does not like us polling for PW_STATUS frequently, so
	 * use the sleeping loop rather than risk the busy spin within
	 * intel_wait_for_register().
	 *
	 * Transitioning between RC6 states should be at most 2ms (see
	 * valleyview_enable_rps) so use a 3ms timeout.
	 */
	return wait_for((I915_READ_NOTRACE(VLV_GTLC_PW_STATUS) & mask) == val,
			3);
}

int vlv_force_gfx_clock(struct drm_i915_private *dev_priv, bool force_on)
{
	u32 val;
	int err;

	val = I915_READ(VLV_GTLC_SURVIVABILITY_REG);
	val &= ~VLV_GFX_CLK_FORCE_ON_BIT;
	if (force_on)
		val |= VLV_GFX_CLK_FORCE_ON_BIT;
	I915_WRITE(VLV_GTLC_SURVIVABILITY_REG, val);

	if (!force_on)
		return 0;

	err = intel_wait_for_register(dev_priv,
				      VLV_GTLC_SURVIVABILITY_REG,
				      VLV_GFX_CLK_STATUS_BIT,
				      VLV_GFX_CLK_STATUS_BIT,
				      20);
	if (err)
		DRM_ERROR("timeout waiting for GFX clock force-on (%08x)\n",
			  I915_READ(VLV_GTLC_SURVIVABILITY_REG));

	return err;
}

static int vlv_allow_gt_wake(struct drm_i915_private *dev_priv, bool allow)
{
	u32 mask;
	u32 val;
	int err;

	val = I915_READ(VLV_GTLC_WAKE_CTRL);
	val &= ~VLV_GTLC_ALLOWWAKEREQ;
	if (allow)
		val |= VLV_GTLC_ALLOWWAKEREQ;
	I915_WRITE(VLV_GTLC_WAKE_CTRL, val);
	POSTING_READ(VLV_GTLC_WAKE_CTRL);

	mask = VLV_GTLC_ALLOWWAKEACK;
	val = allow ? mask : 0;

	err = vlv_wait_for_pw_status(dev_priv, mask, val);
	if (err)
		DRM_ERROR("timeout disabling GT waking\n");

	return err;
}

static void vlv_wait_for_gt_wells(struct drm_i915_private *dev_priv,
				  bool wait_for_on)
{
	u32 mask;
	u32 val;

	mask = VLV_GTLC_PW_MEDIA_STATUS_MASK | VLV_GTLC_PW_RENDER_STATUS_MASK;
	val = wait_for_on ? mask : 0;

	/*
	 * RC6 transitioning can be delayed up to 2 msec (see
	 * valleyview_enable_rps), use 3 msec for safety.
	 *
	 * This can fail to turn off the rc6 if the GPU is stuck after a failed
	 * reset and we are trying to force the machine to sleep.
	 */
	if (vlv_wait_for_pw_status(dev_priv, mask, val))
		DRM_DEBUG_DRIVER("timeout waiting for GT wells to go %s\n",
				 onoff(wait_for_on));
}

static void vlv_check_no_gt_access(struct drm_i915_private *dev_priv)
{
	if (!(I915_READ(VLV_GTLC_PW_STATUS) & VLV_GTLC_ALLOWWAKEERR))
		return;

	DRM_DEBUG_DRIVER("GT register access while GT waking disabled\n");
	I915_WRITE(VLV_GTLC_PW_STATUS, VLV_GTLC_ALLOWWAKEERR);
}

static int vlv_suspend_complete(struct drm_i915_private *dev_priv)
{
	u32 mask;
	int err;

	/*
	 * Bspec defines the following GT well on flags as debug only, so
	 * don't treat them as hard failures.
	 */
	vlv_wait_for_gt_wells(dev_priv, false);

	mask = VLV_GTLC_RENDER_CTX_EXISTS | VLV_GTLC_MEDIA_CTX_EXISTS;
	WARN_ON((I915_READ(VLV_GTLC_WAKE_CTRL) & mask) != mask);

	vlv_check_no_gt_access(dev_priv);

	err = vlv_force_gfx_clock(dev_priv, true);
	if (err)
		goto err1;

	err = vlv_allow_gt_wake(dev_priv, false);
	if (err)
		goto err2;

	if (!IS_CHERRYVIEW(dev_priv))
		vlv_save_gunit_s0ix_state(dev_priv);

	err = vlv_force_gfx_clock(dev_priv, false);
	if (err)
		goto err2;

	return 0;

err2:
	/* For safety always re-enable waking and disable gfx clock forcing */
	vlv_allow_gt_wake(dev_priv, true);
err1:
	vlv_force_gfx_clock(dev_priv, false);

	return err;
}

static int vlv_resume_prepare(struct drm_i915_private *dev_priv,
				bool rpm_resume)
{
	int err;
	int ret;

	/*
	 * If any of the steps fail just try to continue, that's the best we
	 * can do at this point. Return the first error code (which will also
	 * leave RPM permanently disabled).
	 */
	ret = vlv_force_gfx_clock(dev_priv, true);

	if (!IS_CHERRYVIEW(dev_priv))
		vlv_restore_gunit_s0ix_state(dev_priv);

	err = vlv_allow_gt_wake(dev_priv, true);
	if (!ret)
		ret = err;

	err = vlv_force_gfx_clock(dev_priv, false);
	if (!ret)
		ret = err;

	vlv_check_no_gt_access(dev_priv);

	if (rpm_resume)
		intel_init_clock_gating(dev_priv);

	return ret;
}

static int intel_runtime_suspend(struct device *kdev)
{
	struct pci_dev *pdev = to_pci_dev(kdev);
	struct drm_device *dev = pci_get_drvdata(pdev);
	struct drm_i915_private *dev_priv = to_i915(dev);
	int ret;

	if (WARN_ON_ONCE(!(dev_priv->gt_pm.rc6.enabled && HAS_RC6(dev_priv))))
		return -ENODEV;

	if (WARN_ON_ONCE(!HAS_RUNTIME_PM(dev_priv)))
		return -ENODEV;

	DRM_DEBUG_KMS("Suspending device\n");

	disable_rpm_wakeref_asserts(dev_priv);
=======
static int intel_runtime_suspend(struct device *kdev)
{
	struct drm_i915_private *dev_priv = kdev_to_i915(kdev);
	struct intel_runtime_pm *rpm = &dev_priv->runtime_pm;
	int ret;

	if (drm_WARN_ON_ONCE(&dev_priv->drm, !HAS_RUNTIME_PM(dev_priv)))
		return -ENODEV;

	drm_dbg_kms(&dev_priv->drm, "Suspending device\n");

	disable_rpm_wakeref_asserts(rpm);
>>>>>>> upstream/android-13

	/*
	 * We are safe here against re-faults, since the fault handler takes
	 * an RPM reference.
	 */
	i915_gem_runtime_suspend(dev_priv);

<<<<<<< HEAD
	intel_uc_suspend(dev_priv);

	intel_runtime_pm_disable_interrupts(dev_priv);

	intel_uncore_suspend(dev_priv);

	ret = 0;
	if (IS_GEN9_LP(dev_priv)) {
		bxt_display_core_uninit(dev_priv);
		bxt_enable_dc9(dev_priv);
	} else if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv)) {
		hsw_enable_pc8(dev_priv);
	} else if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv)) {
		ret = vlv_suspend_complete(dev_priv);
	}

	if (ret) {
		DRM_ERROR("Runtime suspend failed, disabling it (%d)\n", ret);
		intel_uncore_runtime_resume(dev_priv);

		intel_runtime_pm_enable_interrupts(dev_priv);

		intel_uc_resume(dev_priv);

		i915_gem_init_swizzling(dev_priv);
		i915_gem_restore_fences(dev_priv);

		enable_rpm_wakeref_asserts(dev_priv);
=======
	intel_gt_runtime_suspend(&dev_priv->gt);

	intel_runtime_pm_disable_interrupts(dev_priv);

	intel_uncore_suspend(&dev_priv->uncore);

	intel_display_power_suspend(dev_priv);

	ret = vlv_suspend_complete(dev_priv);
	if (ret) {
		drm_err(&dev_priv->drm,
			"Runtime suspend failed, disabling it (%d)\n", ret);
		intel_uncore_runtime_resume(&dev_priv->uncore);

		intel_runtime_pm_enable_interrupts(dev_priv);

		intel_gt_runtime_resume(&dev_priv->gt);

		enable_rpm_wakeref_asserts(rpm);
>>>>>>> upstream/android-13

		return ret;
	}

<<<<<<< HEAD
	enable_rpm_wakeref_asserts(dev_priv);
	WARN_ON_ONCE(atomic_read(&dev_priv->runtime_pm.wakeref_count));

	if (intel_uncore_arm_unclaimed_mmio_detection(dev_priv))
		DRM_ERROR("Unclaimed access detected prior to suspending\n");

	dev_priv->runtime_pm.suspended = true;
=======
	enable_rpm_wakeref_asserts(rpm);
	intel_runtime_pm_driver_release(rpm);

	if (intel_uncore_arm_unclaimed_mmio_detection(&dev_priv->uncore))
		drm_err(&dev_priv->drm,
			"Unclaimed access detected prior to suspending\n");

	rpm->suspended = true;
>>>>>>> upstream/android-13

	/*
	 * FIXME: We really should find a document that references the arguments
	 * used below!
	 */
	if (IS_BROADWELL(dev_priv)) {
		/*
		 * On Broadwell, if we use PCI_D1 the PCH DDI ports will stop
		 * being detected, and the call we do at intel_runtime_resume()
		 * won't be able to restore them. Since PCI_D3hot matches the
		 * actual specification and appears to be working, use it.
		 */
		intel_opregion_notify_adapter(dev_priv, PCI_D3hot);
	} else {
		/*
		 * current versions of firmware which depend on this opregion
		 * notification have repurposed the D1 definition to mean
		 * "runtime suspended" vs. what you would normally expect (D3)
		 * to distinguish it from notifications that might be sent via
		 * the suspend path.
		 */
		intel_opregion_notify_adapter(dev_priv, PCI_D1);
	}

<<<<<<< HEAD
	assert_forcewakes_inactive(dev_priv);

	if (!IS_VALLEYVIEW(dev_priv) && !IS_CHERRYVIEW(dev_priv))
		intel_hpd_poll_init(dev_priv);

	DRM_DEBUG_KMS("Device suspended\n");
=======
	assert_forcewakes_inactive(&dev_priv->uncore);

	if (!IS_VALLEYVIEW(dev_priv) && !IS_CHERRYVIEW(dev_priv))
		intel_hpd_poll_enable(dev_priv);

	drm_dbg_kms(&dev_priv->drm, "Device suspended\n");
>>>>>>> upstream/android-13
	return 0;
}

static int intel_runtime_resume(struct device *kdev)
{
<<<<<<< HEAD
	struct pci_dev *pdev = to_pci_dev(kdev);
	struct drm_device *dev = pci_get_drvdata(pdev);
	struct drm_i915_private *dev_priv = to_i915(dev);
	int ret = 0;

	if (WARN_ON_ONCE(!HAS_RUNTIME_PM(dev_priv)))
		return -ENODEV;

	DRM_DEBUG_KMS("Resuming device\n");

	WARN_ON_ONCE(atomic_read(&dev_priv->runtime_pm.wakeref_count));
	disable_rpm_wakeref_asserts(dev_priv);

	intel_opregion_notify_adapter(dev_priv, PCI_D0);
	dev_priv->runtime_pm.suspended = false;
	if (intel_uncore_unclaimed_mmio(dev_priv))
		DRM_DEBUG_DRIVER("Unclaimed access during suspend, bios?\n");

	if (IS_GEN9_LP(dev_priv)) {
		bxt_disable_dc9(dev_priv);
		bxt_display_core_init(dev_priv, true);
		if (dev_priv->csr.dmc_payload &&
		    (dev_priv->csr.allowed_dc_mask & DC_STATE_EN_UPTO_DC5))
			gen9_enable_dc5(dev_priv);
	} else if (IS_HASWELL(dev_priv) || IS_BROADWELL(dev_priv)) {
		hsw_disable_pc8(dev_priv);
	} else if (IS_VALLEYVIEW(dev_priv) || IS_CHERRYVIEW(dev_priv)) {
		ret = vlv_resume_prepare(dev_priv, true);
	}

	intel_uncore_runtime_resume(dev_priv);

	intel_runtime_pm_enable_interrupts(dev_priv);

	intel_uc_resume(dev_priv);

=======
	struct drm_i915_private *dev_priv = kdev_to_i915(kdev);
	struct intel_runtime_pm *rpm = &dev_priv->runtime_pm;
	int ret;

	if (drm_WARN_ON_ONCE(&dev_priv->drm, !HAS_RUNTIME_PM(dev_priv)))
		return -ENODEV;

	drm_dbg_kms(&dev_priv->drm, "Resuming device\n");

	drm_WARN_ON_ONCE(&dev_priv->drm, atomic_read(&rpm->wakeref_count));
	disable_rpm_wakeref_asserts(rpm);

	intel_opregion_notify_adapter(dev_priv, PCI_D0);
	rpm->suspended = false;
	if (intel_uncore_unclaimed_mmio(&dev_priv->uncore))
		drm_dbg(&dev_priv->drm,
			"Unclaimed access during suspend, bios?\n");

	intel_display_power_resume(dev_priv);

	ret = vlv_resume_prepare(dev_priv, true);

	intel_uncore_runtime_resume(&dev_priv->uncore);

	intel_runtime_pm_enable_interrupts(dev_priv);

>>>>>>> upstream/android-13
	/*
	 * No point of rolling back things in case of an error, as the best
	 * we can do is to hope that things will still work (and disable RPM).
	 */
<<<<<<< HEAD
	i915_gem_init_swizzling(dev_priv);
	i915_gem_restore_fences(dev_priv);
=======
	intel_gt_runtime_resume(&dev_priv->gt);
>>>>>>> upstream/android-13

	/*
	 * On VLV/CHV display interrupts are part of the display
	 * power well, so hpd is reinitialized from there. For
	 * everyone else do it here.
	 */
<<<<<<< HEAD
	if (!IS_VALLEYVIEW(dev_priv) && !IS_CHERRYVIEW(dev_priv))
		intel_hpd_init(dev_priv);

	intel_enable_ipc(dev_priv);

	enable_rpm_wakeref_asserts(dev_priv);

	if (ret)
		DRM_ERROR("Runtime resume failed, disabling it (%d)\n", ret);
	else
		DRM_DEBUG_KMS("Device resumed\n");
=======
	if (!IS_VALLEYVIEW(dev_priv) && !IS_CHERRYVIEW(dev_priv)) {
		intel_hpd_init(dev_priv);
		intel_hpd_poll_disable(dev_priv);
	}

	intel_enable_ipc(dev_priv);

	enable_rpm_wakeref_asserts(rpm);

	if (ret)
		drm_err(&dev_priv->drm,
			"Runtime resume failed, disabling it (%d)\n", ret);
	else
		drm_dbg_kms(&dev_priv->drm, "Device resumed\n");
>>>>>>> upstream/android-13

	return ret;
}

const struct dev_pm_ops i915_pm_ops = {
	/*
	 * S0ix (via system suspend) and S3 event handlers [PMSG_SUSPEND,
	 * PMSG_RESUME]
	 */
	.prepare = i915_pm_prepare,
	.suspend = i915_pm_suspend,
	.suspend_late = i915_pm_suspend_late,
	.resume_early = i915_pm_resume_early,
	.resume = i915_pm_resume,

	/*
	 * S4 event handlers
	 * @freeze, @freeze_late    : called (1) before creating the
	 *                            hibernation image [PMSG_FREEZE] and
	 *                            (2) after rebooting, before restoring
	 *                            the image [PMSG_QUIESCE]
	 * @thaw, @thaw_early       : called (1) after creating the hibernation
	 *                            image, before writing it [PMSG_THAW]
	 *                            and (2) after failing to create or
	 *                            restore the image [PMSG_RECOVER]
	 * @poweroff, @poweroff_late: called after writing the hibernation
	 *                            image, before rebooting [PMSG_HIBERNATE]
	 * @restore, @restore_early : called after rebooting and restoring the
	 *                            hibernation image [PMSG_RESTORE]
	 */
	.freeze = i915_pm_freeze,
	.freeze_late = i915_pm_freeze_late,
	.thaw_early = i915_pm_thaw_early,
	.thaw = i915_pm_thaw,
	.poweroff = i915_pm_suspend,
	.poweroff_late = i915_pm_poweroff_late,
	.restore_early = i915_pm_restore_early,
	.restore = i915_pm_restore,

	/* S0ix (via runtime suspend) event handlers */
	.runtime_suspend = intel_runtime_suspend,
	.runtime_resume = intel_runtime_resume,
};

<<<<<<< HEAD
static const struct vm_operations_struct i915_gem_vm_ops = {
	.fault = i915_gem_fault,
	.open = drm_gem_vm_open,
	.close = drm_gem_vm_close,
};

static const struct file_operations i915_driver_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.release = drm_release,
	.unlocked_ioctl = drm_ioctl,
	.mmap = drm_gem_mmap,
	.poll = drm_poll,
	.read = drm_read,
	.compat_ioctl = i915_compat_ioctl,
=======
static const struct file_operations i915_driver_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.release = drm_release_noglobal,
	.unlocked_ioctl = drm_ioctl,
	.mmap = i915_gem_mmap,
	.poll = drm_poll,
	.read = drm_read,
	.compat_ioctl = i915_ioc32_compat_ioctl,
>>>>>>> upstream/android-13
	.llseek = noop_llseek,
};

static int
i915_gem_reject_pin_ioctl(struct drm_device *dev, void *data,
			  struct drm_file *file)
{
	return -ENODEV;
}

static const struct drm_ioctl_desc i915_ioctls[] = {
	DRM_IOCTL_DEF_DRV(I915_INIT, drm_noop, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_FLUSH, drm_noop, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(I915_FLIP, drm_noop, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(I915_BATCHBUFFER, drm_noop, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(I915_IRQ_EMIT, drm_noop, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(I915_IRQ_WAIT, drm_noop, DRM_AUTH),
<<<<<<< HEAD
	DRM_IOCTL_DEF_DRV(I915_GETPARAM, i915_getparam_ioctl, DRM_AUTH|DRM_RENDER_ALLOW),
=======
	DRM_IOCTL_DEF_DRV(I915_GETPARAM, i915_getparam_ioctl, DRM_RENDER_ALLOW),
>>>>>>> upstream/android-13
	DRM_IOCTL_DEF_DRV(I915_SETPARAM, drm_noop, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_ALLOC, drm_noop, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(I915_FREE, drm_noop, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(I915_INIT_HEAP, drm_noop, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_CMDBUFFER, drm_noop, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(I915_DESTROY_HEAP,  drm_noop, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_SET_VBLANK_PIPE,  drm_noop, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_GET_VBLANK_PIPE,  drm_noop, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(I915_VBLANK_SWAP, drm_noop, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(I915_HWS_ADDR, drm_noop, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_GEM_INIT, drm_noop, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
<<<<<<< HEAD
	DRM_IOCTL_DEF_DRV(I915_GEM_EXECBUFFER, i915_gem_execbuffer_ioctl, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(I915_GEM_EXECBUFFER2_WR, i915_gem_execbuffer2_ioctl, DRM_AUTH|DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_PIN, i915_gem_reject_pin_ioctl, DRM_AUTH|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_GEM_UNPIN, i915_gem_reject_pin_ioctl, DRM_AUTH|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_GEM_BUSY, i915_gem_busy_ioctl, DRM_AUTH|DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_SET_CACHING, i915_gem_set_caching_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_GET_CACHING, i915_gem_get_caching_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_THROTTLE, i915_gem_throttle_ioctl, DRM_AUTH|DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_ENTERVT, drm_noop, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_GEM_LEAVEVT, drm_noop, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_GEM_CREATE, i915_gem_create_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_PREAD, i915_gem_pread_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_PWRITE, i915_gem_pwrite_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_MMAP, i915_gem_mmap_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_MMAP_GTT, i915_gem_mmap_gtt_ioctl, DRM_RENDER_ALLOW),
=======
	DRM_IOCTL_DEF_DRV(I915_GEM_EXECBUFFER, drm_invalid_op, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(I915_GEM_EXECBUFFER2_WR, i915_gem_execbuffer2_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_PIN, i915_gem_reject_pin_ioctl, DRM_AUTH|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_GEM_UNPIN, i915_gem_reject_pin_ioctl, DRM_AUTH|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_GEM_BUSY, i915_gem_busy_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_SET_CACHING, i915_gem_set_caching_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_GET_CACHING, i915_gem_get_caching_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_THROTTLE, i915_gem_throttle_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_ENTERVT, drm_noop, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_GEM_LEAVEVT, drm_noop, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(I915_GEM_CREATE, i915_gem_create_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_CREATE_EXT, i915_gem_create_ext_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_PREAD, i915_gem_pread_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_PWRITE, i915_gem_pwrite_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_MMAP, i915_gem_mmap_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_MMAP_OFFSET, i915_gem_mmap_offset_ioctl, DRM_RENDER_ALLOW),
>>>>>>> upstream/android-13
	DRM_IOCTL_DEF_DRV(I915_GEM_SET_DOMAIN, i915_gem_set_domain_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_SW_FINISH, i915_gem_sw_finish_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_SET_TILING, i915_gem_set_tiling_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_GET_TILING, i915_gem_get_tiling_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_GET_APERTURE, i915_gem_get_aperture_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GET_PIPE_FROM_CRTC_ID, intel_get_pipe_from_crtc_id_ioctl, 0),
	DRM_IOCTL_DEF_DRV(I915_GEM_MADVISE, i915_gem_madvise_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_OVERLAY_PUT_IMAGE, intel_overlay_put_image_ioctl, DRM_MASTER),
	DRM_IOCTL_DEF_DRV(I915_OVERLAY_ATTRS, intel_overlay_attrs_ioctl, DRM_MASTER),
	DRM_IOCTL_DEF_DRV(I915_SET_SPRITE_COLORKEY, intel_sprite_set_colorkey_ioctl, DRM_MASTER),
	DRM_IOCTL_DEF_DRV(I915_GET_SPRITE_COLORKEY, drm_noop, DRM_MASTER),
<<<<<<< HEAD
	DRM_IOCTL_DEF_DRV(I915_GEM_WAIT, i915_gem_wait_ioctl, DRM_AUTH|DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_CONTEXT_CREATE, i915_gem_context_create_ioctl, DRM_RENDER_ALLOW),
=======
	DRM_IOCTL_DEF_DRV(I915_GEM_WAIT, i915_gem_wait_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_CONTEXT_CREATE_EXT, i915_gem_context_create_ioctl, DRM_RENDER_ALLOW),
>>>>>>> upstream/android-13
	DRM_IOCTL_DEF_DRV(I915_GEM_CONTEXT_DESTROY, i915_gem_context_destroy_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_REG_READ, i915_reg_read_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GET_RESET_STATS, i915_gem_context_reset_stats_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_USERPTR, i915_gem_userptr_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_CONTEXT_GETPARAM, i915_gem_context_getparam_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_CONTEXT_SETPARAM, i915_gem_context_setparam_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_PERF_OPEN, i915_perf_open_ioctl, DRM_RENDER_ALLOW),
<<<<<<< HEAD
	DRM_IOCTL_DEF_DRV(I915_PERF_ADD_CONFIG, i915_perf_add_config_ioctl, DRM_UNLOCKED|DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_PERF_REMOVE_CONFIG, i915_perf_remove_config_ioctl, DRM_UNLOCKED|DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_QUERY, i915_query_ioctl, DRM_UNLOCKED|DRM_RENDER_ALLOW),
};

static struct drm_driver driver = {
=======
	DRM_IOCTL_DEF_DRV(I915_PERF_ADD_CONFIG, i915_perf_add_config_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_PERF_REMOVE_CONFIG, i915_perf_remove_config_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_QUERY, i915_query_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_VM_CREATE, i915_gem_vm_create_ioctl, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(I915_GEM_VM_DESTROY, i915_gem_vm_destroy_ioctl, DRM_RENDER_ALLOW),
};

static const struct drm_driver driver = {
>>>>>>> upstream/android-13
	/* Don't use MTRRs here; the Xserver or userspace app should
	 * deal with them for Intel hardware.
	 */
	.driver_features =
<<<<<<< HEAD
	    DRIVER_HAVE_IRQ | DRIVER_IRQ_SHARED | DRIVER_GEM | DRIVER_PRIME |
	    DRIVER_RENDER | DRIVER_MODESET | DRIVER_ATOMIC | DRIVER_SYNCOBJ,
=======
	    DRIVER_GEM |
	    DRIVER_RENDER | DRIVER_MODESET | DRIVER_ATOMIC | DRIVER_SYNCOBJ |
	    DRIVER_SYNCOBJ_TIMELINE,
>>>>>>> upstream/android-13
	.release = i915_driver_release,
	.open = i915_driver_open,
	.lastclose = i915_driver_lastclose,
	.postclose = i915_driver_postclose,

<<<<<<< HEAD
	.gem_close_object = i915_gem_close_object,
	.gem_free_object_unlocked = i915_gem_free_object,
	.gem_vm_ops = &i915_gem_vm_ops,

	.prime_handle_to_fd = drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle = drm_gem_prime_fd_to_handle,
	.gem_prime_export = i915_gem_prime_export,
	.gem_prime_import = i915_gem_prime_import,

	.dumb_create = i915_gem_dumb_create,
	.dumb_map_offset = i915_gem_mmap_gtt,
=======
	.prime_handle_to_fd = drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle = drm_gem_prime_fd_to_handle,
	.gem_prime_import = i915_gem_prime_import,

	.dumb_create = i915_gem_dumb_create,
	.dumb_map_offset = i915_gem_dumb_mmap_offset,

>>>>>>> upstream/android-13
	.ioctls = i915_ioctls,
	.num_ioctls = ARRAY_SIZE(i915_ioctls),
	.fops = &i915_driver_fops,
	.name = DRIVER_NAME,
	.desc = DRIVER_DESC,
	.date = DRIVER_DATE,
	.major = DRIVER_MAJOR,
	.minor = DRIVER_MINOR,
	.patchlevel = DRIVER_PATCHLEVEL,
};
<<<<<<< HEAD

#if IS_ENABLED(CONFIG_DRM_I915_SELFTEST)
#include "selftests/mock_drm.c"
#endif
=======
>>>>>>> upstream/android-13
