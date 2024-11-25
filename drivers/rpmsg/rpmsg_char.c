// SPDX-License-Identifier: GPL-2.0
/*
<<<<<<< HEAD
=======
 * Copyright (C) 2022, STMicroelectronics
>>>>>>> upstream/android-13
 * Copyright (c) 2016, Linaro Ltd.
 * Copyright (c) 2012, Michal Simek <monstr@monstr.eu>
 * Copyright (c) 2012, PetaLogix
 * Copyright (c) 2011, Texas Instruments, Inc.
 * Copyright (c) 2011, Google, Inc.
 *
 * Based on rpmsg performance statistics driver by Michal Simek, which in turn
 * was based on TI & Google OMX rpmsg driver.
 */
<<<<<<< HEAD
=======

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

>>>>>>> upstream/android-13
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/idr.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/poll.h>
#include <linux/rpmsg.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <uapi/linux/rpmsg.h>

<<<<<<< HEAD
=======
#include "rpmsg_char.h"
>>>>>>> upstream/android-13
#include "rpmsg_internal.h"

#define RPMSG_DEV_MAX	(MINORMASK + 1)

static dev_t rpmsg_major;
<<<<<<< HEAD
static struct class *rpmsg_class;

static DEFINE_IDA(rpmsg_ctrl_ida);
=======

>>>>>>> upstream/android-13
static DEFINE_IDA(rpmsg_ept_ida);
static DEFINE_IDA(rpmsg_minor_ida);

#define dev_to_eptdev(dev) container_of(dev, struct rpmsg_eptdev, dev)
#define cdev_to_eptdev(i_cdev) container_of(i_cdev, struct rpmsg_eptdev, cdev)

<<<<<<< HEAD
#define dev_to_ctrldev(dev) container_of(dev, struct rpmsg_ctrldev, dev)
#define cdev_to_ctrldev(i_cdev) container_of(i_cdev, struct rpmsg_ctrldev, cdev)

/**
 * struct rpmsg_ctrldev - control device for instantiating endpoint devices
 * @rpdev:	underlaying rpmsg device
 * @cdev:	cdev for the ctrl device
 * @dev:	device for the ctrl device
 */
struct rpmsg_ctrldev {
	struct rpmsg_device *rpdev;
	struct cdev cdev;
	struct device dev;
};

=======
>>>>>>> upstream/android-13
/**
 * struct rpmsg_eptdev - endpoint device context
 * @dev:	endpoint device
 * @cdev:	cdev for the endpoint device
 * @rpdev:	underlaying rpmsg device
 * @chinfo:	info used to open the endpoint
 * @ept_lock:	synchronization of @ept modifications
 * @ept:	rpmsg endpoint reference, when open
 * @queue_lock:	synchronization of @queue operations
 * @queue:	incoming message queue
 * @readq:	wait object for incoming queue
<<<<<<< HEAD
=======
 * @default_ept: set to channel default endpoint if the default endpoint should be re-used
 *              on device open to prevent endpoint address update.
>>>>>>> upstream/android-13
 */
struct rpmsg_eptdev {
	struct device dev;
	struct cdev cdev;

	struct rpmsg_device *rpdev;
	struct rpmsg_channel_info chinfo;

	struct mutex ept_lock;
	struct rpmsg_endpoint *ept;
<<<<<<< HEAD
=======
	struct rpmsg_endpoint *default_ept;
>>>>>>> upstream/android-13

	spinlock_t queue_lock;
	struct sk_buff_head queue;
	wait_queue_head_t readq;
<<<<<<< HEAD
};

static int rpmsg_eptdev_destroy(struct device *dev, void *data)
=======

};

int rpmsg_chrdev_eptdev_destroy(struct device *dev, void *data)
>>>>>>> upstream/android-13
{
	struct rpmsg_eptdev *eptdev = dev_to_eptdev(dev);

	mutex_lock(&eptdev->ept_lock);
	if (eptdev->ept) {
		rpmsg_destroy_ept(eptdev->ept);
		eptdev->ept = NULL;
	}
	mutex_unlock(&eptdev->ept_lock);

	/* wake up any blocked readers */
	wake_up_interruptible(&eptdev->readq);

<<<<<<< HEAD
	device_del(&eptdev->dev);
=======
	cdev_device_del(&eptdev->cdev, &eptdev->dev);
>>>>>>> upstream/android-13
	put_device(&eptdev->dev);

	return 0;
}
<<<<<<< HEAD

