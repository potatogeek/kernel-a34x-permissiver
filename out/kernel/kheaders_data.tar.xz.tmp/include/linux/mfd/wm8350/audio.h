

#ifndef __LINUX_MFD_WM8350_AUDIO_H_
#define __LINUX_MFD_WM8350_AUDIO_H_

#include <linux/platform_device.h>

#define WM8350_CLOCK_CONTROL_1                  0x28
#define WM8350_CLOCK_CONTROL_2                  0x29
#define WM8350_FLL_CONTROL_1                    0x2A
#define WM8350_FLL_CONTROL_2                    0x2B
#define WM8350_FLL_CONTROL_3                    0x2C
#define WM8350_FLL_CONTROL_4                    0x2D
#define WM8350_DAC_CONTROL                      0x30
#define WM8350_DAC_DIGITAL_VOLUME_L             0x32
#define WM8350_DAC_DIGITAL_VOLUME_R             0x33
#define WM8350_DAC_LR_RATE                      0x35
#define WM8350_DAC_CLOCK_CONTROL                0x36
#define WM8350_DAC_MUTE                         0x3A
#define WM8350_DAC_MUTE_VOLUME                  0x3B
#define WM8350_DAC_SIDE                         0x3C
#define WM8350_ADC_CONTROL                      0x40
#define WM8350_ADC_DIGITAL_VOLUME_L             0x42
#define WM8350_ADC_DIGITAL_VOLUME_R             0x43
#define WM8350_ADC_DIVIDER                      0x44
#define WM8350_ADC_LR_RATE                      0x46
#define WM8350_INPUT_CONTROL                    0x48
#define WM8350_IN3_INPUT_CONTROL                0x49
#define WM8350_MIC_BIAS_CONTROL                 0x4A
#define WM8350_OUTPUT_CONTROL                   0x4C
#define WM8350_JACK_DETECT                      0x4D
#define WM8350_ANTI_POP_CONTROL                 0x4E
#define WM8350_LEFT_INPUT_VOLUME                0x50
#define WM8350_RIGHT_INPUT_VOLUME               0x51
#define WM8350_LEFT_MIXER_CONTROL               0x58
#define WM8350_RIGHT_MIXER_CONTROL              0x59
#define WM8350_OUT3_MIXER_CONTROL               0x5C
#define WM8350_OUT4_MIXER_CONTROL               0x5D
#define WM8350_OUTPUT_LEFT_MIXER_VOLUME         0x60
#define WM8350_OUTPUT_RIGHT_MIXER_VOLUME        0x61
#define WM8350_INPUT_MIXER_VOLUME_L             0x62
#define WM8350_INPUT_MIXER_VOLUME_R             0x63
#define WM8350_INPUT_MIXER_VOLUME               0x64
#define WM8350_LOUT1_VOLUME                     0x68
#define WM8350_ROUT1_VOLUME                     0x69
#define WM8350_LOUT2_VOLUME                     0x6A
#define WM8350_ROUT2_VOLUME                     0x6B
#define WM8350_BEEP_VOLUME                      0x6F
#define WM8350_AI_FORMATING                     0x70
#define WM8350_ADC_DAC_COMP                     0x71
#define WM8350_AI_ADC_CONTROL                   0x72
#define WM8350_AI_DAC_CONTROL                   0x73
#define WM8350_AIF_TEST                         0x74
#define WM8350_JACK_PIN_STATUS                  0xE7


#define WM8350_CODEC_ISEL_1_5                   0	
#define WM8350_CODEC_ISEL_1_0                   1	
#define WM8350_CODEC_ISEL_0_75                  2	
#define WM8350_CODEC_ISEL_0_5                   3	

#define WM8350_VMID_OFF                         0
#define WM8350_VMID_300K                        1
#define WM8350_VMID_50K                         2
#define WM8350_VMID_5K                          3


