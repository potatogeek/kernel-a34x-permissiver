<<<<<<< HEAD
/*
 * Copyright(c) 2013-2015 Intel Corporation. All rights reserved.
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
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright(c) 2013-2015 Intel Corporation. All rights reserved.
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/moduleparam.h>
>>>>>>> upstream/android-13
#include <linux/vmalloc.h>
#include <linux/device.h>
#include <linux/ndctl.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include "nd-core.h"
#include "label.h"
#include "pmem.h"
#include "nd.h"

static DEFINE_IDA(dimm_ida);

<<<<<<< HEAD
=======
static bool noblk;
module_param(noblk, bool, 0444);
MODULE_PARM_DESC(noblk, "force disable BLK / local alias support");

>>>>>>> upstream/android-13
/*
 * Retrieve bus and dimm handle and return if this bus supports
 * get_config_data commands
 */
int nvdimm_check_config_data(struct device *dev)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);

	if (!nvdimm->cmd_mask ||
	    !test_bit(ND_CMD_GET_CONFIG_DATA, &nvdimm->cmd_mask)) {
<<<<<<< HEAD
		if (test_bit(NDD_ALIASING, &nvdimm->flags))
=======
		if (test_bit(NDD_LABELING, &nvdimm->flags))
>>>>>>> upstream/android-13
			return -ENXIO;
		else
			return -ENOTTY;
	}

	return 0;
}

