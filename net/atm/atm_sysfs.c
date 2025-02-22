// SPDX-License-Identifier: GPL-2.0
/* ATM driver model support. */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/atmdev.h>
#include "common.h"
#include "resources.h"

#define to_atm_dev(cldev) container_of(cldev, struct atm_dev, class_dev)

<<<<<<< HEAD
static ssize_t show_type(struct device *cdev,
=======
static ssize_t type_show(struct device *cdev,
>>>>>>> upstream/android-13
			 struct device_attribute *attr, char *buf)
{
	struct atm_dev *adev = to_atm_dev(cdev);

	return scnprintf(buf, PAGE_SIZE, "%s\n", adev->type);
}

<<<<<<< HEAD
static ssize_t show_address(struct device *cdev,
=======
static ssize_t address_show(struct device *cdev,
>>>>>>> upstream/android-13
			    struct device_attribute *attr, char *buf)
{
	struct atm_dev *adev = to_atm_dev(cdev);

	return scnprintf(buf, PAGE_SIZE, "%pM\n", adev->esi);
}

<<<<<<< HEAD
static ssize_t show_atmaddress(struct device *cdev,
=======
static ssize_t atmaddress_show(struct device *cdev,
>>>>>>> upstream/android-13
			       struct device_attribute *attr, char *buf)
{
	unsigned long flags;
	struct atm_dev *adev = to_atm_dev(cdev);
	struct atm_dev_addr *aaddr;
<<<<<<< HEAD
	int bin[] = { 1, 2, 10, 6, 1 }, *fmt = bin;
	int i, j, count = 0;

	spin_lock_irqsave(&adev->lock, flags);
	list_for_each_entry(aaddr, &adev->local, entry) {
		for (i = 0, j = 0; i < ATM_ESA_LEN; ++i, ++j) {
			if (j == *fmt) {
				count += scnprintf(buf + count,
						   PAGE_SIZE - count, ".");
				++fmt;
				j = 0;
			}
			count += scnprintf(buf + count,
					   PAGE_SIZE - count, "%02x",
					   aaddr->addr.sas_addr.prv[i]);
		}
		count += scnprintf(buf + count, PAGE_SIZE - count, "\n");
=======
	int count = 0;

	spin_lock_irqsave(&adev->lock, flags);
	list_for_each_entry(aaddr, &adev->local, entry) {
		count += scnprintf(buf + count, PAGE_SIZE - count,
				   "%1phN.%2phN.%10phN.%6phN.%1phN\n",
				   &aaddr->addr.sas_addr.prv[0],
				   &aaddr->addr.sas_addr.prv[1],
				   &aaddr->addr.sas_addr.prv[3],
				   &aaddr->addr.sas_addr.prv[13],
				   &aaddr->addr.sas_addr.prv[19]);
>>>>>>> upstream/android-13
	}
	spin_unlock_irqrestore(&adev->lock, flags);

	return count;
}

<<<<<<< HEAD
static ssize_t show_atmindex(struct device *cdev,
=======
static ssize_t atmindex_show(struct device *cdev,
>>>>>>> upstream/android-13
			     struct device_attribute *attr, char *buf)
{
	struct atm_dev *adev = to_atm_dev(cdev);

	return scnprintf(buf, PAGE_SIZE, "%d\n", adev->number);
}

<<<<<<< HEAD
static ssize_t show_carrier(struct device *cdev,
=======
static ssize_t carrier_show(struct device *cdev,
>>>>>>> upstream/android-13
			    struct device_attribute *attr, char *buf)
{
	struct atm_dev *adev = to_atm_dev(cdev);

	return scnprintf(buf, PAGE_SIZE, "%d\n",
			 adev->signal == ATM_PHY_SIG_LOST ? 0 : 1);
}

<<<<<<< HEAD
static ssize_t show_link_rate(struct device *cdev,
=======
static ssize_t link_rate_show(struct device *cdev,
>>>>>>> upstream/android-13
			      struct device_attribute *attr, char *buf)
{
	struct atm_dev *adev = to_atm_dev(cdev);
	int link_rate;

	/* show the link rate, not the data rate */
	switch (adev->link_rate) {
	case ATM_OC3_PCR:
		link_rate = 155520000;
		break;
	case ATM_OC12_PCR:
		link_rate = 622080000;
		break;
	case ATM_25_PCR:
		link_rate = 25600000;
		break;
	default:
		link_rate = adev->link_rate * 8 * 53;
	}
	return scnprintf(buf, PAGE_SIZE, "%d\n", link_rate);
}

<<<<<<< HEAD
static DEVICE_ATTR(address, 0444, show_address, NULL);
static DEVICE_ATTR(atmaddress, 0444, show_atmaddress, NULL);
static DEVICE_ATTR(atmindex, 0444, show_atmindex, NULL);
static DEVICE_ATTR(carrier, 0444, show_carrier, NULL);
static DEVICE_ATTR(type, 0444, show_type, NULL);
static DEVICE_ATTR(link_rate, 0444, show_link_rate, NULL);
=======
static DEVICE_ATTR_RO(address);
static DEVICE_ATTR_RO(atmaddress);
static DEVICE_ATTR_RO(atmindex);
static DEVICE_ATTR_RO(carrier);
static DEVICE_ATTR_RO(type);
static DEVICE_ATTR_RO(link_rate);
>>>>>>> upstream/android-13

static struct device_attribute *atm_attrs[] = {
	&dev_attr_atmaddress,
	&dev_attr_address,
	&dev_attr_atmindex,
	&dev_attr_carrier,
	&dev_attr_type,
	&dev_attr_link_rate,
	NULL
};


static int atm_uevent(struct device *cdev, struct kobj_uevent_env *env)
{
	struct atm_dev *adev;

	if (!cdev)
		return -ENODEV;

	adev = to_atm_dev(cdev);
	if (!adev)
		return -ENODEV;

	if (add_uevent_var(env, "NAME=%s%d", adev->type, adev->number))
		return -ENOMEM;

	return 0;
}

static void atm_release(struct device *cdev)
{
	struct atm_dev *adev = to_atm_dev(cdev);

	kfree(adev);
}

static struct class atm_class = {
	.name		= "atm",
	.dev_release	= atm_release,
	.dev_uevent		= atm_uevent,
};

int atm_register_sysfs(struct atm_dev *adev, struct device *parent)
{
	struct device *cdev = &adev->class_dev;
	int i, j, err;

	cdev->class = &atm_class;
	cdev->parent = parent;
	dev_set_drvdata(cdev, adev);

	dev_set_name(cdev, "%s%d", adev->type, adev->number);
	err = device_register(cdev);
	if (err < 0)
		return err;

	for (i = 0; atm_attrs[i]; i++) {
		err = device_create_file(cdev, atm_attrs[i]);
		if (err)
			goto err_out;
	}

	return 0;

err_out:
	for (j = 0; j < i; j++)
		device_remove_file(cdev, atm_attrs[j]);
	device_del(cdev);
	return err;
}

void atm_unregister_sysfs(struct atm_dev *adev)
{
	struct device *cdev = &adev->class_dev;

	device_del(cdev);
}

int __init atm_sysfs_init(void)
{
	return class_register(&atm_class);
}

void __exit atm_sysfs_exit(void)
{
	class_unregister(&atm_class);
}
