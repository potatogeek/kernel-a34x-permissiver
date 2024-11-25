// SPDX-License-Identifier: GPL-2.0+

/*
 * Ptrace test for hw breakpoints
 *
 * Based on tools/testing/selftests/breakpoints/breakpoint_test.c
 *
 * This test forks and the parent then traces the child doing various
 * types of ptrace enabled breakpoints
 *
 * Copyright (C) 2018 Michael Neuling, IBM Corporation.
 */

#include <sys/ptrace.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/user.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
<<<<<<< HEAD
#include "ptrace.h"

/* Breakpoint access modes */
enum {
	BP_X = 1,
	BP_RW = 2,
	BP_W = 4,
};

static pid_t child_pid;
static struct ppc_debug_info dbginfo;

static void get_dbginfo(void)
{
	int ret;

	ret = ptrace(PPC_PTRACE_GETHWDBGINFO, child_pid, NULL, &dbginfo);
	if (ret) {
		perror("Can't get breakpoint info\n");
=======
#include <sys/syscall.h>
#include <linux/limits.h>
#include "ptrace.h"

#define SPRN_PVR	0x11F
#define PVR_8xx		0x00500000

bool is_8xx;

/*
 * Use volatile on all global var so that compiler doesn't
 * optimise their load/stores. Otherwise selftest can fail.
 */
static volatile __u64 glvar;

#define DAWR_MAX_LEN 512
static volatile __u8 big_var[DAWR_MAX_LEN] __attribute__((aligned(512)));

#define A_LEN 6
#define B_LEN 6
struct gstruct {
	__u8 a[A_LEN]; /* double word aligned */
	__u8 b[B_LEN]; /* double word unaligned */
};
static volatile struct gstruct gstruct __attribute__((aligned(512)));

static volatile char cwd[PATH_MAX] __attribute__((aligned(8)));

static void get_dbginfo(pid_t child_pid, struct ppc_debug_info *dbginfo)
{
	if (ptrace(PPC_PTRACE_GETHWDBGINFO, child_pid, NULL, dbginfo)) {
		perror("Can't get breakpoint info");
>>>>>>> upstream/android-13
		exit(-1);
	}
}

<<<<<<< HEAD
static bool hwbreak_present(void)
{
	return (dbginfo.num_data_bps != 0);
}

static bool dawr_present(void)
{
	return !!(dbginfo.features & PPC_DEBUG_FEATURE_DATA_BP_DAWR);
}

static void set_breakpoint_addr(void *addr)
{
	int ret;

	ret = ptrace(PTRACE_SET_DEBUGREG, child_pid, 0, addr);
	if (ret) {
		perror("Can't set breakpoint addr\n");
		exit(-1);
	}
}

static int set_hwbreakpoint_addr(void *addr, int range)
{
	int ret;

	struct ppc_hw_breakpoint info;

	info.version = 1;
	info.trigger_type = PPC_BREAKPOINT_TRIGGER_RW;
	info.addr_mode = PPC_BREAKPOINT_MODE_EXACT;
	if (range > 0)
		info.addr_mode = PPC_BREAKPOINT_MODE_RANGE_INCLUSIVE;
	info.condition_mode = PPC_BREAKPOINT_CONDITION_NONE;
	info.addr = (__u64)addr;
	info.addr2 = (__u64)addr + range;
	info.condition_value = 0;

	ret = ptrace(PPC_PTRACE_SETHWDEBUG, child_pid, 0, &info);
	if (ret < 0) {
		perror("Can't set breakpoint\n");
		exit(-1);
	}
	return ret;
}

static int del_hwbreakpoint_addr(int watchpoint_handle)
{
	int ret;

	ret = ptrace(PPC_PTRACE_DELHWDEBUG, child_pid, 0, watchpoint_handle);
	if (ret < 0) {
		perror("Can't delete hw breakpoint\n");
		exit(-1);
	}
	return ret;
}

#define DAWR_LENGTH_MAX 512

/* Dummy variables to test read/write accesses */
static unsigned long long
	dummy_array[DAWR_LENGTH_MAX / sizeof(unsigned long long)]
	__attribute__((aligned(512)));
static unsigned long long *dummy_var = dummy_array;

static void write_var(int len)
{
	long long *plval;
	char *pcval;
	short *psval;
	int *pival;

	switch (len) {
	case 1:
		pcval = (char *)dummy_var;
		*pcval = 0xff;
		break;
	case 2:
		psval = (short *)dummy_var;
		*psval = 0xffff;
		break;
	case 4:
		pival = (int *)dummy_var;
		*pival = 0xffffffff;
		break;
	case 8:
		plval = (long long *)dummy_var;
		*plval = 0xffffffffffffffffLL;
=======
static bool dawr_present(struct ppc_debug_info *dbginfo)
{
	return !!(dbginfo->features & PPC_DEBUG_FEATURE_DATA_BP_DAWR);
}

static void write_var(int len)
{
	__u8 *pcvar;
	__u16 *psvar;
	__u32 *pivar;
	__u64 *plvar;

	switch (len) {
	case 1:
		pcvar = (__u8 *)&glvar;
		*pcvar = 0xff;
		break;
	case 2:
		psvar = (__u16 *)&glvar;
		*psvar = 0xffff;
		break;
	case 4:
		pivar = (__u32 *)&glvar;
		*pivar = 0xffffffff;
		break;
	case 8:
		plvar = (__u64 *)&glvar;
		*plvar = 0xffffffffffffffffLL;
>>>>>>> upstream/android-13
		break;
	}
}

static void read_var(int len)
{
<<<<<<< HEAD
	char cval __attribute__((unused));
	short sval __attribute__((unused));
	int ival __attribute__((unused));
	long long lval __attribute__((unused));

	switch (len) {
	case 1:
		cval = *(char *)dummy_var;
		break;
	case 2:
		sval = *(short *)dummy_var;
		break;
	case 4:
		ival = *(int *)dummy_var;
		break;
	case 8:
		lval = *(long long *)dummy_var;
=======
	__u8 cvar __attribute__((unused));
	__u16 svar __attribute__((unused));
	__u32 ivar __attribute__((unused));
	__u64 lvar __attribute__((unused));

	switch (len) {
	case 1:
		cvar = (__u8)glvar;
		break;
	case 2:
		svar = (__u16)glvar;
		break;
	case 4:
		ivar = (__u32)glvar;
		break;
	case 8:
		lvar = (__u64)glvar;
>>>>>>> upstream/android-13
		break;
	}
}

<<<<<<< HEAD
/*
 * Do the r/w accesses to trigger the breakpoints. And run
 * the usual traps.
 */
static void trigger_tests(void)
{
	int len, ret;

	ret = ptrace(PTRACE_TRACEME, 0, NULL, 0);
	if (ret) {
		perror("Can't be traced?\n");
		return;
=======
static void test_workload(void)
{
	__u8 cvar __attribute__((unused));
	__u32 ivar __attribute__((unused));
	int len = 0;

	if (ptrace(PTRACE_TRACEME, 0, NULL, 0)) {
		perror("Child can't be traced?");
		exit(-1);
>>>>>>> upstream/android-13
	}

	/* Wake up father so that it sets up the first test */
	kill(getpid(), SIGUSR1);

<<<<<<< HEAD
	/* Test write watchpoints */
	for (len = 1; len <= sizeof(long); len <<= 1)
		write_var(len);

	/* Test read/write watchpoints (on read accesses) */
	for (len = 1; len <= sizeof(long); len <<= 1)
		read_var(len);

	/* Test when breakpoint is unset */

	/* Test write watchpoints */
	for (len = 1; len <= sizeof(long); len <<= 1)
		write_var(len);

	/* Test read/write watchpoints (on read accesses) */
	for (len = 1; len <= sizeof(long); len <<= 1)
		read_var(len);
}

static void check_success(const char *msg)
{
	const char *msg2;
	int status;
=======
	/* PTRACE_SET_DEBUGREG, WO test */
	for (len = 1; len <= sizeof(glvar); len <<= 1)
		write_var(len);

	/* PTRACE_SET_DEBUGREG, RO test */
	for (len = 1; len <= sizeof(glvar); len <<= 1)
		read_var(len);

	/* PTRACE_SET_DEBUGREG, RW test */
	for (len = 1; len <= sizeof(glvar); len <<= 1) {
		if (rand() % 2)
			read_var(len);
		else
			write_var(len);
	}

	/* PTRACE_SET_DEBUGREG, Kernel Access Userspace test */
	syscall(__NR_getcwd, &cwd, PATH_MAX);

	/* PPC_PTRACE_SETHWDEBUG, MODE_EXACT, WO test */
	write_var(1);

	/* PPC_PTRACE_SETHWDEBUG, MODE_EXACT, RO test */
	read_var(1);

	/* PPC_PTRACE_SETHWDEBUG, MODE_EXACT, RW test */
	if (rand() % 2)
		write_var(1);
	else
		read_var(1);

	/* PPC_PTRACE_SETHWDEBUG, MODE_EXACT, Kernel Access Userspace test */
	syscall(__NR_getcwd, &cwd, PATH_MAX);

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW ALIGNED, WO test */
	gstruct.a[rand() % A_LEN] = 'a';

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW ALIGNED, RO test */
	cvar = gstruct.a[rand() % A_LEN];

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW ALIGNED, RW test */
	if (rand() % 2)
		gstruct.a[rand() % A_LEN] = 'a';
	else
		cvar = gstruct.a[rand() % A_LEN];

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW UNALIGNED, WO test */
	gstruct.b[rand() % B_LEN] = 'b';

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW UNALIGNED, RO test */
	cvar = gstruct.b[rand() % B_LEN];

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW UNALIGNED, RW test */
	if (rand() % 2)
		gstruct.b[rand() % B_LEN] = 'b';
	else
		cvar = gstruct.b[rand() % B_LEN];

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW UNALIGNED, DAR OUTSIDE, RW test */
	if (rand() % 2)
		*((int *)(gstruct.a + 4)) = 10;
	else
		ivar = *((int *)(gstruct.a + 4));

	/* PPC_PTRACE_SETHWDEBUG. DAWR_MAX_LEN. RW test */
	if (rand() % 2)
		big_var[rand() % DAWR_MAX_LEN] = 'a';
	else
		cvar = big_var[rand() % DAWR_MAX_LEN];

	/* PPC_PTRACE_SETHWDEBUG 2, MODE_RANGE, DW ALIGNED, WO test */
	gstruct.a[rand() % A_LEN] = 'a';

	/* PPC_PTRACE_SETHWDEBUG 2, MODE_RANGE, DW UNALIGNED, RO test */
	cvar = gstruct.b[rand() % B_LEN];

	/* PPC_PTRACE_SETHWDEBUG 2, MODE_RANGE, DAWR Overlap, WO test */
	gstruct.a[rand() % A_LEN] = 'a';

	/* PPC_PTRACE_SETHWDEBUG 2, MODE_RANGE, DAWR Overlap, RO test */
	cvar = gstruct.a[rand() % A_LEN];
}

static void check_success(pid_t child_pid, const char *name, const char *type,
			  unsigned long saddr, int len)
{
	int status;
	siginfo_t siginfo;
	unsigned long eaddr = (saddr + len - 1) | 0x7;

	saddr &= ~0x7;
>>>>>>> upstream/android-13

	/* Wait for the child to SIGTRAP */
	wait(&status);

<<<<<<< HEAD
	msg2 = "Failed";

	if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
		msg2 = "Child process hit the breakpoint";
	}

	printf("%s Result: [%s]\n", msg, msg2);
}

static void launch_watchpoints(char *buf, int mode, int len,
			       struct ppc_debug_info *dbginfo, bool dawr)
{
	const char *mode_str;
	unsigned long data = (unsigned long)(dummy_var);
	int wh, range;

	data &= ~0x7UL;

	if (mode == BP_W) {
		data |= (1UL << 1);
		mode_str = "write";
	} else {
		data |= (1UL << 0);
		data |= (1UL << 1);
		mode_str = "read";
	}

	/* Set DABR_TRANSLATION bit */
	data |= (1UL << 2);

	/* use PTRACE_SET_DEBUGREG breakpoints */
	set_breakpoint_addr((void *)data);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	sprintf(buf, "Test %s watchpoint with len: %d ", mode_str, len);
	check_success(buf);
	/* Unregister hw brkpoint */
	set_breakpoint_addr(NULL);

	data = (data & ~7); /* remove dabr control bits */

	/* use PPC_PTRACE_SETHWDEBUG breakpoint */
	if (!(dbginfo->features & PPC_DEBUG_FEATURE_DATA_BP_RANGE))
		return; /* not supported */
	wh = set_hwbreakpoint_addr((void *)data, 0);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	sprintf(buf, "Test %s watchpoint with len: %d ", mode_str, len);
	check_success(buf);
	/* Unregister hw brkpoint */
	del_hwbreakpoint_addr(wh);

	/* try a wider range */
	range = 8;
	if (dawr)
		range = 512 - ((int)data & (DAWR_LENGTH_MAX - 1));
	wh = set_hwbreakpoint_addr((void *)data, range);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	sprintf(buf, "Test %s watchpoint with len: %d ", mode_str, len);
	check_success(buf);
	/* Unregister hw brkpoint */
	del_hwbreakpoint_addr(wh);
}

/* Set the breakpoints and check the child successfully trigger them */
static int launch_tests(bool dawr)
{
	char buf[1024];
	int len, i, status;

	struct ppc_debug_info dbginfo;

	i = ptrace(PPC_PTRACE_GETHWDBGINFO, child_pid, NULL, &dbginfo);
	if (i) {
		perror("Can't set breakpoint info\n");
		exit(-1);
	}
	if (!(dbginfo.features & PPC_DEBUG_FEATURE_DATA_BP_RANGE))
		printf("WARNING: Kernel doesn't support PPC_PTRACE_SETHWDEBUG\n");

	/* Write watchpoint */
	for (len = 1; len <= sizeof(long); len <<= 1)
		launch_watchpoints(buf, BP_W, len, &dbginfo, dawr);

	/* Read-Write watchpoint */
	for (len = 1; len <= sizeof(long); len <<= 1)
		launch_watchpoints(buf, BP_RW, len, &dbginfo, dawr);

	ptrace(PTRACE_CONT, child_pid, NULL, 0);

	/*
	 * Now we have unregistered the breakpoint, access by child
	 * should not cause SIGTRAP.
	 */

	wait(&status);

	if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
		printf("FAIL: Child process hit the breakpoint, which is not expected\n");
		ptrace(PTRACE_CONT, child_pid, NULL, 0);
		return TEST_FAIL;
	}

	if (WIFEXITED(status))
		printf("Child exited normally\n");

	return TEST_PASS;
=======
	ptrace(PTRACE_GETSIGINFO, child_pid, NULL, &siginfo);

	if (!WIFSTOPPED(status) || WSTOPSIG(status) != SIGTRAP ||
	    (unsigned long)siginfo.si_addr < saddr ||
	    (unsigned long)siginfo.si_addr > eaddr) {
		printf("%s, %s, len: %d: Fail\n", name, type, len);
		exit(-1);
	}

	printf("%s, %s, len: %d: Ok\n", name, type, len);

	if (!is_8xx) {
		/*
		 * For ptrace registered watchpoint, signal is generated
		 * before executing load/store. Singlestep the instruction
		 * and then continue the test.
		 */
		ptrace(PTRACE_SINGLESTEP, child_pid, NULL, 0);
		wait(NULL);
	}
}

static void ptrace_set_debugreg(pid_t child_pid, unsigned long wp_addr)
{
	if (ptrace(PTRACE_SET_DEBUGREG, child_pid, 0, wp_addr)) {
		perror("PTRACE_SET_DEBUGREG failed");
		exit(-1);
	}
}

static int ptrace_sethwdebug(pid_t child_pid, struct ppc_hw_breakpoint *info)
{
	int wh = ptrace(PPC_PTRACE_SETHWDEBUG, child_pid, 0, info);

	if (wh <= 0) {
		perror("PPC_PTRACE_SETHWDEBUG failed");
		exit(-1);
	}
	return wh;
}

static void ptrace_delhwdebug(pid_t child_pid, int wh)
{
	if (ptrace(PPC_PTRACE_DELHWDEBUG, child_pid, 0, wh) < 0) {
		perror("PPC_PTRACE_DELHWDEBUG failed");
		exit(-1);
	}
}

#define DABR_READ_SHIFT		0
#define DABR_WRITE_SHIFT	1
#define DABR_TRANSLATION_SHIFT	2

static int test_set_debugreg(pid_t child_pid)
{
	unsigned long wp_addr = (unsigned long)&glvar;
	char *name = "PTRACE_SET_DEBUGREG";
	int len;

	/* PTRACE_SET_DEBUGREG, WO test*/
	wp_addr &= ~0x7UL;
	wp_addr |= (1UL << DABR_WRITE_SHIFT);
	wp_addr |= (1UL << DABR_TRANSLATION_SHIFT);
	for (len = 1; len <= sizeof(glvar); len <<= 1) {
		ptrace_set_debugreg(child_pid, wp_addr);
		ptrace(PTRACE_CONT, child_pid, NULL, 0);
		check_success(child_pid, name, "WO", wp_addr, len);
	}

	/* PTRACE_SET_DEBUGREG, RO test */
	wp_addr &= ~0x7UL;
	wp_addr |= (1UL << DABR_READ_SHIFT);
	wp_addr |= (1UL << DABR_TRANSLATION_SHIFT);
	for (len = 1; len <= sizeof(glvar); len <<= 1) {
		ptrace_set_debugreg(child_pid, wp_addr);
		ptrace(PTRACE_CONT, child_pid, NULL, 0);
		check_success(child_pid, name, "RO", wp_addr, len);
	}

	/* PTRACE_SET_DEBUGREG, RW test */
	wp_addr &= ~0x7UL;
	wp_addr |= (1Ul << DABR_READ_SHIFT);
	wp_addr |= (1UL << DABR_WRITE_SHIFT);
	wp_addr |= (1UL << DABR_TRANSLATION_SHIFT);
	for (len = 1; len <= sizeof(glvar); len <<= 1) {
		ptrace_set_debugreg(child_pid, wp_addr);
		ptrace(PTRACE_CONT, child_pid, NULL, 0);
		check_success(child_pid, name, "RW", wp_addr, len);
	}

	ptrace_set_debugreg(child_pid, 0);
	return 0;
}

static int test_set_debugreg_kernel_userspace(pid_t child_pid)
{
	unsigned long wp_addr = (unsigned long)cwd;
	char *name = "PTRACE_SET_DEBUGREG";

	/* PTRACE_SET_DEBUGREG, Kernel Access Userspace test */
	wp_addr &= ~0x7UL;
	wp_addr |= (1Ul << DABR_READ_SHIFT);
	wp_addr |= (1UL << DABR_WRITE_SHIFT);
	wp_addr |= (1UL << DABR_TRANSLATION_SHIFT);
	ptrace_set_debugreg(child_pid, wp_addr);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "Kernel Access Userspace", wp_addr, 8);

	ptrace_set_debugreg(child_pid, 0);
	return 0;
}

static void get_ppc_hw_breakpoint(struct ppc_hw_breakpoint *info, int type,
				  unsigned long addr, int len)
{
	info->version = 1;
	info->trigger_type = type;
	info->condition_mode = PPC_BREAKPOINT_CONDITION_NONE;
	info->addr = (__u64)addr;
	info->addr2 = (__u64)addr + len;
	info->condition_value = 0;
	if (!len)
		info->addr_mode = PPC_BREAKPOINT_MODE_EXACT;
	else
		info->addr_mode = PPC_BREAKPOINT_MODE_RANGE_INCLUSIVE;
}

static void test_sethwdebug_exact(pid_t child_pid)
{
	struct ppc_hw_breakpoint info;
	unsigned long wp_addr = (unsigned long)&glvar;
	char *name = "PPC_PTRACE_SETHWDEBUG, MODE_EXACT";
	int len = 1; /* hardcoded in kernel */
	int wh;

	/* PPC_PTRACE_SETHWDEBUG, MODE_EXACT, WO test */
	get_ppc_hw_breakpoint(&info, PPC_BREAKPOINT_TRIGGER_WRITE, wp_addr, 0);
	wh = ptrace_sethwdebug(child_pid, &info);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "WO", wp_addr, len);
	ptrace_delhwdebug(child_pid, wh);

	/* PPC_PTRACE_SETHWDEBUG, MODE_EXACT, RO test */
	get_ppc_hw_breakpoint(&info, PPC_BREAKPOINT_TRIGGER_READ, wp_addr, 0);
	wh = ptrace_sethwdebug(child_pid, &info);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "RO", wp_addr, len);
	ptrace_delhwdebug(child_pid, wh);

	/* PPC_PTRACE_SETHWDEBUG, MODE_EXACT, RW test */
	get_ppc_hw_breakpoint(&info, PPC_BREAKPOINT_TRIGGER_RW, wp_addr, 0);
	wh = ptrace_sethwdebug(child_pid, &info);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "RW", wp_addr, len);
	ptrace_delhwdebug(child_pid, wh);
}

static void test_sethwdebug_exact_kernel_userspace(pid_t child_pid)
{
	struct ppc_hw_breakpoint info;
	unsigned long wp_addr = (unsigned long)&cwd;
	char *name = "PPC_PTRACE_SETHWDEBUG, MODE_EXACT";
	int len = 1; /* hardcoded in kernel */
	int wh;

	/* PPC_PTRACE_SETHWDEBUG, MODE_EXACT, Kernel Access Userspace test */
	get_ppc_hw_breakpoint(&info, PPC_BREAKPOINT_TRIGGER_WRITE, wp_addr, 0);
	wh = ptrace_sethwdebug(child_pid, &info);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "Kernel Access Userspace", wp_addr, len);
	ptrace_delhwdebug(child_pid, wh);
}

