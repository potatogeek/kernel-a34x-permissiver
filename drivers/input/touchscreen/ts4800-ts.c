/*
 * Touchscreen driver for the TS-4800 board
 *
 * Copyright (c) 2015 - Savoir-faire Linux
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/bitops.h>
#include <linux/input.h>
<<<<<<< HEAD
#include <linux/input-polldev.h>
=======
>>>>>>> upstream/android-13
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>

/* polling interval in ms */
#define POLL_INTERVAL		3

#define DEBOUNCE_COUNT		1

/* sensor values are 12-bit wide */
#define MAX_12BIT		((1 << 12) - 1)

#define PENDOWN_MASK		0x1

#define X_OFFSET		0x0
#define Y_OFFSET		0x2

struct ts4800_ts {
<<<<<<< HEAD
	struct input_polled_dev *poll_dev;
=======
	struct input_dev        *input;
>>>>>>> upstream/android-13
	struct device           *dev;
	char                    phys[32];

	void __iomem            *base;
	struct regmap           *regmap;
	unsigned int            reg;
	unsigned int            bit;

	bool                    pendown;
	int                     debounce;
};

<<<<<<< HEAD
static void ts4800_ts_open(struct input_polled_dev *dev)
{
	struct ts4800_ts *ts = dev->private;
	int ret;
=======
static int ts4800_ts_open(struct input_dev *input_dev)
{
	struct ts4800_ts *ts = input_get_drvdata(input_dev);
	int error;
>>>>>>> upstream/android-13

	ts->pendown = false;
	ts->debounce = DEBOUNCE_COUNT;

<<<<<<< HEAD
	ret = regmap_update_bits(ts->regmap, ts->reg, ts->bit, ts->bit);
	if (ret)
		dev_warn(ts->dev, "Failed to enable touchscreen\n");
}

static void ts4800_ts_close(struct input_polled_dev *dev)
{
	struct ts4800_ts *ts = dev->private;
=======
	error = regmap_update_bits(ts->regmap, ts->reg, ts->bit, ts->bit);
	if (error) {
		dev_warn(ts->dev, "Failed to enable touchscreen: %d\n", error);
		return error;
	}

	return 0;
}

static void ts4800_ts_close(struct input_dev *input_dev)
{
	struct ts4800_ts *ts = input_get_drvdata(input_dev);
>>>>>>> upstream/android-13
	int ret;

	ret = regmap_update_bits(ts->regmap, ts->reg, ts->bit, 0);
	if (ret)
		dev_warn(ts->dev, "Failed to disable touchscreen\n");

}

<<<<<<< HEAD
static void ts4800_ts_poll(struct input_polled_dev *dev)
{
	struct input_dev *input_dev = dev->input;
	struct ts4800_ts *ts = dev->private;
=======
static void ts4800_ts_poll(struct input_dev *input_dev)
{
	struct ts4800_ts *ts = input_get_drvdata(input_dev);
>>>>>>> upstream/android-13
	u16 last_x = readw(ts->base + X_OFFSET);
	u16 last_y = readw(ts->base + Y_OFFSET);
	bool pendown = last_x & PENDOWN_MASK;

	if (pendown) {
		if (ts->debounce) {
			ts->debounce--;
			return;
		}

		if (!ts->pendown) {
			input_report_key(input_dev, BTN_TOUCH, 1);
			ts->pendown = true;
		}

		last_x = ((~last_x) >> 4) & MAX_12BIT;
		last_y = ((~last_y) >> 4) & MAX_12BIT;

		input_report_abs(input_dev, ABS_X, last_x);
		input_report_abs(input_dev, ABS_Y, last_y);
		input_sync(input_dev);
	} else if (ts->pendown) {
		ts->pendown = false;
		ts->debounce = DEBOUNCE_COUNT;
		input_report_key(input_dev, BTN_TOUCH, 0);
		input_sync(input_dev);
	}
}

static int ts4800_parse_dt(struct platform_device *pdev,
			   struct ts4800_ts *ts)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct device_node *syscon_np;
	u32 reg, bit;
	int error;

	syscon_np = of_parse_phandle(np, "syscon", 0);
	if (!syscon_np) {
		dev_err(dev, "no syscon property\n");
		return -ENODEV;
	}

	ts->regmap = syscon_node_to_regmap(syscon_np);
	of_node_put(syscon_np);
	if (IS_ERR(ts->regmap)) {
		dev_err(dev, "cannot get parent's regmap\n");
		return PTR_ERR(ts->regmap);
	}

	error = of_property_read_u32_index(np, "syscon", 1, &reg);
	if (error < 0) {
		dev_err(dev, "no offset in syscon\n");
		return error;
	}

	ts->reg = reg;

	error = of_property_read_u32_index(np, "syscon", 2, &bit);
	if (error < 0) {
		dev_err(dev, "no bit in syscon\n");
		return error;
	}

	ts->bit = BIT(bit);

	return 0;
}

static int ts4800_ts_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct input_polled_dev *poll_dev;
	struct ts4800_ts *ts;
	struct resource *res;
=======
	struct input_dev *input_dev;
	struct ts4800_ts *ts;
>>>>>>> upstream/android-13
	int error;

	ts = devm_kzalloc(&pdev->dev, sizeof(*ts), GFP_KERNEL);
	if (!ts)
		return -ENOMEM;

	error = ts4800_parse_dt(pdev, ts);
	if (error)
		return error;

<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	ts->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(ts->base))
		return PTR_ERR(ts->base);

	poll_dev = devm_input_allocate_polled_device(&pdev->dev);
	if (!poll_dev)
		return -ENOMEM;

	snprintf(ts->phys, sizeof(ts->phys), "%s/input0", dev_name(&pdev->dev));
	ts->poll_dev = poll_dev;
	ts->dev = &pdev->dev;

	poll_dev->private = ts;
	poll_dev->poll_interval = POLL_INTERVAL;
	poll_dev->open = ts4800_ts_open;
	poll_dev->close = ts4800_ts_close;
	poll_dev->poll = ts4800_ts_poll;

	poll_dev->input->name = "TS-4800 Touchscreen";
	poll_dev->input->phys = ts->phys;

	input_set_capability(poll_dev->input, EV_KEY, BTN_TOUCH);
	input_set_abs_params(poll_dev->input, ABS_X, 0, MAX_12BIT, 0, 0);
	input_set_abs_params(poll_dev->input, ABS_Y, 0, MAX_12BIT, 0, 0);

	error = input_register_polled_device(poll_dev);
	if (error) {
		dev_err(&pdev->dev,
			"Unabled to register polled input device (%d)\n",
			error);
=======
	ts->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(ts->base))
		return PTR_ERR(ts->base);

	input_dev = devm_input_allocate_device(&pdev->dev);
	if (!input_dev)
		return -ENOMEM;

	snprintf(ts->phys, sizeof(ts->phys), "%s/input0", dev_name(&pdev->dev));
	ts->input = input_dev;
	ts->dev = &pdev->dev;

	input_set_drvdata(input_dev, ts);

	input_dev->name = "TS-4800 Touchscreen";
	input_dev->phys = ts->phys;

	input_dev->open = ts4800_ts_open;
	input_dev->close = ts4800_ts_close;

	input_set_capability(input_dev, EV_KEY, BTN_TOUCH);
	input_set_abs_params(input_dev, ABS_X, 0, MAX_12BIT, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0, MAX_12BIT, 0, 0);

	error = input_setup_polling(input_dev, ts4800_ts_poll);
	if (error) {
		dev_err(&pdev->dev, "Unable to set up polling: %d\n", error);
		return error;
	}

	input_set_poll_interval(input_dev, POLL_INTERVAL);

	error = input_register_device(input_dev);
	if (error) {
		dev_err(&pdev->dev,
			"Unable to register input device: %d\n", error);
>>>>>>> upstream/android-13
		return error;
	}

	return 0;
}

static const struct of_device_id ts4800_ts_of_match[] = {
	{ .compatible = "technologic,ts4800-ts", },
	{ },
};
MODULE_DEVICE_TABLE(of, ts4800_ts_of_match);

static struct platform_driver ts4800_ts_driver = {
	.driver = {
		.name = "ts4800-ts",
		.of_match_table = ts4800_ts_of_match,
	},
	.probe = ts4800_ts_probe,
};
module_platform_driver(ts4800_ts_driver);

MODULE_AUTHOR("Damien Riegel <damien.riegel@savoirfairelinux.com>");
MODULE_DESCRIPTION("TS-4800 Touchscreen Driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:ts4800_ts");
