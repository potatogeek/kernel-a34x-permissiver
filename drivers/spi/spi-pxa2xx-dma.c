<<<<<<< HEAD
/*
 * PXA2xx SPI DMA engine support.
 *
 * Copyright (C) 2013, Intel Corporation
 * Author: Mika Westerberg <mika.westerberg@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * PXA2xx SPI DMA engine support.
 *
 * Copyright (C) 2013, 2021 Intel Corporation
 * Author: Mika Westerberg <mika.westerberg@linux.intel.com>
>>>>>>> upstream/android-13
 */

#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
<<<<<<< HEAD
#include <linux/pxa2xx_ssp.h>
#include <linux/scatterlist.h>
#include <linux/sizes.h>
#include <linux/spi/spi.h>
#include <linux/spi/pxa2xx_spi.h>
=======
#include <linux/scatterlist.h>
#include <linux/sizes.h>

#include <linux/spi/pxa2xx_spi.h>
#include <linux/spi/spi.h>
>>>>>>> upstream/android-13

#include "spi-pxa2xx.h"

static void pxa2xx_spi_dma_transfer_complete(struct driver_data *drv_data,
					     bool error)
{
<<<<<<< HEAD
	struct spi_message *msg = drv_data->master->cur_msg;
=======
	struct spi_message *msg = drv_data->controller->cur_msg;
>>>>>>> upstream/android-13

	/*
	 * It is possible that one CPU is handling ROR interrupt and other
	 * just gets DMA completion. Calling pump_transfers() twice for the
	 * same transfer leads to problems thus we prevent concurrent calls
<<<<<<< HEAD
	 * by using ->dma_running.
=======
	 * by using dma_running.
>>>>>>> upstream/android-13
	 */
	if (atomic_dec_and_test(&drv_data->dma_running)) {
		/*
		 * If the other CPU is still handling the ROR interrupt we
		 * might not know about the error yet. So we re-check the
		 * ROR bit here before we clear the status register.
		 */
<<<<<<< HEAD
		if (!error) {
			u32 status = pxa2xx_spi_read(drv_data, SSSR)
				     & drv_data->mask_sr;
			error = status & SSSR_ROR;
		}

		/* Clear status & disable interrupts */
		pxa2xx_spi_write(drv_data, SSCR1,
				 pxa2xx_spi_read(drv_data, SSCR1)
				 & ~drv_data->dma_cr1);
=======
		if (!error)
			error = read_SSSR_bits(drv_data, drv_data->mask_sr) & SSSR_ROR;

		/* Clear status & disable interrupts */
		clear_SSCR1_bits(drv_data, drv_data->dma_cr1);
>>>>>>> upstream/android-13
		write_SSSR_CS(drv_data, drv_data->clear_sr);
		if (!pxa25x_ssp_comp(drv_data))
			pxa2xx_spi_write(drv_data, SSTO, 0);

		if (error) {
			/* In case we got an error we disable the SSP now */
<<<<<<< HEAD
			pxa2xx_spi_write(drv_data, SSCR0,
					 pxa2xx_spi_read(drv_data, SSCR0)
					 & ~SSCR0_SSE);
			msg->status = -EIO;
		}

		spi_finalize_current_transfer(drv_data->master);
=======
			pxa_ssp_disable(drv_data->ssp);
			msg->status = -EIO;
		}

		spi_finalize_current_transfer(drv_data->controller);
>>>>>>> upstream/android-13
	}
}

static void pxa2xx_spi_dma_callback(void *data)
{
	pxa2xx_spi_dma_transfer_complete(data, false);
}

static struct dma_async_tx_descriptor *
pxa2xx_spi_dma_prepare_one(struct driver_data *drv_data,
			   enum dma_transfer_direction dir,
			   struct spi_transfer *xfer)
{
	struct chip_data *chip =
<<<<<<< HEAD
		spi_get_ctldata(drv_data->master->cur_msg->spi);
=======
		spi_get_ctldata(drv_data->controller->cur_msg->spi);
>>>>>>> upstream/android-13
	enum dma_slave_buswidth width;
	struct dma_slave_config cfg;
	struct dma_chan *chan;
	struct sg_table *sgt;
	int ret;

	switch (drv_data->n_bytes) {
	case 1:
		width = DMA_SLAVE_BUSWIDTH_1_BYTE;
		break;
	case 2:
		width = DMA_SLAVE_BUSWIDTH_2_BYTES;
		break;
	default:
		width = DMA_SLAVE_BUSWIDTH_4_BYTES;
		break;
	}

