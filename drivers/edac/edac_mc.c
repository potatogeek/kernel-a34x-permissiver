/*
 * edac_mc kernel module
 * (C) 2005, 2006 Linux Networx (http://lnxi.com)
 * This file may be distributed under the terms of the
 * GNU General Public License.
 *
 * Written by Thayne Harbaugh
 * Based on work by Dan Hollis <goemon at anime dot net> and others.
 *	http://www.anime.net/~goemon/linux-ecc/
 *
 * Modified by Dave Peterson and Doug Thompson
 *
 */

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/smp.h>
#include <linux/init.h>
#include <linux/sysctl.h>
#include <linux/highmem.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/ctype.h>
#include <linux/edac.h>
#include <linux/bitops.h>
#include <linux/uaccess.h>
#include <asm/page.h>
#include "edac_mc.h"
#include "edac_module.h"
#include <ras/ras_event.h>

#ifdef CONFIG_EDAC_ATOMIC_SCRUB
#include <asm/edac.h>
#else
#define edac_atomic_scrub(va, size) do { } while (0)
#endif

int edac_op_state = EDAC_OPSTATE_INVAL;
EXPORT_SYMBOL_GPL(edac_op_state);

<<<<<<< HEAD
static int edac_report = EDAC_REPORTING_ENABLED;

=======
>>>>>>> upstream/android-13
/* lock to memory controller's control array */
static DEFINE_MUTEX(mem_ctls_mutex);
static LIST_HEAD(mc_devices);

/*
 * Used to lock EDAC MC to just one module, avoiding two drivers e. g.
 *	apei/ghes and i7core_edac to be used at the same time.
 */
static const char *edac_mc_owner;

<<<<<<< HEAD
static struct bus_type mc_bus[EDAC_MAX_MCS];

int edac_get_report_status(void)
{
	return edac_report;
}
EXPORT_SYMBOL_GPL(edac_get_report_status);

void edac_set_report_status(int new)
{
	if (new == EDAC_REPORTING_ENABLED ||
	    new == EDAC_REPORTING_DISABLED ||
	    new == EDAC_REPORTING_FORCE)
		edac_report = new;
}
EXPORT_SYMBOL_GPL(edac_set_report_status);

static int edac_report_set(const char *str, const struct kernel_param *kp)
{
	if (!str)
		return -EINVAL;

	if (!strncmp(str, "on", 2))
		edac_report = EDAC_REPORTING_ENABLED;
	else if (!strncmp(str, "off", 3))
		edac_report = EDAC_REPORTING_DISABLED;
	else if (!strncmp(str, "force", 5))
		edac_report = EDAC_REPORTING_FORCE;

	return 0;
}

