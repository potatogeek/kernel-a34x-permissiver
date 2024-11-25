// SPDX-License-Identifier: GPL-2.0
/*
 * Memory subsystem support
 *
 * Written by Matt Tolentino <matthew.e.tolentino@intel.com>
 *            Dave Hansen <haveblue@us.ibm.com>
 *
 * This file provides the necessary infrastructure to represent
 * a SPARSEMEM-memory-model system's physical memory in /sysfs.
 * All arch-independent code that assumes MEMORY_HOTPLUG requires
 * SPARSEMEM should be contained here, or in mm/memory_hotplug.c.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/topology.h>
#include <linux/capability.h>
#include <linux/device.h>
#include <linux/memory.h>
#include <linux/memory_hotplug.h>
#include <linux/mm.h>
<<<<<<< HEAD
#include <linux/mutex.h>
#include <linux/stat.h>
#include <linux/slab.h>
=======
#include <linux/stat.h>
#include <linux/slab.h>
#include <linux/xarray.h>
>>>>>>> upstream/android-13

#include <linux/atomic.h>
#include <linux/uaccess.h>

<<<<<<< HEAD
static DEFINE_MUTEX(mem_sysfs_mutex);

#define MEMORY_CLASS_NAME	"memory"

=======
#define MEMORY_CLASS_NAME	"memory"

static const char *const online_type_to_str[] = {
	[MMOP_OFFLINE] = "offline",
	[MMOP_ONLINE] = "online",
	[MMOP_ONLINE_KERNEL] = "online_kernel",
	[MMOP_ONLINE_MOVABLE] = "online_movable",
};

int mhp_online_type_from_str(const char *str)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(online_type_to_str); i++) {
		if (sysfs_streq(str, online_type_to_str[i]))
			return i;
	}
	return -EINVAL;
}

>>>>>>> upstream/android-13
#define to_memory_block(dev) container_of(dev, struct memory_block, dev)

static int sections_per_block;

<<<<<<< HEAD
static inline int base_memory_block_id(int section_nr)
=======
static inline unsigned long memory_block_id(unsigned long section_nr)
>>>>>>> upstream/android-13
{
	return section_nr / sections_per_block;
}

<<<<<<< HEAD
static inline int pfn_to_block_id(unsigned long pfn)
{
	return base_memory_block_id(pfn_to_section_nr(pfn));
=======
static inline unsigned long pfn_to_block_id(unsigned long pfn)
{
	return memory_block_id(pfn_to_section_nr(pfn));
}

static inline unsigned long phys_to_block_id(unsigned long phys)
{
	return pfn_to_block_id(PFN_DOWN(phys));
>>>>>>> upstream/android-13
}

static int memory_subsys_online(struct device *dev);
static int memory_subsys_offline(struct device *dev);

static struct bus_type memory_subsys = {
	.name = MEMORY_CLASS_NAME,
	.dev_name = MEMORY_CLASS_NAME,
	.online = memory_subsys_online,
	.offline = memory_subsys_offline,
};

<<<<<<< HEAD
=======
/*
 * Memory blocks are cached in a local radix tree to avoid
 * a costly linear search for the corresponding device on
 * the subsystem bus.
 */
static DEFINE_XARRAY(memory_blocks);

/*
 * Memory groups, indexed by memory group id (mgid).
 */
static DEFINE_XARRAY_FLAGS(memory_groups, XA_FLAGS_ALLOC);
#define MEMORY_GROUP_MARK_DYNAMIC	XA_MARK_1

>>>>>>> upstream/android-13
static BLOCKING_NOTIFIER_HEAD(memory_chain);

int register_memory_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&memory_chain, nb);
}
EXPORT_SYMBOL(register_memory_notifier);

void unregister_memory_notifier(struct notifier_block *nb)
{
	blocking_notifier_chain_unregister(&memory_chain, nb);
}
EXPORT_SYMBOL(unregister_memory_notifier);

<<<<<<< HEAD
static ATOMIC_NOTIFIER_HEAD(memory_isolate_chain);

int register_memory_isolate_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_register(&memory_isolate_chain, nb);
}
EXPORT_SYMBOL(register_memory_isolate_notifier);

void unregister_memory_isolate_notifier(struct notifier_block *nb)
{
	atomic_notifier_chain_unregister(&memory_isolate_chain, nb);
}
EXPORT_SYMBOL(unregister_memory_isolate_notifier);

=======
>>>>>>> upstream/android-13
static void memory_block_release(struct device *dev)
{
	struct memory_block *mem = to_memory_block(dev);

	kfree(mem);
}

unsigned long __weak memory_block_size_bytes(void)
{
	return MIN_MEMORY_BLOCK_SIZE;
}
<<<<<<< HEAD

static unsigned long get_memory_block_size(void)
{
	unsigned long block_sz;

	block_sz = memory_block_size_bytes();

	/* Validate blk_sz is a power of 2 and not less than section size */
	if ((block_sz & (block_sz - 1)) || (block_sz < MIN_MEMORY_BLOCK_SIZE)) {
		WARN_ON(1);
		block_sz = MIN_MEMORY_BLOCK_SIZE;
	}

	return block_sz;
}

/*
 * use this as the physical section index that this memsection
 * uses.
 */

static ssize_t show_mem_start_phys_index(struct device *dev,
			struct device_attribute *attr, char *buf)
=======
EXPORT_SYMBOL_GPL(memory_block_size_bytes);

/*
 * Show the first physical section index (number) of this memory block.
 */
static ssize_t phys_index_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
>>>>>>> upstream/android-13
{
	struct memory_block *mem = to_memory_block(dev);
	unsigned long phys_index;

	phys_index = mem->start_section_nr / sections_per_block;
<<<<<<< HEAD
	return sprintf(buf, "%08lx\n", phys_index);
}

/*
 * Show whether the section of memory is likely to be hot-removable
 */
static ssize_t show_mem_removable(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	unsigned long i, pfn;
	int ret = 1;
	struct memory_block *mem = to_memory_block(dev);

	if (mem->state != MEM_ONLINE)
		goto out;

	for (i = 0; i < sections_per_block; i++) {
		if (!present_section_nr(mem->start_section_nr + i))
			continue;
		pfn = section_nr_to_pfn(mem->start_section_nr + i);
		ret &= is_mem_section_removable(pfn, PAGES_PER_SECTION);
	}

out:
	return sprintf(buf, "%d\n", ret);
=======

	return sysfs_emit(buf, "%08lx\n", phys_index);
}

