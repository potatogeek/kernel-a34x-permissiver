/*
 * Copyright (c) 2013, Cisco Systems, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>

#include <rdma/ib_user_verbs.h>
#include <rdma/ib_addr.h>

#include "usnic_common_util.h"
#include "usnic_ib.h"
#include "usnic_ib_qp_grp.h"
#include "usnic_vnic.h"
#include "usnic_ib_verbs.h"
#include "usnic_ib_sysfs.h"
#include "usnic_log.h"

<<<<<<< HEAD
static ssize_t usnic_ib_show_board(struct device *device,
					struct device_attribute *attr,
					char *buf)
{
	struct usnic_ib_dev *us_ibdev =
		container_of(device, struct usnic_ib_dev, ib_dev.dev);
=======
static ssize_t board_id_show(struct device *device,
			     struct device_attribute *attr, char *buf)
{
	struct usnic_ib_dev *us_ibdev =
		rdma_device_to_drv_device(device, struct usnic_ib_dev, ib_dev);
>>>>>>> upstream/android-13
	unsigned short subsystem_device_id;

	mutex_lock(&us_ibdev->usdev_lock);
	subsystem_device_id = us_ibdev->pdev->subsystem_device;
	mutex_unlock(&us_ibdev->usdev_lock);

<<<<<<< HEAD
	return scnprintf(buf, PAGE_SIZE, "%hu\n", subsystem_device_id);
}
=======
	return sysfs_emit(buf, "%u\n", subsystem_device_id);
}
static DEVICE_ATTR_RO(board_id);
>>>>>>> upstream/android-13

/*
 * Report the configuration for this PF
 */
