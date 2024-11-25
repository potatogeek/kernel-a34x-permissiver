<<<<<<< HEAD
// SPDX-License-Identifier: (GPL-2.0 OR BSD-2-Clause)
/*
 * Copyright (C) 2018 Netronome Systems, Inc.
 *
 * This software is dual licensed under the GNU General License Version 2,
 * June 1991 as shown in the file COPYING in the top-level directory of this
 * source tree or the BSD 2-Clause License provided below.  You have the
 * option to license this software under the complete terms of either license.
 *
 * The BSD 2-Clause License:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      1. Redistributions of source code must retain the above
 *         copyright notice, this list of conditions and the following
 *         disclaimer.
 *
 *      2. Redistributions in binary form must reproduce the above
 *         copyright notice, this list of conditions and the following
 *         disclaimer in the documentation and/or other materials
 *         provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <linux/kernel.h>
=======
// SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause)
/* Copyright (C) 2018 Netronome Systems, Inc. */

#include <linux/bitops.h>
#include <linux/kernel.h>
#include <linux/log2.h>
>>>>>>> upstream/android-13

#include "../nfpcore/nfp_cpp.h"
#include "../nfpcore/nfp_nffw.h"
#include "../nfp_app.h"
#include "../nfp_abi.h"
#include "../nfp_main.h"
#include "../nfp_net.h"
#include "main.h"

<<<<<<< HEAD
#define NFP_QLVL_SYM_NAME	"_abi_nfd_out_q_lvls_%u"
=======
#define NFP_NUM_PRIOS_SYM_NAME	"_abi_pci_dscp_num_prio_%u"
#define NFP_NUM_BANDS_SYM_NAME	"_abi_pci_dscp_num_band_%u"
#define NFP_ACT_MASK_SYM_NAME	"_abi_nfd_out_q_actions_%u"

#define NFP_RED_SUPPORT_SYM_NAME	"_abi_nfd_out_red_offload_%u"

#define NFP_QLVL_SYM_NAME	"_abi_nfd_out_q_lvls_%u%s"
>>>>>>> upstream/android-13
#define NFP_QLVL_STRIDE		16
#define NFP_QLVL_BLOG_BYTES	0
#define NFP_QLVL_BLOG_PKTS	4
#define NFP_QLVL_THRS		8
<<<<<<< HEAD

#define NFP_QMSTAT_SYM_NAME	"_abi_nfdqm%u_stats"
=======
#define NFP_QLVL_ACT		12

#define NFP_QMSTAT_SYM_NAME	"_abi_nfdqm%u_stats%s"
>>>>>>> upstream/android-13
#define NFP_QMSTAT_STRIDE	32
#define NFP_QMSTAT_NON_STO	0
#define NFP_QMSTAT_STO		8
#define NFP_QMSTAT_DROP		16
#define NFP_QMSTAT_ECN		24

<<<<<<< HEAD
static unsigned long long
nfp_abm_q_lvl_thrs(struct nfp_abm_link *alink, unsigned int queue)
{
	return alink->abm->q_lvls->addr +
		(alink->queue_base + queue) * NFP_QLVL_STRIDE + NFP_QLVL_THRS;
}

