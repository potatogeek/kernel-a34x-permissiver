// SPDX-License-Identifier: GPL-2.0

#ifndef __LINUX_USB_ASSOCIATION_H
#define __LINUX_USB_ASSOCIATION_H



struct wusb_am_attr {
	__u8 id;
	__u8 len;
};


#define WUSB_AR_AssociationTypeId	{ .id = cpu_to_le16(0x0000), .len = cpu_to_le16(2) }
#define WUSB_AR_AssociationSubTypeId	{ .id = cpu_to_le16(0x0001), .len = cpu_to_le16(2) }
#define WUSB_AR_Length			{ .id = cpu_to_le16(0x0002), .len = cpu_to_le16(4) }
#define WUSB_AR_AssociationStatus	{ .id = cpu_to_le16(0x0004), .len = cpu_to_le16(4) }
#define WUSB_AR_LangID			{ .id = cpu_to_le16(0x0008), .len = cpu_to_le16(2) }
#define WUSB_AR_DeviceFriendlyName	{ .id = cpu_to_le16(0x000b), .len = cpu_to_le16(64) } 
#define WUSB_AR_HostFriendlyName	{ .id = cpu_to_le16(0x000c), .len = cpu_to_le16(64) } 
#define WUSB_AR_CHID			{ .id = cpu_to_le16(0x1000), .len = cpu_to_le16(16) }
#define WUSB_AR_CDID			{ .id = cpu_to_le16(0x1001), .len = cpu_to_le16(16) }
#define WUSB_AR_ConnectionContext	{ .id = cpu_to_le16(0x1002), .len = cpu_to_le16(48) }
#define WUSB_AR_BandGroups		{ .id = cpu_to_le16(0x1004), .len = cpu_to_le16(2) }


enum {
	CBAF_REQ_GET_ASSOCIATION_INFORMATION = 0x01,
	CBAF_REQ_GET_ASSOCIATION_REQUEST,
	CBAF_REQ_SET_ASSOCIATION_RESPONSE
};


enum {
	CBAF_IFACECLASS    = 0xef,
	CBAF_IFACESUBCLASS = 0x03,
	CBAF_IFACEPROTOCOL = 0x01,
};


struct wusb_cbaf_assoc_info {
	__le16 Length;
	__u8 NumAssociationRequests;
	__le16 Flags;
	__u8 AssociationRequestsArray[];
} __attribute__((packed));


struct wusb_cbaf_assoc_request {
	__u8 AssociationDataIndex;
	__u8 Reserved;
	__le16 AssociationTypeId;
	__le16 AssociationSubTypeId;
	__le32 AssociationTypeInfoSize;
} __attribute__((packed));

enum {
	AR_TYPE_WUSB                    = 0x0001,
	AR_TYPE_WUSB_RETRIEVE_HOST_INFO = 0x0000,
	AR_TYPE_WUSB_ASSOCIATE          = 0x0001,
};


struct wusb_cbaf_attr_hdr {
	__le16 id;
	__le16 len;
} __attribute__((packed));


struct wusb_cbaf_host_info {
	struct wusb_cbaf_attr_hdr AssociationTypeId_hdr;
	__le16 AssociationTypeId;
	struct wusb_cbaf_attr_hdr AssociationSubTypeId_hdr;
	__le16 AssociationSubTypeId;
	struct wusb_cbaf_attr_hdr CHID_hdr;
	struct wusb_ckhdid CHID;
	struct wusb_cbaf_attr_hdr LangID_hdr;
	__le16 LangID;
	struct wusb_cbaf_attr_hdr HostFriendlyName_hdr;
	__u8 HostFriendlyName[];
} __attribute__((packed));


struct wusb_cbaf_device_info {
	struct wusb_cbaf_attr_hdr Length_hdr;
	__le32 Length;
	struct wusb_cbaf_attr_hdr CDID_hdr;
	struct wusb_ckhdid CDID;
	struct wusb_cbaf_attr_hdr BandGroups_hdr;
	__le16 BandGroups;
	struct wusb_cbaf_attr_hdr LangID_hdr;
	__le16 LangID;
	struct wusb_cbaf_attr_hdr DeviceFriendlyName_hdr;
	__u8 DeviceFriendlyName[];
} __attribute__((packed));


struct wusb_cbaf_cc_data {
	struct wusb_cbaf_attr_hdr AssociationTypeId_hdr;
	__le16 AssociationTypeId;
	struct wusb_cbaf_attr_hdr AssociationSubTypeId_hdr;
	__le16 AssociationSubTypeId;
	struct wusb_cbaf_attr_hdr Length_hdr;
	__le32 Length;
	struct wusb_cbaf_attr_hdr ConnectionContext_hdr;
	struct wusb_ckhdid CHID;
	struct wusb_ckhdid CDID;
	struct wusb_ckhdid CK;
	struct wusb_cbaf_attr_hdr BandGroups_hdr;
	__le16 BandGroups;
} __attribute__((packed));


struct wusb_cbaf_cc_data_fail {
	struct wusb_cbaf_attr_hdr AssociationTypeId_hdr;
	__le16 AssociationTypeId;
	struct wusb_cbaf_attr_hdr AssociationSubTypeId_hdr;
	__le16 AssociationSubTypeId;
	struct wusb_cbaf_attr_hdr Length_hdr;
	__le16 Length;
	struct wusb_cbaf_attr_hdr AssociationStatus_hdr;
	__u32 AssociationStatus;
} __attribute__((packed));

#endif	
