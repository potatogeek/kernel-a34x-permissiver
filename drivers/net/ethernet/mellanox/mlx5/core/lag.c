/*
 * Copyright (c) 2016, Mellanox Technologies. All rights reserved.
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

#include <linux/netdevice.h>
#include <linux/mlx5/driver.h>
<<<<<<< HEAD
#include <linux/mlx5/vport.h>
#include "mlx5_core.h"

enum {
	MLX5_LAG_FLAG_BONDED = 1 << 0,
};

struct lag_func {
	struct mlx5_core_dev *dev;
	struct net_device    *netdev;
};

/* Used for collection of netdev event info. */
struct lag_tracker {
	enum   netdev_lag_tx_type           tx_type;
	struct netdev_lag_lower_state_info  netdev_state[MLX5_MAX_PORTS];
	bool is_bonded;
};

/* LAG data of a ConnectX card.
 * It serves both its phys functions.
 */
struct mlx5_lag {
	u8                        flags;
	u8                        v2p_map[MLX5_MAX_PORTS];
	struct lag_func           pf[MLX5_MAX_PORTS];
	struct lag_tracker        tracker;
	struct delayed_work       bond_work;
	struct notifier_block     nb;

	/* Admin state. Allow lag only if allowed is true
	 * even if network conditions for lag were met
	 */
	bool                      allowed;
};
=======
#include <linux/mlx5/eswitch.h>
#include <linux/mlx5/vport.h>
#include "lib/devcom.h"
#include "mlx5_core.h"
#include "eswitch.h"
#include "lag.h"
#include "lag_mp.h"
>>>>>>> upstream/android-13

/* General purpose, use for short periods of time.
 * Beware of lock dependencies (preferably, no locks should be acquired
 * under it).
 */
<<<<<<< HEAD
static DEFINE_MUTEX(lag_mutex);

