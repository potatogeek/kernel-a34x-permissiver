/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SH7264_H__
#define __ASM_SH7264_H__

enum {
	/* Port A */
	GPIO_PA3, GPIO_PA2, GPIO_PA1, GPIO_PA0,

	/* Port B */
	GPIO_PB22, GPIO_PB21, GPIO_PB20,
	GPIO_PB19, GPIO_PB18, GPIO_PB17, GPIO_PB16,
	GPIO_PB15, GPIO_PB14, GPIO_PB13, GPIO_PB12,
	GPIO_PB11, GPIO_PB10, GPIO_PB9, GPIO_PB8,
	GPIO_PB7, GPIO_PB6, GPIO_PB5, GPIO_PB4,
	GPIO_PB3, GPIO_PB2, GPIO_PB1,

	/* Port C */
	GPIO_PC10, GPIO_PC9, GPIO_PC8,
	GPIO_PC7, GPIO_PC6, GPIO_PC5, GPIO_PC4,
	GPIO_PC3, GPIO_PC2, GPIO_PC1, GPIO_PC0,

	/* Port D */
	GPIO_PD15, GPIO_PD14, GPIO_PD13, GPIO_PD12,
	GPIO_PD11, GPIO_PD10, GPIO_PD9, GPIO_PD8,
	GPIO_PD7, GPIO_PD6, GPIO_PD5, GPIO_PD4,
	GPIO_PD3, GPIO_PD2, GPIO_PD1, GPIO_PD0,

	/* Port E */
	GPIO_PE5, GPIO_PE4,
	GPIO_PE3, GPIO_PE2, GPIO_PE1, GPIO_PE0,

	/* Port F */
	GPIO_PF12,
	GPIO_PF11, GPIO_PF10, GPIO_PF9, GPIO_PF8,
	GPIO_PF7, GPIO_PF6, GPIO_PF5, GPIO_PF4,
	GPIO_PF3, GPIO_PF2, GPIO_PF1, GPIO_PF0,

	/* Port G */
	GPIO_PG24,
	GPIO_PG23, GPIO_PG22, GPIO_PG21, GPIO_PG20,
	GPIO_PG19, GPIO_PG18, GPIO_PG17, GPIO_PG16,
	GPIO_PG15, GPIO_PG14, GPIO_PG13, GPIO_PG12,
	GPIO_PG11, GPIO_PG10, GPIO_PG9, GPIO_PG8,
	GPIO_PG7, GPIO_PG6, GPIO_PG5, GPIO_PG4,
	GPIO_PG3, GPIO_PG2, GPIO_PG1, GPIO_PG0,

	/* Port H - Port H does not have a Data Register */

	/* Port I - not on device */

	/* Port J */
	GPIO_PJ11, GPIO_PJ10, GPIO_PJ9, GPIO_PJ8,
	GPIO_PJ7, GPIO_PJ6, GPIO_PJ5, GPIO_PJ4,
	GPIO_PJ3, GPIO_PJ2, GPIO_PJ1, GPIO_PJ0,

	/* Port K */
	GPIO_PK11, GPIO_PK10, GPIO_PK9, GPIO_PK8,
	GPIO_PK7, GPIO_PK6, GPIO_PK5, GPIO_PK4,
	GPIO_PK3, GPIO_PK2, GPIO_PK1, GPIO_PK0,

	/* INTC: IRQ and PINT on PB/PD/PE */
	GPIO_FN_PINT7_PG, GPIO_FN_PINT6_PG, GPIO_FN_PINT5_PG, GPIO_FN_PINT4_PG,
	GPIO_FN_PINT3_PG, GPIO_FN_PINT2_PG, GPIO_FN_PINT1_PG,

	GPIO_FN_IRQ7_PC, GPIO_FN_IRQ6_PC, GPIO_FN_IRQ5_PC, GPIO_FN_IRQ4_PC,
	GPIO_FN_IRQ3_PG, GPIO_FN_IRQ2_PG, GPIO_FN_IRQ1_PJ, GPIO_FN_IRQ0_PJ,
	GPIO_FN_IRQ3_PE, GPIO_FN_IRQ2_PE, GPIO_FN_IRQ1_PE, GPIO_FN_IRQ0_PE,

