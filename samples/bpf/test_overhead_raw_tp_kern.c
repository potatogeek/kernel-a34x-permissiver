// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) 2018 Facebook */
#include <uapi/linux/bpf.h>
<<<<<<< HEAD
#include "bpf_helpers.h"
=======
#include <bpf/bpf_helpers.h>
>>>>>>> upstream/android-13

SEC("raw_tracepoint/task_rename")
int prog(struct bpf_raw_tracepoint_args *ctx)
{
	return 0;
}

SEC("raw_tracepoint/urandom_read")
int prog2(struct bpf_raw_tracepoint_args *ctx)
{
	return 0;
}
char _license[] SEC("license") = "GPL";
