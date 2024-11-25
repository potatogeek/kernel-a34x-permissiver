/* SPDX-License-Identifier: GPL-2.0 */


#include <linux/platform_device.h>
#ifndef __LLCC_QCOM__
#define __LLCC_QCOM__

#define LLCC_CPUSS       1
#define LLCC_VIDSC0      2
#define LLCC_VIDSC1      3
#define LLCC_ROTATOR     4
#define LLCC_VOICE       5
#define LLCC_AUDIO       6
#define LLCC_MDMHPGRW    7
#define LLCC_MDM         8
#define LLCC_CMPT        10
#define LLCC_GPUHTW      11
#define LLCC_GPU         12
#define LLCC_MMUHWT      13
#define LLCC_CMPTDMA     15
#define LLCC_DISP        16
#define LLCC_VIDFW       17
#define LLCC_MDMHPFX     20
#define LLCC_MDMPNG      21
#define LLCC_AUDHW       22


struct llcc_slice_desc {
	u32 slice_id;
	size_t slice_size;
};


struct llcc_slice_config {
	u32 usecase_id;
	u32 slice_id;
	u32 max_cap;
	u32 priority;
	bool fixed_size;
	u32 bonus_ways;
	u32 res_ways;
	u32 cache_mode;
	u32 probe_target_ways;
	bool dis_cap_alloc;
	bool retain_on_pc;
	bool activate_on_init;
};


struct llcc_drv_data {
	struct regmap *regmap;
	const struct llcc_slice_config *cfg;
	struct mutex lock;
	u32 cfg_size;
	u32 max_slices;
	u32 bcast_off;
	u32 num_banks;
	unsigned long *bitmap;
	u32 *offsets;
};

#if IS_ENABLED(CONFIG_QCOM_LLCC)

struct llcc_slice_desc *llcc_slice_getd(u32 uid);


void llcc_slice_putd(struct llcc_slice_desc *desc);


int llcc_get_slice_id(struct llcc_slice_desc *desc);


size_t llcc_get_slice_size(struct llcc_slice_desc *desc);


int llcc_slice_activate(struct llcc_slice_desc *desc);


int llcc_slice_deactivate(struct llcc_slice_desc *desc);


int qcom_llcc_probe(struct platform_device *pdev,
		      const struct llcc_slice_config *table, u32 sz);
#else
static inline struct llcc_slice_desc *llcc_slice_getd(u32 uid)
{
	return NULL;
}

static inline void llcc_slice_putd(struct llcc_slice_desc *desc)
{

};

static inline int llcc_get_slice_id(struct llcc_slice_desc *desc)
{
	return -EINVAL;
}

static inline size_t llcc_get_slice_size(struct llcc_slice_desc *desc)
{
	return 0;
}
static inline int llcc_slice_activate(struct llcc_slice_desc *desc)
{
	return -EINVAL;
}

static inline int llcc_slice_deactivate(struct llcc_slice_desc *desc)
{
	return -EINVAL;
}
static inline int qcom_llcc_probe(struct platform_device *pdev,
		      const struct llcc_slice_config *table, u32 sz)
{
	return -ENODEV;
}

static inline int qcom_llcc_remove(struct platform_device *pdev)
{
	return -ENODEV;
}
#endif

#endif
