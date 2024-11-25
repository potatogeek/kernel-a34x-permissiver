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
#include <drm/drmP.h>
=======

#include <linux/pci.h>

>>>>>>> upstream/android-13
#include "amdgpu.h"
#include "amdgpu_ih.h"
#include "soc15.h"

#include "oss/osssys_4_0_offset.h"
#include "oss/osssys_4_0_sh_mask.h"

#include "soc15_common.h"
#include "vega10_ih.h"

<<<<<<< HEAD

=======
#define MAX_REARM_RETRY 10
>>>>>>> upstream/android-13

static void vega10_ih_set_interrupt_funcs(struct amdgpu_device *adev);

/**
<<<<<<< HEAD
 * vega10_ih_enable_interrupts - Enable the interrupt ring buffer
 *
 * @adev: amdgpu_device pointer
 *
 * Enable the interrupt ring buffer (VEGA10).
 */
static void vega10_ih_enable_interrupts(struct amdgpu_device *adev)
{
	u32 ih_rb_cntl = RREG32_SOC15(OSSSYS, 0, mmIH_RB_CNTL);

	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, RB_ENABLE, 1);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, ENABLE_INTR, 1);
	WREG32_SOC15(OSSSYS, 0, mmIH_RB_CNTL, ih_rb_cntl);
	adev->irq.ih.enabled = true;
}

/**
 * vega10_ih_disable_interrupts - Disable the interrupt ring buffer
 *
 * @adev: amdgpu_device pointer
 *
 * Disable the interrupt ring buffer (VEGA10).
 */
static void vega10_ih_disable_interrupts(struct amdgpu_device *adev)
{
	u32 ih_rb_cntl = RREG32_SOC15(OSSSYS, 0, mmIH_RB_CNTL);

	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, RB_ENABLE, 0);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, ENABLE_INTR, 0);
	WREG32_SOC15(OSSSYS, 0, mmIH_RB_CNTL, ih_rb_cntl);
	/* set rptr, wptr to 0 */
	WREG32_SOC15(OSSSYS, 0, mmIH_RB_RPTR, 0);
	WREG32_SOC15(OSSSYS, 0, mmIH_RB_WPTR, 0);
	adev->irq.ih.enabled = false;
	adev->irq.ih.rptr = 0;
=======
 * vega10_ih_init_register_offset - Initialize register offset for ih rings
 *
 * @adev: amdgpu_device pointer
 *
 * Initialize register offset ih rings (VEGA10).
 */
static void vega10_ih_init_register_offset(struct amdgpu_device *adev)
{
	struct amdgpu_ih_regs *ih_regs;

	if (adev->irq.ih.ring_size) {
		ih_regs = &adev->irq.ih.ih_regs;
		ih_regs->ih_rb_base = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_BASE);
		ih_regs->ih_rb_base_hi = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_BASE_HI);
		ih_regs->ih_rb_cntl = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_CNTL);
		ih_regs->ih_rb_wptr = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_WPTR);
		ih_regs->ih_rb_rptr = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_RPTR);
		ih_regs->ih_doorbell_rptr = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_DOORBELL_RPTR);
		ih_regs->ih_rb_wptr_addr_lo = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_WPTR_ADDR_LO);
		ih_regs->ih_rb_wptr_addr_hi = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_WPTR_ADDR_HI);
		ih_regs->psp_reg_id = PSP_REG_IH_RB_CNTL;
	}

	if (adev->irq.ih1.ring_size) {
		ih_regs = &adev->irq.ih1.ih_regs;
		ih_regs->ih_rb_base = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_BASE_RING1);
		ih_regs->ih_rb_base_hi = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_BASE_HI_RING1);
		ih_regs->ih_rb_cntl = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_CNTL_RING1);
		ih_regs->ih_rb_wptr = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_WPTR_RING1);
		ih_regs->ih_rb_rptr = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_RPTR_RING1);
		ih_regs->ih_doorbell_rptr = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_DOORBELL_RPTR_RING1);
		ih_regs->psp_reg_id = PSP_REG_IH_RB_CNTL_RING1;
	}

	if (adev->irq.ih2.ring_size) {
		ih_regs = &adev->irq.ih2.ih_regs;
		ih_regs->ih_rb_base = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_BASE_RING2);
		ih_regs->ih_rb_base_hi = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_BASE_HI_RING2);
		ih_regs->ih_rb_cntl = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_CNTL_RING2);
		ih_regs->ih_rb_wptr = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_WPTR_RING2);
		ih_regs->ih_rb_rptr = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_RPTR_RING2);
		ih_regs->ih_doorbell_rptr = SOC15_REG_OFFSET(OSSSYS, 0, mmIH_DOORBELL_RPTR_RING2);
		ih_regs->psp_reg_id = PSP_REG_IH_RB_CNTL_RING2;
	}
}