static int mlx5_cmd_create_lag(struct mlx5_core_dev *dev, u8 remap_port1,
			       u8 remap_port2)
{
	u32   in[MLX5_ST_SZ_DW(create_lag_in)]   = {0};
	u32   out[MLX5_ST_SZ_DW(create_lag_out)] = {0};
=======
static DEFINE_SPINLOCK(lag_lock);

static int mlx5_cmd_create_lag(struct mlx5_core_dev *dev, u8 remap_port1,
			       u8 remap_port2, bool shared_fdb)
{
	u32 in[MLX5_ST_SZ_DW(create_lag_in)] = {};
>>>>>>> upstream/android-13
	void *lag_ctx = MLX5_ADDR_OF(create_lag_in, in, ctx);

	MLX5_SET(create_lag_in, in, opcode, MLX5_CMD_OP_CREATE_LAG);

	MLX5_SET(lagc, lag_ctx, tx_remap_affinity_1, remap_port1);
	MLX5_SET(lagc, lag_ctx, tx_remap_affinity_2, remap_port2);
<<<<<<< HEAD

	return mlx5_cmd_exec(dev, in, sizeof(in), out, sizeof(out));
=======
	MLX5_SET(lagc, lag_ctx, fdb_selection_mode, shared_fdb);

	return mlx5_cmd_exec_in(dev, create_lag, in);
>>>>>>> upstream/android-13
}

static int mlx5_cmd_modify_lag(struct mlx5_core_dev *dev, u8 remap_port1,
			       u8 remap_port2)
{
<<<<<<< HEAD
	u32   in[MLX5_ST_SZ_DW(modify_lag_in)]   = {0};
	u32   out[MLX5_ST_SZ_DW(modify_lag_out)] = {0};
=======
	u32 in[MLX5_ST_SZ_DW(modify_lag_in)] = {};
>>>>>>> upstream/android-13
	void *lag_ctx = MLX5_ADDR_OF(modify_lag_in, in, ctx);

	MLX5_SET(modify_lag_in, in, opcode, MLX5_CMD_OP_MODIFY_LAG);
	MLX5_SET(modify_lag_in, in, field_select, 0x1);

	MLX5_SET(lagc, lag_ctx, tx_remap_affinity_1, remap_port1);
	MLX5_SET(lagc, lag_ctx, tx_remap_affinity_2, remap_port2);

<<<<<<< HEAD
	return mlx5_cmd_exec(dev, in, sizeof(in), out, sizeof(out));
}

static int mlx5_cmd_destroy_lag(struct mlx5_core_dev *dev)
{
	u32  in[MLX5_ST_SZ_DW(destroy_lag_in)]  = {0};
	u32 out[MLX5_ST_SZ_DW(destroy_lag_out)] = {0};

	MLX5_SET(destroy_lag_in, in, opcode, MLX5_CMD_OP_DESTROY_LAG);

	return mlx5_cmd_exec(dev, in, sizeof(in), out, sizeof(out));
=======
	return mlx5_cmd_exec_in(dev, modify_lag, in);
>>>>>>> upstream/android-13
}

int mlx5_cmd_create_vport_lag(struct mlx5_core_dev *dev)
{
<<<<<<< HEAD
	u32  in[MLX5_ST_SZ_DW(create_vport_lag_in)]  = {0};
	u32 out[MLX5_ST_SZ_DW(create_vport_lag_out)] = {0};

	MLX5_SET(create_vport_lag_in, in, opcode, MLX5_CMD_OP_CREATE_VPORT_LAG);

	return mlx5_cmd_exec(dev, in, sizeof(in), out, sizeof(out));
=======
	u32 in[MLX5_ST_SZ_DW(create_vport_lag_in)] = {};

	MLX5_SET(create_vport_lag_in, in, opcode, MLX5_CMD_OP_CREATE_VPORT_LAG);

	return mlx5_cmd_exec_in(dev, create_vport_lag, in);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(mlx5_cmd_create_vport_lag);

int mlx5_cmd_destroy_vport_lag(struct mlx5_core_dev *dev)
{
<<<<<<< HEAD
	u32  in[MLX5_ST_SZ_DW(destroy_vport_lag_in)]  = {0};
	u32 out[MLX5_ST_SZ_DW(destroy_vport_lag_out)] = {0};

	MLX5_SET(destroy_vport_lag_in, in, opcode, MLX5_CMD_OP_DESTROY_VPORT_LAG);

	return mlx5_cmd_exec(dev, in, sizeof(in), out, sizeof(out));
}
EXPORT_SYMBOL(mlx5_cmd_destroy_vport_lag);

static int mlx5_cmd_query_cong_counter(struct mlx5_core_dev *dev,
				       bool reset, void *out, int out_size)
{
	u32 in[MLX5_ST_SZ_DW(query_cong_statistics_in)] = { };

	MLX5_SET(query_cong_statistics_in, in, opcode,
		 MLX5_CMD_OP_QUERY_CONG_STATISTICS);
	MLX5_SET(query_cong_statistics_in, in, clear, reset);
	return mlx5_cmd_exec(dev, in, sizeof(in), out, out_size);
}

static struct mlx5_lag *mlx5_lag_dev_get(struct mlx5_core_dev *dev)
{
	return dev->priv.lag;
}

static int mlx5_lag_dev_get_netdev_idx(struct mlx5_lag *ldev,
				       struct net_device *ndev)
=======
	u32 in[MLX5_ST_SZ_DW(destroy_vport_lag_in)] = {};

	MLX5_SET(destroy_vport_lag_in, in, opcode, MLX5_CMD_OP_DESTROY_VPORT_LAG);

	return mlx5_cmd_exec_in(dev, destroy_vport_lag, in);
}
EXPORT_SYMBOL(mlx5_cmd_destroy_vport_lag);

static int mlx5_lag_netdev_event(struct notifier_block *this,
				 unsigned long event, void *ptr);
static void mlx5_do_bond_work(struct work_struct *work);

static void mlx5_ldev_free(struct kref *ref)
{
	struct mlx5_lag *ldev = container_of(ref, struct mlx5_lag, ref);

	if (ldev->nb.notifier_call)
		unregister_netdevice_notifier_net(&init_net, &ldev->nb);
	mlx5_lag_mp_cleanup(ldev);
	cancel_delayed_work_sync(&ldev->bond_work);
	destroy_workqueue(ldev->wq);
	kfree(ldev);
}

static void mlx5_ldev_put(struct mlx5_lag *ldev)
{
	kref_put(&ldev->ref, mlx5_ldev_free);
}

static void mlx5_ldev_get(struct mlx5_lag *ldev)
{
	kref_get(&ldev->ref);
}

static struct mlx5_lag *mlx5_lag_dev_alloc(struct mlx5_core_dev *dev)
{
	struct mlx5_lag *ldev;
	int err;

	ldev = kzalloc(sizeof(*ldev), GFP_KERNEL);
	if (!ldev)
		return NULL;

	ldev->wq = create_singlethread_workqueue("mlx5_lag");
	if (!ldev->wq) {
		kfree(ldev);
		return NULL;
	}

	kref_init(&ldev->ref);
	INIT_DELAYED_WORK(&ldev->bond_work, mlx5_do_bond_work);

	ldev->nb.notifier_call = mlx5_lag_netdev_event;
	if (register_netdevice_notifier_net(&init_net, &ldev->nb)) {
		ldev->nb.notifier_call = NULL;
		mlx5_core_err(dev, "Failed to register LAG netdev notifier\n");
	}

	err = mlx5_lag_mp_init(ldev);
	if (err)
		mlx5_core_err(dev, "Failed to init multipath lag err=%d\n",
			      err);

	return ldev;
}

int mlx5_lag_dev_get_netdev_idx(struct mlx5_lag *ldev,
				struct net_device *ndev)
>>>>>>> upstream/android-13
{
	int i;

	for (i = 0; i < MLX5_MAX_PORTS; i++)
		if (ldev->pf[i].netdev == ndev)
			return i;

<<<<<<< HEAD
	return -1;
}

static bool mlx5_lag_is_bonded(struct mlx5_lag *ldev)
{
	return !!(ldev->flags & MLX5_LAG_FLAG_BONDED);
=======
	return -ENOENT;
}

static bool __mlx5_lag_is_roce(struct mlx5_lag *ldev)
{
	return !!(ldev->flags & MLX5_LAG_FLAG_ROCE);
}

static bool __mlx5_lag_is_sriov(struct mlx5_lag *ldev)
{
	return !!(ldev->flags & MLX5_LAG_FLAG_SRIOV);
>>>>>>> upstream/android-13
}

static void mlx5_infer_tx_affinity_mapping(struct lag_tracker *tracker,
					   u8 *port1, u8 *port2)
{
<<<<<<< HEAD
	*port1 = 1;
	*port2 = 2;
	if (!tracker->netdev_state[0].tx_enabled ||
	    !tracker->netdev_state[0].link_up) {
		*port1 = 2;
		return;
	}

	if (!tracker->netdev_state[1].tx_enabled ||
	    !tracker->netdev_state[1].link_up)
		*port2 = 1;
}

static void mlx5_activate_lag(struct mlx5_lag *ldev,
			      struct lag_tracker *tracker)
{
	struct mlx5_core_dev *dev0 = ldev->pf[0].dev;
	int err;

	ldev->flags |= MLX5_LAG_FLAG_BONDED;

	mlx5_infer_tx_affinity_mapping(tracker, &ldev->v2p_map[0],
				       &ldev->v2p_map[1]);

	err = mlx5_cmd_create_lag(dev0, ldev->v2p_map[0], ldev->v2p_map[1]);
	if (err)
		mlx5_core_err(dev0,
			      "Failed to create LAG (%d)\n",
			      err);
}

static void mlx5_deactivate_lag(struct mlx5_lag *ldev)
{
	struct mlx5_core_dev *dev0 = ldev->pf[0].dev;
	int err;

	ldev->flags &= ~MLX5_LAG_FLAG_BONDED;

	err = mlx5_cmd_destroy_lag(dev0);
	if (err)
		mlx5_core_err(dev0,
			      "Failed to destroy LAG (%d)\n",
			      err);
=======
	bool p1en;
	bool p2en;

	p1en = tracker->netdev_state[MLX5_LAG_P1].tx_enabled &&
	       tracker->netdev_state[MLX5_LAG_P1].link_up;

	p2en = tracker->netdev_state[MLX5_LAG_P2].tx_enabled &&
	       tracker->netdev_state[MLX5_LAG_P2].link_up;

	*port1 = 1;
	*port2 = 2;
	if ((!p1en && !p2en) || (p1en && p2en))
		return;

	if (p1en)
		*port2 = 1;
	else
		*port1 = 2;
}

void mlx5_modify_lag(struct mlx5_lag *ldev,
		     struct lag_tracker *tracker)
{
	struct mlx5_core_dev *dev0 = ldev->pf[MLX5_LAG_P1].dev;
	u8 v2p_port1, v2p_port2;
	int err;

	mlx5_infer_tx_affinity_mapping(tracker, &v2p_port1,
				       &v2p_port2);

	if (v2p_port1 != ldev->v2p_map[MLX5_LAG_P1] ||
	    v2p_port2 != ldev->v2p_map[MLX5_LAG_P2]) {
		ldev->v2p_map[MLX5_LAG_P1] = v2p_port1;
		ldev->v2p_map[MLX5_LAG_P2] = v2p_port2;

		mlx5_core_info(dev0, "modify lag map port 1:%d port 2:%d",
			       ldev->v2p_map[MLX5_LAG_P1],
			       ldev->v2p_map[MLX5_LAG_P2]);

		err = mlx5_cmd_modify_lag(dev0, v2p_port1, v2p_port2);
		if (err)
			mlx5_core_err(dev0,
				      "Failed to modify LAG (%d)\n",
				      err);
	}
}

static int mlx5_create_lag(struct mlx5_lag *ldev,
			   struct lag_tracker *tracker,
			   bool shared_fdb)
{
	struct mlx5_core_dev *dev0 = ldev->pf[MLX5_LAG_P1].dev;
	struct mlx5_core_dev *dev1 = ldev->pf[MLX5_LAG_P2].dev;
	u32 in[MLX5_ST_SZ_DW(destroy_lag_in)] = {};
	int err;

	mlx5_infer_tx_affinity_mapping(tracker, &ldev->v2p_map[MLX5_LAG_P1],
				       &ldev->v2p_map[MLX5_LAG_P2]);

	mlx5_core_info(dev0, "lag map port 1:%d port 2:%d shared_fdb:%d",
		       ldev->v2p_map[MLX5_LAG_P1], ldev->v2p_map[MLX5_LAG_P2],
		       shared_fdb);

	err = mlx5_cmd_create_lag(dev0, ldev->v2p_map[MLX5_LAG_P1],
				  ldev->v2p_map[MLX5_LAG_P2], shared_fdb);
	if (err) {
		mlx5_core_err(dev0,
			      "Failed to create LAG (%d)\n",
			      err);
		return err;
	}

	if (shared_fdb) {
		err = mlx5_eswitch_offloads_config_single_fdb(dev0->priv.eswitch,
							      dev1->priv.eswitch);
		if (err)
			mlx5_core_err(dev0, "Can't enable single FDB mode\n");
		else
			mlx5_core_info(dev0, "Operation mode is single FDB\n");
	}

	if (err) {
		MLX5_SET(destroy_lag_in, in, opcode, MLX5_CMD_OP_DESTROY_LAG);
		if (mlx5_cmd_exec_in(dev0, destroy_lag, in))
			mlx5_core_err(dev0,
				      "Failed to deactivate RoCE LAG; driver restart required\n");
	}

	return err;
}

int mlx5_activate_lag(struct mlx5_lag *ldev,
		      struct lag_tracker *tracker,
		      u8 flags,
		      bool shared_fdb)
{
	bool roce_lag = !!(flags & MLX5_LAG_FLAG_ROCE);
	struct mlx5_core_dev *dev0 = ldev->pf[MLX5_LAG_P1].dev;
	int err;

	err = mlx5_create_lag(ldev, tracker, shared_fdb);
	if (err) {
		if (roce_lag) {
			mlx5_core_err(dev0,
				      "Failed to activate RoCE LAG\n");
		} else {
			mlx5_core_err(dev0,
				      "Failed to activate VF LAG\n"
				      "Make sure all VFs are unbound prior to VF LAG activation or deactivation\n");
		}
		return err;
	}

	ldev->flags |= flags;
	ldev->shared_fdb = shared_fdb;
	return 0;
}

static int mlx5_deactivate_lag(struct mlx5_lag *ldev)
{
	struct mlx5_core_dev *dev0 = ldev->pf[MLX5_LAG_P1].dev;
	u32 in[MLX5_ST_SZ_DW(destroy_lag_in)] = {};
	bool roce_lag = __mlx5_lag_is_roce(ldev);
	int err;

	ldev->flags &= ~MLX5_LAG_MODE_FLAGS;
	mlx5_lag_mp_reset(ldev);

	if (ldev->shared_fdb) {
		mlx5_eswitch_offloads_destroy_single_fdb(ldev->pf[MLX5_LAG_P1].dev->priv.eswitch,
							 ldev->pf[MLX5_LAG_P2].dev->priv.eswitch);
		ldev->shared_fdb = false;
	}

	MLX5_SET(destroy_lag_in, in, opcode, MLX5_CMD_OP_DESTROY_LAG);
	err = mlx5_cmd_exec_in(dev0, destroy_lag, in);
	if (err) {
		if (roce_lag) {
			mlx5_core_err(dev0,
				      "Failed to deactivate RoCE LAG; driver restart required\n");
		} else {
			mlx5_core_err(dev0,
				      "Failed to deactivate VF LAG; driver restart required\n"
				      "Make sure all VFs are unbound prior to VF LAG activation or deactivation\n");
		}
	}

	return err;
}

static bool mlx5_lag_check_prereq(struct mlx5_lag *ldev)
{
	if (!ldev->pf[MLX5_LAG_P1].dev || !ldev->pf[MLX5_LAG_P2].dev)
		return false;

#ifdef CONFIG_MLX5_ESWITCH
	return mlx5_esw_lag_prereq(ldev->pf[MLX5_LAG_P1].dev,
				   ldev->pf[MLX5_LAG_P2].dev);
#else
	return (!mlx5_sriov_is_enabled(ldev->pf[MLX5_LAG_P1].dev) &&
		!mlx5_sriov_is_enabled(ldev->pf[MLX5_LAG_P2].dev));
#endif
}

static void mlx5_lag_add_devices(struct mlx5_lag *ldev)
{
	int i;

	for (i = 0; i < MLX5_MAX_PORTS; i++) {
		if (!ldev->pf[i].dev)
			continue;

		if (ldev->pf[i].dev->priv.flags &
		    MLX5_PRIV_FLAGS_DISABLE_ALL_ADEV)
			continue;

		ldev->pf[i].dev->priv.flags &= ~MLX5_PRIV_FLAGS_DISABLE_IB_ADEV;
		mlx5_rescan_drivers_locked(ldev->pf[i].dev);
	}
}

static void mlx5_lag_remove_devices(struct mlx5_lag *ldev)
{
	int i;

	for (i = 0; i < MLX5_MAX_PORTS; i++) {
		if (!ldev->pf[i].dev)
			continue;

		if (ldev->pf[i].dev->priv.flags &
		    MLX5_PRIV_FLAGS_DISABLE_ALL_ADEV)
			continue;

		ldev->pf[i].dev->priv.flags |= MLX5_PRIV_FLAGS_DISABLE_IB_ADEV;
		mlx5_rescan_drivers_locked(ldev->pf[i].dev);
	}
}

static void mlx5_disable_lag(struct mlx5_lag *ldev)
{
	struct mlx5_core_dev *dev0 = ldev->pf[MLX5_LAG_P1].dev;
	struct mlx5_core_dev *dev1 = ldev->pf[MLX5_LAG_P2].dev;
	bool shared_fdb = ldev->shared_fdb;
	bool roce_lag;
	int err;

	roce_lag = __mlx5_lag_is_roce(ldev);

	if (shared_fdb) {
		mlx5_lag_remove_devices(ldev);
	} else if (roce_lag) {
		if (!(dev0->priv.flags & MLX5_PRIV_FLAGS_DISABLE_ALL_ADEV)) {
			dev0->priv.flags |= MLX5_PRIV_FLAGS_DISABLE_IB_ADEV;
			mlx5_rescan_drivers_locked(dev0);
		}
		mlx5_nic_vport_disable_roce(dev1);
	}

	err = mlx5_deactivate_lag(ldev);
	if (err)
		return;

	if (shared_fdb || roce_lag)
		mlx5_lag_add_devices(ldev);

	if (shared_fdb) {
		if (!(dev0->priv.flags & MLX5_PRIV_FLAGS_DISABLE_ALL_ADEV))
			mlx5_eswitch_reload_reps(dev0->priv.eswitch);
		if (!(dev1->priv.flags & MLX5_PRIV_FLAGS_DISABLE_ALL_ADEV))
			mlx5_eswitch_reload_reps(dev1->priv.eswitch);
	}
}

static bool mlx5_shared_fdb_supported(struct mlx5_lag *ldev)
{
	struct mlx5_core_dev *dev0 = ldev->pf[MLX5_LAG_P1].dev;
	struct mlx5_core_dev *dev1 = ldev->pf[MLX5_LAG_P2].dev;

	if (is_mdev_switchdev_mode(dev0) &&
	    is_mdev_switchdev_mode(dev1) &&
	    mlx5_eswitch_vport_match_metadata_enabled(dev0->priv.eswitch) &&
	    mlx5_eswitch_vport_match_metadata_enabled(dev1->priv.eswitch) &&
	    mlx5_devcom_is_paired(dev0->priv.devcom,
				  MLX5_DEVCOM_ESW_OFFLOADS) &&
	    MLX5_CAP_GEN(dev1, lag_native_fdb_selection) &&
	    MLX5_CAP_ESW(dev1, root_ft_on_other_esw) &&
	    MLX5_CAP_ESW(dev0, esw_shared_ingress_acl))
		return true;

	return false;
>>>>>>> upstream/android-13
}

static void mlx5_do_bond(struct mlx5_lag *ldev)
{
<<<<<<< HEAD
	struct mlx5_core_dev *dev0 = ldev->pf[0].dev;
	struct mlx5_core_dev *dev1 = ldev->pf[1].dev;
	struct lag_tracker tracker;
	u8 v2p_port1, v2p_port2;
	int i, err;
	bool do_bond;

	if (!dev0 || !dev1)
		return;

	mutex_lock(&lag_mutex);
	tracker = ldev->tracker;
	mutex_unlock(&lag_mutex);

	do_bond = tracker.is_bonded && ldev->allowed;

	if (do_bond && !mlx5_lag_is_bonded(ldev)) {
		for (i = 0; i < MLX5_MAX_PORTS; i++)
			mlx5_remove_dev_by_protocol(ldev->pf[i].dev,
						    MLX5_INTERFACE_PROTOCOL_IB);

		mlx5_activate_lag(ldev, &tracker);

		mlx5_add_dev_by_protocol(dev0, MLX5_INTERFACE_PROTOCOL_IB);
		mlx5_nic_vport_enable_roce(dev1);
	} else if (do_bond && mlx5_lag_is_bonded(ldev)) {
		mlx5_infer_tx_affinity_mapping(&tracker, &v2p_port1,
					       &v2p_port2);

		if ((v2p_port1 != ldev->v2p_map[0]) ||
		    (v2p_port2 != ldev->v2p_map[1])) {
			ldev->v2p_map[0] = v2p_port1;
			ldev->v2p_map[1] = v2p_port2;

			err = mlx5_cmd_modify_lag(dev0, v2p_port1, v2p_port2);
			if (err)
				mlx5_core_err(dev0,
					      "Failed to modify LAG (%d)\n",
					      err);
		}
	} else if (!do_bond && mlx5_lag_is_bonded(ldev)) {
		mlx5_remove_dev_by_protocol(dev0, MLX5_INTERFACE_PROTOCOL_IB);
		mlx5_nic_vport_disable_roce(dev1);

		mlx5_deactivate_lag(ldev);

		for (i = 0; i < MLX5_MAX_PORTS; i++)
			if (ldev->pf[i].dev)
				mlx5_add_dev_by_protocol(ldev->pf[i].dev,
							 MLX5_INTERFACE_PROTOCOL_IB);
=======
	struct mlx5_core_dev *dev0 = ldev->pf[MLX5_LAG_P1].dev;
	struct mlx5_core_dev *dev1 = ldev->pf[MLX5_LAG_P2].dev;
	struct lag_tracker tracker;
	bool do_bond, roce_lag;
	int err;

	if (!mlx5_lag_is_ready(ldev)) {
		do_bond = false;
	} else {
		/* VF LAG is in multipath mode, ignore bond change requests */
		if (mlx5_lag_is_multipath(dev0))
			return;

		tracker = ldev->tracker;

		do_bond = tracker.is_bonded && mlx5_lag_check_prereq(ldev);
	}

	if (do_bond && !__mlx5_lag_is_active(ldev)) {
		bool shared_fdb = mlx5_shared_fdb_supported(ldev);

		roce_lag = !mlx5_sriov_is_enabled(dev0) &&
			   !mlx5_sriov_is_enabled(dev1);

#ifdef CONFIG_MLX5_ESWITCH
		roce_lag = roce_lag &&
			   dev0->priv.eswitch->mode == MLX5_ESWITCH_NONE &&
			   dev1->priv.eswitch->mode == MLX5_ESWITCH_NONE;
#endif

		if (shared_fdb || roce_lag)
			mlx5_lag_remove_devices(ldev);

		err = mlx5_activate_lag(ldev, &tracker,
					roce_lag ? MLX5_LAG_FLAG_ROCE :
						   MLX5_LAG_FLAG_SRIOV,
					shared_fdb);
		if (err) {
			if (shared_fdb || roce_lag)
				mlx5_lag_add_devices(ldev);

			return;
		} else if (roce_lag) {
			dev0->priv.flags &= ~MLX5_PRIV_FLAGS_DISABLE_IB_ADEV;
			mlx5_rescan_drivers_locked(dev0);
			mlx5_nic_vport_enable_roce(dev1);
		} else if (shared_fdb) {
			dev0->priv.flags &= ~MLX5_PRIV_FLAGS_DISABLE_IB_ADEV;
			mlx5_rescan_drivers_locked(dev0);

			err = mlx5_eswitch_reload_reps(dev0->priv.eswitch);
			if (!err)
				err = mlx5_eswitch_reload_reps(dev1->priv.eswitch);

			if (err) {
				dev0->priv.flags |= MLX5_PRIV_FLAGS_DISABLE_IB_ADEV;
				mlx5_rescan_drivers_locked(dev0);
				mlx5_deactivate_lag(ldev);
				mlx5_lag_add_devices(ldev);
				mlx5_eswitch_reload_reps(dev0->priv.eswitch);
				mlx5_eswitch_reload_reps(dev1->priv.eswitch);
				mlx5_core_err(dev0, "Failed to enable lag\n");
				return;
			}
		}
	} else if (do_bond && __mlx5_lag_is_active(ldev)) {
		mlx5_modify_lag(ldev, &tracker);
	} else if (!do_bond && __mlx5_lag_is_active(ldev)) {
		mlx5_disable_lag(ldev);
>>>>>>> upstream/android-13
	}
}

static void mlx5_queue_bond_work(struct mlx5_lag *ldev, unsigned long delay)
{
<<<<<<< HEAD
	schedule_delayed_work(&ldev->bond_work, delay);
=======
	queue_delayed_work(ldev->wq, &ldev->bond_work, delay);
}

static void mlx5_lag_lock_eswitches(struct mlx5_core_dev *dev0,
				    struct mlx5_core_dev *dev1)
{
	if (dev0)
		mlx5_esw_lock(dev0->priv.eswitch);
	if (dev1)
		mlx5_esw_lock(dev1->priv.eswitch);
}

static void mlx5_lag_unlock_eswitches(struct mlx5_core_dev *dev0,
				      struct mlx5_core_dev *dev1)
{
	if (dev1)
		mlx5_esw_unlock(dev1->priv.eswitch);
	if (dev0)
		mlx5_esw_unlock(dev0->priv.eswitch);
>>>>>>> upstream/android-13
}

static void mlx5_do_bond_work(struct work_struct *work)
{
	struct delayed_work *delayed_work = to_delayed_work(work);
	struct mlx5_lag *ldev = container_of(delayed_work, struct mlx5_lag,
					     bond_work);
<<<<<<< HEAD
=======
	struct mlx5_core_dev *dev0 = ldev->pf[MLX5_LAG_P1].dev;
	struct mlx5_core_dev *dev1 = ldev->pf[MLX5_LAG_P2].dev;
>>>>>>> upstream/android-13
	int status;

	status = mlx5_dev_list_trylock();
	if (!status) {
<<<<<<< HEAD
		/* 1 sec delay. */
=======
>>>>>>> upstream/android-13
		mlx5_queue_bond_work(ldev, HZ);
		return;
	}

<<<<<<< HEAD
	mlx5_do_bond(ldev);
=======
	if (ldev->mode_changes_in_progress) {
		mlx5_dev_list_unlock();
		mlx5_queue_bond_work(ldev, HZ);
		return;
	}

	mlx5_lag_lock_eswitches(dev0, dev1);
	mlx5_do_bond(ldev);
	mlx5_lag_unlock_eswitches(dev0, dev1);
>>>>>>> upstream/android-13
	mlx5_dev_list_unlock();
}

static int mlx5_handle_changeupper_event(struct mlx5_lag *ldev,
					 struct lag_tracker *tracker,
					 struct net_device *ndev,
					 struct netdev_notifier_changeupper_info *info)
{
	struct net_device *upper = info->upper_dev, *ndev_tmp;
	struct netdev_lag_upper_info *lag_upper_info = NULL;
<<<<<<< HEAD
	bool is_bonded;
	int bond_status = 0;
	int num_slaves = 0;
=======
	bool is_bonded, is_in_lag, mode_supported;
	int bond_status = 0;
	int num_slaves = 0;
	int changed = 0;
>>>>>>> upstream/android-13
	int idx;

	if (!netif_is_lag_master(upper))
		return 0;

	if (info->linking)
		lag_upper_info = info->upper_info;

	/* The event may still be of interest if the slave does not belong to
	 * us, but is enslaved to a master which has one or more of our netdevs
	 * as slaves (e.g., if a new slave is added to a master that bonds two
	 * of our netdevs, we should unbond).
	 */
	rcu_read_lock();
	for_each_netdev_in_bond_rcu(upper, ndev_tmp) {
		idx = mlx5_lag_dev_get_netdev_idx(ldev, ndev_tmp);
<<<<<<< HEAD
		if (idx > -1)
=======
		if (idx >= 0)
>>>>>>> upstream/android-13
			bond_status |= (1 << idx);

		num_slaves++;
	}
	rcu_read_unlock();

	/* None of this lagdev's netdevs are slaves of this master. */
	if (!(bond_status & 0x3))
		return 0;

	if (lag_upper_info)
		tracker->tx_type = lag_upper_info->tx_type;

	/* Determine bonding status:
	 * A device is considered bonded if both its physical ports are slaves
	 * of the same lag master, and only them.
<<<<<<< HEAD
	 * Lag mode must be activebackup or hash.
	 */
	is_bonded = (num_slaves == MLX5_MAX_PORTS) &&
		    (bond_status == 0x3) &&
		    ((tracker->tx_type == NETDEV_LAG_TX_TYPE_ACTIVEBACKUP) ||
		     (tracker->tx_type == NETDEV_LAG_TX_TYPE_HASH));

	if (tracker->is_bonded != is_bonded) {
		tracker->is_bonded = is_bonded;
		return 1;
	}

	return 0;
=======
	 */
	is_in_lag = num_slaves == MLX5_MAX_PORTS && bond_status == 0x3;

	/* Lag mode must be activebackup or hash. */
	mode_supported = tracker->tx_type == NETDEV_LAG_TX_TYPE_ACTIVEBACKUP ||
			 tracker->tx_type == NETDEV_LAG_TX_TYPE_HASH;

	is_bonded = is_in_lag && mode_supported;
	if (tracker->is_bonded != is_bonded) {
		tracker->is_bonded = is_bonded;
		changed = 1;
	}

	if (!is_in_lag)
		return changed;

	if (!mlx5_lag_is_ready(ldev))
		NL_SET_ERR_MSG_MOD(info->info.extack,
				   "Can't activate LAG offload, PF is configured with more than 64 VFs");
	else if (!mode_supported)
		NL_SET_ERR_MSG_MOD(info->info.extack,
				   "Can't activate LAG offload, TX type isn't supported");

	return changed;
>>>>>>> upstream/android-13
}

static int mlx5_handle_changelowerstate_event(struct mlx5_lag *ldev,
					      struct lag_tracker *tracker,
					      struct net_device *ndev,
					      struct netdev_notifier_changelowerstate_info *info)
{
	struct netdev_lag_lower_state_info *lag_lower_info;
	int idx;

	if (!netif_is_lag_port(ndev))
		return 0;

	idx = mlx5_lag_dev_get_netdev_idx(ldev, ndev);
<<<<<<< HEAD
	if (idx == -1)
=======
	if (idx < 0)
>>>>>>> upstream/android-13
		return 0;

	/* This information is used to determine virtual to physical
	 * port mapping.
	 */
	lag_lower_info = info->lower_state_info;
	if (!lag_lower_info)
		return 0;

	tracker->netdev_state[idx] = *lag_lower_info;

	return 1;
}

static int mlx5_lag_netdev_event(struct notifier_block *this,
				 unsigned long event, void *ptr)
{
	struct net_device *ndev = netdev_notifier_info_to_dev(ptr);
	struct lag_tracker tracker;
	struct mlx5_lag *ldev;
	int changed = 0;

<<<<<<< HEAD
	if (!net_eq(dev_net(ndev), &init_net))
		return NOTIFY_DONE;

=======
>>>>>>> upstream/android-13
	if ((event != NETDEV_CHANGEUPPER) && (event != NETDEV_CHANGELOWERSTATE))
		return NOTIFY_DONE;

	ldev    = container_of(this, struct mlx5_lag, nb);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	tracker = ldev->tracker;

	switch (event) {
	case NETDEV_CHANGEUPPER:
		changed = mlx5_handle_changeupper_event(ldev, &tracker, ndev,
							ptr);
		break;
	case NETDEV_CHANGELOWERSTATE:
		changed = mlx5_handle_changelowerstate_event(ldev, &tracker,
							     ndev, ptr);
		break;
	}

<<<<<<< HEAD
	mutex_lock(&lag_mutex);
	ldev->tracker = tracker;
	mutex_unlock(&lag_mutex);
=======
	ldev->tracker = tracker;
>>>>>>> upstream/android-13

	if (changed)
		mlx5_queue_bond_work(ldev, 0);

	return NOTIFY_DONE;
}

<<<<<<< HEAD
static bool mlx5_lag_check_prereq(struct mlx5_lag *ldev)
{
	if ((ldev->pf[0].dev && mlx5_sriov_is_enabled(ldev->pf[0].dev)) ||
	    (ldev->pf[1].dev && mlx5_sriov_is_enabled(ldev->pf[1].dev)))
		return false;
	else
		return true;
}

static struct mlx5_lag *mlx5_lag_dev_alloc(void)
{
	struct mlx5_lag *ldev;

	ldev = kzalloc(sizeof(*ldev), GFP_KERNEL);
	if (!ldev)
		return NULL;

	INIT_DELAYED_WORK(&ldev->bond_work, mlx5_do_bond_work);
	ldev->allowed = mlx5_lag_check_prereq(ldev);

	return ldev;
}

static void mlx5_lag_dev_free(struct mlx5_lag *ldev)
{
	kfree(ldev);
}

static void mlx5_lag_dev_add_pf(struct mlx5_lag *ldev,
				struct mlx5_core_dev *dev,
				struct net_device *netdev)
=======
static void mlx5_ldev_add_netdev(struct mlx5_lag *ldev,
				 struct mlx5_core_dev *dev,
				 struct net_device *netdev)
>>>>>>> upstream/android-13
{
	unsigned int fn = PCI_FUNC(dev->pdev->devfn);

	if (fn >= MLX5_MAX_PORTS)
		return;

<<<<<<< HEAD
	mutex_lock(&lag_mutex);
	ldev->pf[fn].dev    = dev;
	ldev->pf[fn].netdev = netdev;
	ldev->tracker.netdev_state[fn].link_up = 0;
	ldev->tracker.netdev_state[fn].tx_enabled = 0;

	ldev->allowed = mlx5_lag_check_prereq(ldev);
	dev->priv.lag = ldev;

	mutex_unlock(&lag_mutex);
}

static void mlx5_lag_dev_remove_pf(struct mlx5_lag *ldev,
				   struct mlx5_core_dev *dev)
=======
	spin_lock(&lag_lock);
	ldev->pf[fn].netdev = netdev;
	ldev->tracker.netdev_state[fn].link_up = 0;
	ldev->tracker.netdev_state[fn].tx_enabled = 0;
	spin_unlock(&lag_lock);
}

static void mlx5_ldev_remove_netdev(struct mlx5_lag *ldev,
				    struct net_device *netdev)
{
	int i;

	spin_lock(&lag_lock);
	for (i = 0; i < MLX5_MAX_PORTS; i++) {
		if (ldev->pf[i].netdev == netdev) {
			ldev->pf[i].netdev = NULL;
			break;
		}
	}
	spin_unlock(&lag_lock);
}

static void mlx5_ldev_add_mdev(struct mlx5_lag *ldev,
			       struct mlx5_core_dev *dev)
{
	unsigned int fn = PCI_FUNC(dev->pdev->devfn);

	if (fn >= MLX5_MAX_PORTS)
		return;

	ldev->pf[fn].dev = dev;
	dev->priv.lag = ldev;
}

/* Must be called with intf_mutex held */
static void mlx5_ldev_remove_mdev(struct mlx5_lag *ldev,
				  struct mlx5_core_dev *dev)
>>>>>>> upstream/android-13
{
	int i;

	for (i = 0; i < MLX5_MAX_PORTS; i++)
		if (ldev->pf[i].dev == dev)
			break;

	if (i == MLX5_MAX_PORTS)
		return;

<<<<<<< HEAD
	mutex_lock(&lag_mutex);
	memset(&ldev->pf[i], 0, sizeof(*ldev->pf));

	dev->priv.lag = NULL;
	ldev->allowed = mlx5_lag_check_prereq(ldev);
	mutex_unlock(&lag_mutex);
}

/* Must be called with intf_mutex held */
void mlx5_lag_add(struct mlx5_core_dev *dev, struct net_device *netdev)
=======
	ldev->pf[i].dev = NULL;
	dev->priv.lag = NULL;
}

