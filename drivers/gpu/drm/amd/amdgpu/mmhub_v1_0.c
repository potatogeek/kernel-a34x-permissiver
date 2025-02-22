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
#include "amdgpu.h"
<<<<<<< HEAD
=======
#include "amdgpu_ras.h"
>>>>>>> upstream/android-13
#include "mmhub_v1_0.h"

#include "mmhub/mmhub_1_0_offset.h"
#include "mmhub/mmhub_1_0_sh_mask.h"
#include "mmhub/mmhub_1_0_default.h"
<<<<<<< HEAD
#include "athub/athub_1_0_offset.h"
#include "athub/athub_1_0_sh_mask.h"
#include "vega10_enum.h"

=======
#include "vega10_enum.h"
#include "soc15.h"
>>>>>>> upstream/android-13
#include "soc15_common.h"

#define mmDAGB0_CNTL_MISC2_RV 0x008f
#define mmDAGB0_CNTL_MISC2_RV_BASE_IDX 0

<<<<<<< HEAD
u64 mmhub_v1_0_get_fb_location(struct amdgpu_device *adev)
{
	u64 base = RREG32_SOC15(MMHUB, 0, mmMC_VM_FB_LOCATION_BASE);
=======
static u64 mmhub_v1_0_get_fb_location(struct amdgpu_device *adev)
{
	u64 base = RREG32_SOC15(MMHUB, 0, mmMC_VM_FB_LOCATION_BASE);
	u64 top = RREG32_SOC15(MMHUB, 0, mmMC_VM_FB_LOCATION_TOP);
>>>>>>> upstream/android-13

	base &= MC_VM_FB_LOCATION_BASE__FB_BASE_MASK;
	base <<= 24;

<<<<<<< HEAD
	return base;
}

static void mmhub_v1_0_init_gart_pt_regs(struct amdgpu_device *adev)
{
	uint64_t value;

	BUG_ON(adev->gart.table_addr & (~0x0000FFFFFFFFF000ULL));
	value = adev->gart.table_addr - adev->gmc.vram_start +
		adev->vm_manager.vram_base_offset;
	value &= 0x0000FFFFFFFFF000ULL;
	value |= 0x1; /* valid bit */

	WREG32_SOC15(MMHUB, 0, mmVM_CONTEXT0_PAGE_TABLE_BASE_ADDR_LO32,
		     lower_32_bits(value));

	WREG32_SOC15(MMHUB, 0, mmVM_CONTEXT0_PAGE_TABLE_BASE_ADDR_HI32,
		     upper_32_bits(value));
=======
	top &= MC_VM_FB_LOCATION_TOP__FB_TOP_MASK;
	top <<= 24;

	adev->gmc.fb_start = base;
	adev->gmc.fb_end = top;

	return base;
}

static void mmhub_v1_0_setup_vm_pt_regs(struct amdgpu_device *adev, uint32_t vmid,
				uint64_t page_table_base)
{
	struct amdgpu_vmhub *hub = &adev->vmhub[AMDGPU_MMHUB_0];

	WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT0_PAGE_TABLE_BASE_ADDR_LO32,
			    hub->ctx_addr_distance * vmid,
			    lower_32_bits(page_table_base));

	WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT0_PAGE_TABLE_BASE_ADDR_HI32,
			    hub->ctx_addr_distance * vmid,
			    upper_32_bits(page_table_base));
>>>>>>> upstream/android-13
}

static void mmhub_v1_0_init_gart_aperture_regs(struct amdgpu_device *adev)
{
<<<<<<< HEAD
	mmhub_v1_0_init_gart_pt_regs(adev);
=======
	uint64_t pt_base = amdgpu_gmc_pd_addr(adev->gart.bo);

	mmhub_v1_0_setup_vm_pt_regs(adev, 0, pt_base);
>>>>>>> upstream/android-13

	WREG32_SOC15(MMHUB, 0, mmVM_CONTEXT0_PAGE_TABLE_START_ADDR_LO32,
		     (u32)(adev->gmc.gart_start >> 12));
	WREG32_SOC15(MMHUB, 0, mmVM_CONTEXT0_PAGE_TABLE_START_ADDR_HI32,
		     (u32)(adev->gmc.gart_start >> 44));

	WREG32_SOC15(MMHUB, 0, mmVM_CONTEXT0_PAGE_TABLE_END_ADDR_LO32,
		     (u32)(adev->gmc.gart_end >> 12));
	WREG32_SOC15(MMHUB, 0, mmVM_CONTEXT0_PAGE_TABLE_END_ADDR_HI32,
		     (u32)(adev->gmc.gart_end >> 44));
}

static void mmhub_v1_0_init_system_aperture_regs(struct amdgpu_device *adev)
{
	uint64_t value;
	uint32_t tmp;

<<<<<<< HEAD
	/* Disable AGP. */
	WREG32_SOC15(MMHUB, 0, mmMC_VM_AGP_BASE, 0);
	WREG32_SOC15(MMHUB, 0, mmMC_VM_AGP_TOP, 0);
	WREG32_SOC15(MMHUB, 0, mmMC_VM_AGP_BOT, 0x00FFFFFF);

	/* Program the system aperture low logical page number. */
	WREG32_SOC15(MMHUB, 0, mmMC_VM_SYSTEM_APERTURE_LOW_ADDR,
		     adev->gmc.vram_start >> 18);
	WREG32_SOC15(MMHUB, 0, mmMC_VM_SYSTEM_APERTURE_HIGH_ADDR,
		     adev->gmc.vram_end >> 18);

	/* Set default page address. */
	value = adev->vram_scratch.gpu_addr - adev->gmc.vram_start +
		adev->vm_manager.vram_base_offset;
=======
	/* Program the AGP BAR */
	WREG32_SOC15(MMHUB, 0, mmMC_VM_AGP_BASE, 0);
	WREG32_SOC15(MMHUB, 0, mmMC_VM_AGP_BOT, adev->gmc.agp_start >> 24);
	WREG32_SOC15(MMHUB, 0, mmMC_VM_AGP_TOP, adev->gmc.agp_end >> 24);

	/* Program the system aperture low logical page number. */
	WREG32_SOC15(MMHUB, 0, mmMC_VM_SYSTEM_APERTURE_LOW_ADDR,
		     min(adev->gmc.fb_start, adev->gmc.agp_start) >> 18);

	if (adev->apu_flags & AMD_APU_IS_RAVEN2)
		/*
		 * Raven2 has a HW issue that it is unable to use the vram which
		 * is out of MC_VM_SYSTEM_APERTURE_HIGH_ADDR. So here is the
		 * workaround that increase system aperture high address (add 1)
		 * to get rid of the VM fault and hardware hang.
		 */
		WREG32_SOC15(MMHUB, 0, mmMC_VM_SYSTEM_APERTURE_HIGH_ADDR,
			     max((adev->gmc.fb_end >> 18) + 0x1,
				 adev->gmc.agp_end >> 18));
	else
		WREG32_SOC15(MMHUB, 0, mmMC_VM_SYSTEM_APERTURE_HIGH_ADDR,
			     max(adev->gmc.fb_end, adev->gmc.agp_end) >> 18);

	if (amdgpu_sriov_vf(adev))
		return;

	/* Set default page address. */
	value = amdgpu_gmc_vram_mc2pa(adev, adev->vram_scratch.gpu_addr);
>>>>>>> upstream/android-13
	WREG32_SOC15(MMHUB, 0, mmMC_VM_SYSTEM_APERTURE_DEFAULT_ADDR_LSB,
		     (u32)(value >> 12));
	WREG32_SOC15(MMHUB, 0, mmMC_VM_SYSTEM_APERTURE_DEFAULT_ADDR_MSB,
		     (u32)(value >> 44));

	/* Program "protection fault". */
	WREG32_SOC15(MMHUB, 0, mmVM_L2_PROTECTION_FAULT_DEFAULT_ADDR_LO32,
		     (u32)(adev->dummy_page_addr >> 12));
	WREG32_SOC15(MMHUB, 0, mmVM_L2_PROTECTION_FAULT_DEFAULT_ADDR_HI32,
		     (u32)((u64)adev->dummy_page_addr >> 44));

	tmp = RREG32_SOC15(MMHUB, 0, mmVM_L2_PROTECTION_FAULT_CNTL2);
	tmp = REG_SET_FIELD(tmp, VM_L2_PROTECTION_FAULT_CNTL2,
			    ACTIVE_PAGE_MIGRATION_PTE_READ_RETRY, 1);
	WREG32_SOC15(MMHUB, 0, mmVM_L2_PROTECTION_FAULT_CNTL2, tmp);
}

