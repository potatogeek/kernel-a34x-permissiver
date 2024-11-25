<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * gpio-hammer - example swiss army knife to shake GPIO lines on a system
 *
 * Copyright (C) 2016 Linus Walleij
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 * Usage:
 *	gpio-hammer -n <device-name> -o <offset1> -o <offset2>
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>
#include "gpio-utils.h"

<<<<<<< HEAD
int hammer_device(const char *device_name, unsigned int *lines, int nlines,
		  unsigned int loops)
{
	struct gpiohandle_data data;
=======
int hammer_device(const char *device_name, unsigned int *lines, int num_lines,
		  unsigned int loops)
{
	struct gpio_v2_line_values values;
	struct gpio_v2_line_config config;
>>>>>>> upstream/android-13
	char swirr[] = "-\\|/";
	int fd;
	int ret;
	int i, j;
	unsigned int iteration = 0;

<<<<<<< HEAD
	memset(&data.values, 0, sizeof(data.values));
	ret = gpiotools_request_linehandle(device_name, lines, nlines,
					   GPIOHANDLE_REQUEST_OUTPUT, &data,
					   "gpio-hammer");
=======
	memset(&config, 0, sizeof(config));
	config.flags = GPIO_V2_LINE_FLAG_OUTPUT;

	ret = gpiotools_request_line(device_name, lines, num_lines,
				     &config, "gpio-hammer");
>>>>>>> upstream/android-13
	if (ret < 0)
		goto exit_error;
	else
		fd = ret;

<<<<<<< HEAD
	ret = gpiotools_get_values(fd, &data);
=======
	values.mask = 0;
	values.bits = 0;
	for (i = 0; i < num_lines; i++)
		gpiotools_set_bit(&values.mask, i);

	ret = gpiotools_get_values(fd, &values);
>>>>>>> upstream/android-13
	if (ret < 0)
		goto exit_close_error;

	fprintf(stdout, "Hammer lines [");
<<<<<<< HEAD
	for (i = 0; i < nlines; i++) {
		fprintf(stdout, "%d", lines[i]);
		if (i != (nlines - 1))
			fprintf(stdout, ", ");
	}
	fprintf(stdout, "] on %s, initial states: [", device_name);
	for (i = 0; i < nlines; i++) {
		fprintf(stdout, "%d", data.values[i]);
		if (i != (nlines - 1))
=======
	for (i = 0; i < num_lines; i++) {
		fprintf(stdout, "%d", lines[i]);
		if (i != (num_lines - 1))
			fprintf(stdout, ", ");
	}
	fprintf(stdout, "] on %s, initial states: [", device_name);
	for (i = 0; i < num_lines; i++) {
		fprintf(stdout, "%d", gpiotools_test_bit(values.bits, i));
		if (i != (num_lines - 1))
>>>>>>> upstream/android-13
			fprintf(stdout, ", ");
	}
	fprintf(stdout, "]\n");

	/* Hammertime! */
	j = 0;
	while (1) {
		/* Invert all lines so we blink */
<<<<<<< HEAD
		for (i = 0; i < nlines; i++)
			data.values[i] = !data.values[i];

		ret = gpiotools_set_values(fd, &data);
=======
		for (i = 0; i < num_lines; i++)
			gpiotools_change_bit(&values.bits, i);

		ret = gpiotools_set_values(fd, &values);
>>>>>>> upstream/android-13
		if (ret < 0)
			goto exit_close_error;

		/* Re-read values to get status */
<<<<<<< HEAD
		ret = gpiotools_get_values(fd, &data);
=======
		ret = gpiotools_get_values(fd, &values);
>>>>>>> upstream/android-13
		if (ret < 0)
			goto exit_close_error;

		fprintf(stdout, "[%c] ", swirr[j]);
		j++;
<<<<<<< HEAD
		if (j == sizeof(swirr)-1)
			j = 0;

		fprintf(stdout, "[");
		for (i = 0; i < nlines; i++) {
			fprintf(stdout, "%d: %d", lines[i], data.values[i]);
			if (i != (nlines - 1))
=======
		if (j == sizeof(swirr) - 1)
			j = 0;

		fprintf(stdout, "[");
		for (i = 0; i < num_lines; i++) {
			fprintf(stdout, "%d: %d", lines[i],
				gpiotools_test_bit(values.bits, i));
			if (i != (num_lines - 1))
>>>>>>> upstream/android-13
				fprintf(stdout, ", ");
		}
		fprintf(stdout, "]\r");
		fflush(stdout);
		sleep(1);
		iteration++;
		if (loops && iteration == loops)
			break;
	}
	fprintf(stdout, "\n");
	ret = 0;

exit_close_error:
<<<<<<< HEAD
	gpiotools_release_linehandle(fd);
=======
	gpiotools_release_line(fd);
>>>>>>> upstream/android-13
exit_error:
	return ret;
}

void print_usage(void)
{
	fprintf(stderr, "Usage: gpio-hammer [options]...\n"
		"Hammer GPIO lines, 0->1->0->1...\n"
		"  -n <name>  Hammer GPIOs on a named device (must be stated)\n"
		"  -o <n>     Offset[s] to hammer, at least one, several can be stated\n"
		" [-c <n>]    Do <n> loops (optional, infinite loop if not stated)\n"
		"  -?         This helptext\n"
		"\n"
		"Example:\n"
		"gpio-hammer -n gpiochip0 -o 4\n"
	);
}

int main(int argc, char **argv)
{
	const char *device_name = NULL;
	unsigned int lines[GPIOHANDLES_MAX];
	unsigned int loops = 0;
<<<<<<< HEAD
	int nlines;
=======
	int num_lines;
>>>>>>> upstream/android-13
	int c;
	int i;

	i = 0;
	while ((c = getopt(argc, argv, "c:n:o:?")) != -1) {
		switch (c) {
		case 'c':
			loops = strtoul(optarg, NULL, 10);
			break;
		case 'n':
			device_name = optarg;
			break;
		case 'o':
			/*
			 * Avoid overflow. Do not immediately error, we want to
			 * be able to accurately report on the amount of times
			 * '-o' was given to give an accurate error message
			 */
			if (i < GPIOHANDLES_MAX)
				lines[i] = strtoul(optarg, NULL, 10);

			i++;
			break;
		case '?':
			print_usage();
			return -1;
		}
	}

	if (i >= GPIOHANDLES_MAX) {
		fprintf(stderr,
<<<<<<< HEAD
			"Only %d occurences of '-o' are allowed, %d were found\n",
=======
			"Only %d occurrences of '-o' are allowed, %d were found\n",
>>>>>>> upstream/android-13
			GPIOHANDLES_MAX, i + 1);
		return -1;
	}

<<<<<<< HEAD
	nlines = i;

	if (!device_name || !nlines) {
		print_usage();
		return -1;
	}
	return hammer_device(device_name, lines, nlines, loops);
=======
	num_lines = i;

	if (!device_name || !num_lines) {
		print_usage();
		return -1;
	}
	return hammer_device(device_name, lines, num_lines, loops);
>>>>>>> upstream/android-13
}
