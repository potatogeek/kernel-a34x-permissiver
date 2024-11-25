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

#include <linux/mlx5/driver.h>
<<<<<<< HEAD
#include "mlx5_core.h"

static LIST_HEAD(intf_list);
static LIST_HEAD(mlx5_dev_list);
/* intf dev list mutex */
static DEFINE_MUTEX(mlx5_intf_mutex);

struct mlx5_device_context {
	struct list_head	list;
	struct mlx5_interface  *intf;
	void		       *context;
	unsigned long		state;
};

struct mlx5_delayed_event {
	struct list_head	list;
	struct mlx5_core_dev	*dev;
	enum mlx5_dev_event	event;
	unsigned long		param;
};

enum {
	MLX5_INTERFACE_ADDED,
	MLX5_INTERFACE_ATTACHED,
};

static void add_delayed_event(struct mlx5_priv *priv,
			      struct mlx5_core_dev *dev,
			      enum mlx5_dev_event event,
			      unsigned long param)
{
	struct mlx5_delayed_event *delayed_event;

	delayed_event = kzalloc(sizeof(*delayed_event), GFP_ATOMIC);
	if (!delayed_event) {
		mlx5_core_err(dev, "event %d is missed\n", event);
		return;
	}

	mlx5_core_dbg(dev, "Accumulating event %d\n", event);
	delayed_event->dev = dev;
	delayed_event->event = event;
	delayed_event->param = param;
	list_add_tail(&delayed_event->list, &priv->waiting_events_list);
}

static void delayed_event_release(struct mlx5_device_context *dev_ctx,
				  struct mlx5_priv *priv)
{
	struct mlx5_core_dev *dev = container_of(priv, struct mlx5_core_dev, priv);
	struct mlx5_delayed_event *de;
	struct mlx5_delayed_event *n;
	struct list_head temp;

	INIT_LIST_HEAD(&temp);

	spin_lock_irq(&priv->ctx_lock);

	priv->is_accum_events = false;
	list_splice_init(&priv->waiting_events_list, &temp);
	if (!dev_ctx->context)
		goto out;
	list_for_each_entry_safe(de, n, &temp, list)
		dev_ctx->intf->event(dev, dev_ctx->context, de->event, de->param);

out:
	spin_unlock_irq(&priv->ctx_lock);

	list_for_each_entry_safe(de, n, &temp, list) {
		list_del(&de->list);
		kfree(de);
	}
}

/* accumulating events that can come after mlx5_ib calls to
 * ib_register_device, till adding that interface to the events list.
 */
static void delayed_event_start(struct mlx5_priv *priv)
{
	spin_lock_irq(&priv->ctx_lock);
	priv->is_accum_events = true;
	spin_unlock_irq(&priv->ctx_lock);
}

void mlx5_add_device(struct mlx5_interface *intf, struct mlx5_priv *priv)
{
	struct mlx5_device_context *dev_ctx;
	struct mlx5_core_dev *dev = container_of(priv, struct mlx5_core_dev, priv);

	if (!mlx5_lag_intf_add(intf, priv))
		return;

	dev_ctx = kzalloc(sizeof(*dev_ctx), GFP_KERNEL);
	if (!dev_ctx)
		return;

	dev_ctx->intf = intf;

	delayed_event_start(priv);

	dev_ctx->context = intf->add(dev);
	if (dev_ctx->context) {
		set_bit(MLX5_INTERFACE_ADDED, &dev_ctx->state);
		if (intf->attach)
			set_bit(MLX5_INTERFACE_ATTACHED, &dev_ctx->state);

		spin_lock_irq(&priv->ctx_lock);
		list_add_tail(&dev_ctx->list, &priv->ctx_list);

#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
		if (dev_ctx->intf->pfault) {
			if (priv->pfault) {
				mlx5_core_err(dev, "multiple page fault handlers not supported");
			} else {
				priv->pfault_ctx = dev_ctx->context;
				priv->pfault = dev_ctx->intf->pfault;
			}
		}
#endif
		spin_unlock_irq(&priv->ctx_lock);
	}

	delayed_event_release(dev_ctx, priv);

	if (!dev_ctx->context)
		kfree(dev_ctx);
}