#define WM8350_TOCLK_RATE                       0x4000
#define WM8350_MCLK_SEL                         0x0800
#define WM8350_MCLK_DIV_MASK                    0x0100
#define WM8350_BCLK_DIV_MASK                    0x00F0
#define WM8350_OPCLK_DIV_MASK                   0x0007


#define WM8350_LRC_ADC_SEL                      0x8000
#define WM8350_MCLK_DIR                         0x0001


#define WM8350_FLL_DITHER_WIDTH_MASK            0x3000
#define WM8350_FLL_DITHER_HP                    0x0800
#define WM8350_FLL_OUTDIV_MASK                  0x0700
#define WM8350_FLL_RSP_RATE_MASK                0x00F0
#define WM8350_FLL_RATE_MASK                    0x0007


#define WM8350_FLL_RATIO_MASK                   0xF800
#define WM8350_FLL_N_MASK                       0x03FF


#define WM8350_FLL_K_MASK                       0xFFFF


#define WM8350_FLL_FRAC                         0x0020
#define WM8350_FLL_SLOW_LOCK_REF                0x0010
#define WM8350_FLL_CLK_SRC_MASK                 0x0003


#define WM8350_DAC_MONO                         0x2000
#define WM8350_AIF_LRCLKRATE                    0x1000
#define WM8350_DEEMP_MASK                       0x0030
#define WM8350_DACL_DATINV                      0x0002
#define WM8350_DACR_DATINV                      0x0001


#define WM8350_DAC_VU                           0x0100
#define WM8350_DACL_VOL_MASK                    0x00FF


#define WM8350_DAC_VU                           0x0100
#define WM8350_DACR_VOL_MASK                    0x00FF


#define WM8350_DACLRC_ENA                       0x0800
#define WM8350_DACLRC_RATE_MASK                 0x07FF


#define WM8350_DACCLK_POL                       0x0010
#define WM8350_DAC_CLKDIV_MASK                  0x0007


#define WM8350_DAC_MUTE_ENA                     0x4000


#define WM8350_DAC_MUTEMODE                     0x4000
#define WM8350_DAC_MUTERATE                     0x2000
#define WM8350_DAC_SB_FILT                      0x1000


#define WM8350_ADC_TO_DACL_MASK                 0x3000
#define WM8350_ADC_TO_DACR_MASK                 0x0C00


#define WM8350_ADC_HPF_CUT_MASK                 0x0300
#define WM8350_ADCL_DATINV                      0x0002
#define WM8350_ADCR_DATINV                      0x0001


#define WM8350_ADC_VU                           0x0100
#define WM8350_ADCL_VOL_MASK                    0x00FF


#define WM8350_ADC_VU                           0x0100
#define WM8350_ADCR_VOL_MASK                    0x00FF


#define WM8350_ADCL_DAC_SVOL_MASK               0x0F00
#define WM8350_ADCR_DAC_SVOL_MASK               0x00F0
#define WM8350_ADCCLK_POL                       0x0008
#define WM8350_ADC_CLKDIV_MASK                  0x0007


#define WM8350_ADCLRC_ENA                       0x0800
#define WM8350_ADCLRC_RATE_MASK                 0x07FF


#define WM8350_IN2R_ENA                         0x0400
#define WM8350_IN1RN_ENA                        0x0200
#define WM8350_IN1RP_ENA                        0x0100
#define WM8350_IN2L_ENA                         0x0004
#define WM8350_IN1LN_ENA                        0x0002
#define WM8350_IN1LP_ENA                        0x0001


#define WM8350_IN3R_SHORT                       0x4000
#define WM8350_IN3L_SHORT                       0x0040


#define WM8350_MICBSEL                          0x4000
#define WM8350_MCDTHR_MASK                      0x001C
#define WM8350_MCDSCTHR_MASK                    0x0003


#define WM8350_OUT4_VROI                        0x0800
#define WM8350_OUT3_VROI                        0x0400
#define WM8350_OUT2_VROI                        0x0200
#define WM8350_OUT1_VROI                        0x0100
#define WM8350_OUT2_FB                          0x0004
#define WM8350_OUT1_FB                          0x0001


