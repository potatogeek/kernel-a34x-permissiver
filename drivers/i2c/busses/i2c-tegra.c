<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * drivers/i2c/busses/i2c-tegra.c
 *
 * Copyright (C) 2010 Google, Inc.
 * Author: Colin Cross <ccross@android.com>
<<<<<<< HEAD
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/of_device.h>
#include <linux/module.h>
#include <linux/reset.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pm_runtime.h>
#include <linux/iopoll.h>

#include <asm/unaligned.h>

#define TEGRA_I2C_TIMEOUT (msecs_to_jiffies(1000))
#define BYTES_PER_FIFO_WORD 4

#define I2C_CNFG				0x000
#define I2C_CNFG_DEBOUNCE_CNT_SHIFT		12
#define I2C_CNFG_PACKET_MODE_EN			BIT(10)
#define I2C_CNFG_NEW_MASTER_FSM			BIT(11)
#define I2C_CNFG_MULTI_MASTER_MODE		BIT(17)
#define I2C_STATUS				0x01C
=======
 */

#include <linux/bitfield.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/pinctrl/consumer.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/reset.h>

#define BYTES_PER_FIFO_WORD 4

#define I2C_CNFG				0x000
#define I2C_CNFG_DEBOUNCE_CNT			GENMASK(14, 12)
#define I2C_CNFG_PACKET_MODE_EN			BIT(10)
#define I2C_CNFG_NEW_MASTER_FSM			BIT(11)
#define I2C_CNFG_MULTI_MASTER_MODE		BIT(17)
#define I2C_STATUS				0x01c
>>>>>>> upstream/android-13
#define I2C_SL_CNFG				0x020
#define I2C_SL_CNFG_NACK			BIT(1)
#define I2C_SL_CNFG_NEWSL			BIT(2)
#define I2C_SL_ADDR1				0x02c
#define I2C_SL_ADDR2				0x030
<<<<<<< HEAD
=======
#define I2C_TLOW_SEXT				0x034
>>>>>>> upstream/android-13
#define I2C_TX_FIFO				0x050
#define I2C_RX_FIFO				0x054
#define I2C_PACKET_TRANSFER_STATUS		0x058
#define I2C_FIFO_CONTROL			0x05c
#define I2C_FIFO_CONTROL_TX_FLUSH		BIT(1)
#define I2C_FIFO_CONTROL_RX_FLUSH		BIT(0)
<<<<<<< HEAD
#define I2C_FIFO_CONTROL_TX_TRIG_SHIFT		5
#define I2C_FIFO_CONTROL_RX_TRIG_SHIFT		2
#define I2C_FIFO_STATUS				0x060
#define I2C_FIFO_STATUS_TX_MASK			0xF0
#define I2C_FIFO_STATUS_TX_SHIFT		4
#define I2C_FIFO_STATUS_RX_MASK			0x0F
#define I2C_FIFO_STATUS_RX_SHIFT		0
#define I2C_INT_MASK				0x064
#define I2C_INT_STATUS				0x068
#define I2C_INT_PACKET_XFER_COMPLETE		BIT(7)
#define I2C_INT_ALL_PACKETS_XFER_COMPLETE	BIT(6)
#define I2C_INT_TX_FIFO_OVERFLOW		BIT(5)
#define I2C_INT_RX_FIFO_UNDERFLOW		BIT(4)
=======
#define I2C_FIFO_CONTROL_TX_TRIG(x)		(((x) - 1) << 5)
#define I2C_FIFO_CONTROL_RX_TRIG(x)		(((x) - 1) << 2)
#define I2C_FIFO_STATUS				0x060
#define I2C_FIFO_STATUS_TX			GENMASK(7, 4)
#define I2C_FIFO_STATUS_RX			GENMASK(3, 0)
#define I2C_INT_MASK				0x064
#define I2C_INT_STATUS				0x068
#define I2C_INT_BUS_CLR_DONE			BIT(11)
#define I2C_INT_PACKET_XFER_COMPLETE		BIT(7)
>>>>>>> upstream/android-13
#define I2C_INT_NO_ACK				BIT(3)
#define I2C_INT_ARBITRATION_LOST		BIT(2)
#define I2C_INT_TX_FIFO_DATA_REQ		BIT(1)
#define I2C_INT_RX_FIFO_DATA_REQ		BIT(0)
#define I2C_CLK_DIVISOR				0x06c
<<<<<<< HEAD
#define I2C_CLK_DIVISOR_STD_FAST_MODE_SHIFT	16
#define I2C_CLK_MULTIPLIER_STD_FAST_MODE	8

#define DVC_CTRL_REG1				0x000
#define DVC_CTRL_REG1_INTR_EN			BIT(10)
#define DVC_CTRL_REG2				0x004
=======
#define I2C_CLK_DIVISOR_STD_FAST_MODE		GENMASK(31, 16)
#define I2C_CLK_DIVISOR_HSMODE			GENMASK(15, 0)

#define DVC_CTRL_REG1				0x000
#define DVC_CTRL_REG1_INTR_EN			BIT(10)
>>>>>>> upstream/android-13
#define DVC_CTRL_REG3				0x008
#define DVC_CTRL_REG3_SW_PROG			BIT(26)
#define DVC_CTRL_REG3_I2C_DONE_INTR_EN		BIT(30)
#define DVC_STATUS				0x00c
#define DVC_STATUS_I2C_DONE_INTR		BIT(30)

#define I2C_ERR_NONE				0x00
<<<<<<< HEAD
#define I2C_ERR_NO_ACK				0x01
#define I2C_ERR_ARBITRATION_LOST		0x02
#define I2C_ERR_UNKNOWN_INTERRUPT		0x04

#define PACKET_HEADER0_HEADER_SIZE_SHIFT	28
#define PACKET_HEADER0_PACKET_ID_SHIFT		16
#define PACKET_HEADER0_CONT_ID_SHIFT		12
#define PACKET_HEADER0_PROTOCOL_I2C		BIT(4)

#define I2C_HEADER_HIGHSPEED_MODE		BIT(22)
#define I2C_HEADER_CONT_ON_NAK			BIT(21)
#define I2C_HEADER_SEND_START_BYTE		BIT(20)
=======
#define I2C_ERR_NO_ACK				BIT(0)
#define I2C_ERR_ARBITRATION_LOST		BIT(1)
#define I2C_ERR_UNKNOWN_INTERRUPT		BIT(2)
#define I2C_ERR_RX_BUFFER_OVERFLOW		BIT(3)

#define PACKET_HEADER0_HEADER_SIZE		GENMASK(29, 28)
#define PACKET_HEADER0_PACKET_ID		GENMASK(23, 16)
#define PACKET_HEADER0_CONT_ID			GENMASK(15, 12)
#define PACKET_HEADER0_PROTOCOL			GENMASK(7, 4)
#define PACKET_HEADER0_PROTOCOL_I2C		1

#define I2C_HEADER_CONT_ON_NAK			BIT(21)
>>>>>>> upstream/android-13
#define I2C_HEADER_READ				BIT(19)
#define I2C_HEADER_10BIT_ADDR			BIT(18)
#define I2C_HEADER_IE_ENABLE			BIT(17)
#define I2C_HEADER_REPEAT_START			BIT(16)
#define I2C_HEADER_CONTINUE_XFER		BIT(15)
<<<<<<< HEAD
#define I2C_HEADER_MASTER_ADDR_SHIFT		12
#define I2C_HEADER_SLAVE_ADDR_SHIFT		1

#define I2C_CONFIG_LOAD				0x08C
#define I2C_MSTR_CONFIG_LOAD			BIT(0)
#define I2C_SLV_CONFIG_LOAD			BIT(1)
#define I2C_TIMEOUT_CONFIG_LOAD			BIT(2)
=======
#define I2C_HEADER_SLAVE_ADDR_SHIFT		1

#define I2C_BUS_CLEAR_CNFG			0x084
#define I2C_BC_SCLK_THRESHOLD			GENMASK(23, 16)
#define I2C_BC_STOP_COND			BIT(2)
#define I2C_BC_TERMINATE			BIT(1)
#define I2C_BC_ENABLE				BIT(0)
#define I2C_BUS_CLEAR_STATUS			0x088
#define I2C_BC_STATUS				BIT(0)

#define I2C_CONFIG_LOAD				0x08c
#define I2C_MSTR_CONFIG_LOAD			BIT(0)
>>>>>>> upstream/android-13

#define I2C_CLKEN_OVERRIDE			0x090
#define I2C_MST_CORE_CLKEN_OVR			BIT(0)

<<<<<<< HEAD
#define I2C_CONFIG_LOAD_TIMEOUT			1000000
=======
#define I2C_INTERFACE_TIMING_0			0x094
#define  I2C_INTERFACE_TIMING_THIGH		GENMASK(13, 8)
#define  I2C_INTERFACE_TIMING_TLOW		GENMASK(5, 0)
#define I2C_INTERFACE_TIMING_1			0x098
#define  I2C_INTERFACE_TIMING_TBUF		GENMASK(29, 24)
#define  I2C_INTERFACE_TIMING_TSU_STO		GENMASK(21, 16)
#define  I2C_INTERFACE_TIMING_THD_STA		GENMASK(13, 8)
#define  I2C_INTERFACE_TIMING_TSU_STA		GENMASK(5, 0)

#define I2C_HS_INTERFACE_TIMING_0		0x09c
#define  I2C_HS_INTERFACE_TIMING_THIGH		GENMASK(13, 8)
#define  I2C_HS_INTERFACE_TIMING_TLOW		GENMASK(5, 0)
#define I2C_HS_INTERFACE_TIMING_1		0x0a0
#define  I2C_HS_INTERFACE_TIMING_TSU_STO	GENMASK(21, 16)
#define  I2C_HS_INTERFACE_TIMING_THD_STA	GENMASK(13, 8)
#define  I2C_HS_INTERFACE_TIMING_TSU_STA	GENMASK(5, 0)
>>>>>>> upstream/android-13

#define I2C_MST_FIFO_CONTROL			0x0b4
#define I2C_MST_FIFO_CONTROL_RX_FLUSH		BIT(0)
#define I2C_MST_FIFO_CONTROL_TX_FLUSH		BIT(1)
#define I2C_MST_FIFO_CONTROL_RX_TRIG(x)		(((x) - 1) <<  4)
#define I2C_MST_FIFO_CONTROL_TX_TRIG(x)		(((x) - 1) << 16)

#define I2C_MST_FIFO_STATUS			0x0b8
<<<<<<< HEAD
#define I2C_MST_FIFO_STATUS_RX_MASK		0xff
#define I2C_MST_FIFO_STATUS_RX_SHIFT		0
#define I2C_MST_FIFO_STATUS_TX_MASK		0xff0000
#define I2C_MST_FIFO_STATUS_TX_SHIFT		16

/*
 * msg_end_type: The bus control which need to be send at end of transfer.
 * @MSG_END_STOP: Send stop pulse at end of transfer.
 * @MSG_END_REPEAT_START: Send repeat start at end of transfer.
 * @MSG_END_CONTINUE: The following on message is coming and so do not send
 *		stop or repeat start.
=======
#define I2C_MST_FIFO_STATUS_TX			GENMASK(23, 16)
#define I2C_MST_FIFO_STATUS_RX			GENMASK(7, 0)

/* configuration load timeout in microseconds */
#define I2C_CONFIG_LOAD_TIMEOUT			1000000

/* packet header size in bytes */
#define I2C_PACKET_HEADER_SIZE			12

/*
 * I2C Controller will use PIO mode for transfers up to 32 bytes in order to
 * avoid DMA overhead, otherwise external APB DMA controller will be used.
 * Note that the actual MAX PIO length is 20 bytes because 32 bytes include
 * I2C_PACKET_HEADER_SIZE.
 */
#define I2C_PIO_MODE_PREFERRED_LEN		32

/*
 * msg_end_type: The bus control which needs to be sent at end of transfer.
 * @MSG_END_STOP: Send stop pulse.
 * @MSG_END_REPEAT_START: Send repeat-start.
 * @MSG_END_CONTINUE: Don't send stop or repeat-start.
>>>>>>> upstream/android-13
 */
enum msg_end_type {
	MSG_END_STOP,
	MSG_END_REPEAT_START,
	MSG_END_CONTINUE,
};

/**
<<<<<<< HEAD
 * struct tegra_i2c_hw_feature : Different HW support on Tegra
 * @has_continue_xfer_support: Continue transfer supports.
 * @has_per_pkt_xfer_complete_irq: Has enable/disable capability for transfer
 *		complete interrupt per packet basis.
 * @has_single_clk_source: The I2C controller has single clock source. Tegra30
 *		and earlier SoCs have two clock sources i.e. div-clk and
 *		fast-clk.
 * @has_config_load_reg: Has the config load register to load the new
 *		configuration.
 * @clk_divisor_hs_mode: Clock divisor in HS mode.
 * @clk_divisor_std_fast_mode: Clock divisor in standard/fast mode. It is
=======
 * struct tegra_i2c_hw_feature : per hardware generation features
 * @has_continue_xfer_support: continue-transfer supported
 * @has_per_pkt_xfer_complete_irq: Has enable/disable capability for transfer
 *		completion interrupt on per packet basis.
 * @has_config_load_reg: Has the config load register to load the new
 *		configuration.
 * @clk_divisor_hs_mode: Clock divisor in HS mode.
 * @clk_divisor_std_mode: Clock divisor in standard mode. It is
 *		applicable if there is no fast clock source i.e. single clock
 *		source.
 * @clk_divisor_fast_mode: Clock divisor in fast mode. It is
>>>>>>> upstream/android-13
 *		applicable if there is no fast clock source i.e. single clock
 *		source.
 * @clk_divisor_fast_plus_mode: Clock divisor in fast mode plus. It is
 *		applicable if there is no fast clock source (i.e. single
 *		clock source).
 * @has_multi_master_mode: The I2C controller supports running in single-master
 *		or multi-master mode.
 * @has_slcg_override_reg: The I2C controller supports a register that
 *		overrides the second level clock gating.
 * @has_mst_fifo: The I2C controller contains the new MST FIFO interface that
 *		provides additional features and allows for longer messages to
 *		be transferred in one go.
<<<<<<< HEAD
 * @quirks: i2c adapter quirks for limiting write/read transfer size and not
 *		allowing 0 length transfers.
=======
 * @quirks: I2C adapter quirks for limiting write/read transfer size and not
 *		allowing 0 length transfers.
 * @supports_bus_clear: Bus Clear support to recover from bus hang during
 *		SDA stuck low from device for some unknown reasons.
 * @has_apb_dma: Support of APBDMA on corresponding Tegra chip.
 * @tlow_std_mode: Low period of the clock in standard mode.
 * @thigh_std_mode: High period of the clock in standard mode.
 * @tlow_fast_fastplus_mode: Low period of the clock in fast/fast-plus modes.
 * @thigh_fast_fastplus_mode: High period of the clock in fast/fast-plus modes.
 * @setup_hold_time_std_mode: Setup and hold time for start and stop conditions
 *		in standard mode.
 * @setup_hold_time_fast_fast_plus_mode: Setup and hold time for start and stop
 *		conditions in fast/fast-plus modes.
 * @setup_hold_time_hs_mode: Setup and hold time for start and stop conditions
 *		in HS mode.
 * @has_interface_timing_reg: Has interface timing register to program the tuned
 *		timing settings.
>>>>>>> upstream/android-13
 */
