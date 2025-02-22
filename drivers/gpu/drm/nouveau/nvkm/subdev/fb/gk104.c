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
 *          Lyude Paul
 */
#include "gk104.h"
#include "gf100.h"
#include "ram.h"

/*
 *******************************************************************************
 * PGRAPH registers for clockgating
 *******************************************************************************
 */
const struct nvkm_therm_clkgate_init
gk104_fb_clkgate_blcg_init_unk_0[] = {
	{ 0x100d10, 1, 0x0000c244 },
	{ 0x100d30, 1, 0x0000c242 },
	{ 0x100d3c, 1, 0x00000242 },
	{ 0x100d48, 1, 0x00000242 },
	{ 0x100d1c, 1, 0x00000042 },
	{}
};

const struct nvkm_therm_clkgate_init
gk104_fb_clkgate_blcg_init_vm_0[] = {
	{ 0x100c98, 1, 0x00000242 },
	{}
};

const struct nvkm_therm_clkgate_init
gk104_fb_clkgate_blcg_init_main_0[] = {
	{ 0x10f000, 1, 0x00000042 },
	{ 0x17e030, 1, 0x00000044 },
	{ 0x17e040, 1, 0x00000044 },
	{}
};

const struct nvkm_therm_clkgate_init
gk104_fb_clkgate_blcg_init_bcast_0[] = {
	{ 0x17ea60, 4, 0x00000044 },
	{}
};

static const struct nvkm_therm_clkgate_pack
gk104_fb_clkgate_pack[] = {
	{ gk104_fb_clkgate_blcg_init_unk_0 },
	{ gk104_fb_clkgate_blcg_init_vm_0 },
	{ gk104_fb_clkgate_blcg_init_main_0 },
	{ gk104_fb_clkgate_blcg_init_bcast_0 },
	{}
};

static const struct nvkm_fb_func
gk104_fb = {
	.dtor = gf100_fb_dtor,
	.oneinit = gf100_fb_oneinit,
	.init = gf100_fb_init,
	.init_page = gf100_fb_init_page,
	.intr = gf100_fb_intr,
	.ram_new = gk104_ram_new,
	.default_bigpage = 17,
	.clkgate_pack = gk104_fb_clkgate_pack,
};

int
<<<<<<< HEAD
gk104_fb_new(struct nvkm_device *device, int index, struct nvkm_fb **pfb)
{
	return gf100_fb_new_(&gk104_fb, device, index, pfb);
=======
gk104_fb_new(struct nvkm_device *device, enum nvkm_subdev_type type, int inst, struct nvkm_fb **pfb)
{
	return gf100_fb_new_(&gk104_fb, device, type, inst, pfb);
>>>>>>> upstream/android-13
}