static void test_sethwdebug_range_aligned(pid_t child_pid)
{
	struct ppc_hw_breakpoint info;
	unsigned long wp_addr;
	char *name = "PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW ALIGNED";
	int len;
	int wh;

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW ALIGNED, WO test */
	wp_addr = (unsigned long)&gstruct.a;
	len = A_LEN;
	get_ppc_hw_breakpoint(&info, PPC_BREAKPOINT_TRIGGER_WRITE, wp_addr, len);
	wh = ptrace_sethwdebug(child_pid, &info);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "WO", wp_addr, len);
	ptrace_delhwdebug(child_pid, wh);

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW ALIGNED, RO test */
	wp_addr = (unsigned long)&gstruct.a;
	len = A_LEN;
	get_ppc_hw_breakpoint(&info, PPC_BREAKPOINT_TRIGGER_READ, wp_addr, len);
	wh = ptrace_sethwdebug(child_pid, &info);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "RO", wp_addr, len);
	ptrace_delhwdebug(child_pid, wh);

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW ALIGNED, RW test */
	wp_addr = (unsigned long)&gstruct.a;
	len = A_LEN;
	get_ppc_hw_breakpoint(&info, PPC_BREAKPOINT_TRIGGER_RW, wp_addr, len);
	wh = ptrace_sethwdebug(child_pid, &info);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "RW", wp_addr, len);
	ptrace_delhwdebug(child_pid, wh);
}

