<<<<<<< HEAD
/*
 * Copyright 2014 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2014 Freescale Semiconductor, Inc.
>>>>>>> upstream/android-13
 */

#ifndef __DTS_IMX6SX_PINFUNC_H
#define __DTS_IMX6SX_PINFUNC_H

/*
 * The pin function ID is a tuple of
 * <mux_reg conf_reg input_reg mux_mode input_val>
 */
#define MX6SX_PAD_GPIO1_IO00__I2C1_SCL                            0x0014 0x035C 0x07A8 0x0 0x1
#define MX6SX_PAD_GPIO1_IO00__USDHC1_VSELECT                      0x0014 0x035C 0x0000 0x1 0x0
#define MX6SX_PAD_GPIO1_IO00__SPDIF_LOCK                          0x0014 0x035C 0x0000 0x2 0x0
#define MX6SX_PAD_GPIO1_IO00__CCM_WAIT                            0x0014 0x035C 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO00__WDOG1_WDOG_ANY                      0x0014 0x035C 0x0000 0x4 0x0
#define MX6SX_PAD_GPIO1_IO00__GPIO1_IO_0                          0x0014 0x035C 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO00__SNVS_HP_WRAPPER_VIO_5               0x0014 0x035C 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO00__PHY_DTB_1                           0x0014 0x035C 0x0000 0x7 0x0
#define MX6SX_PAD_GPIO1_IO01__I2C1_SDA                            0x0018 0x0360 0x07AC 0x0 0x1
#define MX6SX_PAD_GPIO1_IO01__USDHC1_RESET_B                      0x0018 0x0360 0x0000 0x1 0x0
#define MX6SX_PAD_GPIO1_IO01__SPDIF_SR_CLK                        0x0018 0x0360 0x0000 0x2 0x0
#define MX6SX_PAD_GPIO1_IO01__CCM_STOP                            0x0018 0x0360 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO01__WDOG3_WDOG_B                        0x0018 0x0360 0x0000 0x4 0x0
#define MX6SX_PAD_GPIO1_IO01__GPIO1_IO_1                          0x0018 0x0360 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO01__SNVS_HP_WRAPPER_VIO_5_CTL           0x0018 0x0360 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO01__PHY_DTB_0                           0x0018 0x0360 0x0000 0x7 0x0
#define MX6SX_PAD_GPIO1_IO02__I2C2_SCL                            0x001C 0x0364 0x07B0 0x0 0x1
#define MX6SX_PAD_GPIO1_IO02__USDHC1_CD_B                         0x001C 0x0364 0x0864 0x1 0x1
#define MX6SX_PAD_GPIO1_IO02__CSI2_MCLK                           0x001C 0x0364 0x0000 0x2 0x0
#define MX6SX_PAD_GPIO1_IO02__CCM_DI0_EXT_CLK                     0x001C 0x0364 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO02__WDOG1_WDOG_B                        0x001C 0x0364 0x0000 0x4 0x0
#define MX6SX_PAD_GPIO1_IO02__GPIO1_IO_2                          0x001C 0x0364 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO02__CCM_REF_EN_B                        0x001C 0x0364 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO02__PHY_TDI                             0x001C 0x0364 0x0000 0x7 0x0
#define MX6SX_PAD_GPIO1_IO03__I2C2_SDA                            0x0020 0x0368 0x07B4 0x0 0x1
#define MX6SX_PAD_GPIO1_IO03__USDHC1_WP                           0x0020 0x0368 0x0868 0x1 0x1
#define MX6SX_PAD_GPIO1_IO03__ENET1_REF_CLK_25M                   0x0020 0x0368 0x0000 0x2 0x0
#define MX6SX_PAD_GPIO1_IO03__CCM_DI1_EXT_CLK                     0x0020 0x0368 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO03__WDOG2_WDOG_B                        0x0020 0x0368 0x0000 0x4 0x0
#define MX6SX_PAD_GPIO1_IO03__GPIO1_IO_3                          0x0020 0x0368 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO03__CCM_PLL3_BYP                        0x0020 0x0368 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO03__PHY_TCK                             0x0020 0x0368 0x0000 0x7 0x0
<<<<<<< HEAD
#define MX6SX_PAD_GPIO1_IO04__UART1_RX                            0x0024 0x036C 0x0830 0x0 0x0
#define MX6SX_PAD_GPIO1_IO04__UART1_TX                            0x0024 0x036C 0x0000 0x0 0x0
=======
#define MX6SX_PAD_GPIO1_IO04__UART1_DCE_TX                        0x0024 0x036C 0x0000 0x0 0x0
#define MX6SX_PAD_GPIO1_IO04__UART1_DTE_RX                        0x0024 0x036C 0x0830 0x0 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_GPIO1_IO04__USDHC2_RESET_B                      0x0024 0x036C 0x0000 0x1 0x0
#define MX6SX_PAD_GPIO1_IO04__ENET1_MDC                           0x0024 0x036C 0x0000 0x2 0x0
#define MX6SX_PAD_GPIO1_IO04__OSC32K_32K_OUT                      0x0024 0x036C 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO04__ENET2_REF_CLK2                      0x0024 0x036C 0x076C 0x4 0x0
#define MX6SX_PAD_GPIO1_IO04__GPIO1_IO_4                          0x0024 0x036C 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO04__CCM_PLL2_BYP                        0x0024 0x036C 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO04__PHY_TMS                             0x0024 0x036C 0x0000 0x7 0x0
<<<<<<< HEAD
#define MX6SX_PAD_GPIO1_IO05__UART1_RX                            0x0028 0x0370 0x0830 0x0 0x1
#define MX6SX_PAD_GPIO1_IO05__UART1_TX                            0x0028 0x0370 0x0000 0x0 0x0
=======
#define MX6SX_PAD_GPIO1_IO05__UART1_DCE_RX                        0x0028 0x0370 0x0830 0x0 0x1
#define MX6SX_PAD_GPIO1_IO05__UART1_DTE_TX                        0x0028 0x0370 0x0000 0x0 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_GPIO1_IO05__USDHC2_VSELECT                      0x0028 0x0370 0x0000 0x1 0x0
#define MX6SX_PAD_GPIO1_IO05__ENET1_MDIO                          0x0028 0x0370 0x0764 0x2 0x0
#define MX6SX_PAD_GPIO1_IO05__ASRC_ASRC_EXT_CLK                   0x0028 0x0370 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO05__ENET1_REF_CLK1                      0x0028 0x0370 0x0760 0x4 0x0
#define MX6SX_PAD_GPIO1_IO05__GPIO1_IO_5                          0x0028 0x0370 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO05__SRC_TESTER_ACK                      0x0028 0x0370 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO05__PHY_TDO                             0x0028 0x0370 0x0000 0x7 0x0
<<<<<<< HEAD
#define MX6SX_PAD_GPIO1_IO06__UART2_RX                            0x002C 0x0374 0x0838 0x0 0x0
#define MX6SX_PAD_GPIO1_IO06__UART2_TX                            0x002C 0x0374 0x0000 0x0 0x0
#define MX6SX_PAD_GPIO1_IO06__USDHC2_CD_B                         0x002C 0x0374 0x086C 0x1 0x1
#define MX6SX_PAD_GPIO1_IO06__ENET2_MDC                           0x002C 0x0374 0x0000 0x2 0x0
#define MX6SX_PAD_GPIO1_IO06__CSI1_MCLK                           0x002C 0x0374 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO06__UART1_RTS_B                         0x002C 0x0374 0x082C 0x4 0x0
#define MX6SX_PAD_GPIO1_IO06__GPIO1_IO_6                          0x002C 0x0374 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO06__SRC_ANY_PU_RESET                    0x002C 0x0374 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO06__OCOTP_CTRL_WRAPPER_FUSE_LATCHED     0x002C 0x0374 0x0000 0x7 0x0
#define MX6SX_PAD_GPIO1_IO07__UART2_RX                            0x0030 0x0378 0x0838 0x0 0x1
#define MX6SX_PAD_GPIO1_IO07__UART2_TX                            0x0030 0x0378 0x0000 0x0 0x0
#define MX6SX_PAD_GPIO1_IO07__USDHC2_WP                           0x0030 0x0378 0x0870 0x1 0x1
#define MX6SX_PAD_GPIO1_IO07__ENET2_MDIO                          0x0030 0x0378 0x0770 0x2 0x0
#define MX6SX_PAD_GPIO1_IO07__AUDMUX_MCLK                         0x0030 0x0378 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO07__UART1_CTS_B                         0x0030 0x0378 0x0000 0x4 0x0
=======
#define MX6SX_PAD_GPIO1_IO06__UART2_DCE_TX                        0x002C 0x0374 0x0000 0x0 0x0
#define MX6SX_PAD_GPIO1_IO06__UART2_DTE_RX                        0x002C 0x0374 0x0838 0x0 0x0
#define MX6SX_PAD_GPIO1_IO06__USDHC2_CD_B                         0x002C 0x0374 0x086C 0x1 0x1
#define MX6SX_PAD_GPIO1_IO06__ENET2_MDC                           0x002C 0x0374 0x0000 0x2 0x0
#define MX6SX_PAD_GPIO1_IO06__CSI1_MCLK                           0x002C 0x0374 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO06__UART1_DCE_RTS                       0x002C 0x0374 0x082C 0x4 0x0
#define MX6SX_PAD_GPIO1_IO06__UART1_DTE_CTS                       0x002C 0x0374 0x0000 0x4 0x0
#define MX6SX_PAD_GPIO1_IO06__GPIO1_IO_6                          0x002C 0x0374 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO06__SRC_ANY_PU_RESET                    0x002C 0x0374 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO06__OCOTP_CTRL_WRAPPER_FUSE_LATCHED     0x002C 0x0374 0x0000 0x7 0x0
#define MX6SX_PAD_GPIO1_IO07__UART2_DCE_RX                        0x0030 0x0378 0x0838 0x0 0x1
#define MX6SX_PAD_GPIO1_IO07__UART2_DTE_TX                        0x0030 0x0378 0x0000 0x0 0x0
#define MX6SX_PAD_GPIO1_IO07__USDHC2_WP                           0x0030 0x0378 0x0870 0x1 0x1
#define MX6SX_PAD_GPIO1_IO07__ENET2_MDIO                          0x0030 0x0378 0x0770 0x2 0x0
#define MX6SX_PAD_GPIO1_IO07__AUDMUX_MCLK                         0x0030 0x0378 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO07__UART1_DCE_CTS                       0x0030 0x0378 0x0000 0x4 0x0
#define MX6SX_PAD_GPIO1_IO07__UART1_DTE_RTS                       0x0030 0x0378 0x082C 0x4 0x1
>>>>>>> upstream/android-13
#define MX6SX_PAD_GPIO1_IO07__GPIO1_IO_7                          0x0030 0x0378 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO07__SRC_EARLY_RESET                     0x0030 0x0378 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO07__DCIC2_OUT                           0x0030 0x0378 0x0000 0x7 0x0
#define MX6SX_PAD_GPIO1_IO07__VDEC_DEBUG_44                       0x0030 0x0378 0x0000 0x8 0x0
#define MX6SX_PAD_GPIO1_IO08__USB_OTG1_OC                         0x0034 0x037C 0x0860 0x0 0x0
#define MX6SX_PAD_GPIO1_IO08__WDOG1_WDOG_B                        0x0034 0x037C 0x0000 0x1 0x0
#define MX6SX_PAD_GPIO1_IO08__SDMA_EXT_EVENT_0                    0x0034 0x037C 0x081C 0x2 0x0
#define MX6SX_PAD_GPIO1_IO08__CCM_PMIC_RDY                        0x0034 0x037C 0x069C 0x3 0x1
<<<<<<< HEAD
#define MX6SX_PAD_GPIO1_IO08__UART2_RTS_B                         0x0034 0x037C 0x0834 0x4 0x0
=======
#define MX6SX_PAD_GPIO1_IO08__UART2_DCE_RTS                       0x0034 0x037C 0x0834 0x4 0x0
#define MX6SX_PAD_GPIO1_IO08__UART2_DTE_CTS                       0x0034 0x037C 0x0000 0x4 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_GPIO1_IO08__GPIO1_IO_8                          0x0034 0x037C 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO08__SRC_SYSTEM_RESET                    0x0034 0x037C 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO08__DCIC1_OUT                           0x0034 0x037C 0x0000 0x7 0x0
#define MX6SX_PAD_GPIO1_IO08__VDEC_DEBUG_43                       0x0034 0x037C 0x0000 0x8 0x0
#define MX6SX_PAD_GPIO1_IO09__USB_OTG1_PWR                        0x0038 0x0380 0x0000 0x0 0x0
#define MX6SX_PAD_GPIO1_IO09__WDOG2_WDOG_B                        0x0038 0x0380 0x0000 0x1 0x0
#define MX6SX_PAD_GPIO1_IO09__SDMA_EXT_EVENT_1                    0x0038 0x0380 0x0820 0x2 0x0
#define MX6SX_PAD_GPIO1_IO09__CCM_OUT0                            0x0038 0x0380 0x0000 0x3 0x0
<<<<<<< HEAD
#define MX6SX_PAD_GPIO1_IO09__UART2_CTS_B                         0x0038 0x0380 0x0000 0x4 0x0
=======
#define MX6SX_PAD_GPIO1_IO09__UART2_DCE_CTS                       0x0038 0x0380 0x0000 0x4 0x0
#define MX6SX_PAD_GPIO1_IO09__UART2_DTE_RTS                       0x0038 0x0380 0x0834 0x4 0x1
>>>>>>> upstream/android-13
#define MX6SX_PAD_GPIO1_IO09__GPIO1_IO_9                          0x0038 0x0380 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO09__SRC_INT_BOOT                        0x0038 0x0380 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO09__OBSERVE_MUX_OUT_4                   0x0038 0x0380 0x0000 0x7 0x0
#define MX6SX_PAD_GPIO1_IO09__VDEC_DEBUG_42                       0x0038 0x0380 0x0000 0x8 0x0
#define MX6SX_PAD_GPIO1_IO10__ANATOP_OTG1_ID                      0x003C 0x0384 0x0624 0x0 0x0
#define MX6SX_PAD_GPIO1_IO10__SPDIF_EXT_CLK                       0x003C 0x0384 0x0828 0x1 0x0
#define MX6SX_PAD_GPIO1_IO10__PWM1_OUT                            0x003C 0x0384 0x0000 0x2 0x0
#define MX6SX_PAD_GPIO1_IO10__CCM_OUT1                            0x003C 0x0384 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO10__CSI1_FIELD                          0x003C 0x0384 0x070C 0x4 0x1
#define MX6SX_PAD_GPIO1_IO10__GPIO1_IO_10                         0x003C 0x0384 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO10__CSU_CSU_INT_DEB                     0x003C 0x0384 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO10__OBSERVE_MUX_OUT_3                   0x003C 0x0384 0x0000 0x7 0x0
#define MX6SX_PAD_GPIO1_IO10__VDEC_DEBUG_41                       0x003C 0x0384 0x0000 0x8 0x0
#define MX6SX_PAD_GPIO1_IO11__USB_OTG2_OC                         0x0040 0x0388 0x085C 0x0 0x0
#define MX6SX_PAD_GPIO1_IO11__SPDIF_IN                            0x0040 0x0388 0x0824 0x1 0x2
#define MX6SX_PAD_GPIO1_IO11__PWM2_OUT                            0x0040 0x0388 0x0000 0x2 0x0
#define MX6SX_PAD_GPIO1_IO11__CCM_CLKO1                           0x0040 0x0388 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO11__MLB_DATA                            0x0040 0x0388 0x07EC 0x4 0x0
#define MX6SX_PAD_GPIO1_IO11__GPIO1_IO_11                         0x0040 0x0388 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO11__CSU_CSU_ALARM_AUT_0                 0x0040 0x0388 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO11__OBSERVE_MUX_OUT_2                   0x0040 0x0388 0x0000 0x7 0x0
#define MX6SX_PAD_GPIO1_IO11__VDEC_DEBUG_40                       0x0040 0x0388 0x0000 0x8 0x0
#define MX6SX_PAD_GPIO1_IO12__USB_OTG2_PWR                        0x0044 0x038C 0x0000 0x0 0x0
#define MX6SX_PAD_GPIO1_IO12__SPDIF_OUT                           0x0044 0x038C 0x0000 0x1 0x0
#define MX6SX_PAD_GPIO1_IO12__PWM3_OUT                            0x0044 0x038C 0x0000 0x2 0x0
#define MX6SX_PAD_GPIO1_IO12__CCM_CLKO2                           0x0044 0x038C 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO12__MLB_CLK                             0x0044 0x038C 0x07E8 0x4 0x0
#define MX6SX_PAD_GPIO1_IO12__GPIO1_IO_12                         0x0044 0x038C 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO12__CSU_CSU_ALARM_AUT_1                 0x0044 0x038C 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO12__OBSERVE_MUX_OUT_1                   0x0044 0x038C 0x0000 0x7 0x0
#define MX6SX_PAD_GPIO1_IO12__VDEC_DEBUG_39                       0x0044 0x038C 0x0000 0x8 0x0
#define MX6SX_PAD_GPIO1_IO13__WDOG1_WDOG_ANY                      0x0048 0x0390 0x0000 0x0 0x0
#define MX6SX_PAD_GPIO1_IO13__ANATOP_OTG2_ID                      0x0048 0x0390 0x0628 0x1 0x0
#define MX6SX_PAD_GPIO1_IO13__PWM4_OUT                            0x0048 0x0390 0x0000 0x2 0x0
#define MX6SX_PAD_GPIO1_IO13__CCM_OUT2                            0x0048 0x0390 0x0000 0x3 0x0
#define MX6SX_PAD_GPIO1_IO13__MLB_SIG                             0x0048 0x0390 0x07F0 0x4 0x0
#define MX6SX_PAD_GPIO1_IO13__GPIO1_IO_13                         0x0048 0x0390 0x0000 0x5 0x0
#define MX6SX_PAD_GPIO1_IO13__CSU_CSU_ALARM_AUT_2                 0x0048 0x0390 0x0000 0x6 0x0
#define MX6SX_PAD_GPIO1_IO13__OBSERVE_MUX_OUT_0                   0x0048 0x0390 0x0000 0x7 0x0
#define MX6SX_PAD_GPIO1_IO13__VDEC_DEBUG_38                       0x0048 0x0390 0x0000 0x8 0x0
#define MX6SX_PAD_CSI_DATA00__CSI1_DATA_2                         0x004C 0x0394 0x06A8 0x0 0x0
#define MX6SX_PAD_CSI_DATA00__ESAI_TX_CLK                         0x004C 0x0394 0x078C 0x1 0x1
#define MX6SX_PAD_CSI_DATA00__AUDMUX_AUD6_TXC                     0x004C 0x0394 0x0684 0x2 0x1
#define MX6SX_PAD_CSI_DATA00__I2C1_SCL                            0x004C 0x0394 0x07A8 0x3 0x0
#define MX6SX_PAD_CSI_DATA00__UART6_RI_B                          0x004C 0x0394 0x0000 0x4 0x0
#define MX6SX_PAD_CSI_DATA00__GPIO1_IO_14                         0x004C 0x0394 0x0000 0x5 0x0
#define MX6SX_PAD_CSI_DATA00__WEIM_DATA_23                        0x004C 0x0394 0x0000 0x6 0x0
#define MX6SX_PAD_CSI_DATA00__SAI1_TX_BCLK                        0x004C 0x0394 0x0800 0x7 0x0
#define MX6SX_PAD_CSI_DATA00__VADC_DATA_4                         0x004C 0x0394 0x0000 0x8 0x0
#define MX6SX_PAD_CSI_DATA00__MMDC_DEBUG_37                       0x004C 0x0394 0x0000 0x9 0x0
#define MX6SX_PAD_CSI_DATA01__CSI1_DATA_3                         0x0050 0x0398 0x06AC 0x0 0x0
#define MX6SX_PAD_CSI_DATA01__ESAI_TX_FS                          0x0050 0x0398 0x077C 0x1 0x1
#define MX6SX_PAD_CSI_DATA01__AUDMUX_AUD6_TXFS                    0x0050 0x0398 0x0688 0x2 0x1
#define MX6SX_PAD_CSI_DATA01__I2C1_SDA                            0x0050 0x0398 0x07AC 0x3 0x0
#define MX6SX_PAD_CSI_DATA01__UART6_DSR_B                         0x0050 0x0398 0x0000 0x4 0x0
#define MX6SX_PAD_CSI_DATA01__GPIO1_IO_15                         0x0050 0x0398 0x0000 0x5 0x0
#define MX6SX_PAD_CSI_DATA01__WEIM_DATA_22                        0x0050 0x0398 0x0000 0x6 0x0
#define MX6SX_PAD_CSI_DATA01__SAI1_TX_SYNC                        0x0050 0x0398 0x0804 0x7 0x0
#define MX6SX_PAD_CSI_DATA01__VADC_DATA_5                         0x0050 0x0398 0x0000 0x8 0x0
#define MX6SX_PAD_CSI_DATA01__MMDC_DEBUG_38                       0x0050 0x0398 0x0000 0x9 0x0
#define MX6SX_PAD_CSI_DATA02__CSI1_DATA_4                         0x0054 0x039C 0x06B0 0x0 0x0
#define MX6SX_PAD_CSI_DATA02__ESAI_RX_CLK                         0x0054 0x039C 0x0788 0x1 0x1
#define MX6SX_PAD_CSI_DATA02__AUDMUX_AUD6_RXC                     0x0054 0x039C 0x067C 0x2 0x1
#define MX6SX_PAD_CSI_DATA02__KPP_COL_5                           0x0054 0x039C 0x07C8 0x3 0x0
#define MX6SX_PAD_CSI_DATA02__UART6_DTR_B                         0x0054 0x039C 0x0000 0x4 0x0
#define MX6SX_PAD_CSI_DATA02__GPIO1_IO_16                         0x0054 0x039C 0x0000 0x5 0x0
#define MX6SX_PAD_CSI_DATA02__WEIM_DATA_21                        0x0054 0x039C 0x0000 0x6 0x0
#define MX6SX_PAD_CSI_DATA02__SAI1_RX_BCLK                        0x0054 0x039C 0x07F4 0x7 0x0
#define MX6SX_PAD_CSI_DATA02__VADC_DATA_6                         0x0054 0x039C 0x0000 0x8 0x0
#define MX6SX_PAD_CSI_DATA02__MMDC_DEBUG_39                       0x0054 0x039C 0x0000 0x9 0x0
#define MX6SX_PAD_CSI_DATA03__CSI1_DATA_5                         0x0058 0x03A0 0x06B4 0x0 0x0
#define MX6SX_PAD_CSI_DATA03__ESAI_RX_FS                          0x0058 0x03A0 0x0778 0x1 0x1
#define MX6SX_PAD_CSI_DATA03__AUDMUX_AUD6_RXFS                    0x0058 0x03A0 0x0680 0x2 0x1
#define MX6SX_PAD_CSI_DATA03__KPP_ROW_5                           0x0058 0x03A0 0x07D4 0x3 0x0
#define MX6SX_PAD_CSI_DATA03__UART6_DCD_B                         0x0058 0x03A0 0x0000 0x4 0x0
#define MX6SX_PAD_CSI_DATA03__GPIO1_IO_17                         0x0058 0x03A0 0x0000 0x5 0x0
#define MX6SX_PAD_CSI_DATA03__WEIM_DATA_20                        0x0058 0x03A0 0x0000 0x6 0x0
#define MX6SX_PAD_CSI_DATA03__SAI1_RX_SYNC                        0x0058 0x03A0 0x07FC 0x7 0x0
#define MX6SX_PAD_CSI_DATA03__VADC_DATA_7                         0x0058 0x03A0 0x0000 0x8 0x0
#define MX6SX_PAD_CSI_DATA03__MMDC_DEBUG_40                       0x0058 0x03A0 0x0000 0x9 0x0
#define MX6SX_PAD_CSI_DATA04__CSI1_DATA_6                         0x005C 0x03A4 0x06B8 0x0 0x0
#define MX6SX_PAD_CSI_DATA04__ESAI_TX1                            0x005C 0x03A4 0x0794 0x1 0x1
#define MX6SX_PAD_CSI_DATA04__SPDIF_OUT                           0x005C 0x03A4 0x0000 0x2 0x0
#define MX6SX_PAD_CSI_DATA04__KPP_COL_6                           0x005C 0x03A4 0x07CC 0x3 0x0
<<<<<<< HEAD
#define MX6SX_PAD_CSI_DATA04__UART6_RX                            0x005C 0x03A4 0x0858 0x4 0x0
#define MX6SX_PAD_CSI_DATA04__UART6_TX                            0x005C 0x03A4 0x0000 0x4 0x0
=======
#define MX6SX_PAD_CSI_DATA04__UART6_DCE_RX                        0x005C 0x03A4 0x0858 0x4 0x0
#define MX6SX_PAD_CSI_DATA04__UART6_DTE_TX                        0x005C 0x03A4 0x0000 0x4 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_CSI_DATA04__GPIO1_IO_18                         0x005C 0x03A4 0x0000 0x5 0x0
#define MX6SX_PAD_CSI_DATA04__WEIM_DATA_19                        0x005C 0x03A4 0x0000 0x6 0x0
#define MX6SX_PAD_CSI_DATA04__PWM5_OUT                            0x005C 0x03A4 0x0000 0x7 0x0
#define MX6SX_PAD_CSI_DATA04__VADC_DATA_8                         0x005C 0x03A4 0x0000 0x8 0x0
#define MX6SX_PAD_CSI_DATA04__MMDC_DEBUG_41                       0x005C 0x03A4 0x0000 0x9 0x0
#define MX6SX_PAD_CSI_DATA05__CSI1_DATA_7                         0x0060 0x03A8 0x06BC 0x0 0x0
#define MX6SX_PAD_CSI_DATA05__ESAI_TX4_RX1                        0x0060 0x03A8 0x07A0 0x1 0x1
#define MX6SX_PAD_CSI_DATA05__SPDIF_IN                            0x0060 0x03A8 0x0824 0x2 0x1
#define MX6SX_PAD_CSI_DATA05__KPP_ROW_6                           0x0060 0x03A8 0x07D8 0x3 0x0
<<<<<<< HEAD
#define MX6SX_PAD_CSI_DATA05__UART6_RX                            0x0060 0x03A8 0x0858 0x4 0x1
#define MX6SX_PAD_CSI_DATA05__UART6_TX                            0x0060 0x03A8 0x0000 0x4 0x0
=======
#define MX6SX_PAD_CSI_DATA05__UART6_DCE_TX                        0x0060 0x03A8 0x0000 0x4 0x0
#define MX6SX_PAD_CSI_DATA05__UART6_DTE_RX                        0x0060 0x03A8 0x0858 0x4 0x1
>>>>>>> upstream/android-13
#define MX6SX_PAD_CSI_DATA05__GPIO1_IO_19                         0x0060 0x03A8 0x0000 0x5 0x0
#define MX6SX_PAD_CSI_DATA05__WEIM_DATA_18                        0x0060 0x03A8 0x0000 0x6 0x0
#define MX6SX_PAD_CSI_DATA05__PWM6_OUT                            0x0060 0x03A8 0x0000 0x7 0x0
#define MX6SX_PAD_CSI_DATA05__VADC_DATA_9                         0x0060 0x03A8 0x0000 0x8 0x0
#define MX6SX_PAD_CSI_DATA05__MMDC_DEBUG_42                       0x0060 0x03A8 0x0000 0x9 0x0
#define MX6SX_PAD_CSI_DATA06__CSI1_DATA_8                         0x0064 0x03AC 0x06C0 0x0 0x0
#define MX6SX_PAD_CSI_DATA06__ESAI_TX2_RX3                        0x0064 0x03AC 0x0798 0x1 0x1
#define MX6SX_PAD_CSI_DATA06__I2C4_SCL                            0x0064 0x03AC 0x07C0 0x2 0x2
#define MX6SX_PAD_CSI_DATA06__KPP_COL_7                           0x0064 0x03AC 0x07D0 0x3 0x0
<<<<<<< HEAD
#define MX6SX_PAD_CSI_DATA06__UART6_RTS_B                         0x0064 0x03AC 0x0854 0x4 0x0
=======
#define MX6SX_PAD_CSI_DATA06__UART6_DCE_RTS                       0x0064 0x03AC 0x0854 0x4 0x0
#define MX6SX_PAD_CSI_DATA06__UART6_DTE_CTS                       0x0064 0x03AC 0x0000 0x4 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_CSI_DATA06__GPIO1_IO_20                         0x0064 0x03AC 0x0000 0x5 0x0
#define MX6SX_PAD_CSI_DATA06__WEIM_DATA_17                        0x0064 0x03AC 0x0000 0x6 0x0
#define MX6SX_PAD_CSI_DATA06__DCIC2_OUT                           0x0064 0x03AC 0x0000 0x7 0x0
#define MX6SX_PAD_CSI_DATA06__VADC_DATA_10                        0x0064 0x03AC 0x0000 0x8 0x0
#define MX6SX_PAD_CSI_DATA06__MMDC_DEBUG_43                       0x0064 0x03AC 0x0000 0x9 0x0
#define MX6SX_PAD_CSI_DATA07__CSI1_DATA_9                         0x0068 0x03B0 0x06C4 0x0 0x0
#define MX6SX_PAD_CSI_DATA07__ESAI_TX3_RX2                        0x0068 0x03B0 0x079C 0x1 0x1
#define MX6SX_PAD_CSI_DATA07__I2C4_SDA                            0x0068 0x03B0 0x07C4 0x2 0x2
#define MX6SX_PAD_CSI_DATA07__KPP_ROW_7                           0x0068 0x03B0 0x07DC 0x3 0x0
<<<<<<< HEAD
#define MX6SX_PAD_CSI_DATA07__UART6_CTS_B                         0x0068 0x03B0 0x0000 0x4 0x0
=======
#define MX6SX_PAD_CSI_DATA07__UART6_DCE_CTS                       0x0068 0x03B0 0x0000 0x4 0x0
#define MX6SX_PAD_CSI_DATA07__UART6_DTE_RTS                       0x0068 0x03B0 0x0854 0x4 0x1
>>>>>>> upstream/android-13
#define MX6SX_PAD_CSI_DATA07__GPIO1_IO_21                         0x0068 0x03B0 0x0000 0x5 0x0
#define MX6SX_PAD_CSI_DATA07__WEIM_DATA_16                        0x0068 0x03B0 0x0000 0x6 0x0
#define MX6SX_PAD_CSI_DATA07__DCIC1_OUT                           0x0068 0x03B0 0x0000 0x7 0x0
#define MX6SX_PAD_CSI_DATA07__VADC_DATA_11                        0x0068 0x03B0 0x0000 0x8 0x0
#define MX6SX_PAD_CSI_DATA07__MMDC_DEBUG_44                       0x0068 0x03B0 0x0000 0x9 0x0
#define MX6SX_PAD_CSI_HSYNC__CSI1_HSYNC                           0x006C 0x03B4 0x0700 0x0 0x0
#define MX6SX_PAD_CSI_HSYNC__ESAI_TX0                             0x006C 0x03B4 0x0790 0x1 0x1
#define MX6SX_PAD_CSI_HSYNC__AUDMUX_AUD6_TXD                      0x006C 0x03B4 0x0678 0x2 0x1
<<<<<<< HEAD
#define MX6SX_PAD_CSI_HSYNC__UART4_RTS_B                          0x006C 0x03B4 0x0844 0x3 0x2
=======
#define MX6SX_PAD_CSI_HSYNC__UART4_DCE_RTS                        0x006C 0x03B4 0x0844 0x3 0x2
#define MX6SX_PAD_CSI_HSYNC__UART4_DTE_CTS                        0x006C 0x03B4 0x0000 0x3 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_CSI_HSYNC__MQS_LEFT                             0x006C 0x03B4 0x0000 0x4 0x0
#define MX6SX_PAD_CSI_HSYNC__GPIO1_IO_22                          0x006C 0x03B4 0x0000 0x5 0x0
#define MX6SX_PAD_CSI_HSYNC__WEIM_DATA_25                         0x006C 0x03B4 0x0000 0x6 0x0
#define MX6SX_PAD_CSI_HSYNC__SAI1_TX_DATA_0                       0x006C 0x03B4 0x0000 0x7 0x0
#define MX6SX_PAD_CSI_HSYNC__VADC_DATA_2                          0x006C 0x03B4 0x0000 0x8 0x0
#define MX6SX_PAD_CSI_HSYNC__MMDC_DEBUG_35                        0x006C 0x03B4 0x0000 0x9 0x0
#define MX6SX_PAD_CSI_MCLK__CSI1_MCLK                             0x0070 0x03B8 0x0000 0x0 0x0
#define MX6SX_PAD_CSI_MCLK__ESAI_TX_HF_CLK                        0x0070 0x03B8 0x0784 0x1 0x1
#define MX6SX_PAD_CSI_MCLK__OSC32K_32K_OUT                        0x0070 0x03B8 0x0000 0x2 0x0
<<<<<<< HEAD
#define MX6SX_PAD_CSI_MCLK__UART4_RX                              0x0070 0x03B8 0x0848 0x3 0x2
#define MX6SX_PAD_CSI_MCLK__UART4_TX                              0x0070 0x03B8 0x0000 0x3 0x0
=======
#define MX6SX_PAD_CSI_MCLK__UART4_DCE_RX                          0x0070 0x03B8 0x0848 0x3 0x2
#define MX6SX_PAD_CSI_MCLK__UART4_DTE_TX                          0x0070 0x03B8 0x0000 0x3 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_CSI_MCLK__ANATOP_32K_OUT                        0x0070 0x03B8 0x0000 0x4 0x0
#define MX6SX_PAD_CSI_MCLK__GPIO1_IO_23                           0x0070 0x03B8 0x0000 0x5 0x0
#define MX6SX_PAD_CSI_MCLK__WEIM_DATA_26                          0x0070 0x03B8 0x0000 0x6 0x0
#define MX6SX_PAD_CSI_MCLK__CSI1_FIELD                            0x0070 0x03B8 0x070C 0x7 0x0
#define MX6SX_PAD_CSI_MCLK__VADC_DATA_1                           0x0070 0x03B8 0x0000 0x8 0x0
#define MX6SX_PAD_CSI_MCLK__MMDC_DEBUG_34                         0x0070 0x03B8 0x0000 0x9 0x0
#define MX6SX_PAD_CSI_PIXCLK__CSI1_PIXCLK                         0x0074 0x03BC 0x0704 0x0 0x0
#define MX6SX_PAD_CSI_PIXCLK__ESAI_RX_HF_CLK                      0x0074 0x03BC 0x0780 0x1 0x1
#define MX6SX_PAD_CSI_PIXCLK__AUDMUX_MCLK                         0x0074 0x03BC 0x0000 0x2 0x0
<<<<<<< HEAD
#define MX6SX_PAD_CSI_PIXCLK__UART4_RX                            0x0074 0x03BC 0x0848 0x3 0x3
#define MX6SX_PAD_CSI_PIXCLK__UART4_TX                            0x0074 0x03BC 0x0000 0x3 0x0
=======
#define MX6SX_PAD_CSI_PIXCLK__UART4_DCE_TX                        0x0074 0x03BC 0x0000 0x3 0x0
#define MX6SX_PAD_CSI_PIXCLK__UART4_DTE_RX                        0x0074 0x03BC 0x0848 0x3 0x3
>>>>>>> upstream/android-13
#define MX6SX_PAD_CSI_PIXCLK__ANATOP_24M_OUT                      0x0074 0x03BC 0x0000 0x4 0x0
#define MX6SX_PAD_CSI_PIXCLK__GPIO1_IO_24                         0x0074 0x03BC 0x0000 0x5 0x0
#define MX6SX_PAD_CSI_PIXCLK__WEIM_DATA_27                        0x0074 0x03BC 0x0000 0x6 0x0
#define MX6SX_PAD_CSI_PIXCLK__ESAI_TX_HF_CLK                      0x0074 0x03BC 0x0784 0x7 0x2
#define MX6SX_PAD_CSI_PIXCLK__VADC_CLK                            0x0074 0x03BC 0x0000 0x8 0x0
#define MX6SX_PAD_CSI_PIXCLK__MMDC_DEBUG_33                       0x0074 0x03BC 0x0000 0x9 0x0
#define MX6SX_PAD_CSI_VSYNC__CSI1_VSYNC                           0x0078 0x03C0 0x0708 0x0 0x0
#define MX6SX_PAD_CSI_VSYNC__ESAI_TX5_RX0                         0x0078 0x03C0 0x07A4 0x1 0x1
#define MX6SX_PAD_CSI_VSYNC__AUDMUX_AUD6_RXD                      0x0078 0x03C0 0x0674 0x2 0x1
<<<<<<< HEAD
#define MX6SX_PAD_CSI_VSYNC__UART4_CTS_B                          0x0078 0x03C0 0x0000 0x3 0x0
=======
#define MX6SX_PAD_CSI_VSYNC__UART4_DCE_CTS                        0x0078 0x03C0 0x0000 0x3 0x0
#define MX6SX_PAD_CSI_VSYNC__UART4_DTE_RTS                        0x0078 0x03C0 0x0844 0x3 0x3
>>>>>>> upstream/android-13
#define MX6SX_PAD_CSI_VSYNC__MQS_RIGHT                            0x0078 0x03C0 0x0000 0x4 0x0
#define MX6SX_PAD_CSI_VSYNC__GPIO1_IO_25                          0x0078 0x03C0 0x0000 0x5 0x0
#define MX6SX_PAD_CSI_VSYNC__WEIM_DATA_24                         0x0078 0x03C0 0x0000 0x6 0x0
#define MX6SX_PAD_CSI_VSYNC__SAI1_RX_DATA_0                       0x0078 0x03C0 0x07F8 0x7 0x0
#define MX6SX_PAD_CSI_VSYNC__VADC_DATA_3                          0x0078 0x03C0 0x0000 0x8 0x0
#define MX6SX_PAD_CSI_VSYNC__MMDC_DEBUG_36                        0x0078 0x03C0 0x0000 0x9 0x0
#define MX6SX_PAD_ENET1_COL__ENET1_COL                            0x007C 0x03C4 0x0000 0x0 0x0
#define MX6SX_PAD_ENET1_COL__ENET2_MDC                            0x007C 0x03C4 0x0000 0x1 0x0
#define MX6SX_PAD_ENET1_COL__AUDMUX_AUD4_TXC                      0x007C 0x03C4 0x0654 0x2 0x1
#define MX6SX_PAD_ENET1_COL__UART1_RI_B                           0x007C 0x03C4 0x0000 0x3 0x0
#define MX6SX_PAD_ENET1_COL__SPDIF_EXT_CLK                        0x007C 0x03C4 0x0828 0x4 0x1
#define MX6SX_PAD_ENET1_COL__GPIO2_IO_0                           0x007C 0x03C4 0x0000 0x5 0x0
#define MX6SX_PAD_ENET1_COL__CSI2_DATA_23                         0x007C 0x03C4 0x0000 0x6 0x0
#define MX6SX_PAD_ENET1_COL__LCDIF2_DATA_16                       0x007C 0x03C4 0x0000 0x7 0x0
#define MX6SX_PAD_ENET1_COL__VDEC_DEBUG_37                        0x007C 0x03C4 0x0000 0x8 0x0
#define MX6SX_PAD_ENET1_COL__PCIE_CTRL_DEBUG_31                   0x007C 0x03C4 0x0000 0x9 0x0
#define MX6SX_PAD_ENET1_CRS__ENET1_CRS                            0x0080 0x03C8 0x0000 0x0 0x0
#define MX6SX_PAD_ENET1_CRS__ENET2_MDIO                           0x0080 0x03C8 0x0770 0x1 0x1
#define MX6SX_PAD_ENET1_CRS__AUDMUX_AUD4_TXD                      0x0080 0x03C8 0x0648 0x2 0x1
#define MX6SX_PAD_ENET1_CRS__UART1_DCD_B                          0x0080 0x03C8 0x0000 0x3 0x0
#define MX6SX_PAD_ENET1_CRS__SPDIF_LOCK                           0x0080 0x03C8 0x0000 0x4 0x0
#define MX6SX_PAD_ENET1_CRS__GPIO2_IO_1                           0x0080 0x03C8 0x0000 0x5 0x0
#define MX6SX_PAD_ENET1_CRS__CSI2_DATA_22                         0x0080 0x03C8 0x0000 0x6 0x0
#define MX6SX_PAD_ENET1_CRS__LCDIF2_DATA_17                       0x0080 0x03C8 0x0000 0x7 0x0
#define MX6SX_PAD_ENET1_CRS__VDEC_DEBUG_36                        0x0080 0x03C8 0x0000 0x8 0x0
#define MX6SX_PAD_ENET1_CRS__PCIE_CTRL_DEBUG_30                   0x0080 0x03C8 0x0000 0x9 0x0
#define MX6SX_PAD_ENET1_MDC__ENET1_MDC                            0x0084 0x03CC 0x0000 0x0 0x0
#define MX6SX_PAD_ENET1_MDC__ENET2_MDC                            0x0084 0x03CC 0x0000 0x1 0x0
#define MX6SX_PAD_ENET1_MDC__AUDMUX_AUD3_RXFS                     0x0084 0x03CC 0x0638 0x2 0x1
#define MX6SX_PAD_ENET1_MDC__ANATOP_24M_OUT                       0x0084 0x03CC 0x0000 0x3 0x0
#define MX6SX_PAD_ENET1_MDC__EPIT2_OUT                            0x0084 0x03CC 0x0000 0x4 0x0
#define MX6SX_PAD_ENET1_MDC__GPIO2_IO_2                           0x0084 0x03CC 0x0000 0x5 0x0
#define MX6SX_PAD_ENET1_MDC__USB_OTG1_PWR                         0x0084 0x03CC 0x0000 0x6 0x0
#define MX6SX_PAD_ENET1_MDC__PWM7_OUT                             0x0084 0x03CC 0x0000 0x7 0x0
#define MX6SX_PAD_ENET1_MDIO__ENET1_MDIO                          0x0088 0x03D0 0x0764 0x0 0x1
#define MX6SX_PAD_ENET1_MDIO__ENET2_MDIO                          0x0088 0x03D0 0x0770 0x1 0x2
#define MX6SX_PAD_ENET1_MDIO__AUDMUX_MCLK                         0x0088 0x03D0 0x0000 0x2 0x0
#define MX6SX_PAD_ENET1_MDIO__OSC32K_32K_OUT                      0x0088 0x03D0 0x0000 0x3 0x0
#define MX6SX_PAD_ENET1_MDIO__EPIT1_OUT                           0x0088 0x03D0 0x0000 0x4 0x0
#define MX6SX_PAD_ENET1_MDIO__GPIO2_IO_3                          0x0088 0x03D0 0x0000 0x5 0x0
#define MX6SX_PAD_ENET1_MDIO__USB_OTG1_OC                         0x0088 0x03D0 0x0860 0x6 0x1
#define MX6SX_PAD_ENET1_MDIO__PWM8_OUT                            0x0088 0x03D0 0x0000 0x7 0x0
#define MX6SX_PAD_ENET1_RX_CLK__ENET1_RX_CLK                      0x008C 0x03D4 0x0768 0x0 0x0
#define MX6SX_PAD_ENET1_RX_CLK__ENET1_REF_CLK_25M                 0x008C 0x03D4 0x0000 0x1 0x0
#define MX6SX_PAD_ENET1_RX_CLK__AUDMUX_AUD4_TXFS                  0x008C 0x03D4 0x0658 0x2 0x1
#define MX6SX_PAD_ENET1_RX_CLK__UART1_DSR_B                       0x008C 0x03D4 0x0000 0x3 0x0
#define MX6SX_PAD_ENET1_RX_CLK__SPDIF_OUT                         0x008C 0x03D4 0x0000 0x4 0x0
#define MX6SX_PAD_ENET1_RX_CLK__GPIO2_IO_4                        0x008C 0x03D4 0x0000 0x5 0x0
#define MX6SX_PAD_ENET1_RX_CLK__CSI2_DATA_21                      0x008C 0x03D4 0x0000 0x6 0x0
#define MX6SX_PAD_ENET1_RX_CLK__LCDIF2_DATA_18                    0x008C 0x03D4 0x0000 0x7 0x0
#define MX6SX_PAD_ENET1_RX_CLK__VDEC_DEBUG_35                     0x008C 0x03D4 0x0000 0x8 0x0
#define MX6SX_PAD_ENET1_RX_CLK__PCIE_CTRL_DEBUG_29                0x008C 0x03D4 0x0000 0x9 0x0
#define MX6SX_PAD_ENET1_TX_CLK__ENET1_TX_CLK                      0x0090 0x03D8 0x0000 0x0 0x0
/*
 * SION bit is necessary for ENET1_REF_CLK1 (ENET2_REF_CLK2 untested) if it is
 * used as clock output of IMX6SX_CLK_ENET_REF (ENET1_TX_CLK) to e.g. supply a
 * PHY in RMII mode. This configuration is valid if:
 *  - bit 1 in field IMX6SX_GPR1_FEC_CLOCK_PAD_DIR_MASK is set
 *  - bit 1 in field IMX6SX_GPR1_FEC_CLOCK_MUX_SEL_MASK unset
 * It seems to be a silicon bug that in this configuration ENET1_TX reference
 * clock isn't provided automatically.  According to i.MX6SX reference manual
 * (IOMUXC_GPR_GPR1 field descriptions: ENET1_CLK_SEL, Rev. 0 from 2/2015) it
 * should be the case.
 * So this might have unwanted side effects for other hardware units that are
 * also connected to that pin and using respective function as input (e.g.
 * UART1's DTR handling on MX6SX_PAD_ENET1_TX_CLK__UART1_DTR_B).
 */
