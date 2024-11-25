// SPDX-License-Identifier: (GPL-2.0 OR BSD-3-Clause)
// Copyright(c) 2015-17 Intel Corporation.

/*
 * Soundwire Intel Master Driver
 */

#include <linux/acpi.h>
<<<<<<< HEAD
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <sound/pcm_params.h>
=======
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/auxiliary_bus.h>
#include <sound/pcm_params.h>
#include <linux/pm_runtime.h>
>>>>>>> upstream/android-13
#include <sound/soc.h>
#include <linux/soundwire/sdw_registers.h>
#include <linux/soundwire/sdw.h>
#include <linux/soundwire/sdw_intel.h>
#include "cadence_master.h"
<<<<<<< HEAD
#include "intel.h"

/* Intel SHIM Registers Definition */
#define SDW_SHIM_LCAP			0x0
#define SDW_SHIM_LCTL			0x4
#define SDW_SHIM_IPPTR			0x8
#define SDW_SHIM_SYNC			0xC

#define SDW_SHIM_CTLSCAP(x)		(0x010 + 0x60 * x)
#define SDW_SHIM_CTLS0CM(x)		(0x012 + 0x60 * x)
#define SDW_SHIM_CTLS1CM(x)		(0x014 + 0x60 * x)
#define SDW_SHIM_CTLS2CM(x)		(0x016 + 0x60 * x)
#define SDW_SHIM_CTLS3CM(x)		(0x018 + 0x60 * x)
#define SDW_SHIM_PCMSCAP(x)		(0x020 + 0x60 * x)

#define SDW_SHIM_PCMSYCHM(x, y)		(0x022 + (0x60 * x) + (0x2 * y))
#define SDW_SHIM_PCMSYCHC(x, y)		(0x042 + (0x60 * x) + (0x2 * y))
#define SDW_SHIM_PDMSCAP(x)		(0x062 + 0x60 * x)
#define SDW_SHIM_IOCTL(x)		(0x06C + 0x60 * x)
#define SDW_SHIM_CTMCTL(x)		(0x06E + 0x60 * x)

#define SDW_SHIM_WAKEEN			0x190
#define SDW_SHIM_WAKESTS		0x192

#define SDW_SHIM_LCTL_SPA		BIT(0)
#define SDW_SHIM_LCTL_CPA		BIT(8)

#define SDW_SHIM_SYNC_SYNCPRD_VAL	0x176F
#define SDW_SHIM_SYNC_SYNCPRD		GENMASK(14, 0)
#define SDW_SHIM_SYNC_SYNCCPU		BIT(15)
#define SDW_SHIM_SYNC_CMDSYNC_MASK	GENMASK(19, 16)
#define SDW_SHIM_SYNC_CMDSYNC		BIT(16)
#define SDW_SHIM_SYNC_SYNCGO		BIT(24)

#define SDW_SHIM_PCMSCAP_ISS		GENMASK(3, 0)
#define SDW_SHIM_PCMSCAP_OSS		GENMASK(7, 4)
#define SDW_SHIM_PCMSCAP_BSS		GENMASK(12, 8)

#define SDW_SHIM_PCMSYCM_LCHN		GENMASK(3, 0)
#define SDW_SHIM_PCMSYCM_HCHN		GENMASK(7, 4)
#define SDW_SHIM_PCMSYCM_STREAM		GENMASK(13, 8)
#define SDW_SHIM_PCMSYCM_DIR		BIT(15)

#define SDW_SHIM_PDMSCAP_ISS		GENMASK(3, 0)
#define SDW_SHIM_PDMSCAP_OSS		GENMASK(7, 4)
#define SDW_SHIM_PDMSCAP_BSS		GENMASK(12, 8)
#define SDW_SHIM_PDMSCAP_CPSS		GENMASK(15, 13)

#define SDW_SHIM_IOCTL_MIF		BIT(0)
#define SDW_SHIM_IOCTL_CO		BIT(1)
#define SDW_SHIM_IOCTL_COE		BIT(2)
#define SDW_SHIM_IOCTL_DO		BIT(3)
#define SDW_SHIM_IOCTL_DOE		BIT(4)
#define SDW_SHIM_IOCTL_BKE		BIT(5)
#define SDW_SHIM_IOCTL_WPDD		BIT(6)
#define SDW_SHIM_IOCTL_CIBD		BIT(8)
#define SDW_SHIM_IOCTL_DIBD		BIT(9)

#define SDW_SHIM_CTMCTL_DACTQE		BIT(0)
#define SDW_SHIM_CTMCTL_DODS		BIT(1)
#define SDW_SHIM_CTMCTL_DOAIS		GENMASK(4, 3)

#define SDW_SHIM_WAKEEN_ENABLE		BIT(0)
#define SDW_SHIM_WAKESTS_STATUS		BIT(0)

/* Intel ALH Register definitions */
#define SDW_ALH_STRMZCFG(x)		(0x000 + (0x4 * x))

#define SDW_ALH_STRMZCFG_DMAT_VAL	0x3
#define SDW_ALH_STRMZCFG_DMAT		GENMASK(7, 0)
#define SDW_ALH_STRMZCFG_CHN		GENMASK(19, 16)
=======
#include "bus.h"
#include "intel.h"

#define INTEL_MASTER_SUSPEND_DELAY_MS	3000
#define INTEL_MASTER_RESET_ITERATIONS	10

/*
 * debug/config flags for the Intel SoundWire Master.
 *
 * Since we may have multiple masters active, we can have up to 8
 * flags reused in each byte, with master0 using the ls-byte, etc.
 */

#define SDW_INTEL_MASTER_DISABLE_PM_RUNTIME		BIT(0)
#define SDW_INTEL_MASTER_DISABLE_CLOCK_STOP		BIT(1)
#define SDW_INTEL_MASTER_DISABLE_PM_RUNTIME_IDLE	BIT(2)
#define SDW_INTEL_MASTER_DISABLE_MULTI_LINK		BIT(3)

static int md_flags;
module_param_named(sdw_md_flags, md_flags, int, 0444);
MODULE_PARM_DESC(sdw_md_flags, "SoundWire Intel Master device flags (0x0 all off)");
>>>>>>> upstream/android-13

enum intel_pdi_type {
	INTEL_PDI_IN = 0,
	INTEL_PDI_OUT = 1,
	INTEL_PDI_BD = 2,
};

<<<<<<< HEAD
struct sdw_intel {
	struct sdw_cdns cdns;
	int instance;
	struct sdw_intel_link_res *res;
};

=======
>>>>>>> upstream/android-13
#define cdns_to_intel(_cdns) container_of(_cdns, struct sdw_intel, cdns)

/*
 * Read, write helpers for HW registers
 */
static inline int intel_readl(void __iomem *base, int offset)
{
	return readl(base + offset);
}

static inline void intel_writel(void __iomem *base, int offset, int value)
{
	writel(value, base + offset);
}

static inline u16 intel_readw(void __iomem *base, int offset)
{
	return readw(base + offset);
}

static inline void intel_writew(void __iomem *base, int offset, u16 value)
{
	writew(value, base + offset);
}

