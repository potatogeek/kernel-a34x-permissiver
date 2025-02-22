<<<<<<< HEAD
/*
 * Copyright (C) 2014 Renesas Solutions Corp.
 * Copyright (C) 2014 Wolfram Sang, Sang Engineering <wsa@sang-engineering.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
=======
/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2014 Renesas Solutions Corp.
 * Copyright (C) 2014 Wolfram Sang, Sang Engineering <wsa@sang-engineering.com>
>>>>>>> upstream/android-13
 */

#ifndef __DT_BINDINGS_CLOCK_R7S72100_H__
#define __DT_BINDINGS_CLOCK_R7S72100_H__

#define R7S72100_CLK_PLL	0
#define R7S72100_CLK_I		1
#define R7S72100_CLK_G		2

/* MSTP2 */
#define R7S72100_CLK_CORESIGHT	0

/* MSTP3 */
#define R7S72100_CLK_IEBUS	7
#define R7S72100_CLK_IRDA	6
#define R7S72100_CLK_LIN0	5
#define R7S72100_CLK_LIN1	4
#define R7S72100_CLK_MTU2	3
#define R7S72100_CLK_CAN	2
#define R7S72100_CLK_ADCPWR	1
#define R7S72100_CLK_PWM	0

/* MSTP4 */
#define R7S72100_CLK_SCIF0	7
#define R7S72100_CLK_SCIF1	6
#define R7S72100_CLK_SCIF2	5
#define R7S72100_CLK_SCIF3	4
#define R7S72100_CLK_SCIF4	3
#define R7S72100_CLK_SCIF5	2
#define R7S72100_CLK_SCIF6	1
#define R7S72100_CLK_SCIF7	0

/* MSTP5 */
#define R7S72100_CLK_SCI0	7
#define R7S72100_CLK_SCI1	6
#define R7S72100_CLK_SG0	5
#define R7S72100_CLK_SG1	4
#define R7S72100_CLK_SG2	3
#define R7S72100_CLK_SG3	2
#define R7S72100_CLK_OSTM0	1
#define R7S72100_CLK_OSTM1	0

/* MSTP6 */
#define R7S72100_CLK_ADC	7
#define R7S72100_CLK_CEU	6
#define R7S72100_CLK_DOC0	5
#define R7S72100_CLK_DOC1	4
#define R7S72100_CLK_DRC0	3
#define R7S72100_CLK_DRC1	2
#define R7S72100_CLK_JCU	1
#define R7S72100_CLK_RTC	0

/* MSTP7 */
#define R7S72100_CLK_VDEC0	7
#define R7S72100_CLK_VDEC1	6
#define R7S72100_CLK_ETHER	4
#define R7S72100_CLK_NAND	3
#define R7S72100_CLK_USB0	1
#define R7S72100_CLK_USB1	0

/* MSTP8 */
#define R7S72100_CLK_IMR0	7
#define R7S72100_CLK_IMR1	6
#define R7S72100_CLK_IMRDISP	5
#define R7S72100_CLK_MMCIF	4
#define R7S72100_CLK_MLB	3
#define R7S72100_CLK_ETHAVB	2
#define R7S72100_CLK_SCUX	1

/* MSTP9 */
#define R7S72100_CLK_I2C0	7
#define R7S72100_CLK_I2C1	6
#define R7S72100_CLK_I2C2	5
#define R7S72100_CLK_I2C3	4
#define R7S72100_CLK_SPIBSC0	3
#define R7S72100_CLK_SPIBSC1	2
#define R7S72100_CLK_VDC50	1	/* and LVDS */
#define R7S72100_CLK_VDC51	0

/* MSTP10 */
#define R7S72100_CLK_SPI0	7
#define R7S72100_CLK_SPI1	6
#define R7S72100_CLK_SPI2	5
#define R7S72100_CLK_SPI3	4
#define R7S72100_CLK_SPI4	3
#define R7S72100_CLK_CDROM	2
#define R7S72100_CLK_SPDIF	1
#define R7S72100_CLK_RGPVG2	0

/* MSTP11 */
#define R7S72100_CLK_SSI0	5
#define R7S72100_CLK_SSI1	4
#define R7S72100_CLK_SSI2	3
#define R7S72100_CLK_SSI3	2
#define R7S72100_CLK_SSI4	1
#define R7S72100_CLK_SSI5	0

/* MSTP12 */
#define R7S72100_CLK_SDHI00	3
#define R7S72100_CLK_SDHI01	2
#define R7S72100_CLK_SDHI10	1
#define R7S72100_CLK_SDHI11	0

/* MSTP13 */
#define R7S72100_CLK_PIX1	2
#define R7S72100_CLK_PIX0	1

#endif /* __DT_BINDINGS_CLOCK_R7S72100_H__ */
