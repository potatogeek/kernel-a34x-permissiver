// SPDX-License-Identifier: GPL-2.0
/*
 * MediaTek xHCI Host Controller Driver
 *
 * Copyright (c) 2015 MediaTek Inc.
 * Author:
 *  Chunfeng Yun <chunfeng.yun@mediatek.com>
 */

<<<<<<< HEAD
#include <linux/clk.h>
=======
>>>>>>> upstream/android-13
#include <linux/dma-mapping.h>
#include <linux/iopoll.h>
#include <linux/kernel.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
<<<<<<< HEAD
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#if IS_ENABLED(CONFIG_MTK_USB_OFFLOAD)
#include <linux/of_device.h>
#include <linux/usb.h>
#endif
=======
#include <linux/pm_wakeirq.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
>>>>>>> upstream/android-13

#include "xhci.h"
#include "xhci-mtk.h"

<<<<<<< HEAD
#if IS_ENABLED(CONFIG_MTK_USB_OFFLOAD)
#include "usb_offload.h"
#endif

=======
>>>>>>> upstream/android-13
/* ip_pw_ctrl0 register */
#define CTRL0_IP_SW_RST	BIT(0)

/* ip_pw_ctrl1 register */
#define CTRL1_IP_HOST_PDN	BIT(0)

/* ip_pw_ctrl2 register */
#define CTRL2_IP_DEV_PDN	BIT(0)

/* ip_pw_sts1 register */
#define STS1_IP_SLEEP_STS	BIT(30)
#define STS1_U3_MAC_RST	BIT(16)
#define STS1_XHCI_RST		BIT(11)
#define STS1_SYS125_RST	BIT(10)
#define STS1_REF_RST		BIT(8)
#define STS1_SYSPLL_STABLE	BIT(0)

/* ip_xhci_cap register */
#define CAP_U3_PORT_NUM(p)	((p) & 0xff)
#define CAP_U2_PORT_NUM(p)	(((p) >> 8) & 0xff)

/* u3_ctrl_p register */
#define CTRL_U3_PORT_HOST_SEL	BIT(2)
#define CTRL_U3_PORT_PDN	BIT(1)
#define CTRL_U3_PORT_DIS	BIT(0)

/* u2_ctrl_p register */
#define CTRL_U2_PORT_HOST_SEL	BIT(2)
#define CTRL_U2_PORT_PDN	BIT(1)
#define CTRL_U2_PORT_DIS	BIT(0)

/* u2_phy_pll register */
#define CTRL_U2_FORCE_PLL_STB	BIT(28)

<<<<<<< HEAD
/* usb remote wakeup registers in syscon */
=======
/* xHCI CSR */
#define LS_EOF_CFG		0x930
#define LSEOF_OFFSET		0x89

#define FS_EOF_CFG		0x934
#define FSEOF_OFFSET		0x2e

#define SS_GEN1_EOF_CFG		0x93c
#define SSG1EOF_OFFSET		0x78

#define HFCNTR_CFG		0x944
#define ITP_DELTA_CLK		(0xa << 1)
#define ITP_DELTA_CLK_MASK	GENMASK(5, 1)
#define FRMCNT_LEV1_RANG	(0x12b << 8)
#define FRMCNT_LEV1_RANG_MASK	GENMASK(19, 8)

#define SS_GEN2_EOF_CFG		0x990
#define SSG2EOF_OFFSET		0x3c

#define XSEOF_OFFSET_MASK	GENMASK(11, 0)

/* usb remote wakeup registers in syscon */

>>>>>>> upstream/android-13
/* mt8173 etc */
#define PERI_WK_CTRL1	0x4
#define WC1_IS_C(x)	(((x) & 0xf) << 26)  /* cycle debounce */
#define WC1_IS_EN	BIT(25)
#define WC1_IS_P	BIT(6)  /* polarity for ip sleep */

<<<<<<< HEAD
=======
/* mt8183 */
#define PERI_WK_CTRL0	0x0
#define WC0_IS_C(x)	((u32)(((x) & 0xf) << 28))  /* cycle debounce */
#define WC0_IS_P	BIT(12)	/* polarity */
#define WC0_IS_EN	BIT(6)

/* mt8192 */
#define WC0_SSUSB0_CDEN		BIT(6)
#define WC0_IS_SPM_EN		BIT(1)

>>>>>>> upstream/android-13
/* mt2712 etc */
#define PERI_SSUSB_SPM_CTRL	0x0
#define SSC_IP_SLEEP_EN	BIT(4)
#define SSC_SPM_INT_EN		BIT(1)

<<<<<<< HEAD
/*testmode*/
#define HOST_CMD_STOP				0x0
#define HOST_CMD_TEST_J             0x1
#define HOST_CMD_TEST_K             0x2
#define HOST_CMD_TEST_SE0_NAK       0x3
#define HOST_CMD_TEST_PACKET        0x4
#define PMSC_PORT_TEST_CTRL_OFFSET  28

/*procfs node*/
#define PROC_FILES_NUM 1
static struct proc_dir_entry *proc_files[PROC_FILES_NUM] = {
	NULL};

enum ssusb_uwk_vers {
	SSUSB_UWK_V1 = 1,
	SSUSB_UWK_V2,
};

static int xhci_testmode_show(struct seq_file *s, void *unused)
{
	struct xhci_hcd_mtk *mtk = s->private;
	struct xhci_hcd	*xhci = hcd_to_xhci(mtk->hcd);

	switch (xhci->test_mode) {
	case HOST_CMD_STOP:
		seq_puts(s, "0\n");
		break;
	case HOST_CMD_TEST_J:
		seq_puts(s, "test J\n");
		break;
	case HOST_CMD_TEST_K:
		seq_puts(s, "test K\n");
		break;
	case HOST_CMD_TEST_SE0_NAK:
		seq_puts(s, "test SE0 NAK\n");
		break;
	case HOST_CMD_TEST_PACKET:
		seq_puts(s, "test packet\n");
		break;
	default:
		break;
	}

	return 0;
}