static struct mlx5_device_context *mlx5_get_device(struct mlx5_interface *intf,
						   struct mlx5_priv *priv)
{
	struct mlx5_device_context *dev_ctx;

	list_for_each_entry(dev_ctx, &priv->ctx_list, list)
		if (dev_ctx->intf == intf)
			return dev_ctx;
	return NULL;
}

void mlx5_remove_device(struct mlx5_interface *intf, struct mlx5_priv *priv)
{
	struct mlx5_device_context *dev_ctx;
	struct mlx5_core_dev *dev = container_of(priv, struct mlx5_core_dev, priv);

	dev_ctx = mlx5_get_device(intf, priv);
	if (!dev_ctx)
		return;

#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
	spin_lock_irq(&priv->ctx_lock);
	if (priv->pfault == dev_ctx->intf->pfault)
		priv->pfault = NULL;
	spin_unlock_irq(&priv->ctx_lock);

	synchronize_srcu(&priv->pfault_srcu);
#endif

	spin_lock_irq(&priv->ctx_lock);
	list_del(&dev_ctx->list);
	spin_unlock_irq(&priv->ctx_lock);

	if (test_bit(MLX5_INTERFACE_ADDED, &dev_ctx->state))
		intf->remove(dev, dev_ctx->context);

	kfree(dev_ctx);
}

static void mlx5_attach_interface(struct mlx5_interface *intf, struct mlx5_priv *priv)
{
	struct mlx5_device_context *dev_ctx;
	struct mlx5_core_dev *dev = container_of(priv, struct mlx5_core_dev, priv);

	dev_ctx = mlx5_get_device(intf, priv);
	if (!dev_ctx)
		return;

	delayed_event_start(priv);
	if (intf->attach) {
		if (test_bit(MLX5_INTERFACE_ATTACHED, &dev_ctx->state))
			goto out;
		if (intf->attach(dev, dev_ctx->context))
			goto out;

		set_bit(MLX5_INTERFACE_ATTACHED, &dev_ctx->state);
	} else {
		if (test_bit(MLX5_INTERFACE_ADDED, &dev_ctx->state))
			goto out;
		dev_ctx->context = intf->add(dev);
		if (!dev_ctx->context)
			goto out;

		set_bit(MLX5_INTERFACE_ADDED, &dev_ctx->state);
	}

out:
	delayed_event_release(dev_ctx, priv);
}

void mlx5_attach_device(struct mlx5_core_dev *dev)
{
	struct mlx5_priv *priv = &dev->priv;
	struct mlx5_interface *intf;

	mutex_lock(&mlx5_intf_mutex);
	list_for_each_entry(intf, &intf_list, list)
		mlx5_attach_interface(intf, priv);
	mutex_unlock(&mlx5_intf_mutex);
}

