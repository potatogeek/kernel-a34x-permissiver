// SPDX-License-Identifier: GPL-2.0
/*
 * Export the firmware instance and label associated with a PCI device to
 * sysfs
 *
 * Copyright (C) 2010 Dell Inc.
 * by Narendra K <Narendra_K@dell.com>,
 * Jordan Hargrave <Jordan_Hargrave@dell.com>
 *
 * PCI Firmware Specification Revision 3.1 section 4.6.7 (DSM for Naming a
 * PCI or PCI Express Device Under Operating Systems) defines an instance
 * number and string name. This code retrieves them and exports them to sysfs.
 * If the system firmware does not provide the ACPI _DSM (Device Specific
 * Method), then the SMBIOS type 41 instance number and string is exported to
 * sysfs.
 *
 * SMBIOS defines type 41 for onboard pci devices. This code retrieves
 * the instance number and string from the type 41 record and exports
 * it to sysfs.
 *
<<<<<<< HEAD
 * Please see http://linux.dell.com/files/biosdevname/ for more
=======
 * Please see https://linux.dell.com/files/biosdevname/ for more
>>>>>>> upstream/android-13
 * information.
 */

#include <linux/dmi.h>
#include <linux/sysfs.h>
#include <linux/pci.h>
#include <linux/pci_ids.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/nls.h>
#include <linux/acpi.h>
#include <linux/pci-acpi.h>
#include "pci.h"

<<<<<<< HEAD
=======
static bool device_has_acpi_name(struct device *dev)
{
#ifdef CONFIG_ACPI
	acpi_handle handle = ACPI_HANDLE(dev);

	if (!handle)
		return false;

	return acpi_check_dsm(handle, &pci_acpi_dsm_guid, 0x2,
			      1 << DSM_PCI_DEVICE_NAME);
#else
	return false;
#endif
}

>>>>>>> upstream/android-13
#ifdef CONFIG_DMI
enum smbios_attr_enum {
	SMBIOS_ATTR_NONE = 0,
	SMBIOS_ATTR_LABEL_SHOW,
	SMBIOS_ATTR_INSTANCE_SHOW,
};

static size_t find_smbios_instance_string(struct pci_dev *pdev, char *buf,
					  enum smbios_attr_enum attribute)
{
	const struct dmi_device *dmi;
	struct dmi_dev_onboard *donboard;
<<<<<<< HEAD
	int domain_nr;
	int bus;
	int devfn;

	domain_nr = pci_domain_nr(pdev->bus);
	bus = pdev->bus->number;
	devfn = pdev->devfn;
=======
	int domain_nr = pci_domain_nr(pdev->bus);
	int bus = pdev->bus->number;
	int devfn = pdev->devfn;
>>>>>>> upstream/android-13

	dmi = NULL;
	while ((dmi = dmi_find_device(DMI_DEV_TYPE_DEV_ONBOARD,
				      NULL, dmi)) != NULL) {
		donboard = dmi->device_data;
		if (donboard && donboard->segment == domain_nr &&
				donboard->bus == bus &&
				donboard->devfn == devfn) {
			if (buf) {
				if (attribute == SMBIOS_ATTR_INSTANCE_SHOW)
<<<<<<< HEAD
					return scnprintf(buf, PAGE_SIZE,
							 "%d\n",
							 donboard->instance);
				else if (attribute == SMBIOS_ATTR_LABEL_SHOW)
					return scnprintf(buf, PAGE_SIZE,
							 "%s\n",
							 dmi->name);
=======
					return sysfs_emit(buf, "%d\n",
							  donboard->instance);
				else if (attribute == SMBIOS_ATTR_LABEL_SHOW)
					return sysfs_emit(buf, "%s\n",
							  dmi->name);
>>>>>>> upstream/android-13
			}
			return strlen(dmi->name);
		}
	}
	return 0;
}

<<<<<<< HEAD
static umode_t smbios_instance_string_exist(struct kobject *kobj,
					    struct attribute *attr, int n)
{
	struct device *dev;
	struct pci_dev *pdev;

	dev = kobj_to_dev(kobj);
	pdev = to_pci_dev(dev);

	return find_smbios_instance_string(pdev, NULL, SMBIOS_ATTR_NONE) ?
					   S_IRUGO : 0;
}

static ssize_t smbioslabel_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct pci_dev *pdev;
	pdev = to_pci_dev(dev);
=======
static ssize_t smbios_label_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	struct pci_dev *pdev = to_pci_dev(dev);
>>>>>>> upstream/android-13

	return find_smbios_instance_string(pdev, buf,
					   SMBIOS_ATTR_LABEL_SHOW);
}
<<<<<<< HEAD

static ssize_t smbiosinstance_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct pci_dev *pdev;
	pdev = to_pci_dev(dev);
