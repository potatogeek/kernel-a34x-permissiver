// SPDX-License-Identifier: GPL-2.0
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_iommu.h>
<<<<<<< HEAD
#include <linux/dma-mapping.h>
=======
#include <linux/of_reserved_mem.h>
#include <linux/dma-direct.h> /* for bus_dma_region */
#include <linux/dma-map-ops.h>
>>>>>>> upstream/android-13
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/slab.h>
#include <linux/platform_device.h>

#include <asm/errno.h>
#include "of_private.h"

/**
 * of_match_device - Tell if a struct device matches an of_device_id list
<<<<<<< HEAD
 * @ids: array of of device match structures to search in
=======
 * @matches: array of of device match structures to search in
>>>>>>> upstream/android-13
 * @dev: the of device structure to match against
 *
 * Used by a driver to check whether an platform_device present in the
 * system is in its list of supported devices.
 */
const struct of_device_id *of_match_device(const struct of_device_id *matches,
					   const struct device *dev)
{
	if ((!matches) || (!dev->of_node))
		return NULL;
	return of_match_node(matches, dev->of_node);
}
EXPORT_SYMBOL(of_match_device);

<<<<<<< HEAD
struct platform_device *of_dev_get(struct platform_device *dev)
{
	struct device *tmp;

	if (!dev)
		return NULL;
	tmp = get_device(&dev->dev);
	if (tmp)
		return to_platform_device(tmp);
	else
		return NULL;
}
EXPORT_SYMBOL(of_dev_get);

void of_dev_put(struct platform_device *dev)
{
	if (dev)
		put_device(&dev->dev);
}
EXPORT_SYMBOL(of_dev_put);

=======
>>>>>>> upstream/android-13
int of_device_add(struct platform_device *ofdev)
{
	BUG_ON(ofdev->dev.of_node == NULL);

	/* name and id have to be set so that the platform bus doesn't get
	 * confused on matching */
	ofdev->name = dev_name(&ofdev->dev);
	ofdev->id = PLATFORM_DEVID_NONE;

	/*
	 * If this device has not binding numa node in devicetree, that is
	 * of_node_to_nid returns NUMA_NO_NODE. device_add will assume that this
	 * device is on the same node as the parent.
	 */
	set_dev_node(&ofdev->dev, of_node_to_nid(ofdev->dev.of_node));

	return device_add(&ofdev->dev);
}

<<<<<<< HEAD
/**
 * of_dma_configure - Setup DMA configuration
=======
static void
of_dma_set_restricted_buffer(struct device *dev, struct device_node *np)
{
	struct device_node *node, *of_node = dev->of_node;
	int count, i;

	if (!IS_ENABLED(CONFIG_DMA_RESTRICTED_POOL))
		return;

	count = of_property_count_elems_of_size(of_node, "memory-region",
						sizeof(u32));
	/*
	 * If dev->of_node doesn't exist or doesn't contain memory-region, try
	 * the OF node having DMA configuration.
	 */
	if (count <= 0) {
		of_node = np;
		count = of_property_count_elems_of_size(
			of_node, "memory-region", sizeof(u32));
	}

	for (i = 0; i < count; i++) {
		node = of_parse_phandle(of_node, "memory-region", i);
		/*
		 * There might be multiple memory regions, but only one
		 * restricted-dma-pool region is allowed.
		 */
		if (of_device_is_compatible(node, "restricted-dma-pool") &&
		    of_device_is_available(node))
			break;
	}

	/*
	 * Attempt to initialize a restricted-dma-pool region if one was found.
	 * Note that count can hold a negative error code.
	 */
	if (i < count && of_reserved_mem_device_init_by_idx(dev, of_node, i))
		dev_warn(dev, "failed to initialise \"restricted-dma-pool\" memory node\n");
}

