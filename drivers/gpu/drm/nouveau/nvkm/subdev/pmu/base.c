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

<<<<<<< HEAD
#include <core/msgqueue.h>
#include <subdev/timer.h>

=======
#include <core/firmware.h>
#include <subdev/timer.h>

bool
nvkm_pmu_fan_controlled(struct nvkm_device *device)
{
	struct nvkm_pmu *pmu = device->pmu;

	/* Internal PMU FW does not currently control fans in any way,
	 * allow SW control of fans instead.
	 */
	if (pmu && pmu->func->code.size)
		return false;

	/* Default (board-loaded, or VBIOS PMU/PREOS) PMU FW on Fermi
	 * and newer automatically control the fan speed, which would
	 * interfere with SW control.
	 */
	return (device->chipset >= 0xc0);
}

>>>>>>> upstream/android-13
void
nvkm_pmu_pgob(struct nvkm_pmu *pmu, bool enable)
{
	if (pmu && pmu->func->pgob)
		pmu->func->pgob(pmu, enable);
}

static void
nvkm_pmu_recv(struct work_struct *work)
{
	struct nvkm_pmu *pmu = container_of(work, typeof(*pmu), recv.work);
	return pmu->func->recv(pmu);
}

int
nvkm_pmu_send(struct nvkm_pmu *pmu, u32 reply[2],
	      u32 process, u32 message, u32 data0, u32 data1)
{
	if (!pmu || !pmu->func->send)
		return -ENODEV;
	return pmu->func->send(pmu, reply, process, message, data0, data1);
}

static void
nvkm_pmu_intr(struct nvkm_subdev *subdev)
{
	struct nvkm_pmu *pmu = nvkm_pmu(subdev);
	if (!pmu->func->intr)
		return;
	pmu->func->intr(pmu);
}

static int
nvkm_pmu_fini(struct nvkm_subdev *subdev, bool suspend)
{
	struct nvkm_pmu *pmu = nvkm_pmu(subdev);

	if (pmu->func->fini)
		pmu->func->fini(pmu);

	flush_work(&pmu->recv.work);
<<<<<<< HEAD
	return 0;
}

static int
=======

	reinit_completion(&pmu->wpr_ready);

	nvkm_falcon_cmdq_fini(pmu->lpq);
	nvkm_falcon_cmdq_fini(pmu->hpq);
	pmu->initmsg_received = false;
	return 0;
}

static void
>>>>>>> upstream/android-13
nvkm_pmu_reset(struct nvkm_pmu *pmu)
{
	struct nvkm_device *device = pmu->subdev.device;

	if (!pmu->func->enabled(pmu))
<<<<<<< HEAD
		return 0;

	/* Inhibit interrupts, and wait for idle. */
	nvkm_wr32(device, 0x10a014, 0x0000ffff);
	nvkm_msec(device, 2000,
		if (!nvkm_rd32(device, 0x10a04c))
			break;
	);
=======
		return;
>>>>>>> upstream/android-13

	/* Reset. */
	if (pmu->func->reset)
		pmu->func->reset(pmu);

	/* Wait for IMEM/DMEM scrubbing to be complete. */
	nvkm_msec(device, 2000,
		if (!(nvkm_rd32(device, 0x10a10c) & 0x00000006))
			break;
	);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> upstream/android-13
}

static int
nvkm_pmu_preinit(struct nvkm_subdev *subdev)
{
	struct nvkm_pmu *pmu = nvkm_pmu(subdev);
<<<<<<< HEAD
	return nvkm_pmu_reset(pmu);
=======
	nvkm_pmu_reset(pmu);
	return 0;
>>>>>>> upstream/android-13
}

static int
nvkm_pmu_init(struct nvkm_subdev *subdev)
{
	struct nvkm_pmu *pmu = nvkm_pmu(subdev);
<<<<<<< HEAD
	int ret = nvkm_pmu_reset(pmu);
	if (ret == 0 && pmu->func->init)
		ret = pmu->func->init(pmu);
	return ret;
}

static int
nvkm_pmu_oneinit(struct nvkm_subdev *subdev)
{
	struct nvkm_pmu *pmu = nvkm_pmu(subdev);
	return nvkm_falcon_v1_new(&pmu->subdev, "PMU", 0x10a000, &pmu->falcon);
=======
	struct nvkm_device *device = pmu->subdev.device;

	if (!pmu->func->init)
		return 0;

	if (pmu->func->enabled(pmu)) {
		/* Inhibit interrupts, and wait for idle. */
		nvkm_wr32(device, 0x10a014, 0x0000ffff);
		nvkm_msec(device, 2000,
			if (!nvkm_rd32(device, 0x10a04c))
				break;
		);

		nvkm_pmu_reset(pmu);
	}

	return pmu->func->init(pmu);
>>>>>>> upstream/android-13
}

