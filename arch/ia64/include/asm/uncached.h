<<<<<<< HEAD
/*
 * Copyright (C) 2001-2008 Silicon Graphics, Inc.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2001-2008 Silicon Graphics, Inc.  All rights reserved.
 *
>>>>>>> upstream/android-13
 * Prototypes for the uncached page allocator
 */

extern unsigned long uncached_alloc_page(int starting_nid, int n_pages);
extern void uncached_free_page(unsigned long uc_addr, int n_pages);