/**
 * of_dma_configure_id - Setup DMA configuration
>>>>>>> upstream/android-13
 * @dev:	Device to apply DMA configuration
 * @np:		Pointer to OF node having DMA configuration
 * @force_dma:  Whether device is to be set up by of_dma_configure() even if
 *		DMA capability is not explicitly described by firmware.
<<<<<<< HEAD
=======
 * @id:		Optional const pointer value input id
>>>>>>> upstream/android-13
 *
 * Try to get devices's DMA configuration from DT and update it
 * accordingly.
 *
 * If platform code needs to use its own special DMA configuration, it
 * can use a platform bus notifier and handle BUS_NOTIFY_ADD_DEVICE events
 * to fix up DMA configuration.
 */
<<<<<<< HEAD
int of_dma_configure(struct device *dev, struct device_node *np, bool force_dma)
{
	u64 dma_addr, paddr, size = 0;
	int ret;
	bool coherent;
	unsigned long offset;
	const struct iommu_ops *iommu;
	u64 mask;

	ret = of_dma_get_range(np, &dma_addr, &paddr, &size);
=======
int of_dma_configure_id(struct device *dev, struct device_node *np,
			bool force_dma, const u32 *id)
{
	const struct iommu_ops *iommu;
	const struct bus_dma_region *map = NULL;
	u64 dma_start = 0;
	u64 mask, end, size = 0;
	bool coherent;
	int ret;

	ret = of_dma_get_range(np, &map);
>>>>>>> upstream/android-13
	if (ret < 0) {
		/*
		 * For legacy reasons, we have to assume some devices need
		 * DMA configuration regardless of whether "dma-ranges" is
		 * correctly specified or not.
		 */
		if (!force_dma)
			return ret == -ENODEV ? 0 : ret;
<<<<<<< HEAD

		dma_addr = offset = 0;
	} else {
		offset = PFN_DOWN(paddr - dma_addr);
=======
	} else {
		const struct bus_dma_region *r = map;
		u64 dma_end = 0;

		/* Determine the overall bounds of all DMA regions */
		for (dma_start = ~0; r->size; r++) {
			/* Take lower and upper limits */
			if (r->dma_start < dma_start)
				dma_start = r->dma_start;
			if (r->dma_start + r->size > dma_end)
				dma_end = r->dma_start + r->size;
		}
		size = dma_end - dma_start;
>>>>>>> upstream/android-13

		/*
		 * Add a work around to treat the size as mask + 1 in case
		 * it is defined in DT as a mask.
		 */
		if (size & 1) {
<<<<<<< HEAD
			dev_warn(dev, "Invalid size 0x%llx for dma-range\n",
=======
			dev_warn(dev, "Invalid size 0x%llx for dma-range(s)\n",
>>>>>>> upstream/android-13
				 size);
			size = size + 1;
		}

		if (!size) {
			dev_err(dev, "Adjusted size 0x%llx invalid\n", size);
<<<<<<< HEAD
			return -EINVAL;
		}
		dev_dbg(dev, "dma_pfn_offset(%#08lx)\n", offset);
=======
			kfree(map);
			return -EINVAL;
		}
>>>>>>> upstream/android-13
	}

	/*
	 * If @dev is expected to be DMA-capable then the bus code that created
	 * it should have initialised its dma_mask pointer by this point. For
	 * now, we'll continue the legacy behaviour of coercing it to the
	 * coherent mask if not, but we'll no longer do so quietly.
	 */
	if (!dev->dma_mask) {
		dev_warn(dev, "DMA mask not set\n");
		dev->dma_mask = &dev->coherent_dma_mask;
	}

	if (!size && dev->coherent_dma_mask)
		size = max(dev->coherent_dma_mask, dev->coherent_dma_mask + 1);
	else if (!size)
		size = 1ULL << 32;

<<<<<<< HEAD
	dev->dma_pfn_offset = offset;

=======
>>>>>>> upstream/android-13
	/*
	 * Limit coherent and dma mask based on size and default mask
	 * set by the driver.
	 */
<<<<<<< HEAD
	mask = DMA_BIT_MASK(ilog2(dma_addr + size - 1) + 1);
	dev->coherent_dma_mask &= mask;
	*dev->dma_mask &= mask;
	/* ...but only set bus mask if we found valid dma-ranges earlier */
	if (!ret)
		dev->bus_dma_mask = mask;
=======
	end = dma_start + size - 1;
	mask = DMA_BIT_MASK(ilog2(end) + 1);
	dev->coherent_dma_mask &= mask;
	*dev->dma_mask &= mask;
	/* ...but only set bus limit and range map if we found valid dma-ranges earlier */
	if (!ret) {
		dev->bus_dma_limit = end;
		dev->dma_range_map = map;
	}
>>>>>>> upstream/android-13

	coherent = of_dma_is_coherent(np);
	dev_dbg(dev, "device is%sdma coherent\n",
		coherent ? " " : " not ");

<<<<<<< HEAD
	iommu = of_iommu_configure(dev, np);
	if (IS_ERR(iommu) && PTR_ERR(iommu) == -EPROBE_DEFER)
		return -EPROBE_DEFER;
=======
	iommu = of_iommu_configure(dev, np, id);
	if (PTR_ERR(iommu) == -EPROBE_DEFER) {
		/* Don't touch range map if it wasn't set from a valid dma-ranges */
		if (!ret)
			dev->dma_range_map = NULL;
		kfree(map);
		return -EPROBE_DEFER;
	}
>>>>>>> upstream/android-13

	dev_dbg(dev, "device is%sbehind an iommu\n",
		iommu ? " " : " not ");

<<<<<<< HEAD
	arch_setup_dma_ops(dev, dma_addr, size, iommu, coherent);

	return 0;
}
EXPORT_SYMBOL_GPL(of_dma_configure);

/**
 * of_dma_deconfigure - Clean up DMA configuration
 * @dev:	Device for which to clean up DMA configuration
 *
 * Clean up all configuration performed by of_dma_configure_ops() and free all
 * resources that have been allocated.
 */
void of_dma_deconfigure(struct device *dev)
{
	arch_teardown_dma_ops(dev);
}
=======
	arch_setup_dma_ops(dev, dma_start, size, iommu, coherent);

	if (!iommu)
		of_dma_set_restricted_buffer(dev, np);

	return 0;
}
EXPORT_SYMBOL_GPL(of_dma_configure_id);
>>>>>>> upstream/android-13