static int rpmsg_ept_cb(struct rpmsg_device *rpdev, void *buf, int len,
			void *priv, u32 addr)
=======
EXPORT_SYMBOL(rpmsg_chrdev_eptdev_destroy);

static int rpmsg_ept_copy_cb(struct rpmsg_device *rpdev, void *buf, int len,
			     void *priv, u32 addr)
>>>>>>> upstream/android-13
{
	struct rpmsg_eptdev *eptdev = priv;
	struct sk_buff *skb;

	skb = alloc_skb(len, GFP_ATOMIC);
	if (!skb)
		return -ENOMEM;

	skb_put_data(skb, buf, len);

	spin_lock(&eptdev->queue_lock);
	skb_queue_tail(&eptdev->queue, skb);
	spin_unlock(&eptdev->queue_lock);

	/* wake up any blocking processes, waiting for new data */
	wake_up_interruptible(&eptdev->readq);

	return 0;
}

<<<<<<< HEAD
=======
static int rpmsg_ept_no_copy_cb(struct rpmsg_device *rpdev, void *buf, int len,
				void *priv, u32 addr)
{
	struct rpmsg_eptdev *eptdev = priv;
	struct sk_buff *skb;

	skb = alloc_skb(0, GFP_ATOMIC);
	if (!skb)
		return -ENOMEM;

	skb->head = buf;
	skb->data = buf;
	skb_reset_tail_pointer(skb);
	skb_set_end_offset(skb, len);
	skb_put(skb, len);

	spin_lock(&eptdev->queue_lock);
	skb_queue_tail(&eptdev->queue, skb);
	spin_unlock(&eptdev->queue_lock);

	/* wake up any blocking processes, waiting for new data */
	wake_up_interruptible(&eptdev->readq);

	return RPMSG_DEFER;
}

static int rpmsg_ept_cb(struct rpmsg_device *rpdev, void *buf, int len,
			void *priv, u32 addr)
{
	struct rpmsg_eptdev *eptdev = priv;
	rpmsg_rx_cb_t cb;

	cb = (eptdev->ept->rx_done) ? rpmsg_ept_no_copy_cb : rpmsg_ept_copy_cb;

	return cb(rpdev, buf, len, priv, addr);
}

>>>>>>> upstream/android-13
static int rpmsg_eptdev_open(struct inode *inode, struct file *filp)
{
	struct rpmsg_eptdev *eptdev = cdev_to_eptdev(inode->i_cdev);
	struct rpmsg_endpoint *ept;
	struct rpmsg_device *rpdev = eptdev->rpdev;
	struct device *dev = &eptdev->dev;

<<<<<<< HEAD
	get_device(dev);

	ept = rpmsg_create_ept(rpdev, rpmsg_ept_cb, eptdev, eptdev->chinfo);
=======
	if (eptdev->ept)
		return -EBUSY;

	get_device(dev);

	/*
	 * If the default_ept is set, the rpmsg device default endpoint is used.
	 * Else a new endpoint is created on open that will be destroyed on release.
	 */
	if (eptdev->default_ept)
		ept = eptdev->default_ept;
	else
		ept = rpmsg_create_ept(rpdev, rpmsg_ept_cb, eptdev, eptdev->chinfo);

>>>>>>> upstream/android-13
	if (!ept) {
		dev_err(dev, "failed to open %s\n", eptdev->chinfo.name);
		put_device(dev);
		return -EINVAL;
	}

	eptdev->ept = ept;
	filp->private_data = eptdev;

	return 0;
}

