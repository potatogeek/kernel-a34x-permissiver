

#ifndef __LINUX_MFD_WM8350_CORE_H_
#define __LINUX_MFD_WM8350_CORE_H_

#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/interrupt.h>
#include <linux/completion.h>
#include <linux/regmap.h>

#include <linux/mfd/wm8350/audio.h>
#include <linux/mfd/wm8350/gpio.h>
#include <linux/mfd/wm8350/pmic.h>
#include <linux/mfd/wm8350/rtc.h>
#include <linux/mfd/wm8350/supply.h>
#include <linux/mfd/wm8350/wdt.h>


#define WM8350_RESET_ID                         0x00
#define WM8350_ID                               0x01
#define WM8350_REVISION				0x02
#define WM8350_SYSTEM_CONTROL_1                 0x03
#define WM8350_SYSTEM_CONTROL_2                 0x04
#define WM8350_SYSTEM_HIBERNATE                 0x05
#define WM8350_INTERFACE_CONTROL                0x06
#define WM8350_POWER_MGMT_1                     0x08
#define WM8350_POWER_MGMT_2                     0x09
#define WM8350_POWER_MGMT_3                     0x0A
#define WM8350_POWER_MGMT_4                     0x0B
#define WM8350_POWER_MGMT_5                     0x0C
#define WM8350_POWER_MGMT_6                     0x0D
#define WM8350_POWER_MGMT_7                     0x0E

#define WM8350_SYSTEM_INTERRUPTS                0x18
#define WM8350_INT_STATUS_1                     0x19
#define WM8350_INT_STATUS_2                     0x1A
#define WM8350_POWER_UP_INT_STATUS              0x1B
#define WM8350_UNDER_VOLTAGE_INT_STATUS         0x1C
#define WM8350_OVER_CURRENT_INT_STATUS          0x1D
#define WM8350_GPIO_INT_STATUS                  0x1E
#define WM8350_COMPARATOR_INT_STATUS            0x1F
#define WM8350_SYSTEM_INTERRUPTS_MASK           0x20
#define WM8350_INT_STATUS_1_MASK                0x21
#define WM8350_INT_STATUS_2_MASK                0x22
#define WM8350_POWER_UP_INT_STATUS_MASK         0x23
#define WM8350_UNDER_VOLTAGE_INT_STATUS_MASK    0x24
#define WM8350_OVER_CURRENT_INT_STATUS_MASK     0x25
#define WM8350_GPIO_INT_STATUS_MASK             0x26
#define WM8350_COMPARATOR_INT_STATUS_MASK       0x27
#define WM8350_CHARGER_OVERRIDES		0xE2
#define WM8350_MISC_OVERRIDES			0xE3
#define WM8350_COMPARATOR_OVERRIDES		0xE7
#define WM8350_STATE_MACHINE_STATUS		0xE9

#define WM8350_MAX_REGISTER                     0xFF

#define WM8350_UNLOCK_KEY		0x0013
#define WM8350_LOCK_KEY			0x0000




#define WM8350_SW_RESET_CHIP_ID_MASK            0xFFFF


#define WM8350_CHIP_REV_MASK                    0x7000
#define WM8350_CONF_STS_MASK                    0x0C00
#define WM8350_CUST_ID_MASK                     0x00FF


#define WM8350_MASK_REV_MASK			0x00FF


#define WM8350_CHIP_ON                          0x8000
#define WM8350_POWERCYCLE                       0x2000
#define WM8350_VCC_FAULT_OV                     0x1000
#define WM8350_REG_RSTB_TIME_MASK               0x0C00
#define WM8350_BG_SLEEP                         0x0200
#define WM8350_MEM_VALID                        0x0020
#define WM8350_CHIP_SET_UP                      0x0010
#define WM8350_ON_DEB_T                         0x0008
#define WM8350_ON_POL                           0x0002
#define WM8350_IRQ_POL                          0x0001