<<<<<<< HEAD
static int intel_clear_bit(void __iomem *base, int offset, u32 value, u32 mask)
=======
static int intel_wait_bit(void __iomem *base, int offset, u32 mask, u32 target)
>>>>>>> upstream/android-13
{
	int timeout = 10;
	u32 reg_read;

<<<<<<< HEAD
	writel(value, base + offset);
	do {
		reg_read = readl(base + offset);
		if (!(reg_read & mask))
			return 0;

		timeout--;
		udelay(50);
=======
	do {
		reg_read = readl(base + offset);
		if ((reg_read & mask) == target)
			return 0;

		timeout--;
		usleep_range(50, 100);
>>>>>>> upstream/android-13
	} while (timeout != 0);

	return -EAGAIN;
}

<<<<<<< HEAD
static int intel_set_bit(void __iomem *base, int offset, u32 value, u32 mask)
{
	int timeout = 10;
	u32 reg_read;

	writel(value, base + offset);
	do {
		reg_read = readl(base + offset);
		if (reg_read & mask)
			return 0;

		timeout--;
		udelay(50);
	} while (timeout != 0);

	return -EAGAIN;
}

/*
=======
static int intel_clear_bit(void __iomem *base, int offset, u32 value, u32 mask)
{
	writel(value, base + offset);
	return intel_wait_bit(base, offset, mask, 0);
}

static int intel_set_bit(void __iomem *base, int offset, u32 value, u32 mask)
{
	writel(value, base + offset);
	return intel_wait_bit(base, offset, mask, mask);
}

/*
 * debugfs
 */
#ifdef CONFIG_DEBUG_FS

#define RD_BUF (2 * PAGE_SIZE)

static ssize_t intel_sprintf(void __iomem *mem, bool l,
			     char *buf, size_t pos, unsigned int reg)
{
	int value;

	if (l)
		value = intel_readl(mem, reg);
	else
		value = intel_readw(mem, reg);

	return scnprintf(buf + pos, RD_BUF - pos, "%4x\t%4x\n", reg, value);
}

static int intel_reg_show(struct seq_file *s_file, void *data)
{
	struct sdw_intel *sdw = s_file->private;
	void __iomem *s = sdw->link_res->shim;
	void __iomem *a = sdw->link_res->alh;
	char *buf;
	ssize_t ret;
	int i, j;
	unsigned int links, reg;

	buf = kzalloc(RD_BUF, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	links = intel_readl(s, SDW_SHIM_LCAP) & GENMASK(2, 0);

	ret = scnprintf(buf, RD_BUF, "Register  Value\n");
	ret += scnprintf(buf + ret, RD_BUF - ret, "\nShim\n");

	for (i = 0; i < links; i++) {
		reg = SDW_SHIM_LCAP + i * 4;
		ret += intel_sprintf(s, true, buf, ret, reg);
	}

	for (i = 0; i < links; i++) {
		ret += scnprintf(buf + ret, RD_BUF - ret, "\nLink%d\n", i);
		ret += intel_sprintf(s, false, buf, ret, SDW_SHIM_CTLSCAP(i));
		ret += intel_sprintf(s, false, buf, ret, SDW_SHIM_CTLS0CM(i));
		ret += intel_sprintf(s, false, buf, ret, SDW_SHIM_CTLS1CM(i));
		ret += intel_sprintf(s, false, buf, ret, SDW_SHIM_CTLS2CM(i));
		ret += intel_sprintf(s, false, buf, ret, SDW_SHIM_CTLS3CM(i));
		ret += intel_sprintf(s, false, buf, ret, SDW_SHIM_PCMSCAP(i));

		ret += scnprintf(buf + ret, RD_BUF - ret, "\n PCMSyCH registers\n");

		/*
		 * the value 10 is the number of PDIs. We will need a
		 * cleanup to remove hard-coded Intel configurations
		 * from cadence_master.c
		 */
		for (j = 0; j < 10; j++) {
			ret += intel_sprintf(s, false, buf, ret,
					SDW_SHIM_PCMSYCHM(i, j));
			ret += intel_sprintf(s, false, buf, ret,
					SDW_SHIM_PCMSYCHC(i, j));
		}
		ret += scnprintf(buf + ret, RD_BUF - ret, "\n PDMSCAP, IOCTL, CTMCTL\n");

		ret += intel_sprintf(s, false, buf, ret, SDW_SHIM_PDMSCAP(i));
		ret += intel_sprintf(s, false, buf, ret, SDW_SHIM_IOCTL(i));
		ret += intel_sprintf(s, false, buf, ret, SDW_SHIM_CTMCTL(i));
	}

	ret += scnprintf(buf + ret, RD_BUF - ret, "\nWake registers\n");
	ret += intel_sprintf(s, false, buf, ret, SDW_SHIM_WAKEEN);
	ret += intel_sprintf(s, false, buf, ret, SDW_SHIM_WAKESTS);

	ret += scnprintf(buf + ret, RD_BUF - ret, "\nALH STRMzCFG\n");
	for (i = 0; i < SDW_ALH_NUM_STREAMS; i++)
		ret += intel_sprintf(a, true, buf, ret, SDW_ALH_STRMZCFG(i));

	seq_printf(s_file, "%s", buf);
	kfree(buf);

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(intel_reg);

static int intel_set_m_datamode(void *data, u64 value)
{
	struct sdw_intel *sdw = data;
	struct sdw_bus *bus = &sdw->cdns.bus;

	if (value > SDW_PORT_DATA_MODE_STATIC_1)
		return -EINVAL;

	/* Userspace changed the hardware state behind the kernel's back */
	add_taint(TAINT_USER, LOCKDEP_STILL_OK);

	bus->params.m_data_mode = value;

	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(intel_set_m_datamode_fops, NULL,
			 intel_set_m_datamode, "%llu\n");

static int intel_set_s_datamode(void *data, u64 value)
{
	struct sdw_intel *sdw = data;
	struct sdw_bus *bus = &sdw->cdns.bus;

	if (value > SDW_PORT_DATA_MODE_STATIC_1)
		return -EINVAL;

	/* Userspace changed the hardware state behind the kernel's back */
	add_taint(TAINT_USER, LOCKDEP_STILL_OK);

	bus->params.s_data_mode = value;

	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(intel_set_s_datamode_fops, NULL,
			 intel_set_s_datamode, "%llu\n");

static void intel_debugfs_init(struct sdw_intel *sdw)
{
	struct dentry *root = sdw->cdns.bus.debugfs;

	if (!root)
		return;

	sdw->debugfs = debugfs_create_dir("intel-sdw", root);

	debugfs_create_file("intel-registers", 0400, sdw->debugfs, sdw,
			    &intel_reg_fops);

	debugfs_create_file("intel-m-datamode", 0200, sdw->debugfs, sdw,
			    &intel_set_m_datamode_fops);

	debugfs_create_file("intel-s-datamode", 0200, sdw->debugfs, sdw,
			    &intel_set_s_datamode_fops);

	sdw_cdns_debugfs_init(&sdw->cdns, sdw->debugfs);
}

static void intel_debugfs_exit(struct sdw_intel *sdw)
{
	debugfs_remove_recursive(sdw->debugfs);
}
#else
static void intel_debugfs_init(struct sdw_intel *sdw) {}
static void intel_debugfs_exit(struct sdw_intel *sdw) {}
#endif /* CONFIG_DEBUG_FS */

/*
>>>>>>> upstream/android-13
 * shim ops
 */

static int intel_link_power_up(struct sdw_intel *sdw)
{
	unsigned int link_id = sdw->instance;
<<<<<<< HEAD
	void __iomem *shim = sdw->res->shim;
	int spa_mask, cpa_mask;
	int link_control, ret;

	/* Link power up sequence */
	link_control = intel_readl(shim, SDW_SHIM_LCTL);
	spa_mask = (SDW_SHIM_LCTL_SPA << link_id);
	cpa_mask = (SDW_SHIM_LCTL_CPA << link_id);
	link_control |=  spa_mask;

	ret = intel_set_bit(shim, SDW_SHIM_LCTL, link_control, cpa_mask);
	if (ret < 0)
		return ret;

	sdw->cdns.link_up = true;
	return 0;
}

static int intel_shim_init(struct sdw_intel *sdw)
{
	void __iomem *shim = sdw->res->shim;
	unsigned int link_id = sdw->instance;
	int sync_reg, ret;
	u16 ioctl = 0, act = 0;

	/* Initialize Shim */
	ioctl |= SDW_SHIM_IOCTL_BKE;
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);

	ioctl |= SDW_SHIM_IOCTL_WPDD;
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);

	ioctl |= SDW_SHIM_IOCTL_DO;
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);

	ioctl |= SDW_SHIM_IOCTL_DOE;
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);
=======
	void __iomem *shim = sdw->link_res->shim;
	u32 *shim_mask = sdw->link_res->shim_mask;
	struct sdw_bus *bus = &sdw->cdns.bus;
	struct sdw_master_prop *prop = &bus->prop;
	u32 spa_mask, cpa_mask;
	u32 link_control;
	int ret = 0;
	u32 syncprd;
	u32 sync_reg;

	mutex_lock(sdw->link_res->shim_lock);

	/*
	 * The hardware relies on an internal counter, typically 4kHz,
	 * to generate the SoundWire SSP - which defines a 'safe'
	 * synchronization point between commands and audio transport
	 * and allows for multi link synchronization. The SYNCPRD value
	 * is only dependent on the oscillator clock provided to
	 * the IP, so adjust based on _DSD properties reported in DSDT
	 * tables. The values reported are based on either 24MHz
	 * (CNL/CML) or 38.4 MHz (ICL/TGL+).
	 */
	if (prop->mclk_freq % 6000000)
		syncprd = SDW_SHIM_SYNC_SYNCPRD_VAL_38_4;
	else
		syncprd = SDW_SHIM_SYNC_SYNCPRD_VAL_24;

	if (!*shim_mask) {
		dev_dbg(sdw->cdns.dev, "%s: powering up all links\n", __func__);

		/* we first need to program the SyncPRD/CPU registers */
		dev_dbg(sdw->cdns.dev,
			"%s: first link up, programming SYNCPRD\n", __func__);

		/* set SyncPRD period */
		sync_reg = intel_readl(shim, SDW_SHIM_SYNC);
		u32p_replace_bits(&sync_reg, syncprd, SDW_SHIM_SYNC_SYNCPRD);

		/* Set SyncCPU bit */
		sync_reg |= SDW_SHIM_SYNC_SYNCCPU;
		intel_writel(shim, SDW_SHIM_SYNC, sync_reg);

		/* Link power up sequence */
		link_control = intel_readl(shim, SDW_SHIM_LCTL);

		/* only power-up enabled links */
		spa_mask = FIELD_PREP(SDW_SHIM_LCTL_SPA_MASK, sdw->link_res->link_mask);
		cpa_mask = FIELD_PREP(SDW_SHIM_LCTL_CPA_MASK, sdw->link_res->link_mask);

		link_control |=  spa_mask;

		ret = intel_set_bit(shim, SDW_SHIM_LCTL, link_control, cpa_mask);
		if (ret < 0) {
			dev_err(sdw->cdns.dev, "Failed to power up link: %d\n", ret);
			goto out;
		}

		/* SyncCPU will change once link is active */
		ret = intel_wait_bit(shim, SDW_SHIM_SYNC,
				     SDW_SHIM_SYNC_SYNCCPU, 0);
		if (ret < 0) {
			dev_err(sdw->cdns.dev,
				"Failed to set SHIM_SYNC: %d\n", ret);
			goto out;
		}
	}

	*shim_mask |= BIT(link_id);

	sdw->cdns.link_up = true;
out:
	mutex_unlock(sdw->link_res->shim_lock);

	return ret;
}

/* this needs to be called with shim_lock */
static void intel_shim_glue_to_master_ip(struct sdw_intel *sdw)
{
	void __iomem *shim = sdw->link_res->shim;
	unsigned int link_id = sdw->instance;
	u16 ioctl;
>>>>>>> upstream/android-13

	/* Switch to MIP from Glue logic */
	ioctl = intel_readw(shim,  SDW_SHIM_IOCTL(link_id));

	ioctl &= ~(SDW_SHIM_IOCTL_DOE);
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);
<<<<<<< HEAD

	ioctl &= ~(SDW_SHIM_IOCTL_DO);
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);

	ioctl |= (SDW_SHIM_IOCTL_MIF);
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);

	ioctl &= ~(SDW_SHIM_IOCTL_BKE);
	ioctl &= ~(SDW_SHIM_IOCTL_COE);

	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);

	act |= 0x1 << SDW_REG_SHIFT(SDW_SHIM_CTMCTL_DOAIS);
	act |= SDW_SHIM_CTMCTL_DACTQE;
	act |= SDW_SHIM_CTMCTL_DODS;
	intel_writew(shim, SDW_SHIM_CTMCTL(link_id), act);

	/* Now set SyncPRD period */
	sync_reg = intel_readl(shim, SDW_SHIM_SYNC);
	sync_reg |= (SDW_SHIM_SYNC_SYNCPRD_VAL <<
			SDW_REG_SHIFT(SDW_SHIM_SYNC_SYNCPRD));

	/* Set SyncCPU bit */
	sync_reg |= SDW_SHIM_SYNC_SYNCCPU;
	ret = intel_clear_bit(shim, SDW_SHIM_SYNC, sync_reg,
				SDW_SHIM_SYNC_SYNCCPU);
	if (ret < 0)
		dev_err(sdw->cdns.dev, "Failed to set sync period: %d", ret);
=======
	usleep_range(10, 15);

	ioctl &= ~(SDW_SHIM_IOCTL_DO);
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);
	usleep_range(10, 15);

	ioctl |= (SDW_SHIM_IOCTL_MIF);
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);
	usleep_range(10, 15);

	ioctl &= ~(SDW_SHIM_IOCTL_BKE);
	ioctl &= ~(SDW_SHIM_IOCTL_COE);
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);
	usleep_range(10, 15);

	/* at this point Master IP has full control of the I/Os */
}

/* this needs to be called with shim_lock */
static void intel_shim_master_ip_to_glue(struct sdw_intel *sdw)
{
	unsigned int link_id = sdw->instance;
	void __iomem *shim = sdw->link_res->shim;
	u16 ioctl;

	/* Glue logic */
	ioctl = intel_readw(shim, SDW_SHIM_IOCTL(link_id));
	ioctl |= SDW_SHIM_IOCTL_BKE;
	ioctl |= SDW_SHIM_IOCTL_COE;
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);
	usleep_range(10, 15);

	ioctl &= ~(SDW_SHIM_IOCTL_MIF);
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);
	usleep_range(10, 15);

	/* at this point Integration Glue has full control of the I/Os */
}

static int intel_shim_init(struct sdw_intel *sdw, bool clock_stop)
{
	void __iomem *shim = sdw->link_res->shim;
	unsigned int link_id = sdw->instance;
	int ret = 0;
	u16 ioctl = 0, act = 0;

	mutex_lock(sdw->link_res->shim_lock);

	/* Initialize Shim */
	ioctl |= SDW_SHIM_IOCTL_BKE;
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);
	usleep_range(10, 15);

	ioctl |= SDW_SHIM_IOCTL_WPDD;
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);
	usleep_range(10, 15);

	ioctl |= SDW_SHIM_IOCTL_DO;
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);
	usleep_range(10, 15);

	ioctl |= SDW_SHIM_IOCTL_DOE;
	intel_writew(shim, SDW_SHIM_IOCTL(link_id), ioctl);
	usleep_range(10, 15);

	intel_shim_glue_to_master_ip(sdw);

	u16p_replace_bits(&act, 0x1, SDW_SHIM_CTMCTL_DOAIS);
	act |= SDW_SHIM_CTMCTL_DACTQE;
	act |= SDW_SHIM_CTMCTL_DODS;
	intel_writew(shim, SDW_SHIM_CTMCTL(link_id), act);
	usleep_range(10, 15);

	mutex_unlock(sdw->link_res->shim_lock);

	return ret;
}

