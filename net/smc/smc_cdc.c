// SPDX-License-Identifier: GPL-2.0
/*
 * Shared Memory Communications over RDMA (SMC-R) and RoCE
 *
 * Connection Data Control (CDC)
 * handles flow control
 *
 * Copyright IBM Corp. 2016
 *
 * Author(s):  Ursula Braun <ubraun@linux.vnet.ibm.com>
 */

#include <linux/spinlock.h>

#include "smc.h"
#include "smc_wr.h"
#include "smc_cdc.h"
#include "smc_tx.h"
#include "smc_rx.h"
#include "smc_close.h"

/********************************** send *************************************/

<<<<<<< HEAD
struct smc_cdc_tx_pend {
	struct smc_connection	*conn;		/* socket connection */
	union smc_host_cursor	cursor;	/* tx sndbuf cursor sent */
	union smc_host_cursor	p_cursor;	/* rx RMBE cursor produced */
	u16			ctrl_seq;	/* conn. tx sequence # */
};

=======
>>>>>>> upstream/android-13
/* handler for send/transmission completion of a CDC msg */
static void smc_cdc_tx_handler(struct smc_wr_tx_pend_priv *pnd_snd,
			       struct smc_link *link,
			       enum ib_wc_status wc_status)
{
	struct smc_cdc_tx_pend *cdcpend = (struct smc_cdc_tx_pend *)pnd_snd;
	struct smc_connection *conn = cdcpend->conn;
	struct smc_sock *smc;
	int diff;

<<<<<<< HEAD
	if (!conn)
		/* already dismissed */
		return;

=======
>>>>>>> upstream/android-13
	smc = container_of(conn, struct smc_sock, conn);
	bh_lock_sock(&smc->sk);
	if (!wc_status) {
		diff = smc_curs_diff(cdcpend->conn->sndbuf_desc->len,
				     &cdcpend->conn->tx_curs_fin,
				     &cdcpend->cursor);
		/* sndbuf_space is decreased in smc_sendmsg */
		smp_mb__before_atomic();
		atomic_add(diff, &cdcpend->conn->sndbuf_space);
		/* guarantee 0 <= sndbuf_space <= sndbuf_desc->len */
		smp_mb__after_atomic();
		smc_curs_copy(&conn->tx_curs_fin, &cdcpend->cursor, conn);
<<<<<<< HEAD
	}
=======
		smc_curs_copy(&conn->local_tx_ctrl_fin, &cdcpend->p_cursor,
			      conn);
		conn->tx_cdc_seq_fin = cdcpend->ctrl_seq;
	}

	if (atomic_dec_and_test(&conn->cdc_pend_tx_wr) &&
	    unlikely(wq_has_sleeper(&conn->cdc_pend_tx_wq)))
		wake_up(&conn->cdc_pend_tx_wq);
	WARN_ON(atomic_read(&conn->cdc_pend_tx_wr) < 0);

>>>>>>> upstream/android-13
	smc_tx_sndbuf_nonfull(smc);
	bh_unlock_sock(&smc->sk);
}

int smc_cdc_get_free_slot(struct smc_connection *conn,
<<<<<<< HEAD
			  struct smc_wr_buf **wr_buf,
			  struct smc_cdc_tx_pend **pend)
{
	struct smc_link *link = &conn->lgr->lnk[SMC_SINGLE_LINK];
	int rc;

	rc = smc_wr_tx_get_free_slot(link, smc_cdc_tx_handler, wr_buf,
				     (struct smc_wr_tx_pend_priv **)pend);
	if (!conn->alert_token_local)
		/* abnormal termination */
		rc = -EPIPE;
=======
			  struct smc_link *link,
			  struct smc_wr_buf **wr_buf,
			  struct smc_rdma_wr **wr_rdma_buf,
			  struct smc_cdc_tx_pend **pend)
{
	int rc;

	rc = smc_wr_tx_get_free_slot(link, smc_cdc_tx_handler, wr_buf,
				     wr_rdma_buf,
				     (struct smc_wr_tx_pend_priv **)pend);
	if (conn->killed) {
		/* abnormal termination */
		if (!rc)
			smc_wr_tx_put_slot(link,
					   (struct smc_wr_tx_pend_priv *)pend);
		rc = -EPIPE;
	}
>>>>>>> upstream/android-13
	return rc;
}

