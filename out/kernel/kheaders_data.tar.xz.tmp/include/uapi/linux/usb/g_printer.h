/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */


#ifndef __LINUX_USB_G_PRINTER_H
#define __LINUX_USB_G_PRINTER_H

#define PRINTER_NOT_ERROR	0x08
#define PRINTER_SELECTED	0x10
#define PRINTER_PAPER_EMPTY	0x20


#define GADGET_GET_PRINTER_STATUS	_IOR('g', 0x21, unsigned char)
#define GADGET_SET_PRINTER_STATUS	_IOWR('g', 0x22, unsigned char)

#endif 
