<<<<<<< HEAD
/*
 * bsg.c - block layer implementation of the sg v4 interface
 *
 * Copyright (C) 2004 Jens Axboe <axboe@suse.de> SUSE Labs
 * Copyright (C) 2004 Peter M. Jones <pjones@redhat.com>
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License version 2.  See the file "COPYING" in the main directory of this
 *  archive for more details.
 *
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * bsg.c - block layer implementation of the sg v4 interface
>>>>>>> upstream/android-13
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/file.h>
#include <linux/blkdev.h>
#include <linux/cdev.h>
#include <linux/jiffies.h>
#include <linux/percpu.h>
#include <linux/idr.h>
#include <linux/bsg.h>
#include <linux/slab.h>

#include <scsi/scsi.h>
#include <scsi/scsi_ioctl.h>
<<<<<<< HEAD
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_driver.h>
=======
>>>>>>> upstream/android-13
#include <scsi/sg.h>

#define BSG_DESCRIPTION	"Block layer SCSI generic (bsg) driver"
#define BSG_VERSION	"0.4"

<<<<<<< HEAD
#define bsg_dbg(bd, fmt, ...) \
	pr_debug("%s: " fmt, (bd)->name, ##__VA_ARGS__)

struct bsg_device {
	struct request_queue *queue;
	spinlock_t lock;
	struct hlist_node dev_list;
	refcount_t ref_count;
	char name[20];
	int max_queue;
};

#define BSG_DEFAULT_CMDS	64
#define BSG_MAX_DEVS		32768

static DEFINE_MUTEX(bsg_mutex);
static DEFINE_IDR(bsg_minor_idr);

#define BSG_LIST_ARRAY_SIZE	8
static struct hlist_head bsg_device_list[BSG_LIST_ARRAY_SIZE];

static struct class *bsg_class;
static int bsg_major;

static inline struct hlist_head *bsg_dev_idx_hash(int index)
{
	return &bsg_device_list[index & (BSG_LIST_ARRAY_SIZE - 1)];
}

#define uptr64(val) ((void __user *)(uintptr_t)(val))

static int bsg_scsi_check_proto(struct sg_io_v4 *hdr)
{
	if (hdr->protocol != BSG_PROTOCOL_SCSI  ||
	    hdr->subprotocol != BSG_SUB_PROTOCOL_SCSI_CMD)
		return -EINVAL;
	return 0;
}

static int bsg_scsi_fill_hdr(struct request *rq, struct sg_io_v4 *hdr,
		fmode_t mode)
{
	struct scsi_request *sreq = scsi_req(rq);

	sreq->cmd_len = hdr->request_len;
	if (sreq->cmd_len > BLK_MAX_CDB) {
		sreq->cmd = kzalloc(sreq->cmd_len, GFP_KERNEL);
		if (!sreq->cmd)
			return -ENOMEM;
	}

	if (copy_from_user(sreq->cmd, uptr64(hdr->request), sreq->cmd_len))
		return -EFAULT;
	if (blk_verify_command(sreq->cmd, mode))
		return -EPERM;
	return 0;
}

static int bsg_scsi_complete_rq(struct request *rq, struct sg_io_v4 *hdr)
{
	struct scsi_request *sreq = scsi_req(rq);
	int ret = 0;

	/*
	 * fill in all the output members
	 */
	hdr->device_status = sreq->result & 0xff;
	hdr->transport_status = host_byte(sreq->result);
	hdr->driver_status = driver_byte(sreq->result);
	hdr->info = 0;
	if (hdr->device_status || hdr->transport_status || hdr->driver_status)
		hdr->info |= SG_INFO_CHECK;
	hdr->response_len = 0;

	if (sreq->sense_len && hdr->response) {
		int len = min_t(unsigned int, hdr->max_response_len,
					sreq->sense_len);

		if (copy_to_user(uptr64(hdr->response), sreq->sense, len))
			ret = -EFAULT;
		else
			hdr->response_len = len;
	}

	if (rq->next_rq) {
		hdr->dout_resid = sreq->resid_len;
		hdr->din_resid = scsi_req(rq->next_rq)->resid_len;
	} else if (rq_data_dir(rq) == READ) {
		hdr->din_resid = sreq->resid_len;
	} else {
		hdr->dout_resid = sreq->resid_len;
	}

	return ret;
}

