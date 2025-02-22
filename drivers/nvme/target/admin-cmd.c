<<<<<<< HEAD
/*
 * NVMe admin command implementation.
 * Copyright (c) 2015-2016 HGST, a Western Digital Company.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * NVMe admin command implementation.
 * Copyright (c) 2015-2016 HGST, a Western Digital Company.
>>>>>>> upstream/android-13
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/rculist.h>
<<<<<<< HEAD
=======
#include <linux/part_stat.h>
>>>>>>> upstream/android-13

#include <generated/utsrelease.h>
#include <asm/unaligned.h>
#include "nvmet.h"

<<<<<<< HEAD
/*
 * This helper allows us to clear the AEN based on the RAE bit,
 * Please use this helper when processing the log pages which are
 * associated with the AEN.
 */
static inline void nvmet_clear_aen(struct nvmet_req *req, u32 aen_bit)
{
	int rae = le32_to_cpu(req->cmd->common.cdw10[0]) & 1 << 15;

	if (!rae)
		clear_bit(aen_bit, &req->sq->ctrl->aen_masked);
}

=======
>>>>>>> upstream/android-13
u32 nvmet_get_log_page_len(struct nvme_command *cmd)
{
	u32 len = le16_to_cpu(cmd->get_log_page.numdu);

	len <<= 16;
	len += le16_to_cpu(cmd->get_log_page.numdl);
	/* NUMD is a 0's based value */
	len += 1;
	len *= sizeof(u32);

	return len;
}