/* Must be called with intf_mutex held */
static int __mlx5_lag_dev_add_mdev(struct mlx5_core_dev *dev)
>>>>>>> upstream/android-13
{
	struct mlx5_lag *ldev = NULL;
	struct mlx5_core_dev *tmp_dev;

	if (!MLX5_CAP_GEN(dev, vport_group_manager) ||
	    !MLX5_CAP_GEN(dev, lag_master) ||
<<<<<<< HEAD
	    (MLX5_CAP_GEN(dev, num_lag_ports) != MLX5_MAX_PORTS))
		return;
=======
	    MLX5_CAP_GEN(dev, num_lag_ports) != MLX5_MAX_PORTS)
		return 0;
>>>>>>> upstream/android-13

	tmp_dev = mlx5_get_next_phys_dev(dev);
	if (tmp_dev)
		ldev = tmp_dev->priv.lag;

	if (!ldev) {
<<<<<<< HEAD
		ldev = mlx5_lag_dev_alloc();
		if (!ldev) {
			mlx5_core_err(dev, "Failed to alloc lag dev\n");
			return;
		}
	}

	mlx5_lag_dev_add_pf(ldev, dev, netdev);

	if (!ldev->nb.notifier_call) {
		ldev->nb.notifier_call = mlx5_lag_netdev_event;
		if (register_netdevice_notifier(&ldev->nb)) {
			ldev->nb.notifier_call = NULL;
			mlx5_core_err(dev, "Failed to register LAG netdev notifier\n");
		}
	}
}

