<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * I2C Link Layer for Samsung S3FWRN5 NCI based Driver
 *
 * Copyright (C) 2015 Samsung Electrnoics
 * Robert Baldyga <r.baldyga@samsung.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

=======
 */

#include <linux/clk.h>
>>>>>>> upstream/android-13
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/module.h>

#include <net/nfc/nfc.h>

<<<<<<< HEAD
#include "s3fwrn5.h"

#define S3FWRN5_I2C_DRIVER_NAME "s3fwrn5_i2c"

#define S3FWRN5_I2C_MAX_PAYLOAD 32
#define S3FWRN5_EN_WAIT_TIME 150

struct s3fwrn5_i2c_phy {
	struct i2c_client *i2c_dev;
	struct nci_dev *ndev;

	int gpio_en;
	int gpio_fw_wake;

	struct mutex mutex;

	enum s3fwrn5_mode mode;
	unsigned int irq_skip:1;
};

static void s3fwrn5_i2c_set_wake(void *phy_id, bool wake)
{
	struct s3fwrn5_i2c_phy *phy = phy_id;

	mutex_lock(&phy->mutex);
	gpio_set_value(phy->gpio_fw_wake, wake);
	msleep(S3FWRN5_EN_WAIT_TIME/2);
	mutex_unlock(&phy->mutex);
}

=======
#include "phy_common.h"

#define S3FWRN5_I2C_DRIVER_NAME "s3fwrn5_i2c"

struct s3fwrn5_i2c_phy {
	struct phy_common common;
	struct i2c_client *i2c_dev;
	struct clk *clk;

	unsigned int irq_skip:1;
};

>>>>>>> upstream/android-13
static void s3fwrn5_i2c_set_mode(void *phy_id, enum s3fwrn5_mode mode)
{
	struct s3fwrn5_i2c_phy *phy = phy_id;

<<<<<<< HEAD
	mutex_lock(&phy->mutex);

	if (phy->mode == mode)
		goto out;

	phy->mode = mode;

	gpio_set_value(phy->gpio_en, 1);
	gpio_set_value(phy->gpio_fw_wake, 0);
	if (mode == S3FWRN5_MODE_FW)
		gpio_set_value(phy->gpio_fw_wake, 1);

	if (mode != S3FWRN5_MODE_COLD) {
		msleep(S3FWRN5_EN_WAIT_TIME);
		gpio_set_value(phy->gpio_en, 0);
		msleep(S3FWRN5_EN_WAIT_TIME/2);
	}

	phy->irq_skip = true;

out:
	mutex_unlock(&phy->mutex);
}

static enum s3fwrn5_mode s3fwrn5_i2c_get_mode(void *phy_id)
{
	struct s3fwrn5_i2c_phy *phy = phy_id;
	enum s3fwrn5_mode mode;

	mutex_lock(&phy->mutex);

	mode = phy->mode;

	mutex_unlock(&phy->mutex);

	return mode;
=======
	mutex_lock(&phy->common.mutex);

	if (s3fwrn5_phy_power_ctrl(&phy->common, mode) == false)
		goto out;

	phy->irq_skip = true;

out:
	mutex_unlock(&phy->common.mutex);
>>>>>>> upstream/android-13
}

static int s3fwrn5_i2c_write(void *phy_id, struct sk_buff *skb)
{
	struct s3fwrn5_i2c_phy *phy = phy_id;
	int ret;

<<<<<<< HEAD
	mutex_lock(&phy->mutex);
=======
	mutex_lock(&phy->common.mutex);
>>>>>>> upstream/android-13

	phy->irq_skip = false;

	ret = i2c_master_send(phy->i2c_dev, skb->data, skb->len);
	if (ret == -EREMOTEIO) {
		/* Retry, chip was in standby */
		usleep_range(110000, 120000);
		ret  = i2c_master_send(phy->i2c_dev, skb->data, skb->len);
	}

<<<<<<< HEAD
	mutex_unlock(&phy->mutex);
=======
	mutex_unlock(&phy->common.mutex);
>>>>>>> upstream/android-13

	if (ret < 0)
		return ret;

	if (ret != skb->len)
		return -EREMOTEIO;

	return 0;
}

static const struct s3fwrn5_phy_ops i2c_phy_ops = {
<<<<<<< HEAD
	.set_wake = s3fwrn5_i2c_set_wake,
	.set_mode = s3fwrn5_i2c_set_mode,
	.get_mode = s3fwrn5_i2c_get_mode,
=======
	.set_wake = s3fwrn5_phy_set_wake,
	.set_mode = s3fwrn5_i2c_set_mode,
	.get_mode = s3fwrn5_phy_get_mode,
>>>>>>> upstream/android-13
	.write = s3fwrn5_i2c_write,
};

