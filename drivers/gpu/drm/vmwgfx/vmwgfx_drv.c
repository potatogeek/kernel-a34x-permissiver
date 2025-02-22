// SPDX-License-Identifier: GPL-2.0 OR MIT
/**************************************************************************
 *
 * Copyright 2009-2016 VMware, Inc., Palo Alto, CA., USA
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/console.h>

#include <drm/drmP.h>
#include "vmwgfx_drv.h"
#include "vmwgfx_binding.h"
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_object.h>
#include <drm/ttm/ttm_module.h>
#include <linux/dma_remapping.h>

#define VMWGFX_DRIVER_DESC "Linux drm driver for VMware graphics devices"
#define VMWGFX_CHIP_SVGAII 0
#define VMW_FB_RESERVATION 0
=======

#include <linux/console.h>
#include <linux/dma-mapping.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/mem_encrypt.h>

#include <drm/drm_aperture.h>
#include <drm/drm_drv.h>
#include <drm/drm_ioctl.h>
#include <drm/drm_sysfs.h>
#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_range_manager.h>
#include <drm/ttm/ttm_placement.h>
#include <generated/utsrelease.h>

#include "ttm_object.h"
#include "vmwgfx_binding.h"
#include "vmwgfx_devcaps.h"
#include "vmwgfx_drv.h"
#include "vmwgfx_mksstat.h"

#define VMWGFX_DRIVER_DESC "Linux drm driver for VMware graphics devices"
>>>>>>> upstream/android-13

#define VMW_MIN_INITIAL_WIDTH 800
#define VMW_MIN_INITIAL_HEIGHT 600

<<<<<<< HEAD
#ifndef VMWGFX_GIT_VERSION
#define VMWGFX_GIT_VERSION "Unknown"
#endif

#define VMWGFX_REPO "In Tree"


/**
=======
#define VMWGFX_VALIDATION_MEM_GRAN (16*PAGE_SIZE)


/*
>>>>>>> upstream/android-13
 * Fully encoded drm commands. Might move to vmw_drm.h
 */

#define DRM_IOCTL_VMW_GET_PARAM					\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_GET_PARAM,		\
		 struct drm_vmw_getparam_arg)
#define DRM_IOCTL_VMW_ALLOC_DMABUF				\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_ALLOC_DMABUF,	\
		union drm_vmw_alloc_dmabuf_arg)
#define DRM_IOCTL_VMW_UNREF_DMABUF				\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_UNREF_DMABUF,	\
		struct drm_vmw_unref_dmabuf_arg)
#define DRM_IOCTL_VMW_CURSOR_BYPASS				\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_CURSOR_BYPASS,	\
		 struct drm_vmw_cursor_bypass_arg)

#define DRM_IOCTL_VMW_CONTROL_STREAM				\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_CONTROL_STREAM,	\
		 struct drm_vmw_control_stream_arg)
#define DRM_IOCTL_VMW_CLAIM_STREAM				\
	DRM_IOR(DRM_COMMAND_BASE + DRM_VMW_CLAIM_STREAM,	\
		 struct drm_vmw_stream_arg)
#define DRM_IOCTL_VMW_UNREF_STREAM				\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_UNREF_STREAM,	\
		 struct drm_vmw_stream_arg)

#define DRM_IOCTL_VMW_CREATE_CONTEXT				\
	DRM_IOR(DRM_COMMAND_BASE + DRM_VMW_CREATE_CONTEXT,	\
		struct drm_vmw_context_arg)
#define DRM_IOCTL_VMW_UNREF_CONTEXT				\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_UNREF_CONTEXT,	\
		struct drm_vmw_context_arg)
#define DRM_IOCTL_VMW_CREATE_SURFACE				\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_CREATE_SURFACE,	\
		 union drm_vmw_surface_create_arg)
#define DRM_IOCTL_VMW_UNREF_SURFACE				\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_UNREF_SURFACE,	\
		 struct drm_vmw_surface_arg)
#define DRM_IOCTL_VMW_REF_SURFACE				\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_REF_SURFACE,	\
		 union drm_vmw_surface_reference_arg)
#define DRM_IOCTL_VMW_EXECBUF					\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_EXECBUF,		\
		struct drm_vmw_execbuf_arg)
#define DRM_IOCTL_VMW_GET_3D_CAP				\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_GET_3D_CAP,		\
		 struct drm_vmw_get_3d_cap_arg)
#define DRM_IOCTL_VMW_FENCE_WAIT				\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_FENCE_WAIT,		\
		 struct drm_vmw_fence_wait_arg)
#define DRM_IOCTL_VMW_FENCE_SIGNALED				\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_FENCE_SIGNALED,	\
		 struct drm_vmw_fence_signaled_arg)
#define DRM_IOCTL_VMW_FENCE_UNREF				\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_FENCE_UNREF,		\
		 struct drm_vmw_fence_arg)
#define DRM_IOCTL_VMW_FENCE_EVENT				\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_FENCE_EVENT,		\
		 struct drm_vmw_fence_event_arg)
#define DRM_IOCTL_VMW_PRESENT					\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_PRESENT,		\
		 struct drm_vmw_present_arg)
#define DRM_IOCTL_VMW_PRESENT_READBACK				\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_PRESENT_READBACK,	\
		 struct drm_vmw_present_readback_arg)
#define DRM_IOCTL_VMW_UPDATE_LAYOUT				\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_UPDATE_LAYOUT,	\
		 struct drm_vmw_update_layout_arg)
#define DRM_IOCTL_VMW_CREATE_SHADER				\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_CREATE_SHADER,	\
		 struct drm_vmw_shader_create_arg)
#define DRM_IOCTL_VMW_UNREF_SHADER				\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_UNREF_SHADER,	\
		 struct drm_vmw_shader_arg)
#define DRM_IOCTL_VMW_GB_SURFACE_CREATE				\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_GB_SURFACE_CREATE,	\
		 union drm_vmw_gb_surface_create_arg)
#define DRM_IOCTL_VMW_GB_SURFACE_REF				\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_GB_SURFACE_REF,	\
		 union drm_vmw_gb_surface_reference_arg)
#define DRM_IOCTL_VMW_SYNCCPU					\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_SYNCCPU,		\
		 struct drm_vmw_synccpu_arg)
#define DRM_IOCTL_VMW_CREATE_EXTENDED_CONTEXT			\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_CREATE_EXTENDED_CONTEXT,	\
		struct drm_vmw_context_arg)
#define DRM_IOCTL_VMW_GB_SURFACE_CREATE_EXT				\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_GB_SURFACE_CREATE_EXT,	\
		union drm_vmw_gb_surface_create_ext_arg)
#define DRM_IOCTL_VMW_GB_SURFACE_REF_EXT				\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_GB_SURFACE_REF_EXT,		\
		union drm_vmw_gb_surface_reference_ext_arg)
<<<<<<< HEAD

/**
 * The core DRM version of this macro doesn't account for
 * DRM_COMMAND_BASE.
 */