static int edac_report_get(char *buffer, const struct kernel_param *kp)
{
	int ret = 0;

	switch (edac_report) {
	case EDAC_REPORTING_ENABLED:
		ret = sprintf(buffer, "on");
		break;
	case EDAC_REPORTING_DISABLED:
		ret = sprintf(buffer, "off");
		break;
	case EDAC_REPORTING_FORCE:
		ret = sprintf(buffer, "force");
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static const struct kernel_param_ops edac_report_ops = {
	.set = edac_report_set,
	.get = edac_report_get,
};

module_param_cb(edac_report, &edac_report_ops, &edac_report, 0644);

unsigned edac_dimm_info_location(struct dimm_info *dimm, char *buf,
			         unsigned len)
=======
static struct mem_ctl_info *error_desc_to_mci(struct edac_raw_error_desc *e)
{
	return container_of(e, struct mem_ctl_info, error_desc);
}

unsigned int edac_dimm_info_location(struct dimm_info *dimm, char *buf,
				     unsigned int len)
>>>>>>> upstream/android-13
{
	struct mem_ctl_info *mci = dimm->mci;
	int i, n, count = 0;
	char *p = buf;

	for (i = 0; i < mci->n_layers; i++) {
		n = snprintf(p, len, "%s %d ",
			      edac_layer_name[mci->layers[i].type],
			      dimm->location[i]);
		p += n;
		len -= n;
		count += n;
		if (!len)
			break;
	}

	return count;
}

#ifdef CONFIG_EDAC_DEBUG

static void edac_mc_dump_channel(struct rank_info *chan)
{
	edac_dbg(4, "  channel->chan_idx = %d\n", chan->chan_idx);
	edac_dbg(4, "    channel = %p\n", chan);
	edac_dbg(4, "    channel->csrow = %p\n", chan->csrow);
	edac_dbg(4, "    channel->dimm = %p\n", chan->dimm);
}

<<<<<<< HEAD
static void edac_mc_dump_dimm(struct dimm_info *dimm, int number)
{
	char location[80];

=======
static void edac_mc_dump_dimm(struct dimm_info *dimm)
{
	char location[80];

	if (!dimm->nr_pages)
		return;

>>>>>>> upstream/android-13
	edac_dimm_info_location(dimm, location, sizeof(location));

	edac_dbg(4, "%s%i: %smapped as virtual row %d, chan %d\n",
		 dimm->mci->csbased ? "rank" : "dimm",
<<<<<<< HEAD
		 number, location, dimm->csrow, dimm->cschannel);
=======
		 dimm->idx, location, dimm->csrow, dimm->cschannel);
>>>>>>> upstream/android-13
	edac_dbg(4, "  dimm = %p\n", dimm);
	edac_dbg(4, "  dimm->label = '%s'\n", dimm->label);
	edac_dbg(4, "  dimm->nr_pages = 0x%x\n", dimm->nr_pages);
	edac_dbg(4, "  dimm->grain = %d\n", dimm->grain);
	edac_dbg(4, "  dimm->nr_pages = 0x%x\n", dimm->nr_pages);
}

static void edac_mc_dump_csrow(struct csrow_info *csrow)
{
	edac_dbg(4, "csrow->csrow_idx = %d\n", csrow->csrow_idx);
	edac_dbg(4, "  csrow = %p\n", csrow);
	edac_dbg(4, "  csrow->first_page = 0x%lx\n", csrow->first_page);
	edac_dbg(4, "  csrow->last_page = 0x%lx\n", csrow->last_page);
	edac_dbg(4, "  csrow->page_mask = 0x%lx\n", csrow->page_mask);
	edac_dbg(4, "  csrow->nr_channels = %d\n", csrow->nr_channels);
	edac_dbg(4, "  csrow->channels = %p\n", csrow->channels);
	edac_dbg(4, "  csrow->mci = %p\n", csrow->mci);
}

static void edac_mc_dump_mci(struct mem_ctl_info *mci)
{
	edac_dbg(3, "\tmci = %p\n", mci);
	edac_dbg(3, "\tmci->mtype_cap = %lx\n", mci->mtype_cap);
	edac_dbg(3, "\tmci->edac_ctl_cap = %lx\n", mci->edac_ctl_cap);
	edac_dbg(3, "\tmci->edac_cap = %lx\n", mci->edac_cap);
	edac_dbg(4, "\tmci->edac_check = %p\n", mci->edac_check);
	edac_dbg(3, "\tmci->nr_csrows = %d, csrows = %p\n",
		 mci->nr_csrows, mci->csrows);
	edac_dbg(3, "\tmci->nr_dimms = %d, dimms = %p\n",
		 mci->tot_dimms, mci->dimms);
	edac_dbg(3, "\tdev = %p\n", mci->pdev);
	edac_dbg(3, "\tmod_name:ctl_name = %s:%s\n",
		 mci->mod_name, mci->ctl_name);
	edac_dbg(3, "\tpvt_info = %p\n\n", mci->pvt_info);
}

#endif				/* CONFIG_EDAC_DEBUG */

const char * const edac_mem_types[] = {
	[MEM_EMPTY]	= "Empty",
	[MEM_RESERVED]	= "Reserved",
	[MEM_UNKNOWN]	= "Unknown",
	[MEM_FPM]	= "FPM",
	[MEM_EDO]	= "EDO",
	[MEM_BEDO]	= "BEDO",
	[MEM_SDR]	= "Unbuffered-SDR",
	[MEM_RDR]	= "Registered-SDR",
	[MEM_DDR]	= "Unbuffered-DDR",
	[MEM_RDDR]	= "Registered-DDR",
	[MEM_RMBS]	= "RMBS",
	[MEM_DDR2]	= "Unbuffered-DDR2",
	[MEM_FB_DDR2]	= "FullyBuffered-DDR2",
	[MEM_RDDR2]	= "Registered-DDR2",
	[MEM_XDR]	= "XDR",
	[MEM_DDR3]	= "Unbuffered-DDR3",
	[MEM_RDDR3]	= "Registered-DDR3",
	[MEM_LRDDR3]	= "Load-Reduced-DDR3-RAM",
<<<<<<< HEAD
	[MEM_DDR4]	= "Unbuffered-DDR4",
	[MEM_RDDR4]	= "Registered-DDR4",
	[MEM_LRDDR4]	= "Load-Reduced-DDR4-RAM",
	[MEM_NVDIMM]	= "Non-volatile-RAM",
=======
	[MEM_LPDDR3]	= "Low-Power-DDR3-RAM",
	[MEM_DDR4]	= "Unbuffered-DDR4",
	[MEM_RDDR4]	= "Registered-DDR4",
	[MEM_LPDDR4]	= "Low-Power-DDR4-RAM",
	[MEM_LRDDR4]	= "Load-Reduced-DDR4-RAM",
	[MEM_DDR5]	= "Unbuffered-DDR5",
	[MEM_NVDIMM]	= "Non-volatile-RAM",
	[MEM_WIO2]	= "Wide-IO-2",
	[MEM_HBM2]	= "High-bandwidth-memory-Gen2",
>>>>>>> upstream/android-13
};
EXPORT_SYMBOL_GPL(edac_mem_types);

/**
 * edac_align_ptr - Prepares the pointer offsets for a single-shot allocation
 * @p:		pointer to a pointer with the memory offset to be used. At
 *		return, this will be incremented to point to the next offset
 * @size:	Size of the data structure to be reserved
 * @n_elems:	Number of elements that should be reserved
 *
 * If 'size' is a constant, the compiler will optimize this whole function
 * down to either a no-op or the addition of a constant to the value of '*p'.
 *
 * The 'p' pointer is absolutely needed to keep the proper advancing
 * further in memory to the proper offsets when allocating the struct along
 * with its embedded structs, as edac_device_alloc_ctl_info() does it
 * above, for example.
 *
 * At return, the pointer 'p' will be incremented to be used on a next call
 * to this function.
 */
<<<<<<< HEAD
void *edac_align_ptr(void **p, unsigned size, int n_elems)
{
	unsigned align, r;
=======
void *edac_align_ptr(void **p, unsigned int size, int n_elems)
{
	unsigned int align, r;
>>>>>>> upstream/android-13
	void *ptr = *p;

	*p += size * n_elems;

	/*
	 * 'p' can possibly be an unaligned item X such that sizeof(X) is
	 * 'size'.  Adjust 'p' so that its alignment is at least as
	 * stringent as what the compiler would provide for X and return
	 * the aligned result.
	 * Here we assume that the alignment of a "long long" is the most
	 * stringent alignment that the compiler will ever provide by default.
	 * As far as I know, this is a reasonable assumption.
	 */
	if (size > sizeof(long))
		align = sizeof(long long);
	else if (size > sizeof(int))
		align = sizeof(long);
	else if (size > sizeof(short))
		align = sizeof(int);
	else if (size > sizeof(char))
		align = sizeof(short);
	else
		return (char *)ptr;

<<<<<<< HEAD
	r = (unsigned long)p % align;
=======
	r = (unsigned long)ptr % align;
>>>>>>> upstream/android-13

	if (r == 0)
		return (char *)ptr;

	*p += align - r;

	return (void *)(((unsigned long)ptr) + align - r);
}

static void _edac_mc_free(struct mem_ctl_info *mci)
{
<<<<<<< HEAD
	int i, chn, row;
	struct csrow_info *csr;
	const unsigned int tot_dimms = mci->tot_dimms;
	const unsigned int tot_channels = mci->num_cschannel;
	const unsigned int tot_csrows = mci->nr_csrows;

	if (mci->dimms) {
		for (i = 0; i < tot_dimms; i++)
			kfree(mci->dimms[i]);
		kfree(mci->dimms);
	}
	if (mci->csrows) {
		for (row = 0; row < tot_csrows; row++) {
			csr = mci->csrows[row];
			if (csr) {
				if (csr->channels) {
					for (chn = 0; chn < tot_channels; chn++)
						kfree(csr->channels[chn]);
					kfree(csr->channels);
				}
				kfree(csr);
			}
=======
	put_device(&mci->dev);
}

static void mci_release(struct device *dev)
{
	struct mem_ctl_info *mci = container_of(dev, struct mem_ctl_info, dev);
	struct csrow_info *csr;
	int i, chn, row;

	if (mci->dimms) {
		for (i = 0; i < mci->tot_dimms; i++)
			kfree(mci->dimms[i]);
		kfree(mci->dimms);
	}

	if (mci->csrows) {
		for (row = 0; row < mci->nr_csrows; row++) {
			csr = mci->csrows[row];
			if (!csr)
				continue;

			if (csr->channels) {
				for (chn = 0; chn < mci->num_cschannel; chn++)
					kfree(csr->channels[chn]);
				kfree(csr->channels);
			}
			kfree(csr);
>>>>>>> upstream/android-13
		}
		kfree(mci->csrows);
	}
	kfree(mci);
}

<<<<<<< HEAD
struct mem_ctl_info *edac_mc_alloc(unsigned mc_num,
				   unsigned n_layers,
				   struct edac_mc_layer *layers,
				   unsigned sz_pvt)
{
	struct mem_ctl_info *mci;
	struct edac_mc_layer *layer;
	struct csrow_info *csr;
	struct rank_info *chan;
	struct dimm_info *dimm;
	u32 *ce_per_layer[EDAC_MAX_LAYERS], *ue_per_layer[EDAC_MAX_LAYERS];
	unsigned pos[EDAC_MAX_LAYERS];
	unsigned size, tot_dimms = 1, count = 1;
	unsigned tot_csrows = 1, tot_channels = 1, tot_errcount = 0;
	void *pvt, *p, *ptr = NULL;
	int i, j, row, chn, n, len, off;
	bool per_rank = false;

	BUG_ON(n_layers > EDAC_MAX_LAYERS || n_layers == 0);
=======
static int edac_mc_alloc_csrows(struct mem_ctl_info *mci)
{
	unsigned int tot_channels = mci->num_cschannel;
	unsigned int tot_csrows = mci->nr_csrows;
	unsigned int row, chn;

	/*
	 * Alocate and fill the csrow/channels structs
	 */
	mci->csrows = kcalloc(tot_csrows, sizeof(*mci->csrows), GFP_KERNEL);
	if (!mci->csrows)
		return -ENOMEM;

	for (row = 0; row < tot_csrows; row++) {
		struct csrow_info *csr;

		csr = kzalloc(sizeof(**mci->csrows), GFP_KERNEL);
		if (!csr)
			return -ENOMEM;

		mci->csrows[row] = csr;
		csr->csrow_idx = row;
		csr->mci = mci;
		csr->nr_channels = tot_channels;
		csr->channels = kcalloc(tot_channels, sizeof(*csr->channels),
					GFP_KERNEL);
		if (!csr->channels)
			return -ENOMEM;

		for (chn = 0; chn < tot_channels; chn++) {
			struct rank_info *chan;

			chan = kzalloc(sizeof(**csr->channels), GFP_KERNEL);
			if (!chan)
				return -ENOMEM;

			csr->channels[chn] = chan;
			chan->chan_idx = chn;
			chan->csrow = csr;
		}
	}

	return 0;
}

static int edac_mc_alloc_dimms(struct mem_ctl_info *mci)
{
	unsigned int pos[EDAC_MAX_LAYERS];
	unsigned int row, chn, idx;
	int layer;
	void *p;

	/*
	 * Allocate and fill the dimm structs
	 */
	mci->dimms  = kcalloc(mci->tot_dimms, sizeof(*mci->dimms), GFP_KERNEL);
	if (!mci->dimms)
		return -ENOMEM;

	memset(&pos, 0, sizeof(pos));
	row = 0;
	chn = 0;
	for (idx = 0; idx < mci->tot_dimms; idx++) {
		struct dimm_info *dimm;
		struct rank_info *chan;
		int n, len;

		chan = mci->csrows[row]->channels[chn];

		dimm = kzalloc(sizeof(**mci->dimms), GFP_KERNEL);
		if (!dimm)
			return -ENOMEM;
		mci->dimms[idx] = dimm;
		dimm->mci = mci;
		dimm->idx = idx;

		/*
		 * Copy DIMM location and initialize it.
		 */
		len = sizeof(dimm->label);
		p = dimm->label;
		n = snprintf(p, len, "mc#%u", mci->mc_idx);
		p += n;
		len -= n;
		for (layer = 0; layer < mci->n_layers; layer++) {
			n = snprintf(p, len, "%s#%u",
				     edac_layer_name[mci->layers[layer].type],
				     pos[layer]);
			p += n;
			len -= n;
			dimm->location[layer] = pos[layer];

			if (len <= 0)
				break;
		}

		/* Link it to the csrows old API data */
		chan->dimm = dimm;
		dimm->csrow = row;
		dimm->cschannel = chn;

		/* Increment csrow location */
		if (mci->layers[0].is_virt_csrow) {
			chn++;
			if (chn == mci->num_cschannel) {
				chn = 0;
				row++;
			}
		} else {
			row++;
			if (row == mci->nr_csrows) {
				row = 0;
				chn++;
			}
		}

		/* Increment dimm location */
		for (layer = mci->n_layers - 1; layer >= 0; layer--) {
			pos[layer]++;
			if (pos[layer] < mci->layers[layer].size)
				break;
			pos[layer] = 0;
		}
	}

	return 0;
}

struct mem_ctl_info *edac_mc_alloc(unsigned int mc_num,
				   unsigned int n_layers,
				   struct edac_mc_layer *layers,
				   unsigned int sz_pvt)
{
	struct mem_ctl_info *mci;
	struct edac_mc_layer *layer;
	unsigned int idx, size, tot_dimms = 1;
	unsigned int tot_csrows = 1, tot_channels = 1;
	void *pvt, *ptr = NULL;
	bool per_rank = false;

	if (WARN_ON(n_layers > EDAC_MAX_LAYERS || n_layers == 0))
		return NULL;

>>>>>>> upstream/android-13
	/*
	 * Calculate the total amount of dimms and csrows/cschannels while
	 * in the old API emulation mode
	 */
<<<<<<< HEAD
	for (i = 0; i < n_layers; i++) {
		tot_dimms *= layers[i].size;
		if (layers[i].is_virt_csrow)
			tot_csrows *= layers[i].size;
		else
			tot_channels *= layers[i].size;

		if (layers[i].type == EDAC_MC_LAYER_CHIP_SELECT)
=======
	for (idx = 0; idx < n_layers; idx++) {
		tot_dimms *= layers[idx].size;

		if (layers[idx].is_virt_csrow)
			tot_csrows *= layers[idx].size;
		else
			tot_channels *= layers[idx].size;

		if (layers[idx].type == EDAC_MC_LAYER_CHIP_SELECT)
>>>>>>> upstream/android-13
			per_rank = true;
	}

	/* Figure out the offsets of the various items from the start of an mc
	 * structure.  We want the alignment of each item to be at least as
	 * stringent as what the compiler would provide if we could simply
	 * hardcode everything into a single struct.
	 */
<<<<<<< HEAD
	mci = edac_align_ptr(&ptr, sizeof(*mci), 1);
	layer = edac_align_ptr(&ptr, sizeof(*layer), n_layers);
	for (i = 0; i < n_layers; i++) {
		count *= layers[i].size;
		edac_dbg(4, "errcount layer %d size %d\n", i, count);
		ce_per_layer[i] = edac_align_ptr(&ptr, sizeof(u32), count);
		ue_per_layer[i] = edac_align_ptr(&ptr, sizeof(u32), count);
		tot_errcount += 2 * count;
	}

	edac_dbg(4, "allocating %d error counters\n", tot_errcount);
	pvt = edac_align_ptr(&ptr, sz_pvt, 1);
	size = ((unsigned long)pvt) + sz_pvt;
=======
	mci	= edac_align_ptr(&ptr, sizeof(*mci), 1);
	layer	= edac_align_ptr(&ptr, sizeof(*layer), n_layers);
	pvt	= edac_align_ptr(&ptr, sz_pvt, 1);
	size	= ((unsigned long)pvt) + sz_pvt;
>>>>>>> upstream/android-13

	edac_dbg(1, "allocating %u bytes for mci data (%d %s, %d csrows/channels)\n",
		 size,
		 tot_dimms,
		 per_rank ? "ranks" : "dimms",
		 tot_csrows * tot_channels);

	mci = kzalloc(size, GFP_KERNEL);
	if (mci == NULL)
		return NULL;

<<<<<<< HEAD
=======
	mci->dev.release = mci_release;
	device_initialize(&mci->dev);

>>>>>>> upstream/android-13
	/* Adjust pointers so they point within the memory we just allocated
	 * rather than an imaginary chunk of memory located at address 0.
	 */
	layer = (struct edac_mc_layer *)(((char *)mci) + ((unsigned long)layer));
<<<<<<< HEAD
	for (i = 0; i < n_layers; i++) {
		mci->ce_per_layer[i] = (u32 *)((char *)mci + ((unsigned long)ce_per_layer[i]));
		mci->ue_per_layer[i] = (u32 *)((char *)mci + ((unsigned long)ue_per_layer[i]));
	}
=======
>>>>>>> upstream/android-13
	pvt = sz_pvt ? (((char *)mci) + ((unsigned long)pvt)) : NULL;

	/* setup index and various internal pointers */
	mci->mc_idx = mc_num;
	mci->tot_dimms = tot_dimms;
	mci->pvt_info = pvt;
	mci->n_layers = n_layers;
	mci->layers = layer;
	memcpy(mci->layers, layers, sizeof(*layer) * n_layers);
	mci->nr_csrows = tot_csrows;
	mci->num_cschannel = tot_channels;
	mci->csbased = per_rank;

<<<<<<< HEAD
	/*
	 * Alocate and fill the csrow/channels structs
	 */
	mci->csrows = kcalloc(tot_csrows, sizeof(*mci->csrows), GFP_KERNEL);
	if (!mci->csrows)
		goto error;
	for (row = 0; row < tot_csrows; row++) {
		csr = kzalloc(sizeof(**mci->csrows), GFP_KERNEL);
		if (!csr)
			goto error;
		mci->csrows[row] = csr;
		csr->csrow_idx = row;
		csr->mci = mci;
		csr->nr_channels = tot_channels;
		csr->channels = kcalloc(tot_channels, sizeof(*csr->channels),
					GFP_KERNEL);
		if (!csr->channels)
			goto error;

		for (chn = 0; chn < tot_channels; chn++) {
			chan = kzalloc(sizeof(**csr->channels), GFP_KERNEL);
			if (!chan)
				goto error;
			csr->channels[chn] = chan;
			chan->chan_idx = chn;
			chan->csrow = csr;
		}
	}

	/*
	 * Allocate and fill the dimm structs
	 */
	mci->dimms  = kcalloc(tot_dimms, sizeof(*mci->dimms), GFP_KERNEL);
	if (!mci->dimms)
		goto error;

	memset(&pos, 0, sizeof(pos));
	row = 0;
	chn = 0;
	for (i = 0; i < tot_dimms; i++) {
		chan = mci->csrows[row]->channels[chn];
		off = EDAC_DIMM_OFF(layer, n_layers, pos[0], pos[1], pos[2]);
		if (off < 0 || off >= tot_dimms) {
			edac_mc_printk(mci, KERN_ERR, "EDAC core bug: EDAC_DIMM_OFF is trying to do an illegal data access\n");
			goto error;
		}

		dimm = kzalloc(sizeof(**mci->dimms), GFP_KERNEL);
		if (!dimm)
			goto error;
		mci->dimms[off] = dimm;
		dimm->mci = mci;

		/*
		 * Copy DIMM location and initialize it.
		 */
		len = sizeof(dimm->label);
		p = dimm->label;
		n = snprintf(p, len, "mc#%u", mc_num);
		p += n;
		len -= n;
		for (j = 0; j < n_layers; j++) {
			n = snprintf(p, len, "%s#%u",
				     edac_layer_name[layers[j].type],
				     pos[j]);
			p += n;
			len -= n;
			dimm->location[j] = pos[j];

			if (len <= 0)
				break;
		}

		/* Link it to the csrows old API data */
		chan->dimm = dimm;
		dimm->csrow = row;
		dimm->cschannel = chn;

		/* Increment csrow location */
		if (layers[0].is_virt_csrow) {
			chn++;
			if (chn == tot_channels) {
				chn = 0;
				row++;
			}
		} else {
			row++;
			if (row == tot_csrows) {
				row = 0;
				chn++;
			}
		}

		/* Increment dimm location */
		for (j = n_layers - 1; j >= 0; j--) {
			pos[j]++;
			if (pos[j] < layers[j].size)
				break;
			pos[j] = 0;
		}
	}
=======
	if (edac_mc_alloc_csrows(mci))
		goto error;

	if (edac_mc_alloc_dimms(mci))
		goto error;
>>>>>>> upstream/android-13

	mci->op_state = OP_ALLOC;

	return mci;

error:
	_edac_mc_free(mci);

	return NULL;
}
EXPORT_SYMBOL_GPL(edac_mc_alloc);

void edac_mc_free(struct mem_ctl_info *mci)
{
	edac_dbg(1, "\n");

<<<<<<< HEAD
	/* If we're not yet registered with sysfs free only what was allocated
	 * in edac_mc_alloc().
	 */
	if (!device_is_registered(&mci->dev)) {
		_edac_mc_free(mci);
		return;
	}

	/* the mci instance is freed here, when the sysfs object is dropped */
	edac_unregister_sysfs(mci);
=======
	_edac_mc_free(mci);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(edac_mc_free);

bool edac_has_mcs(void)
{
	bool ret;

	mutex_lock(&mem_ctls_mutex);

	ret = list_empty(&mc_devices);

	mutex_unlock(&mem_ctls_mutex);

	return !ret;
}
EXPORT_SYMBOL_GPL(edac_has_mcs);

/* Caller must hold mem_ctls_mutex */
static struct mem_ctl_info *__find_mci_by_dev(struct device *dev)
{
	struct mem_ctl_info *mci;
	struct list_head *item;

	edac_dbg(3, "\n");

	list_for_each(item, &mc_devices) {
		mci = list_entry(item, struct mem_ctl_info, link);

		if (mci->pdev == dev)
			return mci;
	}

	return NULL;
}

/**
 * find_mci_by_dev
 *
 *	scan list of controllers looking for the one that manages
 *	the 'dev' device
 * @dev: pointer to a struct device related with the MCI
 */
struct mem_ctl_info *find_mci_by_dev(struct device *dev)
{
	struct mem_ctl_info *ret;

	mutex_lock(&mem_ctls_mutex);
	ret = __find_mci_by_dev(dev);
	mutex_unlock(&mem_ctls_mutex);

	return ret;
}
EXPORT_SYMBOL_GPL(find_mci_by_dev);

/*
 * edac_mc_workq_function
 *	performs the operation scheduled by a workq request
 */
static void edac_mc_workq_function(struct work_struct *work_req)
{
	struct delayed_work *d_work = to_delayed_work(work_req);
	struct mem_ctl_info *mci = to_edac_mem_ctl_work(d_work);

	mutex_lock(&mem_ctls_mutex);

	if (mci->op_state != OP_RUNNING_POLL) {
		mutex_unlock(&mem_ctls_mutex);
		return;
	}

	if (edac_op_state == EDAC_OPSTATE_POLL)
		mci->edac_check(mci);

	mutex_unlock(&mem_ctls_mutex);

	/* Queue ourselves again. */
	edac_queue_work(&mci->work, msecs_to_jiffies(edac_mc_get_poll_msec()));
}

/*
 * edac_mc_reset_delay_period(unsigned long value)
 *
 *	user space has updated our poll period value, need to
 *	reset our workq delays
 */
void edac_mc_reset_delay_period(unsigned long value)
{
	struct mem_ctl_info *mci;
	struct list_head *item;

	mutex_lock(&mem_ctls_mutex);

	list_for_each(item, &mc_devices) {
		mci = list_entry(item, struct mem_ctl_info, link);

		if (mci->op_state == OP_RUNNING_POLL)
			edac_mod_work(&mci->work, value);
	}
	mutex_unlock(&mem_ctls_mutex);
}



/* Return 0 on success, 1 on failure.
 * Before calling this function, caller must
 * assign a unique value to mci->mc_idx.
 *
 *	locking model:
 *
 *		called with the mem_ctls_mutex lock held
 */
static int add_mc_to_global_list(struct mem_ctl_info *mci)
{
	struct list_head *item, *insert_before;
	struct mem_ctl_info *p;

	insert_before = &mc_devices;

	p = __find_mci_by_dev(mci->pdev);
	if (unlikely(p != NULL))
		goto fail0;

	list_for_each(item, &mc_devices) {
		p = list_entry(item, struct mem_ctl_info, link);

		if (p->mc_idx >= mci->mc_idx) {
			if (unlikely(p->mc_idx == mci->mc_idx))
				goto fail1;

			insert_before = item;
			break;
		}
	}

	list_add_tail_rcu(&mci->link, insert_before);
	return 0;

fail0:
	edac_printk(KERN_WARNING, EDAC_MC,
		"%s (%s) %s %s already assigned %d\n", dev_name(p->pdev),
		edac_dev_name(mci), p->mod_name, p->ctl_name, p->mc_idx);
	return 1;

fail1:
	edac_printk(KERN_WARNING, EDAC_MC,
		"bug in low-level driver: attempt to assign\n"
		"    duplicate mc_idx %d in %s()\n", p->mc_idx, __func__);
	return 1;
}

static int del_mc_from_global_list(struct mem_ctl_info *mci)
{
	list_del_rcu(&mci->link);

	/* these are for safe removal of devices from global list while
	 * NMI handlers may be traversing list
	 */
	synchronize_rcu();
	INIT_LIST_HEAD(&mci->link);

	return list_empty(&mc_devices);
}

struct mem_ctl_info *edac_mc_find(int idx)
{
	struct mem_ctl_info *mci;
	struct list_head *item;

	mutex_lock(&mem_ctls_mutex);

	list_for_each(item, &mc_devices) {
		mci = list_entry(item, struct mem_ctl_info, link);
		if (mci->mc_idx == idx)
			goto unlock;
	}

	mci = NULL;
unlock:
	mutex_unlock(&mem_ctls_mutex);
	return mci;
}
EXPORT_SYMBOL(edac_mc_find);

const char *edac_get_owner(void)
{
	return edac_mc_owner;
}
EXPORT_SYMBOL_GPL(edac_get_owner);

/* FIXME - should a warning be printed if no error detection? correction? */
int edac_mc_add_mc_with_groups(struct mem_ctl_info *mci,
			       const struct attribute_group **groups)
{
	int ret = -EINVAL;
	edac_dbg(0, "\n");

<<<<<<< HEAD
	if (mci->mc_idx >= EDAC_MAX_MCS) {
		pr_warn_once("Too many memory controllers: %d\n", mci->mc_idx);
		return -ENODEV;
	}

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_EDAC_DEBUG
	if (edac_debug_level >= 3)
		edac_mc_dump_mci(mci);

	if (edac_debug_level >= 4) {
<<<<<<< HEAD
=======
		struct dimm_info *dimm;
>>>>>>> upstream/android-13
		int i;

		for (i = 0; i < mci->nr_csrows; i++) {
			struct csrow_info *csrow = mci->csrows[i];
			u32 nr_pages = 0;
			int j;

			for (j = 0; j < csrow->nr_channels; j++)
				nr_pages += csrow->channels[j]->dimm->nr_pages;
			if (!nr_pages)
				continue;
			edac_mc_dump_csrow(csrow);
			for (j = 0; j < csrow->nr_channels; j++)
				if (csrow->channels[j]->dimm->nr_pages)
					edac_mc_dump_channel(csrow->channels[j]);
		}
<<<<<<< HEAD
		for (i = 0; i < mci->tot_dimms; i++)
			if (mci->dimms[i]->nr_pages)
				edac_mc_dump_dimm(mci->dimms[i], i);
=======

		mci_for_each_dimm(mci, dimm)
			edac_mc_dump_dimm(dimm);
>>>>>>> upstream/android-13
	}
#endif
	mutex_lock(&mem_ctls_mutex);

	if (edac_mc_owner && edac_mc_owner != mci->mod_name) {
		ret = -EPERM;
		goto fail0;
	}

	if (add_mc_to_global_list(mci))
		goto fail0;

	/* set load time so that error rate can be tracked */
	mci->start_time = jiffies;

<<<<<<< HEAD
	mci->bus = &mc_bus[mci->mc_idx];
=======
	mci->bus = edac_get_sysfs_subsys();
>>>>>>> upstream/android-13

	if (edac_create_sysfs_mci_device(mci, groups)) {
		edac_mc_printk(mci, KERN_WARNING,
			"failed to create sysfs device\n");
		goto fail1;
	}

	if (mci->edac_check) {
		mci->op_state = OP_RUNNING_POLL;

		INIT_DELAYED_WORK(&mci->work, edac_mc_workq_function);
		edac_queue_work(&mci->work, msecs_to_jiffies(edac_mc_get_poll_msec()));

	} else {
		mci->op_state = OP_RUNNING_INTERRUPT;
	}

	/* Report action taken */
	edac_mc_printk(mci, KERN_INFO,
		"Giving out device to module %s controller %s: DEV %s (%s)\n",
		mci->mod_name, mci->ctl_name, mci->dev_name,
		edac_op_state_to_string(mci->op_state));

	edac_mc_owner = mci->mod_name;

	mutex_unlock(&mem_ctls_mutex);
	return 0;

fail1:
	del_mc_from_global_list(mci);

fail0:
	mutex_unlock(&mem_ctls_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(edac_mc_add_mc_with_groups);

struct mem_ctl_info *edac_mc_del_mc(struct device *dev)
{
	struct mem_ctl_info *mci;

	edac_dbg(0, "\n");

	mutex_lock(&mem_ctls_mutex);

	/* find the requested mci struct in the global list */
	mci = __find_mci_by_dev(dev);
	if (mci == NULL) {
		mutex_unlock(&mem_ctls_mutex);
		return NULL;
	}

	/* mark MCI offline: */
	mci->op_state = OP_OFFLINE;

	if (del_mc_from_global_list(mci))
		edac_mc_owner = NULL;

	mutex_unlock(&mem_ctls_mutex);

	if (mci->edac_check)
		edac_stop_work(&mci->work);

	/* remove from sysfs */
	edac_remove_sysfs_mci_device(mci);

	edac_printk(KERN_INFO, EDAC_MC,
		"Removed device %d for %s %s: DEV %s\n", mci->mc_idx,
		mci->mod_name, mci->ctl_name, edac_dev_name(mci));

	return mci;
}
EXPORT_SYMBOL_GPL(edac_mc_del_mc);

static void edac_mc_scrub_block(unsigned long page, unsigned long offset,
				u32 size)
{
	struct page *pg;
	void *virt_addr;
	unsigned long flags = 0;

	edac_dbg(3, "\n");

	/* ECC error page was not in our memory. Ignore it. */
	if (!pfn_valid(page))
		return;

	/* Find the actual page structure then map it and fix */
	pg = pfn_to_page(page);

	if (PageHighMem(pg))
		local_irq_save(flags);

	virt_addr = kmap_atomic(pg);

	/* Perform architecture specific atomic scrub operation */
	edac_atomic_scrub(virt_addr + offset, size);

	/* Unmap and complete */
	kunmap_atomic(virt_addr);

	if (PageHighMem(pg))
		local_irq_restore(flags);
}

/* FIXME - should return -1 */
int edac_mc_find_csrow_by_page(struct mem_ctl_info *mci, unsigned long page)
{
	struct csrow_info **csrows = mci->csrows;
	int row, i, j, n;

	edac_dbg(1, "MC%d: 0x%lx\n", mci->mc_idx, page);
	row = -1;

	for (i = 0; i < mci->nr_csrows; i++) {
		struct csrow_info *csrow = csrows[i];
		n = 0;
		for (j = 0; j < csrow->nr_channels; j++) {
			struct dimm_info *dimm = csrow->channels[j]->dimm;
			n += dimm->nr_pages;
		}
		if (n == 0)
			continue;

		edac_dbg(3, "MC%d: first(0x%lx) page(0x%lx) last(0x%lx) mask(0x%lx)\n",
			 mci->mc_idx,
			 csrow->first_page, page, csrow->last_page,
			 csrow->page_mask);

		if ((page >= csrow->first_page) &&
		    (page <= csrow->last_page) &&
		    ((page & csrow->page_mask) ==
		     (csrow->first_page & csrow->page_mask))) {
			row = i;
			break;
		}
	}

	if (row == -1)
		edac_mc_printk(mci, KERN_ERR,
			"could not look up page error address %lx\n",
			(unsigned long)page);

	return row;
}
EXPORT_SYMBOL_GPL(edac_mc_find_csrow_by_page);

const char *edac_layer_name[] = {
	[EDAC_MC_LAYER_BRANCH] = "branch",
	[EDAC_MC_LAYER_CHANNEL] = "channel",
	[EDAC_MC_LAYER_SLOT] = "slot",
	[EDAC_MC_LAYER_CHIP_SELECT] = "csrow",
	[EDAC_MC_LAYER_ALL_MEM] = "memory",
};
EXPORT_SYMBOL_GPL(edac_layer_name);

<<<<<<< HEAD
static void edac_inc_ce_error(struct mem_ctl_info *mci,
			      bool enable_per_layer_report,
			      const int pos[EDAC_MAX_LAYERS],
			      const u16 count)
{
	int i, index = 0;

	mci->ce_mc += count;

	if (!enable_per_layer_report) {
		mci->ce_noinfo_count += count;
		return;
	}

	for (i = 0; i < mci->n_layers; i++) {
		if (pos[i] < 0)
			break;
		index += pos[i];
		mci->ce_per_layer[i][index] += count;

		if (i < mci->n_layers - 1)
			index *= mci->layers[i + 1].size;
	}
}

static void edac_inc_ue_error(struct mem_ctl_info *mci,
				    bool enable_per_layer_report,
				    const int pos[EDAC_MAX_LAYERS],
				    const u16 count)
{
	int i, index = 0;

	mci->ue_mc += count;

	if (!enable_per_layer_report) {
		mci->ue_noinfo_count += count;
		return;
	}

	for (i = 0; i < mci->n_layers; i++) {
		if (pos[i] < 0)
			break;
		index += pos[i];
		mci->ue_per_layer[i][index] += count;

		if (i < mci->n_layers - 1)
			index *= mci->layers[i + 1].size;
	}
}

static void edac_ce_error(struct mem_ctl_info *mci,
			  const u16 error_count,
			  const int pos[EDAC_MAX_LAYERS],
			  const char *msg,
			  const char *location,
			  const char *label,
			  const char *detail,
			  const char *other_detail,
			  const bool enable_per_layer_report,
			  const unsigned long page_frame_number,
			  const unsigned long offset_in_page,
			  long grain)
{
	unsigned long remapped_page;
	char *msg_aux = "";

	if (*msg)
		msg_aux = " ";

	if (edac_mc_get_log_ce()) {
		if (other_detail && *other_detail)
			edac_mc_printk(mci, KERN_WARNING,
				       "%d CE %s%son %s (%s %s - %s)\n",
				       error_count, msg, msg_aux, label,
				       location, detail, other_detail);
		else
			edac_mc_printk(mci, KERN_WARNING,
				       "%d CE %s%son %s (%s %s)\n",
				       error_count, msg, msg_aux, label,
				       location, detail);
	}
	edac_inc_ce_error(mci, enable_per_layer_report, pos, error_count);
=======
static void edac_inc_ce_error(struct edac_raw_error_desc *e)
{
	int pos[EDAC_MAX_LAYERS] = { e->top_layer, e->mid_layer, e->low_layer };
	struct mem_ctl_info *mci = error_desc_to_mci(e);
	struct dimm_info *dimm = edac_get_dimm(mci, pos[0], pos[1], pos[2]);

	mci->ce_mc += e->error_count;

	if (dimm)
		dimm->ce_count += e->error_count;
	else
		mci->ce_noinfo_count += e->error_count;
}

static void edac_inc_ue_error(struct edac_raw_error_desc *e)
{
	int pos[EDAC_MAX_LAYERS] = { e->top_layer, e->mid_layer, e->low_layer };
	struct mem_ctl_info *mci = error_desc_to_mci(e);
	struct dimm_info *dimm = edac_get_dimm(mci, pos[0], pos[1], pos[2]);

	mci->ue_mc += e->error_count;

	if (dimm)
		dimm->ue_count += e->error_count;
	else
		mci->ue_noinfo_count += e->error_count;
}

static void edac_ce_error(struct edac_raw_error_desc *e)
{
	struct mem_ctl_info *mci = error_desc_to_mci(e);
	unsigned long remapped_page;

	if (edac_mc_get_log_ce()) {
		edac_mc_printk(mci, KERN_WARNING,
			"%d CE %s%son %s (%s page:0x%lx offset:0x%lx grain:%ld syndrome:0x%lx%s%s)\n",
			e->error_count, e->msg,
			*e->msg ? " " : "",
			e->label, e->location, e->page_frame_number, e->offset_in_page,
			e->grain, e->syndrome,
			*e->other_detail ? " - " : "",
			e->other_detail);
	}

	edac_inc_ce_error(e);
>>>>>>> upstream/android-13

	if (mci->scrub_mode == SCRUB_SW_SRC) {
		/*
			* Some memory controllers (called MCs below) can remap
			* memory so that it is still available at a different
			* address when PCI devices map into memory.
			* MC's that can't do this, lose the memory where PCI
			* devices are mapped. This mapping is MC-dependent
			* and so we call back into the MC driver for it to
			* map the MC page to a physical (CPU) page which can
			* then be mapped to a virtual page - which can then
			* be scrubbed.
			*/
		remapped_page = mci->ctl_page_to_phys ?
<<<<<<< HEAD
			mci->ctl_page_to_phys(mci, page_frame_number) :
			page_frame_number;

		edac_mc_scrub_block(remapped_page,
					offset_in_page, grain);
	}
}

static void edac_ue_error(struct mem_ctl_info *mci,
			  const u16 error_count,
			  const int pos[EDAC_MAX_LAYERS],
			  const char *msg,
			  const char *location,
			  const char *label,
			  const char *detail,
			  const char *other_detail,
			  const bool enable_per_layer_report)
{
	char *msg_aux = "";

	if (*msg)
		msg_aux = " ";

	if (edac_mc_get_log_ue()) {
		if (other_detail && *other_detail)
			edac_mc_printk(mci, KERN_WARNING,
				       "%d UE %s%son %s (%s %s - %s)\n",
				       error_count, msg, msg_aux, label,
				       location, detail, other_detail);
		else
			edac_mc_printk(mci, KERN_WARNING,
				       "%d UE %s%son %s (%s %s)\n",
				       error_count, msg, msg_aux, label,
				       location, detail);
	}

	if (edac_mc_get_panic_on_ue()) {
		if (other_detail && *other_detail)
			panic("UE %s%son %s (%s%s - %s)\n",
			      msg, msg_aux, label, location, detail, other_detail);
		else
			panic("UE %s%son %s (%s%s)\n",
			      msg, msg_aux, label, location, detail);
	}

	edac_inc_ue_error(mci, enable_per_layer_report, pos, error_count);
}

void edac_raw_mc_handle_error(const enum hw_event_mc_err_type type,
			      struct mem_ctl_info *mci,
			      struct edac_raw_error_desc *e)
{
	char detail[80];
	int pos[EDAC_MAX_LAYERS] = { e->top_layer, e->mid_layer, e->low_layer };

	/* Memory type dependent details about the error */
	if (type == HW_EVENT_ERR_CORRECTED) {
		snprintf(detail, sizeof(detail),
			"page:0x%lx offset:0x%lx grain:%ld syndrome:0x%lx",
			e->page_frame_number, e->offset_in_page,
			e->grain, e->syndrome);
		edac_ce_error(mci, e->error_count, pos, e->msg, e->location, e->label,
			      detail, e->other_detail, e->enable_per_layer_report,
			      e->page_frame_number, e->offset_in_page, e->grain);
	} else {
		snprintf(detail, sizeof(detail),
			"page:0x%lx offset:0x%lx grain:%ld",
			e->page_frame_number, e->offset_in_page, e->grain);

		edac_ue_error(mci, e->error_count, pos, e->msg, e->location, e->label,
			      detail, e->other_detail, e->enable_per_layer_report);
	}


=======
			mci->ctl_page_to_phys(mci, e->page_frame_number) :
			e->page_frame_number;

		edac_mc_scrub_block(remapped_page, e->offset_in_page, e->grain);
	}
}

static void edac_ue_error(struct edac_raw_error_desc *e)
{
	struct mem_ctl_info *mci = error_desc_to_mci(e);

	if (edac_mc_get_log_ue()) {
		edac_mc_printk(mci, KERN_WARNING,
			"%d UE %s%son %s (%s page:0x%lx offset:0x%lx grain:%ld%s%s)\n",
			e->error_count, e->msg,
			*e->msg ? " " : "",
			e->label, e->location, e->page_frame_number, e->offset_in_page,
			e->grain,
			*e->other_detail ? " - " : "",
			e->other_detail);
	}

	edac_inc_ue_error(e);

	if (edac_mc_get_panic_on_ue()) {
		panic("UE %s%son %s (%s page:0x%lx offset:0x%lx grain:%ld%s%s)\n",
			e->msg,
			*e->msg ? " " : "",
			e->label, e->location, e->page_frame_number, e->offset_in_page,
			e->grain,
			*e->other_detail ? " - " : "",
			e->other_detail);
	}
}

static void edac_inc_csrow(struct edac_raw_error_desc *e, int row, int chan)
{
	struct mem_ctl_info *mci = error_desc_to_mci(e);
	enum hw_event_mc_err_type type = e->type;
	u16 count = e->error_count;

	if (row < 0)
		return;

	edac_dbg(4, "csrow/channel to increment: (%d,%d)\n", row, chan);

	if (type == HW_EVENT_ERR_CORRECTED) {
		mci->csrows[row]->ce_count += count;
		if (chan >= 0)
			mci->csrows[row]->channels[chan]->ce_count += count;
	} else {
		mci->csrows[row]->ue_count += count;
	}
}

void edac_raw_mc_handle_error(struct edac_raw_error_desc *e)
{
	struct mem_ctl_info *mci = error_desc_to_mci(e);
	u8 grain_bits;

	/* Sanity-check driver-supplied grain value. */
	if (WARN_ON_ONCE(!e->grain))
		e->grain = 1;

	grain_bits = fls_long(e->grain - 1);

	/* Report the error via the trace interface */
	if (IS_ENABLED(CONFIG_RAS))
		trace_mc_event(e->type, e->msg, e->label, e->error_count,
			       mci->mc_idx, e->top_layer, e->mid_layer,
			       e->low_layer,
			       (e->page_frame_number << PAGE_SHIFT) | e->offset_in_page,
			       grain_bits, e->syndrome, e->other_detail);

	if (e->type == HW_EVENT_ERR_CORRECTED)
		edac_ce_error(e);
	else
		edac_ue_error(e);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(edac_raw_mc_handle_error);

void edac_mc_handle_error(const enum hw_event_mc_err_type type,
			  struct mem_ctl_info *mci,
			  const u16 error_count,
			  const unsigned long page_frame_number,
			  const unsigned long offset_in_page,
			  const unsigned long syndrome,
			  const int top_layer,
			  const int mid_layer,
			  const int low_layer,
			  const char *msg,
			  const char *other_detail)
{
<<<<<<< HEAD
=======
	struct dimm_info *dimm;
>>>>>>> upstream/android-13
	char *p;
	int row = -1, chan = -1;
	int pos[EDAC_MAX_LAYERS] = { top_layer, mid_layer, low_layer };
	int i, n_labels = 0;
<<<<<<< HEAD
	u8 grain_bits;
	struct edac_raw_error_desc *e = &mci->error_desc;
=======
	struct edac_raw_error_desc *e = &mci->error_desc;
	bool any_memory = true;
>>>>>>> upstream/android-13

	edac_dbg(3, "MC%d\n", mci->mc_idx);

	/* Fills the error report buffer */
	memset(e, 0, sizeof (*e));
	e->error_count = error_count;
<<<<<<< HEAD
=======
	e->type = type;
>>>>>>> upstream/android-13
	e->top_layer = top_layer;
	e->mid_layer = mid_layer;
	e->low_layer = low_layer;
	e->page_frame_number = page_frame_number;
	e->offset_in_page = offset_in_page;
	e->syndrome = syndrome;
<<<<<<< HEAD
	e->msg = msg;
	e->other_detail = other_detail;

	/*
	 * Check if the event report is consistent and if the memory
	 * location is known. If it is known, enable_per_layer_report will be
	 * true, the DIMM(s) label info will be filled and the per-layer
=======
	/* need valid strings here for both: */
	e->msg = msg ?: "";
	e->other_detail = other_detail ?: "";

	/*
	 * Check if the event report is consistent and if the memory location is
	 * known. If it is, the DIMM(s) label info will be filled and the DIMM's
>>>>>>> upstream/android-13
	 * error counters will be incremented.
	 */
	for (i = 0; i < mci->n_layers; i++) {
		if (pos[i] >= (int)mci->layers[i].size) {

			edac_mc_printk(mci, KERN_ERR,
				       "INTERNAL ERROR: %s value is out of range (%d >= %d)\n",
				       edac_layer_name[mci->layers[i].type],
				       pos[i], mci->layers[i].size);
			/*
			 * Instead of just returning it, let's use what's
			 * known about the error. The increment routines and
			 * the DIMM filter logic will do the right thing by
			 * pointing the likely damaged DIMMs.
			 */
			pos[i] = -1;
		}
		if (pos[i] >= 0)
<<<<<<< HEAD
			e->enable_per_layer_report = true;
=======
			any_memory = false;
>>>>>>> upstream/android-13
	}

	/*
	 * Get the dimm label/grain that applies to the match criteria.
	 * As the error algorithm may not be able to point to just one memory
	 * stick, the logic here will get all possible labels that could
	 * pottentially be affected by the error.
	 * On FB-DIMM memory controllers, for uncorrected errors, it is common
	 * to have only the MC channel and the MC dimm (also called "branch")
	 * but the channel is not known, as the memory is arranged in pairs,
	 * where each memory belongs to a separate channel within the same
	 * branch.
	 */
	p = e->label;
	*p = '\0';

<<<<<<< HEAD
	for (i = 0; i < mci->tot_dimms; i++) {
		struct dimm_info *dimm = mci->dimms[i];

=======
	mci_for_each_dimm(mci, dimm) {
>>>>>>> upstream/android-13
		if (top_layer >= 0 && top_layer != dimm->location[0])
			continue;
		if (mid_layer >= 0 && mid_layer != dimm->location[1])
			continue;
		if (low_layer >= 0 && low_layer != dimm->location[2])
			continue;

		/* get the max grain, over the error match range */
		if (dimm->grain > e->grain)
			e->grain = dimm->grain;

		/*
		 * If the error is memory-controller wide, there's no need to
<<<<<<< HEAD
		 * seek for the affected DIMMs because the whole
		 * channel/memory controller/...  may be affected.
		 * Also, don't show errors for empty DIMM slots.
		 */
		if (e->enable_per_layer_report && dimm->nr_pages) {
			if (n_labels >= EDAC_MAX_LABELS) {
				e->enable_per_layer_report = false;
				break;
			}
			n_labels++;
=======
		 * seek for the affected DIMMs because the whole channel/memory
		 * controller/... may be affected. Also, don't show errors for
		 * empty DIMM slots.
		 */
		if (!dimm->nr_pages)
			continue;

		n_labels++;
		if (n_labels > EDAC_MAX_LABELS) {
			p = e->label;
			*p = '\0';
		} else {
>>>>>>> upstream/android-13
			if (p != e->label) {
				strcpy(p, OTHER_LABEL);
				p += strlen(OTHER_LABEL);
			}
			strcpy(p, dimm->label);
			p += strlen(p);
<<<<<<< HEAD
			*p = '\0';

			/*
			 * get csrow/channel of the DIMM, in order to allow
			 * incrementing the compat API counters
			 */
			edac_dbg(4, "%s csrows map: (%d,%d)\n",
				 mci->csbased ? "rank" : "dimm",
				 dimm->csrow, dimm->cschannel);
			if (row == -1)
				row = dimm->csrow;
			else if (row >= 0 && row != dimm->csrow)
				row = -2;

			if (chan == -1)
				chan = dimm->cschannel;
			else if (chan >= 0 && chan != dimm->cschannel)
				chan = -2;
		}
	}

	if (!e->enable_per_layer_report) {
		strcpy(e->label, "any memory");
	} else {
		edac_dbg(4, "csrow/channel to increment: (%d,%d)\n", row, chan);
		if (p == e->label)
			strcpy(e->label, "unknown memory");
		if (type == HW_EVENT_ERR_CORRECTED) {
			if (row >= 0) {
				mci->csrows[row]->ce_count += error_count;
				if (chan >= 0)
					mci->csrows[row]->channels[chan]->ce_count += error_count;
			}
		} else
			if (row >= 0)
				mci->csrows[row]->ue_count += error_count;
	}
=======
		}

		/*
		 * get csrow/channel of the DIMM, in order to allow
		 * incrementing the compat API counters
		 */
		edac_dbg(4, "%s csrows map: (%d,%d)\n",
			mci->csbased ? "rank" : "dimm",
			dimm->csrow, dimm->cschannel);
		if (row == -1)
			row = dimm->csrow;
		else if (row >= 0 && row != dimm->csrow)
			row = -2;

		if (chan == -1)
			chan = dimm->cschannel;
		else if (chan >= 0 && chan != dimm->cschannel)
			chan = -2;
	}

	if (any_memory)
		strcpy(e->label, "any memory");
	else if (!*e->label)
		strcpy(e->label, "unknown memory");

	edac_inc_csrow(e, row, chan);
>>>>>>> upstream/android-13

	/* Fill the RAM location data */
	p = e->location;

	for (i = 0; i < mci->n_layers; i++) {
		if (pos[i] < 0)
			continue;

		p += sprintf(p, "%s:%d ",
			     edac_layer_name[mci->layers[i].type],
			     pos[i]);
	}
	if (p > e->location)
		*(p - 1) = '\0';

<<<<<<< HEAD
	/* Sanity-check driver-supplied grain value. */
	if (WARN_ON_ONCE(!e->grain))
		e->grain = 1;

	grain_bits = fls_long(e->grain - 1);

	/* Report the error via the trace interface */
	if (IS_ENABLED(CONFIG_RAS))
		trace_mc_event(type, e->msg, e->label, e->error_count,
			       mci->mc_idx, e->top_layer, e->mid_layer,
			       e->low_layer,
			       (e->page_frame_number << PAGE_SHIFT) | e->offset_in_page,
			       grain_bits, e->syndrome, e->other_detail);

	edac_raw_mc_handle_error(type, mci, e);
=======
	edac_raw_mc_handle_error(e);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(edac_mc_handle_error);
