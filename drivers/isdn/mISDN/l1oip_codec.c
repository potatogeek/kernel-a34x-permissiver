<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*

 * l1oip_codec.c  generic codec using lookup table
 *  -> conversion from a-Law to u-Law
 *  -> conversion from u-Law to a-Law
 *  -> compression by reducing the number of sample resolution to 4
 *
 * NOTE: It is not compatible with any standard codec like ADPCM.
 *
 * Author	Andreas Eversberg (jolly@eversberg.eu)
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> upstream/android-13

 */

/*

  How the codec works:
  --------------------

  The volume is increased to increase the dynamic range of the audio signal.
  Each sample is converted to a-LAW with only 16 steps of level resolution.
  A pair of two samples are stored in one byte.

  The first byte is stored in the upper bits, the second byte is stored in the
  lower bits.

  To speed up compression and decompression, two lookup tables are formed:

  - 16 bits index for two samples (law encoded) with 8 bit compressed result.
  - 8 bits index for one compressed data with 16 bits decompressed result.

  NOTE: The bytes are handled as they are law-encoded.

*/

#include <linux/vmalloc.h>
#include <linux/mISDNif.h>
#include <linux/in.h>
#include "core.h"
#include "l1oip.h"

/* definitions of codec. don't use calculations, code may run slower. */

static u8 *table_com;
static u16 *table_dec;


/* alaw -> ulaw */
static u8 alaw_to_ulaw[256] =
{
	0xab, 0x2b, 0xe3, 0x63, 0x8b, 0x0b, 0xc9, 0x49,
	0xba, 0x3a, 0xf6, 0x76, 0x9b, 0x1b, 0xd7, 0x57,
	0xa3, 0x23, 0xdd, 0x5d, 0x83, 0x03, 0xc1, 0x41,
	0xb2, 0x32, 0xeb, 0x6b, 0x93, 0x13, 0xcf, 0x4f,
	0xaf, 0x2f, 0xe7, 0x67, 0x8f, 0x0f, 0xcd, 0x4d,
	0xbe, 0x3e, 0xfe, 0x7e, 0x9f, 0x1f, 0xdb, 0x5b,
	0xa7, 0x27, 0xdf, 0x5f, 0x87, 0x07, 0xc5, 0x45,
	0xb6, 0x36, 0xef, 0x6f, 0x97, 0x17, 0xd3, 0x53,
	0xa9, 0x29, 0xe1, 0x61, 0x89, 0x09, 0xc7, 0x47,
	0xb8, 0x38, 0xf2, 0x72, 0x99, 0x19, 0xd5, 0x55,
	0xa1, 0x21, 0xdc, 0x5c, 0x81, 0x01, 0xbf, 0x3f,
	0xb0, 0x30, 0xe9, 0x69, 0x91, 0x11, 0xce, 0x4e,
	0xad, 0x2d, 0xe5, 0x65, 0x8d, 0x0d, 0xcb, 0x4b,
	0xbc, 0x3c, 0xfa, 0x7a, 0x9d, 0x1d, 0xd9, 0x59,
	0xa5, 0x25, 0xde, 0x5e, 0x85, 0x05, 0xc3, 0x43,
	0xb4, 0x34, 0xed, 0x6d, 0x95, 0x15, 0xd1, 0x51,
	0xac, 0x2c, 0xe4, 0x64, 0x8c, 0x0c, 0xca, 0x4a,
	0xbb, 0x3b, 0xf8, 0x78, 0x9c, 0x1c, 0xd8, 0x58,
	0xa4, 0x24, 0xde, 0x5e, 0x84, 0x04, 0xc2, 0x42,
	0xb3, 0x33, 0xec, 0x6c, 0x94, 0x14, 0xd0, 0x50,
	0xb0, 0x30, 0xe8, 0x68, 0x90, 0x10, 0xce, 0x4e,
	0xbf, 0x3f, 0xfe, 0x7e, 0xa0, 0x20, 0xdc, 0x5c,
	0xa8, 0x28, 0xe0, 0x60, 0x88, 0x08, 0xc6, 0x46,
	0xb7, 0x37, 0xf0, 0x70, 0x98, 0x18, 0xd4, 0x54,
	0xaa, 0x2a, 0xe2, 0x62, 0x8a, 0x0a, 0xc8, 0x48,
	0xb9, 0x39, 0xf4, 0x74, 0x9a, 0x1a, 0xd6, 0x56,
	0xa2, 0x22, 0xdd, 0x5d, 0x82, 0x02, 0xc0, 0x40,
	0xb1, 0x31, 0xea, 0x6a, 0x92, 0x12, 0xcf, 0x4f,
	0xae, 0x2e, 0xe6, 0x66, 0x8e, 0x0e, 0xcc, 0x4c,
	0xbd, 0x3d, 0xfc, 0x7c, 0x9e, 0x1e, 0xda, 0x5a,
	0xa6, 0x26, 0xdf, 0x5f, 0x86, 0x06, 0xc4, 0x44,
	0xb5, 0x35, 0xee, 0x6e, 0x96, 0x16, 0xd2, 0x52
};

