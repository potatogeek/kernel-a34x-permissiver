<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
*/
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2015 MediaTek Inc.
 */
>>>>>>> upstream/android-13

#ifndef _MTK_DRM_GEM_H_
#define _MTK_DRM_GEM_H_

#include <drm/drm_gem.h>
<<<<<<< HEAD
#if defined(CONFIG_MTK_IOMMU_V2)
#include "ion_drv.h"
#include "ion_priv.h"
#include <soc/mediatek/smi.h>
#include "mtk_iommu_ext.h"
#include "pseudo_m4u.h"
#endif
=======
>>>>>>> upstream/android-13

/*
 * mtk drm buffer structure.
 *
 * @base: a gem object.
 *	- a new handle to this gem object would be created
 *	by drm_gem_handle_create().
 * @cookie: the return value of dma_alloc_attrs(), keep it for dma_free_attrs()
 * @kvaddr: kernel virtual address of gem buffer.
 * @dma_addr: dma address of gem buffer.
 * @dma_attrs: dma attributes of gem buffer.
 *
 * P.S. this object would be transferred to user as kms_bo.handle so
 *	user can access the buffer through kms_bo.handle.
 */
struct mtk_drm_gem_obj {
<<<<<<< HEAD
	struct drm_gem_object base;
	void *cookie;
	void *kvaddr;
	dma_addr_t dma_addr;
	size_t size;
	unsigned long dma_attrs;
	struct sg_table *sg;
#if defined(CONFIG_MTK_IOMMU_V2)
	struct ion_handle *handle;
#endif
	bool sec;
	bool is_dumb;
	int sec_id;
};

#define to_mtk_gem_obj(x) container_of(x, struct mtk_drm_gem_obj, base)
=======
	struct drm_gem_object	base;
	void			*cookie;
	void			*kvaddr;
	dma_addr_t		dma_addr;
	unsigned long		dma_attrs;
	struct sg_table		*sg;
	struct page		**pages;
};

#define to_mtk_gem_obj(x)	container_of(x, struct mtk_drm_gem_obj, base)
>>>>>>> upstream/android-13

void mtk_drm_gem_free_object(struct drm_gem_object *gem);
struct mtk_drm_gem_obj *mtk_drm_gem_create(struct drm_device *dev, size_t size,
					   bool alloc_kmap);
int mtk_drm_gem_dumb_create(struct drm_file *file_priv, struct drm_device *dev,
			    struct drm_mode_create_dumb *args);
<<<<<<< HEAD
int mtk_drm_gem_dumb_map_offset(struct drm_file *file_priv,
				struct drm_device *dev, uint32_t handle,
				uint64_t *offset);
int mtk_drm_gem_mmap(struct file *filp, struct vm_area_struct *vma);
int mtk_drm_gem_mmap_buf(struct drm_gem_object *obj,
			 struct vm_area_struct *vma);
#if defined(CONFIG_MTK_IOMMU_V2)
struct ion_client *mtk_drm_gem_ion_create_client(const char *name);
void mtk_drm_gem_ion_destroy_client(struct ion_client *client);
void mtk_drm_gem_ion_free_handle(struct ion_client *client,
	struct ion_handle *handle, const char *name, int line);
struct ion_handle *mtk_drm_gem_ion_import_handle(struct ion_client *client,
	int fd);
#endif
struct drm_gem_object *
mtk_gem_prime_import(struct drm_device *dev, struct dma_buf *dma_buf);
struct sg_table *mtk_gem_prime_get_sg_table(struct drm_gem_object *obj);
struct drm_gem_object *
mtk_gem_prime_import_sg_table(struct drm_device *dev,
			      struct dma_buf_attachment *attach,
			      struct sg_table *sg);

/*
 * request gem object creation and buffer allocation as the size
 * that it is calculated with framebuffer information such as width,
 * height and bpp.
 */
int mtk_gem_create_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file_priv);

/* get buffer offset to map to user space. */
int mtk_gem_map_offset_ioctl(struct drm_device *dev, void *data,
			     struct drm_file *file_priv);

/* submit buffer and return fence */
int mtk_gem_submit_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file_priv);
int mtk_drm_sec_hnd_to_gem_hnd(struct drm_device *dev, void *data,
		struct drm_file *file_priv);

struct mtk_drm_gem_obj *mtk_drm_fb_gem_insert(struct drm_device *dev,
					      size_t size, phys_addr_t fb_base,
					      unsigned int vramsize);
void mtk_drm_fb_gem_release(struct drm_device *dev);
=======
struct sg_table *mtk_gem_prime_get_sg_table(struct drm_gem_object *obj);
struct drm_gem_object *mtk_gem_prime_import_sg_table(struct drm_device *dev,
			struct dma_buf_attachment *attach, struct sg_table *sg);
int mtk_drm_gem_prime_vmap(struct drm_gem_object *obj, struct dma_buf_map *map);
void mtk_drm_gem_prime_vunmap(struct drm_gem_object *obj, struct dma_buf_map *map);

>>>>>>> upstream/android-13
#endif
