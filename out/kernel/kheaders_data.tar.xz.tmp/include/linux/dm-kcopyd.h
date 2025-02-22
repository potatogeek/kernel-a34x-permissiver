

#ifndef _LINUX_DM_KCOPYD_H
#define _LINUX_DM_KCOPYD_H

#ifdef __KERNEL__

#include <linux/dm-io.h>


#define DM_KCOPYD_MAX_REGIONS 8

#define DM_KCOPYD_IGNORE_ERROR 1
#define DM_KCOPYD_WRITE_SEQ    2

struct dm_kcopyd_throttle {
	unsigned throttle;
	unsigned num_io_jobs;
	unsigned io_period;
	unsigned total_period;
	unsigned last_jiffies;
};


#define DECLARE_DM_KCOPYD_THROTTLE_WITH_MODULE_PARM(name, description)	\
static struct dm_kcopyd_throttle dm_kcopyd_throttle = { 100, 0, 0, 0, 0 }; \
module_param_named(name, dm_kcopyd_throttle.throttle, uint, 0644); \
MODULE_PARM_DESC(name, description)


struct dm_kcopyd_client;
struct dm_kcopyd_client *dm_kcopyd_client_create(struct dm_kcopyd_throttle *throttle);
void dm_kcopyd_client_destroy(struct dm_kcopyd_client *kc);


typedef void (*dm_kcopyd_notify_fn)(int read_err, unsigned long write_err,
				    void *context);

void dm_kcopyd_copy(struct dm_kcopyd_client *kc, struct dm_io_region *from,
		    unsigned num_dests, struct dm_io_region *dests,
		    unsigned flags, dm_kcopyd_notify_fn fn, void *context);


void *dm_kcopyd_prepare_callback(struct dm_kcopyd_client *kc,
				 dm_kcopyd_notify_fn fn, void *context);
void dm_kcopyd_do_callback(void *job, int read_err, unsigned long write_err);

void dm_kcopyd_zero(struct dm_kcopyd_client *kc,
		    unsigned num_dests, struct dm_io_region *dests,
		    unsigned flags, dm_kcopyd_notify_fn fn, void *context);

#endif	
#endif	
