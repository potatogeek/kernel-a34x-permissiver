/*
 * Copyright 2014-2018 Advanced Micro Devices, Inc.
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
 */
<<<<<<< HEAD

#define pr_fmt(fmt) "kfd2kgd: " fmt

#include <linux/module.h>
#include <linux/fdtable.h>
#include <linux/uaccess.h>
#include <linux/firmware.h>
#include <drm/drmP.h>
#include "amdgpu.h"
#include "amdgpu_amdkfd.h"
#include "amdgpu_ucode.h"
#include "soc15_hw_ip.h"
=======
#include "amdgpu.h"
#include "amdgpu_amdkfd.h"
>>>>>>> upstream/android-13
#include "gc/gc_9_0_offset.h"
#include "gc/gc_9_0_sh_mask.h"
#include "vega10_enum.h"
#include "sdma0/sdma0_4_0_offset.h"
#include "sdma0/sdma0_4_0_sh_mask.h"
#include "sdma1/sdma1_4_0_offset.h"
#include "sdma1/sdma1_4_0_sh_mask.h"
#include "athub/athub_1_0_offset.h"
#include "athub/athub_1_0_sh_mask.h"
#include "oss/osssys_4_0_offset.h"
#include "oss/osssys_4_0_sh_mask.h"
#include "soc15_common.h"
#include "v9_structs.h"
#include "soc15.h"
#include "soc15d.h"
<<<<<<< HEAD

/* HACK: MMHUB and GC both have VM-related register with the same
 * names but different offsets. Define the MMHUB register we need here
 * with a prefix. A proper solution would be to move the functions
 * programming these registers into gfx_v9_0.c and mmhub_v1_0.c
 * respectively.
 */
#define mmMMHUB_VM_INVALIDATE_ENG16_REQ				0x06f3
#define mmMMHUB_VM_INVALIDATE_ENG16_REQ_BASE_IDX		0

#define mmMMHUB_VM_INVALIDATE_ENG16_ACK				0x0705
#define mmMMHUB_VM_INVALIDATE_ENG16_ACK_BASE_IDX		0

#define mmMMHUB_VM_CONTEXT0_PAGE_TABLE_BASE_ADDR_LO32		0x072b
#define mmMMHUB_VM_CONTEXT0_PAGE_TABLE_BASE_ADDR_LO32_BASE_IDX	0
#define mmMMHUB_VM_CONTEXT0_PAGE_TABLE_BASE_ADDR_HI32		0x072c
#define mmMMHUB_VM_CONTEXT0_PAGE_TABLE_BASE_ADDR_HI32_BASE_IDX	0

#define mmMMHUB_VM_CONTEXT0_PAGE_TABLE_START_ADDR_LO32		0x074b
#define mmMMHUB_VM_CONTEXT0_PAGE_TABLE_START_ADDR_LO32_BASE_IDX	0
#define mmMMHUB_VM_CONTEXT0_PAGE_TABLE_START_ADDR_HI32		0x074c
#define mmMMHUB_VM_CONTEXT0_PAGE_TABLE_START_ADDR_HI32_BASE_IDX	0

#define mmMMHUB_VM_CONTEXT0_PAGE_TABLE_END_ADDR_LO32		0x076b
#define mmMMHUB_VM_CONTEXT0_PAGE_TABLE_END_ADDR_LO32_BASE_IDX	0
#define mmMMHUB_VM_CONTEXT0_PAGE_TABLE_END_ADDR_HI32		0x076c
#define mmMMHUB_VM_CONTEXT0_PAGE_TABLE_END_ADDR_HI32_BASE_IDX	0

#define mmMMHUB_VM_INVALIDATE_ENG16_ADDR_RANGE_LO32		0x0727
#define mmMMHUB_VM_INVALIDATE_ENG16_ADDR_RANGE_LO32_BASE_IDX	0
#define mmMMHUB_VM_INVALIDATE_ENG16_ADDR_RANGE_HI32		0x0728
#define mmMMHUB_VM_INVALIDATE_ENG16_ADDR_RANGE_HI32_BASE_IDX	0

#define V9_PIPE_PER_MEC		(4)
#define V9_QUEUES_PER_PIPE_MEC	(8)
=======
#include "gfx_v9_0.h"
#include "amdgpu_amdkfd_gfx_v9.h"
>>>>>>> upstream/android-13

enum hqd_dequeue_request_type {
	NO_ACTION = 0,
	DRAIN_PIPE,
<<<<<<< HEAD
	RESET_WAVES
};

/*
 * Register access functions
 */

static void kgd_program_sh_mem_settings(struct kgd_dev *kgd, uint32_t vmid,
		uint32_t sh_mem_config,
		uint32_t sh_mem_ape1_base, uint32_t sh_mem_ape1_limit,
		uint32_t sh_mem_bases);
static int kgd_set_pasid_vmid_mapping(struct kgd_dev *kgd, unsigned int pasid,
		unsigned int vmid);
static int kgd_init_interrupts(struct kgd_dev *kgd, uint32_t pipe_id);
static int kgd_hqd_load(struct kgd_dev *kgd, void *mqd, uint32_t pipe_id,
			uint32_t queue_id, uint32_t __user *wptr,
			uint32_t wptr_shift, uint32_t wptr_mask,
			struct mm_struct *mm);
static int kgd_hqd_dump(struct kgd_dev *kgd,
			uint32_t pipe_id, uint32_t queue_id,
			uint32_t (**dump)[2], uint32_t *n_regs);
static int kgd_hqd_sdma_load(struct kgd_dev *kgd, void *mqd,
			     uint32_t __user *wptr, struct mm_struct *mm);
static int kgd_hqd_sdma_dump(struct kgd_dev *kgd,
			     uint32_t engine_id, uint32_t queue_id,
			     uint32_t (**dump)[2], uint32_t *n_regs);
static bool kgd_hqd_is_occupied(struct kgd_dev *kgd, uint64_t queue_address,
		uint32_t pipe_id, uint32_t queue_id);
static bool kgd_hqd_sdma_is_occupied(struct kgd_dev *kgd, void *mqd);
static int kgd_hqd_destroy(struct kgd_dev *kgd, void *mqd,
				enum kfd_preempt_type reset_type,
				unsigned int utimeout, uint32_t pipe_id,
				uint32_t queue_id);
static int kgd_hqd_sdma_destroy(struct kgd_dev *kgd, void *mqd,
				unsigned int utimeout);
static int kgd_address_watch_disable(struct kgd_dev *kgd);
static int kgd_address_watch_execute(struct kgd_dev *kgd,
					unsigned int watch_point_id,
					uint32_t cntl_val,
					uint32_t addr_hi,
					uint32_t addr_lo);
static int kgd_wave_control_execute(struct kgd_dev *kgd,
					uint32_t gfx_index_val,
					uint32_t sq_cmd);
static uint32_t kgd_address_watch_get_offset(struct kgd_dev *kgd,
					unsigned int watch_point_id,
					unsigned int reg_offset);

static bool get_atc_vmid_pasid_mapping_valid(struct kgd_dev *kgd,
		uint8_t vmid);
static uint16_t get_atc_vmid_pasid_mapping_pasid(struct kgd_dev *kgd,
		uint8_t vmid);
static void set_vm_context_page_table_base(struct kgd_dev *kgd, uint32_t vmid,
		uint32_t page_table_base);
static uint16_t get_fw_version(struct kgd_dev *kgd, enum kgd_engine_type type);
static void set_scratch_backing_va(struct kgd_dev *kgd,
					uint64_t va, uint32_t vmid);
static int invalidate_tlbs(struct kgd_dev *kgd, uint16_t pasid);
static int invalidate_tlbs_vmid(struct kgd_dev *kgd, uint16_t vmid);

/* Because of REG_GET_FIELD() being used, we put this function in the
 * asic specific file.
 */
static int amdgpu_amdkfd_get_tile_config(struct kgd_dev *kgd,
		struct tile_config *config)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	config->gb_addr_config = adev->gfx.config.gb_addr_config;

	config->tile_config_ptr = adev->gfx.config.tile_mode_array;
	config->num_tile_configs =
			ARRAY_SIZE(adev->gfx.config.tile_mode_array);
	config->macro_tile_config_ptr =
			adev->gfx.config.macrotile_mode_array;
	config->num_macro_tile_configs =
			ARRAY_SIZE(adev->gfx.config.macrotile_mode_array);

	return 0;
}

