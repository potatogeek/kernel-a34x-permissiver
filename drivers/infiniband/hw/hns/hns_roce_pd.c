/*
 * Copyright (c) 2016 Hisilicon Limited.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
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
 */

#include <linux/platform_device.h>
#include <linux/pci.h>
<<<<<<< HEAD
#include <uapi/rdma/hns-abi.h>
#include "hns_roce_device.h"

static int hns_roce_pd_alloc(struct hns_roce_dev *hr_dev, unsigned long *pdn)
{
	return hns_roce_bitmap_alloc(&hr_dev->pd_bitmap, pdn) ? -ENOMEM : 0;
}

static void hns_roce_pd_free(struct hns_roce_dev *hr_dev, unsigned long pdn)
{
	hns_roce_bitmap_free(&hr_dev->pd_bitmap, pdn, BITMAP_NO_RR);
}

int hns_roce_init_pd_table(struct hns_roce_dev *hr_dev)
{
	return hns_roce_bitmap_init(&hr_dev->pd_bitmap, hr_dev->caps.num_pds,
				    hr_dev->caps.num_pds - 1,
				    hr_dev->caps.reserved_pds, 0);
}

void hns_roce_cleanup_pd_table(struct hns_roce_dev *hr_dev)
{
	hns_roce_bitmap_cleanup(&hr_dev->pd_bitmap);
}

struct ib_pd *hns_roce_alloc_pd(struct ib_device *ib_dev,
				struct ib_ucontext *context,
				struct ib_udata *udata)
{
	struct hns_roce_dev *hr_dev = to_hr_dev(ib_dev);
	struct device *dev = hr_dev->dev;
	struct hns_roce_pd *pd;
	int ret;

	pd = kmalloc(sizeof(*pd), GFP_KERNEL);
	if (!pd)
		return ERR_PTR(-ENOMEM);

	ret = hns_roce_pd_alloc(to_hr_dev(ib_dev), &pd->pdn);
	if (ret) {
		kfree(pd);
		dev_err(dev, "[alloc_pd]hns_roce_pd_alloc failed!\n");
		return ERR_PTR(ret);
	}

	if (context) {
		struct hns_roce_ib_alloc_pd_resp uresp = {.pdn = pd->pdn};

		if (ib_copy_to_udata(udata, &uresp, sizeof(uresp))) {
			hns_roce_pd_free(to_hr_dev(ib_dev), pd->pdn);
			dev_err(dev, "[alloc_pd]ib_copy_to_udata failed!\n");
			kfree(pd);
			return ERR_PTR(-EFAULT);
		}
	}

	return &pd->ibpd;
}
EXPORT_SYMBOL_GPL(hns_roce_alloc_pd);

int hns_roce_dealloc_pd(struct ib_pd *pd)
{
	hns_roce_pd_free(to_hr_dev(pd->device), to_hr_pd(pd)->pdn);
	kfree(to_hr_pd(pd));

	return 0;
}
EXPORT_SYMBOL_GPL(hns_roce_dealloc_pd);