static int xhci_testmode_open(struct inode *inode, struct file *file)
{
	return single_open(file, xhci_testmode_show, PDE_DATA(inode));
}

static ssize_t xhci_testmode_write(struct file *file,  const char __user *ubuf,
			       size_t count, loff_t *ppos)
{
	struct seq_file *s = file->private_data;
	struct xhci_hcd_mtk *mtk = s->private;
	struct xhci_hcd	*xhci = hcd_to_xhci(mtk->hcd);
	int ports = HCS_MAX_PORTS(xhci->hcs_params1);
	char buf[32];
	unsigned long flags;
	u8 testmode = HOST_CMD_STOP;
	u32 temp;
	u32 __iomem *addr;
	int i;

	if (copy_from_user(&buf, ubuf, min_t(size_t, sizeof(buf) - 1, count)))
		return -EFAULT;

	if (!strncmp(buf, "test packet", 10))
		testmode = HOST_CMD_TEST_PACKET;
	else if (!strncmp(buf, "test K", 6))
		testmode = HOST_CMD_TEST_K;
	else if (!strncmp(buf, "test J", 6))
		testmode = HOST_CMD_TEST_J;
	else if (!strncmp(buf, "test SE0 NAK", 12))
		testmode = HOST_CMD_TEST_SE0_NAK;

	if (testmode >= HOST_CMD_STOP && testmode <= HOST_CMD_TEST_PACKET) {
		xhci_info(xhci, "set test mode %d\n", testmode);

		spin_lock_irqsave(&xhci->lock, flags);

		/* set the Run/Stop in USBCMD to 0 */
		addr = &xhci->op_regs->command;
		temp = readl(addr);
		temp &= ~CMD_RUN;
		writel(temp, addr);

		/*  wait for HCHalted */
		xhci_halt(xhci);

		/* test mode */
		for (i = 0; i < ports; i++) {
			addr = &xhci->op_regs->port_power_base +
				NUM_PORT_REGS * (i & 0xff);
			temp = readl(addr);
			temp &= ~(0xf << PMSC_PORT_TEST_CTRL_OFFSET);
			temp |= (testmode << PMSC_PORT_TEST_CTRL_OFFSET);
			writel(temp, addr);
		}

		xhci->test_mode = testmode;
		spin_unlock_irqrestore(&xhci->lock, flags);
	} else {
		pr_info("%s: invalid value\n", __func__);
		return -EINVAL;
	}

	return count;
}

