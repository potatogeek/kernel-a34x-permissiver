<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * OF helpers for IOMMU
 *
 * Copyright (c) 2012, NVIDIA CORPORATION.  All rights reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
=======
>>>>>>> upstream/android-13
 */

#include <linux/export.h>
#include <linux/iommu.h>
#include <linux/limits.h>
<<<<<<< HEAD
#include <linux/of.h>
#include <linux/of_iommu.h>
#include <linux/of_pci.h>
#include <linux/slab.h>

#define NO_IOMMU	1

/**
 * of_get_dma_window - Parse *dma-window property and returns 0 if found.
 *
 * @dn: device node
 * @prefix: prefix for property name if any
 * @index: index to start to parse
 * @busno: Returns busno if supported. Otherwise pass NULL
 * @addr: Returns address that DMA starts
 * @size: Returns the range that DMA can handle
 *
 * This supports different formats flexibly. "prefix" can be
 * configured if any. "busno" and "index" are optionally
 * specified. Set 0(or NULL) if not used.
 */
int of_get_dma_window(struct device_node *dn, const char *prefix, int index,
		      unsigned long *busno, dma_addr_t *addr, size_t *size)
{
	const __be32 *dma_window, *end;
	int bytes, cur_index = 0;
	char propname[NAME_MAX], addrname[NAME_MAX], sizename[NAME_MAX];

	if (!dn || !addr || !size)
		return -EINVAL;

	if (!prefix)
		prefix = "";

	snprintf(propname, sizeof(propname), "%sdma-window", prefix);
	snprintf(addrname, sizeof(addrname), "%s#dma-address-cells", prefix);
	snprintf(sizename, sizeof(sizename), "%s#dma-size-cells", prefix);

	dma_window = of_get_property(dn, propname, &bytes);
	if (!dma_window)
		return -ENODEV;
	end = dma_window + bytes / sizeof(*dma_window);

	while (dma_window < end) {
		u32 cells;
		const void *prop;

		/* busno is one cell if supported */
		if (busno)
			*busno = be32_to_cpup(dma_window++);

		prop = of_get_property(dn, addrname, NULL);
		if (!prop)
			prop = of_get_property(dn, "#address-cells", NULL);

		cells = prop ? be32_to_cpup(prop) : of_n_addr_cells(dn);
		if (!cells)
			return -EINVAL;
		*addr = of_read_number(dma_window, cells);
		dma_window += cells;

		prop = of_get_property(dn, sizename, NULL);
		cells = prop ? be32_to_cpup(prop) : of_n_size_cells(dn);
		if (!cells)
			return -EINVAL;
		*size = of_read_number(dma_window, cells);
		dma_window += cells;

		if (cur_index++ == index)
			break;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(of_get_dma_window);

=======
#include <linux/module.h>
#include <linux/msi.h>
#include <linux/of.h>
#include <linux/of_iommu.h>
#include <linux/of_pci.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/fsl/mc.h>

#define NO_IOMMU	1

>>>>>>> upstream/android-13
static int of_iommu_xlate(struct device *dev,
			  struct of_phandle_args *iommu_spec)
{
	const struct iommu_ops *ops;
	struct fwnode_handle *fwnode = &iommu_spec->np->fwnode;
<<<<<<< HEAD
	int err;
=======
	int ret;
>>>>>>> upstream/android-13

	ops = iommu_ops_from_fwnode(fwnode);
	if ((ops && !ops->of_xlate) ||
	    !of_device_is_available(iommu_spec->np))
		return NO_IOMMU;

<<<<<<< HEAD
	err = iommu_fwspec_init(dev, &iommu_spec->np->fwnode, ops);
	if (err)
		return err;
=======
	ret = iommu_fwspec_init(dev, &iommu_spec->np->fwnode, ops);
	if (ret)
		return ret;
>>>>>>> upstream/android-13
	/*
	 * The otherwise-empty fwspec handily serves to indicate the specific
	 * IOMMU device we're waiting for, which will be useful if we ever get
	 * a proper probe-ordering dependency mechanism in future.
	 */
	if (!ops)
		return driver_deferred_probe_check_state(dev);

<<<<<<< HEAD
	return ops->of_xlate(dev, iommu_spec);
=======
	if (!try_module_get(ops->owner))
		return -ENODEV;

	ret = ops->of_xlate(dev, iommu_spec);
	module_put(ops->owner);
	return ret;
}

static int of_iommu_configure_dev_id(struct device_node *master_np,
				     struct device *dev,
				     const u32 *id)
{
	struct of_phandle_args iommu_spec = { .args_count = 1 };
	int err;

	err = of_map_id(master_np, *id, "iommu-map",
			 "iommu-map-mask", &iommu_spec.np,
			 iommu_spec.args);
	if (err)
		return err == -ENODEV ? NO_IOMMU : err;

	err = of_iommu_xlate(dev, &iommu_spec);
	of_node_put(iommu_spec.np);
	return err;
}

static int of_iommu_configure_dev(struct device_node *master_np,
				  struct device *dev)
{
	struct of_phandle_args iommu_spec;
	int err = NO_IOMMU, idx = 0;

	while (!of_parse_phandle_with_args(master_np, "iommus",
					   "#iommu-cells",
					   idx, &iommu_spec)) {
		err = of_iommu_xlate(dev, &iommu_spec);
		of_node_put(iommu_spec.np);
		idx++;
		if (err)
			break;
	}

	return err;
>>>>>>> upstream/android-13
}

struct of_pci_iommu_alias_info {
	struct device *dev;
	struct device_node *np;
};

static int of_pci_iommu_init(struct pci_dev *pdev, u16 alias, void *data)
{
	struct of_pci_iommu_alias_info *info = data;
<<<<<<< HEAD
	struct of_phandle_args iommu_spec = { .args_count = 1 };
	int err;

	err = of_pci_map_rid(info->np, alias, "iommu-map",
			     "iommu-map-mask", &iommu_spec.np,
			     iommu_spec.args);
	if (err)
		return err == -ENODEV ? NO_IOMMU : err;

	err = of_iommu_xlate(info->dev, &iommu_spec);
	of_node_put(iommu_spec.np);
	return err;
}

const struct iommu_ops *of_iommu_configure(struct device *dev,
					   struct device_node *master_np)
{
	const struct iommu_ops *ops = NULL;
	struct iommu_fwspec *fwspec = dev->iommu_fwspec;
	int err = NO_IOMMU;

	if (!master_np) {
		pr_notice("%s, %d, err master np\n", __func__, __LINE__);
		return NULL;
	}
=======
	u32 input_id = alias;

	return of_iommu_configure_dev_id(info->np, info->dev, &input_id);
}

static int of_iommu_configure_device(struct device_node *master_np,
				     struct device *dev, const u32 *id)
{
	return (id) ? of_iommu_configure_dev_id(master_np, dev, id) :
		      of_iommu_configure_dev(master_np, dev);
}

const struct iommu_ops *of_iommu_configure(struct device *dev,
					   struct device_node *master_np,
					   const u32 *id)
{
	const struct iommu_ops *ops = NULL;
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);
	int err = NO_IOMMU;

	if (!master_np)
		return NULL;
>>>>>>> upstream/android-13

	if (fwspec) {
		if (fwspec->ops)
			return fwspec->ops;

		/* In the deferred case, start again from scratch */
		iommu_fwspec_free(dev);
	}

	/*
	 * We don't currently walk up the tree looking for a parent IOMMU.
	 * See the `Notes:' section of
	 * Documentation/devicetree/bindings/iommu/iommu.txt
	 */
	if (dev_is_pci(dev)) {
		struct of_pci_iommu_alias_info info = {
			.dev = dev,
			.np = master_np,
		};

<<<<<<< HEAD
		err = pci_for_each_dma_alias(to_pci_dev(dev),
					     of_pci_iommu_init, &info);
	} else {
		struct of_phandle_args iommu_spec;
		int idx = 0;

		while (!of_parse_phandle_with_args(master_np, "iommus",
						   "#iommu-cells",
						   idx, &iommu_spec)) {
			err = of_iommu_xlate(dev, &iommu_spec);
			of_node_put(iommu_spec.np);
			idx++;
			if (err)
				break;
		}
=======
		pci_request_acs();
		err = pci_for_each_dma_alias(to_pci_dev(dev),
					     of_pci_iommu_init, &info);
	} else {
		err = of_iommu_configure_device(master_np, dev, id);
>>>>>>> upstream/android-13
	}

	/*
	 * Two success conditions can be represented by non-negative err here:
	 * >0 : there is no IOMMU, or one was unavailable for non-fatal reasons
	 *  0 : we found an IOMMU, and dev->fwspec is initialised appropriately
	 * <0 : any actual error
	 */
<<<<<<< HEAD
	if (!err)
		ops = dev->iommu_fwspec->ops;
	/*
	 * If we have reason to believe the IOMMU driver missed the initial
	 * add_device callback for dev, replay it to get things in order.
	 */
	if (ops && ops->add_device && dev->bus && !dev->iommu_group)
		err = ops->add_device(dev);
=======
	if (!err) {
		/* The fwspec pointer changed, read it again */
		fwspec = dev_iommu_fwspec_get(dev);
		ops    = fwspec->ops;
	}
	/*
	 * If we have reason to believe the IOMMU driver missed the initial
	 * probe for dev, replay it to get things in order.
	 */
	if (!err && dev->bus && !device_iommu_mapped(dev))
		err = iommu_probe_device(dev);
>>>>>>> upstream/android-13

	/* Ignore all other errors apart from EPROBE_DEFER */
	if (err == -EPROBE_DEFER) {
		ops = ERR_PTR(err);
	} else if (err < 0) {
		dev_dbg(dev, "Adding to IOMMU failed: %d\n", err);
		ops = NULL;
	}

	return ops;
}