static void test_multi_sethwdebug_range(pid_t child_pid)
{
	struct ppc_hw_breakpoint info1, info2;
	unsigned long wp_addr1, wp_addr2;
	char *name1 = "PPC_PTRACE_SETHWDEBUG 2, MODE_RANGE, DW ALIGNED";
	char *name2 = "PPC_PTRACE_SETHWDEBUG 2, MODE_RANGE, DW UNALIGNED";
	int len1, len2;
	int wh1, wh2;

	wp_addr1 = (unsigned long)&gstruct.a;
	wp_addr2 = (unsigned long)&gstruct.b;
	len1 = A_LEN;
	len2 = B_LEN;
	get_ppc_hw_breakpoint(&info1, PPC_BREAKPOINT_TRIGGER_WRITE, wp_addr1, len1);
	get_ppc_hw_breakpoint(&info2, PPC_BREAKPOINT_TRIGGER_READ, wp_addr2, len2);

	/* PPC_PTRACE_SETHWDEBUG 2, MODE_RANGE, DW ALIGNED, WO test */
	wh1 = ptrace_sethwdebug(child_pid, &info1);

	/* PPC_PTRACE_SETHWDEBUG 2, MODE_RANGE, DW UNALIGNED, RO test */
	wh2 = ptrace_sethwdebug(child_pid, &info2);

	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name1, "WO", wp_addr1, len1);

	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name2, "RO", wp_addr2, len2);

	ptrace_delhwdebug(child_pid, wh1);
	ptrace_delhwdebug(child_pid, wh2);
}