static void *
nvkm_pmu_dtor(struct nvkm_subdev *subdev)
{
	struct nvkm_pmu *pmu = nvkm_pmu(subdev);
<<<<<<< HEAD
	nvkm_msgqueue_del(&pmu->queue);
	nvkm_falcon_del(&pmu->falcon);
=======
	nvkm_falcon_msgq_del(&pmu->msgq);
	nvkm_falcon_cmdq_del(&pmu->lpq);
	nvkm_falcon_cmdq_del(&pmu->hpq);
	nvkm_falcon_qmgr_del(&pmu->qmgr);
	nvkm_falcon_dtor(&pmu->falcon);
	mutex_destroy(&pmu->send.mutex);
>>>>>>> upstream/android-13
	return nvkm_pmu(subdev);
}

static const struct nvkm_subdev_func
nvkm_pmu = {
	.dtor = nvkm_pmu_dtor,
	.preinit = nvkm_pmu_preinit,
<<<<<<< HEAD
	.oneinit = nvkm_pmu_oneinit,
=======
>>>>>>> upstream/android-13
	.init = nvkm_pmu_init,
	.fini = nvkm_pmu_fini,
	.intr = nvkm_pmu_intr,
};

int
<<<<<<< HEAD
nvkm_pmu_ctor(const struct nvkm_pmu_func *func, struct nvkm_device *device,
	      int index, struct nvkm_pmu *pmu)
{
	nvkm_subdev_ctor(&nvkm_pmu, device, index, &pmu->subdev);
	pmu->func = func;
	INIT_WORK(&pmu->recv.work, nvkm_pmu_recv);
	init_waitqueue_head(&pmu->recv.wait);
=======
nvkm_pmu_ctor(const struct nvkm_pmu_fwif *fwif, struct nvkm_device *device,
	      enum nvkm_subdev_type type, int inst, struct nvkm_pmu *pmu)
{
	int ret;

	nvkm_subdev_ctor(&nvkm_pmu, device, type, inst, &pmu->subdev);

	mutex_init(&pmu->send.mutex);

	INIT_WORK(&pmu->recv.work, nvkm_pmu_recv);
	init_waitqueue_head(&pmu->recv.wait);

	fwif = nvkm_firmware_load(&pmu->subdev, fwif, "Pmu", pmu);
	if (IS_ERR(fwif))
		return PTR_ERR(fwif);

	pmu->func = fwif->func;

	ret = nvkm_falcon_ctor(pmu->func->flcn, &pmu->subdev, pmu->subdev.name,
			       0x10a000, &pmu->falcon);
	if (ret)
		return ret;

	if ((ret = nvkm_falcon_qmgr_new(&pmu->falcon, &pmu->qmgr)) ||
	    (ret = nvkm_falcon_cmdq_new(pmu->qmgr, "hpq", &pmu->hpq)) ||
	    (ret = nvkm_falcon_cmdq_new(pmu->qmgr, "lpq", &pmu->lpq)) ||
	    (ret = nvkm_falcon_msgq_new(pmu->qmgr, "msgq", &pmu->msgq)))
		return ret;

	init_completion(&pmu->wpr_ready);
>>>>>>> upstream/android-13
	return 0;
}

int
<<<<<<< HEAD
nvkm_pmu_new_(const struct nvkm_pmu_func *func, struct nvkm_device *device,
	      int index, struct nvkm_pmu **ppmu)
=======
nvkm_pmu_new_(const struct nvkm_pmu_fwif *fwif, struct nvkm_device *device,
	      enum nvkm_subdev_type type, int inst, struct nvkm_pmu **ppmu)
>>>>>>> upstream/android-13
{
	struct nvkm_pmu *pmu;
	if (!(pmu = *ppmu = kzalloc(sizeof(*pmu), GFP_KERNEL)))
		return -ENOMEM;
<<<<<<< HEAD
	return nvkm_pmu_ctor(func, device, index, *ppmu);
=======
	return nvkm_pmu_ctor(fwif, device, type, inst, *ppmu);
>>>>>>> upstream/android-13
}
