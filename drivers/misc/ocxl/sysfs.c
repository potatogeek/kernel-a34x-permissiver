// SPDX-License-Identifier: GPL-2.0+
// Copyright 2017 IBM Corp.
#include <linux/sysfs.h>
#include "ocxl_internal.h"

<<<<<<< HEAD
=======
static inline struct ocxl_afu *to_afu(struct device *device)
{
	struct ocxl_file_info *info = container_of(device, struct ocxl_file_info, dev);

	return info->afu;
}

>>>>>>> upstream/android-13
static ssize_t global_mmio_size_show(struct device *device,
				struct device_attribute *attr,
				char *buf)
{
<<<<<<< HEAD
	struct ocxl_afu *afu = to_ocxl_afu(device);
=======
	struct ocxl_afu *afu = to_afu(device);
>>>>>>> upstream/android-13

	return scnprintf(buf, PAGE_SIZE, "%d\n",
			afu->config.global_mmio_size);
}

static ssize_t pp_mmio_size_show(struct device *device,
				struct device_attribute *attr,
				char *buf)
{
<<<<<<< HEAD
	struct ocxl_afu *afu = to_ocxl_afu(device);
=======
	struct ocxl_afu *afu = to_afu(device);
>>>>>>> upstream/android-13

	return scnprintf(buf, PAGE_SIZE, "%d\n",
			afu->config.pp_mmio_stride);
}

static ssize_t afu_version_show(struct device *device,
				struct device_attribute *attr,
				char *buf)
{
<<<<<<< HEAD
	struct ocxl_afu *afu = to_ocxl_afu(device);
=======
	struct ocxl_afu *afu = to_afu(device);
>>>>>>> upstream/android-13

	return scnprintf(buf, PAGE_SIZE, "%hhu:%hhu\n",
			afu->config.version_major,
			afu->config.version_minor);
}

static ssize_t contexts_show(struct device *device,
		struct device_attribute *attr,
		char *buf)
{
<<<<<<< HEAD
	struct ocxl_afu *afu = to_ocxl_afu(device);
=======
	struct ocxl_afu *afu = to_afu(device);
>>>>>>> upstream/android-13

	return scnprintf(buf, PAGE_SIZE, "%d/%d\n",
			afu->pasid_count, afu->pasid_max);
}

<<<<<<< HEAD
=======
static ssize_t reload_on_reset_show(struct device *device,
				    struct device_attribute *attr,
				    char *buf)
{
	struct ocxl_afu *afu = to_afu(device);
	struct ocxl_fn *fn = afu->fn;
	struct pci_dev *pci_dev = to_pci_dev(fn->dev.parent);
	int val;

	if (ocxl_config_get_reset_reload(pci_dev, &val))
		return scnprintf(buf, PAGE_SIZE, "unavailable\n");

	return scnprintf(buf, PAGE_SIZE, "%d\n", val);
}

static ssize_t reload_on_reset_store(struct device *device,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	struct ocxl_afu *afu = to_afu(device);
	struct ocxl_fn *fn = afu->fn;
	struct pci_dev *pci_dev = to_pci_dev(fn->dev.parent);
	int rc, val;

	rc = kstrtoint(buf, 0, &val);
	if (rc || (val != 0 && val != 1))
		return -EINVAL;

	if (ocxl_config_set_reset_reload(pci_dev, val))
		return -ENODEV;

	return count;
}

>>>>>>> upstream/android-13
static struct device_attribute afu_attrs[] = {
	__ATTR_RO(global_mmio_size),
	__ATTR_RO(pp_mmio_size),
	__ATTR_RO(afu_version),
	__ATTR_RO(contexts),
<<<<<<< HEAD
=======
	__ATTR_RW(reload_on_reset),
>>>>>>> upstream/android-13
};

static ssize_t global_mmio_read(struct file *filp, struct kobject *kobj,
				struct bin_attribute *bin_attr, char *buf,
				loff_t off, size_t count)
{
<<<<<<< HEAD
	struct ocxl_afu *afu = to_ocxl_afu(kobj_to_dev(kobj));
=======
	struct ocxl_afu *afu = to_afu(kobj_to_dev(kobj));
>>>>>>> upstream/android-13

	if (count == 0 || off < 0 ||
		off >= afu->config.global_mmio_size)
		return 0;
	memcpy_fromio(buf, afu->global_mmio_ptr + off, count);
	return count;
}

