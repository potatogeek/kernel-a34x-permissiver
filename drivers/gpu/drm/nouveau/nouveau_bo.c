/*
 * Copyright 2007 Dave Airlied
 * All Rights Reserved.
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
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Authors: Dave Airlied <airlied@linux.ie>
 *	    Ben Skeggs   <darktama@iinet.net.au>
 *	    Jeremy Kolb  <jkolb@brandeis.edu>
 */

#include <linux/dma-mapping.h>
<<<<<<< HEAD
#include <linux/swiotlb.h>

#include "nouveau_drv.h"
#include "nouveau_dma.h"
=======

#include "nouveau_drv.h"
#include "nouveau_chan.h"
>>>>>>> upstream/android-13
#include "nouveau_fence.h"

#include "nouveau_bo.h"
#include "nouveau_ttm.h"
#include "nouveau_gem.h"
#include "nouveau_mem.h"
#include "nouveau_vmm.h"

#include <nvif/class.h>
#include <nvif/if500b.h>
#include <nvif/if900b.h>

<<<<<<< HEAD
=======
static int nouveau_ttm_tt_bind(struct ttm_device *bdev, struct ttm_tt *ttm,
			       struct ttm_resource *reg);
static void nouveau_ttm_tt_unbind(struct ttm_device *bdev, struct ttm_tt *ttm);

>>>>>>> upstream/android-13
/*
 * NV10-NV40 tiling helpers
 */

static void
nv10_bo_update_tile_region(struct drm_device *dev, struct nouveau_drm_tile *reg,
			   u32 addr, u32 size, u32 pitch, u32 flags)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	int i = reg - drm->tile.reg;
	struct nvkm_fb *fb = nvxx_fb(&drm->client.device);
	struct nvkm_fb_tile *tile = &fb->tile.region[i];

	nouveau_fence_unref(&reg->fence);

	if (tile->pitch)
		nvkm_fb_tile_fini(fb, i, tile);

	if (pitch)
		nvkm_fb_tile_init(fb, i, addr, size, pitch, flags, tile);

	nvkm_fb_tile_prog(fb, i, tile);
}

static struct nouveau_drm_tile *
nv10_bo_get_tile_region(struct drm_device *dev, int i)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_drm_tile *tile = &drm->tile.reg[i];

	spin_lock(&drm->tile.lock);

	if (!tile->used &&
	    (!tile->fence || nouveau_fence_done(tile->fence)))
		tile->used = true;
	else
		tile = NULL;

	spin_unlock(&drm->tile.lock);
	return tile;
}

static void
nv10_bo_put_tile_region(struct drm_device *dev, struct nouveau_drm_tile *tile,
			struct dma_fence *fence)
{
	struct nouveau_drm *drm = nouveau_drm(dev);

	if (tile) {
		spin_lock(&drm->tile.lock);
		tile->fence = (struct nouveau_fence *)dma_fence_get(fence);
		tile->used = false;
		spin_unlock(&drm->tile.lock);
	}
}

static struct nouveau_drm_tile *
nv10_bo_set_tiling(struct drm_device *dev, u32 addr,
		   u32 size, u32 pitch, u32 zeta)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nvkm_fb *fb = nvxx_fb(&drm->client.device);
	struct nouveau_drm_tile *tile, *found = NULL;
	int i;

	for (i = 0; i < fb->tile.regions; i++) {
		tile = nv10_bo_get_tile_region(dev, i);

		if (pitch && !found) {
			found = tile;
			continue;

		} else if (tile && fb->tile.region[i].pitch) {
			/* Kill an unused tile region. */
			nv10_bo_update_tile_region(dev, tile, 0, 0, 0, 0);
		}

		nv10_bo_put_tile_region(dev, tile, NULL);
	}

	if (found)
		nv10_bo_update_tile_region(dev, found, addr, size, pitch, zeta);
	return found;
}

static void
nouveau_bo_del_ttm(struct ttm_buffer_object *bo)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct drm_device *dev = drm->dev;
	struct nouveau_bo *nvbo = nouveau_bo(bo);

<<<<<<< HEAD
	if (unlikely(nvbo->gem.filp))
		DRM_ERROR("bo %p still attached to GEM object\n", bo);
	WARN_ON(nvbo->pin_refcnt > 0);
	nv10_bo_put_tile_region(dev, nvbo->tile, NULL);
=======
	WARN_ON(nvbo->bo.pin_count > 0);
	nouveau_bo_del_io_reserve_lru(bo);
	nv10_bo_put_tile_region(dev, nvbo->tile, NULL);

	/*
	 * If nouveau_bo_new() allocated this buffer, the GEM object was never
	 * initialized, so don't attempt to release it.
	 */
	if (bo->base.dev)
		drm_gem_object_release(&bo->base);
	else
		dma_resv_fini(&bo->base._resv);

>>>>>>> upstream/android-13
	kfree(nvbo);
}

static inline u64
roundup_64(u64 x, u32 y)
{
	x += y - 1;
	do_div(x, y);
	return x * y;
}

static void
<<<<<<< HEAD
nouveau_bo_fixup_align(struct nouveau_bo *nvbo, u32 flags,
		       int *align, u64 *size)
=======
nouveau_bo_fixup_align(struct nouveau_bo *nvbo, int *align, u64 *size)
>>>>>>> upstream/android-13
{
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	struct nvif_device *device = &drm->client.device;

	if (device->info.family < NV_DEVICE_INFO_V0_TESLA) {
		if (nvbo->mode) {
			if (device->info.chipset >= 0x40) {
				*align = 65536;
				*size = roundup_64(*size, 64 * nvbo->mode);

			} else if (device->info.chipset >= 0x30) {
				*align = 32768;
				*size = roundup_64(*size, 64 * nvbo->mode);

			} else if (device->info.chipset >= 0x20) {
				*align = 16384;
				*size = roundup_64(*size, 64 * nvbo->mode);

			} else if (device->info.chipset >= 0x10) {
				*align = 16384;
				*size = roundup_64(*size, 32 * nvbo->mode);
			}
		}
	} else {
		*size = roundup_64(*size, (1 << nvbo->page));
		*align = max((1 <<  nvbo->page), *align);
	}

	*size = roundup_64(*size, PAGE_SIZE);
}

<<<<<<< HEAD
int
nouveau_bo_new(struct nouveau_cli *cli, u64 size, int align,
	       uint32_t flags, uint32_t tile_mode, uint32_t tile_flags,
	       struct sg_table *sg, struct reservation_object *robj,
	       struct nouveau_bo **pnvbo)
=======
struct nouveau_bo *
nouveau_bo_alloc(struct nouveau_cli *cli, u64 *size, int *align, u32 domain,
		 u32 tile_mode, u32 tile_flags)
