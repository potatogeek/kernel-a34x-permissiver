

#ifndef __LINUX_MFD_STMPE_H
#define __LINUX_MFD_STMPE_H

#include <linux/mutex.h>

struct device;
struct regulator;

enum stmpe_block {
	STMPE_BLOCK_GPIO	= 1 << 0,
	STMPE_BLOCK_KEYPAD	= 1 << 1,
	STMPE_BLOCK_TOUCHSCREEN	= 1 << 2,
	STMPE_BLOCK_ADC		= 1 << 3,
	STMPE_BLOCK_PWM		= 1 << 4,
	STMPE_BLOCK_ROTATOR	= 1 << 5,
};

enum stmpe_partnum {
	STMPE610,
	STMPE801,
	STMPE811,
	STMPE1600,
	STMPE1601,
	STMPE1801,
	STMPE2401,
	STMPE2403,
	STMPE_NBR_PARTS
};


enum {
	STMPE_IDX_CHIP_ID,
	STMPE_IDX_SYS_CTRL,
	STMPE_IDX_SYS_CTRL2,
	STMPE_IDX_ICR_LSB,
	STMPE_IDX_IER_LSB,
	STMPE_IDX_IER_MSB,
	STMPE_IDX_ISR_LSB,
	STMPE_IDX_ISR_MSB,
	STMPE_IDX_GPMR_LSB,
	STMPE_IDX_GPMR_CSB,
	STMPE_IDX_GPMR_MSB,
	STMPE_IDX_GPSR_LSB,
	STMPE_IDX_GPSR_CSB,
	STMPE_IDX_GPSR_MSB,
	STMPE_IDX_GPCR_LSB,
	STMPE_IDX_GPCR_CSB,
	STMPE_IDX_GPCR_MSB,
	STMPE_IDX_GPDR_LSB,
	STMPE_IDX_GPDR_CSB,
	STMPE_IDX_GPDR_MSB,
	STMPE_IDX_GPEDR_LSB,
	STMPE_IDX_GPEDR_CSB,
	STMPE_IDX_GPEDR_MSB,
	STMPE_IDX_GPRER_LSB,
	STMPE_IDX_GPRER_CSB,
	STMPE_IDX_GPRER_MSB,
	STMPE_IDX_GPFER_LSB,
	STMPE_IDX_GPFER_CSB,
	STMPE_IDX_GPFER_MSB,
	STMPE_IDX_GPPUR_LSB,
	STMPE_IDX_GPPDR_LSB,
	STMPE_IDX_GPAFR_U_MSB,
	STMPE_IDX_IEGPIOR_LSB,
	STMPE_IDX_IEGPIOR_CSB,
	STMPE_IDX_IEGPIOR_MSB,
	STMPE_IDX_ISGPIOR_LSB,
	STMPE_IDX_ISGPIOR_CSB,
	STMPE_IDX_ISGPIOR_MSB,
	STMPE_IDX_MAX,
};


struct stmpe_variant_info;
struct stmpe_client_info;
struct stmpe_platform_data;


struct stmpe {
	struct regulator *vcc;
	struct regulator *vio;
	struct mutex lock;
	struct mutex irq_lock;
	struct device *dev;
	struct irq_domain *domain;
	void *client;
	struct stmpe_client_info *ci;
	enum stmpe_partnum partnum;
	struct stmpe_variant_info *variant;
	const u8 *regs;

	int irq;
	int num_gpios;
	u8 ier[2];
	u8 oldier[2];
	struct stmpe_platform_data *pdata;
};

extern int stmpe_reg_write(struct stmpe *stmpe, u8 reg, u8 data);
extern int stmpe_reg_read(struct stmpe *stmpe, u8 reg);
extern int stmpe_block_read(struct stmpe *stmpe, u8 reg, u8 length,
			    u8 *values);
extern int stmpe_block_write(struct stmpe *stmpe, u8 reg, u8 length,
			     const u8 *values);
extern int stmpe_set_bits(struct stmpe *stmpe, u8 reg, u8 mask, u8 val);
extern int stmpe_set_altfunc(struct stmpe *stmpe, u32 pins,
			     enum stmpe_block block);
extern int stmpe_enable(struct stmpe *stmpe, unsigned int blocks);
extern int stmpe_disable(struct stmpe *stmpe, unsigned int blocks);

#define STMPE_GPIO_NOREQ_811_TOUCH	(0xf0)

#endif
