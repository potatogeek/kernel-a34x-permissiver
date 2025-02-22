<<<<<<< HEAD
/*
 * Copyright (C) 2014 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2014 ARM Ltd.
>>>>>>> upstream/android-13
 */
#ifndef __ASM_PTDUMP_H
#define __ASM_PTDUMP_H

<<<<<<< HEAD
#ifdef CONFIG_ARM64_PTDUMP_CORE
=======
#ifdef CONFIG_PTDUMP_CORE
>>>>>>> upstream/android-13

#include <linux/mm_types.h>
#include <linux/seq_file.h>

struct addr_marker {
	unsigned long start_address;
	char *name;
};

struct ptdump_info {
	struct mm_struct		*mm;
	const struct addr_marker	*markers;
	unsigned long			base_addr;
};

<<<<<<< HEAD
void ptdump_walk_pgd(struct seq_file *s, struct ptdump_info *info);
#ifdef CONFIG_ARM64_PTDUMP_DEBUGFS
int ptdump_debugfs_register(struct ptdump_info *info, const char *name);
#else
static inline int ptdump_debugfs_register(struct ptdump_info *info,
					const char *name)
{
	return 0;
}
#endif
void ptdump_check_wx(void);
#endif /* CONFIG_ARM64_PTDUMP_CORE */
=======
void ptdump_walk(struct seq_file *s, struct ptdump_info *info);
#ifdef CONFIG_PTDUMP_DEBUGFS
void __init ptdump_debugfs_register(struct ptdump_info *info, const char *name);
#else
static inline void ptdump_debugfs_register(struct ptdump_info *info,
					   const char *name) { }
#endif
void ptdump_check_wx(void);
#endif /* CONFIG_PTDUMP_CORE */
>>>>>>> upstream/android-13

#ifdef CONFIG_DEBUG_WX
#define debug_checkwx()	ptdump_check_wx()
#else
#define debug_checkwx()	do { } while (0)
#endif

#endif /* __ASM_PTDUMP_H */
