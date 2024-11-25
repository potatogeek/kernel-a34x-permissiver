<<<<<<< HEAD
/* Copyright (c) 2014 The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (c) 2014 The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */
#ifndef __A4XX_GPU_H__
#define __A4XX_GPU_H__

#include "adreno_gpu.h"

/* arrg, somehow fb.h is getting pulled in: */
#undef ROP_COPY
#undef ROP_XOR

#include "a4xx.xml.h"

struct a4xx_gpu {
	struct adreno_gpu base;

	/* if OCMEM is used for GMEM: */
<<<<<<< HEAD
	uint32_t ocmem_base;
	void *ocmem_hdl;
=======
	struct adreno_ocmem ocmem;
>>>>>>> upstream/android-13
};
#define to_a4xx_gpu(x) container_of(x, struct a4xx_gpu, base)

#endif /* __A4XX_GPU_H__ */