	memset(&cfg, 0, sizeof(cfg));
	cfg.direction = dir;

	if (dir == DMA_MEM_TO_DEV) {
<<<<<<< HEAD
		cfg.dst_addr = drv_data->ssdr_physical;
=======
		cfg.dst_addr = drv_data->ssp->phys_base + SSDR;
>>>>>>> upstream/android-13
		cfg.dst_addr_width = width;
		cfg.dst_maxburst = chip->dma_burst_size;

		sgt = &xfer->tx_sg;
<<<<<<< HEAD
		chan = drv_data->master->dma_tx;
	} else {
		cfg.src_addr = drv_data->ssdr_physical;
=======
		chan = drv_data->controller->dma_tx;
	} else {
		cfg.src_addr = drv_data->ssp->phys_base + SSDR;
>>>>>>> upstream/android-13
		cfg.src_addr_width = width;
		cfg.src_maxburst = chip->dma_burst_size;

		sgt = &xfer->rx_sg;
<<<<<<< HEAD
		chan = drv_data->master->dma_rx;
=======
		chan = drv_data->controller->dma_rx;
>>>>>>> upstream/android-13
	}

	ret = dmaengine_slave_config(chan, &cfg);
	if (ret) {
<<<<<<< HEAD
		dev_warn(&drv_data->pdev->dev, "DMA slave config failed\n");
=======
		dev_warn(drv_data->ssp->dev, "DMA slave config failed\n");
>>>>>>> upstream/android-13
		return NULL;
	}

