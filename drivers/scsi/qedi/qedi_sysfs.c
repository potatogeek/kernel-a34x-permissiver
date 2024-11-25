<<<<<<< HEAD
/*
 * QLogic iSCSI Offload Driver
 * Copyright (c) 2016 Cavium Inc.
 *
 * This software is available under the terms of the GNU General Public License
 * (GPL) Version 2, available from the file COPYING in the main directory of
 * this source tree.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * QLogic iSCSI Offload Driver
 * Copyright (c) 2016 Cavium Inc.
>>>>>>> upstream/android-13
 */

#include "qedi.h"
#include "qedi_gbl.h"
#include "qedi_iscsi.h"
#include "qedi_dbg.h"

static inline struct qedi_ctx *qedi_dev_to_hba(struct device *dev)
{
	struct Scsi_Host *shost = class_to_shost(dev);

	return iscsi_host_priv(shost);
}

<<<<<<< HEAD
static ssize_t qedi_show_port_state(struct device *dev,
				    struct device_attribute *attr,
				    char *buf)
=======
static ssize_t port_state_show(struct device *dev,
			       struct device_attribute *attr,
			       char *buf)
>>>>>>> upstream/android-13
{
	struct qedi_ctx *qedi = qedi_dev_to_hba(dev);

	if (atomic_read(&qedi->link_state) == QEDI_LINK_UP)
		return sprintf(buf, "Online\n");
	else
		return sprintf(buf, "Linkdown\n");
}

<<<<<<< HEAD
static ssize_t qedi_show_speed(struct device *dev,
			       struct device_attribute *attr, char *buf)
=======
static ssize_t speed_show(struct device *dev,
			  struct device_attribute *attr, char *buf)
>>>>>>> upstream/android-13
{
	struct qedi_ctx *qedi = qedi_dev_to_hba(dev);
	struct qed_link_output if_link;

	qedi_ops->common->get_link(qedi->cdev, &if_link);

	return sprintf(buf, "%d Gbit\n", if_link.speed / 1000);
}

<<<<<<< HEAD
static DEVICE_ATTR(port_state, 0444, qedi_show_port_state, NULL);
static DEVICE_ATTR(speed, 0444, qedi_show_speed, NULL);
=======
static DEVICE_ATTR_RO(port_state);
static DEVICE_ATTR_RO(speed);
>>>>>>> upstream/android-13

struct device_attribute *qedi_shost_attrs[] = {
	&dev_attr_port_state,
	&dev_attr_speed,
	NULL
};
