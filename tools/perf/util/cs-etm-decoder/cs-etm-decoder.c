// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright(C) 2015-2018 Linaro Limited.
 *
 * Author: Tor Jeremiassen <tor@ti.com>
 * Author: Mathieu Poirier <mathieu.poirier@linaro.org>
 */

<<<<<<< HEAD
#include <linux/err.h>
#include <linux/list.h>
#include <stdlib.h>
#include <opencsd/c_api/opencsd_c_api.h>
#include <opencsd/etmv4/trc_pkt_types_etmv4.h>
#include <opencsd/ocsd_if_types.h>

#include "cs-etm.h"
#include "cs-etm-decoder.h"
#include "intlist.h"
#include "util.h"

#define MAX_BUFFER 1024
=======
#include <asm/bug.h>
#include <linux/coresight-pmu.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/zalloc.h>
#include <stdlib.h>
#include <opencsd/c_api/opencsd_c_api.h>

#include "cs-etm.h"
#include "cs-etm-decoder.h"
#include "debug.h"
#include "intlist.h"
>>>>>>> upstream/android-13

/* use raw logging */
#ifdef CS_DEBUG_RAW
#define CS_LOG_RAW_FRAMES
#ifdef CS_RAW_PACKED
#define CS_RAW_DEBUG_FLAGS (OCSD_DFRMTR_UNPACKED_RAW_OUT | \
			    OCSD_DFRMTR_PACKED_RAW_OUT)
#else
#define CS_RAW_DEBUG_FLAGS (OCSD_DFRMTR_UNPACKED_RAW_OUT)
#endif
#endif

<<<<<<< HEAD
#define CS_ETM_INVAL_ADDR	0xdeadbeefdeadbeefUL

struct cs_etm_decoder {
	void *data;
	void (*packet_printer)(const char *msg);
	bool trace_on;
	dcd_tree_handle_t dcd_tree;
	cs_etm_mem_cb_type mem_access;
	ocsd_datapath_resp_t prev_return;
	u32 packet_count;
	u32 head;
	u32 tail;
	struct cs_etm_packet packet_buffer[MAX_BUFFER];
=======
struct cs_etm_decoder {
	void *data;
	void (*packet_printer)(const char *msg);
	bool suppress_printing;
	dcd_tree_handle_t dcd_tree;
	cs_etm_mem_cb_type mem_access;
	ocsd_datapath_resp_t prev_return;
	const char *decoder_name;
>>>>>>> upstream/android-13
};

static u32
cs_etm_decoder__mem_access(const void *context,
			   const ocsd_vaddr_t address,
			   const ocsd_mem_space_acc_t mem_space __maybe_unused,
<<<<<<< HEAD
=======
			   const u8 trace_chan_id,
>>>>>>> upstream/android-13
			   const u32 req_size,
			   u8 *buffer)
{
	struct cs_etm_decoder *decoder = (struct cs_etm_decoder *) context;

<<<<<<< HEAD
	return decoder->mem_access(decoder->data,
				   address,
				   req_size,
				   buffer);
=======
	return decoder->mem_access(decoder->data, trace_chan_id,
				   address, req_size, buffer);
>>>>>>> upstream/android-13
}

int cs_etm_decoder__add_mem_access_cb(struct cs_etm_decoder *decoder,
				      u64 start, u64 end,
				      cs_etm_mem_cb_type cb_func)
{
	decoder->mem_access = cb_func;

<<<<<<< HEAD
	if (ocsd_dt_add_callback_mem_acc(decoder->dcd_tree, start, end,
					 OCSD_MEM_SPACE_ANY,
					 cs_etm_decoder__mem_access, decoder))
=======
	if (ocsd_dt_add_callback_trcid_mem_acc(decoder->dcd_tree, start, end,
					       OCSD_MEM_SPACE_ANY,
					       cs_etm_decoder__mem_access,
					       decoder))
>>>>>>> upstream/android-13
		return -1;

	return 0;
}

int cs_etm_decoder__reset(struct cs_etm_decoder *decoder)
{
	ocsd_datapath_resp_t dp_ret;

	decoder->prev_return = OCSD_RESP_CONT;
<<<<<<< HEAD

	dp_ret = ocsd_dt_process_data(decoder->dcd_tree, OCSD_OP_RESET,
				      0, 0, NULL, NULL);
=======
	decoder->suppress_printing = true;
	dp_ret = ocsd_dt_process_data(decoder->dcd_tree, OCSD_OP_RESET,
				      0, 0, NULL, NULL);
	decoder->suppress_printing = false;
>>>>>>> upstream/android-13
	if (OCSD_DATA_RESP_IS_FATAL(dp_ret))
		return -1;

	return 0;
}

<<<<<<< HEAD
int cs_etm_decoder__get_packet(struct cs_etm_decoder *decoder,
			       struct cs_etm_packet *packet)
{
	if (!decoder || !packet)
		return -EINVAL;

	/* Nothing to do, might as well just return */
	if (decoder->packet_count == 0)
=======
int cs_etm_decoder__get_packet(struct cs_etm_packet_queue *packet_queue,
			       struct cs_etm_packet *packet)
{
	if (!packet_queue || !packet)
		return -EINVAL;