static ssize_t
<<<<<<< HEAD
usnic_ib_show_config(struct device *device, struct device_attribute *attr,
			char *buf)
{
	struct usnic_ib_dev *us_ibdev;
	char *ptr;
	unsigned left;
	unsigned n;
	enum usnic_vnic_res_type res_type;

	us_ibdev = container_of(device, struct usnic_ib_dev, ib_dev.dev);

	/* Buffer space limit is 1 page */
	ptr = buf;
	left = PAGE_SIZE;
=======
config_show(struct device *device, struct device_attribute *attr, char *buf)
{
	struct usnic_ib_dev *us_ibdev =
		rdma_device_to_drv_device(device, struct usnic_ib_dev, ib_dev);
	enum usnic_vnic_res_type res_type;
	int len;
>>>>>>> upstream/android-13

	mutex_lock(&us_ibdev->usdev_lock);
	if (kref_read(&us_ibdev->vf_cnt) > 0) {
		char *busname;
<<<<<<< HEAD

=======
		char *sep = "";
>>>>>>> upstream/android-13
		/*
		 * bus name seems to come with annoying prefix.
		 * Remove it if it is predictable
		 */
		busname = us_ibdev->pdev->bus->name;
		if (strncmp(busname, "PCI Bus ", 8) == 0)
			busname += 8;

<<<<<<< HEAD
		n = scnprintf(ptr, left,
			"%s: %s:%d.%d, %s, %pM, %u VFs\n Per VF:",
			us_ibdev->ib_dev.name,
			busname,
			PCI_SLOT(us_ibdev->pdev->devfn),
			PCI_FUNC(us_ibdev->pdev->devfn),
			netdev_name(us_ibdev->netdev),
			us_ibdev->ufdev->mac,
			kref_read(&us_ibdev->vf_cnt));
		UPDATE_PTR_LEFT(n, ptr, left);

		for (res_type = USNIC_VNIC_RES_TYPE_EOL;
				res_type < USNIC_VNIC_RES_TYPE_MAX;
				res_type++) {
			if (us_ibdev->vf_res_cnt[res_type] == 0)
				continue;
			n = scnprintf(ptr, left, " %d %s%s",
				us_ibdev->vf_res_cnt[res_type],
				usnic_vnic_res_type_to_str(res_type),
				(res_type < (USNIC_VNIC_RES_TYPE_MAX - 1)) ?
				 "," : "");
			UPDATE_PTR_LEFT(n, ptr, left);
		}
		n = scnprintf(ptr, left, "\n");
		UPDATE_PTR_LEFT(n, ptr, left);
	} else {
		n = scnprintf(ptr, left, "%s: no VFs\n",
				us_ibdev->ib_dev.name);
		UPDATE_PTR_LEFT(n, ptr, left);
	}
	mutex_unlock(&us_ibdev->usdev_lock);

	return ptr - buf;
}

static ssize_t
usnic_ib_show_iface(struct device *device, struct device_attribute *attr,
			char *buf)
{
	struct usnic_ib_dev *us_ibdev;

	us_ibdev = container_of(device, struct usnic_ib_dev, ib_dev.dev);

	return scnprintf(buf, PAGE_SIZE, "%s\n",
			netdev_name(us_ibdev->netdev));
}

static ssize_t
usnic_ib_show_max_vf(struct device *device, struct device_attribute *attr,
			char *buf)
{
	struct usnic_ib_dev *us_ibdev;

	us_ibdev = container_of(device, struct usnic_ib_dev, ib_dev.dev);

	return scnprintf(buf, PAGE_SIZE, "%u\n",
			kref_read(&us_ibdev->vf_cnt));
}

static ssize_t
usnic_ib_show_qp_per_vf(struct device *device, struct device_attribute *attr,
			char *buf)
{
	struct usnic_ib_dev *us_ibdev;
	int qp_per_vf;

	us_ibdev = container_of(device, struct usnic_ib_dev, ib_dev.dev);
	qp_per_vf = max(us_ibdev->vf_res_cnt[USNIC_VNIC_RES_TYPE_WQ],
			us_ibdev->vf_res_cnt[USNIC_VNIC_RES_TYPE_RQ]);

	return scnprintf(buf, PAGE_SIZE,
				"%d\n", qp_per_vf);
}

static ssize_t
usnic_ib_show_cq_per_vf(struct device *device, struct device_attribute *attr,
			char *buf)
{
	struct usnic_ib_dev *us_ibdev;

	us_ibdev = container_of(device, struct usnic_ib_dev, ib_dev.dev);

	return scnprintf(buf, PAGE_SIZE, "%d\n",
			us_ibdev->vf_res_cnt[USNIC_VNIC_RES_TYPE_CQ]);
}

static DEVICE_ATTR(board_id, S_IRUGO, usnic_ib_show_board, NULL);
static DEVICE_ATTR(config, S_IRUGO, usnic_ib_show_config, NULL);
static DEVICE_ATTR(iface, S_IRUGO, usnic_ib_show_iface, NULL);
static DEVICE_ATTR(max_vf, S_IRUGO, usnic_ib_show_max_vf, NULL);
static DEVICE_ATTR(qp_per_vf, S_IRUGO, usnic_ib_show_qp_per_vf, NULL);
static DEVICE_ATTR(cq_per_vf, S_IRUGO, usnic_ib_show_cq_per_vf, NULL);

static struct device_attribute *usnic_class_attributes[] = {
	&dev_attr_board_id,
	&dev_attr_config,
	&dev_attr_iface,
	&dev_attr_max_vf,
	&dev_attr_qp_per_vf,
	&dev_attr_cq_per_vf,
=======
		len = sysfs_emit(buf, "%s: %s:%d.%d, %s, %pM, %u VFs\n",
				 dev_name(&us_ibdev->ib_dev.dev),
				 busname,
				 PCI_SLOT(us_ibdev->pdev->devfn),
				 PCI_FUNC(us_ibdev->pdev->devfn),
				 netdev_name(us_ibdev->netdev),
				 us_ibdev->ufdev->mac,
				 kref_read(&us_ibdev->vf_cnt));

		len += sysfs_emit_at(buf, len, " Per VF:");
		for (res_type = USNIC_VNIC_RES_TYPE_EOL;
		     res_type < USNIC_VNIC_RES_TYPE_MAX; res_type++) {
			if (us_ibdev->vf_res_cnt[res_type] == 0)
				continue;
			len += sysfs_emit_at(buf, len, "%s %d %s",
					     sep,
					     us_ibdev->vf_res_cnt[res_type],
					     usnic_vnic_res_type_to_str(res_type));
			sep = ",";
		}
		len += sysfs_emit_at(buf, len, "\n");
	} else {
		len = sysfs_emit(buf, "%s: no VFs\n",
				 dev_name(&us_ibdev->ib_dev.dev));
	}

	mutex_unlock(&us_ibdev->usdev_lock);

	return len;
}
static DEVICE_ATTR_RO(config);

