<<<<<<< HEAD
/*
 * PowerNV LPC bus handling.
 *
 * Copyright 2013 IBM Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * PowerNV SCOM bus debugfs interface
 *
 * Copyright 2010 Benjamin Herrenschmidt, IBM Corp
 *                <benh@kernel.crashing.org>
 *     and        David Gibson, IBM Corporation.
 * Copyright 2013 IBM Corp.
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/bug.h>
#include <linux/gfp.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/uaccess.h>
#include <linux/debugfs.h>
>>>>>>> upstream/android-13

#include <asm/machdep.h>
#include <asm/firmware.h>
#include <asm/opal.h>
<<<<<<< HEAD
#include <asm/scom.h>

/*
 * We could probably fit that inside the scom_map_t
 * which is a void* after all but it's really too ugly
 * so let's kmalloc it for now
 */
struct opal_scom_map {
	uint32_t chip;
	uint64_t addr;
};

static scom_map_t opal_scom_map(struct device_node *dev, u64 reg, u64 count)
{
	struct opal_scom_map *m;
	const __be32 *gcid;

	if (!of_get_property(dev, "scom-controller", NULL)) {
		pr_err("%s: device %pOF is not a SCOM controller\n",
			__func__, dev);
		return SCOM_MAP_INVALID;
	}
	gcid = of_get_property(dev, "ibm,chip-id", NULL);
	if (!gcid) {
		pr_err("%s: device %pOF has no ibm,chip-id\n",
			__func__, dev);
		return SCOM_MAP_INVALID;
	}
	m = kmalloc(sizeof(*m), GFP_KERNEL);
	if (!m)
		return NULL;
	m->chip = be32_to_cpup(gcid);
	m->addr = reg;

	return (scom_map_t)m;
}

static void opal_scom_unmap(scom_map_t map)
{
	kfree(map);
}

static int opal_xscom_err_xlate(int64_t rc)
{
	switch(rc) {
	case 0:
		return 0;
	/* Add more translations if necessary */
	default:
		return -EIO;
	}
}
=======
#include <asm/prom.h>
>>>>>>> upstream/android-13

static u64 opal_scom_unmangle(u64 addr)
{
	u64 tmp;

	/*
	 * XSCOM addresses use the top nibble to set indirect mode and
	 * its form.  Bits 4-11 are always 0.
	 *
	 * Because the debugfs interface uses signed offsets and shifts
	 * the address left by 3, we basically cannot use the top 4 bits
	 * of the 64-bit address, and thus cannot use the indirect bit.
	 *
	 * To deal with that, we support the indirect bits being in
	 * bits 4-7 (IBM notation) instead of bit 0-3 in this API, we
	 * do the conversion here.
	 *
	 * For in-kernel use, we don't need to do this mangling.  In
	 * kernel won't have bits 4-7 set.
	 *
	 * So:
	 *   debugfs will always   set 0-3 = 0 and clear 4-7
	 *    kernel will always clear 0-3 = 0 and   set 4-7
	 */
	tmp = addr;
	tmp  &= 0x0f00000000000000;
	addr &= 0xf0ffffffffffffff;
	addr |= tmp << 4;

	return addr;
}

<<<<<<< HEAD
static int opal_scom_read(scom_map_t map, u64 reg, u64 *value)
{
	struct opal_scom_map *m = map;
	int64_t rc;
	__be64 v;

	reg = opal_scom_unmangle(m->addr + reg);
	rc = opal_xscom_read(m->chip, reg, (__be64 *)__pa(&v));
	*value = be64_to_cpu(v);
	return opal_xscom_err_xlate(rc);
}

static int opal_scom_write(scom_map_t map, u64 reg, u64 value)
{
	struct opal_scom_map *m = map;
	int64_t rc;

	reg = opal_scom_unmangle(m->addr + reg);
	rc = opal_xscom_write(m->chip, reg, value);
	return opal_xscom_err_xlate(rc);
}

static const struct scom_controller opal_scom_controller = {
	.map	= opal_scom_map,
	.unmap	= opal_scom_unmap,
	.read	= opal_scom_read,
	.write	= opal_scom_write
};