static int validate_dimm(struct nvdimm_drvdata *ndd)
{
	int rc;

	if (!ndd)
		return -EINVAL;

	rc = nvdimm_check_config_data(ndd->dev);
	if (rc)
<<<<<<< HEAD
		dev_dbg(ndd->dev, "%pf: %s error: %d\n",
=======
		dev_dbg(ndd->dev, "%ps: %s error: %d\n",
>>>>>>> upstream/android-13
				__builtin_return_address(0), __func__, rc);
	return rc;
}

/**
 * nvdimm_init_nsarea - determine the geometry of a dimm's namespace area
 * @nvdimm: dimm to initialize
 */
int nvdimm_init_nsarea(struct nvdimm_drvdata *ndd)
{
	struct nd_cmd_get_config_size *cmd = &ndd->nsarea;
	struct nvdimm_bus *nvdimm_bus = walk_to_nvdimm_bus(ndd->dev);
	struct nvdimm_bus_descriptor *nd_desc;
	int rc = validate_dimm(ndd);
	int cmd_rc = 0;

	if (rc)
		return rc;

	if (cmd->config_size)
		return 0; /* already valid */

	memset(cmd, 0, sizeof(*cmd));
	nd_desc = nvdimm_bus->nd_desc;
	rc = nd_desc->ndctl(nd_desc, to_nvdimm(ndd->dev),
			ND_CMD_GET_CONFIG_SIZE, cmd, sizeof(*cmd), &cmd_rc);
	if (rc < 0)
		return rc;
	return cmd_rc;
}

<<<<<<< HEAD
int nvdimm_init_config_data(struct nvdimm_drvdata *ndd)
{
	struct nvdimm_bus *nvdimm_bus = walk_to_nvdimm_bus(ndd->dev);
	int rc = validate_dimm(ndd), cmd_rc = 0;
	struct nd_cmd_get_config_data_hdr *cmd;
	struct nvdimm_bus_descriptor *nd_desc;
	u32 max_cmd_size, config_size;
	size_t offset;
=======
int nvdimm_get_config_data(struct nvdimm_drvdata *ndd, void *buf,
			   size_t offset, size_t len)
{
	struct nvdimm_bus *nvdimm_bus = walk_to_nvdimm_bus(ndd->dev);
	struct nvdimm_bus_descriptor *nd_desc = nvdimm_bus->nd_desc;
	int rc = validate_dimm(ndd), cmd_rc = 0;
	struct nd_cmd_get_config_data_hdr *cmd;
	size_t max_cmd_size, buf_offset;
>>>>>>> upstream/android-13

	if (rc)
		return rc;

<<<<<<< HEAD
	if (ndd->data)
		return 0;

	if (ndd->nsarea.status || ndd->nsarea.max_xfer == 0
			|| ndd->nsarea.config_size < ND_LABEL_MIN_SIZE) {
		dev_dbg(ndd->dev, "failed to init config data area: (%d:%d)\n",
				ndd->nsarea.max_xfer, ndd->nsarea.config_size);
		return -ENXIO;
	}

	ndd->data = kvmalloc(ndd->nsarea.config_size, GFP_KERNEL);
	if (!ndd->data)
		return -ENOMEM;

	max_cmd_size = min_t(u32, PAGE_SIZE, ndd->nsarea.max_xfer);
	cmd = kzalloc(max_cmd_size + sizeof(*cmd), GFP_KERNEL);
	if (!cmd)
		return -ENOMEM;

	nd_desc = nvdimm_bus->nd_desc;
	for (config_size = ndd->nsarea.config_size, offset = 0;
			config_size; config_size -= cmd->in_length,
			offset += cmd->in_length) {
		cmd->in_length = min(config_size, max_cmd_size);
		cmd->in_offset = offset;
		rc = nd_desc->ndctl(nd_desc, to_nvdimm(ndd->dev),
				ND_CMD_GET_CONFIG_DATA, cmd,
				cmd->in_length + sizeof(*cmd), &cmd_rc);
=======
	if (offset + len > ndd->nsarea.config_size)
		return -ENXIO;

	max_cmd_size = min_t(u32, len, ndd->nsarea.max_xfer);
	cmd = kvzalloc(max_cmd_size + sizeof(*cmd), GFP_KERNEL);
	if (!cmd)
		return -ENOMEM;

	for (buf_offset = 0; len;
	     len -= cmd->in_length, buf_offset += cmd->in_length) {
		size_t cmd_size;

		cmd->in_offset = offset + buf_offset;
		cmd->in_length = min(max_cmd_size, len);

		cmd_size = sizeof(*cmd) + cmd->in_length;

		rc = nd_desc->ndctl(nd_desc, to_nvdimm(ndd->dev),
				ND_CMD_GET_CONFIG_DATA, cmd, cmd_size, &cmd_rc);
>>>>>>> upstream/android-13
		if (rc < 0)
			break;
		if (cmd_rc < 0) {
			rc = cmd_rc;
			break;
		}
<<<<<<< HEAD
		memcpy(ndd->data + offset, cmd->out_buf, cmd->in_length);
	}
	dev_dbg(ndd->dev, "len: %zu rc: %d\n", offset, rc);
	kfree(cmd);
=======

		/* out_buf should be valid, copy it into our output buffer */
		memcpy(buf + buf_offset, cmd->out_buf, cmd->in_length);
	}
	kvfree(cmd);
>>>>>>> upstream/android-13

	return rc;
}

int nvdimm_set_config_data(struct nvdimm_drvdata *ndd, size_t offset,
		void *buf, size_t len)
{
	size_t max_cmd_size, buf_offset;
	struct nd_cmd_set_config_hdr *cmd;
	int rc = validate_dimm(ndd), cmd_rc = 0;
	struct nvdimm_bus *nvdimm_bus = walk_to_nvdimm_bus(ndd->dev);
	struct nvdimm_bus_descriptor *nd_desc = nvdimm_bus->nd_desc;

	if (rc)
		return rc;

<<<<<<< HEAD
	if (!ndd->data)
		return -ENXIO;

	if (offset + len > ndd->nsarea.config_size)
		return -ENXIO;

	max_cmd_size = min_t(u32, PAGE_SIZE, len);
	max_cmd_size = min_t(u32, max_cmd_size, ndd->nsarea.max_xfer);
	cmd = kzalloc(max_cmd_size + sizeof(*cmd) + sizeof(u32), GFP_KERNEL);
=======
	if (offset + len > ndd->nsarea.config_size)
		return -ENXIO;

	max_cmd_size = min_t(u32, len, ndd->nsarea.max_xfer);
	cmd = kvzalloc(max_cmd_size + sizeof(*cmd) + sizeof(u32), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!cmd)
		return -ENOMEM;

	for (buf_offset = 0; len; len -= cmd->in_length,
			buf_offset += cmd->in_length) {
		size_t cmd_size;

		cmd->in_offset = offset + buf_offset;
		cmd->in_length = min(max_cmd_size, len);
		memcpy(cmd->in_buf, buf + buf_offset, cmd->in_length);

		/* status is output in the last 4-bytes of the command buffer */
		cmd_size = sizeof(*cmd) + cmd->in_length + sizeof(u32);

		rc = nd_desc->ndctl(nd_desc, to_nvdimm(ndd->dev),
				ND_CMD_SET_CONFIG_DATA, cmd, cmd_size, &cmd_rc);
		if (rc < 0)
			break;
		if (cmd_rc < 0) {
			rc = cmd_rc;
			break;
		}
	}
<<<<<<< HEAD
	kfree(cmd);
=======
	kvfree(cmd);
>>>>>>> upstream/android-13

	return rc;
}

<<<<<<< HEAD
void nvdimm_set_aliasing(struct device *dev)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);

	set_bit(NDD_ALIASING, &nvdimm->flags);
=======
void nvdimm_set_labeling(struct device *dev)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);

	set_bit(NDD_LABELING, &nvdimm->flags);
>>>>>>> upstream/android-13
}

void nvdimm_set_locked(struct device *dev)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);

	set_bit(NDD_LOCKED, &nvdimm->flags);
}

void nvdimm_clear_locked(struct device *dev)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);

	clear_bit(NDD_LOCKED, &nvdimm->flags);
}

static void nvdimm_release(struct device *dev)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);

	ida_simple_remove(&dimm_ida, nvdimm->id);
	kfree(nvdimm);
}

<<<<<<< HEAD
static struct device_type nvdimm_device_type = {
	.name = "nvdimm",
	.release = nvdimm_release,
};

bool is_nvdimm(struct device *dev)
{
	return dev->type == &nvdimm_device_type;
}