<<<<<<< HEAD
static void nvmet_execute_get_log_page_noop(struct nvmet_req *req)
{
	nvmet_req_complete(req, nvmet_zero_sgl(req, 0, req->data_len));
=======
static u32 nvmet_feat_data_len(struct nvmet_req *req, u32 cdw10)
{
	switch (cdw10 & 0xff) {
	case NVME_FEAT_HOST_ID:
		return sizeof(req->sq->ctrl->hostid);
	default:
		return 0;
	}
}

u64 nvmet_get_log_page_offset(struct nvme_command *cmd)
{
	return le64_to_cpu(cmd->get_log_page.lpo);
}

static void nvmet_execute_get_log_page_noop(struct nvmet_req *req)
{
	nvmet_req_complete(req, nvmet_zero_sgl(req, 0, req->transfer_len));
}

static void nvmet_execute_get_log_page_error(struct nvmet_req *req)
{
	struct nvmet_ctrl *ctrl = req->sq->ctrl;
	unsigned long flags;
	off_t offset = 0;
	u64 slot;
	u64 i;

	spin_lock_irqsave(&ctrl->error_lock, flags);
	slot = ctrl->err_counter % NVMET_ERROR_LOG_SLOTS;

	for (i = 0; i < NVMET_ERROR_LOG_SLOTS; i++) {
		if (nvmet_copy_to_sgl(req, offset, &ctrl->slots[slot],
				sizeof(struct nvme_error_slot)))
			break;

		if (slot == 0)
			slot = NVMET_ERROR_LOG_SLOTS - 1;
		else
			slot--;
		offset += sizeof(struct nvme_error_slot);
	}
	spin_unlock_irqrestore(&ctrl->error_lock, flags);
	nvmet_req_complete(req, 0);
>>>>>>> upstream/android-13
}

static u16 nvmet_get_smart_log_nsid(struct nvmet_req *req,
		struct nvme_smart_log *slog)
{
<<<<<<< HEAD
	struct nvmet_ns *ns;
	u64 host_reads, host_writes, data_units_read, data_units_written;

	ns = nvmet_find_namespace(req->sq->ctrl, req->cmd->get_log_page.nsid);
	if (!ns) {
		pr_err("nvmet : Could not find namespace id : %d\n",
				le32_to_cpu(req->cmd->get_log_page.nsid));
		return NVME_SC_INVALID_NS;
	}

	/* we don't have the right data for file backed ns */
	if (!ns->bdev)
		goto out;

	host_reads = part_stat_read(ns->bdev->bd_part, ios[READ]);
	data_units_read = DIV_ROUND_UP(part_stat_read(ns->bdev->bd_part,
		sectors[READ]), 1000);
	host_writes = part_stat_read(ns->bdev->bd_part, ios[WRITE]);
	data_units_written = DIV_ROUND_UP(part_stat_read(ns->bdev->bd_part,
		sectors[WRITE]), 1000);
=======
	u64 host_reads, host_writes, data_units_read, data_units_written;
	u16 status;

	status = nvmet_req_find_ns(req);
	if (status)
		return status;

	/* we don't have the right data for file backed ns */
	if (!req->ns->bdev)
		return NVME_SC_SUCCESS;

	host_reads = part_stat_read(req->ns->bdev, ios[READ]);
	data_units_read =
		DIV_ROUND_UP(part_stat_read(req->ns->bdev, sectors[READ]), 1000);
	host_writes = part_stat_read(req->ns->bdev, ios[WRITE]);
	data_units_written =
		DIV_ROUND_UP(part_stat_read(req->ns->bdev, sectors[WRITE]), 1000);
>>>>>>> upstream/android-13

	put_unaligned_le64(host_reads, &slog->host_reads[0]);
	put_unaligned_le64(data_units_read, &slog->data_units_read[0]);
	put_unaligned_le64(host_writes, &slog->host_writes[0]);
	put_unaligned_le64(data_units_written, &slog->data_units_written[0]);
<<<<<<< HEAD
out:
	nvmet_put_namespace(ns);
=======
>>>>>>> upstream/android-13

	return NVME_SC_SUCCESS;
}

static u16 nvmet_get_smart_log_all(struct nvmet_req *req,
		struct nvme_smart_log *slog)
{
	u64 host_reads = 0, host_writes = 0;
	u64 data_units_read = 0, data_units_written = 0;
	struct nvmet_ns *ns;
	struct nvmet_ctrl *ctrl;
<<<<<<< HEAD

	ctrl = req->sq->ctrl;

	rcu_read_lock();
	list_for_each_entry_rcu(ns, &ctrl->subsys->namespaces, dev_link) {
		/* we don't have the right data for file backed ns */
		if (!ns->bdev)
			continue;
		host_reads += part_stat_read(ns->bdev->bd_part, ios[READ]);
		data_units_read += DIV_ROUND_UP(
			part_stat_read(ns->bdev->bd_part, sectors[READ]), 1000);
		host_writes += part_stat_read(ns->bdev->bd_part, ios[WRITE]);
		data_units_written += DIV_ROUND_UP(
			part_stat_read(ns->bdev->bd_part, sectors[WRITE]), 1000);

	}
	rcu_read_unlock();
=======
	unsigned long idx;

	ctrl = req->sq->ctrl;
	xa_for_each(&ctrl->subsys->namespaces, idx, ns) {
		/* we don't have the right data for file backed ns */
		if (!ns->bdev)
			continue;
		host_reads += part_stat_read(ns->bdev, ios[READ]);
		data_units_read += DIV_ROUND_UP(
			part_stat_read(ns->bdev, sectors[READ]), 1000);
		host_writes += part_stat_read(ns->bdev, ios[WRITE]);
		data_units_written += DIV_ROUND_UP(
			part_stat_read(ns->bdev, sectors[WRITE]), 1000);
	}
>>>>>>> upstream/android-13

	put_unaligned_le64(host_reads, &slog->host_reads[0]);
	put_unaligned_le64(data_units_read, &slog->data_units_read[0]);
	put_unaligned_le64(host_writes, &slog->host_writes[0]);
	put_unaligned_le64(data_units_written, &slog->data_units_written[0]);

	return NVME_SC_SUCCESS;
}

static void nvmet_execute_get_log_page_smart(struct nvmet_req *req)
{
	struct nvme_smart_log *log;
	u16 status = NVME_SC_INTERNAL;
<<<<<<< HEAD

	if (req->data_len != sizeof(*log))
=======
	unsigned long flags;

	if (req->transfer_len != sizeof(*log))
>>>>>>> upstream/android-13
		goto out;

	log = kzalloc(sizeof(*log), GFP_KERNEL);
	if (!log)
		goto out;

	if (req->cmd->get_log_page.nsid == cpu_to_le32(NVME_NSID_ALL))
		status = nvmet_get_smart_log_all(req, log);
	else
		status = nvmet_get_smart_log_nsid(req, log);
	if (status)
		goto out_free_log;

<<<<<<< HEAD
=======
	spin_lock_irqsave(&req->sq->ctrl->error_lock, flags);
	put_unaligned_le64(req->sq->ctrl->err_counter,
			&log->num_err_log_entries);
	spin_unlock_irqrestore(&req->sq->ctrl->error_lock, flags);

>>>>>>> upstream/android-13
	status = nvmet_copy_to_sgl(req, 0, log, sizeof(*log));
out_free_log:
	kfree(log);
out:
	nvmet_req_complete(req, status);
}

<<<<<<< HEAD
static void nvmet_execute_get_log_cmd_effects_ns(struct nvmet_req *req)
{
	u16 status = NVME_SC_INTERNAL;
	struct nvme_effects_log *log;

	log = kzalloc(sizeof(*log), GFP_KERNEL);
	if (!log)
		goto out;

=======
static void nvmet_get_cmd_effects_nvm(struct nvme_effects_log *log)
{
>>>>>>> upstream/android-13
	log->acs[nvme_admin_get_log_page]	= cpu_to_le32(1 << 0);
	log->acs[nvme_admin_identify]		= cpu_to_le32(1 << 0);
	log->acs[nvme_admin_abort_cmd]		= cpu_to_le32(1 << 0);
	log->acs[nvme_admin_set_features]	= cpu_to_le32(1 << 0);
	log->acs[nvme_admin_get_features]	= cpu_to_le32(1 << 0);
	log->acs[nvme_admin_async_event]	= cpu_to_le32(1 << 0);
	log->acs[nvme_admin_keep_alive]		= cpu_to_le32(1 << 0);

	log->iocs[nvme_cmd_read]		= cpu_to_le32(1 << 0);
	log->iocs[nvme_cmd_write]		= cpu_to_le32(1 << 0);
	log->iocs[nvme_cmd_flush]		= cpu_to_le32(1 << 0);
	log->iocs[nvme_cmd_dsm]			= cpu_to_le32(1 << 0);
	log->iocs[nvme_cmd_write_zeroes]	= cpu_to_le32(1 << 0);
<<<<<<< HEAD

	status = nvmet_copy_to_sgl(req, 0, log, sizeof(*log));

=======
}

static void nvmet_get_cmd_effects_zns(struct nvme_effects_log *log)
{
	log->iocs[nvme_cmd_zone_append]		= cpu_to_le32(1 << 0);
	log->iocs[nvme_cmd_zone_mgmt_send]	= cpu_to_le32(1 << 0);
	log->iocs[nvme_cmd_zone_mgmt_recv]	= cpu_to_le32(1 << 0);
}

static void nvmet_execute_get_log_cmd_effects_ns(struct nvmet_req *req)
{
	struct nvme_effects_log *log;
	u16 status = NVME_SC_SUCCESS;

	log = kzalloc(sizeof(*log), GFP_KERNEL);
	if (!log) {
		status = NVME_SC_INTERNAL;
		goto out;
	}

	switch (req->cmd->get_log_page.csi) {
	case NVME_CSI_NVM:
		nvmet_get_cmd_effects_nvm(log);
		break;
	case NVME_CSI_ZNS:
		if (!IS_ENABLED(CONFIG_BLK_DEV_ZONED)) {
			status = NVME_SC_INVALID_IO_CMD_SET;
			goto free;
		}
		nvmet_get_cmd_effects_nvm(log);
		nvmet_get_cmd_effects_zns(log);
		break;
	default:
		status = NVME_SC_INVALID_LOG_PAGE;
		goto free;
	}

	status = nvmet_copy_to_sgl(req, 0, log, sizeof(*log));
free:
>>>>>>> upstream/android-13
	kfree(log);
out:
	nvmet_req_complete(req, status);
}

static void nvmet_execute_get_log_changed_ns(struct nvmet_req *req)
{
	struct nvmet_ctrl *ctrl = req->sq->ctrl;
	u16 status = NVME_SC_INTERNAL;
	size_t len;

<<<<<<< HEAD
	if (req->data_len != NVME_MAX_CHANGED_NAMESPACES * sizeof(__le32))
=======
	if (req->transfer_len != NVME_MAX_CHANGED_NAMESPACES * sizeof(__le32))
>>>>>>> upstream/android-13
		goto out;

	mutex_lock(&ctrl->lock);
	if (ctrl->nr_changed_ns == U32_MAX)
		len = sizeof(__le32);
	else
		len = ctrl->nr_changed_ns * sizeof(__le32);
	status = nvmet_copy_to_sgl(req, 0, ctrl->changed_ns_list, len);
	if (!status)
<<<<<<< HEAD
		status = nvmet_zero_sgl(req, len, req->data_len - len);
	ctrl->nr_changed_ns = 0;
	nvmet_clear_aen(req, NVME_AEN_CFG_NS_ATTR);
=======
		status = nvmet_zero_sgl(req, len, req->transfer_len - len);
	ctrl->nr_changed_ns = 0;
	nvmet_clear_aen_bit(req, NVME_AEN_BIT_NS_ATTR);
>>>>>>> upstream/android-13
	mutex_unlock(&ctrl->lock);
out:
	nvmet_req_complete(req, status);
}

static u32 nvmet_format_ana_group(struct nvmet_req *req, u32 grpid,
		struct nvme_ana_group_desc *desc)
{
	struct nvmet_ctrl *ctrl = req->sq->ctrl;
	struct nvmet_ns *ns;
<<<<<<< HEAD
	u32 count = 0;

	if (!(req->cmd->get_log_page.lsp & NVME_ANA_LOG_RGO)) {
		rcu_read_lock();
		list_for_each_entry_rcu(ns, &ctrl->subsys->namespaces, dev_link)
			if (ns->anagrpid == grpid)
				desc->nsids[count++] = cpu_to_le32(ns->nsid);
		rcu_read_unlock();
=======
	unsigned long idx;
	u32 count = 0;

	if (!(req->cmd->get_log_page.lsp & NVME_ANA_LOG_RGO)) {
		xa_for_each(&ctrl->subsys->namespaces, idx, ns)
			if (ns->anagrpid == grpid)
				desc->nsids[count++] = cpu_to_le32(ns->nsid);
>>>>>>> upstream/android-13
	}

	desc->grpid = cpu_to_le32(grpid);
	desc->nnsids = cpu_to_le32(count);
	desc->chgcnt = cpu_to_le64(nvmet_ana_chgcnt);
	desc->state = req->port->ana_state[grpid];
	memset(desc->rsvd17, 0, sizeof(desc->rsvd17));
	return sizeof(struct nvme_ana_group_desc) + count * sizeof(__le32);
}

static void nvmet_execute_get_log_page_ana(struct nvmet_req *req)
{
	struct nvme_ana_rsp_hdr hdr = { 0, };
	struct nvme_ana_group_desc *desc;
	size_t offset = sizeof(struct nvme_ana_rsp_hdr); /* start beyond hdr */
	size_t len;
	u32 grpid;
	u16 ngrps = 0;
	u16 status;

	status = NVME_SC_INTERNAL;
	desc = kmalloc(sizeof(struct nvme_ana_group_desc) +
			NVMET_MAX_NAMESPACES * sizeof(__le32), GFP_KERNEL);
	if (!desc)
		goto out;

	down_read(&nvmet_ana_sem);
	for (grpid = 1; grpid <= NVMET_MAX_ANAGRPS; grpid++) {
		if (!nvmet_ana_group_enabled[grpid])
			continue;
		len = nvmet_format_ana_group(req, grpid, desc);
		status = nvmet_copy_to_sgl(req, offset, desc, len);
		if (status)
			break;
		offset += len;
		ngrps++;
	}
	for ( ; grpid <= NVMET_MAX_ANAGRPS; grpid++) {
		if (nvmet_ana_group_enabled[grpid])
			ngrps++;
	}

	hdr.chgcnt = cpu_to_le64(nvmet_ana_chgcnt);
	hdr.ngrps = cpu_to_le16(ngrps);
<<<<<<< HEAD
	nvmet_clear_aen(req, NVME_AEN_CFG_ANA_CHANGE);
=======
	nvmet_clear_aen_bit(req, NVME_AEN_BIT_ANA_CHANGE);
>>>>>>> upstream/android-13
	up_read(&nvmet_ana_sem);

	kfree(desc);

	/* copy the header last once we know the number of groups */
	status = nvmet_copy_to_sgl(req, 0, &hdr, sizeof(hdr));
out:
	nvmet_req_complete(req, status);
}

<<<<<<< HEAD
static void nvmet_execute_identify_ctrl(struct nvmet_req *req)
{
	struct nvmet_ctrl *ctrl = req->sq->ctrl;
	struct nvme_id_ctrl *id;
	u16 status = 0;
	const char model[] = "Linux";
=======
static void nvmet_execute_get_log_page(struct nvmet_req *req)
{
	if (!nvmet_check_transfer_len(req, nvmet_get_log_page_len(req->cmd)))
		return;

	switch (req->cmd->get_log_page.lid) {
	case NVME_LOG_ERROR:
		return nvmet_execute_get_log_page_error(req);
	case NVME_LOG_SMART:
		return nvmet_execute_get_log_page_smart(req);
	case NVME_LOG_FW_SLOT:
		/*
		 * We only support a single firmware slot which always is
		 * active, so we can zero out the whole firmware slot log and
		 * still claim to fully implement this mandatory log page.
		 */
		return nvmet_execute_get_log_page_noop(req);
	case NVME_LOG_CHANGED_NS:
		return nvmet_execute_get_log_changed_ns(req);
	case NVME_LOG_CMD_EFFECTS:
		return nvmet_execute_get_log_cmd_effects_ns(req);
	case NVME_LOG_ANA:
		return nvmet_execute_get_log_page_ana(req);
	}
	pr_debug("unhandled lid %d on qid %d\n",
	       req->cmd->get_log_page.lid, req->sq->qid);
	req->error_loc = offsetof(struct nvme_get_log_page_command, lid);
	nvmet_req_complete(req, NVME_SC_INVALID_FIELD | NVME_SC_DNR);
}

static void nvmet_execute_identify_ctrl(struct nvmet_req *req)
{
	struct nvmet_ctrl *ctrl = req->sq->ctrl;
	struct nvmet_subsys *subsys = ctrl->subsys;
	struct nvme_id_ctrl *id;
	u32 cmd_capsule_size;
	u16 status = 0;

	if (!subsys->subsys_discovered) {
		mutex_lock(&subsys->lock);
		subsys->subsys_discovered = true;
		mutex_unlock(&subsys->lock);
	}
>>>>>>> upstream/android-13

	id = kzalloc(sizeof(*id), GFP_KERNEL);
	if (!id) {
		status = NVME_SC_INTERNAL;
		goto out;
	}

	/* XXX: figure out how to assign real vendors IDs. */
	id->vid = 0;
	id->ssvid = 0;

<<<<<<< HEAD
	memset(id->sn, ' ', sizeof(id->sn));
	bin2hex(id->sn, &ctrl->subsys->serial,
		min(sizeof(ctrl->subsys->serial), sizeof(id->sn) / 2));
	memcpy_and_pad(id->mn, sizeof(id->mn), model, sizeof(model) - 1, ' ');
=======
	memcpy(id->sn, ctrl->subsys->serial, NVMET_SN_MAX_SIZE);
	memcpy_and_pad(id->mn, sizeof(id->mn), subsys->model_number,
		       strlen(subsys->model_number), ' ');
>>>>>>> upstream/android-13
	memcpy_and_pad(id->fr, sizeof(id->fr),
		       UTS_RELEASE, strlen(UTS_RELEASE), ' ');

	id->rab = 6;

	/*
	 * XXX: figure out how we can assign a IEEE OUI, but until then
	 * the safest is to leave it as zeroes.
	 */

	/* we support multiple ports, multiples hosts and ANA: */
	id->cmic = (1 << 0) | (1 << 1) | (1 << 3);

<<<<<<< HEAD
	/* no limit on data transfer sizes for now */
	id->mdts = 0;
=======
	/* Limit MDTS according to transport capability */
	if (ctrl->ops->get_mdts)
		id->mdts = ctrl->ops->get_mdts(ctrl);
	else
		id->mdts = 0;

>>>>>>> upstream/android-13
	id->cntlid = cpu_to_le16(ctrl->cntlid);
	id->ver = cpu_to_le32(ctrl->subsys->ver);

	/* XXX: figure out what to do about RTD3R/RTD3 */
	id->oaes = cpu_to_le32(NVMET_AEN_CFG_OPTIONAL);
<<<<<<< HEAD
	id->ctratt = cpu_to_le32(1 << 0);
=======
	id->ctratt = cpu_to_le32(NVME_CTRL_ATTR_HID_128_BIT |
		NVME_CTRL_ATTR_TBKAS);
>>>>>>> upstream/android-13

	id->oacs = 0;

	/*
	 * We don't really have a practical limit on the number of abort
	 * comands.  But we don't do anything useful for abort either, so
	 * no point in allowing more abort commands than the spec requires.
	 */
	id->acl = 3;

	id->aerl = NVMET_ASYNC_EVENTS - 1;

	/* first slot is read-only, only one slot supported */
	id->frmw = (1 << 0) | (1 << 1);
	id->lpa = (1 << 0) | (1 << 1) | (1 << 2);
	id->elpe = NVMET_ERROR_LOG_SLOTS - 1;
	id->npss = 0;

	/* We support keep-alive timeout in granularity of seconds */
	id->kas = cpu_to_le16(NVMET_KAS);

	id->sqes = (0x6 << 4) | 0x6;
	id->cqes = (0x4 << 4) | 0x4;

	/* no enforcement soft-limit for maxcmd - pick arbitrary high value */
	id->maxcmd = cpu_to_le16(NVMET_MAX_CMD);

<<<<<<< HEAD
	id->nn = cpu_to_le32(ctrl->subsys->max_nsid);
=======
	id->nn = cpu_to_le32(NVMET_MAX_NAMESPACES);
>>>>>>> upstream/android-13
	id->mnan = cpu_to_le32(NVMET_MAX_NAMESPACES);
	id->oncs = cpu_to_le16(NVME_CTRL_ONCS_DSM |
			NVME_CTRL_ONCS_WRITE_ZEROES);

	/* XXX: don't report vwc if the underlying device is write through */
	id->vwc = NVME_CTRL_VWC_PRESENT;

	/*
	 * We can't support atomic writes bigger than a LBA without support
	 * from the backend device.
	 */
	id->awun = 0;
	id->awupf = 0;

	id->sgls = cpu_to_le32(1 << 0);	/* we always support SGLs */
<<<<<<< HEAD
	if (ctrl->ops->has_keyed_sgls)
=======
	if (ctrl->ops->flags & NVMF_KEYED_SGLS)
>>>>>>> upstream/android-13
		id->sgls |= cpu_to_le32(1 << 2);
	if (req->port->inline_data_size)
		id->sgls |= cpu_to_le32(1 << 20);

<<<<<<< HEAD
	strcpy(id->subnqn, ctrl->subsys->subsysnqn);

	/* Max command capsule size is sqe + single page of in-capsule data */
	id->ioccsz = cpu_to_le32((sizeof(struct nvme_command) +
				  req->port->inline_data_size) / 16);
=======
	strlcpy(id->subnqn, ctrl->subsys->subsysnqn, sizeof(id->subnqn));

	/*
	 * Max command capsule size is sqe + in-capsule data size.
	 * Disable in-capsule data for Metadata capable controllers.
	 */
	cmd_capsule_size = sizeof(struct nvme_command);
	if (!ctrl->pi_support)
		cmd_capsule_size += req->port->inline_data_size;
	id->ioccsz = cpu_to_le32(cmd_capsule_size / 16);

>>>>>>> upstream/android-13
	/* Max response capsule size is cqe */
	id->iorcsz = cpu_to_le32(sizeof(struct nvme_completion) / 16);

	id->msdbd = ctrl->ops->msdbd;

	id->anacap = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);
	id->anatt = 10; /* random value */
	id->anagrpmax = cpu_to_le32(NVMET_MAX_ANAGRPS);
	id->nanagrpid = cpu_to_le32(NVMET_MAX_ANAGRPS);

	/*
	 * Meh, we don't really support any power state.  Fake up the same
	 * values that qemu does.
	 */
	id->psd[0].max_power = cpu_to_le16(0x9c4);
	id->psd[0].entry_lat = cpu_to_le32(0x10);
	id->psd[0].exit_lat = cpu_to_le32(0x4);

	id->nwpc = 1 << 0; /* write protect and no write protect */

	status = nvmet_copy_to_sgl(req, 0, id, sizeof(*id));

	kfree(id);
out:
	nvmet_req_complete(req, status);
}

static void nvmet_execute_identify_ns(struct nvmet_req *req)
{
<<<<<<< HEAD
	struct nvmet_ns *ns;
	struct nvme_id_ns *id;
	u16 status = 0;

	if (le32_to_cpu(req->cmd->identify.nsid) == NVME_NSID_ALL) {
=======
	struct nvme_id_ns *id;
	u16 status;

	if (le32_to_cpu(req->cmd->identify.nsid) == NVME_NSID_ALL) {
		req->error_loc = offsetof(struct nvme_identify, nsid);
>>>>>>> upstream/android-13
		status = NVME_SC_INVALID_NS | NVME_SC_DNR;
		goto out;
	}

	id = kzalloc(sizeof(*id), GFP_KERNEL);
	if (!id) {
		status = NVME_SC_INTERNAL;
		goto out;
	}

	/* return an all zeroed buffer if we can't find an active namespace */
<<<<<<< HEAD
	ns = nvmet_find_namespace(req->sq->ctrl, req->cmd->identify.nsid);
	if (!ns)
		goto done;
=======
	status = nvmet_req_find_ns(req);
	if (status) {
		status = 0;
		goto done;
	}

	nvmet_ns_revalidate(req->ns);
>>>>>>> upstream/android-13

	/*
	 * nuse = ncap = nsze isn't always true, but we have no way to find
	 * that out from the underlying device.
	 */
<<<<<<< HEAD
	id->ncap = id->nsze = cpu_to_le64(ns->size >> ns->blksize_shift);
	switch (req->port->ana_state[ns->anagrpid]) {
=======
	id->ncap = id->nsze =
		cpu_to_le64(req->ns->size >> req->ns->blksize_shift);
	switch (req->port->ana_state[req->ns->anagrpid]) {
>>>>>>> upstream/android-13
	case NVME_ANA_INACCESSIBLE:
	case NVME_ANA_PERSISTENT_LOSS:
		break;
	default:
		id->nuse = id->nsze;
		break;
<<<<<<< HEAD
        }
=======
	}

	if (req->ns->bdev)
		nvmet_bdev_set_limits(req->ns->bdev, id);
>>>>>>> upstream/android-13

	/*
	 * We just provide a single LBA format that matches what the
	 * underlying device reports.
	 */
	id->nlbaf = 0;
	id->flbas = 0;

	/*
	 * Our namespace might always be shared.  Not just with other
	 * controllers, but also with any other user of the block device.
	 */
	id->nmic = (1 << 0);
<<<<<<< HEAD
	id->anagrpid = cpu_to_le32(ns->anagrpid);

	memcpy(&id->nguid, &ns->nguid, sizeof(id->nguid));

	id->lbaf[0].ds = ns->blksize_shift;

	if (ns->readonly)
		id->nsattr |= (1 << 0);
	nvmet_put_namespace(ns);
done:
	status = nvmet_copy_to_sgl(req, 0, id, sizeof(*id));
=======
	id->anagrpid = cpu_to_le32(req->ns->anagrpid);

	memcpy(&id->nguid, &req->ns->nguid, sizeof(id->nguid));

	id->lbaf[0].ds = req->ns->blksize_shift;

	if (req->sq->ctrl->pi_support && nvmet_ns_has_pi(req->ns)) {
		id->dpc = NVME_NS_DPC_PI_FIRST | NVME_NS_DPC_PI_LAST |
			  NVME_NS_DPC_PI_TYPE1 | NVME_NS_DPC_PI_TYPE2 |
			  NVME_NS_DPC_PI_TYPE3;
		id->mc = NVME_MC_EXTENDED_LBA;
		id->dps = req->ns->pi_type;
		id->flbas = NVME_NS_FLBAS_META_EXT;
		id->lbaf[0].ms = cpu_to_le16(req->ns->metadata_size);
	}

	if (req->ns->readonly)
		id->nsattr |= (1 << 0);
done:
	if (!status)
		status = nvmet_copy_to_sgl(req, 0, id, sizeof(*id));

>>>>>>> upstream/android-13
	kfree(id);
out:
	nvmet_req_complete(req, status);
}

static void nvmet_execute_identify_nslist(struct nvmet_req *req)
{
	static const int buf_size = NVME_IDENTIFY_DATA_SIZE;
	struct nvmet_ctrl *ctrl = req->sq->ctrl;
	struct nvmet_ns *ns;
<<<<<<< HEAD
=======
	unsigned long idx;
>>>>>>> upstream/android-13
	u32 min_nsid = le32_to_cpu(req->cmd->identify.nsid);
	__le32 *list;
	u16 status = 0;
	int i = 0;

	list = kzalloc(buf_size, GFP_KERNEL);
	if (!list) {
		status = NVME_SC_INTERNAL;
		goto out;
	}

<<<<<<< HEAD
	rcu_read_lock();
	list_for_each_entry_rcu(ns, &ctrl->subsys->namespaces, dev_link) {
=======
	xa_for_each(&ctrl->subsys->namespaces, idx, ns) {
>>>>>>> upstream/android-13
		if (ns->nsid <= min_nsid)
			continue;
		list[i++] = cpu_to_le32(ns->nsid);
		if (i == buf_size / sizeof(__le32))
			break;
	}
<<<<<<< HEAD
	rcu_read_unlock();
=======
>>>>>>> upstream/android-13

	status = nvmet_copy_to_sgl(req, 0, list, buf_size);

	kfree(list);
out:
	nvmet_req_complete(req, status);
}

static u16 nvmet_copy_ns_identifier(struct nvmet_req *req, u8 type, u8 len,
				    void *id, off_t *off)
{
	struct nvme_ns_id_desc desc = {
		.nidt = type,
		.nidl = len,
	};
	u16 status;

	status = nvmet_copy_to_sgl(req, *off, &desc, sizeof(desc));
	if (status)
		return status;
	*off += sizeof(desc);

	status = nvmet_copy_to_sgl(req, *off, id, len);
	if (status)
		return status;
	*off += len;

	return 0;
}

static void nvmet_execute_identify_desclist(struct nvmet_req *req)
{
<<<<<<< HEAD
	struct nvmet_ns *ns;
	u16 status = 0;
	off_t off = 0;

	ns = nvmet_find_namespace(req->sq->ctrl, req->cmd->identify.nsid);
	if (!ns) {
		status = NVME_SC_INVALID_NS | NVME_SC_DNR;
		goto out;
	}

	if (memchr_inv(&ns->uuid, 0, sizeof(ns->uuid))) {
		status = nvmet_copy_ns_identifier(req, NVME_NIDT_UUID,
						  NVME_NIDT_UUID_LEN,
						  &ns->uuid, &off);
		if (status)
			goto out_put_ns;
	}
	if (memchr_inv(ns->nguid, 0, sizeof(ns->nguid))) {
		status = nvmet_copy_ns_identifier(req, NVME_NIDT_NGUID,
						  NVME_NIDT_NGUID_LEN,
						  &ns->nguid, &off);
		if (status)
			goto out_put_ns;
	}

	if (sg_zero_buffer(req->sg, req->sg_cnt, NVME_IDENTIFY_DATA_SIZE - off,
			off) != NVME_IDENTIFY_DATA_SIZE - off)
		status = NVME_SC_INTERNAL | NVME_SC_DNR;
out_put_ns:
	nvmet_put_namespace(ns);
=======
	off_t off = 0;
	u16 status;

	status = nvmet_req_find_ns(req);
	if (status)
		goto out;

	if (memchr_inv(&req->ns->uuid, 0, sizeof(req->ns->uuid))) {
		status = nvmet_copy_ns_identifier(req, NVME_NIDT_UUID,
						  NVME_NIDT_UUID_LEN,
						  &req->ns->uuid, &off);
		if (status)
			goto out;
	}
	if (memchr_inv(req->ns->nguid, 0, sizeof(req->ns->nguid))) {
		status = nvmet_copy_ns_identifier(req, NVME_NIDT_NGUID,
						  NVME_NIDT_NGUID_LEN,
						  &req->ns->nguid, &off);
		if (status)
			goto out;
	}

	status = nvmet_copy_ns_identifier(req, NVME_NIDT_CSI,
					  NVME_NIDT_CSI_LEN,
					  &req->ns->csi, &off);
	if (status)
		goto out;

	if (sg_zero_buffer(req->sg, req->sg_cnt, NVME_IDENTIFY_DATA_SIZE - off,
			off) != NVME_IDENTIFY_DATA_SIZE - off)
		status = NVME_SC_INTERNAL | NVME_SC_DNR;

>>>>>>> upstream/android-13
out:
	nvmet_req_complete(req, status);
}

<<<<<<< HEAD
=======
static bool nvmet_handle_identify_desclist(struct nvmet_req *req)
{
	switch (req->cmd->identify.csi) {
	case NVME_CSI_NVM:
		nvmet_execute_identify_desclist(req);
		return true;
	case NVME_CSI_ZNS:
		if (IS_ENABLED(CONFIG_BLK_DEV_ZONED)) {
			nvmet_execute_identify_desclist(req);
			return true;
		}
		return false;
	default:
		return false;
	}
}

static void nvmet_execute_identify(struct nvmet_req *req)
{
	if (!nvmet_check_transfer_len(req, NVME_IDENTIFY_DATA_SIZE))
		return;

	switch (req->cmd->identify.cns) {
	case NVME_ID_CNS_NS:
		switch (req->cmd->identify.csi) {
		case NVME_CSI_NVM:
			return nvmet_execute_identify_ns(req);
		default:
			break;
		}
		break;
	case NVME_ID_CNS_CS_NS:
		if (IS_ENABLED(CONFIG_BLK_DEV_ZONED)) {
			switch (req->cmd->identify.csi) {
			case NVME_CSI_ZNS:
				return nvmet_execute_identify_cns_cs_ns(req);
			default:
				break;
			}
		}
		break;
	case NVME_ID_CNS_CTRL:
		switch (req->cmd->identify.csi) {
		case NVME_CSI_NVM:
			return nvmet_execute_identify_ctrl(req);
		}
		break;
	case NVME_ID_CNS_CS_CTRL:
		if (IS_ENABLED(CONFIG_BLK_DEV_ZONED)) {
			switch (req->cmd->identify.csi) {
			case NVME_CSI_ZNS:
				return nvmet_execute_identify_cns_cs_ctrl(req);
			default:
				break;
			}
		}
		break;
	case NVME_ID_CNS_NS_ACTIVE_LIST:
		switch (req->cmd->identify.csi) {
		case NVME_CSI_NVM:
			return nvmet_execute_identify_nslist(req);
		default:
			break;
		}
		break;
	case NVME_ID_CNS_NS_DESC_LIST:
		if (nvmet_handle_identify_desclist(req) == true)
			return;
		break;
	}

	nvmet_req_cns_error_complete(req);
}

>>>>>>> upstream/android-13
/*
 * A "minimum viable" abort implementation: the command is mandatory in the
 * spec, but we are not required to do any useful work.  We couldn't really
 * do a useful abort, so don't bother even with waiting for the command
 * to be exectuted and return immediately telling the command to abort
 * wasn't found.
 */
static void nvmet_execute_abort(struct nvmet_req *req)
{
<<<<<<< HEAD
=======
	if (!nvmet_check_transfer_len(req, 0))
		return;
>>>>>>> upstream/android-13
	nvmet_set_result(req, 1);
	nvmet_req_complete(req, 0);
}

static u16 nvmet_write_protect_flush_sync(struct nvmet_req *req)
{
	u16 status;

	if (req->ns->file)
		status = nvmet_file_flush(req);
	else
		status = nvmet_bdev_flush(req);

	if (status)
		pr_err("write protect flush failed nsid: %u\n", req->ns->nsid);
	return status;
}

static u16 nvmet_set_feat_write_protect(struct nvmet_req *req)
{
<<<<<<< HEAD
	u32 write_protect = le32_to_cpu(req->cmd->common.cdw10[1]);
	struct nvmet_subsys *subsys = req->sq->ctrl->subsys;
	u16 status = NVME_SC_FEATURE_NOT_CHANGEABLE;

	req->ns = nvmet_find_namespace(req->sq->ctrl, req->cmd->rw.nsid);
	if (unlikely(!req->ns))
=======
	u32 write_protect = le32_to_cpu(req->cmd->common.cdw11);
	struct nvmet_subsys *subsys = nvmet_req_subsys(req);
	u16 status;

	status = nvmet_req_find_ns(req);
	if (status)
>>>>>>> upstream/android-13
		return status;

	mutex_lock(&subsys->lock);
	switch (write_protect) {
	case NVME_NS_WRITE_PROTECT:
		req->ns->readonly = true;
		status = nvmet_write_protect_flush_sync(req);
		if (status)
			req->ns->readonly = false;
		break;
	case NVME_NS_NO_WRITE_PROTECT:
		req->ns->readonly = false;
		status = 0;
		break;
	default:
		break;
	}

	if (!status)
		nvmet_ns_changed(subsys, req->ns->nsid);
	mutex_unlock(&subsys->lock);
	return status;
}

<<<<<<< HEAD
static void nvmet_execute_set_features(struct nvmet_req *req)
{
	struct nvmet_subsys *subsys = req->sq->ctrl->subsys;
	u32 cdw10 = le32_to_cpu(req->cmd->common.cdw10[0]);
	u32 val32;
	u16 status = 0;

	switch (cdw10 & 0xff) {
	case NVME_FEAT_NUM_QUEUES:
=======
u16 nvmet_set_feat_kato(struct nvmet_req *req)
{
	u32 val32 = le32_to_cpu(req->cmd->common.cdw11);

	nvmet_stop_keep_alive_timer(req->sq->ctrl);
	req->sq->ctrl->kato = DIV_ROUND_UP(val32, 1000);
	nvmet_start_keep_alive_timer(req->sq->ctrl);

	nvmet_set_result(req, req->sq->ctrl->kato);

	return 0;
}

u16 nvmet_set_feat_async_event(struct nvmet_req *req, u32 mask)
{
	u32 val32 = le32_to_cpu(req->cmd->common.cdw11);

	if (val32 & ~mask) {
		req->error_loc = offsetof(struct nvme_common_command, cdw11);
		return NVME_SC_INVALID_FIELD | NVME_SC_DNR;
	}

	WRITE_ONCE(req->sq->ctrl->aen_enabled, val32);
	nvmet_set_result(req, val32);

	return 0;
}

void nvmet_execute_set_features(struct nvmet_req *req)
{
	struct nvmet_subsys *subsys = nvmet_req_subsys(req);
	u32 cdw10 = le32_to_cpu(req->cmd->common.cdw10);
	u32 cdw11 = le32_to_cpu(req->cmd->common.cdw11);
	u16 status = 0;
	u16 nsqr;
	u16 ncqr;

	if (!nvmet_check_transfer_len(req, 0))
		return;

	switch (cdw10 & 0xff) {
	case NVME_FEAT_NUM_QUEUES:
		ncqr = (cdw11 >> 16) & 0xffff;
		nsqr = cdw11 & 0xffff;
		if (ncqr == 0xffff || nsqr == 0xffff) {
			status = NVME_SC_INVALID_FIELD | NVME_SC_DNR;
			break;
		}
>>>>>>> upstream/android-13
		nvmet_set_result(req,
			(subsys->max_qid - 1) | ((subsys->max_qid - 1) << 16));
		break;
	case NVME_FEAT_KATO:
<<<<<<< HEAD
		val32 = le32_to_cpu(req->cmd->common.cdw10[1]);
		req->sq->ctrl->kato = DIV_ROUND_UP(val32, 1000);
		nvmet_set_result(req, req->sq->ctrl->kato);
		break;
	case NVME_FEAT_ASYNC_EVENT:
		val32 = le32_to_cpu(req->cmd->common.cdw10[1]);
		if (val32 & ~NVMET_AEN_CFG_ALL) {
			status = NVME_SC_INVALID_FIELD | NVME_SC_DNR;
			break;
		}

		WRITE_ONCE(req->sq->ctrl->aen_enabled, val32);
		nvmet_set_result(req, val32);
=======
		status = nvmet_set_feat_kato(req);
		break;
	case NVME_FEAT_ASYNC_EVENT:
		status = nvmet_set_feat_async_event(req, NVMET_AEN_CFG_ALL);
>>>>>>> upstream/android-13
		break;
	case NVME_FEAT_HOST_ID:
		status = NVME_SC_CMD_SEQ_ERROR | NVME_SC_DNR;
		break;
	case NVME_FEAT_WRITE_PROTECT:
		status = nvmet_set_feat_write_protect(req);
		break;
	default:
<<<<<<< HEAD
=======
		req->error_loc = offsetof(struct nvme_common_command, cdw10);
>>>>>>> upstream/android-13
		status = NVME_SC_INVALID_FIELD | NVME_SC_DNR;
		break;
	}

	nvmet_req_complete(req, status);
}

static u16 nvmet_get_feat_write_protect(struct nvmet_req *req)
{
<<<<<<< HEAD
	struct nvmet_subsys *subsys = req->sq->ctrl->subsys;
	u32 result;

	req->ns = nvmet_find_namespace(req->sq->ctrl, req->cmd->common.nsid);
	if (!req->ns)
		return NVME_SC_INVALID_NS | NVME_SC_DNR;
=======
	struct nvmet_subsys *subsys = nvmet_req_subsys(req);
	u32 result;

	result = nvmet_req_find_ns(req);
	if (result)
		return result;
>>>>>>> upstream/android-13

	mutex_lock(&subsys->lock);
	if (req->ns->readonly == true)
		result = NVME_NS_WRITE_PROTECT;
	else
		result = NVME_NS_NO_WRITE_PROTECT;
	nvmet_set_result(req, result);
	mutex_unlock(&subsys->lock);

	return 0;
}

<<<<<<< HEAD
static void nvmet_execute_get_features(struct nvmet_req *req)
{
	struct nvmet_subsys *subsys = req->sq->ctrl->subsys;
	u32 cdw10 = le32_to_cpu(req->cmd->common.cdw10[0]);
	u16 status = 0;

=======
void nvmet_get_feat_kato(struct nvmet_req *req)
{
	nvmet_set_result(req, req->sq->ctrl->kato * 1000);
}

void nvmet_get_feat_async_event(struct nvmet_req *req)
{
	nvmet_set_result(req, READ_ONCE(req->sq->ctrl->aen_enabled));
}

void nvmet_execute_get_features(struct nvmet_req *req)
{
	struct nvmet_subsys *subsys = nvmet_req_subsys(req);
	u32 cdw10 = le32_to_cpu(req->cmd->common.cdw10);
	u16 status = 0;

	if (!nvmet_check_transfer_len(req, nvmet_feat_data_len(req, cdw10)))
		return;

>>>>>>> upstream/android-13
	switch (cdw10 & 0xff) {
	/*
	 * These features are mandatory in the spec, but we don't
	 * have a useful way to implement them.  We'll eventually
	 * need to come up with some fake values for these.
	 */
#if 0
	case NVME_FEAT_ARBITRATION:
		break;
	case NVME_FEAT_POWER_MGMT:
		break;
	case NVME_FEAT_TEMP_THRESH:
		break;
	case NVME_FEAT_ERR_RECOVERY:
		break;
	case NVME_FEAT_IRQ_COALESCE:
		break;
	case NVME_FEAT_IRQ_CONFIG:
		break;
	case NVME_FEAT_WRITE_ATOMIC:
		break;
#endif
	case NVME_FEAT_ASYNC_EVENT:
<<<<<<< HEAD
		nvmet_set_result(req, READ_ONCE(req->sq->ctrl->aen_enabled));
=======
		nvmet_get_feat_async_event(req);
>>>>>>> upstream/android-13
		break;
	case NVME_FEAT_VOLATILE_WC:
		nvmet_set_result(req, 1);
		break;
	case NVME_FEAT_NUM_QUEUES:
		nvmet_set_result(req,
			(subsys->max_qid-1) | ((subsys->max_qid-1) << 16));
		break;
	case NVME_FEAT_KATO:
<<<<<<< HEAD
		nvmet_set_result(req, req->sq->ctrl->kato * 1000);
		break;
	case NVME_FEAT_HOST_ID:
		/* need 128-bit host identifier flag */
		if (!(req->cmd->common.cdw10[1] & cpu_to_le32(1 << 0))) {
=======
		nvmet_get_feat_kato(req);
		break;
	case NVME_FEAT_HOST_ID:
		/* need 128-bit host identifier flag */
		if (!(req->cmd->common.cdw11 & cpu_to_le32(1 << 0))) {
			req->error_loc =
				offsetof(struct nvme_common_command, cdw11);
>>>>>>> upstream/android-13
			status = NVME_SC_INVALID_FIELD | NVME_SC_DNR;
			break;
		}

		status = nvmet_copy_to_sgl(req, 0, &req->sq->ctrl->hostid,
				sizeof(req->sq->ctrl->hostid));
		break;
	case NVME_FEAT_WRITE_PROTECT:
		status = nvmet_get_feat_write_protect(req);
		break;
	default:
<<<<<<< HEAD
=======
		req->error_loc =
			offsetof(struct nvme_common_command, cdw10);
>>>>>>> upstream/android-13
		status = NVME_SC_INVALID_FIELD | NVME_SC_DNR;
		break;
	}

	nvmet_req_complete(req, status);
}

<<<<<<< HEAD
static void nvmet_execute_async_event(struct nvmet_req *req)
{
	struct nvmet_ctrl *ctrl = req->sq->ctrl;

=======
void nvmet_execute_async_event(struct nvmet_req *req)
{
	struct nvmet_ctrl *ctrl = req->sq->ctrl;

	if (!nvmet_check_transfer_len(req, 0))
		return;

>>>>>>> upstream/android-13
	mutex_lock(&ctrl->lock);
	if (ctrl->nr_async_event_cmds >= NVMET_ASYNC_EVENTS) {
		mutex_unlock(&ctrl->lock);
		nvmet_req_complete(req, NVME_SC_ASYNC_LIMIT | NVME_SC_DNR);
		return;
	}
	ctrl->async_event_cmds[ctrl->nr_async_event_cmds++] = req;
	mutex_unlock(&ctrl->lock);

	schedule_work(&ctrl->async_event_work);
}

<<<<<<< HEAD
static void nvmet_execute_keep_alive(struct nvmet_req *req)
{
	struct nvmet_ctrl *ctrl = req->sq->ctrl;

	pr_debug("ctrl %d update keep-alive timer for %d secs\n",
		ctrl->cntlid, ctrl->kato);

	mod_delayed_work(system_wq, &ctrl->ka_work, ctrl->kato * HZ);
	nvmet_req_complete(req, 0);
=======
void nvmet_execute_keep_alive(struct nvmet_req *req)
{
	struct nvmet_ctrl *ctrl = req->sq->ctrl;
	u16 status = 0;

	if (!nvmet_check_transfer_len(req, 0))
		return;

	if (!ctrl->kato) {
		status = NVME_SC_KA_TIMEOUT_INVALID;
		goto out;
	}

	pr_debug("ctrl %d update keep-alive timer for %d secs\n",
		ctrl->cntlid, ctrl->kato);
	mod_delayed_work(system_wq, &ctrl->ka_work, ctrl->kato * HZ);
out:
	nvmet_req_complete(req, status);
>>>>>>> upstream/android-13
}

u16 nvmet_parse_admin_cmd(struct nvmet_req *req)
{
	struct nvme_command *cmd = req->cmd;
	u16 ret;

<<<<<<< HEAD
	ret = nvmet_check_ctrl_status(req, cmd);
	if (unlikely(ret))
		return ret;

	switch (cmd->common.opcode) {
	case nvme_admin_get_log_page:
		req->data_len = nvmet_get_log_page_len(cmd);

		switch (cmd->get_log_page.lid) {
		case NVME_LOG_ERROR:
			/*
			 * We currently never set the More bit in the status
			 * field, so all error log entries are invalid and can
			 * be zeroed out.  This is called a minum viable
			 * implementation (TM) of this mandatory log page.
			 */
			req->execute = nvmet_execute_get_log_page_noop;
			return 0;
		case NVME_LOG_SMART:
			req->execute = nvmet_execute_get_log_page_smart;
			return 0;
		case NVME_LOG_FW_SLOT:
			/*
			 * We only support a single firmware slot which always
			 * is active, so we can zero out the whole firmware slot
			 * log and still claim to fully implement this mandatory
			 * log page.
			 */
			req->execute = nvmet_execute_get_log_page_noop;
			return 0;
		case NVME_LOG_CHANGED_NS:
			req->execute = nvmet_execute_get_log_changed_ns;
			return 0;
		case NVME_LOG_CMD_EFFECTS:
			req->execute = nvmet_execute_get_log_cmd_effects_ns;
			return 0;
		case NVME_LOG_ANA:
			req->execute = nvmet_execute_get_log_page_ana;
			return 0;
		}
		break;
	case nvme_admin_identify:
		req->data_len = NVME_IDENTIFY_DATA_SIZE;
		switch (cmd->identify.cns) {
		case NVME_ID_CNS_NS:
			req->execute = nvmet_execute_identify_ns;
			return 0;
		case NVME_ID_CNS_CTRL:
			req->execute = nvmet_execute_identify_ctrl;
			return 0;
		case NVME_ID_CNS_NS_ACTIVE_LIST:
			req->execute = nvmet_execute_identify_nslist;
			return 0;
		case NVME_ID_CNS_NS_DESC_LIST:
			req->execute = nvmet_execute_identify_desclist;
			return 0;
		}
		break;
	case nvme_admin_abort_cmd:
		req->execute = nvmet_execute_abort;
		req->data_len = 0;
		return 0;
	case nvme_admin_set_features:
		req->execute = nvmet_execute_set_features;
		req->data_len = 0;
		return 0;
	case nvme_admin_get_features:
		req->execute = nvmet_execute_get_features;
		req->data_len = 0;
		return 0;
	case nvme_admin_async_event:
		req->execute = nvmet_execute_async_event;
		req->data_len = 0;
		return 0;
	case nvme_admin_keep_alive:
		req->execute = nvmet_execute_keep_alive;
		req->data_len = 0;
		return 0;
	}

	pr_err("unhandled cmd %d on qid %d\n", cmd->common.opcode,
	       req->sq->qid);
	return NVME_SC_INVALID_OPCODE | NVME_SC_DNR;
=======
	if (nvme_is_fabrics(cmd))
		return nvmet_parse_fabrics_cmd(req);
	if (nvmet_req_subsys(req)->type == NVME_NQN_DISC)
		return nvmet_parse_discovery_cmd(req);

	ret = nvmet_check_ctrl_status(req);
	if (unlikely(ret))
		return ret;

	if (nvmet_is_passthru_req(req))
		return nvmet_parse_passthru_admin_cmd(req);

	switch (cmd->common.opcode) {
	case nvme_admin_get_log_page:
		req->execute = nvmet_execute_get_log_page;
		return 0;
	case nvme_admin_identify:
		req->execute = nvmet_execute_identify;
		return 0;
	case nvme_admin_abort_cmd:
		req->execute = nvmet_execute_abort;
		return 0;
	case nvme_admin_set_features:
		req->execute = nvmet_execute_set_features;
		return 0;
	case nvme_admin_get_features:
		req->execute = nvmet_execute_get_features;
		return 0;
	case nvme_admin_async_event:
		req->execute = nvmet_execute_async_event;
		return 0;
	case nvme_admin_keep_alive:
		req->execute = nvmet_execute_keep_alive;
		return 0;
	default:
		return nvmet_report_invalid_opcode(req);
	}
>>>>>>> upstream/android-13
}
