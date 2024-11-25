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
 */

#include "amdgpu_amdkfd.h"
<<<<<<< HEAD
#include "amd_shared.h"
#include <drm/drmP.h>
#include "amdgpu.h"
#include "amdgpu_gfx.h"
#include <linux/module.h>

const struct kgd2kfd_calls *kgd2kfd;
bool (*kgd2kfd_init_p)(unsigned int, const struct kgd2kfd_calls**);

static const unsigned int compute_vmid_bitmap = 0xFF00;

int amdgpu_amdkfd_init(void)
{
	int ret;

#if defined(CONFIG_HSA_AMD_MODULE)
	int (*kgd2kfd_init_p)(unsigned int, const struct kgd2kfd_calls**);

	kgd2kfd_init_p = symbol_request(kgd2kfd_init);

	if (kgd2kfd_init_p == NULL)
		return -ENOENT;

	ret = kgd2kfd_init_p(KFD_INTERFACE_VERSION, &kgd2kfd);
	if (ret) {
		symbol_put(kgd2kfd_init);
		kgd2kfd = NULL;
	}


#elif defined(CONFIG_HSA_AMD)

	ret = kgd2kfd_init(KFD_INTERFACE_VERSION, &kgd2kfd);
	if (ret)
		kgd2kfd = NULL;

#else
	kgd2kfd = NULL;
	ret = -ENOENT;
#endif

#if defined(CONFIG_HSA_AMD_MODULE) || defined(CONFIG_HSA_AMD)
	amdgpu_amdkfd_gpuvm_init_mem_limits();
#endif
=======
#include "amd_pcie.h"
#include "amd_shared.h"

#include "amdgpu.h"
#include "amdgpu_gfx.h"
#include "amdgpu_dma_buf.h"
#include <linux/module.h>
#include <linux/dma-buf.h>
#include "amdgpu_xgmi.h"
#include <uapi/linux/kfd_ioctl.h>

/* Total memory size in system memory and all GPU VRAM. Used to
 * estimate worst case amount of memory to reserve for page tables
 */
uint64_t amdgpu_amdkfd_total_mem_size;

static bool kfd_initialized;

int amdgpu_amdkfd_init(void)
{
	struct sysinfo si;
	int ret;

	si_meminfo(&si);
	amdgpu_amdkfd_total_mem_size = si.freeram - si.freehigh;
	amdgpu_amdkfd_total_mem_size *= si.mem_unit;

	ret = kgd2kfd_init();
	amdgpu_amdkfd_gpuvm_init_mem_limits();
	kfd_initialized = !ret;
>>>>>>> upstream/android-13

	return ret;
}

void amdgpu_amdkfd_fini(void)
{
<<<<<<< HEAD
	if (kgd2kfd) {
		kgd2kfd->exit();
		symbol_put(kgd2kfd_init);
=======
	if (kfd_initialized) {
		kgd2kfd_exit();
		kfd_initialized = false;
>>>>>>> upstream/android-13
	}
}

void amdgpu_amdkfd_device_probe(struct amdgpu_device *adev)
{
<<<<<<< HEAD
	const struct kfd2kgd_calls *kfd2kgd;

	if (!kgd2kfd)
		return;

	switch (adev->asic_type) {
#ifdef CONFIG_DRM_AMDGPU_CIK
	case CHIP_KAVERI:
	case CHIP_HAWAII:
		kfd2kgd = amdgpu_amdkfd_gfx_7_get_functions();
		break;
#endif
	case CHIP_CARRIZO:
	case CHIP_TONGA:
	case CHIP_FIJI:
	case CHIP_POLARIS10:
	case CHIP_POLARIS11:
		kfd2kgd = amdgpu_amdkfd_gfx_8_0_get_functions();
		break;
	case CHIP_VEGA10:
	case CHIP_RAVEN:
		kfd2kgd = amdgpu_amdkfd_gfx_9_0_get_functions();
		break;
	default:
		dev_info(adev->dev, "kfd not supported on this ASIC\n");
		return;
	}

	adev->kfd = kgd2kfd->probe((struct kgd_dev *)adev,
				   adev->pdev, kfd2kgd);
=======
	bool vf = amdgpu_sriov_vf(adev);

	if (!kfd_initialized)
		return;

	adev->kfd.dev = kgd2kfd_probe((struct kgd_dev *)adev,
				      adev->pdev, adev->asic_type, vf);

	if (adev->kfd.dev)
		amdgpu_amdkfd_total_mem_size += adev->gmc.real_vram_size;
>>>>>>> upstream/android-13
}

/**
 * amdgpu_doorbell_get_kfd_info - Report doorbell configuration required to
 *                                setup amdkfd
 *
 * @adev: amdgpu_device pointer
 * @aperture_base: output returning doorbell aperture base physical address
 * @aperture_size: output returning doorbell aperture size in bytes
 * @start_offset: output returning # of doorbell bytes reserved for amdgpu.
 *
 * amdgpu and amdkfd share the doorbell aperture. amdgpu sets it up,
 * takes doorbells required for its own rings and reports the setup to amdkfd.
 * amdgpu reserved doorbells are at the start of the doorbell aperture.
 */
static void amdgpu_doorbell_get_kfd_info(struct amdgpu_device *adev,
					 phys_addr_t *aperture_base,
					 size_t *aperture_size,
					 size_t *start_offset)
{
	/*
	 * The first num_doorbells are used by amdgpu.
	 * amdkfd takes whatever's left in the aperture.
	 */
	if (adev->doorbell.size > adev->doorbell.num_doorbells * sizeof(u32)) {
		*aperture_base = adev->doorbell.base;
		*aperture_size = adev->doorbell.size;
		*start_offset = adev->doorbell.num_doorbells * sizeof(u32);
	} else {
		*aperture_base = 0;
		*aperture_size = 0;
		*start_offset = 0;
	}
}

void amdgpu_amdkfd_device_init(struct amdgpu_device *adev)
{
	int i;
	int last_valid_bit;
<<<<<<< HEAD
	if (adev->kfd) {
		struct kgd2kfd_shared_resources gpu_resources = {
			.compute_vmid_bitmap = compute_vmid_bitmap,
=======

	if (adev->kfd.dev) {
		struct kgd2kfd_shared_resources gpu_resources = {
			.compute_vmid_bitmap =
				((1 << AMDGPU_NUM_VMID) - 1) -
				((1 << adev->vm_manager.first_kfd_vmid) - 1),
>>>>>>> upstream/android-13
			.num_pipe_per_mec = adev->gfx.mec.num_pipe_per_mec,
			.num_queue_per_pipe = adev->gfx.mec.num_queue_per_pipe,
			.gpuvm_size = min(adev->vm_manager.max_pfn
					  << AMDGPU_GPU_PAGE_SHIFT,
<<<<<<< HEAD
					  AMDGPU_VA_HOLE_START),
			.drm_render_minor = adev->ddev->render->index
		};

		/* this is going to have a few of the MSBs set that we need to
		 * clear */
		bitmap_complement(gpu_resources.queue_bitmap,
				  adev->gfx.mec.queue_bitmap,
				  KGD_MAX_QUEUES);

		/* remove the KIQ bit as well */
		if (adev->gfx.kiq.ring.ready)
			clear_bit(amdgpu_gfx_queue_to_bit(adev,
							  adev->gfx.kiq.ring.me - 1,
							  adev->gfx.kiq.ring.pipe,
							  adev->gfx.kiq.ring.queue),
				  gpu_resources.queue_bitmap);

		/* According to linux/bitmap.h we shouldn't use bitmap_clear if
		 * nbits is not compile time constant */
=======
					  AMDGPU_GMC_HOLE_START),
			.drm_render_minor = adev_to_drm(adev)->render->index,
			.sdma_doorbell_idx = adev->doorbell_index.sdma_engine,

		};

		/* this is going to have a few of the MSBs set that we need to
		 * clear
		 */
		bitmap_complement(gpu_resources.cp_queue_bitmap,
				  adev->gfx.mec.queue_bitmap,
				  KGD_MAX_QUEUES);

		/* According to linux/bitmap.h we shouldn't use bitmap_clear if
		 * nbits is not compile time constant
		 */
>>>>>>> upstream/android-13
		last_valid_bit = 1 /* only first MEC can have compute queues */
				* adev->gfx.mec.num_pipe_per_mec
				* adev->gfx.mec.num_queue_per_pipe;
		for (i = last_valid_bit; i < KGD_MAX_QUEUES; ++i)
<<<<<<< HEAD
			clear_bit(i, gpu_resources.queue_bitmap);
=======
			clear_bit(i, gpu_resources.cp_queue_bitmap);
>>>>>>> upstream/android-13

		amdgpu_doorbell_get_kfd_info(adev,
				&gpu_resources.doorbell_physical_address,
				&gpu_resources.doorbell_aperture_size,
				&gpu_resources.doorbell_start_offset);
<<<<<<< HEAD
		if (adev->asic_type >= CHIP_VEGA10) {
			/* On SOC15 the BIF is involved in routing
			 * doorbells using the low 12 bits of the
			 * address. Communicate the assignments to
			 * KFD. KFD uses two doorbell pages per
			 * process in case of 64-bit doorbells so we
			 * can use each doorbell assignment twice.
			 */
			gpu_resources.sdma_doorbell[0][0] =
				AMDGPU_DOORBELL64_sDMA_ENGINE0;
			gpu_resources.sdma_doorbell[0][1] =
				AMDGPU_DOORBELL64_sDMA_ENGINE0 + 0x200;
			gpu_resources.sdma_doorbell[1][0] =
				AMDGPU_DOORBELL64_sDMA_ENGINE1;
			gpu_resources.sdma_doorbell[1][1] =
				AMDGPU_DOORBELL64_sDMA_ENGINE1 + 0x200;
			/* Doorbells 0x0f0-0ff and 0x2f0-2ff are reserved for
			 * SDMA, IH and VCN. So don't use them for the CP.
			 */
			gpu_resources.reserved_doorbell_mask = 0x1f0;
			gpu_resources.reserved_doorbell_val  = 0x0f0;
		}

		kgd2kfd->device_init(adev->kfd, &gpu_resources);
	}
}

void amdgpu_amdkfd_device_fini(struct amdgpu_device *adev)
{
	if (adev->kfd) {
		kgd2kfd->device_exit(adev->kfd);
		adev->kfd = NULL;
=======

		/* Since SOC15, BIF starts to statically use the
		 * lower 12 bits of doorbell addresses for routing
		 * based on settings in registers like
		 * SDMA0_DOORBELL_RANGE etc..
		 * In order to route a doorbell to CP engine, the lower
		 * 12 bits of its address has to be outside the range
		 * set for SDMA, VCN, and IH blocks.
		 */
		if (adev->asic_type >= CHIP_VEGA10) {
			gpu_resources.non_cp_doorbells_start =
					adev->doorbell_index.first_non_cp;
			gpu_resources.non_cp_doorbells_end =
					adev->doorbell_index.last_non_cp;
		}

		adev->kfd.init_complete = kgd2kfd_device_init(adev->kfd.dev,
						adev_to_drm(adev), &gpu_resources);
	}
}

void amdgpu_amdkfd_device_fini_sw(struct amdgpu_device *adev)
{
	if (adev->kfd.dev) {
		kgd2kfd_device_exit(adev->kfd.dev);
		adev->kfd.dev = NULL;
>>>>>>> upstream/android-13
	}
}

void amdgpu_amdkfd_interrupt(struct amdgpu_device *adev,
		const void *ih_ring_entry)
{
<<<<<<< HEAD
	if (adev->kfd)
		kgd2kfd->interrupt(adev->kfd, ih_ring_entry);
}

void amdgpu_amdkfd_suspend(struct amdgpu_device *adev)
{
	if (adev->kfd)
		kgd2kfd->suspend(adev->kfd);
}

int amdgpu_amdkfd_resume(struct amdgpu_device *adev)
{
	int r = 0;

	if (adev->kfd)
		r = kgd2kfd->resume(adev->kfd);
=======
	if (adev->kfd.dev)
		kgd2kfd_interrupt(adev->kfd.dev, ih_ring_entry);
}

void amdgpu_amdkfd_suspend(struct amdgpu_device *adev, bool run_pm)
{
	if (adev->kfd.dev)
		kgd2kfd_suspend(adev->kfd.dev, run_pm);
}

int amdgpu_amdkfd_resume_iommu(struct amdgpu_device *adev)
{
	int r = 0;

	if (adev->kfd.dev)
		r = kgd2kfd_resume_iommu(adev->kfd.dev);

	return r;
}

int amdgpu_amdkfd_resume(struct amdgpu_device *adev, bool run_pm)
{
	int r = 0;

	if (adev->kfd.dev)
		r = kgd2kfd_resume(adev->kfd.dev, run_pm);
>>>>>>> upstream/android-13

	return r;
}

int amdgpu_amdkfd_pre_reset(struct amdgpu_device *adev)
{
	int r = 0;

<<<<<<< HEAD
	if (adev->kfd)
		r = kgd2kfd->pre_reset(adev->kfd);
=======
	if (adev->kfd.dev)
		r = kgd2kfd_pre_reset(adev->kfd.dev);
>>>>>>> upstream/android-13

	return r;
}

int amdgpu_amdkfd_post_reset(struct amdgpu_device *adev)
{
	int r = 0;

<<<<<<< HEAD
	if (adev->kfd)
		r = kgd2kfd->post_reset(adev->kfd);
=======
	if (adev->kfd.dev)
		r = kgd2kfd_post_reset(adev->kfd.dev);
>>>>>>> upstream/android-13

	return r;
}

void amdgpu_amdkfd_gpu_reset(struct kgd_dev *kgd)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

<<<<<<< HEAD
	amdgpu_device_gpu_recover(adev, NULL, false);
}

int alloc_gtt_mem(struct kgd_dev *kgd, size_t size,
			void **mem_obj, uint64_t *gpu_addr,
			void **cpu_ptr, bool mqd_gfx9)
=======
	if (amdgpu_device_should_recover_gpu(adev))
		amdgpu_device_gpu_recover(adev, NULL);
}