=======
>>>>>>> upstream/android-13
struct nvdimm *to_nvdimm(struct device *dev)
{
	struct nvdimm *nvdimm = container_of(dev, struct nvdimm, dev);

	WARN_ON(!is_nvdimm(dev));
	return nvdimm;
}
EXPORT_SYMBOL_GPL(to_nvdimm);

struct nvdimm *nd_blk_region_to_dimm(struct nd_blk_region *ndbr)
{
	struct nd_region *nd_region = &ndbr->nd_region;
	struct nd_mapping *nd_mapping = &nd_region->mapping[0];

	return nd_mapping->nvdimm;
}
EXPORT_SYMBOL_GPL(nd_blk_region_to_dimm);

unsigned long nd_blk_memremap_flags(struct nd_blk_region *ndbr)
{
	/* pmem mapping properties are private to libnvdimm */
	return ARCH_MEMREMAP_PMEM;
}
EXPORT_SYMBOL_GPL(nd_blk_memremap_flags);

struct nvdimm_drvdata *to_ndd(struct nd_mapping *nd_mapping)
{
	struct nvdimm *nvdimm = nd_mapping->nvdimm;

	WARN_ON_ONCE(!is_nvdimm_bus_locked(&nvdimm->dev));

	return dev_get_drvdata(&nvdimm->dev);
}
EXPORT_SYMBOL(to_ndd);

void nvdimm_drvdata_release(struct kref *kref)
{
	struct nvdimm_drvdata *ndd = container_of(kref, typeof(*ndd), kref);
	struct device *dev = ndd->dev;
	struct resource *res, *_r;

	dev_dbg(dev, "trace\n");
	nvdimm_bus_lock(dev);
	for_each_dpa_resource_safe(ndd, res, _r)
		nvdimm_free_dpa(ndd, res);
	nvdimm_bus_unlock(dev);

	kvfree(ndd->data);
	kfree(ndd);
	put_device(dev);
}

void get_ndd(struct nvdimm_drvdata *ndd)
{
	kref_get(&ndd->kref);
}

void put_ndd(struct nvdimm_drvdata *ndd)
{
	if (ndd)
		kref_put(&ndd->kref, nvdimm_drvdata_release);
}

const char *nvdimm_name(struct nvdimm *nvdimm)
{
	return dev_name(&nvdimm->dev);
}
EXPORT_SYMBOL_GPL(nvdimm_name);

struct kobject *nvdimm_kobj(struct nvdimm *nvdimm)
{
	return &nvdimm->dev.kobj;
}
EXPORT_SYMBOL_GPL(nvdimm_kobj);

unsigned long nvdimm_cmd_mask(struct nvdimm *nvdimm)
{
	return nvdimm->cmd_mask;
}
EXPORT_SYMBOL_GPL(nvdimm_cmd_mask);

void *nvdimm_provider_data(struct nvdimm *nvdimm)
{
	if (nvdimm)
		return nvdimm->provider_data;
	return NULL;
}
EXPORT_SYMBOL_GPL(nvdimm_provider_data);

static ssize_t commands_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);
	int cmd, len = 0;

	if (!nvdimm->cmd_mask)
		return sprintf(buf, "\n");

	for_each_set_bit(cmd, &nvdimm->cmd_mask, BITS_PER_LONG)
		len += sprintf(buf + len, "%s ", nvdimm_cmd_name(cmd));
	len += sprintf(buf + len, "\n");
	return len;
}
static DEVICE_ATTR_RO(commands);

static ssize_t flags_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);

<<<<<<< HEAD
	return sprintf(buf, "%s%s\n",
			test_bit(NDD_ALIASING, &nvdimm->flags) ? "alias " : "",
=======
	return sprintf(buf, "%s%s%s\n",
			test_bit(NDD_ALIASING, &nvdimm->flags) ? "alias " : "",
			test_bit(NDD_LABELING, &nvdimm->flags) ? "label " : "",
>>>>>>> upstream/android-13
			test_bit(NDD_LOCKED, &nvdimm->flags) ? "lock " : "");
}
static DEVICE_ATTR_RO(flags);

static ssize_t state_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);

	/*
	 * The state may be in the process of changing, userspace should
	 * quiesce probing if it wants a static answer
	 */
	nvdimm_bus_lock(dev);
	nvdimm_bus_unlock(dev);
	return sprintf(buf, "%s\n", atomic_read(&nvdimm->busy)
			? "active" : "idle");
}
static DEVICE_ATTR_RO(state);

static ssize_t __available_slots_show(struct nvdimm_drvdata *ndd, char *buf)
{
	struct device *dev;
	ssize_t rc;
	u32 nfree;

	if (!ndd)
		return -ENXIO;

	dev = ndd->dev;
	nvdimm_bus_lock(dev);
	nfree = nd_label_nfree(ndd);
	if (nfree - 1 > nfree) {
		dev_WARN_ONCE(dev, 1, "we ate our last label?\n");
		nfree = 0;
	} else
		nfree--;
	rc = sprintf(buf, "%d\n", nfree);
	nvdimm_bus_unlock(dev);
	return rc;
}

static ssize_t available_slots_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	ssize_t rc;

