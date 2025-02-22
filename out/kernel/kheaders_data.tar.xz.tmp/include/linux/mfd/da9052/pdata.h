

#ifndef __MFD_DA9052_PDATA_H__
#define __MFD_DA9052_PDATA_H__

#define DA9052_MAX_REGULATORS	14

struct da9052;

struct da9052_pdata {
	struct led_platform_data *pled;
	int (*init) (struct da9052 *da9052);
	int irq_base;
	int gpio_base;
	int use_for_apm;
	struct regulator_init_data *regulators[DA9052_MAX_REGULATORS];
};

#endif