static inline void smc_cdc_add_pending_send(struct smc_connection *conn,
					    struct smc_cdc_tx_pend *pend)
{
	BUILD_BUG_ON_MSG(
		sizeof(struct smc_cdc_msg) > SMC_WR_BUF_SIZE,
		"must increase SMC_WR_BUF_SIZE to at least sizeof(struct smc_cdc_msg)");
	BUILD_BUG_ON_MSG(
<<<<<<< HEAD
		sizeof(struct smc_cdc_msg) != SMC_WR_TX_SIZE,
=======
		offsetofend(struct smc_cdc_msg, reserved) > SMC_WR_TX_SIZE,
>>>>>>> upstream/android-13
		"must adapt SMC_WR_TX_SIZE to sizeof(struct smc_cdc_msg); if not all smc_wr upper layer protocols use the same message size any more, must start to set link->wr_tx_sges[i].length on each individual smc_wr_tx_send()");
	BUILD_BUG_ON_MSG(
		sizeof(struct smc_cdc_tx_pend) > SMC_WR_TX_PEND_PRIV_SIZE,
		"must increase SMC_WR_TX_PEND_PRIV_SIZE to at least sizeof(struct smc_cdc_tx_pend)");
	pend->conn = conn;
	pend->cursor = conn->tx_curs_sent;
	pend->p_cursor = conn->local_tx_ctrl.prod;
	pend->ctrl_seq = conn->tx_cdc_seq;
}

int smc_cdc_msg_send(struct smc_connection *conn,
		     struct smc_wr_buf *wr_buf,
		     struct smc_cdc_tx_pend *pend)
{
<<<<<<< HEAD
	union smc_host_cursor cfed;
	struct smc_link *link;
	int rc;

	link = &conn->lgr->lnk[SMC_SINGLE_LINK];

=======
	struct smc_link *link = conn->lnk;
	union smc_host_cursor cfed;
	int rc;

>>>>>>> upstream/android-13
	smc_cdc_add_pending_send(conn, pend);

	conn->tx_cdc_seq++;
	conn->local_tx_ctrl.seqno = conn->tx_cdc_seq;
	smc_host_msg_to_cdc((struct smc_cdc_msg *)wr_buf, conn, &cfed);
<<<<<<< HEAD
	rc = smc_wr_tx_send(link, (struct smc_wr_tx_pend_priv *)pend);
	if (!rc)
		smc_curs_copy(&conn->rx_curs_confirmed, &cfed, conn);
=======

	atomic_inc(&conn->cdc_pend_tx_wr);
	smp_mb__after_atomic(); /* Make sure cdc_pend_tx_wr added before post */

	rc = smc_wr_tx_send(link, (struct smc_wr_tx_pend_priv *)pend);
	if (!rc) {
		smc_curs_copy(&conn->rx_curs_confirmed, &cfed, conn);
		conn->local_rx_ctrl.prod_flags.cons_curs_upd_req = 0;
	} else {
		conn->tx_cdc_seq--;
		conn->local_tx_ctrl.seqno = conn->tx_cdc_seq;
		atomic_dec(&conn->cdc_pend_tx_wr);
	}

	return rc;
}

/* send a validation msg indicating the move of a conn to an other QP link */
int smcr_cdc_msg_send_validation(struct smc_connection *conn,
				 struct smc_cdc_tx_pend *pend,
				 struct smc_wr_buf *wr_buf)
{
	struct smc_host_cdc_msg *local = &conn->local_tx_ctrl;
	struct smc_link *link = conn->lnk;
	struct smc_cdc_msg *peer;
	int rc;

	peer = (struct smc_cdc_msg *)wr_buf;
	peer->common.type = local->common.type;
	peer->len = local->len;
	peer->seqno = htons(conn->tx_cdc_seq_fin); /* seqno last compl. tx */
	peer->token = htonl(local->token);
	peer->prod_flags.failover_validation = 1;

	/* We need to set pend->conn here to make sure smc_cdc_tx_handler()
	 * can handle properly
	 */
	smc_cdc_add_pending_send(conn, pend);

	atomic_inc(&conn->cdc_pend_tx_wr);
	smp_mb__after_atomic(); /* Make sure cdc_pend_tx_wr added before post */

	rc = smc_wr_tx_send(link, (struct smc_wr_tx_pend_priv *)pend);
	if (unlikely(rc))
		atomic_dec(&conn->cdc_pend_tx_wr);
>>>>>>> upstream/android-13

