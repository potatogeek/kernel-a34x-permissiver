// SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB
/*
 * Copyright (c) 2018 Mellanox Technologies. All rights reserved.
 */

<<<<<<< HEAD
#include "ib_rep.h"

static const struct mlx5_ib_profile rep_profile = {
	STAGE_CREATE(MLX5_IB_STAGE_INIT,
		     mlx5_ib_stage_init_init,
		     mlx5_ib_stage_init_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_FLOW_DB,
		     mlx5_ib_stage_rep_flow_db_init,
		     NULL),
	STAGE_CREATE(MLX5_IB_STAGE_CAPS,
		     mlx5_ib_stage_caps_init,
		     NULL),
	STAGE_CREATE(MLX5_IB_STAGE_NON_DEFAULT_CB,
		     mlx5_ib_stage_rep_non_default_cb,
		     NULL),
	STAGE_CREATE(MLX5_IB_STAGE_ROCE,
		     mlx5_ib_stage_rep_roce_init,
		     mlx5_ib_stage_rep_roce_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_DEVICE_RESOURCES,
		     mlx5_ib_stage_dev_res_init,
		     mlx5_ib_stage_dev_res_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_COUNTERS,
		     mlx5_ib_stage_counters_init,
		     mlx5_ib_stage_counters_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_BFREG,
		     mlx5_ib_stage_bfrag_init,
		     mlx5_ib_stage_bfrag_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_PRE_IB_REG_UMR,
		     NULL,
		     mlx5_ib_stage_pre_ib_reg_umr_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_IB_REG,
		     mlx5_ib_stage_ib_reg_init,
		     mlx5_ib_stage_ib_reg_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_POST_IB_REG_UMR,
		     mlx5_ib_stage_post_ib_reg_umr_init,
		     NULL),
	STAGE_CREATE(MLX5_IB_STAGE_CLASS_ATTR,
		     mlx5_ib_stage_class_attr_init,
		     NULL),
};

static int
mlx5_ib_nic_rep_load(struct mlx5_core_dev *dev, struct mlx5_eswitch_rep *rep)
{
	return 0;
}

static void
mlx5_ib_nic_rep_unload(struct mlx5_eswitch_rep *rep)
{
	rep->rep_if[REP_IB].priv = NULL;
}
=======
#include <linux/mlx5/vport.h>
#include "ib_rep.h"
#include "srq.h"

static int
mlx5_ib_set_vport_rep(struct mlx5_core_dev *dev,
		      struct mlx5_eswitch_rep *rep,
		      int vport_index)
{
	struct mlx5_ib_dev *ibdev;

	ibdev = mlx5_eswitch_uplink_get_proto_dev(dev->priv.eswitch, REP_IB);
	if (!ibdev)
		return -EINVAL;

	ibdev->port[vport_index].rep = rep;
	rep->rep_data[REP_IB].priv = ibdev;
	write_lock(&ibdev->port[vport_index].roce.netdev_lock);
	ibdev->port[vport_index].roce.netdev =
		mlx5_ib_get_rep_netdev(rep->esw, rep->vport);
	write_unlock(&ibdev->port[vport_index].roce.netdev_lock);

	return 0;
}

static void mlx5_ib_register_peer_vport_reps(struct mlx5_core_dev *mdev);
>>>>>>> upstream/android-13