static const struct file_operations testmode_fops = {
	.open			= xhci_testmode_open,
	.write			= xhci_testmode_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static void xhci_mtk_dbg_init(struct xhci_hcd_mtk *mtk)
{
	u8 idx = 0;

	proc_mkdir("mtk_usb", NULL);

	proc_files[idx] = proc_create_data("mtk_usb/testmode", 0644, NULL, &testmode_fops, mtk);
	if (!proc_files[idx])
		pr_info("%s: fail to create testmode node in procfs\n", __func__);
	idx++;
}

static void xhci_mtk_dbg_exit(struct xhci_hcd_mtk *mtk)
{
	u8 idx = 0;

	for (; idx < PROC_FILES_NUM; idx++) {
		if (proc_files[idx])
			proc_remove(proc_files[idx]);
	}
}

int mtk_xhci_wakelock_lock(struct xhci_hcd_mtk *mtk)
{
	struct xhci_hcd *xhci = hcd_to_xhci(mtk->hcd);

	pm_stay_awake(mtk->dev);
	xhci_info(xhci, "wakelock_lock\n");

	return 0;
}
EXPORT_SYMBOL_GPL(mtk_xhci_wakelock_lock);

int mtk_xhci_wakelock_unlock(struct xhci_hcd_mtk *mtk)
{
	struct xhci_hcd *xhci = hcd_to_xhci(mtk->hcd);

	pm_relax(mtk->dev);
	xhci_info(xhci, "wakelock_unlock\n");

	return 0;
}
EXPORT_SYMBOL_GPL(mtk_xhci_wakelock_unlock);

=======
enum ssusb_uwk_vers {
	SSUSB_UWK_V1 = 1,
	SSUSB_UWK_V2,
	SSUSB_UWK_V1_1 = 101,	/* specific revision 1.01 */
	SSUSB_UWK_V1_2,		/* specific revision 1.2 */
};

/*
 * MT8195 has 4 controllers, the controller1~3's default SOF/ITP interval
 * is calculated from the frame counter clock 24M, but in fact, the clock
 * is 48M, add workaround for it.
 */
static void xhci_mtk_set_frame_interval(struct xhci_hcd_mtk *mtk)
{
	struct device *dev = mtk->dev;
	struct usb_hcd *hcd = mtk->hcd;
	u32 value;

	if (!of_device_is_compatible(dev->of_node, "mediatek,mt8195-xhci"))
		return;

	value = readl(hcd->regs + HFCNTR_CFG);
	value &= ~(ITP_DELTA_CLK_MASK | FRMCNT_LEV1_RANG_MASK);
	value |= (ITP_DELTA_CLK | FRMCNT_LEV1_RANG);
	writel(value, hcd->regs + HFCNTR_CFG);

	value = readl(hcd->regs + LS_EOF_CFG);
	value &= ~XSEOF_OFFSET_MASK;
	value |= LSEOF_OFFSET;
	writel(value, hcd->regs + LS_EOF_CFG);

	value = readl(hcd->regs + FS_EOF_CFG);
	value &= ~XSEOF_OFFSET_MASK;
	value |= FSEOF_OFFSET;
	writel(value, hcd->regs + FS_EOF_CFG);

	value = readl(hcd->regs + SS_GEN1_EOF_CFG);
	value &= ~XSEOF_OFFSET_MASK;
	value |= SSG1EOF_OFFSET;
	writel(value, hcd->regs + SS_GEN1_EOF_CFG);

	value = readl(hcd->regs + SS_GEN2_EOF_CFG);
	value &= ~XSEOF_OFFSET_MASK;
	value |= SSG2EOF_OFFSET;
	writel(value, hcd->regs + SS_GEN2_EOF_CFG);
}

>>>>>>> upstream/android-13
static int xhci_mtk_host_enable(struct xhci_hcd_mtk *mtk)
{
	struct mu3c_ippc_regs __iomem *ippc = mtk->ippc_regs;
	u32 value, check_val;
<<<<<<< HEAD
	int u3_ports_disabed = 0;
=======
	int u3_ports_disabled = 0;
>>>>>>> upstream/android-13
	int ret;
	int i;

	if (!mtk->has_ippc)
		return 0;

	/* power on host ip */
	value = readl(&ippc->ip_pw_ctr1);
	value &= ~CTRL1_IP_HOST_PDN;
	writel(value, &ippc->ip_pw_ctr1);

	/* power on and enable u3 ports except skipped ones */
	for (i = 0; i < mtk->num_u3_ports; i++) {
		if ((0x1 << i) & mtk->u3p_dis_msk) {
<<<<<<< HEAD
			u3_ports_disabed++;
=======
			u3_ports_disabled++;
>>>>>>> upstream/android-13
			continue;
		}

		value = readl(&ippc->u3_ctrl_p[i]);
		value &= ~(CTRL_U3_PORT_PDN | CTRL_U3_PORT_DIS);
		value |= CTRL_U3_PORT_HOST_SEL;
		writel(value, &ippc->u3_ctrl_p[i]);
	}

<<<<<<< HEAD
	/* power on and enable all u2 ports */
	for (i = 0; i < mtk->num_u2_ports; i++) {
=======
	/* power on and enable all u2 ports except skipped ones */
	for (i = 0; i < mtk->num_u2_ports; i++) {
		if (BIT(i) & mtk->u2p_dis_msk)
			continue;

>>>>>>> upstream/android-13
		value = readl(&ippc->u2_ctrl_p[i]);
		value &= ~(CTRL_U2_PORT_PDN | CTRL_U2_PORT_DIS);
		value |= CTRL_U2_PORT_HOST_SEL;
		writel(value, &ippc->u2_ctrl_p[i]);
	}

	/*
	 * wait for clocks to be stable, and clock domains reset to
	 * be inactive after power on and enable ports
	 */
	check_val = STS1_SYSPLL_STABLE | STS1_REF_RST |
			STS1_SYS125_RST | STS1_XHCI_RST;

<<<<<<< HEAD
	if (mtk->num_u3_ports > u3_ports_disabed)
=======
	if (mtk->num_u3_ports > u3_ports_disabled)
>>>>>>> upstream/android-13
		check_val |= STS1_U3_MAC_RST;

	ret = readl_poll_timeout(&ippc->ip_pw_sts1, value,
			  (check_val == (value & check_val)), 100, 20000);
	if (ret) {
		dev_err(mtk->dev, "clocks are not stable (0x%x)\n", value);
		return ret;
	}

	return 0;
}

static int xhci_mtk_host_disable(struct xhci_hcd_mtk *mtk)
{
	struct mu3c_ippc_regs __iomem *ippc = mtk->ippc_regs;
	u32 value;
	int ret;
	int i;

	if (!mtk->has_ippc)
		return 0;

	/* power down u3 ports except skipped ones */
	for (i = 0; i < mtk->num_u3_ports; i++) {
		if ((0x1 << i) & mtk->u3p_dis_msk)
			continue;

		value = readl(&ippc->u3_ctrl_p[i]);
		value |= CTRL_U3_PORT_PDN;
		writel(value, &ippc->u3_ctrl_p[i]);
	}

<<<<<<< HEAD
	/* power down all u2 ports */
	for (i = 0; i < mtk->num_u2_ports; i++) {
=======
	/* power down all u2 ports except skipped ones */
	for (i = 0; i < mtk->num_u2_ports; i++) {
		if (BIT(i) & mtk->u2p_dis_msk)
			continue;

>>>>>>> upstream/android-13
		value = readl(&ippc->u2_ctrl_p[i]);
		value |= CTRL_U2_PORT_PDN;
		writel(value, &ippc->u2_ctrl_p[i]);
	}

	/* power down host ip */
	value = readl(&ippc->ip_pw_ctr1);
	value |= CTRL1_IP_HOST_PDN;
	writel(value, &ippc->ip_pw_ctr1);

	/* wait for host ip to sleep */
	ret = readl_poll_timeout(&ippc->ip_pw_sts1, value,
			  (value & STS1_IP_SLEEP_STS), 100, 100000);
	if (ret) {
		dev_err(mtk->dev, "ip sleep failed!!!\n");
		return ret;
	}
	return 0;
}

static int xhci_mtk_ssusb_config(struct xhci_hcd_mtk *mtk)
{
	struct mu3c_ippc_regs __iomem *ippc = mtk->ippc_regs;
	u32 value;

	if (!mtk->has_ippc)
		return 0;

	/* reset whole ip */
	value = readl(&ippc->ip_pw_ctr0);
	value |= CTRL0_IP_SW_RST;
	writel(value, &ippc->ip_pw_ctr0);
	udelay(1);
	value = readl(&ippc->ip_pw_ctr0);
	value &= ~CTRL0_IP_SW_RST;
	writel(value, &ippc->ip_pw_ctr0);

	/*
	 * device ip is default power-on in fact
	 * power down device ip, otherwise ip-sleep will fail
	 */
	value = readl(&ippc->ip_pw_ctr2);
	value |= CTRL2_IP_DEV_PDN;
	writel(value, &ippc->ip_pw_ctr2);

	value = readl(&ippc->ip_xhci_cap);
	mtk->num_u3_ports = CAP_U3_PORT_NUM(value);
	mtk->num_u2_ports = CAP_U2_PORT_NUM(value);
	dev_dbg(mtk->dev, "%s u2p:%d, u3p:%d\n", __func__,
			mtk->num_u2_ports, mtk->num_u3_ports);

	return xhci_mtk_host_enable(mtk);
}

<<<<<<< HEAD
static int xhci_mtk_clks_get(struct xhci_hcd_mtk *mtk)
{
	struct device *dev = mtk->dev;

	mtk->sys_clk = devm_clk_get(dev, "sys_ck");
	if (IS_ERR(mtk->sys_clk)) {
		dev_err(dev, "fail to get sys_ck\n");
		return PTR_ERR(mtk->sys_clk);
	}

	mtk->ref_clk = devm_clk_get_optional(dev, "ref_ck");
	if (IS_ERR(mtk->ref_clk))
		return PTR_ERR(mtk->ref_clk);

	mtk->mcu_clk = devm_clk_get_optional(dev, "mcu_ck");
	if (IS_ERR(mtk->mcu_clk))
		return PTR_ERR(mtk->mcu_clk);

	mtk->dma_clk = devm_clk_get_optional(dev, "dma_ck");
	return PTR_ERR_OR_ZERO(mtk->dma_clk);
}

static int xhci_mtk_clks_enable(struct xhci_hcd_mtk *mtk)
{
	int ret;

	ret = clk_prepare_enable(mtk->ref_clk);
	if (ret) {
		dev_err(mtk->dev, "failed to enable ref_clk\n");
		goto ref_clk_err;
	}

	ret = clk_prepare_enable(mtk->sys_clk);
	if (ret) {
		dev_err(mtk->dev, "failed to enable sys_clk\n");
		goto sys_clk_err;
	}

	ret = clk_prepare_enable(mtk->mcu_clk);
	if (ret) {
		dev_err(mtk->dev, "failed to enable mcu_clk\n");
		goto mcu_clk_err;
	}

	ret = clk_prepare_enable(mtk->dma_clk);
	if (ret) {
		dev_err(mtk->dev, "failed to enable dma_clk\n");
		goto dma_clk_err;
	}

	return 0;

dma_clk_err:
	clk_disable_unprepare(mtk->mcu_clk);
mcu_clk_err:
	clk_disable_unprepare(mtk->sys_clk);
sys_clk_err:
	clk_disable_unprepare(mtk->ref_clk);
ref_clk_err:
	return ret;
}

static void xhci_mtk_clks_disable(struct xhci_hcd_mtk *mtk)
{
	clk_disable_unprepare(mtk->dma_clk);
	clk_disable_unprepare(mtk->mcu_clk);
	clk_disable_unprepare(mtk->sys_clk);
	clk_disable_unprepare(mtk->ref_clk);
}

=======
>>>>>>> upstream/android-13
/* only clocks can be turn off for ip-sleep wakeup mode */
static void usb_wakeup_ip_sleep_set(struct xhci_hcd_mtk *mtk, bool enable)
{
	u32 reg, msk, val;

	switch (mtk->uwk_vers) {
	case SSUSB_UWK_V1:
		reg = mtk->uwk_reg_base + PERI_WK_CTRL1;
		msk = WC1_IS_EN | WC1_IS_C(0xf) | WC1_IS_P;
		val = enable ? (WC1_IS_EN | WC1_IS_C(0x8)) : 0;
		break;
<<<<<<< HEAD
=======
	case SSUSB_UWK_V1_1:
		reg = mtk->uwk_reg_base + PERI_WK_CTRL0;
		msk = WC0_IS_EN | WC0_IS_C(0xf) | WC0_IS_P;
		val = enable ? (WC0_IS_EN | WC0_IS_C(0x8)) : 0;
		break;
	case SSUSB_UWK_V1_2:
		reg = mtk->uwk_reg_base + PERI_WK_CTRL0;
		msk = WC0_SSUSB0_CDEN | WC0_IS_SPM_EN;
		val = enable ? msk : 0;
		break;
>>>>>>> upstream/android-13
	case SSUSB_UWK_V2:
		reg = mtk->uwk_reg_base + PERI_SSUSB_SPM_CTRL;
		msk = SSC_IP_SLEEP_EN | SSC_SPM_INT_EN;
		val = enable ? msk : 0;
		break;
	default:
		return;
	}
	regmap_update_bits(mtk->uwk, reg, msk, val);
}

static int usb_wakeup_of_property_parse(struct xhci_hcd_mtk *mtk,
				struct device_node *dn)
{
	struct of_phandle_args args;
	int ret;

	/* Wakeup function is optional */
	mtk->uwk_en = of_property_read_bool(dn, "wakeup-source");
	if (!mtk->uwk_en)
		return 0;

	ret = of_parse_phandle_with_fixed_args(dn,
				"mediatek,syscon-wakeup", 2, 0, &args);
	if (ret)
		return ret;

	mtk->uwk_reg_base = args.args[0];
	mtk->uwk_vers = args.args[1];
	mtk->uwk = syscon_node_to_regmap(args.np);
	of_node_put(args.np);
	dev_info(mtk->dev, "uwk - reg:0x%x, version:%d\n",
			mtk->uwk_reg_base, mtk->uwk_vers);

	return PTR_ERR_OR_ZERO(mtk->uwk);
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
}

static void usb_wakeup_set(struct xhci_hcd_mtk *mtk, bool enable)
{
	if (mtk->uwk_en)
		usb_wakeup_ip_sleep_set(mtk, enable);
}

<<<<<<< HEAD
#if IS_ENABLED(CONFIG_MTK_USB_OFFLOAD)
bool xhci_vendor_is_streaming(struct xhci_hcd *xhci)
{
	struct xhci_vendor_ops *ops = xhci_vendor_get_ops(xhci);

	if (ops && ops->is_streaming)
		return ops->is_streaming(xhci);
	return 0;
}

static int xhci_mtk_bus_suspend(struct usb_hcd *hcd)
{
	struct xhci_hcd *xhci = hcd_to_xhci(hcd);
	int ret;

	if (xhci_vendor_is_streaming(xhci)) {
		pr_info("%s - USB_OFFLOAD bypass\n", __func__);
		return 0;
	}
	ret = xhci_bus_suspend(hcd);

	return ret;
}

static int xhci_mtk_bus_resume(struct usb_hcd *hcd)
{
	struct xhci_hcd *xhci = hcd_to_xhci(hcd);
	int ret;

	if (xhci_vendor_is_streaming(xhci)) {
		pr_info("%s - USB_OFFLOAD bypass\n", __func__);
		return 0;
	}
	ret = xhci_bus_resume(hcd);

	return ret;
}
#endif

static int xhci_mtk_setup(struct usb_hcd *hcd);
static const struct xhci_driver_overrides xhci_mtk_overrides __initconst = {
	.reset = xhci_mtk_setup,
#if IS_ENABLED(CONFIG_MTK_USB_OFFLOAD)
	.bus_suspend = xhci_mtk_bus_suspend,
	.bus_resume = xhci_mtk_bus_resume,
#endif
};

static struct hc_driver __read_mostly xhci_mtk_hc_driver;

=======
static int xhci_mtk_clks_get(struct xhci_hcd_mtk *mtk)
{
	struct clk_bulk_data *clks = mtk->clks;

	clks[0].id = "sys_ck";
	clks[1].id = "xhci_ck";
	clks[2].id = "ref_ck";
	clks[3].id = "mcu_ck";
	clks[4].id = "dma_ck";

	return devm_clk_bulk_get_optional(mtk->dev, BULK_CLKS_NUM, clks);
}

>>>>>>> upstream/android-13
static int xhci_mtk_ldos_enable(struct xhci_hcd_mtk *mtk)
{
	int ret;

	ret = regulator_enable(mtk->vbus);
	if (ret) {
		dev_err(mtk->dev, "failed to enable vbus\n");
		return ret;
	}

	ret = regulator_enable(mtk->vusb33);
	if (ret) {
		dev_err(mtk->dev, "failed to enable vusb33\n");
		regulator_disable(mtk->vbus);
		return ret;
	}
	return 0;
}

static void xhci_mtk_ldos_disable(struct xhci_hcd_mtk *mtk)
{
	regulator_disable(mtk->vbus);
	regulator_disable(mtk->vusb33);
}

static void xhci_mtk_quirks(struct device *dev, struct xhci_hcd *xhci)
{
	struct usb_hcd *hcd = xhci_to_hcd(xhci);
	struct xhci_hcd_mtk *mtk = hcd_to_mtk(hcd);

	/*
	 * As of now platform drivers don't provide MSI support so we ensure
	 * here that the generic code does not try to make a pci_dev from our
	 * dev struct in order to setup MSI
	 */
	xhci->quirks |= XHCI_PLAT;
	xhci->quirks |= XHCI_MTK_HOST;
	/*
	 * MTK host controller gives a spurious successful event after a
	 * short transfer. Ignore it.
	 */
	xhci->quirks |= XHCI_SPURIOUS_SUCCESS;
	if (mtk->lpm_support)
		xhci->quirks |= XHCI_LPM_SUPPORT;
	if (mtk->u2_lpm_disable)
		xhci->quirks |= XHCI_HW_LPM_DISABLE;

	/*
	 * MTK xHCI 0.96: PSA is 1 by default even if doesn't support stream,
	 * and it's 3 when support it.
	 */
	if (xhci->hci_version < 0x100 && HCC_MAX_PSA(xhci->hcc_params) == 4)
		xhci->quirks |= XHCI_BROKEN_STREAMS;
}

/* called during probe() after chip reset completes */
static int xhci_mtk_setup(struct usb_hcd *hcd)
{
	struct xhci_hcd_mtk *mtk = hcd_to_mtk(hcd);
	int ret;

	if (usb_hcd_is_primary_hcd(hcd)) {
		ret = xhci_mtk_ssusb_config(mtk);
		if (ret)
			return ret;
<<<<<<< HEAD
=======

		/* workaround only for mt8195 */
		xhci_mtk_set_frame_interval(mtk);
>>>>>>> upstream/android-13
	}

	ret = xhci_gen_setup(hcd, xhci_mtk_quirks);
	if (ret)
		return ret;

	if (usb_hcd_is_primary_hcd(hcd)) {
		ret = xhci_mtk_sch_init(mtk);
		if (ret)
			return ret;
	}

	return ret;
}

<<<<<<< HEAD
=======
static const struct xhci_driver_overrides xhci_mtk_overrides __initconst = {
	.reset = xhci_mtk_setup,
	.add_endpoint = xhci_mtk_add_ep,
	.drop_endpoint = xhci_mtk_drop_ep,
	.check_bandwidth = xhci_mtk_check_bandwidth,
	.reset_bandwidth = xhci_mtk_reset_bandwidth,
};

static struct hc_driver __read_mostly xhci_mtk_hc_driver;

>>>>>>> upstream/android-13
static int xhci_mtk_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *node = dev->of_node;
	struct xhci_hcd_mtk *mtk;
	const struct hc_driver *driver;
	struct xhci_hcd *xhci;
	struct resource *res;
	struct usb_hcd *hcd;
<<<<<<< HEAD
#if IS_ENABLED(CONFIG_MTK_USB_OFFLOAD)
	struct platform_device *offload_pdev;
	struct device_node *offload_node;
	struct xhci_vendor_ops *ops;
#endif
	int ret = -ENODEV;
=======
	int ret = -ENODEV;
	int wakeup_irq;
>>>>>>> upstream/android-13
	int irq;

	if (usb_disabled())
		return -ENODEV;

	driver = &xhci_mtk_hc_driver;
	mtk = devm_kzalloc(dev, sizeof(*mtk), GFP_KERNEL);
	if (!mtk)
		return -ENOMEM;

	mtk->dev = dev;
	mtk->vbus = devm_regulator_get(dev, "vbus");
	if (IS_ERR(mtk->vbus)) {
		dev_err(dev, "fail to get vbus\n");
		return PTR_ERR(mtk->vbus);
	}

	mtk->vusb33 = devm_regulator_get(dev, "vusb33");
	if (IS_ERR(mtk->vusb33)) {
		dev_err(dev, "fail to get vusb33\n");
		return PTR_ERR(mtk->vusb33);
	}

	ret = xhci_mtk_clks_get(mtk);
	if (ret)
		return ret;

<<<<<<< HEAD
=======
	irq = platform_get_irq_byname_optional(pdev, "host");
	if (irq < 0) {
		if (irq == -EPROBE_DEFER)
			return irq;

		/* for backward compatibility */
		irq = platform_get_irq(pdev, 0);
		if (irq < 0)
			return irq;
	}

	wakeup_irq = platform_get_irq_byname_optional(pdev, "wakeup");
	if (wakeup_irq == -EPROBE_DEFER)
		return wakeup_irq;

>>>>>>> upstream/android-13
	mtk->lpm_support = of_property_read_bool(node, "usb3-lpm-capable");
	mtk->u2_lpm_disable = of_property_read_bool(node, "usb2-lpm-disable");
	/* optional property, ignore the error if it does not exist */
	of_property_read_u32(node, "mediatek,u3p-dis-msk",
			     &mtk->u3p_dis_msk);
<<<<<<< HEAD
=======
	of_property_read_u32(node, "mediatek,u2p-dis-msk",
			     &mtk->u2p_dis_msk);
>>>>>>> upstream/android-13

	ret = usb_wakeup_of_property_parse(mtk, node);
	if (ret) {
		dev_err(dev, "failed to parse uwk property\n");
		return ret;
	}

<<<<<<< HEAD
	pm_runtime_enable(dev);
	pm_runtime_get_sync(dev);
	device_enable_async_suspend(dev);
=======
	pm_runtime_set_active(dev);
	pm_runtime_use_autosuspend(dev);
	pm_runtime_set_autosuspend_delay(dev, 4000);
	pm_runtime_enable(dev);
	pm_runtime_get_sync(dev);
>>>>>>> upstream/android-13

	ret = xhci_mtk_ldos_enable(mtk);
	if (ret)
		goto disable_pm;

<<<<<<< HEAD
	ret = xhci_mtk_clks_enable(mtk);
	if (ret)
		goto disable_ldos;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		ret = irq;
		goto disable_clk;
	}

	/* Initialize dma_mask and coherent_dma_mask to 32-bits */
	ret = dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32));
	if (ret)
		goto disable_clk;

