/*
 * Copyright (C) 2017  Advanced Micro Devices, Inc.
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
#ifndef _vcn_1_0_OFFSET_HEADER
#define _vcn_1_0_OFFSET_HEADER



// addressBlock: uvd_uvd_pg_dec
// base address: 0x1fb00
#define mmUVD_PGFSM_CONFIG                                                                             0x00c0
#define mmUVD_PGFSM_CONFIG_BASE_IDX                                                                    1
#define mmUVD_PGFSM_STATUS                                                                             0x00c1
#define mmUVD_PGFSM_STATUS_BASE_IDX                                                                    1
#define mmUVD_POWER_STATUS                                                                             0x00c4
#define mmUVD_POWER_STATUS_BASE_IDX                                                                    1
#define mmCC_UVD_HARVESTING                                                                            0x00c7
#define mmCC_UVD_HARVESTING_BASE_IDX                                                                   1
<<<<<<< HEAD
=======
#define mmUVD_DPG_LMA_CTL                                                                              0x00d1
#define mmUVD_DPG_LMA_CTL_BASE_IDX                                                                     1
#define mmUVD_DPG_LMA_DATA                                                                             0x00d2
#define mmUVD_DPG_LMA_DATA_BASE_IDX                                                                    1
#define mmUVD_DPG_LMA_MASK                                                                             0x00d3
#define mmUVD_DPG_LMA_MASK_BASE_IDX                                                                    1
#define mmUVD_DPG_PAUSE                                                                                0x00d4
#define mmUVD_DPG_PAUSE_BASE_IDX                                                                       1
>>>>>>> upstream/android-13
#define mmUVD_SCRATCH1                                                                                 0x00d5
#define mmUVD_SCRATCH1_BASE_IDX                                                                        1
#define mmUVD_SCRATCH2                                                                                 0x00d6
#define mmUVD_SCRATCH2_BASE_IDX                                                                        1
#define mmUVD_SCRATCH3                                                                                 0x00d7
#define mmUVD_SCRATCH3_BASE_IDX                                                                        1
#define mmUVD_SCRATCH4                                                                                 0x00d8
#define mmUVD_SCRATCH4_BASE_IDX                                                                        1
#define mmUVD_SCRATCH5                                                                                 0x00d9
#define mmUVD_SCRATCH5_BASE_IDX                                                                        1
#define mmUVD_SCRATCH6                                                                                 0x00da
#define mmUVD_SCRATCH6_BASE_IDX                                                                        1
#define mmUVD_SCRATCH7                                                                                 0x00db
#define mmUVD_SCRATCH7_BASE_IDX                                                                        1
#define mmUVD_SCRATCH8                                                                                 0x00dc
#define mmUVD_SCRATCH8_BASE_IDX                                                                        1
#define mmUVD_SCRATCH9                                                                                 0x00dd
#define mmUVD_SCRATCH9_BASE_IDX                                                                        1
#define mmUVD_SCRATCH10                                                                                0x00de
#define mmUVD_SCRATCH10_BASE_IDX                                                                       1
#define mmUVD_SCRATCH11                                                                                0x00df
#define mmUVD_SCRATCH11_BASE_IDX                                                                       1
#define mmUVD_SCRATCH12                                                                                0x00e0
#define mmUVD_SCRATCH12_BASE_IDX                                                                       1
#define mmUVD_SCRATCH13                                                                                0x00e1
#define mmUVD_SCRATCH13_BASE_IDX                                                                       1
#define mmUVD_SCRATCH14                                                                                0x00e2
#define mmUVD_SCRATCH14_BASE_IDX                                                                       1
#define mmUVD_DPG_LMI_VCPU_CACHE_64BIT_BAR_LOW                                                         0x00e5
#define mmUVD_DPG_LMI_VCPU_CACHE_64BIT_BAR_LOW_BASE_IDX                                                1
#define mmUVD_DPG_LMI_VCPU_CACHE_64BIT_BAR_HIGH                                                        0x00e6
#define mmUVD_DPG_LMI_VCPU_CACHE_64BIT_BAR_HIGH_BASE_IDX                                               1
#define mmUVD_DPG_VCPU_CACHE_OFFSET0                                                                   0x00e7
#define mmUVD_DPG_VCPU_CACHE_OFFSET0_BASE_IDX                                                          1


// addressBlock: uvd_uvdgendec
// base address: 0x1fc00
#define mmUVD_LCM_CGC_CNTRL                                                                            0x0123
#define mmUVD_LCM_CGC_CNTRL_BASE_IDX                                                                   1

<<<<<<< HEAD
=======
#define mmUVD_MIF_CURR_UV_ADDR_CONFIG                                                                  0x0184
#define mmUVD_MIF_CURR_UV_ADDR_CONFIG_BASE_IDX                                                         1
#define mmUVD_MIF_REF_UV_ADDR_CONFIG                                                                   0x0185
#define mmUVD_MIF_REF_UV_ADDR_CONFIG_BASE_IDX                                                          1
#define mmUVD_MIF_RECON1_UV_ADDR_CONFIG                                                                0x0186
#define mmUVD_MIF_RECON1_UV_ADDR_CONFIG_BASE_IDX                                                       1
#define mmUVD_MIF_CURR_ADDR_CONFIG                                                                     0x0192
#define mmUVD_MIF_CURR_ADDR_CONFIG_BASE_IDX                                                            1
#define mmUVD_MIF_REF_ADDR_CONFIG                                                                      0x0193
#define mmUVD_MIF_REF_ADDR_CONFIG_BASE_IDX                                                             1
#define mmUVD_MIF_RECON1_ADDR_CONFIG                                                                   0x01c5
#define mmUVD_MIF_RECON1_ADDR_CONFIG_BASE_IDX                                                          1
>>>>>>> upstream/android-13

// addressBlock: uvd_uvdnpdec
// base address: 0x20000
#define mmUVD_JPEG_CNTL                                                                                0x0200
#define mmUVD_JPEG_CNTL_BASE_IDX                                                                       1
#define mmUVD_JPEG_RB_BASE                                                                             0x0201
#define mmUVD_JPEG_RB_BASE_BASE_IDX                                                                    1
#define mmUVD_JPEG_RB_WPTR                                                                             0x0202
#define mmUVD_JPEG_RB_WPTR_BASE_IDX                                                                    1
#define mmUVD_JPEG_RB_RPTR                                                                             0x0203
#define mmUVD_JPEG_RB_RPTR_BASE_IDX                                                                    1
#define mmUVD_JPEG_RB_SIZE                                                                             0x0204
#define mmUVD_JPEG_RB_SIZE_BASE_IDX                                                                    1
#define mmUVD_JPEG_ADDR_CONFIG                                                                         0x021f
#define mmUVD_JPEG_ADDR_CONFIG_BASE_IDX                                                                1
#define mmUVD_JPEG_PITCH                                                                               0x0222
#define mmUVD_JPEG_PITCH_BASE_IDX                                                                      1
#define mmUVD_JPEG_GPCOM_CMD                                                                           0x022c
#define mmUVD_JPEG_GPCOM_CMD_BASE_IDX                                                                  1
#define mmUVD_JPEG_GPCOM_DATA0                                                                         0x022d
#define mmUVD_JPEG_GPCOM_DATA0_BASE_IDX                                                                1
#define mmUVD_JPEG_GPCOM_DATA1                                                                         0x022e
#define mmUVD_JPEG_GPCOM_DATA1_BASE_IDX                                                                1
#define mmUVD_JPEG_JRB_BASE_LO                                                                         0x022f
#define mmUVD_JPEG_JRB_BASE_LO_BASE_IDX                                                                1
#define mmUVD_JPEG_JRB_BASE_HI                                                                         0x0230
#define mmUVD_JPEG_JRB_BASE_HI_BASE_IDX                                                                1
#define mmUVD_JPEG_JRB_SIZE                                                                            0x0232
#define mmUVD_JPEG_JRB_SIZE_BASE_IDX                                                                   1
#define mmUVD_JPEG_JRB_RPTR                                                                            0x0233
#define mmUVD_JPEG_JRB_RPTR_BASE_IDX                                                                   1
#define mmUVD_JPEG_JRB_WPTR                                                                            0x0234
#define mmUVD_JPEG_JRB_WPTR_BASE_IDX                                                                   1
#define mmUVD_JPEG_UV_ADDR_CONFIG                                                                      0x0238
#define mmUVD_JPEG_UV_ADDR_CONFIG_BASE_IDX                                                             1
#define mmUVD_SEMA_ADDR_LOW                                                                            0x03c0
#define mmUVD_SEMA_ADDR_LOW_BASE_IDX                                                                   1
#define mmUVD_SEMA_ADDR_HIGH                                                                           0x03c1
#define mmUVD_SEMA_ADDR_HIGH_BASE_IDX                                                                  1
#define mmUVD_SEMA_CMD                                                                                 0x03c2
#define mmUVD_SEMA_CMD_BASE_IDX                                                                        1
#define mmUVD_GPCOM_VCPU_CMD                                                                           0x03c3
#define mmUVD_GPCOM_VCPU_CMD_BASE_IDX                                                                  1
#define mmUVD_GPCOM_VCPU_DATA0                                                                         0x03c4
#define mmUVD_GPCOM_VCPU_DATA0_BASE_IDX                                                                1
#define mmUVD_GPCOM_VCPU_DATA1                                                                         0x03c5
#define mmUVD_GPCOM_VCPU_DATA1_BASE_IDX                                                                1
<<<<<<< HEAD
=======
#define mmUVD_ENGINE_CNTL                                                                              0x03c6
#define mmUVD_ENGINE_CNTL_BASE_IDX                                                                     1
>>>>>>> upstream/android-13
#define mmUVD_UDEC_DBW_UV_ADDR_CONFIG                                                                  0x03d2
#define mmUVD_UDEC_DBW_UV_ADDR_CONFIG_BASE_IDX                                                         1
#define mmUVD_UDEC_ADDR_CONFIG                                                                         0x03d3
#define mmUVD_UDEC_ADDR_CONFIG_BASE_IDX                                                                1
#define mmUVD_UDEC_DB_ADDR_CONFIG                                                                      0x03d4
#define mmUVD_UDEC_DB_ADDR_CONFIG_BASE_IDX                                                             1
#define mmUVD_UDEC_DBW_ADDR_CONFIG                                                                     0x03d5
#define mmUVD_UDEC_DBW_ADDR_CONFIG_BASE_IDX                                                            1
#define mmUVD_SUVD_CGC_GATE                                                                            0x03e4
#define mmUVD_SUVD_CGC_GATE_BASE_IDX                                                                   1
#define mmUVD_SUVD_CGC_STATUS                                                                          0x03e5
#define mmUVD_SUVD_CGC_STATUS_BASE_IDX                                                                 1
#define mmUVD_SUVD_CGC_CTRL                                                                            0x03e6
#define mmUVD_SUVD_CGC_CTRL_BASE_IDX                                                                   1
#define mmUVD_LMI_VCPU_CACHE1_64BIT_BAR_LOW                                                            0x03ec
#define mmUVD_LMI_VCPU_CACHE1_64BIT_BAR_LOW_BASE_IDX                                                   1
#define mmUVD_LMI_VCPU_CACHE1_64BIT_BAR_HIGH                                                           0x03ed
#define mmUVD_LMI_VCPU_CACHE1_64BIT_BAR_HIGH_BASE_IDX                                                  1
#define mmUVD_LMI_VCPU_CACHE2_64BIT_BAR_LOW                                                            0x03f0
#define mmUVD_LMI_VCPU_CACHE2_64BIT_BAR_LOW_BASE_IDX                                                   1
#define mmUVD_LMI_VCPU_CACHE2_64BIT_BAR_HIGH                                                           0x03f1
#define mmUVD_LMI_VCPU_CACHE2_64BIT_BAR_HIGH_BASE_IDX                                                  1
#define mmUVD_NO_OP                                                                                    0x03ff
#define mmUVD_NO_OP_BASE_IDX                                                                           1
#define mmUVD_JPEG_CNTL2                                                                               0x0404
#define mmUVD_JPEG_CNTL2_BASE_IDX                                                                      1
#define mmUVD_VERSION                                                                                  0x0409
#define mmUVD_VERSION_BASE_IDX                                                                         1
#define mmUVD_GP_SCRATCH8                                                                              0x040a
#define mmUVD_GP_SCRATCH8_BASE_IDX                                                                     1
#define mmUVD_GP_SCRATCH9                                                                              0x040b
#define mmUVD_GP_SCRATCH9_BASE_IDX                                                                     1
#define mmUVD_GP_SCRATCH10                                                                             0x040c
#define mmUVD_GP_SCRATCH10_BASE_IDX                                                                    1
#define mmUVD_GP_SCRATCH11                                                                             0x040d
#define mmUVD_GP_SCRATCH11_BASE_IDX                                                                    1
#define mmUVD_GP_SCRATCH12                                                                             0x040e
#define mmUVD_GP_SCRATCH12_BASE_IDX                                                                    1
#define mmUVD_GP_SCRATCH13                                                                             0x040f
#define mmUVD_GP_SCRATCH13_BASE_IDX                                                                    1
#define mmUVD_GP_SCRATCH14                                                                             0x0410
#define mmUVD_GP_SCRATCH14_BASE_IDX                                                                    1
#define mmUVD_GP_SCRATCH15                                                                             0x0411
#define mmUVD_GP_SCRATCH15_BASE_IDX                                                                    1
#define mmUVD_GP_SCRATCH16                                                                             0x0412
#define mmUVD_GP_SCRATCH16_BASE_IDX                                                                    1
#define mmUVD_GP_SCRATCH17                                                                             0x0413
#define mmUVD_GP_SCRATCH17_BASE_IDX                                                                    1
#define mmUVD_GP_SCRATCH18                                                                             0x0414
#define mmUVD_GP_SCRATCH18_BASE_IDX                                                                    1
#define mmUVD_GP_SCRATCH19                                                                             0x0415
#define mmUVD_GP_SCRATCH19_BASE_IDX                                                                    1
#define mmUVD_GP_SCRATCH20                                                                             0x0416
#define mmUVD_GP_SCRATCH20_BASE_IDX                                                                    1
#define mmUVD_GP_SCRATCH21                                                                             0x0417
#define mmUVD_GP_SCRATCH21_BASE_IDX                                                                    1
#define mmUVD_GP_SCRATCH22                                                                             0x0418
#define mmUVD_GP_SCRATCH22_BASE_IDX                                                                    1
#define mmUVD_GP_SCRATCH23                                                                             0x0419
#define mmUVD_GP_SCRATCH23_BASE_IDX                                                                    1
#define mmUVD_RB_BASE_LO2                                                                              0x0421
#define mmUVD_RB_BASE_LO2_BASE_IDX                                                                     1
#define mmUVD_RB_BASE_HI2                                                                              0x0422
#define mmUVD_RB_BASE_HI2_BASE_IDX                                                                     1
#define mmUVD_RB_SIZE2                                                                                 0x0423
#define mmUVD_RB_SIZE2_BASE_IDX                                                                        1
#define mmUVD_RB_RPTR2                                                                                 0x0424
#define mmUVD_RB_RPTR2_BASE_IDX                                                                        1
#define mmUVD_RB_WPTR2                                                                                 0x0425
#define mmUVD_RB_WPTR2_BASE_IDX                                                                        1
#define mmUVD_RB_BASE_LO                                                                               0x0426
#define mmUVD_RB_BASE_LO_BASE_IDX                                                                      1
#define mmUVD_RB_BASE_HI                                                                               0x0427
#define mmUVD_RB_BASE_HI_BASE_IDX                                                                      1
#define mmUVD_RB_SIZE                                                                                  0x0428
#define mmUVD_RB_SIZE_BASE_IDX                                                                         1
#define mmUVD_RB_RPTR                                                                                  0x0429
#define mmUVD_RB_RPTR_BASE_IDX                                                                         1
#define mmUVD_RB_WPTR                                                                                  0x042a
#define mmUVD_RB_WPTR_BASE_IDX                                                                         1
#define mmUVD_RB_WPTR4                                                                                 0x0456
#define mmUVD_RB_WPTR4_BASE_IDX                                                                        1
#define mmUVD_JRBC_RB_RPTR                                                                             0x0457
#define mmUVD_JRBC_RB_RPTR_BASE_IDX                                                                    1
#define mmUVD_LMI_JPEG_VMID                                                                            0x045d
#define mmUVD_LMI_JPEG_VMID_BASE_IDX                                                                   1
#define mmUVD_LMI_VCPU_CACHE_64BIT_BAR_HIGH                                                            0x045e
#define mmUVD_LMI_VCPU_CACHE_64BIT_BAR_HIGH_BASE_IDX                                                   1
#define mmUVD_LMI_VCPU_CACHE_64BIT_BAR_LOW                                                             0x045f
#define mmUVD_LMI_VCPU_CACHE_64BIT_BAR_LOW_BASE_IDX                                                    1
#define mmUVD_LMI_RBC_IB_64BIT_BAR_HIGH                                                                0x0466
#define mmUVD_LMI_RBC_IB_64BIT_BAR_HIGH_BASE_IDX                                                       1
#define mmUVD_LMI_RBC_IB_64BIT_BAR_LOW                                                                 0x0467
#define mmUVD_LMI_RBC_IB_64BIT_BAR_LOW_BASE_IDX                                                        1
#define mmUVD_LMI_RBC_RB_64BIT_BAR_HIGH                                                                0x0468
#define mmUVD_LMI_RBC_RB_64BIT_BAR_HIGH_BASE_IDX                                                       1
#define mmUVD_LMI_RBC_RB_64BIT_BAR_LOW                                                                 0x0469
#define mmUVD_LMI_RBC_RB_64BIT_BAR_LOW_BASE_IDX                                                        1


// addressBlock: uvd_uvddec
// base address: 0x20c00
#define mmUVD_SEMA_CNTL                                                                                0x0500
#define mmUVD_SEMA_CNTL_BASE_IDX                                                                       1
#define mmUVD_LMI_JRBC_RB_64BIT_BAR_LOW                                                                0x0503
#define mmUVD_LMI_JRBC_RB_64BIT_BAR_LOW_BASE_IDX                                                       1
#define mmUVD_LMI_JRBC_RB_64BIT_BAR_HIGH                                                               0x0504
#define mmUVD_LMI_JRBC_RB_64BIT_BAR_HIGH_BASE_IDX                                                      1
#define mmUVD_LMI_JRBC_IB_64BIT_BAR_LOW                                                                0x0505
#define mmUVD_LMI_JRBC_IB_64BIT_BAR_LOW_BASE_IDX                                                       1
#define mmUVD_LMI_JRBC_IB_64BIT_BAR_HIGH                                                               0x0506
#define mmUVD_LMI_JRBC_IB_64BIT_BAR_HIGH_BASE_IDX                                                      1
#define mmUVD_LMI_JRBC_IB_VMID                                                                         0x0507
#define mmUVD_LMI_JRBC_IB_VMID_BASE_IDX                                                                1
#define mmUVD_LMI_JRBC_RB_VMID                                                                         0x0508
#define mmUVD_LMI_JRBC_RB_VMID_BASE_IDX                                                                1
#define mmUVD_JRBC_RB_WPTR                                                                             0x0509
#define mmUVD_JRBC_RB_WPTR_BASE_IDX                                                                    1
#define mmUVD_JRBC_RB_CNTL                                                                             0x050a
#define mmUVD_JRBC_RB_CNTL_BASE_IDX                                                                    1
#define mmUVD_JRBC_IB_SIZE                                                                             0x050b
#define mmUVD_JRBC_IB_SIZE_BASE_IDX                                                                    1
#define mmUVD_JRBC_LMI_SWAP_CNTL                                                                       0x050d
#define mmUVD_JRBC_LMI_SWAP_CNTL_BASE_IDX                                                              1
#define mmUVD_LMI_JRBC_RB_MEM_WR_64BIT_BAR_LOW                                                         0x050e
#define mmUVD_LMI_JRBC_RB_MEM_WR_64BIT_BAR_LOW_BASE_IDX                                                1
#define mmUVD_LMI_JRBC_RB_MEM_WR_64BIT_BAR_HIGH                                                        0x050f
#define mmUVD_LMI_JRBC_RB_MEM_WR_64BIT_BAR_HIGH_BASE_IDX                                               1
#define mmUVD_LMI_JRBC_RB_MEM_RD_64BIT_BAR_LOW                                                         0x0510
#define mmUVD_LMI_JRBC_RB_MEM_RD_64BIT_BAR_LOW_BASE_IDX                                                1
#define mmUVD_LMI_JRBC_RB_MEM_RD_64BIT_BAR_HIGH                                                        0x0511
#define mmUVD_LMI_JRBC_RB_MEM_RD_64BIT_BAR_HIGH_BASE_IDX                                               1
#define mmUVD_JRBC_RB_REF_DATA                                                                         0x0512
#define mmUVD_JRBC_RB_REF_DATA_BASE_IDX                                                                1
#define mmUVD_JRBC_RB_COND_RD_TIMER                                                                    0x0513
#define mmUVD_JRBC_RB_COND_RD_TIMER_BASE_IDX                                                           1
#define mmUVD_JRBC_EXTERNAL_REG_BASE                                                                   0x0517
#define mmUVD_JRBC_EXTERNAL_REG_BASE_BASE_IDX                                                          1
#define mmUVD_JRBC_SOFT_RESET                                                                          0x0519
#define mmUVD_JRBC_SOFT_RESET_BASE_IDX                                                                 1
#define mmUVD_JRBC_STATUS                                                                              0x051a
#define mmUVD_JRBC_STATUS_BASE_IDX                                                                     1
#define mmUVD_RB_RPTR3                                                                                 0x051b
#define mmUVD_RB_RPTR3_BASE_IDX                                                                        1
#define mmUVD_RB_WPTR3                                                                                 0x051c
#define mmUVD_RB_WPTR3_BASE_IDX                                                                        1
#define mmUVD_RB_BASE_LO3                                                                              0x051d
#define mmUVD_RB_BASE_LO3_BASE_IDX                                                                     1
#define mmUVD_RB_BASE_HI3                                                                              0x051e
#define mmUVD_RB_BASE_HI3_BASE_IDX                                                                     1
#define mmUVD_RB_SIZE3                                                                                 0x051f
#define mmUVD_RB_SIZE3_BASE_IDX                                                                        1
#define mmJPEG_CGC_GATE                                                                                0x0526
#define mmJPEG_CGC_GATE_BASE_IDX                                                                       1
#define mmUVD_CTX_INDEX                                                                                0x0528
#define mmUVD_CTX_INDEX_BASE_IDX                                                                       1
#define mmUVD_CTX_DATA                                                                                 0x0529
#define mmUVD_CTX_DATA_BASE_IDX                                                                        1
#define mmUVD_CGC_GATE                                                                                 0x052a
#define mmUVD_CGC_GATE_BASE_IDX                                                                        1
#define mmUVD_CGC_STATUS                                                                               0x052b
#define mmUVD_CGC_STATUS_BASE_IDX                                                                      1
#define mmUVD_CGC_CTRL                                                                                 0x052c
#define mmUVD_CGC_CTRL_BASE_IDX                                                                        1
#define mmUVD_GP_SCRATCH0                                                                              0x0534
#define mmUVD_GP_SCRATCH0_BASE_IDX                                                                     1
#define mmUVD_GP_SCRATCH1                                                                              0x0535
#define mmUVD_GP_SCRATCH1_BASE_IDX                                                                     1
#define mmUVD_GP_SCRATCH2                                                                              0x0536
#define mmUVD_GP_SCRATCH2_BASE_IDX                                                                     1
#define mmUVD_GP_SCRATCH3                                                                              0x0537
#define mmUVD_GP_SCRATCH3_BASE_IDX                                                                     1
#define mmUVD_GP_SCRATCH4                                                                              0x0538
#define mmUVD_GP_SCRATCH4_BASE_IDX                                                                     1
#define mmUVD_GP_SCRATCH5                                                                              0x0539
#define mmUVD_GP_SCRATCH5_BASE_IDX                                                                     1
#define mmUVD_GP_SCRATCH6                                                                              0x053a
#define mmUVD_GP_SCRATCH6_BASE_IDX                                                                     1
#define mmUVD_GP_SCRATCH7                                                                              0x053b
#define mmUVD_GP_SCRATCH7_BASE_IDX                                                                     1
#define mmUVD_LMI_VCPU_CACHE_VMID                                                                      0x053c
#define mmUVD_LMI_VCPU_CACHE_VMID_BASE_IDX                                                             1
#define mmUVD_LMI_CTRL2                                                                                0x053d
#define mmUVD_LMI_CTRL2_BASE_IDX                                                                       1
#define mmUVD_MASTINT_EN                                                                               0x0540
#define mmUVD_MASTINT_EN_BASE_IDX                                                                      1
<<<<<<< HEAD
=======
#define mmUVD_SYS_INT_EN                                                                               0x0541
#define mmUVD_SYS_INT_EN_BASE_IDX                                                                      1
>>>>>>> upstream/android-13
#define mmJPEG_CGC_CTRL                                                                                0x0565
#define mmJPEG_CGC_CTRL_BASE_IDX                                                                       1
#define mmUVD_LMI_CTRL                                                                                 0x0566
#define mmUVD_LMI_CTRL_BASE_IDX                                                                        1
#define mmUVD_LMI_STATUS                                                                               0x0567
#define mmUVD_LMI_STATUS_BASE_IDX                                                                      1
#define mmUVD_LMI_VM_CTRL                                                                              0x0568
#define mmUVD_LMI_VM_CTRL_BASE_IDX                                                                     1
#define mmUVD_LMI_SWAP_CNTL                                                                            0x056d
#define mmUVD_LMI_SWAP_CNTL_BASE_IDX                                                                   1
<<<<<<< HEAD
=======
#define mmUVD_MPC_CNTL                                                                                 0x0577
#define mmUVD_MPC_CNTL_BASE_IDX                                                                        1
>>>>>>> upstream/android-13
#define mmUVD_MPC_SET_MUXA0                                                                            0x0579
#define mmUVD_MPC_SET_MUXA0_BASE_IDX                                                                   1
#define mmUVD_MPC_SET_MUXA1                                                                            0x057a
#define mmUVD_MPC_SET_MUXA1_BASE_IDX                                                                   1
#define mmUVD_MPC_SET_MUXB0                                                                            0x057b
#define mmUVD_MPC_SET_MUXB0_BASE_IDX                                                                   1
#define mmUVD_MPC_SET_MUXB1                                                                            0x057c
#define mmUVD_MPC_SET_MUXB1_BASE_IDX                                                                   1
#define mmUVD_MPC_SET_MUX                                                                              0x057d
#define mmUVD_MPC_SET_MUX_BASE_IDX                                                                     1
#define mmUVD_MPC_SET_ALU                                                                              0x057e
#define mmUVD_MPC_SET_ALU_BASE_IDX                                                                     1
#define mmUVD_GPCOM_SYS_CMD                                                                            0x057f
#define mmUVD_GPCOM_SYS_CMD_BASE_IDX                                                                   1
#define mmUVD_GPCOM_SYS_DATA0                                                                          0x0580
#define mmUVD_GPCOM_SYS_DATA0_BASE_IDX                                                                 1
#define mmUVD_GPCOM_SYS_DATA1                                                                          0x0581
#define mmUVD_GPCOM_SYS_DATA1_BASE_IDX                                                                 1
#define mmUVD_VCPU_CACHE_OFFSET0                                                                       0x0582
#define mmUVD_VCPU_CACHE_OFFSET0_BASE_IDX                                                              1
#define mmUVD_VCPU_CACHE_SIZE0                                                                         0x0583
#define mmUVD_VCPU_CACHE_SIZE0_BASE_IDX                                                                1
#define mmUVD_VCPU_CACHE_OFFSET1                                                                       0x0584
#define mmUVD_VCPU_CACHE_OFFSET1_BASE_IDX                                                              1
#define mmUVD_VCPU_CACHE_SIZE1                                                                         0x0585
#define mmUVD_VCPU_CACHE_SIZE1_BASE_IDX                                                                1
#define mmUVD_VCPU_CACHE_OFFSET2                                                                       0x0586
#define mmUVD_VCPU_CACHE_OFFSET2_BASE_IDX                                                              1
#define mmUVD_VCPU_CACHE_SIZE2                                                                         0x0587
#define mmUVD_VCPU_CACHE_SIZE2_BASE_IDX                                                                1
#define mmUVD_VCPU_CNTL                                                                                0x0598
#define mmUVD_VCPU_CNTL_BASE_IDX                                                                       1
#define mmUVD_SOFT_RESET                                                                               0x05a0
#define mmUVD_SOFT_RESET_BASE_IDX                                                                      1
#define mmUVD_LMI_RBC_IB_VMID                                                                          0x05a1
#define mmUVD_LMI_RBC_IB_VMID_BASE_IDX                                                                 1
#define mmUVD_RBC_IB_SIZE                                                                              0x05a2
#define mmUVD_RBC_IB_SIZE_BASE_IDX                                                                     1
#define mmUVD_RBC_RB_RPTR                                                                              0x05a4
#define mmUVD_RBC_RB_RPTR_BASE_IDX                                                                     1
#define mmUVD_RBC_RB_WPTR                                                                              0x05a5
#define mmUVD_RBC_RB_WPTR_BASE_IDX                                                                     1
#define mmUVD_RBC_RB_WPTR_CNTL                                                                         0x05a6
#define mmUVD_RBC_RB_WPTR_CNTL_BASE_IDX                                                                1
#define mmUVD_RBC_RB_CNTL                                                                              0x05a9
#define mmUVD_RBC_RB_CNTL_BASE_IDX                                                                     1
#define mmUVD_RBC_RB_RPTR_ADDR                                                                         0x05aa
#define mmUVD_RBC_RB_RPTR_ADDR_BASE_IDX                                                                1
#define mmUVD_STATUS                                                                                   0x05af
#define mmUVD_STATUS_BASE_IDX                                                                          1
#define mmUVD_SEMA_TIMEOUT_STATUS                                                                      0x05b0
#define mmUVD_SEMA_TIMEOUT_STATUS_BASE_IDX                                                             1
#define mmUVD_SEMA_WAIT_INCOMPLETE_TIMEOUT_CNTL                                                        0x05b1
#define mmUVD_SEMA_WAIT_INCOMPLETE_TIMEOUT_CNTL_BASE_IDX                                               1
#define mmUVD_SEMA_WAIT_FAULT_TIMEOUT_CNTL                                                             0x05b2
#define mmUVD_SEMA_WAIT_FAULT_TIMEOUT_CNTL_BASE_IDX                                                    1
#define mmUVD_SEMA_SIGNAL_INCOMPLETE_TIMEOUT_CNTL                                                      0x05b3
#define mmUVD_SEMA_SIGNAL_INCOMPLETE_TIMEOUT_CNTL_BASE_IDX                                             1
#define mmUVD_CONTEXT_ID                                                                               0x05bd
#define mmUVD_CONTEXT_ID_BASE_IDX                                                                      1
#define mmUVD_CONTEXT_ID2                                                                              0x05bf
#define mmUVD_CONTEXT_ID2_BASE_IDX                                                                     1
#define mmUVD_RBC_WPTR_POLL_CNTL                                                                       0x05d8
#define mmUVD_RBC_WPTR_POLL_CNTL_BASE_IDX                                                              1
#define mmUVD_RBC_WPTR_POLL_ADDR                                                                       0x05d9
#define mmUVD_RBC_WPTR_POLL_ADDR_BASE_IDX                                                              1
#define mmUVD_RB_BASE_LO4                                                                              0x05df
#define mmUVD_RB_BASE_LO4_BASE_IDX                                                                     1
#define mmUVD_RB_BASE_HI4                                                                              0x05e0
#define mmUVD_RB_BASE_HI4_BASE_IDX                                                                     1
#define mmUVD_RB_SIZE4                                                                                 0x05e1
#define mmUVD_RB_SIZE4_BASE_IDX                                                                        1
#define mmUVD_RB_RPTR4                                                                                 0x05e2
#define mmUVD_RB_RPTR4_BASE_IDX                                                                        1


#endif
