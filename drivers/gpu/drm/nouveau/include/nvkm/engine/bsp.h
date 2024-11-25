<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __NVKM_BSP_H__
#define __NVKM_BSP_H__
#include <engine/xtensa.h>
int g84_bsp_new(struct nvkm_device *, int, struct nvkm_engine **);
=======
/* SPDX-License-Identifier: MIT */
#ifndef __NVKM_BSP_H__
#define __NVKM_BSP_H__
#include <engine/xtensa.h>
int g84_bsp_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
>>>>>>> upstream/android-13
#endif
