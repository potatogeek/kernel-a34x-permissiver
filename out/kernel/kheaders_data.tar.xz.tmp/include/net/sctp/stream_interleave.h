

#ifndef __sctp_stream_interleave_h__
#define __sctp_stream_interleave_h__

struct sctp_stream_interleave {
	__u16	data_chunk_len;
	__u16	ftsn_chunk_len;
	
	struct sctp_chunk *(*make_datafrag)(const struct sctp_association *asoc,
					    const struct sctp_sndrcvinfo *sinfo,
					    int len, __u8 flags, gfp_t gfp);
	void	(*assign_number)(struct sctp_chunk *chunk);
	bool	(*validate_data)(struct sctp_chunk *chunk);
	int	(*ulpevent_data)(struct sctp_ulpq *ulpq,
				 struct sctp_chunk *chunk, gfp_t gfp);
	int	(*enqueue_event)(struct sctp_ulpq *ulpq,
				 struct sctp_ulpevent *event);
	void	(*renege_events)(struct sctp_ulpq *ulpq,
				 struct sctp_chunk *chunk, gfp_t gfp);
	void	(*start_pd)(struct sctp_ulpq *ulpq, gfp_t gfp);
	void	(*abort_pd)(struct sctp_ulpq *ulpq, gfp_t gfp);
	
	void	(*generate_ftsn)(struct sctp_outq *q, __u32 ctsn);
	bool	(*validate_ftsn)(struct sctp_chunk *chunk);
	void	(*report_ftsn)(struct sctp_ulpq *ulpq, __u32 ftsn);
	void	(*handle_ftsn)(struct sctp_ulpq *ulpq,
			       struct sctp_chunk *chunk);
};

void sctp_stream_interleave_init(struct sctp_stream *stream);

#endif 
