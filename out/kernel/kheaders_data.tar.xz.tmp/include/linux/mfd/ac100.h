

#ifndef __LINUX_MFD_AC100_H
#define __LINUX_MFD_AC100_H

#include <linux/regmap.h>

struct ac100_dev {
	struct device			*dev;
	struct regmap			*regmap;
};


#define AC100_CHIP_AUDIO_RST		0x00
#define AC100_PLL_CTRL1			0x01
#define AC100_PLL_CTRL2			0x02
#define AC100_SYSCLK_CTRL		0x03
#define AC100_MOD_CLK_ENA		0x04
#define AC100_MOD_RST_CTRL		0x05
#define AC100_I2S_SR_CTRL		0x06


#define AC100_I2S1_CLK_CTRL		0x10
#define AC100_I2S1_SND_OUT_CTRL		0x11
#define AC100_I2S1_SND_IN_CTRL		0x12
#define AC100_I2S1_MXR_SRC		0x13
#define AC100_I2S1_VOL_CTRL1		0x14
#define AC100_I2S1_VOL_CTRL2		0x15
#define AC100_I2S1_VOL_CTRL3		0x16
#define AC100_I2S1_VOL_CTRL4		0x17
#define AC100_I2S1_MXR_GAIN		0x18


#define AC100_I2S2_CLK_CTRL		0x20
#define AC100_I2S2_SND_OUT_CTRL		0x21
#define AC100_I2S2_SND_IN_CTRL		0x22
#define AC100_I2S2_MXR_SRC		0x23
#define AC100_I2S2_VOL_CTRL1		0x24
#define AC100_I2S2_VOL_CTRL2		0x25
#define AC100_I2S2_VOL_CTRL3		0x26
#define AC100_I2S2_VOL_CTRL4		0x27
#define AC100_I2S2_MXR_GAIN		0x28


#define AC100_I2S3_CLK_CTRL		0x30
#define AC100_I2S3_SND_OUT_CTRL		0x31
#define AC100_I2S3_SND_IN_CTRL		0x32
#define AC100_I2S3_SIG_PATH_CTRL	0x33


#define AC100_ADC_DIG_CTRL		0x40
#define AC100_ADC_VOL_CTRL		0x41


#define AC100_HMIC_CTRL1		0x44
#define AC100_HMIC_CTRL2		0x45
#define AC100_HMIC_STATUS		0x46


#define AC100_DAC_DIG_CTRL		0x48
#define AC100_DAC_VOL_CTRL		0x49
#define AC100_DAC_MXR_SRC		0x4c
#define AC100_DAC_MXR_GAIN		0x4d


#define AC100_ADC_APC_CTRL		0x50
#define AC100_ADC_SRC			0x51
#define AC100_ADC_SRC_BST_CTRL		0x52
#define AC100_OUT_MXR_DAC_A_CTRL	0x53
#define AC100_OUT_MXR_SRC		0x54
#define AC100_OUT_MXR_SRC_BST		0x55
#define AC100_HPOUT_CTRL		0x56
#define AC100_ERPOUT_CTRL		0x57
#define AC100_SPKOUT_CTRL		0x58
#define AC100_LINEOUT_CTRL		0x59


