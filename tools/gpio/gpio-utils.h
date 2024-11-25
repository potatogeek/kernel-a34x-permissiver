<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * GPIO tools - utility helpers library for the GPIO tools
 *
 * Copyright (C) 2015 Linus Walleij
 *
 * Portions copied from iio_utils and lssio:
 * Copyright (c) 2010 Manuel Stahl <manuel.stahl@iis.fraunhofer.de>
 * Copyright (c) 2008 Jonathan Cameron
 * *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#ifndef _GPIO_UTILS_H_
#define _GPIO_UTILS_H_

<<<<<<< HEAD
#include <string.h>
=======
#include <stdbool.h>
#include <string.h>
#include <linux/types.h>
>>>>>>> upstream/android-13

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static inline int check_prefix(const char *str, const char *prefix)
{
	return strlen(str) > strlen(prefix) &&
		strncmp(str, prefix, strlen(prefix)) == 0;
}

<<<<<<< HEAD
int gpiotools_request_linehandle(const char *device_name, unsigned int *lines,
				 unsigned int nlines, unsigned int flag,
				 struct gpiohandle_data *data,
				 const char *consumer_label);
int gpiotools_set_values(const int fd, struct gpiohandle_data *data);
int gpiotools_get_values(const int fd, struct gpiohandle_data *data);
int gpiotools_release_linehandle(const int fd);

int gpiotools_get(const char *device_name, unsigned int line);
int gpiotools_gets(const char *device_name, unsigned int *lines,
		   unsigned int nlines, struct gpiohandle_data *data);
int gpiotools_set(const char *device_name, unsigned int line,
		  unsigned int value);
int gpiotools_sets(const char *device_name, unsigned int *lines,
		   unsigned int nlines, struct gpiohandle_data *data);
=======
int gpiotools_request_line(const char *device_name,
			   unsigned int *lines,
			   unsigned int num_lines,
			   struct gpio_v2_line_config *config,
			   const char *consumer);
int gpiotools_set_values(const int fd, struct gpio_v2_line_values *values);
int gpiotools_get_values(const int fd, struct gpio_v2_line_values *values);
int gpiotools_release_line(const int fd);

int gpiotools_get(const char *device_name, unsigned int line);
int gpiotools_gets(const char *device_name, unsigned int *lines,
		   unsigned int num_lines, unsigned int *values);
int gpiotools_set(const char *device_name, unsigned int line,
		  unsigned int value);
int gpiotools_sets(const char *device_name, unsigned int *lines,
		   unsigned int num_lines, unsigned int *values);

/* helper functions for gpio_v2_line_values bits */
static inline void gpiotools_set_bit(__u64 *b, int n)
{
	*b |= _BITULL(n);
}

static inline void gpiotools_change_bit(__u64 *b, int n)
{
	*b ^= _BITULL(n);
}

static inline void gpiotools_clear_bit(__u64 *b, int n)
{
	*b &= ~_BITULL(n);
}

static inline int gpiotools_test_bit(__u64 b, int n)
{
	return !!(b & _BITULL(n));
}

static inline void gpiotools_assign_bit(__u64 *b, int n, bool value)
{
	if (value)
		gpiotools_set_bit(b, n);
	else
		gpiotools_clear_bit(b, n);
}
>>>>>>> upstream/android-13

#endif /* _GPIO_UTILS_H_ */
