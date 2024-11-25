<<<<<<< HEAD
/*
 * Arch specific extensions to struct device
 *
 * This file is released under the GPLv2
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Arch specific extensions to struct device
>>>>>>> upstream/android-13
 */
#ifndef _ASM_IA64_DEVICE_H
#define _ASM_IA64_DEVICE_H

struct dev_archdata {
<<<<<<< HEAD
#ifdef CONFIG_INTEL_IOMMU
	void *iommu; /* hook for IOMMU specific extension */
#endif
=======
>>>>>>> upstream/android-13
};

struct pdev_archdata {
};

#endif /* _ASM_IA64_DEVICE_H */
