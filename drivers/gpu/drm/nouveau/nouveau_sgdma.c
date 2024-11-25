<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
// SPDX-License-Identifier: MIT
>>>>>>> upstream/android-13
#include <linux/pagemap.h>
#include <linux/slab.h>

#include "nouveau_drv.h"
#include "nouveau_mem.h"
#include "nouveau_ttm.h"
<<<<<<< HEAD
=======
#include "nouveau_bo.h"
>>>>>>> upstream/android-13

struct nouveau_sgdma_be {
	/* this has to be the first field so populate/unpopulated in
	 * nouve_bo.c works properly, otherwise have to move them here
	 */
<<<<<<< HEAD
	struct ttm_dma_tt ttm;
	struct nouveau_mem *mem;
};

static void
nouveau_sgdma_destroy(struct ttm_tt *ttm)
=======
	struct ttm_tt ttm;
	struct nouveau_mem *mem;
};

void
nouveau_sgdma_destroy(struct ttm_device *bdev, struct ttm_tt *ttm)
>>>>>>> upstream/android-13
{
	struct nouveau_sgdma_be *nvbe = (struct nouveau_sgdma_be *)ttm;

	if (ttm) {
<<<<<<< HEAD
		ttm_dma_tt_fini(&nvbe->ttm);
=======
		nouveau_sgdma_unbind(bdev, ttm);
		ttm_tt_destroy_common(bdev, ttm);
		ttm_tt_fini(&nvbe->ttm);
>>>>>>> upstream/android-13
		kfree(nvbe);
	}
}

<<<<<<< HEAD
static int
nv04_sgdma_bind(struct ttm_tt *ttm, struct ttm_mem_reg *reg)
{
	struct nouveau_sgdma_be *nvbe = (struct nouveau_sgdma_be *)ttm;
	struct nouveau_mem *mem = nouveau_mem(reg);
	int ret;

=======
int
nouveau_sgdma_bind(struct ttm_device *bdev, struct ttm_tt *ttm, struct ttm_resource *reg)
{
	struct nouveau_sgdma_be *nvbe = (struct nouveau_sgdma_be *)ttm;
	struct nouveau_drm *drm = nouveau_bdev(bdev);
	struct nouveau_mem *mem = nouveau_mem(reg);
	int ret;

	if (nvbe->mem)
		return 0;

>>>>>>> upstream/android-13
	ret = nouveau_mem_host(reg, &nvbe->ttm);
	if (ret)
		return ret;

<<<<<<< HEAD
	ret = nouveau_mem_map(mem, &mem->cli->vmm.vmm, &mem->vma[0]);
	if (ret) {
		nouveau_mem_fini(mem);
		return ret;
=======
	if (drm->client.device.info.family < NV_DEVICE_INFO_V0_TESLA) {
		ret = nouveau_mem_map(mem, &mem->cli->vmm.vmm, &mem->vma[0]);
		if (ret) {
			nouveau_mem_fini(mem);
			return ret;
		}
>>>>>>> upstream/android-13
	}

	nvbe->mem = mem;
	return 0;
}

<<<<<<< HEAD
static int
nv04_sgdma_unbind(struct ttm_tt *ttm)
{
	struct nouveau_sgdma_be *nvbe = (struct nouveau_sgdma_be *)ttm;
	nouveau_mem_fini(nvbe->mem);
	return 0;
}

static struct ttm_backend_func nv04_sgdma_backend = {
	.bind			= nv04_sgdma_bind,
	.unbind			= nv04_sgdma_unbind,
	.destroy		= nouveau_sgdma_destroy
};

static int
nv50_sgdma_bind(struct ttm_tt *ttm, struct ttm_mem_reg *reg)
{
	struct nouveau_sgdma_be *nvbe = (struct nouveau_sgdma_be *)ttm;
	struct nouveau_mem *mem = nouveau_mem(reg);
	int ret;

	ret = nouveau_mem_host(reg, &nvbe->ttm);
	if (ret)
		return ret;

	nvbe->mem = mem;
	return 0;
}

static struct ttm_backend_func nv50_sgdma_backend = {
	.bind			= nv50_sgdma_bind,
	.unbind			= nv04_sgdma_unbind,
	.destroy		= nouveau_sgdma_destroy
};

=======
void
nouveau_sgdma_unbind(struct ttm_device *bdev, struct ttm_tt *ttm)
{
	struct nouveau_sgdma_be *nvbe = (struct nouveau_sgdma_be *)ttm;
	if (nvbe->mem) {
		nouveau_mem_fini(nvbe->mem);
		nvbe->mem = NULL;
	}
}

>>>>>>> upstream/android-13
struct ttm_tt *
nouveau_sgdma_create_ttm(struct ttm_buffer_object *bo, uint32_t page_flags)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
<<<<<<< HEAD
	struct nouveau_sgdma_be *nvbe;
=======
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	struct nouveau_sgdma_be *nvbe;
	enum ttm_caching caching;

	if (nvbo->force_coherent)
		caching = ttm_uncached;
	else if (drm->agp.bridge)
		caching = ttm_write_combined;
	else
		caching = ttm_cached;
>>>>>>> upstream/android-13

	nvbe = kzalloc(sizeof(*nvbe), GFP_KERNEL);
	if (!nvbe)
		return NULL;

<<<<<<< HEAD
	if (drm->client.device.info.family < NV_DEVICE_INFO_V0_TESLA)
		nvbe->ttm.ttm.func = &nv04_sgdma_backend;
	else
		nvbe->ttm.ttm.func = &nv50_sgdma_backend;

	if (ttm_dma_tt_init(&nvbe->ttm, bo, page_flags)) {
		kfree(nvbe);
		return NULL;
	}
	return &nvbe->ttm.ttm;
=======
	if (ttm_sg_tt_init(&nvbe->ttm, bo, page_flags, caching)) {
		kfree(nvbe);
		return NULL;
	}
	return &nvbe->ttm;
>>>>>>> upstream/android-13
}