static void mlx5_detach_interface(struct mlx5_interface *intf, struct mlx5_priv *priv)
{
	struct mlx5_device_context *dev_ctx;
	struct mlx5_core_dev *dev = container_of(priv, struct mlx5_core_dev, priv);

	dev_ctx = mlx5_get_device(intf, priv);
	if (!dev_ctx)
		return;

	if (intf->detach) {
		if (!test_bit(MLX5_INTERFACE_ATTACHED, &dev_ctx->state))
			return;
		intf->detach(dev, dev_ctx->context);
		clear_bit(MLX5_INTERFACE_ATTACHED, &dev_ctx->state);
	} else {
		if (!test_bit(MLX5_INTERFACE_ADDED, &dev_ctx->state))
			return;
		intf->remove(dev, dev_ctx->context);
		clear_bit(MLX5_INTERFACE_ADDED, &dev_ctx->state);
	}
=======
#include <linux/mlx5/eswitch.h>
#include <linux/mlx5/mlx5_ifc_vdpa.h>
#include "mlx5_core.h"

/* intf dev list mutex */
static DEFINE_MUTEX(mlx5_intf_mutex);
static DEFINE_IDA(mlx5_adev_ida);

static bool is_eth_rep_supported(struct mlx5_core_dev *dev)
{
	if (!IS_ENABLED(CONFIG_MLX5_ESWITCH))
		return false;

	if (!MLX5_ESWITCH_MANAGER(dev))
		return false;

	if (!is_mdev_switchdev_mode(dev))
		return false;

	return true;
}

bool mlx5_eth_supported(struct mlx5_core_dev *dev)
{
	if (!IS_ENABLED(CONFIG_MLX5_CORE_EN))
		return false;

	if (MLX5_CAP_GEN(dev, port_type) != MLX5_CAP_PORT_TYPE_ETH)
		return false;

	if (!MLX5_CAP_GEN(dev, eth_net_offloads)) {
		mlx5_core_warn(dev, "Missing eth_net_offloads capability\n");
		return false;
	}

	if (!MLX5_CAP_GEN(dev, nic_flow_table)) {
		mlx5_core_warn(dev, "Missing nic_flow_table capability\n");
		return false;
	}

	if (!MLX5_CAP_ETH(dev, csum_cap)) {
		mlx5_core_warn(dev, "Missing csum_cap capability\n");
		return false;
	}

	if (!MLX5_CAP_ETH(dev, max_lso_cap)) {
		mlx5_core_warn(dev, "Missing max_lso_cap capability\n");
		return false;
	}

	if (!MLX5_CAP_ETH(dev, vlan_cap)) {
		mlx5_core_warn(dev, "Missing vlan_cap capability\n");
		return false;
	}

	if (!MLX5_CAP_ETH(dev, rss_ind_tbl_cap)) {
		mlx5_core_warn(dev, "Missing rss_ind_tbl_cap capability\n");
		return false;
	}

	if (MLX5_CAP_FLOWTABLE(dev,
			       flow_table_properties_nic_receive.max_ft_level) < 3) {
		mlx5_core_warn(dev, "max_ft_level < 3\n");
		return false;
	}

	if (!MLX5_CAP_ETH(dev, self_lb_en_modifiable))
		mlx5_core_warn(dev, "Self loop back prevention is not supported\n");
	if (!MLX5_CAP_GEN(dev, cq_moderation))
		mlx5_core_warn(dev, "CQ moderation is not supported\n");

	return true;
}

static bool is_eth_enabled(struct mlx5_core_dev *dev)
{
	union devlink_param_value val;
	int err;

	err = devlink_param_driverinit_value_get(priv_to_devlink(dev),
						 DEVLINK_PARAM_GENERIC_ID_ENABLE_ETH,
						 &val);
	return err ? false : val.vbool;
}

bool mlx5_vnet_supported(struct mlx5_core_dev *dev)
{
	if (!IS_ENABLED(CONFIG_MLX5_VDPA_NET))
		return false;

	if (mlx5_core_is_pf(dev))
		return false;

	if (!(MLX5_CAP_GEN_64(dev, general_obj_types) &
	      MLX5_GENERAL_OBJ_TYPES_CAP_VIRTIO_NET_Q))
		return false;

	if (!(MLX5_CAP_DEV_VDPA_EMULATION(dev, event_mode) &
	      MLX5_VIRTIO_Q_EVENT_MODE_QP_MODE))
		return false;

	if (!MLX5_CAP_DEV_VDPA_EMULATION(dev, eth_frame_offload_type))
		return false;

	return true;
}

static bool is_vnet_enabled(struct mlx5_core_dev *dev)
{
	union devlink_param_value val;
	int err;

	err = devlink_param_driverinit_value_get(priv_to_devlink(dev),
						 DEVLINK_PARAM_GENERIC_ID_ENABLE_VNET,
						 &val);
	return err ? false : val.vbool;
}

static bool is_ib_rep_supported(struct mlx5_core_dev *dev)
{
	if (!IS_ENABLED(CONFIG_MLX5_INFINIBAND))
		return false;

	if (dev->priv.flags & MLX5_PRIV_FLAGS_DISABLE_IB_ADEV)
		return false;

	if (!is_eth_rep_supported(dev))
		return false;

	if (!MLX5_ESWITCH_MANAGER(dev))
		return false;

	if (!is_mdev_switchdev_mode(dev))
		return false;

	if (mlx5_core_mp_enabled(dev))
		return false;

	return true;
}

static bool is_mp_supported(struct mlx5_core_dev *dev)
{
	if (!IS_ENABLED(CONFIG_MLX5_INFINIBAND))
		return false;

	if (dev->priv.flags & MLX5_PRIV_FLAGS_DISABLE_IB_ADEV)
		return false;

	if (is_ib_rep_supported(dev))
		return false;

	if (MLX5_CAP_GEN(dev, port_type) != MLX5_CAP_PORT_TYPE_ETH)
		return false;

	if (!mlx5_core_is_mp_slave(dev))
		return false;

	return true;
}

bool mlx5_rdma_supported(struct mlx5_core_dev *dev)
{
	if (!IS_ENABLED(CONFIG_MLX5_INFINIBAND))
		return false;

	if (dev->priv.flags & MLX5_PRIV_FLAGS_DISABLE_IB_ADEV)
		return false;

	if (is_ib_rep_supported(dev))
		return false;

	if (is_mp_supported(dev))
		return false;

	return true;
}

static bool is_ib_enabled(struct mlx5_core_dev *dev)
{
	union devlink_param_value val;
	int err;

	err = devlink_param_driverinit_value_get(priv_to_devlink(dev),
						 DEVLINK_PARAM_GENERIC_ID_ENABLE_RDMA,
						 &val);
	return err ? false : val.vbool;
}

enum {
	MLX5_INTERFACE_PROTOCOL_ETH,
	MLX5_INTERFACE_PROTOCOL_ETH_REP,

	MLX5_INTERFACE_PROTOCOL_IB,
	MLX5_INTERFACE_PROTOCOL_IB_REP,
	MLX5_INTERFACE_PROTOCOL_MPIB,

	MLX5_INTERFACE_PROTOCOL_VNET,
};

static const struct mlx5_adev_device {
	const char *suffix;
	bool (*is_supported)(struct mlx5_core_dev *dev);
	bool (*is_enabled)(struct mlx5_core_dev *dev);
} mlx5_adev_devices[] = {
	[MLX5_INTERFACE_PROTOCOL_VNET] = { .suffix = "vnet",
					   .is_supported = &mlx5_vnet_supported,
					   .is_enabled = &is_vnet_enabled },
	[MLX5_INTERFACE_PROTOCOL_IB] = { .suffix = "rdma",
					 .is_supported = &mlx5_rdma_supported,
					 .is_enabled = &is_ib_enabled },
	[MLX5_INTERFACE_PROTOCOL_ETH] = { .suffix = "eth",
					  .is_supported = &mlx5_eth_supported,
					  .is_enabled = &is_eth_enabled },
	[MLX5_INTERFACE_PROTOCOL_ETH_REP] = { .suffix = "eth-rep",
					   .is_supported = &is_eth_rep_supported },
	[MLX5_INTERFACE_PROTOCOL_IB_REP] = { .suffix = "rdma-rep",
					   .is_supported = &is_ib_rep_supported },
	[MLX5_INTERFACE_PROTOCOL_MPIB] = { .suffix = "multiport",
					   .is_supported = &is_mp_supported },
};

int mlx5_adev_idx_alloc(void)
{
	return ida_alloc(&mlx5_adev_ida, GFP_KERNEL);
}

void mlx5_adev_idx_free(int idx)
{
	ida_free(&mlx5_adev_ida, idx);
}

int mlx5_adev_init(struct mlx5_core_dev *dev)
{
	struct mlx5_priv *priv = &dev->priv;

	priv->adev = kcalloc(ARRAY_SIZE(mlx5_adev_devices),
			     sizeof(struct mlx5_adev *), GFP_KERNEL);
	if (!priv->adev)
		return -ENOMEM;

	return 0;
}

void mlx5_adev_cleanup(struct mlx5_core_dev *dev)
{
	struct mlx5_priv *priv = &dev->priv;

	kfree(priv->adev);
}

static void adev_release(struct device *dev)
{
	struct mlx5_adev *mlx5_adev =
		container_of(dev, struct mlx5_adev, adev.dev);
	struct mlx5_priv *priv = &mlx5_adev->mdev->priv;
	int idx = mlx5_adev->idx;

	kfree(mlx5_adev);
	priv->adev[idx] = NULL;
}

static struct mlx5_adev *add_adev(struct mlx5_core_dev *dev, int idx)
{
	const char *suffix = mlx5_adev_devices[idx].suffix;
	struct auxiliary_device *adev;
	struct mlx5_adev *madev;
	int ret;

	madev = kzalloc(sizeof(*madev), GFP_KERNEL);
	if (!madev)
		return ERR_PTR(-ENOMEM);

	adev = &madev->adev;
	adev->id = dev->priv.adev_idx;
	adev->name = suffix;
	adev->dev.parent = dev->device;
	adev->dev.release = adev_release;
	madev->mdev = dev;
	madev->idx = idx;

	ret = auxiliary_device_init(adev);
	if (ret) {
		kfree(madev);
		return ERR_PTR(ret);
	}

	ret = auxiliary_device_add(adev);
	if (ret) {
		auxiliary_device_uninit(adev);
		return ERR_PTR(ret);
	}
	return madev;
}

static void del_adev(struct auxiliary_device *adev)
{
	auxiliary_device_delete(adev);
	auxiliary_device_uninit(adev);
}

int mlx5_attach_device(struct mlx5_core_dev *dev)
{
	struct mlx5_priv *priv = &dev->priv;
	struct auxiliary_device *adev;
	struct auxiliary_driver *adrv;
	int ret = 0, i;

	mutex_lock(&mlx5_intf_mutex);
	priv->flags &= ~MLX5_PRIV_FLAGS_DETACH;
	for (i = 0; i < ARRAY_SIZE(mlx5_adev_devices); i++) {
		if (!priv->adev[i]) {
			bool is_supported = false;

			if (mlx5_adev_devices[i].is_enabled) {
				bool enabled;

				enabled = mlx5_adev_devices[i].is_enabled(dev);
				if (!enabled)
					continue;
			}

			if (mlx5_adev_devices[i].is_supported)
				is_supported = mlx5_adev_devices[i].is_supported(dev);

			if (!is_supported)
				continue;

			priv->adev[i] = add_adev(dev, i);
			if (IS_ERR(priv->adev[i])) {
				ret = PTR_ERR(priv->adev[i]);
				priv->adev[i] = NULL;
			}
		} else {
			adev = &priv->adev[i]->adev;

			/* Pay attention that this is not PCI driver that
			 * mlx5_core_dev is connected, but auxiliary driver.
			 *
			 * Here we can race of module unload with devlink
			 * reload, but we don't need to take extra lock because
			 * we are holding global mlx5_intf_mutex.
			 */
			if (!adev->dev.driver)
				continue;
			adrv = to_auxiliary_drv(adev->dev.driver);

			if (adrv->resume)
				ret = adrv->resume(adev);
		}
		if (ret) {
			mlx5_core_warn(dev, "Device[%d] (%s) failed to load\n",
				       i, mlx5_adev_devices[i].suffix);

			break;
		}
	}
	mutex_unlock(&mlx5_intf_mutex);
	return ret;
>>>>>>> upstream/android-13
}

void mlx5_detach_device(struct mlx5_core_dev *dev)
{
	struct mlx5_priv *priv = &dev->priv;
<<<<<<< HEAD
	struct mlx5_interface *intf;

	mutex_lock(&mlx5_intf_mutex);
	list_for_each_entry(intf, &intf_list, list)
		mlx5_detach_interface(intf, priv);
	mutex_unlock(&mlx5_intf_mutex);
}

bool mlx5_device_registered(struct mlx5_core_dev *dev)
{
	struct mlx5_priv *priv;
	bool found = false;

	mutex_lock(&mlx5_intf_mutex);
	list_for_each_entry(priv, &mlx5_dev_list, dev_list)
		if (priv == &dev->priv)
			found = true;
	mutex_unlock(&mlx5_intf_mutex);

	return found;
}

int mlx5_register_device(struct mlx5_core_dev *dev)
{
	struct mlx5_priv *priv = &dev->priv;
	struct mlx5_interface *intf;

	mutex_lock(&mlx5_intf_mutex);
	list_add_tail(&priv->dev_list, &mlx5_dev_list);
	list_for_each_entry(intf, &intf_list, list)
		mlx5_add_device(intf, priv);
	mutex_unlock(&mlx5_intf_mutex);

	return 0;
=======
	struct auxiliary_device *adev;
	struct auxiliary_driver *adrv;
	pm_message_t pm = {};
	int i;

	mutex_lock(&mlx5_intf_mutex);
	for (i = ARRAY_SIZE(mlx5_adev_devices) - 1; i >= 0; i--) {
		if (!priv->adev[i])
			continue;

		if (mlx5_adev_devices[i].is_enabled) {
			bool enabled;

			enabled = mlx5_adev_devices[i].is_enabled(dev);
			if (!enabled)
				goto skip_suspend;
		}

		adev = &priv->adev[i]->adev;
		/* Auxiliary driver was unbind manually through sysfs */
		if (!adev->dev.driver)
			goto skip_suspend;

		adrv = to_auxiliary_drv(adev->dev.driver);

		if (adrv->suspend) {
			adrv->suspend(adev, pm);
			continue;
		}

skip_suspend:
		del_adev(&priv->adev[i]->adev);
		priv->adev[i] = NULL;
	}
	priv->flags |= MLX5_PRIV_FLAGS_DETACH;
	mutex_unlock(&mlx5_intf_mutex);
}

int mlx5_register_device(struct mlx5_core_dev *dev)
{
	int ret;

	mutex_lock(&mlx5_intf_mutex);
	dev->priv.flags &= ~MLX5_PRIV_FLAGS_DISABLE_ALL_ADEV;
	ret = mlx5_rescan_drivers_locked(dev);
	mutex_unlock(&mlx5_intf_mutex);
	if (ret)
		mlx5_unregister_device(dev);

	return ret;
>>>>>>> upstream/android-13
}

void mlx5_unregister_device(struct mlx5_core_dev *dev)
{
<<<<<<< HEAD
	struct mlx5_priv *priv = &dev->priv;
	struct mlx5_interface *intf;

	mutex_lock(&mlx5_intf_mutex);
	list_for_each_entry_reverse(intf, &intf_list, list)
		mlx5_remove_device(intf, priv);
	list_del(&priv->dev_list);
	mutex_unlock(&mlx5_intf_mutex);
}

int mlx5_register_interface(struct mlx5_interface *intf)
{
	struct mlx5_priv *priv;

	if (!intf->add || !intf->remove)
		return -EINVAL;

	mutex_lock(&mlx5_intf_mutex);
	list_add_tail(&intf->list, &intf_list);
	list_for_each_entry(priv, &mlx5_dev_list, dev_list)
		mlx5_add_device(intf, priv);
	mutex_unlock(&mlx5_intf_mutex);

	return 0;
}
EXPORT_SYMBOL(mlx5_register_interface);

void mlx5_unregister_interface(struct mlx5_interface *intf)
{
	struct mlx5_priv *priv;

	mutex_lock(&mlx5_intf_mutex);
	list_for_each_entry(priv, &mlx5_dev_list, dev_list)
		mlx5_remove_device(intf, priv);
	list_del(&intf->list);
	mutex_unlock(&mlx5_intf_mutex);
}
EXPORT_SYMBOL(mlx5_unregister_interface);

/* Must be called with intf_mutex held */
static bool mlx5_has_added_dev_by_protocol(struct mlx5_core_dev *mdev, int protocol)
{
	struct mlx5_device_context *dev_ctx;
	struct mlx5_interface *intf;
	bool found = false;

	list_for_each_entry(intf, &intf_list, list) {
		if (intf->protocol == protocol) {
			dev_ctx = mlx5_get_device(intf, &mdev->priv);
			if (dev_ctx && test_bit(MLX5_INTERFACE_ADDED, &dev_ctx->state))
				found = true;
			break;
		}
	}

	return found;
}

void mlx5_reload_interface(struct mlx5_core_dev *mdev, int protocol)
{
	mutex_lock(&mlx5_intf_mutex);
	if (mlx5_has_added_dev_by_protocol(mdev, protocol)) {
		mlx5_remove_dev_by_protocol(mdev, protocol);
		mlx5_add_dev_by_protocol(mdev, protocol);
	}
	mutex_unlock(&mlx5_intf_mutex);
}

void *mlx5_get_protocol_dev(struct mlx5_core_dev *mdev, int protocol)
{
	struct mlx5_priv *priv = &mdev->priv;
	struct mlx5_device_context *dev_ctx;
	unsigned long flags;
	void *result = NULL;

	spin_lock_irqsave(&priv->ctx_lock, flags);

	list_for_each_entry(dev_ctx, &mdev->priv.ctx_list, list)
		if ((dev_ctx->intf->protocol == protocol) &&
		    dev_ctx->intf->get_dev) {
			result = dev_ctx->intf->get_dev(dev_ctx->context);
			break;
		}

	spin_unlock_irqrestore(&priv->ctx_lock, flags);

	return result;
}
EXPORT_SYMBOL(mlx5_get_protocol_dev);

/* Must be called with intf_mutex held */
void mlx5_add_dev_by_protocol(struct mlx5_core_dev *dev, int protocol)
{
	struct mlx5_interface *intf;

	list_for_each_entry(intf, &intf_list, list)
		if (intf->protocol == protocol) {
			mlx5_add_device(intf, &dev->priv);
			break;
		}
}

/* Must be called with intf_mutex held */
void mlx5_remove_dev_by_protocol(struct mlx5_core_dev *dev, int protocol)
{
	struct mlx5_interface *intf;

	list_for_each_entry(intf, &intf_list, list)
		if (intf->protocol == protocol) {
			mlx5_remove_device(intf, &dev->priv);
			break;
		}
}

static u32 mlx5_gen_pci_id(struct mlx5_core_dev *dev)
=======
	mutex_lock(&mlx5_intf_mutex);
	dev->priv.flags = MLX5_PRIV_FLAGS_DISABLE_ALL_ADEV;
	mlx5_rescan_drivers_locked(dev);
	mutex_unlock(&mlx5_intf_mutex);
}

