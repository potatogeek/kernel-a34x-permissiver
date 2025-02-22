/*
 * Copyright 2013 Red Hat Inc.
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
#include "fuc/gf100.fuc3.h"

#include <subdev/mc.h>

void
gf100_pmu_reset(struct nvkm_pmu *pmu)
{
	struct nvkm_device *device = pmu->subdev.device;
<<<<<<< HEAD
	nvkm_mc_disable(device, NVKM_SUBDEV_PMU);
	nvkm_mc_enable(device, NVKM_SUBDEV_PMU);
=======
	nvkm_mc_disable(device, NVKM_SUBDEV_PMU, 0);
	nvkm_mc_enable(device, NVKM_SUBDEV_PMU, 0);
>>>>>>> upstream/android-13
}

bool
gf100_pmu_enabled(struct nvkm_pmu *pmu)
{
<<<<<<< HEAD
	return nvkm_mc_enabled(pmu->subdev.device, NVKM_SUBDEV_PMU);
=======
	return nvkm_mc_enabled(pmu->subdev.device, NVKM_SUBDEV_PMU, 0);
>>>>>>> upstream/android-13
}

static const struct nvkm_pmu_func
gf100_pmu = {
<<<<<<< HEAD
=======
	.flcn = &gt215_pmu_flcn,
>>>>>>> upstream/android-13
	.code.data = gf100_pmu_code,
	.code.size = sizeof(gf100_pmu_code),
	.data.data = gf100_pmu_data,
	.data.size = sizeof(gf100_pmu_data),
	.enabled = gf100_pmu_enabled,
	.reset = gf100_pmu_reset,
	.init = gt215_pmu_init,
	.fini = gt215_pmu_fini,
	.intr = gt215_pmu_intr,
	.send = gt215_pmu_send,
	.recv = gt215_pmu_recv,
};

int
<<<<<<< HEAD
gf100_pmu_new(struct nvkm_device *device, int index, struct nvkm_pmu **ppmu)
{
	return nvkm_pmu_new_(&gf100_pmu, device, index, ppmu);
=======
gf100_pmu_nofw(struct nvkm_pmu *pmu, int ver, const struct nvkm_pmu_fwif *fwif)
{
	return 0;
}

static const struct nvkm_pmu_fwif
gf100_pmu_fwif[] = {
	{ -1, gf100_pmu_nofw, &gf100_pmu },
	{}
};

int
gf100_pmu_new(struct nvkm_device *device, enum nvkm_subdev_type type, int inst,
	      struct nvkm_pmu **ppmu)
{
	return nvkm_pmu_new_(gf100_pmu_fwif, device, type, inst, ppmu);
>>>>>>> upstream/android-13
}
