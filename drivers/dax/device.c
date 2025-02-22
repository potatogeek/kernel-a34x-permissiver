<<<<<<< HEAD
/*
 * Copyright(c) 2016 - 2017 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */
=======
// SPDX-License-Identifier: GPL-2.0
/* Copyright(c) 2016-2018 Intel Corporation. All rights reserved. */
#include <linux/memremap.h>
>>>>>>> upstream/android-13
#include <linux/pagemap.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/pfn_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/dax.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include "dax-private.h"
<<<<<<< HEAD
#include "dax.h"

static struct class *dax_class;

/*
 * Rely on the fact that drvdata is set before the attributes are
 * registered, and that the attributes are unregistered before drvdata
 * is cleared to assume that drvdata is always valid.
 */
static ssize_t id_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct dax_region *dax_region = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", dax_region->id);
}
static DEVICE_ATTR_RO(id);

static ssize_t region_size_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct dax_region *dax_region = dev_get_drvdata(dev);

	return sprintf(buf, "%llu\n", (unsigned long long)
			resource_size(&dax_region->res));
}
static struct device_attribute dev_attr_region_size = __ATTR(size, 0444,
		region_size_show, NULL);

static ssize_t align_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct dax_region *dax_region = dev_get_drvdata(dev);

	return sprintf(buf, "%u\n", dax_region->align);
}
static DEVICE_ATTR_RO(align);

static struct attribute *dax_region_attributes[] = {
	&dev_attr_region_size.attr,
	&dev_attr_align.attr,
	&dev_attr_id.attr,
	NULL,
};

static const struct attribute_group dax_region_attribute_group = {
	.name = "dax_region",
	.attrs = dax_region_attributes,
};

static const struct attribute_group *dax_region_attribute_groups[] = {
	&dax_region_attribute_group,
	NULL,
};

static void dax_region_free(struct kref *kref)
{
	struct dax_region *dax_region;

	dax_region = container_of(kref, struct dax_region, kref);
	kfree(dax_region);
}

void dax_region_put(struct dax_region *dax_region)
{
	kref_put(&dax_region->kref, dax_region_free);
}
EXPORT_SYMBOL_GPL(dax_region_put);

static void dax_region_unregister(void *region)
{
	struct dax_region *dax_region = region;

	sysfs_remove_groups(&dax_region->dev->kobj,
			dax_region_attribute_groups);
	dax_region_put(dax_region);
}

struct dax_region *alloc_dax_region(struct device *parent, int region_id,
		struct resource *res, unsigned int align, void *addr,
		unsigned long pfn_flags)
{
	struct dax_region *dax_region;

	/*
	 * The DAX core assumes that it can store its private data in
	 * parent->driver_data. This WARN is a reminder / safeguard for
	 * developers of device-dax drivers.
	 */
	if (dev_get_drvdata(parent)) {
		dev_WARN(parent, "dax core failed to setup private data\n");
		return NULL;
	}

	if (!IS_ALIGNED(res->start, align)
			|| !IS_ALIGNED(resource_size(res), align))
		return NULL;

	dax_region = kzalloc(sizeof(*dax_region), GFP_KERNEL);
	if (!dax_region)
		return NULL;

	dev_set_drvdata(parent, dax_region);
	memcpy(&dax_region->res, res, sizeof(*res));
	dax_region->pfn_flags = pfn_flags;
	kref_init(&dax_region->kref);
	dax_region->id = region_id;
	ida_init(&dax_region->ida);
	dax_region->align = align;
	dax_region->dev = parent;
	dax_region->base = addr;
	if (sysfs_create_groups(&parent->kobj, dax_region_attribute_groups)) {
		kfree(dax_region);
		return NULL;
	}

	kref_get(&dax_region->kref);
	if (devm_add_action_or_reset(parent, dax_region_unregister, dax_region))
		return NULL;
	return dax_region;
}
EXPORT_SYMBOL_GPL(alloc_dax_region);

static struct dev_dax *to_dev_dax(struct device *dev)
{
	return container_of(dev, struct dev_dax, dev);
}

static ssize_t size_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct dev_dax *dev_dax = to_dev_dax(dev);
	unsigned long long size = 0;
	int i;

	for (i = 0; i < dev_dax->num_resources; i++)
		size += resource_size(&dev_dax->res[i]);

	return sprintf(buf, "%llu\n", size);
}
static DEVICE_ATTR_RO(size);

