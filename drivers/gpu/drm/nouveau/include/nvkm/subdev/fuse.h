<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
=======
/* SPDX-License-Identifier: MIT */
>>>>>>> upstream/android-13
#ifndef __NVKM_FUSE_H__
#define __NVKM_FUSE_H__
#include <core/subdev.h>

struct nvkm_fuse {
	const struct nvkm_fuse_func *func;
	struct nvkm_subdev subdev;
	spinlock_t lock;
};

u32 nvkm_fuse_read(struct nvkm_fuse *, u32 addr);

<<<<<<< HEAD
int nv50_fuse_new(struct nvkm_device *, int, struct nvkm_fuse **);
int gf100_fuse_new(struct nvkm_device *, int, struct nvkm_fuse **);
int gm107_fuse_new(struct nvkm_device *, int, struct nvkm_fuse **);
=======
int nv50_fuse_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_fuse **);
int gf100_fuse_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_fuse **);
int gm107_fuse_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_fuse **);
>>>>>>> upstream/android-13
#endif
