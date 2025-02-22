// SPDX-License-Identifier: GPL-2.0
/*
 * 8250_lpss.c - Driver for UART on Intel Braswell and various other Intel SoCs
 *
 * Copyright (C) 2016 Intel Corporation
 * Author: Andy Shevchenko <andriy.shevchenko@linux.intel.com>
 */

#include <linux/bitops.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/rational.h>

#include <linux/dmaengine.h>
#include <linux/dma/dw.h>

<<<<<<< HEAD
#include "8250.h"
=======
#include "8250_dwlib.h"
>>>>>>> upstream/android-13

#define PCI_DEVICE_ID_INTEL_QRK_UARTx	0x0936

#define PCI_DEVICE_ID_INTEL_BYT_UART1	0x0f0a
#define PCI_DEVICE_ID_INTEL_BYT_UART2	0x0f0c

#define PCI_DEVICE_ID_INTEL_BSW_UART1	0x228a
#define PCI_DEVICE_ID_INTEL_BSW_UART2	0x228c

<<<<<<< HEAD
=======
#define PCI_DEVICE_ID_INTEL_EHL_UART0	0x4b96
#define PCI_DEVICE_ID_INTEL_EHL_UART1	0x4b97
#define PCI_DEVICE_ID_INTEL_EHL_UART2	0x4b98
#define PCI_DEVICE_ID_INTEL_EHL_UART3	0x4b99
#define PCI_DEVICE_ID_INTEL_EHL_UART4	0x4b9a
#define PCI_DEVICE_ID_INTEL_EHL_UART5	0x4b9b

>>>>>>> upstream/android-13
#define PCI_DEVICE_ID_INTEL_BDW_UART1	0x9ce3
#define PCI_DEVICE_ID_INTEL_BDW_UART2	0x9ce4

/* Intel LPSS specific registers */

#define BYT_PRV_CLK			0x800
#define BYT_PRV_CLK_EN			BIT(0)
#define BYT_PRV_CLK_M_VAL_SHIFT		1
#define BYT_PRV_CLK_N_VAL_SHIFT		16
#define BYT_PRV_CLK_UPDATE		BIT(31)

#define BYT_TX_OVF_INT			0x820
#define BYT_TX_OVF_INT_MASK		BIT(1)

struct lpss8250;

struct lpss8250_board {
	unsigned long freq;
	unsigned int base_baud;
	int (*setup)(struct lpss8250 *, struct uart_port *p);
	void (*exit)(struct lpss8250 *);
};

struct lpss8250 {
<<<<<<< HEAD
	int line;
	struct lpss8250_board *board;

	/* DMA parameters */
	struct uart_8250_dma dma;
=======
	struct dw8250_port_data data;
	struct lpss8250_board *board;

	/* DMA parameters */
>>>>>>> upstream/android-13
	struct dw_dma_chip dma_chip;
	struct dw_dma_slave dma_param;
	u8 dma_maxburst;
};

<<<<<<< HEAD
=======
static inline struct lpss8250 *to_lpss8250(struct dw8250_port_data *data)
{
	return container_of(data, struct lpss8250, data);
}

>>>>>>> upstream/android-13
static void byt_set_termios(struct uart_port *p, struct ktermios *termios,
			    struct ktermios *old)
{
	unsigned int baud = tty_termios_baud_rate(termios);
<<<<<<< HEAD
	struct lpss8250 *lpss = p->private_data;
=======
	struct lpss8250 *lpss = to_lpss8250(p->private_data);
>>>>>>> upstream/android-13
	unsigned long fref = lpss->board->freq, fuart = baud * 16;
	unsigned long w = BIT(15) - 1;
	unsigned long m, n;
	u32 reg;

	/* Gracefully handle the B0 case: fall back to B9600 */
	fuart = fuart ? fuart : 9600 * 16;

	/* Get Fuart closer to Fref */
	fuart *= rounddown_pow_of_two(fref / fuart);

