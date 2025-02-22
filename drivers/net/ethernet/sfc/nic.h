<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/****************************************************************************
 * Driver for Solarflare network controllers and boards
 * Copyright 2005-2006 Fen Systems Ltd.
 * Copyright 2006-2013 Solarflare Communications Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
=======
>>>>>>> upstream/android-13
 */

#ifndef EFX_NIC_H
#define EFX_NIC_H

<<<<<<< HEAD
#include <linux/net_tstamp.h>
#include <linux/i2c-algo-bit.h>
#include "net_driver.h"
#include "efx.h"
#include "mcdi.h"

enum {
	/* Revisions 0-2 were Falcon A0, A1 and B0 respectively.
	 * They are not supported by this driver but these revision numbers
	 * form part of the ethtool API for register dumping.
	 */
	EFX_REV_SIENA_A0 = 3,
	EFX_REV_HUNT_A0 = 4,
};

static inline int efx_nic_rev(struct efx_nic *efx)
{
	return efx->type->revision;
}

u32 efx_farch_fpga_ver(struct efx_nic *efx);

/* Read the current event from the event queue */
static inline efx_qword_t *efx_event(struct efx_channel *channel,
				     unsigned int index)
{
	return ((efx_qword_t *) (channel->eventq.buf.addr)) +
		(index & channel->eventq_mask);
}

/* See if an event is present
 *
 * We check both the high and low dword of the event for all ones.  We
 * wrote all ones when we cleared the event, and no valid event can
 * have all ones in either its high or low dwords.  This approach is
 * robust against reordering.
 *
 * Note that using a single 64-bit comparison is incorrect; even
 * though the CPU read will be atomic, the DMA write may not be.
 */
static inline int efx_event_present(efx_qword_t *event)
{
	return !(EFX_DWORD_IS_ALL_ONES(event->dword[0]) |
		  EFX_DWORD_IS_ALL_ONES(event->dword[1]));
}

/* Returns a pointer to the specified transmit descriptor in the TX
 * descriptor queue belonging to the specified channel.
 */
static inline efx_qword_t *
efx_tx_desc(struct efx_tx_queue *tx_queue, unsigned int index)
{
	return ((efx_qword_t *) (tx_queue->txd.buf.addr)) + index;
}

/* Get partner of a TX queue, seen as part of the same net core queue */
static struct efx_tx_queue *efx_tx_queue_partner(struct efx_tx_queue *tx_queue)
{
	if (tx_queue->queue & EFX_TXQ_TYPE_OFFLOAD)
		return tx_queue - EFX_TXQ_TYPE_OFFLOAD;
	else
		return tx_queue + EFX_TXQ_TYPE_OFFLOAD;
}

/* Report whether this TX queue would be empty for the given write_count.
 * May return false negative.
 */
static inline bool __efx_nic_tx_is_empty(struct efx_tx_queue *tx_queue,
					 unsigned int write_count)
{
	unsigned int empty_read_count = READ_ONCE(tx_queue->empty_read_count);

	if (empty_read_count == 0)
		return false;

	return ((empty_read_count ^ write_count) & ~EFX_EMPTY_COUNT_VALID) == 0;
}

/* Report whether the NIC considers this TX queue empty, using
 * packet_write_count (the write count recorded for the last completable
 * doorbell push).  May return false negative.  EF10 only, which is OK
 * because only EF10 supports PIO.
 */
static inline bool efx_nic_tx_is_empty(struct efx_tx_queue *tx_queue)
{
	EFX_WARN_ON_ONCE_PARANOID(!tx_queue->efx->type->option_descriptors);
	return __efx_nic_tx_is_empty(tx_queue, tx_queue->packet_write_count);
}

/* Decide whether we can use TX PIO, ie. write packet data directly into
 * a buffer on the device.  This can reduce latency at the expense of
 * throughput, so we only do this if both hardware and software TX rings
 * are empty.  This also ensures that only one packet at a time can be
 * using the PIO buffer.
 */
static inline bool efx_nic_may_tx_pio(struct efx_tx_queue *tx_queue)
{
	struct efx_tx_queue *partner = efx_tx_queue_partner(tx_queue);

	return tx_queue->piobuf && efx_nic_tx_is_empty(tx_queue) &&
	       efx_nic_tx_is_empty(partner);
}

