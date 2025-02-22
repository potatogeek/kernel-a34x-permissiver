
#ifndef _I2C_PXA_H_
#define _I2C_PXA_H_

#if 0
#define DEF_TIMEOUT             3
#else

#define DEF_TIMEOUT             32
#endif

#define BUS_ERROR               (-EREMOTEIO)
#define XFER_NAKED              (-ECONNREFUSED)
#define I2C_RETRY               (-2000) 


#define I2C_ICR_INIT	(ICR_BEIE | ICR_IRFIE | ICR_ITEIE | ICR_GCD | ICR_SCLE)


#define I2C_ISR_INIT	0x7FF  

struct i2c_slave_client;

struct i2c_pxa_platform_data {
	unsigned int		slave_addr;
	struct i2c_slave_client	*slave;
	unsigned int		class;
	unsigned int		use_pio :1;
	unsigned int		fast_mode :1;
	unsigned int		high_mode:1;
	unsigned char		master_code;
	unsigned long		rate;
};
#endif