static void bsg_scsi_free_rq(struct request *rq)
{
	scsi_req_free_cmd(scsi_req(rq));
}

static const struct bsg_ops bsg_scsi_ops = {
	.check_proto		= bsg_scsi_check_proto,
	.fill_hdr		= bsg_scsi_fill_hdr,
	.complete_rq		= bsg_scsi_complete_rq,
	.free_rq		= bsg_scsi_free_rq,
};

static struct request *
bsg_map_hdr(struct request_queue *q, struct sg_io_v4 *hdr, fmode_t mode)
{
	struct request *rq, *next_rq = NULL;
	int ret;

	if (!q->bsg_dev.class_dev)
		return ERR_PTR(-ENXIO);

	if (hdr->guard != 'Q')
		return ERR_PTR(-EINVAL);

	ret = q->bsg_dev.ops->check_proto(hdr);
	if (ret)
		return ERR_PTR(ret);

	rq = blk_get_request(q, hdr->dout_xfer_len ?
			REQ_OP_SCSI_OUT : REQ_OP_SCSI_IN, 0);
	if (IS_ERR(rq))
		return rq;

	ret = q->bsg_dev.ops->fill_hdr(rq, hdr, mode);
	if (ret)
		goto out;

	rq->timeout = msecs_to_jiffies(hdr->timeout);
	if (!rq->timeout)
		rq->timeout = q->sg_timeout;
	if (!rq->timeout)
		rq->timeout = BLK_DEFAULT_SG_TIMEOUT;
	if (rq->timeout < BLK_MIN_SG_TIMEOUT)
		rq->timeout = BLK_MIN_SG_TIMEOUT;

	if (hdr->dout_xfer_len && hdr->din_xfer_len) {
		if (!test_bit(QUEUE_FLAG_BIDI, &q->queue_flags)) {
			ret = -EOPNOTSUPP;
			goto out;
		}

		next_rq = blk_get_request(q, REQ_OP_SCSI_IN, 0);
		if (IS_ERR(next_rq)) {
			ret = PTR_ERR(next_rq);
			goto out;
		}

		rq->next_rq = next_rq;
		ret = blk_rq_map_user(q, next_rq, NULL, uptr64(hdr->din_xferp),
				       hdr->din_xfer_len, GFP_KERNEL);
		if (ret)
			goto out_free_nextrq;
	}

	if (hdr->dout_xfer_len) {
		ret = blk_rq_map_user(q, rq, NULL, uptr64(hdr->dout_xferp),
				hdr->dout_xfer_len, GFP_KERNEL);
	} else if (hdr->din_xfer_len) {
		ret = blk_rq_map_user(q, rq, NULL, uptr64(hdr->din_xferp),
				hdr->din_xfer_len, GFP_KERNEL);
	}

	if (ret)
		goto out_unmap_nextrq;
	return rq;

out_unmap_nextrq:
	if (rq->next_rq)
		blk_rq_unmap_user(rq->next_rq->bio);
out_free_nextrq:
	if (rq->next_rq)
		blk_put_request(rq->next_rq);
out:
	q->bsg_dev.ops->free_rq(rq);
	blk_put_request(rq);
	return ERR_PTR(ret);
}

static int blk_complete_sgv4_hdr_rq(struct request *rq, struct sg_io_v4 *hdr,
				    struct bio *bio, struct bio *bidi_bio)
{
	int ret;

	ret = rq->q->bsg_dev.ops->complete_rq(rq, hdr);

	if (rq->next_rq) {
		blk_rq_unmap_user(bidi_bio);
		blk_put_request(rq->next_rq);
	}

	blk_rq_unmap_user(bio);
	rq->q->bsg_dev.ops->free_rq(rq);
	blk_put_request(rq);
	return ret;
}

static struct bsg_device *bsg_alloc_device(void)
{
	struct bsg_device *bd;

	bd = kzalloc(sizeof(struct bsg_device), GFP_KERNEL);
	if (unlikely(!bd))
		return NULL;

	spin_lock_init(&bd->lock);
	bd->max_queue = BSG_DEFAULT_CMDS;
	INIT_HLIST_NODE(&bd->dev_list);
	return bd;
}