/* ulaw -> alaw */
static u8 ulaw_to_alaw[256] =
{
	0xab, 0x55, 0xd5, 0x15, 0x95, 0x75, 0xf5, 0x35,
	0xb5, 0x45, 0xc5, 0x05, 0x85, 0x65, 0xe5, 0x25,
	0xa5, 0x5d, 0xdd, 0x1d, 0x9d, 0x7d, 0xfd, 0x3d,
	0xbd, 0x4d, 0xcd, 0x0d, 0x8d, 0x6d, 0xed, 0x2d,
	0xad, 0x51, 0xd1, 0x11, 0x91, 0x71, 0xf1, 0x31,
	0xb1, 0x41, 0xc1, 0x01, 0x81, 0x61, 0xe1, 0x21,
	0x59, 0xd9, 0x19, 0x99, 0x79, 0xf9, 0x39, 0xb9,
	0x49, 0xc9, 0x09, 0x89, 0x69, 0xe9, 0x29, 0xa9,
	0xd7, 0x17, 0x97, 0x77, 0xf7, 0x37, 0xb7, 0x47,
	0xc7, 0x07, 0x87, 0x67, 0xe7, 0x27, 0xa7, 0xdf,
	0x9f, 0x7f, 0xff, 0x3f, 0xbf, 0x4f, 0xcf, 0x0f,
	0x8f, 0x6f, 0xef, 0x2f, 0x53, 0x13, 0x73, 0x33,
	0xb3, 0x43, 0xc3, 0x03, 0x83, 0x63, 0xe3, 0x23,
	0xa3, 0x5b, 0xdb, 0x1b, 0x9b, 0x7b, 0xfb, 0x3b,
	0xbb, 0xbb, 0x4b, 0x4b, 0xcb, 0xcb, 0x0b, 0x0b,
	0x8b, 0x8b, 0x6b, 0x6b, 0xeb, 0xeb, 0x2b, 0x2b,
	0xab, 0x54, 0xd4, 0x14, 0x94, 0x74, 0xf4, 0x34,
	0xb4, 0x44, 0xc4, 0x04, 0x84, 0x64, 0xe4, 0x24,
	0xa4, 0x5c, 0xdc, 0x1c, 0x9c, 0x7c, 0xfc, 0x3c,
	0xbc, 0x4c, 0xcc, 0x0c, 0x8c, 0x6c, 0xec, 0x2c,
	0xac, 0x50, 0xd0, 0x10, 0x90, 0x70, 0xf0, 0x30,
	0xb0, 0x40, 0xc0, 0x00, 0x80, 0x60, 0xe0, 0x20,
	0x58, 0xd8, 0x18, 0x98, 0x78, 0xf8, 0x38, 0xb8,
	0x48, 0xc8, 0x08, 0x88, 0x68, 0xe8, 0x28, 0xa8,
	0xd6, 0x16, 0x96, 0x76, 0xf6, 0x36, 0xb6, 0x46,
	0xc6, 0x06, 0x86, 0x66, 0xe6, 0x26, 0xa6, 0xde,
	0x9e, 0x7e, 0xfe, 0x3e, 0xbe, 0x4e, 0xce, 0x0e,
	0x8e, 0x6e, 0xee, 0x2e, 0x52, 0x12, 0x72, 0x32,
	0xb2, 0x42, 0xc2, 0x02, 0x82, 0x62, 0xe2, 0x22,
	0xa2, 0x5a, 0xda, 0x1a, 0x9a, 0x7a, 0xfa, 0x3a,
	0xba, 0xba, 0x4a, 0x4a, 0xca, 0xca, 0x0a, 0x0a,
	0x8a, 0x8a, 0x6a, 0x6a, 0xea, 0xea, 0x2a, 0x2a
};

