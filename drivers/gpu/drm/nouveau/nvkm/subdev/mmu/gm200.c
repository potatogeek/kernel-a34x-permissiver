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
#include "mem.h"
#include "vmm.h"

#include <subdev/fb.h>

#include <nvif/class.h>

const u8 *
<<<<<<< HEAD
gm200_mmu_kind(struct nvkm_mmu *mmu, int *count)
=======
gm200_mmu_kind(struct nvkm_mmu *mmu, int *count, u8 *invalid)
>>>>>>> upstream/android-13
{
	static const u8
	kind[256] = {
		0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0x01, /* 0x00 */
		0x01, 0x01, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0x11, 0xff, 0xff, 0xff, 0xff, 0xff, 0x11, /* 0x10 */
		0x11, 0x11, 0x11, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x26, 0x27, /* 0x20 */
		0x28, 0x29, 0x2a, 0x2b, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x30 */
		0xff, 0xff, 0x26, 0x27, 0x28, 0x29, 0x26, 0x27,
		0x28, 0x29, 0xff, 0xff, 0xff, 0xff, 0x46, 0xff, /* 0x40 */
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0x46, 0x46, 0x46, 0x46, 0xff, 0xff, 0xff, /* 0x50 */
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x60 */
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x70 */
		0xff, 0xff, 0xff, 0x7b, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7b, 0x7b, /* 0x80 */
		0x7b, 0x7b, 0xff, 0x8b, 0x8c, 0x8d, 0x8e, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0x90 */
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0x8b, 0x8c, 0x8d, 0x8e, 0xa7, /* 0xa0 */
		0xa8, 0xa9, 0xaa, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xb0 */
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa7,
		0xa8, 0xa9, 0xaa, 0xc3, 0xff, 0xff, 0xff, 0xff, /* 0xc0 */
		0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xc3, 0xc3,
		0xc3, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* 0xd0 */
		0xfe, 0xff, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe,
		0xfe, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, /* 0xe0 */
		0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xfe, 0xff,
		0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, /* 0xf0 */
		0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xfd, 0xfe, 0xff
	};
	*count = ARRAY_SIZE(kind);
<<<<<<< HEAD
=======
	*invalid = 0xff;
>>>>>>> upstream/android-13
	return kind;
}

static const struct nvkm_mmu_func
gm200_mmu = {
	.dma_bits = 40,
	.mmu = {{ -1, -1, NVIF_CLASS_MMU_GF100}},
	.mem = {{ -1,  0, NVIF_CLASS_MEM_GF100}, gf100_mem_new, gf100_mem_map },
	.vmm = {{ -1,  0, NVIF_CLASS_VMM_GM200}, gm200_vmm_new },
	.kind = gm200_mmu_kind,
	.kind_sys = true,
};

static const struct nvkm_mmu_func
gm200_mmu_fixed = {
	.dma_bits = 40,
	.mmu = {{ -1, -1, NVIF_CLASS_MMU_GF100}},
	.mem = {{ -1,  0, NVIF_CLASS_MEM_GF100}, gf100_mem_new, gf100_mem_map },
	.vmm = {{ -1, -1, NVIF_CLASS_VMM_GM200}, gm200_vmm_new_fixed },
	.kind = gm200_mmu_kind,
	.kind_sys = true,
};

int
<<<<<<< HEAD
gm200_mmu_new(struct nvkm_device *device, int index, struct nvkm_mmu **pmmu)
{
	if (device->fb->page)
		return nvkm_mmu_new_(&gm200_mmu_fixed, device, index, pmmu);
	return nvkm_mmu_new_(&gm200_mmu, device, index, pmmu);
=======
gm200_mmu_new(struct nvkm_device *device, enum nvkm_subdev_type type, int inst,
	      struct nvkm_mmu **pmmu)
{
	if (device->fb->page)
		return nvkm_mmu_new_(&gm200_mmu_fixed, device, type, inst, pmmu);
	return nvkm_mmu_new_(&gm200_mmu, device, type, inst, pmmu);
>>>>>>> upstream/android-13
}
