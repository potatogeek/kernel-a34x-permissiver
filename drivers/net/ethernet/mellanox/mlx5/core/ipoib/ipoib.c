/*
 * Copyright (c) 2017, Mellanox Technologies. All rights reserved.
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

#include <rdma/ib_verbs.h>
#include <linux/mlx5/fs.h>
#include "en.h"
<<<<<<< HEAD
=======
#include "en/params.h"
>>>>>>> upstream/android-13
#include "ipoib.h"

#define IB_DEFAULT_Q_KEY   0xb1b
#define MLX5I_PARAMS_DEFAULT_LOG_RQ_SIZE 9

static int mlx5i_open(struct net_device *netdev);
static int mlx5i_close(struct net_device *netdev);
static int mlx5i_change_mtu(struct net_device *netdev, int new_mtu);

static const struct net_device_ops mlx5i_netdev_ops = {
	.ndo_open                = mlx5i_open,
	.ndo_stop                = mlx5i_close,
<<<<<<< HEAD
	.ndo_init                = mlx5i_dev_init,
	.ndo_uninit              = mlx5i_dev_cleanup,
	.ndo_change_mtu          = mlx5i_change_mtu,
	.ndo_do_ioctl            = mlx5i_ioctl,
=======
	.ndo_get_stats64         = mlx5i_get_stats,
	.ndo_init                = mlx5i_dev_init,
	.ndo_uninit              = mlx5i_dev_cleanup,
	.ndo_change_mtu          = mlx5i_change_mtu,
	.ndo_eth_ioctl            = mlx5i_ioctl,
>>>>>>> upstream/android-13
};

/* IPoIB mlx5 netdev profile */
static void mlx5i_build_nic_params(struct mlx5_core_dev *mdev,
				   struct mlx5e_params *params)
{
	/* Override RQ params as IPoIB supports only LINKED LIST RQ for now */
	MLX5E_SET_PFLAG(params, MLX5E_PFLAG_RX_STRIDING_RQ, false);
	mlx5e_set_rq_type(mdev, params);
	mlx5e_init_rq_type_params(mdev, params);

	/* RQ size in ipoib by default is 512 */
	params->log_rq_mtu_frames = is_kdump_kernel() ?
		MLX5E_PARAMS_MINIMUM_LOG_RQ_SIZE :
		MLX5I_PARAMS_DEFAULT_LOG_RQ_SIZE;

<<<<<<< HEAD
	params->lro_en = false;
	params->hard_mtu = MLX5_IB_GRH_BYTES + MLX5_IPOIB_HARD_LEN;
}

/* Called directly after IPoIB netdevice was created to initialize SW structs */
void mlx5i_init(struct mlx5_core_dev *mdev,
		struct net_device *netdev,
		const struct mlx5e_profile *profile,
		void *ppriv)
{
	struct mlx5e_priv *priv  = mlx5i_epriv(netdev);
	u16 max_mtu;

	/* priv init */
	priv->mdev        = mdev;
	priv->netdev      = netdev;
	priv->profile     = profile;
	priv->ppriv       = ppriv;
	mutex_init(&priv->state_lock);

	mlx5_query_port_max_mtu(mdev, &max_mtu, 1);
	netdev->mtu = max_mtu;

	mlx5e_build_nic_params(mdev, &priv->channels.params,
			       profile->max_nch(mdev), netdev->mtu);
=======
	params->packet_merge.type = MLX5E_PACKET_MERGE_NONE;
	params->hard_mtu = MLX5_IB_GRH_BYTES + MLX5_IPOIB_HARD_LEN;
	params->tunneled_offload_en = false;
}

/* Called directly after IPoIB netdevice was created to initialize SW structs */
int mlx5i_init(struct mlx5_core_dev *mdev, struct net_device *netdev)
{
	struct mlx5e_priv *priv  = mlx5i_epriv(netdev);

	netif_carrier_off(netdev);
	mlx5e_set_netdev_mtu_boundaries(priv);
	netdev->mtu = netdev->max_mtu;

	mlx5e_build_nic_params(priv, NULL, netdev->mtu);
>>>>>>> upstream/android-13
	mlx5i_build_nic_params(mdev, &priv->channels.params);

	mlx5e_timestamp_init(priv);

	/* netdev init */
	netdev->hw_features    |= NETIF_F_SG;
	netdev->hw_features    |= NETIF_F_IP_CSUM;
	netdev->hw_features    |= NETIF_F_IPV6_CSUM;
	netdev->hw_features    |= NETIF_F_GRO;
	netdev->hw_features    |= NETIF_F_TSO;
	netdev->hw_features    |= NETIF_F_TSO6;
	netdev->hw_features    |= NETIF_F_RXCSUM;
	netdev->hw_features    |= NETIF_F_RXHASH;

	netdev->netdev_ops = &mlx5i_netdev_ops;
	netdev->ethtool_ops = &mlx5i_ethtool_ops;
<<<<<<< HEAD
}

/* Called directly before IPoIB netdevice is destroyed to cleanup SW structs */
static void mlx5i_cleanup(struct mlx5e_priv *priv)
{
	/* Do nothing .. */
=======

	return 0;
}

/* Called directly before IPoIB netdevice is destroyed to cleanup SW structs */
void mlx5i_cleanup(struct mlx5e_priv *priv)
{
	mlx5e_priv_cleanup(priv);
}