	/* Nothing to do, might as well just return */
	if (packet_queue->packet_count == 0)
>>>>>>> upstream/android-13
		return 0;
	/*
	 * The queueing process in function cs_etm_decoder__buffer_packet()
	 * increments the tail *before* using it.  This is somewhat counter
	 * intuitive but it has the advantage of centralizing tail management
	 * at a single location.  Because of that we need to follow the same
	 * heuristic with the head, i.e we increment it before using its
	 * value.  Otherwise the first element of the packet queue is not
	 * used.
	 */
<<<<<<< HEAD
	decoder->head = (decoder->head + 1) & (MAX_BUFFER - 1);

	*packet = decoder->packet_buffer[decoder->head];

	decoder->packet_count--;
=======
	packet_queue->head = (packet_queue->head + 1) &
			     (CS_ETM_PACKET_MAX_BUFFER - 1);

	*packet = packet_queue->packet_buffer[packet_queue->head];

	packet_queue->packet_count--;
>>>>>>> upstream/android-13

	return 1;
}

<<<<<<< HEAD
=======
static int cs_etm_decoder__gen_etmv3_config(struct cs_etm_trace_params *params,
					    ocsd_etmv3_cfg *config)
{
	config->reg_idr = params->etmv3.reg_idr;
	config->reg_ctrl = params->etmv3.reg_ctrl;
	config->reg_ccer = params->etmv3.reg_ccer;
	config->reg_trc_id = params->etmv3.reg_trc_id;
	config->arch_ver = ARCH_V7;
	config->core_prof = profile_CortexA;

	return 0;
}

#define TRCIDR1_TRCARCHMIN_SHIFT 4
#define TRCIDR1_TRCARCHMIN_MASK  GENMASK(7, 4)
#define TRCIDR1_TRCARCHMIN(x)    (((x) & TRCIDR1_TRCARCHMIN_MASK) >> TRCIDR1_TRCARCHMIN_SHIFT)

static enum _ocsd_arch_version cs_etm_decoder__get_etmv4_arch_ver(u32 reg_idr1)
{
	/*
	 * For ETMv4 if the trace minor version is 4 or more then we can assume
	 * the architecture is ARCH_AA64 rather than just V8.
	 * ARCH_V8 = V8 architecture
	 * ARCH_AA64 = Min v8r3 plus additional AA64 PE features
	 */
	return TRCIDR1_TRCARCHMIN(reg_idr1) >= 4 ? ARCH_AA64 : ARCH_V8;
}

>>>>>>> upstream/android-13
static void cs_etm_decoder__gen_etmv4_config(struct cs_etm_trace_params *params,
					     ocsd_etmv4_cfg *config)
{
	config->reg_configr = params->etmv4.reg_configr;
	config->reg_traceidr = params->etmv4.reg_traceidr;
	config->reg_idr0 = params->etmv4.reg_idr0;
	config->reg_idr1 = params->etmv4.reg_idr1;
	config->reg_idr2 = params->etmv4.reg_idr2;
	config->reg_idr8 = params->etmv4.reg_idr8;
	config->reg_idr9 = 0;
	config->reg_idr10 = 0;
	config->reg_idr11 = 0;
	config->reg_idr12 = 0;
	config->reg_idr13 = 0;
<<<<<<< HEAD
	config->arch_ver = ARCH_V8;
=======
	config->arch_ver = cs_etm_decoder__get_etmv4_arch_ver(params->etmv4.reg_idr1);
	config->core_prof = profile_CortexA;
}

static void cs_etm_decoder__gen_ete_config(struct cs_etm_trace_params *params,
					   ocsd_ete_cfg *config)
{
	config->reg_configr = params->ete.reg_configr;
	config->reg_traceidr = params->ete.reg_traceidr;
	config->reg_idr0 = params->ete.reg_idr0;
	config->reg_idr1 = params->ete.reg_idr1;
	config->reg_idr2 = params->ete.reg_idr2;
	config->reg_idr8 = params->ete.reg_idr8;
	config->reg_devarch = params->ete.reg_devarch;
	config->arch_ver = ARCH_AA64;
>>>>>>> upstream/android-13
	config->core_prof = profile_CortexA;
}

static void cs_etm_decoder__print_str_cb(const void *p_context,
					 const char *msg,
					 const int str_len)
{
<<<<<<< HEAD
	if (p_context && str_len)
		((struct cs_etm_decoder *)p_context)->packet_printer(msg);
=======
	const struct cs_etm_decoder *decoder = p_context;

	if (p_context && str_len && !decoder->suppress_printing)
		decoder->packet_printer(msg);
>>>>>>> upstream/android-13
}

static int
cs_etm_decoder__init_def_logger_printing(struct cs_etm_decoder_params *d_params,
					 struct cs_etm_decoder *decoder)
{
	int ret = 0;

	if (d_params->packet_printer == NULL)
		return -1;

	decoder->packet_printer = d_params->packet_printer;

	/*
	 * Set up a library default logger to process any printers
	 * (packet/raw frame) we add later.
	 */
	ret = ocsd_def_errlog_init(OCSD_ERR_SEV_ERROR, 1);
	if (ret != 0)
		return -1;