static int add_drivers(struct mlx5_core_dev *dev)
{
	struct mlx5_priv *priv = &dev->priv;
	int i, ret = 0;

	for (i = 0; i < ARRAY_SIZE(mlx5_adev_devices); i++) {
		bool is_supported = false;

		if (priv->adev[i])
			continue;

		if (mlx5_adev_devices[i].is_supported)
			is_supported = mlx5_adev_devices[i].is_supported(dev);

		if (!is_supported)
			continue;

		priv->adev[i] = add_adev(dev, i);
		if (IS_ERR(priv->adev[i])) {
			mlx5_core_warn(dev, "Device[%d] (%s) failed to load\n",
				       i, mlx5_adev_devices[i].suffix);
			/* We continue to rescan drivers and leave to the caller
			 * to make decision if to release everything or continue.
			 */
			ret = PTR_ERR(priv->adev[i]);
			priv->adev[i] = NULL;
		}
	}
	return ret;
}

static void delete_drivers(struct mlx5_core_dev *dev)
{
	struct mlx5_priv *priv = &dev->priv;
	bool delete_all;
	int i;

	delete_all = priv->flags & MLX5_PRIV_FLAGS_DISABLE_ALL_ADEV;

	for (i = ARRAY_SIZE(mlx5_adev_devices) - 1; i >= 0; i--) {
		bool is_supported = false;

		if (!priv->adev[i])
			continue;

		if (mlx5_adev_devices[i].is_enabled) {
			bool enabled;

			enabled = mlx5_adev_devices[i].is_enabled(dev);
			if (!enabled)
				goto del_adev;
		}

		if (mlx5_adev_devices[i].is_supported && !delete_all)
			is_supported = mlx5_adev_devices[i].is_supported(dev);

		if (is_supported)
			continue;

del_adev:
		del_adev(&priv->adev[i]->adev);
		priv->adev[i] = NULL;
	}
}

