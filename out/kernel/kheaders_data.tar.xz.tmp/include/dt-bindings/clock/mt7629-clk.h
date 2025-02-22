/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _DT_BINDINGS_CLK_MT7629_H
#define _DT_BINDINGS_CLK_MT7629_H


#define CLK_TOP_TO_U2_PHY		0
#define CLK_TOP_TO_U2_PHY_1P		1
#define CLK_TOP_PCIE0_PIPE_EN		2
#define CLK_TOP_PCIE1_PIPE_EN		3
#define CLK_TOP_SSUSB_TX250M		4
#define CLK_TOP_SSUSB_EQ_RX250M		5
#define CLK_TOP_SSUSB_CDR_REF		6
#define CLK_TOP_SSUSB_CDR_FB		7
#define CLK_TOP_SATA_ASIC		8
#define CLK_TOP_SATA_RBC		9
#define CLK_TOP_TO_USB3_SYS		10
#define CLK_TOP_P1_1MHZ			11
#define CLK_TOP_4MHZ			12
#define CLK_TOP_P0_1MHZ			13
#define CLK_TOP_ETH_500M		14
#define CLK_TOP_TXCLK_SRC_PRE		15
#define CLK_TOP_RTC			16
#define CLK_TOP_PWM_QTR_26M		17
#define CLK_TOP_CPUM_TCK_IN		18
#define CLK_TOP_TO_USB3_DA_TOP		19
#define CLK_TOP_MEMPLL			20
#define CLK_TOP_DMPLL			21
#define CLK_TOP_DMPLL_D4		22
#define CLK_TOP_DMPLL_D8		23
#define CLK_TOP_SYSPLL_D2		24
#define CLK_TOP_SYSPLL1_D2		25
#define CLK_TOP_SYSPLL1_D4		26
#define CLK_TOP_SYSPLL1_D8		27
#define CLK_TOP_SYSPLL1_D16		28
#define CLK_TOP_SYSPLL2_D2		29
#define CLK_TOP_SYSPLL2_D4		30
#define CLK_TOP_SYSPLL2_D8		31
#define CLK_TOP_SYSPLL_D5		32
#define CLK_TOP_SYSPLL3_D2		33
#define CLK_TOP_SYSPLL3_D4		34
#define CLK_TOP_SYSPLL_D7		35
#define CLK_TOP_SYSPLL4_D2		36
#define CLK_TOP_SYSPLL4_D4		37
#define CLK_TOP_SYSPLL4_D16		38
#define CLK_TOP_UNIVPLL			39
#define CLK_TOP_UNIVPLL1_D2		40
#define CLK_TOP_UNIVPLL1_D4		41
#define CLK_TOP_UNIVPLL1_D8		42
#define CLK_TOP_UNIVPLL_D3		43
#define CLK_TOP_UNIVPLL2_D2		44
#define CLK_TOP_UNIVPLL2_D4		45
#define CLK_TOP_UNIVPLL2_D8		46
#define CLK_TOP_UNIVPLL2_D16		47
#define CLK_TOP_UNIVPLL_D5		48
#define CLK_TOP_UNIVPLL3_D2		49
#define CLK_TOP_UNIVPLL3_D4		50
#define CLK_TOP_UNIVPLL3_D16		51
#define CLK_TOP_UNIVPLL_D7		52
#define CLK_TOP_UNIVPLL_D80_D4		53
#define CLK_TOP_UNIV48M			54
#define CLK_TOP_SGMIIPLL_D2		55
#define CLK_TOP_CLKXTAL_D4		56
#define CLK_TOP_HD_FAXI			57
#define CLK_TOP_FAXI			58
#define CLK_TOP_F_FAUD_INTBUS		59
#define CLK_TOP_AP2WBHIF_HCLK		60
#define CLK_TOP_10M_INFRAO		61
#define CLK_TOP_MSDC30_1		62
#define CLK_TOP_SPI			63
#define CLK_TOP_SF			64
#define CLK_TOP_FLASH			65
#define CLK_TOP_TO_USB3_REF		66
#define CLK_TOP_TO_USB3_MCU		67
#define CLK_TOP_TO_USB3_DMA		68
#define CLK_TOP_FROM_TOP_AHB		69
#define CLK_TOP_FROM_TOP_AXI		70
#define CLK_TOP_PCIE1_MAC_EN		71
#define CLK_TOP_PCIE0_MAC_EN		72
#define CLK_TOP_AXI_SEL			73
#define CLK_TOP_MEM_SEL			74
#define CLK_TOP_DDRPHYCFG_SEL		75
#define CLK_TOP_ETH_SEL			76
#define CLK_TOP_PWM_SEL			77
#define CLK_TOP_F10M_REF_SEL		78
#define CLK_TOP_NFI_INFRA_SEL		79
#define CLK_TOP_FLASH_SEL		80
#define CLK_TOP_UART_SEL		81
#define CLK_TOP_SPI0_SEL		82
#define CLK_TOP_SPI1_SEL		83
#define CLK_TOP_MSDC50_0_SEL		84
#define CLK_TOP_MSDC30_0_SEL		85
#define CLK_TOP_MSDC30_1_SEL		86
#define CLK_TOP_AP2WBMCU_SEL		87
#define CLK_TOP_AP2WBHIF_SEL		88
#define CLK_TOP_AUDIO_SEL		89
#define CLK_TOP_AUD_INTBUS_SEL		90
#define CLK_TOP_PMICSPI_SEL		91
#define CLK_TOP_SCP_SEL			92
#define CLK_TOP_ATB_SEL			93
#define CLK_TOP_HIF_SEL			94
#define CLK_TOP_SATA_SEL		95
#define CLK_TOP_U2_SEL			96
#define CLK_TOP_AUD1_SEL		97
#define CLK_TOP_AUD2_SEL		98
#define CLK_TOP_IRRX_SEL		99
#define CLK_TOP_IRTX_SEL		100
#define CLK_TOP_SATA_MCU_SEL		101
#define CLK_TOP_PCIE0_MCU_SEL		102
#define CLK_TOP_PCIE1_MCU_SEL		103
#define CLK_TOP_SSUSB_MCU_SEL		104
#define CLK_TOP_CRYPTO_SEL		105
#define CLK_TOP_SGMII_REF_1_SEL		106
#define CLK_TOP_10M_SEL			107
#define CLK_TOP_NR_CLK			108


