

#ifndef REMOTEPROC_H
#define REMOTEPROC_H

#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/virtio.h>
#include <linux/completion.h>
#include <linux/idr.h>
#include <linux/of.h>


struct resource_table {
	u32 ver;
	u32 num;
	u32 reserved[2];
	u32 offset[0];
} __packed;


struct fw_rsc_hdr {
	u32 type;
	u8 data[0];
} __packed;


enum fw_resource_type {
	RSC_CARVEOUT	= 0,
	RSC_DEVMEM	= 1,
	RSC_TRACE	= 2,
	RSC_VDEV	= 3,
	RSC_LAST	= 4,
};

#define FW_RSC_ADDR_ANY (-1)


struct fw_rsc_carveout {
	u32 da;
	u32 pa;
	u32 len;
	u32 flags;
	u32 reserved;
	u8 name[32];
} __packed;


struct fw_rsc_devmem {
	u32 da;
	u32 pa;
	u32 len;
	u32 flags;
	u32 reserved;
	u8 name[32];
} __packed;


struct fw_rsc_trace {
	u32 da;
	u32 len;
	u32 reserved;
	u8 name[32];
} __packed;


struct fw_rsc_vdev_vring {
	u32 da;
	u32 align;
	u32 num;
	u32 notifyid;
	u32 pa;
} __packed;


struct fw_rsc_vdev {
	u32 id;
	u32 notifyid;
	u32 dfeatures;
	u32 gfeatures;
	u32 config_len;
	u8 status;
	u8 num_of_vrings;
	u8 reserved[2];
	struct fw_rsc_vdev_vring vring[0];
} __packed;


struct rproc_mem_entry {
	void *va;
	dma_addr_t dma;
	int len;
	u32 da;
	void *priv;
	struct list_head node;
};

struct rproc;
struct firmware;


struct rproc_ops {
	int (*start)(struct rproc *rproc);
	int (*stop)(struct rproc *rproc);
	void (*kick)(struct rproc *rproc, int vqid);
	void * (*da_to_va)(struct rproc *rproc, u64 da, int len);
	int (*parse_fw)(struct rproc *rproc, const struct firmware *fw);
	struct resource_table *(*find_loaded_rsc_table)(
				struct rproc *rproc, const struct firmware *fw);
	int (*load)(struct rproc *rproc, const struct firmware *fw);
	int (*sanity_check)(struct rproc *rproc, const struct firmware *fw);
	u32 (*get_boot_addr)(struct rproc *rproc, const struct firmware *fw);
};


enum rproc_state {
	RPROC_OFFLINE	= 0,
	RPROC_SUSPENDED	= 1,
	RPROC_RUNNING	= 2,
	RPROC_CRASHED	= 3,
	RPROC_DELETED	= 4,
	RPROC_LAST	= 5,
};


enum rproc_crash_type {
	RPROC_MMUFAULT,
	RPROC_WATCHDOG,
	RPROC_FATAL_ERROR,
};


struct rproc_dump_segment {
	struct list_head node;

	dma_addr_t da;
	size_t size;

	loff_t offset;
};


struct rproc {
	struct list_head node;
	struct iommu_domain *domain;
	const char *name;
	char *firmware;
	void *priv;
	struct rproc_ops *ops;
	struct device dev;
	atomic_t power;
	unsigned int state;
	struct mutex lock;
	struct dentry *dbg_dir;
	struct list_head traces;
	int num_traces;
	struct list_head carveouts;
	struct list_head mappings;
	u32 bootaddr;
	struct list_head rvdevs;
	struct list_head subdevs;
	struct idr notifyids;
	int index;
	struct work_struct crash_handler;
	unsigned int crash_cnt;
	bool recovery_disabled;
	int max_notifyid;
	struct resource_table *table_ptr;
	struct resource_table *cached_table;
	size_t table_sz;
	bool has_iommu;
	bool auto_boot;
	struct list_head dump_segments;
};


struct rproc_subdev {
	struct list_head node;

	int (*prepare)(struct rproc_subdev *subdev);
	int (*start)(struct rproc_subdev *subdev);
	void (*stop)(struct rproc_subdev *subdev, bool crashed);
	void (*unprepare)(struct rproc_subdev *subdev);
};



#define RVDEV_NUM_VRINGS 2


struct rproc_vring {
	void *va;
	dma_addr_t dma;
	int len;
	u32 da;
	u32 align;
	int notifyid;
	struct rproc_vdev *rvdev;
	struct virtqueue *vq;
};


struct rproc_vdev {
	struct kref refcount;

	struct rproc_subdev subdev;

	unsigned int id;
	struct list_head node;
	struct rproc *rproc;
	struct virtio_device vdev;
	struct rproc_vring vring[RVDEV_NUM_VRINGS];
	u32 rsc_offset;
};

struct rproc *rproc_get_by_phandle(phandle phandle);
struct rproc *rproc_get_by_child(struct device *dev);

struct rproc *rproc_alloc(struct device *dev, const char *name,
			  const struct rproc_ops *ops,
			  const char *firmware, int len);
void rproc_put(struct rproc *rproc);
int rproc_add(struct rproc *rproc);
int rproc_del(struct rproc *rproc);
void rproc_free(struct rproc *rproc);

int rproc_boot(struct rproc *rproc);
void rproc_shutdown(struct rproc *rproc);
void rproc_report_crash(struct rproc *rproc, enum rproc_crash_type type);
int rproc_coredump_add_segment(struct rproc *rproc, dma_addr_t da, size_t size);

static inline struct rproc_vdev *vdev_to_rvdev(struct virtio_device *vdev)
{
	return container_of(vdev, struct rproc_vdev, vdev);
}

static inline struct rproc *vdev_to_rproc(struct virtio_device *vdev)
{
	struct rproc_vdev *rvdev = vdev_to_rvdev(vdev);

	return rvdev->rproc;
}

void rproc_add_subdev(struct rproc *rproc, struct rproc_subdev *subdev);

void rproc_remove_subdev(struct rproc *rproc, struct rproc_subdev *subdev);

#endif 