#define WM8350_USB_SUSPEND_8MA                  0x8000
#define WM8350_USB_SUSPEND                      0x4000
#define WM8350_USB_MSTR                         0x2000
#define WM8350_USB_MSTR_SRC                     0x1000
#define WM8350_USB_500MA                        0x0800
#define WM8350_USB_NOLIM                        0x0400


#define WM8350_HIBERNATE                        0x8000
#define WM8350_WDOG_HIB_MODE                    0x0080
#define WM8350_REG_HIB_STARTUP_SEQ              0x0040
#define WM8350_REG_RESET_HIB_MODE               0x0020
#define WM8350_RST_HIB_MODE                     0x0010
#define WM8350_IRQ_HIB_MODE                     0x0008
#define WM8350_MEMRST_HIB_MODE                  0x0004
#define WM8350_PCCOMP_HIB_MODE                  0x0002
#define WM8350_TEMPMON_HIB_MODE                 0x0001


#define WM8350_USE_DEV_PINS                     0x8000
#define WM8350_USE_DEV_PINS_MASK                0x8000
#define WM8350_USE_DEV_PINS_SHIFT                   15
#define WM8350_DEV_ADDR_MASK                    0x6000
#define WM8350_DEV_ADDR_SHIFT                       13
#define WM8350_CONFIG_DONE                      0x1000
#define WM8350_CONFIG_DONE_MASK                 0x1000
#define WM8350_CONFIG_DONE_SHIFT                    12
#define WM8350_RECONFIG_AT_ON                   0x0800
#define WM8350_RECONFIG_AT_ON_MASK              0x0800
#define WM8350_RECONFIG_AT_ON_SHIFT                 11
#define WM8350_AUTOINC                          0x0200
#define WM8350_AUTOINC_MASK                     0x0200
#define WM8350_AUTOINC_SHIFT                         9
#define WM8350_ARA                              0x0100
#define WM8350_ARA_MASK                         0x0100
#define WM8350_ARA_SHIFT                             8
#define WM8350_SPI_CFG                          0x0008
#define WM8350_SPI_CFG_MASK                     0x0008
#define WM8350_SPI_CFG_SHIFT                         3
#define WM8350_SPI_4WIRE                        0x0004
#define WM8350_SPI_4WIRE_MASK                   0x0004
#define WM8350_SPI_4WIRE_SHIFT                       2
#define WM8350_SPI_3WIRE                        0x0002
#define WM8350_SPI_3WIRE_MASK                   0x0002
#define WM8350_SPI_3WIRE_SHIFT                       1


#define WM8350_USE_DEV_PINS_PRIMARY                  0
#define WM8350_USE_DEV_PINS_DEV                      1

#define WM8350_DEV_ADDR_34                           0
#define WM8350_DEV_ADDR_36                           1
#define WM8350_DEV_ADDR_3C                           2
#define WM8350_DEV_ADDR_3E                           3

#define WM8350_CONFIG_DONE_OFF                       0
#define WM8350_CONFIG_DONE_DONE                      1

#define WM8350_RECONFIG_AT_ON_OFF                    0
#define WM8350_RECONFIG_AT_ON_ON                     1

#define WM8350_AUTOINC_OFF                           0
#define WM8350_AUTOINC_ON                            1

#define WM8350_ARA_OFF                               0
#define WM8350_ARA_ON                                1

#define WM8350_SPI_CFG_CMOS                          0
#define WM8350_SPI_CFG_OD                            1

#define WM8350_SPI_4WIRE_3WIRE                       0
#define WM8350_SPI_4WIRE_4WIRE                       1

#define WM8350_SPI_3WIRE_I2C                         0
#define WM8350_SPI_3WIRE_SPI                         1


#define WM8350_CODEC_ISEL_MASK                  0xC000
#define WM8350_VBUFEN                           0x2000
#define WM8350_OUTPUT_DRAIN_EN                  0x0400
#define WM8350_MIC_DET_ENA                      0x0100
#define WM8350_BIASEN                           0x0020
#define WM8350_MICBEN                           0x0010
#define WM8350_VMIDEN                           0x0004
#define WM8350_VMID_MASK                        0x0003
#define WM8350_VMID_SHIFT                            0