	/* WDT */
	GPIO_FN_WDTOVF,

	/* CAN */
	GPIO_FN_CTX1, GPIO_FN_CRX1, GPIO_FN_CTX0, GPIO_FN_CTX0_CTX1,
	GPIO_FN_CRX0, GPIO_FN_CRX0_CRX1,

	/* DMAC */
	GPIO_FN_TEND0, GPIO_FN_DACK0, GPIO_FN_DREQ0,
	GPIO_FN_TEND1, GPIO_FN_DACK1, GPIO_FN_DREQ1,

	/* ADC */
	GPIO_FN_ADTRG,

	/* BSC */

	GPIO_FN_A25, GPIO_FN_A24,
	GPIO_FN_A23, GPIO_FN_A22, GPIO_FN_A21, GPIO_FN_A20,
	GPIO_FN_A19, GPIO_FN_A18, GPIO_FN_A17, GPIO_FN_A16,
	GPIO_FN_A15, GPIO_FN_A14, GPIO_FN_A13, GPIO_FN_A12,
	GPIO_FN_A11, GPIO_FN_A10, GPIO_FN_A9, GPIO_FN_A8,
	GPIO_FN_A7, GPIO_FN_A6, GPIO_FN_A5, GPIO_FN_A4,
	GPIO_FN_A3, GPIO_FN_A2, GPIO_FN_A1, GPIO_FN_A0,
	GPIO_FN_D15, GPIO_FN_D14, GPIO_FN_D13, GPIO_FN_D12,
	GPIO_FN_D11, GPIO_FN_D10, GPIO_FN_D9, GPIO_FN_D8,
	GPIO_FN_D7, GPIO_FN_D6, GPIO_FN_D5, GPIO_FN_D4,
	GPIO_FN_D3, GPIO_FN_D2, GPIO_FN_D1, GPIO_FN_D0,

	GPIO_FN_BS,
	GPIO_FN_CS4, GPIO_FN_CS3, GPIO_FN_CS2, GPIO_FN_CS1, GPIO_FN_CS0,
	GPIO_FN_CS6CE1B, GPIO_FN_CS5CE1A,
	GPIO_FN_CE2A, GPIO_FN_CE2B,
	GPIO_FN_RD, GPIO_FN_RDWR,
	GPIO_FN_ICIOWRAH, GPIO_FN_ICIORD,
	GPIO_FN_WE1DQMUWE, GPIO_FN_WE0DQML,
	GPIO_FN_RAS, GPIO_FN_CAS, GPIO_FN_CKE,
	GPIO_FN_WAIT, GPIO_FN_BREQ, GPIO_FN_BACK,
	GPIO_FN_IOIS16,

	/* TMU */
	GPIO_FN_TIOC4D, GPIO_FN_TIOC4C, GPIO_FN_TIOC4B, GPIO_FN_TIOC4A,
	GPIO_FN_TIOC3D, GPIO_FN_TIOC3C, GPIO_FN_TIOC3B, GPIO_FN_TIOC3A,
	GPIO_FN_TIOC2B, GPIO_FN_TIOC1B, GPIO_FN_TIOC2A, GPIO_FN_TIOC1A,
	GPIO_FN_TIOC0D, GPIO_FN_TIOC0C, GPIO_FN_TIOC0B, GPIO_FN_TIOC0A,
	GPIO_FN_TCLKD, GPIO_FN_TCLKC, GPIO_FN_TCLKB, GPIO_FN_TCLKA,

<<<<<<< HEAD
	/* SSU */
	GPIO_FN_SCS0_PD, GPIO_FN_SSO0_PD, GPIO_FN_SSI0_PD, GPIO_FN_SSCK0_PD,
	GPIO_FN_SCS0_PF, GPIO_FN_SSO0_PF, GPIO_FN_SSI0_PF, GPIO_FN_SSCK0_PF,
	GPIO_FN_SCS1_PD, GPIO_FN_SSO1_PD, GPIO_FN_SSI1_PD, GPIO_FN_SSCK1_PD,
	GPIO_FN_SCS1_PF, GPIO_FN_SSO1_PF, GPIO_FN_SSI1_PF, GPIO_FN_SSCK1_PF,

=======
>>>>>>> upstream/android-13
	/* SCIF */
	GPIO_FN_SCK0, GPIO_FN_SCK1, GPIO_FN_SCK2, GPIO_FN_SCK3,
	GPIO_FN_RXD0, GPIO_FN_RXD1, GPIO_FN_RXD2, GPIO_FN_RXD3,
	GPIO_FN_TXD0, GPIO_FN_TXD1, GPIO_FN_TXD2, GPIO_FN_TXD3,
	GPIO_FN_RXD4, GPIO_FN_RXD5, GPIO_FN_RXD6, GPIO_FN_RXD7,
	GPIO_FN_TXD4, GPIO_FN_TXD5, GPIO_FN_TXD6, GPIO_FN_TXD7,
	GPIO_FN_RTS1, GPIO_FN_RTS3, GPIO_FN_CTS1, GPIO_FN_CTS3,

