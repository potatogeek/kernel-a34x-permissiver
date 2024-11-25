<<<<<<< HEAD
/*
 * Copyright 2016-17 IBM Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2016-17 IBM Corp.
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) "vas: " fmt

#include <linux/types.h>
#include <linux/slab.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
<<<<<<< HEAD
=======
#include <asm/vas.h>
>>>>>>> upstream/android-13
#include "vas.h"

static struct dentry *vas_debugfs;

static char *cop_to_str(int cop)
{
	switch (cop) {
	case VAS_COP_TYPE_FAULT:	return "Fault";
	case VAS_COP_TYPE_842:		return "NX-842 Normal Priority";
	case VAS_COP_TYPE_842_HIPRI:	return "NX-842 High Priority";
	case VAS_COP_TYPE_GZIP:		return "NX-GZIP Normal Priority";
	case VAS_COP_TYPE_GZIP_HIPRI:	return "NX-GZIP High Priority";
	case VAS_COP_TYPE_FTW:		return "Fast Thread-wakeup";
	default:			return "Unknown";
	}
}

<<<<<<< HEAD
static int info_dbg_show(struct seq_file *s, void *private)
{
	struct vas_window *window = s->private;
=======
static int info_show(struct seq_file *s, void *private)
{
	struct pnv_vas_window *window = s->private;
>>>>>>> upstream/android-13

	mutex_lock(&vas_mutex);

	/* ensure window is not unmapped */
	if (!window->hvwc_map)
		goto unlock;

<<<<<<< HEAD
	seq_printf(s, "Type: %s, %s\n", cop_to_str(window->cop),
					window->tx_win ? "Send" : "Receive");
	seq_printf(s, "Pid : %d\n", window->pid);
=======
	seq_printf(s, "Type: %s, %s\n", cop_to_str(window->vas_win.cop),
					window->tx_win ? "Send" : "Receive");
	seq_printf(s, "Pid : %d\n", vas_window_pid(&window->vas_win));
>>>>>>> upstream/android-13

unlock:
	mutex_unlock(&vas_mutex);
	return 0;
}

<<<<<<< HEAD
static int info_dbg_open(struct inode *inode, struct file *file)
{
	return single_open(file, info_dbg_show, inode->i_private);
}