	/*
	 * For baud rates 0.5M, 1M, 1.5M, 2M, 2.5M, 3M, 3.5M and 4M the
	 * dividers must be adjusted.
	 *
	 * uartclk = (m / n) * 100 MHz, where m <= n
	 */
	rational_best_approximation(fuart, fref, w, w, &m, &n);
	p->uartclk = fuart;

	/* Reset the clock */
	reg = (m << BYT_PRV_CLK_M_VAL_SHIFT) | (n << BYT_PRV_CLK_N_VAL_SHIFT);
	writel(reg, p->membase + BYT_PRV_CLK);
	reg |= BYT_PRV_CLK_EN | BYT_PRV_CLK_UPDATE;
	writel(reg, p->membase + BYT_PRV_CLK);

	p->status &= ~UPSTAT_AUTOCTS;
	if (termios->c_cflag & CRTSCTS)
		p->status |= UPSTAT_AUTOCTS;

	serial8250_do_set_termios(p, termios, old);
}

static unsigned int byt_get_mctrl(struct uart_port *port)
{
	unsigned int ret = serial8250_do_get_mctrl(port);

	/* Force DCD and DSR signals to permanently be reported as active */
	ret |= TIOCM_CAR | TIOCM_DSR;

	return ret;
}

static int byt_serial_setup(struct lpss8250 *lpss, struct uart_port *port)
{
	struct dw_dma_slave *param = &lpss->dma_param;
<<<<<<< HEAD
	struct uart_8250_port *up = up_to_u8250p(port);
	struct pci_dev *pdev = to_pci_dev(port->dev);
	unsigned int dma_devfn = PCI_DEVFN(PCI_SLOT(pdev->devfn), 0);
	struct pci_dev *dma_dev = pci_get_slot(pdev->bus, dma_devfn);
=======
	struct pci_dev *pdev = to_pci_dev(port->dev);
	struct pci_dev *dma_dev;
>>>>>>> upstream/android-13

	switch (pdev->device) {
	case PCI_DEVICE_ID_INTEL_BYT_UART1:
	case PCI_DEVICE_ID_INTEL_BSW_UART1:
	case PCI_DEVICE_ID_INTEL_BDW_UART1:
		param->src_id = 3;
		param->dst_id = 2;
		break;
	case PCI_DEVICE_ID_INTEL_BYT_UART2:
	case PCI_DEVICE_ID_INTEL_BSW_UART2:
	case PCI_DEVICE_ID_INTEL_BDW_UART2:
		param->src_id = 5;
		param->dst_id = 4;
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
=======
	dma_dev = pci_get_slot(pdev->bus, PCI_DEVFN(PCI_SLOT(pdev->devfn), 0));

>>>>>>> upstream/android-13
	param->dma_dev = &dma_dev->dev;
	param->m_master = 0;
	param->p_master = 1;

<<<<<<< HEAD
	/* TODO: Detect FIFO size automaticaly for DesignWare 8250 */
	port->fifosize = 64;
	up->tx_loadsz = 64;

=======
>>>>>>> upstream/android-13
	lpss->dma_maxburst = 16;

	port->set_termios = byt_set_termios;
	port->get_mctrl = byt_get_mctrl;

	/* Disable TX counter interrupts */
	writel(BYT_TX_OVF_INT_MASK, port->membase + BYT_TX_OVF_INT);

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_SERIAL_8250_DMA
static const struct dw_dma_platform_data qrk_serial_dma_pdata = {
	.nr_channels = 2,
	.is_private = true,
=======
static void byt_serial_exit(struct lpss8250 *lpss)
{
	struct dw_dma_slave *param = &lpss->dma_param;

	/* Paired with pci_get_slot() in the byt_serial_setup() above */
	put_device(param->dma_dev);
}

static int ehl_serial_setup(struct lpss8250 *lpss, struct uart_port *port)
{
	struct uart_8250_dma *dma = &lpss->data.dma;
	struct uart_8250_port *up = up_to_u8250p(port);

	/*
	 * This simply makes the checks in the 8250_port to try the DMA
	 * channel request which in turn uses the magic of ACPI tables
	 * parsing (see drivers/dma/acpi-dma.c for the details) and
	 * matching with the registered General Purpose DMA controllers.
	 */
	up->dma = dma;
	return 0;
}

static void ehl_serial_exit(struct lpss8250 *lpss)
{
	struct uart_8250_port *up = serial8250_get_port(lpss->data.line);

	up->dma = NULL;
}

#ifdef CONFIG_SERIAL_8250_DMA
static const struct dw_dma_platform_data qrk_serial_dma_pdata = {
	.nr_channels = 2,
>>>>>>> upstream/android-13
	.chan_allocation_order = CHAN_ALLOCATION_ASCENDING,
	.chan_priority = CHAN_PRIORITY_ASCENDING,
	.block_size = 4095,
	.nr_masters = 1,
	.data_width = {4},
	.multi_block = {0},
};

static void qrk_serial_setup_dma(struct lpss8250 *lpss, struct uart_port *port)
{
<<<<<<< HEAD
	struct uart_8250_dma *dma = &lpss->dma;
=======
	struct uart_8250_dma *dma = &lpss->data.dma;
>>>>>>> upstream/android-13
	struct dw_dma_chip *chip = &lpss->dma_chip;
	struct dw_dma_slave *param = &lpss->dma_param;
	struct pci_dev *pdev = to_pci_dev(port->dev);
	int ret;

<<<<<<< HEAD
	chip->dev = &pdev->dev;
	chip->irq = pci_irq_vector(pdev, 0);
	chip->regs = pci_ioremap_bar(pdev, 1);
	chip->pdata = &qrk_serial_dma_pdata;
=======
	chip->pdata = &qrk_serial_dma_pdata;
	chip->dev = &pdev->dev;
	chip->id = pdev->devfn;
	chip->irq = pci_irq_vector(pdev, 0);
	chip->regs = pci_ioremap_bar(pdev, 1);
	if (!chip->regs)
		return;
>>>>>>> upstream/android-13

	/* Falling back to PIO mode if DMA probing fails */
	ret = dw_dma_probe(chip);
	if (ret)
		return;

	pci_try_set_mwi(pdev);

	/* Special DMA address for UART */
	dma->rx_dma_addr = 0xfffff000;
	dma->tx_dma_addr = 0xfffff000;

	param->dma_dev = &pdev->dev;
	param->src_id = 0;
	param->dst_id = 1;
	param->hs_polarity = true;

	lpss->dma_maxburst = 8;
}

static void qrk_serial_exit_dma(struct lpss8250 *lpss)
{
<<<<<<< HEAD
=======
	struct dw_dma_chip *chip = &lpss->dma_chip;
>>>>>>> upstream/android-13
	struct dw_dma_slave *param = &lpss->dma_param;

	if (!param->dma_dev)
		return;
<<<<<<< HEAD
	dw_dma_remove(&lpss->dma_chip);
=======

	dw_dma_remove(chip);

	pci_iounmap(to_pci_dev(chip->dev), chip->regs);
>>>>>>> upstream/android-13
}
#else	/* CONFIG_SERIAL_8250_DMA */
static void qrk_serial_setup_dma(struct lpss8250 *lpss, struct uart_port *port) {}
static void qrk_serial_exit_dma(struct lpss8250 *lpss) {}
#endif	/* !CONFIG_SERIAL_8250_DMA */

static int qrk_serial_setup(struct lpss8250 *lpss, struct uart_port *port)
{
<<<<<<< HEAD
	struct pci_dev *pdev = to_pci_dev(port->dev);
	int ret;

	pci_set_master(pdev);

	ret = pci_alloc_irq_vectors(pdev, 1, 1, PCI_IRQ_ALL_TYPES);
	if (ret < 0)
		return ret;

	port->irq = pci_irq_vector(pdev, 0);

=======
>>>>>>> upstream/android-13
	qrk_serial_setup_dma(lpss, port);
	return 0;
}

static void qrk_serial_exit(struct lpss8250 *lpss)
{
	qrk_serial_exit_dma(lpss);
}

static bool lpss8250_dma_filter(struct dma_chan *chan, void *param)
{
	struct dw_dma_slave *dws = param;

	if (dws->dma_dev != chan->device->dev)
		return false;

	chan->private = dws;
	return true;
}

static int lpss8250_dma_setup(struct lpss8250 *lpss, struct uart_8250_port *port)
{
<<<<<<< HEAD
	struct uart_8250_dma *dma = &lpss->dma;
=======
	struct uart_8250_dma *dma = &lpss->data.dma;
>>>>>>> upstream/android-13
	struct dw_dma_slave *rx_param, *tx_param;
	struct device *dev = port->port.dev;

	if (!lpss->dma_param.dma_dev)
		return 0;

	rx_param = devm_kzalloc(dev, sizeof(*rx_param), GFP_KERNEL);
	if (!rx_param)
		return -ENOMEM;

	tx_param = devm_kzalloc(dev, sizeof(*tx_param), GFP_KERNEL);
	if (!tx_param)
		return -ENOMEM;

	*rx_param = lpss->dma_param;
	dma->rxconf.src_maxburst = lpss->dma_maxburst;

	*tx_param = lpss->dma_param;
	dma->txconf.dst_maxburst = lpss->dma_maxburst;

	dma->fn = lpss8250_dma_filter;
	dma->rx_param = rx_param;
	dma->tx_param = tx_param;

	port->dma = dma;
	return 0;
}

static int lpss8250_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
	struct uart_8250_port uart;
	struct lpss8250 *lpss;
	int ret;

	ret = pcim_enable_device(pdev);
	if (ret)
		return ret;

<<<<<<< HEAD
=======
	pci_set_master(pdev);

>>>>>>> upstream/android-13
	lpss = devm_kzalloc(&pdev->dev, sizeof(*lpss), GFP_KERNEL);
	if (!lpss)
		return -ENOMEM;

<<<<<<< HEAD
=======
	ret = pci_alloc_irq_vectors(pdev, 1, 1, PCI_IRQ_ALL_TYPES);
	if (ret < 0)
		return ret;

>>>>>>> upstream/android-13
	lpss->board = (struct lpss8250_board *)id->driver_data;

	memset(&uart, 0, sizeof(struct uart_8250_port));

	uart.port.dev = &pdev->dev;
<<<<<<< HEAD
	uart.port.irq = pdev->irq;
	uart.port.private_data = lpss;
=======
	uart.port.irq = pci_irq_vector(pdev, 0);
	uart.port.private_data = &lpss->data;
>>>>>>> upstream/android-13
	uart.port.type = PORT_16550A;
	uart.port.iotype = UPIO_MEM;
	uart.port.regshift = 2;
	uart.port.uartclk = lpss->board->base_baud * 16;
	uart.port.flags = UPF_SHARE_IRQ | UPF_FIXED_PORT | UPF_FIXED_TYPE;
	uart.capabilities = UART_CAP_FIFO | UART_CAP_AFE;
	uart.port.mapbase = pci_resource_start(pdev, 0);
	uart.port.membase = pcim_iomap(pdev, 0, 0);
	if (!uart.port.membase)
		return -ENOMEM;

	ret = lpss->board->setup(lpss, &uart.port);
	if (ret)
		return ret;

<<<<<<< HEAD
=======
	dw8250_setup_port(&uart.port);

>>>>>>> upstream/android-13
	ret = lpss8250_dma_setup(lpss, &uart);
	if (ret)
		goto err_exit;

	ret = serial8250_register_8250_port(&uart);
	if (ret < 0)
		goto err_exit;

<<<<<<< HEAD
	lpss->line = ret;
=======
	lpss->data.line = ret;
>>>>>>> upstream/android-13

	pci_set_drvdata(pdev, lpss);
	return 0;

err_exit:
<<<<<<< HEAD
	if (lpss->board->exit)
		lpss->board->exit(lpss);
=======
	lpss->board->exit(lpss);
	pci_free_irq_vectors(pdev);
>>>>>>> upstream/android-13
	return ret;
}

static void lpss8250_remove(struct pci_dev *pdev)
{
	struct lpss8250 *lpss = pci_get_drvdata(pdev);

<<<<<<< HEAD
	serial8250_unregister_port(lpss->line);

	if (lpss->board->exit)
		lpss->board->exit(lpss);
=======
	serial8250_unregister_port(lpss->data.line);

	lpss->board->exit(lpss);
	pci_free_irq_vectors(pdev);
>>>>>>> upstream/android-13
}

static const struct lpss8250_board byt_board = {
	.freq = 100000000,
	.base_baud = 2764800,
	.setup = byt_serial_setup,
<<<<<<< HEAD
=======
	.exit = byt_serial_exit,
};

static const struct lpss8250_board ehl_board = {
	.freq = 200000000,
	.base_baud = 12500000,
	.setup = ehl_serial_setup,
	.exit = ehl_serial_exit,
>>>>>>> upstream/android-13
};

static const struct lpss8250_board qrk_board = {
	.freq = 44236800,
	.base_baud = 2764800,
	.setup = qrk_serial_setup,
	.exit = qrk_serial_exit,
};

<<<<<<< HEAD
#define LPSS_DEVICE(id, board) { PCI_VDEVICE(INTEL, id), (kernel_ulong_t)&board }

static const struct pci_device_id pci_ids[] = {
	LPSS_DEVICE(PCI_DEVICE_ID_INTEL_QRK_UARTx, qrk_board),
	LPSS_DEVICE(PCI_DEVICE_ID_INTEL_BYT_UART1, byt_board),
	LPSS_DEVICE(PCI_DEVICE_ID_INTEL_BYT_UART2, byt_board),
	LPSS_DEVICE(PCI_DEVICE_ID_INTEL_BSW_UART1, byt_board),
	LPSS_DEVICE(PCI_DEVICE_ID_INTEL_BSW_UART2, byt_board),
	LPSS_DEVICE(PCI_DEVICE_ID_INTEL_BDW_UART1, byt_board),
	LPSS_DEVICE(PCI_DEVICE_ID_INTEL_BDW_UART2, byt_board),
	{ },
=======
static const struct pci_device_id pci_ids[] = {
	{ PCI_DEVICE_DATA(INTEL, QRK_UARTx, &qrk_board) },
	{ PCI_DEVICE_DATA(INTEL, EHL_UART0, &ehl_board) },
	{ PCI_DEVICE_DATA(INTEL, EHL_UART1, &ehl_board) },
	{ PCI_DEVICE_DATA(INTEL, EHL_UART2, &ehl_board) },
	{ PCI_DEVICE_DATA(INTEL, EHL_UART3, &ehl_board) },
	{ PCI_DEVICE_DATA(INTEL, EHL_UART4, &ehl_board) },
	{ PCI_DEVICE_DATA(INTEL, EHL_UART5, &ehl_board) },
	{ PCI_DEVICE_DATA(INTEL, BYT_UART1, &byt_board) },
	{ PCI_DEVICE_DATA(INTEL, BYT_UART2, &byt_board) },
	{ PCI_DEVICE_DATA(INTEL, BSW_UART1, &byt_board) },
	{ PCI_DEVICE_DATA(INTEL, BSW_UART2, &byt_board) },
	{ PCI_DEVICE_DATA(INTEL, BDW_UART1, &byt_board) },
	{ PCI_DEVICE_DATA(INTEL, BDW_UART2, &byt_board) },
	{ }
>>>>>>> upstream/android-13
};
MODULE_DEVICE_TABLE(pci, pci_ids);

static struct pci_driver lpss8250_pci_driver = {
	.name           = "8250_lpss",
	.id_table       = pci_ids,
	.probe          = lpss8250_probe,
	.remove         = lpss8250_remove,
};

module_pci_driver(lpss8250_pci_driver);

MODULE_AUTHOR("Intel Corporation");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Intel LPSS UART driver");
