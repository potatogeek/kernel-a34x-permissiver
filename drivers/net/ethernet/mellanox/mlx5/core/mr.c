/*
 * Copyright (c) 2013-2015, Mellanox Technologies. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mlx5/driver.h>
<<<<<<< HEAD
#include <linux/mlx5/cmd.h>
#include "mlx5_core.h"

void mlx5_init_mkey_table(struct mlx5_core_dev *dev)
{
	struct mlx5_mkey_table *table = &dev->priv.mkey_table;

	memset(table, 0, sizeof(*table));
	rwlock_init(&table->lock);
	INIT_RADIX_TREE(&table->tree, GFP_ATOMIC);
}

void mlx5_cleanup_mkey_table(struct mlx5_core_dev *dev)
{
}

int mlx5_core_create_mkey_cb(struct mlx5_core_dev *dev,
			     struct mlx5_core_mkey *mkey,
			     u32 *in, int inlen,
			     u32 *out, int outlen,
			     mlx5_cmd_cbk_t callback, void *context)
{
	struct mlx5_mkey_table *table = &dev->priv.mkey_table;
	u32 lout[MLX5_ST_SZ_DW(create_mkey_out)] = {0};
	u32 mkey_index;
	void *mkc;
	int err;
	u8 key;

	spin_lock_irq(&dev->priv.mkey_lock);
	key = dev->priv.mkey_key++;
	spin_unlock_irq(&dev->priv.mkey_lock);
	mkc = MLX5_ADDR_OF(create_mkey_in, in, memory_key_mkey_entry);

	MLX5_SET(create_mkey_in, in, opcode, MLX5_CMD_OP_CREATE_MKEY);
	MLX5_SET(mkc, mkc, mkey_7_0, key);

	if (callback)
		return mlx5_cmd_exec_cb(dev, in, inlen, out, outlen,
					callback, context);

	err = mlx5_cmd_exec(dev, in, inlen, lout, sizeof(lout));
	if (err)
		return err;

	mkey_index = MLX5_GET(create_mkey_out, lout, mkey_index);
	mkey->iova = MLX5_GET64(mkc, mkc, start_addr);
	mkey->size = MLX5_GET64(mkc, mkc, len);
	mkey->key = mlx5_idx_to_mkey(mkey_index) | key;
	mkey->pd = MLX5_GET(mkc, mkc, pd);

	mlx5_core_dbg(dev, "out 0x%x, key 0x%x, mkey 0x%x\n",
		      mkey_index, key, mkey->key);

	/* connect to mkey tree */
	write_lock_irq(&table->lock);
	err = radix_tree_insert(&table->tree, mlx5_base_mkey(mkey->key), mkey);
	write_unlock_irq(&table->lock);
	if (err) {
		mlx5_core_warn(dev, "failed radix tree insert of mkey 0x%x, %d\n",
			       mlx5_base_mkey(mkey->key), err);
		mlx5_core_destroy_mkey(dev, mkey);
	}

	return err;
}
EXPORT_SYMBOL(mlx5_core_create_mkey_cb);

=======
#include "mlx5_core.h"

