/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _DT_BINDINGS_CLK_MT6877_H
#define _DT_BINDINGS_CLK_MT6877_H


#define CLK_TOP_MFGPLL1					0
#define CLK_TOP_MFGPLL4					1
#define CLK_TOP_MAINPLL_D4				2
#define CLK_TOP_MAINPLL_D4_D2				3
#define CLK_TOP_MAINPLL_D4_D4				4
#define CLK_TOP_MAINPLL_D4_D8				5
#define CLK_TOP_MAINPLL_D4_D16				6
#define CLK_TOP_MAINPLL_D5				7
#define CLK_TOP_MAINPLL_D5_D2				8
#define CLK_TOP_MAINPLL_D5_D4				9
#define CLK_TOP_MAINPLL_D5_D8				10
#define CLK_TOP_MAINPLL_D6				11
#define CLK_TOP_MAINPLL_D6_D2				12
#define CLK_TOP_MAINPLL_D6_D4				13
#define CLK_TOP_MAINPLL_D6_D8				14
#define CLK_TOP_MAINPLL_D7				15
#define CLK_TOP_MAINPLL_D7_D2				16
#define CLK_TOP_MAINPLL_D7_D4				17
#define CLK_TOP_MAINPLL_D7_D8				18
#define CLK_TOP_MAINPLL_D9				19
#define CLK_TOP_UNIVPLL					20
#define CLK_TOP_UNIVPLL_D4				21
#define CLK_TOP_UNIVPLL_D4_D2				22
#define CLK_TOP_UNIVPLL_D4_D4				23
#define CLK_TOP_UNIVPLL_D4_D8				24
#define CLK_TOP_UNIVPLL_D5				25
#define CLK_TOP_UNIVPLL_D5_D2				26
#define CLK_TOP_UNIVPLL_D5_D4				27
#define CLK_TOP_UNIVPLL_D6				28
#define CLK_TOP_UNIVPLL_D6_D2				29
#define CLK_TOP_UNIVPLL_D6_D4				30
#define CLK_TOP_UNIVPLL_D6_D8				31
#define CLK_TOP_UNIVPLL_D6_D16				32
#define CLK_TOP_UNIVPLL_D7				33
#define CLK_TOP_UNIVPLL_D7_D2				34
#define CLK_TOP_UNIVPLL_192M_D2				35
#define CLK_TOP_UNIVPLL_192M_D4				36
#define CLK_TOP_UNIVPLL_192M_D8				37
#define CLK_TOP_UNIVPLL_192M_D16			38
#define CLK_TOP_UNIVPLL_192M_D32			39
#define CLK_TOP_APLL1					40
#define CLK_TOP_APLL1_D2				41
#define CLK_TOP_APLL1_D4				42
#define CLK_TOP_APLL1_D8				43
#define CLK_TOP_APLL2					44
#define CLK_TOP_APLL2_D2				45
#define CLK_TOP_APLL2_D4				46
#define CLK_TOP_APLL2_D8				47
#define CLK_TOP_ADSPPLL					48
#define CLK_TOP_MMPLL_D4				49
#define CLK_TOP_MMPLL_D4_D2				50
#define CLK_TOP_MMPLL_D5				51
#define CLK_TOP_MMPLL_D5_D2				52
#define CLK_TOP_MMPLL_D6				53
#define CLK_TOP_MMPLL_D6_D2				54
#define CLK_TOP_MMPLL_D7				55
#define CLK_TOP_MMPLL_D9				56
#define CLK_TOP_APUPLL					57
#define CLK_TOP_NPUPLL					58
#define CLK_TOP_APUPLL1					59
#define CLK_TOP_APUPLL2					60
#define CLK_TOP_TVDPLL					61
#define CLK_TOP_TVDPLL_D2				62
#define CLK_TOP_MSDCPLL					63
#define CLK_TOP_MSDCPLL_D2				64
#define CLK_TOP_MSDCPLL_D4				65
#define CLK_TOP_CLKRTC					66
#define CLK_TOP_TCK_26M_MX9				67
#define CLK_TOP_F26M_CK_D2				68
#define CLK_TOP_OSC					69
#define CLK_TOP_OSC_D2					70
#define CLK_TOP_OSC_D4					71
#define CLK_TOP_OSC_D7					72
#define CLK_TOP_OSC_D8					73
#define CLK_TOP_OSC_D16					74
#define CLK_TOP_OSC_D10					75
#define CLK_TOP_ULPOSC					76
#define CLK_TOP_F26M					77
#define CLK_TOP_AXI					78
#define CLK_TOP_DISP0					79
#define CLK_TOP_MDP0					80
#define CLK_TOP_IMG1					81
#define CLK_TOP_IPE					82
#define CLK_TOP_DPE					83
#define CLK_TOP_CAM					84
#define CLK_TOP_DSP					85
#define CLK_TOP_DSP1					86
#define CLK_TOP_DSP2					87
#define CLK_TOP_DSP4					88
#define CLK_TOP_DSP7					89
#define CLK_TOP_FUART					90
#define CLK_TOP_SPI					91
#define CLK_TOP_MSDC50_0_HCLK				92
#define CLK_TOP_MSDC50_0				93
#define CLK_TOP_MSDC30_1				94
#define CLK_TOP_AUDIO					95
#define CLK_TOP_AUD_INTBUS				96
#define CLK_TOP_FPWRAP_ULPOSC				97
#define CLK_TOP_FDISP_PWM				98
#define CLK_TOP_FUSB_TOP				99
#define CLK_TOP_FSSUSB_XHCI				100
#define CLK_TOP_I2C					101
#define CLK_TOP_DXCC					102
#define CLK_TOP_AUD_ENGEN1				103
#define CLK_TOP_AUD_ENGEN2				104
#define CLK_TOP_AES_UFSFDE				105
#define CLK_TOP_UFS					106
#define CLK_TOP_AUD_1					107
#define CLK_TOP_AUD_2					108
#define CLK_TOP_ADSP					109
#define CLK_TOP_DPMAIF_MAIN				110
#define CLK_TOP_VENC					111
#define CLK_TOP_VDEC					112
#define CLK_TOP_PWM					113
#define CLK_TOP_AUDIO_H					114
#define CLK_TOP_MEM_SUB					115
#define CLK_TOP_AES_MSDCFDE				116
#define CLK_TOP_UFS_MBIST				117
#define CLK_TOP_MSDC_NEW_RX				118
#define CLK_TOP_MFG_INTERNAL1				119
#define CLK_TOP_MFG_INTERNAL2				120
#define CLK_TOP_FI2C_PSEUDO				121
#define CLK_TOP_AXI_SEL					122
#define CLK_TOP_SPM_SEL					123
#define CLK_TOP_SCP_SEL					124
#define CLK_TOP_BUS_AXIMEM_SEL				125
#define CLK_TOP_DISP0_SEL				126
#define CLK_TOP_MDP0_SEL				127
#define CLK_TOP_IMG1_SEL				128
#define CLK_TOP_IPE_SEL					129
#define CLK_TOP_DPE_SEL					130
#define CLK_TOP_CAM_SEL					131
#define CLK_TOP_CCU_SEL					132
#define CLK_TOP_DSP_SEL					133
#define CLK_TOP_DSP1_SEL				134
#define CLK_TOP_DSP2_SEL				135
#define CLK_TOP_DSP4_SEL				136
#define CLK_TOP_DSP7_SEL				137
#define CLK_TOP_CAMTG_SEL				138
#define CLK_TOP_CAMTG2_SEL				139
#define CLK_TOP_CAMTG3_SEL				140
#define CLK_TOP_CAMTG4_SEL				141
#define CLK_TOP_CAMTG5_SEL				142
#define CLK_TOP_UART_SEL				143
#define CLK_TOP_SPI_SEL					144
#define CLK_TOP_MSDC50_0_HCLK_SEL			145
#define CLK_TOP_MSDC50_0_SEL				146
#define CLK_TOP_MSDC30_1_SEL				147
#define CLK_TOP_AUDIO_SEL				148
#define CLK_TOP_AUD_INTBUS_SEL				149
#define CLK_TOP_PWRAP_ULPOSC_SEL			150
#define CLK_TOP_ATB_SEL					151
#define CLK_TOP_SSPM_SEL				152
#define CLK_TOP_DISP_PWM_SEL				153
#define CLK_TOP_USB_TOP_SEL				154
#define CLK_TOP_SSUSB_XHCI_SEL				155
#define CLK_TOP_I2C_SEL					156
#define CLK_TOP_SENINF_SEL				157
#define CLK_TOP_SENINF1_SEL				158
#define CLK_TOP_SENINF2_SEL				159
#define CLK_TOP_SENINF3_SEL				160
#define CLK_TOP_DXCC_SEL				161
#define CLK_TOP_AUD_ENGEN1_SEL				162
#define CLK_TOP_AUD_ENGEN2_SEL				163
#define CLK_TOP_AES_UFSFDE_SEL				164
#define CLK_TOP_UFS_SEL					165
#define CLK_TOP_AUD_1_SEL				166
#define CLK_TOP_AUD_2_SEL				167
#define CLK_TOP_ADSP_SEL				168
#define CLK_TOP_DPMAIF_MAIN_SEL				169
#define CLK_TOP_VENC_SEL				170
#define CLK_TOP_VDEC_SEL				171
#define CLK_TOP_CAMTM_SEL				172
#define CLK_TOP_PWM_SEL					173
#define CLK_TOP_AUDIO_H_SEL				174
#define CLK_TOP_MCUPM_SEL				175
#define CLK_TOP_SPMI_M_MST_SEL				176
#define CLK_TOP_DVFSRC_SEL				177
#define CLK_TOP_MEM_SUB_SEL				178
#define CLK_TOP_AES_MSDCFDE_SEL				179
#define CLK_TOP_UFS_MBIST_SEL				180
#define CLK_TOP_MFG_INTERNAL2_SEL			181
#define CLK_TOP_MFG_INTERNAL1_SEL			182
#define CLK_TOP_AP2CONN_HOST_SEL			183
#define CLK_TOP_MSDC_NEW_RX_SEL				184
#define CLK_TOP_APLL_I2S0_MCK_SEL			185
#define CLK_TOP_APLL_I2S1_MCK_SEL			186
#define CLK_TOP_APLL_I2S2_MCK_SEL			187
#define CLK_TOP_APLL_I2S3_MCK_SEL			188
#define CLK_TOP_APLL_I2S4_MCK_SEL			189
#define CLK_TOP_APLL_I2S5_MCK_SEL			190
#define CLK_TOP_APLL_I2S6_MCK_SEL			191
#define CLK_TOP_APLL_I2S7_MCK_SEL			192
#define CLK_TOP_APLL_I2S8_MCK_SEL			193
#define CLK_TOP_APLL_I2S9_MCK_SEL			194
#define CLK_TOP_APLL12_CK_DIV0				195
#define CLK_TOP_APLL12_CK_DIV1				196
#define CLK_TOP_APLL12_CK_DIV2				197
#define CLK_TOP_APLL12_CK_DIV3				198
#define CLK_TOP_APLL12_CK_DIV4				199
#define CLK_TOP_APLL12_CK_DIVB				200
#define CLK_TOP_APLL12_CK_DIV5				201
#define CLK_TOP_APLL12_CK_DIV6				202
#define CLK_TOP_APLL12_CK_DIV7				203
#define CLK_TOP_APLL12_CK_DIV8				204
#define CLK_TOP_APLL12_CK_DIV9				205
#define CLK_TOP_NR_CLK					206