static struct attribute *dev_dax_attributes[] = {
	&dev_attr_size.attr,
	NULL,
};

static const struct attribute_group dev_dax_attribute_group = {
	.attrs = dev_dax_attributes,
};

static const struct attribute_group *dax_attribute_groups[] = {
	&dev_dax_attribute_group,
	NULL,
};
=======
#include "bus.h"
>>>>>>> upstream/android-13

static int check_vma(struct dev_dax *dev_dax, struct vm_area_struct *vma,
		const char *func)
{
<<<<<<< HEAD
	struct dax_region *dax_region = dev_dax->region;
=======
>>>>>>> upstream/android-13
	struct device *dev = &dev_dax->dev;
	unsigned long mask;

	if (!dax_alive(dev_dax->dax_dev))
		return -ENXIO;

	/* prevent private mappings from being established */
	if ((vma->vm_flags & VM_MAYSHARE) != VM_MAYSHARE) {
		dev_info_ratelimited(dev,
				"%s: %s: fail, attempted private mapping\n",
				current->comm, func);
		return -EINVAL;
	}

<<<<<<< HEAD
	mask = dax_region->align - 1;
=======
	mask = dev_dax->align - 1;
>>>>>>> upstream/android-13
	if (vma->vm_start & mask || vma->vm_end & mask) {
		dev_info_ratelimited(dev,
				"%s: %s: fail, unaligned vma (%#lx - %#lx, %#lx)\n",
				current->comm, func, vma->vm_start, vma->vm_end,
				mask);
		return -EINVAL;
	}

<<<<<<< HEAD
	if ((dax_region->pfn_flags & (PFN_DEV|PFN_MAP)) == PFN_DEV
			&& (vma->vm_flags & VM_DONTCOPY) == 0) {
		dev_info_ratelimited(dev,
				"%s: %s: fail, dax range requires MADV_DONTFORK\n",
				current->comm, func);
		return -EINVAL;
	}

=======
>>>>>>> upstream/android-13
	if (!vma_is_dax(vma)) {
		dev_info_ratelimited(dev,
				"%s: %s: fail, vma is not DAX capable\n",
				current->comm, func);
		return -EINVAL;
	}

	return 0;
}

/* see "strong" declaration in tools/testing/nvdimm/dax-dev.c */
__weak phys_addr_t dax_pgoff_to_phys(struct dev_dax *dev_dax, pgoff_t pgoff,
		unsigned long size)
{
<<<<<<< HEAD
	struct resource *res;
	/* gcc-4.6.3-nolibc for i386 complains that this is uninitialized */
	phys_addr_t uninitialized_var(phys);
	int i;

	for (i = 0; i < dev_dax->num_resources; i++) {
		res = &dev_dax->res[i];
		phys = pgoff * PAGE_SIZE + res->start;
		if (phys >= res->start && phys <= res->end)
			break;
		pgoff -= PHYS_PFN(resource_size(res));
	}

	if (i < dev_dax->num_resources) {
		res = &dev_dax->res[i];
		if (phys + size - 1 <= res->end)
			return phys;
	}

=======
	int i;

	for (i = 0; i < dev_dax->nr_range; i++) {
		struct dev_dax_range *dax_range = &dev_dax->ranges[i];
		struct range *range = &dax_range->range;
		unsigned long long pgoff_end;
		phys_addr_t phys;

		pgoff_end = dax_range->pgoff + PHYS_PFN(range_len(range)) - 1;
		if (pgoff < dax_range->pgoff || pgoff > pgoff_end)
			continue;
		phys = PFN_PHYS(pgoff - dax_range->pgoff) + range->start;
		if (phys + size - 1 <= range->end)
			return phys;
		break;
	}
>>>>>>> upstream/android-13
	return -1;
}

static vm_fault_t __dev_dax_pte_fault(struct dev_dax *dev_dax,
				struct vm_fault *vmf, pfn_t *pfn)
{
	struct device *dev = &dev_dax->dev;
<<<<<<< HEAD
	struct dax_region *dax_region;
=======
>>>>>>> upstream/android-13
	phys_addr_t phys;
	unsigned int fault_size = PAGE_SIZE;