static ssize_t
iface_show(struct device *device, struct device_attribute *attr, char *buf)
{
	struct usnic_ib_dev *us_ibdev =
		rdma_device_to_drv_device(device, struct usnic_ib_dev, ib_dev);

	return sysfs_emit(buf, "%s\n", netdev_name(us_ibdev->netdev));
}
static DEVICE_ATTR_RO(iface);

static ssize_t
max_vf_show(struct device *device, struct device_attribute *attr, char *buf)
{
	struct usnic_ib_dev *us_ibdev =
		rdma_device_to_drv_device(device, struct usnic_ib_dev, ib_dev);

	return sysfs_emit(buf, "%u\n", kref_read(&us_ibdev->vf_cnt));
}
static DEVICE_ATTR_RO(max_vf);

static ssize_t
qp_per_vf_show(struct device *device, struct device_attribute *attr, char *buf)
{
	struct usnic_ib_dev *us_ibdev =
		rdma_device_to_drv_device(device, struct usnic_ib_dev, ib_dev);
	int qp_per_vf;

	qp_per_vf = max(us_ibdev->vf_res_cnt[USNIC_VNIC_RES_TYPE_WQ],
			us_ibdev->vf_res_cnt[USNIC_VNIC_RES_TYPE_RQ]);

	return sysfs_emit(buf, "%d\n", qp_per_vf);
}
static DEVICE_ATTR_RO(qp_per_vf);

static ssize_t
cq_per_vf_show(struct device *device, struct device_attribute *attr, char *buf)
{
	struct usnic_ib_dev *us_ibdev =
		rdma_device_to_drv_device(device, struct usnic_ib_dev, ib_dev);

	return sysfs_emit(buf, "%d\n",
			  us_ibdev->vf_res_cnt[USNIC_VNIC_RES_TYPE_CQ]);
}
static DEVICE_ATTR_RO(cq_per_vf);

static struct attribute *usnic_class_attributes[] = {
	&dev_attr_board_id.attr,
	&dev_attr_config.attr,
	&dev_attr_iface.attr,
	&dev_attr_max_vf.attr,
	&dev_attr_qp_per_vf.attr,
	&dev_attr_cq_per_vf.attr,
	NULL
};

const struct attribute_group usnic_attr_group = {
	.attrs = usnic_class_attributes,
>>>>>>> upstream/android-13
};

struct qpn_attribute {
	struct attribute attr;
	ssize_t (*show)(struct usnic_ib_qp_grp *, char *buf);
};

/*
 * Definitions for supporting QPN entries in sysfs
 */
static ssize_t
usnic_ib_qpn_attr_show(struct kobject *kobj, struct attribute *attr, char *buf)
{
	struct usnic_ib_qp_grp *qp_grp;
	struct qpn_attribute *qpn_attr;

	qp_grp = container_of(kobj, struct usnic_ib_qp_grp, kobj);
	qpn_attr = container_of(attr, struct qpn_attribute, attr);

	return qpn_attr->show(qp_grp, buf);
}

static const struct sysfs_ops usnic_ib_qpn_sysfs_ops = {
	.show = usnic_ib_qpn_attr_show
};

#define QPN_ATTR_RO(NAME) \
struct qpn_attribute qpn_attr_##NAME = __ATTR_RO(NAME)

static ssize_t context_show(struct usnic_ib_qp_grp *qp_grp, char *buf)
{
<<<<<<< HEAD
	return scnprintf(buf, PAGE_SIZE, "0x%p\n", qp_grp->ctx);
=======
	return sysfs_emit(buf, "0x%p\n", qp_grp->ctx);
>>>>>>> upstream/android-13
}

