<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/* Internal definitions for asymmetric key type
 *
 * Copyright (C) 2012 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <keys/asymmetric-type.h>

extern struct asymmetric_key_id *asymmetric_key_hex_to_key_id(const char *id);

extern int __asymmetric_key_hex_to_key_id(const char *id,
					  struct asymmetric_key_id *match_id,
					  size_t hexlen);
<<<<<<< HEAD
=======

extern int asymmetric_key_eds_op(struct kernel_pkey_params *params,
				 const void *in, void *out);
>>>>>>> upstream/android-13