static vm_fault_t global_mmio_fault(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	struct ocxl_afu *afu = vma->vm_private_data;
	unsigned long offset;

	if (vmf->pgoff >= (afu->config.global_mmio_size >> PAGE_SHIFT))
		return VM_FAULT_SIGBUS;

	offset = vmf->pgoff;
	offset += (afu->global_mmio_start >> PAGE_SHIFT);
	return vmf_insert_pfn(vma, vmf->address, offset);
}

static const struct vm_operations_struct global_mmio_vmops = {
	.fault = global_mmio_fault,
};

static int global_mmio_mmap(struct file *filp, struct kobject *kobj,
			struct bin_attribute *bin_attr,
			struct vm_area_struct *vma)
{
<<<<<<< HEAD
	struct ocxl_afu *afu = to_ocxl_afu(kobj_to_dev(kobj));
=======
	struct ocxl_afu *afu = to_afu(kobj_to_dev(kobj));
>>>>>>> upstream/android-13

	if ((vma_pages(vma) + vma->vm_pgoff) >
		(afu->config.global_mmio_size >> PAGE_SHIFT))
		return -EINVAL;

	vma->vm_flags |= VM_IO | VM_PFNMAP;
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	vma->vm_ops = &global_mmio_vmops;
	vma->vm_private_data = afu;
	return 0;
}

<<<<<<< HEAD
int ocxl_sysfs_add_afu(struct ocxl_afu *afu)
=======
int ocxl_sysfs_register_afu(struct ocxl_file_info *info)
>>>>>>> upstream/android-13
{
	int i, rc;

	for (i = 0; i < ARRAY_SIZE(afu_attrs); i++) {
<<<<<<< HEAD
		rc = device_create_file(&afu->dev, &afu_attrs[i]);
=======
		rc = device_create_file(&info->dev, &afu_attrs[i]);
>>>>>>> upstream/android-13
		if (rc)
			goto err;
	}

<<<<<<< HEAD
	sysfs_attr_init(&afu->attr_global_mmio.attr);
	afu->attr_global_mmio.attr.name = "global_mmio_area";
	afu->attr_global_mmio.attr.mode = 0600;
	afu->attr_global_mmio.size = afu->config.global_mmio_size;
	afu->attr_global_mmio.read = global_mmio_read;
	afu->attr_global_mmio.mmap = global_mmio_mmap;
	rc = device_create_bin_file(&afu->dev, &afu->attr_global_mmio);
	if (rc) {
		dev_err(&afu->dev,
			"Unable to create global mmio attr for afu: %d\n",
			rc);
=======
	sysfs_attr_init(&info->attr_global_mmio.attr);
	info->attr_global_mmio.attr.name = "global_mmio_area";
	info->attr_global_mmio.attr.mode = 0600;
	info->attr_global_mmio.size = info->afu->config.global_mmio_size;
	info->attr_global_mmio.read = global_mmio_read;
	info->attr_global_mmio.mmap = global_mmio_mmap;
	rc = device_create_bin_file(&info->dev, &info->attr_global_mmio);
	if (rc) {
		dev_err(&info->dev, "Unable to create global mmio attr for afu: %d\n", rc);
>>>>>>> upstream/android-13
		goto err;
	}

	return 0;

err:
	for (i--; i >= 0; i--)
<<<<<<< HEAD
		device_remove_file(&afu->dev, &afu_attrs[i]);
	return rc;
}

void ocxl_sysfs_remove_afu(struct ocxl_afu *afu)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(afu_attrs); i++)
		device_remove_file(&afu->dev, &afu_attrs[i]);
	device_remove_bin_file(&afu->dev, &afu->attr_global_mmio);
=======
		device_remove_file(&info->dev, &afu_attrs[i]);

	return rc;
}

void ocxl_sysfs_unregister_afu(struct ocxl_file_info *info)
{
	int i;

	/*
	 * device_remove_bin_file is safe to call if the file is not added as
	 * the files are removed by name, and early exit if not found
	 */
	for (i = 0; i < ARRAY_SIZE(afu_attrs); i++)
		device_remove_file(&info->dev, &afu_attrs[i]);
	device_remove_bin_file(&info->dev, &info->attr_global_mmio);
>>>>>>> upstream/android-13
}