/* Must be called with intf_mutex held */
void mlx5_lag_remove(struct mlx5_core_dev *dev)
=======
		ldev = mlx5_lag_dev_alloc(dev);
		if (!ldev) {
			mlx5_core_err(dev, "Failed to alloc lag dev\n");
			return 0;
		}
	} else {
		if (ldev->mode_changes_in_progress)
			return -EAGAIN;
		mlx5_ldev_get(ldev);
	}

	mlx5_ldev_add_mdev(ldev, dev);

	return 0;
}

void mlx5_lag_remove_mdev(struct mlx5_core_dev *dev)
{
	struct mlx5_lag *ldev;

	ldev = mlx5_lag_dev(dev);
	if (!ldev)
		return;

recheck:
	mlx5_dev_list_lock();
	if (ldev->mode_changes_in_progress) {
		mlx5_dev_list_unlock();
		msleep(100);
		goto recheck;
	}
	mlx5_ldev_remove_mdev(ldev, dev);
	mlx5_dev_list_unlock();
	mlx5_ldev_put(ldev);
}

void mlx5_lag_add_mdev(struct mlx5_core_dev *dev)
{
	int err;

recheck:
	mlx5_dev_list_lock();
	err = __mlx5_lag_dev_add_mdev(dev);
	if (err) {
		mlx5_dev_list_unlock();
		msleep(100);
		goto recheck;
	}
	mlx5_dev_list_unlock();
}