int amdgpu_amdkfd_alloc_gtt_mem(struct kgd_dev *kgd, size_t size,
				void **mem_obj, uint64_t *gpu_addr,
				void **cpu_ptr, bool cp_mqd_gfx9)
>>>>>>> upstream/android-13
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;
	struct amdgpu_bo *bo = NULL;
	struct amdgpu_bo_param bp;
	int r;
	void *cpu_ptr_tmp = NULL;

	memset(&bp, 0, sizeof(bp));
	bp.size = size;
	bp.byte_align = PAGE_SIZE;
	bp.domain = AMDGPU_GEM_DOMAIN_GTT;
	bp.flags = AMDGPU_GEM_CREATE_CPU_GTT_USWC;
	bp.type = ttm_bo_type_kernel;
	bp.resv = NULL;
<<<<<<< HEAD

	if (mqd_gfx9)
		bp.flags |= AMDGPU_GEM_CREATE_MQD_GFX9;
=======
	bp.bo_ptr_size = sizeof(struct amdgpu_bo);

	if (cp_mqd_gfx9)
		bp.flags |= AMDGPU_GEM_CREATE_CP_MQD_GFX9;
>>>>>>> upstream/android-13

	r = amdgpu_bo_create(adev, &bp, &bo);
	if (r) {
		dev_err(adev->dev,
			"failed to allocate BO for amdkfd (%d)\n", r);
		return r;
	}

	/* map the buffer */
	r = amdgpu_bo_reserve(bo, true);
	if (r) {
		dev_err(adev->dev, "(%d) failed to reserve bo for amdkfd\n", r);
		goto allocate_mem_reserve_bo_failed;
	}

	r = amdgpu_bo_pin(bo, AMDGPU_GEM_DOMAIN_GTT);
	if (r) {
		dev_err(adev->dev, "(%d) failed to pin bo for amdkfd\n", r);
		goto allocate_mem_pin_bo_failed;
	}

	r = amdgpu_ttm_alloc_gart(&bo->tbo);
	if (r) {
		dev_err(adev->dev, "%p bind failed\n", bo);
		goto allocate_mem_kmap_bo_failed;
	}

	r = amdgpu_bo_kmap(bo, &cpu_ptr_tmp);
	if (r) {
		dev_err(adev->dev,
			"(%d) failed to map bo to kernel for amdkfd\n", r);
		goto allocate_mem_kmap_bo_failed;
	}

	*mem_obj = bo;
	*gpu_addr = amdgpu_bo_gpu_offset(bo);
	*cpu_ptr = cpu_ptr_tmp;

	amdgpu_bo_unreserve(bo);

	return 0;

