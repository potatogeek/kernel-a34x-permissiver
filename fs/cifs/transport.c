<<<<<<< HEAD
/*
 *   fs/cifs/transport.c
=======
// SPDX-License-Identifier: LGPL-2.1
/*
>>>>>>> upstream/android-13
 *
 *   Copyright (C) International Business Machines  Corp., 2002,2008
 *   Author(s): Steve French (sfrench@us.ibm.com)
 *   Jeremy Allison (jra@samba.org) 2006.
 *
<<<<<<< HEAD
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation; either version 2.1 of the License, or
 *   (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
>>>>>>> upstream/android-13
 */

#include <linux/fs.h>
#include <linux/list.h>
#include <linux/gfp.h>
#include <linux/wait.h>
#include <linux/net.h>
#include <linux/delay.h>
#include <linux/freezer.h>
#include <linux/tcp.h>
#include <linux/bvec.h>
#include <linux/highmem.h>
#include <linux/uaccess.h>
#include <asm/processor.h>
#include <linux/mempool.h>
<<<<<<< HEAD
=======
#include <linux/sched/signal.h>
>>>>>>> upstream/android-13
#include "cifspdu.h"
#include "cifsglob.h"
#include "cifsproto.h"
#include "cifs_debug.h"
#include "smb2proto.h"
#include "smbdirect.h"

/* Max number of iovectors we can use off the stack when sending requests. */
#define CIFS_MAX_IOV_SIZE 8

void
cifs_wake_up_task(struct mid_q_entry *mid)
{
	wake_up_process(mid->callback_data);
}

struct mid_q_entry *
AllocMidQEntry(const struct smb_hdr *smb_buffer, struct TCP_Server_Info *server)
{
	struct mid_q_entry *temp;

	if (server == NULL) {
		cifs_dbg(VFS, "Null TCP session in AllocMidQEntry\n");
		return NULL;
	}

	temp = mempool_alloc(cifs_mid_poolp, GFP_NOFS);
	memset(temp, 0, sizeof(struct mid_q_entry));
	kref_init(&temp->refcount);
	temp->mid = get_mid(smb_buffer);
	temp->pid = current->pid;
	temp->command = cpu_to_le16(smb_buffer->Command);
	cifs_dbg(FYI, "For smb_command %d\n", smb_buffer->Command);
	/*	do_gettimeofday(&temp->when_sent);*/ /* easier to use jiffies */
	/* when mid allocated can be before when sent */
	temp->when_alloc = jiffies;
	temp->server = server;

	/*
	 * The default is for the mid to be synchronous, so the
	 * default callback just wakes up the current task.
	 */
<<<<<<< HEAD
=======
	get_task_struct(current);
	temp->creator = current;
>>>>>>> upstream/android-13
	temp->callback = cifs_wake_up_task;
	temp->callback_data = current;

	atomic_inc(&midCount);
	temp->mid_state = MID_REQUEST_ALLOCATED;
	return temp;
}

static void _cifs_mid_q_entry_release(struct kref *refcount)
{
<<<<<<< HEAD
	struct mid_q_entry *mid = container_of(refcount, struct mid_q_entry,
					       refcount);

	mempool_free(mid, cifs_mid_poolp);
=======
	struct mid_q_entry *midEntry =
			container_of(refcount, struct mid_q_entry, refcount);
#ifdef CONFIG_CIFS_STATS2
	__le16 command = midEntry->server->vals->lock_cmd;
	__u16 smb_cmd = le16_to_cpu(midEntry->command);
	unsigned long now;
	unsigned long roundtrip_time;
#endif
	struct TCP_Server_Info *server = midEntry->server;

	if (midEntry->resp_buf && (midEntry->mid_flags & MID_WAIT_CANCELLED) &&
	    midEntry->mid_state == MID_RESPONSE_RECEIVED &&
	    server->ops->handle_cancelled_mid)
		server->ops->handle_cancelled_mid(midEntry, server);

	midEntry->mid_state = MID_FREE;
	atomic_dec(&midCount);
	if (midEntry->large_buf)
		cifs_buf_release(midEntry->resp_buf);
	else
		cifs_small_buf_release(midEntry->resp_buf);
#ifdef CONFIG_CIFS_STATS2
	now = jiffies;
	if (now < midEntry->when_alloc)
		cifs_server_dbg(VFS, "Invalid mid allocation time\n");
	roundtrip_time = now - midEntry->when_alloc;

	if (smb_cmd < NUMBER_OF_SMB2_COMMANDS) {
		if (atomic_read(&server->num_cmds[smb_cmd]) == 0) {
			server->slowest_cmd[smb_cmd] = roundtrip_time;
			server->fastest_cmd[smb_cmd] = roundtrip_time;
		} else {
			if (server->slowest_cmd[smb_cmd] < roundtrip_time)
				server->slowest_cmd[smb_cmd] = roundtrip_time;
			else if (server->fastest_cmd[smb_cmd] > roundtrip_time)
				server->fastest_cmd[smb_cmd] = roundtrip_time;
		}
		cifs_stats_inc(&server->num_cmds[smb_cmd]);
		server->time_per_cmd[smb_cmd] += roundtrip_time;
	}
	/*
	 * commands taking longer than one second (default) can be indications
	 * that something is wrong, unless it is quite a slow link or a very
	 * busy server. Note that this calc is unlikely or impossible to wrap
	 * as long as slow_rsp_threshold is not set way above recommended max
	 * value (32767 ie 9 hours) and is generally harmless even if wrong
	 * since only affects debug counters - so leaving the calc as simple
	 * comparison rather than doing multiple conversions and overflow
	 * checks
	 */
	if ((slow_rsp_threshold != 0) &&
	    time_after(now, midEntry->when_alloc + (slow_rsp_threshold * HZ)) &&
	    (midEntry->command != command)) {
		/*
		 * smb2slowcmd[NUMBER_OF_SMB2_COMMANDS] counts by command
		 * NB: le16_to_cpu returns unsigned so can not be negative below
		 */
		if (smb_cmd < NUMBER_OF_SMB2_COMMANDS)
			cifs_stats_inc(&server->smb2slowcmd[smb_cmd]);

		trace_smb3_slow_rsp(smb_cmd, midEntry->mid, midEntry->pid,
			       midEntry->when_sent, midEntry->when_received);
		if (cifsFYI & CIFS_TIMER) {
			pr_debug("slow rsp: cmd %d mid %llu",
				 midEntry->command, midEntry->mid);
			cifs_info("A: 0x%lx S: 0x%lx R: 0x%lx\n",
				  now - midEntry->when_alloc,
				  now - midEntry->when_sent,
				  now - midEntry->when_received);
		}
	}
#endif
	put_task_struct(midEntry->creator);

	mempool_free(midEntry, cifs_mid_poolp);
>>>>>>> upstream/android-13
}

void cifs_mid_q_entry_release(struct mid_q_entry *midEntry)
{
	spin_lock(&GlobalMid_Lock);
	kref_put(&midEntry->refcount, _cifs_mid_q_entry_release);
	spin_unlock(&GlobalMid_Lock);
}

