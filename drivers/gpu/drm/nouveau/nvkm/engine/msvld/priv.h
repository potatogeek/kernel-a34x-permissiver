<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
=======
/* SPDX-License-Identifier: MIT */
>>>>>>> upstream/android-13
#ifndef __NVKM_MSVLD_PRIV_H__
#define __NVKM_MSVLD_PRIV_H__
#include <engine/msvld.h>

<<<<<<< HEAD
int nvkm_msvld_new_(const struct nvkm_falcon_func *, struct nvkm_device *,
		    int index, struct nvkm_engine **);
=======
int nvkm_msvld_new_(const struct nvkm_falcon_func *, struct nvkm_device *, enum nvkm_subdev_type,
		    int, struct nvkm_engine **);
>>>>>>> upstream/android-13

void g98_msvld_init(struct nvkm_falcon *);

void gf100_msvld_init(struct nvkm_falcon *);
#endif
