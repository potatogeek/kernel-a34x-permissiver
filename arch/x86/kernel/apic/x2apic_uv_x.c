/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * SGI UV APIC functions (note: not an Intel compatible APIC)
 *
<<<<<<< HEAD
 * Copyright (C) 2007-2014 Silicon Graphics, Inc. All rights reserved.
 */
#include <linux/cpumask.h>
#include <linux/hardirq.h>
#include <linux/proc_fs.h>
#include <linux/threads.h>
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/cpu.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/pci.h>
#include <linux/kdebug.h>
#include <linux/delay.h>
#include <linux/crash_dump.h>
#include <linux/reboot.h>
#include <linux/memory.h>

#include <asm/uv/uv_mmrs.h>
#include <asm/uv/uv_hub.h>
#include <asm/current.h>
#include <asm/pgtable.h>
#include <asm/uv/bios.h>
#include <asm/uv/uv.h>
#include <asm/apic.h>
#include <asm/e820/api.h>
#include <asm/ipi.h>
#include <asm/smp.h>
#include <asm/x86_init.h>
#include <asm/nmi.h>

DEFINE_PER_CPU(int, x2apic_extra_bits);

static enum uv_system_type	uv_system_type;
static bool			uv_hubless_system;
static u64			gru_start_paddr, gru_end_paddr;
static u64			gru_dist_base, gru_first_node_paddr = -1LL, gru_last_node_paddr;
static u64			gru_dist_lmask, gru_dist_umask;
static union uvh_apicid		uvh_apicid;

/* Information derived from CPUID: */
static struct {
	unsigned int apicid_shift;
	unsigned int apicid_mask;
	unsigned int socketid_shift;	/* aka pnode_shift for UV1/2/3 */
	unsigned int pnode_mask;
	unsigned int gpa_shift;
	unsigned int gnode_shift;
} uv_cpuid;

int uv_min_hub_revision_id;
EXPORT_SYMBOL_GPL(uv_min_hub_revision_id);

unsigned int uv_apicid_hibits;
EXPORT_SYMBOL_GPL(uv_apicid_hibits);
=======
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright (C) 2007-2014 Silicon Graphics, Inc. All rights reserved.
 */
#include <linux/crash_dump.h>
#include <linux/cpuhotplug.h>
#include <linux/cpumask.h>
#include <linux/proc_fs.h>
#include <linux/memory.h>
#include <linux/export.h>
#include <linux/pci.h>
#include <linux/acpi.h>
#include <linux/efi.h>

#include <asm/e820/api.h>
#include <asm/uv/uv_mmrs.h>
#include <asm/uv/uv_hub.h>
#include <asm/uv/bios.h>
#include <asm/uv/uv.h>
#include <asm/apic.h>

static enum uv_system_type	uv_system_type;
static int			uv_hubbed_system;
static int			uv_hubless_system;
static u64			gru_start_paddr, gru_end_paddr;
static union uvh_apicid		uvh_apicid;
static int			uv_node_id;

/* Unpack AT/OEM/TABLE ID's to be NULL terminated strings */
static u8 uv_archtype[UV_AT_SIZE + 1];
static u8 oem_id[ACPI_OEM_ID_SIZE + 1];
static u8 oem_table_id[ACPI_OEM_TABLE_ID_SIZE + 1];

/* Information derived from CPUID and some UV MMRs */
static struct {
	unsigned int apicid_shift;
	unsigned int apicid_mask;
	unsigned int socketid_shift;	/* aka pnode_shift for UV2/3 */
	unsigned int pnode_mask;
	unsigned int nasid_shift;
	unsigned int gpa_shift;
	unsigned int gnode_shift;
	unsigned int m_skt;
	unsigned int n_skt;
} uv_cpuid;

static int uv_min_hub_revision_id;
>>>>>>> upstream/android-13

static struct apic apic_x2apic_uv_x;
static struct uv_hub_info_s uv_hub_info_node0;

/* Set this to use hardware error handler instead of kernel panic: */
static int disable_uv_undefined_panic = 1;

unsigned long uv_undefined(char *str)
{
	if (likely(!disable_uv_undefined_panic))
		panic("UV: error: undefined MMR: %s\n", str);
	else
		pr_crit("UV: error: undefined MMR: %s\n", str);

	/* Cause a machine fault: */
	return ~0ul;
}
EXPORT_SYMBOL(uv_undefined);

static unsigned long __init uv_early_read_mmr(unsigned long addr)
{
	unsigned long val, *mmr;

	mmr = early_ioremap(UV_LOCAL_MMR_BASE | addr, sizeof(*mmr));
	val = *mmr;
	early_iounmap(mmr, sizeof(*mmr));

	return val;
}

static inline bool is_GRU_range(u64 start, u64 end)
{
<<<<<<< HEAD
	if (gru_dist_base) {
		u64 su = start & gru_dist_umask; /* Upper (incl pnode) bits */
		u64 sl = start & gru_dist_lmask; /* Base offset bits */
		u64 eu = end & gru_dist_umask;
		u64 el = end & gru_dist_lmask;

		/* Must reside completely within a single GRU range: */
		return (sl == gru_dist_base && el == gru_dist_base &&
			su >= gru_first_node_paddr &&
			su <= gru_last_node_paddr &&
			eu == su);
	} else {
		return start >= gru_start_paddr && end <= gru_end_paddr;
	}
=======
	if (!gru_start_paddr)
		return false;

	return start >= gru_start_paddr && end <= gru_end_paddr;
>>>>>>> upstream/android-13
}

static bool uv_is_untracked_pat_range(u64 start, u64 end)
{
	return is_ISA_range(start, end) || is_GRU_range(start, end);
}

<<<<<<< HEAD
static int __init early_get_pnodeid(void)
{
	union uvh_node_id_u node_id;
	union uvh_rh_gam_config_mmr_u  m_n_config;
	int pnode;

	/* Currently, all blades have same revision number */
	node_id.v = uv_early_read_mmr(UVH_NODE_ID);
	m_n_config.v = uv_early_read_mmr(UVH_RH_GAM_CONFIG_MMR);
	uv_min_hub_revision_id = node_id.s.revision;

	switch (node_id.s.part_number) {
	case UV2_HUB_PART_NUMBER:
	case UV2_HUB_PART_NUMBER_X:
		uv_min_hub_revision_id += UV2_HUB_REVISION_BASE - 1;
		break;
	case UV3_HUB_PART_NUMBER:
	case UV3_HUB_PART_NUMBER_X:
		uv_min_hub_revision_id += UV3_HUB_REVISION_BASE;
		break;

	/* Update: UV4A has only a modified revision to indicate HUB fixes */
	case UV4_HUB_PART_NUMBER:
		uv_min_hub_revision_id += UV4_HUB_REVISION_BASE - 1;
		uv_cpuid.gnode_shift = 2; /* min partition is 4 sockets */
		break;
	}

	uv_hub_info->hub_revision = uv_min_hub_revision_id;
	uv_cpuid.pnode_mask = (1 << m_n_config.s.n_skt) - 1;
	pnode = (node_id.s.node_id >> 1) & uv_cpuid.pnode_mask;
	uv_cpuid.gpa_shift = 46;	/* Default unless changed */

	pr_info("UV: rev:%d part#:%x nodeid:%04x n_skt:%d pnmsk:%x pn:%x\n",
		node_id.s.revision, node_id.s.part_number, node_id.s.node_id,
		m_n_config.s.n_skt, uv_cpuid.pnode_mask, pnode);
	return pnode;
=======
static void __init early_get_pnodeid(void)
{
	int pnode;

	uv_cpuid.m_skt = 0;
	if (UVH_RH10_GAM_ADDR_MAP_CONFIG) {
		union uvh_rh10_gam_addr_map_config_u  m_n_config;

		m_n_config.v = uv_early_read_mmr(UVH_RH10_GAM_ADDR_MAP_CONFIG);
		uv_cpuid.n_skt = m_n_config.s.n_skt;
		uv_cpuid.nasid_shift = 0;
	} else if (UVH_RH_GAM_ADDR_MAP_CONFIG) {
		union uvh_rh_gam_addr_map_config_u  m_n_config;

	m_n_config.v = uv_early_read_mmr(UVH_RH_GAM_ADDR_MAP_CONFIG);
		uv_cpuid.n_skt = m_n_config.s.n_skt;
		if (is_uv(UV3))
			uv_cpuid.m_skt = m_n_config.s3.m_skt;
		if (is_uv(UV2))
			uv_cpuid.m_skt = m_n_config.s2.m_skt;
		uv_cpuid.nasid_shift = 1;
	} else {
		unsigned long GAM_ADDR_MAP_CONFIG = 0;

		WARN(GAM_ADDR_MAP_CONFIG == 0,
			"UV: WARN: GAM_ADDR_MAP_CONFIG is not available\n");
		uv_cpuid.n_skt = 0;
		uv_cpuid.nasid_shift = 0;
	}

	if (is_uv(UV4|UVY))
		uv_cpuid.gnode_shift = 2; /* min partition is 4 sockets */

	uv_cpuid.pnode_mask = (1 << uv_cpuid.n_skt) - 1;
	pnode = (uv_node_id >> uv_cpuid.nasid_shift) & uv_cpuid.pnode_mask;
	uv_cpuid.gpa_shift = 46;	/* Default unless changed */

	pr_info("UV: n_skt:%d pnmsk:%x pn:%x\n",
		uv_cpuid.n_skt, uv_cpuid.pnode_mask, pnode);
}

/* Running on a UV Hubbed system, determine which UV Hub Type it is */
static int __init early_set_hub_type(void)
{
	union uvh_node_id_u node_id;

	/*
	 * The NODE_ID MMR is always at offset 0.
	 * Contains the chip part # + revision.
	 * Node_id field started with 15 bits,
	 * ... now 7 but upper 8 are masked to 0.
	 * All blades/nodes have the same part # and hub revision.
	 */
	node_id.v = uv_early_read_mmr(UVH_NODE_ID);
	uv_node_id = node_id.sx.node_id;

	switch (node_id.s.part_number) {

	case UV5_HUB_PART_NUMBER:
		uv_min_hub_revision_id = node_id.s.revision
					 + UV5_HUB_REVISION_BASE;
		uv_hub_type_set(UV5);
		break;

	/* UV4/4A only have a revision difference */
	case UV4_HUB_PART_NUMBER:
		uv_min_hub_revision_id = node_id.s.revision
					 + UV4_HUB_REVISION_BASE - 1;
		uv_hub_type_set(UV4);
		if (uv_min_hub_revision_id == UV4A_HUB_REVISION_BASE)
			uv_hub_type_set(UV4|UV4A);
		break;

	case UV3_HUB_PART_NUMBER:
	case UV3_HUB_PART_NUMBER_X:
		uv_min_hub_revision_id = node_id.s.revision
					 + UV3_HUB_REVISION_BASE;
		uv_hub_type_set(UV3);
		break;

	case UV2_HUB_PART_NUMBER:
	case UV2_HUB_PART_NUMBER_X:
		uv_min_hub_revision_id = node_id.s.revision
					 + UV2_HUB_REVISION_BASE - 1;
		uv_hub_type_set(UV2);
		break;

	default:
		return 0;
	}

	pr_info("UV: part#:%x rev:%d rev_id:%d UVtype:0x%x\n",
		node_id.s.part_number, node_id.s.revision,
		uv_min_hub_revision_id, is_uv(~0));

	return 1;
>>>>>>> upstream/android-13
}

static void __init uv_tsc_check_sync(void)
{
	u64 mmr;
	int sync_state;
	int mmr_shift;
	char *state;
<<<<<<< HEAD
	bool valid;

	/* Accommodate different UV arch BIOSes */
	mmr = uv_early_read_mmr(UVH_TSC_SYNC_MMR);
	mmr_shift =
		is_uv1_hub() ? 0 :
		is_uv2_hub() ? UVH_TSC_SYNC_SHIFT_UV2K : UVH_TSC_SYNC_SHIFT;
	if (mmr_shift)
		sync_state = (mmr >> mmr_shift) & UVH_TSC_SYNC_MASK;
	else
		sync_state = 0;

	switch (sync_state) {
	case UVH_TSC_SYNC_VALID:
		state = "in sync";
		valid = true;
		break;

	case UVH_TSC_SYNC_INVALID:
		state = "unstable";
		valid = false;
		break;
	default:
		state = "unknown: assuming valid";
		valid = true;
		break;
	}
	pr_info("UV: TSC sync state from BIOS:0%d(%s)\n", sync_state, state);

	/* Mark flag that says TSC != 0 is valid for socket 0 */
	if (valid)
		mark_tsc_async_resets("UV BIOS");
	else
		mark_tsc_unstable("UV BIOS");
}

=======

	/* Different returns from different UV BIOS versions */
	mmr = uv_early_read_mmr(UVH_TSC_SYNC_MMR);
	mmr_shift =
		is_uv2_hub() ? UVH_TSC_SYNC_SHIFT_UV2K : UVH_TSC_SYNC_SHIFT;
	sync_state = (mmr >> mmr_shift) & UVH_TSC_SYNC_MASK;

	/* Check if TSC is valid for all sockets */
	switch (sync_state) {
	case UVH_TSC_SYNC_VALID:
		state = "in sync";
		mark_tsc_async_resets("UV BIOS");
		break;

	/* If BIOS state unknown, don't do anything */
	case UVH_TSC_SYNC_UNKNOWN:
		state = "unknown";
		break;

	/* Otherwise, BIOS indicates problem with TSC */
	default:
		state = "unstable";
		mark_tsc_unstable("UV BIOS");
		break;
	}
	pr_info("UV: TSC sync state from BIOS:0%d(%s)\n", sync_state, state);
}