static void intel_shim_wake(struct sdw_intel *sdw, bool wake_enable)
{
	void __iomem *shim = sdw->link_res->shim;
	unsigned int link_id = sdw->instance;
	u16 wake_en, wake_sts;

	mutex_lock(sdw->link_res->shim_lock);
	wake_en = intel_readw(shim, SDW_SHIM_WAKEEN);

	if (wake_enable) {
		/* Enable the wakeup */
		wake_en |= (SDW_SHIM_WAKEEN_ENABLE << link_id);
		intel_writew(shim, SDW_SHIM_WAKEEN, wake_en);
	} else {
		/* Disable the wake up interrupt */
		wake_en &= ~(SDW_SHIM_WAKEEN_ENABLE << link_id);
		intel_writew(shim, SDW_SHIM_WAKEEN, wake_en);

		/* Clear wake status */
		wake_sts = intel_readw(shim, SDW_SHIM_WAKESTS);
		wake_sts |= (SDW_SHIM_WAKESTS_STATUS << link_id);
		intel_writew(shim, SDW_SHIM_WAKESTS, wake_sts);
	}
	mutex_unlock(sdw->link_res->shim_lock);
}

static int intel_link_power_down(struct sdw_intel *sdw)
{
	u32 link_control, spa_mask, cpa_mask;
	unsigned int link_id = sdw->instance;
	void __iomem *shim = sdw->link_res->shim;
	u32 *shim_mask = sdw->link_res->shim_mask;
	int ret = 0;

	mutex_lock(sdw->link_res->shim_lock);

	if (!(*shim_mask & BIT(link_id)))
		dev_err(sdw->cdns.dev,
			"%s: Unbalanced power-up/down calls\n", __func__);

	sdw->cdns.link_up = false;

	intel_shim_master_ip_to_glue(sdw);

	*shim_mask &= ~BIT(link_id);

	if (!*shim_mask) {

		dev_dbg(sdw->cdns.dev, "%s: powering down all links\n", __func__);

		/* Link power down sequence */
		link_control = intel_readl(shim, SDW_SHIM_LCTL);

		/* only power-down enabled links */
		spa_mask = FIELD_PREP(SDW_SHIM_LCTL_SPA_MASK, ~sdw->link_res->link_mask);
		cpa_mask = FIELD_PREP(SDW_SHIM_LCTL_CPA_MASK, sdw->link_res->link_mask);

		link_control &=  spa_mask;

		ret = intel_clear_bit(shim, SDW_SHIM_LCTL, link_control, cpa_mask);
		if (ret < 0) {
			dev_err(sdw->cdns.dev, "%s: could not power down link\n", __func__);

			/*
			 * we leave the sdw->cdns.link_up flag as false since we've disabled
			 * the link at this point and cannot handle interrupts any longer.
			 */
		}
	}

	mutex_unlock(sdw->link_res->shim_lock);

	return ret;
}

static void intel_shim_sync_arm(struct sdw_intel *sdw)
{
	void __iomem *shim = sdw->link_res->shim;
	u32 sync_reg;

	mutex_lock(sdw->link_res->shim_lock);

	/* update SYNC register */
	sync_reg = intel_readl(shim, SDW_SHIM_SYNC);
	sync_reg |= (SDW_SHIM_SYNC_CMDSYNC << sdw->instance);
	intel_writel(shim, SDW_SHIM_SYNC, sync_reg);

	mutex_unlock(sdw->link_res->shim_lock);
}

static int intel_shim_sync_go_unlocked(struct sdw_intel *sdw)
{
	void __iomem *shim = sdw->link_res->shim;
	u32 sync_reg;
	int ret;

	/* Read SYNC register */
	sync_reg = intel_readl(shim, SDW_SHIM_SYNC);

	/*
	 * Set SyncGO bit to synchronously trigger a bank switch for
	 * all the masters. A write to SYNCGO bit clears CMDSYNC bit for all
	 * the Masters.
	 */
	sync_reg |= SDW_SHIM_SYNC_SYNCGO;

	ret = intel_clear_bit(shim, SDW_SHIM_SYNC, sync_reg,
			      SDW_SHIM_SYNC_SYNCGO);

	if (ret < 0)
		dev_err(sdw->cdns.dev, "SyncGO clear failed: %d\n", ret);

	return ret;
}

static int intel_shim_sync_go(struct sdw_intel *sdw)
{
	int ret;

	mutex_lock(sdw->link_res->shim_lock);

	ret = intel_shim_sync_go_unlocked(sdw);

	mutex_unlock(sdw->link_res->shim_lock);
>>>>>>> upstream/android-13

	return ret;
}

/*
 * PDI routines
 */
static void intel_pdi_init(struct sdw_intel *sdw,
<<<<<<< HEAD
			struct sdw_cdns_stream_config *config)
{
	void __iomem *shim = sdw->res->shim;
=======
			   struct sdw_cdns_stream_config *config)
{
	void __iomem *shim = sdw->link_res->shim;
>>>>>>> upstream/android-13
	unsigned int link_id = sdw->instance;
	int pcm_cap, pdm_cap;

	/* PCM Stream Capability */
	pcm_cap = intel_readw(shim, SDW_SHIM_PCMSCAP(link_id));

<<<<<<< HEAD
	config->pcm_bd = (pcm_cap & SDW_SHIM_PCMSCAP_BSS) >>
					SDW_REG_SHIFT(SDW_SHIM_PCMSCAP_BSS);
	config->pcm_in = (pcm_cap & SDW_SHIM_PCMSCAP_ISS) >>
					SDW_REG_SHIFT(SDW_SHIM_PCMSCAP_ISS);
	config->pcm_out = (pcm_cap & SDW_SHIM_PCMSCAP_OSS) >>
					SDW_REG_SHIFT(SDW_SHIM_PCMSCAP_OSS);
=======
	config->pcm_bd = FIELD_GET(SDW_SHIM_PCMSCAP_BSS, pcm_cap);
	config->pcm_in = FIELD_GET(SDW_SHIM_PCMSCAP_ISS, pcm_cap);
	config->pcm_out = FIELD_GET(SDW_SHIM_PCMSCAP_OSS, pcm_cap);

	dev_dbg(sdw->cdns.dev, "PCM cap bd:%d in:%d out:%d\n",
		config->pcm_bd, config->pcm_in, config->pcm_out);
>>>>>>> upstream/android-13

	/* PDM Stream Capability */
	pdm_cap = intel_readw(shim, SDW_SHIM_PDMSCAP(link_id));

<<<<<<< HEAD
	config->pdm_bd = (pdm_cap & SDW_SHIM_PDMSCAP_BSS) >>
					SDW_REG_SHIFT(SDW_SHIM_PDMSCAP_BSS);
	config->pdm_in = (pdm_cap & SDW_SHIM_PDMSCAP_ISS) >>
					SDW_REG_SHIFT(SDW_SHIM_PDMSCAP_ISS);
	config->pdm_out = (pdm_cap & SDW_SHIM_PDMSCAP_OSS) >>
					SDW_REG_SHIFT(SDW_SHIM_PDMSCAP_OSS);
=======
	config->pdm_bd = FIELD_GET(SDW_SHIM_PDMSCAP_BSS, pdm_cap);
	config->pdm_in = FIELD_GET(SDW_SHIM_PDMSCAP_ISS, pdm_cap);
	config->pdm_out = FIELD_GET(SDW_SHIM_PDMSCAP_OSS, pdm_cap);

	dev_dbg(sdw->cdns.dev, "PDM cap bd:%d in:%d out:%d\n",
		config->pdm_bd, config->pdm_in, config->pdm_out);
>>>>>>> upstream/android-13
}

static int
intel_pdi_get_ch_cap(struct sdw_intel *sdw, unsigned int pdi_num, bool pcm)
{
<<<<<<< HEAD
	void __iomem *shim = sdw->res->shim;
=======
	void __iomem *shim = sdw->link_res->shim;
>>>>>>> upstream/android-13
	unsigned int link_id = sdw->instance;
	int count;

	if (pcm) {
		count = intel_readw(shim, SDW_SHIM_PCMSYCHC(link_id, pdi_num));

		/*
		 * WORKAROUND: on all existing Intel controllers, pdi
		 * number 2 reports channel count as 1 even though it
		 * supports 8 channels. Performing hardcoding for pdi
		 * number 2.
		 */
		if (pdi_num == 2)
			count = 7;

	} else {
		count = intel_readw(shim, SDW_SHIM_PDMSCAP(link_id));
<<<<<<< HEAD
		count = ((count & SDW_SHIM_PDMSCAP_CPSS) >>
					SDW_REG_SHIFT(SDW_SHIM_PDMSCAP_CPSS));
=======
		count = FIELD_GET(SDW_SHIM_PDMSCAP_CPSS, count);
>>>>>>> upstream/android-13
	}

	/* zero based values for channel count in register */
	count++;

	return count;
}

static int intel_pdi_get_ch_update(struct sdw_intel *sdw,
<<<<<<< HEAD
				struct sdw_cdns_pdi *pdi,
				unsigned int num_pdi,
				unsigned int *num_ch, bool pcm)
=======
				   struct sdw_cdns_pdi *pdi,
				   unsigned int num_pdi,
				   unsigned int *num_ch, bool pcm)
>>>>>>> upstream/android-13
{
	int i, ch_count = 0;

	for (i = 0; i < num_pdi; i++) {
		pdi->ch_count = intel_pdi_get_ch_cap(sdw, pdi->num, pcm);
		ch_count += pdi->ch_count;
		pdi++;
	}

	*num_ch = ch_count;
	return 0;
}

static int intel_pdi_stream_ch_update(struct sdw_intel *sdw,
<<<<<<< HEAD
				struct sdw_cdns_streams *stream, bool pcm)
{
	intel_pdi_get_ch_update(sdw, stream->bd, stream->num_bd,
			&stream->num_ch_bd, pcm);

	intel_pdi_get_ch_update(sdw, stream->in, stream->num_in,
			&stream->num_ch_in, pcm);

	intel_pdi_get_ch_update(sdw, stream->out, stream->num_out,
			&stream->num_ch_out, pcm);
=======
				      struct sdw_cdns_streams *stream, bool pcm)
{
	intel_pdi_get_ch_update(sdw, stream->bd, stream->num_bd,
				&stream->num_ch_bd, pcm);

	intel_pdi_get_ch_update(sdw, stream->in, stream->num_in,
				&stream->num_ch_in, pcm);

	intel_pdi_get_ch_update(sdw, stream->out, stream->num_out,
				&stream->num_ch_out, pcm);
>>>>>>> upstream/android-13

	return 0;
}

static int intel_pdi_ch_update(struct sdw_intel *sdw)
{
	/* First update PCM streams followed by PDM streams */
	intel_pdi_stream_ch_update(sdw, &sdw->cdns.pcm, true);
	intel_pdi_stream_ch_update(sdw, &sdw->cdns.pdm, false);

	return 0;
}

