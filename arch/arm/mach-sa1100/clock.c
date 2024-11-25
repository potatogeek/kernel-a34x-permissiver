// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/arch/arm/mach-sa1100/clock.c
 */
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/clk.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/io.h>
#include <linux/clkdev.h>
=======
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/clk-provider.h>
#include <linux/io.h>
#include <linux/spinlock.h>
>>>>>>> upstream/android-13

#include <mach/hardware.h>
#include <mach/generic.h>

<<<<<<< HEAD
struct clkops {
	void			(*enable)(struct clk *);
	void			(*disable)(struct clk *);
	unsigned long		(*get_rate)(struct clk *);
};

struct clk {
	const struct clkops	*ops;
	unsigned int		enabled;
};

#define DEFINE_CLK(_name, _ops)				\
struct clk clk_##_name = {				\
		.ops	= _ops,				\
	}

static DEFINE_SPINLOCK(clocks_lock);

/* Dummy clk routine to build generic kernel parts that may be using them */
long clk_round_rate(struct clk *clk, unsigned long rate)
{
	return clk_get_rate(clk);
}
EXPORT_SYMBOL(clk_round_rate);

int clk_set_rate(struct clk *clk, unsigned long rate)
{
	return 0;
}
EXPORT_SYMBOL(clk_set_rate);

int clk_set_parent(struct clk *clk, struct clk *parent)
{
	return 0;
}
EXPORT_SYMBOL(clk_set_parent);

struct clk *clk_get_parent(struct clk *clk)
{
	return NULL;
}
EXPORT_SYMBOL(clk_get_parent);

static void clk_gpio27_enable(struct clk *clk)
{
=======
static const char * const clk_tucr_parents[] = {
	"clk32768", "clk3686400",
};

static DEFINE_SPINLOCK(tucr_lock);

static int clk_gpio27_enable(struct clk_hw *hw)
{
	unsigned long flags;

>>>>>>> upstream/android-13
	/*
	 * First, set up the 3.6864MHz clock on GPIO 27 for the SA-1111:
	 * (SA-1110 Developer's Manual, section 9.1.2.1)
	 */
<<<<<<< HEAD
	GAFR |= GPIO_32_768kHz;
	GPDR |= GPIO_32_768kHz;
	TUCR = TUCR_3_6864MHz;
}

static void clk_gpio27_disable(struct clk *clk)
{
	TUCR = 0;
	GPDR &= ~GPIO_32_768kHz;
	GAFR &= ~GPIO_32_768kHz;
}

static void clk_cpu_enable(struct clk *clk)
{
}

static void clk_cpu_disable(struct clk *clk)
{
}

static unsigned long clk_cpu_get_rate(struct clk *clk)
=======
	local_irq_save(flags);
	GAFR |= GPIO_32_768kHz;
	GPDR |= GPIO_32_768kHz;
	local_irq_restore(flags);

	return 0;
}

static void clk_gpio27_disable(struct clk_hw *hw)
{
	unsigned long flags;

	local_irq_save(flags);
	GPDR &= ~GPIO_32_768kHz;
	GAFR &= ~GPIO_32_768kHz;
	local_irq_restore(flags);
}

static const struct clk_ops clk_gpio27_ops = {
	.enable = clk_gpio27_enable,
	.disable = clk_gpio27_disable,
};

static const char * const clk_gpio27_parents[] = {
	"tucr-mux",
};

static const struct clk_init_data clk_gpio27_init_data __initconst = {
	.name = "gpio27",
	.ops = &clk_gpio27_ops,
	.parent_names = clk_gpio27_parents,
	.num_parents = ARRAY_SIZE(clk_gpio27_parents),
};

/*
 * Derived from the table 8-1 in the SA1110 manual, the MPLL appears to
 * multiply its input rate by 4 x (4 + PPCR).  This calculation gives
 * the exact rate.  The figures given in the table are the rates rounded
 * to 100kHz.  Stick with sa11x0_getspeed() for the time being.
 */
static unsigned long clk_mpll_recalc_rate(struct clk_hw *hw,
	unsigned long prate)
>>>>>>> upstream/android-13
{
	return sa11x0_getspeed(0) * 1000;
}

<<<<<<< HEAD
int clk_enable(struct clk *clk)
{
	unsigned long flags;

	if (clk) {
		spin_lock_irqsave(&clocks_lock, flags);
		if (clk->enabled++ == 0)
			clk->ops->enable(clk);
		spin_unlock_irqrestore(&clocks_lock, flags);
	}

	return 0;
}
EXPORT_SYMBOL(clk_enable);

void clk_disable(struct clk *clk)
{
	unsigned long flags;

	if (clk) {
		WARN_ON(clk->enabled == 0);
		spin_lock_irqsave(&clocks_lock, flags);
		if (--clk->enabled == 0)
			clk->ops->disable(clk);
		spin_unlock_irqrestore(&clocks_lock, flags);
	}
}
EXPORT_SYMBOL(clk_disable);