	if (check_vma(dev_dax, vmf->vma, __func__))
		return VM_FAULT_SIGBUS;

<<<<<<< HEAD
	dax_region = dev_dax->region;
	if (dax_region->align > PAGE_SIZE) {
		dev_dbg(dev, "alignment (%#x) > fault size (%#x)\n",
			dax_region->align, fault_size);
		return VM_FAULT_SIGBUS;
	}

	if (fault_size != dax_region->align)
=======
	if (dev_dax->align > PAGE_SIZE) {
		dev_dbg(dev, "alignment (%#x) > fault size (%#x)\n",
			dev_dax->align, fault_size);
		return VM_FAULT_SIGBUS;
	}

	if (fault_size != dev_dax->align)
>>>>>>> upstream/android-13
		return VM_FAULT_SIGBUS;

	phys = dax_pgoff_to_phys(dev_dax, vmf->pgoff, PAGE_SIZE);
	if (phys == -1) {
		dev_dbg(dev, "pgoff_to_phys(%#lx) failed\n", vmf->pgoff);
		return VM_FAULT_SIGBUS;
	}

<<<<<<< HEAD
	*pfn = phys_to_pfn_t(phys, dax_region->pfn_flags);
=======
	*pfn = phys_to_pfn_t(phys, PFN_DEV|PFN_MAP);
>>>>>>> upstream/android-13

	return vmf_insert_mixed(vmf->vma, vmf->address, *pfn);
}

static vm_fault_t __dev_dax_pmd_fault(struct dev_dax *dev_dax,
				struct vm_fault *vmf, pfn_t *pfn)
{
	unsigned long pmd_addr = vmf->address & PMD_MASK;
	struct device *dev = &dev_dax->dev;
<<<<<<< HEAD
	struct dax_region *dax_region;
=======
>>>>>>> upstream/android-13
	phys_addr_t phys;
	pgoff_t pgoff;
	unsigned int fault_size = PMD_SIZE;

	if (check_vma(dev_dax, vmf->vma, __func__))
		return VM_FAULT_SIGBUS;

<<<<<<< HEAD
	dax_region = dev_dax->region;
	if (dax_region->align > PMD_SIZE) {
		dev_dbg(dev, "alignment (%#x) > fault size (%#x)\n",
			dax_region->align, fault_size);
		return VM_FAULT_SIGBUS;
	}

	/* dax pmd mappings require pfn_t_devmap() */
	if ((dax_region->pfn_flags & (PFN_DEV|PFN_MAP)) != (PFN_DEV|PFN_MAP)) {
		dev_dbg(dev, "region lacks devmap flags\n");
		return VM_FAULT_SIGBUS;
	}

	if (fault_size < dax_region->align)
		return VM_FAULT_SIGBUS;
	else if (fault_size > dax_region->align)
=======
	if (dev_dax->align > PMD_SIZE) {
		dev_dbg(dev, "alignment (%#x) > fault size (%#x)\n",
			dev_dax->align, fault_size);
		return VM_FAULT_SIGBUS;
	}

	if (fault_size < dev_dax->align)
		return VM_FAULT_SIGBUS;
	else if (fault_size > dev_dax->align)
>>>>>>> upstream/android-13
		return VM_FAULT_FALLBACK;

	/* if we are outside of the VMA */
	if (pmd_addr < vmf->vma->vm_start ||
			(pmd_addr + PMD_SIZE) > vmf->vma->vm_end)
		return VM_FAULT_SIGBUS;

	pgoff = linear_page_index(vmf->vma, pmd_addr);
	phys = dax_pgoff_to_phys(dev_dax, pgoff, PMD_SIZE);
	if (phys == -1) {
		dev_dbg(dev, "pgoff_to_phys(%#lx) failed\n", pgoff);
		return VM_FAULT_SIGBUS;
	}

<<<<<<< HEAD
	*pfn = phys_to_pfn_t(phys, dax_region->pfn_flags);
=======
	*pfn = phys_to_pfn_t(phys, PFN_DEV|PFN_MAP);
>>>>>>> upstream/android-13

	return vmf_insert_pfn_pmd(vmf, *pfn, vmf->flags & FAULT_FLAG_WRITE);
}

#ifdef CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD
static vm_fault_t __dev_dax_pud_fault(struct dev_dax *dev_dax,
				struct vm_fault *vmf, pfn_t *pfn)
{
	unsigned long pud_addr = vmf->address & PUD_MASK;
	struct device *dev = &dev_dax->dev;
<<<<<<< HEAD
	struct dax_region *dax_region;
=======
>>>>>>> upstream/android-13
	phys_addr_t phys;
	pgoff_t pgoff;
	unsigned int fault_size = PUD_SIZE;