=======
	ret = clk_bulk_prepare_enable(BULK_CLKS_NUM, mtk->clks);
	if (ret)
		goto disable_ldos;

>>>>>>> upstream/android-13
	hcd = usb_create_hcd(driver, dev, dev_name(dev));
	if (!hcd) {
		ret = -ENOMEM;
		goto disable_clk;
	}

	/*
	 * USB 2.0 roothub is stored in the platform_device.
	 * Swap it with mtk HCD.
	 */
	mtk->hcd = platform_get_drvdata(pdev);
	platform_set_drvdata(pdev, mtk);

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "mac");
	hcd->regs = devm_ioremap_resource(dev, res);
	if (IS_ERR(hcd->regs)) {
		ret = PTR_ERR(hcd->regs);
		goto put_usb2_hcd;
	}
	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "ippc");
	if (res) {	/* ippc register is optional */
<<<<<<< HEAD
		mtk->ippc_regs = devm_ioremap(dev, res->start, resource_size(res));
=======
		mtk->ippc_regs = devm_ioremap_resource(dev, res);
>>>>>>> upstream/android-13
		if (IS_ERR(mtk->ippc_regs)) {
			ret = PTR_ERR(mtk->ippc_regs);
			goto put_usb2_hcd;
		}
		mtk->has_ippc = true;
<<<<<<< HEAD
	} else {
		mtk->has_ippc = false;
=======
>>>>>>> upstream/android-13
	}

	device_init_wakeup(dev, true);

	xhci = hcd_to_xhci(hcd);
	xhci->main_hcd = hcd;