static const struct kfd2kgd_calls kfd2kgd = {
	.init_gtt_mem_allocation = alloc_gtt_mem,
	.free_gtt_mem = free_gtt_mem,
	.get_local_mem_info = get_local_mem_info,
	.get_gpu_clock_counter = get_gpu_clock_counter,
	.get_max_engine_clock_in_mhz = get_max_engine_clock_in_mhz,
	.alloc_pasid = amdgpu_pasid_alloc,
	.free_pasid = amdgpu_pasid_free,
	.program_sh_mem_settings = kgd_program_sh_mem_settings,
	.set_pasid_vmid_mapping = kgd_set_pasid_vmid_mapping,
	.init_interrupts = kgd_init_interrupts,
	.hqd_load = kgd_hqd_load,
	.hqd_sdma_load = kgd_hqd_sdma_load,
	.hqd_dump = kgd_hqd_dump,
	.hqd_sdma_dump = kgd_hqd_sdma_dump,
	.hqd_is_occupied = kgd_hqd_is_occupied,
	.hqd_sdma_is_occupied = kgd_hqd_sdma_is_occupied,
	.hqd_destroy = kgd_hqd_destroy,
	.hqd_sdma_destroy = kgd_hqd_sdma_destroy,
	.address_watch_disable = kgd_address_watch_disable,
	.address_watch_execute = kgd_address_watch_execute,
	.wave_control_execute = kgd_wave_control_execute,
	.address_watch_get_offset = kgd_address_watch_get_offset,
	.get_atc_vmid_pasid_mapping_pasid =
			get_atc_vmid_pasid_mapping_pasid,
	.get_atc_vmid_pasid_mapping_valid =
			get_atc_vmid_pasid_mapping_valid,
	.get_fw_version = get_fw_version,
	.set_scratch_backing_va = set_scratch_backing_va,
	.get_tile_config = amdgpu_amdkfd_get_tile_config,
	.get_cu_info = get_cu_info,
	.get_vram_usage = amdgpu_amdkfd_get_vram_usage,
	.create_process_vm = amdgpu_amdkfd_gpuvm_create_process_vm,
	.acquire_process_vm = amdgpu_amdkfd_gpuvm_acquire_process_vm,
	.destroy_process_vm = amdgpu_amdkfd_gpuvm_destroy_process_vm,
	.get_process_page_dir = amdgpu_amdkfd_gpuvm_get_process_page_dir,
	.set_vm_context_page_table_base = set_vm_context_page_table_base,
	.alloc_memory_of_gpu = amdgpu_amdkfd_gpuvm_alloc_memory_of_gpu,
	.free_memory_of_gpu = amdgpu_amdkfd_gpuvm_free_memory_of_gpu,
	.map_memory_to_gpu = amdgpu_amdkfd_gpuvm_map_memory_to_gpu,
	.unmap_memory_to_gpu = amdgpu_amdkfd_gpuvm_unmap_memory_from_gpu,
	.sync_memory = amdgpu_amdkfd_gpuvm_sync_memory,
	.map_gtt_bo_to_kernel = amdgpu_amdkfd_gpuvm_map_gtt_bo_to_kernel,
	.restore_process_bos = amdgpu_amdkfd_gpuvm_restore_process_bos,
	.invalidate_tlbs = invalidate_tlbs,
	.invalidate_tlbs_vmid = invalidate_tlbs_vmid,
	.submit_ib = amdgpu_amdkfd_submit_ib,
	.gpu_recover = amdgpu_amdkfd_gpu_reset,
	.set_compute_idle = amdgpu_amdkfd_set_compute_idle
};

struct kfd2kgd_calls *amdgpu_amdkfd_gfx_9_0_get_functions(void)
{
	return (struct kfd2kgd_calls *)&kfd2kgd;
}

=======
	RESET_WAVES,
	SAVE_WAVES
};

>>>>>>> upstream/android-13
static inline struct amdgpu_device *get_amdgpu_device(struct kgd_dev *kgd)
{
	return (struct amdgpu_device *)kgd;
}

static void lock_srbm(struct kgd_dev *kgd, uint32_t mec, uint32_t pipe,
			uint32_t queue, uint32_t vmid)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);

	mutex_lock(&adev->srbm_mutex);
	soc15_grbm_select(adev, mec, pipe, queue, vmid);
}

static void unlock_srbm(struct kgd_dev *kgd)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);

	soc15_grbm_select(adev, 0, 0, 0, 0);
	mutex_unlock(&adev->srbm_mutex);
}

static void acquire_queue(struct kgd_dev *kgd, uint32_t pipe_id,
				uint32_t queue_id)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);

	uint32_t mec = (pipe_id / adev->gfx.mec.num_pipe_per_mec) + 1;
	uint32_t pipe = (pipe_id % adev->gfx.mec.num_pipe_per_mec);

	lock_srbm(kgd, mec, pipe, queue_id, 0);
}

<<<<<<< HEAD
static uint32_t get_queue_mask(struct amdgpu_device *adev,
			       uint32_t pipe_id, uint32_t queue_id)
{
	unsigned int bit = (pipe_id * adev->gfx.mec.num_queue_per_pipe +
			    queue_id) & 31;

	return ((uint32_t)1) << bit;
=======
static uint64_t get_queue_mask(struct amdgpu_device *adev,
			       uint32_t pipe_id, uint32_t queue_id)
{
	unsigned int bit = pipe_id * adev->gfx.mec.num_queue_per_pipe +
			queue_id;

	return 1ull << bit;
>>>>>>> upstream/android-13
}

static void release_queue(struct kgd_dev *kgd)
{
	unlock_srbm(kgd);
}

<<<<<<< HEAD
static void kgd_program_sh_mem_settings(struct kgd_dev *kgd, uint32_t vmid,
=======
void kgd_gfx_v9_program_sh_mem_settings(struct kgd_dev *kgd, uint32_t vmid,
>>>>>>> upstream/android-13
					uint32_t sh_mem_config,
					uint32_t sh_mem_ape1_base,
					uint32_t sh_mem_ape1_limit,
					uint32_t sh_mem_bases)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);

	lock_srbm(kgd, 0, 0, 0, vmid);

<<<<<<< HEAD
	WREG32(SOC15_REG_OFFSET(GC, 0, mmSH_MEM_CONFIG), sh_mem_config);
	WREG32(SOC15_REG_OFFSET(GC, 0, mmSH_MEM_BASES), sh_mem_bases);
=======
	WREG32_RLC(SOC15_REG_OFFSET(GC, 0, mmSH_MEM_CONFIG), sh_mem_config);
	WREG32_RLC(SOC15_REG_OFFSET(GC, 0, mmSH_MEM_BASES), sh_mem_bases);
>>>>>>> upstream/android-13
	/* APE1 no longer exists on GFX9 */

	unlock_srbm(kgd);
}

<<<<<<< HEAD
static int kgd_set_pasid_vmid_mapping(struct kgd_dev *kgd, unsigned int pasid,
=======
int kgd_gfx_v9_set_pasid_vmid_mapping(struct kgd_dev *kgd, u32 pasid,
>>>>>>> upstream/android-13
					unsigned int vmid)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);

	/*
	 * We have to assume that there is no outstanding mapping.
	 * The ATC_VMID_PASID_MAPPING_UPDATE_STATUS bit could be 0 because
	 * a mapping is in progress or because a mapping finished
	 * and the SW cleared it.
	 * So the protocol is to always wait & clear.
	 */
	uint32_t pasid_mapping = (pasid == 0) ? 0 : (uint32_t)pasid |
			ATC_VMID0_PASID_MAPPING__VALID_MASK;

	/*
	 * need to do this twice, once for gfx and once for mmhub
	 * for ATC add 16 to VMID for mmhub, for IH different registers.
	 * ATC_VMID0..15 registers are separate from ATC_VMID16..31.
	 */

	WREG32(SOC15_REG_OFFSET(ATHUB, 0, mmATC_VMID0_PASID_MAPPING) + vmid,
	       pasid_mapping);

	while (!(RREG32(SOC15_REG_OFFSET(
				ATHUB, 0,
				mmATC_VMID_PASID_MAPPING_UPDATE_STATUS)) &
		 (1U << vmid)))
		cpu_relax();

	WREG32(SOC15_REG_OFFSET(ATHUB, 0,
				mmATC_VMID_PASID_MAPPING_UPDATE_STATUS),
	       1U << vmid);

	/* Mapping vmid to pasid also for IH block */
	WREG32(SOC15_REG_OFFSET(OSSSYS, 0, mmIH_VMID_0_LUT) + vmid,
	       pasid_mapping);

	WREG32(SOC15_REG_OFFSET(ATHUB, 0, mmATC_VMID16_PASID_MAPPING) + vmid,
	       pasid_mapping);

	while (!(RREG32(SOC15_REG_OFFSET(
				ATHUB, 0,
				mmATC_VMID_PASID_MAPPING_UPDATE_STATUS)) &
		 (1U << (vmid + 16))))
		cpu_relax();

	WREG32(SOC15_REG_OFFSET(ATHUB, 0,
				mmATC_VMID_PASID_MAPPING_UPDATE_STATUS),
	       1U << (vmid + 16));

	/* Mapping vmid to pasid also for IH block */
	WREG32(SOC15_REG_OFFSET(OSSSYS, 0, mmIH_VMID_0_LUT_MM) + vmid,
	       pasid_mapping);
	return 0;
}

/* TODO - RING0 form of field is obsolete, seems to date back to SI
 * but still works
 */