/* This function is used after mlx5_core_dev is reconfigured.
 */
int mlx5_rescan_drivers_locked(struct mlx5_core_dev *dev)
{
	struct mlx5_priv *priv = &dev->priv;

	lockdep_assert_held(&mlx5_intf_mutex);
	if (priv->flags & MLX5_PRIV_FLAGS_DETACH)
		return 0;

	delete_drivers(dev);
	if (priv->flags & MLX5_PRIV_FLAGS_DISABLE_ALL_ADEV)
		return 0;

	return add_drivers(dev);
}

static u32 mlx5_gen_pci_id(const struct mlx5_core_dev *dev)
>>>>>>> upstream/android-13
{
	return (u32)((pci_domain_nr(dev->pdev->bus) << 16) |
		     (dev->pdev->bus->number << 8) |
		     PCI_SLOT(dev->pdev->devfn));
}

<<<<<<< HEAD
/* Must be called with intf_mutex held */
struct mlx5_core_dev *mlx5_get_next_phys_dev(struct mlx5_core_dev *dev)
{
	u32 pci_id = mlx5_gen_pci_id(dev);
	struct mlx5_core_dev *res = NULL;
	struct mlx5_core_dev *tmp_dev;
	struct mlx5_priv *priv;

	list_for_each_entry(priv, &mlx5_dev_list, dev_list) {
		tmp_dev = container_of(priv, struct mlx5_core_dev, priv);
		if ((dev != tmp_dev) && (mlx5_gen_pci_id(tmp_dev) == pci_id)) {
			res = tmp_dev;
			break;
		}
	}