<<<<<<< HEAD
#if IS_ENABLED(CONFIG_MTK_USB_OFFLOAD)
	/* get vendor_ops data */
	offload_node = of_parse_phandle(node, "mediatek,usb-offload", 0);
	if (offload_node) {
		pr_info("%s %d\n", __func__, __LINE__);
		offload_pdev = of_find_device_by_node(offload_node);
		of_node_put(offload_node);
		if (offload_pdev) {
			ops = dev_get_drvdata(&offload_pdev->dev);
			if (ops) {
				dev_info(dev, "set offload vendor_ops data\n");
				xhci->vendor_ops = ops;
			} else
				dev_info(dev, "failed to get offload data\n");
		} else
			dev_info(dev, "failed to get offload platform device\n");
	}
#endif

=======
>>>>>>> upstream/android-13
	/*
	 * imod_interval is the interrupt moderation value in nanoseconds.
	 * The increment interval is 8 times as much as that defined in
	 * the xHCI spec on MTK's controller.
	 */
	xhci->imod_interval = 5000;
	device_property_read_u32(dev, "imod-interval-ns", &xhci->imod_interval);

	xhci->shared_hcd = usb_create_shared_hcd(driver, dev,
			dev_name(dev), hcd);
	if (!xhci->shared_hcd) {
		ret = -ENOMEM;
		goto disable_device_wakeup;
	}

	ret = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (ret)
		goto put_usb3_hcd;

	if (HCC_MAX_PSA(xhci->hcc_params) >= 4 &&
	    !(xhci->quirks & XHCI_BROKEN_STREAMS))
		xhci->shared_hcd->can_do_streams = 1;

	ret = usb_add_hcd(xhci->shared_hcd, irq, IRQF_SHARED);
	if (ret)
		goto dealloc_usb2_hcd;

