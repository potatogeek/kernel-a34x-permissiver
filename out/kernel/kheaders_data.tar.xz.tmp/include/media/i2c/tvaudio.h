

#ifndef _TVAUDIO_H
#define _TVAUDIO_H



#define I2C_ADDR_TDA8425	0x82
#define I2C_ADDR_TDA9840	0x84
#define I2C_ADDR_TDA9874	0xb0 
#define I2C_ADDR_TDA9875	0xb0
#define I2C_ADDR_TDA8425	0x82
#define I2C_ADDR_TDA9840	0x84 
#define I2C_ADDR_TDA985x_L	0xb4 
#define I2C_ADDR_TDA985x_H	0xb6
#define I2C_ADDR_TDA9874	0xb0 
#define I2C_ADDR_TEA6300	0x80 
#define I2C_ADDR_TEA6420	0x98
#define I2C_ADDR_PIC16C54	0x96 


#define TVAUDIO_INPUT_TUNER  0
#define TVAUDIO_INPUT_RADIO  1
#define TVAUDIO_INPUT_EXTERN 2
#define TVAUDIO_INPUT_INTERN 3

static inline const unsigned short *tvaudio_addrs(void)
{
	static const unsigned short addrs[] = {
		I2C_ADDR_TDA8425   >> 1,
		I2C_ADDR_TEA6300   >> 1,
		I2C_ADDR_TEA6420   >> 1,
		I2C_ADDR_TDA9840   >> 1,
		I2C_ADDR_TDA985x_L >> 1,
		I2C_ADDR_TDA985x_H >> 1,
		I2C_ADDR_TDA9874   >> 1,
		I2C_ADDR_PIC16C54  >> 1,
		I2C_CLIENT_END
	};

	return addrs;
}

#endif