>>>>>>> upstream/android-13
{
	struct nouveau_drm *drm = cli->drm;
	struct nouveau_bo *nvbo;
	struct nvif_mmu *mmu = &cli->mmu;
<<<<<<< HEAD
	struct nvif_vmm *vmm = &cli->vmm.vmm;
	size_t acc_size;
	int type = ttm_bo_type_device;
	int ret, i, pi = -1;

	if (!size) {
		NV_WARN(drm, "skipped size %016llx\n", size);
		return -EINVAL;
	}

	if (sg)
		type = ttm_bo_type_sg;

	nvbo = kzalloc(sizeof(struct nouveau_bo), GFP_KERNEL);
	if (!nvbo)
		return -ENOMEM;
=======
	struct nvif_vmm *vmm = cli->svm.cli ? &cli->svm.vmm : &cli->vmm.vmm;
	int i, pi = -1;

	if (!*size) {
		NV_WARN(drm, "skipped size %016llx\n", *size);
		return ERR_PTR(-EINVAL);
	}

	nvbo = kzalloc(sizeof(struct nouveau_bo), GFP_KERNEL);
	if (!nvbo)
		return ERR_PTR(-ENOMEM);
>>>>>>> upstream/android-13
	INIT_LIST_HEAD(&nvbo->head);
	INIT_LIST_HEAD(&nvbo->entry);
	INIT_LIST_HEAD(&nvbo->vma_list);
	nvbo->bo.bdev = &drm->ttm.bdev;

	/* This is confusing, and doesn't actually mean we want an uncached
	 * mapping, but is what NOUVEAU_GEM_DOMAIN_COHERENT gets translated
	 * into in nouveau_gem_new().
	 */
<<<<<<< HEAD
	if (flags & TTM_PL_FLAG_UNCACHED) {
=======
	if (domain & NOUVEAU_GEM_DOMAIN_COHERENT) {
>>>>>>> upstream/android-13
		/* Determine if we can get a cache-coherent map, forcing
		 * uncached mapping if we can't.
		 */
		if (!nouveau_drm_use_coherent_gpu_mapping(drm))
			nvbo->force_coherent = true;
	}

	if (cli->device.info.family >= NV_DEVICE_INFO_V0_FERMI) {
		nvbo->kind = (tile_flags & 0x0000ff00) >> 8;
		if (!nvif_mmu_kind_valid(mmu, nvbo->kind)) {
			kfree(nvbo);
<<<<<<< HEAD
			return -EINVAL;
=======
			return ERR_PTR(-EINVAL);
>>>>>>> upstream/android-13
		}

		nvbo->comp = mmu->kind[nvbo->kind] != nvbo->kind;
	} else
	if (cli->device.info.family >= NV_DEVICE_INFO_V0_TESLA) {
		nvbo->kind = (tile_flags & 0x00007f00) >> 8;
		nvbo->comp = (tile_flags & 0x00030000) >> 16;
		if (!nvif_mmu_kind_valid(mmu, nvbo->kind)) {
			kfree(nvbo);
<<<<<<< HEAD
			return -EINVAL;
=======
			return ERR_PTR(-EINVAL);
>>>>>>> upstream/android-13
		}
	} else {
		nvbo->zeta = (tile_flags & 0x00000007);
	}
	nvbo->mode = tile_mode;
	nvbo->contig = !(tile_flags & NOUVEAU_GEM_TILE_NONCONTIG);

	/* Determine the desirable target GPU page size for the buffer. */
	for (i = 0; i < vmm->page_nr; i++) {
		/* Because we cannot currently allow VMM maps to fail
		 * during buffer migration, we need to determine page
		 * size for the buffer up-front, and pre-allocate its
		 * page tables.
		 *
		 * Skip page sizes that can't support needed domains.
		 */
		if (cli->device.info.family > NV_DEVICE_INFO_V0_CURIE &&
<<<<<<< HEAD
		    (flags & TTM_PL_FLAG_VRAM) && !vmm->page[i].vram)
			continue;
		if ((flags & TTM_PL_FLAG_TT) &&
=======
		    (domain & NOUVEAU_GEM_DOMAIN_VRAM) && !vmm->page[i].vram)
			continue;
		if ((domain & NOUVEAU_GEM_DOMAIN_GART) &&
>>>>>>> upstream/android-13
		    (!vmm->page[i].host || vmm->page[i].shift > PAGE_SHIFT))
			continue;

		/* Select this page size if it's the first that supports
		 * the potential memory domains, or when it's compatible
		 * with the requested compression settings.
		 */
		if (pi < 0 || !nvbo->comp || vmm->page[i].comp)
			pi = i;

		/* Stop once the buffer is larger than the current page size. */
<<<<<<< HEAD
		if (size >= 1ULL << vmm->page[i].shift)
=======
		if (*size >= 1ULL << vmm->page[i].shift)
>>>>>>> upstream/android-13
			break;
	}

	if (WARN_ON(pi < 0))
<<<<<<< HEAD
		return -EINVAL;
=======
		return ERR_PTR(-EINVAL);
>>>>>>> upstream/android-13

	/* Disable compression if suitable settings couldn't be found. */
	if (nvbo->comp && !vmm->page[pi].comp) {
		if (mmu->object.oclass >= NVIF_CLASS_MMU_GF100)
			nvbo->kind = mmu->kind[nvbo->kind];
		nvbo->comp = 0;
	}
	nvbo->page = vmm->page[pi].shift;

<<<<<<< HEAD
	nouveau_bo_fixup_align(nvbo, flags, &align, &size);
	nvbo->bo.mem.num_pages = size >> PAGE_SHIFT;
	nouveau_bo_placement_set(nvbo, flags, 0);

	acc_size = ttm_bo_dma_acc_size(&drm->ttm.bdev, size,
				       sizeof(struct nouveau_bo));

	ret = ttm_bo_init(&drm->ttm.bdev, &nvbo->bo, size,
			  type, &nvbo->placement,
			  align >> PAGE_SHIFT, false, acc_size, sg,
=======
	nouveau_bo_fixup_align(nvbo, align, size);

	return nvbo;
}

int
nouveau_bo_init(struct nouveau_bo *nvbo, u64 size, int align, u32 domain,
		struct sg_table *sg, struct dma_resv *robj)
{
	int type = sg ? ttm_bo_type_sg : ttm_bo_type_device;
	int ret;

	nouveau_bo_placement_set(nvbo, domain, 0);
	INIT_LIST_HEAD(&nvbo->io_reserve_lru);

	ret = ttm_bo_init(nvbo->bo.bdev, &nvbo->bo, size, type,
			  &nvbo->placement, align >> PAGE_SHIFT, false, sg,
>>>>>>> upstream/android-13
			  robj, nouveau_bo_del_ttm);
	if (ret) {
		/* ttm will call nouveau_bo_del_ttm if it fails.. */
		return ret;
	}

<<<<<<< HEAD
=======
	return 0;
}

int
nouveau_bo_new(struct nouveau_cli *cli, u64 size, int align,
	       uint32_t domain, uint32_t tile_mode, uint32_t tile_flags,
	       struct sg_table *sg, struct dma_resv *robj,
	       struct nouveau_bo **pnvbo)
{
	struct nouveau_bo *nvbo;
	int ret;

	nvbo = nouveau_bo_alloc(cli, &size, &align, domain, tile_mode,
				tile_flags);
	if (IS_ERR(nvbo))
		return PTR_ERR(nvbo);

	nvbo->bo.base.size = size;
	dma_resv_init(&nvbo->bo.base._resv);
	drm_vma_node_reset(&nvbo->bo.base.vma_node);

	ret = nouveau_bo_init(nvbo, size, align, domain, sg, robj);
	if (ret)
		return ret;

>>>>>>> upstream/android-13
	*pnvbo = nvbo;
	return 0;
}

static void
<<<<<<< HEAD
set_placement_list(struct ttm_place *pl, unsigned *n, uint32_t type, uint32_t flags)
{
	*n = 0;

	if (type & TTM_PL_FLAG_VRAM)
		pl[(*n)++].flags = TTM_PL_FLAG_VRAM | flags;
	if (type & TTM_PL_FLAG_TT)
		pl[(*n)++].flags = TTM_PL_FLAG_TT | flags;
	if (type & TTM_PL_FLAG_SYSTEM)
		pl[(*n)++].flags = TTM_PL_FLAG_SYSTEM | flags;
}

static void
set_placement_range(struct nouveau_bo *nvbo, uint32_t type)
{
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	u32 vram_pages = drm->client.device.info.ram_size >> PAGE_SHIFT;
	unsigned i, fpfn, lpfn;

	if (drm->client.device.info.family == NV_DEVICE_INFO_V0_CELSIUS &&
	    nvbo->mode && (type & TTM_PL_FLAG_VRAM) &&
	    nvbo->bo.mem.num_pages < vram_pages / 4) {
=======
set_placement_list(struct ttm_place *pl, unsigned *n, uint32_t domain)
{
	*n = 0;

	if (domain & NOUVEAU_GEM_DOMAIN_VRAM) {
		pl[*n].mem_type = TTM_PL_VRAM;
		pl[*n].flags = 0;
		(*n)++;
	}
	if (domain & NOUVEAU_GEM_DOMAIN_GART) {
		pl[*n].mem_type = TTM_PL_TT;
		pl[*n].flags = 0;
		(*n)++;
	}
	if (domain & NOUVEAU_GEM_DOMAIN_CPU) {
		pl[*n].mem_type = TTM_PL_SYSTEM;
		pl[(*n)++].flags = 0;
	}
}

static void
set_placement_range(struct nouveau_bo *nvbo, uint32_t domain)
{
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	u64 vram_size = drm->client.device.info.ram_size;
	unsigned i, fpfn, lpfn;

	if (drm->client.device.info.family == NV_DEVICE_INFO_V0_CELSIUS &&
	    nvbo->mode && (domain & NOUVEAU_GEM_DOMAIN_VRAM) &&
	    nvbo->bo.base.size < vram_size / 4) {
>>>>>>> upstream/android-13
		/*
		 * Make sure that the color and depth buffers are handled
		 * by independent memory controller units. Up to a 9x
		 * speed up when alpha-blending and depth-test are enabled
		 * at the same time.
		 */
		if (nvbo->zeta) {
<<<<<<< HEAD
			fpfn = vram_pages / 2;
			lpfn = ~0;
		} else {
			fpfn = 0;
			lpfn = vram_pages / 2;
=======
			fpfn = (vram_size / 2) >> PAGE_SHIFT;
			lpfn = ~0;
		} else {
			fpfn = 0;
			lpfn = (vram_size / 2) >> PAGE_SHIFT;
>>>>>>> upstream/android-13
		}
		for (i = 0; i < nvbo->placement.num_placement; ++i) {
			nvbo->placements[i].fpfn = fpfn;
			nvbo->placements[i].lpfn = lpfn;
		}
		for (i = 0; i < nvbo->placement.num_busy_placement; ++i) {
			nvbo->busy_placements[i].fpfn = fpfn;
			nvbo->busy_placements[i].lpfn = lpfn;
		}
	}
}

void
<<<<<<< HEAD
nouveau_bo_placement_set(struct nouveau_bo *nvbo, uint32_t type, uint32_t busy)
{
	struct ttm_placement *pl = &nvbo->placement;
	uint32_t flags = (nvbo->force_coherent ? TTM_PL_FLAG_UNCACHED :
						 TTM_PL_MASK_CACHING) |
			 (nvbo->pin_refcnt ? TTM_PL_FLAG_NO_EVICT : 0);

	pl->placement = nvbo->placements;
	set_placement_list(nvbo->placements, &pl->num_placement,
			   type, flags);

	pl->busy_placement = nvbo->busy_placements;
	set_placement_list(nvbo->busy_placements, &pl->num_busy_placement,
			   type | busy, flags);

	set_placement_range(nvbo, type);
}

int
nouveau_bo_pin(struct nouveau_bo *nvbo, uint32_t memtype, bool contig)
=======
nouveau_bo_placement_set(struct nouveau_bo *nvbo, uint32_t domain,
			 uint32_t busy)
{
	struct ttm_placement *pl = &nvbo->placement;

	pl->placement = nvbo->placements;
	set_placement_list(nvbo->placements, &pl->num_placement, domain);

	pl->busy_placement = nvbo->busy_placements;
	set_placement_list(nvbo->busy_placements, &pl->num_busy_placement,
			   domain | busy);

	set_placement_range(nvbo, domain);
}

int
nouveau_bo_pin(struct nouveau_bo *nvbo, uint32_t domain, bool contig)
>>>>>>> upstream/android-13
{
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	struct ttm_buffer_object *bo = &nvbo->bo;
	bool force = false, evict = false;
	int ret;

	ret = ttm_bo_reserve(bo, false, false, NULL);
	if (ret)
		return ret;

	if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_TESLA &&
<<<<<<< HEAD
	    memtype == TTM_PL_FLAG_VRAM && contig) {
=======
	    domain == NOUVEAU_GEM_DOMAIN_VRAM && contig) {
>>>>>>> upstream/android-13
		if (!nvbo->contig) {
			nvbo->contig = true;
			force = true;
			evict = true;
		}
	}

<<<<<<< HEAD
	if (nvbo->pin_refcnt) {
		if (!(memtype & (1 << bo->mem.mem_type)) || evict) {
			NV_ERROR(drm, "bo %p pinned elsewhere: "
				      "0x%08x vs 0x%08x\n", bo,
				 1 << bo->mem.mem_type, memtype);
			ret = -EBUSY;
		}
		nvbo->pin_refcnt++;
=======
	if (nvbo->bo.pin_count) {
		bool error = evict;

		switch (bo->resource->mem_type) {
		case TTM_PL_VRAM:
			error |= !(domain & NOUVEAU_GEM_DOMAIN_VRAM);
			break;
		case TTM_PL_TT:
			error |= !(domain & NOUVEAU_GEM_DOMAIN_GART);
			break;
		default:
			break;
		}

		if (error) {
			NV_ERROR(drm, "bo %p pinned elsewhere: "
				      "0x%08x vs 0x%08x\n", bo,
				 bo->resource->mem_type, domain);
			ret = -EBUSY;
		}
		ttm_bo_pin(&nvbo->bo);
>>>>>>> upstream/android-13
		goto out;
	}

	if (evict) {
<<<<<<< HEAD
		nouveau_bo_placement_set(nvbo, TTM_PL_FLAG_TT, 0);
=======
		nouveau_bo_placement_set(nvbo, NOUVEAU_GEM_DOMAIN_GART, 0);
>>>>>>> upstream/android-13
		ret = nouveau_bo_validate(nvbo, false, false);
		if (ret)
			goto out;
	}

<<<<<<< HEAD
	nvbo->pin_refcnt++;
	nouveau_bo_placement_set(nvbo, memtype, 0);

	/* drop pin_refcnt temporarily, so we don't trip the assertion
	 * in nouveau_bo_move() that makes sure we're not trying to
	 * move a pinned buffer
	 */
	nvbo->pin_refcnt--;
	ret = nouveau_bo_validate(nvbo, false, false);
	if (ret)
		goto out;
	nvbo->pin_refcnt++;

	switch (bo->mem.mem_type) {
	case TTM_PL_VRAM:
		drm->gem.vram_available -= bo->mem.size;
		break;
	case TTM_PL_TT:
		drm->gem.gart_available -= bo->mem.size;
=======
	nouveau_bo_placement_set(nvbo, domain, 0);
	ret = nouveau_bo_validate(nvbo, false, false);
	if (ret)
		goto out;

	ttm_bo_pin(&nvbo->bo);

	switch (bo->resource->mem_type) {
	case TTM_PL_VRAM:
		drm->gem.vram_available -= bo->base.size;
		break;
	case TTM_PL_TT:
		drm->gem.gart_available -= bo->base.size;
>>>>>>> upstream/android-13
		break;
	default:
		break;
	}

out:
	if (force && ret)
		nvbo->contig = false;
	ttm_bo_unreserve(bo);
	return ret;
}

int
nouveau_bo_unpin(struct nouveau_bo *nvbo)
{
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	struct ttm_buffer_object *bo = &nvbo->bo;
<<<<<<< HEAD
	int ret, ref;
=======
	int ret;
>>>>>>> upstream/android-13

	ret = ttm_bo_reserve(bo, false, false, NULL);
	if (ret)
		return ret;

<<<<<<< HEAD
	ref = --nvbo->pin_refcnt;
	WARN_ON_ONCE(ref < 0);
	if (ref)
		goto out;

	nouveau_bo_placement_set(nvbo, bo->mem.placement, 0);

	ret = nouveau_bo_validate(nvbo, false, false);
	if (ret == 0) {
		switch (bo->mem.mem_type) {
		case TTM_PL_VRAM:
			drm->gem.vram_available += bo->mem.size;
			break;
		case TTM_PL_TT:
			drm->gem.gart_available += bo->mem.size;
=======
	ttm_bo_unpin(&nvbo->bo);
	if (!nvbo->bo.pin_count) {
		switch (bo->resource->mem_type) {
		case TTM_PL_VRAM:
			drm->gem.vram_available += bo->base.size;
			break;
		case TTM_PL_TT:
			drm->gem.gart_available += bo->base.size;
>>>>>>> upstream/android-13
			break;
		default:
			break;
		}
	}

<<<<<<< HEAD
out:
	ttm_bo_unreserve(bo);
	return ret;
=======
	ttm_bo_unreserve(bo);
	return 0;
>>>>>>> upstream/android-13
}

int
nouveau_bo_map(struct nouveau_bo *nvbo)
{
	int ret;

	ret = ttm_bo_reserve(&nvbo->bo, false, false, NULL);
	if (ret)
		return ret;

<<<<<<< HEAD
	ret = ttm_bo_kmap(&nvbo->bo, 0, nvbo->bo.mem.num_pages, &nvbo->kmap);
=======
	ret = ttm_bo_kmap(&nvbo->bo, 0, nvbo->bo.resource->num_pages, &nvbo->kmap);
>>>>>>> upstream/android-13

	ttm_bo_unreserve(&nvbo->bo);
	return ret;
}

void
nouveau_bo_unmap(struct nouveau_bo *nvbo)
{
	if (!nvbo)
		return;

	ttm_bo_kunmap(&nvbo->kmap);
}

void
nouveau_bo_sync_for_device(struct nouveau_bo *nvbo)
{
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
<<<<<<< HEAD
	struct ttm_dma_tt *ttm_dma = (struct ttm_dma_tt *)nvbo->bo.ttm;
	int i;

	if (!ttm_dma)
		return;
=======
	struct ttm_tt *ttm_dma = (struct ttm_tt *)nvbo->bo.ttm;
	int i, j;

	if (!ttm_dma || !ttm_dma->dma_address)
		return;
	if (!ttm_dma->pages) {
		NV_DEBUG(drm, "ttm_dma 0x%p: pages NULL\n", ttm_dma);
		return;
	}
>>>>>>> upstream/android-13

	/* Don't waste time looping if the object is coherent */
	if (nvbo->force_coherent)
		return;

<<<<<<< HEAD
	for (i = 0; i < ttm_dma->ttm.num_pages; i++)
		dma_sync_single_for_device(drm->dev->dev,
					   ttm_dma->dma_address[i],
					   PAGE_SIZE, DMA_TO_DEVICE);
=======
	i = 0;
	while (i < ttm_dma->num_pages) {
		struct page *p = ttm_dma->pages[i];
		size_t num_pages = 1;

		for (j = i + 1; j < ttm_dma->num_pages; ++j) {
			if (++p != ttm_dma->pages[j])
				break;

			++num_pages;
		}
		dma_sync_single_for_device(drm->dev->dev,
					   ttm_dma->dma_address[i],
					   num_pages * PAGE_SIZE, DMA_TO_DEVICE);
		i += num_pages;
	}
>>>>>>> upstream/android-13
}

void
nouveau_bo_sync_for_cpu(struct nouveau_bo *nvbo)
{
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
<<<<<<< HEAD
	struct ttm_dma_tt *ttm_dma = (struct ttm_dma_tt *)nvbo->bo.ttm;
	int i;

	if (!ttm_dma)
		return;
=======
	struct ttm_tt *ttm_dma = (struct ttm_tt *)nvbo->bo.ttm;
	int i, j;

	if (!ttm_dma || !ttm_dma->dma_address)
		return;
	if (!ttm_dma->pages) {
		NV_DEBUG(drm, "ttm_dma 0x%p: pages NULL\n", ttm_dma);
		return;
	}
>>>>>>> upstream/android-13

	/* Don't waste time looping if the object is coherent */
	if (nvbo->force_coherent)
		return;

<<<<<<< HEAD
	for (i = 0; i < ttm_dma->ttm.num_pages; i++)
		dma_sync_single_for_cpu(drm->dev->dev, ttm_dma->dma_address[i],
					PAGE_SIZE, DMA_FROM_DEVICE);
=======
	i = 0;
	while (i < ttm_dma->num_pages) {
		struct page *p = ttm_dma->pages[i];
		size_t num_pages = 1;

		for (j = i + 1; j < ttm_dma->num_pages; ++j) {
			if (++p != ttm_dma->pages[j])
				break;

			++num_pages;
		}

		dma_sync_single_for_cpu(drm->dev->dev, ttm_dma->dma_address[i],
					num_pages * PAGE_SIZE, DMA_FROM_DEVICE);
		i += num_pages;
	}
}

void nouveau_bo_add_io_reserve_lru(struct ttm_buffer_object *bo)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct nouveau_bo *nvbo = nouveau_bo(bo);

	mutex_lock(&drm->ttm.io_reserve_mutex);
	list_move_tail(&nvbo->io_reserve_lru, &drm->ttm.io_reserve_lru);
	mutex_unlock(&drm->ttm.io_reserve_mutex);
}

void nouveau_bo_del_io_reserve_lru(struct ttm_buffer_object *bo)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct nouveau_bo *nvbo = nouveau_bo(bo);

	mutex_lock(&drm->ttm.io_reserve_mutex);
	list_del_init(&nvbo->io_reserve_lru);
	mutex_unlock(&drm->ttm.io_reserve_mutex);
>>>>>>> upstream/android-13
}

int
nouveau_bo_validate(struct nouveau_bo *nvbo, bool interruptible,
		    bool no_wait_gpu)
{
	struct ttm_operation_ctx ctx = { interruptible, no_wait_gpu };
	int ret;

	ret = ttm_bo_validate(&nvbo->bo, &nvbo->placement, &ctx);
	if (ret)
		return ret;

	nouveau_bo_sync_for_device(nvbo);

	return 0;
}

void
nouveau_bo_wr16(struct nouveau_bo *nvbo, unsigned index, u16 val)
{
	bool is_iomem;
	u16 *mem = ttm_kmap_obj_virtual(&nvbo->kmap, &is_iomem);

	mem += index;

	if (is_iomem)
		iowrite16_native(val, (void __force __iomem *)mem);
	else
		*mem = val;
}

u32
nouveau_bo_rd32(struct nouveau_bo *nvbo, unsigned index)
{
	bool is_iomem;
	u32 *mem = ttm_kmap_obj_virtual(&nvbo->kmap, &is_iomem);

	mem += index;

	if (is_iomem)
		return ioread32_native((void __force __iomem *)mem);
	else
		return *mem;
}

void
nouveau_bo_wr32(struct nouveau_bo *nvbo, unsigned index, u32 val)
{
	bool is_iomem;
	u32 *mem = ttm_kmap_obj_virtual(&nvbo->kmap, &is_iomem);

	mem += index;

	if (is_iomem)
		iowrite32_native(val, (void __force __iomem *)mem);
	else
		*mem = val;
}

static struct ttm_tt *
nouveau_ttm_tt_create(struct ttm_buffer_object *bo, uint32_t page_flags)
{
#if IS_ENABLED(CONFIG_AGP)
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);

	if (drm->agp.bridge) {
		return ttm_agp_tt_create(bo, drm->agp.bridge, page_flags);
	}
#endif

	return nouveau_sgdma_create_ttm(bo, page_flags);
}

static int
<<<<<<< HEAD
nouveau_bo_invalidate_caches(struct ttm_bo_device *bdev, uint32_t flags)
{
	/* We'll do this from user space. */
	return 0;
}

static int
nouveau_bo_init_mem_type(struct ttm_bo_device *bdev, uint32_t type,
			 struct ttm_mem_type_manager *man)
{
	struct nouveau_drm *drm = nouveau_bdev(bdev);
	struct nvif_mmu *mmu = &drm->client.mmu;

	switch (type) {
	case TTM_PL_SYSTEM:
		man->flags = TTM_MEMTYPE_FLAG_MAPPABLE;
		man->available_caching = TTM_PL_MASK_CACHING;
		man->default_caching = TTM_PL_FLAG_CACHED;
		break;
	case TTM_PL_VRAM:
		man->flags = TTM_MEMTYPE_FLAG_FIXED |
			     TTM_MEMTYPE_FLAG_MAPPABLE;
		man->available_caching = TTM_PL_FLAG_UNCACHED |
					 TTM_PL_FLAG_WC;
		man->default_caching = TTM_PL_FLAG_WC;

		if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_TESLA) {
			/* Some BARs do not support being ioremapped WC */
			const u8 type = mmu->type[drm->ttm.type_vram].type;
			if (type & NVIF_MEM_UNCACHED) {
				man->available_caching = TTM_PL_FLAG_UNCACHED;
				man->default_caching = TTM_PL_FLAG_UNCACHED;
			}

			man->func = &nouveau_vram_manager;
			man->io_reserve_fastpath = false;
			man->use_io_reserve_lru = true;
		} else {
			man->func = &ttm_bo_manager_func;
		}
		break;
	case TTM_PL_TT:
		if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_TESLA)
			man->func = &nouveau_gart_manager;
		else
		if (!drm->agp.bridge)
			man->func = &nv04_gart_manager;
		else
			man->func = &ttm_bo_manager_func;

		if (drm->agp.bridge) {
			man->flags = TTM_MEMTYPE_FLAG_MAPPABLE;
			man->available_caching = TTM_PL_FLAG_UNCACHED |
				TTM_PL_FLAG_WC;
			man->default_caching = TTM_PL_FLAG_WC;
		} else {
			man->flags = TTM_MEMTYPE_FLAG_MAPPABLE |
				     TTM_MEMTYPE_FLAG_CMA;
			man->available_caching = TTM_PL_MASK_CACHING;
			man->default_caching = TTM_PL_FLAG_CACHED;
		}

		break;
	default:
		return -EINVAL;
	}
	return 0;
=======
nouveau_ttm_tt_bind(struct ttm_device *bdev, struct ttm_tt *ttm,
		    struct ttm_resource *reg)
{
#if IS_ENABLED(CONFIG_AGP)
	struct nouveau_drm *drm = nouveau_bdev(bdev);
#endif
	if (!reg)
		return -EINVAL;
#if IS_ENABLED(CONFIG_AGP)
	if (drm->agp.bridge)
		return ttm_agp_bind(ttm, reg);
#endif
	return nouveau_sgdma_bind(bdev, ttm, reg);
}

static void
nouveau_ttm_tt_unbind(struct ttm_device *bdev, struct ttm_tt *ttm)
{
#if IS_ENABLED(CONFIG_AGP)
	struct nouveau_drm *drm = nouveau_bdev(bdev);

	if (drm->agp.bridge) {
		ttm_agp_unbind(ttm);
		return;
	}
#endif
	nouveau_sgdma_unbind(bdev, ttm);
>>>>>>> upstream/android-13
}

static void
nouveau_bo_evict_flags(struct ttm_buffer_object *bo, struct ttm_placement *pl)
{
	struct nouveau_bo *nvbo = nouveau_bo(bo);

<<<<<<< HEAD
	switch (bo->mem.mem_type) {
	case TTM_PL_VRAM:
		nouveau_bo_placement_set(nvbo, TTM_PL_FLAG_TT,
					 TTM_PL_FLAG_SYSTEM);
		break;
	default:
		nouveau_bo_placement_set(nvbo, TTM_PL_FLAG_SYSTEM, 0);
=======
	switch (bo->resource->mem_type) {
	case TTM_PL_VRAM:
		nouveau_bo_placement_set(nvbo, NOUVEAU_GEM_DOMAIN_GART,
					 NOUVEAU_GEM_DOMAIN_CPU);
		break;
	default:
		nouveau_bo_placement_set(nvbo, NOUVEAU_GEM_DOMAIN_CPU, 0);
>>>>>>> upstream/android-13
		break;
	}

	*pl = nvbo->placement;
}

<<<<<<< HEAD

static int
nve0_bo_move_init(struct nouveau_channel *chan, u32 handle)
{
	int ret = RING_SPACE(chan, 2);
	if (ret == 0) {
		BEGIN_NVC0(chan, NvSubCopy, 0x0000, 1);
		OUT_RING  (chan, handle & 0x0000ffff);
		FIRE_RING (chan);
	}
	return ret;
}

static int
nve0_bo_move_copy(struct nouveau_channel *chan, struct ttm_buffer_object *bo,
		  struct ttm_mem_reg *old_reg, struct ttm_mem_reg *new_reg)
{
	struct nouveau_mem *mem = nouveau_mem(old_reg);
	int ret = RING_SPACE(chan, 10);
	if (ret == 0) {
		BEGIN_NVC0(chan, NvSubCopy, 0x0400, 8);
		OUT_RING  (chan, upper_32_bits(mem->vma[0].addr));
		OUT_RING  (chan, lower_32_bits(mem->vma[0].addr));
		OUT_RING  (chan, upper_32_bits(mem->vma[1].addr));
		OUT_RING  (chan, lower_32_bits(mem->vma[1].addr));
		OUT_RING  (chan, PAGE_SIZE);
		OUT_RING  (chan, PAGE_SIZE);
		OUT_RING  (chan, PAGE_SIZE);
		OUT_RING  (chan, new_reg->num_pages);
		BEGIN_IMC0(chan, NvSubCopy, 0x0300, 0x0386);
	}
	return ret;
}

static int
nvc0_bo_move_init(struct nouveau_channel *chan, u32 handle)
{
	int ret = RING_SPACE(chan, 2);
	if (ret == 0) {
		BEGIN_NVC0(chan, NvSubCopy, 0x0000, 1);
		OUT_RING  (chan, handle);
	}
	return ret;
}

static int
nvc0_bo_move_copy(struct nouveau_channel *chan, struct ttm_buffer_object *bo,
		  struct ttm_mem_reg *old_reg, struct ttm_mem_reg *new_reg)
{
	struct nouveau_mem *mem = nouveau_mem(old_reg);
	u64 src_offset = mem->vma[0].addr;
	u64 dst_offset = mem->vma[1].addr;
	u32 page_count = new_reg->num_pages;
	int ret;

	page_count = new_reg->num_pages;
	while (page_count) {
		int line_count = (page_count > 8191) ? 8191 : page_count;

		ret = RING_SPACE(chan, 11);
		if (ret)
			return ret;

		BEGIN_NVC0(chan, NvSubCopy, 0x030c, 8);
		OUT_RING  (chan, upper_32_bits(src_offset));
		OUT_RING  (chan, lower_32_bits(src_offset));
		OUT_RING  (chan, upper_32_bits(dst_offset));
		OUT_RING  (chan, lower_32_bits(dst_offset));
		OUT_RING  (chan, PAGE_SIZE);
		OUT_RING  (chan, PAGE_SIZE);
		OUT_RING  (chan, PAGE_SIZE);
		OUT_RING  (chan, line_count);
		BEGIN_NVC0(chan, NvSubCopy, 0x0300, 1);
		OUT_RING  (chan, 0x00000110);

		page_count -= line_count;
		src_offset += (PAGE_SIZE * line_count);
		dst_offset += (PAGE_SIZE * line_count);
	}

	return 0;
}

static int
nvc0_bo_move_m2mf(struct nouveau_channel *chan, struct ttm_buffer_object *bo,
		  struct ttm_mem_reg *old_reg, struct ttm_mem_reg *new_reg)
{
	struct nouveau_mem *mem = nouveau_mem(old_reg);
	u64 src_offset = mem->vma[0].addr;
	u64 dst_offset = mem->vma[1].addr;
	u32 page_count = new_reg->num_pages;
	int ret;

	page_count = new_reg->num_pages;
	while (page_count) {
		int line_count = (page_count > 2047) ? 2047 : page_count;

		ret = RING_SPACE(chan, 12);
		if (ret)
			return ret;

		BEGIN_NVC0(chan, NvSubCopy, 0x0238, 2);
		OUT_RING  (chan, upper_32_bits(dst_offset));
		OUT_RING  (chan, lower_32_bits(dst_offset));
		BEGIN_NVC0(chan, NvSubCopy, 0x030c, 6);
		OUT_RING  (chan, upper_32_bits(src_offset));
		OUT_RING  (chan, lower_32_bits(src_offset));
		OUT_RING  (chan, PAGE_SIZE); /* src_pitch */
		OUT_RING  (chan, PAGE_SIZE); /* dst_pitch */
		OUT_RING  (chan, PAGE_SIZE); /* line_length */
		OUT_RING  (chan, line_count);
		BEGIN_NVC0(chan, NvSubCopy, 0x0300, 1);
		OUT_RING  (chan, 0x00100110);

		page_count -= line_count;
		src_offset += (PAGE_SIZE * line_count);
		dst_offset += (PAGE_SIZE * line_count);
	}

	return 0;
}

static int
nva3_bo_move_copy(struct nouveau_channel *chan, struct ttm_buffer_object *bo,
		  struct ttm_mem_reg *old_reg, struct ttm_mem_reg *new_reg)
{
	struct nouveau_mem *mem = nouveau_mem(old_reg);
	u64 src_offset = mem->vma[0].addr;
	u64 dst_offset = mem->vma[1].addr;
	u32 page_count = new_reg->num_pages;
	int ret;

	page_count = new_reg->num_pages;
	while (page_count) {
		int line_count = (page_count > 8191) ? 8191 : page_count;

		ret = RING_SPACE(chan, 11);
		if (ret)
			return ret;

		BEGIN_NV04(chan, NvSubCopy, 0x030c, 8);
		OUT_RING  (chan, upper_32_bits(src_offset));
		OUT_RING  (chan, lower_32_bits(src_offset));
		OUT_RING  (chan, upper_32_bits(dst_offset));
		OUT_RING  (chan, lower_32_bits(dst_offset));
		OUT_RING  (chan, PAGE_SIZE);
		OUT_RING  (chan, PAGE_SIZE);
		OUT_RING  (chan, PAGE_SIZE);
		OUT_RING  (chan, line_count);
		BEGIN_NV04(chan, NvSubCopy, 0x0300, 1);
		OUT_RING  (chan, 0x00000110);

		page_count -= line_count;
		src_offset += (PAGE_SIZE * line_count);
		dst_offset += (PAGE_SIZE * line_count);
	}

	return 0;
}

static int
nv98_bo_move_exec(struct nouveau_channel *chan, struct ttm_buffer_object *bo,
		  struct ttm_mem_reg *old_reg, struct ttm_mem_reg *new_reg)
{
	struct nouveau_mem *mem = nouveau_mem(old_reg);
	int ret = RING_SPACE(chan, 7);
	if (ret == 0) {
		BEGIN_NV04(chan, NvSubCopy, 0x0320, 6);
		OUT_RING  (chan, upper_32_bits(mem->vma[0].addr));
		OUT_RING  (chan, lower_32_bits(mem->vma[0].addr));
		OUT_RING  (chan, upper_32_bits(mem->vma[1].addr));
		OUT_RING  (chan, lower_32_bits(mem->vma[1].addr));
		OUT_RING  (chan, 0x00000000 /* COPY */);
		OUT_RING  (chan, new_reg->num_pages << PAGE_SHIFT);
	}
	return ret;
}

static int
nv84_bo_move_exec(struct nouveau_channel *chan, struct ttm_buffer_object *bo,
		  struct ttm_mem_reg *old_reg, struct ttm_mem_reg *new_reg)
{
	struct nouveau_mem *mem = nouveau_mem(old_reg);
	int ret = RING_SPACE(chan, 7);
	if (ret == 0) {
		BEGIN_NV04(chan, NvSubCopy, 0x0304, 6);
		OUT_RING  (chan, new_reg->num_pages << PAGE_SHIFT);
		OUT_RING  (chan, upper_32_bits(mem->vma[0].addr));
		OUT_RING  (chan, lower_32_bits(mem->vma[0].addr));
		OUT_RING  (chan, upper_32_bits(mem->vma[1].addr));
		OUT_RING  (chan, lower_32_bits(mem->vma[1].addr));
		OUT_RING  (chan, 0x00000000 /* MODE_COPY, QUERY_NONE */);
	}
	return ret;
}

static int
nv50_bo_move_init(struct nouveau_channel *chan, u32 handle)
{
	int ret = RING_SPACE(chan, 6);
	if (ret == 0) {
		BEGIN_NV04(chan, NvSubCopy, 0x0000, 1);
		OUT_RING  (chan, handle);
		BEGIN_NV04(chan, NvSubCopy, 0x0180, 3);
		OUT_RING  (chan, chan->drm->ntfy.handle);
		OUT_RING  (chan, chan->vram.handle);
		OUT_RING  (chan, chan->vram.handle);
	}

	return ret;
}

static int
nv50_bo_move_m2mf(struct nouveau_channel *chan, struct ttm_buffer_object *bo,
		  struct ttm_mem_reg *old_reg, struct ttm_mem_reg *new_reg)
{
	struct nouveau_mem *mem = nouveau_mem(old_reg);
	u64 length = (new_reg->num_pages << PAGE_SHIFT);
	u64 src_offset = mem->vma[0].addr;
	u64 dst_offset = mem->vma[1].addr;
	int src_tiled = !!mem->kind;
	int dst_tiled = !!nouveau_mem(new_reg)->kind;
	int ret;

	while (length) {
		u32 amount, stride, height;

		ret = RING_SPACE(chan, 18 + 6 * (src_tiled + dst_tiled));
		if (ret)
			return ret;

		amount  = min(length, (u64)(4 * 1024 * 1024));
		stride  = 16 * 4;
		height  = amount / stride;

		if (src_tiled) {
			BEGIN_NV04(chan, NvSubCopy, 0x0200, 7);
			OUT_RING  (chan, 0);
			OUT_RING  (chan, 0);
			OUT_RING  (chan, stride);
			OUT_RING  (chan, height);
			OUT_RING  (chan, 1);
			OUT_RING  (chan, 0);
			OUT_RING  (chan, 0);
		} else {
			BEGIN_NV04(chan, NvSubCopy, 0x0200, 1);
			OUT_RING  (chan, 1);
		}
		if (dst_tiled) {
			BEGIN_NV04(chan, NvSubCopy, 0x021c, 7);
			OUT_RING  (chan, 0);
			OUT_RING  (chan, 0);
			OUT_RING  (chan, stride);
			OUT_RING  (chan, height);
			OUT_RING  (chan, 1);
			OUT_RING  (chan, 0);
			OUT_RING  (chan, 0);
		} else {
			BEGIN_NV04(chan, NvSubCopy, 0x021c, 1);
			OUT_RING  (chan, 1);
		}

		BEGIN_NV04(chan, NvSubCopy, 0x0238, 2);
		OUT_RING  (chan, upper_32_bits(src_offset));
		OUT_RING  (chan, upper_32_bits(dst_offset));
		BEGIN_NV04(chan, NvSubCopy, 0x030c, 8);
		OUT_RING  (chan, lower_32_bits(src_offset));
		OUT_RING  (chan, lower_32_bits(dst_offset));
		OUT_RING  (chan, stride);
		OUT_RING  (chan, stride);
		OUT_RING  (chan, stride);
		OUT_RING  (chan, height);
		OUT_RING  (chan, 0x00000101);
		OUT_RING  (chan, 0x00000000);
		BEGIN_NV04(chan, NvSubCopy, NV_MEMORY_TO_MEMORY_FORMAT_NOP, 1);
		OUT_RING  (chan, 0);

		length -= amount;
		src_offset += amount;
		dst_offset += amount;
	}

	return 0;
}

static int
nv04_bo_move_init(struct nouveau_channel *chan, u32 handle)
{
	int ret = RING_SPACE(chan, 4);
	if (ret == 0) {
		BEGIN_NV04(chan, NvSubCopy, 0x0000, 1);
		OUT_RING  (chan, handle);
		BEGIN_NV04(chan, NvSubCopy, 0x0180, 1);
		OUT_RING  (chan, chan->drm->ntfy.handle);
	}

	return ret;
}

static inline uint32_t
nouveau_bo_mem_ctxdma(struct ttm_buffer_object *bo,
		      struct nouveau_channel *chan, struct ttm_mem_reg *reg)
{
	if (reg->mem_type == TTM_PL_TT)
		return NvDmaTT;
	return chan->vram.handle;
}

static int
nv04_bo_move_m2mf(struct nouveau_channel *chan, struct ttm_buffer_object *bo,
		  struct ttm_mem_reg *old_reg, struct ttm_mem_reg *new_reg)
{
	u32 src_offset = old_reg->start << PAGE_SHIFT;
	u32 dst_offset = new_reg->start << PAGE_SHIFT;
	u32 page_count = new_reg->num_pages;
	int ret;

	ret = RING_SPACE(chan, 3);
	if (ret)
		return ret;

	BEGIN_NV04(chan, NvSubCopy, NV_MEMORY_TO_MEMORY_FORMAT_DMA_SOURCE, 2);
	OUT_RING  (chan, nouveau_bo_mem_ctxdma(bo, chan, old_reg));
	OUT_RING  (chan, nouveau_bo_mem_ctxdma(bo, chan, new_reg));

	page_count = new_reg->num_pages;
	while (page_count) {
		int line_count = (page_count > 2047) ? 2047 : page_count;

		ret = RING_SPACE(chan, 11);
		if (ret)
			return ret;

		BEGIN_NV04(chan, NvSubCopy,
				 NV_MEMORY_TO_MEMORY_FORMAT_OFFSET_IN, 8);
		OUT_RING  (chan, src_offset);
		OUT_RING  (chan, dst_offset);
		OUT_RING  (chan, PAGE_SIZE); /* src_pitch */
		OUT_RING  (chan, PAGE_SIZE); /* dst_pitch */
		OUT_RING  (chan, PAGE_SIZE); /* line_length */
		OUT_RING  (chan, line_count);
		OUT_RING  (chan, 0x00000101);
		OUT_RING  (chan, 0x00000000);
		BEGIN_NV04(chan, NvSubCopy, NV_MEMORY_TO_MEMORY_FORMAT_NOP, 1);
		OUT_RING  (chan, 0);

		page_count -= line_count;
		src_offset += (PAGE_SIZE * line_count);
		dst_offset += (PAGE_SIZE * line_count);
	}

	return 0;
}

static int
nouveau_bo_move_prep(struct nouveau_drm *drm, struct ttm_buffer_object *bo,
		     struct ttm_mem_reg *reg)
{
	struct nouveau_mem *old_mem = nouveau_mem(&bo->mem);
=======
static int
nouveau_bo_move_prep(struct nouveau_drm *drm, struct ttm_buffer_object *bo,
		     struct ttm_resource *reg)
{
	struct nouveau_mem *old_mem = nouveau_mem(bo->resource);
>>>>>>> upstream/android-13
	struct nouveau_mem *new_mem = nouveau_mem(reg);
	struct nvif_vmm *vmm = &drm->client.vmm.vmm;
	int ret;

	ret = nvif_vmm_get(vmm, LAZY, false, old_mem->mem.page, 0,
			   old_mem->mem.size, &old_mem->vma[0]);
	if (ret)
		return ret;

	ret = nvif_vmm_get(vmm, LAZY, false, new_mem->mem.page, 0,
			   new_mem->mem.size, &old_mem->vma[1]);
	if (ret)
		goto done;

	ret = nouveau_mem_map(old_mem, vmm, &old_mem->vma[0]);
	if (ret)
		goto done;

	ret = nouveau_mem_map(new_mem, vmm, &old_mem->vma[1]);
done:
	if (ret) {
		nvif_vmm_put(vmm, &old_mem->vma[1]);
		nvif_vmm_put(vmm, &old_mem->vma[0]);
	}
	return 0;
}

static int
<<<<<<< HEAD
nouveau_bo_move_m2mf(struct ttm_buffer_object *bo, int evict, bool intr,
		     bool no_wait_gpu, struct ttm_mem_reg *new_reg)
=======
nouveau_bo_move_m2mf(struct ttm_buffer_object *bo, int evict,
		     struct ttm_operation_ctx *ctx,
		     struct ttm_resource *new_reg)
>>>>>>> upstream/android-13
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct nouveau_channel *chan = drm->ttm.chan;
	struct nouveau_cli *cli = (void *)chan->user.client;
	struct nouveau_fence *fence;
	int ret;

	/* create temporary vmas for the transfer and attach them to the
	 * old nvkm_mem node, these will get cleaned up after ttm has
<<<<<<< HEAD
	 * destroyed the ttm_mem_reg
=======
	 * destroyed the ttm_resource
>>>>>>> upstream/android-13
	 */
	if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_TESLA) {
		ret = nouveau_bo_move_prep(drm, bo, new_reg);
		if (ret)
			return ret;
	}

<<<<<<< HEAD
	mutex_lock_nested(&cli->mutex, SINGLE_DEPTH_NESTING);
	ret = nouveau_fence_sync(nouveau_bo(bo), chan, true, intr);
	if (ret == 0) {
		ret = drm->ttm.move(chan, bo, &bo->mem, new_reg);
=======
	if (drm_drv_uses_atomic_modeset(drm->dev))
		mutex_lock(&cli->mutex);
	else
		mutex_lock_nested(&cli->mutex, SINGLE_DEPTH_NESTING);
	ret = nouveau_fence_sync(nouveau_bo(bo), chan, true, ctx->interruptible);
	if (ret == 0) {
		ret = drm->ttm.move(chan, bo, bo->resource, new_reg);
>>>>>>> upstream/android-13
		if (ret == 0) {
			ret = nouveau_fence_new(chan, false, &fence);
			if (ret == 0) {
				ret = ttm_bo_move_accel_cleanup(bo,
								&fence->base,
<<<<<<< HEAD
								evict,
=======
								evict, false,
>>>>>>> upstream/android-13
								new_reg);
				nouveau_fence_unref(&fence);
			}
		}
	}
	mutex_unlock(&cli->mutex);
	return ret;
}

void
nouveau_bo_move_init(struct nouveau_drm *drm)
{
<<<<<<< HEAD
	static const struct {
=======
	static const struct _method_table {
>>>>>>> upstream/android-13
		const char *name;
		int engine;
		s32 oclass;
		int (*exec)(struct nouveau_channel *,
			    struct ttm_buffer_object *,
<<<<<<< HEAD
			    struct ttm_mem_reg *, struct ttm_mem_reg *);
		int (*init)(struct nouveau_channel *, u32 handle);
	} _methods[] = {
=======
			    struct ttm_resource *, struct ttm_resource *);
		int (*init)(struct nouveau_channel *, u32 handle);
	} _methods[] = {
		{  "COPY", 4, 0xc7b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "COPY", 4, 0xc5b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xc5b5, nve0_bo_move_copy, nvc0_bo_move_init },
>>>>>>> upstream/android-13
		{  "COPY", 4, 0xc3b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xc3b5, nve0_bo_move_copy, nvc0_bo_move_init },
		{  "COPY", 4, 0xc1b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xc1b5, nve0_bo_move_copy, nvc0_bo_move_init },
		{  "COPY", 4, 0xc0b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xc0b5, nve0_bo_move_copy, nvc0_bo_move_init },
		{  "COPY", 4, 0xb0b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xb0b5, nve0_bo_move_copy, nvc0_bo_move_init },
		{  "COPY", 4, 0xa0b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xa0b5, nve0_bo_move_copy, nvc0_bo_move_init },
		{ "COPY1", 5, 0x90b8, nvc0_bo_move_copy, nvc0_bo_move_init },
		{ "COPY0", 4, 0x90b5, nvc0_bo_move_copy, nvc0_bo_move_init },
		{  "COPY", 0, 0x85b5, nva3_bo_move_copy, nv50_bo_move_init },
		{ "CRYPT", 0, 0x74c1, nv84_bo_move_exec, nv50_bo_move_init },
		{  "M2MF", 0, 0x9039, nvc0_bo_move_m2mf, nvc0_bo_move_init },
		{  "M2MF", 0, 0x5039, nv50_bo_move_m2mf, nv50_bo_move_init },
		{  "M2MF", 0, 0x0039, nv04_bo_move_m2mf, nv04_bo_move_init },
		{},
<<<<<<< HEAD
		{ "CRYPT", 0, 0x88b4, nv98_bo_move_exec, nv50_bo_move_init },
	}, *mthd = _methods;
=======
	};
	const struct _method_table *mthd = _methods;
>>>>>>> upstream/android-13
	const char *name = "CPU";
	int ret;

	do {
		struct nouveau_channel *chan;

		if (mthd->engine)
			chan = drm->cechan;
		else
			chan = drm->channel;
		if (chan == NULL)
			continue;

<<<<<<< HEAD
		ret = nvif_object_init(&chan->user,
=======
		ret = nvif_object_ctor(&chan->user, "ttmBoMove",
>>>>>>> upstream/android-13
				       mthd->oclass | (mthd->engine << 16),
				       mthd->oclass, NULL, 0,
				       &drm->ttm.copy);
		if (ret == 0) {
			ret = mthd->init(chan, drm->ttm.copy.handle);
			if (ret) {
<<<<<<< HEAD
				nvif_object_fini(&drm->ttm.copy);
=======
				nvif_object_dtor(&drm->ttm.copy);
>>>>>>> upstream/android-13
				continue;
			}

			drm->ttm.move = mthd->exec;
			drm->ttm.chan = chan;
			name = mthd->name;
			break;
		}
	} while ((++mthd)->exec);

	NV_INFO(drm, "MM: using %s for buffer copies\n", name);
}

<<<<<<< HEAD
static int
nouveau_bo_move_flipd(struct ttm_buffer_object *bo, bool evict, bool intr,
		      bool no_wait_gpu, struct ttm_mem_reg *new_reg)
{
	struct ttm_operation_ctx ctx = { intr, no_wait_gpu };
	struct ttm_place placement_memtype = {
		.fpfn = 0,
		.lpfn = 0,
		.flags = TTM_PL_FLAG_TT | TTM_PL_MASK_CACHING
	};
	struct ttm_placement placement;
	struct ttm_mem_reg tmp_reg;
	int ret;

	placement.num_placement = placement.num_busy_placement = 1;
	placement.placement = placement.busy_placement = &placement_memtype;

	tmp_reg = *new_reg;
	tmp_reg.mm_node = NULL;
	ret = ttm_bo_mem_space(bo, &placement, &tmp_reg, &ctx);
	if (ret)
		return ret;

	ret = ttm_tt_bind(bo->ttm, &tmp_reg, &ctx);
	if (ret)
		goto out;

	ret = nouveau_bo_move_m2mf(bo, true, intr, no_wait_gpu, &tmp_reg);
	if (ret)
		goto out;

	ret = ttm_bo_move_ttm(bo, &ctx, new_reg);
out:
	ttm_bo_mem_put(bo, &tmp_reg);
	return ret;
}

static int
nouveau_bo_move_flips(struct ttm_buffer_object *bo, bool evict, bool intr,
		      bool no_wait_gpu, struct ttm_mem_reg *new_reg)
{
	struct ttm_operation_ctx ctx = { intr, no_wait_gpu };
	struct ttm_place placement_memtype = {
		.fpfn = 0,
		.lpfn = 0,
		.flags = TTM_PL_FLAG_TT | TTM_PL_MASK_CACHING
	};
	struct ttm_placement placement;
	struct ttm_mem_reg tmp_reg;
	int ret;

	placement.num_placement = placement.num_busy_placement = 1;
	placement.placement = placement.busy_placement = &placement_memtype;

	tmp_reg = *new_reg;
	tmp_reg.mm_node = NULL;
	ret = ttm_bo_mem_space(bo, &placement, &tmp_reg, &ctx);
	if (ret)
		return ret;

	ret = ttm_bo_move_ttm(bo, &ctx, &tmp_reg);
	if (ret)
		goto out;

	ret = nouveau_bo_move_m2mf(bo, true, intr, no_wait_gpu, new_reg);
	if (ret)
		goto out;

out:
	ttm_bo_mem_put(bo, &tmp_reg);
	return ret;
}

static void
nouveau_bo_move_ntfy(struct ttm_buffer_object *bo, bool evict,
		     struct ttm_mem_reg *new_reg)
=======
static void nouveau_bo_move_ntfy(struct ttm_buffer_object *bo,
				 struct ttm_resource *new_reg)
>>>>>>> upstream/android-13
{
	struct nouveau_mem *mem = new_reg ? nouveau_mem(new_reg) : NULL;
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	struct nouveau_vma *vma;

	/* ttm can now (stupidly) pass the driver bos it didn't create... */
	if (bo->destroy != nouveau_bo_del_ttm)
		return;

<<<<<<< HEAD
=======
	nouveau_bo_del_io_reserve_lru(bo);

>>>>>>> upstream/android-13
	if (mem && new_reg->mem_type != TTM_PL_SYSTEM &&
	    mem->mem.page == nvbo->page) {
		list_for_each_entry(vma, &nvbo->vma_list, head) {
			nouveau_vma_map(vma, mem);
		}
	} else {
		list_for_each_entry(vma, &nvbo->vma_list, head) {
			WARN_ON(ttm_bo_wait(bo, false, false));
			nouveau_vma_unmap(vma);
		}
	}
<<<<<<< HEAD
}

static int
nouveau_bo_vm_bind(struct ttm_buffer_object *bo, struct ttm_mem_reg *new_reg,
=======

	if (new_reg)
		nvbo->offset = (new_reg->start << PAGE_SHIFT);

}

static int
nouveau_bo_vm_bind(struct ttm_buffer_object *bo, struct ttm_resource *new_reg,
>>>>>>> upstream/android-13
		   struct nouveau_drm_tile **new_tile)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct drm_device *dev = drm->dev;
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	u64 offset = new_reg->start << PAGE_SHIFT;

	*new_tile = NULL;
	if (new_reg->mem_type != TTM_PL_VRAM)
		return 0;

	if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_CELSIUS) {
<<<<<<< HEAD
		*new_tile = nv10_bo_set_tiling(dev, offset, new_reg->size,
=======
		*new_tile = nv10_bo_set_tiling(dev, offset, bo->base.size,
>>>>>>> upstream/android-13
					       nvbo->mode, nvbo->zeta);
	}

	return 0;
}

static void
nouveau_bo_vm_cleanup(struct ttm_buffer_object *bo,
		      struct nouveau_drm_tile *new_tile,
		      struct nouveau_drm_tile **old_tile)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct drm_device *dev = drm->dev;
<<<<<<< HEAD
	struct dma_fence *fence = reservation_object_get_excl(bo->resv);
=======
	struct dma_fence *fence = dma_resv_excl_fence(bo->base.resv);
>>>>>>> upstream/android-13

	nv10_bo_put_tile_region(dev, *old_tile, fence);
	*old_tile = new_tile;
}

static int
nouveau_bo_move(struct ttm_buffer_object *bo, bool evict,
		struct ttm_operation_ctx *ctx,
<<<<<<< HEAD
		struct ttm_mem_reg *new_reg)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	struct ttm_mem_reg *old_reg = &bo->mem;
	struct nouveau_drm_tile *new_tile = NULL;
	int ret = 0;

	ret = ttm_bo_wait(bo, ctx->interruptible, ctx->no_wait_gpu);
	if (ret)
		return ret;

	if (nvbo->pin_refcnt)
=======
		struct ttm_resource *new_reg,
		struct ttm_place *hop)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	struct ttm_resource *old_reg = bo->resource;
	struct nouveau_drm_tile *new_tile = NULL;
	int ret = 0;


	if (new_reg->mem_type == TTM_PL_TT) {
		ret = nouveau_ttm_tt_bind(bo->bdev, bo->ttm, new_reg);
		if (ret)
			return ret;
	}

	nouveau_bo_move_ntfy(bo, new_reg);
	ret = ttm_bo_wait_ctx(bo, ctx);
	if (ret)
		goto out_ntfy;

	if (nvbo->bo.pin_count)
>>>>>>> upstream/android-13
		NV_WARN(drm, "Moving pinned object %p!\n", nvbo);

	if (drm->client.device.info.family < NV_DEVICE_INFO_V0_TESLA) {
		ret = nouveau_bo_vm_bind(bo, new_reg, &new_tile);
		if (ret)
<<<<<<< HEAD
			return ret;
=======
			goto out_ntfy;
>>>>>>> upstream/android-13
	}

	/* Fake bo copy. */
	if (old_reg->mem_type == TTM_PL_SYSTEM && !bo->ttm) {
<<<<<<< HEAD
		BUG_ON(bo->mem.mm_node != NULL);
		bo->mem = *new_reg;
		new_reg->mm_node = NULL;
=======
		ttm_bo_move_null(bo, new_reg);
		goto out;
	}

	if (old_reg->mem_type == TTM_PL_SYSTEM &&
	    new_reg->mem_type == TTM_PL_TT) {
		ttm_bo_move_null(bo, new_reg);
		goto out;
	}

	if (old_reg->mem_type == TTM_PL_TT &&
	    new_reg->mem_type == TTM_PL_SYSTEM) {
		nouveau_ttm_tt_unbind(bo->bdev, bo->ttm);
		ttm_resource_free(bo, &bo->resource);
		ttm_bo_assign_mem(bo, new_reg);
>>>>>>> upstream/android-13
		goto out;
	}

	/* Hardware assisted copy. */
	if (drm->ttm.move) {
<<<<<<< HEAD
		if (new_reg->mem_type == TTM_PL_SYSTEM)
			ret = nouveau_bo_move_flipd(bo, evict,
						    ctx->interruptible,
						    ctx->no_wait_gpu, new_reg);
		else if (old_reg->mem_type == TTM_PL_SYSTEM)
			ret = nouveau_bo_move_flips(bo, evict,
						    ctx->interruptible,
						    ctx->no_wait_gpu, new_reg);
		else
			ret = nouveau_bo_move_m2mf(bo, evict,
						   ctx->interruptible,
						   ctx->no_wait_gpu, new_reg);
		if (!ret)
			goto out;
	}

	/* Fallback to software copy. */
	ret = ttm_bo_wait(bo, ctx->interruptible, ctx->no_wait_gpu);
	if (ret == 0)
		ret = ttm_bo_move_memcpy(bo, ctx, new_reg);
=======
		if ((old_reg->mem_type == TTM_PL_SYSTEM &&
		     new_reg->mem_type == TTM_PL_VRAM) ||
		    (old_reg->mem_type == TTM_PL_VRAM &&
		     new_reg->mem_type == TTM_PL_SYSTEM)) {
			hop->fpfn = 0;
			hop->lpfn = 0;
			hop->mem_type = TTM_PL_TT;
			hop->flags = 0;
			return -EMULTIHOP;
		}
		ret = nouveau_bo_move_m2mf(bo, evict, ctx,
					   new_reg);
	} else
		ret = -ENODEV;

	if (ret) {
		/* Fallback to software copy. */
		ret = ttm_bo_move_memcpy(bo, ctx, new_reg);
	}
