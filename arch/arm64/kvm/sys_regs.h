<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2012,2013 - ARM Ltd
 * Author: Marc Zyngier <marc.zyngier@arm.com>
 *
 * Derived from arch/arm/kvm/coproc.h
 * Copyright (C) 2012 - Virtual Open Systems and Columbia University
 * Authors: Christoffer Dall <c.dall@virtualopensystems.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
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
>>>>>>> upstream/android-13
 */

#ifndef __ARM64_KVM_SYS_REGS_LOCAL_H__
#define __ARM64_KVM_SYS_REGS_LOCAL_H__

<<<<<<< HEAD
=======
#include <linux/bsearch.h>

#define reg_to_encoding(x)						\
	sys_reg((u32)(x)->Op0, (u32)(x)->Op1,				\
		(u32)(x)->CRn, (u32)(x)->CRm, (u32)(x)->Op2)

>>>>>>> upstream/android-13
struct sys_reg_params {
	u8	Op0;
	u8	Op1;
	u8	CRn;
	u8	CRm;
	u8	Op2;
	u64	regval;
	bool	is_write;
<<<<<<< HEAD
	bool	is_aarch32;
	bool	is_32bit;	/* Only valid if is_aarch32 is true */
};

struct sys_reg_desc {
=======
};

#define esr_sys64_to_params(esr)                                               \
	((struct sys_reg_params){ .Op0 = ((esr) >> 20) & 3,                    \
				  .Op1 = ((esr) >> 14) & 0x7,                  \
				  .CRn = ((esr) >> 10) & 0xf,                  \
				  .CRm = ((esr) >> 1) & 0xf,                   \
				  .Op2 = ((esr) >> 17) & 0x7,                  \
				  .is_write = !((esr) & 1) })

struct sys_reg_desc {
	/* Sysreg string for debug */
	const char *name;

	enum {
		AA32_ZEROHIGH,
		AA32_LO,
		AA32_HI,
	} aarch32_map;

>>>>>>> upstream/android-13
	/* MRS/MSR instruction which accesses it. */
	u8	Op0;
	u8	Op1;
	u8	CRn;
	u8	CRm;
	u8	Op2;

	/* Trapped access from guest, if non-NULL. */
	bool (*access)(struct kvm_vcpu *,
		       struct sys_reg_params *,
		       const struct sys_reg_desc *);

	/* Initialization for vcpu. */
	void (*reset)(struct kvm_vcpu *, const struct sys_reg_desc *);

	/* Index into sys_reg[], or 0 if we don't need to save it. */
	int reg;

	/* Value (usually reset value) */
	u64 val;

	/* Custom get/set_user functions, fallback to generic if NULL */
	int (*get_user)(struct kvm_vcpu *vcpu, const struct sys_reg_desc *rd,
			const struct kvm_one_reg *reg, void __user *uaddr);
	int (*set_user)(struct kvm_vcpu *vcpu, const struct sys_reg_desc *rd,
			const struct kvm_one_reg *reg, void __user *uaddr);
<<<<<<< HEAD
};

static inline void print_sys_reg_instr(const struct sys_reg_params *p)
{
	/* Look, we even formatted it for you to paste into the table! */
	kvm_pr_unimpl(" { Op0(%2u), Op1(%2u), CRn(%2u), CRm(%2u), Op2(%2u), func_%s },\n",
		      p->Op0, p->Op1, p->CRn, p->CRm, p->Op2, p->is_write ? "write" : "read");
=======

	/* Return mask of REG_* runtime visibility overrides */
	unsigned int (*visibility)(const struct kvm_vcpu *vcpu,
				   const struct sys_reg_desc *rd);
};

#define REG_HIDDEN		(1 << 0) /* hidden from userspace and guest */
#define REG_RAZ			(1 << 1) /* RAZ from userspace and guest */

static __printf(2, 3)
inline void print_sys_reg_msg(const struct sys_reg_params *p,
				       char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	/* Look, we even formatted it for you to paste into the table! */
	kvm_pr_unimpl("%pV { Op0(%2u), Op1(%2u), CRn(%2u), CRm(%2u), Op2(%2u), func_%s },\n",
		      &(struct va_format){ fmt, &va },
		      p->Op0, p->Op1, p->CRn, p->CRm, p->Op2, p->is_write ? "write" : "read");
	va_end(va);
}

static inline void print_sys_reg_instr(const struct sys_reg_params *p)
{
	/* GCC warns on an empty format string */
	print_sys_reg_msg(p, "%s", "");
>>>>>>> upstream/android-13
}

static inline bool ignore_write(struct kvm_vcpu *vcpu,
				const struct sys_reg_params *p)
{
	return true;
}