=======
static struct device_attribute dev_attr_smbios_label = __ATTR(label, 0444,
						    smbios_label_show, NULL);

static ssize_t index_show(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	struct pci_dev *pdev = to_pci_dev(dev);
>>>>>>> upstream/android-13

	return find_smbios_instance_string(pdev, buf,
					   SMBIOS_ATTR_INSTANCE_SHOW);
}
<<<<<<< HEAD

static struct device_attribute smbios_attr_label = {
	.attr = {.name = "label", .mode = 0444},
	.show = smbioslabel_show,
};

static struct device_attribute smbios_attr_instance = {
	.attr = {.name = "index", .mode = 0444},
	.show = smbiosinstance_show,
};

static struct attribute *smbios_attributes[] = {
	&smbios_attr_label.attr,
	&smbios_attr_instance.attr,
	NULL,
};

static const struct attribute_group smbios_attr_group = {
	.attrs = smbios_attributes,
	.is_visible = smbios_instance_string_exist,
};

static int pci_create_smbiosname_file(struct pci_dev *pdev)
{
	return sysfs_create_group(&pdev->dev.kobj, &smbios_attr_group);
}

static void pci_remove_smbiosname_file(struct pci_dev *pdev)
{
	sysfs_remove_group(&pdev->dev.kobj, &smbios_attr_group);
}
#else
static inline int pci_create_smbiosname_file(struct pci_dev *pdev)
{
	return -1;
}

static inline void pci_remove_smbiosname_file(struct pci_dev *pdev)
{
}
=======
static DEVICE_ATTR_RO(index);

static struct attribute *smbios_attrs[] = {
	&dev_attr_smbios_label.attr,
	&dev_attr_index.attr,
	NULL,
};

static umode_t smbios_attr_is_visible(struct kobject *kobj, struct attribute *a,
				      int n)
{
	struct device *dev = kobj_to_dev(kobj);
	struct pci_dev *pdev = to_pci_dev(dev);

	if (device_has_acpi_name(dev))
		return 0;

	if (!find_smbios_instance_string(pdev, NULL, SMBIOS_ATTR_NONE))
		return 0;

	return a->mode;
}

const struct attribute_group pci_dev_smbios_attr_group = {
	.attrs = smbios_attrs,
	.is_visible = smbios_attr_is_visible,
};
>>>>>>> upstream/android-13
#endif

#ifdef CONFIG_ACPI
enum acpi_attr_enum {
	ACPI_ATTR_LABEL_SHOW,
	ACPI_ATTR_INDEX_SHOW,
};

<<<<<<< HEAD
static void dsm_label_utf16s_to_utf8s(union acpi_object *obj, char *buf)
{
	int len;
	len = utf16s_to_utf8s((const wchar_t *)obj->buffer.pointer,
			      obj->buffer.length,
			      UTF16_LITTLE_ENDIAN,
			      buf, PAGE_SIZE);
	buf[len] = '\n';
=======
static int dsm_label_utf16s_to_utf8s(union acpi_object *obj, char *buf)
{
	int len;

	len = utf16s_to_utf8s((const wchar_t *)obj->buffer.pointer,
			      obj->buffer.length,
			      UTF16_LITTLE_ENDIAN,
			      buf, PAGE_SIZE - 1);
	buf[len++] = '\n';

	return len;
>>>>>>> upstream/android-13
}

static int dsm_get_label(struct device *dev, char *buf,
			 enum acpi_attr_enum attr)
{
<<<<<<< HEAD
	acpi_handle handle;
	union acpi_object *obj, *tmp;
	int len = -1;

	handle = ACPI_HANDLE(dev);
=======
	acpi_handle handle = ACPI_HANDLE(dev);
	union acpi_object *obj, *tmp;
	int len = 0;

>>>>>>> upstream/android-13
	if (!handle)
		return -1;

	obj = acpi_evaluate_dsm(handle, &pci_acpi_dsm_guid, 0x2,
<<<<<<< HEAD
				DEVICE_LABEL_DSM, NULL);
=======
				DSM_PCI_DEVICE_NAME, NULL);
>>>>>>> upstream/android-13
	if (!obj)
		return -1;

	tmp = obj->package.elements;
	if (obj->type == ACPI_TYPE_PACKAGE && obj->package.count == 2 &&
	    tmp[0].type == ACPI_TYPE_INTEGER &&
	    (tmp[1].type == ACPI_TYPE_STRING ||
	     tmp[1].type == ACPI_TYPE_BUFFER)) {
		/*
		 * The second string element is optional even when
		 * this _DSM is implemented; when not implemented,
		 * this entry must return a null string.
		 */
		if (attr == ACPI_ATTR_INDEX_SHOW) {
<<<<<<< HEAD
			scnprintf(buf, PAGE_SIZE, "%llu\n", tmp->integer.value);
		} else if (attr == ACPI_ATTR_LABEL_SHOW) {
			if (tmp[1].type == ACPI_TYPE_STRING)
				scnprintf(buf, PAGE_SIZE, "%s\n",
					  tmp[1].string.pointer);
			else if (tmp[1].type == ACPI_TYPE_BUFFER)
				dsm_label_utf16s_to_utf8s(tmp + 1, buf);
		}
		len = strlen(buf) > 0 ? strlen(buf) : -1;
=======
			len = sysfs_emit(buf, "%llu\n", tmp->integer.value);
		} else if (attr == ACPI_ATTR_LABEL_SHOW) {
			if (tmp[1].type == ACPI_TYPE_STRING)
				len = sysfs_emit(buf, "%s\n",
						 tmp[1].string.pointer);
			else if (tmp[1].type == ACPI_TYPE_BUFFER)
				len = dsm_label_utf16s_to_utf8s(tmp + 1, buf);
		}
>>>>>>> upstream/android-13
	}

	ACPI_FREE(obj);