#define VMW_IOCTL_DEF(ioctl, func, flags) \
  [DRM_IOCTL_NR(DRM_IOCTL_##ioctl) - DRM_COMMAND_BASE] = {DRM_IOCTL_##ioctl, flags, func}

/**
=======
#define DRM_IOCTL_VMW_MSG						\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_MSG,			\
		struct drm_vmw_msg_arg)
#define DRM_IOCTL_VMW_MKSSTAT_RESET				\
	DRM_IO(DRM_COMMAND_BASE + DRM_VMW_MKSSTAT_RESET)
#define DRM_IOCTL_VMW_MKSSTAT_ADD				\
	DRM_IOWR(DRM_COMMAND_BASE + DRM_VMW_MKSSTAT_ADD,	\
		struct drm_vmw_mksstat_add_arg)
#define DRM_IOCTL_VMW_MKSSTAT_REMOVE				\
	DRM_IOW(DRM_COMMAND_BASE + DRM_VMW_MKSSTAT_REMOVE,	\
		struct drm_vmw_mksstat_remove_arg)

/*
>>>>>>> upstream/android-13
 * Ioctl definitions.
 */

static const struct drm_ioctl_desc vmw_ioctls[] = {
<<<<<<< HEAD
	VMW_IOCTL_DEF(VMW_GET_PARAM, vmw_getparam_ioctl,
		      DRM_AUTH | DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_ALLOC_DMABUF, vmw_bo_alloc_ioctl,
		      DRM_AUTH | DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_UNREF_DMABUF, vmw_bo_unref_ioctl,
		      DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_CURSOR_BYPASS,
		      vmw_kms_cursor_bypass_ioctl,
		      DRM_MASTER),

	VMW_IOCTL_DEF(VMW_CONTROL_STREAM, vmw_overlay_ioctl,
		      DRM_MASTER),
	VMW_IOCTL_DEF(VMW_CLAIM_STREAM, vmw_stream_claim_ioctl,
		      DRM_MASTER),
	VMW_IOCTL_DEF(VMW_UNREF_STREAM, vmw_stream_unref_ioctl,
		      DRM_MASTER),

	VMW_IOCTL_DEF(VMW_CREATE_CONTEXT, vmw_context_define_ioctl,
		      DRM_AUTH | DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_UNREF_CONTEXT, vmw_context_destroy_ioctl,
		      DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_CREATE_SURFACE, vmw_surface_define_ioctl,
		      DRM_AUTH | DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_UNREF_SURFACE, vmw_surface_destroy_ioctl,
		      DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_REF_SURFACE, vmw_surface_reference_ioctl,
		      DRM_AUTH | DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_EXECBUF, NULL, DRM_AUTH |
		      DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_FENCE_WAIT, vmw_fence_obj_wait_ioctl,
		      DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_FENCE_SIGNALED,
		      vmw_fence_obj_signaled_ioctl,
		      DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_FENCE_UNREF, vmw_fence_obj_unref_ioctl,
		      DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_FENCE_EVENT, vmw_fence_event_ioctl,
		      DRM_AUTH | DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_GET_3D_CAP, vmw_get_cap_3d_ioctl,
		      DRM_AUTH | DRM_RENDER_ALLOW),

	/* these allow direct access to the framebuffers mark as master only */
	VMW_IOCTL_DEF(VMW_PRESENT, vmw_present_ioctl,
		      DRM_MASTER | DRM_AUTH),
	VMW_IOCTL_DEF(VMW_PRESENT_READBACK,
		      vmw_present_readback_ioctl,
		      DRM_MASTER | DRM_AUTH),
=======
	DRM_IOCTL_DEF_DRV(VMW_GET_PARAM, vmw_getparam_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_ALLOC_DMABUF, vmw_bo_alloc_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_UNREF_DMABUF, vmw_bo_unref_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_CURSOR_BYPASS,
			  vmw_kms_cursor_bypass_ioctl,
			  DRM_MASTER),

	DRM_IOCTL_DEF_DRV(VMW_CONTROL_STREAM, vmw_overlay_ioctl,
			  DRM_MASTER),
	DRM_IOCTL_DEF_DRV(VMW_CLAIM_STREAM, vmw_stream_claim_ioctl,
			  DRM_MASTER),
	DRM_IOCTL_DEF_DRV(VMW_UNREF_STREAM, vmw_stream_unref_ioctl,
			  DRM_MASTER),

	DRM_IOCTL_DEF_DRV(VMW_CREATE_CONTEXT, vmw_context_define_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_UNREF_CONTEXT, vmw_context_destroy_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_CREATE_SURFACE, vmw_surface_define_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_UNREF_SURFACE, vmw_surface_destroy_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_REF_SURFACE, vmw_surface_reference_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_EXECBUF, vmw_execbuf_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_FENCE_WAIT, vmw_fence_obj_wait_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_FENCE_SIGNALED,
			  vmw_fence_obj_signaled_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_FENCE_UNREF, vmw_fence_obj_unref_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_FENCE_EVENT, vmw_fence_event_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_GET_3D_CAP, vmw_get_cap_3d_ioctl,
			  DRM_RENDER_ALLOW),

	/* these allow direct access to the framebuffers mark as master only */
	DRM_IOCTL_DEF_DRV(VMW_PRESENT, vmw_present_ioctl,
			  DRM_MASTER | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(VMW_PRESENT_READBACK,
			  vmw_present_readback_ioctl,
			  DRM_MASTER | DRM_AUTH),
>>>>>>> upstream/android-13
	/*
	 * The permissions of the below ioctl are overridden in
	 * vmw_generic_ioctl(). We require either
	 * DRM_MASTER or capable(CAP_SYS_ADMIN).
	 */
<<<<<<< HEAD
	VMW_IOCTL_DEF(VMW_UPDATE_LAYOUT,
		      vmw_kms_update_layout_ioctl,
		      DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_CREATE_SHADER,
		      vmw_shader_define_ioctl,
		      DRM_AUTH | DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_UNREF_SHADER,
		      vmw_shader_destroy_ioctl,
		      DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_GB_SURFACE_CREATE,
		      vmw_gb_surface_define_ioctl,
		      DRM_AUTH | DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_GB_SURFACE_REF,
		      vmw_gb_surface_reference_ioctl,
		      DRM_AUTH | DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_SYNCCPU,
		      vmw_user_bo_synccpu_ioctl,
		      DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_CREATE_EXTENDED_CONTEXT,
		      vmw_extended_context_define_ioctl,
		      DRM_AUTH | DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_GB_SURFACE_CREATE_EXT,
		      vmw_gb_surface_define_ext_ioctl,
		      DRM_AUTH | DRM_RENDER_ALLOW),
	VMW_IOCTL_DEF(VMW_GB_SURFACE_REF_EXT,
		      vmw_gb_surface_reference_ext_ioctl,
		      DRM_AUTH | DRM_RENDER_ALLOW),
};

static const struct pci_device_id vmw_pci_id_list[] = {
	{0x15ad, 0x0405, PCI_ANY_ID, PCI_ANY_ID, 0, 0, VMWGFX_CHIP_SVGAII},
	{0, 0, 0}
=======
	DRM_IOCTL_DEF_DRV(VMW_UPDATE_LAYOUT,
			  vmw_kms_update_layout_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_CREATE_SHADER,
			  vmw_shader_define_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_UNREF_SHADER,
			  vmw_shader_destroy_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_GB_SURFACE_CREATE,
			  vmw_gb_surface_define_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_GB_SURFACE_REF,
			  vmw_gb_surface_reference_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_SYNCCPU,
			  vmw_user_bo_synccpu_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_CREATE_EXTENDED_CONTEXT,
			  vmw_extended_context_define_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_GB_SURFACE_CREATE_EXT,
			  vmw_gb_surface_define_ext_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_GB_SURFACE_REF_EXT,
			  vmw_gb_surface_reference_ext_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_MSG,
			  vmw_msg_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_MKSSTAT_RESET,
			  vmw_mksstat_reset_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_MKSSTAT_ADD,
			  vmw_mksstat_add_ioctl,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(VMW_MKSSTAT_REMOVE,
			  vmw_mksstat_remove_ioctl,
			  DRM_RENDER_ALLOW),
};

static const struct pci_device_id vmw_pci_id_list[] = {
	{ PCI_DEVICE(0x15ad, VMWGFX_PCI_ID_SVGA2) },
	{ PCI_DEVICE(0x15ad, VMWGFX_PCI_ID_SVGA3) },
	{ }
>>>>>>> upstream/android-13
};
MODULE_DEVICE_TABLE(pci, vmw_pci_id_list);

static int enable_fbdev = IS_ENABLED(CONFIG_DRM_VMWGFX_FBCON);
<<<<<<< HEAD
static int vmw_force_iommu;
=======
>>>>>>> upstream/android-13
static int vmw_restrict_iommu;
static int vmw_force_coherent;
static int vmw_restrict_dma_mask;
static int vmw_assume_16bpp;

static int vmw_probe(struct pci_dev *, const struct pci_device_id *);
<<<<<<< HEAD
static void vmw_master_init(struct vmw_master *);
=======
>>>>>>> upstream/android-13
static int vmwgfx_pm_notifier(struct notifier_block *nb, unsigned long val,
			      void *ptr);

MODULE_PARM_DESC(enable_fbdev, "Enable vmwgfx fbdev");
module_param_named(enable_fbdev, enable_fbdev, int, 0600);
<<<<<<< HEAD
MODULE_PARM_DESC(force_dma_api, "Force using the DMA API for TTM pages");
module_param_named(force_dma_api, vmw_force_iommu, int, 0600);
=======
>>>>>>> upstream/android-13
MODULE_PARM_DESC(restrict_iommu, "Try to limit IOMMU usage for TTM pages");
module_param_named(restrict_iommu, vmw_restrict_iommu, int, 0600);
MODULE_PARM_DESC(force_coherent, "Force coherent TTM pages");
module_param_named(force_coherent, vmw_force_coherent, int, 0600);
MODULE_PARM_DESC(restrict_dma_mask, "Restrict DMA mask to 44 bits with IOMMU");
module_param_named(restrict_dma_mask, vmw_restrict_dma_mask, int, 0600);
MODULE_PARM_DESC(assume_16bpp, "Assume 16-bpp when filtering modes");
module_param_named(assume_16bpp, vmw_assume_16bpp, int, 0600);


<<<<<<< HEAD
static void vmw_print_capabilities2(uint32_t capabilities2)
{
	DRM_INFO("Capabilities2:\n");
	if (capabilities2 & SVGA_CAP2_GROW_OTABLE)
		DRM_INFO("  Grow oTable.\n");
	if (capabilities2 & SVGA_CAP2_INTRA_SURFACE_COPY)
		DRM_INFO("  IntraSurface copy.\n");
}

static void vmw_print_capabilities(uint32_t capabilities)
{
	DRM_INFO("Capabilities:\n");
	if (capabilities & SVGA_CAP_RECT_COPY)
		DRM_INFO("  Rect copy.\n");
	if (capabilities & SVGA_CAP_CURSOR)
		DRM_INFO("  Cursor.\n");
	if (capabilities & SVGA_CAP_CURSOR_BYPASS)
		DRM_INFO("  Cursor bypass.\n");
	if (capabilities & SVGA_CAP_CURSOR_BYPASS_2)
		DRM_INFO("  Cursor bypass 2.\n");
	if (capabilities & SVGA_CAP_8BIT_EMULATION)
		DRM_INFO("  8bit emulation.\n");
	if (capabilities & SVGA_CAP_ALPHA_CURSOR)
		DRM_INFO("  Alpha cursor.\n");
	if (capabilities & SVGA_CAP_3D)
		DRM_INFO("  3D.\n");
	if (capabilities & SVGA_CAP_EXTENDED_FIFO)
		DRM_INFO("  Extended Fifo.\n");
	if (capabilities & SVGA_CAP_MULTIMON)
		DRM_INFO("  Multimon.\n");
	if (capabilities & SVGA_CAP_PITCHLOCK)
		DRM_INFO("  Pitchlock.\n");
	if (capabilities & SVGA_CAP_IRQMASK)
		DRM_INFO("  Irq mask.\n");
	if (capabilities & SVGA_CAP_DISPLAY_TOPOLOGY)
		DRM_INFO("  Display Topology.\n");
	if (capabilities & SVGA_CAP_GMR)
		DRM_INFO("  GMR.\n");
	if (capabilities & SVGA_CAP_TRACES)
		DRM_INFO("  Traces.\n");
	if (capabilities & SVGA_CAP_GMR2)
		DRM_INFO("  GMR2.\n");
	if (capabilities & SVGA_CAP_SCREEN_OBJECT_2)
		DRM_INFO("  Screen Object 2.\n");
	if (capabilities & SVGA_CAP_COMMAND_BUFFERS)
		DRM_INFO("  Command Buffers.\n");
	if (capabilities & SVGA_CAP_CMD_BUFFERS_2)
		DRM_INFO("  Command Buffers 2.\n");
	if (capabilities & SVGA_CAP_GBOBJECTS)
		DRM_INFO("  Guest Backed Resources.\n");
	if (capabilities & SVGA_CAP_DX)
		DRM_INFO("  DX Features.\n");
	if (capabilities & SVGA_CAP_HP_CMD_QUEUE)
		DRM_INFO("  HP Command Queue.\n");
=======
struct bitmap_name {
	uint32 value;
	const char *name;
};

static const struct bitmap_name cap1_names[] = {
	{ SVGA_CAP_RECT_COPY, "rect copy" },
	{ SVGA_CAP_CURSOR, "cursor" },
	{ SVGA_CAP_CURSOR_BYPASS, "cursor bypass" },
	{ SVGA_CAP_CURSOR_BYPASS_2, "cursor bypass 2" },
	{ SVGA_CAP_8BIT_EMULATION, "8bit emulation" },
	{ SVGA_CAP_ALPHA_CURSOR, "alpha cursor" },
	{ SVGA_CAP_3D, "3D" },
	{ SVGA_CAP_EXTENDED_FIFO, "extended fifo" },
	{ SVGA_CAP_MULTIMON, "multimon" },
	{ SVGA_CAP_PITCHLOCK, "pitchlock" },
	{ SVGA_CAP_IRQMASK, "irq mask" },
	{ SVGA_CAP_DISPLAY_TOPOLOGY, "display topology" },
	{ SVGA_CAP_GMR, "gmr" },
	{ SVGA_CAP_TRACES, "traces" },
	{ SVGA_CAP_GMR2, "gmr2" },
	{ SVGA_CAP_SCREEN_OBJECT_2, "screen object 2" },
	{ SVGA_CAP_COMMAND_BUFFERS, "command buffers" },
	{ SVGA_CAP_CMD_BUFFERS_2, "command buffers 2" },
	{ SVGA_CAP_GBOBJECTS, "gbobject" },
	{ SVGA_CAP_DX, "dx" },
	{ SVGA_CAP_HP_CMD_QUEUE, "hp cmd queue" },
	{ SVGA_CAP_NO_BB_RESTRICTION, "no bb restriction" },
	{ SVGA_CAP_CAP2_REGISTER, "cap2 register" },
};


static const struct bitmap_name cap2_names[] = {
	{ SVGA_CAP2_GROW_OTABLE, "grow otable" },
	{ SVGA_CAP2_INTRA_SURFACE_COPY, "intra surface copy" },
	{ SVGA_CAP2_DX2, "dx2" },
	{ SVGA_CAP2_GB_MEMSIZE_2, "gb memsize 2" },
	{ SVGA_CAP2_SCREENDMA_REG, "screendma reg" },
	{ SVGA_CAP2_OTABLE_PTDEPTH_2, "otable ptdepth2" },
	{ SVGA_CAP2_NON_MS_TO_MS_STRETCHBLT, "non ms to ms stretchblt" },
	{ SVGA_CAP2_CURSOR_MOB, "cursor mob" },
	{ SVGA_CAP2_MSHINT, "mshint" },
	{ SVGA_CAP2_CB_MAX_SIZE_4MB, "cb max size 4mb" },
	{ SVGA_CAP2_DX3, "dx3" },
	{ SVGA_CAP2_FRAME_TYPE, "frame type" },
	{ SVGA_CAP2_COTABLE_COPY, "cotable copy" },
	{ SVGA_CAP2_TRACE_FULL_FB, "trace full fb" },
	{ SVGA_CAP2_EXTRA_REGS, "extra regs" },
	{ SVGA_CAP2_LO_STAGING, "lo staging" },
};

static void vmw_print_bitmap(struct drm_device *drm,
			     const char *prefix, uint32_t bitmap,
			     const struct bitmap_name *bnames,
			     uint32_t num_names)
{
	char buf[512];
	uint32_t i;
	uint32_t offset = 0;
	for (i = 0; i < num_names; ++i) {
		if ((bitmap & bnames[i].value) != 0) {
			offset += snprintf(buf + offset,
					   ARRAY_SIZE(buf) - offset,
					   "%s, ", bnames[i].name);
			bitmap &= ~bnames[i].value;
		}
	}

	drm_info(drm, "%s: %s\n", prefix, buf);
	if (bitmap != 0)
		drm_dbg(drm, "%s: unknown enums: %x\n", prefix, bitmap);
}


static void vmw_print_sm_type(struct vmw_private *dev_priv)
{
	static const char *names[] = {
		[VMW_SM_LEGACY] = "Legacy",
		[VMW_SM_4] = "SM4",
		[VMW_SM_4_1] = "SM4_1",
		[VMW_SM_5] = "SM_5",
		[VMW_SM_MAX] = "Invalid"
	};
	BUILD_BUG_ON(ARRAY_SIZE(names) != (VMW_SM_MAX + 1));
	drm_info(&dev_priv->drm, "Available shader model: %s.\n",
		 names[dev_priv->sm_type]);
>>>>>>> upstream/android-13
}

/**
 * vmw_dummy_query_bo_create - create a bo to hold a dummy query result
 *
 * @dev_priv: A device private structure.
 *
 * This function creates a small buffer object that holds the query
 * result for dummy queries emitted as query barriers.
 * The function will then map the first page and initialize a pending
 * occlusion query result structure, Finally it will unmap the buffer.
 * No interruptible waits are done within this function.
 *
 * Returns an error if bo creation or initialization fails.
 */
static int vmw_dummy_query_bo_create(struct vmw_private *dev_priv)
{
	int ret;
	struct vmw_buffer_object *vbo;
	struct ttm_bo_kmap_obj map;
	volatile SVGA3dQueryResult *result;
	bool dummy;

	/*
	 * Create the vbo as pinned, so that a tryreserve will
	 * immediately succeed. This is because we're the only
	 * user of the bo currently.
	 */
	vbo = kzalloc(sizeof(*vbo), GFP_KERNEL);
	if (!vbo)
		return -ENOMEM;

	ret = vmw_bo_init(dev_priv, vbo, PAGE_SIZE,
<<<<<<< HEAD
			  &vmw_sys_ne_placement, false,
=======
			  &vmw_sys_placement, false, true,
>>>>>>> upstream/android-13
			  &vmw_bo_bo_free);
	if (unlikely(ret != 0))
		return ret;

	ret = ttm_bo_reserve(&vbo->base, false, true, NULL);
	BUG_ON(ret != 0);
	vmw_bo_pin_reserved(vbo, true);

	ret = ttm_bo_kmap(&vbo->base, 0, 1, &map);
	if (likely(ret == 0)) {
		result = ttm_kmap_obj_virtual(&map, &dummy);
		result->totalSize = sizeof(*result);
		result->state = SVGA3D_QUERYSTATE_PENDING;
		result->result32 = 0xff;
		ttm_bo_kunmap(&map);
	}
	vmw_bo_pin_reserved(vbo, false);
	ttm_bo_unreserve(&vbo->base);

	if (unlikely(ret != 0)) {
		DRM_ERROR("Dummy query buffer map failed.\n");
		vmw_bo_unreference(&vbo);
	} else
		dev_priv->dummy_query_bo = vbo;

	return ret;
}

<<<<<<< HEAD
=======
static int vmw_device_init(struct vmw_private *dev_priv)
{
	bool uses_fb_traces = false;

	dev_priv->enable_state = vmw_read(dev_priv, SVGA_REG_ENABLE);
	dev_priv->config_done_state = vmw_read(dev_priv, SVGA_REG_CONFIG_DONE);
	dev_priv->traces_state = vmw_read(dev_priv, SVGA_REG_TRACES);

	vmw_write(dev_priv, SVGA_REG_ENABLE, SVGA_REG_ENABLE_ENABLE |
		  SVGA_REG_ENABLE_HIDE);

	uses_fb_traces = !vmw_cmd_supported(dev_priv) &&
			 (dev_priv->capabilities & SVGA_CAP_TRACES) != 0;

	vmw_write(dev_priv, SVGA_REG_TRACES, uses_fb_traces);
	dev_priv->fifo = vmw_fifo_create(dev_priv);
	if (IS_ERR(dev_priv->fifo)) {
		int err = PTR_ERR(dev_priv->fifo);
		dev_priv->fifo = NULL;
		return err;
	} else if (!dev_priv->fifo) {
		vmw_write(dev_priv, SVGA_REG_CONFIG_DONE, 1);
	}

	dev_priv->last_read_seqno = vmw_fence_read(dev_priv);
	atomic_set(&dev_priv->marker_seq, dev_priv->last_read_seqno);
	return 0;
}

static void vmw_device_fini(struct vmw_private *vmw)
{
	/*
	 * Legacy sync
	 */
	vmw_write(vmw, SVGA_REG_SYNC, SVGA_SYNC_GENERIC);
	while (vmw_read(vmw, SVGA_REG_BUSY) != 0)
		;

	vmw->last_read_seqno = vmw_fence_read(vmw);

	vmw_write(vmw, SVGA_REG_CONFIG_DONE,
		  vmw->config_done_state);
	vmw_write(vmw, SVGA_REG_ENABLE,
		  vmw->enable_state);
	vmw_write(vmw, SVGA_REG_TRACES,
		  vmw->traces_state);

	vmw_fifo_destroy(vmw);
}

>>>>>>> upstream/android-13
/**
 * vmw_request_device_late - Perform late device setup
 *
 * @dev_priv: Pointer to device private.
 *
 * This function performs setup of otables and enables large command
 * buffer submission. These tasks are split out to a separate function
 * because it reverts vmw_release_device_early and is intended to be used
 * by an error path in the hibernation code.
 */
static int vmw_request_device_late(struct vmw_private *dev_priv)
{
	int ret;

	if (dev_priv->has_mob) {
		ret = vmw_otables_setup(dev_priv);
		if (unlikely(ret != 0)) {
			DRM_ERROR("Unable to initialize "
				  "guest Memory OBjects.\n");
			return ret;
		}
	}

	if (dev_priv->cman) {
<<<<<<< HEAD
		ret = vmw_cmdbuf_set_pool_size(dev_priv->cman,
					       256*4096, 2*4096);
=======
		ret = vmw_cmdbuf_set_pool_size(dev_priv->cman, 256*4096);
>>>>>>> upstream/android-13
		if (ret) {
			struct vmw_cmdbuf_man *man = dev_priv->cman;

			dev_priv->cman = NULL;
			vmw_cmdbuf_man_destroy(man);
		}
	}

	return 0;
}

static int vmw_request_device(struct vmw_private *dev_priv)
{
	int ret;

<<<<<<< HEAD
	ret = vmw_fifo_init(dev_priv, &dev_priv->fifo);
	if (unlikely(ret != 0)) {
		DRM_ERROR("Unable to initialize FIFO.\n");
=======
	ret = vmw_device_init(dev_priv);
	if (unlikely(ret != 0)) {
		DRM_ERROR("Unable to initialize the device.\n");
>>>>>>> upstream/android-13
		return ret;
	}
	vmw_fence_fifo_up(dev_priv->fman);
	dev_priv->cman = vmw_cmdbuf_man_create(dev_priv);
	if (IS_ERR(dev_priv->cman)) {
		dev_priv->cman = NULL;
<<<<<<< HEAD
		dev_priv->has_dx = false;
=======
		dev_priv->sm_type = VMW_SM_LEGACY;
>>>>>>> upstream/android-13
	}

	ret = vmw_request_device_late(dev_priv);
	if (ret)
		goto out_no_mob;

	ret = vmw_dummy_query_bo_create(dev_priv);
	if (unlikely(ret != 0))
		goto out_no_query_bo;

	return 0;

out_no_query_bo:
	if (dev_priv->cman)
		vmw_cmdbuf_remove_pool(dev_priv->cman);
	if (dev_priv->has_mob) {
<<<<<<< HEAD
		(void) ttm_bo_evict_mm(&dev_priv->bdev, VMW_PL_MOB);
=======
		struct ttm_resource_manager *man;

		man = ttm_manager_type(&dev_priv->bdev, VMW_PL_MOB);
		ttm_resource_manager_evict_all(&dev_priv->bdev, man);
>>>>>>> upstream/android-13
		vmw_otables_takedown(dev_priv);
	}
	if (dev_priv->cman)
		vmw_cmdbuf_man_destroy(dev_priv->cman);
out_no_mob:
	vmw_fence_fifo_down(dev_priv->fman);
<<<<<<< HEAD
	vmw_fifo_release(dev_priv, &dev_priv->fifo);
=======
	vmw_device_fini(dev_priv);
>>>>>>> upstream/android-13
	return ret;
}

/**
 * vmw_release_device_early - Early part of fifo takedown.
 *
 * @dev_priv: Pointer to device private struct.
 *
 * This is the first part of command submission takedown, to be called before
 * buffer management is taken down.
 */
static void vmw_release_device_early(struct vmw_private *dev_priv)
{
	/*
	 * Previous destructions should've released
	 * the pinned bo.
	 */

	BUG_ON(dev_priv->pinned_bo != NULL);

	vmw_bo_unreference(&dev_priv->dummy_query_bo);
	if (dev_priv->cman)
		vmw_cmdbuf_remove_pool(dev_priv->cman);

	if (dev_priv->has_mob) {
<<<<<<< HEAD
		ttm_bo_evict_mm(&dev_priv->bdev, VMW_PL_MOB);
=======
		struct ttm_resource_manager *man;

		man = ttm_manager_type(&dev_priv->bdev, VMW_PL_MOB);
		ttm_resource_manager_evict_all(&dev_priv->bdev, man);
>>>>>>> upstream/android-13
		vmw_otables_takedown(dev_priv);
	}
}

/**
 * vmw_release_device_late - Late part of fifo takedown.
 *
 * @dev_priv: Pointer to device private struct.
 *
 * This is the last part of the command submission takedown, to be called when
 * command submission is no longer needed. It may wait on pending fences.
 */
static void vmw_release_device_late(struct vmw_private *dev_priv)
{
	vmw_fence_fifo_down(dev_priv->fman);
	if (dev_priv->cman)
		vmw_cmdbuf_man_destroy(dev_priv->cman);

<<<<<<< HEAD
	vmw_fifo_release(dev_priv, &dev_priv->fifo);
}

/**
=======
	vmw_device_fini(dev_priv);
}

/*
>>>>>>> upstream/android-13
 * Sets the initial_[width|height] fields on the given vmw_private.
 *
 * It does so by reading SVGA_REG_[WIDTH|HEIGHT] regs and then
 * clamping the value to fb_max_[width|height] fields and the
 * VMW_MIN_INITIAL_[WIDTH|HEIGHT].
 * If the values appear to be invalid, set them to
 * VMW_MIN_INITIAL_[WIDTH|HEIGHT].
 */
static void vmw_get_initial_size(struct vmw_private *dev_priv)
{
	uint32_t width;
	uint32_t height;

	width = vmw_read(dev_priv, SVGA_REG_WIDTH);
	height = vmw_read(dev_priv, SVGA_REG_HEIGHT);

	width = max_t(uint32_t, width, VMW_MIN_INITIAL_WIDTH);
	height = max_t(uint32_t, height, VMW_MIN_INITIAL_HEIGHT);

	if (width > dev_priv->fb_max_width ||
	    height > dev_priv->fb_max_height) {

		/*
		 * This is a host error and shouldn't occur.
		 */

		width = VMW_MIN_INITIAL_WIDTH;
		height = VMW_MIN_INITIAL_HEIGHT;
	}

	dev_priv->initial_width = width;
	dev_priv->initial_height = height;
}

/**
 * vmw_dma_select_mode - Determine how DMA mappings should be set up for this
 * system.
 *
 * @dev_priv: Pointer to a struct vmw_private
 *
<<<<<<< HEAD
 * This functions tries to determine the IOMMU setup and what actions
 * need to be taken by the driver to make system pages visible to the
 * device.
=======
 * This functions tries to determine what actions need to be taken by the
 * driver to make system pages visible to the device.
>>>>>>> upstream/android-13
 * If this function decides that DMA is not possible, it returns -EINVAL.
 * The driver may then try to disable features of the device that require
 * DMA.
 */
static int vmw_dma_select_mode(struct vmw_private *dev_priv)
{
	static const char *names[vmw_dma_map_max] = {
<<<<<<< HEAD
		[vmw_dma_phys] = "Using physical TTM page addresses.",
		[vmw_dma_alloc_coherent] = "Using coherent TTM pages.",
		[vmw_dma_map_populate] = "Keeping DMA mappings.",
		[vmw_dma_map_bind] = "Giving up DMA mappings early."};
#ifdef CONFIG_X86
	const struct dma_map_ops *dma_ops = get_dma_ops(dev_priv->dev->dev);

#ifdef CONFIG_INTEL_IOMMU
	if (intel_iommu_enabled) {
		dev_priv->map_mode = vmw_dma_map_populate;
		goto out_fixup;
	}
#endif

	if (!(vmw_force_iommu || vmw_force_coherent)) {
		dev_priv->map_mode = vmw_dma_phys;
		DRM_INFO("DMA map mode: %s\n", names[dev_priv->map_mode]);
		return 0;
	}

	dev_priv->map_mode = vmw_dma_map_populate;

	if (dma_ops->sync_single_for_cpu)
		dev_priv->map_mode = vmw_dma_alloc_coherent;
#ifdef CONFIG_SWIOTLB
	if (swiotlb_nr_tbl() == 0)
		dev_priv->map_mode = vmw_dma_map_populate;
#endif

#ifdef CONFIG_INTEL_IOMMU
out_fixup:
#endif
	if (dev_priv->map_mode == vmw_dma_map_populate &&
	    vmw_restrict_iommu)
		dev_priv->map_mode = vmw_dma_map_bind;

	if (vmw_force_coherent)
		dev_priv->map_mode = vmw_dma_alloc_coherent;

#if !defined(CONFIG_SWIOTLB) && !defined(CONFIG_INTEL_IOMMU)
	/*
	 * No coherent page pool
	 */
	if (dev_priv->map_mode == vmw_dma_alloc_coherent)
		return -EINVAL;
#endif

#else /* CONFIG_X86 */
	dev_priv->map_mode = vmw_dma_map_populate;
#endif /* CONFIG_X86 */

	DRM_INFO("DMA map mode: %s\n", names[dev_priv->map_mode]);

=======
		[vmw_dma_alloc_coherent] = "Using coherent TTM pages.",
		[vmw_dma_map_populate] = "Caching DMA mappings.",
		[vmw_dma_map_bind] = "Giving up DMA mappings early."};

	/* TTM currently doesn't fully support SEV encryption. */
	if (mem_encrypt_active())
		return -EINVAL;

	if (vmw_force_coherent)
		dev_priv->map_mode = vmw_dma_alloc_coherent;
	else if (vmw_restrict_iommu)
		dev_priv->map_mode = vmw_dma_map_bind;
	else
		dev_priv->map_mode = vmw_dma_map_populate;

	drm_info(&dev_priv->drm,
		 "DMA map mode: %s\n", names[dev_priv->map_mode]);
>>>>>>> upstream/android-13
	return 0;
}

/**
 * vmw_dma_masks - set required page- and dma masks
 *
<<<<<<< HEAD
 * @dev: Pointer to struct drm-device
=======
 * @dev_priv: Pointer to struct drm-device
>>>>>>> upstream/android-13
 *
 * With 32-bit we can only handle 32 bit PFNs. Optionally set that
 * restriction also for 64-bit systems.
 */
<<<<<<< HEAD
#ifdef CONFIG_INTEL_IOMMU
static int vmw_dma_masks(struct vmw_private *dev_priv)
{
	struct drm_device *dev = dev_priv->dev;
	int ret = 0;

	ret = dma_set_mask_and_coherent(dev->dev, DMA_BIT_MASK(64));
	if (dev_priv->map_mode != vmw_dma_phys &&
	    (sizeof(unsigned long) == 4 || vmw_restrict_dma_mask)) {
		DRM_INFO("Restricting DMA addresses to 44 bits.\n");
=======
static int vmw_dma_masks(struct vmw_private *dev_priv)
{
	struct drm_device *dev = &dev_priv->drm;
	int ret = 0;

	ret = dma_set_mask_and_coherent(dev->dev, DMA_BIT_MASK(64));
	if (sizeof(unsigned long) == 4 || vmw_restrict_dma_mask) {
		drm_info(&dev_priv->drm,
			 "Restricting DMA addresses to 44 bits.\n");
>>>>>>> upstream/android-13
		return dma_set_mask_and_coherent(dev->dev, DMA_BIT_MASK(44));
	}

	return ret;
}
<<<<<<< HEAD
#else
static int vmw_dma_masks(struct vmw_private *dev_priv)
{
	return 0;
}
#endif

static int vmw_driver_load(struct drm_device *dev, unsigned long chipset)
{
	struct vmw_private *dev_priv;
	int ret;
	uint32_t svga_id;
	enum vmw_res_type i;
	bool refuse_dma = false;
	char host_log[100] = {0};

	dev_priv = kzalloc(sizeof(*dev_priv), GFP_KERNEL);
	if (unlikely(!dev_priv)) {
		DRM_ERROR("Failed allocating a device private struct.\n");
		return -ENOMEM;
	}

	pci_set_master(dev->pdev);

	dev_priv->dev = dev;
	dev_priv->vmw_chipset = chipset;
	dev_priv->last_read_seqno = (uint32_t) -100;
	mutex_init(&dev_priv->cmdbuf_mutex);
	mutex_init(&dev_priv->release_mutex);
	mutex_init(&dev_priv->binding_mutex);
	mutex_init(&dev_priv->requested_layout_mutex);
	mutex_init(&dev_priv->global_kms_state_mutex);
	rwlock_init(&dev_priv->resource_lock);
	ttm_lock_init(&dev_priv->reservation_sem);
	spin_lock_init(&dev_priv->hw_lock);
	spin_lock_init(&dev_priv->waiter_lock);
	spin_lock_init(&dev_priv->cap_lock);
	spin_lock_init(&dev_priv->svga_lock);
	spin_lock_init(&dev_priv->cursor_lock);

	for (i = vmw_res_context; i < vmw_res_max; ++i) {
		idr_init(&dev_priv->res_idr[i]);
		INIT_LIST_HEAD(&dev_priv->res_lru[i]);
	}

	mutex_init(&dev_priv->init_mutex);
=======

static int vmw_vram_manager_init(struct vmw_private *dev_priv)
{
	int ret;
	ret = ttm_range_man_init(&dev_priv->bdev, TTM_PL_VRAM, false,
				 dev_priv->vram_size >> PAGE_SHIFT);
	ttm_resource_manager_set_used(ttm_manager_type(&dev_priv->bdev, TTM_PL_VRAM), false);
	return ret;
}

static void vmw_vram_manager_fini(struct vmw_private *dev_priv)
{
	ttm_range_man_fini(&dev_priv->bdev, TTM_PL_VRAM);
}

static int vmw_setup_pci_resources(struct vmw_private *dev,
				   u32 pci_id)
{
	resource_size_t rmmio_start;
	resource_size_t rmmio_size;
	resource_size_t fifo_start;
	resource_size_t fifo_size;
	int ret;
	struct pci_dev *pdev = to_pci_dev(dev->drm.dev);

	pci_set_master(pdev);

	ret = pci_request_regions(pdev, "vmwgfx probe");
	if (ret)
		return ret;

	dev->pci_id = pci_id;
	if (pci_id == VMWGFX_PCI_ID_SVGA3) {
		rmmio_start = pci_resource_start(pdev, 0);
		rmmio_size = pci_resource_len(pdev, 0);
		dev->vram_start = pci_resource_start(pdev, 2);
		dev->vram_size = pci_resource_len(pdev, 2);

		drm_info(&dev->drm,
			"Register MMIO at 0x%pa size is %llu kiB\n",
			 &rmmio_start, (uint64_t)rmmio_size / 1024);
		dev->rmmio = devm_ioremap(dev->drm.dev,
					  rmmio_start,
					  rmmio_size);
		if (!dev->rmmio) {
			drm_err(&dev->drm,
				"Failed mapping registers mmio memory.\n");
			pci_release_regions(pdev);
			return -ENOMEM;
		}
	} else if (pci_id == VMWGFX_PCI_ID_SVGA2) {
		dev->io_start = pci_resource_start(pdev, 0);
		dev->vram_start = pci_resource_start(pdev, 1);
		dev->vram_size = pci_resource_len(pdev, 1);
		fifo_start = pci_resource_start(pdev, 2);
		fifo_size = pci_resource_len(pdev, 2);

		drm_info(&dev->drm,
			 "FIFO at %pa size is %llu kiB\n",
			 &fifo_start, (uint64_t)fifo_size / 1024);
		dev->fifo_mem = devm_memremap(dev->drm.dev,
					      fifo_start,
					      fifo_size,
					      MEMREMAP_WB);

		if (IS_ERR(dev->fifo_mem)) {
			drm_err(&dev->drm,
				  "Failed mapping FIFO memory.\n");
			pci_release_regions(pdev);
			return PTR_ERR(dev->fifo_mem);
		}
	} else {
		pci_release_regions(pdev);
		return -EINVAL;
	}

	/*
	 * This is approximate size of the vram, the exact size will only
	 * be known after we read SVGA_REG_VRAM_SIZE. The PCI resource
	 * size will be equal to or bigger than the size reported by
	 * SVGA_REG_VRAM_SIZE.
	 */
	drm_info(&dev->drm,
		 "VRAM at %pa size is %llu kiB\n",
		 &dev->vram_start, (uint64_t)dev->vram_size / 1024);

	return 0;
}

static int vmw_detect_version(struct vmw_private *dev)
{
	uint32_t svga_id;

	vmw_write(dev, SVGA_REG_ID, vmw_is_svga_v3(dev) ?
			  SVGA_ID_3 : SVGA_ID_2);
	svga_id = vmw_read(dev, SVGA_REG_ID);
	if (svga_id != SVGA_ID_2 && svga_id != SVGA_ID_3) {
		drm_err(&dev->drm,
			"Unsupported SVGA ID 0x%x on chipset 0x%x\n",
			svga_id, dev->pci_id);
		return -ENOSYS;
	}
	BUG_ON(vmw_is_svga_v3(dev) && (svga_id != SVGA_ID_3));
	drm_info(&dev->drm,
		 "Running on SVGA version %d.\n", (svga_id & 0xff));
	return 0;
}

static int vmw_driver_load(struct vmw_private *dev_priv, u32 pci_id)
{
	int ret;
	enum vmw_res_type i;
	bool refuse_dma = false;
	struct pci_dev *pdev = to_pci_dev(dev_priv->drm.dev);

	dev_priv->drm.dev_private = dev_priv;

	mutex_init(&dev_priv->cmdbuf_mutex);
	mutex_init(&dev_priv->binding_mutex);
	spin_lock_init(&dev_priv->resource_lock);
	spin_lock_init(&dev_priv->hw_lock);
	spin_lock_init(&dev_priv->waiter_lock);
	spin_lock_init(&dev_priv->cursor_lock);

	ret = vmw_setup_pci_resources(dev_priv, pci_id);
	if (ret)
		return ret;
	ret = vmw_detect_version(dev_priv);
	if (ret)
		goto out_no_pci_or_version;


	for (i = vmw_res_context; i < vmw_res_max; ++i) {
		idr_init_base(&dev_priv->res_idr[i], 1);
		INIT_LIST_HEAD(&dev_priv->res_lru[i]);
	}

>>>>>>> upstream/android-13
	init_waitqueue_head(&dev_priv->fence_queue);
	init_waitqueue_head(&dev_priv->fifo_queue);
	dev_priv->fence_queue_waiters = 0;
	dev_priv->fifo_queue_waiters = 0;

	dev_priv->used_memory_size = 0;

<<<<<<< HEAD
	dev_priv->io_start = pci_resource_start(dev->pdev, 0);
	dev_priv->vram_start = pci_resource_start(dev->pdev, 1);
	dev_priv->mmio_start = pci_resource_start(dev->pdev, 2);

=======
>>>>>>> upstream/android-13
	dev_priv->assume_16bpp = !!vmw_assume_16bpp;

	dev_priv->enable_fb = enable_fbdev;

<<<<<<< HEAD
	vmw_write(dev_priv, SVGA_REG_ID, SVGA_ID_2);
	svga_id = vmw_read(dev_priv, SVGA_REG_ID);
	if (svga_id != SVGA_ID_2) {
		ret = -ENOSYS;
		DRM_ERROR("Unsupported SVGA ID 0x%x\n", svga_id);
		goto out_err0;
	}
=======
>>>>>>> upstream/android-13

	dev_priv->capabilities = vmw_read(dev_priv, SVGA_REG_CAPABILITIES);

	if (dev_priv->capabilities & SVGA_CAP_CAP2_REGISTER) {
		dev_priv->capabilities2 = vmw_read(dev_priv, SVGA_REG_CAP2);
	}


	ret = vmw_dma_select_mode(dev_priv);
	if (unlikely(ret != 0)) {
<<<<<<< HEAD
		DRM_INFO("Restricting capabilities due to IOMMU setup.\n");
		refuse_dma = true;
	}

	dev_priv->vram_size = vmw_read(dev_priv, SVGA_REG_VRAM_SIZE);
	dev_priv->mmio_size = vmw_read(dev_priv, SVGA_REG_MEM_SIZE);
=======
		drm_info(&dev_priv->drm,
			 "Restricting capabilities since DMA not available.\n");
		refuse_dma = true;
		if (dev_priv->capabilities & SVGA_CAP_GBOBJECTS)
			drm_info(&dev_priv->drm,
				 "Disabling 3D acceleration.\n");
	}

	dev_priv->vram_size = vmw_read(dev_priv, SVGA_REG_VRAM_SIZE);
	dev_priv->fifo_mem_size = vmw_read(dev_priv, SVGA_REG_MEM_SIZE);
>>>>>>> upstream/android-13
	dev_priv->fb_max_width = vmw_read(dev_priv, SVGA_REG_MAX_WIDTH);
	dev_priv->fb_max_height = vmw_read(dev_priv, SVGA_REG_MAX_HEIGHT);

	vmw_get_initial_size(dev_priv);

	if (dev_priv->capabilities & SVGA_CAP_GMR2) {
		dev_priv->max_gmr_ids =
			vmw_read(dev_priv, SVGA_REG_GMR_MAX_IDS);
		dev_priv->max_gmr_pages =
			vmw_read(dev_priv, SVGA_REG_GMRS_MAX_PAGES);
		dev_priv->memory_size =
			vmw_read(dev_priv, SVGA_REG_MEMORY_SIZE);
		dev_priv->memory_size -= dev_priv->vram_size;
	} else {
		/*
		 * An arbitrary limit of 512MiB on surface
		 * memory. But all HWV8 hardware supports GMR2.
		 */
		dev_priv->memory_size = 512*1024*1024;
	}
	dev_priv->max_mob_pages = 0;
	dev_priv->max_mob_size = 0;
	if (dev_priv->capabilities & SVGA_CAP_GBOBJECTS) {
<<<<<<< HEAD
		uint64_t mem_size =
			vmw_read(dev_priv,
				 SVGA_REG_SUGGESTED_GBOBJECT_MEM_SIZE_KB);
=======
		uint64_t mem_size;

		if (dev_priv->capabilities2 & SVGA_CAP2_GB_MEMSIZE_2)
			mem_size = vmw_read(dev_priv,
					    SVGA_REG_GBOBJECT_MEM_SIZE_KB);
		else
			mem_size =
				vmw_read(dev_priv,
					 SVGA_REG_SUGGESTED_GBOBJECT_MEM_SIZE_KB);
>>>>>>> upstream/android-13

		/*
		 * Workaround for low memory 2D VMs to compensate for the
		 * allocation taken by fbdev
		 */
		if (!(dev_priv->capabilities & SVGA_CAP_3D))
			mem_size *= 3;

		dev_priv->max_mob_pages = mem_size * 1024 / PAGE_SIZE;
<<<<<<< HEAD
		dev_priv->prim_bb_mem =
			vmw_read(dev_priv,
				 SVGA_REG_MAX_PRIMARY_BOUNDING_BOX_MEM);
=======
		dev_priv->max_primary_mem =
			vmw_read(dev_priv, SVGA_REG_MAX_PRIMARY_MEM);
>>>>>>> upstream/android-13
		dev_priv->max_mob_size =
			vmw_read(dev_priv, SVGA_REG_MOB_MAX_SIZE);
		dev_priv->stdu_max_width =
			vmw_read(dev_priv, SVGA_REG_SCREENTARGET_MAX_WIDTH);
		dev_priv->stdu_max_height =
			vmw_read(dev_priv, SVGA_REG_SCREENTARGET_MAX_HEIGHT);

		vmw_write(dev_priv, SVGA_REG_DEV_CAP,
			  SVGA3D_DEVCAP_MAX_TEXTURE_WIDTH);
		dev_priv->texture_max_width = vmw_read(dev_priv,
						       SVGA_REG_DEV_CAP);
		vmw_write(dev_priv, SVGA_REG_DEV_CAP,
			  SVGA3D_DEVCAP_MAX_TEXTURE_HEIGHT);
		dev_priv->texture_max_height = vmw_read(dev_priv,
							SVGA_REG_DEV_CAP);
	} else {
		dev_priv->texture_max_width = 8192;
		dev_priv->texture_max_height = 8192;
<<<<<<< HEAD
		dev_priv->prim_bb_mem = dev_priv->vram_size;
	}

	vmw_print_capabilities(dev_priv->capabilities);
	if (dev_priv->capabilities & SVGA_CAP_CAP2_REGISTER)
		vmw_print_capabilities2(dev_priv->capabilities2);
=======
		dev_priv->max_primary_mem = dev_priv->vram_size;
	}
	drm_info(&dev_priv->drm,
		 "Legacy memory limits: VRAM = %llu kB, FIFO = %llu kB, surface = %u kB\n",
		 (u64)dev_priv->vram_size / 1024,
		 (u64)dev_priv->fifo_mem_size / 1024,
		 dev_priv->memory_size / 1024);

	drm_info(&dev_priv->drm,
		 "MOB limits: max mob size = %u kB, max mob pages = %u\n",
		 dev_priv->max_mob_size / 1024, dev_priv->max_mob_pages);

	vmw_print_bitmap(&dev_priv->drm, "Capabilities",
			 dev_priv->capabilities,
			 cap1_names, ARRAY_SIZE(cap1_names));
	if (dev_priv->capabilities & SVGA_CAP_CAP2_REGISTER)
		vmw_print_bitmap(&dev_priv->drm, "Capabilities2",
				 dev_priv->capabilities2,
				 cap2_names, ARRAY_SIZE(cap2_names));
>>>>>>> upstream/android-13

	ret = vmw_dma_masks(dev_priv);
	if (unlikely(ret != 0))
		goto out_err0;

<<<<<<< HEAD
	dma_set_max_seg_size(dev->dev, min_t(unsigned int, U32_MAX & PAGE_MASK,
					     SCATTERLIST_MAX_SEGMENT));

	if (dev_priv->capabilities & SVGA_CAP_GMR2) {
		DRM_INFO("Max GMR ids is %u\n",
			 (unsigned)dev_priv->max_gmr_ids);
		DRM_INFO("Max number of GMR pages is %u\n",
			 (unsigned)dev_priv->max_gmr_pages);
		DRM_INFO("Max dedicated hypervisor surface memory is %u kiB\n",
			 (unsigned)dev_priv->memory_size / 1024);
	}
	DRM_INFO("Maximum display memory size is %u kiB\n",
		 dev_priv->prim_bb_mem / 1024);
	DRM_INFO("VRAM at 0x%08x size is %u kiB\n",
		 dev_priv->vram_start, dev_priv->vram_size / 1024);
	DRM_INFO("MMIO at 0x%08x size is %u kiB\n",
		 dev_priv->mmio_start, dev_priv->mmio_size / 1024);

	ret = vmw_ttm_global_init(dev_priv);
	if (unlikely(ret != 0))
		goto out_err0;


	vmw_master_init(&dev_priv->fbdev_master);
	ttm_lock_set_kill(&dev_priv->fbdev_master.lock, false, SIGTERM);
	dev_priv->active_master = &dev_priv->fbdev_master;

	dev_priv->mmio_virt = memremap(dev_priv->mmio_start,
				       dev_priv->mmio_size, MEMREMAP_WB);

	if (unlikely(dev_priv->mmio_virt == NULL)) {
		ret = -ENOMEM;
		DRM_ERROR("Failed mapping MMIO.\n");
		goto out_err3;
	}
=======
	dma_set_max_seg_size(dev_priv->drm.dev, U32_MAX);

	if (dev_priv->capabilities & SVGA_CAP_GMR2) {
		drm_info(&dev_priv->drm,
			 "Max GMR ids is %u\n",
			 (unsigned)dev_priv->max_gmr_ids);
		drm_info(&dev_priv->drm,
			 "Max number of GMR pages is %u\n",
			 (unsigned)dev_priv->max_gmr_pages);
	}
	drm_info(&dev_priv->drm,
		 "Maximum display memory size is %llu kiB\n",
		 (uint64_t)dev_priv->max_primary_mem / 1024);
>>>>>>> upstream/android-13

	/* Need mmio memory to check for fifo pitchlock cap. */
	if (!(dev_priv->capabilities & SVGA_CAP_DISPLAY_TOPOLOGY) &&
	    !(dev_priv->capabilities & SVGA_CAP_PITCHLOCK) &&
	    !vmw_fifo_have_pitchlock(dev_priv)) {
		ret = -ENOSYS;
		DRM_ERROR("Hardware has no pitchlock\n");
<<<<<<< HEAD
		goto out_err4;
	}

	dev_priv->tdev = ttm_object_device_init
		(dev_priv->mem_global_ref.object, 12, &vmw_prime_dmabuf_ops);

	if (unlikely(dev_priv->tdev == NULL)) {
		DRM_ERROR("Unable to initialize TTM object management.\n");
		ret = -ENOMEM;
		goto out_err4;
	}

	dev->dev_private = dev_priv;

	ret = pci_request_regions(dev->pdev, "vmwgfx probe");
	dev_priv->stealth = (ret != 0);
	if (dev_priv->stealth) {
		/**
		 * Request at least the mmio PCI resource.
		 */

		DRM_INFO("It appears like vesafb is loaded. "
			 "Ignore above error if any.\n");
		ret = pci_request_region(dev->pdev, 2, "vmwgfx stealth probe");
		if (unlikely(ret != 0)) {
			DRM_ERROR("Failed reserving the SVGA MMIO resource.\n");
			goto out_no_device;
		}
	}

	if (dev_priv->capabilities & SVGA_CAP_IRQMASK) {
		ret = vmw_irq_install(dev, dev->pdev->irq);
		if (ret != 0) {
			DRM_ERROR("Failed installing irq: %d\n", ret);
=======
		goto out_err0;
	}

	dev_priv->tdev = ttm_object_device_init(&ttm_mem_glob, 12,
						&vmw_prime_dmabuf_ops);

	if (unlikely(dev_priv->tdev == NULL)) {
		drm_err(&dev_priv->drm,
			"Unable to initialize TTM object management.\n");
		ret = -ENOMEM;
		goto out_err0;
	}

	if (dev_priv->capabilities & SVGA_CAP_IRQMASK) {
		ret = vmw_irq_install(&dev_priv->drm, pdev->irq);
		if (ret != 0) {
			drm_err(&dev_priv->drm,
				"Failed installing irq: %d\n", ret);
>>>>>>> upstream/android-13
			goto out_no_irq;
		}
	}

	dev_priv->fman = vmw_fence_manager_init(dev_priv);
	if (unlikely(dev_priv->fman == NULL)) {
		ret = -ENOMEM;
		goto out_no_fman;
	}

<<<<<<< HEAD
	ret = ttm_bo_device_init(&dev_priv->bdev,
				 dev_priv->bo_global_ref.ref.object,
				 &vmw_bo_driver,
				 dev->anon_inode->i_mapping,
				 VMWGFX_FILE_PAGE_OFFSET,
				 false);
	if (unlikely(ret != 0)) {
		DRM_ERROR("Failed initializing TTM buffer object driver.\n");
=======
	drm_vma_offset_manager_init(&dev_priv->vma_manager,
				    DRM_FILE_PAGE_OFFSET_START,
				    DRM_FILE_PAGE_OFFSET_SIZE);
	ret = ttm_device_init(&dev_priv->bdev, &vmw_bo_driver,
			      dev_priv->drm.dev,
			      dev_priv->drm.anon_inode->i_mapping,
			      &dev_priv->vma_manager,
			      dev_priv->map_mode == vmw_dma_alloc_coherent,
			      false);
	if (unlikely(ret != 0)) {
		drm_err(&dev_priv->drm,
			"Failed initializing TTM buffer object driver.\n");
>>>>>>> upstream/android-13
		goto out_no_bdev;
	}

	/*
	 * Enable VRAM, but initially don't use it until SVGA is enabled and
	 * unhidden.
	 */
<<<<<<< HEAD
	ret = ttm_bo_init_mm(&dev_priv->bdev, TTM_PL_VRAM,
			     (dev_priv->vram_size >> PAGE_SHIFT));
	if (unlikely(ret != 0)) {
		DRM_ERROR("Failed initializing memory manager for VRAM.\n");
		goto out_no_vram;
	}
	dev_priv->bdev.man[TTM_PL_VRAM].use_type = false;

	dev_priv->has_gmr = true;
	if (((dev_priv->capabilities & (SVGA_CAP_GMR | SVGA_CAP_GMR2)) == 0) ||
	    refuse_dma || ttm_bo_init_mm(&dev_priv->bdev, VMW_PL_GMR,
					 VMW_PL_GMR) != 0) {
		DRM_INFO("No GMR memory available. "
=======

	ret = vmw_vram_manager_init(dev_priv);
	if (unlikely(ret != 0)) {
		drm_err(&dev_priv->drm,
			"Failed initializing memory manager for VRAM.\n");
		goto out_no_vram;
	}

	ret = vmw_devcaps_create(dev_priv);
	if (unlikely(ret != 0)) {
		drm_err(&dev_priv->drm,
			"Failed initializing device caps.\n");
		goto out_no_vram;
	}

	/*
	 * "Guest Memory Regions" is an aperture like feature with
	 *  one slot per bo. There is an upper limit of the number of
	 *  slots as well as the bo size.
	 */
	dev_priv->has_gmr = true;
	/* TODO: This is most likely not correct */
	if (((dev_priv->capabilities & (SVGA_CAP_GMR | SVGA_CAP_GMR2)) == 0) ||
	    refuse_dma ||
	    vmw_gmrid_man_init(dev_priv, VMW_PL_GMR) != 0) {
		drm_info(&dev_priv->drm,
			  "No GMR memory available. "
>>>>>>> upstream/android-13
			 "Graphics memory resources are very limited.\n");
		dev_priv->has_gmr = false;
	}

<<<<<<< HEAD
	if (dev_priv->capabilities & SVGA_CAP_GBOBJECTS) {
		dev_priv->has_mob = true;
		if (ttm_bo_init_mm(&dev_priv->bdev, VMW_PL_MOB,
				   VMW_PL_MOB) != 0) {
			DRM_INFO("No MOB memory available. "
=======
	if (dev_priv->capabilities & SVGA_CAP_GBOBJECTS && !refuse_dma) {
		dev_priv->has_mob = true;

		if (vmw_gmrid_man_init(dev_priv, VMW_PL_MOB) != 0) {
			drm_info(&dev_priv->drm,
				 "No MOB memory available. "
				 "3D will be disabled.\n");
			dev_priv->has_mob = false;
		}
		if (vmw_sys_man_init(dev_priv) != 0) {
			drm_info(&dev_priv->drm,
				 "No MOB page table memory available. "
>>>>>>> upstream/android-13
				 "3D will be disabled.\n");
			dev_priv->has_mob = false;
		}
	}

<<<<<<< HEAD
	if (dev_priv->has_mob) {
		spin_lock(&dev_priv->cap_lock);
		vmw_write(dev_priv, SVGA_REG_DEV_CAP, SVGA3D_DEVCAP_DXCONTEXT);
		dev_priv->has_dx = !!vmw_read(dev_priv, SVGA_REG_DEV_CAP);
		spin_unlock(&dev_priv->cap_lock);
	}

=======
	if (dev_priv->has_mob && (dev_priv->capabilities & SVGA_CAP_DX)) {
		if (vmw_devcap_get(dev_priv, SVGA3D_DEVCAP_DXCONTEXT))
			dev_priv->sm_type = VMW_SM_4;
	}

	vmw_validation_mem_init_ttm(dev_priv, VMWGFX_VALIDATION_MEM_GRAN);

	/* SVGA_CAP2_DX2 (DefineGBSurface_v3) is needed for SM4_1 support */
	if (has_sm4_context(dev_priv) &&
	    (dev_priv->capabilities2 & SVGA_CAP2_DX2)) {
		if (vmw_devcap_get(dev_priv, SVGA3D_DEVCAP_SM41))
			dev_priv->sm_type = VMW_SM_4_1;
		if (has_sm4_1_context(dev_priv) &&
				(dev_priv->capabilities2 & SVGA_CAP2_DX3)) {
			if (vmw_devcap_get(dev_priv, SVGA3D_DEVCAP_SM5))
				dev_priv->sm_type = VMW_SM_5;
		}
	}
>>>>>>> upstream/android-13

	ret = vmw_kms_init(dev_priv);
	if (unlikely(ret != 0))
		goto out_no_kms;
	vmw_overlay_init(dev_priv);

	ret = vmw_request_device(dev_priv);
	if (ret)
		goto out_no_fifo;

<<<<<<< HEAD
	if (dev_priv->has_dx) {
		/*
		 * SVGA_CAP2_DX2 (DefineGBSurface_v3) is needed for SM4_1
		 * support
		 */
		if ((dev_priv->capabilities2 & SVGA_CAP2_DX2) != 0) {
			vmw_write(dev_priv, SVGA_REG_DEV_CAP,
					SVGA3D_DEVCAP_SM41);
			dev_priv->has_sm4_1 = vmw_read(dev_priv,
							SVGA_REG_DEV_CAP);
		}
	}

	DRM_INFO("DX: %s\n", dev_priv->has_dx ? "yes." : "no.");
	DRM_INFO("Atomic: %s\n", (dev->driver->driver_features & DRIVER_ATOMIC)
		 ? "yes." : "no.");
	DRM_INFO("SM4_1: %s\n", dev_priv->has_sm4_1 ? "yes." : "no.");

	snprintf(host_log, sizeof(host_log), "vmwgfx: %s-%s",
		VMWGFX_REPO, VMWGFX_GIT_VERSION);
	vmw_host_log(host_log);

	memset(host_log, 0, sizeof(host_log));
	snprintf(host_log, sizeof(host_log), "vmwgfx: Module Version: %d.%d.%d",
		VMWGFX_DRIVER_MAJOR, VMWGFX_DRIVER_MINOR,
		VMWGFX_DRIVER_PATCHLEVEL);
	vmw_host_log(host_log);
=======
	vmw_print_sm_type(dev_priv);
	vmw_host_printf("vmwgfx: Module Version: %d.%d.%d (kernel: %s)",
			VMWGFX_DRIVER_MAJOR, VMWGFX_DRIVER_MINOR,
			VMWGFX_DRIVER_PATCHLEVEL, UTS_RELEASE);
>>>>>>> upstream/android-13

	if (dev_priv->enable_fb) {
		vmw_fifo_resource_inc(dev_priv);
		vmw_svga_enable(dev_priv);
		vmw_fb_init(dev_priv);
	}

	dev_priv->pm_nb.notifier_call = vmwgfx_pm_notifier;
	register_pm_notifier(&dev_priv->pm_nb);

	return 0;

out_no_fifo:
	vmw_overlay_close(dev_priv);
	vmw_kms_close(dev_priv);
out_no_kms:
<<<<<<< HEAD
	if (dev_priv->has_mob)
		(void) ttm_bo_clean_mm(&dev_priv->bdev, VMW_PL_MOB);
	if (dev_priv->has_gmr)
		(void) ttm_bo_clean_mm(&dev_priv->bdev, VMW_PL_GMR);
	(void)ttm_bo_clean_mm(&dev_priv->bdev, TTM_PL_VRAM);
out_no_vram:
	(void)ttm_bo_device_release(&dev_priv->bdev);
=======
	if (dev_priv->has_mob) {
		vmw_gmrid_man_fini(dev_priv, VMW_PL_MOB);
		vmw_sys_man_fini(dev_priv);
	}
	if (dev_priv->has_gmr)
		vmw_gmrid_man_fini(dev_priv, VMW_PL_GMR);
	vmw_devcaps_destroy(dev_priv);
	vmw_vram_manager_fini(dev_priv);
out_no_vram:
	ttm_device_fini(&dev_priv->bdev);
>>>>>>> upstream/android-13
out_no_bdev:
	vmw_fence_manager_takedown(dev_priv->fman);
out_no_fman:
	if (dev_priv->capabilities & SVGA_CAP_IRQMASK)
<<<<<<< HEAD
		vmw_irq_uninstall(dev_priv->dev);
out_no_irq:
	if (dev_priv->stealth)
		pci_release_region(dev->pdev, 2);
	else
		pci_release_regions(dev->pdev);
out_no_device:
	ttm_object_device_release(&dev_priv->tdev);
out_err4:
	memunmap(dev_priv->mmio_virt);
out_err3:
	vmw_ttm_global_release(dev_priv);
=======
		vmw_irq_uninstall(&dev_priv->drm);
out_no_irq:
	ttm_object_device_release(&dev_priv->tdev);
>>>>>>> upstream/android-13
out_err0:
	for (i = vmw_res_context; i < vmw_res_max; ++i)
		idr_destroy(&dev_priv->res_idr[i]);

	if (dev_priv->ctx.staged_bindings)
		vmw_binding_state_free(dev_priv->ctx.staged_bindings);
<<<<<<< HEAD
	kfree(dev_priv);
=======
out_no_pci_or_version:
	pci_release_regions(pdev);
>>>>>>> upstream/android-13
	return ret;
}

static void vmw_driver_unload(struct drm_device *dev)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
<<<<<<< HEAD
=======
	struct pci_dev *pdev = to_pci_dev(dev->dev);
>>>>>>> upstream/android-13
	enum vmw_res_type i;

	unregister_pm_notifier(&dev_priv->pm_nb);

	if (dev_priv->ctx.res_ht_initialized)
		drm_ht_remove(&dev_priv->ctx.res_ht);
	vfree(dev_priv->ctx.cmd_bounce);
	if (dev_priv->enable_fb) {
		vmw_fb_off(dev_priv);
		vmw_fb_close(dev_priv);
		vmw_fifo_resource_dec(dev_priv);
		vmw_svga_disable(dev_priv);
	}

	vmw_kms_close(dev_priv);
	vmw_overlay_close(dev_priv);

	if (dev_priv->has_gmr)
<<<<<<< HEAD
		(void)ttm_bo_clean_mm(&dev_priv->bdev, VMW_PL_GMR);
	(void)ttm_bo_clean_mm(&dev_priv->bdev, TTM_PL_VRAM);

	vmw_release_device_early(dev_priv);
	if (dev_priv->has_mob)
		(void) ttm_bo_clean_mm(&dev_priv->bdev, VMW_PL_MOB);
	(void) ttm_bo_device_release(&dev_priv->bdev);
	vmw_release_device_late(dev_priv);
	vmw_fence_manager_takedown(dev_priv->fman);
	if (dev_priv->capabilities & SVGA_CAP_IRQMASK)
		vmw_irq_uninstall(dev_priv->dev);
	if (dev_priv->stealth)
		pci_release_region(dev->pdev, 2);
	else
		pci_release_regions(dev->pdev);

	ttm_object_device_release(&dev_priv->tdev);
	memunmap(dev_priv->mmio_virt);
	if (dev_priv->ctx.staged_bindings)
		vmw_binding_state_free(dev_priv->ctx.staged_bindings);
	vmw_ttm_global_release(dev_priv);
=======
		vmw_gmrid_man_fini(dev_priv, VMW_PL_GMR);

	vmw_release_device_early(dev_priv);
	if (dev_priv->has_mob) {
		vmw_gmrid_man_fini(dev_priv, VMW_PL_MOB);
		vmw_sys_man_fini(dev_priv);
	}
	vmw_devcaps_destroy(dev_priv);
	vmw_vram_manager_fini(dev_priv);
	ttm_device_fini(&dev_priv->bdev);
	drm_vma_offset_manager_destroy(&dev_priv->vma_manager);
	vmw_release_device_late(dev_priv);
	vmw_fence_manager_takedown(dev_priv->fman);
	if (dev_priv->capabilities & SVGA_CAP_IRQMASK)
		vmw_irq_uninstall(&dev_priv->drm);

	ttm_object_device_release(&dev_priv->tdev);
	if (dev_priv->ctx.staged_bindings)
		vmw_binding_state_free(dev_priv->ctx.staged_bindings);
>>>>>>> upstream/android-13

	for (i = vmw_res_context; i < vmw_res_max; ++i)
		idr_destroy(&dev_priv->res_idr[i]);

<<<<<<< HEAD
	kfree(dev_priv);
=======
	vmw_mksstat_remove_all(dev_priv);

	pci_release_regions(pdev);
>>>>>>> upstream/android-13
}

static void vmw_postclose(struct drm_device *dev,
			 struct drm_file *file_priv)
{
<<<<<<< HEAD
	struct vmw_fpriv *vmw_fp;

	vmw_fp = vmw_fpriv(file_priv);

	if (vmw_fp->locked_master) {
		struct vmw_master *vmaster =
			vmw_master(vmw_fp->locked_master);

		ttm_lock_set_kill(&vmaster->lock, true, SIGTERM);
		ttm_vt_unlock(&vmaster->lock);
		drm_master_put(&vmw_fp->locked_master);
	}
=======
	struct vmw_fpriv *vmw_fp = vmw_fpriv(file_priv);
>>>>>>> upstream/android-13

	ttm_object_file_release(&vmw_fp->tfile);
	kfree(vmw_fp);
}

static int vmw_driver_open(struct drm_device *dev, struct drm_file *file_priv)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	struct vmw_fpriv *vmw_fp;
	int ret = -ENOMEM;

	vmw_fp = kzalloc(sizeof(*vmw_fp), GFP_KERNEL);
	if (unlikely(!vmw_fp))
		return ret;

	vmw_fp->tfile = ttm_object_file_init(dev_priv->tdev, 10);
	if (unlikely(vmw_fp->tfile == NULL))
		goto out_no_tfile;

	file_priv->driver_priv = vmw_fp;

	return 0;

out_no_tfile:
	kfree(vmw_fp);
	return ret;
}

<<<<<<< HEAD
static struct vmw_master *vmw_master_check(struct drm_device *dev,
					   struct drm_file *file_priv,
					   unsigned int flags)
{
	int ret;
	struct vmw_fpriv *vmw_fp = vmw_fpriv(file_priv);
	struct vmw_master *vmaster;

	if (!drm_is_primary_client(file_priv) || !(flags & DRM_AUTH))
		return NULL;

	ret = mutex_lock_interruptible(&dev->master_mutex);
	if (unlikely(ret != 0))
		return ERR_PTR(-ERESTARTSYS);

	if (drm_is_current_master(file_priv)) {
		mutex_unlock(&dev->master_mutex);
		return NULL;
	}

	/*
	 * Check if we were previously master, but now dropped. In that
	 * case, allow at least render node functionality.
	 */
	if (vmw_fp->locked_master) {
		mutex_unlock(&dev->master_mutex);

		if (flags & DRM_RENDER_ALLOW)
			return NULL;

		DRM_ERROR("Dropped master trying to access ioctl that "
			  "requires authentication.\n");
		return ERR_PTR(-EACCES);
	}
	mutex_unlock(&dev->master_mutex);

	/*
	 * Take the TTM lock. Possibly sleep waiting for the authenticating
	 * master to become master again, or for a SIGTERM if the
	 * authenticating master exits.
	 */
	vmaster = vmw_master(file_priv->master);
	ret = ttm_read_lock(&vmaster->lock, true);
	if (unlikely(ret != 0))
		vmaster = ERR_PTR(ret);

	return vmaster;
}

=======
>>>>>>> upstream/android-13
static long vmw_generic_ioctl(struct file *filp, unsigned int cmd,
			      unsigned long arg,
			      long (*ioctl_func)(struct file *, unsigned int,
						 unsigned long))
{
	struct drm_file *file_priv = filp->private_data;
	struct drm_device *dev = file_priv->minor->dev;
	unsigned int nr = DRM_IOCTL_NR(cmd);
<<<<<<< HEAD
	struct vmw_master *vmaster;
	unsigned int flags;
	long ret;
=======
	unsigned int flags;
>>>>>>> upstream/android-13

	/*
	 * Do extra checking on driver private ioctls.
	 */

	if ((nr >= DRM_COMMAND_BASE) && (nr < DRM_COMMAND_END)
	    && (nr < DRM_COMMAND_BASE + dev->driver->num_ioctls)) {
		const struct drm_ioctl_desc *ioctl =
			&vmw_ioctls[nr - DRM_COMMAND_BASE];

		if (nr == DRM_COMMAND_BASE + DRM_VMW_EXECBUF) {
<<<<<<< HEAD
			ret = (long) drm_ioctl_permit(ioctl->flags, file_priv);
			if (unlikely(ret != 0))
				return ret;

			if (unlikely((cmd & (IOC_IN | IOC_OUT)) != IOC_IN))
				goto out_io_encoding;

			return (long) vmw_execbuf_ioctl(dev, arg, file_priv,
							_IOC_SIZE(cmd));
=======
			return ioctl_func(filp, cmd, arg);
>>>>>>> upstream/android-13
		} else if (nr == DRM_COMMAND_BASE + DRM_VMW_UPDATE_LAYOUT) {
			if (!drm_is_current_master(file_priv) &&
			    !capable(CAP_SYS_ADMIN))
				return -EACCES;
		}

		if (unlikely(ioctl->cmd != cmd))
			goto out_io_encoding;

		flags = ioctl->flags;
	} else if (!drm_ioctl_flags(nr, &flags))
		return -EINVAL;

<<<<<<< HEAD
	vmaster = vmw_master_check(dev, file_priv, flags);
	if (IS_ERR(vmaster)) {
		ret = PTR_ERR(vmaster);

		if (ret != -ERESTARTSYS)
			DRM_INFO("IOCTL ERROR Command %d, Error %ld.\n",
				 nr, ret);
		return ret;
	}

	ret = ioctl_func(filp, cmd, arg);
	if (vmaster)
		ttm_read_unlock(&vmaster->lock);

	return ret;
=======
	return ioctl_func(filp, cmd, arg);
>>>>>>> upstream/android-13

out_io_encoding:
	DRM_ERROR("Invalid command format, ioctl %d\n",
		  nr - DRM_COMMAND_BASE);

	return -EINVAL;
}

static long vmw_unlocked_ioctl(struct file *filp, unsigned int cmd,
			       unsigned long arg)
{
	return vmw_generic_ioctl(filp, cmd, arg, &drm_ioctl);
}

#ifdef CONFIG_COMPAT
static long vmw_compat_ioctl(struct file *filp, unsigned int cmd,
			     unsigned long arg)
{
	return vmw_generic_ioctl(filp, cmd, arg, &drm_compat_ioctl);
}
#endif

<<<<<<< HEAD
static void vmw_lastclose(struct drm_device *dev)
{
}

static void vmw_master_init(struct vmw_master *vmaster)
{
	ttm_lock_init(&vmaster->lock);
}

static int vmw_master_create(struct drm_device *dev,
			     struct drm_master *master)
{
	struct vmw_master *vmaster;

	vmaster = kzalloc(sizeof(*vmaster), GFP_KERNEL);
	if (unlikely(!vmaster))
		return -ENOMEM;

	vmw_master_init(vmaster);
	ttm_lock_set_kill(&vmaster->lock, true, SIGTERM);
	master->driver_priv = vmaster;

	return 0;
}

static void vmw_master_destroy(struct drm_device *dev,
			       struct drm_master *master)
{
	struct vmw_master *vmaster = vmw_master(master);

	master->driver_priv = NULL;
	kfree(vmaster);
}

static int vmw_master_set(struct drm_device *dev,
			  struct drm_file *file_priv,
			  bool from_open)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	struct vmw_fpriv *vmw_fp = vmw_fpriv(file_priv);
	struct vmw_master *active = dev_priv->active_master;
	struct vmw_master *vmaster = vmw_master(file_priv->master);
	int ret = 0;

	if (active) {
		BUG_ON(active != &dev_priv->fbdev_master);
		ret = ttm_vt_lock(&active->lock, false, vmw_fp->tfile);
		if (unlikely(ret != 0))
			return ret;

		ttm_lock_set_kill(&active->lock, true, SIGTERM);
		dev_priv->active_master = NULL;
	}

	ttm_lock_set_kill(&vmaster->lock, false, SIGTERM);
	if (!from_open) {
		ttm_vt_unlock(&vmaster->lock);
		BUG_ON(vmw_fp->locked_master != file_priv->master);
		drm_master_put(&vmw_fp->locked_master);
	}

	dev_priv->active_master = vmaster;

=======
static void vmw_master_set(struct drm_device *dev,
			   struct drm_file *file_priv,
			   bool from_open)
{
>>>>>>> upstream/android-13
	/*
	 * Inform a new master that the layout may have changed while
	 * it was gone.
	 */
	if (!from_open)
		drm_sysfs_hotplug_event(dev);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> upstream/android-13
}

static void vmw_master_drop(struct drm_device *dev,
			    struct drm_file *file_priv)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
<<<<<<< HEAD
	struct vmw_fpriv *vmw_fp = vmw_fpriv(file_priv);
	struct vmw_master *vmaster = vmw_master(file_priv->master);
	int ret;

	/**
	 * Make sure the master doesn't disappear while we have
	 * it locked.
	 */

	vmw_fp->locked_master = drm_master_get(file_priv->master);
	ret = ttm_vt_lock(&vmaster->lock, false, vmw_fp->tfile);
	vmw_kms_legacy_hotspot_clear(dev_priv);
	if (unlikely((ret != 0))) {
		DRM_ERROR("Unable to lock TTM at VT switch.\n");
		drm_master_put(&vmw_fp->locked_master);
	}

	ttm_lock_set_kill(&vmaster->lock, false, SIGTERM);

	if (!dev_priv->enable_fb)
		vmw_svga_disable(dev_priv);

	dev_priv->active_master = &dev_priv->fbdev_master;
	ttm_lock_set_kill(&dev_priv->fbdev_master.lock, false, SIGTERM);
	ttm_vt_unlock(&dev_priv->fbdev_master.lock);
=======

	vmw_kms_legacy_hotspot_clear(dev_priv);
	if (!dev_priv->enable_fb)
		vmw_svga_disable(dev_priv);
>>>>>>> upstream/android-13
}

/**
 * __vmw_svga_enable - Enable SVGA mode, FIFO and use of VRAM.
 *
 * @dev_priv: Pointer to device private struct.
 * Needs the reservation sem to be held in non-exclusive mode.
 */
static void __vmw_svga_enable(struct vmw_private *dev_priv)
{
<<<<<<< HEAD
	spin_lock(&dev_priv->svga_lock);
	if (!dev_priv->bdev.man[TTM_PL_VRAM].use_type) {
		vmw_write(dev_priv, SVGA_REG_ENABLE, SVGA_REG_ENABLE);
		dev_priv->bdev.man[TTM_PL_VRAM].use_type = true;
	}
	spin_unlock(&dev_priv->svga_lock);
=======
	struct ttm_resource_manager *man = ttm_manager_type(&dev_priv->bdev, TTM_PL_VRAM);

	if (!ttm_resource_manager_used(man)) {
		vmw_write(dev_priv, SVGA_REG_ENABLE, SVGA_REG_ENABLE_ENABLE);
		ttm_resource_manager_set_used(man, true);
	}
>>>>>>> upstream/android-13
}

/**
 * vmw_svga_enable - Enable SVGA mode, FIFO and use of VRAM.
 *
 * @dev_priv: Pointer to device private struct.
 */
void vmw_svga_enable(struct vmw_private *dev_priv)
{
<<<<<<< HEAD
	(void) ttm_read_lock(&dev_priv->reservation_sem, false);
	__vmw_svga_enable(dev_priv);
	ttm_read_unlock(&dev_priv->reservation_sem);
=======
	__vmw_svga_enable(dev_priv);
>>>>>>> upstream/android-13
}

/**
 * __vmw_svga_disable - Disable SVGA mode and use of VRAM.
 *
 * @dev_priv: Pointer to device private struct.
 * Needs the reservation sem to be held in exclusive mode.
 * Will not empty VRAM. VRAM must be emptied by caller.
 */
static void __vmw_svga_disable(struct vmw_private *dev_priv)
{
<<<<<<< HEAD
	spin_lock(&dev_priv->svga_lock);
	if (dev_priv->bdev.man[TTM_PL_VRAM].use_type) {
		dev_priv->bdev.man[TTM_PL_VRAM].use_type = false;
=======
	struct ttm_resource_manager *man = ttm_manager_type(&dev_priv->bdev, TTM_PL_VRAM);

	if (ttm_resource_manager_used(man)) {
		ttm_resource_manager_set_used(man, false);
>>>>>>> upstream/android-13
		vmw_write(dev_priv, SVGA_REG_ENABLE,
			  SVGA_REG_ENABLE_HIDE |
			  SVGA_REG_ENABLE_ENABLE);
	}
<<<<<<< HEAD
	spin_unlock(&dev_priv->svga_lock);
=======
>>>>>>> upstream/android-13
}

/**
 * vmw_svga_disable - Disable SVGA_MODE, and use of VRAM. Keep the fifo
 * running.
 *
 * @dev_priv: Pointer to device private struct.
 * Will empty VRAM.
 */
void vmw_svga_disable(struct vmw_private *dev_priv)
{
<<<<<<< HEAD
=======
	struct ttm_resource_manager *man = ttm_manager_type(&dev_priv->bdev, TTM_PL_VRAM);
>>>>>>> upstream/android-13
	/*
	 * Disabling SVGA will turn off device modesetting capabilities, so
	 * notify KMS about that so that it doesn't cache atomic state that
	 * isn't valid anymore, for example crtcs turned on.
	 * Strictly we'd want to do this under the SVGA lock (or an SVGA mutex),
	 * but vmw_kms_lost_device() takes the reservation sem and thus we'll
	 * end up with lock order reversal. Thus, a master may actually perform
	 * a new modeset just after we call vmw_kms_lost_device() and race with
	 * vmw_svga_disable(), but that should at worst cause atomic KMS state
	 * to be inconsistent with the device, causing modesetting problems.
	 *
	 */
<<<<<<< HEAD
	vmw_kms_lost_device(dev_priv->dev);
	ttm_write_lock(&dev_priv->reservation_sem, false);
	spin_lock(&dev_priv->svga_lock);
	if (dev_priv->bdev.man[TTM_PL_VRAM].use_type) {
		dev_priv->bdev.man[TTM_PL_VRAM].use_type = false;
		spin_unlock(&dev_priv->svga_lock);
		if (ttm_bo_evict_mm(&dev_priv->bdev, TTM_PL_VRAM))
			DRM_ERROR("Failed evicting VRAM buffers.\n");
		vmw_write(dev_priv, SVGA_REG_ENABLE,
			  SVGA_REG_ENABLE_HIDE |
			  SVGA_REG_ENABLE_ENABLE);
	} else
		spin_unlock(&dev_priv->svga_lock);
	ttm_write_unlock(&dev_priv->reservation_sem);
=======
	vmw_kms_lost_device(&dev_priv->drm);
	if (ttm_resource_manager_used(man)) {
		if (ttm_resource_manager_evict_all(&dev_priv->bdev, man))
			DRM_ERROR("Failed evicting VRAM buffers.\n");
		ttm_resource_manager_set_used(man, false);
		vmw_write(dev_priv, SVGA_REG_ENABLE,
			  SVGA_REG_ENABLE_HIDE |
			  SVGA_REG_ENABLE_ENABLE);
	}
>>>>>>> upstream/android-13
}

static void vmw_remove(struct pci_dev *pdev)
{
	struct drm_device *dev = pci_get_drvdata(pdev);

<<<<<<< HEAD
	pci_disable_device(pdev);
	drm_put_dev(dev);
=======
	ttm_mem_global_release(&ttm_mem_glob);
	drm_dev_unregister(dev);
	vmw_driver_unload(dev);
}

static unsigned long
vmw_get_unmapped_area(struct file *file, unsigned long uaddr,
		      unsigned long len, unsigned long pgoff,
		      unsigned long flags)
{
	struct drm_file *file_priv = file->private_data;
	struct vmw_private *dev_priv = vmw_priv(file_priv->minor->dev);

	return drm_get_unmapped_area(file, uaddr, len, pgoff, flags,
				     &dev_priv->vma_manager);
>>>>>>> upstream/android-13
}

static int vmwgfx_pm_notifier(struct notifier_block *nb, unsigned long val,
			      void *ptr)
{
	struct vmw_private *dev_priv =
		container_of(nb, struct vmw_private, pm_nb);

	switch (val) {
	case PM_HIBERNATION_PREPARE:
		/*
		 * Take the reservation sem in write mode, which will make sure
		 * there are no other processes holding a buffer object
		 * reservation, meaning we should be able to evict all buffer
		 * objects if needed.
		 * Once user-space processes have been frozen, we can release
		 * the lock again.
		 */
<<<<<<< HEAD
		ttm_suspend_lock(&dev_priv->reservation_sem);
=======
>>>>>>> upstream/android-13
		dev_priv->suspend_locked = true;
		break;
	case PM_POST_HIBERNATION:
	case PM_POST_RESTORE:
		if (READ_ONCE(dev_priv->suspend_locked)) {
			dev_priv->suspend_locked = false;
<<<<<<< HEAD
			ttm_suspend_unlock(&dev_priv->reservation_sem);
=======
>>>>>>> upstream/android-13
		}
		break;
	default:
		break;
	}
	return 0;
}

static int vmw_pci_suspend(struct pci_dev *pdev, pm_message_t state)
{
	struct drm_device *dev = pci_get_drvdata(pdev);
	struct vmw_private *dev_priv = vmw_priv(dev);

	if (dev_priv->refuse_hibernation)
		return -EBUSY;

	pci_save_state(pdev);
	pci_disable_device(pdev);
	pci_set_power_state(pdev, PCI_D3hot);
	return 0;
}

static int vmw_pci_resume(struct pci_dev *pdev)
{
	pci_set_power_state(pdev, PCI_D0);
	pci_restore_state(pdev);
	return pci_enable_device(pdev);
}

static int vmw_pm_suspend(struct device *kdev)
{
	struct pci_dev *pdev = to_pci_dev(kdev);
	struct pm_message dummy;

	dummy.event = 0;

	return vmw_pci_suspend(pdev, dummy);
}

static int vmw_pm_resume(struct device *kdev)
{
	struct pci_dev *pdev = to_pci_dev(kdev);

	return vmw_pci_resume(pdev);
}

static int vmw_pm_freeze(struct device *kdev)
{
	struct pci_dev *pdev = to_pci_dev(kdev);
	struct drm_device *dev = pci_get_drvdata(pdev);
	struct vmw_private *dev_priv = vmw_priv(dev);
<<<<<<< HEAD
	int ret;

	/*
	 * Unlock for vmw_kms_suspend.
	 * No user-space processes should be running now.
	 */
	ttm_suspend_unlock(&dev_priv->reservation_sem);
	ret = vmw_kms_suspend(dev_priv->dev);
	if (ret) {
		ttm_suspend_lock(&dev_priv->reservation_sem);
=======
	struct ttm_operation_ctx ctx = {
		.interruptible = false,
		.no_wait_gpu = false
	};
	int ret;

	/*
	 * No user-space processes should be running now.
	 */
	ret = vmw_kms_suspend(&dev_priv->drm);
	if (ret) {
>>>>>>> upstream/android-13
		DRM_ERROR("Failed to freeze modesetting.\n");
		return ret;
	}
	if (dev_priv->enable_fb)
		vmw_fb_off(dev_priv);

<<<<<<< HEAD
	ttm_suspend_lock(&dev_priv->reservation_sem);
	vmw_execbuf_release_pinned_bo(dev_priv);
	vmw_resource_evict_all(dev_priv);
	vmw_release_device_early(dev_priv);
	ttm_bo_swapout_all(&dev_priv->bdev);
=======
	vmw_execbuf_release_pinned_bo(dev_priv);
	vmw_resource_evict_all(dev_priv);
	vmw_release_device_early(dev_priv);
	while (ttm_device_swapout(&dev_priv->bdev, &ctx, GFP_KERNEL) > 0);
>>>>>>> upstream/android-13
	if (dev_priv->enable_fb)
		vmw_fifo_resource_dec(dev_priv);
	if (atomic_read(&dev_priv->num_fifo_resources) != 0) {
		DRM_ERROR("Can't hibernate while 3D resources are active.\n");
		if (dev_priv->enable_fb)
			vmw_fifo_resource_inc(dev_priv);
		WARN_ON(vmw_request_device_late(dev_priv));
		dev_priv->suspend_locked = false;
<<<<<<< HEAD
		ttm_suspend_unlock(&dev_priv->reservation_sem);
=======
>>>>>>> upstream/android-13
		if (dev_priv->suspend_state)
			vmw_kms_resume(dev);
		if (dev_priv->enable_fb)
			vmw_fb_on(dev_priv);
		return -EBUSY;
	}

	vmw_fence_fifo_down(dev_priv->fman);
	__vmw_svga_disable(dev_priv);
<<<<<<< HEAD
	
=======

>>>>>>> upstream/android-13
	vmw_release_device_late(dev_priv);
	return 0;
}

static int vmw_pm_restore(struct device *kdev)
{
	struct pci_dev *pdev = to_pci_dev(kdev);
	struct drm_device *dev = pci_get_drvdata(pdev);
	struct vmw_private *dev_priv = vmw_priv(dev);
	int ret;

<<<<<<< HEAD
	vmw_write(dev_priv, SVGA_REG_ID, SVGA_ID_2);
	(void) vmw_read(dev_priv, SVGA_REG_ID);
=======
	vmw_detect_version(dev_priv);
>>>>>>> upstream/android-13

	if (dev_priv->enable_fb)
		vmw_fifo_resource_inc(dev_priv);

	ret = vmw_request_device(dev_priv);
	if (ret)
		return ret;

	if (dev_priv->enable_fb)
		__vmw_svga_enable(dev_priv);

	vmw_fence_fifo_up(dev_priv->fman);
	dev_priv->suspend_locked = false;
<<<<<<< HEAD
	ttm_suspend_unlock(&dev_priv->reservation_sem);
	if (dev_priv->suspend_state)
		vmw_kms_resume(dev_priv->dev);
=======
	if (dev_priv->suspend_state)
		vmw_kms_resume(&dev_priv->drm);
>>>>>>> upstream/android-13

	if (dev_priv->enable_fb)
		vmw_fb_on(dev_priv);

	return 0;
}

static const struct dev_pm_ops vmw_pm_ops = {
	.freeze = vmw_pm_freeze,
	.thaw = vmw_pm_restore,
	.restore = vmw_pm_restore,
	.suspend = vmw_pm_suspend,
	.resume = vmw_pm_resume,
};

static const struct file_operations vmwgfx_driver_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.release = drm_release,
	.unlocked_ioctl = vmw_unlocked_ioctl,
	.mmap = vmw_mmap,
<<<<<<< HEAD
	.poll = vmw_fops_poll,
	.read = vmw_fops_read,
=======
	.poll = drm_poll,
	.read = drm_read,
>>>>>>> upstream/android-13
#if defined(CONFIG_COMPAT)
	.compat_ioctl = vmw_compat_ioctl,
#endif
	.llseek = noop_llseek,
<<<<<<< HEAD
};

static struct drm_driver driver = {
	.driver_features = DRIVER_HAVE_IRQ | DRIVER_IRQ_SHARED |
	DRIVER_MODESET | DRIVER_PRIME | DRIVER_RENDER | DRIVER_ATOMIC,
	.load = vmw_driver_load,
	.unload = vmw_driver_unload,
	.lastclose = vmw_lastclose,
	.get_vblank_counter = vmw_get_vblank_counter,
	.enable_vblank = vmw_enable_vblank,
	.disable_vblank = vmw_disable_vblank,
	.ioctls = vmw_ioctls,
	.num_ioctls = ARRAY_SIZE(vmw_ioctls),
	.master_create = vmw_master_create,
	.master_destroy = vmw_master_destroy,
=======
	.get_unmapped_area = vmw_get_unmapped_area,
};

static const struct drm_driver driver = {
	.driver_features =
	DRIVER_MODESET | DRIVER_RENDER | DRIVER_ATOMIC,
	.ioctls = vmw_ioctls,
	.num_ioctls = ARRAY_SIZE(vmw_ioctls),
>>>>>>> upstream/android-13
	.master_set = vmw_master_set,
	.master_drop = vmw_master_drop,
	.open = vmw_driver_open,
	.postclose = vmw_postclose,

	.dumb_create = vmw_dumb_create,
	.dumb_map_offset = vmw_dumb_map_offset,
	.dumb_destroy = vmw_dumb_destroy,

	.prime_fd_to_handle = vmw_prime_fd_to_handle,
	.prime_handle_to_fd = vmw_prime_handle_to_fd,

	.fops = &vmwgfx_driver_fops,
	.name = VMWGFX_DRIVER_NAME,
	.desc = VMWGFX_DRIVER_DESC,
	.date = VMWGFX_DRIVER_DATE,
	.major = VMWGFX_DRIVER_MAJOR,
	.minor = VMWGFX_DRIVER_MINOR,
	.patchlevel = VMWGFX_DRIVER_PATCHLEVEL
};

static struct pci_driver vmw_pci_driver = {
	.name = VMWGFX_DRIVER_NAME,
	.id_table = vmw_pci_id_list,
	.probe = vmw_probe,
	.remove = vmw_remove,
	.driver = {
		.pm = &vmw_pm_ops
	}
};

static int vmw_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
<<<<<<< HEAD
	return drm_get_pci_dev(pdev, ent, &driver);
=======
	struct vmw_private *vmw;
	int ret;

	ret = drm_aperture_remove_conflicting_pci_framebuffers(pdev, &driver);
	if (ret)
		goto out_error;

	ret = pcim_enable_device(pdev);
	if (ret)
		goto out_error;

	vmw = devm_drm_dev_alloc(&pdev->dev, &driver,
				 struct vmw_private, drm);
	if (IS_ERR(vmw)) {
		ret = PTR_ERR(vmw);
		goto out_error;
	}

	pci_set_drvdata(pdev, &vmw->drm);

	ret = ttm_mem_global_init(&ttm_mem_glob, &pdev->dev);
	if (ret)
		goto out_error;

	ret = vmw_driver_load(vmw, ent->device);
	if (ret)
		goto out_release;

	ret = drm_dev_register(&vmw->drm, 0);
	if (ret)
		goto out_unload;

	return 0;
out_unload:
	vmw_driver_unload(&vmw->drm);
out_release:
	ttm_mem_global_release(&ttm_mem_glob);
out_error:
	return ret;
>>>>>>> upstream/android-13
}

static int __init vmwgfx_init(void)
{
	int ret;

	if (vgacon_text_force())
		return -EINVAL;

	ret = pci_register_driver(&vmw_pci_driver);
	if (ret)
		DRM_ERROR("Failed initializing DRM.\n");
	return ret;
}

static void __exit vmwgfx_exit(void)
{
	pci_unregister_driver(&vmw_pci_driver);
}

module_init(vmwgfx_init);
module_exit(vmwgfx_exit);

MODULE_AUTHOR("VMware Inc. and others");
MODULE_DESCRIPTION("Standalone drm driver for the VMware SVGA device");
MODULE_LICENSE("GPL and additional rights");
MODULE_VERSION(__stringify(VMWGFX_DRIVER_MAJOR) "."
	       __stringify(VMWGFX_DRIVER_MINOR) "."
	       __stringify(VMWGFX_DRIVER_PATCHLEVEL) "."
	       "0");
