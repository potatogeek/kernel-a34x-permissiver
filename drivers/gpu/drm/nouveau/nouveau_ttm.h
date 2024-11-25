<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
=======
/* SPDX-License-Identifier: MIT */
>>>>>>> upstream/android-13
#ifndef __NOUVEAU_TTM_H__
#define __NOUVEAU_TTM_H__

static inline struct nouveau_drm *
<<<<<<< HEAD
nouveau_bdev(struct ttm_bo_device *bd)
=======
nouveau_bdev(struct ttm_device *bd)
>>>>>>> upstream/android-13
{
	return container_of(bd, struct nouveau_drm, ttm.bdev);
}

<<<<<<< HEAD
extern const struct ttm_mem_type_manager_func nouveau_vram_manager;
extern const struct ttm_mem_type_manager_func nouveau_gart_manager;
extern const struct ttm_mem_type_manager_func nv04_gart_manager;
=======
extern const struct ttm_resource_manager_func nouveau_vram_manager;
extern const struct ttm_resource_manager_func nouveau_gart_manager;
extern const struct ttm_resource_manager_func nv04_gart_manager;
>>>>>>> upstream/android-13

struct ttm_tt *nouveau_sgdma_create_ttm(struct ttm_buffer_object *bo,
					u32 page_flags);

int  nouveau_ttm_init(struct nouveau_drm *drm);
void nouveau_ttm_fini(struct nouveau_drm *drm);
<<<<<<< HEAD
int  nouveau_ttm_mmap(struct file *, struct vm_area_struct *);
=======
>>>>>>> upstream/android-13

int  nouveau_ttm_global_init(struct nouveau_drm *);
void nouveau_ttm_global_release(struct nouveau_drm *);

<<<<<<< HEAD
=======
int nouveau_sgdma_bind(struct ttm_device *bdev, struct ttm_tt *ttm, struct ttm_resource *reg);
void nouveau_sgdma_unbind(struct ttm_device *bdev, struct ttm_tt *ttm);
void nouveau_sgdma_destroy(struct ttm_device *bdev, struct ttm_tt *ttm);
>>>>>>> upstream/android-13
#endif
