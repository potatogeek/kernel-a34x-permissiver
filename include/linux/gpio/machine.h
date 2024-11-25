/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_GPIO_MACHINE_H
#define __LINUX_GPIO_MACHINE_H

#include <linux/types.h>
#include <linux/list.h>

enum gpio_lookup_flags {
<<<<<<< HEAD
	GPIO_ACTIVE_HIGH = (0 << 0),
	GPIO_ACTIVE_LOW = (1 << 0),
	GPIO_OPEN_DRAIN = (1 << 1),
	GPIO_OPEN_SOURCE = (1 << 2),
	GPIO_PERSISTENT = (0 << 3),
	GPIO_TRANSITORY = (1 << 3),
=======
	GPIO_ACTIVE_HIGH		= (0 << 0),
	GPIO_ACTIVE_LOW			= (1 << 0),
	GPIO_OPEN_DRAIN			= (1 << 1),
	GPIO_OPEN_SOURCE		= (1 << 2),
	GPIO_PERSISTENT			= (0 << 3),
	GPIO_TRANSITORY			= (1 << 3),
	GPIO_PULL_UP			= (1 << 4),
	GPIO_PULL_DOWN			= (1 << 5),

	GPIO_LOOKUP_FLAGS_DEFAULT	= GPIO_ACTIVE_HIGH | GPIO_PERSISTENT,
>>>>>>> upstream/android-13
};

/**
 * struct gpiod_lookup - lookup table
<<<<<<< HEAD
 * @chip_label: name of the chip the GPIO belongs to
 * @chip_hwnum: hardware number (i.e. relative to the chip) of the GPIO
 * @con_id: name of the GPIO from the device's point of view
 * @idx: index of the GPIO in case several GPIOs share the same name
 * @flags: mask of GPIO_* values
=======
 * @key: either the name of the chip the GPIO belongs to, or the GPIO line name
 *       Note that GPIO line names are not guaranteed to be globally unique,
 *       so this will use the first match found!
 * @chip_hwnum: hardware number (i.e. relative to the chip) of the GPIO, or
 *              U16_MAX to indicate that @key is a GPIO line name
 * @con_id: name of the GPIO from the device's point of view
 * @idx: index of the GPIO in case several GPIOs share the same name
 * @flags: bitmask of gpio_lookup_flags GPIO_* values
>>>>>>> upstream/android-13
 *
 * gpiod_lookup is a lookup table for associating GPIOs to specific devices and
 * functions using platform data.
 */
struct gpiod_lookup {
<<<<<<< HEAD
	const char *chip_label;
	u16 chip_hwnum;
	const char *con_id;
	unsigned int idx;
	enum gpio_lookup_flags flags;
=======
	const char *key;
	u16 chip_hwnum;
	const char *con_id;
	unsigned int idx;
	unsigned long flags;
>>>>>>> upstream/android-13
};

struct gpiod_lookup_table {
	struct list_head list;
	const char *dev_id;
	struct gpiod_lookup table[];
};

/**
 * struct gpiod_hog - GPIO line hog table
 * @chip_label: name of the chip the GPIO belongs to
 * @chip_hwnum: hardware number (i.e. relative to the chip) of the GPIO
 * @line_name: consumer name for the hogged line
<<<<<<< HEAD
 * @lflags: mask of GPIO lookup flags
=======
 * @lflags: bitmask of gpio_lookup_flags GPIO_* values
>>>>>>> upstream/android-13
 * @dflags: GPIO flags used to specify the direction and value
 */
struct gpiod_hog {
	struct list_head list;
	const char *chip_label;
	u16 chip_hwnum;
	const char *line_name;
<<<<<<< HEAD
	enum gpio_lookup_flags lflags;
=======
	unsigned long lflags;
>>>>>>> upstream/android-13
	int dflags;
};

/*
 * Simple definition of a single GPIO under a con_id
 */
<<<<<<< HEAD
#define GPIO_LOOKUP(_chip_label, _chip_hwnum, _con_id, _flags) \
	GPIO_LOOKUP_IDX(_chip_label, _chip_hwnum, _con_id, 0, _flags)
=======
#define GPIO_LOOKUP(_key, _chip_hwnum, _con_id, _flags) \
	GPIO_LOOKUP_IDX(_key, _chip_hwnum, _con_id, 0, _flags)
>>>>>>> upstream/android-13

/*
 * Use this macro if you need to have several GPIOs under the same con_id.
 * Each GPIO needs to use a different index and can be accessed using
 * gpiod_get_index()
 */
<<<<<<< HEAD
#define GPIO_LOOKUP_IDX(_chip_label, _chip_hwnum, _con_id, _idx, _flags)  \
{                                                                         \
	.chip_label = _chip_label,                                        \
=======
#define GPIO_LOOKUP_IDX(_key, _chip_hwnum, _con_id, _idx, _flags)         \
(struct gpiod_lookup) {                                                   \
	.key = _key,                                                      \
>>>>>>> upstream/android-13
	.chip_hwnum = _chip_hwnum,                                        \
	.con_id = _con_id,                                                \
	.idx = _idx,                                                      \
	.flags = _flags,                                                  \
}

/*
 * Simple definition of a single GPIO hog in an array.
 */
#define GPIO_HOG(_chip_label, _chip_hwnum, _line_name, _lflags, _dflags)  \
<<<<<<< HEAD
{                                                                         \
=======
(struct gpiod_hog) {                                                      \
>>>>>>> upstream/android-13
	.chip_label = _chip_label,                                        \
	.chip_hwnum = _chip_hwnum,                                        \
	.line_name = _line_name,                                          \
	.lflags = _lflags,                                                \
	.dflags = _dflags,                                                \
}

#ifdef CONFIG_GPIOLIB
void gpiod_add_lookup_table(struct gpiod_lookup_table *table);
void gpiod_add_lookup_tables(struct gpiod_lookup_table **tables, size_t n);
void gpiod_remove_lookup_table(struct gpiod_lookup_table *table);
void gpiod_add_hogs(struct gpiod_hog *hogs);
<<<<<<< HEAD
#else
=======
#else /* ! CONFIG_GPIOLIB */
>>>>>>> upstream/android-13
static inline
void gpiod_add_lookup_table(struct gpiod_lookup_table *table) {}
static inline
void gpiod_add_lookup_tables(struct gpiod_lookup_table **tables, size_t n) {}
static inline
void gpiod_remove_lookup_table(struct gpiod_lookup_table *table) {}
static inline void gpiod_add_hogs(struct gpiod_hog *hogs) {}
<<<<<<< HEAD
#endif
=======
#endif /* CONFIG_GPIOLIB */
>>>>>>> upstream/android-13

#endif /* __LINUX_GPIO_MACHINE_H */
