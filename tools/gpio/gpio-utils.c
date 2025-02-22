<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * GPIO tools - helpers library for the GPIO tools
 *
 * Copyright (C) 2015 Linus Walleij
 * Copyright (C) 2016 Bamvor Jian Zhang
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>
#include "gpio-utils.h"

<<<<<<< HEAD
#define COMSUMER "gpio-utils"

/**
 * doc: Operation of gpio
=======
#define CONSUMER "gpio-utils"

/**
 * DOC: Operation of gpio
>>>>>>> upstream/android-13
 *
 * Provide the api of gpiochip for chardev interface. There are two
 * types of api.  The first one provide as same function as each
 * ioctl, including request and release for lines of gpio, read/write
 * the value of gpio. If the user want to do lots of read and write of
 * lines of gpio, user should use this type of api.
 *
 * The second one provide the easy to use api for user. Each of the
 * following api will request gpio lines, do the operation and then
 * release these lines.
 */
<<<<<<< HEAD
/**
 * gpiotools_request_linehandle() - request gpio lines in a gpiochip
=======

/**
 * gpiotools_request_line() - request gpio lines in a gpiochip
>>>>>>> upstream/android-13
 * @device_name:	The name of gpiochip without prefix "/dev/",
 *			such as "gpiochip0"
 * @lines:		An array desired lines, specified by offset
 *			index for the associated GPIO device.
<<<<<<< HEAD
 * @nline:		The number of lines to request.
 * @flag:		The new flag for requsted gpio. Reference
 *			"linux/gpio.h" for the meaning of flag.
 * @data:		Default value will be set to gpio when flag is
 *			GPIOHANDLE_REQUEST_OUTPUT.
 * @consumer_label:	The name of consumer, such as "sysfs",
=======
 * @num_lines:		The number of lines to request.
 * @config:		The new config for requested gpio. Reference
 *			"linux/gpio.h" for config details.
 * @consumer:		The name of consumer, such as "sysfs",
>>>>>>> upstream/android-13
 *			"powerkey". This is useful for other users to
 *			know who is using.
 *
 * Request gpio lines through the ioctl provided by chardev. User
 * could call gpiotools_set_values() and gpiotools_get_values() to
 * read and write respectively through the returned fd. Call
<<<<<<< HEAD
 * gpiotools_release_linehandle() to release these lines after that.
=======
 * gpiotools_release_line() to release these lines after that.
>>>>>>> upstream/android-13
 *
 * Return:		On success return the fd;
 *			On failure return the errno.
 */