	return res;
}

void mlx5_core_event(struct mlx5_core_dev *dev, enum mlx5_dev_event event,
		     unsigned long param)
{
	struct mlx5_priv *priv = &dev->priv;
	struct mlx5_device_context *dev_ctx;
	unsigned long flags;

	spin_lock_irqsave(&priv->ctx_lock, flags);

	if (priv->is_accum_events)
		add_delayed_event(priv, dev, event, param);

	/* After mlx5_detach_device, the dev_ctx->intf is still set and dev_ctx is
	 * still in priv->ctx_list. In this case, only notify the dev_ctx if its
	 * ADDED or ATTACHED bit are set.
	 */
	list_for_each_entry(dev_ctx, &priv->ctx_list, list)
		if (dev_ctx->intf->event &&
		    (test_bit(MLX5_INTERFACE_ADDED, &dev_ctx->state) ||
		     test_bit(MLX5_INTERFACE_ATTACHED, &dev_ctx->state)))
			dev_ctx->intf->event(dev, dev_ctx->context, event, param);

	spin_unlock_irqrestore(&priv->ctx_lock, flags);
}

#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING
void mlx5_core_page_fault(struct mlx5_core_dev *dev,
			  struct mlx5_pagefault *pfault)
{
	struct mlx5_priv *priv = &dev->priv;
	int srcu_idx;

