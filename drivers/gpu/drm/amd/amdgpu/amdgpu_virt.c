/*
 * Copyright 2016 Advanced Micro Devices, Inc.
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

<<<<<<< HEAD
#include "amdgpu.h"
#define MAX_KIQ_REG_WAIT	5000 /* in usecs, 5ms */
#define MAX_KIQ_REG_BAILOUT_INTERVAL	5 /* in msecs, 5ms */
#define MAX_KIQ_REG_TRY 20

uint64_t amdgpu_csa_vaddr(struct amdgpu_device *adev)
{
	uint64_t addr = adev->vm_manager.max_pfn << AMDGPU_GPU_PAGE_SHIFT;

	addr -= AMDGPU_VA_RESERVED_SIZE;

	if (addr >= AMDGPU_VA_HOLE_START)
		addr |= AMDGPU_VA_HOLE_END;

	return addr;
}
=======
#include <linux/module.h>

#include <drm/drm_drv.h>
#include <xen/xen.h>

#include "amdgpu.h"
#include "amdgpu_ras.h"
#include "vi.h"
#include "soc15.h"
#include "nv.h"

#define POPULATE_UCODE_INFO(vf2pf_info, ucode, ver) \
	do { \
		vf2pf_info->ucode_info[ucode].id = ucode; \
		vf2pf_info->ucode_info[ucode].version = ver; \
	} while (0)
>>>>>>> upstream/android-13

bool amdgpu_virt_mmio_blocked(struct amdgpu_device *adev)
{
	/* By now all MMIO pages except mailbox are blocked */
	/* if blocking is enabled in hypervisor. Choose the */
	/* SCRATCH_REG0 to test. */
	return RREG32_NO_KIQ(0xc040) == 0xffffffff;
}

<<<<<<< HEAD
int amdgpu_allocate_static_csa(struct amdgpu_device *adev)
{
	int r;
	void *ptr;

	r = amdgpu_bo_create_kernel(adev, AMDGPU_CSA_SIZE, PAGE_SIZE,
				AMDGPU_GEM_DOMAIN_VRAM, &adev->virt.csa_obj,
				&adev->virt.csa_vmid0_addr, &ptr);
	if (r)
		return r;

	memset(ptr, 0, AMDGPU_CSA_SIZE);
	return 0;
}

void amdgpu_free_static_csa(struct amdgpu_device *adev) {
	amdgpu_bo_free_kernel(&adev->virt.csa_obj,
						&adev->virt.csa_vmid0_addr,
						NULL);
}

/*
 * amdgpu_map_static_csa should be called during amdgpu_vm_init
 * it maps virtual address amdgpu_csa_vaddr() to this VM, and each command
 * submission of GFX should use this virtual address within META_DATA init
 * package to support SRIOV gfx preemption.
 */
int amdgpu_map_static_csa(struct amdgpu_device *adev, struct amdgpu_vm *vm,
			  struct amdgpu_bo_va **bo_va)
{
	uint64_t csa_addr = amdgpu_csa_vaddr(adev) & AMDGPU_VA_HOLE_MASK;
	struct ww_acquire_ctx ticket;
	struct list_head list;
	struct amdgpu_bo_list_entry pd;
	struct ttm_validate_buffer csa_tv;
	int r;

	INIT_LIST_HEAD(&list);
	INIT_LIST_HEAD(&csa_tv.head);
	csa_tv.bo = &adev->virt.csa_obj->tbo;
	csa_tv.shared = true;

	list_add(&csa_tv.head, &list);
	amdgpu_vm_get_pd_bo(vm, &list, &pd);

	r = ttm_eu_reserve_buffers(&ticket, &list, true, NULL);
	if (r) {
		DRM_ERROR("failed to reserve CSA,PD BOs: err=%d\n", r);
		return r;
	}

	*bo_va = amdgpu_vm_bo_add(adev, vm, adev->virt.csa_obj);
	if (!*bo_va) {
		ttm_eu_backoff_reservation(&ticket, &list);
		DRM_ERROR("failed to create bo_va for static CSA\n");
		return -ENOMEM;
	}

	r = amdgpu_vm_alloc_pts(adev, (*bo_va)->base.vm, csa_addr,
				AMDGPU_CSA_SIZE);
	if (r) {
		DRM_ERROR("failed to allocate pts for static CSA, err=%d\n", r);
		amdgpu_vm_bo_rmv(adev, *bo_va);
		ttm_eu_backoff_reservation(&ticket, &list);
		return r;
	}

	r = amdgpu_vm_bo_map(adev, *bo_va, csa_addr, 0, AMDGPU_CSA_SIZE,
			     AMDGPU_PTE_READABLE | AMDGPU_PTE_WRITEABLE |
			     AMDGPU_PTE_EXECUTABLE);

	if (r) {
		DRM_ERROR("failed to do bo_map on static CSA, err=%d\n", r);
		amdgpu_vm_bo_rmv(adev, *bo_va);
		ttm_eu_backoff_reservation(&ticket, &list);
		return r;
	}

	ttm_eu_backoff_reservation(&ticket, &list);
	return 0;
}