static void
intel_pdi_shim_configure(struct sdw_intel *sdw, struct sdw_cdns_pdi *pdi)
{
<<<<<<< HEAD
	void __iomem *shim = sdw->res->shim;
=======
	void __iomem *shim = sdw->link_res->shim;
>>>>>>> upstream/android-13
	unsigned int link_id = sdw->instance;
	int pdi_conf = 0;

	/* the Bulk and PCM streams are not contiguous */
	pdi->intel_alh_id = (link_id * 16) + pdi->num + 3;
	if (pdi->num >= 2)
		pdi->intel_alh_id += 2;

	/*
	 * Program stream parameters to stream SHIM register
	 * This is applicable for PCM stream only.
	 */
	if (pdi->type != SDW_STREAM_PCM)
		return;

	if (pdi->dir == SDW_DATA_DIR_RX)
		pdi_conf |= SDW_SHIM_PCMSYCM_DIR;
	else
		pdi_conf &= ~(SDW_SHIM_PCMSYCM_DIR);

<<<<<<< HEAD
	pdi_conf |= (pdi->intel_alh_id <<
			SDW_REG_SHIFT(SDW_SHIM_PCMSYCM_STREAM));
	pdi_conf |= (pdi->l_ch_num << SDW_REG_SHIFT(SDW_SHIM_PCMSYCM_LCHN));
	pdi_conf |= (pdi->h_ch_num << SDW_REG_SHIFT(SDW_SHIM_PCMSYCM_HCHN));
=======
	u32p_replace_bits(&pdi_conf, pdi->intel_alh_id, SDW_SHIM_PCMSYCM_STREAM);
	u32p_replace_bits(&pdi_conf, pdi->l_ch_num, SDW_SHIM_PCMSYCM_LCHN);
	u32p_replace_bits(&pdi_conf, pdi->h_ch_num, SDW_SHIM_PCMSYCM_HCHN);
>>>>>>> upstream/android-13

	intel_writew(shim, SDW_SHIM_PCMSYCHM(link_id, pdi->num), pdi_conf);
}

static void
intel_pdi_alh_configure(struct sdw_intel *sdw, struct sdw_cdns_pdi *pdi)
{
<<<<<<< HEAD
	void __iomem *alh = sdw->res->alh;
=======
	void __iomem *alh = sdw->link_res->alh;
>>>>>>> upstream/android-13
	unsigned int link_id = sdw->instance;
	unsigned int conf;

	/* the Bulk and PCM streams are not contiguous */
	pdi->intel_alh_id = (link_id * 16) + pdi->num + 3;
	if (pdi->num >= 2)
		pdi->intel_alh_id += 2;

	/* Program Stream config ALH register */
	conf = intel_readl(alh, SDW_ALH_STRMZCFG(pdi->intel_alh_id));

<<<<<<< HEAD
	conf |= (SDW_ALH_STRMZCFG_DMAT_VAL <<
			SDW_REG_SHIFT(SDW_ALH_STRMZCFG_DMAT));

	conf |= ((pdi->ch_count - 1) <<
			SDW_REG_SHIFT(SDW_ALH_STRMZCFG_CHN));
=======
	u32p_replace_bits(&conf, SDW_ALH_STRMZCFG_DMAT_VAL, SDW_ALH_STRMZCFG_DMAT);
	u32p_replace_bits(&conf, pdi->ch_count - 1, SDW_ALH_STRMZCFG_CHN);
>>>>>>> upstream/android-13

	intel_writel(alh, SDW_ALH_STRMZCFG(pdi->intel_alh_id), conf);
}

<<<<<<< HEAD
static int intel_config_stream(struct sdw_intel *sdw,
			struct snd_pcm_substream *substream,
			struct snd_soc_dai *dai,
			struct snd_pcm_hw_params *hw_params, int link_id)
{
	if (sdw->res->ops && sdw->res->ops->config_stream)
		return sdw->res->ops->config_stream(sdw->res->arg,
				substream, dai, hw_params, link_id);

	return -EIO;
}

=======
static int intel_params_stream(struct sdw_intel *sdw,
			       struct snd_pcm_substream *substream,
			       struct snd_soc_dai *dai,
			       struct snd_pcm_hw_params *hw_params,
			       int link_id, int alh_stream_id)
{
	struct sdw_intel_link_res *res = sdw->link_res;
	struct sdw_intel_stream_params_data params_data;

	params_data.substream = substream;
	params_data.dai = dai;
	params_data.hw_params = hw_params;
	params_data.link_id = link_id;
	params_data.alh_stream_id = alh_stream_id;

	if (res->ops && res->ops->params_stream && res->dev)
		return res->ops->params_stream(res->dev,
					       &params_data);
	return -EIO;
}

static int intel_free_stream(struct sdw_intel *sdw,
			     struct snd_pcm_substream *substream,
			     struct snd_soc_dai *dai,
			     int link_id)
{
	struct sdw_intel_link_res *res = sdw->link_res;
	struct sdw_intel_stream_free_data free_data;

	free_data.substream = substream;
	free_data.dai = dai;
	free_data.link_id = link_id;

	if (res->ops && res->ops->free_stream && res->dev)
		return res->ops->free_stream(res->dev,
					     &free_data);

	return 0;
}

/*
 * bank switch routines
 */

static int intel_pre_bank_switch(struct sdw_bus *bus)
{
	struct sdw_cdns *cdns = bus_to_cdns(bus);
	struct sdw_intel *sdw = cdns_to_intel(cdns);

	/* Write to register only for multi-link */
	if (!bus->multi_link)
		return 0;

	intel_shim_sync_arm(sdw);

	return 0;
}

static int intel_post_bank_switch(struct sdw_bus *bus)
{
	struct sdw_cdns *cdns = bus_to_cdns(bus);
	struct sdw_intel *sdw = cdns_to_intel(cdns);
	void __iomem *shim = sdw->link_res->shim;
	int sync_reg, ret;

	/* Write to register only for multi-link */
	if (!bus->multi_link)
		return 0;

	mutex_lock(sdw->link_res->shim_lock);

	/* Read SYNC register */
	sync_reg = intel_readl(shim, SDW_SHIM_SYNC);

	/*
	 * post_bank_switch() ops is called from the bus in loop for
	 * all the Masters in the steam with the expectation that
	 * we trigger the bankswitch for the only first Master in the list
	 * and do nothing for the other Masters
	 *
	 * So, set the SYNCGO bit only if CMDSYNC bit is set for any Master.
	 */
	if (!(sync_reg & SDW_SHIM_SYNC_CMDSYNC_MASK)) {
		ret = 0;
		goto unlock;
	}

	ret = intel_shim_sync_go_unlocked(sdw);
unlock:
	mutex_unlock(sdw->link_res->shim_lock);

	if (ret < 0)
		dev_err(sdw->cdns.dev, "Post bank switch failed: %d\n", ret);

	return ret;
}

>>>>>>> upstream/android-13
/*
 * DAI routines
 */

<<<<<<< HEAD
static struct sdw_cdns_port *intel_alloc_port(struct sdw_intel *sdw,
				u32 ch, u32 dir, bool pcm)
{
	struct sdw_cdns *cdns = &sdw->cdns;
	struct sdw_cdns_port *port = NULL;
	int i, ret = 0;

	for (i = 0; i < cdns->num_ports; i++) {
		if (cdns->ports[i].assigned == true)
			continue;

		port = &cdns->ports[i];
		port->assigned = true;
		port->direction = dir;
		port->ch = ch;
		break;
	}

	if (!port) {
		dev_err(cdns->dev, "Unable to find a free port\n");
		return NULL;
	}

	if (pcm) {
		ret = sdw_cdns_alloc_stream(cdns, &cdns->pcm, port, ch, dir);
		if (ret)
			goto out;

		intel_pdi_shim_configure(sdw, port->pdi);
		sdw_cdns_config_stream(cdns, port, ch, dir, port->pdi);

		intel_pdi_alh_configure(sdw, port->pdi);

	} else {
		ret = sdw_cdns_alloc_stream(cdns, &cdns->pdm, port, ch, dir);
	}

out:
	if (ret) {
		port->assigned = false;
		port = NULL;
	}

	return port;
}

static void intel_port_cleanup(struct sdw_cdns_dma_data *dma)
{
	int i;

	for (i = 0; i < dma->nr_ports; i++) {
		if (dma->port[i]) {
			dma->port[i]->pdi->assigned = false;
			dma->port[i]->pdi = NULL;
			dma->port[i]->assigned = false;
			dma->port[i] = NULL;
		}
	}
}

static int intel_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params,
				struct snd_soc_dai *dai)
=======
static int intel_startup(struct snd_pcm_substream *substream,
			 struct snd_soc_dai *dai)
{
	struct sdw_cdns *cdns = snd_soc_dai_get_drvdata(dai);
	int ret;

	ret = pm_runtime_get_sync(cdns->dev);
	if (ret < 0 && ret != -EACCES) {
		dev_err_ratelimited(cdns->dev,
				    "pm_runtime_get_sync failed in %s, ret %d\n",
				    __func__, ret);
		pm_runtime_put_noidle(cdns->dev);
		return ret;
	}
	return 0;
}

static int intel_hw_params(struct snd_pcm_substream *substream,
			   struct snd_pcm_hw_params *params,
			   struct snd_soc_dai *dai)
>>>>>>> upstream/android-13
{
	struct sdw_cdns *cdns = snd_soc_dai_get_drvdata(dai);
	struct sdw_intel *sdw = cdns_to_intel(cdns);
	struct sdw_cdns_dma_data *dma;
<<<<<<< HEAD
	struct sdw_stream_config sconfig;
	struct sdw_port_config *pconfig;
	int ret, i, ch, dir;
=======
	struct sdw_cdns_pdi *pdi;
	struct sdw_stream_config sconfig;
	struct sdw_port_config *pconfig;
	int ch, dir;
	int ret;
>>>>>>> upstream/android-13
	bool pcm = true;

	dma = snd_soc_dai_get_dma_data(dai, substream);
	if (!dma)
		return -EIO;

	ch = params_channels(params);
	if (substream->stream == SNDRV_PCM_STREAM_CAPTURE)
		dir = SDW_DATA_DIR_RX;
	else
		dir = SDW_DATA_DIR_TX;

<<<<<<< HEAD
	if (dma->stream_type == SDW_STREAM_PDM) {
		/* TODO: Check whether PDM decimator is already in use */
		dma->nr_ports = sdw_cdns_get_stream(cdns, &cdns->pdm, ch, dir);
		pcm = false;
	} else {
		dma->nr_ports = sdw_cdns_get_stream(cdns, &cdns->pcm, ch, dir);
	}

	if (!dma->nr_ports) {
		dev_err(dai->dev, "ports/resources not available");
		return -EINVAL;
	}

	dma->port = kcalloc(dma->nr_ports, sizeof(*dma->port), GFP_KERNEL);
	if (!dma->port)
		return -ENOMEM;

	for (i = 0; i < dma->nr_ports; i++) {
		dma->port[i] = intel_alloc_port(sdw, ch, dir, pcm);
		if (!dma->port[i]) {
			ret = -EINVAL;
			goto port_error;
		}
	}

	/* Inform DSP about PDI stream number */
	for (i = 0; i < dma->nr_ports; i++) {
		ret = intel_config_stream(sdw, substream, dai, params,
				dma->port[i]->pdi->intel_alh_id);
		if (ret)
			goto port_error;
	}
=======
	if (dma->stream_type == SDW_STREAM_PDM)
		pcm = false;

	if (pcm)
		pdi = sdw_cdns_alloc_pdi(cdns, &cdns->pcm, ch, dir, dai->id);
	else
		pdi = sdw_cdns_alloc_pdi(cdns, &cdns->pdm, ch, dir, dai->id);

	if (!pdi) {
		ret = -EINVAL;
		goto error;
	}

	/* do run-time configurations for SHIM, ALH and PDI/PORT */
	intel_pdi_shim_configure(sdw, pdi);
	intel_pdi_alh_configure(sdw, pdi);
	sdw_cdns_config_stream(cdns, ch, dir, pdi);

	/* store pdi and hw_params, may be needed in prepare step */
	dma->suspended = false;
	dma->pdi = pdi;
	dma->hw_params = params;

	/* Inform DSP about PDI stream number */
	ret = intel_params_stream(sdw, substream, dai, params,
				  sdw->instance,
				  pdi->intel_alh_id);
	if (ret)
		goto error;
>>>>>>> upstream/android-13

	sconfig.direction = dir;
	sconfig.ch_count = ch;
	sconfig.frame_rate = params_rate(params);
	sconfig.type = dma->stream_type;

	if (dma->stream_type == SDW_STREAM_PDM) {
		sconfig.frame_rate *= 50;
		sconfig.bps = 1;
	} else {
		sconfig.bps = snd_pcm_format_width(params_format(params));
	}

	/* Port configuration */
<<<<<<< HEAD
	pconfig = kcalloc(dma->nr_ports, sizeof(*pconfig), GFP_KERNEL);
	if (!pconfig) {
		ret =  -ENOMEM;
		goto port_error;
	}

	for (i = 0; i < dma->nr_ports; i++) {
		pconfig[i].num = dma->port[i]->num;
		pconfig[i].ch_mask = (1 << ch) - 1;
	}

	ret = sdw_stream_add_master(&cdns->bus, &sconfig,
				pconfig, dma->nr_ports, dma->stream);
	if (ret) {
		dev_err(cdns->dev, "add master to stream failed:%d", ret);
		goto stream_error;
	}

	kfree(pconfig);
	return ret;

stream_error:
	kfree(pconfig);
port_error:
	intel_port_cleanup(dma);
	kfree(dma->port);
=======
	pconfig = kzalloc(sizeof(*pconfig), GFP_KERNEL);
	if (!pconfig) {
		ret =  -ENOMEM;
		goto error;
	}

	pconfig->num = pdi->num;
	pconfig->ch_mask = (1 << ch) - 1;

	ret = sdw_stream_add_master(&cdns->bus, &sconfig,
				    pconfig, 1, dma->stream);
	if (ret)
		dev_err(cdns->dev, "add master to stream failed:%d\n", ret);

	kfree(pconfig);
error:
	return ret;
}