#define WM8350_IN3R_ENA                         0x0800
#define WM8350_IN3L_ENA                         0x0400
#define WM8350_INR_ENA                          0x0200
#define WM8350_INL_ENA                          0x0100
#define WM8350_MIXINR_ENA                       0x0080
#define WM8350_MIXINL_ENA                       0x0040
#define WM8350_OUT4_ENA                         0x0020
#define WM8350_OUT3_ENA                         0x0010
#define WM8350_MIXOUTR_ENA                      0x0002
#define WM8350_MIXOUTL_ENA                      0x0001


#define WM8350_IN3R_TO_OUT2R                    0x0080
#define WM8350_OUT2R_ENA                        0x0008
#define WM8350_OUT2L_ENA                        0x0004
#define WM8350_OUT1R_ENA                        0x0002
#define WM8350_OUT1L_ENA                        0x0001


#define WM8350_SYSCLK_ENA                       0x4000
#define WM8350_ADC_HPF_ENA                      0x2000
#define WM8350_FLL_ENA                          0x0800
#define WM8350_FLL_OSC_ENA                      0x0400
#define WM8350_TOCLK_ENA                        0x0100
#define WM8350_DACR_ENA                         0x0020
#define WM8350_DACL_ENA                         0x0010
#define WM8350_ADCR_ENA                         0x0008
#define WM8350_ADCL_ENA                         0x0004


#define WM8350_CODEC_ENA                        0x1000
#define WM8350_RTC_TICK_ENA                     0x0800
#define WM8350_OSC32K_ENA                       0x0400
#define WM8350_CHG_ENA                          0x0200
#define WM8350_ACC_DET_ENA                      0x0100
#define WM8350_AUXADC_ENA                       0x0080
#define WM8350_DCMP4_ENA                        0x0008
#define WM8350_DCMP3_ENA                        0x0004
#define WM8350_DCMP2_ENA                        0x0002
#define WM8350_DCMP1_ENA                        0x0001


#define WM8350_LS_ENA                           0x8000
#define WM8350_LDO4_ENA                         0x0800
#define WM8350_LDO3_ENA                         0x0400
#define WM8350_LDO2_ENA                         0x0200
#define WM8350_LDO1_ENA                         0x0100
#define WM8350_DC6_ENA                          0x0020
#define WM8350_DC5_ENA                          0x0010
#define WM8350_DC4_ENA                          0x0008
#define WM8350_DC3_ENA                          0x0004
#define WM8350_DC2_ENA                          0x0002
#define WM8350_DC1_ENA                          0x0001


#define WM8350_CS2_ENA                          0x0002
#define WM8350_CS1_ENA                          0x0001


#define WM8350_OC_INT                           0x2000
#define WM8350_UV_INT                           0x1000
#define WM8350_PUTO_INT                         0x0800
#define WM8350_CS_INT                           0x0200
#define WM8350_EXT_INT                          0x0100
#define WM8350_CODEC_INT                        0x0080
#define WM8350_GP_INT                           0x0040
#define WM8350_AUXADC_INT                       0x0020
#define WM8350_RTC_INT                          0x0010
#define WM8350_SYS_INT                          0x0008
#define WM8350_CHG_INT                          0x0004
#define WM8350_USB_INT                          0x0002
#define WM8350_WKUP_INT                         0x0001