static const struct file_operations info_fops = {
	.open		= info_dbg_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static inline void print_reg(struct seq_file *s, struct vas_window *win,
=======
DEFINE_SHOW_ATTRIBUTE(info);

static inline void print_reg(struct seq_file *s, struct pnv_vas_window *win,
>>>>>>> upstream/android-13
			char *name, u32 reg)
{
	seq_printf(s, "0x%016llx %s\n", read_hvwc_reg(win, name, reg), name);
}

<<<<<<< HEAD
static int hvwc_dbg_show(struct seq_file *s, void *private)
{
	struct vas_window *window = s->private;
=======
static int hvwc_show(struct seq_file *s, void *private)
{
	struct pnv_vas_window *window = s->private;
>>>>>>> upstream/android-13

	mutex_lock(&vas_mutex);

	/* ensure window is not unmapped */
	if (!window->hvwc_map)
		goto unlock;

	print_reg(s, window, VREG(LPID));
	print_reg(s, window, VREG(PID));
	print_reg(s, window, VREG(XLATE_MSR));
	print_reg(s, window, VREG(XLATE_LPCR));
	print_reg(s, window, VREG(XLATE_CTL));
	print_reg(s, window, VREG(AMR));
	print_reg(s, window, VREG(SEIDR));
	print_reg(s, window, VREG(FAULT_TX_WIN));
	print_reg(s, window, VREG(OSU_INTR_SRC_RA));
	print_reg(s, window, VREG(HV_INTR_SRC_RA));
	print_reg(s, window, VREG(PSWID));
	print_reg(s, window, VREG(LFIFO_BAR));
	print_reg(s, window, VREG(LDATA_STAMP_CTL));
	print_reg(s, window, VREG(LDMA_CACHE_CTL));
	print_reg(s, window, VREG(LRFIFO_PUSH));
	print_reg(s, window, VREG(CURR_MSG_COUNT));
	print_reg(s, window, VREG(LNOTIFY_AFTER_COUNT));
	print_reg(s, window, VREG(LRX_WCRED));
	print_reg(s, window, VREG(LRX_WCRED_ADDER));
	print_reg(s, window, VREG(TX_WCRED));
	print_reg(s, window, VREG(TX_WCRED_ADDER));
	print_reg(s, window, VREG(LFIFO_SIZE));
	print_reg(s, window, VREG(WINCTL));
	print_reg(s, window, VREG(WIN_STATUS));
	print_reg(s, window, VREG(WIN_CTX_CACHING_CTL));
	print_reg(s, window, VREG(TX_RSVD_BUF_COUNT));
	print_reg(s, window, VREG(LRFIFO_WIN_PTR));
	print_reg(s, window, VREG(LNOTIFY_CTL));
	print_reg(s, window, VREG(LNOTIFY_PID));
	print_reg(s, window, VREG(LNOTIFY_LPID));
	print_reg(s, window, VREG(LNOTIFY_TID));
	print_reg(s, window, VREG(LNOTIFY_SCOPE));
	print_reg(s, window, VREG(NX_UTIL_ADDER));
unlock:
	mutex_unlock(&vas_mutex);
	return 0;
}

<<<<<<< HEAD
static int hvwc_dbg_open(struct inode *inode, struct file *file)
{
	return single_open(file, hvwc_dbg_show, inode->i_private);
}

static const struct file_operations hvwc_fops = {
	.open		= hvwc_dbg_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

void vas_window_free_dbgdir(struct vas_window *window)
{
=======
DEFINE_SHOW_ATTRIBUTE(hvwc);

void vas_window_free_dbgdir(struct pnv_vas_window *pnv_win)
{
	struct vas_window *window =  &pnv_win->vas_win;

>>>>>>> upstream/android-13
	if (window->dbgdir) {
		debugfs_remove_recursive(window->dbgdir);
		kfree(window->dbgname);
		window->dbgdir = NULL;
		window->dbgname = NULL;
	}
}

<<<<<<< HEAD
void vas_window_init_dbgdir(struct vas_window *window)
{
	struct dentry *f, *d;
=======
void vas_window_init_dbgdir(struct pnv_vas_window *window)
{
	struct dentry *d;
>>>>>>> upstream/android-13

	if (!window->vinst->dbgdir)
		return;

<<<<<<< HEAD
	window->dbgname = kzalloc(16, GFP_KERNEL);
	if (!window->dbgname)
		return;

	snprintf(window->dbgname, 16, "w%d", window->winid);

	d = debugfs_create_dir(window->dbgname, window->vinst->dbgdir);
	if (IS_ERR(d))
		goto free_name;

	window->dbgdir = d;

	f = debugfs_create_file("info", 0444, d, window, &info_fops);
	if (IS_ERR(f))
		goto remove_dir;

	f = debugfs_create_file("hvwc", 0444, d, window, &hvwc_fops);
	if (IS_ERR(f))
		goto remove_dir;

	return;

remove_dir:
	debugfs_remove_recursive(window->dbgdir);
	window->dbgdir = NULL;

free_name:
	kfree(window->dbgname);
	window->dbgname = NULL;
=======
	window->vas_win.dbgname = kzalloc(16, GFP_KERNEL);
	if (!window->vas_win.dbgname)
		return;

	snprintf(window->vas_win.dbgname, 16, "w%d", window->vas_win.winid);

	d = debugfs_create_dir(window->vas_win.dbgname, window->vinst->dbgdir);
	window->vas_win.dbgdir = d;

	debugfs_create_file("info", 0444, d, window, &info_fops);
	debugfs_create_file("hvwc", 0444, d, window, &hvwc_fops);
>>>>>>> upstream/android-13
}

void vas_instance_init_dbgdir(struct vas_instance *vinst)
{
	struct dentry *d;

	vas_init_dbgdir();
<<<<<<< HEAD
	if (!vas_debugfs)
		return;
=======
>>>>>>> upstream/android-13

	vinst->dbgname = kzalloc(16, GFP_KERNEL);
	if (!vinst->dbgname)
		return;

	snprintf(vinst->dbgname, 16, "v%d", vinst->vas_id);

	d = debugfs_create_dir(vinst->dbgname, vas_debugfs);
<<<<<<< HEAD
	if (IS_ERR(d))
		goto free_name;

	vinst->dbgdir = d;
	return;

free_name:
	kfree(vinst->dbgname);
	vinst->dbgname = NULL;
	vinst->dbgdir = NULL;
=======
	vinst->dbgdir = d;
>>>>>>> upstream/android-13
}

/*
 * Set up the "root" VAS debugfs dir. Return if we already set it up
 * (or failed to) in an earlier instance of VAS.
 */
void vas_init_dbgdir(void)
{
	static bool first_time = true;

	if (!first_time)
		return;

	first_time = false;
	vas_debugfs = debugfs_create_dir("vas", NULL);
<<<<<<< HEAD
	if (IS_ERR(vas_debugfs))
		vas_debugfs = NULL;
=======
>>>>>>> upstream/android-13
}