>>>>>>> upstream/android-13

out:
	if (drm->client.device.info.family < NV_DEVICE_INFO_V0_TESLA) {
		if (ret)
			nouveau_bo_vm_cleanup(bo, NULL, &new_tile);
		else
			nouveau_bo_vm_cleanup(bo, new_tile, &nvbo->tile);
	}
<<<<<<< HEAD

	return ret;
}

static int
nouveau_bo_verify_access(struct ttm_buffer_object *bo, struct file *filp)
{
	struct nouveau_bo *nvbo = nouveau_bo(bo);

	return drm_vma_node_verify_access(&nvbo->gem.vma_node,
					  filp->private_data);
}

static int
nouveau_ttm_io_mem_reserve(struct ttm_bo_device *bdev, struct ttm_mem_reg *reg)
{
	struct ttm_mem_type_manager *man = &bdev->man[reg->mem_type];
	struct nouveau_drm *drm = nouveau_bdev(bdev);
	struct nvkm_device *device = nvxx_device(&drm->client.device);
	struct nouveau_mem *mem = nouveau_mem(reg);

	reg->bus.addr = NULL;
	reg->bus.offset = 0;
	reg->bus.size = reg->num_pages << PAGE_SHIFT;
	reg->bus.base = 0;
	reg->bus.is_iomem = false;
	if (!(man->flags & TTM_MEMTYPE_FLAG_MAPPABLE))
		return -EINVAL;
	switch (reg->mem_type) {
	case TTM_PL_SYSTEM:
		/* System memory */
		return 0;
	case TTM_PL_TT:
#if IS_ENABLED(CONFIG_AGP)
		if (drm->agp.bridge) {
			reg->bus.offset = reg->start << PAGE_SHIFT;
			reg->bus.base = drm->agp.base;
			reg->bus.is_iomem = !drm->agp.cma;
		}
#endif
		if (drm->client.mem->oclass < NVIF_CLASS_MEM_NV50 || !mem->kind)
			/* untiled */
			break;
		/* fallthrough, tiled memory */
	case TTM_PL_VRAM:
		reg->bus.offset = reg->start << PAGE_SHIFT;
		reg->bus.base = device->func->resource_addr(device, 1);
		reg->bus.is_iomem = true;
=======
out_ntfy:
	if (ret) {
		nouveau_bo_move_ntfy(bo, bo->resource);
	}
	return ret;
}

static void
nouveau_ttm_io_mem_free_locked(struct nouveau_drm *drm,
			       struct ttm_resource *reg)
{
	struct nouveau_mem *mem = nouveau_mem(reg);

	if (drm->client.mem->oclass >= NVIF_CLASS_MEM_NV50) {
		switch (reg->mem_type) {
		case TTM_PL_TT:
			if (mem->kind)
				nvif_object_unmap_handle(&mem->mem.object);
			break;
		case TTM_PL_VRAM:
			nvif_object_unmap_handle(&mem->mem.object);
			break;
		default:
			break;
		}
	}
}

static int
nouveau_ttm_io_mem_reserve(struct ttm_device *bdev, struct ttm_resource *reg)
{
	struct nouveau_drm *drm = nouveau_bdev(bdev);
	struct nvkm_device *device = nvxx_device(&drm->client.device);
	struct nouveau_mem *mem = nouveau_mem(reg);
	struct nvif_mmu *mmu = &drm->client.mmu;
	int ret;

	mutex_lock(&drm->ttm.io_reserve_mutex);
retry:
	switch (reg->mem_type) {
	case TTM_PL_SYSTEM:
		/* System memory */
		ret = 0;
		goto out;
	case TTM_PL_TT:
#if IS_ENABLED(CONFIG_AGP)
		if (drm->agp.bridge) {
			reg->bus.offset = (reg->start << PAGE_SHIFT) +
				drm->agp.base;
			reg->bus.is_iomem = !drm->agp.cma;
			reg->bus.caching = ttm_write_combined;
		}
#endif
		if (drm->client.mem->oclass < NVIF_CLASS_MEM_NV50 ||
		    !mem->kind) {
			/* untiled */
			ret = 0;
			break;
		}
		fallthrough;	/* tiled memory */
	case TTM_PL_VRAM:
		reg->bus.offset = (reg->start << PAGE_SHIFT) +
			device->func->resource_addr(device, 1);
		reg->bus.is_iomem = true;

		/* Some BARs do not support being ioremapped WC */
		if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_TESLA &&
		    mmu->type[drm->ttm.type_vram].type & NVIF_MEM_UNCACHED)
			reg->bus.caching = ttm_uncached;
		else
			reg->bus.caching = ttm_write_combined;

>>>>>>> upstream/android-13
		if (drm->client.mem->oclass >= NVIF_CLASS_MEM_NV50) {
			union {
				struct nv50_mem_map_v0 nv50;
				struct gf100_mem_map_v0 gf100;
			} args;
			u64 handle, length;
			u32 argc = 0;
<<<<<<< HEAD
			int ret;
=======
>>>>>>> upstream/android-13

			switch (mem->mem.object.oclass) {
			case NVIF_CLASS_MEM_NV50:
				args.nv50.version = 0;
				args.nv50.ro = 0;
				args.nv50.kind = mem->kind;
				args.nv50.comp = mem->comp;
				argc = sizeof(args.nv50);
				break;
			case NVIF_CLASS_MEM_GF100:
				args.gf100.version = 0;
				args.gf100.ro = 0;
				args.gf100.kind = mem->kind;
				argc = sizeof(args.gf100);
				break;
			default:
				WARN_ON(1);
				break;
			}

			ret = nvif_object_map_handle(&mem->mem.object,
						     &args, argc,
						     &handle, &length);
<<<<<<< HEAD
			if (ret != 1)
				return ret ? ret : -EINVAL;

			reg->bus.base = 0;
			reg->bus.offset = handle;
		}
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static void
nouveau_ttm_io_mem_free(struct ttm_bo_device *bdev, struct ttm_mem_reg *reg)
{
	struct nouveau_drm *drm = nouveau_bdev(bdev);
	struct nouveau_mem *mem = nouveau_mem(reg);

	if (drm->client.mem->oclass >= NVIF_CLASS_MEM_NV50) {
		switch (reg->mem_type) {
		case TTM_PL_TT:
			if (mem->kind)
				nvif_object_unmap_handle(&mem->mem.object);
			break;
		case TTM_PL_VRAM:
			nvif_object_unmap_handle(&mem->mem.object);
			break;
		default:
			break;
		}
	}
}

static int
nouveau_ttm_fault_reserve_notify(struct ttm_buffer_object *bo)
=======
			if (ret != 1) {
				if (WARN_ON(ret == 0))
					ret = -EINVAL;
				goto out;
			}

			reg->bus.offset = handle;
		}
		ret = 0;
		break;
	default:
		ret = -EINVAL;
	}

