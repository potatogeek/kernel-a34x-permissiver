<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/* Big capacity key type.
 *
 * Copyright (C) 2013 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef _KEYS_BIG_KEY_TYPE_H
#define _KEYS_BIG_KEY_TYPE_H

#include <linux/key-type.h>

extern struct key_type key_type_big_key;

extern int big_key_preparse(struct key_preparsed_payload *prep);
extern void big_key_free_preparse(struct key_preparsed_payload *prep);
extern void big_key_revoke(struct key *key);
extern void big_key_destroy(struct key *key);
extern void big_key_describe(const struct key *big_key, struct seq_file *m);
extern long big_key_read(const struct key *key, char *buffer, size_t buflen);
<<<<<<< HEAD
=======
extern int big_key_update(struct key *key, struct key_preparsed_payload *prep);
>>>>>>> upstream/android-13

#endif /* _KEYS_BIG_KEY_TYPE_H */
