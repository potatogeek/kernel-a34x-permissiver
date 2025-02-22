/*
 * Copyright (C) 2018  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef _vega20_ip_offset_HEADER
#define _vega20_ip_offset_HEADER

#define MAX_INSTANCE                                       6
#define MAX_SEGMENT                                        6


struct IP_BASE_INSTANCE
{
    unsigned int segment[MAX_SEGMENT];
};

struct IP_BASE
{
    struct IP_BASE_INSTANCE instance[MAX_INSTANCE];
<<<<<<< HEAD
};
=======
} __maybe_unused;
>>>>>>> upstream/android-13


static const struct IP_BASE ATHUB_BASE            ={ { { { 0x00000C20, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE CLK_BASE            ={ { { { 0x00016C00, 0x00016E00, 0x00017000, 0x00017200, 0x0001B000, 0x0001B200 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE DCE_BASE            ={ { { { 0x00000012, 0x000000C0, 0x000034C0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE DF_BASE            ={ { { { 0x00007000, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE FUSE_BASE            ={ { { { 0x00017400, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE GC_BASE            ={ { { { 0x00002000, 0x0000A000, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE HDP_BASE            ={ { { { 0x00000F20, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE MMHUB_BASE            ={ { { { 0x0001A000, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE MP0_BASE            ={ { { { 0x00016000, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE MP1_BASE            ={ { { { 0x00016000, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE NBIO_BASE            ={ { { { 0x00000000, 0x00000014, 0x00000D20, 0x00010400, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE OSSSYS_BASE            ={ { { { 0x000010A0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE SDMA0_BASE            ={ { { { 0x00001260, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE SDMA1_BASE            ={ { { { 0x00001860, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE SMUIO_BASE            ={ { { { 0x00016800, 0x00016A00, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE THM_BASE            ={ { { { 0x00016600, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE UMC_BASE            ={ { { { 0x00014000, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE UVD_BASE            ={ { { { 0x00007800, 0x00007E00, 0, 0, 0, 0 } },
                                        { { 0, 0x00009000, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
/* Adjust VCE_BASE to make vce_4_1 use vce_4_0 offset header files*/
static const struct IP_BASE VCE_BASE            ={ { { { 0x00007E00/* 0x00008800 */, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE XDMA_BASE            ={ { { { 0x00003400, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };
static const struct IP_BASE RSMU_BASE            ={ { { { 0x00012000, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } },
                                        { { 0, 0, 0, 0, 0, 0 } } } };


#define ATHUB_BASE__INST0_SEG0                     0x00000C20
#define ATHUB_BASE__INST0_SEG1                     0
#define ATHUB_BASE__INST0_SEG2                     0
#define ATHUB_BASE__INST0_SEG3                     0
#define ATHUB_BASE__INST0_SEG4                     0
#define ATHUB_BASE__INST0_SEG5                     0

#define ATHUB_BASE__INST1_SEG0                     0
#define ATHUB_BASE__INST1_SEG1                     0
#define ATHUB_BASE__INST1_SEG2                     0
#define ATHUB_BASE__INST1_SEG3                     0
#define ATHUB_BASE__INST1_SEG4                     0
#define ATHUB_BASE__INST1_SEG5                     0

#define ATHUB_BASE__INST2_SEG0                     0
#define ATHUB_BASE__INST2_SEG1                     0
#define ATHUB_BASE__INST2_SEG2                     0
#define ATHUB_BASE__INST2_SEG3                     0
#define ATHUB_BASE__INST2_SEG4                     0
#define ATHUB_BASE__INST2_SEG5                     0

#define ATHUB_BASE__INST3_SEG0                     0
#define ATHUB_BASE__INST3_SEG1                     0
#define ATHUB_BASE__INST3_SEG2                     0
#define ATHUB_BASE__INST3_SEG3                     0
#define ATHUB_BASE__INST3_SEG4                     0
#define ATHUB_BASE__INST3_SEG5                     0

#define ATHUB_BASE__INST4_SEG0                     0
#define ATHUB_BASE__INST4_SEG1                     0
#define ATHUB_BASE__INST4_SEG2                     0
#define ATHUB_BASE__INST4_SEG3                     0
#define ATHUB_BASE__INST4_SEG4                     0
#define ATHUB_BASE__INST4_SEG5                     0

#define ATHUB_BASE__INST5_SEG0                     0
#define ATHUB_BASE__INST5_SEG1                     0
#define ATHUB_BASE__INST5_SEG2                     0
#define ATHUB_BASE__INST5_SEG3                     0
#define ATHUB_BASE__INST5_SEG4                     0
#define ATHUB_BASE__INST5_SEG5                     0

#define CLK_BASE__INST0_SEG0                       0x00016C00
#define CLK_BASE__INST0_SEG1                       0x00016E00
#define CLK_BASE__INST0_SEG2                       0x00017000
#define CLK_BASE__INST0_SEG3                       0x00017200
#define CLK_BASE__INST0_SEG4                       0x0001B000
#define CLK_BASE__INST0_SEG5                       0x0001B200

#define CLK_BASE__INST1_SEG0                       0
#define CLK_BASE__INST1_SEG1                       0
#define CLK_BASE__INST1_SEG2                       0
#define CLK_BASE__INST1_SEG3                       0
#define CLK_BASE__INST1_SEG4                       0
#define CLK_BASE__INST1_SEG5                       0

#define CLK_BASE__INST2_SEG0                       0
#define CLK_BASE__INST2_SEG1                       0
#define CLK_BASE__INST2_SEG2                       0
#define CLK_BASE__INST2_SEG3                       0
#define CLK_BASE__INST2_SEG4                       0
#define CLK_BASE__INST2_SEG5                       0

#define CLK_BASE__INST3_SEG0                       0
#define CLK_BASE__INST3_SEG1                       0
#define CLK_BASE__INST3_SEG2                       0
#define CLK_BASE__INST3_SEG3                       0
#define CLK_BASE__INST3_SEG4                       0
#define CLK_BASE__INST3_SEG5                       0

#define CLK_BASE__INST4_SEG0                       0
#define CLK_BASE__INST4_SEG1                       0
#define CLK_BASE__INST4_SEG2                       0
#define CLK_BASE__INST4_SEG3                       0
#define CLK_BASE__INST4_SEG4                       0
#define CLK_BASE__INST4_SEG5                       0

#define CLK_BASE__INST5_SEG0                       0
#define CLK_BASE__INST5_SEG1                       0
#define CLK_BASE__INST5_SEG2                       0
#define CLK_BASE__INST5_SEG3                       0
#define CLK_BASE__INST5_SEG4                       0
#define CLK_BASE__INST5_SEG5                       0

#define DCE_BASE__INST0_SEG0                       0x00000012
#define DCE_BASE__INST0_SEG1                       0x000000C0
#define DCE_BASE__INST0_SEG2                       0x000034C0
#define DCE_BASE__INST0_SEG3                       0
#define DCE_BASE__INST0_SEG4                       0
#define DCE_BASE__INST0_SEG5                       0

#define DCE_BASE__INST1_SEG0                       0
#define DCE_BASE__INST1_SEG1                       0
#define DCE_BASE__INST1_SEG2                       0
#define DCE_BASE__INST1_SEG3                       0
#define DCE_BASE__INST1_SEG4                       0
#define DCE_BASE__INST1_SEG5                       0

#define DCE_BASE__INST2_SEG0                       0
#define DCE_BASE__INST2_SEG1                       0
#define DCE_BASE__INST2_SEG2                       0
#define DCE_BASE__INST2_SEG3                       0
#define DCE_BASE__INST2_SEG4                       0
#define DCE_BASE__INST2_SEG5                       0

#define DCE_BASE__INST3_SEG0                       0
#define DCE_BASE__INST3_SEG1                       0
#define DCE_BASE__INST3_SEG2                       0
#define DCE_BASE__INST3_SEG3                       0
#define DCE_BASE__INST3_SEG4                       0
#define DCE_BASE__INST3_SEG5                       0

#define DCE_BASE__INST4_SEG0                       0
#define DCE_BASE__INST4_SEG1                       0
#define DCE_BASE__INST4_SEG2                       0
#define DCE_BASE__INST4_SEG3                       0
#define DCE_BASE__INST4_SEG4                       0
#define DCE_BASE__INST4_SEG5                       0

#define DCE_BASE__INST5_SEG0                       0
#define DCE_BASE__INST5_SEG1                       0
#define DCE_BASE__INST5_SEG2                       0
#define DCE_BASE__INST5_SEG3                       0
#define DCE_BASE__INST5_SEG4                       0
#define DCE_BASE__INST5_SEG5                       0

#define DF_BASE__INST0_SEG0                        0x00007000
#define DF_BASE__INST0_SEG1                        0
#define DF_BASE__INST0_SEG2                        0
#define DF_BASE__INST0_SEG3                        0
#define DF_BASE__INST0_SEG4                        0
#define DF_BASE__INST0_SEG5                        0

#define DF_BASE__INST1_SEG0                        0
#define DF_BASE__INST1_SEG1                        0
#define DF_BASE__INST1_SEG2                        0
#define DF_BASE__INST1_SEG3                        0
#define DF_BASE__INST1_SEG4                        0
#define DF_BASE__INST1_SEG5                        0

#define DF_BASE__INST2_SEG0                        0
#define DF_BASE__INST2_SEG1                        0
#define DF_BASE__INST2_SEG2                        0
#define DF_BASE__INST2_SEG3                        0
#define DF_BASE__INST2_SEG4                        0
#define DF_BASE__INST2_SEG5                        0

#define DF_BASE__INST3_SEG0                        0
#define DF_BASE__INST3_SEG1                        0
#define DF_BASE__INST3_SEG2                        0
#define DF_BASE__INST3_SEG3                        0
#define DF_BASE__INST3_SEG4                        0
#define DF_BASE__INST3_SEG5                        0

#define DF_BASE__INST4_SEG0                        0
#define DF_BASE__INST4_SEG1                        0
#define DF_BASE__INST4_SEG2                        0
#define DF_BASE__INST4_SEG3                        0
#define DF_BASE__INST4_SEG4                        0
#define DF_BASE__INST4_SEG5                        0

#define DF_BASE__INST5_SEG0                        0
#define DF_BASE__INST5_SEG1                        0
#define DF_BASE__INST5_SEG2                        0
#define DF_BASE__INST5_SEG3                        0
#define DF_BASE__INST5_SEG4                        0
#define DF_BASE__INST5_SEG5                        0

#define FUSE_BASE__INST0_SEG0                      0x00017400
#define FUSE_BASE__INST0_SEG1                      0
#define FUSE_BASE__INST0_SEG2                      0
#define FUSE_BASE__INST0_SEG3                      0
#define FUSE_BASE__INST0_SEG4                      0
#define FUSE_BASE__INST0_SEG5                      0

#define FUSE_BASE__INST1_SEG0                      0
#define FUSE_BASE__INST1_SEG1                      0
#define FUSE_BASE__INST1_SEG2                      0
#define FUSE_BASE__INST1_SEG3                      0
#define FUSE_BASE__INST1_SEG4                      0
#define FUSE_BASE__INST1_SEG5                      0

#define FUSE_BASE__INST2_SEG0                      0
#define FUSE_BASE__INST2_SEG1                      0
#define FUSE_BASE__INST2_SEG2                      0
#define FUSE_BASE__INST2_SEG3                      0
#define FUSE_BASE__INST2_SEG4                      0
#define FUSE_BASE__INST2_SEG5                      0

#define FUSE_BASE__INST3_SEG0                      0
#define FUSE_BASE__INST3_SEG1                      0
#define FUSE_BASE__INST3_SEG2                      0
#define FUSE_BASE__INST3_SEG3                      0
#define FUSE_BASE__INST3_SEG4                      0
#define FUSE_BASE__INST3_SEG5                      0

#define FUSE_BASE__INST4_SEG0                      0
#define FUSE_BASE__INST4_SEG1                      0
#define FUSE_BASE__INST4_SEG2                      0
#define FUSE_BASE__INST4_SEG3                      0
#define FUSE_BASE__INST4_SEG4                      0
#define FUSE_BASE__INST4_SEG5                      0

#define FUSE_BASE__INST5_SEG0                      0
#define FUSE_BASE__INST5_SEG1                      0
#define FUSE_BASE__INST5_SEG2                      0
#define FUSE_BASE__INST5_SEG3                      0
#define FUSE_BASE__INST5_SEG4                      0
#define FUSE_BASE__INST5_SEG5                      0

#define GC_BASE__INST0_SEG0                        0x00002000
#define GC_BASE__INST0_SEG1                        0x0000A000
#define GC_BASE__INST0_SEG2                        0
#define GC_BASE__INST0_SEG3                        0
#define GC_BASE__INST0_SEG4                        0
#define GC_BASE__INST0_SEG5                        0

#define GC_BASE__INST1_SEG0                        0
#define GC_BASE__INST1_SEG1                        0
#define GC_BASE__INST1_SEG2                        0
#define GC_BASE__INST1_SEG3                        0
#define GC_BASE__INST1_SEG4                        0
#define GC_BASE__INST1_SEG5                        0

#define GC_BASE__INST2_SEG0                        0
#define GC_BASE__INST2_SEG1                        0
#define GC_BASE__INST2_SEG2                        0
#define GC_BASE__INST2_SEG3                        0
#define GC_BASE__INST2_SEG4                        0
#define GC_BASE__INST2_SEG5                        0

#define GC_BASE__INST3_SEG0                        0
#define GC_BASE__INST3_SEG1                        0
#define GC_BASE__INST3_SEG2                        0
#define GC_BASE__INST3_SEG3                        0
#define GC_BASE__INST3_SEG4                        0
#define GC_BASE__INST3_SEG5                        0

#define GC_BASE__INST4_SEG0                        0
#define GC_BASE__INST4_SEG1                        0
#define GC_BASE__INST4_SEG2                        0
#define GC_BASE__INST4_SEG3                        0
#define GC_BASE__INST4_SEG4                        0
#define GC_BASE__INST4_SEG5                        0

#define GC_BASE__INST5_SEG0                        0
#define GC_BASE__INST5_SEG1                        0
#define GC_BASE__INST5_SEG2                        0
#define GC_BASE__INST5_SEG3                        0
#define GC_BASE__INST5_SEG4                        0
#define GC_BASE__INST5_SEG5                        0

#define HDP_BASE__INST0_SEG0                       0x00000F20
#define HDP_BASE__INST0_SEG1                       0
#define HDP_BASE__INST0_SEG2                       0
#define HDP_BASE__INST0_SEG3                       0
#define HDP_BASE__INST0_SEG4                       0
#define HDP_BASE__INST0_SEG5                       0

#define HDP_BASE__INST1_SEG0                       0
#define HDP_BASE__INST1_SEG1                       0
#define HDP_BASE__INST1_SEG2                       0
#define HDP_BASE__INST1_SEG3                       0
#define HDP_BASE__INST1_SEG4                       0
#define HDP_BASE__INST1_SEG5                       0

#define HDP_BASE__INST2_SEG0                       0
#define HDP_BASE__INST2_SEG1                       0
#define HDP_BASE__INST2_SEG2                       0
#define HDP_BASE__INST2_SEG3                       0
#define HDP_BASE__INST2_SEG4                       0
#define HDP_BASE__INST2_SEG5                       0

#define HDP_BASE__INST3_SEG0                       0
#define HDP_BASE__INST3_SEG1                       0
#define HDP_BASE__INST3_SEG2                       0
#define HDP_BASE__INST3_SEG3                       0
#define HDP_BASE__INST3_SEG4                       0
#define HDP_BASE__INST3_SEG5                       0

#define HDP_BASE__INST4_SEG0                       0
#define HDP_BASE__INST4_SEG1                       0
#define HDP_BASE__INST4_SEG2                       0
#define HDP_BASE__INST4_SEG3                       0
#define HDP_BASE__INST4_SEG4                       0
#define HDP_BASE__INST4_SEG5                       0

#define HDP_BASE__INST5_SEG0                       0
#define HDP_BASE__INST5_SEG1                       0
#define HDP_BASE__INST5_SEG2                       0
#define HDP_BASE__INST5_SEG3                       0
#define HDP_BASE__INST5_SEG4                       0
#define HDP_BASE__INST5_SEG5                       0

#define MMHUB_BASE__INST0_SEG0                     0x0001A000
#define MMHUB_BASE__INST0_SEG1                     0
#define MMHUB_BASE__INST0_SEG2                     0
#define MMHUB_BASE__INST0_SEG3                     0
#define MMHUB_BASE__INST0_SEG4                     0
#define MMHUB_BASE__INST0_SEG5                     0

#define MMHUB_BASE__INST1_SEG0                     0
#define MMHUB_BASE__INST1_SEG1                     0
#define MMHUB_BASE__INST1_SEG2                     0
#define MMHUB_BASE__INST1_SEG3                     0
#define MMHUB_BASE__INST1_SEG4                     0
#define MMHUB_BASE__INST1_SEG5                     0

#define MMHUB_BASE__INST2_SEG0                     0
#define MMHUB_BASE__INST2_SEG1                     0
#define MMHUB_BASE__INST2_SEG2                     0
#define MMHUB_BASE__INST2_SEG3                     0
#define MMHUB_BASE__INST2_SEG4                     0
#define MMHUB_BASE__INST2_SEG5                     0

#define MMHUB_BASE__INST3_SEG0                     0
#define MMHUB_BASE__INST3_SEG1                     0
#define MMHUB_BASE__INST3_SEG2                     0
#define MMHUB_BASE__INST3_SEG3                     0
#define MMHUB_BASE__INST3_SEG4                     0
#define MMHUB_BASE__INST3_SEG5                     0

#define MMHUB_BASE__INST4_SEG0                     0
#define MMHUB_BASE__INST4_SEG1                     0
#define MMHUB_BASE__INST4_SEG2                     0
#define MMHUB_BASE__INST4_SEG3                     0
#define MMHUB_BASE__INST4_SEG4                     0
#define MMHUB_BASE__INST4_SEG5                     0

#define MMHUB_BASE__INST5_SEG0                     0
#define MMHUB_BASE__INST5_SEG1                     0
#define MMHUB_BASE__INST5_SEG2                     0
#define MMHUB_BASE__INST5_SEG3                     0
#define MMHUB_BASE__INST5_SEG4                     0
#define MMHUB_BASE__INST5_SEG5                     0

#define MP0_BASE__INST0_SEG0                       0x00016000
#define MP0_BASE__INST0_SEG1                       0
#define MP0_BASE__INST0_SEG2                       0
#define MP0_BASE__INST0_SEG3                       0
#define MP0_BASE__INST0_SEG4                       0
#define MP0_BASE__INST0_SEG5                       0

#define MP0_BASE__INST1_SEG0                       0
#define MP0_BASE__INST1_SEG1                       0
#define MP0_BASE__INST1_SEG2                       0
#define MP0_BASE__INST1_SEG3                       0
#define MP0_BASE__INST1_SEG4                       0
#define MP0_BASE__INST1_SEG5                       0

#define MP0_BASE__INST2_SEG0                       0
#define MP0_BASE__INST2_SEG1                       0
#define MP0_BASE__INST2_SEG2                       0
#define MP0_BASE__INST2_SEG3                       0
#define MP0_BASE__INST2_SEG4                       0
#define MP0_BASE__INST2_SEG5                       0

#define MP0_BASE__INST3_SEG0                       0
#define MP0_BASE__INST3_SEG1                       0
#define MP0_BASE__INST3_SEG2                       0
#define MP0_BASE__INST3_SEG3                       0
#define MP0_BASE__INST3_SEG4                       0
#define MP0_BASE__INST3_SEG5                       0

#define MP0_BASE__INST4_SEG0                       0
#define MP0_BASE__INST4_SEG1                       0
#define MP0_BASE__INST4_SEG2                       0
#define MP0_BASE__INST4_SEG3                       0
#define MP0_BASE__INST4_SEG4                       0
#define MP0_BASE__INST4_SEG5                       0

#define MP0_BASE__INST5_SEG0                       0
#define MP0_BASE__INST5_SEG1                       0
#define MP0_BASE__INST5_SEG2                       0
#define MP0_BASE__INST5_SEG3                       0
#define MP0_BASE__INST5_SEG4                       0
#define MP0_BASE__INST5_SEG5                       0

#define MP1_BASE__INST0_SEG0                       0x00016000
#define MP1_BASE__INST0_SEG1                       0
#define MP1_BASE__INST0_SEG2                       0
#define MP1_BASE__INST0_SEG3                       0
#define MP1_BASE__INST0_SEG4                       0
#define MP1_BASE__INST0_SEG5                       0

#define MP1_BASE__INST1_SEG0                       0
#define MP1_BASE__INST1_SEG1                       0
#define MP1_BASE__INST1_SEG2                       0
#define MP1_BASE__INST1_SEG3                       0
#define MP1_BASE__INST1_SEG4                       0
#define MP1_BASE__INST1_SEG5                       0

#define MP1_BASE__INST2_SEG0                       0
#define MP1_BASE__INST2_SEG1                       0
#define MP1_BASE__INST2_SEG2                       0
#define MP1_BASE__INST2_SEG3                       0
#define MP1_BASE__INST2_SEG4                       0
#define MP1_BASE__INST2_SEG5                       0

#define MP1_BASE__INST3_SEG0                       0
#define MP1_BASE__INST3_SEG1                       0
#define MP1_BASE__INST3_SEG2                       0
#define MP1_BASE__INST3_SEG3                       0
#define MP1_BASE__INST3_SEG4                       0
#define MP1_BASE__INST3_SEG5                       0

#define MP1_BASE__INST4_SEG0                       0
#define MP1_BASE__INST4_SEG1                       0
#define MP1_BASE__INST4_SEG2                       0
#define MP1_BASE__INST4_SEG3                       0
#define MP1_BASE__INST4_SEG4                       0
#define MP1_BASE__INST4_SEG5                       0

#define MP1_BASE__INST5_SEG0                       0
#define MP1_BASE__INST5_SEG1                       0
#define MP1_BASE__INST5_SEG2                       0
#define MP1_BASE__INST5_SEG3                       0
#define MP1_BASE__INST5_SEG4                       0
#define MP1_BASE__INST5_SEG5                       0

#define NBIO_BASE__INST0_SEG0                      0x00000000
#define NBIO_BASE__INST0_SEG1                      0x00000014
#define NBIO_BASE__INST0_SEG2                      0x00000D20
#define NBIO_BASE__INST0_SEG3                      0x00010400
#define NBIO_BASE__INST0_SEG4                      0
#define NBIO_BASE__INST0_SEG5                      0

#define NBIO_BASE__INST1_SEG0                      0
#define NBIO_BASE__INST1_SEG1                      0
#define NBIO_BASE__INST1_SEG2                      0
#define NBIO_BASE__INST1_SEG3                      0
#define NBIO_BASE__INST1_SEG4                      0
#define NBIO_BASE__INST1_SEG5                      0

#define NBIO_BASE__INST2_SEG0                      0
#define NBIO_BASE__INST2_SEG1                      0
#define NBIO_BASE__INST2_SEG2                      0
#define NBIO_BASE__INST2_SEG3                      0
#define NBIO_BASE__INST2_SEG4                      0
#define NBIO_BASE__INST2_SEG5                      0

#define NBIO_BASE__INST3_SEG0                      0
#define NBIO_BASE__INST3_SEG1                      0
#define NBIO_BASE__INST3_SEG2                      0
#define NBIO_BASE__INST3_SEG3                      0
#define NBIO_BASE__INST3_SEG4                      0
#define NBIO_BASE__INST3_SEG5                      0

#define NBIO_BASE__INST4_SEG0                      0
#define NBIO_BASE__INST4_SEG1                      0
#define NBIO_BASE__INST4_SEG2                      0
#define NBIO_BASE__INST4_SEG3                      0
#define NBIO_BASE__INST4_SEG4                      0
#define NBIO_BASE__INST4_SEG5                      0

#define NBIO_BASE__INST5_SEG0                      0
#define NBIO_BASE__INST5_SEG1                      0
#define NBIO_BASE__INST5_SEG2                      0
#define NBIO_BASE__INST5_SEG3                      0
#define NBIO_BASE__INST5_SEG4                      0
#define NBIO_BASE__INST5_SEG5                      0

#define OSSSYS_BASE__INST0_SEG0                    0x000010A0
#define OSSSYS_BASE__INST0_SEG1                    0
#define OSSSYS_BASE__INST0_SEG2                    0
#define OSSSYS_BASE__INST0_SEG3                    0
#define OSSSYS_BASE__INST0_SEG4                    0
#define OSSSYS_BASE__INST0_SEG5                    0

#define OSSSYS_BASE__INST1_SEG0                    0
#define OSSSYS_BASE__INST1_SEG1                    0
#define OSSSYS_BASE__INST1_SEG2                    0
#define OSSSYS_BASE__INST1_SEG3                    0
#define OSSSYS_BASE__INST1_SEG4                    0
#define OSSSYS_BASE__INST1_SEG5                    0

#define OSSSYS_BASE__INST2_SEG0                    0
#define OSSSYS_BASE__INST2_SEG1                    0
#define OSSSYS_BASE__INST2_SEG2                    0
#define OSSSYS_BASE__INST2_SEG3                    0
#define OSSSYS_BASE__INST2_SEG4                    0
#define OSSSYS_BASE__INST2_SEG5                    0

#define OSSSYS_BASE__INST3_SEG0                    0
#define OSSSYS_BASE__INST3_SEG1                    0
#define OSSSYS_BASE__INST3_SEG2                    0
#define OSSSYS_BASE__INST3_SEG3                    0
#define OSSSYS_BASE__INST3_SEG4                    0
#define OSSSYS_BASE__INST3_SEG5                    0

#define OSSSYS_BASE__INST4_SEG0                    0
#define OSSSYS_BASE__INST4_SEG1                    0
#define OSSSYS_BASE__INST4_SEG2                    0
#define OSSSYS_BASE__INST4_SEG3                    0
#define OSSSYS_BASE__INST4_SEG4                    0
#define OSSSYS_BASE__INST4_SEG5                    0

#define OSSSYS_BASE__INST5_SEG0                    0
#define OSSSYS_BASE__INST5_SEG1                    0
#define OSSSYS_BASE__INST5_SEG2                    0
#define OSSSYS_BASE__INST5_SEG3                    0
#define OSSSYS_BASE__INST5_SEG4                    0
#define OSSSYS_BASE__INST5_SEG5                    0

#define SDMA0_BASE__INST0_SEG0                     0x00001260
#define SDMA0_BASE__INST0_SEG1                     0
#define SDMA0_BASE__INST0_SEG2                     0
#define SDMA0_BASE__INST0_SEG3                     0
#define SDMA0_BASE__INST0_SEG4                     0
#define SDMA0_BASE__INST0_SEG5                     0

#define SDMA0_BASE__INST1_SEG0                     0
#define SDMA0_BASE__INST1_SEG1                     0
#define SDMA0_BASE__INST1_SEG2                     0
#define SDMA0_BASE__INST1_SEG3                     0
#define SDMA0_BASE__INST1_SEG4                     0
#define SDMA0_BASE__INST1_SEG5                     0

#define SDMA0_BASE__INST2_SEG0                     0
#define SDMA0_BASE__INST2_SEG1                     0
#define SDMA0_BASE__INST2_SEG2                     0
#define SDMA0_BASE__INST2_SEG3                     0
#define SDMA0_BASE__INST2_SEG4                     0
#define SDMA0_BASE__INST2_SEG5                     0

#define SDMA0_BASE__INST3_SEG0                     0
#define SDMA0_BASE__INST3_SEG1                     0
#define SDMA0_BASE__INST3_SEG2                     0
#define SDMA0_BASE__INST3_SEG3                     0
#define SDMA0_BASE__INST3_SEG4                     0
#define SDMA0_BASE__INST3_SEG5                     0

#define SDMA0_BASE__INST4_SEG0                     0
#define SDMA0_BASE__INST4_SEG1                     0
#define SDMA0_BASE__INST4_SEG2                     0
#define SDMA0_BASE__INST4_SEG3                     0
#define SDMA0_BASE__INST4_SEG4                     0
#define SDMA0_BASE__INST4_SEG5                     0

#define SDMA0_BASE__INST5_SEG0                     0
#define SDMA0_BASE__INST5_SEG1                     0
#define SDMA0_BASE__INST5_SEG2                     0
#define SDMA0_BASE__INST5_SEG3                     0
#define SDMA0_BASE__INST5_SEG4                     0
#define SDMA0_BASE__INST5_SEG5                     0

#define SDMA1_BASE__INST0_SEG0                     0x00001860
#define SDMA1_BASE__INST0_SEG1                     0
#define SDMA1_BASE__INST0_SEG2                     0
#define SDMA1_BASE__INST0_SEG3                     0
#define SDMA1_BASE__INST0_SEG4                     0
#define SDMA1_BASE__INST0_SEG5                     0

#define SDMA1_BASE__INST1_SEG0                     0
#define SDMA1_BASE__INST1_SEG1                     0
#define SDMA1_BASE__INST1_SEG2                     0
#define SDMA1_BASE__INST1_SEG3                     0
#define SDMA1_BASE__INST1_SEG4                     0
#define SDMA1_BASE__INST1_SEG5                     0

#define SDMA1_BASE__INST2_SEG0                     0
#define SDMA1_BASE__INST2_SEG1                     0
#define SDMA1_BASE__INST2_SEG2                     0
#define SDMA1_BASE__INST2_SEG3                     0
#define SDMA1_BASE__INST2_SEG4                     0
#define SDMA1_BASE__INST2_SEG5                     0

#define SDMA1_BASE__INST3_SEG0                     0
#define SDMA1_BASE__INST3_SEG1                     0
#define SDMA1_BASE__INST3_SEG2                     0
#define SDMA1_BASE__INST3_SEG3                     0
#define SDMA1_BASE__INST3_SEG4                     0
#define SDMA1_BASE__INST3_SEG5                     0

#define SDMA1_BASE__INST4_SEG0                     0
#define SDMA1_BASE__INST4_SEG1                     0
#define SDMA1_BASE__INST4_SEG2                     0
#define SDMA1_BASE__INST4_SEG3                     0
#define SDMA1_BASE__INST4_SEG4                     0
#define SDMA1_BASE__INST4_SEG5                     0

#define SDMA1_BASE__INST5_SEG0                     0
#define SDMA1_BASE__INST5_SEG1                     0
#define SDMA1_BASE__INST5_SEG2                     0
#define SDMA1_BASE__INST5_SEG3                     0
#define SDMA1_BASE__INST5_SEG4                     0
#define SDMA1_BASE__INST5_SEG5                     0

#define SMUIO_BASE__INST0_SEG0                     0x00016800
#define SMUIO_BASE__INST0_SEG1                     0x00016A00
#define SMUIO_BASE__INST0_SEG2                     0
#define SMUIO_BASE__INST0_SEG3                     0
#define SMUIO_BASE__INST0_SEG4                     0
#define SMUIO_BASE__INST0_SEG5                     0

#define SMUIO_BASE__INST1_SEG0                     0
#define SMUIO_BASE__INST1_SEG1                     0
#define SMUIO_BASE__INST1_SEG2                     0
#define SMUIO_BASE__INST1_SEG3                     0
#define SMUIO_BASE__INST1_SEG4                     0
#define SMUIO_BASE__INST1_SEG5                     0

#define SMUIO_BASE__INST2_SEG0                     0
#define SMUIO_BASE__INST2_SEG1                     0
#define SMUIO_BASE__INST2_SEG2                     0
#define SMUIO_BASE__INST2_SEG3                     0
#define SMUIO_BASE__INST2_SEG4                     0
#define SMUIO_BASE__INST2_SEG5                     0

#define SMUIO_BASE__INST3_SEG0                     0
#define SMUIO_BASE__INST3_SEG1                     0
#define SMUIO_BASE__INST3_SEG2                     0
#define SMUIO_BASE__INST3_SEG3                     0
#define SMUIO_BASE__INST3_SEG4                     0
#define SMUIO_BASE__INST3_SEG5                     0

#define SMUIO_BASE__INST4_SEG0                     0
#define SMUIO_BASE__INST4_SEG1                     0
#define SMUIO_BASE__INST4_SEG2                     0
#define SMUIO_BASE__INST4_SEG3                     0
#define SMUIO_BASE__INST4_SEG4                     0
#define SMUIO_BASE__INST4_SEG5                     0

#define SMUIO_BASE__INST5_SEG0                     0
#define SMUIO_BASE__INST5_SEG1                     0
#define SMUIO_BASE__INST5_SEG2                     0
#define SMUIO_BASE__INST5_SEG3                     0
#define SMUIO_BASE__INST5_SEG4                     0
#define SMUIO_BASE__INST5_SEG5                     0

#define THM_BASE__INST0_SEG0                       0x00016600
#define THM_BASE__INST0_SEG1                       0
#define THM_BASE__INST0_SEG2                       0
#define THM_BASE__INST0_SEG3                       0
#define THM_BASE__INST0_SEG4                       0
#define THM_BASE__INST0_SEG5                       0

#define THM_BASE__INST1_SEG0                       0
#define THM_BASE__INST1_SEG1                       0
#define THM_BASE__INST1_SEG2                       0
#define THM_BASE__INST1_SEG3                       0
#define THM_BASE__INST1_SEG4                       0
#define THM_BASE__INST1_SEG5                       0

#define THM_BASE__INST2_SEG0                       0
#define THM_BASE__INST2_SEG1                       0
#define THM_BASE__INST2_SEG2                       0
#define THM_BASE__INST2_SEG3                       0
#define THM_BASE__INST2_SEG4                       0
#define THM_BASE__INST2_SEG5                       0

#define THM_BASE__INST3_SEG0                       0
#define THM_BASE__INST3_SEG1                       0
#define THM_BASE__INST3_SEG2                       0
#define THM_BASE__INST3_SEG3                       0
#define THM_BASE__INST3_SEG4                       0
#define THM_BASE__INST3_SEG5                       0

#define THM_BASE__INST4_SEG0                       0
#define THM_BASE__INST4_SEG1                       0
#define THM_BASE__INST4_SEG2                       0
#define THM_BASE__INST4_SEG3                       0
#define THM_BASE__INST4_SEG4                       0
#define THM_BASE__INST4_SEG5                       0

#define THM_BASE__INST5_SEG0                       0
#define THM_BASE__INST5_SEG1                       0
#define THM_BASE__INST5_SEG2                       0
#define THM_BASE__INST5_SEG3                       0
#define THM_BASE__INST5_SEG4                       0
#define THM_BASE__INST5_SEG5                       0

#define UMC_BASE__INST0_SEG0                       0x00014000
#define UMC_BASE__INST0_SEG1                       0
#define UMC_BASE__INST0_SEG2                       0
#define UMC_BASE__INST0_SEG3                       0
#define UMC_BASE__INST0_SEG4                       0
#define UMC_BASE__INST0_SEG5                       0

#define UMC_BASE__INST1_SEG0                       0
#define UMC_BASE__INST1_SEG1                       0
#define UMC_BASE__INST1_SEG2                       0
#define UMC_BASE__INST1_SEG3                       0
#define UMC_BASE__INST1_SEG4                       0
#define UMC_BASE__INST1_SEG5                       0

#define UMC_BASE__INST2_SEG0                       0
#define UMC_BASE__INST2_SEG1                       0
#define UMC_BASE__INST2_SEG2                       0
#define UMC_BASE__INST2_SEG3                       0
#define UMC_BASE__INST2_SEG4                       0
#define UMC_BASE__INST2_SEG5                       0

#define UMC_BASE__INST3_SEG0                       0
#define UMC_BASE__INST3_SEG1                       0
#define UMC_BASE__INST3_SEG2                       0
#define UMC_BASE__INST3_SEG3                       0
#define UMC_BASE__INST3_SEG4                       0
#define UMC_BASE__INST3_SEG5                       0

#define UMC_BASE__INST4_SEG0                       0
#define UMC_BASE__INST4_SEG1                       0
#define UMC_BASE__INST4_SEG2                       0
#define UMC_BASE__INST4_SEG3                       0
#define UMC_BASE__INST4_SEG4                       0
#define UMC_BASE__INST4_SEG5                       0

#define UMC_BASE__INST5_SEG0                       0
#define UMC_BASE__INST5_SEG1                       0
#define UMC_BASE__INST5_SEG2                       0
#define UMC_BASE__INST5_SEG3                       0
#define UMC_BASE__INST5_SEG4                       0
#define UMC_BASE__INST5_SEG5                       0

#define UVD_BASE__INST0_SEG0                       0x00007800
#define UVD_BASE__INST0_SEG1                       0x00007E00
#define UVD_BASE__INST0_SEG2                       0
#define UVD_BASE__INST0_SEG3                       0
#define UVD_BASE__INST0_SEG4                       0
#define UVD_BASE__INST0_SEG5                       0

#define UVD_BASE__INST1_SEG0                       0
#define UVD_BASE__INST1_SEG1                       0x00009000
#define UVD_BASE__INST1_SEG2                       0
#define UVD_BASE__INST1_SEG3                       0
#define UVD_BASE__INST1_SEG4                       0
#define UVD_BASE__INST1_SEG5                       0

#define UVD_BASE__INST2_SEG0                       0
#define UVD_BASE__INST2_SEG1                       0
#define UVD_BASE__INST2_SEG2                       0
#define UVD_BASE__INST2_SEG3                       0
#define UVD_BASE__INST2_SEG4                       0
#define UVD_BASE__INST2_SEG5                       0

#define UVD_BASE__INST3_SEG0                       0
#define UVD_BASE__INST3_SEG1                       0
#define UVD_BASE__INST3_SEG2                       0
#define UVD_BASE__INST3_SEG3                       0
#define UVD_BASE__INST3_SEG4                       0
#define UVD_BASE__INST3_SEG5                       0

#define UVD_BASE__INST4_SEG0                       0
#define UVD_BASE__INST4_SEG1                       0
#define UVD_BASE__INST4_SEG2                       0
#define UVD_BASE__INST4_SEG3                       0
#define UVD_BASE__INST4_SEG4                       0
#define UVD_BASE__INST4_SEG5                       0

#define UVD_BASE__INST5_SEG0                       0
#define UVD_BASE__INST5_SEG1                       0
#define UVD_BASE__INST5_SEG2                       0
#define UVD_BASE__INST5_SEG3                       0
#define UVD_BASE__INST5_SEG4                       0
#define UVD_BASE__INST5_SEG5                       0

#define VCE_BASE__INST0_SEG0                       0x00008800
#define VCE_BASE__INST0_SEG1                       0
#define VCE_BASE__INST0_SEG2                       0
#define VCE_BASE__INST0_SEG3                       0
#define VCE_BASE__INST0_SEG4                       0
#define VCE_BASE__INST0_SEG5                       0

#define VCE_BASE__INST1_SEG0                       0
#define VCE_BASE__INST1_SEG1                       0
#define VCE_BASE__INST1_SEG2                       0
#define VCE_BASE__INST1_SEG3                       0
#define VCE_BASE__INST1_SEG4                       0
#define VCE_BASE__INST1_SEG5                       0

#define VCE_BASE__INST2_SEG0                       0
#define VCE_BASE__INST2_SEG1                       0
#define VCE_BASE__INST2_SEG2                       0
#define VCE_BASE__INST2_SEG3                       0
#define VCE_BASE__INST2_SEG4                       0
#define VCE_BASE__INST2_SEG5                       0

#define VCE_BASE__INST3_SEG0                       0
#define VCE_BASE__INST3_SEG1                       0
#define VCE_BASE__INST3_SEG2                       0
#define VCE_BASE__INST3_SEG3                       0
#define VCE_BASE__INST3_SEG4                       0
#define VCE_BASE__INST3_SEG5                       0

#define VCE_BASE__INST4_SEG0                       0
#define VCE_BASE__INST4_SEG1                       0
#define VCE_BASE__INST4_SEG2                       0
#define VCE_BASE__INST4_SEG3                       0
#define VCE_BASE__INST4_SEG4                       0
#define VCE_BASE__INST4_SEG5                       0

#define VCE_BASE__INST5_SEG0                       0
#define VCE_BASE__INST5_SEG1                       0
#define VCE_BASE__INST5_SEG2                       0
#define VCE_BASE__INST5_SEG3                       0
#define VCE_BASE__INST5_SEG4                       0
#define VCE_BASE__INST5_SEG5                       0

#define XDMA_BASE__INST0_SEG0                      0x00003400
#define XDMA_BASE__INST0_SEG1                      0
#define XDMA_BASE__INST0_SEG2                      0
#define XDMA_BASE__INST0_SEG3                      0
#define XDMA_BASE__INST0_SEG4                      0
#define XDMA_BASE__INST0_SEG5                      0

#define XDMA_BASE__INST1_SEG0                      0
#define XDMA_BASE__INST1_SEG1                      0
#define XDMA_BASE__INST1_SEG2                      0
#define XDMA_BASE__INST1_SEG3                      0
#define XDMA_BASE__INST1_SEG4                      0
#define XDMA_BASE__INST1_SEG5                      0

#define XDMA_BASE__INST2_SEG0                      0
#define XDMA_BASE__INST2_SEG1                      0
#define XDMA_BASE__INST2_SEG2                      0
#define XDMA_BASE__INST2_SEG3                      0
#define XDMA_BASE__INST2_SEG4                      0
#define XDMA_BASE__INST2_SEG5                      0

#define XDMA_BASE__INST3_SEG0                      0
#define XDMA_BASE__INST3_SEG1                      0
#define XDMA_BASE__INST3_SEG2                      0
#define XDMA_BASE__INST3_SEG3                      0
#define XDMA_BASE__INST3_SEG4                      0
#define XDMA_BASE__INST3_SEG5                      0

#define XDMA_BASE__INST4_SEG0                      0
#define XDMA_BASE__INST4_SEG1                      0
#define XDMA_BASE__INST4_SEG2                      0
#define XDMA_BASE__INST4_SEG3                      0
#define XDMA_BASE__INST4_SEG4                      0
#define XDMA_BASE__INST4_SEG5                      0

#define XDMA_BASE__INST5_SEG0                      0
#define XDMA_BASE__INST5_SEG1                      0
#define XDMA_BASE__INST5_SEG2                      0
#define XDMA_BASE__INST5_SEG3                      0
#define XDMA_BASE__INST5_SEG4                      0
#define XDMA_BASE__INST5_SEG5                      0

#define RSMU_BASE__INST0_SEG0                      0x00012000
#define RSMU_BASE__INST0_SEG1                      0
#define RSMU_BASE__INST0_SEG2                      0
#define RSMU_BASE__INST0_SEG3                      0
#define RSMU_BASE__INST0_SEG4                      0
#define RSMU_BASE__INST0_SEG5                      0

#define RSMU_BASE__INST1_SEG0                      0
#define RSMU_BASE__INST1_SEG1                      0
#define RSMU_BASE__INST1_SEG2                      0
#define RSMU_BASE__INST1_SEG3                      0
#define RSMU_BASE__INST1_SEG4                      0
#define RSMU_BASE__INST1_SEG5                      0

#define RSMU_BASE__INST2_SEG0                      0
#define RSMU_BASE__INST2_SEG1                      0
#define RSMU_BASE__INST2_SEG2                      0
#define RSMU_BASE__INST2_SEG3                      0
#define RSMU_BASE__INST2_SEG4                      0
#define RSMU_BASE__INST2_SEG5                      0

#define RSMU_BASE__INST3_SEG0                      0
#define RSMU_BASE__INST3_SEG1                      0
#define RSMU_BASE__INST3_SEG2                      0
#define RSMU_BASE__INST3_SEG3                      0
#define RSMU_BASE__INST3_SEG4                      0
#define RSMU_BASE__INST3_SEG5                      0

#define RSMU_BASE__INST4_SEG0                      0
#define RSMU_BASE__INST4_SEG1                      0
#define RSMU_BASE__INST4_SEG2                      0
#define RSMU_BASE__INST4_SEG3                      0
#define RSMU_BASE__INST4_SEG4                      0
#define RSMU_BASE__INST4_SEG5                      0

#define RSMU_BASE__INST5_SEG0                      0
#define RSMU_BASE__INST5_SEG1                      0
#define RSMU_BASE__INST5_SEG2                      0
#define RSMU_BASE__INST5_SEG3                      0
#define RSMU_BASE__INST5_SEG4                      0
#define RSMU_BASE__INST5_SEG5                      0

#endif

