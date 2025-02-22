/*
 * Support for virtual IRQ subgroups debugfs mapping.
 *
 * Copyright (C) 2010  Paul Mundt
 *
 * Modelled after arch/powerpc/kernel/irq.c.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/debugfs.h>
#include "internals.h"

<<<<<<< HEAD
static int intc_irq_xlate_debug(struct seq_file *m, void *priv)
=======
static int intc_irq_xlate_show(struct seq_file *m, void *priv)
>>>>>>> upstream/android-13
{
	int i;

	seq_printf(m, "%-5s  %-7s  %-15s\n", "irq", "enum", "chip name");

	for (i = 1; i < nr_irqs; i++) {
		struct intc_map_entry *entry = intc_irq_xlate_get(i);
		struct intc_desc_int *desc = entry->desc;

		if (!desc)
			continue;

		seq_printf(m, "%5d  ", i);
		seq_printf(m, "0x%05x  ", entry->enum_id);
		seq_printf(m, "%-15s\n", desc->chip.name);
	}

	return 0;
}

<<<<<<< HEAD
static int intc_irq_xlate_open(struct inode *inode, struct file *file)
{
	return single_open(file, intc_irq_xlate_debug, inode->i_private);
}

static const struct file_operations intc_irq_xlate_fops = {
	.open = intc_irq_xlate_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};
=======
DEFINE_SHOW_ATTRIBUTE(intc_irq_xlate);
>>>>>>> upstream/android-13

static int __init intc_irq_xlate_init(void)
{
	/*
	 * XXX.. use arch_debugfs_dir here when all of the intc users are
	 * converted.
	 */
	if (debugfs_create_file("intc_irq_xlate", S_IRUGO, NULL, NULL,
				&intc_irq_xlate_fops) == NULL)
		return -ENOMEM;

	return 0;
}
fs_initcall(intc_irq_xlate_init);
