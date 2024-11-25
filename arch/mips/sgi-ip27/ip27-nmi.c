// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/mmzone.h>
#include <linux/nodemask.h>
#include <linux/spinlock.h>
#include <linux/smp.h>
#include <linux/atomic.h>
#include <asm/sn/types.h>
#include <asm/sn/addrs.h>
#include <asm/sn/nmi.h>
#include <asm/sn/arch.h>
<<<<<<< HEAD
#include <asm/sn/sn0/hub.h>
=======
#include <asm/sn/agent.h>
>>>>>>> upstream/android-13

#if 0
#define NODE_NUM_CPUS(n)	CNODE_NUM_CPUS(n)
#else
#define NODE_NUM_CPUS(n)	CPUS_PER_NODE
#endif

<<<<<<< HEAD
#define CNODEID_NONE (cnodeid_t)-1
=======
#define SEND_NMI(_nasid, _slice)	\
	REMOTE_HUB_S((_nasid),  (PI_NMI_A + ((_slice) * PI_NMI_OFFSET)), 1)
>>>>>>> upstream/android-13

typedef unsigned long machreg_t;

static arch_spinlock_t nmi_lock = __ARCH_SPIN_LOCK_UNLOCKED;

/*
 * Let's see what else we need to do here. Set up sp, gp?
 */
void nmi_dump(void)
{
	void cont_nmi_dump(void);

	cont_nmi_dump();
}

void install_cpu_nmi_handler(int slice)
{
	nmi_t *nmi_addr;

	nmi_addr = (nmi_t *)NMI_ADDR(get_nasid(), slice);
	if (nmi_addr->call_addr)
		return;
	nmi_addr->magic = NMI_MAGIC;
	nmi_addr->call_addr = (void *)nmi_dump;
	nmi_addr->call_addr_c =
		(void *)(~((unsigned long)(nmi_addr->call_addr)));
	nmi_addr->call_parm = 0;
}

/*
 * Copy the cpu registers which have been saved in the IP27prom format
 * into the eframe format for the node under consideration.
 */

void nmi_cpu_eframe_save(nasid_t nasid, int slice)
{
	struct reg_struct *nr;
	int		i;

	/* Get the pointer to the current cpu's register set. */
	nr = (struct reg_struct *)
		(TO_UNCAC(TO_NODE(nasid, IP27_NMI_KREGS_OFFSET)) +
		slice * IP27_NMI_KREGS_CPU_SIZE);

<<<<<<< HEAD
	printk("NMI nasid %d: slice %d\n", nasid, slice);
=======
	pr_emerg("NMI nasid %d: slice %d\n", nasid, slice);
>>>>>>> upstream/android-13

	/*
	 * Saved main processor registers
	 */
	for (i = 0; i < 32; ) {
		if ((i % 4) == 0)
<<<<<<< HEAD
			printk("$%2d   :", i);
		printk(" %016lx", nr->gpr[i]);

		i++;
		if ((i % 4) == 0)
			printk("\n");
	}

	printk("Hi    : (value lost)\n");
	printk("Lo    : (value lost)\n");
=======
			pr_emerg("$%2d   :", i);
		pr_cont(" %016lx", nr->gpr[i]);

		i++;
		if ((i % 4) == 0)
			pr_cont("\n");
	}

	pr_emerg("Hi    : (value lost)\n");
	pr_emerg("Lo    : (value lost)\n");
>>>>>>> upstream/android-13

	/*
	 * Saved cp0 registers
	 */