static int opal_xscom_init(void)
{
	if (firmware_has_feature(FW_FEATURE_OPAL))
		scom_init(&opal_scom_controller);
	return 0;
}
machine_arch_initcall(powernv, opal_xscom_init);
=======
static int opal_scom_read(uint32_t chip, uint64_t addr, u64 reg, u64 *value)
{
	int64_t rc;
	__be64 v;

	reg = opal_scom_unmangle(addr + reg);
	rc = opal_xscom_read(chip, reg, (__be64 *)__pa(&v));
	if (rc) {
		*value = 0xfffffffffffffffful;
		return -EIO;
	}
	*value = be64_to_cpu(v);
	return 0;
}

static int opal_scom_write(uint32_t chip, uint64_t addr, u64 reg, u64 value)
{
	int64_t rc;

	reg = opal_scom_unmangle(addr + reg);
	rc = opal_xscom_write(chip, reg, value);
	if (rc)
		return -EIO;
	return 0;
}

struct scom_debug_entry {
	u32 chip;
	struct debugfs_blob_wrapper path;
	char name[16];
};

static ssize_t scom_debug_read(struct file *filp, char __user *ubuf,
			       size_t count, loff_t *ppos)
{
	struct scom_debug_entry *ent = filp->private_data;
	u64 __user *ubuf64 = (u64 __user *)ubuf;
	loff_t off = *ppos;
	ssize_t done = 0;
	u64 reg, reg_base, reg_cnt, val;
	int rc;

	if (off < 0 || (off & 7) || (count & 7))
		return -EINVAL;
	reg_base = off >> 3;
	reg_cnt = count >> 3;

	for (reg = 0; reg < reg_cnt; reg++) {
		rc = opal_scom_read(ent->chip, reg_base, reg, &val);
		if (!rc)
			rc = put_user(val, ubuf64);
		if (rc) {
			if (!done)
				done = rc;
			break;
		}
		ubuf64++;
		*ppos += 8;
		done += 8;
	}
	return done;
}

static ssize_t scom_debug_write(struct file *filp, const char __user *ubuf,
				size_t count, loff_t *ppos)
{
	struct scom_debug_entry *ent = filp->private_data;
	u64 __user *ubuf64 = (u64 __user *)ubuf;
	loff_t off = *ppos;
	ssize_t done = 0;
	u64 reg, reg_base, reg_cnt, val;
	int rc;

	if (off < 0 || (off & 7) || (count & 7))
		return -EINVAL;
	reg_base = off >> 3;
	reg_cnt = count >> 3;

	for (reg = 0; reg < reg_cnt; reg++) {
		rc = get_user(val, ubuf64);
		if (!rc)
			rc = opal_scom_write(ent->chip, reg_base, reg,  val);
		if (rc) {
			if (!done)
				done = rc;
			break;
		}
		ubuf64++;
		done += 8;
	}
	return done;
}

static const struct file_operations scom_debug_fops = {
	.read =		scom_debug_read,
	.write =	scom_debug_write,
	.open =		simple_open,
	.llseek =	default_llseek,
};

static int scom_debug_init_one(struct dentry *root, struct device_node *dn,
			       int chip)
{
	struct scom_debug_entry *ent;
	struct dentry *dir;

	ent = kzalloc(sizeof(*ent), GFP_KERNEL);
	if (!ent)
		return -ENOMEM;

	ent->chip = chip;
	snprintf(ent->name, 16, "%08x", chip);
	ent->path.data = (void *)kasprintf(GFP_KERNEL, "%pOF", dn);
	ent->path.size = strlen((char *)ent->path.data);

	dir = debugfs_create_dir(ent->name, root);
	if (!dir) {
		kfree(ent->path.data);
		kfree(ent);
		return -1;
	}

	debugfs_create_blob("devspec", 0400, dir, &ent->path);
	debugfs_create_file("access", 0600, dir, ent, &scom_debug_fops);

	return 0;
}

static int scom_debug_init(void)
{
	struct device_node *dn;
	struct dentry *root;
	int chip, rc;

	if (!firmware_has_feature(FW_FEATURE_OPAL))
		return 0;

	root = debugfs_create_dir("scom", arch_debugfs_dir);
	if (!root)
		return -1;

	rc = 0;
	for_each_node_with_property(dn, "scom-controller") {
		chip = of_get_ibm_chip_id(dn);
		WARN_ON(chip == -1);
		rc |= scom_debug_init_one(root, dn, chip);
	}

	return rc;
}
device_initcall(scom_debug_init);
>>>>>>> upstream/android-13
