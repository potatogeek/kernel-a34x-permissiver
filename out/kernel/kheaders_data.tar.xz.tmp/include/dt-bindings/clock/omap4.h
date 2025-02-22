
#ifndef __DT_BINDINGS_CLK_OMAP4_H
#define __DT_BINDINGS_CLK_OMAP4_H

#define OMAP4_CLKCTRL_OFFSET	0x20
#define OMAP4_CLKCTRL_INDEX(offset)	((offset) - OMAP4_CLKCTRL_OFFSET)


#define OMAP4_MPU_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)


#define OMAP4_DSP_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)


#define OMAP4_L4_ABE_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)
#define OMAP4_AESS_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x28)
#define OMAP4_MCPDM_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x30)
#define OMAP4_DMIC_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x38)
#define OMAP4_MCASP_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x40)
#define OMAP4_MCBSP1_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x48)
#define OMAP4_MCBSP2_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x50)
#define OMAP4_MCBSP3_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x58)
#define OMAP4_SLIMBUS1_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x60)
#define OMAP4_TIMER5_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x68)
#define OMAP4_TIMER6_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x70)
#define OMAP4_TIMER7_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x78)
#define OMAP4_TIMER8_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x80)
#define OMAP4_WD_TIMER3_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x88)


#define OMAP4_SMARTREFLEX_MPU_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x28)
#define OMAP4_SMARTREFLEX_IVA_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x30)
#define OMAP4_SMARTREFLEX_CORE_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x38)


#define OMAP4_L3_MAIN_1_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)


#define OMAP4_L3_MAIN_2_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)
#define OMAP4_GPMC_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x28)
#define OMAP4_OCMC_RAM_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x30)


#define OMAP4_IPU_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)


#define OMAP4_DMA_SYSTEM_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)


#define OMAP4_DMM_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)
#define OMAP4_EMIF1_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x30)
#define OMAP4_EMIF2_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x38)


#define OMAP4_C2C_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)


#define OMAP4_L4_CFG_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)
#define OMAP4_SPINLOCK_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x28)
#define OMAP4_MAILBOX_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x30)


#define OMAP4_L3_MAIN_3_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)
#define OMAP4_L3_INSTR_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x28)
#define OMAP4_OCP_WP_NOC_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x40)


#define OMAP4_IVA_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)
#define OMAP4_SL2IF_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x28)


#define OMAP4_ISS_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)
#define OMAP4_FDIF_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x28)


#define OMAP4_DSS_CORE_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)


#define OMAP4_GPU_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)


#define OMAP4_MMC1_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x28)
#define OMAP4_MMC2_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x30)
#define OMAP4_HSI_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x38)
#define OMAP4_USB_HOST_HS_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x58)
#define OMAP4_USB_OTG_HS_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x60)
#define OMAP4_USB_TLL_HS_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x68)
#define OMAP4_USB_HOST_FS_CLKCTRL	OMAP4_CLKCTRL_INDEX(0xd0)
#define OMAP4_OCP2SCP_USB_PHY_CLKCTRL	OMAP4_CLKCTRL_INDEX(0xe0)


#define OMAP4_TIMER10_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x28)
#define OMAP4_TIMER11_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x30)
#define OMAP4_TIMER2_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x38)
#define OMAP4_TIMER3_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x40)
#define OMAP4_TIMER4_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x48)
#define OMAP4_TIMER9_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x50)
#define OMAP4_ELM_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x58)
#define OMAP4_GPIO2_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x60)
#define OMAP4_GPIO3_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x68)
#define OMAP4_GPIO4_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x70)
#define OMAP4_GPIO5_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x78)
#define OMAP4_GPIO6_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x80)
#define OMAP4_HDQ1W_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x88)
#define OMAP4_I2C1_CLKCTRL	OMAP4_CLKCTRL_INDEX(0xa0)
#define OMAP4_I2C2_CLKCTRL	OMAP4_CLKCTRL_INDEX(0xa8)
#define OMAP4_I2C3_CLKCTRL	OMAP4_CLKCTRL_INDEX(0xb0)
#define OMAP4_I2C4_CLKCTRL	OMAP4_CLKCTRL_INDEX(0xb8)
#define OMAP4_L4_PER_CLKCTRL	OMAP4_CLKCTRL_INDEX(0xc0)
#define OMAP4_MCBSP4_CLKCTRL	OMAP4_CLKCTRL_INDEX(0xe0)
#define OMAP4_MCSPI1_CLKCTRL	OMAP4_CLKCTRL_INDEX(0xf0)
#define OMAP4_MCSPI2_CLKCTRL	OMAP4_CLKCTRL_INDEX(0xf8)
#define OMAP4_MCSPI3_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x100)
#define OMAP4_MCSPI4_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x108)
#define OMAP4_MMC3_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x120)
#define OMAP4_MMC4_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x128)
#define OMAP4_SLIMBUS2_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x138)
#define OMAP4_UART1_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x140)
#define OMAP4_UART2_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x148)
#define OMAP4_UART3_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x150)
#define OMAP4_UART4_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x158)
#define OMAP4_MMC5_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x160)


#define OMAP4_L4_WKUP_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)
#define OMAP4_WD_TIMER2_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x30)
#define OMAP4_GPIO1_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x38)
#define OMAP4_TIMER1_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x40)
#define OMAP4_COUNTER_32K_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x50)
#define OMAP4_KBD_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x78)


#define OMAP4_DEBUGSS_CLKCTRL	OMAP4_CLKCTRL_INDEX(0x20)

#endif