/* Decide whether to push a TX descriptor to the NIC vs merely writing
 * the doorbell.  This can reduce latency when we are adding a single
 * descriptor to an empty queue, but is otherwise pointless.  Further,
 * Falcon and Siena have hardware bugs (SF bug 33851) that may be
 * triggered if we don't check this.
 * We use the write_count used for the last doorbell push, to get the
 * NIC's view of the tx queue.
 */
static inline bool efx_nic_may_push_tx_desc(struct efx_tx_queue *tx_queue,
					    unsigned int write_count)
{
	bool was_empty = __efx_nic_tx_is_empty(tx_queue, write_count);

	tx_queue->empty_read_count = 0;
	return was_empty && tx_queue->write_count - write_count == 1;
}

/* Returns a pointer to the specified descriptor in the RX descriptor queue */
static inline efx_qword_t *
efx_rx_desc(struct efx_rx_queue *rx_queue, unsigned int index)
{
	return ((efx_qword_t *) (rx_queue->rxd.buf.addr)) + index;
}

=======
#include "nic_common.h"
#include "efx.h"

u32 efx_farch_fpga_ver(struct efx_nic *efx);

>>>>>>> upstream/android-13
enum {
	PHY_TYPE_NONE = 0,
	PHY_TYPE_TXC43128 = 1,
	PHY_TYPE_88E1111 = 2,
	PHY_TYPE_SFX7101 = 3,
	PHY_TYPE_QT2022C2 = 4,
	PHY_TYPE_PM8358 = 6,
	PHY_TYPE_SFT9001A = 8,
	PHY_TYPE_QT2025C = 9,
	PHY_TYPE_SFT9001B = 10,
};

<<<<<<< HEAD
/* Alignment of PCIe DMA boundaries (4KB) */
#define EFX_PAGE_SIZE	4096
/* Size and alignment of buffer table entries (same) */
#define EFX_BUF_SIZE	EFX_PAGE_SIZE

/* NIC-generic software stats */
enum {
	GENERIC_STAT_rx_noskb_drops,
	GENERIC_STAT_rx_nodesc_trunc,
	GENERIC_STAT_COUNT
};

=======
>>>>>>> upstream/android-13
enum {
	SIENA_STAT_tx_bytes = GENERIC_STAT_COUNT,
	SIENA_STAT_tx_good_bytes,
	SIENA_STAT_tx_bad_bytes,
	SIENA_STAT_tx_packets,
	SIENA_STAT_tx_bad,
	SIENA_STAT_tx_pause,
	SIENA_STAT_tx_control,
	SIENA_STAT_tx_unicast,
	SIENA_STAT_tx_multicast,
	SIENA_STAT_tx_broadcast,
	SIENA_STAT_tx_lt64,
	SIENA_STAT_tx_64,
	SIENA_STAT_tx_65_to_127,
	SIENA_STAT_tx_128_to_255,
	SIENA_STAT_tx_256_to_511,
	SIENA_STAT_tx_512_to_1023,
	SIENA_STAT_tx_1024_to_15xx,
	SIENA_STAT_tx_15xx_to_jumbo,
	SIENA_STAT_tx_gtjumbo,
	SIENA_STAT_tx_collision,
	SIENA_STAT_tx_single_collision,
	SIENA_STAT_tx_multiple_collision,
	SIENA_STAT_tx_excessive_collision,
	SIENA_STAT_tx_deferred,
	SIENA_STAT_tx_late_collision,
	SIENA_STAT_tx_excessive_deferred,
	SIENA_STAT_tx_non_tcpudp,
	SIENA_STAT_tx_mac_src_error,
	SIENA_STAT_tx_ip_src_error,
	SIENA_STAT_rx_bytes,
	SIENA_STAT_rx_good_bytes,
	SIENA_STAT_rx_bad_bytes,
	SIENA_STAT_rx_packets,
	SIENA_STAT_rx_good,
	SIENA_STAT_rx_bad,
	SIENA_STAT_rx_pause,
	SIENA_STAT_rx_control,
	SIENA_STAT_rx_unicast,
	SIENA_STAT_rx_multicast,
	SIENA_STAT_rx_broadcast,
	SIENA_STAT_rx_lt64,
	SIENA_STAT_rx_64,
	SIENA_STAT_rx_65_to_127,
	SIENA_STAT_rx_128_to_255,
	SIENA_STAT_rx_256_to_511,
	SIENA_STAT_rx_512_to_1023,
	SIENA_STAT_rx_1024_to_15xx,
	SIENA_STAT_rx_15xx_to_jumbo,
	SIENA_STAT_rx_gtjumbo,
	SIENA_STAT_rx_bad_gtjumbo,
	SIENA_STAT_rx_overflow,
	SIENA_STAT_rx_false_carrier,
	SIENA_STAT_rx_symbol_error,
	SIENA_STAT_rx_align_error,
	SIENA_STAT_rx_length_error,
	SIENA_STAT_rx_internal_error,
	SIENA_STAT_rx_nodesc_drop_cnt,
	SIENA_STAT_COUNT
};