static void test_multi_sethwdebug_range_dawr_overlap(pid_t child_pid)
{
	struct ppc_hw_breakpoint info1, info2;
	unsigned long wp_addr1, wp_addr2;
	char *name = "PPC_PTRACE_SETHWDEBUG 2, MODE_RANGE, DAWR Overlap";
	int len1, len2;
	int wh1, wh2;

	wp_addr1 = (unsigned long)&gstruct.a;
	wp_addr2 = (unsigned long)&gstruct.a;
	len1 = A_LEN;
	len2 = A_LEN;
	get_ppc_hw_breakpoint(&info1, PPC_BREAKPOINT_TRIGGER_WRITE, wp_addr1, len1);
	get_ppc_hw_breakpoint(&info2, PPC_BREAKPOINT_TRIGGER_READ, wp_addr2, len2);

	/* PPC_PTRACE_SETHWDEBUG 2, MODE_RANGE, DAWR Overlap, WO test */
	wh1 = ptrace_sethwdebug(child_pid, &info1);

	/* PPC_PTRACE_SETHWDEBUG 2, MODE_RANGE, DAWR Overlap, RO test */
	wh2 = ptrace_sethwdebug(child_pid, &info2);

	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "WO", wp_addr1, len1);

	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "RO", wp_addr2, len2);

	ptrace_delhwdebug(child_pid, wh1);
	ptrace_delhwdebug(child_pid, wh2);
}

