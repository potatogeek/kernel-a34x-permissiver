/*
 * Copyright 2018 Advanced Micro Devices, Inc.
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

#ifndef __SOC15_IH_CLIENTID_H__
#define __SOC15_IH_CLIENTID_H__

<<<<<<< HEAD
 /*
  * vega10+ IH clients
=======
/*
 * Vega10+ IH clients
 * Whenever this structure is updated, which should not happen, make sure
 * soc15_ih_clientid_name in the below is also updated accordingly.
>>>>>>> upstream/android-13
 */
enum soc15_ih_clientid {
	SOC15_IH_CLIENTID_IH		= 0x00,
	SOC15_IH_CLIENTID_ACP		= 0x01,
	SOC15_IH_CLIENTID_ATHUB		= 0x02,
	SOC15_IH_CLIENTID_BIF		= 0x03,
	SOC15_IH_CLIENTID_DCE		= 0x04,
	SOC15_IH_CLIENTID_ISP		= 0x05,
	SOC15_IH_CLIENTID_PCIE0		= 0x06,
	SOC15_IH_CLIENTID_RLC		= 0x07,
	SOC15_IH_CLIENTID_SDMA0		= 0x08,
	SOC15_IH_CLIENTID_SDMA1		= 0x09,
	SOC15_IH_CLIENTID_SE0SH		= 0x0a,
	SOC15_IH_CLIENTID_SE1SH		= 0x0b,
	SOC15_IH_CLIENTID_SE2SH		= 0x0c,
	SOC15_IH_CLIENTID_SE3SH		= 0x0d,
<<<<<<< HEAD
	SOC15_IH_CLIENTID_SYSHUB	= 0x0e,
=======
>>>>>>> upstream/android-13
	SOC15_IH_CLIENTID_UVD1		= 0x0e,
	SOC15_IH_CLIENTID_THM		= 0x0f,
	SOC15_IH_CLIENTID_UVD		= 0x10,
	SOC15_IH_CLIENTID_VCE0		= 0x11,
	SOC15_IH_CLIENTID_VMC		= 0x12,
	SOC15_IH_CLIENTID_XDMA		= 0x13,
	SOC15_IH_CLIENTID_GRBM_CP	= 0x14,
	SOC15_IH_CLIENTID_ATS		= 0x15,
	SOC15_IH_CLIENTID_ROM_SMUIO	= 0x16,
	SOC15_IH_CLIENTID_DF		= 0x17,
	SOC15_IH_CLIENTID_VCE1		= 0x18,
	SOC15_IH_CLIENTID_PWR		= 0x19,
<<<<<<< HEAD
=======
	SOC15_IH_CLIENTID_RESERVED	= 0x1a,
>>>>>>> upstream/android-13
	SOC15_IH_CLIENTID_UTCL2		= 0x1b,
	SOC15_IH_CLIENTID_EA		= 0x1c,
	SOC15_IH_CLIENTID_UTCL2LOG	= 0x1d,
	SOC15_IH_CLIENTID_MP0		= 0x1e,
	SOC15_IH_CLIENTID_MP1		= 0x1f,

	SOC15_IH_CLIENTID_MAX,

<<<<<<< HEAD
	SOC15_IH_CLIENTID_VCN		= SOC15_IH_CLIENTID_UVD
};

=======
	SOC15_IH_CLIENTID_VCN		= SOC15_IH_CLIENTID_UVD,
	SOC15_IH_CLIENTID_VCN1		= SOC15_IH_CLIENTID_UVD1,
	SOC15_IH_CLIENTID_SDMA2		= SOC15_IH_CLIENTID_ACP,
	SOC15_IH_CLIENTID_SDMA3		= SOC15_IH_CLIENTID_DCE,
	SOC15_IH_CLIENTID_SDMA3_Sienna_Cichlid    = SOC15_IH_CLIENTID_ISP,
	SOC15_IH_CLIENTID_SDMA4		= SOC15_IH_CLIENTID_ISP,
	SOC15_IH_CLIENTID_SDMA5		= SOC15_IH_CLIENTID_VCE0,
	SOC15_IH_CLIENTID_SDMA6		= SOC15_IH_CLIENTID_XDMA,
	SOC15_IH_CLIENTID_SDMA7		= SOC15_IH_CLIENTID_VCE1,
	SOC15_IH_CLIENTID_VMC1		= SOC15_IH_CLIENTID_PCIE0,
};

extern const char *soc15_ih_clientid_name[];

>>>>>>> upstream/android-13
#endif