/**
 * struct siena_nic_data - Siena NIC state
 * @efx: Pointer back to main interface structure
 * @wol_filter_id: Wake-on-LAN packet filter id
 * @stats: Hardware statistics
 * @vf: Array of &struct siena_vf objects
 * @vf_buftbl_base: The zeroth buffer table index used to back VF queues.
 * @vfdi_status: Common VFDI status page to be dmad to VF address space.
 * @local_addr_list: List of local addresses. Protected by %local_lock.
 * @local_page_list: List of DMA addressable pages used to broadcast
 *	%local_addr_list. Protected by %local_lock.
 * @local_lock: Mutex protecting %local_addr_list and %local_page_list.
 * @peer_work: Work item to broadcast peer addresses to VMs.
 */
struct siena_nic_data {
	struct efx_nic *efx;
	int wol_filter_id;
	u64 stats[SIENA_STAT_COUNT];
#ifdef CONFIG_SFC_SRIOV
	struct siena_vf *vf;
	struct efx_channel *vfdi_channel;
	unsigned vf_buftbl_base;
	struct efx_buffer vfdi_status;
	struct list_head local_addr_list;
	struct list_head local_page_list;
	struct mutex local_lock;
	struct work_struct peer_work;
#endif
};

enum {
	EF10_STAT_port_tx_bytes = GENERIC_STAT_COUNT,
	EF10_STAT_port_tx_packets,
	EF10_STAT_port_tx_pause,
	EF10_STAT_port_tx_control,
	EF10_STAT_port_tx_unicast,
	EF10_STAT_port_tx_multicast,
	EF10_STAT_port_tx_broadcast,
	EF10_STAT_port_tx_lt64,
	EF10_STAT_port_tx_64,
	EF10_STAT_port_tx_65_to_127,
	EF10_STAT_port_tx_128_to_255,
	EF10_STAT_port_tx_256_to_511,
	EF10_STAT_port_tx_512_to_1023,
	EF10_STAT_port_tx_1024_to_15xx,
	EF10_STAT_port_tx_15xx_to_jumbo,
	EF10_STAT_port_rx_bytes,
	EF10_STAT_port_rx_bytes_minus_good_bytes,
	EF10_STAT_port_rx_good_bytes,
	EF10_STAT_port_rx_bad_bytes,
	EF10_STAT_port_rx_packets,
	EF10_STAT_port_rx_good,
	EF10_STAT_port_rx_bad,
	EF10_STAT_port_rx_pause,
	EF10_STAT_port_rx_control,
	EF10_STAT_port_rx_unicast,
	EF10_STAT_port_rx_multicast,
	EF10_STAT_port_rx_broadcast,
	EF10_STAT_port_rx_lt64,
	EF10_STAT_port_rx_64,
	EF10_STAT_port_rx_65_to_127,
	EF10_STAT_port_rx_128_to_255,
	EF10_STAT_port_rx_256_to_511,
	EF10_STAT_port_rx_512_to_1023,
	EF10_STAT_port_rx_1024_to_15xx,
	EF10_STAT_port_rx_15xx_to_jumbo,
	EF10_STAT_port_rx_gtjumbo,
	EF10_STAT_port_rx_bad_gtjumbo,
	EF10_STAT_port_rx_overflow,
	EF10_STAT_port_rx_align_error,
	EF10_STAT_port_rx_length_error,
	EF10_STAT_port_rx_nodesc_drops,
	EF10_STAT_port_rx_pm_trunc_bb_overflow,
	EF10_STAT_port_rx_pm_discard_bb_overflow,
	EF10_STAT_port_rx_pm_trunc_vfifo_full,
	EF10_STAT_port_rx_pm_discard_vfifo_full,
	EF10_STAT_port_rx_pm_trunc_qbb,
	EF10_STAT_port_rx_pm_discard_qbb,
	EF10_STAT_port_rx_pm_discard_mapping,
	EF10_STAT_port_rx_dp_q_disabled_packets,
	EF10_STAT_port_rx_dp_di_dropped_packets,
	EF10_STAT_port_rx_dp_streaming_packets,
	EF10_STAT_port_rx_dp_hlb_fetch,
	EF10_STAT_port_rx_dp_hlb_wait,
	EF10_STAT_rx_unicast,
	EF10_STAT_rx_unicast_bytes,
	EF10_STAT_rx_multicast,
	EF10_STAT_rx_multicast_bytes,
	EF10_STAT_rx_broadcast,
	EF10_STAT_rx_broadcast_bytes,
	EF10_STAT_rx_bad,
	EF10_STAT_rx_bad_bytes,
	EF10_STAT_rx_overflow,
	EF10_STAT_tx_unicast,
	EF10_STAT_tx_unicast_bytes,
	EF10_STAT_tx_multicast,
	EF10_STAT_tx_multicast_bytes,
	EF10_STAT_tx_broadcast,
	EF10_STAT_tx_broadcast_bytes,
	EF10_STAT_tx_bad,
	EF10_STAT_tx_bad_bytes,
	EF10_STAT_tx_overflow,
	EF10_STAT_V1_COUNT,
	EF10_STAT_fec_uncorrected_errors = EF10_STAT_V1_COUNT,
	EF10_STAT_fec_corrected_errors,
	EF10_STAT_fec_corrected_symbols_lane0,
	EF10_STAT_fec_corrected_symbols_lane1,
	EF10_STAT_fec_corrected_symbols_lane2,
	EF10_STAT_fec_corrected_symbols_lane3,
	EF10_STAT_ctpio_vi_busy_fallback,
	EF10_STAT_ctpio_long_write_success,
	EF10_STAT_ctpio_missing_dbell_fail,
	EF10_STAT_ctpio_overflow_fail,
	EF10_STAT_ctpio_underflow_fail,
	EF10_STAT_ctpio_timeout_fail,
	EF10_STAT_ctpio_noncontig_wr_fail,
	EF10_STAT_ctpio_frm_clobber_fail,
	EF10_STAT_ctpio_invalid_wr_fail,
	EF10_STAT_ctpio_vi_clobber_fallback,
	EF10_STAT_ctpio_unqualified_fallback,
	EF10_STAT_ctpio_runt_fallback,
	EF10_STAT_ctpio_success,
	EF10_STAT_ctpio_fallback,
	EF10_STAT_ctpio_poison,
	EF10_STAT_ctpio_erase,
	EF10_STAT_COUNT
};

