<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __NVKM_MSVLD_H__
#define __NVKM_MSVLD_H__
#include <engine/falcon.h>
int g98_msvld_new(struct nvkm_device *, int, struct nvkm_engine **);
int gt215_msvld_new(struct nvkm_device *, int, struct nvkm_engine **);
int mcp89_msvld_new(struct nvkm_device *, int, struct nvkm_engine **);
int gf100_msvld_new(struct nvkm_device *, int, struct nvkm_engine **);
int gk104_msvld_new(struct nvkm_device *, int, struct nvkm_engine **);
=======
/* SPDX-License-Identifier: MIT */
#ifndef __NVKM_MSVLD_H__
#define __NVKM_MSVLD_H__
#include <engine/falcon.h>
int g98_msvld_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int gt215_msvld_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int mcp89_msvld_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int gf100_msvld_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int gk104_msvld_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
>>>>>>> upstream/android-13
#endif