<<<<<<< HEAD
	printk("epc   : %016lx %pS\n", nr->epc, (void *) nr->epc);
	printk("%s\n", print_tainted());
	printk("ErrEPC: %016lx %pS\n", nr->error_epc, (void *) nr->error_epc);
	printk("ra    : %016lx %pS\n", nr->gpr[31], (void *) nr->gpr[31]);
	printk("Status: %08lx	      ", nr->sr);

	if (nr->sr & ST0_KX)
		printk("KX ");
	if (nr->sr & ST0_SX)
		printk("SX	");
	if (nr->sr & ST0_UX)
		printk("UX ");

	switch (nr->sr & ST0_KSU) {
	case KSU_USER:
		printk("USER ");
		break;
	case KSU_SUPERVISOR:
		printk("SUPERVISOR ");
		break;
	case KSU_KERNEL:
		printk("KERNEL ");
		break;
	default:
		printk("BAD_MODE ");
=======
	pr_emerg("epc   : %016lx %pS\n", nr->epc, (void *)nr->epc);
	pr_emerg("%s\n", print_tainted());
	pr_emerg("ErrEPC: %016lx %pS\n", nr->error_epc, (void *)nr->error_epc);
	pr_emerg("ra    : %016lx %pS\n", nr->gpr[31], (void *)nr->gpr[31]);
	pr_emerg("Status: %08lx	      ", nr->sr);

	if (nr->sr & ST0_KX)
		pr_cont("KX ");
	if (nr->sr & ST0_SX)
		pr_cont("SX ");
	if (nr->sr & ST0_UX)
		pr_cont("UX ");

	switch (nr->sr & ST0_KSU) {
	case KSU_USER:
		pr_cont("USER ");
		break;
	case KSU_SUPERVISOR:
		pr_cont("SUPERVISOR ");
		break;
	case KSU_KERNEL:
		pr_cont("KERNEL ");
		break;
	default:
		pr_cont("BAD_MODE ");
>>>>>>> upstream/android-13
		break;
	}

	if (nr->sr & ST0_ERL)
<<<<<<< HEAD
		printk("ERL ");
	if (nr->sr & ST0_EXL)
		printk("EXL ");
	if (nr->sr & ST0_IE)
		printk("IE ");
	printk("\n");

	printk("Cause : %08lx\n", nr->cause);
	printk("PrId  : %08x\n", read_c0_prid());
	printk("BadVA : %016lx\n", nr->badva);
	printk("CErr  : %016lx\n", nr->cache_err);
	printk("NMI_SR: %016lx\n", nr->nmi_sr);

	printk("\n");
=======
		pr_cont("ERL ");
	if (nr->sr & ST0_EXL)
		pr_cont("EXL ");
	if (nr->sr & ST0_IE)
		pr_cont("IE ");
	pr_cont("\n");

	pr_emerg("Cause : %08lx\n", nr->cause);
	pr_emerg("PrId  : %08x\n", read_c0_prid());
	pr_emerg("BadVA : %016lx\n", nr->badva);
	pr_emerg("CErr  : %016lx\n", nr->cache_err);
	pr_emerg("NMI_SR: %016lx\n", nr->nmi_sr);

	pr_emerg("\n");
>>>>>>> upstream/android-13
}

void nmi_dump_hub_irq(nasid_t nasid, int slice)
{
<<<<<<< HEAD
	hubreg_t mask0, mask1, pend0, pend1;
=======
	u64 mask0, mask1, pend0, pend1;
>>>>>>> upstream/android-13

	if (slice == 0) {				/* Slice A */
		mask0 = REMOTE_HUB_L(nasid, PI_INT_MASK0_A);
		mask1 = REMOTE_HUB_L(nasid, PI_INT_MASK1_A);
	} else {					/* Slice B */
		mask0 = REMOTE_HUB_L(nasid, PI_INT_MASK0_B);
		mask1 = REMOTE_HUB_L(nasid, PI_INT_MASK1_B);
	}

	pend0 = REMOTE_HUB_L(nasid, PI_INT_PEND0);
	pend1 = REMOTE_HUB_L(nasid, PI_INT_PEND1);

<<<<<<< HEAD
	printk("PI_INT_MASK0: %16Lx PI_INT_MASK1: %16Lx\n", mask0, mask1);
	printk("PI_INT_PEND0: %16Lx PI_INT_PEND1: %16Lx\n", pend0, pend1);
	printk("\n\n");
=======
	pr_emerg("PI_INT_MASK0: %16llx PI_INT_MASK1: %16llx\n", mask0, mask1);
	pr_emerg("PI_INT_PEND0: %16llx PI_INT_PEND1: %16llx\n", pend0, pend1);
	pr_emerg("\n\n");
>>>>>>> upstream/android-13
}

