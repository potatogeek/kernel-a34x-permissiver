<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
=======
/* SPDX-License-Identifier: MIT */
>>>>>>> upstream/android-13
#ifndef __NVKM_NVDEC_H__
#define __NVKM_NVDEC_H__
#define nvkm_nvdec(p) container_of((p), struct nvkm_nvdec, engine)
#include <core/engine.h>
<<<<<<< HEAD

struct nvkm_nvdec {
	struct nvkm_engine engine;
	struct nvkm_falcon *falcon;
};

int gp102_nvdec_new(struct nvkm_device *, int, struct nvkm_nvdec **);
=======
#include <core/falcon.h>

struct nvkm_nvdec {
	const struct nvkm_nvdec_func *func;
	struct nvkm_engine engine;
	struct nvkm_falcon falcon;
};

int gm107_nvdec_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_nvdec **);
>>>>>>> upstream/android-13
#endif
