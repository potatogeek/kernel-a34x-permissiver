<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*******************************************************************************
  Copyright (C) 2013  Vayavya Labs Pvt Ltd

  This implements all the API for managing HW timestamp & PTP.

<<<<<<< HEAD
  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".
=======
>>>>>>> upstream/android-13

  Author: Rayagond Kokatanur <rayagond@vayavyalabs.com>
  Author: Giuseppe Cavallaro <peppe.cavallaro@st.com>
*******************************************************************************/

#include <linux/io.h>
<<<<<<< HEAD
#include <linux/delay.h>
#include "common.h"
#include "stmmac_ptp.h"
=======
#include <linux/iopoll.h>
#include <linux/delay.h>
#include <linux/ptp_clock_kernel.h>
#include "common.h"
#include "stmmac_ptp.h"
#include "dwmac4.h"
#include "stmmac.h"
>>>>>>> upstream/android-13

static void config_hw_tstamping(void __iomem *ioaddr, u32 data)
{
	writel(data, ioaddr + PTP_TCR);
}

static void config_sub_second_increment(void __iomem *ioaddr,
		u32 ptp_clock, int gmac4, u32 *ssinc)
{
	u32 value = readl(ioaddr + PTP_TCR);
	unsigned long data;
	u32 reg_value;

	/* For GMAC3.x, 4.x versions, in "fine adjustement mode" set sub-second
	 * increment to twice the number of nanoseconds of a clock cycle.
	 * The calculation of the default_addend value by the caller will set it
	 * to mid-range = 2^31 when the remainder of this division is zero,
	 * which will make the accumulator overflow once every 2 ptp_clock
	 * cycles, adding twice the number of nanoseconds of a clock cycle :
	 * 2000000000ULL / ptp_clock.
	 */
	if (value & PTP_TCR_TSCFUPDT)
		data = (2000000000ULL / ptp_clock);
	else
		data = (1000000000ULL / ptp_clock);

	/* 0.465ns accuracy */
	if (!(value & PTP_TCR_TSCTRLSSR))
		data = (data * 1000) / 465;

	data &= PTP_SSIR_SSINC_MASK;

	reg_value = data;
	if (gmac4)
		reg_value <<= GMAC4_PTP_SSIR_SSINC_SHIFT;

	writel(reg_value, ioaddr + PTP_SSIR);

	if (ssinc)
		*ssinc = data;
}

static int init_systime(void __iomem *ioaddr, u32 sec, u32 nsec)
{
<<<<<<< HEAD
	int limit;
=======
>>>>>>> upstream/android-13
	u32 value;

	writel(sec, ioaddr + PTP_STSUR);
	writel(nsec, ioaddr + PTP_STNSUR);
	/* issue command to initialize the system time value */
	value = readl(ioaddr + PTP_TCR);
	value |= PTP_TCR_TSINIT;
	writel(value, ioaddr + PTP_TCR);

	/* wait for present system time initialize to complete */
<<<<<<< HEAD
	limit = 10;
	while (limit--) {
		if (!(readl(ioaddr + PTP_TCR) & PTP_TCR_TSINIT))
			break;
		mdelay(10);
	}
	if (limit < 0)
		return -EBUSY;

	return 0;
=======
	return readl_poll_timeout_atomic(ioaddr + PTP_TCR, value,
				 !(value & PTP_TCR_TSINIT),
				 10, 100000);
>>>>>>> upstream/android-13
}

static int config_addend(void __iomem *ioaddr, u32 addend)
{
	u32 value;
	int limit;

	writel(addend, ioaddr + PTP_TAR);
	/* issue command to update the addend value */
	value = readl(ioaddr + PTP_TCR);
	value |= PTP_TCR_TSADDREG;
	writel(value, ioaddr + PTP_TCR);

	/* wait for present addend update to complete */
	limit = 10;
	while (limit--) {
		if (!(readl(ioaddr + PTP_TCR) & PTP_TCR_TSADDREG))
			break;
		mdelay(10);
	}
	if (limit < 0)
		return -EBUSY;

	return 0;
}