static inline bool read_zero(struct kvm_vcpu *vcpu,
			     struct sys_reg_params *p)
{
	p->regval = 0;
	return true;
}

/* Reset functions */
static inline void reset_unknown(struct kvm_vcpu *vcpu,
				 const struct sys_reg_desc *r)
{
	BUG_ON(!r->reg);
	BUG_ON(r->reg >= NR_SYS_REGS);
	__vcpu_sys_reg(vcpu, r->reg) = 0x1de7ec7edbadc0deULL;
}

static inline void reset_val(struct kvm_vcpu *vcpu, const struct sys_reg_desc *r)
{
	BUG_ON(!r->reg);
	BUG_ON(r->reg >= NR_SYS_REGS);
	__vcpu_sys_reg(vcpu, r->reg) = r->val;
}

<<<<<<< HEAD
=======
static inline bool sysreg_hidden(const struct kvm_vcpu *vcpu,
				 const struct sys_reg_desc *r)
{
	if (likely(!r->visibility))
		return false;

	return r->visibility(vcpu, r) & REG_HIDDEN;
}

static inline bool sysreg_visible_as_raz(const struct kvm_vcpu *vcpu,
					 const struct sys_reg_desc *r)
{
	if (likely(!r->visibility))
		return false;

	return r->visibility(vcpu, r) & REG_RAZ;
}

>>>>>>> upstream/android-13
static inline int cmp_sys_reg(const struct sys_reg_desc *i1,
			      const struct sys_reg_desc *i2)
{
	BUG_ON(i1 == i2);
	if (!i1)
		return 1;
	else if (!i2)
		return -1;
	if (i1->Op0 != i2->Op0)
		return i1->Op0 - i2->Op0;
	if (i1->Op1 != i2->Op1)
		return i1->Op1 - i2->Op1;
	if (i1->CRn != i2->CRn)
		return i1->CRn - i2->CRn;
	if (i1->CRm != i2->CRm)
		return i1->CRm - i2->CRm;
	return i1->Op2 - i2->Op2;
}

<<<<<<< HEAD
=======
static inline int match_sys_reg(const void *key, const void *elt)
{
	const unsigned long pval = (unsigned long)key;
	const struct sys_reg_desc *r = elt;

	return pval - reg_to_encoding(r);
}

static inline const struct sys_reg_desc *
find_reg(const struct sys_reg_params *params, const struct sys_reg_desc table[],
	 unsigned int num)
{
	unsigned long pval = reg_to_encoding(params);

	return __inline_bsearch((void *)pval, table, num, sizeof(table[0]), match_sys_reg);
}

static inline u64 calculate_mpidr(const struct kvm_vcpu *vcpu)
{
	u64 mpidr;

	/*
	 * Map the vcpu_id into the first three affinity level fields of
	 * the MPIDR. We limit the number of VCPUs in level 0 due to a
	 * limitation to 16 CPUs in that level in the ICC_SGIxR registers
	 * of the GICv3 to be able to address each CPU directly when
	 * sending IPIs.
	 */
	mpidr = (vcpu->vcpu_id & 0x0f) << MPIDR_LEVEL_SHIFT(0);
	mpidr |= ((vcpu->vcpu_id >> 4) & 0xff) << MPIDR_LEVEL_SHIFT(1);
	mpidr |= ((vcpu->vcpu_id >> 12) & 0xff) << MPIDR_LEVEL_SHIFT(2);
	mpidr |= (1ULL << 31);

	return mpidr;
}

>>>>>>> upstream/android-13
const struct sys_reg_desc *find_reg_by_id(u64 id,
					  struct sys_reg_params *params,
					  const struct sys_reg_desc table[],
					  unsigned int num);

<<<<<<< HEAD
=======
#define AA32(_x)	.aarch32_map = AA32_##_x
>>>>>>> upstream/android-13
#define Op0(_x) 	.Op0 = _x
#define Op1(_x) 	.Op1 = _x
#define CRn(_x)		.CRn = _x
#define CRm(_x) 	.CRm = _x
#define Op2(_x) 	.Op2 = _x

#define SYS_DESC(reg)					\
<<<<<<< HEAD
=======
	.name = #reg,					\
>>>>>>> upstream/android-13
	Op0(sys_reg_Op0(reg)), Op1(sys_reg_Op1(reg)),	\
	CRn(sys_reg_CRn(reg)), CRm(sys_reg_CRm(reg)),	\
	Op2(sys_reg_Op2(reg))

#endif /* __ARM64_KVM_SYS_REGS_LOCAL_H__ */