void amdgpu_virt_init_setting(struct amdgpu_device *adev)
{
	/* enable virtual display */
	adev->mode_info.num_crtc = 1;
	adev->enable_virtual_display = true;
=======
void amdgpu_virt_init_setting(struct amdgpu_device *adev)
{
	struct drm_device *ddev = adev_to_drm(adev);

	/* enable virtual display */
	if (adev->asic_type != CHIP_ALDEBARAN &&
	    adev->asic_type != CHIP_ARCTURUS) {
		if (adev->mode_info.num_crtc == 0)
			adev->mode_info.num_crtc = 1;
		adev->enable_virtual_display = true;
	}
	ddev->driver_features &= ~DRIVER_ATOMIC;
>>>>>>> upstream/android-13
	adev->cg_flags = 0;
	adev->pg_flags = 0;
}

<<<<<<< HEAD
uint32_t amdgpu_virt_kiq_rreg(struct amdgpu_device *adev, uint32_t reg)
{
	signed long r, cnt = 0;
	unsigned long flags;
	uint32_t seq;
	struct amdgpu_kiq *kiq = &adev->gfx.kiq;
	struct amdgpu_ring *ring = &kiq->ring;

	BUG_ON(!ring->funcs->emit_rreg);

	spin_lock_irqsave(&kiq->ring_lock, flags);
	amdgpu_ring_alloc(ring, 32);
	amdgpu_ring_emit_rreg(ring, reg);
	amdgpu_fence_emit_polling(ring, &seq);
=======
void amdgpu_virt_kiq_reg_write_reg_wait(struct amdgpu_device *adev,
					uint32_t reg0, uint32_t reg1,
					uint32_t ref, uint32_t mask)
{
	struct amdgpu_kiq *kiq = &adev->gfx.kiq;
	struct amdgpu_ring *ring = &kiq->ring;
	signed long r, cnt = 0;
	unsigned long flags;
	uint32_t seq;

	spin_lock_irqsave(&kiq->ring_lock, flags);
	amdgpu_ring_alloc(ring, 32);
	amdgpu_ring_emit_reg_write_reg_wait(ring, reg0, reg1,
					    ref, mask);
	r = amdgpu_fence_emit_polling(ring, &seq, MAX_KIQ_REG_WAIT);
	if (r)
		goto failed_undo;

>>>>>>> upstream/android-13
	amdgpu_ring_commit(ring);
	spin_unlock_irqrestore(&kiq->ring_lock, flags);

	r = amdgpu_fence_wait_polling(ring, seq, MAX_KIQ_REG_WAIT);

<<<<<<< HEAD
	/* don't wait anymore for gpu reset case because this way may
	 * block gpu_recover() routine forever, e.g. this virt_kiq_rreg
	 * is triggered in TTM and ttm_bo_lock_delayed_workqueue() will
	 * never return if we keep waiting in virt_kiq_rreg, which cause
	 * gpu_recover() hang there.
	 *
	 * also don't wait anymore for IRQ context
	 * */
	if (r < 1 && (adev->in_gpu_reset || in_interrupt()))
		goto failed_kiq_read;

	if (in_interrupt())
		might_sleep();

	while (r < 1 && cnt++ < MAX_KIQ_REG_TRY) {
		msleep(MAX_KIQ_REG_BAILOUT_INTERVAL);
		r = amdgpu_fence_wait_polling(ring, seq, MAX_KIQ_REG_WAIT);
	}

	if (cnt > MAX_KIQ_REG_TRY)
		goto failed_kiq_read;

	return adev->wb.wb[adev->virt.reg_val_offs];

failed_kiq_read:
	pr_err("failed to read reg:%x\n", reg);
	return ~0;
}

void amdgpu_virt_kiq_wreg(struct amdgpu_device *adev, uint32_t reg, uint32_t v)
{
	signed long r, cnt = 0;
	unsigned long flags;
	uint32_t seq;
	struct amdgpu_kiq *kiq = &adev->gfx.kiq;
	struct amdgpu_ring *ring = &kiq->ring;

	BUG_ON(!ring->funcs->emit_wreg);

	spin_lock_irqsave(&kiq->ring_lock, flags);
	amdgpu_ring_alloc(ring, 32);
	amdgpu_ring_emit_wreg(ring, reg, v);
	amdgpu_fence_emit_polling(ring, &seq);
	amdgpu_ring_commit(ring);
	spin_unlock_irqrestore(&kiq->ring_lock, flags);

	r = amdgpu_fence_wait_polling(ring, seq, MAX_KIQ_REG_WAIT);

	/* don't wait anymore for gpu reset case because this way may
	 * block gpu_recover() routine forever, e.g. this virt_kiq_rreg
	 * is triggered in TTM and ttm_bo_lock_delayed_workqueue() will
	 * never return if we keep waiting in virt_kiq_rreg, which cause
	 * gpu_recover() hang there.
	 *
	 * also don't wait anymore for IRQ context
	 * */
	if (r < 1 && (adev->in_gpu_reset || in_interrupt()))
		goto failed_kiq_write;

	if (in_interrupt())
		might_sleep();

=======
	/* don't wait anymore for IRQ context */
	if (r < 1 && in_interrupt())
		goto failed_kiq;

	might_sleep();
>>>>>>> upstream/android-13
	while (r < 1 && cnt++ < MAX_KIQ_REG_TRY) {

		msleep(MAX_KIQ_REG_BAILOUT_INTERVAL);
		r = amdgpu_fence_wait_polling(ring, seq, MAX_KIQ_REG_WAIT);
	}

	if (cnt > MAX_KIQ_REG_TRY)
<<<<<<< HEAD
		goto failed_kiq_write;

	return;

failed_kiq_write:
	pr_err("failed to write reg:%x\n", reg);
=======
		goto failed_kiq;

	return;

failed_undo:
	amdgpu_ring_undo(ring);
	spin_unlock_irqrestore(&kiq->ring_lock, flags);
failed_kiq:
	dev_err(adev->dev, "failed to write reg %x wait reg %x\n", reg0, reg1);
>>>>>>> upstream/android-13
}

/**
 * amdgpu_virt_request_full_gpu() - request full gpu access
<<<<<<< HEAD
 * @amdgpu:	amdgpu device.
=======
 * @adev:	amdgpu device.
>>>>>>> upstream/android-13
 * @init:	is driver init time.
 * When start to init/fini driver, first need to request full gpu access.
 * Return: Zero if request success, otherwise will return error.
 */
int amdgpu_virt_request_full_gpu(struct amdgpu_device *adev, bool init)
{
	struct amdgpu_virt *virt = &adev->virt;
	int r;

	if (virt->ops && virt->ops->req_full_gpu) {
		r = virt->ops->req_full_gpu(adev, init);
		if (r)
			return r;

		adev->virt.caps &= ~AMDGPU_SRIOV_CAPS_RUNTIME;
	}

	return 0;
}

/**
 * amdgpu_virt_release_full_gpu() - release full gpu access
<<<<<<< HEAD
 * @amdgpu:	amdgpu device.
=======
 * @adev:	amdgpu device.
>>>>>>> upstream/android-13
 * @init:	is driver init time.
 * When finishing driver init/fini, need to release full gpu access.
 * Return: Zero if release success, otherwise will returen error.
 */
int amdgpu_virt_release_full_gpu(struct amdgpu_device *adev, bool init)
{
	struct amdgpu_virt *virt = &adev->virt;
	int r;

	if (virt->ops && virt->ops->rel_full_gpu) {
		r = virt->ops->rel_full_gpu(adev, init);
		if (r)
			return r;

		adev->virt.caps |= AMDGPU_SRIOV_CAPS_RUNTIME;
	}
	return 0;
}

/**
 * amdgpu_virt_reset_gpu() - reset gpu
<<<<<<< HEAD
 * @amdgpu:	amdgpu device.
=======
 * @adev:	amdgpu device.
>>>>>>> upstream/android-13
 * Send reset command to GPU hypervisor to reset GPU that VM is using
 * Return: Zero if reset success, otherwise will return error.
 */
int amdgpu_virt_reset_gpu(struct amdgpu_device *adev)
{
	struct amdgpu_virt *virt = &adev->virt;
	int r;

	if (virt->ops && virt->ops->reset_gpu) {
		r = virt->ops->reset_gpu(adev);
		if (r)
			return r;

		adev->virt.caps &= ~AMDGPU_SRIOV_CAPS_RUNTIME;
	}

	return 0;
}

<<<<<<< HEAD
/**
 * amdgpu_virt_wait_reset() - wait for reset gpu completed
 * @amdgpu:	amdgpu device.
=======
void amdgpu_virt_request_init_data(struct amdgpu_device *adev)
{
	struct amdgpu_virt *virt = &adev->virt;

	if (virt->ops && virt->ops->req_init_data)
		virt->ops->req_init_data(adev);

	if (adev->virt.req_init_data_ver > 0)
		DRM_INFO("host supports REQ_INIT_DATA handshake\n");
	else
		DRM_WARN("host doesn't support REQ_INIT_DATA handshake\n");
}

/**
 * amdgpu_virt_wait_reset() - wait for reset gpu completed
 * @adev:	amdgpu device.
>>>>>>> upstream/android-13
 * Wait for GPU reset completed.
 * Return: Zero if reset success, otherwise will return error.
 */
int amdgpu_virt_wait_reset(struct amdgpu_device *adev)
{
	struct amdgpu_virt *virt = &adev->virt;

	if (!virt->ops || !virt->ops->wait_reset)
		return -EINVAL;

	return virt->ops->wait_reset(adev);
}

/**
 * amdgpu_virt_alloc_mm_table() - alloc memory for mm table
<<<<<<< HEAD
 * @amdgpu:	amdgpu device.
=======
 * @adev:	amdgpu device.
>>>>>>> upstream/android-13
 * MM table is used by UVD and VCE for its initialization
 * Return: Zero if allocate success.
 */
int amdgpu_virt_alloc_mm_table(struct amdgpu_device *adev)
{
	int r;

	if (!amdgpu_sriov_vf(adev) || adev->virt.mm_table.gpu_addr)
		return 0;

	r = amdgpu_bo_create_kernel(adev, PAGE_SIZE, PAGE_SIZE,
				    AMDGPU_GEM_DOMAIN_VRAM,
				    &adev->virt.mm_table.bo,
				    &adev->virt.mm_table.gpu_addr,
				    (void *)&adev->virt.mm_table.cpu_addr);
	if (r) {
		DRM_ERROR("failed to alloc mm table and error = %d.\n", r);
		return r;
	}

	memset((void *)adev->virt.mm_table.cpu_addr, 0, PAGE_SIZE);
	DRM_INFO("MM table gpu addr = 0x%llx, cpu addr = %p.\n",
		 adev->virt.mm_table.gpu_addr,
		 adev->virt.mm_table.cpu_addr);
	return 0;
}

/**
 * amdgpu_virt_free_mm_table() - free mm table memory
<<<<<<< HEAD
 * @amdgpu:	amdgpu device.
=======
 * @adev:	amdgpu device.
>>>>>>> upstream/android-13
 * Free MM table memory
 */
void amdgpu_virt_free_mm_table(struct amdgpu_device *adev)
{
	if (!amdgpu_sriov_vf(adev) || !adev->virt.mm_table.gpu_addr)
		return;

	amdgpu_bo_free_kernel(&adev->virt.mm_table.bo,
			      &adev->virt.mm_table.gpu_addr,
			      (void *)&adev->virt.mm_table.cpu_addr);
	adev->virt.mm_table.gpu_addr = 0;
}


<<<<<<< HEAD
int amdgpu_virt_fw_reserve_get_checksum(void *obj,
					unsigned long obj_size,
					unsigned int key,
					unsigned int chksum)
=======
unsigned int amd_sriov_msg_checksum(void *obj,
				unsigned long obj_size,
				unsigned int key,
				unsigned int checksum)
>>>>>>> upstream/android-13
{
	unsigned int ret = key;
	unsigned long i = 0;
	unsigned char *pos;

	pos = (char *)obj;
	/* calculate checksum */
	for (i = 0; i < obj_size; ++i)
		ret += *(pos + i);
<<<<<<< HEAD
	/* minus the chksum itself */
	pos = (char *)&chksum;
	for (i = 0; i < sizeof(chksum); ++i)
=======
	/* minus the checksum itself */
	pos = (char *)&checksum;
	for (i = 0; i < sizeof(checksum); ++i)
>>>>>>> upstream/android-13
		ret -= *(pos + i);
	return ret;
}

<<<<<<< HEAD
void amdgpu_virt_init_data_exchange(struct amdgpu_device *adev)
{
	uint32_t pf2vf_size = 0;
	uint32_t checksum = 0;
	uint32_t checkval;
	char *str;

	adev->virt.fw_reserve.p_pf2vf = NULL;
	adev->virt.fw_reserve.p_vf2pf = NULL;

	if (adev->fw_vram_usage.va != NULL) {
		adev->virt.fw_reserve.p_pf2vf =
			(struct amdgim_pf2vf_info_header *)(
			adev->fw_vram_usage.va + AMDGIM_DATAEXCHANGE_OFFSET);
		AMDGPU_FW_VRAM_PF2VF_READ(adev, header.size, &pf2vf_size);
		AMDGPU_FW_VRAM_PF2VF_READ(adev, checksum, &checksum);
		AMDGPU_FW_VRAM_PF2VF_READ(adev, feature_flags, &adev->virt.gim_feature);

		/* pf2vf message must be in 4K */
		if (pf2vf_size > 0 && pf2vf_size < 4096) {
			checkval = amdgpu_virt_fw_reserve_get_checksum(
				adev->virt.fw_reserve.p_pf2vf, pf2vf_size,
				adev->virt.fw_reserve.checksum_key, checksum);
			if (checkval == checksum) {
				adev->virt.fw_reserve.p_vf2pf =
					((void *)adev->virt.fw_reserve.p_pf2vf +
					pf2vf_size);
				memset((void *)adev->virt.fw_reserve.p_vf2pf, 0,
					sizeof(amdgim_vf2pf_info));
				AMDGPU_FW_VRAM_VF2PF_WRITE(adev, header.version,
					AMDGPU_FW_VRAM_VF2PF_VER);
				AMDGPU_FW_VRAM_VF2PF_WRITE(adev, header.size,
					sizeof(amdgim_vf2pf_info));
				AMDGPU_FW_VRAM_VF2PF_READ(adev, driver_version,
					&str);
#ifdef MODULE
				if (THIS_MODULE->version != NULL)
					strcpy(str, THIS_MODULE->version);
				else
#endif
					strcpy(str, "N/A");
				AMDGPU_FW_VRAM_VF2PF_WRITE(adev, driver_cert,
					0);
				AMDGPU_FW_VRAM_VF2PF_WRITE(adev, checksum,
					amdgpu_virt_fw_reserve_get_checksum(
					adev->virt.fw_reserve.p_vf2pf,
					pf2vf_size,
					adev->virt.fw_reserve.checksum_key, 0));
			}
=======
static int amdgpu_virt_init_ras_err_handler_data(struct amdgpu_device *adev)
{
	struct amdgpu_virt *virt = &adev->virt;
	struct amdgpu_virt_ras_err_handler_data **data = &virt->virt_eh_data;
	/* GPU will be marked bad on host if bp count more then 10,
	 * so alloc 512 is enough.
	 */
	unsigned int align_space = 512;
	void *bps = NULL;
	struct amdgpu_bo **bps_bo = NULL;

	*data = kmalloc(sizeof(struct amdgpu_virt_ras_err_handler_data), GFP_KERNEL);
	if (!*data)
		return -ENOMEM;

	bps = kmalloc_array(align_space, sizeof((*data)->bps), GFP_KERNEL);
	bps_bo = kmalloc_array(align_space, sizeof((*data)->bps_bo), GFP_KERNEL);

	if (!bps || !bps_bo) {
		kfree(bps);
		kfree(bps_bo);
		kfree(*data);
		return -ENOMEM;
	}

	(*data)->bps = bps;
	(*data)->bps_bo = bps_bo;
	(*data)->count = 0;
	(*data)->last_reserved = 0;

	virt->ras_init_done = true;

	return 0;
}

static void amdgpu_virt_ras_release_bp(struct amdgpu_device *adev)
{
	struct amdgpu_virt *virt = &adev->virt;
	struct amdgpu_virt_ras_err_handler_data *data = virt->virt_eh_data;
	struct amdgpu_bo *bo;
	int i;

	if (!data)
		return;

	for (i = data->last_reserved - 1; i >= 0; i--) {
		bo = data->bps_bo[i];
		amdgpu_bo_free_kernel(&bo, NULL, NULL);
		data->bps_bo[i] = bo;
		data->last_reserved = i;
	}
}

void amdgpu_virt_release_ras_err_handler_data(struct amdgpu_device *adev)
{
	struct amdgpu_virt *virt = &adev->virt;
	struct amdgpu_virt_ras_err_handler_data *data = virt->virt_eh_data;

	virt->ras_init_done = false;

	if (!data)
		return;

	amdgpu_virt_ras_release_bp(adev);

	kfree(data->bps);
	kfree(data->bps_bo);
	kfree(data);
	virt->virt_eh_data = NULL;
}

static void amdgpu_virt_ras_add_bps(struct amdgpu_device *adev,
		struct eeprom_table_record *bps, int pages)
{
	struct amdgpu_virt *virt = &adev->virt;
	struct amdgpu_virt_ras_err_handler_data *data = virt->virt_eh_data;

	if (!data)
		return;

	memcpy(&data->bps[data->count], bps, pages * sizeof(*data->bps));
	data->count += pages;
}

static void amdgpu_virt_ras_reserve_bps(struct amdgpu_device *adev)
{
	struct amdgpu_virt *virt = &adev->virt;
	struct amdgpu_virt_ras_err_handler_data *data = virt->virt_eh_data;
	struct amdgpu_bo *bo = NULL;
	uint64_t bp;
	int i;

	if (!data)
		return;

	for (i = data->last_reserved; i < data->count; i++) {
		bp = data->bps[i].retired_page;

		/* There are two cases of reserve error should be ignored:
		 * 1) a ras bad page has been allocated (used by someone);
		 * 2) a ras bad page has been reserved (duplicate error injection
		 *    for one page);
		 */
		if (amdgpu_bo_create_kernel_at(adev, bp << AMDGPU_GPU_PAGE_SHIFT,
					       AMDGPU_GPU_PAGE_SIZE,
					       AMDGPU_GEM_DOMAIN_VRAM,
					       &bo, NULL))
			DRM_DEBUG("RAS WARN: reserve vram for retired page %llx fail\n", bp);

		data->bps_bo[i] = bo;
		data->last_reserved = i + 1;
		bo = NULL;
	}
}

static bool amdgpu_virt_ras_check_bad_page(struct amdgpu_device *adev,
		uint64_t retired_page)
{
	struct amdgpu_virt *virt = &adev->virt;
	struct amdgpu_virt_ras_err_handler_data *data = virt->virt_eh_data;
	int i;

	if (!data)
		return true;

	for (i = 0; i < data->count; i++)
		if (retired_page == data->bps[i].retired_page)
			return true;

	return false;
}

static void amdgpu_virt_add_bad_page(struct amdgpu_device *adev,
		uint64_t bp_block_offset, uint32_t bp_block_size)
{
	struct eeprom_table_record bp;
	uint64_t retired_page;
	uint32_t bp_idx, bp_cnt;

	if (bp_block_size) {
		bp_cnt = bp_block_size / sizeof(uint64_t);
		for (bp_idx = 0; bp_idx < bp_cnt; bp_idx++) {
			retired_page = *(uint64_t *)(adev->mman.fw_vram_usage_va +
					bp_block_offset + bp_idx * sizeof(uint64_t));
			bp.retired_page = retired_page;

			if (amdgpu_virt_ras_check_bad_page(adev, retired_page))
				continue;

			amdgpu_virt_ras_add_bps(adev, &bp, 1);

			amdgpu_virt_ras_reserve_bps(adev);
>>>>>>> upstream/android-13
		}
	}
}

<<<<<<< HEAD

=======
static int amdgpu_virt_read_pf2vf_data(struct amdgpu_device *adev)
{
	struct amd_sriov_msg_pf2vf_info_header *pf2vf_info = adev->virt.fw_reserve.p_pf2vf;
	uint32_t checksum;
	uint32_t checkval;

	uint32_t i;
	uint32_t tmp;

	if (adev->virt.fw_reserve.p_pf2vf == NULL)
		return -EINVAL;

	if (pf2vf_info->size > 1024) {
		DRM_ERROR("invalid pf2vf message size\n");
		return -EINVAL;
	}

	switch (pf2vf_info->version) {
	case 1:
		checksum = ((struct amdgim_pf2vf_info_v1 *)pf2vf_info)->checksum;
		checkval = amd_sriov_msg_checksum(
			adev->virt.fw_reserve.p_pf2vf, pf2vf_info->size,
			adev->virt.fw_reserve.checksum_key, checksum);
		if (checksum != checkval) {
			DRM_ERROR("invalid pf2vf message\n");
			return -EINVAL;
		}

		adev->virt.gim_feature =
			((struct amdgim_pf2vf_info_v1 *)pf2vf_info)->feature_flags;
		break;
	case 2:
		/* TODO: missing key, need to add it later */
		checksum = ((struct amd_sriov_msg_pf2vf_info *)pf2vf_info)->checksum;
		checkval = amd_sriov_msg_checksum(
			adev->virt.fw_reserve.p_pf2vf, pf2vf_info->size,
			0, checksum);
		if (checksum != checkval) {
			DRM_ERROR("invalid pf2vf message\n");
			return -EINVAL;
		}

		adev->virt.vf2pf_update_interval_ms =
			((struct amd_sriov_msg_pf2vf_info *)pf2vf_info)->vf2pf_update_interval_ms;
		adev->virt.gim_feature =
			((struct amd_sriov_msg_pf2vf_info *)pf2vf_info)->feature_flags.all;
		adev->virt.reg_access =
			((struct amd_sriov_msg_pf2vf_info *)pf2vf_info)->reg_access_flags.all;

		adev->virt.decode_max_dimension_pixels = 0;
		adev->virt.decode_max_frame_pixels = 0;
		adev->virt.encode_max_dimension_pixels = 0;
		adev->virt.encode_max_frame_pixels = 0;
		adev->virt.is_mm_bw_enabled = false;
		for (i = 0; i < AMD_SRIOV_MSG_RESERVE_VCN_INST; i++) {
			tmp = ((struct amd_sriov_msg_pf2vf_info *)pf2vf_info)->mm_bw_management[i].decode_max_dimension_pixels;
			adev->virt.decode_max_dimension_pixels = max(tmp, adev->virt.decode_max_dimension_pixels);

			tmp = ((struct amd_sriov_msg_pf2vf_info *)pf2vf_info)->mm_bw_management[i].decode_max_frame_pixels;
			adev->virt.decode_max_frame_pixels = max(tmp, adev->virt.decode_max_frame_pixels);

			tmp = ((struct amd_sriov_msg_pf2vf_info *)pf2vf_info)->mm_bw_management[i].encode_max_dimension_pixels;
			adev->virt.encode_max_dimension_pixels = max(tmp, adev->virt.encode_max_dimension_pixels);

			tmp = ((struct amd_sriov_msg_pf2vf_info *)pf2vf_info)->mm_bw_management[i].encode_max_frame_pixels;
			adev->virt.encode_max_frame_pixels = max(tmp, adev->virt.encode_max_frame_pixels);
		}
		if((adev->virt.decode_max_dimension_pixels > 0) || (adev->virt.encode_max_dimension_pixels > 0))
			adev->virt.is_mm_bw_enabled = true;

		adev->unique_id =
			((struct amd_sriov_msg_pf2vf_info *)pf2vf_info)->uuid;
		break;
	default:
		DRM_ERROR("invalid pf2vf version\n");
		return -EINVAL;
	}

	/* correct too large or too little interval value */
	if (adev->virt.vf2pf_update_interval_ms < 200 || adev->virt.vf2pf_update_interval_ms > 10000)
		adev->virt.vf2pf_update_interval_ms = 2000;

	return 0;
}

static void amdgpu_virt_populate_vf2pf_ucode_info(struct amdgpu_device *adev)
{
	struct amd_sriov_msg_vf2pf_info *vf2pf_info;
	vf2pf_info = (struct amd_sriov_msg_vf2pf_info *) adev->virt.fw_reserve.p_vf2pf;

	if (adev->virt.fw_reserve.p_vf2pf == NULL)
		return;

	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_VCE,      adev->vce.fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_UVD,      adev->uvd.fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_MC,       adev->gmc.fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_ME,       adev->gfx.me_fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_PFP,      adev->gfx.pfp_fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_CE,       adev->gfx.ce_fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_RLC,      adev->gfx.rlc_fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_RLC_SRLC, adev->gfx.rlc_srlc_fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_RLC_SRLG, adev->gfx.rlc_srlg_fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_RLC_SRLS, adev->gfx.rlc_srls_fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_MEC,      adev->gfx.mec_fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_MEC2,     adev->gfx.mec2_fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_SOS,      adev->psp.sos.fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_ASD,      adev->psp.asd.fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_TA_RAS,   adev->psp.ras.feature_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_TA_XGMI,  adev->psp.xgmi.feature_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_SMC,      adev->pm.fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_SDMA,     adev->sdma.instance[0].fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_SDMA2,    adev->sdma.instance[1].fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_VCN,      adev->vcn.fw_version);
	POPULATE_UCODE_INFO(vf2pf_info, AMD_SRIOV_UCODE_ID_DMCU,     adev->dm.dmcu_fw_version);
}

static int amdgpu_virt_write_vf2pf_data(struct amdgpu_device *adev)
{
	struct amd_sriov_msg_vf2pf_info *vf2pf_info;
	struct ttm_resource_manager *vram_man = ttm_manager_type(&adev->mman.bdev, TTM_PL_VRAM);

	vf2pf_info = (struct amd_sriov_msg_vf2pf_info *) adev->virt.fw_reserve.p_vf2pf;

	if (adev->virt.fw_reserve.p_vf2pf == NULL)
		return -EINVAL;

	memset(vf2pf_info, 0, sizeof(struct amd_sriov_msg_vf2pf_info));

	vf2pf_info->header.size = sizeof(struct amd_sriov_msg_vf2pf_info);
	vf2pf_info->header.version = AMD_SRIOV_MSG_FW_VRAM_VF2PF_VER;

#ifdef MODULE
	if (THIS_MODULE->version != NULL)
		strcpy(vf2pf_info->driver_version, THIS_MODULE->version);
	else
#endif
		strcpy(vf2pf_info->driver_version, "N/A");

	vf2pf_info->pf2vf_version_required = 0; // no requirement, guest understands all
	vf2pf_info->driver_cert = 0;
	vf2pf_info->os_info.all = 0;

	vf2pf_info->fb_usage = amdgpu_vram_mgr_usage(vram_man) >> 20;
	vf2pf_info->fb_vis_usage = amdgpu_vram_mgr_vis_usage(vram_man) >> 20;
	vf2pf_info->fb_size = adev->gmc.real_vram_size >> 20;
	vf2pf_info->fb_vis_size = adev->gmc.visible_vram_size >> 20;

	amdgpu_virt_populate_vf2pf_ucode_info(adev);

	/* TODO: read dynamic info */
	vf2pf_info->gfx_usage = 0;
	vf2pf_info->compute_usage = 0;
	vf2pf_info->encode_usage = 0;
	vf2pf_info->decode_usage = 0;

	vf2pf_info->checksum =
		amd_sriov_msg_checksum(
		vf2pf_info, vf2pf_info->header.size, 0, 0);

	return 0;
}

static void amdgpu_virt_update_vf2pf_work_item(struct work_struct *work)
{
	struct amdgpu_device *adev = container_of(work, struct amdgpu_device, virt.vf2pf_work.work);
	int ret;

	ret = amdgpu_virt_read_pf2vf_data(adev);
	if (ret)
		goto out;
	amdgpu_virt_write_vf2pf_data(adev);

out:
	schedule_delayed_work(&(adev->virt.vf2pf_work), adev->virt.vf2pf_update_interval_ms);
}

void amdgpu_virt_fini_data_exchange(struct amdgpu_device *adev)
{
	if (adev->virt.vf2pf_update_interval_ms != 0) {
		DRM_INFO("clean up the vf2pf work item\n");
		cancel_delayed_work_sync(&adev->virt.vf2pf_work);
		adev->virt.vf2pf_update_interval_ms = 0;
	}
}

void amdgpu_virt_init_data_exchange(struct amdgpu_device *adev)
{
	uint64_t bp_block_offset = 0;
	uint32_t bp_block_size = 0;
	struct amd_sriov_msg_pf2vf_info *pf2vf_v2 = NULL;

	adev->virt.fw_reserve.p_pf2vf = NULL;
	adev->virt.fw_reserve.p_vf2pf = NULL;
	adev->virt.vf2pf_update_interval_ms = 0;

	if (adev->mman.fw_vram_usage_va != NULL) {
		adev->virt.vf2pf_update_interval_ms = 2000;

		adev->virt.fw_reserve.p_pf2vf =
			(struct amd_sriov_msg_pf2vf_info_header *)
			(adev->mman.fw_vram_usage_va + (AMD_SRIOV_MSG_PF2VF_OFFSET_KB << 10));
		adev->virt.fw_reserve.p_vf2pf =
			(struct amd_sriov_msg_vf2pf_info_header *)
			(adev->mman.fw_vram_usage_va + (AMD_SRIOV_MSG_VF2PF_OFFSET_KB << 10));

		amdgpu_virt_read_pf2vf_data(adev);
		amdgpu_virt_write_vf2pf_data(adev);

		/* bad page handling for version 2 */
		if (adev->virt.fw_reserve.p_pf2vf->version == 2) {
				pf2vf_v2 = (struct amd_sriov_msg_pf2vf_info *)adev->virt.fw_reserve.p_pf2vf;

				bp_block_offset = ((uint64_t)pf2vf_v2->bp_block_offset_low & 0xFFFFFFFF) |
						((((uint64_t)pf2vf_v2->bp_block_offset_high) << 32) & 0xFFFFFFFF00000000);
				bp_block_size = pf2vf_v2->bp_block_size;

				if (bp_block_size && !adev->virt.ras_init_done)
					amdgpu_virt_init_ras_err_handler_data(adev);

				if (adev->virt.ras_init_done)
					amdgpu_virt_add_bad_page(adev, bp_block_offset, bp_block_size);
			}
	} else if (adev->bios != NULL) {
		adev->virt.fw_reserve.p_pf2vf =
			(struct amd_sriov_msg_pf2vf_info_header *)
			(adev->bios + (AMD_SRIOV_MSG_PF2VF_OFFSET_KB << 10));

		amdgpu_virt_read_pf2vf_data(adev);

		return;
	}

	if (adev->virt.vf2pf_update_interval_ms != 0) {
		INIT_DELAYED_WORK(&adev->virt.vf2pf_work, amdgpu_virt_update_vf2pf_work_item);
		schedule_delayed_work(&(adev->virt.vf2pf_work), adev->virt.vf2pf_update_interval_ms);
	}
}

void amdgpu_detect_virtualization(struct amdgpu_device *adev)
{
	uint32_t reg;

	switch (adev->asic_type) {
	case CHIP_TONGA:
	case CHIP_FIJI:
		reg = RREG32(mmBIF_IOV_FUNC_IDENTIFIER);
		break;
	case CHIP_VEGA10:
	case CHIP_VEGA20:
	case CHIP_NAVI10:
	case CHIP_NAVI12:
	case CHIP_SIENNA_CICHLID:
	case CHIP_ARCTURUS:
	case CHIP_ALDEBARAN:
		reg = RREG32(mmRCC_IOV_FUNC_IDENTIFIER);
		break;
	default: /* other chip doesn't support SRIOV */
		reg = 0;
		break;
	}

	if (reg & 1)
		adev->virt.caps |= AMDGPU_SRIOV_CAPS_IS_VF;

	if (reg & 0x80000000)
		adev->virt.caps |= AMDGPU_SRIOV_CAPS_ENABLE_IOV;

	if (!reg) {
		/* passthrough mode exclus sriov mod */
		if (is_virtual_machine() && !xen_initial_domain())
			adev->virt.caps |= AMDGPU_PASSTHROUGH_MODE;
	}

	/* we have the ability to check now */
	if (amdgpu_sriov_vf(adev)) {
		switch (adev->asic_type) {
		case CHIP_TONGA:
		case CHIP_FIJI:
			vi_set_virt_ops(adev);
			break;
		case CHIP_VEGA10:
		case CHIP_VEGA20:
		case CHIP_ARCTURUS:
		case CHIP_ALDEBARAN:
			soc15_set_virt_ops(adev);
			break;
		case CHIP_NAVI10:
		case CHIP_NAVI12:
		case CHIP_SIENNA_CICHLID:
			nv_set_virt_ops(adev);
			/* try send GPU_INIT_DATA request to host */
			amdgpu_virt_request_init_data(adev);
			break;
		default: /* other chip doesn't support SRIOV */
			DRM_ERROR("Unknown asic type: %d!\n", adev->asic_type);
			break;
		}
	}
}

static bool amdgpu_virt_access_debugfs_is_mmio(struct amdgpu_device *adev)
{
	return amdgpu_sriov_is_debug(adev) ? true : false;
}

static bool amdgpu_virt_access_debugfs_is_kiq(struct amdgpu_device *adev)
{
	return amdgpu_sriov_is_normal(adev) ? true : false;
}

int amdgpu_virt_enable_access_debugfs(struct amdgpu_device *adev)
{
	if (!amdgpu_sriov_vf(adev) ||
	    amdgpu_virt_access_debugfs_is_kiq(adev))
		return 0;

	if (amdgpu_virt_access_debugfs_is_mmio(adev))
		adev->virt.caps &= ~AMDGPU_SRIOV_CAPS_RUNTIME;
	else
		return -EPERM;

	return 0;
}

void amdgpu_virt_disable_access_debugfs(struct amdgpu_device *adev)
{
	if (amdgpu_sriov_vf(adev))
		adev->virt.caps |= AMDGPU_SRIOV_CAPS_RUNTIME;
}

enum amdgpu_sriov_vf_mode amdgpu_virt_get_sriov_vf_mode(struct amdgpu_device *adev)
{
	enum amdgpu_sriov_vf_mode mode;

	if (amdgpu_sriov_vf(adev)) {
		if (amdgpu_sriov_is_pp_one_vf(adev))
			mode = SRIOV_VF_MODE_ONE_VF;
		else
			mode = SRIOV_VF_MODE_MULTI_VF;
	} else {
		mode = SRIOV_VF_MODE_BARE_METAL;
	}

	return mode;
}

void amdgpu_virt_update_sriov_video_codec(struct amdgpu_device *adev,
			struct amdgpu_video_codec_info *encode, uint32_t encode_array_size,
			struct amdgpu_video_codec_info *decode, uint32_t decode_array_size)
{
	uint32_t i;

	if (!adev->virt.is_mm_bw_enabled)
		return;

	if (encode) {
		for (i = 0; i < encode_array_size; i++) {
			encode[i].max_width = adev->virt.encode_max_dimension_pixels;
			encode[i].max_pixels_per_frame = adev->virt.encode_max_frame_pixels;
			if (encode[i].max_width > 0)
				encode[i].max_height = encode[i].max_pixels_per_frame / encode[i].max_width;
			else
				encode[i].max_height = 0;
		}
	}

	if (decode) {
		for (i = 0; i < decode_array_size; i++) {
			decode[i].max_width = adev->virt.decode_max_dimension_pixels;
			decode[i].max_pixels_per_frame = adev->virt.decode_max_frame_pixels;
			if (decode[i].max_width > 0)
				decode[i].max_height = decode[i].max_pixels_per_frame / decode[i].max_width;
			else
				decode[i].max_height = 0;
		}
	}
}
>>>>>>> upstream/android-13