/**
 * vega10_ih_toggle_ring_interrupts - toggle the interrupt ring buffer
 *
 * @adev: amdgpu_device pointer
 * @ih: amdgpu_ih_ring pointet
 * @enable: true - enable the interrupts, false - disable the interrupts
 *
 * Toggle the interrupt ring buffer (VEGA10)
 */
static int vega10_ih_toggle_ring_interrupts(struct amdgpu_device *adev,
					    struct amdgpu_ih_ring *ih,
					    bool enable)
{
	struct amdgpu_ih_regs *ih_regs;
	uint32_t tmp;

	ih_regs = &ih->ih_regs;

	tmp = RREG32(ih_regs->ih_rb_cntl);
	tmp = REG_SET_FIELD(tmp, IH_RB_CNTL, RB_ENABLE, (enable ? 1 : 0));
	tmp = REG_SET_FIELD(tmp, IH_RB_CNTL, RB_GPU_TS_ENABLE, 1);
	/* enable_intr field is only valid in ring0 */
	if (ih == &adev->irq.ih)
		tmp = REG_SET_FIELD(tmp, IH_RB_CNTL, ENABLE_INTR, (enable ? 1 : 0));
	if (amdgpu_sriov_vf(adev)) {
		if (psp_reg_program(&adev->psp, ih_regs->psp_reg_id, tmp)) {
			dev_err(adev->dev, "PSP program IH_RB_CNTL failed!\n");
			return -ETIMEDOUT;
		}
	} else {
		WREG32(ih_regs->ih_rb_cntl, tmp);
	}

	if (enable) {
		ih->enabled = true;
	} else {
		/* set rptr, wptr to 0 */
		WREG32(ih_regs->ih_rb_rptr, 0);
		WREG32(ih_regs->ih_rb_wptr, 0);
		ih->enabled = false;
		ih->rptr = 0;
	}

	return 0;
}

/**
 * vega10_ih_toggle_interrupts - Toggle all the available interrupt ring buffers
 *
 * @adev: amdgpu_device pointer
 * @enable: enable or disable interrupt ring buffers
 *
 * Toggle all the available interrupt ring buffers (VEGA10).
 */
static int vega10_ih_toggle_interrupts(struct amdgpu_device *adev, bool enable)
{
	struct amdgpu_ih_ring *ih[] = {&adev->irq.ih, &adev->irq.ih1, &adev->irq.ih2};
	int i;
	int r;

	for (i = 0; i < ARRAY_SIZE(ih); i++) {
		if (ih[i]->ring_size) {
			r = vega10_ih_toggle_ring_interrupts(adev, ih[i], enable);
			if (r)
				return r;
		}
	}

	return 0;
}

static uint32_t vega10_ih_rb_cntl(struct amdgpu_ih_ring *ih, uint32_t ih_rb_cntl)
{
	int rb_bufsz = order_base_2(ih->ring_size / 4);

	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL,
				   MC_SPACE, ih->use_bus_addr ? 1 : 4);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL,
				   WPTR_OVERFLOW_CLEAR, 1);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL,
				   WPTR_OVERFLOW_ENABLE, 1);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, RB_SIZE, rb_bufsz);
	/* Ring Buffer write pointer writeback. If enabled, IH_RB_WPTR register
	 * value is written to memory
	 */
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL,
				   WPTR_WRITEBACK_ENABLE, 1);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, MC_SNOOP, 1);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, MC_RO, 0);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, MC_VMID, 0);

	return ih_rb_cntl;
}

static uint32_t vega10_ih_doorbell_rptr(struct amdgpu_ih_ring *ih)
{
	u32 ih_doorbell_rtpr = 0;

	if (ih->use_doorbell) {
		ih_doorbell_rtpr = REG_SET_FIELD(ih_doorbell_rtpr,
						 IH_DOORBELL_RPTR, OFFSET,
						 ih->doorbell_index);
		ih_doorbell_rtpr = REG_SET_FIELD(ih_doorbell_rtpr,
						 IH_DOORBELL_RPTR,
						 ENABLE, 1);
	} else {
		ih_doorbell_rtpr = REG_SET_FIELD(ih_doorbell_rtpr,
						 IH_DOORBELL_RPTR,
						 ENABLE, 0);
	}
	return ih_doorbell_rtpr;
}

/**
 * vega10_ih_enable_ring - enable an ih ring buffer
 *
 * @adev: amdgpu_device pointer
 * @ih: amdgpu_ih_ring pointer
 *
 * Enable an ih ring buffer (VEGA10)
 */
static int vega10_ih_enable_ring(struct amdgpu_device *adev,
				 struct amdgpu_ih_ring *ih)
{
	struct amdgpu_ih_regs *ih_regs;
	uint32_t tmp;

	ih_regs = &ih->ih_regs;