#define CLK_INFRA_MUX1_SEL		0
#define CLK_INFRA_DBGCLK_PD		1
#define CLK_INFRA_TRNG_PD		2
#define CLK_INFRA_DEVAPC_PD		3
#define CLK_INFRA_APXGPT_PD		4
#define CLK_INFRA_SEJ_PD		5
#define CLK_INFRA_NR_CLK		6


#define CLK_PERIBUS_SEL			0
#define CLK_PERI_PWM1_PD		1
#define CLK_PERI_PWM2_PD		2
#define CLK_PERI_PWM3_PD		3
#define CLK_PERI_PWM4_PD		4
#define CLK_PERI_PWM5_PD		5
#define CLK_PERI_PWM6_PD		6
#define CLK_PERI_PWM7_PD		7
#define CLK_PERI_PWM_PD			8
#define CLK_PERI_AP_DMA_PD		9
#define CLK_PERI_MSDC30_1_PD		10
#define CLK_PERI_UART0_PD		11
#define CLK_PERI_UART1_PD		12
#define CLK_PERI_UART2_PD		13
#define CLK_PERI_UART3_PD		14
#define CLK_PERI_BTIF_PD		15
#define CLK_PERI_I2C0_PD		16
#define CLK_PERI_SPI0_PD		17
#define CLK_PERI_SNFI_PD		18
#define CLK_PERI_NFI_PD			19
#define CLK_PERI_NFIECC_PD		20
#define CLK_PERI_FLASH_PD		21
#define CLK_PERI_NR_CLK			22


#define CLK_APMIXED_ARMPLL		0
#define CLK_APMIXED_MAINPLL		1
#define CLK_APMIXED_UNIV2PLL		2
#define CLK_APMIXED_ETH1PLL		3
#define CLK_APMIXED_ETH2PLL		4
#define CLK_APMIXED_SGMIPLL		5
#define CLK_APMIXED_MAIN_CORE_EN	6
#define CLK_APMIXED_NR_CLK		7


#define CLK_SSUSB_U2_PHY_1P_EN		0
#define CLK_SSUSB_U2_PHY_EN		1
#define CLK_SSUSB_REF_EN		2
#define CLK_SSUSB_SYS_EN		3
#define CLK_SSUSB_MCU_EN		4
#define CLK_SSUSB_DMA_EN		5
#define CLK_SSUSB_NR_CLK		6


#define CLK_PCIE_P1_AUX_EN		0
#define CLK_PCIE_P1_OBFF_EN		1
#define CLK_PCIE_P1_AHB_EN		2
#define CLK_PCIE_P1_AXI_EN		3
#define CLK_PCIE_P1_MAC_EN		4
#define CLK_PCIE_P1_PIPE_EN		5
#define CLK_PCIE_P0_AUX_EN		6
#define CLK_PCIE_P0_OBFF_EN		7
#define CLK_PCIE_P0_AHB_EN		8
#define CLK_PCIE_P0_AXI_EN		9
#define CLK_PCIE_P0_MAC_EN		10
#define CLK_PCIE_P0_PIPE_EN		11
#define CLK_PCIE_NR_CLK			12


#define CLK_ETH_FE_EN			0
#define CLK_ETH_GP2_EN			1
#define CLK_ETH_GP1_EN			2
#define CLK_ETH_GP0_EN			3
#define CLK_ETH_ESW_EN			4
#define CLK_ETH_NR_CLK			5


#define CLK_SGMII_TX_EN			0
#define CLK_SGMII_RX_EN			1
#define CLK_SGMII_CDR_REF		2
#define CLK_SGMII_CDR_FB		3
#define CLK_SGMII_NR_CLK		4

#endif 
