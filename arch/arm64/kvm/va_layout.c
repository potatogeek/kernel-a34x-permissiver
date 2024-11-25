<<<<<<< HEAD
/*
 * Copyright (C) 2017 ARM Ltd.
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
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2017 ARM Ltd.
 * Author: Marc Zyngier <marc.zyngier@arm.com>
>>>>>>> upstream/android-13
 */

#include <linux/kvm_host.h>
#include <linux/random.h>
#include <linux/memblock.h>
#include <asm/alternative.h>
#include <asm/debug-monitors.h>
#include <asm/insn.h>
#include <asm/kvm_mmu.h>
<<<<<<< HEAD

/*
 * The LSB of the random hyp VA tag or 0 if no randomization is used.
 */
static u8 tag_lsb;
/*
 * The random hyp VA tag value with the region bit if hyp randomization is used
=======
#include <asm/memory.h>

/*
 * The LSB of the HYP VA tag
 */
static u8 tag_lsb;
/*
 * The HYP VA tag value with the region bit
>>>>>>> upstream/android-13
 */
static u64 tag_val;
static u64 va_mask;

<<<<<<< HEAD
static void compute_layout(void)
{
	phys_addr_t idmap_addr = __pa_symbol(__hyp_idmap_text_start);
	u64 hyp_va_msb;
	int kva_msb;

	/* Where is my RAM region? */
	hyp_va_msb  = idmap_addr & BIT(VA_BITS - 1);
	hyp_va_msb ^= BIT(VA_BITS - 1);

	kva_msb = fls64((u64)phys_to_virt(memblock_start_of_DRAM()) ^
			(u64)(high_memory - 1));

	if (kva_msb == (VA_BITS - 1)) {
		/*
		 * No space in the address, let's compute the mask so
		 * that it covers (VA_BITS - 1) bits, and the region
		 * bit. The tag stays set to zero.
		 */
		va_mask  = BIT(VA_BITS - 1) - 1;
		va_mask |= hyp_va_msb;
	} else {
		/*
		 * We do have some free bits to insert a random tag.
		 * Hyp VAs are now created from kernel linear map VAs
		 * using the following formula (with V == VA_BITS):
		 *
		 *  63 ... V |     V-1    | V-2 .. tag_lsb | tag_lsb - 1 .. 0
		 *  ---------------------------------------------------------
		 * | 0000000 | hyp_va_msb |    random tag  |  kern linear VA |
		 */
		tag_lsb = kva_msb;
		va_mask = GENMASK_ULL(tag_lsb - 1, 0);
		tag_val = get_random_long() & GENMASK_ULL(VA_BITS - 2, tag_lsb);
		tag_val |= hyp_va_msb;
		tag_val >>= tag_lsb;
=======
/*
 * Compute HYP VA by using the same computation as kern_hyp_va().
 */
static u64 __early_kern_hyp_va(u64 addr)
{
	addr &= va_mask;
	addr |= tag_val << tag_lsb;
	return addr;
}

/*
 * Store a hyp VA <-> PA offset into a EL2-owned variable.
 */
static void init_hyp_physvirt_offset(void)
{
	u64 kern_va, hyp_va;

	/* Compute the offset from the hyp VA and PA of a random symbol. */
	kern_va = (u64)lm_alias(__hyp_text_start);
	hyp_va = __early_kern_hyp_va(kern_va);
	hyp_physvirt_offset = (s64)__pa(kern_va) - (s64)hyp_va;
}

/*
 * We want to generate a hyp VA with the following format (with V ==
 * vabits_actual):
 *
 *  63 ... V |     V-1    | V-2 .. tag_lsb | tag_lsb - 1 .. 0
 *  ---------------------------------------------------------
 * | 0000000 | hyp_va_msb |   random tag   |  kern linear VA |
 *           |--------- tag_val -----------|----- va_mask ---|
 *
 * which does not conflict with the idmap regions.
 */
__init void kvm_compute_layout(void)
{
	phys_addr_t idmap_addr = __pa_symbol(__hyp_idmap_text_start);
	u64 hyp_va_msb;

	/* Where is my RAM region? */
	hyp_va_msb  = idmap_addr & BIT(vabits_actual - 1);
	hyp_va_msb ^= BIT(vabits_actual - 1);

	tag_lsb = fls64((u64)phys_to_virt(memblock_start_of_DRAM()) ^
			(u64)(high_memory - 1));

	va_mask = GENMASK_ULL(tag_lsb - 1, 0);
	tag_val = hyp_va_msb;

	if (IS_ENABLED(CONFIG_RANDOMIZE_BASE) && tag_lsb != (vabits_actual - 1)) {
		/* We have some free bits to insert a random tag. */
		tag_val |= get_random_long() & GENMASK_ULL(vabits_actual - 2, tag_lsb);
	}
	tag_val >>= tag_lsb;

	init_hyp_physvirt_offset();
}

/*
 * The .hyp.reloc ELF section contains a list of kimg positions that
 * contains kimg VAs but will be accessed only in hyp execution context.
 * Convert them to hyp VAs. See gen-hyprel.c for more details.
 */
__init void kvm_apply_hyp_relocations(void)
{
	int32_t *rel;
	int32_t *begin = (int32_t *)__hyp_reloc_begin;
	int32_t *end = (int32_t *)__hyp_reloc_end;

	for (rel = begin; rel < end; ++rel) {
		uintptr_t *ptr, kimg_va;

		/*
		 * Each entry contains a 32-bit relative offset from itself
		 * to a kimg VA position.
		 */
		ptr = (uintptr_t *)lm_alias((char *)rel + *rel);

		/* Read the kimg VA value at the relocation address. */
		kimg_va = *ptr;

		/* Convert to hyp VA and store back to the relocation address. */
		*ptr = __early_kern_hyp_va((uintptr_t)lm_alias(kimg_va));
>>>>>>> upstream/android-13
	}
}

static u32 compute_instruction(int n, u32 rd, u32 rn)
{
	u32 insn = AARCH64_BREAK_FAULT;

	switch (n) {
	case 0:
		insn = aarch64_insn_gen_logical_immediate(AARCH64_INSN_LOGIC_AND,
							  AARCH64_INSN_VARIANT_64BIT,
							  rn, rd, va_mask);
		break;

	case 1:
		/* ROR is a variant of EXTR with Rm = Rn */
		insn = aarch64_insn_gen_extr(AARCH64_INSN_VARIANT_64BIT,
					     rn, rn, rd,
					     tag_lsb);
		break;

	case 2:
		insn = aarch64_insn_gen_add_sub_imm(rd, rn,
						    tag_val & GENMASK(11, 0),
						    AARCH64_INSN_VARIANT_64BIT,
						    AARCH64_INSN_ADSB_ADD);
		break;

	case 3:
		insn = aarch64_insn_gen_add_sub_imm(rd, rn,
						    tag_val & GENMASK(23, 12),
						    AARCH64_INSN_VARIANT_64BIT,
						    AARCH64_INSN_ADSB_ADD);
		break;

	case 4:
		/* ROR is a variant of EXTR with Rm = Rn */
		insn = aarch64_insn_gen_extr(AARCH64_INSN_VARIANT_64BIT,
					     rn, rn, rd, 64 - tag_lsb);
		break;
	}

	return insn;
}

void __init kvm_update_va_mask(struct alt_instr *alt,
			       __le32 *origptr, __le32 *updptr, int nr_inst)
{
	int i;

	BUG_ON(nr_inst != 5);

<<<<<<< HEAD
	if (!has_vhe() && !va_mask)
		compute_layout();

=======
>>>>>>> upstream/android-13
	for (i = 0; i < nr_inst; i++) {
		u32 rd, rn, insn, oinsn;

		/*
		 * VHE doesn't need any address translation, let's NOP
		 * everything.
		 *
<<<<<<< HEAD
		 * Alternatively, if we don't have any spare bits in
		 * the address, NOP everything after masking that
		 * kernel VA.
		 */
		if (has_vhe() || (!tag_lsb && i > 0)) {
=======
		 * Alternatively, if the tag is zero (because the layout
		 * dictates it and we don't have any spare bits in the
		 * address), NOP everything after masking the kernel VA.
		 */
		if (has_vhe() || (!tag_val && i > 0)) {
>>>>>>> upstream/android-13
			updptr[i] = cpu_to_le32(aarch64_insn_gen_nop());
			continue;
		}

		oinsn = le32_to_cpu(origptr[i]);
		rd = aarch64_insn_decode_register(AARCH64_INSN_REGTYPE_RD, oinsn);
		rn = aarch64_insn_decode_register(AARCH64_INSN_REGTYPE_RN, oinsn);

		insn = compute_instruction(i, rd, rn);
		BUG_ON(insn == AARCH64_BREAK_FAULT);

		updptr[i] = cpu_to_le32(insn);
	}
}

<<<<<<< HEAD
void *__kvm_bp_vect_base;
int __kvm_harden_el2_vector_slot;

=======
>>>>>>> upstream/android-13
void kvm_patch_vector_branch(struct alt_instr *alt,
			     __le32 *origptr, __le32 *updptr, int nr_inst)
{
	u64 addr;
	u32 insn;

<<<<<<< HEAD
	BUG_ON(nr_inst != 5);

	if (has_vhe() || !cpus_have_const_cap(ARM64_HARDEN_EL2_VECTORS)) {
		WARN_ON_ONCE(cpus_have_const_cap(ARM64_HARDEN_EL2_VECTORS));
		return;
	}

	if (!va_mask)
		compute_layout();
=======
	BUG_ON(nr_inst != 4);

	if (!cpus_have_const_cap(ARM64_SPECTRE_V3A) || WARN_ON_ONCE(has_vhe()))
		return;
>>>>>>> upstream/android-13

	/*
	 * Compute HYP VA by using the same computation as kern_hyp_va()
	 */
<<<<<<< HEAD
	addr = (uintptr_t)kvm_ksym_ref(__kvm_hyp_vector);
	addr &= va_mask;
	addr |= tag_val << tag_lsb;
=======
	addr = __early_kern_hyp_va((u64)kvm_ksym_ref(__kvm_hyp_vector));
>>>>>>> upstream/android-13

	/* Use PC[10:7] to branch to the same vector in KVM */
	addr |= ((u64)origptr & GENMASK_ULL(10, 7));

	/*
<<<<<<< HEAD
	 * Branch to the second instruction in the vectors in order to
	 * avoid the initial store on the stack (which we already
	 * perform in the hardening vectors).
	 */
	addr += AARCH64_INSN_SIZE;

	/* stp x0, x1, [sp, #-16]! */
	insn = aarch64_insn_gen_load_store_pair(AARCH64_INSN_REG_0,
						AARCH64_INSN_REG_1,
						AARCH64_INSN_REG_SP,
						-16,
						AARCH64_INSN_VARIANT_64BIT,
						AARCH64_INSN_LDST_STORE_PAIR_PRE_INDEX);
	*updptr++ = cpu_to_le32(insn);
=======
	 * Branch over the preamble in order to avoid the initial store on
	 * the stack (which we already perform in the hardening vectors).
	 */
	addr += KVM_VECTOR_PREAMBLE;
>>>>>>> upstream/android-13

	/* movz x0, #(addr & 0xffff) */
	insn = aarch64_insn_gen_movewide(AARCH64_INSN_REG_0,
					 (u16)addr,
					 0,
					 AARCH64_INSN_VARIANT_64BIT,
					 AARCH64_INSN_MOVEWIDE_ZERO);
	*updptr++ = cpu_to_le32(insn);

	/* movk x0, #((addr >> 16) & 0xffff), lsl #16 */
	insn = aarch64_insn_gen_movewide(AARCH64_INSN_REG_0,
					 (u16)(addr >> 16),
					 16,
					 AARCH64_INSN_VARIANT_64BIT,
					 AARCH64_INSN_MOVEWIDE_KEEP);
	*updptr++ = cpu_to_le32(insn);

	/* movk x0, #((addr >> 32) & 0xffff), lsl #32 */
	insn = aarch64_insn_gen_movewide(AARCH64_INSN_REG_0,
					 (u16)(addr >> 32),
					 32,
					 AARCH64_INSN_VARIANT_64BIT,
					 AARCH64_INSN_MOVEWIDE_KEEP);
	*updptr++ = cpu_to_le32(insn);

	/* br x0 */
	insn = aarch64_insn_gen_branch_reg(AARCH64_INSN_REG_0,
					   AARCH64_INSN_BRANCH_NOLINK);
	*updptr++ = cpu_to_le32(insn);
}
<<<<<<< HEAD
=======

static void generate_mov_q(u64 val, __le32 *origptr, __le32 *updptr, int nr_inst)
{
	u32 insn, oinsn, rd;

	BUG_ON(nr_inst != 4);

	/* Compute target register */
	oinsn = le32_to_cpu(*origptr);
	rd = aarch64_insn_decode_register(AARCH64_INSN_REGTYPE_RD, oinsn);

	/* movz rd, #(val & 0xffff) */
	insn = aarch64_insn_gen_movewide(rd,
					 (u16)val,
					 0,
					 AARCH64_INSN_VARIANT_64BIT,
					 AARCH64_INSN_MOVEWIDE_ZERO);
	*updptr++ = cpu_to_le32(insn);

	/* movk rd, #((val >> 16) & 0xffff), lsl #16 */
	insn = aarch64_insn_gen_movewide(rd,
					 (u16)(val >> 16),
					 16,
					 AARCH64_INSN_VARIANT_64BIT,
					 AARCH64_INSN_MOVEWIDE_KEEP);
	*updptr++ = cpu_to_le32(insn);

	/* movk rd, #((val >> 32) & 0xffff), lsl #32 */
	insn = aarch64_insn_gen_movewide(rd,
					 (u16)(val >> 32),
					 32,
					 AARCH64_INSN_VARIANT_64BIT,
					 AARCH64_INSN_MOVEWIDE_KEEP);
	*updptr++ = cpu_to_le32(insn);

	/* movk rd, #((val >> 48) & 0xffff), lsl #48 */
	insn = aarch64_insn_gen_movewide(rd,
					 (u16)(val >> 48),
					 48,
					 AARCH64_INSN_VARIANT_64BIT,
					 AARCH64_INSN_MOVEWIDE_KEEP);
	*updptr++ = cpu_to_le32(insn);
}

void kvm_get_kimage_voffset(struct alt_instr *alt,
			    __le32 *origptr, __le32 *updptr, int nr_inst)
{
	generate_mov_q(kimage_voffset, origptr, updptr, nr_inst);
}

void kvm_compute_final_ctr_el0(struct alt_instr *alt,
			       __le32 *origptr, __le32 *updptr, int nr_inst)
{
	generate_mov_q(read_sanitised_ftr_reg(SYS_CTR_EL0),
		       origptr, updptr, nr_inst);
}
>>>>>>> upstream/android-13