	/* Ring Buffer base. [39:8] of 40-bit address of the beginning of the ring buffer*/
	WREG32(ih_regs->ih_rb_base, ih->gpu_addr >> 8);
	WREG32(ih_regs->ih_rb_base_hi, (ih->gpu_addr >> 40) & 0xff);

	tmp = RREG32(ih_regs->ih_rb_cntl);
	tmp = vega10_ih_rb_cntl(ih, tmp);
	if (ih == &adev->irq.ih)
		tmp = REG_SET_FIELD(tmp, IH_RB_CNTL, RPTR_REARM, !!adev->irq.msi_enabled);
	if (ih == &adev->irq.ih1)
		tmp = REG_SET_FIELD(tmp, IH_RB_CNTL, RB_FULL_DRAIN_ENABLE, 1);
	if (amdgpu_sriov_vf(adev)) {
		if (psp_reg_program(&adev->psp, ih_regs->psp_reg_id, tmp)) {
			dev_err(adev->dev, "PSP program IH_RB_CNTL failed!\n");
			return -ETIMEDOUT;
		}
	} else {
		WREG32(ih_regs->ih_rb_cntl, tmp);
	}

	if (ih == &adev->irq.ih) {
		/* set the ih ring 0 writeback address whether it's enabled or not */
		WREG32(ih_regs->ih_rb_wptr_addr_lo, lower_32_bits(ih->wptr_addr));
		WREG32(ih_regs->ih_rb_wptr_addr_hi, upper_32_bits(ih->wptr_addr) & 0xFFFF);
	}

	/* set rptr, wptr to 0 */
	WREG32(ih_regs->ih_rb_wptr, 0);
	WREG32(ih_regs->ih_rb_rptr, 0);

	WREG32(ih_regs->ih_doorbell_rptr, vega10_ih_doorbell_rptr(ih));

	return 0;
>>>>>>> upstream/android-13
}

/**
 * vega10_ih_irq_init - init and enable the interrupt ring
 *
 * @adev: amdgpu_device pointer
 *
 * Allocate a ring buffer for the interrupt controller,
 * enable the RLC, disable interrupts, enable the IH
 * ring buffer and enable it (VI).
 * Called at device load and reume.
 * Returns 0 for success, errors for failure.
 */
static int vega10_ih_irq_init(struct amdgpu_device *adev)
{
<<<<<<< HEAD
	int ret = 0;
	int rb_bufsz;
	u32 ih_rb_cntl, ih_doorbell_rtpr;
	u32 tmp;
	u64 wptr_off;

	/* disable irqs */
	vega10_ih_disable_interrupts(adev);

	adev->nbio_funcs->ih_control(adev);

	ih_rb_cntl = RREG32_SOC15(OSSSYS, 0, mmIH_RB_CNTL);
	/* Ring Buffer base. [39:8] of 40-bit address of the beginning of the ring buffer*/
	if (adev->irq.ih.use_bus_addr) {
		WREG32_SOC15(OSSSYS, 0, mmIH_RB_BASE, adev->irq.ih.rb_dma_addr >> 8);
		WREG32_SOC15(OSSSYS, 0, mmIH_RB_BASE_HI, ((u64)adev->irq.ih.rb_dma_addr >> 40) & 0xff);
		ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, MC_SPACE, 1);
	} else {
		WREG32_SOC15(OSSSYS, 0, mmIH_RB_BASE, adev->irq.ih.gpu_addr >> 8);
		WREG32_SOC15(OSSSYS, 0, mmIH_RB_BASE_HI, (adev->irq.ih.gpu_addr >> 40) & 0xff);
		ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, MC_SPACE, 4);
	}
	rb_bufsz = order_base_2(adev->irq.ih.ring_size / 4);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, WPTR_OVERFLOW_CLEAR, 1);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, WPTR_OVERFLOW_ENABLE, 1);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, RB_SIZE, rb_bufsz);
	/* Ring Buffer write pointer writeback. If enabled, IH_RB_WPTR register value is written to memory */
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, WPTR_WRITEBACK_ENABLE, 1);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, MC_SNOOP, 1);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, MC_RO, 0);
	ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, MC_VMID, 0);

	if (adev->irq.msi_enabled)
		ih_rb_cntl = REG_SET_FIELD(ih_rb_cntl, IH_RB_CNTL, RPTR_REARM, 1);

	WREG32_SOC15(OSSSYS, 0, mmIH_RB_CNTL, ih_rb_cntl);

	/* set the writeback address whether it's enabled or not */
	if (adev->irq.ih.use_bus_addr)
		wptr_off = adev->irq.ih.rb_dma_addr + (adev->irq.ih.wptr_offs * 4);
	else
		wptr_off = adev->wb.gpu_addr + (adev->irq.ih.wptr_offs * 4);
	WREG32_SOC15(OSSSYS, 0, mmIH_RB_WPTR_ADDR_LO, lower_32_bits(wptr_off));
	WREG32_SOC15(OSSSYS, 0, mmIH_RB_WPTR_ADDR_HI, upper_32_bits(wptr_off) & 0xFFFF);

	/* set rptr, wptr to 0 */
	WREG32_SOC15(OSSSYS, 0, mmIH_RB_RPTR, 0);
	WREG32_SOC15(OSSSYS, 0, mmIH_RB_WPTR, 0);

	ih_doorbell_rtpr = RREG32_SOC15(OSSSYS, 0, mmIH_DOORBELL_RPTR);
	if (adev->irq.ih.use_doorbell) {
		ih_doorbell_rtpr = REG_SET_FIELD(ih_doorbell_rtpr, IH_DOORBELL_RPTR,
						 OFFSET, adev->irq.ih.doorbell_index);
		ih_doorbell_rtpr = REG_SET_FIELD(ih_doorbell_rtpr, IH_DOORBELL_RPTR,
						 ENABLE, 1);
	} else {
		ih_doorbell_rtpr = REG_SET_FIELD(ih_doorbell_rtpr, IH_DOORBELL_RPTR,
						 ENABLE, 0);
	}
	WREG32_SOC15(OSSSYS, 0, mmIH_DOORBELL_RPTR, ih_doorbell_rtpr);
	adev->nbio_funcs->ih_doorbell_range(adev, adev->irq.ih.use_doorbell,
					    adev->irq.ih.doorbell_index);

	tmp = RREG32_SOC15(OSSSYS, 0, mmIH_STORM_CLIENT_LIST_CNTL);
	tmp = REG_SET_FIELD(tmp, IH_STORM_CLIENT_LIST_CNTL,
			    CLIENT18_IS_STORM_CLIENT, 1);
	WREG32_SOC15(OSSSYS, 0, mmIH_STORM_CLIENT_LIST_CNTL, tmp);

	tmp = RREG32_SOC15(OSSSYS, 0, mmIH_INT_FLOOD_CNTL);
	tmp = REG_SET_FIELD(tmp, IH_INT_FLOOD_CNTL, FLOOD_CNTL_ENABLE, 1);
	WREG32_SOC15(OSSSYS, 0, mmIH_INT_FLOOD_CNTL, tmp);
