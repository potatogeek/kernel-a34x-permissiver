<<<<<<< HEAD
/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
>>>>>>> upstream/android-13
 *
 * Copyright (C) 2010 John Crispin <john@phrozen.org>
 */

#ifndef _LTQ_PROM_H__
#define _LTQ_PROM_H__

#define LTQ_SYS_TYPE_LEN	0x100
#define LTQ_SYS_REV_LEN		0x10

struct ltq_soc_info {
	unsigned char *name;
	unsigned int rev;
	unsigned char rev_type[LTQ_SYS_REV_LEN];
	unsigned int srev;
	unsigned int partnum;
	unsigned int type;
	unsigned char sys_type[LTQ_SYS_TYPE_LEN];
	unsigned char *compatible;
};

extern void ltq_soc_detect(struct ltq_soc_info *i);
extern void ltq_soc_init(void);

#endif
