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
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright(c) 2013-2015 Intel Corporation. All rights reserved.
>>>>>>> upstream/android-13
 */
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/sizes.h>
#include <linux/ndctl.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/nd.h>
#include "label.h"
#include "nd.h"

static int nvdimm_probe(struct device *dev)
{
	struct nvdimm_drvdata *ndd;
	int rc;

<<<<<<< HEAD
=======
	rc = nvdimm_security_setup_events(dev);
	if (rc < 0) {
		dev_err(dev, "security event setup failed: %d\n", rc);
		return rc;
	}

>>>>>>> upstream/android-13
	rc = nvdimm_check_config_data(dev);
	if (rc) {
		/* not required for non-aliased nvdimm, ex. NVDIMM-N */
		if (rc == -ENOTTY)
			rc = 0;
		return rc;
	}

<<<<<<< HEAD
	/* reset locked, to be validated below... */
=======
	/*
	 * The locked status bit reflects explicit status codes from the
	 * label reading commands, revalidate it each time the driver is
	 * activated and re-reads the label area.
	 */
>>>>>>> upstream/android-13
	nvdimm_clear_locked(dev);

	ndd = kzalloc(sizeof(*ndd), GFP_KERNEL);
	if (!ndd)
		return -ENOMEM;

	dev_set_drvdata(dev, ndd);
	ndd->dpa.name = dev_name(dev);
	ndd->ns_current = -1;
	ndd->ns_next = -1;
	ndd->dpa.start = 0;
	ndd->dpa.end = -1;
	ndd->dev = dev;
	get_device(dev);
	kref_init(&ndd->kref);

	/*
<<<<<<< HEAD
=======
	 * Attempt to unlock, if the DIMM supports security commands,
	 * otherwise the locked indication is determined by explicit
	 * status codes from the label reading commands.
	 */
	rc = nvdimm_security_unlock(dev);
	if (rc < 0)
		dev_dbg(dev, "failed to unlock dimm: %d\n", rc);


	/*
>>>>>>> upstream/android-13
	 * EACCES failures reading the namespace label-area-properties
	 * are interpreted as the DIMM capacity being locked but the
	 * namespace labels themselves being accessible.
	 */
	rc = nvdimm_init_nsarea(ndd);
	if (rc == -EACCES) {
		/*
		 * See nvdimm_namespace_common_probe() where we fail to
		 * allow namespaces to probe while the DIMM is locked,
		 * but we do allow for namespace enumeration.
		 */
		nvdimm_set_locked(dev);
		rc = 0;
	}
	if (rc)
		goto err;

	/*
	 * EACCES failures reading the namespace label-data are
	 * interpreted as the label area being locked in addition to the
	 * DIMM capacity. We fail the dimm probe to prevent regions from
	 * attempting to parse the label area.
	 */
<<<<<<< HEAD
	rc = nvdimm_init_config_data(ndd);
=======
	rc = nd_label_data_init(ndd);
>>>>>>> upstream/android-13
	if (rc == -EACCES)
		nvdimm_set_locked(dev);
	if (rc)
		goto err;

	dev_dbg(dev, "config data size: %d\n", ndd->nsarea.config_size);

	nvdimm_bus_lock(dev);
<<<<<<< HEAD
	ndd->ns_current = nd_label_validate(ndd);
	ndd->ns_next = nd_label_next_nsindex(ndd->ns_current);
	nd_label_copy(ndd, to_next_namespace_index(ndd),
			to_current_namespace_index(ndd));
	if (ndd->ns_current >= 0) {
		rc = nd_label_reserve_dpa(ndd);
		if (rc == 0)
			nvdimm_set_aliasing(dev);
=======
	if (ndd->ns_current >= 0) {
		rc = nd_label_reserve_dpa(ndd);
		if (rc == 0)
			nvdimm_set_labeling(dev);
>>>>>>> upstream/android-13
	}
	nvdimm_bus_unlock(dev);

	if (rc)
		goto err;

	return 0;

 err:
	put_ndd(ndd);
	return rc;
}

<<<<<<< HEAD
static int nvdimm_remove(struct device *dev)
{
	struct nvdimm_drvdata *ndd = dev_get_drvdata(dev);

	if (!ndd)
		return 0;

=======
static void nvdimm_remove(struct device *dev)
{
	struct nvdimm_drvdata *ndd = dev_get_drvdata(dev);

>>>>>>> upstream/android-13
	nvdimm_bus_lock(dev);
	dev_set_drvdata(dev, NULL);
	nvdimm_bus_unlock(dev);
	put_ndd(ndd);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> upstream/android-13
}

static struct nd_device_driver nvdimm_driver = {
	.probe = nvdimm_probe,
	.remove = nvdimm_remove,
	.drv = {
		.name = "nvdimm",
	},
	.type = ND_DRIVER_DIMM,
};

int __init nvdimm_init(void)
{
	return nd_driver_register(&nvdimm_driver);
}

void nvdimm_exit(void)
{
	driver_unregister(&nvdimm_driver.drv);
}

MODULE_ALIAS_ND_DEVICE(ND_DEVICE_DIMM);
