/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __MACH_MX31_H__
#define __MACH_MX31_H__

<<<<<<< HEAD
/*
 * IRAM
 */
#define MX31_IRAM_BASE_ADDR		0x1ffc0000	/* internal ram */
#define MX31_IRAM_SIZE			SZ_16K

#define MX31_L2CC_BASE_ADDR		0x30000000
#define MX31_L2CC_SIZE			SZ_1M

#define MX31_AIPS1_BASE_ADDR		0x43f00000
#define MX31_AIPS1_SIZE			SZ_1M
#define MX31_MAX_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0x04000)
#define MX31_EVTMON_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0x08000)
#define MX31_CLKCTL_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0x0c000)
#define MX31_ETB_SLOT4_BASE_ADDR		(MX31_AIPS1_BASE_ADDR + 0x10000)
#define MX31_ETB_SLOT5_BASE_ADDR		(MX31_AIPS1_BASE_ADDR + 0x14000)
#define MX31_ECT_CTIO_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0x18000)
#define MX31_I2C1_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0x80000)
#define MX31_I2C3_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0x84000)
#define MX31_USB_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0x88000)
#define MX31_USB_OTG_BASE_ADDR			(MX31_USB_BASE_ADDR + 0x0000)
#define MX31_USB_HS1_BASE_ADDR			(MX31_USB_BASE_ADDR + 0x0200)
#define MX31_USB_HS2_BASE_ADDR			(MX31_USB_BASE_ADDR + 0x0400)
#define MX31_ATA_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0x8c000)
#define MX31_UART1_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0x90000)
#define MX31_UART2_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0x94000)
#define MX31_I2C2_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0x98000)
#define MX31_OWIRE_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0x9c000)
#define MX31_SSI1_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0xa0000)
#define MX31_CSPI1_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0xa4000)
#define MX31_KPP_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0xa8000)
#define MX31_IOMUXC_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0xac000)
#define MX31_UART4_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0xb0000)
#define MX31_UART5_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0xb4000)
#define MX31_ECT_IP1_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0xb8000)
#define MX31_ECT_IP2_BASE_ADDR			(MX31_AIPS1_BASE_ADDR + 0xbc000)

#define MX31_SPBA0_BASE_ADDR		0x50000000
#define MX31_SPBA0_SIZE			SZ_1M
#define MX31_SDHC1_BASE_ADDR			(MX31_SPBA0_BASE_ADDR + 0x04000)
#define MX31_SDHC2_BASE_ADDR			(MX31_SPBA0_BASE_ADDR + 0x08000)
#define MX31_UART3_BASE_ADDR			(MX31_SPBA0_BASE_ADDR + 0x0c000)
#define MX31_CSPI2_BASE_ADDR			(MX31_SPBA0_BASE_ADDR + 0x10000)
#define MX31_SSI2_BASE_ADDR			(MX31_SPBA0_BASE_ADDR + 0x14000)
#define MX31_SIM1_BASE_ADDR			(MX31_SPBA0_BASE_ADDR + 0x18000)
#define MX31_IIM_BASE_ADDR			(MX31_SPBA0_BASE_ADDR + 0x1c000)
#define MX31_ATA_DMA_BASE_ADDR			(MX31_SPBA0_BASE_ADDR + 0x20000)
#define MX31_MSHC1_BASE_ADDR			(MX31_SPBA0_BASE_ADDR + 0x24000)
#define MX31_SPBA_CTRL_BASE_ADDR		(MX31_SPBA0_BASE_ADDR + 0x3c000)

#define MX31_AIPS2_BASE_ADDR		0x53f00000
#define MX31_AIPS2_SIZE			SZ_1M
#define MX31_CCM_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0x80000)
#define MX31_CSPI3_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0x84000)
#define MX31_FIRI_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0x8c000)
#define MX31_GPT1_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0x90000)
#define MX31_EPIT1_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0x94000)
#define MX31_EPIT2_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0x98000)
#define MX31_GPIO3_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xa4000)
#define MX31_SCC_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xac000)
#define MX31_SCM_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xae000)
#define MX31_SMN_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xaf000)
#define MX31_RNGA_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xb0000)
#define MX31_IPU_CTRL_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xc0000)
#define MX31_AUDMUX_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xc4000)
#define MX31_MPEG4_ENC_BASE_ADDR		(MX31_AIPS2_BASE_ADDR + 0xc8000)
#define MX31_GPIO1_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xcc000)
#define MX31_GPIO2_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xd0000)
#define MX31_SDMA_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xd4000)
#define MX31_RTC_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xd8000)
#define MX31_WDOG_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xdc000)
#define MX31_PWM_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xe0000)
#define MX31_RTIC_BASE_ADDR			(MX31_AIPS2_BASE_ADDR + 0xec000)