allocate_mem_kmap_bo_failed:
	amdgpu_bo_unpin(bo);
allocate_mem_pin_bo_failed:
	amdgpu_bo_unreserve(bo);
allocate_mem_reserve_bo_failed:
	amdgpu_bo_unref(&bo);

	return r;
}

<<<<<<< HEAD
void free_gtt_mem(struct kgd_dev *kgd, void *mem_obj)
=======
void amdgpu_amdkfd_free_gtt_mem(struct kgd_dev *kgd, void *mem_obj)
>>>>>>> upstream/android-13
{
	struct amdgpu_bo *bo = (struct amdgpu_bo *) mem_obj;

	amdgpu_bo_reserve(bo, true);
	amdgpu_bo_kunmap(bo);
	amdgpu_bo_unpin(bo);
	amdgpu_bo_unreserve(bo);
	amdgpu_bo_unref(&(bo));
}

<<<<<<< HEAD
void get_local_mem_info(struct kgd_dev *kgd,
			struct kfd_local_mem_info *mem_info)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;
	uint64_t address_mask = adev->dev->dma_mask ? ~*adev->dev->dma_mask :
					     ~((1ULL << 32) - 1);
	resource_size_t aper_limit = adev->gmc.aper_base + adev->gmc.aper_size;

	memset(mem_info, 0, sizeof(*mem_info));
	if (!(adev->gmc.aper_base & address_mask || aper_limit & address_mask)) {
		mem_info->local_mem_size_public = adev->gmc.visible_vram_size;
		mem_info->local_mem_size_private = adev->gmc.real_vram_size -
				adev->gmc.visible_vram_size;
	} else {
		mem_info->local_mem_size_public = 0;
		mem_info->local_mem_size_private = adev->gmc.real_vram_size;
	}
	mem_info->vram_width = adev->gmc.vram_width;

	pr_debug("Address base: %pap limit %pap public 0x%llx private 0x%llx\n",
			&adev->gmc.aper_base, &aper_limit,
=======
int amdgpu_amdkfd_alloc_gws(struct kgd_dev *kgd, size_t size,
				void **mem_obj)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;
	struct amdgpu_bo *bo = NULL;
	struct amdgpu_bo_user *ubo;
	struct amdgpu_bo_param bp;
	int r;

	memset(&bp, 0, sizeof(bp));
	bp.size = size;
	bp.byte_align = 1;
	bp.domain = AMDGPU_GEM_DOMAIN_GWS;
	bp.flags = AMDGPU_GEM_CREATE_NO_CPU_ACCESS;
	bp.type = ttm_bo_type_device;
	bp.resv = NULL;
	bp.bo_ptr_size = sizeof(struct amdgpu_bo);

	r = amdgpu_bo_create_user(adev, &bp, &ubo);
	if (r) {
		dev_err(adev->dev,
			"failed to allocate gws BO for amdkfd (%d)\n", r);
		return r;
	}

	bo = &ubo->bo;
	*mem_obj = bo;
	return 0;
}

