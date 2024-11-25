<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
=======
/* SPDX-License-Identifier: MIT */
>>>>>>> upstream/android-13
#ifndef __NVKM_XTENSA_H__
#define __NVKM_XTENSA_H__
#define nvkm_xtensa(p) container_of((p), struct nvkm_xtensa, engine)
#include <core/engine.h>

struct nvkm_xtensa {
	const struct nvkm_xtensa_func *func;
	u32 addr;
	struct nvkm_engine engine;

	struct nvkm_memory *gpu_fw;
};

int nvkm_xtensa_new_(const struct nvkm_xtensa_func *, struct nvkm_device *,
<<<<<<< HEAD
		     int index, bool enable, u32 addr, struct nvkm_engine **);
=======
		     enum nvkm_subdev_type, int, bool enable, u32 addr, struct nvkm_engine **);
>>>>>>> upstream/android-13

struct nvkm_xtensa_func {
	u32 fifo_val;
	u32 unkd28;
	struct nvkm_sclass sclass[];
};
#endif
