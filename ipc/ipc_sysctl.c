<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  Copyright (C) 2007
 *
 *  Author: Eric Biederman <ebiederm@xmision.com>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, version 2 of the
 *  License.
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/ipc.h>
#include <linux/nsproxy.h>
#include <linux/sysctl.h>
#include <linux/uaccess.h>
#include <linux/ipc_namespace.h>
#include <linux/msg.h>
#include "util.h"

static void *get_ipc(struct ctl_table *table)
{
	char *which = table->data;
	struct ipc_namespace *ipc_ns = current->nsproxy->ipc_ns;
	which = (which - (char *)&init_ipc_ns) + (char *)ipc_ns;
	return which;
}

#ifdef CONFIG_PROC_SYSCTL
static int proc_ipc_dointvec(struct ctl_table *table, int write,
<<<<<<< HEAD
	void __user *buffer, size_t *lenp, loff_t *ppos)
=======
		void *buffer, size_t *lenp, loff_t *ppos)
>>>>>>> upstream/android-13
{
	struct ctl_table ipc_table;

	memcpy(&ipc_table, table, sizeof(ipc_table));
	ipc_table.data = get_ipc(table);

	return proc_dointvec(&ipc_table, write, buffer, lenp, ppos);
}

static int proc_ipc_dointvec_minmax(struct ctl_table *table, int write,
<<<<<<< HEAD
	void __user *buffer, size_t *lenp, loff_t *ppos)
=======
		void *buffer, size_t *lenp, loff_t *ppos)
>>>>>>> upstream/android-13
{
	struct ctl_table ipc_table;

	memcpy(&ipc_table, table, sizeof(ipc_table));
	ipc_table.data = get_ipc(table);

	return proc_dointvec_minmax(&ipc_table, write, buffer, lenp, ppos);
}

static int proc_ipc_dointvec_minmax_orphans(struct ctl_table *table, int write,
<<<<<<< HEAD
	void __user *buffer, size_t *lenp, loff_t *ppos)
=======
		void *buffer, size_t *lenp, loff_t *ppos)
>>>>>>> upstream/android-13
{
	struct ipc_namespace *ns = current->nsproxy->ipc_ns;
	int err = proc_ipc_dointvec_minmax(table, write, buffer, lenp, ppos);

	if (err < 0)
		return err;
	if (ns->shm_rmid_forced)
		shm_destroy_orphaned(ns);
	return err;
}

static int proc_ipc_doulongvec_minmax(struct ctl_table *table, int write,
<<<<<<< HEAD
	void __user *buffer, size_t *lenp, loff_t *ppos)
=======
		void *buffer, size_t *lenp, loff_t *ppos)
>>>>>>> upstream/android-13
{
	struct ctl_table ipc_table;
	memcpy(&ipc_table, table, sizeof(ipc_table));
	ipc_table.data = get_ipc(table);

	return proc_doulongvec_minmax(&ipc_table, write, buffer,
					lenp, ppos);
}

static int proc_ipc_auto_msgmni(struct ctl_table *table, int write,
<<<<<<< HEAD
	void __user *buffer, size_t *lenp, loff_t *ppos)
=======
		void *buffer, size_t *lenp, loff_t *ppos)
>>>>>>> upstream/android-13
{
	struct ctl_table ipc_table;
	int dummy = 0;

	memcpy(&ipc_table, table, sizeof(ipc_table));
	ipc_table.data = &dummy;

	if (write)
		pr_info_once("writing to auto_msgmni has no effect");

	return proc_dointvec_minmax(&ipc_table, write, buffer, lenp, ppos);
}

<<<<<<< HEAD
=======
static int proc_ipc_sem_dointvec(struct ctl_table *table, int write,
	void *buffer, size_t *lenp, loff_t *ppos)
{
	int ret, semmni;
	struct ipc_namespace *ns = current->nsproxy->ipc_ns;

	semmni = ns->sem_ctls[3];
	ret = proc_ipc_dointvec(table, write, buffer, lenp, ppos);

	if (!ret)
		ret = sem_check_semmni(current->nsproxy->ipc_ns);

	/*
	 * Reset the semmni value if an error happens.
	 */
	if (ret)
		ns->sem_ctls[3] = semmni;
	return ret;
}

>>>>>>> upstream/android-13
#else
#define proc_ipc_doulongvec_minmax NULL
#define proc_ipc_dointvec	   NULL
#define proc_ipc_dointvec_minmax   NULL
#define proc_ipc_dointvec_minmax_orphans   NULL
#define proc_ipc_auto_msgmni	   NULL
<<<<<<< HEAD
#endif

static int zero;
static int one = 1;
static int int_max = INT_MAX;
=======
#define proc_ipc_sem_dointvec	   NULL
#endif

int ipc_mni = IPCMNI;
int ipc_mni_shift = IPCMNI_SHIFT;
int ipc_min_cycle = RADIX_TREE_MAP_SIZE;
>>>>>>> upstream/android-13

