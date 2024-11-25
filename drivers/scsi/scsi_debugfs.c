// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
=======
#include <linux/bitops.h>
>>>>>>> upstream/android-13
#include <linux/seq_file.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_dbg.h>
#include "scsi_debugfs.h"

#define SCSI_CMD_FLAG_NAME(name)[const_ilog2(SCMD_##name)] = #name
static const char *const scsi_cmd_flags[] = {
	SCSI_CMD_FLAG_NAME(TAGGED),
<<<<<<< HEAD
	SCSI_CMD_FLAG_NAME(UNCHECKED_ISA_DMA),
	SCSI_CMD_FLAG_NAME(INITIALIZED),
=======
	SCSI_CMD_FLAG_NAME(INITIALIZED),
	SCSI_CMD_FLAG_NAME(LAST),
>>>>>>> upstream/android-13
};
#undef SCSI_CMD_FLAG_NAME

static int scsi_flags_show(struct seq_file *m, const unsigned long flags,
			   const char *const *flag_name, int flag_name_count)
{
	bool sep = false;
	int i;

<<<<<<< HEAD
	for (i = 0; i < sizeof(flags) * BITS_PER_BYTE; i++) {
		if (!(flags & BIT(i)))
			continue;
=======
	for_each_set_bit(i, &flags, BITS_PER_LONG) {
>>>>>>> upstream/android-13
		if (sep)
			seq_puts(m, "|");
		sep = true;
		if (i < flag_name_count && flag_name[i])
			seq_puts(m, flag_name[i]);
		else
			seq_printf(m, "%d", i);
	}
	return 0;
}

void scsi_show_rq(struct seq_file *m, struct request *rq)
{
	struct scsi_cmnd *cmd = container_of(scsi_req(rq), typeof(*cmd), req);
	int alloc_ms = jiffies_to_msecs(jiffies - cmd->jiffies_at_alloc);
	int timeout_ms = jiffies_to_msecs(rq->timeout);
	const u8 *const cdb = READ_ONCE(cmd->cmnd);
	char buf[80] = "(?)";

	if (cdb)
		__scsi_format_command(buf, sizeof(buf), cdb, cmd->cmd_len);
	seq_printf(m, ", .cmd=%s, .retries=%d, .result = %#x, .flags=", buf,
		   cmd->retries, cmd->result);
	scsi_flags_show(m, cmd->flags, scsi_cmd_flags,
			ARRAY_SIZE(scsi_cmd_flags));
	seq_printf(m, ", .timeout=%d.%03d, allocated %d.%03d s ago",
		   timeout_ms / 1000, timeout_ms % 1000,
		   alloc_ms / 1000, alloc_ms % 1000);
}
