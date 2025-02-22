<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
=======
/* SPDX-License-Identifier: MIT */
>>>>>>> upstream/android-13
#ifndef __NVKM_FB_NV50_H__
#define __NVKM_FB_NV50_H__
#define nv50_fb(p) container_of((p), struct nv50_fb, base)
#include "priv.h"

struct nv50_fb {
	const struct nv50_fb_func *func;
	struct nvkm_fb base;
	struct page *r100c08_page;
	dma_addr_t r100c08;
};

struct nv50_fb_func {
	int (*ram_new)(struct nvkm_fb *, struct nvkm_ram **);
	u32 (*tags)(struct nvkm_fb *);
	u32 trap;
};

<<<<<<< HEAD
int nv50_fb_new_(const struct nv50_fb_func *, struct nvkm_device *, int index,
=======
int nv50_fb_new_(const struct nv50_fb_func *, struct nvkm_device *, enum nvkm_subdev_type, int,
>>>>>>> upstream/android-13
		 struct nvkm_fb **pfb);
#endif
