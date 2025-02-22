<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2006 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * S3C2410 - SPI Controller platform_device info
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
*/

#ifndef __LINUX_SPI_S3C24XX_H
#define __LINUX_SPI_S3C24XX_H __FILE__

struct s3c2410_spi_info {
	int			 pin_cs;	/* simple gpio cs */
	unsigned int		 num_cs;	/* total chipselects */
	int			 bus_num;       /* bus number to use. */

	unsigned int		 use_fiq:1;	/* use fiq */

	void (*gpio_setup)(struct s3c2410_spi_info *spi, int enable);
	void (*set_cs)(struct s3c2410_spi_info *spi, int cs, int pol);
};

<<<<<<< HEAD
extern int s3c24xx_set_fiq(unsigned int irq, bool on);
=======
extern int s3c24xx_set_fiq(unsigned int irq, u32 *ack_ptr, bool on);
>>>>>>> upstream/android-13

#endif /* __LINUX_SPI_S3C24XX_H */
