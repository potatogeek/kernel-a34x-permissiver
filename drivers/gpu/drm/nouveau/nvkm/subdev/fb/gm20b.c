/*
 * Copyright (c) 2016, NVIDIA CORPORATION. All rights reserved.
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
#include "priv.h"
#include "gf100.h"

/* GM20B's FB is similar to GM200, but without the ability to allocate VRAM */
static const struct nvkm_fb_func
gm20b_fb = {
	.dtor = gf100_fb_dtor,
	.oneinit = gf100_fb_oneinit,
	.init = gm200_fb_init,
	.init_page = gm200_fb_init_page,
	.intr = gf100_fb_intr,
	.default_bigpage = 0 /* per-instance. */,
};

int
<<<<<<< HEAD
gm20b_fb_new(struct nvkm_device *device, int index, struct nvkm_fb **pfb)
{
	return gf100_fb_new_(&gm20b_fb, device, index, pfb);
=======
gm20b_fb_new(struct nvkm_device *device, enum nvkm_subdev_type type, int inst, struct nvkm_fb **pfb)
{
	return gf100_fb_new_(&gm20b_fb, device, type, inst, pfb);
>>>>>>> upstream/android-13
}