static int bsg_put_device(struct bsg_device *bd)
{
	struct request_queue *q = bd->queue;

	mutex_lock(&bsg_mutex);

	if (!refcount_dec_and_test(&bd->ref_count)) {
		mutex_unlock(&bsg_mutex);
		return 0;
	}

	hlist_del(&bd->dev_list);
	mutex_unlock(&bsg_mutex);

	bsg_dbg(bd, "tearing down\n");

	/*
	 * close can always block
	 */
	kfree(bd);
	blk_put_queue(q);
	return 0;
}

static struct bsg_device *bsg_add_device(struct inode *inode,
					 struct request_queue *rq,
					 struct file *file)
{
	struct bsg_device *bd;
	unsigned char buf[32];

	lockdep_assert_held(&bsg_mutex);

	if (!blk_get_queue(rq))
		return ERR_PTR(-ENXIO);

	bd = bsg_alloc_device();
	if (!bd) {
		blk_put_queue(rq);
		return ERR_PTR(-ENOMEM);
	}

	bd->queue = rq;

	refcount_set(&bd->ref_count, 1);
	hlist_add_head(&bd->dev_list, bsg_dev_idx_hash(iminor(inode)));

	strncpy(bd->name, dev_name(rq->bsg_dev.class_dev), sizeof(bd->name) - 1);
	bsg_dbg(bd, "bound to <%s>, max queue %d\n",
		format_dev_t(buf, inode->i_rdev), bd->max_queue);

	return bd;
}

static struct bsg_device *__bsg_get_device(int minor, struct request_queue *q)
{
	struct bsg_device *bd;

	lockdep_assert_held(&bsg_mutex);

	hlist_for_each_entry(bd, bsg_dev_idx_hash(minor), dev_list) {
		if (bd->queue == q) {
			refcount_inc(&bd->ref_count);
			goto found;
		}
	}
	bd = NULL;
found:
	return bd;
}

static struct bsg_device *bsg_get_device(struct inode *inode, struct file *file)
{
	struct bsg_device *bd;
	struct bsg_class_device *bcd;

	/*
	 * find the class device
	 */
	mutex_lock(&bsg_mutex);
	bcd = idr_find(&bsg_minor_idr, iminor(inode));

	if (!bcd) {
		bd = ERR_PTR(-ENODEV);
		goto out_unlock;
	}

	bd = __bsg_get_device(iminor(inode), bcd->queue);
	if (!bd)
		bd = bsg_add_device(inode, bcd->queue, file);

out_unlock:
	mutex_unlock(&bsg_mutex);
	return bd;
}