void amdgpu_amdkfd_free_gws(struct kgd_dev *kgd, void *mem_obj)
{
	struct amdgpu_bo *bo = (struct amdgpu_bo *)mem_obj;

	amdgpu_bo_unref(&bo);
}

uint32_t amdgpu_amdkfd_get_fw_version(struct kgd_dev *kgd,
				      enum kgd_engine_type type)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	switch (type) {
	case KGD_ENGINE_PFP:
		return adev->gfx.pfp_fw_version;

	case KGD_ENGINE_ME:
		return adev->gfx.me_fw_version;

	case KGD_ENGINE_CE:
		return adev->gfx.ce_fw_version;

	case KGD_ENGINE_MEC1:
		return adev->gfx.mec_fw_version;

	case KGD_ENGINE_MEC2:
		return adev->gfx.mec2_fw_version;

	case KGD_ENGINE_RLC:
		return adev->gfx.rlc_fw_version;

	case KGD_ENGINE_SDMA1:
		return adev->sdma.instance[0].fw_version;

	case KGD_ENGINE_SDMA2:
		return adev->sdma.instance[1].fw_version;

	default:
		return 0;
	}

	return 0;
}

void amdgpu_amdkfd_get_local_mem_info(struct kgd_dev *kgd,
				      struct kfd_local_mem_info *mem_info)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	memset(mem_info, 0, sizeof(*mem_info));

	mem_info->local_mem_size_public = adev->gmc.visible_vram_size;
	mem_info->local_mem_size_private = adev->gmc.real_vram_size -
						adev->gmc.visible_vram_size;

	mem_info->vram_width = adev->gmc.vram_width;

	pr_debug("Address base: %pap public 0x%llx private 0x%llx\n",
			&adev->gmc.aper_base,
>>>>>>> upstream/android-13
			mem_info->local_mem_size_public,
			mem_info->local_mem_size_private);

	if (amdgpu_sriov_vf(adev))
		mem_info->mem_clk_max = adev->clock.default_mclk / 100;
