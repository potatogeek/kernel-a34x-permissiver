/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ARCH_PERF_BOOK3S_HV_HCALLS_H
#define ARCH_PERF_BOOK3S_HV_HCALLS_H

/*
 * PowerPC HCALL codes : hcall code to name mapping
 */
#define kvm_trace_symbol_hcall \
	{0x4, "H_REMOVE"},					\
	{0x8, "H_ENTER"},					\
	{0xc, "H_READ"},					\
	{0x10, "H_CLEAR_MOD"},					\
	{0x14, "H_CLEAR_REF"},					\
	{0x18, "H_PROTECT"},					\
	{0x1c, "H_GET_TCE"},					\
	{0x20, "H_PUT_TCE"},					\
	{0x24, "H_SET_SPRG0"},					\
	{0x28, "H_SET_DABR"},					\
	{0x2c, "H_PAGE_INIT"},					\
	{0x30, "H_SET_ASR"},					\
	{0x34, "H_ASR_ON"},					\
	{0x38, "H_ASR_OFF"},					\
	{0x3c, "H_LOGICAL_CI_LOAD"},				\
	{0x40, "H_LOGICAL_CI_STORE"},				\
	{0x44, "H_LOGICAL_CACHE_LOAD"},				\
	{0x48, "H_LOGICAL_CACHE_STORE"},			\
	{0x4c, "H_LOGICAL_ICBI"},				\
	{0x50, "H_LOGICAL_DCBF"},				\
	{0x54, "H_GET_TERM_CHAR"},				\
	{0x58, "H_PUT_TERM_CHAR"},				\
	{0x5c, "H_REAL_TO_LOGICAL"},				\
	{0x60, "H_HYPERVISOR_DATA"},				\
	{0x64, "H_EOI"},					\
	{0x68, "H_CPPR"},					\
	{0x6c, "H_IPI"},					\
	{0x70, "H_IPOLL"},					\
	{0x74, "H_XIRR"},					\
	{0x78, "H_MIGRATE_DMA"},				\
	{0x7c, "H_PERFMON"},					\
	{0xdc, "H_REGISTER_VPA"},				\
	{0xe0, "H_CEDE"},					\
	{0xe4, "H_CONFER"},					\
	{0xe8, "H_PROD"},					\
	{0xec, "H_GET_PPP"},					\
	{0xf0, "H_SET_PPP"},					\
	{0xf4, "H_PURR"},					\
	{0xf8, "H_PIC"},					\
	{0xfc, "H_REG_CRQ"},					\
	{0x100, "H_FREE_CRQ"},					\
	{0x104, "H_VIO_SIGNAL"},				\
	{0x108, "H_SEND_CRQ"},					\
	{0x110, "H_COPY_RDMA"},					\
	{0x114, "H_REGISTER_LOGICAL_LAN"},			\
	{0x118, "H_FREE_LOGICAL_LAN"},				\
	{0x11c, "H_ADD_LOGICAL_LAN_BUFFER"},			\
	{0x120, "H_SEND_LOGICAL_LAN"},				\
	{0x124, "H_BULK_REMOVE"},				\
	{0x130, "H_MULTICAST_CTRL"},				\
	{0x134, "H_SET_XDABR"},					\
	{0x138, "H_STUFF_TCE"},					\
	{0x13c, "H_PUT_TCE_INDIRECT"},				\
	{0x14c, "H_CHANGE_LOGICAL_LAN_MAC"},			\
	{0x150, "H_VTERM_PARTNER_INFO"},			\
	{0x154, "H_REGISTER_VTERM"},				\
	{0x158, "H_FREE_VTERM"},				\
	{0x15c, "H_RESET_EVENTS"},				\
	{0x160, "H_ALLOC_RESOURCE"},				\
	{0x164, "H_FREE_RESOURCE"},				\
	{0x168, "H_MODIFY_QP"},					\
	{0x16c, "H_QUERY_QP"},					\
	{0x170, "H_REREGISTER_PMR"},				\
	{0x174, "H_REGISTER_SMR"},				\
	{0x178, "H_QUERY_MR"},					\
	{0x17c, "H_QUERY_MW"},					\
	{0x180, "H_QUERY_HCA"},					\
	{0x184, "H_QUERY_PORT"},				\
	{0x188, "H_MODIFY_PORT"},				\
	{0x18c, "H_DEFINE_AQP1"},				\
	{0x190, "H_GET_TRACE_BUFFER"},				\
	{0x194, "H_DEFINE_AQP0"},				\
	{0x198, "H_RESIZE_MR"},					\
	{0x19c, "H_ATTACH_MCQP"},				\
	{0x1a0, "H_DETACH_MCQP"},				\
	{0x1a4, "H_CREATE_RPT"},				\
	{0x1a8, "H_REMOVE_RPT"},				\
	{0x1ac, "H_REGISTER_RPAGES"},				\
<<<<<<< HEAD
	{0x1b0, "H_DISABLE_AND_GETC"},				\
=======
	{0x1b0, "H_DISABLE_AND_GET"},				\
>>>>>>> upstream/android-13
	{0x1b4, "H_ERROR_DATA"},				\
	{0x1b8, "H_GET_HCA_INFO"},				\
	{0x1bc, "H_GET_PERF_COUNT"},				\
	{0x1c0, "H_MANAGE_TRACE"},				\
	{0x1d4, "H_FREE_LOGICAL_LAN_BUFFER"},			\
	{0x1d8, "H_POLL_PENDING"},				\
	{0x1e4, "H_QUERY_INT_STATE"},				\
	{0x244, "H_ILLAN_ATTRIBUTES"},				\
	{0x250, "H_MODIFY_HEA_QP"},				\
	{0x254, "H_QUERY_HEA_QP"},				\
	{0x258, "H_QUERY_HEA"},					\
	{0x25c, "H_QUERY_HEA_PORT"},				\
	{0x260, "H_MODIFY_HEA_PORT"},				\
	{0x264, "H_REG_BCMC"},					\
	{0x268, "H_DEREG_BCMC"},				\
	{0x26c, "H_REGISTER_HEA_RPAGES"},			\
	{0x270, "H_DISABLE_AND_GET_HEA"},			\
	{0x274, "H_GET_HEA_INFO"},				\
	{0x278, "H_ALLOC_HEA_RESOURCE"},			\
	{0x284, "H_ADD_CONN"},					\
	{0x288, "H_DEL_CONN"},					\
	{0x298, "H_JOIN"},					\
	{0x2a4, "H_VASI_STATE"},				\
	{0x2b0, "H_ENABLE_CRQ"},				\
	{0x2b8, "H_GET_EM_PARMS"},				\
	{0x2d0, "H_SET_MPP"},					\
	{0x2d4, "H_GET_MPP"},					\
	{0x2ec, "H_HOME_NODE_ASSOCIATIVITY"},			\
	{0x2f4, "H_BEST_ENERGY"},				\
	{0x2fc, "H_XIRR_X"},					\
	{0x300, "H_RANDOM"},					\
	{0x304, "H_COP"},					\
	{0x314, "H_GET_MPP_X"},					\
	{0x31c, "H_SET_MODE"},					\
	{0xf000, "H_RTAS"}					\

#endif