#define MX31_ROMP_BASE_ADDR		0x60000000
#define MX31_ROMP_BASE_ADDR_VIRT	IOMEM(0xfc500000)
#define MX31_ROMP_SIZE			SZ_1M

#define MX31_AVIC_BASE_ADDR		0x68000000
#define MX31_AVIC_SIZE			SZ_1M

#define MX31_IPU_MEM_BASE_ADDR		0x70000000
#define MX31_CSD0_BASE_ADDR		0x80000000
#define MX31_CSD1_BASE_ADDR		0x90000000

#define MX31_CS0_BASE_ADDR		0xa0000000
#define MX31_CS1_BASE_ADDR		0xa8000000
#define MX31_CS2_BASE_ADDR		0xb0000000
#define MX31_CS3_BASE_ADDR		0xb2000000

#define MX31_CS4_BASE_ADDR		0xb4000000
#define MX31_CS4_BASE_ADDR_VIRT		IOMEM(0xf6000000)
#define MX31_CS4_SIZE			SZ_32M

#define MX31_CS5_BASE_ADDR		0xb6000000
#define MX31_CS5_BASE_ADDR_VIRT		IOMEM(0xf8000000)
#define MX31_CS5_SIZE			SZ_32M

#define MX31_X_MEMC_BASE_ADDR		0xb8000000
#define MX31_X_MEMC_SIZE		SZ_64K
#define MX31_NFC_BASE_ADDR			(MX31_X_MEMC_BASE_ADDR + 0x0000)
#define MX31_ESDCTL_BASE_ADDR			(MX31_X_MEMC_BASE_ADDR + 0x1000)
#define MX31_WEIM_BASE_ADDR			(MX31_X_MEMC_BASE_ADDR + 0x2000)
#define MX31_M3IF_BASE_ADDR			(MX31_X_MEMC_BASE_ADDR + 0x3000)
#define MX31_EMI_CTL_BASE_ADDR			(MX31_X_MEMC_BASE_ADDR + 0x4000)
#define MX31_PCMCIA_CTL_BASE_ADDR		MX31_EMI_CTL_BASE_ADDR

#define MX31_WEIM_CSCRx_BASE_ADDR(cs)	(MX31_WEIM_BASE_ADDR + (cs) * 0x10)
#define MX31_WEIM_CSCRxU(cs)			(MX31_WEIM_CSCRx_BASE_ADDR(cs))
#define MX31_WEIM_CSCRxL(cs)			(MX31_WEIM_CSCRx_BASE_ADDR(cs) + 0x4)
#define MX31_WEIM_CSCRxA(cs)			(MX31_WEIM_CSCRx_BASE_ADDR(cs) + 0x8)

#define MX31_PCMCIA_MEM_BASE_ADDR	0xbc000000

#define MX31_IO_P2V(x)			IMX_IO_P2V(x)
#define MX31_IO_ADDRESS(x)		IOMEM(MX31_IO_P2V(x))

/*
 * Interrupt numbers
 */