<<<<<<< HEAD
	else if (adev->powerplay.pp_funcs)
		mem_info->mem_clk_max = amdgpu_dpm_get_mclk(adev, false) / 100;
	else
		mem_info->mem_clk_max = 100;
}

uint64_t get_gpu_clock_counter(struct kgd_dev *kgd)
=======
	else if (adev->pm.dpm_enabled) {
		if (amdgpu_emu_mode == 1)
			mem_info->mem_clk_max = 0;
		else
			mem_info->mem_clk_max = amdgpu_dpm_get_mclk(adev, false) / 100;
	} else
		mem_info->mem_clk_max = 100;
}

uint64_t amdgpu_amdkfd_get_gpu_clock_counter(struct kgd_dev *kgd)
>>>>>>> upstream/android-13
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	if (adev->gfx.funcs->get_gpu_clock_counter)
		return adev->gfx.funcs->get_gpu_clock_counter(adev);
	return 0;
}

<<<<<<< HEAD
uint32_t get_max_engine_clock_in_mhz(struct kgd_dev *kgd)
=======
uint32_t amdgpu_amdkfd_get_max_engine_clock_in_mhz(struct kgd_dev *kgd)
>>>>>>> upstream/android-13
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	/* the sclk is in quantas of 10kHz */
	if (amdgpu_sriov_vf(adev))
		return adev->clock.default_sclk / 100;
<<<<<<< HEAD
	else if (adev->powerplay.pp_funcs)
=======
	else if (adev->pm.dpm_enabled)
>>>>>>> upstream/android-13
		return amdgpu_dpm_get_sclk(adev, false) / 100;
	else
		return 100;
}

