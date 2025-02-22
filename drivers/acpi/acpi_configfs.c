<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * ACPI configfs support
 *
 * Copyright (c) 2016 Intel Corporation
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) "ACPI configfs: " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/configfs.h>
#include <linux/acpi.h>
<<<<<<< HEAD

#include "acpica/accommon.h"
#include "acpica/actables.h"
=======
#include <linux/security.h>
>>>>>>> upstream/android-13

static struct config_group *acpi_table_group;

struct acpi_table {
	struct config_item cfg;
	struct acpi_table_header *header;
	u32 index;
};

static ssize_t acpi_table_aml_write(struct config_item *cfg,
				    const void *data, size_t size)
{
	const struct acpi_table_header *header = data;
	struct acpi_table *table;
<<<<<<< HEAD
	int ret;
=======
	int ret = security_locked_down(LOCKDOWN_ACPI_TABLES);

	if (ret)
		return ret;
>>>>>>> upstream/android-13

	table = container_of(cfg, struct acpi_table, cfg);

	if (table->header) {
		pr_err("table already loaded\n");
		return -EBUSY;
	}

	if (header->length != size) {
		pr_err("invalid table length\n");
		return -EINVAL;
	}

	if (memcmp(header->signature, ACPI_SIG_SSDT, 4)) {
		pr_err("invalid table signature\n");
		return -EINVAL;
	}

	table = container_of(cfg, struct acpi_table, cfg);

	table->header = kmemdup(header, header->length, GFP_KERNEL);
	if (!table->header)
		return -ENOMEM;

<<<<<<< HEAD
	ACPI_INFO(("Host-directed Dynamic ACPI Table Load:"));
	ret = acpi_tb_install_and_load_table(
			ACPI_PTR_TO_PHYSADDR(table->header),
			ACPI_TABLE_ORIGIN_EXTERNAL_VIRTUAL, FALSE,
			&table->index);
=======
	ret = acpi_load_table(table->header, &table->index);
>>>>>>> upstream/android-13
	if (ret) {
		kfree(table->header);
		table->header = NULL;
	}

	return ret;
}

static inline struct acpi_table_header *get_header(struct config_item *cfg)
{
	struct acpi_table *table = container_of(cfg, struct acpi_table, cfg);

	if (!table->header)
		pr_err("table not loaded\n");

<<<<<<< HEAD
	return table->header;
=======
	return table->header ?: ERR_PTR(-EINVAL);
>>>>>>> upstream/android-13
}

static ssize_t acpi_table_aml_read(struct config_item *cfg,
				   void *data, size_t size)
{
	struct acpi_table_header *h = get_header(cfg);

<<<<<<< HEAD
	if (!h)
		return -EINVAL;
=======
	if (IS_ERR(h))
		return PTR_ERR(h);
>>>>>>> upstream/android-13

	if (data)
		memcpy(data, h, h->length);

	return h->length;
}

#define MAX_ACPI_TABLE_SIZE (128 * 1024)

CONFIGFS_BIN_ATTR(acpi_table_, aml, NULL, MAX_ACPI_TABLE_SIZE);