<<<<<<< HEAD
static int kgd_init_interrupts(struct kgd_dev *kgd, uint32_t pipe_id)
=======
int kgd_gfx_v9_init_interrupts(struct kgd_dev *kgd, uint32_t pipe_id)
>>>>>>> upstream/android-13
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	uint32_t mec;
	uint32_t pipe;

	mec = (pipe_id / adev->gfx.mec.num_pipe_per_mec) + 1;
	pipe = (pipe_id % adev->gfx.mec.num_pipe_per_mec);

	lock_srbm(kgd, mec, pipe, 0, 0);

	WREG32(SOC15_REG_OFFSET(GC, 0, mmCPC_INT_CNTL),
		CP_INT_CNTL_RING0__TIME_STAMP_INT_ENABLE_MASK |
		CP_INT_CNTL_RING0__OPCODE_ERROR_INT_ENABLE_MASK);

	unlock_srbm(kgd);

	return 0;
}

<<<<<<< HEAD
static uint32_t get_sdma_base_addr(struct amdgpu_device *adev,
				unsigned int engine_id,
				unsigned int queue_id)
{
	uint32_t base[2] = {
		SOC15_REG_OFFSET(SDMA0, 0,
				 mmSDMA0_RLC0_RB_CNTL) - mmSDMA0_RLC0_RB_CNTL,
		SOC15_REG_OFFSET(SDMA1, 0,
				 mmSDMA1_RLC0_RB_CNTL) - mmSDMA1_RLC0_RB_CNTL
	};
	uint32_t retval;

	retval = base[engine_id] + queue_id * (mmSDMA0_RLC1_RB_CNTL -
					       mmSDMA0_RLC0_RB_CNTL);

	pr_debug("sdma base address: 0x%x\n", retval);

	return retval;
=======
static uint32_t get_sdma_rlc_reg_offset(struct amdgpu_device *adev,
				unsigned int engine_id,
				unsigned int queue_id)
{
	uint32_t sdma_engine_reg_base = 0;
	uint32_t sdma_rlc_reg_offset;

	switch (engine_id) {
	default:
		dev_warn(adev->dev,
			 "Invalid sdma engine id (%d), using engine id 0\n",
			 engine_id);
		fallthrough;
	case 0:
		sdma_engine_reg_base = SOC15_REG_OFFSET(SDMA0, 0,
				mmSDMA0_RLC0_RB_CNTL) - mmSDMA0_RLC0_RB_CNTL;
		break;
	case 1:
		sdma_engine_reg_base = SOC15_REG_OFFSET(SDMA1, 0,
				mmSDMA1_RLC0_RB_CNTL) - mmSDMA0_RLC0_RB_CNTL;
		break;
	}

	sdma_rlc_reg_offset = sdma_engine_reg_base
		+ queue_id * (mmSDMA0_RLC1_RB_CNTL - mmSDMA0_RLC0_RB_CNTL);

	pr_debug("RLC register offset for SDMA%d RLC%d: 0x%x\n", engine_id,
		 queue_id, sdma_rlc_reg_offset);

	return sdma_rlc_reg_offset;
>>>>>>> upstream/android-13
}

static inline struct v9_mqd *get_mqd(void *mqd)
{
	return (struct v9_mqd *)mqd;
}

static inline struct v9_sdma_mqd *get_sdma_mqd(void *mqd)
{
	return (struct v9_sdma_mqd *)mqd;
}

<<<<<<< HEAD
static int kgd_hqd_load(struct kgd_dev *kgd, void *mqd, uint32_t pipe_id,
=======
int kgd_gfx_v9_hqd_load(struct kgd_dev *kgd, void *mqd, uint32_t pipe_id,
>>>>>>> upstream/android-13
			uint32_t queue_id, uint32_t __user *wptr,
			uint32_t wptr_shift, uint32_t wptr_mask,
			struct mm_struct *mm)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct v9_mqd *m;
	uint32_t *mqd_hqd;
	uint32_t reg, hqd_base, data;

	m = get_mqd(mqd);

	acquire_queue(kgd, pipe_id, queue_id);

<<<<<<< HEAD
	/* HIQ is set during driver init period with vmid set to 0*/
	if (m->cp_hqd_vmid == 0) {
		uint32_t value, mec, pipe;

		mec = (pipe_id / adev->gfx.mec.num_pipe_per_mec) + 1;
		pipe = (pipe_id % adev->gfx.mec.num_pipe_per_mec);

		pr_debug("kfd: set HIQ, mec:%d, pipe:%d, queue:%d.\n",
			mec, pipe, queue_id);
		value = RREG32(SOC15_REG_OFFSET(GC, 0, mmRLC_CP_SCHEDULERS));
		value = REG_SET_FIELD(value, RLC_CP_SCHEDULERS, scheduler1,
			((mec << 5) | (pipe << 3) | queue_id | 0x80));
		WREG32(SOC15_REG_OFFSET(GC, 0, mmRLC_CP_SCHEDULERS), value);
	}

=======
>>>>>>> upstream/android-13
	/* HQD registers extend from CP_MQD_BASE_ADDR to CP_HQD_EOP_WPTR_MEM. */
	mqd_hqd = &m->cp_mqd_base_addr_lo;
	hqd_base = SOC15_REG_OFFSET(GC, 0, mmCP_MQD_BASE_ADDR);

	for (reg = hqd_base;
	     reg <= SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_WPTR_HI); reg++)
<<<<<<< HEAD
		WREG32(reg, mqd_hqd[reg - hqd_base]);
=======
		WREG32_RLC(reg, mqd_hqd[reg - hqd_base]);
>>>>>>> upstream/android-13


	/* Activate doorbell logic before triggering WPTR poll. */
	data = REG_SET_FIELD(m->cp_hqd_pq_doorbell_control,
			     CP_HQD_PQ_DOORBELL_CONTROL, DOORBELL_EN, 1);
<<<<<<< HEAD
	WREG32(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_DOORBELL_CONTROL), data);
=======
	WREG32_RLC(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_DOORBELL_CONTROL), data);
>>>>>>> upstream/android-13

	if (wptr) {
		/* Don't read wptr with get_user because the user
		 * context may not be accessible (if this function
		 * runs in a work queue). Instead trigger a one-shot
		 * polling read from memory in the CP. This assumes
		 * that wptr is GPU-accessible in the queue's VMID via
		 * ATC or SVM. WPTR==RPTR before starting the poll so
		 * the CP starts fetching new commands from the right
		 * place.
		 *
		 * Guessing a 64-bit WPTR from a 32-bit RPTR is a bit
		 * tricky. Assume that the queue didn't overflow. The
		 * number of valid bits in the 32-bit RPTR depends on
		 * the queue size. The remaining bits are taken from
		 * the saved 64-bit WPTR. If the WPTR wrapped, add the
		 * queue size.
		 */
		uint32_t queue_size =
			2 << REG_GET_FIELD(m->cp_hqd_pq_control,
					   CP_HQD_PQ_CONTROL, QUEUE_SIZE);
		uint64_t guessed_wptr = m->cp_hqd_pq_rptr & (queue_size - 1);

		if ((m->cp_hqd_pq_wptr_lo & (queue_size - 1)) < guessed_wptr)
			guessed_wptr += queue_size;
		guessed_wptr += m->cp_hqd_pq_wptr_lo & ~(queue_size - 1);
		guessed_wptr += (uint64_t)m->cp_hqd_pq_wptr_hi << 32;

<<<<<<< HEAD
		WREG32(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_WPTR_LO),
		       lower_32_bits(guessed_wptr));
		WREG32(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_WPTR_HI),
		       upper_32_bits(guessed_wptr));
		WREG32(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_WPTR_POLL_ADDR),
		       lower_32_bits((uintptr_t)wptr));
		WREG32(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_WPTR_POLL_ADDR_HI),
		       upper_32_bits((uintptr_t)wptr));
		WREG32(SOC15_REG_OFFSET(GC, 0, mmCP_PQ_WPTR_POLL_CNTL1),
		       get_queue_mask(adev, pipe_id, queue_id));
	}

	/* Start the EOP fetcher */
	WREG32(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_EOP_RPTR),
=======
		WREG32_RLC(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_WPTR_LO),
		       lower_32_bits(guessed_wptr));
		WREG32_RLC(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_WPTR_HI),
		       upper_32_bits(guessed_wptr));
		WREG32_RLC(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_WPTR_POLL_ADDR),
		       lower_32_bits((uintptr_t)wptr));
		WREG32_RLC(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_WPTR_POLL_ADDR_HI),
		       upper_32_bits((uintptr_t)wptr));
		WREG32(SOC15_REG_OFFSET(GC, 0, mmCP_PQ_WPTR_POLL_CNTL1),
		       (uint32_t)get_queue_mask(adev, pipe_id, queue_id));
	}

	/* Start the EOP fetcher */
	WREG32_RLC(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_EOP_RPTR),
>>>>>>> upstream/android-13
	       REG_SET_FIELD(m->cp_hqd_eop_rptr,
			     CP_HQD_EOP_RPTR, INIT_FETCHER, 1));

	data = REG_SET_FIELD(m->cp_hqd_active, CP_HQD_ACTIVE, ACTIVE, 1);