/*
 * Legacy interface that we cannot remove. Always indicate "removable"
 * with CONFIG_MEMORY_HOTREMOVE - bad heuristic.
 */
static ssize_t removable_show(struct device *dev, struct device_attribute *attr,
			      char *buf)
{
	return sysfs_emit(buf, "%d\n", (int)IS_ENABLED(CONFIG_MEMORY_HOTREMOVE));
>>>>>>> upstream/android-13
}

/*
 * online, offline, going offline, etc.
 */
<<<<<<< HEAD
static ssize_t show_mem_state(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct memory_block *mem = to_memory_block(dev);
	ssize_t len = 0;
=======
static ssize_t state_show(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	struct memory_block *mem = to_memory_block(dev);
	const char *output;
>>>>>>> upstream/android-13

	/*
	 * We can probably put these states in a nice little array
	 * so that they're not open-coded
	 */
	switch (mem->state) {
	case MEM_ONLINE:
<<<<<<< HEAD
		len = sprintf(buf, "online\n");
		break;
	case MEM_OFFLINE:
		len = sprintf(buf, "offline\n");
		break;
	case MEM_GOING_OFFLINE:
		len = sprintf(buf, "going-offline\n");
		break;
	default:
		len = sprintf(buf, "ERROR-UNKNOWN-%ld\n",
				mem->state);
		WARN_ON(1);
		break;
	}

	return len;
=======
		output = "online";
		break;
	case MEM_OFFLINE:
		output = "offline";
		break;
	case MEM_GOING_OFFLINE:
		output = "going-offline";
		break;
	default:
		WARN_ON(1);
		return sysfs_emit(buf, "ERROR-UNKNOWN-%ld\n", mem->state);
	}

	return sysfs_emit(buf, "%s\n", output);
>>>>>>> upstream/android-13
}

int memory_notify(unsigned long val, void *v)
{
	return blocking_notifier_call_chain(&memory_chain, val, v);
}

<<<<<<< HEAD
int memory_isolate_notify(unsigned long val, void *v)
{
	return atomic_notifier_call_chain(&memory_isolate_chain, val, v);
}

/*
 * The probe routines leave the pages uninitialized, just as the bootmem code
 * does. Make sure we do not access them, but instead use only information from
 * within sections.
 */