<<<<<<< HEAD
struct configfs_bin_attribute *acpi_table_bin_attrs[] = {
=======
static struct configfs_bin_attribute *acpi_table_bin_attrs[] = {
>>>>>>> upstream/android-13
	&acpi_table_attr_aml,
	NULL,
};

<<<<<<< HEAD
ssize_t acpi_table_signature_show(struct config_item *cfg, char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (!h)
		return -EINVAL;

	return sprintf(str, "%.*s\n", ACPI_NAME_SIZE, h->signature);
}

ssize_t acpi_table_length_show(struct config_item *cfg, char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (!h)
		return -EINVAL;

	return sprintf(str, "%d\n", h->length);
}

ssize_t acpi_table_revision_show(struct config_item *cfg, char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (!h)
		return -EINVAL;

	return sprintf(str, "%d\n", h->revision);
}

ssize_t acpi_table_oem_id_show(struct config_item *cfg, char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (!h)
		return -EINVAL;

	return sprintf(str, "%.*s\n", ACPI_OEM_ID_SIZE, h->oem_id);
}

ssize_t acpi_table_oem_table_id_show(struct config_item *cfg, char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (!h)
		return -EINVAL;

	return sprintf(str, "%.*s\n", ACPI_OEM_TABLE_ID_SIZE, h->oem_table_id);
}

ssize_t acpi_table_oem_revision_show(struct config_item *cfg, char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (!h)
		return -EINVAL;

	return sprintf(str, "%d\n", h->oem_revision);
}

ssize_t acpi_table_asl_compiler_id_show(struct config_item *cfg, char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (!h)
		return -EINVAL;

	return sprintf(str, "%.*s\n", ACPI_NAME_SIZE, h->asl_compiler_id);
}

ssize_t acpi_table_asl_compiler_revision_show(struct config_item *cfg,
					      char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (!h)
		return -EINVAL;

	return sprintf(str, "%d\n", h->asl_compiler_revision);
=======
static ssize_t acpi_table_signature_show(struct config_item *cfg, char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (IS_ERR(h))
		return PTR_ERR(h);

	return sysfs_emit(str, "%.*s\n", ACPI_NAMESEG_SIZE, h->signature);
}

static ssize_t acpi_table_length_show(struct config_item *cfg, char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (IS_ERR(h))
		return PTR_ERR(h);

	return sysfs_emit(str, "%d\n", h->length);
}

static ssize_t acpi_table_revision_show(struct config_item *cfg, char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (IS_ERR(h))
		return PTR_ERR(h);

	return sysfs_emit(str, "%d\n", h->revision);
}

static ssize_t acpi_table_oem_id_show(struct config_item *cfg, char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (IS_ERR(h))
		return PTR_ERR(h);

	return sysfs_emit(str, "%.*s\n", ACPI_OEM_ID_SIZE, h->oem_id);
}

static ssize_t acpi_table_oem_table_id_show(struct config_item *cfg, char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (IS_ERR(h))
		return PTR_ERR(h);

	return sysfs_emit(str, "%.*s\n", ACPI_OEM_TABLE_ID_SIZE, h->oem_table_id);
}

static ssize_t acpi_table_oem_revision_show(struct config_item *cfg, char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (IS_ERR(h))
		return PTR_ERR(h);

	return sysfs_emit(str, "%d\n", h->oem_revision);
}

static ssize_t acpi_table_asl_compiler_id_show(struct config_item *cfg,
					       char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (IS_ERR(h))
		return PTR_ERR(h);

	return sysfs_emit(str, "%.*s\n", ACPI_NAMESEG_SIZE, h->asl_compiler_id);
}

static ssize_t acpi_table_asl_compiler_revision_show(struct config_item *cfg,
						     char *str)
{
	struct acpi_table_header *h = get_header(cfg);

	if (IS_ERR(h))
		return PTR_ERR(h);

	return sysfs_emit(str, "%d\n", h->asl_compiler_revision);
>>>>>>> upstream/android-13
}

CONFIGFS_ATTR_RO(acpi_table_, signature);
CONFIGFS_ATTR_RO(acpi_table_, length);
CONFIGFS_ATTR_RO(acpi_table_, revision);
CONFIGFS_ATTR_RO(acpi_table_, oem_id);
CONFIGFS_ATTR_RO(acpi_table_, oem_table_id);
CONFIGFS_ATTR_RO(acpi_table_, oem_revision);
CONFIGFS_ATTR_RO(acpi_table_, asl_compiler_id);
CONFIGFS_ATTR_RO(acpi_table_, asl_compiler_revision);

<<<<<<< HEAD
struct configfs_attribute *acpi_table_attrs[] = {
=======
static struct configfs_attribute *acpi_table_attrs[] = {
>>>>>>> upstream/android-13
	&acpi_table_attr_signature,
	&acpi_table_attr_length,
	&acpi_table_attr_revision,
	&acpi_table_attr_oem_id,
	&acpi_table_attr_oem_table_id,
	&acpi_table_attr_oem_revision,
	&acpi_table_attr_asl_compiler_id,
	&acpi_table_attr_asl_compiler_revision,
	NULL,
};

static const struct config_item_type acpi_table_type = {
	.ct_owner = THIS_MODULE,
	.ct_bin_attrs = acpi_table_bin_attrs,
	.ct_attrs = acpi_table_attrs,
};

static struct config_item *acpi_table_make_item(struct config_group *group,
						const char *name)
{
	struct acpi_table *table;

	table = kzalloc(sizeof(*table), GFP_KERNEL);
	if (!table)
		return ERR_PTR(-ENOMEM);

	config_item_init_type_name(&table->cfg, name, &acpi_table_type);
	return &table->cfg;
}

static void acpi_table_drop_item(struct config_group *group,
				 struct config_item *cfg)
{
	struct acpi_table *table = container_of(cfg, struct acpi_table, cfg);

<<<<<<< HEAD
	ACPI_INFO(("Host-directed Dynamic ACPI Table Unload"));
	acpi_tb_unload_table(table->index);
}

struct configfs_group_operations acpi_table_group_ops = {
=======
	pr_debug("Host-directed Dynamic ACPI Table Unload\n");
	acpi_unload_table(table->index);
	config_item_put(cfg);
}

static struct configfs_group_operations acpi_table_group_ops = {
>>>>>>> upstream/android-13
	.make_item = acpi_table_make_item,
	.drop_item = acpi_table_drop_item,
};

static const struct config_item_type acpi_tables_type = {
	.ct_owner = THIS_MODULE,
	.ct_group_ops = &acpi_table_group_ops,
};

static const struct config_item_type acpi_root_group_type = {
	.ct_owner = THIS_MODULE,
};

static struct configfs_subsystem acpi_configfs = {
	.su_group = {
		.cg_item = {
			.ci_namebuf = "acpi",
			.ci_type = &acpi_root_group_type,
		},
	},
	.su_mutex = __MUTEX_INITIALIZER(acpi_configfs.su_mutex),
};

static int __init acpi_configfs_init(void)
{
	int ret;
	struct config_group *root = &acpi_configfs.su_group;

	config_group_init(root);

	ret = configfs_register_subsystem(&acpi_configfs);
	if (ret)
		return ret;

	acpi_table_group = configfs_register_default_group(root, "table",
							   &acpi_tables_type);
	if (IS_ERR(acpi_table_group)) {
		configfs_unregister_subsystem(&acpi_configfs);
		return PTR_ERR(acpi_table_group);
	}

	return 0;
}
module_init(acpi_configfs_init);

static void __exit acpi_configfs_exit(void)
{
	configfs_unregister_default_group(acpi_table_group);
	configfs_unregister_subsystem(&acpi_configfs);
}
module_exit(acpi_configfs_exit);

MODULE_AUTHOR("Octavian Purdila <octavian.purdila@intel.com>");
MODULE_DESCRIPTION("ACPI configfs support");
MODULE_LICENSE("GPL v2");