#define WM8350_CHG_BAT_HOT_EINT                 0x8000
#define WM8350_CHG_BAT_COLD_EINT                0x4000
#define WM8350_CHG_BAT_FAIL_EINT                0x2000
#define WM8350_CHG_TO_EINT                      0x1000
#define WM8350_CHG_END_EINT                     0x0800
#define WM8350_CHG_START_EINT                   0x0400
#define WM8350_CHG_FAST_RDY_EINT                0x0200
#define WM8350_RTC_PER_EINT                     0x0080
#define WM8350_RTC_SEC_EINT                     0x0040
#define WM8350_RTC_ALM_EINT                     0x0020
#define WM8350_CHG_VBATT_LT_3P9_EINT            0x0004
#define WM8350_CHG_VBATT_LT_3P1_EINT            0x0002
#define WM8350_CHG_VBATT_LT_2P85_EINT           0x0001


#define WM8350_CS1_EINT                         0x2000
#define WM8350_CS2_EINT                         0x1000
#define WM8350_USB_LIMIT_EINT                   0x0400
#define WM8350_AUXADC_DATARDY_EINT              0x0100
#define WM8350_AUXADC_DCOMP4_EINT               0x0080
#define WM8350_AUXADC_DCOMP3_EINT               0x0040
#define WM8350_AUXADC_DCOMP2_EINT               0x0020
#define WM8350_AUXADC_DCOMP1_EINT               0x0010
#define WM8350_SYS_HYST_COMP_FAIL_EINT          0x0008
#define WM8350_SYS_CHIP_GT115_EINT              0x0004
#define WM8350_SYS_CHIP_GT140_EINT              0x0002
#define WM8350_SYS_WDOG_TO_EINT                 0x0001


#define WM8350_PUTO_LDO4_EINT                   0x0800
#define WM8350_PUTO_LDO3_EINT                   0x0400
#define WM8350_PUTO_LDO2_EINT                   0x0200
#define WM8350_PUTO_LDO1_EINT                   0x0100
#define WM8350_PUTO_DC6_EINT                    0x0020
#define WM8350_PUTO_DC5_EINT                    0x0010
#define WM8350_PUTO_DC4_EINT                    0x0008
#define WM8350_PUTO_DC3_EINT                    0x0004
#define WM8350_PUTO_DC2_EINT                    0x0002
#define WM8350_PUTO_DC1_EINT                    0x0001


#define WM8350_UV_LDO4_EINT                     0x0800
#define WM8350_UV_LDO3_EINT                     0x0400
#define WM8350_UV_LDO2_EINT                     0x0200
#define WM8350_UV_LDO1_EINT                     0x0100
#define WM8350_UV_DC6_EINT                      0x0020
#define WM8350_UV_DC5_EINT                      0x0010
#define WM8350_UV_DC4_EINT                      0x0008
#define WM8350_UV_DC3_EINT                      0x0004
#define WM8350_UV_DC2_EINT                      0x0002
#define WM8350_UV_DC1_EINT                      0x0001


#define WM8350_OC_LS_EINT                       0x8000


#define WM8350_GP12_EINT                        0x1000
#define WM8350_GP11_EINT                        0x0800
#define WM8350_GP10_EINT                        0x0400
#define WM8350_GP9_EINT                         0x0200
#define WM8350_GP8_EINT                         0x0100
#define WM8350_GP7_EINT                         0x0080
#define WM8350_GP6_EINT                         0x0040
#define WM8350_GP5_EINT                         0x0020
#define WM8350_GP4_EINT                         0x0010
#define WM8350_GP3_EINT                         0x0008
#define WM8350_GP2_EINT                         0x0004
#define WM8350_GP1_EINT                         0x0002
#define WM8350_GP0_EINT                         0x0001


#define WM8350_EXT_USB_FB_EINT                  0x8000
#define WM8350_EXT_WALL_FB_EINT                 0x4000
#define WM8350_EXT_BAT_FB_EINT                  0x2000
#define WM8350_CODEC_JCK_DET_L_EINT             0x0800
#define WM8350_CODEC_JCK_DET_R_EINT             0x0400
#define WM8350_CODEC_MICSCD_EINT                0x0200
#define WM8350_CODEC_MICD_EINT                  0x0100
#define WM8350_WKUP_OFF_STATE_EINT              0x0040
#define WM8350_WKUP_HIB_STATE_EINT              0x0020
#define WM8350_WKUP_CONV_FAULT_EINT             0x0010
#define WM8350_WKUP_WDOG_RST_EINT               0x0008
#define WM8350_WKUP_GP_PWR_ON_EINT              0x0004
#define WM8350_WKUP_ONKEY_EINT                  0x0002
#define WM8350_WKUP_GP_WAKEUP_EINT              0x0001


