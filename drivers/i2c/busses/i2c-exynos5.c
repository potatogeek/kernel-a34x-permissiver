<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/**
 * i2c-exynos5.c - Samsung Exynos5 I2C Controller Driver
 *
 * Copyright (C) 2013 Samsung Electronics Co., Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
*/

#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/i2c.h>
<<<<<<< HEAD
=======
#include <linux/init.h>
>>>>>>> upstream/android-13
#include <linux/time.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
=======
#include <linux/pm_runtime.h>
>>>>>>> upstream/android-13
#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
<<<<<<< HEAD
#include <linux/spinlock.h>
=======
#include <linux/of_gpio.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>
#include "../../pinctrl/core.h"

#ifdef CONFIG_CPU_IDLE
#include <soc/samsung/exynos-cpupm.h>
#endif
>>>>>>> upstream/android-13

/*
 * HSI2C controller from Samsung supports 2 modes of operation
 * 1. Auto mode: Where in master automatically controls the whole transaction
 * 2. Manual mode: Software controls the transaction by issuing commands
 *    START, READ, WRITE, STOP, RESTART in I2C_MANUAL_CMD register.
 *
 * Operation mode can be selected by setting AUTO_MODE bit in I2C_CONF register
 *
 * Special bits are available for both modes of operation to set commands
 * and for checking transfer status
 */

/* Register Map */
#define HSI2C_CTL		0x00
#define HSI2C_FIFO_CTL		0x04
#define HSI2C_TRAILIG_CTL	0x08
#define HSI2C_CLK_CTL		0x0C
#define HSI2C_CLK_SLOT		0x10
#define HSI2C_INT_ENABLE	0x20
#define HSI2C_INT_STATUS	0x24
#define HSI2C_ERR_STATUS	0x2C
#define HSI2C_FIFO_STATUS	0x30
#define HSI2C_TX_DATA		0x34
#define HSI2C_RX_DATA		0x38
#define HSI2C_CONF		0x40
#define HSI2C_AUTO_CONF		0x44
#define HSI2C_TIMEOUT		0x48
#define HSI2C_MANUAL_CMD	0x4C
#define HSI2C_TRANS_STATUS	0x50
#define HSI2C_TIMING_HS1	0x54
#define HSI2C_TIMING_HS2	0x58
#define HSI2C_TIMING_HS3	0x5C
#define HSI2C_TIMING_FS1	0x60
#define HSI2C_TIMING_FS2	0x64
#define HSI2C_TIMING_FS3	0x68
#define HSI2C_TIMING_SLA	0x6C
#define HSI2C_ADDR		0x70

/* I2C_CTL Register bits */
#define HSI2C_FUNC_MODE_I2C			(1u << 0)
#define HSI2C_MASTER				(1u << 3)
#define HSI2C_RXCHON				(1u << 6)
#define HSI2C_TXCHON				(1u << 7)
<<<<<<< HEAD
=======
#define HSI2C_NO_LOSE_ARBITRATION		(1u << 22)
#define HSI2C_EXT_MSB				(1u << 29)
#define HSI2C_EXT_ADDR				(1u << 30)
>>>>>>> upstream/android-13
#define HSI2C_SW_RST				(1u << 31)

/* I2C_FIFO_CTL Register bits */
#define HSI2C_RXFIFO_EN				(1u << 0)
#define HSI2C_TXFIFO_EN				(1u << 1)
<<<<<<< HEAD
#define HSI2C_RXFIFO_TRIGGER_LEVEL(x)		((x) << 4)
#define HSI2C_TXFIFO_TRIGGER_LEVEL(x)		((x) << 16)

/* I2C_TRAILING_CTL Register bits */
#define HSI2C_TRAILING_COUNT			(0xf)
=======
#define HSI2C_FIFO_MAX				(0x40)
#define HSI2C_RXFIFO_TRIGGER_LEVEL(x)	(x << 4)
#define HSI2C_TXFIFO_TRIGGER_LEVEL(x)	(x << 16)
/* I2C_TRAILING_CTL Register bits */
#define HSI2C_TRAILING_COUNT			(0xffffff)
>>>>>>> upstream/android-13

/* I2C_INT_EN Register bits */
#define HSI2C_INT_TX_ALMOSTEMPTY_EN		(1u << 0)
#define HSI2C_INT_RX_ALMOSTFULL_EN		(1u << 1)
#define HSI2C_INT_TRAILING_EN			(1u << 6)
<<<<<<< HEAD
=======
#define HSI2C_INT_TRANSFER_DONE			(1u << 7)
#define HSI2C_INT_I2C_EN			(1u << 9)
#define HSI2C_INT_CHK_TRANS_STATE		(0xf << 8)
>>>>>>> upstream/android-13

/* I2C_INT_STAT Register bits */
#define HSI2C_INT_TX_ALMOSTEMPTY		(1u << 0)
#define HSI2C_INT_RX_ALMOSTFULL			(1u << 1)
#define HSI2C_INT_TX_UNDERRUN			(1u << 2)
#define HSI2C_INT_TX_OVERRUN			(1u << 3)
#define HSI2C_INT_RX_UNDERRUN			(1u << 4)
#define HSI2C_INT_RX_OVERRUN			(1u << 5)
#define HSI2C_INT_TRAILING			(1u << 6)
#define HSI2C_INT_I2C				(1u << 9)
<<<<<<< HEAD

#define HSI2C_INT_TRANS_DONE			(1u << 7)
#define HSI2C_INT_TRANS_ABORT			(1u << 8)
#define HSI2C_INT_NO_DEV_ACK			(1u << 9)
#define HSI2C_INT_NO_DEV			(1u << 10)
#define HSI2C_INT_TIMEOUT			(1u << 11)
#define HSI2C_INT_I2C_TRANS			(HSI2C_INT_TRANS_DONE |	\
						HSI2C_INT_TRANS_ABORT |	\
						HSI2C_INT_NO_DEV_ACK |	\
						HSI2C_INT_NO_DEV |	\
						HSI2C_INT_TIMEOUT)
=======
#define HSI2C_INT_NODEV				(1u << 10)
#define HSI2C_RX_INT				(HSI2C_INT_RX_ALMOSTFULL | \
						 HSI2C_INT_RX_UNDERRUN | \
						 HSI2C_INT_RX_OVERRUN | \
						 HSI2C_INT_TRAILING)
>>>>>>> upstream/android-13

/* I2C_FIFO_STAT Register bits */
#define HSI2C_RX_FIFO_EMPTY			(1u << 24)
#define HSI2C_RX_FIFO_FULL			(1u << 23)
#define HSI2C_RX_FIFO_LVL(x)			((x >> 16) & 0x7f)
<<<<<<< HEAD
#define HSI2C_TX_FIFO_EMPTY			(1u << 8)
#define HSI2C_TX_FIFO_FULL			(1u << 7)
#define HSI2C_TX_FIFO_LVL(x)			((x >> 0) & 0x7f)
=======
#define HSI2C_RX_FIFO_LVL_MASK			(0x7F << 16)
#define HSI2C_TX_FIFO_EMPTY			(1u << 8)
#define HSI2C_TX_FIFO_FULL			(1u << 7)
#define HSI2C_TX_FIFO_LVL(x)			((x >> 0) & 0x7f)
#define HSI2C_TX_FIFO_LVL_MASK			(0x7F << 0)
#define HSI2C_FIFO_EMPTY			(HSI2C_RX_FIFO_EMPTY |	\
						HSI2C_TX_FIFO_EMPTY)
>>>>>>> upstream/android-13

/* I2C_CONF Register bits */
#define HSI2C_AUTO_MODE				(1u << 31)
#define HSI2C_10BIT_ADDR_MODE			(1u << 30)
#define HSI2C_HS_MODE				(1u << 29)
<<<<<<< HEAD
=======
#define HSI2C_FILTER_EN_SCL			(1u << 28)
#define HSI2C_FILTER_EN_SDA			(1u << 27)
#define HSI2C_FTL_CYCLE_SCL_MASK		(0xf << 16)
#define HSI2C_FTL_CYCLE_SCL(x)			((x & 0xf) << 16)
#define HSI2C_FTL_CYCLE_SDA_MASK		(0xf << 12)
#define HSI2C_FTL_CYCLE_SDA(x)			((x & 0xf) << 12)
>>>>>>> upstream/android-13

/* I2C_AUTO_CONF Register bits */
#define HSI2C_READ_WRITE			(1u << 16)
#define HSI2C_STOP_AFTER_TRANS			(1u << 17)
#define HSI2C_MASTER_RUN			(1u << 31)

/* I2C_TIMEOUT Register bits */
#define HSI2C_TIMEOUT_EN			(1u << 31)
<<<<<<< HEAD
#define HSI2C_TIMEOUT_MASK			0xff
=======
>>>>>>> upstream/android-13

/* I2C_MANUAL_CMD register bits */
#define HSI2C_CMD_READ_DATA			(1u << 4)
#define HSI2C_CMD_SEND_STOP			(1u << 2)

/* I2C_TRANS_STATUS register bits */
#define HSI2C_MASTER_BUSY			(1u << 17)
#define HSI2C_SLAVE_BUSY			(1u << 16)

/* I2C_TRANS_STATUS register bits for Exynos5 variant */
#define HSI2C_TIMEOUT_AUTO			(1u << 4)
#define HSI2C_NO_DEV				(1u << 3)
#define HSI2C_NO_DEV_ACK			(1u << 2)
#define HSI2C_TRANS_ABORT			(1u << 1)
#define HSI2C_TRANS_DONE			(1u << 0)
<<<<<<< HEAD
=======
#define HSI2C_MAST_ST_MASK			(0xf << 0)
#define HSI2C_MASTER_ST_INIT			(0x1)
>>>>>>> upstream/android-13

/* I2C_TRANS_STATUS register bits for Exynos7 variant */
#define HSI2C_MASTER_ST_MASK			0xf
#define HSI2C_MASTER_ST_IDLE			0x0
#define HSI2C_MASTER_ST_START			0x1
#define HSI2C_MASTER_ST_RESTART			0x2
#define HSI2C_MASTER_ST_STOP			0x3
#define HSI2C_MASTER_ST_MASTER_ID		0x4
#define HSI2C_MASTER_ST_ADDR0			0x5
#define HSI2C_MASTER_ST_ADDR1			0x6
#define HSI2C_MASTER_ST_ADDR2			0x7
#define HSI2C_MASTER_ST_ADDR_SR			0x8
#define HSI2C_MASTER_ST_READ			0x9
#define HSI2C_MASTER_ST_WRITE			0xa
#define HSI2C_MASTER_ST_NO_ACK			0xb
#define HSI2C_MASTER_ST_LOSE			0xc
#define HSI2C_MASTER_ST_WAIT			0xd
#define HSI2C_MASTER_ST_WAIT_CMD		0xe

/* I2C_ADDR register bits */
#define HSI2C_SLV_ADDR_SLV(x)			((x & 0x3ff) << 0)
#define HSI2C_SLV_ADDR_MAS(x)			((x & 0x3ff) << 10)
#define HSI2C_MASTER_ID(x)			((x & 0xff) << 24)
<<<<<<< HEAD
#define MASTER_ID(x)				((x & 0x7) + 0x08)
=======
#define MASTER_ID(x)				((((x << 1) + 0x1) & 0x7) + 0x08)
>>>>>>> upstream/android-13

/*
 * Controller operating frequency, timing values for operation
 * are calculated against this frequency
 */
<<<<<<< HEAD
#define HSI2C_HS_TX_CLOCK	1000000
#define HSI2C_FS_TX_CLOCK	100000

#define EXYNOS5_I2C_TIMEOUT (msecs_to_jiffies(100))

