/* SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0 */
/* Copyright (c) 2017-2018 Mellanox Technologies. All rights reserved */

#ifndef _MLXSW_PIPELINE_H_
#define _MLXSW_PIPELINE_H_

<<<<<<< HEAD
#if IS_ENABLED(CONFIG_NET_DEVLINK)

int mlxsw_sp_dpipe_init(struct mlxsw_sp *mlxsw_sp);
void mlxsw_sp_dpipe_fini(struct mlxsw_sp *mlxsw_sp);

#else

static inline int mlxsw_sp_dpipe_init(struct mlxsw_sp *mlxsw_sp)
{
	return 0;
}

static inline void mlxsw_sp_dpipe_fini(struct mlxsw_sp *mlxsw_sp)
{
}

#endif

=======
int mlxsw_sp_dpipe_init(struct mlxsw_sp *mlxsw_sp);
void mlxsw_sp_dpipe_fini(struct mlxsw_sp *mlxsw_sp);

>>>>>>> upstream/android-13
#define MLXSW_SP_DPIPE_TABLE_NAME_ERIF "mlxsw_erif"
#define MLXSW_SP_DPIPE_TABLE_NAME_HOST4 "mlxsw_host4"
#define MLXSW_SP_DPIPE_TABLE_NAME_HOST6 "mlxsw_host6"
#define MLXSW_SP_DPIPE_TABLE_NAME_ADJ "mlxsw_adj"

#endif /* _MLXSW_PIPELINE_H_*/
