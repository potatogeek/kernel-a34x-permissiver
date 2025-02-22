/*
 * Copyright 2018 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#include "rootnv50.h"
#include "channv50.h"

#include <nvif/class.h>

static const struct nv50_disp_root_func
gv100_disp_root = {
	.user = {
<<<<<<< HEAD
=======
		{{-1,-1,GV100_DISP_CAPS                }, gv100_disp_caps_new },
>>>>>>> upstream/android-13
		{{0,0,GV100_DISP_CURSOR                }, gv100_disp_curs_new },
		{{0,0,GV100_DISP_WINDOW_IMM_CHANNEL_DMA}, gv100_disp_wimm_new },
		{{0,0,GV100_DISP_CORE_CHANNEL_DMA      }, gv100_disp_core_new },
		{{0,0,GV100_DISP_WINDOW_CHANNEL_DMA    }, gv100_disp_wndw_new },
		{}
	},
};

static int
gv100_disp_root_new(struct nvkm_disp *disp, const struct nvkm_oclass *oclass,
		    void *data, u32 size, struct nvkm_object **pobject)
{
	return nv50_disp_root_new_(&gv100_disp_root, disp, oclass,
				   data, size, pobject);
}

const struct nvkm_disp_oclass
gv100_disp_root_oclass = {
	.base.oclass = GV100_DISP,
	.base.minver = -1,
	.base.maxver = -1,
	.ctor = gv100_disp_root_new,
};