=======
	struct amdgpu_ih_ring *ih[] = {&adev->irq.ih, &adev->irq.ih1, &adev->irq.ih2};
	u32 ih_chicken;
	int ret;
	int i;

	/* disable irqs */
	ret = vega10_ih_toggle_interrupts(adev, false);
	if (ret)
		return ret;

	adev->nbio.funcs->ih_control(adev);

	if (adev->asic_type == CHIP_RENOIR) {
		ih_chicken = RREG32_SOC15(OSSSYS, 0, mmIH_CHICKEN);
		if (adev->irq.ih.use_bus_addr) {
			ih_chicken = REG_SET_FIELD(ih_chicken, IH_CHICKEN,
						   MC_SPACE_GPA_ENABLE, 1);
		}
		WREG32_SOC15(OSSSYS, 0, mmIH_CHICKEN, ih_chicken);
	}

	for (i = 0; i < ARRAY_SIZE(ih); i++) {
		if (ih[i]->ring_size) {
			ret = vega10_ih_enable_ring(adev, ih[i]);
			if (ret)
				return ret;
		}
	}
>>>>>>> upstream/android-13

	pci_set_master(adev->pdev);

	/* enable interrupts */
<<<<<<< HEAD
	vega10_ih_enable_interrupts(adev);

	return ret;
=======
	ret = vega10_ih_toggle_interrupts(adev, true);
	if (ret)
		return ret;

	if (adev->irq.ih_soft.ring_size)
		adev->irq.ih_soft.enabled = true;

	return 0;
>>>>>>> upstream/android-13
}

/**
 * vega10_ih_irq_disable - disable interrupts
 *
 * @adev: amdgpu_device pointer
 *
 * Disable interrupts on the hw (VEGA10).
 */
static void vega10_ih_irq_disable(struct amdgpu_device *adev)
{
<<<<<<< HEAD
	vega10_ih_disable_interrupts(adev);
=======
	vega10_ih_toggle_interrupts(adev, false);
>>>>>>> upstream/android-13

	/* Wait and acknowledge irq */
	mdelay(1);
}

/**
 * vega10_ih_get_wptr - get the IH ring buffer wptr
 *
 * @adev: amdgpu_device pointer
<<<<<<< HEAD
=======
 * @ih: IH ring buffer to fetch wptr
>>>>>>> upstream/android-13
 *
 * Get the IH ring buffer wptr from either the register
 * or the writeback memory buffer (VEGA10).  Also check for
 * ring buffer overflow and deal with it.
 * Returns the value of the wptr.
 */