<<<<<<< HEAD
	device_lock(dev);
	rc = __available_slots_show(dev_get_drvdata(dev), buf);
	device_unlock(dev);
=======
	nd_device_lock(dev);
	rc = __available_slots_show(dev_get_drvdata(dev), buf);
	nd_device_unlock(dev);
>>>>>>> upstream/android-13

	return rc;
}
static DEVICE_ATTR_RO(available_slots);

<<<<<<< HEAD
=======
__weak ssize_t security_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);

	if (test_bit(NVDIMM_SECURITY_OVERWRITE, &nvdimm->sec.flags))
		return sprintf(buf, "overwrite\n");
	if (test_bit(NVDIMM_SECURITY_DISABLED, &nvdimm->sec.flags))
		return sprintf(buf, "disabled\n");
	if (test_bit(NVDIMM_SECURITY_UNLOCKED, &nvdimm->sec.flags))
		return sprintf(buf, "unlocked\n");
	if (test_bit(NVDIMM_SECURITY_LOCKED, &nvdimm->sec.flags))
		return sprintf(buf, "locked\n");
	return -ENOTTY;
}

static ssize_t frozen_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);

	return sprintf(buf, "%d\n", test_bit(NVDIMM_SECURITY_FROZEN,
				&nvdimm->sec.flags));
}
static DEVICE_ATTR_RO(frozen);

static ssize_t security_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t len)

{
	ssize_t rc;

	/*
	 * Require all userspace triggered security management to be
	 * done while probing is idle and the DIMM is not in active use
	 * in any region.
	 */
	nd_device_lock(dev);
	nvdimm_bus_lock(dev);
	wait_nvdimm_bus_probe_idle(dev);
	rc = nvdimm_security_store(dev, buf, len);
	nvdimm_bus_unlock(dev);
	nd_device_unlock(dev);

	return rc;
}
static DEVICE_ATTR_RW(security);

>>>>>>> upstream/android-13
static struct attribute *nvdimm_attributes[] = {
	&dev_attr_state.attr,
	&dev_attr_flags.attr,
	&dev_attr_commands.attr,
	&dev_attr_available_slots.attr,
<<<<<<< HEAD
	NULL,
};

struct attribute_group nvdimm_attribute_group = {
	.attrs = nvdimm_attributes,
};
EXPORT_SYMBOL_GPL(nvdimm_attribute_group);

struct nvdimm *nvdimm_create(struct nvdimm_bus *nvdimm_bus, void *provider_data,
		const struct attribute_group **groups, unsigned long flags,
		unsigned long cmd_mask, int num_flush,
		struct resource *flush_wpq)
=======
	&dev_attr_security.attr,
	&dev_attr_frozen.attr,
	NULL,
};

static umode_t nvdimm_visible(struct kobject *kobj, struct attribute *a, int n)
{
	struct device *dev = container_of(kobj, typeof(*dev), kobj);
	struct nvdimm *nvdimm = to_nvdimm(dev);

	if (a != &dev_attr_security.attr && a != &dev_attr_frozen.attr)
		return a->mode;
	if (!nvdimm->sec.flags)
		return 0;

	if (a == &dev_attr_security.attr) {
		/* Are there any state mutation ops (make writable)? */
		if (nvdimm->sec.ops->freeze || nvdimm->sec.ops->disable
				|| nvdimm->sec.ops->change_key
				|| nvdimm->sec.ops->erase
				|| nvdimm->sec.ops->overwrite)
			return a->mode;
		return 0444;
	}

	if (nvdimm->sec.ops->freeze)
		return a->mode;
	return 0;
}

static const struct attribute_group nvdimm_attribute_group = {
	.attrs = nvdimm_attributes,
	.is_visible = nvdimm_visible,
};

static ssize_t result_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);
	enum nvdimm_fwa_result result;

	if (!nvdimm->fw_ops)
		return -EOPNOTSUPP;

	nvdimm_bus_lock(dev);
	result = nvdimm->fw_ops->activate_result(nvdimm);
	nvdimm_bus_unlock(dev);

	switch (result) {
	case NVDIMM_FWA_RESULT_NONE:
		return sprintf(buf, "none\n");
	case NVDIMM_FWA_RESULT_SUCCESS:
		return sprintf(buf, "success\n");
	case NVDIMM_FWA_RESULT_FAIL:
		return sprintf(buf, "fail\n");
	case NVDIMM_FWA_RESULT_NOTSTAGED:
		return sprintf(buf, "not_staged\n");
	case NVDIMM_FWA_RESULT_NEEDRESET:
		return sprintf(buf, "need_reset\n");
	default:
		return -ENXIO;
	}
}
static DEVICE_ATTR_ADMIN_RO(result);

static ssize_t activate_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);
	enum nvdimm_fwa_state state;

	if (!nvdimm->fw_ops)
		return -EOPNOTSUPP;

	nvdimm_bus_lock(dev);
	state = nvdimm->fw_ops->activate_state(nvdimm);
	nvdimm_bus_unlock(dev);

	switch (state) {
	case NVDIMM_FWA_IDLE:
		return sprintf(buf, "idle\n");
	case NVDIMM_FWA_BUSY:
		return sprintf(buf, "busy\n");
	case NVDIMM_FWA_ARMED:
		return sprintf(buf, "armed\n");
	default:
		return -ENXIO;
	}
}

