<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * vpd_decode.h
 *
 * Google VPD decoding routines.
 *
 * Copyright 2017 Google Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v2.0 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef __VPD_DECODE_H
#define __VPD_DECODE_H

#include <linux/types.h>

enum {
	VPD_OK = 0,
	VPD_FAIL,
};

enum {
	VPD_TYPE_TERMINATOR = 0,
	VPD_TYPE_STRING,
	VPD_TYPE_INFO                = 0xfe,
	VPD_TYPE_IMPLICIT_TERMINATOR = 0xff,
};

/* Callback for vpd_decode_string to invoke. */
typedef int vpd_decode_callback(const u8 *key, u32 key_len,
				const u8 *value, u32 value_len,
				void *arg);

/*
 * vpd_decode_string
 *
 * Given the encoded string, this function invokes callback with extracted
 * (key, value). The *consumed will be plused the number of bytes consumed in
 * this function.
 *
 * The input_buf points to the first byte of the input buffer.
 *
 * The *consumed starts from 0, which is actually the next byte to be decoded.
 * It can be non-zero to be used in multiple calls.
 *
 * If one entry is successfully decoded, sends it to callback and returns the
 * result.
 */
int vpd_decode_string(const u32 max_len, const u8 *input_buf, u32 *consumed,
		      vpd_decode_callback callback, void *callback_arg);

#endif  /* __VPD_DECODE_H */
