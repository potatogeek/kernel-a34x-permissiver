# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the drm device driver.  This driver provides support for the

<<<<<<< HEAD
ttm-y := ttm_memory.o ttm_tt.o ttm_bo.o \
	ttm_bo_util.o ttm_bo_vm.o ttm_module.o \
	ttm_object.o ttm_lock.o ttm_execbuf_util.o ttm_page_alloc.o \
	ttm_bo_manager.o ttm_page_alloc_dma.o
=======
ttm-y := ttm_tt.o ttm_bo.o ttm_bo_util.o ttm_bo_vm.o ttm_module.o \
	ttm_execbuf_util.o ttm_range_manager.o ttm_resource.o ttm_pool.o \
	ttm_device.o ttm_sys_manager.o
>>>>>>> upstream/android-13
ttm-$(CONFIG_AGP) += ttm_agp_backend.o

obj-$(CONFIG_DRM_TTM) += ttm.o