out:
	if (ret == -ENOSPC) {
		struct nouveau_bo *nvbo;

		nvbo = list_first_entry_or_null(&drm->ttm.io_reserve_lru,
						typeof(*nvbo),
						io_reserve_lru);
		if (nvbo) {
			list_del_init(&nvbo->io_reserve_lru);
			drm_vma_node_unmap(&nvbo->bo.base.vma_node,
					   bdev->dev_mapping);
			nouveau_ttm_io_mem_free_locked(drm, nvbo->bo.resource);
			goto retry;
		}

	}
	mutex_unlock(&drm->ttm.io_reserve_mutex);
	return ret;
}

static void
nouveau_ttm_io_mem_free(struct ttm_device *bdev, struct ttm_resource *reg)
{
	struct nouveau_drm *drm = nouveau_bdev(bdev);

	mutex_lock(&drm->ttm.io_reserve_mutex);
	nouveau_ttm_io_mem_free_locked(drm, reg);
	mutex_unlock(&drm->ttm.io_reserve_mutex);
}

vm_fault_t nouveau_ttm_fault_reserve_notify(struct ttm_buffer_object *bo)
>>>>>>> upstream/android-13
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	struct nvkm_device *device = nvxx_device(&drm->client.device);
	u32 mappable = device->func->resource_size(device, 1) >> PAGE_SHIFT;
	int i, ret;

	/* as long as the bo isn't in vram, and isn't tiled, we've got
	 * nothing to do here.
	 */