#define WM8350_IM_OC_INT                        0x2000
#define WM8350_IM_UV_INT                        0x1000
#define WM8350_IM_PUTO_INT                      0x0800
#define WM8350_IM_SPARE_INT                     0x0400
#define WM8350_IM_CS_INT                        0x0200
#define WM8350_IM_EXT_INT                       0x0100
#define WM8350_IM_CODEC_INT                     0x0080
#define WM8350_IM_GP_INT                        0x0040
#define WM8350_IM_AUXADC_INT                    0x0020
#define WM8350_IM_RTC_INT                       0x0010
#define WM8350_IM_SYS_INT                       0x0008
#define WM8350_IM_CHG_INT                       0x0004
#define WM8350_IM_USB_INT                       0x0002
#define WM8350_IM_WKUP_INT                      0x0001


#define WM8350_IM_CHG_BAT_HOT_EINT              0x8000
#define WM8350_IM_CHG_BAT_COLD_EINT             0x4000
#define WM8350_IM_CHG_BAT_FAIL_EINT             0x2000
#define WM8350_IM_CHG_TO_EINT                   0x1000
#define WM8350_IM_CHG_END_EINT                  0x0800
#define WM8350_IM_CHG_START_EINT                0x0400
#define WM8350_IM_CHG_FAST_RDY_EINT             0x0200
#define WM8350_IM_RTC_PER_EINT                  0x0080
#define WM8350_IM_RTC_SEC_EINT                  0x0040
#define WM8350_IM_RTC_ALM_EINT                  0x0020
#define WM8350_IM_CHG_VBATT_LT_3P9_EINT         0x0004
#define WM8350_IM_CHG_VBATT_LT_3P1_EINT         0x0002
#define WM8350_IM_CHG_VBATT_LT_2P85_EINT        0x0001


#define WM8350_IM_SPARE2_EINT                   0x8000
#define WM8350_IM_SPARE1_EINT                   0x4000
#define WM8350_IM_CS1_EINT                      0x2000
#define WM8350_IM_CS2_EINT                      0x1000
#define WM8350_IM_USB_LIMIT_EINT                0x0400
#define WM8350_IM_AUXADC_DATARDY_EINT           0x0100
#define WM8350_IM_AUXADC_DCOMP4_EINT            0x0080
#define WM8350_IM_AUXADC_DCOMP3_EINT            0x0040
#define WM8350_IM_AUXADC_DCOMP2_EINT            0x0020
#define WM8350_IM_AUXADC_DCOMP1_EINT            0x0010
#define WM8350_IM_SYS_HYST_COMP_FAIL_EINT       0x0008
#define WM8350_IM_SYS_CHIP_GT115_EINT           0x0004
#define WM8350_IM_SYS_CHIP_GT140_EINT           0x0002
#define WM8350_IM_SYS_WDOG_TO_EINT              0x0001


#define WM8350_IM_PUTO_LDO4_EINT                0x0800
#define WM8350_IM_PUTO_LDO3_EINT                0x0400
#define WM8350_IM_PUTO_LDO2_EINT                0x0200
#define WM8350_IM_PUTO_LDO1_EINT                0x0100
#define WM8350_IM_PUTO_DC6_EINT                 0x0020
#define WM8350_IM_PUTO_DC5_EINT                 0x0010
#define WM8350_IM_PUTO_DC4_EINT                 0x0008
#define WM8350_IM_PUTO_DC3_EINT                 0x0004
#define WM8350_IM_PUTO_DC2_EINT                 0x0002
#define WM8350_IM_PUTO_DC1_EINT                 0x0001