<<<<<<< HEAD
static u32 vega10_ih_get_wptr(struct amdgpu_device *adev)
{
	u32 wptr, tmp;

	if (adev->irq.ih.use_bus_addr)
		wptr = le32_to_cpu(adev->irq.ih.ring[adev->irq.ih.wptr_offs]);
	else
		wptr = le32_to_cpu(adev->wb.wb[adev->irq.ih.wptr_offs]);

	if (REG_GET_FIELD(wptr, IH_RB_WPTR, RB_OVERFLOW)) {
		wptr = REG_SET_FIELD(wptr, IH_RB_WPTR, RB_OVERFLOW, 0);

		/* When a ring buffer overflow happen start parsing interrupt
		 * from the last not overwritten vector (wptr + 32). Hopefully
		 * this should allow us to catchup.
		 */
		tmp = (wptr + 32) & adev->irq.ih.ptr_mask;
		dev_warn(adev->dev, "IH ring buffer overflow (0x%08X, 0x%08X, 0x%08X)\n",
			wptr, adev->irq.ih.rptr, tmp);
		adev->irq.ih.rptr = tmp;

		tmp = RREG32_NO_KIQ(SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_CNTL));
		tmp = REG_SET_FIELD(tmp, IH_RB_CNTL, WPTR_OVERFLOW_CLEAR, 1);
		WREG32_NO_KIQ(SOC15_REG_OFFSET(OSSSYS, 0, mmIH_RB_CNTL), tmp);
	}
	return (wptr & adev->irq.ih.ptr_mask);
}

/**
 * vega10_ih_prescreen_iv - prescreen an interrupt vector
 *
 * @adev: amdgpu_device pointer
 *
 * Returns true if the interrupt vector should be further processed.
 */
static bool vega10_ih_prescreen_iv(struct amdgpu_device *adev)
{
	u32 ring_index = adev->irq.ih.rptr >> 2;
	u32 dw0, dw3, dw4, dw5;
	u16 pasid;
	u64 addr, key;
	struct amdgpu_vm *vm;
	int r;

	dw0 = le32_to_cpu(adev->irq.ih.ring[ring_index + 0]);
	dw3 = le32_to_cpu(adev->irq.ih.ring[ring_index + 3]);
	dw4 = le32_to_cpu(adev->irq.ih.ring[ring_index + 4]);
	dw5 = le32_to_cpu(adev->irq.ih.ring[ring_index + 5]);

	/* Filter retry page faults, let only the first one pass. If
	 * there are too many outstanding faults, ignore them until
	 * some faults get cleared.
	 */
	switch (dw0 & 0xff) {
	case SOC15_IH_CLIENTID_VMC:
	case SOC15_IH_CLIENTID_UTCL2:
		break;
	default:
		/* Not a VM fault */
		return true;
	}

	pasid = dw3 & 0xffff;
	/* No PASID, can't identify faulting process */
	if (!pasid)
		return true;

	/* Not a retry fault, check fault credit */
	if (!(dw5 & 0x80)) {
		if (!amdgpu_vm_pasid_fault_credit(adev, pasid))
			goto ignore_iv;
		return true;
	}

	addr = ((u64)(dw5 & 0xf) << 44) | ((u64)dw4 << 12);
	key = AMDGPU_VM_FAULT(pasid, addr);
	r = amdgpu_ih_add_fault(adev, key);

	/* Hash table is full or the fault is already being processed,
	 * ignore further page faults
	 */
	if (r != 0)
		goto ignore_iv;

	/* Track retry faults in per-VM fault FIFO. */
	spin_lock(&adev->vm_manager.pasid_lock);
	vm = idr_find(&adev->vm_manager.pasid_idr, pasid);
	if (!vm) {
		/* VM not found, process it normally */
		spin_unlock(&adev->vm_manager.pasid_lock);
		amdgpu_ih_clear_fault(adev, key);
		return true;
	}
	/* No locking required with single writer and single reader */
	r = kfifo_put(&vm->faults, key);
	if (!r) {
		/* FIFO is full. Ignore it until there is space */
		spin_unlock(&adev->vm_manager.pasid_lock);
		amdgpu_ih_clear_fault(adev, key);
		goto ignore_iv;
	}
	spin_unlock(&adev->vm_manager.pasid_lock);

	/* It's the first fault for this address, process it normally */
	return true;

ignore_iv:
	adev->irq.ih.rptr += 32;
	return false;
}

/**
 * vega10_ih_decode_iv - decode an interrupt vector
 *
 * @adev: amdgpu_device pointer
 *
 * Decodes the interrupt vector at the current rptr
 * position and also advance the position.
 */
static void vega10_ih_decode_iv(struct amdgpu_device *adev,
				 struct amdgpu_iv_entry *entry)
{
	/* wptr/rptr are in bytes! */
	u32 ring_index = adev->irq.ih.rptr >> 2;
	uint32_t dw[8];