	/* no stdout / err / file output */
	ret = ocsd_def_errlog_config_output(C_API_MSGLOGOUT_FLG_NONE, NULL);
	if (ret != 0)
		return -1;

	/*
	 * Set the string CB for the default logger, passes strings to
	 * perf print logger.
	 */
	ret = ocsd_def_errlog_set_strprint_cb(decoder->dcd_tree,
					      (void *)decoder,
					      cs_etm_decoder__print_str_cb);
	if (ret != 0)
		ret = -1;

	return 0;
}

#ifdef CS_LOG_RAW_FRAMES
static void
cs_etm_decoder__init_raw_frame_logging(struct cs_etm_decoder_params *d_params,
				       struct cs_etm_decoder *decoder)
{
	/* Only log these during a --dump operation */
	if (d_params->operation == CS_ETM_OPERATION_PRINT) {
		/* set up a library default logger to process the
		 *  raw frame printer we add later
		 */
		ocsd_def_errlog_init(OCSD_ERR_SEV_ERROR, 1);

		/* no stdout / err / file output */
		ocsd_def_errlog_config_output(C_API_MSGLOGOUT_FLG_NONE, NULL);

		/* set the string CB for the default logger,
		 * passes strings to perf print logger.
		 */
		ocsd_def_errlog_set_strprint_cb(decoder->dcd_tree,
						(void *)decoder,
						cs_etm_decoder__print_str_cb);

		/* use the built in library printer for the raw frames */
		ocsd_dt_set_raw_frame_printer(decoder->dcd_tree,
					      CS_RAW_DEBUG_FLAGS);
	}
}
#else
static void
cs_etm_decoder__init_raw_frame_logging(
		struct cs_etm_decoder_params *d_params __maybe_unused,
		struct cs_etm_decoder *decoder __maybe_unused)
{
}
#endif

<<<<<<< HEAD
static int cs_etm_decoder__create_packet_printer(struct cs_etm_decoder *decoder,
						 const char *decoder_name,
						 void *trace_config)
{
	u8 csid;

	if (ocsd_dt_create_decoder(decoder->dcd_tree, decoder_name,
				   OCSD_CREATE_FLG_PACKET_PROC,
				   trace_config, &csid))
		return -1;

	if (ocsd_dt_set_pkt_protocol_printer(decoder->dcd_tree, csid, 0))
		return -1;

	return 0;
}

static int
cs_etm_decoder__create_etm_packet_printer(struct cs_etm_trace_params *t_params,
					  struct cs_etm_decoder *decoder)
{
	const char *decoder_name;
	ocsd_etmv4_cfg trace_config_etmv4;
	void *trace_config;

	switch (t_params->protocol) {
	case CS_ETM_PROTO_ETMV4i:
		cs_etm_decoder__gen_etmv4_config(t_params, &trace_config_etmv4);
		decoder_name = OCSD_BUILTIN_DCD_ETMV4I;
		trace_config = &trace_config_etmv4;
		break;
	default:
		return -1;
	}