static void mmhub_v1_0_init_tlb_regs(struct amdgpu_device *adev)
{
	uint32_t tmp;

	/* Setup TLB control */
	tmp = RREG32_SOC15(MMHUB, 0, mmMC_VM_MX_L1_TLB_CNTL);

	tmp = REG_SET_FIELD(tmp, MC_VM_MX_L1_TLB_CNTL, ENABLE_L1_TLB, 1);
	tmp = REG_SET_FIELD(tmp, MC_VM_MX_L1_TLB_CNTL, SYSTEM_ACCESS_MODE, 3);
	tmp = REG_SET_FIELD(tmp, MC_VM_MX_L1_TLB_CNTL,
			    ENABLE_ADVANCED_DRIVER_MODEL, 1);
	tmp = REG_SET_FIELD(tmp, MC_VM_MX_L1_TLB_CNTL,
			    SYSTEM_APERTURE_UNMAPPED_ACCESS, 0);
<<<<<<< HEAD
	tmp = REG_SET_FIELD(tmp, MC_VM_MX_L1_TLB_CNTL, ECO_BITS, 0);
=======
>>>>>>> upstream/android-13
	tmp = REG_SET_FIELD(tmp, MC_VM_MX_L1_TLB_CNTL,
			    MTYPE, MTYPE_UC);/* XXX for emulation. */
	tmp = REG_SET_FIELD(tmp, MC_VM_MX_L1_TLB_CNTL, ATC_EN, 1);

	WREG32_SOC15(MMHUB, 0, mmMC_VM_MX_L1_TLB_CNTL, tmp);
}

static void mmhub_v1_0_init_cache_regs(struct amdgpu_device *adev)
{
	uint32_t tmp;

<<<<<<< HEAD
=======
	if (amdgpu_sriov_vf(adev))
		return;

>>>>>>> upstream/android-13
	/* Setup L2 cache */
	tmp = RREG32_SOC15(MMHUB, 0, mmVM_L2_CNTL);
	tmp = REG_SET_FIELD(tmp, VM_L2_CNTL, ENABLE_L2_CACHE, 1);
	tmp = REG_SET_FIELD(tmp, VM_L2_CNTL, ENABLE_L2_FRAGMENT_PROCESSING, 1);
	/* XXX for emulation, Refer to closed source code.*/
	tmp = REG_SET_FIELD(tmp, VM_L2_CNTL, L2_PDE0_CACHE_TAG_GENERATION_MODE,
			    0);
<<<<<<< HEAD
	tmp = REG_SET_FIELD(tmp, VM_L2_CNTL, PDE_FAULT_CLASSIFICATION, 1);
=======
	tmp = REG_SET_FIELD(tmp, VM_L2_CNTL, PDE_FAULT_CLASSIFICATION, 0);
>>>>>>> upstream/android-13
	tmp = REG_SET_FIELD(tmp, VM_L2_CNTL, CONTEXT1_IDENTITY_ACCESS_MODE, 1);
	tmp = REG_SET_FIELD(tmp, VM_L2_CNTL, IDENTITY_MODE_FRAGMENT_SIZE, 0);
	WREG32_SOC15(MMHUB, 0, mmVM_L2_CNTL, tmp);

	tmp = RREG32_SOC15(MMHUB, 0, mmVM_L2_CNTL2);
	tmp = REG_SET_FIELD(tmp, VM_L2_CNTL2, INVALIDATE_ALL_L1_TLBS, 1);
	tmp = REG_SET_FIELD(tmp, VM_L2_CNTL2, INVALIDATE_L2_CACHE, 1);
	WREG32_SOC15(MMHUB, 0, mmVM_L2_CNTL2, tmp);

	if (adev->gmc.translate_further) {
		tmp = REG_SET_FIELD(tmp, VM_L2_CNTL3, BANK_SELECT, 12);
		tmp = REG_SET_FIELD(tmp, VM_L2_CNTL3,
				    L2_CACHE_BIGK_FRAGMENT_SIZE, 9);
	} else {
		tmp = REG_SET_FIELD(tmp, VM_L2_CNTL3, BANK_SELECT, 9);
		tmp = REG_SET_FIELD(tmp, VM_L2_CNTL3,
				    L2_CACHE_BIGK_FRAGMENT_SIZE, 6);
	}
	WREG32_SOC15(MMHUB, 0, mmVM_L2_CNTL3, tmp);

	tmp = mmVM_L2_CNTL4_DEFAULT;
	tmp = REG_SET_FIELD(tmp, VM_L2_CNTL4, VMC_TAP_PDE_REQUEST_PHYSICAL, 0);
	tmp = REG_SET_FIELD(tmp, VM_L2_CNTL4, VMC_TAP_PTE_REQUEST_PHYSICAL, 0);
	WREG32_SOC15(MMHUB, 0, mmVM_L2_CNTL4, tmp);
}

static void mmhub_v1_0_enable_system_domain(struct amdgpu_device *adev)
{
	uint32_t tmp;

	tmp = RREG32_SOC15(MMHUB, 0, mmVM_CONTEXT0_CNTL);
	tmp = REG_SET_FIELD(tmp, VM_CONTEXT0_CNTL, ENABLE_CONTEXT, 1);
	tmp = REG_SET_FIELD(tmp, VM_CONTEXT0_CNTL, PAGE_TABLE_DEPTH, 0);
<<<<<<< HEAD
=======
	tmp = REG_SET_FIELD(tmp, VM_CONTEXT0_CNTL,
			    RETRY_PERMISSION_OR_INVALID_PAGE_FAULT, 0);
>>>>>>> upstream/android-13
	WREG32_SOC15(MMHUB, 0, mmVM_CONTEXT0_CNTL, tmp);
}

static void mmhub_v1_0_disable_identity_aperture(struct amdgpu_device *adev)
{
<<<<<<< HEAD
=======
	if (amdgpu_sriov_vf(adev))
		return;

>>>>>>> upstream/android-13
	WREG32_SOC15(MMHUB, 0, mmVM_L2_CONTEXT1_IDENTITY_APERTURE_LOW_ADDR_LO32,
		     0XFFFFFFFF);
	WREG32_SOC15(MMHUB, 0, mmVM_L2_CONTEXT1_IDENTITY_APERTURE_LOW_ADDR_HI32,
		     0x0000000F);

	WREG32_SOC15(MMHUB, 0,
		     mmVM_L2_CONTEXT1_IDENTITY_APERTURE_HIGH_ADDR_LO32, 0);
	WREG32_SOC15(MMHUB, 0,
		     mmVM_L2_CONTEXT1_IDENTITY_APERTURE_HIGH_ADDR_HI32, 0);

	WREG32_SOC15(MMHUB, 0, mmVM_L2_CONTEXT_IDENTITY_PHYSICAL_OFFSET_LO32,
		     0);
	WREG32_SOC15(MMHUB, 0, mmVM_L2_CONTEXT_IDENTITY_PHYSICAL_OFFSET_HI32,
		     0);
}