	dw[0] = le32_to_cpu(adev->irq.ih.ring[ring_index + 0]);
	dw[1] = le32_to_cpu(adev->irq.ih.ring[ring_index + 1]);
	dw[2] = le32_to_cpu(adev->irq.ih.ring[ring_index + 2]);
	dw[3] = le32_to_cpu(adev->irq.ih.ring[ring_index + 3]);
	dw[4] = le32_to_cpu(adev->irq.ih.ring[ring_index + 4]);
	dw[5] = le32_to_cpu(adev->irq.ih.ring[ring_index + 5]);
	dw[6] = le32_to_cpu(adev->irq.ih.ring[ring_index + 6]);
	dw[7] = le32_to_cpu(adev->irq.ih.ring[ring_index + 7]);

	entry->client_id = dw[0] & 0xff;
	entry->src_id = (dw[0] >> 8) & 0xff;
	entry->ring_id = (dw[0] >> 16) & 0xff;
	entry->vmid = (dw[0] >> 24) & 0xf;
	entry->vmid_src = (dw[0] >> 31);
	entry->timestamp = dw[1] | ((u64)(dw[2] & 0xffff) << 32);
	entry->timestamp_src = dw[2] >> 31;
	entry->pasid = dw[3] & 0xffff;
	entry->pasid_src = dw[3] >> 31;
	entry->src_data[0] = dw[4];
	entry->src_data[1] = dw[5];
	entry->src_data[2] = dw[6];
	entry->src_data[3] = dw[7];


	/* wptr/rptr are in bytes! */
	adev->irq.ih.rptr += 32;
=======
static u32 vega10_ih_get_wptr(struct amdgpu_device *adev,
			      struct amdgpu_ih_ring *ih)
{
	u32 wptr, tmp;
	struct amdgpu_ih_regs *ih_regs;

	if (ih == &adev->irq.ih) {
		/* Only ring0 supports writeback. On other rings fall back
		 * to register-based code with overflow checking below.
		 */
		wptr = le32_to_cpu(*ih->wptr_cpu);

		if (!REG_GET_FIELD(wptr, IH_RB_WPTR, RB_OVERFLOW))
			goto out;
	}

	ih_regs = &ih->ih_regs;

	/* Double check that the overflow wasn't already cleared. */
	wptr = RREG32_NO_KIQ(ih_regs->ih_rb_wptr);
	if (!REG_GET_FIELD(wptr, IH_RB_WPTR, RB_OVERFLOW))
		goto out;

	wptr = REG_SET_FIELD(wptr, IH_RB_WPTR, RB_OVERFLOW, 0);

	/* When a ring buffer overflow happen start parsing interrupt
	 * from the last not overwritten vector (wptr + 32). Hopefully
	 * this should allow us to catchup.
	 */
	tmp = (wptr + 32) & ih->ptr_mask;
	dev_warn(adev->dev, "IH ring buffer overflow "
		 "(0x%08X, 0x%08X, 0x%08X)\n",
		 wptr, ih->rptr, tmp);
	ih->rptr = tmp;

	tmp = RREG32_NO_KIQ(ih_regs->ih_rb_cntl);
	tmp = REG_SET_FIELD(tmp, IH_RB_CNTL, WPTR_OVERFLOW_CLEAR, 1);
	WREG32_NO_KIQ(ih_regs->ih_rb_cntl, tmp);

out:
	return (wptr & ih->ptr_mask);
}

/**
 * vega10_ih_irq_rearm - rearm IRQ if lost
 *
 * @adev: amdgpu_device pointer
 * @ih: IH ring to match
 *
 */
static void vega10_ih_irq_rearm(struct amdgpu_device *adev,
			       struct amdgpu_ih_ring *ih)
{
	uint32_t v = 0;
	uint32_t i = 0;
	struct amdgpu_ih_regs *ih_regs;

	ih_regs = &ih->ih_regs;
	/* Rearm IRQ / re-wwrite doorbell if doorbell write is lost */
	for (i = 0; i < MAX_REARM_RETRY; i++) {
		v = RREG32_NO_KIQ(ih_regs->ih_rb_rptr);
		if ((v < ih->ring_size) && (v != ih->rptr))
			WDOORBELL32(ih->doorbell_index, ih->rptr);
		else
			break;
	}
>>>>>>> upstream/android-13
}

/**
 * vega10_ih_set_rptr - set the IH ring buffer rptr
 *
 * @adev: amdgpu_device pointer
<<<<<<< HEAD
 *
 * Set the IH ring buffer rptr.
 */
static void vega10_ih_set_rptr(struct amdgpu_device *adev)
{
	if (adev->irq.ih.use_doorbell) {
		/* XXX check if swapping is necessary on BE */
		if (adev->irq.ih.use_bus_addr)
			adev->irq.ih.ring[adev->irq.ih.rptr_offs] = adev->irq.ih.rptr;
		else
			adev->wb.wb[adev->irq.ih.rptr_offs] = adev->irq.ih.rptr;
		WDOORBELL32(adev->irq.ih.doorbell_index, adev->irq.ih.rptr);
	} else {
		WREG32_SOC15(OSSSYS, 0, mmIH_RB_RPTR, adev->irq.ih.rptr);
	}
}

=======
 * @ih: IH ring buffer to set rptr
 *
 * Set the IH ring buffer rptr.
 */
static void vega10_ih_set_rptr(struct amdgpu_device *adev,
			       struct amdgpu_ih_ring *ih)
{
	struct amdgpu_ih_regs *ih_regs;