static int adjust_systime(void __iomem *ioaddr, u32 sec, u32 nsec,
		int add_sub, int gmac4)
{
	u32 value;
	int limit;

	if (add_sub) {
		/* If the new sec value needs to be subtracted with
		 * the system time, then MAC_STSUR reg should be
		 * programmed with (2^32 – <new_sec_value>)
		 */
		if (gmac4)
			sec = -sec;

		value = readl(ioaddr + PTP_TCR);
		if (value & PTP_TCR_TSCTRLSSR)
			nsec = (PTP_DIGITAL_ROLLOVER_MODE - nsec);
		else
			nsec = (PTP_BINARY_ROLLOVER_MODE - nsec);
	}

	writel(sec, ioaddr + PTP_STSUR);
	value = (add_sub << PTP_STNSUR_ADDSUB_SHIFT) | nsec;
	writel(value, ioaddr + PTP_STNSUR);

	/* issue command to initialize the system time value */
	value = readl(ioaddr + PTP_TCR);
	value |= PTP_TCR_TSUPDT;
	writel(value, ioaddr + PTP_TCR);

	/* wait for present system time adjust/update to complete */
	limit = 10;
	while (limit--) {
		if (!(readl(ioaddr + PTP_TCR) & PTP_TCR_TSUPDT))
			break;
		mdelay(10);
	}
	if (limit < 0)
		return -EBUSY;

	return 0;
}

static void get_systime(void __iomem *ioaddr, u64 *systime)
{
<<<<<<< HEAD
	u64 ns;

	/* Get the TSSS value */
	ns = readl(ioaddr + PTP_STNSR);
	/* Get the TSS and convert sec time value to nanosecond */
	ns += readl(ioaddr + PTP_STSR) * 1000000000ULL;

	if (systime)
		*systime = ns;
=======
	u64 ns, sec0, sec1;

	/* Get the TSS value */
	sec1 = readl_relaxed(ioaddr + PTP_STSR);
	do {
		sec0 = sec1;
		/* Get the TSSS value */
		ns = readl_relaxed(ioaddr + PTP_STNSR);
		/* Get the TSS value */
		sec1 = readl_relaxed(ioaddr + PTP_STSR);
	} while (sec0 != sec1);

	if (systime)
		*systime = ns + (sec1 * 1000000000ULL);
}

static void get_ptptime(void __iomem *ptpaddr, u64 *ptp_time)
{
	u64 ns;

	ns = readl(ptpaddr + PTP_ATNR);
	ns += readl(ptpaddr + PTP_ATSR) * NSEC_PER_SEC;

	*ptp_time = ns;
}

static void timestamp_interrupt(struct stmmac_priv *priv)
{
	u32 num_snapshot, ts_status, tsync_int;
	struct ptp_clock_event event;
	unsigned long flags;
	u64 ptp_time;
	int i;

	tsync_int = readl(priv->ioaddr + GMAC_INT_STATUS) & GMAC_INT_TSIE;

	if (!tsync_int)
		return;

	/* Read timestamp status to clear interrupt from either external
	 * timestamp or start/end of PPS.
	 */
	ts_status = readl(priv->ioaddr + GMAC_TIMESTAMP_STATUS);

	if (!priv->plat->ext_snapshot_en)
		return;

	num_snapshot = (ts_status & GMAC_TIMESTAMP_ATSNS_MASK) >>
		       GMAC_TIMESTAMP_ATSNS_SHIFT;

	for (i = 0; i < num_snapshot; i++) {
		spin_lock_irqsave(&priv->ptp_lock, flags);
		get_ptptime(priv->ptpaddr, &ptp_time);
		spin_unlock_irqrestore(&priv->ptp_lock, flags);
		event.type = PTP_CLOCK_EXTTS;
		event.index = 0;
		event.timestamp = ptp_time;
		ptp_clock_event(priv->ptp_clock, &event);
	}
>>>>>>> upstream/android-13
}

const struct stmmac_hwtimestamp stmmac_ptp = {
	.config_hw_tstamping = config_hw_tstamping,
	.init_systime = init_systime,
	.config_sub_second_increment = config_sub_second_increment,
	.config_addend = config_addend,
	.adjust_systime = adjust_systime,
	.get_systime = get_systime,
<<<<<<< HEAD
=======
	.get_ptptime = get_ptptime,
	.timestamp_interrupt = timestamp_interrupt,
>>>>>>> upstream/android-13
};
