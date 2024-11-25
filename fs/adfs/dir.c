<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  linux/fs/adfs/dir.c
 *
 *  Copyright (C) 1999-2000 Russell King
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Common directory handling for ADFS
 */
=======
 *  Common directory handling for ADFS
 */
#include <linux/slab.h>
>>>>>>> upstream/android-13
#include "adfs.h"

/*
 * For future.  This should probably be per-directory.
 */
<<<<<<< HEAD
static DEFINE_RWLOCK(adfs_dir_lock);

static int
adfs_readdir(struct file *file, struct dir_context *ctx)
=======
static DECLARE_RWSEM(adfs_dir_rwsem);

int adfs_dir_copyfrom(void *dst, struct adfs_dir *dir, unsigned int offset,
		      size_t len)
{
	struct super_block *sb = dir->sb;
	unsigned int index, remain;

	index = offset >> sb->s_blocksize_bits;
	offset &= sb->s_blocksize - 1;
	remain = sb->s_blocksize - offset;
	if (index + (remain < len) >= dir->nr_buffers)
		return -EINVAL;

	if (remain < len) {
		memcpy(dst, dir->bhs[index]->b_data + offset, remain);
		dst += remain;
		len -= remain;
		index += 1;
		offset = 0;
	}

	memcpy(dst, dir->bhs[index]->b_data + offset, len);

	return 0;
}

int adfs_dir_copyto(struct adfs_dir *dir, unsigned int offset, const void *src,
		    size_t len)
{
	struct super_block *sb = dir->sb;
	unsigned int index, remain;

	index = offset >> sb->s_blocksize_bits;
	offset &= sb->s_blocksize - 1;
	remain = sb->s_blocksize - offset;
	if (index + (remain < len) >= dir->nr_buffers)
		return -EINVAL;

	if (remain < len) {
		memcpy(dir->bhs[index]->b_data + offset, src, remain);
		src += remain;
		len -= remain;
		index += 1;
		offset = 0;
	}

	memcpy(dir->bhs[index]->b_data + offset, src, len);

	return 0;
}

static void __adfs_dir_cleanup(struct adfs_dir *dir)
{
	dir->nr_buffers = 0;

	if (dir->bhs != dir->bh)
		kfree(dir->bhs);
	dir->bhs = NULL;
	dir->sb = NULL;
}

void adfs_dir_relse(struct adfs_dir *dir)
{
	unsigned int i;

	for (i = 0; i < dir->nr_buffers; i++)
		brelse(dir->bhs[i]);

	__adfs_dir_cleanup(dir);
}

static void adfs_dir_forget(struct adfs_dir *dir)
{
	unsigned int i;

	for (i = 0; i < dir->nr_buffers; i++)
		bforget(dir->bhs[i]);

	__adfs_dir_cleanup(dir);
}

int adfs_dir_read_buffers(struct super_block *sb, u32 indaddr,
			  unsigned int size, struct adfs_dir *dir)
{
	struct buffer_head **bhs;
	unsigned int i, num;
	int block;

	num = ALIGN(size, sb->s_blocksize) >> sb->s_blocksize_bits;
	if (num > ARRAY_SIZE(dir->bh)) {
		/* We only allow one extension */
		if (dir->bhs != dir->bh)
			return -EINVAL;

		bhs = kcalloc(num, sizeof(*bhs), GFP_KERNEL);
		if (!bhs)
			return -ENOMEM;

		if (dir->nr_buffers)
			memcpy(bhs, dir->bhs, dir->nr_buffers * sizeof(*bhs));

		dir->bhs = bhs;
	}

	for (i = dir->nr_buffers; i < num; i++) {
		block = __adfs_block_map(sb, indaddr, i);
		if (!block) {
			adfs_error(sb, "dir %06x has a hole at offset %u",
				   indaddr, i);
			goto error;
		}

		dir->bhs[i] = sb_bread(sb, block);
		if (!dir->bhs[i]) {
			adfs_error(sb,
				   "dir %06x failed read at offset %u, mapped block 0x%08x",
				   indaddr, i, block);
			goto error;
		}

		dir->nr_buffers++;
	}
	return 0;

error:
	adfs_dir_relse(dir);

	return -EIO;
}

static int adfs_dir_read(struct super_block *sb, u32 indaddr,
			 unsigned int size, struct adfs_dir *dir)
{
	dir->sb = sb;
	dir->bhs = dir->bh;
	dir->nr_buffers = 0;

	return ADFS_SB(sb)->s_dir->read(sb, indaddr, size, dir);
}