#define WM8350_JDL_ENA                          0x8000
#define WM8350_JDR_ENA                          0x4000


#define WM8350_ANTI_POP_MASK                    0x0300
#define WM8350_DIS_OP_LN4_MASK                  0x00C0
#define WM8350_DIS_OP_LN3_MASK                  0x0030
#define WM8350_DIS_OP_OUT2_MASK                 0x000C
#define WM8350_DIS_OP_OUT1_MASK                 0x0003


#define WM8350_INL_MUTE                         0x4000
#define WM8350_INL_ZC                           0x2000
#define WM8350_IN_VU                            0x0100
#define WM8350_INL_VOL_MASK                     0x00FC


#define WM8350_INR_MUTE                         0x4000
#define WM8350_INR_ZC                           0x2000
#define WM8350_IN_VU                            0x0100
#define WM8350_INR_VOL_MASK                     0x00FC


#define WM8350_DACR_TO_MIXOUTL                  0x1000
#define WM8350_DACL_TO_MIXOUTL                  0x0800
#define WM8350_IN3L_TO_MIXOUTL                  0x0004
#define WM8350_INR_TO_MIXOUTL                   0x0002
#define WM8350_INL_TO_MIXOUTL                   0x0001


#define WM8350_DACR_TO_MIXOUTR                  0x1000
#define WM8350_DACL_TO_MIXOUTR                  0x0800
#define WM8350_IN3R_TO_MIXOUTR                  0x0008
#define WM8350_INR_TO_MIXOUTR                   0x0002
#define WM8350_INL_TO_MIXOUTR                   0x0001


#define WM8350_DACL_TO_OUT3                     0x0800
#define WM8350_MIXINL_TO_OUT3                   0x0100
#define WM8350_OUT4_TO_OUT3                     0x0008
#define WM8350_MIXOUTL_TO_OUT3                  0x0001


#define WM8350_DACR_TO_OUT4                     0x1000
#define WM8350_DACL_TO_OUT4                     0x0800
#define WM8350_OUT4_ATTN                        0x0400
#define WM8350_MIXINR_TO_OUT4                   0x0200
#define WM8350_OUT3_TO_OUT4                     0x0004
#define WM8350_MIXOUTR_TO_OUT4                  0x0002
#define WM8350_MIXOUTL_TO_OUT4                  0x0001


#define WM8350_IN3L_MIXOUTL_VOL_MASK            0x0E00
#define WM8350_IN3L_MIXOUTL_VOL_SHIFT                9
#define WM8350_INR_MIXOUTL_VOL_MASK             0x00E0
#define WM8350_INR_MIXOUTL_VOL_SHIFT                 5
#define WM8350_INL_MIXOUTL_VOL_MASK             0x000E
#define WM8350_INL_MIXOUTL_VOL_SHIFT                 1


#define WM8350_IN3L_MIXOUTL_VOL_OFF                  0
#define WM8350_IN3L_MIXOUTL_VOL_M12DB                1
#define WM8350_IN3L_MIXOUTL_VOL_M9DB                 2
#define WM8350_IN3L_MIXOUTL_VOL_M6DB                 3
#define WM8350_IN3L_MIXOUTL_VOL_M3DB                 4
#define WM8350_IN3L_MIXOUTL_VOL_0DB                  5
#define WM8350_IN3L_MIXOUTL_VOL_3DB                  6
#define WM8350_IN3L_MIXOUTL_VOL_6DB                  7

#define WM8350_INR_MIXOUTL_VOL_OFF                   0
#define WM8350_INR_MIXOUTL_VOL_M12DB                 1
#define WM8350_INR_MIXOUTL_VOL_M9DB                  2
#define WM8350_INR_MIXOUTL_VOL_M6DB                  3
#define WM8350_INR_MIXOUTL_VOL_M3DB                  4
#define WM8350_INR_MIXOUTL_VOL_0DB                   5
#define WM8350_INR_MIXOUTL_VOL_3DB                   6
#define WM8350_INR_MIXOUTL_VOL_6DB                   7