	srcu_idx = srcu_read_lock(&priv->pfault_srcu);
	if (priv->pfault)
		priv->pfault(dev, priv->pfault_ctx, pfault);
	srcu_read_unlock(&priv->pfault_srcu, srcu_idx);
}
#endif

=======
static int next_phys_dev(struct device *dev, const void *data)
{
	struct mlx5_adev *madev = container_of(dev, struct mlx5_adev, adev.dev);
	struct mlx5_core_dev *mdev = madev->mdev;
	const struct mlx5_core_dev *curr = data;

	if (!mlx5_core_is_pf(mdev))
		return 0;

	if (mdev == curr)
		return 0;

	if (mlx5_gen_pci_id(mdev) != mlx5_gen_pci_id(curr))
		return 0;

	return 1;
}

/* Must be called with intf_mutex held */
struct mlx5_core_dev *mlx5_get_next_phys_dev(struct mlx5_core_dev *dev)
{
	struct auxiliary_device *adev;
	struct mlx5_adev *madev;

	if (!mlx5_core_is_pf(dev))
		return NULL;

	adev = auxiliary_find_device(NULL, dev, &next_phys_dev);
	if (!adev)
		return NULL;

	madev = container_of(adev, struct mlx5_adev, adev);
	put_device(&adev->dev);
	return madev->mdev;
}

>>>>>>> upstream/android-13
void mlx5_dev_list_lock(void)
{
	mutex_lock(&mlx5_intf_mutex);
}
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
void mlx5_dev_list_unlock(void)
{
	mutex_unlock(&mlx5_intf_mutex);
}

int mlx5_dev_list_trylock(void)
{
	return mutex_trylock(&mlx5_intf_mutex);
}
