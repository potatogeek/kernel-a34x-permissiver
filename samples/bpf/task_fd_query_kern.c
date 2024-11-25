// SPDX-License-Identifier: GPL-2.0
#include <linux/version.h>
#include <linux/ptrace.h>
#include <uapi/linux/bpf.h>
<<<<<<< HEAD
#include "bpf_helpers.h"

SEC("kprobe/blk_start_request")
=======
#include <bpf/bpf_helpers.h>

SEC("kprobe/blk_mq_start_request")
>>>>>>> upstream/android-13
int bpf_prog1(struct pt_regs *ctx)
{
	return 0;
}

<<<<<<< HEAD
SEC("kretprobe/blk_account_io_completion")
=======
SEC("kretprobe/blk_account_io_done")
>>>>>>> upstream/android-13
int bpf_prog2(struct pt_regs *ctx)
{
	return 0;
}
char _license[] SEC("license") = "GPL";
u32 _version SEC("version") = LINUX_VERSION_CODE;
