<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Generic GPIO card-detect helper header
 *
 * Copyright (C) 2011, Guennadi Liakhovetski <g.liakhovetski@gmx.de>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef MMC_SLOT_GPIO_H
#define MMC_SLOT_GPIO_H

#include <linux/types.h>
#include <linux/irqreturn.h>

struct mmc_host;

int mmc_gpio_get_ro(struct mmc_host *host);
<<<<<<< HEAD
int mmc_gpio_request_ro(struct mmc_host *host, unsigned int gpio);

int mmc_gpio_get_cd(struct mmc_host *host);
int mmc_gpio_request_cd(struct mmc_host *host, unsigned int gpio,
			unsigned int debounce);

int mmc_gpiod_request_cd(struct mmc_host *host, const char *con_id,
			 unsigned int idx, bool override_active_level,
			 unsigned int debounce, bool *gpio_invert);
int mmc_gpiod_request_ro(struct mmc_host *host, const char *con_id,
			 unsigned int idx, bool override_active_level,
			 unsigned int debounce, bool *gpio_invert);
=======
int mmc_gpio_get_cd(struct mmc_host *host);
int mmc_gpiod_request_cd(struct mmc_host *host, const char *con_id,
			 unsigned int idx, bool override_active_level,
			 unsigned int debounce);
int mmc_gpiod_request_ro(struct mmc_host *host, const char *con_id,
			 unsigned int idx, unsigned int debounce);
>>>>>>> upstream/android-13
void mmc_gpio_set_cd_isr(struct mmc_host *host,
			 irqreturn_t (*isr)(int irq, void *dev_id));
int mmc_gpio_set_cd_wake(struct mmc_host *host, bool on);
void mmc_gpiod_request_cd_irq(struct mmc_host *host);
bool mmc_can_gpio_cd(struct mmc_host *host);
bool mmc_can_gpio_ro(struct mmc_host *host);

#endif
