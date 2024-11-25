

#ifndef __DRM_PRIME_H__
#define __DRM_PRIME_H__

#include <linux/mutex.h>
#include <linux/rbtree.h>
#include <linux/scatterlist.h>



struct drm_prime_file_private {

	struct mutex lock;
	struct rb_root dmabufs;
	struct rb_root handles;
};

struct device;

struct dma_buf_export_info;
struct dma_buf;
struct dma_buf_attachment;

enum dma_data_direction;

struct drm_device;
struct drm_gem_object;
struct drm_file;

struct device;

struct dma_buf *drm_gem_prime_export(struct drm_device *dev,
				     struct drm_gem_object *obj,
				     int flags);
int drm_gem_prime_handle_to_fd(struct drm_device *dev,
			       struct drm_file *file_priv, uint32_t handle, uint32_t flags,
			       int *prime_fd);
int drm_gem_prime_mmap(struct drm_gem_object *obj, struct vm_area_struct *vma);
struct drm_gem_object *drm_gem_prime_import(struct drm_device *dev,
					    struct dma_buf *dma_buf);

struct drm_gem_object *drm_gem_prime_import_dev(struct drm_device *dev,
						struct dma_buf *dma_buf,
						struct device *attach_dev);

int drm_gem_prime_fd_to_handle(struct drm_device *dev,
			       struct drm_file *file_priv, int prime_fd, uint32_t *handle);
struct dma_buf *drm_gem_dmabuf_export(struct drm_device *dev,
				      struct dma_buf_export_info *exp_info);
void drm_gem_dmabuf_release(struct dma_buf *dma_buf);
int drm_gem_map_attach(struct dma_buf *dma_buf,
		       struct dma_buf_attachment *attach);
void drm_gem_map_detach(struct dma_buf *dma_buf,
			struct dma_buf_attachment *attach);
struct sg_table *drm_gem_map_dma_buf(struct dma_buf_attachment *attach,
				     enum dma_data_direction dir);
void drm_gem_unmap_dma_buf(struct dma_buf_attachment *attach,
			   struct sg_table *sgt,
			   enum dma_data_direction dir);
void *drm_gem_dmabuf_vmap(struct dma_buf *dma_buf);
void drm_gem_dmabuf_vunmap(struct dma_buf *dma_buf, void *vaddr);
void *drm_gem_dmabuf_kmap(struct dma_buf *dma_buf, unsigned long page_num);
void drm_gem_dmabuf_kunmap(struct dma_buf *dma_buf, unsigned long page_num,
			   void *addr);
int drm_gem_dmabuf_mmap(struct dma_buf *dma_buf, struct vm_area_struct *vma);

int drm_prime_sg_to_page_addr_arrays(struct sg_table *sgt, struct page **pages,
				     dma_addr_t *addrs, int max_pages);
struct sg_table *drm_prime_pages_to_sg(struct page **pages, unsigned int nr_pages);
void drm_prime_gem_destroy(struct drm_gem_object *obj, struct sg_table *sg);

int drm_gem_dmabuf_get_uuid(struct dma_buf *dma_buf, uuid_t *uuid);

#endif 
