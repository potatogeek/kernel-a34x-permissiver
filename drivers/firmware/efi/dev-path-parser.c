<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * dev-path-parser.c - EFI Device Path parser
 * Copyright (C) 2016 Lukas Wunner <lukas@wunner.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (version 2) as
 * published by the Free Software Foundation.
<<<<<<< HEAD
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#include <linux/acpi.h>
#include <linux/efi.h>
#include <linux/pci.h>

<<<<<<< HEAD
struct acpi_hid_uid {
	struct acpi_device_id hid[2];
	char uid[11]; /* UINT_MAX + null byte */
};

static int __init match_acpi_dev(struct device *dev, void *data)
{
	struct acpi_hid_uid hid_uid = *(struct acpi_hid_uid *)data;
	struct acpi_device *adev = to_acpi_device(dev);

	if (acpi_match_device_ids(adev, hid_uid.hid))
		return 0;

	if (adev->pnp.unique_id)
		return !strcmp(adev->pnp.unique_id, hid_uid.uid);
	else
		return !strcmp("0", hid_uid.uid);
}

static long __init parse_acpi_path(struct efi_dev_path *node,
				   struct device *parent, struct device **child)
{
	struct acpi_hid_uid hid_uid = {};
	struct device *phys_dev;

	if (node->length != 12)
		return -EINVAL;