struct tegra_i2c_hw_feature {
	bool has_continue_xfer_support;
	bool has_per_pkt_xfer_complete_irq;
<<<<<<< HEAD
	bool has_single_clk_source;
	bool has_config_load_reg;
	int clk_divisor_hs_mode;
	int clk_divisor_std_fast_mode;
	u16 clk_divisor_fast_plus_mode;
=======
	bool has_config_load_reg;
	u32 clk_divisor_hs_mode;
	u32 clk_divisor_std_mode;
	u32 clk_divisor_fast_mode;
	u32 clk_divisor_fast_plus_mode;
>>>>>>> upstream/android-13
	bool has_multi_master_mode;
	bool has_slcg_override_reg;
	bool has_mst_fifo;
	const struct i2c_adapter_quirks *quirks;
<<<<<<< HEAD
=======
	bool supports_bus_clear;
	bool has_apb_dma;
	u32 tlow_std_mode;
	u32 thigh_std_mode;
	u32 tlow_fast_fastplus_mode;
	u32 thigh_fast_fastplus_mode;
	u32 setup_hold_time_std_mode;
	u32 setup_hold_time_fast_fast_plus_mode;
	u32 setup_hold_time_hs_mode;
	bool has_interface_timing_reg;
>>>>>>> upstream/android-13
};

/**
 * struct tegra_i2c_dev - per device I2C context
 * @dev: device reference for power management
 * @hw: Tegra I2C HW feature
 * @adapter: core I2C layer adapter information
 * @div_clk: clock reference for div clock of I2C controller
<<<<<<< HEAD
 * @fast_clk: clock reference for fast clock of I2C controller
 * @rst: reset control for the I2C controller
 * @base: ioremapped registers cookie
 * @cont_id: I2C controller ID, used for packet header
 * @irq: IRQ number of transfer complete interrupt
 * @irq_disabled: used to track whether or not the interrupt is enabled
 * @is_dvc: identifies the DVC I2C controller, has a different register layout
=======
 * @clocks: array of I2C controller clocks
 * @nclocks: number of clocks in the array
 * @rst: reset control for the I2C controller
 * @base: ioremapped registers cookie
 * @base_phys: physical base address of the I2C controller
 * @cont_id: I2C controller ID, used for packet header
 * @irq: IRQ number of transfer complete interrupt
 * @is_dvc: identifies the DVC I2C controller, has a different register layout
 * @is_vi: identifies the VI I2C controller, has a different register layout
>>>>>>> upstream/android-13
 * @msg_complete: transfer completion notifier
 * @msg_err: error code for completed message
 * @msg_buf: pointer to current message data
 * @msg_buf_remaining: size of unsent data in the message buffer
<<<<<<< HEAD
 * @msg_read: identifies read transfers
 * @bus_clk_rate: current I2C bus clock rate
 * @clk_divisor_non_hs_mode: clock divider for non-high-speed modes
 * @is_multimaster_mode: track if I2C controller is in multi-master mode
 * @xfer_lock: lock to serialize transfer submission and processing
 */
struct tegra_i2c_dev {
	struct device *dev;
	const struct tegra_i2c_hw_feature *hw;
	struct i2c_adapter adapter;
	struct clk *div_clk;
	struct clk *fast_clk;
	struct reset_control *rst;
	void __iomem *base;
	int cont_id;
	int irq;
	bool irq_disabled;
	int is_dvc;
	struct completion msg_complete;
	int msg_err;
	u8 *msg_buf;
	size_t msg_buf_remaining;
	int msg_read;
	u32 bus_clk_rate;
	u16 clk_divisor_non_hs_mode;
	bool is_multimaster_mode;
	spinlock_t xfer_lock;
};

static void dvc_writel(struct tegra_i2c_dev *i2c_dev, u32 val,
		       unsigned long reg)
{
	writel(val, i2c_dev->base + reg);
}

static u32 dvc_readl(struct tegra_i2c_dev *i2c_dev, unsigned long reg)
{
	return readl(i2c_dev->base + reg);
}

/*
 * i2c_writel and i2c_readl will offset the register if necessary to talk
 * to the I2C block inside the DVC block
 */
static unsigned long tegra_i2c_reg_addr(struct tegra_i2c_dev *i2c_dev,
	unsigned long reg)
{
	if (i2c_dev->is_dvc)
		reg += (reg >= I2C_TX_FIFO) ? 0x10 : 0x40;
	return reg;
}

static void i2c_writel(struct tegra_i2c_dev *i2c_dev, u32 val,
	unsigned long reg)
{
	writel(val, i2c_dev->base + tegra_i2c_reg_addr(i2c_dev, reg));

	/* Read back register to make sure that register writes completed */
	if (reg != I2C_TX_FIFO)
		readl(i2c_dev->base + tegra_i2c_reg_addr(i2c_dev, reg));
}

static u32 i2c_readl(struct tegra_i2c_dev *i2c_dev, unsigned long reg)
{
	return readl(i2c_dev->base + tegra_i2c_reg_addr(i2c_dev, reg));
}

static void i2c_writesl(struct tegra_i2c_dev *i2c_dev, void *data,
	unsigned long reg, int len)
=======
 * @msg_read: indicates that the transfer is a read access
 * @bus_clk_rate: current I2C bus clock rate
 * @multimaster_mode: indicates that I2C controller is in multi-master mode
 * @tx_dma_chan: DMA transmit channel
 * @rx_dma_chan: DMA receive channel
 * @dma_phys: handle to DMA resources
 * @dma_buf: pointer to allocated DMA buffer
 * @dma_buf_size: DMA buffer size
 * @dma_mode: indicates active DMA transfer
 * @dma_complete: DMA completion notifier
 * @atomic_mode: indicates active atomic transfer
 */
struct tegra_i2c_dev {
	struct device *dev;
	struct i2c_adapter adapter;

	const struct tegra_i2c_hw_feature *hw;
	struct reset_control *rst;
	unsigned int cont_id;
	unsigned int irq;

	phys_addr_t base_phys;
	void __iomem *base;

	struct clk_bulk_data clocks[2];
	unsigned int nclocks;

	struct clk *div_clk;
	u32 bus_clk_rate;

	struct completion msg_complete;
	size_t msg_buf_remaining;
	int msg_err;
	u8 *msg_buf;

	struct completion dma_complete;
	struct dma_chan *tx_dma_chan;
	struct dma_chan *rx_dma_chan;
	unsigned int dma_buf_size;
	dma_addr_t dma_phys;
	void *dma_buf;

	bool multimaster_mode;
	bool atomic_mode;
	bool dma_mode;
	bool msg_read;
	bool is_dvc;
	bool is_vi;
};

static void dvc_writel(struct tegra_i2c_dev *i2c_dev, u32 val,
		       unsigned int reg)
{
	writel_relaxed(val, i2c_dev->base + reg);
}

static u32 dvc_readl(struct tegra_i2c_dev *i2c_dev, unsigned int reg)
{
	return readl_relaxed(i2c_dev->base + reg);
}

/*
 * If necessary, i2c_writel() and i2c_readl() will offset the register
 * in order to talk to the I2C block inside the DVC block.
 */
static u32 tegra_i2c_reg_addr(struct tegra_i2c_dev *i2c_dev, unsigned int reg)
{
	if (i2c_dev->is_dvc)
		reg += (reg >= I2C_TX_FIFO) ? 0x10 : 0x40;
	else if (i2c_dev->is_vi)
		reg = 0xc00 + (reg << 2);

	return reg;
}

static void i2c_writel(struct tegra_i2c_dev *i2c_dev, u32 val, unsigned int reg)
{
	writel_relaxed(val, i2c_dev->base + tegra_i2c_reg_addr(i2c_dev, reg));

	/* read back register to make sure that register writes completed */
	if (reg != I2C_TX_FIFO)
		readl_relaxed(i2c_dev->base + tegra_i2c_reg_addr(i2c_dev, reg));
	else if (i2c_dev->is_vi)
		readl_relaxed(i2c_dev->base + tegra_i2c_reg_addr(i2c_dev, I2C_INT_STATUS));
}

static u32 i2c_readl(struct tegra_i2c_dev *i2c_dev, unsigned int reg)
{
	return readl_relaxed(i2c_dev->base + tegra_i2c_reg_addr(i2c_dev, reg));
}

static void i2c_writesl(struct tegra_i2c_dev *i2c_dev, void *data,
			unsigned int reg, unsigned int len)
>>>>>>> upstream/android-13
{
	writesl(i2c_dev->base + tegra_i2c_reg_addr(i2c_dev, reg), data, len);
}

<<<<<<< HEAD
static void i2c_readsl(struct tegra_i2c_dev *i2c_dev, void *data,
	unsigned long reg, int len)
=======
static void i2c_writesl_vi(struct tegra_i2c_dev *i2c_dev, void *data,
			   unsigned int reg, unsigned int len)
{
	u32 *data32 = data;

	/*
	 * VI I2C controller has known hardware bug where writes get stuck
	 * when immediate multiple writes happen to TX_FIFO register.
	 * Recommended software work around is to read I2C register after
	 * each write to TX_FIFO register to flush out the data.
	 */
	while (len--)
		i2c_writel(i2c_dev, *data32++, reg);
}

static void i2c_readsl(struct tegra_i2c_dev *i2c_dev, void *data,
		       unsigned int reg, unsigned int len)
>>>>>>> upstream/android-13
{
	readsl(i2c_dev->base + tegra_i2c_reg_addr(i2c_dev, reg), data, len);
}

static void tegra_i2c_mask_irq(struct tegra_i2c_dev *i2c_dev, u32 mask)
{
	u32 int_mask;

	int_mask = i2c_readl(i2c_dev, I2C_INT_MASK) & ~mask;
	i2c_writel(i2c_dev, int_mask, I2C_INT_MASK);
}

static void tegra_i2c_unmask_irq(struct tegra_i2c_dev *i2c_dev, u32 mask)
{
	u32 int_mask;

	int_mask = i2c_readl(i2c_dev, I2C_INT_MASK) | mask;
	i2c_writel(i2c_dev, int_mask, I2C_INT_MASK);
}

<<<<<<< HEAD
static int tegra_i2c_flush_fifos(struct tegra_i2c_dev *i2c_dev)
{
	unsigned long timeout = jiffies + HZ;
	unsigned int offset;
	u32 mask, val;

	if (i2c_dev->hw->has_mst_fifo) {
		mask = I2C_MST_FIFO_CONTROL_TX_FLUSH |
		       I2C_MST_FIFO_CONTROL_RX_FLUSH;
		offset = I2C_MST_FIFO_CONTROL;
	} else {
		mask = I2C_FIFO_CONTROL_TX_FLUSH |
		       I2C_FIFO_CONTROL_RX_FLUSH;
		offset = I2C_FIFO_CONTROL;
	}

	val = i2c_readl(i2c_dev, offset);
	val |= mask;
	i2c_writel(i2c_dev, val, offset);

	while (i2c_readl(i2c_dev, offset) & mask) {
		if (time_after(jiffies, timeout)) {
			dev_warn(i2c_dev->dev, "timeout waiting for fifo flush\n");
			return -ETIMEDOUT;
		}
		msleep(1);
	}
	return 0;
}

