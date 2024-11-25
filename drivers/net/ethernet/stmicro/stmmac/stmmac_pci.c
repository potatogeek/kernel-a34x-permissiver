<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*******************************************************************************
  This contains the functions to handle the pci driver.

  Copyright (C) 2011-2012  Vayavya Labs Pvt Ltd

<<<<<<< HEAD
  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".
=======
>>>>>>> upstream/android-13

  Author: Rayagond Kokatanur <rayagond@vayavyalabs.com>
  Author: Giuseppe Cavallaro <peppe.cavallaro@st.com>
*******************************************************************************/

<<<<<<< HEAD
=======
#include <linux/clk-provider.h>
>>>>>>> upstream/android-13
#include <linux/pci.h>
#include <linux/dmi.h>

#include "stmmac.h"

<<<<<<< HEAD
/*
 * This struct is used to associate PCI Function of MAC controller on a board,
 * discovered via DMI, with the address of PHY connected to the MAC. The
 * negative value of the address means that MAC controller is not connected
 * with PHY.
 */
struct stmmac_pci_func_data {
	unsigned int func;
	int phy_addr;
};

struct stmmac_pci_dmi_data {
	const struct stmmac_pci_func_data *func;
	size_t nfuncs;
};

=======
>>>>>>> upstream/android-13
struct stmmac_pci_info {
	int (*setup)(struct pci_dev *pdev, struct plat_stmmacenet_data *plat);
};

<<<<<<< HEAD
static int stmmac_pci_find_phy_addr(struct pci_dev *pdev,
				    const struct dmi_system_id *dmi_list)
{
	const struct stmmac_pci_func_data *func_data;
	const struct stmmac_pci_dmi_data *dmi_data;
	const struct dmi_system_id *dmi_id;
	int func = PCI_FUNC(pdev->devfn);
	size_t n;

	dmi_id = dmi_first_match(dmi_list);
	if (!dmi_id)
		return -ENODEV;

	dmi_data = dmi_id->driver_data;
	func_data = dmi_data->func;

	for (n = 0; n < dmi_data->nfuncs; n++, func_data++)
		if (func_data->func == func)
			return func_data->phy_addr;

	return -ENODEV;
}

=======
>>>>>>> upstream/android-13
static void common_default_data(struct plat_stmmacenet_data *plat)
{
	plat->clk_csr = 2;	/* clk_csr_i = 20-35MHz & MDC = clk_csr_i/16 */
	plat->has_gmac = 1;
	plat->force_sf_dma_mode = 1;

<<<<<<< HEAD
	plat->mdio_bus_data->phy_reset = NULL;
	plat->mdio_bus_data->phy_mask = 0;
=======
	plat->mdio_bus_data->needs_reset = true;
>>>>>>> upstream/android-13

	/* Set default value for multicast hash bins */
	plat->multicast_filter_bins = HASH_TABLE_SIZE;

	/* Set default value for unicast filter entries */
	plat->unicast_filter_entries = 1;

	/* Set the maxmtu to a default of JUMBO_LEN */
	plat->maxmtu = JUMBO_LEN;

	/* Set default number of RX and TX queues to use */
	plat->tx_queues_to_use = 1;
	plat->rx_queues_to_use = 1;

	/* Disable Priority config by default */
	plat->tx_queues_cfg[0].use_prio = false;
	plat->rx_queues_cfg[0].use_prio = false;

	/* Disable RX queues routing by default */
	plat->rx_queues_cfg[0].pkt_route = 0x0;
}

static int stmmac_default_data(struct pci_dev *pdev,
			       struct plat_stmmacenet_data *plat)
{
	/* Set common default data first */
	common_default_data(plat);

	plat->bus_id = 1;
	plat->phy_addr = 0;
<<<<<<< HEAD
	plat->interface = PHY_INTERFACE_MODE_GMII;
=======
	plat->phy_interface = PHY_INTERFACE_MODE_GMII;
>>>>>>> upstream/android-13

	plat->dma_cfg->pbl = 32;
	plat->dma_cfg->pblx8 = true;
	/* TODO: AXI */

	return 0;
}

static const struct stmmac_pci_info stmmac_pci_info = {
	.setup = stmmac_default_data,
};

<<<<<<< HEAD
static const struct stmmac_pci_func_data galileo_stmmac_func_data[] = {
	{
		.func = 6,
		.phy_addr = 1,
	},
};

