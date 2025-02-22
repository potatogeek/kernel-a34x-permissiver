<<<<<<< HEAD
/*
 * Copyright (c) 2013
 * Phillip Lougher <phillip@squashfs.org.uk>
 *
 * This work is licensed under the terms of the GNU GPL, version 2. See
 * the COPYING file in the top-level directory.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2013
 * Phillip Lougher <phillip@squashfs.org.uk>
>>>>>>> upstream/android-13
 */

#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/buffer_head.h>
=======
#include <linux/bio.h>
>>>>>>> upstream/android-13

#include "squashfs_fs.h"
#include "squashfs_fs_sb.h"
#include "decompressor.h"
#include "squashfs.h"

/*
 * This file implements single-threaded decompression in the
 * decompressor framework
 */

struct squashfs_stream {
	void		*stream;
	struct mutex	mutex;
};

void *squashfs_decompressor_create(struct squashfs_sb_info *msblk,
						void *comp_opts)
{
	struct squashfs_stream *stream;
	int err = -ENOMEM;

	stream = kmalloc(sizeof(*stream), GFP_KERNEL);
	if (stream == NULL)
		goto out;

	stream->stream = msblk->decompressor->init(msblk, comp_opts);
	if (IS_ERR(stream->stream)) {
		err = PTR_ERR(stream->stream);
		goto out;
	}

	kfree(comp_opts);
	mutex_init(&stream->mutex);
	return stream;

out:
	kfree(stream);
	return ERR_PTR(err);
}

void squashfs_decompressor_destroy(struct squashfs_sb_info *msblk)
{
	struct squashfs_stream *stream = msblk->stream;

	if (stream) {
		msblk->decompressor->free(stream->stream);
		kfree(stream);
	}
}

<<<<<<< HEAD
int squashfs_decompress(struct squashfs_sb_info *msblk, struct buffer_head **bh,
	int b, int offset, int length, struct squashfs_page_actor *output)
=======
int squashfs_decompress(struct squashfs_sb_info *msblk, struct bio *bio,
			int offset, int length,
			struct squashfs_page_actor *output)
>>>>>>> upstream/android-13
{
	int res;
	struct squashfs_stream *stream = msblk->stream;

	mutex_lock(&stream->mutex);
<<<<<<< HEAD
	res = msblk->decompressor->decompress(msblk, stream->stream, bh, b,
=======
	res = msblk->decompressor->decompress(msblk, stream->stream, bio,
>>>>>>> upstream/android-13
		offset, length, output);
	mutex_unlock(&stream->mutex);

	if (res < 0)
		ERROR("%s decompression failed, data probably corrupt\n",
			msblk->decompressor->name);

	return res;
}

int squashfs_max_decompressors(void)
{
	return 1;
}