static int adfs_dir_read_inode(struct super_block *sb, struct inode *inode,
			       struct adfs_dir *dir)
{
	int ret;

	ret = adfs_dir_read(sb, ADFS_I(inode)->indaddr, inode->i_size, dir);
	if (ret)
		return ret;

	if (ADFS_I(inode)->parent_id != dir->parent_id) {
		adfs_error(sb,
			   "parent directory id changed under me! (%06x but got %06x)\n",
			   ADFS_I(inode)->parent_id, dir->parent_id);
		adfs_dir_relse(dir);
		ret = -EIO;
	}

	return ret;
}

static void adfs_dir_mark_dirty(struct adfs_dir *dir)
{
	unsigned int i;

	/* Mark the buffers dirty */
	for (i = 0; i < dir->nr_buffers; i++)
		mark_buffer_dirty(dir->bhs[i]);
}

static int adfs_dir_sync(struct adfs_dir *dir)
{
	int err = 0;
	int i;

	for (i = dir->nr_buffers - 1; i >= 0; i--) {
		struct buffer_head *bh = dir->bhs[i];
		sync_dirty_buffer(bh);
		if (buffer_req(bh) && !buffer_uptodate(bh))
			err = -EIO;
	}

	return err;
}

void adfs_object_fixup(struct adfs_dir *dir, struct object_info *obj)
{
	unsigned int dots, i;

	/*
	 * RISC OS allows the use of '/' in directory entry names, so we need
	 * to fix these up.  '/' is typically used for FAT compatibility to
	 * represent '.', so do the same conversion here.  In any case, '.'
	 * will never be in a RISC OS name since it is used as the pathname
	 * separator.  Handle the case where we may generate a '.' or '..'
	 * name, replacing the first character with '^' (the RISC OS "parent
	 * directory" character.)
	 */
	for (i = dots = 0; i < obj->name_len; i++)
		if (obj->name[i] == '/') {
			obj->name[i] = '.';
			dots++;
		}

	if (obj->name_len <= 2 && dots == obj->name_len)
		obj->name[0] = '^';

	/*
	 * If the object is a file, and the user requested the ,xyz hex
	 * filetype suffix to the name, check the filetype and append.
	 */
	if (!(obj->attr & ADFS_NDA_DIRECTORY) && ADFS_SB(dir->sb)->s_ftsuffix) {
		u16 filetype = adfs_filetype(obj->loadaddr);

		if (filetype != ADFS_FILETYPE_NONE) {
			obj->name[obj->name_len++] = ',';
			obj->name[obj->name_len++] = hex_asc_lo(filetype >> 8);
			obj->name[obj->name_len++] = hex_asc_lo(filetype >> 4);
			obj->name[obj->name_len++] = hex_asc_lo(filetype >> 0);
		}
	}
}

static int adfs_iterate(struct file *file, struct dir_context *ctx)
>>>>>>> upstream/android-13
{
	struct inode *inode = file_inode(file);
	struct super_block *sb = inode->i_sb;
	const struct adfs_dir_ops *ops = ADFS_SB(sb)->s_dir;
<<<<<<< HEAD
	struct object_info obj;
	struct adfs_dir dir;
	int ret = 0;

	if (ctx->pos >> 32)
		return 0;

	ret = ops->read(sb, inode->i_ino, inode->i_size, &dir);
	if (ret)
		return ret;

	if (ctx->pos == 0) {
		if (!dir_emit_dot(file, ctx))
			goto free_out;
=======
	struct adfs_dir dir;
	int ret;

	down_read(&adfs_dir_rwsem);
	ret = adfs_dir_read_inode(sb, inode, &dir);
	if (ret)
		goto unlock;

	if (ctx->pos == 0) {
		if (!dir_emit_dot(file, ctx))
			goto unlock_relse;
>>>>>>> upstream/android-13
		ctx->pos = 1;
	}
	if (ctx->pos == 1) {
		if (!dir_emit(ctx, "..", 2, dir.parent_id, DT_DIR))
<<<<<<< HEAD
			goto free_out;
		ctx->pos = 2;
	}

	read_lock(&adfs_dir_lock);

	ret = ops->setpos(&dir, ctx->pos - 2);
	if (ret)
		goto unlock_out;
	while (ops->getnext(&dir, &obj) == 0) {
		if (!dir_emit(ctx, obj.name, obj.name_len,
			    obj.file_id, DT_UNKNOWN))
			break;
		ctx->pos++;
	}

unlock_out:
	read_unlock(&adfs_dir_lock);

free_out:
	ops->free(&dir);
=======
			goto unlock_relse;
		ctx->pos = 2;
	}

	ret = ops->iterate(&dir, ctx);

unlock_relse:
	up_read(&adfs_dir_rwsem);
	adfs_dir_relse(&dir);
	return ret;

unlock:
	up_read(&adfs_dir_rwsem);
>>>>>>> upstream/android-13
	return ret;
}