static const struct stmmac_pci_dmi_data galileo_stmmac_dmi_data = {
	.func = galileo_stmmac_func_data,
	.nfuncs = ARRAY_SIZE(galileo_stmmac_func_data),
};

static const struct stmmac_pci_func_data iot2040_stmmac_func_data[] = {
	{
		.func = 6,
		.phy_addr = 1,
	},
	{
		.func = 7,
		.phy_addr = 1,
	},
};

static const struct stmmac_pci_dmi_data iot2040_stmmac_dmi_data = {
	.func = iot2040_stmmac_func_data,
	.nfuncs = ARRAY_SIZE(iot2040_stmmac_func_data),
};

static const struct dmi_system_id quark_pci_dmi[] = {
	{
		.matches = {
			DMI_EXACT_MATCH(DMI_BOARD_NAME, "Galileo"),
		},
		.driver_data = (void *)&galileo_stmmac_dmi_data,
	},
	{
		.matches = {
			DMI_EXACT_MATCH(DMI_BOARD_NAME, "GalileoGen2"),
		},
		.driver_data = (void *)&galileo_stmmac_dmi_data,
	},
	{
		.matches = {
			DMI_EXACT_MATCH(DMI_BOARD_NAME, "SIMATIC IOT2000"),
			DMI_EXACT_MATCH(DMI_BOARD_ASSET_TAG,
					"6ES7647-0AA00-0YA2"),
		},
		.driver_data = (void *)&galileo_stmmac_dmi_data,
	},
	{
		.matches = {
			DMI_EXACT_MATCH(DMI_BOARD_NAME, "SIMATIC IOT2000"),
			DMI_EXACT_MATCH(DMI_BOARD_ASSET_TAG,
					"6ES7647-0AA00-1YA2"),
		},
		.driver_data = (void *)&iot2040_stmmac_dmi_data,
	},
	{}
};

static int quark_default_data(struct pci_dev *pdev,
			      struct plat_stmmacenet_data *plat)
{
	int ret;

	/* Set common default data first */
	common_default_data(plat);

	/*
	 * Refuse to load the driver and register net device if MAC controller
	 * does not connect to any PHY interface.
	 */
	ret = stmmac_pci_find_phy_addr(pdev, quark_pci_dmi);
	if (ret < 0) {
		/* Return error to the caller on DMI enabled boards. */
		if (dmi_get_system_info(DMI_BOARD_NAME))
			return ret;

		/*
		 * Galileo boards with old firmware don't support DMI. We always
		 * use 1 here as PHY address, so at least the first found MAC
		 * controller would be probed.
		 */
		ret = 1;
	}

	plat->bus_id = PCI_DEVID(pdev->bus->number, pdev->devfn);
	plat->phy_addr = ret;
	plat->interface = PHY_INTERFACE_MODE_RMII;

	plat->dma_cfg->pbl = 16;
	plat->dma_cfg->pblx8 = true;
	plat->dma_cfg->fixed_burst = 1;
	/* AXI (TODO) */
=======
static int snps_gmac5_default_data(struct pci_dev *pdev,
				   struct plat_stmmacenet_data *plat)
{
	int i;

	plat->clk_csr = 5;
	plat->has_gmac4 = 1;
	plat->force_sf_dma_mode = 1;
	plat->tso_en = 1;
	plat->pmt = 1;

	/* Set default value for multicast hash bins */
	plat->multicast_filter_bins = HASH_TABLE_SIZE;

	/* Set default value for unicast filter entries */
	plat->unicast_filter_entries = 1;

	/* Set the maxmtu to a default of JUMBO_LEN */
	plat->maxmtu = JUMBO_LEN;

	/* Set default number of RX and TX queues to use */
	plat->tx_queues_to_use = 4;
	plat->rx_queues_to_use = 4;

	plat->tx_sched_algorithm = MTL_TX_ALGORITHM_WRR;
	for (i = 0; i < plat->tx_queues_to_use; i++) {
		plat->tx_queues_cfg[i].use_prio = false;
		plat->tx_queues_cfg[i].mode_to_use = MTL_QUEUE_DCB;
		plat->tx_queues_cfg[i].weight = 25;
		if (i > 0)
			plat->tx_queues_cfg[i].tbs_en = 1;
	}