<<<<<<< HEAD
int gpiotools_request_linehandle(const char *device_name, unsigned int *lines,
				 unsigned int nlines, unsigned int flag,
				 struct gpiohandle_data *data,
				 const char *consumer_label)
{
	struct gpiohandle_request req;
=======
int gpiotools_request_line(const char *device_name, unsigned int *lines,
			   unsigned int num_lines,
			   struct gpio_v2_line_config *config,
			   const char *consumer)
{
	struct gpio_v2_line_request req;
>>>>>>> upstream/android-13
	char *chrdev_name;
	int fd;
	int i;
	int ret;

	ret = asprintf(&chrdev_name, "/dev/%s", device_name);
	if (ret < 0)
		return -ENOMEM;

	fd = open(chrdev_name, 0);
	if (fd == -1) {
		ret = -errno;
		fprintf(stderr, "Failed to open %s, %s\n",
			chrdev_name, strerror(errno));
<<<<<<< HEAD
		goto exit_close_error;
	}

	for (i = 0; i < nlines; i++)
		req.lineoffsets[i] = lines[i];

	req.flags = flag;
	strcpy(req.consumer_label, consumer_label);
	req.lines = nlines;
	if (flag & GPIOHANDLE_REQUEST_OUTPUT)
		memcpy(req.default_values, data, sizeof(req.default_values));

	ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
	if (ret == -1) {
		ret = -errno;
		fprintf(stderr, "Failed to issue %s (%d), %s\n",
			"GPIO_GET_LINEHANDLE_IOCTL", ret, strerror(errno));
	}

exit_close_error:
	if (close(fd) == -1)
		perror("Failed to close GPIO character device file");
	free(chrdev_name);
	return ret < 0 ? ret : req.fd;
}
/**
 * gpiotools_set_values(): Set the value of gpio(s)
 * @fd:			The fd returned by
 *			gpiotools_request_linehandle().
 * @data:		The array of values want to set.
=======
		goto exit_free_name;
	}

	memset(&req, 0, sizeof(req));
	for (i = 0; i < num_lines; i++)
		req.offsets[i] = lines[i];

	req.config = *config;
	strcpy(req.consumer, consumer);
	req.num_lines = num_lines;

	ret = ioctl(fd, GPIO_V2_GET_LINE_IOCTL, &req);
	if (ret == -1) {
		ret = -errno;
		fprintf(stderr, "Failed to issue %s (%d), %s\n",
			"GPIO_GET_LINE_IOCTL", ret, strerror(errno));
	}

	if (close(fd) == -1)
		perror("Failed to close GPIO character device file");
exit_free_name:
	free(chrdev_name);
	return ret < 0 ? ret : req.fd;
}

/**
 * gpiotools_set_values() - Set the value of gpio(s)
 * @fd:			The fd returned by
 *			gpiotools_request_line().
 * @values:		The array of values want to set.
>>>>>>> upstream/android-13
 *
 * Return:		On success return 0;
 *			On failure return the errno.
 */
<<<<<<< HEAD
int gpiotools_set_values(const int fd, struct gpiohandle_data *data)
{
	int ret;

	ret = ioctl(fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, data);
=======
int gpiotools_set_values(const int fd, struct gpio_v2_line_values *values)
{
	int ret;

	ret = ioctl(fd, GPIO_V2_LINE_SET_VALUES_IOCTL, values);
>>>>>>> upstream/android-13
	if (ret == -1) {
		ret = -errno;
		fprintf(stderr, "Failed to issue %s (%d), %s\n",
			"GPIOHANDLE_SET_LINE_VALUES_IOCTL", ret,
			strerror(errno));
	}

	return ret;
}

/**
<<<<<<< HEAD
 * gpiotools_get_values(): Get the value of gpio(s)
 * @fd:			The fd returned by
 *			gpiotools_request_linehandle().
 * @data:		The array of values get from hardware.
=======
 * gpiotools_get_values() - Get the value of gpio(s)
 * @fd:			The fd returned by
 *			gpiotools_request_line().
 * @values:		The array of values get from hardware.
>>>>>>> upstream/android-13
 *
 * Return:		On success return 0;
 *			On failure return the errno.
 */
<<<<<<< HEAD
int gpiotools_get_values(const int fd, struct gpiohandle_data *data)
{
	int ret;

	ret = ioctl(fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, data);
=======
int gpiotools_get_values(const int fd, struct gpio_v2_line_values *values)
{
	int ret;

	ret = ioctl(fd, GPIO_V2_LINE_GET_VALUES_IOCTL, values);
>>>>>>> upstream/android-13
	if (ret == -1) {
		ret = -errno;
		fprintf(stderr, "Failed to issue %s (%d), %s\n",
			"GPIOHANDLE_GET_LINE_VALUES_IOCTL", ret,
			strerror(errno));
	}

	return ret;
}

/**
<<<<<<< HEAD
 * gpiotools_release_linehandle(): Release the line(s) of gpiochip
 * @fd:			The fd returned by
 *			gpiotools_request_linehandle().
=======
 * gpiotools_release_line() - Release the line(s) of gpiochip
 * @fd:			The fd returned by
 *			gpiotools_request_line().
>>>>>>> upstream/android-13
 *
 * Return:		On success return 0;
 *			On failure return the errno.
 */
<<<<<<< HEAD
int gpiotools_release_linehandle(const int fd)
=======
int gpiotools_release_line(const int fd)
>>>>>>> upstream/android-13
{
	int ret;

	ret = close(fd);
	if (ret == -1) {
<<<<<<< HEAD
		perror("Failed to close GPIO LINEHANDLE device file");
=======
		perror("Failed to close GPIO LINE device file");
>>>>>>> upstream/android-13
		ret = -errno;
	}

	return ret;
}

/**
<<<<<<< HEAD
 * gpiotools_get(): Get value from specific line
=======
 * gpiotools_get() - Get value from specific line
>>>>>>> upstream/android-13
 * @device_name:	The name of gpiochip without prefix "/dev/",
 *			such as "gpiochip0"
 * @line:		number of line, such as 2.
 *
 * Return:		On success return 0;
 *			On failure return the errno.
 */
int gpiotools_get(const char *device_name, unsigned int line)
{
<<<<<<< HEAD
	struct gpiohandle_data data;
	unsigned int lines[] = {line};

	gpiotools_gets(device_name, lines, 1, &data);
	return data.values[0];
=======
	int ret;
	unsigned int value;
	unsigned int lines[] = {line};

	ret = gpiotools_gets(device_name, lines, 1, &value);
	if (ret)
		return ret;
	return value;
>>>>>>> upstream/android-13
}


/**
<<<<<<< HEAD
 * gpiotools_gets(): Get values from specific lines.
=======
 * gpiotools_gets() - Get values from specific lines.
>>>>>>> upstream/android-13
 * @device_name:	The name of gpiochip without prefix "/dev/",
 *			such as "gpiochip0".
 * @lines:		An array desired lines, specified by offset
 *			index for the associated GPIO device.
<<<<<<< HEAD
 * @nline:		The number of lines to request.
 * @data:		The array of values get from gpiochip.
=======
 * @num_lines:		The number of lines to request.
 * @values:		The array of values get from gpiochip.
>>>>>>> upstream/android-13
 *
 * Return:		On success return 0;
 *			On failure return the errno.
 */
int gpiotools_gets(const char *device_name, unsigned int *lines,
<<<<<<< HEAD
		   unsigned int nlines, struct gpiohandle_data *data)
{
	int fd;
	int ret;
	int ret_close;

	ret = gpiotools_request_linehandle(device_name, lines, nlines,
					   GPIOHANDLE_REQUEST_INPUT, data,
					   COMSUMER);
=======
		   unsigned int num_lines, unsigned int *values)
{
	int fd, i;
	int ret;
	int ret_close;
	struct gpio_v2_line_config config;
	struct gpio_v2_line_values lv;

	memset(&config, 0, sizeof(config));
	config.flags = GPIO_V2_LINE_FLAG_INPUT;
	ret = gpiotools_request_line(device_name, lines, num_lines,
				     &config, CONSUMER);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;

	fd = ret;
<<<<<<< HEAD
	ret = gpiotools_get_values(fd, data);
	ret_close = gpiotools_release_linehandle(fd);
=======
	for (i = 0; i < num_lines; i++)
		gpiotools_set_bit(&lv.mask, i);
	ret = gpiotools_get_values(fd, &lv);
	if (!ret)
		for (i = 0; i < num_lines; i++)
			values[i] = gpiotools_test_bit(lv.bits, i);
	ret_close = gpiotools_release_line(fd);
>>>>>>> upstream/android-13
	return ret < 0 ? ret : ret_close;
}

/**
<<<<<<< HEAD
 * gpiotools_set(): Set value to specific line
=======
 * gpiotools_set() - Set value to specific line
>>>>>>> upstream/android-13
 * @device_name:	The name of gpiochip without prefix "/dev/",
 *			such as "gpiochip0"
 * @line:		number of line, such as 2.
 * @value:		The value of gpio, must be 0(low) or 1(high).
 *
 * Return:		On success return 0;
 *			On failure return the errno.
 */
int gpiotools_set(const char *device_name, unsigned int line,
		  unsigned int value)
{
<<<<<<< HEAD
	struct gpiohandle_data data;
	unsigned int lines[] = {line};

	data.values[0] = value;
	return gpiotools_sets(device_name, lines, 1, &data);
}

/**
 * gpiotools_sets(): Set values to specific lines.
=======
	unsigned int lines[] = {line};

	return gpiotools_sets(device_name, lines, 1, &value);
}

/**
 * gpiotools_sets() - Set values to specific lines.
>>>>>>> upstream/android-13
 * @device_name:	The name of gpiochip without prefix "/dev/",
 *			such as "gpiochip0".
 * @lines:		An array desired lines, specified by offset
 *			index for the associated GPIO device.
<<<<<<< HEAD
 * @nline:		The number of lines to request.
 * @data:		The array of values set to gpiochip, must be
=======
 * @num_lines:		The number of lines to request.
 * @values:		The array of values set to gpiochip, must be
>>>>>>> upstream/android-13
 *			0(low) or 1(high).
 *
 * Return:		On success return 0;
 *			On failure return the errno.
 */
int gpiotools_sets(const char *device_name, unsigned int *lines,
<<<<<<< HEAD
		   unsigned int nlines, struct gpiohandle_data *data)
{
	int ret;

	ret = gpiotools_request_linehandle(device_name, lines, nlines,
					   GPIOHANDLE_REQUEST_OUTPUT, data,
					   COMSUMER);
	if (ret < 0)
		return ret;

	return gpiotools_release_linehandle(ret);
=======
		   unsigned int num_lines, unsigned int *values)
{
	int ret, i;
	struct gpio_v2_line_config config;

	memset(&config, 0, sizeof(config));
	config.flags = GPIO_V2_LINE_FLAG_OUTPUT;
	config.num_attrs = 1;
	config.attrs[0].attr.id = GPIO_V2_LINE_ATTR_ID_OUTPUT_VALUES;
	for (i = 0; i < num_lines; i++) {
		gpiotools_set_bit(&config.attrs[0].mask, i);
		gpiotools_assign_bit(&config.attrs[0].attr.values,
				     i, values[i]);
	}
	ret = gpiotools_request_line(device_name, lines, num_lines,
				     &config, CONSUMER);
	if (ret < 0)
		return ret;

	return gpiotools_release_line(ret);
>>>>>>> upstream/android-13
}