#define MX6SX_PAD_ENET1_TX_CLK__ENET1_REF_CLK1                    0x0090 0x03D8 0x0760 0x1 0x1
#define MX6SX_PAD_ENET1_TX_CLK__AUDMUX_AUD4_RXD                   0x0090 0x03D8 0x0644 0x2 0x1
#define MX6SX_PAD_ENET1_TX_CLK__UART1_DTR_B                       0x0090 0x03D8 0x0000 0x3 0x0
#define MX6SX_PAD_ENET1_TX_CLK__SPDIF_SR_CLK                      0x0090 0x03D8 0x0000 0x4 0x0
#define MX6SX_PAD_ENET1_TX_CLK__GPIO2_IO_5                        0x0090 0x03D8 0x0000 0x5 0x0
#define MX6SX_PAD_ENET1_TX_CLK__CSI2_DATA_20                      0x0090 0x03D8 0x0000 0x6 0x0
#define MX6SX_PAD_ENET1_TX_CLK__LCDIF2_DATA_19                    0x0090 0x03D8 0x0000 0x7 0x0
#define MX6SX_PAD_ENET1_TX_CLK__VDEC_DEBUG_34                     0x0090 0x03D8 0x0000 0x8 0x0
#define MX6SX_PAD_ENET1_TX_CLK__PCIE_CTRL_DEBUG_28                0x0090 0x03D8 0x0000 0x9 0x0
#define MX6SX_PAD_ENET2_COL__ENET2_COL                            0x0094 0x03DC 0x0000 0x0 0x0
#define MX6SX_PAD_ENET2_COL__ENET1_MDC                            0x0094 0x03DC 0x0000 0x1 0x0
#define MX6SX_PAD_ENET2_COL__AUDMUX_AUD4_RXC                      0x0094 0x03DC 0x064C 0x2 0x1
<<<<<<< HEAD
#define MX6SX_PAD_ENET2_COL__UART1_RX                             0x0094 0x03DC 0x0830 0x3 0x2
#define MX6SX_PAD_ENET2_COL__UART1_TX                             0x0094 0x03DC 0x0000 0x3 0x0
=======
#define MX6SX_PAD_ENET2_COL__UART1_DCE_RX                         0x0094 0x03DC 0x0830 0x3 0x2
#define MX6SX_PAD_ENET2_COL__UART1_DTE_TX                         0x0094 0x03DC 0x0000 0x3 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_ENET2_COL__SPDIF_IN                             0x0094 0x03DC 0x0824 0x4 0x3
#define MX6SX_PAD_ENET2_COL__GPIO2_IO_6                           0x0094 0x03DC 0x0000 0x5 0x0
#define MX6SX_PAD_ENET2_COL__ANATOP_OTG1_ID                       0x0094 0x03DC 0x0624 0x6 0x1
#define MX6SX_PAD_ENET2_COL__LCDIF2_DATA_20                       0x0094 0x03DC 0x0000 0x7 0x0
#define MX6SX_PAD_ENET2_COL__VDEC_DEBUG_33                        0x0094 0x03DC 0x0000 0x8 0x0
#define MX6SX_PAD_ENET2_COL__PCIE_CTRL_DEBUG_27                   0x0094 0x03DC 0x0000 0x9 0x0
#define MX6SX_PAD_ENET2_CRS__ENET2_CRS                            0x0098 0x03E0 0x0000 0x0 0x0
#define MX6SX_PAD_ENET2_CRS__ENET1_MDIO                           0x0098 0x03E0 0x0764 0x1 0x2
#define MX6SX_PAD_ENET2_CRS__AUDMUX_AUD4_RXFS                     0x0098 0x03E0 0x0650 0x2 0x1
<<<<<<< HEAD
#define MX6SX_PAD_ENET2_CRS__UART1_RX                             0x0098 0x03E0 0x0830 0x3 0x3
#define MX6SX_PAD_ENET2_CRS__UART1_TX                             0x0098 0x03E0 0x0000 0x3 0x0
=======
#define MX6SX_PAD_ENET2_CRS__UART1_DCE_TX                         0x0098 0x03E0 0x0000 0x3 0x0
#define MX6SX_PAD_ENET2_CRS__UART1_DTE_RX                         0x0098 0x03E0 0x0830 0x3 0x3
>>>>>>> upstream/android-13
#define MX6SX_PAD_ENET2_CRS__MLB_SIG                              0x0098 0x03E0 0x07F0 0x4 0x1
#define MX6SX_PAD_ENET2_CRS__GPIO2_IO_7                           0x0098 0x03E0 0x0000 0x5 0x0
#define MX6SX_PAD_ENET2_CRS__ANATOP_OTG2_ID                       0x0098 0x03E0 0x0628 0x6 0x1
#define MX6SX_PAD_ENET2_CRS__LCDIF2_DATA_21                       0x0098 0x03E0 0x0000 0x7 0x0
#define MX6SX_PAD_ENET2_CRS__VDEC_DEBUG_32                        0x0098 0x03E0 0x0000 0x8 0x0
#define MX6SX_PAD_ENET2_CRS__PCIE_CTRL_DEBUG_26                   0x0098 0x03E0 0x0000 0x9 0x0
#define MX6SX_PAD_ENET2_RX_CLK__ENET2_RX_CLK                      0x009C 0x03E4 0x0774 0x0 0x0
#define MX6SX_PAD_ENET2_RX_CLK__ENET2_REF_CLK_25M                 0x009C 0x03E4 0x0000 0x1 0x0
#define MX6SX_PAD_ENET2_RX_CLK__I2C3_SCL                          0x009C 0x03E4 0x07B8 0x2 0x1
<<<<<<< HEAD
#define MX6SX_PAD_ENET2_RX_CLK__UART1_RTS_B                       0x009C 0x03E4 0x082C 0x3 0x2
=======
#define MX6SX_PAD_ENET2_RX_CLK__UART1_DCE_RTS                     0x009C 0x03E4 0x082C 0x3 0x2
#define MX6SX_PAD_ENET2_RX_CLK__UART1_DTE_CTS                     0x009C 0x03E4 0x0000 0x3 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_ENET2_RX_CLK__MLB_DATA                          0x009C 0x03E4 0x07EC 0x4 0x1
#define MX6SX_PAD_ENET2_RX_CLK__GPIO2_IO_8                        0x009C 0x03E4 0x0000 0x5 0x0
#define MX6SX_PAD_ENET2_RX_CLK__USB_OTG2_OC                       0x009C 0x03E4 0x085C 0x6 0x1
#define MX6SX_PAD_ENET2_RX_CLK__LCDIF2_DATA_22                    0x009C 0x03E4 0x0000 0x7 0x0
#define MX6SX_PAD_ENET2_RX_CLK__VDEC_DEBUG_31                     0x009C 0x03E4 0x0000 0x8 0x0
#define MX6SX_PAD_ENET2_RX_CLK__PCIE_CTRL_DEBUG_25                0x009C 0x03E4 0x0000 0x9 0x0
#define MX6SX_PAD_ENET2_TX_CLK__ENET2_TX_CLK                      0x00A0 0x03E8 0x0000 0x0 0x0
#define MX6SX_PAD_ENET2_TX_CLK__ENET2_REF_CLK2                    0x00A0 0x03E8 0x076C 0x1 0x1
#define MX6SX_PAD_ENET2_TX_CLK__I2C3_SDA                          0x00A0 0x03E8 0x07BC 0x2 0x1
<<<<<<< HEAD
#define MX6SX_PAD_ENET2_TX_CLK__UART1_CTS_B                       0x00A0 0x03E8 0x0000 0x3 0x0
=======
#define MX6SX_PAD_ENET2_TX_CLK__UART1_DCE_CTS                     0x00A0 0x03E8 0x0000 0x3 0x0
#define MX6SX_PAD_ENET2_TX_CLK__UART1_DTE_RTS                     0x00A0 0x03E8 0x082C 0x3 0x3
>>>>>>> upstream/android-13
#define MX6SX_PAD_ENET2_TX_CLK__MLB_CLK                           0x00A0 0x03E8 0x07E8 0x4 0x1
#define MX6SX_PAD_ENET2_TX_CLK__GPIO2_IO_9                        0x00A0 0x03E8 0x0000 0x5 0x0
#define MX6SX_PAD_ENET2_TX_CLK__USB_OTG2_PWR                      0x00A0 0x03E8 0x0000 0x6 0x0
#define MX6SX_PAD_ENET2_TX_CLK__LCDIF2_DATA_23                    0x00A0 0x03E8 0x0000 0x7 0x0
#define MX6SX_PAD_ENET2_TX_CLK__VDEC_DEBUG_30                     0x00A0 0x03E8 0x0000 0x8 0x0
#define MX6SX_PAD_ENET2_TX_CLK__PCIE_CTRL_DEBUG_24                0x00A0 0x03E8 0x0000 0x9 0x0
#define MX6SX_PAD_KEY_COL0__KPP_COL_0                             0x00A4 0x03EC 0x0000 0x0 0x0
#define MX6SX_PAD_KEY_COL0__USDHC3_CD_B                           0x00A4 0x03EC 0x0000 0x1 0x0
<<<<<<< HEAD
#define MX6SX_PAD_KEY_COL0__UART6_RTS_B                           0x00A4 0x03EC 0x0854 0x2 0x2
=======
#define MX6SX_PAD_KEY_COL0__UART6_DCE_RTS                         0x00A4 0x03EC 0x0854 0x2 0x2
#define MX6SX_PAD_KEY_COL0__UART6_DTE_CTS                         0x00A4 0x03EC 0x0000 0x2 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_KEY_COL0__ECSPI1_SCLK                           0x00A4 0x03EC 0x0710 0x3 0x0
#define MX6SX_PAD_KEY_COL0__AUDMUX_AUD5_TXC                       0x00A4 0x03EC 0x066C 0x4 0x0
#define MX6SX_PAD_KEY_COL0__GPIO2_IO_10                           0x00A4 0x03EC 0x0000 0x5 0x0
#define MX6SX_PAD_KEY_COL0__SDMA_EXT_EVENT_1                      0x00A4 0x03EC 0x0820 0x6 0x1
#define MX6SX_PAD_KEY_COL0__SAI2_TX_BCLK                          0x00A4 0x03EC 0x0814 0x7 0x0
#define MX6SX_PAD_KEY_COL0__VADC_DATA_0                           0x00A4 0x03EC 0x0000 0x8 0x0
#define MX6SX_PAD_KEY_COL1__KPP_COL_1                             0x00A8 0x03F0 0x0000 0x0 0x0
#define MX6SX_PAD_KEY_COL1__USDHC3_RESET_B                        0x00A8 0x03F0 0x0000 0x1 0x0
<<<<<<< HEAD
#define MX6SX_PAD_KEY_COL1__UART6_RX                              0x00A8 0x03F0 0x0858 0x2 0x2
#define MX6SX_PAD_KEY_COL1__UART6_TX                              0x00A8 0x03F0 0x0000 0x2 0x0
=======
#define MX6SX_PAD_KEY_COL1__UART6_DCE_TX                          0x00A8 0x03F0 0x0000 0x2 0x0
#define MX6SX_PAD_KEY_COL1__UART6_DTE_RX                          0x00A8 0x03F0 0x0858 0x2 0x2
>>>>>>> upstream/android-13
#define MX6SX_PAD_KEY_COL1__ECSPI1_MISO                           0x00A8 0x03F0 0x0714 0x3 0x0
#define MX6SX_PAD_KEY_COL1__AUDMUX_AUD5_TXFS                      0x00A8 0x03F0 0x0670 0x4 0x0
#define MX6SX_PAD_KEY_COL1__GPIO2_IO_11                           0x00A8 0x03F0 0x0000 0x5 0x0
#define MX6SX_PAD_KEY_COL1__USDHC3_RESET                          0x00A8 0x03F0 0x0000 0x6 0x0
#define MX6SX_PAD_KEY_COL1__SAI2_TX_SYNC                          0x00A8 0x03F0 0x0818 0x7 0x0
#define MX6SX_PAD_KEY_COL2__KPP_COL_2                             0x00AC 0x03F4 0x0000 0x0 0x0
#define MX6SX_PAD_KEY_COL2__USDHC4_CD_B                           0x00AC 0x03F4 0x0874 0x1 0x1
<<<<<<< HEAD
#define MX6SX_PAD_KEY_COL2__UART5_RTS_B                           0x00AC 0x03F4 0x084C 0x2 0x2
=======
#define MX6SX_PAD_KEY_COL2__UART5_DCE_RTS                         0x00AC 0x03F4 0x084C 0x2 0x2
#define MX6SX_PAD_KEY_COL2__UART5_DTE_CTS                         0x00AC 0x03F4 0x0000 0x2 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_KEY_COL2__CAN1_TX                               0x00AC 0x03F4 0x0000 0x3 0x0
#define MX6SX_PAD_KEY_COL2__CANFD_TX1                             0x00AC 0x03F4 0x0000 0x4 0x0
#define MX6SX_PAD_KEY_COL2__GPIO2_IO_12                           0x00AC 0x03F4 0x0000 0x5 0x0
#define MX6SX_PAD_KEY_COL2__WEIM_DATA_30                          0x00AC 0x03F4 0x0000 0x6 0x0
#define MX6SX_PAD_KEY_COL2__ECSPI1_RDY                            0x00AC 0x03F4 0x0000 0x7 0x0
#define MX6SX_PAD_KEY_COL3__KPP_COL_3                             0x00B0 0x03F8 0x0000 0x0 0x0
#define MX6SX_PAD_KEY_COL3__USDHC4_LCTL                           0x00B0 0x03F8 0x0000 0x1 0x0
<<<<<<< HEAD
#define MX6SX_PAD_KEY_COL3__UART5_RX                              0x00B0 0x03F8 0x0850 0x2 0x2
#define MX6SX_PAD_KEY_COL3__UART5_TX                              0x00B0 0x03F8 0x0000 0x2 0x0
=======
#define MX6SX_PAD_KEY_COL3__UART5_DCE_TX                          0x00B0 0x03F8 0x0000 0x2 0x0
#define MX6SX_PAD_KEY_COL3__UART5_DTE_RX                          0x00B0 0x03F8 0x0850 0x2 0x2
>>>>>>> upstream/android-13
#define MX6SX_PAD_KEY_COL3__CAN2_TX                               0x00B0 0x03F8 0x0000 0x3 0x0
#define MX6SX_PAD_KEY_COL3__CANFD_TX2                             0x00B0 0x03F8 0x0000 0x4 0x0
#define MX6SX_PAD_KEY_COL3__GPIO2_IO_13                           0x00B0 0x03F8 0x0000 0x5 0x0
#define MX6SX_PAD_KEY_COL3__WEIM_DATA_28                          0x00B0 0x03F8 0x0000 0x6 0x0
#define MX6SX_PAD_KEY_COL3__ECSPI1_SS2                            0x00B0 0x03F8 0x0000 0x7 0x0
#define MX6SX_PAD_KEY_COL4__KPP_COL_4                             0x00B4 0x03FC 0x0000 0x0 0x0
#define MX6SX_PAD_KEY_COL4__ENET2_MDC                             0x00B4 0x03FC 0x0000 0x1 0x0
#define MX6SX_PAD_KEY_COL4__I2C3_SCL                              0x00B4 0x03FC 0x07B8 0x2 0x2
#define MX6SX_PAD_KEY_COL4__USDHC2_LCTL                           0x00B4 0x03FC 0x0000 0x3 0x0
#define MX6SX_PAD_KEY_COL4__AUDMUX_AUD5_RXC                       0x00B4 0x03FC 0x0664 0x4 0x0
#define MX6SX_PAD_KEY_COL4__GPIO2_IO_14                           0x00B4 0x03FC 0x0000 0x5 0x0
#define MX6SX_PAD_KEY_COL4__WEIM_CRE                              0x00B4 0x03FC 0x0000 0x6 0x0
#define MX6SX_PAD_KEY_COL4__SAI2_RX_BCLK                          0x00B4 0x03FC 0x0808 0x7 0x0
#define MX6SX_PAD_KEY_ROW0__KPP_ROW_0                             0x00B8 0x0400 0x0000 0x0 0x0
#define MX6SX_PAD_KEY_ROW0__USDHC3_WP                             0x00B8 0x0400 0x0000 0x1 0x0
<<<<<<< HEAD
#define MX6SX_PAD_KEY_ROW0__UART6_CTS_B                           0x00B8 0x0400 0x0000 0x2 0x0
=======
#define MX6SX_PAD_KEY_ROW0__UART6_DCE_CTS                         0x00B8 0x0400 0x0000 0x2 0x0
#define MX6SX_PAD_KEY_ROW0__UART6_DTE_RTS                         0x00B8 0x0400 0x0854 0x2 0x3
>>>>>>> upstream/android-13
#define MX6SX_PAD_KEY_ROW0__ECSPI1_MOSI                           0x00B8 0x0400 0x0718 0x3 0x0
#define MX6SX_PAD_KEY_ROW0__AUDMUX_AUD5_TXD                       0x00B8 0x0400 0x0660 0x4 0x0
#define MX6SX_PAD_KEY_ROW0__GPIO2_IO_15                           0x00B8 0x0400 0x0000 0x5 0x0
#define MX6SX_PAD_KEY_ROW0__SDMA_EXT_EVENT_0                      0x00B8 0x0400 0x081C 0x6 0x1
#define MX6SX_PAD_KEY_ROW0__SAI2_TX_DATA_0                        0x00B8 0x0400 0x0000 0x7 0x0
#define MX6SX_PAD_KEY_ROW0__GPU_IDLE                              0x00B8 0x0400 0x0000 0x8 0x0
#define MX6SX_PAD_KEY_ROW1__KPP_ROW_1                             0x00BC 0x0404 0x0000 0x0 0x0
#define MX6SX_PAD_KEY_ROW1__USDHC4_VSELECT                        0x00BC 0x0404 0x0000 0x1 0x0
<<<<<<< HEAD
#define MX6SX_PAD_KEY_ROW1__UART6_RX                              0x00BC 0x0404 0x0858 0x2 0x3
#define MX6SX_PAD_KEY_ROW1__UART6_TX                              0x00BC 0x0404 0x0000 0x2 0x0
=======
#define MX6SX_PAD_KEY_ROW1__UART6_DCE_RX                          0x00BC 0x0404 0x0858 0x2 0x3
#define MX6SX_PAD_KEY_ROW1__UART6_DTE_TX                          0x00BC 0x0404 0x0000 0x2 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_KEY_ROW1__ECSPI1_SS0                            0x00BC 0x0404 0x071C 0x3 0x0
#define MX6SX_PAD_KEY_ROW1__AUDMUX_AUD5_RXD                       0x00BC 0x0404 0x065C 0x4 0x0
#define MX6SX_PAD_KEY_ROW1__GPIO2_IO_16                           0x00BC 0x0404 0x0000 0x5 0x0
#define MX6SX_PAD_KEY_ROW1__WEIM_DATA_31                          0x00BC 0x0404 0x0000 0x6 0x0
#define MX6SX_PAD_KEY_ROW1__SAI2_RX_DATA_0                        0x00BC 0x0404 0x080C 0x7 0x0
#define MX6SX_PAD_KEY_ROW1__M4_NMI                                0x00BC 0x0404 0x0000 0x8 0x0
#define MX6SX_PAD_KEY_ROW2__KPP_ROW_2                             0x00C0 0x0408 0x0000 0x0 0x0
#define MX6SX_PAD_KEY_ROW2__USDHC4_WP                             0x00C0 0x0408 0x0878 0x1 0x1
<<<<<<< HEAD
#define MX6SX_PAD_KEY_ROW2__UART5_CTS_B                           0x00C0 0x0408 0x0000 0x2 0x0
=======
#define MX6SX_PAD_KEY_ROW2__UART5_DCE_CTS                         0x00C0 0x0408 0x0000 0x2 0x0
#define MX6SX_PAD_KEY_ROW2__UART5_DTE_RTS                         0x00C0 0x0408 0x084C 0x2 0x3
>>>>>>> upstream/android-13
#define MX6SX_PAD_KEY_ROW2__CAN1_RX                               0x00C0 0x0408 0x068C 0x3 0x1
#define MX6SX_PAD_KEY_ROW2__CANFD_RX1                             0x00C0 0x0408 0x0694 0x4 0x1
#define MX6SX_PAD_KEY_ROW2__GPIO2_IO_17                           0x00C0 0x0408 0x0000 0x5 0x0
#define MX6SX_PAD_KEY_ROW2__WEIM_DATA_29                          0x00C0 0x0408 0x0000 0x6 0x0
#define MX6SX_PAD_KEY_ROW2__ECSPI1_SS3                            0x00C0 0x0408 0x0000 0x7 0x0
#define MX6SX_PAD_KEY_ROW3__KPP_ROW_3                             0x00C4 0x040C 0x0000 0x0 0x0
#define MX6SX_PAD_KEY_ROW3__USDHC3_LCTL                           0x00C4 0x040C 0x0000 0x1 0x0
<<<<<<< HEAD
#define MX6SX_PAD_KEY_ROW3__UART5_RX                              0x00C4 0x040C 0x0850 0x2 0x3
#define MX6SX_PAD_KEY_ROW3__UART5_TX                              0x00C4 0x040C 0x0000 0x2 0x0
=======
#define MX6SX_PAD_KEY_ROW3__UART5_DCE_RX                          0x00C4 0x040C 0x0850 0x2 0x3
#define MX6SX_PAD_KEY_ROW3__UART5_DTE_TX                          0x00C4 0x040C 0x0000 0x2 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_KEY_ROW3__CAN2_RX                               0x00C4 0x040C 0x0690 0x3 0x1
#define MX6SX_PAD_KEY_ROW3__CANFD_RX2                             0x00C4 0x040C 0x0698 0x4 0x1
#define MX6SX_PAD_KEY_ROW3__GPIO2_IO_18                           0x00C4 0x040C 0x0000 0x5 0x0
#define MX6SX_PAD_KEY_ROW3__WEIM_DTACK_B                          0x00C4 0x040C 0x0000 0x6 0x0
#define MX6SX_PAD_KEY_ROW3__ECSPI1_SS1                            0x00C4 0x040C 0x0000 0x7 0x0
#define MX6SX_PAD_KEY_ROW4__KPP_ROW_4                             0x00C8 0x0410 0x0000 0x0 0x0
#define MX6SX_PAD_KEY_ROW4__ENET2_MDIO                            0x00C8 0x0410 0x0770 0x1 0x3
#define MX6SX_PAD_KEY_ROW4__I2C3_SDA                              0x00C8 0x0410 0x07BC 0x2 0x2
#define MX6SX_PAD_KEY_ROW4__USDHC1_LCTL                           0x00C8 0x0410 0x0000 0x3 0x0
#define MX6SX_PAD_KEY_ROW4__AUDMUX_AUD5_RXFS                      0x00C8 0x0410 0x0668 0x4 0x0
#define MX6SX_PAD_KEY_ROW4__GPIO2_IO_19                           0x00C8 0x0410 0x0000 0x5 0x0
#define MX6SX_PAD_KEY_ROW4__WEIM_ACLK_FREERUN                     0x00C8 0x0410 0x0000 0x6 0x0
#define MX6SX_PAD_KEY_ROW4__SAI2_RX_SYNC                          0x00C8 0x0410 0x0810 0x7 0x0
#define MX6SX_PAD_LCD1_CLK__LCDIF1_CLK                            0x00CC 0x0414 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_CLK__LCDIF1_WR_RWN                         0x00CC 0x0414 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_CLK__AUDMUX_AUD3_RXC                       0x00CC 0x0414 0x0634 0x2 0x1
#define MX6SX_PAD_LCD1_CLK__ENET1_1588_EVENT2_IN                  0x00CC 0x0414 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_CLK__CSI1_DATA_16                          0x00CC 0x0414 0x06DC 0x4 0x0
#define MX6SX_PAD_LCD1_CLK__GPIO3_IO_0                            0x00CC 0x0414 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_CLK__USDHC1_WP                             0x00CC 0x0414 0x0868 0x6 0x0
#define MX6SX_PAD_LCD1_CLK__SIM_M_HADDR_16                        0x00CC 0x0414 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_CLK__VADC_TEST_0                           0x00CC 0x0414 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_CLK__MMDC_DEBUG_0                          0x00CC 0x0414 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA00__LCDIF1_DATA_0                      0x00D0 0x0418 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA00__WEIM_CS1_B                         0x00D0 0x0418 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA00__M4_TRACE_0                         0x00D0 0x0418 0x0000 0x2 0x0
#define MX6SX_PAD_LCD1_DATA00__KITTEN_TRACE_0                     0x00D0 0x0418 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA00__CSI1_DATA_20                       0x00D0 0x0418 0x06EC 0x4 0x0
#define MX6SX_PAD_LCD1_DATA00__GPIO3_IO_1                         0x00D0 0x0418 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA00__SRC_BT_CFG_0                       0x00D0 0x0418 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA00__SIM_M_HADDR_21                     0x00D0 0x0418 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA00__VADC_TEST_5                        0x00D0 0x0418 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA00__MMDC_DEBUG_5                       0x00D0 0x0418 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA01__LCDIF1_DATA_1                      0x00D4 0x041C 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA01__WEIM_CS2_B                         0x00D4 0x041C 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA01__M4_TRACE_1                         0x00D4 0x041C 0x0000 0x2 0x0
#define MX6SX_PAD_LCD1_DATA01__KITTEN_TRACE_1                     0x00D4 0x041C 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA01__CSI1_DATA_21                       0x00D4 0x041C 0x06F0 0x4 0x0
#define MX6SX_PAD_LCD1_DATA01__GPIO3_IO_2                         0x00D4 0x041C 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA01__SRC_BT_CFG_1                       0x00D4 0x041C 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA01__SIM_M_HADDR_22                     0x00D4 0x041C 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA01__VADC_TEST_6                        0x00D4 0x041C 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA01__MMDC_DEBUG_6                       0x00D4 0x041C 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA02__LCDIF1_DATA_2                      0x00D8 0x0420 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA02__WEIM_CS3_B                         0x00D8 0x0420 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA02__M4_TRACE_2                         0x00D8 0x0420 0x0000 0x2 0x0
#define MX6SX_PAD_LCD1_DATA02__KITTEN_TRACE_2                     0x00D8 0x0420 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA02__CSI1_DATA_22                       0x00D8 0x0420 0x06F4 0x4 0x0
#define MX6SX_PAD_LCD1_DATA02__GPIO3_IO_3                         0x00D8 0x0420 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA02__SRC_BT_CFG_2                       0x00D8 0x0420 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA02__SIM_M_HADDR_23                     0x00D8 0x0420 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA02__VADC_TEST_7                        0x00D8 0x0420 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA02__MMDC_DEBUG_7                       0x00D8 0x0420 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA03__LCDIF1_DATA_3                      0x00DC 0x0424 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA03__WEIM_ADDR_24                       0x00DC 0x0424 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA03__M4_TRACE_3                         0x00DC 0x0424 0x0000 0x2 0x0
#define MX6SX_PAD_LCD1_DATA03__KITTEN_TRACE_3                     0x00DC 0x0424 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA03__CSI1_DATA_23                       0x00DC 0x0424 0x06F8 0x4 0x0
#define MX6SX_PAD_LCD1_DATA03__GPIO3_IO_4                         0x00DC 0x0424 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA03__SRC_BT_CFG_3                       0x00DC 0x0424 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA03__SIM_M_HADDR_24                     0x00DC 0x0424 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA03__VADC_TEST_8                        0x00DC 0x0424 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA03__MMDC_DEBUG_8                       0x00DC 0x0424 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA04__LCDIF1_DATA_4                      0x00E0 0x0428 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA04__WEIM_ADDR_25                       0x00E0 0x0428 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA04__KITTEN_TRACE_4                     0x00E0 0x0428 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA04__CSI1_VSYNC                         0x00E0 0x0428 0x0708 0x4 0x1
#define MX6SX_PAD_LCD1_DATA04__GPIO3_IO_5                         0x00E0 0x0428 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA04__SRC_BT_CFG_4                       0x00E0 0x0428 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA04__SIM_M_HADDR_25                     0x00E0 0x0428 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA04__VADC_TEST_9                        0x00E0 0x0428 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA04__MMDC_DEBUG_9                       0x00E0 0x0428 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA05__LCDIF1_DATA_5                      0x00E4 0x042C 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA05__WEIM_ADDR_26                       0x00E4 0x042C 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA05__KITTEN_TRACE_5                     0x00E4 0x042C 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA05__CSI1_HSYNC                         0x00E4 0x042C 0x0700 0x4 0x1
#define MX6SX_PAD_LCD1_DATA05__GPIO3_IO_6                         0x00E4 0x042C 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA05__SRC_BT_CFG_5                       0x00E4 0x042C 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA05__SIM_M_HADDR_26                     0x00E4 0x042C 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA05__VADC_TEST_10                       0x00E4 0x042C 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA05__MMDC_DEBUG_10                      0x00E4 0x042C 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA06__LCDIF1_DATA_6                      0x00E8 0x0430 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA06__WEIM_EB_B_2                        0x00E8 0x0430 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA06__KITTEN_TRACE_6                     0x00E8 0x0430 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA06__CSI1_PIXCLK                        0x00E8 0x0430 0x0704 0x4 0x1
#define MX6SX_PAD_LCD1_DATA06__GPIO3_IO_7                         0x00E8 0x0430 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA06__SRC_BT_CFG_6                       0x00E8 0x0430 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA06__SIM_M_HADDR_27                     0x00E8 0x0430 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA06__VADC_TEST_11                       0x00E8 0x0430 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA06__MMDC_DEBUG_11                      0x00E8 0x0430 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA07__LCDIF1_DATA_7                      0x00EC 0x0434 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA07__WEIM_EB_B_3                        0x00EC 0x0434 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA07__KITTEN_TRACE_7                     0x00EC 0x0434 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA07__CSI1_MCLK                          0x00EC 0x0434 0x0000 0x4 0x0
#define MX6SX_PAD_LCD1_DATA07__GPIO3_IO_8                         0x00EC 0x0434 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA07__SRC_BT_CFG_7                       0x00EC 0x0434 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA07__SIM_M_HADDR_28                     0x00EC 0x0434 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA07__VADC_TEST_12                       0x00EC 0x0434 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA07__MMDC_DEBUG_12                      0x00EC 0x0434 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA08__LCDIF1_DATA_8                      0x00F0 0x0438 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA08__WEIM_AD_8                          0x00F0 0x0438 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA08__KITTEN_TRACE_8                     0x00F0 0x0438 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA08__CSI1_DATA_9                        0x00F0 0x0438 0x06C4 0x4 0x1
#define MX6SX_PAD_LCD1_DATA08__GPIO3_IO_9                         0x00F0 0x0438 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA08__SRC_BT_CFG_8                       0x00F0 0x0438 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA08__SIM_M_HADDR_29                     0x00F0 0x0438 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA08__VADC_TEST_13                       0x00F0 0x0438 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA08__MMDC_DEBUG_13                      0x00F0 0x0438 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA09__LCDIF1_DATA_9                      0x00F4 0x043C 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA09__WEIM_AD_9                          0x00F4 0x043C 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA09__KITTEN_TRACE_9                     0x00F4 0x043C 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA09__CSI1_DATA_8                        0x00F4 0x043C 0x06C0 0x4 0x1
#define MX6SX_PAD_LCD1_DATA09__GPIO3_IO_10                        0x00F4 0x043C 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA09__SRC_BT_CFG_9                       0x00F4 0x043C 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA09__SIM_M_HADDR_30                     0x00F4 0x043C 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA09__VADC_TEST_14                       0x00F4 0x043C 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA09__MMDC_DEBUG_14                      0x00F4 0x043C 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA10__LCDIF1_DATA_10                     0x00F8 0x0440 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA10__WEIM_AD_10                         0x00F8 0x0440 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA10__KITTEN_TRACE_10                    0x00F8 0x0440 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA10__CSI1_DATA_7                        0x00F8 0x0440 0x06BC 0x4 0x1
#define MX6SX_PAD_LCD1_DATA10__GPIO3_IO_11                        0x00F8 0x0440 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA10__SRC_BT_CFG_10                      0x00F8 0x0440 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA10__SIM_M_HADDR_31                     0x00F8 0x0440 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA10__VADC_TEST_15                       0x00F8 0x0440 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA10__MMDC_DEBUG_15                      0x00F8 0x0440 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA11__LCDIF1_DATA_11                     0x00FC 0x0444 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA11__WEIM_AD_11                         0x00FC 0x0444 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA11__KITTEN_TRACE_11                    0x00FC 0x0444 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA11__CSI1_DATA_6                        0x00FC 0x0444 0x06B8 0x4 0x1
#define MX6SX_PAD_LCD1_DATA11__GPIO3_IO_12                        0x00FC 0x0444 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA11__SRC_BT_CFG_11                      0x00FC 0x0444 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA11__SIM_M_HBURST_0                     0x00FC 0x0444 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA11__VADC_TEST_16                       0x00FC 0x0444 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA11__MMDC_DEBUG_16                      0x00FC 0x0444 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA12__LCDIF1_DATA_12                     0x0100 0x0448 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA12__WEIM_AD_12                         0x0100 0x0448 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA12__KITTEN_TRACE_12                    0x0100 0x0448 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA12__CSI1_DATA_5                        0x0100 0x0448 0x06B4 0x4 0x1
#define MX6SX_PAD_LCD1_DATA12__GPIO3_IO_13                        0x0100 0x0448 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA12__SRC_BT_CFG_12                      0x0100 0x0448 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA12__SIM_M_HBURST_1                     0x0100 0x0448 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA12__VADC_TEST_17                       0x0100 0x0448 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA12__MMDC_DEBUG_17                      0x0100 0x0448 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA13__LCDIF1_DATA_13                     0x0104 0x044C 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA13__WEIM_AD_13                         0x0104 0x044C 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA13__KITTEN_TRACE_13                    0x0104 0x044C 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA13__CSI1_DATA_4                        0x0104 0x044C 0x06B0 0x4 0x1
#define MX6SX_PAD_LCD1_DATA13__GPIO3_IO_14                        0x0104 0x044C 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA13__SRC_BT_CFG_13                      0x0104 0x044C 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA13__SIM_M_HBURST_2                     0x0104 0x044C 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA13__VADC_TEST_18                       0x0104 0x044C 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA13__MMDC_DEBUG_18                      0x0104 0x044C 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA14__LCDIF1_DATA_14                     0x0108 0x0450 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA14__WEIM_AD_14                         0x0108 0x0450 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA14__KITTEN_TRACE_14                    0x0108 0x0450 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA14__CSI1_DATA_3                        0x0108 0x0450 0x06AC 0x4 0x1
#define MX6SX_PAD_LCD1_DATA14__GPIO3_IO_15                        0x0108 0x0450 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA14__SRC_BT_CFG_14                      0x0108 0x0450 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA14__SIM_M_HMASTLOCK                    0x0108 0x0450 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA14__VADC_TEST_19                       0x0108 0x0450 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA14__MMDC_DEBUG_19                      0x0108 0x0450 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA15__LCDIF1_DATA_15                     0x010C 0x0454 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA15__WEIM_AD_15                         0x010C 0x0454 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA15__KITTEN_TRACE_15                    0x010C 0x0454 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA15__CSI1_DATA_2                        0x010C 0x0454 0x06A8 0x4 0x1
#define MX6SX_PAD_LCD1_DATA15__GPIO3_IO_16                        0x010C 0x0454 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA15__SRC_BT_CFG_15                      0x010C 0x0454 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA15__SIM_M_HPROT_0                      0x010C 0x0454 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA15__VDEC_DEBUG_0                       0x010C 0x0454 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA15__MMDC_DEBUG_20                      0x010C 0x0454 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA16__LCDIF1_DATA_16                     0x0110 0x0458 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA16__WEIM_ADDR_16                       0x0110 0x0458 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA16__M4_TRACE_CLK                       0x0110 0x0458 0x0000 0x2 0x0
#define MX6SX_PAD_LCD1_DATA16__KITTEN_TRACE_CLK                   0x0110 0x0458 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA16__CSI1_DATA_1                        0x0110 0x0458 0x06A4 0x4 0x0
#define MX6SX_PAD_LCD1_DATA16__GPIO3_IO_17                        0x0110 0x0458 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA16__SRC_BT_CFG_24                      0x0110 0x0458 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA16__SIM_M_HPROT_1                      0x0110 0x0458 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA16__VDEC_DEBUG_1                       0x0110 0x0458 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA16__MMDC_DEBUG_21                      0x0110 0x0458 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA17__LCDIF1_DATA_17                     0x0114 0x045C 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA17__WEIM_ADDR_17                       0x0114 0x045C 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA17__KITTEN_TRACE_CTL                   0x0114 0x045C 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA17__CSI1_DATA_0                        0x0114 0x045C 0x06A0 0x4 0x0
#define MX6SX_PAD_LCD1_DATA17__GPIO3_IO_18                        0x0114 0x045C 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA17__SRC_BT_CFG_25                      0x0114 0x045C 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA17__SIM_M_HPROT_2                      0x0114 0x045C 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA17__VDEC_DEBUG_2                       0x0114 0x045C 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA17__MMDC_DEBUG_22                      0x0114 0x045C 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA18__LCDIF1_DATA_18                     0x0118 0x0460 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA18__WEIM_ADDR_18                       0x0118 0x0460 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA18__M4_EVENTO                          0x0118 0x0460 0x0000 0x2 0x0
#define MX6SX_PAD_LCD1_DATA18__KITTEN_EVENTO                      0x0118 0x0460 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA18__CSI1_DATA_15                       0x0118 0x0460 0x06D8 0x4 0x0
#define MX6SX_PAD_LCD1_DATA18__GPIO3_IO_19                        0x0118 0x0460 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA18__SRC_BT_CFG_26                      0x0118 0x0460 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA18__SIM_M_HPROT_3                      0x0118 0x0460 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA18__VDEC_DEBUG_3                       0x0118 0x0460 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA18__MMDC_DEBUG_23                      0x0118 0x0460 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA19__LCDIF1_DATA_19                     0x011C 0x0464 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA19__WEIM_ADDR_19                       0x011C 0x0464 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA19__M4_TRACE_SWO                       0x011C 0x0464 0x0000 0x2 0x0
#define MX6SX_PAD_LCD1_DATA19__CSI1_DATA_14                       0x011C 0x0464 0x06D4 0x4 0x0
#define MX6SX_PAD_LCD1_DATA19__GPIO3_IO_20                        0x011C 0x0464 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA19__SRC_BT_CFG_27                      0x011C 0x0464 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA19__SIM_M_HREADYOUT                    0x011C 0x0464 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA19__VDEC_DEBUG_4                       0x011C 0x0464 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA19__MMDC_DEBUG_24                      0x011C 0x0464 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA20__LCDIF1_DATA_20                     0x0120 0x0468 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA20__WEIM_ADDR_20                       0x0120 0x0468 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA20__PWM8_OUT                           0x0120 0x0468 0x0000 0x2 0x0
#define MX6SX_PAD_LCD1_DATA20__ENET1_1588_EVENT2_OUT              0x0120 0x0468 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA20__CSI1_DATA_13                       0x0120 0x0468 0x06D0 0x4 0x0
#define MX6SX_PAD_LCD1_DATA20__GPIO3_IO_21                        0x0120 0x0468 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA20__SRC_BT_CFG_28                      0x0120 0x0468 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA20__SIM_M_HRESP                        0x0120 0x0468 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA20__VDEC_DEBUG_5                       0x0120 0x0468 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA20__MMDC_DEBUG_25                      0x0120 0x0468 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA21__LCDIF1_DATA_21                     0x0124 0x046C 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA21__WEIM_ADDR_21                       0x0124 0x046C 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA21__PWM7_OUT                           0x0124 0x046C 0x0000 0x2 0x0
#define MX6SX_PAD_LCD1_DATA21__ENET1_1588_EVENT3_OUT              0x0124 0x046C 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA21__CSI1_DATA_12                       0x0124 0x046C 0x06CC 0x4 0x0
#define MX6SX_PAD_LCD1_DATA21__GPIO3_IO_22                        0x0124 0x046C 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA21__SRC_BT_CFG_29                      0x0124 0x046C 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA21__SIM_M_HSIZE_0                      0x0124 0x046C 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA21__VDEC_DEBUG_6                       0x0124 0x046C 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA21__MMDC_DEBUG_26                      0x0124 0x046C 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA22__LCDIF1_DATA_22                     0x0128 0x0470 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA22__WEIM_ADDR_22                       0x0128 0x0470 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA22__PWM6_OUT                           0x0128 0x0470 0x0000 0x2 0x0
#define MX6SX_PAD_LCD1_DATA22__ENET2_1588_EVENT2_OUT              0x0128 0x0470 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA22__CSI1_DATA_11                       0x0128 0x0470 0x06C8 0x4 0x0
#define MX6SX_PAD_LCD1_DATA22__GPIO3_IO_23                        0x0128 0x0470 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA22__SRC_BT_CFG_30                      0x0128 0x0470 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA22__SIM_M_HSIZE_1                      0x0128 0x0470 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA22__VDEC_DEBUG_7                       0x0128 0x0470 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA22__MMDC_DEBUG_27                      0x0128 0x0470 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_DATA23__LCDIF1_DATA_23                     0x012C 0x0474 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_DATA23__WEIM_ADDR_23                       0x012C 0x0474 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_DATA23__PWM5_OUT                           0x012C 0x0474 0x0000 0x2 0x0
#define MX6SX_PAD_LCD1_DATA23__ENET2_1588_EVENT3_OUT              0x012C 0x0474 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_DATA23__CSI1_DATA_10                       0x012C 0x0474 0x06FC 0x4 0x0
#define MX6SX_PAD_LCD1_DATA23__GPIO3_IO_24                        0x012C 0x0474 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_DATA23__SRC_BT_CFG_31                      0x012C 0x0474 0x0000 0x6 0x0
#define MX6SX_PAD_LCD1_DATA23__SIM_M_HSIZE_2                      0x012C 0x0474 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_DATA23__VDEC_DEBUG_8                       0x012C 0x0474 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_DATA23__MMDC_DEBUG_28                      0x012C 0x0474 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_ENABLE__LCDIF1_ENABLE                      0x0130 0x0478 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_ENABLE__LCDIF1_RD_E                        0x0130 0x0478 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_ENABLE__AUDMUX_AUD3_TXC                    0x0130 0x0478 0x063C 0x2 0x1
#define MX6SX_PAD_LCD1_ENABLE__ENET1_1588_EVENT3_IN               0x0130 0x0478 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_ENABLE__CSI1_DATA_17                       0x0130 0x0478 0x06E0 0x4 0x0
#define MX6SX_PAD_LCD1_ENABLE__GPIO3_IO_25                        0x0130 0x0478 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_ENABLE__USDHC1_CD_B                        0x0130 0x0478 0x0864 0x6 0x0
#define MX6SX_PAD_LCD1_ENABLE__SIM_M_HADDR_17                     0x0130 0x0478 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_ENABLE__VADC_TEST_1                        0x0130 0x0478 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_ENABLE__MMDC_DEBUG_1                       0x0130 0x0478 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_HSYNC__LCDIF1_HSYNC                        0x0134 0x047C 0x07E0 0x0 0x0
#define MX6SX_PAD_LCD1_HSYNC__LCDIF1_RS                           0x0134 0x047C 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_HSYNC__AUDMUX_AUD3_TXD                     0x0134 0x047C 0x0630 0x2 0x1
#define MX6SX_PAD_LCD1_HSYNC__ENET2_1588_EVENT2_IN                0x0134 0x047C 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_HSYNC__CSI1_DATA_18                        0x0134 0x047C 0x06E4 0x4 0x0
#define MX6SX_PAD_LCD1_HSYNC__GPIO3_IO_26                         0x0134 0x047C 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_HSYNC__USDHC2_WP                           0x0134 0x047C 0x0870 0x6 0x0
#define MX6SX_PAD_LCD1_HSYNC__SIM_M_HADDR_18                      0x0134 0x047C 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_HSYNC__VADC_TEST_2                         0x0134 0x047C 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_HSYNC__MMDC_DEBUG_2                        0x0134 0x047C 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_RESET__LCDIF1_RESET                        0x0138 0x0480 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_RESET__LCDIF1_CS                           0x0138 0x0480 0x0000 0x1 0x0
#define MX6SX_PAD_LCD1_RESET__AUDMUX_AUD3_RXD                     0x0138 0x0480 0x062C 0x2 0x1
#define MX6SX_PAD_LCD1_RESET__KITTEN_EVENTI                       0x0138 0x0480 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_RESET__M4_EVENTI                           0x0138 0x0480 0x0000 0x4 0x0
#define MX6SX_PAD_LCD1_RESET__GPIO3_IO_27                         0x0138 0x0480 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_RESET__CCM_PMIC_RDY                        0x0138 0x0480 0x069C 0x6 0x0
#define MX6SX_PAD_LCD1_RESET__SIM_M_HADDR_20                      0x0138 0x0480 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_RESET__VADC_TEST_4                         0x0138 0x0480 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_RESET__MMDC_DEBUG_4                        0x0138 0x0480 0x0000 0x9 0x0
#define MX6SX_PAD_LCD1_VSYNC__LCDIF1_VSYNC                        0x013C 0x0484 0x0000 0x0 0x0
#define MX6SX_PAD_LCD1_VSYNC__LCDIF1_BUSY                         0x013C 0x0484 0x07E0 0x1 0x1
#define MX6SX_PAD_LCD1_VSYNC__AUDMUX_AUD3_TXFS                    0x013C 0x0484 0x0640 0x2 0x1
#define MX6SX_PAD_LCD1_VSYNC__ENET2_1588_EVENT3_IN                0x013C 0x0484 0x0000 0x3 0x0
#define MX6SX_PAD_LCD1_VSYNC__CSI1_DATA_19                        0x013C 0x0484 0x06E8 0x4 0x0
#define MX6SX_PAD_LCD1_VSYNC__GPIO3_IO_28                         0x013C 0x0484 0x0000 0x5 0x0
#define MX6SX_PAD_LCD1_VSYNC__USDHC2_CD_B                         0x013C 0x0484 0x086C 0x6 0x0
#define MX6SX_PAD_LCD1_VSYNC__SIM_M_HADDR_19                      0x013C 0x0484 0x0000 0x7 0x0
#define MX6SX_PAD_LCD1_VSYNC__VADC_TEST_3                         0x013C 0x0484 0x0000 0x8 0x0
#define MX6SX_PAD_LCD1_VSYNC__MMDC_DEBUG_3                        0x013C 0x0484 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_ALE__RAWNAND_ALE                           0x0140 0x0488 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_ALE__I2C3_SDA                              0x0140 0x0488 0x07BC 0x1 0x0
#define MX6SX_PAD_NAND_ALE__QSPI2_A_SS0_B                         0x0140 0x0488 0x0000 0x2 0x0
#define MX6SX_PAD_NAND_ALE__ECSPI2_SS0                            0x0140 0x0488 0x072C 0x3 0x0
#define MX6SX_PAD_NAND_ALE__ESAI_TX3_RX2                          0x0140 0x0488 0x079C 0x4 0x0
#define MX6SX_PAD_NAND_ALE__GPIO4_IO_0                            0x0140 0x0488 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_ALE__WEIM_CS0_B                            0x0140 0x0488 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_ALE__TPSMP_HDATA_0                         0x0140 0x0488 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_ALE__ANATOP_USBPHY1_TSTI_TX_EN             0x0140 0x0488 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_ALE__SDMA_DEBUG_PC_12                      0x0140 0x0488 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_CE0_B__RAWNAND_CE0_B                       0x0144 0x048C 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_CE0_B__USDHC2_VSELECT                      0x0144 0x048C 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_CE0_B__QSPI2_A_DATA_2                      0x0144 0x048C 0x0000 0x2 0x0
#define MX6SX_PAD_NAND_CE0_B__AUDMUX_AUD4_TXC                     0x0144 0x048C 0x0654 0x3 0x0
#define MX6SX_PAD_NAND_CE0_B__ESAI_TX_CLK                         0x0144 0x048C 0x078C 0x4 0x0
#define MX6SX_PAD_NAND_CE0_B__GPIO4_IO_1                          0x0144 0x048C 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_CE0_B__WEIM_LBA_B                          0x0144 0x048C 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_CE0_B__TPSMP_HDATA_3                       0x0144 0x048C 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_CE0_B__ANATOP_USBPHY1_TSTI_TX_HIZ          0x0144 0x048C 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_CE0_B__SDMA_DEBUG_PC_9                     0x0144 0x048C 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_CE1_B__RAWNAND_CE1_B                       0x0148 0x0490 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_CE1_B__USDHC3_RESET_B                      0x0148 0x0490 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_CE1_B__QSPI2_A_DATA_3                      0x0148 0x0490 0x0000 0x2 0x0
#define MX6SX_PAD_NAND_CE1_B__AUDMUX_AUD4_TXD                     0x0148 0x0490 0x0648 0x3 0x0
#define MX6SX_PAD_NAND_CE1_B__ESAI_TX0                            0x0148 0x0490 0x0790 0x4 0x0
#define MX6SX_PAD_NAND_CE1_B__GPIO4_IO_2                          0x0148 0x0490 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_CE1_B__WEIM_OE                             0x0148 0x0490 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_CE1_B__TPSMP_HDATA_4                       0x0148 0x0490 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_CE1_B__ANATOP_USBPHY1_TSTI_TX_LS_MODE      0x0148 0x0490 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_CE1_B__SDMA_DEBUG_PC_8                     0x0148 0x0490 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_CLE__RAWNAND_CLE                           0x014C 0x0494 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_CLE__I2C3_SCL                              0x014C 0x0494 0x07B8 0x1 0x0
#define MX6SX_PAD_NAND_CLE__QSPI2_A_SCLK                          0x014C 0x0494 0x0000 0x2 0x0
#define MX6SX_PAD_NAND_CLE__ECSPI2_SCLK                           0x014C 0x0494 0x0720 0x3 0x0
#define MX6SX_PAD_NAND_CLE__ESAI_TX2_RX3                          0x014C 0x0494 0x0798 0x4 0x0
#define MX6SX_PAD_NAND_CLE__GPIO4_IO_3                            0x014C 0x0494 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_CLE__WEIM_BCLK                             0x014C 0x0494 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_CLE__TPSMP_CLK                             0x014C 0x0494 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_CLE__ANATOP_USBPHY1_TSTI_TX_DP             0x014C 0x0494 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_CLE__SDMA_DEBUG_PC_13                      0x014C 0x0494 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_DATA00__RAWNAND_DATA00                     0x0150 0x0498 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_DATA00__USDHC1_DATA4                       0x0150 0x0498 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_DATA00__QSPI2_B_DATA_1                     0x0150 0x0498 0x0000 0x2 0x0
#define MX6SX_PAD_NAND_DATA00__ECSPI5_MISO                        0x0150 0x0498 0x0754 0x3 0x0
#define MX6SX_PAD_NAND_DATA00__ESAI_RX_CLK                        0x0150 0x0498 0x0788 0x4 0x0
#define MX6SX_PAD_NAND_DATA00__GPIO4_IO_4                         0x0150 0x0498 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_DATA00__WEIM_AD_0                          0x0150 0x0498 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_DATA00__TPSMP_HDATA_7                      0x0150 0x0498 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_DATA00__ANATOP_USBPHY1_TSTO_RX_DISCON_DET  0x0150 0x0498 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_DATA00__SDMA_DEBUG_EVT_CHN_LINES_5         0x0150 0x0498 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_DATA01__RAWNAND_DATA01                     0x0154 0x049C 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_DATA01__USDHC1_DATA5                       0x0154 0x049C 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_DATA01__QSPI2_B_DATA_0                     0x0154 0x049C 0x0000 0x2 0x0
#define MX6SX_PAD_NAND_DATA01__ECSPI5_MOSI                        0x0154 0x049C 0x0758 0x3 0x0
#define MX6SX_PAD_NAND_DATA01__ESAI_RX_FS                         0x0154 0x049C 0x0778 0x4 0x0
#define MX6SX_PAD_NAND_DATA01__GPIO4_IO_5                         0x0154 0x049C 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_DATA01__WEIM_AD_1                          0x0154 0x049C 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_DATA01__TPSMP_HDATA_8                      0x0154 0x049C 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_DATA01__ANATOP_USBPHY1_TSTO_RX_HS_RXD      0x0154 0x049C 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_DATA01__SDMA_DEBUG_EVT_CHN_LINES_4         0x0154 0x049C 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_DATA02__RAWNAND_DATA02                     0x0158 0x04A0 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_DATA02__USDHC1_DATA6                       0x0158 0x04A0 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_DATA02__QSPI2_B_SCLK                       0x0158 0x04A0 0x0000 0x2 0x0
#define MX6SX_PAD_NAND_DATA02__ECSPI5_SCLK                        0x0158 0x04A0 0x0750 0x3 0x0
#define MX6SX_PAD_NAND_DATA02__ESAI_TX_HF_CLK                     0x0158 0x04A0 0x0784 0x4 0x0
#define MX6SX_PAD_NAND_DATA02__GPIO4_IO_6                         0x0158 0x04A0 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_DATA02__WEIM_AD_2                          0x0158 0x04A0 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_DATA02__TPSMP_HDATA_9                      0x0158 0x04A0 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_DATA02__ANATOP_USBPHY2_TSTO_PLL_CLK20DIV   0x0158 0x04A0 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_DATA02__SDMA_DEBUG_EVT_CHN_LINES_3         0x0158 0x04A0 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_DATA03__RAWNAND_DATA03                     0x015C 0x04A4 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_DATA03__USDHC1_DATA7                       0x015C 0x04A4 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_DATA03__QSPI2_B_SS0_B                      0x015C 0x04A4 0x0000 0x2 0x0
#define MX6SX_PAD_NAND_DATA03__ECSPI5_SS0                         0x015C 0x04A4 0x075C 0x3 0x0
#define MX6SX_PAD_NAND_DATA03__ESAI_RX_HF_CLK                     0x015C 0x04A4 0x0780 0x4 0x0
#define MX6SX_PAD_NAND_DATA03__GPIO4_IO_7                         0x015C 0x04A4 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_DATA03__WEIM_AD_3                          0x015C 0x04A4 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_DATA03__TPSMP_HDATA_10                     0x015C 0x04A4 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_DATA03__ANATOP_USBPHY1_TSTO_RX_SQUELCH     0x015C 0x04A4 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_DATA03__SDMA_DEBUG_EVT_CHN_LINES_6         0x015C 0x04A4 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_DATA04__RAWNAND_DATA04                     0x0160 0x04A8 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_DATA04__USDHC2_DATA4                       0x0160 0x04A8 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_DATA04__QSPI2_B_SS1_B                      0x0160 0x04A8 0x0000 0x2 0x0
<<<<<<< HEAD
#define MX6SX_PAD_NAND_DATA04__UART3_RTS_B                        0x0160 0x04A8 0x083C 0x3 0x0
=======
#define MX6SX_PAD_NAND_DATA04__UART3_DCE_RTS                      0x0160 0x04A8 0x083C 0x3 0x0
#define MX6SX_PAD_NAND_DATA04__UART3_DTE_CTS                      0x0160 0x04A8 0x0000 0x3 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_NAND_DATA04__AUDMUX_AUD4_RXFS                   0x0160 0x04A8 0x0650 0x4 0x0
#define MX6SX_PAD_NAND_DATA04__GPIO4_IO_8                         0x0160 0x04A8 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_DATA04__WEIM_AD_4                          0x0160 0x04A8 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_DATA04__TPSMP_HDATA_11                     0x0160 0x04A8 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_DATA04__ANATOP_USBPHY2_TSTO_RX_SQUELCH     0x0160 0x04A8 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_DATA04__SDMA_DEBUG_CORE_STATE_0            0x0160 0x04A8 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_DATA05__RAWNAND_DATA05                     0x0164 0x04AC 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_DATA05__USDHC2_DATA5                       0x0164 0x04AC 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_DATA05__QSPI2_B_DQS                        0x0164 0x04AC 0x0000 0x2 0x0
<<<<<<< HEAD
#define MX6SX_PAD_NAND_DATA05__UART3_CTS_B                        0x0164 0x04AC 0x0000 0x3 0x0
=======
#define MX6SX_PAD_NAND_DATA05__UART3_DCE_CTS                      0x0164 0x04AC 0x0000 0x3 0x0
#define MX6SX_PAD_NAND_DATA05__UART3_DTE_RTS                      0x0164 0x04AC 0x083C 0x3 0x1
>>>>>>> upstream/android-13
#define MX6SX_PAD_NAND_DATA05__AUDMUX_AUD4_RXC                    0x0164 0x04AC 0x064C 0x4 0x0
#define MX6SX_PAD_NAND_DATA05__GPIO4_IO_9                         0x0164 0x04AC 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_DATA05__WEIM_AD_5                          0x0164 0x04AC 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_DATA05__TPSMP_HDATA_12                     0x0164 0x04AC 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_DATA05__ANATOP_USBPHY2_TSTO_RX_DISCON_DET  0x0164 0x04AC 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_DATA05__SDMA_DEBUG_CORE_STATE_1            0x0164 0x04AC 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_DATA06__RAWNAND_DATA06                     0x0168 0x04B0 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_DATA06__USDHC2_DATA6                       0x0168 0x04B0 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_DATA06__QSPI2_A_SS1_B                      0x0168 0x04B0 0x0000 0x2 0x0
<<<<<<< HEAD
#define MX6SX_PAD_NAND_DATA06__UART3_RX                           0x0168 0x04B0 0x0840 0x3 0x0
#define MX6SX_PAD_NAND_DATA06__UART3_TX                           0x0168 0x04B0 0x0000 0x3 0x0
=======
#define MX6SX_PAD_NAND_DATA06__UART3_DCE_RX                       0x0168 0x04B0 0x0840 0x3 0x0
#define MX6SX_PAD_NAND_DATA06__UART3_DTE_TX                       0x0168 0x04B0 0x0000 0x3 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_NAND_DATA06__PWM3_OUT                           0x0168 0x04B0 0x0000 0x4 0x0
#define MX6SX_PAD_NAND_DATA06__GPIO4_IO_10                        0x0168 0x04B0 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_DATA06__WEIM_AD_6                          0x0168 0x04B0 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_DATA06__TPSMP_HDATA_13                     0x0168 0x04B0 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_DATA06__ANATOP_USBPHY2_TSTO_RX_FS_RXD      0x0168 0x04B0 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_DATA06__SDMA_DEBUG_CORE_STATE_2            0x0168 0x04B0 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_DATA07__RAWNAND_DATA07                     0x016C 0x04B4 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_DATA07__USDHC2_DATA7                       0x016C 0x04B4 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_DATA07__QSPI2_A_DQS                        0x016C 0x04B4 0x0000 0x2 0x0
<<<<<<< HEAD
#define MX6SX_PAD_NAND_DATA07__UART3_RX                           0x016C 0x04B4 0x0840 0x3 0x1
#define MX6SX_PAD_NAND_DATA07__UART3_TX                           0x016C 0x04B4 0x0000 0x3 0x0
=======
#define MX6SX_PAD_NAND_DATA07__UART3_DCE_TX                       0x016C 0x04B4 0x0000 0x3 0x0
#define MX6SX_PAD_NAND_DATA07__UART3_DTE_RX                       0x016C 0x04B4 0x0840 0x3 0x1
>>>>>>> upstream/android-13
#define MX6SX_PAD_NAND_DATA07__PWM4_OUT                           0x016C 0x04B4 0x0000 0x4 0x0
#define MX6SX_PAD_NAND_DATA07__GPIO4_IO_11                        0x016C 0x04B4 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_DATA07__WEIM_AD_7                          0x016C 0x04B4 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_DATA07__TPSMP_HDATA_14                     0x016C 0x04B4 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_DATA07__ANATOP_USBPHY1_TSTO_RX_FS_RXD      0x016C 0x04B4 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_DATA07__SDMA_DEBUG_CORE_STATE_3            0x016C 0x04B4 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_RE_B__RAWNAND_RE_B                         0x0170 0x04B8 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_RE_B__USDHC2_RESET_B                       0x0170 0x04B8 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_RE_B__QSPI2_B_DATA_3                       0x0170 0x04B8 0x0000 0x2 0x0
#define MX6SX_PAD_NAND_RE_B__AUDMUX_AUD4_TXFS                     0x0170 0x04B8 0x0658 0x3 0x0
#define MX6SX_PAD_NAND_RE_B__ESAI_TX_FS                           0x0170 0x04B8 0x077C 0x4 0x0
#define MX6SX_PAD_NAND_RE_B__GPIO4_IO_12                          0x0170 0x04B8 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_RE_B__WEIM_RW                              0x0170 0x04B8 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_RE_B__TPSMP_HDATA_5                        0x0170 0x04B8 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_RE_B__ANATOP_USBPHY2_TSTO_RX_HS_RXD        0x0170 0x04B8 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_RE_B__SDMA_DEBUG_PC_7                      0x0170 0x04B8 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_READY_B__RAWNAND_READY_B                   0x0174 0x04BC 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_READY_B__USDHC1_VSELECT                    0x0174 0x04BC 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_READY_B__QSPI2_A_DATA_1                    0x0174 0x04BC 0x0000 0x2 0x0
#define MX6SX_PAD_NAND_READY_B__ECSPI2_MISO                       0x0174 0x04BC 0x0724 0x3 0x0
#define MX6SX_PAD_NAND_READY_B__ESAI_TX1                          0x0174 0x04BC 0x0794 0x4 0x0
#define MX6SX_PAD_NAND_READY_B__GPIO4_IO_13                       0x0174 0x04BC 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_READY_B__WEIM_EB_B_1                       0x0174 0x04BC 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_READY_B__TPSMP_HDATA_2                     0x0174 0x04BC 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_READY_B__ANATOP_USBPHY1_TSTI_TX_DN         0x0174 0x04BC 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_READY_B__SDMA_DEBUG_PC_10                  0x0174 0x04BC 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_WE_B__RAWNAND_WE_B                         0x0178 0x04C0 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_WE_B__USDHC4_VSELECT                       0x0178 0x04C0 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_WE_B__QSPI2_B_DATA_2                       0x0178 0x04C0 0x0000 0x2 0x0
#define MX6SX_PAD_NAND_WE_B__AUDMUX_AUD4_RXD                      0x0178 0x04C0 0x0644 0x3 0x0
#define MX6SX_PAD_NAND_WE_B__ESAI_TX5_RX0                         0x0178 0x04C0 0x07A4 0x4 0x0
#define MX6SX_PAD_NAND_WE_B__GPIO4_IO_14                          0x0178 0x04C0 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_WE_B__WEIM_WAIT                            0x0178 0x04C0 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_WE_B__TPSMP_HDATA_6                        0x0178 0x04C0 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_WE_B__ANATOP_USBPHY1_TSTO_PLL_CLK20DIV     0x0178 0x04C0 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_WE_B__SDMA_DEBUG_PC_6                      0x0178 0x04C0 0x0000 0x9 0x0
#define MX6SX_PAD_NAND_WP_B__RAWNAND_WP_B                         0x017C 0x04C4 0x0000 0x0 0x0
#define MX6SX_PAD_NAND_WP_B__USDHC1_RESET_B                       0x017C 0x04C4 0x0000 0x1 0x0
#define MX6SX_PAD_NAND_WP_B__QSPI2_A_DATA_0                       0x017C 0x04C4 0x0000 0x2 0x0
#define MX6SX_PAD_NAND_WP_B__ECSPI2_MOSI                          0x017C 0x04C4 0x0728 0x3 0x0
#define MX6SX_PAD_NAND_WP_B__ESAI_TX4_RX1                         0x017C 0x04C4 0x07A0 0x4 0x0
#define MX6SX_PAD_NAND_WP_B__GPIO4_IO_15                          0x017C 0x04C4 0x0000 0x5 0x0
#define MX6SX_PAD_NAND_WP_B__WEIM_EB_B_0                          0x017C 0x04C4 0x0000 0x6 0x0
#define MX6SX_PAD_NAND_WP_B__TPSMP_HDATA_1                        0x017C 0x04C4 0x0000 0x7 0x0
#define MX6SX_PAD_NAND_WP_B__ANATOP_USBPHY1_TSTI_TX_HS_MODE       0x017C 0x04C4 0x0000 0x8 0x0
#define MX6SX_PAD_NAND_WP_B__SDMA_DEBUG_PC_11                     0x017C 0x04C4 0x0000 0x9 0x0
#define MX6SX_PAD_QSPI1A_DATA0__QSPI1_A_DATA_0                    0x0180 0x04C8 0x0000 0x0 0x0
#define MX6SX_PAD_QSPI1A_DATA0__USB_OTG2_OC                       0x0180 0x04C8 0x085C 0x1 0x2
#define MX6SX_PAD_QSPI1A_DATA0__ECSPI1_MOSI                       0x0180 0x04C8 0x0718 0x2 0x1
#define MX6SX_PAD_QSPI1A_DATA0__ESAI_TX4_RX1                      0x0180 0x04C8 0x07A0 0x3 0x2
#define MX6SX_PAD_QSPI1A_DATA0__CSI1_DATA_14                      0x0180 0x04C8 0x06D4 0x4 0x1
#define MX6SX_PAD_QSPI1A_DATA0__GPIO4_IO_16                       0x0180 0x04C8 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1A_DATA0__WEIM_DATA_6                       0x0180 0x04C8 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1A_DATA0__SIM_M_HADDR_3                     0x0180 0x04C8 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1A_DATA0__SDMA_DEBUG_BUS_DEVICE_3           0x0180 0x04C8 0x0000 0x9 0x0
#define MX6SX_PAD_QSPI1A_DATA1__QSPI1_A_DATA_1                    0x0184 0x04CC 0x0000 0x0 0x0
#define MX6SX_PAD_QSPI1A_DATA1__ANATOP_OTG1_ID                    0x0184 0x04CC 0x0624 0x1 0x2
#define MX6SX_PAD_QSPI1A_DATA1__ECSPI1_MISO                       0x0184 0x04CC 0x0714 0x2 0x1
#define MX6SX_PAD_QSPI1A_DATA1__ESAI_TX1                          0x0184 0x04CC 0x0794 0x3 0x2
#define MX6SX_PAD_QSPI1A_DATA1__CSI1_DATA_13                      0x0184 0x04CC 0x06D0 0x4 0x1
#define MX6SX_PAD_QSPI1A_DATA1__GPIO4_IO_17                       0x0184 0x04CC 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1A_DATA1__WEIM_DATA_5                       0x0184 0x04CC 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1A_DATA1__SIM_M_HADDR_4                     0x0184 0x04CC 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1A_DATA1__SDMA_DEBUG_PC_0                   0x0184 0x04CC 0x0000 0x9 0x0
#define MX6SX_PAD_QSPI1A_DATA2__QSPI1_A_DATA_2                    0x0188 0x04D0 0x0000 0x0 0x0
#define MX6SX_PAD_QSPI1A_DATA2__USB_OTG1_PWR                      0x0188 0x04D0 0x0000 0x1 0x0
#define MX6SX_PAD_QSPI1A_DATA2__ECSPI5_SS1                        0x0188 0x04D0 0x0000 0x2 0x0
#define MX6SX_PAD_QSPI1A_DATA2__ESAI_TX_CLK                       0x0188 0x04D0 0x078C 0x3 0x2
#define MX6SX_PAD_QSPI1A_DATA2__CSI1_DATA_12                      0x0188 0x04D0 0x06CC 0x4 0x1
#define MX6SX_PAD_QSPI1A_DATA2__GPIO4_IO_18                       0x0188 0x04D0 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1A_DATA2__WEIM_DATA_4                       0x0188 0x04D0 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1A_DATA2__SIM_M_HADDR_6                     0x0188 0x04D0 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1A_DATA2__SDMA_DEBUG_PC_1                   0x0188 0x04D0 0x0000 0x9 0x0
#define MX6SX_PAD_QSPI1A_DATA3__QSPI1_A_DATA_3                    0x018C 0x04D4 0x0000 0x0 0x0
#define MX6SX_PAD_QSPI1A_DATA3__USB_OTG1_OC                       0x018C 0x04D4 0x0860 0x1 0x2
#define MX6SX_PAD_QSPI1A_DATA3__ECSPI5_SS2                        0x018C 0x04D4 0x0000 0x2 0x0
#define MX6SX_PAD_QSPI1A_DATA3__ESAI_TX0                          0x018C 0x04D4 0x0790 0x3 0x2
#define MX6SX_PAD_QSPI1A_DATA3__CSI1_DATA_11                      0x018C 0x04D4 0x06C8 0x4 0x1
#define MX6SX_PAD_QSPI1A_DATA3__GPIO4_IO_19                       0x018C 0x04D4 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1A_DATA3__WEIM_DATA_3                       0x018C 0x04D4 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1A_DATA3__SIM_M_HADDR_7                     0x018C 0x04D4 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1A_DATA3__SDMA_DEBUG_PC_2                   0x018C 0x04D4 0x0000 0x9 0x0
#define MX6SX_PAD_QSPI1A_DQS__QSPI1_A_DQS                         0x0190 0x04D8 0x0000 0x0 0x0
#define MX6SX_PAD_QSPI1A_DQS__CAN2_TX                             0x0190 0x04D8 0x0000 0x1 0x0
#define MX6SX_PAD_QSPI1A_DQS__CANFD_TX2                           0x0190 0x04D8 0x0000 0x2 0x0
#define MX6SX_PAD_QSPI1A_DQS__ECSPI5_MOSI                         0x0190 0x04D8 0x0758 0x3 0x1
#define MX6SX_PAD_QSPI1A_DQS__CSI1_DATA_15                        0x0190 0x04D8 0x06D8 0x4 0x1
#define MX6SX_PAD_QSPI1A_DQS__GPIO4_IO_20                         0x0190 0x04D8 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1A_DQS__WEIM_DATA_7                         0x0190 0x04D8 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1A_DQS__SIM_M_HADDR_13                      0x0190 0x04D8 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1A_DQS__SDMA_DEBUG_BUS_DEVICE_4             0x0190 0x04D8 0x0000 0x9 0x0
#define MX6SX_PAD_QSPI1A_SCLK__QSPI1_A_SCLK                       0x0194 0x04DC 0x0000 0x0 0x0
#define MX6SX_PAD_QSPI1A_SCLK__ANATOP_OTG2_ID                     0x0194 0x04DC 0x0628 0x1 0x2
#define MX6SX_PAD_QSPI1A_SCLK__ECSPI1_SCLK                        0x0194 0x04DC 0x0710 0x2 0x1
#define MX6SX_PAD_QSPI1A_SCLK__ESAI_TX2_RX3                       0x0194 0x04DC 0x0798 0x3 0x2
#define MX6SX_PAD_QSPI1A_SCLK__CSI1_DATA_1                        0x0194 0x04DC 0x06A4 0x4 0x1
#define MX6SX_PAD_QSPI1A_SCLK__GPIO4_IO_21                        0x0194 0x04DC 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1A_SCLK__WEIM_DATA_0                        0x0194 0x04DC 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1A_SCLK__SIM_M_HADDR_0                      0x0194 0x04DC 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1A_SCLK__SDMA_DEBUG_PC_5                    0x0194 0x04DC 0x0000 0x9 0x0
#define MX6SX_PAD_QSPI1A_SS0_B__QSPI1_A_SS0_B                     0x0198 0x04E0 0x0000 0x0 0x0
#define MX6SX_PAD_QSPI1A_SS0_B__USB_OTG2_PWR                      0x0198 0x04E0 0x0000 0x1 0x0
#define MX6SX_PAD_QSPI1A_SS0_B__ECSPI1_SS0                        0x0198 0x04E0 0x071C 0x2 0x1
#define MX6SX_PAD_QSPI1A_SS0_B__ESAI_TX3_RX2                      0x0198 0x04E0 0x079C 0x3 0x2
#define MX6SX_PAD_QSPI1A_SS0_B__CSI1_DATA_0                       0x0198 0x04E0 0x06A0 0x4 0x1
#define MX6SX_PAD_QSPI1A_SS0_B__GPIO4_IO_22                       0x0198 0x04E0 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1A_SS0_B__WEIM_DATA_1                       0x0198 0x04E0 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1A_SS0_B__SIM_M_HADDR_1                     0x0198 0x04E0 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1A_SS0_B__SDMA_DEBUG_PC_4                   0x0198 0x04E0 0x0000 0x9 0x0
#define MX6SX_PAD_QSPI1A_SS1_B__QSPI1_A_SS1_B                     0x019C 0x04E4 0x0000 0x0 0x0
#define MX6SX_PAD_QSPI1A_SS1_B__CAN1_RX                           0x019C 0x04E4 0x068C 0x1 0x2
#define MX6SX_PAD_QSPI1A_SS1_B__CANFD_RX1                         0x019C 0x04E4 0x0694 0x2 0x2
#define MX6SX_PAD_QSPI1A_SS1_B__ECSPI5_MISO                       0x019C 0x04E4 0x0754 0x3 0x1
#define MX6SX_PAD_QSPI1A_SS1_B__CSI1_DATA_10                      0x019C 0x04E4 0x06FC 0x4 0x1
#define MX6SX_PAD_QSPI1A_SS1_B__GPIO4_IO_23                       0x019C 0x04E4 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1A_SS1_B__WEIM_DATA_2                       0x019C 0x04E4 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1A_SS1_B__SIM_M_HADDR_12                    0x019C 0x04E4 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1A_SS1_B__SDMA_DEBUG_PC_3                   0x019C 0x04E4 0x0000 0x9 0x0
#define MX6SX_PAD_QSPI1B_DATA0__QSPI1_B_DATA_0                    0x01A0 0x04E8 0x0000 0x0 0x0
<<<<<<< HEAD
#define MX6SX_PAD_QSPI1B_DATA0__UART3_CTS_B                       0x01A0 0x04E8 0x0000 0x1 0x0
=======
#define MX6SX_PAD_QSPI1B_DATA0__UART3_DCE_CTS                     0x01A0 0x04E8 0x0000 0x1 0x0
#define MX6SX_PAD_QSPI1B_DATA0__UART3_DTE_RTS                     0x01A0 0x04E8 0x083C 0x1 0x4
>>>>>>> upstream/android-13
#define MX6SX_PAD_QSPI1B_DATA0__ECSPI3_MOSI                       0x01A0 0x04E8 0x0738 0x2 0x1
#define MX6SX_PAD_QSPI1B_DATA0__ESAI_RX_FS                        0x01A0 0x04E8 0x0778 0x3 0x2
#define MX6SX_PAD_QSPI1B_DATA0__CSI1_DATA_22                      0x01A0 0x04E8 0x06F4 0x4 0x1
#define MX6SX_PAD_QSPI1B_DATA0__GPIO4_IO_24                       0x01A0 0x04E8 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1B_DATA0__WEIM_DATA_14                      0x01A0 0x04E8 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1B_DATA0__SIM_M_HADDR_9                     0x01A0 0x04E8 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1B_DATA1__QSPI1_B_DATA_1                    0x01A4 0x04EC 0x0000 0x0 0x0
<<<<<<< HEAD
#define MX6SX_PAD_QSPI1B_DATA1__UART3_RTS_B                       0x01A4 0x04EC 0x083C 0x1 0x5
=======
#define MX6SX_PAD_QSPI1B_DATA1__UART3_DCE_RTS                     0x01A4 0x04EC 0x083C 0x1 0x5
#define MX6SX_PAD_QSPI1B_DATA1__UART3_DTE_CTS                     0x01A4 0x04EC 0x0000 0x1 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_QSPI1B_DATA1__ECSPI3_MISO                       0x01A4 0x04EC 0x0734 0x2 0x1
#define MX6SX_PAD_QSPI1B_DATA1__ESAI_RX_CLK                       0x01A4 0x04EC 0x0788 0x3 0x2
#define MX6SX_PAD_QSPI1B_DATA1__CSI1_DATA_21                      0x01A4 0x04EC 0x06F0 0x4 0x1
#define MX6SX_PAD_QSPI1B_DATA1__GPIO4_IO_25                       0x01A4 0x04EC 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1B_DATA1__WEIM_DATA_13                      0x01A4 0x04EC 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1B_DATA1__SIM_M_HADDR_8                     0x01A4 0x04EC 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1B_DATA2__QSPI1_B_DATA_2                    0x01A8 0x04F0 0x0000 0x0 0x0
#define MX6SX_PAD_QSPI1B_DATA2__I2C2_SDA                          0x01A8 0x04F0 0x07B4 0x1 0x2
#define MX6SX_PAD_QSPI1B_DATA2__ECSPI5_RDY                        0x01A8 0x04F0 0x0000 0x2 0x0
#define MX6SX_PAD_QSPI1B_DATA2__ESAI_TX5_RX0                      0x01A8 0x04F0 0x07A4 0x3 0x2
#define MX6SX_PAD_QSPI1B_DATA2__CSI1_DATA_20                      0x01A8 0x04F0 0x06EC 0x4 0x1
#define MX6SX_PAD_QSPI1B_DATA2__GPIO4_IO_26                       0x01A8 0x04F0 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1B_DATA2__WEIM_DATA_12                      0x01A8 0x04F0 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1B_DATA2__SIM_M_HADDR_5                     0x01A8 0x04F0 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1B_DATA3__QSPI1_B_DATA_3                    0x01AC 0x04F4 0x0000 0x0 0x0
#define MX6SX_PAD_QSPI1B_DATA3__I2C2_SCL                          0x01AC 0x04F4 0x07B0 0x1 0x2
#define MX6SX_PAD_QSPI1B_DATA3__ECSPI5_SS3                        0x01AC 0x04F4 0x0000 0x2 0x0
#define MX6SX_PAD_QSPI1B_DATA3__ESAI_TX_FS                        0x01AC 0x04F4 0x077C 0x3 0x2
#define MX6SX_PAD_QSPI1B_DATA3__CSI1_DATA_19                      0x01AC 0x04F4 0x06E8 0x4 0x1
#define MX6SX_PAD_QSPI1B_DATA3__GPIO4_IO_27                       0x01AC 0x04F4 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1B_DATA3__WEIM_DATA_11                      0x01AC 0x04F4 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1B_DATA3__SIM_M_HADDR_2                     0x01AC 0x04F4 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1B_DQS__QSPI1_B_DQS                         0x01B0 0x04F8 0x0000 0x0 0x0
#define MX6SX_PAD_QSPI1B_DQS__CAN1_TX                             0x01B0 0x04F8 0x0000 0x1 0x0
#define MX6SX_PAD_QSPI1B_DQS__CANFD_TX1                           0x01B0 0x04F8 0x0000 0x2 0x0
#define MX6SX_PAD_QSPI1B_DQS__ECSPI5_SS0                          0x01B0 0x04F8 0x075C 0x3 0x1
#define MX6SX_PAD_QSPI1B_DQS__CSI1_DATA_23                        0x01B0 0x04F8 0x06F8 0x4 0x1
#define MX6SX_PAD_QSPI1B_DQS__GPIO4_IO_28                         0x01B0 0x04F8 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1B_DQS__WEIM_DATA_15                        0x01B0 0x04F8 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1B_DQS__SIM_M_HADDR_15                      0x01B0 0x04F8 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1B_SCLK__QSPI1_B_SCLK                       0x01B4 0x04FC 0x0000 0x0 0x0
<<<<<<< HEAD
#define MX6SX_PAD_QSPI1B_SCLK__UART3_RX                           0x01B4 0x04FC 0x0840 0x1 0x4
#define MX6SX_PAD_QSPI1B_SCLK__UART3_TX                           0x01B4 0x04FC 0x0000 0x0 0x0
=======
#define MX6SX_PAD_QSPI1B_SCLK__UART3_DCE_RX                       0x01B4 0x04FC 0x0840 0x1 0x4
#define MX6SX_PAD_QSPI1B_SCLK__UART3_DTE_TX                       0x01B4 0x04FC 0x0000 0x1 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_QSPI1B_SCLK__ECSPI3_SCLK                        0x01B4 0x04FC 0x0730 0x2 0x1
#define MX6SX_PAD_QSPI1B_SCLK__ESAI_RX_HF_CLK                     0x01B4 0x04FC 0x0780 0x3 0x2
#define MX6SX_PAD_QSPI1B_SCLK__CSI1_DATA_16                       0x01B4 0x04FC 0x06DC 0x4 0x1
#define MX6SX_PAD_QSPI1B_SCLK__GPIO4_IO_29                        0x01B4 0x04FC 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1B_SCLK__WEIM_DATA_8                        0x01B4 0x04FC 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1B_SCLK__SIM_M_HADDR_11                     0x01B4 0x04FC 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1B_SS0_B__QSPI1_B_SS0_B                     0x01B8 0x0500 0x0000 0x0 0x0
<<<<<<< HEAD
#define MX6SX_PAD_QSPI1B_SS0_B__UART3_RX                          0x01B8 0x0500 0x0840 0x1 0x5
#define MX6SX_PAD_QSPI1B_SS0_B__UART3_TX                          0x01B8 0x0500 0x0000 0x1 0x0
=======
#define MX6SX_PAD_QSPI1B_SS0_B__UART3_DCE_TX                      0x01B8 0x0500 0x0000 0x1 0x0
#define MX6SX_PAD_QSPI1B_SS0_B__UART3_DTE_RX                      0x01B8 0x0500 0x0840 0x1 0x5
>>>>>>> upstream/android-13
#define MX6SX_PAD_QSPI1B_SS0_B__ECSPI3_SS0                        0x01B8 0x0500 0x073C 0x2 0x1
#define MX6SX_PAD_QSPI1B_SS0_B__ESAI_TX_HF_CLK                    0x01B8 0x0500 0x0784 0x3 0x3
#define MX6SX_PAD_QSPI1B_SS0_B__CSI1_DATA_17                      0x01B8 0x0500 0x06E0 0x4 0x1
#define MX6SX_PAD_QSPI1B_SS0_B__GPIO4_IO_30                       0x01B8 0x0500 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1B_SS0_B__WEIM_DATA_9                       0x01B8 0x0500 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1B_SS0_B__SIM_M_HADDR_10                    0x01B8 0x0500 0x0000 0x7 0x0
#define MX6SX_PAD_QSPI1B_SS1_B__QSPI1_B_SS1_B                     0x01BC 0x0504 0x0000 0x0 0x0
#define MX6SX_PAD_QSPI1B_SS1_B__CAN2_RX                           0x01BC 0x0504 0x0690 0x1 0x2
#define MX6SX_PAD_QSPI1B_SS1_B__CANFD_RX2                         0x01BC 0x0504 0x0698 0x2 0x2
#define MX6SX_PAD_QSPI1B_SS1_B__ECSPI5_SCLK                       0x01BC 0x0504 0x0750 0x3 0x1
#define MX6SX_PAD_QSPI1B_SS1_B__CSI1_DATA_18                      0x01BC 0x0504 0x06E4 0x4 0x1
#define MX6SX_PAD_QSPI1B_SS1_B__GPIO4_IO_31                       0x01BC 0x0504 0x0000 0x5 0x0
#define MX6SX_PAD_QSPI1B_SS1_B__WEIM_DATA_10                      0x01BC 0x0504 0x0000 0x6 0x0
#define MX6SX_PAD_QSPI1B_SS1_B__SIM_M_HADDR_14                    0x01BC 0x0504 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII1_RD0__ENET1_RX_DATA_0                     0x01C0 0x0508 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII1_RD0__GPIO5_IO_0                          0x01C0 0x0508 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII1_RD0__CSI2_DATA_10                        0x01C0 0x0508 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII1_RD0__ANATOP_TESTI_0                      0x01C0 0x0508 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII1_RD0__RAWNAND_TESTER_TRIGGER              0x01C0 0x0508 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII1_RD0__PCIE_CTRL_DEBUG_0                   0x01C0 0x0508 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII1_RD1__ENET1_RX_DATA_1                     0x01C4 0x050C 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII1_RD1__GPIO5_IO_1                          0x01C4 0x050C 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII1_RD1__CSI2_DATA_11                        0x01C4 0x050C 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII1_RD1__ANATOP_TESTI_1                      0x01C4 0x050C 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII1_RD1__USDHC1_TESTER_TRIGGER               0x01C4 0x050C 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII1_RD1__PCIE_CTRL_DEBUG_1                   0x01C4 0x050C 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII1_RD2__ENET1_RX_DATA_2                     0x01C8 0x0510 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII1_RD2__GPIO5_IO_2                          0x01C8 0x0510 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII1_RD2__CSI2_DATA_12                        0x01C8 0x0510 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII1_RD2__ANATOP_TESTI_2                      0x01C8 0x0510 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII1_RD2__USDHC2_TESTER_TRIGGER               0x01C8 0x0510 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII1_RD2__PCIE_CTRL_DEBUG_2                   0x01C8 0x0510 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII1_RD3__ENET1_RX_DATA_3                     0x01CC 0x0514 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII1_RD3__GPIO5_IO_3                          0x01CC 0x0514 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII1_RD3__CSI2_DATA_13                        0x01CC 0x0514 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII1_RD3__ANATOP_TESTI_3                      0x01CC 0x0514 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII1_RD3__USDHC3_TESTER_TRIGGER               0x01CC 0x0514 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII1_RD3__PCIE_CTRL_DEBUG_3                   0x01CC 0x0514 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII1_RX_CTL__ENET1_RX_EN                      0x01D0 0x0518 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII1_RX_CTL__GPIO5_IO_4                       0x01D0 0x0518 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII1_RX_CTL__CSI2_DATA_14                     0x01D0 0x0518 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII1_RX_CTL__ANATOP_TESTO_0                   0x01D0 0x0518 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII1_RX_CTL__USDHC4_TESTER_TRIGGER            0x01D0 0x0518 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII1_RX_CTL__PCIE_CTRL_DEBUG_4                0x01D0 0x0518 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII1_RXC__ENET1_RX_CLK                        0x01D4 0x051C 0x0768 0x0 0x1
#define MX6SX_PAD_RGMII1_RXC__ENET1_RX_ER                         0x01D4 0x051C 0x0000 0x1 0x0
#define MX6SX_PAD_RGMII1_RXC__GPIO5_IO_5                          0x01D4 0x051C 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII1_RXC__CSI2_DATA_15                        0x01D4 0x051C 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII1_RXC__ANATOP_TESTO_1                      0x01D4 0x051C 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII1_RXC__ECSPI1_TESTER_TRIGGER               0x01D4 0x051C 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII1_RXC__PCIE_CTRL_DEBUG_5                   0x01D4 0x051C 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII1_TD0__ENET1_TX_DATA_0                     0x01D8 0x0520 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII1_TD0__SAI2_RX_SYNC                        0x01D8 0x0520 0x0810 0x2 0x1
#define MX6SX_PAD_RGMII1_TD0__GPIO5_IO_6                          0x01D8 0x0520 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII1_TD0__CSI2_DATA_16                        0x01D8 0x0520 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII1_TD0__ANATOP_TESTO_2                      0x01D8 0x0520 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII1_TD0__ECSPI2_TESTER_TRIGGER               0x01D8 0x0520 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII1_TD0__PCIE_CTRL_DEBUG_6                   0x01D8 0x0520 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII1_TD1__ENET1_TX_DATA_1                     0x01DC 0x0524 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII1_TD1__SAI2_RX_BCLK                        0x01DC 0x0524 0x0808 0x2 0x1
#define MX6SX_PAD_RGMII1_TD1__GPIO5_IO_7                          0x01DC 0x0524 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII1_TD1__CSI2_DATA_17                        0x01DC 0x0524 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII1_TD1__ANATOP_TESTO_3                      0x01DC 0x0524 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII1_TD1__ECSPI3_TESTER_TRIGGER               0x01DC 0x0524 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII1_TD1__PCIE_CTRL_DEBUG_7                   0x01DC 0x0524 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII1_TD2__ENET1_TX_DATA_2                     0x01E0 0x0528 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII1_TD2__SAI2_TX_SYNC                        0x01E0 0x0528 0x0818 0x2 0x1
#define MX6SX_PAD_RGMII1_TD2__GPIO5_IO_8                          0x01E0 0x0528 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII1_TD2__CSI2_DATA_18                        0x01E0 0x0528 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII1_TD2__ANATOP_TESTO_4                      0x01E0 0x0528 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII1_TD2__ECSPI4_TESTER_TRIGGER               0x01E0 0x0528 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII1_TD2__PCIE_CTRL_DEBUG_8                   0x01E0 0x0528 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII1_TD3__ENET1_TX_DATA_3                     0x01E4 0x052C 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII1_TD3__SAI2_TX_BCLK                        0x01E4 0x052C 0x0814 0x2 0x1
#define MX6SX_PAD_RGMII1_TD3__GPIO5_IO_9                          0x01E4 0x052C 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII1_TD3__CSI2_DATA_19                        0x01E4 0x052C 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII1_TD3__ANATOP_TESTO_5                      0x01E4 0x052C 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII1_TD3__ECSPI5_TESTER_TRIGGER               0x01E4 0x052C 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII1_TD3__PCIE_CTRL_DEBUG_9                   0x01E4 0x052C 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII1_TX_CTL__ENET1_TX_EN                      0x01E8 0x0530 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII1_TX_CTL__SAI2_RX_DATA_0                   0x01E8 0x0530 0x080C 0x2 0x1
#define MX6SX_PAD_RGMII1_TX_CTL__GPIO5_IO_10                      0x01E8 0x0530 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII1_TX_CTL__CSI2_DATA_0                      0x01E8 0x0530 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII1_TX_CTL__ANATOP_TESTO_6                   0x01E8 0x0530 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII1_TX_CTL__QSPI1_TESTER_TRIGGER             0x01E8 0x0530 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII1_TX_CTL__PCIE_CTRL_DEBUG_10               0x01E8 0x0530 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII1_TXC__ENET1_RGMII_TXC                     0x01EC 0x0534 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII1_TXC__ENET1_TX_ER                         0x01EC 0x0534 0x0000 0x1 0x0
#define MX6SX_PAD_RGMII1_TXC__SAI2_TX_DATA_0                      0x01EC 0x0534 0x0000 0x2 0x0
#define MX6SX_PAD_RGMII1_TXC__GPIO5_IO_11                         0x01EC 0x0534 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII1_TXC__CSI2_DATA_1                         0x01EC 0x0534 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII1_TXC__ANATOP_TESTO_7                      0x01EC 0x0534 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII1_TXC__QSPI2_TESTER_TRIGGER                0x01EC 0x0534 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII1_TXC__PCIE_CTRL_DEBUG_11                  0x01EC 0x0534 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII2_RD0__ENET2_RX_DATA_0                     0x01F0 0x0538 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII2_RD0__PWM4_OUT                            0x01F0 0x0538 0x0000 0x2 0x0
#define MX6SX_PAD_RGMII2_RD0__GPIO5_IO_12                         0x01F0 0x0538 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII2_RD0__CSI2_DATA_2                         0x01F0 0x0538 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII2_RD0__ANATOP_TESTO_8                      0x01F0 0x0538 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII2_RD0__VDEC_DEBUG_18                       0x01F0 0x0538 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII2_RD0__PCIE_CTRL_DEBUG_12                  0x01F0 0x0538 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII2_RD1__ENET2_RX_DATA_1                     0x01F4 0x053C 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII2_RD1__PWM3_OUT                            0x01F4 0x053C 0x0000 0x2 0x0
#define MX6SX_PAD_RGMII2_RD1__GPIO5_IO_13                         0x01F4 0x053C 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII2_RD1__CSI2_DATA_3                         0x01F4 0x053C 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII2_RD1__ANATOP_TESTO_9                      0x01F4 0x053C 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII2_RD1__VDEC_DEBUG_19                       0x01F4 0x053C 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII2_RD1__PCIE_CTRL_DEBUG_13                  0x01F4 0x053C 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII2_RD2__ENET2_RX_DATA_2                     0x01F8 0x0540 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII2_RD2__PWM2_OUT                            0x01F8 0x0540 0x0000 0x2 0x0
#define MX6SX_PAD_RGMII2_RD2__GPIO5_IO_14                         0x01F8 0x0540 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII2_RD2__CSI2_DATA_4                         0x01F8 0x0540 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII2_RD2__ANATOP_TESTO_10                     0x01F8 0x0540 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII2_RD2__VDEC_DEBUG_20                       0x01F8 0x0540 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII2_RD2__PCIE_CTRL_DEBUG_14                  0x01F8 0x0540 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII2_RD3__ENET2_RX_DATA_3                     0x01FC 0x0544 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII2_RD3__PWM1_OUT                            0x01FC 0x0544 0x0000 0x2 0x0
#define MX6SX_PAD_RGMII2_RD3__GPIO5_IO_15                         0x01FC 0x0544 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII2_RD3__CSI2_DATA_5                         0x01FC 0x0544 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII2_RD3__ANATOP_TESTO_11                     0x01FC 0x0544 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII2_RD3__VDEC_DEBUG_21                       0x01FC 0x0544 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII2_RD3__PCIE_CTRL_DEBUG_15                  0x01FC 0x0544 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII2_RX_CTL__ENET2_RX_EN                      0x0200 0x0548 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII2_RX_CTL__GPIO5_IO_16                      0x0200 0x0548 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII2_RX_CTL__CSI2_DATA_6                      0x0200 0x0548 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII2_RX_CTL__ANATOP_TESTO_12                  0x0200 0x0548 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII2_RX_CTL__VDEC_DEBUG_22                    0x0200 0x0548 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII2_RX_CTL__PCIE_CTRL_DEBUG_16               0x0200 0x0548 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII2_RXC__ENET2_RX_CLK                        0x0204 0x054C 0x0774 0x0 0x1
#define MX6SX_PAD_RGMII2_RXC__ENET2_RX_ER                         0x0204 0x054C 0x0000 0x1 0x0
#define MX6SX_PAD_RGMII2_RXC__GPIO5_IO_17                         0x0204 0x054C 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII2_RXC__CSI2_DATA_7                         0x0204 0x054C 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII2_RXC__ANATOP_TESTO_13                     0x0204 0x054C 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII2_RXC__VDEC_DEBUG_23                       0x0204 0x054C 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII2_RXC__PCIE_CTRL_DEBUG_17                  0x0204 0x054C 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII2_TD0__ENET2_TX_DATA_0                     0x0208 0x0550 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII2_TD0__SAI1_RX_SYNC                        0x0208 0x0550 0x07FC 0x2 0x1
#define MX6SX_PAD_RGMII2_TD0__PWM8_OUT                            0x0208 0x0550 0x0000 0x3 0x0
#define MX6SX_PAD_RGMII2_TD0__GPIO5_IO_18                         0x0208 0x0550 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII2_TD0__CSI2_DATA_8                         0x0208 0x0550 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII2_TD0__ANATOP_TESTO_14                     0x0208 0x0550 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII2_TD0__VDEC_DEBUG_24                       0x0208 0x0550 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII2_TD0__PCIE_CTRL_DEBUG_18                  0x0208 0x0550 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII2_TD1__ENET2_TX_DATA_1                     0x020C 0x0554 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII2_TD1__SAI1_RX_BCLK                        0x020C 0x0554 0x07F4 0x2 0x1
#define MX6SX_PAD_RGMII2_TD1__PWM7_OUT                            0x020C 0x0554 0x0000 0x3 0x0
#define MX6SX_PAD_RGMII2_TD1__GPIO5_IO_19                         0x020C 0x0554 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII2_TD1__CSI2_DATA_9                         0x020C 0x0554 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII2_TD1__ANATOP_TESTO_15                     0x020C 0x0554 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII2_TD1__VDEC_DEBUG_25                       0x020C 0x0554 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII2_TD1__PCIE_CTRL_DEBUG_19                  0x020C 0x0554 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII2_TD2__ENET2_TX_DATA_2                     0x0210 0x0558 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII2_TD2__SAI1_TX_SYNC                        0x0210 0x0558 0x0804 0x2 0x1
#define MX6SX_PAD_RGMII2_TD2__PWM6_OUT                            0x0210 0x0558 0x0000 0x3 0x0
#define MX6SX_PAD_RGMII2_TD2__GPIO5_IO_20                         0x0210 0x0558 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII2_TD2__CSI2_VSYNC                          0x0210 0x0558 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII2_TD2__SJC_FAIL                            0x0210 0x0558 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII2_TD2__VDEC_DEBUG_26                       0x0210 0x0558 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII2_TD2__PCIE_CTRL_DEBUG_20                  0x0210 0x0558 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII2_TD3__ENET2_TX_DATA_3                     0x0214 0x055C 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII2_TD3__SAI1_TX_BCLK                        0x0214 0x055C 0x0800 0x2 0x1
#define MX6SX_PAD_RGMII2_TD3__PWM5_OUT                            0x0214 0x055C 0x0000 0x3 0x0
#define MX6SX_PAD_RGMII2_TD3__GPIO5_IO_21                         0x0214 0x055C 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII2_TD3__CSI2_HSYNC                          0x0214 0x055C 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII2_TD3__SJC_JTAG_ACT                        0x0214 0x055C 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII2_TD3__VDEC_DEBUG_27                       0x0214 0x055C 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII2_TD3__PCIE_CTRL_DEBUG_21                  0x0214 0x055C 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII2_TX_CTL__ENET2_TX_EN                      0x0218 0x0560 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII2_TX_CTL__SAI1_RX_DATA_0                   0x0218 0x0560 0x07F8 0x2 0x1
#define MX6SX_PAD_RGMII2_TX_CTL__GPIO5_IO_22                      0x0218 0x0560 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII2_TX_CTL__CSI2_FIELD                       0x0218 0x0560 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII2_TX_CTL__SJC_DE_B                         0x0218 0x0560 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII2_TX_CTL__VDEC_DEBUG_28                    0x0218 0x0560 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII2_TX_CTL__PCIE_CTRL_DEBUG_22               0x0218 0x0560 0x0000 0x9 0x0
#define MX6SX_PAD_RGMII2_TXC__ENET2_RGMII_TXC                     0x021C 0x0564 0x0000 0x0 0x0
#define MX6SX_PAD_RGMII2_TXC__ENET2_TX_ER                         0x021C 0x0564 0x0000 0x1 0x0
#define MX6SX_PAD_RGMII2_TXC__SAI1_TX_DATA_0                      0x021C 0x0564 0x0000 0x2 0x0
#define MX6SX_PAD_RGMII2_TXC__GPIO5_IO_23                         0x021C 0x0564 0x0000 0x5 0x0
#define MX6SX_PAD_RGMII2_TXC__CSI2_PIXCLK                         0x021C 0x0564 0x0000 0x6 0x0
#define MX6SX_PAD_RGMII2_TXC__SJC_DONE                            0x021C 0x0564 0x0000 0x7 0x0
#define MX6SX_PAD_RGMII2_TXC__VDEC_DEBUG_29                       0x021C 0x0564 0x0000 0x8 0x0
#define MX6SX_PAD_RGMII2_TXC__PCIE_CTRL_DEBUG_23                  0x021C 0x0564 0x0000 0x9 0x0
#define MX6SX_PAD_SD1_CLK__USDHC1_CLK                             0x0220 0x0568 0x0000 0x0 0x0
#define MX6SX_PAD_SD1_CLK__AUDMUX_AUD5_RXFS                       0x0220 0x0568 0x0668 0x1 0x1
#define MX6SX_PAD_SD1_CLK__WDOG2_WDOG_B                           0x0220 0x0568 0x0000 0x2 0x0
#define MX6SX_PAD_SD1_CLK__GPT_CLK                                0x0220 0x0568 0x0000 0x3 0x0
#define MX6SX_PAD_SD1_CLK__WDOG2_WDOG_RST_B_DEB                   0x0220 0x0568 0x0000 0x4 0x0
#define MX6SX_PAD_SD1_CLK__GPIO6_IO_0                             0x0220 0x0568 0x0000 0x5 0x0
#define MX6SX_PAD_SD1_CLK__ENET2_1588_EVENT1_OUT                  0x0220 0x0568 0x0000 0x6 0x0
#define MX6SX_PAD_SD1_CLK__CCM_OUT1                               0x0220 0x0568 0x0000 0x7 0x0
#define MX6SX_PAD_SD1_CLK__VADC_ADC_PROC_CLK                      0x0220 0x0568 0x0000 0x8 0x0
#define MX6SX_PAD_SD1_CLK__MMDC_DEBUG_45                          0x0220 0x0568 0x0000 0x9 0x0
#define MX6SX_PAD_SD1_CMD__USDHC1_CMD                             0x0224 0x056C 0x0000 0x0 0x0
#define MX6SX_PAD_SD1_CMD__AUDMUX_AUD5_RXC                        0x0224 0x056C 0x0664 0x1 0x1
#define MX6SX_PAD_SD1_CMD__WDOG1_WDOG_B                           0x0224 0x056C 0x0000 0x2 0x0
#define MX6SX_PAD_SD1_CMD__GPT_COMPARE1                           0x0224 0x056C 0x0000 0x3 0x0
#define MX6SX_PAD_SD1_CMD__WDOG1_WDOG_RST_B_DEB                   0x0224 0x056C 0x0000 0x4 0x0
#define MX6SX_PAD_SD1_CMD__GPIO6_IO_1                             0x0224 0x056C 0x0000 0x5 0x0
#define MX6SX_PAD_SD1_CMD__ENET2_1588_EVENT1_IN                   0x0224 0x056C 0x0000 0x6 0x0
#define MX6SX_PAD_SD1_CMD__CCM_CLKO1                              0x0224 0x056C 0x0000 0x7 0x0
#define MX6SX_PAD_SD1_CMD__VADC_EXT_SYSCLK                        0x0224 0x056C 0x0000 0x8 0x0
#define MX6SX_PAD_SD1_CMD__MMDC_DEBUG_46                          0x0224 0x056C 0x0000 0x9 0x0
#define MX6SX_PAD_SD1_DATA0__USDHC1_DATA0                         0x0228 0x0570 0x0000 0x0 0x0
#define MX6SX_PAD_SD1_DATA0__AUDMUX_AUD5_RXD                      0x0228 0x0570 0x065C 0x1 0x1
#define MX6SX_PAD_SD1_DATA0__CAAM_WRAPPER_RNG_OSC_OBS             0x0228 0x0570 0x0000 0x2 0x0
#define MX6SX_PAD_SD1_DATA0__GPT_CAPTURE1                         0x0228 0x0570 0x0000 0x3 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD1_DATA0__UART2_RX                             0x0228 0x0570 0x0838 0x4 0x2
#define MX6SX_PAD_SD1_DATA0__UART2_TX                             0x0228 0x0570 0x0000 0x4 0x0
=======
#define MX6SX_PAD_SD1_DATA0__UART2_DCE_RX                         0x0228 0x0570 0x0838 0x4 0x2
#define MX6SX_PAD_SD1_DATA0__UART2_DTE_TX                         0x0228 0x0570 0x0000 0x4 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD1_DATA0__GPIO6_IO_2                           0x0228 0x0570 0x0000 0x5 0x0
#define MX6SX_PAD_SD1_DATA0__ENET1_1588_EVENT1_IN                 0x0228 0x0570 0x0000 0x6 0x0
#define MX6SX_PAD_SD1_DATA0__CCM_OUT2                             0x0228 0x0570 0x0000 0x7 0x0
#define MX6SX_PAD_SD1_DATA0__VADC_CLAMP_UP                        0x0228 0x0570 0x0000 0x8 0x0
#define MX6SX_PAD_SD1_DATA0__MMDC_DEBUG_48                        0x0228 0x0570 0x0000 0x9 0x0
#define MX6SX_PAD_SD1_DATA1__USDHC1_DATA1                         0x022C 0x0574 0x0000 0x0 0x0
#define MX6SX_PAD_SD1_DATA1__AUDMUX_AUD5_TXC                      0x022C 0x0574 0x066C 0x1 0x1
#define MX6SX_PAD_SD1_DATA1__PWM4_OUT                             0x022C 0x0574 0x0000 0x2 0x0
#define MX6SX_PAD_SD1_DATA1__GPT_CAPTURE2                         0x022C 0x0574 0x0000 0x3 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD1_DATA1__UART2_RX                             0x022C 0x0574 0x0838 0x4 0x3
#define MX6SX_PAD_SD1_DATA1__UART2_TX                             0x022C 0x0574 0x0000 0x4 0x0
=======
#define MX6SX_PAD_SD1_DATA1__UART2_DCE_TX                         0x022C 0x0574 0x0000 0x4 0x0
#define MX6SX_PAD_SD1_DATA1__UART2_DTE_RX                         0x022C 0x0574 0x0838 0x4 0x3
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD1_DATA1__GPIO6_IO_3                           0x022C 0x0574 0x0000 0x5 0x0
#define MX6SX_PAD_SD1_DATA1__ENET1_1588_EVENT1_OUT                0x022C 0x0574 0x0000 0x6 0x0
#define MX6SX_PAD_SD1_DATA1__CCM_CLKO2                            0x022C 0x0574 0x0000 0x7 0x0
#define MX6SX_PAD_SD1_DATA1__VADC_CLAMP_DOWN                      0x022C 0x0574 0x0000 0x8 0x0
#define MX6SX_PAD_SD1_DATA1__MMDC_DEBUG_47                        0x022C 0x0574 0x0000 0x9 0x0
#define MX6SX_PAD_SD1_DATA2__USDHC1_DATA2                         0x0230 0x0578 0x0000 0x0 0x0
#define MX6SX_PAD_SD1_DATA2__AUDMUX_AUD5_TXFS                     0x0230 0x0578 0x0670 0x1 0x1
#define MX6SX_PAD_SD1_DATA2__PWM3_OUT                             0x0230 0x0578 0x0000 0x2 0x0
#define MX6SX_PAD_SD1_DATA2__GPT_COMPARE2                         0x0230 0x0578 0x0000 0x3 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD1_DATA2__UART2_CTS_B                          0x0230 0x0578 0x0000 0x4 0x0
=======
#define MX6SX_PAD_SD1_DATA2__UART2_DCE_CTS                        0x0230 0x0578 0x0000 0x4 0x0
#define MX6SX_PAD_SD1_DATA2__UART2_DTE_RTS                        0x0230 0x0578 0x0834 0x4 0x2
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD1_DATA2__GPIO6_IO_4                           0x0230 0x0578 0x0000 0x5 0x0
#define MX6SX_PAD_SD1_DATA2__ECSPI4_RDY                           0x0230 0x0578 0x0000 0x6 0x0
#define MX6SX_PAD_SD1_DATA2__CCM_OUT0                             0x0230 0x0578 0x0000 0x7 0x0
#define MX6SX_PAD_SD1_DATA2__VADC_EXT_PD_N                        0x0230 0x0578 0x0000 0x8 0x0
#define MX6SX_PAD_SD1_DATA3__USDHC1_DATA3                         0x0234 0x057C 0x0000 0x0 0x0
#define MX6SX_PAD_SD1_DATA3__AUDMUX_AUD5_TXD                      0x0234 0x057C 0x0660 0x1 0x1
#define MX6SX_PAD_SD1_DATA3__AUDMUX_AUD5_RXD                      0x0234 0x057C 0x065C 0x2 0x2
#define MX6SX_PAD_SD1_DATA3__GPT_COMPARE3                         0x0234 0x057C 0x0000 0x3 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD1_DATA3__UART2_RTS_B                          0x0234 0x057C 0x0834 0x4 0x3
=======
#define MX6SX_PAD_SD1_DATA3__UART2_DCE_RTS                        0x0234 0x057C 0x0834 0x4 0x3
#define MX6SX_PAD_SD1_DATA3__UART2_DTE_CTS                        0x0234 0x057C 0x0000 0x4 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD1_DATA3__GPIO6_IO_5                           0x0234 0x057C 0x0000 0x5 0x0
#define MX6SX_PAD_SD1_DATA3__ECSPI4_SS1                           0x0234 0x057C 0x0000 0x6 0x0
#define MX6SX_PAD_SD1_DATA3__CCM_PMIC_RDY                         0x0234 0x057C 0x069C 0x7 0x2
#define MX6SX_PAD_SD1_DATA3__VADC_RST_N                           0x0234 0x057C 0x0000 0x8 0x0
#define MX6SX_PAD_SD2_CLK__USDHC2_CLK                             0x0238 0x0580 0x0000 0x0 0x0
#define MX6SX_PAD_SD2_CLK__AUDMUX_AUD6_RXFS                       0x0238 0x0580 0x0680 0x1 0x2
#define MX6SX_PAD_SD2_CLK__KPP_COL_5                              0x0238 0x0580 0x07C8 0x2 0x1
#define MX6SX_PAD_SD2_CLK__ECSPI4_SCLK                            0x0238 0x0580 0x0740 0x3 0x1
#define MX6SX_PAD_SD2_CLK__MLB_SIG                                0x0238 0x0580 0x07F0 0x4 0x2
#define MX6SX_PAD_SD2_CLK__GPIO6_IO_6                             0x0238 0x0580 0x0000 0x5 0x0
#define MX6SX_PAD_SD2_CLK__MQS_RIGHT                              0x0238 0x0580 0x0000 0x6 0x0
#define MX6SX_PAD_SD2_CLK__WDOG1_WDOG_ANY                         0x0238 0x0580 0x0000 0x7 0x0
#define MX6SX_PAD_SD2_CLK__VADC_CLAMP_CURRENT_5                   0x0238 0x0580 0x0000 0x8 0x0
#define MX6SX_PAD_SD2_CLK__MMDC_DEBUG_29                          0x0238 0x0580 0x0000 0x9 0x0
#define MX6SX_PAD_SD2_CMD__USDHC2_CMD                             0x023C 0x0584 0x0000 0x0 0x0
#define MX6SX_PAD_SD2_CMD__AUDMUX_AUD6_RXC                        0x023C 0x0584 0x067C 0x1 0x2
#define MX6SX_PAD_SD2_CMD__KPP_ROW_5                              0x023C 0x0584 0x07D4 0x2 0x1
#define MX6SX_PAD_SD2_CMD__ECSPI4_MOSI                            0x023C 0x0584 0x0748 0x3 0x1
#define MX6SX_PAD_SD2_CMD__MLB_CLK                                0x023C 0x0584 0x07E8 0x4 0x2
#define MX6SX_PAD_SD2_CMD__GPIO6_IO_7                             0x023C 0x0584 0x0000 0x5 0x0
#define MX6SX_PAD_SD2_CMD__MQS_LEFT                               0x023C 0x0584 0x0000 0x6 0x0
#define MX6SX_PAD_SD2_CMD__WDOG3_WDOG_B                           0x023C 0x0584 0x0000 0x7 0x0
#define MX6SX_PAD_SD2_CMD__VADC_CLAMP_CURRENT_4                   0x023C 0x0584 0x0000 0x8 0x0
#define MX6SX_PAD_SD2_CMD__MMDC_DEBUG_30                          0x023C 0x0584 0x0000 0x9 0x0
#define MX6SX_PAD_SD2_DATA0__USDHC2_DATA0                         0x0240 0x0588 0x0000 0x0 0x0
#define MX6SX_PAD_SD2_DATA0__AUDMUX_AUD6_RXD                      0x0240 0x0588 0x0674 0x1 0x2
#define MX6SX_PAD_SD2_DATA0__KPP_ROW_7                            0x0240 0x0588 0x07DC 0x2 0x1
#define MX6SX_PAD_SD2_DATA0__PWM1_OUT                             0x0240 0x0588 0x0000 0x3 0x0
#define MX6SX_PAD_SD2_DATA0__I2C4_SDA                             0x0240 0x0588 0x07C4 0x4 0x3
#define MX6SX_PAD_SD2_DATA0__GPIO6_IO_8                           0x0240 0x0588 0x0000 0x5 0x0
#define MX6SX_PAD_SD2_DATA0__ECSPI4_SS3                           0x0240 0x0588 0x0000 0x6 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD2_DATA0__UART4_RX                             0x0240 0x0588 0x0848 0x7 0x4
#define MX6SX_PAD_SD2_DATA0__UART4_TX                             0x0240 0x0588 0x0000 0x7 0x0
=======
#define MX6SX_PAD_SD2_DATA0__UART4_DCE_RX                         0x0240 0x0588 0x0848 0x7 0x4
#define MX6SX_PAD_SD2_DATA0__UART4_DTE_TX                         0x0240 0x0588 0x0000 0x7 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD2_DATA0__VADC_CLAMP_CURRENT_0                 0x0240 0x0588 0x0000 0x8 0x0
#define MX6SX_PAD_SD2_DATA0__MMDC_DEBUG_50                        0x0240 0x0588 0x0000 0x9 0x0
#define MX6SX_PAD_SD2_DATA1__USDHC2_DATA1                         0x0244 0x058C 0x0000 0x0 0x0
#define MX6SX_PAD_SD2_DATA1__AUDMUX_AUD6_TXC                      0x0244 0x058C 0x0684 0x1 0x2
#define MX6SX_PAD_SD2_DATA1__KPP_COL_7                            0x0244 0x058C 0x07D0 0x2 0x1
#define MX6SX_PAD_SD2_DATA1__PWM2_OUT                             0x0244 0x058C 0x0000 0x3 0x0
#define MX6SX_PAD_SD2_DATA1__I2C4_SCL                             0x0244 0x058C 0x07C0 0x4 0x3
#define MX6SX_PAD_SD2_DATA1__GPIO6_IO_9                           0x0244 0x058C 0x0000 0x5 0x0
#define MX6SX_PAD_SD2_DATA1__ECSPI4_SS2                           0x0244 0x058C 0x0000 0x6 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD2_DATA1__UART4_RX                             0x0244 0x058C 0x0848 0x7 0x5
#define MX6SX_PAD_SD2_DATA1__UART4_TX                             0x0244 0x058C 0x0000 0x7 0x0
=======
#define MX6SX_PAD_SD2_DATA1__UART4_DCE_TX                         0x0244 0x058C 0x0000 0x7 0x0
#define MX6SX_PAD_SD2_DATA1__UART4_DTE_RX                         0x0244 0x058C 0x0848 0x7 0x5
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD2_DATA1__VADC_CLAMP_CURRENT_1                 0x0244 0x058C 0x0000 0x8 0x0
#define MX6SX_PAD_SD2_DATA1__MMDC_DEBUG_49                        0x0244 0x058C 0x0000 0x9 0x0
#define MX6SX_PAD_SD2_DATA2__USDHC2_DATA2                         0x0248 0x0590 0x0000 0x0 0x0
#define MX6SX_PAD_SD2_DATA2__AUDMUX_AUD6_TXFS                     0x0248 0x0590 0x0688 0x1 0x2
#define MX6SX_PAD_SD2_DATA2__KPP_ROW_6                            0x0248 0x0590 0x07D8 0x2 0x1
#define MX6SX_PAD_SD2_DATA2__ECSPI4_SS0                           0x0248 0x0590 0x074C 0x3 0x1
#define MX6SX_PAD_SD2_DATA2__SDMA_EXT_EVENT_0                     0x0248 0x0590 0x081C 0x4 0x2
#define MX6SX_PAD_SD2_DATA2__GPIO6_IO_10                          0x0248 0x0590 0x0000 0x5 0x0
#define MX6SX_PAD_SD2_DATA2__SPDIF_OUT                            0x0248 0x0590 0x0000 0x6 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD2_DATA2__UART6_RX                             0x0248 0x0590 0x0858 0x7 0x4
#define MX6SX_PAD_SD2_DATA2__UART6_TX                             0x0248 0x0590 0x0000 0x7 0x0
=======
#define MX6SX_PAD_SD2_DATA2__UART6_DCE_RX                         0x0248 0x0590 0x0858 0x7 0x4
#define MX6SX_PAD_SD2_DATA2__UART6_DTE_TX                         0x0248 0x0590 0x0000 0x7 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD2_DATA2__VADC_CLAMP_CURRENT_2                 0x0248 0x0590 0x0000 0x8 0x0
#define MX6SX_PAD_SD2_DATA2__MMDC_DEBUG_32                        0x0248 0x0590 0x0000 0x9 0x0
#define MX6SX_PAD_SD2_DATA3__USDHC2_DATA3                         0x024C 0x0594 0x0000 0x0 0x0
#define MX6SX_PAD_SD2_DATA3__AUDMUX_AUD6_TXD                      0x024C 0x0594 0x0678 0x1 0x2
#define MX6SX_PAD_SD2_DATA3__KPP_COL_6                            0x024C 0x0594 0x07CC 0x2 0x1
#define MX6SX_PAD_SD2_DATA3__ECSPI4_MISO                          0x024C 0x0594 0x0744 0x3 0x1
#define MX6SX_PAD_SD2_DATA3__MLB_DATA                             0x024C 0x0594 0x07EC 0x4 0x2
#define MX6SX_PAD_SD2_DATA3__GPIO6_IO_11                          0x024C 0x0594 0x0000 0x5 0x0
#define MX6SX_PAD_SD2_DATA3__SPDIF_IN                             0x024C 0x0594 0x0824 0x6 0x4
<<<<<<< HEAD
#define MX6SX_PAD_SD2_DATA3__UART6_RX                             0x024C 0x0594 0x0858 0x7 0x5
#define MX6SX_PAD_SD2_DATA3__UART6_TX                             0x024C 0x0594 0x0000 0x7 0x0
#define MX6SX_PAD_SD2_DATA3__VADC_CLAMP_CURRENT_3                 0x024C 0x0594 0x0000 0x8 0x0
#define MX6SX_PAD_SD2_DATA3__MMDC_DEBUG_31                        0x024C 0x0594 0x0000 0x9 0x0
#define MX6SX_PAD_SD3_CLK__USDHC3_CLK                             0x0250 0x0598 0x0000 0x0 0x0
#define MX6SX_PAD_SD3_CLK__UART4_CTS_B                            0x0250 0x0598 0x0000 0x1 0x0
=======
#define MX6SX_PAD_SD2_DATA3__UART6_DCE_TX                         0x024C 0x0594 0x0000 0x7 0x0
#define MX6SX_PAD_SD2_DATA3__UART6_DTE_RX                         0x024C 0x0594 0x0858 0x7 0x5
#define MX6SX_PAD_SD2_DATA3__VADC_CLAMP_CURRENT_3                 0x024C 0x0594 0x0000 0x8 0x0
#define MX6SX_PAD_SD2_DATA3__MMDC_DEBUG_31                        0x024C 0x0594 0x0000 0x9 0x0
#define MX6SX_PAD_SD3_CLK__USDHC3_CLK                             0x0250 0x0598 0x0000 0x0 0x0
#define MX6SX_PAD_SD3_CLK__UART4_DCE_CTS                          0x0250 0x0598 0x0000 0x1 0x0
#define MX6SX_PAD_SD3_CLK__UART4_DTE_RTS                          0x0250 0x0598 0x0844 0x1 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD3_CLK__ECSPI4_SCLK                            0x0250 0x0598 0x0740 0x2 0x0
#define MX6SX_PAD_SD3_CLK__AUDMUX_AUD6_RXFS                       0x0250 0x0598 0x0680 0x3 0x0
#define MX6SX_PAD_SD3_CLK__LCDIF2_VSYNC                           0x0250 0x0598 0x0000 0x4 0x0
#define MX6SX_PAD_SD3_CLK__GPIO7_IO_0                             0x0250 0x0598 0x0000 0x5 0x0
#define MX6SX_PAD_SD3_CLK__LCDIF2_BUSY                            0x0250 0x0598 0x07E4 0x6 0x0
#define MX6SX_PAD_SD3_CLK__TPSMP_HDATA_29                         0x0250 0x0598 0x0000 0x7 0x0
#define MX6SX_PAD_SD3_CLK__SDMA_DEBUG_EVENT_CHANNEL_5             0x0250 0x0598 0x0000 0x9 0x0
#define MX6SX_PAD_SD3_CMD__USDHC3_CMD                             0x0254 0x059C 0x0000 0x0 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD3_CMD__UART4_RX                               0x0254 0x059C 0x0848 0x1 0x0
#define MX6SX_PAD_SD3_CMD__UART4_TX                               0x0254 0x059C 0x0000 0x1 0x0
=======
#define MX6SX_PAD_SD3_CMD__UART4_DCE_TX                           0x0254 0x059C 0x0000 0x1 0x0
#define MX6SX_PAD_SD3_CMD__UART4_DTE_RX                           0x0254 0x059C 0x0848 0x1 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD3_CMD__ECSPI4_MOSI                            0x0254 0x059C 0x0748 0x2 0x0
#define MX6SX_PAD_SD3_CMD__AUDMUX_AUD6_RXC                        0x0254 0x059C 0x067C 0x3 0x0
#define MX6SX_PAD_SD3_CMD__LCDIF2_HSYNC                           0x0254 0x059C 0x07E4 0x4 0x1
#define MX6SX_PAD_SD3_CMD__GPIO7_IO_1                             0x0254 0x059C 0x0000 0x5 0x0
#define MX6SX_PAD_SD3_CMD__LCDIF2_RS                              0x0254 0x059C 0x0000 0x6 0x0
#define MX6SX_PAD_SD3_CMD__TPSMP_HDATA_28                         0x0254 0x059C 0x0000 0x7 0x0
#define MX6SX_PAD_SD3_CMD__SDMA_DEBUG_EVENT_CHANNEL_4             0x0254 0x059C 0x0000 0x9 0x0
#define MX6SX_PAD_SD3_DATA0__USDHC3_DATA0                         0x0258 0x05A0 0x0000 0x0 0x0
#define MX6SX_PAD_SD3_DATA0__I2C4_SCL                             0x0258 0x05A0 0x07C0 0x1 0x0
#define MX6SX_PAD_SD3_DATA0__ECSPI2_SS1                           0x0258 0x05A0 0x0000 0x2 0x0
#define MX6SX_PAD_SD3_DATA0__AUDMUX_AUD6_RXD                      0x0258 0x05A0 0x0674 0x3 0x0
#define MX6SX_PAD_SD3_DATA0__LCDIF2_DATA_1                        0x0258 0x05A0 0x0000 0x4 0x0
#define MX6SX_PAD_SD3_DATA0__GPIO7_IO_2                           0x0258 0x05A0 0x0000 0x5 0x0
#define MX6SX_PAD_SD3_DATA0__DCIC1_OUT                            0x0258 0x05A0 0x0000 0x6 0x0
#define MX6SX_PAD_SD3_DATA0__TPSMP_HDATA_30                       0x0258 0x05A0 0x0000 0x7 0x0
#define MX6SX_PAD_SD3_DATA0__GPU_DEBUG_0                          0x0258 0x05A0 0x0000 0x8 0x0
#define MX6SX_PAD_SD3_DATA0__SDMA_DEBUG_EVT_CHN_LINES_0           0x0258 0x05A0 0x0000 0x9 0x0
#define MX6SX_PAD_SD3_DATA1__USDHC3_DATA1                         0x025C 0x05A4 0x0000 0x0 0x0
#define MX6SX_PAD_SD3_DATA1__I2C4_SDA                             0x025C 0x05A4 0x07C4 0x1 0x0
#define MX6SX_PAD_SD3_DATA1__ECSPI2_SS2                           0x025C 0x05A4 0x0000 0x2 0x0
#define MX6SX_PAD_SD3_DATA1__AUDMUX_AUD6_TXC                      0x025C 0x05A4 0x0684 0x3 0x0
#define MX6SX_PAD_SD3_DATA1__LCDIF2_DATA_0                        0x025C 0x05A4 0x0000 0x4 0x0
#define MX6SX_PAD_SD3_DATA1__GPIO7_IO_3                           0x025C 0x05A4 0x0000 0x5 0x0
#define MX6SX_PAD_SD3_DATA1__DCIC2_OUT                            0x025C 0x05A4 0x0000 0x6 0x0
#define MX6SX_PAD_SD3_DATA1__TPSMP_HDATA_31                       0x025C 0x05A4 0x0000 0x7 0x0
#define MX6SX_PAD_SD3_DATA1__GPU_DEBUG_1                          0x025C 0x05A4 0x0000 0x8 0x0
#define MX6SX_PAD_SD3_DATA1__SDMA_DEBUG_EVT_CHN_LINES_1           0x025C 0x05A4 0x0000 0x9 0x0
#define MX6SX_PAD_SD3_DATA2__USDHC3_DATA2                         0x0260 0x05A8 0x0000 0x0 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD3_DATA2__UART4_RTS_B                          0x0260 0x05A8 0x0844 0x1 0x1
=======
#define MX6SX_PAD_SD3_DATA2__UART4_DCE_RTS                        0x0260 0x05A8 0x0844 0x1 0x1
#define MX6SX_PAD_SD3_DATA2__UART4_DTE_CTS                        0x0260 0x05A8 0x0000 0x1 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD3_DATA2__ECSPI4_SS0                           0x0260 0x05A8 0x074C 0x2 0x0
#define MX6SX_PAD_SD3_DATA2__AUDMUX_AUD6_TXFS                     0x0260 0x05A8 0x0688 0x3 0x0
#define MX6SX_PAD_SD3_DATA2__LCDIF2_CLK                           0x0260 0x05A8 0x0000 0x4 0x0
#define MX6SX_PAD_SD3_DATA2__GPIO7_IO_4                           0x0260 0x05A8 0x0000 0x5 0x0
#define MX6SX_PAD_SD3_DATA2__LCDIF2_WR_RWN                        0x0260 0x05A8 0x0000 0x6 0x0
#define MX6SX_PAD_SD3_DATA2__TPSMP_HDATA_26                       0x0260 0x05A8 0x0000 0x7 0x0
#define MX6SX_PAD_SD3_DATA2__GPU_DEBUG_2                          0x0260 0x05A8 0x0000 0x8 0x0
#define MX6SX_PAD_SD3_DATA2__SDMA_DEBUG_EVENT_CHANNEL_2           0x0260 0x05A8 0x0000 0x9 0x0
#define MX6SX_PAD_SD3_DATA3__USDHC3_DATA3                         0x0264 0x05AC 0x0000 0x0 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD3_DATA3__UART4_RX                             0x0264 0x05AC 0x0848 0x1 0x1
#define MX6SX_PAD_SD3_DATA3__UART4_TX                             0x0264 0x05AC 0x0000 0x1 0x0
=======
#define MX6SX_PAD_SD3_DATA3__UART4_DCE_RX                         0x0264 0x05AC 0x0848 0x1 0x1
#define MX6SX_PAD_SD3_DATA3__UART4_DTE_TX                         0x0264 0x05AC 0x0000 0x1 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD3_DATA3__ECSPI4_MISO                          0x0264 0x05AC 0x0744 0x2 0x0
#define MX6SX_PAD_SD3_DATA3__AUDMUX_AUD6_TXD                      0x0264 0x05AC 0x0678 0x3 0x0
#define MX6SX_PAD_SD3_DATA3__LCDIF2_ENABLE                        0x0264 0x05AC 0x0000 0x4 0x0
#define MX6SX_PAD_SD3_DATA3__GPIO7_IO_5                           0x0264 0x05AC 0x0000 0x5 0x0
#define MX6SX_PAD_SD3_DATA3__LCDIF2_RD_E                          0x0264 0x05AC 0x0000 0x6 0x0
#define MX6SX_PAD_SD3_DATA3__TPSMP_HDATA_27                       0x0264 0x05AC 0x0000 0x7 0x0
#define MX6SX_PAD_SD3_DATA3__GPU_DEBUG_3                          0x0264 0x05AC 0x0000 0x8 0x0
#define MX6SX_PAD_SD3_DATA3__SDMA_DEBUG_EVENT_CHANNEL_3           0x0264 0x05AC 0x0000 0x9 0x0
#define MX6SX_PAD_SD3_DATA4__USDHC3_DATA4                         0x0268 0x05B0 0x0000 0x0 0x0
#define MX6SX_PAD_SD3_DATA4__CAN2_RX                              0x0268 0x05B0 0x0690 0x1 0x0
#define MX6SX_PAD_SD3_DATA4__CANFD_RX2                            0x0268 0x05B0 0x0698 0x2 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD3_DATA4__UART3_RX                             0x0268 0x05B0 0x0840 0x3 0x2
#define MX6SX_PAD_SD3_DATA4__UART3_TX                             0x0268 0x05B0 0x0000 0x3 0x0
=======
#define MX6SX_PAD_SD3_DATA4__UART3_DCE_RX                         0x0268 0x05B0 0x0840 0x3 0x2
#define MX6SX_PAD_SD3_DATA4__UART3_DTE_TX                         0x0268 0x05B0 0x0000 0x3 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD3_DATA4__LCDIF2_DATA_3                        0x0268 0x05B0 0x0000 0x4 0x0
#define MX6SX_PAD_SD3_DATA4__GPIO7_IO_6                           0x0268 0x05B0 0x0000 0x5 0x0
#define MX6SX_PAD_SD3_DATA4__ENET2_1588_EVENT0_IN                 0x0268 0x05B0 0x0000 0x6 0x0
#define MX6SX_PAD_SD3_DATA4__TPSMP_HTRANS_1                       0x0268 0x05B0 0x0000 0x7 0x0
#define MX6SX_PAD_SD3_DATA4__GPU_DEBUG_4                          0x0268 0x05B0 0x0000 0x8 0x0
#define MX6SX_PAD_SD3_DATA4__SDMA_DEBUG_BUS_DEVICE_0              0x0268 0x05B0 0x0000 0x9 0x0
#define MX6SX_PAD_SD3_DATA5__USDHC3_DATA5                         0x026C 0x05B4 0x0000 0x0 0x0
#define MX6SX_PAD_SD3_DATA5__CAN1_TX                              0x026C 0x05B4 0x0000 0x1 0x0
#define MX6SX_PAD_SD3_DATA5__CANFD_TX1                            0x026C 0x05B4 0x0000 0x2 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD3_DATA5__UART3_RX                             0x026C 0x05B4 0x0840 0x3 0x3
#define MX6SX_PAD_SD3_DATA5__UART3_TX                             0x026C 0x05B4 0x0000 0x3 0x0
=======
#define MX6SX_PAD_SD3_DATA5__UART3_DCE_TX                         0x026C 0x05B4 0x0000 0x3 0x0
#define MX6SX_PAD_SD3_DATA5__UART3_DTE_RX                         0x026C 0x05B4 0x0840 0x3 0x3
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD3_DATA5__LCDIF2_DATA_2                        0x026C 0x05B4 0x0000 0x4 0x0
#define MX6SX_PAD_SD3_DATA5__GPIO7_IO_7                           0x026C 0x05B4 0x0000 0x5 0x0
#define MX6SX_PAD_SD3_DATA5__ENET2_1588_EVENT0_OUT                0x026C 0x05B4 0x0000 0x6 0x0
#define MX6SX_PAD_SD3_DATA5__SIM_M_HWRITE                         0x026C 0x05B4 0x0000 0x7 0x0
#define MX6SX_PAD_SD3_DATA5__GPU_DEBUG_5                          0x026C 0x05B4 0x0000 0x8 0x0
#define MX6SX_PAD_SD3_DATA5__SDMA_DEBUG_BUS_DEVICE_1              0x026C 0x05B4 0x0000 0x9 0x0
#define MX6SX_PAD_SD3_DATA6__USDHC3_DATA6                         0x0270 0x05B8 0x0000 0x0 0x0
#define MX6SX_PAD_SD3_DATA6__CAN2_TX                              0x0270 0x05B8 0x0000 0x1 0x0
#define MX6SX_PAD_SD3_DATA6__CANFD_TX2                            0x0270 0x05B8 0x0000 0x2 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD3_DATA6__UART3_RTS_B                          0x0270 0x05B8 0x083C 0x3 0x2
=======
#define MX6SX_PAD_SD3_DATA6__UART3_DCE_RTS                        0x0270 0x05B8 0x083C 0x3 0x2
#define MX6SX_PAD_SD3_DATA6__UART3_DTE_CTS                        0x0270 0x05B8 0x0000 0x3 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD3_DATA6__LCDIF2_DATA_4                        0x0270 0x05B8 0x0000 0x4 0x0
#define MX6SX_PAD_SD3_DATA6__GPIO7_IO_8                           0x0270 0x05B8 0x0000 0x5 0x0
#define MX6SX_PAD_SD3_DATA6__ENET1_1588_EVENT0_OUT                0x0270 0x05B8 0x0000 0x6 0x0
#define MX6SX_PAD_SD3_DATA6__TPSMP_HTRANS_0                       0x0270 0x05B8 0x0000 0x7 0x0
#define MX6SX_PAD_SD3_DATA6__GPU_DEBUG_7                          0x0270 0x05B8 0x0000 0x8 0x0
#define MX6SX_PAD_SD3_DATA6__SDMA_DEBUG_EVT_CHN_LINES_7           0x0270 0x05B8 0x0000 0x9 0x0
#define MX6SX_PAD_SD3_DATA7__USDHC3_DATA7                         0x0274 0x05BC 0x0000 0x0 0x0
#define MX6SX_PAD_SD3_DATA7__CAN1_RX                              0x0274 0x05BC 0x068C 0x1 0x0
#define MX6SX_PAD_SD3_DATA7__CANFD_RX1                            0x0274 0x05BC 0x0694 0x2 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD3_DATA7__UART3_CTS_B                          0x0274 0x05BC 0x0000 0x3 0x0
=======
#define MX6SX_PAD_SD3_DATA7__UART3_DCE_CTS                        0x0274 0x05BC 0x0000 0x3 0x0
#define MX6SX_PAD_SD3_DATA7__UART3_DTE_RTS                        0x0274 0x05BC 0x083C 0x3 0x3
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD3_DATA7__LCDIF2_DATA_5                        0x0274 0x05BC 0x0000 0x4 0x0
#define MX6SX_PAD_SD3_DATA7__GPIO7_IO_9                           0x0274 0x05BC 0x0000 0x5 0x0
#define MX6SX_PAD_SD3_DATA7__ENET1_1588_EVENT0_IN                 0x0274 0x05BC 0x0000 0x6 0x0
#define MX6SX_PAD_SD3_DATA7__TPSMP_HDATA_DIR                      0x0274 0x05BC 0x0000 0x7 0x0
#define MX6SX_PAD_SD3_DATA7__GPU_DEBUG_6                          0x0274 0x05BC 0x0000 0x8 0x0
#define MX6SX_PAD_SD3_DATA7__SDMA_DEBUG_EVT_CHN_LINES_2           0x0274 0x05BC 0x0000 0x9 0x0
#define MX6SX_PAD_SD4_CLK__USDHC4_CLK                             0x0278 0x05C0 0x0000 0x0 0x0
#define MX6SX_PAD_SD4_CLK__RAWNAND_DATA15                         0x0278 0x05C0 0x0000 0x1 0x0
#define MX6SX_PAD_SD4_CLK__ECSPI2_MISO                            0x0278 0x05C0 0x0724 0x2 0x1
#define MX6SX_PAD_SD4_CLK__AUDMUX_AUD3_RXFS                       0x0278 0x05C0 0x0638 0x3 0x0
#define MX6SX_PAD_SD4_CLK__LCDIF2_DATA_13                         0x0278 0x05C0 0x0000 0x4 0x0
#define MX6SX_PAD_SD4_CLK__GPIO6_IO_12                            0x0278 0x05C0 0x0000 0x5 0x0
#define MX6SX_PAD_SD4_CLK__ECSPI3_SS2                             0x0278 0x05C0 0x0000 0x6 0x0
#define MX6SX_PAD_SD4_CLK__TPSMP_HDATA_20                         0x0278 0x05C0 0x0000 0x7 0x0
#define MX6SX_PAD_SD4_CLK__VDEC_DEBUG_12                          0x0278 0x05C0 0x0000 0x8 0x0
#define MX6SX_PAD_SD4_CLK__SDMA_DEBUG_EVENT_CHANNEL_SEL           0x0278 0x05C0 0x0000 0x9 0x0
#define MX6SX_PAD_SD4_CMD__USDHC4_CMD                             0x027C 0x05C4 0x0000 0x0 0x0
#define MX6SX_PAD_SD4_CMD__RAWNAND_DATA14                         0x027C 0x05C4 0x0000 0x1 0x0
#define MX6SX_PAD_SD4_CMD__ECSPI2_MOSI                            0x027C 0x05C4 0x0728 0x2 0x1
#define MX6SX_PAD_SD4_CMD__AUDMUX_AUD3_RXC                        0x027C 0x05C4 0x0634 0x3 0x0
#define MX6SX_PAD_SD4_CMD__LCDIF2_DATA_14                         0x027C 0x05C4 0x0000 0x4 0x0
#define MX6SX_PAD_SD4_CMD__GPIO6_IO_13                            0x027C 0x05C4 0x0000 0x5 0x0
#define MX6SX_PAD_SD4_CMD__ECSPI3_SS1                             0x027C 0x05C4 0x0000 0x6 0x0
#define MX6SX_PAD_SD4_CMD__TPSMP_HDATA_19                         0x027C 0x05C4 0x0000 0x7 0x0
#define MX6SX_PAD_SD4_CMD__VDEC_DEBUG_11                          0x027C 0x05C4 0x0000 0x8 0x0
#define MX6SX_PAD_SD4_CMD__SDMA_DEBUG_CORE_RUN                    0x027C 0x05C4 0x0000 0x9 0x0
#define MX6SX_PAD_SD4_DATA0__USDHC4_DATA0                         0x0280 0x05C8 0x0000 0x0 0x0
#define MX6SX_PAD_SD4_DATA0__RAWNAND_DATA10                       0x0280 0x05C8 0x0000 0x1 0x0
#define MX6SX_PAD_SD4_DATA0__ECSPI2_SS0                           0x0280 0x05C8 0x072C 0x2 0x1
#define MX6SX_PAD_SD4_DATA0__AUDMUX_AUD3_RXD                      0x0280 0x05C8 0x062C 0x3 0x0
#define MX6SX_PAD_SD4_DATA0__LCDIF2_DATA_12                       0x0280 0x05C8 0x0000 0x4 0x0
#define MX6SX_PAD_SD4_DATA0__GPIO6_IO_14                          0x0280 0x05C8 0x0000 0x5 0x0
#define MX6SX_PAD_SD4_DATA0__ECSPI3_SS3                           0x0280 0x05C8 0x0000 0x6 0x0
#define MX6SX_PAD_SD4_DATA0__TPSMP_HDATA_21                       0x0280 0x05C8 0x0000 0x7 0x0
#define MX6SX_PAD_SD4_DATA0__VDEC_DEBUG_13                        0x0280 0x05C8 0x0000 0x8 0x0
#define MX6SX_PAD_SD4_DATA0__SDMA_DEBUG_MODE                      0x0280 0x05C8 0x0000 0x9 0x0
#define MX6SX_PAD_SD4_DATA1__USDHC4_DATA1                         0x0284 0x05CC 0x0000 0x0 0x0
#define MX6SX_PAD_SD4_DATA1__RAWNAND_DATA11                       0x0284 0x05CC 0x0000 0x1 0x0
#define MX6SX_PAD_SD4_DATA1__ECSPI2_SCLK                          0x0284 0x05CC 0x0720 0x2 0x1
#define MX6SX_PAD_SD4_DATA1__AUDMUX_AUD3_TXC                      0x0284 0x05CC 0x063C 0x3 0x0
#define MX6SX_PAD_SD4_DATA1__LCDIF2_DATA_11                       0x0284 0x05CC 0x0000 0x4 0x0
#define MX6SX_PAD_SD4_DATA1__GPIO6_IO_15                          0x0284 0x05CC 0x0000 0x5 0x0
#define MX6SX_PAD_SD4_DATA1__ECSPI3_RDY                           0x0284 0x05CC 0x0000 0x6 0x0
#define MX6SX_PAD_SD4_DATA1__TPSMP_HDATA_22                       0x0284 0x05CC 0x0000 0x7 0x0
#define MX6SX_PAD_SD4_DATA1__VDEC_DEBUG_14                        0x0284 0x05CC 0x0000 0x8 0x0
#define MX6SX_PAD_SD4_DATA1__SDMA_DEBUG_BUS_ERROR                 0x0284 0x05CC 0x0000 0x9 0x0
#define MX6SX_PAD_SD4_DATA2__USDHC4_DATA2                         0x0288 0x05D0 0x0000 0x0 0x0
#define MX6SX_PAD_SD4_DATA2__RAWNAND_DATA12                       0x0288 0x05D0 0x0000 0x1 0x0
#define MX6SX_PAD_SD4_DATA2__I2C2_SDA                             0x0288 0x05D0 0x07B4 0x2 0x0
#define MX6SX_PAD_SD4_DATA2__AUDMUX_AUD3_TXFS                     0x0288 0x05D0 0x0640 0x3 0x0
#define MX6SX_PAD_SD4_DATA2__LCDIF2_DATA_10                       0x0288 0x05D0 0x0000 0x4 0x0
#define MX6SX_PAD_SD4_DATA2__GPIO6_IO_16                          0x0288 0x05D0 0x0000 0x5 0x0
#define MX6SX_PAD_SD4_DATA2__ECSPI2_SS3                           0x0288 0x05D0 0x0000 0x6 0x0
#define MX6SX_PAD_SD4_DATA2__TPSMP_HDATA_23                       0x0288 0x05D0 0x0000 0x7 0x0
#define MX6SX_PAD_SD4_DATA2__VDEC_DEBUG_15                        0x0288 0x05D0 0x0000 0x8 0x0
#define MX6SX_PAD_SD4_DATA2__SDMA_DEBUG_BUS_RWB                   0x0288 0x05D0 0x0000 0x9 0x0
#define MX6SX_PAD_SD4_DATA3__USDHC4_DATA3                         0x028C 0x05D4 0x0000 0x0 0x0
#define MX6SX_PAD_SD4_DATA3__RAWNAND_DATA13                       0x028C 0x05D4 0x0000 0x1 0x0
#define MX6SX_PAD_SD4_DATA3__I2C2_SCL                             0x028C 0x05D4 0x07B0 0x2 0x0
#define MX6SX_PAD_SD4_DATA3__AUDMUX_AUD3_TXD                      0x028C 0x05D4 0x0630 0x3 0x0
#define MX6SX_PAD_SD4_DATA3__LCDIF2_DATA_9                        0x028C 0x05D4 0x0000 0x4 0x0
#define MX6SX_PAD_SD4_DATA3__GPIO6_IO_17                          0x028C 0x05D4 0x0000 0x5 0x0
#define MX6SX_PAD_SD4_DATA3__ECSPI2_RDY                           0x028C 0x05D4 0x0000 0x6 0x0
#define MX6SX_PAD_SD4_DATA3__TPSMP_HDATA_24                       0x028C 0x05D4 0x0000 0x7 0x0
#define MX6SX_PAD_SD4_DATA3__VDEC_DEBUG_16                        0x028C 0x05D4 0x0000 0x8 0x0
#define MX6SX_PAD_SD4_DATA3__SDMA_DEBUG_MATCHED_DMBUS             0x028C 0x05D4 0x0000 0x9 0x0
#define MX6SX_PAD_SD4_DATA4__USDHC4_DATA4                         0x0290 0x05D8 0x0000 0x0 0x0
#define MX6SX_PAD_SD4_DATA4__RAWNAND_DATA09                       0x0290 0x05D8 0x0000 0x1 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD4_DATA4__UART5_RX                             0x0290 0x05D8 0x0850 0x2 0x0
#define MX6SX_PAD_SD4_DATA4__UART5_TX                             0x0290 0x05D8 0x0000 0x2 0x0
=======
#define MX6SX_PAD_SD4_DATA4__UART5_DCE_RX                         0x0290 0x05D8 0x0850 0x2 0x0
#define MX6SX_PAD_SD4_DATA4__UART5_DTE_TX                         0x0290 0x05D8 0x0000 0x2 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD4_DATA4__ECSPI3_SCLK                          0x0290 0x05D8 0x0730 0x3 0x0
#define MX6SX_PAD_SD4_DATA4__LCDIF2_DATA_8                        0x0290 0x05D8 0x0000 0x4 0x0
#define MX6SX_PAD_SD4_DATA4__GPIO6_IO_18                          0x0290 0x05D8 0x0000 0x5 0x0
#define MX6SX_PAD_SD4_DATA4__SPDIF_OUT                            0x0290 0x05D8 0x0000 0x6 0x0
#define MX6SX_PAD_SD4_DATA4__TPSMP_HDATA_16                       0x0290 0x05D8 0x0000 0x7 0x0
#define MX6SX_PAD_SD4_DATA4__USB_OTG_HOST_MODE                    0x0290 0x05D8 0x0000 0x8 0x0
#define MX6SX_PAD_SD4_DATA4__SDMA_DEBUG_RTBUFFER_WRITE            0x0290 0x05D8 0x0000 0x9 0x0
#define MX6SX_PAD_SD4_DATA5__USDHC4_DATA5                         0x0294 0x05DC 0x0000 0x0 0x0
#define MX6SX_PAD_SD4_DATA5__RAWNAND_CE2_B                        0x0294 0x05DC 0x0000 0x1 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD4_DATA5__UART5_RX                             0x0294 0x05DC 0x0850 0x2 0x1
#define MX6SX_PAD_SD4_DATA5__UART5_TX                             0x0294 0x05DC 0x0000 0x2 0x0
=======
#define MX6SX_PAD_SD4_DATA5__UART5_DCE_TX                         0x0294 0x05DC 0x0000 0x2 0x0
#define MX6SX_PAD_SD4_DATA5__UART5_DTE_RX                         0x0294 0x05DC 0x0850 0x2 0x1
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD4_DATA5__ECSPI3_MOSI                          0x0294 0x05DC 0x0738 0x3 0x0
#define MX6SX_PAD_SD4_DATA5__LCDIF2_DATA_7                        0x0294 0x05DC 0x0000 0x4 0x0
#define MX6SX_PAD_SD4_DATA5__GPIO6_IO_19                          0x0294 0x05DC 0x0000 0x5 0x0
#define MX6SX_PAD_SD4_DATA5__SPDIF_IN                             0x0294 0x05DC 0x0824 0x6 0x0
#define MX6SX_PAD_SD4_DATA5__TPSMP_HDATA_17                       0x0294 0x05DC 0x0000 0x7 0x0
#define MX6SX_PAD_SD4_DATA5__VDEC_DEBUG_9                         0x0294 0x05DC 0x0000 0x8 0x0
#define MX6SX_PAD_SD4_DATA5__SDMA_DEBUG_EVENT_CHANNEL_0           0x0294 0x05DC 0x0000 0x9 0x0
#define MX6SX_PAD_SD4_DATA6__USDHC4_DATA6                         0x0298 0x05E0 0x0000 0x0 0x0
#define MX6SX_PAD_SD4_DATA6__RAWNAND_CE3_B                        0x0298 0x05E0 0x0000 0x1 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD4_DATA6__UART5_RTS_B                          0x0298 0x05E0 0x084C 0x2 0x0
=======
#define MX6SX_PAD_SD4_DATA6__UART5_DCE_RTS                        0x0298 0x05E0 0x084C 0x2 0x0
#define MX6SX_PAD_SD4_DATA6__UART5_DTE_CTS                        0x0298 0x05E0 0x0000 0x2 0x0
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD4_DATA6__ECSPI3_MISO                          0x0298 0x05E0 0x0734 0x3 0x0
#define MX6SX_PAD_SD4_DATA6__LCDIF2_DATA_6                        0x0298 0x05E0 0x0000 0x4 0x0
#define MX6SX_PAD_SD4_DATA6__GPIO6_IO_20                          0x0298 0x05E0 0x0000 0x5 0x0
#define MX6SX_PAD_SD4_DATA6__USDHC4_WP                            0x0298 0x05E0 0x0878 0x6 0x0
#define MX6SX_PAD_SD4_DATA6__TPSMP_HDATA_18                       0x0298 0x05E0 0x0000 0x7 0x0
#define MX6SX_PAD_SD4_DATA6__VDEC_DEBUG_10                        0x0298 0x05E0 0x0000 0x8 0x0
#define MX6SX_PAD_SD4_DATA6__SDMA_DEBUG_EVENT_CHANNEL_1           0x0298 0x05E0 0x0000 0x9 0x0
#define MX6SX_PAD_SD4_DATA7__USDHC4_DATA7                         0x029C 0x05E4 0x0000 0x0 0x0
#define MX6SX_PAD_SD4_DATA7__RAWNAND_DATA08                       0x029C 0x05E4 0x0000 0x1 0x0
<<<<<<< HEAD
#define MX6SX_PAD_SD4_DATA7__UART5_CTS_B                          0x029C 0x05E4 0x0000 0x2 0x0
=======
#define MX6SX_PAD_SD4_DATA7__UART5_DCE_CTS                        0x029C 0x05E4 0x0000 0x2 0x0
#define MX6SX_PAD_SD4_DATA7__UART5_DTE_RTS                        0x029C 0x05E4 0x084C 0x2 0x1
>>>>>>> upstream/android-13
#define MX6SX_PAD_SD4_DATA7__ECSPI3_SS0                           0x029C 0x05E4 0x073C 0x3 0x0
#define MX6SX_PAD_SD4_DATA7__LCDIF2_DATA_15                       0x029C 0x05E4 0x0000 0x4 0x0
#define MX6SX_PAD_SD4_DATA7__GPIO6_IO_21                          0x029C 0x05E4 0x0000 0x5 0x0
#define MX6SX_PAD_SD4_DATA7__USDHC4_CD_B                          0x029C 0x05E4 0x0874 0x6 0x0
#define MX6SX_PAD_SD4_DATA7__TPSMP_HDATA_15                       0x029C 0x05E4 0x0000 0x7 0x0
#define MX6SX_PAD_SD4_DATA7__USB_OTG_PWR_WAKE                     0x029C 0x05E4 0x0000 0x8 0x0
#define MX6SX_PAD_SD4_DATA7__SDMA_DEBUG_YIELD                     0x029C 0x05E4 0x0000 0x9 0x0
#define MX6SX_PAD_SD4_RESET_B__USDHC4_RESET_B                     0x02A0 0x05E8 0x0000 0x0 0x0
#define MX6SX_PAD_SD4_RESET_B__RAWNAND_DQS                        0x02A0 0x05E8 0x0000 0x1 0x0
#define MX6SX_PAD_SD4_RESET_B__USDHC4_RESET                       0x02A0 0x05E8 0x0000 0x2 0x0
#define MX6SX_PAD_SD4_RESET_B__AUDMUX_MCLK                        0x02A0 0x05E8 0x0000 0x3 0x0
#define MX6SX_PAD_SD4_RESET_B__LCDIF2_RESET                       0x02A0 0x05E8 0x0000 0x4 0x0
#define MX6SX_PAD_SD4_RESET_B__GPIO6_IO_22                        0x02A0 0x05E8 0x0000 0x5 0x0
#define MX6SX_PAD_SD4_RESET_B__LCDIF2_CS                          0x02A0 0x05E8 0x0000 0x6 0x0
#define MX6SX_PAD_SD4_RESET_B__TPSMP_HDATA_25                     0x02A0 0x05E8 0x0000 0x7 0x0
#define MX6SX_PAD_SD4_RESET_B__VDEC_DEBUG_17                      0x02A0 0x05E8 0x0000 0x8 0x0
#define MX6SX_PAD_SD4_RESET_B__SDMA_DEBUG_BUS_DEVICE_2            0x02A0 0x05E8 0x0000 0x9 0x0
#define MX6SX_PAD_USB_H_DATA__USB_H_DATA                          0x02A4 0x05EC 0x0000 0x0 0x0
#define MX6SX_PAD_USB_H_DATA__PWM2_OUT                            0x02A4 0x05EC 0x0000 0x1 0x0
#define MX6SX_PAD_USB_H_DATA__ANATOP_24M_OUT                      0x02A4 0x05EC 0x0000 0x2 0x0
#define MX6SX_PAD_USB_H_DATA__I2C4_SDA                            0x02A4 0x05EC 0x07C4 0x3 0x1
#define MX6SX_PAD_USB_H_DATA__WDOG3_WDOG_B                        0x02A4 0x05EC 0x0000 0x4 0x0
#define MX6SX_PAD_USB_H_DATA__GPIO7_IO_10                         0x02A4 0x05EC 0x0000 0x5 0x0
#define MX6SX_PAD_USB_H_STROBE__USB_H_STROBE                      0x02A8 0x05F0 0x0000 0x0 0x0
#define MX6SX_PAD_USB_H_STROBE__PWM1_OUT                          0x02A8 0x05F0 0x0000 0x1 0x0
#define MX6SX_PAD_USB_H_STROBE__ANATOP_32K_OUT                    0x02A8 0x05F0 0x0000 0x2 0x0
#define MX6SX_PAD_USB_H_STROBE__I2C4_SCL                          0x02A8 0x05F0 0x07C0 0x3 0x1
#define MX6SX_PAD_USB_H_STROBE__WDOG3_WDOG_RST_B_DEB              0x02A8 0x05F0 0x0000 0x4 0x0
#define MX6SX_PAD_USB_H_STROBE__GPIO7_IO_11                       0x02A8 0x05F0 0x0000 0x5 0x0

