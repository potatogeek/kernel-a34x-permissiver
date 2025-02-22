/*
 * Copyright (c) 2017, NVIDIA CORPORATION. All rights reserved.
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
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include "gk104.h"
#include "changk104.h"

#include <nvif/class.h>

static const struct gk104_fifo_func
gp10b_fifo = {
<<<<<<< HEAD
	.init_pbdma_timeout = gk208_fifo_init_pbdma_timeout,
=======
	.intr.fault = gp100_fifo_intr_fault,
	.pbdma = &gm200_fifo_pbdma,
>>>>>>> upstream/android-13
	.fault.access = gk104_fifo_fault_access,
	.fault.engine = gp100_fifo_fault_engine,
	.fault.reason = gk104_fifo_fault_reason,
	.fault.hubclient = gk104_fifo_fault_hubclient,
	.fault.gpcclient = gk104_fifo_fault_gpcclient,
	.runlist = &gm107_fifo_runlist,
	.chan = {{0,0,PASCAL_CHANNEL_GPFIFO_A}, gk104_fifo_gpfifo_new },
	.cgrp_force = true,
};

int
<<<<<<< HEAD
gp10b_fifo_new(struct nvkm_device *device, int index, struct nvkm_fifo **pfifo)
{
	return gk104_fifo_new_(&gp10b_fifo, device, index, 512, pfifo);
=======
gp10b_fifo_new(struct nvkm_device *device, enum nvkm_subdev_type type, int inst,
	       struct nvkm_fifo **pfifo)
{
	return gk104_fifo_new_(&gp10b_fifo, device, type, inst, 512, pfifo);
>>>>>>> upstream/android-13
}