static int bsg_open(struct inode *inode, struct file *file)
{
	struct bsg_device *bd;

	bd = bsg_get_device(inode, file);

	if (IS_ERR(bd))
		return PTR_ERR(bd);

	file->private_data = bd;
=======
struct bsg_device {
	struct request_queue *queue;
	struct device device;
	struct cdev cdev;
	int max_queue;
	unsigned int timeout;
	unsigned int reserved_size;
	bsg_sg_io_fn *sg_io_fn;
};

static inline struct bsg_device *to_bsg_device(struct inode *inode)
{
	return container_of(inode->i_cdev, struct bsg_device, cdev);
}

#define BSG_DEFAULT_CMDS	64
#define BSG_MAX_DEVS		32768

static DEFINE_IDA(bsg_minor_ida);
static struct class *bsg_class;
static int bsg_major;

static unsigned int bsg_timeout(struct bsg_device *bd, struct sg_io_v4 *hdr)
{
	unsigned int timeout = BLK_DEFAULT_SG_TIMEOUT;

	if (hdr->timeout)
		timeout = msecs_to_jiffies(hdr->timeout);
	else if (bd->timeout)
		timeout = bd->timeout;

	return max_t(unsigned int, timeout, BLK_MIN_SG_TIMEOUT);
}

static int bsg_sg_io(struct bsg_device *bd, fmode_t mode, void __user *uarg)
{
	struct sg_io_v4 hdr;
	int ret;

	if (copy_from_user(&hdr, uarg, sizeof(hdr)))
		return -EFAULT;
	if (hdr.guard != 'Q')
		return -EINVAL;
	ret = bd->sg_io_fn(bd->queue, &hdr, mode, bsg_timeout(bd, &hdr));
	if (!ret && copy_to_user(uarg, &hdr, sizeof(hdr)))
		return -EFAULT;
	return ret;
}

static int bsg_open(struct inode *inode, struct file *file)
{
	if (!blk_get_queue(to_bsg_device(inode)->queue))
		return -ENXIO;
>>>>>>> upstream/android-13
	return 0;
}

static int bsg_release(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	struct bsg_device *bd = file->private_data;

	file->private_data = NULL;
	return bsg_put_device(bd);
=======
	blk_put_queue(to_bsg_device(inode)->queue);
	return 0;
}

static int bsg_get_command_q(struct bsg_device *bd, int __user *uarg)
{
	return put_user(READ_ONCE(bd->max_queue), uarg);
}

static int bsg_set_command_q(struct bsg_device *bd, int __user *uarg)
{
	int max_queue;

	if (get_user(max_queue, uarg))
		return -EFAULT;
	if (max_queue < 1)
		return -EINVAL;
	WRITE_ONCE(bd->max_queue, max_queue);
	return 0;
>>>>>>> upstream/android-13
}

static long bsg_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
<<<<<<< HEAD
	struct bsg_device *bd = file->private_data;
	int __user *uarg = (int __user *) arg;
	int ret;

	switch (cmd) {
		/*
		 * our own ioctls
		 */
	case SG_GET_COMMAND_Q:
		return put_user(bd->max_queue, uarg);
	case SG_SET_COMMAND_Q: {
		int queue;

		if (get_user(queue, uarg))
			return -EFAULT;
		if (queue < 1)
			return -EINVAL;

		spin_lock_irq(&bd->lock);
		bd->max_queue = queue;
		spin_unlock_irq(&bd->lock);
		return 0;
	}
=======
	struct bsg_device *bd = to_bsg_device(file_inode(file));
	struct request_queue *q = bd->queue;
	void __user *uarg = (void __user *) arg;
	int __user *intp = uarg;
	int val;

	switch (cmd) {
	/*
	 * Our own ioctls
	 */
	case SG_GET_COMMAND_Q:
		return bsg_get_command_q(bd, uarg);
	case SG_SET_COMMAND_Q:
		return bsg_set_command_q(bd, uarg);
>>>>>>> upstream/android-13

	/*
	 * SCSI/sg ioctls
	 */
	case SG_GET_VERSION_NUM:
<<<<<<< HEAD
	case SCSI_IOCTL_GET_IDLUN:
	case SCSI_IOCTL_GET_BUS_NUMBER:
	case SG_SET_TIMEOUT:
	case SG_GET_TIMEOUT:
	case SG_GET_RESERVED_SIZE:
	case SG_SET_RESERVED_SIZE:
	case SG_EMULATED_HOST:
	case SCSI_IOCTL_SEND_COMMAND: {
		void __user *uarg = (void __user *) arg;
		return scsi_cmd_ioctl(bd->queue, NULL, file->f_mode, cmd, uarg);
	}
	case SG_IO: {
		struct request *rq;
		struct bio *bio, *bidi_bio = NULL;
		struct sg_io_v4 hdr;
		int at_head;

		if (copy_from_user(&hdr, uarg, sizeof(hdr)))
			return -EFAULT;

		rq = bsg_map_hdr(bd->queue, &hdr, file->f_mode);
		if (IS_ERR(rq))
			return PTR_ERR(rq);

		bio = rq->bio;
		if (rq->next_rq)
			bidi_bio = rq->next_rq->bio;

		at_head = (0 == (hdr.flags & BSG_FLAG_Q_AT_TAIL));
		blk_execute_rq(bd->queue, NULL, rq, at_head);
		ret = blk_complete_sgv4_hdr_rq(rq, &hdr, bio, bidi_bio);

		if (copy_to_user(uarg, &hdr, sizeof(hdr)))
			return -EFAULT;

		return ret;
	}
=======
		return put_user(30527, intp);
	case SCSI_IOCTL_GET_IDLUN:
		return put_user(0, intp);
	case SCSI_IOCTL_GET_BUS_NUMBER:
		return put_user(0, intp);
	case SG_SET_TIMEOUT:
		if (get_user(val, intp))
			return -EFAULT;
		bd->timeout = clock_t_to_jiffies(val);
		return 0;
	case SG_GET_TIMEOUT:
		return jiffies_to_clock_t(bd->timeout);
	case SG_GET_RESERVED_SIZE:
		return put_user(min(bd->reserved_size, queue_max_bytes(q)),
				intp);
	case SG_SET_RESERVED_SIZE:
		if (get_user(val, intp))
			return -EFAULT;
		if (val < 0)
			return -EINVAL;
		bd->reserved_size =
			min_t(unsigned int, val, queue_max_bytes(q));
		return 0;
	case SG_EMULATED_HOST:
		return put_user(1, intp);
	case SG_IO:
		return bsg_sg_io(bd, file->f_mode, uarg);
	case SCSI_IOCTL_SEND_COMMAND:
		pr_warn_ratelimited("%s: calling unsupported SCSI_IOCTL_SEND_COMMAND\n",
				current->comm);
		return -EINVAL;
>>>>>>> upstream/android-13
	default:
		return -ENOTTY;
	}
}

static const struct file_operations bsg_fops = {
	.open		=	bsg_open,
	.release	=	bsg_release,
	.unlocked_ioctl	=	bsg_ioctl,
<<<<<<< HEAD
=======
	.compat_ioctl	=	compat_ptr_ioctl,
>>>>>>> upstream/android-13
	.owner		=	THIS_MODULE,
	.llseek		=	default_llseek,
};

<<<<<<< HEAD
void bsg_unregister_queue(struct request_queue *q)
{
	struct bsg_class_device *bcd = &q->bsg_dev;

	if (!bcd->class_dev)
		return;

	mutex_lock(&bsg_mutex);
	idr_remove(&bsg_minor_idr, bcd->minor);
	if (q->kobj.sd)
		sysfs_remove_link(&q->kobj, "bsg");
	device_unregister(bcd->class_dev);
	bcd->class_dev = NULL;
	mutex_unlock(&bsg_mutex);
}
EXPORT_SYMBOL_GPL(bsg_unregister_queue);

int bsg_register_queue(struct request_queue *q, struct device *parent,
		const char *name, const struct bsg_ops *ops)
{
	struct bsg_class_device *bcd;
	dev_t dev;
	int ret;
	struct device *class_dev = NULL;

	/*
	 * we need a proper transport to send commands, not a stacked device
	 */
	if (!queue_is_rq_based(q))
		return 0;

	bcd = &q->bsg_dev;
	memset(bcd, 0, sizeof(*bcd));

	mutex_lock(&bsg_mutex);

	ret = idr_alloc(&bsg_minor_idr, bcd, 0, BSG_MAX_DEVS, GFP_KERNEL);
	if (ret < 0) {
		if (ret == -ENOSPC) {
			printk(KERN_ERR "bsg: too many bsg devices\n");
			ret = -EINVAL;
		}
		goto unlock;
	}

	bcd->minor = ret;
	bcd->queue = q;
	bcd->ops = ops;
	dev = MKDEV(bsg_major, bcd->minor);
	class_dev = device_create(bsg_class, parent, dev, NULL, "%s", name);
	if (IS_ERR(class_dev)) {
		ret = PTR_ERR(class_dev);
		goto idr_remove;
	}
	bcd->class_dev = class_dev;

	if (q->kobj.sd) {
		ret = sysfs_create_link(&q->kobj, &bcd->class_dev->kobj, "bsg");
		if (ret)
			goto unregister_class_dev;
	}

	mutex_unlock(&bsg_mutex);
	return 0;

unregister_class_dev:
	device_unregister(class_dev);
idr_remove:
	idr_remove(&bsg_minor_idr, bcd->minor);
unlock:
	mutex_unlock(&bsg_mutex);
	return ret;
}

int bsg_scsi_register_queue(struct request_queue *q, struct device *parent)
{
	if (!blk_queue_scsi_passthrough(q)) {
		WARN_ONCE(true, "Attempt to register a non-SCSI queue\n");
		return -EINVAL;
	}

	return bsg_register_queue(q, parent, dev_name(parent), &bsg_scsi_ops);
}
EXPORT_SYMBOL_GPL(bsg_scsi_register_queue);

static struct cdev bsg_cdev;
=======
static void bsg_device_release(struct device *dev)
{
	struct bsg_device *bd = container_of(dev, struct bsg_device, device);

	ida_simple_remove(&bsg_minor_ida, MINOR(bd->device.devt));
	kfree(bd);
}

void bsg_unregister_queue(struct bsg_device *bd)
{
	if (bd->queue->kobj.sd)
		sysfs_remove_link(&bd->queue->kobj, "bsg");
	cdev_device_del(&bd->cdev, &bd->device);
	put_device(&bd->device);
}
EXPORT_SYMBOL_GPL(bsg_unregister_queue);

struct bsg_device *bsg_register_queue(struct request_queue *q,
		struct device *parent, const char *name, bsg_sg_io_fn *sg_io_fn)
{
	struct bsg_device *bd;
	int ret;

	bd = kzalloc(sizeof(*bd), GFP_KERNEL);
	if (!bd)
		return ERR_PTR(-ENOMEM);
	bd->max_queue = BSG_DEFAULT_CMDS;
	bd->reserved_size = INT_MAX;
	bd->queue = q;
	bd->sg_io_fn = sg_io_fn;

	ret = ida_simple_get(&bsg_minor_ida, 0, BSG_MAX_DEVS, GFP_KERNEL);
	if (ret < 0) {
		if (ret == -ENOSPC)
			dev_err(parent, "bsg: too many bsg devices\n");
		kfree(bd);
		return ERR_PTR(ret);
	}
	bd->device.devt = MKDEV(bsg_major, ret);
	bd->device.class = bsg_class;
	bd->device.parent = parent;
	bd->device.release = bsg_device_release;
	dev_set_name(&bd->device, "%s", name);
	device_initialize(&bd->device);

	cdev_init(&bd->cdev, &bsg_fops);
	bd->cdev.owner = THIS_MODULE;
	ret = cdev_device_add(&bd->cdev, &bd->device);
	if (ret)
		goto out_put_device;

	if (q->kobj.sd) {
		ret = sysfs_create_link(&q->kobj, &bd->device.kobj, "bsg");
		if (ret)
			goto out_device_del;
	}

	return bd;

out_device_del:
	cdev_device_del(&bd->cdev, &bd->device);
out_put_device:
	put_device(&bd->device);
	return ERR_PTR(ret);
}
EXPORT_SYMBOL_GPL(bsg_register_queue);
>>>>>>> upstream/android-13

static char *bsg_devnode(struct device *dev, umode_t *mode)
{
	return kasprintf(GFP_KERNEL, "bsg/%s", dev_name(dev));
}

static int __init bsg_init(void)
{
<<<<<<< HEAD
	int ret, i;
	dev_t devid;

	for (i = 0; i < BSG_LIST_ARRAY_SIZE; i++)
		INIT_HLIST_HEAD(&bsg_device_list[i]);
=======
	dev_t devid;
	int ret;
>>>>>>> upstream/android-13

	bsg_class = class_create(THIS_MODULE, "bsg");
	if (IS_ERR(bsg_class))
		return PTR_ERR(bsg_class);
	bsg_class->devnode = bsg_devnode;

	ret = alloc_chrdev_region(&devid, 0, BSG_MAX_DEVS, "bsg");
	if (ret)
		goto destroy_bsg_class;
<<<<<<< HEAD

	bsg_major = MAJOR(devid);

	cdev_init(&bsg_cdev, &bsg_fops);
	ret = cdev_add(&bsg_cdev, MKDEV(bsg_major, 0), BSG_MAX_DEVS);
	if (ret)
		goto unregister_chrdev;

	printk(KERN_INFO BSG_DESCRIPTION " version " BSG_VERSION
	       " loaded (major %d)\n", bsg_major);
	return 0;
unregister_chrdev:
	unregister_chrdev_region(MKDEV(bsg_major, 0), BSG_MAX_DEVS);
=======
	bsg_major = MAJOR(devid);

	printk(KERN_INFO BSG_DESCRIPTION " version " BSG_VERSION
	       " loaded (major %d)\n", bsg_major);
	return 0;

>>>>>>> upstream/android-13
destroy_bsg_class:
	class_destroy(bsg_class);
	return ret;
}

MODULE_AUTHOR("Jens Axboe");
MODULE_DESCRIPTION(BSG_DESCRIPTION);
MODULE_LICENSE("GPL");

device_initcall(bsg_init);