static ssize_t summary_show(struct usnic_ib_qp_grp *qp_grp, char *buf)
{
<<<<<<< HEAD
	int i, j, n;
	int left;
	char *ptr;
	struct usnic_vnic_res_chunk *res_chunk;
	struct usnic_vnic_res *vnic_res;

	left = PAGE_SIZE;
	ptr = buf;

	n = scnprintf(ptr, left,
			"QPN: %d State: (%s) PID: %u VF Idx: %hu ",
			qp_grp->ibqp.qp_num,
			usnic_ib_qp_grp_state_to_string(qp_grp->state),
			qp_grp->owner_pid,
			usnic_vnic_get_index(qp_grp->vf->vnic));
	UPDATE_PTR_LEFT(n, ptr, left);
=======
	int i, j;
	struct usnic_vnic_res_chunk *res_chunk;
	struct usnic_vnic_res *vnic_res;
	int len;

	len = sysfs_emit(buf, "QPN: %d State: (%s) PID: %u VF Idx: %hu",
			 qp_grp->ibqp.qp_num,
			 usnic_ib_qp_grp_state_to_string(qp_grp->state),
			 qp_grp->owner_pid,
			 usnic_vnic_get_index(qp_grp->vf->vnic));
>>>>>>> upstream/android-13

	for (i = 0; qp_grp->res_chunk_list[i]; i++) {
		res_chunk = qp_grp->res_chunk_list[i];
		for (j = 0; j < res_chunk->cnt; j++) {
			vnic_res = res_chunk->res[j];
<<<<<<< HEAD
			n = scnprintf(ptr, left, "%s[%d] ",
				usnic_vnic_res_type_to_str(vnic_res->type),
				vnic_res->vnic_idx);
			UPDATE_PTR_LEFT(n, ptr, left);
		}
	}

	n = scnprintf(ptr, left, "\n");
	UPDATE_PTR_LEFT(n, ptr, left);

	return ptr - buf;
=======
			len += sysfs_emit_at(buf, len, " %s[%d]",
				usnic_vnic_res_type_to_str(vnic_res->type),
				vnic_res->vnic_idx);
		}
	}

	len += sysfs_emit_at(buf, len, "\n");

	return len;
>>>>>>> upstream/android-13
}

static QPN_ATTR_RO(context);
static QPN_ATTR_RO(summary);

static struct attribute *usnic_ib_qpn_default_attrs[] = {
	&qpn_attr_context.attr,
	&qpn_attr_summary.attr,
	NULL
};

static struct kobj_type usnic_ib_qpn_type = {
	.sysfs_ops = &usnic_ib_qpn_sysfs_ops,
	.default_attrs = usnic_ib_qpn_default_attrs
};

int usnic_ib_sysfs_register_usdev(struct usnic_ib_dev *us_ibdev)
{
<<<<<<< HEAD
	int i;
	int err;
	for (i = 0; i < ARRAY_SIZE(usnic_class_attributes); ++i) {
		err = device_create_file(&us_ibdev->ib_dev.dev,
						usnic_class_attributes[i]);
		if (err) {
			usnic_err("Failed to create device file %d for %s eith err %d",
				i, us_ibdev->ib_dev.name, err);
			return -EINVAL;
		}
	}

=======
>>>>>>> upstream/android-13
	/* create kernel object for looking at individual QPs */
	kobject_get(&us_ibdev->ib_dev.dev.kobj);
	us_ibdev->qpn_kobj = kobject_create_and_add("qpn",
			&us_ibdev->ib_dev.dev.kobj);
	if (us_ibdev->qpn_kobj == NULL) {
		kobject_put(&us_ibdev->ib_dev.dev.kobj);
		return -ENOMEM;
	}

	return 0;
}

void usnic_ib_sysfs_unregister_usdev(struct usnic_ib_dev *us_ibdev)
{
<<<<<<< HEAD
	int i;
	for (i = 0; i < ARRAY_SIZE(usnic_class_attributes); ++i) {
		device_remove_file(&us_ibdev->ib_dev.dev,
					usnic_class_attributes[i]);
	}

=======
>>>>>>> upstream/android-13
	kobject_put(us_ibdev->qpn_kobj);
}

void usnic_ib_sysfs_qpn_add(struct usnic_ib_qp_grp *qp_grp)
{
	struct usnic_ib_dev *us_ibdev;
	int err;

	us_ibdev = qp_grp->vf->pf;

	err = kobject_init_and_add(&qp_grp->kobj, &usnic_ib_qpn_type,
			kobject_get(us_ibdev->qpn_kobj),
			"%d", qp_grp->grp_id);
	if (err) {
		kobject_put(us_ibdev->qpn_kobj);
		return;
	}
}

void usnic_ib_sysfs_qpn_remove(struct usnic_ib_qp_grp *qp_grp)
{
	struct usnic_ib_dev *us_ibdev;

	us_ibdev = qp_grp->vf->pf;

	kobject_put(&qp_grp->kobj);
	kobject_put(us_ibdev->qpn_kobj);
}
