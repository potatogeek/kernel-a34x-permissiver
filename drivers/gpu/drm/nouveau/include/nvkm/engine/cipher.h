<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __NVKM_CIPHER_H__
#define __NVKM_CIPHER_H__
#include <core/engine.h>
int g84_cipher_new(struct nvkm_device *, int, struct nvkm_engine **);
=======
/* SPDX-License-Identifier: MIT */
#ifndef __NVKM_CIPHER_H__
#define __NVKM_CIPHER_H__
#include <core/engine.h>
int g84_cipher_new(struct nvkm_device *, enum nvkm_subdev_type, int inst, struct nvkm_engine **);
>>>>>>> upstream/android-13
#endif