/* Selector for (4|4A|5) structs */
#define uvxy_field(sname, field, undef) (	\
	is_uv(UV4A) ? sname.s4a.field :		\
	is_uv(UV4) ? sname.s4.field :		\
	is_uv(UV3) ? sname.s3.field :		\
	undef)

>>>>>>> upstream/android-13
/* [Copied from arch/x86/kernel/cpu/topology.c:detect_extended_topology()] */

#define SMT_LEVEL			0	/* Leaf 0xb SMT level */
#define INVALID_TYPE			0	/* Leaf 0xb sub-leaf types */
#define SMT_TYPE			1
#define CORE_TYPE			2
#define LEAFB_SUBTYPE(ecx)		(((ecx) >> 8) & 0xff)
#define BITS_SHIFT_NEXT_LEVEL(eax)	((eax) & 0x1f)

static void set_x2apic_bits(void)
{
	unsigned int eax, ebx, ecx, edx, sub_index;
	unsigned int sid_shift;

	cpuid(0, &eax, &ebx, &ecx, &edx);
	if (eax < 0xb) {
		pr_info("UV: CPU does not have CPUID.11\n");
		return;
	}

	cpuid_count(0xb, SMT_LEVEL, &eax, &ebx, &ecx, &edx);
	if (ebx == 0 || (LEAFB_SUBTYPE(ecx) != SMT_TYPE)) {
		pr_info("UV: CPUID.11 not implemented\n");
		return;
	}

	sid_shift = BITS_SHIFT_NEXT_LEVEL(eax);
	sub_index = 1;
	do {
		cpuid_count(0xb, sub_index, &eax, &ebx, &ecx, &edx);
		if (LEAFB_SUBTYPE(ecx) == CORE_TYPE) {
			sid_shift = BITS_SHIFT_NEXT_LEVEL(eax);
			break;
		}
		sub_index++;
	} while (LEAFB_SUBTYPE(ecx) != INVALID_TYPE);

	uv_cpuid.apicid_shift	= 0;
	uv_cpuid.apicid_mask	= (~(-1 << sid_shift));
	uv_cpuid.socketid_shift = sid_shift;
}

static void __init early_get_apic_socketid_shift(void)
{
	if (is_uv2_hub() || is_uv3_hub())
		uvh_apicid.v = uv_early_read_mmr(UVH_APICID);

	set_x2apic_bits();

	pr_info("UV: apicid_shift:%d apicid_mask:0x%x\n", uv_cpuid.apicid_shift, uv_cpuid.apicid_mask);
	pr_info("UV: socketid_shift:%d pnode_mask:0x%x\n", uv_cpuid.socketid_shift, uv_cpuid.pnode_mask);
}

<<<<<<< HEAD
/*
 * Add an extra bit as dictated by bios to the destination apicid of
 * interrupts potentially passing through the UV HUB.  This prevents
 * a deadlock between interrupts and IO port operations.
 */
static void __init uv_set_apicid_hibit(void)
{
	union uv1h_lb_target_physical_apic_id_mask_u apicid_mask;

	if (is_uv1_hub()) {
		apicid_mask.v = uv_early_read_mmr(UV1H_LB_TARGET_PHYSICAL_APIC_ID_MASK);
		uv_apicid_hibits = apicid_mask.s1.bit_enables & UV_APICID_HIBIT_MASK;
	}
}