<<<<<<< HEAD
	xhci_mtk_dbg_init(mtk);

	device_set_wakeup_enable(&hcd->self.root_hub->dev, 1);
	device_set_wakeup_enable(&xhci->shared_hcd->self.root_hub->dev, 1);
	mtk_xhci_wakelock_lock(mtk);

	return 0;

=======
	if (wakeup_irq > 0) {
		ret = dev_pm_set_dedicated_wake_irq(dev, wakeup_irq);
		if (ret) {
			dev_err(dev, "set wakeup irq %d failed\n", wakeup_irq);
			goto dealloc_usb3_hcd;
		}
		dev_info(dev, "wakeup irq %d\n", wakeup_irq);
	}

	device_enable_async_suspend(dev);
	pm_runtime_mark_last_busy(dev);
	pm_runtime_put_autosuspend(dev);
	pm_runtime_forbid(dev);

	return 0;

dealloc_usb3_hcd:
	usb_remove_hcd(xhci->shared_hcd);
	xhci->shared_hcd = NULL;

>>>>>>> upstream/android-13
dealloc_usb2_hcd:
	usb_remove_hcd(hcd);

put_usb3_hcd:
	xhci_mtk_sch_exit(mtk);
	usb_put_hcd(xhci->shared_hcd);

disable_device_wakeup:
	device_init_wakeup(dev, false);