int
adfs_dir_update(struct super_block *sb, struct object_info *obj, int wait)
{
<<<<<<< HEAD
	int ret = -EINVAL;
#ifdef CONFIG_ADFS_FS_RW
	const struct adfs_dir_ops *ops = ADFS_SB(sb)->s_dir;
	struct adfs_dir dir;

	printk(KERN_INFO "adfs_dir_update: object %06X in dir %06X\n",
		 obj->file_id, obj->parent_id);

	if (!ops->update) {
		ret = -EINVAL;
		goto out;
	}

	ret = ops->read(sb, obj->parent_id, 0, &dir);
	if (ret)
		goto out;

	write_lock(&adfs_dir_lock);
	ret = ops->update(&dir, obj);
	write_unlock(&adfs_dir_lock);

	if (wait) {
		int err = ops->sync(&dir);
		if (!ret)
			ret = err;
	}

	ops->free(&dir);
out:
#endif
	return ret;
}

static int
adfs_match(const struct qstr *name, struct object_info *obj)
{
	int i;

	if (name->len != obj->name_len)
		return 0;

	for (i = 0; i < name->len; i++) {
		char c1, c2;

		c1 = name->name[i];
		c2 = obj->name[i];

		if (c1 >= 'A' && c1 <= 'Z')
			c1 += 'a' - 'A';
		if (c2 >= 'A' && c2 <= 'Z')
			c2 += 'a' - 'A';

		if (c1 != c2)
			return 0;
	}
	return 1;
}

