<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * gpio-event-mon - monitor GPIO line events from userspace
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
 *	gpio-event-mon -n <device-name> -o <offset>
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/gpio.h>
<<<<<<< HEAD

int monitor_device(const char *device_name,
		   unsigned int line,
		   uint32_t handleflags,
		   uint32_t eventflags,
		   unsigned int loops)
{
	struct gpioevent_request req;
	struct gpiohandle_data data;
	char *chrdev_name;
	int fd;
=======
#include "gpio-utils.h"

int monitor_device(const char *device_name,
		   unsigned int *lines,
		   unsigned int num_lines,
		   struct gpio_v2_line_config *config,
		   unsigned int loops)
{
	struct gpio_v2_line_values values;
	char *chrdev_name;
	int cfd, lfd;
>>>>>>> upstream/android-13
	int ret;
	int i = 0;

	ret = asprintf(&chrdev_name, "/dev/%s", device_name);
	if (ret < 0)
		return -ENOMEM;

<<<<<<< HEAD
	fd = open(chrdev_name, 0);
	if (fd == -1) {
		ret = -errno;
		fprintf(stderr, "Failed to open %s\n", chrdev_name);
		goto exit_close_error;
	}

	req.lineoffset = line;
	req.handleflags = handleflags;
	req.eventflags = eventflags;
	strcpy(req.consumer_label, "gpio-event-mon");

	ret = ioctl(fd, GPIO_GET_LINEEVENT_IOCTL, &req);
	if (ret == -1) {
		ret = -errno;
		fprintf(stderr, "Failed to issue GET EVENT "
			"IOCTL (%d)\n",
			ret);
		goto exit_close_error;
	}

	/* Read initial states */
	ret = ioctl(req.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);
	if (ret == -1) {
		ret = -errno;
		fprintf(stderr, "Failed to issue GPIOHANDLE GET LINE "
			"VALUES IOCTL (%d)\n",
			ret);
		goto exit_close_error;
	}

	fprintf(stdout, "Monitoring line %d on %s\n", line, device_name);
	fprintf(stdout, "Initial line value: %d\n", data.values[0]);

	while (1) {
		struct gpioevent_data event;

		ret = read(req.fd, &event, sizeof(event));
=======
	cfd = open(chrdev_name, 0);
	if (cfd == -1) {
		ret = -errno;
		fprintf(stderr, "Failed to open %s\n", chrdev_name);
		goto exit_free_name;
	}

	ret = gpiotools_request_line(device_name, lines, num_lines, config,
				     "gpio-event-mon");
	if (ret < 0)
		goto exit_device_close;
	else
		lfd = ret;

	/* Read initial states */
	values.mask = 0;
	values.bits = 0;
	for (i = 0; i < num_lines; i++)
		gpiotools_set_bit(&values.mask, i);
	ret = gpiotools_get_values(lfd, &values);
	if (ret < 0) {
		fprintf(stderr,
			"Failed to issue GPIO LINE GET VALUES IOCTL (%d)\n",
			ret);
		goto exit_line_close;
	}

	if (num_lines == 1) {
		fprintf(stdout, "Monitoring line %d on %s\n", lines[0], device_name);
		fprintf(stdout, "Initial line value: %d\n",
			gpiotools_test_bit(values.bits, 0));
	} else {
		fprintf(stdout, "Monitoring lines %d", lines[0]);
		for (i = 1; i < num_lines - 1; i++)
			fprintf(stdout, ", %d", lines[i]);
		fprintf(stdout, " and %d on %s\n", lines[i], device_name);
		fprintf(stdout, "Initial line values: %d",
			gpiotools_test_bit(values.bits, 0));
		for (i = 1; i < num_lines - 1; i++)
			fprintf(stdout, ", %d",
				gpiotools_test_bit(values.bits, i));
		fprintf(stdout, " and %d\n",
			gpiotools_test_bit(values.bits, i));
	}

	while (1) {
		struct gpio_v2_line_event event;

		ret = read(lfd, &event, sizeof(event));
>>>>>>> upstream/android-13
		if (ret == -1) {
			if (errno == -EAGAIN) {
				fprintf(stderr, "nothing available\n");
				continue;
			} else {
				ret = -errno;
				fprintf(stderr, "Failed to read event (%d)\n",
					ret);
				break;
			}
		}

		if (ret != sizeof(event)) {
			fprintf(stderr, "Reading event failed\n");
			ret = -EIO;
			break;
		}
<<<<<<< HEAD
		fprintf(stdout, "GPIO EVENT %llu: ", event.timestamp);
		switch (event.id) {
		case GPIOEVENT_EVENT_RISING_EDGE:
			fprintf(stdout, "rising edge");
			break;
		case GPIOEVENT_EVENT_FALLING_EDGE:
=======
		fprintf(stdout, "GPIO EVENT at %" PRIu64 " on line %d (%d|%d) ",
			(uint64_t)event.timestamp_ns, event.offset, event.line_seqno,
			event.seqno);
		switch (event.id) {
		case GPIO_V2_LINE_EVENT_RISING_EDGE:
			fprintf(stdout, "rising edge");
			break;
		case GPIO_V2_LINE_EVENT_FALLING_EDGE:
>>>>>>> upstream/android-13
			fprintf(stdout, "falling edge");
			break;
		default:
			fprintf(stdout, "unknown event");
		}
		fprintf(stdout, "\n");

		i++;
		if (i == loops)
			break;
	}

<<<<<<< HEAD
exit_close_error:
	if (close(fd) == -1)
		perror("Failed to close GPIO character device file");
=======
exit_line_close:
	if (close(lfd) == -1)
		perror("Failed to close line file");
exit_device_close:
	if (close(cfd) == -1)
		perror("Failed to close GPIO character device file");
exit_free_name:
>>>>>>> upstream/android-13
	free(chrdev_name);
	return ret;
}

void print_usage(void)
{
	fprintf(stderr, "Usage: gpio-event-mon [options]...\n"
		"Listen to events on GPIO lines, 0->1 1->0\n"
		"  -n <name>  Listen on GPIOs on a named device (must be stated)\n"
<<<<<<< HEAD
		"  -o <n>     Offset to monitor\n"
=======
		"  -o <n>     Offset of line to monitor (may be repeated)\n"
>>>>>>> upstream/android-13
		"  -d         Set line as open drain\n"
		"  -s         Set line as open source\n"
		"  -r         Listen for rising edges\n"
		"  -f         Listen for falling edges\n"
<<<<<<< HEAD
=======
		"  -w         Report the wall-clock time for events\n"
		"  -b <n>     Debounce the line with period n microseconds\n"
>>>>>>> upstream/android-13
		" [-c <n>]    Do <n> loops (optional, infinite loop if not stated)\n"
		"  -?         This helptext\n"
		"\n"
		"Example:\n"
<<<<<<< HEAD
		"gpio-event-mon -n gpiochip0 -o 4 -r -f\n"
	);
}

int main(int argc, char **argv)
{
	const char *device_name = NULL;
	unsigned int line = -1;
	unsigned int loops = 0;
	uint32_t handleflags = GPIOHANDLE_REQUEST_INPUT;
	uint32_t eventflags = 0;
	int c;

	while ((c = getopt(argc, argv, "c:n:o:dsrf?")) != -1) {
=======
		"gpio-event-mon -n gpiochip0 -o 4 -r -f -b 10000\n"
	);
}

#define EDGE_FLAGS \
	(GPIO_V2_LINE_FLAG_EDGE_RISING | \
	 GPIO_V2_LINE_FLAG_EDGE_FALLING)

int main(int argc, char **argv)
{
	const char *device_name = NULL;
	unsigned int lines[GPIO_V2_LINES_MAX];
	unsigned int num_lines = 0;
	unsigned int loops = 0;
	struct gpio_v2_line_config config;
	int c, attr, i;
	unsigned long debounce_period_us = 0;

	memset(&config, 0, sizeof(config));
	config.flags = GPIO_V2_LINE_FLAG_INPUT;
	while ((c = getopt(argc, argv, "c:n:o:b:dsrfw?")) != -1) {
>>>>>>> upstream/android-13
		switch (c) {
		case 'c':
			loops = strtoul(optarg, NULL, 10);
			break;
		case 'n':
			device_name = optarg;
			break;
		case 'o':
<<<<<<< HEAD
			line = strtoul(optarg, NULL, 10);
			break;
		case 'd':
			handleflags |= GPIOHANDLE_REQUEST_OPEN_DRAIN;
			break;
		case 's':
			handleflags |= GPIOHANDLE_REQUEST_OPEN_SOURCE;
			break;
		case 'r':
			eventflags |= GPIOEVENT_REQUEST_RISING_EDGE;
			break;
		case 'f':
			eventflags |= GPIOEVENT_REQUEST_FALLING_EDGE;
=======
			if (num_lines >= GPIO_V2_LINES_MAX) {
				print_usage();
				return -1;
			}
			lines[num_lines] = strtoul(optarg, NULL, 10);
			num_lines++;
			break;
		case 'b':
			debounce_period_us = strtoul(optarg, NULL, 10);
			break;
		case 'd':
			config.flags |= GPIO_V2_LINE_FLAG_OPEN_DRAIN;
			break;
		case 's':
			config.flags |= GPIO_V2_LINE_FLAG_OPEN_SOURCE;
			break;
		case 'r':
			config.flags |= GPIO_V2_LINE_FLAG_EDGE_RISING;
			break;
		case 'f':
			config.flags |= GPIO_V2_LINE_FLAG_EDGE_FALLING;
			break;
		case 'w':
			config.flags |= GPIO_V2_LINE_FLAG_EVENT_CLOCK_REALTIME;
>>>>>>> upstream/android-13
			break;
		case '?':
			print_usage();
			return -1;
		}
	}

<<<<<<< HEAD
	if (!device_name || line == -1) {
		print_usage();
		return -1;
	}
	if (!eventflags) {
		printf("No flags specified, listening on both rising and "
		       "falling edges\n");
		eventflags = GPIOEVENT_REQUEST_BOTH_EDGES;
	}
	return monitor_device(device_name, line, handleflags,
			      eventflags, loops);
=======
	if (debounce_period_us) {
		attr = config.num_attrs;
		config.num_attrs++;
		for (i = 0; i < num_lines; i++)
			gpiotools_set_bit(&config.attrs[attr].mask, i);
		config.attrs[attr].attr.id = GPIO_V2_LINE_ATTR_ID_DEBOUNCE;
		config.attrs[attr].attr.debounce_period_us = debounce_period_us;
	}

	if (!device_name || num_lines == 0) {
		print_usage();
		return -1;
	}
	if (!(config.flags & EDGE_FLAGS)) {
		printf("No flags specified, listening on both rising and "
		       "falling edges\n");
		config.flags |= EDGE_FLAGS;
	}
	return monitor_device(device_name, lines, num_lines, &config, loops);
>>>>>>> upstream/android-13
}