/* Maximum number of TX PIO buffers we may allocate to a function.
 * This matches the total number of buffers on each SFC9100-family
 * controller.
 */
#define EF10_TX_PIOBUF_COUNT 16

/**
 * struct efx_ef10_nic_data - EF10 architecture NIC state
 * @mcdi_buf: DMA buffer for MCDI
 * @warm_boot_count: Last seen MC warm boot count
 * @vi_base: Absolute index of first VI in this function
 * @n_allocated_vis: Number of VIs allocated to this function
<<<<<<< HEAD
 * @must_realloc_vis: Flag: VIs have yet to be reallocated after MC reboot
 * @must_restore_rss_contexts: Flag: RSS contexts have yet to be restored after
 *	MC reboot
 * @must_restore_filters: Flag: filters have yet to be restored after MC reboot
=======
>>>>>>> upstream/android-13
 * @n_piobufs: Number of PIO buffers allocated to this function
 * @wc_membase: Base address of write-combining mapping of the memory BAR
 * @pio_write_base: Base address for writing PIO buffers
 * @pio_write_vi_base: Relative VI number for @pio_write_base
 * @piobuf_handle: Handle of each PIO buffer allocated
 * @piobuf_size: size of a single PIO buffer
 * @must_restore_piobufs: Flag: PIO buffers have yet to be restored after MC
 *	reboot
<<<<<<< HEAD
 * @rx_rss_context_exclusive: Whether our RSS context is exclusive or shared
=======
 * @mc_stats: Scratch buffer for converting statistics to the kernel's format
>>>>>>> upstream/android-13
 * @stats: Hardware statistics
 * @workaround_35388: Flag: firmware supports workaround for bug 35388
 * @workaround_26807: Flag: firmware supports workaround for bug 26807
 * @workaround_61265: Flag: firmware supports workaround for bug 61265
 * @must_check_datapath_caps: Flag: @datapath_caps needs to be revalidated
 *	after MC reboot
 * @datapath_caps: Capabilities of datapath firmware (FLAGS1 field of
 *	%MC_CMD_GET_CAPABILITIES response)
 * @datapath_caps2: Further Capabilities of datapath firmware (FLAGS2 field of
 * %MC_CMD_GET_CAPABILITIES response)
 * @rx_dpcpu_fw_id: Firmware ID of the RxDPCPU
 * @tx_dpcpu_fw_id: Firmware ID of the TxDPCPU
<<<<<<< HEAD
 * @vport_id: The function's vport ID, only relevant for PFs
=======
>>>>>>> upstream/android-13
 * @must_probe_vswitching: Flag: vswitching has yet to be setup after MC reboot
 * @pf_index: The number for this PF, or the parent PF if this is a VF
#ifdef CONFIG_SFC_SRIOV
 * @vf: Pointer to VF data structure
#endif
 * @vport_mac: The MAC address on the vport, only for PFs; VFs will be zero
 * @vlan_list: List of VLANs added over the interface. Serialised by vlan_lock.
 * @vlan_lock: Lock to serialize access to vlan_list.
 * @udp_tunnels: UDP tunnel port numbers and types.
 * @udp_tunnels_dirty: flag indicating a reboot occurred while pushing
 *	@udp_tunnels to hardware and thus the push must be re-done.
 * @udp_tunnels_lock: Serialises writes to @udp_tunnels and @udp_tunnels_dirty.
 */
