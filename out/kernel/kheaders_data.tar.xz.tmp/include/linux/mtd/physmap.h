

#ifndef __LINUX_MTD_PHYSMAP__
#define __LINUX_MTD_PHYSMAP__

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

struct map_info;
struct platform_device;

struct physmap_flash_data {
	unsigned int		width;
	int			(*init)(struct platform_device *);
	void			(*exit)(struct platform_device *);
	void			(*set_vpp)(struct platform_device *, int);
	unsigned int		nr_parts;
	unsigned int		pfow_base;
	char                    *probe_type;
	struct mtd_partition	*parts;
	const char * const	*part_probe_types;
};

#endif 