/* Must be called with intf_mutex held */
void mlx5_lag_remove_netdev(struct mlx5_core_dev *dev,
			    struct net_device *netdev)
{
	struct mlx5_lag *ldev;

	ldev = mlx5_lag_dev(dev);
	if (!ldev)
		return;

	mlx5_ldev_remove_netdev(ldev, netdev);
	ldev->flags &= ~MLX5_LAG_FLAG_READY;

	if (__mlx5_lag_is_active(ldev))
		mlx5_queue_bond_work(ldev, 0);
}

/* Must be called with intf_mutex held */
void mlx5_lag_add_netdev(struct mlx5_core_dev *dev,
			 struct net_device *netdev)
>>>>>>> upstream/android-13
{
	struct mlx5_lag *ldev;
	int i;

<<<<<<< HEAD
	ldev = mlx5_lag_dev_get(dev);
	if (!ldev)
		return;

	if (mlx5_lag_is_bonded(ldev))
		mlx5_deactivate_lag(ldev);

	mlx5_lag_dev_remove_pf(ldev, dev);

	for (i = 0; i < MLX5_MAX_PORTS; i++)
		if (ldev->pf[i].dev)
			break;

	if (i == MLX5_MAX_PORTS) {
		if (ldev->nb.notifier_call)
			unregister_netdevice_notifier(&ldev->nb);
		cancel_delayed_work_sync(&ldev->bond_work);
		mlx5_lag_dev_free(ldev);
	}
}

