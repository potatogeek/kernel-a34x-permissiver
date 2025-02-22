<<<<<<< HEAD
/*
 * Copyright (C) 2016 Google, Inc.
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
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2016 Google, Inc.
>>>>>>> upstream/android-13
 */

#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../kselftest.h"

void child(int cpu)
{
	cpu_set_t set;

	CPU_ZERO(&set);
	CPU_SET(cpu, &set);
	if (sched_setaffinity(0, sizeof(set), &set) != 0) {
		ksft_print_msg("sched_setaffinity() failed: %s\n",
			strerror(errno));
		_exit(1);
	}

	if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) != 0) {
		ksft_print_msg("ptrace(PTRACE_TRACEME) failed: %s\n",
			strerror(errno));
		_exit(1);
	}

	if (raise(SIGSTOP) != 0) {
		ksft_print_msg("raise(SIGSTOP) failed: %s\n", strerror(errno));
		_exit(1);
	}

	_exit(0);
}

<<<<<<< HEAD
bool run_test(int cpu)
=======
int run_test(int cpu)
>>>>>>> upstream/android-13
{
	int status;
	pid_t pid = fork();
	pid_t wpid;

	if (pid < 0) {
		ksft_print_msg("fork() failed: %s\n", strerror(errno));
<<<<<<< HEAD
		return false;
=======
		return KSFT_FAIL;
>>>>>>> upstream/android-13
	}
	if (pid == 0)
		child(cpu);

	wpid = waitpid(pid, &status, __WALL);
	if (wpid != pid) {
		ksft_print_msg("waitpid() failed: %s\n", strerror(errno));
<<<<<<< HEAD
		return false;
	}
	if (!WIFSTOPPED(status)) {
		ksft_print_msg("child did not stop: %s\n", strerror(errno));
		return false;
=======
		return KSFT_FAIL;
	}
	if (!WIFSTOPPED(status)) {
		ksft_print_msg("child did not stop: %s\n", strerror(errno));
		return KSFT_FAIL;
>>>>>>> upstream/android-13
	}
	if (WSTOPSIG(status) != SIGSTOP) {
		ksft_print_msg("child did not stop with SIGSTOP: %s\n",
			strerror(errno));
<<<<<<< HEAD
		return false;
=======
		return KSFT_FAIL;
>>>>>>> upstream/android-13
	}

	if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) < 0) {
		if (errno == EIO) {
<<<<<<< HEAD
			ksft_exit_skip(
				"ptrace(PTRACE_SINGLESTEP) not supported on this architecture: %s\n",
				strerror(errno));
		}
		ksft_print_msg("ptrace(PTRACE_SINGLESTEP) failed: %s\n",
			strerror(errno));
		return false;
=======
			ksft_print_msg(
				"ptrace(PTRACE_SINGLESTEP) not supported on this architecture: %s\n",
				strerror(errno));
			return KSFT_SKIP;
		}
		ksft_print_msg("ptrace(PTRACE_SINGLESTEP) failed: %s\n",
			strerror(errno));
		return KSFT_FAIL;
>>>>>>> upstream/android-13
	}

	wpid = waitpid(pid, &status, __WALL);
	if (wpid != pid) {
		ksft_print_msg("waitpid() failed: $s\n", strerror(errno));
<<<<<<< HEAD
		return false;
=======
		return KSFT_FAIL;
>>>>>>> upstream/android-13
	}
	if (WIFEXITED(status)) {
		ksft_print_msg("child did not single-step: %s\n",
			strerror(errno));
<<<<<<< HEAD
		return false;
	}
	if (!WIFSTOPPED(status)) {
		ksft_print_msg("child did not stop: %s\n", strerror(errno));
		return false;
=======
		return KSFT_FAIL;
	}
	if (!WIFSTOPPED(status)) {
		ksft_print_msg("child did not stop: %s\n", strerror(errno));
		return KSFT_FAIL;
>>>>>>> upstream/android-13
	}
	if (WSTOPSIG(status) != SIGTRAP) {
		ksft_print_msg("child did not stop with SIGTRAP: %s\n",
			strerror(errno));
<<<<<<< HEAD
		return false;
=======
		return KSFT_FAIL;
>>>>>>> upstream/android-13
	}

	if (ptrace(PTRACE_CONT, pid, NULL, NULL) < 0) {
		ksft_print_msg("ptrace(PTRACE_CONT) failed: %s\n",
			strerror(errno));
<<<<<<< HEAD
		return false;
=======
		return KSFT_FAIL;
>>>>>>> upstream/android-13
	}

	wpid = waitpid(pid, &status, __WALL);
	if (wpid != pid) {
		ksft_print_msg("waitpid() failed: %s\n", strerror(errno));
<<<<<<< HEAD
		return false;
=======
		return KSFT_FAIL;
>>>>>>> upstream/android-13
	}
	if (!WIFEXITED(status)) {
		ksft_print_msg("child did not exit after PTRACE_CONT: %s\n",
			strerror(errno));
<<<<<<< HEAD
		return false;
	}

	return true;
