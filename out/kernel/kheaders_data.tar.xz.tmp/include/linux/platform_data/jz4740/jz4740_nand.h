

#ifndef __JZ4740_NAND_H__
#define __JZ4740_NAND_H__

#include <linux/mtd/rawnand.h>
#include <linux/mtd/partitions.h>

#define JZ_NAND_NUM_BANKS 4

struct jz_nand_platform_data {
	int			num_partitions;
	struct mtd_partition	*partitions;

	unsigned char banks[JZ_NAND_NUM_BANKS];

	void (*ident_callback)(struct platform_device *, struct mtd_info *,
				struct mtd_partition **, int *num_partitions);
};

#endif
