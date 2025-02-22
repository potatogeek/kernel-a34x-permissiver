<<<<<<< HEAD
/*
 * Copyright (C) 2012-2015 - ARM Ltd
 * Author: Marc Zyngier <marc.zyngier@arm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2012-2015 - ARM Ltd
 * Author: Marc Zyngier <marc.zyngier@arm.com>
 */

#include <hyp/adjust_pc.h>

>>>>>>> upstream/android-13
#include <linux/compiler.h>
#include <linux/irqchip/arm-gic.h>
#include <linux/kvm_host.h>
#include <linux/swab.h>

#include <asm/kvm_emulate.h>
#include <asm/kvm_hyp.h>
#include <asm/kvm_mmu.h>

<<<<<<< HEAD
static bool __hyp_text __is_be(struct kvm_vcpu *vcpu)
{
	if (vcpu_mode_is_32bit(vcpu))
		return !!(read_sysreg_el2(spsr) & PSR_AA32_E_BIT);
=======
static bool __is_be(struct kvm_vcpu *vcpu)
{
	if (vcpu_mode_is_32bit(vcpu))
		return !!(read_sysreg_el2(SYS_SPSR) & PSR_AA32_E_BIT);
>>>>>>> upstream/android-13

	return !!(read_sysreg(SCTLR_EL1) & SCTLR_ELx_EE);
}

/*
 * __vgic_v2_perform_cpuif_access -- perform a GICV access on behalf of the
 *				     guest.
 *
 * @vcpu: the offending vcpu
 *
 * Returns:
 *  1: GICV access successfully performed
 *  0: Not a GICV access
<<<<<<< HEAD
 * -1: Illegal GICV access
 */
int __hyp_text __vgic_v2_perform_cpuif_access(struct kvm_vcpu *vcpu)
=======
 * -1: Illegal GICV access successfully performed
 */
int __vgic_v2_perform_cpuif_access(struct kvm_vcpu *vcpu)
>>>>>>> upstream/android-13
{
	struct kvm *kvm = kern_hyp_va(vcpu->kvm);
	struct vgic_dist *vgic = &kvm->arch.vgic;
	phys_addr_t fault_ipa;
	void __iomem *addr;
	int rd;

	/* Build the full address */
	fault_ipa  = kvm_vcpu_get_fault_ipa(vcpu);
	fault_ipa |= kvm_vcpu_get_hfar(vcpu) & GENMASK(11, 0);

	/* If not for GICV, move on */
	if (fault_ipa <  vgic->vgic_cpu_base ||
	    fault_ipa >= (vgic->vgic_cpu_base + KVM_VGIC_V2_CPU_SIZE))
		return 0;

	/* Reject anything but a 32bit access */
<<<<<<< HEAD
	if (kvm_vcpu_dabt_get_as(vcpu) != sizeof(u32))
		return -1;

	/* Not aligned? Don't bother */
	if (fault_ipa & 3)
		return -1;

	rd = kvm_vcpu_dabt_get_rd(vcpu);
	addr  = hyp_symbol_addr(kvm_vgic_global_state)->vcpu_hyp_va;
=======
	if (kvm_vcpu_dabt_get_as(vcpu) != sizeof(u32)) {
		__kvm_skip_instr(vcpu);
		return -1;
	}

	/* Not aligned? Don't bother */
	if (fault_ipa & 3) {
		__kvm_skip_instr(vcpu);
		return -1;
	}

	rd = kvm_vcpu_dabt_get_rd(vcpu);
	addr  = kvm_vgic_global_state.vcpu_hyp_va;
>>>>>>> upstream/android-13
	addr += fault_ipa - vgic->vgic_cpu_base;

	if (kvm_vcpu_dabt_iswrite(vcpu)) {
		u32 data = vcpu_get_reg(vcpu, rd);
		if (__is_be(vcpu)) {
			/* guest pre-swabbed data, undo this for writel() */
<<<<<<< HEAD
			data = swab32(data);
=======
			data = __kvm_swab32(data);
>>>>>>> upstream/android-13
		}
		writel_relaxed(data, addr);
	} else {
		u32 data = readl_relaxed(addr);
		if (__is_be(vcpu)) {
			/* guest expects swabbed data */
<<<<<<< HEAD
			data = swab32(data);
=======
			data = __kvm_swab32(data);
>>>>>>> upstream/android-13
		}
		vcpu_set_reg(vcpu, rd, data);
	}

<<<<<<< HEAD
=======
	__kvm_skip_instr(vcpu);

>>>>>>> upstream/android-13
	return 1;
}