enum i2c_type_exynos {
	I2C_TYPE_EXYNOS5,
	I2C_TYPE_EXYNOS7,
};

struct exynos5_i2c {
	struct i2c_adapter	adap;
=======
#define HSI2C_HS_TX_CLOCK			3400000
#define HSI2C_FAST_PLUS_TX_CLOCK	1000000
#define HSI2C_FS_PLUS_HIGH_TX_CLOCK	900000
#define HSI2C_FS_TX_CLOCK			400000
#define HSI2C_STAND_TX_CLOCK		100000

#define HSI2C_STAND_SPD			3
#define HSI2C_FAST_PLUS_SPD		2
#define HSI2C_HIGH_SPD			1
#define HSI2C_FAST_SPD			0

#define HSI2C_POLLING 0
#define HSI2C_INTERRUPT 1

#define EXYNOS5_I2C_TIMEOUT		300
#define EXYNOS5_FIFO_SIZE		16
#define EXYNOS5_I2C_DEFAULT_FILTER_CYCLE	(1)

#define EXYNOS5_HSI2C_RUNTIME_PM_DELAY	(4)

#define USI_CON				(0xC4)
#define USI_OPTION			(0xC8)

#define USI_RESET			(0<<0)
#define USI_HWACG_CLKREQ_ON		(1<<1)
#define USI_HWACG_CLKSTOP_ON		(1<<2)

#define FIFO_TRIG_CRITERIA	(8)

#define USI_SW_CONF_MASK	(0x7 << 0)
#define USI_I2C_SW_CONF		(1<<2)

struct exynos5_i2c {
	struct list_head	node;
	struct i2c_adapter	adap;
	unsigned int		need_hw_init;
>>>>>>> upstream/android-13
	unsigned int		suspended:1;

	struct i2c_msg		*msg;
	struct completion	msg_complete;
	unsigned int		msg_ptr;
<<<<<<< HEAD

	unsigned int		irq;

	void __iomem		*regs;
	struct clk		*clk;
	struct device		*dev;
	int			state;

	spinlock_t		lock;		/* IRQ synchronization */
=======
	unsigned int		msg_len;

	unsigned int		irq;
	struct cpumask		affinity_hint;
	bool			use_all_core;

	void __iomem		*regs;
	struct clk		*clk;
	struct clk		*rate_clk;
	struct device		*dev;
	int			state;
	struct regmap		*usi_reg;
	unsigned int		usi_offset;
>>>>>>> upstream/android-13

	/*
	 * Since the TRANS_DONE bit is cleared on read, and we may read it
	 * either during an IRQ or after a transaction, keep track of its
	 * state here.
	 */
	int			trans_done;

	/* Controller operating frequency */
<<<<<<< HEAD
	unsigned int		op_clock;

	/* Version of HS-I2C Hardware */
	const struct exynos_hsi2c_variant *variant;
};

/**
 * struct exynos_hsi2c_variant - platform specific HSI2C driver data
 * @fifo_depth: the fifo depth supported by the HSI2C module
 * @hw: the hardware variant of Exynos I2C controller
 *
 * Specifies platform specific configuration of HSI2C module.
 * Note: A structure for driver specific platform data is used for future
 * expansion of its usage.
 */
struct exynos_hsi2c_variant {
	unsigned int		fifo_depth;
	enum i2c_type_exynos	hw;
};

static const struct exynos_hsi2c_variant exynos5250_hsi2c_data = {
	.fifo_depth	= 64,
	.hw		= I2C_TYPE_EXYNOS5,
};

static const struct exynos_hsi2c_variant exynos5260_hsi2c_data = {
	.fifo_depth	= 16,
	.hw		= I2C_TYPE_EXYNOS5,
};

static const struct exynos_hsi2c_variant exynos7_hsi2c_data = {
	.fifo_depth	= 16,
	.hw		= I2C_TYPE_EXYNOS7,
};

static const struct of_device_id exynos5_i2c_match[] = {
	{
		.compatible = "samsung,exynos5-hsi2c",
		.data = &exynos5250_hsi2c_data
	}, {
		.compatible = "samsung,exynos5250-hsi2c",
		.data = &exynos5250_hsi2c_data
	}, {
		.compatible = "samsung,exynos5260-hsi2c",
		.data = &exynos5260_hsi2c_data
	}, {
		.compatible = "samsung,exynos7-hsi2c",
		.data = &exynos7_hsi2c_data
	}, {},
};
MODULE_DEVICE_TABLE(of, exynos5_i2c_match);

=======
	unsigned int		fs_clock;
	unsigned int		hs_clock;
	unsigned int		fs_plus_clock;
	unsigned int		stand_clock;
	unsigned int		clock_frequency;
	unsigned int		tscl_h;
	unsigned int		tscl_l;

	/* to set the source clock */
	unsigned int		default_clk;

	/*
	 * HSI2C Controller can operate in
	 * 1. High speed upto 3.4Mbps
	 * 2. Fast speed upto 1Mbps
	 */
	int			speed_mode;
	int			operation_mode;
	int			bus_id;
	int			scl_clk_stretch;
	int			stop_after_trans;
	unsigned int		transfer_delay;

	int			idle_ip_index;
	int			reset_before_trans;
	unsigned int		runtime_resumed;
	int			nack_restart;
	unsigned int		autosuspend_delay;
	unsigned int		xfer_timeout_ms;
	unsigned int		filter_cycle;

	/*
	 * Disable the lose arbitration.
	 */
	int			is_no_arbitration;
};

static const struct of_device_id exynos5_i2c_match[] = {
	{ .compatible = "samsung,exynos5-hsi2c" },
	{},
};
MODULE_DEVICE_TABLE(of, exynos5_i2c_match);

#ifdef CONFIG_GPIOLIB
static void change_i2c_gpio(struct exynos5_i2c *i2c)
{
	struct pinctrl_state *default_i2c_pins;
	struct pinctrl *default_i2c_pinctrl;
	int status = 0;

	default_i2c_pinctrl = devm_pinctrl_get(i2c->dev);
	if (IS_ERR(default_i2c_pinctrl)) {
		dev_err(i2c->dev, "Can't get i2c pinctrl!!!\n");
		return ;
	}

	default_i2c_pins = pinctrl_lookup_state(default_i2c_pinctrl,
				"default");
	if (!IS_ERR(default_i2c_pins)) {
		default_i2c_pinctrl->state = NULL;
		status = pinctrl_select_state(default_i2c_pinctrl, default_i2c_pins);
		if (status)
			dev_err(i2c->dev, "Can't set default i2c pins!!!\n");
	} else {
		dev_err(i2c->dev, "Can't get default pinstate!!!\n");
	}
}

static void recover_gpio_pins(struct exynos5_i2c *i2c)
{
	int gpio_sda, gpio_scl;
	int sda_val, scl_val, clk_cnt;
	unsigned long timeout;
	struct device_node *np = i2c->adap.dev.of_node;

	dev_err(i2c->dev, "Recover GPIO pins\n");

	gpio_sda = of_get_named_gpio(np, "gpio_sda", 0);
	if (!gpio_is_valid(gpio_sda)) {
		dev_err(i2c->dev, "Can't get gpio_sda!!!\n");
		return ;
	}
	gpio_scl = of_get_named_gpio(np, "gpio_scl", 0);
	if (!gpio_is_valid(gpio_scl)) {
		dev_err(i2c->dev, "Can't get gpio_scl!!!\n");
		return ;
	}

	sda_val = gpio_get_value(gpio_sda);
	scl_val = gpio_get_value(gpio_scl);

	dev_err(i2c->dev, "SDA line : %s, SCL line : %s\n",
			sda_val ? "HIGH" : "LOW", scl_val ? "HIGH" : "LOW");

	if (sda_val == 1)
		return ;

	/* Wait for SCL as high for 500msec */
	if (scl_val == 0) {
		timeout = jiffies + msecs_to_jiffies(500);
		while (time_before(jiffies, timeout)) {
			if (gpio_get_value(gpio_scl) != 0) {
				timeout = 0;
				break;
			}
			msleep(10);
		}
		if (timeout)
			dev_err(i2c->dev, "SCL line is still LOW!!!\n");
	}

	sda_val = gpio_get_value(gpio_sda);

	if (sda_val == 0) {
		gpio_direction_output(gpio_scl, 1);
		gpio_direction_input(gpio_sda);

		for (clk_cnt = 0; clk_cnt < 100; clk_cnt++) {
			/* Make clock for slave */
			gpio_set_value(gpio_scl, 0);
			udelay(5);
			gpio_set_value(gpio_scl, 1);
			udelay(5);
			if (gpio_get_value(gpio_sda) == 1) {
				dev_err(i2c->dev, "SDA line is recovered.\n");
				break;
			}
		}
		if (clk_cnt == 100)
			dev_err(i2c->dev, "SDA line is not recovered!!!\n");
	}

	/* Change I2C GPIO as default function */
	change_i2c_gpio(i2c);
}
#endif

static inline void dump_i2c_register(struct exynos5_i2c *i2c)
{
	dev_err(i2c->dev, "Register dump(suspended : %d)\n"
		"CTL          0x%08x   "
		"FIFO_CTL     0x%08x   "
		"INT_EN       0x%08x   "
		"INT_STAT     0x%08x \n"
		"FIFO_STAT    0x%08x   "
		"CONF         0x%08x   "
		"CONF2        0x%08x   "
		"TRANS_STAT   0x%08x \n"
		"TIMING_HS1   0x%08x   "
		"TIMING_HS2   0x%08x   "
		"TIMING_HS3   0x%08x   "
		"TIMING_FS1   0x%08x \n"
		"TIMING_FS2   0x%08x   "
		"TIMING_FS3   0x%08x   "
		"TRAILING_CTL 0x%08x   "
		"ADDR         0x%08x \n"
		, i2c->suspended
		, readl(i2c->regs + HSI2C_CTL)
		, readl(i2c->regs + HSI2C_FIFO_CTL)
		, readl(i2c->regs + HSI2C_INT_ENABLE)
		, readl(i2c->regs + HSI2C_INT_STATUS)
		, readl(i2c->regs + HSI2C_FIFO_STATUS)
		, readl(i2c->regs + HSI2C_CONF)
		, readl(i2c->regs + HSI2C_AUTO_CONF)
		, readl(i2c->regs + HSI2C_TRANS_STATUS)
		, readl(i2c->regs + HSI2C_TIMING_HS1)
		, readl(i2c->regs + HSI2C_TIMING_HS2)
		, readl(i2c->regs + HSI2C_TIMING_HS3)
		, readl(i2c->regs + HSI2C_TIMING_FS1)
		, readl(i2c->regs + HSI2C_TIMING_FS2)
		, readl(i2c->regs + HSI2C_TIMING_FS3)
		, readl(i2c->regs + HSI2C_TRAILIG_CTL)
		, readl(i2c->regs + HSI2C_ADDR)
	);

#ifdef CONFIG_GPIOLIB
	recover_gpio_pins(i2c);
#endif
}

>>>>>>> upstream/android-13
static void exynos5_i2c_clr_pend_irq(struct exynos5_i2c *i2c)
{
	writel(readl(i2c->regs + HSI2C_INT_STATUS),
				i2c->regs + HSI2C_INT_STATUS);
}

/*
 * exynos5_i2c_set_timing: updates the registers with appropriate
 * timing values calculated
 *
<<<<<<< HEAD
 * Returns 0 on success, -EINVAL if the cycle length cannot
 * be calculated.
 */