	/* RSPI */
	GPIO_FN_RSPCK0, GPIO_FN_MOSI0,
	GPIO_FN_MISO0_PF12, GPIO_FN_MISO1,
	GPIO_FN_SSL00,
	GPIO_FN_RSPCK1, GPIO_FN_MOSI1,
	GPIO_FN_MISO1_PG19, GPIO_FN_SSL10,

	/* IIC3 */
	GPIO_FN_SCL0, GPIO_FN_SCL1, GPIO_FN_SCL2,
	GPIO_FN_SDA2, GPIO_FN_SDA1, GPIO_FN_SDA0,

	/* SSI */
	GPIO_FN_SSISCK0, GPIO_FN_SSIWS0, GPIO_FN_SSITXD0, GPIO_FN_SSIRXD0,
	GPIO_FN_SSIWS1, GPIO_FN_SSIWS2, GPIO_FN_SSIWS3,
	GPIO_FN_SSISCK1, GPIO_FN_SSISCK2, GPIO_FN_SSISCK3,
	GPIO_FN_SSIDATA1, GPIO_FN_SSIDATA2, GPIO_FN_SSIDATA3,
	GPIO_FN_AUDIO_CLK,

	/* SIOF */
	GPIO_FN_SIOFTXD, GPIO_FN_SIOFRXD, GPIO_FN_SIOFSYNC, GPIO_FN_SIOFSCK,

	/* SPDIF */
	GPIO_FN_SPDIF_IN,
	GPIO_FN_SPDIF_OUT,

	/* NANDFMC */ /* NOTE Controller is not available in boot mode 0 */
	GPIO_FN_FCE,
	GPIO_FN_FRB,

	/* VDC3 */
	GPIO_FN_DV_CLK, GPIO_FN_DV_VSYNC, GPIO_FN_DV_HSYNC,
	GPIO_FN_DV_DATA7, GPIO_FN_DV_DATA6, GPIO_FN_DV_DATA5, GPIO_FN_DV_DATA4,
	GPIO_FN_DV_DATA3, GPIO_FN_DV_DATA2, GPIO_FN_DV_DATA1, GPIO_FN_DV_DATA0,
	GPIO_FN_LCD_CLK, GPIO_FN_LCD_EXTCLK,
	GPIO_FN_LCD_VSYNC, GPIO_FN_LCD_HSYNC, GPIO_FN_LCD_DE,
	GPIO_FN_LCD_DATA15, GPIO_FN_LCD_DATA14,
	GPIO_FN_LCD_DATA13, GPIO_FN_LCD_DATA12,
	GPIO_FN_LCD_DATA11, GPIO_FN_LCD_DATA10,
	GPIO_FN_LCD_DATA9, GPIO_FN_LCD_DATA8,
	GPIO_FN_LCD_DATA7, GPIO_FN_LCD_DATA6,
	GPIO_FN_LCD_DATA5, GPIO_FN_LCD_DATA4,
	GPIO_FN_LCD_DATA3, GPIO_FN_LCD_DATA2,
	GPIO_FN_LCD_DATA1, GPIO_FN_LCD_DATA0,
	GPIO_FN_LCD_M_DISP,
};

#endif /* __ASM_SH7264_H__ */
