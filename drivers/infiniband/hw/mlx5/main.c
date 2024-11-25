<<<<<<< HEAD
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
=======
// SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB
/*
 * Copyright (c) 2013-2020, Mellanox Technologies inc. All rights reserved.
 * Copyright (c) 2020, Intel Corporation. All rights reserved.
>>>>>>> upstream/android-13
 */

#include <linux/debugfs.h>
#include <linux/highmem.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/pci.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/bitmap.h>
<<<<<<< HEAD
#if defined(CONFIG_X86)
#include <asm/pat.h>
#endif
=======
>>>>>>> upstream/android-13
#include <linux/sched.h>
#include <linux/sched/mm.h>
#include <linux/sched/task.h>
#include <linux/delay.h>
#include <rdma/ib_user_verbs.h>
#include <rdma/ib_addr.h>
#include <rdma/ib_cache.h>
#include <linux/mlx5/port.h>
#include <linux/mlx5/vport.h>
#include <linux/mlx5/fs.h>
<<<<<<< HEAD
#include <linux/list.h>
#include <rdma/ib_smi.h>
#include <rdma/ib_umem.h>
=======
#include <linux/mlx5/eswitch.h>
#include <linux/list.h>
#include <rdma/ib_smi.h>
#include <rdma/ib_umem.h>
#include <rdma/lag.h>
>>>>>>> upstream/android-13
#include <linux/in.h>
#include <linux/etherdevice.h>
#include "mlx5_ib.h"
#include "ib_rep.h"
#include "cmd.h"
<<<<<<< HEAD
#include <linux/mlx5/fs_helpers.h>
#include <linux/mlx5/accel.h>
#include <rdma/uverbs_std_types.h>
#include <rdma/mlx5_user_ioctl_verbs.h>
#include <rdma/mlx5_user_ioctl_cmds.h>
=======
#include "devx.h"
#include "dm.h"
#include "fs.h"
#include "srq.h"
#include "qp.h"
#include "wr.h"
#include "restrack.h"
#include "counters.h"
#include <linux/mlx5/accel.h>
#include <rdma/uverbs_std_types.h>
#include <rdma/uverbs_ioctl.h>
#include <rdma/mlx5_user_ioctl_verbs.h>
#include <rdma/mlx5_user_ioctl_cmds.h>
#include <rdma/ib_umem_odp.h>
>>>>>>> upstream/android-13

#define UVERBS_MODULE_NAME mlx5_ib
#include <rdma/uverbs_named_ioctl.h>

<<<<<<< HEAD
#define DRIVER_NAME "mlx5_ib"
#define DRIVER_VERSION "5.0-0"

MODULE_AUTHOR("Eli Cohen <eli@mellanox.com>");
MODULE_DESCRIPTION("Mellanox Connect-IB HCA IB driver");
MODULE_LICENSE("Dual BSD/GPL");

static char mlx5_version[] =
	DRIVER_NAME ": Mellanox Connect-IB Infiniband driver v"
	DRIVER_VERSION "\n";

struct mlx5_ib_event_work {
	struct work_struct	work;
	struct mlx5_core_dev	*dev;
	void			*context;
	enum mlx5_dev_event	event;
	unsigned long		param;
=======
MODULE_AUTHOR("Eli Cohen <eli@mellanox.com>");
MODULE_DESCRIPTION("Mellanox 5th generation network adapters (ConnectX series) IB driver");
MODULE_LICENSE("Dual BSD/GPL");

struct mlx5_ib_event_work {
	struct work_struct	work;
	union {
		struct mlx5_ib_dev	      *dev;
		struct mlx5_ib_multiport_info *mpi;
	};
	bool			is_slave;
	unsigned int		event;
	void			*param;
>>>>>>> upstream/android-13
};

enum {
	MLX5_ATOMIC_SIZE_QP_8BYTES = 1 << 3,
};

static struct workqueue_struct *mlx5_ib_event_wq;
static LIST_HEAD(mlx5_ib_unaffiliated_port_list);
static LIST_HEAD(mlx5_ib_dev_list);
/*
 * This mutex should be held when accessing either of the above lists
 */
static DEFINE_MUTEX(mlx5_ib_multiport_mutex);

<<<<<<< HEAD
/* We can't use an array for xlt_emergency_page because dma_map_single
 * doesn't work on kernel modules memory
 */
static unsigned long xlt_emergency_page;
static struct mutex xlt_emergency_page_mutex;

=======
>>>>>>> upstream/android-13
struct mlx5_ib_dev *mlx5_ib_get_ibdev_from_mpi(struct mlx5_ib_multiport_info *mpi)
{
	struct mlx5_ib_dev *dev;

	mutex_lock(&mlx5_ib_multiport_mutex);
	dev = mpi->ibdev;
	mutex_unlock(&mlx5_ib_multiport_mutex);
	return dev;
}

static enum rdma_link_layer
mlx5_port_type_cap_to_rdma_ll(int port_type_cap)
{
	switch (port_type_cap) {
	case MLX5_CAP_PORT_TYPE_IB:
		return IB_LINK_LAYER_INFINIBAND;
	case MLX5_CAP_PORT_TYPE_ETH:
		return IB_LINK_LAYER_ETHERNET;
	default:
		return IB_LINK_LAYER_UNSPECIFIED;
	}
}

static enum rdma_link_layer
<<<<<<< HEAD
mlx5_ib_port_link_layer(struct ib_device *device, u8 port_num)
=======
mlx5_ib_port_link_layer(struct ib_device *device, u32 port_num)
>>>>>>> upstream/android-13
{
	struct mlx5_ib_dev *dev = to_mdev(device);
	int port_type_cap = MLX5_CAP_GEN(dev->mdev, port_type);

	return mlx5_port_type_cap_to_rdma_ll(port_type_cap);
}

static int get_port_state(struct ib_device *ibdev,
<<<<<<< HEAD
			  u8 port_num,
=======
			  u32 port_num,
>>>>>>> upstream/android-13
			  enum ib_port_state *state)
{
	struct ib_port_attr attr;
	int ret;

	memset(&attr, 0, sizeof(attr));
<<<<<<< HEAD
	ret = ibdev->query_port(ibdev, port_num, &attr);
=======
	ret = ibdev->ops.query_port(ibdev, port_num, &attr);
>>>>>>> upstream/android-13
	if (!ret)
		*state = attr.state;
	return ret;
}

<<<<<<< HEAD
=======
static struct mlx5_roce *mlx5_get_rep_roce(struct mlx5_ib_dev *dev,
					   struct net_device *ndev,
					   struct net_device *upper,
					   u32 *port_num)
{
	struct net_device *rep_ndev;
	struct mlx5_ib_port *port;
	int i;

	for (i = 0; i < dev->num_ports; i++) {
		port  = &dev->port[i];
		if (!port->rep)
			continue;

		if (upper == ndev && port->rep->vport == MLX5_VPORT_UPLINK) {
			*port_num = i + 1;
			return &port->roce;
		}

		if (upper && port->rep->vport == MLX5_VPORT_UPLINK)
			continue;

		read_lock(&port->roce.netdev_lock);
		rep_ndev = mlx5_ib_get_rep_netdev(port->rep->esw,
						  port->rep->vport);
		if (rep_ndev == ndev) {
			read_unlock(&port->roce.netdev_lock);
			*port_num = i + 1;
			return &port->roce;
		}
		read_unlock(&port->roce.netdev_lock);
	}

	return NULL;
}

>>>>>>> upstream/android-13
static int mlx5_netdev_event(struct notifier_block *this,
			     unsigned long event, void *ptr)
{
	struct mlx5_roce *roce = container_of(this, struct mlx5_roce, nb);
	struct net_device *ndev = netdev_notifier_info_to_dev(ptr);
<<<<<<< HEAD
	u8 port_num = roce->native_port_num;
=======
	u32 port_num = roce->native_port_num;
>>>>>>> upstream/android-13
	struct mlx5_core_dev *mdev;
	struct mlx5_ib_dev *ibdev;

	ibdev = roce->dev;
	mdev = mlx5_ib_get_native_port_mdev(ibdev, port_num, NULL);
	if (!mdev)
		return NOTIFY_DONE;

	switch (event) {
	case NETDEV_REGISTER:
<<<<<<< HEAD
	case NETDEV_UNREGISTER:
		write_lock(&roce->netdev_lock);
		if (ibdev->rep) {
			struct mlx5_eswitch *esw = ibdev->mdev->priv.eswitch;
			struct net_device *rep_ndev;

			rep_ndev = mlx5_ib_get_rep_netdev(esw,
							  ibdev->rep->vport);
			if (rep_ndev == ndev)
				roce->netdev = (event == NETDEV_UNREGISTER) ?
					NULL : ndev;
		} else if (ndev->dev.parent == &mdev->pdev->dev) {
			roce->netdev = (event == NETDEV_UNREGISTER) ?
				NULL : ndev;
		}
=======
		/* Should already be registered during the load */
		if (ibdev->is_rep)
			break;
		write_lock(&roce->netdev_lock);
		if (ndev->dev.parent == mdev->device)
			roce->netdev = ndev;
		write_unlock(&roce->netdev_lock);
		break;

	case NETDEV_UNREGISTER:
		/* In case of reps, ib device goes away before the netdevs */
		write_lock(&roce->netdev_lock);
		if (roce->netdev == ndev)
			roce->netdev = NULL;
>>>>>>> upstream/android-13
		write_unlock(&roce->netdev_lock);
		break;

	case NETDEV_CHANGE:
	case NETDEV_UP:
	case NETDEV_DOWN: {
		struct net_device *lag_ndev = mlx5_lag_get_roce_netdev(mdev);
		struct net_device *upper = NULL;

		if (lag_ndev) {
			upper = netdev_master_upper_dev_get(lag_ndev);
			dev_put(lag_ndev);
		}

<<<<<<< HEAD
		if ((upper == ndev || (!upper && ndev == roce->netdev))
		    && ibdev->ib_active) {
=======
		if (ibdev->is_rep)
			roce = mlx5_get_rep_roce(ibdev, ndev, upper, &port_num);
		if (!roce)
			return NOTIFY_DONE;
		if ((upper == ndev ||
		     ((!upper || ibdev->is_rep) && ndev == roce->netdev)) &&
		    ibdev->ib_active) {
>>>>>>> upstream/android-13
			struct ib_event ibev = { };
			enum ib_port_state port_state;

			if (get_port_state(&ibdev->ib_dev, port_num,
					   &port_state))
				goto done;

			if (roce->last_port_state == port_state)
				goto done;

			roce->last_port_state = port_state;
			ibev.device = &ibdev->ib_dev;
			if (port_state == IB_PORT_DOWN)
				ibev.event = IB_EVENT_PORT_ERR;
			else if (port_state == IB_PORT_ACTIVE)
				ibev.event = IB_EVENT_PORT_ACTIVE;
			else
				goto done;

			ibev.element.port_num = port_num;
			ib_dispatch_event(&ibev);
		}
		break;
	}

	default:
		break;
	}
done:
	mlx5_ib_put_native_port_mdev(ibdev, port_num);
	return NOTIFY_DONE;
}

static struct net_device *mlx5_ib_get_netdev(struct ib_device *device,
<<<<<<< HEAD
					     u8 port_num)
=======
					     u32 port_num)
>>>>>>> upstream/android-13
{
	struct mlx5_ib_dev *ibdev = to_mdev(device);
	struct net_device *ndev;
	struct mlx5_core_dev *mdev;

	mdev = mlx5_ib_get_native_port_mdev(ibdev, port_num, NULL);
	if (!mdev)
		return NULL;

	ndev = mlx5_lag_get_roce_netdev(mdev);
	if (ndev)
		goto out;

	/* Ensure ndev does not disappear before we invoke dev_hold()
	 */
<<<<<<< HEAD
	read_lock(&ibdev->roce[port_num - 1].netdev_lock);
	ndev = ibdev->roce[port_num - 1].netdev;
	if (ndev)
		dev_hold(ndev);
	read_unlock(&ibdev->roce[port_num - 1].netdev_lock);
=======
	read_lock(&ibdev->port[port_num - 1].roce.netdev_lock);
	ndev = ibdev->port[port_num - 1].roce.netdev;
	if (ndev)
		dev_hold(ndev);
	read_unlock(&ibdev->port[port_num - 1].roce.netdev_lock);
>>>>>>> upstream/android-13

out:
	mlx5_ib_put_native_port_mdev(ibdev, port_num);
	return ndev;
}

struct mlx5_core_dev *mlx5_ib_get_native_port_mdev(struct mlx5_ib_dev *ibdev,
<<<<<<< HEAD
						   u8 ib_port_num,
						   u8 *native_port_num)
=======
						   u32 ib_port_num,
						   u32 *native_port_num)
>>>>>>> upstream/android-13
{
	enum rdma_link_layer ll = mlx5_ib_port_link_layer(&ibdev->ib_dev,
							  ib_port_num);
	struct mlx5_core_dev *mdev = NULL;
	struct mlx5_ib_multiport_info *mpi;
	struct mlx5_ib_port *port;

	if (!mlx5_core_mp_enabled(ibdev->mdev) ||
	    ll != IB_LINK_LAYER_ETHERNET) {
		if (native_port_num)
			*native_port_num = ib_port_num;
		return ibdev->mdev;
	}

	if (native_port_num)
		*native_port_num = 1;

	port = &ibdev->port[ib_port_num - 1];
<<<<<<< HEAD
	if (!port)
		return NULL;

=======
>>>>>>> upstream/android-13
	spin_lock(&port->mp.mpi_lock);
	mpi = ibdev->port[ib_port_num - 1].mp.mpi;
	if (mpi && !mpi->unaffiliate) {
		mdev = mpi->mdev;
		/* If it's the master no need to refcount, it'll exist
		 * as long as the ib_dev exists.
		 */
		if (!mpi->is_master)
			mpi->mdev_refcnt++;
	}
	spin_unlock(&port->mp.mpi_lock);

	return mdev;
}

<<<<<<< HEAD
void mlx5_ib_put_native_port_mdev(struct mlx5_ib_dev *ibdev, u8 port_num)
=======
void mlx5_ib_put_native_port_mdev(struct mlx5_ib_dev *ibdev, u32 port_num)
>>>>>>> upstream/android-13
{
	enum rdma_link_layer ll = mlx5_ib_port_link_layer(&ibdev->ib_dev,
							  port_num);
	struct mlx5_ib_multiport_info *mpi;
	struct mlx5_ib_port *port;

	if (!mlx5_core_mp_enabled(ibdev->mdev) || ll != IB_LINK_LAYER_ETHERNET)
		return;

	port = &ibdev->port[port_num - 1];

	spin_lock(&port->mp.mpi_lock);
	mpi = ibdev->port[port_num - 1].mp.mpi;
	if (mpi->is_master)
		goto out;

	mpi->mdev_refcnt--;
	if (mpi->unaffiliate)
		complete(&mpi->unref_comp);
out:
	spin_unlock(&port->mp.mpi_lock);
}

<<<<<<< HEAD
static int translate_eth_proto_oper(u32 eth_proto_oper, u8 *active_speed,
				    u8 *active_width)
=======
static int translate_eth_legacy_proto_oper(u32 eth_proto_oper,
					   u16 *active_speed, u8 *active_width)
>>>>>>> upstream/android-13
{
	switch (eth_proto_oper) {
	case MLX5E_PROT_MASK(MLX5E_1000BASE_CX_SGMII):
	case MLX5E_PROT_MASK(MLX5E_1000BASE_KX):
	case MLX5E_PROT_MASK(MLX5E_100BASE_TX):
	case MLX5E_PROT_MASK(MLX5E_1000BASE_T):
		*active_width = IB_WIDTH_1X;
		*active_speed = IB_SPEED_SDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_10GBASE_T):
	case MLX5E_PROT_MASK(MLX5E_10GBASE_CX4):
	case MLX5E_PROT_MASK(MLX5E_10GBASE_KX4):
	case MLX5E_PROT_MASK(MLX5E_10GBASE_KR):
	case MLX5E_PROT_MASK(MLX5E_10GBASE_CR):
	case MLX5E_PROT_MASK(MLX5E_10GBASE_SR):
	case MLX5E_PROT_MASK(MLX5E_10GBASE_ER):
		*active_width = IB_WIDTH_1X;
		*active_speed = IB_SPEED_QDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_25GBASE_CR):
	case MLX5E_PROT_MASK(MLX5E_25GBASE_KR):
	case MLX5E_PROT_MASK(MLX5E_25GBASE_SR):
		*active_width = IB_WIDTH_1X;
		*active_speed = IB_SPEED_EDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_40GBASE_CR4):
	case MLX5E_PROT_MASK(MLX5E_40GBASE_KR4):
	case MLX5E_PROT_MASK(MLX5E_40GBASE_SR4):
	case MLX5E_PROT_MASK(MLX5E_40GBASE_LR4):
		*active_width = IB_WIDTH_4X;
		*active_speed = IB_SPEED_QDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_50GBASE_CR2):
	case MLX5E_PROT_MASK(MLX5E_50GBASE_KR2):
	case MLX5E_PROT_MASK(MLX5E_50GBASE_SR2):
		*active_width = IB_WIDTH_1X;
		*active_speed = IB_SPEED_HDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_56GBASE_R4):
		*active_width = IB_WIDTH_4X;
		*active_speed = IB_SPEED_FDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_100GBASE_CR4):
	case MLX5E_PROT_MASK(MLX5E_100GBASE_SR4):
	case MLX5E_PROT_MASK(MLX5E_100GBASE_KR4):
	case MLX5E_PROT_MASK(MLX5E_100GBASE_LR4):
		*active_width = IB_WIDTH_4X;
		*active_speed = IB_SPEED_EDR;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD
static int mlx5_query_port_roce(struct ib_device *device, u8 port_num,
				struct ib_port_attr *props)
{
	struct mlx5_ib_dev *dev = to_mdev(device);
=======
static int translate_eth_ext_proto_oper(u32 eth_proto_oper, u16 *active_speed,
					u8 *active_width)
{
	switch (eth_proto_oper) {
	case MLX5E_PROT_MASK(MLX5E_SGMII_100M):
	case MLX5E_PROT_MASK(MLX5E_1000BASE_X_SGMII):
		*active_width = IB_WIDTH_1X;
		*active_speed = IB_SPEED_SDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_5GBASE_R):
		*active_width = IB_WIDTH_1X;
		*active_speed = IB_SPEED_DDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_10GBASE_XFI_XAUI_1):
		*active_width = IB_WIDTH_1X;
		*active_speed = IB_SPEED_QDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_40GBASE_XLAUI_4_XLPPI_4):
		*active_width = IB_WIDTH_4X;
		*active_speed = IB_SPEED_QDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_25GAUI_1_25GBASE_CR_KR):
		*active_width = IB_WIDTH_1X;
		*active_speed = IB_SPEED_EDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_50GAUI_2_LAUI_2_50GBASE_CR2_KR2):
		*active_width = IB_WIDTH_2X;
		*active_speed = IB_SPEED_EDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_50GAUI_1_LAUI_1_50GBASE_CR_KR):
		*active_width = IB_WIDTH_1X;
		*active_speed = IB_SPEED_HDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_CAUI_4_100GBASE_CR4_KR4):
		*active_width = IB_WIDTH_4X;
		*active_speed = IB_SPEED_EDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_100GAUI_2_100GBASE_CR2_KR2):
		*active_width = IB_WIDTH_2X;
		*active_speed = IB_SPEED_HDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_100GAUI_1_100GBASE_CR_KR):
		*active_width = IB_WIDTH_1X;
		*active_speed = IB_SPEED_NDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_200GAUI_4_200GBASE_CR4_KR4):
		*active_width = IB_WIDTH_4X;
		*active_speed = IB_SPEED_HDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_200GAUI_2_200GBASE_CR2_KR2):
		*active_width = IB_WIDTH_2X;
		*active_speed = IB_SPEED_NDR;
		break;
	case MLX5E_PROT_MASK(MLX5E_400GAUI_4_400GBASE_CR4_KR4):
		*active_width = IB_WIDTH_4X;
		*active_speed = IB_SPEED_NDR;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int translate_eth_proto_oper(u32 eth_proto_oper, u16 *active_speed,
				    u8 *active_width, bool ext)
{
	return ext ?
		translate_eth_ext_proto_oper(eth_proto_oper, active_speed,
					     active_width) :
		translate_eth_legacy_proto_oper(eth_proto_oper, active_speed,
						active_width);
}

static int mlx5_query_port_roce(struct ib_device *device, u32 port_num,
				struct ib_port_attr *props)
{
	struct mlx5_ib_dev *dev = to_mdev(device);
	u32 out[MLX5_ST_SZ_DW(ptys_reg)] = {0};
>>>>>>> upstream/android-13
	struct mlx5_core_dev *mdev;
	struct net_device *ndev, *upper;
	enum ib_mtu ndev_ib_mtu;
	bool put_mdev = true;
<<<<<<< HEAD
	u16 qkey_viol_cntr;
	u32 eth_prot_oper;
	u8 mdev_port_num;
=======
	u32 eth_prot_oper;
	u32 mdev_port_num;
	bool ext;
>>>>>>> upstream/android-13
	int err;

	mdev = mlx5_ib_get_native_port_mdev(dev, port_num, &mdev_port_num);
	if (!mdev) {
		/* This means the port isn't affiliated yet. Get the
		 * info for the master port instead.
		 */
		put_mdev = false;
		mdev = dev->mdev;
		mdev_port_num = 1;
		port_num = 1;
	}

	/* Possible bad flows are checked before filling out props so in case
	 * of an error it will still be zeroed out.
<<<<<<< HEAD
	 */
	err = mlx5_query_port_eth_proto_oper(mdev, &eth_prot_oper,
					     mdev_port_num);
	if (err)
		goto out;
=======
	 * Use native port in case of reps
	 */
	if (dev->is_rep)
		err = mlx5_query_port_ptys(mdev, out, sizeof(out), MLX5_PTYS_EN,
					   1);
	else
		err = mlx5_query_port_ptys(mdev, out, sizeof(out), MLX5_PTYS_EN,
					   mdev_port_num);
	if (err)
		goto out;
	ext = !!MLX5_GET_ETH_PROTO(ptys_reg, out, true, eth_proto_capability);
	eth_prot_oper = MLX5_GET_ETH_PROTO(ptys_reg, out, ext, eth_proto_oper);
>>>>>>> upstream/android-13

	props->active_width     = IB_WIDTH_4X;
	props->active_speed     = IB_SPEED_QDR;

	translate_eth_proto_oper(eth_prot_oper, &props->active_speed,
<<<<<<< HEAD
				 &props->active_width);

	props->port_cap_flags |= IB_PORT_CM_SUP;
	props->ip_gids = true;

	props->gid_tbl_len      = MLX5_CAP_ROCE(dev->mdev,
						roce_address_table_size);
=======
				 &props->active_width, ext);

	if (!dev->is_rep && dev->mdev->roce.roce_en) {
		u16 qkey_viol_cntr;

		props->port_cap_flags |= IB_PORT_CM_SUP;
		props->ip_gids = true;
		props->gid_tbl_len = MLX5_CAP_ROCE(dev->mdev,
						   roce_address_table_size);
		mlx5_query_nic_vport_qkey_viol_cntr(mdev, &qkey_viol_cntr);
		props->qkey_viol_cntr = qkey_viol_cntr;
	}
>>>>>>> upstream/android-13
	props->max_mtu          = IB_MTU_4096;
	props->max_msg_sz       = 1 << MLX5_CAP_GEN(dev->mdev, log_max_msg);
	props->pkey_tbl_len     = 1;
	props->state            = IB_PORT_DOWN;
<<<<<<< HEAD
	props->phys_state       = 3;

	mlx5_query_nic_vport_qkey_viol_cntr(mdev, &qkey_viol_cntr);
	props->qkey_viol_cntr = qkey_viol_cntr;
=======
	props->phys_state       = IB_PORT_PHYS_STATE_DISABLED;
>>>>>>> upstream/android-13

	/* If this is a stub query for an unaffiliated port stop here */
	if (!put_mdev)
		goto out;

	ndev = mlx5_ib_get_netdev(device, port_num);
	if (!ndev)
		goto out;

<<<<<<< HEAD
	if (mlx5_lag_is_active(dev->mdev)) {
=======
	if (dev->lag_active) {
>>>>>>> upstream/android-13
		rcu_read_lock();
		upper = netdev_master_upper_dev_get_rcu(ndev);
		if (upper) {
			dev_put(ndev);
			ndev = upper;
			dev_hold(ndev);
		}
		rcu_read_unlock();
	}

	if (netif_running(ndev) && netif_carrier_ok(ndev)) {
		props->state      = IB_PORT_ACTIVE;
<<<<<<< HEAD
		props->phys_state = 5;
=======
		props->phys_state = IB_PORT_PHYS_STATE_LINK_UP;
>>>>>>> upstream/android-13
	}

	ndev_ib_mtu = iboe_get_mtu(ndev->mtu);

	dev_put(ndev);

	props->active_mtu	= min(props->max_mtu, ndev_ib_mtu);
out:
	if (put_mdev)
		mlx5_ib_put_native_port_mdev(dev, port_num);
	return err;
}

<<<<<<< HEAD
static int set_roce_addr(struct mlx5_ib_dev *dev, u8 port_num,
			 unsigned int index, const union ib_gid *gid,
			 const struct ib_gid_attr *attr)
{
	enum ib_gid_type gid_type = IB_GID_TYPE_IB;
	u8 roce_version = 0;
	u8 roce_l3_type = 0;
	bool vlan = false;
	u8 mac[ETH_ALEN];
	u16 vlan_id = 0;

	if (gid) {
		gid_type = attr->gid_type;
		ether_addr_copy(mac, attr->ndev->dev_addr);

		if (is_vlan_dev(attr->ndev)) {
			vlan = true;
			vlan_id = vlan_dev_vlan_id(attr->ndev);
		}
	}

	switch (gid_type) {
	case IB_GID_TYPE_IB:
=======
static int set_roce_addr(struct mlx5_ib_dev *dev, u32 port_num,
			 unsigned int index, const union ib_gid *gid,
			 const struct ib_gid_attr *attr)
{
	enum ib_gid_type gid_type;
	u16 vlan_id = 0xffff;
	u8 roce_version = 0;
	u8 roce_l3_type = 0;
	u8 mac[ETH_ALEN];
	int ret;

	gid_type = attr->gid_type;
	if (gid) {
		ret = rdma_read_gid_l2_fields(attr, &vlan_id, &mac[0]);
		if (ret)
			return ret;
	}

	switch (gid_type) {
	case IB_GID_TYPE_ROCE:
>>>>>>> upstream/android-13
		roce_version = MLX5_ROCE_VERSION_1;
		break;
	case IB_GID_TYPE_ROCE_UDP_ENCAP:
		roce_version = MLX5_ROCE_VERSION_2;
<<<<<<< HEAD
		if (ipv6_addr_v4mapped((void *)gid))
=======
		if (gid && ipv6_addr_v4mapped((void *)gid))
>>>>>>> upstream/android-13
			roce_l3_type = MLX5_ROCE_L3_TYPE_IPV4;
		else
			roce_l3_type = MLX5_ROCE_L3_TYPE_IPV6;
		break;

	default:
		mlx5_ib_warn(dev, "Unexpected GID type %u\n", gid_type);
	}

	return mlx5_core_roce_gid_set(dev->mdev, index, roce_version,
<<<<<<< HEAD
				      roce_l3_type, gid->raw, mac, vlan,
				      vlan_id, port_num);
=======
				      roce_l3_type, gid->raw, mac,
				      vlan_id < VLAN_CFI_MASK, vlan_id,
				      port_num);
>>>>>>> upstream/android-13
}

static int mlx5_ib_add_gid(const struct ib_gid_attr *attr,
			   __always_unused void **context)
{
	return set_roce_addr(to_mdev(attr->device), attr->port_num,
			     attr->index, &attr->gid, attr);
}

static int mlx5_ib_del_gid(const struct ib_gid_attr *attr,
			   __always_unused void **context)
{
	return set_roce_addr(to_mdev(attr->device), attr->port_num,
<<<<<<< HEAD
			     attr->index, NULL, NULL);
}

__be16 mlx5_get_roce_udp_sport(struct mlx5_ib_dev *dev,
			       const struct ib_gid_attr *attr)
=======
			     attr->index, NULL, attr);
}

__be16 mlx5_get_roce_udp_sport_min(const struct mlx5_ib_dev *dev,
				   const struct ib_gid_attr *attr)
>>>>>>> upstream/android-13
{
	if (attr->gid_type != IB_GID_TYPE_ROCE_UDP_ENCAP)
		return 0;

	return cpu_to_be16(MLX5_CAP_ROCE(dev->mdev, r_roce_min_src_udp_port));
}

static int mlx5_use_mad_ifc(struct mlx5_ib_dev *dev)
{
	if (MLX5_CAP_GEN(dev->mdev, port_type) == MLX5_CAP_PORT_TYPE_IB)
		return !MLX5_CAP_GEN(dev->mdev, ib_virt);
	return 0;
}

enum {
	MLX5_VPORT_ACCESS_METHOD_MAD,
	MLX5_VPORT_ACCESS_METHOD_HCA,
	MLX5_VPORT_ACCESS_METHOD_NIC,
};

static int mlx5_get_vport_access_method(struct ib_device *ibdev)
{
	if (mlx5_use_mad_ifc(to_mdev(ibdev)))
		return MLX5_VPORT_ACCESS_METHOD_MAD;

	if (mlx5_ib_port_link_layer(ibdev, 1) ==
	    IB_LINK_LAYER_ETHERNET)
		return MLX5_VPORT_ACCESS_METHOD_NIC;

	return MLX5_VPORT_ACCESS_METHOD_HCA;
}

static void get_atomic_caps(struct mlx5_ib_dev *dev,
			    u8 atomic_size_qp,
			    struct ib_device_attr *props)
{
	u8 tmp;
	u8 atomic_operations = MLX5_CAP_ATOMIC(dev->mdev, atomic_operations);
	u8 atomic_req_8B_endianness_mode =
		MLX5_CAP_ATOMIC(dev->mdev, atomic_req_8B_endianness_mode);

	/* Check if HW supports 8 bytes standard atomic operations and capable
	 * of host endianness respond
	 */
	tmp = MLX5_ATOMIC_OPS_CMP_SWAP | MLX5_ATOMIC_OPS_FETCH_ADD;
	if (((atomic_operations & tmp) == tmp) &&
	    (atomic_size_qp & MLX5_ATOMIC_SIZE_QP_8BYTES) &&
	    (atomic_req_8B_endianness_mode)) {
		props->atomic_cap = IB_ATOMIC_HCA;
	} else {
		props->atomic_cap = IB_ATOMIC_NONE;
	}
}

static void get_atomic_caps_qp(struct mlx5_ib_dev *dev,
			       struct ib_device_attr *props)
{
	u8 atomic_size_qp = MLX5_CAP_ATOMIC(dev->mdev, atomic_size_qp);

	get_atomic_caps(dev, atomic_size_qp, props);
}

<<<<<<< HEAD
static void get_atomic_caps_dc(struct mlx5_ib_dev *dev,
			       struct ib_device_attr *props)
{
	u8 atomic_size_qp = MLX5_CAP_ATOMIC(dev->mdev, atomic_size_dc);

	get_atomic_caps(dev, atomic_size_qp, props);
}

bool mlx5_ib_dc_atomic_is_supported(struct mlx5_ib_dev *dev)
{
	struct ib_device_attr props = {};

	get_atomic_caps_dc(dev, &props);
	return (props.atomic_cap == IB_ATOMIC_HCA) ? true : false;
}
=======
>>>>>>> upstream/android-13
static int mlx5_query_system_image_guid(struct ib_device *ibdev,
					__be64 *sys_image_guid)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	struct mlx5_core_dev *mdev = dev->mdev;
	u64 tmp;
	int err;

	switch (mlx5_get_vport_access_method(ibdev)) {
	case MLX5_VPORT_ACCESS_METHOD_MAD:
		return mlx5_query_mad_ifc_system_image_guid(ibdev,
							    sys_image_guid);

	case MLX5_VPORT_ACCESS_METHOD_HCA:
		err = mlx5_query_hca_vport_system_image_guid(mdev, &tmp);
		break;

	case MLX5_VPORT_ACCESS_METHOD_NIC:
		err = mlx5_query_nic_vport_system_image_guid(mdev, &tmp);
		break;

	default:
		return -EINVAL;
	}

	if (!err)
		*sys_image_guid = cpu_to_be64(tmp);

	return err;

}

static int mlx5_query_max_pkeys(struct ib_device *ibdev,
				u16 *max_pkeys)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	struct mlx5_core_dev *mdev = dev->mdev;

	switch (mlx5_get_vport_access_method(ibdev)) {
	case MLX5_VPORT_ACCESS_METHOD_MAD:
		return mlx5_query_mad_ifc_max_pkeys(ibdev, max_pkeys);

	case MLX5_VPORT_ACCESS_METHOD_HCA:
	case MLX5_VPORT_ACCESS_METHOD_NIC:
		*max_pkeys = mlx5_to_sw_pkey_sz(MLX5_CAP_GEN(mdev,
						pkey_table_size));
		return 0;

	default:
		return -EINVAL;
	}
}

static int mlx5_query_vendor_id(struct ib_device *ibdev,
				u32 *vendor_id)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);

	switch (mlx5_get_vport_access_method(ibdev)) {
	case MLX5_VPORT_ACCESS_METHOD_MAD:
		return mlx5_query_mad_ifc_vendor_id(ibdev, vendor_id);

	case MLX5_VPORT_ACCESS_METHOD_HCA:
	case MLX5_VPORT_ACCESS_METHOD_NIC:
		return mlx5_core_query_vendor_id(dev->mdev, vendor_id);

	default:
		return -EINVAL;
	}
}

static int mlx5_query_node_guid(struct mlx5_ib_dev *dev,
				__be64 *node_guid)
{
	u64 tmp;
	int err;

	switch (mlx5_get_vport_access_method(&dev->ib_dev)) {
	case MLX5_VPORT_ACCESS_METHOD_MAD:
		return mlx5_query_mad_ifc_node_guid(dev, node_guid);

	case MLX5_VPORT_ACCESS_METHOD_HCA:
		err = mlx5_query_hca_vport_node_guid(dev->mdev, &tmp);
		break;

	case MLX5_VPORT_ACCESS_METHOD_NIC:
		err = mlx5_query_nic_vport_node_guid(dev->mdev, &tmp);
		break;

	default:
		return -EINVAL;
	}

	if (!err)
		*node_guid = cpu_to_be64(tmp);

	return err;
}

struct mlx5_reg_node_desc {
	u8	desc[IB_DEVICE_NODE_DESC_MAX];
};

static int mlx5_query_node_desc(struct mlx5_ib_dev *dev, char *node_desc)
{
	struct mlx5_reg_node_desc in;

	if (mlx5_use_mad_ifc(dev))
		return mlx5_query_mad_ifc_node_desc(dev, node_desc);

	memset(&in, 0, sizeof(in));

	return mlx5_core_access_reg(dev->mdev, &in, sizeof(in), node_desc,
				    sizeof(struct mlx5_reg_node_desc),
				    MLX5_REG_NODE_DESC, 0, 0);
}

static int mlx5_ib_query_device(struct ib_device *ibdev,
				struct ib_device_attr *props,
				struct ib_udata *uhw)
{
<<<<<<< HEAD
=======
	size_t uhw_outlen = (uhw) ? uhw->outlen : 0;
>>>>>>> upstream/android-13
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	struct mlx5_core_dev *mdev = dev->mdev;
	int err = -ENOMEM;
	int max_sq_desc;
	int max_rq_sg;
	int max_sq_sg;
	u64 min_page_size = 1ull << MLX5_CAP_GEN(mdev, log_pg_sz);
	bool raw_support = !mlx5_core_mp_enabled(mdev);
	struct mlx5_ib_query_device_resp resp = {};
	size_t resp_len;
	u64 max_tso;

	resp_len = sizeof(resp.comp_mask) + sizeof(resp.response_length);
<<<<<<< HEAD
	if (uhw->outlen && uhw->outlen < resp_len)
		return -EINVAL;
	else
		resp.response_length = resp_len;

	if (uhw->inlen && !ib_is_udata_cleared(uhw, 0, uhw->inlen))
=======
	if (uhw_outlen && uhw_outlen < resp_len)
		return -EINVAL;

	resp.response_length = resp_len;

	if (uhw && uhw->inlen && !ib_is_udata_cleared(uhw, 0, uhw->inlen))
>>>>>>> upstream/android-13
		return -EINVAL;

	memset(props, 0, sizeof(*props));
	err = mlx5_query_system_image_guid(ibdev,
					   &props->sys_image_guid);
	if (err)
		return err;

<<<<<<< HEAD
	err = mlx5_query_max_pkeys(ibdev, &props->max_pkeys);
	if (err)
		return err;
=======
	props->max_pkeys = dev->pkey_table_len;
>>>>>>> upstream/android-13

	err = mlx5_query_vendor_id(ibdev, &props->vendor_id);
	if (err)
		return err;

	props->fw_ver = ((u64)fw_rev_maj(dev->mdev) << 32) |
		(fw_rev_min(dev->mdev) << 16) |
		fw_rev_sub(dev->mdev);
	props->device_cap_flags    = IB_DEVICE_CHANGE_PHY_PORT |
		IB_DEVICE_PORT_ACTIVE_EVENT		|
		IB_DEVICE_SYS_IMAGE_GUID		|
		IB_DEVICE_RC_RNR_NAK_GEN;

	if (MLX5_CAP_GEN(mdev, pkv))
		props->device_cap_flags |= IB_DEVICE_BAD_PKEY_CNTR;
	if (MLX5_CAP_GEN(mdev, qkv))
		props->device_cap_flags |= IB_DEVICE_BAD_QKEY_CNTR;
	if (MLX5_CAP_GEN(mdev, apm))
		props->device_cap_flags |= IB_DEVICE_AUTO_PATH_MIG;
	if (MLX5_CAP_GEN(mdev, xrc))
		props->device_cap_flags |= IB_DEVICE_XRC;
	if (MLX5_CAP_GEN(mdev, imaicl)) {
		props->device_cap_flags |= IB_DEVICE_MEM_WINDOW |
					   IB_DEVICE_MEM_WINDOW_TYPE_2B;
		props->max_mw = 1 << MLX5_CAP_GEN(mdev, log_max_mkey);
		/* We support 'Gappy' memory registration too */
		props->device_cap_flags |= IB_DEVICE_SG_GAPS_REG;
	}
<<<<<<< HEAD
	props->device_cap_flags |= IB_DEVICE_MEM_MGT_EXTENSIONS;
	if (MLX5_CAP_GEN(mdev, sho)) {
		props->device_cap_flags |= IB_DEVICE_SIGNATURE_HANDOVER;
=======
	/* IB_WR_REG_MR always requires changing the entity size with UMR */
	if (!MLX5_CAP_GEN(dev->mdev, umr_modify_entity_size_disabled))
		props->device_cap_flags |= IB_DEVICE_MEM_MGT_EXTENSIONS;
	if (MLX5_CAP_GEN(mdev, sho)) {
		props->device_cap_flags |= IB_DEVICE_INTEGRITY_HANDOVER;
>>>>>>> upstream/android-13
		/* At this stage no support for signature handover */
		props->sig_prot_cap = IB_PROT_T10DIF_TYPE_1 |
				      IB_PROT_T10DIF_TYPE_2 |
				      IB_PROT_T10DIF_TYPE_3;
		props->sig_guard_cap = IB_GUARD_T10DIF_CRC |
				       IB_GUARD_T10DIF_CSUM;
	}
	if (MLX5_CAP_GEN(mdev, block_lb_mc))
		props->device_cap_flags |= IB_DEVICE_BLOCK_MULTICAST_LOOPBACK;

	if (MLX5_CAP_GEN(dev->mdev, eth_net_offloads) && raw_support) {
		if (MLX5_CAP_ETH(mdev, csum_cap)) {
			/* Legacy bit to support old userspace libraries */
			props->device_cap_flags |= IB_DEVICE_RAW_IP_CSUM;
			props->raw_packet_caps |= IB_RAW_PACKET_CAP_IP_CSUM;
		}

		if (MLX5_CAP_ETH(dev->mdev, vlan_cap))
			props->raw_packet_caps |=
				IB_RAW_PACKET_CAP_CVLAN_STRIPPING;

<<<<<<< HEAD
		if (field_avail(typeof(resp), tso_caps, uhw->outlen)) {
=======
		if (offsetofend(typeof(resp), tso_caps) <= uhw_outlen) {
>>>>>>> upstream/android-13
			max_tso = MLX5_CAP_ETH(mdev, max_lso_cap);
			if (max_tso) {
				resp.tso_caps.max_tso = 1 << max_tso;
				resp.tso_caps.supported_qpts |=
					1 << IB_QPT_RAW_PACKET;
				resp.response_length += sizeof(resp.tso_caps);
			}
		}

<<<<<<< HEAD
		if (field_avail(typeof(resp), rss_caps, uhw->outlen)) {
=======
		if (offsetofend(typeof(resp), rss_caps) <= uhw_outlen) {
>>>>>>> upstream/android-13
			resp.rss_caps.rx_hash_function =
						MLX5_RX_HASH_FUNC_TOEPLITZ;
			resp.rss_caps.rx_hash_fields_mask =
						MLX5_RX_HASH_SRC_IPV4 |
						MLX5_RX_HASH_DST_IPV4 |
						MLX5_RX_HASH_SRC_IPV6 |
						MLX5_RX_HASH_DST_IPV6 |
						MLX5_RX_HASH_SRC_PORT_TCP |
						MLX5_RX_HASH_DST_PORT_TCP |
						MLX5_RX_HASH_SRC_PORT_UDP |
						MLX5_RX_HASH_DST_PORT_UDP |
						MLX5_RX_HASH_INNER;
			if (mlx5_accel_ipsec_device_caps(dev->mdev) &
			    MLX5_ACCEL_IPSEC_CAP_DEVICE)
				resp.rss_caps.rx_hash_fields_mask |=
					MLX5_RX_HASH_IPSEC_SPI;
			resp.response_length += sizeof(resp.rss_caps);
		}
	} else {
<<<<<<< HEAD
		if (field_avail(typeof(resp), tso_caps, uhw->outlen))
			resp.response_length += sizeof(resp.tso_caps);
		if (field_avail(typeof(resp), rss_caps, uhw->outlen))
=======
		if (offsetofend(typeof(resp), tso_caps) <= uhw_outlen)
			resp.response_length += sizeof(resp.tso_caps);
		if (offsetofend(typeof(resp), rss_caps) <= uhw_outlen)
>>>>>>> upstream/android-13
			resp.response_length += sizeof(resp.rss_caps);
	}

	if (MLX5_CAP_GEN(mdev, ipoib_basic_offloads)) {
		props->device_cap_flags |= IB_DEVICE_UD_IP_CSUM;
		props->device_cap_flags |= IB_DEVICE_UD_TSO;
	}

	if (MLX5_CAP_GEN(dev->mdev, rq_delay_drop) &&
	    MLX5_CAP_GEN(dev->mdev, general_notification_event) &&
	    raw_support)
		props->raw_packet_caps |= IB_RAW_PACKET_CAP_DELAY_DROP;

	if (MLX5_CAP_GEN(mdev, ipoib_enhanced_offloads) &&
	    MLX5_CAP_IPOIB_ENHANCED(mdev, csum_cap))
		props->device_cap_flags |= IB_DEVICE_UD_IP_CSUM;

	if (MLX5_CAP_GEN(dev->mdev, eth_net_offloads) &&
	    MLX5_CAP_ETH(dev->mdev, scatter_fcs) &&
	    raw_support) {
		/* Legacy bit to support old userspace libraries */
		props->device_cap_flags |= IB_DEVICE_RAW_SCATTER_FCS;
		props->raw_packet_caps |= IB_RAW_PACKET_CAP_SCATTER_FCS;
	}

	if (MLX5_CAP_DEV_MEM(mdev, memic)) {
		props->max_dm_size =
			MLX5_CAP_DEV_MEM(mdev, max_memic_size);
	}

	if (mlx5_get_flow_namespace(dev->mdev, MLX5_FLOW_NAMESPACE_BYPASS))
		props->device_cap_flags |= IB_DEVICE_MANAGED_FLOW_STEERING;

	if (MLX5_CAP_GEN(mdev, end_pad))
		props->device_cap_flags |= IB_DEVICE_PCI_WRITE_END_PADDING;

	props->vendor_part_id	   = mdev->pdev->device;
	props->hw_ver		   = mdev->pdev->revision;

	props->max_mr_size	   = ~0ull;
	props->page_size_cap	   = ~(min_page_size - 1);
	props->max_qp		   = 1 << MLX5_CAP_GEN(mdev, log_max_qp);
	props->max_qp_wr	   = 1 << MLX5_CAP_GEN(mdev, log_max_qp_sz);
	max_rq_sg =  MLX5_CAP_GEN(mdev, max_wqe_sz_rq) /
		     sizeof(struct mlx5_wqe_data_seg);
	max_sq_desc = min_t(int, MLX5_CAP_GEN(mdev, max_wqe_sz_sq), 512);
	max_sq_sg = (max_sq_desc - sizeof(struct mlx5_wqe_ctrl_seg) -
		     sizeof(struct mlx5_wqe_raddr_seg)) /
		sizeof(struct mlx5_wqe_data_seg);
	props->max_send_sge = max_sq_sg;
	props->max_recv_sge = max_rq_sg;
	props->max_sge_rd	   = MLX5_MAX_SGE_RD;
	props->max_cq		   = 1 << MLX5_CAP_GEN(mdev, log_max_cq);
	props->max_cqe = (1 << MLX5_CAP_GEN(mdev, log_max_cq_sz)) - 1;
	props->max_mr		   = 1 << MLX5_CAP_GEN(mdev, log_max_mkey);
	props->max_pd		   = 1 << MLX5_CAP_GEN(mdev, log_max_pd);
	props->max_qp_rd_atom	   = 1 << MLX5_CAP_GEN(mdev, log_max_ra_req_qp);
	props->max_qp_init_rd_atom = 1 << MLX5_CAP_GEN(mdev, log_max_ra_res_qp);
	props->max_srq		   = 1 << MLX5_CAP_GEN(mdev, log_max_srq);
	props->max_srq_wr = (1 << MLX5_CAP_GEN(mdev, log_max_srq_sz)) - 1;
	props->local_ca_ack_delay  = MLX5_CAP_GEN(mdev, local_ca_ack_delay);
	props->max_res_rd_atom	   = props->max_qp_rd_atom * props->max_qp;
	props->max_srq_sge	   = max_rq_sg - 1;
	props->max_fast_reg_page_list_len =
		1 << MLX5_CAP_GEN(mdev, log_max_klm_list_size);
<<<<<<< HEAD
=======
	props->max_pi_fast_reg_page_list_len =
		props->max_fast_reg_page_list_len / 2;
	props->max_sgl_rd =
		MLX5_CAP_GEN(mdev, max_sgl_for_optimized_performance);
>>>>>>> upstream/android-13
	get_atomic_caps_qp(dev, props);
	props->masked_atomic_cap   = IB_ATOMIC_NONE;
	props->max_mcast_grp	   = 1 << MLX5_CAP_GEN(mdev, log_max_mcg);
	props->max_mcast_qp_attach = MLX5_CAP_GEN(mdev, max_qp_mcg);
	props->max_total_mcast_qp_attach = props->max_mcast_qp_attach *
					   props->max_mcast_grp;
<<<<<<< HEAD
	props->max_map_per_fmr = INT_MAX; /* no limit in ConnectIB */
=======
>>>>>>> upstream/android-13
	props->max_ah = INT_MAX;
	props->hca_core_clock = MLX5_CAP_GEN(mdev, device_frequency_khz);
	props->timestamp_mask = 0x7FFFFFFFFFFFFFFFULL;

<<<<<<< HEAD
#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
	if (MLX5_CAP_GEN(mdev, pg))
		props->device_cap_flags |= IB_DEVICE_ON_DEMAND_PAGING;
	props->odp_caps = dev->odp_caps;
#endif
=======
	if (IS_ENABLED(CONFIG_INFINIBAND_ON_DEMAND_PAGING)) {
		if (dev->odp_caps.general_caps & IB_ODP_SUPPORT)
			props->device_cap_flags |= IB_DEVICE_ON_DEMAND_PAGING;
		props->odp_caps = dev->odp_caps;
		if (!uhw) {
			/* ODP for kernel QPs is not implemented for receive
			 * WQEs and SRQ WQEs
			 */
			props->odp_caps.per_transport_caps.rc_odp_caps &=
				~(IB_ODP_SUPPORT_READ |
				  IB_ODP_SUPPORT_SRQ_RECV);
			props->odp_caps.per_transport_caps.uc_odp_caps &=
				~(IB_ODP_SUPPORT_READ |
				  IB_ODP_SUPPORT_SRQ_RECV);
			props->odp_caps.per_transport_caps.ud_odp_caps &=
				~(IB_ODP_SUPPORT_READ |
				  IB_ODP_SUPPORT_SRQ_RECV);
			props->odp_caps.per_transport_caps.xrc_odp_caps &=
				~(IB_ODP_SUPPORT_READ |
				  IB_ODP_SUPPORT_SRQ_RECV);
		}
	}
>>>>>>> upstream/android-13

	if (MLX5_CAP_GEN(mdev, cd))
		props->device_cap_flags |= IB_DEVICE_CROSS_CHANNEL;

<<<<<<< HEAD
	if (!mlx5_core_is_pf(mdev))
=======
	if (mlx5_core_is_vf(mdev))
>>>>>>> upstream/android-13
		props->device_cap_flags |= IB_DEVICE_VIRTUAL_FUNCTION;

	if (mlx5_ib_port_link_layer(ibdev, 1) ==
	    IB_LINK_LAYER_ETHERNET && raw_support) {
		props->rss_caps.max_rwq_indirection_tables =
			1 << MLX5_CAP_GEN(dev->mdev, log_max_rqt);
		props->rss_caps.max_rwq_indirection_table_size =
			1 << MLX5_CAP_GEN(dev->mdev, log_max_rqt_size);
		props->rss_caps.supported_qpts = 1 << IB_QPT_RAW_PACKET;
		props->max_wq_type_rq =
			1 << MLX5_CAP_GEN(dev->mdev, log_max_rq);
	}

	if (MLX5_CAP_GEN(mdev, tag_matching)) {
		props->tm_caps.max_num_tags =
			(1 << MLX5_CAP_GEN(mdev, log_tag_matching_list_sz)) - 1;
		props->tm_caps.max_ops =
			1 << MLX5_CAP_GEN(mdev, log_max_qp_sz);
		props->tm_caps.max_sge = MLX5_TM_MAX_SGE;
	}

	if (MLX5_CAP_GEN(mdev, tag_matching) &&
	    MLX5_CAP_GEN(mdev, rndv_offload_rc)) {
		props->tm_caps.flags = IB_TM_CAP_RNDV_RC;
		props->tm_caps.max_rndv_hdr_size = MLX5_TM_MAX_RNDV_MSG_SIZE;
	}

	if (MLX5_CAP_GEN(dev->mdev, cq_moderation)) {
		props->cq_caps.max_cq_moderation_count =
						MLX5_MAX_CQ_COUNT;
		props->cq_caps.max_cq_moderation_period =
						MLX5_MAX_CQ_PERIOD;
	}

<<<<<<< HEAD
	if (field_avail(typeof(resp), cqe_comp_caps, uhw->outlen)) {
=======
	if (offsetofend(typeof(resp), cqe_comp_caps) <= uhw_outlen) {
>>>>>>> upstream/android-13
		resp.response_length += sizeof(resp.cqe_comp_caps);

		if (MLX5_CAP_GEN(dev->mdev, cqe_compression)) {
			resp.cqe_comp_caps.max_num =
				MLX5_CAP_GEN(dev->mdev,
					     cqe_compression_max_num);

			resp.cqe_comp_caps.supported_format =
				MLX5_IB_CQE_RES_FORMAT_HASH |
				MLX5_IB_CQE_RES_FORMAT_CSUM;

			if (MLX5_CAP_GEN(dev->mdev, mini_cqe_resp_stride_index))
				resp.cqe_comp_caps.supported_format |=
					MLX5_IB_CQE_RES_FORMAT_CSUM_STRIDX;
		}
	}

<<<<<<< HEAD
	if (field_avail(typeof(resp), packet_pacing_caps, uhw->outlen) &&
=======
	if (offsetofend(typeof(resp), packet_pacing_caps) <= uhw_outlen &&
>>>>>>> upstream/android-13
	    raw_support) {
		if (MLX5_CAP_QOS(mdev, packet_pacing) &&
		    MLX5_CAP_GEN(mdev, qos)) {
			resp.packet_pacing_caps.qp_rate_limit_max =
				MLX5_CAP_QOS(mdev, packet_pacing_max_rate);
			resp.packet_pacing_caps.qp_rate_limit_min =
				MLX5_CAP_QOS(mdev, packet_pacing_min_rate);
			resp.packet_pacing_caps.supported_qpts |=
				1 << IB_QPT_RAW_PACKET;
			if (MLX5_CAP_QOS(mdev, packet_pacing_burst_bound) &&
			    MLX5_CAP_QOS(mdev, packet_pacing_typical_size))
				resp.packet_pacing_caps.cap_flags |=
					MLX5_IB_PP_SUPPORT_BURST;
		}
		resp.response_length += sizeof(resp.packet_pacing_caps);
	}

<<<<<<< HEAD
	if (field_avail(typeof(resp), mlx5_ib_support_multi_pkt_send_wqes,
			uhw->outlen)) {
=======
	if (offsetofend(typeof(resp), mlx5_ib_support_multi_pkt_send_wqes) <=
	    uhw_outlen) {
>>>>>>> upstream/android-13
		if (MLX5_CAP_ETH(mdev, multi_pkt_send_wqe))
			resp.mlx5_ib_support_multi_pkt_send_wqes =
				MLX5_IB_ALLOW_MPW;

		if (MLX5_CAP_ETH(mdev, enhanced_multi_pkt_send_wqe))
			resp.mlx5_ib_support_multi_pkt_send_wqes |=
				MLX5_IB_SUPPORT_EMPW;

		resp.response_length +=
			sizeof(resp.mlx5_ib_support_multi_pkt_send_wqes);
	}

<<<<<<< HEAD
	if (field_avail(typeof(resp), flags, uhw->outlen)) {
=======
	if (offsetofend(typeof(resp), flags) <= uhw_outlen) {
>>>>>>> upstream/android-13
		resp.response_length += sizeof(resp.flags);

		if (MLX5_CAP_GEN(mdev, cqe_compression_128))
			resp.flags |=
				MLX5_IB_QUERY_DEV_RESP_FLAGS_CQE_128B_COMP;

		if (MLX5_CAP_GEN(mdev, cqe_128_always))
			resp.flags |= MLX5_IB_QUERY_DEV_RESP_FLAGS_CQE_128B_PAD;
<<<<<<< HEAD
	}

	if (field_avail(typeof(resp), sw_parsing_caps,
			uhw->outlen)) {
=======
		if (MLX5_CAP_GEN(mdev, qp_packet_based))
			resp.flags |=
				MLX5_IB_QUERY_DEV_RESP_PACKET_BASED_CREDIT_MODE;

		resp.flags |= MLX5_IB_QUERY_DEV_RESP_FLAGS_SCAT2CQE_DCT;
	}

	if (offsetofend(typeof(resp), sw_parsing_caps) <= uhw_outlen) {
>>>>>>> upstream/android-13
		resp.response_length += sizeof(resp.sw_parsing_caps);
		if (MLX5_CAP_ETH(mdev, swp)) {
			resp.sw_parsing_caps.sw_parsing_offloads |=
				MLX5_IB_SW_PARSING;

			if (MLX5_CAP_ETH(mdev, swp_csum))
				resp.sw_parsing_caps.sw_parsing_offloads |=
					MLX5_IB_SW_PARSING_CSUM;

			if (MLX5_CAP_ETH(mdev, swp_lso))
				resp.sw_parsing_caps.sw_parsing_offloads |=
					MLX5_IB_SW_PARSING_LSO;

			if (resp.sw_parsing_caps.sw_parsing_offloads)
				resp.sw_parsing_caps.supported_qpts =
					BIT(IB_QPT_RAW_PACKET);
		}
	}

<<<<<<< HEAD
	if (field_avail(typeof(resp), striding_rq_caps, uhw->outlen) &&
=======
	if (offsetofend(typeof(resp), striding_rq_caps) <= uhw_outlen &&
>>>>>>> upstream/android-13
	    raw_support) {
		resp.response_length += sizeof(resp.striding_rq_caps);
		if (MLX5_CAP_GEN(mdev, striding_rq)) {
			resp.striding_rq_caps.min_single_stride_log_num_of_bytes =
				MLX5_MIN_SINGLE_STRIDE_LOG_NUM_BYTES;
			resp.striding_rq_caps.max_single_stride_log_num_of_bytes =
				MLX5_MAX_SINGLE_STRIDE_LOG_NUM_BYTES;
<<<<<<< HEAD
			resp.striding_rq_caps.min_single_wqe_log_num_of_strides =
				MLX5_MIN_SINGLE_WQE_LOG_NUM_STRIDES;
=======
			if (MLX5_CAP_GEN(dev->mdev, ext_stride_num_range))
				resp.striding_rq_caps
					.min_single_wqe_log_num_of_strides =
					MLX5_EXT_MIN_SINGLE_WQE_LOG_NUM_STRIDES;
			else
				resp.striding_rq_caps
					.min_single_wqe_log_num_of_strides =
					MLX5_MIN_SINGLE_WQE_LOG_NUM_STRIDES;
>>>>>>> upstream/android-13
			resp.striding_rq_caps.max_single_wqe_log_num_of_strides =
				MLX5_MAX_SINGLE_WQE_LOG_NUM_STRIDES;
			resp.striding_rq_caps.supported_qpts =
				BIT(IB_QPT_RAW_PACKET);
		}
	}

<<<<<<< HEAD
	if (field_avail(typeof(resp), tunnel_offloads_caps,
			uhw->outlen)) {
=======
	if (offsetofend(typeof(resp), tunnel_offloads_caps) <= uhw_outlen) {
>>>>>>> upstream/android-13
		resp.response_length += sizeof(resp.tunnel_offloads_caps);
		if (MLX5_CAP_ETH(mdev, tunnel_stateless_vxlan))
			resp.tunnel_offloads_caps |=
				MLX5_IB_TUNNELED_OFFLOADS_VXLAN;
		if (MLX5_CAP_ETH(mdev, tunnel_stateless_geneve_rx))
			resp.tunnel_offloads_caps |=
				MLX5_IB_TUNNELED_OFFLOADS_GENEVE;
		if (MLX5_CAP_ETH(mdev, tunnel_stateless_gre))
			resp.tunnel_offloads_caps |=
				MLX5_IB_TUNNELED_OFFLOADS_GRE;
		if (MLX5_CAP_ETH(mdev, tunnel_stateless_mpls_over_gre))
			resp.tunnel_offloads_caps |=
				MLX5_IB_TUNNELED_OFFLOADS_MPLS_GRE;
		if (MLX5_CAP_ETH(mdev, tunnel_stateless_mpls_over_udp))
			resp.tunnel_offloads_caps |=
				MLX5_IB_TUNNELED_OFFLOADS_MPLS_UDP;
	}

<<<<<<< HEAD
	if (uhw->outlen) {
=======
	if (offsetofend(typeof(resp), dci_streams_caps) <= uhw_outlen) {
		resp.response_length += sizeof(resp.dci_streams_caps);

		resp.dci_streams_caps.max_log_num_concurent =
			MLX5_CAP_GEN(mdev, log_max_dci_stream_channels);

		resp.dci_streams_caps.max_log_num_errored =
			MLX5_CAP_GEN(mdev, log_max_dci_errored_streams);
	}

	if (uhw_outlen) {
>>>>>>> upstream/android-13
		err = ib_copy_to_udata(uhw, &resp, resp.response_length);

		if (err)
			return err;
	}

	return 0;
}

<<<<<<< HEAD
enum mlx5_ib_width {
	MLX5_IB_WIDTH_1X	= 1 << 0,
	MLX5_IB_WIDTH_2X	= 1 << 1,
	MLX5_IB_WIDTH_4X	= 1 << 2,
	MLX5_IB_WIDTH_8X	= 1 << 3,
	MLX5_IB_WIDTH_12X	= 1 << 4
};

static void translate_active_width(struct ib_device *ibdev, u8 active_width,
				  u8 *ib_width)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);

	if (active_width & MLX5_IB_WIDTH_1X)
		*ib_width = IB_WIDTH_1X;
	else if (active_width & MLX5_IB_WIDTH_4X)
		*ib_width = IB_WIDTH_4X;
	else if (active_width & MLX5_IB_WIDTH_8X)
		*ib_width = IB_WIDTH_8X;
	else if (active_width & MLX5_IB_WIDTH_12X)
		*ib_width = IB_WIDTH_12X;
	else {
		mlx5_ib_dbg(dev, "Invalid active_width %d, setting width to default value: 4x\n",
			    (int)active_width);
=======
static void translate_active_width(struct ib_device *ibdev, u16 active_width,
				   u8 *ib_width)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);

	if (active_width & MLX5_PTYS_WIDTH_1X)
		*ib_width = IB_WIDTH_1X;
	else if (active_width & MLX5_PTYS_WIDTH_2X)
		*ib_width = IB_WIDTH_2X;
	else if (active_width & MLX5_PTYS_WIDTH_4X)
		*ib_width = IB_WIDTH_4X;
	else if (active_width & MLX5_PTYS_WIDTH_8X)
		*ib_width = IB_WIDTH_8X;
	else if (active_width & MLX5_PTYS_WIDTH_12X)
		*ib_width = IB_WIDTH_12X;
	else {
		mlx5_ib_dbg(dev, "Invalid active_width %d, setting width to default value: 4x\n",
			    active_width);
>>>>>>> upstream/android-13
		*ib_width = IB_WIDTH_4X;
	}

	return;
}

static int mlx5_mtu_to_ib_mtu(int mtu)
{
	switch (mtu) {
	case 256: return 1;
	case 512: return 2;
	case 1024: return 3;
	case 2048: return 4;
	case 4096: return 5;
	default:
		pr_warn("invalid mtu\n");
		return -1;
	}
}

enum ib_max_vl_num {
	__IB_MAX_VL_0		= 1,
	__IB_MAX_VL_0_1		= 2,
	__IB_MAX_VL_0_3		= 3,
	__IB_MAX_VL_0_7		= 4,
	__IB_MAX_VL_0_14	= 5,
};

enum mlx5_vl_hw_cap {
	MLX5_VL_HW_0	= 1,
	MLX5_VL_HW_0_1	= 2,
	MLX5_VL_HW_0_2	= 3,
	MLX5_VL_HW_0_3	= 4,
	MLX5_VL_HW_0_4	= 5,
	MLX5_VL_HW_0_5	= 6,
	MLX5_VL_HW_0_6	= 7,
	MLX5_VL_HW_0_7	= 8,
	MLX5_VL_HW_0_14	= 15
};

static int translate_max_vl_num(struct ib_device *ibdev, u8 vl_hw_cap,
				u8 *max_vl_num)
{
	switch (vl_hw_cap) {
	case MLX5_VL_HW_0:
		*max_vl_num = __IB_MAX_VL_0;
		break;
	case MLX5_VL_HW_0_1:
		*max_vl_num = __IB_MAX_VL_0_1;
		break;
	case MLX5_VL_HW_0_3:
		*max_vl_num = __IB_MAX_VL_0_3;
		break;
	case MLX5_VL_HW_0_7:
		*max_vl_num = __IB_MAX_VL_0_7;
		break;
	case MLX5_VL_HW_0_14:
		*max_vl_num = __IB_MAX_VL_0_14;
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD
static int mlx5_query_hca_port(struct ib_device *ibdev, u8 port,
=======
static int mlx5_query_hca_port(struct ib_device *ibdev, u32 port,
>>>>>>> upstream/android-13
			       struct ib_port_attr *props)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	struct mlx5_core_dev *mdev = dev->mdev;
	struct mlx5_hca_vport_context *rep;
	u16 max_mtu;
	u16 oper_mtu;
	int err;
<<<<<<< HEAD
	u8 ib_link_width_oper;
=======
	u16 ib_link_width_oper;
>>>>>>> upstream/android-13
	u8 vl_hw_cap;

	rep = kzalloc(sizeof(*rep), GFP_KERNEL);
	if (!rep) {
		err = -ENOMEM;
		goto out;
	}

	/* props being zeroed by the caller, avoid zeroing it here */

	err = mlx5_query_hca_vport_context(mdev, 0, port, 0, rep);
	if (err)
		goto out;

	props->lid		= rep->lid;
	props->lmc		= rep->lmc;
	props->sm_lid		= rep->sm_lid;
	props->sm_sl		= rep->sm_sl;
	props->state		= rep->vport_state;
	props->phys_state	= rep->port_physical_state;
	props->port_cap_flags	= rep->cap_mask1;
	props->gid_tbl_len	= mlx5_get_gid_table_len(MLX5_CAP_GEN(mdev, gid_table_size));
	props->max_msg_sz	= 1 << MLX5_CAP_GEN(mdev, log_max_msg);
	props->pkey_tbl_len	= mlx5_to_sw_pkey_sz(MLX5_CAP_GEN(mdev, pkey_table_size));
	props->bad_pkey_cntr	= rep->pkey_violation_counter;
	props->qkey_viol_cntr	= rep->qkey_violation_counter;
	props->subnet_timeout	= rep->subnet_timeout;
	props->init_type_reply	= rep->init_type_reply;

<<<<<<< HEAD
	err = mlx5_query_port_link_width_oper(mdev, &ib_link_width_oper, port);
=======
	if (props->port_cap_flags & IB_PORT_CAP_MASK2_SUP)
		props->port_cap_flags2 = rep->cap_mask2;

	err = mlx5_query_ib_port_oper(mdev, &ib_link_width_oper,
				      &props->active_speed, port);
>>>>>>> upstream/android-13
	if (err)
		goto out;

	translate_active_width(ibdev, ib_link_width_oper, &props->active_width);

<<<<<<< HEAD
	err = mlx5_query_port_ib_proto_oper(mdev, &props->active_speed, port);
	if (err)
		goto out;

=======
>>>>>>> upstream/android-13
	mlx5_query_port_max_mtu(mdev, &max_mtu, port);

	props->max_mtu = mlx5_mtu_to_ib_mtu(max_mtu);

	mlx5_query_port_oper_mtu(mdev, &oper_mtu, port);

	props->active_mtu = mlx5_mtu_to_ib_mtu(oper_mtu);

	err = mlx5_query_port_vl_hw_cap(mdev, &vl_hw_cap, port);
	if (err)
		goto out;

	err = translate_max_vl_num(ibdev, vl_hw_cap,
				   &props->max_vl_num);
out:
	kfree(rep);
	return err;
}

<<<<<<< HEAD
int mlx5_ib_query_port(struct ib_device *ibdev, u8 port,
=======
int mlx5_ib_query_port(struct ib_device *ibdev, u32 port,
>>>>>>> upstream/android-13
		       struct ib_port_attr *props)
{
	unsigned int count;
	int ret;

	switch (mlx5_get_vport_access_method(ibdev)) {
	case MLX5_VPORT_ACCESS_METHOD_MAD:
		ret = mlx5_query_mad_ifc_port(ibdev, port, props);
		break;

	case MLX5_VPORT_ACCESS_METHOD_HCA:
		ret = mlx5_query_hca_port(ibdev, port, props);
		break;

	case MLX5_VPORT_ACCESS_METHOD_NIC:
		ret = mlx5_query_port_roce(ibdev, port, props);
		break;

	default:
		ret = -EINVAL;
	}

	if (!ret && props) {
		struct mlx5_ib_dev *dev = to_mdev(ibdev);
		struct mlx5_core_dev *mdev;
		bool put_mdev = true;

		mdev = mlx5_ib_get_native_port_mdev(dev, port, NULL);
		if (!mdev) {
			/* If the port isn't affiliated yet query the master.
			 * The master and slave will have the same values.
			 */
			mdev = dev->mdev;
			port = 1;
			put_mdev = false;
		}
		count = mlx5_core_reserved_gids_count(mdev);
		if (put_mdev)
			mlx5_ib_put_native_port_mdev(dev, port);
		props->gid_tbl_len -= count;
	}
	return ret;
}

<<<<<<< HEAD
static int mlx5_ib_rep_query_port(struct ib_device *ibdev, u8 port,
				  struct ib_port_attr *props)
{
	int ret;

	/* Only link layer == ethernet is valid for representors */
	ret = mlx5_query_port_roce(ibdev, port, props);
	if (ret || !props)
		return ret;

	/* We don't support GIDS */
	props->gid_tbl_len = 0;

	return ret;
}

static int mlx5_ib_query_gid(struct ib_device *ibdev, u8 port, int index,
=======
static int mlx5_ib_rep_query_port(struct ib_device *ibdev, u32 port,
				  struct ib_port_attr *props)
{
	return mlx5_query_port_roce(ibdev, port, props);
}

static int mlx5_ib_rep_query_pkey(struct ib_device *ibdev, u32 port, u16 index,
				  u16 *pkey)
{
	/* Default special Pkey for representor device port as per the
	 * IB specification 1.3 section 10.9.1.2.
	 */
	*pkey = 0xffff;
	return 0;
}

static int mlx5_ib_query_gid(struct ib_device *ibdev, u32 port, int index,
>>>>>>> upstream/android-13
			     union ib_gid *gid)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	struct mlx5_core_dev *mdev = dev->mdev;

	switch (mlx5_get_vport_access_method(ibdev)) {
	case MLX5_VPORT_ACCESS_METHOD_MAD:
		return mlx5_query_mad_ifc_gids(ibdev, port, index, gid);

	case MLX5_VPORT_ACCESS_METHOD_HCA:
		return mlx5_query_hca_vport_gid(mdev, 0, port, 0, index, gid);

	default:
		return -EINVAL;
	}

}

<<<<<<< HEAD
static int mlx5_query_hca_nic_pkey(struct ib_device *ibdev, u8 port,
=======
static int mlx5_query_hca_nic_pkey(struct ib_device *ibdev, u32 port,
>>>>>>> upstream/android-13
				   u16 index, u16 *pkey)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	struct mlx5_core_dev *mdev;
	bool put_mdev = true;
<<<<<<< HEAD
	u8 mdev_port_num;
=======
	u32 mdev_port_num;
>>>>>>> upstream/android-13
	int err;

	mdev = mlx5_ib_get_native_port_mdev(dev, port, &mdev_port_num);
	if (!mdev) {
		/* The port isn't affiliated yet, get the PKey from the master
		 * port. For RoCE the PKey tables will be the same.
		 */
		put_mdev = false;
		mdev = dev->mdev;
		mdev_port_num = 1;
	}

	err = mlx5_query_hca_vport_pkey(mdev, 0, mdev_port_num, 0,
					index, pkey);
	if (put_mdev)
		mlx5_ib_put_native_port_mdev(dev, port);

	return err;
}

<<<<<<< HEAD
static int mlx5_ib_query_pkey(struct ib_device *ibdev, u8 port, u16 index,
=======
static int mlx5_ib_query_pkey(struct ib_device *ibdev, u32 port, u16 index,
>>>>>>> upstream/android-13
			      u16 *pkey)
{
	switch (mlx5_get_vport_access_method(ibdev)) {
	case MLX5_VPORT_ACCESS_METHOD_MAD:
		return mlx5_query_mad_ifc_pkey(ibdev, port, index, pkey);

	case MLX5_VPORT_ACCESS_METHOD_HCA:
	case MLX5_VPORT_ACCESS_METHOD_NIC:
		return mlx5_query_hca_nic_pkey(ibdev, port, index, pkey);
	default:
		return -EINVAL;
	}
}

static int mlx5_ib_modify_device(struct ib_device *ibdev, int mask,
				 struct ib_device_modify *props)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	struct mlx5_reg_node_desc in;
	struct mlx5_reg_node_desc out;
	int err;

	if (mask & ~IB_DEVICE_MODIFY_NODE_DESC)
		return -EOPNOTSUPP;

	if (!(mask & IB_DEVICE_MODIFY_NODE_DESC))
		return 0;

	/*
	 * If possible, pass node desc to FW, so it can generate
	 * a 144 trap.  If cmd fails, just ignore.
	 */
	memcpy(&in, props->node_desc, IB_DEVICE_NODE_DESC_MAX);
	err = mlx5_core_access_reg(dev->mdev, &in, sizeof(in), &out,
				   sizeof(out), MLX5_REG_NODE_DESC, 0, 1);
	if (err)
		return err;

	memcpy(ibdev->node_desc, props->node_desc, IB_DEVICE_NODE_DESC_MAX);

	return err;
}

<<<<<<< HEAD
static int set_port_caps_atomic(struct mlx5_ib_dev *dev, u8 port_num, u32 mask,
=======
static int set_port_caps_atomic(struct mlx5_ib_dev *dev, u32 port_num, u32 mask,
>>>>>>> upstream/android-13
				u32 value)
{
	struct mlx5_hca_vport_context ctx = {};
	struct mlx5_core_dev *mdev;
<<<<<<< HEAD
	u8 mdev_port_num;
=======
	u32 mdev_port_num;
>>>>>>> upstream/android-13
	int err;

	mdev = mlx5_ib_get_native_port_mdev(dev, port_num, &mdev_port_num);
	if (!mdev)
		return -ENODEV;

	err = mlx5_query_hca_vport_context(mdev, 0, mdev_port_num, 0, &ctx);
	if (err)
		goto out;

	if (~ctx.cap_mask1_perm & mask) {
		mlx5_ib_warn(dev, "trying to change bitmask 0x%X but change supported 0x%X\n",
			     mask, ctx.cap_mask1_perm);
		err = -EINVAL;
		goto out;
	}

	ctx.cap_mask1 = value;
	ctx.cap_mask1_perm = mask;
	err = mlx5_core_modify_hca_vport_context(mdev, 0, mdev_port_num,
						 0, &ctx);

out:
	mlx5_ib_put_native_port_mdev(dev, port_num);

	return err;
}

<<<<<<< HEAD
static int mlx5_ib_modify_port(struct ib_device *ibdev, u8 port, int mask,
=======
static int mlx5_ib_modify_port(struct ib_device *ibdev, u32 port, int mask,
>>>>>>> upstream/android-13
			       struct ib_port_modify *props)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	struct ib_port_attr attr;
	u32 tmp;
	int err;
	u32 change_mask;
	u32 value;
	bool is_ib = (mlx5_ib_port_link_layer(ibdev, port) ==
		      IB_LINK_LAYER_INFINIBAND);

	/* CM layer calls ib_modify_port() regardless of the link layer. For
	 * Ethernet ports, qkey violation and Port capabilities are meaningless.
	 */
	if (!is_ib)
		return 0;

	if (MLX5_CAP_GEN(dev->mdev, ib_virt) && is_ib) {
		change_mask = props->clr_port_cap_mask | props->set_port_cap_mask;
		value = ~props->clr_port_cap_mask | props->set_port_cap_mask;
		return set_port_caps_atomic(dev, port, change_mask, value);
	}

	mutex_lock(&dev->cap_mask_mutex);

	err = ib_query_port(ibdev, port, &attr);
	if (err)
		goto out;

	tmp = (attr.port_cap_flags | props->set_port_cap_mask) &
		~props->clr_port_cap_mask;

	err = mlx5_set_port_caps(dev->mdev, port, tmp);

out:
	mutex_unlock(&dev->cap_mask_mutex);
	return err;
}

static void print_lib_caps(struct mlx5_ib_dev *dev, u64 caps)
{
	mlx5_ib_dbg(dev, "MLX5_LIB_CAP_4K_UAR = %s\n",
		    caps & MLX5_LIB_CAP_4K_UAR ? "y" : "n");
}

static u16 calc_dynamic_bfregs(int uars_per_sys_page)
{
	/* Large page with non 4k uar support might limit the dynamic size */
	if (uars_per_sys_page == 1  && PAGE_SIZE > 4096)
		return MLX5_MIN_DYN_BFREGS;

	return MLX5_MAX_DYN_BFREGS;
}

static int calc_total_bfregs(struct mlx5_ib_dev *dev, bool lib_uar_4k,
			     struct mlx5_ib_alloc_ucontext_req_v2 *req,
			     struct mlx5_bfreg_info *bfregi)
{
	int uars_per_sys_page;
	int bfregs_per_sys_page;
	int ref_bfregs = req->total_num_bfregs;

	if (req->total_num_bfregs == 0)
		return -EINVAL;

	BUILD_BUG_ON(MLX5_MAX_BFREGS % MLX5_NON_FP_BFREGS_IN_PAGE);
	BUILD_BUG_ON(MLX5_MAX_BFREGS < MLX5_NON_FP_BFREGS_IN_PAGE);

	if (req->total_num_bfregs > MLX5_MAX_BFREGS)
		return -ENOMEM;

	uars_per_sys_page = get_uars_per_sys_page(dev, lib_uar_4k);
	bfregs_per_sys_page = uars_per_sys_page * MLX5_NON_FP_BFREGS_PER_UAR;
	/* This holds the required static allocation asked by the user */
	req->total_num_bfregs = ALIGN(req->total_num_bfregs, bfregs_per_sys_page);
	if (req->num_low_latency_bfregs > req->total_num_bfregs - 1)
		return -EINVAL;

	bfregi->num_static_sys_pages = req->total_num_bfregs / bfregs_per_sys_page;
	bfregi->num_dyn_bfregs = ALIGN(calc_dynamic_bfregs(uars_per_sys_page), bfregs_per_sys_page);
	bfregi->total_num_bfregs = req->total_num_bfregs + bfregi->num_dyn_bfregs;
	bfregi->num_sys_pages = bfregi->total_num_bfregs / bfregs_per_sys_page;

	mlx5_ib_dbg(dev, "uar_4k: fw support %s, lib support %s, user requested %d bfregs, allocated %d, total bfregs %d, using %d sys pages\n",
		    MLX5_CAP_GEN(dev->mdev, uar_4k) ? "yes" : "no",
		    lib_uar_4k ? "yes" : "no", ref_bfregs,
		    req->total_num_bfregs, bfregi->total_num_bfregs,
		    bfregi->num_sys_pages);

	return 0;
}

static int allocate_uars(struct mlx5_ib_dev *dev, struct mlx5_ib_ucontext *context)
{
	struct mlx5_bfreg_info *bfregi;
	int err;
	int i;

	bfregi = &context->bfregi;
	for (i = 0; i < bfregi->num_static_sys_pages; i++) {
		err = mlx5_cmd_alloc_uar(dev->mdev, &bfregi->sys_pages[i]);
		if (err)
			goto error;

		mlx5_ib_dbg(dev, "allocated uar %d\n", bfregi->sys_pages[i]);
	}

	for (i = bfregi->num_static_sys_pages; i < bfregi->num_sys_pages; i++)
		bfregi->sys_pages[i] = MLX5_IB_INVALID_UAR_INDEX;

	return 0;

error:
	for (--i; i >= 0; i--)
		if (mlx5_cmd_free_uar(dev->mdev, bfregi->sys_pages[i]))
			mlx5_ib_warn(dev, "failed to free uar %d\n", i);

	return err;
}

static void deallocate_uars(struct mlx5_ib_dev *dev,
			    struct mlx5_ib_ucontext *context)
{
	struct mlx5_bfreg_info *bfregi;
	int i;

	bfregi = &context->bfregi;
	for (i = 0; i < bfregi->num_sys_pages; i++)
		if (i < bfregi->num_static_sys_pages ||
		    bfregi->sys_pages[i] != MLX5_IB_INVALID_UAR_INDEX)
			mlx5_cmd_free_uar(dev->mdev, bfregi->sys_pages[i]);
}

<<<<<<< HEAD
static int mlx5_ib_alloc_transport_domain(struct mlx5_ib_dev *dev, u32 *tdn)
=======
int mlx5_ib_enable_lb(struct mlx5_ib_dev *dev, bool td, bool qp)
{
	int err = 0;

	mutex_lock(&dev->lb.mutex);
	if (td)
		dev->lb.user_td++;
	if (qp)
		dev->lb.qps++;

	if (dev->lb.user_td == 2 ||
	    dev->lb.qps == 1) {
		if (!dev->lb.enabled) {
			err = mlx5_nic_vport_update_local_lb(dev->mdev, true);
			dev->lb.enabled = true;
		}
	}

	mutex_unlock(&dev->lb.mutex);

	return err;
}

void mlx5_ib_disable_lb(struct mlx5_ib_dev *dev, bool td, bool qp)
{
	mutex_lock(&dev->lb.mutex);
	if (td)
		dev->lb.user_td--;
	if (qp)
		dev->lb.qps--;

	if (dev->lb.user_td == 1 &&
	    dev->lb.qps == 0) {
		if (dev->lb.enabled) {
			mlx5_nic_vport_update_local_lb(dev->mdev, false);
			dev->lb.enabled = false;
		}
	}

	mutex_unlock(&dev->lb.mutex);
}

static int mlx5_ib_alloc_transport_domain(struct mlx5_ib_dev *dev, u32 *tdn,
					  u16 uid)
>>>>>>> upstream/android-13
{
	int err;

	if (!MLX5_CAP_GEN(dev->mdev, log_max_transport_domain))
		return 0;

<<<<<<< HEAD
	err = mlx5_core_alloc_transport_domain(dev->mdev, tdn);
=======
	err = mlx5_cmd_alloc_transport_domain(dev->mdev, tdn, uid);
>>>>>>> upstream/android-13
	if (err)
		return err;

	if ((MLX5_CAP_GEN(dev->mdev, port_type) != MLX5_CAP_PORT_TYPE_ETH) ||
	    (!MLX5_CAP_GEN(dev->mdev, disable_local_lb_uc) &&
	     !MLX5_CAP_GEN(dev->mdev, disable_local_lb_mc)))
		return err;

<<<<<<< HEAD
	mutex_lock(&dev->lb_mutex);
	dev->user_td++;

	if (dev->user_td == 2)
		err = mlx5_nic_vport_update_local_lb(dev->mdev, true);

	mutex_unlock(&dev->lb_mutex);
	return err;
}

static void mlx5_ib_dealloc_transport_domain(struct mlx5_ib_dev *dev, u32 tdn)
=======
	return mlx5_ib_enable_lb(dev, true, false);
}

static void mlx5_ib_dealloc_transport_domain(struct mlx5_ib_dev *dev, u32 tdn,
					     u16 uid)
>>>>>>> upstream/android-13
{
	if (!MLX5_CAP_GEN(dev->mdev, log_max_transport_domain))
		return;

<<<<<<< HEAD
	mlx5_core_dealloc_transport_domain(dev->mdev, tdn);
=======
	mlx5_cmd_dealloc_transport_domain(dev->mdev, tdn, uid);
>>>>>>> upstream/android-13

	if ((MLX5_CAP_GEN(dev->mdev, port_type) != MLX5_CAP_PORT_TYPE_ETH) ||
	    (!MLX5_CAP_GEN(dev->mdev, disable_local_lb_uc) &&
	     !MLX5_CAP_GEN(dev->mdev, disable_local_lb_mc)))
		return;

<<<<<<< HEAD
	mutex_lock(&dev->lb_mutex);
	dev->user_td--;

	if (dev->user_td < 2)
		mlx5_nic_vport_update_local_lb(dev->mdev, false);

	mutex_unlock(&dev->lb_mutex);
}

static struct ib_ucontext *mlx5_ib_alloc_ucontext(struct ib_device *ibdev,
						  struct ib_udata *udata)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	struct mlx5_ib_alloc_ucontext_req_v2 req = {};
	struct mlx5_ib_alloc_ucontext_resp resp = {};
	struct mlx5_core_dev *mdev = dev->mdev;
	struct mlx5_ib_ucontext *context;
=======
	mlx5_ib_disable_lb(dev, true, false);
}

static int set_ucontext_resp(struct ib_ucontext *uctx,
			     struct mlx5_ib_alloc_ucontext_resp *resp)
{
	struct ib_device *ibdev = uctx->device;
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	struct mlx5_ib_ucontext *context = to_mucontext(uctx);
	struct mlx5_bfreg_info *bfregi = &context->bfregi;
	int err;

	if (MLX5_CAP_GEN(dev->mdev, dump_fill_mkey)) {
		err = mlx5_cmd_dump_fill_mkey(dev->mdev,
					      &resp->dump_fill_mkey);
		if (err)
			return err;
		resp->comp_mask |=
			MLX5_IB_ALLOC_UCONTEXT_RESP_MASK_DUMP_FILL_MKEY;
	}

	resp->qp_tab_size = 1 << MLX5_CAP_GEN(dev->mdev, log_max_qp);
	if (dev->wc_support)
		resp->bf_reg_size = 1 << MLX5_CAP_GEN(dev->mdev,
						      log_bf_reg_size);
	resp->cache_line_size = cache_line_size();
	resp->max_sq_desc_sz = MLX5_CAP_GEN(dev->mdev, max_wqe_sz_sq);
	resp->max_rq_desc_sz = MLX5_CAP_GEN(dev->mdev, max_wqe_sz_rq);
	resp->max_send_wqebb = 1 << MLX5_CAP_GEN(dev->mdev, log_max_qp_sz);
	resp->max_recv_wr = 1 << MLX5_CAP_GEN(dev->mdev, log_max_qp_sz);
	resp->max_srq_recv_wr = 1 << MLX5_CAP_GEN(dev->mdev, log_max_srq_sz);
	resp->cqe_version = context->cqe_version;
	resp->log_uar_size = MLX5_CAP_GEN(dev->mdev, uar_4k) ?
				MLX5_ADAPTER_PAGE_SHIFT : PAGE_SHIFT;
	resp->num_uars_per_page = MLX5_CAP_GEN(dev->mdev, uar_4k) ?
					MLX5_CAP_GEN(dev->mdev,
						     num_of_uars_per_page) : 1;

	if (mlx5_accel_ipsec_device_caps(dev->mdev) &
				MLX5_ACCEL_IPSEC_CAP_DEVICE) {
		if (mlx5_get_flow_namespace(dev->mdev,
				MLX5_FLOW_NAMESPACE_EGRESS))
			resp->flow_action_flags |= MLX5_USER_ALLOC_UCONTEXT_FLOW_ACTION_FLAGS_ESP_AES_GCM;
		if (mlx5_accel_ipsec_device_caps(dev->mdev) &
				MLX5_ACCEL_IPSEC_CAP_REQUIRED_METADATA)
			resp->flow_action_flags |= MLX5_USER_ALLOC_UCONTEXT_FLOW_ACTION_FLAGS_ESP_AES_GCM_REQ_METADATA;
		if (MLX5_CAP_FLOWTABLE(dev->mdev, flow_table_properties_nic_receive.ft_field_support.outer_esp_spi))
			resp->flow_action_flags |= MLX5_USER_ALLOC_UCONTEXT_FLOW_ACTION_FLAGS_ESP_AES_GCM_SPI_STEERING;
		if (mlx5_accel_ipsec_device_caps(dev->mdev) &
				MLX5_ACCEL_IPSEC_CAP_TX_IV_IS_ESN)
			resp->flow_action_flags |= MLX5_USER_ALLOC_UCONTEXT_FLOW_ACTION_FLAGS_ESP_AES_GCM_TX_IV_IS_ESN;
		/* MLX5_USER_ALLOC_UCONTEXT_FLOW_ACTION_FLAGS_ESP_AES_GCM_FULL_OFFLOAD is currently always 0 */
	}

	resp->tot_bfregs = bfregi->lib_uar_dyn ? 0 :
			bfregi->total_num_bfregs - bfregi->num_dyn_bfregs;
	resp->num_ports = dev->num_ports;
	resp->cmds_supp_uhw |= MLX5_USER_CMDS_SUPP_UHW_QUERY_DEVICE |
				      MLX5_USER_CMDS_SUPP_UHW_CREATE_AH;

	if (mlx5_ib_port_link_layer(ibdev, 1) == IB_LINK_LAYER_ETHERNET) {
		mlx5_query_min_inline(dev->mdev, &resp->eth_min_inline);
		resp->eth_min_inline++;
	}

	if (dev->mdev->clock_info)
		resp->clock_info_versions = BIT(MLX5_IB_CLOCK_INFO_V1);

	/*
	 * We don't want to expose information from the PCI bar that is located
	 * after 4096 bytes, so if the arch only supports larger pages, let's
	 * pretend we don't support reading the HCA's core clock. This is also
	 * forced by mmap function.
	 */
	if (PAGE_SIZE <= 4096) {
		resp->comp_mask |=
			MLX5_IB_ALLOC_UCONTEXT_RESP_MASK_CORE_CLOCK_OFFSET;
		resp->hca_core_clock_offset =
			offsetof(struct mlx5_init_seg,
				 internal_timer_h) % PAGE_SIZE;
	}

	if (MLX5_CAP_GEN(dev->mdev, ece_support))
		resp->comp_mask |= MLX5_IB_ALLOC_UCONTEXT_RESP_MASK_ECE;

	if (rt_supported(MLX5_CAP_GEN(dev->mdev, sq_ts_format)) &&
	    rt_supported(MLX5_CAP_GEN(dev->mdev, rq_ts_format)) &&
	    rt_supported(MLX5_CAP_ROCE(dev->mdev, qp_ts_format)))
		resp->comp_mask |=
			MLX5_IB_ALLOC_UCONTEXT_RESP_MASK_REAL_TIME_TS;

	resp->num_dyn_bfregs = bfregi->num_dyn_bfregs;

	if (MLX5_CAP_GEN(dev->mdev, drain_sigerr))
		resp->comp_mask |= MLX5_IB_ALLOC_UCONTEXT_RESP_MASK_SQD2RTS;

	return 0;
}

static int mlx5_ib_alloc_ucontext(struct ib_ucontext *uctx,
				  struct ib_udata *udata)
{
	struct ib_device *ibdev = uctx->device;
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	struct mlx5_ib_alloc_ucontext_req_v2 req = {};
	struct mlx5_ib_alloc_ucontext_resp resp = {};
	struct mlx5_ib_ucontext *context = to_mucontext(uctx);
>>>>>>> upstream/android-13
	struct mlx5_bfreg_info *bfregi;
	int ver;
	int err;
	size_t min_req_v2 = offsetof(struct mlx5_ib_alloc_ucontext_req_v2,
				     max_cqe_version);
<<<<<<< HEAD
	u32 dump_fill_mkey;
	bool lib_uar_4k;

	if (!dev->ib_active)
		return ERR_PTR(-EAGAIN);
=======
	bool lib_uar_4k;
	bool lib_uar_dyn;

	if (!dev->ib_active)
		return -EAGAIN;
>>>>>>> upstream/android-13

	if (udata->inlen == sizeof(struct mlx5_ib_alloc_ucontext_req))
		ver = 0;
	else if (udata->inlen >= min_req_v2)
		ver = 2;
	else
<<<<<<< HEAD
		return ERR_PTR(-EINVAL);

	err = ib_copy_from_udata(&req, udata, min(udata->inlen, sizeof(req)));
	if (err)
		return ERR_PTR(err);

	if (req.flags & ~MLX5_IB_ALLOC_UCTX_DEVX)
		return ERR_PTR(-EOPNOTSUPP);

	if (req.comp_mask || req.reserved0 || req.reserved1 || req.reserved2)
		return ERR_PTR(-EOPNOTSUPP);
=======
		return -EINVAL;

	err = ib_copy_from_udata(&req, udata, min(udata->inlen, sizeof(req)));
	if (err)
		return err;

	if (req.flags & ~MLX5_IB_ALLOC_UCTX_DEVX)
		return -EOPNOTSUPP;

	if (req.comp_mask || req.reserved0 || req.reserved1 || req.reserved2)
		return -EOPNOTSUPP;
>>>>>>> upstream/android-13

	req.total_num_bfregs = ALIGN(req.total_num_bfregs,
				    MLX5_NON_FP_BFREGS_PER_UAR);
	if (req.num_low_latency_bfregs > req.total_num_bfregs - 1)
<<<<<<< HEAD
		return ERR_PTR(-EINVAL);

	resp.qp_tab_size = 1 << MLX5_CAP_GEN(dev->mdev, log_max_qp);
	if (mlx5_core_is_pf(dev->mdev) && MLX5_CAP_GEN(dev->mdev, bf))
		resp.bf_reg_size = 1 << MLX5_CAP_GEN(dev->mdev, log_bf_reg_size);
	resp.cache_line_size = cache_line_size();
	resp.max_sq_desc_sz = MLX5_CAP_GEN(dev->mdev, max_wqe_sz_sq);
	resp.max_rq_desc_sz = MLX5_CAP_GEN(dev->mdev, max_wqe_sz_rq);
	resp.max_send_wqebb = 1 << MLX5_CAP_GEN(dev->mdev, log_max_qp_sz);
	resp.max_recv_wr = 1 << MLX5_CAP_GEN(dev->mdev, log_max_qp_sz);
	resp.max_srq_recv_wr = 1 << MLX5_CAP_GEN(dev->mdev, log_max_srq_sz);
	resp.cqe_version = min_t(__u8,
				 (__u8)MLX5_CAP_GEN(dev->mdev, cqe_version),
				 req.max_cqe_version);
	resp.log_uar_size = MLX5_CAP_GEN(dev->mdev, uar_4k) ?
				MLX5_ADAPTER_PAGE_SHIFT : PAGE_SHIFT;
	resp.num_uars_per_page = MLX5_CAP_GEN(dev->mdev, uar_4k) ?
					MLX5_CAP_GEN(dev->mdev, num_of_uars_per_page) : 1;
	resp.response_length = min(offsetof(typeof(resp), response_length) +
				   sizeof(resp.response_length), udata->outlen);

	if (mlx5_accel_ipsec_device_caps(dev->mdev) & MLX5_ACCEL_IPSEC_CAP_DEVICE) {
		if (mlx5_get_flow_namespace(dev->mdev, MLX5_FLOW_NAMESPACE_EGRESS))
			resp.flow_action_flags |= MLX5_USER_ALLOC_UCONTEXT_FLOW_ACTION_FLAGS_ESP_AES_GCM;
		if (mlx5_accel_ipsec_device_caps(dev->mdev) & MLX5_ACCEL_IPSEC_CAP_REQUIRED_METADATA)
			resp.flow_action_flags |= MLX5_USER_ALLOC_UCONTEXT_FLOW_ACTION_FLAGS_ESP_AES_GCM_REQ_METADATA;
		if (MLX5_CAP_FLOWTABLE(dev->mdev, flow_table_properties_nic_receive.ft_field_support.outer_esp_spi))
			resp.flow_action_flags |= MLX5_USER_ALLOC_UCONTEXT_FLOW_ACTION_FLAGS_ESP_AES_GCM_SPI_STEERING;
		if (mlx5_accel_ipsec_device_caps(dev->mdev) & MLX5_ACCEL_IPSEC_CAP_TX_IV_IS_ESN)
			resp.flow_action_flags |= MLX5_USER_ALLOC_UCONTEXT_FLOW_ACTION_FLAGS_ESP_AES_GCM_TX_IV_IS_ESN;
		/* MLX5_USER_ALLOC_UCONTEXT_FLOW_ACTION_FLAGS_ESP_AES_GCM_FULL_OFFLOAD is currently always 0 */
	}

	context = kzalloc(sizeof(*context), GFP_KERNEL);
	if (!context)
		return ERR_PTR(-ENOMEM);

	lib_uar_4k = req.lib_caps & MLX5_LIB_CAP_4K_UAR;
	bfregi = &context->bfregi;

=======
		return -EINVAL;

	lib_uar_4k = req.lib_caps & MLX5_LIB_CAP_4K_UAR;
	lib_uar_dyn = req.lib_caps & MLX5_LIB_CAP_DYN_UAR;
	bfregi = &context->bfregi;

	if (lib_uar_dyn) {
		bfregi->lib_uar_dyn = lib_uar_dyn;
		goto uar_done;
	}

>>>>>>> upstream/android-13
	/* updates req->total_num_bfregs */
	err = calc_total_bfregs(dev, lib_uar_4k, &req, bfregi);
	if (err)
		goto out_ctx;

	mutex_init(&bfregi->lock);
	bfregi->lib_uar_4k = lib_uar_4k;
	bfregi->count = kcalloc(bfregi->total_num_bfregs, sizeof(*bfregi->count),
				GFP_KERNEL);
	if (!bfregi->count) {
		err = -ENOMEM;
		goto out_ctx;
	}

	bfregi->sys_pages = kcalloc(bfregi->num_sys_pages,
				    sizeof(*bfregi->sys_pages),
				    GFP_KERNEL);
	if (!bfregi->sys_pages) {
		err = -ENOMEM;
		goto out_count;
	}

	err = allocate_uars(dev, context);
	if (err)
		goto out_sys_pages;

<<<<<<< HEAD
#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
	context->ibucontext.invalidate_range = &mlx5_ib_invalidate_range;
#endif

	err = mlx5_ib_alloc_transport_domain(dev, &context->tdn);
	if (err)
		goto out_uars;

	if (req.flags & MLX5_IB_ALLOC_UCTX_DEVX) {
		/* Block DEVX on Infiniband as of SELinux */
		if (mlx5_ib_port_link_layer(ibdev, 1) != IB_LINK_LAYER_ETHERNET) {
			err = -EPERM;
			goto out_td;
		}

		err = mlx5_ib_devx_create(dev, context);
		if (err)
			goto out_td;
	}

	if (MLX5_CAP_GEN(dev->mdev, dump_fill_mkey)) {
		err = mlx5_cmd_dump_fill_mkey(dev->mdev, &dump_fill_mkey);
		if (err)
			goto out_mdev;
	}

	INIT_LIST_HEAD(&context->vma_private_list);
	mutex_init(&context->vma_private_list_mutex);
	INIT_LIST_HEAD(&context->db_page_list);
	mutex_init(&context->db_page_mutex);

	resp.tot_bfregs = req.total_num_bfregs;
	resp.num_ports = dev->num_ports;

	if (field_avail(typeof(resp), cqe_version, udata->outlen))
		resp.response_length += sizeof(resp.cqe_version);

	if (field_avail(typeof(resp), cmds_supp_uhw, udata->outlen)) {
		resp.cmds_supp_uhw |= MLX5_USER_CMDS_SUPP_UHW_QUERY_DEVICE |
				      MLX5_USER_CMDS_SUPP_UHW_CREATE_AH;
		resp.response_length += sizeof(resp.cmds_supp_uhw);
	}

	if (field_avail(typeof(resp), eth_min_inline, udata->outlen)) {
		if (mlx5_ib_port_link_layer(ibdev, 1) == IB_LINK_LAYER_ETHERNET) {
			mlx5_query_min_inline(dev->mdev, &resp.eth_min_inline);
			resp.eth_min_inline++;
		}
		resp.response_length += sizeof(resp.eth_min_inline);
	}

	if (field_avail(typeof(resp), clock_info_versions, udata->outlen)) {
		if (mdev->clock_info)
			resp.clock_info_versions = BIT(MLX5_IB_CLOCK_INFO_V1);
		resp.response_length += sizeof(resp.clock_info_versions);
	}

	/*
	 * We don't want to expose information from the PCI bar that is located
	 * after 4096 bytes, so if the arch only supports larger pages, let's
	 * pretend we don't support reading the HCA's core clock. This is also
	 * forced by mmap function.
	 */
	if (field_avail(typeof(resp), hca_core_clock_offset, udata->outlen)) {
		if (PAGE_SIZE <= 4096) {
			resp.comp_mask |=
				MLX5_IB_ALLOC_UCONTEXT_RESP_MASK_CORE_CLOCK_OFFSET;
			resp.hca_core_clock_offset =
				offsetof(struct mlx5_init_seg, internal_timer_h) % PAGE_SIZE;
		}
		resp.response_length += sizeof(resp.hca_core_clock_offset);
	}

	if (field_avail(typeof(resp), log_uar_size, udata->outlen))
		resp.response_length += sizeof(resp.log_uar_size);

	if (field_avail(typeof(resp), num_uars_per_page, udata->outlen))
		resp.response_length += sizeof(resp.num_uars_per_page);

	if (field_avail(typeof(resp), num_dyn_bfregs, udata->outlen)) {
		resp.num_dyn_bfregs = bfregi->num_dyn_bfregs;
		resp.response_length += sizeof(resp.num_dyn_bfregs);
	}

	if (field_avail(typeof(resp), dump_fill_mkey, udata->outlen)) {
		if (MLX5_CAP_GEN(dev->mdev, dump_fill_mkey)) {
			resp.dump_fill_mkey = dump_fill_mkey;
			resp.comp_mask |=
				MLX5_IB_ALLOC_UCONTEXT_RESP_MASK_DUMP_FILL_MKEY;
		}
		resp.response_length += sizeof(resp.dump_fill_mkey);
	}

=======
uar_done:
	if (req.flags & MLX5_IB_ALLOC_UCTX_DEVX) {
		err = mlx5_ib_devx_create(dev, true);
		if (err < 0)
			goto out_uars;
		context->devx_uid = err;
	}

	err = mlx5_ib_alloc_transport_domain(dev, &context->tdn,
					     context->devx_uid);
	if (err)
		goto out_devx;

	INIT_LIST_HEAD(&context->db_page_list);
	mutex_init(&context->db_page_mutex);

	context->cqe_version = min_t(__u8,
				 (__u8)MLX5_CAP_GEN(dev->mdev, cqe_version),
				 req.max_cqe_version);

	err = set_ucontext_resp(uctx, &resp);
	if (err)
		goto out_mdev;

	resp.response_length = min(udata->outlen, sizeof(resp));
>>>>>>> upstream/android-13
	err = ib_copy_to_udata(udata, &resp, resp.response_length);
	if (err)
		goto out_mdev;

	bfregi->ver = ver;
	bfregi->num_low_latency_bfregs = req.num_low_latency_bfregs;
<<<<<<< HEAD
	context->cqe_version = resp.cqe_version;
	context->lib_caps = req.lib_caps;
	print_lib_caps(dev, context->lib_caps);

	if (mlx5_lag_is_active(dev->mdev)) {
		u8 port = mlx5_core_native_port_num(dev->mdev);

		atomic_set(&context->tx_port_affinity,
			   atomic_add_return(
				   1, &dev->roce[port].tx_port_affinity));
	}

	return &context->ibucontext;

out_mdev:
	if (req.flags & MLX5_IB_ALLOC_UCTX_DEVX)
		mlx5_ib_devx_destroy(dev, context);
out_td:
	mlx5_ib_dealloc_transport_domain(dev, context->tdn);
=======
	context->lib_caps = req.lib_caps;
	print_lib_caps(dev, context->lib_caps);

	if (mlx5_ib_lag_should_assign_affinity(dev)) {
		u32 port = mlx5_core_native_port_num(dev->mdev) - 1;

		atomic_set(&context->tx_port_affinity,
			   atomic_add_return(
				   1, &dev->port[port].roce.tx_port_affinity));
	}

	return 0;

out_mdev:
	mlx5_ib_dealloc_transport_domain(dev, context->tdn, context->devx_uid);
out_devx:
	if (req.flags & MLX5_IB_ALLOC_UCTX_DEVX)
		mlx5_ib_devx_destroy(dev, context->devx_uid);
>>>>>>> upstream/android-13

out_uars:
	deallocate_uars(dev, context);

out_sys_pages:
	kfree(bfregi->sys_pages);

out_count:
	kfree(bfregi->count);

out_ctx:
<<<<<<< HEAD
	kfree(context);

	return ERR_PTR(err);
}

static int mlx5_ib_dealloc_ucontext(struct ib_ucontext *ibcontext)
=======
	return err;
}

static int mlx5_ib_query_ucontext(struct ib_ucontext *ibcontext,
				  struct uverbs_attr_bundle *attrs)
{
	struct mlx5_ib_alloc_ucontext_resp uctx_resp = {};
	int ret;

	ret = set_ucontext_resp(ibcontext, &uctx_resp);
	if (ret)
		return ret;

	uctx_resp.response_length =
		min_t(size_t,
		      uverbs_attr_get_len(attrs,
				MLX5_IB_ATTR_QUERY_CONTEXT_RESP_UCTX),
		      sizeof(uctx_resp));

	ret = uverbs_copy_to_struct_or_zero(attrs,
					MLX5_IB_ATTR_QUERY_CONTEXT_RESP_UCTX,
					&uctx_resp,
					sizeof(uctx_resp));
	return ret;
}

static void mlx5_ib_dealloc_ucontext(struct ib_ucontext *ibcontext)
>>>>>>> upstream/android-13
{
	struct mlx5_ib_ucontext *context = to_mucontext(ibcontext);
	struct mlx5_ib_dev *dev = to_mdev(ibcontext->device);
	struct mlx5_bfreg_info *bfregi;

<<<<<<< HEAD
	if (context->devx_uid)
		mlx5_ib_devx_destroy(dev, context);

	bfregi = &context->bfregi;
	mlx5_ib_dealloc_transport_domain(dev, context->tdn);
=======
	bfregi = &context->bfregi;
	mlx5_ib_dealloc_transport_domain(dev, context->tdn, context->devx_uid);

	if (context->devx_uid)
		mlx5_ib_devx_destroy(dev, context->devx_uid);
>>>>>>> upstream/android-13

	deallocate_uars(dev, context);
	kfree(bfregi->sys_pages);
	kfree(bfregi->count);
<<<<<<< HEAD
	kfree(context);

	return 0;
=======
>>>>>>> upstream/android-13
}

static phys_addr_t uar_index2pfn(struct mlx5_ib_dev *dev,
				 int uar_idx)
{
	int fw_uars_per_page;

	fw_uars_per_page = MLX5_CAP_GEN(dev->mdev, uar_4k) ? MLX5_UARS_IN_PAGE : 1;

<<<<<<< HEAD
	return (pci_resource_start(dev->mdev->pdev, 0) >> PAGE_SHIFT) + uar_idx / fw_uars_per_page;
=======
	return (dev->mdev->bar_addr >> PAGE_SHIFT) + uar_idx / fw_uars_per_page;
}

static u64 uar_index2paddress(struct mlx5_ib_dev *dev,
				 int uar_idx)
{
	unsigned int fw_uars_per_page;

	fw_uars_per_page = MLX5_CAP_GEN(dev->mdev, uar_4k) ?
				MLX5_UARS_IN_PAGE : 1;

	return (dev->mdev->bar_addr + (uar_idx / fw_uars_per_page) * PAGE_SIZE);
>>>>>>> upstream/android-13
}

static int get_command(unsigned long offset)
{
	return (offset >> MLX5_IB_MMAP_CMD_SHIFT) & MLX5_IB_MMAP_CMD_MASK;
}

static int get_arg(unsigned long offset)
{
	return offset & ((1 << MLX5_IB_MMAP_CMD_SHIFT) - 1);
}

static int get_index(unsigned long offset)
{
	return get_arg(offset);
}

/* Index resides in an extra byte to enable larger values than 255 */
static int get_extended_index(unsigned long offset)
{
	return get_arg(offset) | ((offset >> 16) & 0xff) << 8;
}

<<<<<<< HEAD
static void  mlx5_ib_vma_open(struct vm_area_struct *area)
{
	/* vma_open is called when a new VMA is created on top of our VMA.  This
	 * is done through either mremap flow or split_vma (usually due to
	 * mlock, madvise, munmap, etc.) We do not support a clone of the VMA,
	 * as this VMA is strongly hardware related.  Therefore we set the
	 * vm_ops of the newly created/cloned VMA to NULL, to prevent it from
	 * calling us again and trying to do incorrect actions.  We assume that
	 * the original VMA size is exactly a single page, and therefore all
	 * "splitting" operation will not happen to it.
	 */
	area->vm_ops = NULL;
}

static void  mlx5_ib_vma_close(struct vm_area_struct *area)
{
	struct mlx5_ib_vma_private_data *mlx5_ib_vma_priv_data;

	/* It's guaranteed that all VMAs opened on a FD are closed before the
	 * file itself is closed, therefore no sync is needed with the regular
	 * closing flow. (e.g. mlx5 ib_dealloc_ucontext)
	 * However need a sync with accessing the vma as part of
	 * mlx5_ib_disassociate_ucontext.
	 * The close operation is usually called under mm->mmap_sem except when
	 * process is exiting.
	 * The exiting case is handled explicitly as part of
	 * mlx5_ib_disassociate_ucontext.
	 */
	mlx5_ib_vma_priv_data = (struct mlx5_ib_vma_private_data *)area->vm_private_data;

	/* setting the vma context pointer to null in the mlx5_ib driver's
	 * private data, to protect a race condition in
	 * mlx5_ib_disassociate_ucontext().
	 */
	mlx5_ib_vma_priv_data->vma = NULL;
	mutex_lock(mlx5_ib_vma_priv_data->vma_private_list_mutex);
	list_del(&mlx5_ib_vma_priv_data->list);
	mutex_unlock(mlx5_ib_vma_priv_data->vma_private_list_mutex);
	kfree(mlx5_ib_vma_priv_data);
}

static const struct vm_operations_struct mlx5_ib_vm_ops = {
	.open = mlx5_ib_vma_open,
	.close = mlx5_ib_vma_close
};

static int mlx5_ib_set_vma_data(struct vm_area_struct *vma,
				struct mlx5_ib_ucontext *ctx)
{
	struct mlx5_ib_vma_private_data *vma_prv;
	struct list_head *vma_head = &ctx->vma_private_list;

	vma_prv = kzalloc(sizeof(*vma_prv), GFP_KERNEL);
	if (!vma_prv)
		return -ENOMEM;

	vma_prv->vma = vma;
	vma_prv->vma_private_list_mutex = &ctx->vma_private_list_mutex;
	vma->vm_private_data = vma_prv;
	vma->vm_ops =  &mlx5_ib_vm_ops;

	mutex_lock(&ctx->vma_private_list_mutex);
	list_add(&vma_prv->list, vma_head);
	mutex_unlock(&ctx->vma_private_list_mutex);

	return 0;
}

static void mlx5_ib_disassociate_ucontext(struct ib_ucontext *ibcontext)
{
	struct vm_area_struct *vma;
	struct mlx5_ib_vma_private_data *vma_private, *n;
	struct mlx5_ib_ucontext *context = to_mucontext(ibcontext);

	mutex_lock(&context->vma_private_list_mutex);
	list_for_each_entry_safe(vma_private, n, &context->vma_private_list,
				 list) {
		vma = vma_private->vma;
		zap_vma_ptes(vma, vma->vm_start, PAGE_SIZE);
		/* context going to be destroyed, should
		 * not access ops any more.
		 */
		vma->vm_flags &= ~(VM_SHARED | VM_MAYSHARE);
		vma->vm_ops = NULL;
		list_del(&vma_private->list);
		kfree(vma_private);
	}
	mutex_unlock(&context->vma_private_list_mutex);
=======

static void mlx5_ib_disassociate_ucontext(struct ib_ucontext *ibcontext)
{
>>>>>>> upstream/android-13
}

static inline char *mmap_cmd2str(enum mlx5_ib_mmap_cmd cmd)
{
	switch (cmd) {
	case MLX5_IB_MMAP_WC_PAGE:
		return "WC";
	case MLX5_IB_MMAP_REGULAR_PAGE:
		return "best effort WC";
	case MLX5_IB_MMAP_NC_PAGE:
		return "NC";
	case MLX5_IB_MMAP_DEVICE_MEM:
		return "Device Memory";
	default:
		return NULL;
	}
}

static int mlx5_ib_mmap_clock_info_page(struct mlx5_ib_dev *dev,
					struct vm_area_struct *vma,
					struct mlx5_ib_ucontext *context)
{
<<<<<<< HEAD
	phys_addr_t pfn;
	int err;

	if (vma->vm_end - vma->vm_start != PAGE_SIZE)
=======
	if ((vma->vm_end - vma->vm_start != PAGE_SIZE) ||
	    !(vma->vm_flags & VM_SHARED))
>>>>>>> upstream/android-13
		return -EINVAL;

	if (get_index(vma->vm_pgoff) != MLX5_IB_CLOCK_INFO_V1)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (vma->vm_flags & VM_WRITE)
		return -EPERM;
	vma->vm_flags &= ~VM_MAYWRITE;

	if (!dev->mdev->clock_info_page)
		return -EOPNOTSUPP;

	pfn = page_to_pfn(dev->mdev->clock_info_page);
	err = remap_pfn_range(vma, vma->vm_start, pfn, PAGE_SIZE,
			      vma->vm_page_prot);
	if (err)
		return err;

	return mlx5_ib_set_vma_data(vma, context);
=======
	if (vma->vm_flags & (VM_WRITE | VM_EXEC))
		return -EPERM;
	vma->vm_flags &= ~VM_MAYWRITE;

	if (!dev->mdev->clock_info)
		return -EOPNOTSUPP;

	return vm_insert_page(vma, vma->vm_start,
			      virt_to_page(dev->mdev->clock_info));
}

static void mlx5_ib_mmap_free(struct rdma_user_mmap_entry *entry)
{
	struct mlx5_user_mmap_entry *mentry = to_mmmap(entry);
	struct mlx5_ib_dev *dev = to_mdev(entry->ucontext->device);
	struct mlx5_var_table *var_table = &dev->var_table;

	switch (mentry->mmap_flag) {
	case MLX5_IB_MMAP_TYPE_MEMIC:
	case MLX5_IB_MMAP_TYPE_MEMIC_OP:
		mlx5_ib_dm_mmap_free(dev, mentry);
		break;
	case MLX5_IB_MMAP_TYPE_VAR:
		mutex_lock(&var_table->bitmap_lock);
		clear_bit(mentry->page_idx, var_table->bitmap);
		mutex_unlock(&var_table->bitmap_lock);
		kfree(mentry);
		break;
	case MLX5_IB_MMAP_TYPE_UAR_WC:
	case MLX5_IB_MMAP_TYPE_UAR_NC:
		mlx5_cmd_free_uar(dev->mdev, mentry->page_idx);
		kfree(mentry);
		break;
	default:
		WARN_ON(true);
	}
>>>>>>> upstream/android-13
}

static int uar_mmap(struct mlx5_ib_dev *dev, enum mlx5_ib_mmap_cmd cmd,
		    struct vm_area_struct *vma,
		    struct mlx5_ib_ucontext *context)
{
	struct mlx5_bfreg_info *bfregi = &context->bfregi;
	int err;
	unsigned long idx;
	phys_addr_t pfn;
	pgprot_t prot;
	u32 bfreg_dyn_idx = 0;
	u32 uar_index;
	int dyn_uar = (cmd == MLX5_IB_MMAP_ALLOC_WC);
	int max_valid_idx = dyn_uar ? bfregi->num_sys_pages :
				bfregi->num_static_sys_pages;

<<<<<<< HEAD
=======
	if (bfregi->lib_uar_dyn)
		return -EINVAL;

>>>>>>> upstream/android-13
	if (vma->vm_end - vma->vm_start != PAGE_SIZE)
		return -EINVAL;

	if (dyn_uar)
		idx = get_extended_index(vma->vm_pgoff) + bfregi->num_static_sys_pages;
	else
		idx = get_index(vma->vm_pgoff);

	if (idx >= max_valid_idx) {
		mlx5_ib_warn(dev, "invalid uar index %lu, max=%d\n",
			     idx, max_valid_idx);
		return -EINVAL;
	}

	switch (cmd) {
	case MLX5_IB_MMAP_WC_PAGE:
	case MLX5_IB_MMAP_ALLOC_WC:
<<<<<<< HEAD
/* Some architectures don't support WC memory */
#if defined(CONFIG_X86)
		if (!pat_enabled())
			return -EPERM;
#elif !(defined(CONFIG_PPC) || (defined(CONFIG_ARM) && defined(CONFIG_MMU)))
			return -EPERM;
#endif
	/* fall through */
=======
>>>>>>> upstream/android-13
	case MLX5_IB_MMAP_REGULAR_PAGE:
		/* For MLX5_IB_MMAP_REGULAR_PAGE do the best effort to get WC */
		prot = pgprot_writecombine(vma->vm_page_prot);
		break;
	case MLX5_IB_MMAP_NC_PAGE:
		prot = pgprot_noncached(vma->vm_page_prot);
		break;
	default:
		return -EINVAL;
	}

	if (dyn_uar) {
		int uars_per_page;

		uars_per_page = get_uars_per_sys_page(dev, bfregi->lib_uar_4k);
		bfreg_dyn_idx = idx * (uars_per_page * MLX5_NON_FP_BFREGS_PER_UAR);
		if (bfreg_dyn_idx >= bfregi->total_num_bfregs) {
			mlx5_ib_warn(dev, "invalid bfreg_dyn_idx %u, max=%u\n",
				     bfreg_dyn_idx, bfregi->total_num_bfregs);
			return -EINVAL;
		}

		mutex_lock(&bfregi->lock);
		/* Fail if uar already allocated, first bfreg index of each
		 * page holds its count.
		 */
		if (bfregi->count[bfreg_dyn_idx]) {
			mlx5_ib_warn(dev, "wrong offset, idx %lu is busy, bfregn=%u\n", idx, bfreg_dyn_idx);
			mutex_unlock(&bfregi->lock);
			return -EINVAL;
		}

		bfregi->count[bfreg_dyn_idx]++;
		mutex_unlock(&bfregi->lock);

		err = mlx5_cmd_alloc_uar(dev->mdev, &uar_index);
		if (err) {
			mlx5_ib_warn(dev, "UAR alloc failed\n");
			goto free_bfreg;
		}
	} else {
		uar_index = bfregi->sys_pages[idx];
	}

	pfn = uar_index2pfn(dev, uar_index);
	mlx5_ib_dbg(dev, "uar idx 0x%lx, pfn %pa\n", idx, &pfn);

<<<<<<< HEAD
	vma->vm_page_prot = prot;
	err = io_remap_pfn_range(vma, vma->vm_start, pfn,
				 PAGE_SIZE, vma->vm_page_prot);
	if (err) {
		mlx5_ib_err(dev,
			    "io_remap_pfn_range failed with error=%d, mmap_cmd=%s\n",
			    err, mmap_cmd2str(cmd));
		err = -EAGAIN;
		goto err;
	}

	err = mlx5_ib_set_vma_data(vma, context);
	if (err)
		goto err;

=======
	err = rdma_user_mmap_io(&context->ibucontext, vma, pfn, PAGE_SIZE,
				prot, NULL);
	if (err) {
		mlx5_ib_err(dev,
			    "rdma_user_mmap_io failed with error=%d, mmap_cmd=%s\n",
			    err, mmap_cmd2str(cmd));
		goto err;
	}

>>>>>>> upstream/android-13
	if (dyn_uar)
		bfregi->sys_pages[idx] = uar_index;
	return 0;

err:
	if (!dyn_uar)
		return err;

	mlx5_cmd_free_uar(dev->mdev, idx);

free_bfreg:
	mlx5_ib_free_bfreg(dev, bfregi, bfreg_dyn_idx);

	return err;
}

<<<<<<< HEAD
static int dm_mmap(struct ib_ucontext *context, struct vm_area_struct *vma)
{
	struct mlx5_ib_ucontext *mctx = to_mucontext(context);
	struct mlx5_ib_dev *dev = to_mdev(context->device);
	u16 page_idx = get_extended_index(vma->vm_pgoff);
	size_t map_size = vma->vm_end - vma->vm_start;
	u32 npages = map_size >> PAGE_SHIFT;
	phys_addr_t pfn;
	pgprot_t prot;

	if (find_next_zero_bit(mctx->dm_pages, page_idx + npages, page_idx) !=
	    page_idx + npages)
		return -EINVAL;

	pfn = ((pci_resource_start(dev->mdev->pdev, 0) +
	      MLX5_CAP64_DEV_MEM(dev->mdev, memic_bar_start_addr)) >>
	      PAGE_SHIFT) +
	      page_idx;
	prot = pgprot_writecombine(vma->vm_page_prot);
	vma->vm_page_prot = prot;

	if (io_remap_pfn_range(vma, vma->vm_start, pfn, map_size,
			       vma->vm_page_prot))
		return -EAGAIN;

	return mlx5_ib_set_vma_data(vma, mctx);
=======
static unsigned long mlx5_vma_to_pgoff(struct vm_area_struct *vma)
{
	unsigned long idx;
	u8 command;

	command = get_command(vma->vm_pgoff);
	idx = get_extended_index(vma->vm_pgoff);

	return (command << 16 | idx);
}

static int mlx5_ib_mmap_offset(struct mlx5_ib_dev *dev,
			       struct vm_area_struct *vma,
			       struct ib_ucontext *ucontext)
{
	struct mlx5_user_mmap_entry *mentry;
	struct rdma_user_mmap_entry *entry;
	unsigned long pgoff;
	pgprot_t prot;
	phys_addr_t pfn;
	int ret;

	pgoff = mlx5_vma_to_pgoff(vma);
	entry = rdma_user_mmap_entry_get_pgoff(ucontext, pgoff);
	if (!entry)
		return -EINVAL;

	mentry = to_mmmap(entry);
	pfn = (mentry->address >> PAGE_SHIFT);
	if (mentry->mmap_flag == MLX5_IB_MMAP_TYPE_VAR ||
	    mentry->mmap_flag == MLX5_IB_MMAP_TYPE_UAR_NC)
		prot = pgprot_noncached(vma->vm_page_prot);
	else
		prot = pgprot_writecombine(vma->vm_page_prot);
	ret = rdma_user_mmap_io(ucontext, vma, pfn,
				entry->npages * PAGE_SIZE,
				prot,
				entry);
	rdma_user_mmap_entry_put(&mentry->rdma_entry);
	return ret;
}

static u64 mlx5_entry_to_mmap_offset(struct mlx5_user_mmap_entry *entry)
{
	u64 cmd = (entry->rdma_entry.start_pgoff >> 16) & 0xFFFF;
	u64 index = entry->rdma_entry.start_pgoff & 0xFFFF;

	return (((index >> 8) << 16) | (cmd << MLX5_IB_MMAP_CMD_SHIFT) |
		(index & 0xFF)) << PAGE_SHIFT;
>>>>>>> upstream/android-13
}

static int mlx5_ib_mmap(struct ib_ucontext *ibcontext, struct vm_area_struct *vma)
{
	struct mlx5_ib_ucontext *context = to_mucontext(ibcontext);
	struct mlx5_ib_dev *dev = to_mdev(ibcontext->device);
	unsigned long command;
	phys_addr_t pfn;

	command = get_command(vma->vm_pgoff);
	switch (command) {
	case MLX5_IB_MMAP_WC_PAGE:
<<<<<<< HEAD
	case MLX5_IB_MMAP_NC_PAGE:
	case MLX5_IB_MMAP_REGULAR_PAGE:
	case MLX5_IB_MMAP_ALLOC_WC:
=======
	case MLX5_IB_MMAP_ALLOC_WC:
		if (!dev->wc_support)
			return -EPERM;
		fallthrough;
	case MLX5_IB_MMAP_NC_PAGE:
	case MLX5_IB_MMAP_REGULAR_PAGE:
>>>>>>> upstream/android-13
		return uar_mmap(dev, command, vma, context);

	case MLX5_IB_MMAP_GET_CONTIGUOUS_PAGES:
		return -ENOSYS;

	case MLX5_IB_MMAP_CORE_CLOCK:
		if (vma->vm_end - vma->vm_start != PAGE_SIZE)
			return -EINVAL;

		if (vma->vm_flags & VM_WRITE)
			return -EPERM;
		vma->vm_flags &= ~VM_MAYWRITE;

		/* Don't expose to user-space information it shouldn't have */
		if (PAGE_SIZE > 4096)
			return -EOPNOTSUPP;

<<<<<<< HEAD
		vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
		pfn = (dev->mdev->iseg_base +
		       offsetof(struct mlx5_init_seg, internal_timer_h)) >>
			PAGE_SHIFT;
		if (io_remap_pfn_range(vma, vma->vm_start, pfn,
				       PAGE_SIZE, vma->vm_page_prot))
			return -EAGAIN;
		break;
	case MLX5_IB_MMAP_CLOCK_INFO:
		return mlx5_ib_mmap_clock_info_page(dev, vma, context);

	case MLX5_IB_MMAP_DEVICE_MEM:
		return dm_mmap(ibcontext, vma);

	default:
		return -EINVAL;
=======
		pfn = (dev->mdev->iseg_base +
		       offsetof(struct mlx5_init_seg, internal_timer_h)) >>
			PAGE_SHIFT;
		return rdma_user_mmap_io(&context->ibucontext, vma, pfn,
					 PAGE_SIZE,
					 pgprot_noncached(vma->vm_page_prot),
					 NULL);
	case MLX5_IB_MMAP_CLOCK_INFO:
		return mlx5_ib_mmap_clock_info_page(dev, vma, context);

	default:
		return mlx5_ib_mmap_offset(dev, vma, ibcontext);
>>>>>>> upstream/android-13
	}

	return 0;
}

<<<<<<< HEAD
struct ib_dm *mlx5_ib_alloc_dm(struct ib_device *ibdev,
			       struct ib_ucontext *context,
			       struct ib_dm_alloc_attr *attr,
			       struct uverbs_attr_bundle *attrs)
{
	u64 act_size = roundup(attr->length, MLX5_MEMIC_BASE_SIZE);
	struct mlx5_memic *memic = &to_mdev(ibdev)->memic;
	phys_addr_t memic_addr;
	struct mlx5_ib_dm *dm;
	u64 start_offset;
	u32 page_idx;
	int err;

	dm = kzalloc(sizeof(*dm), GFP_KERNEL);
	if (!dm)
		return ERR_PTR(-ENOMEM);

	mlx5_ib_dbg(to_mdev(ibdev), "alloc_memic req: user_length=0x%llx act_length=0x%llx log_alignment=%d\n",
		    attr->length, act_size, attr->alignment);

	err = mlx5_cmd_alloc_memic(memic, &memic_addr,
				   act_size, attr->alignment);
	if (err)
		goto err_free;

	start_offset = memic_addr & ~PAGE_MASK;
	page_idx = (memic_addr - pci_resource_start(memic->dev->pdev, 0) -
		    MLX5_CAP64_DEV_MEM(memic->dev, memic_bar_start_addr)) >>
		    PAGE_SHIFT;

	err = uverbs_copy_to(attrs,
			     MLX5_IB_ATTR_ALLOC_DM_RESP_START_OFFSET,
			     &start_offset, sizeof(start_offset));
	if (err)
		goto err_dealloc;

	err = uverbs_copy_to(attrs,
			     MLX5_IB_ATTR_ALLOC_DM_RESP_PAGE_INDEX,
			     &page_idx, sizeof(page_idx));
	if (err)
		goto err_dealloc;

	bitmap_set(to_mucontext(context)->dm_pages, page_idx,
		   DIV_ROUND_UP(act_size, PAGE_SIZE));

	dm->dev_addr = memic_addr;

	return &dm->ibdm;

err_dealloc:
	mlx5_cmd_dealloc_memic(memic, memic_addr,
			       act_size);
err_free:
	kfree(dm);
	return ERR_PTR(err);
}

int mlx5_ib_dealloc_dm(struct ib_dm *ibdm)
{
	struct mlx5_memic *memic = &to_mdev(ibdm->device)->memic;
	struct mlx5_ib_dm *dm = to_mdm(ibdm);
	u64 act_size = roundup(dm->ibdm.length, MLX5_MEMIC_BASE_SIZE);
	u32 page_idx;
	int ret;

	ret = mlx5_cmd_dealloc_memic(memic, dm->dev_addr, act_size);
	if (ret)
		return ret;

	page_idx = (dm->dev_addr - pci_resource_start(memic->dev->pdev, 0) -
		    MLX5_CAP64_DEV_MEM(memic->dev, memic_bar_start_addr)) >>
		    PAGE_SHIFT;
	bitmap_clear(to_mucontext(ibdm->uobject->context)->dm_pages,
		     page_idx,
		     DIV_ROUND_UP(act_size, PAGE_SIZE));

	kfree(dm);

	return 0;
}

static struct ib_pd *mlx5_ib_alloc_pd(struct ib_device *ibdev,
				      struct ib_ucontext *context,
				      struct ib_udata *udata)
{
	struct mlx5_ib_alloc_pd_resp resp;
	struct mlx5_ib_pd *pd;
	int err;

	pd = kmalloc(sizeof(*pd), GFP_KERNEL);
	if (!pd)
		return ERR_PTR(-ENOMEM);

	err = mlx5_core_alloc_pd(to_mdev(ibdev)->mdev, &pd->pdn);
	if (err) {
		kfree(pd);
		return ERR_PTR(err);
	}

	if (context) {
		resp.pdn = pd->pdn;
		if (ib_copy_to_udata(udata, &resp, sizeof(resp))) {
			mlx5_core_dealloc_pd(to_mdev(ibdev)->mdev, pd->pdn);
			kfree(pd);
			return ERR_PTR(-EFAULT);
		}
	}

	return &pd->ibpd;
}

static int mlx5_ib_dealloc_pd(struct ib_pd *pd)
=======
static int mlx5_ib_alloc_pd(struct ib_pd *ibpd, struct ib_udata *udata)
{
	struct mlx5_ib_pd *pd = to_mpd(ibpd);
	struct ib_device *ibdev = ibpd->device;
	struct mlx5_ib_alloc_pd_resp resp;
	int err;
	u32 out[MLX5_ST_SZ_DW(alloc_pd_out)] = {};
	u32 in[MLX5_ST_SZ_DW(alloc_pd_in)] = {};
	u16 uid = 0;
	struct mlx5_ib_ucontext *context = rdma_udata_to_drv_context(
		udata, struct mlx5_ib_ucontext, ibucontext);

	uid = context ? context->devx_uid : 0;
	MLX5_SET(alloc_pd_in, in, opcode, MLX5_CMD_OP_ALLOC_PD);
	MLX5_SET(alloc_pd_in, in, uid, uid);
	err = mlx5_cmd_exec_inout(to_mdev(ibdev)->mdev, alloc_pd, in, out);
	if (err)
		return err;

	pd->pdn = MLX5_GET(alloc_pd_out, out, pd);
	pd->uid = uid;
	if (udata) {
		resp.pdn = pd->pdn;
		if (ib_copy_to_udata(udata, &resp, sizeof(resp))) {
			mlx5_cmd_dealloc_pd(to_mdev(ibdev)->mdev, pd->pdn, uid);
			return -EFAULT;
		}
	}

	return 0;
}

static int mlx5_ib_dealloc_pd(struct ib_pd *pd, struct ib_udata *udata)
>>>>>>> upstream/android-13
{
	struct mlx5_ib_dev *mdev = to_mdev(pd->device);
	struct mlx5_ib_pd *mpd = to_mpd(pd);

<<<<<<< HEAD
	mlx5_core_dealloc_pd(mdev->mdev, mpd->pdn);
	kfree(mpd);

	return 0;
}

enum {
	MATCH_CRITERIA_ENABLE_OUTER_BIT,
	MATCH_CRITERIA_ENABLE_MISC_BIT,
	MATCH_CRITERIA_ENABLE_INNER_BIT,
	MATCH_CRITERIA_ENABLE_MISC2_BIT
};

#define HEADER_IS_ZERO(match_criteria, headers)			           \
	!(memchr_inv(MLX5_ADDR_OF(fte_match_param, match_criteria, headers), \
		    0, MLX5_FLD_SZ_BYTES(fte_match_param, headers)))       \

static u8 get_match_criteria_enable(u32 *match_criteria)
{
	u8 match_criteria_enable;

	match_criteria_enable =
		(!HEADER_IS_ZERO(match_criteria, outer_headers)) <<
		MATCH_CRITERIA_ENABLE_OUTER_BIT;
	match_criteria_enable |=
		(!HEADER_IS_ZERO(match_criteria, misc_parameters)) <<
		MATCH_CRITERIA_ENABLE_MISC_BIT;
	match_criteria_enable |=
		(!HEADER_IS_ZERO(match_criteria, inner_headers)) <<
		MATCH_CRITERIA_ENABLE_INNER_BIT;
	match_criteria_enable |=
		(!HEADER_IS_ZERO(match_criteria, misc_parameters_2)) <<
		MATCH_CRITERIA_ENABLE_MISC2_BIT;

	return match_criteria_enable;
}

static int set_proto(void *outer_c, void *outer_v, u8 mask, u8 val)
{
	u8 entry_mask;
	u8 entry_val;
	int err = 0;

	if (!mask)
		goto out;

	entry_mask = MLX5_GET(fte_match_set_lyr_2_4, outer_c,
			      ip_protocol);
	entry_val = MLX5_GET(fte_match_set_lyr_2_4, outer_v,
			     ip_protocol);
	if (!entry_mask) {
		MLX5_SET(fte_match_set_lyr_2_4, outer_c, ip_protocol, mask);
		MLX5_SET(fte_match_set_lyr_2_4, outer_v, ip_protocol, val);
		goto out;
	}
	/* Don't override existing ip protocol */
	if (mask != entry_mask || val != entry_val)
		err = -EINVAL;
out:
	return err;
}

static void set_flow_label(void *misc_c, void *misc_v, u32 mask, u32 val,
			   bool inner)
{
	if (inner) {
		MLX5_SET(fte_match_set_misc,
			 misc_c, inner_ipv6_flow_label, mask);
		MLX5_SET(fte_match_set_misc,
			 misc_v, inner_ipv6_flow_label, val);
	} else {
		MLX5_SET(fte_match_set_misc,
			 misc_c, outer_ipv6_flow_label, mask);
		MLX5_SET(fte_match_set_misc,
			 misc_v, outer_ipv6_flow_label, val);
	}
}

static void set_tos(void *outer_c, void *outer_v, u8 mask, u8 val)
{
	MLX5_SET(fte_match_set_lyr_2_4, outer_c, ip_ecn, mask);
	MLX5_SET(fte_match_set_lyr_2_4, outer_v, ip_ecn, val);
	MLX5_SET(fte_match_set_lyr_2_4, outer_c, ip_dscp, mask >> 2);
	MLX5_SET(fte_match_set_lyr_2_4, outer_v, ip_dscp, val >> 2);
}

static int check_mpls_supp_fields(u32 field_support, const __be32 *set_mask)
{
	if (MLX5_GET(fte_match_mpls, set_mask, mpls_label) &&
	    !(field_support & MLX5_FIELD_SUPPORT_MPLS_LABEL))
		return -EOPNOTSUPP;

	if (MLX5_GET(fte_match_mpls, set_mask, mpls_exp) &&
	    !(field_support & MLX5_FIELD_SUPPORT_MPLS_EXP))
		return -EOPNOTSUPP;

	if (MLX5_GET(fte_match_mpls, set_mask, mpls_s_bos) &&
	    !(field_support & MLX5_FIELD_SUPPORT_MPLS_S_BOS))
		return -EOPNOTSUPP;

	if (MLX5_GET(fte_match_mpls, set_mask, mpls_ttl) &&
	    !(field_support & MLX5_FIELD_SUPPORT_MPLS_TTL))
		return -EOPNOTSUPP;

	return 0;
}

#define LAST_ETH_FIELD vlan_tag
#define LAST_IB_FIELD sl
#define LAST_IPV4_FIELD tos
#define LAST_IPV6_FIELD traffic_class
#define LAST_TCP_UDP_FIELD src_port
#define LAST_TUNNEL_FIELD tunnel_id
#define LAST_FLOW_TAG_FIELD tag_id
#define LAST_DROP_FIELD size
#define LAST_COUNTERS_FIELD counters

/* Field is the last supported field */
#define FIELDS_NOT_SUPPORTED(filter, field)\
	memchr_inv((void *)&filter.field  +\
		   sizeof(filter.field), 0,\
		   sizeof(filter) -\
		   offsetof(typeof(filter), field) -\
		   sizeof(filter.field))

static int parse_flow_flow_action(const union ib_flow_spec *ib_spec,
				  const struct ib_flow_attr *flow_attr,
				  struct mlx5_flow_act *action)
{
	struct mlx5_ib_flow_action *maction = to_mflow_act(ib_spec->action.act);

	switch (maction->ib_action.type) {
	case IB_FLOW_ACTION_ESP:
		/* Currently only AES_GCM keymat is supported by the driver */
		action->esp_id = (uintptr_t)maction->esp_aes_gcm.ctx;
		action->action |= flow_attr->flags & IB_FLOW_ATTR_FLAGS_EGRESS ?
			MLX5_FLOW_CONTEXT_ACTION_ENCRYPT :
			MLX5_FLOW_CONTEXT_ACTION_DECRYPT;
		return 0;
	default:
		return -EOPNOTSUPP;
	}
}

static int parse_flow_attr(struct mlx5_core_dev *mdev, u32 *match_c,
			   u32 *match_v, const union ib_flow_spec *ib_spec,
			   const struct ib_flow_attr *flow_attr,
			   struct mlx5_flow_act *action, u32 prev_type)
{
	void *misc_params_c = MLX5_ADDR_OF(fte_match_param, match_c,
					   misc_parameters);
	void *misc_params_v = MLX5_ADDR_OF(fte_match_param, match_v,
					   misc_parameters);
	void *misc_params2_c = MLX5_ADDR_OF(fte_match_param, match_c,
					    misc_parameters_2);
	void *misc_params2_v = MLX5_ADDR_OF(fte_match_param, match_v,
					    misc_parameters_2);
	void *headers_c;
	void *headers_v;
	int match_ipv;
	int ret;

	if (ib_spec->type & IB_FLOW_SPEC_INNER) {
		headers_c = MLX5_ADDR_OF(fte_match_param, match_c,
					 inner_headers);
		headers_v = MLX5_ADDR_OF(fte_match_param, match_v,
					 inner_headers);
		match_ipv = MLX5_CAP_FLOWTABLE_NIC_RX(mdev,
					ft_field_support.inner_ip_version);
	} else {
		headers_c = MLX5_ADDR_OF(fte_match_param, match_c,
					 outer_headers);
		headers_v = MLX5_ADDR_OF(fte_match_param, match_v,
					 outer_headers);
		match_ipv = MLX5_CAP_FLOWTABLE_NIC_RX(mdev,
					ft_field_support.outer_ip_version);
	}

	switch (ib_spec->type & ~IB_FLOW_SPEC_INNER) {
	case IB_FLOW_SPEC_ETH:
		if (FIELDS_NOT_SUPPORTED(ib_spec->eth.mask, LAST_ETH_FIELD))
			return -EOPNOTSUPP;

		ether_addr_copy(MLX5_ADDR_OF(fte_match_set_lyr_2_4, headers_c,
					     dmac_47_16),
				ib_spec->eth.mask.dst_mac);
		ether_addr_copy(MLX5_ADDR_OF(fte_match_set_lyr_2_4, headers_v,
					     dmac_47_16),
				ib_spec->eth.val.dst_mac);

		ether_addr_copy(MLX5_ADDR_OF(fte_match_set_lyr_2_4, headers_c,
					     smac_47_16),
				ib_spec->eth.mask.src_mac);
		ether_addr_copy(MLX5_ADDR_OF(fte_match_set_lyr_2_4, headers_v,
					     smac_47_16),
				ib_spec->eth.val.src_mac);

		if (ib_spec->eth.mask.vlan_tag) {
			MLX5_SET(fte_match_set_lyr_2_4, headers_c,
				 cvlan_tag, 1);
			MLX5_SET(fte_match_set_lyr_2_4, headers_v,
				 cvlan_tag, 1);

			MLX5_SET(fte_match_set_lyr_2_4, headers_c,
				 first_vid, ntohs(ib_spec->eth.mask.vlan_tag));
			MLX5_SET(fte_match_set_lyr_2_4, headers_v,
				 first_vid, ntohs(ib_spec->eth.val.vlan_tag));

			MLX5_SET(fte_match_set_lyr_2_4, headers_c,
				 first_cfi,
				 ntohs(ib_spec->eth.mask.vlan_tag) >> 12);
			MLX5_SET(fte_match_set_lyr_2_4, headers_v,
				 first_cfi,
				 ntohs(ib_spec->eth.val.vlan_tag) >> 12);

			MLX5_SET(fte_match_set_lyr_2_4, headers_c,
				 first_prio,
				 ntohs(ib_spec->eth.mask.vlan_tag) >> 13);
			MLX5_SET(fte_match_set_lyr_2_4, headers_v,
				 first_prio,
				 ntohs(ib_spec->eth.val.vlan_tag) >> 13);
		}
		MLX5_SET(fte_match_set_lyr_2_4, headers_c,
			 ethertype, ntohs(ib_spec->eth.mask.ether_type));
		MLX5_SET(fte_match_set_lyr_2_4, headers_v,
			 ethertype, ntohs(ib_spec->eth.val.ether_type));
		break;
	case IB_FLOW_SPEC_IPV4:
		if (FIELDS_NOT_SUPPORTED(ib_spec->ipv4.mask, LAST_IPV4_FIELD))
			return -EOPNOTSUPP;

		if (match_ipv) {
			MLX5_SET(fte_match_set_lyr_2_4, headers_c,
				 ip_version, 0xf);
			MLX5_SET(fte_match_set_lyr_2_4, headers_v,
				 ip_version, MLX5_FS_IPV4_VERSION);
		} else {
			MLX5_SET(fte_match_set_lyr_2_4, headers_c,
				 ethertype, 0xffff);
			MLX5_SET(fte_match_set_lyr_2_4, headers_v,
				 ethertype, ETH_P_IP);
		}

		memcpy(MLX5_ADDR_OF(fte_match_set_lyr_2_4, headers_c,
				    src_ipv4_src_ipv6.ipv4_layout.ipv4),
		       &ib_spec->ipv4.mask.src_ip,
		       sizeof(ib_spec->ipv4.mask.src_ip));
		memcpy(MLX5_ADDR_OF(fte_match_set_lyr_2_4, headers_v,
				    src_ipv4_src_ipv6.ipv4_layout.ipv4),
		       &ib_spec->ipv4.val.src_ip,
		       sizeof(ib_spec->ipv4.val.src_ip));
		memcpy(MLX5_ADDR_OF(fte_match_set_lyr_2_4, headers_c,
				    dst_ipv4_dst_ipv6.ipv4_layout.ipv4),
		       &ib_spec->ipv4.mask.dst_ip,
		       sizeof(ib_spec->ipv4.mask.dst_ip));
		memcpy(MLX5_ADDR_OF(fte_match_set_lyr_2_4, headers_v,
				    dst_ipv4_dst_ipv6.ipv4_layout.ipv4),
		       &ib_spec->ipv4.val.dst_ip,
		       sizeof(ib_spec->ipv4.val.dst_ip));

		set_tos(headers_c, headers_v,
			ib_spec->ipv4.mask.tos, ib_spec->ipv4.val.tos);

		if (set_proto(headers_c, headers_v,
			      ib_spec->ipv4.mask.proto,
			      ib_spec->ipv4.val.proto))
			return -EINVAL;
		break;
	case IB_FLOW_SPEC_IPV6:
		if (FIELDS_NOT_SUPPORTED(ib_spec->ipv6.mask, LAST_IPV6_FIELD))
			return -EOPNOTSUPP;

		if (match_ipv) {
			MLX5_SET(fte_match_set_lyr_2_4, headers_c,
				 ip_version, 0xf);
			MLX5_SET(fte_match_set_lyr_2_4, headers_v,
				 ip_version, MLX5_FS_IPV6_VERSION);
		} else {
			MLX5_SET(fte_match_set_lyr_2_4, headers_c,
				 ethertype, 0xffff);
			MLX5_SET(fte_match_set_lyr_2_4, headers_v,
				 ethertype, ETH_P_IPV6);
		}

		memcpy(MLX5_ADDR_OF(fte_match_set_lyr_2_4, headers_c,
				    src_ipv4_src_ipv6.ipv6_layout.ipv6),
		       &ib_spec->ipv6.mask.src_ip,
		       sizeof(ib_spec->ipv6.mask.src_ip));
		memcpy(MLX5_ADDR_OF(fte_match_set_lyr_2_4, headers_v,
				    src_ipv4_src_ipv6.ipv6_layout.ipv6),
		       &ib_spec->ipv6.val.src_ip,
		       sizeof(ib_spec->ipv6.val.src_ip));
		memcpy(MLX5_ADDR_OF(fte_match_set_lyr_2_4, headers_c,
				    dst_ipv4_dst_ipv6.ipv6_layout.ipv6),
		       &ib_spec->ipv6.mask.dst_ip,
		       sizeof(ib_spec->ipv6.mask.dst_ip));
		memcpy(MLX5_ADDR_OF(fte_match_set_lyr_2_4, headers_v,
				    dst_ipv4_dst_ipv6.ipv6_layout.ipv6),
		       &ib_spec->ipv6.val.dst_ip,
		       sizeof(ib_spec->ipv6.val.dst_ip));

		set_tos(headers_c, headers_v,
			ib_spec->ipv6.mask.traffic_class,
			ib_spec->ipv6.val.traffic_class);

		if (set_proto(headers_c, headers_v,
			      ib_spec->ipv6.mask.next_hdr,
			      ib_spec->ipv6.val.next_hdr))
			return -EINVAL;

		set_flow_label(misc_params_c, misc_params_v,
			       ntohl(ib_spec->ipv6.mask.flow_label),
			       ntohl(ib_spec->ipv6.val.flow_label),
			       ib_spec->type & IB_FLOW_SPEC_INNER);
		break;
	case IB_FLOW_SPEC_ESP:
		if (ib_spec->esp.mask.seq)
			return -EOPNOTSUPP;

		MLX5_SET(fte_match_set_misc, misc_params_c, outer_esp_spi,
			 ntohl(ib_spec->esp.mask.spi));
		MLX5_SET(fte_match_set_misc, misc_params_v, outer_esp_spi,
			 ntohl(ib_spec->esp.val.spi));
		break;
	case IB_FLOW_SPEC_TCP:
		if (FIELDS_NOT_SUPPORTED(ib_spec->tcp_udp.mask,
					 LAST_TCP_UDP_FIELD))
			return -EOPNOTSUPP;

		if (set_proto(headers_c, headers_v, 0xff, IPPROTO_TCP))
			return -EINVAL;

		MLX5_SET(fte_match_set_lyr_2_4, headers_c, tcp_sport,
			 ntohs(ib_spec->tcp_udp.mask.src_port));
		MLX5_SET(fte_match_set_lyr_2_4, headers_v, tcp_sport,
			 ntohs(ib_spec->tcp_udp.val.src_port));

		MLX5_SET(fte_match_set_lyr_2_4, headers_c, tcp_dport,
			 ntohs(ib_spec->tcp_udp.mask.dst_port));
		MLX5_SET(fte_match_set_lyr_2_4, headers_v, tcp_dport,
			 ntohs(ib_spec->tcp_udp.val.dst_port));
		break;
	case IB_FLOW_SPEC_UDP:
		if (FIELDS_NOT_SUPPORTED(ib_spec->tcp_udp.mask,
					 LAST_TCP_UDP_FIELD))
			return -EOPNOTSUPP;

		if (set_proto(headers_c, headers_v, 0xff, IPPROTO_UDP))
			return -EINVAL;

		MLX5_SET(fte_match_set_lyr_2_4, headers_c, udp_sport,
			 ntohs(ib_spec->tcp_udp.mask.src_port));
		MLX5_SET(fte_match_set_lyr_2_4, headers_v, udp_sport,
			 ntohs(ib_spec->tcp_udp.val.src_port));

		MLX5_SET(fte_match_set_lyr_2_4, headers_c, udp_dport,
			 ntohs(ib_spec->tcp_udp.mask.dst_port));
		MLX5_SET(fte_match_set_lyr_2_4, headers_v, udp_dport,
			 ntohs(ib_spec->tcp_udp.val.dst_port));
		break;
	case IB_FLOW_SPEC_GRE:
		if (ib_spec->gre.mask.c_ks_res0_ver)
			return -EOPNOTSUPP;

		if (set_proto(headers_c, headers_v, 0xff, IPPROTO_GRE))
			return -EINVAL;

		MLX5_SET(fte_match_set_lyr_2_4, headers_c, ip_protocol,
			 0xff);
		MLX5_SET(fte_match_set_lyr_2_4, headers_v, ip_protocol,
			 IPPROTO_GRE);

		MLX5_SET(fte_match_set_misc, misc_params_c, gre_protocol,
			 ntohs(ib_spec->gre.mask.protocol));
		MLX5_SET(fte_match_set_misc, misc_params_v, gre_protocol,
			 ntohs(ib_spec->gre.val.protocol));

		memcpy(MLX5_ADDR_OF(fte_match_set_misc, misc_params_c,
				    gre_key_h),
		       &ib_spec->gre.mask.key,
		       sizeof(ib_spec->gre.mask.key));
		memcpy(MLX5_ADDR_OF(fte_match_set_misc, misc_params_v,
				    gre_key_h),
		       &ib_spec->gre.val.key,
		       sizeof(ib_spec->gre.val.key));
		break;
	case IB_FLOW_SPEC_MPLS:
		switch (prev_type) {
		case IB_FLOW_SPEC_UDP:
			if (check_mpls_supp_fields(MLX5_CAP_FLOWTABLE_NIC_RX(mdev,
						   ft_field_support.outer_first_mpls_over_udp),
						   &ib_spec->mpls.mask.tag))
				return -EOPNOTSUPP;

			memcpy(MLX5_ADDR_OF(fte_match_set_misc2, misc_params2_v,
					    outer_first_mpls_over_udp),
			       &ib_spec->mpls.val.tag,
			       sizeof(ib_spec->mpls.val.tag));
			memcpy(MLX5_ADDR_OF(fte_match_set_misc2, misc_params2_c,
					    outer_first_mpls_over_udp),
			       &ib_spec->mpls.mask.tag,
			       sizeof(ib_spec->mpls.mask.tag));
			break;
		case IB_FLOW_SPEC_GRE:
			if (check_mpls_supp_fields(MLX5_CAP_FLOWTABLE_NIC_RX(mdev,
						   ft_field_support.outer_first_mpls_over_gre),
						   &ib_spec->mpls.mask.tag))
				return -EOPNOTSUPP;

			memcpy(MLX5_ADDR_OF(fte_match_set_misc2, misc_params2_v,
					    outer_first_mpls_over_gre),
			       &ib_spec->mpls.val.tag,
			       sizeof(ib_spec->mpls.val.tag));
			memcpy(MLX5_ADDR_OF(fte_match_set_misc2, misc_params2_c,
					    outer_first_mpls_over_gre),
			       &ib_spec->mpls.mask.tag,
			       sizeof(ib_spec->mpls.mask.tag));
			break;
		default:
			if (ib_spec->type & IB_FLOW_SPEC_INNER) {
				if (check_mpls_supp_fields(MLX5_CAP_FLOWTABLE_NIC_RX(mdev,
							   ft_field_support.inner_first_mpls),
							   &ib_spec->mpls.mask.tag))
					return -EOPNOTSUPP;

				memcpy(MLX5_ADDR_OF(fte_match_set_misc2, misc_params2_v,
						    inner_first_mpls),
				       &ib_spec->mpls.val.tag,
				       sizeof(ib_spec->mpls.val.tag));
				memcpy(MLX5_ADDR_OF(fte_match_set_misc2, misc_params2_c,
						    inner_first_mpls),
				       &ib_spec->mpls.mask.tag,
				       sizeof(ib_spec->mpls.mask.tag));
			} else {
				if (check_mpls_supp_fields(MLX5_CAP_FLOWTABLE_NIC_RX(mdev,
							   ft_field_support.outer_first_mpls),
							   &ib_spec->mpls.mask.tag))
					return -EOPNOTSUPP;

				memcpy(MLX5_ADDR_OF(fte_match_set_misc2, misc_params2_v,
						    outer_first_mpls),
				       &ib_spec->mpls.val.tag,
				       sizeof(ib_spec->mpls.val.tag));
				memcpy(MLX5_ADDR_OF(fte_match_set_misc2, misc_params2_c,
						    outer_first_mpls),
				       &ib_spec->mpls.mask.tag,
				       sizeof(ib_spec->mpls.mask.tag));
			}
		}
		break;
	case IB_FLOW_SPEC_VXLAN_TUNNEL:
		if (FIELDS_NOT_SUPPORTED(ib_spec->tunnel.mask,
					 LAST_TUNNEL_FIELD))
			return -EOPNOTSUPP;

		MLX5_SET(fte_match_set_misc, misc_params_c, vxlan_vni,
			 ntohl(ib_spec->tunnel.mask.tunnel_id));
		MLX5_SET(fte_match_set_misc, misc_params_v, vxlan_vni,
			 ntohl(ib_spec->tunnel.val.tunnel_id));
		break;
	case IB_FLOW_SPEC_ACTION_TAG:
		if (FIELDS_NOT_SUPPORTED(ib_spec->flow_tag,
					 LAST_FLOW_TAG_FIELD))
			return -EOPNOTSUPP;
		if (ib_spec->flow_tag.tag_id >= BIT(24))
			return -EINVAL;

		action->flow_tag = ib_spec->flow_tag.tag_id;
		action->has_flow_tag = true;
		break;
	case IB_FLOW_SPEC_ACTION_DROP:
		if (FIELDS_NOT_SUPPORTED(ib_spec->drop,
					 LAST_DROP_FIELD))
			return -EOPNOTSUPP;
		action->action |= MLX5_FLOW_CONTEXT_ACTION_DROP;
		break;
	case IB_FLOW_SPEC_ACTION_HANDLE:
		ret = parse_flow_flow_action(ib_spec, flow_attr, action);
		if (ret)
			return ret;
		break;
	case IB_FLOW_SPEC_ACTION_COUNT:
		if (FIELDS_NOT_SUPPORTED(ib_spec->flow_count,
					 LAST_COUNTERS_FIELD))
			return -EOPNOTSUPP;

		/* for now support only one counters spec per flow */
		if (action->action & MLX5_FLOW_CONTEXT_ACTION_COUNT)
			return -EINVAL;

		action->counters = ib_spec->flow_count.counters;
		action->action |= MLX5_FLOW_CONTEXT_ACTION_COUNT;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

/* If a flow could catch both multicast and unicast packets,
 * it won't fall into the multicast flow steering table and this rule
 * could steal other multicast packets.
 */
static bool flow_is_multicast_only(const struct ib_flow_attr *ib_attr)
{
	union ib_flow_spec *flow_spec;

	if (ib_attr->type != IB_FLOW_ATTR_NORMAL ||
	    ib_attr->num_of_specs < 1)
		return false;

	flow_spec = (union ib_flow_spec *)(ib_attr + 1);
	if (flow_spec->type == IB_FLOW_SPEC_IPV4) {
		struct ib_flow_spec_ipv4 *ipv4_spec;

		ipv4_spec = (struct ib_flow_spec_ipv4 *)flow_spec;
		if (ipv4_is_multicast(ipv4_spec->val.dst_ip))
			return true;

		return false;
	}

	if (flow_spec->type == IB_FLOW_SPEC_ETH) {
		struct ib_flow_spec_eth *eth_spec;

		eth_spec = (struct ib_flow_spec_eth *)flow_spec;
		return is_multicast_ether_addr(eth_spec->mask.dst_mac) &&
		       is_multicast_ether_addr(eth_spec->val.dst_mac);
	}

	return false;
}

enum valid_spec {
	VALID_SPEC_INVALID,
	VALID_SPEC_VALID,
	VALID_SPEC_NA,
};

static enum valid_spec
is_valid_esp_aes_gcm(struct mlx5_core_dev *mdev,
		     const struct mlx5_flow_spec *spec,
		     const struct mlx5_flow_act *flow_act,
		     bool egress)
{
	const u32 *match_c = spec->match_criteria;
	bool is_crypto =
		(flow_act->action & (MLX5_FLOW_CONTEXT_ACTION_ENCRYPT |
				     MLX5_FLOW_CONTEXT_ACTION_DECRYPT));
	bool is_ipsec = mlx5_fs_is_ipsec_flow(match_c);
	bool is_drop = flow_act->action & MLX5_FLOW_CONTEXT_ACTION_DROP;

	/*
	 * Currently only crypto is supported in egress, when regular egress
	 * rules would be supported, always return VALID_SPEC_NA.
	 */
	if (!is_crypto)
		return egress ? VALID_SPEC_INVALID : VALID_SPEC_NA;

	return is_crypto && is_ipsec &&
		(!egress || (!is_drop && !flow_act->has_flow_tag)) ?
		VALID_SPEC_VALID : VALID_SPEC_INVALID;
}

static bool is_valid_spec(struct mlx5_core_dev *mdev,
			  const struct mlx5_flow_spec *spec,
			  const struct mlx5_flow_act *flow_act,
			  bool egress)
{
	/* We curretly only support ipsec egress flow */
	return is_valid_esp_aes_gcm(mdev, spec, flow_act, egress) != VALID_SPEC_INVALID;
}

static bool is_valid_ethertype(struct mlx5_core_dev *mdev,
			       const struct ib_flow_attr *flow_attr,
			       bool check_inner)
{
	union ib_flow_spec *ib_spec = (union ib_flow_spec *)(flow_attr + 1);
	int match_ipv = check_inner ?
			MLX5_CAP_FLOWTABLE_NIC_RX(mdev,
					ft_field_support.inner_ip_version) :
			MLX5_CAP_FLOWTABLE_NIC_RX(mdev,
					ft_field_support.outer_ip_version);
	int inner_bit = check_inner ? IB_FLOW_SPEC_INNER : 0;
	bool ipv4_spec_valid, ipv6_spec_valid;
	unsigned int ip_spec_type = 0;
	bool has_ethertype = false;
	unsigned int spec_index;
	bool mask_valid = true;
	u16 eth_type = 0;
	bool type_valid;

	/* Validate that ethertype is correct */
	for (spec_index = 0; spec_index < flow_attr->num_of_specs; spec_index++) {
		if ((ib_spec->type == (IB_FLOW_SPEC_ETH | inner_bit)) &&
		    ib_spec->eth.mask.ether_type) {
			mask_valid = (ib_spec->eth.mask.ether_type ==
				      htons(0xffff));
			has_ethertype = true;
			eth_type = ntohs(ib_spec->eth.val.ether_type);
		} else if ((ib_spec->type == (IB_FLOW_SPEC_IPV4 | inner_bit)) ||
			   (ib_spec->type == (IB_FLOW_SPEC_IPV6 | inner_bit))) {
			ip_spec_type = ib_spec->type;
		}
		ib_spec = (void *)ib_spec + ib_spec->size;
	}

	type_valid = (!has_ethertype) || (!ip_spec_type);
	if (!type_valid && mask_valid) {
		ipv4_spec_valid = (eth_type == ETH_P_IP) &&
			(ip_spec_type == (IB_FLOW_SPEC_IPV4 | inner_bit));
		ipv6_spec_valid = (eth_type == ETH_P_IPV6) &&
			(ip_spec_type == (IB_FLOW_SPEC_IPV6 | inner_bit));

		type_valid = (ipv4_spec_valid) || (ipv6_spec_valid) ||
			     (((eth_type == ETH_P_MPLS_UC) ||
			       (eth_type == ETH_P_MPLS_MC)) && match_ipv);
	}

	return type_valid;
}

static bool is_valid_attr(struct mlx5_core_dev *mdev,
			  const struct ib_flow_attr *flow_attr)
{
	return is_valid_ethertype(mdev, flow_attr, false) &&
	       is_valid_ethertype(mdev, flow_attr, true);
}

static void put_flow_table(struct mlx5_ib_dev *dev,
			   struct mlx5_ib_flow_prio *prio, bool ft_added)
{
	prio->refcount -= !!ft_added;
	if (!prio->refcount) {
		mlx5_destroy_flow_table(prio->flow_table);
		prio->flow_table = NULL;
	}
}

static void counters_clear_description(struct ib_counters *counters)
{
	struct mlx5_ib_mcounters *mcounters = to_mcounters(counters);

	mutex_lock(&mcounters->mcntrs_mutex);
	kfree(mcounters->counters_data);
	mcounters->counters_data = NULL;
	mcounters->cntrs_max_index = 0;
	mutex_unlock(&mcounters->mcntrs_mutex);
}

static int mlx5_ib_destroy_flow(struct ib_flow *flow_id)
{
	struct mlx5_ib_flow_handler *handler = container_of(flow_id,
							  struct mlx5_ib_flow_handler,
							  ibflow);
	struct mlx5_ib_flow_handler *iter, *tmp;
	struct mlx5_ib_dev *dev = handler->dev;

	mutex_lock(&dev->flow_db->lock);

	list_for_each_entry_safe(iter, tmp, &handler->list, list) {
		mlx5_del_flow_rules(iter->rule);
		put_flow_table(dev, iter->prio, true);
		list_del(&iter->list);
		kfree(iter);
	}

	mlx5_del_flow_rules(handler->rule);
	put_flow_table(dev, handler->prio, true);
	if (handler->ibcounters &&
	    atomic_read(&handler->ibcounters->usecnt) == 1)
		counters_clear_description(handler->ibcounters);

	mutex_unlock(&dev->flow_db->lock);
	if (handler->flow_matcher)
		atomic_dec(&handler->flow_matcher->usecnt);
	kfree(handler);

	return 0;
}

static int ib_prio_to_core_prio(unsigned int priority, bool dont_trap)
{
	priority *= 2;
	if (!dont_trap)
		priority++;
	return priority;
}

enum flow_table_type {
	MLX5_IB_FT_RX,
	MLX5_IB_FT_TX
};

#define MLX5_FS_MAX_TYPES	 6
#define MLX5_FS_MAX_ENTRIES	 BIT(16)

static struct mlx5_ib_flow_prio *_get_prio(struct mlx5_flow_namespace *ns,
					   struct mlx5_ib_flow_prio *prio,
					   int priority,
					   int num_entries, int num_groups)
{
	struct mlx5_flow_table *ft;

	ft = mlx5_create_auto_grouped_flow_table(ns, priority,
						 num_entries,
						 num_groups,
						 0, 0);
	if (IS_ERR(ft))
		return ERR_CAST(ft);

	prio->flow_table = ft;
	prio->refcount = 0;
	return prio;
}

static struct mlx5_ib_flow_prio *get_flow_table(struct mlx5_ib_dev *dev,
						struct ib_flow_attr *flow_attr,
						enum flow_table_type ft_type)
{
	bool dont_trap = flow_attr->flags & IB_FLOW_ATTR_FLAGS_DONT_TRAP;
	struct mlx5_flow_namespace *ns = NULL;
	struct mlx5_ib_flow_prio *prio;
	struct mlx5_flow_table *ft;
	int max_table_size;
	int num_entries;
	int num_groups;
	int priority;

	max_table_size = BIT(MLX5_CAP_FLOWTABLE_NIC_RX(dev->mdev,
						       log_max_ft_size));
	if (flow_attr->type == IB_FLOW_ATTR_NORMAL) {
		if (ft_type == MLX5_IB_FT_TX)
			priority = 0;
		else if (flow_is_multicast_only(flow_attr) &&
			 !dont_trap)
			priority = MLX5_IB_FLOW_MCAST_PRIO;
		else
			priority = ib_prio_to_core_prio(flow_attr->priority,
							dont_trap);
		ns = mlx5_get_flow_namespace(dev->mdev,
					     ft_type == MLX5_IB_FT_TX ?
					     MLX5_FLOW_NAMESPACE_EGRESS :
					     MLX5_FLOW_NAMESPACE_BYPASS);
		num_entries = MLX5_FS_MAX_ENTRIES;
		num_groups = MLX5_FS_MAX_TYPES;
		prio = &dev->flow_db->prios[priority];
	} else if (flow_attr->type == IB_FLOW_ATTR_ALL_DEFAULT ||
		   flow_attr->type == IB_FLOW_ATTR_MC_DEFAULT) {
		ns = mlx5_get_flow_namespace(dev->mdev,
					     MLX5_FLOW_NAMESPACE_LEFTOVERS);
		build_leftovers_ft_param(&priority,
					 &num_entries,
					 &num_groups);
		prio = &dev->flow_db->prios[MLX5_IB_FLOW_LEFTOVERS_PRIO];
	} else if (flow_attr->type == IB_FLOW_ATTR_SNIFFER) {
		if (!MLX5_CAP_FLOWTABLE(dev->mdev,
					allow_sniffer_and_nic_rx_shared_tir))
			return ERR_PTR(-ENOTSUPP);

		ns = mlx5_get_flow_namespace(dev->mdev, ft_type == MLX5_IB_FT_RX ?
					     MLX5_FLOW_NAMESPACE_SNIFFER_RX :
					     MLX5_FLOW_NAMESPACE_SNIFFER_TX);

		prio = &dev->flow_db->sniffer[ft_type];
		priority = 0;
		num_entries = 1;
		num_groups = 1;
	}

	if (!ns)
		return ERR_PTR(-ENOTSUPP);

	if (num_entries > max_table_size)
		return ERR_PTR(-ENOMEM);

	ft = prio->flow_table;
	if (!ft)
		return _get_prio(ns, prio, priority, num_entries, num_groups);

	return prio;
}

static void set_underlay_qp(struct mlx5_ib_dev *dev,
			    struct mlx5_flow_spec *spec,
			    u32 underlay_qpn)
{
	void *misc_params_c = MLX5_ADDR_OF(fte_match_param,
					   spec->match_criteria,
					   misc_parameters);
	void *misc_params_v = MLX5_ADDR_OF(fte_match_param, spec->match_value,
					   misc_parameters);

	if (underlay_qpn &&
	    MLX5_CAP_FLOWTABLE_NIC_RX(dev->mdev,
				      ft_field_support.bth_dst_qp)) {
		MLX5_SET(fte_match_set_misc,
			 misc_params_v, bth_dst_qp, underlay_qpn);
		MLX5_SET(fte_match_set_misc,
			 misc_params_c, bth_dst_qp, 0xffffff);
	}
}

static int read_flow_counters(struct ib_device *ibdev,
			      struct mlx5_read_counters_attr *read_attr)
{
	struct mlx5_fc *fc = read_attr->hw_cntrs_hndl;
	struct mlx5_ib_dev *dev = to_mdev(ibdev);

	return mlx5_fc_query(dev->mdev, fc,
			     &read_attr->out[IB_COUNTER_PACKETS],
			     &read_attr->out[IB_COUNTER_BYTES]);
}

/* flow counters currently expose two counters packets and bytes */
#define FLOW_COUNTERS_NUM 2
static int counters_set_description(struct ib_counters *counters,
				    enum mlx5_ib_counters_type counters_type,
				    struct mlx5_ib_flow_counters_desc *desc_data,
				    u32 ncounters)
{
	struct mlx5_ib_mcounters *mcounters = to_mcounters(counters);
	u32 cntrs_max_index = 0;
	int i;

	if (counters_type != MLX5_IB_COUNTERS_FLOW)
		return -EINVAL;

	/* init the fields for the object */
	mcounters->type = counters_type;
	mcounters->read_counters = read_flow_counters;
	mcounters->counters_num = FLOW_COUNTERS_NUM;
	mcounters->ncounters = ncounters;
	/* each counter entry have both description and index pair */
	for (i = 0; i < ncounters; i++) {
		if (desc_data[i].description > IB_COUNTER_BYTES)
			return -EINVAL;

		if (cntrs_max_index <= desc_data[i].index)
			cntrs_max_index = desc_data[i].index + 1;
	}

	mutex_lock(&mcounters->mcntrs_mutex);
	mcounters->counters_data = desc_data;
	mcounters->cntrs_max_index = cntrs_max_index;
	mutex_unlock(&mcounters->mcntrs_mutex);

	return 0;
}

#define MAX_COUNTERS_NUM (USHRT_MAX / (sizeof(u32) * 2))
static int flow_counters_set_data(struct ib_counters *ibcounters,
				  struct mlx5_ib_create_flow *ucmd)
{
	struct mlx5_ib_mcounters *mcounters = to_mcounters(ibcounters);
	struct mlx5_ib_flow_counters_data *cntrs_data = NULL;
	struct mlx5_ib_flow_counters_desc *desc_data = NULL;
	bool hw_hndl = false;
	int ret = 0;

	if (ucmd && ucmd->ncounters_data != 0) {
		cntrs_data = ucmd->data;
		if (cntrs_data->ncounters > MAX_COUNTERS_NUM)
			return -EINVAL;

		desc_data = kcalloc(cntrs_data->ncounters,
				    sizeof(*desc_data),
				    GFP_KERNEL);
		if (!desc_data)
			return  -ENOMEM;

		if (copy_from_user(desc_data,
				   u64_to_user_ptr(cntrs_data->counters_data),
				   sizeof(*desc_data) * cntrs_data->ncounters)) {
			ret = -EFAULT;
			goto free;
		}
	}

	if (!mcounters->hw_cntrs_hndl) {
		mcounters->hw_cntrs_hndl = mlx5_fc_create(
			to_mdev(ibcounters->device)->mdev, false);
		if (IS_ERR(mcounters->hw_cntrs_hndl)) {
			ret = PTR_ERR(mcounters->hw_cntrs_hndl);
			goto free;
		}
		hw_hndl = true;
	}

	if (desc_data) {
		/* counters already bound to at least one flow */
		if (mcounters->cntrs_max_index) {
			ret = -EINVAL;
			goto free_hndl;
		}

		ret = counters_set_description(ibcounters,
					       MLX5_IB_COUNTERS_FLOW,
					       desc_data,
					       cntrs_data->ncounters);
		if (ret)
			goto free_hndl;

	} else if (!mcounters->cntrs_max_index) {
		/* counters not bound yet, must have udata passed */
		ret = -EINVAL;
		goto free_hndl;
	}

	return 0;

free_hndl:
	if (hw_hndl) {
		mlx5_fc_destroy(to_mdev(ibcounters->device)->mdev,
				mcounters->hw_cntrs_hndl);
		mcounters->hw_cntrs_hndl = NULL;
	}
free:
	kfree(desc_data);
	return ret;
}

static struct mlx5_ib_flow_handler *_create_flow_rule(struct mlx5_ib_dev *dev,
						      struct mlx5_ib_flow_prio *ft_prio,
						      const struct ib_flow_attr *flow_attr,
						      struct mlx5_flow_destination *dst,
						      u32 underlay_qpn,
						      struct mlx5_ib_create_flow *ucmd)
{
	struct mlx5_flow_table	*ft = ft_prio->flow_table;
	struct mlx5_ib_flow_handler *handler;
	struct mlx5_flow_act flow_act = {.flow_tag = MLX5_FS_DEFAULT_FLOW_TAG};
	struct mlx5_flow_spec *spec;
	struct mlx5_flow_destination dest_arr[2] = {};
	struct mlx5_flow_destination *rule_dst = dest_arr;
	const void *ib_flow = (const void *)flow_attr + sizeof(*flow_attr);
	unsigned int spec_index;
	u32 prev_type = 0;
	int err = 0;
	int dest_num = 0;
	bool is_egress = flow_attr->flags & IB_FLOW_ATTR_FLAGS_EGRESS;

	if (!is_valid_attr(dev->mdev, flow_attr))
		return ERR_PTR(-EINVAL);

	spec = kvzalloc(sizeof(*spec), GFP_KERNEL);
	handler = kzalloc(sizeof(*handler), GFP_KERNEL);
	if (!handler || !spec) {
		err = -ENOMEM;
		goto free;
	}

	INIT_LIST_HEAD(&handler->list);

	for (spec_index = 0; spec_index < flow_attr->num_of_specs; spec_index++) {
		err = parse_flow_attr(dev->mdev, spec->match_criteria,
				      spec->match_value,
				      ib_flow, flow_attr, &flow_act,
				      prev_type);
		if (err < 0)
			goto free;

		prev_type = ((union ib_flow_spec *)ib_flow)->type;
		ib_flow += ((union ib_flow_spec *)ib_flow)->size;
	}

	if (dst && !(flow_act.action & MLX5_FLOW_CONTEXT_ACTION_DROP)) {
		memcpy(&dest_arr[0], dst, sizeof(*dst));
		dest_num++;
	}

	if (!flow_is_multicast_only(flow_attr))
		set_underlay_qp(dev, spec, underlay_qpn);

	if (dev->rep) {
		void *misc;

		misc = MLX5_ADDR_OF(fte_match_param, spec->match_value,
				    misc_parameters);
		MLX5_SET(fte_match_set_misc, misc, source_port,
			 dev->rep->vport);
		misc = MLX5_ADDR_OF(fte_match_param, spec->match_criteria,
				    misc_parameters);
		MLX5_SET_TO_ONES(fte_match_set_misc, misc, source_port);
	}

	spec->match_criteria_enable = get_match_criteria_enable(spec->match_criteria);

	if (is_egress &&
	    !is_valid_spec(dev->mdev, spec, &flow_act, is_egress)) {
		err = -EINVAL;
		goto free;
	}

	if (flow_act.action & MLX5_FLOW_CONTEXT_ACTION_COUNT) {
		err = flow_counters_set_data(flow_act.counters, ucmd);
		if (err)
			goto free;

		handler->ibcounters = flow_act.counters;
		dest_arr[dest_num].type =
			MLX5_FLOW_DESTINATION_TYPE_COUNTER;
		dest_arr[dest_num].counter =
			to_mcounters(flow_act.counters)->hw_cntrs_hndl;
		dest_num++;
	}

	if (flow_act.action & MLX5_FLOW_CONTEXT_ACTION_DROP) {
		if (!dest_num)
			rule_dst = NULL;
	} else {
		if (is_egress)
			flow_act.action |= MLX5_FLOW_CONTEXT_ACTION_ALLOW;
		else
			flow_act.action |=
				dest_num ?  MLX5_FLOW_CONTEXT_ACTION_FWD_DEST :
					MLX5_FLOW_CONTEXT_ACTION_FWD_NEXT_PRIO;
	}

	if (flow_act.has_flow_tag &&
	    (flow_attr->type == IB_FLOW_ATTR_ALL_DEFAULT ||
	     flow_attr->type == IB_FLOW_ATTR_MC_DEFAULT)) {
		mlx5_ib_warn(dev, "Flow tag %u and attribute type %x isn't allowed in leftovers\n",
			     flow_act.flow_tag, flow_attr->type);
		err = -EINVAL;
		goto free;
	}
	handler->rule = mlx5_add_flow_rules(ft, spec,
					    &flow_act,
					    rule_dst, dest_num);

	if (IS_ERR(handler->rule)) {
		err = PTR_ERR(handler->rule);
		goto free;
	}

	ft_prio->refcount++;
	handler->prio = ft_prio;
	handler->dev = dev;

	ft_prio->flow_table = ft;
free:
	if (err && handler) {
		if (handler->ibcounters &&
		    atomic_read(&handler->ibcounters->usecnt) == 1)
			counters_clear_description(handler->ibcounters);
		kfree(handler);
	}
	kvfree(spec);
	return err ? ERR_PTR(err) : handler;
}

static struct mlx5_ib_flow_handler *create_flow_rule(struct mlx5_ib_dev *dev,
						     struct mlx5_ib_flow_prio *ft_prio,
						     const struct ib_flow_attr *flow_attr,
						     struct mlx5_flow_destination *dst)
{
	return _create_flow_rule(dev, ft_prio, flow_attr, dst, 0, NULL);
}

static struct mlx5_ib_flow_handler *create_dont_trap_rule(struct mlx5_ib_dev *dev,
							  struct mlx5_ib_flow_prio *ft_prio,
							  struct ib_flow_attr *flow_attr,
							  struct mlx5_flow_destination *dst)
{
	struct mlx5_ib_flow_handler *handler_dst = NULL;
	struct mlx5_ib_flow_handler *handler = NULL;

	handler = create_flow_rule(dev, ft_prio, flow_attr, NULL);
	if (!IS_ERR(handler)) {
		handler_dst = create_flow_rule(dev, ft_prio,
					       flow_attr, dst);
		if (IS_ERR(handler_dst)) {
			mlx5_del_flow_rules(handler->rule);
			ft_prio->refcount--;
			kfree(handler);
			handler = handler_dst;
		} else {
			list_add(&handler_dst->list, &handler->list);
		}
	}

	return handler;
}
enum {
	LEFTOVERS_MC,
	LEFTOVERS_UC,
};

static struct mlx5_ib_flow_handler *create_leftovers_rule(struct mlx5_ib_dev *dev,
							  struct mlx5_ib_flow_prio *ft_prio,
							  struct ib_flow_attr *flow_attr,
							  struct mlx5_flow_destination *dst)
{
	struct mlx5_ib_flow_handler *handler_ucast = NULL;
	struct mlx5_ib_flow_handler *handler = NULL;

	static struct {
		struct ib_flow_attr	flow_attr;
		struct ib_flow_spec_eth eth_flow;
	} leftovers_specs[] = {
		[LEFTOVERS_MC] = {
			.flow_attr = {
				.num_of_specs = 1,
				.size = sizeof(leftovers_specs[0])
			},
			.eth_flow = {
				.type = IB_FLOW_SPEC_ETH,
				.size = sizeof(struct ib_flow_spec_eth),
				.mask = {.dst_mac = {0x1} },
				.val =  {.dst_mac = {0x1} }
			}
		},
		[LEFTOVERS_UC] = {
			.flow_attr = {
				.num_of_specs = 1,
				.size = sizeof(leftovers_specs[0])
			},
			.eth_flow = {
				.type = IB_FLOW_SPEC_ETH,
				.size = sizeof(struct ib_flow_spec_eth),
				.mask = {.dst_mac = {0x1} },
				.val = {.dst_mac = {} }
			}
		}
	};

	handler = create_flow_rule(dev, ft_prio,
				   &leftovers_specs[LEFTOVERS_MC].flow_attr,
				   dst);
	if (!IS_ERR(handler) &&
	    flow_attr->type == IB_FLOW_ATTR_ALL_DEFAULT) {
		handler_ucast = create_flow_rule(dev, ft_prio,
						 &leftovers_specs[LEFTOVERS_UC].flow_attr,
						 dst);
		if (IS_ERR(handler_ucast)) {
			mlx5_del_flow_rules(handler->rule);
			ft_prio->refcount--;
			kfree(handler);
			handler = handler_ucast;
		} else {
			list_add(&handler_ucast->list, &handler->list);
		}
	}

	return handler;
}

static struct mlx5_ib_flow_handler *create_sniffer_rule(struct mlx5_ib_dev *dev,
							struct mlx5_ib_flow_prio *ft_rx,
							struct mlx5_ib_flow_prio *ft_tx,
							struct mlx5_flow_destination *dst)
{
	struct mlx5_ib_flow_handler *handler_rx;
	struct mlx5_ib_flow_handler *handler_tx;
	int err;
	static const struct ib_flow_attr flow_attr  = {
		.num_of_specs = 0,
		.size = sizeof(flow_attr)
	};

	handler_rx = create_flow_rule(dev, ft_rx, &flow_attr, dst);
	if (IS_ERR(handler_rx)) {
		err = PTR_ERR(handler_rx);
		goto err;
	}

	handler_tx = create_flow_rule(dev, ft_tx, &flow_attr, dst);
	if (IS_ERR(handler_tx)) {
		err = PTR_ERR(handler_tx);
		goto err_tx;
	}

	list_add(&handler_tx->list, &handler_rx->list);

	return handler_rx;

err_tx:
	mlx5_del_flow_rules(handler_rx->rule);
	ft_rx->refcount--;
	kfree(handler_rx);
err:
	return ERR_PTR(err);
}

static struct ib_flow *mlx5_ib_create_flow(struct ib_qp *qp,
					   struct ib_flow_attr *flow_attr,
					   int domain,
					   struct ib_udata *udata)
{
	struct mlx5_ib_dev *dev = to_mdev(qp->device);
	struct mlx5_ib_qp *mqp = to_mqp(qp);
	struct mlx5_ib_flow_handler *handler = NULL;
	struct mlx5_flow_destination *dst = NULL;
	struct mlx5_ib_flow_prio *ft_prio_tx = NULL;
	struct mlx5_ib_flow_prio *ft_prio;
	bool is_egress = flow_attr->flags & IB_FLOW_ATTR_FLAGS_EGRESS;
	struct mlx5_ib_create_flow *ucmd = NULL, ucmd_hdr;
	size_t min_ucmd_sz, required_ucmd_sz;
	int err;
	int underlay_qpn;

	if (udata && udata->inlen) {
		min_ucmd_sz = offsetof(typeof(ucmd_hdr), reserved) +
				sizeof(ucmd_hdr.reserved);
		if (udata->inlen < min_ucmd_sz)
			return ERR_PTR(-EOPNOTSUPP);

		err = ib_copy_from_udata(&ucmd_hdr, udata, min_ucmd_sz);
		if (err)
			return ERR_PTR(err);

		/* currently supports only one counters data */
		if (ucmd_hdr.ncounters_data > 1)
			return ERR_PTR(-EINVAL);

		required_ucmd_sz = min_ucmd_sz +
			sizeof(struct mlx5_ib_flow_counters_data) *
			ucmd_hdr.ncounters_data;
		if (udata->inlen > required_ucmd_sz &&
		    !ib_is_udata_cleared(udata, required_ucmd_sz,
					 udata->inlen - required_ucmd_sz))
			return ERR_PTR(-EOPNOTSUPP);

		ucmd = kzalloc(required_ucmd_sz, GFP_KERNEL);
		if (!ucmd)
			return ERR_PTR(-ENOMEM);

		err = ib_copy_from_udata(ucmd, udata, required_ucmd_sz);
		if (err)
			goto free_ucmd;
	}

	if (flow_attr->priority > MLX5_IB_FLOW_LAST_PRIO) {
		err = -ENOMEM;
		goto free_ucmd;
	}

	if (domain != IB_FLOW_DOMAIN_USER ||
	    flow_attr->port > dev->num_ports ||
	    (flow_attr->flags & ~(IB_FLOW_ATTR_FLAGS_DONT_TRAP |
				  IB_FLOW_ATTR_FLAGS_EGRESS))) {
		err = -EINVAL;
		goto free_ucmd;
	}

	if (is_egress &&
	    (flow_attr->type == IB_FLOW_ATTR_ALL_DEFAULT ||
	     flow_attr->type == IB_FLOW_ATTR_MC_DEFAULT)) {
		err = -EINVAL;
		goto free_ucmd;
	}

	dst = kzalloc(sizeof(*dst), GFP_KERNEL);
	if (!dst) {
		err = -ENOMEM;
		goto free_ucmd;
	}

	mutex_lock(&dev->flow_db->lock);

	ft_prio = get_flow_table(dev, flow_attr,
				 is_egress ? MLX5_IB_FT_TX : MLX5_IB_FT_RX);
	if (IS_ERR(ft_prio)) {
		err = PTR_ERR(ft_prio);
		goto unlock;
	}
	if (flow_attr->type == IB_FLOW_ATTR_SNIFFER) {
		ft_prio_tx = get_flow_table(dev, flow_attr, MLX5_IB_FT_TX);
		if (IS_ERR(ft_prio_tx)) {
			err = PTR_ERR(ft_prio_tx);
			ft_prio_tx = NULL;
			goto destroy_ft;
		}
	}

	if (is_egress) {
		dst->type = MLX5_FLOW_DESTINATION_TYPE_PORT;
	} else {
		dst->type = MLX5_FLOW_DESTINATION_TYPE_TIR;
		if (mqp->flags & MLX5_IB_QP_RSS)
			dst->tir_num = mqp->rss_qp.tirn;
		else
			dst->tir_num = mqp->raw_packet_qp.rq.tirn;
	}

	if (flow_attr->type == IB_FLOW_ATTR_NORMAL) {
		if (flow_attr->flags & IB_FLOW_ATTR_FLAGS_DONT_TRAP)  {
			handler = create_dont_trap_rule(dev, ft_prio,
							flow_attr, dst);
		} else {
			underlay_qpn = (mqp->flags & MLX5_IB_QP_UNDERLAY) ?
					mqp->underlay_qpn : 0;
			handler = _create_flow_rule(dev, ft_prio, flow_attr,
						    dst, underlay_qpn, ucmd);
		}
	} else if (flow_attr->type == IB_FLOW_ATTR_ALL_DEFAULT ||
		   flow_attr->type == IB_FLOW_ATTR_MC_DEFAULT) {
		handler = create_leftovers_rule(dev, ft_prio, flow_attr,
						dst);
	} else if (flow_attr->type == IB_FLOW_ATTR_SNIFFER) {
		handler = create_sniffer_rule(dev, ft_prio, ft_prio_tx, dst);
	} else {
		err = -EINVAL;
		goto destroy_ft;
	}

	if (IS_ERR(handler)) {
		err = PTR_ERR(handler);
		handler = NULL;
		goto destroy_ft;
	}

	mutex_unlock(&dev->flow_db->lock);
	kfree(dst);
	kfree(ucmd);

	return &handler->ibflow;

destroy_ft:
	put_flow_table(dev, ft_prio, false);
	if (ft_prio_tx)
		put_flow_table(dev, ft_prio_tx, false);
unlock:
	mutex_unlock(&dev->flow_db->lock);
	kfree(dst);
free_ucmd:
	kfree(ucmd);
	return ERR_PTR(err);
}

static struct mlx5_ib_flow_prio *_get_flow_table(struct mlx5_ib_dev *dev,
						 int priority, bool mcast)
{
	int max_table_size;
	struct mlx5_flow_namespace *ns = NULL;
	struct mlx5_ib_flow_prio *prio;

	max_table_size = BIT(MLX5_CAP_FLOWTABLE_NIC_RX(dev->mdev,
			     log_max_ft_size));
	if (max_table_size < MLX5_FS_MAX_ENTRIES)
		return ERR_PTR(-ENOMEM);

	if (mcast)
		priority = MLX5_IB_FLOW_MCAST_PRIO;
	else
		priority = ib_prio_to_core_prio(priority, false);

	ns = mlx5_get_flow_namespace(dev->mdev, MLX5_FLOW_NAMESPACE_BYPASS);
	if (!ns)
		return ERR_PTR(-ENOTSUPP);

	prio = &dev->flow_db->prios[priority];

	if (prio->flow_table)
		return prio;

	return _get_prio(ns, prio, priority, MLX5_FS_MAX_ENTRIES,
			 MLX5_FS_MAX_TYPES);
}

static struct mlx5_ib_flow_handler *
_create_raw_flow_rule(struct mlx5_ib_dev *dev,
		      struct mlx5_ib_flow_prio *ft_prio,
		      struct mlx5_flow_destination *dst,
		      struct mlx5_ib_flow_matcher  *fs_matcher,
		      void *cmd_in, int inlen)
{
	struct mlx5_ib_flow_handler *handler;
	struct mlx5_flow_act flow_act = {.flow_tag = MLX5_FS_DEFAULT_FLOW_TAG};
	struct mlx5_flow_spec *spec;
	struct mlx5_flow_table *ft = ft_prio->flow_table;
	int err = 0;

	spec = kvzalloc(sizeof(*spec), GFP_KERNEL);
	handler = kzalloc(sizeof(*handler), GFP_KERNEL);
	if (!handler || !spec) {
		err = -ENOMEM;
		goto free;
	}

	INIT_LIST_HEAD(&handler->list);

	memcpy(spec->match_value, cmd_in, inlen);
	memcpy(spec->match_criteria, fs_matcher->matcher_mask.match_params,
	       fs_matcher->mask_len);
	spec->match_criteria_enable = fs_matcher->match_criteria_enable;

	flow_act.action |= MLX5_FLOW_CONTEXT_ACTION_FWD_DEST;
	handler->rule = mlx5_add_flow_rules(ft, spec,
					    &flow_act, dst, 1);

	if (IS_ERR(handler->rule)) {
		err = PTR_ERR(handler->rule);
		goto free;
	}

	ft_prio->refcount++;
	handler->prio = ft_prio;
	handler->dev = dev;
	ft_prio->flow_table = ft;

free:
	if (err)
		kfree(handler);
	kvfree(spec);
	return err ? ERR_PTR(err) : handler;
}

static bool raw_fs_is_multicast(struct mlx5_ib_flow_matcher *fs_matcher,
				void *match_v)
{
	void *match_c;
	void *match_v_set_lyr_2_4, *match_c_set_lyr_2_4;
	void *dmac, *dmac_mask;
	void *ipv4, *ipv4_mask;

	if (!(fs_matcher->match_criteria_enable &
	      (1 << MATCH_CRITERIA_ENABLE_OUTER_BIT)))
		return false;

	match_c = fs_matcher->matcher_mask.match_params;
	match_v_set_lyr_2_4 = MLX5_ADDR_OF(fte_match_param, match_v,
					   outer_headers);
	match_c_set_lyr_2_4 = MLX5_ADDR_OF(fte_match_param, match_c,
					   outer_headers);

	dmac = MLX5_ADDR_OF(fte_match_set_lyr_2_4, match_v_set_lyr_2_4,
			    dmac_47_16);
	dmac_mask = MLX5_ADDR_OF(fte_match_set_lyr_2_4, match_c_set_lyr_2_4,
				 dmac_47_16);

	if (is_multicast_ether_addr(dmac) &&
	    is_multicast_ether_addr(dmac_mask))
		return true;

	ipv4 = MLX5_ADDR_OF(fte_match_set_lyr_2_4, match_v_set_lyr_2_4,
			    dst_ipv4_dst_ipv6.ipv4_layout.ipv4);

	ipv4_mask = MLX5_ADDR_OF(fte_match_set_lyr_2_4, match_c_set_lyr_2_4,
				 dst_ipv4_dst_ipv6.ipv4_layout.ipv4);

	if (ipv4_is_multicast(*(__be32 *)(ipv4)) &&
	    ipv4_is_multicast(*(__be32 *)(ipv4_mask)))
		return true;

	return false;
}

struct mlx5_ib_flow_handler *
mlx5_ib_raw_fs_rule_add(struct mlx5_ib_dev *dev,
			struct mlx5_ib_flow_matcher *fs_matcher,
			void *cmd_in, int inlen, int dest_id,
			int dest_type)
{
	struct mlx5_flow_destination *dst;
	struct mlx5_ib_flow_prio *ft_prio;
	int priority = fs_matcher->priority;
	struct mlx5_ib_flow_handler *handler;
	bool mcast;
	int err;

	if (fs_matcher->flow_type != MLX5_IB_FLOW_TYPE_NORMAL)
		return ERR_PTR(-EOPNOTSUPP);

	if (fs_matcher->priority > MLX5_IB_FLOW_LAST_PRIO)
		return ERR_PTR(-ENOMEM);

	dst = kzalloc(sizeof(*dst), GFP_KERNEL);
	if (!dst)
		return ERR_PTR(-ENOMEM);

	mcast = raw_fs_is_multicast(fs_matcher, cmd_in);
	mutex_lock(&dev->flow_db->lock);

	ft_prio = _get_flow_table(dev, priority, mcast);
	if (IS_ERR(ft_prio)) {
		err = PTR_ERR(ft_prio);
		goto unlock;
	}

	if (dest_type == MLX5_FLOW_DESTINATION_TYPE_TIR) {
		dst->type = dest_type;
		dst->tir_num = dest_id;
	} else {
		dst->type = MLX5_FLOW_DESTINATION_TYPE_FLOW_TABLE_NUM;
		dst->ft_num = dest_id;
	}

	handler = _create_raw_flow_rule(dev, ft_prio, dst, fs_matcher, cmd_in,
					inlen);

	if (IS_ERR(handler)) {
		err = PTR_ERR(handler);
		goto destroy_ft;
	}

	mutex_unlock(&dev->flow_db->lock);
	atomic_inc(&fs_matcher->usecnt);
	handler->flow_matcher = fs_matcher;

	kfree(dst);

	return handler;

destroy_ft:
	put_flow_table(dev, ft_prio, false);
unlock:
	mutex_unlock(&dev->flow_db->lock);
	kfree(dst);

	return ERR_PTR(err);
}

static u32 mlx5_ib_flow_action_flags_to_accel_xfrm_flags(u32 mlx5_flags)
{
	u32 flags = 0;

	if (mlx5_flags & MLX5_IB_UAPI_FLOW_ACTION_FLAGS_REQUIRE_METADATA)
		flags |= MLX5_ACCEL_XFRM_FLAG_REQUIRE_METADATA;

	return flags;
}

#define MLX5_FLOW_ACTION_ESP_CREATE_LAST_SUPPORTED	MLX5_IB_UAPI_FLOW_ACTION_FLAGS_REQUIRE_METADATA
static struct ib_flow_action *
mlx5_ib_create_flow_action_esp(struct ib_device *device,
			       const struct ib_flow_action_attrs_esp *attr,
			       struct uverbs_attr_bundle *attrs)
{
	struct mlx5_ib_dev *mdev = to_mdev(device);
	struct ib_uverbs_flow_action_esp_keymat_aes_gcm *aes_gcm;
	struct mlx5_accel_esp_xfrm_attrs accel_attrs = {};
	struct mlx5_ib_flow_action *action;
	u64 action_flags;
	u64 flags;
	int err = 0;

	err = uverbs_get_flags64(
		&action_flags, attrs, MLX5_IB_ATTR_CREATE_FLOW_ACTION_FLAGS,
		((MLX5_FLOW_ACTION_ESP_CREATE_LAST_SUPPORTED << 1) - 1));
	if (err)
		return ERR_PTR(err);

	flags = mlx5_ib_flow_action_flags_to_accel_xfrm_flags(action_flags);

	/* We current only support a subset of the standard features. Only a
	 * keymat of type AES_GCM, with icv_len == 16, iv_algo == SEQ and esn
	 * (with overlap). Full offload mode isn't supported.
	 */
	if (!attr->keymat || attr->replay || attr->encap ||
	    attr->spi || attr->seq || attr->tfc_pad ||
	    attr->hard_limit_pkts ||
	    (attr->flags & ~(IB_FLOW_ACTION_ESP_FLAGS_ESN_TRIGGERED |
			     IB_UVERBS_FLOW_ACTION_ESP_FLAGS_ENCRYPT)))
		return ERR_PTR(-EOPNOTSUPP);

	if (attr->keymat->protocol !=
	    IB_UVERBS_FLOW_ACTION_ESP_KEYMAT_AES_GCM)
		return ERR_PTR(-EOPNOTSUPP);

	aes_gcm = &attr->keymat->keymat.aes_gcm;

	if (aes_gcm->icv_len != 16 ||
	    aes_gcm->iv_algo != IB_UVERBS_FLOW_ACTION_IV_ALGO_SEQ)
		return ERR_PTR(-EOPNOTSUPP);

	action = kmalloc(sizeof(*action), GFP_KERNEL);
	if (!action)
		return ERR_PTR(-ENOMEM);

	action->esp_aes_gcm.ib_flags = attr->flags;
	memcpy(&accel_attrs.keymat.aes_gcm.aes_key, &aes_gcm->aes_key,
	       sizeof(accel_attrs.keymat.aes_gcm.aes_key));
	accel_attrs.keymat.aes_gcm.key_len = aes_gcm->key_len * 8;
	memcpy(&accel_attrs.keymat.aes_gcm.salt, &aes_gcm->salt,
	       sizeof(accel_attrs.keymat.aes_gcm.salt));
	memcpy(&accel_attrs.keymat.aes_gcm.seq_iv, &aes_gcm->iv,
	       sizeof(accel_attrs.keymat.aes_gcm.seq_iv));
	accel_attrs.keymat.aes_gcm.icv_len = aes_gcm->icv_len * 8;
	accel_attrs.keymat.aes_gcm.iv_algo = MLX5_ACCEL_ESP_AES_GCM_IV_ALGO_SEQ;
	accel_attrs.keymat_type = MLX5_ACCEL_ESP_KEYMAT_AES_GCM;

	accel_attrs.esn = attr->esn;
	if (attr->flags & IB_FLOW_ACTION_ESP_FLAGS_ESN_TRIGGERED)
		accel_attrs.flags |= MLX5_ACCEL_ESP_FLAGS_ESN_TRIGGERED;
	if (attr->flags & IB_UVERBS_FLOW_ACTION_ESP_FLAGS_ESN_NEW_WINDOW)
		accel_attrs.flags |= MLX5_ACCEL_ESP_FLAGS_ESN_STATE_OVERLAP;

	if (attr->flags & IB_UVERBS_FLOW_ACTION_ESP_FLAGS_ENCRYPT)
		accel_attrs.action |= MLX5_ACCEL_ESP_ACTION_ENCRYPT;

	action->esp_aes_gcm.ctx =
		mlx5_accel_esp_create_xfrm(mdev->mdev, &accel_attrs, flags);
	if (IS_ERR(action->esp_aes_gcm.ctx)) {
		err = PTR_ERR(action->esp_aes_gcm.ctx);
		goto err_parse;
	}

	action->esp_aes_gcm.ib_flags = attr->flags;

	return &action->ib_action;

err_parse:
	kfree(action);
	return ERR_PTR(err);
}

static int
mlx5_ib_modify_flow_action_esp(struct ib_flow_action *action,
			       const struct ib_flow_action_attrs_esp *attr,
			       struct uverbs_attr_bundle *attrs)
{
	struct mlx5_ib_flow_action *maction = to_mflow_act(action);
	struct mlx5_accel_esp_xfrm_attrs accel_attrs;
	int err = 0;

	if (attr->keymat || attr->replay || attr->encap ||
	    attr->spi || attr->seq || attr->tfc_pad ||
	    attr->hard_limit_pkts ||
	    (attr->flags & ~(IB_FLOW_ACTION_ESP_FLAGS_ESN_TRIGGERED |
			     IB_FLOW_ACTION_ESP_FLAGS_MOD_ESP_ATTRS |
			     IB_UVERBS_FLOW_ACTION_ESP_FLAGS_ESN_NEW_WINDOW)))
		return -EOPNOTSUPP;

	/* Only the ESN value or the MLX5_ACCEL_ESP_FLAGS_ESN_STATE_OVERLAP can
	 * be modified.
	 */
	if (!(maction->esp_aes_gcm.ib_flags &
	      IB_FLOW_ACTION_ESP_FLAGS_ESN_TRIGGERED) &&
	    attr->flags & (IB_FLOW_ACTION_ESP_FLAGS_ESN_TRIGGERED |
			   IB_UVERBS_FLOW_ACTION_ESP_FLAGS_ESN_NEW_WINDOW))
		return -EINVAL;

	memcpy(&accel_attrs, &maction->esp_aes_gcm.ctx->attrs,
	       sizeof(accel_attrs));

	accel_attrs.esn = attr->esn;
	if (attr->flags & IB_UVERBS_FLOW_ACTION_ESP_FLAGS_ESN_NEW_WINDOW)
		accel_attrs.flags |= MLX5_ACCEL_ESP_FLAGS_ESN_STATE_OVERLAP;
	else
		accel_attrs.flags &= ~MLX5_ACCEL_ESP_FLAGS_ESN_STATE_OVERLAP;

	err = mlx5_accel_esp_modify_xfrm(maction->esp_aes_gcm.ctx,
					 &accel_attrs);
	if (err)
		return err;

	maction->esp_aes_gcm.ib_flags &=
		~IB_UVERBS_FLOW_ACTION_ESP_FLAGS_ESN_NEW_WINDOW;
	maction->esp_aes_gcm.ib_flags |=
		attr->flags & IB_UVERBS_FLOW_ACTION_ESP_FLAGS_ESN_NEW_WINDOW;

	return 0;
}

static int mlx5_ib_destroy_flow_action(struct ib_flow_action *action)
{
	struct mlx5_ib_flow_action *maction = to_mflow_act(action);

	switch (action->type) {
	case IB_FLOW_ACTION_ESP:
		/*
		 * We only support aes_gcm by now, so we implicitly know this is
		 * the underline crypto.
		 */
		mlx5_accel_esp_destroy_xfrm(maction->esp_aes_gcm.ctx);
		break;
	default:
		WARN_ON(true);
		break;
	}

	kfree(maction);
	return 0;
=======
	return mlx5_cmd_dealloc_pd(mdev->mdev, mpd->pdn, mpd->uid);
>>>>>>> upstream/android-13
}

static int mlx5_ib_mcg_attach(struct ib_qp *ibqp, union ib_gid *gid, u16 lid)
{
	struct mlx5_ib_dev *dev = to_mdev(ibqp->device);
	struct mlx5_ib_qp *mqp = to_mqp(ibqp);
	int err;
<<<<<<< HEAD

	if (mqp->flags & MLX5_IB_QP_UNDERLAY) {
=======
	u16 uid;

	uid = ibqp->pd ?
		to_mpd(ibqp->pd)->uid : 0;

	if (mqp->flags & IB_QP_CREATE_SOURCE_QPN) {
>>>>>>> upstream/android-13
		mlx5_ib_dbg(dev, "Attaching a multi cast group to underlay QP is not supported\n");
		return -EOPNOTSUPP;
	}

<<<<<<< HEAD
	err = mlx5_core_attach_mcg(dev->mdev, gid, ibqp->qp_num);
=======
	err = mlx5_cmd_attach_mcg(dev->mdev, gid, ibqp->qp_num, uid);
>>>>>>> upstream/android-13
	if (err)
		mlx5_ib_warn(dev, "failed attaching QPN 0x%x, MGID %pI6\n",
			     ibqp->qp_num, gid->raw);

	return err;
}

static int mlx5_ib_mcg_detach(struct ib_qp *ibqp, union ib_gid *gid, u16 lid)
{
	struct mlx5_ib_dev *dev = to_mdev(ibqp->device);
	int err;
<<<<<<< HEAD

	err = mlx5_core_detach_mcg(dev->mdev, gid, ibqp->qp_num);
=======
	u16 uid;

	uid = ibqp->pd ?
		to_mpd(ibqp->pd)->uid : 0;
	err = mlx5_cmd_detach_mcg(dev->mdev, gid, ibqp->qp_num, uid);
>>>>>>> upstream/android-13
	if (err)
		mlx5_ib_warn(dev, "failed detaching QPN 0x%x, MGID %pI6\n",
			     ibqp->qp_num, gid->raw);

	return err;
}

static int init_node_data(struct mlx5_ib_dev *dev)
{
	int err;

	err = mlx5_query_node_desc(dev, dev->ib_dev.node_desc);
	if (err)
		return err;

	dev->mdev->rev_id = dev->mdev->pdev->revision;

	return mlx5_query_node_guid(dev, &dev->ib_dev.node_guid);
}

<<<<<<< HEAD
static ssize_t show_fw_pages(struct device *device, struct device_attribute *attr,
			     char *buf)
{
	struct mlx5_ib_dev *dev =
		container_of(device, struct mlx5_ib_dev, ib_dev.dev);

	return sprintf(buf, "%d\n", dev->mdev->priv.fw_pages);
}

static ssize_t show_reg_pages(struct device *device,
			      struct device_attribute *attr, char *buf)
{
	struct mlx5_ib_dev *dev =
		container_of(device, struct mlx5_ib_dev, ib_dev.dev);

	return sprintf(buf, "%d\n", atomic_read(&dev->mdev->priv.reg_pages));
}

static ssize_t show_hca(struct device *device, struct device_attribute *attr,
			char *buf)
{
	struct mlx5_ib_dev *dev =
		container_of(device, struct mlx5_ib_dev, ib_dev.dev);
	return sprintf(buf, "MT%d\n", dev->mdev->pdev->device);
}

static ssize_t show_rev(struct device *device, struct device_attribute *attr,
			char *buf)
{
	struct mlx5_ib_dev *dev =
		container_of(device, struct mlx5_ib_dev, ib_dev.dev);
	return sprintf(buf, "%x\n", dev->mdev->rev_id);
}

static ssize_t show_board(struct device *device, struct device_attribute *attr,
			  char *buf)
{
	struct mlx5_ib_dev *dev =
		container_of(device, struct mlx5_ib_dev, ib_dev.dev);
	return sprintf(buf, "%.*s\n", MLX5_BOARD_ID_LEN,
		       dev->mdev->board_id);
}

static DEVICE_ATTR(hw_rev,   S_IRUGO, show_rev,    NULL);
static DEVICE_ATTR(hca_type, S_IRUGO, show_hca,    NULL);
static DEVICE_ATTR(board_id, S_IRUGO, show_board,  NULL);
static DEVICE_ATTR(fw_pages, S_IRUGO, show_fw_pages, NULL);
static DEVICE_ATTR(reg_pages, S_IRUGO, show_reg_pages, NULL);

static struct device_attribute *mlx5_class_attributes[] = {
	&dev_attr_hw_rev,
	&dev_attr_hca_type,
	&dev_attr_board_id,
	&dev_attr_fw_pages,
	&dev_attr_reg_pages,
=======
static ssize_t fw_pages_show(struct device *device,
			     struct device_attribute *attr, char *buf)
{
	struct mlx5_ib_dev *dev =
		rdma_device_to_drv_device(device, struct mlx5_ib_dev, ib_dev);

	return sysfs_emit(buf, "%d\n", dev->mdev->priv.fw_pages);
}
static DEVICE_ATTR_RO(fw_pages);

static ssize_t reg_pages_show(struct device *device,
			      struct device_attribute *attr, char *buf)
{
	struct mlx5_ib_dev *dev =
		rdma_device_to_drv_device(device, struct mlx5_ib_dev, ib_dev);

	return sysfs_emit(buf, "%d\n", atomic_read(&dev->mdev->priv.reg_pages));
}
static DEVICE_ATTR_RO(reg_pages);

static ssize_t hca_type_show(struct device *device,
			     struct device_attribute *attr, char *buf)
{
	struct mlx5_ib_dev *dev =
		rdma_device_to_drv_device(device, struct mlx5_ib_dev, ib_dev);

	return sysfs_emit(buf, "MT%d\n", dev->mdev->pdev->device);
}
static DEVICE_ATTR_RO(hca_type);

static ssize_t hw_rev_show(struct device *device,
			   struct device_attribute *attr, char *buf)
{
	struct mlx5_ib_dev *dev =
		rdma_device_to_drv_device(device, struct mlx5_ib_dev, ib_dev);

	return sysfs_emit(buf, "%x\n", dev->mdev->rev_id);
}
static DEVICE_ATTR_RO(hw_rev);

static ssize_t board_id_show(struct device *device,
			     struct device_attribute *attr, char *buf)
{
	struct mlx5_ib_dev *dev =
		rdma_device_to_drv_device(device, struct mlx5_ib_dev, ib_dev);

	return sysfs_emit(buf, "%.*s\n", MLX5_BOARD_ID_LEN,
			  dev->mdev->board_id);
}
static DEVICE_ATTR_RO(board_id);

static struct attribute *mlx5_class_attributes[] = {
	&dev_attr_hw_rev.attr,
	&dev_attr_hca_type.attr,
	&dev_attr_board_id.attr,
	&dev_attr_fw_pages.attr,
	&dev_attr_reg_pages.attr,
	NULL,
};

static const struct attribute_group mlx5_attr_group = {
	.attrs = mlx5_class_attributes,
>>>>>>> upstream/android-13
};

static void pkey_change_handler(struct work_struct *work)
{
	struct mlx5_ib_port_resources *ports =
		container_of(work, struct mlx5_ib_port_resources,
			     pkey_change_work);

<<<<<<< HEAD
	mutex_lock(&ports->devr->mutex);
	mlx5_ib_gsi_pkey_change(ports->gsi);
	mutex_unlock(&ports->devr->mutex);
=======
	if (!ports->gsi)
		/*
		 * We got this event before device was fully configured
		 * and MAD registration code wasn't called/finished yet.
		 */
		return;

	mlx5_ib_gsi_pkey_change(ports->gsi);
>>>>>>> upstream/android-13
}

static void mlx5_ib_handle_internal_error(struct mlx5_ib_dev *ibdev)
{
	struct mlx5_ib_qp *mqp;
	struct mlx5_ib_cq *send_mcq, *recv_mcq;
	struct mlx5_core_cq *mcq;
	struct list_head cq_armed_list;
	unsigned long flags_qp;
	unsigned long flags_cq;
	unsigned long flags;

	INIT_LIST_HEAD(&cq_armed_list);

	/* Go over qp list reside on that ibdev, sync with create/destroy qp.*/
	spin_lock_irqsave(&ibdev->reset_flow_resource_lock, flags);
	list_for_each_entry(mqp, &ibdev->qp_list, qps_list) {
		spin_lock_irqsave(&mqp->sq.lock, flags_qp);
		if (mqp->sq.tail != mqp->sq.head) {
			send_mcq = to_mcq(mqp->ibqp.send_cq);
			spin_lock_irqsave(&send_mcq->lock, flags_cq);
			if (send_mcq->mcq.comp &&
			    mqp->ibqp.send_cq->comp_handler) {
				if (!send_mcq->mcq.reset_notify_added) {
					send_mcq->mcq.reset_notify_added = 1;
					list_add_tail(&send_mcq->mcq.reset_notify,
						      &cq_armed_list);
				}
			}
			spin_unlock_irqrestore(&send_mcq->lock, flags_cq);
		}
		spin_unlock_irqrestore(&mqp->sq.lock, flags_qp);
		spin_lock_irqsave(&mqp->rq.lock, flags_qp);
		/* no handling is needed for SRQ */
		if (!mqp->ibqp.srq) {
			if (mqp->rq.tail != mqp->rq.head) {
				recv_mcq = to_mcq(mqp->ibqp.recv_cq);
				spin_lock_irqsave(&recv_mcq->lock, flags_cq);
				if (recv_mcq->mcq.comp &&
				    mqp->ibqp.recv_cq->comp_handler) {
					if (!recv_mcq->mcq.reset_notify_added) {
						recv_mcq->mcq.reset_notify_added = 1;
						list_add_tail(&recv_mcq->mcq.reset_notify,
							      &cq_armed_list);
					}
				}
				spin_unlock_irqrestore(&recv_mcq->lock,
						       flags_cq);
			}
		}
		spin_unlock_irqrestore(&mqp->rq.lock, flags_qp);
	}
	/*At that point all inflight post send were put to be executed as of we
	 * lock/unlock above locks Now need to arm all involved CQs.
	 */
	list_for_each_entry(mcq, &cq_armed_list, reset_notify) {
<<<<<<< HEAD
		mcq->comp(mcq);
=======
		mcq->comp(mcq, NULL);
>>>>>>> upstream/android-13
	}
	spin_unlock_irqrestore(&ibdev->reset_flow_resource_lock, flags);
}

static void delay_drop_handler(struct work_struct *work)
{
	int err;
	struct mlx5_ib_delay_drop *delay_drop =
		container_of(work, struct mlx5_ib_delay_drop,
			     delay_drop_work);

	atomic_inc(&delay_drop->events_cnt);

	mutex_lock(&delay_drop->lock);
<<<<<<< HEAD
	err = mlx5_core_set_delay_drop(delay_drop->dev->mdev,
				       delay_drop->timeout);
=======
	err = mlx5_core_set_delay_drop(delay_drop->dev, delay_drop->timeout);
>>>>>>> upstream/android-13
	if (err) {
		mlx5_ib_warn(delay_drop->dev, "Failed to set delay drop, timeout=%u\n",
			     delay_drop->timeout);
		delay_drop->activate = false;
	}
	mutex_unlock(&delay_drop->lock);
}

<<<<<<< HEAD
=======
static void handle_general_event(struct mlx5_ib_dev *ibdev, struct mlx5_eqe *eqe,
				 struct ib_event *ibev)
{
	u32 port = (eqe->data.port.port >> 4) & 0xf;

	switch (eqe->sub_type) {
	case MLX5_GENERAL_SUBTYPE_DELAY_DROP_TIMEOUT:
		if (mlx5_ib_port_link_layer(&ibdev->ib_dev, port) ==
					    IB_LINK_LAYER_ETHERNET)
			schedule_work(&ibdev->delay_drop.delay_drop_work);
		break;
	default: /* do nothing */
		return;
	}
}

static int handle_port_change(struct mlx5_ib_dev *ibdev, struct mlx5_eqe *eqe,
			      struct ib_event *ibev)
{
	u32 port = (eqe->data.port.port >> 4) & 0xf;

	ibev->element.port_num = port;

	switch (eqe->sub_type) {
	case MLX5_PORT_CHANGE_SUBTYPE_ACTIVE:
	case MLX5_PORT_CHANGE_SUBTYPE_DOWN:
	case MLX5_PORT_CHANGE_SUBTYPE_INITIALIZED:
		/* In RoCE, port up/down events are handled in
		 * mlx5_netdev_event().
		 */
		if (mlx5_ib_port_link_layer(&ibdev->ib_dev, port) ==
					    IB_LINK_LAYER_ETHERNET)
			return -EINVAL;

		ibev->event = (eqe->sub_type == MLX5_PORT_CHANGE_SUBTYPE_ACTIVE) ?
				IB_EVENT_PORT_ACTIVE : IB_EVENT_PORT_ERR;
		break;

	case MLX5_PORT_CHANGE_SUBTYPE_LID:
		ibev->event = IB_EVENT_LID_CHANGE;
		break;

	case MLX5_PORT_CHANGE_SUBTYPE_PKEY:
		ibev->event = IB_EVENT_PKEY_CHANGE;
		schedule_work(&ibdev->devr.ports[port - 1].pkey_change_work);
		break;

	case MLX5_PORT_CHANGE_SUBTYPE_GUID:
		ibev->event = IB_EVENT_GID_CHANGE;
		break;

	case MLX5_PORT_CHANGE_SUBTYPE_CLIENT_REREG:
		ibev->event = IB_EVENT_CLIENT_REREGISTER;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

>>>>>>> upstream/android-13
static void mlx5_ib_handle_event(struct work_struct *_work)
{
	struct mlx5_ib_event_work *work =
		container_of(_work, struct mlx5_ib_event_work, work);
	struct mlx5_ib_dev *ibdev;
	struct ib_event ibev;
	bool fatal = false;
<<<<<<< HEAD
	u8 port = (u8)work->param;

	if (mlx5_core_is_mp_slave(work->dev)) {
		ibdev = mlx5_ib_get_ibdev_from_mpi(work->context);
		if (!ibdev)
			goto out;
	} else {
		ibdev = work->context;
=======

	if (work->is_slave) {
		ibdev = mlx5_ib_get_ibdev_from_mpi(work->mpi);
		if (!ibdev)
			goto out;
	} else {
		ibdev = work->dev;
>>>>>>> upstream/android-13
	}

	switch (work->event) {
	case MLX5_DEV_EVENT_SYS_ERROR:
		ibev.event = IB_EVENT_DEVICE_FATAL;
		mlx5_ib_handle_internal_error(ibdev);
<<<<<<< HEAD
		fatal = true;
		break;

	case MLX5_DEV_EVENT_PORT_UP:
	case MLX5_DEV_EVENT_PORT_DOWN:
	case MLX5_DEV_EVENT_PORT_INITIALIZED:
		/* In RoCE, port up/down events are handled in
		 * mlx5_netdev_event().
		 */
		if (mlx5_ib_port_link_layer(&ibdev->ib_dev, port) ==
			IB_LINK_LAYER_ETHERNET)
			goto out;

		ibev.event = (work->event == MLX5_DEV_EVENT_PORT_UP) ?
			     IB_EVENT_PORT_ACTIVE : IB_EVENT_PORT_ERR;
		break;

	case MLX5_DEV_EVENT_LID_CHANGE:
		ibev.event = IB_EVENT_LID_CHANGE;
		break;

	case MLX5_DEV_EVENT_PKEY_CHANGE:
		ibev.event = IB_EVENT_PKEY_CHANGE;
		schedule_work(&ibdev->devr.ports[port - 1].pkey_change_work);
		break;

	case MLX5_DEV_EVENT_GUID_CHANGE:
		ibev.event = IB_EVENT_GID_CHANGE;
		break;

	case MLX5_DEV_EVENT_CLIENT_REREG:
		ibev.event = IB_EVENT_CLIENT_REREGISTER;
		break;
	case MLX5_DEV_EVENT_DELAY_DROP_TIMEOUT:
		schedule_work(&ibdev->delay_drop.delay_drop_work);
		goto out;
=======
		ibev.element.port_num  = (u8)(unsigned long)work->param;
		fatal = true;
		break;
	case MLX5_EVENT_TYPE_PORT_CHANGE:
		if (handle_port_change(ibdev, work->param, &ibev))
			goto out;
		break;
	case MLX5_EVENT_TYPE_GENERAL_EVENT:
		handle_general_event(ibdev, work->param, &ibev);
		fallthrough;
>>>>>>> upstream/android-13
	default:
		goto out;
	}

<<<<<<< HEAD
	ibev.device	      = &ibdev->ib_dev;
	ibev.element.port_num = port;

	if (!rdma_is_port_valid(&ibdev->ib_dev, port)) {
		mlx5_ib_warn(ibdev, "warning: event on port %d\n", port);
=======
	ibev.device = &ibdev->ib_dev;

	if (!rdma_is_port_valid(&ibdev->ib_dev, ibev.element.port_num)) {
		mlx5_ib_warn(ibdev, "warning: event on port %d\n",  ibev.element.port_num);
>>>>>>> upstream/android-13
		goto out;
	}

	if (ibdev->ib_active)
		ib_dispatch_event(&ibev);

	if (fatal)
		ibdev->ib_active = false;
out:
	kfree(work);
}

<<<<<<< HEAD
static void mlx5_ib_event(struct mlx5_core_dev *dev, void *context,
			  enum mlx5_dev_event event, unsigned long param)
=======
static int mlx5_ib_event(struct notifier_block *nb,
			 unsigned long event, void *param)
>>>>>>> upstream/android-13
{
	struct mlx5_ib_event_work *work;

	work = kmalloc(sizeof(*work), GFP_ATOMIC);
	if (!work)
<<<<<<< HEAD
		return;

	INIT_WORK(&work->work, mlx5_ib_handle_event);
	work->dev = dev;
	work->param = param;
	work->context = context;
	work->event = event;

	queue_work(mlx5_ib_event_wq, &work->work);
=======
		return NOTIFY_DONE;

	INIT_WORK(&work->work, mlx5_ib_handle_event);
	work->dev = container_of(nb, struct mlx5_ib_dev, mdev_events);
	work->is_slave = false;
	work->param = param;
	work->event = event;

	queue_work(mlx5_ib_event_wq, &work->work);

	return NOTIFY_OK;
}

static int mlx5_ib_event_slave_port(struct notifier_block *nb,
				    unsigned long event, void *param)
{
	struct mlx5_ib_event_work *work;

	work = kmalloc(sizeof(*work), GFP_ATOMIC);
	if (!work)
		return NOTIFY_DONE;

	INIT_WORK(&work->work, mlx5_ib_handle_event);
	work->mpi = container_of(nb, struct mlx5_ib_multiport_info, mdev_events);
	work->is_slave = true;
	work->param = param;
	work->event = event;
	queue_work(mlx5_ib_event_wq, &work->work);

	return NOTIFY_OK;
>>>>>>> upstream/android-13
}

static int set_has_smi_cap(struct mlx5_ib_dev *dev)
{
	struct mlx5_hca_vport_context vport_ctx;
	int err;
	int port;

<<<<<<< HEAD
	for (port = 1; port <= dev->num_ports; port++) {
		dev->mdev->port_caps[port - 1].has_smi = false;
=======
	for (port = 1; port <= ARRAY_SIZE(dev->port_caps); port++) {
		dev->port_caps[port - 1].has_smi = false;
>>>>>>> upstream/android-13
		if (MLX5_CAP_GEN(dev->mdev, port_type) ==
		    MLX5_CAP_PORT_TYPE_IB) {
			if (MLX5_CAP_GEN(dev->mdev, ib_virt)) {
				err = mlx5_query_hca_vport_context(dev->mdev, 0,
								   port, 0,
								   &vport_ctx);
				if (err) {
					mlx5_ib_err(dev, "query_hca_vport_context for port=%d failed %d\n",
						    port, err);
					return err;
				}
<<<<<<< HEAD
				dev->mdev->port_caps[port - 1].has_smi =
					vport_ctx.has_smi;
			} else {
				dev->mdev->port_caps[port - 1].has_smi = true;
=======
				dev->port_caps[port - 1].has_smi =
					vport_ctx.has_smi;
			} else {
				dev->port_caps[port - 1].has_smi = true;
>>>>>>> upstream/android-13
			}
		}
	}
	return 0;
}

static void get_ext_port_caps(struct mlx5_ib_dev *dev)
{
<<<<<<< HEAD
	int port;

	for (port = 1; port <= dev->num_ports; port++)
		mlx5_query_ext_port_caps(dev, port);
}

static int get_port_caps(struct mlx5_ib_dev *dev, u8 port)
{
	struct ib_device_attr *dprops = NULL;
	struct ib_port_attr *pprops = NULL;
	int err = -ENOMEM;
	struct ib_udata uhw = {.inlen = 0, .outlen = 0};

	pprops = kmalloc(sizeof(*pprops), GFP_KERNEL);
	if (!pprops)
		goto out;

	dprops = kmalloc(sizeof(*dprops), GFP_KERNEL);
	if (!dprops)
		goto out;

	err = set_has_smi_cap(dev);
	if (err)
		goto out;

	err = mlx5_ib_query_device(&dev->ib_dev, dprops, &uhw);
	if (err) {
		mlx5_ib_warn(dev, "query_device failed %d\n", err);
		goto out;
	}

	memset(pprops, 0, sizeof(*pprops));
	err = mlx5_ib_query_port(&dev->ib_dev, port, pprops);
	if (err) {
		mlx5_ib_warn(dev, "query_port %d failed %d\n",
			     port, err);
		goto out;
	}

	dev->mdev->port_caps[port - 1].pkey_table_len =
					dprops->max_pkeys;
	dev->mdev->port_caps[port - 1].gid_table_len =
					pprops->gid_tbl_len;
	mlx5_ib_dbg(dev, "port %d: pkey_table_len %d, gid_table_len %d\n",
		    port, dprops->max_pkeys, pprops->gid_tbl_len);

out:
	kfree(pprops);
	kfree(dprops);

	return err;
}

static void destroy_umrc_res(struct mlx5_ib_dev *dev)
{
	int err;

	err = mlx5_mr_cache_cleanup(dev);
	if (err)
		mlx5_ib_warn(dev, "mr cache cleanup failed\n");

	if (dev->umrc.qp)
		mlx5_ib_destroy_qp(dev->umrc.qp);
	if (dev->umrc.cq)
		ib_free_cq(dev->umrc.cq);
	if (dev->umrc.pd)
		ib_dealloc_pd(dev->umrc.pd);
}

enum {
	MAX_UMR_WR = 128,
};

static int create_umr_res(struct mlx5_ib_dev *dev)
{
	struct ib_qp_init_attr *init_attr = NULL;
	struct ib_qp_attr *attr = NULL;
	struct ib_pd *pd;
	struct ib_cq *cq;
	struct ib_qp *qp;
	int ret;

	attr = kzalloc(sizeof(*attr), GFP_KERNEL);
	init_attr = kzalloc(sizeof(*init_attr), GFP_KERNEL);
	if (!attr || !init_attr) {
		ret = -ENOMEM;
		goto error_0;
	}

	pd = ib_alloc_pd(&dev->ib_dev, 0);
	if (IS_ERR(pd)) {
		mlx5_ib_dbg(dev, "Couldn't create PD for sync UMR QP\n");
		ret = PTR_ERR(pd);
		goto error_0;
	}

	cq = ib_alloc_cq(&dev->ib_dev, NULL, 128, 0, IB_POLL_SOFTIRQ);
	if (IS_ERR(cq)) {
		mlx5_ib_dbg(dev, "Couldn't create CQ for sync UMR QP\n");
		ret = PTR_ERR(cq);
		goto error_2;
	}

	init_attr->send_cq = cq;
	init_attr->recv_cq = cq;
	init_attr->sq_sig_type = IB_SIGNAL_ALL_WR;
	init_attr->cap.max_send_wr = MAX_UMR_WR;
	init_attr->cap.max_send_sge = 1;
	init_attr->qp_type = MLX5_IB_QPT_REG_UMR;
	init_attr->port_num = 1;
	qp = mlx5_ib_create_qp(pd, init_attr, NULL);
	if (IS_ERR(qp)) {
		mlx5_ib_dbg(dev, "Couldn't create sync UMR QP\n");
		ret = PTR_ERR(qp);
		goto error_3;
	}
	qp->device     = &dev->ib_dev;
	qp->real_qp    = qp;
	qp->uobject    = NULL;
	qp->qp_type    = MLX5_IB_QPT_REG_UMR;
	qp->send_cq    = init_attr->send_cq;
	qp->recv_cq    = init_attr->recv_cq;

	attr->qp_state = IB_QPS_INIT;
	attr->port_num = 1;
	ret = mlx5_ib_modify_qp(qp, attr, IB_QP_STATE | IB_QP_PKEY_INDEX |
				IB_QP_PORT, NULL);
	if (ret) {
		mlx5_ib_dbg(dev, "Couldn't modify UMR QP\n");
		goto error_4;
	}

	memset(attr, 0, sizeof(*attr));
	attr->qp_state = IB_QPS_RTR;
	attr->path_mtu = IB_MTU_256;

	ret = mlx5_ib_modify_qp(qp, attr, IB_QP_STATE, NULL);
	if (ret) {
		mlx5_ib_dbg(dev, "Couldn't modify umr QP to rtr\n");
		goto error_4;
	}

	memset(attr, 0, sizeof(*attr));
	attr->qp_state = IB_QPS_RTS;
	ret = mlx5_ib_modify_qp(qp, attr, IB_QP_STATE, NULL);
	if (ret) {
		mlx5_ib_dbg(dev, "Couldn't modify umr QP to rts\n");
		goto error_4;
	}

	dev->umrc.qp = qp;
	dev->umrc.cq = cq;
	dev->umrc.pd = pd;

	sema_init(&dev->umrc.sem, MAX_UMR_WR);
	ret = mlx5_mr_cache_init(dev);
	if (ret) {
		mlx5_ib_warn(dev, "mr cache init failed %d\n", ret);
		goto error_4;
	}

	kfree(attr);
	kfree(init_attr);

	return 0;

error_4:
	mlx5_ib_destroy_qp(qp);
	dev->umrc.qp = NULL;

error_3:
	ib_free_cq(cq);
	dev->umrc.cq = NULL;

error_2:
	ib_dealloc_pd(pd);
	dev->umrc.pd = NULL;

error_0:
	kfree(attr);
	kfree(init_attr);
	return ret;
}

=======
	unsigned int port;

	rdma_for_each_port (&dev->ib_dev, port)
		mlx5_query_ext_port_caps(dev, port);
}

>>>>>>> upstream/android-13
static u8 mlx5_get_umr_fence(u8 umr_fence_cap)
{
	switch (umr_fence_cap) {
	case MLX5_CAP_UMR_FENCE_NONE:
		return MLX5_FENCE_MODE_NONE;
	case MLX5_CAP_UMR_FENCE_SMALL:
		return MLX5_FENCE_MODE_INITIATOR_SMALL;
	default:
		return MLX5_FENCE_MODE_STRONG_ORDERING;
	}
}

<<<<<<< HEAD
static int create_dev_resources(struct mlx5_ib_resources *devr)
{
	struct ib_srq_init_attr attr;
	struct mlx5_ib_dev *dev;
=======
static int mlx5_ib_dev_res_init(struct mlx5_ib_dev *dev)
{
	struct mlx5_ib_resources *devr = &dev->devr;
	struct ib_srq_init_attr attr;
	struct ib_device *ibdev;
>>>>>>> upstream/android-13
	struct ib_cq_init_attr cq_attr = {.cqe = 1};
	int port;
	int ret = 0;

<<<<<<< HEAD
	dev = container_of(devr, struct mlx5_ib_dev, devr);

	mutex_init(&devr->mutex);

	devr->p0 = mlx5_ib_alloc_pd(&dev->ib_dev, NULL, NULL);
	if (IS_ERR(devr->p0)) {
		ret = PTR_ERR(devr->p0);
		goto error0;
	}
	devr->p0->device  = &dev->ib_dev;
	devr->p0->uobject = NULL;
	atomic_set(&devr->p0->usecnt, 0);

	devr->c0 = mlx5_ib_create_cq(&dev->ib_dev, &cq_attr, NULL, NULL);
=======
	ibdev = &dev->ib_dev;

	if (!MLX5_CAP_GEN(dev->mdev, xrc))
		return -EOPNOTSUPP;

	devr->p0 = ib_alloc_pd(ibdev, 0);
	if (IS_ERR(devr->p0))
		return PTR_ERR(devr->p0);

	devr->c0 = ib_create_cq(ibdev, NULL, NULL, NULL, &cq_attr);
>>>>>>> upstream/android-13
	if (IS_ERR(devr->c0)) {
		ret = PTR_ERR(devr->c0);
		goto error1;
	}
<<<<<<< HEAD
	devr->c0->device        = &dev->ib_dev;
	devr->c0->uobject       = NULL;
	devr->c0->comp_handler  = NULL;
	devr->c0->event_handler = NULL;
	devr->c0->cq_context    = NULL;
	atomic_set(&devr->c0->usecnt, 0);

	devr->x0 = mlx5_ib_alloc_xrcd(&dev->ib_dev, NULL, NULL);
	if (IS_ERR(devr->x0)) {
		ret = PTR_ERR(devr->x0);
		goto error2;
	}
	devr->x0->device = &dev->ib_dev;
	devr->x0->inode = NULL;
	atomic_set(&devr->x0->usecnt, 0);
	mutex_init(&devr->x0->tgt_qp_mutex);
	INIT_LIST_HEAD(&devr->x0->tgt_qp_list);

	devr->x1 = mlx5_ib_alloc_xrcd(&dev->ib_dev, NULL, NULL);
	if (IS_ERR(devr->x1)) {
		ret = PTR_ERR(devr->x1);
		goto error3;
	}
	devr->x1->device = &dev->ib_dev;
	devr->x1->inode = NULL;
	atomic_set(&devr->x1->usecnt, 0);
	mutex_init(&devr->x1->tgt_qp_mutex);
	INIT_LIST_HEAD(&devr->x1->tgt_qp_list);
=======

	ret = mlx5_cmd_xrcd_alloc(dev->mdev, &devr->xrcdn0, 0);
	if (ret)
		goto error2;

	ret = mlx5_cmd_xrcd_alloc(dev->mdev, &devr->xrcdn1, 0);
	if (ret)
		goto error3;
>>>>>>> upstream/android-13

	memset(&attr, 0, sizeof(attr));
	attr.attr.max_sge = 1;
	attr.attr.max_wr = 1;
	attr.srq_type = IB_SRQT_XRC;
	attr.ext.cq = devr->c0;
<<<<<<< HEAD
	attr.ext.xrc.xrcd = devr->x0;

	devr->s0 = mlx5_ib_create_srq(devr->p0, &attr, NULL);
	if (IS_ERR(devr->s0)) {
		ret = PTR_ERR(devr->s0);
		goto error4;
	}
	devr->s0->device	= &dev->ib_dev;
	devr->s0->pd		= devr->p0;
	devr->s0->uobject       = NULL;
	devr->s0->event_handler = NULL;
	devr->s0->srq_context   = NULL;
	devr->s0->srq_type      = IB_SRQT_XRC;
	devr->s0->ext.xrc.xrcd	= devr->x0;
	devr->s0->ext.cq	= devr->c0;
	atomic_inc(&devr->s0->ext.xrc.xrcd->usecnt);
	atomic_inc(&devr->s0->ext.cq->usecnt);
	atomic_inc(&devr->p0->usecnt);
	atomic_set(&devr->s0->usecnt, 0);
=======

	devr->s0 = ib_create_srq(devr->p0, &attr);
	if (IS_ERR(devr->s0)) {
		ret = PTR_ERR(devr->s0);
		goto err_create;
	}
>>>>>>> upstream/android-13

	memset(&attr, 0, sizeof(attr));
	attr.attr.max_sge = 1;
	attr.attr.max_wr = 1;
	attr.srq_type = IB_SRQT_BASIC;
<<<<<<< HEAD
	devr->s1 = mlx5_ib_create_srq(devr->p0, &attr, NULL);
	if (IS_ERR(devr->s1)) {
		ret = PTR_ERR(devr->s1);
		goto error5;
	}
	devr->s1->device	= &dev->ib_dev;
	devr->s1->pd		= devr->p0;
	devr->s1->uobject       = NULL;
	devr->s1->event_handler = NULL;
	devr->s1->srq_context   = NULL;
	devr->s1->srq_type      = IB_SRQT_BASIC;
	devr->s1->ext.cq	= devr->c0;
	atomic_inc(&devr->p0->usecnt);
	atomic_set(&devr->s1->usecnt, 0);

	for (port = 0; port < ARRAY_SIZE(devr->ports); ++port) {
		INIT_WORK(&devr->ports[port].pkey_change_work,
			  pkey_change_handler);
		devr->ports[port].devr = devr;
	}

	return 0;

error5:
	mlx5_ib_destroy_srq(devr->s0);
error4:
	mlx5_ib_dealloc_xrcd(devr->x1);
error3:
	mlx5_ib_dealloc_xrcd(devr->x0);
error2:
	mlx5_ib_destroy_cq(devr->c0);
error1:
	mlx5_ib_dealloc_pd(devr->p0);
error0:
	return ret;
}

static void destroy_dev_resources(struct mlx5_ib_resources *devr)
{
	struct mlx5_ib_dev *dev =
		container_of(devr, struct mlx5_ib_dev, devr);
	int port;

	mlx5_ib_destroy_srq(devr->s1);
	mlx5_ib_destroy_srq(devr->s0);
	mlx5_ib_dealloc_xrcd(devr->x0);
	mlx5_ib_dealloc_xrcd(devr->x1);
	mlx5_ib_destroy_cq(devr->c0);
	mlx5_ib_dealloc_pd(devr->p0);

	/* Make sure no change P_Key work items are still executing */
	for (port = 0; port < dev->num_ports; ++port)
		cancel_work_sync(&devr->ports[port].pkey_change_work);
=======

	devr->s1 = ib_create_srq(devr->p0, &attr);
	if (IS_ERR(devr->s1)) {
		ret = PTR_ERR(devr->s1);
		goto error6;
	}

	for (port = 0; port < ARRAY_SIZE(devr->ports); ++port)
		INIT_WORK(&devr->ports[port].pkey_change_work,
			  pkey_change_handler);

	return 0;

error6:
	ib_destroy_srq(devr->s0);
err_create:
	mlx5_cmd_xrcd_dealloc(dev->mdev, devr->xrcdn1, 0);
error3:
	mlx5_cmd_xrcd_dealloc(dev->mdev, devr->xrcdn0, 0);
error2:
	ib_destroy_cq(devr->c0);
error1:
	ib_dealloc_pd(devr->p0);
	return ret;
}

static void mlx5_ib_dev_res_cleanup(struct mlx5_ib_dev *dev)
{
	struct mlx5_ib_resources *devr = &dev->devr;
	int port;

	/*
	 * Make sure no change P_Key work items are still executing.
	 *
	 * At this stage, the mlx5_ib_event should be unregistered
	 * and it ensures that no new works are added.
	 */
	for (port = 0; port < ARRAY_SIZE(devr->ports); ++port)
		cancel_work_sync(&devr->ports[port].pkey_change_work);

	ib_destroy_srq(devr->s1);
	ib_destroy_srq(devr->s0);
	mlx5_cmd_xrcd_dealloc(dev->mdev, devr->xrcdn1, 0);
	mlx5_cmd_xrcd_dealloc(dev->mdev, devr->xrcdn0, 0);
	ib_destroy_cq(devr->c0);
	ib_dealloc_pd(devr->p0);
>>>>>>> upstream/android-13
}

static u32 get_core_cap_flags(struct ib_device *ibdev,
			      struct mlx5_hca_vport_context *rep)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	enum rdma_link_layer ll = mlx5_ib_port_link_layer(ibdev, 1);
	u8 l3_type_cap = MLX5_CAP_ROCE(dev->mdev, l3_type);
	u8 roce_version_cap = MLX5_CAP_ROCE(dev->mdev, roce_version);
	bool raw_support = !mlx5_core_mp_enabled(dev->mdev);
	u32 ret = 0;

	if (rep->grh_required)
		ret |= RDMA_CORE_CAP_IB_GRH_REQUIRED;

	if (ll == IB_LINK_LAYER_INFINIBAND)
		return ret | RDMA_CORE_PORT_IBA_IB;

	if (raw_support)
		ret |= RDMA_CORE_PORT_RAW_PACKET;

	if (!(l3_type_cap & MLX5_ROCE_L3_TYPE_IPV4_CAP))
		return ret;

	if (!(l3_type_cap & MLX5_ROCE_L3_TYPE_IPV6_CAP))
		return ret;

	if (roce_version_cap & MLX5_ROCE_VERSION_1_CAP)
		ret |= RDMA_CORE_PORT_IBA_ROCE;

	if (roce_version_cap & MLX5_ROCE_VERSION_2_CAP)
		ret |= RDMA_CORE_PORT_IBA_ROCE_UDP_ENCAP;

	return ret;
}

<<<<<<< HEAD
static int mlx5_port_immutable(struct ib_device *ibdev, u8 port_num,
=======
static int mlx5_port_immutable(struct ib_device *ibdev, u32 port_num,
>>>>>>> upstream/android-13
			       struct ib_port_immutable *immutable)
{
	struct ib_port_attr attr;
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	enum rdma_link_layer ll = mlx5_ib_port_link_layer(ibdev, port_num);
	struct mlx5_hca_vport_context rep = {0};
	int err;

	err = ib_query_port(ibdev, port_num, &attr);
	if (err)
		return err;

	if (ll == IB_LINK_LAYER_INFINIBAND) {
		err = mlx5_query_hca_vport_context(dev->mdev, 0, port_num, 0,
						   &rep);
		if (err)
			return err;
	}

	immutable->pkey_tbl_len = attr.pkey_tbl_len;
	immutable->gid_tbl_len = attr.gid_tbl_len;
	immutable->core_cap_flags = get_core_cap_flags(ibdev, &rep);
<<<<<<< HEAD
	if ((ll == IB_LINK_LAYER_INFINIBAND) || MLX5_CAP_GEN(dev->mdev, roce))
		immutable->max_mad_size = IB_MGMT_MAD_SIZE;
=======
	immutable->max_mad_size = IB_MGMT_MAD_SIZE;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int mlx5_port_rep_immutable(struct ib_device *ibdev, u8 port_num,
=======
static int mlx5_port_rep_immutable(struct ib_device *ibdev, u32 port_num,
>>>>>>> upstream/android-13
				   struct ib_port_immutable *immutable)
{
	struct ib_port_attr attr;
	int err;

	immutable->core_cap_flags = RDMA_CORE_PORT_RAW_PACKET;

	err = ib_query_port(ibdev, port_num, &attr);
	if (err)
		return err;

	immutable->pkey_tbl_len = attr.pkey_tbl_len;
	immutable->gid_tbl_len = attr.gid_tbl_len;
	immutable->core_cap_flags = RDMA_CORE_PORT_RAW_PACKET;

	return 0;
}

static void get_dev_fw_str(struct ib_device *ibdev, char *str)
{
	struct mlx5_ib_dev *dev =
		container_of(ibdev, struct mlx5_ib_dev, ib_dev);
	snprintf(str, IB_FW_VERSION_NAME_MAX, "%d.%d.%04d",
		 fw_rev_maj(dev->mdev), fw_rev_min(dev->mdev),
		 fw_rev_sub(dev->mdev));
}

static int mlx5_eth_lag_init(struct mlx5_ib_dev *dev)
{
	struct mlx5_core_dev *mdev = dev->mdev;
	struct mlx5_flow_namespace *ns = mlx5_get_flow_namespace(mdev,
								 MLX5_FLOW_NAMESPACE_LAG);
	struct mlx5_flow_table *ft;
	int err;

	if (!ns || !mlx5_lag_is_active(mdev))
		return 0;

	err = mlx5_cmd_create_vport_lag(mdev);
	if (err)
		return err;

	ft = mlx5_create_lag_demux_flow_table(ns, 0, 0);
	if (IS_ERR(ft)) {
		err = PTR_ERR(ft);
		goto err_destroy_vport_lag;
	}

	dev->flow_db->lag_demux_ft = ft;
<<<<<<< HEAD
=======
	dev->lag_active = true;
>>>>>>> upstream/android-13
	return 0;

err_destroy_vport_lag:
	mlx5_cmd_destroy_vport_lag(mdev);
	return err;
}

static void mlx5_eth_lag_cleanup(struct mlx5_ib_dev *dev)
{
	struct mlx5_core_dev *mdev = dev->mdev;

<<<<<<< HEAD
	if (dev->flow_db->lag_demux_ft) {
=======
	if (dev->lag_active) {
		dev->lag_active = false;

>>>>>>> upstream/android-13
		mlx5_destroy_flow_table(dev->flow_db->lag_demux_ft);
		dev->flow_db->lag_demux_ft = NULL;

		mlx5_cmd_destroy_vport_lag(mdev);
	}
}

<<<<<<< HEAD
static int mlx5_add_netdev_notifier(struct mlx5_ib_dev *dev, u8 port_num)
{
	int err;

	dev->roce[port_num].nb.notifier_call = mlx5_netdev_event;
	err = register_netdevice_notifier(&dev->roce[port_num].nb);
	if (err) {
		dev->roce[port_num].nb.notifier_call = NULL;
=======
static int mlx5_add_netdev_notifier(struct mlx5_ib_dev *dev, u32 port_num)
{
	int err;

	dev->port[port_num].roce.nb.notifier_call = mlx5_netdev_event;
	err = register_netdevice_notifier(&dev->port[port_num].roce.nb);
	if (err) {
		dev->port[port_num].roce.nb.notifier_call = NULL;
>>>>>>> upstream/android-13
		return err;
	}

	return 0;
}

<<<<<<< HEAD
static void mlx5_remove_netdev_notifier(struct mlx5_ib_dev *dev, u8 port_num)
{
	if (dev->roce[port_num].nb.notifier_call) {
		unregister_netdevice_notifier(&dev->roce[port_num].nb);
		dev->roce[port_num].nb.notifier_call = NULL;
=======
static void mlx5_remove_netdev_notifier(struct mlx5_ib_dev *dev, u32 port_num)
{
	if (dev->port[port_num].roce.nb.notifier_call) {
		unregister_netdevice_notifier(&dev->port[port_num].roce.nb);
		dev->port[port_num].roce.nb.notifier_call = NULL;
>>>>>>> upstream/android-13
	}
}

static int mlx5_enable_eth(struct mlx5_ib_dev *dev)
{
	int err;

<<<<<<< HEAD
	if (MLX5_CAP_GEN(dev->mdev, roce)) {
=======
	if (!dev->is_rep && dev->profile != &raw_eth_profile) {
>>>>>>> upstream/android-13
		err = mlx5_nic_vport_enable_roce(dev->mdev);
		if (err)
			return err;
	}

	err = mlx5_eth_lag_init(dev);
	if (err)
		goto err_disable_roce;

	return 0;

err_disable_roce:
<<<<<<< HEAD
	if (MLX5_CAP_GEN(dev->mdev, roce))
=======
	if (!dev->is_rep && dev->profile != &raw_eth_profile)
>>>>>>> upstream/android-13
		mlx5_nic_vport_disable_roce(dev->mdev);

	return err;
}

static void mlx5_disable_eth(struct mlx5_ib_dev *dev)
{
	mlx5_eth_lag_cleanup(dev);
<<<<<<< HEAD
	if (MLX5_CAP_GEN(dev->mdev, roce))
		mlx5_nic_vport_disable_roce(dev->mdev);
}

struct mlx5_ib_counter {
	const char *name;
	size_t offset;
};

#define INIT_Q_COUNTER(_name)		\
	{ .name = #_name, .offset = MLX5_BYTE_OFF(query_q_counter_out, _name)}

static const struct mlx5_ib_counter basic_q_cnts[] = {
	INIT_Q_COUNTER(rx_write_requests),
	INIT_Q_COUNTER(rx_read_requests),
	INIT_Q_COUNTER(rx_atomic_requests),
	INIT_Q_COUNTER(out_of_buffer),
};

static const struct mlx5_ib_counter out_of_seq_q_cnts[] = {
	INIT_Q_COUNTER(out_of_sequence),
};

static const struct mlx5_ib_counter retrans_q_cnts[] = {
	INIT_Q_COUNTER(duplicate_request),
	INIT_Q_COUNTER(rnr_nak_retry_err),
	INIT_Q_COUNTER(packet_seq_err),
	INIT_Q_COUNTER(implied_nak_seq_err),
	INIT_Q_COUNTER(local_ack_timeout_err),
};

#define INIT_CONG_COUNTER(_name)		\
	{ .name = #_name, .offset =	\
		MLX5_BYTE_OFF(query_cong_statistics_out, _name ## _high)}

static const struct mlx5_ib_counter cong_cnts[] = {
	INIT_CONG_COUNTER(rp_cnp_ignored),
	INIT_CONG_COUNTER(rp_cnp_handled),
	INIT_CONG_COUNTER(np_ecn_marked_roce_packets),
	INIT_CONG_COUNTER(np_cnp_sent),
};

static const struct mlx5_ib_counter extended_err_cnts[] = {
	INIT_Q_COUNTER(resp_local_length_error),
	INIT_Q_COUNTER(resp_cqe_error),
	INIT_Q_COUNTER(req_cqe_error),
	INIT_Q_COUNTER(req_remote_invalid_request),
	INIT_Q_COUNTER(req_remote_access_errors),
	INIT_Q_COUNTER(resp_remote_access_errors),
	INIT_Q_COUNTER(resp_cqe_flush_error),
	INIT_Q_COUNTER(req_cqe_flush_error),
};

#define INIT_EXT_PPCNT_COUNTER(_name)		\
	{ .name = #_name, .offset =	\
	MLX5_BYTE_OFF(ppcnt_reg, \
		      counter_set.eth_extended_cntrs_grp_data_layout._name##_high)}

static const struct mlx5_ib_counter ext_ppcnt_cnts[] = {
	INIT_EXT_PPCNT_COUNTER(rx_icrc_encapsulated),
};

static void mlx5_ib_dealloc_counters(struct mlx5_ib_dev *dev)
{
	int i;

	for (i = 0; i < dev->num_ports; i++) {
		if (dev->port[i].cnts.set_id_valid)
			mlx5_core_dealloc_q_counter(dev->mdev,
						    dev->port[i].cnts.set_id);
		kfree(dev->port[i].cnts.names);
		kfree(dev->port[i].cnts.offsets);
	}
}

static int __mlx5_ib_alloc_counters(struct mlx5_ib_dev *dev,
				    struct mlx5_ib_counters *cnts)
{
	u32 num_counters;

	num_counters = ARRAY_SIZE(basic_q_cnts);

	if (MLX5_CAP_GEN(dev->mdev, out_of_seq_cnt))
		num_counters += ARRAY_SIZE(out_of_seq_q_cnts);

	if (MLX5_CAP_GEN(dev->mdev, retransmission_q_counters))
		num_counters += ARRAY_SIZE(retrans_q_cnts);

	if (MLX5_CAP_GEN(dev->mdev, enhanced_error_q_counters))
		num_counters += ARRAY_SIZE(extended_err_cnts);

	cnts->num_q_counters = num_counters;

	if (MLX5_CAP_GEN(dev->mdev, cc_query_allowed)) {
		cnts->num_cong_counters = ARRAY_SIZE(cong_cnts);
		num_counters += ARRAY_SIZE(cong_cnts);
	}
	if (MLX5_CAP_PCAM_FEATURE(dev->mdev, rx_icrc_encapsulated_counter)) {
		cnts->num_ext_ppcnt_counters = ARRAY_SIZE(ext_ppcnt_cnts);
		num_counters += ARRAY_SIZE(ext_ppcnt_cnts);
	}
	cnts->names = kcalloc(num_counters, sizeof(cnts->names), GFP_KERNEL);
	if (!cnts->names)
		return -ENOMEM;

	cnts->offsets = kcalloc(num_counters,
				sizeof(cnts->offsets), GFP_KERNEL);
	if (!cnts->offsets)
		goto err_names;

	return 0;

err_names:
	kfree(cnts->names);
	cnts->names = NULL;
	return -ENOMEM;
}

static void mlx5_ib_fill_counters(struct mlx5_ib_dev *dev,
				  const char **names,
				  size_t *offsets)
{
	int i;
	int j = 0;

	for (i = 0; i < ARRAY_SIZE(basic_q_cnts); i++, j++) {
		names[j] = basic_q_cnts[i].name;
		offsets[j] = basic_q_cnts[i].offset;
	}

	if (MLX5_CAP_GEN(dev->mdev, out_of_seq_cnt)) {
		for (i = 0; i < ARRAY_SIZE(out_of_seq_q_cnts); i++, j++) {
			names[j] = out_of_seq_q_cnts[i].name;
			offsets[j] = out_of_seq_q_cnts[i].offset;
		}
	}

	if (MLX5_CAP_GEN(dev->mdev, retransmission_q_counters)) {
		for (i = 0; i < ARRAY_SIZE(retrans_q_cnts); i++, j++) {
			names[j] = retrans_q_cnts[i].name;
			offsets[j] = retrans_q_cnts[i].offset;
		}
	}

	if (MLX5_CAP_GEN(dev->mdev, enhanced_error_q_counters)) {
		for (i = 0; i < ARRAY_SIZE(extended_err_cnts); i++, j++) {
			names[j] = extended_err_cnts[i].name;
			offsets[j] = extended_err_cnts[i].offset;
		}
	}

	if (MLX5_CAP_GEN(dev->mdev, cc_query_allowed)) {
		for (i = 0; i < ARRAY_SIZE(cong_cnts); i++, j++) {
			names[j] = cong_cnts[i].name;
			offsets[j] = cong_cnts[i].offset;
		}
	}

	if (MLX5_CAP_PCAM_FEATURE(dev->mdev, rx_icrc_encapsulated_counter)) {
		for (i = 0; i < ARRAY_SIZE(ext_ppcnt_cnts); i++, j++) {
			names[j] = ext_ppcnt_cnts[i].name;
			offsets[j] = ext_ppcnt_cnts[i].offset;
		}
	}
}

static int mlx5_ib_alloc_counters(struct mlx5_ib_dev *dev)
{
	int err = 0;
	int i;

	for (i = 0; i < dev->num_ports; i++) {
		err = __mlx5_ib_alloc_counters(dev, &dev->port[i].cnts);
		if (err)
			goto err_alloc;

		mlx5_ib_fill_counters(dev, dev->port[i].cnts.names,
				      dev->port[i].cnts.offsets);

		err = mlx5_core_alloc_q_counter(dev->mdev,
						&dev->port[i].cnts.set_id);
		if (err) {
			mlx5_ib_warn(dev,
				     "couldn't allocate queue counter for port %d, err %d\n",
				     i + 1, err);
			goto err_alloc;
		}
		dev->port[i].cnts.set_id_valid = true;
	}

	return 0;

err_alloc:
	mlx5_ib_dealloc_counters(dev);
	return err;
}

static struct rdma_hw_stats *mlx5_ib_alloc_hw_stats(struct ib_device *ibdev,
						    u8 port_num)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	struct mlx5_ib_port *port = &dev->port[port_num - 1];

	/* We support only per port stats */
	if (port_num == 0)
		return NULL;

	return rdma_alloc_hw_stats_struct(port->cnts.names,
					  port->cnts.num_q_counters +
					  port->cnts.num_cong_counters +
					  port->cnts.num_ext_ppcnt_counters,
					  RDMA_HW_STATS_DEFAULT_LIFESPAN);
}

static int mlx5_ib_query_q_counters(struct mlx5_core_dev *mdev,
				    struct mlx5_ib_port *port,
				    struct rdma_hw_stats *stats)
{
	int outlen = MLX5_ST_SZ_BYTES(query_q_counter_out);
	void *out;
	__be32 val;
	int ret, i;

	out = kvzalloc(outlen, GFP_KERNEL);
	if (!out)
		return -ENOMEM;

	ret = mlx5_core_query_q_counter(mdev,
					port->cnts.set_id, 0,
					out, outlen);
	if (ret)
		goto free;

	for (i = 0; i < port->cnts.num_q_counters; i++) {
		val = *(__be32 *)(out + port->cnts.offsets[i]);
		stats->value[i] = (u64)be32_to_cpu(val);
	}

free:
	kvfree(out);
	return ret;
}

static int mlx5_ib_query_ext_ppcnt_counters(struct mlx5_ib_dev *dev,
					  struct mlx5_ib_port *port,
					  struct rdma_hw_stats *stats)
{
	int offset = port->cnts.num_q_counters + port->cnts.num_cong_counters;
	int sz = MLX5_ST_SZ_BYTES(ppcnt_reg);
	int ret, i;
	void *out;

	out = kvzalloc(sz, GFP_KERNEL);
	if (!out)
		return -ENOMEM;

	ret = mlx5_cmd_query_ext_ppcnt_counters(dev->mdev, out);
	if (ret)
		goto free;

	for (i = 0; i < port->cnts.num_ext_ppcnt_counters; i++) {
		stats->value[i + offset] =
			be64_to_cpup((__be64 *)(out +
				    port->cnts.offsets[i + offset]));
	}

free:
	kvfree(out);
	return ret;
}

static int mlx5_ib_get_hw_stats(struct ib_device *ibdev,
				struct rdma_hw_stats *stats,
				u8 port_num, int index)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);
	struct mlx5_ib_port *port = &dev->port[port_num - 1];
	struct mlx5_core_dev *mdev;
	int ret, num_counters;
	u8 mdev_port_num;

	if (!stats)
		return -EINVAL;

	num_counters = port->cnts.num_q_counters +
		       port->cnts.num_cong_counters +
		       port->cnts.num_ext_ppcnt_counters;

	/* q_counters are per IB device, query the master mdev */
	ret = mlx5_ib_query_q_counters(dev->mdev, port, stats);
	if (ret)
		return ret;

	if (MLX5_CAP_PCAM_FEATURE(dev->mdev, rx_icrc_encapsulated_counter)) {
		ret =  mlx5_ib_query_ext_ppcnt_counters(dev, port, stats);
		if (ret)
			return ret;
	}

	if (MLX5_CAP_GEN(dev->mdev, cc_query_allowed)) {
		mdev = mlx5_ib_get_native_port_mdev(dev, port_num,
						    &mdev_port_num);
		if (!mdev) {
			/* If port is not affiliated yet, its in down state
			 * which doesn't have any counters yet, so it would be
			 * zero. So no need to read from the HCA.
			 */
			goto done;
		}
		ret = mlx5_lag_query_cong_counters(dev->mdev,
						   stats->value +
						   port->cnts.num_q_counters,
						   port->cnts.num_cong_counters,
						   port->cnts.offsets +
						   port->cnts.num_q_counters);

		mlx5_ib_put_native_port_mdev(dev, port_num);
		if (ret)
			return ret;
	}

done:
	return num_counters;
}

static struct net_device*
mlx5_ib_alloc_rdma_netdev(struct ib_device *hca,
			  u8 port_num,
			  enum rdma_netdev_t type,
			  const char *name,
			  unsigned char name_assign_type,
			  void (*setup)(struct net_device *))
{
	struct net_device *netdev;

	if (type != RDMA_NETDEV_IPOIB)
		return ERR_PTR(-EOPNOTSUPP);

	netdev = mlx5_rdma_netdev_alloc(to_mdev(hca)->mdev, hca,
					name, setup);
	return netdev;
}

static void delay_drop_debugfs_cleanup(struct mlx5_ib_dev *dev)
{
	if (!dev->delay_drop.dbg)
		return;
	debugfs_remove_recursive(dev->delay_drop.dbg->dir_debugfs);
	kfree(dev->delay_drop.dbg);
	dev->delay_drop.dbg = NULL;
}

static void cancel_delay_drop(struct mlx5_ib_dev *dev)
{
	if (!(dev->ib_dev.attrs.raw_packet_caps & IB_RAW_PACKET_CAP_DELAY_DROP))
		return;

	cancel_work_sync(&dev->delay_drop.delay_drop_work);
	delay_drop_debugfs_cleanup(dev);
=======
	if (!dev->is_rep && dev->profile != &raw_eth_profile)
		mlx5_nic_vport_disable_roce(dev->mdev);
}

static int mlx5_ib_rn_get_params(struct ib_device *device, u32 port_num,
				 enum rdma_netdev_t type,
				 struct rdma_netdev_alloc_params *params)
{
	if (type != RDMA_NETDEV_IPOIB)
		return -EOPNOTSUPP;

	return mlx5_rdma_rn_get_params(to_mdev(device)->mdev, device, params);
>>>>>>> upstream/android-13
}

static ssize_t delay_drop_timeout_read(struct file *filp, char __user *buf,
				       size_t count, loff_t *pos)
{
	struct mlx5_ib_delay_drop *delay_drop = filp->private_data;
	char lbuf[20];
	int len;

	len = snprintf(lbuf, sizeof(lbuf), "%u\n", delay_drop->timeout);
	return simple_read_from_buffer(buf, count, pos, lbuf, len);
}

static ssize_t delay_drop_timeout_write(struct file *filp, const char __user *buf,
					size_t count, loff_t *pos)
{
	struct mlx5_ib_delay_drop *delay_drop = filp->private_data;
	u32 timeout;
	u32 var;

	if (kstrtouint_from_user(buf, count, 0, &var))
		return -EFAULT;

	timeout = min_t(u32, roundup(var, 100), MLX5_MAX_DELAY_DROP_TIMEOUT_MS *
			1000);
	if (timeout != var)
		mlx5_ib_dbg(delay_drop->dev, "Round delay drop timeout to %u usec\n",
			    timeout);

	delay_drop->timeout = timeout;

	return count;
}

static const struct file_operations fops_delay_drop_timeout = {
	.owner	= THIS_MODULE,
	.open	= simple_open,
	.write	= delay_drop_timeout_write,
	.read	= delay_drop_timeout_read,
};

<<<<<<< HEAD
static int delay_drop_debugfs_init(struct mlx5_ib_dev *dev)
{
	struct mlx5_ib_dbg_delay_drop *dbg;

	if (!mlx5_debugfs_root)
		return 0;

	dbg = kzalloc(sizeof(*dbg), GFP_KERNEL);
	if (!dbg)
		return -ENOMEM;

	dev->delay_drop.dbg = dbg;

	dbg->dir_debugfs =
		debugfs_create_dir("delay_drop",
				   dev->mdev->priv.dbg_root);
	if (!dbg->dir_debugfs)
		goto out_debugfs;

	dbg->events_cnt_debugfs =
		debugfs_create_atomic_t("num_timeout_events", 0400,
					dbg->dir_debugfs,
					&dev->delay_drop.events_cnt);
	if (!dbg->events_cnt_debugfs)
		goto out_debugfs;

	dbg->rqs_cnt_debugfs =
		debugfs_create_atomic_t("num_rqs", 0400,
					dbg->dir_debugfs,
					&dev->delay_drop.rqs_cnt);
	if (!dbg->rqs_cnt_debugfs)
		goto out_debugfs;

	dbg->timeout_debugfs =
		debugfs_create_file("timeout", 0600,
				    dbg->dir_debugfs,
				    &dev->delay_drop,
				    &fops_delay_drop_timeout);
	if (!dbg->timeout_debugfs)
		goto out_debugfs;

	return 0;

out_debugfs:
	delay_drop_debugfs_cleanup(dev);
	return -ENOMEM;
}

static void init_delay_drop(struct mlx5_ib_dev *dev)
{
	if (!(dev->ib_dev.attrs.raw_packet_caps & IB_RAW_PACKET_CAP_DELAY_DROP))
		return;

	mutex_init(&dev->delay_drop.lock);
	dev->delay_drop.dev = dev;
	dev->delay_drop.activate = false;
	dev->delay_drop.timeout = MLX5_MAX_DELAY_DROP_TIMEOUT_MS * 1000;
	INIT_WORK(&dev->delay_drop.delay_drop_work, delay_drop_handler);
	atomic_set(&dev->delay_drop.rqs_cnt, 0);
	atomic_set(&dev->delay_drop.events_cnt, 0);

	if (delay_drop_debugfs_init(dev))
		mlx5_ib_warn(dev, "Failed to init delay drop debugfs\n");
}

static const struct cpumask *
mlx5_ib_get_vector_affinity(struct ib_device *ibdev, int comp_vector)
{
	struct mlx5_ib_dev *dev = to_mdev(ibdev);

	return mlx5_get_vector_affinity_hint(dev->mdev, comp_vector);
}

/* The mlx5_ib_multiport_mutex should be held when calling this function */
static void mlx5_ib_unbind_slave_port(struct mlx5_ib_dev *ibdev,
				      struct mlx5_ib_multiport_info *mpi)
{
	u8 port_num = mlx5_core_native_port_num(mpi->mdev) - 1;
=======
static void mlx5_ib_unbind_slave_port(struct mlx5_ib_dev *ibdev,
				      struct mlx5_ib_multiport_info *mpi)
{
	u32 port_num = mlx5_core_native_port_num(mpi->mdev) - 1;
>>>>>>> upstream/android-13
	struct mlx5_ib_port *port = &ibdev->port[port_num];
	int comps;
	int err;
	int i;

<<<<<<< HEAD
=======
	lockdep_assert_held(&mlx5_ib_multiport_mutex);

>>>>>>> upstream/android-13
	mlx5_ib_cleanup_cong_debugfs(ibdev, port_num);

	spin_lock(&port->mp.mpi_lock);
	if (!mpi->ibdev) {
		spin_unlock(&port->mp.mpi_lock);
		return;
	}
<<<<<<< HEAD
	mpi->ibdev = NULL;

	spin_unlock(&port->mp.mpi_lock);
=======

	mpi->ibdev = NULL;

	spin_unlock(&port->mp.mpi_lock);
	if (mpi->mdev_events.notifier_call)
		mlx5_notifier_unregister(mpi->mdev, &mpi->mdev_events);
	mpi->mdev_events.notifier_call = NULL;
>>>>>>> upstream/android-13
	mlx5_remove_netdev_notifier(ibdev, port_num);
	spin_lock(&port->mp.mpi_lock);

	comps = mpi->mdev_refcnt;
	if (comps) {
		mpi->unaffiliate = true;
		init_completion(&mpi->unref_comp);
		spin_unlock(&port->mp.mpi_lock);

		for (i = 0; i < comps; i++)
			wait_for_completion(&mpi->unref_comp);

		spin_lock(&port->mp.mpi_lock);
		mpi->unaffiliate = false;
	}

	port->mp.mpi = NULL;

<<<<<<< HEAD
	list_add_tail(&mpi->list, &mlx5_ib_unaffiliated_port_list);

=======
>>>>>>> upstream/android-13
	spin_unlock(&port->mp.mpi_lock);

	err = mlx5_nic_vport_unaffiliate_multiport(mpi->mdev);

<<<<<<< HEAD
	mlx5_ib_dbg(ibdev, "unaffiliated port %d\n", port_num + 1);
=======
	mlx5_ib_dbg(ibdev, "unaffiliated port %u\n", port_num + 1);
>>>>>>> upstream/android-13
	/* Log an error, still needed to cleanup the pointers and add
	 * it back to the list.
	 */
	if (err)
		mlx5_ib_err(ibdev, "Failed to unaffiliate port %u\n",
			    port_num + 1);

<<<<<<< HEAD
	ibdev->roce[port_num].last_port_state = IB_PORT_DOWN;
}

/* The mlx5_ib_multiport_mutex should be held when calling this function */
static bool mlx5_ib_bind_slave_port(struct mlx5_ib_dev *ibdev,
				    struct mlx5_ib_multiport_info *mpi)
{
	u8 port_num = mlx5_core_native_port_num(mpi->mdev) - 1;
	int err;

	spin_lock(&ibdev->port[port_num].mp.mpi_lock);
	if (ibdev->port[port_num].mp.mpi) {
		mlx5_ib_dbg(ibdev, "port %d already affiliated.\n",
=======
	ibdev->port[port_num].roce.last_port_state = IB_PORT_DOWN;
}

static bool mlx5_ib_bind_slave_port(struct mlx5_ib_dev *ibdev,
				    struct mlx5_ib_multiport_info *mpi)
{
	u32 port_num = mlx5_core_native_port_num(mpi->mdev) - 1;
	int err;

	lockdep_assert_held(&mlx5_ib_multiport_mutex);

	spin_lock(&ibdev->port[port_num].mp.mpi_lock);
	if (ibdev->port[port_num].mp.mpi) {
		mlx5_ib_dbg(ibdev, "port %u already affiliated.\n",
>>>>>>> upstream/android-13
			    port_num + 1);
		spin_unlock(&ibdev->port[port_num].mp.mpi_lock);
		return false;
	}

	ibdev->port[port_num].mp.mpi = mpi;
	mpi->ibdev = ibdev;
<<<<<<< HEAD
=======
	mpi->mdev_events.notifier_call = NULL;
>>>>>>> upstream/android-13
	spin_unlock(&ibdev->port[port_num].mp.mpi_lock);

	err = mlx5_nic_vport_affiliate_multiport(ibdev->mdev, mpi->mdev);
	if (err)
		goto unbind;

<<<<<<< HEAD
	err = get_port_caps(ibdev, mlx5_core_native_port_num(mpi->mdev));
	if (err)
		goto unbind;

=======
>>>>>>> upstream/android-13
	err = mlx5_add_netdev_notifier(ibdev, port_num);
	if (err) {
		mlx5_ib_err(ibdev, "failed adding netdev notifier for port %u\n",
			    port_num + 1);
		goto unbind;
	}

<<<<<<< HEAD
	err = mlx5_ib_init_cong_debugfs(ibdev, port_num);
	if (err)
		goto unbind;
=======
	mpi->mdev_events.notifier_call = mlx5_ib_event_slave_port;
	mlx5_notifier_register(mpi->mdev, &mpi->mdev_events);

	mlx5_ib_init_cong_debugfs(ibdev, port_num);
>>>>>>> upstream/android-13

	return true;

unbind:
	mlx5_ib_unbind_slave_port(ibdev, mpi);
	return false;
}

static int mlx5_ib_init_multiport_master(struct mlx5_ib_dev *dev)
{
<<<<<<< HEAD
	int port_num = mlx5_core_native_port_num(dev->mdev) - 1;
=======
	u32 port_num = mlx5_core_native_port_num(dev->mdev) - 1;
>>>>>>> upstream/android-13
	enum rdma_link_layer ll = mlx5_ib_port_link_layer(&dev->ib_dev,
							  port_num + 1);
	struct mlx5_ib_multiport_info *mpi;
	int err;
<<<<<<< HEAD
	int i;
=======
	u32 i;
>>>>>>> upstream/android-13

	if (!mlx5_core_is_mp_master(dev->mdev) || ll != IB_LINK_LAYER_ETHERNET)
		return 0;

	err = mlx5_query_nic_vport_system_image_guid(dev->mdev,
						     &dev->sys_image_guid);
	if (err)
		return err;

	err = mlx5_nic_vport_enable_roce(dev->mdev);
	if (err)
		return err;

	mutex_lock(&mlx5_ib_multiport_mutex);
	for (i = 0; i < dev->num_ports; i++) {
		bool bound = false;

		/* build a stub multiport info struct for the native port. */
		if (i == port_num) {
			mpi = kzalloc(sizeof(*mpi), GFP_KERNEL);
			if (!mpi) {
				mutex_unlock(&mlx5_ib_multiport_mutex);
				mlx5_nic_vport_disable_roce(dev->mdev);
				return -ENOMEM;
			}

			mpi->is_master = true;
			mpi->mdev = dev->mdev;
			mpi->sys_image_guid = dev->sys_image_guid;
			dev->port[i].mp.mpi = mpi;
			mpi->ibdev = dev;
			mpi = NULL;
			continue;
		}

		list_for_each_entry(mpi, &mlx5_ib_unaffiliated_port_list,
				    list) {
			if (dev->sys_image_guid == mpi->sys_image_guid &&
			    (mlx5_core_native_port_num(mpi->mdev) - 1) == i) {
				bound = mlx5_ib_bind_slave_port(dev, mpi);
			}

			if (bound) {
<<<<<<< HEAD
				dev_dbg(&mpi->mdev->pdev->dev, "removing port from unaffiliated list.\n");
=======
				dev_dbg(mpi->mdev->device,
					"removing port from unaffiliated list.\n");
>>>>>>> upstream/android-13
				mlx5_ib_dbg(dev, "port %d bound\n", i + 1);
				list_del(&mpi->list);
				break;
			}
		}
<<<<<<< HEAD
		if (!bound) {
			get_port_caps(dev, i + 1);
			mlx5_ib_dbg(dev, "no free port found for port %d\n",
				    i + 1);
		}
=======
		if (!bound)
			mlx5_ib_dbg(dev, "no free port found for port %d\n",
				    i + 1);
>>>>>>> upstream/android-13
	}

	list_add_tail(&dev->ib_dev_list, &mlx5_ib_dev_list);
	mutex_unlock(&mlx5_ib_multiport_mutex);
	return err;
}

static void mlx5_ib_cleanup_multiport_master(struct mlx5_ib_dev *dev)
{
<<<<<<< HEAD
	int port_num = mlx5_core_native_port_num(dev->mdev) - 1;
	enum rdma_link_layer ll = mlx5_ib_port_link_layer(&dev->ib_dev,
							  port_num + 1);
	int i;
=======
	u32 port_num = mlx5_core_native_port_num(dev->mdev) - 1;
	enum rdma_link_layer ll = mlx5_ib_port_link_layer(&dev->ib_dev,
							  port_num + 1);
	u32 i;
>>>>>>> upstream/android-13

	if (!mlx5_core_is_mp_master(dev->mdev) || ll != IB_LINK_LAYER_ETHERNET)
		return;

	mutex_lock(&mlx5_ib_multiport_mutex);
	for (i = 0; i < dev->num_ports; i++) {
		if (dev->port[i].mp.mpi) {
			/* Destroy the native port stub */
			if (i == port_num) {
				kfree(dev->port[i].mp.mpi);
				dev->port[i].mp.mpi = NULL;
			} else {
<<<<<<< HEAD
				mlx5_ib_dbg(dev, "unbinding port_num: %d\n", i + 1);
				mlx5_ib_unbind_slave_port(dev, dev->port[i].mp.mpi);
=======
				mlx5_ib_dbg(dev, "unbinding port_num: %u\n",
					    i + 1);
				list_add_tail(&dev->port[i].mp.mpi->list,
					      &mlx5_ib_unaffiliated_port_list);
				mlx5_ib_unbind_slave_port(dev,
							  dev->port[i].mp.mpi);
>>>>>>> upstream/android-13
			}
		}
	}

	mlx5_ib_dbg(dev, "removing from devlist\n");
	list_del(&dev->ib_dev_list);
	mutex_unlock(&mlx5_ib_multiport_mutex);

	mlx5_nic_vport_disable_roce(dev->mdev);
}

<<<<<<< HEAD
ADD_UVERBS_ATTRIBUTES_SIMPLE(
	mlx5_ib_dm,
	UVERBS_OBJECT_DM,
	UVERBS_METHOD_DM_ALLOC,
	UVERBS_ATTR_PTR_OUT(MLX5_IB_ATTR_ALLOC_DM_RESP_START_OFFSET,
			    UVERBS_ATTR_TYPE(u64),
			    UA_MANDATORY),
	UVERBS_ATTR_PTR_OUT(MLX5_IB_ATTR_ALLOC_DM_RESP_PAGE_INDEX,
			    UVERBS_ATTR_TYPE(u16),
			    UA_MANDATORY));

=======
static int mmap_obj_cleanup(struct ib_uobject *uobject,
			    enum rdma_remove_reason why,
			    struct uverbs_attr_bundle *attrs)
{
	struct mlx5_user_mmap_entry *obj = uobject->object;

	rdma_user_mmap_entry_remove(&obj->rdma_entry);
	return 0;
}

static int mlx5_rdma_user_mmap_entry_insert(struct mlx5_ib_ucontext *c,
					    struct mlx5_user_mmap_entry *entry,
					    size_t length)
{
	return rdma_user_mmap_entry_insert_range(
		&c->ibucontext, &entry->rdma_entry, length,
		(MLX5_IB_MMAP_OFFSET_START << 16),
		((MLX5_IB_MMAP_OFFSET_END << 16) + (1UL << 16) - 1));
}

static struct mlx5_user_mmap_entry *
alloc_var_entry(struct mlx5_ib_ucontext *c)
{
	struct mlx5_user_mmap_entry *entry;
	struct mlx5_var_table *var_table;
	u32 page_idx;
	int err;

	var_table = &to_mdev(c->ibucontext.device)->var_table;
	entry = kzalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return ERR_PTR(-ENOMEM);

	mutex_lock(&var_table->bitmap_lock);
	page_idx = find_first_zero_bit(var_table->bitmap,
				       var_table->num_var_hw_entries);
	if (page_idx >= var_table->num_var_hw_entries) {
		err = -ENOSPC;
		mutex_unlock(&var_table->bitmap_lock);
		goto end;
	}

	set_bit(page_idx, var_table->bitmap);
	mutex_unlock(&var_table->bitmap_lock);

	entry->address = var_table->hw_start_addr +
				(page_idx * var_table->stride_size);
	entry->page_idx = page_idx;
	entry->mmap_flag = MLX5_IB_MMAP_TYPE_VAR;

	err = mlx5_rdma_user_mmap_entry_insert(c, entry,
					       var_table->stride_size);
	if (err)
		goto err_insert;

	return entry;

err_insert:
	mutex_lock(&var_table->bitmap_lock);
	clear_bit(page_idx, var_table->bitmap);
	mutex_unlock(&var_table->bitmap_lock);
end:
	kfree(entry);
	return ERR_PTR(err);
}

static int UVERBS_HANDLER(MLX5_IB_METHOD_VAR_OBJ_ALLOC)(
	struct uverbs_attr_bundle *attrs)
{
	struct ib_uobject *uobj = uverbs_attr_get_uobject(
		attrs, MLX5_IB_ATTR_VAR_OBJ_ALLOC_HANDLE);
	struct mlx5_ib_ucontext *c;
	struct mlx5_user_mmap_entry *entry;
	u64 mmap_offset;
	u32 length;
	int err;

	c = to_mucontext(ib_uverbs_get_ucontext(attrs));
	if (IS_ERR(c))
		return PTR_ERR(c);

	entry = alloc_var_entry(c);
	if (IS_ERR(entry))
		return PTR_ERR(entry);

	mmap_offset = mlx5_entry_to_mmap_offset(entry);
	length = entry->rdma_entry.npages * PAGE_SIZE;
	uobj->object = entry;
	uverbs_finalize_uobj_create(attrs, MLX5_IB_ATTR_VAR_OBJ_ALLOC_HANDLE);

	err = uverbs_copy_to(attrs, MLX5_IB_ATTR_VAR_OBJ_ALLOC_MMAP_OFFSET,
			     &mmap_offset, sizeof(mmap_offset));
	if (err)
		return err;

	err = uverbs_copy_to(attrs, MLX5_IB_ATTR_VAR_OBJ_ALLOC_PAGE_ID,
			     &entry->page_idx, sizeof(entry->page_idx));
	if (err)
		return err;

	err = uverbs_copy_to(attrs, MLX5_IB_ATTR_VAR_OBJ_ALLOC_MMAP_LENGTH,
			     &length, sizeof(length));
	return err;
}

DECLARE_UVERBS_NAMED_METHOD(
	MLX5_IB_METHOD_VAR_OBJ_ALLOC,
	UVERBS_ATTR_IDR(MLX5_IB_ATTR_VAR_OBJ_ALLOC_HANDLE,
			MLX5_IB_OBJECT_VAR,
			UVERBS_ACCESS_NEW,
			UA_MANDATORY),
	UVERBS_ATTR_PTR_OUT(MLX5_IB_ATTR_VAR_OBJ_ALLOC_PAGE_ID,
			   UVERBS_ATTR_TYPE(u32),
			   UA_MANDATORY),
	UVERBS_ATTR_PTR_OUT(MLX5_IB_ATTR_VAR_OBJ_ALLOC_MMAP_LENGTH,
			   UVERBS_ATTR_TYPE(u32),
			   UA_MANDATORY),
	UVERBS_ATTR_PTR_OUT(MLX5_IB_ATTR_VAR_OBJ_ALLOC_MMAP_OFFSET,
			    UVERBS_ATTR_TYPE(u64),
			    UA_MANDATORY));

DECLARE_UVERBS_NAMED_METHOD_DESTROY(
	MLX5_IB_METHOD_VAR_OBJ_DESTROY,
	UVERBS_ATTR_IDR(MLX5_IB_ATTR_VAR_OBJ_DESTROY_HANDLE,
			MLX5_IB_OBJECT_VAR,
			UVERBS_ACCESS_DESTROY,
			UA_MANDATORY));

DECLARE_UVERBS_NAMED_OBJECT(MLX5_IB_OBJECT_VAR,
			    UVERBS_TYPE_ALLOC_IDR(mmap_obj_cleanup),
			    &UVERBS_METHOD(MLX5_IB_METHOD_VAR_OBJ_ALLOC),
			    &UVERBS_METHOD(MLX5_IB_METHOD_VAR_OBJ_DESTROY));

static bool var_is_supported(struct ib_device *device)
{
	struct mlx5_ib_dev *dev = to_mdev(device);

	return (MLX5_CAP_GEN_64(dev->mdev, general_obj_types) &
			MLX5_GENERAL_OBJ_TYPES_CAP_VIRTIO_NET_Q);
}

static struct mlx5_user_mmap_entry *
alloc_uar_entry(struct mlx5_ib_ucontext *c,
		enum mlx5_ib_uapi_uar_alloc_type alloc_type)
{
	struct mlx5_user_mmap_entry *entry;
	struct mlx5_ib_dev *dev;
	u32 uar_index;
	int err;

	entry = kzalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return ERR_PTR(-ENOMEM);

	dev = to_mdev(c->ibucontext.device);
	err = mlx5_cmd_alloc_uar(dev->mdev, &uar_index);
	if (err)
		goto end;

	entry->page_idx = uar_index;
	entry->address = uar_index2paddress(dev, uar_index);
	if (alloc_type == MLX5_IB_UAPI_UAR_ALLOC_TYPE_BF)
		entry->mmap_flag = MLX5_IB_MMAP_TYPE_UAR_WC;
	else
		entry->mmap_flag = MLX5_IB_MMAP_TYPE_UAR_NC;

	err = mlx5_rdma_user_mmap_entry_insert(c, entry, PAGE_SIZE);
	if (err)
		goto err_insert;

	return entry;

err_insert:
	mlx5_cmd_free_uar(dev->mdev, uar_index);
end:
	kfree(entry);
	return ERR_PTR(err);
}

static int UVERBS_HANDLER(MLX5_IB_METHOD_UAR_OBJ_ALLOC)(
	struct uverbs_attr_bundle *attrs)
{
	struct ib_uobject *uobj = uverbs_attr_get_uobject(
		attrs, MLX5_IB_ATTR_UAR_OBJ_ALLOC_HANDLE);
	enum mlx5_ib_uapi_uar_alloc_type alloc_type;
	struct mlx5_ib_ucontext *c;
	struct mlx5_user_mmap_entry *entry;
	u64 mmap_offset;
	u32 length;
	int err;

	c = to_mucontext(ib_uverbs_get_ucontext(attrs));
	if (IS_ERR(c))
		return PTR_ERR(c);

	err = uverbs_get_const(&alloc_type, attrs,
			       MLX5_IB_ATTR_UAR_OBJ_ALLOC_TYPE);
	if (err)
		return err;

	if (alloc_type != MLX5_IB_UAPI_UAR_ALLOC_TYPE_BF &&
	    alloc_type != MLX5_IB_UAPI_UAR_ALLOC_TYPE_NC)
		return -EOPNOTSUPP;

	if (!to_mdev(c->ibucontext.device)->wc_support &&
	    alloc_type == MLX5_IB_UAPI_UAR_ALLOC_TYPE_BF)
		return -EOPNOTSUPP;

	entry = alloc_uar_entry(c, alloc_type);
	if (IS_ERR(entry))
		return PTR_ERR(entry);

	mmap_offset = mlx5_entry_to_mmap_offset(entry);
	length = entry->rdma_entry.npages * PAGE_SIZE;
	uobj->object = entry;
	uverbs_finalize_uobj_create(attrs, MLX5_IB_ATTR_UAR_OBJ_ALLOC_HANDLE);

	err = uverbs_copy_to(attrs, MLX5_IB_ATTR_UAR_OBJ_ALLOC_MMAP_OFFSET,
			     &mmap_offset, sizeof(mmap_offset));
	if (err)
		return err;

	err = uverbs_copy_to(attrs, MLX5_IB_ATTR_UAR_OBJ_ALLOC_PAGE_ID,
			     &entry->page_idx, sizeof(entry->page_idx));
	if (err)
		return err;

	err = uverbs_copy_to(attrs, MLX5_IB_ATTR_UAR_OBJ_ALLOC_MMAP_LENGTH,
			     &length, sizeof(length));
	return err;
}

DECLARE_UVERBS_NAMED_METHOD(
	MLX5_IB_METHOD_UAR_OBJ_ALLOC,
	UVERBS_ATTR_IDR(MLX5_IB_ATTR_UAR_OBJ_ALLOC_HANDLE,
			MLX5_IB_OBJECT_UAR,
			UVERBS_ACCESS_NEW,
			UA_MANDATORY),
	UVERBS_ATTR_CONST_IN(MLX5_IB_ATTR_UAR_OBJ_ALLOC_TYPE,
			     enum mlx5_ib_uapi_uar_alloc_type,
			     UA_MANDATORY),
	UVERBS_ATTR_PTR_OUT(MLX5_IB_ATTR_UAR_OBJ_ALLOC_PAGE_ID,
			   UVERBS_ATTR_TYPE(u32),
			   UA_MANDATORY),
	UVERBS_ATTR_PTR_OUT(MLX5_IB_ATTR_UAR_OBJ_ALLOC_MMAP_LENGTH,
			   UVERBS_ATTR_TYPE(u32),
			   UA_MANDATORY),
	UVERBS_ATTR_PTR_OUT(MLX5_IB_ATTR_UAR_OBJ_ALLOC_MMAP_OFFSET,
			    UVERBS_ATTR_TYPE(u64),
			    UA_MANDATORY));

DECLARE_UVERBS_NAMED_METHOD_DESTROY(
	MLX5_IB_METHOD_UAR_OBJ_DESTROY,
	UVERBS_ATTR_IDR(MLX5_IB_ATTR_UAR_OBJ_DESTROY_HANDLE,
			MLX5_IB_OBJECT_UAR,
			UVERBS_ACCESS_DESTROY,
			UA_MANDATORY));

DECLARE_UVERBS_NAMED_OBJECT(MLX5_IB_OBJECT_UAR,
			    UVERBS_TYPE_ALLOC_IDR(mmap_obj_cleanup),
			    &UVERBS_METHOD(MLX5_IB_METHOD_UAR_OBJ_ALLOC),
			    &UVERBS_METHOD(MLX5_IB_METHOD_UAR_OBJ_DESTROY));

>>>>>>> upstream/android-13
ADD_UVERBS_ATTRIBUTES_SIMPLE(
	mlx5_ib_flow_action,
	UVERBS_OBJECT_FLOW_ACTION,
	UVERBS_METHOD_FLOW_ACTION_ESP_CREATE,
	UVERBS_ATTR_FLAGS_IN(MLX5_IB_ATTR_CREATE_FLOW_ACTION_FLAGS,
			     enum mlx5_ib_uapi_flow_action_flags));

<<<<<<< HEAD
static int populate_specs_root(struct mlx5_ib_dev *dev)
{
	const struct uverbs_object_tree_def **trees = dev->driver_trees;
	size_t num_trees = 0;

	if (mlx5_accel_ipsec_device_caps(dev->mdev) &
	    MLX5_ACCEL_IPSEC_CAP_DEVICE)
		trees[num_trees++] = &mlx5_ib_flow_action;

	if (MLX5_CAP_DEV_MEM(dev->mdev, memic))
		trees[num_trees++] = &mlx5_ib_dm;

	if (MLX5_CAP_GEN_64(dev->mdev, general_obj_types) &
	    MLX5_GENERAL_OBJ_TYPES_CAP_UCTX)
		trees[num_trees++] = mlx5_ib_get_devx_tree();

	num_trees += mlx5_ib_get_flow_trees(trees + num_trees);

	WARN_ON(num_trees >= ARRAY_SIZE(dev->driver_trees));
	trees[num_trees] = NULL;
	dev->ib_dev.driver_specs = trees;

	return 0;
}

static int mlx5_ib_read_counters(struct ib_counters *counters,
				 struct ib_counters_read_attr *read_attr,
				 struct uverbs_attr_bundle *attrs)
{
	struct mlx5_ib_mcounters *mcounters = to_mcounters(counters);
	struct mlx5_read_counters_attr mread_attr = {};
	struct mlx5_ib_flow_counters_desc *desc;
	int ret, i;

	mutex_lock(&mcounters->mcntrs_mutex);
	if (mcounters->cntrs_max_index > read_attr->ncounters) {
		ret = -EINVAL;
		goto err_bound;
	}

	mread_attr.out = kcalloc(mcounters->counters_num, sizeof(u64),
				 GFP_KERNEL);
	if (!mread_attr.out) {
		ret = -ENOMEM;
		goto err_bound;
	}

	mread_attr.hw_cntrs_hndl = mcounters->hw_cntrs_hndl;
	mread_attr.flags = read_attr->flags;
	ret = mcounters->read_counters(counters->device, &mread_attr);
	if (ret)
		goto err_read;

	/* do the pass over the counters data array to assign according to the
	 * descriptions and indexing pairs
	 */
	desc = mcounters->counters_data;
	for (i = 0; i < mcounters->ncounters; i++)
		read_attr->counters_buff[desc[i].index] += mread_attr.out[desc[i].description];

err_read:
	kfree(mread_attr.out);
err_bound:
	mutex_unlock(&mcounters->mcntrs_mutex);
	return ret;
}

static int mlx5_ib_destroy_counters(struct ib_counters *counters)
{
	struct mlx5_ib_mcounters *mcounters = to_mcounters(counters);

	counters_clear_description(counters);
	if (mcounters->hw_cntrs_hndl)
		mlx5_fc_destroy(to_mdev(counters->device)->mdev,
				mcounters->hw_cntrs_hndl);

	kfree(mcounters);

	return 0;
}

static struct ib_counters *mlx5_ib_create_counters(struct ib_device *device,
						   struct uverbs_attr_bundle *attrs)
{
	struct mlx5_ib_mcounters *mcounters;

	mcounters = kzalloc(sizeof(*mcounters), GFP_KERNEL);
	if (!mcounters)
		return ERR_PTR(-ENOMEM);

	mutex_init(&mcounters->mcntrs_mutex);

	return &mcounters->ibcntrs;
}

void mlx5_ib_stage_init_cleanup(struct mlx5_ib_dev *dev)
{
	mlx5_ib_cleanup_multiport_master(dev);
#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
	cleanup_srcu_struct(&dev->mr_srcu);
#endif
	kfree(dev->port);
}

int mlx5_ib_stage_init_init(struct mlx5_ib_dev *dev)
{
	struct mlx5_core_dev *mdev = dev->mdev;
	const char *name;
	int err;
	int i;

	dev->port = kcalloc(dev->num_ports, sizeof(*dev->port),
			    GFP_KERNEL);
	if (!dev->port)
		return -ENOMEM;

	for (i = 0; i < dev->num_ports; i++) {
		spin_lock_init(&dev->port[i].mp.mpi_lock);
		rwlock_init(&dev->roce[i].netdev_lock);
=======
ADD_UVERBS_ATTRIBUTES_SIMPLE(
	mlx5_ib_query_context,
	UVERBS_OBJECT_DEVICE,
	UVERBS_METHOD_QUERY_CONTEXT,
	UVERBS_ATTR_PTR_OUT(
		MLX5_IB_ATTR_QUERY_CONTEXT_RESP_UCTX,
		UVERBS_ATTR_STRUCT(struct mlx5_ib_alloc_ucontext_resp,
				   dump_fill_mkey),
		UA_MANDATORY));

static const struct uapi_definition mlx5_ib_defs[] = {
	UAPI_DEF_CHAIN(mlx5_ib_devx_defs),
	UAPI_DEF_CHAIN(mlx5_ib_flow_defs),
	UAPI_DEF_CHAIN(mlx5_ib_qos_defs),
	UAPI_DEF_CHAIN(mlx5_ib_std_types_defs),
	UAPI_DEF_CHAIN(mlx5_ib_dm_defs),

	UAPI_DEF_CHAIN_OBJ_TREE(UVERBS_OBJECT_FLOW_ACTION,
				&mlx5_ib_flow_action),
	UAPI_DEF_CHAIN_OBJ_TREE(UVERBS_OBJECT_DEVICE, &mlx5_ib_query_context),
	UAPI_DEF_CHAIN_OBJ_TREE_NAMED(MLX5_IB_OBJECT_VAR,
				UAPI_DEF_IS_OBJ_SUPPORTED(var_is_supported)),
	UAPI_DEF_CHAIN_OBJ_TREE_NAMED(MLX5_IB_OBJECT_UAR),
	{}
};

static void mlx5_ib_stage_init_cleanup(struct mlx5_ib_dev *dev)
{
	mlx5_ib_cleanup_multiport_master(dev);
	WARN_ON(!xa_empty(&dev->odp_mkeys));
	mutex_destroy(&dev->cap_mask_mutex);
	WARN_ON(!xa_empty(&dev->sig_mrs));
	WARN_ON(!bitmap_empty(dev->dm.memic_alloc_pages, MLX5_MAX_MEMIC_PAGES));
}

static int mlx5_ib_stage_init_init(struct mlx5_ib_dev *dev)
{
	struct mlx5_core_dev *mdev = dev->mdev;
	int err;
	int i;

	dev->ib_dev.node_type = RDMA_NODE_IB_CA;
	dev->ib_dev.local_dma_lkey = 0 /* not supported for now */;
	dev->ib_dev.phys_port_cnt = dev->num_ports;
	dev->ib_dev.dev.parent = mdev->device;
	dev->ib_dev.lag_flags = RDMA_LAG_FLAGS_HASH_ALL_SLAVES;

	for (i = 0; i < dev->num_ports; i++) {
		spin_lock_init(&dev->port[i].mp.mpi_lock);
		rwlock_init(&dev->port[i].roce.netdev_lock);
		dev->port[i].roce.dev = dev;
		dev->port[i].roce.native_port_num = i + 1;
		dev->port[i].roce.last_port_state = IB_PORT_DOWN;
>>>>>>> upstream/android-13
	}

	err = mlx5_ib_init_multiport_master(dev);
	if (err)
<<<<<<< HEAD
		goto err_free_port;

	if (!mlx5_core_mp_enabled(mdev)) {
		for (i = 1; i <= dev->num_ports; i++) {
			err = get_port_caps(dev, i);
			if (err)
				break;
		}
	} else {
		err = get_port_caps(dev, mlx5_core_native_port_num(mdev));
	}
=======
		return err;

	err = set_has_smi_cap(dev);
	if (err)
		goto err_mp;

	err = mlx5_query_max_pkeys(&dev->ib_dev, &dev->pkey_table_len);
>>>>>>> upstream/android-13
	if (err)
		goto err_mp;

	if (mlx5_use_mad_ifc(dev))
		get_ext_port_caps(dev);

<<<<<<< HEAD
	if (!mlx5_lag_is_active(mdev))
		name = "mlx5_%d";
	else
		name = "mlx5_bond_%d";

	strlcpy(dev->ib_dev.name, name, IB_DEVICE_NAME_MAX);
	dev->ib_dev.owner		= THIS_MODULE;
	dev->ib_dev.node_type		= RDMA_NODE_IB_CA;
	dev->ib_dev.local_dma_lkey	= 0 /* not supported for now */;
	dev->ib_dev.phys_port_cnt	= dev->num_ports;
	dev->ib_dev.num_comp_vectors    =
		dev->mdev->priv.eq_table.num_comp_vectors;
	dev->ib_dev.dev.parent		= &mdev->pdev->dev;
=======
	dev->ib_dev.num_comp_vectors    = mlx5_comp_vectors_count(mdev);
>>>>>>> upstream/android-13

	mutex_init(&dev->cap_mask_mutex);
	INIT_LIST_HEAD(&dev->qp_list);
	spin_lock_init(&dev->reset_flow_resource_lock);
<<<<<<< HEAD

	spin_lock_init(&dev->memic.memic_lock);
	dev->memic.dev = mdev;

#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
	err = init_srcu_struct(&dev->mr_srcu);
	if (err)
		goto err_free_port;
#endif

	return 0;
err_mp:
	mlx5_ib_cleanup_multiport_master(dev);

err_free_port:
	kfree(dev->port);

	return -ENOMEM;
}

static int mlx5_ib_stage_flow_db_init(struct mlx5_ib_dev *dev)
{
	dev->flow_db = kzalloc(sizeof(*dev->flow_db), GFP_KERNEL);

	if (!dev->flow_db)
		return -ENOMEM;

	mutex_init(&dev->flow_db->lock);

	return 0;
}

int mlx5_ib_stage_rep_flow_db_init(struct mlx5_ib_dev *dev)
{
	struct mlx5_ib_dev *nic_dev;

	nic_dev = mlx5_ib_get_uplink_ibdev(dev->mdev->priv.eswitch);

	if (!nic_dev)
		return -EINVAL;

	dev->flow_db = nic_dev->flow_db;

	return 0;
}

static void mlx5_ib_stage_flow_db_cleanup(struct mlx5_ib_dev *dev)
{
	kfree(dev->flow_db);
}

int mlx5_ib_stage_caps_init(struct mlx5_ib_dev *dev)
=======
	xa_init(&dev->odp_mkeys);
	xa_init(&dev->sig_mrs);
	atomic_set(&dev->mkey_var, 0);

	spin_lock_init(&dev->dm.lock);
	dev->dm.dev = mdev;
	return 0;

err_mp:
	mlx5_ib_cleanup_multiport_master(dev);
	return err;
}

static int mlx5_ib_enable_driver(struct ib_device *dev)
{
	struct mlx5_ib_dev *mdev = to_mdev(dev);
	int ret;

	ret = mlx5_ib_test_wc(mdev);
	mlx5_ib_dbg(mdev, "Write-Combining %s",
		    mdev->wc_support ? "supported" : "not supported");

	return ret;
}

static const struct ib_device_ops mlx5_ib_dev_ops = {
	.owner = THIS_MODULE,
	.driver_id = RDMA_DRIVER_MLX5,
	.uverbs_abi_ver	= MLX5_IB_UVERBS_ABI_VERSION,

	.add_gid = mlx5_ib_add_gid,
	.alloc_mr = mlx5_ib_alloc_mr,
	.alloc_mr_integrity = mlx5_ib_alloc_mr_integrity,
	.alloc_pd = mlx5_ib_alloc_pd,
	.alloc_ucontext = mlx5_ib_alloc_ucontext,
	.attach_mcast = mlx5_ib_mcg_attach,
	.check_mr_status = mlx5_ib_check_mr_status,
	.create_ah = mlx5_ib_create_ah,
	.create_cq = mlx5_ib_create_cq,
	.create_qp = mlx5_ib_create_qp,
	.create_srq = mlx5_ib_create_srq,
	.create_user_ah = mlx5_ib_create_ah,
	.dealloc_pd = mlx5_ib_dealloc_pd,
	.dealloc_ucontext = mlx5_ib_dealloc_ucontext,
	.del_gid = mlx5_ib_del_gid,
	.dereg_mr = mlx5_ib_dereg_mr,
	.destroy_ah = mlx5_ib_destroy_ah,
	.destroy_cq = mlx5_ib_destroy_cq,
	.destroy_qp = mlx5_ib_destroy_qp,
	.destroy_srq = mlx5_ib_destroy_srq,
	.detach_mcast = mlx5_ib_mcg_detach,
	.disassociate_ucontext = mlx5_ib_disassociate_ucontext,
	.drain_rq = mlx5_ib_drain_rq,
	.drain_sq = mlx5_ib_drain_sq,
	.device_group = &mlx5_attr_group,
	.enable_driver = mlx5_ib_enable_driver,
	.get_dev_fw_str = get_dev_fw_str,
	.get_dma_mr = mlx5_ib_get_dma_mr,
	.get_link_layer = mlx5_ib_port_link_layer,
	.map_mr_sg = mlx5_ib_map_mr_sg,
	.map_mr_sg_pi = mlx5_ib_map_mr_sg_pi,
	.mmap = mlx5_ib_mmap,
	.mmap_free = mlx5_ib_mmap_free,
	.modify_cq = mlx5_ib_modify_cq,
	.modify_device = mlx5_ib_modify_device,
	.modify_port = mlx5_ib_modify_port,
	.modify_qp = mlx5_ib_modify_qp,
	.modify_srq = mlx5_ib_modify_srq,
	.poll_cq = mlx5_ib_poll_cq,
	.post_recv = mlx5_ib_post_recv_nodrain,
	.post_send = mlx5_ib_post_send_nodrain,
	.post_srq_recv = mlx5_ib_post_srq_recv,
	.process_mad = mlx5_ib_process_mad,
	.query_ah = mlx5_ib_query_ah,
	.query_device = mlx5_ib_query_device,
	.query_gid = mlx5_ib_query_gid,
	.query_pkey = mlx5_ib_query_pkey,
	.query_qp = mlx5_ib_query_qp,
	.query_srq = mlx5_ib_query_srq,
	.query_ucontext = mlx5_ib_query_ucontext,
	.reg_user_mr = mlx5_ib_reg_user_mr,
	.reg_user_mr_dmabuf = mlx5_ib_reg_user_mr_dmabuf,
	.req_notify_cq = mlx5_ib_arm_cq,
	.rereg_user_mr = mlx5_ib_rereg_user_mr,
	.resize_cq = mlx5_ib_resize_cq,

	INIT_RDMA_OBJ_SIZE(ib_ah, mlx5_ib_ah, ibah),
	INIT_RDMA_OBJ_SIZE(ib_counters, mlx5_ib_mcounters, ibcntrs),
	INIT_RDMA_OBJ_SIZE(ib_cq, mlx5_ib_cq, ibcq),
	INIT_RDMA_OBJ_SIZE(ib_pd, mlx5_ib_pd, ibpd),
	INIT_RDMA_OBJ_SIZE(ib_qp, mlx5_ib_qp, ibqp),
	INIT_RDMA_OBJ_SIZE(ib_srq, mlx5_ib_srq, ibsrq),
	INIT_RDMA_OBJ_SIZE(ib_ucontext, mlx5_ib_ucontext, ibucontext),
};

static const struct ib_device_ops mlx5_ib_dev_ipoib_enhanced_ops = {
	.rdma_netdev_get_params = mlx5_ib_rn_get_params,
};

static const struct ib_device_ops mlx5_ib_dev_sriov_ops = {
	.get_vf_config = mlx5_ib_get_vf_config,
	.get_vf_guid = mlx5_ib_get_vf_guid,
	.get_vf_stats = mlx5_ib_get_vf_stats,
	.set_vf_guid = mlx5_ib_set_vf_guid,
	.set_vf_link_state = mlx5_ib_set_vf_link_state,
};

static const struct ib_device_ops mlx5_ib_dev_mw_ops = {
	.alloc_mw = mlx5_ib_alloc_mw,
	.dealloc_mw = mlx5_ib_dealloc_mw,

	INIT_RDMA_OBJ_SIZE(ib_mw, mlx5_ib_mw, ibmw),
};

static const struct ib_device_ops mlx5_ib_dev_xrc_ops = {
	.alloc_xrcd = mlx5_ib_alloc_xrcd,
	.dealloc_xrcd = mlx5_ib_dealloc_xrcd,

	INIT_RDMA_OBJ_SIZE(ib_xrcd, mlx5_ib_xrcd, ibxrcd),
};

static int mlx5_ib_init_var_table(struct mlx5_ib_dev *dev)
{
	struct mlx5_core_dev *mdev = dev->mdev;
	struct mlx5_var_table *var_table = &dev->var_table;
	u8 log_doorbell_bar_size;
	u8 log_doorbell_stride;
	u64 bar_size;

	log_doorbell_bar_size = MLX5_CAP_DEV_VDPA_EMULATION(mdev,
					log_doorbell_bar_size);
	log_doorbell_stride = MLX5_CAP_DEV_VDPA_EMULATION(mdev,
					log_doorbell_stride);
	var_table->hw_start_addr = dev->mdev->bar_addr +
				MLX5_CAP64_DEV_VDPA_EMULATION(mdev,
					doorbell_bar_offset);
	bar_size = (1ULL << log_doorbell_bar_size) * 4096;
	var_table->stride_size = 1ULL << log_doorbell_stride;
	var_table->num_var_hw_entries = div_u64(bar_size,
						var_table->stride_size);
	mutex_init(&var_table->bitmap_lock);
	var_table->bitmap = bitmap_zalloc(var_table->num_var_hw_entries,
					  GFP_KERNEL);
	return (var_table->bitmap) ? 0 : -ENOMEM;
}

static void mlx5_ib_stage_caps_cleanup(struct mlx5_ib_dev *dev)
{
	bitmap_free(dev->var_table.bitmap);
}

static int mlx5_ib_stage_caps_init(struct mlx5_ib_dev *dev)
>>>>>>> upstream/android-13
{
	struct mlx5_core_dev *mdev = dev->mdev;
	int err;

<<<<<<< HEAD
	dev->ib_dev.uverbs_abi_ver	= MLX5_IB_UVERBS_ABI_VERSION;
	dev->ib_dev.uverbs_cmd_mask	=
		(1ull << IB_USER_VERBS_CMD_GET_CONTEXT)		|
		(1ull << IB_USER_VERBS_CMD_QUERY_DEVICE)	|
		(1ull << IB_USER_VERBS_CMD_QUERY_PORT)		|
		(1ull << IB_USER_VERBS_CMD_ALLOC_PD)		|
		(1ull << IB_USER_VERBS_CMD_DEALLOC_PD)		|
		(1ull << IB_USER_VERBS_CMD_CREATE_AH)		|
		(1ull << IB_USER_VERBS_CMD_DESTROY_AH)		|
		(1ull << IB_USER_VERBS_CMD_REG_MR)		|
		(1ull << IB_USER_VERBS_CMD_REREG_MR)		|
		(1ull << IB_USER_VERBS_CMD_DEREG_MR)		|
		(1ull << IB_USER_VERBS_CMD_CREATE_COMP_CHANNEL)	|
		(1ull << IB_USER_VERBS_CMD_CREATE_CQ)		|
		(1ull << IB_USER_VERBS_CMD_RESIZE_CQ)		|
		(1ull << IB_USER_VERBS_CMD_DESTROY_CQ)		|
		(1ull << IB_USER_VERBS_CMD_CREATE_QP)		|
		(1ull << IB_USER_VERBS_CMD_MODIFY_QP)		|
		(1ull << IB_USER_VERBS_CMD_QUERY_QP)		|
		(1ull << IB_USER_VERBS_CMD_DESTROY_QP)		|
		(1ull << IB_USER_VERBS_CMD_ATTACH_MCAST)	|
		(1ull << IB_USER_VERBS_CMD_DETACH_MCAST)	|
		(1ull << IB_USER_VERBS_CMD_CREATE_SRQ)		|
		(1ull << IB_USER_VERBS_CMD_MODIFY_SRQ)		|
		(1ull << IB_USER_VERBS_CMD_QUERY_SRQ)		|
		(1ull << IB_USER_VERBS_CMD_DESTROY_SRQ)		|
		(1ull << IB_USER_VERBS_CMD_CREATE_XSRQ)		|
		(1ull << IB_USER_VERBS_CMD_OPEN_QP);
	dev->ib_dev.uverbs_ex_cmd_mask =
		(1ull << IB_USER_VERBS_EX_CMD_QUERY_DEVICE)	|
		(1ull << IB_USER_VERBS_EX_CMD_CREATE_CQ)	|
		(1ull << IB_USER_VERBS_EX_CMD_CREATE_QP)	|
		(1ull << IB_USER_VERBS_EX_CMD_MODIFY_QP)	|
		(1ull << IB_USER_VERBS_EX_CMD_MODIFY_CQ);

	dev->ib_dev.query_device	= mlx5_ib_query_device;
	dev->ib_dev.get_link_layer	= mlx5_ib_port_link_layer;
	dev->ib_dev.query_gid		= mlx5_ib_query_gid;
	dev->ib_dev.add_gid		= mlx5_ib_add_gid;
	dev->ib_dev.del_gid		= mlx5_ib_del_gid;
	dev->ib_dev.query_pkey		= mlx5_ib_query_pkey;
	dev->ib_dev.modify_device	= mlx5_ib_modify_device;
	dev->ib_dev.modify_port		= mlx5_ib_modify_port;
	dev->ib_dev.alloc_ucontext	= mlx5_ib_alloc_ucontext;
	dev->ib_dev.dealloc_ucontext	= mlx5_ib_dealloc_ucontext;
	dev->ib_dev.mmap		= mlx5_ib_mmap;
	dev->ib_dev.alloc_pd		= mlx5_ib_alloc_pd;
	dev->ib_dev.dealloc_pd		= mlx5_ib_dealloc_pd;
	dev->ib_dev.create_ah		= mlx5_ib_create_ah;
	dev->ib_dev.query_ah		= mlx5_ib_query_ah;
	dev->ib_dev.destroy_ah		= mlx5_ib_destroy_ah;
	dev->ib_dev.create_srq		= mlx5_ib_create_srq;
	dev->ib_dev.modify_srq		= mlx5_ib_modify_srq;
	dev->ib_dev.query_srq		= mlx5_ib_query_srq;
	dev->ib_dev.destroy_srq		= mlx5_ib_destroy_srq;
	dev->ib_dev.post_srq_recv	= mlx5_ib_post_srq_recv;
	dev->ib_dev.create_qp		= mlx5_ib_create_qp;
	dev->ib_dev.modify_qp		= mlx5_ib_modify_qp;
	dev->ib_dev.query_qp		= mlx5_ib_query_qp;
	dev->ib_dev.destroy_qp		= mlx5_ib_destroy_qp;
	dev->ib_dev.drain_sq		= mlx5_ib_drain_sq;
	dev->ib_dev.drain_rq		= mlx5_ib_drain_rq;
	dev->ib_dev.post_send		= mlx5_ib_post_send;
	dev->ib_dev.post_recv		= mlx5_ib_post_recv;
	dev->ib_dev.create_cq		= mlx5_ib_create_cq;
	dev->ib_dev.modify_cq		= mlx5_ib_modify_cq;
	dev->ib_dev.resize_cq		= mlx5_ib_resize_cq;
	dev->ib_dev.destroy_cq		= mlx5_ib_destroy_cq;
	dev->ib_dev.poll_cq		= mlx5_ib_poll_cq;
	dev->ib_dev.req_notify_cq	= mlx5_ib_arm_cq;
	dev->ib_dev.get_dma_mr		= mlx5_ib_get_dma_mr;
	dev->ib_dev.reg_user_mr		= mlx5_ib_reg_user_mr;
	dev->ib_dev.rereg_user_mr	= mlx5_ib_rereg_user_mr;
	dev->ib_dev.dereg_mr		= mlx5_ib_dereg_mr;
	dev->ib_dev.attach_mcast	= mlx5_ib_mcg_attach;
	dev->ib_dev.detach_mcast	= mlx5_ib_mcg_detach;
	dev->ib_dev.process_mad		= mlx5_ib_process_mad;
	dev->ib_dev.alloc_mr		= mlx5_ib_alloc_mr;
	dev->ib_dev.map_mr_sg		= mlx5_ib_map_mr_sg;
	dev->ib_dev.check_mr_status	= mlx5_ib_check_mr_status;
	dev->ib_dev.get_dev_fw_str      = get_dev_fw_str;
	dev->ib_dev.get_vector_affinity	= mlx5_ib_get_vector_affinity;
	if (MLX5_CAP_GEN(mdev, ipoib_enhanced_offloads))
		dev->ib_dev.alloc_rdma_netdev	= mlx5_ib_alloc_rdma_netdev;

	if (mlx5_core_is_pf(mdev)) {
		dev->ib_dev.get_vf_config	= mlx5_ib_get_vf_config;
		dev->ib_dev.set_vf_link_state	= mlx5_ib_set_vf_link_state;
		dev->ib_dev.get_vf_stats	= mlx5_ib_get_vf_stats;
		dev->ib_dev.set_vf_guid		= mlx5_ib_set_vf_guid;
	}

	dev->ib_dev.disassociate_ucontext = mlx5_ib_disassociate_ucontext;

	dev->umr_fence = mlx5_get_umr_fence(MLX5_CAP_GEN(mdev, umr_fence));

	if (MLX5_CAP_GEN(mdev, imaicl)) {
		dev->ib_dev.alloc_mw		= mlx5_ib_alloc_mw;
		dev->ib_dev.dealloc_mw		= mlx5_ib_dealloc_mw;
		dev->ib_dev.uverbs_cmd_mask |=
			(1ull << IB_USER_VERBS_CMD_ALLOC_MW)	|
			(1ull << IB_USER_VERBS_CMD_DEALLOC_MW);
	}

	if (MLX5_CAP_GEN(mdev, xrc)) {
		dev->ib_dev.alloc_xrcd = mlx5_ib_alloc_xrcd;
		dev->ib_dev.dealloc_xrcd = mlx5_ib_dealloc_xrcd;
		dev->ib_dev.uverbs_cmd_mask |=
			(1ull << IB_USER_VERBS_CMD_OPEN_XRCD) |
			(1ull << IB_USER_VERBS_CMD_CLOSE_XRCD);
	}

	if (MLX5_CAP_DEV_MEM(mdev, memic)) {
		dev->ib_dev.alloc_dm = mlx5_ib_alloc_dm;
		dev->ib_dev.dealloc_dm = mlx5_ib_dealloc_dm;
		dev->ib_dev.reg_dm_mr = mlx5_ib_reg_dm_mr;
	}

	dev->ib_dev.create_flow	= mlx5_ib_create_flow;
	dev->ib_dev.destroy_flow = mlx5_ib_destroy_flow;
	dev->ib_dev.uverbs_ex_cmd_mask |=
			(1ull << IB_USER_VERBS_EX_CMD_CREATE_FLOW) |
			(1ull << IB_USER_VERBS_EX_CMD_DESTROY_FLOW);
	dev->ib_dev.create_flow_action_esp = mlx5_ib_create_flow_action_esp;
	dev->ib_dev.destroy_flow_action = mlx5_ib_destroy_flow_action;
	dev->ib_dev.modify_flow_action_esp = mlx5_ib_modify_flow_action_esp;
	dev->ib_dev.driver_id = RDMA_DRIVER_MLX5;
	dev->ib_dev.create_counters = mlx5_ib_create_counters;
	dev->ib_dev.destroy_counters = mlx5_ib_destroy_counters;
	dev->ib_dev.read_counters = mlx5_ib_read_counters;
=======
	if (MLX5_CAP_GEN(mdev, ipoib_enhanced_offloads) &&
	    IS_ENABLED(CONFIG_MLX5_CORE_IPOIB))
		ib_set_device_ops(&dev->ib_dev,
				  &mlx5_ib_dev_ipoib_enhanced_ops);

	if (mlx5_core_is_pf(mdev))
		ib_set_device_ops(&dev->ib_dev, &mlx5_ib_dev_sriov_ops);

	dev->umr_fence = mlx5_get_umr_fence(MLX5_CAP_GEN(mdev, umr_fence));

	if (MLX5_CAP_GEN(mdev, imaicl))
		ib_set_device_ops(&dev->ib_dev, &mlx5_ib_dev_mw_ops);

	if (MLX5_CAP_GEN(mdev, xrc))
		ib_set_device_ops(&dev->ib_dev, &mlx5_ib_dev_xrc_ops);

	if (MLX5_CAP_DEV_MEM(mdev, memic) ||
	    MLX5_CAP_GEN_64(dev->mdev, general_obj_types) &
	    MLX5_GENERAL_OBJ_TYPES_CAP_SW_ICM)
		ib_set_device_ops(&dev->ib_dev, &mlx5_ib_dev_dm_ops);

	ib_set_device_ops(&dev->ib_dev, &mlx5_ib_dev_ops);

	if (IS_ENABLED(CONFIG_INFINIBAND_USER_ACCESS))
		dev->ib_dev.driver_def = mlx5_ib_defs;
>>>>>>> upstream/android-13

	err = init_node_data(dev);
	if (err)
		return err;

	if ((MLX5_CAP_GEN(dev->mdev, port_type) == MLX5_CAP_PORT_TYPE_ETH) &&
	    (MLX5_CAP_GEN(dev->mdev, disable_local_lb_uc) ||
	     MLX5_CAP_GEN(dev->mdev, disable_local_lb_mc)))
<<<<<<< HEAD
		mutex_init(&dev->lb_mutex);
=======
		mutex_init(&dev->lb.mutex);

	if (MLX5_CAP_GEN_64(dev->mdev, general_obj_types) &
			MLX5_GENERAL_OBJ_TYPES_CAP_VIRTIO_NET_Q) {
		err = mlx5_ib_init_var_table(dev);
		if (err)
			return err;
	}

	dev->ib_dev.use_cq_dim = true;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int mlx5_ib_stage_non_default_cb(struct mlx5_ib_dev *dev)
{
	dev->ib_dev.get_port_immutable  = mlx5_port_immutable;
	dev->ib_dev.query_port		= mlx5_ib_query_port;

	return 0;
}

int mlx5_ib_stage_rep_non_default_cb(struct mlx5_ib_dev *dev)
{
	dev->ib_dev.get_port_immutable  = mlx5_port_rep_immutable;
	dev->ib_dev.query_port		= mlx5_ib_rep_query_port;

	return 0;
}

static int mlx5_ib_stage_common_roce_init(struct mlx5_ib_dev *dev)
{
	u8 port_num;
	int i;

	for (i = 0; i < dev->num_ports; i++) {
		dev->roce[i].dev = dev;
		dev->roce[i].native_port_num = i + 1;
		dev->roce[i].last_port_state = IB_PORT_DOWN;
	}

	dev->ib_dev.get_netdev	= mlx5_ib_get_netdev;
	dev->ib_dev.create_wq	 = mlx5_ib_create_wq;
	dev->ib_dev.modify_wq	 = mlx5_ib_modify_wq;
	dev->ib_dev.destroy_wq	 = mlx5_ib_destroy_wq;
	dev->ib_dev.create_rwq_ind_table = mlx5_ib_create_rwq_ind_table;
	dev->ib_dev.destroy_rwq_ind_table = mlx5_ib_destroy_rwq_ind_table;

	dev->ib_dev.uverbs_ex_cmd_mask |=
			(1ull << IB_USER_VERBS_EX_CMD_CREATE_WQ) |
			(1ull << IB_USER_VERBS_EX_CMD_MODIFY_WQ) |
			(1ull << IB_USER_VERBS_EX_CMD_DESTROY_WQ) |
			(1ull << IB_USER_VERBS_EX_CMD_CREATE_RWQ_IND_TBL) |
			(1ull << IB_USER_VERBS_EX_CMD_DESTROY_RWQ_IND_TBL);

	port_num = mlx5_core_native_port_num(dev->mdev) - 1;

	return mlx5_add_netdev_notifier(dev, port_num);
}

static void mlx5_ib_stage_common_roce_cleanup(struct mlx5_ib_dev *dev)
{
	u8 port_num = mlx5_core_native_port_num(dev->mdev) - 1;

	mlx5_remove_netdev_notifier(dev, port_num);
}

int mlx5_ib_stage_rep_roce_init(struct mlx5_ib_dev *dev)
{
	struct mlx5_core_dev *mdev = dev->mdev;
	enum rdma_link_layer ll;
	int port_type_cap;
	int err = 0;

	port_type_cap = MLX5_CAP_GEN(mdev, port_type);
	ll = mlx5_port_type_cap_to_rdma_ll(port_type_cap);

	if (ll == IB_LINK_LAYER_ETHERNET)
		err = mlx5_ib_stage_common_roce_init(dev);

	return err;
}

void mlx5_ib_stage_rep_roce_cleanup(struct mlx5_ib_dev *dev)
{
	mlx5_ib_stage_common_roce_cleanup(dev);
}

static int mlx5_ib_stage_roce_init(struct mlx5_ib_dev *dev)
=======
static const struct ib_device_ops mlx5_ib_dev_port_ops = {
	.get_port_immutable = mlx5_port_immutable,
	.query_port = mlx5_ib_query_port,
};

static int mlx5_ib_stage_non_default_cb(struct mlx5_ib_dev *dev)
{
	ib_set_device_ops(&dev->ib_dev, &mlx5_ib_dev_port_ops);
	return 0;
}

static const struct ib_device_ops mlx5_ib_dev_port_rep_ops = {
	.get_port_immutable = mlx5_port_rep_immutable,
	.query_port = mlx5_ib_rep_query_port,
	.query_pkey = mlx5_ib_rep_query_pkey,
};

static int mlx5_ib_stage_raw_eth_non_default_cb(struct mlx5_ib_dev *dev)
{
	ib_set_device_ops(&dev->ib_dev, &mlx5_ib_dev_port_rep_ops);
	return 0;
}

static const struct ib_device_ops mlx5_ib_dev_common_roce_ops = {
	.create_rwq_ind_table = mlx5_ib_create_rwq_ind_table,
	.create_wq = mlx5_ib_create_wq,
	.destroy_rwq_ind_table = mlx5_ib_destroy_rwq_ind_table,
	.destroy_wq = mlx5_ib_destroy_wq,
	.get_netdev = mlx5_ib_get_netdev,
	.modify_wq = mlx5_ib_modify_wq,

	INIT_RDMA_OBJ_SIZE(ib_rwq_ind_table, mlx5_ib_rwq_ind_table,
			   ib_rwq_ind_tbl),
};

static int mlx5_ib_roce_init(struct mlx5_ib_dev *dev)
>>>>>>> upstream/android-13
{
	struct mlx5_core_dev *mdev = dev->mdev;
	enum rdma_link_layer ll;
	int port_type_cap;
<<<<<<< HEAD
=======
	u32 port_num = 0;
>>>>>>> upstream/android-13
	int err;

	port_type_cap = MLX5_CAP_GEN(mdev, port_type);
	ll = mlx5_port_type_cap_to_rdma_ll(port_type_cap);

	if (ll == IB_LINK_LAYER_ETHERNET) {
<<<<<<< HEAD
		err = mlx5_ib_stage_common_roce_init(dev);
=======
		ib_set_device_ops(&dev->ib_dev, &mlx5_ib_dev_common_roce_ops);

		port_num = mlx5_core_native_port_num(dev->mdev) - 1;

		/* Register only for native ports */
		err = mlx5_add_netdev_notifier(dev, port_num);
>>>>>>> upstream/android-13
		if (err)
			return err;

		err = mlx5_enable_eth(dev);
		if (err)
			goto cleanup;
	}

	return 0;
cleanup:
<<<<<<< HEAD
	mlx5_ib_stage_common_roce_cleanup(dev);

	return err;
}

static void mlx5_ib_stage_roce_cleanup(struct mlx5_ib_dev *dev)
=======
	mlx5_remove_netdev_notifier(dev, port_num);
	return err;
}

static void mlx5_ib_roce_cleanup(struct mlx5_ib_dev *dev)
>>>>>>> upstream/android-13
{
	struct mlx5_core_dev *mdev = dev->mdev;
	enum rdma_link_layer ll;
	int port_type_cap;
<<<<<<< HEAD
=======
	u32 port_num;
>>>>>>> upstream/android-13

	port_type_cap = MLX5_CAP_GEN(mdev, port_type);
	ll = mlx5_port_type_cap_to_rdma_ll(port_type_cap);

	if (ll == IB_LINK_LAYER_ETHERNET) {
		mlx5_disable_eth(dev);
<<<<<<< HEAD
		mlx5_ib_stage_common_roce_cleanup(dev);
	}
}

int mlx5_ib_stage_dev_res_init(struct mlx5_ib_dev *dev)
{
	return create_dev_resources(&dev->devr);
}

void mlx5_ib_stage_dev_res_cleanup(struct mlx5_ib_dev *dev)
{
	destroy_dev_resources(&dev->devr);
}

static int mlx5_ib_stage_odp_init(struct mlx5_ib_dev *dev)
{
	mlx5_ib_internal_fill_odp_caps(dev);

	return mlx5_ib_odp_init_one(dev);
}

int mlx5_ib_stage_counters_init(struct mlx5_ib_dev *dev)
{
	if (MLX5_CAP_GEN(dev->mdev, max_qp_cnt)) {
		dev->ib_dev.get_hw_stats	= mlx5_ib_get_hw_stats;
		dev->ib_dev.alloc_hw_stats	= mlx5_ib_alloc_hw_stats;

		return mlx5_ib_alloc_counters(dev);
	}

	return 0;
}

void mlx5_ib_stage_counters_cleanup(struct mlx5_ib_dev *dev)
{
	if (MLX5_CAP_GEN(dev->mdev, max_qp_cnt))
		mlx5_ib_dealloc_counters(dev);
}

static int mlx5_ib_stage_cong_debugfs_init(struct mlx5_ib_dev *dev)
{
	return mlx5_ib_init_cong_debugfs(dev,
					 mlx5_core_native_port_num(dev->mdev) - 1);
=======

		port_num = mlx5_core_native_port_num(dev->mdev) - 1;
		mlx5_remove_netdev_notifier(dev, port_num);
	}
}

static int mlx5_ib_stage_cong_debugfs_init(struct mlx5_ib_dev *dev)
{
	mlx5_ib_init_cong_debugfs(dev,
				  mlx5_core_native_port_num(dev->mdev) - 1);
	return 0;
>>>>>>> upstream/android-13
}

static void mlx5_ib_stage_cong_debugfs_cleanup(struct mlx5_ib_dev *dev)
{
	mlx5_ib_cleanup_cong_debugfs(dev,
				     mlx5_core_native_port_num(dev->mdev) - 1);
}

static int mlx5_ib_stage_uar_init(struct mlx5_ib_dev *dev)
{
	dev->mdev->priv.uar = mlx5_get_uars_page(dev->mdev);
	return PTR_ERR_OR_ZERO(dev->mdev->priv.uar);
}

static void mlx5_ib_stage_uar_cleanup(struct mlx5_ib_dev *dev)
{
	mlx5_put_uars_page(dev->mdev, dev->mdev->priv.uar);
}

<<<<<<< HEAD
int mlx5_ib_stage_bfrag_init(struct mlx5_ib_dev *dev)
=======
static int mlx5_ib_stage_bfrag_init(struct mlx5_ib_dev *dev)
>>>>>>> upstream/android-13
{
	int err;

	err = mlx5_alloc_bfreg(dev->mdev, &dev->bfreg, false, false);
	if (err)
		return err;

	err = mlx5_alloc_bfreg(dev->mdev, &dev->fp_bfreg, false, true);
	if (err)
		mlx5_free_bfreg(dev->mdev, &dev->bfreg);

	return err;
}

<<<<<<< HEAD
void mlx5_ib_stage_bfrag_cleanup(struct mlx5_ib_dev *dev)
=======
static void mlx5_ib_stage_bfrag_cleanup(struct mlx5_ib_dev *dev)
>>>>>>> upstream/android-13
{
	mlx5_free_bfreg(dev->mdev, &dev->fp_bfreg);
	mlx5_free_bfreg(dev->mdev, &dev->bfreg);
}

<<<<<<< HEAD
static int mlx5_ib_stage_populate_specs(struct mlx5_ib_dev *dev)
{
	return populate_specs_root(dev);
}

int mlx5_ib_stage_ib_reg_init(struct mlx5_ib_dev *dev)
{
	return ib_register_device(&dev->ib_dev, NULL);
}

void mlx5_ib_stage_pre_ib_reg_umr_cleanup(struct mlx5_ib_dev *dev)
{
	destroy_umrc_res(dev);
}

void mlx5_ib_stage_ib_reg_cleanup(struct mlx5_ib_dev *dev)
=======
static int mlx5_ib_stage_ib_reg_init(struct mlx5_ib_dev *dev)
{
	const char *name;

	if (!mlx5_lag_is_active(dev->mdev))
		name = "mlx5_%d";
	else
		name = "mlx5_bond_%d";
	return ib_register_device(&dev->ib_dev, name, &dev->mdev->pdev->dev);
}

static void mlx5_ib_stage_pre_ib_reg_umr_cleanup(struct mlx5_ib_dev *dev)
{
	int err;

	err = mlx5_mr_cache_cleanup(dev);
	if (err)
		mlx5_ib_warn(dev, "mr cache cleanup failed\n");

	if (dev->umrc.qp)
		ib_destroy_qp(dev->umrc.qp);
	if (dev->umrc.cq)
		ib_free_cq(dev->umrc.cq);
	if (dev->umrc.pd)
		ib_dealloc_pd(dev->umrc.pd);
}

static void mlx5_ib_stage_ib_reg_cleanup(struct mlx5_ib_dev *dev)
>>>>>>> upstream/android-13
{
	ib_unregister_device(&dev->ib_dev);
}

<<<<<<< HEAD
int mlx5_ib_stage_post_ib_reg_umr_init(struct mlx5_ib_dev *dev)
{
	return create_umr_res(dev);
=======
enum {
	MAX_UMR_WR = 128,
};

static int mlx5_ib_stage_post_ib_reg_umr_init(struct mlx5_ib_dev *dev)
{
	struct ib_qp_init_attr *init_attr = NULL;
	struct ib_qp_attr *attr = NULL;
	struct ib_pd *pd;
	struct ib_cq *cq;
	struct ib_qp *qp;
	int ret;

	attr = kzalloc(sizeof(*attr), GFP_KERNEL);
	init_attr = kzalloc(sizeof(*init_attr), GFP_KERNEL);
	if (!attr || !init_attr) {
		ret = -ENOMEM;
		goto error_0;
	}

	pd = ib_alloc_pd(&dev->ib_dev, 0);
	if (IS_ERR(pd)) {
		mlx5_ib_dbg(dev, "Couldn't create PD for sync UMR QP\n");
		ret = PTR_ERR(pd);
		goto error_0;
	}

	cq = ib_alloc_cq(&dev->ib_dev, NULL, 128, 0, IB_POLL_SOFTIRQ);
	if (IS_ERR(cq)) {
		mlx5_ib_dbg(dev, "Couldn't create CQ for sync UMR QP\n");
		ret = PTR_ERR(cq);
		goto error_2;
	}

	init_attr->send_cq = cq;
	init_attr->recv_cq = cq;
	init_attr->sq_sig_type = IB_SIGNAL_ALL_WR;
	init_attr->cap.max_send_wr = MAX_UMR_WR;
	init_attr->cap.max_send_sge = 1;
	init_attr->qp_type = MLX5_IB_QPT_REG_UMR;
	init_attr->port_num = 1;
	qp = ib_create_qp(pd, init_attr);
	if (IS_ERR(qp)) {
		mlx5_ib_dbg(dev, "Couldn't create sync UMR QP\n");
		ret = PTR_ERR(qp);
		goto error_3;
	}

	attr->qp_state = IB_QPS_INIT;
	attr->port_num = 1;
	ret = ib_modify_qp(qp, attr,
			   IB_QP_STATE | IB_QP_PKEY_INDEX | IB_QP_PORT);
	if (ret) {
		mlx5_ib_dbg(dev, "Couldn't modify UMR QP\n");
		goto error_4;
	}

	memset(attr, 0, sizeof(*attr));
	attr->qp_state = IB_QPS_RTR;
	attr->path_mtu = IB_MTU_256;

	ret = ib_modify_qp(qp, attr, IB_QP_STATE);
	if (ret) {
		mlx5_ib_dbg(dev, "Couldn't modify umr QP to rtr\n");
		goto error_4;
	}

	memset(attr, 0, sizeof(*attr));
	attr->qp_state = IB_QPS_RTS;
	ret = ib_modify_qp(qp, attr, IB_QP_STATE);
	if (ret) {
		mlx5_ib_dbg(dev, "Couldn't modify umr QP to rts\n");
		goto error_4;
	}

	dev->umrc.qp = qp;
	dev->umrc.cq = cq;
	dev->umrc.pd = pd;

	sema_init(&dev->umrc.sem, MAX_UMR_WR);
	ret = mlx5_mr_cache_init(dev);
	if (ret) {
		mlx5_ib_warn(dev, "mr cache init failed %d\n", ret);
		goto error_4;
	}

	kfree(attr);
	kfree(init_attr);

	return 0;

error_4:
	ib_destroy_qp(qp);
	dev->umrc.qp = NULL;

error_3:
	ib_free_cq(cq);
	dev->umrc.cq = NULL;

error_2:
	ib_dealloc_pd(pd);
	dev->umrc.pd = NULL;

error_0:
	kfree(attr);
	kfree(init_attr);
	return ret;
>>>>>>> upstream/android-13
}

static int mlx5_ib_stage_delay_drop_init(struct mlx5_ib_dev *dev)
{
<<<<<<< HEAD
	init_delay_drop(dev);

=======
	struct dentry *root;

	if (!(dev->ib_dev.attrs.raw_packet_caps & IB_RAW_PACKET_CAP_DELAY_DROP))
		return 0;

	mutex_init(&dev->delay_drop.lock);
	dev->delay_drop.dev = dev;
	dev->delay_drop.activate = false;
	dev->delay_drop.timeout = MLX5_MAX_DELAY_DROP_TIMEOUT_MS * 1000;
	INIT_WORK(&dev->delay_drop.delay_drop_work, delay_drop_handler);
	atomic_set(&dev->delay_drop.rqs_cnt, 0);
	atomic_set(&dev->delay_drop.events_cnt, 0);

	if (!mlx5_debugfs_root)
		return 0;

	root = debugfs_create_dir("delay_drop", dev->mdev->priv.dbg_root);
	dev->delay_drop.dir_debugfs = root;

	debugfs_create_atomic_t("num_timeout_events", 0400, root,
				&dev->delay_drop.events_cnt);
	debugfs_create_atomic_t("num_rqs", 0400, root,
				&dev->delay_drop.rqs_cnt);
	debugfs_create_file("timeout", 0600, root, &dev->delay_drop,
			    &fops_delay_drop_timeout);
>>>>>>> upstream/android-13
	return 0;
}

static void mlx5_ib_stage_delay_drop_cleanup(struct mlx5_ib_dev *dev)
{
<<<<<<< HEAD
	cancel_delay_drop(dev);
}

int mlx5_ib_stage_class_attr_init(struct mlx5_ib_dev *dev)
{
	int err;
	int i;

	for (i = 0; i < ARRAY_SIZE(mlx5_class_attributes); i++) {
		err = device_create_file(&dev->ib_dev.dev,
					 mlx5_class_attributes[i]);
		if (err)
			return err;
	}

	return 0;
}

static int mlx5_ib_stage_rep_reg_init(struct mlx5_ib_dev *dev)
{
	mlx5_ib_register_vport_reps(dev);

	return 0;
}

static void mlx5_ib_stage_rep_reg_cleanup(struct mlx5_ib_dev *dev)
{
	mlx5_ib_unregister_vport_reps(dev);
=======
	if (!(dev->ib_dev.attrs.raw_packet_caps & IB_RAW_PACKET_CAP_DELAY_DROP))
		return;

	cancel_work_sync(&dev->delay_drop.delay_drop_work);
	if (!dev->delay_drop.dir_debugfs)
		return;

	debugfs_remove_recursive(dev->delay_drop.dir_debugfs);
	dev->delay_drop.dir_debugfs = NULL;
}

static int mlx5_ib_stage_dev_notifier_init(struct mlx5_ib_dev *dev)
{
	dev->mdev_events.notifier_call = mlx5_ib_event;
	mlx5_notifier_register(dev->mdev, &dev->mdev_events);
	return 0;
}

static void mlx5_ib_stage_dev_notifier_cleanup(struct mlx5_ib_dev *dev)
{
	mlx5_notifier_unregister(dev->mdev, &dev->mdev_events);
>>>>>>> upstream/android-13
}

void __mlx5_ib_remove(struct mlx5_ib_dev *dev,
		      const struct mlx5_ib_profile *profile,
		      int stage)
{
<<<<<<< HEAD
=======
	dev->ib_active = false;

>>>>>>> upstream/android-13
	/* Number of stages to cleanup */
	while (stage) {
		stage--;
		if (profile->stage[stage].cleanup)
			profile->stage[stage].cleanup(dev);
	}

<<<<<<< HEAD
	ib_dealloc_device((struct ib_device *)dev);
}

void *__mlx5_ib_add(struct mlx5_ib_dev *dev,
		    const struct mlx5_ib_profile *profile)
=======
	kfree(dev->port);
	ib_dealloc_device(&dev->ib_dev);
}

int __mlx5_ib_add(struct mlx5_ib_dev *dev,
		  const struct mlx5_ib_profile *profile)
>>>>>>> upstream/android-13
{
	int err;
	int i;

<<<<<<< HEAD
	printk_once(KERN_INFO "%s", mlx5_version);
=======
	dev->profile = profile;
>>>>>>> upstream/android-13

	for (i = 0; i < MLX5_IB_STAGE_MAX; i++) {
		if (profile->stage[i].init) {
			err = profile->stage[i].init(dev);
			if (err)
				goto err_out;
		}
	}

<<<<<<< HEAD
	dev->profile = profile;
	dev->ib_active = true;

	return dev;

err_out:
	__mlx5_ib_remove(dev, profile, i);

	return NULL;
=======
	dev->ib_active = true;
	return 0;

err_out:
	/* Clean up stages which were initialized */
	while (i) {
		i--;
		if (profile->stage[i].cleanup)
			profile->stage[i].cleanup(dev);
	}
	return -ENOMEM;
>>>>>>> upstream/android-13
}

static const struct mlx5_ib_profile pf_profile = {
	STAGE_CREATE(MLX5_IB_STAGE_INIT,
		     mlx5_ib_stage_init_init,
		     mlx5_ib_stage_init_cleanup),
<<<<<<< HEAD
	STAGE_CREATE(MLX5_IB_STAGE_FLOW_DB,
		     mlx5_ib_stage_flow_db_init,
		     mlx5_ib_stage_flow_db_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_CAPS,
		     mlx5_ib_stage_caps_init,
		     NULL),
=======
	STAGE_CREATE(MLX5_IB_STAGE_FS,
		     mlx5_ib_fs_init,
		     mlx5_ib_fs_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_CAPS,
		     mlx5_ib_stage_caps_init,
		     mlx5_ib_stage_caps_cleanup),
>>>>>>> upstream/android-13
	STAGE_CREATE(MLX5_IB_STAGE_NON_DEFAULT_CB,
		     mlx5_ib_stage_non_default_cb,
		     NULL),
	STAGE_CREATE(MLX5_IB_STAGE_ROCE,
<<<<<<< HEAD
		     mlx5_ib_stage_roce_init,
		     mlx5_ib_stage_roce_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_DEVICE_RESOURCES,
		     mlx5_ib_stage_dev_res_init,
		     mlx5_ib_stage_dev_res_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_ODP,
		     mlx5_ib_stage_odp_init,
		     NULL),
	STAGE_CREATE(MLX5_IB_STAGE_COUNTERS,
		     mlx5_ib_stage_counters_init,
		     mlx5_ib_stage_counters_cleanup),
=======
		     mlx5_ib_roce_init,
		     mlx5_ib_roce_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_QP,
		     mlx5_init_qp_table,
		     mlx5_cleanup_qp_table),
	STAGE_CREATE(MLX5_IB_STAGE_SRQ,
		     mlx5_init_srq_table,
		     mlx5_cleanup_srq_table),
	STAGE_CREATE(MLX5_IB_STAGE_DEVICE_RESOURCES,
		     mlx5_ib_dev_res_init,
		     mlx5_ib_dev_res_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_DEVICE_NOTIFIER,
		     mlx5_ib_stage_dev_notifier_init,
		     mlx5_ib_stage_dev_notifier_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_ODP,
		     mlx5_ib_odp_init_one,
		     mlx5_ib_odp_cleanup_one),
	STAGE_CREATE(MLX5_IB_STAGE_COUNTERS,
		     mlx5_ib_counters_init,
		     mlx5_ib_counters_cleanup),
>>>>>>> upstream/android-13
	STAGE_CREATE(MLX5_IB_STAGE_CONG_DEBUGFS,
		     mlx5_ib_stage_cong_debugfs_init,
		     mlx5_ib_stage_cong_debugfs_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_UAR,
		     mlx5_ib_stage_uar_init,
		     mlx5_ib_stage_uar_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_BFREG,
		     mlx5_ib_stage_bfrag_init,
		     mlx5_ib_stage_bfrag_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_PRE_IB_REG_UMR,
		     NULL,
		     mlx5_ib_stage_pre_ib_reg_umr_cleanup),
<<<<<<< HEAD
	STAGE_CREATE(MLX5_IB_STAGE_SPECS,
		     mlx5_ib_stage_populate_specs,
		     NULL),
=======
	STAGE_CREATE(MLX5_IB_STAGE_WHITELIST_UID,
		     mlx5_ib_devx_init,
		     mlx5_ib_devx_cleanup),
>>>>>>> upstream/android-13
	STAGE_CREATE(MLX5_IB_STAGE_IB_REG,
		     mlx5_ib_stage_ib_reg_init,
		     mlx5_ib_stage_ib_reg_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_POST_IB_REG_UMR,
		     mlx5_ib_stage_post_ib_reg_umr_init,
		     NULL),
	STAGE_CREATE(MLX5_IB_STAGE_DELAY_DROP,
		     mlx5_ib_stage_delay_drop_init,
		     mlx5_ib_stage_delay_drop_cleanup),
<<<<<<< HEAD
	STAGE_CREATE(MLX5_IB_STAGE_CLASS_ATTR,
		     mlx5_ib_stage_class_attr_init,
		     NULL),
};

static const struct mlx5_ib_profile nic_rep_profile = {
	STAGE_CREATE(MLX5_IB_STAGE_INIT,
		     mlx5_ib_stage_init_init,
		     mlx5_ib_stage_init_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_FLOW_DB,
		     mlx5_ib_stage_flow_db_init,
		     mlx5_ib_stage_flow_db_cleanup),
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
=======
	STAGE_CREATE(MLX5_IB_STAGE_RESTRACK,
		     mlx5_ib_restrack_init,
		     NULL),
};

const struct mlx5_ib_profile raw_eth_profile = {
	STAGE_CREATE(MLX5_IB_STAGE_INIT,
		     mlx5_ib_stage_init_init,
		     mlx5_ib_stage_init_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_FS,
		     mlx5_ib_fs_init,
		     mlx5_ib_fs_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_CAPS,
		     mlx5_ib_stage_caps_init,
		     mlx5_ib_stage_caps_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_NON_DEFAULT_CB,
		     mlx5_ib_stage_raw_eth_non_default_cb,
		     NULL),
	STAGE_CREATE(MLX5_IB_STAGE_ROCE,
		     mlx5_ib_roce_init,
		     mlx5_ib_roce_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_QP,
		     mlx5_init_qp_table,
		     mlx5_cleanup_qp_table),
	STAGE_CREATE(MLX5_IB_STAGE_SRQ,
		     mlx5_init_srq_table,
		     mlx5_cleanup_srq_table),
	STAGE_CREATE(MLX5_IB_STAGE_DEVICE_RESOURCES,
		     mlx5_ib_dev_res_init,
		     mlx5_ib_dev_res_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_DEVICE_NOTIFIER,
		     mlx5_ib_stage_dev_notifier_init,
		     mlx5_ib_stage_dev_notifier_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_COUNTERS,
		     mlx5_ib_counters_init,
		     mlx5_ib_counters_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_CONG_DEBUGFS,
		     mlx5_ib_stage_cong_debugfs_init,
		     mlx5_ib_stage_cong_debugfs_cleanup),
>>>>>>> upstream/android-13
	STAGE_CREATE(MLX5_IB_STAGE_UAR,
		     mlx5_ib_stage_uar_init,
		     mlx5_ib_stage_uar_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_BFREG,
		     mlx5_ib_stage_bfrag_init,
		     mlx5_ib_stage_bfrag_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_PRE_IB_REG_UMR,
		     NULL,
		     mlx5_ib_stage_pre_ib_reg_umr_cleanup),
<<<<<<< HEAD
	STAGE_CREATE(MLX5_IB_STAGE_SPECS,
		     mlx5_ib_stage_populate_specs,
		     NULL),
=======
	STAGE_CREATE(MLX5_IB_STAGE_WHITELIST_UID,
		     mlx5_ib_devx_init,
		     mlx5_ib_devx_cleanup),
>>>>>>> upstream/android-13
	STAGE_CREATE(MLX5_IB_STAGE_IB_REG,
		     mlx5_ib_stage_ib_reg_init,
		     mlx5_ib_stage_ib_reg_cleanup),
	STAGE_CREATE(MLX5_IB_STAGE_POST_IB_REG_UMR,
		     mlx5_ib_stage_post_ib_reg_umr_init,
		     NULL),
<<<<<<< HEAD
	STAGE_CREATE(MLX5_IB_STAGE_CLASS_ATTR,
		     mlx5_ib_stage_class_attr_init,
		     NULL),
	STAGE_CREATE(MLX5_IB_STAGE_REP_REG,
		     mlx5_ib_stage_rep_reg_init,
		     mlx5_ib_stage_rep_reg_cleanup),
};

static void *mlx5_ib_add_slave_port(struct mlx5_core_dev *mdev)
{
=======
	STAGE_CREATE(MLX5_IB_STAGE_RESTRACK,
		     mlx5_ib_restrack_init,
		     NULL),
};

static int mlx5r_mp_probe(struct auxiliary_device *adev,
			  const struct auxiliary_device_id *id)
{
	struct mlx5_adev *idev = container_of(adev, struct mlx5_adev, adev);
	struct mlx5_core_dev *mdev = idev->mdev;
>>>>>>> upstream/android-13
	struct mlx5_ib_multiport_info *mpi;
	struct mlx5_ib_dev *dev;
	bool bound = false;
	int err;

	mpi = kzalloc(sizeof(*mpi), GFP_KERNEL);
	if (!mpi)
<<<<<<< HEAD
		return NULL;

	mpi->mdev = mdev;

=======
		return -ENOMEM;

	mpi->mdev = mdev;
>>>>>>> upstream/android-13
	err = mlx5_query_nic_vport_system_image_guid(mdev,
						     &mpi->sys_image_guid);
	if (err) {
		kfree(mpi);
<<<<<<< HEAD
		return NULL;
=======
		return err;
>>>>>>> upstream/android-13
	}

	mutex_lock(&mlx5_ib_multiport_mutex);
	list_for_each_entry(dev, &mlx5_ib_dev_list, ib_dev_list) {
		if (dev->sys_image_guid == mpi->sys_image_guid)
			bound = mlx5_ib_bind_slave_port(dev, mpi);

		if (bound) {
			rdma_roce_rescan_device(&dev->ib_dev);
<<<<<<< HEAD
=======
			mpi->ibdev->ib_active = true;
>>>>>>> upstream/android-13
			break;
		}
	}

	if (!bound) {
		list_add_tail(&mpi->list, &mlx5_ib_unaffiliated_port_list);
<<<<<<< HEAD
		dev_dbg(&mdev->pdev->dev, "no suitable IB device found to bind to, added to unaffiliated list.\n");
	}
	mutex_unlock(&mlx5_ib_multiport_mutex);

	return mpi;
}

static void *mlx5_ib_add(struct mlx5_core_dev *mdev)
{
	enum rdma_link_layer ll;
	struct mlx5_ib_dev *dev;
	int port_type_cap;

	printk_once(KERN_INFO "%s", mlx5_version);
=======
		dev_dbg(mdev->device,
			"no suitable IB device found to bind to, added to unaffiliated list.\n");
	}
	mutex_unlock(&mlx5_ib_multiport_mutex);

	dev_set_drvdata(&adev->dev, mpi);
	return 0;
}

static void mlx5r_mp_remove(struct auxiliary_device *adev)
{
	struct mlx5_ib_multiport_info *mpi;

	mpi = dev_get_drvdata(&adev->dev);
	mutex_lock(&mlx5_ib_multiport_mutex);
	if (mpi->ibdev)
		mlx5_ib_unbind_slave_port(mpi->ibdev, mpi);
	else
		list_del(&mpi->list);
	mutex_unlock(&mlx5_ib_multiport_mutex);
	kfree(mpi);
}

static int mlx5r_probe(struct auxiliary_device *adev,
		       const struct auxiliary_device_id *id)
{
	struct mlx5_adev *idev = container_of(adev, struct mlx5_adev, adev);
	struct mlx5_core_dev *mdev = idev->mdev;
	const struct mlx5_ib_profile *profile;
	int port_type_cap, num_ports, ret;
	enum rdma_link_layer ll;
	struct mlx5_ib_dev *dev;
>>>>>>> upstream/android-13

	port_type_cap = MLX5_CAP_GEN(mdev, port_type);
	ll = mlx5_port_type_cap_to_rdma_ll(port_type_cap);

<<<<<<< HEAD
	if (mlx5_core_is_mp_slave(mdev) && ll == IB_LINK_LAYER_ETHERNET)
		return mlx5_ib_add_slave_port(mdev);

	dev = (struct mlx5_ib_dev *)ib_alloc_device(sizeof(*dev));
	if (!dev)
		return NULL;

	dev->mdev = mdev;
	dev->num_ports = max(MLX5_CAP_GEN(mdev, num_ports),
			     MLX5_CAP_GEN(mdev, num_vhca_ports));

	if (MLX5_ESWITCH_MANAGER(mdev) &&
	    mlx5_ib_eswitch_mode(mdev->priv.eswitch) == SRIOV_OFFLOADS) {
		dev->rep = mlx5_ib_vport_rep(mdev->priv.eswitch, 0);

		return __mlx5_ib_add(dev, &nic_rep_profile);
	}

	return __mlx5_ib_add(dev, &pf_profile);
}

static void mlx5_ib_remove(struct mlx5_core_dev *mdev, void *context)
{
	struct mlx5_ib_multiport_info *mpi;
	struct mlx5_ib_dev *dev;

	if (mlx5_core_is_mp_slave(mdev)) {
		mpi = context;
		mutex_lock(&mlx5_ib_multiport_mutex);
		if (mpi->ibdev)
			mlx5_ib_unbind_slave_port(mpi->ibdev, mpi);
		list_del(&mpi->list);
		mutex_unlock(&mlx5_ib_multiport_mutex);
		kfree(mpi);
		return;
	}

	dev = context;
	__mlx5_ib_remove(dev, dev->profile, MLX5_IB_STAGE_MAX);
}

static struct mlx5_interface mlx5_ib_interface = {
	.add            = mlx5_ib_add,
	.remove         = mlx5_ib_remove,
	.event          = mlx5_ib_event,
#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
	.pfault		= mlx5_ib_pfault,
#endif
	.protocol	= MLX5_INTERFACE_PROTOCOL_IB,
};

unsigned long mlx5_ib_get_xlt_emergency_page(void)
{
	mutex_lock(&xlt_emergency_page_mutex);
	return xlt_emergency_page;
}

void mlx5_ib_put_xlt_emergency_page(void)
{
	mutex_unlock(&xlt_emergency_page_mutex);
}

static int __init mlx5_ib_init(void)
{
	int err;

	xlt_emergency_page = __get_free_page(GFP_KERNEL);
	if (!xlt_emergency_page)
		return -ENOMEM;

	mutex_init(&xlt_emergency_page_mutex);

	mlx5_ib_event_wq = alloc_ordered_workqueue("mlx5_ib_event_wq", 0);
	if (!mlx5_ib_event_wq) {
		free_page(xlt_emergency_page);
=======
	num_ports = max(MLX5_CAP_GEN(mdev, num_ports),
			MLX5_CAP_GEN(mdev, num_vhca_ports));
	dev = ib_alloc_device(mlx5_ib_dev, ib_dev);
	if (!dev)
		return -ENOMEM;
	dev->port = kcalloc(num_ports, sizeof(*dev->port),
			     GFP_KERNEL);
	if (!dev->port) {
		ib_dealloc_device(&dev->ib_dev);
		return -ENOMEM;
	}

	dev->mdev = mdev;
	dev->num_ports = num_ports;

	if (ll == IB_LINK_LAYER_ETHERNET && !mlx5_is_roce_init_enabled(mdev))
		profile = &raw_eth_profile;
	else
		profile = &pf_profile;

	ret = __mlx5_ib_add(dev, profile);
	if (ret) {
		kfree(dev->port);
		ib_dealloc_device(&dev->ib_dev);
		return ret;
	}

	dev_set_drvdata(&adev->dev, dev);
	return 0;
}

static void mlx5r_remove(struct auxiliary_device *adev)
{
	struct mlx5_ib_dev *dev;

	dev = dev_get_drvdata(&adev->dev);
	__mlx5_ib_remove(dev, dev->profile, MLX5_IB_STAGE_MAX);
}

static const struct auxiliary_device_id mlx5r_mp_id_table[] = {
	{ .name = MLX5_ADEV_NAME ".multiport", },
	{},
};

static const struct auxiliary_device_id mlx5r_id_table[] = {
	{ .name = MLX5_ADEV_NAME ".rdma", },
	{},
};

MODULE_DEVICE_TABLE(auxiliary, mlx5r_mp_id_table);
MODULE_DEVICE_TABLE(auxiliary, mlx5r_id_table);

static struct auxiliary_driver mlx5r_mp_driver = {
	.name = "multiport",
	.probe = mlx5r_mp_probe,
	.remove = mlx5r_mp_remove,
	.id_table = mlx5r_mp_id_table,
};

static struct auxiliary_driver mlx5r_driver = {
	.name = "rdma",
	.probe = mlx5r_probe,
	.remove = mlx5r_remove,
	.id_table = mlx5r_id_table,
};

static int __init mlx5_ib_init(void)
{
	int ret;

	xlt_emergency_page = (void *)__get_free_page(GFP_KERNEL);
	if (!xlt_emergency_page)
		return -ENOMEM;

	mlx5_ib_event_wq = alloc_ordered_workqueue("mlx5_ib_event_wq", 0);
	if (!mlx5_ib_event_wq) {
		free_page((unsigned long)xlt_emergency_page);
>>>>>>> upstream/android-13
		return -ENOMEM;
	}

	mlx5_ib_odp_init();
<<<<<<< HEAD

	err = mlx5_register_interface(&mlx5_ib_interface);

	return err;
=======
	ret = mlx5r_rep_init();
	if (ret)
		goto rep_err;
	ret = auxiliary_driver_register(&mlx5r_mp_driver);
	if (ret)
		goto mp_err;
	ret = auxiliary_driver_register(&mlx5r_driver);
	if (ret)
		goto drv_err;
	return 0;

drv_err:
	auxiliary_driver_unregister(&mlx5r_mp_driver);
mp_err:
	mlx5r_rep_cleanup();
rep_err:
	destroy_workqueue(mlx5_ib_event_wq);
	free_page((unsigned long)xlt_emergency_page);
	return ret;
>>>>>>> upstream/android-13
}

static void __exit mlx5_ib_cleanup(void)
{
<<<<<<< HEAD
	mlx5_unregister_interface(&mlx5_ib_interface);
	destroy_workqueue(mlx5_ib_event_wq);
	mutex_destroy(&xlt_emergency_page_mutex);
	free_page(xlt_emergency_page);
=======
	auxiliary_driver_unregister(&mlx5r_driver);
	auxiliary_driver_unregister(&mlx5r_mp_driver);
	mlx5r_rep_cleanup();

	destroy_workqueue(mlx5_ib_event_wq);
	free_page((unsigned long)xlt_emergency_page);
>>>>>>> upstream/android-13
}

module_init(mlx5_ib_init);
module_exit(mlx5_ib_cleanup);