<<<<<<< HEAD
void get_cu_info(struct kgd_dev *kgd, struct kfd_cu_info *cu_info)
=======
void amdgpu_amdkfd_get_cu_info(struct kgd_dev *kgd, struct kfd_cu_info *cu_info)
>>>>>>> upstream/android-13
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;
	struct amdgpu_cu_info acu_info = adev->gfx.cu_info;

	memset(cu_info, 0, sizeof(*cu_info));
	if (sizeof(cu_info->cu_bitmap) != sizeof(acu_info.bitmap))
		return;

	cu_info->cu_active_number = acu_info.number;
	cu_info->cu_ao_mask = acu_info.ao_cu_mask;
	memcpy(&cu_info->cu_bitmap[0], &acu_info.bitmap[0],
	       sizeof(acu_info.bitmap));
	cu_info->num_shader_engines = adev->gfx.config.max_shader_engines;
	cu_info->num_shader_arrays_per_engine = adev->gfx.config.max_sh_per_se;
	cu_info->num_cu_per_sh = adev->gfx.config.max_cu_per_sh;
	cu_info->simd_per_cu = acu_info.simd_per_cu;
	cu_info->max_waves_per_simd = acu_info.max_waves_per_simd;
	cu_info->wave_front_size = acu_info.wave_front_size;
	cu_info->max_scratch_slots_per_cu = acu_info.max_scratch_slots_per_cu;
	cu_info->lds_size = acu_info.lds_size;
}

<<<<<<< HEAD
uint64_t amdgpu_amdkfd_get_vram_usage(struct kgd_dev *kgd)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	return amdgpu_vram_mgr_usage(&adev->mman.bdev.man[TTM_PL_VRAM]);
=======
int amdgpu_amdkfd_get_dmabuf_info(struct kgd_dev *kgd, int dma_buf_fd,
				  struct kgd_dev **dma_buf_kgd,
				  uint64_t *bo_size, void *metadata_buffer,
				  size_t buffer_size, uint32_t *metadata_size,
				  uint32_t *flags)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;
	struct dma_buf *dma_buf;
	struct drm_gem_object *obj;
	struct amdgpu_bo *bo;
	uint64_t metadata_flags;
	int r = -EINVAL;

	dma_buf = dma_buf_get(dma_buf_fd);
	if (IS_ERR(dma_buf))
		return PTR_ERR(dma_buf);

	if (dma_buf->ops != &amdgpu_dmabuf_ops)
		/* Can't handle non-graphics buffers */
		goto out_put;

	obj = dma_buf->priv;
	if (obj->dev->driver != adev_to_drm(adev)->driver)
		/* Can't handle buffers from different drivers */
		goto out_put;

	adev = drm_to_adev(obj->dev);
	bo = gem_to_amdgpu_bo(obj);
	if (!(bo->preferred_domains & (AMDGPU_GEM_DOMAIN_VRAM |
				    AMDGPU_GEM_DOMAIN_GTT)))
		/* Only VRAM and GTT BOs are supported */
		goto out_put;

	r = 0;
	if (dma_buf_kgd)
		*dma_buf_kgd = (struct kgd_dev *)adev;
	if (bo_size)
		*bo_size = amdgpu_bo_size(bo);
	if (metadata_buffer)
		r = amdgpu_bo_get_metadata(bo, metadata_buffer, buffer_size,
					   metadata_size, &metadata_flags);
	if (flags) {
		*flags = (bo->preferred_domains & AMDGPU_GEM_DOMAIN_VRAM) ?
				KFD_IOC_ALLOC_MEM_FLAGS_VRAM
				: KFD_IOC_ALLOC_MEM_FLAGS_GTT;

		if (bo->flags & AMDGPU_GEM_CREATE_CPU_ACCESS_REQUIRED)
			*flags |= KFD_IOC_ALLOC_MEM_FLAGS_PUBLIC;
	}

out_put:
	dma_buf_put(dma_buf);
	return r;
}

uint64_t amdgpu_amdkfd_get_vram_usage(struct kgd_dev *kgd)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;
	struct ttm_resource_manager *vram_man = ttm_manager_type(&adev->mman.bdev, TTM_PL_VRAM);

	return amdgpu_vram_mgr_usage(vram_man);
}

uint64_t amdgpu_amdkfd_get_hive_id(struct kgd_dev *kgd)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	return adev->gmc.xgmi.hive_id;
}

uint64_t amdgpu_amdkfd_get_unique_id(struct kgd_dev *kgd)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	return adev->unique_id;
}

uint8_t amdgpu_amdkfd_get_xgmi_hops_count(struct kgd_dev *dst, struct kgd_dev *src)
{
	struct amdgpu_device *peer_adev = (struct amdgpu_device *)src;
	struct amdgpu_device *adev = (struct amdgpu_device *)dst;
	int ret = amdgpu_xgmi_get_hops_count(adev, peer_adev);

	if (ret < 0) {
		DRM_ERROR("amdgpu: failed to get  xgmi hops count between node %d and %d. ret = %d\n",
			adev->gmc.xgmi.physical_node_id,
			peer_adev->gmc.xgmi.physical_node_id, ret);
		ret = 0;
	}
	return  (uint8_t)ret;
}

