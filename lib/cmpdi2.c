<<<<<<< HEAD
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
>>>>>>> upstream/android-13
 */

#include <linux/export.h>

#include <linux/libgcc.h>

word_type notrace __cmpdi2(long long a, long long b)
{
	const DWunion au = {
		.ll = a
	};
	const DWunion bu = {
		.ll = b
	};

	if (au.s.high < bu.s.high)
		return 0;
	else if (au.s.high > bu.s.high)
		return 2;

	if ((unsigned int) au.s.low < (unsigned int) bu.s.low)
		return 0;
	else if ((unsigned int) au.s.low > (unsigned int) bu.s.low)
		return 2;

	return 1;
}
EXPORT_SYMBOL(__cmpdi2);