#define CLK_IFRAO_PMIC_TMR				0
#define CLK_IFRAO_PMIC_AP				1
#define CLK_IFRAO_PMIC_MD				2
#define CLK_IFRAO_PMIC_CONN				3
#define CLK_IFRAO_APXGPT				4
#define CLK_IFRAO_GCE					5
#define CLK_IFRAO_GCE2					6
#define CLK_IFRAO_THERM					7
#define CLK_IFRAO_I2C_PSEUDO				8
#define CLK_IFRAO_PWM_HCLK				9
#define CLK_IFRAO_PWM1					10
#define CLK_IFRAO_PWM2					11
#define CLK_IFRAO_PWM3					12
#define CLK_IFRAO_PWM4					13
#define CLK_IFRAO_PWM					14
#define CLK_IFRAO_UART0					15
#define CLK_IFRAO_UART1					16
#define CLK_IFRAO_UART2					17
#define CLK_IFRAO_UART3					18
#define CLK_IFRAO_GCE_26M				19
#define CLK_IFRAO_BTIF					20
#define CLK_IFRAO_SPI0					21
#define CLK_IFRAO_MSDC0					22
#define CLK_IFRAO_MSDC1					23
#define CLK_IFRAO_MSDC0_SRC				24
#define CLK_IFRAO_AUXADC				25
#define CLK_IFRAO_CPUM					26
#define CLK_IFRAO_CCIF1_AP				27
#define CLK_IFRAO_CCIF1_MD				28
#define CLK_IFRAO_AUXADC_MD				29
#define CLK_IFRAO_MSDC1_SRC				30
#define CLK_IFRAO_MSDC0_AES				31
#define CLK_IFRAO_DEVICE_APC				32
#define CLK_IFRAO_CCIF_AP				33
#define CLK_IFRAO_AUDIO					34
#define CLK_IFRAO_CCIF_MD				35
#define CLK_IFRAO_SSUSB					36
#define CLK_IFRAO_DISP_PWM				37
#define CLK_IFRAO_CLDMA_BCLK				38
#define CLK_IFRAO_AUDIO_26M_BCLK			39
#define CLK_IFRAO_SPI1					40
#define CLK_IFRAO_SPI2					41
#define CLK_IFRAO_SPI3					42
#define CLK_IFRAO_UNIPRO_SYSCLK				43
#define CLK_IFRAO_UFS_MP_SAP_BCLK			44
#define CLK_IFRAO_APDMA					45
#define CLK_IFRAO_SPI4					46
#define CLK_IFRAO_SPI5					47
#define CLK_IFRAO_CQ_DMA				48
#define CLK_IFRAO_UFS					49
#define CLK_IFRAO_AES_UFSFDE				50
#define CLK_IFRAO_SSUSB_XHCI				51
#define CLK_IFRAO_AP_MSDC0				52
#define CLK_IFRAO_MD_MSDC0				53
#define CLK_IFRAO_CCIF5_MD				54
#define CLK_IFRAO_CCIF2_AP				55
#define CLK_IFRAO_CCIF2_MD				56
#define CLK_IFRAO_FBIST2FPC				57
#define CLK_IFRAO_DPMAIF_MAIN				58
#define CLK_IFRAO_CCIF4_MD				59
#define CLK_IFRAO_SPI6_CK				60
#define CLK_IFRAO_SPI7_CK				61
#define CLK_IFRAO_RG_AES_CK_0P_CK			62
#define CLK_IFRAO_NR_CLK				63


