<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Copyright 1998-2009 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2008 S3 Graphics, Inc. All Rights Reserved.

<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTIES OR REPRESENTATIONS; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.See the GNU General Public License
 * for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
=======
>>>>>>> upstream/android-13
 */
#ifndef __VIA_I2C_H__
#define __VIA_I2C_H__

#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>

struct via_i2c_stuff {
	u16 i2c_port;			/* GPIO or I2C port */
	u16 is_active;			/* Being used as I2C? */
	struct i2c_adapter adapter;
	struct i2c_algo_bit_data algo;
};


int viafb_i2c_readbyte(u8 adap, u8 slave_addr, u8 index, u8 *pdata);
int viafb_i2c_writebyte(u8 adap, u8 slave_addr, u8 index, u8 data);
int viafb_i2c_readbytes(u8 adap, u8 slave_addr, u8 index, u8 *buff, int buff_len);
struct i2c_adapter *viafb_find_i2c_adapter(enum viafb_i2c_adap which);

extern int viafb_i2c_init(void);
extern void viafb_i2c_exit(void);
#endif /* __VIA_I2C_H__ */