static int rpmsg_eptdev_release(struct inode *inode, struct file *filp)
{
	struct rpmsg_eptdev *eptdev = cdev_to_eptdev(inode->i_cdev);
	struct device *dev = &eptdev->dev;
<<<<<<< HEAD
	struct sk_buff *skb;
=======
>>>>>>> upstream/android-13

	/* Close the endpoint, if it's not already destroyed by the parent */
	mutex_lock(&eptdev->ept_lock);
	if (eptdev->ept) {
<<<<<<< HEAD
		rpmsg_destroy_ept(eptdev->ept);
=======
		if (!eptdev->default_ept)
			rpmsg_destroy_ept(eptdev->ept);
>>>>>>> upstream/android-13
		eptdev->ept = NULL;
	}
	mutex_unlock(&eptdev->ept_lock);

	/* Discard all SKBs */
<<<<<<< HEAD
	while (!skb_queue_empty(&eptdev->queue)) {
		skb = skb_dequeue(&eptdev->queue);
		kfree_skb(skb);
	}
=======
	skb_queue_purge(&eptdev->queue);
>>>>>>> upstream/android-13

	put_device(dev);

	return 0;
}

<<<<<<< HEAD
static ssize_t rpmsg_eptdev_read(struct file *filp, char __user *buf,
				 size_t len, loff_t *f_pos)
{
=======
static ssize_t rpmsg_eptdev_read_iter(struct kiocb *iocb, struct iov_iter *to)
{
	struct file *filp = iocb->ki_filp;
>>>>>>> upstream/android-13
	struct rpmsg_eptdev *eptdev = filp->private_data;
	unsigned long flags;
	struct sk_buff *skb;
	int use;

	if (!eptdev->ept)
		return -EPIPE;

	spin_lock_irqsave(&eptdev->queue_lock, flags);

	/* Wait for data in the queue */
	if (skb_queue_empty(&eptdev->queue)) {
		spin_unlock_irqrestore(&eptdev->queue_lock, flags);

		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;

		/* Wait until we get data or the endpoint goes away */
		if (wait_event_interruptible(eptdev->readq,
					     !skb_queue_empty(&eptdev->queue) ||
					     !eptdev->ept))
			return -ERESTARTSYS;

		/* We lost the endpoint while waiting */
		if (!eptdev->ept)
			return -EPIPE;

		spin_lock_irqsave(&eptdev->queue_lock, flags);
	}

	skb = skb_dequeue(&eptdev->queue);
	spin_unlock_irqrestore(&eptdev->queue_lock, flags);
	if (!skb)
		return -EFAULT;

<<<<<<< HEAD
	use = min_t(size_t, len, skb->len);
	if (copy_to_user(buf, skb->data, use))
		use = -EFAULT;

=======
	use = min_t(size_t, iov_iter_count(to), skb->len);
	if (copy_to_iter(skb->data, use, to) != use)
		use = -EFAULT;

	if (eptdev->ept->rx_done) {
		rpmsg_rx_done(eptdev->ept, skb->data);
		/*
		 * Data memory is freed by rpmsg_rx_done(), reset the skb data
		 * pointers so kfree_skb() does not try to free a second time.
		 */
		skb->head = NULL;
		skb->data = NULL;
	}
>>>>>>> upstream/android-13
	kfree_skb(skb);

	return use;
}

<<<<<<< HEAD
static ssize_t rpmsg_eptdev_write(struct file *filp, const char __user *buf,
				  size_t len, loff_t *f_pos)
{
	struct rpmsg_eptdev *eptdev = filp->private_data;
	void *kbuf;
	int ret;

	kbuf = memdup_user(buf, len);
	if (IS_ERR(kbuf))
		return PTR_ERR(kbuf);
=======
static ssize_t rpmsg_eptdev_write_iter(struct kiocb *iocb,
				       struct iov_iter *from)
{
	struct file *filp = iocb->ki_filp;
	struct rpmsg_eptdev *eptdev = filp->private_data;
	size_t len = iov_iter_count(from);
	void *kbuf;
	int ret;

	kbuf = kzalloc(len, GFP_KERNEL);
	if (!kbuf)
		return -ENOMEM;

	if (!copy_from_iter_full(kbuf, len, from)) {
		ret = -EFAULT;
		goto free_kbuf;
	}
>>>>>>> upstream/android-13

	if (mutex_lock_interruptible(&eptdev->ept_lock)) {
		ret = -ERESTARTSYS;
		goto free_kbuf;
	}

	if (!eptdev->ept) {
		ret = -EPIPE;
		goto unlock_eptdev;
	}

	if (filp->f_flags & O_NONBLOCK)
<<<<<<< HEAD
		ret = rpmsg_trysend(eptdev->ept, kbuf, len);
	else
		ret = rpmsg_send(eptdev->ept, kbuf, len);
=======
		ret = rpmsg_trysendto(eptdev->ept, kbuf, len, eptdev->chinfo.dst);
	else
		ret = rpmsg_sendto(eptdev->ept, kbuf, len, eptdev->chinfo.dst);
>>>>>>> upstream/android-13

unlock_eptdev:
	mutex_unlock(&eptdev->ept_lock);

free_kbuf:
	kfree(kbuf);
	return ret < 0 ? ret : len;
}

static __poll_t rpmsg_eptdev_poll(struct file *filp, poll_table *wait)
{
	struct rpmsg_eptdev *eptdev = filp->private_data;
	__poll_t mask = 0;

	if (!eptdev->ept)
		return EPOLLERR;

	poll_wait(filp, &eptdev->readq, wait);

	if (!skb_queue_empty(&eptdev->queue))
		mask |= EPOLLIN | EPOLLRDNORM;

	mask |= rpmsg_poll(eptdev->ept, filp, wait);

	return mask;
}

static long rpmsg_eptdev_ioctl(struct file *fp, unsigned int cmd,
			       unsigned long arg)
{
	struct rpmsg_eptdev *eptdev = fp->private_data;

	if (cmd != RPMSG_DESTROY_EPT_IOCTL)
		return -EINVAL;

<<<<<<< HEAD
	return rpmsg_eptdev_destroy(&eptdev->dev, NULL);
=======
	/* Don't allow to destroy a default endpoint. */
	if (eptdev->default_ept)
		return -EINVAL;

	return rpmsg_chrdev_eptdev_destroy(&eptdev->dev, NULL);
>>>>>>> upstream/android-13
}

static const struct file_operations rpmsg_eptdev_fops = {
	.owner = THIS_MODULE,
	.open = rpmsg_eptdev_open,
	.release = rpmsg_eptdev_release,
<<<<<<< HEAD
	.read = rpmsg_eptdev_read,
	.write = rpmsg_eptdev_write,
	.poll = rpmsg_eptdev_poll,
	.unlocked_ioctl = rpmsg_eptdev_ioctl,
	.compat_ioctl = rpmsg_eptdev_ioctl,
=======
	.read_iter = rpmsg_eptdev_read_iter,
	.write_iter = rpmsg_eptdev_write_iter,
	.poll = rpmsg_eptdev_poll,
	.unlocked_ioctl = rpmsg_eptdev_ioctl,
	.compat_ioctl = compat_ptr_ioctl,
>>>>>>> upstream/android-13
};

static ssize_t name_show(struct device *dev, struct device_attribute *attr,
			 char *buf)
{
	struct rpmsg_eptdev *eptdev = dev_get_drvdata(dev);

	return sprintf(buf, "%s\n", eptdev->chinfo.name);
}
static DEVICE_ATTR_RO(name);

static ssize_t src_show(struct device *dev, struct device_attribute *attr,
			 char *buf)
{
	struct rpmsg_eptdev *eptdev = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", eptdev->chinfo.src);
}
static DEVICE_ATTR_RO(src);

static ssize_t dst_show(struct device *dev, struct device_attribute *attr,
			 char *buf)
{
	struct rpmsg_eptdev *eptdev = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", eptdev->chinfo.dst);
}
static DEVICE_ATTR_RO(dst);

static struct attribute *rpmsg_eptdev_attrs[] = {
	&dev_attr_name.attr,
	&dev_attr_src.attr,
	&dev_attr_dst.attr,
	NULL
};
ATTRIBUTE_GROUPS(rpmsg_eptdev);

static void rpmsg_eptdev_release_device(struct device *dev)
{
	struct rpmsg_eptdev *eptdev = dev_to_eptdev(dev);

	ida_simple_remove(&rpmsg_ept_ida, dev->id);
	ida_simple_remove(&rpmsg_minor_ida, MINOR(eptdev->dev.devt));
<<<<<<< HEAD
	cdev_del(&eptdev->cdev);
	kfree(eptdev);
}

static int rpmsg_eptdev_create(struct rpmsg_ctrldev *ctrldev,
			       struct rpmsg_channel_info chinfo)
{
	struct rpmsg_device *rpdev = ctrldev->rpdev;
	struct rpmsg_eptdev *eptdev;
	struct device *dev;
	int ret;

	eptdev = kzalloc(sizeof(*eptdev), GFP_KERNEL);
	if (!eptdev)
		return -ENOMEM;

	dev = &eptdev->dev;
	eptdev->rpdev = rpdev;
	eptdev->chinfo = chinfo;
=======
	kfree(eptdev);
}

static struct rpmsg_eptdev *rpmsg_chrdev_eptdev_alloc(struct rpmsg_device *rpdev,
						      struct device *parent)
{
	struct rpmsg_eptdev *eptdev;
	struct device *dev;

	eptdev = kzalloc(sizeof(*eptdev), GFP_KERNEL);
	if (!eptdev)
		return ERR_PTR(-ENOMEM);

	dev = &eptdev->dev;
	eptdev->rpdev = rpdev;
>>>>>>> upstream/android-13

	mutex_init(&eptdev->ept_lock);
	spin_lock_init(&eptdev->queue_lock);
	skb_queue_head_init(&eptdev->queue);
	init_waitqueue_head(&eptdev->readq);

	device_initialize(dev);
	dev->class = rpmsg_class;
<<<<<<< HEAD
	dev->parent = &ctrldev->dev;
=======
	dev->parent = parent;
>>>>>>> upstream/android-13
	dev->groups = rpmsg_eptdev_groups;
	dev_set_drvdata(dev, eptdev);

	cdev_init(&eptdev->cdev, &rpmsg_eptdev_fops);
	eptdev->cdev.owner = THIS_MODULE;

<<<<<<< HEAD
=======
	return eptdev;
}

static int rpmsg_chrdev_eptdev_add(struct rpmsg_eptdev *eptdev, struct rpmsg_channel_info chinfo)
{
	struct device *dev = &eptdev->dev;
	int ret;

	eptdev->chinfo = chinfo;

>>>>>>> upstream/android-13
	ret = ida_simple_get(&rpmsg_minor_ida, 0, RPMSG_DEV_MAX, GFP_KERNEL);
	if (ret < 0)
		goto free_eptdev;
	dev->devt = MKDEV(MAJOR(rpmsg_major), ret);

	ret = ida_simple_get(&rpmsg_ept_ida, 0, 0, GFP_KERNEL);
	if (ret < 0)
		goto free_minor_ida;
	dev->id = ret;
	dev_set_name(dev, "rpmsg%d", ret);

<<<<<<< HEAD
	ret = cdev_add(&eptdev->cdev, dev->devt, 1);
=======
	ret = cdev_device_add(&eptdev->cdev, &eptdev->dev);
>>>>>>> upstream/android-13
	if (ret)
		goto free_ept_ida;

	/* We can now rely on the release function for cleanup */
	dev->release = rpmsg_eptdev_release_device;

<<<<<<< HEAD
	ret = device_add(dev);
	if (ret) {
		dev_err(dev, "device_add failed: %d\n", ret);
		put_device(dev);
	}

=======
>>>>>>> upstream/android-13
	return ret;

free_ept_ida:
	ida_simple_remove(&rpmsg_ept_ida, dev->id);
free_minor_ida:
	ida_simple_remove(&rpmsg_minor_ida, MINOR(dev->devt));
free_eptdev:
	put_device(dev);
	kfree(eptdev);

	return ret;
}

<<<<<<< HEAD
static int rpmsg_ctrldev_open(struct inode *inode, struct file *filp)
{
	struct rpmsg_ctrldev *ctrldev = cdev_to_ctrldev(inode->i_cdev);

	get_device(&ctrldev->dev);
	filp->private_data = ctrldev;

	return 0;
}

static int rpmsg_ctrldev_release(struct inode *inode, struct file *filp)
{
	struct rpmsg_ctrldev *ctrldev = cdev_to_ctrldev(inode->i_cdev);

	put_device(&ctrldev->dev);

	return 0;
}

static long rpmsg_ctrldev_ioctl(struct file *fp, unsigned int cmd,
				unsigned long arg)
{
	struct rpmsg_ctrldev *ctrldev = fp->private_data;
	void __user *argp = (void __user *)arg;
	struct rpmsg_endpoint_info eptinfo;
	struct rpmsg_channel_info chinfo;

	if (cmd != RPMSG_CREATE_EPT_IOCTL)
		return -EINVAL;

	if (copy_from_user(&eptinfo, argp, sizeof(eptinfo)))
		return -EFAULT;

	memcpy(chinfo.name, eptinfo.name, RPMSG_NAME_SIZE);
	chinfo.name[RPMSG_NAME_SIZE-1] = '\0';
	chinfo.src = eptinfo.src;
	chinfo.dst = eptinfo.dst;

	return rpmsg_eptdev_create(ctrldev, chinfo);
};

static const struct file_operations rpmsg_ctrldev_fops = {
	.owner = THIS_MODULE,
	.open = rpmsg_ctrldev_open,
	.release = rpmsg_ctrldev_release,
	.unlocked_ioctl = rpmsg_ctrldev_ioctl,
	.compat_ioctl = rpmsg_ctrldev_ioctl,
};

static void rpmsg_ctrldev_release_device(struct device *dev)
{
	struct rpmsg_ctrldev *ctrldev = dev_to_ctrldev(dev);

	ida_simple_remove(&rpmsg_ctrl_ida, dev->id);
	ida_simple_remove(&rpmsg_minor_ida, MINOR(dev->devt));
	cdev_del(&ctrldev->cdev);
	kfree(ctrldev);
}

static int rpmsg_chrdev_probe(struct rpmsg_device *rpdev)
{
	struct rpmsg_ctrldev *ctrldev;
	struct device *dev;
	int ret;

	ctrldev = kzalloc(sizeof(*ctrldev), GFP_KERNEL);
	if (!ctrldev)
		return -ENOMEM;

	ctrldev->rpdev = rpdev;

	dev = &ctrldev->dev;
	device_initialize(dev);
	dev->parent = &rpdev->dev;
	dev->class = rpmsg_class;

	cdev_init(&ctrldev->cdev, &rpmsg_ctrldev_fops);
	ctrldev->cdev.owner = THIS_MODULE;

	ret = ida_simple_get(&rpmsg_minor_ida, 0, RPMSG_DEV_MAX, GFP_KERNEL);
	if (ret < 0)
		goto free_ctrldev;
	dev->devt = MKDEV(MAJOR(rpmsg_major), ret);

	ret = ida_simple_get(&rpmsg_ctrl_ida, 0, 0, GFP_KERNEL);
	if (ret < 0)
		goto free_minor_ida;
	dev->id = ret;
	dev_set_name(&ctrldev->dev, "rpmsg_ctrl%d", ret);

	ret = cdev_add(&ctrldev->cdev, dev->devt, 1);
	if (ret)
		goto free_ctrl_ida;

	/* We can now rely on the release function for cleanup */
	dev->release = rpmsg_ctrldev_release_device;

	ret = device_add(dev);
	if (ret) {
		dev_err(&rpdev->dev, "device_add failed: %d\n", ret);
		put_device(dev);
	}

	dev_set_drvdata(&rpdev->dev, ctrldev);

	return ret;

free_ctrl_ida:
	ida_simple_remove(&rpmsg_ctrl_ida, dev->id);
free_minor_ida:
	ida_simple_remove(&rpmsg_minor_ida, MINOR(dev->devt));
free_ctrldev:
	put_device(dev);
	kfree(ctrldev);

	return ret;
=======
int rpmsg_chrdev_eptdev_create(struct rpmsg_device *rpdev, struct device *parent,
			       struct rpmsg_channel_info chinfo)
{
	struct rpmsg_eptdev *eptdev;
	int ret;

	eptdev = rpmsg_chrdev_eptdev_alloc(rpdev, parent);
	if (IS_ERR(eptdev))
		return PTR_ERR(eptdev);

	ret = rpmsg_chrdev_eptdev_add(eptdev, chinfo);

	return ret;
}
EXPORT_SYMBOL(rpmsg_chrdev_eptdev_create);

static int rpmsg_chrdev_probe(struct rpmsg_device *rpdev)
{
	struct rpmsg_channel_info chinfo;
	struct rpmsg_eptdev *eptdev;
	struct device *dev = &rpdev->dev;

	memcpy(chinfo.name, rpdev->id.name, RPMSG_NAME_SIZE);
	chinfo.src = rpdev->src;
	chinfo.dst = rpdev->dst;

	eptdev = rpmsg_chrdev_eptdev_alloc(rpdev, dev);
	if (IS_ERR(eptdev))
		return PTR_ERR(eptdev);

	/* Set the default_ept to the rpmsg device endpoint */
	eptdev->default_ept = rpdev->ept;

	/*
	 * The rpmsg_ept_cb uses *priv parameter to get its rpmsg_eptdev context.
	 * Storedit in default_ept *priv field.
	 */
	eptdev->default_ept->priv = eptdev;

	return rpmsg_chrdev_eptdev_add(eptdev, chinfo);
>>>>>>> upstream/android-13
}

static void rpmsg_chrdev_remove(struct rpmsg_device *rpdev)
{
<<<<<<< HEAD
	struct rpmsg_ctrldev *ctrldev = dev_get_drvdata(&rpdev->dev);
	int ret;

	/* Destroy all endpoints */
	ret = device_for_each_child(&ctrldev->dev, NULL, rpmsg_eptdev_destroy);
	if (ret)
		dev_warn(&rpdev->dev, "failed to nuke endpoints: %d\n", ret);

	device_del(&ctrldev->dev);
	put_device(&ctrldev->dev);
}

static struct rpmsg_driver rpmsg_chrdev_driver = {
	.probe = rpmsg_chrdev_probe,
	.remove = rpmsg_chrdev_remove,
	.drv = {
		.name = "rpmsg_chrdev",
	},
};

static int rpmsg_char_init(void)
{
	int ret;

	ret = alloc_chrdev_region(&rpmsg_major, 0, RPMSG_DEV_MAX, "rpmsg");
	if (ret < 0) {
		pr_err("rpmsg: failed to allocate char dev region\n");
		return ret;
	}

	rpmsg_class = class_create(THIS_MODULE, "rpmsg");
	if (IS_ERR(rpmsg_class)) {
		pr_err("failed to create rpmsg class\n");
		unregister_chrdev_region(rpmsg_major, RPMSG_DEV_MAX);
		return PTR_ERR(rpmsg_class);
	}

	ret = register_rpmsg_driver(&rpmsg_chrdev_driver);
	if (ret < 0) {
		pr_err("rpmsgchr: failed to register rpmsg driver\n");
		class_destroy(rpmsg_class);
		unregister_chrdev_region(rpmsg_major, RPMSG_DEV_MAX);
	}

	return ret;
}
postcore_initcall(rpmsg_char_init);
=======
	int ret;

	ret = device_for_each_child(&rpdev->dev, NULL, rpmsg_chrdev_eptdev_destroy);
	if (ret)
		dev_warn(&rpdev->dev, "failed to destroy endpoints: %d\n", ret);
}

static struct rpmsg_device_id rpmsg_chrdev_id_table[] = {
	{ .name	= "rpmsg-raw" },
	{ },
};

static struct rpmsg_driver rpmsg_chrdev_driver = {
	.probe = rpmsg_chrdev_probe,
	.remove = rpmsg_chrdev_remove,
	.callback = rpmsg_ept_cb,
	.id_table = rpmsg_chrdev_id_table,
	.drv.name = "rpmsg_chrdev",
};

static int rpmsg_chrdev_init(void)
{
	int ret;

	ret = alloc_chrdev_region(&rpmsg_major, 0, RPMSG_DEV_MAX, "rpmsg_char");
	if (ret < 0) {
		pr_err("failed to allocate char dev region\n");
		return ret;
	}

	ret = register_rpmsg_driver(&rpmsg_chrdev_driver);
	if (ret < 0) {
		pr_err("rpmsg: failed to register rpmsg raw driver\n");
		goto free_region;
	}

	return 0;

free_region:
	unregister_chrdev_region(rpmsg_major, RPMSG_DEV_MAX);

	return ret;
}
postcore_initcall(rpmsg_chrdev_init);
>>>>>>> upstream/android-13

static void rpmsg_chrdev_exit(void)
{
	unregister_rpmsg_driver(&rpmsg_chrdev_driver);
<<<<<<< HEAD
	class_destroy(rpmsg_class);
=======
>>>>>>> upstream/android-13
	unregister_chrdev_region(rpmsg_major, RPMSG_DEV_MAX);
}
module_exit(rpmsg_chrdev_exit);

MODULE_ALIAS("rpmsg:rpmsg_chrdev");
MODULE_LICENSE("GPL v2");
