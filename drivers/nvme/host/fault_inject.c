<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * fault injection support for nvme.
 *
 * Copyright (c) 2018, Oracle and/or its affiliates
<<<<<<< HEAD
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/moduleparam.h>
#include "nvme.h"

static DECLARE_FAULT_ATTR(fail_default_attr);
/* optional fault injection attributes boot time option:
 * nvme_core.fail_request=<interval>,<probability>,<space>,<times>
 */
static char *fail_request;
module_param(fail_request, charp, 0000);

<<<<<<< HEAD
void nvme_fault_inject_init(struct nvme_ns *ns)
{
	struct dentry *dir, *parent;
	char *name = ns->disk->disk_name;
	struct nvme_fault_inject *fault_inj = &ns->fault_inject;
=======
void nvme_fault_inject_init(struct nvme_fault_inject *fault_inj,
			    const char *dev_name)
{
	struct dentry *dir, *parent;
>>>>>>> upstream/android-13
	struct fault_attr *attr = &fault_inj->attr;

	/* set default fault injection attribute */
	if (fail_request)
		setup_fault_attr(&fail_default_attr, fail_request);

	/* create debugfs directory and attribute */
<<<<<<< HEAD
	parent = debugfs_create_dir(name, NULL);
	if (!parent) {
		pr_warn("%s: failed to create debugfs directory\n", name);
=======
	parent = debugfs_create_dir(dev_name, NULL);
	if (!parent) {
		pr_warn("%s: failed to create debugfs directory\n", dev_name);
>>>>>>> upstream/android-13
		return;
	}

	*attr = fail_default_attr;
	dir = fault_create_debugfs_attr("fault_inject", parent, attr);
	if (IS_ERR(dir)) {
<<<<<<< HEAD
		pr_warn("%s: failed to create debugfs attr\n", name);
		debugfs_remove_recursive(parent);
		return;
	}
	ns->fault_inject.parent = parent;
=======
		pr_warn("%s: failed to create debugfs attr\n", dev_name);
		debugfs_remove_recursive(parent);
		return;
	}
	fault_inj->parent = parent;
>>>>>>> upstream/android-13

	/* create debugfs for status code and dont_retry */
	fault_inj->status = NVME_SC_INVALID_OPCODE;
	fault_inj->dont_retry = true;
	debugfs_create_x16("status", 0600, dir,	&fault_inj->status);
	debugfs_create_bool("dont_retry", 0600, dir, &fault_inj->dont_retry);
}

<<<<<<< HEAD
void nvme_fault_inject_fini(struct nvme_ns *ns)
{
	/* remove debugfs directories */
	debugfs_remove_recursive(ns->fault_inject.parent);
=======
void nvme_fault_inject_fini(struct nvme_fault_inject *fault_inject)
{
	/* remove debugfs directories */
	debugfs_remove_recursive(fault_inject->parent);
>>>>>>> upstream/android-13
}

void nvme_should_fail(struct request *req)
{
	struct gendisk *disk = req->rq_disk;
<<<<<<< HEAD
	struct nvme_ns *ns = NULL;
	u16 status;

	/*
	 * make sure this request is coming from a valid namespace
	 */
	if (!disk)
		return;

	ns = disk->private_data;
	if (ns && should_fail(&ns->fault_inject.attr, 1)) {
		/* inject status code and DNR bit */
		status = ns->fault_inject.status;
		if (ns->fault_inject.dont_retry)
=======
	struct nvme_fault_inject *fault_inject = NULL;
	u16 status;

	if (disk) {
		struct nvme_ns *ns = disk->private_data;

		if (ns)
			fault_inject = &ns->fault_inject;
		else
			WARN_ONCE(1, "No namespace found for request\n");
	} else {
		fault_inject = &nvme_req(req)->ctrl->fault_inject;
	}

	if (fault_inject && should_fail(&fault_inject->attr, 1)) {
		/* inject status code and DNR bit */
		status = fault_inject->status;
		if (fault_inject->dont_retry)
>>>>>>> upstream/android-13
			status |= NVME_SC_DNR;
		nvme_req(req)->status =	status;
	}
}
EXPORT_SYMBOL_GPL(nvme_should_fail);