static void test_sethwdebug_range_unaligned(pid_t child_pid)
{
	struct ppc_hw_breakpoint info;
	unsigned long wp_addr;
	char *name = "PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW UNALIGNED";
	int len;
	int wh;

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW UNALIGNED, WO test */
	wp_addr = (unsigned long)&gstruct.b;
	len = B_LEN;
	get_ppc_hw_breakpoint(&info, PPC_BREAKPOINT_TRIGGER_WRITE, wp_addr, len);
	wh = ptrace_sethwdebug(child_pid, &info);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "WO", wp_addr, len);
	ptrace_delhwdebug(child_pid, wh);

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW UNALIGNED, RO test */
	wp_addr = (unsigned long)&gstruct.b;
	len = B_LEN;
	get_ppc_hw_breakpoint(&info, PPC_BREAKPOINT_TRIGGER_READ, wp_addr, len);
	wh = ptrace_sethwdebug(child_pid, &info);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "RO", wp_addr, len);
	ptrace_delhwdebug(child_pid, wh);

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW UNALIGNED, RW test */
	wp_addr = (unsigned long)&gstruct.b;
	len = B_LEN;
	get_ppc_hw_breakpoint(&info, PPC_BREAKPOINT_TRIGGER_RW, wp_addr, len);
	wh = ptrace_sethwdebug(child_pid, &info);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "RW", wp_addr, len);
	ptrace_delhwdebug(child_pid, wh);

}

