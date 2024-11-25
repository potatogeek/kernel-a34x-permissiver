// SPDX-License-Identifier: GPL-2.0
#include <net/mac80211.h>
#include <linux/bcma/bcma_driver_chipcommon.h>
<<<<<<< HEAD
#include <linux/gpio.h>
=======
#include <linux/gpio/driver.h>
#include <linux/gpio/machine.h>
#include <linux/gpio/consumer.h>
>>>>>>> upstream/android-13

#include "mac80211_if.h"
#include "pub.h"
#include "main.h"
#include "led.h"

	/* number of leds */
#define  BRCMS_LED_NO		4
	/* behavior mask */
#define  BRCMS_LED_BEH_MASK	0x7f
	/* activelow (polarity) bit */
#define  BRCMS_LED_AL_MASK	0x80
	/* radio enabled */
#define  BRCMS_LED_RADIO	3

static void brcms_radio_led_ctrl(struct brcms_info *wl, bool state)
{
<<<<<<< HEAD
	if (wl->radio_led.gpio == -1)
		return;

	if (wl->radio_led.active_low)
		state = !state;

	if (state)
		gpio_set_value(wl->radio_led.gpio, 1);
	else
		gpio_set_value(wl->radio_led.gpio, 0);
=======
	if (!wl->radio_led.gpiod)
		return;

	if (state)
		gpiod_set_value(wl->radio_led.gpiod, 1);
	else
		gpiod_set_value(wl->radio_led.gpiod, 0);
>>>>>>> upstream/android-13
}


/* Callback from the LED subsystem. */
static void brcms_led_brightness_set(struct led_classdev *led_dev,
				   enum led_brightness brightness)
{
	struct brcms_info *wl = container_of(led_dev,
		struct brcms_info, led_dev);
	brcms_radio_led_ctrl(wl, brightness);
}

void brcms_led_unregister(struct brcms_info *wl)
{
	if (wl->led_dev.dev)
		led_classdev_unregister(&wl->led_dev);
<<<<<<< HEAD
	if (wl->radio_led.gpio != -1)
		gpio_free(wl->radio_led.gpio);
=======
	if (wl->radio_led.gpiod)
		gpiochip_free_own_desc(wl->radio_led.gpiod);
>>>>>>> upstream/android-13
}

int brcms_led_register(struct brcms_info *wl)
{
	int i, err;
	struct brcms_led *radio_led = &wl->radio_led;
	/* get CC core */
	struct bcma_drv_cc *cc_drv  = &wl->wlc->hw->d11core->bus->drv_cc;
	struct gpio_chip *bcma_gpio = &cc_drv->gpio;
	struct ssb_sprom *sprom = &wl->wlc->hw->d11core->bus->sprom;
	u8 *leds[] = { &sprom->gpio0,
		&sprom->gpio1,
		&sprom->gpio2,
		&sprom->gpio3 };
<<<<<<< HEAD
	unsigned gpio = -1;
	bool active_low = false;

	/* none by default */
	radio_led->gpio = -1;
	radio_led->active_low = false;
=======
	int hwnum = -1;
	enum gpio_lookup_flags lflags = GPIO_ACTIVE_HIGH;
>>>>>>> upstream/android-13

	if (!bcma_gpio || !gpio_is_valid(bcma_gpio->base))
		return -ENODEV;

	/* find radio enabled LED */
	for (i = 0; i < BRCMS_LED_NO; i++) {
		u8 led = *leds[i];
		if ((led & BRCMS_LED_BEH_MASK) == BRCMS_LED_RADIO) {
<<<<<<< HEAD
			gpio = bcma_gpio->base + i;
			if (led & BRCMS_LED_AL_MASK)
				active_low = true;
=======
			hwnum = i;
			if (led & BRCMS_LED_AL_MASK)
				lflags = GPIO_ACTIVE_LOW;
>>>>>>> upstream/android-13
			break;
		}
	}

<<<<<<< HEAD
	if (gpio == -1 || !gpio_is_valid(gpio))
		return -ENODEV;

	/* request and configure LED gpio */
	err = gpio_request_one(gpio,
				active_low ? GPIOF_OUT_INIT_HIGH
					: GPIOF_OUT_INIT_LOW,
				"radio on");
	if (err) {
		wiphy_err(wl->wiphy, "requesting led gpio %d failed (err: %d)\n",
			  gpio, err);
		return err;
	}
	err = gpio_direction_output(gpio, 1);
	if (err) {
		wiphy_err(wl->wiphy, "cannot set led gpio %d to output (err: %d)\n",
			  gpio, err);
=======
	/* No LED, bail out */
	if (hwnum == -1)
		return -ENODEV;

	/* Try to obtain this LED GPIO line */
	radio_led->gpiod = gpiochip_request_own_desc(bcma_gpio, hwnum,
						     "radio on", lflags,
						     GPIOD_OUT_LOW);

	if (IS_ERR(radio_led->gpiod)) {
		err = PTR_ERR(radio_led->gpiod);
		wiphy_err(wl->wiphy, "requesting led GPIO failed (err: %d)\n",
			  err);
>>>>>>> upstream/android-13
		return err;
	}

	snprintf(wl->radio_led.name, sizeof(wl->radio_led.name),
		 "brcmsmac-%s:radio", wiphy_name(wl->wiphy));

	wl->led_dev.name = wl->radio_led.name;
	wl->led_dev.default_trigger =
		ieee80211_get_radio_led_name(wl->pub->ieee_hw);
	wl->led_dev.brightness_set = brcms_led_brightness_set;
	err = led_classdev_register(wiphy_dev(wl->wiphy), &wl->led_dev);

	if (err) {
		wiphy_err(wl->wiphy, "cannot register led device: %s (err: %d)\n",
			  wl->radio_led.name, err);
		return err;
	}

<<<<<<< HEAD
	wiphy_info(wl->wiphy, "registered radio enabled led device: %s gpio: %d\n",
		   wl->radio_led.name,
		   gpio);
	radio_led->gpio = gpio;
	radio_led->active_low = active_low;
=======
	wiphy_info(wl->wiphy, "registered radio enabled led device: %s\n",
		   wl->radio_led.name);
>>>>>>> upstream/android-13

	return 0;
}
