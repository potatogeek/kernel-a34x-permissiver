<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * apple-properties.c - EFI device properties on Macs
 * Copyright (C) 2016 Lukas Wunner <lukas@wunner.de>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (version 2) as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Note, all properties are considered as u8 arrays.
 * To get a value of any of them the caller must use device_property_read_u8_array().
=======
 * Properties are stored either as:
 * u8 arrays which can be retrieved with device_property_read_u8_array() or
 * booleans which can be queried with device_property_present().
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) "apple-properties: " fmt

<<<<<<< HEAD
#include <linux/bootmem.h>
=======
#include <linux/memblock.h>
>>>>>>> upstream/android-13
#include <linux/efi.h>
#include <linux/io.h>
#include <linux/platform_data/x86/apple.h>
#include <linux/property.h>
#include <linux/slab.h>
#include <linux/ucs2_string.h>
#include <asm/setup.h>

static bool dump_properties __initdata;

static int __init dump_properties_enable(char *arg)
{
	dump_properties = true;
<<<<<<< HEAD
	return 0;
=======
	return 1;
>>>>>>> upstream/android-13
}

__setup("dump_apple_properties", dump_properties_enable);

struct dev_header {
	u32 len;
	u32 prop_count;
<<<<<<< HEAD
	struct efi_dev_path path[0];
=======
	struct efi_dev_path path[];
>>>>>>> upstream/android-13
	/*
	 * followed by key/value pairs, each key and value preceded by u32 len,
	 * len includes itself, value may be empty (in which case its len is 4)
	 */
};

struct properties_header {
	u32 len;
	u32 version;
	u32 dev_count;
<<<<<<< HEAD
	struct dev_header dev_header[0];
};

static void __init unmarshal_key_value_pairs(struct dev_header *dev_header,
					     struct device *dev, void *ptr,
=======
	struct dev_header dev_header[];
};

static void __init unmarshal_key_value_pairs(struct dev_header *dev_header,
					     struct device *dev, const void *ptr,
>>>>>>> upstream/android-13
					     struct property_entry entry[])
{
	int i;