	if (ih->use_doorbell) {
		/* XXX check if swapping is necessary on BE */
		*ih->rptr_cpu = ih->rptr;
		WDOORBELL32(ih->doorbell_index, ih->rptr);

		if (amdgpu_sriov_vf(adev))
			vega10_ih_irq_rearm(adev, ih);
	} else {
		ih_regs = &ih->ih_regs;
		WREG32(ih_regs->ih_rb_rptr, ih->rptr);
	}
}

/**
 * vega10_ih_self_irq - dispatch work for ring 1 and 2
 *
 * @adev: amdgpu_device pointer
 * @source: irq source
 * @entry: IV with WPTR update
 *
 * Update the WPTR from the IV and schedule work to handle the entries.
 */
static int vega10_ih_self_irq(struct amdgpu_device *adev,
			      struct amdgpu_irq_src *source,
			      struct amdgpu_iv_entry *entry)
{
	switch (entry->ring_id) {
	case 1:
		schedule_work(&adev->irq.ih1_work);
		break;
	case 2:
		schedule_work(&adev->irq.ih2_work);
		break;
	default: break;
	}
	return 0;
}

static const struct amdgpu_irq_src_funcs vega10_ih_self_irq_funcs = {
	.process = vega10_ih_self_irq,
};

static void vega10_ih_set_self_irq_funcs(struct amdgpu_device *adev)
{
	adev->irq.self_irq.num_types = 0;
	adev->irq.self_irq.funcs = &vega10_ih_self_irq_funcs;
}

>>>>>>> upstream/android-13
static int vega10_ih_early_init(void *handle)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

	vega10_ih_set_interrupt_funcs(adev);
<<<<<<< HEAD
=======
	vega10_ih_set_self_irq_funcs(adev);
>>>>>>> upstream/android-13
	return 0;
}

static int vega10_ih_sw_init(void *handle)
{
<<<<<<< HEAD
	int r;
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

	r = amdgpu_ih_ring_init(adev, 256 * 1024, true);
=======
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;
	int r;

	r = amdgpu_irq_add_id(adev, SOC15_IH_CLIENTID_IH, 0,
			      &adev->irq.self_irq);
	if (r)
		return r;

	r = amdgpu_ih_ring_init(adev, &adev->irq.ih, 256 * 1024, true);
>>>>>>> upstream/android-13
	if (r)
		return r;

	adev->irq.ih.use_doorbell = true;
<<<<<<< HEAD
	adev->irq.ih.doorbell_index = AMDGPU_DOORBELL64_IH << 1;

	adev->irq.ih.faults = kmalloc(sizeof(*adev->irq.ih.faults), GFP_KERNEL);
	if (!adev->irq.ih.faults)
		return -ENOMEM;
	INIT_CHASH_TABLE(adev->irq.ih.faults->hash,
			 AMDGPU_PAGEFAULT_HASH_BITS, 8, 0);
	spin_lock_init(&adev->irq.ih.faults->lock);
	adev->irq.ih.faults->count = 0;
=======
	adev->irq.ih.doorbell_index = adev->doorbell_index.ih << 1;

	if (!(adev->flags & AMD_IS_APU)) {
		r = amdgpu_ih_ring_init(adev, &adev->irq.ih1, PAGE_SIZE, true);
		if (r)
			return r;

		adev->irq.ih1.use_doorbell = true;
		adev->irq.ih1.doorbell_index = (adev->doorbell_index.ih + 1) << 1;

		r = amdgpu_ih_ring_init(adev, &adev->irq.ih2, PAGE_SIZE, true);
		if (r)
			return r;

		adev->irq.ih2.use_doorbell = true;
		adev->irq.ih2.doorbell_index = (adev->doorbell_index.ih + 2) << 1;
	}
	/* initialize ih control registers offset */
	vega10_ih_init_register_offset(adev);

	r = amdgpu_ih_ring_init(adev, &adev->irq.ih_soft, PAGE_SIZE, true);
	if (r)
		return r;
>>>>>>> upstream/android-13

	r = amdgpu_irq_init(adev);

	return r;
}

static int vega10_ih_sw_fini(void *handle)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

<<<<<<< HEAD
	amdgpu_irq_fini(adev);
	amdgpu_ih_ring_fini(adev);

	kfree(adev->irq.ih.faults);
	adev->irq.ih.faults = NULL;
=======
	amdgpu_irq_fini_sw(adev);