<<<<<<< HEAD
	WREG32(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_ACTIVE), data);
=======
	WREG32_RLC(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_ACTIVE), data);
>>>>>>> upstream/android-13

	release_queue(kgd);

	return 0;
}

<<<<<<< HEAD
static int kgd_hqd_dump(struct kgd_dev *kgd,
=======
int kgd_gfx_v9_hiq_mqd_load(struct kgd_dev *kgd, void *mqd,
			    uint32_t pipe_id, uint32_t queue_id,
			    uint32_t doorbell_off)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct amdgpu_ring *kiq_ring = &adev->gfx.kiq.ring;
	struct v9_mqd *m;
	uint32_t mec, pipe;
	int r;

	m = get_mqd(mqd);

	acquire_queue(kgd, pipe_id, queue_id);

	mec = (pipe_id / adev->gfx.mec.num_pipe_per_mec) + 1;
	pipe = (pipe_id % adev->gfx.mec.num_pipe_per_mec);

	pr_debug("kfd: set HIQ, mec:%d, pipe:%d, queue:%d.\n",
		 mec, pipe, queue_id);

	spin_lock(&adev->gfx.kiq.ring_lock);
	r = amdgpu_ring_alloc(kiq_ring, 7);
	if (r) {
		pr_err("Failed to alloc KIQ (%d).\n", r);
		goto out_unlock;
	}

	amdgpu_ring_write(kiq_ring, PACKET3(PACKET3_MAP_QUEUES, 5));
	amdgpu_ring_write(kiq_ring,
			  PACKET3_MAP_QUEUES_QUEUE_SEL(0) | /* Queue_Sel */
			  PACKET3_MAP_QUEUES_VMID(m->cp_hqd_vmid) | /* VMID */
			  PACKET3_MAP_QUEUES_QUEUE(queue_id) |
			  PACKET3_MAP_QUEUES_PIPE(pipe) |
			  PACKET3_MAP_QUEUES_ME((mec - 1)) |
			  PACKET3_MAP_QUEUES_QUEUE_TYPE(0) | /*queue_type: normal compute queue */
			  PACKET3_MAP_QUEUES_ALLOC_FORMAT(0) | /* alloc format: all_on_one_pipe */
			  PACKET3_MAP_QUEUES_ENGINE_SEL(1) | /* engine_sel: hiq */
			  PACKET3_MAP_QUEUES_NUM_QUEUES(1)); /* num_queues: must be 1 */
	amdgpu_ring_write(kiq_ring,
			  PACKET3_MAP_QUEUES_DOORBELL_OFFSET(doorbell_off));
	amdgpu_ring_write(kiq_ring, m->cp_mqd_base_addr_lo);
	amdgpu_ring_write(kiq_ring, m->cp_mqd_base_addr_hi);
	amdgpu_ring_write(kiq_ring, m->cp_hqd_pq_wptr_poll_addr_lo);
	amdgpu_ring_write(kiq_ring, m->cp_hqd_pq_wptr_poll_addr_hi);
	amdgpu_ring_commit(kiq_ring);

out_unlock:
	spin_unlock(&adev->gfx.kiq.ring_lock);
	release_queue(kgd);

	return r;
}

int kgd_gfx_v9_hqd_dump(struct kgd_dev *kgd,
>>>>>>> upstream/android-13
			uint32_t pipe_id, uint32_t queue_id,
			uint32_t (**dump)[2], uint32_t *n_regs)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	uint32_t i = 0, reg;
#define HQD_N_REGS 56
#define DUMP_REG(addr) do {				\
		if (WARN_ON_ONCE(i >= HQD_N_REGS))	\
			break;				\
		(*dump)[i][0] = (addr) << 2;		\
		(*dump)[i++][1] = RREG32(addr);		\
	} while (0)

	*dump = kmalloc_array(HQD_N_REGS * 2, sizeof(uint32_t), GFP_KERNEL);
	if (*dump == NULL)
		return -ENOMEM;

	acquire_queue(kgd, pipe_id, queue_id);

	for (reg = SOC15_REG_OFFSET(GC, 0, mmCP_MQD_BASE_ADDR);
	     reg <= SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_WPTR_HI); reg++)
		DUMP_REG(reg);

	release_queue(kgd);

	WARN_ON_ONCE(i != HQD_N_REGS);
	*n_regs = i;

	return 0;
}

static int kgd_hqd_sdma_load(struct kgd_dev *kgd, void *mqd,
			     uint32_t __user *wptr, struct mm_struct *mm)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct v9_sdma_mqd *m;
<<<<<<< HEAD
	uint32_t sdma_base_addr, sdmax_gfx_context_cntl;
=======
	uint32_t sdma_rlc_reg_offset;
>>>>>>> upstream/android-13
	unsigned long end_jiffies;
	uint32_t data;
	uint64_t data64;
	uint64_t __user *wptr64 = (uint64_t __user *)wptr;

	m = get_sdma_mqd(mqd);
<<<<<<< HEAD
	sdma_base_addr = get_sdma_base_addr(adev, m->sdma_engine_id,
					    m->sdma_queue_id);
	sdmax_gfx_context_cntl = m->sdma_engine_id ?
		SOC15_REG_OFFSET(SDMA1, 0, mmSDMA1_GFX_CONTEXT_CNTL) :
		SOC15_REG_OFFSET(SDMA0, 0, mmSDMA0_GFX_CONTEXT_CNTL);

	WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_CNTL,
=======
	sdma_rlc_reg_offset = get_sdma_rlc_reg_offset(adev, m->sdma_engine_id,
					    m->sdma_queue_id);

	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_CNTL,
>>>>>>> upstream/android-13
		m->sdmax_rlcx_rb_cntl & (~SDMA0_RLC0_RB_CNTL__RB_ENABLE_MASK));

	end_jiffies = msecs_to_jiffies(2000) + jiffies;
	while (true) {
<<<<<<< HEAD
		data = RREG32(sdma_base_addr + mmSDMA0_RLC0_CONTEXT_STATUS);
		if (data & SDMA0_RLC0_CONTEXT_STATUS__IDLE_MASK)
			break;
		if (time_after(jiffies, end_jiffies))
			return -ETIME;
		usleep_range(500, 1000);
	}
	data = RREG32(sdmax_gfx_context_cntl);
	data = REG_SET_FIELD(data, SDMA0_GFX_CONTEXT_CNTL,
			     RESUME_CTX, 0);
	WREG32(sdmax_gfx_context_cntl, data);

	WREG32(sdma_base_addr + mmSDMA0_RLC0_DOORBELL_OFFSET,
=======
		data = RREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_CONTEXT_STATUS);
		if (data & SDMA0_RLC0_CONTEXT_STATUS__IDLE_MASK)
			break;
		if (time_after(jiffies, end_jiffies)) {
			pr_err("SDMA RLC not idle in %s\n", __func__);
			return -ETIME;
		}
		usleep_range(500, 1000);
	}

	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_DOORBELL_OFFSET,
>>>>>>> upstream/android-13
	       m->sdmax_rlcx_doorbell_offset);

	data = REG_SET_FIELD(m->sdmax_rlcx_doorbell, SDMA0_RLC0_DOORBELL,
			     ENABLE, 1);
<<<<<<< HEAD
	WREG32(sdma_base_addr + mmSDMA0_RLC0_DOORBELL, data);
	WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_RPTR, m->sdmax_rlcx_rb_rptr);
	WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_RPTR_HI,
				m->sdmax_rlcx_rb_rptr_hi);

	WREG32(sdma_base_addr + mmSDMA0_RLC0_MINOR_PTR_UPDATE, 1);
	if (read_user_wptr(mm, wptr64, data64)) {
		WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_WPTR,
		       lower_32_bits(data64));
		WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_WPTR_HI,
		       upper_32_bits(data64));
	} else {
		WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_WPTR,
		       m->sdmax_rlcx_rb_rptr);
		WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_WPTR_HI,
		       m->sdmax_rlcx_rb_rptr_hi);
	}
	WREG32(sdma_base_addr + mmSDMA0_RLC0_MINOR_PTR_UPDATE, 0);

	WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_BASE, m->sdmax_rlcx_rb_base);
	WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_BASE_HI,
			m->sdmax_rlcx_rb_base_hi);
	WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_RPTR_ADDR_LO,
			m->sdmax_rlcx_rb_rptr_addr_lo);
	WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_RPTR_ADDR_HI,
=======
	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_DOORBELL, data);
	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_RPTR,
				m->sdmax_rlcx_rb_rptr);
	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_RPTR_HI,
				m->sdmax_rlcx_rb_rptr_hi);

	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_MINOR_PTR_UPDATE, 1);
	if (read_user_wptr(mm, wptr64, data64)) {
		WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_WPTR,
		       lower_32_bits(data64));
		WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_WPTR_HI,
		       upper_32_bits(data64));
	} else {
		WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_WPTR,
		       m->sdmax_rlcx_rb_rptr);
		WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_WPTR_HI,
		       m->sdmax_rlcx_rb_rptr_hi);
	}
	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_MINOR_PTR_UPDATE, 0);

	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_BASE, m->sdmax_rlcx_rb_base);
	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_BASE_HI,
			m->sdmax_rlcx_rb_base_hi);
	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_RPTR_ADDR_LO,
			m->sdmax_rlcx_rb_rptr_addr_lo);
	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_RPTR_ADDR_HI,