	return rc;
}

static int smcr_cdc_get_slot_and_msg_send(struct smc_connection *conn)
{
	struct smc_cdc_tx_pend *pend;
	struct smc_wr_buf *wr_buf;
<<<<<<< HEAD
	int rc;

	rc = smc_cdc_get_free_slot(conn, &wr_buf, &pend);
	if (rc)
		return rc;

	return smc_cdc_msg_send(conn, wr_buf, pend);
=======
	struct smc_link *link;
	bool again = false;
	int rc;

again:
	link = conn->lnk;
	if (!smc_wr_tx_link_hold(link))
		return -ENOLINK;
	rc = smc_cdc_get_free_slot(conn, link, &wr_buf, NULL, &pend);
	if (rc)
		goto put_out;

	spin_lock_bh(&conn->send_lock);
	if (link != conn->lnk) {
		/* link of connection changed, try again one time*/
		spin_unlock_bh(&conn->send_lock);
		smc_wr_tx_put_slot(link,
				   (struct smc_wr_tx_pend_priv *)pend);
		smc_wr_tx_link_put(link);
		if (again)
			return -ENOLINK;
		again = true;
		goto again;
	}
	rc = smc_cdc_msg_send(conn, wr_buf, pend);
	spin_unlock_bh(&conn->send_lock);
put_out:
	smc_wr_tx_link_put(link);
	return rc;
>>>>>>> upstream/android-13
}

int smc_cdc_get_slot_and_msg_send(struct smc_connection *conn)
{
	int rc;

<<<<<<< HEAD
=======
	if (!conn->lgr || (conn->lgr->is_smcd && conn->lgr->peer_shutdown))
		return -EPIPE;

>>>>>>> upstream/android-13
	if (conn->lgr->is_smcd) {
		spin_lock_bh(&conn->send_lock);
		rc = smcd_cdc_msg_send(conn);
		spin_unlock_bh(&conn->send_lock);
	} else {
		rc = smcr_cdc_get_slot_and_msg_send(conn);
	}

	return rc;
}

<<<<<<< HEAD
static bool smc_cdc_tx_filter(struct smc_wr_tx_pend_priv *tx_pend,
			      unsigned long data)
{
	struct smc_connection *conn = (struct smc_connection *)data;
	struct smc_cdc_tx_pend *cdc_pend =
		(struct smc_cdc_tx_pend *)tx_pend;

	return cdc_pend->conn == conn;
}

static void smc_cdc_tx_dismisser(struct smc_wr_tx_pend_priv *tx_pend)
{
	struct smc_cdc_tx_pend *cdc_pend =
		(struct smc_cdc_tx_pend *)tx_pend;

	cdc_pend->conn = NULL;
}

