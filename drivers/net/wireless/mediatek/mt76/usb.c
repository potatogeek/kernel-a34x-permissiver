<<<<<<< HEAD
/*
 * Copyright (C) 2018 Lorenzo Bianconi <lorenzo.bianconi83@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

=======
// SPDX-License-Identifier: ISC
/*
 * Copyright (C) 2018 Lorenzo Bianconi <lorenzo.bianconi83@gmail.com>
 */

#include <linux/module.h>
>>>>>>> upstream/android-13
#include "mt76.h"
#include "usb_trace.h"
#include "dma.h"

#define MT_VEND_REQ_MAX_RETRY	10
#define MT_VEND_REQ_TOUT_MS	300

<<<<<<< HEAD
/* should be called with usb_ctrl_mtx locked */
=======
static bool disable_usb_sg;
module_param_named(disable_usb_sg, disable_usb_sg, bool, 0644);
MODULE_PARM_DESC(disable_usb_sg, "Disable usb scatter-gather support");

>>>>>>> upstream/android-13
static int __mt76u_vendor_request(struct mt76_dev *dev, u8 req,
				  u8 req_type, u16 val, u16 offset,
				  void *buf, size_t len)
{
<<<<<<< HEAD
	struct usb_interface *intf = to_usb_interface(dev->dev);
	struct usb_device *udev = interface_to_usbdev(intf);
	unsigned int pipe;
	int i, ret;

	pipe = (req_type & USB_DIR_IN) ? usb_rcvctrlpipe(udev, 0)
				       : usb_sndctrlpipe(udev, 0);
	for (i = 0; i < MT_VEND_REQ_MAX_RETRY; i++) {
		if (test_bit(MT76_REMOVED, &dev->state))
=======
	struct usb_interface *uintf = to_usb_interface(dev->dev);
	struct usb_device *udev = interface_to_usbdev(uintf);
	unsigned int pipe;
	int i, ret;

	lockdep_assert_held(&dev->usb.usb_ctrl_mtx);

	pipe = (req_type & USB_DIR_IN) ? usb_rcvctrlpipe(udev, 0)
				       : usb_sndctrlpipe(udev, 0);
	for (i = 0; i < MT_VEND_REQ_MAX_RETRY; i++) {
		if (test_bit(MT76_REMOVED, &dev->phy.state))
>>>>>>> upstream/android-13
			return -EIO;

		ret = usb_control_msg(udev, pipe, req, req_type, val,
				      offset, buf, len, MT_VEND_REQ_TOUT_MS);
		if (ret == -ENODEV)
<<<<<<< HEAD
			set_bit(MT76_REMOVED, &dev->state);
=======
			set_bit(MT76_REMOVED, &dev->phy.state);
>>>>>>> upstream/android-13
		if (ret >= 0 || ret == -ENODEV)
			return ret;
		usleep_range(5000, 10000);
	}

	dev_err(dev->dev, "vendor request req:%02x off:%04x failed:%d\n",
		req, offset, ret);
	return ret;
}

int mt76u_vendor_request(struct mt76_dev *dev, u8 req,
			 u8 req_type, u16 val, u16 offset,
			 void *buf, size_t len)
{
	int ret;

	mutex_lock(&dev->usb.usb_ctrl_mtx);
	ret = __mt76u_vendor_request(dev, req, req_type,
				     val, offset, buf, len);
	trace_usb_reg_wr(dev, offset, val);
	mutex_unlock(&dev->usb.usb_ctrl_mtx);

	return ret;
}
EXPORT_SYMBOL_GPL(mt76u_vendor_request);

<<<<<<< HEAD
/* should be called with usb_ctrl_mtx locked */
static u32 __mt76u_rr(struct mt76_dev *dev, u32 addr)
{
	struct mt76_usb *usb = &dev->usb;
	u32 data = ~0;
	u16 offset;
	int ret;
=======
static u32 ___mt76u_rr(struct mt76_dev *dev, u8 req, u32 addr)
{
	struct mt76_usb *usb = &dev->usb;
	u32 data = ~0;
	int ret;

	ret = __mt76u_vendor_request(dev, req,
				     USB_DIR_IN | USB_TYPE_VENDOR,
				     addr >> 16, addr, usb->data,
				     sizeof(__le32));
	if (ret == sizeof(__le32))
		data = get_unaligned_le32(usb->data);
	trace_usb_reg_rr(dev, addr, data);

	return data;
}

static u32 __mt76u_rr(struct mt76_dev *dev, u32 addr)
{
>>>>>>> upstream/android-13
	u8 req;

	switch (addr & MT_VEND_TYPE_MASK) {
	case MT_VEND_TYPE_EEPROM:
		req = MT_VEND_READ_EEPROM;
		break;
	case MT_VEND_TYPE_CFG:
		req = MT_VEND_READ_CFG;
		break;
	default:
		req = MT_VEND_MULTI_READ;
		break;
	}
<<<<<<< HEAD
	offset = addr & ~MT_VEND_TYPE_MASK;

	ret = __mt76u_vendor_request(dev, req,
				     USB_DIR_IN | USB_TYPE_VENDOR,
				     0, offset, usb->data, sizeof(__le32));
	if (ret == sizeof(__le32))
		data = get_unaligned_le32(usb->data);
	trace_usb_reg_rr(dev, addr, data);

	return data;
}

u32 mt76u_rr(struct mt76_dev *dev, u32 addr)
=======

	return ___mt76u_rr(dev, req, addr & ~MT_VEND_TYPE_MASK);
}

static u32 mt76u_rr(struct mt76_dev *dev, u32 addr)
>>>>>>> upstream/android-13
{
	u32 ret;

	mutex_lock(&dev->usb.usb_ctrl_mtx);
	ret = __mt76u_rr(dev, addr);
	mutex_unlock(&dev->usb.usb_ctrl_mtx);

	return ret;
}

<<<<<<< HEAD
/* should be called with usb_ctrl_mtx locked */
static void __mt76u_wr(struct mt76_dev *dev, u32 addr, u32 val)
{
	struct mt76_usb *usb = &dev->usb;
	u16 offset;
=======
static u32 mt76u_rr_ext(struct mt76_dev *dev, u32 addr)
{
	u32 ret;

	mutex_lock(&dev->usb.usb_ctrl_mtx);
	ret = ___mt76u_rr(dev, MT_VEND_READ_EXT, addr);
	mutex_unlock(&dev->usb.usb_ctrl_mtx);

	return ret;
}

static void ___mt76u_wr(struct mt76_dev *dev, u8 req,
			u32 addr, u32 val)
{
	struct mt76_usb *usb = &dev->usb;

	put_unaligned_le32(val, usb->data);
	__mt76u_vendor_request(dev, req,
			       USB_DIR_OUT | USB_TYPE_VENDOR,
			       addr >> 16, addr, usb->data,
			       sizeof(__le32));
	trace_usb_reg_wr(dev, addr, val);
}

static void __mt76u_wr(struct mt76_dev *dev, u32 addr, u32 val)
{
>>>>>>> upstream/android-13
	u8 req;

	switch (addr & MT_VEND_TYPE_MASK) {
	case MT_VEND_TYPE_CFG:
		req = MT_VEND_WRITE_CFG;
		break;
	default:
		req = MT_VEND_MULTI_WRITE;
		break;
	}
<<<<<<< HEAD
	offset = addr & ~MT_VEND_TYPE_MASK;

	put_unaligned_le32(val, usb->data);
	__mt76u_vendor_request(dev, req,
			       USB_DIR_OUT | USB_TYPE_VENDOR, 0,
			       offset, usb->data, sizeof(__le32));
	trace_usb_reg_wr(dev, addr, val);
}

void mt76u_wr(struct mt76_dev *dev, u32 addr, u32 val)
=======
	___mt76u_wr(dev, req, addr & ~MT_VEND_TYPE_MASK, val);
}

static void mt76u_wr(struct mt76_dev *dev, u32 addr, u32 val)
>>>>>>> upstream/android-13
{
	mutex_lock(&dev->usb.usb_ctrl_mtx);
	__mt76u_wr(dev, addr, val);
	mutex_unlock(&dev->usb.usb_ctrl_mtx);
}

<<<<<<< HEAD
=======
static void mt76u_wr_ext(struct mt76_dev *dev, u32 addr, u32 val)
{
	mutex_lock(&dev->usb.usb_ctrl_mtx);
	___mt76u_wr(dev, MT_VEND_WRITE_EXT, addr, val);
	mutex_unlock(&dev->usb.usb_ctrl_mtx);
}

>>>>>>> upstream/android-13
static u32 mt76u_rmw(struct mt76_dev *dev, u32 addr,
		     u32 mask, u32 val)
{
	mutex_lock(&dev->usb.usb_ctrl_mtx);
	val |= __mt76u_rr(dev, addr) & ~mask;
	__mt76u_wr(dev, addr, val);
	mutex_unlock(&dev->usb.usb_ctrl_mtx);

	return val;
}

<<<<<<< HEAD
=======
static u32 mt76u_rmw_ext(struct mt76_dev *dev, u32 addr,
			 u32 mask, u32 val)
{
	mutex_lock(&dev->usb.usb_ctrl_mtx);
	val |= ___mt76u_rr(dev, MT_VEND_READ_EXT, addr) & ~mask;
	___mt76u_wr(dev, MT_VEND_WRITE_EXT, addr, val);
	mutex_unlock(&dev->usb.usb_ctrl_mtx);

	return val;
}

>>>>>>> upstream/android-13
static void mt76u_copy(struct mt76_dev *dev, u32 offset,
		       const void *data, int len)
{
	struct mt76_usb *usb = &dev->usb;
<<<<<<< HEAD
	const u32 *val = data;
	int i, ret;

	mutex_lock(&usb->usb_ctrl_mtx);
	for (i = 0; i < (len / 4); i++) {
		put_unaligned_le32(val[i], usb->data);
		ret = __mt76u_vendor_request(dev, MT_VEND_MULTI_WRITE,
					     USB_DIR_OUT | USB_TYPE_VENDOR,
					     0, offset + i * 4, usb->data,
					     sizeof(__le32));
		if (ret < 0)
			break;
=======
	const u8 *val = data;
	int ret;
	int current_batch_size;
	int i = 0;

	/* Assure that always a multiple of 4 bytes are copied,
	 * otherwise beacons can be corrupted.
	 * See: "mt76: round up length on mt76_wr_copy"
	 * Commit 850e8f6fbd5d0003b0
	 */
	len = round_up(len, 4);

	mutex_lock(&usb->usb_ctrl_mtx);
	while (i < len) {
		current_batch_size = min_t(int, usb->data_len, len - i);
		memcpy(usb->data, val + i, current_batch_size);
		ret = __mt76u_vendor_request(dev, MT_VEND_MULTI_WRITE,
					     USB_DIR_OUT | USB_TYPE_VENDOR,
					     0, offset + i, usb->data,
					     current_batch_size);
		if (ret < 0)
			break;

		i += current_batch_size;
	}
	mutex_unlock(&usb->usb_ctrl_mtx);
}

static void mt76u_copy_ext(struct mt76_dev *dev, u32 offset,
			   const void *data, int len)
{
	struct mt76_usb *usb = &dev->usb;
	int ret, i = 0, batch_len;
	const u8 *val = data;

	len = round_up(len, 4);
	mutex_lock(&usb->usb_ctrl_mtx);
	while (i < len) {
		batch_len = min_t(int, usb->data_len, len - i);
		memcpy(usb->data, val + i, batch_len);
		ret = __mt76u_vendor_request(dev, MT_VEND_WRITE_EXT,
					     USB_DIR_OUT | USB_TYPE_VENDOR,
					     (offset + i) >> 16, offset + i,
					     usb->data, batch_len);
		if (ret < 0)
			break;

		i += batch_len;
	}
	mutex_unlock(&usb->usb_ctrl_mtx);
}

static void
mt76u_read_copy_ext(struct mt76_dev *dev, u32 offset,
		    void *data, int len)
{
	struct mt76_usb *usb = &dev->usb;
	int i = 0, batch_len, ret;
	u8 *val = data;

	len = round_up(len, 4);
	mutex_lock(&usb->usb_ctrl_mtx);
	while (i < len) {
		batch_len = min_t(int, usb->data_len, len - i);
		ret = __mt76u_vendor_request(dev, MT_VEND_READ_EXT,
					     USB_DIR_IN | USB_TYPE_VENDOR,
					     (offset + i) >> 16, offset + i,
					     usb->data, batch_len);
		if (ret < 0)
			break;

		memcpy(val + i, usb->data, batch_len);
		i += batch_len;
>>>>>>> upstream/android-13
	}
	mutex_unlock(&usb->usb_ctrl_mtx);
}

void mt76u_single_wr(struct mt76_dev *dev, const u8 req,
		     const u16 offset, const u32 val)
{
	mutex_lock(&dev->usb.usb_ctrl_mtx);
	__mt76u_vendor_request(dev, req,
			       USB_DIR_OUT | USB_TYPE_VENDOR,
			       val & 0xffff, offset, NULL, 0);
	__mt76u_vendor_request(dev, req,
			       USB_DIR_OUT | USB_TYPE_VENDOR,
			       val >> 16, offset + 2, NULL, 0);
	mutex_unlock(&dev->usb.usb_ctrl_mtx);
}
EXPORT_SYMBOL_GPL(mt76u_single_wr);

static int
<<<<<<< HEAD
=======
mt76u_req_wr_rp(struct mt76_dev *dev, u32 base,
		const struct mt76_reg_pair *data, int len)
{
	struct mt76_usb *usb = &dev->usb;

	mutex_lock(&usb->usb_ctrl_mtx);
	while (len > 0) {
		__mt76u_wr(dev, base + data->reg, data->value);
		len--;
		data++;
	}
	mutex_unlock(&usb->usb_ctrl_mtx);

	return 0;
}

static int
mt76u_wr_rp(struct mt76_dev *dev, u32 base,
	    const struct mt76_reg_pair *data, int n)
{
	if (test_bit(MT76_STATE_MCU_RUNNING, &dev->phy.state))
		return dev->mcu_ops->mcu_wr_rp(dev, base, data, n);
	else
		return mt76u_req_wr_rp(dev, base, data, n);
}

static int
mt76u_req_rd_rp(struct mt76_dev *dev, u32 base, struct mt76_reg_pair *data,
		int len)
{
	struct mt76_usb *usb = &dev->usb;

	mutex_lock(&usb->usb_ctrl_mtx);
	while (len > 0) {
		data->value = __mt76u_rr(dev, base + data->reg);
		len--;
		data++;
	}
	mutex_unlock(&usb->usb_ctrl_mtx);

	return 0;
}

static int
mt76u_rd_rp(struct mt76_dev *dev, u32 base,
	    struct mt76_reg_pair *data, int n)
{
	if (test_bit(MT76_STATE_MCU_RUNNING, &dev->phy.state))
		return dev->mcu_ops->mcu_rd_rp(dev, base, data, n);
	else
		return mt76u_req_rd_rp(dev, base, data, n);
}

static bool mt76u_check_sg(struct mt76_dev *dev)
{
	struct usb_interface *uintf = to_usb_interface(dev->dev);
	struct usb_device *udev = interface_to_usbdev(uintf);

	return (!disable_usb_sg && udev->bus->sg_tablesize > 0 &&
		(udev->bus->no_sg_constraint ||
		 udev->speed == USB_SPEED_WIRELESS));
}

static int
>>>>>>> upstream/android-13
mt76u_set_endpoints(struct usb_interface *intf,
		    struct mt76_usb *usb)
{
	struct usb_host_interface *intf_desc = intf->cur_altsetting;
	struct usb_endpoint_descriptor *ep_desc;
	int i, in_ep = 0, out_ep = 0;

	for (i = 0; i < intf_desc->desc.bNumEndpoints; i++) {
		ep_desc = &intf_desc->endpoint[i].desc;

		if (usb_endpoint_is_bulk_in(ep_desc) &&
		    in_ep < __MT_EP_IN_MAX) {
			usb->in_ep[in_ep] = usb_endpoint_num(ep_desc);
<<<<<<< HEAD
			usb->in_max_packet = usb_endpoint_maxp(ep_desc);
=======
>>>>>>> upstream/android-13
			in_ep++;
		} else if (usb_endpoint_is_bulk_out(ep_desc) &&
			   out_ep < __MT_EP_OUT_MAX) {
			usb->out_ep[out_ep] = usb_endpoint_num(ep_desc);
<<<<<<< HEAD
			usb->out_max_packet = usb_endpoint_maxp(ep_desc);
=======
>>>>>>> upstream/android-13
			out_ep++;
		}
	}

	if (in_ep != __MT_EP_IN_MAX || out_ep != __MT_EP_OUT_MAX)
		return -EINVAL;
	return 0;
}

static int
<<<<<<< HEAD
mt76u_fill_rx_sg(struct mt76_dev *dev, struct mt76u_buf *buf,
		 int nsgs, int len, int sglen)
{
	struct urb *urb = buf->urb;
=======
mt76u_fill_rx_sg(struct mt76_dev *dev, struct mt76_queue *q, struct urb *urb,
		 int nsgs, gfp_t gfp)
{
>>>>>>> upstream/android-13
	int i;

	for (i = 0; i < nsgs; i++) {
		struct page *page;
		void *data;
		int offset;

<<<<<<< HEAD
		data = netdev_alloc_frag(len);
=======
		data = page_frag_alloc(&q->rx_page, q->buf_size, gfp);
>>>>>>> upstream/android-13
		if (!data)
			break;

		page = virt_to_head_page(data);
		offset = data - page_address(page);
<<<<<<< HEAD
		sg_set_page(&urb->sg[i], page, sglen, offset);
=======
		sg_set_page(&urb->sg[i], page, q->buf_size, offset);
>>>>>>> upstream/android-13
	}

	if (i < nsgs) {
		int j;

		for (j = nsgs; j < urb->num_sgs; j++)
			skb_free_frag(sg_virt(&urb->sg[j]));
		urb->num_sgs = i;
	}

	urb->num_sgs = max_t(int, i, urb->num_sgs);
<<<<<<< HEAD
	buf->len = urb->num_sgs * sglen,
=======
	urb->transfer_buffer_length = urb->num_sgs * q->buf_size;
>>>>>>> upstream/android-13
	sg_init_marker(urb->sg, urb->num_sgs);

	return i ? : -ENOMEM;
}

<<<<<<< HEAD
int mt76u_buf_alloc(struct mt76_dev *dev, struct mt76u_buf *buf,
		    int nsgs, int len, int sglen, gfp_t gfp)
{
	buf->urb = usb_alloc_urb(0, gfp);
	if (!buf->urb)
		return -ENOMEM;

	buf->urb->sg = devm_kcalloc(dev->dev, nsgs, sizeof(*buf->urb->sg),
				    gfp);
	if (!buf->urb->sg)
		return -ENOMEM;

	sg_init_table(buf->urb->sg, nsgs);
	buf->dev = dev;

	return mt76u_fill_rx_sg(dev, buf, nsgs, len, sglen);
}
EXPORT_SYMBOL_GPL(mt76u_buf_alloc);

void mt76u_buf_free(struct mt76u_buf *buf)
{
	struct urb *urb = buf->urb;
	struct scatterlist *sg;
	int i;

	for (i = 0; i < urb->num_sgs; i++) {
		sg = &urb->sg[i];
		if (!sg)
			continue;

		skb_free_frag(sg_virt(sg));
	}
	usb_free_urb(buf->urb);
}
EXPORT_SYMBOL_GPL(mt76u_buf_free);

int mt76u_submit_buf(struct mt76_dev *dev, int dir, int index,
		     struct mt76u_buf *buf, gfp_t gfp,
		     usb_complete_t complete_fn, void *context)
{
	struct usb_interface *intf = to_usb_interface(dev->dev);
	struct usb_device *udev = interface_to_usbdev(intf);
=======
static int
mt76u_refill_rx(struct mt76_dev *dev, struct mt76_queue *q,
		struct urb *urb, int nsgs, gfp_t gfp)
{
	enum mt76_rxq_id qid = q - &dev->q_rx[MT_RXQ_MAIN];

	if (qid == MT_RXQ_MAIN && dev->usb.sg_en)
		return mt76u_fill_rx_sg(dev, q, urb, nsgs, gfp);

	urb->transfer_buffer_length = q->buf_size;
	urb->transfer_buffer = page_frag_alloc(&q->rx_page, q->buf_size, gfp);

	return urb->transfer_buffer ? 0 : -ENOMEM;
}

static int
mt76u_urb_alloc(struct mt76_dev *dev, struct mt76_queue_entry *e,
		int sg_max_size)
{
	unsigned int size = sizeof(struct urb);

	if (dev->usb.sg_en)
		size += sg_max_size * sizeof(struct scatterlist);

	e->urb = kzalloc(size, GFP_KERNEL);
	if (!e->urb)
		return -ENOMEM;

	usb_init_urb(e->urb);

	if (dev->usb.sg_en && sg_max_size > 0)
		e->urb->sg = (struct scatterlist *)(e->urb + 1);

	return 0;
}

static int
mt76u_rx_urb_alloc(struct mt76_dev *dev, struct mt76_queue *q,
		   struct mt76_queue_entry *e)
{
	enum mt76_rxq_id qid = q - &dev->q_rx[MT_RXQ_MAIN];
	int err, sg_size;

	sg_size = qid == MT_RXQ_MAIN ? MT_RX_SG_MAX_SIZE : 0;
	err = mt76u_urb_alloc(dev, e, sg_size);
	if (err)
		return err;

	return mt76u_refill_rx(dev, q, e->urb, sg_size, GFP_KERNEL);
}

static void mt76u_urb_free(struct urb *urb)
{
	int i;

	for (i = 0; i < urb->num_sgs; i++)
		skb_free_frag(sg_virt(&urb->sg[i]));

	if (urb->transfer_buffer)
		skb_free_frag(urb->transfer_buffer);

	usb_free_urb(urb);
}

static void
mt76u_fill_bulk_urb(struct mt76_dev *dev, int dir, int index,
		    struct urb *urb, usb_complete_t complete_fn,
		    void *context)
{
	struct usb_interface *uintf = to_usb_interface(dev->dev);
	struct usb_device *udev = interface_to_usbdev(uintf);
>>>>>>> upstream/android-13
	unsigned int pipe;

	if (dir == USB_DIR_IN)
		pipe = usb_rcvbulkpipe(udev, dev->usb.in_ep[index]);
	else
		pipe = usb_sndbulkpipe(udev, dev->usb.out_ep[index]);

<<<<<<< HEAD
	usb_fill_bulk_urb(buf->urb, udev, pipe, NULL, buf->len,
			  complete_fn, context);

	return usb_submit_urb(buf->urb, gfp);
}
EXPORT_SYMBOL_GPL(mt76u_submit_buf);

static inline struct mt76u_buf
*mt76u_get_next_rx_entry(struct mt76_queue *q)
{
	struct mt76u_buf *buf = NULL;
=======
	urb->dev = udev;
	urb->pipe = pipe;
	urb->complete = complete_fn;
	urb->context = context;
}

static struct urb *
mt76u_get_next_rx_entry(struct mt76_queue *q)
{
	struct urb *urb = NULL;
>>>>>>> upstream/android-13
	unsigned long flags;

	spin_lock_irqsave(&q->lock, flags);
	if (q->queued > 0) {
<<<<<<< HEAD
		buf = &q->entry[q->head].ubuf;
		q->head = (q->head + 1) % q->ndesc;
=======
		urb = q->entry[q->tail].urb;
		q->tail = (q->tail + 1) % q->ndesc;
>>>>>>> upstream/android-13
		q->queued--;
	}
	spin_unlock_irqrestore(&q->lock, flags);

<<<<<<< HEAD
	return buf;
}

static int mt76u_get_rx_entry_len(u8 *data, u32 data_len)
=======
	return urb;
}

static int
mt76u_get_rx_entry_len(struct mt76_dev *dev, u8 *data,
		       u32 data_len)
>>>>>>> upstream/android-13
{
	u16 dma_len, min_len;

	dma_len = get_unaligned_le16(data);
<<<<<<< HEAD
	min_len = MT_DMA_HDR_LEN + MT_RX_RXWI_LEN +
		  MT_FCE_INFO_LEN;

	if (data_len < min_len || WARN_ON(!dma_len) ||
	    WARN_ON(dma_len + MT_DMA_HDR_LEN > data_len) ||
	    WARN_ON(dma_len & 0x3))
=======
	if (dev->drv->drv_flags & MT_DRV_RX_DMA_HDR)
		return dma_len;

	min_len = MT_DMA_HDR_LEN + MT_RX_RXWI_LEN + MT_FCE_INFO_LEN;
	if (data_len < min_len || !dma_len ||
	    dma_len + MT_DMA_HDR_LEN > data_len ||
	    (dma_len & 0x3))
>>>>>>> upstream/android-13
		return -EINVAL;
	return dma_len;
}

<<<<<<< HEAD
static int
mt76u_process_rx_entry(struct mt76_dev *dev, struct urb *urb)
{
	struct mt76_queue *q = &dev->q_rx[MT_RXQ_MAIN];
	u8 *data = sg_virt(&urb->sg[0]);
	int data_len, len, nsgs = 1;
	struct sk_buff *skb;

	if (!test_bit(MT76_STATE_INITIALIZED, &dev->state))
		return 0;

	len = mt76u_get_rx_entry_len(data, urb->actual_length);
	if (len < 0)
		return 0;

	skb = build_skb(data, q->buf_size);
	if (!skb)
		return 0;

	data_len = min_t(int, len, urb->sg[0].length - MT_DMA_HDR_LEN);
	skb_reserve(skb, MT_DMA_HDR_LEN);
	if (skb->tail + data_len > skb->end) {
		dev_kfree_skb(skb);
		return 1;
	}

	__skb_put(skb, data_len);
	len -= data_len;

	while (len > 0) {
		data_len = min_t(int, len, urb->sg[nsgs].length);
		skb_add_rx_frag(skb, skb_shinfo(skb)->nr_frags,
				sg_page(&urb->sg[nsgs]),
				urb->sg[nsgs].offset,
				data_len, q->buf_size);
=======
static struct sk_buff *
mt76u_build_rx_skb(struct mt76_dev *dev, void *data,
		   int len, int buf_size)
{
	int head_room, drv_flags = dev->drv->drv_flags;
	struct sk_buff *skb;

	head_room = drv_flags & MT_DRV_RX_DMA_HDR ? 0 : MT_DMA_HDR_LEN;
	if (SKB_WITH_OVERHEAD(buf_size) < head_room + len) {
		struct page *page;

		/* slow path, not enough space for data and
		 * skb_shared_info
		 */
		skb = alloc_skb(MT_SKB_HEAD_LEN, GFP_ATOMIC);
		if (!skb)
			return NULL;

		skb_put_data(skb, data + head_room, MT_SKB_HEAD_LEN);
		data += head_room + MT_SKB_HEAD_LEN;
		page = virt_to_head_page(data);
		skb_add_rx_frag(skb, skb_shinfo(skb)->nr_frags,
				page, data - page_address(page),
				len - MT_SKB_HEAD_LEN, buf_size);

		return skb;
	}

	/* fast path */
	skb = build_skb(data, buf_size);
	if (!skb)
		return NULL;

	skb_reserve(skb, head_room);
	__skb_put(skb, len);

	return skb;
}

static int
mt76u_process_rx_entry(struct mt76_dev *dev, struct urb *urb,
		       int buf_size)
{
	u8 *data = urb->num_sgs ? sg_virt(&urb->sg[0]) : urb->transfer_buffer;
	int data_len = urb->num_sgs ? urb->sg[0].length : urb->actual_length;
	int len, nsgs = 1, head_room, drv_flags = dev->drv->drv_flags;
	struct sk_buff *skb;

	if (!test_bit(MT76_STATE_INITIALIZED, &dev->phy.state))
		return 0;

	len = mt76u_get_rx_entry_len(dev, data, urb->actual_length);
	if (len < 0)
		return 0;

	head_room = drv_flags & MT_DRV_RX_DMA_HDR ? 0 : MT_DMA_HDR_LEN;
	data_len = min_t(int, len, data_len - head_room);
	skb = mt76u_build_rx_skb(dev, data, data_len, buf_size);
	if (!skb)
		return 0;

	len -= data_len;
	while (len > 0 && nsgs < urb->num_sgs) {
		data_len = min_t(int, len, urb->sg[nsgs].length);
		skb_add_rx_frag(skb, skb_shinfo(skb)->nr_frags,
				sg_page(&urb->sg[nsgs]),
				urb->sg[nsgs].offset, data_len,
				buf_size);
>>>>>>> upstream/android-13
		len -= data_len;
		nsgs++;
	}
	dev->drv->rx_skb(dev, MT_RXQ_MAIN, skb);

	return nsgs;
}

static void mt76u_complete_rx(struct urb *urb)
{
<<<<<<< HEAD
	struct mt76_dev *dev = urb->context;
	struct mt76_queue *q = &dev->q_rx[MT_RXQ_MAIN];
	unsigned long flags;

=======
	struct mt76_dev *dev = dev_get_drvdata(&urb->dev->dev);
	struct mt76_queue *q = urb->context;
	unsigned long flags;

	trace_rx_urb(dev, urb);

>>>>>>> upstream/android-13
	switch (urb->status) {
	case -ECONNRESET:
	case -ESHUTDOWN:
	case -ENOENT:
<<<<<<< HEAD
		return;
	default:
		dev_err(dev->dev, "rx urb failed: %d\n", urb->status);
		/* fall through */
=======
	case -EPROTO:
		return;
	default:
		dev_err_ratelimited(dev->dev, "rx urb failed: %d\n",
				    urb->status);
		fallthrough;
>>>>>>> upstream/android-13
	case 0:
		break;
	}

	spin_lock_irqsave(&q->lock, flags);
<<<<<<< HEAD
	if (WARN_ONCE(q->entry[q->tail].ubuf.urb != urb, "rx urb mismatch"))
		goto out;

	q->tail = (q->tail + 1) % q->ndesc;
	q->queued++;
	tasklet_schedule(&dev->usb.rx_tasklet);
=======
	if (WARN_ONCE(q->entry[q->head].urb != urb, "rx urb mismatch"))
		goto out;

	q->head = (q->head + 1) % q->ndesc;
	q->queued++;
	mt76_worker_schedule(&dev->usb.rx_worker);
>>>>>>> upstream/android-13
out:
	spin_unlock_irqrestore(&q->lock, flags);
}

<<<<<<< HEAD
static void mt76u_rx_tasklet(unsigned long data)
{
	struct mt76_dev *dev = (struct mt76_dev *)data;
	struct mt76_queue *q = &dev->q_rx[MT_RXQ_MAIN];
	int err, nsgs, buf_len = q->buf_size;
	struct mt76u_buf *buf;

	rcu_read_lock();

	while (true) {
		buf = mt76u_get_next_rx_entry(q);
		if (!buf)
			break;

		nsgs = mt76u_process_rx_entry(dev, buf->urb);
		if (nsgs > 0) {
			err = mt76u_fill_rx_sg(dev, buf, nsgs,
					       buf_len,
					       SKB_WITH_OVERHEAD(buf_len));
			if (err < 0)
				break;
		}
		mt76u_submit_buf(dev, USB_DIR_IN, MT_EP_IN_PKT_RX,
				 buf, GFP_ATOMIC,
				 mt76u_complete_rx, dev);
	}
	mt76_rx_poll_complete(dev, MT_RXQ_MAIN, NULL);

	rcu_read_unlock();
}

int mt76u_submit_rx_buffers(struct mt76_dev *dev)
{
	struct mt76_queue *q = &dev->q_rx[MT_RXQ_MAIN];
=======
static int
mt76u_submit_rx_buf(struct mt76_dev *dev, enum mt76_rxq_id qid,
		    struct urb *urb)
{
	int ep = qid == MT_RXQ_MAIN ? MT_EP_IN_PKT_RX : MT_EP_IN_CMD_RESP;

	mt76u_fill_bulk_urb(dev, USB_DIR_IN, ep, urb,
			    mt76u_complete_rx, &dev->q_rx[qid]);
	trace_submit_urb(dev, urb);

	return usb_submit_urb(urb, GFP_ATOMIC);
}

static void
mt76u_process_rx_queue(struct mt76_dev *dev, struct mt76_queue *q)
{
	int qid = q - &dev->q_rx[MT_RXQ_MAIN];
	struct urb *urb;
	int err, count;

	while (true) {
		urb = mt76u_get_next_rx_entry(q);
		if (!urb)
			break;

		count = mt76u_process_rx_entry(dev, urb, q->buf_size);
		if (count > 0) {
			err = mt76u_refill_rx(dev, q, urb, count, GFP_ATOMIC);
			if (err < 0)
				break;
		}
		mt76u_submit_rx_buf(dev, qid, urb);
	}
	if (qid == MT_RXQ_MAIN) {
		local_bh_disable();
		mt76_rx_poll_complete(dev, MT_RXQ_MAIN, NULL);
		local_bh_enable();
	}
}

static void mt76u_rx_worker(struct mt76_worker *w)
{
	struct mt76_usb *usb = container_of(w, struct mt76_usb, rx_worker);
	struct mt76_dev *dev = container_of(usb, struct mt76_dev, usb);
	int i;

	rcu_read_lock();
	mt76_for_each_q_rx(dev, i)
		mt76u_process_rx_queue(dev, &dev->q_rx[i]);
	rcu_read_unlock();
}

static int
mt76u_submit_rx_buffers(struct mt76_dev *dev, enum mt76_rxq_id qid)
{
	struct mt76_queue *q = &dev->q_rx[qid];
>>>>>>> upstream/android-13
	unsigned long flags;
	int i, err = 0;

	spin_lock_irqsave(&q->lock, flags);
	for (i = 0; i < q->ndesc; i++) {
<<<<<<< HEAD
		err = mt76u_submit_buf(dev, USB_DIR_IN, MT_EP_IN_PKT_RX,
				       &q->entry[i].ubuf, GFP_ATOMIC,
				       mt76u_complete_rx, dev);
=======
		err = mt76u_submit_rx_buf(dev, qid, q->entry[i].urb);
>>>>>>> upstream/android-13
		if (err < 0)
			break;
	}
	q->head = q->tail = 0;
	q->queued = 0;
	spin_unlock_irqrestore(&q->lock, flags);

	return err;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(mt76u_submit_rx_buffers);

static int mt76u_alloc_rx(struct mt76_dev *dev)
{
	struct mt76_queue *q = &dev->q_rx[MT_RXQ_MAIN];
	int i, err, nsgs;
=======

static int
mt76u_alloc_rx_queue(struct mt76_dev *dev, enum mt76_rxq_id qid)
{
	struct mt76_queue *q = &dev->q_rx[qid];
	int i, err;
>>>>>>> upstream/android-13

	spin_lock_init(&q->lock);
	q->entry = devm_kcalloc(dev->dev,
				MT_NUM_RX_ENTRIES, sizeof(*q->entry),
				GFP_KERNEL);
	if (!q->entry)
		return -ENOMEM;

<<<<<<< HEAD
	if (mt76u_check_sg(dev)) {
		q->buf_size = MT_RX_BUF_SIZE;
		nsgs = MT_SG_MAX_SIZE;
	} else {
		q->buf_size = PAGE_SIZE;
		nsgs = 1;
	}

	q->ndesc = MT_NUM_RX_ENTRIES;
	for (i = 0; i < q->ndesc; i++) {
		err = mt76u_buf_alloc(dev, &q->entry[i].ubuf,
				      nsgs, q->buf_size,
				      SKB_WITH_OVERHEAD(q->buf_size),
				      GFP_KERNEL);
=======
	q->ndesc = MT_NUM_RX_ENTRIES;
	q->buf_size = PAGE_SIZE;

	for (i = 0; i < q->ndesc; i++) {
		err = mt76u_rx_urb_alloc(dev, q, &q->entry[i]);
>>>>>>> upstream/android-13
		if (err < 0)
			return err;
	}

<<<<<<< HEAD
	return mt76u_submit_rx_buffers(dev);
=======
	return mt76u_submit_rx_buffers(dev, qid);
}

int mt76u_alloc_mcu_queue(struct mt76_dev *dev)
{
	return mt76u_alloc_rx_queue(dev, MT_RXQ_MCU);
}
EXPORT_SYMBOL_GPL(mt76u_alloc_mcu_queue);

static void
mt76u_free_rx_queue(struct mt76_dev *dev, struct mt76_queue *q)
{
	struct page *page;
	int i;

	for (i = 0; i < q->ndesc; i++) {
		if (!q->entry[i].urb)
			continue;

		mt76u_urb_free(q->entry[i].urb);
		q->entry[i].urb = NULL;
	}

	if (!q->rx_page.va)
		return;

	page = virt_to_page(q->rx_page.va);
	__page_frag_cache_drain(page, q->rx_page.pagecnt_bias);
	memset(&q->rx_page, 0, sizeof(q->rx_page));
>>>>>>> upstream/android-13
}

static void mt76u_free_rx(struct mt76_dev *dev)
{
<<<<<<< HEAD
	struct mt76_queue *q = &dev->q_rx[MT_RXQ_MAIN];
	int i;

	for (i = 0; i < q->ndesc; i++)
		mt76u_buf_free(&q->entry[i].ubuf);
}

static void mt76u_stop_rx(struct mt76_dev *dev)
{
	struct mt76_queue *q = &dev->q_rx[MT_RXQ_MAIN];
	int i;

	for (i = 0; i < q->ndesc; i++)
		usb_kill_urb(q->entry[i].ubuf.urb);
}

int mt76u_skb_dma_info(struct sk_buff *skb, int port, u32 flags)
{
	struct sk_buff *iter, *last = skb;
	u32 info, pad;

	/* Buffer layout:
	 *	|   4B   | xfer len |      pad       |  4B  |
	 *	| TXINFO | pkt/cmd  | zero pad to 4B | zero |
	 *
	 * length field of TXINFO should be set to 'xfer len'.
	 */
	info = FIELD_PREP(MT_TXD_INFO_LEN, round_up(skb->len, 4)) |
	       FIELD_PREP(MT_TXD_INFO_DPORT, port) | flags;
	put_unaligned_le32(info, skb_push(skb, sizeof(info)));

	pad = round_up(skb->len, 4) + 4 - skb->len;
	skb_walk_frags(skb, iter) {
		last = iter;
		if (!iter->next) {
			skb->data_len += pad;
			skb->len += pad;
			break;
		}
	}

	if (unlikely(pad)) {
		if (__skb_pad(last, pad, true))
			return -ENOMEM;
		__skb_put(last, pad);
	}
	return 0;
}
EXPORT_SYMBOL_GPL(mt76u_skb_dma_info);

static void mt76u_tx_tasklet(unsigned long data)
{
	struct mt76_dev *dev = (struct mt76_dev *)data;
	struct mt76u_buf *buf;
	struct mt76_queue *q;
	bool wake;
	int i;

	for (i = 0; i < IEEE80211_NUM_ACS; i++) {
		q = &dev->q_tx[i];

		spin_lock_bh(&q->lock);
		while (true) {
			buf = &q->entry[q->head].ubuf;
			if (!buf->done || !q->queued)
				break;

			dev->drv->tx_complete_skb(dev, q,
						  &q->entry[q->head],
						  false);

			if (q->entry[q->head].schedule) {
				q->entry[q->head].schedule = false;
				q->swq_queued--;
			}

			q->head = (q->head + 1) % q->ndesc;
			q->queued--;
		}
		mt76_txq_schedule(dev, q);
		wake = i < IEEE80211_NUM_ACS && q->queued < q->ndesc - 8;
		if (!q->queued)
			wake_up(&dev->tx_wait);

		spin_unlock_bh(&q->lock);

		if (!test_and_set_bit(MT76_READING_STATS, &dev->state))
			ieee80211_queue_delayed_work(dev->hw,
						     &dev->usb.stat_work,
						     msecs_to_jiffies(10));

		if (wake)
			ieee80211_wake_queue(dev->hw, i);
=======
	int i;

	mt76_worker_teardown(&dev->usb.rx_worker);

	mt76_for_each_q_rx(dev, i)
		mt76u_free_rx_queue(dev, &dev->q_rx[i]);
}

void mt76u_stop_rx(struct mt76_dev *dev)
{
	int i;

	mt76_worker_disable(&dev->usb.rx_worker);

	mt76_for_each_q_rx(dev, i) {
		struct mt76_queue *q = &dev->q_rx[i];
		int j;

		for (j = 0; j < q->ndesc; j++)
			usb_poison_urb(q->entry[j].urb);
	}
}
EXPORT_SYMBOL_GPL(mt76u_stop_rx);

int mt76u_resume_rx(struct mt76_dev *dev)
{
	int i;

	mt76_for_each_q_rx(dev, i) {
		struct mt76_queue *q = &dev->q_rx[i];
		int err, j;

		for (j = 0; j < q->ndesc; j++)
			usb_unpoison_urb(q->entry[j].urb);

		err = mt76u_submit_rx_buffers(dev, i);
		if (err < 0)
			return err;
	}

	mt76_worker_enable(&dev->usb.rx_worker);

	return 0;
}
EXPORT_SYMBOL_GPL(mt76u_resume_rx);

static void mt76u_status_worker(struct mt76_worker *w)
{
	struct mt76_usb *usb = container_of(w, struct mt76_usb, status_worker);
	struct mt76_dev *dev = container_of(usb, struct mt76_dev, usb);
	struct mt76_queue_entry entry;
	struct mt76_queue *q;
	int i;

	for (i = 0; i < IEEE80211_NUM_ACS; i++) {
		q = dev->phy.q_tx[i];
		if (!q)
			continue;

		while (q->queued > 0) {
			if (!q->entry[q->tail].done)
				break;

			entry = q->entry[q->tail];
			q->entry[q->tail].done = false;

			mt76_queue_tx_complete(dev, q, &entry);
		}

		if (!q->queued)
			wake_up(&dev->tx_wait);

		mt76_worker_schedule(&dev->tx_worker);

		if (dev->drv->tx_status_data &&
		    !test_and_set_bit(MT76_READING_STATS, &dev->phy.state))
			queue_work(dev->wq, &dev->usb.stat_work);
>>>>>>> upstream/android-13
	}
}

static void mt76u_tx_status_data(struct work_struct *work)
{
	struct mt76_usb *usb;
	struct mt76_dev *dev;
	u8 update = 1;
	u16 count = 0;

<<<<<<< HEAD
	usb = container_of(work, struct mt76_usb, stat_work.work);
	dev = container_of(usb, struct mt76_dev, usb);

	while (true) {
		if (test_bit(MT76_REMOVED, &dev->state))
=======
	usb = container_of(work, struct mt76_usb, stat_work);
	dev = container_of(usb, struct mt76_dev, usb);

	while (true) {
		if (test_bit(MT76_REMOVED, &dev->phy.state))
>>>>>>> upstream/android-13
			break;

		if (!dev->drv->tx_status_data(dev, &update))
			break;
		count++;
	}

<<<<<<< HEAD
	if (count && test_bit(MT76_STATE_RUNNING, &dev->state))
		ieee80211_queue_delayed_work(dev->hw, &usb->stat_work,
					     msecs_to_jiffies(10));
	else
		clear_bit(MT76_READING_STATS, &dev->state);
=======
	if (count && test_bit(MT76_STATE_RUNNING, &dev->phy.state))
		queue_work(dev->wq, &usb->stat_work);
	else
		clear_bit(MT76_READING_STATS, &dev->phy.state);
>>>>>>> upstream/android-13
}

static void mt76u_complete_tx(struct urb *urb)
{
<<<<<<< HEAD
	struct mt76u_buf *buf = urb->context;
	struct mt76_dev *dev = buf->dev;

	if (mt76u_urb_error(urb))
		dev_err(dev->dev, "tx urb failed: %d\n", urb->status);
	buf->done = true;

	tasklet_schedule(&dev->usb.tx_tasklet);
}

static int
mt76u_tx_build_sg(struct sk_buff *skb, struct urb *urb)
{
	int nsgs = 1 + skb_shinfo(skb)->nr_frags;
	struct sk_buff *iter;

	skb_walk_frags(skb, iter)
		nsgs += 1 + skb_shinfo(iter)->nr_frags;

	memset(urb->sg, 0, sizeof(*urb->sg) * MT_SG_MAX_SIZE);

	nsgs = min_t(int, MT_SG_MAX_SIZE, nsgs);
	sg_init_marker(urb->sg, nsgs);
	urb->num_sgs = nsgs;

	return skb_to_sgvec_nomark(skb, urb->sg, 0, skb->len);
=======
	struct mt76_dev *dev = dev_get_drvdata(&urb->dev->dev);
	struct mt76_queue_entry *e = urb->context;

	if (mt76u_urb_error(urb))
		dev_err(dev->dev, "tx urb failed: %d\n", urb->status);
	e->done = true;

	mt76_worker_schedule(&dev->usb.status_worker);
}

static int
mt76u_tx_setup_buffers(struct mt76_dev *dev, struct sk_buff *skb,
		       struct urb *urb)
{
	urb->transfer_buffer_length = skb->len;

	if (!dev->usb.sg_en) {
		urb->transfer_buffer = skb->data;
		return 0;
	}

	sg_init_table(urb->sg, MT_TX_SG_MAX_SIZE);
	urb->num_sgs = skb_to_sgvec(skb, urb->sg, 0, skb->len);
	if (!urb->num_sgs)
		return -ENOMEM;

	return urb->num_sgs;
>>>>>>> upstream/android-13
}

static int
mt76u_tx_queue_skb(struct mt76_dev *dev, struct mt76_queue *q,
		   struct sk_buff *skb, struct mt76_wcid *wcid,
		   struct ieee80211_sta *sta)
{
<<<<<<< HEAD
	struct usb_interface *intf = to_usb_interface(dev->dev);
	struct usb_device *udev = interface_to_usbdev(intf);
	u8 ep = q2ep(q->hw_idx);
	struct mt76u_buf *buf;
	u16 idx = q->tail;
	unsigned int pipe;
=======
	struct mt76_tx_info tx_info = {
		.skb = skb,
	};
	u16 idx = q->head;
>>>>>>> upstream/android-13
	int err;

	if (q->queued == q->ndesc)
		return -ENOSPC;

<<<<<<< HEAD
	err = dev->drv->tx_prepare_skb(dev, NULL, skb, q, wcid, sta, NULL);
	if (err < 0)
		return err;

	buf = &q->entry[idx].ubuf;
	buf->done = false;

	err = mt76u_tx_build_sg(skb, buf->urb);
	if (err < 0)
		return err;

	pipe = usb_sndbulkpipe(udev, dev->usb.out_ep[ep]);
	usb_fill_bulk_urb(buf->urb, udev, pipe, NULL, skb->len,
			  mt76u_complete_tx, buf);

	q->tail = (q->tail + 1) % q->ndesc;
	q->entry[idx].skb = skb;
=======
	skb->prev = skb->next = NULL;
	err = dev->drv->tx_prepare_skb(dev, NULL, q->qid, wcid, sta, &tx_info);
	if (err < 0)
		return err;

	err = mt76u_tx_setup_buffers(dev, tx_info.skb, q->entry[idx].urb);
	if (err < 0)
		return err;

	mt76u_fill_bulk_urb(dev, USB_DIR_OUT, q2ep(q->hw_idx),
			    q->entry[idx].urb, mt76u_complete_tx,
			    &q->entry[idx]);

	q->head = (q->head + 1) % q->ndesc;
	q->entry[idx].skb = tx_info.skb;
	q->entry[idx].wcid = 0xffff;
>>>>>>> upstream/android-13
	q->queued++;

	return idx;
}

static void mt76u_tx_kick(struct mt76_dev *dev, struct mt76_queue *q)
{
<<<<<<< HEAD
	struct mt76u_buf *buf;
	int err;

	while (q->first != q->tail) {
		buf = &q->entry[q->first].ubuf;
		err = usb_submit_urb(buf->urb, GFP_ATOMIC);
		if (err < 0) {
			if (err == -ENODEV)
				set_bit(MT76_REMOVED, &dev->state);
=======
	struct urb *urb;
	int err;

	while (q->first != q->head) {
		urb = q->entry[q->first].urb;

		trace_submit_urb(dev, urb);
		err = usb_submit_urb(urb, GFP_ATOMIC);
		if (err < 0) {
			if (err == -ENODEV)
				set_bit(MT76_REMOVED, &dev->phy.state);
>>>>>>> upstream/android-13
			else
				dev_err(dev->dev, "tx urb submit failed:%d\n",
					err);
			break;
		}
		q->first = (q->first + 1) % q->ndesc;
	}
}

<<<<<<< HEAD
static int mt76u_alloc_tx(struct mt76_dev *dev)
{
	struct mt76u_buf *buf;
	struct mt76_queue *q;
	size_t size;
	int i, j;

	size = MT_SG_MAX_SIZE * sizeof(struct scatterlist);
	for (i = 0; i < IEEE80211_NUM_ACS; i++) {
		q = &dev->q_tx[i];
		spin_lock_init(&q->lock);
		INIT_LIST_HEAD(&q->swq);
		q->hw_idx = q2hwq(i);
=======
static u8 mt76u_ac_to_hwq(struct mt76_dev *dev, u8 ac)
{
	if (mt76_chip(dev) == 0x7663) {
		static const u8 lmac_queue_map[] = {
			/* ac to lmac mapping */
			[IEEE80211_AC_BK] = 0,
			[IEEE80211_AC_BE] = 1,
			[IEEE80211_AC_VI] = 2,
			[IEEE80211_AC_VO] = 4,
		};

		if (WARN_ON(ac >= ARRAY_SIZE(lmac_queue_map)))
			return 1; /* BE */

		return lmac_queue_map[ac];
	}

	return mt76_ac_to_hwq(ac);
}

static int mt76u_alloc_tx(struct mt76_dev *dev)
{
	struct mt76_queue *q;
	int i, j, err;

	for (i = 0; i <= MT_TXQ_PSD; i++) {
		if (i >= IEEE80211_NUM_ACS) {
			dev->phy.q_tx[i] = dev->phy.q_tx[0];
			continue;
		}

		q = devm_kzalloc(dev->dev, sizeof(*q), GFP_KERNEL);
		if (!q)
			return -ENOMEM;

		spin_lock_init(&q->lock);
		q->hw_idx = mt76u_ac_to_hwq(dev, i);
		q->qid = i;

		dev->phy.q_tx[i] = q;
>>>>>>> upstream/android-13

		q->entry = devm_kcalloc(dev->dev,
					MT_NUM_TX_ENTRIES, sizeof(*q->entry),
					GFP_KERNEL);
		if (!q->entry)
			return -ENOMEM;

		q->ndesc = MT_NUM_TX_ENTRIES;
		for (j = 0; j < q->ndesc; j++) {
<<<<<<< HEAD
			buf = &q->entry[j].ubuf;
			buf->dev = dev;

			buf->urb = usb_alloc_urb(0, GFP_KERNEL);
			if (!buf->urb)
				return -ENOMEM;

			buf->urb->sg = devm_kzalloc(dev->dev, size, GFP_KERNEL);
			if (!buf->urb->sg)
				return -ENOMEM;
=======
			err = mt76u_urb_alloc(dev, &q->entry[j],
					      MT_TX_SG_MAX_SIZE);
			if (err < 0)
				return err;
>>>>>>> upstream/android-13
		}
	}
	return 0;
}

static void mt76u_free_tx(struct mt76_dev *dev)
{
<<<<<<< HEAD
	struct mt76_queue *q;
	int i, j;

	for (i = 0; i < IEEE80211_NUM_ACS; i++) {
		q = &dev->q_tx[i];
		for (j = 0; j < q->ndesc; j++)
			usb_free_urb(q->entry[j].ubuf.urb);
	}
}

static void mt76u_stop_tx(struct mt76_dev *dev)
{
	struct mt76_queue *q;
	int i, j;

	for (i = 0; i < IEEE80211_NUM_ACS; i++) {
		q = &dev->q_tx[i];
		for (j = 0; j < q->ndesc; j++)
			usb_kill_urb(q->entry[j].ubuf.urb);
	}
}

void mt76u_stop_queues(struct mt76_dev *dev)
{
	tasklet_disable(&dev->usb.rx_tasklet);
	tasklet_disable(&dev->usb.tx_tasklet);

	mt76u_stop_rx(dev);
	mt76u_stop_tx(dev);
}
EXPORT_SYMBOL_GPL(mt76u_stop_queues);

void mt76u_stop_stat_wk(struct mt76_dev *dev)
{
	cancel_delayed_work_sync(&dev->usb.stat_work);
	clear_bit(MT76_READING_STATS, &dev->state);
}
EXPORT_SYMBOL_GPL(mt76u_stop_stat_wk);

void mt76u_queues_deinit(struct mt76_dev *dev)
{
	mt76u_stop_queues(dev);
=======
	int i;

	mt76_worker_teardown(&dev->usb.status_worker);

	for (i = 0; i < IEEE80211_NUM_ACS; i++) {
		struct mt76_queue *q;
		int j;

		q = dev->phy.q_tx[i];
		if (!q)
			continue;

		for (j = 0; j < q->ndesc; j++) {
			usb_free_urb(q->entry[j].urb);
			q->entry[j].urb = NULL;
		}
	}
}

void mt76u_stop_tx(struct mt76_dev *dev)
{
	int ret;

	mt76_worker_disable(&dev->usb.status_worker);

	ret = wait_event_timeout(dev->tx_wait, !mt76_has_tx_pending(&dev->phy),
				 HZ / 5);
	if (!ret) {
		struct mt76_queue_entry entry;
		struct mt76_queue *q;
		int i, j;

		dev_err(dev->dev, "timed out waiting for pending tx\n");

		for (i = 0; i < IEEE80211_NUM_ACS; i++) {
			q = dev->phy.q_tx[i];
			if (!q)
				continue;

			for (j = 0; j < q->ndesc; j++)
				usb_kill_urb(q->entry[j].urb);
		}

		mt76_worker_disable(&dev->tx_worker);

		/* On device removal we maight queue skb's, but mt76u_tx_kick()
		 * will fail to submit urb, cleanup those skb's manually.
		 */
		for (i = 0; i < IEEE80211_NUM_ACS; i++) {
			q = dev->phy.q_tx[i];
			if (!q)
				continue;

			while (q->queued > 0) {
				entry = q->entry[q->tail];
				q->entry[q->tail].done = false;
				mt76_queue_tx_complete(dev, q, &entry);
			}
		}

		mt76_worker_enable(&dev->tx_worker);
	}

	cancel_work_sync(&dev->usb.stat_work);
	clear_bit(MT76_READING_STATS, &dev->phy.state);

	mt76_worker_enable(&dev->usb.status_worker);

	mt76_tx_status_check(dev, NULL, true);
}
EXPORT_SYMBOL_GPL(mt76u_stop_tx);

void mt76u_queues_deinit(struct mt76_dev *dev)
{
	mt76u_stop_rx(dev);
	mt76u_stop_tx(dev);
>>>>>>> upstream/android-13

	mt76u_free_rx(dev);
	mt76u_free_tx(dev);
}
EXPORT_SYMBOL_GPL(mt76u_queues_deinit);

int mt76u_alloc_queues(struct mt76_dev *dev)
{
	int err;

<<<<<<< HEAD
	err = mt76u_alloc_rx(dev);
=======
	err = mt76u_alloc_rx_queue(dev, MT_RXQ_MAIN);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	return mt76u_alloc_tx(dev);
}
EXPORT_SYMBOL_GPL(mt76u_alloc_queues);

static const struct mt76_queue_ops usb_queue_ops = {
	.tx_queue_skb = mt76u_tx_queue_skb,
	.kick = mt76u_tx_kick,
};

int mt76u_init(struct mt76_dev *dev,
<<<<<<< HEAD
	       struct usb_interface *intf)
{
	static const struct mt76_bus_ops mt76u_ops = {
		.rr = mt76u_rr,
		.wr = mt76u_wr,
		.rmw = mt76u_rmw,
		.copy = mt76u_copy,
	};
	struct mt76_usb *usb = &dev->usb;

	tasklet_init(&usb->rx_tasklet, mt76u_rx_tasklet, (unsigned long)dev);
	tasklet_init(&usb->tx_tasklet, mt76u_tx_tasklet, (unsigned long)dev);
	INIT_DELAYED_WORK(&usb->stat_work, mt76u_tx_status_data);
	skb_queue_head_init(&dev->rx_skb[MT_RXQ_MAIN]);

	init_completion(&usb->mcu.cmpl);
	mutex_init(&usb->mcu.mutex);
=======
	       struct usb_interface *intf, bool ext)
{
	static struct mt76_bus_ops mt76u_ops = {
		.read_copy = mt76u_read_copy_ext,
		.wr_rp = mt76u_wr_rp,
		.rd_rp = mt76u_rd_rp,
		.type = MT76_BUS_USB,
	};
	struct usb_device *udev = interface_to_usbdev(intf);
	struct mt76_usb *usb = &dev->usb;
	int err;

	mt76u_ops.rr = ext ? mt76u_rr_ext : mt76u_rr;
	mt76u_ops.wr = ext ? mt76u_wr_ext : mt76u_wr;
	mt76u_ops.rmw = ext ? mt76u_rmw_ext : mt76u_rmw;
	mt76u_ops.write_copy = ext ? mt76u_copy_ext : mt76u_copy;

	INIT_WORK(&usb->stat_work, mt76u_tx_status_data);

	usb->data_len = usb_maxpacket(udev, usb_sndctrlpipe(udev, 0), 1);
	if (usb->data_len < 32)
		usb->data_len = 32;

	usb->data = devm_kmalloc(dev->dev, usb->data_len, GFP_KERNEL);
	if (!usb->data)
		return -ENOMEM;
>>>>>>> upstream/android-13

	mutex_init(&usb->usb_ctrl_mtx);
	dev->bus = &mt76u_ops;
	dev->queue_ops = &usb_queue_ops;

<<<<<<< HEAD
	return mt76u_set_endpoints(intf, usb);
=======
	dev_set_drvdata(&udev->dev, dev);

	usb->sg_en = mt76u_check_sg(dev);

	err = mt76u_set_endpoints(intf, usb);
	if (err < 0)
		return err;

	err = mt76_worker_setup(dev->hw, &usb->rx_worker, mt76u_rx_worker,
				"usb-rx");
	if (err)
		return err;

	err = mt76_worker_setup(dev->hw, &usb->status_worker,
				mt76u_status_worker, "usb-status");
	if (err)
		return err;

	sched_set_fifo_low(usb->rx_worker.task);
	sched_set_fifo_low(usb->status_worker.task);

	return 0;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(mt76u_init);

MODULE_AUTHOR("Lorenzo Bianconi <lorenzo.bianconi83@gmail.com>");
MODULE_LICENSE("Dual BSD/GPL");