static void test_sethwdebug_range_unaligned_dar(pid_t child_pid)
{
	struct ppc_hw_breakpoint info;
	unsigned long wp_addr;
	char *name = "PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW UNALIGNED, DAR OUTSIDE";
	int len;
	int wh;

	/* PPC_PTRACE_SETHWDEBUG, MODE_RANGE, DW UNALIGNED, DAR OUTSIDE, RW test */
	wp_addr = (unsigned long)&gstruct.b;
	len = B_LEN;
	get_ppc_hw_breakpoint(&info, PPC_BREAKPOINT_TRIGGER_WRITE, wp_addr, len);
	wh = ptrace_sethwdebug(child_pid, &info);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "RW", wp_addr, len);
	ptrace_delhwdebug(child_pid, wh);
}

static void test_sethwdebug_dawr_max_range(pid_t child_pid)
{
	struct ppc_hw_breakpoint info;
	unsigned long wp_addr;
	char *name = "PPC_PTRACE_SETHWDEBUG, DAWR_MAX_LEN";
	int len;
	int wh;

	/* PPC_PTRACE_SETHWDEBUG, DAWR_MAX_LEN, RW test */
	wp_addr = (unsigned long)big_var;
	len = DAWR_MAX_LEN;
	get_ppc_hw_breakpoint(&info, PPC_BREAKPOINT_TRIGGER_RW, wp_addr, len);
	wh = ptrace_sethwdebug(child_pid, &info);
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	check_success(child_pid, name, "RW", wp_addr, len);
	ptrace_delhwdebug(child_pid, wh);
}

