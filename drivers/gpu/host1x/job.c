<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Tegra host1x Job
 *
 * Copyright (c) 2010-2015, NVIDIA Corporation.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/host1x.h>
<<<<<<< HEAD
=======
#include <linux/iommu.h>
>>>>>>> upstream/android-13
#include <linux/kref.h>
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <trace/events/host1x.h>

#include "channel.h"
#include "dev.h"
#include "job.h"
#include "syncpt.h"

#define HOST1X_WAIT_SYNCPT_OFFSET 0x8

struct host1x_job *host1x_job_alloc(struct host1x_channel *ch,
<<<<<<< HEAD
				    u32 num_cmdbufs, u32 num_relocs)
{
	struct host1x_job *job = NULL;
	unsigned int num_unpins = num_cmdbufs + num_relocs;
	u64 total;
	void *mem;

=======
				    u32 num_cmdbufs, u32 num_relocs,
				    bool skip_firewall)
{
	struct host1x_job *job = NULL;
	unsigned int num_unpins = num_relocs;
	bool enable_firewall;
	u64 total;
	void *mem;

	enable_firewall = IS_ENABLED(CONFIG_TEGRA_HOST1X_FIREWALL) && !skip_firewall;

	if (!enable_firewall)
		num_unpins += num_cmdbufs;

>>>>>>> upstream/android-13
	/* Check that we're not going to overflow */
	total = sizeof(struct host1x_job) +
		(u64)num_relocs * sizeof(struct host1x_reloc) +
		(u64)num_unpins * sizeof(struct host1x_job_unpin_data) +
<<<<<<< HEAD
		(u64)num_cmdbufs * sizeof(struct host1x_job_gather) +
=======
		(u64)num_cmdbufs * sizeof(struct host1x_job_cmd) +
>>>>>>> upstream/android-13
		(u64)num_unpins * sizeof(dma_addr_t) +
		(u64)num_unpins * sizeof(u32 *);
	if (total > ULONG_MAX)
		return NULL;

	mem = job = kzalloc(total, GFP_KERNEL);
	if (!job)
		return NULL;

<<<<<<< HEAD
=======
	job->enable_firewall = enable_firewall;

>>>>>>> upstream/android-13
	kref_init(&job->ref);
	job->channel = ch;

	/* Redistribute memory to the structs  */
	mem += sizeof(struct host1x_job);
	job->relocs = num_relocs ? mem : NULL;
	mem += num_relocs * sizeof(struct host1x_reloc);
	job->unpins = num_unpins ? mem : NULL;
	mem += num_unpins * sizeof(struct host1x_job_unpin_data);
<<<<<<< HEAD
	job->gathers = num_cmdbufs ? mem : NULL;
	mem += num_cmdbufs * sizeof(struct host1x_job_gather);
=======
	job->cmds = num_cmdbufs ? mem : NULL;
	mem += num_cmdbufs * sizeof(struct host1x_job_cmd);
>>>>>>> upstream/android-13
	job->addr_phys = num_unpins ? mem : NULL;

	job->reloc_addr_phys = job->addr_phys;
	job->gather_addr_phys = &job->addr_phys[num_relocs];

	return job;
}
EXPORT_SYMBOL(host1x_job_alloc);

struct host1x_job *host1x_job_get(struct host1x_job *job)
{
	kref_get(&job->ref);
	return job;
}
EXPORT_SYMBOL(host1x_job_get);

static void job_free(struct kref *ref)
{
	struct host1x_job *job = container_of(ref, struct host1x_job, ref);

<<<<<<< HEAD
=======
	if (job->release)
		job->release(job);

	if (job->waiter)
		host1x_intr_put_ref(job->syncpt->host, job->syncpt->id,
				    job->waiter, false);

	if (job->syncpt)
		host1x_syncpt_put(job->syncpt);

>>>>>>> upstream/android-13
	kfree(job);
}

void host1x_job_put(struct host1x_job *job)
{
	kref_put(&job->ref, job_free);
}
EXPORT_SYMBOL(host1x_job_put);

void host1x_job_add_gather(struct host1x_job *job, struct host1x_bo *bo,
			   unsigned int words, unsigned int offset)
{
<<<<<<< HEAD
	struct host1x_job_gather *gather = &job->gathers[job->num_gathers];
=======
	struct host1x_job_gather *gather = &job->cmds[job->num_cmds].gather;
>>>>>>> upstream/android-13

	gather->words = words;
	gather->bo = bo;
	gather->offset = offset;

<<<<<<< HEAD
	job->num_gathers++;
}
EXPORT_SYMBOL(host1x_job_add_gather);

