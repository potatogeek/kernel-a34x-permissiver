<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
=======
/* SPDX-License-Identifier: MIT */
>>>>>>> upstream/android-13
#ifndef __NVKM_CE_PRIV_H__
#define __NVKM_CE_PRIV_H__
#include <engine/ce.h>

void gt215_ce_intr(struct nvkm_falcon *, struct nvkm_fifo_chan *);
void gk104_ce_intr(struct nvkm_engine *);
void gp100_ce_intr(struct nvkm_engine *);
#endif
