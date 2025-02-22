/*
 * Copyright 2014 Martin Peres
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
 * Authors: Martin Peres
 */
#include "priv.h"

static u32
gm107_fuse_read(struct nvkm_fuse *fuse, u32 addr)
{
	struct nvkm_device *device = fuse->subdev.device;
	return nvkm_rd32(device, 0x021100 + addr);
}

static const struct nvkm_fuse_func
gm107_fuse = {
	.read = gm107_fuse_read,
};

int
<<<<<<< HEAD
gm107_fuse_new(struct nvkm_device *device, int index, struct nvkm_fuse **pfuse)
{
	return nvkm_fuse_new_(&gm107_fuse, device, index, pfuse);
=======
gm107_fuse_new(struct nvkm_device *device, enum nvkm_subdev_type type, int inst,
	       struct nvkm_fuse **pfuse)
{
	return nvkm_fuse_new_(&gm107_fuse, device, type, inst, pfuse);
>>>>>>> upstream/android-13
}