static int s3fwrn5_i2c_read(struct s3fwrn5_i2c_phy *phy)
{
	struct sk_buff *skb;
	size_t hdr_size;
	size_t data_len;
	char hdr[4];
	int ret;

<<<<<<< HEAD
	hdr_size = (phy->mode == S3FWRN5_MODE_NCI) ?
=======
	hdr_size = (phy->common.mode == S3FWRN5_MODE_NCI) ?
>>>>>>> upstream/android-13
		NCI_CTRL_HDR_SIZE : S3FWRN5_FW_HDR_SIZE;
	ret = i2c_master_recv(phy->i2c_dev, hdr, hdr_size);
	if (ret < 0)
		return ret;

	if (ret < hdr_size)
		return -EBADMSG;

<<<<<<< HEAD
	data_len = (phy->mode == S3FWRN5_MODE_NCI) ?
=======
	data_len = (phy->common.mode == S3FWRN5_MODE_NCI) ?
>>>>>>> upstream/android-13
		((struct nci_ctrl_hdr *)hdr)->plen :
		((struct s3fwrn5_fw_header *)hdr)->len;

	skb = alloc_skb(hdr_size + data_len, GFP_KERNEL);
	if (!skb)
		return -ENOMEM;

	skb_put_data(skb, hdr, hdr_size);

	if (data_len == 0)
		goto out;

	ret = i2c_master_recv(phy->i2c_dev, skb_put(skb, data_len), data_len);
	if (ret != data_len) {
		kfree_skb(skb);
		return -EBADMSG;
	}

out:
<<<<<<< HEAD
	return s3fwrn5_recv_frame(phy->ndev, skb, phy->mode);
=======
	return s3fwrn5_recv_frame(phy->common.ndev, skb, phy->common.mode);
>>>>>>> upstream/android-13
}

