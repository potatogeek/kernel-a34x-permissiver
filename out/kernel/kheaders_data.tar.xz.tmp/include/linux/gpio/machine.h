/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_GPIO_MACHINE_H
#define __LINUX_GPIO_MACHINE_H

#include <linux/types.h>
#include <linux/list.h>

enum gpio_lookup_flags {
	GPIO_ACTIVE_HIGH = (0 << 0),
	GPIO_ACTIVE_LOW = (1 << 0),
	GPIO_OPEN_DRAIN = (1 << 1),
	GPIO_OPEN_SOURCE = (1 << 2),
	GPIO_PERSISTENT = (0 << 3),
	GPIO_TRANSITORY = (1 << 3),
};


struct gpiod_lookup {
	const char *chip_label;
	u16 chip_hwnum;
	const char *con_id;
	unsigned int idx;
	enum gpio_lookup_flags flags;
};

struct gpiod_lookup_table {
	struct list_head list;
	const char *dev_id;
	struct gpiod_lookup table[];
};


struct gpiod_hog {
	struct list_head list;
	const char *chip_label;
	u16 chip_hwnum;
	const char *line_name;
	enum gpio_lookup_flags lflags;
	int dflags;
};


#define GPIO_LOOKUP(_chip_label, _chip_hwnum, _con_id, _flags) \
	GPIO_LOOKUP_IDX(_chip_label, _chip_hwnum, _con_id, 0, _flags)


#define GPIO_LOOKUP_IDX(_chip_label, _chip_hwnum, _con_id, _idx, _flags)  \
{                                                                         \
	.chip_label = _chip_label,                                        \
	.chip_hwnum = _chip_hwnum,                                        \
	.con_id = _con_id,                                                \
	.idx = _idx,                                                      \
	.flags = _flags,                                                  \
}


#define GPIO_HOG(_chip_label, _chip_hwnum, _line_name, _lflags, _dflags)  \
{                                                                         \
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
#else
static inline
void gpiod_add_lookup_table(struct gpiod_lookup_table *table) {}
static inline
void gpiod_add_lookup_tables(struct gpiod_lookup_table **tables, size_t n) {}
static inline
void gpiod_remove_lookup_table(struct gpiod_lookup_table *table) {}
static inline void gpiod_add_hogs(struct gpiod_hog *hogs) {}
#endif

#endif 
