<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __NVKM_MSPDEC_H__
#define __NVKM_MSPDEC_H__
#include <engine/falcon.h>
int g98_mspdec_new(struct nvkm_device *, int, struct nvkm_engine **);
int gt215_mspdec_new(struct nvkm_device *, int, struct nvkm_engine **);
int gf100_mspdec_new(struct nvkm_device *, int, struct nvkm_engine **);
int gk104_mspdec_new(struct nvkm_device *, int, struct nvkm_engine **);
=======
/* SPDX-License-Identifier: MIT */
#ifndef __NVKM_MSPDEC_H__
#define __NVKM_MSPDEC_H__
#include <engine/falcon.h>
int g98_mspdec_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int gt215_mspdec_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int gf100_mspdec_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int gk104_mspdec_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
>>>>>>> upstream/android-13
#endif
