/*
 * Copyright (c) 2018 Mellanox Technologies. All rights reserved.
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
 *
 */

#include <linux/ethtool.h>
#include <net/sock.h>

#include "en.h"
<<<<<<< HEAD
#include "accel/tls.h"
=======
>>>>>>> upstream/android-13
#include "fpga/sdk.h"
#include "en_accel/tls.h"

static const struct counter_desc mlx5e_tls_sw_stats_desc[] = {
	{ MLX5E_DECLARE_STAT(struct mlx5e_tls_sw_stats, tx_tls_drop_metadata) },
	{ MLX5E_DECLARE_STAT(struct mlx5e_tls_sw_stats, tx_tls_drop_resync_alloc) },
	{ MLX5E_DECLARE_STAT(struct mlx5e_tls_sw_stats, tx_tls_drop_no_sync_data) },
	{ MLX5E_DECLARE_STAT(struct mlx5e_tls_sw_stats, tx_tls_drop_bypass_required) },
};

<<<<<<< HEAD
#define MLX5E_READ_CTR_ATOMIC64(ptr, dsc, i) \
	atomic64_read((atomic64_t *)((char *)(ptr) + (dsc)[i].offset))

#define NUM_TLS_SW_COUNTERS ARRAY_SIZE(mlx5e_tls_sw_stats_desc)
=======
static const struct counter_desc mlx5e_ktls_sw_stats_desc[] = {
	{ MLX5E_DECLARE_STAT(struct mlx5e_tls_sw_stats, tx_tls_ctx) },
	{ MLX5E_DECLARE_STAT(struct mlx5e_tls_sw_stats, tx_tls_del) },
	{ MLX5E_DECLARE_STAT(struct mlx5e_tls_sw_stats, rx_tls_ctx) },
	{ MLX5E_DECLARE_STAT(struct mlx5e_tls_sw_stats, rx_tls_del) },
};

#define MLX5E_READ_CTR_ATOMIC64(ptr, dsc, i) \
	atomic64_read((atomic64_t *)((char *)(ptr) + (dsc)[i].offset))

static const struct counter_desc *get_tls_atomic_stats(struct mlx5e_priv *priv)
{
	if (!priv->tls)
		return NULL;
	if (mlx5e_accel_is_ktls_device(priv->mdev))
		return mlx5e_ktls_sw_stats_desc;
	return mlx5e_tls_sw_stats_desc;
}
>>>>>>> upstream/android-13

int mlx5e_tls_get_count(struct mlx5e_priv *priv)
{
	if (!priv->tls)
		return 0;
<<<<<<< HEAD

	return NUM_TLS_SW_COUNTERS;
=======
	if (mlx5e_accel_is_ktls_device(priv->mdev))
		return ARRAY_SIZE(mlx5e_ktls_sw_stats_desc);
	return ARRAY_SIZE(mlx5e_tls_sw_stats_desc);
>>>>>>> upstream/android-13
}

int mlx5e_tls_get_strings(struct mlx5e_priv *priv, uint8_t *data)
{
<<<<<<< HEAD
	unsigned int i, idx = 0;

	if (!priv->tls)
		return 0;

	for (i = 0; i < NUM_TLS_SW_COUNTERS; i++)
		strcpy(data + (idx++) * ETH_GSTRING_LEN,
		       mlx5e_tls_sw_stats_desc[i].format);

	return NUM_TLS_SW_COUNTERS;
=======
	const struct counter_desc *stats_desc;
	unsigned int i, n, idx = 0;

	stats_desc = get_tls_atomic_stats(priv);
	n = mlx5e_tls_get_count(priv);

	for (i = 0; i < n; i++)
		strcpy(data + (idx++) * ETH_GSTRING_LEN,
		       stats_desc[i].format);

	return n;
>>>>>>> upstream/android-13
}

int mlx5e_tls_get_stats(struct mlx5e_priv *priv, u64 *data)
{
<<<<<<< HEAD
	int i, idx = 0;

	if (!priv->tls)
		return 0;

	for (i = 0; i < NUM_TLS_SW_COUNTERS; i++)
		data[idx++] =
		    MLX5E_READ_CTR_ATOMIC64(&priv->tls->sw_stats,
					    mlx5e_tls_sw_stats_desc, i);

	return NUM_TLS_SW_COUNTERS;
=======
	const struct counter_desc *stats_desc;
	unsigned int i, n, idx = 0;

	stats_desc = get_tls_atomic_stats(priv);
	n = mlx5e_tls_get_count(priv);

	for (i = 0; i < n; i++)
		data[idx++] =
		    MLX5E_READ_CTR_ATOMIC64(&priv->tls->sw_stats,
					    stats_desc, i);

	return n;
>>>>>>> upstream/android-13
}