static bool pages_correctly_probed(unsigned long start_pfn)
{
	unsigned long section_nr = pfn_to_section_nr(start_pfn);
	unsigned long section_nr_end = section_nr + sections_per_block;
	unsigned long pfn = start_pfn;

	/*
	 * memmap between sections is not contiguous except with
	 * SPARSEMEM_VMEMMAP. We lookup the page once per section
	 * and assume memmap is contiguous within each section
	 */
	for (; section_nr < section_nr_end; section_nr++) {
		if (WARN_ON_ONCE(!pfn_valid(pfn)))
			return false;

		if (!present_section_nr(section_nr)) {
			pr_warn("section %ld pfn[%lx, %lx) not present",
				section_nr, pfn, pfn + PAGES_PER_SECTION);
			return false;
		} else if (!valid_section_nr(section_nr)) {
			pr_warn("section %ld pfn[%lx, %lx) no valid memmap",
				section_nr, pfn, pfn + PAGES_PER_SECTION);
			return false;
		} else if (online_section_nr(section_nr)) {
			pr_warn("section %ld pfn[%lx, %lx) is already online",
				section_nr, pfn, pfn + PAGES_PER_SECTION);
			return false;
		}
		pfn += PAGES_PER_SECTION;
	}

	return true;
=======
static int memory_block_online(struct memory_block *mem)
{
	unsigned long start_pfn = section_nr_to_pfn(mem->start_section_nr);
	unsigned long nr_pages = PAGES_PER_SECTION * sections_per_block;
	unsigned long nr_vmemmap_pages = mem->nr_vmemmap_pages;
	struct zone *zone;
	int ret;

	zone = zone_for_pfn_range(mem->online_type, mem->nid, mem->group,
				  start_pfn, nr_pages);

	/*
	 * Although vmemmap pages have a different lifecycle than the pages
	 * they describe (they remain until the memory is unplugged), doing
	 * their initialization and accounting at memory onlining/offlining
	 * stage helps to keep accounting easier to follow - e.g vmemmaps
	 * belong to the same zone as the memory they backed.
	 */
	if (nr_vmemmap_pages) {
		ret = mhp_init_memmap_on_memory(start_pfn, nr_vmemmap_pages, zone);
		if (ret)
			return ret;
	}

	ret = online_pages(start_pfn + nr_vmemmap_pages,
			   nr_pages - nr_vmemmap_pages, zone, mem->group);
	if (ret) {
		if (nr_vmemmap_pages)
			mhp_deinit_memmap_on_memory(start_pfn, nr_vmemmap_pages);
		return ret;
	}

	/*
	 * Account once onlining succeeded. If the zone was unpopulated, it is
	 * now already properly populated.
	 */
	if (nr_vmemmap_pages)
		adjust_present_page_count(pfn_to_page(start_pfn), mem->group,
					  nr_vmemmap_pages);

	return ret;
}

static int memory_block_offline(struct memory_block *mem)
{
	unsigned long start_pfn = section_nr_to_pfn(mem->start_section_nr);
	unsigned long nr_pages = PAGES_PER_SECTION * sections_per_block;
	unsigned long nr_vmemmap_pages = mem->nr_vmemmap_pages;
	int ret;

	/*
	 * Unaccount before offlining, such that unpopulated zone and kthreads
	 * can properly be torn down in offline_pages().
	 */
	if (nr_vmemmap_pages)
		adjust_present_page_count(pfn_to_page(start_pfn), mem->group,
					  -nr_vmemmap_pages);

	ret = offline_pages(start_pfn + nr_vmemmap_pages,
			    nr_pages - nr_vmemmap_pages, mem->group);
	if (ret) {
		/* offline_pages() failed. Account back. */
		if (nr_vmemmap_pages)
			adjust_present_page_count(pfn_to_page(start_pfn),
						  mem->group, nr_vmemmap_pages);
		return ret;
	}

	if (nr_vmemmap_pages)
		mhp_deinit_memmap_on_memory(start_pfn, nr_vmemmap_pages);

	return ret;
>>>>>>> upstream/android-13
}

/*
 * MEMORY_HOTPLUG depends on SPARSEMEM in mm/Kconfig, so it is
 * OK to have direct references to sparsemem variables in here.
 */
static int
<<<<<<< HEAD
memory_block_action(unsigned long start_section_nr, unsigned long action,
		    int online_type)
{
	unsigned long start_pfn;
	unsigned long nr_pages = PAGES_PER_SECTION * sections_per_block;
	int ret;

	start_pfn = section_nr_to_pfn(start_section_nr);

	switch (action) {
	case MEM_ONLINE:
		if (!pages_correctly_probed(start_pfn))
			return -EBUSY;

		ret = online_pages(start_pfn, nr_pages, online_type);
		break;
	case MEM_OFFLINE:
		ret = offline_pages(start_pfn, nr_pages);
		break;
	default:
		WARN(1, KERN_WARNING "%s(%ld, %ld) unknown action: "
		     "%ld\n", __func__, start_section_nr, action, action);
=======
memory_block_action(struct memory_block *mem, unsigned long action)
{
	int ret;

	switch (action) {
	case MEM_ONLINE:
		ret = memory_block_online(mem);
		break;
	case MEM_OFFLINE:
		ret = memory_block_offline(mem);
		break;
	default:
		WARN(1, KERN_WARNING "%s(%ld, %ld) unknown action: "
		     "%ld\n", __func__, mem->start_section_nr, action, action);
>>>>>>> upstream/android-13
		ret = -EINVAL;
	}

	return ret;
}

static int memory_block_change_state(struct memory_block *mem,
		unsigned long to_state, unsigned long from_state_req)
{
	int ret = 0;

	if (mem->state != from_state_req)
		return -EINVAL;

	if (to_state == MEM_OFFLINE)
		mem->state = MEM_GOING_OFFLINE;

<<<<<<< HEAD
	ret = memory_block_action(mem->start_section_nr, to_state,
				mem->online_type);

=======
	ret = memory_block_action(mem, to_state);
>>>>>>> upstream/android-13
	mem->state = ret ? from_state_req : to_state;

	return ret;
}

/* The device lock serializes operations on memory_subsys_[online|offline] */
static int memory_subsys_online(struct device *dev)
{
	struct memory_block *mem = to_memory_block(dev);
	int ret;

	if (mem->state == MEM_ONLINE)
		return 0;

	/*
<<<<<<< HEAD
	 * If we are called from store_mem_state(), online_type will be
	 * set >= 0 Otherwise we were called from the device online
	 * attribute and need to set the online_type.
	 */
	if (mem->online_type < 0)
		mem->online_type = MMOP_ONLINE_KEEP;

	ret = memory_block_change_state(mem, MEM_ONLINE, MEM_OFFLINE);

	/* clear online_type */
	mem->online_type = -1;
=======
	 * When called via device_online() without configuring the online_type,
	 * we want to default to MMOP_ONLINE.
	 */
	if (mem->online_type == MMOP_OFFLINE)
		mem->online_type = MMOP_ONLINE;

	ret = memory_block_change_state(mem, MEM_ONLINE, MEM_OFFLINE);
	mem->online_type = MMOP_OFFLINE;
>>>>>>> upstream/android-13

	return ret;
}

static int memory_subsys_offline(struct device *dev)
{
	struct memory_block *mem = to_memory_block(dev);

	if (mem->state == MEM_OFFLINE)
		return 0;

<<<<<<< HEAD
	/* Can't offline block with non-present sections */
	if (mem->section_count != sections_per_block)
		return -EINVAL;

	return memory_block_change_state(mem, MEM_OFFLINE, MEM_ONLINE);
}

static ssize_t
store_mem_state(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct memory_block *mem = to_memory_block(dev);
	int ret, online_type;
=======
	return memory_block_change_state(mem, MEM_OFFLINE, MEM_ONLINE);
}

static ssize_t state_store(struct device *dev, struct device_attribute *attr,
			   const char *buf, size_t count)
{
	const int online_type = mhp_online_type_from_str(buf);
	struct memory_block *mem = to_memory_block(dev);
	int ret;

	if (online_type < 0)
		return -EINVAL;
>>>>>>> upstream/android-13

	ret = lock_device_hotplug_sysfs();
	if (ret)
		return ret;

<<<<<<< HEAD
	if (sysfs_streq(buf, "online_kernel"))
		online_type = MMOP_ONLINE_KERNEL;
	else if (sysfs_streq(buf, "online_movable"))
		online_type = MMOP_ONLINE_MOVABLE;
	else if (sysfs_streq(buf, "online"))
		online_type = MMOP_ONLINE_KEEP;
	else if (sysfs_streq(buf, "offline"))
		online_type = MMOP_OFFLINE;
	else {
		ret = -EINVAL;
		goto err;
	}

	switch (online_type) {
	case MMOP_ONLINE_KERNEL:
	case MMOP_ONLINE_MOVABLE:
	case MMOP_ONLINE_KEEP:
=======
	switch (online_type) {
	case MMOP_ONLINE_KERNEL:
	case MMOP_ONLINE_MOVABLE:
	case MMOP_ONLINE:
>>>>>>> upstream/android-13
		/* mem->online_type is protected by device_hotplug_lock */
		mem->online_type = online_type;
		ret = device_online(&mem->dev);
		break;
	case MMOP_OFFLINE:
		ret = device_offline(&mem->dev);
		break;
	default:
		ret = -EINVAL; /* should never happen */
	}

<<<<<<< HEAD
err:
=======
>>>>>>> upstream/android-13
	unlock_device_hotplug();

	if (ret < 0)
		return ret;
	if (ret)
		return -EINVAL;

	return count;
}

/*
<<<<<<< HEAD
 * phys_device is a bad name for this.  What I really want
 * is a way to differentiate between memory ranges that
 * are part of physical devices that constitute
 * a complete removable unit or fru.
 * i.e. do these ranges belong to the same physical device,
 * s.t. if I offline all of these sections I can then
 * remove the physical device?
 */
static ssize_t show_phys_device(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct memory_block *mem = to_memory_block(dev);
	return sprintf(buf, "%d\n", mem->phys_device);
}

#ifdef CONFIG_MEMORY_HOTREMOVE
static void print_allowed_zone(char *buf, int nid, unsigned long start_pfn,
		unsigned long nr_pages, int online_type,
		struct zone *default_zone)
{
	struct zone *zone;

	zone = zone_for_pfn_range(online_type, nid, start_pfn, nr_pages);
	if (zone != default_zone) {
		strcat(buf, " ");
		strcat(buf, zone->name);
	}
}

static ssize_t show_valid_zones(struct device *dev,
=======
 * Legacy interface that we cannot remove: s390x exposes the storage increment
 * covered by a memory block, allowing for identifying which memory blocks
 * comprise a storage increment. Since a memory block spans complete
 * storage increments nowadays, this interface is basically unused. Other
 * archs never exposed != 0.
 */
static ssize_t phys_device_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct memory_block *mem = to_memory_block(dev);
	unsigned long start_pfn = section_nr_to_pfn(mem->start_section_nr);

	return sysfs_emit(buf, "%d\n",
			  arch_get_memory_phys_device(start_pfn));
}

#ifdef CONFIG_MEMORY_HOTREMOVE
static int print_allowed_zone(char *buf, int len, int nid,
			      struct memory_group *group,
			      unsigned long start_pfn, unsigned long nr_pages,
			      int online_type, struct zone *default_zone)
{
	struct zone *zone;

	zone = zone_for_pfn_range(online_type, nid, group, start_pfn, nr_pages);
	if (zone == default_zone)
		return 0;

	return sysfs_emit_at(buf, len, " %s", zone->name);
}

static ssize_t valid_zones_show(struct device *dev,
>>>>>>> upstream/android-13
				struct device_attribute *attr, char *buf)
{
	struct memory_block *mem = to_memory_block(dev);
	unsigned long start_pfn = section_nr_to_pfn(mem->start_section_nr);
	unsigned long nr_pages = PAGES_PER_SECTION * sections_per_block;
<<<<<<< HEAD
	unsigned long valid_start_pfn, valid_end_pfn;
	struct zone *default_zone;
	int nid;
=======
	struct memory_group *group = mem->group;
	struct zone *default_zone;
	int nid = mem->nid;
	int len = 0;
>>>>>>> upstream/android-13

	/*
	 * Check the existing zone. Make sure that we do that only on the
	 * online nodes otherwise the page_zone is not reliable
	 */
	if (mem->state == MEM_ONLINE) {
		/*
		 * The block contains more than one zone can not be offlined.
		 * This can happen e.g. for ZONE_DMA and ZONE_DMA32
		 */
<<<<<<< HEAD
		if (!test_pages_in_a_zone(start_pfn, start_pfn + nr_pages,
					  &valid_start_pfn, &valid_end_pfn))
			return sprintf(buf, "none\n");
		start_pfn = valid_start_pfn;
		strcat(buf, page_zone(pfn_to_page(start_pfn))->name);
		goto out;
	}

	nid = mem->nid;
	default_zone = zone_for_pfn_range(MMOP_ONLINE_KEEP, nid, start_pfn, nr_pages);
	strcat(buf, default_zone->name);

	print_allowed_zone(buf, nid, start_pfn, nr_pages, MMOP_ONLINE_KERNEL,
			default_zone);
	print_allowed_zone(buf, nid, start_pfn, nr_pages, MMOP_ONLINE_MOVABLE,
			default_zone);
out:
	strcat(buf, "\n");

	return strlen(buf);
}
static DEVICE_ATTR(valid_zones, 0444, show_valid_zones, NULL);
#endif

static DEVICE_ATTR(phys_index, 0444, show_mem_start_phys_index, NULL);
static DEVICE_ATTR(state, 0644, show_mem_state, store_mem_state);
static DEVICE_ATTR(phys_device, 0444, show_phys_device, NULL);
static DEVICE_ATTR(removable, 0444, show_mem_removable, NULL);

/*
 * Block size attribute stuff
 */
static ssize_t
print_block_size(struct device *dev, struct device_attribute *attr,
		 char *buf)
{
	return sprintf(buf, "%lx\n", get_memory_block_size());
}

static DEVICE_ATTR(block_size_bytes, 0444, print_block_size, NULL);
=======
		default_zone = test_pages_in_a_zone(start_pfn,
						    start_pfn + nr_pages);
		if (!default_zone)
			return sysfs_emit(buf, "%s\n", "none");
		len += sysfs_emit_at(buf, len, "%s", default_zone->name);
		goto out;
	}

	default_zone = zone_for_pfn_range(MMOP_ONLINE, nid, group,
					  start_pfn, nr_pages);

	len += sysfs_emit_at(buf, len, "%s", default_zone->name);
	len += print_allowed_zone(buf, len, nid, group, start_pfn, nr_pages,
				  MMOP_ONLINE_KERNEL, default_zone);
	len += print_allowed_zone(buf, len, nid, group, start_pfn, nr_pages,
				  MMOP_ONLINE_MOVABLE, default_zone);
out:
	len += sysfs_emit_at(buf, len, "\n");
	return len;
}
static DEVICE_ATTR_RO(valid_zones);
#endif

static DEVICE_ATTR_RO(phys_index);
static DEVICE_ATTR_RW(state);
static DEVICE_ATTR_RO(phys_device);
static DEVICE_ATTR_RO(removable);

/*
 * Show the memory block size (shared by all memory blocks).
 */
static ssize_t block_size_bytes_show(struct device *dev,
				     struct device_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%lx\n", memory_block_size_bytes());
}

static DEVICE_ATTR_RO(block_size_bytes);
>>>>>>> upstream/android-13

/*
 * Memory auto online policy.
 */

<<<<<<< HEAD
static ssize_t
show_auto_online_blocks(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	if (memhp_auto_online)
		return sprintf(buf, "online\n");
	else
		return sprintf(buf, "offline\n");
}

static ssize_t
store_auto_online_blocks(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	if (sysfs_streq(buf, "online"))
		memhp_auto_online = true;
	else if (sysfs_streq(buf, "offline"))
		memhp_auto_online = false;
	else
		return -EINVAL;

	return count;
}

static DEVICE_ATTR(auto_online_blocks, 0644, show_auto_online_blocks,
		   store_auto_online_blocks);
=======
static ssize_t auto_online_blocks_show(struct device *dev,
				       struct device_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%s\n",
			  online_type_to_str[mhp_default_online_type]);
}

static ssize_t auto_online_blocks_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	const int online_type = mhp_online_type_from_str(buf);

	if (online_type < 0)
		return -EINVAL;

	mhp_default_online_type = online_type;
	return count;
}