=======
	ldev = mlx5_lag_dev(dev);
	if (!ldev)
		return;

	mlx5_ldev_add_netdev(ldev, dev, netdev);

	for (i = 0; i < MLX5_MAX_PORTS; i++)
		if (!ldev->pf[i].dev)
			break;

	if (i >= MLX5_MAX_PORTS)
		ldev->flags |= MLX5_LAG_FLAG_READY;
	mlx5_queue_bond_work(ldev, 0);
}

bool mlx5_lag_is_roce(struct mlx5_core_dev *dev)
{
	struct mlx5_lag *ldev;
	bool res;

	spin_lock(&lag_lock);
	ldev = mlx5_lag_dev(dev);
	res  = ldev && __mlx5_lag_is_roce(ldev);
	spin_unlock(&lag_lock);

	return res;
}
EXPORT_SYMBOL(mlx5_lag_is_roce);

>>>>>>> upstream/android-13
bool mlx5_lag_is_active(struct mlx5_core_dev *dev)
{
	struct mlx5_lag *ldev;
	bool res;

<<<<<<< HEAD
	mutex_lock(&lag_mutex);
	ldev = mlx5_lag_dev_get(dev);
	res  = ldev && mlx5_lag_is_bonded(ldev);
	mutex_unlock(&lag_mutex);
=======
	spin_lock(&lag_lock);
	ldev = mlx5_lag_dev(dev);
	res  = ldev && __mlx5_lag_is_active(ldev);
	spin_unlock(&lag_lock);
>>>>>>> upstream/android-13

	return res;
}
EXPORT_SYMBOL(mlx5_lag_is_active);

