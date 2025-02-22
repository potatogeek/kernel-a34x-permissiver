<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/******************************************************************************
 * rtl871x_eeprom.c
 *
 * Copyright(c) 2007 - 2010 Realtek Corporation. All rights reserved.
 * Linux device driver for RTL8192SU
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
=======
>>>>>>> upstream/android-13
 * Modifications for inclusion into the Linux staging tree are
 * Copyright(c) 2010 Larry Finger. All rights reserved.
 *
 * Contact information:
 * WLAN FAE <wlanfae@realtek.com>
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 ******************************************************************************/

#define _RTL871X_EEPROM_C_

#include "osdep_service.h"
#include "drv_types.h"

static void up_clk(struct _adapter *padapter, u16 *x)
{
	*x = *x | _EESK;
	r8712_write8(padapter, EE_9346CR, (u8)*x);
	udelay(CLOCK_RATE);
}

static void down_clk(struct _adapter *padapter, u16 *x)
{
	*x = *x & ~_EESK;
	r8712_write8(padapter, EE_9346CR, (u8)*x);
	udelay(CLOCK_RATE);
}

static void shift_out_bits(struct _adapter *padapter, u16 data, u16 count)
{
	u16 x, mask;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved)
=======
	if (padapter->surprise_removed)
>>>>>>> upstream/android-13
		goto out;
	mask = 0x01 << (count - 1);
	x = r8712_read8(padapter, EE_9346CR);
	x &= ~(_EEDO | _EEDI);
	do {
		x &= ~_EEDI;
		if (data & mask)
			x |= _EEDI;
<<<<<<< HEAD
		if (padapter->bSurpriseRemoved)
=======
		if (padapter->surprise_removed)
>>>>>>> upstream/android-13
			goto out;
		r8712_write8(padapter, EE_9346CR, (u8)x);
		udelay(CLOCK_RATE);
		up_clk(padapter, &x);
		down_clk(padapter, &x);
		mask >>= 1;
	} while (mask);
<<<<<<< HEAD
	if (padapter->bSurpriseRemoved)
=======
	if (padapter->surprise_removed)
>>>>>>> upstream/android-13
		goto out;
	x &= ~_EEDI;
	r8712_write8(padapter, EE_9346CR, (u8)x);
out:;
}

static u16 shift_in_bits(struct _adapter *padapter)
{
	u16 x, d = 0, i;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved)
=======
	if (padapter->surprise_removed)
>>>>>>> upstream/android-13
		goto out;
	x = r8712_read8(padapter, EE_9346CR);
	x &= ~(_EEDO | _EEDI);
	d = 0;
	for (i = 0; i < 16; i++) {
		d <<= 1;
		up_clk(padapter, &x);
<<<<<<< HEAD
		if (padapter->bSurpriseRemoved)
=======
		if (padapter->surprise_removed)
>>>>>>> upstream/android-13
			goto out;
		x = r8712_read8(padapter, EE_9346CR);
		x &= ~(_EEDI);
		if (x & _EEDO)
			d |= 1;
		down_clk(padapter, &x);
	}
out:
	return d;
}

static void standby(struct _adapter *padapter)
{
	u8   x;

	x = r8712_read8(padapter, EE_9346CR);
	x &= ~(_EECS | _EESK);
	r8712_write8(padapter, EE_9346CR, x);
	udelay(CLOCK_RATE);
	x |= _EECS;
	r8712_write8(padapter, EE_9346CR, x);
	udelay(CLOCK_RATE);
}

static u16 wait_eeprom_cmd_done(struct _adapter *padapter)
{
	u8	x;
	u16	i;

	standby(padapter);
	for (i = 0; i < 200; i++) {
		x = r8712_read8(padapter, EE_9346CR);
		if (x & _EEDO)
			return true;
		udelay(CLOCK_RATE);
	}
	return false;
}

static void eeprom_clean(struct _adapter *padapter)
{
	u16 x;

<<<<<<< HEAD
	if (padapter->bSurpriseRemoved)
		return;
	x = r8712_read8(padapter, EE_9346CR);
	if (padapter->bSurpriseRemoved)
		return;
	x &= ~(_EECS | _EEDI);
	r8712_write8(padapter, EE_9346CR, (u8)x);
	if (padapter->bSurpriseRemoved)
		return;
	up_clk(padapter, &x);
	if (padapter->bSurpriseRemoved)
=======
	if (padapter->surprise_removed)
		return;
	x = r8712_read8(padapter, EE_9346CR);
	if (padapter->surprise_removed)
		return;
	x &= ~(_EECS | _EEDI);
	r8712_write8(padapter, EE_9346CR, (u8)x);
	if (padapter->surprise_removed)
		return;
	up_clk(padapter, &x);
	if (padapter->surprise_removed)
>>>>>>> upstream/android-13
		return;
	down_clk(padapter, &x);
}