int hns_roce_uar_alloc(struct hns_roce_dev *hr_dev, struct hns_roce_uar *uar)
{
	struct resource *res;
	int ret = 0;

	/* Using bitmap to manager UAR index */
	ret = hns_roce_bitmap_alloc(&hr_dev->uar_table.bitmap, &uar->logic_idx);
	if (ret == -1)
		return -ENOMEM;
=======
#include "hns_roce_device.h"

void hns_roce_init_pd_table(struct hns_roce_dev *hr_dev)
{
	struct hns_roce_ida *pd_ida = &hr_dev->pd_ida;

	ida_init(&pd_ida->ida);
	pd_ida->max = hr_dev->caps.num_pds - 1;
	pd_ida->min = hr_dev->caps.reserved_pds;
}

int hns_roce_alloc_pd(struct ib_pd *ibpd, struct ib_udata *udata)
{
	struct ib_device *ib_dev = ibpd->device;
	struct hns_roce_dev *hr_dev = to_hr_dev(ib_dev);
	struct hns_roce_ida *pd_ida = &hr_dev->pd_ida;
	struct hns_roce_pd *pd = to_hr_pd(ibpd);
	int ret = 0;
	int id;

	id = ida_alloc_range(&pd_ida->ida, pd_ida->min, pd_ida->max,
			     GFP_KERNEL);
	if (id < 0) {
		ibdev_err(ib_dev, "failed to alloc pd, id = %d.\n", id);
		return -ENOMEM;
	}
	pd->pdn = (unsigned long)id;

	if (udata) {
		struct hns_roce_ib_alloc_pd_resp resp = {.pdn = pd->pdn};

		ret = ib_copy_to_udata(udata, &resp,
				       min(udata->outlen, sizeof(resp)));
		if (ret) {
			ida_free(&pd_ida->ida, id);
			ibdev_err(ib_dev, "failed to copy to udata, ret = %d\n", ret);
		}
	}

	return ret;
}

int hns_roce_dealloc_pd(struct ib_pd *pd, struct ib_udata *udata)
{
	struct hns_roce_dev *hr_dev = to_hr_dev(pd->device);

	ida_free(&hr_dev->pd_ida.ida, (int)to_hr_pd(pd)->pdn);

	return 0;
}

int hns_roce_uar_alloc(struct hns_roce_dev *hr_dev, struct hns_roce_uar *uar)
{
	struct hns_roce_ida *uar_ida = &hr_dev->uar_ida;
	struct resource *res;
	int id;

	/* Using bitmap to manager UAR index */
	id = ida_alloc_range(&uar_ida->ida, uar_ida->min, uar_ida->max,
			     GFP_KERNEL);
	if (id < 0) {
		ibdev_err(&hr_dev->ib_dev, "failed to alloc uar id(%d).\n", id);
		return -ENOMEM;
	}
	uar->logic_idx = (unsigned long)id;
>>>>>>> upstream/android-13

	if (uar->logic_idx > 0 && hr_dev->caps.phy_num_uars > 1)
		uar->index = (uar->logic_idx - 1) %
			     (hr_dev->caps.phy_num_uars - 1) + 1;
	else
		uar->index = 0;

	if (!dev_is_pci(hr_dev->dev)) {
		res = platform_get_resource(hr_dev->pdev, IORESOURCE_MEM, 0);
		if (!res) {
<<<<<<< HEAD
=======
			ida_free(&uar_ida->ida, id);
>>>>>>> upstream/android-13
			dev_err(&hr_dev->pdev->dev, "memory resource not found!\n");
			return -EINVAL;
		}
		uar->pfn = ((res->start) >> PAGE_SHIFT) + uar->index;
	} else {
		uar->pfn = ((pci_resource_start(hr_dev->pci_dev, 2))
			   >> PAGE_SHIFT);
	}

	return 0;
}

<<<<<<< HEAD
void hns_roce_uar_free(struct hns_roce_dev *hr_dev, struct hns_roce_uar *uar)
{
	hns_roce_bitmap_free(&hr_dev->uar_table.bitmap, uar->logic_idx,
			     BITMAP_NO_RR);
}

int hns_roce_init_uar_table(struct hns_roce_dev *hr_dev)
{
	return hns_roce_bitmap_init(&hr_dev->uar_table.bitmap,
				    hr_dev->caps.num_uars,
				    hr_dev->caps.num_uars - 1,
				    hr_dev->caps.reserved_uars, 0);
}

void hns_roce_cleanup_uar_table(struct hns_roce_dev *hr_dev)
{
	hns_roce_bitmap_cleanup(&hr_dev->uar_table.bitmap);
=======
void hns_roce_init_uar_table(struct hns_roce_dev *hr_dev)
{
	struct hns_roce_ida *uar_ida = &hr_dev->uar_ida;

	ida_init(&uar_ida->ida);
	uar_ida->max = hr_dev->caps.num_uars - 1;
	uar_ida->min = hr_dev->caps.reserved_uars;
}

static int hns_roce_xrcd_alloc(struct hns_roce_dev *hr_dev, u32 *xrcdn)
{
	struct hns_roce_ida *xrcd_ida = &hr_dev->xrcd_ida;
	int id;

	id = ida_alloc_range(&xrcd_ida->ida, xrcd_ida->min, xrcd_ida->max,
			     GFP_KERNEL);
	if (id < 0) {
		ibdev_err(&hr_dev->ib_dev, "failed to alloc xrcdn(%d).\n", id);
		return -ENOMEM;
	}
	*xrcdn = (u32)id;

	return 0;
}

void hns_roce_init_xrcd_table(struct hns_roce_dev *hr_dev)
{
	struct hns_roce_ida *xrcd_ida = &hr_dev->xrcd_ida;

	ida_init(&xrcd_ida->ida);
	xrcd_ida->max = hr_dev->caps.num_xrcds - 1;
	xrcd_ida->min = hr_dev->caps.reserved_xrcds;
}

int hns_roce_alloc_xrcd(struct ib_xrcd *ib_xrcd, struct ib_udata *udata)
{
	struct hns_roce_dev *hr_dev = to_hr_dev(ib_xrcd->device);
	struct hns_roce_xrcd *xrcd = to_hr_xrcd(ib_xrcd);
	int ret;

	if (!(hr_dev->caps.flags & HNS_ROCE_CAP_FLAG_XRC))
		return -EINVAL;

	ret = hns_roce_xrcd_alloc(hr_dev, &xrcd->xrcdn);
	if (ret)
		return ret;

	return 0;
}

int hns_roce_dealloc_xrcd(struct ib_xrcd *ib_xrcd, struct ib_udata *udata)
{
	struct hns_roce_dev *hr_dev = to_hr_dev(ib_xrcd->device);
	u32 xrcdn = to_hr_xrcd(ib_xrcd)->xrcdn;

	ida_free(&hr_dev->xrcd_ida.ida, (int)xrcdn);

	return 0;
>>>>>>> upstream/android-13
}
