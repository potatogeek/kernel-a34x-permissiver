/*
 * Copyright 2017 Red Hat Inc.
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
 */
#include "gf100.h"

static const struct nvkm_bar_func
gm20b_bar_func = {
	.dtor = gf100_bar_dtor,
	.oneinit = gf100_bar_oneinit,
	.bar1.init = gf100_bar_bar1_init,
<<<<<<< HEAD
	.bar1.fini = gf100_bar_bar1_fini,
=======
>>>>>>> upstream/android-13
	.bar1.wait = gm107_bar_bar1_wait,
	.bar1.vmm = gf100_bar_bar1_vmm,
	.flush = g84_bar_flush,
};

int
<<<<<<< HEAD
gm20b_bar_new(struct nvkm_device *device, int index, struct nvkm_bar **pbar)
{
	int ret = gf100_bar_new_(&gm20b_bar_func, device, index, pbar);
=======
gm20b_bar_new(struct nvkm_device *device, enum nvkm_subdev_type type, int inst,
	      struct nvkm_bar **pbar)
{
	int ret = gf100_bar_new_(&gm20b_bar_func, device, type, inst, pbar);
>>>>>>> upstream/android-13
	if (ret == 0)
		(*pbar)->iomap_uncached = true;
	return ret;
}