#define WM8350_INL_MIXOUTL_VOL_OFF                   0
#define WM8350_INL_MIXOUTL_VOL_M12DB                 1
#define WM8350_INL_MIXOUTL_VOL_M9DB                  2
#define WM8350_INL_MIXOUTL_VOL_M6DB                  3
#define WM8350_INL_MIXOUTL_VOL_M3DB                  4
#define WM8350_INL_MIXOUTL_VOL_0DB                   5
#define WM8350_INL_MIXOUTL_VOL_3DB                   6
#define WM8350_INL_MIXOUTL_VOL_6DB                   7


#define WM8350_IN3R_MIXOUTR_VOL_MASK            0xE000
#define WM8350_IN3R_MIXOUTR_VOL_SHIFT               13
#define WM8350_INR_MIXOUTR_VOL_MASK             0x00E0
#define WM8350_INR_MIXOUTR_VOL_SHIFT                 5
#define WM8350_INL_MIXOUTR_VOL_MASK             0x000E
#define WM8350_INL_MIXOUTR_VOL_SHIFT                 1


#define WM8350_IN3R_MIXOUTR_VOL_OFF                  0
#define WM8350_IN3R_MIXOUTR_VOL_M12DB                1
#define WM8350_IN3R_MIXOUTR_VOL_M9DB                 2
#define WM8350_IN3R_MIXOUTR_VOL_M6DB                 3
#define WM8350_IN3R_MIXOUTR_VOL_M3DB                 4
#define WM8350_IN3R_MIXOUTR_VOL_0DB                  5
#define WM8350_IN3R_MIXOUTR_VOL_3DB                  6
#define WM8350_IN3R_MIXOUTR_VOL_6DB                  7

#define WM8350_INR_MIXOUTR_VOL_OFF                   0
#define WM8350_INR_MIXOUTR_VOL_M12DB                 1
#define WM8350_INR_MIXOUTR_VOL_M9DB                  2
#define WM8350_INR_MIXOUTR_VOL_M6DB                  3
#define WM8350_INR_MIXOUTR_VOL_M3DB                  4
#define WM8350_INR_MIXOUTR_VOL_0DB                   5
#define WM8350_INR_MIXOUTR_VOL_3DB                   6
#define WM8350_INR_MIXOUTR_VOL_6DB                   7

#define WM8350_INL_MIXOUTR_VOL_OFF                   0
#define WM8350_INL_MIXOUTR_VOL_M12DB                 1
#define WM8350_INL_MIXOUTR_VOL_M9DB                  2
#define WM8350_INL_MIXOUTR_VOL_M6DB                  3
#define WM8350_INL_MIXOUTR_VOL_M3DB                  4
#define WM8350_INL_MIXOUTR_VOL_0DB                   5
#define WM8350_INL_MIXOUTR_VOL_3DB                   6
#define WM8350_INL_MIXOUTR_VOL_6DB                   7


#define WM8350_IN3L_MIXINL_VOL_MASK             0x0E00
#define WM8350_IN2L_MIXINL_VOL_MASK             0x000E
#define WM8350_INL_MIXINL_VOL                   0x0001


#define WM8350_IN3R_MIXINR_VOL_MASK             0xE000
#define WM8350_IN2R_MIXINR_VOL_MASK             0x00E0
#define WM8350_INR_MIXINR_VOL                   0x0001


#define WM8350_OUT4_MIXIN_DST                   0x8000
#define WM8350_OUT4_MIXIN_VOL_MASK              0x000E


#define WM8350_OUT1L_MUTE                       0x4000
#define WM8350_OUT1L_ZC                         0x2000
#define WM8350_OUT1_VU                          0x0100
#define WM8350_OUT1L_VOL_MASK                   0x00FC
#define WM8350_OUT1L_VOL_SHIFT                       2