#define CLK_APMIXED_ARMPLL_LL				0
#define CLK_APMIXED_ARMPLL_BL				1
#define CLK_APMIXED_CCIPLL				2
#define CLK_APMIXED_MAINPLL				3
#define CLK_APMIXED_UNIVPLL				4
#define CLK_APMIXED_MSDCPLL				5
#define CLK_APMIXED_MMPLL				6
#define CLK_APMIXED_ADSPPLL				7
#define CLK_APMIXED_TVDPLL				8
#define CLK_APMIXED_APLL1				9
#define CLK_APMIXED_APLL2				10
#define CLK_APMIXED_MPLL				11
#define CLK_APMIXED_USBPLL				12
#define CLK_APMIXED_NR_CLK				13


#define CLK_SCP_PAR_RG_AUDIODSP				0
#define CLK_SCP_PAR_NR_CLK				1


#define CLK_AUDSYS_AFE					0
#define CLK_AUDSYS_22M					1
#define CLK_AUDSYS_24M					2
#define CLK_AUDSYS_APLL2_TUNER				3
#define CLK_AUDSYS_APLL_TUNER				4
#define CLK_AUDSYS_TDM					5
#define CLK_AUDSYS_ADC					6
#define CLK_AUDSYS_DAC					7
#define CLK_AUDSYS_DAC_PREDIS				8
#define CLK_AUDSYS_TML					9
#define CLK_AUDSYS_NLE					10
#define CLK_AUDSYS_CONNSYS_I2S_ASRC			11
#define CLK_AUDSYS_GENERAL1_ASRC			12
#define CLK_AUDSYS_GENERAL2_ASRC			13
#define CLK_AUDSYS_DAC_HIRES				14
#define CLK_AUDSYS_ADC_HIRES				15
#define CLK_AUDSYS_ADC_HIRES_TML			16
#define CLK_AUDSYS_ADDA6_ADC				17
#define CLK_AUDSYS_ADDA6_ADC_HIRES			18
#define CLK_AUDSYS_3RD_DAC				19
#define CLK_AUDSYS_3RD_DAC_PREDIS			20
#define CLK_AUDSYS_3RD_DAC_TML				21
#define CLK_AUDSYS_3RD_DAC_HIRES			22
#define CLK_AUDSYS_NR_CLK				23