#define WM8350_IM_UV_LDO4_EINT                  0x0800
#define WM8350_IM_UV_LDO3_EINT                  0x0400
#define WM8350_IM_UV_LDO2_EINT                  0x0200
#define WM8350_IM_UV_LDO1_EINT                  0x0100
#define WM8350_IM_UV_DC6_EINT                   0x0020
#define WM8350_IM_UV_DC5_EINT                   0x0010
#define WM8350_IM_UV_DC4_EINT                   0x0008
#define WM8350_IM_UV_DC3_EINT                   0x0004
#define WM8350_IM_UV_DC2_EINT                   0x0002
#define WM8350_IM_UV_DC1_EINT                   0x0001


#define WM8350_IM_OC_LS_EINT                    0x8000


#define WM8350_IM_GP12_EINT                     0x1000
#define WM8350_IM_GP11_EINT                     0x0800
#define WM8350_IM_GP10_EINT                     0x0400
#define WM8350_IM_GP9_EINT                      0x0200
#define WM8350_IM_GP8_EINT                      0x0100
#define WM8350_IM_GP7_EINT                      0x0080
#define WM8350_IM_GP6_EINT                      0x0040
#define WM8350_IM_GP5_EINT                      0x0020
#define WM8350_IM_GP4_EINT                      0x0010
#define WM8350_IM_GP3_EINT                      0x0008
#define WM8350_IM_GP2_EINT                      0x0004
#define WM8350_IM_GP1_EINT                      0x0002
#define WM8350_IM_GP0_EINT                      0x0001


#define WM8350_IM_EXT_USB_FB_EINT               0x8000
#define WM8350_IM_EXT_WALL_FB_EINT              0x4000
#define WM8350_IM_EXT_BAT_FB_EINT               0x2000
#define WM8350_IM_CODEC_JCK_DET_L_EINT          0x0800
#define WM8350_IM_CODEC_JCK_DET_R_EINT          0x0400
#define WM8350_IM_CODEC_MICSCD_EINT             0x0200
#define WM8350_IM_CODEC_MICD_EINT               0x0100
#define WM8350_IM_WKUP_OFF_STATE_EINT           0x0040
#define WM8350_IM_WKUP_HIB_STATE_EINT           0x0020
#define WM8350_IM_WKUP_CONV_FAULT_EINT          0x0010
#define WM8350_IM_WKUP_WDOG_RST_EINT            0x0008
#define WM8350_IM_WKUP_GP_PWR_ON_EINT           0x0004
#define WM8350_IM_WKUP_ONKEY_EINT               0x0002
#define WM8350_IM_WKUP_GP_WAKEUP_EINT           0x0001


#define WM8350_READ_STATUS                      0x0800
#define WM8350_TSTRAM_CLK                       0x0100
#define WM8350_TSTRAM_CLK_ENA                   0x0080
#define WM8350_STARTSEQ                         0x0040
#define WM8350_READ_SRC                         0x0020
#define WM8350_COUNT_DIR                        0x0010
#define WM8350_TSTRAM_MODE_MASK                 0x000E
#define WM8350_TSTRAM_ENA                       0x0001


#define WM8350_LS_STS                           0x8000
#define WM8350_LDO4_STS                         0x0800
#define WM8350_LDO3_STS                         0x0400
#define WM8350_LDO2_STS                         0x0200
#define WM8350_LDO1_STS                         0x0100
#define WM8350_DC6_STS                          0x0020
#define WM8350_DC5_STS                          0x0010
#define WM8350_DC4_STS                          0x0008
#define WM8350_DC3_STS                          0x0004
#define WM8350_DC2_STS                          0x0002
#define WM8350_DC1_STS                          0x0001


#define WM8350_CHG_BATT_HOT_OVRDE		0x8000
#define WM8350_CHG_BATT_COLD_OVRDE		0x4000


#define WM8350_USB_LIMIT_OVRDE			0x0400


