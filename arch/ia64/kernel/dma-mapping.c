// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
#include <linux/dma-mapping.h>
#include <linux/swiotlb.h>
=======
#include <linux/dma-map-ops.h>
>>>>>>> upstream/android-13
#include <linux/export.h>

/* Set this to 1 if there is a HW IOMMU in the system */
int iommu_detected __read_mostly;

const struct dma_map_ops *dma_ops;
EXPORT_SYMBOL(dma_ops);
<<<<<<< HEAD

const struct dma_map_ops *dma_get_ops(struct device *dev)
{
	return dma_ops;
}
EXPORT_SYMBOL(dma_get_ops);

#ifdef CONFIG_SWIOTLB
void __init swiotlb_dma_init(void)
{
	dma_ops = &swiotlb_dma_ops;
	swiotlb_init(1);
}
#endif
=======
>>>>>>> upstream/android-13