#define CLK_MSDC0_MSDC_NEW_RX_PATH_SEL			0
#define CLK_MSDC0_NR_CLK				1


#define CLK_IMPC_AP_CLOCK_I2C10				0
#define CLK_IMPC_AP_CLOCK_I2C11				1
#define CLK_IMPC_NR_CLK					2


#define CLK_IMPE_AP_CLOCK_I2C3				0
#define CLK_IMPE_NR_CLK					1


#define CLK_IMPS_AP_CLOCK_I2C5				0
#define CLK_IMPS_AP_CLOCK_I2C7				1
#define CLK_IMPS_AP_CLOCK_I2C8				2
#define CLK_IMPS_AP_CLOCK_I2C9				3
#define CLK_IMPS_NR_CLK					4


#define CLK_IMPWS_AP_CLOCK_I2C1				0
#define CLK_IMPWS_AP_CLOCK_I2C2				1
#define CLK_IMPWS_AP_CLOCK_I2C4				2
#define CLK_IMPWS_NR_CLK				3


#define CLK_IMPW_AP_CLOCK_I2C0				0
#define CLK_IMPW_NR_CLK					1


#define CLK_IMPN_AP_CLOCK_I2C6				0
#define CLK_IMPN_NR_CLK					1


#define CLK_MFG_AO_MFGPLL1				0
#define CLK_MFG_AO_MFGPLL4				1
#define CLK_MFG_AO_NR_CLK				2