/* Set the breakpoints and check the child successfully trigger them */
static void
run_tests(pid_t child_pid, struct ppc_debug_info *dbginfo, bool dawr)
{
	test_set_debugreg(child_pid);
	test_set_debugreg_kernel_userspace(child_pid);
	test_sethwdebug_exact(child_pid);
	test_sethwdebug_exact_kernel_userspace(child_pid);
	if (dbginfo->features & PPC_DEBUG_FEATURE_DATA_BP_RANGE) {
		test_sethwdebug_range_aligned(child_pid);
		if (dawr || is_8xx) {
			test_sethwdebug_range_unaligned(child_pid);
			test_sethwdebug_range_unaligned_dar(child_pid);
			test_sethwdebug_dawr_max_range(child_pid);
			if (dbginfo->num_data_bps > 1) {
				test_multi_sethwdebug_range(child_pid);
				test_multi_sethwdebug_range_dawr_overlap(child_pid);
			}
		}
	}
>>>>>>> upstream/android-13
}

static int ptrace_hwbreak(void)
{
<<<<<<< HEAD
	pid_t pid;
	int ret;
	bool dawr;

	pid = fork();
	if (!pid) {
		trigger_tests();
=======
	pid_t child_pid;
	struct ppc_debug_info dbginfo;
	bool dawr;

	child_pid = fork();
	if (!child_pid) {
		test_workload();
>>>>>>> upstream/android-13
		return 0;
	}

	wait(NULL);

<<<<<<< HEAD
	child_pid = pid;

	get_dbginfo();
	SKIP_IF(!hwbreak_present());
	dawr = dawr_present();

	ret = launch_tests(dawr);

	wait(NULL);

	return ret;
=======
	get_dbginfo(child_pid, &dbginfo);
	SKIP_IF(dbginfo.num_data_bps == 0);

	dawr = dawr_present(&dbginfo);
	run_tests(child_pid, &dbginfo, dawr);

	/* Let the child exit first. */
	ptrace(PTRACE_CONT, child_pid, NULL, 0);
	wait(NULL);

	/*
	 * Testcases exits immediately with -1 on any failure. If
	 * it has reached here, it means all tests were successful.
	 */
	return TEST_PASS;
>>>>>>> upstream/android-13
}

int main(int argc, char **argv, char **envp)
{
<<<<<<< HEAD
=======
	int pvr = 0;
	asm __volatile__ ("mfspr %0,%1" : "=r"(pvr) : "i"(SPRN_PVR));
	if (pvr == PVR_8xx)
		is_8xx = true;

>>>>>>> upstream/android-13
	return test_harness(ptrace_hwbreak, "ptrace-hwbreak");
}