	plat->rx_sched_algorithm = MTL_RX_ALGORITHM_SP;
	for (i = 0; i < plat->rx_queues_to_use; i++) {
		plat->rx_queues_cfg[i].use_prio = false;
		plat->rx_queues_cfg[i].mode_to_use = MTL_QUEUE_DCB;
		plat->rx_queues_cfg[i].pkt_route = 0x0;
		plat->rx_queues_cfg[i].chan = i;
	}

	plat->bus_id = 1;
	plat->phy_addr = -1;
	plat->phy_interface = PHY_INTERFACE_MODE_GMII;

	plat->dma_cfg->pbl = 32;
	plat->dma_cfg->pblx8 = true;

	/* Axi Configuration */
	plat->axi = devm_kzalloc(&pdev->dev, sizeof(*plat->axi), GFP_KERNEL);
	if (!plat->axi)
		return -ENOMEM;

	plat->axi->axi_wr_osr_lmt = 31;
	plat->axi->axi_rd_osr_lmt = 31;

	plat->axi->axi_fb = false;
	plat->axi->axi_blen[0] = 4;
	plat->axi->axi_blen[1] = 8;
	plat->axi->axi_blen[2] = 16;
	plat->axi->axi_blen[3] = 32;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static const struct stmmac_pci_info quark_pci_info = {
	.setup = quark_default_data,
=======
static const struct stmmac_pci_info snps_gmac5_pci_info = {
	.setup = snps_gmac5_default_data,
>>>>>>> upstream/android-13
};

/**
 * stmmac_pci_probe
 *
 * @pdev: pci device pointer
 * @id: pointer to table of device id/id's.
 *
 * Description: This probing function gets called for all PCI devices which
 * match the ID table and are not "owned" by other driver yet. This function
 * gets passed a "struct pci_dev *" for each device whose entry in the ID table
 * matches the device. The probe functions returns zero when the driver choose
 * to take "ownership" of the device or an error code(-ve no) otherwise.
 */
static int stmmac_pci_probe(struct pci_dev *pdev,
			    const struct pci_device_id *id)
{
	struct stmmac_pci_info *info = (struct stmmac_pci_info *)id->driver_data;
	struct plat_stmmacenet_data *plat;
	struct stmmac_resources res;
	int i;
	int ret;

	plat = devm_kzalloc(&pdev->dev, sizeof(*plat), GFP_KERNEL);
	if (!plat)
		return -ENOMEM;

	plat->mdio_bus_data = devm_kzalloc(&pdev->dev,
					   sizeof(*plat->mdio_bus_data),
					   GFP_KERNEL);
	if (!plat->mdio_bus_data)
		return -ENOMEM;

	plat->dma_cfg = devm_kzalloc(&pdev->dev, sizeof(*plat->dma_cfg),
				     GFP_KERNEL);
	if (!plat->dma_cfg)
		return -ENOMEM;

<<<<<<< HEAD
=======
	plat->safety_feat_cfg = devm_kzalloc(&pdev->dev,
					     sizeof(*plat->safety_feat_cfg),
					     GFP_KERNEL);
	if (!plat->safety_feat_cfg)
		return -ENOMEM;

>>>>>>> upstream/android-13
	/* Enable pci device */
	ret = pci_enable_device(pdev);
	if (ret) {
		dev_err(&pdev->dev, "%s: ERROR: failed to enable device\n",
			__func__);
		return ret;
	}

	/* Get the base address of device */
<<<<<<< HEAD
	for (i = 0; i <= PCI_STD_RESOURCE_END; i++) {
=======
	for (i = 0; i < PCI_STD_NUM_BARS; i++) {
>>>>>>> upstream/android-13
		if (pci_resource_len(pdev, i) == 0)
			continue;
		ret = pcim_iomap_regions(pdev, BIT(i), pci_name(pdev));
		if (ret)
			return ret;
		break;
	}

	pci_set_master(pdev);

	ret = info->setup(pdev, plat);
	if (ret)
		return ret;

<<<<<<< HEAD
	pci_enable_msi(pdev);

=======
>>>>>>> upstream/android-13
	memset(&res, 0, sizeof(res));
	res.addr = pcim_iomap_table(pdev)[i];
	res.wol_irq = pdev->irq;
	res.irq = pdev->irq;

<<<<<<< HEAD
=======
	plat->safety_feat_cfg->tsoee = 1;
	plat->safety_feat_cfg->mrxpee = 1;
	plat->safety_feat_cfg->mestee = 1;
	plat->safety_feat_cfg->mrxee = 1;
	plat->safety_feat_cfg->mtxee = 1;
	plat->safety_feat_cfg->epsi = 1;
	plat->safety_feat_cfg->edpp = 1;
	plat->safety_feat_cfg->prtyen = 1;
	plat->safety_feat_cfg->tmouten = 1;

>>>>>>> upstream/android-13
	return stmmac_dvr_probe(&pdev->dev, plat, &res);
}

/**
 * stmmac_pci_remove
 *
 * @pdev: platform device pointer
 * Description: this function calls the main to free the net resources
 * and releases the PCI resources.
 */
static void stmmac_pci_remove(struct pci_dev *pdev)
{
<<<<<<< HEAD
	stmmac_dvr_remove(&pdev->dev);
=======
	int i;

	stmmac_dvr_remove(&pdev->dev);

	for (i = 0; i < PCI_STD_NUM_BARS; i++) {
		if (pci_resource_len(pdev, i) == 0)
			continue;
		pcim_iounmap_regions(pdev, BIT(i));
		break;
	}

>>>>>>> upstream/android-13
	pci_disable_device(pdev);
}

static int __maybe_unused stmmac_pci_suspend(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	int ret;

	ret = stmmac_suspend(dev);
	if (ret)
		return ret;

	ret = pci_save_state(pdev);
	if (ret)
		return ret;

	pci_disable_device(pdev);
	pci_wake_from_d3(pdev, true);
	return 0;
}

static int __maybe_unused stmmac_pci_resume(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	int ret;

	pci_restore_state(pdev);
	pci_set_power_state(pdev, PCI_D0);

	ret = pci_enable_device(pdev);
	if (ret)
		return ret;

	pci_set_master(pdev);

	return stmmac_resume(dev);
}

static SIMPLE_DEV_PM_OPS(stmmac_pm_ops, stmmac_pci_suspend, stmmac_pci_resume);

/* synthetic ID, no official vendor */
<<<<<<< HEAD
#define PCI_VENDOR_ID_STMMAC 0x700

#define STMMAC_QUARK_ID  0x0937
#define STMMAC_DEVICE_ID 0x1108

#define STMMAC_DEVICE(vendor_id, dev_id, info)	{	\
	PCI_VDEVICE(vendor_id, dev_id),			\
	.driver_data = (kernel_ulong_t)&info		\
	}

static const struct pci_device_id stmmac_id_table[] = {
	STMMAC_DEVICE(STMMAC, STMMAC_DEVICE_ID, stmmac_pci_info),
	STMMAC_DEVICE(STMICRO, PCI_DEVICE_ID_STMICRO_MAC, stmmac_pci_info),
	STMMAC_DEVICE(INTEL, STMMAC_QUARK_ID, quark_pci_info),
=======
#define PCI_VENDOR_ID_STMMAC		0x0700

#define PCI_DEVICE_ID_STMMAC_STMMAC		0x1108
#define PCI_DEVICE_ID_SYNOPSYS_GMAC5_ID		0x7102

static const struct pci_device_id stmmac_id_table[] = {
	{ PCI_DEVICE_DATA(STMMAC, STMMAC, &stmmac_pci_info) },
	{ PCI_DEVICE_DATA(STMICRO, MAC, &stmmac_pci_info) },
	{ PCI_DEVICE_DATA(SYNOPSYS, GMAC5_ID, &snps_gmac5_pci_info) },
>>>>>>> upstream/android-13
	{}
};

MODULE_DEVICE_TABLE(pci, stmmac_id_table);

static struct pci_driver stmmac_pci_driver = {
	.name = STMMAC_RESOURCE_NAME,
	.id_table = stmmac_id_table,
	.probe = stmmac_pci_probe,
	.remove = stmmac_pci_remove,
	.driver         = {
		.pm     = &stmmac_pm_ops,
	},
};

module_pci_driver(stmmac_pci_driver);

MODULE_DESCRIPTION("STMMAC 10/100/1000 Ethernet PCI driver");
MODULE_AUTHOR("Rayagond Kokatanur <rayagond.kokatanur@vayavyalabs.com>");
MODULE_AUTHOR("Giuseppe Cavallaro <peppe.cavallaro@st.com>");
MODULE_LICENSE("GPL");
