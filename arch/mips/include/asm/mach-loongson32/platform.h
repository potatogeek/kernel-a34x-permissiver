<<<<<<< HEAD
/*
 * Copyright (c) 2011 Zhang, Keguang <keguang.zhang@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (c) 2011 Zhang, Keguang <keguang.zhang@gmail.com>
>>>>>>> upstream/android-13
 */

#ifndef __ASM_MACH_LOONGSON32_PLATFORM_H
#define __ASM_MACH_LOONGSON32_PLATFORM_H

#include <linux/platform_device.h>

#include <dma.h>
#include <nand.h>

extern struct platform_device ls1x_uart_pdev;
extern struct platform_device ls1x_cpufreq_pdev;
<<<<<<< HEAD
extern struct platform_device ls1x_dma_pdev;
=======
>>>>>>> upstream/android-13
extern struct platform_device ls1x_eth0_pdev;
extern struct platform_device ls1x_eth1_pdev;
extern struct platform_device ls1x_ehci_pdev;
extern struct platform_device ls1x_gpio0_pdev;
extern struct platform_device ls1x_gpio1_pdev;
<<<<<<< HEAD
extern struct platform_device ls1x_nand_pdev;
=======
>>>>>>> upstream/android-13
extern struct platform_device ls1x_rtc_pdev;
extern struct platform_device ls1x_wdt_pdev;

void __init ls1x_clk_init(void);
<<<<<<< HEAD
void __init ls1x_dma_set_platdata(struct plat_ls1x_dma *pdata);
void __init ls1x_nand_set_platdata(struct plat_ls1x_nand *pdata);
=======
>>>>>>> upstream/android-13
void __init ls1x_rtc_set_extclk(struct platform_device *pdev);
void __init ls1x_serial_set_uartclk(struct platform_device *pdev);

#endif /* __ASM_MACH_LOONGSON32_PLATFORM_H */
