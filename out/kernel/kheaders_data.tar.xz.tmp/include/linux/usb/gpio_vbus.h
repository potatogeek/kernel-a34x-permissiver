// SPDX-License-Identifier: GPL-2.0



struct gpio_vbus_mach_info {
	int gpio_vbus;
	int gpio_pullup;
	bool gpio_vbus_inverted;
	bool gpio_pullup_inverted;
	bool wakeup;
};
