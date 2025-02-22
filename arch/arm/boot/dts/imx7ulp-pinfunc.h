<<<<<<< HEAD
/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
>>>>>>> upstream/android-13
 */

#ifndef __DTS_IMX7ULP_PINFUNC_H
#define __DTS_IMX7ULP_PINFUNC_H

/*
 * The pin function ID is a tuple of
 * <mux_conf_reg input_reg mux_mode input_val>
 */

#define IMX7ULP_PAD_PTC0__PTC0                                       0x0000 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC0__TRACE_D15                                  0x0000 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC0__LPUART4_CTS_B                              0x0000 0x0244 0x4 0x1
#define IMX7ULP_PAD_PTC0__LPI2C4_SCL                                 0x0000 0x0278 0x5 0x1
#define IMX7ULP_PAD_PTC0__TPM4_CLKIN                                 0x0000 0x0298 0x6 0x1
#define IMX7ULP_PAD_PTC0__FB_AD0                                     0x0000 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC1__PTC1                                       0x0004 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC1__TRACE_D14                                  0x0004 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC1__LPUART4_RTS_B                              0x0004 0x0000 0x4 0x0
#define IMX7ULP_PAD_PTC1__LPI2C4_SDA                                 0x0004 0x027c 0x5 0x1
#define IMX7ULP_PAD_PTC1__TPM4_CH0                                   0x0004 0x0280 0x6 0x1
#define IMX7ULP_PAD_PTC1__FB_AD1                                     0x0004 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC2__PTC2                                       0x0008 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC2__TRACE_D13                                  0x0008 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC2__LPUART4_TX                                 0x0008 0x024c 0x4 0x1
#define IMX7ULP_PAD_PTC2__LPI2C4_HREQ                                0x0008 0x0274 0x5 0x1
#define IMX7ULP_PAD_PTC2__TPM4_CH1                                   0x0008 0x0284 0x6 0x1
#define IMX7ULP_PAD_PTC2__FB_AD2                                     0x0008 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC3__PTC3                                       0x000c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC3__TRACE_D12                                  0x000c 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC3__LPUART4_RX                                 0x000c 0x0248 0x4 0x1
#define IMX7ULP_PAD_PTC3__TPM4_CH2                                   0x000c 0x0288 0x6 0x1
#define IMX7ULP_PAD_PTC3__FB_AD3                                     0x000c 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC4__PTC4                                       0x0010 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC4__TRACE_D11                                  0x0010 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC4__FXIO1_D0                                   0x0010 0x0204 0x2 0x1
#define IMX7ULP_PAD_PTC4__LPSPI2_PCS1                                0x0010 0x02a0 0x3 0x1
#define IMX7ULP_PAD_PTC4__LPUART5_CTS_B                              0x0010 0x0250 0x4 0x1
#define IMX7ULP_PAD_PTC4__LPI2C5_SCL                                 0x0010 0x02bc 0x5 0x1
#define IMX7ULP_PAD_PTC4__TPM4_CH3                                   0x0010 0x028c 0x6 0x1
#define IMX7ULP_PAD_PTC4__FB_AD4                                     0x0010 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC5__PTC5                                       0x0014 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC5__TRACE_D10                                  0x0014 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC5__FXIO1_D1                                   0x0014 0x0208 0x2 0x1
#define IMX7ULP_PAD_PTC5__LPSPI2_PCS2                                0x0014 0x02a4 0x3 0x1
#define IMX7ULP_PAD_PTC5__LPUART5_RTS_B                              0x0014 0x0000 0x4 0x0
#define IMX7ULP_PAD_PTC5__LPI2C5_SDA                                 0x0014 0x02c0 0x5 0x1
#define IMX7ULP_PAD_PTC5__TPM4_CH4                                   0x0014 0x0290 0x6 0x1
#define IMX7ULP_PAD_PTC5__FB_AD5                                     0x0014 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC6__PTC6                                       0x0018 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC6__TRACE_D9                                   0x0018 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC6__FXIO1_D2                                   0x0018 0x020c 0x2 0x1
#define IMX7ULP_PAD_PTC6__LPSPI2_PCS3                                0x0018 0x02a8 0x3 0x1
#define IMX7ULP_PAD_PTC6__LPUART5_TX                                 0x0018 0x0258 0x4 0x1
#define IMX7ULP_PAD_PTC6__LPI2C5_HREQ                                0x0018 0x02b8 0x5 0x1
#define IMX7ULP_PAD_PTC6__TPM4_CH5                                   0x0018 0x0294 0x6 0x1
#define IMX7ULP_PAD_PTC6__FB_AD6                                     0x0018 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC7__PTC7                                       0x001c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC7__TRACE_D8                                   0x001c 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC7__FXIO1_D3                                   0x001c 0x0210 0x2 0x1
#define IMX7ULP_PAD_PTC7__LPUART5_RX                                 0x001c 0x0254 0x4 0x1
#define IMX7ULP_PAD_PTC7__TPM5_CH1                                   0x001c 0x02c8 0x6 0x1
#define IMX7ULP_PAD_PTC7__FB_AD7                                     0x001c 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC8__PTC8                                       0x0020 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC8__TRACE_D7                                   0x0020 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC8__FXIO1_D4                                   0x0020 0x0214 0x2 0x1
#define IMX7ULP_PAD_PTC8__LPSPI2_SIN                                 0x0020 0x02b0 0x3 0x1
#define IMX7ULP_PAD_PTC8__LPUART6_CTS_B                              0x0020 0x025c 0x4 0x1
#define IMX7ULP_PAD_PTC8__LPI2C6_SCL                                 0x0020 0x02fc 0x5 0x1
#define IMX7ULP_PAD_PTC8__TPM5_CLKIN                                 0x0020 0x02cc 0x6 0x1
#define IMX7ULP_PAD_PTC8__FB_AD8                                     0x0020 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC9__PTC9                                       0x0024 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC9__TRACE_D6                                   0x0024 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC9__FXIO1_D5                                   0x0024 0x0218 0x2 0x1
#define IMX7ULP_PAD_PTC9__LPSPI2_SOUT                                0x0024 0x02b4 0x3 0x1
#define IMX7ULP_PAD_PTC9__LPUART6_RTS_B                              0x0024 0x0000 0x4 0x0
#define IMX7ULP_PAD_PTC9__LPI2C6_SDA                                 0x0024 0x0300 0x5 0x1
#define IMX7ULP_PAD_PTC9__TPM5_CH0                                   0x0024 0x02c4 0x6 0x1
#define IMX7ULP_PAD_PTC9__FB_AD9                                     0x0024 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC10__PTC10                                     0x0028 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC10__TRACE_D5                                  0x0028 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC10__FXIO1_D6                                  0x0028 0x021c 0x2 0x1
#define IMX7ULP_PAD_PTC10__LPSPI2_SCK                                0x0028 0x02ac 0x3 0x1
#define IMX7ULP_PAD_PTC10__LPUART6_TX                                0x0028 0x0264 0x4 0x1
#define IMX7ULP_PAD_PTC10__LPI2C6_HREQ                               0x0028 0x02f8 0x5 0x1
#define IMX7ULP_PAD_PTC10__TPM7_CH3                                  0x0028 0x02e8 0x6 0x1
#define IMX7ULP_PAD_PTC10__FB_AD10                                   0x0028 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC11__PTC11                                     0x002c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC11__TRACE_D4                                  0x002c 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC11__FXIO1_D7                                  0x002c 0x0220 0x2 0x1
#define IMX7ULP_PAD_PTC11__LPSPI2_PCS0                               0x002c 0x029c 0x3 0x1
#define IMX7ULP_PAD_PTC11__LPUART6_RX                                0x002c 0x0260 0x4 0x1
#define IMX7ULP_PAD_PTC11__TPM7_CH4                                  0x002c 0x02ec 0x6 0x1
#define IMX7ULP_PAD_PTC11__FB_AD11                                   0x002c 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC12__PTC12                                     0x0030 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC12__TRACE_D3                                  0x0030 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC12__FXIO1_D8                                  0x0030 0x0224 0x2 0x1
#define IMX7ULP_PAD_PTC12__LPSPI3_PCS1                               0x0030 0x0314 0x3 0x1
#define IMX7ULP_PAD_PTC12__LPUART7_CTS_B                             0x0030 0x0268 0x4 0x1
#define IMX7ULP_PAD_PTC12__LPI2C7_SCL                                0x0030 0x0308 0x5 0x1
#define IMX7ULP_PAD_PTC12__TPM7_CH5                                  0x0030 0x02f0 0x6 0x1
#define IMX7ULP_PAD_PTC12__FB_AD12                                   0x0030 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC13__PTC13                                     0x0034 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC13__TRACE_D2                                  0x0034 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC13__FXIO1_D9                                  0x0034 0x0228 0x2 0x1
#define IMX7ULP_PAD_PTC13__LPSPI3_PCS2                               0x0034 0x0318 0x3 0x1
#define IMX7ULP_PAD_PTC13__LPUART7_RTS_B                             0x0034 0x0000 0x4 0x0
#define IMX7ULP_PAD_PTC13__LPI2C7_SDA                                0x0034 0x030c 0x5 0x1
#define IMX7ULP_PAD_PTC13__TPM7_CLKIN                                0x0034 0x02f4 0x6 0x1
#define IMX7ULP_PAD_PTC13__FB_AD13                                   0x0034 0x0000 0x9 0x0
<<<<<<< HEAD
=======
#define IMX7ULP_PAD_PTC13__USB0_ID                                   0x0034 0x0338 0xb 0x1
>>>>>>> upstream/android-13
#define IMX7ULP_PAD_PTC14__PTC14                                     0x0038 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC14__TRACE_D1                                  0x0038 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC14__FXIO1_D10                                 0x0038 0x022c 0x2 0x1
#define IMX7ULP_PAD_PTC14__LPSPI3_PCS3                               0x0038 0x031c 0x3 0x1
#define IMX7ULP_PAD_PTC14__LPUART7_TX                                0x0038 0x0270 0x4 0x1
#define IMX7ULP_PAD_PTC14__LPI2C7_HREQ                               0x0038 0x0304 0x5 0x1
#define IMX7ULP_PAD_PTC14__TPM7_CH0                                  0x0038 0x02dc 0x6 0x1
#define IMX7ULP_PAD_PTC14__FB_AD14                                   0x0038 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC15__PTC15                                     0x003c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC15__TRACE_D0                                  0x003c 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC15__FXIO1_D11                                 0x003c 0x0230 0x2 0x1
#define IMX7ULP_PAD_PTC15__LPUART7_RX                                0x003c 0x026c 0x4 0x1
#define IMX7ULP_PAD_PTC15__TPM7_CH1                                  0x003c 0x02e0 0x6 0x1
#define IMX7ULP_PAD_PTC15__FB_AD15                                   0x003c 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC16__PTC16                                     0x0040 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC16__TRACE_CLKOUT                              0x0040 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTC16__FXIO1_D12                                 0x0040 0x0234 0x2 0x1
#define IMX7ULP_PAD_PTC16__LPSPI3_SIN                                0x0040 0x0324 0x3 0x1
#define IMX7ULP_PAD_PTC16__TPM7_CH2                                  0x0040 0x02e4 0x6 0x1
#define IMX7ULP_PAD_PTC16__FB_ALE_FB_CS1_B_FB_TS_B                   0x0040 0x0000 0x9 0x0
<<<<<<< HEAD
=======
#define IMX7ULP_PAD_PTC16__USB1_OC2                                  0x0040 0x0334 0xb 0x1
>>>>>>> upstream/android-13
#define IMX7ULP_PAD_PTC17__PTC17                                     0x0044 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC17__FXIO1_D13                                 0x0044 0x0238 0x2 0x1
#define IMX7ULP_PAD_PTC17__LPSPI3_SOUT                               0x0044 0x0328 0x3 0x1
#define IMX7ULP_PAD_PTC17__TPM6_CLKIN                                0x0044 0x02d8 0x6 0x1
#define IMX7ULP_PAD_PTC17__FB_CS0_B                                  0x0044 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTC18__PTC18                                     0x0048 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC18__FXIO1_D14                                 0x0048 0x023c 0x2 0x1
#define IMX7ULP_PAD_PTC18__LPSPI3_SCK                                0x0048 0x0320 0x3 0x1
#define IMX7ULP_PAD_PTC18__TPM6_CH0                                  0x0048 0x02d0 0x6 0x1
#define IMX7ULP_PAD_PTC18__FB_OE_B                                   0x0048 0x0000 0x9 0x0
<<<<<<< HEAD
=======
#define IMX7ULP_PAD_PTC18__USB0_ID                                   0x0048 0x0338 0xb 0x2
#define IMX7ULP_PAD_PTC18__VIU_DE                                    0x0048 0x033c 0xc 0x1
>>>>>>> upstream/android-13
#define IMX7ULP_PAD_PTC19__PTC19                                     0x004c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTC19__FXIO1_D15                                 0x004c 0x0240 0x2 0x1
#define IMX7ULP_PAD_PTC19__LPSPI3_PCS0                               0x004c 0x0310 0x3 0x1
#define IMX7ULP_PAD_PTC19__TPM6_CH1                                  0x004c 0x02d4 0x6 0x1
#define IMX7ULP_PAD_PTC19__FB_A16                                    0x004c 0x0000 0x9 0x0
<<<<<<< HEAD
=======
#define IMX7ULP_PAD_PTC19__USB0_ID                                   0x004c 0x0338 0xa 0x3
#define IMX7ULP_PAD_PTC19__USB1_PWR2                                 0x004c 0x0000 0xb 0x0
#define IMX7ULP_PAD_PTC19__VIU_DE                                    0x004c 0x033c 0xc 0x3
>>>>>>> upstream/android-13
#define IMX7ULP_PAD_PTD0__PTD0                                       0x0080 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTD0__SDHC0_RESET_B                              0x0080 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTD1__PTD1                                       0x0084 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTD1__SDHC0_CMD                                  0x0084 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTD2__PTD2                                       0x0088 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTD2__SDHC0_CLK                                  0x0088 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTD3__PTD3                                       0x008c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTD3__SDHC0_D7                                   0x008c 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTD4__PTD4                                       0x0090 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTD4__SDHC0_D6                                   0x0090 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTD5__PTD5                                       0x0094 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTD5__SDHC0_D5                                   0x0094 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTD6__PTD6                                       0x0098 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTD6__SDHC0_D4                                   0x0098 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTD7__PTD7                                       0x009c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTD7__SDHC0_D3                                   0x009c 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTD8__PTD8                                       0x00a0 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTD8__TPM4_CLKIN                                 0x00a0 0x0298 0x6 0x2
#define IMX7ULP_PAD_PTD8__SDHC0_D2                                   0x00a0 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTD9__PTD9                                       0x00a4 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTD9__TPM4_CH0                                   0x00a4 0x0280 0x6 0x2
#define IMX7ULP_PAD_PTD9__SDHC0_D1                                   0x00a4 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTD10__PTD10                                     0x00a8 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTD10__TPM4_CH1                                  0x00a8 0x0284 0x6 0x2
#define IMX7ULP_PAD_PTD10__SDHC0_D0                                  0x00a8 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTD11__PTD11                                     0x00ac 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTD11__TPM4_CH2                                  0x00ac 0x0288 0x6 0x2
#define IMX7ULP_PAD_PTD11__SDHC0_DQS                                 0x00ac 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTE0__PTE0                                       0x0100 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE0__FXIO1_D31                                  0x0100 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE0__LPSPI2_PCS1                                0x0100 0x02a0 0x3 0x2
#define IMX7ULP_PAD_PTE0__LPUART4_CTS_B                              0x0100 0x0244 0x4 0x2
#define IMX7ULP_PAD_PTE0__LPI2C4_SCL                                 0x0100 0x0278 0x5 0x2
#define IMX7ULP_PAD_PTE0__SDHC1_D1                                   0x0100 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTE0__FB_A25                                     0x0100 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTE1__PTE1                                       0x0104 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE1__FXIO1_D30                                  0x0104 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE1__LPSPI2_PCS2                                0x0104 0x02a4 0x3 0x2
#define IMX7ULP_PAD_PTE1__LPUART4_RTS_B                              0x0104 0x0000 0x4 0x0
#define IMX7ULP_PAD_PTE1__LPI2C4_SDA                                 0x0104 0x027c 0x5 0x2
#define IMX7ULP_PAD_PTE1__SDHC1_D0                                   0x0104 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTE1__FB_A26                                     0x0104 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTE2__PTE2                                       0x0108 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE2__FXIO1_D29                                  0x0108 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE2__LPSPI2_PCS3                                0x0108 0x02a8 0x3 0x2
#define IMX7ULP_PAD_PTE2__LPUART4_TX                                 0x0108 0x024c 0x4 0x2
#define IMX7ULP_PAD_PTE2__LPI2C4_HREQ                                0x0108 0x0274 0x5 0x2
#define IMX7ULP_PAD_PTE2__SDHC1_CLK                                  0x0108 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTE3__PTE3                                       0x010c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE3__FXIO1_D28                                  0x010c 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE3__LPUART4_RX                                 0x010c 0x0248 0x4 0x2
#define IMX7ULP_PAD_PTE3__TPM5_CH1                                   0x010c 0x02c8 0x6 0x2
#define IMX7ULP_PAD_PTE3__SDHC1_CMD                                  0x010c 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTE4__PTE4                                       0x0110 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE4__FXIO1_D27                                  0x0110 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE4__LPSPI2_SIN                                 0x0110 0x02b0 0x3 0x2
#define IMX7ULP_PAD_PTE4__LPUART5_CTS_B                              0x0110 0x0250 0x4 0x2
#define IMX7ULP_PAD_PTE4__LPI2C5_SCL                                 0x0110 0x02bc 0x5 0x2
#define IMX7ULP_PAD_PTE4__TPM5_CLKIN                                 0x0110 0x02cc 0x6 0x2
#define IMX7ULP_PAD_PTE4__SDHC1_D3                                   0x0110 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTE5__PTE5                                       0x0114 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE5__FXIO1_D26                                  0x0114 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE5__LPSPI2_SOUT                                0x0114 0x02b4 0x3 0x2
#define IMX7ULP_PAD_PTE5__LPUART5_RTS_B                              0x0114 0x0000 0x4 0x0
#define IMX7ULP_PAD_PTE5__LPI2C5_SDA                                 0x0114 0x02c0 0x5 0x2
#define IMX7ULP_PAD_PTE5__TPM5_CH0                                   0x0114 0x02c4 0x6 0x2
#define IMX7ULP_PAD_PTE5__SDHC1_D2                                   0x0114 0x0000 0x8 0x0
<<<<<<< HEAD
=======
#define IMX7ULP_PAD_PTE5__VIU_DE                                     0x0114 0x033c 0xc 0x2
>>>>>>> upstream/android-13
#define IMX7ULP_PAD_PTE6__PTE6                                       0x0118 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE6__FXIO1_D25                                  0x0118 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE6__LPSPI2_SCK                                 0x0118 0x02ac 0x3 0x2
#define IMX7ULP_PAD_PTE6__LPUART5_TX                                 0x0118 0x0258 0x4 0x2
#define IMX7ULP_PAD_PTE6__LPI2C5_HREQ                                0x0118 0x02b8 0x5 0x2
#define IMX7ULP_PAD_PTE6__TPM7_CH3                                   0x0118 0x02e8 0x6 0x2
#define IMX7ULP_PAD_PTE6__SDHC1_D4                                   0x0118 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTE6__FB_A17                                     0x0118 0x0000 0x9 0x0
<<<<<<< HEAD
#define IMX7ULP_PAD_PTE7__PTE7                                       0x011c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE7__TRACE_D7                                   0x011c 0x0000 0xa 0x0
=======
#define IMX7ULP_PAD_PTE6__USB0_OC                                    0x0118 0x0330 0xb 0x1
#define IMX7ULP_PAD_PTE7__PTE7                                       0x011c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE7__TRACE_D7                                   0x011c 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTE7__USB0_PWR                                   0x011c 0x0000 0xb 0x0
>>>>>>> upstream/android-13
#define IMX7ULP_PAD_PTE7__VIU_FID                                    0x011c 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTE7__FXIO1_D24                                  0x011c 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE7__LPSPI2_PCS0                                0x011c 0x029c 0x3 0x2
#define IMX7ULP_PAD_PTE7__LPUART5_RX                                 0x011c 0x0254 0x4 0x2
#define IMX7ULP_PAD_PTE7__TPM7_CH4                                   0x011c 0x02ec 0x6 0x2
#define IMX7ULP_PAD_PTE7__SDHC1_D5                                   0x011c 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTE7__FB_A18                                     0x011c 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTE8__PTE8                                       0x0120 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE8__TRACE_D6                                   0x0120 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTE8__VIU_D16                                    0x0120 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTE8__FXIO1_D23                                  0x0120 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE8__LPSPI3_PCS1                                0x0120 0x0314 0x3 0x2
#define IMX7ULP_PAD_PTE8__LPUART6_CTS_B                              0x0120 0x025c 0x4 0x2
#define IMX7ULP_PAD_PTE8__LPI2C6_SCL                                 0x0120 0x02fc 0x5 0x2
#define IMX7ULP_PAD_PTE8__TPM7_CH5                                   0x0120 0x02f0 0x6 0x2
#define IMX7ULP_PAD_PTE8__SDHC1_WP                                   0x0120 0x0200 0x7 0x1
#define IMX7ULP_PAD_PTE8__SDHC1_D6                                   0x0120 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTE8__FB_CS3_B_FB_BE7_0_BLS31_24_B               0x0120 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTE9__PTE9                                       0x0124 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE9__TRACE_D5                                   0x0124 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTE9__VIU_D17                                    0x0124 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTE9__FXIO1_D22                                  0x0124 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE9__LPSPI3_PCS2                                0x0124 0x0318 0x3 0x2
#define IMX7ULP_PAD_PTE9__LPUART6_RTS_B                              0x0124 0x0000 0x4 0x0
#define IMX7ULP_PAD_PTE9__LPI2C6_SDA                                 0x0124 0x0300 0x5 0x2
#define IMX7ULP_PAD_PTE9__TPM7_CLKIN                                 0x0124 0x02f4 0x6 0x2
#define IMX7ULP_PAD_PTE9__SDHC1_CD                                   0x0124 0x032c 0x7 0x1
#define IMX7ULP_PAD_PTE9__SDHC1_D7                                   0x0124 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTE9__FB_TBST_B_FB_CS2_B_FB_BE15_8_BLS23_16_B    0x0124 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTE10__PTE10                                     0x0128 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE10__TRACE_D4                                  0x0128 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTE10__VIU_D18                                   0x0128 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTE10__FXIO1_D21                                 0x0128 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE10__LPSPI3_PCS3                               0x0128 0x031c 0x3 0x2
#define IMX7ULP_PAD_PTE10__LPUART6_TX                                0x0128 0x0264 0x4 0x2
#define IMX7ULP_PAD_PTE10__LPI2C6_HREQ                               0x0128 0x02f8 0x5 0x2
#define IMX7ULP_PAD_PTE10__TPM7_CH0                                  0x0128 0x02dc 0x6 0x2
#define IMX7ULP_PAD_PTE10__SDHC1_VS                                  0x0128 0x0000 0x7 0x0
#define IMX7ULP_PAD_PTE10__SDHC1_DQS                                 0x0128 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTE10__FB_A19                                    0x0128 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTE11__PTE11                                     0x012c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE11__TRACE_D3                                  0x012c 0x0000 0xa 0x0
#define IMX7ULP_PAD_PTE11__VIU_D19                                   0x012c 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTE11__FXIO1_D20                                 0x012c 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE11__LPUART6_RX                                0x012c 0x0260 0x4 0x2
#define IMX7ULP_PAD_PTE11__TPM7_CH1                                  0x012c 0x02e0 0x6 0x2
#define IMX7ULP_PAD_PTE11__SDHC1_RESET_B                             0x012c 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTE11__FB_A20                                    0x012c 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTE12__PTE12                                     0x0130 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE12__TRACE_D2                                  0x0130 0x0000 0xa 0x0
<<<<<<< HEAD
=======
#define IMX7ULP_PAD_PTE12__USB1_OC2                                  0x0130 0x0334 0xb 0x2
>>>>>>> upstream/android-13
#define IMX7ULP_PAD_PTE12__VIU_D20                                   0x0130 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTE12__FXIO1_D19                                 0x0130 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE12__LPSPI3_SIN                                0x0130 0x0324 0x3 0x2
#define IMX7ULP_PAD_PTE12__LPUART7_CTS_B                             0x0130 0x0268 0x4 0x2
#define IMX7ULP_PAD_PTE12__LPI2C7_SCL                                0x0130 0x0308 0x5 0x2
#define IMX7ULP_PAD_PTE12__TPM7_CH2                                  0x0130 0x02e4 0x6 0x2
#define IMX7ULP_PAD_PTE12__SDHC1_WP                                  0x0130 0x0200 0x8 0x2
#define IMX7ULP_PAD_PTE12__FB_A21                                    0x0130 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTE13__PTE13                                     0x0134 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE13__TRACE_D1                                  0x0134 0x0000 0xa 0x0
<<<<<<< HEAD
=======
#define IMX7ULP_PAD_PTE13__USB1_PWR2                                 0x0134 0x0000 0xb 0x0
>>>>>>> upstream/android-13
#define IMX7ULP_PAD_PTE13__VIU_D21                                   0x0134 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTE13__FXIO1_D18                                 0x0134 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE13__LPSPI3_SOUT                               0x0134 0x0328 0x3 0x2
#define IMX7ULP_PAD_PTE13__LPUART7_RTS_B                             0x0134 0x0000 0x4 0x0
#define IMX7ULP_PAD_PTE13__LPI2C7_SDA                                0x0134 0x030c 0x5 0x2
#define IMX7ULP_PAD_PTE13__TPM6_CLKIN                                0x0134 0x02d8 0x6 0x2
#define IMX7ULP_PAD_PTE13__SDHC1_CD                                  0x0134 0x032c 0x8 0x2
#define IMX7ULP_PAD_PTE13__FB_A22                                    0x0134 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTE14__PTE14                                     0x0138 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE14__TRACE_D0                                  0x0138 0x0000 0xa 0x0
<<<<<<< HEAD
=======
#define IMX7ULP_PAD_PTE14__USB0_OC                                   0x0138 0x0330 0xb 0x2
>>>>>>> upstream/android-13
#define IMX7ULP_PAD_PTE14__VIU_D22                                   0x0138 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTE14__FXIO1_D17                                 0x0138 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE14__LPSPI3_SCK                                0x0138 0x0320 0x3 0x2
#define IMX7ULP_PAD_PTE14__LPUART7_TX                                0x0138 0x0270 0x4 0x2
#define IMX7ULP_PAD_PTE14__LPI2C7_HREQ                               0x0138 0x0304 0x5 0x2
#define IMX7ULP_PAD_PTE14__TPM6_CH0                                  0x0138 0x02d0 0x6 0x2
#define IMX7ULP_PAD_PTE14__SDHC1_VS                                  0x0138 0x0000 0x8 0x0
#define IMX7ULP_PAD_PTE14__FB_A23                                    0x0138 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTE15__PTE15                                     0x013c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTE15__TRACE_CLKOUT                              0x013c 0x0000 0xa 0x0
<<<<<<< HEAD
=======
#define IMX7ULP_PAD_PTE15__USB0_PWR                                  0x013c 0x0000 0xb 0x0
>>>>>>> upstream/android-13
#define IMX7ULP_PAD_PTE15__VIU_D23                                   0x013c 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTE15__FXIO1_D16                                 0x013c 0x0000 0x2 0x0
#define IMX7ULP_PAD_PTE15__LPSPI3_PCS0                               0x013c 0x0310 0x3 0x2
#define IMX7ULP_PAD_PTE15__LPUART7_RX                                0x013c 0x026c 0x4 0x2
#define IMX7ULP_PAD_PTE15__TPM6_CH1                                  0x013c 0x02d4 0x6 0x2
#define IMX7ULP_PAD_PTE15__FB_A24                                    0x013c 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF0__PTF0                                       0x0180 0x0000 0x1 0x0
<<<<<<< HEAD
#define IMX7ULP_PAD_PTF0__VIU_DE                                     0x0180 0x0000 0xc 0x0
=======
#define IMX7ULP_PAD_PTF0__VIU_DE                                     0x0180 0x033c 0xc 0x0
>>>>>>> upstream/android-13
#define IMX7ULP_PAD_PTF0__LPUART4_CTS_B                              0x0180 0x0244 0x4 0x3
#define IMX7ULP_PAD_PTF0__LPI2C4_SCL                                 0x0180 0x0278 0x5 0x3
#define IMX7ULP_PAD_PTF0__TPM4_CLKIN                                 0x0180 0x0298 0x6 0x3
#define IMX7ULP_PAD_PTF0__FB_RW_B                                    0x0180 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF1__PTF1                                       0x0184 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF1__VIU_HSYNC                                  0x0184 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF1__LPUART4_RTS_B                              0x0184 0x0000 0x4 0x0
#define IMX7ULP_PAD_PTF1__LPI2C4_SDA                                 0x0184 0x027c 0x5 0x3
#define IMX7ULP_PAD_PTF1__TPM4_CH0                                   0x0184 0x0280 0x6 0x3
#define IMX7ULP_PAD_PTF1__CLKOUT                                     0x0184 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF2__PTF2                                       0x0188 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF2__VIU_VSYNC                                  0x0188 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF2__LPUART4_TX                                 0x0188 0x024c 0x4 0x3
#define IMX7ULP_PAD_PTF2__LPI2C4_HREQ                                0x0188 0x0274 0x5 0x3
#define IMX7ULP_PAD_PTF2__TPM4_CH1                                   0x0188 0x0284 0x6 0x3
#define IMX7ULP_PAD_PTF2__FB_TSIZ1_FB_CS5_B_FB_BE23_16_BLS15_8_B     0x0188 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF3__PTF3                                       0x018c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF3__VIU_PCLK                                   0x018c 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF3__LPUART4_RX                                 0x018c 0x0248 0x4 0x3
#define IMX7ULP_PAD_PTF3__TPM4_CH2                                   0x018c 0x0288 0x6 0x3
#define IMX7ULP_PAD_PTF3__FB_AD16                                    0x018c 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF4__PTF4                                       0x0190 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF4__VIU_D0                                     0x0190 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF4__FXIO1_D0                                   0x0190 0x0204 0x2 0x2
#define IMX7ULP_PAD_PTF4__LPSPI2_PCS1                                0x0190 0x02a0 0x3 0x3
#define IMX7ULP_PAD_PTF4__LPUART5_CTS_B                              0x0190 0x0250 0x4 0x3
#define IMX7ULP_PAD_PTF4__LPI2C5_SCL                                 0x0190 0x02bc 0x5 0x3
#define IMX7ULP_PAD_PTF4__TPM4_CH3                                   0x0190 0x028c 0x6 0x2
#define IMX7ULP_PAD_PTF4__FB_AD17                                    0x0190 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF5__PTF5                                       0x0194 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF5__VIU_D1                                     0x0194 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF5__FXIO1_D1                                   0x0194 0x0208 0x2 0x2
#define IMX7ULP_PAD_PTF5__LPSPI2_PCS2                                0x0194 0x02a4 0x3 0x3
#define IMX7ULP_PAD_PTF5__LPUART5_RTS_B                              0x0194 0x0000 0x4 0x0
#define IMX7ULP_PAD_PTF5__LPI2C5_SDA                                 0x0194 0x02c0 0x5 0x3
#define IMX7ULP_PAD_PTF5__TPM4_CH4                                   0x0194 0x0290 0x6 0x2
#define IMX7ULP_PAD_PTF5__FB_AD18                                    0x0194 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF6__PTF6                                       0x0198 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF6__VIU_D2                                     0x0198 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF6__FXIO1_D2                                   0x0198 0x020c 0x2 0x2
#define IMX7ULP_PAD_PTF6__LPSPI2_PCS3                                0x0198 0x02a8 0x3 0x3
#define IMX7ULP_PAD_PTF6__LPUART5_TX                                 0x0198 0x0258 0x4 0x3
#define IMX7ULP_PAD_PTF6__LPI2C5_HREQ                                0x0198 0x02b8 0x5 0x3
#define IMX7ULP_PAD_PTF6__TPM4_CH5                                   0x0198 0x0294 0x6 0x2
#define IMX7ULP_PAD_PTF6__FB_AD19                                    0x0198 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF7__PTF7                                       0x019c 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF7__VIU_D3                                     0x019c 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF7__FXIO1_D3                                   0x019c 0x0210 0x2 0x2
#define IMX7ULP_PAD_PTF7__LPUART5_RX                                 0x019c 0x0254 0x4 0x3
#define IMX7ULP_PAD_PTF7__TPM5_CH1                                   0x019c 0x02c8 0x6 0x3
#define IMX7ULP_PAD_PTF7__FB_AD20                                    0x019c 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF8__PTF8                                       0x01a0 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF8__USB1_ULPI_CLK                              0x01a0 0x0000 0xb 0x0
#define IMX7ULP_PAD_PTF8__VIU_D4                                     0x01a0 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF8__FXIO1_D4                                   0x01a0 0x0214 0x2 0x2
#define IMX7ULP_PAD_PTF8__LPSPI2_SIN                                 0x01a0 0x02b0 0x3 0x3
#define IMX7ULP_PAD_PTF8__LPUART6_CTS_B                              0x01a0 0x025c 0x4 0x3
#define IMX7ULP_PAD_PTF8__LPI2C6_SCL                                 0x01a0 0x02fc 0x5 0x3
#define IMX7ULP_PAD_PTF8__TPM5_CLKIN                                 0x01a0 0x02cc 0x6 0x3
#define IMX7ULP_PAD_PTF8__FB_AD21                                    0x01a0 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF9__PTF9                                       0x01a4 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF9__USB1_ULPI_NXT                              0x01a4 0x0000 0xb 0x0
#define IMX7ULP_PAD_PTF9__VIU_D5                                     0x01a4 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF9__FXIO1_D5                                   0x01a4 0x0218 0x2 0x2
#define IMX7ULP_PAD_PTF9__LPSPI2_SOUT                                0x01a4 0x02b4 0x3 0x3
#define IMX7ULP_PAD_PTF9__LPUART6_RTS_B                              0x01a4 0x0000 0x4 0x0
#define IMX7ULP_PAD_PTF9__LPI2C6_SDA                                 0x01a4 0x0300 0x5 0x3
#define IMX7ULP_PAD_PTF9__TPM5_CH0                                   0x01a4 0x02c4 0x6 0x3
#define IMX7ULP_PAD_PTF9__FB_AD22                                    0x01a4 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF10__PTF10                                     0x01a8 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF10__USB1_ULPI_STP                             0x01a8 0x0000 0xb 0x0
#define IMX7ULP_PAD_PTF10__VIU_D6                                    0x01a8 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF10__FXIO1_D6                                  0x01a8 0x021c 0x2 0x2
#define IMX7ULP_PAD_PTF10__LPSPI2_SCK                                0x01a8 0x02ac 0x3 0x3
#define IMX7ULP_PAD_PTF10__LPUART6_TX                                0x01a8 0x0264 0x4 0x3
#define IMX7ULP_PAD_PTF10__LPI2C6_HREQ                               0x01a8 0x02f8 0x5 0x3
#define IMX7ULP_PAD_PTF10__TPM7_CH3                                  0x01a8 0x02e8 0x6 0x3
#define IMX7ULP_PAD_PTF10__FB_AD23                                   0x01a8 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF11__PTF11                                     0x01ac 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF11__USB1_ULPI_DIR                             0x01ac 0x0000 0xb 0x0
#define IMX7ULP_PAD_PTF11__VIU_D7                                    0x01ac 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF11__FXIO1_D7                                  0x01ac 0x0220 0x2 0x2
#define IMX7ULP_PAD_PTF11__LPSPI2_PCS0                               0x01ac 0x029c 0x3 0x3
#define IMX7ULP_PAD_PTF11__LPUART6_RX                                0x01ac 0x0260 0x4 0x3
#define IMX7ULP_PAD_PTF11__TPM7_CH4                                  0x01ac 0x02ec 0x6 0x3
#define IMX7ULP_PAD_PTF11__FB_CS4_B_FB_TSIZ0_FB_BE31_24_BLS7_0_B     0x01ac 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF12__PTF12                                     0x01b0 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF12__USB1_ULPI_DATA0                           0x01b0 0x0000 0xb 0x0
#define IMX7ULP_PAD_PTF12__VIU_D8                                    0x01b0 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF12__FXIO1_D8                                  0x01b0 0x0224 0x2 0x2
#define IMX7ULP_PAD_PTF12__LPSPI3_PCS1                               0x01b0 0x0314 0x3 0x3
#define IMX7ULP_PAD_PTF12__LPUART7_CTS_B                             0x01b0 0x0268 0x4 0x3
#define IMX7ULP_PAD_PTF12__LPI2C7_SCL                                0x01b0 0x0308 0x5 0x3
#define IMX7ULP_PAD_PTF12__TPM7_CH5                                  0x01b0 0x02f0 0x6 0x3
#define IMX7ULP_PAD_PTF12__FB_AD24                                   0x01b0 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF13__PTF13                                     0x01b4 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF13__USB1_ULPI_DATA1                           0x01b4 0x0000 0xb 0x0
#define IMX7ULP_PAD_PTF13__VIU_D9                                    0x01b4 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF13__FXIO1_D9                                  0x01b4 0x0228 0x2 0x2
#define IMX7ULP_PAD_PTF13__LPSPI3_PCS2                               0x01b4 0x0318 0x3 0x3
#define IMX7ULP_PAD_PTF13__LPUART7_RTS_B                             0x01b4 0x0000 0x4 0x0
#define IMX7ULP_PAD_PTF13__LPI2C7_SDA                                0x01b4 0x030c 0x5 0x3
#define IMX7ULP_PAD_PTF13__TPM7_CLKIN                                0x01b4 0x02f4 0x6 0x3
#define IMX7ULP_PAD_PTF13__FB_AD25                                   0x01b4 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF14__PTF14                                     0x01b8 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF14__USB1_ULPI_DATA2                           0x01b8 0x0000 0xb 0x0
#define IMX7ULP_PAD_PTF14__VIU_D10                                   0x01b8 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF14__FXIO1_D10                                 0x01b8 0x022c 0x2 0x2
#define IMX7ULP_PAD_PTF14__LPSPI3_PCS3                               0x01b8 0x031c 0x3 0x3
#define IMX7ULP_PAD_PTF14__LPUART7_TX                                0x01b8 0x0270 0x4 0x3
#define IMX7ULP_PAD_PTF14__LPI2C7_HREQ                               0x01b8 0x0304 0x5 0x3
#define IMX7ULP_PAD_PTF14__TPM7_CH0                                  0x01b8 0x02dc 0x6 0x3
#define IMX7ULP_PAD_PTF14__FB_AD26                                   0x01b8 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF15__PTF15                                     0x01bc 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF15__USB1_ULPI_DATA3                           0x01bc 0x0000 0xb 0x0
#define IMX7ULP_PAD_PTF15__VIU_D11                                   0x01bc 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF15__FXIO1_D11                                 0x01bc 0x0230 0x2 0x2
#define IMX7ULP_PAD_PTF15__LPUART7_RX                                0x01bc 0x026c 0x4 0x3
#define IMX7ULP_PAD_PTF15__TPM7_CH1                                  0x01bc 0x02e0 0x6 0x3
#define IMX7ULP_PAD_PTF15__FB_AD27                                   0x01bc 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF16__PTF16                                     0x01c0 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF16__USB1_ULPI_DATA4                           0x01c0 0x0000 0xb 0x0
#define IMX7ULP_PAD_PTF16__VIU_D12                                   0x01c0 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF16__FXIO1_D12                                 0x01c0 0x0234 0x2 0x2
#define IMX7ULP_PAD_PTF16__LPSPI3_SIN                                0x01c0 0x0324 0x3 0x3
#define IMX7ULP_PAD_PTF16__TPM7_CH2                                  0x01c0 0x02e4 0x6 0x3
#define IMX7ULP_PAD_PTF16__FB_AD28                                   0x01c0 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF17__PTF17                                     0x01c4 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF17__USB1_ULPI_DATA5                           0x01c4 0x0000 0xb 0x0
#define IMX7ULP_PAD_PTF17__VIU_D13                                   0x01c4 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF17__FXIO1_D13                                 0x01c4 0x0238 0x2 0x2
#define IMX7ULP_PAD_PTF17__LPSPI3_SOUT                               0x01c4 0x0328 0x3 0x3
#define IMX7ULP_PAD_PTF17__TPM6_CLKIN                                0x01c4 0x02d8 0x6 0x3
#define IMX7ULP_PAD_PTF17__FB_AD29                                   0x01c4 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF18__PTF18                                     0x01c8 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF18__USB1_ULPI_DATA6                           0x01c8 0x0000 0xb 0x0
#define IMX7ULP_PAD_PTF18__VIU_D14                                   0x01c8 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF18__FXIO1_D14                                 0x01c8 0x023c 0x2 0x2
#define IMX7ULP_PAD_PTF18__LPSPI3_SCK                                0x01c8 0x0320 0x3 0x3
#define IMX7ULP_PAD_PTF18__TPM6_CH0                                  0x01c8 0x02d0 0x6 0x3
#define IMX7ULP_PAD_PTF18__FB_AD30                                   0x01c8 0x0000 0x9 0x0
#define IMX7ULP_PAD_PTF19__PTF19                                     0x01cc 0x0000 0x1 0x0
#define IMX7ULP_PAD_PTF19__USB1_ULPI_DATA7                           0x01cc 0x0000 0xb 0x0
#define IMX7ULP_PAD_PTF19__VIU_D15                                   0x01cc 0x0000 0xc 0x0
#define IMX7ULP_PAD_PTF19__FXIO1_D15                                 0x01cc 0x0240 0x2 0x2
#define IMX7ULP_PAD_PTF19__LPSPI3_PCS0                               0x01cc 0x0310 0x3 0x3
#define IMX7ULP_PAD_PTF19__TPM6_CH1                                  0x01cc 0x02d4 0x6 0x3
#define IMX7ULP_PAD_PTF19__FB_AD31                                   0x01cc 0x0000 0x9 0x0

#endif /* __DTS_IMX7ULP_PINFUNC_H */