int amdgpu_amdkfd_get_xgmi_bandwidth_mbytes(struct kgd_dev *dst, struct kgd_dev *src, bool is_min)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)dst, *peer_adev;
	int num_links;

	if (adev->asic_type != CHIP_ALDEBARAN)
		return 0;

	if (src)
		peer_adev = (struct amdgpu_device *)src;

	/* num links returns 0 for indirect peers since indirect route is unknown. */
	num_links = is_min ? 1 : amdgpu_xgmi_get_num_links(adev, peer_adev);
	if (num_links < 0) {
		DRM_ERROR("amdgpu: failed to get xgmi num links between node %d and %d. ret = %d\n",
			adev->gmc.xgmi.physical_node_id,
			peer_adev->gmc.xgmi.physical_node_id, num_links);
		num_links = 0;
	}

	/* Aldebaran xGMI DPM is defeatured so assume x16 x 25Gbps for bandwidth. */
	return (num_links * 16 * 25000)/BITS_PER_BYTE;
}

int amdgpu_amdkfd_get_pcie_bandwidth_mbytes(struct kgd_dev *dev, bool is_min)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)dev;
	int num_lanes_shift = (is_min ? ffs(adev->pm.pcie_mlw_mask) :
							fls(adev->pm.pcie_mlw_mask)) - 1;
	int gen_speed_shift = (is_min ? ffs(adev->pm.pcie_gen_mask &
						CAIL_PCIE_LINK_SPEED_SUPPORT_MASK) :
					fls(adev->pm.pcie_gen_mask &
						CAIL_PCIE_LINK_SPEED_SUPPORT_MASK)) - 1;
	uint32_t num_lanes_mask = 1 << num_lanes_shift;
	uint32_t gen_speed_mask = 1 << gen_speed_shift;
	int num_lanes_factor = 0, gen_speed_mbits_factor = 0;

	switch (num_lanes_mask) {
	case CAIL_PCIE_LINK_WIDTH_SUPPORT_X1:
		num_lanes_factor = 1;
		break;
	case CAIL_PCIE_LINK_WIDTH_SUPPORT_X2:
		num_lanes_factor = 2;
		break;
	case CAIL_PCIE_LINK_WIDTH_SUPPORT_X4:
		num_lanes_factor = 4;
		break;
	case CAIL_PCIE_LINK_WIDTH_SUPPORT_X8:
		num_lanes_factor = 8;
		break;
	case CAIL_PCIE_LINK_WIDTH_SUPPORT_X12:
		num_lanes_factor = 12;
		break;
	case CAIL_PCIE_LINK_WIDTH_SUPPORT_X16:
		num_lanes_factor = 16;
		break;
	case CAIL_PCIE_LINK_WIDTH_SUPPORT_X32:
		num_lanes_factor = 32;
		break;
	}

	switch (gen_speed_mask) {
	case CAIL_PCIE_LINK_SPEED_SUPPORT_GEN1:
		gen_speed_mbits_factor = 2500;
		break;
	case CAIL_PCIE_LINK_SPEED_SUPPORT_GEN2:
		gen_speed_mbits_factor = 5000;
		break;
	case CAIL_PCIE_LINK_SPEED_SUPPORT_GEN3:
		gen_speed_mbits_factor = 8000;
		break;
	case CAIL_PCIE_LINK_SPEED_SUPPORT_GEN4:
		gen_speed_mbits_factor = 16000;
		break;
	case CAIL_PCIE_LINK_SPEED_SUPPORT_GEN5:
		gen_speed_mbits_factor = 32000;
		break;
	}

	return (num_lanes_factor * gen_speed_mbits_factor)/BITS_PER_BYTE;
}

uint64_t amdgpu_amdkfd_get_mmio_remap_phys_addr(struct kgd_dev *kgd)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	return adev->rmmio_remap.bus_addr;
}

uint32_t amdgpu_amdkfd_get_num_gws(struct kgd_dev *kgd)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	return adev->gds.gws_size;
}

uint32_t amdgpu_amdkfd_get_asic_rev_id(struct kgd_dev *kgd)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	return adev->rev_id;
}

int amdgpu_amdkfd_get_noretry(struct kgd_dev *kgd)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	return adev->gmc.noretry;
>>>>>>> upstream/android-13
}

