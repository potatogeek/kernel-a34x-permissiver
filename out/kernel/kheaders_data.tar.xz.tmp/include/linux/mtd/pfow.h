/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __LINUX_MTD_PFOW_H
#define __LINUX_MTD_PFOW_H

#include <linux/mtd/qinfo.h>



#define PFOW_QUERY_STRING_P			0x0000

#define PFOW_QUERY_STRING_F			0x0002

#define PFOW_QUERY_STRING_O			0x0004

#define PFOW_QUERY_STRING_W			0x0006

#define PFOW_MANUFACTURER_ID			0x0020
#define PFOW_DEVICE_ID				0x0022

#define PFOW_PROGRAM_BUFFER_OFFSET		0x0040

#define PFOW_PROGRAM_BUFFER_SIZE		0x0042

#define PFOW_COMMAND_CODE			0x0080

#define PFOW_COMMAND_DATA			0x0084

#define PFOW_COMMAND_ADDRESS_L			0x0088

#define PFOW_COMMAND_ADDRESS_H			0x008a

#define PFOW_DATA_COUNT_L			0x0090

#define PFOW_DATA_COUNT_H			0x0092

#define PFOW_COMMAND_EXECUTE			0x00c0

#define PFOW_CLEAR_PROGRAM_BUFFER		0x00c4

#define PFOW_PROGRAM_ERASE_SUSPEND		0x00c8

#define PFOW_DSR				0x00cc



#define LPDDR_WORD_PROGRAM		0x0041
#define LPDDR_BUFF_PROGRAM		0x00E9
#define LPDDR_BLOCK_ERASE		0x0020
#define LPDDR_LOCK_BLOCK		0x0061
#define LPDDR_UNLOCK_BLOCK		0x0062
#define LPDDR_READ_BLOCK_LOCK_STATUS	0x0065
#define LPDDR_INFO_QUERY		0x0098
#define LPDDR_READ_OTP			0x0097
#define LPDDR_PROG_OTP			0x00C0
#define LPDDR_RESUME			0x00D0


#define LPDDR_START_EXECUTION			0x0001


#define LPDDR_SUSPEND				0x0001



#define DSR_DPS			(1<<1) 
#define DSR_PSS			(1<<2) 
#define DSR_VPPS		(1<<3) 
#define DSR_PROGRAM_STATUS	(1<<4) 
#define DSR_ERASE_STATUS	(1<<5) 
#define DSR_ESS			(1<<6) 
#define DSR_READY_STATUS	(1<<7) 
#define DSR_RPS			(0x3<<8) 
#define DSR_AOS			(1<<12) 
#define DSR_AVAILABLE		(1<<15) 


#define DSR_ERR			0x133A

static inline void send_pfow_command(struct map_info *map,
				unsigned long cmd_code, unsigned long adr,
				unsigned long len, map_word *datum)
{
	int bits_per_chip = map_bankwidth(map) * 8;

	map_write(map, CMD(cmd_code), map->pfow_base + PFOW_COMMAND_CODE);
	map_write(map, CMD(adr & ((1<<bits_per_chip) - 1)),
				map->pfow_base + PFOW_COMMAND_ADDRESS_L);
	map_write(map, CMD(adr>>bits_per_chip),
				map->pfow_base + PFOW_COMMAND_ADDRESS_H);
	if (len) {
		map_write(map, CMD(len & ((1<<bits_per_chip) - 1)),
					map->pfow_base + PFOW_DATA_COUNT_L);
		map_write(map, CMD(len>>bits_per_chip),
					map->pfow_base + PFOW_DATA_COUNT_H);
	}
	if (datum)
		map_write(map, *datum, map->pfow_base + PFOW_COMMAND_DATA);

	
	map_write(map, CMD(LPDDR_START_EXECUTION),
			map->pfow_base + PFOW_COMMAND_EXECUTE);
}

static inline void print_drs_error(unsigned dsr)
{
	int prog_status = (dsr & DSR_RPS) >> 8;

	if (!(dsr & DSR_AVAILABLE))
		printk(KERN_NOTICE"DSR.15: (0) Device not Available\n");
	if ((prog_status & 0x03) == 0x03)
		printk(KERN_NOTICE"DSR.9,8: (11) Attempt to program invalid "
						"half with 41h command\n");
	else if (prog_status & 0x02)
		printk(KERN_NOTICE"DSR.9,8: (10) Object Mode Program attempt "
					"in region with Control Mode data\n");
	else if (prog_status &  0x01)
		printk(KERN_NOTICE"DSR.9,8: (01) Program attempt in region "
						"with Object Mode data\n");
	if (!(dsr & DSR_READY_STATUS))
		printk(KERN_NOTICE"DSR.7: (0) Device is Busy\n");
	if (dsr & DSR_ESS)
		printk(KERN_NOTICE"DSR.6: (1) Erase Suspended\n");
	if (dsr & DSR_ERASE_STATUS)
		printk(KERN_NOTICE"DSR.5: (1) Erase/Blank check error\n");
	if (dsr & DSR_PROGRAM_STATUS)
		printk(KERN_NOTICE"DSR.4: (1) Program Error\n");
	if (dsr & DSR_VPPS)
		printk(KERN_NOTICE"DSR.3: (1) Vpp low detect, operation "
					"aborted\n");
	if (dsr & DSR_PSS)
		printk(KERN_NOTICE"DSR.2: (1) Program suspended\n");
	if (dsr & DSR_DPS)
		printk(KERN_NOTICE"DSR.1: (1) Aborted Erase/Program attempt "
					"on locked block\n");
}
#endif 
