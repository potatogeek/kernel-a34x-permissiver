

#ifndef __LINUX_PLATFORM_DATA_MLXREG_H
#define __LINUX_PLATFORM_DATA_MLXREG_H

#define MLXREG_CORE_LABEL_MAX_SIZE	32


struct mlxreg_hotplug_device {
	struct i2c_adapter *adapter;
	struct i2c_client *client;
	struct i2c_board_info *brdinfo;
	int nr;
};


struct mlxreg_core_data {
	char label[MLXREG_CORE_LABEL_MAX_SIZE];
	u32 reg;
	u32 mask;
	u32 bit;
	umode_t	mode;
	struct device_node *np;
	struct mlxreg_hotplug_device hpdev;
	u8 health_cntr;
	bool attached;
};


struct mlxreg_core_item {
	struct mlxreg_core_data *data;
	u32 aggr_mask;
	u32 reg;
	u32 mask;
	u32 cache;
	u8 count;
	u8 ind;
	u8 inversed;
	u8 health;
};


struct mlxreg_core_platform_data {
	struct mlxreg_core_data *data;
	void *regmap;
	int counter;
};


struct mlxreg_core_hotplug_platform_data {
	struct mlxreg_core_item *items;
	int irq;
	void *regmap;
	int counter;
	u32 cell;
	u32 mask;
	u32 cell_low;
	u32 mask_low;
	int deferred_nr;
	int shift_nr;
};

#endif 