static ssize_t activate_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t len)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);
	enum nvdimm_fwa_trigger arg;
	int rc;

	if (!nvdimm->fw_ops)
		return -EOPNOTSUPP;

	if (sysfs_streq(buf, "arm"))
		arg = NVDIMM_FWA_ARM;
	else if (sysfs_streq(buf, "disarm"))
		arg = NVDIMM_FWA_DISARM;
	else
		return -EINVAL;

	nvdimm_bus_lock(dev);
	rc = nvdimm->fw_ops->arm(nvdimm, arg);
	nvdimm_bus_unlock(dev);

	if (rc < 0)
		return rc;
	return len;
}
static DEVICE_ATTR_ADMIN_RW(activate);

static struct attribute *nvdimm_firmware_attributes[] = {
	&dev_attr_activate.attr,
	&dev_attr_result.attr,
	NULL,
};

static umode_t nvdimm_firmware_visible(struct kobject *kobj, struct attribute *a, int n)
{
	struct device *dev = container_of(kobj, typeof(*dev), kobj);
	struct nvdimm_bus *nvdimm_bus = walk_to_nvdimm_bus(dev);
	struct nvdimm_bus_descriptor *nd_desc = nvdimm_bus->nd_desc;
	struct nvdimm *nvdimm = to_nvdimm(dev);
	enum nvdimm_fwa_capability cap;

	if (!nd_desc->fw_ops)
		return 0;
	if (!nvdimm->fw_ops)
		return 0;

	nvdimm_bus_lock(dev);
	cap = nd_desc->fw_ops->capability(nd_desc);
	nvdimm_bus_unlock(dev);

	if (cap < NVDIMM_FWA_CAP_QUIESCE)
		return 0;

	return a->mode;
}

static const struct attribute_group nvdimm_firmware_attribute_group = {
	.name = "firmware",
	.attrs = nvdimm_firmware_attributes,
	.is_visible = nvdimm_firmware_visible,
};

static const struct attribute_group *nvdimm_attribute_groups[] = {
	&nd_device_attribute_group,
	&nvdimm_attribute_group,
	&nvdimm_firmware_attribute_group,
	NULL,
};

static const struct device_type nvdimm_device_type = {
	.name = "nvdimm",
	.release = nvdimm_release,
	.groups = nvdimm_attribute_groups,
};

bool is_nvdimm(struct device *dev)
{
	return dev->type == &nvdimm_device_type;
}

struct nvdimm *__nvdimm_create(struct nvdimm_bus *nvdimm_bus,
		void *provider_data, const struct attribute_group **groups,
		unsigned long flags, unsigned long cmd_mask, int num_flush,
		struct resource *flush_wpq, const char *dimm_id,
		const struct nvdimm_security_ops *sec_ops,
		const struct nvdimm_fw_ops *fw_ops)