static int
adfs_dir_lookup_byname(struct inode *inode, const struct qstr *name, struct object_info *obj)
{
	struct super_block *sb = inode->i_sb;
=======
>>>>>>> upstream/android-13
	const struct adfs_dir_ops *ops = ADFS_SB(sb)->s_dir;
	struct adfs_dir dir;
	int ret;

<<<<<<< HEAD
	ret = ops->read(sb, inode->i_ino, inode->i_size, &dir);
	if (ret)
		goto out;

	if (ADFS_I(inode)->parent_id != dir.parent_id) {
		adfs_error(sb, "parent directory changed under me! (%lx but got %x)\n",
			   ADFS_I(inode)->parent_id, dir.parent_id);
		ret = -EIO;
		goto free_out;
	}

	obj->parent_id = inode->i_ino;

	read_lock(&adfs_dir_lock);

	ret = ops->setpos(&dir, 0);
	if (ret)
		goto unlock_out;

	ret = -ENOENT;
	while (ops->getnext(&dir, obj) == 0) {
		if (adfs_match(name, obj)) {
=======
	if (!IS_ENABLED(CONFIG_ADFS_FS_RW))
		return -EINVAL;

	if (!ops->update)
		return -EINVAL;

	down_write(&adfs_dir_rwsem);
	ret = adfs_dir_read(sb, obj->parent_id, 0, &dir);
	if (ret)
		goto unlock;

	ret = ops->update(&dir, obj);
	if (ret)
		goto forget;

	ret = ops->commit(&dir);
	if (ret)
		goto forget;
	up_write(&adfs_dir_rwsem);

	adfs_dir_mark_dirty(&dir);

	if (wait)
		ret = adfs_dir_sync(&dir);

	adfs_dir_relse(&dir);
	return ret;

	/*
	 * If the updated failed because the entry wasn't found, we can
	 * just release the buffers. If it was any other error, forget
	 * the dirtied buffers so they aren't written back to the media.
	 */
forget:
	if (ret == -ENOENT)
		adfs_dir_relse(&dir);
	else
		adfs_dir_forget(&dir);
unlock:
	up_write(&adfs_dir_rwsem);

	return ret;
}

static unsigned char adfs_tolower(unsigned char c)
{
	if (c >= 'A' && c <= 'Z')
		c += 'a' - 'A';
	return c;
}

static int __adfs_compare(const unsigned char *qstr, u32 qlen,
			  const char *str, u32 len)
{
	u32 i;

	if (qlen != len)
		return 1;

	for (i = 0; i < qlen; i++)
		if (adfs_tolower(qstr[i]) != adfs_tolower(str[i]))
			return 1;

	return 0;
}

static int adfs_dir_lookup_byname(struct inode *inode, const struct qstr *qstr,
				  struct object_info *obj)
{
	struct super_block *sb = inode->i_sb;
	const struct adfs_dir_ops *ops = ADFS_SB(sb)->s_dir;
	const unsigned char *name;
	struct adfs_dir dir;
	u32 name_len;
	int ret;

	down_read(&adfs_dir_rwsem);
	ret = adfs_dir_read_inode(sb, inode, &dir);
	if (ret)
		goto unlock;

	ret = ops->setpos(&dir, 0);
	if (ret)
		goto unlock_relse;

	ret = -ENOENT;
	name = qstr->name;
	name_len = qstr->len;
	while (ops->getnext(&dir, obj) == 0) {
		if (!__adfs_compare(name, name_len, obj->name, obj->name_len)) {
>>>>>>> upstream/android-13
			ret = 0;
			break;
		}
	}
<<<<<<< HEAD

unlock_out:
	read_unlock(&adfs_dir_lock);

free_out:
	ops->free(&dir);
out:
=======
	obj->parent_id = ADFS_I(inode)->indaddr;

unlock_relse:
	up_read(&adfs_dir_rwsem);
	adfs_dir_relse(&dir);
	return ret;

unlock:
	up_read(&adfs_dir_rwsem);
>>>>>>> upstream/android-13
	return ret;
}

const struct file_operations adfs_dir_operations = {
	.read		= generic_read_dir,
	.llseek		= generic_file_llseek,
<<<<<<< HEAD
	.iterate	= adfs_readdir,
=======
	.iterate_shared	= adfs_iterate,
>>>>>>> upstream/android-13
	.fsync		= generic_file_fsync,
};

static int
adfs_hash(const struct dentry *parent, struct qstr *qstr)
{
<<<<<<< HEAD
	const unsigned int name_len = ADFS_SB(parent->d_sb)->s_namelen;
	const unsigned char *name;
	unsigned long hash;
	int i;

	if (qstr->len < name_len)
		return 0;

	/*
	 * Truncate the name in place, avoids
	 * having to define a compare function.
	 */
	qstr->len = i = name_len;
	name = qstr->name;
	hash = init_name_hash(parent);
	while (i--) {
		char c;

		c = *name++;
		if (c >= 'A' && c <= 'Z')
			c += 'a' - 'A';

		hash = partial_name_hash(c, hash);
	}
=======
	const unsigned char *name;
	unsigned long hash;
	u32 len;

	if (qstr->len > ADFS_SB(parent->d_sb)->s_namelen)
		return -ENAMETOOLONG;

	len = qstr->len;
	name = qstr->name;
	hash = init_name_hash(parent);
	while (len--)
		hash = partial_name_hash(adfs_tolower(*name++), hash);
>>>>>>> upstream/android-13
	qstr->hash = end_name_hash(hash);

	return 0;
}

/*
 * Compare two names, taking note of the name length
 * requirements of the underlying filesystem.
 */
<<<<<<< HEAD
static int
adfs_compare(const struct dentry *dentry,
		unsigned int len, const char *str, const struct qstr *name)
{
	int i;

	if (len != name->len)
		return 1;

	for (i = 0; i < name->len; i++) {
		char a, b;

		a = str[i];
		b = name->name[i];

		if (a >= 'A' && a <= 'Z')
			a += 'a' - 'A';
		if (b >= 'A' && b <= 'Z')
			b += 'a' - 'A';

		if (a != b)
			return 1;
	}
	return 0;
=======
static int adfs_compare(const struct dentry *dentry, unsigned int len,
			const char *str, const struct qstr *qstr)
{
	return __adfs_compare(qstr->name, qstr->len, str, len);
>>>>>>> upstream/android-13
}

const struct dentry_operations adfs_dentry_operations = {
	.d_hash		= adfs_hash,
	.d_compare	= adfs_compare,
};

static struct dentry *
adfs_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags)
{
	struct inode *inode = NULL;
	struct object_info obj;
	int error;

	error = adfs_dir_lookup_byname(dir, &dentry->d_name, &obj);
	if (error == 0) {
		/*
		 * This only returns NULL if get_empty_inode
		 * fails.
		 */
		inode = adfs_iget(dir->i_sb, &obj);
		if (!inode)
			inode = ERR_PTR(-EACCES);
	} else if (error != -ENOENT) {
		inode = ERR_PTR(error);
	}
	return d_splice_alias(inode, dentry);
}

/*
 * directories can handle most operations...
 */
const struct inode_operations adfs_dir_inode_operations = {
	.lookup		= adfs_lookup,
	.setattr	= adfs_notify_change,
};
