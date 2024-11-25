// SPDX-License-Identifier: GPL-2.0
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/stringify.h>
#include "header.h"
<<<<<<< HEAD
#include "util.h"

#define mfspr(rn)       ({unsigned long rval; \
			 asm volatile("mfspr %0," __stringify(rn) \
				      : "=r" (rval)); rval; })

#define SPRN_PVR        0x11F	/* Processor Version Register */
#define PVR_VER(pvr)    (((pvr) >>  16) & 0xFFFF) /* Version field */
#define PVR_REV(pvr)    (((pvr) >>   0) & 0xFFFF) /* Revison field */
=======
#include "utils_header.h"
#include "metricgroup.h"
#include <api/fs/fs.h>
>>>>>>> upstream/android-13

int
get_cpuid(char *buffer, size_t sz)
{
	unsigned long pvr;
	int nb;

	pvr = mfspr(SPRN_PVR);

	nb = scnprintf(buffer, sz, "%lu,%lu$", PVR_VER(pvr), PVR_REV(pvr));

	/* look for end marker to ensure the entire data fit */
	if (strchr(buffer, '$')) {
		buffer[nb-1] = '\0';
		return 0;
	}
	return ENOBUFS;
}

char *
get_cpuid_str(struct perf_pmu *pmu __maybe_unused)
{
	char *bufp;

	if (asprintf(&bufp, "%.8lx", mfspr(SPRN_PVR)) < 0)
		bufp = NULL;

	return bufp;
}
<<<<<<< HEAD
=======

int arch_get_runtimeparam(struct pmu_event *pe)
{
	int count;
	char path[PATH_MAX] = "/devices/hv_24x7/interface/";

	atoi(pe->aggr_mode) == PerChip ? strcat(path, "sockets") : strcat(path, "coresperchip");
	return sysfs__read_int(path, &count) < 0 ? 1 : count;
}
>>>>>>> upstream/android-13