>>>>>>> upstream/android-13
{
	struct nvdimm *nvdimm = kzalloc(sizeof(*nvdimm), GFP_KERNEL);
	struct device *dev;

	if (!nvdimm)
		return NULL;

	nvdimm->id = ida_simple_get(&dimm_ida, 0, 0, GFP_KERNEL);
	if (nvdimm->id < 0) {
		kfree(nvdimm);
		return NULL;
	}
<<<<<<< HEAD
	nvdimm->provider_data = provider_data;
=======

	nvdimm->dimm_id = dimm_id;
	nvdimm->provider_data = provider_data;
	if (noblk)
		flags |= 1 << NDD_NOBLK;
>>>>>>> upstream/android-13
	nvdimm->flags = flags;
	nvdimm->cmd_mask = cmd_mask;
	nvdimm->num_flush = num_flush;
	nvdimm->flush_wpq = flush_wpq;
	atomic_set(&nvdimm->busy, 0);
	dev = &nvdimm->dev;
	dev_set_name(dev, "nmem%d", nvdimm->id);
	dev->parent = &nvdimm_bus->dev;
	dev->type = &nvdimm_device_type;
	dev->devt = MKDEV(nvdimm_major, nvdimm->id);
	dev->groups = groups;
<<<<<<< HEAD
=======
	nvdimm->sec.ops = sec_ops;
	nvdimm->fw_ops = fw_ops;
	nvdimm->sec.overwrite_tmo = 0;
	INIT_DELAYED_WORK(&nvdimm->dwork, nvdimm_security_overwrite_query);
	/*
	 * Security state must be initialized before device_add() for
	 * attribute visibility.
	 */
	/* get security state and extended (master) state */
	nvdimm->sec.flags = nvdimm_security_flags(nvdimm, NVDIMM_USER);
	nvdimm->sec.ext_flags = nvdimm_security_flags(nvdimm, NVDIMM_MASTER);
>>>>>>> upstream/android-13
	nd_device_register(dev);

	return nvdimm;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(nvdimm_create);
=======
EXPORT_SYMBOL_GPL(__nvdimm_create);

void nvdimm_delete(struct nvdimm *nvdimm)
{
	struct device *dev = &nvdimm->dev;
	bool dev_put = false;

	/* We are shutting down. Make state frozen artificially. */
	nvdimm_bus_lock(dev);
	set_bit(NVDIMM_SECURITY_FROZEN, &nvdimm->sec.flags);
	if (test_and_clear_bit(NDD_WORK_PENDING, &nvdimm->flags))
		dev_put = true;
	nvdimm_bus_unlock(dev);
	cancel_delayed_work_sync(&nvdimm->dwork);
	if (dev_put)
		put_device(dev);
	nd_device_unregister(dev, ND_SYNC);
}
EXPORT_SYMBOL_GPL(nvdimm_delete);

static void shutdown_security_notify(void *data)
{
	struct nvdimm *nvdimm = data;

	sysfs_put(nvdimm->sec.overwrite_state);
}

int nvdimm_security_setup_events(struct device *dev)
{
	struct nvdimm *nvdimm = to_nvdimm(dev);

	if (!nvdimm->sec.flags || !nvdimm->sec.ops
			|| !nvdimm->sec.ops->overwrite)
		return 0;
	nvdimm->sec.overwrite_state = sysfs_get_dirent(dev->kobj.sd, "security");
	if (!nvdimm->sec.overwrite_state)
		return -ENOMEM;

	return devm_add_action_or_reset(dev, shutdown_security_notify, nvdimm);
}
EXPORT_SYMBOL_GPL(nvdimm_security_setup_events);

int nvdimm_in_overwrite(struct nvdimm *nvdimm)
{
	return test_bit(NDD_SECURITY_OVERWRITE, &nvdimm->flags);
}
EXPORT_SYMBOL_GPL(nvdimm_in_overwrite);

int nvdimm_security_freeze(struct nvdimm *nvdimm)
{
	int rc;

	WARN_ON_ONCE(!is_nvdimm_bus_locked(&nvdimm->dev));

	if (!nvdimm->sec.ops || !nvdimm->sec.ops->freeze)
		return -EOPNOTSUPP;

	if (!nvdimm->sec.flags)
		return -EIO;

	if (test_bit(NDD_SECURITY_OVERWRITE, &nvdimm->flags)) {
		dev_warn(&nvdimm->dev, "Overwrite operation in progress.\n");
		return -EBUSY;
	}

	rc = nvdimm->sec.ops->freeze(nvdimm);
	nvdimm->sec.flags = nvdimm_security_flags(nvdimm, NVDIMM_USER);

	return rc;
}

static unsigned long dpa_align(struct nd_region *nd_region)
{
	struct device *dev = &nd_region->dev;

	if (dev_WARN_ONCE(dev, !is_nvdimm_bus_locked(dev),
				"bus lock required for capacity provision\n"))
		return 0;
	if (dev_WARN_ONCE(dev, !nd_region->ndr_mappings || nd_region->align
				% nd_region->ndr_mappings,
				"invalid region align %#lx mappings: %d\n",
				nd_region->align, nd_region->ndr_mappings))
		return 0;
	return nd_region->align / nd_region->ndr_mappings;
}
>>>>>>> upstream/android-13

int alias_dpa_busy(struct device *dev, void *data)
{
	resource_size_t map_end, blk_start, new;
	struct blk_alloc_info *info = data;
	struct nd_mapping *nd_mapping;
	struct nd_region *nd_region;
	struct nvdimm_drvdata *ndd;
	struct resource *res;
<<<<<<< HEAD
=======
	unsigned long align;
>>>>>>> upstream/android-13
	int i;

	if (!is_memory(dev))
		return 0;

	nd_region = to_nd_region(dev);
	for (i = 0; i < nd_region->ndr_mappings; i++) {
		nd_mapping  = &nd_region->mapping[i];
		if (nd_mapping->nvdimm == info->nd_mapping->nvdimm)
			break;
	}

	if (i >= nd_region->ndr_mappings)
		return 0;

	ndd = to_ndd(nd_mapping);
	map_end = nd_mapping->start + nd_mapping->size - 1;
	blk_start = nd_mapping->start;

	/*
	 * In the allocation case ->res is set to free space that we are
	 * looking to validate against PMEM aliasing collision rules
	 * (i.e. BLK is allocated after all aliased PMEM).
	 */
	if (info->res) {
		if (info->res->start >= nd_mapping->start
				&& info->res->start < map_end)
			/* pass */;
		else
			return 0;
	}

 retry:
	/*
	 * Find the free dpa from the end of the last pmem allocation to
	 * the end of the interleave-set mapping.
	 */
<<<<<<< HEAD
	for_each_dpa_resource(ndd, res) {
		if (strncmp(res->name, "pmem", 4) != 0)
			continue;
		if ((res->start >= blk_start && res->start < map_end)
				|| (res->end >= blk_start
					&& res->end <= map_end)) {
			new = max(blk_start, min(map_end + 1, res->end + 1));
=======
	align = dpa_align(nd_region);
	if (!align)
		return 0;

	for_each_dpa_resource(ndd, res) {
		resource_size_t start, end;

		if (strncmp(res->name, "pmem", 4) != 0)
			continue;

		start = ALIGN_DOWN(res->start, align);
		end = ALIGN(res->end + 1, align) - 1;
		if ((start >= blk_start && start < map_end)
				|| (end >= blk_start && end <= map_end)) {
			new = max(blk_start, min(map_end, end) + 1);
>>>>>>> upstream/android-13
			if (new != blk_start) {
				blk_start = new;
				goto retry;
			}
		}
	}

	/* update the free space range with the probed blk_start */
	if (info->res && blk_start > info->res->start) {
		info->res->start = max(info->res->start, blk_start);
		if (info->res->start > info->res->end)
			info->res->end = info->res->start - 1;
		return 1;
	}

	info->available -= blk_start - nd_mapping->start;

	return 0;
}

/**
 * nd_blk_available_dpa - account the unused dpa of BLK region
 * @nd_mapping: container of dpa-resource-root + labels
 *
 * Unlike PMEM, BLK namespaces can occupy discontiguous DPA ranges, but
 * we arrange for them to never start at an lower dpa than the last
 * PMEM allocation in an aliased region.
 */
resource_size_t nd_blk_available_dpa(struct nd_region *nd_region)
{
	struct nvdimm_bus *nvdimm_bus = walk_to_nvdimm_bus(&nd_region->dev);
	struct nd_mapping *nd_mapping = &nd_region->mapping[0];
	struct nvdimm_drvdata *ndd = to_ndd(nd_mapping);
	struct blk_alloc_info info = {
		.nd_mapping = nd_mapping,
		.available = nd_mapping->size,
		.res = NULL,
	};
	struct resource *res;
<<<<<<< HEAD
=======
	unsigned long align;
>>>>>>> upstream/android-13

	if (!ndd)
		return 0;

	device_for_each_child(&nvdimm_bus->dev, &info, alias_dpa_busy);

	/* now account for busy blk allocations in unaliased dpa */
<<<<<<< HEAD
	for_each_dpa_resource(ndd, res) {
		if (strncmp(res->name, "blk", 3) != 0)
			continue;
		info.available -= resource_size(res);
=======
	align = dpa_align(nd_region);
	if (!align)
		return 0;
	for_each_dpa_resource(ndd, res) {
		resource_size_t start, end, size;

		if (strncmp(res->name, "blk", 3) != 0)
			continue;
		start = ALIGN_DOWN(res->start, align);
		end = ALIGN(res->end + 1, align) - 1;
		size = end - start + 1;
		if (size >= info.available)
			return 0;
		info.available -= size;
>>>>>>> upstream/android-13
	}

	return info.available;
}

/**
 * nd_pmem_max_contiguous_dpa - For the given dimm+region, return the max
 *			   contiguous unallocated dpa range.
 * @nd_region: constrain available space check to this reference region
 * @nd_mapping: container of dpa-resource-root + labels
 */
resource_size_t nd_pmem_max_contiguous_dpa(struct nd_region *nd_region,
					   struct nd_mapping *nd_mapping)
{
	struct nvdimm_drvdata *ndd = to_ndd(nd_mapping);
	struct nvdimm_bus *nvdimm_bus;
	resource_size_t max = 0;
	struct resource *res;
<<<<<<< HEAD
=======
	unsigned long align;
>>>>>>> upstream/android-13

	/* if a dimm is disabled the available capacity is zero */
	if (!ndd)
		return 0;

<<<<<<< HEAD
=======
	align = dpa_align(nd_region);
	if (!align)
		return 0;

>>>>>>> upstream/android-13
	nvdimm_bus = walk_to_nvdimm_bus(ndd->dev);
	if (__reserve_free_pmem(&nd_region->dev, nd_mapping->nvdimm))
		return 0;
	for_each_dpa_resource(ndd, res) {
<<<<<<< HEAD
		if (strcmp(res->name, "pmem-reserve") != 0)
			continue;
		if (resource_size(res) > max)
			max = resource_size(res);
=======
		resource_size_t start, end;

		if (strcmp(res->name, "pmem-reserve") != 0)
			continue;
		/* trim free space relative to current alignment setting */
		start = ALIGN(res->start, align);
		end = ALIGN_DOWN(res->end + 1, align) - 1;
		if (end < start)
			continue;
		if (end - start + 1 > max)
			max = end - start + 1;
>>>>>>> upstream/android-13
	}
	release_free_pmem(nvdimm_bus, nd_mapping);
	return max;
}

/**
 * nd_pmem_available_dpa - for the given dimm+region account unallocated dpa
 * @nd_mapping: container of dpa-resource-root + labels
 * @nd_region: constrain available space check to this reference region
 * @overlap: calculate available space assuming this level of overlap
 *
 * Validate that a PMEM label, if present, aligns with the start of an
 * interleave set and truncate the available size at the lowest BLK
 * overlap point.
 *
 * The expectation is that this routine is called multiple times as it
 * probes for the largest BLK encroachment for any single member DIMM of
 * the interleave set.  Once that value is determined the PMEM-limit for
 * the set can be established.
 */
resource_size_t nd_pmem_available_dpa(struct nd_region *nd_region,
		struct nd_mapping *nd_mapping, resource_size_t *overlap)
{
	resource_size_t map_start, map_end, busy = 0, available, blk_start;
	struct nvdimm_drvdata *ndd = to_ndd(nd_mapping);
	struct resource *res;
	const char *reason;
<<<<<<< HEAD
=======
	unsigned long align;
>>>>>>> upstream/android-13

	if (!ndd)
		return 0;

<<<<<<< HEAD
=======
	align = dpa_align(nd_region);
	if (!align)
		return 0;

>>>>>>> upstream/android-13
	map_start = nd_mapping->start;
	map_end = map_start + nd_mapping->size - 1;
	blk_start = max(map_start, map_end + 1 - *overlap);
	for_each_dpa_resource(ndd, res) {
<<<<<<< HEAD
		if (res->start >= map_start && res->start < map_end) {
			if (strncmp(res->name, "blk", 3) == 0)
				blk_start = min(blk_start,
						max(map_start, res->start));
			else if (res->end > map_end) {
				reason = "misaligned to iset";
				goto err;
			} else
				busy += resource_size(res);
		} else if (res->end >= map_start && res->end <= map_end) {
=======
		resource_size_t start, end;

		start = ALIGN_DOWN(res->start, align);
		end = ALIGN(res->end + 1, align) - 1;
		if (start >= map_start && start < map_end) {
			if (strncmp(res->name, "blk", 3) == 0)
				blk_start = min(blk_start,
						max(map_start, start));
			else if (end > map_end) {
				reason = "misaligned to iset";
				goto err;
			} else
				busy += end - start + 1;
		} else if (end >= map_start && end <= map_end) {
>>>>>>> upstream/android-13
			if (strncmp(res->name, "blk", 3) == 0) {
				/*
				 * If a BLK allocation overlaps the start of
				 * PMEM the entire interleave set may now only
				 * be used for BLK.
				 */
				blk_start = map_start;
			} else
<<<<<<< HEAD
				busy += resource_size(res);
		} else if (map_start > res->start && map_start < res->end) {
=======
				busy += end - start + 1;
		} else if (map_start > start && map_start < end) {
>>>>>>> upstream/android-13
			/* total eclipse of the mapping */
			busy += nd_mapping->size;
			blk_start = map_start;
		}
	}

	*overlap = map_end + 1 - blk_start;
	available = blk_start - map_start;
	if (busy < available)
<<<<<<< HEAD
		return available - busy;
=======
		return ALIGN_DOWN(available - busy, align);
>>>>>>> upstream/android-13
	return 0;

 err:
	nd_dbg_dpa(nd_region, ndd, res, "%s\n", reason);
	return 0;
}

void nvdimm_free_dpa(struct nvdimm_drvdata *ndd, struct resource *res)
{
	WARN_ON_ONCE(!is_nvdimm_bus_locked(ndd->dev));
	kfree(res->name);
	__release_region(&ndd->dpa, res->start, resource_size(res));
}

struct resource *nvdimm_allocate_dpa(struct nvdimm_drvdata *ndd,
		struct nd_label_id *label_id, resource_size_t start,
		resource_size_t n)
{
	char *name = kmemdup(label_id, sizeof(*label_id), GFP_KERNEL);
	struct resource *res;

	if (!name)
		return NULL;

	WARN_ON_ONCE(!is_nvdimm_bus_locked(ndd->dev));
	res = __request_region(&ndd->dpa, start, n, name, 0);
	if (!res)
		kfree(name);
	return res;
}

/**
 * nvdimm_allocated_dpa - sum up the dpa currently allocated to this label_id
 * @nvdimm: container of dpa-resource-root + labels
 * @label_id: dpa resource name of the form {pmem|blk}-<human readable uuid>
 */
resource_size_t nvdimm_allocated_dpa(struct nvdimm_drvdata *ndd,
		struct nd_label_id *label_id)
{
	resource_size_t allocated = 0;
	struct resource *res;

	for_each_dpa_resource(ndd, res)
		if (strcmp(res->name, label_id->id) == 0)
			allocated += resource_size(res);

	return allocated;
}

static int count_dimms(struct device *dev, void *c)
{
	int *count = c;

	if (is_nvdimm(dev))
		(*count)++;
	return 0;
}

int nvdimm_bus_check_dimm_count(struct nvdimm_bus *nvdimm_bus, int dimm_count)
{
	int count = 0;
	/* Flush any possible dimm registration failures */
	nd_synchronize();

	device_for_each_child(&nvdimm_bus->dev, &count, count_dimms);
	dev_dbg(&nvdimm_bus->dev, "count: %d\n", count);
	if (count != dimm_count)
		return -ENXIO;
	return 0;
}
EXPORT_SYMBOL_GPL(nvdimm_bus_check_dimm_count);

void __exit nvdimm_devs_exit(void)
{
	ida_destroy(&dimm_ida);
}