<<<<<<< HEAD
	return len;
}

static bool device_has_dsm(struct device *dev)
{
	acpi_handle handle;

	handle = ACPI_HANDLE(dev);
	if (!handle)
		return false;

	return !!acpi_check_dsm(handle, &pci_acpi_dsm_guid, 0x2,
				1 << DEVICE_LABEL_DSM);
}

static umode_t acpi_index_string_exist(struct kobject *kobj,
				       struct attribute *attr, int n)
{
	struct device *dev;

	dev = kobj_to_dev(kobj);

	if (device_has_dsm(dev))
		return S_IRUGO;

	return 0;
}

static ssize_t acpilabel_show(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	return dsm_get_label(dev, buf, ACPI_ATTR_LABEL_SHOW);
}

static ssize_t acpiindex_show(struct device *dev,
=======
	return len > 0 ? len : -1;
}

static ssize_t label_show(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	return dsm_get_label(dev, buf, ACPI_ATTR_LABEL_SHOW);
}
static DEVICE_ATTR_RO(label);

static ssize_t acpi_index_show(struct device *dev,
>>>>>>> upstream/android-13
			      struct device_attribute *attr, char *buf)
{
	return dsm_get_label(dev, buf, ACPI_ATTR_INDEX_SHOW);
}
<<<<<<< HEAD

static struct device_attribute acpi_attr_label = {
	.attr = {.name = "label", .mode = 0444},
	.show = acpilabel_show,
};

static struct device_attribute acpi_attr_index = {
	.attr = {.name = "acpi_index", .mode = 0444},
	.show = acpiindex_show,
};

static struct attribute *acpi_attributes[] = {
	&acpi_attr_label.attr,
	&acpi_attr_index.attr,
	NULL,
};

static const struct attribute_group acpi_attr_group = {
	.attrs = acpi_attributes,
	.is_visible = acpi_index_string_exist,
};

static int pci_create_acpi_index_label_files(struct pci_dev *pdev)
{
	return sysfs_create_group(&pdev->dev.kobj, &acpi_attr_group);
}

static int pci_remove_acpi_index_label_files(struct pci_dev *pdev)
{
	sysfs_remove_group(&pdev->dev.kobj, &acpi_attr_group);
	return 0;
}
#else
static inline int pci_create_acpi_index_label_files(struct pci_dev *pdev)
{
	return -1;
}

static inline int pci_remove_acpi_index_label_files(struct pci_dev *pdev)
{
	return -1;
}

static inline bool device_has_dsm(struct device *dev)
{
	return false;
}
#endif

void pci_create_firmware_label_files(struct pci_dev *pdev)
{
	if (device_has_dsm(&pdev->dev))
		pci_create_acpi_index_label_files(pdev);
	else
		pci_create_smbiosname_file(pdev);
}

void pci_remove_firmware_label_files(struct pci_dev *pdev)
{
	if (device_has_dsm(&pdev->dev))
		pci_remove_acpi_index_label_files(pdev);
	else
		pci_remove_smbiosname_file(pdev);
}
=======
static DEVICE_ATTR_RO(acpi_index);

static struct attribute *acpi_attrs[] = {
	&dev_attr_label.attr,
	&dev_attr_acpi_index.attr,
	NULL,
};

static umode_t acpi_attr_is_visible(struct kobject *kobj, struct attribute *a,
				    int n)
{
	struct device *dev = kobj_to_dev(kobj);

	if (!device_has_acpi_name(dev))
		return 0;

	return a->mode;
}

const struct attribute_group pci_dev_acpi_attr_group = {
	.attrs = acpi_attrs,
	.is_visible = acpi_attr_is_visible,
};
#endif
>>>>>>> upstream/android-13