<<<<<<< HEAD
void
DeleteMidQEntry(struct mid_q_entry *midEntry)
{
#ifdef CONFIG_CIFS_STATS2
	__le16 command = midEntry->server->vals->lock_cmd;
	unsigned long now;
#endif
	midEntry->mid_state = MID_FREE;
	atomic_dec(&midCount);
	if (midEntry->large_buf)
		cifs_buf_release(midEntry->resp_buf);
	else
		cifs_small_buf_release(midEntry->resp_buf);
#ifdef CONFIG_CIFS_STATS2
	now = jiffies;
	/* commands taking longer than one second are indications that
	   something is wrong, unless it is quite a slow link or server */
	if (time_after(now, midEntry->when_alloc + HZ) &&
	    (midEntry->command != command)) {
		/* smb2slowcmd[NUMBER_OF_SMB2_COMMANDS] counts by command */
		if ((le16_to_cpu(midEntry->command) < NUMBER_OF_SMB2_COMMANDS) &&
		    (le16_to_cpu(midEntry->command) >= 0))
			cifs_stats_inc(&midEntry->server->smb2slowcmd[le16_to_cpu(midEntry->command)]);

		trace_smb3_slow_rsp(le16_to_cpu(midEntry->command),
			       midEntry->mid, midEntry->pid,
			       midEntry->when_sent, midEntry->when_received);
		if (cifsFYI & CIFS_TIMER) {
			pr_debug(" CIFS slow rsp: cmd %d mid %llu",
			       midEntry->command, midEntry->mid);
			pr_info(" A: 0x%lx S: 0x%lx R: 0x%lx\n",
			       now - midEntry->when_alloc,
			       now - midEntry->when_sent,
			       now - midEntry->when_received);
		}
	}
#endif
=======
void DeleteMidQEntry(struct mid_q_entry *midEntry)
{
>>>>>>> upstream/android-13
	cifs_mid_q_entry_release(midEntry);
}

void
cifs_delete_mid(struct mid_q_entry *mid)
{
	spin_lock(&GlobalMid_Lock);
<<<<<<< HEAD
	list_del_init(&mid->qhead);
	mid->mid_flags |= MID_DELETED;
=======
	if (!(mid->mid_flags & MID_DELETED)) {
		list_del_init(&mid->qhead);
		mid->mid_flags |= MID_DELETED;
	}
>>>>>>> upstream/android-13
	spin_unlock(&GlobalMid_Lock);

	DeleteMidQEntry(mid);
}

/*
 * smb_send_kvec - send an array of kvecs to the server
 * @server:	Server to send the data to
 * @smb_msg:	Message to send
 * @sent:	amount of data sent on socket is stored here
 *
 * Our basic "send data to server" function. Should be called with srv_mutex
 * held. The caller is responsible for handling the results.
 */
static int
smb_send_kvec(struct TCP_Server_Info *server, struct msghdr *smb_msg,
	      size_t *sent)
{
	int rc = 0;
	int retries = 0;
	struct socket *ssocket = server->ssocket;

	*sent = 0;

	smb_msg->msg_name = (struct sockaddr *) &server->dstaddr;
	smb_msg->msg_namelen = sizeof(struct sockaddr);
	smb_msg->msg_control = NULL;
	smb_msg->msg_controllen = 0;
	if (server->noblocksnd)
		smb_msg->msg_flags = MSG_DONTWAIT + MSG_NOSIGNAL;
	else
		smb_msg->msg_flags = MSG_NOSIGNAL;

	while (msg_data_left(smb_msg)) {
		/*
		 * If blocking send, we try 3 times, since each can block
		 * for 5 seconds. For nonblocking  we have to try more
		 * but wait increasing amounts of time allowing time for
		 * socket to clear.  The overall time we wait in either
		 * case to send on the socket is about 15 seconds.
		 * Similarly we wait for 15 seconds for a response from
		 * the server in SendReceive[2] for the server to send
		 * a response back for most types of requests (except
		 * SMB Write past end of file which can be slow, and
		 * blocking lock operations). NFS waits slightly longer
		 * than CIFS, but this can make it take longer for
		 * nonresponsive servers to be detected and 15 seconds
		 * is more than enough time for modern networks to
		 * send a packet.  In most cases if we fail to send
		 * after the retries we will kill the socket and
		 * reconnect which may clear the network problem.
		 */
		rc = sock_sendmsg(ssocket, smb_msg);
		if (rc == -EAGAIN) {
			retries++;
			if (retries >= 14 ||
			    (!server->noblocksnd && (retries > 2))) {
<<<<<<< HEAD
				cifs_dbg(VFS, "sends on sock %p stuck for 15 seconds\n",
=======
				cifs_server_dbg(VFS, "sends on sock %p stuck for 15 seconds\n",
>>>>>>> upstream/android-13
					 ssocket);
				return -EAGAIN;
			}
			msleep(1 << retries);
			continue;
		}

		if (rc < 0)
			return rc;

		if (rc == 0) {
			/* should never happen, letting socket clear before
			   retrying is our only obvious option here */
<<<<<<< HEAD
			cifs_dbg(VFS, "tcp sent no data\n");
=======
			cifs_server_dbg(VFS, "tcp sent no data\n");
>>>>>>> upstream/android-13
			msleep(500);
			continue;
		}

		/* send was at least partially successful */
		*sent += rc;
		retries = 0; /* in case we get ENOSPC on the next send */
	}
	return 0;
}

unsigned long
smb_rqst_len(struct TCP_Server_Info *server, struct smb_rqst *rqst)
{
	unsigned int i;
	struct kvec *iov;
	int nvec;
	unsigned long buflen = 0;

	if (server->vals->header_preamble_size == 0 &&
	    rqst->rq_nvec >= 2 && rqst->rq_iov[0].iov_len == 4) {
		iov = &rqst->rq_iov[1];
		nvec = rqst->rq_nvec - 1;
	} else {
		iov = rqst->rq_iov;
		nvec = rqst->rq_nvec;
	}

	/* total up iov array first */
	for (i = 0; i < nvec; i++)
		buflen += iov[i].iov_len;

	/*
	 * Add in the page array if there is one. The caller needs to make
	 * sure rq_offset and rq_tailsz are set correctly. If a buffer of
	 * multiple pages ends at page boundary, rq_tailsz needs to be set to
	 * PAGE_SIZE.
	 */
	if (rqst->rq_npages) {
		if (rqst->rq_npages == 1)
			buflen += rqst->rq_tailsz;
		else {
			/*
			 * If there is more than one page, calculate the
			 * buffer length based on rq_offset and rq_tailsz
			 */
			buflen += rqst->rq_pagesz * (rqst->rq_npages - 1) -
					rqst->rq_offset;
			buflen += rqst->rq_tailsz;
		}
	}

	return buflen;
}

static int
__smb_send_rqst(struct TCP_Server_Info *server, int num_rqst,
		struct smb_rqst *rqst)
{
	int rc = 0;
	struct kvec *iov;
	int n_vec;
	unsigned int send_length = 0;
	unsigned int i, j;
<<<<<<< HEAD
	size_t total_len = 0, sent, size;
	struct socket *ssocket = server->ssocket;
	struct msghdr smb_msg;
	int val = 1;
=======
	sigset_t mask, oldmask;
	size_t total_len = 0, sent, size;
	struct socket *ssocket = server->ssocket;
	struct msghdr smb_msg;
>>>>>>> upstream/android-13
	__be32 rfc1002_marker;

	if (cifs_rdma_enabled(server)) {
		/* return -EAGAIN when connecting or reconnecting */
		rc = -EAGAIN;
		if (server->smbd_conn)
			rc = smbd_send(server, num_rqst, rqst);
		goto smbd_done;
	}
<<<<<<< HEAD
	if (ssocket == NULL)
		return -ENOTSOCK;

	/* cork the socket */
	kernel_setsockopt(ssocket, SOL_TCP, TCP_CORK,
				(char *)&val, sizeof(val));
=======

	if (ssocket == NULL)
		return -EAGAIN;

	if (fatal_signal_pending(current)) {
		cifs_dbg(FYI, "signal pending before send request\n");
		return -ERESTARTSYS;
	}

	/* cork the socket */
	tcp_sock_set_cork(ssocket->sk, true);
>>>>>>> upstream/android-13

	for (j = 0; j < num_rqst; j++)
		send_length += smb_rqst_len(server, &rqst[j]);
	rfc1002_marker = cpu_to_be32(send_length);

<<<<<<< HEAD
=======
	/*
	 * We should not allow signals to interrupt the network send because
	 * any partial send will cause session reconnects thus increasing
	 * latency of system calls and overload a server with unnecessary
	 * requests.
	 */

	sigfillset(&mask);
	sigprocmask(SIG_BLOCK, &mask, &oldmask);

>>>>>>> upstream/android-13
	/* Generate a rfc1002 marker for SMB2+ */
	if (server->vals->header_preamble_size == 0) {
		struct kvec hiov = {
			.iov_base = &rfc1002_marker,
			.iov_len  = 4
		};
<<<<<<< HEAD
		iov_iter_kvec(&smb_msg.msg_iter, WRITE | ITER_KVEC, &hiov,
			      1, 4);
		rc = smb_send_kvec(server, &smb_msg, &sent);
		if (rc < 0)
			goto uncork;
=======
		iov_iter_kvec(&smb_msg.msg_iter, WRITE, &hiov, 1, 4);
		rc = smb_send_kvec(server, &smb_msg, &sent);
		if (rc < 0)
			goto unmask;
>>>>>>> upstream/android-13

		total_len += sent;
		send_length += 4;
	}

	cifs_dbg(FYI, "Sending smb: smb_len=%u\n", send_length);

	for (j = 0; j < num_rqst; j++) {
		iov = rqst[j].rq_iov;
		n_vec = rqst[j].rq_nvec;

		size = 0;
		for (i = 0; i < n_vec; i++) {
			dump_smb(iov[i].iov_base, iov[i].iov_len);
			size += iov[i].iov_len;
		}

<<<<<<< HEAD
		iov_iter_kvec(&smb_msg.msg_iter, WRITE | ITER_KVEC,
			      iov, n_vec, size);

		rc = smb_send_kvec(server, &smb_msg, &sent);
		if (rc < 0)
			goto uncork;
=======
		iov_iter_kvec(&smb_msg.msg_iter, WRITE, iov, n_vec, size);

		rc = smb_send_kvec(server, &smb_msg, &sent);
		if (rc < 0)
			goto unmask;
>>>>>>> upstream/android-13

		total_len += sent;

		/* now walk the page array and send each page in it */
		for (i = 0; i < rqst[j].rq_npages; i++) {
			struct bio_vec bvec;

			bvec.bv_page = rqst[j].rq_pages[i];
			rqst_page_get_length(&rqst[j], i, &bvec.bv_len,
					     &bvec.bv_offset);

<<<<<<< HEAD
			iov_iter_bvec(&smb_msg.msg_iter, WRITE | ITER_BVEC,
=======
			iov_iter_bvec(&smb_msg.msg_iter, WRITE,
>>>>>>> upstream/android-13
				      &bvec, 1, bvec.bv_len);
			rc = smb_send_kvec(server, &smb_msg, &sent);
			if (rc < 0)
				break;

			total_len += sent;
		}
	}

<<<<<<< HEAD
uncork:
	/* uncork it */
	val = 0;
	kernel_setsockopt(ssocket, SOL_TCP, TCP_CORK,
				(char *)&val, sizeof(val));
=======
unmask:
	sigprocmask(SIG_SETMASK, &oldmask, NULL);

	/*
	 * If signal is pending but we have already sent the whole packet to
	 * the server we need to return success status to allow a corresponding
	 * mid entry to be kept in the pending requests queue thus allowing
	 * to handle responses from the server by the client.
	 *
	 * If only part of the packet has been sent there is no need to hide
	 * interrupt because the session will be reconnected anyway, so there
	 * won't be any response from the server to handle.
	 */

	if (signal_pending(current) && (total_len != send_length)) {
		cifs_dbg(FYI, "signal is pending after attempt to send\n");
		rc = -ERESTARTSYS;
	}

	/* uncork it */
	tcp_sock_set_cork(ssocket->sk, false);
>>>>>>> upstream/android-13

	if ((total_len > 0) && (total_len != send_length)) {
		cifs_dbg(FYI, "partial send (wanted=%u sent=%zu): terminating session\n",
			 send_length, total_len);
		/*
		 * If we have only sent part of an SMB then the next SMB could
		 * be taken as the remainder of this one. We need to kill the
		 * socket so the server throws away the partial SMB
		 */
<<<<<<< HEAD
		server->tcpStatus = CifsNeedReconnect;
		trace_smb3_partial_send_reconnect(server->CurrentMid,
						  server->hostname);
	}
smbd_done:
	if (rc < 0 && rc != -EINTR)
		cifs_dbg(VFS, "Error %d sending data on socket to server\n",
=======
		spin_lock(&GlobalMid_Lock);
		server->tcpStatus = CifsNeedReconnect;
		spin_unlock(&GlobalMid_Lock);
		trace_smb3_partial_send_reconnect(server->CurrentMid,
						  server->conn_id, server->hostname);
	}
smbd_done:
	if (rc < 0 && rc != -EINTR)
		cifs_server_dbg(VFS, "Error %d sending data on socket to server\n",
>>>>>>> upstream/android-13
			 rc);
	else if (rc > 0)
		rc = 0;

	return rc;
}

static int
smb_send_rqst(struct TCP_Server_Info *server, int num_rqst,
	      struct smb_rqst *rqst, int flags)
{
	struct kvec iov;
	struct smb2_transform_hdr *tr_hdr;
	struct smb_rqst cur_rqst[MAX_COMPOUND];
	int rc;

	if (!(flags & CIFS_TRANSFORM_REQ))
		return __smb_send_rqst(server, num_rqst, rqst);

	if (num_rqst > MAX_COMPOUND - 1)
		return -ENOMEM;

	if (!server->ops->init_transform_rq) {
<<<<<<< HEAD
		cifs_dbg(VFS, "Encryption requested but transform callback "
			 "is missing\n");
=======
		cifs_server_dbg(VFS, "Encryption requested but transform callback is missing\n");
>>>>>>> upstream/android-13
		return -EIO;
	}

	tr_hdr = kmalloc(sizeof(*tr_hdr), GFP_NOFS);
	if (!tr_hdr)
		return -ENOMEM;

	memset(&cur_rqst[0], 0, sizeof(cur_rqst));
	memset(&iov, 0, sizeof(iov));
	memset(tr_hdr, 0, sizeof(*tr_hdr));

	iov.iov_base = tr_hdr;
	iov.iov_len = sizeof(*tr_hdr);
	cur_rqst[0].rq_iov = &iov;
	cur_rqst[0].rq_nvec = 1;

	rc = server->ops->init_transform_rq(server, num_rqst + 1,
					    &cur_rqst[0], rqst);
	if (rc)
		goto out;

	rc = __smb_send_rqst(server, num_rqst + 1, &cur_rqst[0]);
	smb3_free_compound_rqst(num_rqst, &cur_rqst[1]);
out:
	kfree(tr_hdr);
	return rc;
}

int
smb_send(struct TCP_Server_Info *server, struct smb_hdr *smb_buffer,
	 unsigned int smb_buf_length)
{
	struct kvec iov[2];
	struct smb_rqst rqst = { .rq_iov = iov,
				 .rq_nvec = 2 };

	iov[0].iov_base = smb_buffer;
	iov[0].iov_len = 4;
	iov[1].iov_base = (char *)smb_buffer + 4;
	iov[1].iov_len = smb_buf_length;

	return __smb_send_rqst(server, 1, &rqst);
}

static int
<<<<<<< HEAD
wait_for_free_credits(struct TCP_Server_Info *server, const int timeout,
		      int *credits)
{
	int rc;

	spin_lock(&server->req_lock);
	if (timeout == CIFS_ASYNC_OP) {
		/* oplock breaks must not be held up */
		server->in_flight++;
		*credits -= 1;
		spin_unlock(&server->req_lock);
=======
wait_for_free_credits(struct TCP_Server_Info *server, const int num_credits,
		      const int timeout, const int flags,
		      unsigned int *instance)
{
	long rc;
	int *credits;
	int optype;
	long int t;
	int scredits, in_flight;

	if (timeout < 0)
		t = MAX_JIFFY_OFFSET;
	else
		t = msecs_to_jiffies(timeout);

	optype = flags & CIFS_OP_MASK;

	*instance = 0;

	credits = server->ops->get_credits_field(server, optype);
	/* Since an echo is already inflight, no need to wait to send another */
	if (*credits <= 0 && optype == CIFS_ECHO_OP)
		return -EAGAIN;

	spin_lock(&server->req_lock);
	if ((flags & CIFS_TIMEOUT_MASK) == CIFS_NON_BLOCKING) {
		/* oplock breaks must not be held up */
		server->in_flight++;
		if (server->in_flight > server->max_in_flight)
			server->max_in_flight = server->in_flight;
		*credits -= 1;
		*instance = server->reconnect_instance;
		scredits = *credits;
		in_flight = server->in_flight;
		spin_unlock(&server->req_lock);

		trace_smb3_add_credits(server->CurrentMid,
				server->conn_id, server->hostname, scredits, -1, in_flight);
		cifs_dbg(FYI, "%s: remove %u credits total=%d\n",
				__func__, 1, scredits);

>>>>>>> upstream/android-13
		return 0;
	}

	while (1) {
<<<<<<< HEAD
		if (*credits <= 0) {
			spin_unlock(&server->req_lock);
			cifs_num_waiters_inc(server);
			rc = wait_event_killable(server->request_q,
						 has_credits(server, credits));
			cifs_num_waiters_dec(server);
			if (rc)
				return rc;
=======
		if (*credits < num_credits) {
			scredits = *credits;
			spin_unlock(&server->req_lock);

			cifs_num_waiters_inc(server);
			rc = wait_event_killable_timeout(server->request_q,
				has_credits(server, credits, num_credits), t);
			cifs_num_waiters_dec(server);
			if (!rc) {
				spin_lock(&server->req_lock);
				scredits = *credits;
				in_flight = server->in_flight;
				spin_unlock(&server->req_lock);

				trace_smb3_credit_timeout(server->CurrentMid,
						server->conn_id, server->hostname, scredits,
						num_credits, in_flight);
				cifs_server_dbg(VFS, "wait timed out after %d ms\n",
						timeout);
				return -EBUSY;
			}
			if (rc == -ERESTARTSYS)
				return -ERESTARTSYS;
>>>>>>> upstream/android-13
			spin_lock(&server->req_lock);
		} else {
			if (server->tcpStatus == CifsExiting) {
				spin_unlock(&server->req_lock);
				return -ENOENT;
			}

			/*
<<<<<<< HEAD
=======
			 * For normal commands, reserve the last MAX_COMPOUND
			 * credits to compound requests.
			 * Otherwise these compounds could be permanently
			 * starved for credits by single-credit requests.
			 *
			 * To prevent spinning CPU, block this thread until
			 * there are >MAX_COMPOUND credits available.
			 * But only do this is we already have a lot of
			 * credits in flight to avoid triggering this check
			 * for servers that are slow to hand out credits on
			 * new sessions.
			 */
			if (!optype && num_credits == 1 &&
			    server->in_flight > 2 * MAX_COMPOUND &&
			    *credits <= MAX_COMPOUND) {
				spin_unlock(&server->req_lock);

				cifs_num_waiters_inc(server);
				rc = wait_event_killable_timeout(
					server->request_q,
					has_credits(server, credits,
						    MAX_COMPOUND + 1),
					t);
				cifs_num_waiters_dec(server);
				if (!rc) {
					spin_lock(&server->req_lock);
					scredits = *credits;
					in_flight = server->in_flight;
					spin_unlock(&server->req_lock);

					trace_smb3_credit_timeout(
							server->CurrentMid,
							server->conn_id, server->hostname,
							scredits, num_credits, in_flight);
					cifs_server_dbg(VFS, "wait timed out after %d ms\n",
							timeout);
					return -EBUSY;
				}
				if (rc == -ERESTARTSYS)
					return -ERESTARTSYS;
				spin_lock(&server->req_lock);
				continue;
			}

			/*
>>>>>>> upstream/android-13
			 * Can not count locking commands against total
			 * as they are allowed to block on server.
			 */

			/* update # of requests on the wire to server */
<<<<<<< HEAD
			if (timeout != CIFS_BLOCKING_OP) {
				*credits -= 1;
				server->in_flight++;
			}
			spin_unlock(&server->req_lock);
=======
			if ((flags & CIFS_TIMEOUT_MASK) != CIFS_BLOCKING_OP) {
				*credits -= num_credits;
				server->in_flight += num_credits;
				if (server->in_flight > server->max_in_flight)
					server->max_in_flight = server->in_flight;
				*instance = server->reconnect_instance;
			}
			scredits = *credits;
			in_flight = server->in_flight;
			spin_unlock(&server->req_lock);

			trace_smb3_add_credits(server->CurrentMid,
					server->conn_id, server->hostname, scredits,
					-(num_credits), in_flight);
			cifs_dbg(FYI, "%s: remove %u credits total=%d\n",
					__func__, num_credits, scredits);
>>>>>>> upstream/android-13
			break;
		}
	}
	return 0;
}

static int
<<<<<<< HEAD
wait_for_free_request(struct TCP_Server_Info *server, const int timeout,
		      const int optype)
{
	int *val;

	val = server->ops->get_credits_field(server, optype);
	/* Since an echo is already inflight, no need to wait to send another */
	if (*val <= 0 && optype == CIFS_ECHO_OP)
		return -EAGAIN;
	return wait_for_free_credits(server, timeout, val);
=======
wait_for_free_request(struct TCP_Server_Info *server, const int flags,
		      unsigned int *instance)
{
	return wait_for_free_credits(server, 1, -1, flags,
				     instance);
}

static int
wait_for_compound_request(struct TCP_Server_Info *server, int num,
			  const int flags, unsigned int *instance)
{
	int *credits;
	int scredits, in_flight;

	credits = server->ops->get_credits_field(server, flags & CIFS_OP_MASK);

	spin_lock(&server->req_lock);
	scredits = *credits;
	in_flight = server->in_flight;

	if (*credits < num) {
		/*
		 * If the server is tight on resources or just gives us less
		 * credits for other reasons (e.g. requests are coming out of
		 * order and the server delays granting more credits until it
		 * processes a missing mid) and we exhausted most available
		 * credits there may be situations when we try to send
		 * a compound request but we don't have enough credits. At this
		 * point the client needs to decide if it should wait for
		 * additional credits or fail the request. If at least one
		 * request is in flight there is a high probability that the
		 * server will return enough credits to satisfy this compound
		 * request.
		 *
		 * Return immediately if no requests in flight since we will be
		 * stuck on waiting for credits.
		 */
		if (server->in_flight == 0) {
			spin_unlock(&server->req_lock);
			trace_smb3_insufficient_credits(server->CurrentMid,
					server->conn_id, server->hostname, scredits,
					num, in_flight);
			cifs_dbg(FYI, "%s: %d requests in flight, needed %d total=%d\n",
					__func__, in_flight, num, scredits);
			return -EDEADLK;
		}
	}
	spin_unlock(&server->req_lock);

	return wait_for_free_credits(server, num, 60000, flags,
				     instance);
>>>>>>> upstream/android-13
}

int
cifs_wait_mtu_credits(struct TCP_Server_Info *server, unsigned int size,
<<<<<<< HEAD
		      unsigned int *num, unsigned int *credits)
{
	*num = size;
	*credits = 0;
=======
		      unsigned int *num, struct cifs_credits *credits)
{
	*num = size;
	credits->value = 0;
	credits->instance = server->reconnect_instance;
>>>>>>> upstream/android-13
	return 0;
}

static int allocate_mid(struct cifs_ses *ses, struct smb_hdr *in_buf,
			struct mid_q_entry **ppmidQ)
{
	if (ses->server->tcpStatus == CifsExiting) {
		return -ENOENT;
	}

	if (ses->server->tcpStatus == CifsNeedReconnect) {
		cifs_dbg(FYI, "tcp session dead - return to caller to retry\n");
		return -EAGAIN;
	}

	if (ses->status == CifsNew) {
		if ((in_buf->Command != SMB_COM_SESSION_SETUP_ANDX) &&
			(in_buf->Command != SMB_COM_NEGOTIATE))
			return -EAGAIN;
		/* else ok - we are setting up session */
	}

	if (ses->status == CifsExiting) {
		/* check if SMB session is bad because we are setting it up */
		if (in_buf->Command != SMB_COM_LOGOFF_ANDX)
			return -EAGAIN;
		/* else ok - we are shutting down session */
	}

	*ppmidQ = AllocMidQEntry(in_buf, ses->server);
	if (*ppmidQ == NULL)
		return -ENOMEM;
	spin_lock(&GlobalMid_Lock);
	list_add_tail(&(*ppmidQ)->qhead, &ses->server->pending_mid_q);
	spin_unlock(&GlobalMid_Lock);
	return 0;
}

static int
wait_for_response(struct TCP_Server_Info *server, struct mid_q_entry *midQ)
{
	int error;

	error = wait_event_freezekillable_unsafe(server->response_q,
				    midQ->mid_state != MID_REQUEST_SUBMITTED);
	if (error < 0)
		return -ERESTARTSYS;

	return 0;
}

struct mid_q_entry *
cifs_setup_async_request(struct TCP_Server_Info *server, struct smb_rqst *rqst)
{
	int rc;
	struct smb_hdr *hdr = (struct smb_hdr *)rqst->rq_iov[0].iov_base;
	struct mid_q_entry *mid;

	if (rqst->rq_iov[0].iov_len != 4 ||
	    rqst->rq_iov[0].iov_base + 4 != rqst->rq_iov[1].iov_base)
		return ERR_PTR(-EIO);

	/* enable signing if server requires it */
	if (server->sign)
		hdr->Flags2 |= SMBFLG2_SECURITY_SIGNATURE;

	mid = AllocMidQEntry(hdr, server);
	if (mid == NULL)
		return ERR_PTR(-ENOMEM);

	rc = cifs_sign_rqst(rqst, server, &mid->sequence_number);
	if (rc) {
		DeleteMidQEntry(mid);
		return ERR_PTR(rc);
	}

	return mid;
}

/*
 * Send a SMB request and set the callback function in the mid to handle
 * the result. Caller is responsible for dealing with timeouts.
 */
int
cifs_call_async(struct TCP_Server_Info *server, struct smb_rqst *rqst,
		mid_receive_t *receive, mid_callback_t *callback,
<<<<<<< HEAD
		mid_handle_t *handle, void *cbdata, const int flags)
{
	int rc, timeout, optype;
	struct mid_q_entry *mid;
	unsigned int credits = 0;

	timeout = flags & CIFS_TIMEOUT_MASK;
	optype = flags & CIFS_OP_MASK;

	if ((flags & CIFS_HAS_CREDITS) == 0) {
		rc = wait_for_free_request(server, timeout, optype);
		if (rc)
			return rc;
		credits = 1;
	}

	mutex_lock(&server->srv_mutex);
	mid = server->ops->setup_async_request(server, rqst);
	if (IS_ERR(mid)) {
		mutex_unlock(&server->srv_mutex);
		add_credits_and_wake_if(server, credits, optype);
=======
		mid_handle_t *handle, void *cbdata, const int flags,
		const struct cifs_credits *exist_credits)
{
	int rc;
	struct mid_q_entry *mid;
	struct cifs_credits credits = { .value = 0, .instance = 0 };
	unsigned int instance;
	int optype;

	optype = flags & CIFS_OP_MASK;

	if ((flags & CIFS_HAS_CREDITS) == 0) {
		rc = wait_for_free_request(server, flags, &instance);
		if (rc)
			return rc;
		credits.value = 1;
		credits.instance = instance;
	} else
		instance = exist_credits->instance;

	mutex_lock(&server->srv_mutex);

	/*
	 * We can't use credits obtained from the previous session to send this
	 * request. Check if there were reconnects after we obtained credits and
	 * return -EAGAIN in such cases to let callers handle it.
	 */
	if (instance != server->reconnect_instance) {
		mutex_unlock(&server->srv_mutex);
		add_credits_and_wake_if(server, &credits, optype);
		return -EAGAIN;
	}

	mid = server->ops->setup_async_request(server, rqst);
	if (IS_ERR(mid)) {
		mutex_unlock(&server->srv_mutex);
		add_credits_and_wake_if(server, &credits, optype);
>>>>>>> upstream/android-13
		return PTR_ERR(mid);
	}

	mid->receive = receive;
	mid->callback = callback;
	mid->callback_data = cbdata;
	mid->handle = handle;
	mid->mid_state = MID_REQUEST_SUBMITTED;

	/* put it on the pending_mid_q */
	spin_lock(&GlobalMid_Lock);
	list_add_tail(&mid->qhead, &server->pending_mid_q);
	spin_unlock(&GlobalMid_Lock);

	/*
	 * Need to store the time in mid before calling I/O. For call_async,
	 * I/O response may come back and free the mid entry on another thread.
	 */
	cifs_save_when_sent(mid);
	cifs_in_send_inc(server);
	rc = smb_send_rqst(server, 1, rqst, flags);
	cifs_in_send_dec(server);

	if (rc < 0) {
		revert_current_mid(server, mid->credits);
		server->sequence_number -= 2;
		cifs_delete_mid(mid);
	}

	mutex_unlock(&server->srv_mutex);

	if (rc == 0)
		return 0;

<<<<<<< HEAD
	add_credits_and_wake_if(server, credits, optype);
=======
	add_credits_and_wake_if(server, &credits, optype);
>>>>>>> upstream/android-13
	return rc;
}

/*
 *
 * Send an SMB Request.  No response info (other than return code)
 * needs to be parsed.
 *
 * flags indicate the type of request buffer and how long to wait
 * and whether to log NT STATUS code (error) before mapping it to POSIX error
 *
 */
int
SendReceiveNoRsp(const unsigned int xid, struct cifs_ses *ses,
		 char *in_buf, int flags)
{
	int rc;
	struct kvec iov[1];
	struct kvec rsp_iov;
	int resp_buf_type;

	iov[0].iov_base = in_buf;
	iov[0].iov_len = get_rfc1002_length(in_buf) + 4;
<<<<<<< HEAD
	flags |= CIFS_NO_RESP;
=======
	flags |= CIFS_NO_RSP_BUF;
>>>>>>> upstream/android-13
	rc = SendReceive2(xid, ses, iov, 1, &resp_buf_type, flags, &rsp_iov);
	cifs_dbg(NOISY, "SendRcvNoRsp flags %d rc %d\n", flags, rc);

	return rc;
}

static int
cifs_sync_mid_result(struct mid_q_entry *mid, struct TCP_Server_Info *server)
{
	int rc = 0;

	cifs_dbg(FYI, "%s: cmd=%d mid=%llu state=%d\n",
		 __func__, le16_to_cpu(mid->command), mid->mid, mid->mid_state);

	spin_lock(&GlobalMid_Lock);
	switch (mid->mid_state) {
	case MID_RESPONSE_RECEIVED:
		spin_unlock(&GlobalMid_Lock);
		return rc;
	case MID_RETRY_NEEDED:
		rc = -EAGAIN;
		break;
	case MID_RESPONSE_MALFORMED:
		rc = -EIO;
		break;
	case MID_SHUTDOWN:
		rc = -EHOSTDOWN;
		break;
	default:
<<<<<<< HEAD
		list_del_init(&mid->qhead);
		cifs_dbg(VFS, "%s: invalid mid state mid=%llu state=%d\n",
=======
		if (!(mid->mid_flags & MID_DELETED)) {
			list_del_init(&mid->qhead);
			mid->mid_flags |= MID_DELETED;
		}
		cifs_server_dbg(VFS, "%s: invalid mid state mid=%llu state=%d\n",
>>>>>>> upstream/android-13
			 __func__, mid->mid, mid->mid_state);
		rc = -EIO;
	}
	spin_unlock(&GlobalMid_Lock);

	DeleteMidQEntry(mid);
	return rc;
}

static inline int
send_cancel(struct TCP_Server_Info *server, struct smb_rqst *rqst,
	    struct mid_q_entry *mid)
{
	return server->ops->send_cancel ?
				server->ops->send_cancel(server, rqst, mid) : 0;
}

int
cifs_check_receive(struct mid_q_entry *mid, struct TCP_Server_Info *server,
		   bool log_error)
{
	unsigned int len = get_rfc1002_length(mid->resp_buf) + 4;

	dump_smb(mid->resp_buf, min_t(u32, 92, len));

	/* convert the length into a more usable form */
	if (server->sign) {
		struct kvec iov[2];
		int rc = 0;
		struct smb_rqst rqst = { .rq_iov = iov,
					 .rq_nvec = 2 };

		iov[0].iov_base = mid->resp_buf;
		iov[0].iov_len = 4;
		iov[1].iov_base = (char *)mid->resp_buf + 4;
		iov[1].iov_len = len - 4;
		/* FIXME: add code to kill session */
		rc = cifs_verify_signature(&rqst, server,
					   mid->sequence_number);
		if (rc)
<<<<<<< HEAD
			cifs_dbg(VFS, "SMB signature verification returned error = %d\n",
=======
			cifs_server_dbg(VFS, "SMB signature verification returned error = %d\n",
>>>>>>> upstream/android-13
				 rc);
	}

	/* BB special case reconnect tid and uid here? */
<<<<<<< HEAD
	return map_smb_to_linux_error(mid->resp_buf, log_error);
}

struct mid_q_entry *
cifs_setup_request(struct cifs_ses *ses, struct smb_rqst *rqst)
=======
	return map_and_check_smb_error(mid, log_error);
}

struct mid_q_entry *
cifs_setup_request(struct cifs_ses *ses, struct TCP_Server_Info *ignored,
		   struct smb_rqst *rqst)
>>>>>>> upstream/android-13
{
	int rc;
	struct smb_hdr *hdr = (struct smb_hdr *)rqst->rq_iov[0].iov_base;
	struct mid_q_entry *mid;

	if (rqst->rq_iov[0].iov_len != 4 ||
	    rqst->rq_iov[0].iov_base + 4 != rqst->rq_iov[1].iov_base)
		return ERR_PTR(-EIO);

	rc = allocate_mid(ses, hdr, &mid);
	if (rc)
		return ERR_PTR(rc);
	rc = cifs_sign_rqst(rqst, ses->server, &mid->sequence_number);
	if (rc) {
		cifs_delete_mid(mid);
		return ERR_PTR(rc);
	}
	return mid;
}

static void
<<<<<<< HEAD
cifs_noop_callback(struct mid_q_entry *mid)
{
=======
cifs_compound_callback(struct mid_q_entry *mid)
{
	struct TCP_Server_Info *server = mid->server;
	struct cifs_credits credits;

	credits.value = server->ops->get_credits(mid);
	credits.instance = server->reconnect_instance;

	add_credits(server, &credits, mid->optype);
}

static void
cifs_compound_last_callback(struct mid_q_entry *mid)
{
	cifs_compound_callback(mid);
	cifs_wake_up_task(mid);
}

static void
cifs_cancelled_callback(struct mid_q_entry *mid)
{
	cifs_compound_callback(mid);
	DeleteMidQEntry(mid);
}

/*
 * Return a channel (master if none) of @ses that can be used to send
 * regular requests.
 *
 * If we are currently binding a new channel (negprot/sess.setup),
 * return the new incomplete channel.
 */
struct TCP_Server_Info *cifs_pick_channel(struct cifs_ses *ses)
{
	uint index = 0;

	if (!ses)
		return NULL;

	spin_lock(&ses->chan_lock);
	if (!ses->binding) {
		/* round robin */
		if (ses->chan_count > 1) {
			index = (uint)atomic_inc_return(&ses->chan_seq);
			index %= ses->chan_count;
		}
		spin_unlock(&ses->chan_lock);
		return ses->chans[index].server;
	} else {
		spin_unlock(&ses->chan_lock);
		return cifs_ses_server(ses);
	}
>>>>>>> upstream/android-13
}

int
compound_send_recv(const unsigned int xid, struct cifs_ses *ses,
<<<<<<< HEAD
		   const int flags, const int num_rqst, struct smb_rqst *rqst,
		   int *resp_buf_type, struct kvec *resp_iov)
{
	int i, j, rc = 0;
	int timeout, optype;
	struct mid_q_entry *midQ[MAX_COMPOUND];
	bool cancelled_mid[MAX_COMPOUND] = {false};
	unsigned int credits[MAX_COMPOUND] = {0};
	char *buf;

	timeout = flags & CIFS_TIMEOUT_MASK;
=======
		   struct TCP_Server_Info *server,
		   const int flags, const int num_rqst, struct smb_rqst *rqst,
		   int *resp_buf_type, struct kvec *resp_iov)
{
	int i, j, optype, rc = 0;
	struct mid_q_entry *midQ[MAX_COMPOUND];
	bool cancelled_mid[MAX_COMPOUND] = {false};
	struct cifs_credits credits[MAX_COMPOUND] = {
		{ .value = 0, .instance = 0 }
	};
	unsigned int instance;
	char *buf;

>>>>>>> upstream/android-13
	optype = flags & CIFS_OP_MASK;

	for (i = 0; i < num_rqst; i++)
		resp_buf_type[i] = CIFS_NO_BUFFER;  /* no response buf yet */

<<<<<<< HEAD
	if ((ses == NULL) || (ses->server == NULL)) {
=======
	if (!ses || !ses->server || !server) {
>>>>>>> upstream/android-13
		cifs_dbg(VFS, "Null session\n");
		return -EIO;
	}

<<<<<<< HEAD
	if (ses->server->tcpStatus == CifsExiting)
		return -ENOENT;

	/*
	 * Ensure we obtain 1 credit per request in the compound chain.
	 * It can be optimized further by waiting for all the credits
	 * at once but this can wait long enough if we don't have enough
	 * credits due to some heavy operations in progress or the server
	 * not granting us much, so a fallback to the current approach is
	 * needed anyway.
	 */
	for (i = 0; i < num_rqst; i++) {
		rc = wait_for_free_request(ses->server, timeout, optype);
		if (rc) {
			/*
			 * We haven't sent an SMB packet to the server yet but
			 * we already obtained credits for i requests in the
			 * compound chain - need to return those credits back
			 * for future use. Note that we need to call add_credits
			 * multiple times to match the way we obtained credits
			 * in the first place and to account for in flight
			 * requests correctly.
			 */
			for (j = 0; j < i; j++)
				add_credits(ses->server, 1, optype);
			return rc;
		}
		credits[i] = 1;
=======
	if (server->tcpStatus == CifsExiting)
		return -ENOENT;

	/*
	 * Wait for all the requests to become available.
	 * This approach still leaves the possibility to be stuck waiting for
	 * credits if the server doesn't grant credits to the outstanding
	 * requests and if the client is completely idle, not generating any
	 * other requests.
	 * This can be handled by the eventual session reconnect.
	 */
	rc = wait_for_compound_request(server, num_rqst, flags,
				       &instance);
	if (rc)
		return rc;

	for (i = 0; i < num_rqst; i++) {
		credits[i].value = 1;
		credits[i].instance = instance;
>>>>>>> upstream/android-13
	}

	/*
	 * Make sure that we sign in the same order that we send on this socket
	 * and avoid races inside tcp sendmsg code that could cause corruption
	 * of smb data.
	 */

<<<<<<< HEAD
	mutex_lock(&ses->server->srv_mutex);

	for (i = 0; i < num_rqst; i++) {
		midQ[i] = ses->server->ops->setup_request(ses, &rqst[i]);
		if (IS_ERR(midQ[i])) {
			revert_current_mid(ses->server, i);
			for (j = 0; j < i; j++)
				cifs_delete_mid(midQ[j]);
			mutex_unlock(&ses->server->srv_mutex);

			/* Update # of requests on wire to server */
			for (j = 0; j < num_rqst; j++)
				add_credits(ses->server, credits[j], optype);
=======
	mutex_lock(&server->srv_mutex);

	/*
	 * All the parts of the compound chain belong obtained credits from the
	 * same session. We can not use credits obtained from the previous
	 * session to send this request. Check if there were reconnects after
	 * we obtained credits and return -EAGAIN in such cases to let callers
	 * handle it.
	 */
	if (instance != server->reconnect_instance) {
		mutex_unlock(&server->srv_mutex);
		for (j = 0; j < num_rqst; j++)
			add_credits(server, &credits[j], optype);
		return -EAGAIN;
	}

	for (i = 0; i < num_rqst; i++) {
		midQ[i] = server->ops->setup_request(ses, server, &rqst[i]);
		if (IS_ERR(midQ[i])) {
			revert_current_mid(server, i);
			for (j = 0; j < i; j++)
				cifs_delete_mid(midQ[j]);
			mutex_unlock(&server->srv_mutex);

			/* Update # of requests on wire to server */
			for (j = 0; j < num_rqst; j++)
				add_credits(server, &credits[j], optype);
>>>>>>> upstream/android-13
			return PTR_ERR(midQ[i]);
		}

		midQ[i]->mid_state = MID_REQUEST_SUBMITTED;
<<<<<<< HEAD
		/*
		 * We don't invoke the callback compounds unless it is the last
		 * request.
		 */
		if (i < num_rqst - 1)
			midQ[i]->callback = cifs_noop_callback;
	}
	cifs_in_send_inc(ses->server);
	rc = smb_send_rqst(ses->server, num_rqst, rqst, flags);
	cifs_in_send_dec(ses->server);
=======
		midQ[i]->optype = optype;
		/*
		 * Invoke callback for every part of the compound chain
		 * to calculate credits properly. Wake up this thread only when
		 * the last element is received.
		 */
		if (i < num_rqst - 1)
			midQ[i]->callback = cifs_compound_callback;
		else
			midQ[i]->callback = cifs_compound_last_callback;
	}
	cifs_in_send_inc(server);
	rc = smb_send_rqst(server, num_rqst, rqst, flags);
	cifs_in_send_dec(server);
>>>>>>> upstream/android-13

	for (i = 0; i < num_rqst; i++)
		cifs_save_when_sent(midQ[i]);

	if (rc < 0) {
<<<<<<< HEAD
		revert_current_mid(ses->server, num_rqst);
		ses->server->sequence_number -= 2;
	}

	mutex_unlock(&ses->server->srv_mutex);

	if (rc < 0)
		goto out;
=======
		revert_current_mid(server, num_rqst);
		server->sequence_number -= 2;
	}

	mutex_unlock(&server->srv_mutex);

	/*
	 * If sending failed for some reason or it is an oplock break that we
	 * will not receive a response to - return credits back
	 */
	if (rc < 0 || (flags & CIFS_NO_SRV_RSP)) {
		for (i = 0; i < num_rqst; i++)
			add_credits(server, &credits[i], optype);
		goto out;
	}

	/*
	 * At this point the request is passed to the network stack - we assume
	 * that any credits taken from the server structure on the client have
	 * been spent and we can't return them back. Once we receive responses
	 * we will collect credits granted by the server in the mid callbacks
	 * and add those credits to the server structure.
	 */
>>>>>>> upstream/android-13

	/*
	 * Compounding is never used during session establish.
	 */
<<<<<<< HEAD
	if ((ses->status == CifsNew) || (optype & CIFS_NEG_OP)) {
		mutex_lock(&ses->server->srv_mutex);
		smb311_update_preauth_hash(ses, rqst[0].rq_iov,
					   rqst[0].rq_nvec);
		mutex_unlock(&ses->server->srv_mutex);
	}

	if (timeout == CIFS_ASYNC_OP)
		goto out;

	for (i = 0; i < num_rqst; i++) {
		rc = wait_for_response(ses->server, midQ[i]);
		if (rc != 0) {
			cifs_dbg(FYI, "Cancelling wait for mid %llu\n",
				 midQ[i]->mid);
			send_cancel(ses->server, &rqst[i], midQ[i]);
			spin_lock(&GlobalMid_Lock);
			if (midQ[i]->mid_state == MID_REQUEST_SUBMITTED) {
				midQ[i]->mid_flags |= MID_WAIT_CANCELLED;
				midQ[i]->callback = DeleteMidQEntry;
				cancelled_mid[i] = true;
=======
	if ((ses->status == CifsNew) || (optype & CIFS_NEG_OP) || (optype & CIFS_SESS_OP)) {
		mutex_lock(&server->srv_mutex);
		smb311_update_preauth_hash(ses, rqst[0].rq_iov,
					   rqst[0].rq_nvec);
		mutex_unlock(&server->srv_mutex);
	}

	for (i = 0; i < num_rqst; i++) {
		rc = wait_for_response(server, midQ[i]);
		if (rc != 0)
			break;
	}
	if (rc != 0) {
		for (; i < num_rqst; i++) {
			cifs_server_dbg(FYI, "Cancelling wait for mid %llu cmd: %d\n",
				 midQ[i]->mid, le16_to_cpu(midQ[i]->command));
			send_cancel(server, &rqst[i], midQ[i]);
			spin_lock(&GlobalMid_Lock);
			midQ[i]->mid_flags |= MID_WAIT_CANCELLED;
			if (midQ[i]->mid_state == MID_REQUEST_SUBMITTED) {
				midQ[i]->callback = cifs_cancelled_callback;
				cancelled_mid[i] = true;
				credits[i].value = 0;
>>>>>>> upstream/android-13
			}
			spin_unlock(&GlobalMid_Lock);
		}
	}

<<<<<<< HEAD
	for (i = 0; i < num_rqst; i++)
		if (!cancelled_mid[i] && midQ[i]->resp_buf
		    && (midQ[i]->mid_state == MID_RESPONSE_RECEIVED))
			credits[i] = ses->server->ops->get_credits(midQ[i]);

=======
>>>>>>> upstream/android-13
	for (i = 0; i < num_rqst; i++) {
		if (rc < 0)
			goto out;

<<<<<<< HEAD
		rc = cifs_sync_mid_result(midQ[i], ses->server);
=======
		rc = cifs_sync_mid_result(midQ[i], server);
>>>>>>> upstream/android-13
		if (rc != 0) {
			/* mark this mid as cancelled to not free it below */
			cancelled_mid[i] = true;
			goto out;
		}

		if (!midQ[i]->resp_buf ||
		    midQ[i]->mid_state != MID_RESPONSE_RECEIVED) {
			rc = -EIO;
			cifs_dbg(FYI, "Bad MID state?\n");
			goto out;
		}

		buf = (char *)midQ[i]->resp_buf;
		resp_iov[i].iov_base = buf;
		resp_iov[i].iov_len = midQ[i]->resp_buf_size +
<<<<<<< HEAD
			ses->server->vals->header_preamble_size;
=======
			server->vals->header_preamble_size;
>>>>>>> upstream/android-13

		if (midQ[i]->large_buf)
			resp_buf_type[i] = CIFS_LARGE_BUFFER;
		else
			resp_buf_type[i] = CIFS_SMALL_BUFFER;

<<<<<<< HEAD
		rc = ses->server->ops->check_receive(midQ[i], ses->server,
						     flags & CIFS_LOG_ERROR);

		/* mark it so buf will not be freed by cifs_delete_mid */
		if ((flags & CIFS_NO_RESP) == 0)
=======
		rc = server->ops->check_receive(midQ[i], server,
						     flags & CIFS_LOG_ERROR);

		/* mark it so buf will not be freed by cifs_delete_mid */
		if ((flags & CIFS_NO_RSP_BUF) == 0)
>>>>>>> upstream/android-13
			midQ[i]->resp_buf = NULL;

	}

	/*
	 * Compounding is never used during session establish.
	 */
<<<<<<< HEAD
	if ((ses->status == CifsNew) || (optype & CIFS_NEG_OP)) {
=======
	if ((ses->status == CifsNew) || (optype & CIFS_NEG_OP) || (optype & CIFS_SESS_OP)) {
>>>>>>> upstream/android-13
		struct kvec iov = {
			.iov_base = resp_iov[0].iov_base,
			.iov_len = resp_iov[0].iov_len
		};
<<<<<<< HEAD
		mutex_lock(&ses->server->srv_mutex);
		smb311_update_preauth_hash(ses, &iov, 1);
		mutex_unlock(&ses->server->srv_mutex);
=======
		mutex_lock(&server->srv_mutex);
		smb311_update_preauth_hash(ses, &iov, 1);
		mutex_unlock(&server->srv_mutex);
>>>>>>> upstream/android-13
	}

out:
	/*
	 * This will dequeue all mids. After this it is important that the
	 * demultiplex_thread will not process any of these mids any futher.
	 * This is prevented above by using a noop callback that will not
	 * wake this thread except for the very last PDU.
	 */
	for (i = 0; i < num_rqst; i++) {
		if (!cancelled_mid[i])
			cifs_delete_mid(midQ[i]);
<<<<<<< HEAD
		add_credits(ses->server, credits[i], optype);
=======
>>>>>>> upstream/android-13
	}

	return rc;
}

int
cifs_send_recv(const unsigned int xid, struct cifs_ses *ses,
<<<<<<< HEAD
	       struct smb_rqst *rqst, int *resp_buf_type, const int flags,
	       struct kvec *resp_iov)
{
	return compound_send_recv(xid, ses, flags, 1, rqst, resp_buf_type,
				  resp_iov);
=======
	       struct TCP_Server_Info *server,
	       struct smb_rqst *rqst, int *resp_buf_type, const int flags,
	       struct kvec *resp_iov)
{
	return compound_send_recv(xid, ses, server, flags, 1,
				  rqst, resp_buf_type, resp_iov);
>>>>>>> upstream/android-13
}

int
SendReceive2(const unsigned int xid, struct cifs_ses *ses,
	     struct kvec *iov, int n_vec, int *resp_buf_type /* ret */,
	     const int flags, struct kvec *resp_iov)
{
	struct smb_rqst rqst;
	struct kvec s_iov[CIFS_MAX_IOV_SIZE], *new_iov;
	int rc;

	if (n_vec + 1 > CIFS_MAX_IOV_SIZE) {
		new_iov = kmalloc_array(n_vec + 1, sizeof(struct kvec),
					GFP_KERNEL);
		if (!new_iov) {
			/* otherwise cifs_send_recv below sets resp_buf_type */
			*resp_buf_type = CIFS_NO_BUFFER;
			return -ENOMEM;
		}
	} else
		new_iov = s_iov;

	/* 1st iov is a RFC1001 length followed by the rest of the packet */
	memcpy(new_iov + 1, iov, (sizeof(struct kvec) * n_vec));

	new_iov[0].iov_base = new_iov[1].iov_base;
	new_iov[0].iov_len = 4;
	new_iov[1].iov_base += 4;
	new_iov[1].iov_len -= 4;

	memset(&rqst, 0, sizeof(struct smb_rqst));
	rqst.rq_iov = new_iov;
	rqst.rq_nvec = n_vec + 1;

<<<<<<< HEAD
	rc = cifs_send_recv(xid, ses, &rqst, resp_buf_type, flags, resp_iov);
=======
	rc = cifs_send_recv(xid, ses, ses->server,
			    &rqst, resp_buf_type, flags, resp_iov);
>>>>>>> upstream/android-13
	if (n_vec + 1 > CIFS_MAX_IOV_SIZE)
		kfree(new_iov);
	return rc;
}

int
SendReceive(const unsigned int xid, struct cifs_ses *ses,
	    struct smb_hdr *in_buf, struct smb_hdr *out_buf,
<<<<<<< HEAD
	    int *pbytes_returned, const int timeout)
=======
	    int *pbytes_returned, const int flags)
>>>>>>> upstream/android-13
{
	int rc = 0;
	struct mid_q_entry *midQ;
	unsigned int len = be32_to_cpu(in_buf->smb_buf_length);
	struct kvec iov = { .iov_base = in_buf, .iov_len = len };
	struct smb_rqst rqst = { .rq_iov = &iov, .rq_nvec = 1 };
<<<<<<< HEAD
=======
	struct cifs_credits credits = { .value = 1, .instance = 0 };
	struct TCP_Server_Info *server;
>>>>>>> upstream/android-13

	if (ses == NULL) {
		cifs_dbg(VFS, "Null smb session\n");
		return -EIO;
	}
<<<<<<< HEAD
	if (ses->server == NULL) {
=======
	server = ses->server;
	if (server == NULL) {
>>>>>>> upstream/android-13
		cifs_dbg(VFS, "Null tcp session\n");
		return -EIO;
	}

<<<<<<< HEAD
	if (ses->server->tcpStatus == CifsExiting)
=======
	if (server->tcpStatus == CifsExiting)
>>>>>>> upstream/android-13
		return -ENOENT;

	/* Ensure that we do not send more than 50 overlapping requests
	   to the same server. We may make this configurable later or
	   use ses->maxReq */

	if (len > CIFSMaxBufSize + MAX_CIFS_HDR_SIZE - 4) {
<<<<<<< HEAD
		cifs_dbg(VFS, "Illegal length, greater than maximum frame, %d\n",
			 len);
		return -EIO;
	}

	rc = wait_for_free_request(ses->server, timeout, 0);
=======
		cifs_server_dbg(VFS, "Invalid length, greater than maximum frame, %d\n",
				len);
		return -EIO;
	}

	rc = wait_for_free_request(server, flags, &credits.instance);
>>>>>>> upstream/android-13
	if (rc)
		return rc;

	/* make sure that we sign in the same order that we send on this socket
	   and avoid races inside tcp sendmsg code that could cause corruption
	   of smb data */

<<<<<<< HEAD
	mutex_lock(&ses->server->srv_mutex);

	rc = allocate_mid(ses, in_buf, &midQ);
	if (rc) {
		mutex_unlock(&ses->server->srv_mutex);
		/* Update # of requests on wire to server */
		add_credits(ses->server, 1, 0);
		return rc;
	}

	rc = cifs_sign_smb(in_buf, ses->server, &midQ->sequence_number);
	if (rc) {
		mutex_unlock(&ses->server->srv_mutex);
=======
	mutex_lock(&server->srv_mutex);

	rc = allocate_mid(ses, in_buf, &midQ);
	if (rc) {
		mutex_unlock(&server->srv_mutex);
		/* Update # of requests on wire to server */
		add_credits(server, &credits, 0);
		return rc;
	}

	rc = cifs_sign_smb(in_buf, server, &midQ->sequence_number);
	if (rc) {
		mutex_unlock(&server->srv_mutex);
>>>>>>> upstream/android-13
		goto out;
	}

	midQ->mid_state = MID_REQUEST_SUBMITTED;

<<<<<<< HEAD
	cifs_in_send_inc(ses->server);
	rc = smb_send(ses->server, in_buf, len);
	cifs_in_send_dec(ses->server);
	cifs_save_when_sent(midQ);

	if (rc < 0)
		ses->server->sequence_number -= 2;

	mutex_unlock(&ses->server->srv_mutex);
=======
	cifs_in_send_inc(server);
	rc = smb_send(server, in_buf, len);
	cifs_in_send_dec(server);
	cifs_save_when_sent(midQ);

	if (rc < 0)
		server->sequence_number -= 2;

	mutex_unlock(&server->srv_mutex);
>>>>>>> upstream/android-13

	if (rc < 0)
		goto out;

<<<<<<< HEAD
	if (timeout == CIFS_ASYNC_OP)
		goto out;

	rc = wait_for_response(ses->server, midQ);
	if (rc != 0) {
		send_cancel(ses->server, &rqst, midQ);
=======
	rc = wait_for_response(server, midQ);
	if (rc != 0) {
		send_cancel(server, &rqst, midQ);
>>>>>>> upstream/android-13
		spin_lock(&GlobalMid_Lock);
		if (midQ->mid_state == MID_REQUEST_SUBMITTED) {
			/* no longer considered to be "in-flight" */
			midQ->callback = DeleteMidQEntry;
			spin_unlock(&GlobalMid_Lock);
<<<<<<< HEAD
			add_credits(ses->server, 1, 0);
=======
			add_credits(server, &credits, 0);
>>>>>>> upstream/android-13
			return rc;
		}
		spin_unlock(&GlobalMid_Lock);
	}

<<<<<<< HEAD
	rc = cifs_sync_mid_result(midQ, ses->server);
	if (rc != 0) {
		add_credits(ses->server, 1, 0);
=======
	rc = cifs_sync_mid_result(midQ, server);
	if (rc != 0) {
		add_credits(server, &credits, 0);
>>>>>>> upstream/android-13
		return rc;
	}

	if (!midQ->resp_buf || !out_buf ||
	    midQ->mid_state != MID_RESPONSE_RECEIVED) {
		rc = -EIO;
<<<<<<< HEAD
		cifs_dbg(VFS, "Bad MID state?\n");
=======
		cifs_server_dbg(VFS, "Bad MID state?\n");
>>>>>>> upstream/android-13
		goto out;
	}

	*pbytes_returned = get_rfc1002_length(midQ->resp_buf);
	memcpy(out_buf, midQ->resp_buf, *pbytes_returned + 4);
<<<<<<< HEAD
	rc = cifs_check_receive(midQ, ses->server, 0);
out:
	cifs_delete_mid(midQ);
	add_credits(ses->server, 1, 0);
=======
	rc = cifs_check_receive(midQ, server, 0);
out:
	cifs_delete_mid(midQ);
	add_credits(server, &credits, 0);
>>>>>>> upstream/android-13

	return rc;
}

/* We send a LOCKINGX_CANCEL_LOCK to cause the Windows
   blocking lock to return. */

static int
send_lock_cancel(const unsigned int xid, struct cifs_tcon *tcon,
			struct smb_hdr *in_buf,
			struct smb_hdr *out_buf)
{
	int bytes_returned;
	struct cifs_ses *ses = tcon->ses;
	LOCK_REQ *pSMB = (LOCK_REQ *)in_buf;

	/* We just modify the current in_buf to change
	   the type of lock from LOCKING_ANDX_SHARED_LOCK
	   or LOCKING_ANDX_EXCLUSIVE_LOCK to
	   LOCKING_ANDX_CANCEL_LOCK. */

	pSMB->LockType = LOCKING_ANDX_CANCEL_LOCK|LOCKING_ANDX_LARGE_FILES;
	pSMB->Timeout = 0;
	pSMB->hdr.Mid = get_next_mid(ses->server);

	return SendReceive(xid, ses, in_buf, out_buf,
			&bytes_returned, 0);
}

int
SendReceiveBlockingLock(const unsigned int xid, struct cifs_tcon *tcon,
	    struct smb_hdr *in_buf, struct smb_hdr *out_buf,
	    int *pbytes_returned)
{
	int rc = 0;
	int rstart = 0;
	struct mid_q_entry *midQ;
	struct cifs_ses *ses;
	unsigned int len = be32_to_cpu(in_buf->smb_buf_length);
	struct kvec iov = { .iov_base = in_buf, .iov_len = len };
	struct smb_rqst rqst = { .rq_iov = &iov, .rq_nvec = 1 };
<<<<<<< HEAD
=======
	unsigned int instance;
	struct TCP_Server_Info *server;
>>>>>>> upstream/android-13

	if (tcon == NULL || tcon->ses == NULL) {
		cifs_dbg(VFS, "Null smb session\n");
		return -EIO;
	}
	ses = tcon->ses;
<<<<<<< HEAD

	if (ses->server == NULL) {
=======
	server = ses->server;

	if (server == NULL) {
>>>>>>> upstream/android-13
		cifs_dbg(VFS, "Null tcp session\n");
		return -EIO;
	}

<<<<<<< HEAD
	if (ses->server->tcpStatus == CifsExiting)
=======
	if (server->tcpStatus == CifsExiting)
>>>>>>> upstream/android-13
		return -ENOENT;

	/* Ensure that we do not send more than 50 overlapping requests
	   to the same server. We may make this configurable later or
	   use ses->maxReq */

	if (len > CIFSMaxBufSize + MAX_CIFS_HDR_SIZE - 4) {
<<<<<<< HEAD
		cifs_dbg(VFS, "Illegal length, greater than maximum frame, %d\n",
			 len);
		return -EIO;
	}

	rc = wait_for_free_request(ses->server, CIFS_BLOCKING_OP, 0);
=======
		cifs_tcon_dbg(VFS, "Invalid length, greater than maximum frame, %d\n",
			      len);
		return -EIO;
	}

	rc = wait_for_free_request(server, CIFS_BLOCKING_OP, &instance);
>>>>>>> upstream/android-13
	if (rc)
		return rc;

	/* make sure that we sign in the same order that we send on this socket
	   and avoid races inside tcp sendmsg code that could cause corruption
	   of smb data */

<<<<<<< HEAD
	mutex_lock(&ses->server->srv_mutex);

	rc = allocate_mid(ses, in_buf, &midQ);
	if (rc) {
		mutex_unlock(&ses->server->srv_mutex);
		return rc;
	}

	rc = cifs_sign_smb(in_buf, ses->server, &midQ->sequence_number);
	if (rc) {
		cifs_delete_mid(midQ);
		mutex_unlock(&ses->server->srv_mutex);
=======
	mutex_lock(&server->srv_mutex);

	rc = allocate_mid(ses, in_buf, &midQ);
	if (rc) {
		mutex_unlock(&server->srv_mutex);
		return rc;
	}

	rc = cifs_sign_smb(in_buf, server, &midQ->sequence_number);
	if (rc) {
		cifs_delete_mid(midQ);
		mutex_unlock(&server->srv_mutex);
>>>>>>> upstream/android-13
		return rc;
	}

	midQ->mid_state = MID_REQUEST_SUBMITTED;
<<<<<<< HEAD
	cifs_in_send_inc(ses->server);
	rc = smb_send(ses->server, in_buf, len);
	cifs_in_send_dec(ses->server);
	cifs_save_when_sent(midQ);

	if (rc < 0)
		ses->server->sequence_number -= 2;

	mutex_unlock(&ses->server->srv_mutex);
=======
	cifs_in_send_inc(server);
	rc = smb_send(server, in_buf, len);
	cifs_in_send_dec(server);
	cifs_save_when_sent(midQ);

	if (rc < 0)
		server->sequence_number -= 2;

	mutex_unlock(&server->srv_mutex);
>>>>>>> upstream/android-13

	if (rc < 0) {
		cifs_delete_mid(midQ);
		return rc;
	}

	/* Wait for a reply - allow signals to interrupt. */
<<<<<<< HEAD
	rc = wait_event_interruptible(ses->server->response_q,
		(!(midQ->mid_state == MID_REQUEST_SUBMITTED)) ||
		((ses->server->tcpStatus != CifsGood) &&
		 (ses->server->tcpStatus != CifsNew)));
=======
	rc = wait_event_interruptible(server->response_q,
		(!(midQ->mid_state == MID_REQUEST_SUBMITTED)) ||
		((server->tcpStatus != CifsGood) &&
		 (server->tcpStatus != CifsNew)));
>>>>>>> upstream/android-13

	/* Were we interrupted by a signal ? */
	if ((rc == -ERESTARTSYS) &&
		(midQ->mid_state == MID_REQUEST_SUBMITTED) &&
<<<<<<< HEAD
		((ses->server->tcpStatus == CifsGood) ||
		 (ses->server->tcpStatus == CifsNew))) {
=======
		((server->tcpStatus == CifsGood) ||
		 (server->tcpStatus == CifsNew))) {
>>>>>>> upstream/android-13

		if (in_buf->Command == SMB_COM_TRANSACTION2) {
			/* POSIX lock. We send a NT_CANCEL SMB to cause the
			   blocking lock to return. */
<<<<<<< HEAD
			rc = send_cancel(ses->server, &rqst, midQ);
=======
			rc = send_cancel(server, &rqst, midQ);
>>>>>>> upstream/android-13
			if (rc) {
				cifs_delete_mid(midQ);
				return rc;
			}
		} else {
			/* Windows lock. We send a LOCKINGX_CANCEL_LOCK
			   to cause the blocking lock to return. */

			rc = send_lock_cancel(xid, tcon, in_buf, out_buf);

			/* If we get -ENOLCK back the lock may have
			   already been removed. Don't exit in this case. */
			if (rc && rc != -ENOLCK) {
				cifs_delete_mid(midQ);
				return rc;
			}
		}

<<<<<<< HEAD
		rc = wait_for_response(ses->server, midQ);
		if (rc) {
			send_cancel(ses->server, &rqst, midQ);
=======
		rc = wait_for_response(server, midQ);
		if (rc) {
			send_cancel(server, &rqst, midQ);
>>>>>>> upstream/android-13
			spin_lock(&GlobalMid_Lock);
			if (midQ->mid_state == MID_REQUEST_SUBMITTED) {
				/* no longer considered to be "in-flight" */
				midQ->callback = DeleteMidQEntry;
				spin_unlock(&GlobalMid_Lock);
				return rc;
			}
			spin_unlock(&GlobalMid_Lock);
		}

		/* We got the response - restart system call. */
		rstart = 1;
	}

<<<<<<< HEAD
	rc = cifs_sync_mid_result(midQ, ses->server);
=======
	rc = cifs_sync_mid_result(midQ, server);
>>>>>>> upstream/android-13
	if (rc != 0)
		return rc;

	/* rcvd frame is ok */
	if (out_buf == NULL || midQ->mid_state != MID_RESPONSE_RECEIVED) {
		rc = -EIO;
<<<<<<< HEAD
		cifs_dbg(VFS, "Bad MID state?\n");
=======
		cifs_tcon_dbg(VFS, "Bad MID state?\n");
>>>>>>> upstream/android-13
		goto out;
	}

	*pbytes_returned = get_rfc1002_length(midQ->resp_buf);
	memcpy(out_buf, midQ->resp_buf, *pbytes_returned + 4);
<<<<<<< HEAD
	rc = cifs_check_receive(midQ, ses->server, 0);
=======
	rc = cifs_check_receive(midQ, server, 0);
>>>>>>> upstream/android-13
out:
	cifs_delete_mid(midQ);
	if (rstart && rc == -EACCES)
		return -ERESTARTSYS;
	return rc;
}