#define WM8350_OUT1R_MUTE                       0x4000
#define WM8350_OUT1R_ZC                         0x2000
#define WM8350_OUT1_VU                          0x0100
#define WM8350_OUT1R_VOL_MASK                   0x00FC
#define WM8350_OUT1R_VOL_SHIFT                       2


#define WM8350_OUT2L_MUTE                       0x4000
#define WM8350_OUT2L_ZC                         0x2000
#define WM8350_OUT2_VU                          0x0100
#define WM8350_OUT2L_VOL_MASK                   0x00FC


#define WM8350_OUT2R_MUTE                       0x4000
#define WM8350_OUT2R_ZC                         0x2000
#define WM8350_OUT2R_INV                        0x0400
#define WM8350_OUT2R_INV_MUTE                   0x0200
#define WM8350_OUT2_VU                          0x0100
#define WM8350_OUT2R_VOL_MASK                   0x00FC


#define WM8350_IN3R_OUT2R_VOL_MASK              0x00E0


#define WM8350_AIF_BCLK_INV                     0x8000
#define WM8350_AIF_TRI                          0x2000
#define WM8350_AIF_LRCLK_INV                    0x1000
#define WM8350_AIF_WL_MASK                      0x0C00
#define WM8350_AIF_FMT_MASK                     0x0300


#define WM8350_DAC_COMP                         0x0080
#define WM8350_DAC_COMPMODE                     0x0040
#define WM8350_ADC_COMP                         0x0020
#define WM8350_ADC_COMPMODE                     0x0010
#define WM8350_LOOPBACK                         0x0001


#define WM8350_AIFADC_PD                        0x0080
#define WM8350_AIFADCL_SRC                      0x0040
#define WM8350_AIFADCR_SRC                      0x0020
#define WM8350_AIFADC_TDM_CHAN                  0x0010
#define WM8350_AIFADC_TDM                       0x0008


#define WM8350_BCLK_MSTR                        0x4000
#define WM8350_AIFDAC_PD                        0x0080
#define WM8350_DACL_SRC                         0x0040
#define WM8350_DACR_SRC                         0x0020
#define WM8350_AIFDAC_TDM_CHAN                  0x0010
#define WM8350_AIFDAC_TDM                       0x0008
#define WM8350_DAC_BOOST_MASK                   0x0003


#define WM8350_CODEC_BYP                        0x4000
#define WM8350_AIFADC_WR_TST                    0x2000
#define WM8350_AIFADC_RD_TST                    0x1000
#define WM8350_AIFDAC_WR_TST                    0x0800
#define WM8350_AIFDAC_RD_TST                    0x0400
#define WM8350_AIFADC_ASYN                      0x0020
#define WM8350_AIFDAC_ASYN                      0x0010


#define WM8350_JACK_L_LVL			0x0800
#define WM8350_JACK_R_LVL                       0x0400
#define WM8350_JACK_MICSCD_LVL			0x0200
#define WM8350_JACK_MICSD_LVL			0x0100


#define WM8350_S_CURVE_NONE			0x0
#define WM8350_S_CURVE_FAST			0x1
#define WM8350_S_CURVE_MEDIUM			0x2
#define WM8350_S_CURVE_SLOW			0x3

#define WM8350_DISCHARGE_OFF			0x0
#define WM8350_DISCHARGE_FAST			0x1
#define WM8350_DISCHARGE_MEDIUM			0x2
#define WM8350_DISCHARGE_SLOW			0x3

#define WM8350_TIE_OFF_500R			0x0
#define WM8350_TIE_OFF_30K			0x1


#define WM8350_SYSCLK				0

#define WM8350_MCLK_SEL_PLL_MCLK		0
#define WM8350_MCLK_SEL_PLL_DAC			1
#define WM8350_MCLK_SEL_PLL_ADC			2
#define WM8350_MCLK_SEL_PLL_32K			3
#define WM8350_MCLK_SEL_MCLK			5


