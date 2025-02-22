<<<<<<< HEAD
/*
 * Copyright (C) 2014 Sergey Senozhatsky.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2014 Sergey Senozhatsky.
>>>>>>> upstream/android-13
 */

#ifndef _ZCOMP_H_
#define _ZCOMP_H_
<<<<<<< HEAD

struct zcomp_strm {
	/* compression/decompression buffer */
	void *buffer;
	struct crypto_comp *tfm;
	void *tmpbuf;
=======
#include <linux/local_lock.h>

struct zcomp_strm {
	/* The members ->buffer and ->tfm are protected by ->lock. */
	local_lock_t lock;
	/* compression/decompression buffer */
	void *buffer;
	struct crypto_comp *tfm;
>>>>>>> upstream/android-13
};

/* dynamic per-device compression frontend */
struct zcomp {
<<<<<<< HEAD
	struct zcomp_strm * __percpu *stream;
=======
	struct zcomp_strm __percpu *stream;
>>>>>>> upstream/android-13
	const char *name;
	struct hlist_node node;
};

int zcomp_cpu_up_prepare(unsigned int cpu, struct hlist_node *node);
int zcomp_cpu_dead(unsigned int cpu, struct hlist_node *node);
ssize_t zcomp_available_show(const char *comp, char *buf);
bool zcomp_available_algorithm(const char *comp);

struct zcomp *zcomp_create(const char *comp);
void zcomp_destroy(struct zcomp *comp);

struct zcomp_strm *zcomp_stream_get(struct zcomp *comp);
void zcomp_stream_put(struct zcomp *comp);

int zcomp_compress(struct zcomp_strm *zstrm,
		const void *src, unsigned int *dst_len);

int zcomp_decompress(struct zcomp_strm *zstrm,
		const void *src, unsigned int src_len, void *dst);

bool zcomp_set_max_streams(struct zcomp *comp, int num_strm);
#endif /* _ZCOMP_H_ */
