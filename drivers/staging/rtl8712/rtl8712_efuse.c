<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * rtl8712_efuse.c
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
 * WLAN FAE <wlanfae@realtek.com>.
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 ******************************************************************************/

#define _RTL8712_EFUSE_C_

#include "osdep_service.h"
#include "drv_types.h"
#include "rtl8712_efuse.h"

/* reserve 3 bytes for HW stop read */
static int efuse_available_max_size = EFUSE_MAX_SIZE - 3 /*0x1FD*/;

<<<<<<< HEAD
static void efuse_reg_ctrl(struct _adapter *padapter, u8 bPowerOn)
=======
static void efuse_reg_ctrl(struct _adapter *adapter, u8 bPowerOn)
>>>>>>> upstream/android-13
{
	u8 tmpu8 = 0;

	if (bPowerOn) {
		/* -----------------e-fuse pwr & clk reg ctrl ---------------
		 * Enable LDOE25 Macro Block
		 */
<<<<<<< HEAD
		tmpu8 = r8712_read8(padapter, EFUSE_TEST + 3);
		tmpu8 |= 0x80;
		r8712_write8(padapter, EFUSE_TEST + 3, tmpu8);
		msleep(20); /* for some platform , need some delay time */
		/* Change Efuse Clock for write action to 40MHZ */
		r8712_write8(padapter, EFUSE_CLK_CTRL, 0x03);
=======
		tmpu8 = r8712_read8(adapter, EFUSE_TEST + 3);
		tmpu8 |= 0x80;
		r8712_write8(adapter, EFUSE_TEST + 3, tmpu8);
		msleep(20); /* for some platform , need some delay time */
		/* Change Efuse Clock for write action to 40MHZ */
		r8712_write8(adapter, EFUSE_CLK_CTRL, 0x03);
>>>>>>> upstream/android-13
		msleep(20); /* for some platform , need some delay time */
	} else {
		/* -----------------e-fuse pwr & clk reg ctrl -----------------
		 * Disable LDOE25 Macro Block
		 */
<<<<<<< HEAD
		tmpu8 = r8712_read8(padapter, EFUSE_TEST + 3);
		tmpu8 &= 0x7F;
		r8712_write8(padapter, EFUSE_TEST + 3, tmpu8);
		/* Change Efuse Clock for write action to 500K */
		r8712_write8(padapter, EFUSE_CLK_CTRL, 0x02);
=======
		tmpu8 = r8712_read8(adapter, EFUSE_TEST + 3);
		tmpu8 &= 0x7F;
		r8712_write8(adapter, EFUSE_TEST + 3, tmpu8);
		/* Change Efuse Clock for write action to 500K */
		r8712_write8(adapter, EFUSE_CLK_CTRL, 0x02);
>>>>>>> upstream/android-13
	}
}

/*
 * Before write E-Fuse, this function must be called.
 */
<<<<<<< HEAD
u8 r8712_efuse_reg_init(struct _adapter *padapter)
=======
u8 r8712_efuse_reg_init(struct _adapter *adapter)
>>>>>>> upstream/android-13
{
	return true;
}

<<<<<<< HEAD
void r8712_efuse_reg_uninit(struct _adapter *padapter)
{
	efuse_reg_ctrl(padapter, false);
}

static u8 efuse_one_byte_read(struct _adapter *padapter, u16 addr, u8 *data)
=======
void r8712_efuse_reg_uninit(struct _adapter *adapter)
{
	efuse_reg_ctrl(adapter, false);
}