int of_device_register(struct platform_device *pdev)
{
	device_initialize(&pdev->dev);
	return of_device_add(pdev);
}
EXPORT_SYMBOL(of_device_register);

void of_device_unregister(struct platform_device *ofdev)
{
	device_unregister(&ofdev->dev);
}
EXPORT_SYMBOL(of_device_unregister);

const void *of_device_get_match_data(const struct device *dev)
{
	const struct of_device_id *match;

	match = of_match_device(dev->driver->of_match_table, dev);
	if (!match)
		return NULL;

	return match->data;
}
EXPORT_SYMBOL(of_device_get_match_data);

static ssize_t of_device_get_modalias(struct device *dev, char *str, ssize_t len)
{
	const char *compat;
	char *c;
	struct property *p;
	ssize_t csize;
	ssize_t tsize;

	if ((!dev) || (!dev->of_node))
		return -ENODEV;

	/* Name & Type */
	/* %p eats all alphanum characters, so %c must be used here */
	csize = snprintf(str, len, "of:N%pOFn%c%s", dev->of_node, 'T',
<<<<<<< HEAD
			 dev->of_node->type);
=======
			 of_node_get_device_type(dev->of_node));
>>>>>>> upstream/android-13
	tsize = csize;
	len -= csize;
	if (str)
		str += csize;

	of_property_for_each_string(dev->of_node, "compatible", p, compat) {
		csize = strlen(compat) + 1;
		tsize += csize;
		if (csize > len)
			continue;

		csize = snprintf(str, len, "C%s", compat);
		for (c = str; c; ) {
			c = strchr(c, ' ');
			if (c)
				*c++ = '_';
		}
		len -= csize;
		str += csize;
	}

	return tsize;
}

