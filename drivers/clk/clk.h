<<<<<<< HEAD
/*
 * linux/drivers/clk/clk.h
 *
 * Copyright (C) 2013 Samsung Electronics Co., Ltd.
 * Sylwester Nawrocki <s.nawrocki@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

struct clk_hw;

#if defined(CONFIG_OF) && defined(CONFIG_COMMON_CLK)
struct clk *__of_clk_get_from_provider(struct of_phandle_args *clkspec,
				       const char *dev_id, const char *con_id);
#endif

#ifdef CONFIG_COMMON_CLK
struct clk *__clk_create_clk(struct clk_hw *hw, const char *dev_id,
			     const char *con_id);
void __clk_free_clk(struct clk *clk);
int __clk_get(struct clk *clk);
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2013 Samsung Electronics Co., Ltd.
 * Sylwester Nawrocki <s.nawrocki@samsung.com>
 */

struct clk_hw;
struct device;
struct of_phandle_args;

#if defined(CONFIG_OF) && defined(CONFIG_COMMON_CLK)
struct clk_hw *of_clk_get_hw(struct device_node *np,
				    int index, const char *con_id);
#else /* !CONFIG_COMMON_CLK || !CONFIG_OF */
static inline struct clk_hw *of_clk_get_hw(struct device_node *np,
				    int index, const char *con_id)
{
	return ERR_PTR(-ENOENT);
}
#endif

struct clk_hw *clk_find_hw(const char *dev_id, const char *con_id);

#ifdef CONFIG_COMMON_CLK
struct clk *clk_hw_create_clk(struct device *dev, struct clk_hw *hw,
			      const char *dev_id, const char *con_id);
>>>>>>> upstream/android-13
void __clk_put(struct clk *clk);
#else
/* All these casts to avoid ifdefs in clkdev... */
static inline struct clk *
<<<<<<< HEAD
__clk_create_clk(struct clk_hw *hw, const char *dev_id, const char *con_id)
{
	return (struct clk *)hw;
}
static inline void __clk_free_clk(struct clk *clk) { }
static struct clk_hw *__clk_get_hw(struct clk *clk)
{
	return (struct clk_hw *)clk;
}
static inline int __clk_get(struct clk *clk) { return 1; }
=======
clk_hw_create_clk(struct device *dev, struct clk_hw *hw, const char *dev_id,
		  const char *con_id)
{
	return (struct clk *)hw;
}
>>>>>>> upstream/android-13
static inline void __clk_put(struct clk *clk) { }

#endif
