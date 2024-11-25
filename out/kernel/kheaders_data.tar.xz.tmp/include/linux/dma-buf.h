
#ifndef __DMA_BUF_H__
#define __DMA_BUF_H__

#include <linux/file.h>
#include <linux/err.h>
#include <linux/scatterlist.h>
#include <linux/list.h>
#include <linux/dma-mapping.h>
#include <linux/fs.h>
#include <linux/dma-fence.h>
#include <linux/wait.h>

struct device;
struct dma_buf;
struct dma_buf_attachment;


struct dma_buf_ops {
	
	int (*attach)(struct dma_buf *, struct dma_buf_attachment *);

	
	void (*detach)(struct dma_buf *, struct dma_buf_attachment *);

	
	struct sg_table * (*map_dma_buf)(struct dma_buf_attachment *,
					 enum dma_data_direction);
	
	void (*unmap_dma_buf)(struct dma_buf_attachment *,
			      struct sg_table *,
			      enum dma_data_direction);

	

	
	void (*release)(struct dma_buf *);

	
	int (*begin_cpu_access)(struct dma_buf *, enum dma_data_direction);

	
	int (*begin_cpu_access_umapped)(struct dma_buf *dmabuf,
					enum dma_data_direction);

	
	int (*begin_cpu_access_partial)(struct dma_buf *dmabuf,
					enum dma_data_direction,
					unsigned int offset, unsigned int len);

	
	int (*end_cpu_access)(struct dma_buf *, enum dma_data_direction);

	
	int (*end_cpu_access_umapped)(struct dma_buf *dmabuf,
				      enum dma_data_direction);

	
	int (*end_cpu_access_partial)(struct dma_buf *dmabuf,
				      enum dma_data_direction,
				      unsigned int offset, unsigned int len);

	void *(*map_atomic)(struct dma_buf *dmabuf, unsigned long page_num);
	void (*unmap_atomic)(struct dma_buf *dma_buf, unsigned long page_num, void *vaddr);
	void *(*map)(struct dma_buf *, unsigned long);
	void (*unmap)(struct dma_buf *, unsigned long, void *);

	
	int (*mmap)(struct dma_buf *, struct vm_area_struct *vma);

	void *(*vmap)(struct dma_buf *);
	void (*vunmap)(struct dma_buf *, void *vaddr);

	
	int (*get_uuid)(struct dma_buf *dmabuf, uuid_t *uuid);

	
	int (*get_flags)(struct dma_buf *dmabuf, unsigned long *flags);
};


typedef int (*dma_buf_destructor)(struct dma_buf *dmabuf, void *dtor_data);


struct dma_buf {
	atomic_t ref_dbg;
	size_t size;
	struct file *file;
	struct list_head attachments;
	const struct dma_buf_ops *ops;
	struct mutex lock;
	unsigned vmapping_counter;
	void *vmap_ptr;
	const char *exp_name;
	const char *name;
	spinlock_t name_lock; 
	struct module *owner;
	struct list_head node;
	struct list_head list_node;
	void *priv;
	struct reservation_object *resv;

	
	wait_queue_head_t poll;

	struct dma_buf_poll_cb_t {
		struct dma_fence_cb cb;
		wait_queue_head_t *poll;

		__poll_t active;
	} cb_excl, cb_shared;
	dma_buf_destructor dtor;
	void *dtor_data;
	atomic_t dent_count;
};


struct dma_buf_attachment {
	struct dma_buf *dmabuf;
	struct device *dev;
	struct list_head node;
	void *priv;
	unsigned long dma_map_attrs;
};


struct dma_buf_export_info {
	const char *exp_name;
	struct module *owner;
	const struct dma_buf_ops *ops;
	size_t size;
	int flags;
	struct reservation_object *resv;
	void *priv;
};


#define DEFINE_DMA_BUF_EXPORT_INFO(name)	\
	struct dma_buf_export_info name = { .exp_name = KBUILD_MODNAME, \
					 .owner = THIS_MODULE }


static inline void get_dma_buf(struct dma_buf *dmabuf)
{
	atomic_inc(&dmabuf->ref_dbg);
	get_file(dmabuf->file);
}

int is_dma_buf_file(struct file *file);
struct dma_buf_attachment *dma_buf_attach(struct dma_buf *dmabuf,
							struct device *dev);
void dma_buf_detach(struct dma_buf *dmabuf,
				struct dma_buf_attachment *dmabuf_attach);

struct dma_buf *dma_buf_export(const struct dma_buf_export_info *exp_info);

int dma_buf_fd(struct dma_buf *dmabuf, int flags);
struct dma_buf *dma_buf_get(int fd);
void dma_buf_put(struct dma_buf *dmabuf);

struct sg_table *dma_buf_map_attachment(struct dma_buf_attachment *,
					enum dma_data_direction);
void dma_buf_unmap_attachment(struct dma_buf_attachment *, struct sg_table *,
				enum dma_data_direction);
int dma_buf_begin_cpu_access(struct dma_buf *dma_buf,
			     enum dma_data_direction dir);
int dma_buf_begin_cpu_access_partial(struct dma_buf *dma_buf,
				     enum dma_data_direction dir,
				     unsigned int offset, unsigned int len);
int dma_buf_end_cpu_access(struct dma_buf *dma_buf,
			   enum dma_data_direction dir);
int dma_buf_end_cpu_access_partial(struct dma_buf *dma_buf,
				     enum dma_data_direction dir,
				     unsigned int offset, unsigned int len);
void *dma_buf_kmap(struct dma_buf *, unsigned long);
void dma_buf_kunmap(struct dma_buf *, unsigned long, void *);

int dma_buf_mmap(struct dma_buf *, struct vm_area_struct *,
		 unsigned long);
void *dma_buf_vmap(struct dma_buf *);
void dma_buf_vunmap(struct dma_buf *, void *vaddr);
int dma_buf_get_flags(struct dma_buf *dmabuf, unsigned long *flags);
int dma_buf_get_uuid(struct dma_buf *dmabuf, uuid_t *uuid);


static inline void dma_buf_set_destructor(struct dma_buf *dmabuf,
					  dma_buf_destructor dtor,
					  void *dtor_data)
{
	dmabuf->dtor = dtor;
	dmabuf->dtor_data = dtor_data;
}

#endif 