static int
nfp_abm_ctrl_stat(struct nfp_abm_link *alink, const struct nfp_rtsym *sym,
		  unsigned int stride, unsigned int offset, unsigned int i,
		  bool is_u64, u64 *res)
{
	struct nfp_cpp *cpp = alink->abm->app->cpp;
	u32 val32, mur;
	u64 val, addr;
	int err;

	mur = NFP_CPP_ATOMIC_RD(sym->target, sym->domain);

	addr = sym->addr + (alink->queue_base + i) * stride + offset;
	if (is_u64)
		err = nfp_cpp_readq(cpp, mur, addr, &val);
	else
		err = nfp_cpp_readl(cpp, mur, addr, &val32);
	if (err) {
		nfp_err(cpp,
			"RED offload reading stat failed on vNIC %d queue %d\n",
			alink->id, i);
=======
#define NFP_Q_STAT_SYM_NAME	"_abi_nfd_rxq_stats%u%s"
#define NFP_Q_STAT_STRIDE	16
#define NFP_Q_STAT_PKTS		0
#define NFP_Q_STAT_BYTES	8

#define NFP_NET_ABM_MBOX_CMD		NFP_NET_CFG_MBOX_SIMPLE_CMD
#define NFP_NET_ABM_MBOX_RET		NFP_NET_CFG_MBOX_SIMPLE_RET
#define NFP_NET_ABM_MBOX_DATALEN	NFP_NET_CFG_MBOX_SIMPLE_VAL
#define NFP_NET_ABM_MBOX_RESERVED	(NFP_NET_CFG_MBOX_SIMPLE_VAL + 4)
#define NFP_NET_ABM_MBOX_DATA		(NFP_NET_CFG_MBOX_SIMPLE_VAL + 8)

static int
nfp_abm_ctrl_stat(struct nfp_abm_link *alink, const struct nfp_rtsym *sym,
		  unsigned int stride, unsigned int offset, unsigned int band,
		  unsigned int queue, bool is_u64, u64 *res)
{
	struct nfp_cpp *cpp = alink->abm->app->cpp;
	u64 val, sym_offset;
	unsigned int qid;
	u32 val32;
	int err;

	qid = band * NFP_NET_MAX_RX_RINGS + alink->queue_base + queue;

	sym_offset = qid * stride + offset;
	if (is_u64)
		err = __nfp_rtsym_readq(cpp, sym, 3, 0, sym_offset, &val);
	else
		err = __nfp_rtsym_readl(cpp, sym, 3, 0, sym_offset, &val32);
	if (err) {
		nfp_err(cpp, "RED offload reading stat failed on vNIC %d band %d queue %d (+ %d)\n",
			alink->id, band, queue, alink->queue_base);
>>>>>>> upstream/android-13
		return err;
	}

	*res = is_u64 ? val : val32;
	return 0;
}

<<<<<<< HEAD
static int
nfp_abm_ctrl_stat_all(struct nfp_abm_link *alink, const struct nfp_rtsym *sym,
		      unsigned int stride, unsigned int offset, bool is_u64,
		      u64 *res)
{
	u64 val, sum = 0;
	unsigned int i;
	int err;

	for (i = 0; i < alink->vnic->max_rx_rings; i++) {
		err = nfp_abm_ctrl_stat(alink, sym, stride, offset, i,
					is_u64, &val);
		if (err)
			return err;
		sum += val;
	}

	*res = sum;
	return 0;
}

int nfp_abm_ctrl_set_q_lvl(struct nfp_abm_link *alink, unsigned int i, u32 val)
{
	struct nfp_cpp *cpp = alink->abm->app->cpp;
	u32 muw;
	int err;

	muw = NFP_CPP_ATOMIC_WR(alink->abm->q_lvls->target,
				alink->abm->q_lvls->domain);

	err = nfp_cpp_writel(cpp, muw, nfp_abm_q_lvl_thrs(alink, i), val);
	if (err) {
		nfp_err(cpp, "RED offload setting level failed on vNIC %d queue %d\n",
			alink->id, i);
		return err;
	}

	return 0;
}

int nfp_abm_ctrl_set_all_q_lvls(struct nfp_abm_link *alink, u32 val)
{
	int i, err;

	for (i = 0; i < alink->vnic->max_rx_rings; i++) {
		err = nfp_abm_ctrl_set_q_lvl(alink, i, val);
		if (err)
			return err;
	}

	return 0;
}

u64 nfp_abm_ctrl_stat_non_sto(struct nfp_abm_link *alink, unsigned int i)
{
	u64 val;

	if (nfp_abm_ctrl_stat(alink, alink->abm->qm_stats, NFP_QMSTAT_STRIDE,
			      NFP_QMSTAT_NON_STO, i, true, &val))
		return 0;
	return val;
}

u64 nfp_abm_ctrl_stat_sto(struct nfp_abm_link *alink, unsigned int i)
{
	u64 val;

	if (nfp_abm_ctrl_stat(alink, alink->abm->qm_stats, NFP_QMSTAT_STRIDE,
			      NFP_QMSTAT_STO, i, true, &val))
		return 0;
	return val;
}

int nfp_abm_ctrl_read_q_stats(struct nfp_abm_link *alink, unsigned int i,
			      struct nfp_alink_stats *stats)
{
	int err;

	stats->tx_pkts = nn_readq(alink->vnic, NFP_NET_CFG_RXR_STATS(i));
	stats->tx_bytes = nn_readq(alink->vnic, NFP_NET_CFG_RXR_STATS(i) + 8);

	err = nfp_abm_ctrl_stat(alink, alink->abm->q_lvls,
				NFP_QLVL_STRIDE, NFP_QLVL_BLOG_BYTES,
				i, false, &stats->backlog_bytes);
=======
int __nfp_abm_ctrl_set_q_lvl(struct nfp_abm *abm, unsigned int id, u32 val)
{
	struct nfp_cpp *cpp = abm->app->cpp;
	u64 sym_offset;
	int err;

	__clear_bit(id, abm->threshold_undef);
	if (abm->thresholds[id] == val)
		return 0;

	sym_offset = id * NFP_QLVL_STRIDE + NFP_QLVL_THRS;
	err = __nfp_rtsym_writel(cpp, abm->q_lvls, 4, 0, sym_offset, val);
	if (err) {
		nfp_err(cpp,
			"RED offload setting level failed on subqueue %d\n",
			id);
		return err;
	}

	abm->thresholds[id] = val;
	return 0;
}

int nfp_abm_ctrl_set_q_lvl(struct nfp_abm_link *alink, unsigned int band,
			   unsigned int queue, u32 val)
{
	unsigned int threshold;

	threshold = band * NFP_NET_MAX_RX_RINGS + alink->queue_base + queue;

	return __nfp_abm_ctrl_set_q_lvl(alink->abm, threshold, val);
}

int __nfp_abm_ctrl_set_q_act(struct nfp_abm *abm, unsigned int id,
			     enum nfp_abm_q_action act)
{
	struct nfp_cpp *cpp = abm->app->cpp;
	u64 sym_offset;
	int err;

	if (abm->actions[id] == act)
		return 0;

	sym_offset = id * NFP_QLVL_STRIDE + NFP_QLVL_ACT;
	err = __nfp_rtsym_writel(cpp, abm->q_lvls, 4, 0, sym_offset, act);
	if (err) {
		nfp_err(cpp,
			"RED offload setting action failed on subqueue %d\n",
			id);
		return err;
	}

	abm->actions[id] = act;
	return 0;
}

int nfp_abm_ctrl_set_q_act(struct nfp_abm_link *alink, unsigned int band,
			   unsigned int queue, enum nfp_abm_q_action act)
{
	unsigned int qid;

	qid = band * NFP_NET_MAX_RX_RINGS + alink->queue_base + queue;

	return __nfp_abm_ctrl_set_q_act(alink->abm, qid, act);
}

u64 nfp_abm_ctrl_stat_non_sto(struct nfp_abm_link *alink, unsigned int queue)
{
	unsigned int band;
	u64 val, sum = 0;

	for (band = 0; band < alink->abm->num_bands; band++) {
		if (nfp_abm_ctrl_stat(alink, alink->abm->qm_stats,
				      NFP_QMSTAT_STRIDE, NFP_QMSTAT_NON_STO,
				      band, queue, true, &val))
			return 0;
		sum += val;
	}

	return sum;
}

u64 nfp_abm_ctrl_stat_sto(struct nfp_abm_link *alink, unsigned int queue)
{
	unsigned int band;
	u64 val, sum = 0;

	for (band = 0; band < alink->abm->num_bands; band++) {
		if (nfp_abm_ctrl_stat(alink, alink->abm->qm_stats,
				      NFP_QMSTAT_STRIDE, NFP_QMSTAT_STO,
				      band, queue, true, &val))
			return 0;
		sum += val;
	}

	return sum;
}

static int
nfp_abm_ctrl_stat_basic(struct nfp_abm_link *alink, unsigned int band,
			unsigned int queue, unsigned int off, u64 *val)
{
	if (!nfp_abm_has_prio(alink->abm)) {
		if (!band) {
			unsigned int id = alink->queue_base + queue;

			*val = nn_readq(alink->vnic,
					NFP_NET_CFG_RXR_STATS(id) + off);
		} else {
			*val = 0;
		}

		return 0;
	} else {
		return nfp_abm_ctrl_stat(alink, alink->abm->q_stats,
					 NFP_Q_STAT_STRIDE, off, band, queue,
					 true, val);
	}
}

int nfp_abm_ctrl_read_q_stats(struct nfp_abm_link *alink, unsigned int band,
			      unsigned int queue, struct nfp_alink_stats *stats)
{
	int err;

	err = nfp_abm_ctrl_stat_basic(alink, band, queue, NFP_Q_STAT_PKTS,
				      &stats->tx_pkts);
	if (err)
		return err;

	err = nfp_abm_ctrl_stat_basic(alink, band, queue, NFP_Q_STAT_BYTES,
				      &stats->tx_bytes);
	if (err)
		return err;

	err = nfp_abm_ctrl_stat(alink, alink->abm->q_lvls, NFP_QLVL_STRIDE,
				NFP_QLVL_BLOG_BYTES, band, queue, false,
				&stats->backlog_bytes);
>>>>>>> upstream/android-13
	if (err)
		return err;

	err = nfp_abm_ctrl_stat(alink, alink->abm->q_lvls,
				NFP_QLVL_STRIDE, NFP_QLVL_BLOG_PKTS,
<<<<<<< HEAD
				i, false, &stats->backlog_pkts);
=======
				band, queue, false, &stats->backlog_pkts);
>>>>>>> upstream/android-13
	if (err)
		return err;

	err = nfp_abm_ctrl_stat(alink, alink->abm->qm_stats,
				NFP_QMSTAT_STRIDE, NFP_QMSTAT_DROP,
<<<<<<< HEAD
				i, true, &stats->drops);
=======
				band, queue, true, &stats->drops);
>>>>>>> upstream/android-13
	if (err)
		return err;

	return nfp_abm_ctrl_stat(alink, alink->abm->qm_stats,
				 NFP_QMSTAT_STRIDE, NFP_QMSTAT_ECN,
<<<<<<< HEAD
				 i, true, &stats->overlimits);
}

int nfp_abm_ctrl_read_stats(struct nfp_abm_link *alink,
			    struct nfp_alink_stats *stats)
{
	u64 pkts = 0, bytes = 0;
	int i, err;

	for (i = 0; i < alink->vnic->max_rx_rings; i++) {
		pkts += nn_readq(alink->vnic, NFP_NET_CFG_RXR_STATS(i));
		bytes += nn_readq(alink->vnic, NFP_NET_CFG_RXR_STATS(i) + 8);
	}
	stats->tx_pkts = pkts;
	stats->tx_bytes = bytes;

	err = nfp_abm_ctrl_stat_all(alink, alink->abm->q_lvls,
				    NFP_QLVL_STRIDE, NFP_QLVL_BLOG_BYTES,
				    false, &stats->backlog_bytes);
	if (err)
		return err;

	err = nfp_abm_ctrl_stat_all(alink, alink->abm->q_lvls,
				    NFP_QLVL_STRIDE, NFP_QLVL_BLOG_PKTS,
				    false, &stats->backlog_pkts);
	if (err)
		return err;

	err = nfp_abm_ctrl_stat_all(alink, alink->abm->qm_stats,
				    NFP_QMSTAT_STRIDE, NFP_QMSTAT_DROP,
				    true, &stats->drops);
	if (err)
		return err;

	return nfp_abm_ctrl_stat_all(alink, alink->abm->qm_stats,
				     NFP_QMSTAT_STRIDE, NFP_QMSTAT_ECN,
				     true, &stats->overlimits);
}

int nfp_abm_ctrl_read_q_xstats(struct nfp_abm_link *alink, unsigned int i,
=======
				 band, queue, true, &stats->overlimits);
}