<<<<<<< HEAD
	if (bo->mem.mem_type != TTM_PL_VRAM) {
=======
	if (bo->resource->mem_type != TTM_PL_VRAM) {
>>>>>>> upstream/android-13
		if (drm->client.device.info.family < NV_DEVICE_INFO_V0_TESLA ||
		    !nvbo->kind)
			return 0;

<<<<<<< HEAD
		if (bo->mem.mem_type == TTM_PL_SYSTEM) {
			nouveau_bo_placement_set(nvbo, TTM_PL_TT, 0);

			ret = nouveau_bo_validate(nvbo, false, false);
			if (ret)
				return ret;
		}
		return 0;
	}

	/* make sure bo is in mappable vram */
	if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_TESLA ||
	    bo->mem.start + bo->mem.num_pages < mappable)
		return 0;

	for (i = 0; i < nvbo->placement.num_placement; ++i) {
		nvbo->placements[i].fpfn = 0;
		nvbo->placements[i].lpfn = mappable;
	}

	for (i = 0; i < nvbo->placement.num_busy_placement; ++i) {
		nvbo->busy_placements[i].fpfn = 0;
		nvbo->busy_placements[i].lpfn = mappable;
	}

	nouveau_bo_placement_set(nvbo, TTM_PL_FLAG_VRAM, 0);
	return nouveau_bo_validate(nvbo, false, false);
}

