/*
 * Copyright 2014 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "kfd_mqd_manager.h"
<<<<<<< HEAD

struct mqd_manager *mqd_manager_init(enum KFD_MQD_TYPE type,
					struct kfd_dev *dev)
{
	switch (dev->device_info->asic_family) {
	case CHIP_KAVERI:
		return mqd_manager_init_cik(type, dev);
	case CHIP_HAWAII:
		return mqd_manager_init_cik_hawaii(type, dev);
	case CHIP_CARRIZO:
		return mqd_manager_init_vi(type, dev);
	case CHIP_TONGA:
	case CHIP_FIJI:
	case CHIP_POLARIS10:
	case CHIP_POLARIS11:
		return mqd_manager_init_vi_tonga(type, dev);
	case CHIP_VEGA10:
	case CHIP_RAVEN:
		return mqd_manager_init_v9(type, dev);
	default:
		WARN(1, "Unexpected ASIC family %u",
		     dev->device_info->asic_family);
	}

	return NULL;
=======
#include "amdgpu_amdkfd.h"
#include "kfd_device_queue_manager.h"

/* Mapping queue priority to pipe priority, indexed by queue priority */
int pipe_priority_map[] = {
	KFD_PIPE_PRIORITY_CS_LOW,
	KFD_PIPE_PRIORITY_CS_LOW,
	KFD_PIPE_PRIORITY_CS_LOW,
	KFD_PIPE_PRIORITY_CS_LOW,
	KFD_PIPE_PRIORITY_CS_LOW,
	KFD_PIPE_PRIORITY_CS_LOW,
	KFD_PIPE_PRIORITY_CS_LOW,
	KFD_PIPE_PRIORITY_CS_MEDIUM,
	KFD_PIPE_PRIORITY_CS_MEDIUM,
	KFD_PIPE_PRIORITY_CS_MEDIUM,
	KFD_PIPE_PRIORITY_CS_MEDIUM,
	KFD_PIPE_PRIORITY_CS_HIGH,
	KFD_PIPE_PRIORITY_CS_HIGH,
	KFD_PIPE_PRIORITY_CS_HIGH,
	KFD_PIPE_PRIORITY_CS_HIGH,
	KFD_PIPE_PRIORITY_CS_HIGH
};

struct kfd_mem_obj *allocate_hiq_mqd(struct kfd_dev *dev, struct queue_properties *q)
{
	struct kfd_mem_obj *mqd_mem_obj = NULL;

	mqd_mem_obj = kzalloc(sizeof(struct kfd_mem_obj), GFP_KERNEL);
	if (!mqd_mem_obj)
		return NULL;

	mqd_mem_obj->gtt_mem = dev->dqm->hiq_sdma_mqd.gtt_mem;
	mqd_mem_obj->gpu_addr = dev->dqm->hiq_sdma_mqd.gpu_addr;
	mqd_mem_obj->cpu_ptr = dev->dqm->hiq_sdma_mqd.cpu_ptr;

	return mqd_mem_obj;
}

struct kfd_mem_obj *allocate_sdma_mqd(struct kfd_dev *dev,
					struct queue_properties *q)
{
	struct kfd_mem_obj *mqd_mem_obj = NULL;
	uint64_t offset;

	mqd_mem_obj = kzalloc(sizeof(struct kfd_mem_obj), GFP_KERNEL);
	if (!mqd_mem_obj)
		return NULL;

	offset = (q->sdma_engine_id *
		dev->device_info->num_sdma_queues_per_engine +
		q->sdma_queue_id) *
		dev->dqm->mqd_mgrs[KFD_MQD_TYPE_SDMA]->mqd_size;

	offset += dev->dqm->mqd_mgrs[KFD_MQD_TYPE_HIQ]->mqd_size;

	mqd_mem_obj->gtt_mem = (void *)((uint64_t)dev->dqm->hiq_sdma_mqd.gtt_mem
				+ offset);
	mqd_mem_obj->gpu_addr = dev->dqm->hiq_sdma_mqd.gpu_addr + offset;
	mqd_mem_obj->cpu_ptr = (uint32_t *)((uint64_t)
				dev->dqm->hiq_sdma_mqd.cpu_ptr + offset);

	return mqd_mem_obj;
}

void free_mqd_hiq_sdma(struct mqd_manager *mm, void *mqd,
			struct kfd_mem_obj *mqd_mem_obj)
{
	WARN_ON(!mqd_mem_obj->gtt_mem);
	kfree(mqd_mem_obj);
>>>>>>> upstream/android-13
}