void smc_cdc_tx_dismiss_slots(struct smc_connection *conn)
{
	struct smc_link *link = &conn->lgr->lnk[SMC_SINGLE_LINK];

	smc_wr_tx_dismiss_slots(link, SMC_CDC_MSG_TYPE,
				smc_cdc_tx_filter, smc_cdc_tx_dismisser,
				(unsigned long)conn);
=======
void smc_cdc_wait_pend_tx_wr(struct smc_connection *conn)
{
	wait_event(conn->cdc_pend_tx_wq, !atomic_read(&conn->cdc_pend_tx_wr));
>>>>>>> upstream/android-13
}

/* Send a SMC-D CDC header.
 * This increments the free space available in our send buffer.
 * Also update the confirmed receive buffer with what was sent to the peer.
 */
int smcd_cdc_msg_send(struct smc_connection *conn)
{
	struct smc_sock *smc = container_of(conn, struct smc_sock, conn);
<<<<<<< HEAD
=======
	union smc_host_cursor curs;
>>>>>>> upstream/android-13
	struct smcd_cdc_msg cdc;
	int rc, diff;

	memset(&cdc, 0, sizeof(cdc));
	cdc.common.type = SMC_CDC_MSG_TYPE;
<<<<<<< HEAD
	cdc.prod_wrap = conn->local_tx_ctrl.prod.wrap;
	cdc.prod_count = conn->local_tx_ctrl.prod.count;

	cdc.cons_wrap = conn->local_tx_ctrl.cons.wrap;
	cdc.cons_count = conn->local_tx_ctrl.cons.count;
	cdc.prod_flags = conn->local_tx_ctrl.prod_flags;
	cdc.conn_state_flags = conn->local_tx_ctrl.conn_state_flags;
	rc = smcd_tx_ism_write(conn, &cdc, sizeof(cdc), 0, 1);
	if (rc)
		return rc;
	smc_curs_copy(&conn->rx_curs_confirmed, &conn->local_tx_ctrl.cons,
		      conn);
=======
	curs.acurs.counter = atomic64_read(&conn->local_tx_ctrl.prod.acurs);
	cdc.prod.wrap = curs.wrap;
	cdc.prod.count = curs.count;
	curs.acurs.counter = atomic64_read(&conn->local_tx_ctrl.cons.acurs);
	cdc.cons.wrap = curs.wrap;
	cdc.cons.count = curs.count;
	cdc.cons.prod_flags = conn->local_tx_ctrl.prod_flags;
	cdc.cons.conn_state_flags = conn->local_tx_ctrl.conn_state_flags;
	rc = smcd_tx_ism_write(conn, &cdc, sizeof(cdc), 0, 1);
	if (rc)
		return rc;
	smc_curs_copy(&conn->rx_curs_confirmed, &curs, conn);
	conn->local_rx_ctrl.prod_flags.cons_curs_upd_req = 0;
>>>>>>> upstream/android-13
	/* Calculate transmitted data and increment free send buffer space */
	diff = smc_curs_diff(conn->sndbuf_desc->len, &conn->tx_curs_fin,
			     &conn->tx_curs_sent);
	/* increased by confirmed number of bytes */
	smp_mb__before_atomic();
	atomic_add(diff, &conn->sndbuf_space);
	/* guarantee 0 <= sndbuf_space <= sndbuf_desc->len */
	smp_mb__after_atomic();
	smc_curs_copy(&conn->tx_curs_fin, &conn->tx_curs_sent, conn);

	smc_tx_sndbuf_nonfull(smc);
	return rc;
}

/********************************* receive ***********************************/

static inline bool smc_cdc_before(u16 seq1, u16 seq2)
{
	return (s16)(seq1 - seq2) < 0;
}

static void smc_cdc_handle_urg_data_arrival(struct smc_sock *smc,
					    int *diff_prod)
{
	struct smc_connection *conn = &smc->conn;
	char *base;

	/* new data included urgent business */
	smc_curs_copy(&conn->urg_curs, &conn->local_rx_ctrl.prod, conn);
	conn->urg_state = SMC_URG_VALID;
	if (!sock_flag(&smc->sk, SOCK_URGINLINE))
		/* we'll skip the urgent byte, so don't account for it */
		(*diff_prod)--;
	base = (char *)conn->rmb_desc->cpu_addr + conn->rx_off;
	if (conn->urg_curs.count)
		conn->urg_rx_byte = *(base + conn->urg_curs.count - 1);
	else
		conn->urg_rx_byte = *(base + conn->rmb_desc->len - 1);
	sk_send_sigurg(&smc->sk);
}

<<<<<<< HEAD
=======
static void smc_cdc_msg_validate(struct smc_sock *smc, struct smc_cdc_msg *cdc,
				 struct smc_link *link)
{
	struct smc_connection *conn = &smc->conn;
	u16 recv_seq = ntohs(cdc->seqno);
	s16 diff;

	/* check that seqnum was seen before */
	diff = conn->local_rx_ctrl.seqno - recv_seq;
	if (diff < 0) { /* diff larger than 0x7fff */
		/* drop connection */
		conn->out_of_sync = 1;	/* prevent any further receives */
		spin_lock_bh(&conn->send_lock);
		conn->local_tx_ctrl.conn_state_flags.peer_conn_abort = 1;
		conn->lnk = link;
		spin_unlock_bh(&conn->send_lock);
		sock_hold(&smc->sk); /* sock_put in abort_work */
		if (!queue_work(smc_close_wq, &conn->abort_work))
			sock_put(&smc->sk);
	}
}

>>>>>>> upstream/android-13
static void smc_cdc_msg_recv_action(struct smc_sock *smc,
				    struct smc_cdc_msg *cdc)
{
	union smc_host_cursor cons_old, prod_old;
	struct smc_connection *conn = &smc->conn;
	int diff_cons, diff_prod;

	smc_curs_copy(&prod_old, &conn->local_rx_ctrl.prod, conn);
	smc_curs_copy(&cons_old, &conn->local_rx_ctrl.cons, conn);
	smc_cdc_msg_to_host(&conn->local_rx_ctrl, cdc, conn);

	diff_cons = smc_curs_diff(conn->peer_rmbe_size, &cons_old,
				  &conn->local_rx_ctrl.cons);
	if (diff_cons) {
		/* peer_rmbe_space is decreased during data transfer with RDMA
		 * write
		 */
		smp_mb__before_atomic();
		atomic_add(diff_cons, &conn->peer_rmbe_space);
		/* guarantee 0 <= peer_rmbe_space <= peer_rmbe_size */
		smp_mb__after_atomic();
	}

	diff_prod = smc_curs_diff(conn->rmb_desc->len, &prod_old,
				  &conn->local_rx_ctrl.prod);
	if (diff_prod) {
		if (conn->local_rx_ctrl.prod_flags.urg_data_present)
			smc_cdc_handle_urg_data_arrival(smc, &diff_prod);
		/* bytes_to_rcv is decreased in smc_recvmsg */
		smp_mb__before_atomic();
		atomic_add(diff_prod, &conn->bytes_to_rcv);
		/* guarantee 0 <= bytes_to_rcv <= rmb_desc->len */
		smp_mb__after_atomic();
		smc->sk.sk_data_ready(&smc->sk);
	} else {
<<<<<<< HEAD
		if (conn->local_rx_ctrl.prod_flags.write_blocked ||
		    conn->local_rx_ctrl.prod_flags.cons_curs_upd_req ||
		    conn->local_rx_ctrl.prod_flags.urg_data_pending) {
			if (conn->local_rx_ctrl.prod_flags.urg_data_pending)
				conn->urg_state = SMC_URG_NOTYET;
			/* force immediate tx of current consumer cursor, but
			 * under send_lock to guarantee arrival in seqno-order
			 */
			if (smc->sk.sk_state != SMC_INIT)
				smc_tx_sndbuf_nonempty(conn);
		}
	}

	/* piggy backed tx info */
	/* trigger sndbuf consumer: RDMA write into peer RMBE and CDC */
	if (diff_cons && smc_tx_prepared_sends(conn)) {
		smc_tx_sndbuf_nonempty(conn);
		/* trigger socket release if connection closed */
		smc_close_wake_tx_prepared(smc);
	}
=======
		if (conn->local_rx_ctrl.prod_flags.write_blocked)
			smc->sk.sk_data_ready(&smc->sk);
		if (conn->local_rx_ctrl.prod_flags.urg_data_pending)
			conn->urg_state = SMC_URG_NOTYET;
	}

	/* trigger sndbuf consumer: RDMA write into peer RMBE and CDC */
	if ((diff_cons && smc_tx_prepared_sends(conn)) ||
	    conn->local_rx_ctrl.prod_flags.cons_curs_upd_req ||
	    conn->local_rx_ctrl.prod_flags.urg_data_pending)
		smc_tx_sndbuf_nonempty(conn);

>>>>>>> upstream/android-13
	if (diff_cons && conn->urg_tx_pend &&
	    atomic_read(&conn->peer_rmbe_space) == conn->peer_rmbe_size) {
		/* urg data confirmed by peer, indicate we're ready for more */
		conn->urg_tx_pend = false;
		smc->sk.sk_write_space(&smc->sk);
	}

	if (conn->local_rx_ctrl.conn_state_flags.peer_conn_abort) {
		smc->sk.sk_err = ECONNRESET;
		conn->local_tx_ctrl.conn_state_flags.peer_conn_abort = 1;
	}
	if (smc_cdc_rxed_any_close_or_senddone(conn)) {
		smc->sk.sk_shutdown |= RCV_SHUTDOWN;
		if (smc->clcsock && smc->clcsock->sk)
			smc->clcsock->sk->sk_shutdown |= RCV_SHUTDOWN;
		sock_set_flag(&smc->sk, SOCK_DONE);
		sock_hold(&smc->sk); /* sock_put in close_work */
<<<<<<< HEAD
		if (!schedule_work(&conn->close_work))
=======
		if (!queue_work(smc_close_wq, &conn->close_work))
>>>>>>> upstream/android-13
			sock_put(&smc->sk);
	}
}

