<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * cx88-vp3054-i2c.h  --  support for the secondary I2C bus of the
 *			  DNTV Live! DVB-T Pro (VP-3054), wired as:
 *			  GPIO[0] -> SCL, GPIO[1] -> SDA
 *
 * (c) 2005 Chris Pascoe <c.pascoe@itee.uq.edu.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

/* ----------------------------------------------------------------------- */
struct vp3054_i2c_state {
	struct i2c_adapter         adap;
	struct i2c_algo_bit_data   algo;
	u32                        state;
};

/* ----------------------------------------------------------------------- */
#if IS_ENABLED(CONFIG_VIDEO_CX88_VP3054)
int  vp3054_i2c_probe(struct cx8802_dev *dev);
void vp3054_i2c_remove(struct cx8802_dev *dev);
#else
static inline int  vp3054_i2c_probe(struct cx8802_dev *dev)
{ return 0; }
static inline void vp3054_i2c_remove(struct cx8802_dev *dev)
{ }
#endif