>>>>>>> upstream/android-13
			m->sdmax_rlcx_rb_rptr_addr_hi);

	data = REG_SET_FIELD(m->sdmax_rlcx_rb_cntl, SDMA0_RLC0_RB_CNTL,
			     RB_ENABLE, 1);
<<<<<<< HEAD
	WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_CNTL, data);
=======
	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_CNTL, data);
>>>>>>> upstream/android-13

	return 0;
}

static int kgd_hqd_sdma_dump(struct kgd_dev *kgd,
			     uint32_t engine_id, uint32_t queue_id,
			     uint32_t (**dump)[2], uint32_t *n_regs)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
<<<<<<< HEAD
	uint32_t sdma_base_addr = get_sdma_base_addr(adev, engine_id, queue_id);
=======
	uint32_t sdma_rlc_reg_offset = get_sdma_rlc_reg_offset(adev,
			engine_id, queue_id);
>>>>>>> upstream/android-13
	uint32_t i = 0, reg;
#undef HQD_N_REGS
#define HQD_N_REGS (19+6+7+10)

	*dump = kmalloc_array(HQD_N_REGS * 2, sizeof(uint32_t), GFP_KERNEL);
	if (*dump == NULL)
		return -ENOMEM;

	for (reg = mmSDMA0_RLC0_RB_CNTL; reg <= mmSDMA0_RLC0_DOORBELL; reg++)
<<<<<<< HEAD
		DUMP_REG(sdma_base_addr + reg);
	for (reg = mmSDMA0_RLC0_STATUS; reg <= mmSDMA0_RLC0_CSA_ADDR_HI; reg++)
		DUMP_REG(sdma_base_addr + reg);
	for (reg = mmSDMA0_RLC0_IB_SUB_REMAIN;
	     reg <= mmSDMA0_RLC0_MINOR_PTR_UPDATE; reg++)
		DUMP_REG(sdma_base_addr + reg);
	for (reg = mmSDMA0_RLC0_MIDCMD_DATA0;
	     reg <= mmSDMA0_RLC0_MIDCMD_CNTL; reg++)
		DUMP_REG(sdma_base_addr + reg);
=======
		DUMP_REG(sdma_rlc_reg_offset + reg);
	for (reg = mmSDMA0_RLC0_STATUS; reg <= mmSDMA0_RLC0_CSA_ADDR_HI; reg++)
		DUMP_REG(sdma_rlc_reg_offset + reg);
	for (reg = mmSDMA0_RLC0_IB_SUB_REMAIN;
	     reg <= mmSDMA0_RLC0_MINOR_PTR_UPDATE; reg++)
		DUMP_REG(sdma_rlc_reg_offset + reg);
	for (reg = mmSDMA0_RLC0_MIDCMD_DATA0;
	     reg <= mmSDMA0_RLC0_MIDCMD_CNTL; reg++)
		DUMP_REG(sdma_rlc_reg_offset + reg);
>>>>>>> upstream/android-13

	WARN_ON_ONCE(i != HQD_N_REGS);
	*n_regs = i;

	return 0;
}

<<<<<<< HEAD
static bool kgd_hqd_is_occupied(struct kgd_dev *kgd, uint64_t queue_address,
=======
bool kgd_gfx_v9_hqd_is_occupied(struct kgd_dev *kgd, uint64_t queue_address,
>>>>>>> upstream/android-13
				uint32_t pipe_id, uint32_t queue_id)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	uint32_t act;
	bool retval = false;
	uint32_t low, high;

	acquire_queue(kgd, pipe_id, queue_id);
	act = RREG32(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_ACTIVE));
	if (act) {
		low = lower_32_bits(queue_address >> 8);
		high = upper_32_bits(queue_address >> 8);

		if (low == RREG32(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_BASE)) &&
		   high == RREG32(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_PQ_BASE_HI)))
			retval = true;
	}
	release_queue(kgd);
	return retval;
}

static bool kgd_hqd_sdma_is_occupied(struct kgd_dev *kgd, void *mqd)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct v9_sdma_mqd *m;
<<<<<<< HEAD
	uint32_t sdma_base_addr;
	uint32_t sdma_rlc_rb_cntl;

	m = get_sdma_mqd(mqd);
	sdma_base_addr = get_sdma_base_addr(adev, m->sdma_engine_id,
					    m->sdma_queue_id);

	sdma_rlc_rb_cntl = RREG32(sdma_base_addr + mmSDMA0_RLC0_RB_CNTL);
=======
	uint32_t sdma_rlc_reg_offset;
	uint32_t sdma_rlc_rb_cntl;

	m = get_sdma_mqd(mqd);
	sdma_rlc_reg_offset = get_sdma_rlc_reg_offset(adev, m->sdma_engine_id,
					    m->sdma_queue_id);

	sdma_rlc_rb_cntl = RREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_CNTL);
>>>>>>> upstream/android-13

	if (sdma_rlc_rb_cntl & SDMA0_RLC0_RB_CNTL__RB_ENABLE_MASK)
		return true;

	return false;
}

<<<<<<< HEAD
static int kgd_hqd_destroy(struct kgd_dev *kgd, void *mqd,
=======
int kgd_gfx_v9_hqd_destroy(struct kgd_dev *kgd, void *mqd,
>>>>>>> upstream/android-13
				enum kfd_preempt_type reset_type,
				unsigned int utimeout, uint32_t pipe_id,
				uint32_t queue_id)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	enum hqd_dequeue_request_type type;
	unsigned long end_jiffies;
	uint32_t temp;
	struct v9_mqd *m = get_mqd(mqd);

<<<<<<< HEAD
	if (adev->in_gpu_reset)
=======
	if (amdgpu_in_reset(adev))
>>>>>>> upstream/android-13
		return -EIO;

	acquire_queue(kgd, pipe_id, queue_id);

	if (m->cp_hqd_vmid == 0)
<<<<<<< HEAD
		WREG32_FIELD15(GC, 0, RLC_CP_SCHEDULERS, scheduler1, 0);
=======
		WREG32_FIELD15_RLC(GC, 0, RLC_CP_SCHEDULERS, scheduler1, 0);
>>>>>>> upstream/android-13

	switch (reset_type) {
	case KFD_PREEMPT_TYPE_WAVEFRONT_DRAIN:
		type = DRAIN_PIPE;
		break;
	case KFD_PREEMPT_TYPE_WAVEFRONT_RESET:
		type = RESET_WAVES;
		break;
<<<<<<< HEAD
=======
	case KFD_PREEMPT_TYPE_WAVEFRONT_SAVE:
		type = SAVE_WAVES;
		break;
>>>>>>> upstream/android-13
	default:
		type = DRAIN_PIPE;
		break;
	}

<<<<<<< HEAD
	WREG32(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_DEQUEUE_REQUEST), type);
=======
	WREG32_RLC(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_DEQUEUE_REQUEST), type);
>>>>>>> upstream/android-13

	end_jiffies = (utimeout * HZ / 1000) + jiffies;
	while (true) {
		temp = RREG32(SOC15_REG_OFFSET(GC, 0, mmCP_HQD_ACTIVE));
		if (!(temp & CP_HQD_ACTIVE__ACTIVE_MASK))
			break;
		if (time_after(jiffies, end_jiffies)) {
			pr_err("cp queue preemption time out.\n");
			release_queue(kgd);
			return -ETIME;
		}
		usleep_range(500, 1000);
	}

	release_queue(kgd);
	return 0;
}

static int kgd_hqd_sdma_destroy(struct kgd_dev *kgd, void *mqd,
				unsigned int utimeout)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct v9_sdma_mqd *m;
<<<<<<< HEAD
	uint32_t sdma_base_addr;
=======
	uint32_t sdma_rlc_reg_offset;
>>>>>>> upstream/android-13
	uint32_t temp;
	unsigned long end_jiffies = (utimeout * HZ / 1000) + jiffies;

	m = get_sdma_mqd(mqd);
<<<<<<< HEAD
	sdma_base_addr = get_sdma_base_addr(adev, m->sdma_engine_id,
					    m->sdma_queue_id);

	temp = RREG32(sdma_base_addr + mmSDMA0_RLC0_RB_CNTL);
	temp = temp & ~SDMA0_RLC0_RB_CNTL__RB_ENABLE_MASK;
	WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_CNTL, temp);

	while (true) {
		temp = RREG32(sdma_base_addr + mmSDMA0_RLC0_CONTEXT_STATUS);
		if (temp & SDMA0_RLC0_CONTEXT_STATUS__IDLE_MASK)
			break;
		if (time_after(jiffies, end_jiffies))
			return -ETIME;
		usleep_range(500, 1000);
	}

	WREG32(sdma_base_addr + mmSDMA0_RLC0_DOORBELL, 0);
	WREG32(sdma_base_addr + mmSDMA0_RLC0_RB_CNTL,
		RREG32(sdma_base_addr + mmSDMA0_RLC0_RB_CNTL) |
		SDMA0_RLC0_RB_CNTL__RB_ENABLE_MASK);

	m->sdmax_rlcx_rb_rptr = RREG32(sdma_base_addr + mmSDMA0_RLC0_RB_RPTR);
	m->sdmax_rlcx_rb_rptr_hi =
		RREG32(sdma_base_addr + mmSDMA0_RLC0_RB_RPTR_HI);
