// SPDX-License-Identifier: GPL-2.0
/*
 * Generic PCI host driver common code
 *
 * Copyright (C) 2014 ARM Limited
 *
 * Author: Will Deacon <will.deacon@arm.com>
 */

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/of_address.h>
=======
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
>>>>>>> upstream/android-13
#include <linux/of_pci.h>
#include <linux/pci-ecam.h>
#include <linux/platform_device.h>

static void gen_pci_unmap_cfg(void *ptr)
{
	pci_ecam_free((struct pci_config_window *)ptr);
}

static struct pci_config_window *gen_pci_init(struct device *dev,
<<<<<<< HEAD
		struct list_head *resources, struct pci_ecam_ops *ops)
{
	int err;
	struct resource cfgres;
	struct resource *bus_range = NULL;
	struct pci_config_window *cfg;

	/* Parse our PCI ranges and request their resources */
	err = pci_parse_request_of_pci_ranges(dev, resources, &bus_range);
	if (err)
		return ERR_PTR(err);

	err = of_address_to_resource(dev->of_node, 0, &cfgres);
	if (err) {
		dev_err(dev, "missing \"reg\" property\n");
		goto err_out;
	}

	cfg = pci_ecam_create(dev, &cfgres, bus_range, ops);
	if (IS_ERR(cfg)) {
		err = PTR_ERR(cfg);
		goto err_out;
	}

	err = devm_add_action(dev, gen_pci_unmap_cfg, cfg);
	if (err) {
		gen_pci_unmap_cfg(cfg);
		goto err_out;
	}
	return cfg;

err_out:
	pci_free_resource_list(resources);
	return ERR_PTR(err);
}

int pci_host_common_probe(struct platform_device *pdev,
			  struct pci_ecam_ops *ops)
{
	const char *type;
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct pci_host_bridge *bridge;
	struct pci_config_window *cfg;
	struct list_head resources;
	int ret;
=======
		struct pci_host_bridge *bridge, const struct pci_ecam_ops *ops)
{
	int err;
	struct resource cfgres;
	struct resource_entry *bus;
	struct pci_config_window *cfg;

	err = of_address_to_resource(dev->of_node, 0, &cfgres);
	if (err) {
		dev_err(dev, "missing \"reg\" property\n");
		return ERR_PTR(err);
	}

	bus = resource_list_first_type(&bridge->windows, IORESOURCE_BUS);
	if (!bus)
		return ERR_PTR(-ENODEV);

	cfg = pci_ecam_create(dev, &cfgres, bus->res, ops);
	if (IS_ERR(cfg))
		return cfg;

	err = devm_add_action_or_reset(dev, gen_pci_unmap_cfg, cfg);
	if (err)
		return ERR_PTR(err);

	return cfg;
}

int pci_host_common_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct pci_host_bridge *bridge;
	struct pci_config_window *cfg;
	const struct pci_ecam_ops *ops;

	ops = of_device_get_match_data(&pdev->dev);
	if (!ops)
		return -ENODEV;
>>>>>>> upstream/android-13

	bridge = devm_pci_alloc_host_bridge(dev, 0);
	if (!bridge)
		return -ENOMEM;

<<<<<<< HEAD
	type = of_get_property(np, "device_type", NULL);
	if (!type || strcmp(type, "pci")) {
		dev_err(dev, "invalid \"device_type\" %s\n", type);
		return -EINVAL;
	}
=======
	platform_set_drvdata(pdev, bridge);
>>>>>>> upstream/android-13

	of_pci_check_probe_only();

	/* Parse and map our Configuration Space windows */
<<<<<<< HEAD
	cfg = gen_pci_init(dev, &resources, ops);
=======
	cfg = gen_pci_init(dev, bridge, ops);
>>>>>>> upstream/android-13
	if (IS_ERR(cfg))
		return PTR_ERR(cfg);

	/* Do not reassign resources if probe only */
	if (!pci_has_flag(PCI_PROBE_ONLY))
		pci_add_flags(PCI_REASSIGN_ALL_BUS);

<<<<<<< HEAD
	list_splice_init(&resources, &bridge->windows);
	bridge->dev.parent = dev;
	bridge->sysdata = cfg;
	bridge->busnr = cfg->busr.start;
	bridge->ops = &ops->pci_ops;
	bridge->map_irq = of_irq_parse_and_map_pci;
	bridge->swizzle_irq = pci_common_swizzle;

	ret = pci_host_probe(bridge);
	if (ret < 0) {
		pci_free_resource_list(&resources);
		return ret;
	}

	platform_set_drvdata(pdev, bridge->bus);
	return 0;
}

int pci_host_common_remove(struct platform_device *pdev)
{
	struct pci_bus *bus = platform_get_drvdata(pdev);

	pci_lock_rescan_remove();
	pci_stop_root_bus(bus);
	pci_remove_root_bus(bus);
=======
	bridge->sysdata = cfg;
	bridge->ops = (struct pci_ops *)&ops->pci_ops;
	bridge->msi_domain = true;

	return pci_host_probe(bridge);
}
EXPORT_SYMBOL_GPL(pci_host_common_probe);

int pci_host_common_remove(struct platform_device *pdev)
{
	struct pci_host_bridge *bridge = platform_get_drvdata(pdev);

	pci_lock_rescan_remove();
	pci_stop_root_bus(bridge->bus);
	pci_remove_root_bus(bridge->bus);
>>>>>>> upstream/android-13
	pci_unlock_rescan_remove();

	return 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(pci_host_common_remove);

MODULE_LICENSE("GPL v2");
>>>>>>> upstream/android-13