static int
mlx5_ib_vport_rep_load(struct mlx5_core_dev *dev, struct mlx5_eswitch_rep *rep)
{
<<<<<<< HEAD
	struct mlx5_ib_dev *ibdev;

	ibdev = (struct mlx5_ib_dev *)ib_alloc_device(sizeof(*ibdev));
	if (!ibdev)
		return -ENOMEM;

	ibdev->rep = rep;
	ibdev->mdev = dev;
	ibdev->num_ports = max(MLX5_CAP_GEN(dev, num_ports),
			       MLX5_CAP_GEN(dev, num_vhca_ports));
	if (!__mlx5_ib_add(ibdev, &rep_profile)) {
		ib_dealloc_device(&ibdev->ib_dev);
		return -EINVAL;
	}

	rep->rep_if[REP_IB].priv = ibdev;

	return 0;
=======
	u32 num_ports = mlx5_eswitch_get_total_vports(dev);
	const struct mlx5_ib_profile *profile;
	struct mlx5_core_dev *peer_dev;
	struct mlx5_ib_dev *ibdev;
	u32 peer_num_ports;
	int vport_index;
	int ret;

	vport_index = rep->vport_index;

	if (mlx5_lag_is_shared_fdb(dev)) {
		peer_dev = mlx5_lag_get_peer_mdev(dev);
		peer_num_ports = mlx5_eswitch_get_total_vports(peer_dev);
		if (mlx5_lag_is_master(dev)) {
			/* Only 1 ib port is the representor for both uplinks */
			num_ports += peer_num_ports - 1;
		} else {
			if (rep->vport == MLX5_VPORT_UPLINK)
				return 0;
			vport_index += peer_num_ports;
			dev = peer_dev;
		}
	}

	if (rep->vport == MLX5_VPORT_UPLINK)
		profile = &raw_eth_profile;
	else
		return mlx5_ib_set_vport_rep(dev, rep, vport_index);

	ibdev = ib_alloc_device(mlx5_ib_dev, ib_dev);
	if (!ibdev)
		return -ENOMEM;

	ibdev->port = kcalloc(num_ports, sizeof(*ibdev->port),
			      GFP_KERNEL);
	if (!ibdev->port) {
		ret = -ENOMEM;
		goto fail_port;
	}

	ibdev->is_rep = true;
	vport_index = rep->vport_index;
	ibdev->port[vport_index].rep = rep;
	ibdev->port[vport_index].roce.netdev =
		mlx5_ib_get_rep_netdev(dev->priv.eswitch, rep->vport);
	ibdev->mdev = dev;
	ibdev->num_ports = num_ports;

	ret = __mlx5_ib_add(ibdev, profile);
	if (ret)
		goto fail_add;

	rep->rep_data[REP_IB].priv = ibdev;
	if (mlx5_lag_is_shared_fdb(dev))
		mlx5_ib_register_peer_vport_reps(dev);

	return 0;

fail_add:
	kfree(ibdev->port);
fail_port:
	ib_dealloc_device(&ibdev->ib_dev);
	return ret;
}

static void *mlx5_ib_rep_to_dev(struct mlx5_eswitch_rep *rep)
{
	return rep->rep_data[REP_IB].priv;
>>>>>>> upstream/android-13
}

static void
mlx5_ib_vport_rep_unload(struct mlx5_eswitch_rep *rep)
{
<<<<<<< HEAD
	struct mlx5_ib_dev *dev;

	if (!rep->rep_if[REP_IB].priv)
		return;

	dev = mlx5_ib_rep_to_dev(rep);
	__mlx5_ib_remove(dev, dev->profile, MLX5_IB_STAGE_MAX);
	rep->rep_if[REP_IB].priv = NULL;
}

static void *mlx5_ib_vport_get_proto_dev(struct mlx5_eswitch_rep *rep)
{
	return mlx5_ib_rep_to_dev(rep);
}

static void mlx5_ib_rep_register_vf_vports(struct mlx5_ib_dev *dev)
{
	struct mlx5_eswitch *esw   = dev->mdev->priv.eswitch;
	int total_vfs = MLX5_TOTAL_VPORTS(dev->mdev);
	int vport;

	for (vport = 1; vport < total_vfs; vport++) {
		struct mlx5_eswitch_rep_if rep_if = {};

		rep_if.load = mlx5_ib_vport_rep_load;
		rep_if.unload = mlx5_ib_vport_rep_unload;
		rep_if.get_proto_dev = mlx5_ib_vport_get_proto_dev;
		mlx5_eswitch_register_vport_rep(esw, vport, &rep_if, REP_IB);
	}
}

