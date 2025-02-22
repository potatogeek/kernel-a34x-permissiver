/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SPI_SPI_OC_TINY_H
#define _LINUX_SPI_SPI_OC_TINY_H

/**
 * struct tiny_spi_platform_data - platform data of the OpenCores tiny SPI
 * @freq:	input clock freq to the core.
 * @baudwidth:	baud rate divider width of the core.
<<<<<<< HEAD
 * @gpio_cs_count:	number of gpio pins used for chipselect.
 * @gpio_cs:	array of gpio pins used for chipselect.
=======
>>>>>>> upstream/android-13
 *
 * freq and baudwidth are used only if the divider is programmable.
 */
struct tiny_spi_platform_data {
	unsigned int freq;
	unsigned int baudwidth;
<<<<<<< HEAD
	unsigned int gpio_cs_count;
	int *gpio_cs;
=======
>>>>>>> upstream/android-13
};

#endif /* _LINUX_SPI_SPI_OC_TINY_H */
