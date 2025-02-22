#ifndef LLC_S_AC_H
#define LLC_S_AC_H


#define SAP_ACT_UNITDATA_IND	1
#define SAP_ACT_SEND_UI		2
#define SAP_ACT_SEND_XID_C	3
#define SAP_ACT_SEND_XID_R	4
#define SAP_ACT_SEND_TEST_C	5
#define SAP_ACT_SEND_TEST_R	6
#define SAP_ACT_REPORT_STATUS	7
#define SAP_ACT_XID_IND		8
#define SAP_ACT_TEST_IND	9


typedef int (*llc_sap_action_t)(struct llc_sap *sap, struct sk_buff *skb);

int llc_sap_action_unitdata_ind(struct llc_sap *sap, struct sk_buff *skb);
int llc_sap_action_send_ui(struct llc_sap *sap, struct sk_buff *skb);
int llc_sap_action_send_xid_c(struct llc_sap *sap, struct sk_buff *skb);
int llc_sap_action_send_xid_r(struct llc_sap *sap, struct sk_buff *skb);
int llc_sap_action_send_test_c(struct llc_sap *sap, struct sk_buff *skb);
int llc_sap_action_send_test_r(struct llc_sap *sap, struct sk_buff *skb);
int llc_sap_action_report_status(struct llc_sap *sap, struct sk_buff *skb);
int llc_sap_action_xid_ind(struct llc_sap *sap, struct sk_buff *skb);
int llc_sap_action_test_ind(struct llc_sap *sap, struct sk_buff *skb);
#endif 