static void mmhub_v1_0_setup_vmid_config(struct amdgpu_device *adev)
{
<<<<<<< HEAD
=======
	struct amdgpu_vmhub *hub = &adev->vmhub[AMDGPU_MMHUB_0];
>>>>>>> upstream/android-13
	unsigned num_level, block_size;
	uint32_t tmp;
	int i;

	num_level = adev->vm_manager.num_level;
	block_size = adev->vm_manager.block_size;
	if (adev->gmc.translate_further)
		num_level -= 1;
	else
		block_size -= 9;

	for (i = 0; i <= 14; i++) {
		tmp = RREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT1_CNTL, i);
		tmp = REG_SET_FIELD(tmp, VM_CONTEXT1_CNTL, ENABLE_CONTEXT, 1);
		tmp = REG_SET_FIELD(tmp, VM_CONTEXT1_CNTL, PAGE_TABLE_DEPTH,
				    num_level);
		tmp = REG_SET_FIELD(tmp, VM_CONTEXT1_CNTL,
				    RANGE_PROTECTION_FAULT_ENABLE_DEFAULT, 1);
		tmp = REG_SET_FIELD(tmp, VM_CONTEXT1_CNTL,
				    DUMMY_PAGE_PROTECTION_FAULT_ENABLE_DEFAULT,
				    1);
		tmp = REG_SET_FIELD(tmp, VM_CONTEXT1_CNTL,
				    PDE0_PROTECTION_FAULT_ENABLE_DEFAULT, 1);
		tmp = REG_SET_FIELD(tmp, VM_CONTEXT1_CNTL,
				    VALID_PROTECTION_FAULT_ENABLE_DEFAULT, 1);
		tmp = REG_SET_FIELD(tmp, VM_CONTEXT1_CNTL,
				    READ_PROTECTION_FAULT_ENABLE_DEFAULT, 1);
		tmp = REG_SET_FIELD(tmp, VM_CONTEXT1_CNTL,
				    WRITE_PROTECTION_FAULT_ENABLE_DEFAULT, 1);
		tmp = REG_SET_FIELD(tmp, VM_CONTEXT1_CNTL,
				    EXECUTE_PROTECTION_FAULT_ENABLE_DEFAULT, 1);
		tmp = REG_SET_FIELD(tmp, VM_CONTEXT1_CNTL,
				    PAGE_TABLE_BLOCK_SIZE,
				    block_size);
		/* Send no-retry XNACK on fault to suppress VM fault storm. */
		tmp = REG_SET_FIELD(tmp, VM_CONTEXT1_CNTL,
<<<<<<< HEAD
				    RETRY_PERMISSION_OR_INVALID_PAGE_FAULT, 0);
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT1_CNTL, i, tmp);
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT1_PAGE_TABLE_START_ADDR_LO32, i*2, 0);
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT1_PAGE_TABLE_START_ADDR_HI32, i*2, 0);
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT1_PAGE_TABLE_END_ADDR_LO32, i*2,
			lower_32_bits(adev->vm_manager.max_pfn - 1));
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT1_PAGE_TABLE_END_ADDR_HI32, i*2,
			upper_32_bits(adev->vm_manager.max_pfn - 1));
=======
				    RETRY_PERMISSION_OR_INVALID_PAGE_FAULT,
				    !adev->gmc.noretry);
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT1_CNTL,
				    i * hub->ctx_distance, tmp);
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT1_PAGE_TABLE_START_ADDR_LO32,
				    i * hub->ctx_addr_distance, 0);
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT1_PAGE_TABLE_START_ADDR_HI32,
				    i * hub->ctx_addr_distance, 0);
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT1_PAGE_TABLE_END_ADDR_LO32,
				    i * hub->ctx_addr_distance,
				    lower_32_bits(adev->vm_manager.max_pfn - 1));
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT1_PAGE_TABLE_END_ADDR_HI32,
				    i * hub->ctx_addr_distance,
				    upper_32_bits(adev->vm_manager.max_pfn - 1));
>>>>>>> upstream/android-13
	}
}

static void mmhub_v1_0_program_invalidation(struct amdgpu_device *adev)
{
<<<<<<< HEAD
=======
	struct amdgpu_vmhub *hub = &adev->vmhub[AMDGPU_MMHUB_0];
>>>>>>> upstream/android-13
	unsigned i;

	for (i = 0; i < 18; ++i) {
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_INVALIDATE_ENG0_ADDR_RANGE_LO32,
<<<<<<< HEAD
				    2 * i, 0xffffffff);
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_INVALIDATE_ENG0_ADDR_RANGE_HI32,
				    2 * i, 0x1f);
	}
}

struct pctl_data {
	uint32_t index;
	uint32_t data;
};

static const struct pctl_data pctl0_data[] = {
	{0x0, 0x7a640},
	{0x9, 0x2a64a},
	{0xd, 0x2a680},
	{0x11, 0x6a684},
	{0x19, 0xea68e},
	{0x29, 0xa69e},
	{0x2b, 0x0010a6c0},
	{0x3d, 0x83a707},
	{0xc2, 0x8a7a4},
	{0xcc, 0x1a7b8},
	{0xcf, 0xfa7cc},
	{0xe0, 0x17a7dd},
	{0xf9, 0xa7dc},
	{0xfb, 0x12a7f5},
	{0x10f, 0xa808},
	{0x111, 0x12a810},
	{0x125, 0x7a82c}
};
#define PCTL0_DATA_LEN (ARRAY_SIZE(pctl0_data))

#define PCTL0_RENG_EXEC_END_PTR 0x12d
#define PCTL0_STCTRL_REG_SAVE_RANGE0_BASE  0xa640
#define PCTL0_STCTRL_REG_SAVE_RANGE0_LIMIT 0xa833

static const struct pctl_data pctl1_data[] = {
	{0x0, 0x39a000},
	{0x3b, 0x44a040},
	{0x81, 0x2a08d},
	{0x85, 0x6ba094},
	{0xf2, 0x18a100},
	{0x10c, 0x4a132},
	{0x112, 0xca141},
	{0x120, 0x2fa158},
	{0x151, 0x17a1d0},
	{0x16a, 0x1a1e9},
	{0x16d, 0x13a1ec},
	{0x182, 0x7a201},
	{0x18b, 0x3a20a},
	{0x190, 0x7a580},
	{0x199, 0xa590},
	{0x19b, 0x4a594},
	{0x1a1, 0x1a59c},
	{0x1a4, 0x7a82c},
	{0x1ad, 0xfa7cc},
	{0x1be, 0x17a7dd},
	{0x1d7, 0x12a810},
	{0x1eb, 0x4000a7e1},
	{0x1ec, 0x5000a7f5},
	{0x1ed, 0x4000a7e2},
	{0x1ee, 0x5000a7dc},
	{0x1ef, 0x4000a7e3},
	{0x1f0, 0x5000a7f6},
	{0x1f1, 0x5000a7e4}
};
#define PCTL1_DATA_LEN (ARRAY_SIZE(pctl1_data))

#define PCTL1_RENG_EXEC_END_PTR 0x1f1
#define PCTL1_STCTRL_REG_SAVE_RANGE0_BASE  0xa000
#define PCTL1_STCTRL_REG_SAVE_RANGE0_LIMIT 0xa20d
#define PCTL1_STCTRL_REG_SAVE_RANGE1_BASE  0xa580
#define PCTL1_STCTRL_REG_SAVE_RANGE1_LIMIT 0xa59d
#define PCTL1_STCTRL_REG_SAVE_RANGE2_BASE  0xa82c
#define PCTL1_STCTRL_REG_SAVE_RANGE2_LIMIT 0xa833