static void mlx5_ib_rep_unregister_vf_vports(struct mlx5_ib_dev *dev)
{
	struct mlx5_eswitch *esw   = dev->mdev->priv.eswitch;
	int total_vfs = MLX5_TOTAL_VPORTS(dev->mdev);
	int vport;

	for (vport = 1; vport < total_vfs; vport++)
		mlx5_eswitch_unregister_vport_rep(esw, vport, REP_IB);
}

void mlx5_ib_register_vport_reps(struct mlx5_ib_dev *dev)
{
	struct mlx5_eswitch *esw = dev->mdev->priv.eswitch;
	struct mlx5_eswitch_rep_if rep_if = {};

	rep_if.load = mlx5_ib_nic_rep_load;
	rep_if.unload = mlx5_ib_nic_rep_unload;
	rep_if.get_proto_dev = mlx5_ib_vport_get_proto_dev;
	rep_if.priv = dev;

	mlx5_eswitch_register_vport_rep(esw, 0, &rep_if, REP_IB);

	mlx5_ib_rep_register_vf_vports(dev);
}

void mlx5_ib_unregister_vport_reps(struct mlx5_ib_dev *dev)
{
	struct mlx5_eswitch *esw   = dev->mdev->priv.eswitch;

	mlx5_ib_rep_unregister_vf_vports(dev); /* VFs vports */
	mlx5_eswitch_unregister_vport_rep(esw, 0, REP_IB); /* UPLINK PF*/
}

u8 mlx5_ib_eswitch_mode(struct mlx5_eswitch *esw)
{
	return mlx5_eswitch_mode(esw);
}

struct mlx5_ib_dev *mlx5_ib_get_rep_ibdev(struct mlx5_eswitch *esw,
					  int vport_index)
{
	return mlx5_eswitch_get_proto_dev(esw, vport_index, REP_IB);
}

struct net_device *mlx5_ib_get_rep_netdev(struct mlx5_eswitch *esw,
					  int vport_index)
{
	return mlx5_eswitch_get_proto_dev(esw, vport_index, REP_ETH);
}

struct mlx5_ib_dev *mlx5_ib_get_uplink_ibdev(struct mlx5_eswitch *esw)
{
	return mlx5_eswitch_uplink_get_proto_dev(esw, REP_IB);
}

struct mlx5_eswitch_rep *mlx5_ib_vport_rep(struct mlx5_eswitch *esw, int vport)
{
	return mlx5_eswitch_vport_rep(esw, vport);
}

int create_flow_rule_vport_sq(struct mlx5_ib_dev *dev,
			      struct mlx5_ib_sq *sq)
{
	struct mlx5_flow_handle *flow_rule;
	struct mlx5_eswitch *esw = dev->mdev->priv.eswitch;

	if (!dev->rep)
		return 0;

	flow_rule =
		mlx5_eswitch_add_send_to_vport_rule(esw,
						    dev->rep->vport,
						    sq->base.mqp.qpn);
	if (IS_ERR(flow_rule))
		return PTR_ERR(flow_rule);
	sq->flow_rule = flow_rule;

	return 0;
}
=======
	struct mlx5_core_dev *mdev = mlx5_eswitch_get_core_dev(rep->esw);
	struct mlx5_ib_dev *dev = mlx5_ib_rep_to_dev(rep);
	int vport_index = rep->vport_index;
	struct mlx5_ib_port *port;

	if (WARN_ON(!mdev))
		return;

	if (mlx5_lag_is_shared_fdb(mdev) &&
	    !mlx5_lag_is_master(mdev)) {
		struct mlx5_core_dev *peer_mdev;

		if (rep->vport == MLX5_VPORT_UPLINK)
			return;
		peer_mdev = mlx5_lag_get_peer_mdev(mdev);
		vport_index += mlx5_eswitch_get_total_vports(peer_mdev);
	}

	if (!dev)
		return;

	port = &dev->port[vport_index];
	write_lock(&port->roce.netdev_lock);
	port->roce.netdev = NULL;
	write_unlock(&port->roce.netdev_lock);
	rep->rep_data[REP_IB].priv = NULL;
	port->rep = NULL;

	if (rep->vport == MLX5_VPORT_UPLINK) {
		struct mlx5_core_dev *peer_mdev;
		struct mlx5_eswitch *esw;

		if (mlx5_lag_is_shared_fdb(mdev)) {
			peer_mdev = mlx5_lag_get_peer_mdev(mdev);
			esw = peer_mdev->priv.eswitch;
			mlx5_eswitch_unregister_vport_reps(esw, REP_IB);
		}
		__mlx5_ib_remove(dev, dev->profile, MLX5_IB_STAGE_MAX);
	}
}