static DEVICE_ATTR_RW(auto_online_blocks);
>>>>>>> upstream/android-13

/*
 * Some architectures will have custom drivers to do this, and
 * will not need to do it from userspace.  The fake hot-add code
 * as well as ppc64 will do all of their discovery in userspace
 * and will require this interface.
 */
#ifdef CONFIG_ARCH_MEMORY_PROBE
<<<<<<< HEAD
static ssize_t
memory_probe_store(struct device *dev, struct device_attribute *attr,
		   const char *buf, size_t count)
=======
static ssize_t probe_store(struct device *dev, struct device_attribute *attr,
			   const char *buf, size_t count)
>>>>>>> upstream/android-13
{
	u64 phys_addr;
	int nid, ret;
	unsigned long pages_per_block = PAGES_PER_SECTION * sections_per_block;

	ret = kstrtoull(buf, 0, &phys_addr);
	if (ret)
		return ret;

	if (phys_addr & ((pages_per_block << PAGE_SHIFT) - 1))
		return -EINVAL;

	ret = lock_device_hotplug_sysfs();
	if (ret)
		return ret;

	nid = memory_add_physaddr_to_nid(phys_addr);
	ret = __add_memory(nid, phys_addr,
<<<<<<< HEAD
			   MIN_MEMORY_BLOCK_SIZE * sections_per_block);
=======
			   MIN_MEMORY_BLOCK_SIZE * sections_per_block,
			   MHP_NONE);