static void mlx5i_grp_sw_update_stats(struct mlx5e_priv *priv)
{
	struct mlx5e_sw_stats s = { 0 };
	int i, j;

	for (i = 0; i < priv->stats_nch; i++) {
		struct mlx5e_channel_stats *channel_stats;
		struct mlx5e_rq_stats *rq_stats;

		channel_stats = &priv->channel_stats[i];
		rq_stats = &channel_stats->rq;

		s.rx_packets += rq_stats->packets;
		s.rx_bytes   += rq_stats->bytes;

		for (j = 0; j < priv->max_opened_tc; j++) {
			struct mlx5e_sq_stats *sq_stats = &channel_stats->sq[j];

			s.tx_packets           += sq_stats->packets;
			s.tx_bytes             += sq_stats->bytes;
			s.tx_queue_dropped     += sq_stats->dropped;
		}
	}

	memcpy(&priv->stats.sw, &s, sizeof(s));
}

void mlx5i_get_stats(struct net_device *dev, struct rtnl_link_stats64 *stats)
{
	struct mlx5e_priv     *priv   = mlx5i_epriv(dev);
	struct mlx5e_sw_stats *sstats = &priv->stats.sw;

	mlx5i_grp_sw_update_stats(priv);

	stats->rx_packets = sstats->rx_packets;
	stats->rx_bytes   = sstats->rx_bytes;
	stats->tx_packets = sstats->tx_packets;
	stats->tx_bytes   = sstats->tx_bytes;
	stats->tx_dropped = sstats->tx_queue_dropped;
>>>>>>> upstream/android-13
}

int mlx5i_init_underlay_qp(struct mlx5e_priv *priv)
{
	struct mlx5_core_dev *mdev = priv->mdev;
	struct mlx5i_priv *ipriv = priv->ppriv;
<<<<<<< HEAD
	struct mlx5_core_qp *qp = &ipriv->qp;
	struct mlx5_qp_context *context;
	int ret;

	/* QP states */
	context = kzalloc(sizeof(*context), GFP_KERNEL);
	if (!context)
		return -ENOMEM;

	context->flags = cpu_to_be32(MLX5_QP_PM_MIGRATED << 11);
	context->pri_path.port = 1;
	context->pri_path.pkey_index = cpu_to_be16(ipriv->pkey_index);
	context->qkey = cpu_to_be32(IB_DEFAULT_Q_KEY);

	ret = mlx5_core_qp_modify(mdev, MLX5_CMD_OP_RST2INIT_QP, 0, context, qp);
	if (ret) {
		mlx5_core_err(mdev, "Failed to modify qp RST2INIT, err: %d\n", ret);
		goto err_qp_modify_to_err;
	}
	memset(context, 0, sizeof(*context));

	ret = mlx5_core_qp_modify(mdev, MLX5_CMD_OP_INIT2RTR_QP, 0, context, qp);
	if (ret) {
		mlx5_core_err(mdev, "Failed to modify qp INIT2RTR, err: %d\n", ret);
		goto err_qp_modify_to_err;
	}

	ret = mlx5_core_qp_modify(mdev, MLX5_CMD_OP_RTR2RTS_QP, 0, context, qp);
	if (ret) {
		mlx5_core_err(mdev, "Failed to modify qp RTR2RTS, err: %d\n", ret);
		goto err_qp_modify_to_err;
	}

	kfree(context);
	return 0;

err_qp_modify_to_err:
	mlx5_core_qp_modify(mdev, MLX5_CMD_OP_2ERR_QP, 0, &context, qp);
	kfree(context);
=======
	int ret;

	{
		u32 in[MLX5_ST_SZ_DW(rst2init_qp_in)] = {};
		u32 *qpc;

		qpc = MLX5_ADDR_OF(rst2init_qp_in, in, qpc);

		MLX5_SET(qpc, qpc, pm_state, MLX5_QP_PM_MIGRATED);
		MLX5_SET(qpc, qpc, primary_address_path.pkey_index,
			 ipriv->pkey_index);
		MLX5_SET(qpc, qpc, primary_address_path.vhca_port_num, 1);
		MLX5_SET(qpc, qpc, q_key, IB_DEFAULT_Q_KEY);

		MLX5_SET(rst2init_qp_in, in, opcode, MLX5_CMD_OP_RST2INIT_QP);
		MLX5_SET(rst2init_qp_in, in, qpn, ipriv->qpn);
		ret = mlx5_cmd_exec_in(mdev, rst2init_qp, in);
		if (ret)
			goto err_qp_modify_to_err;
	}
	{
		u32 in[MLX5_ST_SZ_DW(init2rtr_qp_in)] = {};

		MLX5_SET(init2rtr_qp_in, in, opcode, MLX5_CMD_OP_INIT2RTR_QP);
		MLX5_SET(init2rtr_qp_in, in, qpn, ipriv->qpn);
		ret = mlx5_cmd_exec_in(mdev, init2rtr_qp, in);
		if (ret)
			goto err_qp_modify_to_err;
	}
	{
		u32 in[MLX5_ST_SZ_DW(rtr2rts_qp_in)] = {};

		MLX5_SET(rtr2rts_qp_in, in, opcode, MLX5_CMD_OP_RTR2RTS_QP);
		MLX5_SET(rtr2rts_qp_in, in, qpn, ipriv->qpn);
		ret = mlx5_cmd_exec_in(mdev, rtr2rts_qp, in);
		if (ret)
			goto err_qp_modify_to_err;
	}
	return 0;

err_qp_modify_to_err:
	{
		u32 in[MLX5_ST_SZ_DW(qp_2err_in)] = {};

		MLX5_SET(qp_2err_in, in, opcode, MLX5_CMD_OP_2ERR_QP);
		MLX5_SET(qp_2err_in, in, qpn, ipriv->qpn);
		mlx5_cmd_exec_in(mdev, qp_2err, in);
	}
>>>>>>> upstream/android-13
	return ret;
}