static const struct mlx5_eswitch_rep_ops rep_ops = {
	.load = mlx5_ib_vport_rep_load,
	.unload = mlx5_ib_vport_rep_unload,
	.get_proto_dev = mlx5_ib_rep_to_dev,
};

static void mlx5_ib_register_peer_vport_reps(struct mlx5_core_dev *mdev)
{
	struct mlx5_core_dev *peer_mdev = mlx5_lag_get_peer_mdev(mdev);
	struct mlx5_eswitch *esw;

	if (!peer_mdev)
		return;

	esw = peer_mdev->priv.eswitch;
	mlx5_eswitch_register_vport_reps(esw, &rep_ops, REP_IB);
}

struct net_device *mlx5_ib_get_rep_netdev(struct mlx5_eswitch *esw,
					  u16 vport_num)
{
	return mlx5_eswitch_get_proto_dev(esw, vport_num, REP_ETH);
}

struct mlx5_flow_handle *create_flow_rule_vport_sq(struct mlx5_ib_dev *dev,
						   struct mlx5_ib_sq *sq,
						   u32 port)
{
	struct mlx5_eswitch *esw = dev->mdev->priv.eswitch;
	struct mlx5_eswitch_rep *rep;

	if (!dev->is_rep || !port)
		return NULL;

	if (!dev->port[port - 1].rep)
		return ERR_PTR(-EINVAL);

	rep = dev->port[port - 1].rep;

	return mlx5_eswitch_add_send_to_vport_rule(esw, esw, rep, sq->base.mqp.qpn);
}

static int mlx5r_rep_probe(struct auxiliary_device *adev,
			   const struct auxiliary_device_id *id)
{
	struct mlx5_adev *idev = container_of(adev, struct mlx5_adev, adev);
	struct mlx5_core_dev *mdev = idev->mdev;
	struct mlx5_eswitch *esw;

	esw = mdev->priv.eswitch;
	mlx5_eswitch_register_vport_reps(esw, &rep_ops, REP_IB);
	return 0;
}

static void mlx5r_rep_remove(struct auxiliary_device *adev)
{
	struct mlx5_adev *idev = container_of(adev, struct mlx5_adev, adev);
	struct mlx5_core_dev *mdev = idev->mdev;
	struct mlx5_eswitch *esw;

	esw = mdev->priv.eswitch;
	mlx5_eswitch_unregister_vport_reps(esw, REP_IB);
}

static const struct auxiliary_device_id mlx5r_rep_id_table[] = {
	{ .name = MLX5_ADEV_NAME ".rdma-rep", },
	{},
};

MODULE_DEVICE_TABLE(auxiliary, mlx5r_rep_id_table);

static struct auxiliary_driver mlx5r_rep_driver = {
	.name = "rep",
	.probe = mlx5r_rep_probe,
	.remove = mlx5r_rep_remove,
	.id_table = mlx5r_rep_id_table,
};

int mlx5r_rep_init(void)
{
	return auxiliary_driver_register(&mlx5r_rep_driver);
}

void mlx5r_rep_cleanup(void)
{
	auxiliary_driver_unregister(&mlx5r_rep_driver);
}
>>>>>>> upstream/android-13