static u8 efuse_one_byte_read(struct _adapter *adapter, u16 addr, u8 *data)
>>>>>>> upstream/android-13
{
	u8 tmpidx = 0, bResult;

	/* -----------------e-fuse reg ctrl --------------------------------- */
<<<<<<< HEAD
	r8712_write8(padapter, EFUSE_CTRL + 1, (u8)(addr & 0xFF)); /* address */
	r8712_write8(padapter, EFUSE_CTRL + 2, ((u8)((addr >> 8) & 0x03)) |
	       (r8712_read8(padapter, EFUSE_CTRL + 2) & 0xFC));
	r8712_write8(padapter, EFUSE_CTRL + 3, 0x72); /* read cmd */
	/* wait for complete */
	while (!(0x80 & r8712_read8(padapter, EFUSE_CTRL + 3)) &&
	       (tmpidx < 100))
		tmpidx++;
	if (tmpidx < 100) {
		*data = r8712_read8(padapter, EFUSE_CTRL);
=======
	r8712_write8(adapter, EFUSE_CTRL + 1, (u8)(addr & 0xFF)); /* address */
	r8712_write8(adapter, EFUSE_CTRL + 2, ((u8)((addr >> 8) & 0x03)) |
	       (r8712_read8(adapter, EFUSE_CTRL + 2) & 0xFC));
	r8712_write8(adapter, EFUSE_CTRL + 3, 0x72); /* read cmd */
	/* wait for complete */
	while (!(0x80 & r8712_read8(adapter, EFUSE_CTRL + 3)) &&
	       (tmpidx < 100))
		tmpidx++;
	if (tmpidx < 100) {
		*data = r8712_read8(adapter, EFUSE_CTRL);
>>>>>>> upstream/android-13
		bResult = true;
	} else {
		*data = 0xff;
		bResult = false;
	}
	return bResult;
}

<<<<<<< HEAD
static u8 efuse_one_byte_write(struct _adapter *padapter, u16 addr, u8 data)
=======
static u8 efuse_one_byte_write(struct _adapter *adapter, u16 addr, u8 data)
>>>>>>> upstream/android-13
{
	u8 tmpidx = 0, bResult;

	/* -----------------e-fuse reg ctrl -------------------------------- */
<<<<<<< HEAD
	r8712_write8(padapter, EFUSE_CTRL + 1, (u8)(addr & 0xFF)); /* address */
	r8712_write8(padapter, EFUSE_CTRL + 2, ((u8)((addr >> 8) & 0x03)) |
	       (r8712_read8(padapter, EFUSE_CTRL + 2) & 0xFC));
	r8712_write8(padapter, EFUSE_CTRL, data); /* data */
	r8712_write8(padapter, EFUSE_CTRL + 3, 0xF2); /* write cmd */
	/* wait for complete */
	while ((0x80 &  r8712_read8(padapter, EFUSE_CTRL + 3)) &&
=======
	r8712_write8(adapter, EFUSE_CTRL + 1, (u8)(addr & 0xFF)); /* address */
	r8712_write8(adapter, EFUSE_CTRL + 2, ((u8)((addr >> 8) & 0x03)) |
	       (r8712_read8(adapter, EFUSE_CTRL + 2) & 0xFC));
	r8712_write8(adapter, EFUSE_CTRL, data); /* data */
	r8712_write8(adapter, EFUSE_CTRL + 3, 0xF2); /* write cmd */
	/* wait for complete */
	while ((0x80 &  r8712_read8(adapter, EFUSE_CTRL + 3)) &&
>>>>>>> upstream/android-13
	       (tmpidx < 100))
		tmpidx++;
	if (tmpidx < 100)
		bResult = true;
	else
		bResult = false;
	return bResult;
}

<<<<<<< HEAD
static u8 efuse_one_byte_rw(struct _adapter *padapter, u8 bRead, u16 addr,
=======
static u8 efuse_one_byte_rw(struct _adapter *adapter, u8 bRead, u16 addr,
>>>>>>> upstream/android-13
			    u8 *data)
{
	u8 tmpidx = 0, tmpv8 = 0, bResult;

	/* -----------------e-fuse reg ctrl --------------------------------- */
<<<<<<< HEAD
	r8712_write8(padapter, EFUSE_CTRL + 1, (u8)(addr & 0xFF)); /* address */
	tmpv8 = ((u8)((addr >> 8) & 0x03)) |
		 (r8712_read8(padapter, EFUSE_CTRL + 2) & 0xFC);
	r8712_write8(padapter, EFUSE_CTRL + 2, tmpv8);
	if (bRead) {
		r8712_write8(padapter, EFUSE_CTRL + 3,  0x72); /* read cmd */
		while (!(0x80 & r8712_read8(padapter, EFUSE_CTRL + 3)) &&
		       (tmpidx < 100))
			tmpidx++;
		if (tmpidx < 100) {
			*data = r8712_read8(padapter, EFUSE_CTRL);
=======
	r8712_write8(adapter, EFUSE_CTRL + 1, (u8)(addr & 0xFF)); /* address */
	tmpv8 = ((u8)((addr >> 8) & 0x03)) |
		 (r8712_read8(adapter, EFUSE_CTRL + 2) & 0xFC);
	r8712_write8(adapter, EFUSE_CTRL + 2, tmpv8);
	if (bRead) {
		r8712_write8(adapter, EFUSE_CTRL + 3,  0x72); /* read cmd */
		while (!(0x80 & r8712_read8(adapter, EFUSE_CTRL + 3)) &&
		       (tmpidx < 100))
			tmpidx++;
		if (tmpidx < 100) {
			*data = r8712_read8(adapter, EFUSE_CTRL);
>>>>>>> upstream/android-13
			bResult = true;
		} else {
			*data = 0;
			bResult = false;
		}
	} else {
<<<<<<< HEAD
		r8712_write8(padapter, EFUSE_CTRL, *data); /* data */
		r8712_write8(padapter, EFUSE_CTRL + 3, 0xF2); /* write cmd */
		while ((0x80 & r8712_read8(padapter, EFUSE_CTRL + 3)) &&
=======
		r8712_write8(adapter, EFUSE_CTRL, *data); /* data */
		r8712_write8(adapter, EFUSE_CTRL + 3, 0xF2); /* write cmd */
		while ((0x80 & r8712_read8(adapter, EFUSE_CTRL + 3)) &&
>>>>>>> upstream/android-13
		       (tmpidx < 100))
			tmpidx++;
		if (tmpidx < 100)
			bResult = true;
		else
			bResult = false;
	}
	return bResult;
}

<<<<<<< HEAD
static u8 efuse_is_empty(struct _adapter *padapter, u8 *empty)
=======
static u8 efuse_is_empty(struct _adapter *adapter, u8 *empty)
>>>>>>> upstream/android-13
{
	u8 value, ret = true;

	/* read one byte to check if E-Fuse is empty */
<<<<<<< HEAD
	if (efuse_one_byte_rw(padapter, true, 0, &value)) {
=======
	if (efuse_one_byte_rw(adapter, true, 0, &value)) {
>>>>>>> upstream/android-13
		if (value == 0xFF)
			*empty = true;
		else
			*empty = false;
	} else {
		ret = false;
	}
	return ret;
}

<<<<<<< HEAD
void r8712_efuse_change_max_size(struct _adapter *padapter)
=======
void r8712_efuse_change_max_size(struct _adapter *adapter)
>>>>>>> upstream/android-13
{
	u16 pre_pg_data_saddr = 0x1FB;
	u16 i;
	u16 pre_pg_data_size = 5;
	u8 pre_pg_data[5];

	for (i = 0; i < pre_pg_data_size; i++)
<<<<<<< HEAD
		efuse_one_byte_read(padapter, pre_pg_data_saddr + i,
=======
		efuse_one_byte_read(adapter, pre_pg_data_saddr + i,
>>>>>>> upstream/android-13
				    &pre_pg_data[i]);
	if ((pre_pg_data[0] == 0x03) && (pre_pg_data[1] == 0x00) &&
	    (pre_pg_data[2] == 0x00) && (pre_pg_data[3] == 0x00) &&
	    (pre_pg_data[4] == 0x0C))
		efuse_available_max_size -= pre_pg_data_size;
}

<<<<<<< HEAD
int r8712_efuse_get_max_size(struct _adapter *padapter)
=======
int r8712_efuse_get_max_size(struct _adapter *adapter)
>>>>>>> upstream/android-13
{
	return	efuse_available_max_size;
}

static u8 calculate_word_cnts(const u8 word_en)
{
	u8 word_cnts = 0;
	u8 word_idx;

	for (word_idx = 0; word_idx < PGPKG_MAX_WORDS; word_idx++)
		if (!(word_en & BIT(word_idx)))
			word_cnts++; /* 0 : write enable */
	return word_cnts;
}

static void pgpacket_copy_data(const u8 word_en, const u8 *sourdata,
			       u8 *targetdata)
{
	u8 tmpindex = 0;
	u8 word_idx, byte_idx;

	for (word_idx = 0; word_idx < PGPKG_MAX_WORDS; word_idx++) {
		if (!(word_en & BIT(word_idx))) {
			byte_idx = word_idx * 2;
			targetdata[byte_idx] = sourdata[tmpindex++];
			targetdata[byte_idx + 1] = sourdata[tmpindex++];
		}
	}
}

<<<<<<< HEAD
u16 r8712_efuse_get_current_size(struct _adapter *padapter)
=======
u16 r8712_efuse_get_current_size(struct _adapter *adapter)
>>>>>>> upstream/android-13
{
	int bContinual = true;
	u16 efuse_addr = 0;
	u8 hworden = 0;
	u8 efuse_data, word_cnts = 0;

<<<<<<< HEAD
	while (bContinual && efuse_one_byte_read(padapter, efuse_addr,
=======
	while (bContinual && efuse_one_byte_read(adapter, efuse_addr,
>>>>>>> upstream/android-13
	       &efuse_data) && (efuse_addr < efuse_available_max_size)) {
		if (efuse_data != 0xFF) {
			hworden =  efuse_data & 0x0F;
			word_cnts = calculate_word_cnts(hworden);
			/* read next header */
			efuse_addr = efuse_addr + (word_cnts * 2) + 1;
		} else {
			bContinual = false;
		}
	}
	return efuse_addr;
}

<<<<<<< HEAD
u8 r8712_efuse_pg_packet_read(struct _adapter *padapter, u8 offset, u8 *data)
=======
u8 r8712_efuse_pg_packet_read(struct _adapter *adapter, u8 offset, u8 *data)
>>>>>>> upstream/android-13
{
	u8 hoffset = 0, hworden = 0, word_cnts = 0;
	u16 efuse_addr = 0;
	u8 efuse_data;
	u8 tmpidx = 0;
	u8 tmpdata[PGPKT_DATA_SIZE];
	u8 ret = true;

	if (!data)
		return false;
	if (offset > 0x0f)
		return false;
	memset(data, 0xFF, sizeof(u8) * PGPKT_DATA_SIZE);
	while (efuse_addr < efuse_available_max_size) {
<<<<<<< HEAD
		if (efuse_one_byte_read(padapter, efuse_addr, &efuse_data)) {
=======
		if (efuse_one_byte_read(adapter, efuse_addr, &efuse_data)) {
>>>>>>> upstream/android-13
			if (efuse_data == 0xFF)
				break;
			hoffset = (efuse_data >> 4) & 0x0F;
			hworden =  efuse_data & 0x0F;
			word_cnts = calculate_word_cnts(hworden);
			if (hoffset == offset) {
				memset(tmpdata, 0xFF, PGPKT_DATA_SIZE);
				for (tmpidx = 0; tmpidx < word_cnts * 2;
				     tmpidx++) {
<<<<<<< HEAD
					if (efuse_one_byte_read(padapter,
=======
					if (efuse_one_byte_read(adapter,
>>>>>>> upstream/android-13
					    efuse_addr + 1 + tmpidx,
					    &efuse_data)) {
						tmpdata[tmpidx] = efuse_data;
					} else {
						ret = false;
					}
				}
				pgpacket_copy_data(hworden, tmpdata, data);
			}
			efuse_addr += 1 + (word_cnts * 2);
		} else {
			ret = false;
			break;
		}
	}
	return ret;
}

<<<<<<< HEAD
static u8 fix_header(struct _adapter *padapter, u8 header, u16 header_addr)
=======
static u8 fix_header(struct _adapter *adapter, u8 header, u16 header_addr)
>>>>>>> upstream/android-13
{
	struct PGPKT_STRUCT pkt;
	u8 offset, word_en, value;
	u16 addr;
	int i;
	u8 ret = true;

	pkt.offset = GET_EFUSE_OFFSET(header);
	pkt.word_en = GET_EFUSE_WORD_EN(header);
	addr = header_addr + 1 + calculate_word_cnts(pkt.word_en) * 2;
	if (addr > efuse_available_max_size)
		return false;
	/* retrieve original data */
	addr = 0;
	while (addr < header_addr) {
<<<<<<< HEAD
		if (!efuse_one_byte_read(padapter, addr++, &value)) {
=======
		if (!efuse_one_byte_read(adapter, addr++, &value)) {
>>>>>>> upstream/android-13
			ret = false;
			break;
		}
		offset = GET_EFUSE_OFFSET(value);
		word_en = GET_EFUSE_WORD_EN(value);
		if (pkt.offset != offset) {
			addr += calculate_word_cnts(word_en) * 2;
			continue;
		}
		for (i = 0; i < PGPKG_MAX_WORDS; i++) {
			if (BIT(i) & word_en) {
				if (BIT(i) & pkt.word_en) {
					if (efuse_one_byte_read(
<<<<<<< HEAD
							padapter, addr,
=======
							adapter, addr,
>>>>>>> upstream/android-13
							&value))
						pkt.data[i * 2] = value;
					else
						return false;
					if (efuse_one_byte_read(
<<<<<<< HEAD
							padapter,
=======
							adapter,
>>>>>>> upstream/android-13
							addr + 1,
							&value))
						pkt.data[i * 2 + 1] =
							value;
					else
						return false;
				}
				addr += 2;
			}
		}
	}
	if (addr != header_addr)
		return false;
	addr++;
	/* fill original data */
	for (i = 0; i < PGPKG_MAX_WORDS; i++) {
		if (BIT(i) & pkt.word_en) {
<<<<<<< HEAD
			efuse_one_byte_write(padapter, addr, pkt.data[i * 2]);
			efuse_one_byte_write(padapter, addr + 1,
					     pkt.data[i * 2 + 1]);
			/* additional check */
			if (!efuse_one_byte_read(padapter, addr, &value)) {
=======
			efuse_one_byte_write(adapter, addr, pkt.data[i * 2]);
			efuse_one_byte_write(adapter, addr + 1,
					     pkt.data[i * 2 + 1]);
			/* additional check */
			if (!efuse_one_byte_read(adapter, addr, &value)) {
>>>>>>> upstream/android-13
				ret = false;
			} else if (pkt.data[i * 2] != value) {
				ret = false;
				if (value == 0xFF) /* write again */
<<<<<<< HEAD
					efuse_one_byte_write(padapter, addr,
							     pkt.data[i * 2]);
			}
			if (!efuse_one_byte_read(padapter, addr + 1, &value)) {
=======
					efuse_one_byte_write(adapter, addr,
							     pkt.data[i * 2]);
			}
			if (!efuse_one_byte_read(adapter, addr + 1, &value)) {
>>>>>>> upstream/android-13
				ret = false;
			} else if (pkt.data[i * 2 + 1] != value) {
				ret = false;
				if (value == 0xFF) /* write again */
<<<<<<< HEAD
					efuse_one_byte_write(padapter, addr + 1,
=======
					efuse_one_byte_write(adapter, addr + 1,
>>>>>>> upstream/android-13
							     pkt.data[i * 2 +
								      1]);
			}
		}
		addr += 2;
	}
	return ret;
}

<<<<<<< HEAD
u8 r8712_efuse_pg_packet_write(struct _adapter *padapter, const u8 offset,
			 const u8 word_en, const u8 *data)
=======
u8 r8712_efuse_pg_packet_write(struct _adapter *adapter, const u8 offset,
			       const u8 word_en, const u8 *data)
>>>>>>> upstream/android-13
{
	u8 pg_header = 0;
	u16 efuse_addr = 0, curr_size = 0;
	u8 efuse_data, target_word_cnts = 0;
<<<<<<< HEAD
	static int repeat_times;
=======
	int repeat_times;
>>>>>>> upstream/android-13
	int sub_repeat;
	u8 bResult = true;

	/* check if E-Fuse Clock Enable and E-Fuse Clock is 40M */
<<<<<<< HEAD
	efuse_data = r8712_read8(padapter, EFUSE_CLK_CTRL);
=======
	efuse_data = r8712_read8(adapter, EFUSE_CLK_CTRL);
>>>>>>> upstream/android-13
	if (efuse_data != 0x03)
		return false;
	pg_header = MAKE_EFUSE_HEADER(offset, word_en);
	target_word_cnts = calculate_word_cnts(word_en);
	repeat_times = 0;
	efuse_addr = 0;
	while (efuse_addr < efuse_available_max_size) {
<<<<<<< HEAD
		curr_size = r8712_efuse_get_current_size(padapter);
=======
		curr_size = r8712_efuse_get_current_size(adapter);
>>>>>>> upstream/android-13
		if ((curr_size + 1 + target_word_cnts * 2) >
		     efuse_available_max_size)
			return false; /*target_word_cnts + pg header(1 byte)*/
		efuse_addr = curr_size; /* current size is also the last addr*/
<<<<<<< HEAD
		efuse_one_byte_write(padapter, efuse_addr, pg_header); /*hdr*/
		sub_repeat = 0;
		/* check if what we read is what we write */
		while (!efuse_one_byte_read(padapter, efuse_addr,
=======
		efuse_one_byte_write(adapter, efuse_addr, pg_header); /*hdr*/
		sub_repeat = 0;
		/* check if what we read is what we write */
		while (!efuse_one_byte_read(adapter, efuse_addr,
>>>>>>> upstream/android-13
					    &efuse_data)) {
			if (++sub_repeat > _REPEAT_THRESHOLD_) {
				bResult = false; /* continue to blind write */
				break; /* continue to blind write */
			}
		}
		if ((sub_repeat > _REPEAT_THRESHOLD_) ||
		    (pg_header == efuse_data)) {
			/* write header ok OR can't check header(creep) */
			u8 i;

			/* go to next address */
			efuse_addr++;
			for (i = 0; i < target_word_cnts * 2; i++) {
<<<<<<< HEAD
				efuse_one_byte_write(padapter,
						     efuse_addr + i,
						     *(data + i));
				if (!efuse_one_byte_read(padapter,
=======
				efuse_one_byte_write(adapter,
						     efuse_addr + i,
						     *(data + i));
				if (!efuse_one_byte_read(adapter,
>>>>>>> upstream/android-13
							 efuse_addr + i,
							 &efuse_data))
					bResult = false;
				else if (*(data + i) != efuse_data) /* fail */
					bResult = false;
			}
			break;
		}
		/* write header fail */
		bResult = false;
		if (efuse_data == 0xFF)
			return bResult; /* nothing damaged. */
		/* call rescue procedure */
<<<<<<< HEAD
		if (!fix_header(padapter, efuse_data, efuse_addr))
=======
		if (!fix_header(adapter, efuse_data, efuse_addr))
>>>>>>> upstream/android-13
			return false; /* rescue fail */

		if (++repeat_times > _REPEAT_THRESHOLD_) /* fail */
			break;
		/* otherwise, take another risk... */
	}
	return bResult;
}

<<<<<<< HEAD
u8 r8712_efuse_access(struct _adapter *padapter, u8 bRead, u16 start_addr,
=======
u8 r8712_efuse_access(struct _adapter *adapter, u8 bRead, u16 start_addr,
>>>>>>> upstream/android-13
		      u16 cnts, u8 *data)
{
	int i;
	u8 res = true;

	if (start_addr > EFUSE_MAX_SIZE)
		return false;
	if (!bRead && ((start_addr + cnts) >
	   efuse_available_max_size))
		return false;
<<<<<<< HEAD
	if (!bRead && !r8712_efuse_reg_init(padapter))
=======
	if (!bRead && !r8712_efuse_reg_init(adapter))
>>>>>>> upstream/android-13
		return false;
	/* -----------------e-fuse one byte read / write ---------------------*/
	for (i = 0; i < cnts; i++) {
		if ((start_addr + i) > EFUSE_MAX_SIZE) {
			res = false;
			break;
		}
<<<<<<< HEAD
		res = efuse_one_byte_rw(padapter, bRead, start_addr + i,
		      data + i);
=======
		res = efuse_one_byte_rw(adapter, bRead, start_addr + i,
					data + i);
>>>>>>> upstream/android-13
		if (!bRead && !res)
			break;
	}
	if (!bRead)
<<<<<<< HEAD
		r8712_efuse_reg_uninit(padapter);
	return res;
}

u8 r8712_efuse_map_read(struct _adapter *padapter, u16 addr, u16 cnts, u8 *data)
=======
		r8712_efuse_reg_uninit(adapter);
	return res;
}

u8 r8712_efuse_map_read(struct _adapter *adapter, u16 addr, u16 cnts, u8 *data)
>>>>>>> upstream/android-13
{
	u8 offset, ret = true;
	u8 pktdata[PGPKT_DATA_SIZE];
	int i, idx;

	if ((addr + cnts) > EFUSE_MAP_MAX_SIZE)
		return false;
<<<<<<< HEAD
	if (efuse_is_empty(padapter, &offset) && offset) {
=======
	if (efuse_is_empty(adapter, &offset) && offset) {
>>>>>>> upstream/android-13
		for (i = 0; i < cnts; i++)
			data[i] = 0xFF;
		return ret;
	}
	offset = (addr >> 3) & 0xF;
<<<<<<< HEAD
	ret = r8712_efuse_pg_packet_read(padapter, offset, pktdata);
=======
	ret = r8712_efuse_pg_packet_read(adapter, offset, pktdata);
>>>>>>> upstream/android-13
	i = addr & 0x7;	/* pktdata index */
	idx = 0;	/* data index */

	do {
		for (; i < PGPKT_DATA_SIZE; i++) {
			data[idx++] = pktdata[i];
			if (idx == cnts)
				return ret;
		}
		offset++;
<<<<<<< HEAD
		if (!r8712_efuse_pg_packet_read(padapter, offset, pktdata))
=======
		if (!r8712_efuse_pg_packet_read(adapter, offset, pktdata))
>>>>>>> upstream/android-13
			ret = false;
		i = 0;
	} while (1);
	return ret;
}

<<<<<<< HEAD
u8 r8712_efuse_map_write(struct _adapter *padapter, u16 addr, u16 cnts,
=======
u8 r8712_efuse_map_write(struct _adapter *adapter, u16 addr, u16 cnts,
>>>>>>> upstream/android-13
			 u8 *data)
{
	u8 offset, word_en, empty;
	u8 pktdata[PGPKT_DATA_SIZE], newdata[PGPKT_DATA_SIZE];
	int i, j, idx;

	if ((addr + cnts) > EFUSE_MAP_MAX_SIZE)
		return false;
	/* check if E-Fuse Clock Enable and E-Fuse Clock is 40M */
<<<<<<< HEAD
	empty = r8712_read8(padapter, EFUSE_CLK_CTRL);
	if (empty != 0x03)
		return false;
	if (efuse_is_empty(padapter, &empty)) {
=======
	empty = r8712_read8(adapter, EFUSE_CLK_CTRL);
	if (empty != 0x03)
		return false;
	if (efuse_is_empty(adapter, &empty)) {
>>>>>>> upstream/android-13
		if (empty)
			memset(pktdata, 0xFF, PGPKT_DATA_SIZE);
	} else {
		return false;
	}
	offset = (addr >> 3) & 0xF;
	if (!empty)
<<<<<<< HEAD
		if (!r8712_efuse_pg_packet_read(padapter, offset, pktdata))
=======
		if (!r8712_efuse_pg_packet_read(adapter, offset, pktdata))
>>>>>>> upstream/android-13
			return false;
	word_en = 0xF;
	memset(newdata, 0xFF, PGPKT_DATA_SIZE);
	i = addr & 0x7;	/* pktdata index */
	j = 0;		/* newdata index */
	idx = 0;	/* data index */

	if (i & 0x1) {
		/*  odd start */
		if (data[idx] != pktdata[i]) {
			word_en &= ~BIT(i >> 1);
			newdata[j++] = pktdata[i - 1];
			newdata[j++] = data[idx];
		}
		i++;
		idx++;
	}
	do {
		for (; i < PGPKT_DATA_SIZE; i += 2) {
			if ((cnts - idx) == 1) {
				if (data[idx] != pktdata[i]) {
					word_en &= ~BIT(i >> 1);
					newdata[j++] = data[idx];
					newdata[j++] = pktdata[1 + 1];
				}
				idx++;
				break;
			}

			if ((data[idx] != pktdata[i]) || (data[idx + 1] !=
			     pktdata[i + 1])) {
				word_en &= ~BIT(i >> 1);
				newdata[j++] = data[idx];
				newdata[j++] = data[idx + 1];
			}
			idx += 2;

			if (idx == cnts)
				break;
		}

		if (word_en != 0xF)
<<<<<<< HEAD
			if (!r8712_efuse_pg_packet_write(padapter, offset,
=======
			if (!r8712_efuse_pg_packet_write(adapter, offset,
>>>>>>> upstream/android-13
							 word_en, newdata))
				return false;
		if (idx == cnts)
			break;
		offset++;
		if (!empty)
<<<<<<< HEAD
			if (!r8712_efuse_pg_packet_read(padapter, offset,
			    pktdata))
=======
			if (!r8712_efuse_pg_packet_read(adapter, offset,
							pktdata))
>>>>>>> upstream/android-13
				return false;
		i = 0;
		j = 0;
		word_en = 0xF;
		memset(newdata, 0xFF, PGPKT_DATA_SIZE);
	} while (1);

	return true;
}