static int
nouveau_ttm_tt_populate(struct ttm_tt *ttm, struct ttm_operation_ctx *ctx)
{
	struct ttm_dma_tt *ttm_dma = (void *)ttm;
	struct nouveau_drm *drm;
	struct device *dev;
	unsigned i;
	int r;
	bool slave = !!(ttm->page_flags & TTM_PAGE_FLAG_SG);

	if (ttm->state != tt_unpopulated)
		return 0;

	if (slave && ttm->sg) {
		/* make userspace faulting work */
		drm_prime_sg_to_page_addr_arrays(ttm->sg, ttm->pages,
						 ttm_dma->dma_address, ttm->num_pages);
		ttm->state = tt_unbound;
		return 0;
	}

	drm = nouveau_bdev(ttm->bdev);
	dev = drm->dev->dev;

#if IS_ENABLED(CONFIG_AGP)
	if (drm->agp.bridge) {
		return ttm_agp_tt_populate(ttm, ctx);
	}
#endif

#if IS_ENABLED(CONFIG_SWIOTLB) && IS_ENABLED(CONFIG_X86)
	if (swiotlb_nr_tbl()) {
		return ttm_dma_populate((void *)ttm, dev, ctx);
	}
#endif

	r = ttm_pool_populate(ttm, ctx);
	if (r) {
		return r;
	}

	for (i = 0; i < ttm->num_pages; i++) {
		dma_addr_t addr;

		addr = dma_map_page(dev, ttm->pages[i], 0, PAGE_SIZE,
				    DMA_BIDIRECTIONAL);

		if (dma_mapping_error(dev, addr)) {
			while (i--) {
				dma_unmap_page(dev, ttm_dma->dma_address[i],
					       PAGE_SIZE, DMA_BIDIRECTIONAL);
				ttm_dma->dma_address[i] = 0;
			}
			ttm_pool_unpopulate(ttm);
			return -EFAULT;
		}

		ttm_dma->dma_address[i] = addr;
	}
	return 0;
}