=======
	sdma_rlc_reg_offset = get_sdma_rlc_reg_offset(adev, m->sdma_engine_id,
					    m->sdma_queue_id);

	temp = RREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_CNTL);
	temp = temp & ~SDMA0_RLC0_RB_CNTL__RB_ENABLE_MASK;
	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_CNTL, temp);

	while (true) {
		temp = RREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_CONTEXT_STATUS);
		if (temp & SDMA0_RLC0_CONTEXT_STATUS__IDLE_MASK)
			break;
		if (time_after(jiffies, end_jiffies)) {
			pr_err("SDMA RLC not idle in %s\n", __func__);
			return -ETIME;
		}
		usleep_range(500, 1000);
	}

	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_DOORBELL, 0);
	WREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_CNTL,
		RREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_CNTL) |
		SDMA0_RLC0_RB_CNTL__RB_ENABLE_MASK);

	m->sdmax_rlcx_rb_rptr = RREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_RPTR);
	m->sdmax_rlcx_rb_rptr_hi =
		RREG32(sdma_rlc_reg_offset + mmSDMA0_RLC0_RB_RPTR_HI);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static bool get_atc_vmid_pasid_mapping_valid(struct kgd_dev *kgd,
							uint8_t vmid)
{
	uint32_t reg;
	struct amdgpu_device *adev = (struct amdgpu_device *) kgd;

	reg = RREG32(SOC15_REG_OFFSET(ATHUB, 0, mmATC_VMID0_PASID_MAPPING)
		     + vmid);
	return reg & ATC_VMID0_PASID_MAPPING__VALID_MASK;
}

static uint16_t get_atc_vmid_pasid_mapping_pasid(struct kgd_dev *kgd,
								uint8_t vmid)
{
	uint32_t reg;
	struct amdgpu_device *adev = (struct amdgpu_device *) kgd;

	reg = RREG32(SOC15_REG_OFFSET(ATHUB, 0, mmATC_VMID0_PASID_MAPPING)
		     + vmid);
	return reg & ATC_VMID0_PASID_MAPPING__PASID_MASK;
}

static void write_vmid_invalidate_request(struct kgd_dev *kgd, uint8_t vmid)
{
	struct amdgpu_device *adev = (struct amdgpu_device *) kgd;
	uint32_t req = (1 << vmid) |
		(0 << VM_INVALIDATE_ENG16_REQ__FLUSH_TYPE__SHIFT) | /* legacy */
		VM_INVALIDATE_ENG16_REQ__INVALIDATE_L2_PTES_MASK |
		VM_INVALIDATE_ENG16_REQ__INVALIDATE_L2_PDE0_MASK |
		VM_INVALIDATE_ENG16_REQ__INVALIDATE_L2_PDE1_MASK |
		VM_INVALIDATE_ENG16_REQ__INVALIDATE_L2_PDE2_MASK |
		VM_INVALIDATE_ENG16_REQ__INVALIDATE_L1_PTES_MASK;

	mutex_lock(&adev->srbm_mutex);

	/* Use legacy mode tlb invalidation.
	 *
	 * Currently on Raven the code below is broken for anything but
	 * legacy mode due to a MMHUB power gating problem. A workaround
	 * is for MMHUB to wait until the condition PER_VMID_INVALIDATE_REQ
	 * == PER_VMID_INVALIDATE_ACK instead of simply waiting for the ack
	 * bit.
	 *
	 * TODO 1: agree on the right set of invalidation registers for
	 * KFD use. Use the last one for now. Invalidate both GC and
	 * MMHUB.
	 *
	 * TODO 2: support range-based invalidation, requires kfg2kgd
	 * interface change
	 */
	WREG32(SOC15_REG_OFFSET(GC, 0, mmVM_INVALIDATE_ENG16_ADDR_RANGE_LO32),
				0xffffffff);
	WREG32(SOC15_REG_OFFSET(GC, 0, mmVM_INVALIDATE_ENG16_ADDR_RANGE_HI32),
				0x0000001f);

	WREG32(SOC15_REG_OFFSET(MMHUB, 0,
				mmMMHUB_VM_INVALIDATE_ENG16_ADDR_RANGE_LO32),
				0xffffffff);
	WREG32(SOC15_REG_OFFSET(MMHUB, 0,
				mmMMHUB_VM_INVALIDATE_ENG16_ADDR_RANGE_HI32),
				0x0000001f);

	WREG32(SOC15_REG_OFFSET(GC, 0, mmVM_INVALIDATE_ENG16_REQ), req);

	WREG32(SOC15_REG_OFFSET(MMHUB, 0, mmMMHUB_VM_INVALIDATE_ENG16_REQ),
				req);

	while (!(RREG32(SOC15_REG_OFFSET(GC, 0, mmVM_INVALIDATE_ENG16_ACK)) &
					(1 << vmid)))
		cpu_relax();

	while (!(RREG32(SOC15_REG_OFFSET(MMHUB, 0,
					mmMMHUB_VM_INVALIDATE_ENG16_ACK)) &
					(1 << vmid)))
		cpu_relax();

	mutex_unlock(&adev->srbm_mutex);

}

static int invalidate_tlbs_with_kiq(struct amdgpu_device *adev, uint16_t pasid)
{
	signed long r;
	uint32_t seq;
	struct amdgpu_ring *ring = &adev->gfx.kiq.ring;

	spin_lock(&adev->gfx.kiq.ring_lock);
	amdgpu_ring_alloc(ring, 12); /* fence + invalidate_tlbs package*/
	amdgpu_ring_write(ring, PACKET3(PACKET3_INVALIDATE_TLBS, 0));
	amdgpu_ring_write(ring,
			PACKET3_INVALIDATE_TLBS_DST_SEL(1) |
			PACKET3_INVALIDATE_TLBS_ALL_HUB(1) |
			PACKET3_INVALIDATE_TLBS_PASID(pasid) |
			PACKET3_INVALIDATE_TLBS_FLUSH_TYPE(0)); /* legacy */
	amdgpu_fence_emit_polling(ring, &seq);
	amdgpu_ring_commit(ring);
	spin_unlock(&adev->gfx.kiq.ring_lock);

	r = amdgpu_fence_wait_polling(ring, seq, adev->usec_timeout);
	if (r < 1) {
		DRM_ERROR("wait for kiq fence error: %ld.\n", r);
		return -ETIME;
	}

	return 0;
}

static int invalidate_tlbs(struct kgd_dev *kgd, uint16_t pasid)
{
	struct amdgpu_device *adev = (struct amdgpu_device *) kgd;
	int vmid;
	struct amdgpu_ring *ring = &adev->gfx.kiq.ring;

	if (adev->in_gpu_reset)
		return -EIO;

	if (ring->ready)
		return invalidate_tlbs_with_kiq(adev, pasid);

	for (vmid = 0; vmid < 16; vmid++) {
		if (!amdgpu_amdkfd_is_kfd_vmid(adev, vmid))
			continue;
		if (get_atc_vmid_pasid_mapping_valid(kgd, vmid)) {
			if (get_atc_vmid_pasid_mapping_pasid(kgd, vmid)
				== pasid) {
				write_vmid_invalidate_request(kgd, vmid);
				break;
			}
		}
	}

	return 0;
}

static int invalidate_tlbs_vmid(struct kgd_dev *kgd, uint16_t vmid)
{
	struct amdgpu_device *adev = (struct amdgpu_device *) kgd;

	if (!amdgpu_amdkfd_is_kfd_vmid(adev, vmid)) {
		pr_err("non kfd vmid %d\n", vmid);
		return 0;
	}

	write_vmid_invalidate_request(kgd, vmid);
	return 0;
}

static int kgd_address_watch_disable(struct kgd_dev *kgd)
=======
bool kgd_gfx_v9_get_atc_vmid_pasid_mapping_info(struct kgd_dev *kgd,
					uint8_t vmid, uint16_t *p_pasid)
{
	uint32_t value;
	struct amdgpu_device *adev = (struct amdgpu_device *) kgd;

	value = RREG32(SOC15_REG_OFFSET(ATHUB, 0, mmATC_VMID0_PASID_MAPPING)
		     + vmid);
	*p_pasid = value & ATC_VMID0_PASID_MAPPING__PASID_MASK;

	return !!(value & ATC_VMID0_PASID_MAPPING__VALID_MASK);
}

int kgd_gfx_v9_address_watch_disable(struct kgd_dev *kgd)
>>>>>>> upstream/android-13
{
	return 0;
}