	sprintf(hid_uid.hid[0].id, "%c%c%c%04X",
=======
static long __init parse_acpi_path(const struct efi_dev_path *node,
				   struct device *parent, struct device **child)
{
	char hid[ACPI_ID_LEN], uid[11]; /* UINT_MAX + null byte */
	struct acpi_device *adev;
	struct device *phys_dev;

	if (node->header.length != 12)
		return -EINVAL;

	sprintf(hid, "%c%c%c%04X",
>>>>>>> upstream/android-13
		'A' + ((node->acpi.hid >> 10) & 0x1f) - 1,
		'A' + ((node->acpi.hid >>  5) & 0x1f) - 1,
		'A' + ((node->acpi.hid >>  0) & 0x1f) - 1,
			node->acpi.hid >> 16);
<<<<<<< HEAD
	sprintf(hid_uid.uid, "%u", node->acpi.uid);

	*child = bus_find_device(&acpi_bus_type, NULL, &hid_uid,
				 match_acpi_dev);
	if (!*child)
		return -ENODEV;

	phys_dev = acpi_get_first_physical_node(to_acpi_device(*child));
	if (phys_dev) {
		get_device(phys_dev);
		put_device(*child);
		*child = phys_dev;
	}
=======
	sprintf(uid, "%u", node->acpi.uid);

	for_each_acpi_dev_match(adev, hid, NULL, -1) {
		if (adev->pnp.unique_id && !strcmp(adev->pnp.unique_id, uid))
			break;
		if (!adev->pnp.unique_id && node->acpi.uid == 0)
			break;
	}
	if (!adev)
		return -ENODEV;

	phys_dev = acpi_get_first_physical_node(adev);
	if (phys_dev) {
		*child = get_device(phys_dev);
		acpi_dev_put(adev);
	} else
		*child = &adev->dev;
>>>>>>> upstream/android-13

	return 0;
}

static int __init match_pci_dev(struct device *dev, void *data)
{
	unsigned int devfn = *(unsigned int *)data;

	return dev_is_pci(dev) && to_pci_dev(dev)->devfn == devfn;
}

<<<<<<< HEAD
static long __init parse_pci_path(struct efi_dev_path *node,
=======
static long __init parse_pci_path(const struct efi_dev_path *node,
>>>>>>> upstream/android-13
				  struct device *parent, struct device **child)
{
	unsigned int devfn;

<<<<<<< HEAD
	if (node->length != 6)
=======
	if (node->header.length != 6)
>>>>>>> upstream/android-13
		return -EINVAL;
	if (!parent)
		return -EINVAL;

	devfn = PCI_DEVFN(node->pci.dev, node->pci.fn);

	*child = device_find_child(parent, &devfn, match_pci_dev);
	if (!*child)
		return -ENODEV;

	return 0;
}

/*
 * Insert parsers for further node types here.
 *
 * Each parser takes a pointer to the @node and to the @parent (will be NULL
 * for the first device path node). If a device corresponding to @node was
 * found below @parent, its reference count should be incremented and the
 * device returned in @child.
 *
 * The return value should be 0 on success or a negative int on failure.
 * The special return values 0x01 (EFI_DEV_END_INSTANCE) and 0xFF
 * (EFI_DEV_END_ENTIRE) signal the end of the device path, only
 * parse_end_path() is supposed to return this.
 *
 * Be sure to validate the node length and contents before commencing the
 * search for a device.
 */

<<<<<<< HEAD
static long __init parse_end_path(struct efi_dev_path *node,
				  struct device *parent, struct device **child)
{
	if (node->length != 4)
		return -EINVAL;
	if (node->sub_type != EFI_DEV_END_INSTANCE &&
	    node->sub_type != EFI_DEV_END_ENTIRE)
=======
static long __init parse_end_path(const struct efi_dev_path *node,
				  struct device *parent, struct device **child)
{
	if (node->header.length != 4)
		return -EINVAL;
	if (node->header.sub_type != EFI_DEV_END_INSTANCE &&
	    node->header.sub_type != EFI_DEV_END_ENTIRE)
>>>>>>> upstream/android-13
		return -EINVAL;
	if (!parent)
		return -ENODEV;

	*child = get_device(parent);
<<<<<<< HEAD
	return node->sub_type;
=======
	return node->header.sub_type;
>>>>>>> upstream/android-13
}

/**
 * efi_get_device_by_path - find device by EFI Device Path
 * @node: EFI Device Path
 * @len: maximum length of EFI Device Path in bytes
 *
 * Parse a series of EFI Device Path nodes at @node and find the corresponding
 * device.  If the device was found, its reference count is incremented and a
 * pointer to it is returned.  The caller needs to drop the reference with
 * put_device() after use.  The @node pointer is updated to point to the
 * location immediately after the "End of Hardware Device Path" node.
 *
 * If another Device Path instance follows, @len is decremented by the number
 * of bytes consumed.  Otherwise @len is set to %0.
 *
 * If a Device Path node is malformed or its corresponding device is not found,
 * @node is updated to point to this offending node and an ERR_PTR is returned.
 *
 * If @len is initially %0, the function returns %NULL.  Thus, to iterate over
 * all instances in a path, the following idiom may be used:
 *
 *	while (!IS_ERR_OR_NULL(dev = efi_get_device_by_path(&node, &len))) {
 *		// do something with dev
 *		put_device(dev);
 *	}
 *	if (IS_ERR(dev))
 *		// report error
 *
 * Devices can only be found if they're already instantiated. Most buses
 * instantiate devices in the "subsys" initcall level, hence the earliest
 * initcall level in which this function should be called is "fs".
 *
 * Returns the device on success or
 *	%ERR_PTR(-ENODEV) if no device was found,
 *	%ERR_PTR(-EINVAL) if a node is malformed or exceeds @len,
 *	%ERR_PTR(-ENOTSUPP) if support for a node type is not yet implemented.
 */
<<<<<<< HEAD
struct device * __init efi_get_device_by_path(struct efi_dev_path **node,
=======
struct device * __init efi_get_device_by_path(const struct efi_dev_path **node,
>>>>>>> upstream/android-13
					      size_t *len)
{
	struct device *parent = NULL, *child;
	long ret = 0;

	if (!*len)
		return NULL;

	while (!ret) {
<<<<<<< HEAD
		if (*len < 4 || *len < (*node)->length)
			ret = -EINVAL;
		else if ((*node)->type     == EFI_DEV_ACPI &&
			 (*node)->sub_type == EFI_DEV_BASIC_ACPI)
			ret = parse_acpi_path(*node, parent, &child);
		else if ((*node)->type     == EFI_DEV_HW &&
			 (*node)->sub_type == EFI_DEV_PCI)
			ret = parse_pci_path(*node, parent, &child);
		else if (((*node)->type    == EFI_DEV_END_PATH ||
			  (*node)->type    == EFI_DEV_END_PATH2))
=======
		if (*len < 4 || *len < (*node)->header.length)
			ret = -EINVAL;
		else if ((*node)->header.type		== EFI_DEV_ACPI &&
			 (*node)->header.sub_type	== EFI_DEV_BASIC_ACPI)
			ret = parse_acpi_path(*node, parent, &child);
		else if ((*node)->header.type		== EFI_DEV_HW &&
			 (*node)->header.sub_type	== EFI_DEV_PCI)
			ret = parse_pci_path(*node, parent, &child);
		else if (((*node)->header.type		== EFI_DEV_END_PATH ||
			  (*node)->header.type		== EFI_DEV_END_PATH2))
>>>>>>> upstream/android-13
			ret = parse_end_path(*node, parent, &child);
		else
			ret = -ENOTSUPP;

		put_device(parent);
		if (ret < 0)
			return ERR_PTR(ret);

		parent = child;
<<<<<<< HEAD
		*node  = (void *)*node + (*node)->length;
		*len  -= (*node)->length;
=======
		*node  = (void *)*node + (*node)->header.length;
		*len  -= (*node)->header.length;
>>>>>>> upstream/android-13
	}

	if (ret == EFI_DEV_END_ENTIRE)
		*len = 0;

	return child;
}