>>>>>>> upstream/android-13

	if (ret)
		goto out;

	ret = count;
out:
	unlock_device_hotplug();
	return ret;
}

<<<<<<< HEAD
static DEVICE_ATTR(probe, S_IWUSR, NULL, memory_probe_store);
=======
static DEVICE_ATTR_WO(probe);
>>>>>>> upstream/android-13
#endif

#ifdef CONFIG_MEMORY_FAILURE
/*
 * Support for offlining pages of memory
 */

/* Soft offline a page */
<<<<<<< HEAD
static ssize_t
store_soft_offline_page(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t count)
=======
static ssize_t soft_offline_page_store(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf, size_t count)
>>>>>>> upstream/android-13
{
	int ret;
	u64 pfn;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (kstrtoull(buf, 0, &pfn) < 0)
		return -EINVAL;
	pfn >>= PAGE_SHIFT;
<<<<<<< HEAD
	if (!pfn_valid(pfn))
		return -ENXIO;
	/* Only online pages can be soft-offlined (esp., not ZONE_DEVICE). */
	if (!pfn_to_online_page(pfn))
		return -EIO;
	ret = soft_offline_page(pfn_to_page(pfn), 0);
=======
	ret = soft_offline_page(pfn, 0);
>>>>>>> upstream/android-13
	return ret == 0 ? count : ret;
}

/* Forcibly offline a page, including killing processes. */
<<<<<<< HEAD
static ssize_t
store_hard_offline_page(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t count)
=======
static ssize_t hard_offline_page_store(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf, size_t count)
>>>>>>> upstream/android-13
{
	int ret;
	u64 pfn;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (kstrtoull(buf, 0, &pfn) < 0)
		return -EINVAL;
	pfn >>= PAGE_SHIFT;
	ret = memory_failure(pfn, 0);
	return ret ? ret : count;
}

<<<<<<< HEAD
static DEVICE_ATTR(soft_offline_page, S_IWUSR, NULL, store_soft_offline_page);
static DEVICE_ATTR(hard_offline_page, S_IWUSR, NULL, store_hard_offline_page);
#endif

/*
 * Note that phys_device is optional.  It is here to allow for
 * differentiation between which *physical* devices each
 * section belongs to...
 */
=======
static DEVICE_ATTR_WO(soft_offline_page);
static DEVICE_ATTR_WO(hard_offline_page);
#endif

/* See phys_device_show(). */
>>>>>>> upstream/android-13
int __weak arch_get_memory_phys_device(unsigned long start_pfn)
{
	return 0;
}

/*
<<<<<<< HEAD
 * A reference for the returned object is held and the reference for the
 * hinted object is released.
 */
static struct memory_block *find_memory_block_by_id(int block_id,
						    struct memory_block *hint)
{
	struct device *hintdev = hint ? &hint->dev : NULL;
	struct device *dev;

	dev = subsys_find_device_by_id(&memory_subsys, block_id, hintdev);
	if (hint)
		put_device(&hint->dev);
	if (!dev)
		return NULL;
	return to_memory_block(dev);
}

struct memory_block *find_memory_block_hinted(struct mem_section *section,
					      struct memory_block *hint)
{
	int block_id = base_memory_block_id(__section_nr(section));

	return find_memory_block_by_id(block_id, hint);
}

/*
 * For now, we have a linear search to go find the appropriate
 * memory_block corresponding to a particular phys_index. If
 * this gets to be a real problem, we can always use a radix
 * tree or something here.
 *
 * This could be made generic for all device subsystems.
 */
struct memory_block *find_memory_block(struct mem_section *section)
{
	return find_memory_block_hinted(section, NULL);
=======
 * A reference for the returned memory block device is acquired.
 *
 * Called under device_hotplug_lock.
 */
static struct memory_block *find_memory_block_by_id(unsigned long block_id)
{
	struct memory_block *mem;

	mem = xa_load(&memory_blocks, block_id);
	if (mem)
		get_device(&mem->dev);
	return mem;
}

/*
 * Called under device_hotplug_lock.
 */
struct memory_block *find_memory_block(unsigned long section_nr)
{
	unsigned long block_id = memory_block_id(section_nr);

