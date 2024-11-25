/*
 * Copyright 2016 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: Ben Skeggs <bskeggs@redhat.com>
 */
#include "priv.h"

struct nvkm_top_device *
nvkm_top_device_new(struct nvkm_top *top)
{
	struct nvkm_top_device *info = kmalloc(sizeof(*info), GFP_KERNEL);
	if (info) {
<<<<<<< HEAD
		info->index = NVKM_SUBDEV_NR;
=======
		info->type = NVKM_SUBDEV_NR;
		info->inst = -1;
>>>>>>> upstream/android-13
		info->addr = 0;
		info->fault = -1;
		info->engine = -1;
		info->runlist = -1;
		info->reset = -1;
		info->intr = -1;
		list_add_tail(&info->head, &top->device);
	}
	return info;
}

u32
<<<<<<< HEAD
nvkm_top_reset(struct nvkm_device *device, enum nvkm_devidx index)
=======
nvkm_top_addr(struct nvkm_device *device, enum nvkm_subdev_type type, int inst)
>>>>>>> upstream/android-13
{
	struct nvkm_top *top = device->top;
	struct nvkm_top_device *info;

	if (top) {
		list_for_each_entry(info, &top->device, head) {
<<<<<<< HEAD
			if (info->index == index && info->reset >= 0)
=======
			if (info->type == type && info->inst == inst)
				return info->addr;
		}
	}

	return 0;
}

u32
nvkm_top_reset(struct nvkm_device *device, enum nvkm_subdev_type type, int inst)
{
	struct nvkm_top *top = device->top;
	struct nvkm_top_device *info;

	if (top) {
		list_for_each_entry(info, &top->device, head) {
			if (info->type == type && info->inst == inst && info->reset >= 0)
>>>>>>> upstream/android-13
				return BIT(info->reset);
		}
	}

	return 0;
}

u32
<<<<<<< HEAD
nvkm_top_intr_mask(struct nvkm_device *device, enum nvkm_devidx devidx)
=======
nvkm_top_intr_mask(struct nvkm_device *device, enum nvkm_subdev_type type, int inst)
>>>>>>> upstream/android-13
{
	struct nvkm_top *top = device->top;
	struct nvkm_top_device *info;

	if (top) {
		list_for_each_entry(info, &top->device, head) {
<<<<<<< HEAD
			if (info->index == devidx && info->intr >= 0)
=======
			if (info->type == type && info->inst == inst && info->intr >= 0)
>>>>>>> upstream/android-13
				return BIT(info->intr);
		}
	}

	return 0;
}

<<<<<<< HEAD
u32
nvkm_top_intr(struct nvkm_device *device, u32 intr, u64 *psubdevs)
{
	struct nvkm_top *top = device->top;
	struct nvkm_top_device *info;
	u64 subdevs = 0;
	u32 handled = 0;

	if (top) {
		list_for_each_entry(info, &top->device, head) {
			if (info->index != NVKM_SUBDEV_NR && info->intr >= 0) {
				if (intr & BIT(info->intr)) {
					subdevs |= BIT_ULL(info->index);
					handled |= BIT(info->intr);
				}
			}
		}
	}

	*psubdevs = subdevs;
	return intr & ~handled;
}

int
nvkm_top_fault_id(struct nvkm_device *device, enum nvkm_devidx devidx)
=======
int
nvkm_top_fault_id(struct nvkm_device *device, enum nvkm_subdev_type type, int inst)
>>>>>>> upstream/android-13
{
	struct nvkm_top *top = device->top;
	struct nvkm_top_device *info;

	list_for_each_entry(info, &top->device, head) {
<<<<<<< HEAD
		if (info->index == devidx && info->fault >= 0)
=======
		if (info->type == type && info->inst == inst && info->fault >= 0)
>>>>>>> upstream/android-13
			return info->fault;
	}

	return -ENOENT;
}

<<<<<<< HEAD
enum nvkm_devidx
=======
struct nvkm_subdev *
>>>>>>> upstream/android-13
nvkm_top_fault(struct nvkm_device *device, int fault)
{
	struct nvkm_top *top = device->top;
	struct nvkm_top_device *info;

	list_for_each_entry(info, &top->device, head) {
		if (info->fault == fault)
<<<<<<< HEAD
			return info->index;
	}

	return NVKM_SUBDEV_NR;
}

enum nvkm_devidx
nvkm_top_engine(struct nvkm_device *device, int index, int *runl, int *engn)
{
	struct nvkm_top *top = device->top;
	struct nvkm_top_device *info;
	int n = 0;

	list_for_each_entry(info, &top->device, head) {
		if (info->engine >= 0 && info->runlist >= 0 && n++ == index) {
			*runl = info->runlist;
			*engn = info->engine;
			return info->index;
		}
	}

	return -ENODEV;
=======
			return nvkm_device_subdev(device, info->type, info->inst);
	}

	return NULL;
>>>>>>> upstream/android-13
}

static int
nvkm_top_oneinit(struct nvkm_subdev *subdev)
{
	struct nvkm_top *top = nvkm_top(subdev);
	return top->func->oneinit(top);
}

static void *
nvkm_top_dtor(struct nvkm_subdev *subdev)
{
	struct nvkm_top *top = nvkm_top(subdev);
	struct nvkm_top_device *info, *temp;

	list_for_each_entry_safe(info, temp, &top->device, head) {
		list_del(&info->head);
		kfree(info);
	}

	return top;
}

static const struct nvkm_subdev_func
nvkm_top = {
	.dtor = nvkm_top_dtor,
	.oneinit = nvkm_top_oneinit,
};

int
nvkm_top_new_(const struct nvkm_top_func *func, struct nvkm_device *device,
<<<<<<< HEAD
	      int index, struct nvkm_top **ptop)
=======
	      enum nvkm_subdev_type type, int inst, struct nvkm_top **ptop)
>>>>>>> upstream/android-13
{
	struct nvkm_top *top;
	if (!(top = *ptop = kzalloc(sizeof(*top), GFP_KERNEL)))
		return -ENOMEM;
<<<<<<< HEAD
	nvkm_subdev_ctor(&nvkm_top, device, index, &top->subdev);
=======
	nvkm_subdev_ctor(&nvkm_top, device, type, inst, &top->subdev);
>>>>>>> upstream/android-13
	top->func = func;
	INIT_LIST_HEAD(&top->device);
	return 0;
}