int nfp_abm_ctrl_read_q_xstats(struct nfp_abm_link *alink,
			       unsigned int band, unsigned int queue,
>>>>>>> upstream/android-13
			       struct nfp_alink_xstats *xstats)
{
	int err;

	err = nfp_abm_ctrl_stat(alink, alink->abm->qm_stats,
				NFP_QMSTAT_STRIDE, NFP_QMSTAT_DROP,
<<<<<<< HEAD
				i, true, &xstats->pdrop);
=======
				band, queue, true, &xstats->pdrop);
>>>>>>> upstream/android-13
	if (err)
		return err;

	return nfp_abm_ctrl_stat(alink, alink->abm->qm_stats,
				 NFP_QMSTAT_STRIDE, NFP_QMSTAT_ECN,
<<<<<<< HEAD
				 i, true, &xstats->ecn_marked);
}

int nfp_abm_ctrl_read_xstats(struct nfp_abm_link *alink,
			     struct nfp_alink_xstats *xstats)
{
	int err;

	err = nfp_abm_ctrl_stat_all(alink, alink->abm->qm_stats,
				    NFP_QMSTAT_STRIDE, NFP_QMSTAT_DROP,
				    true, &xstats->pdrop);
	if (err)
		return err;

	return nfp_abm_ctrl_stat_all(alink, alink->abm->qm_stats,
				     NFP_QMSTAT_STRIDE, NFP_QMSTAT_ECN,
				     true, &xstats->ecn_marked);
=======
				 band, queue, true, &xstats->ecn_marked);
