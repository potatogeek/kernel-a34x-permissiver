/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_SPI_EEPROM_H
#define __LINUX_SPI_EEPROM_H

#include <linux/memory.h>


struct spi_eeprom {
	u32		byte_len;
	char		name[10];
	u16		page_size;		
	u16		flags;
#define	EE_ADDR1	0x0001			
#define	EE_ADDR2	0x0002			
#define	EE_ADDR3	0x0004			
#define	EE_READONLY	0x0008			

	
#define EE_INSTR_BIT3_IS_ADDR	0x0010

	void *context;
};

#endif 