void mqd_symmetrically_map_cu_mask(struct mqd_manager *mm,
		const uint32_t *cu_mask, uint32_t cu_mask_count,
		uint32_t *se_mask)
{
	struct kfd_cu_info cu_info;
<<<<<<< HEAD
	uint32_t cu_per_sh[4] = {0};
	int i, se, cu = 0;

	mm->dev->kfd2kgd->get_cu_info(mm->dev->kgd, &cu_info);
=======
	uint32_t cu_per_sh[KFD_MAX_NUM_SE][KFD_MAX_NUM_SH_PER_SE] = {0};
	int i, se, sh, cu;
	amdgpu_amdkfd_get_cu_info(mm->dev->kgd, &cu_info);
>>>>>>> upstream/android-13

	if (cu_mask_count > cu_info.cu_active_number)
		cu_mask_count = cu_info.cu_active_number;

<<<<<<< HEAD
	for (se = 0; se < cu_info.num_shader_engines; se++)
		for (i = 0; i < 4; i++)
			cu_per_sh[se] += hweight32(cu_info.cu_bitmap[se][i]);

	/* Symmetrically map cu_mask to all SEs:
	 * cu_mask[0] bit0 -> se_mask[0] bit0;
	 * cu_mask[0] bit1 -> se_mask[1] bit0;
	 * ... (if # SE is 4)
	 * cu_mask[0] bit4 -> se_mask[0] bit1;
	 * ...
	 */
	se = 0;
	for (i = 0; i < cu_mask_count; i++) {
		if (cu_mask[i / 32] & (1 << (i % 32)))
			se_mask[se] |= 1 << cu;

		do {
			se++;
			if (se == cu_info.num_shader_engines) {
				se = 0;
				cu++;
			}
		} while (cu >= cu_per_sh[se] && cu < 32);
=======
	/* Exceeding these bounds corrupts the stack and indicates a coding error.
	 * Returning with no CU's enabled will hang the queue, which should be
	 * attention grabbing.
	 */
	if (cu_info.num_shader_engines > KFD_MAX_NUM_SE) {
		pr_err("Exceeded KFD_MAX_NUM_SE, chip reports %d\n", cu_info.num_shader_engines);
		return;
	}
	if (cu_info.num_shader_arrays_per_engine > KFD_MAX_NUM_SH_PER_SE) {
		pr_err("Exceeded KFD_MAX_NUM_SH, chip reports %d\n",
			cu_info.num_shader_arrays_per_engine * cu_info.num_shader_engines);
		return;
	}
	/* Count active CUs per SH.
	 *
	 * Some CUs in an SH may be disabled.	HW expects disabled CUs to be
	 * represented in the high bits of each SH's enable mask (the upper and lower
	 * 16 bits of se_mask) and will take care of the actual distribution of
	 * disabled CUs within each SH automatically.
	 * Each half of se_mask must be filled only on bits 0-cu_per_sh[se][sh]-1.
	 *
	 * See note on Arcturus cu_bitmap layout in gfx_v9_0_get_cu_info.
	 */
	for (se = 0; se < cu_info.num_shader_engines; se++)
		for (sh = 0; sh < cu_info.num_shader_arrays_per_engine; sh++)
			cu_per_sh[se][sh] = hweight32(cu_info.cu_bitmap[se % 4][sh + (se / 4)]);

	/* Symmetrically map cu_mask to all SEs & SHs:
	 * se_mask programs up to 2 SH in the upper and lower 16 bits.
	 *
	 * Examples
	 * Assuming 1 SH/SE, 4 SEs:
	 * cu_mask[0] bit0 -> se_mask[0] bit0
	 * cu_mask[0] bit1 -> se_mask[1] bit0
	 * ...
	 * cu_mask[0] bit4 -> se_mask[0] bit1
	 * ...
	 *
	 * Assuming 2 SH/SE, 4 SEs
	 * cu_mask[0] bit0 -> se_mask[0] bit0 (SE0,SH0,CU0)
	 * cu_mask[0] bit1 -> se_mask[1] bit0 (SE1,SH0,CU0)
	 * ...
	 * cu_mask[0] bit4 -> se_mask[0] bit16 (SE0,SH1,CU0)
	 * cu_mask[0] bit5 -> se_mask[1] bit16 (SE1,SH1,CU0)
	 * ...
	 * cu_mask[0] bit8 -> se_mask[0] bit1 (SE0,SH0,CU1)
	 * ...
	 *
	 * First ensure all CUs are disabled, then enable user specified CUs.
	 */
	for (i = 0; i < cu_info.num_shader_engines; i++)
		se_mask[i] = 0;

	i = 0;
	for (cu = 0; cu < 16; cu++) {
		for (sh = 0; sh < cu_info.num_shader_arrays_per_engine; sh++) {
			for (se = 0; se < cu_info.num_shader_engines; se++) {
				if (cu_per_sh[se][sh] > cu) {
					if (cu_mask[i / 32] & (1 << (i % 32)))
						se_mask[se] |= 1 << (cu + sh * 16);
					i++;
					if (i == cu_mask_count)
						return;
				}
			}
		}
>>>>>>> upstream/android-13
	}
}