	return find_memory_block_by_id(block_id);
>>>>>>> upstream/android-13
}

static struct attribute *memory_memblk_attrs[] = {
	&dev_attr_phys_index.attr,
	&dev_attr_state.attr,
	&dev_attr_phys_device.attr,
	&dev_attr_removable.attr,
#ifdef CONFIG_MEMORY_HOTREMOVE
	&dev_attr_valid_zones.attr,
#endif
	NULL
};

<<<<<<< HEAD
static struct attribute_group memory_memblk_attr_group = {
=======
static const struct attribute_group memory_memblk_attr_group = {
>>>>>>> upstream/android-13
	.attrs = memory_memblk_attrs,
};

static const struct attribute_group *memory_memblk_attr_groups[] = {
	&memory_memblk_attr_group,
	NULL,
};

/*
 * register_memory - Setup a sysfs device for a memory block
 */
static
int register_memory(struct memory_block *memory)
{
	int ret;

	memory->dev.bus = &memory_subsys;
	memory->dev.id = memory->start_section_nr / sections_per_block;
	memory->dev.release = memory_block_release;
	memory->dev.groups = memory_memblk_attr_groups;
	memory->dev.offline = memory->state == MEM_OFFLINE;

	ret = device_register(&memory->dev);
<<<<<<< HEAD
	if (ret)
		put_device(&memory->dev);

	return ret;
}

static int init_memory_block(struct memory_block **memory, int block_id,
			     unsigned long state)
{
	struct memory_block *mem;
	unsigned long start_pfn;
	int ret = 0;

	mem = find_memory_block_by_id(block_id, NULL);
=======
	if (ret) {
		put_device(&memory->dev);
		return ret;
	}
	ret = xa_err(xa_store(&memory_blocks, memory->dev.id, memory,
			      GFP_KERNEL));
	if (ret) {
		put_device(&memory->dev);
		device_unregister(&memory->dev);
	}
	return ret;
}

static int init_memory_block(unsigned long block_id, unsigned long state,
			     unsigned long nr_vmemmap_pages,
			     struct memory_group *group)
{
	struct memory_block *mem;
	int ret = 0;

	mem = find_memory_block_by_id(block_id);
>>>>>>> upstream/android-13
	if (mem) {
		put_device(&mem->dev);
		return -EEXIST;
	}
	mem = kzalloc(sizeof(*mem), GFP_KERNEL);
	if (!mem)
		return -ENOMEM;

	mem->start_section_nr = block_id * sections_per_block;
<<<<<<< HEAD
	mem->end_section_nr = mem->start_section_nr + sections_per_block - 1;
	mem->state = state;
	start_pfn = section_nr_to_pfn(mem->start_section_nr);
	mem->phys_device = arch_get_memory_phys_device(start_pfn);
	mem->nid = NUMA_NO_NODE;

	ret = register_memory(mem);

