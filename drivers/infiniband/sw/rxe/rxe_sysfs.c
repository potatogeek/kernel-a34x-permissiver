<<<<<<< HEAD
/*
 * Copyright (c) 2016 Mellanox Technologies Ltd. All rights reserved.
 * Copyright (c) 2015 System Fabric Works, Inc. All rights reserved.
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
 *	- Redistributions of source code must retain the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer.
 *
 *	- Redistributions in binary form must reproduce the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer in the documentation and/or other materials
 *	  provided with the distribution.
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
 * Copyright (c) 2016 Mellanox Technologies Ltd. All rights reserved.
 * Copyright (c) 2015 System Fabric Works, Inc. All rights reserved.
>>>>>>> upstream/android-13
 */

#include "rxe.h"
#include "rxe_net.h"

/* Copy argument and remove trailing CR. Return the new length. */
static int sanitize_arg(const char *val, char *intf, int intf_len)
{
	int len;

	if (!val)
		return 0;

	/* Remove newline. */
	for (len = 0; len < intf_len - 1 && val[len] && val[len] != '\n'; len++)
		intf[len] = val[len];
	intf[len] = 0;

	if (len == 0 || (val[len] != 0 && val[len] != '\n'))
		return 0;

	return len;
}

<<<<<<< HEAD
static void rxe_set_port_state(struct net_device *ndev)
{
	struct rxe_dev *rxe = net_to_rxe(ndev);
	bool is_up = netif_running(ndev) && netif_carrier_ok(ndev);

	if (!rxe)
		goto out;

	if (is_up)
		rxe_port_up(rxe);
	else
		rxe_port_down(rxe); /* down for unknown state */
out:
	return;
}

=======
>>>>>>> upstream/android-13
static int rxe_param_set_add(const char *val, const struct kernel_param *kp)
{
	int len;
	int err = 0;
	char intf[32];
<<<<<<< HEAD
	struct net_device *ndev = NULL;
	struct rxe_dev *rxe;
=======
	struct net_device *ndev;
	struct rxe_dev *exists;

	if (!rxe_initialized) {
		pr_err("Module parameters are not supported, use rdma link add or rxe_cfg\n");
		return -EAGAIN;
	}
>>>>>>> upstream/android-13

	len = sanitize_arg(val, intf, sizeof(intf));
	if (!len) {
		pr_err("add: invalid interface name\n");
<<<<<<< HEAD
		err = -EINVAL;
		goto err;
=======
		return -EINVAL;
>>>>>>> upstream/android-13
	}

	ndev = dev_get_by_name(&init_net, intf);
	if (!ndev) {
		pr_err("interface %s not found\n", intf);
<<<<<<< HEAD
		err = -EINVAL;
		goto err;
	}

	if (net_to_rxe(ndev)) {
=======
		return -EINVAL;
	}

	if (is_vlan_dev(ndev)) {
		pr_err("rxe creation allowed on top of a real device only\n");
		err = -EPERM;
		goto err;
	}

	exists = rxe_get_dev_from_net(ndev);
	if (exists) {
		ib_device_put(&exists->ib_dev);
>>>>>>> upstream/android-13
		pr_err("already configured on %s\n", intf);
		err = -EINVAL;
		goto err;
	}

<<<<<<< HEAD
	rxe = rxe_net_add(ndev);
	if (!rxe) {
		pr_err("failed to add %s\n", intf);
		err = -EINVAL;
		goto err;
	}

	rxe_set_port_state(ndev);
	pr_info("added %s to %s\n", rxe->ib_dev.name, intf);
err:
	if (ndev)
		dev_put(ndev);
=======
	err = rxe_net_add("rxe%d", ndev);
	if (err) {
		pr_err("failed to add %s\n", intf);
		goto err;
	}

err:
	dev_put(ndev);
>>>>>>> upstream/android-13
	return err;
}

static int rxe_param_set_remove(const char *val, const struct kernel_param *kp)
{
	int len;
	char intf[32];
<<<<<<< HEAD
	struct rxe_dev *rxe;
=======
	struct ib_device *ib_dev;
>>>>>>> upstream/android-13

	len = sanitize_arg(val, intf, sizeof(intf));
	if (!len) {
		pr_err("add: invalid interface name\n");
		return -EINVAL;
	}

	if (strncmp("all", intf, len) == 0) {
		pr_info("rxe_sys: remove all");
<<<<<<< HEAD
		rxe_remove_all();
		return 0;
	}

	rxe = get_rxe_by_name(intf);

	if (!rxe) {
=======
		ib_unregister_driver(RDMA_DRIVER_RXE);
		return 0;
	}

	ib_dev = ib_device_get_by_name(intf, RDMA_DRIVER_RXE);
	if (!ib_dev) {
>>>>>>> upstream/android-13
		pr_err("not configured on %s\n", intf);
		return -EINVAL;
	}

<<<<<<< HEAD
	list_del(&rxe->list);
	rxe_remove(rxe);
=======
	ib_unregister_device_and_put(ib_dev);
>>>>>>> upstream/android-13

	return 0;
}

static const struct kernel_param_ops rxe_add_ops = {
	.set = rxe_param_set_add,
};

static const struct kernel_param_ops rxe_remove_ops = {
	.set = rxe_param_set_remove,
};

module_param_cb(add, &rxe_add_ops, NULL, 0200);
<<<<<<< HEAD
MODULE_PARM_DESC(add, "Create RXE device over network interface");
module_param_cb(remove, &rxe_remove_ops, NULL, 0200);
MODULE_PARM_DESC(remove, "Remove RXE device over network interface");
=======
MODULE_PARM_DESC(add, "DEPRECATED.  Create RXE device over network interface");
module_param_cb(remove, &rxe_remove_ops, NULL, 0200);
MODULE_PARM_DESC(remove, "DEPRECATED.  Remove RXE device over network interface");
>>>>>>> upstream/android-13