	return cs_etm_decoder__create_packet_printer(decoder,
						     decoder_name,
						     trace_config);
}

static void cs_etm_decoder__clear_buffer(struct cs_etm_decoder *decoder)
{
	int i;

	decoder->head = 0;
	decoder->tail = 0;
	decoder->packet_count = 0;
	for (i = 0; i < MAX_BUFFER; i++) {
		decoder->packet_buffer[i].start_addr = CS_ETM_INVAL_ADDR;
		decoder->packet_buffer[i].end_addr = CS_ETM_INVAL_ADDR;
		decoder->packet_buffer[i].last_instr_taken_branch = false;
		decoder->packet_buffer[i].exc = false;
		decoder->packet_buffer[i].exc_ret = false;
		decoder->packet_buffer[i].cpu = INT_MIN;
	}
}

static ocsd_datapath_resp_t
cs_etm_decoder__buffer_packet(struct cs_etm_decoder *decoder,
=======
static ocsd_datapath_resp_t
cs_etm_decoder__do_soft_timestamp(struct cs_etm_queue *etmq,
				  struct cs_etm_packet_queue *packet_queue,
				  const uint8_t trace_chan_id)
{
	/* No timestamp packet has been received, nothing to do */
	if (!packet_queue->cs_timestamp)
		return OCSD_RESP_CONT;

	packet_queue->cs_timestamp = packet_queue->next_cs_timestamp;

	/* Estimate the timestamp for the next range packet */
	packet_queue->next_cs_timestamp += packet_queue->instr_count;
	packet_queue->instr_count = 0;

	/* Tell the front end which traceid_queue needs attention */
	cs_etm__etmq_set_traceid_queue_timestamp(etmq, trace_chan_id);

	return OCSD_RESP_WAIT;
}

static ocsd_datapath_resp_t
cs_etm_decoder__do_hard_timestamp(struct cs_etm_queue *etmq,
				  const ocsd_generic_trace_elem *elem,
				  const uint8_t trace_chan_id,
				  const ocsd_trc_index_t indx)
{
	struct cs_etm_packet_queue *packet_queue;

	/* First get the packet queue for this traceID */
	packet_queue = cs_etm__etmq_get_packet_queue(etmq, trace_chan_id);
	if (!packet_queue)
		return OCSD_RESP_FATAL_SYS_ERR;

	/*
	 * We've seen a timestamp packet before - simply record the new value.
	 * Function do_soft_timestamp() will report the value to the front end,
	 * hence asking the decoder to keep decoding rather than stopping.
	 */
	if (packet_queue->cs_timestamp) {
		packet_queue->next_cs_timestamp = elem->timestamp;
		return OCSD_RESP_CONT;
	}


	if (!elem->timestamp) {
		/*
		 * Zero timestamps can be seen due to misconfiguration or hardware bugs.
		 * Warn once, and don't try to subtract instr_count as it would result in an
		 * underflow.
		 */
		packet_queue->cs_timestamp = 0;
		if (!cs_etm__etmq_is_timeless(etmq))
			pr_warning_once("Zero Coresight timestamp found at Idx:%" OCSD_TRC_IDX_STR
					". Decoding may be improved by prepending 'Z' to your current --itrace arguments.\n",
					indx);

	} else if (packet_queue->instr_count > elem->timestamp) {
		/*
		 * Sanity check that the elem->timestamp - packet_queue->instr_count would not
		 * result in an underflow. Warn and clamp at 0 if it would.
		 */
		packet_queue->cs_timestamp = 0;
		pr_err("Timestamp calculation underflow at Idx:%" OCSD_TRC_IDX_STR "\n", indx);
	} else {
		/*
		 * This is the first timestamp we've seen since the beginning of traces
		 * or a discontinuity.  Since timestamps packets are generated *after*
		 * range packets have been generated, we need to estimate the time at
		 * which instructions started by subtracting the number of instructions
		 * executed to the timestamp.
		 */
		packet_queue->cs_timestamp = elem->timestamp - packet_queue->instr_count;
	}
	packet_queue->next_cs_timestamp = elem->timestamp;
	packet_queue->instr_count = 0;

	/* Tell the front end which traceid_queue needs attention */
	cs_etm__etmq_set_traceid_queue_timestamp(etmq, trace_chan_id);

	/* Halt processing until we are being told to proceed */
	return OCSD_RESP_WAIT;
}

static void
cs_etm_decoder__reset_timestamp(struct cs_etm_packet_queue *packet_queue)
{
	packet_queue->cs_timestamp = 0;
	packet_queue->next_cs_timestamp = 0;
	packet_queue->instr_count = 0;
}

static ocsd_datapath_resp_t
cs_etm_decoder__buffer_packet(struct cs_etm_packet_queue *packet_queue,
>>>>>>> upstream/android-13
			      const u8 trace_chan_id,
			      enum cs_etm_sample_type sample_type)
{
	u32 et = 0;
	int cpu;

<<<<<<< HEAD
	if (decoder->packet_count >= MAX_BUFFER - 1)
=======
	if (packet_queue->packet_count >= CS_ETM_PACKET_MAX_BUFFER - 1)
>>>>>>> upstream/android-13
		return OCSD_RESP_FATAL_SYS_ERR;

	if (cs_etm__get_cpu(trace_chan_id, &cpu) < 0)
		return OCSD_RESP_FATAL_SYS_ERR;

<<<<<<< HEAD
	et = decoder->tail;
	et = (et + 1) & (MAX_BUFFER - 1);
	decoder->tail = et;
	decoder->packet_count++;

	decoder->packet_buffer[et].sample_type = sample_type;
	decoder->packet_buffer[et].exc = false;
	decoder->packet_buffer[et].exc_ret = false;
	decoder->packet_buffer[et].cpu = cpu;
	decoder->packet_buffer[et].start_addr = CS_ETM_INVAL_ADDR;
	decoder->packet_buffer[et].end_addr = CS_ETM_INVAL_ADDR;

	if (decoder->packet_count == MAX_BUFFER - 1)
=======
	et = packet_queue->tail;
	et = (et + 1) & (CS_ETM_PACKET_MAX_BUFFER - 1);
	packet_queue->tail = et;
	packet_queue->packet_count++;

	packet_queue->packet_buffer[et].sample_type = sample_type;
	packet_queue->packet_buffer[et].isa = CS_ETM_ISA_UNKNOWN;
	packet_queue->packet_buffer[et].cpu = cpu;
	packet_queue->packet_buffer[et].start_addr = CS_ETM_INVAL_ADDR;
	packet_queue->packet_buffer[et].end_addr = CS_ETM_INVAL_ADDR;
	packet_queue->packet_buffer[et].instr_count = 0;
	packet_queue->packet_buffer[et].last_instr_taken_branch = false;
	packet_queue->packet_buffer[et].last_instr_size = 0;
	packet_queue->packet_buffer[et].last_instr_type = 0;
	packet_queue->packet_buffer[et].last_instr_subtype = 0;
	packet_queue->packet_buffer[et].last_instr_cond = 0;
	packet_queue->packet_buffer[et].flags = 0;
	packet_queue->packet_buffer[et].exception_number = UINT32_MAX;
	packet_queue->packet_buffer[et].trace_chan_id = trace_chan_id;

	if (packet_queue->packet_count == CS_ETM_PACKET_MAX_BUFFER - 1)
>>>>>>> upstream/android-13
		return OCSD_RESP_WAIT;

	return OCSD_RESP_CONT;
}

static ocsd_datapath_resp_t
<<<<<<< HEAD
cs_etm_decoder__buffer_range(struct cs_etm_decoder *decoder,
=======
cs_etm_decoder__buffer_range(struct cs_etm_queue *etmq,
			     struct cs_etm_packet_queue *packet_queue,
>>>>>>> upstream/android-13
			     const ocsd_generic_trace_elem *elem,
			     const uint8_t trace_chan_id)
{
	int ret = 0;
	struct cs_etm_packet *packet;

<<<<<<< HEAD
	ret = cs_etm_decoder__buffer_packet(decoder, trace_chan_id,
=======
	ret = cs_etm_decoder__buffer_packet(packet_queue, trace_chan_id,
>>>>>>> upstream/android-13
					    CS_ETM_RANGE);
	if (ret != OCSD_RESP_CONT && ret != OCSD_RESP_WAIT)
		return ret;

<<<<<<< HEAD
	packet = &decoder->packet_buffer[decoder->tail];

	packet->start_addr = elem->st_addr;
	packet->end_addr = elem->en_addr;
	switch (elem->last_i_type) {
	case OCSD_INSTR_BR:
	case OCSD_INSTR_BR_INDIRECT:
		packet->last_instr_taken_branch = elem->last_instr_exec;
		break;
	case OCSD_INSTR_ISB:
	case OCSD_INSTR_DSB_DMB:
	case OCSD_INSTR_OTHER:
	default:
		packet->last_instr_taken_branch = false;
		break;
	}
=======
	packet = &packet_queue->packet_buffer[packet_queue->tail];

	switch (elem->isa) {
	case ocsd_isa_aarch64:
		packet->isa = CS_ETM_ISA_A64;
		break;
	case ocsd_isa_arm:
		packet->isa = CS_ETM_ISA_A32;
		break;
	case ocsd_isa_thumb2:
		packet->isa = CS_ETM_ISA_T32;
		break;
	case ocsd_isa_tee:
	case ocsd_isa_jazelle:
	case ocsd_isa_custom:
	case ocsd_isa_unknown:
	default:
		packet->isa = CS_ETM_ISA_UNKNOWN;
	}

	packet->start_addr = elem->st_addr;
	packet->end_addr = elem->en_addr;
	packet->instr_count = elem->num_instr_range;
	packet->last_instr_type = elem->last_i_type;
	packet->last_instr_subtype = elem->last_i_subtype;
	packet->last_instr_cond = elem->last_instr_cond;

	if (elem->last_i_type == OCSD_INSTR_BR || elem->last_i_type == OCSD_INSTR_BR_INDIRECT)
		packet->last_instr_taken_branch = elem->last_instr_exec;
	else
		packet->last_instr_taken_branch = false;

	packet->last_instr_size = elem->last_instr_sz;

	/* per-thread scenario, no need to generate a timestamp */
	if (cs_etm__etmq_is_timeless(etmq))
		goto out;

	/*
	 * The packet queue is full and we haven't seen a timestamp (had we
	 * seen one the packet queue wouldn't be full).  Let the front end
	 * deal with it.
	 */
	if (ret == OCSD_RESP_WAIT)
		goto out;

	packet_queue->instr_count += elem->num_instr_range;
	/* Tell the front end we have a new timestamp to process */
	ret = cs_etm_decoder__do_soft_timestamp(etmq, packet_queue,
						trace_chan_id);
out:
	return ret;
}

static ocsd_datapath_resp_t
cs_etm_decoder__buffer_discontinuity(struct cs_etm_packet_queue *queue,
				     const uint8_t trace_chan_id)
{
	/*
	 * Something happened and who knows when we'll get new traces so
	 * reset time statistics.
	 */
	cs_etm_decoder__reset_timestamp(queue);
	return cs_etm_decoder__buffer_packet(queue, trace_chan_id,
					     CS_ETM_DISCONTINUITY);
}

static ocsd_datapath_resp_t
cs_etm_decoder__buffer_exception(struct cs_etm_packet_queue *queue,
				 const ocsd_generic_trace_elem *elem,
				 const uint8_t trace_chan_id)
{	int ret = 0;
	struct cs_etm_packet *packet;

	ret = cs_etm_decoder__buffer_packet(queue, trace_chan_id,
					    CS_ETM_EXCEPTION);
	if (ret != OCSD_RESP_CONT && ret != OCSD_RESP_WAIT)
		return ret;

	packet = &queue->packet_buffer[queue->tail];
	packet->exception_number = elem->exception_number;
>>>>>>> upstream/android-13

	return ret;
}

static ocsd_datapath_resp_t
<<<<<<< HEAD
cs_etm_decoder__buffer_trace_on(struct cs_etm_decoder *decoder,
				const uint8_t trace_chan_id)
{
	return cs_etm_decoder__buffer_packet(decoder, trace_chan_id,
					     CS_ETM_TRACE_ON);
=======
cs_etm_decoder__buffer_exception_ret(struct cs_etm_packet_queue *queue,
				     const uint8_t trace_chan_id)
{
	return cs_etm_decoder__buffer_packet(queue, trace_chan_id,
					     CS_ETM_EXCEPTION_RET);
}

static ocsd_datapath_resp_t
cs_etm_decoder__set_tid(struct cs_etm_queue *etmq,
			struct cs_etm_packet_queue *packet_queue,
			const ocsd_generic_trace_elem *elem,
			const uint8_t trace_chan_id)
{
	pid_t tid = -1;
	static u64 pid_fmt;
	int ret;

	/*
	 * As all the ETMs run at the same exception level, the system should
	 * have the same PID format crossing CPUs.  So cache the PID format
	 * and reuse it for sequential decoding.
	 */
	if (!pid_fmt) {
		ret = cs_etm__get_pid_fmt(trace_chan_id, &pid_fmt);
		if (ret)
			return OCSD_RESP_FATAL_SYS_ERR;
	}

	/*
	 * Process the PE_CONTEXT packets if we have a valid contextID or VMID.
	 * If the kernel is running at EL2, the PID is traced in CONTEXTIDR_EL2
	 * as VMID, Bit ETM_OPT_CTXTID2 is set in this case.
	 */
	switch (pid_fmt) {
	case BIT(ETM_OPT_CTXTID):
		if (elem->context.ctxt_id_valid)
			tid = elem->context.context_id;
		break;
	case BIT(ETM_OPT_CTXTID2):
		if (elem->context.vmid_valid)
			tid = elem->context.vmid;
		break;
	default:
		break;
	}

	if (tid == -1)
		return OCSD_RESP_CONT;

	if (cs_etm__etmq_set_tid(etmq, tid, trace_chan_id))
		return OCSD_RESP_FATAL_SYS_ERR;

	/*
	 * A timestamp is generated after a PE_CONTEXT element so make sure
	 * to rely on that coming one.
	 */
	cs_etm_decoder__reset_timestamp(packet_queue);

	return OCSD_RESP_CONT;
>>>>>>> upstream/android-13
}

static ocsd_datapath_resp_t cs_etm_decoder__gen_trace_elem_printer(
				const void *context,
<<<<<<< HEAD
				const ocsd_trc_index_t indx __maybe_unused,
=======
				const ocsd_trc_index_t indx,
>>>>>>> upstream/android-13
				const u8 trace_chan_id __maybe_unused,
				const ocsd_generic_trace_elem *elem)
{
	ocsd_datapath_resp_t resp = OCSD_RESP_CONT;
	struct cs_etm_decoder *decoder = (struct cs_etm_decoder *) context;
<<<<<<< HEAD
=======
	struct cs_etm_queue *etmq = decoder->data;
	struct cs_etm_packet_queue *packet_queue;

	/* First get the packet queue for this traceID */
	packet_queue = cs_etm__etmq_get_packet_queue(etmq, trace_chan_id);
	if (!packet_queue)
		return OCSD_RESP_FATAL_SYS_ERR;
>>>>>>> upstream/android-13

	switch (elem->elem_type) {
	case OCSD_GEN_TRC_ELEM_UNKNOWN:
		break;
<<<<<<< HEAD
	case OCSD_GEN_TRC_ELEM_NO_SYNC:
		decoder->trace_on = false;
		break;
	case OCSD_GEN_TRC_ELEM_TRACE_ON:
		resp = cs_etm_decoder__buffer_trace_on(decoder,
						       trace_chan_id);
		decoder->trace_on = true;
		break;
	case OCSD_GEN_TRC_ELEM_INSTR_RANGE:
		resp = cs_etm_decoder__buffer_range(decoder, elem,
						    trace_chan_id);
		break;
	case OCSD_GEN_TRC_ELEM_EXCEPTION:
		decoder->packet_buffer[decoder->tail].exc = true;
		break;
	case OCSD_GEN_TRC_ELEM_EXCEPTION_RET:
		decoder->packet_buffer[decoder->tail].exc_ret = true;
		break;
	case OCSD_GEN_TRC_ELEM_PE_CONTEXT:
	case OCSD_GEN_TRC_ELEM_EO_TRACE:
	case OCSD_GEN_TRC_ELEM_ADDR_NACC:
	case OCSD_GEN_TRC_ELEM_TIMESTAMP:
=======
	case OCSD_GEN_TRC_ELEM_EO_TRACE:
	case OCSD_GEN_TRC_ELEM_NO_SYNC:
	case OCSD_GEN_TRC_ELEM_TRACE_ON:
		resp = cs_etm_decoder__buffer_discontinuity(packet_queue,
							    trace_chan_id);
		break;
	case OCSD_GEN_TRC_ELEM_INSTR_RANGE:
		resp = cs_etm_decoder__buffer_range(etmq, packet_queue, elem,
						    trace_chan_id);
		break;
	case OCSD_GEN_TRC_ELEM_EXCEPTION:
		resp = cs_etm_decoder__buffer_exception(packet_queue, elem,
							trace_chan_id);
		break;
	case OCSD_GEN_TRC_ELEM_EXCEPTION_RET:
		resp = cs_etm_decoder__buffer_exception_ret(packet_queue,
							    trace_chan_id);
		break;
	case OCSD_GEN_TRC_ELEM_TIMESTAMP:
		resp = cs_etm_decoder__do_hard_timestamp(etmq, elem,
							 trace_chan_id,
							 indx);
		break;
	case OCSD_GEN_TRC_ELEM_PE_CONTEXT:
		resp = cs_etm_decoder__set_tid(etmq, packet_queue,
					       elem, trace_chan_id);
		break;
	/* Unused packet types */
	case OCSD_GEN_TRC_ELEM_I_RANGE_NOPATH:
	case OCSD_GEN_TRC_ELEM_ADDR_NACC:
>>>>>>> upstream/android-13
	case OCSD_GEN_TRC_ELEM_CYCLE_COUNT:
	case OCSD_GEN_TRC_ELEM_ADDR_UNKNOWN:
	case OCSD_GEN_TRC_ELEM_EVENT:
	case OCSD_GEN_TRC_ELEM_SWTRACE:
	case OCSD_GEN_TRC_ELEM_CUSTOM:
<<<<<<< HEAD
=======
	case OCSD_GEN_TRC_ELEM_SYNC_MARKER:
	case OCSD_GEN_TRC_ELEM_MEMTRANS:
>>>>>>> upstream/android-13
	default:
		break;
	}

	return resp;
}

<<<<<<< HEAD
static int cs_etm_decoder__create_etm_packet_decoder(
					struct cs_etm_trace_params *t_params,
					struct cs_etm_decoder *decoder)
{
	const char *decoder_name;
	ocsd_etmv4_cfg trace_config_etmv4;
	void *trace_config;
	u8 csid;

	switch (t_params->protocol) {
	case CS_ETM_PROTO_ETMV4i:
		cs_etm_decoder__gen_etmv4_config(t_params, &trace_config_etmv4);
		decoder_name = OCSD_BUILTIN_DCD_ETMV4I;
		trace_config = &trace_config_etmv4;
		break;
	default:
		return -1;
	}

	if (ocsd_dt_create_decoder(decoder->dcd_tree,
				     decoder_name,
				     OCSD_CREATE_FLG_FULL_DECODER,
				     trace_config, &csid))
		return -1;

	if (ocsd_dt_set_gen_elem_outfn(decoder->dcd_tree,
				       cs_etm_decoder__gen_trace_elem_printer,
				       decoder))
		return -1;

	return 0;
}

=======
>>>>>>> upstream/android-13
static int
cs_etm_decoder__create_etm_decoder(struct cs_etm_decoder_params *d_params,
				   struct cs_etm_trace_params *t_params,
				   struct cs_etm_decoder *decoder)
{
<<<<<<< HEAD
	if (d_params->operation == CS_ETM_OPERATION_PRINT)
		return cs_etm_decoder__create_etm_packet_printer(t_params,
								 decoder);
	else if (d_params->operation == CS_ETM_OPERATION_DECODE)
		return cs_etm_decoder__create_etm_packet_decoder(t_params,
								 decoder);
=======
	ocsd_etmv3_cfg config_etmv3;
	ocsd_etmv4_cfg trace_config_etmv4;
	ocsd_ete_cfg trace_config_ete;
	void *trace_config;
	u8 csid;

	switch (t_params->protocol) {
	case CS_ETM_PROTO_ETMV3:
	case CS_ETM_PROTO_PTM:
		cs_etm_decoder__gen_etmv3_config(t_params, &config_etmv3);
		decoder->decoder_name = (t_params->protocol == CS_ETM_PROTO_ETMV3) ?
							OCSD_BUILTIN_DCD_ETMV3 :
							OCSD_BUILTIN_DCD_PTM;
		trace_config = &config_etmv3;
		break;
	case CS_ETM_PROTO_ETMV4i:
		cs_etm_decoder__gen_etmv4_config(t_params, &trace_config_etmv4);
		decoder->decoder_name = OCSD_BUILTIN_DCD_ETMV4I;
		trace_config = &trace_config_etmv4;
		break;
	case CS_ETM_PROTO_ETE:
		cs_etm_decoder__gen_ete_config(t_params, &trace_config_ete);
		decoder->decoder_name = OCSD_BUILTIN_DCD_ETE;
		trace_config = &trace_config_ete;
		break;
	default:
		return -1;
	}

	if (d_params->operation == CS_ETM_OPERATION_DECODE) {
		if (ocsd_dt_create_decoder(decoder->dcd_tree,
					   decoder->decoder_name,
					   OCSD_CREATE_FLG_FULL_DECODER,
					   trace_config, &csid))
			return -1;

		if (ocsd_dt_set_gen_elem_outfn(decoder->dcd_tree,
					       cs_etm_decoder__gen_trace_elem_printer,
					       decoder))
			return -1;

		return 0;
	} else if (d_params->operation == CS_ETM_OPERATION_PRINT) {
		if (ocsd_dt_create_decoder(decoder->dcd_tree, decoder->decoder_name,
					   OCSD_CREATE_FLG_PACKET_PROC,
					   trace_config, &csid))
			return -1;

		if (ocsd_dt_set_pkt_protocol_printer(decoder->dcd_tree, csid, 0))
			return -1;

		return 0;
	}
>>>>>>> upstream/android-13

	return -1;
}

struct cs_etm_decoder *
<<<<<<< HEAD
cs_etm_decoder__new(int num_cpu, struct cs_etm_decoder_params *d_params,
=======
cs_etm_decoder__new(int decoders, struct cs_etm_decoder_params *d_params,
>>>>>>> upstream/android-13
		    struct cs_etm_trace_params t_params[])
{
	struct cs_etm_decoder *decoder;
	ocsd_dcd_tree_src_t format;
	u32 flags;
	int i, ret;

	if ((!t_params) || (!d_params))
		return NULL;

	decoder = zalloc(sizeof(*decoder));

	if (!decoder)
		return NULL;

	decoder->data = d_params->data;
	decoder->prev_return = OCSD_RESP_CONT;
<<<<<<< HEAD
	cs_etm_decoder__clear_buffer(decoder);
=======
>>>>>>> upstream/android-13
	format = (d_params->formatted ? OCSD_TRC_SRC_FRAME_FORMATTED :
					 OCSD_TRC_SRC_SINGLE);
	flags = 0;
	flags |= (d_params->fsyncs ? OCSD_DFRMTR_HAS_FSYNCS : 0);
	flags |= (d_params->hsyncs ? OCSD_DFRMTR_HAS_HSYNCS : 0);
	flags |= (d_params->frame_aligned ? OCSD_DFRMTR_FRAME_MEM_ALIGN : 0);

	/*
	 * Drivers may add barrier frames when used with perf, set up to
	 * handle this. Barriers const of FSYNC packet repeated 4 times.
	 */
	flags |= OCSD_DFRMTR_RESET_ON_4X_FSYNC;

	/* Create decode tree for the data source */
	decoder->dcd_tree = ocsd_create_dcd_tree(format, flags);

	if (decoder->dcd_tree == 0)
		goto err_free_decoder;

	/* init library print logging support */
	ret = cs_etm_decoder__init_def_logger_printing(d_params, decoder);
	if (ret != 0)
<<<<<<< HEAD
		goto err_free_decoder_tree;
=======
		goto err_free_decoder;
>>>>>>> upstream/android-13

	/* init raw frame logging if required */
	cs_etm_decoder__init_raw_frame_logging(d_params, decoder);

<<<<<<< HEAD
	for (i = 0; i < num_cpu; i++) {
=======
	for (i = 0; i < decoders; i++) {
>>>>>>> upstream/android-13
		ret = cs_etm_decoder__create_etm_decoder(d_params,
							 &t_params[i],
							 decoder);
		if (ret != 0)
<<<<<<< HEAD
			goto err_free_decoder_tree;
=======
			goto err_free_decoder;
>>>>>>> upstream/android-13
	}

	return decoder;

<<<<<<< HEAD
err_free_decoder_tree:
	ocsd_destroy_dcd_tree(decoder->dcd_tree);
err_free_decoder:
	free(decoder);
=======
err_free_decoder:
	cs_etm_decoder__free(decoder);
>>>>>>> upstream/android-13
	return NULL;
}

int cs_etm_decoder__process_data_block(struct cs_etm_decoder *decoder,
				       u64 indx, const u8 *buf,
				       size_t len, size_t *consumed)
{
	int ret = 0;
	ocsd_datapath_resp_t cur = OCSD_RESP_CONT;
	ocsd_datapath_resp_t prev_return = decoder->prev_return;
	size_t processed = 0;
	u32 count;

	while (processed < len) {
		if (OCSD_DATA_RESP_IS_WAIT(prev_return)) {
			cur = ocsd_dt_process_data(decoder->dcd_tree,
						   OCSD_OP_FLUSH,
						   0,
						   0,
						   NULL,
						   NULL);
		} else if (OCSD_DATA_RESP_IS_CONT(prev_return)) {
			cur = ocsd_dt_process_data(decoder->dcd_tree,
						   OCSD_OP_DATA,
						   indx + processed,
						   len - processed,
						   &buf[processed],
						   &count);
			processed += count;
		} else {
			ret = -EINVAL;
			break;
		}

		/*
		 * Return to the input code if the packet buffer is full.
		 * Flushing will get done once the packet buffer has been
		 * processed.
		 */
		if (OCSD_DATA_RESP_IS_WAIT(cur))
			break;

		prev_return = cur;
	}

	decoder->prev_return = cur;
	*consumed = processed;

	return ret;
}

void cs_etm_decoder__free(struct cs_etm_decoder *decoder)
{
	if (!decoder)
		return;

	ocsd_destroy_dcd_tree(decoder->dcd_tree);
	decoder->dcd_tree = NULL;
	free(decoder);
}
<<<<<<< HEAD
=======

const char *cs_etm_decoder__get_name(struct cs_etm_decoder *decoder)
{
	return decoder->decoder_name;
}
>>>>>>> upstream/android-13