struct efx_ef10_nic_data {
	struct efx_buffer mcdi_buf;
	u16 warm_boot_count;
	unsigned int vi_base;
	unsigned int n_allocated_vis;
<<<<<<< HEAD
	bool must_realloc_vis;
	bool must_restore_rss_contexts;
	bool must_restore_filters;
=======
>>>>>>> upstream/android-13
	unsigned int n_piobufs;
	void __iomem *wc_membase, *pio_write_base;
	unsigned int pio_write_vi_base;
	unsigned int piobuf_handle[EF10_TX_PIOBUF_COUNT];
	u16 piobuf_size;
	bool must_restore_piobufs;
<<<<<<< HEAD
	bool rx_rss_context_exclusive;
=======
	__le64 *mc_stats;
>>>>>>> upstream/android-13
	u64 stats[EF10_STAT_COUNT];
	bool workaround_35388;
	bool workaround_26807;
	bool workaround_61265;
	bool must_check_datapath_caps;
	u32 datapath_caps;
	u32 datapath_caps2;
	unsigned int rx_dpcpu_fw_id;
	unsigned int tx_dpcpu_fw_id;
<<<<<<< HEAD
	unsigned int vport_id;
=======
>>>>>>> upstream/android-13
	bool must_probe_vswitching;
	unsigned int pf_index;
	u8 port_id[ETH_ALEN];
#ifdef CONFIG_SFC_SRIOV
	unsigned int vf_index;
	struct ef10_vf *vf;
#endif
	u8 vport_mac[ETH_ALEN];
	struct list_head vlan_list;
	struct mutex vlan_lock;
	struct efx_udp_tunnel udp_tunnels[16];
	bool udp_tunnels_dirty;
	struct mutex udp_tunnels_lock;
	u64 licensed_features;
};

<<<<<<< HEAD
int efx_init_sriov(void);
void efx_fini_sriov(void);

