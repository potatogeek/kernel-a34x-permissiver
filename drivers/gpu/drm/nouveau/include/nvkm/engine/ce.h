<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
=======
/* SPDX-License-Identifier: MIT */
>>>>>>> upstream/android-13
#ifndef __NVKM_CE_H__
#define __NVKM_CE_H__
#include <engine/falcon.h>

<<<<<<< HEAD
int gt215_ce_new(struct nvkm_device *, int, struct nvkm_engine **);
int gf100_ce_new(struct nvkm_device *, int, struct nvkm_engine **);
int gk104_ce_new(struct nvkm_device *, int, struct nvkm_engine **);
int gm107_ce_new(struct nvkm_device *, int, struct nvkm_engine **);
int gm200_ce_new(struct nvkm_device *, int, struct nvkm_engine **);
int gp100_ce_new(struct nvkm_device *, int, struct nvkm_engine **);
int gp102_ce_new(struct nvkm_device *, int, struct nvkm_engine **);
int gv100_ce_new(struct nvkm_device *, int, struct nvkm_engine **);
=======
int gt215_ce_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int gf100_ce_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int gk104_ce_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int gm107_ce_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int gm200_ce_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int gp100_ce_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int gp102_ce_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int gv100_ce_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
int tu102_ce_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
>>>>>>> upstream/android-13
#endif
