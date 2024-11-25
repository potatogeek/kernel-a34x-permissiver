/*
 * Copyright 2016 Advanced Micro Devices, Inc.
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
 */
<<<<<<< HEAD
#include <linux/firmware.h>
#include <drm/drm_cache.h>
#include "amdgpu.h"
#include "gmc_v9_0.h"
#include "amdgpu_atomfirmware.h"

#include "hdp/hdp_4_0_offset.h"
#include "hdp/hdp_4_0_sh_mask.h"
=======

#include <linux/firmware.h>
#include <linux/pci.h>

#include <drm/drm_cache.h>

#include "amdgpu.h"
#include "gmc_v9_0.h"
#include "amdgpu_atomfirmware.h"
#include "amdgpu_gem.h"

>>>>>>> upstream/android-13
#include "gc/gc_9_0_sh_mask.h"
#include "dce/dce_12_0_offset.h"
#include "dce/dce_12_0_sh_mask.h"
#include "vega10_enum.h"
#include "mmhub/mmhub_1_0_offset.h"
<<<<<<< HEAD
=======
#include "athub/athub_1_0_sh_mask.h"
>>>>>>> upstream/android-13
#include "athub/athub_1_0_offset.h"
#include "oss/osssys_4_0_offset.h"

#include "soc15.h"
<<<<<<< HEAD
=======
#include "soc15d.h"
>>>>>>> upstream/android-13
#include "soc15_common.h"
#include "umc/umc_6_0_sh_mask.h"

#include "gfxhub_v1_0.h"
#include "mmhub_v1_0.h"
<<<<<<< HEAD

#include "ivsrcid/vmc/irqsrcs_vmc_1_0.h"

=======
#include "athub_v1_0.h"
#include "gfxhub_v1_1.h"
#include "mmhub_v9_4.h"
#include "mmhub_v1_7.h"
#include "umc_v6_1.h"
#include "umc_v6_0.h"
#include "umc_v6_7.h"
#include "hdp_v4_0.h"
#include "mca_v3_0.h"

#include "ivsrcid/vmc/irqsrcs_vmc_1_0.h"

#include "amdgpu_ras.h"
#include "amdgpu_xgmi.h"

>>>>>>> upstream/android-13
/* add these here since we already include dce12 headers and these are for DCN */
#define mmHUBP0_DCSURF_PRI_VIEWPORT_DIMENSION                                                          0x055d
#define mmHUBP0_DCSURF_PRI_VIEWPORT_DIMENSION_BASE_IDX                                                 2
#define HUBP0_DCSURF_PRI_VIEWPORT_DIMENSION__PRI_VIEWPORT_WIDTH__SHIFT                                        0x0
#define HUBP0_DCSURF_PRI_VIEWPORT_DIMENSION__PRI_VIEWPORT_HEIGHT__SHIFT                                       0x10
#define HUBP0_DCSURF_PRI_VIEWPORT_DIMENSION__PRI_VIEWPORT_WIDTH_MASK                                          0x00003FFFL
#define HUBP0_DCSURF_PRI_VIEWPORT_DIMENSION__PRI_VIEWPORT_HEIGHT_MASK                                         0x3FFF0000L
<<<<<<< HEAD

/* XXX Move this macro to VEGA10 header file, which is like vid.h for VI.*/
#define AMDGPU_NUM_OF_VMIDS			8

