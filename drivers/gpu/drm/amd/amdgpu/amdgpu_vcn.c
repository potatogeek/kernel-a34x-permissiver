/*
 * Copyright 2016 Advanced Micro Devices, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 */

#include <linux/firmware.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/drm.h>
=======
#include <linux/pci.h>
#include <drm/drm_drv.h>
>>>>>>> upstream/android-13

#include "amdgpu.h"
#include "amdgpu_pm.h"
#include "amdgpu_vcn.h"
#include "soc15d.h"
<<<<<<< HEAD
#include "soc15_common.h"

#include "vcn/vcn_1_0_offset.h"

/* 1 second timeout */
#define VCN_IDLE_TIMEOUT	msecs_to_jiffies(1000)

/* Firmware Names */
#define FIRMWARE_RAVEN		"amdgpu/raven_vcn.bin"

MODULE_FIRMWARE(FIRMWARE_RAVEN);
=======

/* Firmware Names */
#define FIRMWARE_RAVEN		"amdgpu/raven_vcn.bin"
#define FIRMWARE_PICASSO	"amdgpu/picasso_vcn.bin"
#define FIRMWARE_RAVEN2		"amdgpu/raven2_vcn.bin"
#define FIRMWARE_ARCTURUS	"amdgpu/arcturus_vcn.bin"
#define FIRMWARE_RENOIR		"amdgpu/renoir_vcn.bin"
#define FIRMWARE_GREEN_SARDINE	"amdgpu/green_sardine_vcn.bin"
#define FIRMWARE_NAVI10		"amdgpu/navi10_vcn.bin"
#define FIRMWARE_NAVI14		"amdgpu/navi14_vcn.bin"
#define FIRMWARE_NAVI12		"amdgpu/navi12_vcn.bin"
#define FIRMWARE_SIENNA_CICHLID	"amdgpu/sienna_cichlid_vcn.bin"
#define FIRMWARE_NAVY_FLOUNDER	"amdgpu/navy_flounder_vcn.bin"
#define FIRMWARE_VANGOGH	"amdgpu/vangogh_vcn.bin"
#define FIRMWARE_DIMGREY_CAVEFISH	"amdgpu/dimgrey_cavefish_vcn.bin"
#define FIRMWARE_ALDEBARAN	"amdgpu/aldebaran_vcn.bin"
#define FIRMWARE_BEIGE_GOBY	"amdgpu/beige_goby_vcn.bin"
#define FIRMWARE_YELLOW_CARP	"amdgpu/yellow_carp_vcn.bin"

MODULE_FIRMWARE(FIRMWARE_RAVEN);
MODULE_FIRMWARE(FIRMWARE_PICASSO);
MODULE_FIRMWARE(FIRMWARE_RAVEN2);
MODULE_FIRMWARE(FIRMWARE_ARCTURUS);
MODULE_FIRMWARE(FIRMWARE_RENOIR);
MODULE_FIRMWARE(FIRMWARE_GREEN_SARDINE);
MODULE_FIRMWARE(FIRMWARE_ALDEBARAN);
MODULE_FIRMWARE(FIRMWARE_NAVI10);
MODULE_FIRMWARE(FIRMWARE_NAVI14);
MODULE_FIRMWARE(FIRMWARE_NAVI12);
MODULE_FIRMWARE(FIRMWARE_SIENNA_CICHLID);
MODULE_FIRMWARE(FIRMWARE_NAVY_FLOUNDER);
MODULE_FIRMWARE(FIRMWARE_VANGOGH);
MODULE_FIRMWARE(FIRMWARE_DIMGREY_CAVEFISH);
MODULE_FIRMWARE(FIRMWARE_BEIGE_GOBY);
MODULE_FIRMWARE(FIRMWARE_YELLOW_CARP);
>>>>>>> upstream/android-13

static void amdgpu_vcn_idle_work_handler(struct work_struct *work);