/*
 * Copy the cpu registers which have been saved in the IP27prom format
 * into the eframe format for the node under consideration.
 */
<<<<<<< HEAD
void nmi_node_eframe_save(cnodeid_t  cnode)
{
	nasid_t nasid;
	int slice;

	/* Make sure that we have a valid node */
	if (cnode == CNODEID_NONE)
		return;

	nasid = COMPACT_TO_NASID_NODEID(cnode);
=======
void nmi_node_eframe_save(nasid_t nasid)
{
	int slice;

>>>>>>> upstream/android-13
	if (nasid == INVALID_NASID)
		return;

	/* Save the registers into eframe for each cpu */
	for (slice = 0; slice < NODE_NUM_CPUS(slice); slice++) {
		nmi_cpu_eframe_save(nasid, slice);
		nmi_dump_hub_irq(nasid, slice);
	}
}

/*
 * Save the nmi cpu registers for all cpus in the system.
 */
void
nmi_eframes_save(void)
{
<<<<<<< HEAD
	cnodeid_t	cnode;

	for_each_online_node(cnode)
		nmi_node_eframe_save(cnode);
=======
	nasid_t nasid;

	for_each_online_node(nasid)
		nmi_node_eframe_save(nasid);
>>>>>>> upstream/android-13
}

void
cont_nmi_dump(void)
{
#ifndef REAL_NMI_SIGNAL
	static atomic_t nmied_cpus = ATOMIC_INIT(0);

	atomic_inc(&nmied_cpus);
#endif
	/*
	 * Only allow 1 cpu to proceed
	 */
	arch_spin_lock(&nmi_lock);

#ifdef REAL_NMI_SIGNAL
	/*
	 * Wait up to 15 seconds for the other cpus to respond to the NMI.
	 * If a cpu has not responded after 10 sec, send it 1 additional NMI.
	 * This is for 2 reasons:
	 *	- sometimes a MMSC fail to NMI all cpus.
	 *	- on 512p SN0 system, the MMSC will only send NMIs to
	 *	  half the cpus. Unfortunately, we don't know which cpus may be
	 *	  NMIed - it depends on how the site chooses to configure.
	 *
	 * Note: it has been measure that it takes the MMSC up to 2.3 secs to
	 * send NMIs to all cpus on a 256p system.
	 */
	for (i=0; i < 1500; i++) {
		for_each_online_node(node)
			if (NODEPDA(node)->dump_count == 0)
				break;
		if (node == MAX_NUMNODES)
			break;
		if (i == 1000) {
			for_each_online_node(node)
				if (NODEPDA(node)->dump_count == 0) {
					cpu = cpumask_first(cpumask_of_node(node));
					for (n=0; n < CNODE_NUM_CPUS(node); cpu++, n++) {
						CPUMASK_SETB(nmied_cpus, cpu);
						/*
						 * cputonasid, cputoslice
						 * needs kernel cpuid
						 */
						SEND_NMI((cputonasid(cpu)), (cputoslice(cpu)));
					}
				}

		}
		udelay(10000);
	}
#else
	while (atomic_read(&nmied_cpus) != num_online_cpus());
#endif

	/*
	 * Save the nmi cpu registers for all cpu in the eframe format.
	 */
	nmi_eframes_save();
	LOCAL_HUB_S(NI_PORT_RESET, NPR_PORTRESET | NPR_LOCALRESET);
}