static const u32 golden_settings_vega10_hdp[] =
{
	0xf64, 0x0fffffff, 0x00000000,
	0xf65, 0x0fffffff, 0x00000000,
	0xf66, 0x0fffffff, 0x00000000,
	0xf67, 0x0fffffff, 0x00000000,
	0xf68, 0x0fffffff, 0x00000000,
	0xf6a, 0x0fffffff, 0x00000000,
	0xf6b, 0x0fffffff, 0x00000000,
	0xf6c, 0x0fffffff, 0x00000000,
	0xf6d, 0x0fffffff, 0x00000000,
	0xf6e, 0x0fffffff, 0x00000000,
=======
#define mmDCHUBBUB_SDPIF_MMIO_CNTRL_0                                                                  0x049d
#define mmDCHUBBUB_SDPIF_MMIO_CNTRL_0_BASE_IDX                                                         2

#define mmHUBP0_DCSURF_PRI_VIEWPORT_DIMENSION_DCN2                                                          0x05ea
#define mmHUBP0_DCSURF_PRI_VIEWPORT_DIMENSION_DCN2_BASE_IDX                                                 2


static const char *gfxhub_client_ids[] = {
	"CB",
	"DB",
	"IA",
	"WD",
	"CPF",
	"CPC",
	"CPG",
	"RLC",
	"TCP",
	"SQC (inst)",
	"SQC (data)",
	"SQG",
	"PA",
};

static const char *mmhub_client_ids_raven[][2] = {
	[0][0] = "MP1",
	[1][0] = "MP0",
	[2][0] = "VCN",
	[3][0] = "VCNU",
	[4][0] = "HDP",
	[5][0] = "DCE",
	[13][0] = "UTCL2",
	[19][0] = "TLS",
	[26][0] = "OSS",
	[27][0] = "SDMA0",
	[0][1] = "MP1",
	[1][1] = "MP0",
	[2][1] = "VCN",
	[3][1] = "VCNU",
	[4][1] = "HDP",
	[5][1] = "XDP",
	[6][1] = "DBGU0",
	[7][1] = "DCE",
	[8][1] = "DCEDWB0",
	[9][1] = "DCEDWB1",
	[26][1] = "OSS",
	[27][1] = "SDMA0",
};

static const char *mmhub_client_ids_renoir[][2] = {
	[0][0] = "MP1",
	[1][0] = "MP0",
	[2][0] = "HDP",
	[4][0] = "DCEDMC",
	[5][0] = "DCEVGA",
	[13][0] = "UTCL2",
	[19][0] = "TLS",
	[26][0] = "OSS",
	[27][0] = "SDMA0",
	[28][0] = "VCN",
	[29][0] = "VCNU",
	[30][0] = "JPEG",
	[0][1] = "MP1",
	[1][1] = "MP0",
	[2][1] = "HDP",
	[3][1] = "XDP",
	[6][1] = "DBGU0",
	[7][1] = "DCEDMC",
	[8][1] = "DCEVGA",
	[9][1] = "DCEDWB",
	[26][1] = "OSS",
	[27][1] = "SDMA0",
	[28][1] = "VCN",
	[29][1] = "VCNU",
	[30][1] = "JPEG",
};

static const char *mmhub_client_ids_vega10[][2] = {
	[0][0] = "MP0",
	[1][0] = "UVD",
	[2][0] = "UVDU",
	[3][0] = "HDP",
	[13][0] = "UTCL2",
	[14][0] = "OSS",
	[15][0] = "SDMA1",
	[32+0][0] = "VCE0",
	[32+1][0] = "VCE0U",
	[32+2][0] = "XDMA",
	[32+3][0] = "DCE",
	[32+4][0] = "MP1",
	[32+14][0] = "SDMA0",
	[0][1] = "MP0",
	[1][1] = "UVD",
	[2][1] = "UVDU",
	[3][1] = "DBGU0",
	[4][1] = "HDP",
	[5][1] = "XDP",
	[14][1] = "OSS",
	[15][1] = "SDMA0",
	[32+0][1] = "VCE0",
	[32+1][1] = "VCE0U",
	[32+2][1] = "XDMA",
	[32+3][1] = "DCE",
	[32+4][1] = "DCEDWB",
	[32+5][1] = "MP1",
	[32+6][1] = "DBGU1",
	[32+14][1] = "SDMA1",
};

static const char *mmhub_client_ids_vega12[][2] = {
	[0][0] = "MP0",
	[1][0] = "VCE0",
	[2][0] = "VCE0U",
	[3][0] = "HDP",
	[13][0] = "UTCL2",
	[14][0] = "OSS",
	[15][0] = "SDMA1",
	[32+0][0] = "DCE",
	[32+1][0] = "XDMA",
	[32+2][0] = "UVD",
	[32+3][0] = "UVDU",
	[32+4][0] = "MP1",
	[32+15][0] = "SDMA0",
	[0][1] = "MP0",
	[1][1] = "VCE0",
	[2][1] = "VCE0U",
	[3][1] = "DBGU0",
	[4][1] = "HDP",
	[5][1] = "XDP",
	[14][1] = "OSS",
	[15][1] = "SDMA0",
	[32+0][1] = "DCE",
	[32+1][1] = "DCEDWB",
	[32+2][1] = "XDMA",
	[32+3][1] = "UVD",
	[32+4][1] = "UVDU",
	[32+5][1] = "MP1",
	[32+6][1] = "DBGU1",
	[32+15][1] = "SDMA1",
};

static const char *mmhub_client_ids_vega20[][2] = {
	[0][0] = "XDMA",
	[1][0] = "DCE",
	[2][0] = "VCE0",
	[3][0] = "VCE0U",
	[4][0] = "UVD",
	[5][0] = "UVD1U",
	[13][0] = "OSS",
	[14][0] = "HDP",
	[15][0] = "SDMA0",
	[32+0][0] = "UVD",
	[32+1][0] = "UVDU",
	[32+2][0] = "MP1",
	[32+3][0] = "MP0",
	[32+12][0] = "UTCL2",
	[32+14][0] = "SDMA1",
	[0][1] = "XDMA",
	[1][1] = "DCE",
	[2][1] = "DCEDWB",
	[3][1] = "VCE0",
	[4][1] = "VCE0U",
	[5][1] = "UVD1",
	[6][1] = "UVD1U",
	[7][1] = "DBGU0",
	[8][1] = "XDP",
	[13][1] = "OSS",
	[14][1] = "HDP",
	[15][1] = "SDMA0",
	[32+0][1] = "UVD",
	[32+1][1] = "UVDU",
	[32+2][1] = "DBGU1",
	[32+3][1] = "MP1",
	[32+4][1] = "MP0",
	[32+14][1] = "SDMA1",
};

static const char *mmhub_client_ids_arcturus[][2] = {
	[0][0] = "DBGU1",
	[1][0] = "XDP",
	[2][0] = "MP1",
	[14][0] = "HDP",
	[171][0] = "JPEG",
	[172][0] = "VCN",
	[173][0] = "VCNU",
	[203][0] = "JPEG1",
	[204][0] = "VCN1",
	[205][0] = "VCN1U",
	[256][0] = "SDMA0",
	[257][0] = "SDMA1",
	[258][0] = "SDMA2",
	[259][0] = "SDMA3",
	[260][0] = "SDMA4",
	[261][0] = "SDMA5",
	[262][0] = "SDMA6",
	[263][0] = "SDMA7",
	[384][0] = "OSS",
	[0][1] = "DBGU1",
	[1][1] = "XDP",
	[2][1] = "MP1",
	[14][1] = "HDP",
	[171][1] = "JPEG",
	[172][1] = "VCN",
	[173][1] = "VCNU",
	[203][1] = "JPEG1",
	[204][1] = "VCN1",
	[205][1] = "VCN1U",
	[256][1] = "SDMA0",
	[257][1] = "SDMA1",
	[258][1] = "SDMA2",
	[259][1] = "SDMA3",
	[260][1] = "SDMA4",
	[261][1] = "SDMA5",
	[262][1] = "SDMA6",
	[263][1] = "SDMA7",
	[384][1] = "OSS",
};

static const char *mmhub_client_ids_aldebaran[][2] = {
	[2][0] = "MP1",
	[3][0] = "MP0",
	[32+1][0] = "DBGU_IO0",
	[32+2][0] = "DBGU_IO2",
	[32+4][0] = "MPIO",
	[96+11][0] = "JPEG0",
	[96+12][0] = "VCN0",
	[96+13][0] = "VCNU0",
	[128+11][0] = "JPEG1",
	[128+12][0] = "VCN1",
	[128+13][0] = "VCNU1",
	[160+1][0] = "XDP",
	[160+14][0] = "HDP",
	[256+0][0] = "SDMA0",
	[256+1][0] = "SDMA1",
	[256+2][0] = "SDMA2",
	[256+3][0] = "SDMA3",
	[256+4][0] = "SDMA4",
	[384+0][0] = "OSS",
	[2][1] = "MP1",
	[3][1] = "MP0",
	[32+1][1] = "DBGU_IO0",
	[32+2][1] = "DBGU_IO2",
	[32+4][1] = "MPIO",
	[96+11][1] = "JPEG0",
	[96+12][1] = "VCN0",
	[96+13][1] = "VCNU0",
	[128+11][1] = "JPEG1",
	[128+12][1] = "VCN1",
	[128+13][1] = "VCNU1",
	[160+1][1] = "XDP",
	[160+14][1] = "HDP",
	[256+0][1] = "SDMA0",
	[256+1][1] = "SDMA1",
	[256+2][1] = "SDMA2",
	[256+3][1] = "SDMA3",
	[256+4][1] = "SDMA4",
	[384+0][1] = "OSS",
>>>>>>> upstream/android-13
};

static const struct soc15_reg_golden golden_settings_mmhub_1_0_0[] =
{
	SOC15_REG_GOLDEN_VALUE(MMHUB, 0, mmDAGB1_WRCLI2, 0x00000007, 0xfe5fe0fa),
	SOC15_REG_GOLDEN_VALUE(MMHUB, 0, mmMMEA1_DRAM_WR_CLI2GRP_MAP0, 0x00000030, 0x55555565)
};

static const struct soc15_reg_golden golden_settings_athub_1_0_0[] =
{
	SOC15_REG_GOLDEN_VALUE(ATHUB, 0, mmRPB_ARB_CNTL, 0x0000ff00, 0x00000800),
	SOC15_REG_GOLDEN_VALUE(ATHUB, 0, mmRPB_ARB_CNTL2, 0x00ff00ff, 0x00080008)
};

<<<<<<< HEAD
/* Ecc related register addresses, (BASE + reg offset) */
/* Universal Memory Controller caps (may be fused). */
/* UMCCH:UmcLocalCap */
#define UMCLOCALCAPS_ADDR0	(0x00014306 + 0x00000000)
#define UMCLOCALCAPS_ADDR1	(0x00014306 + 0x00000800)
#define UMCLOCALCAPS_ADDR2	(0x00014306 + 0x00001000)
#define UMCLOCALCAPS_ADDR3	(0x00014306 + 0x00001800)
#define UMCLOCALCAPS_ADDR4	(0x00054306 + 0x00000000)
#define UMCLOCALCAPS_ADDR5	(0x00054306 + 0x00000800)
#define UMCLOCALCAPS_ADDR6	(0x00054306 + 0x00001000)
#define UMCLOCALCAPS_ADDR7	(0x00054306 + 0x00001800)
#define UMCLOCALCAPS_ADDR8	(0x00094306 + 0x00000000)
#define UMCLOCALCAPS_ADDR9	(0x00094306 + 0x00000800)
#define UMCLOCALCAPS_ADDR10	(0x00094306 + 0x00001000)
#define UMCLOCALCAPS_ADDR11	(0x00094306 + 0x00001800)
#define UMCLOCALCAPS_ADDR12	(0x000d4306 + 0x00000000)
#define UMCLOCALCAPS_ADDR13	(0x000d4306 + 0x00000800)
#define UMCLOCALCAPS_ADDR14	(0x000d4306 + 0x00001000)
#define UMCLOCALCAPS_ADDR15	(0x000d4306 + 0x00001800)

/* Universal Memory Controller Channel config. */
/* UMCCH:UMC_CONFIG */
#define UMCCH_UMC_CONFIG_ADDR0	(0x00014040 + 0x00000000)
#define UMCCH_UMC_CONFIG_ADDR1	(0x00014040 + 0x00000800)
#define UMCCH_UMC_CONFIG_ADDR2	(0x00014040 + 0x00001000)
#define UMCCH_UMC_CONFIG_ADDR3	(0x00014040 + 0x00001800)
#define UMCCH_UMC_CONFIG_ADDR4	(0x00054040 + 0x00000000)
#define UMCCH_UMC_CONFIG_ADDR5	(0x00054040 + 0x00000800)
#define UMCCH_UMC_CONFIG_ADDR6	(0x00054040 + 0x00001000)
#define UMCCH_UMC_CONFIG_ADDR7	(0x00054040 + 0x00001800)
#define UMCCH_UMC_CONFIG_ADDR8	(0x00094040 + 0x00000000)
#define UMCCH_UMC_CONFIG_ADDR9	(0x00094040 + 0x00000800)
#define UMCCH_UMC_CONFIG_ADDR10	(0x00094040 + 0x00001000)
#define UMCCH_UMC_CONFIG_ADDR11	(0x00094040 + 0x00001800)
#define UMCCH_UMC_CONFIG_ADDR12	(0x000d4040 + 0x00000000)
#define UMCCH_UMC_CONFIG_ADDR13	(0x000d4040 + 0x00000800)
#define UMCCH_UMC_CONFIG_ADDR14	(0x000d4040 + 0x00001000)
#define UMCCH_UMC_CONFIG_ADDR15	(0x000d4040 + 0x00001800)

/* Universal Memory Controller Channel Ecc config. */
/* UMCCH:EccCtrl */
#define UMCCH_ECCCTRL_ADDR0	(0x00014053 + 0x00000000)
#define UMCCH_ECCCTRL_ADDR1	(0x00014053 + 0x00000800)
#define UMCCH_ECCCTRL_ADDR2	(0x00014053 + 0x00001000)
#define UMCCH_ECCCTRL_ADDR3	(0x00014053 + 0x00001800)
#define UMCCH_ECCCTRL_ADDR4	(0x00054053 + 0x00000000)
#define UMCCH_ECCCTRL_ADDR5	(0x00054053 + 0x00000800)
#define UMCCH_ECCCTRL_ADDR6	(0x00054053 + 0x00001000)
#define UMCCH_ECCCTRL_ADDR7	(0x00054053 + 0x00001800)
#define UMCCH_ECCCTRL_ADDR8	(0x00094053 + 0x00000000)
#define UMCCH_ECCCTRL_ADDR9	(0x00094053 + 0x00000800)
#define UMCCH_ECCCTRL_ADDR10	(0x00094053 + 0x00001000)
#define UMCCH_ECCCTRL_ADDR11	(0x00094053 + 0x00001800)
#define UMCCH_ECCCTRL_ADDR12	(0x000d4053 + 0x00000000)
#define UMCCH_ECCCTRL_ADDR13	(0x000d4053 + 0x00000800)
#define UMCCH_ECCCTRL_ADDR14	(0x000d4053 + 0x00001000)
#define UMCCH_ECCCTRL_ADDR15	(0x000d4053 + 0x00001800)

static const uint32_t ecc_umclocalcap_addrs[] = {
	UMCLOCALCAPS_ADDR0,
	UMCLOCALCAPS_ADDR1,
	UMCLOCALCAPS_ADDR2,
	UMCLOCALCAPS_ADDR3,
	UMCLOCALCAPS_ADDR4,
	UMCLOCALCAPS_ADDR5,
	UMCLOCALCAPS_ADDR6,
	UMCLOCALCAPS_ADDR7,
	UMCLOCALCAPS_ADDR8,
	UMCLOCALCAPS_ADDR9,
	UMCLOCALCAPS_ADDR10,
	UMCLOCALCAPS_ADDR11,
	UMCLOCALCAPS_ADDR12,
	UMCLOCALCAPS_ADDR13,
	UMCLOCALCAPS_ADDR14,
	UMCLOCALCAPS_ADDR15,
};

static const uint32_t ecc_umcch_umc_config_addrs[] = {
	UMCCH_UMC_CONFIG_ADDR0,
	UMCCH_UMC_CONFIG_ADDR1,
	UMCCH_UMC_CONFIG_ADDR2,
	UMCCH_UMC_CONFIG_ADDR3,
	UMCCH_UMC_CONFIG_ADDR4,
	UMCCH_UMC_CONFIG_ADDR5,
	UMCCH_UMC_CONFIG_ADDR6,
	UMCCH_UMC_CONFIG_ADDR7,
	UMCCH_UMC_CONFIG_ADDR8,
	UMCCH_UMC_CONFIG_ADDR9,
	UMCCH_UMC_CONFIG_ADDR10,
	UMCCH_UMC_CONFIG_ADDR11,
	UMCCH_UMC_CONFIG_ADDR12,
	UMCCH_UMC_CONFIG_ADDR13,
	UMCCH_UMC_CONFIG_ADDR14,
	UMCCH_UMC_CONFIG_ADDR15,
};

static const uint32_t ecc_umcch_eccctrl_addrs[] = {
	UMCCH_ECCCTRL_ADDR0,
	UMCCH_ECCCTRL_ADDR1,
	UMCCH_ECCCTRL_ADDR2,
	UMCCH_ECCCTRL_ADDR3,
	UMCCH_ECCCTRL_ADDR4,
	UMCCH_ECCCTRL_ADDR5,
	UMCCH_ECCCTRL_ADDR6,
	UMCCH_ECCCTRL_ADDR7,
	UMCCH_ECCCTRL_ADDR8,
	UMCCH_ECCCTRL_ADDR9,
	UMCCH_ECCCTRL_ADDR10,
	UMCCH_ECCCTRL_ADDR11,
	UMCCH_ECCCTRL_ADDR12,
	UMCCH_ECCCTRL_ADDR13,
	UMCCH_ECCCTRL_ADDR14,
	UMCCH_ECCCTRL_ADDR15,
};
=======
static const uint32_t ecc_umc_mcumc_ctrl_addrs[] = {
	(0x000143c0 + 0x00000000),
	(0x000143c0 + 0x00000800),
	(0x000143c0 + 0x00001000),
	(0x000143c0 + 0x00001800),
	(0x000543c0 + 0x00000000),
	(0x000543c0 + 0x00000800),
	(0x000543c0 + 0x00001000),
	(0x000543c0 + 0x00001800),
	(0x000943c0 + 0x00000000),
	(0x000943c0 + 0x00000800),
	(0x000943c0 + 0x00001000),
	(0x000943c0 + 0x00001800),
	(0x000d43c0 + 0x00000000),
	(0x000d43c0 + 0x00000800),
	(0x000d43c0 + 0x00001000),
	(0x000d43c0 + 0x00001800),
	(0x001143c0 + 0x00000000),
	(0x001143c0 + 0x00000800),
	(0x001143c0 + 0x00001000),
	(0x001143c0 + 0x00001800),
	(0x001543c0 + 0x00000000),
	(0x001543c0 + 0x00000800),
	(0x001543c0 + 0x00001000),
	(0x001543c0 + 0x00001800),
	(0x001943c0 + 0x00000000),
	(0x001943c0 + 0x00000800),
	(0x001943c0 + 0x00001000),
	(0x001943c0 + 0x00001800),
	(0x001d43c0 + 0x00000000),
	(0x001d43c0 + 0x00000800),
	(0x001d43c0 + 0x00001000),
	(0x001d43c0 + 0x00001800),
};

static const uint32_t ecc_umc_mcumc_ctrl_mask_addrs[] = {
	(0x000143e0 + 0x00000000),
	(0x000143e0 + 0x00000800),
	(0x000143e0 + 0x00001000),
	(0x000143e0 + 0x00001800),
	(0x000543e0 + 0x00000000),
	(0x000543e0 + 0x00000800),
	(0x000543e0 + 0x00001000),
	(0x000543e0 + 0x00001800),
	(0x000943e0 + 0x00000000),
	(0x000943e0 + 0x00000800),
	(0x000943e0 + 0x00001000),
	(0x000943e0 + 0x00001800),
	(0x000d43e0 + 0x00000000),
	(0x000d43e0 + 0x00000800),
	(0x000d43e0 + 0x00001000),
	(0x000d43e0 + 0x00001800),
	(0x001143e0 + 0x00000000),
	(0x001143e0 + 0x00000800),
	(0x001143e0 + 0x00001000),
	(0x001143e0 + 0x00001800),
	(0x001543e0 + 0x00000000),
	(0x001543e0 + 0x00000800),
	(0x001543e0 + 0x00001000),
	(0x001543e0 + 0x00001800),
	(0x001943e0 + 0x00000000),
	(0x001943e0 + 0x00000800),
	(0x001943e0 + 0x00001000),
	(0x001943e0 + 0x00001800),
	(0x001d43e0 + 0x00000000),
	(0x001d43e0 + 0x00000800),
	(0x001d43e0 + 0x00001000),
	(0x001d43e0 + 0x00001800),
};

static int gmc_v9_0_ecc_interrupt_state(struct amdgpu_device *adev,
		struct amdgpu_irq_src *src,
		unsigned type,
		enum amdgpu_interrupt_state state)
{
	u32 bits, i, tmp, reg;

	/* Devices newer then VEGA10/12 shall have these programming
	     sequences performed by PSP BL */
	if (adev->asic_type >= CHIP_VEGA20)
		return 0;

	bits = 0x7f;

	switch (state) {
	case AMDGPU_IRQ_STATE_DISABLE:
		for (i = 0; i < ARRAY_SIZE(ecc_umc_mcumc_ctrl_addrs); i++) {
			reg = ecc_umc_mcumc_ctrl_addrs[i];
			tmp = RREG32(reg);
			tmp &= ~bits;
			WREG32(reg, tmp);
		}
		for (i = 0; i < ARRAY_SIZE(ecc_umc_mcumc_ctrl_mask_addrs); i++) {
			reg = ecc_umc_mcumc_ctrl_mask_addrs[i];
			tmp = RREG32(reg);
			tmp &= ~bits;
			WREG32(reg, tmp);
		}
		break;
	case AMDGPU_IRQ_STATE_ENABLE:
		for (i = 0; i < ARRAY_SIZE(ecc_umc_mcumc_ctrl_addrs); i++) {
			reg = ecc_umc_mcumc_ctrl_addrs[i];
			tmp = RREG32(reg);
			tmp |= bits;
			WREG32(reg, tmp);
		}
		for (i = 0; i < ARRAY_SIZE(ecc_umc_mcumc_ctrl_mask_addrs); i++) {
			reg = ecc_umc_mcumc_ctrl_mask_addrs[i];
			tmp = RREG32(reg);
			tmp |= bits;
			WREG32(reg, tmp);
		}
		break;
	default:
		break;
	}

	return 0;
}
>>>>>>> upstream/android-13

static int gmc_v9_0_vm_fault_interrupt_state(struct amdgpu_device *adev,
					struct amdgpu_irq_src *src,
					unsigned type,
					enum amdgpu_interrupt_state state)
{
	struct amdgpu_vmhub *hub;
	u32 tmp, reg, bits, i, j;

	bits = VM_CONTEXT1_CNTL__RANGE_PROTECTION_FAULT_ENABLE_INTERRUPT_MASK |
		VM_CONTEXT1_CNTL__DUMMY_PAGE_PROTECTION_FAULT_ENABLE_INTERRUPT_MASK |
		VM_CONTEXT1_CNTL__PDE0_PROTECTION_FAULT_ENABLE_INTERRUPT_MASK |
		VM_CONTEXT1_CNTL__VALID_PROTECTION_FAULT_ENABLE_INTERRUPT_MASK |
		VM_CONTEXT1_CNTL__READ_PROTECTION_FAULT_ENABLE_INTERRUPT_MASK |
		VM_CONTEXT1_CNTL__WRITE_PROTECTION_FAULT_ENABLE_INTERRUPT_MASK |
		VM_CONTEXT1_CNTL__EXECUTE_PROTECTION_FAULT_ENABLE_INTERRUPT_MASK;

	switch (state) {
	case AMDGPU_IRQ_STATE_DISABLE:
<<<<<<< HEAD
		for (j = 0; j < AMDGPU_MAX_VMHUBS; j++) {
=======
		for (j = 0; j < adev->num_vmhubs; j++) {
>>>>>>> upstream/android-13
			hub = &adev->vmhub[j];
			for (i = 0; i < 16; i++) {
				reg = hub->vm_context0_cntl + i;
				tmp = RREG32(reg);
				tmp &= ~bits;
				WREG32(reg, tmp);
			}
		}
		break;
	case AMDGPU_IRQ_STATE_ENABLE:
<<<<<<< HEAD
		for (j = 0; j < AMDGPU_MAX_VMHUBS; j++) {
=======
		for (j = 0; j < adev->num_vmhubs; j++) {
>>>>>>> upstream/android-13
			hub = &adev->vmhub[j];
			for (i = 0; i < 16; i++) {
				reg = hub->vm_context0_cntl + i;
				tmp = RREG32(reg);
				tmp |= bits;
				WREG32(reg, tmp);
			}
		}
<<<<<<< HEAD
=======
		break;
>>>>>>> upstream/android-13
	default:
		break;
	}

	return 0;
}

static int gmc_v9_0_process_interrupt(struct amdgpu_device *adev,
<<<<<<< HEAD
				struct amdgpu_irq_src *source,
				struct amdgpu_iv_entry *entry)
{
	struct amdgpu_vmhub *hub = &adev->vmhub[entry->vmid_src];
	uint32_t status = 0;
=======
				      struct amdgpu_irq_src *source,
				      struct amdgpu_iv_entry *entry)
{
	bool retry_fault = !!(entry->src_data[1] & 0x80);
	bool write_fault = !!(entry->src_data[1] & 0x20);
	uint32_t status = 0, cid = 0, rw = 0;
	struct amdgpu_task_info task_info;
	struct amdgpu_vmhub *hub;
	const char *mmhub_cid;
	const char *hub_name;
>>>>>>> upstream/android-13
	u64 addr;

	addr = (u64)entry->src_data[0] << 12;
	addr |= ((u64)entry->src_data[1] & 0xf) << 44;

<<<<<<< HEAD
	if (!amdgpu_sriov_vf(adev)) {
		status = RREG32(hub->vm_l2_pro_fault_status);
		WREG32_P(hub->vm_l2_pro_fault_cntl, 1, ~1);
	}

	if (printk_ratelimit()) {
		struct amdgpu_task_info task_info = { 0 };

		amdgpu_vm_get_task_info(adev, entry->pasid, &task_info);

		dev_err(adev->dev,
			"[%s] VMC page fault (src_id:%u ring:%u vmid:%u pasid:%u, for process %s pid %d thread %s pid %d\n)\n",
			entry->vmid_src ? "mmhub" : "gfxhub",
			entry->src_id, entry->ring_id, entry->vmid,
			entry->pasid, task_info.process_name, task_info.tgid,
			task_info.task_name, task_info.pid);
		dev_err(adev->dev, "  at address 0x%016llx from %d\n",
			addr, entry->client_id);
		if (!amdgpu_sriov_vf(adev))
			dev_err(adev->dev,
				"VM_L2_PROTECTION_FAULT_STATUS:0x%08X\n",
				status);
	}

=======
	if (retry_fault) {
		/* Returning 1 here also prevents sending the IV to the KFD */

		/* Process it onyl if it's the first fault for this address */
		if (entry->ih != &adev->irq.ih_soft &&
		    amdgpu_gmc_filter_faults(adev, addr, entry->pasid,
					     entry->timestamp))
			return 1;

		/* Delegate it to a different ring if the hardware hasn't
		 * already done it.
		 */
		if (entry->ih == &adev->irq.ih) {
			amdgpu_irq_delegate(adev, entry, 8);
			return 1;
		}

		/* Try to handle the recoverable page faults by filling page
		 * tables
		 */
		if (amdgpu_vm_handle_fault(adev, entry->pasid, addr, write_fault))
			return 1;
	}

	if (!printk_ratelimit())
		return 0;

	if (entry->client_id == SOC15_IH_CLIENTID_VMC) {
		hub_name = "mmhub0";
		hub = &adev->vmhub[AMDGPU_MMHUB_0];
	} else if (entry->client_id == SOC15_IH_CLIENTID_VMC1) {
		hub_name = "mmhub1";
		hub = &adev->vmhub[AMDGPU_MMHUB_1];
	} else {
		hub_name = "gfxhub0";
		hub = &adev->vmhub[AMDGPU_GFXHUB_0];
	}

	memset(&task_info, 0, sizeof(struct amdgpu_task_info));
	amdgpu_vm_get_task_info(adev, entry->pasid, &task_info);

	dev_err(adev->dev,
		"[%s] %s page fault (src_id:%u ring:%u vmid:%u "
		"pasid:%u, for process %s pid %d thread %s pid %d)\n",
		hub_name, retry_fault ? "retry" : "no-retry",
		entry->src_id, entry->ring_id, entry->vmid,
		entry->pasid, task_info.process_name, task_info.tgid,
		task_info.task_name, task_info.pid);
	dev_err(adev->dev, "  in page starting at address 0x%016llx from IH client 0x%x (%s)\n",
		addr, entry->client_id,
		soc15_ih_clientid_name[entry->client_id]);

	if (amdgpu_sriov_vf(adev))
		return 0;

	/*
	 * Issue a dummy read to wait for the status register to
	 * be updated to avoid reading an incorrect value due to
	 * the new fast GRBM interface.
	 */
	if ((entry->vmid_src == AMDGPU_GFXHUB_0) &&
	    (adev->asic_type < CHIP_ALDEBARAN))
		RREG32(hub->vm_l2_pro_fault_status);

	status = RREG32(hub->vm_l2_pro_fault_status);
	cid = REG_GET_FIELD(status, VM_L2_PROTECTION_FAULT_STATUS, CID);
	rw = REG_GET_FIELD(status, VM_L2_PROTECTION_FAULT_STATUS, RW);
	WREG32_P(hub->vm_l2_pro_fault_cntl, 1, ~1);


	dev_err(adev->dev,
		"VM_L2_PROTECTION_FAULT_STATUS:0x%08X\n",
		status);
	if (hub == &adev->vmhub[AMDGPU_GFXHUB_0]) {
		dev_err(adev->dev, "\t Faulty UTCL2 client ID: %s (0x%x)\n",
			cid >= ARRAY_SIZE(gfxhub_client_ids) ? "unknown" :
			gfxhub_client_ids[cid],
			cid);
	} else {
		switch (adev->asic_type) {
		case CHIP_VEGA10:
			mmhub_cid = mmhub_client_ids_vega10[cid][rw];
			break;
		case CHIP_VEGA12:
			mmhub_cid = mmhub_client_ids_vega12[cid][rw];
			break;
		case CHIP_VEGA20:
			mmhub_cid = mmhub_client_ids_vega20[cid][rw];
			break;
		case CHIP_ARCTURUS:
			mmhub_cid = mmhub_client_ids_arcturus[cid][rw];
			break;
		case CHIP_RAVEN:
			mmhub_cid = mmhub_client_ids_raven[cid][rw];
			break;
		case CHIP_RENOIR:
			mmhub_cid = mmhub_client_ids_renoir[cid][rw];
			break;
		case CHIP_ALDEBARAN:
			mmhub_cid = mmhub_client_ids_aldebaran[cid][rw];
			break;
		default:
			mmhub_cid = NULL;
			break;
		}
		dev_err(adev->dev, "\t Faulty UTCL2 client ID: %s (0x%x)\n",
			mmhub_cid ? mmhub_cid : "unknown", cid);
	}
	dev_err(adev->dev, "\t MORE_FAULTS: 0x%lx\n",
		REG_GET_FIELD(status,
		VM_L2_PROTECTION_FAULT_STATUS, MORE_FAULTS));
	dev_err(adev->dev, "\t WALKER_ERROR: 0x%lx\n",
		REG_GET_FIELD(status,
		VM_L2_PROTECTION_FAULT_STATUS, WALKER_ERROR));
	dev_err(adev->dev, "\t PERMISSION_FAULTS: 0x%lx\n",
		REG_GET_FIELD(status,
		VM_L2_PROTECTION_FAULT_STATUS, PERMISSION_FAULTS));
	dev_err(adev->dev, "\t MAPPING_ERROR: 0x%lx\n",
		REG_GET_FIELD(status,
		VM_L2_PROTECTION_FAULT_STATUS, MAPPING_ERROR));
	dev_err(adev->dev, "\t RW: 0x%x\n", rw);
>>>>>>> upstream/android-13
	return 0;
}

static const struct amdgpu_irq_src_funcs gmc_v9_0_irq_funcs = {
	.set = gmc_v9_0_vm_fault_interrupt_state,
	.process = gmc_v9_0_process_interrupt,
};

<<<<<<< HEAD
=======

static const struct amdgpu_irq_src_funcs gmc_v9_0_ecc_funcs = {
	.set = gmc_v9_0_ecc_interrupt_state,
	.process = amdgpu_umc_process_ecc_irq,
};

>>>>>>> upstream/android-13
static void gmc_v9_0_set_irq_funcs(struct amdgpu_device *adev)
{
	adev->gmc.vm_fault.num_types = 1;
	adev->gmc.vm_fault.funcs = &gmc_v9_0_irq_funcs;
<<<<<<< HEAD
}

static uint32_t gmc_v9_0_get_invalidate_req(unsigned int vmid)
{
	u32 req = 0;

	/* invalidate using legacy mode on vmid*/
	req = REG_SET_FIELD(req, VM_INVALIDATE_ENG0_REQ,
			    PER_VMID_INVALIDATE_REQ, 1 << vmid);
	req = REG_SET_FIELD(req, VM_INVALIDATE_ENG0_REQ, FLUSH_TYPE, 0);
=======

	if (!amdgpu_sriov_vf(adev) &&
	    !adev->gmc.xgmi.connected_to_cpu) {
		adev->gmc.ecc_irq.num_types = 1;
		adev->gmc.ecc_irq.funcs = &gmc_v9_0_ecc_funcs;
	}
}

static uint32_t gmc_v9_0_get_invalidate_req(unsigned int vmid,
					uint32_t flush_type)
{
	u32 req = 0;

	req = REG_SET_FIELD(req, VM_INVALIDATE_ENG0_REQ,
			    PER_VMID_INVALIDATE_REQ, 1 << vmid);
	req = REG_SET_FIELD(req, VM_INVALIDATE_ENG0_REQ, FLUSH_TYPE, flush_type);
>>>>>>> upstream/android-13
	req = REG_SET_FIELD(req, VM_INVALIDATE_ENG0_REQ, INVALIDATE_L2_PTES, 1);
	req = REG_SET_FIELD(req, VM_INVALIDATE_ENG0_REQ, INVALIDATE_L2_PDE0, 1);
	req = REG_SET_FIELD(req, VM_INVALIDATE_ENG0_REQ, INVALIDATE_L2_PDE1, 1);
	req = REG_SET_FIELD(req, VM_INVALIDATE_ENG0_REQ, INVALIDATE_L2_PDE2, 1);
	req = REG_SET_FIELD(req, VM_INVALIDATE_ENG0_REQ, INVALIDATE_L1_PTES, 1);
	req = REG_SET_FIELD(req, VM_INVALIDATE_ENG0_REQ,
			    CLEAR_PROTECTION_FAULT_STATUS_ADDR,	0);

	return req;
}

<<<<<<< HEAD
=======
/**
 * gmc_v9_0_use_invalidate_semaphore - judge whether to use semaphore
 *
 * @adev: amdgpu_device pointer
 * @vmhub: vmhub type
 *
 */
static bool gmc_v9_0_use_invalidate_semaphore(struct amdgpu_device *adev,
				       uint32_t vmhub)
{
	if (adev->asic_type == CHIP_ALDEBARAN)
		return false;

	return ((vmhub == AMDGPU_MMHUB_0 ||
		 vmhub == AMDGPU_MMHUB_1) &&
		(!amdgpu_sriov_vf(adev)) &&
		(!(!(adev->apu_flags & AMD_APU_IS_RAVEN2) &&
		   (adev->apu_flags & AMD_APU_IS_PICASSO))));
}

static bool gmc_v9_0_get_atc_vmid_pasid_mapping_info(struct amdgpu_device *adev,
					uint8_t vmid, uint16_t *p_pasid)
{
	uint32_t value;

	value = RREG32(SOC15_REG_OFFSET(ATHUB, 0, mmATC_VMID0_PASID_MAPPING)
		     + vmid);
	*p_pasid = value & ATC_VMID0_PASID_MAPPING__PASID_MASK;

	return !!(value & ATC_VMID0_PASID_MAPPING__VALID_MASK);
}

>>>>>>> upstream/android-13
/*
 * GART
 * VMID 0 is the physical GPU addresses as used by the kernel.
 * VMIDs 1-15 are used for userspace clients and are handled
 * by the amdgpu vm/hsa code.
 */

/**
<<<<<<< HEAD
 * gmc_v9_0_flush_gpu_tlb - gart tlb flush callback
 *
 * @adev: amdgpu_device pointer
 * @vmid: vm instance to flush
 *
 * Flush the TLB for the requested page table.
 */
static void gmc_v9_0_flush_gpu_tlb(struct amdgpu_device *adev,
					uint32_t vmid)
{
	/* Use register 17 for GART */
	const unsigned eng = 17;
	unsigned i, j;

	spin_lock(&adev->gmc.invalidate_lock);

	for (i = 0; i < AMDGPU_MAX_VMHUBS; ++i) {
		struct amdgpu_vmhub *hub = &adev->vmhub[i];
		u32 tmp = gmc_v9_0_get_invalidate_req(vmid);

		WREG32_NO_KIQ(hub->vm_inv_eng0_req + eng, tmp);

		/* Busy wait for ACK.*/
		for (j = 0; j < 100; j++) {
			tmp = RREG32_NO_KIQ(hub->vm_inv_eng0_ack + eng);
			tmp &= 1 << vmid;
			if (tmp)
				break;
			cpu_relax();
		}
		if (j < 100)
			continue;

		/* Wait for ACK with a delay.*/
		for (j = 0; j < adev->usec_timeout; j++) {
			tmp = RREG32_NO_KIQ(hub->vm_inv_eng0_ack + eng);
			tmp &= 1 << vmid;
			if (tmp)
				break;
			udelay(1);
		}
		if (j < adev->usec_timeout)
			continue;

		DRM_ERROR("Timeout waiting for VM flush ACK!\n");
	}

	spin_unlock(&adev->gmc.invalidate_lock);
=======
 * gmc_v9_0_flush_gpu_tlb - tlb flush with certain type
 *
 * @adev: amdgpu_device pointer
 * @vmid: vm instance to flush
 * @vmhub: which hub to flush
 * @flush_type: the flush type
 *
 * Flush the TLB for the requested page table using certain type.
 */
static void gmc_v9_0_flush_gpu_tlb(struct amdgpu_device *adev, uint32_t vmid,
					uint32_t vmhub, uint32_t flush_type)
{
	bool use_semaphore = gmc_v9_0_use_invalidate_semaphore(adev, vmhub);
	const unsigned eng = 17;
	u32 j, inv_req, inv_req2, tmp;
	struct amdgpu_vmhub *hub;

	BUG_ON(vmhub >= adev->num_vmhubs);

	hub = &adev->vmhub[vmhub];
	if (adev->gmc.xgmi.num_physical_nodes &&
	    adev->asic_type == CHIP_VEGA20) {
		/* Vega20+XGMI caches PTEs in TC and TLB. Add a
		 * heavy-weight TLB flush (type 2), which flushes
		 * both. Due to a race condition with concurrent
		 * memory accesses using the same TLB cache line, we
		 * still need a second TLB flush after this.
		 */
		inv_req = gmc_v9_0_get_invalidate_req(vmid, 2);
		inv_req2 = gmc_v9_0_get_invalidate_req(vmid, flush_type);
	} else {
		inv_req = gmc_v9_0_get_invalidate_req(vmid, flush_type);
		inv_req2 = 0;
	}

	/* This is necessary for a HW workaround under SRIOV as well
	 * as GFXOFF under bare metal
	 */
	if (adev->gfx.kiq.ring.sched.ready &&
	    (amdgpu_sriov_runtime(adev) || !amdgpu_sriov_vf(adev)) &&
	    down_read_trylock(&adev->reset_sem)) {
		uint32_t req = hub->vm_inv_eng0_req + hub->eng_distance * eng;
		uint32_t ack = hub->vm_inv_eng0_ack + hub->eng_distance * eng;

		amdgpu_virt_kiq_reg_write_reg_wait(adev, req, ack, inv_req,
						   1 << vmid);
		up_read(&adev->reset_sem);
		return;
	}

	spin_lock(&adev->gmc.invalidate_lock);

	/*
	 * It may lose gpuvm invalidate acknowldege state across power-gating
	 * off cycle, add semaphore acquire before invalidation and semaphore
	 * release after invalidation to avoid entering power gated state
	 * to WA the Issue
	 */

	/* TODO: It needs to continue working on debugging with semaphore for GFXHUB as well. */
	if (use_semaphore) {
		for (j = 0; j < adev->usec_timeout; j++) {
			/* a read return value of 1 means semaphore acuqire */
			tmp = RREG32_NO_KIQ(hub->vm_inv_eng0_sem +
					    hub->eng_distance * eng);
			if (tmp & 0x1)
				break;
			udelay(1);
		}

		if (j >= adev->usec_timeout)
			DRM_ERROR("Timeout waiting for sem acquire in VM flush!\n");
	}

	do {
		WREG32_NO_KIQ(hub->vm_inv_eng0_req +
			      hub->eng_distance * eng, inv_req);

		/*
		 * Issue a dummy read to wait for the ACK register to
		 * be cleared to avoid a false ACK due to the new fast
		 * GRBM interface.
		 */
		if ((vmhub == AMDGPU_GFXHUB_0) &&
		    (adev->asic_type < CHIP_ALDEBARAN))
			RREG32_NO_KIQ(hub->vm_inv_eng0_req +
				      hub->eng_distance * eng);

		for (j = 0; j < adev->usec_timeout; j++) {
			tmp = RREG32_NO_KIQ(hub->vm_inv_eng0_ack +
					    hub->eng_distance * eng);
			if (tmp & (1 << vmid))
				break;
			udelay(1);
		}

		inv_req = inv_req2;
		inv_req2 = 0;
	} while (inv_req);

	/* TODO: It needs to continue working on debugging with semaphore for GFXHUB as well. */
	if (use_semaphore)
		/*
		 * add semaphore release after invalidation,
		 * write with 0 means semaphore release
		 */
		WREG32_NO_KIQ(hub->vm_inv_eng0_sem +
			      hub->eng_distance * eng, 0);

	spin_unlock(&adev->gmc.invalidate_lock);

	if (j < adev->usec_timeout)
		return;

	DRM_ERROR("Timeout waiting for VM flush ACK!\n");
}

/**
 * gmc_v9_0_flush_gpu_tlb_pasid - tlb flush via pasid
 *
 * @adev: amdgpu_device pointer
 * @pasid: pasid to be flush
 * @flush_type: the flush type
 * @all_hub: flush all hubs
 *
 * Flush the TLB for the requested pasid.
 */
static int gmc_v9_0_flush_gpu_tlb_pasid(struct amdgpu_device *adev,
					uint16_t pasid, uint32_t flush_type,
					bool all_hub)
{
	int vmid, i;
	signed long r;
	uint32_t seq;
	uint16_t queried_pasid;
	bool ret;
	struct amdgpu_ring *ring = &adev->gfx.kiq.ring;
	struct amdgpu_kiq *kiq = &adev->gfx.kiq;

	if (amdgpu_in_reset(adev))
		return -EIO;

	if (ring->sched.ready && down_read_trylock(&adev->reset_sem)) {
		/* Vega20+XGMI caches PTEs in TC and TLB. Add a
		 * heavy-weight TLB flush (type 2), which flushes
		 * both. Due to a race condition with concurrent
		 * memory accesses using the same TLB cache line, we
		 * still need a second TLB flush after this.
		 */
		bool vega20_xgmi_wa = (adev->gmc.xgmi.num_physical_nodes &&
				       adev->asic_type == CHIP_VEGA20);
		/* 2 dwords flush + 8 dwords fence */
		unsigned int ndw = kiq->pmf->invalidate_tlbs_size + 8;

		if (vega20_xgmi_wa)
			ndw += kiq->pmf->invalidate_tlbs_size;

		spin_lock(&adev->gfx.kiq.ring_lock);
		/* 2 dwords flush + 8 dwords fence */
		amdgpu_ring_alloc(ring, ndw);
		if (vega20_xgmi_wa)
			kiq->pmf->kiq_invalidate_tlbs(ring,
						      pasid, 2, all_hub);
		kiq->pmf->kiq_invalidate_tlbs(ring,
					pasid, flush_type, all_hub);
		r = amdgpu_fence_emit_polling(ring, &seq, MAX_KIQ_REG_WAIT);
		if (r) {
			amdgpu_ring_undo(ring);
			spin_unlock(&adev->gfx.kiq.ring_lock);
			up_read(&adev->reset_sem);
			return -ETIME;
		}

		amdgpu_ring_commit(ring);
		spin_unlock(&adev->gfx.kiq.ring_lock);
		r = amdgpu_fence_wait_polling(ring, seq, adev->usec_timeout);
		if (r < 1) {
			dev_err(adev->dev, "wait for kiq fence error: %ld.\n", r);
			up_read(&adev->reset_sem);
			return -ETIME;
		}
		up_read(&adev->reset_sem);
		return 0;
	}

	for (vmid = 1; vmid < 16; vmid++) {

		ret = gmc_v9_0_get_atc_vmid_pasid_mapping_info(adev, vmid,
				&queried_pasid);
		if (ret && queried_pasid == pasid) {
			if (all_hub) {
				for (i = 0; i < adev->num_vmhubs; i++)
					gmc_v9_0_flush_gpu_tlb(adev, vmid,
							i, flush_type);
			} else {
				gmc_v9_0_flush_gpu_tlb(adev, vmid,
						AMDGPU_GFXHUB_0, flush_type);
			}
			break;
		}
	}

	return 0;

>>>>>>> upstream/android-13
}

static uint64_t gmc_v9_0_emit_flush_gpu_tlb(struct amdgpu_ring *ring,
					    unsigned vmid, uint64_t pd_addr)
{
<<<<<<< HEAD
	struct amdgpu_device *adev = ring->adev;
	struct amdgpu_vmhub *hub = &adev->vmhub[ring->funcs->vmhub];
	uint32_t req = gmc_v9_0_get_invalidate_req(vmid);
	uint64_t flags = AMDGPU_PTE_VALID;
	unsigned eng = ring->vm_inv_eng;

	amdgpu_gmc_get_vm_pde(adev, -1, &pd_addr, &flags);
	pd_addr |= flags;

	amdgpu_ring_emit_wreg(ring, hub->ctx0_ptb_addr_lo32 + (2 * vmid),
			      lower_32_bits(pd_addr));

	amdgpu_ring_emit_wreg(ring, hub->ctx0_ptb_addr_hi32 + (2 * vmid),
			      upper_32_bits(pd_addr));

	amdgpu_ring_emit_reg_write_reg_wait(ring, hub->vm_inv_eng0_req + eng,
					    hub->vm_inv_eng0_ack + eng,
					    req, 1 << vmid);

=======
	bool use_semaphore = gmc_v9_0_use_invalidate_semaphore(ring->adev, ring->funcs->vmhub);
	struct amdgpu_device *adev = ring->adev;
	struct amdgpu_vmhub *hub = &adev->vmhub[ring->funcs->vmhub];
	uint32_t req = gmc_v9_0_get_invalidate_req(vmid, 0);
	unsigned eng = ring->vm_inv_eng;

	/*
	 * It may lose gpuvm invalidate acknowldege state across power-gating
	 * off cycle, add semaphore acquire before invalidation and semaphore
	 * release after invalidation to avoid entering power gated state
	 * to WA the Issue
	 */

	/* TODO: It needs to continue working on debugging with semaphore for GFXHUB as well. */
	if (use_semaphore)
		/* a read return value of 1 means semaphore acuqire */
		amdgpu_ring_emit_reg_wait(ring,
					  hub->vm_inv_eng0_sem +
					  hub->eng_distance * eng, 0x1, 0x1);

	amdgpu_ring_emit_wreg(ring, hub->ctx0_ptb_addr_lo32 +
			      (hub->ctx_addr_distance * vmid),
			      lower_32_bits(pd_addr));

	amdgpu_ring_emit_wreg(ring, hub->ctx0_ptb_addr_hi32 +
			      (hub->ctx_addr_distance * vmid),
			      upper_32_bits(pd_addr));

	amdgpu_ring_emit_reg_write_reg_wait(ring, hub->vm_inv_eng0_req +
					    hub->eng_distance * eng,
					    hub->vm_inv_eng0_ack +
					    hub->eng_distance * eng,
					    req, 1 << vmid);

	/* TODO: It needs to continue working on debugging with semaphore for GFXHUB as well. */
	if (use_semaphore)
		/*
		 * add semaphore release after invalidation,
		 * write with 0 means semaphore release
		 */
		amdgpu_ring_emit_wreg(ring, hub->vm_inv_eng0_sem +
				      hub->eng_distance * eng, 0);

>>>>>>> upstream/android-13
	return pd_addr;
}

static void gmc_v9_0_emit_pasid_mapping(struct amdgpu_ring *ring, unsigned vmid,
					unsigned pasid)
{
	struct amdgpu_device *adev = ring->adev;
	uint32_t reg;

<<<<<<< HEAD
	if (ring->funcs->vmhub == AMDGPU_GFXHUB)
=======
	/* Do nothing because there's no lut register for mmhub1. */
	if (ring->funcs->vmhub == AMDGPU_MMHUB_1)
		return;

	if (ring->funcs->vmhub == AMDGPU_GFXHUB_0)
>>>>>>> upstream/android-13
		reg = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_VMID_0_LUT) + vmid;
	else
		reg = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_VMID_0_LUT_MM) + vmid;

	amdgpu_ring_emit_wreg(ring, reg, pasid);
}

<<<<<<< HEAD
/**
 * gmc_v9_0_set_pte_pde - update the page tables using MMIO
 *
 * @adev: amdgpu_device pointer
 * @cpu_pt_addr: cpu address of the page table
 * @gpu_page_idx: entry in the page table to update
 * @addr: dst addr to write into pte/pde
 * @flags: access flags
 *
 * Update the page tables using the CPU.
 */
static int gmc_v9_0_set_pte_pde(struct amdgpu_device *adev, void *cpu_pt_addr,
				uint32_t gpu_page_idx, uint64_t addr,
				uint64_t flags)
{
	void __iomem *ptr = (void *)cpu_pt_addr;
	uint64_t value;

	/*
	 * PTE format on VEGA 10:
	 * 63:59 reserved
	 * 58:57 mtype
	 * 56 F
	 * 55 L
	 * 54 P
	 * 53 SW
	 * 52 T
	 * 50:48 reserved
	 * 47:12 4k physical page base address
	 * 11:7 fragment
	 * 6 write
	 * 5 read
	 * 4 exe
	 * 3 Z
	 * 2 snooped
	 * 1 system
	 * 0 valid
	 *
	 * PDE format on VEGA 10:
	 * 63:59 block fragment size
	 * 58:55 reserved
	 * 54 P
	 * 53:48 reserved
	 * 47:6 physical base address of PD or PTE
	 * 5:3 reserved
	 * 2 C
	 * 1 system
	 * 0 valid
	 */

	/*
	 * The following is for PTE only. GART does not have PDEs.
	*/
	value = addr & 0x0000FFFFFFFFF000ULL;
	value |= flags;
	writeq(value, ptr + (gpu_page_idx * 8));
	return 0;
}

static uint64_t gmc_v9_0_get_vm_pte_flags(struct amdgpu_device *adev,
						uint32_t flags)

{
	uint64_t pte_flag = 0;

	if (flags & AMDGPU_VM_PAGE_EXECUTABLE)
		pte_flag |= AMDGPU_PTE_EXECUTABLE;
	if (flags & AMDGPU_VM_PAGE_READABLE)
		pte_flag |= AMDGPU_PTE_READABLE;
	if (flags & AMDGPU_VM_PAGE_WRITEABLE)
		pte_flag |= AMDGPU_PTE_WRITEABLE;

	switch (flags & AMDGPU_VM_MTYPE_MASK) {
	case AMDGPU_VM_MTYPE_DEFAULT:
		pte_flag |= AMDGPU_PTE_MTYPE(MTYPE_NC);
		break;
	case AMDGPU_VM_MTYPE_NC:
		pte_flag |= AMDGPU_PTE_MTYPE(MTYPE_NC);
		break;
	case AMDGPU_VM_MTYPE_WC:
		pte_flag |= AMDGPU_PTE_MTYPE(MTYPE_WC);
		break;
	case AMDGPU_VM_MTYPE_CC:
		pte_flag |= AMDGPU_PTE_MTYPE(MTYPE_CC);
		break;
	case AMDGPU_VM_MTYPE_UC:
		pte_flag |= AMDGPU_PTE_MTYPE(MTYPE_UC);
		break;
	default:
		pte_flag |= AMDGPU_PTE_MTYPE(MTYPE_NC);
		break;
	}

	if (flags & AMDGPU_VM_PAGE_PRT)
		pte_flag |= AMDGPU_PTE_PRT;

	return pte_flag;
=======
/*
 * PTE format on VEGA 10:
 * 63:59 reserved
 * 58:57 mtype
 * 56 F
 * 55 L
 * 54 P
 * 53 SW
 * 52 T
 * 50:48 reserved
 * 47:12 4k physical page base address
 * 11:7 fragment
 * 6 write
 * 5 read
 * 4 exe
 * 3 Z
 * 2 snooped
 * 1 system
 * 0 valid
 *
 * PDE format on VEGA 10:
 * 63:59 block fragment size
 * 58:55 reserved
 * 54 P
 * 53:48 reserved
 * 47:6 physical base address of PD or PTE
 * 5:3 reserved
 * 2 C
 * 1 system
 * 0 valid
 */

static uint64_t gmc_v9_0_map_mtype(struct amdgpu_device *adev, uint32_t flags)

{
	switch (flags) {
	case AMDGPU_VM_MTYPE_DEFAULT:
		return AMDGPU_PTE_MTYPE_VG10(MTYPE_NC);
	case AMDGPU_VM_MTYPE_NC:
		return AMDGPU_PTE_MTYPE_VG10(MTYPE_NC);
	case AMDGPU_VM_MTYPE_WC:
		return AMDGPU_PTE_MTYPE_VG10(MTYPE_WC);
	case AMDGPU_VM_MTYPE_RW:
		return AMDGPU_PTE_MTYPE_VG10(MTYPE_RW);
	case AMDGPU_VM_MTYPE_CC:
		return AMDGPU_PTE_MTYPE_VG10(MTYPE_CC);
	case AMDGPU_VM_MTYPE_UC:
		return AMDGPU_PTE_MTYPE_VG10(MTYPE_UC);
	default:
		return AMDGPU_PTE_MTYPE_VG10(MTYPE_NC);
	}
>>>>>>> upstream/android-13
}

static void gmc_v9_0_get_vm_pde(struct amdgpu_device *adev, int level,
				uint64_t *addr, uint64_t *flags)
{
<<<<<<< HEAD
	if (!(*flags & AMDGPU_PDE_PTE))
		*addr = adev->vm_manager.vram_base_offset + *addr -
			adev->gmc.vram_start;
=======
	if (!(*flags & AMDGPU_PDE_PTE) && !(*flags & AMDGPU_PTE_SYSTEM))
		*addr = amdgpu_gmc_vram_mc2pa(adev, *addr);
>>>>>>> upstream/android-13
	BUG_ON(*addr & 0xFFFF00000000003FULL);

	if (!adev->gmc.translate_further)
		return;

	if (level == AMDGPU_VM_PDB1) {
		/* Set the block fragment size */
		if (!(*flags & AMDGPU_PDE_PTE))
			*flags |= AMDGPU_PDE_BFS(0x9);

	} else if (level == AMDGPU_VM_PDB0) {
		if (*flags & AMDGPU_PDE_PTE)
			*flags &= ~AMDGPU_PDE_PTE;
		else
			*flags |= AMDGPU_PTE_TF;
	}
}

<<<<<<< HEAD
static const struct amdgpu_gmc_funcs gmc_v9_0_gmc_funcs = {
	.flush_gpu_tlb = gmc_v9_0_flush_gpu_tlb,
	.emit_flush_gpu_tlb = gmc_v9_0_emit_flush_gpu_tlb,
	.emit_pasid_mapping = gmc_v9_0_emit_pasid_mapping,
	.set_pte_pde = gmc_v9_0_set_pte_pde,
	.get_vm_pte_flags = gmc_v9_0_get_vm_pte_flags,
	.get_vm_pde = gmc_v9_0_get_vm_pde
=======
static void gmc_v9_0_get_vm_pte(struct amdgpu_device *adev,
				struct amdgpu_bo_va_mapping *mapping,
				uint64_t *flags)
{
	*flags &= ~AMDGPU_PTE_EXECUTABLE;
	*flags |= mapping->flags & AMDGPU_PTE_EXECUTABLE;

	*flags &= ~AMDGPU_PTE_MTYPE_VG10_MASK;
	*flags |= mapping->flags & AMDGPU_PTE_MTYPE_VG10_MASK;

	if (mapping->flags & AMDGPU_PTE_PRT) {
		*flags |= AMDGPU_PTE_PRT;
		*flags &= ~AMDGPU_PTE_VALID;
	}

	if ((adev->asic_type == CHIP_ARCTURUS ||
	    adev->asic_type == CHIP_ALDEBARAN) &&
	    !(*flags & AMDGPU_PTE_SYSTEM) &&
	    mapping->bo_va->is_xgmi)
		*flags |= AMDGPU_PTE_SNOOPED;

	if (adev->asic_type == CHIP_ALDEBARAN)
		*flags |= mapping->flags & AMDGPU_PTE_SNOOPED;
}

static unsigned gmc_v9_0_get_vbios_fb_size(struct amdgpu_device *adev)
{
	u32 d1vga_control = RREG32_SOC15(DCE, 0, mmD1VGA_CONTROL);
	unsigned size;

	/* TODO move to DC so GMC doesn't need to hard-code DCN registers */

	if (REG_GET_FIELD(d1vga_control, D1VGA_CONTROL, D1VGA_MODE_ENABLE)) {
		size = AMDGPU_VBIOS_VGA_ALLOCATION;
	} else {
		u32 viewport;

		switch (adev->asic_type) {
		case CHIP_RAVEN:
			viewport = RREG32_SOC15(DCE, 0, mmHUBP0_DCSURF_PRI_VIEWPORT_DIMENSION);
			size = (REG_GET_FIELD(viewport,
					      HUBP0_DCSURF_PRI_VIEWPORT_DIMENSION, PRI_VIEWPORT_HEIGHT) *
				REG_GET_FIELD(viewport,
					      HUBP0_DCSURF_PRI_VIEWPORT_DIMENSION, PRI_VIEWPORT_WIDTH) *
				4);
			break;
		case CHIP_RENOIR:
			viewport = RREG32_SOC15(DCE, 0, mmHUBP0_DCSURF_PRI_VIEWPORT_DIMENSION_DCN2);
			size = (REG_GET_FIELD(viewport,
					      HUBP0_DCSURF_PRI_VIEWPORT_DIMENSION, PRI_VIEWPORT_HEIGHT) *
				REG_GET_FIELD(viewport,
					      HUBP0_DCSURF_PRI_VIEWPORT_DIMENSION, PRI_VIEWPORT_WIDTH) *
				4);
			break;
		case CHIP_VEGA10:
		case CHIP_VEGA12:
		case CHIP_VEGA20:
		default:
			viewport = RREG32_SOC15(DCE, 0, mmSCL0_VIEWPORT_SIZE);
			size = (REG_GET_FIELD(viewport, SCL0_VIEWPORT_SIZE, VIEWPORT_HEIGHT) *
				REG_GET_FIELD(viewport, SCL0_VIEWPORT_SIZE, VIEWPORT_WIDTH) *
				4);
			break;
		}
	}

	return size;
}

static const struct amdgpu_gmc_funcs gmc_v9_0_gmc_funcs = {
	.flush_gpu_tlb = gmc_v9_0_flush_gpu_tlb,
	.flush_gpu_tlb_pasid = gmc_v9_0_flush_gpu_tlb_pasid,
	.emit_flush_gpu_tlb = gmc_v9_0_emit_flush_gpu_tlb,
	.emit_pasid_mapping = gmc_v9_0_emit_pasid_mapping,
	.map_mtype = gmc_v9_0_map_mtype,
	.get_vm_pde = gmc_v9_0_get_vm_pde,
	.get_vm_pte = gmc_v9_0_get_vm_pte,
	.get_vbios_fb_size = gmc_v9_0_get_vbios_fb_size,
>>>>>>> upstream/android-13
};

static void gmc_v9_0_set_gmc_funcs(struct amdgpu_device *adev)
{
<<<<<<< HEAD
	if (adev->gmc.gmc_funcs == NULL)
		adev->gmc.gmc_funcs = &gmc_v9_0_gmc_funcs;
=======
	adev->gmc.gmc_funcs = &gmc_v9_0_gmc_funcs;
}

static void gmc_v9_0_set_umc_funcs(struct amdgpu_device *adev)
{
	switch (adev->asic_type) {
	case CHIP_VEGA10:
		adev->umc.funcs = &umc_v6_0_funcs;
		break;
	case CHIP_VEGA20:
		adev->umc.max_ras_err_cnt_per_query = UMC_V6_1_TOTAL_CHANNEL_NUM;
		adev->umc.channel_inst_num = UMC_V6_1_CHANNEL_INSTANCE_NUM;
		adev->umc.umc_inst_num = UMC_V6_1_UMC_INSTANCE_NUM;
		adev->umc.channel_offs = UMC_V6_1_PER_CHANNEL_OFFSET_VG20;
		adev->umc.channel_idx_tbl = &umc_v6_1_channel_idx_tbl[0][0];
		adev->umc.ras_funcs = &umc_v6_1_ras_funcs;
		break;
	case CHIP_ARCTURUS:
		adev->umc.max_ras_err_cnt_per_query = UMC_V6_1_TOTAL_CHANNEL_NUM;
		adev->umc.channel_inst_num = UMC_V6_1_CHANNEL_INSTANCE_NUM;
		adev->umc.umc_inst_num = UMC_V6_1_UMC_INSTANCE_NUM;
		adev->umc.channel_offs = UMC_V6_1_PER_CHANNEL_OFFSET_ARCT;
		adev->umc.channel_idx_tbl = &umc_v6_1_channel_idx_tbl[0][0];
		adev->umc.ras_funcs = &umc_v6_1_ras_funcs;
		break;
	case CHIP_ALDEBARAN:
		adev->umc.max_ras_err_cnt_per_query = UMC_V6_7_TOTAL_CHANNEL_NUM;
		adev->umc.channel_inst_num = UMC_V6_7_CHANNEL_INSTANCE_NUM;
		adev->umc.umc_inst_num = UMC_V6_7_UMC_INSTANCE_NUM;
		adev->umc.channel_offs = UMC_V6_7_PER_CHANNEL_OFFSET;
		if (!adev->gmc.xgmi.connected_to_cpu)
			adev->umc.ras_funcs = &umc_v6_7_ras_funcs;
		if (1 & adev->smuio.funcs->get_die_id(adev))
			adev->umc.channel_idx_tbl = &umc_v6_7_channel_idx_tbl_first[0][0];
		else
			adev->umc.channel_idx_tbl = &umc_v6_7_channel_idx_tbl_second[0][0];
		break;
	default:
		break;
	}
}

static void gmc_v9_0_set_mmhub_funcs(struct amdgpu_device *adev)
{
	switch (adev->asic_type) {
	case CHIP_ARCTURUS:
		adev->mmhub.funcs = &mmhub_v9_4_funcs;
		break;
	case CHIP_ALDEBARAN:
		adev->mmhub.funcs = &mmhub_v1_7_funcs;
		break;
	default:
		adev->mmhub.funcs = &mmhub_v1_0_funcs;
		break;
	}
}

static void gmc_v9_0_set_mmhub_ras_funcs(struct amdgpu_device *adev)
{
	switch (adev->asic_type) {
	case CHIP_VEGA20:
		adev->mmhub.ras_funcs = &mmhub_v1_0_ras_funcs;
		break;
	case CHIP_ARCTURUS:
		adev->mmhub.ras_funcs = &mmhub_v9_4_ras_funcs;
		break;
	case CHIP_ALDEBARAN:
		adev->mmhub.ras_funcs = &mmhub_v1_7_ras_funcs;
		break;
	default:
		/* mmhub ras is not available */
		break;
	}
}

static void gmc_v9_0_set_gfxhub_funcs(struct amdgpu_device *adev)
{
	adev->gfxhub.funcs = &gfxhub_v1_0_funcs;
}

static void gmc_v9_0_set_hdp_ras_funcs(struct amdgpu_device *adev)
{
	adev->hdp.ras_funcs = &hdp_v4_0_ras_funcs;
}

static void gmc_v9_0_set_mca_funcs(struct amdgpu_device *adev)
{
	switch (adev->asic_type) {
	case CHIP_ALDEBARAN:
		if (!adev->gmc.xgmi.connected_to_cpu)
			adev->mca.funcs = &mca_v3_0_funcs;
		break;
	default:
		break;
	}
>>>>>>> upstream/android-13
}

static int gmc_v9_0_early_init(void *handle)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

<<<<<<< HEAD
	gmc_v9_0_set_gmc_funcs(adev);
	gmc_v9_0_set_irq_funcs(adev);
=======
	if (adev->asic_type == CHIP_VEGA20 ||
	    adev->asic_type == CHIP_ARCTURUS)
		adev->gmc.xgmi.supported = true;

	if (adev->asic_type == CHIP_ALDEBARAN) {
		adev->gmc.xgmi.supported = true;
		adev->gmc.xgmi.connected_to_cpu =
			adev->smuio.funcs->is_host_gpu_xgmi_supported(adev);
	}

	gmc_v9_0_set_gmc_funcs(adev);
	gmc_v9_0_set_irq_funcs(adev);
	gmc_v9_0_set_umc_funcs(adev);
	gmc_v9_0_set_mmhub_funcs(adev);
	gmc_v9_0_set_mmhub_ras_funcs(adev);
	gmc_v9_0_set_gfxhub_funcs(adev);
	gmc_v9_0_set_hdp_ras_funcs(adev);
	gmc_v9_0_set_mca_funcs(adev);
>>>>>>> upstream/android-13

	adev->gmc.shared_aperture_start = 0x2000000000000000ULL;
	adev->gmc.shared_aperture_end =
		adev->gmc.shared_aperture_start + (4ULL << 30) - 1;
	adev->gmc.private_aperture_start = 0x1000000000000000ULL;
	adev->gmc.private_aperture_end =
		adev->gmc.private_aperture_start + (4ULL << 30) - 1;

	return 0;
}

<<<<<<< HEAD
static int gmc_v9_0_ecc_available(struct amdgpu_device *adev)
{
	uint32_t reg_val;
	uint32_t reg_addr;
	uint32_t field_val;
	size_t i;
	uint32_t fv2;
	size_t lost_sheep;

	DRM_DEBUG("ecc: gmc_v9_0_ecc_available()\n");

	lost_sheep = 0;
	for (i = 0; i < ARRAY_SIZE(ecc_umclocalcap_addrs); ++i) {
		reg_addr = ecc_umclocalcap_addrs[i];
		DRM_DEBUG("ecc: "
			  "UMCCH_UmcLocalCap[%zu]: reg_addr: 0x%08x\n",
			  i, reg_addr);
		reg_val = RREG32(reg_addr);
		field_val = REG_GET_FIELD(reg_val, UMCCH0_0_UmcLocalCap,
					  EccDis);
		DRM_DEBUG("ecc: "
			  "reg_val: 0x%08x, "
			  "EccDis: 0x%08x, ",
			  reg_val, field_val);
		if (field_val) {
			DRM_ERROR("ecc: UmcLocalCap:EccDis is set.\n");
			++lost_sheep;
		}
	}

	for (i = 0; i < ARRAY_SIZE(ecc_umcch_umc_config_addrs); ++i) {
		reg_addr = ecc_umcch_umc_config_addrs[i];
		DRM_DEBUG("ecc: "
			  "UMCCH0_0_UMC_CONFIG[%zu]: reg_addr: 0x%08x",
			  i, reg_addr);
		reg_val = RREG32(reg_addr);
		field_val = REG_GET_FIELD(reg_val, UMCCH0_0_UMC_CONFIG,
					  DramReady);
		DRM_DEBUG("ecc: "
			  "reg_val: 0x%08x, "
			  "DramReady: 0x%08x\n",
			  reg_val, field_val);

		if (!field_val) {
			DRM_ERROR("ecc: UMC_CONFIG:DramReady is not set.\n");
			++lost_sheep;
		}
	}

	for (i = 0; i < ARRAY_SIZE(ecc_umcch_eccctrl_addrs); ++i) {
		reg_addr = ecc_umcch_eccctrl_addrs[i];
		DRM_DEBUG("ecc: "
			  "UMCCH_EccCtrl[%zu]: reg_addr: 0x%08x, ",
			  i, reg_addr);
		reg_val = RREG32(reg_addr);
		field_val = REG_GET_FIELD(reg_val, UMCCH0_0_EccCtrl,
					  WrEccEn);
		fv2 = REG_GET_FIELD(reg_val, UMCCH0_0_EccCtrl,
				    RdEccEn);
		DRM_DEBUG("ecc: "
			  "reg_val: 0x%08x, "
			  "WrEccEn: 0x%08x, "
			  "RdEccEn: 0x%08x\n",
			  reg_val, field_val, fv2);

		if (!field_val) {
			DRM_DEBUG("ecc: WrEccEn is not set\n");
			++lost_sheep;
		}
		if (!fv2) {
			DRM_DEBUG("ecc: RdEccEn is not set\n");
			++lost_sheep;
		}
	}

	DRM_DEBUG("ecc: lost_sheep: %zu\n", lost_sheep);
	return lost_sheep == 0;
}

static int gmc_v9_0_late_init(void *handle)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;
	/*
	 * The latest engine allocation on gfx9 is:
	 * Engine 0, 1: idle
	 * Engine 2, 3: firmware
	 * Engine 4~13: amdgpu ring, subject to change when ring number changes
	 * Engine 14~15: idle
	 * Engine 16: kfd tlb invalidation
	 * Engine 17: Gart flushes
	 */
	unsigned vm_inv_eng[AMDGPU_MAX_VMHUBS] = { 4, 4 };
	unsigned i;
	int r;

	/*
	 * TODO - Uncomment once GART corruption issue is fixed.
	 */
	/* amdgpu_bo_late_init(adev); */

	for(i = 0; i < adev->num_rings; ++i) {
		struct amdgpu_ring *ring = adev->rings[i];
		unsigned vmhub = ring->funcs->vmhub;

		ring->vm_inv_eng = vm_inv_eng[vmhub]++;
		dev_info(adev->dev, "ring %u(%s) uses VM inv eng %u on hub %u\n",
			 ring->idx, ring->name, ring->vm_inv_eng,
			 ring->funcs->vmhub);
	}

	/* Engine 16 is used for KFD and 17 for GART flushes */
	for(i = 0; i < AMDGPU_MAX_VMHUBS; ++i)
		BUG_ON(vm_inv_eng[i] > 16);

	if (adev->asic_type == CHIP_VEGA10 && !amdgpu_sriov_vf(adev)) {
		r = gmc_v9_0_ecc_available(adev);
		if (r == 1) {
			DRM_INFO("ECC is active.\n");
		} else if (r == 0) {
			DRM_INFO("ECC is not present.\n");
			adev->df_funcs->enable_ecc_force_par_wr_rmw(adev, false);
		} else {
			DRM_ERROR("gmc_v9_0_ecc_available() failed. r: %d\n", r);
			return r;
		}
	}

=======
static int gmc_v9_0_late_init(void *handle)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;
	int r;

	r = amdgpu_gmc_allocate_vm_inv_eng(adev);
	if (r)
		return r;

	/*
	 * Workaround performance drop issue with VBIOS enables partial
	 * writes, while disables HBM ECC for vega10.
	 */
	if (!amdgpu_sriov_vf(adev) && (adev->asic_type == CHIP_VEGA10)) {
		if (!(adev->ras_enabled & (1 << AMDGPU_RAS_BLOCK__UMC))) {
			if (adev->df.funcs->enable_ecc_force_par_wr_rmw)
				adev->df.funcs->enable_ecc_force_par_wr_rmw(adev, false);
		}
	}

	if (!amdgpu_persistent_edc_harvesting_supported(adev)) {
		if (adev->mmhub.ras_funcs &&
		    adev->mmhub.ras_funcs->reset_ras_error_count)
			adev->mmhub.ras_funcs->reset_ras_error_count(adev);

		if (adev->hdp.ras_funcs &&
		    adev->hdp.ras_funcs->reset_ras_error_count)
			adev->hdp.ras_funcs->reset_ras_error_count(adev);
	}

	r = amdgpu_gmc_ras_late_init(adev);
	if (r)
		return r;

>>>>>>> upstream/android-13
	return amdgpu_irq_get(adev, &adev->gmc.vm_fault, 0);
}

static void gmc_v9_0_vram_gtt_location(struct amdgpu_device *adev,
					struct amdgpu_gmc *mc)
{
<<<<<<< HEAD
	u64 base = 0;
	if (!amdgpu_sriov_vf(adev))
		base = mmhub_v1_0_get_fb_location(adev);
	amdgpu_device_vram_location(adev, &adev->gmc, base);
	amdgpu_device_gart_location(adev, mc);
	/* base offset of vram pages */
	adev->vm_manager.vram_base_offset = gfxhub_v1_0_get_mc_fb_offset(adev);
=======
	u64 base = adev->mmhub.funcs->get_fb_location(adev);

	/* add the xgmi offset of the physical node */
	base += adev->gmc.xgmi.physical_node_id * adev->gmc.xgmi.node_segment_size;
	if (adev->gmc.xgmi.connected_to_cpu) {
		amdgpu_gmc_sysvm_location(adev, mc);
	} else {
		amdgpu_gmc_vram_location(adev, mc, base);
		amdgpu_gmc_gart_location(adev, mc);
		amdgpu_gmc_agp_location(adev, mc);
	}
	/* base offset of vram pages */
	adev->vm_manager.vram_base_offset = adev->gfxhub.funcs->get_mc_fb_offset(adev);

	/* XXX: add the xgmi offset of the physical node? */
	adev->vm_manager.vram_base_offset +=
		adev->gmc.xgmi.physical_node_id * adev->gmc.xgmi.node_segment_size;
>>>>>>> upstream/android-13
}

/**
 * gmc_v9_0_mc_init - initialize the memory controller driver params
 *
 * @adev: amdgpu_device pointer
 *
 * Look up the amount of vram, vram width, and decide how to place
 * vram and gart within the GPU's physical address space.
 * Returns 0 for success.
 */
static int gmc_v9_0_mc_init(struct amdgpu_device *adev)
{
<<<<<<< HEAD
	int chansize, numchan;
	int r;

	if (amdgpu_emu_mode != 1)
		adev->gmc.vram_width = amdgpu_atomfirmware_get_vram_width(adev);
	if (!adev->gmc.vram_width) {
		/* hbm memory channel size */
		if (adev->flags & AMD_IS_APU)
			chansize = 64;
		else
			chansize = 128;

		numchan = adev->df_funcs->get_hbm_channel_number(adev);
		adev->gmc.vram_width = numchan * chansize;
	}

	/* size in MB on si */
	adev->gmc.mc_vram_size =
		adev->nbio_funcs->get_memsize(adev) * 1024ULL * 1024ULL;
	adev->gmc.real_vram_size = adev->gmc.mc_vram_size;

	if (!(adev->flags & AMD_IS_APU)) {
=======
	int r;

	/* size in MB on si */
	adev->gmc.mc_vram_size =
		adev->nbio.funcs->get_memsize(adev) * 1024ULL * 1024ULL;
	adev->gmc.real_vram_size = adev->gmc.mc_vram_size;

	if (!(adev->flags & AMD_IS_APU) &&
	    !adev->gmc.xgmi.connected_to_cpu) {
>>>>>>> upstream/android-13
		r = amdgpu_device_resize_fb_bar(adev);
		if (r)
			return r;
	}
	adev->gmc.aper_base = pci_resource_start(adev->pdev, 0);
	adev->gmc.aper_size = pci_resource_len(adev->pdev, 0);

#ifdef CONFIG_X86_64
<<<<<<< HEAD
	if (adev->flags & AMD_IS_APU) {
		adev->gmc.aper_base = gfxhub_v1_0_get_mc_fb_offset(adev);
		adev->gmc.aper_size = adev->gmc.real_vram_size;
	}
=======
	/*
	 * AMD Accelerated Processing Platform (APP) supporting GPU-HOST xgmi
	 * interface can use VRAM through here as it appears system reserved
	 * memory in host address space.
	 *
	 * For APUs, VRAM is just the stolen system memory and can be accessed
	 * directly.
	 *
	 * Otherwise, use the legacy Host Data Path (HDP) through PCIe BAR.
	 */

	/* check whether both host-gpu and gpu-gpu xgmi links exist */
	if (((adev->flags & AMD_IS_APU) && !amdgpu_passthrough(adev)) ||
	    (adev->gmc.xgmi.supported &&
	     adev->gmc.xgmi.connected_to_cpu)) {
		adev->gmc.aper_base =
			adev->gfxhub.funcs->get_mc_fb_offset(adev) +
			adev->gmc.xgmi.physical_node_id *
			adev->gmc.xgmi.node_segment_size;
		adev->gmc.aper_size = adev->gmc.real_vram_size;
	}

>>>>>>> upstream/android-13
#endif
	/* In case the PCI BAR is larger than the actual amount of vram */
	adev->gmc.visible_vram_size = adev->gmc.aper_size;
	if (adev->gmc.visible_vram_size > adev->gmc.real_vram_size)
		adev->gmc.visible_vram_size = adev->gmc.real_vram_size;

	/* set the gart size */
	if (amdgpu_gart_size == -1) {
		switch (adev->asic_type) {
		case CHIP_VEGA10:  /* all engines support GPUVM */
		case CHIP_VEGA12:  /* all engines support GPUVM */
		case CHIP_VEGA20:
<<<<<<< HEAD
=======
		case CHIP_ARCTURUS:
		case CHIP_ALDEBARAN:
>>>>>>> upstream/android-13
		default:
			adev->gmc.gart_size = 512ULL << 20;
			break;
		case CHIP_RAVEN:   /* DCE SG support */
<<<<<<< HEAD
=======
		case CHIP_RENOIR:
>>>>>>> upstream/android-13
			adev->gmc.gart_size = 1024ULL << 20;
			break;
		}
	} else {
		adev->gmc.gart_size = (u64)amdgpu_gart_size << 20;
	}

<<<<<<< HEAD
=======
	adev->gmc.gart_size += adev->pm.smu_prv_buffer_size;

>>>>>>> upstream/android-13
	gmc_v9_0_vram_gtt_location(adev, &adev->gmc);

	return 0;
}

static int gmc_v9_0_gart_init(struct amdgpu_device *adev)
{
	int r;

<<<<<<< HEAD
	if (adev->gart.robj) {
		WARN(1, "VEGA10 PCIE GART already initialized\n");
		return 0;
	}
=======
	if (adev->gart.bo) {
		WARN(1, "VEGA10 PCIE GART already initialized\n");
		return 0;
	}

	if (adev->gmc.xgmi.connected_to_cpu) {
		adev->gmc.vmid0_page_table_depth = 1;
		adev->gmc.vmid0_page_table_block_size = 12;
	} else {
		adev->gmc.vmid0_page_table_depth = 0;
		adev->gmc.vmid0_page_table_block_size = 0;
	}

>>>>>>> upstream/android-13
	/* Initialize common gart structure */
	r = amdgpu_gart_init(adev);
	if (r)
		return r;
	adev->gart.table_size = adev->gart.num_gpu_pages * 8;
<<<<<<< HEAD
	adev->gart.gart_pte_flags = AMDGPU_PTE_MTYPE(MTYPE_UC) |
				 AMDGPU_PTE_EXECUTABLE;
	return amdgpu_gart_table_vram_alloc(adev);
}

static unsigned gmc_v9_0_get_vbios_fb_size(struct amdgpu_device *adev)
{
#if 0
	u32 d1vga_control = RREG32_SOC15(DCE, 0, mmD1VGA_CONTROL);
#endif
	unsigned size;

	/*
	 * TODO Remove once GART corruption is resolved
	 * Check related code in gmc_v9_0_sw_fini
	 * */
	size = 9 * 1024 * 1024;

#if 0
	if (REG_GET_FIELD(d1vga_control, D1VGA_CONTROL, D1VGA_MODE_ENABLE)) {
		size = 9 * 1024 * 1024; /* reserve 8MB for vga emulator and 1 MB for FB */
	} else {
		u32 viewport;

		switch (adev->asic_type) {
		case CHIP_RAVEN:
			viewport = RREG32_SOC15(DCE, 0, mmHUBP0_DCSURF_PRI_VIEWPORT_DIMENSION);
			size = (REG_GET_FIELD(viewport,
					      HUBP0_DCSURF_PRI_VIEWPORT_DIMENSION, PRI_VIEWPORT_HEIGHT) *
				REG_GET_FIELD(viewport,
					      HUBP0_DCSURF_PRI_VIEWPORT_DIMENSION, PRI_VIEWPORT_WIDTH) *
				4);
			break;
		case CHIP_VEGA10:
		case CHIP_VEGA12:
		default:
			viewport = RREG32_SOC15(DCE, 0, mmSCL0_VIEWPORT_SIZE);
			size = (REG_GET_FIELD(viewport, SCL0_VIEWPORT_SIZE, VIEWPORT_HEIGHT) *
				REG_GET_FIELD(viewport, SCL0_VIEWPORT_SIZE, VIEWPORT_WIDTH) *
				4);
			break;
		}
	}
	/* return 0 if the pre-OS buffer uses up most of vram */
	if ((adev->gmc.real_vram_size - size) < (8 * 1024 * 1024))
		return 0;

#endif
	return size;
=======
	adev->gart.gart_pte_flags = AMDGPU_PTE_MTYPE_VG10(MTYPE_UC) |
				 AMDGPU_PTE_EXECUTABLE;

	r = amdgpu_gart_table_vram_alloc(adev);
	if (r)
		return r;

	if (adev->gmc.xgmi.connected_to_cpu) {
		r = amdgpu_gmc_pdb0_alloc(adev);
	}

	return r;
}

/**
 * gmc_v9_0_save_registers - saves regs
 *
 * @adev: amdgpu_device pointer
 *
 * This saves potential register values that should be
 * restored upon resume
 */
static void gmc_v9_0_save_registers(struct amdgpu_device *adev)
{
	if (adev->asic_type == CHIP_RAVEN)
		adev->gmc.sdpif_register = RREG32_SOC15(DCE, 0, mmDCHUBBUB_SDPIF_MMIO_CNTRL_0);
>>>>>>> upstream/android-13
}

static int gmc_v9_0_sw_init(void *handle)
{
<<<<<<< HEAD
	int r;
	int dma_bits;
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

	gfxhub_v1_0_init(adev);
	mmhub_v1_0_init(adev);

	spin_lock_init(&adev->gmc.invalidate_lock);

	adev->gmc.vram_type = amdgpu_atomfirmware_get_vram_type(adev);
	switch (adev->asic_type) {
	case CHIP_RAVEN:
=======
	int r, vram_width = 0, vram_type = 0, vram_vendor = 0;
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

	adev->gfxhub.funcs->init(adev);

	adev->mmhub.funcs->init(adev);
	if (adev->mca.funcs)
		adev->mca.funcs->init(adev);

	spin_lock_init(&adev->gmc.invalidate_lock);

	r = amdgpu_atomfirmware_get_vram_info(adev,
		&vram_width, &vram_type, &vram_vendor);
	if (amdgpu_sriov_vf(adev))
		/* For Vega10 SR-IOV, vram_width can't be read from ATOM as RAVEN,
		 * and DF related registers is not readable, seems hardcord is the
		 * only way to set the correct vram_width
		 */
		adev->gmc.vram_width = 2048;
	else if (amdgpu_emu_mode != 1)
		adev->gmc.vram_width = vram_width;

	if (!adev->gmc.vram_width) {
		int chansize, numchan;

		/* hbm memory channel size */
		if (adev->flags & AMD_IS_APU)
			chansize = 64;
		else
			chansize = 128;

		numchan = adev->df.funcs->get_hbm_channel_number(adev);
		adev->gmc.vram_width = numchan * chansize;
	}

	adev->gmc.vram_type = vram_type;
	adev->gmc.vram_vendor = vram_vendor;
	switch (adev->asic_type) {
	case CHIP_RAVEN:
		adev->num_vmhubs = 2;

>>>>>>> upstream/android-13
		if (adev->rev_id == 0x0 || adev->rev_id == 0x1) {
			amdgpu_vm_adjust_size(adev, 256 * 1024, 9, 3, 48);
		} else {
			/* vm_size is 128TB + 512GB for legacy 3-level page support */
			amdgpu_vm_adjust_size(adev, 128 * 1024 + 512, 9, 2, 48);
			adev->gmc.translate_further =
				adev->vm_manager.num_level > 1;
		}
		break;
	case CHIP_VEGA10:
	case CHIP_VEGA12:
	case CHIP_VEGA20:
<<<<<<< HEAD
=======
	case CHIP_RENOIR:
	case CHIP_ALDEBARAN:
		adev->num_vmhubs = 2;


>>>>>>> upstream/android-13
		/*
		 * To fulfill 4-level page support,
		 * vm size is 256TB (48bit), maximum size of Vega10,
		 * block size 512 (9bit)
		 */
<<<<<<< HEAD
=======
		/* sriov restrict max_pfn below AMDGPU_GMC_HOLE */
		if (amdgpu_sriov_vf(adev))
			amdgpu_vm_adjust_size(adev, 256 * 1024, 9, 3, 47);
		else
			amdgpu_vm_adjust_size(adev, 256 * 1024, 9, 3, 48);
		break;
	case CHIP_ARCTURUS:
		adev->num_vmhubs = 3;

		/* Keep the vm size same with Vega20 */
>>>>>>> upstream/android-13
		amdgpu_vm_adjust_size(adev, 256 * 1024, 9, 3, 48);
		break;
	default:
		break;
	}

	/* This interrupt is VMC page fault.*/
	r = amdgpu_irq_add_id(adev, SOC15_IH_CLIENTID_VMC, VMC_1_0__SRCID__VM_FAULT,
				&adev->gmc.vm_fault);
<<<<<<< HEAD
=======
	if (r)
		return r;

	if (adev->asic_type == CHIP_ARCTURUS) {
		r = amdgpu_irq_add_id(adev, SOC15_IH_CLIENTID_VMC1, VMC_1_0__SRCID__VM_FAULT,
					&adev->gmc.vm_fault);
		if (r)
			return r;
	}

>>>>>>> upstream/android-13
	r = amdgpu_irq_add_id(adev, SOC15_IH_CLIENTID_UTCL2, UTCL2_1_0__SRCID__FAULT,
				&adev->gmc.vm_fault);

	if (r)
		return r;

<<<<<<< HEAD
=======
	if (!amdgpu_sriov_vf(adev) &&
	    !adev->gmc.xgmi.connected_to_cpu) {
		/* interrupt sent to DF. */
		r = amdgpu_irq_add_id(adev, SOC15_IH_CLIENTID_DF, 0,
				      &adev->gmc.ecc_irq);
		if (r)
			return r;
	}

>>>>>>> upstream/android-13
	/* Set the internal MC address mask
	 * This is the max address of the GPU's
	 * internal address space.
	 */
	adev->gmc.mc_mask = 0xffffffffffffULL; /* 48 bit MC */

<<<<<<< HEAD
	/* set DMA mask + need_dma32 flags.
	 * PCIE - can handle 44-bits.
	 * IGP - can handle 44-bits
	 * PCI - dma32 for legacy pci gart, 44 bits on vega10
	 */
	adev->need_dma32 = false;
	dma_bits = adev->need_dma32 ? 32 : 44;
	r = pci_set_dma_mask(adev->pdev, DMA_BIT_MASK(dma_bits));
	if (r) {
		adev->need_dma32 = true;
		dma_bits = 32;
		printk(KERN_WARNING "amdgpu: No suitable DMA available.\n");
	}
	r = pci_set_consistent_dma_mask(adev->pdev, DMA_BIT_MASK(dma_bits));
	if (r) {
		pci_set_consistent_dma_mask(adev->pdev, DMA_BIT_MASK(32));
		printk(KERN_WARNING "amdgpu: No coherent DMA available.\n");
	}
	adev->need_swiotlb = drm_get_max_iomem() > ((u64)1 << dma_bits);
=======
	r = dma_set_mask_and_coherent(adev->dev, DMA_BIT_MASK(44));
	if (r) {
		printk(KERN_WARNING "amdgpu: No suitable DMA available.\n");
		return r;
	}
	adev->need_swiotlb = drm_need_swiotlb(44);

	if (adev->gmc.xgmi.supported) {
		r = adev->gfxhub.funcs->get_xgmi_info(adev);
		if (r)
			return r;
	}
>>>>>>> upstream/android-13

	r = gmc_v9_0_mc_init(adev);
	if (r)
		return r;

<<<<<<< HEAD
	adev->gmc.stolen_size = gmc_v9_0_get_vbios_fb_size(adev);
=======
	amdgpu_gmc_get_vbios_allocations(adev);
>>>>>>> upstream/android-13

	/* Memory manager */
	r = amdgpu_bo_init(adev);
	if (r)
		return r;

	r = gmc_v9_0_gart_init(adev);
	if (r)
		return r;

	/*
	 * number of VMs
	 * VMID 0 is reserved for System
<<<<<<< HEAD
	 * amdgpu graphics/compute will use VMIDs 1-7
	 * amdkfd will use VMIDs 8-15
	 */
	adev->vm_manager.id_mgr[AMDGPU_GFXHUB].num_ids = AMDGPU_NUM_OF_VMIDS;
	adev->vm_manager.id_mgr[AMDGPU_MMHUB].num_ids = AMDGPU_NUM_OF_VMIDS;

	amdgpu_vm_manager_init(adev);

=======
	 * amdgpu graphics/compute will use VMIDs 1..n-1
	 * amdkfd will use VMIDs n..15
	 *
	 * The first KFD VMID is 8 for GPUs with graphics, 3 for
	 * compute-only GPUs. On compute-only GPUs that leaves 2 VMIDs
	 * for video processing.
	 */
	adev->vm_manager.first_kfd_vmid =
		(adev->asic_type == CHIP_ARCTURUS ||
		 adev->asic_type == CHIP_ALDEBARAN) ? 3 : 8;

	amdgpu_vm_manager_init(adev);

	gmc_v9_0_save_registers(adev);

>>>>>>> upstream/android-13
	return 0;
}

static int gmc_v9_0_sw_fini(void *handle)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

<<<<<<< HEAD
	amdgpu_gem_force_release(adev);
	amdgpu_vm_manager_fini(adev);

	/*
	* TODO:
	* Currently there is a bug where some memory client outside
	* of the driver writes to first 8M of VRAM on S3 resume,
	* this overrides GART which by default gets placed in first 8M and
	* causes VM_FAULTS once GTT is accessed.
	* Keep the stolen memory reservation until the while this is not solved.
	* Also check code in gmc_v9_0_get_vbios_fb_size and gmc_v9_0_late_init
	*/
	amdgpu_bo_free_kernel(&adev->stolen_vga_memory, NULL, NULL);

	amdgpu_gart_table_vram_free(adev);
	amdgpu_bo_fini(adev);
	amdgpu_gart_fini(adev);
=======
	amdgpu_gmc_ras_fini(adev);
	amdgpu_gem_force_release(adev);
	amdgpu_vm_manager_fini(adev);
	amdgpu_gart_table_vram_free(adev);
	amdgpu_bo_free_kernel(&adev->gmc.pdb0_bo, NULL, &adev->gmc.ptr_pdb0);
	amdgpu_bo_fini(adev);
>>>>>>> upstream/android-13

	return 0;
}

static void gmc_v9_0_init_golden_registers(struct amdgpu_device *adev)
{

	switch (adev->asic_type) {
	case CHIP_VEGA10:
<<<<<<< HEAD
=======
		if (amdgpu_sriov_vf(adev))
			break;
		fallthrough;
>>>>>>> upstream/android-13
	case CHIP_VEGA20:
		soc15_program_register_sequence(adev,
						golden_settings_mmhub_1_0_0,
						ARRAY_SIZE(golden_settings_mmhub_1_0_0));
		soc15_program_register_sequence(adev,
						golden_settings_athub_1_0_0,
						ARRAY_SIZE(golden_settings_athub_1_0_0));
		break;
	case CHIP_VEGA12:
		break;
	case CHIP_RAVEN:
<<<<<<< HEAD
=======
		/* TODO for renoir */
>>>>>>> upstream/android-13
		soc15_program_register_sequence(adev,
						golden_settings_athub_1_0_0,
						ARRAY_SIZE(golden_settings_athub_1_0_0));
		break;
	default:
		break;
	}
}

/**
 * gmc_v9_0_restore_registers - restores regs
 *
 * @adev: amdgpu_device pointer
 *
 * This restores register values, saved at suspend.
 */
<<<<<<< HEAD
static void gmc_v9_0_restore_registers(struct amdgpu_device *adev)
{
	if (adev->asic_type == CHIP_RAVEN)
		WREG32(mmDCHUBBUB_SDPIF_MMIO_CNTRL_0, adev->gmc.sdpif_register);
=======
void gmc_v9_0_restore_registers(struct amdgpu_device *adev)
{
	if (adev->asic_type == CHIP_RAVEN) {
		WREG32_SOC15(DCE, 0, mmDCHUBBUB_SDPIF_MMIO_CNTRL_0, adev->gmc.sdpif_register);
		WARN_ON(adev->gmc.sdpif_register !=
			RREG32_SOC15(DCE, 0, mmDCHUBBUB_SDPIF_MMIO_CNTRL_0));
	}
>>>>>>> upstream/android-13
}

/**
 * gmc_v9_0_gart_enable - gart enable
 *
 * @adev: amdgpu_device pointer
 */
static int gmc_v9_0_gart_enable(struct amdgpu_device *adev)
{
	int r;
<<<<<<< HEAD
	bool value;
	u32 tmp;

	amdgpu_device_program_register_sequence(adev,
						golden_settings_vega10_hdp,
						ARRAY_SIZE(golden_settings_vega10_hdp));

	if (adev->gart.robj == NULL) {
		dev_err(adev->dev, "No VRAM object for PCIE GART.\n");
		return -EINVAL;
	}
=======

	if (adev->gmc.xgmi.connected_to_cpu)
		amdgpu_gmc_init_pdb0(adev);

	if (adev->gart.bo == NULL) {
		dev_err(adev->dev, "No VRAM object for PCIE GART.\n");
		return -EINVAL;
	}

	if (amdgpu_sriov_vf(adev) && amdgpu_in_reset(adev))
		goto skip_pin_bo;

>>>>>>> upstream/android-13
	r = amdgpu_gart_table_vram_pin(adev);
	if (r)
		return r;

<<<<<<< HEAD
	switch (adev->asic_type) {
	case CHIP_RAVEN:
		mmhub_v1_0_initialize_power_gating(adev);
		mmhub_v1_0_update_power_gating(adev, true);
		break;
	default:
		break;
	}

	r = gfxhub_v1_0_gart_enable(adev);
	if (r)
		return r;

	r = mmhub_v1_0_gart_enable(adev);
	if (r)
		return r;

	WREG32_FIELD15(HDP, 0, HDP_MISC_CNTL, FLUSH_INVALIDATE_CACHE, 1);

	tmp = RREG32_SOC15(HDP, 0, mmHDP_HOST_PATH_CNTL);
	WREG32_SOC15(HDP, 0, mmHDP_HOST_PATH_CNTL, tmp);

	WREG32_SOC15(HDP, 0, mmHDP_NONSURFACE_BASE, (adev->gmc.vram_start >> 8));
	WREG32_SOC15(HDP, 0, mmHDP_NONSURFACE_BASE_HI, (adev->gmc.vram_start >> 40));

	/* After HDP is initialized, flush HDP.*/
	adev->nbio_funcs->hdp_flush(adev, NULL);

	if (amdgpu_vm_fault_stop == AMDGPU_VM_FAULT_STOP_ALWAYS)
		value = false;
	else
		value = true;

	gfxhub_v1_0_set_fault_enable_default(adev, value);
	mmhub_v1_0_set_fault_enable_default(adev, value);
	gmc_v9_0_flush_gpu_tlb(adev, 0);

	DRM_INFO("PCIE GART of %uM enabled (table at 0x%016llX).\n",
		 (unsigned)(adev->gmc.gart_size >> 20),
		 (unsigned long long)adev->gart.table_addr);
=======
skip_pin_bo:
	r = adev->gfxhub.funcs->gart_enable(adev);
	if (r)
		return r;

	r = adev->mmhub.funcs->gart_enable(adev);
	if (r)
		return r;

	DRM_INFO("PCIE GART of %uM enabled.\n",
		 (unsigned)(adev->gmc.gart_size >> 20));
	if (adev->gmc.pdb0_bo)
		DRM_INFO("PDB0 located at 0x%016llX\n",
				(unsigned long long)amdgpu_bo_gpu_offset(adev->gmc.pdb0_bo));
	DRM_INFO("PTB located at 0x%016llX\n",
			(unsigned long long)amdgpu_bo_gpu_offset(adev->gart.bo));

>>>>>>> upstream/android-13
	adev->gart.ready = true;
	return 0;
}

static int gmc_v9_0_hw_init(void *handle)
{
<<<<<<< HEAD
	int r;
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;
=======
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;
	bool value;
	int r, i;
>>>>>>> upstream/android-13

	/* The sequence of these two function calls matters.*/
	gmc_v9_0_init_golden_registers(adev);

	if (adev->mode_info.num_crtc) {
		/* Lockout access through VGA aperture*/
		WREG32_FIELD15(DCE, 0, VGA_HDP_CONTROL, VGA_MEMORY_DISABLE, 1);
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
		/* disable VGA render */
		WREG32_FIELD15(DCE, 0, VGA_RENDER_CONTROL, VGA_VSTATUS_CNTL, 0);
	}

<<<<<<< HEAD
=======
	if (adev->mmhub.funcs->update_power_gating)
		adev->mmhub.funcs->update_power_gating(adev, true);

	adev->hdp.funcs->init_registers(adev);

	/* After HDP is initialized, flush HDP.*/
	adev->hdp.funcs->flush_hdp(adev, NULL);

	if (amdgpu_vm_fault_stop == AMDGPU_VM_FAULT_STOP_ALWAYS)
		value = false;
	else
		value = true;

	if (!amdgpu_sriov_vf(adev)) {
		adev->gfxhub.funcs->set_fault_enable_default(adev, value);
		adev->mmhub.funcs->set_fault_enable_default(adev, value);
	}
	for (i = 0; i < adev->num_vmhubs; ++i)
		gmc_v9_0_flush_gpu_tlb(adev, 0, i, 0);

	if (adev->umc.funcs && adev->umc.funcs->init_registers)
		adev->umc.funcs->init_registers(adev);

>>>>>>> upstream/android-13
	r = gmc_v9_0_gart_enable(adev);

	return r;
}

/**
<<<<<<< HEAD
 * gmc_v9_0_save_registers - saves regs
 *
 * @adev: amdgpu_device pointer
 *
 * This saves potential register values that should be
 * restored upon resume
 */
static void gmc_v9_0_save_registers(struct amdgpu_device *adev)
{
	if (adev->asic_type == CHIP_RAVEN)
		adev->gmc.sdpif_register = RREG32(mmDCHUBBUB_SDPIF_MMIO_CNTRL_0);
}

/**
=======
>>>>>>> upstream/android-13
 * gmc_v9_0_gart_disable - gart disable
 *
 * @adev: amdgpu_device pointer
 *
 * This disables all VM page table.
 */
static void gmc_v9_0_gart_disable(struct amdgpu_device *adev)
{
<<<<<<< HEAD
	gfxhub_v1_0_gart_disable(adev);
	mmhub_v1_0_gart_disable(adev);
=======
	adev->gfxhub.funcs->gart_disable(adev);
	adev->mmhub.funcs->gart_disable(adev);
>>>>>>> upstream/android-13
	amdgpu_gart_table_vram_unpin(adev);
}

static int gmc_v9_0_hw_fini(void *handle)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

<<<<<<< HEAD
=======
	gmc_v9_0_gart_disable(adev);

>>>>>>> upstream/android-13
	if (amdgpu_sriov_vf(adev)) {
		/* full access mode, so don't touch any GMC register */
		DRM_DEBUG("For SRIOV client, shouldn't do anything.\n");
		return 0;
	}

<<<<<<< HEAD
	amdgpu_irq_put(adev, &adev->gmc.vm_fault, 0);
	gmc_v9_0_gart_disable(adev);
=======
	amdgpu_irq_put(adev, &adev->gmc.ecc_irq, 0);
	amdgpu_irq_put(adev, &adev->gmc.vm_fault, 0);
>>>>>>> upstream/android-13

	return 0;
}

static int gmc_v9_0_suspend(void *handle)
{
<<<<<<< HEAD
	int r;
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

	r = gmc_v9_0_hw_fini(adev);
	if (r)
		return r;

	gmc_v9_0_save_registers(adev);

	return 0;
=======
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

	return gmc_v9_0_hw_fini(adev);
>>>>>>> upstream/android-13
}

static int gmc_v9_0_resume(void *handle)
{
	int r;
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

<<<<<<< HEAD
	gmc_v9_0_restore_registers(adev);
=======
>>>>>>> upstream/android-13
	r = gmc_v9_0_hw_init(adev);
	if (r)
		return r;

	amdgpu_vmid_reset_all(adev);

	return 0;
}

static bool gmc_v9_0_is_idle(void *handle)
{
	/* MC is always ready in GMC v9.*/
	return true;
}

static int gmc_v9_0_wait_for_idle(void *handle)
{
	/* There is no need to wait for MC idle in GMC v9.*/
	return 0;
}

static int gmc_v9_0_soft_reset(void *handle)
{
	/* XXX for emulation.*/
	return 0;
}

static int gmc_v9_0_set_clockgating_state(void *handle,
					enum amd_clockgating_state state)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

<<<<<<< HEAD
	return mmhub_v1_0_set_clockgating(adev, state);
=======
	adev->mmhub.funcs->set_clockgating(adev, state);

	athub_v1_0_set_clockgating(adev, state);

	return 0;
>>>>>>> upstream/android-13
}

static void gmc_v9_0_get_clockgating_state(void *handle, u32 *flags)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

<<<<<<< HEAD
	mmhub_v1_0_get_clockgating(adev, flags);
=======
	adev->mmhub.funcs->get_clockgating(adev, flags);

	athub_v1_0_get_clockgating(adev, flags);
>>>>>>> upstream/android-13
}

static int gmc_v9_0_set_powergating_state(void *handle,
					enum amd_powergating_state state)
{
	return 0;
}

const struct amd_ip_funcs gmc_v9_0_ip_funcs = {
	.name = "gmc_v9_0",
	.early_init = gmc_v9_0_early_init,
	.late_init = gmc_v9_0_late_init,
	.sw_init = gmc_v9_0_sw_init,
	.sw_fini = gmc_v9_0_sw_fini,
	.hw_init = gmc_v9_0_hw_init,
	.hw_fini = gmc_v9_0_hw_fini,
	.suspend = gmc_v9_0_suspend,
	.resume = gmc_v9_0_resume,
	.is_idle = gmc_v9_0_is_idle,
	.wait_for_idle = gmc_v9_0_wait_for_idle,
	.soft_reset = gmc_v9_0_soft_reset,
	.set_clockgating_state = gmc_v9_0_set_clockgating_state,
	.set_powergating_state = gmc_v9_0_set_powergating_state,
	.get_clockgating_state = gmc_v9_0_get_clockgating_state,
};

const struct amdgpu_ip_block_version gmc_v9_0_ip_block =
{
	.type = AMD_IP_BLOCK_TYPE_GMC,
	.major = 9,
	.minor = 0,
	.rev = 0,
	.funcs = &gmc_v9_0_ip_funcs,
};