	if (check_vma(dev_dax, vmf->vma, __func__))
		return VM_FAULT_SIGBUS;

<<<<<<< HEAD
	dax_region = dev_dax->region;
	if (dax_region->align > PUD_SIZE) {
		dev_dbg(dev, "alignment (%#x) > fault size (%#x)\n",
			dax_region->align, fault_size);
		return VM_FAULT_SIGBUS;
	}

	/* dax pud mappings require pfn_t_devmap() */
	if ((dax_region->pfn_flags & (PFN_DEV|PFN_MAP)) != (PFN_DEV|PFN_MAP)) {
		dev_dbg(dev, "region lacks devmap flags\n");
		return VM_FAULT_SIGBUS;
	}

	if (fault_size < dax_region->align)
		return VM_FAULT_SIGBUS;
	else if (fault_size > dax_region->align)
=======
	if (dev_dax->align > PUD_SIZE) {
		dev_dbg(dev, "alignment (%#x) > fault size (%#x)\n",
			dev_dax->align, fault_size);
		return VM_FAULT_SIGBUS;
	}

	if (fault_size < dev_dax->align)
		return VM_FAULT_SIGBUS;
	else if (fault_size > dev_dax->align)
>>>>>>> upstream/android-13
		return VM_FAULT_FALLBACK;

	/* if we are outside of the VMA */
	if (pud_addr < vmf->vma->vm_start ||
			(pud_addr + PUD_SIZE) > vmf->vma->vm_end)
		return VM_FAULT_SIGBUS;

	pgoff = linear_page_index(vmf->vma, pud_addr);
	phys = dax_pgoff_to_phys(dev_dax, pgoff, PUD_SIZE);
	if (phys == -1) {
		dev_dbg(dev, "pgoff_to_phys(%#lx) failed\n", pgoff);
		return VM_FAULT_SIGBUS;
	}

<<<<<<< HEAD
	*pfn = phys_to_pfn_t(phys, dax_region->pfn_flags);
=======
	*pfn = phys_to_pfn_t(phys, PFN_DEV|PFN_MAP);
>>>>>>> upstream/android-13

	return vmf_insert_pfn_pud(vmf, *pfn, vmf->flags & FAULT_FLAG_WRITE);
}
#else
static vm_fault_t __dev_dax_pud_fault(struct dev_dax *dev_dax,
				struct vm_fault *vmf, pfn_t *pfn)
{
	return VM_FAULT_FALLBACK;
}
#endif /* !CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD */

static vm_fault_t dev_dax_huge_fault(struct vm_fault *vmf,
		enum page_entry_size pe_size)
{
	struct file *filp = vmf->vma->vm_file;
	unsigned long fault_size;
	vm_fault_t rc = VM_FAULT_SIGBUS;
	int id;
	pfn_t pfn;
	struct dev_dax *dev_dax = filp->private_data;

	dev_dbg(&dev_dax->dev, "%s: %s (%#lx - %#lx) size = %d\n", current->comm,
			(vmf->flags & FAULT_FLAG_WRITE) ? "write" : "read",
			vmf->vma->vm_start, vmf->vma->vm_end, pe_size);

	id = dax_read_lock();
	switch (pe_size) {
	case PE_SIZE_PTE:
		fault_size = PAGE_SIZE;
		rc = __dev_dax_pte_fault(dev_dax, vmf, &pfn);
		break;
	case PE_SIZE_PMD:
		fault_size = PMD_SIZE;
		rc = __dev_dax_pmd_fault(dev_dax, vmf, &pfn);
		break;
	case PE_SIZE_PUD:
		fault_size = PUD_SIZE;
		rc = __dev_dax_pud_fault(dev_dax, vmf, &pfn);
		break;
	default:
		rc = VM_FAULT_SIGBUS;
	}

	if (rc == VM_FAULT_NOPAGE) {
		unsigned long i;
		pgoff_t pgoff;

		/*
		 * In the device-dax case the only possibility for a
		 * VM_FAULT_NOPAGE result is when device-dax capacity is
		 * mapped. No need to consider the zero page, or racing
		 * conflicting mappings.
		 */
		pgoff = linear_page_index(vmf->vma, vmf->address
				& ~(fault_size - 1));
		for (i = 0; i < fault_size / PAGE_SIZE; i++) {
			struct page *page;

			page = pfn_to_page(pfn_t_to_pfn(pfn) + i);
			if (page->mapping)
				continue;
			page->mapping = filp->f_mapping;
			page->index = pgoff + i;
		}
	}
	dax_read_unlock(id);