struct ethtool_ts_info;
int efx_ptp_probe(struct efx_nic *efx, struct efx_channel *channel);
void efx_ptp_defer_probe_with_channel(struct efx_nic *efx);
struct efx_channel *efx_ptp_channel(struct efx_nic *efx);
void efx_ptp_remove(struct efx_nic *efx);
int efx_ptp_set_ts_config(struct efx_nic *efx, struct ifreq *ifr);
int efx_ptp_get_ts_config(struct efx_nic *efx, struct ifreq *ifr);
void efx_ptp_get_ts_info(struct efx_nic *efx, struct ethtool_ts_info *ts_info);
bool efx_ptp_is_ptp_tx(struct efx_nic *efx, struct sk_buff *skb);
int efx_ptp_get_mode(struct efx_nic *efx);
int efx_ptp_change_mode(struct efx_nic *efx, bool enable_wanted,
			unsigned int new_mode);
int efx_ptp_tx(struct efx_nic *efx, struct sk_buff *skb);
void efx_ptp_event(struct efx_nic *efx, efx_qword_t *ev);
size_t efx_ptp_describe_stats(struct efx_nic *efx, u8 *strings);
size_t efx_ptp_update_stats(struct efx_nic *efx, u64 *stats);
void efx_time_sync_event(struct efx_channel *channel, efx_qword_t *ev);
void __efx_rx_skb_attach_timestamp(struct efx_channel *channel,
				   struct sk_buff *skb);
static inline void efx_rx_skb_attach_timestamp(struct efx_channel *channel,
					       struct sk_buff *skb)
{
	if (channel->sync_events_state == SYNC_EVENTS_VALID)
		__efx_rx_skb_attach_timestamp(channel, skb);
}
void efx_ptp_start_datapath(struct efx_nic *efx);
void efx_ptp_stop_datapath(struct efx_nic *efx);
bool efx_ptp_use_mac_tx_timestamps(struct efx_nic *efx);
ktime_t efx_ptp_nic_to_kernel_time(struct efx_tx_queue *tx_queue);

extern const struct efx_nic_type falcon_a1_nic_type;
extern const struct efx_nic_type falcon_b0_nic_type;
=======
/* TSOv2 */
int efx_ef10_tx_tso_desc(struct efx_tx_queue *tx_queue, struct sk_buff *skb,
			 bool *data_mapped);

int efx_init_sriov(void);
void efx_fini_sriov(void);

>>>>>>> upstream/android-13
extern const struct efx_nic_type siena_a0_nic_type;
extern const struct efx_nic_type efx_hunt_a0_nic_type;
extern const struct efx_nic_type efx_hunt_a0_vf_nic_type;

<<<<<<< HEAD
/**************************************************************************
 *
 * Externs
 *
 **************************************************************************
 */

int falcon_probe_board(struct efx_nic *efx, u16 revision_info);

/* TX data path */
static inline int efx_nic_probe_tx(struct efx_tx_queue *tx_queue)
{
	return tx_queue->efx->type->tx_probe(tx_queue);
}
static inline void efx_nic_init_tx(struct efx_tx_queue *tx_queue)
{
	tx_queue->efx->type->tx_init(tx_queue);
}
static inline void efx_nic_remove_tx(struct efx_tx_queue *tx_queue)
{
	tx_queue->efx->type->tx_remove(tx_queue);
}
static inline void efx_nic_push_buffers(struct efx_tx_queue *tx_queue)
{
	tx_queue->efx->type->tx_write(tx_queue);
}

/* RX data path */
static inline int efx_nic_probe_rx(struct efx_rx_queue *rx_queue)
{
	return rx_queue->efx->type->rx_probe(rx_queue);
}
static inline void efx_nic_init_rx(struct efx_rx_queue *rx_queue)
{
	rx_queue->efx->type->rx_init(rx_queue);
}
static inline void efx_nic_remove_rx(struct efx_rx_queue *rx_queue)
{
	rx_queue->efx->type->rx_remove(rx_queue);
}
static inline void efx_nic_notify_rx_desc(struct efx_rx_queue *rx_queue)
{
	rx_queue->efx->type->rx_write(rx_queue);
}
static inline void efx_nic_generate_fill_event(struct efx_rx_queue *rx_queue)
{
	rx_queue->efx->type->rx_defer_refill(rx_queue);
}