static void
nouveau_ttm_tt_unpopulate(struct ttm_tt *ttm)
{
	struct ttm_dma_tt *ttm_dma = (void *)ttm;
	struct nouveau_drm *drm;
	struct device *dev;
	unsigned i;
=======
		if (bo->resource->mem_type != TTM_PL_SYSTEM)
			return 0;

		nouveau_bo_placement_set(nvbo, NOUVEAU_GEM_DOMAIN_GART, 0);

	} else {
		/* make sure bo is in mappable vram */
		if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_TESLA ||
		    bo->resource->start + bo->resource->num_pages < mappable)
			return 0;

		for (i = 0; i < nvbo->placement.num_placement; ++i) {
			nvbo->placements[i].fpfn = 0;
			nvbo->placements[i].lpfn = mappable;
		}

		for (i = 0; i < nvbo->placement.num_busy_placement; ++i) {
			nvbo->busy_placements[i].fpfn = 0;
			nvbo->busy_placements[i].lpfn = mappable;
		}

		nouveau_bo_placement_set(nvbo, NOUVEAU_GEM_DOMAIN_VRAM, 0);
	}

	ret = nouveau_bo_validate(nvbo, false, false);
	if (unlikely(ret == -EBUSY || ret == -ERESTARTSYS))
		return VM_FAULT_NOPAGE;
	else if (unlikely(ret))
		return VM_FAULT_SIGBUS;

	ttm_bo_move_to_lru_tail_unlocked(bo);
	return 0;
}