<<<<<<< HEAD
static int mlx5_lag_set_state(struct mlx5_core_dev *dev, bool allow)
{
	struct mlx5_lag *ldev;
	int ret = 0;
	bool lag_active;

	mlx5_dev_list_lock();

	ldev = mlx5_lag_dev_get(dev);
	if (!ldev) {
		ret = -ENODEV;
		goto unlock;
	}
	lag_active = mlx5_lag_is_bonded(ldev);
	if (!mlx5_lag_check_prereq(ldev) && allow) {
		ret = -EINVAL;
		goto unlock;
	}
	if (ldev->allowed == allow)
		goto unlock;
	ldev->allowed = allow;
	if ((lag_active && !allow) || allow)
		mlx5_do_bond(ldev);
unlock:
	mlx5_dev_list_unlock();
	return ret;
}

int mlx5_lag_forbid(struct mlx5_core_dev *dev)
{
	return mlx5_lag_set_state(dev, false);
}

int mlx5_lag_allow(struct mlx5_core_dev *dev)
{
	return mlx5_lag_set_state(dev, true);
=======
bool mlx5_lag_is_master(struct mlx5_core_dev *dev)
{
	struct mlx5_lag *ldev;
	bool res;

	spin_lock(&lag_lock);
	ldev = mlx5_lag_dev(dev);
	res = ldev && __mlx5_lag_is_active(ldev) &&
		dev == ldev->pf[MLX5_LAG_P1].dev;
	spin_unlock(&lag_lock);

	return res;
}
EXPORT_SYMBOL(mlx5_lag_is_master);

bool mlx5_lag_is_sriov(struct mlx5_core_dev *dev)
{
	struct mlx5_lag *ldev;
	bool res;

	spin_lock(&lag_lock);
	ldev = mlx5_lag_dev(dev);
	res  = ldev && __mlx5_lag_is_sriov(ldev);
	spin_unlock(&lag_lock);

	return res;
}
EXPORT_SYMBOL(mlx5_lag_is_sriov);

bool mlx5_lag_is_shared_fdb(struct mlx5_core_dev *dev)
{
	struct mlx5_lag *ldev;
	bool res;

	spin_lock(&lag_lock);
	ldev = mlx5_lag_dev(dev);
	res = ldev && __mlx5_lag_is_sriov(ldev) && ldev->shared_fdb;
	spin_unlock(&lag_lock);

	return res;
}
EXPORT_SYMBOL(mlx5_lag_is_shared_fdb);

void mlx5_lag_disable_change(struct mlx5_core_dev *dev)
{
	struct mlx5_core_dev *dev0;
	struct mlx5_core_dev *dev1;
	struct mlx5_lag *ldev;

	ldev = mlx5_lag_dev(dev);
	if (!ldev)
		return;

	mlx5_dev_list_lock();

	dev0 = ldev->pf[MLX5_LAG_P1].dev;
	dev1 = ldev->pf[MLX5_LAG_P2].dev;

	ldev->mode_changes_in_progress++;
	if (__mlx5_lag_is_active(ldev)) {
		mlx5_lag_lock_eswitches(dev0, dev1);
		mlx5_disable_lag(ldev);
		mlx5_lag_unlock_eswitches(dev0, dev1);
	}
	mlx5_dev_list_unlock();
}

void mlx5_lag_enable_change(struct mlx5_core_dev *dev)
{
	struct mlx5_lag *ldev;

	ldev = mlx5_lag_dev(dev);
	if (!ldev)
		return;

	mlx5_dev_list_lock();
	ldev->mode_changes_in_progress--;
	mlx5_dev_list_unlock();
	mlx5_queue_bond_work(ldev, 0);
>>>>>>> upstream/android-13
}

struct net_device *mlx5_lag_get_roce_netdev(struct mlx5_core_dev *dev)
{
	struct net_device *ndev = NULL;
	struct mlx5_lag *ldev;

<<<<<<< HEAD
	mutex_lock(&lag_mutex);
	ldev = mlx5_lag_dev_get(dev);

	if (!(ldev && mlx5_lag_is_bonded(ldev)))
		goto unlock;

	if (ldev->tracker.tx_type == NETDEV_LAG_TX_TYPE_ACTIVEBACKUP) {
		ndev = ldev->tracker.netdev_state[0].tx_enabled ?
		       ldev->pf[0].netdev : ldev->pf[1].netdev;
	} else {
		ndev = ldev->pf[0].netdev;
=======
	spin_lock(&lag_lock);
	ldev = mlx5_lag_dev(dev);

	if (!(ldev && __mlx5_lag_is_roce(ldev)))
		goto unlock;

	if (ldev->tracker.tx_type == NETDEV_LAG_TX_TYPE_ACTIVEBACKUP) {
		ndev = ldev->tracker.netdev_state[MLX5_LAG_P1].tx_enabled ?
		       ldev->pf[MLX5_LAG_P1].netdev :
		       ldev->pf[MLX5_LAG_P2].netdev;
	} else {
		ndev = ldev->pf[MLX5_LAG_P1].netdev;
>>>>>>> upstream/android-13
	}
	if (ndev)
		dev_hold(ndev);

unlock:
<<<<<<< HEAD
	mutex_unlock(&lag_mutex);
=======
	spin_unlock(&lag_lock);
>>>>>>> upstream/android-13

	return ndev;
}
EXPORT_SYMBOL(mlx5_lag_get_roce_netdev);

<<<<<<< HEAD
bool mlx5_lag_intf_add(struct mlx5_interface *intf, struct mlx5_priv *priv)
{
	struct mlx5_core_dev *dev = container_of(priv, struct mlx5_core_dev,
						 priv);
	struct mlx5_lag *ldev;

	if (intf->protocol != MLX5_INTERFACE_PROTOCOL_IB)
		return true;

	ldev = mlx5_lag_dev_get(dev);
	if (!ldev || !mlx5_lag_is_bonded(ldev) || ldev->pf[0].dev == dev)
		return true;

	/* If bonded, we do not add an IB device for PF1. */
	return false;
}
=======
u8 mlx5_lag_get_slave_port(struct mlx5_core_dev *dev,
			   struct net_device *slave)
{
	struct mlx5_lag *ldev;
	u8 port = 0;

	spin_lock(&lag_lock);
	ldev = mlx5_lag_dev(dev);
	if (!(ldev && __mlx5_lag_is_roce(ldev)))
		goto unlock;

	if (ldev->pf[MLX5_LAG_P1].netdev == slave)
		port = MLX5_LAG_P1;
	else
		port = MLX5_LAG_P2;

	port = ldev->v2p_map[port];

unlock:
	spin_unlock(&lag_lock);
	return port;
}
EXPORT_SYMBOL(mlx5_lag_get_slave_port);

struct mlx5_core_dev *mlx5_lag_get_peer_mdev(struct mlx5_core_dev *dev)
{
	struct mlx5_core_dev *peer_dev = NULL;
	struct mlx5_lag *ldev;

	spin_lock(&lag_lock);
	ldev = mlx5_lag_dev(dev);
	if (!ldev)
		goto unlock;

	peer_dev = ldev->pf[MLX5_LAG_P1].dev == dev ?
			   ldev->pf[MLX5_LAG_P2].dev :
			   ldev->pf[MLX5_LAG_P1].dev;

unlock:
	spin_unlock(&lag_lock);
	return peer_dev;
}
EXPORT_SYMBOL(mlx5_lag_get_peer_mdev);
>>>>>>> upstream/android-13

int mlx5_lag_query_cong_counters(struct mlx5_core_dev *dev,
				 u64 *values,
				 int num_counters,
				 size_t *offsets)
{
	int outlen = MLX5_ST_SZ_BYTES(query_cong_statistics_out);
	struct mlx5_core_dev *mdev[MLX5_MAX_PORTS];
	struct mlx5_lag *ldev;
	int num_ports;
	int ret, i, j;
	void *out;

	out = kvzalloc(outlen, GFP_KERNEL);
	if (!out)
		return -ENOMEM;

	memset(values, 0, sizeof(*values) * num_counters);

<<<<<<< HEAD
	mutex_lock(&lag_mutex);
	ldev = mlx5_lag_dev_get(dev);
	if (ldev && mlx5_lag_is_bonded(ldev)) {
		num_ports = MLX5_MAX_PORTS;
		mdev[0] = ldev->pf[0].dev;
		mdev[1] = ldev->pf[1].dev;
	} else {
		num_ports = 1;
		mdev[0] = dev;
	}

	for (i = 0; i < num_ports; ++i) {
		ret = mlx5_cmd_query_cong_counter(mdev[i], false, out, outlen);
		if (ret)
			goto unlock;
=======
	spin_lock(&lag_lock);
	ldev = mlx5_lag_dev(dev);
	if (ldev && __mlx5_lag_is_active(ldev)) {
		num_ports = MLX5_MAX_PORTS;
		mdev[MLX5_LAG_P1] = ldev->pf[MLX5_LAG_P1].dev;
		mdev[MLX5_LAG_P2] = ldev->pf[MLX5_LAG_P2].dev;
	} else {
		num_ports = 1;
		mdev[MLX5_LAG_P1] = dev;
	}
	spin_unlock(&lag_lock);

	for (i = 0; i < num_ports; ++i) {
		u32 in[MLX5_ST_SZ_DW(query_cong_statistics_in)] = {};

		MLX5_SET(query_cong_statistics_in, in, opcode,
			 MLX5_CMD_OP_QUERY_CONG_STATISTICS);
		ret = mlx5_cmd_exec_inout(mdev[i], query_cong_statistics, in,
					  out);
		if (ret)
			goto free;
>>>>>>> upstream/android-13

		for (j = 0; j < num_counters; ++j)
			values[j] += be64_to_cpup((__be64 *)(out + offsets[j]));
	}

<<<<<<< HEAD
unlock:
	mutex_unlock(&lag_mutex);
=======
free:
>>>>>>> upstream/android-13
	kvfree(out);
	return ret;
}
EXPORT_SYMBOL(mlx5_lag_query_cong_counters);