static int intel_prepare(struct snd_pcm_substream *substream,
			 struct snd_soc_dai *dai)
{
	struct sdw_cdns *cdns = snd_soc_dai_get_drvdata(dai);
	struct sdw_intel *sdw = cdns_to_intel(cdns);
	struct sdw_cdns_dma_data *dma;
	int ch, dir;
	int ret = 0;

	dma = snd_soc_dai_get_dma_data(dai, substream);
	if (!dma) {
		dev_err(dai->dev, "failed to get dma data in %s\n",
			__func__);
		return -EIO;
	}

	if (dma->suspended) {
		dma->suspended = false;

		/*
		 * .prepare() is called after system resume, where we
		 * need to reinitialize the SHIM/ALH/Cadence IP.
		 * .prepare() is also called to deal with underflows,
		 * but in those cases we cannot touch ALH/SHIM
		 * registers
		 */

		/* configure stream */
		ch = params_channels(dma->hw_params);
		if (substream->stream == SNDRV_PCM_STREAM_CAPTURE)
			dir = SDW_DATA_DIR_RX;
		else
			dir = SDW_DATA_DIR_TX;

		intel_pdi_shim_configure(sdw, dma->pdi);
		intel_pdi_alh_configure(sdw, dma->pdi);
		sdw_cdns_config_stream(cdns, ch, dir, dma->pdi);

		/* Inform DSP about PDI stream number */
		ret = intel_params_stream(sdw, substream, dai,
					  dma->hw_params,
					  sdw->instance,
					  dma->pdi->intel_alh_id);
	}

>>>>>>> upstream/android-13
	return ret;
}

static int
intel_hw_free(struct snd_pcm_substream *substream, struct snd_soc_dai *dai)
{
	struct sdw_cdns *cdns = snd_soc_dai_get_drvdata(dai);
<<<<<<< HEAD
=======
	struct sdw_intel *sdw = cdns_to_intel(cdns);
>>>>>>> upstream/android-13
	struct sdw_cdns_dma_data *dma;
	int ret;

	dma = snd_soc_dai_get_dma_data(dai, substream);
	if (!dma)
		return -EIO;

<<<<<<< HEAD
	ret = sdw_stream_remove_master(&cdns->bus, dma->stream);
	if (ret < 0)
		dev_err(dai->dev, "remove master from stream %s failed: %d",
							dma->stream->name, ret);

	intel_port_cleanup(dma);
	kfree(dma->port);
	return ret;
}

static int intel_pcm_set_sdw_stream(struct snd_soc_dai *dai,
					void *stream, int direction)
=======
	/*
	 * The sdw stream state will transition to RELEASED when stream->
	 * master_list is empty. So the stream state will transition to
	 * DEPREPARED for the first cpu-dai and to RELEASED for the last
	 * cpu-dai.
	 */
	ret = sdw_stream_remove_master(&cdns->bus, dma->stream);
	if (ret < 0) {
		dev_err(dai->dev, "remove master from stream %s failed: %d\n",
			dma->stream->name, ret);
		return ret;
	}

	ret = intel_free_stream(sdw, substream, dai, sdw->instance);
	if (ret < 0) {
		dev_err(dai->dev, "intel_free_stream: failed %d\n", ret);
		return ret;
	}

	dma->hw_params = NULL;
	dma->pdi = NULL;

	return 0;
}

static void intel_shutdown(struct snd_pcm_substream *substream,
			   struct snd_soc_dai *dai)
{
	struct sdw_cdns *cdns = snd_soc_dai_get_drvdata(dai);

	pm_runtime_mark_last_busy(cdns->dev);
	pm_runtime_put_autosuspend(cdns->dev);
}

static int intel_component_dais_suspend(struct snd_soc_component *component)
{
	struct sdw_cdns_dma_data *dma;
	struct snd_soc_dai *dai;

	for_each_component_dais(component, dai) {
		/*
		 * we don't have a .suspend dai_ops, and we don't have access
		 * to the substream, so let's mark both capture and playback
		 * DMA contexts as suspended
		 */
		dma = dai->playback_dma_data;
		if (dma)
			dma->suspended = true;

		dma = dai->capture_dma_data;
		if (dma)
			dma->suspended = true;
	}

	return 0;
}

static int intel_pcm_set_sdw_stream(struct snd_soc_dai *dai,
				    void *stream, int direction)
>>>>>>> upstream/android-13
{
	return cdns_set_sdw_stream(dai, stream, true, direction);
}

static int intel_pdm_set_sdw_stream(struct snd_soc_dai *dai,
<<<<<<< HEAD
					void *stream, int direction)
=======
				    void *stream, int direction)
>>>>>>> upstream/android-13
{
	return cdns_set_sdw_stream(dai, stream, false, direction);
}

<<<<<<< HEAD
static struct snd_soc_dai_ops intel_pcm_dai_ops = {
	.hw_params = intel_hw_params,
	.hw_free = intel_hw_free,
	.shutdown = sdw_cdns_shutdown,
	.set_sdw_stream = intel_pcm_set_sdw_stream,
};

static struct snd_soc_dai_ops intel_pdm_dai_ops = {
	.hw_params = intel_hw_params,
	.hw_free = intel_hw_free,
	.shutdown = sdw_cdns_shutdown,
	.set_sdw_stream = intel_pdm_set_sdw_stream,
=======
static void *intel_get_sdw_stream(struct snd_soc_dai *dai,
				  int direction)
{
	struct sdw_cdns_dma_data *dma;

	if (direction == SNDRV_PCM_STREAM_PLAYBACK)
		dma = dai->playback_dma_data;
	else
		dma = dai->capture_dma_data;

	if (!dma)
		return ERR_PTR(-EINVAL);

	return dma->stream;
}

static const struct snd_soc_dai_ops intel_pcm_dai_ops = {
	.startup = intel_startup,
	.hw_params = intel_hw_params,
	.prepare = intel_prepare,
	.hw_free = intel_hw_free,
	.shutdown = intel_shutdown,
	.set_sdw_stream = intel_pcm_set_sdw_stream,
	.get_sdw_stream = intel_get_sdw_stream,
};

static const struct snd_soc_dai_ops intel_pdm_dai_ops = {
	.startup = intel_startup,
	.hw_params = intel_hw_params,
	.prepare = intel_prepare,
	.hw_free = intel_hw_free,
	.shutdown = intel_shutdown,
	.set_sdw_stream = intel_pdm_set_sdw_stream,
	.get_sdw_stream = intel_get_sdw_stream,
>>>>>>> upstream/android-13
};

static const struct snd_soc_component_driver dai_component = {
	.name           = "soundwire",
<<<<<<< HEAD
};

static int intel_create_dai(struct sdw_cdns *cdns,
			struct snd_soc_dai_driver *dais,
			enum intel_pdi_type type,
			u32 num, u32 off, u32 max_ch, bool pcm)
=======
	.suspend	= intel_component_dais_suspend
};

static int intel_create_dai(struct sdw_cdns *cdns,
			    struct snd_soc_dai_driver *dais,
			    enum intel_pdi_type type,
			    u32 num, u32 off, u32 max_ch, bool pcm)
>>>>>>> upstream/android-13
{
	int i;

	if (num == 0)
		return 0;

	 /* TODO: Read supported rates/formats from hardware */
	for (i = off; i < (off + num); i++) {
<<<<<<< HEAD
		dais[i].name = kasprintf(GFP_KERNEL, "SDW%d Pin%d",
					cdns->instance, i);
=======
		dais[i].name = devm_kasprintf(cdns->dev, GFP_KERNEL,
					      "SDW%d Pin%d",
					      cdns->instance, i);
>>>>>>> upstream/android-13
		if (!dais[i].name)
			return -ENOMEM;

		if (type == INTEL_PDI_BD || type == INTEL_PDI_OUT) {
<<<<<<< HEAD
			dais[i].playback.stream_name = kasprintf(GFP_KERNEL,
							"SDW%d Tx%d",
							cdns->instance, i);
			if (!dais[i].playback.stream_name) {
				kfree(dais[i].name);
				return -ENOMEM;
			}

=======
>>>>>>> upstream/android-13
			dais[i].playback.channels_min = 1;
			dais[i].playback.channels_max = max_ch;
			dais[i].playback.rates = SNDRV_PCM_RATE_48000;
			dais[i].playback.formats = SNDRV_PCM_FMTBIT_S16_LE;
		}

		if (type == INTEL_PDI_BD || type == INTEL_PDI_IN) {
<<<<<<< HEAD
			dais[i].capture.stream_name = kasprintf(GFP_KERNEL,
							"SDW%d Rx%d",
							cdns->instance, i);
			if (!dais[i].capture.stream_name) {
				kfree(dais[i].name);
				kfree(dais[i].playback.stream_name);
				return -ENOMEM;
			}

=======
>>>>>>> upstream/android-13
			dais[i].capture.channels_min = 1;
			dais[i].capture.channels_max = max_ch;
			dais[i].capture.rates = SNDRV_PCM_RATE_48000;
			dais[i].capture.formats = SNDRV_PCM_FMTBIT_S16_LE;
		}

<<<<<<< HEAD
		dais[i].id = SDW_DAI_ID_RANGE_START + i;

=======
>>>>>>> upstream/android-13
		if (pcm)
			dais[i].ops = &intel_pcm_dai_ops;
		else
			dais[i].ops = &intel_pdm_dai_ops;
	}

	return 0;
}

