<<<<<<< HEAD
/*
 *   Copyright (C) International Business Machines Corp., 2000-2002
 *   Portions Copyright (C) Christoph Hellwig, 2001-2002
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 *   Copyright (C) International Business Machines Corp., 2000-2002
 *   Portions Copyright (C) Christoph Hellwig, 2001-2002
>>>>>>> upstream/android-13
 */
#ifndef _H_JFS_DEBUG
#define _H_JFS_DEBUG

/*
 *	jfs_debug.h
 *
 * global debug message, data structure/macro definitions
 * under control of CONFIG_JFS_DEBUG, CONFIG_JFS_STATISTICS;
 */

/*
 * Create /proc/fs/jfs if procfs is enabled andeither
 * CONFIG_JFS_DEBUG or CONFIG_JFS_STATISTICS is defined
 */
#if defined(CONFIG_PROC_FS) && (defined(CONFIG_JFS_DEBUG) || defined(CONFIG_JFS_STATISTICS))
#define PROC_FS_JFS
extern void jfs_proc_init(void);
extern void jfs_proc_clean(void);
#endif

/*
 *	assert with traditional printf/panic
 */
#define assert(p) do {	\
	if (!(p)) {	\
		printk(KERN_CRIT "BUG at %s:%d assert(%s)\n",	\
		       __FILE__, __LINE__, #p);			\
		BUG();	\
	}		\
} while (0)

/*
 *	debug ON
 *	--------
 */
#ifdef CONFIG_JFS_DEBUG
#define ASSERT(p) assert(p)

/* printk verbosity */
#define JFS_LOGLEVEL_ERR 1
#define JFS_LOGLEVEL_WARN 2
#define JFS_LOGLEVEL_DEBUG 3
#define JFS_LOGLEVEL_INFO 4

extern int jfsloglevel;

int jfs_txanchor_proc_show(struct seq_file *m, void *v);

/* information message: e.g., configuration, major event */
#define jfs_info(fmt, arg...) do {			\
	if (jfsloglevel >= JFS_LOGLEVEL_INFO)		\
		printk(KERN_INFO fmt "\n", ## arg);	\
} while (0)

/* debug message: ad hoc */
#define jfs_debug(fmt, arg...) do {			\
	if (jfsloglevel >= JFS_LOGLEVEL_DEBUG)		\
		printk(KERN_DEBUG fmt "\n", ## arg);	\
} while (0)

/* warn message: */
#define jfs_warn(fmt, arg...) do {			\
	if (jfsloglevel >= JFS_LOGLEVEL_WARN)		\
		printk(KERN_WARNING fmt "\n", ## arg);	\
} while (0)

/* error event message: e.g., i/o error */
#define jfs_err(fmt, arg...) do {			\
	if (jfsloglevel >= JFS_LOGLEVEL_ERR)		\
		printk(KERN_ERR fmt "\n", ## arg);	\
} while (0)

/*
 *	debug OFF
 *	---------
 */
#else				/* CONFIG_JFS_DEBUG */
#define ASSERT(p) do {} while (0)
#define jfs_info(fmt, arg...) do {} while (0)
#define jfs_debug(fmt, arg...) do {} while (0)
#define jfs_warn(fmt, arg...) do {} while (0)
#define jfs_err(fmt, arg...) do {} while (0)
#endif				/* CONFIG_JFS_DEBUG */

/*
 *	statistics
 *	----------
 */
#ifdef	CONFIG_JFS_STATISTICS
int jfs_lmstats_proc_show(struct seq_file *m, void *v);
int jfs_txstats_proc_show(struct seq_file *m, void *v);
int jfs_mpstat_proc_show(struct seq_file *m, void *v);
int jfs_xtstat_proc_show(struct seq_file *m, void *v);

#define	INCREMENT(x)		((x)++)
#define	DECREMENT(x)		((x)--)
#define	HIGHWATERMARK(x,y)	((x) = max((x), (y)))
#else
#define	INCREMENT(x)
#define	DECREMENT(x)
#define	HIGHWATERMARK(x,y)
#endif				/* CONFIG_JFS_STATISTICS */

#endif				/* _H_JFS_DEBUG */