static irqreturn_t s3fwrn5_i2c_irq_thread_fn(int irq, void *phy_id)
{
	struct s3fwrn5_i2c_phy *phy = phy_id;
<<<<<<< HEAD
	int ret = 0;

	if (!phy || !phy->ndev) {
=======

	if (!phy || !phy->common.ndev) {
>>>>>>> upstream/android-13
		WARN_ON_ONCE(1);
		return IRQ_NONE;
	}

<<<<<<< HEAD
	mutex_lock(&phy->mutex);
=======
	mutex_lock(&phy->common.mutex);
>>>>>>> upstream/android-13

	if (phy->irq_skip)
		goto out;

<<<<<<< HEAD
	switch (phy->mode) {
	case S3FWRN5_MODE_NCI:
	case S3FWRN5_MODE_FW:
		ret = s3fwrn5_i2c_read(phy);
		break;
	case S3FWRN5_MODE_COLD:
		ret = -EREMOTEIO;
=======
	switch (phy->common.mode) {
	case S3FWRN5_MODE_NCI:
	case S3FWRN5_MODE_FW:
		s3fwrn5_i2c_read(phy);
		break;
	case S3FWRN5_MODE_COLD:
>>>>>>> upstream/android-13
		break;
	}

out:
<<<<<<< HEAD
	mutex_unlock(&phy->mutex);
=======
	mutex_unlock(&phy->common.mutex);
>>>>>>> upstream/android-13

	return IRQ_HANDLED;
}

static int s3fwrn5_i2c_parse_dt(struct i2c_client *client)
{
	struct s3fwrn5_i2c_phy *phy = i2c_get_clientdata(client);
	struct device_node *np = client->dev.of_node;

	if (!np)
		return -ENODEV;

<<<<<<< HEAD
	phy->gpio_en = of_get_named_gpio(np, "s3fwrn5,en-gpios", 0);
	if (!gpio_is_valid(phy->gpio_en))
		return -ENODEV;

	phy->gpio_fw_wake = of_get_named_gpio(np, "s3fwrn5,fw-gpios", 0);
	if (!gpio_is_valid(phy->gpio_fw_wake))
		return -ENODEV;
=======
	phy->common.gpio_en = of_get_named_gpio(np, "en-gpios", 0);
	if (!gpio_is_valid(phy->common.gpio_en)) {
		/* Support also deprecated property */
		phy->common.gpio_en = of_get_named_gpio(np,
							"s3fwrn5,en-gpios",
							0);
		if (!gpio_is_valid(phy->common.gpio_en))
			return -ENODEV;
	}

	phy->common.gpio_fw_wake = of_get_named_gpio(np, "wake-gpios", 0);
	if (!gpio_is_valid(phy->common.gpio_fw_wake)) {
		/* Support also deprecated property */
		phy->common.gpio_fw_wake = of_get_named_gpio(np,
							     "s3fwrn5,fw-gpios",
							     0);
		if (!gpio_is_valid(phy->common.gpio_fw_wake))
			return -ENODEV;
	}
>>>>>>> upstream/android-13

	return 0;
}

static int s3fwrn5_i2c_probe(struct i2c_client *client,
				  const struct i2c_device_id *id)
{
	struct s3fwrn5_i2c_phy *phy;
	int ret;

	phy = devm_kzalloc(&client->dev, sizeof(*phy), GFP_KERNEL);
	if (!phy)
		return -ENOMEM;

<<<<<<< HEAD
	mutex_init(&phy->mutex);
	phy->mode = S3FWRN5_MODE_COLD;
=======
	mutex_init(&phy->common.mutex);
	phy->common.mode = S3FWRN5_MODE_COLD;
>>>>>>> upstream/android-13
	phy->irq_skip = true;

	phy->i2c_dev = client;
	i2c_set_clientdata(client, phy);

	ret = s3fwrn5_i2c_parse_dt(client);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	ret = devm_gpio_request_one(&phy->i2c_dev->dev, phy->gpio_en,
		GPIOF_OUT_INIT_HIGH, "s3fwrn5_en");
	if (ret < 0)
		return ret;

	ret = devm_gpio_request_one(&phy->i2c_dev->dev, phy->gpio_fw_wake,
		GPIOF_OUT_INIT_LOW, "s3fwrn5_fw_wake");
	if (ret < 0)
		return ret;

	ret = s3fwrn5_probe(&phy->ndev, phy, &phy->i2c_dev->dev, &i2c_phy_ops,
		S3FWRN5_I2C_MAX_PAYLOAD);
	if (ret < 0)
		return ret;

	ret = devm_request_threaded_irq(&client->dev, phy->i2c_dev->irq, NULL,
		s3fwrn5_i2c_irq_thread_fn, IRQF_TRIGGER_HIGH | IRQF_ONESHOT,
		S3FWRN5_I2C_DRIVER_NAME, phy);
	if (ret)
		s3fwrn5_remove(phy->ndev);

=======
	ret = devm_gpio_request_one(&phy->i2c_dev->dev, phy->common.gpio_en,
				    GPIOF_OUT_INIT_HIGH, "s3fwrn5_en");
	if (ret < 0)
		return ret;

	ret = devm_gpio_request_one(&phy->i2c_dev->dev,
				    phy->common.gpio_fw_wake,
				    GPIOF_OUT_INIT_LOW, "s3fwrn5_fw_wake");
	if (ret < 0)
		return ret;

	phy->clk = devm_clk_get_optional(&client->dev, NULL);
	if (IS_ERR(phy->clk))
		return dev_err_probe(&client->dev, PTR_ERR(phy->clk),
				     "failed to get clock\n");

	/*
	 * S3FWRN5 depends on a clock input ("XI" pin) to function properly.
	 * Depending on the hardware configuration this could be an always-on
	 * oscillator or some external clock that must be explicitly enabled.
	 * Make sure the clock is running before starting S3FWRN5.
	 */
	ret = clk_prepare_enable(phy->clk);
	if (ret < 0) {
		dev_err(&client->dev, "failed to enable clock: %d\n", ret);
		return ret;
	}

	ret = s3fwrn5_probe(&phy->common.ndev, phy, &phy->i2c_dev->dev,
			    &i2c_phy_ops);
	if (ret < 0)
		goto disable_clk;

	ret = devm_request_threaded_irq(&client->dev, phy->i2c_dev->irq, NULL,
		s3fwrn5_i2c_irq_thread_fn, IRQF_ONESHOT,
		S3FWRN5_I2C_DRIVER_NAME, phy);
	if (ret)
		goto s3fwrn5_remove;

	return 0;

s3fwrn5_remove:
	s3fwrn5_remove(phy->common.ndev);
disable_clk:
	clk_disable_unprepare(phy->clk);
>>>>>>> upstream/android-13
	return ret;
}

static int s3fwrn5_i2c_remove(struct i2c_client *client)
{
	struct s3fwrn5_i2c_phy *phy = i2c_get_clientdata(client);

<<<<<<< HEAD
	s3fwrn5_remove(phy->ndev);
=======
	s3fwrn5_remove(phy->common.ndev);
	clk_disable_unprepare(phy->clk);
>>>>>>> upstream/android-13

	return 0;
}

static const struct i2c_device_id s3fwrn5_i2c_id_table[] = {
	{S3FWRN5_I2C_DRIVER_NAME, 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, s3fwrn5_i2c_id_table);

<<<<<<< HEAD
static const struct of_device_id of_s3fwrn5_i2c_match[] = {
=======
static const struct of_device_id of_s3fwrn5_i2c_match[] __maybe_unused = {
>>>>>>> upstream/android-13
	{ .compatible = "samsung,s3fwrn5-i2c", },
	{}
};
MODULE_DEVICE_TABLE(of, of_s3fwrn5_i2c_match);

static struct i2c_driver s3fwrn5_i2c_driver = {
	.driver = {
<<<<<<< HEAD
		.owner = THIS_MODULE,
=======
>>>>>>> upstream/android-13
		.name = S3FWRN5_I2C_DRIVER_NAME,
		.of_match_table = of_match_ptr(of_s3fwrn5_i2c_match),
	},
	.probe = s3fwrn5_i2c_probe,
	.remove = s3fwrn5_i2c_remove,
	.id_table = s3fwrn5_i2c_id_table,
};

module_i2c_driver(s3fwrn5_i2c_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("I2C driver for Samsung S3FWRN5");
MODULE_AUTHOR("Robert Baldyga <r.baldyga@samsung.com>");