static void mmhub_v1_0_power_gating_write_save_ranges(struct amdgpu_device *adev)
{
	uint32_t tmp = 0;

	/* PCTL0_STCTRL_REGISTER_SAVE_RANGE0 */
	tmp = REG_SET_FIELD(tmp, PCTL0_STCTRL_REGISTER_SAVE_RANGE0,
			STCTRL_REGISTER_SAVE_BASE,
			PCTL0_STCTRL_REG_SAVE_RANGE0_BASE);
	tmp = REG_SET_FIELD(tmp, PCTL0_STCTRL_REGISTER_SAVE_RANGE0,
			STCTRL_REGISTER_SAVE_LIMIT,
			PCTL0_STCTRL_REG_SAVE_RANGE0_LIMIT);
	WREG32_SOC15(MMHUB, 0, mmPCTL0_STCTRL_REGISTER_SAVE_RANGE0, tmp);

	/* PCTL1_STCTRL_REGISTER_SAVE_RANGE0 */
	tmp = 0;
	tmp = REG_SET_FIELD(tmp, PCTL1_STCTRL_REGISTER_SAVE_RANGE0,
			STCTRL_REGISTER_SAVE_BASE,
			PCTL1_STCTRL_REG_SAVE_RANGE0_BASE);
	tmp = REG_SET_FIELD(tmp, PCTL1_STCTRL_REGISTER_SAVE_RANGE0,
			STCTRL_REGISTER_SAVE_LIMIT,
			PCTL1_STCTRL_REG_SAVE_RANGE0_LIMIT);
	WREG32_SOC15(MMHUB, 0, mmPCTL1_STCTRL_REGISTER_SAVE_RANGE0, tmp);

	/* PCTL1_STCTRL_REGISTER_SAVE_RANGE1 */
	tmp = 0;
	tmp = REG_SET_FIELD(tmp, PCTL1_STCTRL_REGISTER_SAVE_RANGE1,
			STCTRL_REGISTER_SAVE_BASE,
			PCTL1_STCTRL_REG_SAVE_RANGE1_BASE);
	tmp = REG_SET_FIELD(tmp, PCTL1_STCTRL_REGISTER_SAVE_RANGE1,
			STCTRL_REGISTER_SAVE_LIMIT,
			PCTL1_STCTRL_REG_SAVE_RANGE1_LIMIT);
	WREG32_SOC15(MMHUB, 0, mmPCTL1_STCTRL_REGISTER_SAVE_RANGE1, tmp);

	/* PCTL1_STCTRL_REGISTER_SAVE_RANGE2 */
	tmp = 0;
	tmp = REG_SET_FIELD(tmp, PCTL1_STCTRL_REGISTER_SAVE_RANGE2,
			STCTRL_REGISTER_SAVE_BASE,
			PCTL1_STCTRL_REG_SAVE_RANGE2_BASE);
	tmp = REG_SET_FIELD(tmp, PCTL1_STCTRL_REGISTER_SAVE_RANGE2,
			STCTRL_REGISTER_SAVE_LIMIT,
			PCTL1_STCTRL_REG_SAVE_RANGE2_LIMIT);
	WREG32_SOC15(MMHUB, 0, mmPCTL1_STCTRL_REGISTER_SAVE_RANGE2, tmp);
}

void mmhub_v1_0_initialize_power_gating(struct amdgpu_device *adev)
{
	uint32_t pctl0_misc = 0;
	uint32_t pctl0_reng_execute = 0;
	uint32_t pctl1_misc = 0;
	uint32_t pctl1_reng_execute = 0;
	int i = 0;

	if (amdgpu_sriov_vf(adev))
		return;

	/****************** pctl0 **********************/
	pctl0_misc = RREG32_SOC15(MMHUB, 0, mmPCTL0_MISC);
	pctl0_reng_execute = RREG32_SOC15(MMHUB, 0, mmPCTL0_RENG_EXECUTE);

	/* Light sleep must be disabled before writing to pctl0 registers */
	pctl0_misc &= ~PCTL0_MISC__RENG_MEM_LS_ENABLE_MASK;
	WREG32_SOC15(MMHUB, 0, mmPCTL0_MISC, pctl0_misc);

	/* Write data used to access ram of register engine */
	for (i = 0; i < PCTL0_DATA_LEN; i++) {
                WREG32_SOC15(MMHUB, 0, mmPCTL0_RENG_RAM_INDEX,
			pctl0_data[i].index);
                WREG32_SOC15(MMHUB, 0, mmPCTL0_RENG_RAM_DATA,
			pctl0_data[i].data);
        }

	/* Re-enable light sleep */
	pctl0_misc |= PCTL0_MISC__RENG_MEM_LS_ENABLE_MASK;
	WREG32_SOC15(MMHUB, 0, mmPCTL0_MISC, pctl0_misc);

	/****************** pctl1 **********************/
	pctl1_misc = RREG32_SOC15(MMHUB, 0, mmPCTL1_MISC);
	pctl1_reng_execute = RREG32_SOC15(MMHUB, 0, mmPCTL1_RENG_EXECUTE);

	/* Light sleep must be disabled before writing to pctl1 registers */
	pctl1_misc &= ~PCTL1_MISC__RENG_MEM_LS_ENABLE_MASK;
	WREG32_SOC15(MMHUB, 0, mmPCTL1_MISC, pctl1_misc);

	/* Write data used to access ram of register engine */
	for (i = 0; i < PCTL1_DATA_LEN; i++) {
                WREG32_SOC15(MMHUB, 0, mmPCTL1_RENG_RAM_INDEX,
			pctl1_data[i].index);
                WREG32_SOC15(MMHUB, 0, mmPCTL1_RENG_RAM_DATA,
			pctl1_data[i].data);
        }

	/* Re-enable light sleep */
	pctl1_misc |= PCTL1_MISC__RENG_MEM_LS_ENABLE_MASK;
	WREG32_SOC15(MMHUB, 0, mmPCTL1_MISC, pctl1_misc);

	mmhub_v1_0_power_gating_write_save_ranges(adev);

	/* Set the reng execute end ptr for pctl0 */
	pctl0_reng_execute = REG_SET_FIELD(pctl0_reng_execute,
					PCTL0_RENG_EXECUTE,
					RENG_EXECUTE_END_PTR,
					PCTL0_RENG_EXEC_END_PTR);
	WREG32_SOC15(MMHUB, 0, mmPCTL0_RENG_EXECUTE, pctl0_reng_execute);

	/* Set the reng execute end ptr for pctl1 */
	pctl1_reng_execute = REG_SET_FIELD(pctl1_reng_execute,
					PCTL1_RENG_EXECUTE,
					RENG_EXECUTE_END_PTR,
					PCTL1_RENG_EXEC_END_PTR);
	WREG32_SOC15(MMHUB, 0, mmPCTL1_RENG_EXECUTE, pctl1_reng_execute);
}