=======
		return KSFT_FAIL;
	}

	return KSFT_PASS;
>>>>>>> upstream/android-13
}

void suspend(void)
{
	int power_state_fd;
	struct sigevent event = {};
	int timerfd;
	int err;
	struct itimerspec spec = {};

	if (getuid() != 0)
		ksft_exit_skip("Please run the test as root - Exiting.\n");

	power_state_fd = open("/sys/power/state", O_RDWR);
	if (power_state_fd < 0)
		ksft_exit_fail_msg(
			"open(\"/sys/power/state\") failed %s)\n",
			strerror(errno));

	timerfd = timerfd_create(CLOCK_BOOTTIME_ALARM, 0);
	if (timerfd < 0)
		ksft_exit_fail_msg("timerfd_create() failed\n");

	spec.it_value.tv_sec = 5;
	err = timerfd_settime(timerfd, 0, &spec, NULL);
	if (err < 0)
		ksft_exit_fail_msg("timerfd_settime() failed\n");

	if (write(power_state_fd, "mem", strlen("mem")) != strlen("mem"))
		ksft_exit_fail_msg("Failed to enter Suspend state\n");

	close(timerfd);
	close(power_state_fd);
}

int main(int argc, char **argv)
{
	int opt;
	bool do_suspend = true;
	bool succeeded = true;
<<<<<<< HEAD
=======
	unsigned int tests = 0;
>>>>>>> upstream/android-13
	cpu_set_t available_cpus;
	int err;
	int cpu;

	ksft_print_header();

	while ((opt = getopt(argc, argv, "n")) != -1) {
		switch (opt) {
		case 'n':
			do_suspend = false;
			break;
		default:
			printf("Usage: %s [-n]\n", argv[0]);
			printf("        -n: do not trigger a suspend/resume cycle before the test\n");
			return -1;
		}
	}

<<<<<<< HEAD
	if (do_suspend)
		suspend();

=======
>>>>>>> upstream/android-13
	err = sched_getaffinity(0, sizeof(available_cpus), &available_cpus);
	if (err < 0)
		ksft_exit_fail_msg("sched_getaffinity() failed\n");

	for (cpu = 0; cpu < CPU_SETSIZE; cpu++) {
<<<<<<< HEAD
		bool test_success;
=======
		if (!CPU_ISSET(cpu, &available_cpus))
			continue;
		tests++;
	}

	if (do_suspend)
		suspend();

	ksft_set_plan(tests);
	for (cpu = 0; cpu < CPU_SETSIZE; cpu++) {
		int test_success;
>>>>>>> upstream/android-13

		if (!CPU_ISSET(cpu, &available_cpus))
			continue;

		test_success = run_test(cpu);
<<<<<<< HEAD
		if (test_success) {
			ksft_test_result_pass("CPU %d\n", cpu);
		} else {
			ksft_test_result_fail("CPU %d\n", cpu);
			succeeded = false;
=======
		switch (test_success) {
		case KSFT_PASS:
			ksft_test_result_pass("CPU %d\n", cpu);
			break;
		case KSFT_SKIP:
			ksft_test_result_skip("CPU %d\n", cpu);
			break;
		case KSFT_FAIL:
			ksft_test_result_fail("CPU %d\n", cpu);
			succeeded = false;
			break;
>>>>>>> upstream/android-13
		}
	}

	if (succeeded)
		ksft_exit_pass();
	else
		ksft_exit_fail();
}