static int tegra_i2c_empty_rx_fifo(struct tegra_i2c_dev *i2c_dev)
{
	u32 val;
	int rx_fifo_avail;
	u8 *buf = i2c_dev->msg_buf;
	size_t buf_remaining = i2c_dev->msg_buf_remaining;
	int words_to_transfer;

	if (i2c_dev->hw->has_mst_fifo) {
		val = i2c_readl(i2c_dev, I2C_MST_FIFO_STATUS);
		rx_fifo_avail = (val & I2C_MST_FIFO_STATUS_RX_MASK) >>
			I2C_MST_FIFO_STATUS_RX_SHIFT;
	} else {
		val = i2c_readl(i2c_dev, I2C_FIFO_STATUS);
		rx_fifo_avail = (val & I2C_FIFO_STATUS_RX_MASK) >>
			I2C_FIFO_STATUS_RX_SHIFT;
	}

	/* Rounds down to not include partial word at the end of buf */
	words_to_transfer = buf_remaining / BYTES_PER_FIFO_WORD;
	if (words_to_transfer > rx_fifo_avail)
		words_to_transfer = rx_fifo_avail;

	i2c_readsl(i2c_dev, buf, I2C_RX_FIFO, words_to_transfer);

	buf += words_to_transfer * BYTES_PER_FIFO_WORD;
	buf_remaining -= words_to_transfer * BYTES_PER_FIFO_WORD;
	rx_fifo_avail -= words_to_transfer;

	/*
	 * If there is a partial word at the end of buf, handle it manually to
	 * prevent overwriting past the end of buf
	 */
	if (rx_fifo_avail > 0 && buf_remaining > 0) {
		BUG_ON(buf_remaining > 3);
		val = i2c_readl(i2c_dev, I2C_RX_FIFO);
		val = cpu_to_le32(val);
		memcpy(buf, &val, buf_remaining);
		buf_remaining = 0;
		rx_fifo_avail--;
	}

	BUG_ON(rx_fifo_avail > 0 && buf_remaining > 0);
	i2c_dev->msg_buf_remaining = buf_remaining;
	i2c_dev->msg_buf = buf;
=======
static void tegra_i2c_dma_complete(void *args)
{
	struct tegra_i2c_dev *i2c_dev = args;

	complete(&i2c_dev->dma_complete);
}

static int tegra_i2c_dma_submit(struct tegra_i2c_dev *i2c_dev, size_t len)
{
	struct dma_async_tx_descriptor *dma_desc;
	enum dma_transfer_direction dir;
	struct dma_chan *chan;

	dev_dbg(i2c_dev->dev, "starting DMA for length: %zu\n", len);

	reinit_completion(&i2c_dev->dma_complete);

	dir = i2c_dev->msg_read ? DMA_DEV_TO_MEM : DMA_MEM_TO_DEV;
	chan = i2c_dev->msg_read ? i2c_dev->rx_dma_chan : i2c_dev->tx_dma_chan;

	dma_desc = dmaengine_prep_slave_single(chan, i2c_dev->dma_phys,
					       len, dir, DMA_PREP_INTERRUPT |
					       DMA_CTRL_ACK);
	if (!dma_desc) {
		dev_err(i2c_dev->dev, "failed to get %s DMA descriptor\n",
			i2c_dev->msg_read ? "RX" : "TX");
		return -EINVAL;
	}

	dma_desc->callback = tegra_i2c_dma_complete;
	dma_desc->callback_param = i2c_dev;

	dmaengine_submit(dma_desc);
	dma_async_issue_pending(chan);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int tegra_i2c_fill_tx_fifo(struct tegra_i2c_dev *i2c_dev)
{
	u32 val;
	int tx_fifo_avail;
	u8 *buf = i2c_dev->msg_buf;
	size_t buf_remaining = i2c_dev->msg_buf_remaining;
	int words_to_transfer;

	if (i2c_dev->hw->has_mst_fifo) {
		val = i2c_readl(i2c_dev, I2C_MST_FIFO_STATUS);
		tx_fifo_avail = (val & I2C_MST_FIFO_STATUS_TX_MASK) >>
			I2C_MST_FIFO_STATUS_TX_SHIFT;
	} else {
		val = i2c_readl(i2c_dev, I2C_FIFO_STATUS);
		tx_fifo_avail = (val & I2C_FIFO_STATUS_TX_MASK) >>
			I2C_FIFO_STATUS_TX_SHIFT;
	}

	/* Rounds down to not include partial word at the end of buf */
	words_to_transfer = buf_remaining / BYTES_PER_FIFO_WORD;

	/* It's very common to have < 4 bytes, so optimize that case. */
	if (words_to_transfer) {
		if (words_to_transfer > tx_fifo_avail)
			words_to_transfer = tx_fifo_avail;

		/*
		 * Update state before writing to FIFO.  If this casues us
		 * to finish writing all bytes (AKA buf_remaining goes to 0) we
		 * have a potential for an interrupt (PACKET_XFER_COMPLETE is
		 * not maskable).  We need to make sure that the isr sees
		 * buf_remaining as 0 and doesn't call us back re-entrantly.
		 */
		buf_remaining -= words_to_transfer * BYTES_PER_FIFO_WORD;
		tx_fifo_avail -= words_to_transfer;
		i2c_dev->msg_buf_remaining = buf_remaining;
		i2c_dev->msg_buf = buf +
			words_to_transfer * BYTES_PER_FIFO_WORD;
		barrier();

		i2c_writesl(i2c_dev, buf, I2C_TX_FIFO, words_to_transfer);

		buf += words_to_transfer * BYTES_PER_FIFO_WORD;
	}

	/*
	 * If there is a partial word at the end of buf, handle it manually to
	 * prevent reading past the end of buf, which could cross a page
	 * boundary and fault.
	 */
	if (tx_fifo_avail > 0 && buf_remaining > 0) {
		BUG_ON(buf_remaining > 3);
		memcpy(&val, buf, buf_remaining);
		val = le32_to_cpu(val);

		/* Again update before writing to FIFO to make sure isr sees. */
		i2c_dev->msg_buf_remaining = 0;
		i2c_dev->msg_buf = NULL;
		barrier();

		i2c_writel(i2c_dev, val, I2C_TX_FIFO);
	}

	return 0;
=======
static void tegra_i2c_release_dma(struct tegra_i2c_dev *i2c_dev)
{
	if (i2c_dev->dma_buf) {
		dma_free_coherent(i2c_dev->dev, i2c_dev->dma_buf_size,
				  i2c_dev->dma_buf, i2c_dev->dma_phys);
		i2c_dev->dma_buf = NULL;
	}

	if (i2c_dev->tx_dma_chan) {
		dma_release_channel(i2c_dev->tx_dma_chan);
		i2c_dev->tx_dma_chan = NULL;
	}

	if (i2c_dev->rx_dma_chan) {
		dma_release_channel(i2c_dev->rx_dma_chan);
		i2c_dev->rx_dma_chan = NULL;
	}
}

static int tegra_i2c_init_dma(struct tegra_i2c_dev *i2c_dev)
{
	struct dma_chan *chan;
	dma_addr_t dma_phys;
	u32 *dma_buf;
	int err;

	if (!i2c_dev->hw->has_apb_dma || i2c_dev->is_vi)
		return 0;

	if (!IS_ENABLED(CONFIG_TEGRA20_APB_DMA)) {
		dev_dbg(i2c_dev->dev, "DMA support not enabled\n");
		return 0;
	}

	chan = dma_request_chan(i2c_dev->dev, "rx");
	if (IS_ERR(chan)) {
		err = PTR_ERR(chan);
		goto err_out;
	}

	i2c_dev->rx_dma_chan = chan;

	chan = dma_request_chan(i2c_dev->dev, "tx");
	if (IS_ERR(chan)) {
		err = PTR_ERR(chan);
		goto err_out;
	}

	i2c_dev->tx_dma_chan = chan;

	i2c_dev->dma_buf_size = i2c_dev->hw->quirks->max_write_len +
				I2C_PACKET_HEADER_SIZE;

	dma_buf = dma_alloc_coherent(i2c_dev->dev, i2c_dev->dma_buf_size,
				     &dma_phys, GFP_KERNEL | __GFP_NOWARN);
	if (!dma_buf) {
		dev_err(i2c_dev->dev, "failed to allocate DMA buffer\n");
		err = -ENOMEM;
		goto err_out;
	}

	i2c_dev->dma_buf = dma_buf;
	i2c_dev->dma_phys = dma_phys;

	return 0;

err_out:
	tegra_i2c_release_dma(i2c_dev);
	if (err != -EPROBE_DEFER) {
		dev_err(i2c_dev->dev, "cannot use DMA: %d\n", err);
		dev_err(i2c_dev->dev, "falling back to PIO\n");
		return 0;
	}

	return err;
>>>>>>> upstream/android-13
}

/*
 * One of the Tegra I2C blocks is inside the DVC (Digital Voltage Controller)
 * block.  This block is identical to the rest of the I2C blocks, except that
 * it only supports master mode, it has registers moved around, and it needs
 * some extra init to get it into I2C mode.  The register moves are handled
<<<<<<< HEAD
 * by i2c_readl and i2c_writel
=======
 * by i2c_readl() and i2c_writel().
>>>>>>> upstream/android-13
 */
static void tegra_dvc_init(struct tegra_i2c_dev *i2c_dev)
{
	u32 val;

	val = dvc_readl(i2c_dev, DVC_CTRL_REG3);
	val |= DVC_CTRL_REG3_SW_PROG;
	val |= DVC_CTRL_REG3_I2C_DONE_INTR_EN;
	dvc_writel(i2c_dev, val, DVC_CTRL_REG3);

	val = dvc_readl(i2c_dev, DVC_CTRL_REG1);
	val |= DVC_CTRL_REG1_INTR_EN;
	dvc_writel(i2c_dev, val, DVC_CTRL_REG1);
}

<<<<<<< HEAD
static int tegra_i2c_runtime_resume(struct device *dev)
{
	struct tegra_i2c_dev *i2c_dev = dev_get_drvdata(dev);
	int ret;

	ret = pinctrl_pm_select_default_state(i2c_dev->dev);
	if (ret)
		return ret;

	if (!i2c_dev->hw->has_single_clk_source) {
		ret = clk_enable(i2c_dev->fast_clk);
		if (ret < 0) {
			dev_err(i2c_dev->dev,
				"Enabling fast clk failed, err %d\n", ret);
			return ret;
		}
	}

	ret = clk_enable(i2c_dev->div_clk);
	if (ret < 0) {
		dev_err(i2c_dev->dev,
			"Enabling div clk failed, err %d\n", ret);
		clk_disable(i2c_dev->fast_clk);
		return ret;
=======
static void tegra_i2c_vi_init(struct tegra_i2c_dev *i2c_dev)
{
	u32 value;

	value = FIELD_PREP(I2C_INTERFACE_TIMING_THIGH, 2) |
		FIELD_PREP(I2C_INTERFACE_TIMING_TLOW, 4);
	i2c_writel(i2c_dev, value, I2C_INTERFACE_TIMING_0);

	value = FIELD_PREP(I2C_INTERFACE_TIMING_TBUF, 4) |
		FIELD_PREP(I2C_INTERFACE_TIMING_TSU_STO, 7) |
		FIELD_PREP(I2C_INTERFACE_TIMING_THD_STA, 4) |
		FIELD_PREP(I2C_INTERFACE_TIMING_TSU_STA, 4);
	i2c_writel(i2c_dev, value, I2C_INTERFACE_TIMING_1);

	value = FIELD_PREP(I2C_HS_INTERFACE_TIMING_THIGH, 3) |
		FIELD_PREP(I2C_HS_INTERFACE_TIMING_TLOW, 8);
	i2c_writel(i2c_dev, value, I2C_HS_INTERFACE_TIMING_0);

	value = FIELD_PREP(I2C_HS_INTERFACE_TIMING_TSU_STO, 11) |
		FIELD_PREP(I2C_HS_INTERFACE_TIMING_THD_STA, 11) |
		FIELD_PREP(I2C_HS_INTERFACE_TIMING_TSU_STA, 11);
	i2c_writel(i2c_dev, value, I2C_HS_INTERFACE_TIMING_1);

	value = FIELD_PREP(I2C_BC_SCLK_THRESHOLD, 9) | I2C_BC_STOP_COND;
	i2c_writel(i2c_dev, value, I2C_BUS_CLEAR_CNFG);

	i2c_writel(i2c_dev, 0x0, I2C_TLOW_SEXT);
}

static int tegra_i2c_poll_register(struct tegra_i2c_dev *i2c_dev,
				   u32 reg, u32 mask, u32 delay_us,
				   u32 timeout_us)
{
	void __iomem *addr = i2c_dev->base + tegra_i2c_reg_addr(i2c_dev, reg);
	u32 val;

	if (!i2c_dev->atomic_mode)
		return readl_relaxed_poll_timeout(addr, val, !(val & mask),
						  delay_us, timeout_us);

	return readl_relaxed_poll_timeout_atomic(addr, val, !(val & mask),
						 delay_us, timeout_us);
}

static int tegra_i2c_flush_fifos(struct tegra_i2c_dev *i2c_dev)
{
	u32 mask, val, offset;
	int err;

	if (i2c_dev->hw->has_mst_fifo) {
		mask = I2C_MST_FIFO_CONTROL_TX_FLUSH |
		       I2C_MST_FIFO_CONTROL_RX_FLUSH;
		offset = I2C_MST_FIFO_CONTROL;
	} else {
		mask = I2C_FIFO_CONTROL_TX_FLUSH |
		       I2C_FIFO_CONTROL_RX_FLUSH;
		offset = I2C_FIFO_CONTROL;
	}

	val = i2c_readl(i2c_dev, offset);
	val |= mask;
	i2c_writel(i2c_dev, val, offset);

	err = tegra_i2c_poll_register(i2c_dev, offset, mask, 1000, 1000000);
	if (err) {
		dev_err(i2c_dev->dev, "failed to flush FIFO\n");
		return err;
>>>>>>> upstream/android-13
	}

	return 0;
}

<<<<<<< HEAD
static int tegra_i2c_runtime_suspend(struct device *dev)
{
	struct tegra_i2c_dev *i2c_dev = dev_get_drvdata(dev);

	clk_disable(i2c_dev->div_clk);
	if (!i2c_dev->hw->has_single_clk_source)
		clk_disable(i2c_dev->fast_clk);

	return pinctrl_pm_select_idle_state(i2c_dev->dev);
}

static int tegra_i2c_wait_for_config_load(struct tegra_i2c_dev *i2c_dev)
{
	unsigned long reg_offset;
	void __iomem *addr;
	u32 val;
	int err;

	if (i2c_dev->hw->has_config_load_reg) {
		reg_offset = tegra_i2c_reg_addr(i2c_dev, I2C_CONFIG_LOAD);
		addr = i2c_dev->base + reg_offset;
		i2c_writel(i2c_dev, I2C_MSTR_CONFIG_LOAD, I2C_CONFIG_LOAD);
		if (in_interrupt())
			err = readl_poll_timeout_atomic(addr, val, val == 0,
					1000, I2C_CONFIG_LOAD_TIMEOUT);
		else
			err = readl_poll_timeout(addr, val, val == 0,
					1000, I2C_CONFIG_LOAD_TIMEOUT);

		if (err) {
			dev_warn(i2c_dev->dev,
				 "timeout waiting for config load\n");
			return err;
		}
=======
static int tegra_i2c_wait_for_config_load(struct tegra_i2c_dev *i2c_dev)
{
	int err;

	if (!i2c_dev->hw->has_config_load_reg)
		return 0;

	i2c_writel(i2c_dev, I2C_MSTR_CONFIG_LOAD, I2C_CONFIG_LOAD);

	err = tegra_i2c_poll_register(i2c_dev, I2C_CONFIG_LOAD, 0xffffffff,
				      1000, I2C_CONFIG_LOAD_TIMEOUT);
	if (err) {
		dev_err(i2c_dev->dev, "failed to load config\n");
		return err;
>>>>>>> upstream/android-13
	}

	return 0;
}

static int tegra_i2c_init(struct tegra_i2c_dev *i2c_dev)
{
<<<<<<< HEAD
	u32 val;
	int err;
	u32 clk_divisor;

	err = pm_runtime_get_sync(i2c_dev->dev);
	if (err < 0) {
		dev_err(i2c_dev->dev, "runtime resume failed %d\n", err);
		return err;
	}

	reset_control_assert(i2c_dev->rst);
	udelay(2);
	reset_control_deassert(i2c_dev->rst);
=======
	u32 val, clk_divisor, clk_multiplier, tsu_thd, tlow, thigh, non_hs_mode;
	int err;

	/*
	 * The reset shouldn't ever fail in practice. The failure will be a
	 * sign of a severe problem that needs to be resolved. Still we don't
	 * want to fail the initialization completely because this may break
	 * kernel boot up since voltage regulators use I2C. Hence, we will
	 * emit a noisy warning on error, which won't stay unnoticed and
	 * won't hose machine entirely.
	 */
	err = reset_control_reset(i2c_dev->rst);
	WARN_ON_ONCE(err);
>>>>>>> upstream/android-13

	if (i2c_dev->is_dvc)
		tegra_dvc_init(i2c_dev);

	val = I2C_CNFG_NEW_MASTER_FSM | I2C_CNFG_PACKET_MODE_EN |
<<<<<<< HEAD
		(0x2 << I2C_CNFG_DEBOUNCE_CNT_SHIFT);
=======
	      FIELD_PREP(I2C_CNFG_DEBOUNCE_CNT, 2);
>>>>>>> upstream/android-13

	if (i2c_dev->hw->has_multi_master_mode)
		val |= I2C_CNFG_MULTI_MASTER_MODE;

	i2c_writel(i2c_dev, val, I2C_CNFG);
	i2c_writel(i2c_dev, 0, I2C_INT_MASK);

<<<<<<< HEAD
	/* Make sure clock divisor programmed correctly */
	clk_divisor = i2c_dev->hw->clk_divisor_hs_mode;
	clk_divisor |= i2c_dev->clk_divisor_non_hs_mode <<
					I2C_CLK_DIVISOR_STD_FAST_MODE_SHIFT;
	i2c_writel(i2c_dev, clk_divisor, I2C_CLK_DIVISOR);

	if (!i2c_dev->is_dvc) {
=======
	if (i2c_dev->is_vi)
		tegra_i2c_vi_init(i2c_dev);

	switch (i2c_dev->bus_clk_rate) {
	case I2C_MAX_STANDARD_MODE_FREQ + 1 ... I2C_MAX_FAST_MODE_PLUS_FREQ:
	default:
		tlow = i2c_dev->hw->tlow_fast_fastplus_mode;
		thigh = i2c_dev->hw->thigh_fast_fastplus_mode;
		tsu_thd = i2c_dev->hw->setup_hold_time_fast_fast_plus_mode;

		if (i2c_dev->bus_clk_rate > I2C_MAX_FAST_MODE_FREQ)
			non_hs_mode = i2c_dev->hw->clk_divisor_fast_plus_mode;
		else
			non_hs_mode = i2c_dev->hw->clk_divisor_fast_mode;
		break;

	case 0 ... I2C_MAX_STANDARD_MODE_FREQ:
		tlow = i2c_dev->hw->tlow_std_mode;
		thigh = i2c_dev->hw->thigh_std_mode;
		tsu_thd = i2c_dev->hw->setup_hold_time_std_mode;
		non_hs_mode = i2c_dev->hw->clk_divisor_std_mode;
		break;
	}

	/* make sure clock divisor programmed correctly */
	clk_divisor = FIELD_PREP(I2C_CLK_DIVISOR_HSMODE,
				 i2c_dev->hw->clk_divisor_hs_mode) |
		      FIELD_PREP(I2C_CLK_DIVISOR_STD_FAST_MODE, non_hs_mode);
	i2c_writel(i2c_dev, clk_divisor, I2C_CLK_DIVISOR);

	if (i2c_dev->hw->has_interface_timing_reg) {
		val = FIELD_PREP(I2C_INTERFACE_TIMING_THIGH, thigh) |
		      FIELD_PREP(I2C_INTERFACE_TIMING_TLOW, tlow);
		i2c_writel(i2c_dev, val, I2C_INTERFACE_TIMING_0);
	}

	/*
	 * Configure setup and hold times only when tsu_thd is non-zero.
	 * Otherwise, preserve the chip default values.
	 */
	if (i2c_dev->hw->has_interface_timing_reg && tsu_thd)
		i2c_writel(i2c_dev, tsu_thd, I2C_INTERFACE_TIMING_1);

	clk_multiplier = (tlow + thigh + 2) * (non_hs_mode + 1);

	err = clk_set_rate(i2c_dev->div_clk,
			   i2c_dev->bus_clk_rate * clk_multiplier);
	if (err) {
		dev_err(i2c_dev->dev, "failed to set div-clk rate: %d\n", err);
		return err;
	}

	if (!i2c_dev->is_dvc && !i2c_dev->is_vi) {
>>>>>>> upstream/android-13
		u32 sl_cfg = i2c_readl(i2c_dev, I2C_SL_CNFG);

		sl_cfg |= I2C_SL_CNFG_NACK | I2C_SL_CNFG_NEWSL;
		i2c_writel(i2c_dev, sl_cfg, I2C_SL_CNFG);
		i2c_writel(i2c_dev, 0xfc, I2C_SL_ADDR1);
		i2c_writel(i2c_dev, 0x00, I2C_SL_ADDR2);
	}

<<<<<<< HEAD
	if (i2c_dev->hw->has_mst_fifo) {
		val = I2C_MST_FIFO_CONTROL_TX_TRIG(8) |
		      I2C_MST_FIFO_CONTROL_RX_TRIG(1);
		i2c_writel(i2c_dev, val, I2C_MST_FIFO_CONTROL);
	} else {
		val = 7 << I2C_FIFO_CONTROL_TX_TRIG_SHIFT |
			0 << I2C_FIFO_CONTROL_RX_TRIG_SHIFT;
		i2c_writel(i2c_dev, val, I2C_FIFO_CONTROL);
	}

	err = tegra_i2c_flush_fifos(i2c_dev);
	if (err)
		goto err;

	if (i2c_dev->is_multimaster_mode && i2c_dev->hw->has_slcg_override_reg)
=======
	err = tegra_i2c_flush_fifos(i2c_dev);
	if (err)
		return err;

	if (i2c_dev->multimaster_mode && i2c_dev->hw->has_slcg_override_reg)
>>>>>>> upstream/android-13
		i2c_writel(i2c_dev, I2C_MST_CORE_CLKEN_OVR, I2C_CLKEN_OVERRIDE);

	err = tegra_i2c_wait_for_config_load(i2c_dev);
	if (err)
<<<<<<< HEAD
		goto err;

	if (i2c_dev->irq_disabled) {
		i2c_dev->irq_disabled = false;
		enable_irq(i2c_dev->irq);
	}

err:
	pm_runtime_put(i2c_dev->dev);
	return err;
=======
		return err;

	return 0;
>>>>>>> upstream/android-13
}

static int tegra_i2c_disable_packet_mode(struct tegra_i2c_dev *i2c_dev)
{
	u32 cnfg;

	/*
	 * NACK interrupt is generated before the I2C controller generates
<<<<<<< HEAD
	 * the STOP condition on the bus. So wait for 2 clock periods
=======
	 * the STOP condition on the bus.  So, wait for 2 clock periods
>>>>>>> upstream/android-13
	 * before disabling the controller so that the STOP condition has
	 * been delivered properly.
	 */
	udelay(DIV_ROUND_UP(2 * 1000000, i2c_dev->bus_clk_rate));

	cnfg = i2c_readl(i2c_dev, I2C_CNFG);
	if (cnfg & I2C_CNFG_PACKET_MODE_EN)
		i2c_writel(i2c_dev, cnfg & ~I2C_CNFG_PACKET_MODE_EN, I2C_CNFG);

	return tegra_i2c_wait_for_config_load(i2c_dev);
}

<<<<<<< HEAD
static irqreturn_t tegra_i2c_isr(int irq, void *dev_id)
{
	u32 status;
	const u32 status_err = I2C_INT_NO_ACK | I2C_INT_ARBITRATION_LOST;
	struct tegra_i2c_dev *i2c_dev = dev_id;
	unsigned long flags;

	status = i2c_readl(i2c_dev, I2C_INT_STATUS);

	spin_lock_irqsave(&i2c_dev->xfer_lock, flags);
	if (status == 0) {
		dev_warn(i2c_dev->dev, "irq status 0 %08x %08x %08x\n",
=======
static int tegra_i2c_empty_rx_fifo(struct tegra_i2c_dev *i2c_dev)
{
	size_t buf_remaining = i2c_dev->msg_buf_remaining;
	unsigned int words_to_transfer, rx_fifo_avail;
	u8 *buf = i2c_dev->msg_buf;
	u32 val;

	/*
	 * Catch overflow due to message fully sent before the check for
	 * RX FIFO availability.
	 */
	if (WARN_ON_ONCE(!(i2c_dev->msg_buf_remaining)))
		return -EINVAL;

	if (i2c_dev->hw->has_mst_fifo) {
		val = i2c_readl(i2c_dev, I2C_MST_FIFO_STATUS);
		rx_fifo_avail = FIELD_GET(I2C_MST_FIFO_STATUS_RX, val);
	} else {
		val = i2c_readl(i2c_dev, I2C_FIFO_STATUS);
		rx_fifo_avail = FIELD_GET(I2C_FIFO_STATUS_RX, val);
	}

	/* round down to exclude partial word at the end of buffer */
	words_to_transfer = buf_remaining / BYTES_PER_FIFO_WORD;
	if (words_to_transfer > rx_fifo_avail)
		words_to_transfer = rx_fifo_avail;

	i2c_readsl(i2c_dev, buf, I2C_RX_FIFO, words_to_transfer);

	buf += words_to_transfer * BYTES_PER_FIFO_WORD;
	buf_remaining -= words_to_transfer * BYTES_PER_FIFO_WORD;
	rx_fifo_avail -= words_to_transfer;

	/*
	 * If there is a partial word at the end of buffer, handle it
	 * manually to prevent overwriting past the end of buffer.
	 */
	if (rx_fifo_avail > 0 && buf_remaining > 0) {
		/*
		 * buf_remaining > 3 check not needed as rx_fifo_avail == 0
		 * when (words_to_transfer was > rx_fifo_avail) earlier
		 * in this function.
		 */
		val = i2c_readl(i2c_dev, I2C_RX_FIFO);
		val = cpu_to_le32(val);
		memcpy(buf, &val, buf_remaining);
		buf_remaining = 0;
		rx_fifo_avail--;
	}

	/* RX FIFO must be drained, otherwise it's an Overflow case. */
	if (WARN_ON_ONCE(rx_fifo_avail))
		return -EINVAL;

	i2c_dev->msg_buf_remaining = buf_remaining;
	i2c_dev->msg_buf = buf;

	return 0;
}

static int tegra_i2c_fill_tx_fifo(struct tegra_i2c_dev *i2c_dev)
{
	size_t buf_remaining = i2c_dev->msg_buf_remaining;
	unsigned int words_to_transfer, tx_fifo_avail;
	u8 *buf = i2c_dev->msg_buf;
	u32 val;

	if (i2c_dev->hw->has_mst_fifo) {
		val = i2c_readl(i2c_dev, I2C_MST_FIFO_STATUS);
		tx_fifo_avail = FIELD_GET(I2C_MST_FIFO_STATUS_TX, val);
	} else {
		val = i2c_readl(i2c_dev, I2C_FIFO_STATUS);
		tx_fifo_avail = FIELD_GET(I2C_FIFO_STATUS_TX, val);
	}

	/* round down to exclude partial word at the end of buffer */
	words_to_transfer = buf_remaining / BYTES_PER_FIFO_WORD;

	/*
	 * This hunk pushes 4 bytes at a time into the TX FIFO.
	 *
	 * It's very common to have < 4 bytes, hence there is no word
	 * to push if we have less than 4 bytes to transfer.
	 */
	if (words_to_transfer) {
		if (words_to_transfer > tx_fifo_avail)
			words_to_transfer = tx_fifo_avail;

		/*
		 * Update state before writing to FIFO.  Note that this may
		 * cause us to finish writing all bytes (AKA buf_remaining
		 * goes to 0), hence we have a potential for an interrupt
		 * (PACKET_XFER_COMPLETE is not maskable), but GIC interrupt
		 * is disabled at this point.
		 */
		buf_remaining -= words_to_transfer * BYTES_PER_FIFO_WORD;
		tx_fifo_avail -= words_to_transfer;

		i2c_dev->msg_buf_remaining = buf_remaining;
		i2c_dev->msg_buf = buf + words_to_transfer * BYTES_PER_FIFO_WORD;

		if (i2c_dev->is_vi)
			i2c_writesl_vi(i2c_dev, buf, I2C_TX_FIFO, words_to_transfer);
		else
			i2c_writesl(i2c_dev, buf, I2C_TX_FIFO, words_to_transfer);

		buf += words_to_transfer * BYTES_PER_FIFO_WORD;
	}

	/*
	 * If there is a partial word at the end of buffer, handle it manually
	 * to prevent reading past the end of buffer, which could cross a page
	 * boundary and fault.
	 */
	if (tx_fifo_avail > 0 && buf_remaining > 0) {
		/*
		 * buf_remaining > 3 check not needed as tx_fifo_avail == 0
		 * when (words_to_transfer was > tx_fifo_avail) earlier
		 * in this function for non-zero words_to_transfer.
		 */
		memcpy(&val, buf, buf_remaining);
		val = le32_to_cpu(val);

		i2c_dev->msg_buf_remaining = 0;
		i2c_dev->msg_buf = NULL;

		i2c_writel(i2c_dev, val, I2C_TX_FIFO);
	}

	return 0;
}

static irqreturn_t tegra_i2c_isr(int irq, void *dev_id)
{
	const u32 status_err = I2C_INT_NO_ACK | I2C_INT_ARBITRATION_LOST;
	struct tegra_i2c_dev *i2c_dev = dev_id;
	u32 status;

	status = i2c_readl(i2c_dev, I2C_INT_STATUS);

	if (status == 0) {
		dev_warn(i2c_dev->dev, "IRQ status 0 %08x %08x %08x\n",
>>>>>>> upstream/android-13
			 i2c_readl(i2c_dev, I2C_PACKET_TRANSFER_STATUS),
			 i2c_readl(i2c_dev, I2C_STATUS),
			 i2c_readl(i2c_dev, I2C_CNFG));
		i2c_dev->msg_err |= I2C_ERR_UNKNOWN_INTERRUPT;
<<<<<<< HEAD

		if (!i2c_dev->irq_disabled) {
			disable_irq_nosync(i2c_dev->irq);
			i2c_dev->irq_disabled = true;
		}
		goto err;
	}

	if (unlikely(status & status_err)) {
=======
		goto err;
	}

	if (status & status_err) {
>>>>>>> upstream/android-13
		tegra_i2c_disable_packet_mode(i2c_dev);
		if (status & I2C_INT_NO_ACK)
			i2c_dev->msg_err |= I2C_ERR_NO_ACK;
		if (status & I2C_INT_ARBITRATION_LOST)
			i2c_dev->msg_err |= I2C_ERR_ARBITRATION_LOST;
		goto err;
	}

<<<<<<< HEAD
	if (i2c_dev->msg_read && (status & I2C_INT_RX_FIFO_DATA_REQ)) {
		if (i2c_dev->msg_buf_remaining)
			tegra_i2c_empty_rx_fifo(i2c_dev);
		else
			BUG();
	}

	if (!i2c_dev->msg_read && (status & I2C_INT_TX_FIFO_DATA_REQ)) {
		if (i2c_dev->msg_buf_remaining)
			tegra_i2c_fill_tx_fifo(i2c_dev);
		else
			tegra_i2c_mask_irq(i2c_dev, I2C_INT_TX_FIFO_DATA_REQ);
=======
	/*
	 * I2C transfer is terminated during the bus clear, so skip
	 * processing the other interrupts.
	 */
	if (i2c_dev->hw->supports_bus_clear && (status & I2C_INT_BUS_CLR_DONE))
		goto err;

	if (!i2c_dev->dma_mode) {
		if (i2c_dev->msg_read && (status & I2C_INT_RX_FIFO_DATA_REQ)) {
			if (tegra_i2c_empty_rx_fifo(i2c_dev)) {
				/*
				 * Overflow error condition: message fully sent,
				 * with no XFER_COMPLETE interrupt but hardware
				 * asks to transfer more.
				 */
				i2c_dev->msg_err |= I2C_ERR_RX_BUFFER_OVERFLOW;
				goto err;
			}
		}

		if (!i2c_dev->msg_read && (status & I2C_INT_TX_FIFO_DATA_REQ)) {
			if (i2c_dev->msg_buf_remaining)
				tegra_i2c_fill_tx_fifo(i2c_dev);
			else
				tegra_i2c_mask_irq(i2c_dev,
						   I2C_INT_TX_FIFO_DATA_REQ);
		}
>>>>>>> upstream/android-13
	}

	i2c_writel(i2c_dev, status, I2C_INT_STATUS);
	if (i2c_dev->is_dvc)
		dvc_writel(i2c_dev, DVC_STATUS_I2C_DONE_INTR, DVC_STATUS);

<<<<<<< HEAD
	if (status & I2C_INT_PACKET_XFER_COMPLETE) {
		BUG_ON(i2c_dev->msg_buf_remaining);
=======
	/*
	 * During message read XFER_COMPLETE interrupt is triggered prior to
	 * DMA completion and during message write XFER_COMPLETE interrupt is
	 * triggered after DMA completion.
	 *
	 * PACKETS_XFER_COMPLETE indicates completion of all bytes of transfer,
	 * so forcing msg_buf_remaining to 0 in DMA mode.
	 */
	if (status & I2C_INT_PACKET_XFER_COMPLETE) {
		if (i2c_dev->dma_mode)
			i2c_dev->msg_buf_remaining = 0;
		/*
		 * Underflow error condition: XFER_COMPLETE before message
		 * fully sent.
		 */
		if (WARN_ON_ONCE(i2c_dev->msg_buf_remaining)) {
			i2c_dev->msg_err |= I2C_ERR_UNKNOWN_INTERRUPT;
			goto err;
		}
>>>>>>> upstream/android-13
		complete(&i2c_dev->msg_complete);
	}
	goto done;
err:
<<<<<<< HEAD
	/* An error occurred, mask all interrupts */
	tegra_i2c_mask_irq(i2c_dev, I2C_INT_NO_ACK | I2C_INT_ARBITRATION_LOST |
		I2C_INT_PACKET_XFER_COMPLETE | I2C_INT_TX_FIFO_DATA_REQ |
		I2C_INT_RX_FIFO_DATA_REQ);
	i2c_writel(i2c_dev, status, I2C_INT_STATUS);
	if (i2c_dev->is_dvc)
		dvc_writel(i2c_dev, DVC_STATUS_I2C_DONE_INTR, DVC_STATUS);

	complete(&i2c_dev->msg_complete);
done:
	spin_unlock_irqrestore(&i2c_dev->xfer_lock, flags);
	return IRQ_HANDLED;
}

static int tegra_i2c_xfer_msg(struct tegra_i2c_dev *i2c_dev,
	struct i2c_msg *msg, enum msg_end_type end_state)
{
	u32 packet_header;
	u32 int_mask;
	unsigned long time_left;
	unsigned long flags;

	tegra_i2c_flush_fifos(i2c_dev);

	i2c_dev->msg_buf = msg->buf;
	i2c_dev->msg_buf_remaining = msg->len;
	i2c_dev->msg_err = I2C_ERR_NONE;
	i2c_dev->msg_read = (msg->flags & I2C_M_RD);
	reinit_completion(&i2c_dev->msg_complete);

	spin_lock_irqsave(&i2c_dev->xfer_lock, flags);

	int_mask = I2C_INT_NO_ACK | I2C_INT_ARBITRATION_LOST;
	tegra_i2c_unmask_irq(i2c_dev, int_mask);

	packet_header = (0 << PACKET_HEADER0_HEADER_SIZE_SHIFT) |
			PACKET_HEADER0_PROTOCOL_I2C |
			(i2c_dev->cont_id << PACKET_HEADER0_CONT_ID_SHIFT) |
			(1 << PACKET_HEADER0_PACKET_ID_SHIFT);
	i2c_writel(i2c_dev, packet_header, I2C_TX_FIFO);

	packet_header = msg->len - 1;
	i2c_writel(i2c_dev, packet_header, I2C_TX_FIFO);

	packet_header = I2C_HEADER_IE_ENABLE;
=======
	/* mask all interrupts on error */
	tegra_i2c_mask_irq(i2c_dev,
			   I2C_INT_NO_ACK |
			   I2C_INT_ARBITRATION_LOST |
			   I2C_INT_PACKET_XFER_COMPLETE |
			   I2C_INT_TX_FIFO_DATA_REQ |
			   I2C_INT_RX_FIFO_DATA_REQ);

	if (i2c_dev->hw->supports_bus_clear)
		tegra_i2c_mask_irq(i2c_dev, I2C_INT_BUS_CLR_DONE);

	i2c_writel(i2c_dev, status, I2C_INT_STATUS);

	if (i2c_dev->is_dvc)
		dvc_writel(i2c_dev, DVC_STATUS_I2C_DONE_INTR, DVC_STATUS);

	if (i2c_dev->dma_mode) {
		if (i2c_dev->msg_read)
			dmaengine_terminate_async(i2c_dev->rx_dma_chan);
		else
			dmaengine_terminate_async(i2c_dev->tx_dma_chan);

		complete(&i2c_dev->dma_complete);
	}

	complete(&i2c_dev->msg_complete);
done:
	return IRQ_HANDLED;
}

static void tegra_i2c_config_fifo_trig(struct tegra_i2c_dev *i2c_dev,
				       size_t len)
{
	struct dma_slave_config slv_config = {0};
	u32 val, reg, dma_burst, reg_offset;
	struct dma_chan *chan;
	int err;

	if (i2c_dev->hw->has_mst_fifo)
		reg = I2C_MST_FIFO_CONTROL;
	else
		reg = I2C_FIFO_CONTROL;

	if (i2c_dev->dma_mode) {
		if (len & 0xF)
			dma_burst = 1;
		else if (len & 0x10)
			dma_burst = 4;
		else
			dma_burst = 8;

		if (i2c_dev->msg_read) {
			chan = i2c_dev->rx_dma_chan;
			reg_offset = tegra_i2c_reg_addr(i2c_dev, I2C_RX_FIFO);

			slv_config.src_addr = i2c_dev->base_phys + reg_offset;
			slv_config.src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
			slv_config.src_maxburst = dma_burst;

			if (i2c_dev->hw->has_mst_fifo)
				val = I2C_MST_FIFO_CONTROL_RX_TRIG(dma_burst);
			else
				val = I2C_FIFO_CONTROL_RX_TRIG(dma_burst);
		} else {
			chan = i2c_dev->tx_dma_chan;
			reg_offset = tegra_i2c_reg_addr(i2c_dev, I2C_TX_FIFO);

			slv_config.dst_addr = i2c_dev->base_phys + reg_offset;
			slv_config.dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
			slv_config.dst_maxburst = dma_burst;

			if (i2c_dev->hw->has_mst_fifo)
				val = I2C_MST_FIFO_CONTROL_TX_TRIG(dma_burst);
			else
				val = I2C_FIFO_CONTROL_TX_TRIG(dma_burst);
		}

		slv_config.device_fc = true;
		err = dmaengine_slave_config(chan, &slv_config);
		if (err) {
			dev_err(i2c_dev->dev, "DMA config failed: %d\n", err);
			dev_err(i2c_dev->dev, "falling back to PIO\n");

			tegra_i2c_release_dma(i2c_dev);
			i2c_dev->dma_mode = false;
		} else {
			goto out;
		}
	}

	if (i2c_dev->hw->has_mst_fifo)
		val = I2C_MST_FIFO_CONTROL_TX_TRIG(8) |
		      I2C_MST_FIFO_CONTROL_RX_TRIG(1);
	else
		val = I2C_FIFO_CONTROL_TX_TRIG(8) |
		      I2C_FIFO_CONTROL_RX_TRIG(1);
out:
	i2c_writel(i2c_dev, val, reg);
}

static unsigned long tegra_i2c_poll_completion(struct tegra_i2c_dev *i2c_dev,
					       struct completion *complete,
					       unsigned int timeout_ms)
{
	ktime_t ktime = ktime_get();
	ktime_t ktimeout = ktime_add_ms(ktime, timeout_ms);

	do {
		u32 status = i2c_readl(i2c_dev, I2C_INT_STATUS);

		if (status)
			tegra_i2c_isr(i2c_dev->irq, i2c_dev);

		if (completion_done(complete)) {
			s64 delta = ktime_ms_delta(ktimeout, ktime);

			return msecs_to_jiffies(delta) ?: 1;
		}

		ktime = ktime_get();

	} while (ktime_before(ktime, ktimeout));

	return 0;
}

static unsigned long tegra_i2c_wait_completion(struct tegra_i2c_dev *i2c_dev,
					       struct completion *complete,
					       unsigned int timeout_ms)
{
	unsigned long ret;

	if (i2c_dev->atomic_mode) {
		ret = tegra_i2c_poll_completion(i2c_dev, complete, timeout_ms);
	} else {
		enable_irq(i2c_dev->irq);
		ret = wait_for_completion_timeout(complete,
						  msecs_to_jiffies(timeout_ms));
		disable_irq(i2c_dev->irq);

		/*
		 * Under some rare circumstances (like running KASAN +
		 * NFS root) CPU, which handles interrupt, may stuck in
		 * uninterruptible state for a significant time.  In this
		 * case we will get timeout if I2C transfer is running on
		 * a sibling CPU, despite of IRQ being raised.
		 *
		 * In order to handle this rare condition, the IRQ status
		 * needs to be checked after timeout.
		 */
		if (ret == 0)
			ret = tegra_i2c_poll_completion(i2c_dev, complete, 0);
	}

	return ret;
}

static int tegra_i2c_issue_bus_clear(struct i2c_adapter *adap)
{
	struct tegra_i2c_dev *i2c_dev = i2c_get_adapdata(adap);
	u32 val, time_left;
	int err;

	reinit_completion(&i2c_dev->msg_complete);

	val = FIELD_PREP(I2C_BC_SCLK_THRESHOLD, 9) | I2C_BC_STOP_COND |
	      I2C_BC_TERMINATE;
	i2c_writel(i2c_dev, val, I2C_BUS_CLEAR_CNFG);

	err = tegra_i2c_wait_for_config_load(i2c_dev);
	if (err)
		return err;

	val |= I2C_BC_ENABLE;
	i2c_writel(i2c_dev, val, I2C_BUS_CLEAR_CNFG);
	tegra_i2c_unmask_irq(i2c_dev, I2C_INT_BUS_CLR_DONE);

	time_left = tegra_i2c_wait_completion(i2c_dev, &i2c_dev->msg_complete, 50);
	tegra_i2c_mask_irq(i2c_dev, I2C_INT_BUS_CLR_DONE);

	if (time_left == 0) {
		dev_err(i2c_dev->dev, "failed to clear bus\n");
		return -ETIMEDOUT;
	}

	val = i2c_readl(i2c_dev, I2C_BUS_CLEAR_STATUS);
	if (!(val & I2C_BC_STATUS)) {
		dev_err(i2c_dev->dev, "un-recovered arbitration lost\n");
		return -EIO;
	}

	return -EAGAIN;
}

static void tegra_i2c_push_packet_header(struct tegra_i2c_dev *i2c_dev,
					 struct i2c_msg *msg,
					 enum msg_end_type end_state)
{
	u32 *dma_buf = i2c_dev->dma_buf;
	u32 packet_header;

	packet_header = FIELD_PREP(PACKET_HEADER0_HEADER_SIZE, 0) |
			FIELD_PREP(PACKET_HEADER0_PROTOCOL,
				   PACKET_HEADER0_PROTOCOL_I2C) |
			FIELD_PREP(PACKET_HEADER0_CONT_ID, i2c_dev->cont_id) |
			FIELD_PREP(PACKET_HEADER0_PACKET_ID, 1);

	if (i2c_dev->dma_mode && !i2c_dev->msg_read)
		*dma_buf++ = packet_header;
	else
		i2c_writel(i2c_dev, packet_header, I2C_TX_FIFO);

	packet_header = msg->len - 1;

	if (i2c_dev->dma_mode && !i2c_dev->msg_read)
		*dma_buf++ = packet_header;
	else
		i2c_writel(i2c_dev, packet_header, I2C_TX_FIFO);

	packet_header = I2C_HEADER_IE_ENABLE;

>>>>>>> upstream/android-13
	if (end_state == MSG_END_CONTINUE)
		packet_header |= I2C_HEADER_CONTINUE_XFER;
	else if (end_state == MSG_END_REPEAT_START)
		packet_header |= I2C_HEADER_REPEAT_START;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	if (msg->flags & I2C_M_TEN) {
		packet_header |= msg->addr;
		packet_header |= I2C_HEADER_10BIT_ADDR;
	} else {
		packet_header |= msg->addr << I2C_HEADER_SLAVE_ADDR_SHIFT;
	}
<<<<<<< HEAD
	if (msg->flags & I2C_M_IGNORE_NAK)
		packet_header |= I2C_HEADER_CONT_ON_NAK;
	if (msg->flags & I2C_M_RD)
		packet_header |= I2C_HEADER_READ;
	i2c_writel(i2c_dev, packet_header, I2C_TX_FIFO);

	if (!(msg->flags & I2C_M_RD))
		tegra_i2c_fill_tx_fifo(i2c_dev);

	if (i2c_dev->hw->has_per_pkt_xfer_complete_irq)
		int_mask |= I2C_INT_PACKET_XFER_COMPLETE;
	if (msg->flags & I2C_M_RD)
		int_mask |= I2C_INT_RX_FIFO_DATA_REQ;
	else if (i2c_dev->msg_buf_remaining)
		int_mask |= I2C_INT_TX_FIFO_DATA_REQ;

	tegra_i2c_unmask_irq(i2c_dev, int_mask);
	spin_unlock_irqrestore(&i2c_dev->xfer_lock, flags);
	dev_dbg(i2c_dev->dev, "unmasked irq: %02x\n",
		i2c_readl(i2c_dev, I2C_INT_MASK));

	time_left = wait_for_completion_timeout(&i2c_dev->msg_complete,
						TEGRA_I2C_TIMEOUT);
	tegra_i2c_mask_irq(i2c_dev, int_mask);

	if (time_left == 0) {
		dev_err(i2c_dev->dev, "i2c transfer timed out\n");

=======

	if (msg->flags & I2C_M_IGNORE_NAK)
		packet_header |= I2C_HEADER_CONT_ON_NAK;

	if (msg->flags & I2C_M_RD)
		packet_header |= I2C_HEADER_READ;

	if (i2c_dev->dma_mode && !i2c_dev->msg_read)
		*dma_buf++ = packet_header;
	else
		i2c_writel(i2c_dev, packet_header, I2C_TX_FIFO);
}

static int tegra_i2c_error_recover(struct tegra_i2c_dev *i2c_dev,
				   struct i2c_msg *msg)
{
	if (i2c_dev->msg_err == I2C_ERR_NONE)
		return 0;

	tegra_i2c_init(i2c_dev);

	/* start recovery upon arbitration loss in single master mode */
	if (i2c_dev->msg_err == I2C_ERR_ARBITRATION_LOST) {
		if (!i2c_dev->multimaster_mode)
			return i2c_recover_bus(&i2c_dev->adapter);

		return -EAGAIN;
	}

	if (i2c_dev->msg_err == I2C_ERR_NO_ACK) {
		if (msg->flags & I2C_M_IGNORE_NAK)
			return 0;

		return -EREMOTEIO;
	}

	return -EIO;
}

static int tegra_i2c_xfer_msg(struct tegra_i2c_dev *i2c_dev,
			      struct i2c_msg *msg,
			      enum msg_end_type end_state)
{
	unsigned long time_left, xfer_time = 100;
	size_t xfer_size;
	u32 int_mask;
	int err;

	err = tegra_i2c_flush_fifos(i2c_dev);
	if (err)
		return err;

	i2c_dev->msg_buf = msg->buf;
	i2c_dev->msg_buf_remaining = msg->len;
	i2c_dev->msg_err = I2C_ERR_NONE;
	i2c_dev->msg_read = !!(msg->flags & I2C_M_RD);
	reinit_completion(&i2c_dev->msg_complete);

	if (i2c_dev->msg_read)
		xfer_size = msg->len;
	else
		xfer_size = msg->len + I2C_PACKET_HEADER_SIZE;

	xfer_size = ALIGN(xfer_size, BYTES_PER_FIFO_WORD);

	i2c_dev->dma_mode = xfer_size > I2C_PIO_MODE_PREFERRED_LEN &&
			    i2c_dev->dma_buf && !i2c_dev->atomic_mode;

	tegra_i2c_config_fifo_trig(i2c_dev, xfer_size);

	/*
	 * Transfer time in mSec = Total bits / transfer rate
	 * Total bits = 9 bits per byte (including ACK bit) + Start & stop bits
	 */
	xfer_time += DIV_ROUND_CLOSEST(((xfer_size * 9) + 2) * MSEC_PER_SEC,
				       i2c_dev->bus_clk_rate);

	int_mask = I2C_INT_NO_ACK | I2C_INT_ARBITRATION_LOST;
	tegra_i2c_unmask_irq(i2c_dev, int_mask);

	if (i2c_dev->dma_mode) {
		if (i2c_dev->msg_read) {
			dma_sync_single_for_device(i2c_dev->dev,
						   i2c_dev->dma_phys,
						   xfer_size, DMA_FROM_DEVICE);

			err = tegra_i2c_dma_submit(i2c_dev, xfer_size);
			if (err)
				return err;
		} else {
			dma_sync_single_for_cpu(i2c_dev->dev,
						i2c_dev->dma_phys,
						xfer_size, DMA_TO_DEVICE);
		}
	}

	tegra_i2c_push_packet_header(i2c_dev, msg, end_state);

	if (!i2c_dev->msg_read) {
		if (i2c_dev->dma_mode) {
			memcpy(i2c_dev->dma_buf + I2C_PACKET_HEADER_SIZE,
			       msg->buf, msg->len);

			dma_sync_single_for_device(i2c_dev->dev,
						   i2c_dev->dma_phys,
						   xfer_size, DMA_TO_DEVICE);

			err = tegra_i2c_dma_submit(i2c_dev, xfer_size);
			if (err)
				return err;
		} else {
			tegra_i2c_fill_tx_fifo(i2c_dev);
		}
	}

	if (i2c_dev->hw->has_per_pkt_xfer_complete_irq)
		int_mask |= I2C_INT_PACKET_XFER_COMPLETE;

	if (!i2c_dev->dma_mode) {
		if (msg->flags & I2C_M_RD)
			int_mask |= I2C_INT_RX_FIFO_DATA_REQ;
		else if (i2c_dev->msg_buf_remaining)
			int_mask |= I2C_INT_TX_FIFO_DATA_REQ;
	}

	tegra_i2c_unmask_irq(i2c_dev, int_mask);
	dev_dbg(i2c_dev->dev, "unmasked IRQ: %02x\n",
		i2c_readl(i2c_dev, I2C_INT_MASK));

	if (i2c_dev->dma_mode) {
		time_left = tegra_i2c_wait_completion(i2c_dev,
						      &i2c_dev->dma_complete,
						      xfer_time);

		/*
		 * Synchronize DMA first, since dmaengine_terminate_sync()
		 * performs synchronization after the transfer's termination
		 * and we want to get a completion if transfer succeeded.
		 */
		dmaengine_synchronize(i2c_dev->msg_read ?
				      i2c_dev->rx_dma_chan :
				      i2c_dev->tx_dma_chan);

		dmaengine_terminate_sync(i2c_dev->msg_read ?
					 i2c_dev->rx_dma_chan :
					 i2c_dev->tx_dma_chan);

		if (!time_left && !completion_done(&i2c_dev->dma_complete)) {
			dev_err(i2c_dev->dev, "DMA transfer timed out\n");
			tegra_i2c_init(i2c_dev);
			return -ETIMEDOUT;
		}

		if (i2c_dev->msg_read && i2c_dev->msg_err == I2C_ERR_NONE) {
			dma_sync_single_for_cpu(i2c_dev->dev,
						i2c_dev->dma_phys,
						xfer_size, DMA_FROM_DEVICE);

			memcpy(i2c_dev->msg_buf, i2c_dev->dma_buf, msg->len);
		}
	}

	time_left = tegra_i2c_wait_completion(i2c_dev, &i2c_dev->msg_complete,
					      xfer_time);

	tegra_i2c_mask_irq(i2c_dev, int_mask);

	if (time_left == 0) {
		dev_err(i2c_dev->dev, "I2C transfer timed out\n");
>>>>>>> upstream/android-13
		tegra_i2c_init(i2c_dev);
		return -ETIMEDOUT;
	}

	dev_dbg(i2c_dev->dev, "transfer complete: %lu %d %d\n",
		time_left, completion_done(&i2c_dev->msg_complete),
		i2c_dev->msg_err);

<<<<<<< HEAD
	if (likely(i2c_dev->msg_err == I2C_ERR_NONE))
		return 0;

	tegra_i2c_init(i2c_dev);
	if (i2c_dev->msg_err == I2C_ERR_NO_ACK) {
		if (msg->flags & I2C_M_IGNORE_NAK)
			return 0;
		return -EREMOTEIO;
	}

	return -EIO;
}

static int tegra_i2c_xfer(struct i2c_adapter *adap, struct i2c_msg msgs[],
	int num)
{
	struct tegra_i2c_dev *i2c_dev = i2c_get_adapdata(adap);
	int i;
	int ret = 0;
=======
	i2c_dev->dma_mode = false;

	err = tegra_i2c_error_recover(i2c_dev, msg);
	if (err)
		return err;

	return 0;
}

static int tegra_i2c_xfer(struct i2c_adapter *adap, struct i2c_msg msgs[],
			  int num)
{
	struct tegra_i2c_dev *i2c_dev = i2c_get_adapdata(adap);
	int i, ret;
>>>>>>> upstream/android-13

	ret = pm_runtime_get_sync(i2c_dev->dev);
	if (ret < 0) {
		dev_err(i2c_dev->dev, "runtime resume failed %d\n", ret);
<<<<<<< HEAD
=======
		pm_runtime_put_noidle(i2c_dev->dev);
>>>>>>> upstream/android-13
		return ret;
	}

	for (i = 0; i < num; i++) {
		enum msg_end_type end_type = MSG_END_STOP;

		if (i < (num - 1)) {
<<<<<<< HEAD
=======
			/* check whether follow up message is coming */
>>>>>>> upstream/android-13
			if (msgs[i + 1].flags & I2C_M_NOSTART)
				end_type = MSG_END_CONTINUE;
			else
				end_type = MSG_END_REPEAT_START;
		}
		ret = tegra_i2c_xfer_msg(i2c_dev, &msgs[i], end_type);
		if (ret)
			break;
	}

	pm_runtime_put(i2c_dev->dev);

	return ret ?: i;
}

<<<<<<< HEAD
=======
static int tegra_i2c_xfer_atomic(struct i2c_adapter *adap,
				 struct i2c_msg msgs[], int num)
{
	struct tegra_i2c_dev *i2c_dev = i2c_get_adapdata(adap);
	int ret;

	i2c_dev->atomic_mode = true;
	ret = tegra_i2c_xfer(adap, msgs, num);
	i2c_dev->atomic_mode = false;

	return ret;
}

>>>>>>> upstream/android-13
static u32 tegra_i2c_func(struct i2c_adapter *adap)
{
	struct tegra_i2c_dev *i2c_dev = i2c_get_adapdata(adap);
	u32 ret = I2C_FUNC_I2C | (I2C_FUNC_SMBUS_EMUL & ~I2C_FUNC_SMBUS_QUICK) |
		  I2C_FUNC_10BIT_ADDR |	I2C_FUNC_PROTOCOL_MANGLING;

	if (i2c_dev->hw->has_continue_xfer_support)
		ret |= I2C_FUNC_NOSTART;
<<<<<<< HEAD
	return ret;
}

static void tegra_i2c_parse_dt(struct tegra_i2c_dev *i2c_dev)
{
	struct device_node *np = i2c_dev->dev->of_node;
	int ret;

	ret = of_property_read_u32(np, "clock-frequency",
			&i2c_dev->bus_clk_rate);
	if (ret)
		i2c_dev->bus_clk_rate = 100000; /* default clock rate */

	i2c_dev->is_multimaster_mode = of_property_read_bool(np,
			"multi-master");
}

static const struct i2c_algorithm tegra_i2c_algo = {
	.master_xfer	= tegra_i2c_xfer,
	.functionality	= tegra_i2c_func,
=======

	return ret;
}

static const struct i2c_algorithm tegra_i2c_algo = {
	.master_xfer		= tegra_i2c_xfer,
	.master_xfer_atomic	= tegra_i2c_xfer_atomic,
	.functionality		= tegra_i2c_func,
>>>>>>> upstream/android-13
};

/* payload size is only 12 bit */
static const struct i2c_adapter_quirks tegra_i2c_quirks = {
	.flags = I2C_AQ_NO_ZERO_LEN,
<<<<<<< HEAD
	.max_read_len = 4096,
	.max_write_len = 4096 - 12,
=======
	.max_read_len = SZ_4K,
	.max_write_len = SZ_4K - I2C_PACKET_HEADER_SIZE,
>>>>>>> upstream/android-13
};

static const struct i2c_adapter_quirks tegra194_i2c_quirks = {
	.flags = I2C_AQ_NO_ZERO_LEN,
<<<<<<< HEAD
=======
	.max_write_len = SZ_64K - I2C_PACKET_HEADER_SIZE,
};

static struct i2c_bus_recovery_info tegra_i2c_recovery_info = {
	.recover_bus = tegra_i2c_issue_bus_clear,
>>>>>>> upstream/android-13
};

static const struct tegra_i2c_hw_feature tegra20_i2c_hw = {
	.has_continue_xfer_support = false,
	.has_per_pkt_xfer_complete_irq = false,
<<<<<<< HEAD
	.has_single_clk_source = false,
	.clk_divisor_hs_mode = 3,
	.clk_divisor_std_fast_mode = 0,
=======
	.clk_divisor_hs_mode = 3,
	.clk_divisor_std_mode = 0,
	.clk_divisor_fast_mode = 0,
>>>>>>> upstream/android-13
	.clk_divisor_fast_plus_mode = 0,
	.has_config_load_reg = false,
	.has_multi_master_mode = false,
	.has_slcg_override_reg = false,
	.has_mst_fifo = false,
	.quirks = &tegra_i2c_quirks,
<<<<<<< HEAD
=======
	.supports_bus_clear = false,
	.has_apb_dma = true,
	.tlow_std_mode = 0x4,
	.thigh_std_mode = 0x2,
	.tlow_fast_fastplus_mode = 0x4,
	.thigh_fast_fastplus_mode = 0x2,
	.setup_hold_time_std_mode = 0x0,
	.setup_hold_time_fast_fast_plus_mode = 0x0,
	.setup_hold_time_hs_mode = 0x0,
	.has_interface_timing_reg = false,
>>>>>>> upstream/android-13
};

static const struct tegra_i2c_hw_feature tegra30_i2c_hw = {
	.has_continue_xfer_support = true,
	.has_per_pkt_xfer_complete_irq = false,
<<<<<<< HEAD
	.has_single_clk_source = false,
	.clk_divisor_hs_mode = 3,
	.clk_divisor_std_fast_mode = 0,
=======
	.clk_divisor_hs_mode = 3,
	.clk_divisor_std_mode = 0,
	.clk_divisor_fast_mode = 0,
>>>>>>> upstream/android-13
	.clk_divisor_fast_plus_mode = 0,
	.has_config_load_reg = false,
	.has_multi_master_mode = false,
	.has_slcg_override_reg = false,
	.has_mst_fifo = false,
	.quirks = &tegra_i2c_quirks,
<<<<<<< HEAD
=======
	.supports_bus_clear = false,
	.has_apb_dma = true,
	.tlow_std_mode = 0x4,
	.thigh_std_mode = 0x2,
	.tlow_fast_fastplus_mode = 0x4,
	.thigh_fast_fastplus_mode = 0x2,
	.setup_hold_time_std_mode = 0x0,
	.setup_hold_time_fast_fast_plus_mode = 0x0,
	.setup_hold_time_hs_mode = 0x0,
	.has_interface_timing_reg = false,
>>>>>>> upstream/android-13
};

static const struct tegra_i2c_hw_feature tegra114_i2c_hw = {
	.has_continue_xfer_support = true,
	.has_per_pkt_xfer_complete_irq = true,
<<<<<<< HEAD
	.has_single_clk_source = true,
	.clk_divisor_hs_mode = 1,
	.clk_divisor_std_fast_mode = 0x19,
=======
	.clk_divisor_hs_mode = 1,
	.clk_divisor_std_mode = 0x19,
	.clk_divisor_fast_mode = 0x19,
>>>>>>> upstream/android-13
	.clk_divisor_fast_plus_mode = 0x10,
	.has_config_load_reg = false,
	.has_multi_master_mode = false,
	.has_slcg_override_reg = false,
	.has_mst_fifo = false,
	.quirks = &tegra_i2c_quirks,
<<<<<<< HEAD
=======
	.supports_bus_clear = true,
	.has_apb_dma = true,
	.tlow_std_mode = 0x4,
	.thigh_std_mode = 0x2,
	.tlow_fast_fastplus_mode = 0x4,
	.thigh_fast_fastplus_mode = 0x2,
	.setup_hold_time_std_mode = 0x0,
	.setup_hold_time_fast_fast_plus_mode = 0x0,
	.setup_hold_time_hs_mode = 0x0,
	.has_interface_timing_reg = false,
>>>>>>> upstream/android-13
};

static const struct tegra_i2c_hw_feature tegra124_i2c_hw = {
	.has_continue_xfer_support = true,
	.has_per_pkt_xfer_complete_irq = true,
<<<<<<< HEAD
	.has_single_clk_source = true,
	.clk_divisor_hs_mode = 1,
	.clk_divisor_std_fast_mode = 0x19,
=======
	.clk_divisor_hs_mode = 1,
	.clk_divisor_std_mode = 0x19,
	.clk_divisor_fast_mode = 0x19,
>>>>>>> upstream/android-13
	.clk_divisor_fast_plus_mode = 0x10,
	.has_config_load_reg = true,
	.has_multi_master_mode = false,
	.has_slcg_override_reg = true,
	.has_mst_fifo = false,
	.quirks = &tegra_i2c_quirks,
<<<<<<< HEAD
=======
	.supports_bus_clear = true,
	.has_apb_dma = true,
	.tlow_std_mode = 0x4,
	.thigh_std_mode = 0x2,
	.tlow_fast_fastplus_mode = 0x4,
	.thigh_fast_fastplus_mode = 0x2,
	.setup_hold_time_std_mode = 0x0,
	.setup_hold_time_fast_fast_plus_mode = 0x0,
	.setup_hold_time_hs_mode = 0x0,
	.has_interface_timing_reg = true,
>>>>>>> upstream/android-13
};

static const struct tegra_i2c_hw_feature tegra210_i2c_hw = {
	.has_continue_xfer_support = true,
	.has_per_pkt_xfer_complete_irq = true,
<<<<<<< HEAD
	.has_single_clk_source = true,
	.clk_divisor_hs_mode = 1,
	.clk_divisor_std_fast_mode = 0x19,
	.clk_divisor_fast_plus_mode = 0x10,
	.has_config_load_reg = true,
	.has_multi_master_mode = true,
	.has_slcg_override_reg = true,
	.has_mst_fifo = false,
	.quirks = &tegra_i2c_quirks,
=======
	.clk_divisor_hs_mode = 1,
	.clk_divisor_std_mode = 0x19,
	.clk_divisor_fast_mode = 0x19,
	.clk_divisor_fast_plus_mode = 0x10,
	.has_config_load_reg = true,
	.has_multi_master_mode = false,
	.has_slcg_override_reg = true,
	.has_mst_fifo = false,
	.quirks = &tegra_i2c_quirks,
	.supports_bus_clear = true,
	.has_apb_dma = true,
	.tlow_std_mode = 0x4,
	.thigh_std_mode = 0x2,
	.tlow_fast_fastplus_mode = 0x4,
	.thigh_fast_fastplus_mode = 0x2,
	.setup_hold_time_std_mode = 0,
	.setup_hold_time_fast_fast_plus_mode = 0,
	.setup_hold_time_hs_mode = 0,
	.has_interface_timing_reg = true,
};

static const struct tegra_i2c_hw_feature tegra186_i2c_hw = {
	.has_continue_xfer_support = true,
	.has_per_pkt_xfer_complete_irq = true,
	.clk_divisor_hs_mode = 1,
	.clk_divisor_std_mode = 0x16,
	.clk_divisor_fast_mode = 0x19,
	.clk_divisor_fast_plus_mode = 0x10,
	.has_config_load_reg = true,
	.has_multi_master_mode = false,
	.has_slcg_override_reg = true,
	.has_mst_fifo = false,
	.quirks = &tegra_i2c_quirks,
	.supports_bus_clear = true,
	.has_apb_dma = false,
	.tlow_std_mode = 0x4,
	.thigh_std_mode = 0x3,
	.tlow_fast_fastplus_mode = 0x4,
	.thigh_fast_fastplus_mode = 0x2,
	.setup_hold_time_std_mode = 0,
	.setup_hold_time_fast_fast_plus_mode = 0,
	.setup_hold_time_hs_mode = 0,
	.has_interface_timing_reg = true,
>>>>>>> upstream/android-13
};

static const struct tegra_i2c_hw_feature tegra194_i2c_hw = {
	.has_continue_xfer_support = true,
	.has_per_pkt_xfer_complete_irq = true,
<<<<<<< HEAD
	.has_single_clk_source = true,
	.clk_divisor_hs_mode = 1,
	.clk_divisor_std_fast_mode = 0x19,
	.clk_divisor_fast_plus_mode = 0x10,
=======
	.clk_divisor_hs_mode = 1,
	.clk_divisor_std_mode = 0x4f,
	.clk_divisor_fast_mode = 0x3c,
	.clk_divisor_fast_plus_mode = 0x16,
>>>>>>> upstream/android-13
	.has_config_load_reg = true,
	.has_multi_master_mode = true,
	.has_slcg_override_reg = true,
	.has_mst_fifo = true,
	.quirks = &tegra194_i2c_quirks,
<<<<<<< HEAD
};

/* Match table for of_platform binding */
static const struct of_device_id tegra_i2c_of_match[] = {
	{ .compatible = "nvidia,tegra194-i2c", .data = &tegra194_i2c_hw, },
=======
	.supports_bus_clear = true,
	.has_apb_dma = false,
	.tlow_std_mode = 0x8,
	.thigh_std_mode = 0x7,
	.tlow_fast_fastplus_mode = 0x2,
	.thigh_fast_fastplus_mode = 0x2,
	.setup_hold_time_std_mode = 0x08080808,
	.setup_hold_time_fast_fast_plus_mode = 0x02020202,
	.setup_hold_time_hs_mode = 0x090909,
	.has_interface_timing_reg = true,
};

static const struct of_device_id tegra_i2c_of_match[] = {
	{ .compatible = "nvidia,tegra194-i2c", .data = &tegra194_i2c_hw, },
	{ .compatible = "nvidia,tegra186-i2c", .data = &tegra186_i2c_hw, },
	{ .compatible = "nvidia,tegra210-i2c-vi", .data = &tegra210_i2c_hw, },
>>>>>>> upstream/android-13
	{ .compatible = "nvidia,tegra210-i2c", .data = &tegra210_i2c_hw, },
	{ .compatible = "nvidia,tegra124-i2c", .data = &tegra124_i2c_hw, },
	{ .compatible = "nvidia,tegra114-i2c", .data = &tegra114_i2c_hw, },
	{ .compatible = "nvidia,tegra30-i2c", .data = &tegra30_i2c_hw, },
	{ .compatible = "nvidia,tegra20-i2c", .data = &tegra20_i2c_hw, },
	{ .compatible = "nvidia,tegra20-i2c-dvc", .data = &tegra20_i2c_hw, },
	{},
};
MODULE_DEVICE_TABLE(of, tegra_i2c_of_match);

<<<<<<< HEAD
=======
static void tegra_i2c_parse_dt(struct tegra_i2c_dev *i2c_dev)
{
	struct device_node *np = i2c_dev->dev->of_node;
	bool multi_mode;
	int err;

	err = of_property_read_u32(np, "clock-frequency",
				   &i2c_dev->bus_clk_rate);
	if (err)
		i2c_dev->bus_clk_rate = I2C_MAX_STANDARD_MODE_FREQ;

	multi_mode = of_property_read_bool(np, "multi-master");
	i2c_dev->multimaster_mode = multi_mode;

	if (of_device_is_compatible(np, "nvidia,tegra20-i2c-dvc"))
		i2c_dev->is_dvc = true;

	if (of_device_is_compatible(np, "nvidia,tegra210-i2c-vi"))
		i2c_dev->is_vi = true;
}

static int tegra_i2c_init_clocks(struct tegra_i2c_dev *i2c_dev)
{
	int err;

	i2c_dev->clocks[i2c_dev->nclocks++].id = "div-clk";

	if (i2c_dev->hw == &tegra20_i2c_hw || i2c_dev->hw == &tegra30_i2c_hw)
		i2c_dev->clocks[i2c_dev->nclocks++].id = "fast-clk";

	if (i2c_dev->is_vi)
		i2c_dev->clocks[i2c_dev->nclocks++].id = "slow";

	err = devm_clk_bulk_get(i2c_dev->dev, i2c_dev->nclocks,
				i2c_dev->clocks);
	if (err)
		return err;

	err = clk_bulk_prepare(i2c_dev->nclocks, i2c_dev->clocks);
	if (err)
		return err;

	i2c_dev->div_clk = i2c_dev->clocks[0].clk;

	if (!i2c_dev->multimaster_mode)
		return 0;

	err = clk_enable(i2c_dev->div_clk);
	if (err) {
		dev_err(i2c_dev->dev, "failed to enable div-clk: %d\n", err);
		goto unprepare_clocks;
	}

	return 0;

unprepare_clocks:
	clk_bulk_unprepare(i2c_dev->nclocks, i2c_dev->clocks);

	return err;
}

static void tegra_i2c_release_clocks(struct tegra_i2c_dev *i2c_dev)
{
	if (i2c_dev->multimaster_mode)
		clk_disable(i2c_dev->div_clk);

	clk_bulk_unprepare(i2c_dev->nclocks, i2c_dev->clocks);
}

static int tegra_i2c_init_hardware(struct tegra_i2c_dev *i2c_dev)
{
	int ret;

	ret = pm_runtime_get_sync(i2c_dev->dev);
	if (ret < 0)
		dev_err(i2c_dev->dev, "runtime resume failed: %d\n", ret);
	else
		ret = tegra_i2c_init(i2c_dev);

	pm_runtime_put(i2c_dev->dev);

	return ret;
}

>>>>>>> upstream/android-13
static int tegra_i2c_probe(struct platform_device *pdev)
{
	struct tegra_i2c_dev *i2c_dev;
	struct resource *res;
<<<<<<< HEAD
	struct clk *div_clk;
	struct clk *fast_clk;
	void __iomem *base;
	int irq;
	int ret = 0;
	int clk_multiplier = I2C_CLK_MULTIPLIER_STD_FAST_MODE;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(base))
		return PTR_ERR(base);

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err(&pdev->dev, "no irq resource\n");
		return -EINVAL;
	}
	irq = res->start;

	div_clk = devm_clk_get(&pdev->dev, "div-clk");
	if (IS_ERR(div_clk)) {
		dev_err(&pdev->dev, "missing controller clock\n");
		return PTR_ERR(div_clk);
	}
=======
	int err;
>>>>>>> upstream/android-13

	i2c_dev = devm_kzalloc(&pdev->dev, sizeof(*i2c_dev), GFP_KERNEL);
	if (!i2c_dev)
		return -ENOMEM;

<<<<<<< HEAD
	i2c_dev->base = base;
	i2c_dev->div_clk = div_clk;
	i2c_dev->adapter.algo = &tegra_i2c_algo;
	i2c_dev->irq = irq;
	i2c_dev->cont_id = pdev->id;
	i2c_dev->dev = &pdev->dev;

	i2c_dev->rst = devm_reset_control_get_exclusive(&pdev->dev, "i2c");
	if (IS_ERR(i2c_dev->rst)) {
		dev_err(&pdev->dev, "missing controller reset\n");
=======
	platform_set_drvdata(pdev, i2c_dev);

	init_completion(&i2c_dev->msg_complete);
	init_completion(&i2c_dev->dma_complete);

	i2c_dev->hw = of_device_get_match_data(&pdev->dev);
	i2c_dev->cont_id = pdev->id;
	i2c_dev->dev = &pdev->dev;

	i2c_dev->base = devm_platform_get_and_ioremap_resource(pdev, 0, &res);
	if (IS_ERR(i2c_dev->base))
		return PTR_ERR(i2c_dev->base);

	i2c_dev->base_phys = res->start;

	err = platform_get_irq(pdev, 0);
	if (err < 0)
		return err;

	i2c_dev->irq = err;

	/* interrupt will be enabled during of transfer time */
	irq_set_status_flags(i2c_dev->irq, IRQ_NOAUTOEN);

	err = devm_request_threaded_irq(i2c_dev->dev, i2c_dev->irq,
					NULL, tegra_i2c_isr,
					IRQF_NO_SUSPEND | IRQF_ONESHOT,
					dev_name(i2c_dev->dev), i2c_dev);
	if (err)
		return err;

	i2c_dev->rst = devm_reset_control_get_exclusive(i2c_dev->dev, "i2c");
	if (IS_ERR(i2c_dev->rst)) {
		dev_err_probe(i2c_dev->dev, PTR_ERR(i2c_dev->rst),
			      "failed to get reset control\n");
>>>>>>> upstream/android-13
		return PTR_ERR(i2c_dev->rst);
	}

	tegra_i2c_parse_dt(i2c_dev);

<<<<<<< HEAD
	i2c_dev->hw = of_device_get_match_data(&pdev->dev);
	i2c_dev->is_dvc = of_device_is_compatible(pdev->dev.of_node,
						  "nvidia,tegra20-i2c-dvc");
	i2c_dev->adapter.quirks = i2c_dev->hw->quirks;
	init_completion(&i2c_dev->msg_complete);
	spin_lock_init(&i2c_dev->xfer_lock);

	if (!i2c_dev->hw->has_single_clk_source) {
		fast_clk = devm_clk_get(&pdev->dev, "fast-clk");
		if (IS_ERR(fast_clk)) {
			dev_err(&pdev->dev, "missing fast clock\n");
			return PTR_ERR(fast_clk);
		}
		i2c_dev->fast_clk = fast_clk;
	}

	platform_set_drvdata(pdev, i2c_dev);

	if (!i2c_dev->hw->has_single_clk_source) {
		ret = clk_prepare(i2c_dev->fast_clk);
		if (ret < 0) {
			dev_err(i2c_dev->dev, "Clock prepare failed %d\n", ret);
			return ret;
		}
	}

	i2c_dev->clk_divisor_non_hs_mode =
			i2c_dev->hw->clk_divisor_std_fast_mode;
	if (i2c_dev->hw->clk_divisor_fast_plus_mode &&
		(i2c_dev->bus_clk_rate == 1000000))
		i2c_dev->clk_divisor_non_hs_mode =
			i2c_dev->hw->clk_divisor_fast_plus_mode;

	clk_multiplier *= (i2c_dev->clk_divisor_non_hs_mode + 1);
	ret = clk_set_rate(i2c_dev->div_clk,
			   i2c_dev->bus_clk_rate * clk_multiplier);
	if (ret) {
		dev_err(i2c_dev->dev, "Clock rate change failed %d\n", ret);
		goto unprepare_fast_clk;
	}

	ret = clk_prepare(i2c_dev->div_clk);
	if (ret < 0) {
		dev_err(i2c_dev->dev, "Clock prepare failed %d\n", ret);
		goto unprepare_fast_clk;
	}

	pm_runtime_enable(&pdev->dev);
	if (!pm_runtime_enabled(&pdev->dev)) {
		ret = tegra_i2c_runtime_resume(&pdev->dev);
		if (ret < 0) {
			dev_err(&pdev->dev, "runtime resume failed\n");
			goto unprepare_div_clk;
		}
	}

	if (i2c_dev->is_multimaster_mode) {
		ret = clk_enable(i2c_dev->div_clk);
		if (ret < 0) {
			dev_err(i2c_dev->dev, "div_clk enable failed %d\n",
				ret);
			goto disable_rpm;
		}
	}

	ret = tegra_i2c_init(i2c_dev);
	if (ret) {
		dev_err(&pdev->dev, "Failed to initialize i2c controller\n");
		goto disable_div_clk;
	}

	ret = devm_request_irq(&pdev->dev, i2c_dev->irq,
			tegra_i2c_isr, 0, dev_name(&pdev->dev), i2c_dev);
	if (ret) {
		dev_err(&pdev->dev, "Failed to request irq %i\n", i2c_dev->irq);
		goto disable_div_clk;
	}

	i2c_set_adapdata(&i2c_dev->adapter, i2c_dev);
	i2c_dev->adapter.owner = THIS_MODULE;
	i2c_dev->adapter.class = I2C_CLASS_DEPRECATED;
	strlcpy(i2c_dev->adapter.name, dev_name(&pdev->dev),
		sizeof(i2c_dev->adapter.name));
	i2c_dev->adapter.dev.parent = &pdev->dev;
	i2c_dev->adapter.nr = pdev->id;
	i2c_dev->adapter.dev.of_node = pdev->dev.of_node;

	ret = i2c_add_numbered_adapter(&i2c_dev->adapter);
	if (ret)
		goto disable_div_clk;

	return 0;

disable_div_clk:
	if (i2c_dev->is_multimaster_mode)
		clk_disable(i2c_dev->div_clk);

disable_rpm:
	pm_runtime_disable(&pdev->dev);
	if (!pm_runtime_status_suspended(&pdev->dev))
		tegra_i2c_runtime_suspend(&pdev->dev);

unprepare_div_clk:
	clk_unprepare(i2c_dev->div_clk);

unprepare_fast_clk:
	if (!i2c_dev->hw->has_single_clk_source)
		clk_unprepare(i2c_dev->fast_clk);

	return ret;
=======
	err = tegra_i2c_init_clocks(i2c_dev);
	if (err)
		return err;

	err = tegra_i2c_init_dma(i2c_dev);
	if (err)
		goto release_clocks;

	/*
	 * VI I2C is in VE power domain which is not always ON and not
	 * IRQ-safe.  Thus, IRQ-safe device shouldn't be attached to a
	 * non IRQ-safe domain because this prevents powering off the power
	 * domain.
	 *
	 * VI I2C device shouldn't be marked as IRQ-safe because VI I2C won't
	 * be used for atomic transfers.
	 */
	if (!i2c_dev->is_vi)
		pm_runtime_irq_safe(i2c_dev->dev);

	pm_runtime_enable(i2c_dev->dev);

	err = tegra_i2c_init_hardware(i2c_dev);
	if (err)
		goto release_rpm;

	i2c_set_adapdata(&i2c_dev->adapter, i2c_dev);
	i2c_dev->adapter.dev.of_node = i2c_dev->dev->of_node;
	i2c_dev->adapter.dev.parent = i2c_dev->dev;
	i2c_dev->adapter.retries = 1;
	i2c_dev->adapter.timeout = 6 * HZ;
	i2c_dev->adapter.quirks = i2c_dev->hw->quirks;
	i2c_dev->adapter.owner = THIS_MODULE;
	i2c_dev->adapter.class = I2C_CLASS_DEPRECATED;
	i2c_dev->adapter.algo = &tegra_i2c_algo;
	i2c_dev->adapter.nr = pdev->id;

	if (i2c_dev->hw->supports_bus_clear)
		i2c_dev->adapter.bus_recovery_info = &tegra_i2c_recovery_info;

	strlcpy(i2c_dev->adapter.name, dev_name(i2c_dev->dev),
		sizeof(i2c_dev->adapter.name));

	err = i2c_add_numbered_adapter(&i2c_dev->adapter);
	if (err)
		goto release_rpm;

	return 0;

release_rpm:
	pm_runtime_disable(i2c_dev->dev);

	tegra_i2c_release_dma(i2c_dev);
release_clocks:
	tegra_i2c_release_clocks(i2c_dev);

	return err;
>>>>>>> upstream/android-13
}

static int tegra_i2c_remove(struct platform_device *pdev)
{
	struct tegra_i2c_dev *i2c_dev = platform_get_drvdata(pdev);

	i2c_del_adapter(&i2c_dev->adapter);
<<<<<<< HEAD

	if (i2c_dev->is_multimaster_mode)
		clk_disable(i2c_dev->div_clk);

	pm_runtime_disable(&pdev->dev);
	if (!pm_runtime_status_suspended(&pdev->dev))
		tegra_i2c_runtime_suspend(&pdev->dev);

	clk_unprepare(i2c_dev->div_clk);
	if (!i2c_dev->hw->has_single_clk_source)
		clk_unprepare(i2c_dev->fast_clk);
=======
	pm_runtime_disable(i2c_dev->dev);

	tegra_i2c_release_dma(i2c_dev);
	tegra_i2c_release_clocks(i2c_dev);

	return 0;
}

static int __maybe_unused tegra_i2c_runtime_resume(struct device *dev)
{
	struct tegra_i2c_dev *i2c_dev = dev_get_drvdata(dev);
	int err;

	err = pinctrl_pm_select_default_state(dev);
	if (err)
		return err;

	err = clk_bulk_enable(i2c_dev->nclocks, i2c_dev->clocks);
	if (err)
		return err;

	/*
	 * VI I2C device is attached to VE power domain which goes through
	 * power ON/OFF during runtime PM resume/suspend, meaning that
	 * controller needs to be re-initialized after power ON.
	 */
	if (i2c_dev->is_vi) {
		err = tegra_i2c_init(i2c_dev);
		if (err)
			goto disable_clocks;
	}

	return 0;

disable_clocks:
	clk_bulk_disable(i2c_dev->nclocks, i2c_dev->clocks);

	return err;
}

static int __maybe_unused tegra_i2c_runtime_suspend(struct device *dev)
{
	struct tegra_i2c_dev *i2c_dev = dev_get_drvdata(dev);

	clk_bulk_disable(i2c_dev->nclocks, i2c_dev->clocks);

	return pinctrl_pm_select_idle_state(dev);
}

static int __maybe_unused tegra_i2c_suspend(struct device *dev)
{
	struct tegra_i2c_dev *i2c_dev = dev_get_drvdata(dev);
	int err;

	i2c_mark_adapter_suspended(&i2c_dev->adapter);

	if (!pm_runtime_status_suspended(dev)) {
		err = tegra_i2c_runtime_suspend(dev);
		if (err)
			return err;
	}

	return 0;
}

static int __maybe_unused tegra_i2c_resume(struct device *dev)
{
	struct tegra_i2c_dev *i2c_dev = dev_get_drvdata(dev);
	int err;

	/*
	 * We need to ensure that clocks are enabled so that registers can be
	 * restored in tegra_i2c_init().
	 */
	err = tegra_i2c_runtime_resume(dev);
	if (err)
		return err;

	err = tegra_i2c_init(i2c_dev);
	if (err)
		return err;

	/*
	 * In case we are runtime suspended, disable clocks again so that we
	 * don't unbalance the clock reference counts during the next runtime
	 * resume transition.
	 */
	if (pm_runtime_status_suspended(dev)) {
		err = tegra_i2c_runtime_suspend(dev);
		if (err)
			return err;
	}

	i2c_mark_adapter_resumed(&i2c_dev->adapter);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM_SLEEP
static const struct dev_pm_ops tegra_i2c_pm = {
	SET_RUNTIME_PM_OPS(tegra_i2c_runtime_suspend, tegra_i2c_runtime_resume,
			   NULL)
};
#define TEGRA_I2C_PM	(&tegra_i2c_pm)
#else
#define TEGRA_I2C_PM	NULL
#endif

static struct platform_driver tegra_i2c_driver = {
	.probe   = tegra_i2c_probe,
	.remove  = tegra_i2c_remove,
	.driver  = {
		.name  = "tegra-i2c",
		.of_match_table = tegra_i2c_of_match,
		.pm    = TEGRA_I2C_PM,
	},
};

static int __init tegra_i2c_init_driver(void)
{
	return platform_driver_register(&tegra_i2c_driver);
}

static void __exit tegra_i2c_exit_driver(void)
{
	platform_driver_unregister(&tegra_i2c_driver);
}

subsys_initcall(tegra_i2c_init_driver);
module_exit(tegra_i2c_exit_driver);

MODULE_DESCRIPTION("nVidia Tegra2 I2C Bus Controller driver");
=======
static const struct dev_pm_ops tegra_i2c_pm = {
	SET_NOIRQ_SYSTEM_SLEEP_PM_OPS(tegra_i2c_suspend, tegra_i2c_resume)
	SET_RUNTIME_PM_OPS(tegra_i2c_runtime_suspend, tegra_i2c_runtime_resume,
			   NULL)
};

static struct platform_driver tegra_i2c_driver = {
	.probe = tegra_i2c_probe,
	.remove = tegra_i2c_remove,
	.driver = {
		.name = "tegra-i2c",
		.of_match_table = tegra_i2c_of_match,
		.pm = &tegra_i2c_pm,
	},
};
module_platform_driver(tegra_i2c_driver);

MODULE_DESCRIPTION("NVIDIA Tegra I2C Bus Controller driver");
>>>>>>> upstream/android-13
MODULE_AUTHOR("Colin Cross");
MODULE_LICENSE("GPL v2");
