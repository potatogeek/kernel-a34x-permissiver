

#ifndef __ISL9305_H
#define __ISL9305_H

#define ISL9305_DCD1 0
#define ISL9305_DCD2 1
#define ISL9305_LDO1 2
#define ISL9305_LDO2 3

#define ISL9305_MAX_REGULATOR ISL9305_LDO2

struct regulator_init_data;

struct isl9305_pdata {
	struct regulator_init_data *init_data[ISL9305_MAX_REGULATOR + 1];
};

#endif
