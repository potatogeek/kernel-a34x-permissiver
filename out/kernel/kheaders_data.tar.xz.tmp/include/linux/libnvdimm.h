
#ifndef __LIBNVDIMM_H__
#define __LIBNVDIMM_H__
#include <linux/kernel.h>
#include <linux/sizes.h>
#include <linux/types.h>
#include <linux/uuid.h>
#include <linux/spinlock.h>
#include <linux/bio.h>

struct badrange_entry {
	u64 start;
	u64 length;
	struct list_head list;
};

struct badrange {
	struct list_head list;
	spinlock_t lock;
};

enum {
	
	NDD_ALIASING = 0,
	
	NDD_UNARMED = 1,
	
	NDD_LOCKED = 2,

	
	ND_IOCTL_MAX_BUFLEN = SZ_4M,
	ND_CMD_MAX_ELEM = 5,
	ND_CMD_MAX_ENVELOPE = 256,
	ND_MAX_MAPPINGS = 32,

	
	ND_REGION_PAGEMAP = 0,
	
	ND_REGION_PERSIST_CACHE = 1,
	
	ND_REGION_PERSIST_MEMCTRL = 2,

	
	ND_REGION_ASYNC = 3,

	
	DPA_RESOURCE_ADJUSTED = 1 << 0,
};

extern struct attribute_group nvdimm_bus_attribute_group;
extern struct attribute_group nvdimm_attribute_group;
extern struct attribute_group nd_device_attribute_group;
extern struct attribute_group nd_numa_attribute_group;
extern struct attribute_group nd_region_attribute_group;
extern struct attribute_group nd_mapping_attribute_group;

struct nvdimm;
struct nvdimm_bus_descriptor;
typedef int (*ndctl_fn)(struct nvdimm_bus_descriptor *nd_desc,
		struct nvdimm *nvdimm, unsigned int cmd, void *buf,
		unsigned int buf_len, int *cmd_rc);

struct device_node;
struct nvdimm_bus_descriptor {
	const struct attribute_group **attr_groups;
	unsigned long bus_dsm_mask;
	unsigned long cmd_mask;
	struct module *module;
	char *provider_name;
	struct device_node *of_node;
	ndctl_fn ndctl;
	int (*flush_probe)(struct nvdimm_bus_descriptor *nd_desc);
	int (*clear_to_send)(struct nvdimm_bus_descriptor *nd_desc,
			struct nvdimm *nvdimm, unsigned int cmd);
};

struct nd_cmd_desc {
	int in_num;
	int out_num;
	u32 in_sizes[ND_CMD_MAX_ELEM];
	int out_sizes[ND_CMD_MAX_ELEM];
};

struct nd_interleave_set {
	
	u64 cookie1;
	
	u64 cookie2;
	
	u64 altcookie;

	guid_t type_guid;
};

struct nd_mapping_desc {
	struct nvdimm *nvdimm;
	u64 start;
	u64 size;
	int position;
};

struct nd_region;
struct nd_region_desc {
	struct resource *res;
	struct nd_mapping_desc *mapping;
	u16 num_mappings;
	const struct attribute_group **attr_groups;
	struct nd_interleave_set *nd_set;
	void *provider_data;
	int num_lanes;
	int numa_node;
	unsigned long flags;
	struct device_node *of_node;
	int (*flush)(struct nd_region *nd_region, struct bio *bio);
};

struct device;
void *devm_nvdimm_memremap(struct device *dev, resource_size_t offset,
		size_t size, unsigned long flags);
static inline void __iomem *devm_nvdimm_ioremap(struct device *dev,
		resource_size_t offset, size_t size)
{
	return (void __iomem *) devm_nvdimm_memremap(dev, offset, size, 0);
}

struct nvdimm_bus;
struct module;
struct device;
struct nd_blk_region;
struct nd_blk_region_desc {
	int (*enable)(struct nvdimm_bus *nvdimm_bus, struct device *dev);
	int (*do_io)(struct nd_blk_region *ndbr, resource_size_t dpa,
			void *iobuf, u64 len, int rw);
	struct nd_region_desc ndr_desc;
};

static inline struct nd_blk_region_desc *to_blk_region_desc(
		struct nd_region_desc *ndr_desc)
{
	return container_of(ndr_desc, struct nd_blk_region_desc, ndr_desc);

}