static int exynos5_i2c_set_timing(struct exynos5_i2c *i2c, bool hs_timings)
{
	u32 i2c_timing_s1;
	u32 i2c_timing_s2;
	u32 i2c_timing_s3;
	u32 i2c_timing_sla;
	unsigned int t_start_su, t_start_hd;
	unsigned int t_stop_su;
	unsigned int t_data_su, t_data_hd;
	unsigned int t_scl_l, t_scl_h;
	unsigned int t_sr_release;
	unsigned int t_ftl_cycle;
	unsigned int clkin = clk_get_rate(i2c->clk);
	unsigned int op_clk = hs_timings ? i2c->op_clock :
		(i2c->op_clock >= HSI2C_HS_TX_CLOCK) ? HSI2C_FS_TX_CLOCK :
		i2c->op_clock;
	int div, clk_cycle, temp;

	/*
	 * In case of HSI2C controller in Exynos5 series
	 * FPCLK / FI2C =
	 * (CLK_DIV + 1) * (TSCLK_L + TSCLK_H + 2) + 8 + 2 * FLT_CYCLE
	 *
	 * In case of HSI2C controllers in Exynos7 series
	 * FPCLK / FI2C =
	 * (CLK_DIV + 1) * (TSCLK_L + TSCLK_H + 2) + 8 + FLT_CYCLE
	 *
	 * clk_cycle := TSCLK_L + TSCLK_H
	 * temp := (CLK_DIV + 1) * (clk_cycle + 2)
	 *
	 * Constraints: 4 <= temp, 0 <= CLK_DIV < 256, 2 <= clk_cycle <= 510
	 *
	 */
	t_ftl_cycle = (readl(i2c->regs + HSI2C_CONF) >> 16) & 0x7;
	temp = clkin / op_clk - 8 - t_ftl_cycle;
	if (i2c->variant->hw != I2C_TYPE_EXYNOS7)
		temp -= t_ftl_cycle;
	div = temp / 512;
	clk_cycle = temp / (div + 1) - 2;
	if (temp < 4 || div >= 256 || clk_cycle < 2) {
		dev_err(i2c->dev, "%s clock set-up failed\n",
			hs_timings ? "HS" : "FS");
		return -EINVAL;
	}

	t_scl_l = clk_cycle / 2;
	t_scl_h = clk_cycle / 2;
	t_start_su = t_scl_l;
	t_start_hd = t_scl_l;
	t_stop_su = t_scl_l;
	t_data_su = t_scl_l / 2;
	t_data_hd = t_scl_l / 2;
	t_sr_release = clk_cycle;

	i2c_timing_s1 = t_start_su << 24 | t_start_hd << 16 | t_stop_su << 8;
	i2c_timing_s2 = t_data_su << 24 | t_scl_l << 8 | t_scl_h << 0;
	i2c_timing_s3 = div << 16 | t_sr_release << 0;
	i2c_timing_sla = t_data_hd << 0;

	dev_dbg(i2c->dev, "tSTART_SU: %X, tSTART_HD: %X, tSTOP_SU: %X\n",
		t_start_su, t_start_hd, t_stop_su);
	dev_dbg(i2c->dev, "tDATA_SU: %X, tSCL_L: %X, tSCL_H: %X\n",
		t_data_su, t_scl_l, t_scl_h);
	dev_dbg(i2c->dev, "nClkDiv: %X, tSR_RELEASE: %X\n",
		div, t_sr_release);
	dev_dbg(i2c->dev, "tDATA_HD: %X\n", t_data_hd);

	if (hs_timings) {
		writel(i2c_timing_s1, i2c->regs + HSI2C_TIMING_HS1);
		writel(i2c_timing_s2, i2c->regs + HSI2C_TIMING_HS2);
		writel(i2c_timing_s3, i2c->regs + HSI2C_TIMING_HS3);
	} else {
		writel(i2c_timing_s1, i2c->regs + HSI2C_TIMING_FS1);
		writel(i2c_timing_s2, i2c->regs + HSI2C_TIMING_FS2);
		writel(i2c_timing_s3, i2c->regs + HSI2C_TIMING_FS3);
	}
	writel(i2c_timing_sla, i2c->regs + HSI2C_TIMING_SLA);
=======
 * Timing values for operation are calculated against either 100kHz
 * or 1MHz controller operating frequency.
 *
 * Returns 0 on success, -EINVAL if the cycle length cannot
 * be calculated.
 */
static int exynos5_i2c_set_timing(struct exynos5_i2c *i2c, int mode)
{
	int ret;
	unsigned int ipclk;
	unsigned int op_clk;

	u32 hs_div, uTSCL_H_HS, uTSCL_L_HS, uTSTART_HD_HS;
	u32 fs_div, uTSCL_H_FS, uTSCL_L_FS, uTSTART_HD_FS;
	u32 utemp;

	if (i2c->default_clk) {
		ret = clk_set_rate(i2c->rate_clk, i2c->default_clk);

		if (ret < 0)
			dev_err(i2c->dev, "Failed to set clock\n");
	}

	ipclk = (unsigned int)clk_get_rate(i2c->rate_clk);

	if (mode == HSI2C_STAND_SPD) {
		op_clk = i2c->stand_clock;

		if (!op_clk)
			op_clk = HSI2C_STAND_TX_CLOCK;

		fs_div = ipclk / (op_clk * 16);
		fs_div &= 0xFF;
		utemp = readl(i2c->regs + HSI2C_TIMING_FS3) & ~0x00FF0000;
		writel(utemp | (fs_div << 16), i2c->regs + HSI2C_TIMING_FS3);

		if (!i2c->tscl_h) {
			uTSCL_H_FS = (25 *(ipclk / (1000 * 1000))) / ((fs_div + 1) * 10);
			uTSCL_H_FS = (0xFF << uTSCL_H_FS) & 0xFF;
		} else {
			uTSCL_H_FS = i2c->tscl_h;
		}
		utemp = readl(i2c->regs + HSI2C_TIMING_FS2) & ~0x000000FF;
		writel(utemp | (uTSCL_H_FS << 0), i2c->regs + HSI2C_TIMING_FS2);

		if (i2c->tscl_l) {
			uTSCL_L_FS = i2c->tscl_l;
			utemp = readl(i2c->regs + HSI2C_TIMING_FS2) & ~0x0000FF00;
			writel(utemp | (uTSCL_L_FS << 8), i2c->regs + HSI2C_TIMING_FS2);
		}

		uTSTART_HD_FS = (25 * (ipclk / (1000 * 1000))) / ((fs_div + 1) * 10) - 1;
		uTSTART_HD_FS = (0xFF << uTSTART_HD_FS) & 0xFF;
		utemp = readl(i2c->regs + HSI2C_TIMING_FS1) & ~0x00FF0000;
		writel(utemp | (uTSTART_HD_FS << 16), i2c->regs + HSI2C_TIMING_FS1);

		dev_info(i2c->dev, "%s IPCLK = %d OP_CLK = %d DIV = %d Timing FS1(STAND) = 0x%X "
				"TIMING FS2(STAND) = 0x%X TIMING FS3(STAND) = 0x%X\n",__func__, ipclk, op_clk, fs_div,
				readl(i2c->regs + HSI2C_TIMING_FS1), readl(i2c->regs + HSI2C_TIMING_FS2),
				readl(i2c->regs + HSI2C_TIMING_FS3));
	} else if (mode == HSI2C_FAST_PLUS_SPD) {
		unsigned int sample_factor;

		op_clk = i2c->fs_plus_clock;

		if (!op_clk)
			op_clk = HSI2C_FAST_PLUS_TX_CLOCK;

		if (op_clk > HSI2C_FS_PLUS_HIGH_TX_CLOCK)
			sample_factor = 17;
		else
			sample_factor = 16;

		fs_div = ipclk / (op_clk * sample_factor);
		fs_div &= 0xFF;
		utemp = readl(i2c->regs + HSI2C_TIMING_FS3) & ~0x00FF0000;
		writel(utemp | (fs_div << 16), i2c->regs + HSI2C_TIMING_FS3);

		if (!i2c->tscl_h) {
			uTSCL_H_FS = ((ipclk / (1000 * 1000)) * 35 / 10) / ((fs_div + 1) * 10);
			uTSCL_H_FS = (0xFF << uTSCL_H_FS) & 0xFF;
		} else {
			uTSCL_H_FS = i2c->tscl_h;
		}
		utemp = readl(i2c->regs + HSI2C_TIMING_FS2) & ~0x000000FF;
		writel(utemp | (uTSCL_H_FS << 0), i2c->regs + HSI2C_TIMING_FS2);

		if (i2c->tscl_l) {
			uTSCL_L_FS = i2c->tscl_l;
			utemp = readl(i2c->regs + HSI2C_TIMING_FS2) & ~0x0000FF00;
			writel(utemp | (uTSCL_L_FS << 8), i2c->regs + HSI2C_TIMING_FS2);
		}

		uTSTART_HD_FS = (4 * (ipclk / (1000 * 1000))) / ((fs_div + 1) * 10) - 1;
		uTSTART_HD_FS = (0xFF << uTSTART_HD_FS) & 0xFF;
		utemp = readl(i2c->regs + HSI2C_TIMING_FS1) & ~0x00FF0000;
		writel(utemp | (uTSTART_HD_FS << 16), i2c->regs + HSI2C_TIMING_FS1);

		dev_info(i2c->dev, "%s IPCLK = %d OP_CLK = %d DIV = %d Timing FS1(FS+) = 0x%X "
				"TIMING FS2(FS+) = 0x%X TIMING FS3(FS+) = 0x%X\n",__func__, ipclk, op_clk, fs_div,
				readl(i2c->regs + HSI2C_TIMING_FS1), readl(i2c->regs + HSI2C_TIMING_FS2),
				readl(i2c->regs + HSI2C_TIMING_FS3));
	} else if (mode == HSI2C_HIGH_SPD) {
		/* ipclk's unit is Hz, op_clk's unit is Hz */
		op_clk = i2c->hs_clock;

		if (!op_clk)
			op_clk = HSI2C_HS_TX_CLOCK;

		hs_div = ipclk / (op_clk * 17);
		hs_div &= 0xFF;
		utemp = readl(i2c->regs + HSI2C_TIMING_HS3) & ~0x00FF0000;
		writel(utemp | (hs_div << 16), i2c->regs + HSI2C_TIMING_HS3);

		if (!i2c->tscl_h) {
			uTSCL_H_HS = ((7 * ipclk) / (1000 * 1000)) / ((hs_div + 1) * 100);
			/* make to 0 into TSCL_H_HS from LSB */
			uTSCL_H_HS = (0xFFFFFFFF >> uTSCL_H_HS) << uTSCL_H_HS;
			uTSCL_H_HS &= 0xFF;
		} else {
			uTSCL_H_HS = i2c->tscl_h;
		}
		utemp = readl(i2c->regs + HSI2C_TIMING_HS2) & ~0x000000FF;
		writel(utemp | (uTSCL_H_HS << 0), i2c->regs + HSI2C_TIMING_HS2);

		if (i2c->tscl_l) {
			uTSCL_L_HS = i2c->tscl_l;
			utemp = readl(i2c->regs + HSI2C_TIMING_FS2) & ~0x0000FF00;
			writel(utemp | (uTSCL_L_HS << 8), i2c->regs + HSI2C_TIMING_FS2);
		}

		uTSTART_HD_HS = (7 * ipclk / (1000 * 1000)) / ((hs_div + 1) * 100) - 1;
		/* make to 0 into uTSTART_HD_HS from LSB */
		uTSTART_HD_HS = (0xFFFFFFFF >> uTSTART_HD_HS) << uTSTART_HD_HS;
		uTSTART_HD_HS &= 0xFF;
		utemp = readl(i2c->regs + HSI2C_TIMING_HS1) & ~0x00FF0000;
		writel(utemp | (uTSTART_HD_HS << 16), i2c->regs + HSI2C_TIMING_HS1);

		dev_info(i2c->dev, "%s IPCLK = %d OP_CLK = %d DIV = %d Timing HS1 = 0x%08X "
				"TIMING HS2 = 0x%08X TIMING HS3 = 0x%08X\n",__func__, ipclk, op_clk, hs_div,
				readl(i2c->regs + HSI2C_TIMING_HS1), readl(i2c->regs + HSI2C_TIMING_HS2),
				readl(i2c->regs + HSI2C_TIMING_HS3));
	}
	else {
		/* Fast speed mode */
		/* ipclk's unit is Hz, op_clk's unit is Hz */
		op_clk = i2c->fs_clock;

		if (!op_clk)
			op_clk = HSI2C_FS_TX_CLOCK;

		fs_div = ipclk / (op_clk * 16);
		fs_div &= 0xFF;
		utemp = readl(i2c->regs + HSI2C_TIMING_FS3) & ~0x00FF0000;
		writel(utemp | (fs_div << 16), i2c->regs + HSI2C_TIMING_FS3);

		if (!i2c->tscl_h) {
			uTSCL_H_FS = ((9 * ipclk) / (1000 * 1000)) / ((fs_div + 1) * 10);
			/* make to 0 into TSCL_H_FS from LSB */
			uTSCL_H_FS = (0xFFFFFFFF >> uTSCL_H_FS) << uTSCL_H_FS;
			uTSCL_H_FS &= 0xFF;
		} else {
			uTSCL_H_FS = i2c->tscl_h;
		}
		utemp = readl(i2c->regs + HSI2C_TIMING_FS2) & ~0x000000FF;
		writel(utemp | (uTSCL_H_FS << 0), i2c->regs + HSI2C_TIMING_FS2);

		if (i2c->tscl_l) {
			uTSCL_L_FS = i2c->tscl_l;
			utemp = readl(i2c->regs + HSI2C_TIMING_FS2) & ~0x0000FF00;
			writel(utemp | (uTSCL_L_FS << 8), i2c->regs + HSI2C_TIMING_FS2);
		}

		uTSTART_HD_FS = (9 * ipclk / (1000 * 1000)) / ((fs_div + 1) * 10) - 1;
		/* make to 0 into uTSTART_HD_FS from LSB */
		uTSTART_HD_FS = (0xFFFFFFFF >> uTSTART_HD_FS) << uTSTART_HD_FS;
		uTSTART_HD_FS &= 0xFF;
		utemp = readl(i2c->regs + HSI2C_TIMING_FS1) & ~0x00FF0000;
		writel(utemp | (uTSTART_HD_FS << 16), i2c->regs + HSI2C_TIMING_FS1);

		dev_info(i2c->dev, "%s IPCLK = %d OP_CLK = %d DIV = %d Timing FS1 = 0x%X "
				"TIMING FS2 = 0x%X TIMING FS3 = 0x%X\n",__func__, ipclk, op_clk, fs_div,
				readl(i2c->regs + HSI2C_TIMING_FS1), readl(i2c->regs + HSI2C_TIMING_FS2),
				readl(i2c->regs + HSI2C_TIMING_FS3));
	}
>>>>>>> upstream/android-13

	return 0;
}

static int exynos5_hsi2c_clock_setup(struct exynos5_i2c *i2c)
{
<<<<<<< HEAD
	/* always set Fast Speed timings */
	int ret = exynos5_i2c_set_timing(i2c, false);

	if (ret < 0 || i2c->op_clock < HSI2C_HS_TX_CLOCK)
		return ret;

	return exynos5_i2c_set_timing(i2c, true);
=======
	/*
	 * Configure the Fast speed timing values for High Speed mode
	 * Because High Speed mode initially starts with Fast mode
	 */
	if (i2c->speed_mode == HSI2C_HIGH_SPD) {
		if (exynos5_i2c_set_timing(i2c, HSI2C_FAST_SPD)) {
			dev_err(i2c->dev, "HSI2C FS Clock set up failed\n");
			return -EINVAL;
		}
	}

	if (exynos5_i2c_set_timing(i2c, i2c->speed_mode)) {
		dev_err(i2c->dev, "HSI2C mode(%d) Clock set up failed\n", i2c->speed_mode);
		return -EINVAL;
	}

	return 0;
}

static void exynos_usi_init(struct exynos5_i2c *i2c)
{
	/* USI_RESET is active High signal.
	 * Reset value of USI_RESET is 'h1 to drive stable value to PAD.
	 * Due to this feature, the USI_RESET must be cleared (set as '0')
	 * before transaction starts.
	 */
	writel(USI_RESET, i2c->regs + USI_CON);
>>>>>>> upstream/android-13
}

/*
 * exynos5_i2c_init: configures the controller for I2C functionality
 * Programs I2C controller for Master mode operation
 */
static void exynos5_i2c_init(struct exynos5_i2c *i2c)
{
	u32 i2c_conf = readl(i2c->regs + HSI2C_CONF);
<<<<<<< HEAD
	u32 i2c_timeout = readl(i2c->regs + HSI2C_TIMEOUT);

	/* Clear to disable Timeout */
	i2c_timeout &= ~HSI2C_TIMEOUT_EN;
	writel(i2c_timeout, i2c->regs + HSI2C_TIMEOUT);

	writel((HSI2C_FUNC_MODE_I2C | HSI2C_MASTER),
					i2c->regs + HSI2C_CTL);
	writel(HSI2C_TRAILING_COUNT, i2c->regs + HSI2C_TRAILIG_CTL);

	if (i2c->op_clock >= HSI2C_HS_TX_CLOCK) {
		writel(HSI2C_MASTER_ID(MASTER_ID(i2c->adap.nr)),
=======

	writel(HSI2C_MASTER, i2c->regs + HSI2C_CTL);
	writel(HSI2C_TRAILING_COUNT, i2c->regs + HSI2C_TRAILIG_CTL);

	if (i2c->speed_mode == HSI2C_HIGH_SPD) {
		writel(HSI2C_MASTER_ID(MASTER_ID(i2c->bus_id)),
>>>>>>> upstream/android-13
					i2c->regs + HSI2C_ADDR);
		i2c_conf |= HSI2C_HS_MODE;
	}

<<<<<<< HEAD
	writel(i2c_conf | HSI2C_AUTO_MODE, i2c->regs + HSI2C_CONF);
=======
	/* set glitch filter cycle */
	i2c_conf |= HSI2C_FTL_CYCLE_SCL(i2c->filter_cycle);
	i2c_conf |= HSI2C_FTL_CYCLE_SDA(i2c->filter_cycle);
	dev_info(i2c->dev, "Filter cycle: %d\n", i2c->filter_cycle);

	writel(i2c_conf | HSI2C_AUTO_MODE, i2c->regs + HSI2C_CONF);

	i2c->need_hw_init = 0;
>>>>>>> upstream/android-13
}

static void exynos5_i2c_reset(struct exynos5_i2c *i2c)
{
	u32 i2c_ctl;

	/* Set and clear the bit for reset */
	i2c_ctl = readl(i2c->regs + HSI2C_CTL);
	i2c_ctl |= HSI2C_SW_RST;
	writel(i2c_ctl, i2c->regs + HSI2C_CTL);

	i2c_ctl = readl(i2c->regs + HSI2C_CTL);
	i2c_ctl &= ~HSI2C_SW_RST;
	writel(i2c_ctl, i2c->regs + HSI2C_CTL);

	/* We don't expect calculations to fail during the run */
	exynos5_hsi2c_clock_setup(i2c);
	/* Initialize the configure registers */
	exynos5_i2c_init(i2c);
}

<<<<<<< HEAD
=======
static inline void exynos5_i2c_stop(struct exynos5_i2c *i2c)
{
	writel(0, i2c->regs + HSI2C_INT_ENABLE);

	complete(&i2c->msg_complete);
}

>>>>>>> upstream/android-13
/*
 * exynos5_i2c_irq: top level IRQ servicing routine
 *
 * INT_STATUS registers gives the interrupt details. Further,
 * FIFO_STATUS or TRANS_STATUS registers are to be check for detailed
 * state of the bus.
 */
<<<<<<< HEAD
static irqreturn_t exynos5_i2c_irq(int irqno, void *dev_id)
{
	struct exynos5_i2c *i2c = dev_id;
	u32 fifo_level, int_status, fifo_status, trans_status;
	unsigned char byte;
	int len = 0;

	i2c->state = -EINVAL;

	spin_lock(&i2c->lock);

	int_status = readl(i2c->regs + HSI2C_INT_STATUS);
	writel(int_status, i2c->regs + HSI2C_INT_STATUS);

	/* handle interrupt related to the transfer status */
	if (i2c->variant->hw == I2C_TYPE_EXYNOS7) {
		if (int_status & HSI2C_INT_TRANS_DONE) {
			i2c->trans_done = 1;
			i2c->state = 0;
		} else if (int_status & HSI2C_INT_TRANS_ABORT) {
			dev_dbg(i2c->dev, "Deal with arbitration lose\n");
			i2c->state = -EAGAIN;
			goto stop;
		} else if (int_status & HSI2C_INT_NO_DEV_ACK) {
			dev_dbg(i2c->dev, "No ACK from device\n");
			i2c->state = -ENXIO;
			goto stop;
		} else if (int_status & HSI2C_INT_NO_DEV) {
			dev_dbg(i2c->dev, "No device\n");
			i2c->state = -ENXIO;
			goto stop;
		} else if (int_status & HSI2C_INT_TIMEOUT) {
			dev_dbg(i2c->dev, "Accessing device timed out\n");
			i2c->state = -ETIMEDOUT;
			goto stop;
		}
	} else if (int_status & HSI2C_INT_I2C) {
		trans_status = readl(i2c->regs + HSI2C_TRANS_STATUS);
		if (trans_status & HSI2C_NO_DEV_ACK) {
			dev_dbg(i2c->dev, "No ACK from device\n");
			i2c->state = -ENXIO;
			goto stop;
		} else if (trans_status & HSI2C_NO_DEV) {
			dev_dbg(i2c->dev, "No device\n");
			i2c->state = -ENXIO;
			goto stop;
		} else if (trans_status & HSI2C_TRANS_ABORT) {
			dev_dbg(i2c->dev, "Deal with arbitration lose\n");
			i2c->state = -EAGAIN;
			goto stop;
		} else if (trans_status & HSI2C_TIMEOUT_AUTO) {
			dev_dbg(i2c->dev, "Accessing device timed out\n");
			i2c->state = -ETIMEDOUT;
			goto stop;
		} else if (trans_status & HSI2C_TRANS_DONE) {
			i2c->trans_done = 1;
			i2c->state = 0;
		}
	}

	if ((i2c->msg->flags & I2C_M_RD) && (int_status &
			(HSI2C_INT_TRAILING | HSI2C_INT_RX_ALMOSTFULL))) {
		fifo_status = readl(i2c->regs + HSI2C_FIFO_STATUS);
		fifo_level = HSI2C_RX_FIFO_LVL(fifo_status);
		len = min(fifo_level, i2c->msg->len - i2c->msg_ptr);

		while (len > 0) {
			byte = (unsigned char)
				readl(i2c->regs + HSI2C_RX_DATA);
			i2c->msg->buf[i2c->msg_ptr++] = byte;
			len--;
		}
		i2c->state = 0;
	} else if (int_status & HSI2C_INT_TX_ALMOSTEMPTY) {
		fifo_status = readl(i2c->regs + HSI2C_FIFO_STATUS);
		fifo_level = HSI2C_TX_FIFO_LVL(fifo_status);

		len = i2c->variant->fifo_depth - fifo_level;
		if (len > (i2c->msg->len - i2c->msg_ptr)) {
			u32 int_en = readl(i2c->regs + HSI2C_INT_ENABLE);

			int_en &= ~HSI2C_INT_TX_ALMOSTEMPTY_EN;
			writel(int_en, i2c->regs + HSI2C_INT_ENABLE);
			len = i2c->msg->len - i2c->msg_ptr;
		}

		while (len > 0) {
			byte = i2c->msg->buf[i2c->msg_ptr++];
			writel(byte, i2c->regs + HSI2C_TX_DATA);
			len--;
		}
		i2c->state = 0;
	}

 stop:
	if ((i2c->trans_done && (i2c->msg->len == i2c->msg_ptr)) ||
	    (i2c->state < 0)) {
		writel(0, i2c->regs + HSI2C_INT_ENABLE);
		exynos5_i2c_clr_pend_irq(i2c);
		complete(&i2c->msg_complete);
	}

	spin_unlock(&i2c->lock);

	return IRQ_HANDLED;
}

/*
 * exynos5_i2c_wait_bus_idle
 *
 * Wait for the bus to go idle, indicated by the MASTER_BUSY bit being
 * cleared.
 *
 * Returns -EBUSY if the bus cannot be bought to idle
 */
static int exynos5_i2c_wait_bus_idle(struct exynos5_i2c *i2c)
{
	unsigned long stop_time;
	u32 trans_status;

	/* wait for 100 milli seconds for the bus to be idle */
	stop_time = jiffies + msecs_to_jiffies(100) + 1;
	do {
		trans_status = readl(i2c->regs + HSI2C_TRANS_STATUS);
		if (!(trans_status & HSI2C_MASTER_BUSY))
			return 0;

		usleep_range(50, 200);
	} while (time_before(jiffies, stop_time));

	return -EBUSY;
}

static void exynos5_i2c_bus_recover(struct exynos5_i2c *i2c)
{
	u32 val;

	val = readl(i2c->regs + HSI2C_CTL) | HSI2C_RXCHON;
	writel(val, i2c->regs + HSI2C_CTL);
	val = readl(i2c->regs + HSI2C_CONF) & ~HSI2C_AUTO_MODE;
	writel(val, i2c->regs + HSI2C_CONF);

	/*
	 * Specification says master should send nine clock pulses. It can be
	 * emulated by sending manual read command (nine pulses for read eight
	 * bits + one pulse for NACK).
	 */
	writel(HSI2C_CMD_READ_DATA, i2c->regs + HSI2C_MANUAL_CMD);
	exynos5_i2c_wait_bus_idle(i2c);
	writel(HSI2C_CMD_SEND_STOP, i2c->regs + HSI2C_MANUAL_CMD);
	exynos5_i2c_wait_bus_idle(i2c);

	val = readl(i2c->regs + HSI2C_CTL) & ~HSI2C_RXCHON;
	writel(val, i2c->regs + HSI2C_CTL);
	val = readl(i2c->regs + HSI2C_CONF) | HSI2C_AUTO_MODE;
	writel(val, i2c->regs + HSI2C_CONF);
}

static void exynos5_i2c_bus_check(struct exynos5_i2c *i2c)
{
	unsigned long timeout;

	if (i2c->variant->hw != I2C_TYPE_EXYNOS7)
		return;

	/*
	 * HSI2C_MASTER_ST_LOSE state in EXYNOS7 variant before transaction
	 * indicates that bus is stuck (SDA is low). In such case bus recovery
	 * can be performed.
	 */
	timeout = jiffies + msecs_to_jiffies(100);
	for (;;) {
		u32 st = readl(i2c->regs + HSI2C_TRANS_STATUS);

		if ((st & HSI2C_MASTER_ST_MASK) != HSI2C_MASTER_ST_LOSE)
			return;

		if (time_is_before_jiffies(timeout))
			return;

		exynos5_i2c_bus_recover(i2c);
	}
}

/*
 * exynos5_i2c_message_start: Configures the bus and starts the xfer
 * i2c: struct exynos5_i2c pointer for the current bus
 * stop: Enables stop after transfer if set. Set for last transfer of
 *       in the list of messages.
 *
 * Configures the bus for read/write function
 * Sets chip address to talk to, message length to be sent.
 * Enables appropriate interrupts and sends start xfer command.
 */
static void exynos5_i2c_message_start(struct exynos5_i2c *i2c, int stop)
{
	u32 i2c_ctl;
	u32 int_en = 0;
	u32 i2c_auto_conf = 0;
	u32 fifo_ctl;
	unsigned long flags;
	unsigned short trig_lvl;

	if (i2c->variant->hw == I2C_TYPE_EXYNOS7)
		int_en |= HSI2C_INT_I2C_TRANS;
	else
		int_en |= HSI2C_INT_I2C;

	i2c_ctl = readl(i2c->regs + HSI2C_CTL);
	i2c_ctl &= ~(HSI2C_TXCHON | HSI2C_RXCHON);
	fifo_ctl = HSI2C_RXFIFO_EN | HSI2C_TXFIFO_EN;

	if (i2c->msg->flags & I2C_M_RD) {
		i2c_ctl |= HSI2C_RXCHON;

		i2c_auto_conf |= HSI2C_READ_WRITE;

		trig_lvl = (i2c->msg->len > i2c->variant->fifo_depth) ?
			(i2c->variant->fifo_depth * 3 / 4) : i2c->msg->len;
		fifo_ctl |= HSI2C_RXFIFO_TRIGGER_LEVEL(trig_lvl);

		int_en |= (HSI2C_INT_RX_ALMOSTFULL_EN |
			HSI2C_INT_TRAILING_EN);
	} else {
		i2c_ctl |= HSI2C_TXCHON;

		trig_lvl = (i2c->msg->len > i2c->variant->fifo_depth) ?
			(i2c->variant->fifo_depth * 1 / 4) : i2c->msg->len;
		fifo_ctl |= HSI2C_TXFIFO_TRIGGER_LEVEL(trig_lvl);

		int_en |= HSI2C_INT_TX_ALMOSTEMPTY_EN;
	}

	writel(HSI2C_SLV_ADDR_MAS(i2c->msg->addr), i2c->regs + HSI2C_ADDR);

	writel(fifo_ctl, i2c->regs + HSI2C_FIFO_CTL);
	writel(i2c_ctl, i2c->regs + HSI2C_CTL);

	exynos5_i2c_bus_check(i2c);

	/*
	 * Enable interrupts before starting the transfer so that we don't
	 * miss any INT_I2C interrupts.
	 */
	spin_lock_irqsave(&i2c->lock, flags);
	writel(int_en, i2c->regs + HSI2C_INT_ENABLE);

	if (stop == 1)
		i2c_auto_conf |= HSI2C_STOP_AFTER_TRANS;
	i2c_auto_conf |= i2c->msg->len;
	i2c_auto_conf |= HSI2C_MASTER_RUN;
	writel(i2c_auto_conf, i2c->regs + HSI2C_AUTO_CONF);
	spin_unlock_irqrestore(&i2c->lock, flags);
=======

static irqreturn_t exynos5_i2c_irq(int irqno, void *dev_id)
{
	struct exynos5_i2c *i2c = dev_id;
	unsigned long reg_val;
	unsigned long trans_status;
	unsigned char byte;

	if (!i2c) {
		pr_err("irq nodev (irqno:%d)\n", irqno);
		return IRQ_HANDLED;
	}

	if (i2c->msg == NULL || readl(i2c->regs + HSI2C_INT_ENABLE) == 0x0) {
		pr_err("invalid irq (irqno:%d)\n", irqno);
		reg_val = readl(i2c->regs + HSI2C_INT_STATUS);
		goto out;
	}

	if (i2c->msg->flags & I2C_M_RD) {
		while ((readl(i2c->regs + HSI2C_FIFO_STATUS) &
			0x1000000) == 0) {
			byte = (unsigned char)readl(i2c->regs + HSI2C_RX_DATA);
			i2c->msg->buf[i2c->msg_ptr++] = byte;
		}

		if (i2c->msg_ptr >= i2c->msg->len) {
			reg_val = readl(i2c->regs + HSI2C_INT_ENABLE);
			reg_val &= ~(HSI2C_INT_RX_ALMOSTFULL_EN);
			writel(reg_val, i2c->regs + HSI2C_INT_ENABLE);
			exynos5_i2c_stop(i2c);
		}
	} else {
		while ((readl(i2c->regs + HSI2C_FIFO_STATUS) &
			0x80) == 0) {
			if (i2c->msg_ptr >= i2c->msg->len) {
				reg_val = readl(i2c->regs + HSI2C_INT_ENABLE);
				reg_val &= ~(HSI2C_INT_TX_ALMOSTEMPTY_EN);
				writel(reg_val, i2c->regs + HSI2C_INT_ENABLE);
				break;
			}
			byte = i2c->msg->buf[i2c->msg_ptr++];
			writel(byte, i2c->regs + HSI2C_TX_DATA);
		}
	}

	reg_val = readl(i2c->regs + HSI2C_INT_STATUS);

	/*
	 * Checking Error State in INT_STATUS register
	 */
	if (reg_val & HSI2C_INT_CHK_TRANS_STATE) {
		trans_status = readl(i2c->regs + HSI2C_TRANS_STATUS);
		dev_err(i2c->dev, "HSI2C Error Interrupt "
				"occurred(IS:0x%08x, TR:0x%08x)\n",
				(unsigned int)reg_val, (unsigned int)trans_status);

		if (reg_val & HSI2C_INT_NODEV) {
			dev_err(i2c->dev, "HSI2C NO ACK occured\n");
			if (i2c->nack_restart) {
				if (reg_val & HSI2C_INT_TRANSFER_DONE)
					exynos5_i2c_stop(i2c);
				goto out;
			}
		}

		i2c->trans_done = -ENXIO;
		exynos5_i2c_stop(i2c);
		goto out;
	}
	/* Checking INT_TRANSFER_DONE */
	if ((reg_val & HSI2C_INT_TRANSFER_DONE) &&
		(i2c->msg_ptr >= i2c->msg->len) &&
		!(i2c->msg->flags & I2C_M_RD))
		exynos5_i2c_stop(i2c);

	/*Check read transfer done, But read bytes not enough.*/
	if ((reg_val & HSI2C_INT_TRANSFER_DONE) &&
		(i2c->msg_ptr < i2c->msg->len) &&
		(i2c->msg->flags & I2C_M_RD)) {
		while ((readl(i2c->regs + HSI2C_FIFO_STATUS) &
			0x1000000) == 0) {
			byte = (unsigned char)readl(i2c->regs + HSI2C_RX_DATA);
			i2c->msg->buf[i2c->msg_ptr++] = byte;
		}
		if (i2c->msg_ptr >= i2c->msg->len) {
			reg_val = readl(i2c->regs + HSI2C_INT_ENABLE);
			reg_val &= ~(HSI2C_INT_RX_ALMOSTFULL_EN);
			writel(reg_val, i2c->regs + HSI2C_INT_ENABLE);
			exynos5_i2c_stop(i2c);
		} else {
			dev_err(i2c->dev, "Get transfer done. But Bytes not enough\n");
		}
	}
out:
	writel(reg_val, i2c->regs +  HSI2C_INT_STATUS);

	return IRQ_HANDLED;
>>>>>>> upstream/android-13
}

static int exynos5_i2c_xfer_msg(struct exynos5_i2c *i2c,
			      struct i2c_msg *msgs, int stop)
{
	unsigned long timeout;
<<<<<<< HEAD
	int ret;
=======
	unsigned long timeout_max;
	unsigned long trans_status;
	unsigned long i2c_ctl;
	unsigned long i2c_auto_conf;
	unsigned long i2c_timeout;
	unsigned long i2c_addr;
	unsigned long i2c_int_en;
	unsigned long i2c_fifo_ctl;
	unsigned char byte;
	int ret = 0;
	int operation_mode = i2c->operation_mode;
>>>>>>> upstream/android-13

	i2c->msg = msgs;
	i2c->msg_ptr = 0;
	i2c->trans_done = 0;

<<<<<<< HEAD
	reinit_completion(&i2c->msg_complete);

	exynos5_i2c_message_start(i2c, stop);

	timeout = wait_for_completion_timeout(&i2c->msg_complete,
					      EXYNOS5_I2C_TIMEOUT);
	if (timeout == 0)
		ret = -ETIMEDOUT;
	else
		ret = i2c->state;

	/*
	 * If this is the last message to be transfered (stop == 1)
	 * Then check if the bus can be brought back to idle.
	 */
	if (ret == 0 && stop)
		ret = exynos5_i2c_wait_bus_idle(i2c);

	if (ret < 0) {
		exynos5_i2c_reset(i2c);
		if (ret == -ETIMEDOUT)
			dev_warn(i2c->dev, "%s timeout\n",
				 (msgs->flags & I2C_M_RD) ? "rx" : "tx");
	}

	/* Return the state as in interrupt routine */
=======
	/* (length * (bits + ack) * (s/ms) * / freq) * (tolerance) */
	timeout_max = (i2c->msg->len * 9 * 1000 / i2c->clock_frequency) * 4;
	/* Minimum timeout is 300ms */
	if (timeout_max < i2c->xfer_timeout_ms)
		timeout_max = i2c->xfer_timeout_ms;

	reinit_completion(&i2c->msg_complete);

	i2c_ctl = readl(i2c->regs + HSI2C_CTL);
	i2c_auto_conf = readl(i2c->regs + HSI2C_AUTO_CONF);
	i2c_timeout = readl(i2c->regs + HSI2C_TIMEOUT);
	i2c_timeout &= ~HSI2C_TIMEOUT_EN;
	writel(i2c_timeout, i2c->regs + HSI2C_TIMEOUT);

	/*
	 * In case of short length request it'd be better to set
	 * trigger level as msg length
	 */
	if (i2c->msg->len >= FIFO_TRIG_CRITERIA) {
		i2c_fifo_ctl = HSI2C_RXFIFO_EN | HSI2C_TXFIFO_EN |
			HSI2C_RXFIFO_TRIGGER_LEVEL(FIFO_TRIG_CRITERIA) |
			HSI2C_TXFIFO_TRIGGER_LEVEL(FIFO_TRIG_CRITERIA);
	} else {
		i2c_fifo_ctl = HSI2C_RXFIFO_EN | HSI2C_TXFIFO_EN |
			HSI2C_RXFIFO_TRIGGER_LEVEL(i2c->msg->len) |
			HSI2C_TXFIFO_TRIGGER_LEVEL(i2c->msg->len);
	}

	writel(i2c_fifo_ctl, i2c->regs + HSI2C_FIFO_CTL);

	i2c_int_en = 0;
	if (msgs->flags & I2C_M_RD) {
		i2c_ctl &= ~HSI2C_TXCHON;
		i2c_ctl |= HSI2C_RXCHON;

		i2c_auto_conf |= HSI2C_READ_WRITE;

		i2c_int_en |= (HSI2C_INT_RX_ALMOSTFULL_EN |
			HSI2C_INT_TRAILING_EN);
	} else {
		i2c_ctl &= ~HSI2C_RXCHON;
		i2c_ctl |= HSI2C_TXCHON;

		i2c_auto_conf &= ~HSI2C_READ_WRITE;

		i2c_int_en |= HSI2C_INT_TX_ALMOSTEMPTY_EN;
	}

	if (operation_mode == HSI2C_INTERRUPT)
		exynos5_i2c_clr_pend_irq(i2c);

	if ((stop == 1) || (i2c->stop_after_trans == 1))
		i2c_auto_conf |= HSI2C_STOP_AFTER_TRANS;
	else
		i2c_auto_conf &= ~HSI2C_STOP_AFTER_TRANS;

	if (i2c->is_no_arbitration)
		i2c_ctl |= HSI2C_NO_LOSE_ARBITRATION;

	i2c_addr = readl(i2c->regs + HSI2C_ADDR);
	i2c_addr &= ~(0x3ff << 10);
	i2c_addr &= ~(0x3ff << 0);
	if (i2c->speed_mode != HSI2C_HIGH_SPD) {
		i2c_addr &= ~(0xff << 24);
		i2c_addr |= (0x7 << 24);
	}
	i2c_addr |= ((msgs->addr & 0x7f) << 10);
	writel(i2c_addr, i2c->regs + HSI2C_ADDR);

	writel(i2c_ctl, i2c->regs + HSI2C_CTL);

	if (operation_mode == HSI2C_INTERRUPT) {
		if (i2c->use_all_core) {
			unsigned int cpu = raw_smp_processor_id();

			irq_set_affinity_hint(i2c->irq, cpumask_of(cpu));
		}
		i2c_int_en |= HSI2C_INT_CHK_TRANS_STATE | HSI2C_INT_TRANSFER_DONE;
		writel(i2c_int_en, i2c->regs + HSI2C_INT_ENABLE);

		enable_irq(i2c->irq);
	} else {
		writel(HSI2C_INT_TRANSFER_DONE, i2c->regs + HSI2C_INT_ENABLE);
	}

	i2c_auto_conf &= ~(0xffff);
	i2c_auto_conf |= i2c->msg->len;
	writel(i2c_auto_conf, i2c->regs + HSI2C_AUTO_CONF);

	i2c_auto_conf = readl(i2c->regs + HSI2C_AUTO_CONF);
	i2c_auto_conf |= HSI2C_MASTER_RUN;
	writel(i2c_auto_conf, i2c->regs + HSI2C_AUTO_CONF);

	ret = -EAGAIN;
	if (msgs->flags & I2C_M_RD) {
		if (operation_mode == HSI2C_POLLING) {
			timeout = jiffies + msecs_to_jiffies(timeout_max);
			while (time_before(jiffies, timeout)){
				if ((readl(i2c->regs + HSI2C_FIFO_STATUS) &
					HSI2C_RX_FIFO_EMPTY) == 0) {
					/* RX FIFO is not empty */
					byte = (unsigned char)readl
						(i2c->regs + HSI2C_RX_DATA);
					i2c->msg->buf[i2c->msg_ptr++]
						= byte;
				}

				if (i2c->msg_ptr >= i2c->msg->len) {
					ret = 0;
					break;
				}
			}

			if (ret == -EAGAIN) {
				dump_i2c_register(i2c);
				exynos5_i2c_reset(i2c);
				dev_warn(i2c->dev, "rx timeout\n");
				return ret;
			}
		} else {
			timeout = wait_for_completion_timeout
				(&i2c->msg_complete, msecs_to_jiffies(timeout_max));

			ret = 0;

			if (i2c->scl_clk_stretch) {
				unsigned long timeout = jiffies + msecs_to_jiffies(100);

				do {
					trans_status = readl(i2c->regs + HSI2C_TRANS_STATUS);
					if ((trans_status & HSI2C_MAST_ST_MASK) == HSI2C_MASTER_ST_INIT){
						timeout = 0;
						break;
					}
				} while(time_before(jiffies, timeout));

				if (timeout)
					dev_err(i2c->dev, "SDA check timeout AT READ!!! = 0x%8lx\n",trans_status);
			}

			disable_irq(i2c->irq);

			if (i2c->trans_done < 0) {
				dev_err(i2c->dev, "ack was not received at read\n");
				ret = i2c->trans_done;
				exynos5_i2c_reset(i2c);
			}

			if (timeout == 0) {
				dump_i2c_register(i2c);
				exynos5_i2c_reset(i2c);
				dev_warn(i2c->dev, "rx timeout\n");
				ret = -EAGAIN;
				return ret;
			}
		}
	} else {
		if (operation_mode == HSI2C_POLLING) {
			unsigned long int_status;
			unsigned long fifo_status;
			timeout = jiffies + msecs_to_jiffies(timeout_max);
			while (time_before(jiffies, timeout) &&
				(i2c->msg_ptr < i2c->msg->len)) {
				if ((readl(i2c->regs + HSI2C_FIFO_STATUS)
					& HSI2C_TX_FIFO_LVL_MASK) < EXYNOS5_FIFO_SIZE) {
					byte = i2c->msg->buf[i2c->msg_ptr++];
					writel(byte, i2c->regs + HSI2C_TX_DATA);
				}
			}
			while (time_before(jiffies, timeout)) {
				int_status = readl(i2c->regs + HSI2C_INT_STATUS);
				fifo_status = readl(i2c->regs + HSI2C_FIFO_STATUS);
				if (int_status & HSI2C_INT_TRANSFER_DONE &&
					fifo_status & HSI2C_TX_FIFO_EMPTY) {
					writel(int_status, i2c->regs +  HSI2C_INT_STATUS);
					ret = 0;
					break;
				}
				udelay(1);
			}
			if (ret == -EAGAIN) {
				dump_i2c_register(i2c);
				exynos5_i2c_reset(i2c);
				dev_warn(i2c->dev, "tx timeout\n");
				return ret;
			}
		} else {
			timeout = wait_for_completion_timeout
				(&i2c->msg_complete, msecs_to_jiffies(timeout_max));
			disable_irq(i2c->irq);

			if (timeout == 0) {
				dump_i2c_register(i2c);
				exynos5_i2c_reset(i2c);
				dev_warn(i2c->dev, "tx timeout\n");
				return ret;
			}

			timeout = jiffies + timeout;
			if (i2c->trans_done < 0) {
				dev_err(i2c->dev, "ack was not received at write\n");
				ret = i2c->trans_done;
				exynos5_i2c_reset(i2c);
			} else {
				if (i2c->scl_clk_stretch) {
					unsigned long timeout = jiffies + msecs_to_jiffies(100);

					do {
						trans_status = readl(i2c->regs + HSI2C_TRANS_STATUS);
						if ((trans_status & HSI2C_MAST_ST_MASK) == HSI2C_MASTER_ST_INIT){
							timeout = 0;
							break;
						}
					} while(time_before(jiffies, timeout));

					if (timeout)
						dev_err(i2c->dev, "SDA check timeout AT WRITE!!! = 0x%8lx\n",trans_status);
				}

				ret = 0;
			}
		}
	}

>>>>>>> upstream/android-13
	return ret;
}

static int exynos5_i2c_xfer(struct i2c_adapter *adap,
			struct i2c_msg *msgs, int num)
{
<<<<<<< HEAD
	struct exynos5_i2c *i2c = adap->algo_data;
	int i, ret;
=======
	struct exynos5_i2c *i2c = (struct exynos5_i2c *)adap->algo_data;
	struct i2c_msg *msgs_ptr = msgs;
	int retry, i = 0;
	int ret = 0;
	int stop = 0;

#ifdef CONFIG_PM
	int clk_ret = 0;
#endif
>>>>>>> upstream/android-13

	if (i2c->suspended) {
		dev_err(i2c->dev, "HS-I2C is not initialized.\n");
		return -EIO;
	}

<<<<<<< HEAD
	ret = clk_enable(i2c->clk);
	if (ret)
		return ret;

	for (i = 0; i < num; ++i) {
		ret = exynos5_i2c_xfer_msg(i2c, msgs + i, i + 1 == num);
		if (ret)
			break;
	}

	clk_disable(i2c->clk);

	return ret ?: num;
=======
#ifdef CONFIG_PM
	clk_ret = pm_runtime_get_sync(i2c->dev);
	if (clk_ret < 0) {
#ifdef CONFIG_CPU_IDLE
		exynos_update_ip_idle_status(i2c->idle_ip_index, 0);
#endif
		ret = clk_enable(i2c->clk);
		if (ret) {
#ifdef CONFIG_CPU_IDLE
			exynos_update_ip_idle_status(i2c->idle_ip_index, 1);
#endif
			return ret;
		}
	}
#else
#ifdef CONFIG_CPU_IDLE
	exynos_update_ip_idle_status(i2c->idle_ip_index, 0);
#endif
	ret = clk_enable(i2c->clk);
	if (ret) {
#ifdef CONFIG_CPU_IDLE
		exynos_update_ip_idle_status(i2c->idle_ip_index, 1);
#endif
		return ret;
	}
#endif

	/* If master is in arbitration lost state before transfer */
	/* master should be reset */
	if (i2c->reset_before_trans) {
		if (unlikely((readl(i2c->regs + HSI2C_TRANS_STATUS)
			& HSI2C_MAST_ST_MASK) == 0x6)) {
			pr_info("%s trans status and reset = %s\n", __func__, adap->name);
			i2c->need_hw_init = 1;
		}
	}

	if (i2c->need_hw_init)
		exynos5_i2c_reset(i2c);

	if (unlikely(!(readl(i2c->regs + HSI2C_CONF)
			& HSI2C_AUTO_MODE))) {
		dev_err(i2c->dev, "HSI2C should be reconfigured\n");
		exynos5_hsi2c_clock_setup(i2c);
		exynos5_i2c_init(i2c);
	}

	for (retry = 0; retry < adap->retries; retry++) {
		for (i = 0; i < num; i++) {
			stop = (i == num - 1);

			if (i2c->transfer_delay)
				udelay(i2c->transfer_delay);

			ret = exynos5_i2c_xfer_msg(i2c, msgs_ptr, stop);

			msgs_ptr++;

			if (ret == -EAGAIN) {
				msgs_ptr = msgs;
				break;
			} else if (ret < 0) {
				goto out;
			}
		}

		if ((i == num) && (ret != -EAGAIN))
			break;

		dev_dbg(i2c->dev, "retrying transfer (%d)\n", retry);

		udelay(100);
	}

	if (i == num) {
		ret = num;
	} else {
		ret = -EREMOTEIO;
		dev_warn(i2c->dev, "xfer message failed\n");
	}

 out:
#ifdef CONFIG_PM
	if (clk_ret < 0) {
		clk_disable(i2c->clk);
#ifdef CONFIG_CPU_IDLE
		exynos_update_ip_idle_status(i2c->idle_ip_index, 1);
#endif
	}
	pm_runtime_mark_last_busy(i2c->dev);
	pm_runtime_put_autosuspend(i2c->dev);
#else
	clk_disable(i2c->clk);
#ifdef CONFIG_CPU_IDLE
	exynos_update_ip_idle_status(i2c->idle_ip_index, 1);
#endif
#endif

	return ret;
>>>>>>> upstream/android-13
}

static u32 exynos5_i2c_func(struct i2c_adapter *adap)
{
	return I2C_FUNC_I2C | (I2C_FUNC_SMBUS_EMUL & ~I2C_FUNC_SMBUS_QUICK);
}

static const struct i2c_algorithm exynos5_i2c_algorithm = {
	.master_xfer		= exynos5_i2c_xfer,
	.functionality		= exynos5_i2c_func,
};

static int exynos5_i2c_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
<<<<<<< HEAD
	struct exynos5_i2c *i2c;
	struct resource *mem;
	int ret;

	i2c = devm_kzalloc(&pdev->dev, sizeof(struct exynos5_i2c), GFP_KERNEL);
	if (!i2c)
		return -ENOMEM;

	if (of_property_read_u32(np, "clock-frequency", &i2c->op_clock))
		i2c->op_clock = HSI2C_FS_TX_CLOCK;
=======
	struct device *dev = &pdev->dev;
	struct exynos5_i2c *i2c;
	int ret;

	if (!np) {
		dev_err(&pdev->dev, "no device node\n");
		return -ENOENT;
	}

	i2c = devm_kzalloc(&pdev->dev, sizeof(struct exynos5_i2c), GFP_KERNEL);
	if (!i2c) {
		dev_err(&pdev->dev, "no memory for state\n");
		return -ENOMEM;
	}

	if (of_property_read_u32(np, "samsung,usi-offset", &i2c->usi_offset))
		dev_warn(dev, "usi offset is not specified\n");

	i2c->usi_reg = syscon_regmap_lookup_by_phandle(dev->of_node,
						"samsung,usi-phandle");
	if (IS_ERR(i2c->usi_reg)) {
		dev_info(dev, "no lookup for usi-phandle.\n");
	} else {
		u32 val;
		regmap_update_bits(i2c->usi_reg, i2c->usi_offset,
			USI_SW_CONF_MASK, USI_I2C_SW_CONF);
		regmap_read(i2c->usi_reg, i2c->usi_offset, &val);
		dev_info(dev, "set usi mode - 0x%x\n", val & USI_SW_CONF_MASK);
	}

	if (of_property_read_u32(np, "default-clk", &i2c->default_clk))
		dev_err(i2c->dev, "Failed to get default clk info\n");

	if (of_get_property(np, "samsung,no_lose_arbitration", NULL)) {
		i2c->is_no_arbitration = 1;
		dev_info(dev, "Disable lose arbitration\n");
	} else {
		i2c->is_no_arbitration = 0;
	}

	/* Mode of operation High/Fast/Fast+ Speed mode */
	if (of_property_read_u32(np, "clock-frequency", &i2c->clock_frequency)) {
		i2c->clock_frequency = HSI2C_FS_TX_CLOCK;
		i2c->fs_clock = i2c->clock_frequency;
		i2c->speed_mode = HSI2C_FAST_SPD;
	} else {

		if (i2c->clock_frequency <= HSI2C_STAND_TX_CLOCK) {
			i2c->stand_clock = i2c->clock_frequency;
			i2c->speed_mode = HSI2C_STAND_SPD;
		}
		else if (i2c->clock_frequency <= HSI2C_FS_TX_CLOCK) {
			i2c->fs_clock = i2c->clock_frequency;
			i2c->speed_mode = HSI2C_FAST_SPD;
		}
		else if (i2c->clock_frequency <= HSI2C_FAST_PLUS_TX_CLOCK) {
			i2c->fs_plus_clock = i2c->clock_frequency;
			i2c->speed_mode = HSI2C_FAST_PLUS_SPD;
		}
		else {
			if (of_get_property(np, "samsung,hs-mode", NULL)) {
				i2c->hs_clock = i2c->clock_frequency;
				i2c->speed_mode = HSI2C_HIGH_SPD;
			} else {
				i2c->fs_plus_clock = i2c->clock_frequency;
				i2c->speed_mode = HSI2C_FAST_PLUS_SPD;
			}
		}
	}

	ret = of_property_read_u32(np, "samsung,tscl-h", &i2c->tscl_h);
	if (!ret)
		dev_warn(&pdev->dev, "tSCL_HIGH val: 0x%x\n", i2c->tscl_h);

	ret = of_property_read_u32(np, "samsung,tscl-l", &i2c->tscl_l);
	if (!ret)
		dev_warn(&pdev->dev, "tSCL_LOW val: 0x%x\n", i2c->tscl_l);

	/* Mode of operation Polling/Interrupt mode */
	if (of_get_property(np, "samsung,polling-mode", NULL)) {
		i2c->operation_mode = HSI2C_POLLING;
	} else {
		i2c->operation_mode = HSI2C_INTERRUPT;
	}

	if (of_get_property(np, "samsung,scl-clk-stretching", NULL))
		i2c->scl_clk_stretch = 1;
	else
		i2c->scl_clk_stretch = 0;

	ret = of_property_read_u32(np, "samsung,transfer_delay", &i2c->transfer_delay);
	if (!ret)
		dev_warn(&pdev->dev, "Transfer delay is not needed.\n");

	if (of_get_property(np, "samsung,stop-after-trans", NULL))
		i2c->stop_after_trans = 1;
	else
		i2c->stop_after_trans = 0;

	if (of_get_property(np, "samsung,reset-before-trans", NULL))
		i2c->reset_before_trans = 1;
	else
		i2c->reset_before_trans = 0;

	if (of_get_property(np, "samsung,no-dev-restart", NULL))
		i2c->nack_restart = 1;
	else
		i2c->nack_restart = 0;

	if (of_property_read_u32(np, "samsung,autosuspend-delay",
				&i2c->autosuspend_delay))
		i2c->autosuspend_delay = EXYNOS5_HSI2C_RUNTIME_PM_DELAY;
	dev_dbg(&pdev->dev, "autosuspend-delay : %d ms\n", i2c->autosuspend_delay);

	i2c->use_all_core = of_property_read_bool(np, "samsung,use-all-core");

	ret = of_property_read_u32(np, "samsung,i2c-timeout-ms", &i2c->xfer_timeout_ms);
	if (ret)
		i2c->xfer_timeout_ms = EXYNOS5_I2C_TIMEOUT;
	dev_info(&pdev->dev, "timeout : %d ms\n", i2c->xfer_timeout_ms);

	ret = of_property_read_u32(np, "samsung,filter-cycle", &i2c->filter_cycle);
	if (ret)
		i2c->filter_cycle = EXYNOS5_I2C_DEFAULT_FILTER_CYCLE;

#ifdef CONFIG_CPU_IDLE
	i2c->idle_ip_index = exynos_get_idle_ip_index(dev_name(&pdev->dev), 1);
#endif
>>>>>>> upstream/android-13

	strlcpy(i2c->adap.name, "exynos5-i2c", sizeof(i2c->adap.name));
	i2c->adap.owner   = THIS_MODULE;
	i2c->adap.algo    = &exynos5_i2c_algorithm;
<<<<<<< HEAD
	i2c->adap.retries = 3;

	i2c->dev = &pdev->dev;
	i2c->clk = devm_clk_get(&pdev->dev, "hsi2c");
=======
	i2c->adap.retries = 2;
	i2c->adap.class   = I2C_CLASS_HWMON | I2C_CLASS_SPD;

	i2c->dev = &pdev->dev;
	i2c->clk = devm_clk_get(&pdev->dev, "gate_hsi2c_clk");
>>>>>>> upstream/android-13
	if (IS_ERR(i2c->clk)) {
		dev_err(&pdev->dev, "cannot get clock\n");
		return -ENOENT;
	}

<<<<<<< HEAD
	ret = clk_prepare_enable(i2c->clk);
	if (ret)
		return ret;

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	i2c->regs = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(i2c->regs)) {
		ret = PTR_ERR(i2c->regs);
		goto err_clk;
=======
	i2c->rate_clk = devm_clk_get(&pdev->dev, "ipclk_hsi2c");
	if (IS_ERR(i2c->rate_clk)) {
		dev_err(&pdev->dev, "cannot get rate clock\n");
		return -ENOENT;
	}

	ret = clk_prepare(i2c->clk);
	if (ret) {
		dev_err(&pdev->dev, "Clock prepare failed\n");
			return ret;
	}

#ifdef CONFIG_PM
	pm_runtime_use_autosuspend(&pdev->dev);
	pm_runtime_set_autosuspend_delay(&pdev->dev, i2c->autosuspend_delay);
	pm_runtime_enable(&pdev->dev);
#endif

	i2c->regs = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(i2c->regs)) {
		dev_err(&pdev->dev, "cannot map HS-I2C IO\n");
		ret = PTR_ERR(i2c->regs);
		goto err_clk1;
>>>>>>> upstream/android-13
	}

	i2c->adap.dev.of_node = np;
	i2c->adap.algo_data = i2c;
	i2c->adap.dev.parent = &pdev->dev;

<<<<<<< HEAD
	/* Clear pending interrupts from u-boot or misc causes */
	exynos5_i2c_clr_pend_irq(i2c);

	spin_lock_init(&i2c->lock);
	init_completion(&i2c->msg_complete);

	i2c->irq = ret = platform_get_irq(pdev, 0);
	if (ret <= 0) {
		dev_err(&pdev->dev, "cannot find HS-I2C IRQ\n");
		ret = -EINVAL;
		goto err_clk;
	}

	ret = devm_request_irq(&pdev->dev, i2c->irq, exynos5_i2c_irq,
				IRQF_NO_SUSPEND | IRQF_ONESHOT,
				dev_name(&pdev->dev), i2c);

	if (ret != 0) {
		dev_err(&pdev->dev, "cannot request HS-I2C IRQ %d\n", i2c->irq);
		goto err_clk;
	}

	i2c->variant = of_device_get_match_data(&pdev->dev);

	ret = exynos5_hsi2c_clock_setup(i2c);
	if (ret)
		goto err_clk;

	exynos5_i2c_reset(i2c);

	ret = i2c_add_adapter(&i2c->adap);
	if (ret < 0)
		goto err_clk;

	platform_set_drvdata(pdev, i2c);

	clk_disable(i2c->clk);

	return 0;

 err_clk:
	clk_disable_unprepare(i2c->clk);
=======
	init_completion(&i2c->msg_complete);

	platform_set_drvdata(pdev, i2c);
#ifdef CONFIG_PM
	pm_runtime_get_sync(&pdev->dev);
#else
#ifdef CONFIG_CPU_IDLE
	exynos_update_ip_idle_status(i2c->idle_ip_index, 0);
#endif
	ret = clk_enable(i2c->clk);
	if (ret) {
#ifdef CONFIG_CPU_IDLE
		exynos_update_ip_idle_status(i2c->idle_ip_index, 1);
#endif
		return ret;
	}
#endif
	exynos_usi_init(i2c);

	/* Clear pending interrupts from u-boot or misc causes */
	exynos5_i2c_clr_pend_irq(i2c);
	/* Reset i2c SFR from u-boot or misc causes */
	exynos5_i2c_reset(i2c);

	if (i2c->operation_mode == HSI2C_INTERRUPT) {
		i2c->irq = ret = irq_of_parse_and_map(np, 0);
		if (ret <= 0) {
			dev_err(&pdev->dev, "cannot find HS-I2C IRQ\n");
			ret = -EINVAL;
			goto err_clk1;
		}

		ret = devm_request_irq(&pdev->dev, i2c->irq,
					exynos5_i2c_irq, IRQF_NO_THREAD, dev_name(&pdev->dev), i2c);
		disable_irq(i2c->irq);

		if (ret != 0) {
			dev_err(&pdev->dev, "cannot request HS-I2C IRQ %d\n",
					i2c->irq);
			goto err_clk1;
		}

		if (!i2c->use_all_core) {
			cpulist_parse("0-3", &i2c->affinity_hint);
			cpumask_and(&i2c->affinity_hint, &i2c->affinity_hint, cpu_possible_mask);
			cpumask_and(&i2c->affinity_hint, &i2c->affinity_hint, cpu_online_mask);
			irq_set_affinity_hint(i2c->irq, &i2c->affinity_hint);
		}
	}

	i2c->bus_id = of_alias_get_id(i2c->adap.dev.of_node, "hsi2c");

	exynos5_i2c_init(i2c);

	i2c->adap.nr = -1;
	ret = i2c_add_numbered_adapter(&i2c->adap);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to add bus to i2c core\n");
		goto err_clk2;
	}

#ifdef CONFIG_PM
	pm_runtime_mark_last_busy(&pdev->dev);
	pm_runtime_put_autosuspend(&pdev->dev);
#else
	clk_disable(i2c->clk);
#ifdef CONFIG_CPU_IDLE
	exynos_update_ip_idle_status(i2c->idle_ip_index, 1);
#endif
#endif
	return 0;

 err_clk2:
#ifdef CONFIG_PM
	pm_runtime_mark_last_busy(&pdev->dev);
	pm_runtime_put_autosuspend(&pdev->dev);
#else
	clk_disable_unprepare(i2c->clk);
#ifdef CONFIG_CPU_IDLE
	exynos_update_ip_idle_status(i2c->idle_ip_index, 1);
#endif
#endif
 err_clk1:
>>>>>>> upstream/android-13
	return ret;
}

static int exynos5_i2c_remove(struct platform_device *pdev)
{
	struct exynos5_i2c *i2c = platform_get_drvdata(pdev);

	i2c_del_adapter(&i2c->adap);

	clk_unprepare(i2c->clk);

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM_SLEEP
static int exynos5_i2c_suspend_noirq(struct device *dev)
{
	struct exynos5_i2c *i2c = dev_get_drvdata(dev);

	i2c->suspended = 1;

	clk_unprepare(i2c->clk);
=======
#ifdef CONFIG_PM
static int exynos5_i2c_runtime_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct exynos5_i2c *i2c = platform_get_drvdata(pdev);

	clk_disable(i2c->clk);
#ifdef CONFIG_CPU_IDLE
	exynos_update_ip_idle_status(i2c->idle_ip_index, 1);
#endif
	i2c->runtime_resumed = 0;

	return 0;
}

static int exynos5_i2c_runtime_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct exynos5_i2c *i2c = platform_get_drvdata(pdev);
	int ret = 0;

#ifdef CONFIG_CPU_IDLE
	exynos_update_ip_idle_status(i2c->idle_ip_index, 0);
#endif
	ret = clk_enable(i2c->clk);
	i2c->runtime_resumed = 1;
	if (ret) {
#ifdef CONFIG_CPU_IDLE
		exynos_update_ip_idle_status(i2c->idle_ip_index, 1);
#endif
		return ret;
	}

	return 0;
}
#endif /* CONFIG_PM */

#ifdef CONFIG_PM
static int exynos5_i2c_suspend_noirq(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct exynos5_i2c *i2c = platform_get_drvdata(pdev);
#ifdef CONFIG_I2C_SAMSUNG_HWACG
	int ret = 0;
#endif

	i2c_lock_bus(&i2c->adap, I2C_LOCK_ROOT_ADAPTER);
#ifdef CONFIG_I2C_SAMSUNG_HWACG
#ifdef CONFIG_CPU_IDLE
	exynos_update_ip_idle_status(i2c->idle_ip_index, 0);
#endif
	ret = clk_enable(i2c->clk);
	if (ret) {
#ifdef CONFIG_CPU_IDLE
		exynos_update_ip_idle_status(i2c->idle_ip_index, 1);
#endif
		i2c_unlock_bus(&i2c->adap, I2C_LOCK_ROOT_ADAPTER);
		return ret;
	}
	writel(HSI2C_SW_RST, i2c->regs + HSI2C_CTL);
	clk_disable(i2c->clk);
#ifdef CONFIG_CPU_IDLE
	exynos_update_ip_idle_status(i2c->idle_ip_index, 1);
#endif
#endif

	if (!pm_runtime_status_suspended(dev))
		exynos5_i2c_runtime_suspend(dev);

	i2c->suspended = 1;
	i2c_unlock_bus(&i2c->adap, I2C_LOCK_ROOT_ADAPTER);
>>>>>>> upstream/android-13

	return 0;
}

static int exynos5_i2c_resume_noirq(struct device *dev)
{
<<<<<<< HEAD
	struct exynos5_i2c *i2c = dev_get_drvdata(dev);
	int ret = 0;

	ret = clk_prepare_enable(i2c->clk);
	if (ret)
		return ret;

	ret = exynos5_hsi2c_clock_setup(i2c);
	if (ret) {
		clk_disable_unprepare(i2c->clk);
		return ret;
	}

	exynos5_i2c_init(i2c);
	clk_disable(i2c->clk);
	i2c->suspended = 0;

	return 0;
}
#endif

static const struct dev_pm_ops exynos5_i2c_dev_pm_ops = {
	SET_NOIRQ_SYSTEM_SLEEP_PM_OPS(exynos5_i2c_suspend_noirq,
				      exynos5_i2c_resume_noirq)
=======
	struct platform_device *pdev = to_platform_device(dev);
	struct exynos5_i2c *i2c = platform_get_drvdata(pdev);
	int ret = 0;

	if (!IS_ERR(i2c->usi_reg))
		regmap_update_bits(i2c->usi_reg, i2c->usi_offset,
			USI_SW_CONF_MASK, USI_I2C_SW_CONF);

	i2c_lock_bus(&i2c->adap, I2C_LOCK_ROOT_ADAPTER);

	if (!pm_runtime_status_suspended(dev))
		exynos5_i2c_runtime_resume(dev);

#ifdef CONFIG_CPU_IDLE
	exynos_update_ip_idle_status(i2c->idle_ip_index, 0);
#endif
	ret = clk_enable(i2c->clk);
	if (ret) {
#ifdef CONFIG_CPU_IDLE
		exynos_update_ip_idle_status(i2c->idle_ip_index, 1);
#endif
		i2c_unlock_bus(&i2c->adap, I2C_LOCK_ROOT_ADAPTER);
		return ret;
	}

	exynos_usi_init(i2c);
	exynos5_i2c_reset(i2c);
	clk_disable(i2c->clk);
#ifdef CONFIG_CPU_IDLE
	exynos_update_ip_idle_status(i2c->idle_ip_index, 1);
#endif
	i2c->suspended = 0;
	i2c_unlock_bus(&i2c->adap, I2C_LOCK_ROOT_ADAPTER);

	return 0;
}

#else
static int exynos5_i2c_suspend_noirq(struct device *dev)
{
	return 0;
}

static int exynos5_i2c_resume_noirq(struct device *dev)
{
	return 0;
}
#endif

static const struct dev_pm_ops exynos5_i2c_pm = {
	SET_NOIRQ_SYSTEM_SLEEP_PM_OPS(exynos5_i2c_suspend_noirq,
				      exynos5_i2c_resume_noirq)
	SET_RUNTIME_PM_OPS(exynos5_i2c_runtime_suspend,
			   exynos5_i2c_runtime_resume, NULL)
>>>>>>> upstream/android-13
};

static struct platform_driver exynos5_i2c_driver = {
	.probe		= exynos5_i2c_probe,
	.remove		= exynos5_i2c_remove,
	.driver		= {
		.name	= "exynos5-hsi2c",
<<<<<<< HEAD
		.pm	= &exynos5_i2c_dev_pm_ops,
=======
		.pm	= &exynos5_i2c_pm,
>>>>>>> upstream/android-13
		.of_match_table = exynos5_i2c_match,
	},
};

module_platform_driver(exynos5_i2c_driver);

MODULE_DESCRIPTION("Exynos5 HS-I2C Bus driver");
<<<<<<< HEAD
MODULE_AUTHOR("Naveen Krishna Chatradhi, <ch.naveen@samsung.com>");
MODULE_AUTHOR("Taekgyun Ko, <taeggyun.ko@samsung.com>");
=======
MODULE_AUTHOR("Naveen Krishna Chatradhi <ch.naveen@samsung.com>");
MODULE_AUTHOR("Taekgyun Ko <taeggyun.ko@samsung.com>");
>>>>>>> upstream/android-13
MODULE_LICENSE("GPL v2");
