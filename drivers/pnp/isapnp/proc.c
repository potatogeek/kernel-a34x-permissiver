<<<<<<< HEAD
/*
 *  ISA Plug & Play support
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  ISA Plug & Play support
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/isapnp.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>

extern struct pnp_protocol isapnp_protocol;

static struct proc_dir_entry *isapnp_proc_bus_dir = NULL;

static loff_t isapnp_proc_bus_lseek(struct file *file, loff_t off, int whence)
{
	return fixed_size_llseek(file, off, whence, 256);
}

static ssize_t isapnp_proc_bus_read(struct file *file, char __user * buf,
				    size_t nbytes, loff_t * ppos)
{
	struct pnp_dev *dev = PDE_DATA(file_inode(file));
	int pos = *ppos;
	int cnt, size = 256;

	if (pos >= size)
		return 0;
	if (nbytes >= size)
		nbytes = size;
	if (pos + nbytes > size)
		nbytes = size - pos;
	cnt = nbytes;

<<<<<<< HEAD
	if (!access_ok(VERIFY_WRITE, buf, cnt))
=======
	if (!access_ok(buf, cnt))
>>>>>>> upstream/android-13
		return -EINVAL;

	isapnp_cfg_begin(dev->card->number, dev->number);
	for (; pos < 256 && cnt > 0; pos++, buf++, cnt--) {
		unsigned char val;
		val = isapnp_read_byte(pos);
		__put_user(val, buf);
	}
	isapnp_cfg_end();

	*ppos = pos;
	return nbytes;
}

<<<<<<< HEAD
static const struct file_operations isapnp_proc_bus_file_operations = {
	.owner	= THIS_MODULE,
	.llseek = isapnp_proc_bus_lseek,
	.read = isapnp_proc_bus_read,
=======
static const struct proc_ops isapnp_proc_bus_proc_ops = {
	.proc_lseek	= isapnp_proc_bus_lseek,
	.proc_read	= isapnp_proc_bus_read,
>>>>>>> upstream/android-13
};

static int isapnp_proc_attach_device(struct pnp_dev *dev)
{
	struct pnp_card *bus = dev->card;
<<<<<<< HEAD
	struct proc_dir_entry *de, *e;
	char name[16];

	if (!(de = bus->procdir)) {
		sprintf(name, "%02x", bus->number);
		de = bus->procdir = proc_mkdir(name, isapnp_proc_bus_dir);
		if (!de)
			return -ENOMEM;
	}
	sprintf(name, "%02x", dev->number);
	e = dev->procent = proc_create_data(name, S_IFREG | S_IRUGO, de,
			&isapnp_proc_bus_file_operations, dev);
	if (!e)
		return -ENOMEM;
	proc_set_size(e, 256);
=======
	char name[16];

	if (!bus->procdir) {
		sprintf(name, "%02x", bus->number);
		bus->procdir = proc_mkdir(name, isapnp_proc_bus_dir);
		if (!bus->procdir)
			return -ENOMEM;
	}
	sprintf(name, "%02x", dev->number);
	dev->procent = proc_create_data(name, S_IFREG | S_IRUGO, bus->procdir,
					    &isapnp_proc_bus_proc_ops, dev);
	if (!dev->procent)
		return -ENOMEM;
	proc_set_size(dev->procent, 256);
>>>>>>> upstream/android-13
	return 0;
}

int __init isapnp_proc_init(void)
{
	struct pnp_dev *dev;

	isapnp_proc_bus_dir = proc_mkdir("bus/isapnp", NULL);
	protocol_for_each_dev(&isapnp_protocol, dev) {
		isapnp_proc_attach_device(dev);
	}
	return 0;
}