static int intel_register_dai(struct sdw_intel *sdw)
{
	struct sdw_cdns *cdns = &sdw->cdns;
	struct sdw_cdns_streams *stream;
	struct snd_soc_dai_driver *dais;
	int num_dai, ret, off = 0;

	/* DAIs are created based on total number of PDIs supported */
	num_dai = cdns->pcm.num_pdi + cdns->pdm.num_pdi;

	dais = devm_kcalloc(cdns->dev, num_dai, sizeof(*dais), GFP_KERNEL);
	if (!dais)
		return -ENOMEM;

	/* Create PCM DAIs */
	stream = &cdns->pcm;

<<<<<<< HEAD
	ret = intel_create_dai(cdns, dais, INTEL_PDI_IN,
			stream->num_in, off, stream->num_ch_in, true);
=======
	ret = intel_create_dai(cdns, dais, INTEL_PDI_IN, cdns->pcm.num_in,
			       off, stream->num_ch_in, true);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	off += cdns->pcm.num_in;
<<<<<<< HEAD
	ret = intel_create_dai(cdns, dais, INTEL_PDI_OUT,
			cdns->pcm.num_out, off, stream->num_ch_out, true);
=======
	ret = intel_create_dai(cdns, dais, INTEL_PDI_OUT, cdns->pcm.num_out,
			       off, stream->num_ch_out, true);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	off += cdns->pcm.num_out;
<<<<<<< HEAD
	ret = intel_create_dai(cdns, dais, INTEL_PDI_BD,
			cdns->pcm.num_bd, off, stream->num_ch_bd, true);
=======
	ret = intel_create_dai(cdns, dais, INTEL_PDI_BD, cdns->pcm.num_bd,
			       off, stream->num_ch_bd, true);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	/* Create PDM DAIs */
	stream = &cdns->pdm;
	off += cdns->pcm.num_bd;
<<<<<<< HEAD
	ret = intel_create_dai(cdns, dais, INTEL_PDI_IN,
			cdns->pdm.num_in, off, stream->num_ch_in, false);
=======
	ret = intel_create_dai(cdns, dais, INTEL_PDI_IN, cdns->pdm.num_in,
			       off, stream->num_ch_in, false);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	off += cdns->pdm.num_in;
<<<<<<< HEAD
	ret = intel_create_dai(cdns, dais, INTEL_PDI_OUT,
			cdns->pdm.num_out, off, stream->num_ch_out, false);
	if (ret)
		return ret;

	off += cdns->pdm.num_bd;
	ret = intel_create_dai(cdns, dais, INTEL_PDI_BD,
			cdns->pdm.num_bd, off, stream->num_ch_bd, false);
=======
	ret = intel_create_dai(cdns, dais, INTEL_PDI_OUT, cdns->pdm.num_out,
			       off, stream->num_ch_out, false);
	if (ret)
		return ret;

	off += cdns->pdm.num_out;
	ret = intel_create_dai(cdns, dais, INTEL_PDI_BD, cdns->pdm.num_bd,
			       off, stream->num_ch_bd, false);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	return snd_soc_register_component(cdns->dev, &dai_component,
<<<<<<< HEAD
				dais, num_dai);
=======
					  dais, num_dai);
}

static int sdw_master_read_intel_prop(struct sdw_bus *bus)
{
	struct sdw_master_prop *prop = &bus->prop;
	struct fwnode_handle *link;
	char name[32];
	u32 quirk_mask;

	/* Find master handle */
	snprintf(name, sizeof(name),
		 "mipi-sdw-link-%d-subproperties", bus->link_id);

	link = device_get_named_child_node(bus->dev, name);
	if (!link) {
		dev_err(bus->dev, "Master node %s not found\n", name);
		return -EIO;
	}

	fwnode_property_read_u32(link,
				 "intel-sdw-ip-clock",
				 &prop->mclk_freq);

	/* the values reported by BIOS are the 2x clock, not the bus clock */
	prop->mclk_freq /= 2;

	fwnode_property_read_u32(link,
				 "intel-quirk-mask",
				 &quirk_mask);

	if (quirk_mask & SDW_INTEL_QUIRK_MASK_BUS_DISABLE)
		prop->hw_disabled = true;

	prop->quirks = SDW_MASTER_QUIRKS_CLEAR_INITIAL_CLASH |
		SDW_MASTER_QUIRKS_CLEAR_INITIAL_PARITY;

	return 0;
>>>>>>> upstream/android-13
}

static int intel_prop_read(struct sdw_bus *bus)
{
	/* Initialize with default handler to read all DisCo properties */
	sdw_master_read_prop(bus);

<<<<<<< HEAD
	/* BIOS is not giving some values correctly. So, lets override them */
	bus->prop.num_freq = 1;
	bus->prop.freq = devm_kcalloc(bus->dev, sizeof(*bus->prop.freq),
					bus->prop.num_freq, GFP_KERNEL);
	if (!bus->prop.freq)
		return -ENOMEM;

	bus->prop.freq[0] = bus->prop.max_freq;
	bus->prop.err_threshold = 5;
=======
	/* read Intel-specific properties */
	sdw_master_read_intel_prop(bus);
>>>>>>> upstream/android-13

	return 0;
}

static struct sdw_master_ops sdw_intel_ops = {
	.read_prop = sdw_master_read_prop,
<<<<<<< HEAD
=======
	.override_adr = sdw_dmi_override_adr,
>>>>>>> upstream/android-13
	.xfer_msg = cdns_xfer_msg,
	.xfer_msg_defer = cdns_xfer_msg_defer,
	.reset_page_addr = cdns_reset_page_addr,
	.set_bus_conf = cdns_bus_conf,
<<<<<<< HEAD
};

/*
 * probe and init
 */
static int intel_probe(struct platform_device *pdev)
{
	struct sdw_cdns_stream_config config;
	struct sdw_intel *sdw;
	int ret;

	sdw = devm_kzalloc(&pdev->dev, sizeof(*sdw), GFP_KERNEL);
	if (!sdw)
		return -ENOMEM;

	sdw->instance = pdev->id;
	sdw->res = dev_get_platdata(&pdev->dev);
	sdw->cdns.dev = &pdev->dev;
	sdw->cdns.registers = sdw->res->registers;
	sdw->cdns.instance = sdw->instance;
	sdw->cdns.msg_count = 0;
	sdw->cdns.bus.dev = &pdev->dev;
	sdw->cdns.bus.link_id = pdev->id;

	sdw_cdns_probe(&sdw->cdns);

	/* Set property read ops */
	sdw_intel_ops.read_prop = intel_prop_read;
	sdw->cdns.bus.ops = &sdw_intel_ops;

	sdw_intel_ops.read_prop = intel_prop_read;
	sdw->cdns.bus.ops = &sdw_intel_ops;

	platform_set_drvdata(pdev, sdw);

	ret = sdw_add_bus_master(&sdw->cdns.bus);
	if (ret) {
		dev_err(&pdev->dev, "sdw_add_bus_master fail: %d\n", ret);
		goto err_master_reg;
	}

	/* Initialize shim and controller */
	intel_link_power_up(sdw);
	intel_shim_init(sdw);

	ret = sdw_cdns_init(&sdw->cdns);
	if (ret)
		goto err_init;

	ret = sdw_cdns_enable_interrupt(&sdw->cdns);

	/* Read the PDI config and initialize cadence PDI */
	intel_pdi_init(sdw, &config);
	ret = sdw_cdns_pdi_init(&sdw->cdns, config);
=======
	.pre_bank_switch = intel_pre_bank_switch,
	.post_bank_switch = intel_post_bank_switch,
};

static int intel_init(struct sdw_intel *sdw)
{
	bool clock_stop;

	/* Initialize shim and controller */
	intel_link_power_up(sdw);

	clock_stop = sdw_cdns_is_clock_stop(&sdw->cdns);

	intel_shim_init(sdw, clock_stop);

	return 0;
}

/*
 * probe and init (aux_dev_id argument is required by function prototype but not used)
 */
static int intel_link_probe(struct auxiliary_device *auxdev,
			    const struct auxiliary_device_id *aux_dev_id)

{
	struct device *dev = &auxdev->dev;
	struct sdw_intel_link_dev *ldev = auxiliary_dev_to_sdw_intel_link_dev(auxdev);
	struct sdw_intel *sdw;
	struct sdw_cdns *cdns;
	struct sdw_bus *bus;
	int ret;

	sdw = devm_kzalloc(dev, sizeof(*sdw), GFP_KERNEL);
	if (!sdw)
		return -ENOMEM;

	cdns = &sdw->cdns;
	bus = &cdns->bus;

	sdw->instance = auxdev->id;
	sdw->link_res = &ldev->link_res;
	cdns->dev = dev;
	cdns->registers = sdw->link_res->registers;
	cdns->instance = sdw->instance;
	cdns->msg_count = 0;

	bus->link_id = auxdev->id;

	sdw_cdns_probe(cdns);

	/* Set property read ops */
	sdw_intel_ops.read_prop = intel_prop_read;
	bus->ops = &sdw_intel_ops;

	/* set driver data, accessed by snd_soc_dai_get_drvdata() */
	dev_set_drvdata(dev, cdns);

	/* use generic bandwidth allocation algorithm */
	sdw->cdns.bus.compute_params = sdw_compute_params;

	ret = sdw_bus_master_add(bus, dev, dev->fwnode);
	if (ret) {
		dev_err(dev, "sdw_bus_master_add fail: %d\n", ret);
		return ret;
	}

	if (bus->prop.hw_disabled)
		dev_info(dev,
			 "SoundWire master %d is disabled, will be ignored\n",
			 bus->link_id);
	/*
	 * Ignore BIOS err_threshold, it's a really bad idea when dealing
	 * with multiple hardware synchronized links
	 */
	bus->prop.err_threshold = 0;

	return 0;
}

int intel_link_startup(struct auxiliary_device *auxdev)
{
	struct sdw_cdns_stream_config config;
	struct device *dev = &auxdev->dev;
	struct sdw_cdns *cdns = dev_get_drvdata(dev);
	struct sdw_intel *sdw = cdns_to_intel(cdns);
	struct sdw_bus *bus = &cdns->bus;
	int link_flags;
	bool multi_link;
	u32 clock_stop_quirks;
	int ret;

	if (bus->prop.hw_disabled) {
		dev_info(dev,
			 "SoundWire master %d is disabled, ignoring\n",
			 sdw->instance);
		return 0;
	}

	link_flags = md_flags >> (bus->link_id * 8);
	multi_link = !(link_flags & SDW_INTEL_MASTER_DISABLE_MULTI_LINK);
	if (!multi_link) {
		dev_dbg(dev, "Multi-link is disabled\n");
		bus->multi_link = false;
	} else {
		/*
		 * hardware-based synchronization is required regardless
		 * of the number of segments used by a stream: SSP-based
		 * synchronization is gated by gsync when the multi-master
		 * mode is set.
		 */
		bus->multi_link = true;
		bus->hw_sync_min_links = 1;
	}

	/* Initialize shim, controller */
	ret = intel_init(sdw);
	if (ret)
		goto err_init;

	/* Read the PDI config and initialize cadence PDI */
	intel_pdi_init(sdw, &config);
	ret = sdw_cdns_pdi_init(cdns, config);
>>>>>>> upstream/android-13
	if (ret)
		goto err_init;

	intel_pdi_ch_update(sdw);

<<<<<<< HEAD
	/* Acquire IRQ */
	ret = request_threaded_irq(sdw->res->irq, sdw_cdns_irq,
			sdw_cdns_thread, IRQF_SHARED, KBUILD_MODNAME,
			&sdw->cdns);
	if (ret < 0) {
		dev_err(sdw->cdns.dev, "unable to grab IRQ %d, disabling device\n",
				sdw->res->irq);
		goto err_init;
	}

	/* Register DAIs */
	ret = intel_register_dai(sdw);
	if (ret) {
		dev_err(sdw->cdns.dev, "DAI registration failed: %d", ret);
		snd_soc_unregister_component(sdw->cdns.dev);
		goto err_dai;
	}

	return 0;

err_dai:
	free_irq(sdw->res->irq, sdw);
err_init:
	sdw_delete_bus_master(&sdw->cdns.bus);
err_master_reg:
	return ret;
}

