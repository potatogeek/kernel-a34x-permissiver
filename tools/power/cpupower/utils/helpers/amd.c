// SPDX-License-Identifier: GPL-2.0
#if defined(__i386__) || defined(__x86_64__)
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>

#include <pci/pci.h>

#include "helpers/helpers.h"

#define MSR_AMD_PSTATE_STATUS	0xc0010063
#define MSR_AMD_PSTATE		0xc0010064
#define MSR_AMD_PSTATE_LIMIT	0xc0010061

<<<<<<< HEAD
union msr_pstate {
=======
union core_pstate {
	/* pre fam 17h: */
>>>>>>> upstream/android-13
	struct {
		unsigned fid:6;
		unsigned did:3;
		unsigned vid:7;
		unsigned res1:6;
		unsigned nbdid:1;
		unsigned res2:2;
		unsigned nbvid:7;
		unsigned iddval:8;
		unsigned idddiv:2;
		unsigned res3:21;
		unsigned en:1;
<<<<<<< HEAD
	} bits;
=======
	} pstate;
	/* since fam 17h: */
>>>>>>> upstream/android-13
	struct {
		unsigned fid:8;
		unsigned did:6;
		unsigned vid:8;
		unsigned iddval:8;
		unsigned idddiv:2;
		unsigned res1:31;
		unsigned en:1;
<<<<<<< HEAD
	} fam17h_bits;
	unsigned long long val;
};

static int get_did(int family, union msr_pstate pstate)
{
	int t;

	if (family == 0x12)
		t = pstate.val & 0xf;
	else if (family == 0x17)
		t = pstate.fam17h_bits.did;
	else
		t = pstate.bits.did;
=======
	} pstatedef;
	unsigned long long val;
};

static int get_did(union core_pstate pstate)
{
	int t;

	if (cpupower_cpu_info.caps & CPUPOWER_CAP_AMD_PSTATEDEF)
		t = pstate.pstatedef.did;
	else if (cpupower_cpu_info.family == 0x12)
		t = pstate.val & 0xf;
	else
		t = pstate.pstate.did;
>>>>>>> upstream/android-13

	return t;
}

<<<<<<< HEAD
static int get_cof(int family, union msr_pstate pstate)
=======
static int get_cof(union core_pstate pstate)
>>>>>>> upstream/android-13
{
	int t;
	int fid, did, cof;

<<<<<<< HEAD
	did = get_did(family, pstate);
	if (family == 0x17) {
		fid = pstate.fam17h_bits.fid;
		cof = 200 * fid / did;
	} else {
		t = 0x10;
		fid = pstate.bits.fid;
		if (family == 0x11)
=======
	did = get_did(pstate);
	if (cpupower_cpu_info.caps & CPUPOWER_CAP_AMD_PSTATEDEF) {
		fid = pstate.pstatedef.fid;
		cof = 200 * fid / did;
	} else {
		t = 0x10;
		fid = pstate.pstate.fid;
		if (cpupower_cpu_info.family == 0x11)
>>>>>>> upstream/android-13
			t = 0x8;
		cof = (100 * (fid + t)) >> did;
	}
	return cof;
}

/* Needs:
 * cpu          -> the cpu that gets evaluated
<<<<<<< HEAD
 * cpu_family   -> The cpu's family (0x10, 0x12,...)
 * boots_states -> how much boost states the machines support
=======
 * boost_states -> how much boost states the machines support
>>>>>>> upstream/android-13
 *
 * Fills up:
 * pstates -> a pointer to an array of size MAX_HW_PSTATES
 *            must be initialized with zeros.
 *            All available  HW pstates (including boost states)
 * no      -> amount of pstates above array got filled up with
 *
 * returns zero on success, -1 on failure
 */
<<<<<<< HEAD
int decode_pstates(unsigned int cpu, unsigned int cpu_family,
		   int boost_states, unsigned long *pstates, int *no)
{
	int i, psmax, pscur;
	union msr_pstate pstate;
	unsigned long long val;

	/* Only read out frequencies from HW when CPU might be boostable
	   to keep the code as short and clean as possible.
	   Otherwise frequencies are exported via ACPI tables.
	*/
	if (cpu_family < 0x10 || cpu_family == 0x14)
=======
int decode_pstates(unsigned int cpu, int boost_states,
		   unsigned long *pstates, int *no)
{
	int i, psmax;
	union core_pstate pstate;
	unsigned long long val;

	/* Only read out frequencies from HW if HW Pstate is supported,
	 * otherwise frequencies are exported via ACPI tables.
	 */
	if (!(cpupower_cpu_info.caps & CPUPOWER_CAP_AMD_HW_PSTATE))
>>>>>>> upstream/android-13
		return -1;

	if (read_msr(cpu, MSR_AMD_PSTATE_LIMIT, &val))
		return -1;

	psmax = (val >> 4) & 0x7;
<<<<<<< HEAD

	if (read_msr(cpu, MSR_AMD_PSTATE_STATUS, &val))
		return -1;

	pscur = val & 0x7;

	pscur += boost_states;
=======
>>>>>>> upstream/android-13
	psmax += boost_states;
	for (i = 0; i <= psmax; i++) {
		if (i >= MAX_HW_PSTATES) {
			fprintf(stderr, "HW pstates [%d] exceeding max [%d]\n",
				psmax, MAX_HW_PSTATES);
			return -1;
		}
		if (read_msr(cpu, MSR_AMD_PSTATE + i, &pstate.val))
			return -1;
<<<<<<< HEAD
		if ((cpu_family == 0x17) && (!pstate.fam17h_bits.en))
			continue;
		else if (!pstate.bits.en)
			continue;

		pstates[i] = get_cof(cpu_family, pstate);
=======

		/* The enabled bit (bit 63) is common for all families */
		if (!pstate.pstatedef.en)
			continue;

		pstates[i] = get_cof(pstate);
>>>>>>> upstream/android-13
	}
	*no = i;
	return 0;
}

int amd_pci_get_num_boost_states(int *active, int *states)
{
	struct pci_access *pci_acc;
	struct pci_dev *device;
	uint8_t val = 0;

	*active = *states = 0;

	device = pci_slot_func_init(&pci_acc, 0x18, 4);

	if (device == NULL)
		return -ENODEV;

	val = pci_read_byte(device, 0x15c);
	if (val & 3)
		*active = 1;
	else
		*active = 0;
	*states = (val >> 2) & 7;

	pci_cleanup(pci_acc);
	return 0;
}
#endif /* defined(__i386__) || defined(__x86_64__) */