	return dmaengine_prep_slave_sg(chan, sgt->sgl, sgt->nents, dir,
				       DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
}

irqreturn_t pxa2xx_spi_dma_transfer(struct driver_data *drv_data)
{
	u32 status;

<<<<<<< HEAD
	status = pxa2xx_spi_read(drv_data, SSSR) & drv_data->mask_sr;
	if (status & SSSR_ROR) {
		dev_err(&drv_data->pdev->dev, "FIFO overrun\n");

		dmaengine_terminate_async(drv_data->master->dma_rx);
		dmaengine_terminate_async(drv_data->master->dma_tx);
=======
	status = read_SSSR_bits(drv_data, drv_data->mask_sr);
	if (status & SSSR_ROR) {
		dev_err(drv_data->ssp->dev, "FIFO overrun\n");

		dmaengine_terminate_async(drv_data->controller->dma_rx);
		dmaengine_terminate_async(drv_data->controller->dma_tx);
>>>>>>> upstream/android-13

		pxa2xx_spi_dma_transfer_complete(drv_data, true);
		return IRQ_HANDLED;
	}

	return IRQ_NONE;
}

int pxa2xx_spi_dma_prepare(struct driver_data *drv_data,
			   struct spi_transfer *xfer)
{
	struct dma_async_tx_descriptor *tx_desc, *rx_desc;
	int err;

	tx_desc = pxa2xx_spi_dma_prepare_one(drv_data, DMA_MEM_TO_DEV, xfer);
	if (!tx_desc) {
<<<<<<< HEAD
		dev_err(&drv_data->pdev->dev,
			"failed to get DMA TX descriptor\n");
=======
		dev_err(drv_data->ssp->dev, "failed to get DMA TX descriptor\n");
>>>>>>> upstream/android-13
		err = -EBUSY;
		goto err_tx;
	}

	rx_desc = pxa2xx_spi_dma_prepare_one(drv_data, DMA_DEV_TO_MEM, xfer);
	if (!rx_desc) {
<<<<<<< HEAD
		dev_err(&drv_data->pdev->dev,
			"failed to get DMA RX descriptor\n");
=======
		dev_err(drv_data->ssp->dev, "failed to get DMA RX descriptor\n");
>>>>>>> upstream/android-13
		err = -EBUSY;
		goto err_rx;
	}

	/* We are ready when RX completes */
	rx_desc->callback = pxa2xx_spi_dma_callback;
	rx_desc->callback_param = drv_data;

	dmaengine_submit(rx_desc);
	dmaengine_submit(tx_desc);
	return 0;

err_rx:
<<<<<<< HEAD
	dmaengine_terminate_async(drv_data->master->dma_tx);
=======
	dmaengine_terminate_async(drv_data->controller->dma_tx);
>>>>>>> upstream/android-13
err_tx:
	return err;
}

void pxa2xx_spi_dma_start(struct driver_data *drv_data)
{
<<<<<<< HEAD
	dma_async_issue_pending(drv_data->master->dma_rx);
	dma_async_issue_pending(drv_data->master->dma_tx);
=======
	dma_async_issue_pending(drv_data->controller->dma_rx);
	dma_async_issue_pending(drv_data->controller->dma_tx);
>>>>>>> upstream/android-13

	atomic_set(&drv_data->dma_running, 1);
}

void pxa2xx_spi_dma_stop(struct driver_data *drv_data)
{
	atomic_set(&drv_data->dma_running, 0);
<<<<<<< HEAD
	dmaengine_terminate_sync(drv_data->master->dma_rx);
	dmaengine_terminate_sync(drv_data->master->dma_tx);
=======
	dmaengine_terminate_sync(drv_data->controller->dma_rx);
	dmaengine_terminate_sync(drv_data->controller->dma_tx);
>>>>>>> upstream/android-13
}

int pxa2xx_spi_dma_setup(struct driver_data *drv_data)
{
<<<<<<< HEAD
	struct pxa2xx_spi_master *pdata = drv_data->master_info;
	struct device *dev = &drv_data->pdev->dev;
	struct spi_controller *master = drv_data->master;
=======
	struct pxa2xx_spi_controller *pdata = drv_data->controller_info;
	struct spi_controller *controller = drv_data->controller;
	struct device *dev = drv_data->ssp->dev;
>>>>>>> upstream/android-13
	dma_cap_mask_t mask;

	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

<<<<<<< HEAD
	master->dma_tx = dma_request_slave_channel_compat(mask,
				pdata->dma_filter, pdata->tx_param, dev, "tx");
	if (!master->dma_tx)
		return -ENODEV;

	master->dma_rx = dma_request_slave_channel_compat(mask,
				pdata->dma_filter, pdata->rx_param, dev, "rx");
	if (!master->dma_rx) {
		dma_release_channel(master->dma_tx);
		master->dma_tx = NULL;
=======
	controller->dma_tx = dma_request_slave_channel_compat(mask,
				pdata->dma_filter, pdata->tx_param, dev, "tx");
	if (!controller->dma_tx)
		return -ENODEV;

	controller->dma_rx = dma_request_slave_channel_compat(mask,
				pdata->dma_filter, pdata->rx_param, dev, "rx");
	if (!controller->dma_rx) {
		dma_release_channel(controller->dma_tx);
		controller->dma_tx = NULL;
>>>>>>> upstream/android-13
		return -ENODEV;
	}

	return 0;
}

void pxa2xx_spi_dma_release(struct driver_data *drv_data)
{
<<<<<<< HEAD
	struct spi_controller *master = drv_data->master;

	if (master->dma_rx) {
		dmaengine_terminate_sync(master->dma_rx);
		dma_release_channel(master->dma_rx);
		master->dma_rx = NULL;
	}
	if (master->dma_tx) {
		dmaengine_terminate_sync(master->dma_tx);
		dma_release_channel(master->dma_tx);
		master->dma_tx = NULL;
=======
	struct spi_controller *controller = drv_data->controller;

	if (controller->dma_rx) {
		dmaengine_terminate_sync(controller->dma_rx);
		dma_release_channel(controller->dma_rx);
		controller->dma_rx = NULL;
	}
	if (controller->dma_tx) {
		dmaengine_terminate_sync(controller->dma_tx);
		dma_release_channel(controller->dma_tx);
		controller->dma_tx = NULL;
>>>>>>> upstream/android-13
	}
}

int pxa2xx_spi_set_dma_burst_and_threshold(struct chip_data *chip,
					   struct spi_device *spi,
					   u8 bits_per_word, u32 *burst_code,
					   u32 *threshold)
{
	struct pxa2xx_spi_chip *chip_info = spi->controller_data;
<<<<<<< HEAD
=======
	struct driver_data *drv_data = spi_controller_get_devdata(spi->controller);
	u32 dma_burst_size = drv_data->controller_info->dma_burst_size;
>>>>>>> upstream/android-13

	/*
	 * If the DMA burst size is given in chip_info we use that,
	 * otherwise we use the default. Also we use the default FIFO
	 * thresholds for now.
	 */
<<<<<<< HEAD
	*burst_code = chip_info ? chip_info->dma_burst_size : 1;
=======
	*burst_code = chip_info ? chip_info->dma_burst_size : dma_burst_size;
>>>>>>> upstream/android-13
	*threshold = SSCR1_RxTresh(RX_THRESH_DFLT)
		   | SSCR1_TxTresh(TX_THRESH_DFLT);

	return 0;
}