static int intel_remove(struct platform_device *pdev)
{
	struct sdw_intel *sdw;

	sdw = platform_get_drvdata(pdev);

	free_irq(sdw->res->irq, sdw);
	snd_soc_unregister_component(sdw->cdns.dev);
	sdw_delete_bus_master(&sdw->cdns.bus);
=======
	ret = sdw_cdns_enable_interrupt(cdns, true);
	if (ret < 0) {
		dev_err(dev, "cannot enable interrupts\n");
		goto err_init;
	}

	/*
	 * follow recommended programming flows to avoid timeouts when
	 * gsync is enabled
	 */
	if (multi_link)
		intel_shim_sync_arm(sdw);

	ret = sdw_cdns_init(cdns);
	if (ret < 0) {
		dev_err(dev, "unable to initialize Cadence IP\n");
		goto err_interrupt;
	}

	ret = sdw_cdns_exit_reset(cdns);
	if (ret < 0) {
		dev_err(dev, "unable to exit bus reset sequence\n");
		goto err_interrupt;
	}

	if (multi_link) {
		ret = intel_shim_sync_go(sdw);
		if (ret < 0) {
			dev_err(dev, "sync go failed: %d\n", ret);
			goto err_interrupt;
		}
	}
	sdw_cdns_check_self_clearing_bits(cdns, __func__,
					  true, INTEL_MASTER_RESET_ITERATIONS);

	/* Register DAIs */
	ret = intel_register_dai(sdw);
	if (ret) {
		dev_err(dev, "DAI registration failed: %d\n", ret);
		snd_soc_unregister_component(dev);
		goto err_interrupt;
	}

	intel_debugfs_init(sdw);

	/* Enable runtime PM */
	if (!(link_flags & SDW_INTEL_MASTER_DISABLE_PM_RUNTIME)) {
		pm_runtime_set_autosuspend_delay(dev,
						 INTEL_MASTER_SUSPEND_DELAY_MS);
		pm_runtime_use_autosuspend(dev);
		pm_runtime_mark_last_busy(dev);

		pm_runtime_set_active(dev);
		pm_runtime_enable(dev);
	}

	clock_stop_quirks = sdw->link_res->clock_stop_quirks;
	if (clock_stop_quirks & SDW_INTEL_CLK_STOP_NOT_ALLOWED) {
		/*
		 * To keep the clock running we need to prevent
		 * pm_runtime suspend from happening by increasing the
		 * reference count.
		 * This quirk is specified by the parent PCI device in
		 * case of specific latency requirements. It will have
		 * no effect if pm_runtime is disabled by the user via
		 * a module parameter for testing purposes.
		 */
		pm_runtime_get_noresume(dev);
	}

	/*
	 * The runtime PM status of Slave devices is "Unsupported"
	 * until they report as ATTACHED. If they don't, e.g. because
	 * there are no Slave devices populated or if the power-on is
	 * delayed or dependent on a power switch, the Master will
	 * remain active and prevent its parent from suspending.
	 *
	 * Conditionally force the pm_runtime core to re-evaluate the
	 * Master status in the absence of any Slave activity. A quirk
	 * is provided to e.g. deal with Slaves that may be powered on
	 * with a delay. A more complete solution would require the
	 * definition of Master properties.
	 */
	if (!(link_flags & SDW_INTEL_MASTER_DISABLE_PM_RUNTIME_IDLE))
		pm_runtime_idle(dev);

	sdw->startup_done = true;
	return 0;

err_interrupt:
	sdw_cdns_enable_interrupt(cdns, false);
err_init:
	return ret;
}

static void intel_link_remove(struct auxiliary_device *auxdev)
{
	struct device *dev = &auxdev->dev;
	struct sdw_cdns *cdns = dev_get_drvdata(dev);
	struct sdw_intel *sdw = cdns_to_intel(cdns);
	struct sdw_bus *bus = &cdns->bus;

	/*
	 * Since pm_runtime is already disabled, we don't decrease
	 * the refcount when the clock_stop_quirk is
	 * SDW_INTEL_CLK_STOP_NOT_ALLOWED
	 */
	if (!bus->prop.hw_disabled) {
		intel_debugfs_exit(sdw);
		sdw_cdns_enable_interrupt(cdns, false);
		snd_soc_unregister_component(dev);
	}
	sdw_bus_master_delete(bus);
}