>>>>>>> upstream/android-13

	return 0;
}

static int vega10_ih_hw_init(void *handle)
{
	int r;
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

	r = vega10_ih_irq_init(adev);
	if (r)
		return r;

	return 0;
}

static int vega10_ih_hw_fini(void *handle)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

	vega10_ih_irq_disable(adev);

	return 0;
}

static int vega10_ih_suspend(void *handle)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

	return vega10_ih_hw_fini(adev);
}

static int vega10_ih_resume(void *handle)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

	return vega10_ih_hw_init(adev);
}

static bool vega10_ih_is_idle(void *handle)
{
	/* todo */
	return true;
}

static int vega10_ih_wait_for_idle(void *handle)
{
	/* todo */
	return -ETIMEDOUT;
}

static int vega10_ih_soft_reset(void *handle)
{
	/* todo */

	return 0;
}

<<<<<<< HEAD
static int vega10_ih_set_clockgating_state(void *handle,
					  enum amd_clockgating_state state)
{
	return 0;
=======
static void vega10_ih_update_clockgating_state(struct amdgpu_device *adev,
					       bool enable)
{
	uint32_t data, def, field_val;

	if (adev->cg_flags & AMD_CG_SUPPORT_IH_CG) {
		def = data = RREG32_SOC15(OSSSYS, 0, mmIH_CLK_CTRL);
		field_val = enable ? 0 : 1;
		/**
		 * Vega10/12 and RAVEN don't have IH_BUFFER_MEM_CLK_SOFT_OVERRIDE field.
		 */
		if (adev->asic_type == CHIP_RENOIR)
			data = REG_SET_FIELD(data, IH_CLK_CTRL,
				     IH_BUFFER_MEM_CLK_SOFT_OVERRIDE, field_val);

		data = REG_SET_FIELD(data, IH_CLK_CTRL,
				     DBUS_MUX_CLK_SOFT_OVERRIDE, field_val);
		data = REG_SET_FIELD(data, IH_CLK_CTRL,
				     OSSSYS_SHARE_CLK_SOFT_OVERRIDE, field_val);
		data = REG_SET_FIELD(data, IH_CLK_CTRL,
				     LIMIT_SMN_CLK_SOFT_OVERRIDE, field_val);
		data = REG_SET_FIELD(data, IH_CLK_CTRL,
				     DYN_CLK_SOFT_OVERRIDE, field_val);
		data = REG_SET_FIELD(data, IH_CLK_CTRL,
				     REG_CLK_SOFT_OVERRIDE, field_val);
		if (def != data)
			WREG32_SOC15(OSSSYS, 0, mmIH_CLK_CTRL, data);
	}
}

static int vega10_ih_set_clockgating_state(void *handle,
					  enum amd_clockgating_state state)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)handle;

	vega10_ih_update_clockgating_state(adev,
				state == AMD_CG_STATE_GATE);
	return 0;

>>>>>>> upstream/android-13
}

static int vega10_ih_set_powergating_state(void *handle,
					  enum amd_powergating_state state)
{
	return 0;
}

const struct amd_ip_funcs vega10_ih_ip_funcs = {
	.name = "vega10_ih",
	.early_init = vega10_ih_early_init,
	.late_init = NULL,
	.sw_init = vega10_ih_sw_init,
	.sw_fini = vega10_ih_sw_fini,
	.hw_init = vega10_ih_hw_init,
	.hw_fini = vega10_ih_hw_fini,
	.suspend = vega10_ih_suspend,
	.resume = vega10_ih_resume,
	.is_idle = vega10_ih_is_idle,
	.wait_for_idle = vega10_ih_wait_for_idle,
	.soft_reset = vega10_ih_soft_reset,
	.set_clockgating_state = vega10_ih_set_clockgating_state,
	.set_powergating_state = vega10_ih_set_powergating_state,
};

static const struct amdgpu_ih_funcs vega10_ih_funcs = {
	.get_wptr = vega10_ih_get_wptr,
<<<<<<< HEAD
	.prescreen_iv = vega10_ih_prescreen_iv,
	.decode_iv = vega10_ih_decode_iv,
=======
	.decode_iv = amdgpu_ih_decode_iv_helper,
>>>>>>> upstream/android-13
	.set_rptr = vega10_ih_set_rptr
};

static void vega10_ih_set_interrupt_funcs(struct amdgpu_device *adev)
{
<<<<<<< HEAD
	if (adev->irq.ih_funcs == NULL)
		adev->irq.ih_funcs = &vega10_ih_funcs;
=======
	adev->irq.ih_funcs = &vega10_ih_funcs;
>>>>>>> upstream/android-13
}

const struct amdgpu_ip_block_version vega10_ih_ip_block =
{
	.type = AMD_IP_BLOCK_TYPE_IH,
	.major = 4,
	.minor = 0,
	.rev = 0,
	.funcs = &vega10_ih_ip_funcs,
};
