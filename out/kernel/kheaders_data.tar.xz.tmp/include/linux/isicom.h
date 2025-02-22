/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_ISICOM_H
#define _LINUX_ISICOM_H

#define		YES	1
#define		NO	0



#define		ISICOM_NAME	"ISICom"



#define		DEVID_COUNT	9
#define		VENDOR_ID	0x10b5



#define		ISICOM_NMAJOR	112	
#define		ISICOM_CMAJOR	113	
#define		ISICOM_MAGIC	(('M' << 8) | 'T')

#define		WAKEUP_CHARS	256	
#define		TX_SIZE		254

#define		BOARD_COUNT	4
#define		PORT_COUNT	(BOARD_COUNT*16)



#define		ISICOM_CS5		0x0000
#define		ISICOM_CS6		0x0001
#define		ISICOM_CS7		0x0002
#define		ISICOM_CS8		0x0003



#define		ISICOM_1SB		0x0000
#define		ISICOM_2SB		0x0004



#define		ISICOM_NOPAR		0x0000
#define		ISICOM_ODPAR		0x0008
#define		ISICOM_EVPAR		0x0018



#define		ISICOM_CTSRTS		0x03
#define		ISICOM_INITIATE_XONXOFF	0x04
#define		ISICOM_RESPOND_XONXOFF	0x08

#define	BOARD(line)  (((line) >> 4) & 0x3)

	

#define		ISICOM_KILLTX		0x01
#define		ISICOM_KILLRX		0x02

	

#define		FIRMWARE_LOADED		0x0001
#define		BOARD_ACTIVE		0x0002
#define		BOARD_INIT		0x0004

 	

#define		ISI_CTS			0x1000
#define		ISI_DSR			0x2000
#define		ISI_RI			0x4000
#define		ISI_DCD			0x8000
#define		ISI_DTR			0x0100
#define		ISI_RTS			0x0200


#define		ISI_TXOK		0x0001

#endif	
