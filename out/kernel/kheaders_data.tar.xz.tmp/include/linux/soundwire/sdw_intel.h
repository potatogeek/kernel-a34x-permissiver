// SPDX-License-Identifier: (GPL-2.0 OR BSD-3-Clause)
// Copyright(c) 2015-17 Intel Corporation.

#ifndef __SDW_INTEL_H
#define __SDW_INTEL_H


struct sdw_intel_ops {
	int (*config_stream)(void *arg, void *substream,
			void *dai, void *hw_params, int stream_num);
};


struct sdw_intel_res {
	void __iomem *mmio_base;
	int irq;
	acpi_handle handle;
	struct device *parent;
	const struct sdw_intel_ops *ops;
	void *arg;
};

void *sdw_intel_init(acpi_handle *parent_handle, struct sdw_intel_res *res);
void sdw_intel_exit(void *arg);

#endif
