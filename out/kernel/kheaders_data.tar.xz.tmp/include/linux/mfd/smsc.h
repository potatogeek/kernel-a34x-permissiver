

#ifndef __LINUX_MFD_SMSC_H
#define __LINUX_MFD_SMSC_H

#include <linux/regmap.h>

#define SMSC_ID_ECE1099			1
#define SMSC_NUM_CLIENTS		2

#define SMSC_BASE_ADDR			0x38
#define OMAP_GPIO_SMSC_IRQ		151

#define SMSC_MAXGPIO         32
#define SMSC_BANK(offs)      ((offs) >> 3)
#define SMSC_BIT(offs)       (1u << ((offs) & 0x7))

struct smsc {
	struct device *dev;
	struct i2c_client *i2c_clients[SMSC_NUM_CLIENTS];
	struct regmap *regmap;
	int clk;
	
	int id;
};

struct smsc_gpio;
struct smsc_keypad;

static inline int smsc_read(struct device *child, unsigned int reg,
	unsigned int *dest)
{
	struct smsc     *smsc = dev_get_drvdata(child->parent);

	return regmap_read(smsc->regmap, reg, dest);
}

static inline int smsc_write(struct device *child, unsigned int reg,
	unsigned int value)
{
	struct smsc     *smsc = dev_get_drvdata(child->parent);

	return regmap_write(smsc->regmap, reg, value);
}


#define SMSC_RESET						0xF5
#define SMSC_GRP_INT						0xF9
#define SMSC_CLK_CTRL						0xFA
#define SMSC_WKUP_CTRL						0xFB
#define SMSC_DEV_ID						0xFC
#define SMSC_DEV_REV						0xFD
#define SMSC_VEN_ID_L						0xFE
#define SMSC_VEN_ID_H						0xFF


#define SMSC_CLK_VALUE						0x13


#define SMSC_GPIO_DATA_IN_START					0x00


#define SMSC_GPIO_DATA_OUT_START                                       0x05


#define SMSC_GPIO_INPUT_LOW					0x01
#define SMSC_GPIO_INPUT_RISING					0x09
#define SMSC_GPIO_INPUT_FALLING					0x11
#define SMSC_GPIO_INPUT_BOTH_EDGE				0x19
#define SMSC_GPIO_OUTPUT_PP					0x21
#define SMSC_GPIO_OUTPUT_OP					0x31

#define GRP_INT_STAT						0xf9
#define	SMSC_GPI_INT						0x0f
#define SMSC_CFG_START						0x0A


#define SMSC_GPIO_INT_STAT_START                                  0x32


#define SMSC_GPIO_INT_MASK_START                               0x37


#define SMSC_KP_OUT						0x40
#define SMSC_KP_IN						0x41
#define SMSC_KP_INT_STAT					0x42
#define SMSC_KP_INT_MASK					0x43


#define SMSC_KP_KSO           0x70
#define SMSC_KP_KSI           0x51
#define SMSC_KSO_ALL_LOW        0x20
#define SMSC_KP_SET_LOW_PWR        0x0B
#define SMSC_KP_SET_HIGH           0xFF
#define SMSC_KSO_EVAL           0x00

#endif 
