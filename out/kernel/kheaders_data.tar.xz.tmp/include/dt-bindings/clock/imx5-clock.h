

#ifndef __DT_BINDINGS_CLOCK_IMX5_H
#define __DT_BINDINGS_CLOCK_IMX5_H

#define IMX5_CLK_DUMMY			0
#define IMX5_CLK_CKIL			1
#define IMX5_CLK_OSC			2
#define IMX5_CLK_CKIH1			3
#define IMX5_CLK_CKIH2			4
#define IMX5_CLK_AHB			5
#define IMX5_CLK_IPG			6
#define IMX5_CLK_AXI_A			7
#define IMX5_CLK_AXI_B			8
#define IMX5_CLK_UART_PRED		9
#define IMX5_CLK_UART_ROOT		10
#define IMX5_CLK_ESDHC_A_PRED		11
#define IMX5_CLK_ESDHC_B_PRED		12
#define IMX5_CLK_ESDHC_C_SEL		13
#define IMX5_CLK_ESDHC_D_SEL		14
#define IMX5_CLK_EMI_SEL		15
#define IMX5_CLK_EMI_SLOW_PODF		16
#define IMX5_CLK_NFC_PODF		17
#define IMX5_CLK_ECSPI_PRED		18
#define IMX5_CLK_ECSPI_PODF		19
#define IMX5_CLK_USBOH3_PRED		20
#define IMX5_CLK_USBOH3_PODF		21
#define IMX5_CLK_USB_PHY_PRED		22
#define IMX5_CLK_USB_PHY_PODF		23
#define IMX5_CLK_CPU_PODF		24
#define IMX5_CLK_DI_PRED		25
#define IMX5_CLK_TVE_SEL		27
#define IMX5_CLK_UART1_IPG_GATE		28
#define IMX5_CLK_UART1_PER_GATE		29
#define IMX5_CLK_UART2_IPG_GATE		30
#define IMX5_CLK_UART2_PER_GATE		31
#define IMX5_CLK_UART3_IPG_GATE		32
#define IMX5_CLK_UART3_PER_GATE		33
#define IMX5_CLK_I2C1_GATE		34
#define IMX5_CLK_I2C2_GATE		35
#define IMX5_CLK_GPT_IPG_GATE		36
#define IMX5_CLK_PWM1_IPG_GATE		37
#define IMX5_CLK_PWM1_HF_GATE		38
#define IMX5_CLK_PWM2_IPG_GATE		39
#define IMX5_CLK_PWM2_HF_GATE		40
#define IMX5_CLK_GPT_HF_GATE		41
#define IMX5_CLK_FEC_GATE		42
#define IMX5_CLK_USBOH3_PER_GATE	43
#define IMX5_CLK_ESDHC1_IPG_GATE	44
#define IMX5_CLK_ESDHC2_IPG_GATE	45
#define IMX5_CLK_ESDHC3_IPG_GATE	46
#define IMX5_CLK_ESDHC4_IPG_GATE	47
#define IMX5_CLK_SSI1_IPG_GATE		48
#define IMX5_CLK_SSI2_IPG_GATE		49
#define IMX5_CLK_SSI3_IPG_GATE		50
#define IMX5_CLK_ECSPI1_IPG_GATE	51
#define IMX5_CLK_ECSPI1_PER_GATE	52
#define IMX5_CLK_ECSPI2_IPG_GATE	53
#define IMX5_CLK_ECSPI2_PER_GATE	54
#define IMX5_CLK_CSPI_IPG_GATE		55
#define IMX5_CLK_SDMA_GATE		56
#define IMX5_CLK_EMI_SLOW_GATE		57
#define IMX5_CLK_IPU_SEL		58
#define IMX5_CLK_IPU_GATE		59
#define IMX5_CLK_NFC_GATE		60
#define IMX5_CLK_IPU_DI1_GATE		61
#define IMX5_CLK_VPU_SEL		62
#define IMX5_CLK_VPU_GATE		63
#define IMX5_CLK_VPU_REFERENCE_GATE	64
#define IMX5_CLK_UART4_IPG_GATE		65
#define IMX5_CLK_UART4_PER_GATE		66
#define IMX5_CLK_UART5_IPG_GATE		67
#define IMX5_CLK_UART5_PER_GATE		68
#define IMX5_CLK_TVE_GATE		69
#define IMX5_CLK_TVE_PRED		70
#define IMX5_CLK_ESDHC1_PER_GATE	71
#define IMX5_CLK_ESDHC2_PER_GATE	72
#define IMX5_CLK_ESDHC3_PER_GATE	73
#define IMX5_CLK_ESDHC4_PER_GATE	74
#define IMX5_CLK_USB_PHY_GATE		75
#define IMX5_CLK_HSI2C_GATE		76
#define IMX5_CLK_MIPI_HSC1_GATE		77
#define IMX5_CLK_MIPI_HSC2_GATE		78
#define IMX5_CLK_MIPI_ESC_GATE		79
#define IMX5_CLK_MIPI_HSP_GATE		80
#define IMX5_CLK_LDB_DI1_DIV_3_5	81
#define IMX5_CLK_LDB_DI1_DIV		82
#define IMX5_CLK_LDB_DI0_DIV_3_5	83
#define IMX5_CLK_LDB_DI0_DIV		84
#define IMX5_CLK_LDB_DI1_GATE		85
#define IMX5_CLK_CAN2_SERIAL_GATE	86
#define IMX5_CLK_CAN2_IPG_GATE		87
#define IMX5_CLK_I2C3_GATE		88
#define IMX5_CLK_LP_APM			89
#define IMX5_CLK_PERIPH_APM		90
#define IMX5_CLK_MAIN_BUS		91
#define IMX5_CLK_AHB_MAX		92
#define IMX5_CLK_AIPS_TZ1		93
#define IMX5_CLK_AIPS_TZ2		94
#define IMX5_CLK_TMAX1			95
#define IMX5_CLK_TMAX2			96
#define IMX5_CLK_TMAX3			97
#define IMX5_CLK_SPBA			98
#define IMX5_CLK_UART_SEL		99
#define IMX5_CLK_ESDHC_A_SEL		100
#define IMX5_CLK_ESDHC_B_SEL		101
#define IMX5_CLK_ESDHC_A_PODF		102
#define IMX5_CLK_ESDHC_B_PODF		103
#define IMX5_CLK_ECSPI_SEL		104
#define IMX5_CLK_USBOH3_SEL		105
#define IMX5_CLK_USB_PHY_SEL		106
#define IMX5_CLK_IIM_GATE		107
#define IMX5_CLK_USBOH3_GATE		108
#define IMX5_CLK_EMI_FAST_GATE		109
#define IMX5_CLK_IPU_DI0_GATE		110
#define IMX5_CLK_GPC_DVFS		111
#define IMX5_CLK_PLL1_SW		112
#define IMX5_CLK_PLL2_SW		113
#define IMX5_CLK_PLL3_SW		114
#define IMX5_CLK_IPU_DI0_SEL		115
#define IMX5_CLK_IPU_DI1_SEL		116
#define IMX5_CLK_TVE_EXT_SEL		117
#define IMX5_CLK_MX51_MIPI		118
#define IMX5_CLK_PLL4_SW		119
#define IMX5_CLK_LDB_DI1_SEL		120
#define IMX5_CLK_DI_PLL4_PODF		121
#define IMX5_CLK_LDB_DI0_SEL		122
#define IMX5_CLK_LDB_DI0_GATE		123
#define IMX5_CLK_USB_PHY1_GATE		124
#define IMX5_CLK_USB_PHY2_GATE		125
#define IMX5_CLK_PER_LP_APM		126
#define IMX5_CLK_PER_PRED1		127
#define IMX5_CLK_PER_PRED2		128
#define IMX5_CLK_PER_PODF		129
#define IMX5_CLK_PER_ROOT		130
#define IMX5_CLK_SSI_APM		131
#define IMX5_CLK_SSI1_ROOT_SEL		132
#define IMX5_CLK_SSI2_ROOT_SEL		133
#define IMX5_CLK_SSI3_ROOT_SEL		134
#define IMX5_CLK_SSI_EXT1_SEL		135
#define IMX5_CLK_SSI_EXT2_SEL		136
#define IMX5_CLK_SSI_EXT1_COM_SEL	137
#define IMX5_CLK_SSI_EXT2_COM_SEL	138
#define IMX5_CLK_SSI1_ROOT_PRED		139
#define IMX5_CLK_SSI1_ROOT_PODF		140
#define IMX5_CLK_SSI2_ROOT_PRED		141
#define IMX5_CLK_SSI2_ROOT_PODF		142
#define IMX5_CLK_SSI_EXT1_PRED		143
#define IMX5_CLK_SSI_EXT1_PODF		144
#define IMX5_CLK_SSI_EXT2_PRED		145
#define IMX5_CLK_SSI_EXT2_PODF		146
#define IMX5_CLK_SSI1_ROOT_GATE		147
#define IMX5_CLK_SSI2_ROOT_GATE		148
#define IMX5_CLK_SSI3_ROOT_GATE		149
#define IMX5_CLK_SSI_EXT1_GATE		150
#define IMX5_CLK_SSI_EXT2_GATE		151
#define IMX5_CLK_EPIT1_IPG_GATE		152
#define IMX5_CLK_EPIT1_HF_GATE		153
#define IMX5_CLK_EPIT2_IPG_GATE		154
#define IMX5_CLK_EPIT2_HF_GATE		155
#define IMX5_CLK_CAN_SEL		156
#define IMX5_CLK_CAN1_SERIAL_GATE	157
#define IMX5_CLK_CAN1_IPG_GATE		158
#define IMX5_CLK_OWIRE_GATE		159
#define IMX5_CLK_GPU3D_SEL		160
#define IMX5_CLK_GPU2D_SEL		161
#define IMX5_CLK_GPU3D_GATE		162
#define IMX5_CLK_GPU2D_GATE		163
#define IMX5_CLK_GARB_GATE		164
#define IMX5_CLK_CKO1_SEL		165
#define IMX5_CLK_CKO1_PODF		166
#define IMX5_CLK_CKO1			167
#define IMX5_CLK_CKO2_SEL		168
#define IMX5_CLK_CKO2_PODF		169
#define IMX5_CLK_CKO2			170
#define IMX5_CLK_SRTC_GATE		171
#define IMX5_CLK_PATA_GATE		172
#define IMX5_CLK_SATA_GATE		173
#define IMX5_CLK_SPDIF_XTAL_SEL		174
#define IMX5_CLK_SPDIF0_SEL		175
#define IMX5_CLK_SPDIF1_SEL		176
#define IMX5_CLK_SPDIF0_PRED		177
#define IMX5_CLK_SPDIF0_PODF		178
#define IMX5_CLK_SPDIF1_PRED		179
#define IMX5_CLK_SPDIF1_PODF		180
#define IMX5_CLK_SPDIF0_COM_SEL		181
#define IMX5_CLK_SPDIF1_COM_SEL		182
#define IMX5_CLK_SPDIF0_GATE		183
#define IMX5_CLK_SPDIF1_GATE		184
#define IMX5_CLK_SPDIF_IPG_GATE		185
#define IMX5_CLK_OCRAM			186
#define IMX5_CLK_SAHARA_IPG_GATE	187
#define IMX5_CLK_SATA_REF		188
#define IMX5_CLK_STEP_SEL		189
#define IMX5_CLK_CPU_PODF_SEL		190
#define IMX5_CLK_ARM			191
#define IMX5_CLK_FIRI_PRED		192
#define IMX5_CLK_FIRI_SEL		193
#define IMX5_CLK_FIRI_PODF		194
#define IMX5_CLK_FIRI_SERIAL_GATE	195
#define IMX5_CLK_FIRI_IPG_GATE		196
#define IMX5_CLK_CSI0_MCLK1_PRED	197
#define IMX5_CLK_CSI0_MCLK1_SEL		198
#define IMX5_CLK_CSI0_MCLK1_PODF	199
#define IMX5_CLK_CSI0_MCLK1_GATE	200
#define IMX5_CLK_IEEE1588_PRED		201
#define IMX5_CLK_IEEE1588_SEL		202
#define IMX5_CLK_IEEE1588_PODF		203
#define IMX5_CLK_IEEE1588_GATE		204
#define IMX5_CLK_END			205

#endif 