>>>>>>> upstream/android-13
int mlx5_core_create_mkey(struct mlx5_core_dev *dev,
			  struct mlx5_core_mkey *mkey,
			  u32 *in, int inlen)
{
<<<<<<< HEAD
	return mlx5_core_create_mkey_cb(dev, mkey, in, inlen,
					NULL, 0, NULL, NULL);
=======
	u32 lout[MLX5_ST_SZ_DW(create_mkey_out)] = {};
	u32 mkey_index;
	void *mkc;
	int err;

	MLX5_SET(create_mkey_in, in, opcode, MLX5_CMD_OP_CREATE_MKEY);

	err = mlx5_cmd_exec(dev, in, inlen, lout, sizeof(lout));
	if (err)
		return err;

	mkc = MLX5_ADDR_OF(create_mkey_in, in, memory_key_mkey_entry);
	mkey_index = MLX5_GET(create_mkey_out, lout, mkey_index);
	mkey->iova = MLX5_GET64(mkc, mkc, start_addr);
	mkey->size = MLX5_GET64(mkc, mkc, len);
	mkey->key = (u32)mlx5_mkey_variant(mkey->key) | mlx5_idx_to_mkey(mkey_index);
	mkey->pd = MLX5_GET(mkc, mkc, pd);
	init_waitqueue_head(&mkey->wait);

	mlx5_core_dbg(dev, "out 0x%x, mkey 0x%x\n", mkey_index, mkey->key);
	return 0;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(mlx5_core_create_mkey);

int mlx5_core_destroy_mkey(struct mlx5_core_dev *dev,
			   struct mlx5_core_mkey *mkey)
{
<<<<<<< HEAD
	struct mlx5_mkey_table *table = &dev->priv.mkey_table;
	u32 out[MLX5_ST_SZ_DW(destroy_mkey_out)] = {0};
	u32 in[MLX5_ST_SZ_DW(destroy_mkey_in)]   = {0};
	struct mlx5_core_mkey *deleted_mkey;
	unsigned long flags;

	write_lock_irqsave(&table->lock, flags);
	deleted_mkey = radix_tree_delete(&table->tree, mlx5_base_mkey(mkey->key));
	write_unlock_irqrestore(&table->lock, flags);
	if (!deleted_mkey) {
		mlx5_core_dbg(dev, "failed radix tree delete of mkey 0x%x\n",
			      mlx5_base_mkey(mkey->key));
		return -ENOENT;
	}

	MLX5_SET(destroy_mkey_in, in, opcode, MLX5_CMD_OP_DESTROY_MKEY);
	MLX5_SET(destroy_mkey_in, in, mkey_index, mlx5_mkey_to_idx(mkey->key));
	return mlx5_cmd_exec(dev, in, sizeof(in), out, sizeof(out));
=======
	u32 in[MLX5_ST_SZ_DW(destroy_mkey_in)] = {};

	MLX5_SET(destroy_mkey_in, in, opcode, MLX5_CMD_OP_DESTROY_MKEY);
	MLX5_SET(destroy_mkey_in, in, mkey_index, mlx5_mkey_to_idx(mkey->key));
	return mlx5_cmd_exec_in(dev, destroy_mkey, in);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(mlx5_core_destroy_mkey);

int mlx5_core_query_mkey(struct mlx5_core_dev *dev, struct mlx5_core_mkey *mkey,
			 u32 *out, int outlen)
{
<<<<<<< HEAD
	u32 in[MLX5_ST_SZ_DW(query_mkey_in)] = {0};
=======
	u32 in[MLX5_ST_SZ_DW(query_mkey_in)] = {};
>>>>>>> upstream/android-13

	memset(out, 0, outlen);
	MLX5_SET(query_mkey_in, in, opcode, MLX5_CMD_OP_QUERY_MKEY);
	MLX5_SET(query_mkey_in, in, mkey_index, mlx5_mkey_to_idx(mkey->key));
	return mlx5_cmd_exec(dev, in, sizeof(in), out, outlen);
}
EXPORT_SYMBOL(mlx5_core_query_mkey);

static inline u32 mlx5_get_psv(u32 *out, int psv_index)
{
	switch (psv_index) {
	case 1: return MLX5_GET(create_psv_out, out, psv1_index);
	case 2: return MLX5_GET(create_psv_out, out, psv2_index);
	case 3: return MLX5_GET(create_psv_out, out, psv3_index);
	default: return MLX5_GET(create_psv_out, out, psv0_index);
	}
}

int mlx5_core_create_psv(struct mlx5_core_dev *dev, u32 pdn,
			 int npsvs, u32 *sig_index)
{
<<<<<<< HEAD
	u32 out[MLX5_ST_SZ_DW(create_psv_out)] = {0};
	u32 in[MLX5_ST_SZ_DW(create_psv_in)]   = {0};
=======
	u32 out[MLX5_ST_SZ_DW(create_psv_out)] = {};
	u32 in[MLX5_ST_SZ_DW(create_psv_in)] = {};
>>>>>>> upstream/android-13
	int i, err;

	if (npsvs > MLX5_MAX_PSVS)
		return -EINVAL;

	MLX5_SET(create_psv_in, in, opcode, MLX5_CMD_OP_CREATE_PSV);
	MLX5_SET(create_psv_in, in, pd, pdn);
	MLX5_SET(create_psv_in, in, num_psv, npsvs);

<<<<<<< HEAD
	err = mlx5_cmd_exec(dev, in, sizeof(in), out, sizeof(out));
=======
	err = mlx5_cmd_exec_inout(dev, create_psv, in, out);
>>>>>>> upstream/android-13
	if (err)
		return err;

	for (i = 0; i < npsvs; i++)
		sig_index[i] = mlx5_get_psv(out, i);

	return err;
}
EXPORT_SYMBOL(mlx5_core_create_psv);

int mlx5_core_destroy_psv(struct mlx5_core_dev *dev, int psv_num)
{
<<<<<<< HEAD
	u32 out[MLX5_ST_SZ_DW(destroy_psv_out)] = {0};
	u32 in[MLX5_ST_SZ_DW(destroy_psv_in)]   = {0};

	MLX5_SET(destroy_psv_in, in, opcode, MLX5_CMD_OP_DESTROY_PSV);
	MLX5_SET(destroy_psv_in, in, psvn, psv_num);
	return mlx5_cmd_exec(dev, in, sizeof(in), out, sizeof(out));
=======
	u32 in[MLX5_ST_SZ_DW(destroy_psv_in)] = {};

	MLX5_SET(destroy_psv_in, in, opcode, MLX5_CMD_OP_DESTROY_PSV);
	MLX5_SET(destroy_psv_in, in, psvn, psv_num);
	return mlx5_cmd_exec_in(dev, destroy_psv, in);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(mlx5_core_destroy_psv);