int amdgpu_amdkfd_submit_ib(struct kgd_dev *kgd, enum kgd_engine_type engine,
				uint32_t vmid, uint64_t gpu_addr,
				uint32_t *ib_cmd, uint32_t ib_len)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;
	struct amdgpu_job *job;
	struct amdgpu_ib *ib;
	struct amdgpu_ring *ring;
	struct dma_fence *f = NULL;
	int ret;

	switch (engine) {
	case KGD_ENGINE_MEC1:
		ring = &adev->gfx.compute_ring[0];
		break;
	case KGD_ENGINE_SDMA1:
		ring = &adev->sdma.instance[0].ring;
		break;
	case KGD_ENGINE_SDMA2:
		ring = &adev->sdma.instance[1].ring;
		break;
	default:
		pr_err("Invalid engine in IB submission: %d\n", engine);
		ret = -EINVAL;
		goto err;
	}

	ret = amdgpu_job_alloc(adev, 1, &job, NULL);
	if (ret)
		goto err;

	ib = &job->ibs[0];
	memset(ib, 0, sizeof(struct amdgpu_ib));

	ib->gpu_addr = gpu_addr;
	ib->ptr = ib_cmd;
	ib->length_dw = ib_len;
	/* This works for NO_HWS. TODO: need to handle without knowing VMID */
	job->vmid = vmid;

	ret = amdgpu_ib_schedule(ring, 1, ib, job, &f);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	if (ret) {
		DRM_ERROR("amdgpu: failed to schedule IB.\n");
		goto err_ib_sched;
	}

	ret = dma_fence_wait(f, false);

err_ib_sched:
<<<<<<< HEAD
	dma_fence_put(f);
=======
>>>>>>> upstream/android-13
	amdgpu_job_free(job);
err:
	return ret;
}

void amdgpu_amdkfd_set_compute_idle(struct kgd_dev *kgd, bool idle)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

<<<<<<< HEAD
	if (adev->powerplay.pp_funcs &&
	    adev->powerplay.pp_funcs->switch_power_profile)
		amdgpu_dpm_switch_power_profile(adev,
						PP_SMC_POWER_PROFILE_COMPUTE,
						!idle);
=======
	amdgpu_dpm_switch_power_profile(adev,
					PP_SMC_POWER_PROFILE_COMPUTE,
					!idle);
>>>>>>> upstream/android-13
}

bool amdgpu_amdkfd_is_kfd_vmid(struct amdgpu_device *adev, u32 vmid)
{
<<<<<<< HEAD
	if (adev->kfd) {
		if ((1 << vmid) & compute_vmid_bitmap)
			return true;
	}

	return false;
}

#if !defined(CONFIG_HSA_AMD_MODULE) && !defined(CONFIG_HSA_AMD)
bool amdkfd_fence_check_mm(struct dma_fence *f, struct mm_struct *mm)
{
	return false;
}

void amdgpu_amdkfd_unreserve_system_memory_limit(struct amdgpu_bo *bo)
{
}

void amdgpu_amdkfd_gpuvm_destroy_cb(struct amdgpu_device *adev,
					struct amdgpu_vm *vm)
{
}

struct amdgpu_amdkfd_fence *to_amdgpu_amdkfd_fence(struct dma_fence *f)
{
	return NULL;
}

int amdgpu_amdkfd_evict_userptr(struct kgd_mem *mem, struct mm_struct *mm)
{
	return 0;
}

struct kfd2kgd_calls *amdgpu_amdkfd_gfx_7_get_functions(void)
{
	return NULL;
}

struct kfd2kgd_calls *amdgpu_amdkfd_gfx_8_0_get_functions(void)
{
	return NULL;
}

struct kfd2kgd_calls *amdgpu_amdkfd_gfx_9_0_get_functions(void)
{
	return NULL;
}
#endif
=======
	if (adev->kfd.dev)
		return vmid >= adev->vm_manager.first_kfd_vmid;

	return false;
}

int amdgpu_amdkfd_flush_gpu_tlb_vmid(struct kgd_dev *kgd, uint16_t vmid)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	if (adev->family == AMDGPU_FAMILY_AI) {
		int i;

		for (i = 0; i < adev->num_vmhubs; i++)
			amdgpu_gmc_flush_gpu_tlb(adev, vmid, i, 0);
	} else {
		amdgpu_gmc_flush_gpu_tlb(adev, vmid, AMDGPU_GFXHUB_0, 0);
	}

	return 0;
}

int amdgpu_amdkfd_flush_gpu_tlb_pasid(struct kgd_dev *kgd, uint16_t pasid,
				      enum TLB_FLUSH_TYPE flush_type)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;
	bool all_hub = false;

	if (adev->family == AMDGPU_FAMILY_AI)
		all_hub = true;

	return amdgpu_gmc_flush_gpu_tlb_pasid(adev, pasid, flush_type, all_hub);
}

bool amdgpu_amdkfd_have_atomics_support(struct kgd_dev *kgd)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	return adev->have_atomics_support;
}
>>>>>>> upstream/android-13