#define WM8350_ADC_CLKDIV			0
#define WM8350_DAC_CLKDIV			1
#define WM8350_BCLK_CLKDIV			2
#define WM8350_OPCLK_CLKDIV			3
#define WM8350_TO_CLKDIV			4
#define WM8350_SYS_CLKDIV			5
#define WM8350_DACLR_CLKDIV			6
#define WM8350_ADCLR_CLKDIV			7


#define WM8350_ADCDIV_1				0x0
#define WM8350_ADCDIV_1_5			0x1
#define WM8350_ADCDIV_2				0x2
#define WM8350_ADCDIV_3				0x3
#define WM8350_ADCDIV_4				0x4
#define WM8350_ADCDIV_5_5			0x5
#define WM8350_ADCDIV_6				0x6


#define WM8350_DACDIV_1				0x0
#define WM8350_DACDIV_1_5			0x1
#define WM8350_DACDIV_2				0x2
#define WM8350_DACDIV_3				0x3
#define WM8350_DACDIV_4				0x4
#define WM8350_DACDIV_5_5			0x5
#define WM8350_DACDIV_6				0x6


#define WM8350_BCLK_DIV_1			(0x0 << 4)
#define WM8350_BCLK_DIV_1_5			(0x1 << 4)
#define WM8350_BCLK_DIV_2			(0x2 << 4)
#define WM8350_BCLK_DIV_3			(0x3 << 4)
#define WM8350_BCLK_DIV_4			(0x4 << 4)
#define WM8350_BCLK_DIV_5_5			(0x5 << 4)
#define WM8350_BCLK_DIV_6			(0x6 << 4)
#define WM8350_BCLK_DIV_8			(0x7 << 4)
#define WM8350_BCLK_DIV_11			(0x8 << 4)
#define WM8350_BCLK_DIV_12			(0x9 << 4)
#define WM8350_BCLK_DIV_16			(0xa << 4)
#define WM8350_BCLK_DIV_22			(0xb << 4)
#define WM8350_BCLK_DIV_24			(0xc << 4)
#define WM8350_BCLK_DIV_32			(0xd << 4)
#define WM8350_BCLK_DIV_44			(0xe << 4)
#define WM8350_BCLK_DIV_48			(0xf << 4)


#define WM8350_MCLK_DIV_1			(0x0 << 8)
#define WM8350_MCLK_DIV_2			(0x1 << 8)


#define WM8350_OPCLK_DIV_1			0x0
#define WM8350_OPCLK_DIV_2			0x1
#define WM8350_OPCLK_DIV_3			0x2
#define WM8350_OPCLK_DIV_4			0x3
#define WM8350_OPCLK_DIV_5_5			0x4
#define WM8350_OPCLK_DIV_6			0x5


#define WM8350_HIFI_DAI				0


#define WM8350_IRQ_CODEC_JCK_DET_L		39
#define WM8350_IRQ_CODEC_JCK_DET_R		40
#define WM8350_IRQ_CODEC_MICSCD			41
#define WM8350_IRQ_CODEC_MICD			42


struct wm8350_audio_platform_data {
	int vmid_discharge_msecs;	
	int drain_msecs;	
	int cap_discharge_msecs;	
	int vmid_charge_msecs;	
	u32 vmid_s_curve:2;	
	u32 dis_out4:2;		
	u32 dis_out3:2;		
	u32 dis_out2:2;		
	u32 dis_out1:2;		
	u32 vroi_out4:1;	
	u32 vroi_out3:1;	
	u32 vroi_out2:1;	
	u32 vroi_out1:1;	
	u32 vroi_enable:1;	
	u32 codec_current_on:2;	
	u32 codec_current_standby:2;	
	u32 codec_current_charge:2;	
};

struct wm8350_codec {
	struct platform_device *pdev;
	struct wm8350_audio_platform_data *platform_data;
};

#endif