/* Event data path */
static inline int efx_nic_probe_eventq(struct efx_channel *channel)
{
	return channel->efx->type->ev_probe(channel);
}
static inline int efx_nic_init_eventq(struct efx_channel *channel)
{
	return channel->efx->type->ev_init(channel);
}
static inline void efx_nic_fini_eventq(struct efx_channel *channel)
{
	channel->efx->type->ev_fini(channel);
}
static inline void efx_nic_remove_eventq(struct efx_channel *channel)
{
	channel->efx->type->ev_remove(channel);
}
static inline int
efx_nic_process_eventq(struct efx_channel *channel, int quota)
{
	return channel->efx->type->ev_process(channel, quota);
}
static inline void efx_nic_eventq_read_ack(struct efx_channel *channel)
{
	channel->efx->type->ev_read_ack(channel);
}
void efx_nic_event_test_start(struct efx_channel *channel);

=======
int falcon_probe_board(struct efx_nic *efx, u16 revision_info);

>>>>>>> upstream/android-13
/* Falcon/Siena queue operations */
int efx_farch_tx_probe(struct efx_tx_queue *tx_queue);
void efx_farch_tx_init(struct efx_tx_queue *tx_queue);
void efx_farch_tx_fini(struct efx_tx_queue *tx_queue);
void efx_farch_tx_remove(struct efx_tx_queue *tx_queue);
void efx_farch_tx_write(struct efx_tx_queue *tx_queue);
unsigned int efx_farch_tx_limit_len(struct efx_tx_queue *tx_queue,
				    dma_addr_t dma_addr, unsigned int len);
int efx_farch_rx_probe(struct efx_rx_queue *rx_queue);
void efx_farch_rx_init(struct efx_rx_queue *rx_queue);
void efx_farch_rx_fini(struct efx_rx_queue *rx_queue);
void efx_farch_rx_remove(struct efx_rx_queue *rx_queue);
void efx_farch_rx_write(struct efx_rx_queue *rx_queue);
void efx_farch_rx_defer_refill(struct efx_rx_queue *rx_queue);
int efx_farch_ev_probe(struct efx_channel *channel);
int efx_farch_ev_init(struct efx_channel *channel);
void efx_farch_ev_fini(struct efx_channel *channel);
void efx_farch_ev_remove(struct efx_channel *channel);
int efx_farch_ev_process(struct efx_channel *channel, int quota);
void efx_farch_ev_read_ack(struct efx_channel *channel);
void efx_farch_ev_test_generate(struct efx_channel *channel);

/* Falcon/Siena filter operations */
int efx_farch_filter_table_probe(struct efx_nic *efx);
void efx_farch_filter_table_restore(struct efx_nic *efx);
void efx_farch_filter_table_remove(struct efx_nic *efx);
void efx_farch_filter_update_rx_scatter(struct efx_nic *efx);
s32 efx_farch_filter_insert(struct efx_nic *efx, struct efx_filter_spec *spec,
			    bool replace);
int efx_farch_filter_remove_safe(struct efx_nic *efx,
				 enum efx_filter_priority priority,
				 u32 filter_id);
int efx_farch_filter_get_safe(struct efx_nic *efx,
			      enum efx_filter_priority priority, u32 filter_id,
			      struct efx_filter_spec *);
int efx_farch_filter_clear_rx(struct efx_nic *efx,
			      enum efx_filter_priority priority);
u32 efx_farch_filter_count_rx_used(struct efx_nic *efx,
				   enum efx_filter_priority priority);
u32 efx_farch_filter_get_rx_id_limit(struct efx_nic *efx);
s32 efx_farch_filter_get_rx_ids(struct efx_nic *efx,
				enum efx_filter_priority priority, u32 *buf,
				u32 size);
#ifdef CONFIG_RFS_ACCEL
bool efx_farch_filter_rfs_expire_one(struct efx_nic *efx, u32 flow_id,
				     unsigned int index);
#endif
void efx_farch_filter_sync_rx_mode(struct efx_nic *efx);

<<<<<<< HEAD
bool efx_nic_event_present(struct efx_channel *channel);