static struct ctl_table ipc_kern_table[] = {
	{
		.procname	= "shmmax",
		.data		= &init_ipc_ns.shm_ctlmax,
		.maxlen		= sizeof(init_ipc_ns.shm_ctlmax),
		.mode		= 0644,
		.proc_handler	= proc_ipc_doulongvec_minmax,
	},
	{
		.procname	= "shmall",
		.data		= &init_ipc_ns.shm_ctlall,
		.maxlen		= sizeof(init_ipc_ns.shm_ctlall),
		.mode		= 0644,
		.proc_handler	= proc_ipc_doulongvec_minmax,
	},
	{
		.procname	= "shmmni",
		.data		= &init_ipc_ns.shm_ctlmni,
		.maxlen		= sizeof(init_ipc_ns.shm_ctlmni),
		.mode		= 0644,
<<<<<<< HEAD
		.proc_handler	= proc_ipc_dointvec,
=======
		.proc_handler	= proc_ipc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
		.extra2		= &ipc_mni,
>>>>>>> upstream/android-13
	},
	{
		.procname	= "shm_rmid_forced",
		.data		= &init_ipc_ns.shm_rmid_forced,
		.maxlen		= sizeof(init_ipc_ns.shm_rmid_forced),
		.mode		= 0644,
		.proc_handler	= proc_ipc_dointvec_minmax_orphans,
<<<<<<< HEAD
		.extra1		= &zero,
		.extra2		= &one,
=======
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_ONE,
>>>>>>> upstream/android-13
	},
	{
		.procname	= "msgmax",
		.data		= &init_ipc_ns.msg_ctlmax,
		.maxlen		= sizeof(init_ipc_ns.msg_ctlmax),
		.mode		= 0644,
		.proc_handler	= proc_ipc_dointvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
		.extra2		= &int_max,
=======
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_INT_MAX,
>>>>>>> upstream/android-13
	},
	{
		.procname	= "msgmni",
		.data		= &init_ipc_ns.msg_ctlmni,
		.maxlen		= sizeof(init_ipc_ns.msg_ctlmni),
		.mode		= 0644,
		.proc_handler	= proc_ipc_dointvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
		.extra2		= &int_max,
=======
		.extra1		= SYSCTL_ZERO,
		.extra2		= &ipc_mni,
>>>>>>> upstream/android-13
	},
	{
		.procname	= "auto_msgmni",
		.data		= NULL,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_ipc_auto_msgmni,
<<<<<<< HEAD
		.extra1		= &zero,
		.extra2		= &one,
=======
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_ONE,
>>>>>>> upstream/android-13
	},
	{
		.procname	=  "msgmnb",
		.data		= &init_ipc_ns.msg_ctlmnb,
		.maxlen		= sizeof(init_ipc_ns.msg_ctlmnb),
		.mode		= 0644,
		.proc_handler	= proc_ipc_dointvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
		.extra2		= &int_max,
=======
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_INT_MAX,
>>>>>>> upstream/android-13
	},
	{
		.procname	= "sem",
		.data		= &init_ipc_ns.sem_ctls,
		.maxlen		= 4*sizeof(int),
		.mode		= 0644,
<<<<<<< HEAD
		.proc_handler	= proc_ipc_dointvec,
=======
		.proc_handler	= proc_ipc_sem_dointvec,
>>>>>>> upstream/android-13
	},
#ifdef CONFIG_CHECKPOINT_RESTORE
	{
		.procname	= "sem_next_id",
		.data		= &init_ipc_ns.ids[IPC_SEM_IDS].next_id,
		.maxlen		= sizeof(init_ipc_ns.ids[IPC_SEM_IDS].next_id),
		.mode		= 0644,
		.proc_handler	= proc_ipc_dointvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
		.extra2		= &int_max,
=======
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_INT_MAX,
>>>>>>> upstream/android-13
	},
	{
		.procname	= "msg_next_id",
		.data		= &init_ipc_ns.ids[IPC_MSG_IDS].next_id,
		.maxlen		= sizeof(init_ipc_ns.ids[IPC_MSG_IDS].next_id),
		.mode		= 0644,
		.proc_handler	= proc_ipc_dointvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
		.extra2		= &int_max,
=======
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_INT_MAX,
>>>>>>> upstream/android-13
	},
	{
		.procname	= "shm_next_id",
		.data		= &init_ipc_ns.ids[IPC_SHM_IDS].next_id,
		.maxlen		= sizeof(init_ipc_ns.ids[IPC_SHM_IDS].next_id),
		.mode		= 0644,
		.proc_handler	= proc_ipc_dointvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
		.extra2		= &int_max,
=======
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_INT_MAX,
>>>>>>> upstream/android-13
	},
#endif
	{}
};

static struct ctl_table ipc_root_table[] = {
	{
		.procname	= "kernel",
		.mode		= 0555,
		.child		= ipc_kern_table,
	},
	{}
};

static int __init ipc_sysctl_init(void)
{
	register_sysctl_table(ipc_root_table);
	return 0;
}

device_initcall(ipc_sysctl_init);
<<<<<<< HEAD
=======

static int __init ipc_mni_extend(char *str)
{
	ipc_mni = IPCMNI_EXTEND;
	ipc_mni_shift = IPCMNI_EXTEND_SHIFT;
	ipc_min_cycle = IPCMNI_EXTEND_MIN_CYCLE;
	pr_info("IPCMNI extended to %d.\n", ipc_mni);
	return 0;
}
early_param("ipcmni_extend", ipc_mni_extend);
>>>>>>> upstream/android-13