void mmhub_v1_0_update_power_gating(struct amdgpu_device *adev,
				bool enable)
{
	uint32_t pctl0_reng_execute = 0;
	uint32_t pctl1_reng_execute = 0;

	if (amdgpu_sriov_vf(adev))
		return;

	pctl0_reng_execute = RREG32_SOC15(MMHUB, 0, mmPCTL0_RENG_EXECUTE);
	pctl1_reng_execute = RREG32_SOC15(MMHUB, 0, mmPCTL1_RENG_EXECUTE);

	if (enable && adev->pg_flags & AMD_PG_SUPPORT_MMHUB) {
		pctl0_reng_execute = REG_SET_FIELD(pctl0_reng_execute,
						PCTL0_RENG_EXECUTE,
						RENG_EXECUTE_ON_PWR_UP, 1);
		pctl0_reng_execute = REG_SET_FIELD(pctl0_reng_execute,
						PCTL0_RENG_EXECUTE,
						RENG_EXECUTE_ON_REG_UPDATE, 1);
		WREG32_SOC15(MMHUB, 0, mmPCTL0_RENG_EXECUTE, pctl0_reng_execute);

		pctl1_reng_execute = REG_SET_FIELD(pctl1_reng_execute,
						PCTL1_RENG_EXECUTE,
						RENG_EXECUTE_ON_PWR_UP, 1);
		pctl1_reng_execute = REG_SET_FIELD(pctl1_reng_execute,
						PCTL1_RENG_EXECUTE,
						RENG_EXECUTE_ON_REG_UPDATE, 1);
		WREG32_SOC15(MMHUB, 0, mmPCTL1_RENG_EXECUTE, pctl1_reng_execute);

		if (adev->powerplay.pp_funcs->set_powergating_by_smu)
			amdgpu_dpm_set_powergating_by_smu(adev, AMD_IP_BLOCK_TYPE_GMC, true);

	} else {
		pctl0_reng_execute = REG_SET_FIELD(pctl0_reng_execute,
						PCTL0_RENG_EXECUTE,
						RENG_EXECUTE_ON_PWR_UP, 0);
		pctl0_reng_execute = REG_SET_FIELD(pctl0_reng_execute,
						PCTL0_RENG_EXECUTE,
						RENG_EXECUTE_ON_REG_UPDATE, 0);
		WREG32_SOC15(MMHUB, 0, mmPCTL0_RENG_EXECUTE, pctl0_reng_execute);

		pctl1_reng_execute = REG_SET_FIELD(pctl1_reng_execute,
						PCTL1_RENG_EXECUTE,
						RENG_EXECUTE_ON_PWR_UP, 0);
		pctl1_reng_execute = REG_SET_FIELD(pctl1_reng_execute,
						PCTL1_RENG_EXECUTE,
						RENG_EXECUTE_ON_REG_UPDATE, 0);
		WREG32_SOC15(MMHUB, 0, mmPCTL1_RENG_EXECUTE, pctl1_reng_execute);
	}
}

int mmhub_v1_0_gart_enable(struct amdgpu_device *adev)
=======
				    i * hub->eng_addr_distance, 0xffffffff);
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_INVALIDATE_ENG0_ADDR_RANGE_HI32,
				    i * hub->eng_addr_distance, 0x1f);
	}
}

static void mmhub_v1_0_update_power_gating(struct amdgpu_device *adev,
				bool enable)
{
	if (amdgpu_sriov_vf(adev))
		return;

	if (enable && adev->pg_flags & AMD_PG_SUPPORT_MMHUB) {
		amdgpu_dpm_set_powergating_by_smu(adev, AMD_IP_BLOCK_TYPE_GMC, true);

	}
}

static int mmhub_v1_0_gart_enable(struct amdgpu_device *adev)
>>>>>>> upstream/android-13
{
	if (amdgpu_sriov_vf(adev)) {
		/*
		 * MC_VM_FB_LOCATION_BASE/TOP is NULL for VF, becuase they are
		 * VF copy registers so vbios post doesn't program them, for
		 * SRIOV driver need to program them
		 */
		WREG32_SOC15(MMHUB, 0, mmMC_VM_FB_LOCATION_BASE,
			     adev->gmc.vram_start >> 24);
		WREG32_SOC15(MMHUB, 0, mmMC_VM_FB_LOCATION_TOP,
			     adev->gmc.vram_end >> 24);
	}

	/* GART Enable. */
	mmhub_v1_0_init_gart_aperture_regs(adev);
	mmhub_v1_0_init_system_aperture_regs(adev);
	mmhub_v1_0_init_tlb_regs(adev);
	mmhub_v1_0_init_cache_regs(adev);

	mmhub_v1_0_enable_system_domain(adev);
	mmhub_v1_0_disable_identity_aperture(adev);
	mmhub_v1_0_setup_vmid_config(adev);
	mmhub_v1_0_program_invalidation(adev);

	return 0;
}

<<<<<<< HEAD
void mmhub_v1_0_gart_disable(struct amdgpu_device *adev)
{
=======
static void mmhub_v1_0_gart_disable(struct amdgpu_device *adev)
{
	struct amdgpu_vmhub *hub = &adev->vmhub[AMDGPU_MMHUB_0];
>>>>>>> upstream/android-13
	u32 tmp;
	u32 i;

	/* Disable all tables */
<<<<<<< HEAD
	for (i = 0; i < 16; i++)
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT0_CNTL, i, 0);
=======
	for (i = 0; i < AMDGPU_NUM_VMID; i++)
		WREG32_SOC15_OFFSET(MMHUB, 0, mmVM_CONTEXT0_CNTL,
				    i * hub->ctx_distance, 0);
>>>>>>> upstream/android-13

	/* Setup TLB control */
	tmp = RREG32_SOC15(MMHUB, 0, mmMC_VM_MX_L1_TLB_CNTL);
	tmp = REG_SET_FIELD(tmp, MC_VM_MX_L1_TLB_CNTL, ENABLE_L1_TLB, 0);
	tmp = REG_SET_FIELD(tmp,
				MC_VM_MX_L1_TLB_CNTL,
				ENABLE_ADVANCED_DRIVER_MODEL,
				0);
	WREG32_SOC15(MMHUB, 0, mmMC_VM_MX_L1_TLB_CNTL, tmp);

<<<<<<< HEAD
	/* Setup L2 cache */
	tmp = RREG32_SOC15(MMHUB, 0, mmVM_L2_CNTL);
	tmp = REG_SET_FIELD(tmp, VM_L2_CNTL, ENABLE_L2_CACHE, 0);
	WREG32_SOC15(MMHUB, 0, mmVM_L2_CNTL, tmp);
	WREG32_SOC15(MMHUB, 0, mmVM_L2_CNTL3, 0);
=======
	if (!amdgpu_sriov_vf(adev)) {
		/* Setup L2 cache */
		tmp = RREG32_SOC15(MMHUB, 0, mmVM_L2_CNTL);
		tmp = REG_SET_FIELD(tmp, VM_L2_CNTL, ENABLE_L2_CACHE, 0);
		WREG32_SOC15(MMHUB, 0, mmVM_L2_CNTL, tmp);
		WREG32_SOC15(MMHUB, 0, mmVM_L2_CNTL3, 0);
	}
>>>>>>> upstream/android-13
}

/**
 * mmhub_v1_0_set_fault_enable_default - update GART/VM fault handling
 *
 * @adev: amdgpu_device pointer
 * @value: true redirects VM faults to the default page
 */
<<<<<<< HEAD
void mmhub_v1_0_set_fault_enable_default(struct amdgpu_device *adev, bool value)
{
	u32 tmp;
=======
static void mmhub_v1_0_set_fault_enable_default(struct amdgpu_device *adev, bool value)
{
	u32 tmp;

	if (amdgpu_sriov_vf(adev))
		return;

>>>>>>> upstream/android-13
	tmp = RREG32_SOC15(MMHUB, 0, mmVM_L2_PROTECTION_FAULT_CNTL);
	tmp = REG_SET_FIELD(tmp, VM_L2_PROTECTION_FAULT_CNTL,
			RANGE_PROTECTION_FAULT_ENABLE_DEFAULT, value);
	tmp = REG_SET_FIELD(tmp, VM_L2_PROTECTION_FAULT_CNTL,
			PDE0_PROTECTION_FAULT_ENABLE_DEFAULT, value);
	tmp = REG_SET_FIELD(tmp, VM_L2_PROTECTION_FAULT_CNTL,
			PDE1_PROTECTION_FAULT_ENABLE_DEFAULT, value);
	tmp = REG_SET_FIELD(tmp, VM_L2_PROTECTION_FAULT_CNTL,
			PDE2_PROTECTION_FAULT_ENABLE_DEFAULT, value);
	tmp = REG_SET_FIELD(tmp,
			VM_L2_PROTECTION_FAULT_CNTL,
			TRANSLATE_FURTHER_PROTECTION_FAULT_ENABLE_DEFAULT,
			value);
	tmp = REG_SET_FIELD(tmp, VM_L2_PROTECTION_FAULT_CNTL,
			NACK_PROTECTION_FAULT_ENABLE_DEFAULT, value);
	tmp = REG_SET_FIELD(tmp, VM_L2_PROTECTION_FAULT_CNTL,
			DUMMY_PAGE_PROTECTION_FAULT_ENABLE_DEFAULT, value);
	tmp = REG_SET_FIELD(tmp, VM_L2_PROTECTION_FAULT_CNTL,
			VALID_PROTECTION_FAULT_ENABLE_DEFAULT, value);
	tmp = REG_SET_FIELD(tmp, VM_L2_PROTECTION_FAULT_CNTL,
			READ_PROTECTION_FAULT_ENABLE_DEFAULT, value);
	tmp = REG_SET_FIELD(tmp, VM_L2_PROTECTION_FAULT_CNTL,
			WRITE_PROTECTION_FAULT_ENABLE_DEFAULT, value);
	tmp = REG_SET_FIELD(tmp, VM_L2_PROTECTION_FAULT_CNTL,
			EXECUTE_PROTECTION_FAULT_ENABLE_DEFAULT, value);
	if (!value) {
		tmp = REG_SET_FIELD(tmp, VM_L2_PROTECTION_FAULT_CNTL,
				CRASH_ON_NO_RETRY_FAULT, 1);
		tmp = REG_SET_FIELD(tmp, VM_L2_PROTECTION_FAULT_CNTL,
				CRASH_ON_RETRY_FAULT, 1);
<<<<<<< HEAD
    }
=======
	}
>>>>>>> upstream/android-13

	WREG32_SOC15(MMHUB, 0, mmVM_L2_PROTECTION_FAULT_CNTL, tmp);
}