	for (i = 0; i < dev_header->prop_count; i++) {
		int remaining = dev_header->len - (ptr - (void *)dev_header);
<<<<<<< HEAD
		u32 key_len, val_len;
=======
		u32 key_len, val_len, entry_len;
		const u8 *entry_data;
>>>>>>> upstream/android-13
		char *key;

		if (sizeof(key_len) > remaining)
			break;

		key_len = *(typeof(key_len) *)ptr;
		if (key_len + sizeof(val_len) > remaining ||
		    key_len < sizeof(key_len) + sizeof(efi_char16_t) ||
		    *(efi_char16_t *)(ptr + sizeof(key_len)) == 0) {
			dev_err(dev, "invalid property name len at %#zx\n",
				ptr - (void *)dev_header);
			break;
		}

		val_len = *(typeof(val_len) *)(ptr + key_len);
		if (key_len + val_len > remaining ||
		    val_len < sizeof(val_len)) {
			dev_err(dev, "invalid property val len at %#zx\n",
				ptr - (void *)dev_header + key_len);
			break;
		}

		/* 4 bytes to accommodate UTF-8 code points + null byte */
		key = kzalloc((key_len - sizeof(key_len)) * 4 + 1, GFP_KERNEL);
		if (!key) {
			dev_err(dev, "cannot allocate property name\n");
			break;
		}
		ucs2_as_utf8(key, ptr + sizeof(key_len),
			     key_len - sizeof(key_len));

<<<<<<< HEAD
		entry[i].name = key;
		entry[i].length = val_len - sizeof(val_len);
		entry[i].is_array = !!entry[i].length;
		entry[i].type = DEV_PROP_U8;
		entry[i].pointer.u8_data = ptr + key_len + sizeof(val_len);

		if (dump_properties) {
			dev_info(dev, "property: %s\n", entry[i].name);
			print_hex_dump(KERN_INFO, pr_fmt(), DUMP_PREFIX_OFFSET,
				16, 1, entry[i].pointer.u8_data,
				entry[i].length, true);
=======
		entry_data = ptr + key_len + sizeof(val_len);
		entry_len = val_len - sizeof(val_len);
		if (entry_len)
			entry[i] = PROPERTY_ENTRY_U8_ARRAY_LEN(key, entry_data,
							       entry_len);
		else
			entry[i] = PROPERTY_ENTRY_BOOL(key);

		if (dump_properties) {
			dev_info(dev, "property: %s\n", key);
			print_hex_dump(KERN_INFO, pr_fmt(), DUMP_PREFIX_OFFSET,
				16, 1, entry_data, entry_len, true);
>>>>>>> upstream/android-13
		}

		ptr += key_len + val_len;
	}

	if (i != dev_header->prop_count) {
		dev_err(dev, "got %d device properties, expected %u\n", i,
			dev_header->prop_count);
		print_hex_dump(KERN_ERR, pr_fmt(), DUMP_PREFIX_OFFSET,
			16, 1, dev_header, dev_header->len, true);
		return;
	}

	dev_info(dev, "assigning %d device properties\n", i);
}

static int __init unmarshal_devices(struct properties_header *properties)
{
	size_t offset = offsetof(struct properties_header, dev_header[0]);

	while (offset + sizeof(struct dev_header) < properties->len) {
		struct dev_header *dev_header = (void *)properties + offset;
		struct property_entry *entry = NULL;
<<<<<<< HEAD
		struct device *dev;
		size_t len;
		int ret, i;
		void *ptr;
=======
		const struct efi_dev_path *ptr;
		struct device *dev;
		size_t len;
		int ret, i;
>>>>>>> upstream/android-13

		if (offset + dev_header->len > properties->len ||
		    dev_header->len <= sizeof(*dev_header)) {
			pr_err("invalid len in dev_header at %#zx\n", offset);
			return -EINVAL;
		}

		ptr = dev_header->path;
		len = dev_header->len - sizeof(*dev_header);

<<<<<<< HEAD
		dev = efi_get_device_by_path((struct efi_dev_path **)&ptr, &len);
		if (IS_ERR(dev)) {
			pr_err("device path parse error %ld at %#zx:\n",
			       PTR_ERR(dev), ptr - (void *)dev_header);
=======
		dev = efi_get_device_by_path(&ptr, &len);
		if (IS_ERR(dev)) {
			pr_err("device path parse error %ld at %#zx:\n",
			       PTR_ERR(dev), (void *)ptr - (void *)dev_header);
>>>>>>> upstream/android-13
			print_hex_dump(KERN_ERR, pr_fmt(), DUMP_PREFIX_OFFSET,
			       16, 1, dev_header, dev_header->len, true);
			dev = NULL;
			goto skip_device;
		}

		entry = kcalloc(dev_header->prop_count + 1, sizeof(*entry),
				GFP_KERNEL);
		if (!entry) {
			dev_err(dev, "cannot allocate properties\n");
			goto skip_device;
		}

		unmarshal_key_value_pairs(dev_header, dev, ptr, entry);
		if (!entry[0].name)
			goto skip_device;

<<<<<<< HEAD
		ret = device_add_properties(dev, entry); /* makes deep copy */
=======
		ret = device_create_managed_software_node(dev, entry, NULL);
>>>>>>> upstream/android-13
		if (ret)
			dev_err(dev, "error %d assigning properties\n", ret);

		for (i = 0; entry[i].name; i++)
			kfree(entry[i].name);

skip_device:
		kfree(entry);
		put_device(dev);
		offset += dev_header->len;
	}

	return 0;
}

static int __init map_properties(void)
{
	struct properties_header *properties;
	struct setup_data *data;
	u32 data_len;
	u64 pa_data;
	int ret;

	if (!x86_apple_machine)
		return 0;

	pa_data = boot_params.hdr.setup_data;
	while (pa_data) {
		data = memremap(pa_data, sizeof(*data), MEMREMAP_WB);
		if (!data) {
			pr_err("cannot map setup_data header\n");
			return -ENOMEM;
		}

		if (data->type != SETUP_APPLE_PROPERTIES) {
			pa_data = data->next;
			memunmap(data);
			continue;
		}

		data_len = data->len;
		memunmap(data);

		data = memremap(pa_data, sizeof(*data) + data_len, MEMREMAP_WB);
		if (!data) {
			pr_err("cannot map setup_data payload\n");
			return -ENOMEM;
		}

		properties = (struct properties_header *)data->data;
		if (properties->version != 1) {
			pr_err("unsupported version:\n");
			print_hex_dump(KERN_ERR, pr_fmt(), DUMP_PREFIX_OFFSET,
			       16, 1, properties, data_len, true);
			ret = -ENOTSUPP;
		} else if (properties->len != data_len) {
			pr_err("length mismatch, expected %u\n", data_len);
			print_hex_dump(KERN_ERR, pr_fmt(), DUMP_PREFIX_OFFSET,
			       16, 1, properties, data_len, true);
			ret = -EINVAL;
		} else
			ret = unmarshal_devices(properties);

		/*
		 * Can only free the setup_data payload but not its header
		 * to avoid breaking the chain of ->next pointers.
		 */
		data->len = 0;
		memunmap(data);
<<<<<<< HEAD
		free_bootmem_late(pa_data + sizeof(*data), data_len);
=======
		memblock_free_late(pa_data + sizeof(*data), data_len);
>>>>>>> upstream/android-13

		return ret;
	}
	return 0;
}

fs_initcall(map_properties);
