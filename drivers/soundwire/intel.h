<<<<<<< HEAD
// SPDX-License-Identifier: (GPL-2.0 OR BSD-3-Clause)
// Copyright(c) 2015-17 Intel Corporation.
=======
/* SPDX-License-Identifier: (GPL-2.0 OR BSD-3-Clause) */
/* Copyright(c) 2015-17 Intel Corporation. */
>>>>>>> upstream/android-13

#ifndef __SDW_INTEL_LOCAL_H
#define __SDW_INTEL_LOCAL_H

/**
<<<<<<< HEAD
 * struct sdw_intel_res - Soundwire link resources
=======
 * struct sdw_intel_link_res - Soundwire Intel link resource structure,
 * typically populated by the controller driver.
 * @mmio_base: mmio base of SoundWire registers
>>>>>>> upstream/android-13
 * @registers: Link IO registers base
 * @shim: Audio shim pointer
 * @alh: ALH (Audio Link Hub) pointer
 * @irq: Interrupt line
 * @ops: Shim callback ops
<<<<<<< HEAD
 * @arg: Shim callback ops argument
 *
 * This is set as pdata for each link instance.
 */
struct sdw_intel_link_res {
=======
 * @dev: device implementing hw_params and free callbacks
 * @shim_lock: mutex to handle access to shared SHIM registers
 * @shim_mask: global pointer to check SHIM register initialization
 * @clock_stop_quirks: mask defining requested behavior on pm_suspend
 * @link_mask: global mask needed for power-up/down sequences
 * @cdns: Cadence master descriptor
 * @list: used to walk-through all masters exposed by the same controller
 */
struct sdw_intel_link_res {
	void __iomem *mmio_base; /* not strictly needed, useful for debug */
>>>>>>> upstream/android-13
	void __iomem *registers;
	void __iomem *shim;
	void __iomem *alh;
	int irq;
	const struct sdw_intel_ops *ops;
<<<<<<< HEAD
	void *arg;
};

=======
	struct device *dev;
	struct mutex *shim_lock; /* protect shared registers */
	u32 *shim_mask;
	u32 clock_stop_quirks;
	u32 link_mask;
	struct sdw_cdns *cdns;
	struct list_head list;
};

struct sdw_intel {
	struct sdw_cdns cdns;
	int instance;
	struct sdw_intel_link_res *link_res;
	bool startup_done;
#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs;
#endif
};

int intel_link_startup(struct auxiliary_device *auxdev);
int intel_link_process_wakeen_event(struct auxiliary_device *auxdev);

struct sdw_intel_link_dev {
	struct auxiliary_device auxdev;
	struct sdw_intel_link_res link_res;
};

#define auxiliary_dev_to_sdw_intel_link_dev(auxiliary_dev) \
	container_of(auxiliary_dev, struct sdw_intel_link_dev, auxdev)

>>>>>>> upstream/android-13
#endif /* __SDW_INTEL_LOCAL_H */