void badrange_init(struct badrange *badrange);
int badrange_add(struct badrange *badrange, u64 addr, u64 length);
void badrange_forget(struct badrange *badrange, phys_addr_t start,
		unsigned int len);
int nvdimm_bus_add_badrange(struct nvdimm_bus *nvdimm_bus, u64 addr,
		u64 length);
struct nvdimm_bus *nvdimm_bus_register(struct device *parent,
		struct nvdimm_bus_descriptor *nfit_desc);
void nvdimm_bus_unregister(struct nvdimm_bus *nvdimm_bus);
struct nvdimm_bus *to_nvdimm_bus(struct device *dev);
struct nvdimm *to_nvdimm(struct device *dev);
struct nd_region *to_nd_region(struct device *dev);
struct device *nd_region_dev(struct nd_region *nd_region);
struct nd_blk_region *to_nd_blk_region(struct device *dev);
struct nvdimm_bus_descriptor *to_nd_desc(struct nvdimm_bus *nvdimm_bus);
struct device *to_nvdimm_bus_dev(struct nvdimm_bus *nvdimm_bus);
const char *nvdimm_name(struct nvdimm *nvdimm);
struct kobject *nvdimm_kobj(struct nvdimm *nvdimm);
unsigned long nvdimm_cmd_mask(struct nvdimm *nvdimm);
void *nvdimm_provider_data(struct nvdimm *nvdimm);
struct nvdimm *nvdimm_create(struct nvdimm_bus *nvdimm_bus, void *provider_data,
		const struct attribute_group **groups, unsigned long flags,
		unsigned long cmd_mask, int num_flush,
		struct resource *flush_wpq);
const struct nd_cmd_desc *nd_cmd_dimm_desc(int cmd);
const struct nd_cmd_desc *nd_cmd_bus_desc(int cmd);
u32 nd_cmd_in_size(struct nvdimm *nvdimm, int cmd,
		const struct nd_cmd_desc *desc, int idx, void *buf);
u32 nd_cmd_out_size(struct nvdimm *nvdimm, int cmd,
		const struct nd_cmd_desc *desc, int idx, const u32 *in_field,
		const u32 *out_field, unsigned long remainder);
int nvdimm_bus_check_dimm_count(struct nvdimm_bus *nvdimm_bus, int dimm_count);
struct nd_region *nvdimm_pmem_region_create(struct nvdimm_bus *nvdimm_bus,
		struct nd_region_desc *ndr_desc);
struct nd_region *nvdimm_blk_region_create(struct nvdimm_bus *nvdimm_bus,
		struct nd_region_desc *ndr_desc);
struct nd_region *nvdimm_volatile_region_create(struct nvdimm_bus *nvdimm_bus,
		struct nd_region_desc *ndr_desc);
void *nd_region_provider_data(struct nd_region *nd_region);
void *nd_blk_region_provider_data(struct nd_blk_region *ndbr);
void nd_blk_region_set_provider_data(struct nd_blk_region *ndbr, void *data);
struct nvdimm *nd_blk_region_to_dimm(struct nd_blk_region *ndbr);
unsigned long nd_blk_memremap_flags(struct nd_blk_region *ndbr);
unsigned int nd_region_acquire_lane(struct nd_region *nd_region);
void nd_region_release_lane(struct nd_region *nd_region, unsigned int lane);
u64 nd_fletcher64(void *addr, size_t len, bool le);
int nvdimm_flush(struct nd_region *nd_region, struct bio *bio);
int generic_nvdimm_flush(struct nd_region *nd_region);
int nvdimm_has_flush(struct nd_region *nd_region);
int nvdimm_has_cache(struct nd_region *nd_region);

#ifdef CONFIG_ARCH_HAS_PMEM_API
#define ARCH_MEMREMAP_PMEM MEMREMAP_WB
void arch_wb_cache_pmem(void *addr, size_t size);
void arch_invalidate_pmem(void *addr, size_t size);
#else
#define ARCH_MEMREMAP_PMEM MEMREMAP_WT
static inline void arch_wb_cache_pmem(void *addr, size_t size)
{
}
static inline void arch_invalidate_pmem(void *addr, size_t size)
{
}
#endif

#endif 