static int __init uv_acpi_madt_oem_check(char *oem_id, char *oem_table_id)
{
	int pnodeid;
	int uv_apic;

	if (strncmp(oem_id, "SGI", 3) != 0) {
		if (strncmp(oem_id, "NSGI", 4) == 0) {
			uv_hubless_system = true;
			pr_info("UV: OEM IDs %s/%s, HUBLESS\n",
				oem_id, oem_table_id);
		}
=======
static void __init uv_stringify(int len, char *to, char *from)
{
	/* Relies on 'to' being NULL chars so result will be NULL terminated */
	strncpy(to, from, len-1);

	/* Trim trailing spaces */
	(void)strim(to);
}

/* Find UV arch type entry in UVsystab */
static unsigned long __init early_find_archtype(struct uv_systab *st)
{
	int i;

	for (i = 0; st->entry[i].type != UV_SYSTAB_TYPE_UNUSED; i++) {
		unsigned long ptr = st->entry[i].offset;

		if (!ptr)
			continue;
		ptr += (unsigned long)st;
		if (st->entry[i].type == UV_SYSTAB_TYPE_ARCH_TYPE)
			return ptr;
	}
	return 0;
}

/* Validate UV arch type field in UVsystab */
static int __init decode_arch_type(unsigned long ptr)
{
	struct uv_arch_type_entry *uv_ate = (struct uv_arch_type_entry *)ptr;
	int n = strlen(uv_ate->archtype);

	if (n > 0 && n < sizeof(uv_ate->archtype)) {
		pr_info("UV: UVarchtype received from BIOS\n");
		uv_stringify(sizeof(uv_archtype), uv_archtype, uv_ate->archtype);
		return 1;
	}
	return 0;
}

/* Determine if UV arch type entry might exist in UVsystab */
static int __init early_get_arch_type(void)
{
	unsigned long uvst_physaddr, uvst_size, ptr;
	struct uv_systab *st;
	u32 rev;
	int ret;

	uvst_physaddr = get_uv_systab_phys(0);
	if (!uvst_physaddr)
		return 0;

	st = early_memremap_ro(uvst_physaddr, sizeof(struct uv_systab));
	if (!st) {
		pr_err("UV: Cannot access UVsystab, remap failed\n");
		return 0;
	}

	rev = st->revision;
	if (rev < UV_SYSTAB_VERSION_UV5) {
		early_memunmap(st, sizeof(struct uv_systab));
		return 0;
	}

	uvst_size = st->size;
	early_memunmap(st, sizeof(struct uv_systab));
	st = early_memremap_ro(uvst_physaddr, uvst_size);
	if (!st) {
		pr_err("UV: Cannot access UVarchtype, remap failed\n");
		return 0;
	}

	ptr = early_find_archtype(st);
	if (!ptr) {
		early_memunmap(st, uvst_size);
		return 0;
	}

	ret = decode_arch_type(ptr);
	early_memunmap(st, uvst_size);
	return ret;
}

/* UV system found, check which APIC MODE BIOS already selected */
static void __init early_set_apic_mode(void)
{
	if (x2apic_enabled())
		uv_system_type = UV_X2APIC;
	else
		uv_system_type = UV_LEGACY_APIC;
}

static int __init uv_set_system_type(char *_oem_id, char *_oem_table_id)
{
	/* Save OEM_ID passed from ACPI MADT */
	uv_stringify(sizeof(oem_id), oem_id, _oem_id);

	/* Check if BIOS sent us a UVarchtype */
	if (!early_get_arch_type())

		/* If not use OEM ID for UVarchtype */
		uv_stringify(sizeof(uv_archtype), uv_archtype, oem_id);

	/* Check if not hubbed */
	if (strncmp(uv_archtype, "SGI", 3) != 0) {

		/* (Not hubbed), check if not hubless */
		if (strncmp(uv_archtype, "NSGI", 4) != 0)

			/* (Not hubless), not a UV */
			return 0;

		/* Is UV hubless system */
		uv_hubless_system = 0x01;

		/* UV5 Hubless */
		if (strncmp(uv_archtype, "NSGI5", 5) == 0)
			uv_hubless_system |= 0x20;

		/* UV4 Hubless: CH */
		else if (strncmp(uv_archtype, "NSGI4", 5) == 0)
			uv_hubless_system |= 0x10;

		/* UV3 Hubless: UV300/MC990X w/o hub */
		else
			uv_hubless_system |= 0x8;

		/* Copy OEM Table ID */
		uv_stringify(sizeof(oem_table_id), oem_table_id, _oem_table_id);

		pr_info("UV: OEM IDs %s/%s, SystemType %d, HUBLESS ID %x\n",
			oem_id, oem_table_id, uv_system_type, uv_hubless_system);

>>>>>>> upstream/android-13
		return 0;
	}

	if (numa_off) {
		pr_err("UV: NUMA is off, disabling UV support\n");
		return 0;
	}

<<<<<<< HEAD
	/* Set up early hub type field in uv_hub_info for Node 0 */
	uv_cpu_info->p_uv_hub_info = &uv_hub_info_node0;

	/*
	 * Determine UV arch type.
	 *   SGI:  UV100/1000
	 *   SGI2: UV2000/3000
	 *   SGI3: UV300 (truncated to 4 chars because of different varieties)
	 *   SGI4: UV400 (truncated to 4 chars because of different varieties)
	 */
	uv_hub_info->hub_revision =
		!strncmp(oem_id, "SGI4", 4) ? UV4_HUB_REVISION_BASE :
		!strncmp(oem_id, "SGI3", 4) ? UV3_HUB_REVISION_BASE :
		!strcmp(oem_id, "SGI2") ? UV2_HUB_REVISION_BASE :
		!strcmp(oem_id, "SGI") ? UV1_HUB_REVISION_BASE : 0;

	if (uv_hub_info->hub_revision == 0)
		goto badbios;

	pnodeid = early_get_pnodeid();
	early_get_apic_socketid_shift();

	x86_platform.is_untracked_pat_range = uv_is_untracked_pat_range;
	x86_platform.nmi_init = uv_nmi_init;

	if (!strcmp(oem_table_id, "UVX")) {
		/* This is the most common hardware variant: */
		uv_system_type = UV_X2APIC;
		uv_apic = 0;

	} else if (!strcmp(oem_table_id, "UVH")) {
		/* Only UV1 systems: */
		uv_system_type = UV_NON_UNIQUE_APIC;
		x86_platform.legacy.warm_reset = 0;
		__this_cpu_write(x2apic_extra_bits, pnodeid << uvh_apicid.s.pnode_shift);
		uv_set_apicid_hibit();
		uv_apic = 1;

	} else if (!strcmp(oem_table_id, "UVL")) {
		/* Only used for very small systems:  */
		uv_system_type = UV_LEGACY_APIC;
		uv_apic = 0;

	} else {
		goto badbios;
	}

	pr_info("UV: OEM IDs %s/%s, System/HUB Types %d/%d, uv_apic %d\n", oem_id, oem_table_id, uv_system_type, uv_min_hub_revision_id, uv_apic);
	uv_tsc_check_sync();

	return uv_apic;

badbios:
	pr_err("UV: OEM_ID:%s OEM_TABLE_ID:%s\n", oem_id, oem_table_id);
	pr_err("Current BIOS not supported, update kernel and/or BIOS\n");
	BUG();
=======
	/* Set hubbed type if true */
	uv_hub_info->hub_revision =
		!strncmp(uv_archtype, "SGI5", 4) ? UV5_HUB_REVISION_BASE :
		!strncmp(uv_archtype, "SGI4", 4) ? UV4_HUB_REVISION_BASE :
		!strncmp(uv_archtype, "SGI3", 4) ? UV3_HUB_REVISION_BASE :
		!strcmp(uv_archtype, "SGI2") ? UV2_HUB_REVISION_BASE : 0;

	switch (uv_hub_info->hub_revision) {
	case UV5_HUB_REVISION_BASE:
		uv_hubbed_system = 0x21;
		uv_hub_type_set(UV5);
		break;

	case UV4_HUB_REVISION_BASE:
		uv_hubbed_system = 0x11;
		uv_hub_type_set(UV4);
		break;

	case UV3_HUB_REVISION_BASE:
		uv_hubbed_system = 0x9;
		uv_hub_type_set(UV3);
		break;

	case UV2_HUB_REVISION_BASE:
		uv_hubbed_system = 0x5;
		uv_hub_type_set(UV2);
		break;

	default:
		return 0;
	}

	/* Get UV hub chip part number & revision */
	early_set_hub_type();

	/* Other UV setup functions */
	early_set_apic_mode();
	early_get_pnodeid();
	early_get_apic_socketid_shift();
	x86_platform.is_untracked_pat_range = uv_is_untracked_pat_range;
	x86_platform.nmi_init = uv_nmi_init;
	uv_tsc_check_sync();

	return 1;
}

/* Called early to probe for the correct APIC driver */
static int __init uv_acpi_madt_oem_check(char *_oem_id, char *_oem_table_id)
{
	/* Set up early hub info fields for Node 0 */
	uv_cpu_info->p_uv_hub_info = &uv_hub_info_node0;

	/* If not UV, return. */
	if (uv_set_system_type(_oem_id, _oem_table_id) == 0)
		return 0;

	/* Save for display of the OEM Table ID */
	uv_stringify(sizeof(oem_table_id), oem_table_id, _oem_table_id);

	pr_info("UV: OEM IDs %s/%s, System/UVType %d/0x%x, HUB RevID %d\n",
		oem_id, oem_table_id, uv_system_type, is_uv(UV_ANY),
		uv_min_hub_revision_id);

	return 0;
>>>>>>> upstream/android-13
}

enum uv_system_type get_uv_system_type(void)
{
	return uv_system_type;
}

<<<<<<< HEAD
=======
int uv_get_hubless_system(void)
{
	return uv_hubless_system;
}
EXPORT_SYMBOL_GPL(uv_get_hubless_system);

ssize_t uv_get_archtype(char *buf, int len)
{
	return scnprintf(buf, len, "%s/%s", uv_archtype, oem_table_id);
}
EXPORT_SYMBOL_GPL(uv_get_archtype);

>>>>>>> upstream/android-13
int is_uv_system(void)
{
	return uv_system_type != UV_NONE;
}
EXPORT_SYMBOL_GPL(is_uv_system);

<<<<<<< HEAD
int is_uv_hubless(void)
{
	return uv_hubless_system;
}
EXPORT_SYMBOL_GPL(is_uv_hubless);
=======
int is_uv_hubbed(int uvtype)
{
	return (uv_hubbed_system & uvtype);
}
EXPORT_SYMBOL_GPL(is_uv_hubbed);

static int is_uv_hubless(int uvtype)
{
	return (uv_hubless_system & uvtype);
}
>>>>>>> upstream/android-13

void **__uv_hub_info_list;
EXPORT_SYMBOL_GPL(__uv_hub_info_list);

DEFINE_PER_CPU(struct uv_cpu_info_s, __uv_cpu_info);
EXPORT_PER_CPU_SYMBOL_GPL(__uv_cpu_info);

short uv_possible_blades;
EXPORT_SYMBOL_GPL(uv_possible_blades);

unsigned long sn_rtc_cycles_per_second;
EXPORT_SYMBOL(sn_rtc_cycles_per_second);

/* The following values are used for the per node hub info struct */
static __initdata unsigned short		*_node_to_pnode;
static __initdata unsigned short		_min_socket, _max_socket;
static __initdata unsigned short		_min_pnode, _max_pnode, _gr_table_len;
static __initdata struct uv_gam_range_entry	*uv_gre_table;
static __initdata struct uv_gam_parameters	*uv_gp_table;
static __initdata unsigned short		*_socket_to_node;
static __initdata unsigned short		*_socket_to_pnode;
static __initdata unsigned short		*_pnode_to_socket;

static __initdata struct uv_gam_range_s		*_gr_table;

#define	SOCK_EMPTY	((unsigned short)~0)

<<<<<<< HEAD
extern int uv_hub_info_version(void)
{
	return UV_HUB_INFO_VERSION;
}
EXPORT_SYMBOL(uv_hub_info_version);

=======
>>>>>>> upstream/android-13
/* Default UV memory block size is 2GB */
static unsigned long mem_block_size __initdata = (2UL << 30);

/* Kernel parameter to specify UV mem block size */
static int __init parse_mem_block_size(char *ptr)
{
	unsigned long size = memparse(ptr, NULL);

	/* Size will be rounded down by set_block_size() below */
	mem_block_size = size;
	return 0;
}
early_param("uv_memblksize", parse_mem_block_size);

static __init int adj_blksize(u32 lgre)
{
	unsigned long base = (unsigned long)lgre << UV_GAM_RANGE_SHFT;
	unsigned long size;

	for (size = mem_block_size; size > MIN_MEMORY_BLOCK_SIZE; size >>= 1)
		if (IS_ALIGNED(base, size))
			break;

	if (size >= mem_block_size)
		return 0;

	mem_block_size = size;
	return 1;
}

static __init void set_block_size(void)
{
	unsigned int order = ffs(mem_block_size);

	if (order) {
		/* adjust for ffs return of 1..64 */
		set_memory_block_size_order(order - 1);
		pr_info("UV: mem_block_size set to 0x%lx\n", mem_block_size);
	} else {
		/* bad or zero value, default to 1UL << 31 (2GB) */
		pr_err("UV: mem_block_size error with 0x%lx\n", mem_block_size);
		set_memory_block_size_order(31);
	}
}

/* Build GAM range lookup table: */
static __init void build_uv_gr_table(void)
{
	struct uv_gam_range_entry *gre = uv_gre_table;
	struct uv_gam_range_s *grt;
	unsigned long last_limit = 0, ram_limit = 0;
	int bytes, i, sid, lsid = -1, indx = 0, lindx = -1;

	if (!gre)
		return;

	bytes = _gr_table_len * sizeof(struct uv_gam_range_s);
	grt = kzalloc(bytes, GFP_KERNEL);
	BUG_ON(!grt);
	_gr_table = grt;

	for (; gre->type != UV_GAM_RANGE_TYPE_UNUSED; gre++) {
		if (gre->type == UV_GAM_RANGE_TYPE_HOLE) {
			if (!ram_limit) {
				/* Mark hole between RAM/non-RAM: */
				ram_limit = last_limit;
				last_limit = gre->limit;
				lsid++;
				continue;
			}
			last_limit = gre->limit;
			pr_info("UV: extra hole in GAM RE table @%d\n", (int)(gre - uv_gre_table));
			continue;
		}
		if (_max_socket < gre->sockid) {
			pr_err("UV: GAM table sockid(%d) too large(>%d) @%d\n", gre->sockid, _max_socket, (int)(gre - uv_gre_table));
			continue;
		}
		sid = gre->sockid - _min_socket;
		if (lsid < sid) {
			/* New range: */
			grt = &_gr_table[indx];
			grt->base = lindx;
			grt->nasid = gre->nasid;
			grt->limit = last_limit = gre->limit;
			lsid = sid;
			lindx = indx++;
			continue;
		}
		/* Update range: */
		if (lsid == sid && !ram_limit) {
			/* .. if contiguous: */
			if (grt->limit == last_limit) {
				grt->limit = last_limit = gre->limit;
				continue;
			}
		}
		/* Non-contiguous RAM range: */
		if (!ram_limit) {
			grt++;
			grt->base = lindx;
			grt->nasid = gre->nasid;
			grt->limit = last_limit = gre->limit;
			continue;
		}
		/* Non-contiguous/non-RAM: */
		grt++;
		/* base is this entry */
		grt->base = grt - _gr_table;
		grt->nasid = gre->nasid;
		grt->limit = last_limit = gre->limit;
		lsid++;
	}

	/* Shorten table if possible */
	grt++;
	i = grt - _gr_table;
	if (i < _gr_table_len) {
		void *ret;

		bytes = i * sizeof(struct uv_gam_range_s);
		ret = krealloc(_gr_table, bytes, GFP_KERNEL);
		if (ret) {
			_gr_table = ret;
			_gr_table_len = i;
		}
	}

	/* Display resultant GAM range table: */
	for (i = 0, grt = _gr_table; i < _gr_table_len; i++, grt++) {
		unsigned long start, end;
		int gb = grt->base;

		start = gb < 0 ?  0 : (unsigned long)_gr_table[gb].limit << UV_GAM_RANGE_SHFT;
		end = (unsigned long)grt->limit << UV_GAM_RANGE_SHFT;

		pr_info("UV: GAM Range %2d %04x 0x%013lx-0x%013lx (%d)\n", i, grt->nasid, start, end, gb);
	}
}

static int uv_wakeup_secondary(int phys_apicid, unsigned long start_rip)
{
	unsigned long val;
	int pnode;

	pnode = uv_apicid_to_pnode(phys_apicid);
<<<<<<< HEAD
	phys_apicid |= uv_apicid_hibits;
=======
>>>>>>> upstream/android-13

	val = (1UL << UVH_IPI_INT_SEND_SHFT) |
	    (phys_apicid << UVH_IPI_INT_APIC_ID_SHFT) |
	    ((start_rip << UVH_IPI_INT_VECTOR_SHFT) >> 12) |
	    APIC_DM_INIT;

	uv_write_global_mmr64(pnode, UVH_IPI_INT, val);

	val = (1UL << UVH_IPI_INT_SEND_SHFT) |
	    (phys_apicid << UVH_IPI_INT_APIC_ID_SHFT) |
	    ((start_rip << UVH_IPI_INT_VECTOR_SHFT) >> 12) |
	    APIC_DM_STARTUP;

	uv_write_global_mmr64(pnode, UVH_IPI_INT, val);

	return 0;
}

static void uv_send_IPI_one(int cpu, int vector)
{
<<<<<<< HEAD
	unsigned long apicid;
	int pnode;

	apicid = per_cpu(x86_cpu_to_apicid, cpu);
	pnode = uv_apicid_to_pnode(apicid);
	uv_hub_send_ipi(pnode, apicid, vector);
=======
	unsigned long apicid = per_cpu(x86_cpu_to_apicid, cpu);
	int pnode = uv_apicid_to_pnode(apicid);
	unsigned long dmode, val;

	if (vector == NMI_VECTOR)
		dmode = APIC_DELIVERY_MODE_NMI;
	else
		dmode = APIC_DELIVERY_MODE_FIXED;

	val = (1UL << UVH_IPI_INT_SEND_SHFT) |
		(apicid << UVH_IPI_INT_APIC_ID_SHFT) |
		(dmode << UVH_IPI_INT_DELIVERY_MODE_SHFT) |
		(vector << UVH_IPI_INT_VECTOR_SHFT);

	uv_write_global_mmr64(pnode, UVH_IPI_INT, val);
>>>>>>> upstream/android-13
}

static void uv_send_IPI_mask(const struct cpumask *mask, int vector)
{
	unsigned int cpu;

	for_each_cpu(cpu, mask)
		uv_send_IPI_one(cpu, vector);
}

static void uv_send_IPI_mask_allbutself(const struct cpumask *mask, int vector)
{
	unsigned int this_cpu = smp_processor_id();
	unsigned int cpu;

	for_each_cpu(cpu, mask) {
		if (cpu != this_cpu)
			uv_send_IPI_one(cpu, vector);
	}
}

static void uv_send_IPI_allbutself(int vector)
{
	unsigned int this_cpu = smp_processor_id();
	unsigned int cpu;

	for_each_online_cpu(cpu) {
		if (cpu != this_cpu)
			uv_send_IPI_one(cpu, vector);
	}
}

static void uv_send_IPI_all(int vector)
{
	uv_send_IPI_mask(cpu_online_mask, vector);
}

static int uv_apic_id_valid(u32 apicid)
{
	return 1;
}

static int uv_apic_id_registered(void)
{
	return 1;
}

static void uv_init_apic_ldr(void)
{
}

static u32 apic_uv_calc_apicid(unsigned int cpu)
{
<<<<<<< HEAD
	return apic_default_calc_apicid(cpu) | uv_apicid_hibits;
}

static unsigned int x2apic_get_apic_id(unsigned long x)
{
	unsigned int id;

	WARN_ON(preemptible() && num_online_cpus() > 1);
	id = x | __this_cpu_read(x2apic_extra_bits);

=======
	return apic_default_calc_apicid(cpu);
}

static unsigned int x2apic_get_apic_id(unsigned long id)
{
>>>>>>> upstream/android-13
	return id;
}

static u32 set_apic_id(unsigned int id)
{
<<<<<<< HEAD
	/* CHECKME: Do we need to mask out the xapic extra bits? */
=======
>>>>>>> upstream/android-13
	return id;
}

static unsigned int uv_read_apic_id(void)
{
	return x2apic_get_apic_id(apic_read(APIC_ID));
}

static int uv_phys_pkg_id(int initial_apicid, int index_msb)
{
	return uv_read_apic_id() >> index_msb;
}

static void uv_send_IPI_self(int vector)
{
	apic_write(APIC_SELF_IPI, vector);
}

static int uv_probe(void)
{
	return apic == &apic_x2apic_uv_x;
}

static struct apic apic_x2apic_uv_x __ro_after_init = {

	.name				= "UV large system",
	.probe				= uv_probe,
	.acpi_madt_oem_check		= uv_acpi_madt_oem_check,
	.apic_id_valid			= uv_apic_id_valid,
	.apic_id_registered		= uv_apic_id_registered,

<<<<<<< HEAD
	.irq_delivery_mode		= dest_Fixed,
	.irq_dest_mode			= 0, /* Physical */

	.disable_esr			= 0,
	.dest_logical			= APIC_DEST_LOGICAL,
	.check_apicid_used		= NULL,

	.init_apic_ldr			= uv_init_apic_ldr,

=======
	.delivery_mode			= APIC_DELIVERY_MODE_FIXED,
	.dest_mode_logical		= false,

	.disable_esr			= 0,

	.check_apicid_used		= NULL,
	.init_apic_ldr			= uv_init_apic_ldr,
>>>>>>> upstream/android-13
	.ioapic_phys_id_map		= NULL,
	.setup_apic_routing		= NULL,
	.cpu_present_to_apicid		= default_cpu_present_to_apicid,
	.apicid_to_cpu_present		= NULL,
	.check_phys_apicid_present	= default_check_phys_apicid_present,
	.phys_pkg_id			= uv_phys_pkg_id,

	.get_apic_id			= x2apic_get_apic_id,
	.set_apic_id			= set_apic_id,

	.calc_dest_apicid		= apic_uv_calc_apicid,

	.send_IPI			= uv_send_IPI_one,
	.send_IPI_mask			= uv_send_IPI_mask,
	.send_IPI_mask_allbutself	= uv_send_IPI_mask_allbutself,
	.send_IPI_allbutself		= uv_send_IPI_allbutself,
	.send_IPI_all			= uv_send_IPI_all,
	.send_IPI_self			= uv_send_IPI_self,

	.wakeup_secondary_cpu		= uv_wakeup_secondary,
	.inquire_remote_apic		= NULL,

	.read				= native_apic_msr_read,
	.write				= native_apic_msr_write,
	.eoi_write			= native_apic_msr_eoi_write,
	.icr_read			= native_x2apic_icr_read,
	.icr_write			= native_x2apic_icr_write,
	.wait_icr_idle			= native_x2apic_wait_icr_idle,
	.safe_wait_icr_idle		= native_safe_x2apic_wait_icr_idle,
};

<<<<<<< HEAD
static void set_x2apic_extra_bits(int pnode)
{
	__this_cpu_write(x2apic_extra_bits, pnode << uvh_apicid.s.pnode_shift);
}

#define	UVH_RH_GAM_ALIAS210_REDIRECT_CONFIG_LENGTH	3
#define DEST_SHIFT UVH_RH_GAM_ALIAS210_REDIRECT_CONFIG_0_MMR_DEST_BASE_SHFT

static __init void get_lowmem_redirect(unsigned long *base, unsigned long *size)
{
	union uvh_rh_gam_alias210_overlay_config_2_mmr_u alias;
	union uvh_rh_gam_alias210_redirect_config_2_mmr_u redirect;
=======
#define	UVH_RH_GAM_ALIAS210_REDIRECT_CONFIG_LENGTH	3
#define DEST_SHIFT UVXH_RH_GAM_ALIAS_0_REDIRECT_CONFIG_DEST_BASE_SHFT

static __init void get_lowmem_redirect(unsigned long *base, unsigned long *size)
{
	union uvh_rh_gam_alias_2_overlay_config_u alias;
	union uvh_rh_gam_alias_2_redirect_config_u redirect;
>>>>>>> upstream/android-13
	unsigned long m_redirect;
	unsigned long m_overlay;
	int i;

	for (i = 0; i < UVH_RH_GAM_ALIAS210_REDIRECT_CONFIG_LENGTH; i++) {
		switch (i) {
		case 0:
<<<<<<< HEAD
			m_redirect = UVH_RH_GAM_ALIAS210_REDIRECT_CONFIG_0_MMR;
			m_overlay  = UVH_RH_GAM_ALIAS210_OVERLAY_CONFIG_0_MMR;
			break;
		case 1:
			m_redirect = UVH_RH_GAM_ALIAS210_REDIRECT_CONFIG_1_MMR;
			m_overlay  = UVH_RH_GAM_ALIAS210_OVERLAY_CONFIG_1_MMR;
			break;
		case 2:
			m_redirect = UVH_RH_GAM_ALIAS210_REDIRECT_CONFIG_2_MMR;
			m_overlay  = UVH_RH_GAM_ALIAS210_OVERLAY_CONFIG_2_MMR;
=======
			m_redirect = UVH_RH_GAM_ALIAS_0_REDIRECT_CONFIG;
			m_overlay  = UVH_RH_GAM_ALIAS_0_OVERLAY_CONFIG;
			break;
		case 1:
			m_redirect = UVH_RH_GAM_ALIAS_1_REDIRECT_CONFIG;
			m_overlay  = UVH_RH_GAM_ALIAS_1_OVERLAY_CONFIG;
			break;
		case 2:
			m_redirect = UVH_RH_GAM_ALIAS_2_REDIRECT_CONFIG;
			m_overlay  = UVH_RH_GAM_ALIAS_2_OVERLAY_CONFIG;
>>>>>>> upstream/android-13
			break;
		}
		alias.v = uv_read_local_mmr(m_overlay);
		if (alias.s.enable && alias.s.base == 0) {
			*size = (1UL << alias.s.m_alias);
			redirect.v = uv_read_local_mmr(m_redirect);
			*base = (unsigned long)redirect.s.dest_base << DEST_SHIFT;
			return;
		}
	}
	*base = *size = 0;
}

enum map_type {map_wb, map_uc};
<<<<<<< HEAD
=======
static const char * const mt[] = { "WB", "UC" };
>>>>>>> upstream/android-13

static __init void map_high(char *id, unsigned long base, int pshift, int bshift, int max_pnode, enum map_type map_type)
{
	unsigned long bytes, paddr;

	paddr = base << pshift;
	bytes = (1UL << bshift) * (max_pnode + 1);
	if (!paddr) {
		pr_info("UV: Map %s_HI base address NULL\n", id);
		return;
	}
<<<<<<< HEAD
	pr_debug("UV: Map %s_HI 0x%lx - 0x%lx\n", id, paddr, paddr + bytes);
=======
>>>>>>> upstream/android-13
	if (map_type == map_uc)
		init_extra_mapping_uc(paddr, bytes);
	else
		init_extra_mapping_wb(paddr, bytes);
<<<<<<< HEAD
}

static __init void map_gru_distributed(unsigned long c)
{
	union uvh_rh_gam_gru_overlay_config_mmr_u gru;
	u64 paddr;
	unsigned long bytes;
	int nid;

	gru.v = c;

	/* Only base bits 42:28 relevant in dist mode */
	gru_dist_base = gru.v & 0x000007fff0000000UL;
	if (!gru_dist_base) {
		pr_info("UV: Map GRU_DIST base address NULL\n");
		return;
	}

	bytes = 1UL << UVH_RH_GAM_GRU_OVERLAY_CONFIG_MMR_BASE_SHFT;
	gru_dist_lmask = ((1UL << uv_hub_info->m_val) - 1) & ~(bytes - 1);
	gru_dist_umask = ~((1UL << uv_hub_info->m_val) - 1);
	gru_dist_base &= gru_dist_lmask; /* Clear bits above M */

	for_each_online_node(nid) {
		paddr = ((u64)uv_node_to_pnode(nid) << uv_hub_info->m_val) |
				gru_dist_base;
		init_extra_mapping_wb(paddr, bytes);
		gru_first_node_paddr = min(paddr, gru_first_node_paddr);
		gru_last_node_paddr = max(paddr, gru_last_node_paddr);
	}

	/* Save upper (63:M) bits of address only for is_GRU_range */
	gru_first_node_paddr &= gru_dist_umask;
	gru_last_node_paddr &= gru_dist_umask;

	pr_debug("UV: Map GRU_DIST base 0x%016llx  0x%016llx - 0x%016llx\n", gru_dist_base, gru_first_node_paddr, gru_last_node_paddr);
=======

	pr_info("UV: Map %s_HI 0x%lx - 0x%lx %s (%d segments)\n",
		id, paddr, paddr + bytes, mt[map_type], max_pnode + 1);
>>>>>>> upstream/android-13
}

static __init void map_gru_high(int max_pnode)
{
<<<<<<< HEAD
	union uvh_rh_gam_gru_overlay_config_mmr_u gru;
	int shift = UVH_RH_GAM_GRU_OVERLAY_CONFIG_MMR_BASE_SHFT;
	unsigned long mask = UVH_RH_GAM_GRU_OVERLAY_CONFIG_MMR_BASE_MASK;
	unsigned long base;

	gru.v = uv_read_local_mmr(UVH_RH_GAM_GRU_OVERLAY_CONFIG_MMR);
	if (!gru.s.enable) {
		pr_info("UV: GRU disabled\n");
		return;
	}

	/* Only UV3 has distributed GRU mode */
	if (is_uv3_hub() && gru.s3.mode) {
		map_gru_distributed(gru.v);
=======
	union uvh_rh_gam_gru_overlay_config_u gru;
	unsigned long mask, base;
	int shift;

	if (UVH_RH_GAM_GRU_OVERLAY_CONFIG) {
		gru.v = uv_read_local_mmr(UVH_RH_GAM_GRU_OVERLAY_CONFIG);
		shift = UVH_RH_GAM_GRU_OVERLAY_CONFIG_BASE_SHFT;
		mask = UVH_RH_GAM_GRU_OVERLAY_CONFIG_BASE_MASK;
	} else if (UVH_RH10_GAM_GRU_OVERLAY_CONFIG) {
		gru.v = uv_read_local_mmr(UVH_RH10_GAM_GRU_OVERLAY_CONFIG);
		shift = UVH_RH10_GAM_GRU_OVERLAY_CONFIG_BASE_SHFT;
		mask = UVH_RH10_GAM_GRU_OVERLAY_CONFIG_BASE_MASK;
	} else {
		pr_err("UV: GRU unavailable (no MMR)\n");
		return;
	}

	if (!gru.s.enable) {
		pr_info("UV: GRU disabled (by BIOS)\n");
>>>>>>> upstream/android-13
		return;
	}

	base = (gru.v & mask) >> shift;
	map_high("GRU", base, shift, shift, max_pnode, map_wb);
	gru_start_paddr = ((u64)base << shift);
	gru_end_paddr = gru_start_paddr + (1UL << shift) * (max_pnode + 1);
}

static __init void map_mmr_high(int max_pnode)
{
<<<<<<< HEAD
	union uvh_rh_gam_mmr_overlay_config_mmr_u mmr;
	int shift = UVH_RH_GAM_MMR_OVERLAY_CONFIG_MMR_BASE_SHFT;

	mmr.v = uv_read_local_mmr(UVH_RH_GAM_MMR_OVERLAY_CONFIG_MMR);
	if (mmr.s.enable)
		map_high("MMR", mmr.s.base, shift, shift, max_pnode, map_uc);
=======
	unsigned long base;
	int shift;
	bool enable;

	if (UVH_RH10_GAM_MMR_OVERLAY_CONFIG) {
		union uvh_rh10_gam_mmr_overlay_config_u mmr;

		mmr.v = uv_read_local_mmr(UVH_RH10_GAM_MMR_OVERLAY_CONFIG);
		enable = mmr.s.enable;
		base = mmr.s.base;
		shift = UVH_RH10_GAM_MMR_OVERLAY_CONFIG_BASE_SHFT;
	} else if (UVH_RH_GAM_MMR_OVERLAY_CONFIG) {
		union uvh_rh_gam_mmr_overlay_config_u mmr;

		mmr.v = uv_read_local_mmr(UVH_RH_GAM_MMR_OVERLAY_CONFIG);
		enable = mmr.s.enable;
		base = mmr.s.base;
		shift = UVH_RH_GAM_MMR_OVERLAY_CONFIG_BASE_SHFT;
	} else {
		pr_err("UV:%s:RH_GAM_MMR_OVERLAY_CONFIG MMR undefined?\n",
			__func__);
		return;
	}

	if (enable)
		map_high("MMR", base, shift, shift, max_pnode, map_uc);
>>>>>>> upstream/android-13
	else
		pr_info("UV: MMR disabled\n");
}

<<<<<<< HEAD
/* UV3/4 have identical MMIOH overlay configs, UV4A is slightly different */
static __init void map_mmioh_high_uv34(int index, int min_pnode, int max_pnode)
{
	unsigned long overlay;
	unsigned long mmr;
	unsigned long base;
	unsigned long nasid_mask;
	unsigned long m_overlay;
	int i, n, shift, m_io, max_io;
	int nasid, lnasid, fi, li;
	char *id;

	if (index == 0) {
		id = "MMIOH0";
		m_overlay = UVH_RH_GAM_MMIOH_OVERLAY_CONFIG0_MMR;
		overlay = uv_read_local_mmr(m_overlay);
		base = overlay & UVH_RH_GAM_MMIOH_OVERLAY_CONFIG0_MMR_BASE_MASK;
		mmr = UVH_RH_GAM_MMIOH_REDIRECT_CONFIG0_MMR;
		m_io = (overlay & UVH_RH_GAM_MMIOH_OVERLAY_CONFIG0_MMR_M_IO_MASK)
			>> UVH_RH_GAM_MMIOH_OVERLAY_CONFIG0_MMR_M_IO_SHFT;
		shift = UVH_RH_GAM_MMIOH_OVERLAY_CONFIG0_MMR_M_IO_SHFT;
		n = UVH_RH_GAM_MMIOH_REDIRECT_CONFIG0_MMR_DEPTH;
		nasid_mask = UVH_RH_GAM_MMIOH_REDIRECT_CONFIG0_MMR_NASID_MASK;
	} else {
		id = "MMIOH1";
		m_overlay = UVH_RH_GAM_MMIOH_OVERLAY_CONFIG1_MMR;
		overlay = uv_read_local_mmr(m_overlay);
		base = overlay & UVH_RH_GAM_MMIOH_OVERLAY_CONFIG1_MMR_BASE_MASK;
		mmr = UVH_RH_GAM_MMIOH_REDIRECT_CONFIG1_MMR;
		m_io = (overlay & UVH_RH_GAM_MMIOH_OVERLAY_CONFIG1_MMR_M_IO_MASK)
			>> UVH_RH_GAM_MMIOH_OVERLAY_CONFIG1_MMR_M_IO_SHFT;
		shift = UVH_RH_GAM_MMIOH_OVERLAY_CONFIG1_MMR_M_IO_SHFT;
		n = UVH_RH_GAM_MMIOH_REDIRECT_CONFIG1_MMR_DEPTH;
		nasid_mask = UVH_RH_GAM_MMIOH_REDIRECT_CONFIG1_MMR_NASID_MASK;
	}
	pr_info("UV: %s overlay 0x%lx base:0x%lx m_io:%d\n", id, overlay, base, m_io);
	if (!(overlay & UVH_RH_GAM_MMIOH_OVERLAY_CONFIG0_MMR_ENABLE_MASK)) {
		pr_info("UV: %s disabled\n", id);
		return;
	}

	/* Convert to NASID: */
	min_pnode *= 2;
	max_pnode *= 2;
	max_io = lnasid = fi = li = -1;

=======
/* Arch specific ENUM cases */
enum mmioh_arch {
	UV2_MMIOH = -1,
	UVY_MMIOH0, UVY_MMIOH1,
	UVX_MMIOH0, UVX_MMIOH1,
};

/* Calculate and Map MMIOH Regions */
static void __init calc_mmioh_map(enum mmioh_arch index,
	int min_pnode, int max_pnode,
	int shift, unsigned long base, int m_io, int n_io)
{
	unsigned long mmr, nasid_mask;
	int nasid, min_nasid, max_nasid, lnasid, mapped;
	int i, fi, li, n, max_io;
	char id[8];

	/* One (UV2) mapping */
	if (index == UV2_MMIOH) {
		strncpy(id, "MMIOH", sizeof(id));
		max_io = max_pnode;
		mapped = 0;
		goto map_exit;
	}

	/* small and large MMIOH mappings */
	switch (index) {
	case UVY_MMIOH0:
		mmr = UVH_RH10_GAM_MMIOH_REDIRECT_CONFIG0;
		nasid_mask = UVH_RH10_GAM_MMIOH_OVERLAY_CONFIG0_BASE_MASK;
		n = UVH_RH10_GAM_MMIOH_REDIRECT_CONFIG0_DEPTH;
		min_nasid = min_pnode;
		max_nasid = max_pnode;
		mapped = 1;
		break;
	case UVY_MMIOH1:
		mmr = UVH_RH10_GAM_MMIOH_REDIRECT_CONFIG1;
		nasid_mask = UVH_RH10_GAM_MMIOH_OVERLAY_CONFIG1_BASE_MASK;
		n = UVH_RH10_GAM_MMIOH_REDIRECT_CONFIG1_DEPTH;
		min_nasid = min_pnode;
		max_nasid = max_pnode;
		mapped = 1;
		break;
	case UVX_MMIOH0:
		mmr = UVH_RH_GAM_MMIOH_REDIRECT_CONFIG0;
		nasid_mask = UVH_RH_GAM_MMIOH_OVERLAY_CONFIG0_BASE_MASK;
		n = UVH_RH_GAM_MMIOH_REDIRECT_CONFIG0_DEPTH;
		min_nasid = min_pnode * 2;
		max_nasid = max_pnode * 2;
		mapped = 1;
		break;
	case UVX_MMIOH1:
		mmr = UVH_RH_GAM_MMIOH_REDIRECT_CONFIG1;
		nasid_mask = UVH_RH_GAM_MMIOH_OVERLAY_CONFIG1_BASE_MASK;
		n = UVH_RH_GAM_MMIOH_REDIRECT_CONFIG1_DEPTH;
		min_nasid = min_pnode * 2;
		max_nasid = max_pnode * 2;
		mapped = 1;
		break;
	default:
		pr_err("UV:%s:Invalid mapping type:%d\n", __func__, index);
		return;
	}

	/* enum values chosen so (index mod 2) is MMIOH 0/1 (low/high) */
	snprintf(id, sizeof(id), "MMIOH%d", index%2);

	max_io = lnasid = fi = li = -1;
>>>>>>> upstream/android-13
	for (i = 0; i < n; i++) {
		unsigned long m_redirect = mmr + i * 8;
		unsigned long redirect = uv_read_local_mmr(m_redirect);

		nasid = redirect & nasid_mask;
		if (i == 0)
			pr_info("UV: %s redirect base 0x%lx(@0x%lx) 0x%04x\n",
				id, redirect, m_redirect, nasid);

<<<<<<< HEAD
		/* Invalid NASID: */
		if (nasid < min_pnode || max_pnode < nasid)
			nasid = -1;
=======
		/* Invalid NASID check */
		if (nasid < min_nasid || max_nasid < nasid) {
			pr_err("UV:%s:Invalid NASID:%x (range:%x..%x)\n",
				__func__, index, min_nasid, max_nasid);
			nasid = -1;
		}
>>>>>>> upstream/android-13

		if (nasid == lnasid) {
			li = i;
			/* Last entry check: */
			if (i != n-1)
				continue;
		}

		/* Check if we have a cached (or last) redirect to print: */
		if (lnasid != -1 || (i == n-1 && nasid != -1))  {
			unsigned long addr1, addr2;
			int f, l;

			if (lnasid == -1) {
				f = l = i;
				lnasid = nasid;
			} else {
				f = fi;
				l = li;
			}
			addr1 = (base << shift) + f * (1ULL << m_io);
			addr2 = (base << shift) + (l + 1) * (1ULL << m_io);
<<<<<<< HEAD
			pr_info("UV: %s[%03d..%03d] NASID 0x%04x ADDR 0x%016lx - 0x%016lx\n", id, fi, li, lnasid, addr1, addr2);
=======
			pr_info("UV: %s[%03d..%03d] NASID 0x%04x ADDR 0x%016lx - 0x%016lx\n",
				id, fi, li, lnasid, addr1, addr2);
>>>>>>> upstream/android-13
			if (max_io < l)
				max_io = l;
		}
		fi = li = i;
		lnasid = nasid;
	}

<<<<<<< HEAD
	pr_info("UV: %s base:0x%lx shift:%d M_IO:%d MAX_IO:%d\n", id, base, shift, m_io, max_io);

	if (max_io >= 0)
=======
map_exit:
	pr_info("UV: %s base:0x%lx shift:%d m_io:%d max_io:%d max_pnode:0x%x\n",
		id, base, shift, m_io, max_io, max_pnode);

	if (max_io >= 0 && !mapped)
>>>>>>> upstream/android-13
		map_high(id, base, shift, m_io, max_io, map_uc);
}

static __init void map_mmioh_high(int min_pnode, int max_pnode)
{
<<<<<<< HEAD
	union uvh_rh_gam_mmioh_overlay_config_mmr_u mmioh;
	unsigned long mmr, base;
	int shift, enable, m_io, n_io;

	if (is_uv3_hub() || is_uv4_hub()) {
		/* Map both MMIOH regions: */
		map_mmioh_high_uv34(0, min_pnode, max_pnode);
		map_mmioh_high_uv34(1, min_pnode, max_pnode);
		return;
	}

	if (is_uv1_hub()) {
		mmr	= UV1H_RH_GAM_MMIOH_OVERLAY_CONFIG_MMR;
		shift	= UV1H_RH_GAM_MMIOH_OVERLAY_CONFIG_MMR_BASE_SHFT;
		mmioh.v	= uv_read_local_mmr(mmr);
		enable	= !!mmioh.s1.enable;
		base	= mmioh.s1.base;
		m_io	= mmioh.s1.m_io;
		n_io	= mmioh.s1.n_io;
	} else if (is_uv2_hub()) {
		mmr	= UV2H_RH_GAM_MMIOH_OVERLAY_CONFIG_MMR;
		shift	= UV2H_RH_GAM_MMIOH_OVERLAY_CONFIG_MMR_BASE_SHFT;
		mmioh.v	= uv_read_local_mmr(mmr);
		enable	= !!mmioh.s2.enable;
		base	= mmioh.s2.base;
		m_io	= mmioh.s2.m_io;
		n_io	= mmioh.s2.n_io;
	} else {
		return;
	}

	if (enable) {
		max_pnode &= (1 << n_io) - 1;
		pr_info("UV: base:0x%lx shift:%d N_IO:%d M_IO:%d max_pnode:0x%x\n", base, shift, m_io, n_io, max_pnode);
		map_high("MMIOH", base, shift, m_io, max_pnode, map_uc);
	} else {
		pr_info("UV: MMIOH disabled\n");
=======
	/* UVY flavor */
	if (UVH_RH10_GAM_MMIOH_OVERLAY_CONFIG0) {
		union uvh_rh10_gam_mmioh_overlay_config0_u mmioh0;
		union uvh_rh10_gam_mmioh_overlay_config1_u mmioh1;

		mmioh0.v = uv_read_local_mmr(UVH_RH10_GAM_MMIOH_OVERLAY_CONFIG0);
		if (unlikely(mmioh0.s.enable == 0))
			pr_info("UV: MMIOH0 disabled\n");
		else
			calc_mmioh_map(UVY_MMIOH0, min_pnode, max_pnode,
				UVH_RH10_GAM_MMIOH_OVERLAY_CONFIG0_BASE_SHFT,
				mmioh0.s.base, mmioh0.s.m_io, mmioh0.s.n_io);

		mmioh1.v = uv_read_local_mmr(UVH_RH10_GAM_MMIOH_OVERLAY_CONFIG1);
		if (unlikely(mmioh1.s.enable == 0))
			pr_info("UV: MMIOH1 disabled\n");
		else
			calc_mmioh_map(UVY_MMIOH1, min_pnode, max_pnode,
				UVH_RH10_GAM_MMIOH_OVERLAY_CONFIG1_BASE_SHFT,
				mmioh1.s.base, mmioh1.s.m_io, mmioh1.s.n_io);
		return;
	}
	/* UVX flavor */
	if (UVH_RH_GAM_MMIOH_OVERLAY_CONFIG0) {
		union uvh_rh_gam_mmioh_overlay_config0_u mmioh0;
		union uvh_rh_gam_mmioh_overlay_config1_u mmioh1;

		mmioh0.v = uv_read_local_mmr(UVH_RH_GAM_MMIOH_OVERLAY_CONFIG0);
		if (unlikely(mmioh0.s.enable == 0))
			pr_info("UV: MMIOH0 disabled\n");
		else {
			unsigned long base = uvxy_field(mmioh0, base, 0);
			int m_io = uvxy_field(mmioh0, m_io, 0);
			int n_io = uvxy_field(mmioh0, n_io, 0);

			calc_mmioh_map(UVX_MMIOH0, min_pnode, max_pnode,
				UVH_RH_GAM_MMIOH_OVERLAY_CONFIG0_BASE_SHFT,
				base, m_io, n_io);
		}

		mmioh1.v = uv_read_local_mmr(UVH_RH_GAM_MMIOH_OVERLAY_CONFIG1);
		if (unlikely(mmioh1.s.enable == 0))
			pr_info("UV: MMIOH1 disabled\n");
		else {
			unsigned long base = uvxy_field(mmioh1, base, 0);
			int m_io = uvxy_field(mmioh1, m_io, 0);
			int n_io = uvxy_field(mmioh1, n_io, 0);

			calc_mmioh_map(UVX_MMIOH1, min_pnode, max_pnode,
				UVH_RH_GAM_MMIOH_OVERLAY_CONFIG1_BASE_SHFT,
				base, m_io, n_io);
		}
		return;
	}

	/* UV2 flavor */
	if (UVH_RH_GAM_MMIOH_OVERLAY_CONFIG) {
		union uvh_rh_gam_mmioh_overlay_config_u mmioh;

		mmioh.v	= uv_read_local_mmr(UVH_RH_GAM_MMIOH_OVERLAY_CONFIG);
		if (unlikely(mmioh.s2.enable == 0))
			pr_info("UV: MMIOH disabled\n");
		else
			calc_mmioh_map(UV2_MMIOH, min_pnode, max_pnode,
				UV2H_RH_GAM_MMIOH_OVERLAY_CONFIG_BASE_SHFT,
				mmioh.s2.base, mmioh.s2.m_io, mmioh.s2.n_io);
		return;
>>>>>>> upstream/android-13
	}
}

static __init void map_low_mmrs(void)
{
<<<<<<< HEAD
	init_extra_mapping_uc(UV_GLOBAL_MMR32_BASE, UV_GLOBAL_MMR32_SIZE);
	init_extra_mapping_uc(UV_LOCAL_MMR_BASE, UV_LOCAL_MMR_SIZE);
=======
	if (UV_GLOBAL_MMR32_BASE)
		init_extra_mapping_uc(UV_GLOBAL_MMR32_BASE, UV_GLOBAL_MMR32_SIZE);

	if (UV_LOCAL_MMR_BASE)
		init_extra_mapping_uc(UV_LOCAL_MMR_BASE, UV_LOCAL_MMR_SIZE);
>>>>>>> upstream/android-13
}

static __init void uv_rtc_init(void)
{
	long status;
	u64 ticks_per_sec;

	status = uv_bios_freq_base(BIOS_FREQ_BASE_REALTIME_CLOCK, &ticks_per_sec);

	if (status != BIOS_STATUS_SUCCESS || ticks_per_sec < 100000) {
		pr_warn("UV: unable to determine platform RTC clock frequency, guessing.\n");

		/* BIOS gives wrong value for clock frequency, so guess: */
		sn_rtc_cycles_per_second = 1000000000000UL / 30000UL;
	} else {
		sn_rtc_cycles_per_second = ticks_per_sec;
	}
}

<<<<<<< HEAD
/*
 * percpu heartbeat timer
 */
static void uv_heartbeat(struct timer_list *timer)
{
	unsigned char bits = uv_scir_info->state;

	/* Flip heartbeat bit: */
	bits ^= SCIR_CPU_HEARTBEAT;

	/* Is this CPU idle? */
	if (idle_cpu(raw_smp_processor_id()))
		bits &= ~SCIR_CPU_ACTIVITY;
	else
		bits |= SCIR_CPU_ACTIVITY;

	/* Update system controller interface reg: */
	uv_set_scir_bits(bits);

	/* Enable next timer period: */
	mod_timer(timer, jiffies + SCIR_CPU_HB_INTERVAL);
}

static int uv_heartbeat_enable(unsigned int cpu)
{
	while (!uv_cpu_scir_info(cpu)->enabled) {
		struct timer_list *timer = &uv_cpu_scir_info(cpu)->timer;

		uv_set_cpu_scir_bits(cpu, SCIR_CPU_HEARTBEAT|SCIR_CPU_ACTIVITY);
		timer_setup(timer, uv_heartbeat, TIMER_PINNED);
		timer->expires = jiffies + SCIR_CPU_HB_INTERVAL;
		add_timer_on(timer, cpu);
		uv_cpu_scir_info(cpu)->enabled = 1;

		/* Also ensure that boot CPU is enabled: */
		cpu = 0;
	}
	return 0;
}

#ifdef CONFIG_HOTPLUG_CPU
static int uv_heartbeat_disable(unsigned int cpu)
{
	if (uv_cpu_scir_info(cpu)->enabled) {
		uv_cpu_scir_info(cpu)->enabled = 0;
		del_timer(&uv_cpu_scir_info(cpu)->timer);
	}
	uv_set_cpu_scir_bits(cpu, 0xff);
	return 0;
}

static __init void uv_scir_register_cpu_notifier(void)
{
	cpuhp_setup_state_nocalls(CPUHP_AP_ONLINE_DYN, "x86/x2apic-uvx:online",
				  uv_heartbeat_enable, uv_heartbeat_disable);
}

#else /* !CONFIG_HOTPLUG_CPU */

static __init void uv_scir_register_cpu_notifier(void)
{
}

static __init int uv_init_heartbeat(void)
{
	int cpu;

	if (is_uv_system()) {
		for_each_online_cpu(cpu)
			uv_heartbeat_enable(cpu);
	}

	return 0;
}

late_initcall(uv_init_heartbeat);

#endif /* !CONFIG_HOTPLUG_CPU */

/* Direct Legacy VGA I/O traffic to designated IOH */
int uv_set_vga_state(struct pci_dev *pdev, bool decode, unsigned int command_bits, u32 flags)
=======
/* Direct Legacy VGA I/O traffic to designated IOH */
static int uv_set_vga_state(struct pci_dev *pdev, bool decode, unsigned int command_bits, u32 flags)
>>>>>>> upstream/android-13
{
	int domain, bus, rc;

	if (!(flags & PCI_VGA_STATE_CHANGE_BRIDGE))
		return 0;

	if ((command_bits & PCI_COMMAND_IO) == 0)
		return 0;

	domain = pci_domain_nr(pdev->bus);
	bus = pdev->bus->number;

	rc = uv_bios_set_legacy_vga_target(decode, domain, bus);

	return rc;
}

/*
 * Called on each CPU to initialize the per_cpu UV data area.
 * FIXME: hotplug not supported yet
 */
void uv_cpu_init(void)
{
	/* CPU 0 initialization will be done via uv_system_init. */
	if (smp_processor_id() == 0)
		return;

	uv_hub_info->nr_online_cpus++;
<<<<<<< HEAD

	if (get_uv_system_type() == UV_NON_UNIQUE_APIC)
		set_x2apic_extra_bits(uv_hub_info->pnode);
=======
>>>>>>> upstream/android-13
}

struct mn {
	unsigned char	m_val;
	unsigned char	n_val;
	unsigned char	m_shift;
	unsigned char	n_lshift;
};

<<<<<<< HEAD
static void get_mn(struct mn *mnp)
{
	union uvh_rh_gam_config_mmr_u m_n_config;
	union uv3h_gr0_gam_gr_config_u m_gr_config;

	/* Make sure the whole structure is well initialized: */
	memset(mnp, 0, sizeof(*mnp));

	m_n_config.v	= uv_read_local_mmr(UVH_RH_GAM_CONFIG_MMR);
	mnp->n_val	= m_n_config.s.n_skt;

	if (is_uv4_hub()) {
		mnp->m_val	= 0;
		mnp->n_lshift	= 0;
	} else if (is_uv3_hub()) {
		mnp->m_val	= m_n_config.s3.m_skt;
		m_gr_config.v	= uv_read_local_mmr(UV3H_GR0_GAM_GR_CONFIG);
		mnp->n_lshift	= m_gr_config.s3.m_skt;
	} else if (is_uv2_hub()) {
		mnp->m_val	= m_n_config.s2.m_skt;
		mnp->n_lshift	= mnp->m_val == 40 ? 40 : 39;
	} else if (is_uv1_hub()) {
		mnp->m_val	= m_n_config.s1.m_skt;
		mnp->n_lshift	= mnp->m_val;
=======
/* Initialize caller's MN struct and fill in values */
static void get_mn(struct mn *mnp)
{
	memset(mnp, 0, sizeof(*mnp));
	mnp->n_val	= uv_cpuid.n_skt;
	if (is_uv(UV4|UVY)) {
		mnp->m_val	= 0;
		mnp->n_lshift	= 0;
	} else if (is_uv3_hub()) {
		union uvyh_gr0_gam_gr_config_u m_gr_config;

		mnp->m_val	= uv_cpuid.m_skt;
		m_gr_config.v	= uv_read_local_mmr(UVH_GR0_GAM_GR_CONFIG);
		mnp->n_lshift	= m_gr_config.s3.m_skt;
	} else if (is_uv2_hub()) {
		mnp->m_val	= uv_cpuid.m_skt;
		mnp->n_lshift	= mnp->m_val == 40 ? 40 : 39;
>>>>>>> upstream/android-13
	}
	mnp->m_shift = mnp->m_val ? 64 - mnp->m_val : 0;
}

<<<<<<< HEAD
void __init uv_init_hub_info(struct uv_hub_info_s *hi)
{
	union uvh_node_id_u node_id;
=======
static void __init uv_init_hub_info(struct uv_hub_info_s *hi)
{
>>>>>>> upstream/android-13
	struct mn mn;

	get_mn(&mn);
	hi->gpa_mask = mn.m_val ?
		(1UL << (mn.m_val + mn.n_val)) - 1 :
		(1UL << uv_cpuid.gpa_shift) - 1;

	hi->m_val		= mn.m_val;
	hi->n_val		= mn.n_val;
	hi->m_shift		= mn.m_shift;
	hi->n_lshift		= mn.n_lshift ? mn.n_lshift : 0;
	hi->hub_revision	= uv_hub_info->hub_revision;
<<<<<<< HEAD
	hi->pnode_mask		= uv_cpuid.pnode_mask;
=======
	hi->hub_type		= uv_hub_info->hub_type;
	hi->pnode_mask		= uv_cpuid.pnode_mask;
	hi->nasid_shift		= uv_cpuid.nasid_shift;
>>>>>>> upstream/android-13
	hi->min_pnode		= _min_pnode;
	hi->min_socket		= _min_socket;
	hi->pnode_to_socket	= _pnode_to_socket;
	hi->socket_to_node	= _socket_to_node;
	hi->socket_to_pnode	= _socket_to_pnode;
	hi->gr_table_len	= _gr_table_len;
	hi->gr_table		= _gr_table;

<<<<<<< HEAD
	node_id.v		= uv_read_local_mmr(UVH_NODE_ID);
	uv_cpuid.gnode_shift	= max_t(unsigned int, uv_cpuid.gnode_shift, mn.n_val);
	hi->gnode_extra		= (node_id.s.node_id & ~((1 << uv_cpuid.gnode_shift) - 1)) >> 1;
=======
	uv_cpuid.gnode_shift	= max_t(unsigned int, uv_cpuid.gnode_shift, mn.n_val);
	hi->gnode_extra		= (uv_node_id & ~((1 << uv_cpuid.gnode_shift) - 1)) >> 1;
>>>>>>> upstream/android-13
	if (mn.m_val)
		hi->gnode_upper	= (u64)hi->gnode_extra << mn.m_val;

	if (uv_gp_table) {
		hi->global_mmr_base	= uv_gp_table->mmr_base;
		hi->global_mmr_shift	= uv_gp_table->mmr_shift;
		hi->global_gru_base	= uv_gp_table->gru_base;
		hi->global_gru_shift	= uv_gp_table->gru_shift;
		hi->gpa_shift		= uv_gp_table->gpa_shift;
		hi->gpa_mask		= (1UL << hi->gpa_shift) - 1;
	} else {
<<<<<<< HEAD
		hi->global_mmr_base	= uv_read_local_mmr(UVH_RH_GAM_MMR_OVERLAY_CONFIG_MMR) & ~UV_MMR_ENABLE;
=======
		hi->global_mmr_base	=
			uv_read_local_mmr(UVH_RH_GAM_MMR_OVERLAY_CONFIG) &
			~UV_MMR_ENABLE;
>>>>>>> upstream/android-13
		hi->global_mmr_shift	= _UV_GLOBAL_MMR64_PNODE_SHIFT;
	}

	get_lowmem_redirect(&hi->lowmem_remap_base, &hi->lowmem_remap_top);

	hi->apic_pnode_shift = uv_cpuid.socketid_shift;

	/* Show system specific info: */
	pr_info("UV: N:%d M:%d m_shift:%d n_lshift:%d\n", hi->n_val, hi->m_val, hi->m_shift, hi->n_lshift);
	pr_info("UV: gpa_mask/shift:0x%lx/%d pnode_mask:0x%x apic_pns:%d\n", hi->gpa_mask, hi->gpa_shift, hi->pnode_mask, hi->apic_pnode_shift);
<<<<<<< HEAD
	pr_info("UV: mmr_base/shift:0x%lx/%ld gru_base/shift:0x%lx/%ld\n", hi->global_mmr_base, hi->global_mmr_shift, hi->global_gru_base, hi->global_gru_shift);
=======
	pr_info("UV: mmr_base/shift:0x%lx/%ld\n", hi->global_mmr_base, hi->global_mmr_shift);
	if (hi->global_gru_base)
		pr_info("UV: gru_base/shift:0x%lx/%ld\n",
			hi->global_gru_base, hi->global_gru_shift);

>>>>>>> upstream/android-13
	pr_info("UV: gnode_upper:0x%lx gnode_extra:0x%x\n", hi->gnode_upper, hi->gnode_extra);
}

static void __init decode_gam_params(unsigned long ptr)
{
	uv_gp_table = (struct uv_gam_parameters *)ptr;

	pr_info("UV: GAM Params...\n");
	pr_info("UV: mmr_base/shift:0x%llx/%d gru_base/shift:0x%llx/%d gpa_shift:%d\n",
		uv_gp_table->mmr_base, uv_gp_table->mmr_shift,
		uv_gp_table->gru_base, uv_gp_table->gru_shift,
		uv_gp_table->gpa_shift);
}

static void __init decode_gam_rng_tbl(unsigned long ptr)
{
	struct uv_gam_range_entry *gre = (struct uv_gam_range_entry *)ptr;
	unsigned long lgre = 0;
	int index = 0;
	int sock_min = 999999, pnode_min = 99999;
	int sock_max = -1, pnode_max = -1;

	uv_gre_table = gre;
	for (; gre->type != UV_GAM_RANGE_TYPE_UNUSED; gre++) {
		unsigned long size = ((unsigned long)(gre->limit - lgre)
					<< UV_GAM_RANGE_SHFT);
		int order = 0;
		char suffix[] = " KMGTPE";
		int flag = ' ';

		while (size > 9999 && order < sizeof(suffix)) {
			size /= 1024;
			order++;
		}

		/* adjust max block size to current range start */
		if (gre->type == 1 || gre->type == 2)
			if (adj_blksize(lgre))
				flag = '*';

		if (!index) {
			pr_info("UV: GAM Range Table...\n");
			pr_info("UV:  # %20s %14s %6s %4s %5s %3s %2s\n", "Range", "", "Size", "Type", "NASID", "SID", "PN");
		}
		pr_info("UV: %2d: 0x%014lx-0x%014lx%c %5lu%c %3d   %04x  %02x %02x\n",
			index++,
			(unsigned long)lgre << UV_GAM_RANGE_SHFT,
			(unsigned long)gre->limit << UV_GAM_RANGE_SHFT,
			flag, size, suffix[order],
			gre->type, gre->nasid, gre->sockid, gre->pnode);

		/* update to next range start */
		lgre = gre->limit;
		if (sock_min > gre->sockid)
			sock_min = gre->sockid;
		if (sock_max < gre->sockid)
			sock_max = gre->sockid;
		if (pnode_min > gre->pnode)
			pnode_min = gre->pnode;
		if (pnode_max < gre->pnode)
			pnode_max = gre->pnode;
	}
	_min_socket	= sock_min;
	_max_socket	= sock_max;
	_min_pnode	= pnode_min;
	_max_pnode	= pnode_max;
	_gr_table_len	= index;

	pr_info("UV: GRT: %d entries, sockets(min:%x,max:%x) pnodes(min:%x,max:%x)\n", index, _min_socket, _max_socket, _min_pnode, _max_pnode);
}

<<<<<<< HEAD
=======
/* Walk through UVsystab decoding the fields */
>>>>>>> upstream/android-13
static int __init decode_uv_systab(void)
{
	struct uv_systab *st;
	int i;

<<<<<<< HEAD
	if (uv_hub_info->hub_revision < UV4_HUB_REVISION_BASE)
		return 0;	/* No extended UVsystab required */

	st = uv_systab;
	if ((!st) || (st->revision < UV_SYSTAB_VERSION_UV4_LATEST)) {
		int rev = st ? st->revision : 0;

		pr_err("UV: BIOS UVsystab version(%x) mismatch, expecting(%x)\n", rev, UV_SYSTAB_VERSION_UV4_LATEST);
		pr_err("UV: Cannot support UV operations, switching to generic PC\n");
=======
	/* Get mapped UVsystab pointer */
	st = uv_systab;

	/* If UVsystab is version 1, there is no extended UVsystab */
	if (st && st->revision == UV_SYSTAB_VERSION_1)
		return 0;

	if ((!st) || (st->revision < UV_SYSTAB_VERSION_UV4_LATEST)) {
		int rev = st ? st->revision : 0;

		pr_err("UV: BIOS UVsystab mismatch, (%x < %x)\n",
			rev, UV_SYSTAB_VERSION_UV4_LATEST);
		pr_err("UV: Does not support UV, switch to non-UV x86_64\n");
>>>>>>> upstream/android-13
		uv_system_type = UV_NONE;

		return -EINVAL;
	}

	for (i = 0; st->entry[i].type != UV_SYSTAB_TYPE_UNUSED; i++) {
		unsigned long ptr = st->entry[i].offset;

		if (!ptr)
			continue;

<<<<<<< HEAD
		ptr = ptr + (unsigned long)st;
=======
		/* point to payload */
		ptr += (unsigned long)st;
>>>>>>> upstream/android-13

		switch (st->entry[i].type) {
		case UV_SYSTAB_TYPE_GAM_PARAMS:
			decode_gam_params(ptr);
			break;

		case UV_SYSTAB_TYPE_GAM_RNG_TBL:
			decode_gam_rng_tbl(ptr);
			break;
<<<<<<< HEAD
=======

		case UV_SYSTAB_TYPE_ARCH_TYPE:
			/* already processed in early startup */
			break;

		default:
			pr_err("UV:%s:Unrecognized UV_SYSTAB_TYPE:%d, skipped\n",
				__func__, st->entry[i].type);
			break;
>>>>>>> upstream/android-13
		}
	}
	return 0;
}

<<<<<<< HEAD
/*
 * Set up physical blade translations from UVH_NODE_PRESENT_TABLE
 * .. NB: UVH_NODE_PRESENT_TABLE is going away,
 * .. being replaced by GAM Range Table
 */
static __init void boot_init_possible_blades(struct uv_hub_info_s *hub_info)
{
	int i, uv_pb = 0;

	pr_info("UV: NODE_PRESENT_DEPTH = %d\n", UVH_NODE_PRESENT_TABLE_DEPTH);
	for (i = 0; i < UVH_NODE_PRESENT_TABLE_DEPTH; i++) {
		unsigned long np;

		np = uv_read_local_mmr(UVH_NODE_PRESENT_TABLE + i * 8);
		if (np)
			pr_info("UV: NODE_PRESENT(%d) = 0x%016lx\n", i, np);

=======
/* Set up physical blade translations from UVH_NODE_PRESENT_TABLE */
static __init void boot_init_possible_blades(struct uv_hub_info_s *hub_info)
{
	unsigned long np;
	int i, uv_pb = 0;

	if (UVH_NODE_PRESENT_TABLE) {
		pr_info("UV: NODE_PRESENT_DEPTH = %d\n",
			UVH_NODE_PRESENT_TABLE_DEPTH);
		for (i = 0; i < UVH_NODE_PRESENT_TABLE_DEPTH; i++) {
			np = uv_read_local_mmr(UVH_NODE_PRESENT_TABLE + i * 8);
			pr_info("UV: NODE_PRESENT(%d) = 0x%016lx\n", i, np);
			uv_pb += hweight64(np);
		}
	}
	if (UVH_NODE_PRESENT_0) {
		np = uv_read_local_mmr(UVH_NODE_PRESENT_0);
		pr_info("UV: NODE_PRESENT_0 = 0x%016lx\n", np);
		uv_pb += hweight64(np);
	}
	if (UVH_NODE_PRESENT_1) {
		np = uv_read_local_mmr(UVH_NODE_PRESENT_1);
		pr_info("UV: NODE_PRESENT_1 = 0x%016lx\n", np);
>>>>>>> upstream/android-13
		uv_pb += hweight64(np);
	}
	if (uv_possible_blades != uv_pb)
		uv_possible_blades = uv_pb;
<<<<<<< HEAD
=======

	pr_info("UV: number nodes/possible blades %d\n", uv_pb);
>>>>>>> upstream/android-13
}

static void __init build_socket_tables(void)
{
	struct uv_gam_range_entry *gre = uv_gre_table;
	int num, nump;
	int cpu, i, lnid;
	int minsock = _min_socket;
	int maxsock = _max_socket;
	int minpnode = _min_pnode;
	int maxpnode = _max_pnode;
	size_t bytes;

	if (!gre) {
<<<<<<< HEAD
		if (is_uv1_hub() || is_uv2_hub() || is_uv3_hub()) {
			pr_info("UV: No UVsystab socket table, ignoring\n");
			return;
		}
		pr_crit("UV: Error: UVsystab address translations not available!\n");
=======
		if (is_uv2_hub() || is_uv3_hub()) {
			pr_info("UV: No UVsystab socket table, ignoring\n");
			return;
		}
		pr_err("UV: Error: UVsystab address translations not available!\n");
>>>>>>> upstream/android-13
		BUG();
	}

	/* Build socket id -> node id, pnode */
	num = maxsock - minsock + 1;
	bytes = num * sizeof(_socket_to_node[0]);
	_socket_to_node = kmalloc(bytes, GFP_KERNEL);
	_socket_to_pnode = kmalloc(bytes, GFP_KERNEL);

	nump = maxpnode - minpnode + 1;
	bytes = nump * sizeof(_pnode_to_socket[0]);
	_pnode_to_socket = kmalloc(bytes, GFP_KERNEL);
	BUG_ON(!_socket_to_node || !_socket_to_pnode || !_pnode_to_socket);

	for (i = 0; i < num; i++)
		_socket_to_node[i] = _socket_to_pnode[i] = SOCK_EMPTY;

	for (i = 0; i < nump; i++)
		_pnode_to_socket[i] = SOCK_EMPTY;

	/* Fill in pnode/node/addr conversion list values: */
	pr_info("UV: GAM Building socket/pnode conversion tables\n");
	for (; gre->type != UV_GAM_RANGE_TYPE_UNUSED; gre++) {
		if (gre->type == UV_GAM_RANGE_TYPE_HOLE)
			continue;
		i = gre->sockid - minsock;
		/* Duplicate: */
		if (_socket_to_pnode[i] != SOCK_EMPTY)
			continue;
		_socket_to_pnode[i] = gre->pnode;

		i = gre->pnode - minpnode;
		_pnode_to_socket[i] = gre->sockid;

		pr_info("UV: sid:%02x type:%d nasid:%04x pn:%02x pn2s:%2x\n",
			gre->sockid, gre->type, gre->nasid,
			_socket_to_pnode[gre->sockid - minsock],
			_pnode_to_socket[gre->pnode - minpnode]);
	}

	/* Set socket -> node values: */
<<<<<<< HEAD
	lnid = -1;
=======
	lnid = NUMA_NO_NODE;
>>>>>>> upstream/android-13
	for_each_present_cpu(cpu) {
		int nid = cpu_to_node(cpu);
		int apicid, sockid;

		if (lnid == nid)
			continue;
		lnid = nid;
		apicid = per_cpu(x86_cpu_to_apicid, cpu);
		sockid = apicid >> uv_cpuid.socketid_shift;
		_socket_to_node[sockid - minsock] = nid;
		pr_info("UV: sid:%02x: apicid:%04x node:%2d\n",
			sockid, apicid, nid);
	}

	/* Set up physical blade to pnode translation from GAM Range Table: */
	bytes = num_possible_nodes() * sizeof(_node_to_pnode[0]);
	_node_to_pnode = kmalloc(bytes, GFP_KERNEL);
	BUG_ON(!_node_to_pnode);

	for (lnid = 0; lnid < num_possible_nodes(); lnid++) {
		unsigned short sockid;

		for (sockid = minsock; sockid <= maxsock; sockid++) {
			if (lnid == _socket_to_node[sockid - minsock]) {
				_node_to_pnode[lnid] = _socket_to_pnode[sockid - minsock];
				break;
			}
		}
		if (sockid > maxsock) {
			pr_err("UV: socket for node %d not found!\n", lnid);
			BUG();
		}
	}

	/*
	 * If socket id == pnode or socket id == node for all nodes,
	 *   system runs faster by removing corresponding conversion table.
	 */
	pr_info("UV: Checking socket->node/pnode for identity maps\n");
	if (minsock == 0) {
		for (i = 0; i < num; i++)
			if (_socket_to_node[i] == SOCK_EMPTY || i != _socket_to_node[i])
				break;
		if (i >= num) {
			kfree(_socket_to_node);
			_socket_to_node = NULL;
			pr_info("UV: 1:1 socket_to_node table removed\n");
		}
	}
	if (minsock == minpnode) {
		for (i = 0; i < num; i++)
			if (_socket_to_pnode[i] != SOCK_EMPTY &&
				_socket_to_pnode[i] != i + minpnode)
				break;
		if (i >= num) {
			kfree(_socket_to_pnode);
			_socket_to_pnode = NULL;
			pr_info("UV: 1:1 socket_to_pnode table removed\n");
		}
	}
}

<<<<<<< HEAD
=======
/* Check which reboot to use */
static void check_efi_reboot(void)
{
	/* If EFI reboot not available, use ACPI reboot */
	if (!efi_enabled(EFI_BOOT))
		reboot_type = BOOT_ACPI;
}

/*
 * User proc fs file handling now deprecated.
 * Recommend using /sys/firmware/sgi_uv/... instead.
 */
static int __maybe_unused proc_hubbed_show(struct seq_file *file, void *data)
{
	pr_notice_once("%s: using deprecated /proc/sgi_uv/hubbed, use /sys/firmware/sgi_uv/hub_type\n",
		       current->comm);
	seq_printf(file, "0x%x\n", uv_hubbed_system);
	return 0;
}

static int __maybe_unused proc_hubless_show(struct seq_file *file, void *data)
{
	pr_notice_once("%s: using deprecated /proc/sgi_uv/hubless, use /sys/firmware/sgi_uv/hubless\n",
		       current->comm);
	seq_printf(file, "0x%x\n", uv_hubless_system);
	return 0;
}

static int __maybe_unused proc_archtype_show(struct seq_file *file, void *data)
{
	pr_notice_once("%s: using deprecated /proc/sgi_uv/archtype, use /sys/firmware/sgi_uv/archtype\n",
		       current->comm);
	seq_printf(file, "%s/%s\n", uv_archtype, oem_table_id);
	return 0;
}

static __init void uv_setup_proc_files(int hubless)
{
	struct proc_dir_entry *pde;

	pde = proc_mkdir(UV_PROC_NODE, NULL);
	proc_create_single("archtype", 0, pde, proc_archtype_show);
	if (hubless)
		proc_create_single("hubless", 0, pde, proc_hubless_show);
	else
		proc_create_single("hubbed", 0, pde, proc_hubbed_show);
}

/* Initialize UV hubless systems */
static __init int uv_system_init_hubless(void)
{
	int rc;

	/* Setup PCH NMI handler */
	uv_nmi_setup_hubless();

	/* Init kernel/BIOS interface */
	rc = uv_bios_init();
	if (rc < 0)
		return rc;

	/* Process UVsystab */
	rc = decode_uv_systab();
	if (rc < 0)
		return rc;

	/* Set section block size for current node memory */
	set_block_size();

	/* Create user access node */
	if (rc >= 0)
		uv_setup_proc_files(1);

	check_efi_reboot();

	return rc;
}

>>>>>>> upstream/android-13
static void __init uv_system_init_hub(void)
{
	struct uv_hub_info_s hub_info = {0};
	int bytes, cpu, nodeid;
	unsigned short min_pnode = 9999, max_pnode = 0;
<<<<<<< HEAD
	char *hub = is_uv4_hub() ? "UV400" :
		    is_uv3_hub() ? "UV300" :
		    is_uv2_hub() ? "UV2000/3000" :
		    is_uv1_hub() ? "UV100/1000" : NULL;
=======
	char *hub = is_uv5_hub() ? "UV500" :
		    is_uv4_hub() ? "UV400" :
		    is_uv3_hub() ? "UV300" :
		    is_uv2_hub() ? "UV2000/3000" : NULL;
>>>>>>> upstream/android-13

	if (!hub) {
		pr_err("UV: Unknown/unsupported UV hub\n");
		return;
	}
	pr_info("UV: Found %s hub\n", hub);

	map_low_mmrs();

<<<<<<< HEAD
	/* Get uv_systab for decoding: */
	uv_bios_init();

	/* If there's an UVsystab problem then abort UV init: */
	if (decode_uv_systab() < 0)
		return;
=======
	/* Get uv_systab for decoding, setup UV BIOS calls */
	uv_bios_init();

	/* If there's an UVsystab problem then abort UV init: */
	if (decode_uv_systab() < 0) {
		pr_err("UV: Mangled UVsystab format\n");
		return;
	}
>>>>>>> upstream/android-13

	build_socket_tables();
	build_uv_gr_table();
	set_block_size();
	uv_init_hub_info(&hub_info);
	uv_possible_blades = num_possible_nodes();
	if (!_node_to_pnode)
		boot_init_possible_blades(&hub_info);

	/* uv_num_possible_blades() is really the hub count: */
	pr_info("UV: Found %d hubs, %d nodes, %d CPUs\n", uv_num_possible_blades(), num_possible_nodes(), num_possible_cpus());

	uv_bios_get_sn_info(0, &uv_type, &sn_partition_id, &sn_coherency_id, &sn_region_size, &system_serial_number);
	hub_info.coherency_domain_number = sn_coherency_id;
	uv_rtc_init();

	bytes = sizeof(void *) * uv_num_possible_blades();
	__uv_hub_info_list = kzalloc(bytes, GFP_KERNEL);
	BUG_ON(!__uv_hub_info_list);

	bytes = sizeof(struct uv_hub_info_s);
	for_each_node(nodeid) {
		struct uv_hub_info_s *new_hub;

		if (__uv_hub_info_list[nodeid]) {
			pr_err("UV: Node %d UV HUB already initialized!?\n", nodeid);
			BUG();
		}

		/* Allocate new per hub info list */
		new_hub = (nodeid == 0) ?  &uv_hub_info_node0 : kzalloc_node(bytes, GFP_KERNEL, nodeid);
		BUG_ON(!new_hub);
		__uv_hub_info_list[nodeid] = new_hub;
		new_hub = uv_hub_info_list(nodeid);
		BUG_ON(!new_hub);
		*new_hub = hub_info;

		/* Use information from GAM table if available: */
		if (_node_to_pnode)
			new_hub->pnode = _node_to_pnode[nodeid];
		else /* Or fill in during CPU loop: */
			new_hub->pnode = 0xffff;

		new_hub->numa_blade_id = uv_node_to_blade_id(nodeid);
<<<<<<< HEAD
		new_hub->memory_nid = -1;
=======
		new_hub->memory_nid = NUMA_NO_NODE;
>>>>>>> upstream/android-13
		new_hub->nr_possible_cpus = 0;
		new_hub->nr_online_cpus = 0;
	}

	/* Initialize per CPU info: */
	for_each_possible_cpu(cpu) {
		int apicid = per_cpu(x86_cpu_to_apicid, cpu);
		int numa_node_id;
		unsigned short pnode;

		nodeid = cpu_to_node(cpu);
		numa_node_id = numa_cpu_node(cpu);
		pnode = uv_apicid_to_pnode(apicid);

		uv_cpu_info_per(cpu)->p_uv_hub_info = uv_hub_info_list(nodeid);
		uv_cpu_info_per(cpu)->blade_cpu_id = uv_cpu_hub_info(cpu)->nr_possible_cpus++;
<<<<<<< HEAD
		if (uv_cpu_hub_info(cpu)->memory_nid == -1)
=======
		if (uv_cpu_hub_info(cpu)->memory_nid == NUMA_NO_NODE)
>>>>>>> upstream/android-13
			uv_cpu_hub_info(cpu)->memory_nid = cpu_to_node(cpu);

		/* Init memoryless node: */
		if (nodeid != numa_node_id &&
		    uv_hub_info_list(numa_node_id)->pnode == 0xffff)
			uv_hub_info_list(numa_node_id)->pnode = pnode;
		else if (uv_cpu_hub_info(cpu)->pnode == 0xffff)
			uv_cpu_hub_info(cpu)->pnode = pnode;
<<<<<<< HEAD

		uv_cpu_scir_info(cpu)->offset = uv_scir_offset(apicid);
=======
>>>>>>> upstream/android-13
	}

	for_each_node(nodeid) {
		unsigned short pnode = uv_hub_info_list(nodeid)->pnode;

		/* Add pnode info for pre-GAM list nodes without CPUs: */
		if (pnode == 0xffff) {
			unsigned long paddr;

			paddr = node_start_pfn(nodeid) << PAGE_SHIFT;
			pnode = uv_gpa_to_pnode(uv_soc_phys_ram_to_gpa(paddr));
			uv_hub_info_list(nodeid)->pnode = pnode;
		}
		min_pnode = min(pnode, min_pnode);
		max_pnode = max(pnode, max_pnode);
		pr_info("UV: UVHUB node:%2d pn:%02x nrcpus:%d\n",
			nodeid,
			uv_hub_info_list(nodeid)->pnode,
			uv_hub_info_list(nodeid)->nr_possible_cpus);
	}

	pr_info("UV: min_pnode:%02x max_pnode:%02x\n", min_pnode, max_pnode);
	map_gru_high(max_pnode);
	map_mmr_high(max_pnode);
	map_mmioh_high(min_pnode, max_pnode);

	uv_nmi_setup();
	uv_cpu_init();
<<<<<<< HEAD
	uv_scir_register_cpu_notifier();
	proc_mkdir("sgi_uv", NULL);
=======
	uv_setup_proc_files(0);
>>>>>>> upstream/android-13

	/* Register Legacy VGA I/O redirection handler: */
	pci_register_set_vga_state(uv_set_vga_state);

<<<<<<< HEAD
	/*
	 * For a kdump kernel the reset must be BOOT_ACPI, not BOOT_EFI, as
	 * EFI is not enabled in the kdump kernel:
	 */
	if (is_kdump_kernel())
		reboot_type = BOOT_ACPI;
}

/*
 * There is a small amount of UV specific code needed to initialize a
 * UV system that does not have a "UV HUB" (referred to as "hubless").
 */
void __init uv_system_init(void)
{
	if (likely(!is_uv_system() && !is_uv_hubless()))
=======
	check_efi_reboot();
}

/*
 * There is a different code path needed to initialize a UV system that does
 * not have a "UV HUB" (referred to as "hubless").
 */
void __init uv_system_init(void)
{
	if (likely(!is_uv_system() && !is_uv_hubless(1)))
>>>>>>> upstream/android-13
		return;

	if (is_uv_system())
		uv_system_init_hub();
	else
<<<<<<< HEAD
		uv_nmi_setup_hubless();
=======
		uv_system_init_hubless();
>>>>>>> upstream/android-13
}

apic_driver(apic_x2apic_uv_x);