int amdgpu_vcn_sw_init(struct amdgpu_device *adev)
{
	unsigned long bo_size;
	const char *fw_name;
	const struct common_firmware_header *hdr;
	unsigned char fw_check;
<<<<<<< HEAD
	int r;

	INIT_DELAYED_WORK(&adev->vcn.idle_work, amdgpu_vcn_idle_work_handler);

	switch (adev->asic_type) {
	case CHIP_RAVEN:
		fw_name = FIRMWARE_RAVEN;
=======
	int i, r;

	INIT_DELAYED_WORK(&adev->vcn.idle_work, amdgpu_vcn_idle_work_handler);
	mutex_init(&adev->vcn.vcn_pg_lock);
	mutex_init(&adev->vcn.vcn1_jpeg1_workaround);
	atomic_set(&adev->vcn.total_submission_cnt, 0);
	for (i = 0; i < adev->vcn.num_vcn_inst; i++)
		atomic_set(&adev->vcn.inst[i].dpg_enc_submission_cnt, 0);

	switch (adev->asic_type) {
	case CHIP_RAVEN:
		if (adev->apu_flags & AMD_APU_IS_RAVEN2)
			fw_name = FIRMWARE_RAVEN2;
		else if (adev->apu_flags & AMD_APU_IS_PICASSO)
			fw_name = FIRMWARE_PICASSO;
		else
			fw_name = FIRMWARE_RAVEN;
		break;
	case CHIP_ARCTURUS:
		fw_name = FIRMWARE_ARCTURUS;
		if ((adev->firmware.load_type == AMDGPU_FW_LOAD_PSP) &&
		    (adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG))
			adev->vcn.indirect_sram = true;
		break;
	case CHIP_RENOIR:
		if (adev->apu_flags & AMD_APU_IS_RENOIR)
			fw_name = FIRMWARE_RENOIR;
		else
			fw_name = FIRMWARE_GREEN_SARDINE;

		if ((adev->firmware.load_type == AMDGPU_FW_LOAD_PSP) &&
		    (adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG))
			adev->vcn.indirect_sram = true;
		break;
	case CHIP_ALDEBARAN:
		fw_name = FIRMWARE_ALDEBARAN;
		if ((adev->firmware.load_type == AMDGPU_FW_LOAD_PSP) &&
		    (adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG))
			adev->vcn.indirect_sram = true;
		break;
	case CHIP_NAVI10:
		fw_name = FIRMWARE_NAVI10;
		if ((adev->firmware.load_type == AMDGPU_FW_LOAD_PSP) &&
		    (adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG))
			adev->vcn.indirect_sram = true;
		break;
	case CHIP_NAVI14:
		fw_name = FIRMWARE_NAVI14;
		if ((adev->firmware.load_type == AMDGPU_FW_LOAD_PSP) &&
		    (adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG))
			adev->vcn.indirect_sram = true;
		break;
	case CHIP_NAVI12:
		fw_name = FIRMWARE_NAVI12;
		if ((adev->firmware.load_type == AMDGPU_FW_LOAD_PSP) &&
		    (adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG))
			adev->vcn.indirect_sram = true;
		break;
	case CHIP_SIENNA_CICHLID:
		fw_name = FIRMWARE_SIENNA_CICHLID;
		if ((adev->firmware.load_type == AMDGPU_FW_LOAD_PSP) &&
		    (adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG))
			adev->vcn.indirect_sram = true;
		break;
	case CHIP_NAVY_FLOUNDER:
		fw_name = FIRMWARE_NAVY_FLOUNDER;
		if ((adev->firmware.load_type == AMDGPU_FW_LOAD_PSP) &&
		    (adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG))
			adev->vcn.indirect_sram = true;
		break;
	case CHIP_VANGOGH:
		fw_name = FIRMWARE_VANGOGH;
		break;
	case CHIP_DIMGREY_CAVEFISH:
		fw_name = FIRMWARE_DIMGREY_CAVEFISH;
		if ((adev->firmware.load_type == AMDGPU_FW_LOAD_PSP) &&
		    (adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG))
			adev->vcn.indirect_sram = true;
		break;
	case CHIP_BEIGE_GOBY:
		fw_name = FIRMWARE_BEIGE_GOBY;
		if ((adev->firmware.load_type == AMDGPU_FW_LOAD_PSP) &&
		    (adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG))
			adev->vcn.indirect_sram = true;
		break;
	case CHIP_YELLOW_CARP:
		fw_name = FIRMWARE_YELLOW_CARP;
		if ((adev->firmware.load_type == AMDGPU_FW_LOAD_PSP) &&
		    (adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG))
			adev->vcn.indirect_sram = true;
>>>>>>> upstream/android-13
		break;
	default:
		return -EINVAL;
	}

	r = request_firmware(&adev->vcn.fw, fw_name, adev->dev);
	if (r) {
		dev_err(adev->dev, "amdgpu_vcn: Can't load firmware \"%s\"\n",
			fw_name);
		return r;
	}

	r = amdgpu_ucode_validate(adev->vcn.fw);
	if (r) {
		dev_err(adev->dev, "amdgpu_vcn: Can't validate firmware \"%s\"\n",
			fw_name);
		release_firmware(adev->vcn.fw);
		adev->vcn.fw = NULL;
		return r;
	}

	hdr = (const struct common_firmware_header *)adev->vcn.fw->data;
	adev->vcn.fw_version = le32_to_cpu(hdr->ucode_version);

	/* Bit 20-23, it is encode major and non-zero for new naming convention.
	 * This field is part of version minor and DRM_DISABLED_FLAG in old naming
	 * convention. Since the l:wq!atest version minor is 0x5B and DRM_DISABLED_FLAG
	 * is zero in old naming convention, this field is always zero so far.
	 * These four bits are used to tell which naming convention is present.
	 */
	fw_check = (le32_to_cpu(hdr->ucode_version) >> 20) & 0xf;
	if (fw_check) {
		unsigned int dec_ver, enc_major, enc_minor, vep, fw_rev;

		fw_rev = le32_to_cpu(hdr->ucode_version) & 0xfff;
		enc_minor = (le32_to_cpu(hdr->ucode_version) >> 12) & 0xff;
		enc_major = fw_check;
		dec_ver = (le32_to_cpu(hdr->ucode_version) >> 24) & 0xf;
		vep = (le32_to_cpu(hdr->ucode_version) >> 28) & 0xf;
<<<<<<< HEAD
		DRM_INFO("Found VCN firmware Version ENC: %hu.%hu DEC: %hu VEP: %hu Revision: %hu\n",
=======
		DRM_INFO("Found VCN firmware Version ENC: %u.%u DEC: %u VEP: %u Revision: %u\n",
>>>>>>> upstream/android-13
			enc_major, enc_minor, dec_ver, vep, fw_rev);
	} else {
		unsigned int version_major, version_minor, family_id;

		family_id = le32_to_cpu(hdr->ucode_version) & 0xff;
		version_major = (le32_to_cpu(hdr->ucode_version) >> 24) & 0xff;
		version_minor = (le32_to_cpu(hdr->ucode_version) >> 8) & 0xff;
<<<<<<< HEAD
		DRM_INFO("Found VCN firmware Version: %hu.%hu Family ID: %hu\n",
			version_major, version_minor, family_id);
	}

	bo_size = AMDGPU_VCN_STACK_SIZE + AMDGPU_VCN_HEAP_SIZE
		  +  AMDGPU_VCN_SESSION_SIZE * 40;
	if (adev->firmware.load_type != AMDGPU_FW_LOAD_PSP)
		bo_size += AMDGPU_GPU_PAGE_ALIGN(le32_to_cpu(hdr->ucode_size_bytes) + 8);
	r = amdgpu_bo_create_kernel(adev, bo_size, PAGE_SIZE,
				    AMDGPU_GEM_DOMAIN_VRAM, &adev->vcn.vcpu_bo,
				    &adev->vcn.gpu_addr, &adev->vcn.cpu_addr);
	if (r) {
		dev_err(adev->dev, "(%d) failed to allocate vcn bo\n", r);
		return r;
=======
		DRM_INFO("Found VCN firmware Version: %u.%u Family ID: %u\n",
			version_major, version_minor, family_id);
	}

	bo_size = AMDGPU_VCN_STACK_SIZE + AMDGPU_VCN_CONTEXT_SIZE;
	if (adev->firmware.load_type != AMDGPU_FW_LOAD_PSP)
		bo_size += AMDGPU_GPU_PAGE_ALIGN(le32_to_cpu(hdr->ucode_size_bytes) + 8);
	bo_size += AMDGPU_GPU_PAGE_ALIGN(sizeof(struct amdgpu_fw_shared));

	for (i = 0; i < adev->vcn.num_vcn_inst; i++) {
		if (adev->vcn.harvest_config & (1 << i))
			continue;

		r = amdgpu_bo_create_kernel(adev, bo_size, PAGE_SIZE,
						AMDGPU_GEM_DOMAIN_VRAM, &adev->vcn.inst[i].vcpu_bo,
						&adev->vcn.inst[i].gpu_addr, &adev->vcn.inst[i].cpu_addr);
		if (r) {
			dev_err(adev->dev, "(%d) failed to allocate vcn bo\n", r);
			return r;
		}

		adev->vcn.inst[i].fw_shared_cpu_addr = adev->vcn.inst[i].cpu_addr +
				bo_size - AMDGPU_GPU_PAGE_ALIGN(sizeof(struct amdgpu_fw_shared));
		adev->vcn.inst[i].fw_shared_gpu_addr = adev->vcn.inst[i].gpu_addr +
				bo_size - AMDGPU_GPU_PAGE_ALIGN(sizeof(struct amdgpu_fw_shared));

		if (adev->vcn.indirect_sram) {
			r = amdgpu_bo_create_kernel(adev, 64 * 2 * 4, PAGE_SIZE,
					AMDGPU_GEM_DOMAIN_VRAM, &adev->vcn.inst[i].dpg_sram_bo,
					&adev->vcn.inst[i].dpg_sram_gpu_addr, &adev->vcn.inst[i].dpg_sram_cpu_addr);
			if (r) {
				dev_err(adev->dev, "VCN %d (%d) failed to allocate DPG bo\n", i, r);
				return r;
			}
		}
>>>>>>> upstream/android-13
	}

	return 0;
}

int amdgpu_vcn_sw_fini(struct amdgpu_device *adev)
{
<<<<<<< HEAD
	int i;

	kvfree(adev->vcn.saved_bo);

	amdgpu_bo_free_kernel(&adev->vcn.vcpu_bo,
			      &adev->vcn.gpu_addr,
			      (void **)&adev->vcn.cpu_addr);

	amdgpu_ring_fini(&adev->vcn.ring_dec);

	for (i = 0; i < adev->vcn.num_enc_rings; ++i)
		amdgpu_ring_fini(&adev->vcn.ring_enc[i]);

	amdgpu_ring_fini(&adev->vcn.ring_jpeg);

	release_firmware(adev->vcn.fw);
=======
	int i, j;

	for (j = 0; j < adev->vcn.num_vcn_inst; ++j) {
		if (adev->vcn.harvest_config & (1 << j))
			continue;

		if (adev->vcn.indirect_sram) {
			amdgpu_bo_free_kernel(&adev->vcn.inst[j].dpg_sram_bo,
						  &adev->vcn.inst[j].dpg_sram_gpu_addr,
						  (void **)&adev->vcn.inst[j].dpg_sram_cpu_addr);
		}
		kvfree(adev->vcn.inst[j].saved_bo);

		amdgpu_bo_free_kernel(&adev->vcn.inst[j].vcpu_bo,
					  &adev->vcn.inst[j].gpu_addr,
					  (void **)&adev->vcn.inst[j].cpu_addr);

		amdgpu_ring_fini(&adev->vcn.inst[j].ring_dec);

		for (i = 0; i < adev->vcn.num_enc_rings; ++i)
			amdgpu_ring_fini(&adev->vcn.inst[j].ring_enc[i]);
	}

	release_firmware(adev->vcn.fw);
	mutex_destroy(&adev->vcn.vcn1_jpeg1_workaround);
	mutex_destroy(&adev->vcn.vcn_pg_lock);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
=======
bool amdgpu_vcn_is_disabled_vcn(struct amdgpu_device *adev, enum vcn_ring_type type, uint32_t vcn_instance)
{
	bool ret = false;

	int major;
	int minor;
	int revision;

	/* if cannot find IP data, then this VCN does not exist */
	if (amdgpu_discovery_get_vcn_version(adev, vcn_instance, &major, &minor, &revision) != 0)
		return true;

	if ((type == VCN_ENCODE_RING) && (revision & VCN_BLOCK_ENCODE_DISABLE_MASK)) {
		ret = true;
	} else if ((type == VCN_DECODE_RING) && (revision & VCN_BLOCK_DECODE_DISABLE_MASK)) {
		ret = true;
	} else if ((type == VCN_UNIFIED_RING) && (revision & VCN_BLOCK_QUEUE_DISABLE_MASK)) {
		ret = true;
	}

	return ret;
}

>>>>>>> upstream/android-13
int amdgpu_vcn_suspend(struct amdgpu_device *adev)
{
	unsigned size;
	void *ptr;
<<<<<<< HEAD

	cancel_delayed_work_sync(&adev->vcn.idle_work);

	if (adev->vcn.vcpu_bo == NULL)
		return 0;

	size = amdgpu_bo_size(adev->vcn.vcpu_bo);
	ptr = adev->vcn.cpu_addr;

	adev->vcn.saved_bo = kvmalloc(size, GFP_KERNEL);
	if (!adev->vcn.saved_bo)
		return -ENOMEM;

	memcpy_fromio(adev->vcn.saved_bo, ptr, size);

=======
	int i, idx;

	cancel_delayed_work_sync(&adev->vcn.idle_work);

	for (i = 0; i < adev->vcn.num_vcn_inst; ++i) {
		if (adev->vcn.harvest_config & (1 << i))
			continue;
		if (adev->vcn.inst[i].vcpu_bo == NULL)
			return 0;

		size = amdgpu_bo_size(adev->vcn.inst[i].vcpu_bo);
		ptr = adev->vcn.inst[i].cpu_addr;

		adev->vcn.inst[i].saved_bo = kvmalloc(size, GFP_KERNEL);
		if (!adev->vcn.inst[i].saved_bo)
			return -ENOMEM;

		if (drm_dev_enter(&adev->ddev, &idx)) {
			memcpy_fromio(adev->vcn.inst[i].saved_bo, ptr, size);
			drm_dev_exit(idx);
		}
	}
>>>>>>> upstream/android-13
	return 0;
}

int amdgpu_vcn_resume(struct amdgpu_device *adev)
{
	unsigned size;
	void *ptr;
<<<<<<< HEAD

	if (adev->vcn.vcpu_bo == NULL)
		return -EINVAL;

	size = amdgpu_bo_size(adev->vcn.vcpu_bo);
	ptr = adev->vcn.cpu_addr;

	if (adev->vcn.saved_bo != NULL) {
		memcpy_toio(ptr, adev->vcn.saved_bo, size);
		kvfree(adev->vcn.saved_bo);
		adev->vcn.saved_bo = NULL;
	} else {
		const struct common_firmware_header *hdr;
		unsigned offset;

		hdr = (const struct common_firmware_header *)adev->vcn.fw->data;
		if (adev->firmware.load_type != AMDGPU_FW_LOAD_PSP) {
			offset = le32_to_cpu(hdr->ucode_array_offset_bytes);
			memcpy_toio(adev->vcn.cpu_addr, adev->vcn.fw->data + offset,
				    le32_to_cpu(hdr->ucode_size_bytes));
			size -= le32_to_cpu(hdr->ucode_size_bytes);
			ptr += le32_to_cpu(hdr->ucode_size_bytes);
		}
		memset_io(ptr, 0, size);
	}

=======
	int i, idx;

	for (i = 0; i < adev->vcn.num_vcn_inst; ++i) {
		if (adev->vcn.harvest_config & (1 << i))
			continue;
		if (adev->vcn.inst[i].vcpu_bo == NULL)
			return -EINVAL;

		size = amdgpu_bo_size(adev->vcn.inst[i].vcpu_bo);
		ptr = adev->vcn.inst[i].cpu_addr;

		if (adev->vcn.inst[i].saved_bo != NULL) {
			if (drm_dev_enter(&adev->ddev, &idx)) {
				memcpy_toio(ptr, adev->vcn.inst[i].saved_bo, size);
				drm_dev_exit(idx);
			}
			kvfree(adev->vcn.inst[i].saved_bo);
			adev->vcn.inst[i].saved_bo = NULL;
		} else {
			const struct common_firmware_header *hdr;
			unsigned offset;

			hdr = (const struct common_firmware_header *)adev->vcn.fw->data;
			if (adev->firmware.load_type != AMDGPU_FW_LOAD_PSP) {
				offset = le32_to_cpu(hdr->ucode_array_offset_bytes);
				if (drm_dev_enter(&adev->ddev, &idx)) {
					memcpy_toio(adev->vcn.inst[i].cpu_addr, adev->vcn.fw->data + offset,
						    le32_to_cpu(hdr->ucode_size_bytes));
					drm_dev_exit(idx);
				}
				size -= le32_to_cpu(hdr->ucode_size_bytes);
				ptr += le32_to_cpu(hdr->ucode_size_bytes);
			}
			memset_io(ptr, 0, size);
		}
	}
>>>>>>> upstream/android-13
	return 0;
}

static void amdgpu_vcn_idle_work_handler(struct work_struct *work)
{
	struct amdgpu_device *adev =
		container_of(work, struct amdgpu_device, vcn.idle_work.work);
<<<<<<< HEAD
	unsigned fences = amdgpu_fence_count_emitted(&adev->vcn.ring_dec);
	unsigned i;

	for (i = 0; i < adev->vcn.num_enc_rings; ++i) {
		fences += amdgpu_fence_count_emitted(&adev->vcn.ring_enc[i]);
	}

	fences += amdgpu_fence_count_emitted(&adev->vcn.ring_jpeg);

	if (fences == 0) {
		if (adev->pm.dpm_enabled)
			amdgpu_dpm_enable_uvd(adev, false);
		else
			amdgpu_device_ip_set_powergating_state(adev, AMD_IP_BLOCK_TYPE_VCN,
							       AMD_PG_STATE_GATE);
=======
	unsigned int fences = 0, fence[AMDGPU_MAX_VCN_INSTANCES] = {0};
	unsigned int i, j;
	int r = 0;

	for (j = 0; j < adev->vcn.num_vcn_inst; ++j) {
		if (adev->vcn.harvest_config & (1 << j))
			continue;

		for (i = 0; i < adev->vcn.num_enc_rings; ++i) {
			fence[j] += amdgpu_fence_count_emitted(&adev->vcn.inst[j].ring_enc[i]);
		}

		if (adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG)	{
			struct dpg_pause_state new_state;

			if (fence[j] ||
				unlikely(atomic_read(&adev->vcn.inst[j].dpg_enc_submission_cnt)))
				new_state.fw_based = VCN_DPG_STATE__PAUSE;
			else
				new_state.fw_based = VCN_DPG_STATE__UNPAUSE;

			adev->vcn.pause_dpg_mode(adev, j, &new_state);
		}

		fence[j] += amdgpu_fence_count_emitted(&adev->vcn.inst[j].ring_dec);
		fences += fence[j];
	}

	if (!fences && !atomic_read(&adev->vcn.total_submission_cnt)) {
		amdgpu_device_ip_set_powergating_state(adev, AMD_IP_BLOCK_TYPE_VCN,
		       AMD_PG_STATE_GATE);
		r = amdgpu_dpm_switch_power_profile(adev, PP_SMC_POWER_PROFILE_VIDEO,
				false);
		if (r)
			dev_warn(adev->dev, "(%d) failed to disable video power profile mode\n", r);
>>>>>>> upstream/android-13
	} else {
		schedule_delayed_work(&adev->vcn.idle_work, VCN_IDLE_TIMEOUT);
	}
}

void amdgpu_vcn_ring_begin_use(struct amdgpu_ring *ring)
{
	struct amdgpu_device *adev = ring->adev;
<<<<<<< HEAD
	bool set_clocks = !cancel_delayed_work_sync(&adev->vcn.idle_work);

	if (set_clocks) {
		if (adev->pm.dpm_enabled)
			amdgpu_dpm_enable_uvd(adev, true);
		else
			amdgpu_device_ip_set_powergating_state(adev, AMD_IP_BLOCK_TYPE_VCN,
							       AMD_PG_STATE_UNGATE);
	}
=======
	int r = 0;

	atomic_inc(&adev->vcn.total_submission_cnt);

	if (!cancel_delayed_work_sync(&adev->vcn.idle_work)) {
		r = amdgpu_dpm_switch_power_profile(adev, PP_SMC_POWER_PROFILE_VIDEO,
				true);
		if (r)
			dev_warn(adev->dev, "(%d) failed to switch to video power profile mode\n", r);
	}

	mutex_lock(&adev->vcn.vcn_pg_lock);
	amdgpu_device_ip_set_powergating_state(adev, AMD_IP_BLOCK_TYPE_VCN,
	       AMD_PG_STATE_UNGATE);

	if (adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG)	{
		struct dpg_pause_state new_state;

		if (ring->funcs->type == AMDGPU_RING_TYPE_VCN_ENC) {
			atomic_inc(&adev->vcn.inst[ring->me].dpg_enc_submission_cnt);
			new_state.fw_based = VCN_DPG_STATE__PAUSE;
		} else {
			unsigned int fences = 0;
			unsigned int i;

			for (i = 0; i < adev->vcn.num_enc_rings; ++i)
				fences += amdgpu_fence_count_emitted(&adev->vcn.inst[ring->me].ring_enc[i]);

			if (fences || atomic_read(&adev->vcn.inst[ring->me].dpg_enc_submission_cnt))
				new_state.fw_based = VCN_DPG_STATE__PAUSE;
			else
				new_state.fw_based = VCN_DPG_STATE__UNPAUSE;
		}

		adev->vcn.pause_dpg_mode(adev, ring->me, &new_state);
	}
	mutex_unlock(&adev->vcn.vcn_pg_lock);
>>>>>>> upstream/android-13
}

void amdgpu_vcn_ring_end_use(struct amdgpu_ring *ring)
{
<<<<<<< HEAD
=======
	if (ring->adev->pg_flags & AMD_PG_SUPPORT_VCN_DPG &&
		ring->funcs->type == AMDGPU_RING_TYPE_VCN_ENC)
		atomic_dec(&ring->adev->vcn.inst[ring->me].dpg_enc_submission_cnt);

	atomic_dec(&ring->adev->vcn.total_submission_cnt);

>>>>>>> upstream/android-13
	schedule_delayed_work(&ring->adev->vcn.idle_work, VCN_IDLE_TIMEOUT);
}

int amdgpu_vcn_dec_ring_test_ring(struct amdgpu_ring *ring)
{
	struct amdgpu_device *adev = ring->adev;
	uint32_t tmp = 0;
	unsigned i;
	int r;

<<<<<<< HEAD
	WREG32(SOC15_REG_OFFSET(UVD, 0, mmUVD_CONTEXT_ID), 0xCAFEDEAD);
	r = amdgpu_ring_alloc(ring, 3);
	if (r) {
		DRM_ERROR("amdgpu: cp failed to lock ring %d (%d).\n",
			  ring->idx, r);
		return r;
	}
	amdgpu_ring_write(ring,
		PACKET0(SOC15_REG_OFFSET(UVD, 0, mmUVD_CONTEXT_ID), 0));
	amdgpu_ring_write(ring, 0xDEADBEEF);
	amdgpu_ring_commit(ring);
	for (i = 0; i < adev->usec_timeout; i++) {
		tmp = RREG32(SOC15_REG_OFFSET(UVD, 0, mmUVD_CONTEXT_ID));
		if (tmp == 0xDEADBEEF)
			break;
		DRM_UDELAY(1);
	}

	if (i < adev->usec_timeout) {
		DRM_DEBUG("ring test on %d succeeded in %d usecs\n",
			 ring->idx, i);
	} else {
		DRM_ERROR("amdgpu: ring %d test failed (0x%08X)\n",
			  ring->idx, tmp);
		r = -EINVAL;
	}
=======
	/* VCN in SRIOV does not support direct register read/write */
	if (amdgpu_sriov_vf(adev))
		return 0;

	WREG32(adev->vcn.inst[ring->me].external.scratch9, 0xCAFEDEAD);
	r = amdgpu_ring_alloc(ring, 3);
	if (r)
		return r;
	amdgpu_ring_write(ring, PACKET0(adev->vcn.internal.scratch9, 0));
	amdgpu_ring_write(ring, 0xDEADBEEF);
	amdgpu_ring_commit(ring);
	for (i = 0; i < adev->usec_timeout; i++) {
		tmp = RREG32(adev->vcn.inst[ring->me].external.scratch9);
		if (tmp == 0xDEADBEEF)
			break;
		udelay(1);
	}

	if (i >= adev->usec_timeout)
		r = -ETIMEDOUT;

	return r;
}

int amdgpu_vcn_dec_sw_ring_test_ring(struct amdgpu_ring *ring)
{
	struct amdgpu_device *adev = ring->adev;
	uint32_t rptr;
	unsigned int i;
	int r;

	if (amdgpu_sriov_vf(adev))
		return 0;

	r = amdgpu_ring_alloc(ring, 16);
	if (r)
		return r;

	rptr = amdgpu_ring_get_rptr(ring);

	amdgpu_ring_write(ring, VCN_DEC_SW_CMD_END);
	amdgpu_ring_commit(ring);

	for (i = 0; i < adev->usec_timeout; i++) {
		if (amdgpu_ring_get_rptr(ring) != rptr)
			break;
		udelay(1);
	}

	if (i >= adev->usec_timeout)
		r = -ETIMEDOUT;

>>>>>>> upstream/android-13
	return r;
}

static int amdgpu_vcn_dec_send_msg(struct amdgpu_ring *ring,
				   struct amdgpu_bo *bo,
				   struct dma_fence **fence)
{
	struct amdgpu_device *adev = ring->adev;
	struct dma_fence *f = NULL;
	struct amdgpu_job *job;
	struct amdgpu_ib *ib;
	uint64_t addr;
<<<<<<< HEAD
	int i, r;

	r = amdgpu_job_alloc_with_ib(adev, 64, &job);
=======
	void *msg = NULL;
	int i, r;

	r = amdgpu_job_alloc_with_ib(adev, 64,
					AMDGPU_IB_POOL_DIRECT, &job);
>>>>>>> upstream/android-13
	if (r)
		goto err;

	ib = &job->ibs[0];
	addr = amdgpu_bo_gpu_offset(bo);
<<<<<<< HEAD
	ib->ptr[0] = PACKET0(SOC15_REG_OFFSET(UVD, 0, mmUVD_GPCOM_VCPU_DATA0), 0);
	ib->ptr[1] = addr;
	ib->ptr[2] = PACKET0(SOC15_REG_OFFSET(UVD, 0, mmUVD_GPCOM_VCPU_DATA1), 0);
	ib->ptr[3] = addr >> 32;
	ib->ptr[4] = PACKET0(SOC15_REG_OFFSET(UVD, 0, mmUVD_GPCOM_VCPU_CMD), 0);
	ib->ptr[5] = 0;
	for (i = 6; i < 16; i += 2) {
		ib->ptr[i] = PACKET0(SOC15_REG_OFFSET(UVD, 0, mmUVD_NO_OP), 0);
=======
	msg = amdgpu_bo_kptr(bo);
	ib->ptr[0] = PACKET0(adev->vcn.internal.data0, 0);
	ib->ptr[1] = addr;
	ib->ptr[2] = PACKET0(adev->vcn.internal.data1, 0);
	ib->ptr[3] = addr >> 32;
	ib->ptr[4] = PACKET0(adev->vcn.internal.cmd, 0);
	ib->ptr[5] = 0;
	for (i = 6; i < 16; i += 2) {
		ib->ptr[i] = PACKET0(adev->vcn.internal.nop, 0);
>>>>>>> upstream/android-13
		ib->ptr[i+1] = 0;
	}
	ib->length_dw = 16;

	r = amdgpu_job_submit_direct(job, ring, &f);
	if (r)
		goto err_free;

	amdgpu_bo_fence(bo, f, false);
	amdgpu_bo_unreserve(bo);
<<<<<<< HEAD
=======
	amdgpu_bo_free_kernel(&bo, NULL, (void **)&msg);

	if (fence)
		*fence = dma_fence_get(f);
	dma_fence_put(f);

	return 0;

err_free:
	amdgpu_job_free(job);

err:
	amdgpu_bo_unreserve(bo);
	amdgpu_bo_free_kernel(&bo, NULL, (void **)&msg);
	return r;
}

static int amdgpu_vcn_dec_get_create_msg(struct amdgpu_ring *ring, uint32_t handle,
					 struct amdgpu_bo **bo)
{
	struct amdgpu_device *adev = ring->adev;
	uint32_t *msg;
	int r, i;

	*bo = NULL;
	r = amdgpu_bo_create_reserved(adev, 1024, PAGE_SIZE,
				      AMDGPU_GEM_DOMAIN_VRAM,
				      bo, NULL, (void **)&msg);
	if (r)
		return r;

	msg[0] = cpu_to_le32(0x00000028);
	msg[1] = cpu_to_le32(0x00000038);
	msg[2] = cpu_to_le32(0x00000001);
	msg[3] = cpu_to_le32(0x00000000);
	msg[4] = cpu_to_le32(handle);
	msg[5] = cpu_to_le32(0x00000000);
	msg[6] = cpu_to_le32(0x00000001);
	msg[7] = cpu_to_le32(0x00000028);
	msg[8] = cpu_to_le32(0x00000010);
	msg[9] = cpu_to_le32(0x00000000);
	msg[10] = cpu_to_le32(0x00000007);
	msg[11] = cpu_to_le32(0x00000000);
	msg[12] = cpu_to_le32(0x00000780);
	msg[13] = cpu_to_le32(0x00000440);
	for (i = 14; i < 1024; ++i)
		msg[i] = cpu_to_le32(0x0);

	return 0;
}

static int amdgpu_vcn_dec_get_destroy_msg(struct amdgpu_ring *ring, uint32_t handle,
					  struct amdgpu_bo **bo)
{
	struct amdgpu_device *adev = ring->adev;
	uint32_t *msg;
	int r, i;

	*bo = NULL;
	r = amdgpu_bo_create_reserved(adev, 1024, PAGE_SIZE,
				      AMDGPU_GEM_DOMAIN_VRAM,
				      bo, NULL, (void **)&msg);
	if (r)
		return r;

	msg[0] = cpu_to_le32(0x00000028);
	msg[1] = cpu_to_le32(0x00000018);
	msg[2] = cpu_to_le32(0x00000000);
	msg[3] = cpu_to_le32(0x00000002);
	msg[4] = cpu_to_le32(handle);
	msg[5] = cpu_to_le32(0x00000000);
	for (i = 6; i < 1024; ++i)
		msg[i] = cpu_to_le32(0x0);

	return 0;
}

int amdgpu_vcn_dec_ring_test_ib(struct amdgpu_ring *ring, long timeout)
{
	struct dma_fence *fence = NULL;
	struct amdgpu_bo *bo;
	long r;

	r = amdgpu_vcn_dec_get_create_msg(ring, 1, &bo);
	if (r)
		goto error;

	r = amdgpu_vcn_dec_send_msg(ring, bo, NULL);
	if (r)
		goto error;
	r = amdgpu_vcn_dec_get_destroy_msg(ring, 1, &bo);
	if (r)
		goto error;

	r = amdgpu_vcn_dec_send_msg(ring, bo, &fence);
	if (r)
		goto error;

	r = dma_fence_wait_timeout(fence, false, timeout);
	if (r == 0)
		r = -ETIMEDOUT;
	else if (r > 0)
		r = 0;

	dma_fence_put(fence);
error:
	return r;
}

static int amdgpu_vcn_dec_sw_send_msg(struct amdgpu_ring *ring,
				   struct amdgpu_bo *bo,
				   struct dma_fence **fence)
{
	struct amdgpu_vcn_decode_buffer *decode_buffer = NULL;
	const unsigned int ib_size_dw = 64;
	struct amdgpu_device *adev = ring->adev;
	struct dma_fence *f = NULL;
	struct amdgpu_job *job;
	struct amdgpu_ib *ib;
	uint64_t addr;
	int i, r;

	r = amdgpu_job_alloc_with_ib(adev, ib_size_dw * 4,
				AMDGPU_IB_POOL_DIRECT, &job);
	if (r)
		goto err;

	ib = &job->ibs[0];
	addr = amdgpu_bo_gpu_offset(bo);
	ib->length_dw = 0;

	ib->ptr[ib->length_dw++] = sizeof(struct amdgpu_vcn_decode_buffer) + 8;
	ib->ptr[ib->length_dw++] = cpu_to_le32(AMDGPU_VCN_IB_FLAG_DECODE_BUFFER);
	decode_buffer = (struct amdgpu_vcn_decode_buffer *)&(ib->ptr[ib->length_dw]);
	ib->length_dw += sizeof(struct amdgpu_vcn_decode_buffer) / 4;
	memset(decode_buffer, 0, sizeof(struct amdgpu_vcn_decode_buffer));

	decode_buffer->valid_buf_flag |= cpu_to_le32(AMDGPU_VCN_CMD_FLAG_MSG_BUFFER);
	decode_buffer->msg_buffer_address_hi = cpu_to_le32(addr >> 32);
	decode_buffer->msg_buffer_address_lo = cpu_to_le32(addr);

	for (i = ib->length_dw; i < ib_size_dw; ++i)
		ib->ptr[i] = 0x0;

	r = amdgpu_job_submit_direct(job, ring, &f);
	if (r)
		goto err_free;

	amdgpu_bo_fence(bo, f, false);
	amdgpu_bo_unreserve(bo);
>>>>>>> upstream/android-13
	amdgpu_bo_unref(&bo);

	if (fence)
		*fence = dma_fence_get(f);
	dma_fence_put(f);

	return 0;

err_free:
	amdgpu_job_free(job);

err:
	amdgpu_bo_unreserve(bo);
	amdgpu_bo_unref(&bo);
	return r;
}

<<<<<<< HEAD
static int amdgpu_vcn_dec_get_create_msg(struct amdgpu_ring *ring, uint32_t handle,
			      struct dma_fence **fence)
{
	struct amdgpu_device *adev = ring->adev;
	struct amdgpu_bo *bo = NULL;
	uint32_t *msg;
	int r, i;

	r = amdgpu_bo_create_reserved(adev, 1024, PAGE_SIZE,
				      AMDGPU_GEM_DOMAIN_VRAM,
				      &bo, NULL, (void **)&msg);
	if (r)
		return r;

	msg[0] = cpu_to_le32(0x00000028);
	msg[1] = cpu_to_le32(0x00000038);
	msg[2] = cpu_to_le32(0x00000001);
	msg[3] = cpu_to_le32(0x00000000);
	msg[4] = cpu_to_le32(handle);
	msg[5] = cpu_to_le32(0x00000000);
	msg[6] = cpu_to_le32(0x00000001);
	msg[7] = cpu_to_le32(0x00000028);
	msg[8] = cpu_to_le32(0x00000010);
	msg[9] = cpu_to_le32(0x00000000);
	msg[10] = cpu_to_le32(0x00000007);
	msg[11] = cpu_to_le32(0x00000000);
	msg[12] = cpu_to_le32(0x00000780);
	msg[13] = cpu_to_le32(0x00000440);
	for (i = 14; i < 1024; ++i)
		msg[i] = cpu_to_le32(0x0);

	return amdgpu_vcn_dec_send_msg(ring, bo, fence);
}

static int amdgpu_vcn_dec_get_destroy_msg(struct amdgpu_ring *ring, uint32_t handle,
			       struct dma_fence **fence)
{
	struct amdgpu_device *adev = ring->adev;
	struct amdgpu_bo *bo = NULL;
	uint32_t *msg;
	int r, i;

	r = amdgpu_bo_create_reserved(adev, 1024, PAGE_SIZE,
				      AMDGPU_GEM_DOMAIN_VRAM,
				      &bo, NULL, (void **)&msg);
	if (r)
		return r;

	msg[0] = cpu_to_le32(0x00000028);
	msg[1] = cpu_to_le32(0x00000018);
	msg[2] = cpu_to_le32(0x00000000);
	msg[3] = cpu_to_le32(0x00000002);
	msg[4] = cpu_to_le32(handle);
	msg[5] = cpu_to_le32(0x00000000);
	for (i = 6; i < 1024; ++i)
		msg[i] = cpu_to_le32(0x0);

	return amdgpu_vcn_dec_send_msg(ring, bo, fence);
}

int amdgpu_vcn_dec_ring_test_ib(struct amdgpu_ring *ring, long timeout)
{
	struct dma_fence *fence;
	long r;

	r = amdgpu_vcn_dec_get_create_msg(ring, 1, NULL);
	if (r) {
		DRM_ERROR("amdgpu: failed to get create msg (%ld).\n", r);
		goto error;
	}

	r = amdgpu_vcn_dec_get_destroy_msg(ring, 1, &fence);
	if (r) {
		DRM_ERROR("amdgpu: failed to get destroy ib (%ld).\n", r);
		goto error;
	}

	r = dma_fence_wait_timeout(fence, false, timeout);
	if (r == 0) {
		DRM_ERROR("amdgpu: IB test timed out.\n");
		r = -ETIMEDOUT;
	} else if (r < 0) {
		DRM_ERROR("amdgpu: fence wait failed (%ld).\n", r);
	} else {
		DRM_DEBUG("ib test on ring %d succeeded\n",  ring->idx);
		r = 0;
	}

	dma_fence_put(fence);

=======
int amdgpu_vcn_dec_sw_ring_test_ib(struct amdgpu_ring *ring, long timeout)
{
	struct dma_fence *fence = NULL;
	struct amdgpu_bo *bo;
	long r;

	r = amdgpu_vcn_dec_get_create_msg(ring, 1, &bo);
	if (r)
		goto error;

	r = amdgpu_vcn_dec_sw_send_msg(ring, bo, NULL);
	if (r)
		goto error;
	r = amdgpu_vcn_dec_get_destroy_msg(ring, 1, &bo);
	if (r)
		goto error;

	r = amdgpu_vcn_dec_sw_send_msg(ring, bo, &fence);
	if (r)
		goto error;

	r = dma_fence_wait_timeout(fence, false, timeout);
	if (r == 0)
		r = -ETIMEDOUT;
	else if (r > 0)
		r = 0;

	dma_fence_put(fence);
>>>>>>> upstream/android-13
error:
	return r;
}

int amdgpu_vcn_enc_ring_test_ring(struct amdgpu_ring *ring)
{
	struct amdgpu_device *adev = ring->adev;
	uint32_t rptr;
	unsigned i;
	int r;

<<<<<<< HEAD
	r = amdgpu_ring_alloc(ring, 16);
	if (r) {
		DRM_ERROR("amdgpu: vcn enc failed to lock ring %d (%d).\n",
			  ring->idx, r);
		return r;
	}
=======
	if (amdgpu_sriov_vf(adev))
		return 0;

	r = amdgpu_ring_alloc(ring, 16);
	if (r)
		return r;
>>>>>>> upstream/android-13

	rptr = amdgpu_ring_get_rptr(ring);

	amdgpu_ring_write(ring, VCN_ENC_CMD_END);
	amdgpu_ring_commit(ring);

	for (i = 0; i < adev->usec_timeout; i++) {
		if (amdgpu_ring_get_rptr(ring) != rptr)
			break;
<<<<<<< HEAD
		DRM_UDELAY(1);
	}

	if (i < adev->usec_timeout) {
		DRM_DEBUG("ring test on %d succeeded in %d usecs\n",
			 ring->idx, i);
	} else {
		DRM_ERROR("amdgpu: ring %d test failed\n",
			  ring->idx);
		r = -ETIMEDOUT;
	}
=======
		udelay(1);
	}

	if (i >= adev->usec_timeout)
		r = -ETIMEDOUT;
>>>>>>> upstream/android-13

	return r;
}

static int amdgpu_vcn_enc_get_create_msg(struct amdgpu_ring *ring, uint32_t handle,
<<<<<<< HEAD
			      struct dma_fence **fence)
=======
					 struct amdgpu_bo *bo,
					 struct dma_fence **fence)
>>>>>>> upstream/android-13
{
	const unsigned ib_size_dw = 16;
	struct amdgpu_job *job;
	struct amdgpu_ib *ib;
	struct dma_fence *f = NULL;
<<<<<<< HEAD
	uint64_t dummy;
	int i, r;

	r = amdgpu_job_alloc_with_ib(ring->adev, ib_size_dw * 4, &job);
=======
	uint64_t addr;
	int i, r;

	r = amdgpu_job_alloc_with_ib(ring->adev, ib_size_dw * 4,
					AMDGPU_IB_POOL_DIRECT, &job);
>>>>>>> upstream/android-13
	if (r)
		return r;

	ib = &job->ibs[0];
<<<<<<< HEAD
	dummy = ib->gpu_addr + 1024;
=======
	addr = amdgpu_bo_gpu_offset(bo);
>>>>>>> upstream/android-13

	ib->length_dw = 0;
	ib->ptr[ib->length_dw++] = 0x00000018;
	ib->ptr[ib->length_dw++] = 0x00000001; /* session info */
	ib->ptr[ib->length_dw++] = handle;
<<<<<<< HEAD
	ib->ptr[ib->length_dw++] = upper_32_bits(dummy);
	ib->ptr[ib->length_dw++] = dummy;
=======
	ib->ptr[ib->length_dw++] = upper_32_bits(addr);
	ib->ptr[ib->length_dw++] = addr;
>>>>>>> upstream/android-13
	ib->ptr[ib->length_dw++] = 0x0000000b;

	ib->ptr[ib->length_dw++] = 0x00000014;
	ib->ptr[ib->length_dw++] = 0x00000002; /* task info */
	ib->ptr[ib->length_dw++] = 0x0000001c;
	ib->ptr[ib->length_dw++] = 0x00000000;
	ib->ptr[ib->length_dw++] = 0x00000000;

	ib->ptr[ib->length_dw++] = 0x00000008;
	ib->ptr[ib->length_dw++] = 0x08000001; /* op initialize */

	for (i = ib->length_dw; i < ib_size_dw; ++i)
		ib->ptr[i] = 0x0;

	r = amdgpu_job_submit_direct(job, ring, &f);
	if (r)
		goto err;

	if (fence)
		*fence = dma_fence_get(f);
	dma_fence_put(f);

	return 0;

err:
	amdgpu_job_free(job);
	return r;
}

static int amdgpu_vcn_enc_get_destroy_msg(struct amdgpu_ring *ring, uint32_t handle,
<<<<<<< HEAD
				struct dma_fence **fence)
=======
					  struct amdgpu_bo *bo,
					  struct dma_fence **fence)
>>>>>>> upstream/android-13
{
	const unsigned ib_size_dw = 16;
	struct amdgpu_job *job;
	struct amdgpu_ib *ib;
	struct dma_fence *f = NULL;
<<<<<<< HEAD
	uint64_t dummy;
	int i, r;

	r = amdgpu_job_alloc_with_ib(ring->adev, ib_size_dw * 4, &job);
=======
	uint64_t addr;
	int i, r;

	r = amdgpu_job_alloc_with_ib(ring->adev, ib_size_dw * 4,
					AMDGPU_IB_POOL_DIRECT, &job);
>>>>>>> upstream/android-13
	if (r)
		return r;

	ib = &job->ibs[0];
<<<<<<< HEAD
	dummy = ib->gpu_addr + 1024;
=======
	addr = amdgpu_bo_gpu_offset(bo);
>>>>>>> upstream/android-13

	ib->length_dw = 0;
	ib->ptr[ib->length_dw++] = 0x00000018;
	ib->ptr[ib->length_dw++] = 0x00000001;
	ib->ptr[ib->length_dw++] = handle;
<<<<<<< HEAD
	ib->ptr[ib->length_dw++] = upper_32_bits(dummy);
	ib->ptr[ib->length_dw++] = dummy;
=======
	ib->ptr[ib->length_dw++] = upper_32_bits(addr);
	ib->ptr[ib->length_dw++] = addr;
>>>>>>> upstream/android-13
	ib->ptr[ib->length_dw++] = 0x0000000b;

	ib->ptr[ib->length_dw++] = 0x00000014;
	ib->ptr[ib->length_dw++] = 0x00000002;
	ib->ptr[ib->length_dw++] = 0x0000001c;
	ib->ptr[ib->length_dw++] = 0x00000000;
	ib->ptr[ib->length_dw++] = 0x00000000;

	ib->ptr[ib->length_dw++] = 0x00000008;
	ib->ptr[ib->length_dw++] = 0x08000002; /* op close session */

	for (i = ib->length_dw; i < ib_size_dw; ++i)
		ib->ptr[i] = 0x0;

	r = amdgpu_job_submit_direct(job, ring, &f);
	if (r)
		goto err;

	if (fence)
		*fence = dma_fence_get(f);
	dma_fence_put(f);

	return 0;

err:
	amdgpu_job_free(job);
	return r;
}

int amdgpu_vcn_enc_ring_test_ib(struct amdgpu_ring *ring, long timeout)
{
	struct dma_fence *fence = NULL;
<<<<<<< HEAD
	long r;

	r = amdgpu_vcn_enc_get_create_msg(ring, 1, NULL);
	if (r) {
		DRM_ERROR("amdgpu: failed to get create msg (%ld).\n", r);
		goto error;
	}

	r = amdgpu_vcn_enc_get_destroy_msg(ring, 1, &fence);
	if (r) {
		DRM_ERROR("amdgpu: failed to get destroy ib (%ld).\n", r);
		goto error;
	}

	r = dma_fence_wait_timeout(fence, false, timeout);
	if (r == 0) {
		DRM_ERROR("amdgpu: IB test timed out.\n");
		r = -ETIMEDOUT;
	} else if (r < 0) {
		DRM_ERROR("amdgpu: fence wait failed (%ld).\n", r);
	} else {
		DRM_DEBUG("ib test on ring %d succeeded\n", ring->idx);
		r = 0;
	}
error:
	dma_fence_put(fence);
	return r;
}

int amdgpu_vcn_jpeg_ring_test_ring(struct amdgpu_ring *ring)
{
	struct amdgpu_device *adev = ring->adev;
	uint32_t tmp = 0;
	unsigned i;
	int r;

	WREG32(SOC15_REG_OFFSET(UVD, 0, mmUVD_CONTEXT_ID), 0xCAFEDEAD);
	r = amdgpu_ring_alloc(ring, 3);

	if (r) {
		DRM_ERROR("amdgpu: cp failed to lock ring %d (%d).\n",
				  ring->idx, r);
		return r;
	}

	amdgpu_ring_write(ring,
		PACKETJ(SOC15_REG_OFFSET(UVD, 0, mmUVD_CONTEXT_ID), 0, 0, 0));
	amdgpu_ring_write(ring, 0xDEADBEEF);
	amdgpu_ring_commit(ring);

	for (i = 0; i < adev->usec_timeout; i++) {
		tmp = RREG32(SOC15_REG_OFFSET(UVD, 0, mmUVD_CONTEXT_ID));
		if (tmp == 0xDEADBEEF)
			break;
		DRM_UDELAY(1);
	}

	if (i < adev->usec_timeout) {
		DRM_DEBUG("ring test on %d succeeded in %d usecs\n",
				  ring->idx, i);
	} else {
		DRM_ERROR("amdgpu: ring %d test failed (0x%08X)\n",
				  ring->idx, tmp);
		r = -EINVAL;
	}

	return r;
}

static int amdgpu_vcn_jpeg_set_reg(struct amdgpu_ring *ring, uint32_t handle,
		struct dma_fence **fence)
{
	struct amdgpu_device *adev = ring->adev;
	struct amdgpu_job *job;
	struct amdgpu_ib *ib;
	struct dma_fence *f = NULL;
	const unsigned ib_size_dw = 16;
	int i, r;

	r = amdgpu_job_alloc_with_ib(ring->adev, ib_size_dw * 4, &job);
	if (r)
		return r;

	ib = &job->ibs[0];

	ib->ptr[0] = PACKETJ(SOC15_REG_OFFSET(UVD, 0, mmUVD_JPEG_PITCH), 0, 0, PACKETJ_TYPE0);
	ib->ptr[1] = 0xDEADBEEF;
	for (i = 2; i < 16; i += 2) {
		ib->ptr[i] = PACKETJ(0, 0, 0, PACKETJ_TYPE6);
		ib->ptr[i+1] = 0;
	}
	ib->length_dw = 16;

	r = amdgpu_job_submit_direct(job, ring, &f);
	if (r)
		goto err;

	if (fence)
		*fence = dma_fence_get(f);
	dma_fence_put(f);

	return 0;

err:
	amdgpu_job_free(job);
	return r;
}

int amdgpu_vcn_jpeg_ring_test_ib(struct amdgpu_ring *ring, long timeout)
{
	struct amdgpu_device *adev = ring->adev;
	uint32_t tmp = 0;
	unsigned i;
	struct dma_fence *fence = NULL;
	long r = 0;

	r = amdgpu_vcn_jpeg_set_reg(ring, 1, &fence);
	if (r) {
		DRM_ERROR("amdgpu: failed to set jpeg register (%ld).\n", r);
		goto error;
	}

	r = dma_fence_wait_timeout(fence, false, timeout);
	if (r == 0) {
		DRM_ERROR("amdgpu: IB test timed out.\n");
		r = -ETIMEDOUT;
		goto error;
	} else if (r < 0) {
		DRM_ERROR("amdgpu: fence wait failed (%ld).\n", r);
		goto error;
	} else
		r = 0;

	for (i = 0; i < adev->usec_timeout; i++) {
		tmp = RREG32(SOC15_REG_OFFSET(UVD, 0, mmUVD_JPEG_PITCH));
		if (tmp == 0xDEADBEEF)
			break;
		DRM_UDELAY(1);
	}

	if (i < adev->usec_timeout)
		DRM_DEBUG("ib test on ring %d succeeded\n", ring->idx);
	else {
		DRM_ERROR("ib test failed (0x%08X)\n", tmp);
		r = -EINVAL;
	}

	dma_fence_put(fence);

error:
=======
	struct amdgpu_bo *bo = NULL;
	long r;

	r = amdgpu_bo_create_reserved(ring->adev, 128 * 1024, PAGE_SIZE,
				      AMDGPU_GEM_DOMAIN_VRAM,
				      &bo, NULL, NULL);
	if (r)
		return r;

	r = amdgpu_vcn_enc_get_create_msg(ring, 1, bo, NULL);
	if (r)
		goto error;

	r = amdgpu_vcn_enc_get_destroy_msg(ring, 1, bo, &fence);
	if (r)
		goto error;

	r = dma_fence_wait_timeout(fence, false, timeout);
	if (r == 0)
		r = -ETIMEDOUT;
	else if (r > 0)
		r = 0;

error:
	dma_fence_put(fence);
	amdgpu_bo_unreserve(bo);
	amdgpu_bo_free_kernel(&bo, NULL, NULL);

>>>>>>> upstream/android-13
	return r;
}
