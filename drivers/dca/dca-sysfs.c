<<<<<<< HEAD
/*
 * Copyright(c) 2007 - 2009 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called COPYING.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright(c) 2007 - 2009 Intel Corporation. All rights reserved.
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/idr.h>
#include <linux/kdev_t.h>
#include <linux/err.h>
#include <linux/dca.h>
#include <linux/gfp.h>
#include <linux/export.h>

static struct class *dca_class;
static struct idr dca_idr;
static spinlock_t dca_idr_lock;

int dca_sysfs_add_req(struct dca_provider *dca, struct device *dev, int slot)
{
	struct device *cd;
	static int req_count;

	cd = device_create(dca_class, dca->cd, MKDEV(0, slot + 1), NULL,
			   "requester%d", req_count++);
<<<<<<< HEAD
	if (IS_ERR(cd))
		return PTR_ERR(cd);
	return 0;
=======
	return PTR_ERR_OR_ZERO(cd);
>>>>>>> upstream/android-13
}

void dca_sysfs_remove_req(struct dca_provider *dca, int slot)
{
	device_destroy(dca_class, MKDEV(0, slot + 1));
}

int dca_sysfs_add_provider(struct dca_provider *dca, struct device *dev)
{
	struct device *cd;
	int ret;

	idr_preload(GFP_KERNEL);
	spin_lock(&dca_idr_lock);

	ret = idr_alloc(&dca_idr, dca, 0, 0, GFP_NOWAIT);
	if (ret >= 0)
		dca->id = ret;

	spin_unlock(&dca_idr_lock);
	idr_preload_end();
	if (ret < 0)
		return ret;

	cd = device_create(dca_class, dev, MKDEV(0, 0), NULL, "dca%d", dca->id);
	if (IS_ERR(cd)) {
		spin_lock(&dca_idr_lock);
		idr_remove(&dca_idr, dca->id);
		spin_unlock(&dca_idr_lock);
		return PTR_ERR(cd);
	}
	dca->cd = cd;
	return 0;
}

void dca_sysfs_remove_provider(struct dca_provider *dca)
{
	device_unregister(dca->cd);
	dca->cd = NULL;
	spin_lock(&dca_idr_lock);
	idr_remove(&dca_idr, dca->id);
	spin_unlock(&dca_idr_lock);
}

int __init dca_sysfs_init(void)
{
	idr_init(&dca_idr);
	spin_lock_init(&dca_idr_lock);

	dca_class = class_create(THIS_MODULE, "dca");
	if (IS_ERR(dca_class)) {
		idr_destroy(&dca_idr);
		return PTR_ERR(dca_class);
	}
	return 0;
}

void __exit dca_sysfs_exit(void)
{
	class_destroy(dca_class);
	idr_destroy(&dca_idr);
}