void mlx5i_uninit_underlay_qp(struct mlx5e_priv *priv)
{
	struct mlx5i_priv *ipriv = priv->ppriv;
	struct mlx5_core_dev *mdev = priv->mdev;
<<<<<<< HEAD
	struct mlx5_qp_context context;
	int err;

	err = mlx5_core_qp_modify(mdev, MLX5_CMD_OP_2RST_QP, 0, &context,
				  &ipriv->qp);
	if (err)
		mlx5_core_err(mdev, "Failed to modify qp 2RST, err: %d\n", err);
=======
	u32 in[MLX5_ST_SZ_DW(qp_2rst_in)] = {};

	MLX5_SET(qp_2rst_in, in, opcode, MLX5_CMD_OP_2RST_QP);
	MLX5_SET(qp_2rst_in, in, qpn, ipriv->qpn);
	mlx5_cmd_exec_in(mdev, qp_2rst, in);
>>>>>>> upstream/android-13
}

#define MLX5_QP_ENHANCED_ULP_STATELESS_MODE 2

<<<<<<< HEAD
int mlx5i_create_underlay_qp(struct mlx5_core_dev *mdev, struct mlx5_core_qp *qp)
{
	u32 *in = NULL;
	void *addr_path;
	int ret = 0;
	int inlen;
	void *qpc;

	inlen = MLX5_ST_SZ_BYTES(create_qp_in);
	in = kvzalloc(inlen, GFP_KERNEL);
	if (!in)
		return -ENOMEM;

	qpc = MLX5_ADDR_OF(create_qp_in, in, qpc);
=======
int mlx5i_create_underlay_qp(struct mlx5e_priv *priv)
{
	unsigned char *dev_addr = priv->netdev->dev_addr;
	u32 out[MLX5_ST_SZ_DW(create_qp_out)] = {};
	u32 in[MLX5_ST_SZ_DW(create_qp_in)] = {};
	struct mlx5i_priv *ipriv = priv->ppriv;
	void *addr_path;
	int qpn = 0;
	int ret = 0;
	void *qpc;

	if (MLX5_CAP_GEN(priv->mdev, mkey_by_name)) {
		qpn = (dev_addr[1] << 16) + (dev_addr[2] << 8) + dev_addr[3];
		MLX5_SET(create_qp_in, in, input_qpn, qpn);
	}

	qpc = MLX5_ADDR_OF(create_qp_in, in, qpc);
	MLX5_SET(qpc, qpc, ts_format, mlx5_get_qp_default_ts(priv->mdev));
>>>>>>> upstream/android-13
	MLX5_SET(qpc, qpc, st, MLX5_QP_ST_UD);
	MLX5_SET(qpc, qpc, pm_state, MLX5_QP_PM_MIGRATED);
	MLX5_SET(qpc, qpc, ulp_stateless_offload_mode,
		 MLX5_QP_ENHANCED_ULP_STATELESS_MODE);

	addr_path = MLX5_ADDR_OF(qpc, qpc, primary_address_path);
	MLX5_SET(ads, addr_path, vhca_port_num, 1);
	MLX5_SET(ads, addr_path, grh, 1);

<<<<<<< HEAD
	ret = mlx5_core_create_qp(mdev, qp, in, inlen);
	if (ret) {
		mlx5_core_err(mdev, "Failed creating IPoIB QP err : %d\n", ret);
		goto out;
	}

out:
	kvfree(in);
	return ret;
}

void mlx5i_destroy_underlay_qp(struct mlx5_core_dev *mdev, struct mlx5_core_qp *qp)
{
	mlx5_core_destroy_qp(mdev, qp);
=======
	MLX5_SET(create_qp_in, in, opcode, MLX5_CMD_OP_CREATE_QP);
	ret = mlx5_cmd_exec_inout(priv->mdev, create_qp, in, out);
	if (ret)
		return ret;

	ipriv->qpn = MLX5_GET(create_qp_out, out, qpn);

	return 0;
}

void mlx5i_destroy_underlay_qp(struct mlx5_core_dev *mdev, u32 qpn)
{
	u32 in[MLX5_ST_SZ_DW(destroy_qp_in)] = {};

	MLX5_SET(destroy_qp_in, in, opcode, MLX5_CMD_OP_DESTROY_QP);
	MLX5_SET(destroy_qp_in, in, qpn, qpn);
	mlx5_cmd_exec_in(mdev, destroy_qp, in);
}

int mlx5i_update_nic_rx(struct mlx5e_priv *priv)
{
	return mlx5e_refresh_tirs(priv, true, true);
}

int mlx5i_create_tis(struct mlx5_core_dev *mdev, u32 underlay_qpn, u32 *tisn)
{
	u32 in[MLX5_ST_SZ_DW(create_tis_in)] = {};
	void *tisc;

	tisc = MLX5_ADDR_OF(create_tis_in, in, ctx);

	MLX5_SET(tisc, tisc, underlay_qpn, underlay_qpn);

	return mlx5e_create_tis(mdev, in, tisn);
>>>>>>> upstream/android-13
}

static int mlx5i_init_tx(struct mlx5e_priv *priv)
{
	struct mlx5i_priv *ipriv = priv->ppriv;
	int err;

<<<<<<< HEAD
	err = mlx5i_create_underlay_qp(priv->mdev, &ipriv->qp);
=======
	err = mlx5i_create_underlay_qp(priv);
>>>>>>> upstream/android-13
	if (err) {
		mlx5_core_warn(priv->mdev, "create underlay QP failed, %d\n", err);
		return err;
	}

<<<<<<< HEAD
	err = mlx5e_create_tis(priv->mdev, 0 /* tc */, ipriv->qp.qpn, &priv->tisn[0]);
=======
	err = mlx5i_create_tis(priv->mdev, ipriv->qpn, &priv->tisn[0][0]);
>>>>>>> upstream/android-13
	if (err) {
		mlx5_core_warn(priv->mdev, "create tis failed, %d\n", err);
		goto err_destroy_underlay_qp;
	}

	return 0;

err_destroy_underlay_qp:
<<<<<<< HEAD
	mlx5i_destroy_underlay_qp(priv->mdev, &ipriv->qp);
=======
	mlx5i_destroy_underlay_qp(priv->mdev, ipriv->qpn);
>>>>>>> upstream/android-13
	return err;
}

static void mlx5i_cleanup_tx(struct mlx5e_priv *priv)
{
	struct mlx5i_priv *ipriv = priv->ppriv;

<<<<<<< HEAD
	mlx5e_destroy_tis(priv->mdev, priv->tisn[0]);
	mlx5i_destroy_underlay_qp(priv->mdev, &ipriv->qp);
=======
	mlx5e_destroy_tis(priv->mdev, priv->tisn[0][0]);
	mlx5i_destroy_underlay_qp(priv->mdev, ipriv->qpn);
>>>>>>> upstream/android-13
}

static int mlx5i_create_flow_steering(struct mlx5e_priv *priv)
{
<<<<<<< HEAD
	struct ttc_params ttc_params = {};
	int tt, err;
=======
	int err;
>>>>>>> upstream/android-13

	priv->fs.ns = mlx5_get_flow_namespace(priv->mdev,
					       MLX5_FLOW_NAMESPACE_KERNEL);

	if (!priv->fs.ns)
		return -EINVAL;

	err = mlx5e_arfs_create_tables(priv);
	if (err) {
		netdev_err(priv->netdev, "Failed to create arfs tables, err=%d\n",
			   err);
		priv->netdev->hw_features &= ~NETIF_F_NTUPLE;
	}

<<<<<<< HEAD
	mlx5e_set_ttc_basic_params(priv, &ttc_params);
	mlx5e_set_inner_ttc_ft_params(&ttc_params);
	for (tt = 0; tt < MLX5E_NUM_INDIR_TIRS; tt++)
		ttc_params.indir_tirn[tt] = priv->inner_indir_tir[tt].tirn;

	err = mlx5e_create_inner_ttc_table(priv, &ttc_params, &priv->fs.inner_ttc);
	if (err) {
		netdev_err(priv->netdev, "Failed to create inner ttc table, err=%d\n",
=======
	err = mlx5e_create_ttc_table(priv);
	if (err) {
		netdev_err(priv->netdev, "Failed to create ttc table, err=%d\n",
>>>>>>> upstream/android-13
			   err);
		goto err_destroy_arfs_tables;
	}

<<<<<<< HEAD
	mlx5e_set_ttc_ft_params(&ttc_params);
	for (tt = 0; tt < MLX5E_NUM_INDIR_TIRS; tt++)
		ttc_params.indir_tirn[tt] = priv->indir_tir[tt].tirn;

	err = mlx5e_create_ttc_table(priv, &ttc_params, &priv->fs.ttc);
	if (err) {
		netdev_err(priv->netdev, "Failed to create ttc table, err=%d\n",
			   err);
		goto err_destroy_inner_ttc_table;
	}

	return 0;

err_destroy_inner_ttc_table:
	mlx5e_destroy_inner_ttc_table(priv, &priv->fs.inner_ttc);
=======
	return 0;

>>>>>>> upstream/android-13
err_destroy_arfs_tables:
	mlx5e_arfs_destroy_tables(priv);

	return err;
}

static void mlx5i_destroy_flow_steering(struct mlx5e_priv *priv)
{
<<<<<<< HEAD
	mlx5e_destroy_ttc_table(priv, &priv->fs.ttc);
	mlx5e_destroy_inner_ttc_table(priv, &priv->fs.inner_ttc);
=======
	mlx5e_destroy_ttc_table(priv);
>>>>>>> upstream/android-13
	mlx5e_arfs_destroy_tables(priv);
}

static int mlx5i_init_rx(struct mlx5e_priv *priv)
{
<<<<<<< HEAD
	int err;

	err = mlx5e_create_indirect_rqt(priv);
	if (err)
		return err;

	err = mlx5e_create_direct_rqts(priv);
	if (err)
		goto err_destroy_indirect_rqts;

	err = mlx5e_create_indirect_tirs(priv);
	if (err)
		goto err_destroy_direct_rqts;

	err = mlx5e_create_direct_tirs(priv);
	if (err)
		goto err_destroy_indirect_tirs;

	err = mlx5i_create_flow_steering(priv);
	if (err)
		goto err_destroy_direct_tirs;

	return 0;

err_destroy_direct_tirs:
	mlx5e_destroy_direct_tirs(priv);
err_destroy_indirect_tirs:
	mlx5e_destroy_indirect_tirs(priv);
err_destroy_direct_rqts:
	mlx5e_destroy_direct_rqts(priv);
err_destroy_indirect_rqts:
	mlx5e_destroy_rqt(priv, &priv->indir_rqt);
=======
	struct mlx5_core_dev *mdev = priv->mdev;
	int err;

	priv->rx_res = mlx5e_rx_res_alloc();
	if (!priv->rx_res)
		return -ENOMEM;

	mlx5e_create_q_counters(priv);

	err = mlx5e_open_drop_rq(priv, &priv->drop_rq);
	if (err) {
		mlx5_core_err(mdev, "open drop rq failed, %d\n", err);
		goto err_destroy_q_counters;
	}

	err = mlx5e_rx_res_init(priv->rx_res, priv->mdev, 0,
				priv->max_nch, priv->drop_rq.rqn,
				&priv->channels.params.packet_merge,
				priv->channels.params.num_channels);
	if (err)
		goto err_close_drop_rq;

	err = mlx5i_create_flow_steering(priv);
	if (err)
		goto err_destroy_rx_res;

	return 0;

err_destroy_rx_res:
	mlx5e_rx_res_destroy(priv->rx_res);
err_close_drop_rq:
	mlx5e_close_drop_rq(&priv->drop_rq);
err_destroy_q_counters:
	mlx5e_destroy_q_counters(priv);
	mlx5e_rx_res_free(priv->rx_res);
	priv->rx_res = NULL;
>>>>>>> upstream/android-13
	return err;
}

static void mlx5i_cleanup_rx(struct mlx5e_priv *priv)
{
	mlx5i_destroy_flow_steering(priv);
<<<<<<< HEAD
	mlx5e_destroy_direct_tirs(priv);
	mlx5e_destroy_indirect_tirs(priv);
	mlx5e_destroy_direct_rqts(priv);
	mlx5e_destroy_rqt(priv, &priv->indir_rqt);
=======
	mlx5e_rx_res_destroy(priv->rx_res);
	mlx5e_close_drop_rq(&priv->drop_rq);
	mlx5e_destroy_q_counters(priv);
	mlx5e_rx_res_free(priv->rx_res);
	priv->rx_res = NULL;
}

/* The stats groups order is opposite to the update_stats() order calls */
static mlx5e_stats_grp_t mlx5i_stats_grps[] = {
	&MLX5E_STATS_GRP(sw),
	&MLX5E_STATS_GRP(qcnt),
	&MLX5E_STATS_GRP(vnic_env),
	&MLX5E_STATS_GRP(vport),
	&MLX5E_STATS_GRP(802_3),
	&MLX5E_STATS_GRP(2863),
	&MLX5E_STATS_GRP(2819),
	&MLX5E_STATS_GRP(phy),
	&MLX5E_STATS_GRP(pcie),
	&MLX5E_STATS_GRP(per_prio),
	&MLX5E_STATS_GRP(pme),
	&MLX5E_STATS_GRP(channels),
	&MLX5E_STATS_GRP(per_port_buff_congest),
};

static unsigned int mlx5i_stats_grps_num(struct mlx5e_priv *priv)
{
	return ARRAY_SIZE(mlx5i_stats_grps);
>>>>>>> upstream/android-13
}

static const struct mlx5e_profile mlx5i_nic_profile = {
	.init		   = mlx5i_init,
	.cleanup	   = mlx5i_cleanup,
	.init_tx	   = mlx5i_init_tx,
	.cleanup_tx	   = mlx5i_cleanup_tx,
	.init_rx	   = mlx5i_init_rx,
	.cleanup_rx	   = mlx5i_cleanup_rx,
	.enable		   = NULL, /* mlx5i_enable */
	.disable	   = NULL, /* mlx5i_disable */
<<<<<<< HEAD
	.update_stats	   = NULL, /* mlx5i_update_stats */
	.max_nch	   = mlx5e_get_max_num_channels,
	.update_carrier    = NULL, /* no HW update in IB link */
	.rx_handlers.handle_rx_cqe       = mlx5i_handle_rx_cqe,
	.rx_handlers.handle_rx_cqe_mpwqe = NULL, /* Not supported */
	.max_tc		   = MLX5I_MAX_NUM_TC,
=======
	.update_rx	   = mlx5i_update_nic_rx,
	.update_stats	   = NULL, /* mlx5i_update_stats */
	.update_carrier    = NULL, /* no HW update in IB link */
	.rx_handlers       = &mlx5i_rx_handlers,
	.max_tc		   = MLX5I_MAX_NUM_TC,
	.rq_groups	   = MLX5E_NUM_RQ_GROUPS(REGULAR),
	.stats_grps        = mlx5i_stats_grps,
	.stats_grps_num    = mlx5i_stats_grps_num,
	.rx_ptp_support    = false,
>>>>>>> upstream/android-13
};

/* mlx5i netdev NDos */

static int mlx5i_change_mtu(struct net_device *netdev, int new_mtu)
{
	struct mlx5e_priv *priv = mlx5i_epriv(netdev);
<<<<<<< HEAD
	struct mlx5e_channels new_channels = {};
	struct mlx5e_params *params;
=======
	struct mlx5e_params new_params;
>>>>>>> upstream/android-13
	int err = 0;

	mutex_lock(&priv->state_lock);

<<<<<<< HEAD
	params = &priv->channels.params;

	if (!test_bit(MLX5E_STATE_OPENED, &priv->state)) {
		params->sw_mtu = new_mtu;
		netdev->mtu = params->sw_mtu;
		goto out;
	}

	new_channels.params = *params;
	new_channels.params.sw_mtu = new_mtu;
	err = mlx5e_open_channels(priv, &new_channels);
	if (err)
		goto out;

	mlx5e_switch_priv_channels(priv, &new_channels, NULL);
	netdev->mtu = new_channels.params.sw_mtu;
=======
	new_params = priv->channels.params;
	new_params.sw_mtu = new_mtu;

	err = mlx5e_safe_switch_params(priv, &new_params, NULL, NULL, true);
	if (err)
		goto out;

	netdev->mtu = new_params.sw_mtu;
>>>>>>> upstream/android-13

out:
	mutex_unlock(&priv->state_lock);
	return err;
}

int mlx5i_dev_init(struct net_device *dev)
{
	struct mlx5e_priv    *priv   = mlx5i_epriv(dev);
	struct mlx5i_priv    *ipriv  = priv->ppriv;

	/* Set dev address using underlay QP */
<<<<<<< HEAD
	dev->dev_addr[1] = (ipriv->qp.qpn >> 16) & 0xff;
	dev->dev_addr[2] = (ipriv->qp.qpn >>  8) & 0xff;
	dev->dev_addr[3] = (ipriv->qp.qpn) & 0xff;

	/* Add QPN to net-device mapping to HT */
	mlx5i_pkey_add_qpn(dev ,ipriv->qp.qpn);
=======
	dev->dev_addr[1] = (ipriv->qpn >> 16) & 0xff;
	dev->dev_addr[2] = (ipriv->qpn >>  8) & 0xff;
	dev->dev_addr[3] = (ipriv->qpn) & 0xff;

	/* Add QPN to net-device mapping to HT */
	mlx5i_pkey_add_qpn(dev, ipriv->qpn);
>>>>>>> upstream/android-13

	return 0;
}

int mlx5i_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	struct mlx5e_priv *priv = mlx5i_epriv(dev);

	switch (cmd) {
	case SIOCSHWTSTAMP:
		return mlx5e_hwstamp_set(priv, ifr);
	case SIOCGHWTSTAMP:
		return mlx5e_hwstamp_get(priv, ifr);
	default:
		return -EOPNOTSUPP;
	}
}

void mlx5i_dev_cleanup(struct net_device *dev)
{
	struct mlx5e_priv    *priv   = mlx5i_epriv(dev);
	struct mlx5i_priv    *ipriv = priv->ppriv;

	mlx5i_uninit_underlay_qp(priv);

	/* Delete QPN to net-device mapping from HT */
<<<<<<< HEAD
	mlx5i_pkey_del_qpn(dev, ipriv->qp.qpn);
=======
	mlx5i_pkey_del_qpn(dev, ipriv->qpn);
>>>>>>> upstream/android-13
}

static int mlx5i_open(struct net_device *netdev)
{
	struct mlx5e_priv *epriv = mlx5i_epriv(netdev);
	struct mlx5i_priv *ipriv = epriv->ppriv;
	struct mlx5_core_dev *mdev = epriv->mdev;
	int err;

	mutex_lock(&epriv->state_lock);

	set_bit(MLX5E_STATE_OPENED, &epriv->state);

	err = mlx5i_init_underlay_qp(epriv);
	if (err) {
		mlx5_core_warn(mdev, "prepare underlay qp state failed, %d\n", err);
		goto err_clear_state_opened_flag;
	}

<<<<<<< HEAD
	err = mlx5_fs_add_rx_underlay_qpn(mdev, ipriv->qp.qpn);
=======
	err = mlx5_fs_add_rx_underlay_qpn(mdev, ipriv->qpn);
>>>>>>> upstream/android-13
	if (err) {
		mlx5_core_warn(mdev, "attach underlay qp to ft failed, %d\n", err);
		goto err_reset_qp;
	}

	err = mlx5e_open_channels(epriv, &epriv->channels);
	if (err)
		goto err_remove_fs_underlay_qp;

<<<<<<< HEAD
	mlx5e_refresh_tirs(epriv, false);
=======
	epriv->profile->update_rx(epriv);
>>>>>>> upstream/android-13
	mlx5e_activate_priv_channels(epriv);

	mutex_unlock(&epriv->state_lock);
	return 0;

err_remove_fs_underlay_qp:
<<<<<<< HEAD
	mlx5_fs_remove_rx_underlay_qpn(mdev, ipriv->qp.qpn);
=======
	mlx5_fs_remove_rx_underlay_qpn(mdev, ipriv->qpn);
>>>>>>> upstream/android-13
err_reset_qp:
	mlx5i_uninit_underlay_qp(epriv);
err_clear_state_opened_flag:
	clear_bit(MLX5E_STATE_OPENED, &epriv->state);
	mutex_unlock(&epriv->state_lock);
	return err;
}

static int mlx5i_close(struct net_device *netdev)
{
	struct mlx5e_priv *epriv = mlx5i_epriv(netdev);
	struct mlx5i_priv *ipriv = epriv->ppriv;
	struct mlx5_core_dev *mdev = epriv->mdev;

	/* May already be CLOSED in case a previous configuration operation
	 * (e.g RX/TX queue size change) that involves close&open failed.
	 */
	mutex_lock(&epriv->state_lock);

	if (!test_bit(MLX5E_STATE_OPENED, &epriv->state))
		goto unlock;

	clear_bit(MLX5E_STATE_OPENED, &epriv->state);

	netif_carrier_off(epriv->netdev);
<<<<<<< HEAD
	mlx5_fs_remove_rx_underlay_qpn(mdev, ipriv->qp.qpn);
=======
	mlx5_fs_remove_rx_underlay_qpn(mdev, ipriv->qpn);
>>>>>>> upstream/android-13
	mlx5e_deactivate_priv_channels(epriv);
	mlx5e_close_channels(&epriv->channels);
	mlx5i_uninit_underlay_qp(epriv);
unlock:
	mutex_unlock(&epriv->state_lock);
	return 0;
}

/* IPoIB RDMA netdev callbacks */
static int mlx5i_attach_mcast(struct net_device *netdev, struct ib_device *hca,
			      union ib_gid *gid, u16 lid, int set_qkey,
			      u32 qkey)
{
	struct mlx5e_priv    *epriv = mlx5i_epriv(netdev);
	struct mlx5_core_dev *mdev  = epriv->mdev;
	struct mlx5i_priv    *ipriv = epriv->ppriv;
	int err;

<<<<<<< HEAD
	mlx5_core_dbg(mdev, "attaching QPN 0x%x, MGID %pI6\n", ipriv->qp.qpn, gid->raw);
	err = mlx5_core_attach_mcg(mdev, gid, ipriv->qp.qpn);
	if (err)
		mlx5_core_warn(mdev, "failed attaching QPN 0x%x, MGID %pI6\n",
			       ipriv->qp.qpn, gid->raw);
=======
	mlx5_core_dbg(mdev, "attaching QPN 0x%x, MGID %pI6\n", ipriv->qpn,
		      gid->raw);
	err = mlx5_core_attach_mcg(mdev, gid, ipriv->qpn);
	if (err)
		mlx5_core_warn(mdev, "failed attaching QPN 0x%x, MGID %pI6\n",
			       ipriv->qpn, gid->raw);
>>>>>>> upstream/android-13

	if (set_qkey) {
		mlx5_core_dbg(mdev, "%s setting qkey 0x%x\n",
			      netdev->name, qkey);
		ipriv->qkey = qkey;
	}

	return err;
}

static int mlx5i_detach_mcast(struct net_device *netdev, struct ib_device *hca,
			      union ib_gid *gid, u16 lid)
{
	struct mlx5e_priv    *epriv = mlx5i_epriv(netdev);
	struct mlx5_core_dev *mdev  = epriv->mdev;
	struct mlx5i_priv    *ipriv = epriv->ppriv;
	int err;

<<<<<<< HEAD
	mlx5_core_dbg(mdev, "detaching QPN 0x%x, MGID %pI6\n", ipriv->qp.qpn, gid->raw);

	err = mlx5_core_detach_mcg(mdev, gid, ipriv->qp.qpn);
	if (err)
		mlx5_core_dbg(mdev, "failed detaching QPN 0x%x, MGID %pI6\n",
			      ipriv->qp.qpn, gid->raw);
=======
	mlx5_core_dbg(mdev, "detaching QPN 0x%x, MGID %pI6\n", ipriv->qpn,
		      gid->raw);

	err = mlx5_core_detach_mcg(mdev, gid, ipriv->qpn);
	if (err)
		mlx5_core_dbg(mdev, "failed detaching QPN 0x%x, MGID %pI6\n",
			      ipriv->qpn, gid->raw);
>>>>>>> upstream/android-13

	return err;
}

static int mlx5i_xmit(struct net_device *dev, struct sk_buff *skb,
		      struct ib_ah *address, u32 dqpn)
{
	struct mlx5e_priv *epriv = mlx5i_epriv(dev);
	struct mlx5e_txqsq *sq   = epriv->txq2sq[skb_get_queue_mapping(skb)];
	struct mlx5_ib_ah *mah   = to_mah(address);
	struct mlx5i_priv *ipriv = epriv->ppriv;

<<<<<<< HEAD
	return mlx5i_sq_xmit(sq, skb, &mah->av, dqpn, ipriv->qkey);
=======
	mlx5i_sq_xmit(sq, skb, &mah->av, dqpn, ipriv->qkey, netdev_xmit_more());

	return NETDEV_TX_OK;
>>>>>>> upstream/android-13
}

static void mlx5i_set_pkey_index(struct net_device *netdev, int id)
{
	struct mlx5i_priv *ipriv = netdev_priv(netdev);

	ipriv->pkey_index = (u16)id;
}

static int mlx5i_check_required_hca_cap(struct mlx5_core_dev *mdev)
{
	if (MLX5_CAP_GEN(mdev, port_type) != MLX5_CAP_PORT_TYPE_IB)
		return -EOPNOTSUPP;

	if (!MLX5_CAP_GEN(mdev, ipoib_enhanced_offloads)) {
		mlx5_core_warn(mdev, "IPoIB enhanced offloads are not supported\n");
		return -EOPNOTSUPP;
	}

	return 0;
}

static void mlx5_rdma_netdev_free(struct net_device *netdev)
{
	struct mlx5e_priv *priv = mlx5i_epriv(netdev);
<<<<<<< HEAD
=======
	struct mlx5_core_dev *mdev = priv->mdev;
>>>>>>> upstream/android-13
	struct mlx5i_priv *ipriv = priv->ppriv;
	const struct mlx5e_profile *profile = priv->profile;

	mlx5e_detach_netdev(priv);
	profile->cleanup(priv);
<<<<<<< HEAD
	destroy_workqueue(priv->wq);

	if (!ipriv->sub_interface) {
		mlx5i_pkey_qpn_ht_cleanup(netdev);
		mlx5e_destroy_mdev_resources(priv->mdev);
	}
}

struct net_device *mlx5_rdma_netdev_alloc(struct mlx5_core_dev *mdev,
					  struct ib_device *ibdev,
					  const char *name,
					  void (*setup)(struct net_device *))
{
	const struct mlx5e_profile *profile;
	struct net_device *netdev;
	struct mlx5i_priv *ipriv;
	struct mlx5e_priv *epriv;
	struct rdma_netdev *rn;
	bool sub_interface;
	int nch;
	int err;

	if (mlx5i_check_required_hca_cap(mdev)) {
		mlx5_core_warn(mdev, "Accelerated mode is not supported\n");
		return ERR_PTR(-EOPNOTSUPP);
	}

	/* TODO: Need to find a better way to check if child device*/
	sub_interface = (mdev->mlx5e_res.pdn != 0);

	if (sub_interface)
		profile = mlx5i_pkey_get_profile();
	else
		profile = &mlx5i_nic_profile;

	nch = profile->max_nch(mdev);

	netdev = alloc_netdev_mqs(sizeof(struct mlx5i_priv) + sizeof(struct mlx5e_priv),
				  name, NET_NAME_UNKNOWN,
				  setup,
				  nch * MLX5E_MAX_NUM_TC,
				  nch);
	if (!netdev) {
		mlx5_core_warn(mdev, "alloc_netdev_mqs failed\n");
		return NULL;
	}

	ipriv = netdev_priv(netdev);
	epriv = mlx5i_epriv(netdev);

	epriv->wq = create_singlethread_workqueue("mlx5i");
	if (!epriv->wq)
		goto err_free_netdev;

	ipriv->sub_interface = sub_interface;
=======

	if (!ipriv->sub_interface) {
		mlx5i_pkey_qpn_ht_cleanup(netdev);
		mlx5e_destroy_mdev_resources(mdev);
	}
}

static bool mlx5_is_sub_interface(struct mlx5_core_dev *mdev)
{
	return mdev->mlx5e_res.hw_objs.pdn != 0;
}

static const struct mlx5e_profile *mlx5_get_profile(struct mlx5_core_dev *mdev)
{
	if (mlx5_is_sub_interface(mdev))
		return mlx5i_pkey_get_profile();
	return &mlx5i_nic_profile;
}

static int mlx5_rdma_setup_rn(struct ib_device *ibdev, u32 port_num,
			      struct net_device *netdev, void *param)
{
	struct mlx5_core_dev *mdev = (struct mlx5_core_dev *)param;
	const struct mlx5e_profile *prof = mlx5_get_profile(mdev);
	struct mlx5i_priv *ipriv;
	struct mlx5e_priv *epriv;
	struct rdma_netdev *rn;
	int err;

	ipriv = netdev_priv(netdev);
	epriv = mlx5i_epriv(netdev);

	ipriv->sub_interface = mlx5_is_sub_interface(mdev);
>>>>>>> upstream/android-13
	if (!ipriv->sub_interface) {
		err = mlx5i_pkey_qpn_ht_init(netdev);
		if (err) {
			mlx5_core_warn(mdev, "allocate qpn_to_netdev ht failed\n");
<<<<<<< HEAD
			goto destroy_wq;
=======
			return err;
>>>>>>> upstream/android-13
		}

		/* This should only be called once per mdev */
		err = mlx5e_create_mdev_resources(mdev);
		if (err)
			goto destroy_ht;
	}

<<<<<<< HEAD
	profile->init(mdev, netdev, profile, ipriv);
=======
	err = mlx5e_priv_init(epriv, prof, netdev, mdev);
	if (err)
		goto destroy_mdev_resources;

	epriv->profile = prof;
	epriv->ppriv = ipriv;

	prof->init(mdev, netdev);
>>>>>>> upstream/android-13

	err = mlx5e_attach_netdev(epriv);
	if (err)
		goto detach;
	netif_carrier_off(netdev);

	/* set rdma_netdev func pointers */
	rn = &ipriv->rn;
	rn->hca  = ibdev;
	rn->send = mlx5i_xmit;
	rn->attach_mcast = mlx5i_attach_mcast;
	rn->detach_mcast = mlx5i_detach_mcast;
	rn->set_id = mlx5i_set_pkey_index;

	netdev->priv_destructor = mlx5_rdma_netdev_free;
	netdev->needs_free_netdev = 1;

<<<<<<< HEAD
	return netdev;

detach:
	profile->cleanup(epriv);
	if (ipriv->sub_interface)
		return NULL;
	mlx5e_destroy_mdev_resources(mdev);
destroy_ht:
	mlx5i_pkey_qpn_ht_cleanup(netdev);
destroy_wq:
	destroy_workqueue(epriv->wq);
err_free_netdev:
	free_netdev(netdev);

	return NULL;
}
EXPORT_SYMBOL(mlx5_rdma_netdev_alloc);
=======
	return 0;

detach:
	prof->cleanup(epriv);
	if (ipriv->sub_interface)
		return err;
destroy_mdev_resources:
	mlx5e_destroy_mdev_resources(mdev);
destroy_ht:
	mlx5i_pkey_qpn_ht_cleanup(netdev);
	return err;
}

int mlx5_rdma_rn_get_params(struct mlx5_core_dev *mdev,
			    struct ib_device *device,
			    struct rdma_netdev_alloc_params *params)
{
	int nch;
	int rc;

	rc = mlx5i_check_required_hca_cap(mdev);
	if (rc)
		return rc;

	nch = mlx5e_get_max_num_channels(mdev);

	*params = (struct rdma_netdev_alloc_params){
		.sizeof_priv = sizeof(struct mlx5i_priv) +
			       sizeof(struct mlx5e_priv),
		.txqs = nch * MLX5E_MAX_NUM_TC,
		.rxqs = nch,
		.param = mdev,
		.initialize_rdma_netdev = mlx5_rdma_setup_rn,
	};

	return 0;
}
EXPORT_SYMBOL(mlx5_rdma_rn_get_params);
>>>>>>> upstream/android-13