static unsigned int pin_job(struct host1x *host, struct host1x_job *job)
{
	unsigned int i;
	int err;

=======
	job->num_cmds++;
}
EXPORT_SYMBOL(host1x_job_add_gather);

void host1x_job_add_wait(struct host1x_job *job, u32 id, u32 thresh,
			 bool relative, u32 next_class)
{
	struct host1x_job_cmd *cmd = &job->cmds[job->num_cmds];

	cmd->is_wait = true;
	cmd->wait.id = id;
	cmd->wait.threshold = thresh;
	cmd->wait.next_class = next_class;
	cmd->wait.relative = relative;

	job->num_cmds++;
}
EXPORT_SYMBOL(host1x_job_add_wait);

static unsigned int pin_job(struct host1x *host, struct host1x_job *job)
{
	struct host1x_client *client = job->client;
	struct device *dev = client->dev;
	struct host1x_job_gather *g;
	struct iommu_domain *domain;
	struct sg_table *sgt;
	unsigned int i;
	int err;

	domain = iommu_get_domain_for_dev(dev);
>>>>>>> upstream/android-13
	job->num_unpins = 0;

	for (i = 0; i < job->num_relocs; i++) {
		struct host1x_reloc *reloc = &job->relocs[i];
<<<<<<< HEAD
		struct sg_table *sgt;
		dma_addr_t phys_addr;
=======
		dma_addr_t phys_addr, *phys;
>>>>>>> upstream/android-13

		reloc->target.bo = host1x_bo_get(reloc->target.bo);
		if (!reloc->target.bo) {
			err = -EINVAL;
			goto unpin;
		}

<<<<<<< HEAD
		phys_addr = host1x_bo_pin(reloc->target.bo, &sgt);
=======
		/*
		 * If the client device is not attached to an IOMMU, the
		 * physical address of the buffer object can be used.
		 *
		 * Similarly, when an IOMMU domain is shared between all
		 * host1x clients, the IOVA is already available, so no
		 * need to map the buffer object again.
		 *
		 * XXX Note that this isn't always safe to do because it
		 * relies on an assumption that no cache maintenance is
		 * needed on the buffer objects.
		 */
		if (!domain || client->group)
			phys = &phys_addr;
		else
			phys = NULL;

		sgt = host1x_bo_pin(dev, reloc->target.bo, phys);
		if (IS_ERR(sgt)) {
			err = PTR_ERR(sgt);
			goto unpin;
		}

		if (sgt) {
			unsigned long mask = HOST1X_RELOC_READ |
					     HOST1X_RELOC_WRITE;
			enum dma_data_direction dir;

			switch (reloc->flags & mask) {
			case HOST1X_RELOC_READ:
				dir = DMA_TO_DEVICE;
				break;

			case HOST1X_RELOC_WRITE:
				dir = DMA_FROM_DEVICE;
				break;

			case HOST1X_RELOC_READ | HOST1X_RELOC_WRITE:
				dir = DMA_BIDIRECTIONAL;
				break;

			default:
				err = -EINVAL;
				goto unpin;
			}

			err = dma_map_sgtable(dev, sgt, dir, 0);
			if (err)
				goto unpin;

			job->unpins[job->num_unpins].dev = dev;
			job->unpins[job->num_unpins].dir = dir;
			phys_addr = sg_dma_address(sgt->sgl);
		}
>>>>>>> upstream/android-13

		job->addr_phys[job->num_unpins] = phys_addr;
		job->unpins[job->num_unpins].bo = reloc->target.bo;
		job->unpins[job->num_unpins].sgt = sgt;
		job->num_unpins++;
	}

<<<<<<< HEAD
	for (i = 0; i < job->num_gathers; i++) {
		struct host1x_job_gather *g = &job->gathers[i];
		size_t gather_size = 0;
		struct scatterlist *sg;
		struct sg_table *sgt;
		dma_addr_t phys_addr;
		unsigned long shift;
		struct iova *alloc;
		unsigned int j;

=======
	/*
	 * We will copy gathers BO content later, so there is no need to
	 * hold and pin them.
	 */
	if (job->enable_firewall)
		return 0;

	for (i = 0; i < job->num_cmds; i++) {
		size_t gather_size = 0;
		struct scatterlist *sg;
		dma_addr_t phys_addr;
		unsigned long shift;
		struct iova *alloc;
		dma_addr_t *phys;
		unsigned int j;

		if (job->cmds[i].is_wait)
			continue;

		g = &job->cmds[i].gather;

>>>>>>> upstream/android-13
		g->bo = host1x_bo_get(g->bo);
		if (!g->bo) {
			err = -EINVAL;
			goto unpin;
		}

<<<<<<< HEAD
		phys_addr = host1x_bo_pin(g->bo, &sgt);

		if (!IS_ENABLED(CONFIG_TEGRA_HOST1X_FIREWALL) && host->domain) {
			for_each_sg(sgt->sgl, sg, sgt->nents, j)
=======
		/**
		 * If the host1x is not attached to an IOMMU, there is no need
		 * to map the buffer object for the host1x, since the physical
		 * address can simply be used.
		 */
		if (!iommu_get_domain_for_dev(host->dev))
			phys = &phys_addr;
		else
			phys = NULL;

		sgt = host1x_bo_pin(host->dev, g->bo, phys);
		if (IS_ERR(sgt)) {
			err = PTR_ERR(sgt);
			goto put;
		}

		if (host->domain) {
			for_each_sgtable_sg(sgt, sg, j)
>>>>>>> upstream/android-13
				gather_size += sg->length;
			gather_size = iova_align(&host->iova, gather_size);

			shift = iova_shift(&host->iova);
			alloc = alloc_iova(&host->iova, gather_size >> shift,
					   host->iova_end >> shift, true);
			if (!alloc) {
				err = -ENOMEM;
<<<<<<< HEAD
				goto unpin;
			}

			err = iommu_map_sg(host->domain,
					iova_dma_addr(&host->iova, alloc),
					sgt->sgl, sgt->nents, IOMMU_READ);
			if (err == 0) {
				__free_iova(&host->iova, alloc);
				err = -EINVAL;
				goto unpin;
			}

			job->addr_phys[job->num_unpins] =
				iova_dma_addr(&host->iova, alloc);
			job->unpins[job->num_unpins].size = gather_size;
		} else {
			job->addr_phys[job->num_unpins] = phys_addr;
		}

		job->gather_addr_phys[i] = job->addr_phys[job->num_unpins];
=======
				goto put;
			}

			err = iommu_map_sgtable(host->domain,
					iova_dma_addr(&host->iova, alloc),
					sgt, IOMMU_READ);
			if (err == 0) {
				__free_iova(&host->iova, alloc);
				err = -EINVAL;
				goto put;
			}

			job->unpins[job->num_unpins].size = gather_size;
			phys_addr = iova_dma_addr(&host->iova, alloc);
		} else if (sgt) {
			err = dma_map_sgtable(host->dev, sgt, DMA_TO_DEVICE, 0);
			if (err)
				goto put;

			job->unpins[job->num_unpins].dir = DMA_TO_DEVICE;
			job->unpins[job->num_unpins].dev = host->dev;
			phys_addr = sg_dma_address(sgt->sgl);
		}

		job->addr_phys[job->num_unpins] = phys_addr;
		job->gather_addr_phys[i] = phys_addr;
>>>>>>> upstream/android-13

		job->unpins[job->num_unpins].bo = g->bo;
		job->unpins[job->num_unpins].sgt = sgt;
		job->num_unpins++;
	}

	return 0;

<<<<<<< HEAD
=======
put:
	host1x_bo_put(g->bo);
>>>>>>> upstream/android-13
unpin:
	host1x_job_unpin(job);
	return err;
}

