<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
	Copyright (C) 2004 - 2009 Ivo van Doorn <IvDoorn@gmail.com>
	<http://rt2x00.serialmonkey.com>

<<<<<<< HEAD
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

/*
	Module: rt2x00debug
	Abstract: Data structures for the rt2x00debug.
 */

#ifndef RT2X00DEBUG_H
#define RT2X00DEBUG_H

struct rt2x00_dev;

/**
 * enum rt2x00debugfs_entry_flags: Flags for debugfs registry entry
 *
 * @RT2X00DEBUGFS_OFFSET: rt2x00lib should pass the register offset
 *	as argument when using the callback function read()/write()
 */
enum rt2x00debugfs_entry_flags {
	RT2X00DEBUGFS_OFFSET	= (1 << 0),
};

#define RT2X00DEBUGFS_REGISTER_ENTRY(__name, __type)		\
struct reg##__name {						\
	__type (*read)(struct rt2x00_dev *rt2x00dev,		\
		     const unsigned int word);			\
	void (*write)(struct rt2x00_dev *rt2x00dev,		\
		      const unsigned int word, __type data);	\
								\
	unsigned int flags;					\
								\
	unsigned int word_base;					\
	unsigned int word_size;					\
	unsigned int word_count;				\
} __name

struct rt2x00debug {
	/*
	 * Reference to the modules structure.
	 */
	struct module *owner;

	/*
	 * Register access entries.
	 */
	RT2X00DEBUGFS_REGISTER_ENTRY(csr, u32);
	RT2X00DEBUGFS_REGISTER_ENTRY(eeprom, u16);
	RT2X00DEBUGFS_REGISTER_ENTRY(bbp, u8);
	RT2X00DEBUGFS_REGISTER_ENTRY(rf, u32);
	RT2X00DEBUGFS_REGISTER_ENTRY(rfcsr, u8);
};

#endif /* RT2X00DEBUG_H */
