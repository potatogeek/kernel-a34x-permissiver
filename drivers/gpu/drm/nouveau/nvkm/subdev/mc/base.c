/*
 * Copyright 2012 Red Hat Inc.
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
 * Authors: Ben Skeggs
 */
#include "priv.h"

#include <core/option.h>
#include <subdev/top.h>

void
nvkm_mc_unk260(struct nvkm_device *device, u32 data)
{
	struct nvkm_mc *mc = device->mc;
	if (likely(mc) && mc->func->unk260)
		mc->func->unk260(mc, data);
}

void
<<<<<<< HEAD
nvkm_mc_intr_mask(struct nvkm_device *device, enum nvkm_devidx devidx, bool en)
=======
nvkm_mc_intr_mask(struct nvkm_device *device, enum nvkm_subdev_type type, int inst, bool en)
>>>>>>> upstream/android-13
{
	struct nvkm_mc *mc = device->mc;
	const struct nvkm_mc_map *map;
	if (likely(mc) && mc->func->intr_mask) {
<<<<<<< HEAD
		u32 mask = nvkm_top_intr_mask(device, devidx);
		for (map = mc->func->intr; !mask && map->stat; map++) {
			if (map->unit == devidx)
=======
		u32 mask = nvkm_top_intr_mask(device, type, inst);
		for (map = mc->func->intr; !mask && map->stat; map++) {
			if (map->type == type && map->inst == inst)
>>>>>>> upstream/android-13
				mask = map->stat;
		}
		mc->func->intr_mask(mc, mask, en ? mask : 0);
	}
}

void
nvkm_mc_intr_unarm(struct nvkm_device *device)
{
	struct nvkm_mc *mc = device->mc;
	if (likely(mc))
		mc->func->intr_unarm(mc);
}

void
nvkm_mc_intr_rearm(struct nvkm_device *device)
{
	struct nvkm_mc *mc = device->mc;
	if (likely(mc))
		mc->func->intr_rearm(mc);
}

static u32
nvkm_mc_intr_stat(struct nvkm_mc *mc)
{
	u32 intr = mc->func->intr_stat(mc);
	if (WARN_ON_ONCE(intr == 0xffffffff))
		intr = 0; /* likely fallen off the bus */
	return intr;
}

void
nvkm_mc_intr(struct nvkm_device *device, bool *handled)
{
	struct nvkm_mc *mc = device->mc;
<<<<<<< HEAD
	struct nvkm_subdev *subdev;
	const struct nvkm_mc_map *map;
	u32 stat, intr;
	u64 subdevs;
=======
	struct nvkm_top *top = device->top;
	struct nvkm_top_device *tdev;
	struct nvkm_subdev *subdev;
	const struct nvkm_mc_map *map;
	u32 stat, intr;
>>>>>>> upstream/android-13

	if (unlikely(!mc))
		return;

<<<<<<< HEAD
	intr = nvkm_mc_intr_stat(mc);
	stat = nvkm_top_intr(device, intr, &subdevs);
	while (subdevs) {
		enum nvkm_devidx subidx = __ffs64(subdevs);
		subdev = nvkm_device_subdev(device, subidx);
		if (subdev)
			nvkm_subdev_intr(subdev);
		subdevs &= ~BIT_ULL(subidx);
=======
	stat = intr = nvkm_mc_intr_stat(mc);

	if (top) {
		list_for_each_entry(tdev, &top->device, head) {
			if (tdev->intr >= 0 && (stat & BIT(tdev->intr))) {
				subdev = nvkm_device_subdev(device, tdev->type, tdev->inst);
				if (subdev) {
					nvkm_subdev_intr(subdev);
					stat &= ~BIT(tdev->intr);
					if (!stat)
						break;
				}
			}
		}
>>>>>>> upstream/android-13
	}

	for (map = mc->func->intr; map->stat; map++) {
		if (intr & map->stat) {
<<<<<<< HEAD
			subdev = nvkm_device_subdev(device, map->unit);
=======
			subdev = nvkm_device_subdev(device, map->type, map->inst);
>>>>>>> upstream/android-13
			if (subdev)
				nvkm_subdev_intr(subdev);
			stat &= ~map->stat;
		}
	}

	if (stat)
		nvkm_error(&mc->subdev, "intr %08x\n", stat);
	*handled = intr != 0;
}

static u32
<<<<<<< HEAD
nvkm_mc_reset_mask(struct nvkm_device *device, bool isauto,
		   enum nvkm_devidx devidx)
=======
nvkm_mc_reset_mask(struct nvkm_device *device, bool isauto, enum nvkm_subdev_type type, int inst)
>>>>>>> upstream/android-13
{
	struct nvkm_mc *mc = device->mc;
	const struct nvkm_mc_map *map;
	u64 pmc_enable = 0;
	if (likely(mc)) {
<<<<<<< HEAD
		if (!(pmc_enable = nvkm_top_reset(device, devidx))) {
			for (map = mc->func->reset; map && map->stat; map++) {
				if (!isauto || !map->noauto) {
					if (map->unit == devidx) {
=======
		if (!(pmc_enable = nvkm_top_reset(device, type, inst))) {
			for (map = mc->func->reset; map && map->stat; map++) {
				if (!isauto || !map->noauto) {
					if (map->type == type && map->inst == inst) {
>>>>>>> upstream/android-13
						pmc_enable = map->stat;
						break;
					}
				}
			}
		}
	}
	return pmc_enable;
}

void
<<<<<<< HEAD
nvkm_mc_reset(struct nvkm_device *device, enum nvkm_devidx devidx)
{
	u64 pmc_enable = nvkm_mc_reset_mask(device, true, devidx);
=======
nvkm_mc_reset(struct nvkm_device *device, enum nvkm_subdev_type type, int inst)
{
	u64 pmc_enable = nvkm_mc_reset_mask(device, true, type, inst);
>>>>>>> upstream/android-13
	if (pmc_enable) {
		nvkm_mask(device, 0x000200, pmc_enable, 0x00000000);
		nvkm_mask(device, 0x000200, pmc_enable, pmc_enable);
		nvkm_rd32(device, 0x000200);
	}
}

void
<<<<<<< HEAD
nvkm_mc_disable(struct nvkm_device *device, enum nvkm_devidx devidx)
{
	u64 pmc_enable = nvkm_mc_reset_mask(device, false, devidx);
=======
nvkm_mc_disable(struct nvkm_device *device, enum nvkm_subdev_type type, int inst)
{
	u64 pmc_enable = nvkm_mc_reset_mask(device, false, type, inst);
>>>>>>> upstream/android-13
	if (pmc_enable)
		nvkm_mask(device, 0x000200, pmc_enable, 0x00000000);
}

void
<<<<<<< HEAD
nvkm_mc_enable(struct nvkm_device *device, enum nvkm_devidx devidx)
{
	u64 pmc_enable = nvkm_mc_reset_mask(device, false, devidx);
=======
nvkm_mc_enable(struct nvkm_device *device, enum nvkm_subdev_type type, int inst)
{
	u64 pmc_enable = nvkm_mc_reset_mask(device, false, type, inst);
>>>>>>> upstream/android-13
	if (pmc_enable) {
		nvkm_mask(device, 0x000200, pmc_enable, pmc_enable);
		nvkm_rd32(device, 0x000200);
	}
}

bool
<<<<<<< HEAD
nvkm_mc_enabled(struct nvkm_device *device, enum nvkm_devidx devidx)
{
	u64 pmc_enable = nvkm_mc_reset_mask(device, false, devidx);
=======
nvkm_mc_enabled(struct nvkm_device *device, enum nvkm_subdev_type type, int inst)
{
	u64 pmc_enable = nvkm_mc_reset_mask(device, false, type, inst);
>>>>>>> upstream/android-13

	return (pmc_enable != 0) &&
	       ((nvkm_rd32(device, 0x000200) & pmc_enable) == pmc_enable);
}


static int
nvkm_mc_fini(struct nvkm_subdev *subdev, bool suspend)
{
	nvkm_mc_intr_unarm(subdev->device);
	return 0;
}

static int
nvkm_mc_init(struct nvkm_subdev *subdev)
{
	struct nvkm_mc *mc = nvkm_mc(subdev);
	if (mc->func->init)
		mc->func->init(mc);
	nvkm_mc_intr_rearm(subdev->device);
	return 0;
}

static void *
nvkm_mc_dtor(struct nvkm_subdev *subdev)
{
	return nvkm_mc(subdev);
}

static const struct nvkm_subdev_func
nvkm_mc = {
	.dtor = nvkm_mc_dtor,
	.init = nvkm_mc_init,
	.fini = nvkm_mc_fini,
};

void
nvkm_mc_ctor(const struct nvkm_mc_func *func, struct nvkm_device *device,
<<<<<<< HEAD
	     int index, struct nvkm_mc *mc)
{
	nvkm_subdev_ctor(&nvkm_mc, device, index, &mc->subdev);
=======
	     enum nvkm_subdev_type type, int inst, struct nvkm_mc *mc)
{
	nvkm_subdev_ctor(&nvkm_mc, device, type, inst, &mc->subdev);
>>>>>>> upstream/android-13
	mc->func = func;
}

int
nvkm_mc_new_(const struct nvkm_mc_func *func, struct nvkm_device *device,
<<<<<<< HEAD
	     int index, struct nvkm_mc **pmc)
=======
	     enum nvkm_subdev_type type, int inst, struct nvkm_mc **pmc)
>>>>>>> upstream/android-13
{
	struct nvkm_mc *mc;
	if (!(mc = *pmc = kzalloc(sizeof(*mc), GFP_KERNEL)))
		return -ENOMEM;
<<<<<<< HEAD
	nvkm_mc_ctor(func, device, index, *pmc);
=======
	nvkm_mc_ctor(func, device, type, inst, *pmc);
>>>>>>> upstream/android-13
	return 0;
}
