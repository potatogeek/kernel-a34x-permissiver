<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* FS-Cache statistics viewing interface
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#define FSCACHE_DEBUG_LEVEL OPERATION
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include "internal.h"

/*
 * initialise the /proc/fs/fscache/ directory
 */
int __init fscache_proc_init(void)
{
	_enter("");

	if (!proc_mkdir("fs/fscache", NULL))
		goto error_dir;

<<<<<<< HEAD
=======
	if (!proc_create_seq("fs/fscache/cookies", S_IFREG | 0444, NULL,
			     &fscache_cookies_seq_ops))
		goto error_cookies;

>>>>>>> upstream/android-13
#ifdef CONFIG_FSCACHE_STATS
	if (!proc_create_single("fs/fscache/stats", S_IFREG | 0444, NULL,
			fscache_stats_show))
		goto error_stats;
#endif

<<<<<<< HEAD
#ifdef CONFIG_FSCACHE_HISTOGRAM
	if (!proc_create_seq("fs/fscache/histogram", S_IFREG | 0444, NULL,
			 &fscache_histogram_ops))
		goto error_histogram;
#endif

#ifdef CONFIG_FSCACHE_OBJECT_LIST
	if (!proc_create("fs/fscache/objects", S_IFREG | 0444, NULL,
			 &fscache_objlist_fops))
=======
#ifdef CONFIG_FSCACHE_OBJECT_LIST
	if (!proc_create("fs/fscache/objects", S_IFREG | 0444, NULL,
			 &fscache_objlist_proc_ops))
>>>>>>> upstream/android-13
		goto error_objects;
#endif

	_leave(" = 0");
	return 0;

#ifdef CONFIG_FSCACHE_OBJECT_LIST
error_objects:
#endif
<<<<<<< HEAD
#ifdef CONFIG_FSCACHE_HISTOGRAM
	remove_proc_entry("fs/fscache/histogram", NULL);
error_histogram:
#endif
=======
>>>>>>> upstream/android-13
#ifdef CONFIG_FSCACHE_STATS
	remove_proc_entry("fs/fscache/stats", NULL);
error_stats:
#endif
<<<<<<< HEAD
=======
	remove_proc_entry("fs/fscache/cookies", NULL);
error_cookies:
>>>>>>> upstream/android-13
	remove_proc_entry("fs/fscache", NULL);
error_dir:
	_leave(" = -ENOMEM");
	return -ENOMEM;
}

/*
 * clean up the /proc/fs/fscache/ directory
 */
void fscache_proc_cleanup(void)
{
#ifdef CONFIG_FSCACHE_OBJECT_LIST
	remove_proc_entry("fs/fscache/objects", NULL);
#endif
<<<<<<< HEAD
#ifdef CONFIG_FSCACHE_HISTOGRAM
	remove_proc_entry("fs/fscache/histogram", NULL);
#endif
#ifdef CONFIG_FSCACHE_STATS
	remove_proc_entry("fs/fscache/stats", NULL);
#endif
=======
#ifdef CONFIG_FSCACHE_STATS
	remove_proc_entry("fs/fscache/stats", NULL);
#endif
	remove_proc_entry("fs/fscache/cookies", NULL);
>>>>>>> upstream/android-13
	remove_proc_entry("fs/fscache", NULL);
}