	return rc;
}

static vm_fault_t dev_dax_fault(struct vm_fault *vmf)
{
	return dev_dax_huge_fault(vmf, PE_SIZE_PTE);
}

<<<<<<< HEAD
static int dev_dax_split(struct vm_area_struct *vma, unsigned long addr)
{
	struct file *filp = vma->vm_file;
	struct dev_dax *dev_dax = filp->private_data;
	struct dax_region *dax_region = dev_dax->region;

	if (!IS_ALIGNED(addr, dax_region->align))
=======
static int dev_dax_may_split(struct vm_area_struct *vma, unsigned long addr)
{
	struct file *filp = vma->vm_file;
	struct dev_dax *dev_dax = filp->private_data;

	if (!IS_ALIGNED(addr, dev_dax->align))
>>>>>>> upstream/android-13
		return -EINVAL;
	return 0;
}

static unsigned long dev_dax_pagesize(struct vm_area_struct *vma)
{
	struct file *filp = vma->vm_file;
	struct dev_dax *dev_dax = filp->private_data;
<<<<<<< HEAD
	struct dax_region *dax_region = dev_dax->region;

	return dax_region->align;
=======

	return dev_dax->align;
>>>>>>> upstream/android-13
}

static const struct vm_operations_struct dax_vm_ops = {
	.fault = dev_dax_fault,
	.huge_fault = dev_dax_huge_fault,
<<<<<<< HEAD
	.split = dev_dax_split,
=======
	.may_split = dev_dax_may_split,
>>>>>>> upstream/android-13
	.pagesize = dev_dax_pagesize,
};

static int dax_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct dev_dax *dev_dax = filp->private_data;
	int rc, id;

	dev_dbg(&dev_dax->dev, "trace\n");

	/*
	 * We lock to check dax_dev liveness and will re-check at
	 * fault time.
	 */
	id = dax_read_lock();
	rc = check_vma(dev_dax, vma, __func__);
	dax_read_unlock(id);
	if (rc)
		return rc;

	vma->vm_ops = &dax_vm_ops;
	vma->vm_flags |= VM_HUGEPAGE;
	return 0;
}

/* return an unmapped area aligned to the dax region specified alignment */
static unsigned long dax_get_unmapped_area(struct file *filp,
		unsigned long addr, unsigned long len, unsigned long pgoff,
		unsigned long flags)
{
	unsigned long off, off_end, off_align, len_align, addr_align, align;
	struct dev_dax *dev_dax = filp ? filp->private_data : NULL;
<<<<<<< HEAD
	struct dax_region *dax_region;
=======
>>>>>>> upstream/android-13

	if (!dev_dax || addr)
		goto out;

<<<<<<< HEAD
	dax_region = dev_dax->region;
	align = dax_region->align;
=======
	align = dev_dax->align;
>>>>>>> upstream/android-13
	off = pgoff << PAGE_SHIFT;
	off_end = off + len;
	off_align = round_up(off, align);

	if ((off_end <= off_align) || ((off_end - off_align) < align))
		goto out;

	len_align = len + align;
	if ((off + len_align) < off)
		goto out;

	addr_align = current->mm->get_unmapped_area(filp, addr, len_align,
			pgoff, flags);
	if (!IS_ERR_VALUE(addr_align)) {
		addr_align += (off - addr_align) & (align - 1);
		return addr_align;
	}
 out:
	return current->mm->get_unmapped_area(filp, addr, len, pgoff, flags);
}

static const struct address_space_operations dev_dax_aops = {
<<<<<<< HEAD
	.set_page_dirty		= noop_set_page_dirty,
=======
	.set_page_dirty		= __set_page_dirty_no_writeback,
>>>>>>> upstream/android-13
	.invalidatepage		= noop_invalidatepage,
};