<<<<<<< HEAD
static int kgd_address_watch_execute(struct kgd_dev *kgd,
=======
int kgd_gfx_v9_address_watch_execute(struct kgd_dev *kgd,
>>>>>>> upstream/android-13
					unsigned int watch_point_id,
					uint32_t cntl_val,
					uint32_t addr_hi,
					uint32_t addr_lo)
{
	return 0;
}

<<<<<<< HEAD
static int kgd_wave_control_execute(struct kgd_dev *kgd,
=======
int kgd_gfx_v9_wave_control_execute(struct kgd_dev *kgd,
>>>>>>> upstream/android-13
					uint32_t gfx_index_val,
					uint32_t sq_cmd)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	uint32_t data = 0;

	mutex_lock(&adev->grbm_idx_mutex);

<<<<<<< HEAD
	WREG32(SOC15_REG_OFFSET(GC, 0, mmGRBM_GFX_INDEX), gfx_index_val);
=======
	WREG32_SOC15_RLC_SHADOW(GC, 0, mmGRBM_GFX_INDEX, gfx_index_val);
>>>>>>> upstream/android-13
	WREG32(SOC15_REG_OFFSET(GC, 0, mmSQ_CMD), sq_cmd);

	data = REG_SET_FIELD(data, GRBM_GFX_INDEX,
		INSTANCE_BROADCAST_WRITES, 1);
	data = REG_SET_FIELD(data, GRBM_GFX_INDEX,
		SH_BROADCAST_WRITES, 1);
	data = REG_SET_FIELD(data, GRBM_GFX_INDEX,
		SE_BROADCAST_WRITES, 1);

<<<<<<< HEAD
	WREG32(SOC15_REG_OFFSET(GC, 0, mmGRBM_GFX_INDEX), data);
=======
	WREG32_SOC15_RLC_SHADOW(GC, 0, mmGRBM_GFX_INDEX, data);
>>>>>>> upstream/android-13
	mutex_unlock(&adev->grbm_idx_mutex);

	return 0;
}

<<<<<<< HEAD
static uint32_t kgd_address_watch_get_offset(struct kgd_dev *kgd,
=======
uint32_t kgd_gfx_v9_address_watch_get_offset(struct kgd_dev *kgd,
>>>>>>> upstream/android-13
					unsigned int watch_point_id,
					unsigned int reg_offset)
{
	return 0;
}

<<<<<<< HEAD
static void set_scratch_backing_va(struct kgd_dev *kgd,
					uint64_t va, uint32_t vmid)
{
	/* No longer needed on GFXv9. The scratch base address is
	 * passed to the shader by the CP. It's the user mode driver's
	 * responsibility.
	 */
}

/* FIXME: Does this need to be ASIC-specific code? */
static uint16_t get_fw_version(struct kgd_dev *kgd, enum kgd_engine_type type)
{
	struct amdgpu_device *adev = (struct amdgpu_device *) kgd;
	const union amdgpu_firmware_header *hdr;

	switch (type) {
	case KGD_ENGINE_PFP:
		hdr = (const union amdgpu_firmware_header *)adev->gfx.pfp_fw->data;
		break;

	case KGD_ENGINE_ME:
		hdr = (const union amdgpu_firmware_header *)adev->gfx.me_fw->data;
		break;

	case KGD_ENGINE_CE:
		hdr = (const union amdgpu_firmware_header *)adev->gfx.ce_fw->data;
		break;

	case KGD_ENGINE_MEC1:
		hdr = (const union amdgpu_firmware_header *)adev->gfx.mec_fw->data;
		break;

	case KGD_ENGINE_MEC2:
		hdr = (const union amdgpu_firmware_header *)adev->gfx.mec2_fw->data;
		break;

	case KGD_ENGINE_RLC:
		hdr = (const union amdgpu_firmware_header *)adev->gfx.rlc_fw->data;
		break;

	case KGD_ENGINE_SDMA1:
		hdr = (const union amdgpu_firmware_header *)adev->sdma.instance[0].fw->data;
		break;

	case KGD_ENGINE_SDMA2:
		hdr = (const union amdgpu_firmware_header *)adev->sdma.instance[1].fw->data;
		break;

	default:
		return 0;
	}

	if (hdr == NULL)
		return 0;

	/* Only 12 bit in use*/
	return hdr->common.ucode_version;
}

static void set_vm_context_page_table_base(struct kgd_dev *kgd, uint32_t vmid,
		uint32_t page_table_base)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	uint64_t base = (uint64_t)page_table_base << PAGE_SHIFT |
		AMDGPU_PTE_VALID;
=======
void kgd_gfx_v9_set_vm_context_page_table_base(struct kgd_dev *kgd,
			uint32_t vmid, uint64_t page_table_base)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
>>>>>>> upstream/android-13

	if (!amdgpu_amdkfd_is_kfd_vmid(adev, vmid)) {
		pr_err("trying to set page table base for wrong VMID %u\n",
		       vmid);
		return;
	}

<<<<<<< HEAD
	/* TODO: take advantage of per-process address space size. For
	 * now, all processes share the same address space size, like
	 * on GFX8 and older.
	 */
	WREG32(SOC15_REG_OFFSET(MMHUB, 0, mmMMHUB_VM_CONTEXT0_PAGE_TABLE_START_ADDR_LO32) + (vmid*2), 0);
	WREG32(SOC15_REG_OFFSET(MMHUB, 0, mmMMHUB_VM_CONTEXT0_PAGE_TABLE_START_ADDR_HI32) + (vmid*2), 0);

	WREG32(SOC15_REG_OFFSET(MMHUB, 0, mmMMHUB_VM_CONTEXT0_PAGE_TABLE_END_ADDR_LO32) + (vmid*2),
			lower_32_bits(adev->vm_manager.max_pfn - 1));
	WREG32(SOC15_REG_OFFSET(MMHUB, 0, mmMMHUB_VM_CONTEXT0_PAGE_TABLE_END_ADDR_HI32) + (vmid*2),
			upper_32_bits(adev->vm_manager.max_pfn - 1));

	WREG32(SOC15_REG_OFFSET(MMHUB, 0, mmMMHUB_VM_CONTEXT0_PAGE_TABLE_BASE_ADDR_LO32) + (vmid*2), lower_32_bits(base));
	WREG32(SOC15_REG_OFFSET(MMHUB, 0, mmMMHUB_VM_CONTEXT0_PAGE_TABLE_BASE_ADDR_HI32) + (vmid*2), upper_32_bits(base));

	WREG32(SOC15_REG_OFFSET(GC, 0, mmVM_CONTEXT0_PAGE_TABLE_START_ADDR_LO32) + (vmid*2), 0);
	WREG32(SOC15_REG_OFFSET(GC, 0, mmVM_CONTEXT0_PAGE_TABLE_START_ADDR_HI32) + (vmid*2), 0);

	WREG32(SOC15_REG_OFFSET(GC, 0, mmVM_CONTEXT0_PAGE_TABLE_END_ADDR_LO32) + (vmid*2),
			lower_32_bits(adev->vm_manager.max_pfn - 1));
	WREG32(SOC15_REG_OFFSET(GC, 0, mmVM_CONTEXT0_PAGE_TABLE_END_ADDR_HI32) + (vmid*2),
			upper_32_bits(adev->vm_manager.max_pfn - 1));

	WREG32(SOC15_REG_OFFSET(GC, 0, mmVM_CONTEXT0_PAGE_TABLE_BASE_ADDR_LO32) + (vmid*2), lower_32_bits(base));
	WREG32(SOC15_REG_OFFSET(GC, 0, mmVM_CONTEXT0_PAGE_TABLE_BASE_ADDR_HI32) + (vmid*2), upper_32_bits(base));
}
=======
	adev->mmhub.funcs->setup_vm_pt_regs(adev, vmid, page_table_base);

	adev->gfxhub.funcs->setup_vm_pt_regs(adev, vmid, page_table_base);
}

static void lock_spi_csq_mutexes(struct amdgpu_device *adev)
{
	mutex_lock(&adev->srbm_mutex);
	mutex_lock(&adev->grbm_idx_mutex);

}

static void unlock_spi_csq_mutexes(struct amdgpu_device *adev)
{
	mutex_unlock(&adev->grbm_idx_mutex);
	mutex_unlock(&adev->srbm_mutex);
}

/**
 * get_wave_count: Read device registers to get number of waves in flight for
 * a particular queue. The method also returns the VMID associated with the
 * queue.
 *
 * @adev: Handle of device whose registers are to be read
 * @queue_idx: Index of queue in the queue-map bit-field
 * @wave_cnt: Output parameter updated with number of waves in flight
 * @vmid: Output parameter updated with VMID of queue whose wave count
 * is being collected
 */
