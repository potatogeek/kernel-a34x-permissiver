<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
=======
/* SPDX-License-Identifier: MIT */
>>>>>>> upstream/android-13
#ifndef __NVKM_PM_NV40_H__
#define __NVKM_PM_NV40_H__
#define nv40_pm(p) container_of((p), struct nv40_pm, base)
#include "priv.h"

struct nv40_pm {
	struct nvkm_pm base;
	u32 sequence;
};

<<<<<<< HEAD
int nv40_pm_new_(const struct nvkm_specdom *, struct nvkm_device *,
		 int index, struct nvkm_pm **);
=======
int nv40_pm_new_(const struct nvkm_specdom *, struct nvkm_device *, enum nvkm_subdev_type, int,
		 struct nvkm_pm **);
>>>>>>> upstream/android-13
extern const struct nvkm_funcdom nv40_perfctr_func;
#endif