#include <asm/irq.h>
#define MX31_INT_I2C3		(NR_IRQS_LEGACY + 3)
#define MX31_INT_I2C2		(NR_IRQS_LEGACY + 4)
#define MX31_INT_MPEG4_ENCODER	(NR_IRQS_LEGACY + 5)
#define MX31_INT_RTIC		(NR_IRQS_LEGACY + 6)
#define MX31_INT_FIRI		(NR_IRQS_LEGACY + 7)
#define MX31_INT_SDHC2		(NR_IRQS_LEGACY + 8)
#define MX31_INT_SDHC1		(NR_IRQS_LEGACY + 9)
#define MX31_INT_I2C1		(NR_IRQS_LEGACY + 10)
#define MX31_INT_SSI2		(NR_IRQS_LEGACY + 11)
#define MX31_INT_SSI1		(NR_IRQS_LEGACY + 12)
#define MX31_INT_CSPI2		(NR_IRQS_LEGACY + 13)
#define MX31_INT_CSPI1		(NR_IRQS_LEGACY + 14)
#define MX31_INT_ATA		(NR_IRQS_LEGACY + 15)
#define MX31_INT_MBX		(NR_IRQS_LEGACY + 16)
#define MX31_INT_CSPI3		(NR_IRQS_LEGACY + 17)
#define MX31_INT_UART3		(NR_IRQS_LEGACY + 18)
#define MX31_INT_IIM		(NR_IRQS_LEGACY + 19)
#define MX31_INT_SIM2		(NR_IRQS_LEGACY + 20)
#define MX31_INT_SIM1		(NR_IRQS_LEGACY + 21)
#define MX31_INT_RNGA		(NR_IRQS_LEGACY + 22)
#define MX31_INT_EVTMON		(NR_IRQS_LEGACY + 23)
#define MX31_INT_KPP		(NR_IRQS_LEGACY + 24)
#define MX31_INT_RTC		(NR_IRQS_LEGACY + 25)
#define MX31_INT_PWM		(NR_IRQS_LEGACY + 26)
#define MX31_INT_EPIT2		(NR_IRQS_LEGACY + 27)
#define MX31_INT_EPIT1		(NR_IRQS_LEGACY + 28)
#define MX31_INT_GPT		(NR_IRQS_LEGACY + 29)
#define MX31_INT_POWER_FAIL	(NR_IRQS_LEGACY + 30)
#define MX31_INT_CCM_DVFS	(NR_IRQS_LEGACY + 31)
#define MX31_INT_UART2		(NR_IRQS_LEGACY + 32)
#define MX31_INT_NFC		(NR_IRQS_LEGACY + 33)
#define MX31_INT_SDMA		(NR_IRQS_LEGACY + 34)
#define MX31_INT_USB_HS1	(NR_IRQS_LEGACY + 35)
#define MX31_INT_USB_HS2	(NR_IRQS_LEGACY + 36)
#define MX31_INT_USB_OTG	(NR_IRQS_LEGACY + 37)
#define MX31_INT_MSHC1		(NR_IRQS_LEGACY + 39)
#define MX31_INT_MSHC2		(NR_IRQS_LEGACY + 40)
#define MX31_INT_IPU_ERR	(NR_IRQS_LEGACY + 41)
#define MX31_INT_IPU_SYN	(NR_IRQS_LEGACY + 42)
#define MX31_INT_UART1		(NR_IRQS_LEGACY + 45)
#define MX31_INT_UART4		(NR_IRQS_LEGACY + 46)
#define MX31_INT_UART5		(NR_IRQS_LEGACY + 47)
#define MX31_INT_ECT		(NR_IRQS_LEGACY + 48)
#define MX31_INT_SCC_SCM	(NR_IRQS_LEGACY + 49)
#define MX31_INT_SCC_SMN	(NR_IRQS_LEGACY + 50)
#define MX31_INT_GPIO2		(NR_IRQS_LEGACY + 51)
#define MX31_INT_GPIO1		(NR_IRQS_LEGACY + 52)
#define MX31_INT_CCM		(NR_IRQS_LEGACY + 53)
#define MX31_INT_PCMCIA		(NR_IRQS_LEGACY + 54)
#define MX31_INT_WDOG		(NR_IRQS_LEGACY + 55)
#define MX31_INT_GPIO3		(NR_IRQS_LEGACY + 56)
#define MX31_INT_EXT_POWER	(NR_IRQS_LEGACY + 58)
#define MX31_INT_EXT_TEMPER	(NR_IRQS_LEGACY + 59)
#define MX31_INT_EXT_SENSOR60	(NR_IRQS_LEGACY + 60)
#define MX31_INT_EXT_SENSOR61	(NR_IRQS_LEGACY + 61)
#define MX31_INT_EXT_WDOG	(NR_IRQS_LEGACY + 62)
#define MX31_INT_EXT_TV		(NR_IRQS_LEGACY + 63)

#define MX31_DMA_REQ_SDHC1	20
#define MX31_DMA_REQ_SDHC2	21
#define MX31_DMA_REQ_SSI2_RX1	22
#define MX31_DMA_REQ_SSI2_TX1	23
#define MX31_DMA_REQ_SSI2_RX0	24
#define MX31_DMA_REQ_SSI2_TX0	25
#define MX31_DMA_REQ_SSI1_RX1	26
#define MX31_DMA_REQ_SSI1_TX1	27
#define MX31_DMA_REQ_SSI1_RX0	28
#define MX31_DMA_REQ_SSI1_TX0	29

#define MX31_PROD_SIGNATURE		0x1	/* For MX31 */
=======
#define MX31_AIPS1_BASE_ADDR		0x43f00000
#define MX31_AIPS1_SIZE			SZ_1M
#define MX31_SPBA0_BASE_ADDR		0x50000000
#define MX31_SPBA0_SIZE			SZ_1M
#define MX31_AIPS2_BASE_ADDR		0x53f00000
#define MX31_AIPS2_SIZE			SZ_1M
#define MX31_AVIC_BASE_ADDR		0x68000000
#define MX31_AVIC_SIZE			SZ_1M
#define MX31_X_MEMC_BASE_ADDR		0xb8000000
#define MX31_X_MEMC_SIZE		SZ_64K

#define MX31_IO_P2V(x)			IMX_IO_P2V(x)
>>>>>>> upstream/android-13

#endif /* ifndef __MACH_MX31_H__ */