<<<<<<< HEAD
void mmhub_v1_0_init(struct amdgpu_device *adev)
{
	struct amdgpu_vmhub *hub = &adev->vmhub[AMDGPU_MMHUB];
=======
static void mmhub_v1_0_init(struct amdgpu_device *adev)
{
	struct amdgpu_vmhub *hub = &adev->vmhub[AMDGPU_MMHUB_0];
>>>>>>> upstream/android-13

	hub->ctx0_ptb_addr_lo32 =
		SOC15_REG_OFFSET(MMHUB, 0,
				 mmVM_CONTEXT0_PAGE_TABLE_BASE_ADDR_LO32);
	hub->ctx0_ptb_addr_hi32 =
		SOC15_REG_OFFSET(MMHUB, 0,
				 mmVM_CONTEXT0_PAGE_TABLE_BASE_ADDR_HI32);
<<<<<<< HEAD
=======
	hub->vm_inv_eng0_sem =
		SOC15_REG_OFFSET(MMHUB, 0, mmVM_INVALIDATE_ENG0_SEM);
>>>>>>> upstream/android-13
	hub->vm_inv_eng0_req =
		SOC15_REG_OFFSET(MMHUB, 0, mmVM_INVALIDATE_ENG0_REQ);
	hub->vm_inv_eng0_ack =
		SOC15_REG_OFFSET(MMHUB, 0, mmVM_INVALIDATE_ENG0_ACK);
	hub->vm_context0_cntl =
		SOC15_REG_OFFSET(MMHUB, 0, mmVM_CONTEXT0_CNTL);
	hub->vm_l2_pro_fault_status =
		SOC15_REG_OFFSET(MMHUB, 0, mmVM_L2_PROTECTION_FAULT_STATUS);
	hub->vm_l2_pro_fault_cntl =
		SOC15_REG_OFFSET(MMHUB, 0, mmVM_L2_PROTECTION_FAULT_CNTL);

<<<<<<< HEAD
=======
	hub->ctx_distance = mmVM_CONTEXT1_CNTL - mmVM_CONTEXT0_CNTL;
	hub->ctx_addr_distance = mmVM_CONTEXT1_PAGE_TABLE_BASE_ADDR_LO32 -
		mmVM_CONTEXT0_PAGE_TABLE_BASE_ADDR_LO32;
	hub->eng_distance = mmVM_INVALIDATE_ENG1_REQ - mmVM_INVALIDATE_ENG0_REQ;
	hub->eng_addr_distance = mmVM_INVALIDATE_ENG1_ADDR_RANGE_LO32 -
		mmVM_INVALIDATE_ENG0_ADDR_RANGE_LO32;
>>>>>>> upstream/android-13
}

static void mmhub_v1_0_update_medium_grain_clock_gating(struct amdgpu_device *adev,
							bool enable)
{
	uint32_t def, data, def1, data1, def2 = 0, data2 = 0;

	def  = data  = RREG32_SOC15(MMHUB, 0, mmATC_L2_MISC_CG);

	if (adev->asic_type != CHIP_RAVEN) {
		def1 = data1 = RREG32_SOC15(MMHUB, 0, mmDAGB0_CNTL_MISC2);
		def2 = data2 = RREG32_SOC15(MMHUB, 0, mmDAGB1_CNTL_MISC2);
	} else
		def1 = data1 = RREG32_SOC15(MMHUB, 0, mmDAGB0_CNTL_MISC2_RV);

	if (enable && (adev->cg_flags & AMD_CG_SUPPORT_MC_MGCG)) {
		data |= ATC_L2_MISC_CG__ENABLE_MASK;

		data1 &= ~(DAGB0_CNTL_MISC2__DISABLE_WRREQ_CG_MASK |
		           DAGB0_CNTL_MISC2__DISABLE_WRRET_CG_MASK |
		           DAGB0_CNTL_MISC2__DISABLE_RDREQ_CG_MASK |
		           DAGB0_CNTL_MISC2__DISABLE_RDRET_CG_MASK |
		           DAGB0_CNTL_MISC2__DISABLE_TLBWR_CG_MASK |
		           DAGB0_CNTL_MISC2__DISABLE_TLBRD_CG_MASK);

		if (adev->asic_type != CHIP_RAVEN)
			data2 &= ~(DAGB1_CNTL_MISC2__DISABLE_WRREQ_CG_MASK |
			           DAGB1_CNTL_MISC2__DISABLE_WRRET_CG_MASK |
			           DAGB1_CNTL_MISC2__DISABLE_RDREQ_CG_MASK |
			           DAGB1_CNTL_MISC2__DISABLE_RDRET_CG_MASK |
			           DAGB1_CNTL_MISC2__DISABLE_TLBWR_CG_MASK |
			           DAGB1_CNTL_MISC2__DISABLE_TLBRD_CG_MASK);
	} else {
		data &= ~ATC_L2_MISC_CG__ENABLE_MASK;

		data1 |= (DAGB0_CNTL_MISC2__DISABLE_WRREQ_CG_MASK |
			  DAGB0_CNTL_MISC2__DISABLE_WRRET_CG_MASK |
			  DAGB0_CNTL_MISC2__DISABLE_RDREQ_CG_MASK |
			  DAGB0_CNTL_MISC2__DISABLE_RDRET_CG_MASK |
			  DAGB0_CNTL_MISC2__DISABLE_TLBWR_CG_MASK |
			  DAGB0_CNTL_MISC2__DISABLE_TLBRD_CG_MASK);

		if (adev->asic_type != CHIP_RAVEN)
			data2 |= (DAGB1_CNTL_MISC2__DISABLE_WRREQ_CG_MASK |
			          DAGB1_CNTL_MISC2__DISABLE_WRRET_CG_MASK |
			          DAGB1_CNTL_MISC2__DISABLE_RDREQ_CG_MASK |
			          DAGB1_CNTL_MISC2__DISABLE_RDRET_CG_MASK |
			          DAGB1_CNTL_MISC2__DISABLE_TLBWR_CG_MASK |
			          DAGB1_CNTL_MISC2__DISABLE_TLBRD_CG_MASK);
	}

	if (def != data)
		WREG32_SOC15(MMHUB, 0, mmATC_L2_MISC_CG, data);

	if (def1 != data1) {
		if (adev->asic_type != CHIP_RAVEN)
			WREG32_SOC15(MMHUB, 0, mmDAGB0_CNTL_MISC2, data1);
		else
			WREG32_SOC15(MMHUB, 0, mmDAGB0_CNTL_MISC2_RV, data1);
	}

	if (adev->asic_type != CHIP_RAVEN && def2 != data2)
		WREG32_SOC15(MMHUB, 0, mmDAGB1_CNTL_MISC2, data2);
}

<<<<<<< HEAD
static void athub_update_medium_grain_clock_gating(struct amdgpu_device *adev,
						   bool enable)
{
	uint32_t def, data;

	def = data = RREG32_SOC15(ATHUB, 0, mmATHUB_MISC_CNTL);

	if (enable && (adev->cg_flags & AMD_CG_SUPPORT_MC_MGCG))
		data |= ATHUB_MISC_CNTL__CG_ENABLE_MASK;
	else
		data &= ~ATHUB_MISC_CNTL__CG_ENABLE_MASK;

	if (def != data)
		WREG32_SOC15(ATHUB, 0, mmATHUB_MISC_CNTL, data);
}

=======
>>>>>>> upstream/android-13
static void mmhub_v1_0_update_medium_grain_light_sleep(struct amdgpu_device *adev,
						       bool enable)
{
	uint32_t def, data;

	def = data = RREG32_SOC15(MMHUB, 0, mmATC_L2_MISC_CG);

	if (enable && (adev->cg_flags & AMD_CG_SUPPORT_MC_LS))
		data |= ATC_L2_MISC_CG__MEM_LS_ENABLE_MASK;
	else
		data &= ~ATC_L2_MISC_CG__MEM_LS_ENABLE_MASK;

	if (def != data)
		WREG32_SOC15(MMHUB, 0, mmATC_L2_MISC_CG, data);
}

<<<<<<< HEAD
static void athub_update_medium_grain_light_sleep(struct amdgpu_device *adev,
						  bool enable)
{
	uint32_t def, data;

	def = data = RREG32_SOC15(ATHUB, 0, mmATHUB_MISC_CNTL);

	if (enable && (adev->cg_flags & AMD_CG_SUPPORT_MC_LS) &&
	    (adev->cg_flags & AMD_CG_SUPPORT_HDP_LS))
		data |= ATHUB_MISC_CNTL__CG_MEM_LS_ENABLE_MASK;
	else
		data &= ~ATHUB_MISC_CNTL__CG_MEM_LS_ENABLE_MASK;

	if(def != data)
		WREG32_SOC15(ATHUB, 0, mmATHUB_MISC_CNTL, data);
}

int mmhub_v1_0_set_clockgating(struct amdgpu_device *adev,
=======
static int mmhub_v1_0_set_clockgating(struct amdgpu_device *adev,
>>>>>>> upstream/android-13
			       enum amd_clockgating_state state)
{
	if (amdgpu_sriov_vf(adev))
		return 0;

	switch (adev->asic_type) {
	case CHIP_VEGA10:
	case CHIP_VEGA12:
	case CHIP_VEGA20:
	case CHIP_RAVEN:
<<<<<<< HEAD
		mmhub_v1_0_update_medium_grain_clock_gating(adev,
				state == AMD_CG_STATE_GATE ? true : false);
		athub_update_medium_grain_clock_gating(adev,
				state == AMD_CG_STATE_GATE ? true : false);
		mmhub_v1_0_update_medium_grain_light_sleep(adev,
				state == AMD_CG_STATE_GATE ? true : false);
		athub_update_medium_grain_light_sleep(adev,
				state == AMD_CG_STATE_GATE ? true : false);
=======
	case CHIP_RENOIR:
		mmhub_v1_0_update_medium_grain_clock_gating(adev,
				state == AMD_CG_STATE_GATE);
		mmhub_v1_0_update_medium_grain_light_sleep(adev,
				state == AMD_CG_STATE_GATE);
>>>>>>> upstream/android-13
		break;
	default:
		break;
	}

	return 0;
}

<<<<<<< HEAD
void mmhub_v1_0_get_clockgating(struct amdgpu_device *adev, u32 *flags)
{
	int data;
=======
static void mmhub_v1_0_get_clockgating(struct amdgpu_device *adev, u32 *flags)
{
	int data, data1;
>>>>>>> upstream/android-13

	if (amdgpu_sriov_vf(adev))
		*flags = 0;

<<<<<<< HEAD
	/* AMD_CG_SUPPORT_MC_MGCG */
	data = RREG32_SOC15(ATHUB, 0, mmATHUB_MISC_CNTL);
	if (data & ATHUB_MISC_CNTL__CG_ENABLE_MASK)
		*flags |= AMD_CG_SUPPORT_MC_MGCG;

	/* AMD_CG_SUPPORT_MC_LS */
	data = RREG32_SOC15(MMHUB, 0, mmATC_L2_MISC_CG);
	if (data & ATC_L2_MISC_CG__MEM_LS_ENABLE_MASK)
		*flags |= AMD_CG_SUPPORT_MC_LS;
}
=======
	data = RREG32_SOC15(MMHUB, 0, mmATC_L2_MISC_CG);

	data1 = RREG32_SOC15(MMHUB, 0, mmDAGB0_CNTL_MISC2);

	/* AMD_CG_SUPPORT_MC_MGCG */
	if ((data & ATC_L2_MISC_CG__ENABLE_MASK) &&
	    !(data1 & (DAGB0_CNTL_MISC2__DISABLE_WRREQ_CG_MASK |
		       DAGB0_CNTL_MISC2__DISABLE_WRRET_CG_MASK |
		       DAGB0_CNTL_MISC2__DISABLE_RDREQ_CG_MASK |
		       DAGB0_CNTL_MISC2__DISABLE_RDRET_CG_MASK |
		       DAGB0_CNTL_MISC2__DISABLE_TLBWR_CG_MASK |
		       DAGB0_CNTL_MISC2__DISABLE_TLBRD_CG_MASK)))
		*flags |= AMD_CG_SUPPORT_MC_MGCG;

	/* AMD_CG_SUPPORT_MC_LS */
	if (data & ATC_L2_MISC_CG__MEM_LS_ENABLE_MASK)
		*flags |= AMD_CG_SUPPORT_MC_LS;
}

static const struct soc15_ras_field_entry mmhub_v1_0_ras_fields[] = {
	{ "MMEA0_DRAMRD_CMDMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, DRAMRD_CMDMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, DRAMRD_CMDMEM_DED_COUNT),
	},
	{ "MMEA0_DRAMWR_CMDMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, DRAMWR_CMDMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, DRAMWR_CMDMEM_DED_COUNT),
	},
	{ "MMEA0_DRAMWR_DATAMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, DRAMWR_DATAMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, DRAMWR_DATAMEM_DED_COUNT),
	},
	{ "MMEA0_RRET_TAGMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, RRET_TAGMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, RRET_TAGMEM_DED_COUNT),
	},
	{ "MMEA0_WRET_TAGMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, WRET_TAGMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, WRET_TAGMEM_DED_COUNT),
	},
	{ "MMEA0_DRAMRD_PAGEMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, DRAMRD_PAGEMEM_SED_COUNT),
	0, 0,
	},
	{ "MMEA0_DRAMWR_PAGEMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, DRAMWR_PAGEMEM_SED_COUNT),
	0, 0,
	},
	{ "MMEA0_IORD_CMDMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, IORD_CMDMEM_SED_COUNT),
	0, 0,
	},
	{ "MMEA0_IOWR_CMDMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, IOWR_CMDMEM_SED_COUNT),
	0, 0,
	},
	{ "MMEA0_IOWR_DATAMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT_VG20, IOWR_DATAMEM_SED_COUNT),
	0, 0,
	},
	{ "MMEA0_GMIRD_CMDMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT2_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT2_VG20, GMIRD_CMDMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA0_EDC_CNT2_VG20, GMIRD_CMDMEM_DED_COUNT),
	},
	{ "MMEA0_GMIWR_CMDMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT2_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT2_VG20, GMIWR_CMDMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA0_EDC_CNT2_VG20, GMIWR_CMDMEM_DED_COUNT),
	},
	{ "MMEA0_GMIWR_DATAMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT2_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT2_VG20, GMIWR_DATAMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA0_EDC_CNT2_VG20, GMIWR_DATAMEM_DED_COUNT),
	},
	{ "MMEA0_GMIRD_PAGEMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT2_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT2_VG20, GMIRD_PAGEMEM_SED_COUNT),
	0, 0,
	},
	{ "MMEA0_GMIWR_PAGEMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT2_VG20),
	SOC15_REG_FIELD(MMEA0_EDC_CNT2_VG20, GMIWR_PAGEMEM_SED_COUNT),
	0, 0,
	},
	{ "MMEA1_DRAMRD_CMDMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, DRAMRD_CMDMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, DRAMRD_CMDMEM_DED_COUNT),
	},
	{ "MMEA1_DRAMWR_CMDMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, DRAMWR_CMDMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, DRAMWR_CMDMEM_DED_COUNT),
	},
	{ "MMEA1_DRAMWR_DATAMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, DRAMWR_DATAMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, DRAMWR_DATAMEM_DED_COUNT),
	},
	{ "MMEA1_RRET_TAGMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, RRET_TAGMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, RRET_TAGMEM_DED_COUNT),
	},
	{ "MMEA1_WRET_TAGMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, WRET_TAGMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, WRET_TAGMEM_DED_COUNT),
	},
	{ "MMEA1_DRAMRD_PAGEMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, DRAMRD_PAGEMEM_SED_COUNT),
	0, 0,
	},
	{ "MMEA1_DRAMWR_PAGEMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, DRAMWR_PAGEMEM_SED_COUNT),
	0, 0,
	},
	{ "MMEA1_IORD_CMDMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, IORD_CMDMEM_SED_COUNT),
	0, 0,
	},
	{ "MMEA1_IOWR_CMDMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, IOWR_CMDMEM_SED_COUNT),
	0, 0,
	},
	{ "MMEA1_IOWR_DATAMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT_VG20, IOWR_DATAMEM_SED_COUNT),
	0, 0,
	},
	{ "MMEA1_GMIRD_CMDMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT2_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT2_VG20, GMIRD_CMDMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA1_EDC_CNT2_VG20, GMIRD_CMDMEM_DED_COUNT),
	},
	{ "MMEA1_GMIWR_CMDMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT2_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT2_VG20, GMIWR_CMDMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA1_EDC_CNT2_VG20, GMIWR_CMDMEM_DED_COUNT),
	},
	{ "MMEA1_GMIWR_DATAMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT2_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT2_VG20, GMIWR_DATAMEM_SEC_COUNT),
	SOC15_REG_FIELD(MMEA1_EDC_CNT2_VG20, GMIWR_DATAMEM_DED_COUNT),
	},
	{ "MMEA1_GMIRD_PAGEMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT2_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT2_VG20, GMIRD_PAGEMEM_SED_COUNT),
	0, 0,
	},
	{ "MMEA1_GMIWR_PAGEMEM", SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT2_VG20),
	SOC15_REG_FIELD(MMEA1_EDC_CNT2_VG20, GMIWR_PAGEMEM_SED_COUNT),
	0, 0,
	}
};

static const struct soc15_reg_entry mmhub_v1_0_edc_cnt_regs[] = {
   { SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT_VG20), 0, 0, 0},
   { SOC15_REG_ENTRY(MMHUB, 0, mmMMEA0_EDC_CNT2_VG20), 0, 0, 0},
   { SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT_VG20), 0, 0, 0},
   { SOC15_REG_ENTRY(MMHUB, 0, mmMMEA1_EDC_CNT2_VG20), 0, 0, 0},
};