#define AC100_ADC_DAP_L_STA		0x80
#define AC100_ADC_DAP_R_STA		0x81
#define AC100_ADC_DAP_L_CTRL		0x82
#define AC100_ADC_DAP_R_CTRL		0x83
#define AC100_ADC_DAP_L_T_L		0x84 
#define AC100_ADC_DAP_R_T_L		0x85 
#define AC100_ADC_DAP_L_H_A_C		0x86 
#define AC100_ADC_DAP_L_L_A_C		0x87 
#define AC100_ADC_DAP_R_H_A_C		0x88 
#define AC100_ADC_DAP_R_L_A_C		0x89 
#define AC100_ADC_DAP_L_D_T		0x8a 
#define AC100_ADC_DAP_L_A_T		0x8b 
#define AC100_ADC_DAP_R_D_T		0x8c 
#define AC100_ADC_DAP_R_A_T		0x8d 
#define AC100_ADC_DAP_N_TH		0x8e 
#define AC100_ADC_DAP_L_H_N_A_C		0x8f 
#define AC100_ADC_DAP_L_L_N_A_C		0x90 
#define AC100_ADC_DAP_R_H_N_A_C		0x91 
#define AC100_ADC_DAP_R_L_N_A_C		0x92 
#define AC100_ADC_DAP_H_HPF_C		0x93 
#define AC100_ADC_DAP_L_HPF_C		0x94 
#define AC100_ADC_DAP_OPT		0x95 


#define AC100_DAC_DAP_CTRL		0xa0
#define AC100_DAC_DAP_H_HPF_C		0xa1 
#define AC100_DAC_DAP_L_HPF_C		0xa2 
#define AC100_DAC_DAP_L_H_E_A_C		0xa3 
#define AC100_DAC_DAP_L_L_E_A_C		0xa4 
#define AC100_DAC_DAP_R_H_E_A_C		0xa5 
#define AC100_DAC_DAP_R_L_E_A_C		0xa6 
#define AC100_DAC_DAP_H_G_D_T_C		0xa7 
#define AC100_DAC_DAP_L_G_D_T_C		0xa8 
#define AC100_DAC_DAP_H_G_A_T_C		0xa9 
#define AC100_DAC_DAP_L_G_A_T_C		0xaa 
#define AC100_DAC_DAP_H_E_TH		0xab 
#define AC100_DAC_DAP_L_E_TH		0xac 
#define AC100_DAC_DAP_H_G_K		0xad 
#define AC100_DAC_DAP_L_G_K		0xae 
#define AC100_DAC_DAP_H_G_OFF		0xaf 
#define AC100_DAC_DAP_L_G_OFF		0xb0 
#define AC100_DAC_DAP_OPT		0xb1 


#define AC100_ADC_DAP_ENA		0xb4
#define AC100_DAC_DAP_ENA		0xb5


#define AC100_SRC1_CTRL1		0xb8
#define AC100_SRC1_CTRL2		0xb9
#define AC100_SRC1_CTRL3		0xba
#define AC100_SRC1_CTRL4		0xbb
#define AC100_SRC2_CTRL1		0xbc
#define AC100_SRC2_CTRL2		0xbd
#define AC100_SRC2_CTRL3		0xbe
#define AC100_SRC2_CTRL4		0xbf


#define AC100_CLK32K_ANALOG_CTRL	0xc0
#define AC100_CLKOUT_CTRL1		0xc1
#define AC100_CLKOUT_CTRL2		0xc2
#define AC100_CLKOUT_CTRL3		0xc3


#define AC100_RTC_RST			0xc6
#define AC100_RTC_CTRL			0xc7
#define AC100_RTC_SEC			0xc8 
#define AC100_RTC_MIN			0xc9 
#define AC100_RTC_HOU			0xca 
#define AC100_RTC_WEE			0xcb 
#define AC100_RTC_DAY			0xcc 
#define AC100_RTC_MON			0xcd 
#define AC100_RTC_YEA			0xce 
#define AC100_RTC_UPD			0xcf 


#define AC100_ALM_INT_ENA		0xd0
#define	AC100_ALM_INT_STA		0xd1
#define AC100_ALM_SEC			0xd8
#define AC100_ALM_MIN			0xd9
#define AC100_ALM_HOU			0xda
#define AC100_ALM_WEE			0xdb
#define AC100_ALM_DAY			0xdc
#define AC100_ALM_MON			0xdd
#define AC100_ALM_YEA			0xde
#define AC100_ALM_UPD			0xdf


#define AC100_RTC_GP(x)			(0xe0 + (x))

#endif 