int of_device_request_module(struct device *dev)
{
	char *str;
	ssize_t size;
	int ret;

	size = of_device_get_modalias(dev, NULL, 0);
	if (size < 0)
		return size;

	str = kmalloc(size + 1, GFP_KERNEL);
	if (!str)
		return -ENOMEM;

	of_device_get_modalias(dev, str, size);
	str[size] = '\0';
	ret = request_module(str);
	kfree(str);

	return ret;
}
EXPORT_SYMBOL_GPL(of_device_request_module);

/**
 * of_device_modalias - Fill buffer with newline terminated modalias string
<<<<<<< HEAD
=======
 * @dev:	Calling device
 * @str:	Modalias string
 * @len:	Size of @str
>>>>>>> upstream/android-13
 */
ssize_t of_device_modalias(struct device *dev, char *str, ssize_t len)
{
	ssize_t sl = of_device_get_modalias(dev, str, len - 2);
	if (sl < 0)
		return sl;
	if (sl > len - 2)
		return -ENOMEM;

	str[sl++] = '\n';
	str[sl] = 0;
	return sl;
}
EXPORT_SYMBOL_GPL(of_device_modalias);

/**
 * of_device_uevent - Display OF related uevent information
<<<<<<< HEAD
 */
void of_device_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	const char *compat;
=======
 * @dev:	Device to apply DMA configuration
 * @env:	Kernel object's userspace event reference
 */
void of_device_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	const char *compat, *type;
>>>>>>> upstream/android-13
	struct alias_prop *app;
	struct property *p;
	int seen = 0;

	if ((!dev) || (!dev->of_node))
		return;

	add_uevent_var(env, "OF_NAME=%pOFn", dev->of_node);
	add_uevent_var(env, "OF_FULLNAME=%pOF", dev->of_node);
<<<<<<< HEAD
	if (dev->of_node->type && strcmp("<NULL>", dev->of_node->type) != 0)
		add_uevent_var(env, "OF_TYPE=%s", dev->of_node->type);
=======
	type = of_node_get_device_type(dev->of_node);
	if (type)
		add_uevent_var(env, "OF_TYPE=%s", type);
>>>>>>> upstream/android-13

	/* Since the compatible field can contain pretty much anything
	 * it's not really legal to split it out with commas. We split it
	 * up using a number of environment variables instead. */
	of_property_for_each_string(dev->of_node, "compatible", p, compat) {
		add_uevent_var(env, "OF_COMPATIBLE_%d=%s", seen, compat);
		seen++;
	}
	add_uevent_var(env, "OF_COMPATIBLE_N=%d", seen);

	seen = 0;
	mutex_lock(&of_mutex);
	list_for_each_entry(app, &aliases_lookup, link) {
		if (dev->of_node == app->np) {
			add_uevent_var(env, "OF_ALIAS_%d=%s", seen,
				       app->alias);
			seen++;
		}
	}
	mutex_unlock(&of_mutex);
}

int of_device_uevent_modalias(struct device *dev, struct kobj_uevent_env *env)
{
	int sl;

	if ((!dev) || (!dev->of_node))
		return -ENODEV;

	/* Devicetree modalias is tricky, we add it in 2 steps */
	if (add_uevent_var(env, "MODALIAS="))
		return -ENOMEM;

	sl = of_device_get_modalias(dev, &env->buf[env->buflen-1],
				    sizeof(env->buf) - env->buflen);
	if (sl >= (sizeof(env->buf) - env->buflen))
		return -ENOMEM;
	env->buflen += sl;

	return 0;
}
EXPORT_SYMBOL_GPL(of_device_uevent_modalias);
