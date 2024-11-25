<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
*/
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2014 MediaTek Inc.
 * Author: James Liao <jamesjj.liao@mediatek.com>
 */
>>>>>>> upstream/android-13

#ifndef __DRV_CLK_GATE_H
#define __DRV_CLK_GATE_H

#include <linux/regmap.h>
#include <linux/clk-provider.h>

struct clk;

<<<<<<< HEAD
/*
 * define pwr status information.
 * including offsets/mask.
 */

struct pwr_status {
	u32 pwr_ofs;
	u32 pwr2_ofs;
	s32 other_ofs;
	u32 mask;
	u32 val;
};

=======
>>>>>>> upstream/android-13
struct mtk_clk_gate {
	struct clk_hw	hw;
	struct regmap	*regmap;
	int		set_ofs;
	int		clr_ofs;
	int		sta_ofs;
	u8		bit;
<<<<<<< HEAD
	struct pwr_status	*pwr_stat;
	struct regmap	*pwr_regmap;
=======
>>>>>>> upstream/android-13
};

static inline struct mtk_clk_gate *to_mtk_clk_gate(struct clk_hw *hw)
{
	return container_of(hw, struct mtk_clk_gate, hw);
}

extern const struct clk_ops mtk_clk_gate_ops_setclr;
<<<<<<< HEAD
extern const struct clk_ops mtk_clk_gate_ops_setclr_dummy;
extern const struct clk_ops mtk_clk_gate_ops_setclr_inv;
extern const struct clk_ops mtk_clk_gate_ops_setclr_inv_dummy;
=======
extern const struct clk_ops mtk_clk_gate_ops_setclr_inv;
>>>>>>> upstream/android-13
extern const struct clk_ops mtk_clk_gate_ops_no_setclr;
extern const struct clk_ops mtk_clk_gate_ops_no_setclr_inv;

struct clk *mtk_clk_register_gate(
		const char *name,
		const char *parent_name,
		struct regmap *regmap,
		int set_ofs,
		int clr_ofs,
		int sta_ofs,
		u8 bit,
		const struct clk_ops *ops,
		unsigned long flags,
<<<<<<< HEAD
		struct pwr_status *pwr_stat,
		struct regmap *pwr_regmap);
=======
		struct device *dev);
>>>>>>> upstream/android-13

#define GATE_MTK_FLAGS(_id, _name, _parent, _regs, _shift,	\
			_ops, _flags) {				\
		.id = _id,					\
		.name = _name,					\
		.parent_name = _parent,				\
		.regs = _regs,					\
		.shift = _shift,				\
		.ops = _ops,					\
		.flags = _flags,				\
	}

<<<<<<< HEAD
#define GATE_MTK(_id, _name, _parent, _regs, _shift, _ops)	\
	GATE_MTK_FLAGS(_id, _name, _parent, _regs, _shift, _ops, 0)

#define GATE_PWR_STAT(_pwr_ofs, _pwr2_ofs, _other_ofs, _mask, _val) {	\
		.pwr_ofs = _pwr_ofs,				\
		.pwr2_ofs = _pwr2_ofs,				\
		.other_ofs = _other_ofs,			\
		.mask = _mask,				\
		.val = _val,				\
}

=======
#define GATE_MTK(_id, _name, _parent, _regs, _shift, _ops)		\
	GATE_MTK_FLAGS(_id, _name, _parent, _regs, _shift, _ops, 0)

>>>>>>> upstream/android-13
#endif /* __DRV_CLK_GATE_H */