int intel_link_process_wakeen_event(struct auxiliary_device *auxdev)
{
	struct device *dev = &auxdev->dev;
	struct sdw_intel *sdw;
	struct sdw_bus *bus;
	void __iomem *shim;
	u16 wake_sts;

	sdw = dev_get_drvdata(dev);
	bus = &sdw->cdns.bus;

	if (bus->prop.hw_disabled || !sdw->startup_done) {
		dev_dbg(dev, "SoundWire master %d is disabled or not-started, ignoring\n",
			bus->link_id);
		return 0;
	}

	shim = sdw->link_res->shim;
	wake_sts = intel_readw(shim, SDW_SHIM_WAKESTS);

	if (!(wake_sts & BIT(sdw->instance)))
		return 0;

	/* disable WAKEEN interrupt ASAP to prevent interrupt flood */
	intel_shim_wake(sdw, false);

	/*
	 * resume the Master, which will generate a bus reset and result in
	 * Slaves re-attaching and be re-enumerated. The SoundWire physical
	 * device which generated the wake will trigger an interrupt, which
	 * will in turn cause the corresponding Linux Slave device to be
	 * resumed and the Slave codec driver to check the status.
	 */
	pm_request_resume(dev);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static struct platform_driver sdw_intel_drv = {
	.probe = intel_probe,
	.remove = intel_remove,
	.driver = {
		.name = "int-sdw",

	},
};

module_platform_driver(sdw_intel_drv);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("platform:int-sdw");
MODULE_DESCRIPTION("Intel Soundwire Master Driver");
=======
/*
 * PM calls
 */

static int intel_resume_child_device(struct device *dev, void *data)
{
	int ret;
	struct sdw_slave *slave = dev_to_sdw_dev(dev);

	if (!slave->probed) {
		dev_dbg(dev, "%s: skipping device, no probed driver\n", __func__);
		return 0;
	}
	if (!slave->dev_num_sticky) {
		dev_dbg(dev, "%s: skipping device, never detected on bus\n", __func__);
		return 0;
	}

	ret = pm_request_resume(dev);
	if (ret < 0)
		dev_err(dev, "%s: pm_request_resume failed: %d\n", __func__, ret);

	return ret;
}

static int __maybe_unused intel_pm_prepare(struct device *dev)
{
	struct sdw_cdns *cdns = dev_get_drvdata(dev);
	struct sdw_intel *sdw = cdns_to_intel(cdns);
	struct sdw_bus *bus = &cdns->bus;
	u32 clock_stop_quirks;
	int ret = 0;

	if (bus->prop.hw_disabled || !sdw->startup_done) {
		dev_dbg(dev, "SoundWire master %d is disabled or not-started, ignoring\n",
			bus->link_id);
		return 0;
	}

	clock_stop_quirks = sdw->link_res->clock_stop_quirks;

	if (pm_runtime_suspended(dev) &&
	    pm_runtime_suspended(dev->parent) &&
	    ((clock_stop_quirks & SDW_INTEL_CLK_STOP_BUS_RESET) ||
	     !clock_stop_quirks)) {
		/*
		 * if we've enabled clock stop, and the parent is suspended, the SHIM registers
		 * are not accessible and the shim wake cannot be disabled.
		 * The only solution is to resume the entire bus to full power
		 */

		/*
		 * If any operation in this block fails, we keep going since we don't want
		 * to prevent system suspend from happening and errors should be recoverable
		 * on resume.
		 */

		/*
		 * first resume the device for this link. This will also by construction
		 * resume the PCI parent device.
		 */
		ret = pm_request_resume(dev);
		if (ret < 0) {
			dev_err(dev, "%s: pm_request_resume failed: %d\n", __func__, ret);
			return 0;
		}

		/*
		 * Continue resuming the entire bus (parent + child devices) to exit
		 * the clock stop mode. If there are no devices connected on this link
		 * this is a no-op.
		 * The resume to full power could have been implemented with a .prepare
		 * step in SoundWire codec drivers. This would however require a lot
		 * of code to handle an Intel-specific corner case. It is simpler in
		 * practice to add a loop at the link level.
		 */
		ret = device_for_each_child(bus->dev, NULL, intel_resume_child_device);

		if (ret < 0)
			dev_err(dev, "%s: intel_resume_child_device failed: %d\n", __func__, ret);
	}

	return 0;
}

static int __maybe_unused intel_suspend(struct device *dev)
{
	struct sdw_cdns *cdns = dev_get_drvdata(dev);
	struct sdw_intel *sdw = cdns_to_intel(cdns);
	struct sdw_bus *bus = &cdns->bus;
	u32 clock_stop_quirks;
	int ret;

	if (bus->prop.hw_disabled || !sdw->startup_done) {
		dev_dbg(dev, "SoundWire master %d is disabled or not-started, ignoring\n",
			bus->link_id);
		return 0;
	}

	if (pm_runtime_suspended(dev)) {
		dev_dbg(dev, "%s: pm_runtime status: suspended\n", __func__);

		clock_stop_quirks = sdw->link_res->clock_stop_quirks;

		if ((clock_stop_quirks & SDW_INTEL_CLK_STOP_BUS_RESET) ||
		    !clock_stop_quirks) {

			if (pm_runtime_suspended(dev->parent)) {
				/*
				 * paranoia check: this should not happen with the .prepare
				 * resume to full power
				 */
				dev_err(dev, "%s: invalid config: parent is suspended\n", __func__);
			} else {
				intel_shim_wake(sdw, false);
			}
		}

		return 0;
	}

	ret = sdw_cdns_enable_interrupt(cdns, false);
	if (ret < 0) {
		dev_err(dev, "cannot disable interrupts on suspend\n");
		return ret;
	}

	ret = intel_link_power_down(sdw);
	if (ret) {
		dev_err(dev, "Link power down failed: %d\n", ret);
		return ret;
	}

	intel_shim_wake(sdw, false);

	return 0;
}

static int __maybe_unused intel_suspend_runtime(struct device *dev)
{
	struct sdw_cdns *cdns = dev_get_drvdata(dev);
	struct sdw_intel *sdw = cdns_to_intel(cdns);
	struct sdw_bus *bus = &cdns->bus;
	u32 clock_stop_quirks;
	int ret;

	if (bus->prop.hw_disabled || !sdw->startup_done) {
		dev_dbg(dev, "SoundWire master %d is disabled or not-started, ignoring\n",
			bus->link_id);
		return 0;
	}

	clock_stop_quirks = sdw->link_res->clock_stop_quirks;

	if (clock_stop_quirks & SDW_INTEL_CLK_STOP_TEARDOWN) {

		ret = sdw_cdns_enable_interrupt(cdns, false);
		if (ret < 0) {
			dev_err(dev, "cannot disable interrupts on suspend\n");
			return ret;
		}

		ret = intel_link_power_down(sdw);
		if (ret) {
			dev_err(dev, "Link power down failed: %d\n", ret);
			return ret;
		}

		intel_shim_wake(sdw, false);

	} else if (clock_stop_quirks & SDW_INTEL_CLK_STOP_BUS_RESET ||
		   !clock_stop_quirks) {
		bool wake_enable = true;

		ret = sdw_cdns_clock_stop(cdns, true);
		if (ret < 0) {
			dev_err(dev, "cannot enable clock stop on suspend\n");
			wake_enable = false;
		}

		ret = sdw_cdns_enable_interrupt(cdns, false);
		if (ret < 0) {
			dev_err(dev, "cannot disable interrupts on suspend\n");
			return ret;
		}

		ret = intel_link_power_down(sdw);
		if (ret) {
			dev_err(dev, "Link power down failed: %d\n", ret);
			return ret;
		}

		intel_shim_wake(sdw, wake_enable);
	} else {
		dev_err(dev, "%s clock_stop_quirks %x unsupported\n",
			__func__, clock_stop_quirks);
		ret = -EINVAL;
	}

	return ret;
}

static int __maybe_unused intel_resume(struct device *dev)
{
	struct sdw_cdns *cdns = dev_get_drvdata(dev);
	struct sdw_intel *sdw = cdns_to_intel(cdns);
	struct sdw_bus *bus = &cdns->bus;
	int link_flags;
	bool multi_link;
	int ret;

	if (bus->prop.hw_disabled || !sdw->startup_done) {
		dev_dbg(dev, "SoundWire master %d is disabled or not-started, ignoring\n",
			bus->link_id);
		return 0;
	}

	link_flags = md_flags >> (bus->link_id * 8);
	multi_link = !(link_flags & SDW_INTEL_MASTER_DISABLE_MULTI_LINK);

	if (pm_runtime_suspended(dev)) {
		dev_dbg(dev, "%s: pm_runtime status was suspended, forcing active\n", __func__);

		/* follow required sequence from runtime_pm.rst */
		pm_runtime_disable(dev);
		pm_runtime_set_active(dev);
		pm_runtime_mark_last_busy(dev);
		pm_runtime_enable(dev);

		link_flags = md_flags >> (bus->link_id * 8);

		if (!(link_flags & SDW_INTEL_MASTER_DISABLE_PM_RUNTIME_IDLE))
			pm_runtime_idle(dev);
	}

	ret = intel_init(sdw);
	if (ret) {
		dev_err(dev, "%s failed: %d\n", __func__, ret);
		return ret;
	}

	/*
	 * make sure all Slaves are tagged as UNATTACHED and provide
	 * reason for reinitialization
	 */
	sdw_clear_slave_status(bus, SDW_UNATTACH_REQUEST_MASTER_RESET);

	ret = sdw_cdns_enable_interrupt(cdns, true);
	if (ret < 0) {
		dev_err(dev, "cannot enable interrupts during resume\n");
		return ret;
	}

	/*
	 * follow recommended programming flows to avoid timeouts when
	 * gsync is enabled
	 */
	if (multi_link)
		intel_shim_sync_arm(sdw);

	ret = sdw_cdns_init(&sdw->cdns);
	if (ret < 0) {
		dev_err(dev, "unable to initialize Cadence IP during resume\n");
		return ret;
	}

	ret = sdw_cdns_exit_reset(cdns);
	if (ret < 0) {
		dev_err(dev, "unable to exit bus reset sequence during resume\n");
		return ret;
	}

	if (multi_link) {
		ret = intel_shim_sync_go(sdw);
		if (ret < 0) {
			dev_err(dev, "sync go failed during resume\n");
			return ret;
		}
	}
	sdw_cdns_check_self_clearing_bits(cdns, __func__,
					  true, INTEL_MASTER_RESET_ITERATIONS);

	/*
	 * after system resume, the pm_runtime suspend() may kick in
	 * during the enumeration, before any children device force the
	 * master device to remain active.  Using pm_runtime_get()
	 * routines is not really possible, since it'd prevent the
	 * master from suspending.
	 * A reasonable compromise is to update the pm_runtime
	 * counters and delay the pm_runtime suspend by several
	 * seconds, by when all enumeration should be complete.
	 */
	pm_runtime_mark_last_busy(dev);

	return ret;
}

static int __maybe_unused intel_resume_runtime(struct device *dev)
{
	struct sdw_cdns *cdns = dev_get_drvdata(dev);
	struct sdw_intel *sdw = cdns_to_intel(cdns);
	struct sdw_bus *bus = &cdns->bus;
	u32 clock_stop_quirks;
	bool clock_stop0;
	int link_flags;
	bool multi_link;
	int status;
	int ret;

	if (bus->prop.hw_disabled || !sdw->startup_done) {
		dev_dbg(dev, "SoundWire master %d is disabled or not-started, ignoring\n",
			bus->link_id);
		return 0;
	}

	link_flags = md_flags >> (bus->link_id * 8);
	multi_link = !(link_flags & SDW_INTEL_MASTER_DISABLE_MULTI_LINK);

	clock_stop_quirks = sdw->link_res->clock_stop_quirks;

	if (clock_stop_quirks & SDW_INTEL_CLK_STOP_TEARDOWN) {
		ret = intel_init(sdw);
		if (ret) {
			dev_err(dev, "%s failed: %d\n", __func__, ret);
			return ret;
		}

		/*
		 * make sure all Slaves are tagged as UNATTACHED and provide
		 * reason for reinitialization
		 */
		sdw_clear_slave_status(bus, SDW_UNATTACH_REQUEST_MASTER_RESET);

		ret = sdw_cdns_enable_interrupt(cdns, true);
		if (ret < 0) {
			dev_err(dev, "cannot enable interrupts during resume\n");
			return ret;
		}

		/*
		 * follow recommended programming flows to avoid
		 * timeouts when gsync is enabled
		 */
		if (multi_link)
			intel_shim_sync_arm(sdw);

		ret = sdw_cdns_init(&sdw->cdns);
		if (ret < 0) {
			dev_err(dev, "unable to initialize Cadence IP during resume\n");
			return ret;
		}

		ret = sdw_cdns_exit_reset(cdns);
		if (ret < 0) {
			dev_err(dev, "unable to exit bus reset sequence during resume\n");
			return ret;
		}

		if (multi_link) {
			ret = intel_shim_sync_go(sdw);
			if (ret < 0) {
				dev_err(dev, "sync go failed during resume\n");
				return ret;
			}
		}
		sdw_cdns_check_self_clearing_bits(cdns, "intel_resume_runtime TEARDOWN",
						  true, INTEL_MASTER_RESET_ITERATIONS);

	} else if (clock_stop_quirks & SDW_INTEL_CLK_STOP_BUS_RESET) {
		ret = intel_init(sdw);
		if (ret) {
			dev_err(dev, "%s failed: %d\n", __func__, ret);
			return ret;
		}

		/*
		 * An exception condition occurs for the CLK_STOP_BUS_RESET
		 * case if one or more masters remain active. In this condition,
		 * all the masters are powered on for they are in the same power
		 * domain. Master can preserve its context for clock stop0, so
		 * there is no need to clear slave status and reset bus.
		 */
		clock_stop0 = sdw_cdns_is_clock_stop(&sdw->cdns);

		if (!clock_stop0) {

			/*
			 * make sure all Slaves are tagged as UNATTACHED and
			 * provide reason for reinitialization
			 */

			status = SDW_UNATTACH_REQUEST_MASTER_RESET;
			sdw_clear_slave_status(bus, status);

			ret = sdw_cdns_enable_interrupt(cdns, true);
			if (ret < 0) {
				dev_err(dev, "cannot enable interrupts during resume\n");
				return ret;
			}

			/*
			 * follow recommended programming flows to avoid
			 * timeouts when gsync is enabled
			 */
			if (multi_link)
				intel_shim_sync_arm(sdw);

			/*
			 * Re-initialize the IP since it was powered-off
			 */
			sdw_cdns_init(&sdw->cdns);

		} else {
			ret = sdw_cdns_enable_interrupt(cdns, true);
			if (ret < 0) {
				dev_err(dev, "cannot enable interrupts during resume\n");
				return ret;
			}
		}

		ret = sdw_cdns_clock_restart(cdns, !clock_stop0);
		if (ret < 0) {
			dev_err(dev, "unable to restart clock during resume\n");
			return ret;
		}

		if (!clock_stop0) {
			ret = sdw_cdns_exit_reset(cdns);
			if (ret < 0) {
				dev_err(dev, "unable to exit bus reset sequence during resume\n");
				return ret;
			}

			if (multi_link) {
				ret = intel_shim_sync_go(sdw);
				if (ret < 0) {
					dev_err(sdw->cdns.dev, "sync go failed during resume\n");
					return ret;
				}
			}
		}
		sdw_cdns_check_self_clearing_bits(cdns, "intel_resume_runtime BUS_RESET",
						  true, INTEL_MASTER_RESET_ITERATIONS);

	} else if (!clock_stop_quirks) {

		clock_stop0 = sdw_cdns_is_clock_stop(&sdw->cdns);
		if (!clock_stop0)
			dev_err(dev, "%s invalid configuration, clock was not stopped", __func__);

		ret = intel_init(sdw);
		if (ret) {
			dev_err(dev, "%s failed: %d\n", __func__, ret);
			return ret;
		}

		ret = sdw_cdns_enable_interrupt(cdns, true);
		if (ret < 0) {
			dev_err(dev, "cannot enable interrupts during resume\n");
			return ret;
		}

		ret = sdw_cdns_clock_restart(cdns, false);
		if (ret < 0) {
			dev_err(dev, "unable to resume master during resume\n");
			return ret;
		}

		sdw_cdns_check_self_clearing_bits(cdns, "intel_resume_runtime no_quirks",
						  true, INTEL_MASTER_RESET_ITERATIONS);
	} else {
		dev_err(dev, "%s clock_stop_quirks %x unsupported\n",
			__func__, clock_stop_quirks);
		ret = -EINVAL;
	}

	return ret;
}

static const struct dev_pm_ops intel_pm = {
	.prepare = intel_pm_prepare,
	SET_SYSTEM_SLEEP_PM_OPS(intel_suspend, intel_resume)
	SET_RUNTIME_PM_OPS(intel_suspend_runtime, intel_resume_runtime, NULL)
};

static const struct auxiliary_device_id intel_link_id_table[] = {
	{ .name = "soundwire_intel.link" },
	{},
};
MODULE_DEVICE_TABLE(auxiliary, intel_link_id_table);

static struct auxiliary_driver sdw_intel_drv = {
	.probe = intel_link_probe,
	.remove = intel_link_remove,
	.driver = {
		/* auxiliary_driver_register() sets .name to be the modname */
		.pm = &intel_pm,
	},
	.id_table = intel_link_id_table
};
module_auxiliary_driver(sdw_intel_drv);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Intel Soundwire Link Driver");
>>>>>>> upstream/android-13
