// SPDX-License-Identifier: GPL-2.0
/*
 * procfs-based user access to knfsd statistics
 *
 * /proc/net/rpc/nfsd
 *
 * Format:
 *	rc <hits> <misses> <nocache>
 *			Statistsics for the reply cache
<<<<<<< HEAD
 *	fh <stale> <total-lookups> <anonlookups> <dir-not-in-dcache> <nondir-not-in-dcache>
 *			statistics for filehandle lookup
 *	io <bytes-read> <bytes-written>
 *			statistics for IO throughput
 *	th <threads> <fullcnt> <10%-20%> <20%-30%> ... <90%-100%> <100%> 
 *			time (seconds) when nfsd thread usage above thresholds
 *			and number of times that all threads were in use
 *	ra cache-size  <10%  <20%  <30% ... <100% not-found
 *			number of times that read-ahead entry was found that deep in
 *			the cache.
=======
 *	fh <stale> <deprecated filehandle cache stats>
 *			statistics for filehandle lookup
 *	io <bytes-read> <bytes-written>
 *			statistics for IO throughput
 *	th <threads> <deprecated thread usage histogram stats>
 *			number of threads
 *	ra <deprecated ra-cache stats>
 *
>>>>>>> upstream/android-13
 *	plus generic RPC stats (see net/sunrpc/stats.c)
 *
 * Copyright (C) 1995, 1996, 1997 Olaf Kirch <okir@monad.swb.de>
 */

#include <linux/seq_file.h>
#include <linux/module.h>
#include <linux/sunrpc/stats.h>
#include <net/net_namespace.h>

#include "nfsd.h"

struct nfsd_stats	nfsdstats;
struct svc_stat		nfsd_svcstats = {
	.program	= &nfsd_program,
};

static int nfsd_proc_show(struct seq_file *seq, void *v)
{
	int i;

<<<<<<< HEAD
	seq_printf(seq, "rc %u %u %u\nfh %u %u %u %u %u\nio %u %u\n",
		      nfsdstats.rchits,
		      nfsdstats.rcmisses,
		      nfsdstats.rcnocache,
		      nfsdstats.fh_stale,
		      nfsdstats.fh_lookup,
		      nfsdstats.fh_anon,
		      nfsdstats.fh_nocache_dir,
		      nfsdstats.fh_nocache_nondir,
		      nfsdstats.io_read,
		      nfsdstats.io_write);
	/* thread usage: */
	seq_printf(seq, "th %u %u", nfsdstats.th_cnt, nfsdstats.th_fullcnt);
	for (i=0; i<10; i++) {
		unsigned int jifs = nfsdstats.th_usage[i];
		unsigned int sec = jifs / HZ, msec = (jifs % HZ)*1000/HZ;
		seq_printf(seq, " %u.%03u", sec, msec);
	}

	/* newline and ra-cache */
	seq_printf(seq, "\nra %u", nfsdstats.ra_size);
	for (i=0; i<11; i++)
		seq_printf(seq, " %u", nfsdstats.ra_depth[i]);
	seq_putc(seq, '\n');
	
=======
	seq_printf(seq, "rc %lld %lld %lld\nfh %lld 0 0 0 0\nio %lld %lld\n",
		   percpu_counter_sum_positive(&nfsdstats.counter[NFSD_STATS_RC_HITS]),
		   percpu_counter_sum_positive(&nfsdstats.counter[NFSD_STATS_RC_MISSES]),
		   percpu_counter_sum_positive(&nfsdstats.counter[NFSD_STATS_RC_NOCACHE]),
		   percpu_counter_sum_positive(&nfsdstats.counter[NFSD_STATS_FH_STALE]),
		   percpu_counter_sum_positive(&nfsdstats.counter[NFSD_STATS_IO_READ]),
		   percpu_counter_sum_positive(&nfsdstats.counter[NFSD_STATS_IO_WRITE]));

	/* thread usage: */
	seq_printf(seq, "th %u 0", nfsdstats.th_cnt);

	/* deprecated thread usage histogram stats */
	for (i = 0; i < 10; i++)
		seq_puts(seq, " 0.000");

	/* deprecated ra-cache stats */
	seq_puts(seq, "\nra 0 0 0 0 0 0 0 0 0 0 0 0\n");

>>>>>>> upstream/android-13
	/* show my rpc info */
	svc_seq_show(seq, &nfsd_svcstats);

#ifdef CONFIG_NFSD_V4
	/* Show count for individual nfsv4 operations */
	/* Writing operation numbers 0 1 2 also for maintaining uniformity */
	seq_printf(seq,"proc4ops %u", LAST_NFS4_OP + 1);
<<<<<<< HEAD
	for (i = 0; i <= LAST_NFS4_OP; i++)
		seq_printf(seq, " %u", nfsdstats.nfs4_opcount[i]);
=======
	for (i = 0; i <= LAST_NFS4_OP; i++) {
		seq_printf(seq, " %lld",
			   percpu_counter_sum_positive(&nfsdstats.counter[NFSD_STATS_NFS4_OP(i)]));
	}
>>>>>>> upstream/android-13

	seq_putc(seq, '\n');
#endif

	return 0;
}

static int nfsd_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, nfsd_proc_show, NULL);
}

<<<<<<< HEAD
static const struct file_operations nfsd_proc_fops = {
	.open = nfsd_proc_open,
	.read  = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

void
nfsd_stat_init(void)
{
	svc_proc_register(&init_net, &nfsd_svcstats, &nfsd_proc_fops);
}

void
nfsd_stat_shutdown(void)
{
=======
static const struct proc_ops nfsd_proc_ops = {
	.proc_open	= nfsd_proc_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
};

int nfsd_percpu_counters_init(struct percpu_counter counters[], int num)
{
	int i, err = 0;

	for (i = 0; !err && i < num; i++)
		err = percpu_counter_init(&counters[i], 0, GFP_KERNEL);

	if (!err)
		return 0;

	for (; i > 0; i--)
		percpu_counter_destroy(&counters[i-1]);

	return err;
}

void nfsd_percpu_counters_reset(struct percpu_counter counters[], int num)
{
	int i;

	for (i = 0; i < num; i++)
		percpu_counter_set(&counters[i], 0);
}

void nfsd_percpu_counters_destroy(struct percpu_counter counters[], int num)
{
	int i;

	for (i = 0; i < num; i++)
		percpu_counter_destroy(&counters[i]);
}

static int nfsd_stat_counters_init(void)
{
	return nfsd_percpu_counters_init(nfsdstats.counter, NFSD_STATS_COUNTERS_NUM);
}

static void nfsd_stat_counters_destroy(void)
{
	nfsd_percpu_counters_destroy(nfsdstats.counter, NFSD_STATS_COUNTERS_NUM);
}

int nfsd_stat_init(void)
{
	int err;

	err = nfsd_stat_counters_init();
	if (err)
		return err;

	svc_proc_register(&init_net, &nfsd_svcstats, &nfsd_proc_ops);

	return 0;
}

void nfsd_stat_shutdown(void)
{
	nfsd_stat_counters_destroy();
>>>>>>> upstream/android-13
	svc_proc_unregister(&init_net, "nfsd");
}