#define WM8350_USB_FB_OVRDE			0x8000
#define WM8350_WALL_FB_OVRDE			0x4000
#define WM8350_BATT_FB_OVRDE			0x2000



#define WM8350_USB_SM_MASK			0x0700
#define WM8350_USB_SM_SHIFT			8

#define WM8350_USB_SM_100_SLV   1
#define WM8350_USB_SM_500_SLV   5
#define WM8350_USB_SM_STDBY_SLV 7


#define WM8350_IRQ_WKUP_OFF_STATE		43
#define WM8350_IRQ_WKUP_HIB_STATE		44
#define WM8350_IRQ_WKUP_CONV_FAULT		45
#define WM8350_IRQ_WKUP_WDOG_RST		46
#define WM8350_IRQ_WKUP_GP_PWR_ON		47
#define WM8350_IRQ_WKUP_ONKEY			48
#define WM8350_IRQ_WKUP_GP_WAKEUP		49


#define WM8350_REV_E				0x4
#define WM8350_REV_F				0x5
#define WM8350_REV_G				0x6
#define WM8350_REV_H				0x7

#define WM8350_NUM_IRQ				63

#define WM8350_NUM_IRQ_REGS 7

extern const struct regmap_config wm8350_regmap;

struct wm8350;

struct wm8350_hwmon {
	struct platform_device *pdev;
	struct device *classdev;
};

struct wm8350 {
	struct device *dev;

	
	struct regmap *regmap;
	bool unlocked;

	struct mutex auxadc_mutex;
	struct completion auxadc_done;

	
	struct mutex irq_lock;
	int chip_irq;
	int irq_base;
	u16 irq_masks[WM8350_NUM_IRQ_REGS];

	
	struct wm8350_codec codec;
	struct wm8350_gpio gpio;
	struct wm8350_hwmon hwmon;
	struct wm8350_pmic pmic;
	struct wm8350_power power;
	struct wm8350_rtc rtc;
	struct wm8350_wdt wdt;
};


struct wm8350_platform_data {
	int (*init)(struct wm8350 *wm8350);
	int irq_high;
	int irq_base;
	int gpio_base;
};



int wm8350_device_init(struct wm8350 *wm8350, int irq,
		       struct wm8350_platform_data *pdata);
void wm8350_device_exit(struct wm8350 *wm8350);


int wm8350_clear_bits(struct wm8350 *wm8350, u16 reg, u16 mask);
int wm8350_set_bits(struct wm8350 *wm8350, u16 reg, u16 mask);
u16 wm8350_reg_read(struct wm8350 *wm8350, int reg);
int wm8350_reg_write(struct wm8350 *wm8350, int reg, u16 val);
int wm8350_reg_lock(struct wm8350 *wm8350);
int wm8350_reg_unlock(struct wm8350 *wm8350);
int wm8350_block_read(struct wm8350 *wm8350, int reg, int size, u16 *dest);
int wm8350_block_write(struct wm8350 *wm8350, int reg, int size, u16 *src);


static inline int wm8350_register_irq(struct wm8350 *wm8350, int irq,
				      irq_handler_t handler,
				      unsigned long flags,
				      const char *name, void *data)
{
	if (!wm8350->irq_base)
		return -ENODEV;

	return request_threaded_irq(irq + wm8350->irq_base, NULL,
				    handler, flags, name, data);
}

static inline void wm8350_free_irq(struct wm8350 *wm8350, int irq, void *data)
{
	free_irq(irq + wm8350->irq_base, data);
}

static inline void wm8350_mask_irq(struct wm8350 *wm8350, int irq)
{
	disable_irq(irq + wm8350->irq_base);
}

static inline void wm8350_unmask_irq(struct wm8350 *wm8350, int irq)
{
	enable_irq(irq + wm8350->irq_base);
}

int wm8350_irq_init(struct wm8350 *wm8350, int irq,
		    struct wm8350_platform_data *pdata);
int wm8350_irq_exit(struct wm8350 *wm8350);

#endif