void r8712_eeprom_write16(struct _adapter *padapter, u16 reg, u16 data)
{
	u8 x;
	u8 tmp8_ori, tmp8_new, tmp8_clk_ori, tmp8_clk_new;

	tmp8_ori = r8712_read8(padapter, 0x102502f1);
	tmp8_new = tmp8_ori & 0xf7;
	if (tmp8_ori != tmp8_new)
		r8712_write8(padapter, 0x102502f1, tmp8_new);
	tmp8_clk_ori = r8712_read8(padapter, 0x10250003);
	tmp8_clk_new = tmp8_clk_ori | 0x20;
	if (tmp8_clk_new != tmp8_clk_ori)
		r8712_write8(padapter, 0x10250003, tmp8_clk_new);
	x = r8712_read8(padapter, EE_9346CR);
	x &= ~(_EEDI | _EEDO | _EESK | _EEM0);
	x |= _EEM1 | _EECS;
	r8712_write8(padapter, EE_9346CR, x);
	shift_out_bits(padapter, EEPROM_EWEN_OPCODE, 5);
<<<<<<< HEAD
	if (padapter->EepromAddressSize == 8)	/*CF+ and SDIO*/
=======
	if (padapter->eeprom_address_size == 8)	/*CF+ and SDIO*/
>>>>>>> upstream/android-13
		shift_out_bits(padapter, 0, 6);
	else	/* USB */
		shift_out_bits(padapter, 0, 4);
	standby(padapter);
	/* Erase this particular word.  Write the erase opcode and register
	 * number in that order. The opcode is 3bits in length; reg is 6
	 * bits long.
	 */
	standby(padapter);
	/* write the new word to the EEPROM
	 * send the write opcode the EEPORM
	 */
	shift_out_bits(padapter, EEPROM_WRITE_OPCODE, 3);
	/* select which word in the EEPROM that we are writing to. */
<<<<<<< HEAD
	shift_out_bits(padapter, reg, padapter->EepromAddressSize);
=======
	shift_out_bits(padapter, reg, padapter->eeprom_address_size);
>>>>>>> upstream/android-13
	/* write the data to the selected EEPROM word. */
	shift_out_bits(padapter, data, 16);
	if (wait_eeprom_cmd_done(padapter)) {
		standby(padapter);
		shift_out_bits(padapter, EEPROM_EWDS_OPCODE, 5);
		shift_out_bits(padapter, reg, 4);
		eeprom_clean(padapter);
	}
	if (tmp8_clk_new != tmp8_clk_ori)
		r8712_write8(padapter, 0x10250003, tmp8_clk_ori);
	if (tmp8_new != tmp8_ori)
		r8712_write8(padapter, 0x102502f1, tmp8_ori);
}

u16 r8712_eeprom_read16(struct _adapter *padapter, u16 reg) /*ReadEEprom*/
{
	u16 x;
	u16 data = 0;
	u8 tmp8_ori, tmp8_new, tmp8_clk_ori, tmp8_clk_new;

	tmp8_ori = r8712_read8(padapter, 0x102502f1);
	tmp8_new = tmp8_ori & 0xf7;
	if (tmp8_ori != tmp8_new)
		r8712_write8(padapter, 0x102502f1, tmp8_new);
	tmp8_clk_ori = r8712_read8(padapter, 0x10250003);
	tmp8_clk_new = tmp8_clk_ori | 0x20;
	if (tmp8_clk_new != tmp8_clk_ori)
		r8712_write8(padapter, 0x10250003, tmp8_clk_new);
<<<<<<< HEAD
	if (padapter->bSurpriseRemoved)
		goto out;
	/* select EEPROM, reset bits, set _EECS */
	x = r8712_read8(padapter, EE_9346CR);
	if (padapter->bSurpriseRemoved)
=======
	if (padapter->surprise_removed)
		goto out;
	/* select EEPROM, reset bits, set _EECS */
	x = r8712_read8(padapter, EE_9346CR);
	if (padapter->surprise_removed)
>>>>>>> upstream/android-13
		goto out;
	x &= ~(_EEDI | _EEDO | _EESK | _EEM0);
	x |= _EEM1 | _EECS;
	r8712_write8(padapter, EE_9346CR, (unsigned char)x);
	/* write the read opcode and register number in that order
	 * The opcode is 3bits in length, reg is 6 bits long
	 */
	shift_out_bits(padapter, EEPROM_READ_OPCODE, 3);
<<<<<<< HEAD
	shift_out_bits(padapter, reg, padapter->EepromAddressSize);
=======
	shift_out_bits(padapter, reg, padapter->eeprom_address_size);
>>>>>>> upstream/android-13
	/* Now read the data (16 bits) in from the selected EEPROM word */
	data = shift_in_bits(padapter);
	eeprom_clean(padapter);
out:
	if (tmp8_clk_new != tmp8_clk_ori)
		r8712_write8(padapter, 0x10250003, tmp8_clk_ori);
	if (tmp8_new != tmp8_ori)
		r8712_write8(padapter, 0x102502f1, tmp8_ori);
	return data;
}
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