static void get_wave_count(struct amdgpu_device *adev, int queue_idx,
		int *wave_cnt, int *vmid)
{
	int pipe_idx;
	int queue_slot;
	unsigned int reg_val;

	/*
	 * Program GRBM with appropriate MEID, PIPEID, QUEUEID and VMID
	 * parameters to read out waves in flight. Get VMID if there are
	 * non-zero waves in flight.
	 */
	*vmid = 0xFF;
	*wave_cnt = 0;
	pipe_idx = queue_idx / adev->gfx.mec.num_queue_per_pipe;
	queue_slot = queue_idx % adev->gfx.mec.num_queue_per_pipe;
	soc15_grbm_select(adev, 1, pipe_idx, queue_slot, 0);
	reg_val = RREG32(SOC15_REG_OFFSET(GC, 0, mmSPI_CSQ_WF_ACTIVE_COUNT_0) +
			 queue_slot);
	*wave_cnt = reg_val & SPI_CSQ_WF_ACTIVE_COUNT_0__COUNT_MASK;
	if (*wave_cnt != 0)
		*vmid = (RREG32_SOC15(GC, 0, mmCP_HQD_VMID) &
			 CP_HQD_VMID__VMID_MASK) >> CP_HQD_VMID__VMID__SHIFT;
}

/**
 * kgd_gfx_v9_get_cu_occupancy: Reads relevant registers associated with each
 * shader engine and aggregates the number of waves that are in flight for the
 * process whose pasid is provided as a parameter. The process could have ZERO
 * or more queues running and submitting waves to compute units.
 *
 * @kgd: Handle of device from which to get number of waves in flight
 * @pasid: Identifies the process for which this query call is invoked
 * @pasid_wave_cnt: Output parameter updated with number of waves in flight that
 * belong to process with given pasid
 * @max_waves_per_cu: Output parameter updated with maximum number of waves
 * possible per Compute Unit
 *
 * Note: It's possible that the device has too many queues (oversubscription)
 * in which case a VMID could be remapped to a different PASID. This could lead
 * to an iaccurate wave count. Following is a high-level sequence:
 *    Time T1: vmid = getVmid(); vmid is associated with Pasid P1
 *    Time T2: passId = getPasId(vmid); vmid is associated with Pasid P2
 * In the sequence above wave count obtained from time T1 will be incorrectly
 * lost or added to total wave count.
 *
 * The registers that provide the waves in flight are:
 *
 *  SPI_CSQ_WF_ACTIVE_STATUS - bit-map of queues per pipe. The bit is ON if a
 *  queue is slotted, OFF if there is no queue. A process could have ZERO or
 *  more queues slotted and submitting waves to be run on compute units. Even
 *  when there is a queue it is possible there could be zero wave fronts, this
 *  can happen when queue is waiting on top-of-pipe events - e.g. waitRegMem
 *  command
 *
 *  For each bit that is ON from above:
 *
 *    Read (SPI_CSQ_WF_ACTIVE_COUNT_0 + queue_idx) register. It provides the
 *    number of waves that are in flight for the queue at specified index. The
 *    index ranges from 0 to 7.
 *
 *    If non-zero waves are in flight, read CP_HQD_VMID register to obtain VMID
 *    of the wave(s).
 *
 *    Determine if VMID from above step maps to pasid provided as parameter. If
 *    it matches agrregate the wave count. That the VMID will not match pasid is
 *    a normal condition i.e. a device is expected to support multiple queues
 *    from multiple proceses.
 *
 *  Reading registers referenced above involves programming GRBM appropriately
 */
void kgd_gfx_v9_get_cu_occupancy(struct kgd_dev *kgd, int pasid,
		int *pasid_wave_cnt, int *max_waves_per_cu)
{
	int qidx;
	int vmid;
	int se_idx;
	int sh_idx;
	int se_cnt;
	int sh_cnt;
	int wave_cnt;
	int queue_map;
	int pasid_tmp;
	int max_queue_cnt;
	int vmid_wave_cnt = 0;
	struct amdgpu_device *adev;
	DECLARE_BITMAP(cp_queue_bitmap, KGD_MAX_QUEUES);

	adev = get_amdgpu_device(kgd);
	lock_spi_csq_mutexes(adev);
	soc15_grbm_select(adev, 1, 0, 0, 0);

	/*
	 * Iterate through the shader engines and arrays of the device
	 * to get number of waves in flight
	 */
	bitmap_complement(cp_queue_bitmap, adev->gfx.mec.queue_bitmap,
			  KGD_MAX_QUEUES);
	max_queue_cnt = adev->gfx.mec.num_pipe_per_mec *
			adev->gfx.mec.num_queue_per_pipe;
	sh_cnt = adev->gfx.config.max_sh_per_se;
	se_cnt = adev->gfx.config.max_shader_engines;
	for (se_idx = 0; se_idx < se_cnt; se_idx++) {
		for (sh_idx = 0; sh_idx < sh_cnt; sh_idx++) {

			gfx_v9_0_select_se_sh(adev, se_idx, sh_idx, 0xffffffff);
			queue_map = RREG32(SOC15_REG_OFFSET(GC, 0,
					   mmSPI_CSQ_WF_ACTIVE_STATUS));

			/*
			 * Assumption: queue map encodes following schema: four
			 * pipes per each micro-engine, with each pipe mapping
			 * eight queues. This schema is true for GFX9 devices
			 * and must be verified for newer device families
			 */
			for (qidx = 0; qidx < max_queue_cnt; qidx++) {

				/* Skip qeueus that are not associated with
				 * compute functions
				 */
				if (!test_bit(qidx, cp_queue_bitmap))
					continue;

				if (!(queue_map & (1 << qidx)))
					continue;

				/* Get number of waves in flight and aggregate them */
				get_wave_count(adev, qidx, &wave_cnt, &vmid);
				if (wave_cnt != 0) {
					pasid_tmp =
					  RREG32(SOC15_REG_OFFSET(OSSSYS, 0,
						 mmIH_VMID_0_LUT) + vmid);
					if (pasid_tmp == pasid)
						vmid_wave_cnt += wave_cnt;
				}
			}
		}
	}

	gfx_v9_0_select_se_sh(adev, 0xffffffff, 0xffffffff, 0xffffffff);
	soc15_grbm_select(adev, 0, 0, 0, 0);
	unlock_spi_csq_mutexes(adev);

	/* Update the output parameters and return */
	*pasid_wave_cnt = vmid_wave_cnt;
	*max_waves_per_cu = adev->gfx.cu_info.simd_per_cu *
				adev->gfx.cu_info.max_waves_per_simd;
}

void kgd_gfx_v9_program_trap_handler_settings(struct kgd_dev *kgd,
                        uint32_t vmid, uint64_t tba_addr, uint64_t tma_addr)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);

	lock_srbm(kgd, 0, 0, 0, vmid);

	/*
	 * Program TBA registers
	 */
	WREG32(SOC15_REG_OFFSET(GC, 0, mmSQ_SHADER_TBA_LO),
                        lower_32_bits(tba_addr >> 8));
	WREG32(SOC15_REG_OFFSET(GC, 0, mmSQ_SHADER_TBA_HI),
                        upper_32_bits(tba_addr >> 8));

	/*
	 * Program TMA registers
	 */
	WREG32(SOC15_REG_OFFSET(GC, 0, mmSQ_SHADER_TMA_LO),
			lower_32_bits(tma_addr >> 8));
	WREG32(SOC15_REG_OFFSET(GC, 0, mmSQ_SHADER_TMA_HI),
			upper_32_bits(tma_addr >> 8));

	unlock_srbm(kgd);
}

const struct kfd2kgd_calls gfx_v9_kfd2kgd = {
	.program_sh_mem_settings = kgd_gfx_v9_program_sh_mem_settings,
	.set_pasid_vmid_mapping = kgd_gfx_v9_set_pasid_vmid_mapping,
	.init_interrupts = kgd_gfx_v9_init_interrupts,
	.hqd_load = kgd_gfx_v9_hqd_load,
	.hiq_mqd_load = kgd_gfx_v9_hiq_mqd_load,
	.hqd_sdma_load = kgd_hqd_sdma_load,
	.hqd_dump = kgd_gfx_v9_hqd_dump,
	.hqd_sdma_dump = kgd_hqd_sdma_dump,
	.hqd_is_occupied = kgd_gfx_v9_hqd_is_occupied,
	.hqd_sdma_is_occupied = kgd_hqd_sdma_is_occupied,
	.hqd_destroy = kgd_gfx_v9_hqd_destroy,
	.hqd_sdma_destroy = kgd_hqd_sdma_destroy,
	.address_watch_disable = kgd_gfx_v9_address_watch_disable,
	.address_watch_execute = kgd_gfx_v9_address_watch_execute,
	.wave_control_execute = kgd_gfx_v9_wave_control_execute,
	.address_watch_get_offset = kgd_gfx_v9_address_watch_get_offset,
	.get_atc_vmid_pasid_mapping_info =
			kgd_gfx_v9_get_atc_vmid_pasid_mapping_info,
	.set_vm_context_page_table_base = kgd_gfx_v9_set_vm_context_page_table_base,
	.get_cu_occupancy = kgd_gfx_v9_get_cu_occupancy,
	.program_trap_handler_settings = kgd_gfx_v9_program_trap_handler_settings,
};
>>>>>>> upstream/android-13
