

#ifndef _FC_MS_H_
#define	_FC_MS_H_

#include <linux/types.h>






#define	FC_FDMI_SUBTYPE	    0x10 


enum fc_fdmi_req {
	FC_FDMI_GRHL = 0x0100,	
	FC_FDMI_GHAT = 0x0101,	
	FC_FDMI_GRPL = 0x0102,	
	FC_FDMI_GPAT = 0x0110,	
	FC_FDMI_RHBA = 0x0200,	
	FC_FDMI_RHAT = 0x0201,	
	FC_FDMI_RPRT = 0x0210,	
	FC_FDMI_RPA = 0x0211,	
	FC_FDMI_DHBA = 0x0300,	
	FC_FDMI_DHAT = 0x0301,	
	FC_FDMI_DPRT = 0x0310,	
	FC_FDMI_DPA = 0x0311,	
};


enum fc_fdmi_hba_attr_type {
	FC_FDMI_HBA_ATTR_NODENAME = 0x0001,
	FC_FDMI_HBA_ATTR_MANUFACTURER = 0x0002,
	FC_FDMI_HBA_ATTR_SERIALNUMBER = 0x0003,
	FC_FDMI_HBA_ATTR_MODEL = 0x0004,
	FC_FDMI_HBA_ATTR_MODELDESCRIPTION = 0x0005,
	FC_FDMI_HBA_ATTR_HARDWAREVERSION = 0x0006,
	FC_FDMI_HBA_ATTR_DRIVERVERSION = 0x0007,
	FC_FDMI_HBA_ATTR_OPTIONROMVERSION = 0x0008,
	FC_FDMI_HBA_ATTR_FIRMWAREVERSION = 0x0009,
	FC_FDMI_HBA_ATTR_OSNAMEVERSION = 0x000A,
	FC_FDMI_HBA_ATTR_MAXCTPAYLOAD = 0x000B,
};


#define FC_FDMI_HBA_ATTR_NODENAME_LEN		8
#define FC_FDMI_HBA_ATTR_MANUFACTURER_LEN	64
#define FC_FDMI_HBA_ATTR_SERIALNUMBER_LEN	64
#define FC_FDMI_HBA_ATTR_MODEL_LEN		256
#define FC_FDMI_HBA_ATTR_MODELDESCR_LEN		256
#define FC_FDMI_HBA_ATTR_HARDWAREVERSION_LEN	256
#define FC_FDMI_HBA_ATTR_DRIVERVERSION_LEN	256
#define FC_FDMI_HBA_ATTR_OPTIONROMVERSION_LEN	256
#define FC_FDMI_HBA_ATTR_FIRMWAREVERSION_LEN	256
#define FC_FDMI_HBA_ATTR_OSNAMEVERSION_LEN	256
#define FC_FDMI_HBA_ATTR_MAXCTPAYLOAD_LEN	4


enum fc_fdmi_port_attr_type {
	FC_FDMI_PORT_ATTR_FC4TYPES = 0x0001,
	FC_FDMI_PORT_ATTR_SUPPORTEDSPEED = 0x0002,
	FC_FDMI_PORT_ATTR_CURRENTPORTSPEED = 0x0003,
	FC_FDMI_PORT_ATTR_MAXFRAMESIZE = 0x0004,
	FC_FDMI_PORT_ATTR_OSDEVICENAME = 0x0005,
	FC_FDMI_PORT_ATTR_HOSTNAME = 0x0006,
};


#define FC_FDMI_PORT_ATTR_FC4TYPES_LEN		32
#define FC_FDMI_PORT_ATTR_SUPPORTEDSPEED_LEN	4
#define FC_FDMI_PORT_ATTR_CURRENTPORTSPEED_LEN	4
#define FC_FDMI_PORT_ATTR_MAXFRAMESIZE_LEN	4
#define FC_FDMI_PORT_ATTR_OSDEVICENAME_LEN	256
#define FC_FDMI_PORT_ATTR_HOSTNAME_LEN		256


struct fc_fdmi_hba_identifier {
	__be64		id;
};


struct fc_fdmi_port_name {
	__be64		portname;
};


#define FC_FDMI_ATTR_ENTRY_HEADER_LEN	4
struct fc_fdmi_attr_entry {
	__be16		type;
	__be16		len;
	__u8		value[1];
} __attribute__((__packed__));


struct fs_fdmi_attrs {
	__be32				numattrs;
	struct fc_fdmi_attr_entry	attr[1];
} __attribute__((__packed__));


struct fc_fdmi_rpl {
	__be32				numport;
	struct fc_fdmi_port_name	port[1];
} __attribute__((__packed__));


struct fc_fdmi_rhba {
	struct fc_fdmi_hba_identifier hbaid;
	struct fc_fdmi_rpl		 port;
	struct fs_fdmi_attrs		 hba_attrs;
} __attribute__((__packed__));


struct fc_fdmi_rhat {
	struct fc_fdmi_hba_identifier hbaid;
	struct fs_fdmi_attrs		 hba_attrs;
} __attribute__((__packed__));


struct fc_fdmi_rprt {
	struct fc_fdmi_hba_identifier hbaid;
	struct fc_fdmi_port_name	 port;
	struct fs_fdmi_attrs		 hba_attrs;
} __attribute__((__packed__));


struct fc_fdmi_rpa {
	struct fc_fdmi_port_name	 port;
	struct fs_fdmi_attrs		 hba_attrs;
} __attribute__((__packed__));


struct fc_fdmi_dprt {
	struct fc_fdmi_port_name	 port;
} __attribute__((__packed__));


struct fc_fdmi_dpa {
	struct fc_fdmi_port_name	 port;
	struct fs_fdmi_attrs		 hba_attrs;
} __attribute__((__packed__));


struct fc_fdmi_dhat {
	struct fc_fdmi_hba_identifier hbaid;
} __attribute__((__packed__));


struct fc_fdmi_dhba {
	struct fc_fdmi_hba_identifier hbaid;
} __attribute__((__packed__));

#endif 