/* Some statistics are computed as A - B where A and B each increase
 * linearly with some hardware counter(s) and the counters are read
 * asynchronously.  If the counters contributing to B are always read
 * after those contributing to A, the computed value may be lower than
 * the true value by some variable amount, and may decrease between
 * subsequent computations.
 *
 * We should never allow statistics to decrease or to exceed the true
 * value.  Since the computed value will never be greater than the
 * true value, we can achieve this by only storing the computed value
 * when it increases.
 */
static inline void efx_update_diff_stat(u64 *stat, u64 diff)
{
	if ((s64)(diff - *stat) > 0)
		*stat = diff;
}

/* Interrupts */
int efx_nic_init_interrupt(struct efx_nic *efx);
int efx_nic_irq_test_start(struct efx_nic *efx);
void efx_nic_fini_interrupt(struct efx_nic *efx);

=======
>>>>>>> upstream/android-13
/* Falcon/Siena interrupts */
void efx_farch_irq_enable_master(struct efx_nic *efx);
int efx_farch_irq_test_generate(struct efx_nic *efx);
void efx_farch_irq_disable_master(struct efx_nic *efx);
irqreturn_t efx_farch_msi_interrupt(int irq, void *dev_id);
irqreturn_t efx_farch_legacy_interrupt(int irq, void *dev_id);
irqreturn_t efx_farch_fatal_interrupt(struct efx_nic *efx);

<<<<<<< HEAD
static inline int efx_nic_event_test_irq_cpu(struct efx_channel *channel)
{
	return READ_ONCE(channel->event_test_cpu);
}
static inline int efx_nic_irq_test_irq_cpu(struct efx_nic *efx)
{
	return READ_ONCE(efx->last_irq_cpu);
}

/* Global Resources */
int efx_nic_flush_queues(struct efx_nic *efx);
=======
/* Global Resources */
>>>>>>> upstream/android-13
void siena_prepare_flush(struct efx_nic *efx);
int efx_farch_fini_dmaq(struct efx_nic *efx);
void efx_farch_finish_flr(struct efx_nic *efx);
void siena_finish_flush(struct efx_nic *efx);
void falcon_start_nic_stats(struct efx_nic *efx);
void falcon_stop_nic_stats(struct efx_nic *efx);
int falcon_reset_xaui(struct efx_nic *efx);
void efx_farch_dimension_resources(struct efx_nic *efx, unsigned sram_lim_qw);
void efx_farch_init_common(struct efx_nic *efx);
<<<<<<< HEAD
void efx_ef10_handle_drain_event(struct efx_nic *efx);
void efx_farch_rx_push_indir_table(struct efx_nic *efx);
void efx_farch_rx_pull_indir_table(struct efx_nic *efx);

int efx_nic_alloc_buffer(struct efx_nic *efx, struct efx_buffer *buffer,
			 unsigned int len, gfp_t gfp_flags);
void efx_nic_free_buffer(struct efx_nic *efx, struct efx_buffer *buffer);

=======
void efx_farch_rx_push_indir_table(struct efx_nic *efx);
void efx_farch_rx_pull_indir_table(struct efx_nic *efx);

>>>>>>> upstream/android-13
/* Tests */
struct efx_farch_register_test {
	unsigned address;
	efx_oword_t mask;
};
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
int efx_farch_test_registers(struct efx_nic *efx,
			     const struct efx_farch_register_test *regs,
			     size_t n_regs);

<<<<<<< HEAD
size_t efx_nic_get_regs_len(struct efx_nic *efx);
void efx_nic_get_regs(struct efx_nic *efx, void *buf);

size_t efx_nic_describe_stats(const struct efx_hw_stat_desc *desc, size_t count,
			      const unsigned long *mask, u8 *names);
void efx_nic_update_stats(const struct efx_hw_stat_desc *desc, size_t count,
			  const unsigned long *mask, u64 *stats,
			  const void *dma_buf, bool accumulate);
void efx_nic_fix_nodesc_drop_stat(struct efx_nic *efx, u64 *stat);

#define EFX_MAX_FLUSH_TIME 5000

=======
>>>>>>> upstream/android-13
void efx_farch_generate_event(struct efx_nic *efx, unsigned int evq,
			      efx_qword_t *event);

#endif /* EFX_NIC_H */