static int
nouveau_ttm_tt_populate(struct ttm_device *bdev,
			struct ttm_tt *ttm, struct ttm_operation_ctx *ctx)
{
	struct ttm_tt *ttm_dma = (void *)ttm;
	struct nouveau_drm *drm;
	struct device *dev;
	bool slave = !!(ttm->page_flags & TTM_PAGE_FLAG_SG);

	if (ttm_tt_is_populated(ttm))
		return 0;

	if (slave && ttm->sg) {
		drm_prime_sg_to_dma_addr_array(ttm->sg, ttm_dma->dma_address,
					       ttm->num_pages);
		return 0;
	}

	drm = nouveau_bdev(bdev);
	dev = drm->dev->dev;

	return ttm_pool_alloc(&drm->ttm.bdev.pool, ttm, ctx);
}

static void
nouveau_ttm_tt_unpopulate(struct ttm_device *bdev,
			  struct ttm_tt *ttm)
{
	struct nouveau_drm *drm;
	struct device *dev;
>>>>>>> upstream/android-13
	bool slave = !!(ttm->page_flags & TTM_PAGE_FLAG_SG);

	if (slave)
		return;

<<<<<<< HEAD
	drm = nouveau_bdev(ttm->bdev);
	dev = drm->dev->dev;

#if IS_ENABLED(CONFIG_AGP)
	if (drm->agp.bridge) {
		ttm_agp_tt_unpopulate(ttm);
		return;
	}
#endif

#if IS_ENABLED(CONFIG_SWIOTLB) && IS_ENABLED(CONFIG_X86)
	if (swiotlb_nr_tbl()) {
		ttm_dma_unpopulate((void *)ttm, dev);
		return;
	}
#endif

	for (i = 0; i < ttm->num_pages; i++) {
		if (ttm_dma->dma_address[i]) {
			dma_unmap_page(dev, ttm_dma->dma_address[i], PAGE_SIZE,
				       DMA_BIDIRECTIONAL);
		}
	}

	ttm_pool_unpopulate(ttm);
=======
	drm = nouveau_bdev(bdev);
	dev = drm->dev->dev;

	return ttm_pool_free(&drm->ttm.bdev.pool, ttm);
}

static void
nouveau_ttm_tt_destroy(struct ttm_device *bdev,
		       struct ttm_tt *ttm)
{
#if IS_ENABLED(CONFIG_AGP)
	struct nouveau_drm *drm = nouveau_bdev(bdev);
	if (drm->agp.bridge) {
		ttm_agp_unbind(ttm);
		ttm_tt_destroy_common(bdev, ttm);
		ttm_agp_destroy(ttm);
		return;
	}
#endif
	nouveau_sgdma_destroy(bdev, ttm);
>>>>>>> upstream/android-13
}

void
nouveau_bo_fence(struct nouveau_bo *nvbo, struct nouveau_fence *fence, bool exclusive)
{
<<<<<<< HEAD
	struct reservation_object *resv = nvbo->bo.resv;

	if (exclusive)
		reservation_object_add_excl_fence(resv, &fence->base);
	else if (fence)
		reservation_object_add_shared_fence(resv, &fence->base);
}

struct ttm_bo_driver nouveau_bo_driver = {
	.ttm_tt_create = &nouveau_ttm_tt_create,
	.ttm_tt_populate = &nouveau_ttm_tt_populate,
	.ttm_tt_unpopulate = &nouveau_ttm_tt_unpopulate,
	.invalidate_caches = nouveau_bo_invalidate_caches,
	.init_mem_type = nouveau_bo_init_mem_type,
	.eviction_valuable = ttm_bo_eviction_valuable,
	.evict_flags = nouveau_bo_evict_flags,
	.move_notify = nouveau_bo_move_ntfy,
	.move = nouveau_bo_move,
	.verify_access = nouveau_bo_verify_access,
	.fault_reserve_notify = &nouveau_ttm_fault_reserve_notify,
=======
	struct dma_resv *resv = nvbo->bo.base.resv;

	if (exclusive)
		dma_resv_add_excl_fence(resv, &fence->base);
	else if (fence)
		dma_resv_add_shared_fence(resv, &fence->base);
}

static void
nouveau_bo_delete_mem_notify(struct ttm_buffer_object *bo)
{
	nouveau_bo_move_ntfy(bo, NULL);
}

struct ttm_device_funcs nouveau_bo_driver = {
	.ttm_tt_create = &nouveau_ttm_tt_create,
	.ttm_tt_populate = &nouveau_ttm_tt_populate,
	.ttm_tt_unpopulate = &nouveau_ttm_tt_unpopulate,
	.ttm_tt_destroy = &nouveau_ttm_tt_destroy,
	.eviction_valuable = ttm_bo_eviction_valuable,
	.evict_flags = nouveau_bo_evict_flags,
	.delete_mem_notify = nouveau_bo_delete_mem_notify,
	.move = nouveau_bo_move,
>>>>>>> upstream/android-13
	.io_mem_reserve = &nouveau_ttm_io_mem_reserve,
	.io_mem_free = &nouveau_ttm_io_mem_free,
};
