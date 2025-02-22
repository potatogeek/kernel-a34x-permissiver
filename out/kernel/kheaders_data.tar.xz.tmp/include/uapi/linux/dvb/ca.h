/* SPDX-License-Identifier: LGPL-2.1+ WITH Linux-syscall-note */


#ifndef _DVBCA_H_
#define _DVBCA_H_



struct ca_slot_info {
	int num;
	int type;
#define CA_CI            1
#define CA_CI_LINK       2
#define CA_CI_PHYS       4
#define CA_DESCR         8
#define CA_SC          128

	unsigned int flags;
#define CA_CI_MODULE_PRESENT 1
#define CA_CI_MODULE_READY   2
};



struct ca_descr_info {
	unsigned int num;
	unsigned int type;
#define CA_ECD           1
#define CA_NDS           2
#define CA_DSS           4
};


struct ca_caps {
	unsigned int slot_num;
	unsigned int slot_type;
	unsigned int descr_num;
	unsigned int descr_type;
};


struct ca_msg {
	unsigned int index;
	unsigned int type;
	unsigned int length;
	unsigned char msg[256];
};


struct ca_descr {
	unsigned int index;
	unsigned int parity;
	unsigned char cw[8];
};

#define CA_RESET          _IO('o', 128)
#define CA_GET_CAP        _IOR('o', 129, struct ca_caps)
#define CA_GET_SLOT_INFO  _IOR('o', 130, struct ca_slot_info)
#define CA_GET_DESCR_INFO _IOR('o', 131, struct ca_descr_info)
#define CA_GET_MSG        _IOR('o', 132, struct ca_msg)
#define CA_SEND_MSG       _IOW('o', 133, struct ca_msg)
#define CA_SET_DESCR      _IOW('o', 134, struct ca_descr)

#if !defined(__KERNEL__)


typedef struct ca_slot_info ca_slot_info_t;
typedef struct ca_descr_info  ca_descr_info_t;
typedef struct ca_caps  ca_caps_t;
typedef struct ca_msg ca_msg_t;
typedef struct ca_descr ca_descr_t;

#endif


#endif