<<<<<<< HEAD
=======
/* these are not supposed to be used any more and remove them after some time */
#define MX6SX_PAD_GPIO1_IO04__UART1_RX		MX6SX_PAD_GPIO1_IO04__UART1_DTE_RX
#define MX6SX_PAD_GPIO1_IO04__UART1_TX		MX6SX_PAD_GPIO1_IO04__UART1_DCE_TX
#define MX6SX_PAD_GPIO1_IO05__UART1_RX		MX6SX_PAD_GPIO1_IO05__UART1_DCE_RX
#define MX6SX_PAD_GPIO1_IO05__UART1_TX		MX6SX_PAD_GPIO1_IO05__UART1_DTE_TX
#define MX6SX_PAD_GPIO1_IO06__UART2_RX		MX6SX_PAD_GPIO1_IO06__UART2_DTE_RX
#define MX6SX_PAD_GPIO1_IO06__UART2_TX		MX6SX_PAD_GPIO1_IO06__UART2_DCE_TX
#define MX6SX_PAD_GPIO1_IO06__UART1_RTS_B	MX6SX_PAD_GPIO1_IO06__UART1_DCE_RTS
#define MX6SX_PAD_GPIO1_IO07__UART2_RX		MX6SX_PAD_GPIO1_IO07__UART2_DCE_RX
#define MX6SX_PAD_GPIO1_IO07__UART2_TX		MX6SX_PAD_GPIO1_IO07__UART2_DTE_TX
#define MX6SX_PAD_GPIO1_IO07__UART1_CTS_B	MX6SX_PAD_GPIO1_IO07__UART1_DCE_CTS
#define MX6SX_PAD_GPIO1_IO08__UART2_RTS_B	MX6SX_PAD_GPIO1_IO08__UART2_DCE_RTS
#define MX6SX_PAD_GPIO1_IO09__UART2_CTS_B	MX6SX_PAD_GPIO1_IO09__UART2_DCE_CTS
#define MX6SX_PAD_CSI_DATA04__UART6_RX		MX6SX_PAD_CSI_DATA04__UART6_DCE_RX
#define MX6SX_PAD_CSI_DATA04__UART6_TX		MX6SX_PAD_CSI_DATA04__UART6_DTE_TX
#define MX6SX_PAD_CSI_DATA05__UART6_RX		MX6SX_PAD_CSI_DATA05__UART6_DTE_RX
#define MX6SX_PAD_CSI_DATA05__UART6_TX		MX6SX_PAD_CSI_DATA05__UART6_DCE_TX
#define MX6SX_PAD_CSI_DATA06__UART6_RTS_B	MX6SX_PAD_CSI_DATA06__UART6_DCE_RTS
#define MX6SX_PAD_CSI_DATA07__UART6_CTS_B	MX6SX_PAD_CSI_DATA07__UART6_DCE_CTS
#define MX6SX_PAD_CSI_HSYNC__UART4_RTS_B	MX6SX_PAD_CSI_HSYNC__UART4_DCE_RTS
#define MX6SX_PAD_CSI_MCLK__UART4_RX		MX6SX_PAD_CSI_MCLK__UART4_DCE_RX
#define MX6SX_PAD_CSI_MCLK__UART4_TX		MX6SX_PAD_CSI_MCLK__UART4_DTE_TX
#define MX6SX_PAD_CSI_PIXCLK__UART4_RX		MX6SX_PAD_CSI_PIXCLK__UART4_DTE_RX
#define MX6SX_PAD_CSI_PIXCLK__UART4_TX		MX6SX_PAD_CSI_PIXCLK__UART4_DCE_TX
#define MX6SX_PAD_CSI_VSYNC__UART4_CTS_B	MX6SX_PAD_CSI_VSYNC__UART4_DCE_CTS
#define MX6SX_PAD_ENET2_COL__UART1_RX		MX6SX_PAD_ENET2_COL__UART1_DCE_RX
#define MX6SX_PAD_ENET2_COL__UART1_TX		MX6SX_PAD_ENET2_COL__UART1_DTE_TX
#define MX6SX_PAD_ENET2_CRS__UART1_RX		MX6SX_PAD_ENET2_CRS__UART1_DTE_RX
#define MX6SX_PAD_ENET2_CRS__UART1_TX		MX6SX_PAD_ENET2_CRS__UART1_DCE_TX
#define MX6SX_PAD_ENET2_RX_CLK__UART1_RTS_B	MX6SX_PAD_ENET2_RX_CLK__UART1_DCE_RTS
#define MX6SX_PAD_ENET2_TX_CLK__UART1_CTS_B	MX6SX_PAD_ENET2_TX_CLK__UART1_DCE_CTS
#define MX6SX_PAD_KEY_COL0__UART6_RTS_B		MX6SX_PAD_KEY_COL0__UART6_DCE_RTS
#define MX6SX_PAD_KEY_COL1__UART6_RX		MX6SX_PAD_KEY_COL1__UART6_DTE_RX
#define MX6SX_PAD_KEY_COL1__UART6_TX		MX6SX_PAD_KEY_COL1__UART6_DCE_TX
#define MX6SX_PAD_KEY_COL2__UART5_RTS_B		MX6SX_PAD_KEY_COL2__UART5_DCE_RTS
#define MX6SX_PAD_KEY_COL3__UART5_RX		MX6SX_PAD_KEY_COL3__UART5_DTE_RX
#define MX6SX_PAD_KEY_COL3__UART5_TX		MX6SX_PAD_KEY_COL3__UART5_DCE_TX
#define MX6SX_PAD_KEY_ROW0__UART6_CTS_B		MX6SX_PAD_KEY_ROW0__UART6_DCE_CTS
#define MX6SX_PAD_KEY_ROW1__UART6_RX		MX6SX_PAD_KEY_ROW1__UART6_DCE_RX
#define MX6SX_PAD_KEY_ROW1__UART6_TX		MX6SX_PAD_KEY_ROW1__UART6_DTE_TX
#define MX6SX_PAD_KEY_ROW2__UART5_CTS_B		MX6SX_PAD_KEY_ROW2__UART5_DCE_CTS
#define MX6SX_PAD_KEY_ROW3__UART5_RX		MX6SX_PAD_KEY_ROW3__UART5_DCE_RX
#define MX6SX_PAD_KEY_ROW3__UART5_TX		MX6SX_PAD_KEY_ROW3__UART5_DTE_TX
#define MX6SX_PAD_NAND_DATA04__UART3_RTS_B	MX6SX_PAD_NAND_DATA04__UART3_DCE_RTS
#define MX6SX_PAD_NAND_DATA05__UART3_CTS_B	MX6SX_PAD_NAND_DATA05__UART3_DCE_CTS
#define MX6SX_PAD_NAND_DATA06__UART3_RX		MX6SX_PAD_NAND_DATA06__UART3_DCE_RX
#define MX6SX_PAD_NAND_DATA06__UART3_TX		MX6SX_PAD_NAND_DATA06__UART3_DTE_TX
#define MX6SX_PAD_NAND_DATA07__UART3_RX		MX6SX_PAD_NAND_DATA07__UART3_DTE_RX
#define MX6SX_PAD_NAND_DATA07__UART3_TX		MX6SX_PAD_NAND_DATA07__UART3_DCE_TX
#define MX6SX_PAD_QSPI1B_DATA0__UART3_CTS_B	MX6SX_PAD_QSPI1B_DATA0__UART3_DCE_CTS
#define MX6SX_PAD_QSPI1B_DATA1__UART3_RTS_B	MX6SX_PAD_QSPI1B_DATA1__UART3_DCE_RTS
#define MX6SX_PAD_QSPI1B_SCLK__UART3_RX		MX6SX_PAD_QSPI1B_SCLK__UART3_DCE_RX
#define MX6SX_PAD_QSPI1B_SCLK__UART3_TX		MX6SX_PAD_QSPI1B_SCLK__UART3_DTE_TX
#define MX6SX_PAD_QSPI1B_SS0_B__UART3_RX	MX6SX_PAD_QSPI1B_SS0_B__UART3_DTE_RX
#define MX6SX_PAD_QSPI1B_SS0_B__UART3_TX	MX6SX_PAD_QSPI1B_SS0_B__UART3_DCE_TX
#define MX6SX_PAD_SD1_DATA0__UART2_RX		MX6SX_PAD_SD1_DATA0__UART2_DCE_RX
#define MX6SX_PAD_SD1_DATA0__UART2_TX		MX6SX_PAD_SD1_DATA0__UART2_DTE_TX
#define MX6SX_PAD_SD1_DATA1__UART2_RX		MX6SX_PAD_SD1_DATA1__UART2_DTE_RX
#define MX6SX_PAD_SD1_DATA1__UART2_TX		MX6SX_PAD_SD1_DATA1__UART2_DCE_TX
#define MX6SX_PAD_SD1_DATA2__UART2_CTS_B	MX6SX_PAD_SD1_DATA2__UART2_DCE_CTS
#define MX6SX_PAD_SD1_DATA3__UART2_RTS_B	MX6SX_PAD_SD1_DATA3__UART2_DCE_RTS
#define MX6SX_PAD_SD2_DATA0__UART4_RX		MX6SX_PAD_SD2_DATA0__UART4_DCE_RX
#define MX6SX_PAD_SD2_DATA0__UART4_TX		MX6SX_PAD_SD2_DATA0__UART4_DTE_TX
#define MX6SX_PAD_SD2_DATA1__UART4_RX		MX6SX_PAD_SD2_DATA1__UART4_DTE_RX
#define MX6SX_PAD_SD2_DATA1__UART4_TX		MX6SX_PAD_SD2_DATA1__UART4_DCE_TX
#define MX6SX_PAD_SD2_DATA2__UART6_RX		MX6SX_PAD_SD2_DATA2__UART6_DCE_RX
#define MX6SX_PAD_SD2_DATA2__UART6_TX		MX6SX_PAD_SD2_DATA2__UART6_DTE_TX
#define MX6SX_PAD_SD2_DATA3__UART6_RX		MX6SX_PAD_SD2_DATA3__UART6_DTE_RX
#define MX6SX_PAD_SD2_DATA3__UART6_TX		MX6SX_PAD_SD2_DATA3__UART6_DCE_TX
#define MX6SX_PAD_SD3_CLK__UART4_CTS_B		MX6SX_PAD_SD3_CLK__UART4_DCE_CTS
#define MX6SX_PAD_SD3_CMD__UART4_RX		MX6SX_PAD_SD3_CMD__UART4_DTE_RX
#define MX6SX_PAD_SD3_CMD__UART4_TX		MX6SX_PAD_SD3_CMD__UART4_DCE_TX
#define MX6SX_PAD_SD3_DATA2__UART4_RTS_B	MX6SX_PAD_SD3_DATA2__UART4_DCE_RTS
#define MX6SX_PAD_SD3_DATA3__UART4_RX		MX6SX_PAD_SD3_DATA3__UART4_DCE_RX
#define MX6SX_PAD_SD3_DATA3__UART4_TX		MX6SX_PAD_SD3_DATA3__UART4_DTE_TX
#define MX6SX_PAD_SD3_DATA4__UART3_RX		MX6SX_PAD_SD3_DATA4__UART3_DCE_RX
#define MX6SX_PAD_SD3_DATA4__UART3_TX		MX6SX_PAD_SD3_DATA4__UART3_DTE_TX
#define MX6SX_PAD_SD3_DATA5__UART3_RX		MX6SX_PAD_SD3_DATA5__UART3_DTE_RX
#define MX6SX_PAD_SD3_DATA5__UART3_TX		MX6SX_PAD_SD3_DATA5__UART3_DCE_TX
#define MX6SX_PAD_SD3_DATA6__UART3_RTS_B	MX6SX_PAD_SD3_DATA6__UART3_DCE_RTS
#define MX6SX_PAD_SD3_DATA7__UART3_CTS_B	MX6SX_PAD_SD3_DATA7__UART3_DCE_CTS
#define MX6SX_PAD_SD4_DATA4__UART5_RX		MX6SX_PAD_SD4_DATA4__UART5_DCE_RX
#define MX6SX_PAD_SD4_DATA4__UART5_TX		MX6SX_PAD_SD4_DATA4__UART5_DTE_TX
#define MX6SX_PAD_SD4_DATA5__UART5_RX		MX6SX_PAD_SD4_DATA5__UART5_DTE_RX
#define MX6SX_PAD_SD4_DATA5__UART5_TX		MX6SX_PAD_SD4_DATA5__UART5_DCE_TX
#define MX6SX_PAD_SD4_DATA6__UART5_RTS_B	MX6SX_PAD_SD4_DATA6__UART5_DCE_RTS
#define MX6SX_PAD_SD4_DATA7__UART5_CTS_B	MX6SX_PAD_SD4_DATA7__UART5_DCE_CTS

>>>>>>> upstream/android-13
#endif /* __DTS_IMX6SX_PINFUNC_H */
