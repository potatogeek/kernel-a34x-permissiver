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
#include "pad.h"

void
gk104_aux_stat(struct nvkm_i2c *i2c, u32 *hi, u32 *lo, u32 *rq, u32 *tx)
{
	struct nvkm_device *device = i2c->subdev.device;
	u32 intr = nvkm_rd32(device, 0x00dc60);
	u32 stat = nvkm_rd32(device, 0x00dc68) & intr, i;
	for (i = 0, *hi = *lo = *rq = *tx = 0; i < 8; i++) {
		if ((stat & (1 << (i * 4)))) *hi |= 1 << i;
		if ((stat & (2 << (i * 4)))) *lo |= 1 << i;
		if ((stat & (4 << (i * 4)))) *rq |= 1 << i;
		if ((stat & (8 << (i * 4)))) *tx |= 1 << i;
	}
	nvkm_wr32(device, 0x00dc60, intr);
}

void
gk104_aux_mask(struct nvkm_i2c *i2c, u32 type, u32 mask, u32 data)
{
	struct nvkm_device *device = i2c->subdev.device;
	u32 temp = nvkm_rd32(device, 0x00dc68), i;
	for (i = 0; i < 8; i++) {
		if (mask & (1 << i)) {
			if (!(data & (1 << i))) {
				temp &= ~(type << (i * 4));
				continue;
			}
			temp |= type << (i * 4);
		}
	}
	nvkm_wr32(device, 0x00dc68, temp);
}

static const struct nvkm_i2c_func
gk104_i2c = {
	.pad_x_new = gf119_i2c_pad_x_new,
	.pad_s_new = gf119_i2c_pad_s_new,
	.aux = 4,
	.aux_stat = gk104_aux_stat,
	.aux_mask = gk104_aux_mask,
};

int
<<<<<<< HEAD
gk104_i2c_new(struct nvkm_device *device, int index, struct nvkm_i2c **pi2c)
{
	return nvkm_i2c_new_(&gk104_i2c, device, index, pi2c);
=======
gk104_i2c_new(struct nvkm_device *device, enum nvkm_subdev_type type, int inst,
	      struct nvkm_i2c **pi2c)
{
	return nvkm_i2c_new_(&gk104_i2c, device, type, inst, pi2c);
>>>>>>> upstream/android-13
}