#define CLK_MFGCFG_BG3D					0
#define CLK_MFGCFG_NR_CLK				1


#define CLK_MM_DISP_MUTEX0				0
#define CLK_MM_APB_BUS					1
#define CLK_MM_DISP_OVL0				2
#define CLK_MM_DISP_RDMA0				3
#define CLK_MM_DISP_OVL0_2L				4
#define CLK_MM_DISP_WDMA0				5
#define CLK_MM_DISP_CCORR1				6
#define CLK_MM_DISP_RSZ0				7
#define CLK_MM_DISP_AAL0				8
#define CLK_MM_DISP_CCORR0				9
#define CLK_MM_DISP_COLOR0				10
#define CLK_MM_SMI_INFRA				11
#define CLK_MM_DISP_GAMMA0				12
#define CLK_MM_DISP_POSTMASK0				13
#define CLK_MM_DISP_SPR0				14
#define CLK_MM_DISP_DITHER0				15
#define CLK_MM_SMI_COMMON				16
#define CLK_MM_DISP_CM0					17
#define CLK_MM_DSI0					18
#define CLK_MM_SMI_GALS					19
#define CLK_MM_DISP_DSC_WRAP				20
#define CLK_MM_SMI_IOMMU				21
#define CLK_MM_DISP_OVL1_2L				22
#define CLK_MM_DISP_UFBC_WDMA0				23
#define CLK_MM_DSI0_DSI_CK_DOMAIN			24
#define CLK_MM_DISP_26M					25
#define CLK_MM_NR_CLK					26