static int mmhub_v1_0_get_ras_error_count(struct amdgpu_device *adev,
	const struct soc15_reg_entry *reg,
	uint32_t value, uint32_t *sec_count, uint32_t *ded_count)
{
	uint32_t i;
	uint32_t sec_cnt, ded_cnt;

	for (i = 0; i < ARRAY_SIZE(mmhub_v1_0_ras_fields); i++) {
		if (mmhub_v1_0_ras_fields[i].reg_offset != reg->reg_offset)
			continue;

		sec_cnt = (value &
				mmhub_v1_0_ras_fields[i].sec_count_mask) >>
				mmhub_v1_0_ras_fields[i].sec_count_shift;
		if (sec_cnt) {
			dev_info(adev->dev,
				"MMHUB SubBlock %s, SEC %d\n",
				mmhub_v1_0_ras_fields[i].name,
				sec_cnt);
			*sec_count += sec_cnt;
		}

		ded_cnt = (value &
				mmhub_v1_0_ras_fields[i].ded_count_mask) >>
				mmhub_v1_0_ras_fields[i].ded_count_shift;
		if (ded_cnt) {
			dev_info(adev->dev,
				"MMHUB SubBlock %s, DED %d\n",
				mmhub_v1_0_ras_fields[i].name,
				ded_cnt);
			*ded_count += ded_cnt;
		}
	}

	return 0;
}