static int dax_open(struct inode *inode, struct file *filp)
{
	struct dax_device *dax_dev = inode_dax(inode);
	struct inode *__dax_inode = dax_inode(dax_dev);
	struct dev_dax *dev_dax = dax_get_private(dax_dev);

	dev_dbg(&dev_dax->dev, "trace\n");
	inode->i_mapping = __dax_inode->i_mapping;
	inode->i_mapping->host = __dax_inode;
	inode->i_mapping->a_ops = &dev_dax_aops;
	filp->f_mapping = inode->i_mapping;
	filp->f_wb_err = filemap_sample_wb_err(filp->f_mapping);
<<<<<<< HEAD
=======
	filp->f_sb_err = file_sample_sb_err(filp);
>>>>>>> upstream/android-13
	filp->private_data = dev_dax;
	inode->i_flags = S_DAX;

	return 0;
}

static int dax_release(struct inode *inode, struct file *filp)
{
	struct dev_dax *dev_dax = filp->private_data;

	dev_dbg(&dev_dax->dev, "trace\n");
	return 0;
}

static const struct file_operations dax_fops = {
	.llseek = noop_llseek,
	.owner = THIS_MODULE,
	.open = dax_open,
	.release = dax_release,
	.get_unmapped_area = dax_get_unmapped_area,
	.mmap = dax_mmap,
	.mmap_supported_flags = MAP_SYNC,
};

<<<<<<< HEAD
static void dev_dax_release(struct device *dev)
{
	struct dev_dax *dev_dax = to_dev_dax(dev);
	struct dax_region *dax_region = dev_dax->region;
	struct dax_device *dax_dev = dev_dax->dax_dev;

	if (dev_dax->id >= 0)
		ida_simple_remove(&dax_region->ida, dev_dax->id);
	dax_region_put(dax_region);
	put_dax(dax_dev);
	kfree(dev_dax);
}

static void kill_dev_dax(struct dev_dax *dev_dax)
{
	struct dax_device *dax_dev = dev_dax->dax_dev;
	struct inode *inode = dax_inode(dax_dev);

	kill_dax(dax_dev);
	unmap_mapping_range(inode->i_mapping, 0, 0, 1);
}

static void unregister_dev_dax(void *dev)
{
	struct dev_dax *dev_dax = to_dev_dax(dev);
	struct dax_device *dax_dev = dev_dax->dax_dev;
	struct inode *inode = dax_inode(dax_dev);
	struct cdev *cdev = inode->i_cdev;

	dev_dbg(dev, "trace\n");

	kill_dev_dax(dev_dax);
	cdev_device_del(cdev, dev);
	put_device(dev);
}

struct dev_dax *devm_create_dev_dax(struct dax_region *dax_region,
		int id, struct resource *res, int count)
{
	struct device *parent = dax_region->dev;
	struct dax_device *dax_dev;
	struct dev_dax *dev_dax;
	struct inode *inode;
	struct device *dev;
	struct cdev *cdev;
	int rc, i;

	if (!count)
		return ERR_PTR(-EINVAL);

	dev_dax = kzalloc(struct_size(dev_dax, res, count), GFP_KERNEL);
	if (!dev_dax)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < count; i++) {
		if (!IS_ALIGNED(res[i].start, dax_region->align)
				|| !IS_ALIGNED(resource_size(&res[i]),
					dax_region->align)) {
			rc = -EINVAL;
			break;
		}
		dev_dax->res[i].start = res[i].start;
		dev_dax->res[i].end = res[i].end;
	}

	if (i < count)
		goto err_id;

	if (id < 0) {
		id = ida_simple_get(&dax_region->ida, 0, 0, GFP_KERNEL);
		dev_dax->id = id;
		if (id < 0) {
			rc = id;
			goto err_id;
		}
	} else {
		/* region provider owns @id lifetime */
		dev_dax->id = -1;
	}

	/*
	 * No 'host' or dax_operations since there is no access to this
	 * device outside of mmap of the resulting character device.
	 */
	dax_dev = alloc_dax(dev_dax, NULL, NULL);
	if (!dax_dev) {
		rc = -ENOMEM;
		goto err_dax;
	}

	/* from here on we're committed to teardown via dax_dev_release() */
	dev = &dev_dax->dev;
	device_initialize(dev);
=======
static void dev_dax_cdev_del(void *cdev)
{
	cdev_del(cdev);
}

static void dev_dax_kill(void *dev_dax)
{
	kill_dev_dax(dev_dax);
}

