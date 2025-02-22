/*
 * Copyright 2013 Red Hat Inc.
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
 *
 * Authors: Ben Skeggs <bskeggs@redhat.com>
 */
#include "gf100.h"
#include "ctxgf100.h"

#include <nvif/class.h>

/*******************************************************************************
 * PGRAPH register lists
 ******************************************************************************/

static const struct gf100_gr_init
gk110b_gr_init_l1c_0[] = {
	{ 0x419c98,   1, 0x04, 0x00000000 },
	{ 0x419ca8,   1, 0x04, 0x00000000 },
	{ 0x419cb0,   1, 0x04, 0x09000000 },
	{ 0x419cb4,   1, 0x04, 0x00000000 },
	{ 0x419cb8,   1, 0x04, 0x00b08bea },
	{ 0x419c84,   1, 0x04, 0x00010384 },
	{ 0x419cbc,   1, 0x04, 0x281b3646 },
	{ 0x419cc0,   2, 0x04, 0x00000000 },
	{ 0x419c80,   1, 0x04, 0x00020230 },
	{ 0x419ccc,   2, 0x04, 0x00000000 },
	{}
};

static const struct gf100_gr_init
gk110b_gr_init_sm_0[] = {
	{ 0x419e00,   1, 0x04, 0x00000080 },
	{ 0x419ea0,   1, 0x04, 0x00000000 },
	{ 0x419ee4,   1, 0x04, 0x00000000 },
	{ 0x419ea4,   1, 0x04, 0x00000100 },
	{ 0x419ea8,   1, 0x04, 0x00000000 },
	{ 0x419eb4,   1, 0x04, 0x00000000 },
	{ 0x419ebc,   2, 0x04, 0x00000000 },
	{ 0x419edc,   1, 0x04, 0x00000000 },
	{ 0x419f00,   1, 0x04, 0x00000000 },
	{ 0x419ed0,   1, 0x04, 0x00002616 },
	{ 0x419f74,   1, 0x04, 0x00015555 },
	{ 0x419f80,   4, 0x04, 0x00000000 },
	{}
};

static const struct gf100_gr_pack
gk110b_gr_pack_mmio[] = {
	{ gk104_gr_init_main_0 },
	{ gk110_gr_init_fe_0 },
	{ gf100_gr_init_pri_0 },
	{ gf100_gr_init_rstr2d_0 },
	{ gf119_gr_init_pd_0 },
	{ gk110_gr_init_ds_0 },
	{ gf100_gr_init_scc_0 },
	{ gk110_gr_init_sked_0 },
	{ gk110_gr_init_cwd_0 },
	{ gf119_gr_init_prop_0 },
	{ gf108_gr_init_gpc_unk_0 },
	{ gf100_gr_init_setup_0 },
	{ gf100_gr_init_crstr_0 },
	{ gf108_gr_init_setup_1 },
	{ gf100_gr_init_zcull_0 },
	{ gf119_gr_init_gpm_0 },
	{ gk110_gr_init_gpc_unk_1 },
	{ gf100_gr_init_gcc_0 },
	{ gk104_gr_init_gpc_unk_2 },
	{ gk104_gr_init_tpccs_0 },
	{ gk110_gr_init_tex_0 },
	{ gk104_gr_init_pe_0 },
	{ gk110b_gr_init_l1c_0 },
	{ gf100_gr_init_mpc_0 },
	{ gk110b_gr_init_sm_0 },
	{ gf117_gr_init_pes_0 },
	{ gf117_gr_init_wwdx_0 },
	{ gf117_gr_init_cbm_0 },
	{ gk104_gr_init_be_0 },
	{ gf100_gr_init_fe_1 },
	{}
};

/*******************************************************************************
 * PGRAPH engine/subdev functions
 ******************************************************************************/

static const struct gf100_gr_func
gk110b_gr = {
	.oneinit_tiles = gf100_gr_oneinit_tiles,
	.oneinit_sm_id = gf100_gr_oneinit_sm_id,
	.init = gf100_gr_init,
	.init_gpc_mmu = gf100_gr_init_gpc_mmu,
	.init_vsc_stream_master = gk104_gr_init_vsc_stream_master,
	.init_zcull = gf117_gr_init_zcull,
	.init_num_active_ltcs = gf100_gr_init_num_active_ltcs,
	.init_rop_active_fbps = gk104_gr_init_rop_active_fbps,
	.init_fecs_exceptions = gf100_gr_init_fecs_exceptions,
	.init_sked_hww_esr = gk104_gr_init_sked_hww_esr,
	.init_419cc0 = gf100_gr_init_419cc0,
	.init_419eb4 = gk110_gr_init_419eb4,
	.init_ppc_exceptions = gk104_gr_init_ppc_exceptions,
	.init_tex_hww_esr = gf100_gr_init_tex_hww_esr,
	.init_shader_exceptions = gf100_gr_init_shader_exceptions,
	.init_400054 = gf100_gr_init_400054,
	.trap_mp = gf100_gr_trap_mp,
	.mmio = gk110b_gr_pack_mmio,
	.fecs.ucode = &gk110_gr_fecs_ucode,
	.gpccs.ucode = &gk110_gr_gpccs_ucode,
	.rops = gf100_gr_rops,
	.ppc_nr = 2,
	.grctx = &gk110b_grctx,
	.zbc = &gf100_gr_zbc,
	.sclass = {
		{ -1, -1, FERMI_TWOD_A },
		{ -1, -1, KEPLER_INLINE_TO_MEMORY_B },
		{ -1, -1, KEPLER_B, &gf100_fermi },
		{ -1, -1, KEPLER_COMPUTE_B },
		{}
	}
};

<<<<<<< HEAD
int
gk110b_gr_new(struct nvkm_device *device, int index, struct nvkm_gr **pgr)
{
	return gf100_gr_new_(&gk110b_gr, device, index, pgr);
=======
static const struct gf100_gr_fwif
gk110b_gr_fwif[] = {
	{ -1, gf100_gr_load, &gk110b_gr },
	{ -1, gf100_gr_nofw, &gk110b_gr },
	{}
};

int
gk110b_gr_new(struct nvkm_device *device, enum nvkm_subdev_type type, int inst,
	      struct nvkm_gr **pgr)
{
	return gf100_gr_new_(gk110b_gr_fwif, device, type, inst, pgr);
>>>>>>> upstream/android-13
}