static int do_relocs(struct host1x_job *job, struct host1x_job_gather *g)
{
<<<<<<< HEAD
	u32 last_page = ~0;
	void *cmdbuf_page_addr = NULL;
=======
	void *cmdbuf_addr = NULL;
>>>>>>> upstream/android-13
	struct host1x_bo *cmdbuf = g->bo;
	unsigned int i;

	/* pin & patch the relocs for one gather */
	for (i = 0; i < job->num_relocs; i++) {
		struct host1x_reloc *reloc = &job->relocs[i];
		u32 reloc_addr = (job->reloc_addr_phys[i] +
				  reloc->target.offset) >> reloc->shift;
		u32 *target;

		/* skip all other gathers */
		if (cmdbuf != reloc->cmdbuf.bo)
			continue;

<<<<<<< HEAD
		if (IS_ENABLED(CONFIG_TEGRA_HOST1X_FIREWALL)) {
=======
		if (job->enable_firewall) {
>>>>>>> upstream/android-13
			target = (u32 *)job->gather_copy_mapped +
					reloc->cmdbuf.offset / sizeof(u32) +
						g->offset / sizeof(u32);
			goto patch_reloc;
		}

<<<<<<< HEAD
		if (last_page != reloc->cmdbuf.offset >> PAGE_SHIFT) {
			if (cmdbuf_page_addr)
				host1x_bo_kunmap(cmdbuf, last_page,
						 cmdbuf_page_addr);

			cmdbuf_page_addr = host1x_bo_kmap(cmdbuf,
					reloc->cmdbuf.offset >> PAGE_SHIFT);
			last_page = reloc->cmdbuf.offset >> PAGE_SHIFT;

			if (unlikely(!cmdbuf_page_addr)) {
=======
		if (!cmdbuf_addr) {
			cmdbuf_addr = host1x_bo_mmap(cmdbuf);

			if (unlikely(!cmdbuf_addr)) {
>>>>>>> upstream/android-13
				pr_err("Could not map cmdbuf for relocation\n");
				return -ENOMEM;
			}
		}

<<<<<<< HEAD
		target = cmdbuf_page_addr + (reloc->cmdbuf.offset & ~PAGE_MASK);
=======
		target = cmdbuf_addr + reloc->cmdbuf.offset;
>>>>>>> upstream/android-13
patch_reloc:
		*target = reloc_addr;
	}

<<<<<<< HEAD
	if (cmdbuf_page_addr)
		host1x_bo_kunmap(cmdbuf, last_page, cmdbuf_page_addr);
=======
	if (cmdbuf_addr)
		host1x_bo_munmap(cmdbuf, cmdbuf_addr);
>>>>>>> upstream/android-13

	return 0;
}

static bool check_reloc(struct host1x_reloc *reloc, struct host1x_bo *cmdbuf,
			unsigned int offset)
{
	offset *= sizeof(u32);

	if (reloc->cmdbuf.bo != cmdbuf || reloc->cmdbuf.offset != offset)
		return false;

	/* relocation shift value validation isn't implemented yet */
	if (reloc->shift)
		return false;

	return true;
}

struct host1x_firewall {
	struct host1x_job *job;
	struct device *dev;

	unsigned int num_relocs;
	struct host1x_reloc *reloc;

	struct host1x_bo *cmdbuf;
	unsigned int offset;

	u32 words;
	u32 class;
	u32 reg;
	u32 mask;
	u32 count;
};

static int check_register(struct host1x_firewall *fw, unsigned long offset)
{
	if (!fw->job->is_addr_reg)
		return 0;

	if (fw->job->is_addr_reg(fw->dev, fw->class, offset)) {
		if (!fw->num_relocs)
			return -EINVAL;

		if (!check_reloc(fw->reloc, fw->cmdbuf, fw->offset))
			return -EINVAL;

		fw->num_relocs--;
		fw->reloc++;
	}

	return 0;
}

static int check_class(struct host1x_firewall *fw, u32 class)
{
	if (!fw->job->is_valid_class) {
		if (fw->class != class)
			return -EINVAL;
	} else {
		if (!fw->job->is_valid_class(fw->class))
			return -EINVAL;
	}

	return 0;
}

static int check_mask(struct host1x_firewall *fw)
{
	u32 mask = fw->mask;
	u32 reg = fw->reg;
	int ret;

	while (mask) {
		if (fw->words == 0)
			return -EINVAL;

		if (mask & 1) {
			ret = check_register(fw, reg);
			if (ret < 0)
				return ret;

			fw->words--;
			fw->offset++;
		}
		mask >>= 1;
		reg++;
	}

	return 0;
}

static int check_incr(struct host1x_firewall *fw)
{
	u32 count = fw->count;
	u32 reg = fw->reg;
	int ret;

	while (count) {
		if (fw->words == 0)
			return -EINVAL;

		ret = check_register(fw, reg);
		if (ret < 0)
			return ret;

		reg++;
		fw->words--;
		fw->offset++;
		count--;
	}

	return 0;
}

static int check_nonincr(struct host1x_firewall *fw)
{
	u32 count = fw->count;
	int ret;

	while (count) {
		if (fw->words == 0)
			return -EINVAL;

		ret = check_register(fw, fw->reg);
		if (ret < 0)
			return ret;

		fw->words--;
		fw->offset++;
		count--;
	}

	return 0;
}

static int validate(struct host1x_firewall *fw, struct host1x_job_gather *g)
{
	u32 *cmdbuf_base = (u32 *)fw->job->gather_copy_mapped +
		(g->offset / sizeof(u32));
	u32 job_class = fw->class;
	int err = 0;

	fw->words = g->words;
	fw->cmdbuf = g->bo;
	fw->offset = 0;

	while (fw->words && !err) {
		u32 word = cmdbuf_base[fw->offset];
		u32 opcode = (word & 0xf0000000) >> 28;

		fw->mask = 0;
		fw->reg = 0;
		fw->count = 0;
		fw->words--;
		fw->offset++;

		switch (opcode) {
		case 0:
			fw->class = word >> 6 & 0x3ff;
			fw->mask = word & 0x3f;
			fw->reg = word >> 16 & 0xfff;
			err = check_class(fw, job_class);
			if (!err)
				err = check_mask(fw);
			if (err)
				goto out;
			break;
		case 1:
			fw->reg = word >> 16 & 0xfff;
			fw->count = word & 0xffff;
			err = check_incr(fw);
			if (err)
				goto out;
			break;

		case 2:
			fw->reg = word >> 16 & 0xfff;
			fw->count = word & 0xffff;
			err = check_nonincr(fw);
			if (err)
				goto out;
			break;

		case 3:
			fw->mask = word & 0xffff;
			fw->reg = word >> 16 & 0xfff;
			err = check_mask(fw);
			if (err)
				goto out;
			break;
		case 4:
		case 14:
			break;
		default:
			err = -EINVAL;
			break;
		}
	}

out:
	return err;
}

static inline int copy_gathers(struct device *host, struct host1x_job *job,
			       struct device *dev)
{
	struct host1x_firewall fw;
	size_t size = 0;
	size_t offset = 0;
	unsigned int i;

	fw.job = job;
	fw.dev = dev;
	fw.reloc = job->relocs;
	fw.num_relocs = job->num_relocs;
	fw.class = job->class;

<<<<<<< HEAD
	for (i = 0; i < job->num_gathers; i++) {
		struct host1x_job_gather *g = &job->gathers[i];
=======
	for (i = 0; i < job->num_cmds; i++) {
		struct host1x_job_gather *g;

		if (job->cmds[i].is_wait)
			continue;

		g = &job->cmds[i].gather;
>>>>>>> upstream/android-13

		size += g->words * sizeof(u32);
	}

	/*
	 * Try a non-blocking allocation from a higher priority pools first,
	 * as awaiting for the allocation here is a major performance hit.
	 */
	job->gather_copy_mapped = dma_alloc_wc(host, size, &job->gather_copy,
					       GFP_NOWAIT);

	/* the higher priority allocation failed, try the generic-blocking */
	if (!job->gather_copy_mapped)
		job->gather_copy_mapped = dma_alloc_wc(host, size,
						       &job->gather_copy,
						       GFP_KERNEL);
	if (!job->gather_copy_mapped)
		return -ENOMEM;

	job->gather_copy_size = size;

<<<<<<< HEAD
	for (i = 0; i < job->num_gathers; i++) {
		struct host1x_job_gather *g = &job->gathers[i];
		void *gather;

=======
	for (i = 0; i < job->num_cmds; i++) {
		struct host1x_job_gather *g;
		void *gather;

		if (job->cmds[i].is_wait)
			continue;
		g = &job->cmds[i].gather;

>>>>>>> upstream/android-13
		/* Copy the gather */
		gather = host1x_bo_mmap(g->bo);
		memcpy(job->gather_copy_mapped + offset, gather + g->offset,
		       g->words * sizeof(u32));
		host1x_bo_munmap(g->bo, gather);

		/* Store the location in the buffer */
		g->base = job->gather_copy;
		g->offset = offset;

		/* Validate the job */
		if (validate(&fw, g))
			return -EINVAL;

		offset += g->words * sizeof(u32);
	}

	/* No relocs should remain at this point */
	if (fw.num_relocs)
		return -EINVAL;

	return 0;
}

int host1x_job_pin(struct host1x_job *job, struct device *dev)
{
	int err;
	unsigned int i, j;
	struct host1x *host = dev_get_drvdata(dev->parent);

	/* pin memory */
	err = pin_job(host, job);
	if (err)
		goto out;

<<<<<<< HEAD
	if (IS_ENABLED(CONFIG_TEGRA_HOST1X_FIREWALL)) {
=======
	if (job->enable_firewall) {
>>>>>>> upstream/android-13
		err = copy_gathers(host->dev, job, dev);
		if (err)
			goto out;
	}

	/* patch gathers */
<<<<<<< HEAD
	for (i = 0; i < job->num_gathers; i++) {
		struct host1x_job_gather *g = &job->gathers[i];
=======
	for (i = 0; i < job->num_cmds; i++) {
		struct host1x_job_gather *g;

		if (job->cmds[i].is_wait)
			continue;
		g = &job->cmds[i].gather;
>>>>>>> upstream/android-13

		/* process each gather mem only once */
		if (g->handled)
			continue;

		/* copy_gathers() sets gathers base if firewall is enabled */
<<<<<<< HEAD
		if (!IS_ENABLED(CONFIG_TEGRA_HOST1X_FIREWALL))
			g->base = job->gather_addr_phys[i];

		for (j = i + 1; j < job->num_gathers; j++) {
			if (job->gathers[j].bo == g->bo) {
				job->gathers[j].handled = true;
				job->gathers[j].base = g->base;
=======
		if (!job->enable_firewall)
			g->base = job->gather_addr_phys[i];

		for (j = i + 1; j < job->num_cmds; j++) {
			if (!job->cmds[j].is_wait &&
			    job->cmds[j].gather.bo == g->bo) {
				job->cmds[j].gather.handled = true;
				job->cmds[j].gather.base = g->base;
>>>>>>> upstream/android-13
			}
		}

		err = do_relocs(job, g);
		if (err)
			break;
	}

out:
	if (err)
		host1x_job_unpin(job);
	wmb();

	return err;
}
EXPORT_SYMBOL(host1x_job_pin);

void host1x_job_unpin(struct host1x_job *job)
{
	struct host1x *host = dev_get_drvdata(job->channel->dev->parent);
	unsigned int i;

	for (i = 0; i < job->num_unpins; i++) {
		struct host1x_job_unpin_data *unpin = &job->unpins[i];
<<<<<<< HEAD

		if (!IS_ENABLED(CONFIG_TEGRA_HOST1X_FIREWALL) &&
		    unpin->size && host->domain) {
=======
		struct device *dev = unpin->dev ?: host->dev;
		struct sg_table *sgt = unpin->sgt;

		if (!job->enable_firewall && unpin->size && host->domain) {
>>>>>>> upstream/android-13
			iommu_unmap(host->domain, job->addr_phys[i],
				    unpin->size);
			free_iova(&host->iova,
				iova_pfn(&host->iova, job->addr_phys[i]));
		}

<<<<<<< HEAD
		host1x_bo_unpin(unpin->bo, unpin->sgt);
=======
		if (unpin->dev && sgt)
			dma_unmap_sgtable(unpin->dev, sgt, unpin->dir, 0);

		host1x_bo_unpin(dev, unpin->bo, sgt);
>>>>>>> upstream/android-13
		host1x_bo_put(unpin->bo);
	}

	job->num_unpins = 0;

	if (job->gather_copy_size)
		dma_free_wc(host->dev, job->gather_copy_size,
			    job->gather_copy_mapped, job->gather_copy);
}
EXPORT_SYMBOL(host1x_job_unpin);

/*
 * Debug routine used to dump job entries
 */
void host1x_job_dump(struct device *dev, struct host1x_job *job)
{
<<<<<<< HEAD
	dev_dbg(dev, "    SYNCPT_ID   %d\n", job->syncpt_id);
=======
	dev_dbg(dev, "    SYNCPT_ID   %d\n", job->syncpt->id);
>>>>>>> upstream/android-13
	dev_dbg(dev, "    SYNCPT_VAL  %d\n", job->syncpt_end);
	dev_dbg(dev, "    FIRST_GET   0x%x\n", job->first_get);
	dev_dbg(dev, "    TIMEOUT     %d\n", job->timeout);
	dev_dbg(dev, "    NUM_SLOTS   %d\n", job->num_slots);
	dev_dbg(dev, "    NUM_HANDLES %d\n", job->num_unpins);
}