int dev_dax_probe(struct dev_dax *dev_dax)
{
	struct dax_device *dax_dev = dev_dax->dax_dev;
	struct device *dev = &dev_dax->dev;
	struct dev_pagemap *pgmap;
	struct inode *inode;
	struct cdev *cdev;
	void *addr;
	int rc, i;

	pgmap = dev_dax->pgmap;
	if (dev_WARN_ONCE(dev, pgmap && dev_dax->nr_range > 1,
			"static pgmap / multi-range device conflict\n"))
		return -EINVAL;

	if (!pgmap) {
		pgmap = devm_kzalloc(dev, sizeof(*pgmap) + sizeof(struct range)
				* (dev_dax->nr_range - 1), GFP_KERNEL);
		if (!pgmap)
			return -ENOMEM;
		pgmap->nr_range = dev_dax->nr_range;
	}

	for (i = 0; i < dev_dax->nr_range; i++) {
		struct range *range = &dev_dax->ranges[i].range;

		if (!devm_request_mem_region(dev, range->start,
					range_len(range), dev_name(dev))) {
			dev_warn(dev, "mapping%d: %#llx-%#llx could not reserve range\n",
					i, range->start, range->end);
			return -EBUSY;
		}
		/* don't update the range for static pgmap */
		if (!dev_dax->pgmap)
			pgmap->ranges[i] = *range;
	}

	pgmap->type = MEMORY_DEVICE_GENERIC;
	addr = devm_memremap_pages(dev, pgmap);
	if (IS_ERR(addr))
		return PTR_ERR(addr);
>>>>>>> upstream/android-13

	inode = dax_inode(dax_dev);
	cdev = inode->i_cdev;
	cdev_init(cdev, &dax_fops);
<<<<<<< HEAD
	cdev->owner = parent->driver->owner;

	dev_dax->num_resources = count;
	dev_dax->dax_dev = dax_dev;
	dev_dax->region = dax_region;
	kref_get(&dax_region->kref);

	dev->devt = inode->i_rdev;
	dev->class = dax_class;
	dev->parent = parent;
	dev->groups = dax_attribute_groups;
	dev->release = dev_dax_release;
	dev_set_name(dev, "dax%d.%d", dax_region->id, id);

	rc = cdev_device_add(cdev, dev);
	if (rc) {
		kill_dev_dax(dev_dax);
		put_device(dev);
		return ERR_PTR(rc);
	}

	rc = devm_add_action_or_reset(dax_region->dev, unregister_dev_dax, dev);
	if (rc)
		return ERR_PTR(rc);

	return dev_dax;

 err_dax:
	if (dev_dax->id >= 0)
		ida_simple_remove(&dax_region->ida, dev_dax->id);
 err_id:
	kfree(dev_dax);

	return ERR_PTR(rc);
}
EXPORT_SYMBOL_GPL(devm_create_dev_dax);

static int __init dax_init(void)
{
	dax_class = class_create(THIS_MODULE, "dax");
	return PTR_ERR_OR_ZERO(dax_class);
=======
	if (dev->class) {
		/* for the CONFIG_DEV_DAX_PMEM_COMPAT case */
		cdev->owner = dev->parent->driver->owner;
	} else
		cdev->owner = dev->driver->owner;
	cdev_set_parent(cdev, &dev->kobj);
	rc = cdev_add(cdev, dev->devt, 1);
	if (rc)
		return rc;

	rc = devm_add_action_or_reset(dev, dev_dax_cdev_del, cdev);
	if (rc)
		return rc;

	run_dax(dax_dev);
	return devm_add_action_or_reset(dev, dev_dax_kill, dev_dax);
}
EXPORT_SYMBOL_GPL(dev_dax_probe);

static struct dax_device_driver device_dax_driver = {
	.probe = dev_dax_probe,
	/* all probe actions are unwound by devm, so .remove isn't necessary */
	.match_always = 1,
};

static int __init dax_init(void)
{
	return dax_driver_register(&device_dax_driver);
>>>>>>> upstream/android-13
}

static void __exit dax_exit(void)
{
<<<<<<< HEAD
	class_destroy(dax_class);
=======
	dax_driver_unregister(&device_dax_driver);
>>>>>>> upstream/android-13
}

MODULE_AUTHOR("Intel Corporation");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
subsys_initcall(dax_init);
module_exit(dax_exit);
=======
module_init(dax_init);
module_exit(dax_exit);
MODULE_ALIAS_DAX_DEVICE(0);
>>>>>>> upstream/android-13