/* called under tasklet context */
static void smc_cdc_msg_recv(struct smc_sock *smc, struct smc_cdc_msg *cdc)
{
	sock_hold(&smc->sk);
	bh_lock_sock(&smc->sk);
	smc_cdc_msg_recv_action(smc, cdc);
	bh_unlock_sock(&smc->sk);
	sock_put(&smc->sk); /* no free sk in softirq-context */
}

/* Schedule a tasklet for this connection. Triggered from the ISM device IRQ
 * handler to indicate update in the DMBE.
 *
 * Context:
 * - tasklet context
 */
<<<<<<< HEAD
static void smcd_cdc_rx_tsklet(unsigned long data)
{
	struct smc_connection *conn = (struct smc_connection *)data;
	struct smcd_cdc_msg cdc;
	struct smc_sock *smc;

	if (!conn)
		return;

	memcpy(&cdc, conn->rmb_desc->cpu_addr, sizeof(cdc));
=======
static void smcd_cdc_rx_tsklet(struct tasklet_struct *t)
{
	struct smc_connection *conn = from_tasklet(conn, t, rx_tsklet);
	struct smcd_cdc_msg *data_cdc;
	struct smcd_cdc_msg cdc;
	struct smc_sock *smc;

	if (!conn || conn->killed)
		return;

	data_cdc = (struct smcd_cdc_msg *)conn->rmb_desc->cpu_addr;
	smcd_curs_copy(&cdc.prod, &data_cdc->prod, conn);
	smcd_curs_copy(&cdc.cons, &data_cdc->cons, conn);
>>>>>>> upstream/android-13
	smc = container_of(conn, struct smc_sock, conn);
	smc_cdc_msg_recv(smc, (struct smc_cdc_msg *)&cdc);
}

/* Initialize receive tasklet. Called from ISM device IRQ handler to start
 * receiver side.
 */
void smcd_cdc_rx_init(struct smc_connection *conn)
{
<<<<<<< HEAD
	tasklet_init(&conn->rx_tsklet, smcd_cdc_rx_tsklet, (unsigned long)conn);
=======
	tasklet_setup(&conn->rx_tsklet, smcd_cdc_rx_tsklet);
>>>>>>> upstream/android-13
}

/***************************** init, exit, misc ******************************/

static void smc_cdc_rx_handler(struct ib_wc *wc, void *buf)
{
	struct smc_link *link = (struct smc_link *)wc->qp->qp_context;
	struct smc_cdc_msg *cdc = buf;
	struct smc_connection *conn;
	struct smc_link_group *lgr;
	struct smc_sock *smc;

	if (wc->byte_len < offsetof(struct smc_cdc_msg, reserved))
		return; /* short message */
	if (cdc->len != SMC_WR_TX_SIZE)
		return; /* invalid message */

	/* lookup connection */
	lgr = smc_get_lgr(link);
	read_lock_bh(&lgr->conns_lock);
	conn = smc_lgr_find_conn(ntohl(cdc->token), lgr);
	read_unlock_bh(&lgr->conns_lock);
<<<<<<< HEAD
	if (!conn)
		return;
	smc = container_of(conn, struct smc_sock, conn);

	if (!cdc->prod_flags.failover_validation) {
		if (smc_cdc_before(ntohs(cdc->seqno),
				   conn->local_rx_ctrl.seqno))
			/* received seqno is old */
			return;
	}
=======
	if (!conn || conn->out_of_sync)
		return;
	smc = container_of(conn, struct smc_sock, conn);

	if (cdc->prod_flags.failover_validation) {
		smc_cdc_msg_validate(smc, cdc, link);
		return;
	}
	if (smc_cdc_before(ntohs(cdc->seqno),
			   conn->local_rx_ctrl.seqno))
		/* received seqno is old */
		return;

>>>>>>> upstream/android-13
	smc_cdc_msg_recv(smc, cdc);
}

static struct smc_wr_rx_handler smc_cdc_rx_handlers[] = {
	{
		.handler	= smc_cdc_rx_handler,
		.type		= SMC_CDC_MSG_TYPE
	},
	{
		.handler	= NULL,
	}
};

int __init smc_cdc_init(void)
{
	struct smc_wr_rx_handler *handler;
	int rc = 0;

	for (handler = smc_cdc_rx_handlers; handler->handler; handler++) {
		INIT_HLIST_NODE(&handler->list);
		rc = smc_wr_rx_register_handler(handler);
		if (rc)
			break;
	}
	return rc;
}
