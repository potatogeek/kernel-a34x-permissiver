// SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0
/* Copyright (c) 2016-2018 Mellanox Technologies. All rights reserved */

#include <linux/kernel.h>
<<<<<<< HEAD
=======
#include <linux/mutex.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>

#include "spectrum.h"

struct mlxsw_sp_kvdl {
	const struct mlxsw_sp_kvdl_ops *kvdl_ops;
<<<<<<< HEAD
	unsigned long priv[0];
=======
	struct mutex kvdl_lock; /* Protects kvdl allocations */
	unsigned long priv[];
>>>>>>> upstream/android-13
	/* priv has to be always the last item */
};

int mlxsw_sp_kvdl_init(struct mlxsw_sp *mlxsw_sp)
{
	const struct mlxsw_sp_kvdl_ops *kvdl_ops = mlxsw_sp->kvdl_ops;
	struct mlxsw_sp_kvdl *kvdl;
	int err;

	kvdl = kzalloc(sizeof(*mlxsw_sp->kvdl) + kvdl_ops->priv_size,
		       GFP_KERNEL);
	if (!kvdl)
		return -ENOMEM;
<<<<<<< HEAD
=======
	mutex_init(&kvdl->kvdl_lock);
>>>>>>> upstream/android-13
	kvdl->kvdl_ops = kvdl_ops;
	mlxsw_sp->kvdl = kvdl;

	err = kvdl_ops->init(mlxsw_sp, kvdl->priv);
	if (err)
		goto err_init;
	return 0;

err_init:
<<<<<<< HEAD
=======
	mutex_destroy(&kvdl->kvdl_lock);
>>>>>>> upstream/android-13
	kfree(kvdl);
	return err;
}

void mlxsw_sp_kvdl_fini(struct mlxsw_sp *mlxsw_sp)
{
	struct mlxsw_sp_kvdl *kvdl = mlxsw_sp->kvdl;

	kvdl->kvdl_ops->fini(mlxsw_sp, kvdl->priv);
<<<<<<< HEAD
=======
	mutex_destroy(&kvdl->kvdl_lock);
>>>>>>> upstream/android-13
	kfree(kvdl);
}

int mlxsw_sp_kvdl_alloc(struct mlxsw_sp *mlxsw_sp,
			enum mlxsw_sp_kvdl_entry_type type,
			unsigned int entry_count, u32 *p_entry_index)
{
	struct mlxsw_sp_kvdl *kvdl = mlxsw_sp->kvdl;
<<<<<<< HEAD

	return kvdl->kvdl_ops->alloc(mlxsw_sp, kvdl->priv, type,
				     entry_count, p_entry_index);
=======
	int err;

	mutex_lock(&kvdl->kvdl_lock);
	err = kvdl->kvdl_ops->alloc(mlxsw_sp, kvdl->priv, type,
				    entry_count, p_entry_index);
	mutex_unlock(&kvdl->kvdl_lock);

	return err;
>>>>>>> upstream/android-13
}

void mlxsw_sp_kvdl_free(struct mlxsw_sp *mlxsw_sp,
			enum mlxsw_sp_kvdl_entry_type type,
			unsigned int entry_count, int entry_index)
{
	struct mlxsw_sp_kvdl *kvdl = mlxsw_sp->kvdl;

<<<<<<< HEAD
	kvdl->kvdl_ops->free(mlxsw_sp, kvdl->priv, type,
			     entry_count, entry_index);
=======
	mutex_lock(&kvdl->kvdl_lock);
	kvdl->kvdl_ops->free(mlxsw_sp, kvdl->priv, type,
			     entry_count, entry_index);
	mutex_unlock(&kvdl->kvdl_lock);
>>>>>>> upstream/android-13
}

int mlxsw_sp_kvdl_alloc_count_query(struct mlxsw_sp *mlxsw_sp,
				    enum mlxsw_sp_kvdl_entry_type type,
				    unsigned int entry_count,
				    unsigned int *p_alloc_count)
{
	struct mlxsw_sp_kvdl *kvdl = mlxsw_sp->kvdl;

	return kvdl->kvdl_ops->alloc_size_query(mlxsw_sp, kvdl->priv, type,
						entry_count, p_alloc_count);
}
