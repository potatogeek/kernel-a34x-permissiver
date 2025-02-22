/*
 * Copyright 2012-15 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: AMD
 *
 */

#ifndef __DAL_GPIO_SERVICE_H__
#define __DAL_GPIO_SERVICE_H__

struct hw_translate;
struct hw_factory;

struct gpio_service {
	struct dc_context *ctx;
	struct hw_translate translate;
	struct hw_factory factory;
	/*
	 * @brief
	 * Business storage.
<<<<<<< HEAD
	 * For each member of 'enum gpio_id',
	 * store array of bits (packed into uint32_t slots),
	 * index individual bit by 'en' value */
	uint32_t *busyness[GPIO_ID_COUNT];
};

enum gpio_result dal_gpio_service_open(
	struct gpio_service *service,
	enum gpio_id id,
	uint32_t en,
	enum gpio_mode mode,
	struct hw_gpio_pin **ptr);
=======
	 * one byte For each member of 'enum gpio_id'
	 */
	char *busyness[GPIO_ID_COUNT];
};

enum gpio_result dal_gpio_service_open(
	struct gpio *gpio);
>>>>>>> upstream/android-13

void dal_gpio_service_close(
	struct gpio_service *service,
	struct hw_gpio_pin **ptr);

<<<<<<< HEAD
=======
enum gpio_result dal_gpio_service_lock(
	struct gpio_service *service,
	enum gpio_id id,
	uint32_t en);

enum gpio_result dal_gpio_service_unlock(
	struct gpio_service *service,
	enum gpio_id id,
	uint32_t en);

>>>>>>> upstream/android-13
#endif