put_usb2_hcd:
	usb_put_hcd(hcd);

disable_clk:
<<<<<<< HEAD
	xhci_mtk_clks_disable(mtk);
=======
	clk_bulk_disable_unprepare(BULK_CLKS_NUM, mtk->clks);
>>>>>>> upstream/android-13

disable_ldos:
	xhci_mtk_ldos_disable(mtk);

disable_pm:
<<<<<<< HEAD
	pm_runtime_put_sync(dev);
=======
	pm_runtime_put_noidle(dev);
>>>>>>> upstream/android-13
	pm_runtime_disable(dev);
	return ret;
}

<<<<<<< HEAD
static int xhci_mtk_remove(struct platform_device *dev)
{
	struct xhci_hcd_mtk *mtk = platform_get_drvdata(dev);
	struct usb_hcd	*hcd = mtk->hcd;
	struct xhci_hcd	*xhci = hcd_to_xhci(hcd);
	struct usb_hcd  *shared_hcd = xhci->shared_hcd;

	pm_runtime_put_noidle(&dev->dev);
	pm_runtime_disable(&dev->dev);
#if defined(CONFIG_USB_HOST_SAMSUNG_FEATURE)
	/* In order to prevent kernel panic */
	if (!pm_runtime_suspended(&xhci->shared_hcd->self.root_hub->dev)) {
		pr_info("%s, shared_hcd pm_runtime_forbid\n", __func__);
		pm_runtime_forbid(&xhci->shared_hcd->self.root_hub->dev);
	}
	if (!pm_runtime_suspended(&xhci->main_hcd->self.root_hub->dev)) {
		pr_info("%s, main_hcd pm_runtime_forbid\n", __func__);
		pm_runtime_forbid(&xhci->main_hcd->self.root_hub->dev);
	}
#endif
	xhci->xhc_state |= XHCI_STATE_REMOVING;
#ifdef CONFIG_USB_DEBUG_DETAILED_LOG
	pr_info("%s - remove hcd (shared_hcd)\n", __func__);
#endif
	usb_remove_hcd(shared_hcd);
	xhci->shared_hcd = NULL;
	device_init_wakeup(&dev->dev, false);

	mtk_xhci_wakelock_unlock(mtk);
#ifdef CONFIG_USB_DEBUG_DETAILED_LOG
	pr_info("%s - remove hcd (main)\n", __func__);
#endif
=======
static int xhci_mtk_remove(struct platform_device *pdev)
{
	struct xhci_hcd_mtk *mtk = platform_get_drvdata(pdev);
	struct usb_hcd	*hcd = mtk->hcd;
	struct xhci_hcd	*xhci = hcd_to_xhci(hcd);
	struct usb_hcd  *shared_hcd = xhci->shared_hcd;
	struct device *dev = &pdev->dev;

	pm_runtime_get_sync(dev);
	xhci->xhc_state |= XHCI_STATE_REMOVING;
	dev_pm_clear_wake_irq(dev);
	device_init_wakeup(dev, false);

	usb_remove_hcd(shared_hcd);
	xhci->shared_hcd = NULL;
>>>>>>> upstream/android-13
	usb_remove_hcd(hcd);
	usb_put_hcd(shared_hcd);
	usb_put_hcd(hcd);
	xhci_mtk_sch_exit(mtk);
<<<<<<< HEAD
	xhci_mtk_clks_disable(mtk);
	xhci_mtk_ldos_disable(mtk);
	xhci_mtk_dbg_exit(mtk);
=======
	clk_bulk_disable_unprepare(BULK_CLKS_NUM, mtk->clks);
	xhci_mtk_ldos_disable(mtk);

	pm_runtime_disable(dev);
	pm_runtime_put_noidle(dev);
	pm_runtime_set_suspended(dev);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
/*
 * if ip sleep fails, and all clocks are disabled, access register will hang
 * AHB bus, so stop polling roothubs to avoid regs access on bus suspend.
 * and no need to check whether ip sleep failed or not; this will cause SPM
 * to wake up system immediately after system suspend complete if ip sleep
 * fails, it is what we wanted.
 */
=======
>>>>>>> upstream/android-13
static int __maybe_unused xhci_mtk_suspend(struct device *dev)
{
	struct xhci_hcd_mtk *mtk = dev_get_drvdata(dev);
	struct usb_hcd *hcd = mtk->hcd;
	struct xhci_hcd *xhci = hcd_to_xhci(hcd);
<<<<<<< HEAD

#if IS_ENABLED(CONFIG_MTK_USB_OFFLOAD)
	if (xhci_vendor_is_streaming(xhci)) {
		pr_info("%s - USB_OFFLOAD bypass\n", __func__);
		return 0;
	}
#endif
=======
	int ret;
>>>>>>> upstream/android-13

	xhci_dbg(xhci, "%s: stop port polling\n", __func__);
	clear_bit(HCD_FLAG_POLL_RH, &hcd->flags);
	del_timer_sync(&hcd->rh_timer);
	clear_bit(HCD_FLAG_POLL_RH, &xhci->shared_hcd->flags);
	del_timer_sync(&xhci->shared_hcd->rh_timer);

<<<<<<< HEAD
	xhci_mtk_host_disable(mtk);
	xhci_mtk_clks_disable(mtk);
	usb_wakeup_set(mtk, true);
	return 0;
=======
	ret = xhci_mtk_host_disable(mtk);
	if (ret)
		goto restart_poll_rh;

	clk_bulk_disable_unprepare(BULK_CLKS_NUM, mtk->clks);
	usb_wakeup_set(mtk, true);
	return 0;

restart_poll_rh:
	xhci_dbg(xhci, "%s: restart port polling\n", __func__);
	set_bit(HCD_FLAG_POLL_RH, &xhci->shared_hcd->flags);
	usb_hcd_poll_rh_status(xhci->shared_hcd);
	set_bit(HCD_FLAG_POLL_RH, &hcd->flags);
	usb_hcd_poll_rh_status(hcd);
	return ret;
>>>>>>> upstream/android-13
}

static int __maybe_unused xhci_mtk_resume(struct device *dev)
{
	struct xhci_hcd_mtk *mtk = dev_get_drvdata(dev);
	struct usb_hcd *hcd = mtk->hcd;
	struct xhci_hcd *xhci = hcd_to_xhci(hcd);
<<<<<<< HEAD

#if IS_ENABLED(CONFIG_MTK_USB_OFFLOAD)
	if (xhci_vendor_is_streaming(xhci)) {
		pr_info("%s - USB_OFFLOAD bypass\n", __func__);
		return 0;
	}
#endif

	usb_wakeup_set(mtk, false);
	xhci_mtk_clks_enable(mtk);
	xhci_mtk_host_enable(mtk);
=======
	int ret;

	usb_wakeup_set(mtk, false);
	ret = clk_bulk_prepare_enable(BULK_CLKS_NUM, mtk->clks);
	if (ret)
		goto enable_wakeup;

	ret = xhci_mtk_host_enable(mtk);
	if (ret)
		goto disable_clks;
>>>>>>> upstream/android-13

	xhci_dbg(xhci, "%s: restart port polling\n", __func__);
	set_bit(HCD_FLAG_POLL_RH, &xhci->shared_hcd->flags);
	usb_hcd_poll_rh_status(xhci->shared_hcd);
	set_bit(HCD_FLAG_POLL_RH, &hcd->flags);
	usb_hcd_poll_rh_status(hcd);
	return 0;
<<<<<<< HEAD
=======

disable_clks:
	clk_bulk_disable_unprepare(BULK_CLKS_NUM, mtk->clks);
enable_wakeup:
	usb_wakeup_set(mtk, true);
	return ret;
}

static int __maybe_unused xhci_mtk_runtime_suspend(struct device *dev)
{
	struct xhci_hcd_mtk  *mtk = dev_get_drvdata(dev);
	struct xhci_hcd *xhci = hcd_to_xhci(mtk->hcd);
	int ret = 0;

	if (xhci->xhc_state)
		return -ESHUTDOWN;

	if (device_may_wakeup(dev))
		ret = xhci_mtk_suspend(dev);

	/* -EBUSY: let PM automatically reschedule another autosuspend */
	return ret ? -EBUSY : 0;
}

static int __maybe_unused xhci_mtk_runtime_resume(struct device *dev)
{
	struct xhci_hcd_mtk  *mtk = dev_get_drvdata(dev);
	struct xhci_hcd *xhci = hcd_to_xhci(mtk->hcd);
	int ret = 0;

	if (xhci->xhc_state)
		return -ESHUTDOWN;

	if (device_may_wakeup(dev))
		ret = xhci_mtk_resume(dev);

	return ret;
>>>>>>> upstream/android-13
}

static const struct dev_pm_ops xhci_mtk_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(xhci_mtk_suspend, xhci_mtk_resume)
<<<<<<< HEAD
};
#define DEV_PM_OPS IS_ENABLED(CONFIG_PM) ? &xhci_mtk_pm_ops : NULL

#ifdef CONFIG_OF
static const struct of_device_id mtk_xhci_of_match[] = {
	{ .compatible = "mediatek,mt8173-xhci"},
=======
	SET_RUNTIME_PM_OPS(xhci_mtk_runtime_suspend,
			   xhci_mtk_runtime_resume, NULL)
};

#define DEV_PM_OPS (IS_ENABLED(CONFIG_PM) ? &xhci_mtk_pm_ops : NULL)

static const struct of_device_id mtk_xhci_of_match[] = {
	{ .compatible = "mediatek,mt8173-xhci"},
	{ .compatible = "mediatek,mt8195-xhci"},
>>>>>>> upstream/android-13
	{ .compatible = "mediatek,mtk-xhci"},
	{ },
};
MODULE_DEVICE_TABLE(of, mtk_xhci_of_match);
<<<<<<< HEAD
#endif
=======
>>>>>>> upstream/android-13

static struct platform_driver mtk_xhci_driver = {
	.probe	= xhci_mtk_probe,
	.remove	= xhci_mtk_remove,
	.driver	= {
		.name = "xhci-mtk",
		.pm = DEV_PM_OPS,
<<<<<<< HEAD
		.of_match_table = of_match_ptr(mtk_xhci_of_match),
	},
};
MODULE_ALIAS("platform:xhci-mtk");
=======
		.of_match_table = mtk_xhci_of_match,
	},
};
>>>>>>> upstream/android-13

static int __init xhci_mtk_init(void)
{
	xhci_init_driver(&xhci_mtk_hc_driver, &xhci_mtk_overrides);
	return platform_driver_register(&mtk_xhci_driver);
}
module_init(xhci_mtk_init);

static void __exit xhci_mtk_exit(void)
{
	platform_driver_unregister(&mtk_xhci_driver);
}
module_exit(xhci_mtk_exit);

MODULE_AUTHOR("Chunfeng Yun <chunfeng.yun@mediatek.com>");
MODULE_DESCRIPTION("MediaTek xHCI Host Controller Driver");
MODULE_LICENSE("GPL v2");