unsigned long clk_get_rate(struct clk *clk)
{
	if (clk && clk->ops && clk->ops->get_rate)
		return clk->ops->get_rate(clk);

	return 0;
}
EXPORT_SYMBOL(clk_get_rate);

const struct clkops clk_gpio27_ops = {
	.enable		= clk_gpio27_enable,
	.disable	= clk_gpio27_disable,
};

const struct clkops clk_cpu_ops = {
	.enable		= clk_cpu_enable,
	.disable	= clk_cpu_disable,
	.get_rate	= clk_cpu_get_rate,
};

static DEFINE_CLK(gpio27, &clk_gpio27_ops);

static DEFINE_CLK(cpu, &clk_cpu_ops);

static unsigned long clk_36864_get_rate(struct clk *clk)
{
	return 3686400;
}

static struct clkops clk_36864_ops = {
	.enable		= clk_cpu_enable,
	.disable	= clk_cpu_disable,
	.get_rate	= clk_36864_get_rate,
};

static DEFINE_CLK(36864, &clk_36864_ops);

static struct clk_lookup sa11xx_clkregs[] = {
	CLKDEV_INIT("sa1111.0", NULL, &clk_gpio27),
	CLKDEV_INIT("sa1100-rtc", NULL, NULL),
	CLKDEV_INIT("sa11x0-fb", NULL, &clk_cpu),
	CLKDEV_INIT("sa11x0-pcmcia", NULL, &clk_cpu),
	CLKDEV_INIT("sa11x0-pcmcia.0", NULL, &clk_cpu),
	CLKDEV_INIT("sa11x0-pcmcia.1", NULL, &clk_cpu),
	/* sa1111 names devices using internal offsets, PCMCIA is at 0x1800 */
	CLKDEV_INIT("1800", NULL, &clk_cpu),
	CLKDEV_INIT(NULL, "OSTIMER0", &clk_36864),
=======
static const struct clk_ops clk_mpll_ops = {
	.recalc_rate = clk_mpll_recalc_rate,
};

static const char * const clk_mpll_parents[] = {
	"clk3686400",
};

static const struct clk_init_data clk_mpll_init_data __initconst = {
	.name = "mpll",
	.ops = &clk_mpll_ops,
	.parent_names = clk_mpll_parents,
	.num_parents = ARRAY_SIZE(clk_mpll_parents),
	.flags = CLK_GET_RATE_NOCACHE | CLK_IS_CRITICAL,
>>>>>>> upstream/android-13
};

int __init sa11xx_clk_init(void)
{
<<<<<<< HEAD
	clkdev_add_table(sa11xx_clkregs, ARRAY_SIZE(sa11xx_clkregs));
=======
	struct clk_hw *hw;
	int ret;

	hw = clk_hw_register_fixed_rate(NULL, "clk32768", NULL, 0, 32768);
	if (IS_ERR(hw))
		return PTR_ERR(hw);

	clk_hw_register_clkdev(hw, NULL, "sa1100-rtc");

	hw = clk_hw_register_fixed_rate(NULL, "clk3686400", NULL, 0, 3686400);
	if (IS_ERR(hw))
		return PTR_ERR(hw);

	clk_hw_register_clkdev(hw, "OSTIMER0", NULL);

	hw = kzalloc(sizeof(*hw), GFP_KERNEL);
	if (!hw)
		return -ENOMEM;
	hw->init = &clk_mpll_init_data;
	ret = clk_hw_register(NULL, hw);
	if (ret) {
		kfree(hw);
		return ret;
	}

	clk_hw_register_clkdev(hw, NULL, "sa11x0-fb");
	clk_hw_register_clkdev(hw, NULL, "sa11x0-pcmcia");
	clk_hw_register_clkdev(hw, NULL, "sa11x0-pcmcia.0");
	clk_hw_register_clkdev(hw, NULL, "sa11x0-pcmcia.1");
	clk_hw_register_clkdev(hw, NULL, "1800");

	hw = clk_hw_register_mux(NULL, "tucr-mux", clk_tucr_parents,
				 ARRAY_SIZE(clk_tucr_parents), 0,
				 (void __iomem *)&TUCR, FShft(TUCR_TSEL),
				 FAlnMsk(TUCR_TSEL), 0, &tucr_lock);
	clk_set_rate(hw->clk, 3686400);

	hw = kzalloc(sizeof(*hw), GFP_KERNEL);
	if (!hw)
		return -ENOMEM;
	hw->init = &clk_gpio27_init_data;
	ret = clk_hw_register(NULL, hw);
	if (ret) {
		kfree(hw);
		return ret;
	}

	clk_hw_register_clkdev(hw, NULL, "sa1111.0");

>>>>>>> upstream/android-13
	return 0;
}