#define CLK_IMGSYS1_LARB9				0
#define CLK_IMGSYS1_DIP					1
#define CLK_IMGSYS1_GALS				2
#define CLK_IMGSYS1_NR_CLK				3


#define CLK_IMGSYS2_LARB9				0
#define CLK_IMGSYS2_LARB10				1
#define CLK_IMGSYS2_MFB					2
#define CLK_IMGSYS2_WPE					3
#define CLK_IMGSYS2_MSS					4
#define CLK_IMGSYS2_GALS				5
#define CLK_IMGSYS2_NR_CLK				6


#define CLK_VDE2_VDEC_CKEN				0
#define CLK_VDE2_NR_CLK					1


#define CLK_VEN1_CKE0_LARB				0
#define CLK_VEN1_CKE1_VENC				1
#define CLK_VEN1_CKE2_JPGENC				2
#define CLK_VEN1_CKE5_GALS				3
#define CLK_VEN1_NR_CLK					4


#define CLK_APU_CONN2_AHB				0
#define CLK_APU_CONN2_AXI				1
#define CLK_APU_CONN2_ISP				2
#define CLK_APU_CONN2_CAM_ADL				3
#define CLK_APU_CONN2_IMG_ADL				4
#define CLK_APU_CONN2_EMI_26M				5
#define CLK_APU_CONN2_VPU_UDI				6
#define CLK_APU_CONN2_EDMA_0				7
#define CLK_APU_CONN2_EDMA_1				8
#define CLK_APU_CONN2_EDMAL_0				9
#define CLK_APU_CONN2_EDMAL_1				10
#define CLK_APU_CONN2_MNOC				11
#define CLK_APU_CONN2_TCM				12
#define CLK_APU_CONN2_MD32				13
#define CLK_APU_CONN2_IOMMU_0				14
#define CLK_APU_CONN2_IOMMU_1				15
#define CLK_APU_CONN2_MD32_32K				16
#define CLK_APU_CONN2_CPE				17
#define CLK_APU_CONN2_NR_CLK				18


#define CLK_APU_CONN1_AXI				0
#define CLK_APU_CONN1_EDMA_0				1
#define CLK_APU_CONN1_EDMA_1				2
#define CLK_APU_CONN1_IOMMU_0				3
#define CLK_APU_CONN1_IOMMU_1				4
#define CLK_APU_CONN1_NR_CLK				5


#define CLK_APUV_AHB					0
#define CLK_APUV_AXI					1
#define CLK_APUV_ADL					2
#define CLK_APUV_QOS					3
#define CLK_APUV_NR_CLK					4


#define CLK_APU0_APU					0
#define CLK_APU0_AXI_M					1
#define CLK_APU0_JTAG					2
#define CLK_APU0_NR_CLK					3


#define CLK_APU1_APU					0
#define CLK_APU1_AXI_M					1
#define CLK_APU1_JTAG					2
#define CLK_APU1_NR_CLK					3


#define CLK_APUM0_MDLA_CG0				0
#define CLK_APUM0_MDLA_CG1				1
#define CLK_APUM0_MDLA_CG2				2
#define CLK_APUM0_MDLA_CG3				3
#define CLK_APUM0_MDLA_CG4				4
#define CLK_APUM0_MDLA_CG5				5
#define CLK_APUM0_MDLA_CG6				6
#define CLK_APUM0_MDLA_CG7				7
#define CLK_APUM0_MDLA_CG8				8
#define CLK_APUM0_MDLA_CG9				9
#define CLK_APUM0_MDLA_CG10				10
#define CLK_APUM0_MDLA_CG11				11
#define CLK_APUM0_MDLA_CG12				12
#define CLK_APUM0_APB					13
#define CLK_APUM0_AXI_M					14
#define CLK_APUM0_NR_CLK				15


