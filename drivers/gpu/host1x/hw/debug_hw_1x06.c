<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2010 Google, Inc.
 * Author: Erik Gilling <konkers@android.com>
 *
 * Copyright (C) 2011-2017 NVIDIA Corporation
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
=======
>>>>>>> upstream/android-13
 */

#include "../dev.h"
#include "../debug.h"
#include "../cdma.h"
#include "../channel.h"

static void host1x_debug_show_channel_cdma(struct host1x *host,
					   struct host1x_channel *ch,
					   struct output *o)
{
	struct host1x_cdma *cdma = &ch->cdma;
<<<<<<< HEAD
=======
	dma_addr_t dmastart = 0, dmaend = 0;
>>>>>>> upstream/android-13
	u32 dmaput, dmaget, dmactrl;
	u32 offset, class;
	u32 ch_stat;

<<<<<<< HEAD
=======
#if defined(CONFIG_ARCH_DMA_ADDR_T_64BIT) && HOST1X_HW >= 6
	dmastart = host1x_ch_readl(ch, HOST1X_CHANNEL_DMASTART_HI);
	dmastart <<= 32;
#endif
	dmastart |= host1x_ch_readl(ch, HOST1X_CHANNEL_DMASTART);

#if defined(CONFIG_ARCH_DMA_ADDR_T_64BIT) && HOST1X_HW >= 6
	dmaend = host1x_ch_readl(ch, HOST1X_CHANNEL_DMAEND_HI);
	dmaend <<= 32;
#endif
	dmaend |= host1x_ch_readl(ch, HOST1X_CHANNEL_DMAEND);

>>>>>>> upstream/android-13
	dmaput = host1x_ch_readl(ch, HOST1X_CHANNEL_DMAPUT);
	dmaget = host1x_ch_readl(ch, HOST1X_CHANNEL_DMAGET);
	dmactrl = host1x_ch_readl(ch, HOST1X_CHANNEL_DMACTRL);
	offset = host1x_ch_readl(ch, HOST1X_CHANNEL_CMDP_OFFSET);
	class = host1x_ch_readl(ch, HOST1X_CHANNEL_CMDP_CLASS);
	ch_stat = host1x_ch_readl(ch, HOST1X_CHANNEL_CHANNELSTAT);

	host1x_debug_output(o, "%u-%s: ", ch->id, dev_name(ch->dev));

	if (dmactrl & HOST1X_CHANNEL_DMACTRL_DMASTOP ||
	    !ch->cdma.push_buffer.mapped) {
		host1x_debug_output(o, "inactive\n\n");
		return;
	}

	if (class == HOST1X_CLASS_HOST1X && offset == HOST1X_UCLASS_WAIT_SYNCPT)
		host1x_debug_output(o, "waiting on syncpt\n");
	else
		host1x_debug_output(o, "active class %02x, offset %04x\n",
				    class, offset);

<<<<<<< HEAD
	host1x_debug_output(o, "DMAPUT %08x, DMAGET %08x, DMACTL %08x\n",
=======
	host1x_debug_output(o, "DMASTART %pad, DMAEND %pad\n", &dmastart, &dmaend);
	host1x_debug_output(o, "DMAPUT %08x DMAGET %08x DMACTL %08x\n",
>>>>>>> upstream/android-13
			    dmaput, dmaget, dmactrl);
	host1x_debug_output(o, "CHANNELSTAT %02x\n", ch_stat);

	show_channel_gathers(o, cdma);
	host1x_debug_output(o, "\n");
}

static void host1x_debug_show_channel_fifo(struct host1x *host,
					   struct host1x_channel *ch,
					   struct output *o)
{
<<<<<<< HEAD
	u32 val, rd_ptr, wr_ptr, start, end;
	u32 payload = INVALID_PAYLOAD;
	unsigned int data_count = 0;
=======
#if HOST1X_HW <= 6
	u32 rd_ptr, wr_ptr, start, end;
	u32 payload = INVALID_PAYLOAD;
	unsigned int data_count = 0;
#endif
	u32 val;
>>>>>>> upstream/android-13

	host1x_debug_output(o, "%u: fifo:\n", ch->id);

	val = host1x_ch_readl(ch, HOST1X_CHANNEL_CMDFIFO_STAT);
	host1x_debug_output(o, "CMDFIFO_STAT %08x\n", val);
	if (val & HOST1X_CHANNEL_CMDFIFO_STAT_EMPTY) {
		host1x_debug_output(o, "[empty]\n");
		return;
	}

	val = host1x_ch_readl(ch, HOST1X_CHANNEL_CMDFIFO_RDATA);
	host1x_debug_output(o, "CMDFIFO_RDATA %08x\n", val);

<<<<<<< HEAD
=======
#if HOST1X_HW <= 6
>>>>>>> upstream/android-13
	/* Peek pointer values are invalid during SLCG, so disable it */
	host1x_hypervisor_writel(host, 0x1, HOST1X_HV_ICG_EN_OVERRIDE);

	val = 0;
	val |= HOST1X_HV_CMDFIFO_PEEK_CTRL_ENABLE;
	val |= HOST1X_HV_CMDFIFO_PEEK_CTRL_CHANNEL(ch->id);
	host1x_hypervisor_writel(host, val, HOST1X_HV_CMDFIFO_PEEK_CTRL);

	val = host1x_hypervisor_readl(host, HOST1X_HV_CMDFIFO_PEEK_PTRS);
	rd_ptr = HOST1X_HV_CMDFIFO_PEEK_PTRS_RD_PTR_V(val);
	wr_ptr = HOST1X_HV_CMDFIFO_PEEK_PTRS_WR_PTR_V(val);

	val = host1x_hypervisor_readl(host, HOST1X_HV_CMDFIFO_SETUP(ch->id));
	start = HOST1X_HV_CMDFIFO_SETUP_BASE_V(val);
	end = HOST1X_HV_CMDFIFO_SETUP_LIMIT_V(val);

	do {
		val = 0;
		val |= HOST1X_HV_CMDFIFO_PEEK_CTRL_ENABLE;
		val |= HOST1X_HV_CMDFIFO_PEEK_CTRL_CHANNEL(ch->id);
		val |= HOST1X_HV_CMDFIFO_PEEK_CTRL_ADDR(rd_ptr);
		host1x_hypervisor_writel(host, val,
					 HOST1X_HV_CMDFIFO_PEEK_CTRL);

		val = host1x_hypervisor_readl(host,
					      HOST1X_HV_CMDFIFO_PEEK_READ);

		if (!data_count) {
			host1x_debug_output(o, "%03x 0x%08x: ",
					    rd_ptr - start, val);
			data_count = show_channel_command(o, val, &payload);
		} else {
			host1x_debug_cont(o, "%08x%s", val,
					  data_count > 1 ? ", " : "])\n");
			data_count--;
		}

		if (rd_ptr == end)
			rd_ptr = start;
		else
			rd_ptr++;
	} while (rd_ptr != wr_ptr);

	if (data_count)
		host1x_debug_cont(o, ", ...])\n");
	host1x_debug_output(o, "\n");

	host1x_hypervisor_writel(host, 0x0, HOST1X_HV_CMDFIFO_PEEK_CTRL);
	host1x_hypervisor_writel(host, 0x0, HOST1X_HV_ICG_EN_OVERRIDE);
<<<<<<< HEAD
=======
#endif
>>>>>>> upstream/android-13
}

static void host1x_debug_show_mlocks(struct host1x *host, struct output *o)
{
	/* TODO */
}
