<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (C) STMicroelectronics 2017
 *
 * Author: Fabrice Gasnier <fabrice.gasnier@st.com>
<<<<<<< HEAD
 *
 * License terms:  GNU General Public License (GPL), version 2
=======
>>>>>>> upstream/android-13
 */

#ifndef _STM32_LPTIM_TRIGGER_H_
#define _STM32_LPTIM_TRIGGER_H_

#include <linux/iio/iio.h>
#include <linux/iio/trigger.h>

#define LPTIM1_OUT	"lptim1_out"
#define LPTIM2_OUT	"lptim2_out"
#define LPTIM3_OUT	"lptim3_out"

#if IS_REACHABLE(CONFIG_IIO_STM32_LPTIMER_TRIGGER)
bool is_stm32_lptim_trigger(struct iio_trigger *trig);
#else
static inline bool is_stm32_lptim_trigger(struct iio_trigger *trig)
{
#if IS_ENABLED(CONFIG_IIO_STM32_LPTIMER_TRIGGER)
	pr_warn_once("stm32 lptim_trigger not linked in\n");
#endif
	return false;
}
#endif
#endif