#define CLK_APU_AO_APUPLL				0
#define CLK_APU_AO_NPUPLL				1
#define CLK_APU_AO_APUPLL1				2
#define CLK_APU_AO_APUPLL2				3
#define CLK_APU_AO_NR_CLK				4


#define CLK_CAM_M_LARB13				0
#define CLK_CAM_M_LARB14				1
#define CLK_CAM_M_CAM					2
#define CLK_CAM_M_CAMTG					3
#define CLK_CAM_M_SENINF				4
#define CLK_CAM_M_CAMSV0				5
#define CLK_CAM_M_CAMSV1				6
#define CLK_CAM_M_CAMSV2				7
#define CLK_CAM_M_CAMSV3				8
#define CLK_CAM_M_CCU0					9
#define CLK_CAM_M_CCU1					10
#define CLK_CAM_M_MRAW0					11
#define CLK_CAM_M_CCU_GALS				12
#define CLK_CAM_M_CAM2MM_GALS				13
#define CLK_CAM_M_CAMSV4				14
#define CLK_CAM_M_PDA					15
#define CLK_CAM_M_NR_CLK				16


#define CLK_CAM_RA_LARBX				0
#define CLK_CAM_RA_CAM					1
#define CLK_CAM_RA_CAMTG				2
#define CLK_CAM_RA_NR_CLK				3


#define CLK_CAM_RB_LARBX				0
#define CLK_CAM_RB_CAM					1
#define CLK_CAM_RB_CAMTG				2
#define CLK_CAM_RB_NR_CLK				3


#define CLK_IPE_LARB19					0
#define CLK_IPE_LARB20					1
#define CLK_IPE_SMI_SUBCOM				2
#define CLK_IPE_FD					3
#define CLK_IPE_FE					4
#define CLK_IPE_RSC					5
#define CLK_IPE_DPE					6
#define CLK_IPE_GALS					7
#define CLK_IPE_NR_CLK					8


#define CLK_MDP_RDMA0					0
#define CLK_MDP_TDSHP0					1
#define CLK_MDP_IMG_DL_ASYNC0				2
#define CLK_MDP_IMG_DL_ASYNC1				3
#define CLK_MDP_RDMA1					4
#define CLK_MDP_TDSHP1					5
#define CLK_MDP_SMI0					6
#define CLK_MDP_APB_BUS					7
#define CLK_MDP_WROT0					8
#define CLK_MDP_RSZ0					9
#define CLK_MDP_HDR0					10
#define CLK_MDP_MUTEX0					11
#define CLK_MDP_WROT1					12
#define CLK_MDP_COLOR0					13
#define CLK_MDP_AAL0					14
#define CLK_MDP_AAL1					15
#define CLK_MDP_RSZ1					16
#define CLK_MDP_IMG_DL_RELAY0_ASYNC0			17
#define CLK_MDP_IMG_DL_RELAY1_ASYNC1			18
#define CLK_MDP_NR_CLK					19


#define SCP_SYS_MFG0					0
#define SCP_SYS_MFG1					1
#define SCP_SYS_MFG2					2
#define SCP_SYS_MFG3					3
#define SCP_SYS_MFG4					4
#define SCP_SYS_MFG5					5
#define SCP_SYS_MD					6
#define SCP_SYS_CONN					7
#define SCP_SYS_ISP0					8
#define SCP_SYS_ISP1					9
#define SCP_SYS_IPE					10
#define SCP_SYS_VDEC					11
#define SCP_SYS_VENC					12
#define SCP_SYS_DISP					13
#define SCP_SYS_AUDIO					14
#define SCP_SYS_ADSP_DORMANT				15
#define SCP_SYS_CAM					16
#define SCP_SYS_CAM_RAWA				17
#define SCP_SYS_CAM_RAWB				18
#define SCP_SYS_CSI					19
#define SCP_SYS_APU					20
#define SCP_NR_SYSS					21

#endif 