/* alaw -> 4bit compression */
static u8 alaw_to_4bit[256] = {
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x08, 0x07, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x09, 0x06, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x08, 0x07, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x09, 0x06, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x08, 0x07, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0d, 0x02,
	0x0e, 0x02, 0x09, 0x06, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x08, 0x07, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x09, 0x06, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x08, 0x07, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x09, 0x06, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x02, 0x09, 0x06, 0x0f, 0x00, 0x0b, 0x04,
	0x0d, 0x02, 0x08, 0x07, 0x0f, 0x01, 0x0a, 0x05,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x09, 0x07, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x08, 0x07, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x09, 0x06, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x08, 0x07, 0x0f, 0x00, 0x0b, 0x04,
	0x0e, 0x01, 0x0a, 0x05, 0x0f, 0x00, 0x0c, 0x03,
	0x0d, 0x02, 0x09, 0x06, 0x0f, 0x00, 0x0b, 0x04,
};

/* 4bit -> alaw decompression */
static u8 _4bit_to_alaw[16] = {
	0x5d, 0x51, 0xd9, 0xd7, 0x5f, 0x53, 0xa3, 0x4b,
	0x2a, 0x3a, 0x22, 0x2e, 0x26, 0x56, 0x20, 0x2c,
};

/* ulaw -> 4bit compression */
static u8 ulaw_to_4bit[256] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x08,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	0x0f, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e,
	0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e,
	0x0e, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
	0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0b, 0x0b,
	0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
	0x0b, 0x0b, 0x0b, 0x0b, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
	0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
	0x09, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
};

/* 4bit -> ulaw decompression */
static u8 _4bit_to_ulaw[16] = {
	0x11, 0x21, 0x31, 0x40, 0x4e, 0x5c, 0x68, 0x71,
	0xfe, 0xef, 0xe7, 0xdb, 0xcd, 0xbf, 0xaf, 0x9f,
};


/*
 * Compresses data to the result buffer
 * The result size must be at least half of the input buffer.
 * The number of samples also must be even!
 */
int
l1oip_law_to_4bit(u8 *data, int len, u8 *result, u32 *state)
{
	int ii, i = 0, o = 0;

	if (!len)
		return 0;

	/* send saved byte and first input byte */
	if (*state) {
		*result++ = table_com[(((*state) << 8) & 0xff00) | (*data++)];
		len--;
		o++;
	}

	ii = len >> 1;

	while (i < ii) {
		*result++ = table_com[(data[0]<<8) | (data[1])];
		data += 2;
		i++;
		o++;
	}

	/* if len has an odd number, we save byte for next call */
	if (len & 1)
		*state = 0x100 + *data;
	else
		*state = 0;

	return o;
}

/* Decompress data to the result buffer
 * The result size must be the number of sample in packet. (2 * input data)
 * The number of samples in the result are even!
 */
int
l1oip_4bit_to_law(u8 *data, int len, u8 *result)
{
	int i = 0;
	u16 r;

	while (i < len) {
		r = table_dec[*data++];
		*result++ = r >> 8;
		*result++ = r;
		i++;
	}

	return len << 1;
}


/*
 * law conversion
 */
int
l1oip_alaw_to_ulaw(u8 *data, int len, u8 *result)
{
	int i = 0;

	while (i < len) {
		*result++ = alaw_to_ulaw[*data++];
		i++;
	}

	return len;
}

int
l1oip_ulaw_to_alaw(u8 *data, int len, u8 *result)
{
	int i = 0;

	while (i < len) {
		*result++ = ulaw_to_alaw[*data++];
		i++;
	}

	return len;
}


/*
 * generate/free compression and decompression table
 */
void
l1oip_4bit_free(void)
{
	vfree(table_dec);
	vfree(table_com);
	table_com = NULL;
	table_dec = NULL;
}

int
l1oip_4bit_alloc(int ulaw)
{
	int i1, i2, c, sample;

	/* in case, it is called again */
	if (table_dec)
		return 0;

	/* alloc conversion tables */
	table_com = vzalloc(65536);
	table_dec = vzalloc(512);
	if (!table_com || !table_dec) {
		l1oip_4bit_free();
		return -ENOMEM;
	}
	/* generate compression table */
	i1 = 0;
	while (i1 < 256) {
		if (ulaw)
			c = ulaw_to_4bit[i1];
		else
			c = alaw_to_4bit[i1];
		i2 = 0;
		while (i2 < 256) {
			table_com[(i1 << 8) | i2] |= (c << 4);
			table_com[(i2 << 8) | i1] |= c;
			i2++;
		}
		i1++;
	}

	/* generate decompression table */
	i1 = 0;
	while (i1 < 16) {
		if (ulaw)
			sample = _4bit_to_ulaw[i1];
		else
			sample = _4bit_to_alaw[i1];
		i2 = 0;
		while (i2 < 16) {
			table_dec[(i1 << 4) | i2] |= (sample << 8);
			table_dec[(i2 << 4) | i1] |= sample;
			i2++;
		}
		i1++;
	}

	return 0;
}