	*memory = mem;
	return ret;
}

static int add_memory_block(int base_section_nr)
{
	struct memory_block *mem;
	int i, ret, section_count = 0;

	for (i = base_section_nr;
	     i < base_section_nr + sections_per_block;
	     i++)
		if (present_section_nr(i))
=======
	mem->state = state;
	mem->nid = NUMA_NO_NODE;
	mem->nr_vmemmap_pages = nr_vmemmap_pages;
	INIT_LIST_HEAD(&mem->group_next);

	ret = register_memory(mem);
	if (ret)
		return ret;

	if (group) {
		mem->group = group;
		list_add(&mem->group_next, &group->memory_blocks);
	}

	return 0;
}

static int add_memory_block(unsigned long base_section_nr)
{
	int section_count = 0;
	unsigned long nr;

	for (nr = base_section_nr; nr < base_section_nr + sections_per_block;
	     nr++)
		if (present_section_nr(nr))
>>>>>>> upstream/android-13
			section_count++;

	if (section_count == 0)
		return 0;
<<<<<<< HEAD
	ret = init_memory_block(&mem, base_memory_block_id(base_section_nr),
				MEM_ONLINE);
	if (ret)
		return ret;
	mem->section_count = section_count;
	return 0;
=======
	return init_memory_block(memory_block_id(base_section_nr),
				 MEM_ONLINE, 0,  NULL);
>>>>>>> upstream/android-13
}

static void unregister_memory(struct memory_block *memory)
{
	if (WARN_ON_ONCE(memory->dev.bus != &memory_subsys))
		return;

<<<<<<< HEAD
=======
	WARN_ON(xa_erase(&memory_blocks, memory->dev.id) == NULL);

	if (memory->group) {
		list_del(&memory->group_next);
		memory->group = NULL;
	}

>>>>>>> upstream/android-13
	/* drop the ref. we got via find_memory_block() */
	put_device(&memory->dev);
	device_unregister(&memory->dev);
}

/*
 * Create memory block devices for the given memory area. Start and size
 * have to be aligned to memory block granularity. Memory block devices
 * will be initialized as offline.
<<<<<<< HEAD
 */
int create_memory_block_devices(unsigned long start, unsigned long size)
{
	const int start_block_id = pfn_to_block_id(PFN_DOWN(start));
	int end_block_id = pfn_to_block_id(PFN_DOWN(start + size));
=======
 *
 * Called under device_hotplug_lock.
 */
int create_memory_block_devices(unsigned long start, unsigned long size,
				unsigned long vmemmap_pages,
				struct memory_group *group)
{
	const unsigned long start_block_id = pfn_to_block_id(PFN_DOWN(start));
	unsigned long end_block_id = pfn_to_block_id(PFN_DOWN(start + size));
>>>>>>> upstream/android-13
	struct memory_block *mem;
	unsigned long block_id;
	int ret = 0;

	if (WARN_ON_ONCE(!IS_ALIGNED(start, memory_block_size_bytes()) ||
			 !IS_ALIGNED(size, memory_block_size_bytes())))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&mem_sysfs_mutex);
	for (block_id = start_block_id; block_id != end_block_id; block_id++) {
		ret = init_memory_block(&mem, block_id, MEM_OFFLINE);
		if (ret)
			break;
		mem->section_count = sections_per_block;
=======
	for (block_id = start_block_id; block_id != end_block_id; block_id++) {
		ret = init_memory_block(block_id, MEM_OFFLINE, vmemmap_pages,
					group);
		if (ret)
			break;
>>>>>>> upstream/android-13
	}
	if (ret) {
		end_block_id = block_id;
		for (block_id = start_block_id; block_id != end_block_id;
		     block_id++) {
<<<<<<< HEAD
			mem = find_memory_block_by_id(block_id, NULL);
			mem->section_count = 0;
			unregister_memory(mem);
		}
	}
	mutex_unlock(&mem_sysfs_mutex);
=======
			mem = find_memory_block_by_id(block_id);
			if (WARN_ON_ONCE(!mem))
				continue;
			unregister_memory(mem);
		}
	}
>>>>>>> upstream/android-13
	return ret;
}

/*
 * Remove memory block devices for the given memory area. Start and size
 * have to be aligned to memory block granularity. Memory block devices
 * have to be offline.
<<<<<<< HEAD
 */
void remove_memory_block_devices(unsigned long start, unsigned long size)
{
	const int start_block_id = pfn_to_block_id(PFN_DOWN(start));
	const int end_block_id = pfn_to_block_id(PFN_DOWN(start + size));
	struct memory_block *mem;
	int block_id;
=======
 *
 * Called under device_hotplug_lock.
 */
void remove_memory_block_devices(unsigned long start, unsigned long size)
{
	const unsigned long start_block_id = pfn_to_block_id(PFN_DOWN(start));
	const unsigned long end_block_id = pfn_to_block_id(PFN_DOWN(start + size));
	struct memory_block *mem;
	unsigned long block_id;
>>>>>>> upstream/android-13

	if (WARN_ON_ONCE(!IS_ALIGNED(start, memory_block_size_bytes()) ||
			 !IS_ALIGNED(size, memory_block_size_bytes())))
		return;

<<<<<<< HEAD
	mutex_lock(&mem_sysfs_mutex);
	for (block_id = start_block_id; block_id != end_block_id; block_id++) {
		mem = find_memory_block_by_id(block_id, NULL);
		if (WARN_ON_ONCE(!mem))
			continue;
		mem->section_count = 0;
		unregister_memory_block_under_nodes(mem);
		unregister_memory(mem);
	}
	mutex_unlock(&mem_sysfs_mutex);
=======
	for (block_id = start_block_id; block_id != end_block_id; block_id++) {
		mem = find_memory_block_by_id(block_id);
		if (WARN_ON_ONCE(!mem))
			continue;
		unregister_memory_block_under_nodes(mem);
		unregister_memory(mem);
	}
>>>>>>> upstream/android-13
}

/* return true if the memory block is offlined, otherwise, return false */
bool is_memblock_offlined(struct memory_block *mem)
{
	return mem->state == MEM_OFFLINE;
}

static struct attribute *memory_root_attrs[] = {
#ifdef CONFIG_ARCH_MEMORY_PROBE
	&dev_attr_probe.attr,
#endif

#ifdef CONFIG_MEMORY_FAILURE
	&dev_attr_soft_offline_page.attr,
	&dev_attr_hard_offline_page.attr,
#endif

	&dev_attr_block_size_bytes.attr,
	&dev_attr_auto_online_blocks.attr,
	NULL
};

<<<<<<< HEAD
static struct attribute_group memory_root_attr_group = {
=======
static const struct attribute_group memory_root_attr_group = {
>>>>>>> upstream/android-13
	.attrs = memory_root_attrs,
};

static const struct attribute_group *memory_root_attr_groups[] = {
	&memory_root_attr_group,
	NULL,
};

/*
<<<<<<< HEAD
 * Initialize the sysfs support for memory devices...
 */
int __init memory_dev_init(void)
{
	unsigned int i;
	int ret;
	int err;
	unsigned long block_sz;

	ret = subsys_system_register(&memory_subsys, memory_root_attr_groups);
	if (ret)
		goto out;

	block_sz = get_memory_block_size();
	sections_per_block = block_sz / MIN_MEMORY_BLOCK_SIZE;
=======
 * Initialize the sysfs support for memory devices. At the time this function
 * is called, we cannot have concurrent creation/deletion of memory block
 * devices, the device_hotplug_lock is not needed.
 */
void __init memory_dev_init(void)
{
	int ret;
	unsigned long block_sz, nr;

	/* Validate the configured memory block size */
	block_sz = memory_block_size_bytes();
	if (!is_power_of_2(block_sz) || block_sz < MIN_MEMORY_BLOCK_SIZE)
		panic("Memory block size not suitable: 0x%lx\n", block_sz);
	sections_per_block = block_sz / MIN_MEMORY_BLOCK_SIZE;

	ret = subsys_system_register(&memory_subsys, memory_root_attr_groups);
	if (ret)
		panic("%s() failed to register subsystem: %d\n", __func__, ret);
>>>>>>> upstream/android-13

	/*
	 * Create entries for memory sections that were found
	 * during boot and have been initialized
	 */
<<<<<<< HEAD
	mutex_lock(&mem_sysfs_mutex);
	for (i = 0; i <= __highest_present_section_nr;
		i += sections_per_block) {
		err = add_memory_block(i);
		if (!ret)
			ret = err;
	}
	mutex_unlock(&mem_sysfs_mutex);

out:
	if (ret)
		printk(KERN_ERR "%s() failed: %d\n", __func__, ret);
=======
	for (nr = 0; nr <= __highest_present_section_nr;
	     nr += sections_per_block) {
		ret = add_memory_block(nr);
		if (ret)
			panic("%s() failed to add memory block: %d\n", __func__,
			      ret);
	}
}

/**
 * walk_memory_blocks - walk through all present memory blocks overlapped
 *			by the range [start, start + size)
 *
 * @start: start address of the memory range
 * @size: size of the memory range
 * @arg: argument passed to func
 * @func: callback for each memory section walked
 *
 * This function walks through all present memory blocks overlapped by the
 * range [start, start + size), calling func on each memory block.
 *
 * In case func() returns an error, walking is aborted and the error is
 * returned.
 *
 * Called under device_hotplug_lock.
 */
int walk_memory_blocks(unsigned long start, unsigned long size,
		       void *arg, walk_memory_blocks_func_t func)
{
	const unsigned long start_block_id = phys_to_block_id(start);
	const unsigned long end_block_id = phys_to_block_id(start + size - 1);
	struct memory_block *mem;
	unsigned long block_id;
	int ret = 0;

	if (!size)
		return 0;

	for (block_id = start_block_id; block_id <= end_block_id; block_id++) {
		mem = find_memory_block_by_id(block_id);
		if (!mem)
			continue;

		ret = func(mem, arg);
		put_device(&mem->dev);
		if (ret)
			break;
	}
>>>>>>> upstream/android-13
	return ret;
}

struct for_each_memory_block_cb_data {
	walk_memory_blocks_func_t func;
	void *arg;
};

static int for_each_memory_block_cb(struct device *dev, void *data)
{
	struct memory_block *mem = to_memory_block(dev);
	struct for_each_memory_block_cb_data *cb_data = data;

	return cb_data->func(mem, cb_data->arg);
}

/**
 * for_each_memory_block - walk through all present memory blocks
 *
 * @arg: argument passed to func
 * @func: callback for each memory block walked
 *
 * This function walks through all present memory blocks, calling func on
 * each memory block.
 *
 * In case func() returns an error, walking is aborted and the error is
 * returned.
 */
int for_each_memory_block(void *arg, walk_memory_blocks_func_t func)
{
	struct for_each_memory_block_cb_data cb_data = {
		.func = func,
		.arg = arg,
	};

	return bus_for_each_dev(&memory_subsys, NULL, &cb_data,
				for_each_memory_block_cb);
}
<<<<<<< HEAD
=======

/*
 * This is an internal helper to unify allocation and initialization of
 * memory groups. Note that the passed memory group will be copied to a
 * dynamically allocated memory group. After this call, the passed
 * memory group should no longer be used.
 */
static int memory_group_register(struct memory_group group)
{
	struct memory_group *new_group;
	uint32_t mgid;
	int ret;

	if (!node_possible(group.nid))
		return -EINVAL;

	new_group = kzalloc(sizeof(group), GFP_KERNEL);
	if (!new_group)
		return -ENOMEM;
	*new_group = group;
	INIT_LIST_HEAD(&new_group->memory_blocks);

	ret = xa_alloc(&memory_groups, &mgid, new_group, xa_limit_31b,
		       GFP_KERNEL);
	if (ret) {
		kfree(new_group);
		return ret;
	} else if (group.is_dynamic) {
		xa_set_mark(&memory_groups, mgid, MEMORY_GROUP_MARK_DYNAMIC);
	}
	return mgid;
}

/**
 * memory_group_register_static() - Register a static memory group.
 * @nid: The node id.
 * @max_pages: The maximum number of pages we'll have in this static memory
 *	       group.
 *
 * Register a new static memory group and return the memory group id.
 * All memory in the group belongs to a single unit, such as a DIMM. All
 * memory belonging to a static memory group is added in one go to be removed
 * in one go -- it's static.
 *
 * Returns an error if out of memory, if the node id is invalid, if no new
 * memory groups can be registered, or if max_pages is invalid (0). Otherwise,
 * returns the new memory group id.
 */
int memory_group_register_static(int nid, unsigned long max_pages)
{
	struct memory_group group = {
		.nid = nid,
		.s = {
			.max_pages = max_pages,
		},
	};

	if (!max_pages)
		return -EINVAL;
	return memory_group_register(group);
}
EXPORT_SYMBOL_GPL(memory_group_register_static);

/**
 * memory_group_register_dynamic() - Register a dynamic memory group.
 * @nid: The node id.
 * @unit_pages: Unit in pages in which is memory added/removed in this dynamic
 *		memory group.
 *
 * Register a new dynamic memory group and return the memory group id.
 * Memory within a dynamic memory group is added/removed dynamically
 * in unit_pages.
 *
 * Returns an error if out of memory, if the node id is invalid, if no new
 * memory groups can be registered, or if unit_pages is invalid (0, not a
 * power of two, smaller than a single memory block). Otherwise, returns the
 * new memory group id.
 */
int memory_group_register_dynamic(int nid, unsigned long unit_pages)
{
	struct memory_group group = {
		.nid = nid,
		.is_dynamic = true,
		.d = {
			.unit_pages = unit_pages,
		},
	};

	if (!unit_pages || !is_power_of_2(unit_pages) ||
	    unit_pages < PHYS_PFN(memory_block_size_bytes()))
		return -EINVAL;
	return memory_group_register(group);
}
EXPORT_SYMBOL_GPL(memory_group_register_dynamic);

/**
 * memory_group_unregister() - Unregister a memory group.
 * @mgid: the memory group id
 *
 * Unregister a memory group. If any memory block still belongs to this
 * memory group, unregistering will fail.
 *
 * Returns -EINVAL if the memory group id is invalid, returns -EBUSY if some
 * memory blocks still belong to this memory group and returns 0 if
 * unregistering succeeded.
 */
int memory_group_unregister(int mgid)
{
	struct memory_group *group;

	if (mgid < 0)
		return -EINVAL;

	group = xa_load(&memory_groups, mgid);
	if (!group)
		return -EINVAL;
	if (!list_empty(&group->memory_blocks))
		return -EBUSY;
	xa_erase(&memory_groups, mgid);
	kfree(group);
	return 0;
}
EXPORT_SYMBOL_GPL(memory_group_unregister);

/*
 * This is an internal helper only to be used in core memory hotplug code to
 * lookup a memory group. We don't care about locking, as we don't expect a
 * memory group to get unregistered while adding memory to it -- because
 * the group and the memory is managed by the same driver.
 */
struct memory_group *memory_group_find_by_id(int mgid)
{
	return xa_load(&memory_groups, mgid);
}

/*
 * This is an internal helper only to be used in core memory hotplug code to
 * walk all dynamic memory groups excluding a given memory group, either
 * belonging to a specific node, or belonging to any node.
 */
int walk_dynamic_memory_groups(int nid, walk_memory_groups_func_t func,
			       struct memory_group *excluded, void *arg)
{
	struct memory_group *group;
	unsigned long index;
	int ret = 0;

	xa_for_each_marked(&memory_groups, index, group,
			   MEMORY_GROUP_MARK_DYNAMIC) {
		if (group == excluded)
			continue;
#ifdef CONFIG_NUMA
		if (nid != NUMA_NO_NODE && group->nid != nid)
			continue;
#endif /* CONFIG_NUMA */
		ret = func(group, arg);
		if (ret)
			break;
	}
	return ret;
}
>>>>>>> upstream/android-13