static void mmhub_v1_0_query_ras_error_count(struct amdgpu_device *adev,
					   void *ras_error_status)
{
	struct ras_err_data *err_data = (struct ras_err_data *)ras_error_status;
	uint32_t sec_count = 0, ded_count = 0;
	uint32_t i;
	uint32_t reg_value;

	err_data->ue_count = 0;
	err_data->ce_count = 0;

	for (i = 0; i < ARRAY_SIZE(mmhub_v1_0_edc_cnt_regs); i++) {
		reg_value =
			RREG32(SOC15_REG_ENTRY_OFFSET(mmhub_v1_0_edc_cnt_regs[i]));
		if (reg_value)
			mmhub_v1_0_get_ras_error_count(adev,
				&mmhub_v1_0_edc_cnt_regs[i],
				reg_value, &sec_count, &ded_count);
	}

	err_data->ce_count += sec_count;
	err_data->ue_count += ded_count;
}

static void mmhub_v1_0_reset_ras_error_count(struct amdgpu_device *adev)
{
	uint32_t i;

	/* read back edc counter registers to reset the counters to 0 */
	if (amdgpu_ras_is_supported(adev, AMDGPU_RAS_BLOCK__MMHUB)) {
		for (i = 0; i < ARRAY_SIZE(mmhub_v1_0_edc_cnt_regs); i++)
			RREG32(SOC15_REG_ENTRY_OFFSET(mmhub_v1_0_edc_cnt_regs[i]));
	}
}

const struct amdgpu_mmhub_ras_funcs mmhub_v1_0_ras_funcs = {
	.ras_late_init = amdgpu_mmhub_ras_late_init,
	.ras_fini = amdgpu_mmhub_ras_fini,
	.query_ras_error_count = mmhub_v1_0_query_ras_error_count,
	.reset_ras_error_count = mmhub_v1_0_reset_ras_error_count,
};

const struct amdgpu_mmhub_funcs mmhub_v1_0_funcs = {
	.get_fb_location = mmhub_v1_0_get_fb_location,
	.init = mmhub_v1_0_init,
	.gart_enable = mmhub_v1_0_gart_enable,
	.set_fault_enable_default = mmhub_v1_0_set_fault_enable_default,
	.gart_disable = mmhub_v1_0_gart_disable,
	.set_clockgating = mmhub_v1_0_set_clockgating,
	.get_clockgating = mmhub_v1_0_get_clockgating,
	.setup_vm_pt_regs = mmhub_v1_0_setup_vm_pt_regs,
	.update_power_gating = mmhub_v1_0_update_power_gating,
};
>>>>>>> upstream/android-13