>>>>>>> upstream/android-13
}

int nfp_abm_ctrl_qm_enable(struct nfp_abm *abm)
{
	return nfp_mbox_cmd(abm->app->pf, NFP_MBOX_PCIE_ABM_ENABLE,
			    NULL, 0, NULL, 0);
}

int nfp_abm_ctrl_qm_disable(struct nfp_abm *abm)
{
	return nfp_mbox_cmd(abm->app->pf, NFP_MBOX_PCIE_ABM_DISABLE,
			    NULL, 0, NULL, 0);
}

<<<<<<< HEAD
void nfp_abm_ctrl_read_params(struct nfp_abm_link *alink)
{
	alink->queue_base = nn_readl(alink->vnic, NFP_NET_CFG_START_RXQ);
	alink->queue_base /= alink->vnic->stride_rx;
=======
int nfp_abm_ctrl_prio_map_update(struct nfp_abm_link *alink, u32 *packed)
{
	const u32 cmd = NFP_NET_CFG_MBOX_CMD_PCI_DSCP_PRIOMAP_SET;
	struct nfp_net *nn = alink->vnic;
	unsigned int i;
	int err;

	err = nfp_net_mbox_lock(nn, alink->abm->prio_map_len);
	if (err)
		return err;

	/* Write data_len and wipe reserved */
	nn_writeq(nn, nn->tlv_caps.mbox_off + NFP_NET_ABM_MBOX_DATALEN,
		  alink->abm->prio_map_len);

	for (i = 0; i < alink->abm->prio_map_len; i += sizeof(u32))
		nn_writel(nn, nn->tlv_caps.mbox_off + NFP_NET_ABM_MBOX_DATA + i,
			  packed[i / sizeof(u32)]);

	err = nfp_net_mbox_reconfig_and_unlock(nn, cmd);
	if (err)
		nfp_err(alink->abm->app->cpp,
			"setting DSCP -> VQ map failed with error %d\n", err);
	return err;
}

static int nfp_abm_ctrl_prio_check_params(struct nfp_abm_link *alink)
{
	struct nfp_abm *abm = alink->abm;
	struct nfp_net *nn = alink->vnic;
	unsigned int min_mbox_sz;

	if (!nfp_abm_has_prio(alink->abm))
		return 0;

	min_mbox_sz = NFP_NET_ABM_MBOX_DATA + alink->abm->prio_map_len;
	if (nn->tlv_caps.mbox_len < min_mbox_sz) {
		nfp_err(abm->app->pf->cpp, "vNIC mailbox too small for prio offload: %u, need: %u\n",
			nn->tlv_caps.mbox_len,  min_mbox_sz);
		return -EINVAL;
	}

	return 0;
}

int nfp_abm_ctrl_read_params(struct nfp_abm_link *alink)
{
	alink->queue_base = nn_readl(alink->vnic, NFP_NET_CFG_START_RXQ);
	alink->queue_base /= alink->vnic->stride_rx;

	return nfp_abm_ctrl_prio_check_params(alink);
}

static unsigned int nfp_abm_ctrl_prio_map_size(struct nfp_abm *abm)
{
	unsigned int size;

	size = roundup_pow_of_two(order_base_2(abm->num_bands));
	size = DIV_ROUND_UP(size * abm->num_prios, BITS_PER_BYTE);
	size = round_up(size, sizeof(u32));

	return size;
>>>>>>> upstream/android-13
}

static const struct nfp_rtsym *
nfp_abm_ctrl_find_rtsym(struct nfp_pf *pf, const char *name, unsigned int size)
{
	const struct nfp_rtsym *sym;

	sym = nfp_rtsym_lookup(pf->rtbl, name);
	if (!sym) {
		nfp_err(pf->cpp, "Symbol '%s' not found\n", name);
		return ERR_PTR(-ENOENT);
	}
<<<<<<< HEAD
	if (sym->size != size) {
		nfp_err(pf->cpp,
			"Symbol '%s' wrong size: expected %u got %llu\n",
			name, size, sym->size);
=======
	if (nfp_rtsym_size(sym) != size) {
		nfp_err(pf->cpp,
			"Symbol '%s' wrong size: expected %u got %llu\n",
			name, size, nfp_rtsym_size(sym));
>>>>>>> upstream/android-13
		return ERR_PTR(-EINVAL);
	}

	return sym;
}

static const struct nfp_rtsym *
<<<<<<< HEAD
nfp_abm_ctrl_find_q_rtsym(struct nfp_pf *pf, const char *name,
			  unsigned int size)
{
	return nfp_abm_ctrl_find_rtsym(pf, name, size * NFP_NET_MAX_RX_RINGS);
=======
nfp_abm_ctrl_find_q_rtsym(struct nfp_abm *abm, const char *name_fmt,
			  size_t size)
{
	char pf_symbol[64];

	size = array3_size(size, abm->num_bands, NFP_NET_MAX_RX_RINGS);
	snprintf(pf_symbol, sizeof(pf_symbol), name_fmt,
		 abm->pf_id, nfp_abm_has_prio(abm) ? "_per_band" : "");

	return nfp_abm_ctrl_find_rtsym(abm->app->pf, pf_symbol, size);
>>>>>>> upstream/android-13
}

int nfp_abm_ctrl_find_addrs(struct nfp_abm *abm)
{
	struct nfp_pf *pf = abm->app->pf;
	const struct nfp_rtsym *sym;
<<<<<<< HEAD
	unsigned int pf_id;
	char pf_symbol[64];

	pf_id =	nfp_cppcore_pcie_unit(pf->cpp);
	abm->pf_id = pf_id;

	snprintf(pf_symbol, sizeof(pf_symbol), NFP_QLVL_SYM_NAME, pf_id);
	sym = nfp_abm_ctrl_find_q_rtsym(pf, pf_symbol, NFP_QLVL_STRIDE);
=======
	int res;

	abm->pf_id = nfp_cppcore_pcie_unit(pf->cpp);

	/* Check if Qdisc offloads are supported */
	res = nfp_pf_rtsym_read_optional(pf, NFP_RED_SUPPORT_SYM_NAME, 1);
	if (res < 0)
		return res;
	abm->red_support = res;

	/* Read count of prios and prio bands */
	res = nfp_pf_rtsym_read_optional(pf, NFP_NUM_BANDS_SYM_NAME, 1);
	if (res < 0)
		return res;
	abm->num_bands = res;

	res = nfp_pf_rtsym_read_optional(pf, NFP_NUM_PRIOS_SYM_NAME, 1);
	if (res < 0)
		return res;
	abm->num_prios = res;

	/* Read available actions */
	res = nfp_pf_rtsym_read_optional(pf, NFP_ACT_MASK_SYM_NAME,
					 BIT(NFP_ABM_ACT_MARK_DROP));
	if (res < 0)
		return res;
	abm->action_mask = res;

	abm->prio_map_len = nfp_abm_ctrl_prio_map_size(abm);
	abm->dscp_mask = GENMASK(7, 8 - order_base_2(abm->num_prios));

	/* Check values are sane, U16_MAX is arbitrarily chosen as max */
	if (!is_power_of_2(abm->num_bands) || !is_power_of_2(abm->num_prios) ||
	    abm->num_bands > U16_MAX || abm->num_prios > U16_MAX ||
	    (abm->num_bands == 1) != (abm->num_prios == 1)) {
		nfp_err(pf->cpp,
			"invalid priomap description num bands: %u and num prios: %u\n",
			abm->num_bands, abm->num_prios);
		return -EINVAL;
	}

	/* Find level and stat symbols */
	if (!abm->red_support)
		return 0;

	sym = nfp_abm_ctrl_find_q_rtsym(abm, NFP_QLVL_SYM_NAME,
					NFP_QLVL_STRIDE);
>>>>>>> upstream/android-13
	if (IS_ERR(sym))
		return PTR_ERR(sym);
	abm->q_lvls = sym;

<<<<<<< HEAD
	snprintf(pf_symbol, sizeof(pf_symbol), NFP_QMSTAT_SYM_NAME, pf_id);
	sym = nfp_abm_ctrl_find_q_rtsym(pf, pf_symbol, NFP_QMSTAT_STRIDE);
=======
	sym = nfp_abm_ctrl_find_q_rtsym(abm, NFP_QMSTAT_SYM_NAME,
					NFP_QMSTAT_STRIDE);
>>>>>>> upstream/android-13
	if (IS_ERR(sym))
		return PTR_ERR(sym);
	abm->qm_stats = sym;

<<<<<<< HEAD
=======
	if (nfp_abm_has_prio(abm)) {
		sym = nfp_abm_ctrl_find_q_rtsym(abm, NFP_Q_STAT_SYM_NAME,
						NFP_Q_STAT_STRIDE);
		if (IS_ERR(sym))
			return PTR_ERR(sym);
		abm->q_stats = sym;
	}

>>>>>>> upstream/android-13
	return 0;
}
