// SPDX-License-Identifier: GPL-2.0+
// Copyright (c) 2016-2017 Hisilicon Limited.

#include <linux/device.h>
#include <linux/dma-direction.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include "hclgevf_cmd.h"
#include "hclgevf_main.h"
#include "hnae3.h"

<<<<<<< HEAD
#define hclgevf_is_csq(ring) ((ring)->flag & HCLGEVF_TYPE_CSQ)
#define hclgevf_ring_to_dma_dir(ring) (hclgevf_is_csq(ring) ? \
					DMA_TO_DEVICE : DMA_FROM_DEVICE)
=======
>>>>>>> upstream/android-13
#define cmq_ring_to_dev(ring)   (&(ring)->dev->pdev->dev)

static int hclgevf_ring_space(struct hclgevf_cmq_ring *ring)
{
	int ntc = ring->next_to_clean;
	int ntu = ring->next_to_use;
	int used;

	used = (ntu - ntc + ring->desc_num) % ring->desc_num;

	return ring->desc_num - used - 1;
}

<<<<<<< HEAD
static int hclgevf_cmd_csq_clean(struct hclgevf_hw *hw)
{
	struct hclgevf_cmq_ring *csq = &hw->cmq.csq;
	u16 ntc = csq->next_to_clean;
	struct hclgevf_desc *desc;
	int clean = 0;
	u32 head;

	desc = &csq->desc[ntc];
	head = hclgevf_read_dev(hw, HCLGEVF_NIC_CSQ_HEAD_REG);
	while (head != ntc) {
		memset(desc, 0, sizeof(*desc));
		ntc++;
		if (ntc == csq->desc_num)
			ntc = 0;
		desc = &csq->desc[ntc];
		clean++;
	}
	csq->next_to_clean = ntc;

=======
static int hclgevf_is_valid_csq_clean_head(struct hclgevf_cmq_ring *ring,
					   int head)
{
	int ntu = ring->next_to_use;
	int ntc = ring->next_to_clean;

	if (ntu > ntc)
		return head >= ntc && head <= ntu;

	return head >= ntc || head <= ntu;
}

static int hclgevf_cmd_csq_clean(struct hclgevf_hw *hw)
{
	struct hclgevf_dev *hdev = container_of(hw, struct hclgevf_dev, hw);
	struct hclgevf_cmq_ring *csq = &hw->cmq.csq;
	int clean;
	u32 head;

	head = hclgevf_read_dev(hw, HCLGEVF_NIC_CSQ_HEAD_REG);
	rmb(); /* Make sure head is ready before touch any data */

	if (!hclgevf_is_valid_csq_clean_head(csq, head)) {
		dev_warn(&hdev->pdev->dev, "wrong cmd head (%u, %d-%d)\n", head,
			 csq->next_to_use, csq->next_to_clean);
		dev_warn(&hdev->pdev->dev,
			 "Disabling any further commands to IMP firmware\n");
		set_bit(HCLGEVF_STATE_CMD_DISABLE, &hdev->state);
		return -EIO;
	}

	clean = (head - csq->next_to_clean + csq->desc_num) % csq->desc_num;
	csq->next_to_clean = head;
>>>>>>> upstream/android-13
	return clean;
}

static bool hclgevf_cmd_csq_done(struct hclgevf_hw *hw)
{
	u32 head;

	head = hclgevf_read_dev(hw, HCLGEVF_NIC_CSQ_HEAD_REG);

	return head == hw->cmq.csq.next_to_use;
}

static bool hclgevf_is_special_opcode(u16 opcode)
{
<<<<<<< HEAD
	u16 spec_opcode[] = {0x30, 0x31, 0x32};
=======
	const u16 spec_opcode[] = {0x30, 0x31, 0x32};
>>>>>>> upstream/android-13
	int i;

	for (i = 0; i < ARRAY_SIZE(spec_opcode); i++) {
		if (spec_opcode[i] == opcode)
			return true;
	}

	return false;
}

<<<<<<< HEAD
=======
static void hclgevf_cmd_config_regs(struct hclgevf_cmq_ring *ring)
{
	struct hclgevf_dev *hdev = ring->dev;
	struct hclgevf_hw *hw = &hdev->hw;
	u32 reg_val;

	if (ring->flag == HCLGEVF_TYPE_CSQ) {
		reg_val = lower_32_bits(ring->desc_dma_addr);
		hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_BASEADDR_L_REG, reg_val);
		reg_val = upper_32_bits(ring->desc_dma_addr);
		hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_BASEADDR_H_REG, reg_val);

		reg_val = hclgevf_read_dev(hw, HCLGEVF_NIC_CSQ_DEPTH_REG);
		reg_val &= HCLGEVF_NIC_SW_RST_RDY;
		reg_val |= (ring->desc_num >> HCLGEVF_NIC_CMQ_DESC_NUM_S);
		hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_DEPTH_REG, reg_val);

		hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_HEAD_REG, 0);
		hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_TAIL_REG, 0);
	} else {
		reg_val = lower_32_bits(ring->desc_dma_addr);
		hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_BASEADDR_L_REG, reg_val);
		reg_val = upper_32_bits(ring->desc_dma_addr);
		hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_BASEADDR_H_REG, reg_val);

		reg_val = (ring->desc_num >> HCLGEVF_NIC_CMQ_DESC_NUM_S);
		hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_DEPTH_REG, reg_val);

		hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_HEAD_REG, 0);
		hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_TAIL_REG, 0);
	}
}

static void hclgevf_cmd_init_regs(struct hclgevf_hw *hw)
{
	hclgevf_cmd_config_regs(&hw->cmq.csq);
	hclgevf_cmd_config_regs(&hw->cmq.crq);
}

>>>>>>> upstream/android-13
static int hclgevf_alloc_cmd_desc(struct hclgevf_cmq_ring *ring)
{
	int size = ring->desc_num * sizeof(struct hclgevf_desc);

<<<<<<< HEAD
	ring->desc = dma_zalloc_coherent(cmq_ring_to_dev(ring),
					 size, &ring->desc_dma_addr,
					 GFP_KERNEL);
=======
	ring->desc = dma_alloc_coherent(cmq_ring_to_dev(ring), size,
					&ring->desc_dma_addr, GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!ring->desc)
		return -ENOMEM;

	return 0;
}

static void hclgevf_free_cmd_desc(struct hclgevf_cmq_ring *ring)
{
	int size  = ring->desc_num * sizeof(struct hclgevf_desc);

	if (ring->desc) {
		dma_free_coherent(cmq_ring_to_dev(ring), size,
				  ring->desc, ring->desc_dma_addr);
		ring->desc = NULL;
	}
}

<<<<<<< HEAD
static int hclgevf_init_cmd_queue(struct hclgevf_dev *hdev,
				  struct hclgevf_cmq_ring *ring)
{
	struct hclgevf_hw *hw = &hdev->hw;
	int ring_type = ring->flag;
	u32 reg_val;
	int ret;

	ring->desc_num = HCLGEVF_NIC_CMQ_DESC_NUM;
	spin_lock_init(&ring->lock);
	ring->next_to_clean = 0;
	ring->next_to_use = 0;
	ring->dev = hdev;

	/* allocate CSQ/CRQ descriptor */
	ret = hclgevf_alloc_cmd_desc(ring);
	if (ret) {
		dev_err(&hdev->pdev->dev, "failed(%d) to alloc %s desc\n", ret,
			(ring_type == HCLGEVF_TYPE_CSQ) ? "CSQ" : "CRQ");
		return ret;
	}

	/* initialize the hardware registers with csq/crq dma-address,
	 * descriptor number, head & tail pointers
	 */
	switch (ring_type) {
	case HCLGEVF_TYPE_CSQ:
		reg_val = (u32)ring->desc_dma_addr;
		hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_BASEADDR_L_REG, reg_val);
		reg_val = (u32)((ring->desc_dma_addr >> 31) >> 1);
		hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_BASEADDR_H_REG, reg_val);

		reg_val = (ring->desc_num >> HCLGEVF_NIC_CMQ_DESC_NUM_S);
		reg_val |= HCLGEVF_NIC_CMQ_ENABLE;
		hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_DEPTH_REG, reg_val);

		hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_HEAD_REG, 0);
		hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_TAIL_REG, 0);
		break;
	case HCLGEVF_TYPE_CRQ:
		reg_val = (u32)ring->desc_dma_addr;
		hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_BASEADDR_L_REG, reg_val);
		reg_val = (u32)((ring->desc_dma_addr >> 31) >> 1);
		hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_BASEADDR_H_REG, reg_val);

		reg_val = (ring->desc_num >> HCLGEVF_NIC_CMQ_DESC_NUM_S);
		reg_val |= HCLGEVF_NIC_CMQ_ENABLE;
		hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_DEPTH_REG, reg_val);

		hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_HEAD_REG, 0);
		hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_TAIL_REG, 0);
		break;
	}

	return 0;
=======
static int hclgevf_alloc_cmd_queue(struct hclgevf_dev *hdev, int ring_type)
{
	struct hclgevf_hw *hw = &hdev->hw;
	struct hclgevf_cmq_ring *ring =
		(ring_type == HCLGEVF_TYPE_CSQ) ? &hw->cmq.csq : &hw->cmq.crq;
	int ret;

	ring->dev = hdev;
	ring->flag = ring_type;

	/* allocate CSQ/CRQ descriptor */
	ret = hclgevf_alloc_cmd_desc(ring);
	if (ret)
		dev_err(&hdev->pdev->dev, "failed(%d) to alloc %s desc\n", ret,
			(ring_type == HCLGEVF_TYPE_CSQ) ? "CSQ" : "CRQ");

	return ret;
>>>>>>> upstream/android-13
}

void hclgevf_cmd_setup_basic_desc(struct hclgevf_desc *desc,
				  enum hclgevf_opcode_type opcode, bool is_read)
{
	memset(desc, 0, sizeof(struct hclgevf_desc));
	desc->opcode = cpu_to_le16(opcode);
	desc->flag = cpu_to_le16(HCLGEVF_CMD_FLAG_NO_INTR |
				 HCLGEVF_CMD_FLAG_IN);
	if (is_read)
		desc->flag |= cpu_to_le16(HCLGEVF_CMD_FLAG_WR);
	else
		desc->flag &= cpu_to_le16(~HCLGEVF_CMD_FLAG_WR);
}

<<<<<<< HEAD
=======
struct vf_errcode {
	u32 imp_errcode;
	int common_errno;
};

static void hclgevf_cmd_copy_desc(struct hclgevf_hw *hw,
				  struct hclgevf_desc *desc, int num)
{
	struct hclgevf_desc *desc_to_use;
	int handle = 0;

	while (handle < num) {
		desc_to_use = &hw->cmq.csq.desc[hw->cmq.csq.next_to_use];
		*desc_to_use = desc[handle];
		(hw->cmq.csq.next_to_use)++;
		if (hw->cmq.csq.next_to_use == hw->cmq.csq.desc_num)
			hw->cmq.csq.next_to_use = 0;
		handle++;
	}
}

static int hclgevf_cmd_convert_err_code(u16 desc_ret)
{
	struct vf_errcode hclgevf_cmd_errcode[] = {
		{HCLGEVF_CMD_EXEC_SUCCESS, 0},
		{HCLGEVF_CMD_NO_AUTH, -EPERM},
		{HCLGEVF_CMD_NOT_SUPPORTED, -EOPNOTSUPP},
		{HCLGEVF_CMD_QUEUE_FULL, -EXFULL},
		{HCLGEVF_CMD_NEXT_ERR, -ENOSR},
		{HCLGEVF_CMD_UNEXE_ERR, -ENOTBLK},
		{HCLGEVF_CMD_PARA_ERR, -EINVAL},
		{HCLGEVF_CMD_RESULT_ERR, -ERANGE},
		{HCLGEVF_CMD_TIMEOUT, -ETIME},
		{HCLGEVF_CMD_HILINK_ERR, -ENOLINK},
		{HCLGEVF_CMD_QUEUE_ILLEGAL, -ENXIO},
		{HCLGEVF_CMD_INVALID, -EBADR},
	};
	u32 errcode_count = ARRAY_SIZE(hclgevf_cmd_errcode);
	u32 i;

	for (i = 0; i < errcode_count; i++)
		if (hclgevf_cmd_errcode[i].imp_errcode == desc_ret)
			return hclgevf_cmd_errcode[i].common_errno;

	return -EIO;
}

static int hclgevf_cmd_check_retval(struct hclgevf_hw *hw,
				    struct hclgevf_desc *desc, int num, int ntc)
{
	u16 opcode, desc_ret;
	int handle;

	opcode = le16_to_cpu(desc[0].opcode);
	for (handle = 0; handle < num; handle++) {
		/* Get the result of hardware write back */
		desc[handle] = hw->cmq.csq.desc[ntc];
		ntc++;
		if (ntc == hw->cmq.csq.desc_num)
			ntc = 0;
	}
	if (likely(!hclgevf_is_special_opcode(opcode)))
		desc_ret = le16_to_cpu(desc[num - 1].retval);
	else
		desc_ret = le16_to_cpu(desc[0].retval);
	hw->cmq.last_status = desc_ret;

	return hclgevf_cmd_convert_err_code(desc_ret);
}

static int hclgevf_cmd_check_result(struct hclgevf_hw *hw,
				    struct hclgevf_desc *desc, int num, int ntc)
{
	struct hclgevf_dev *hdev = (struct hclgevf_dev *)hw->hdev;
	bool is_completed = false;
	u32 timeout = 0;
	int handle, ret;

	/* If the command is sync, wait for the firmware to write back,
	 * if multi descriptors to be sent, use the first one to check
	 */
	if (HCLGEVF_SEND_SYNC(le16_to_cpu(desc->flag))) {
		do {
			if (hclgevf_cmd_csq_done(hw)) {
				is_completed = true;
				break;
			}
			udelay(1);
			timeout++;
		} while (timeout < hw->cmq.tx_timeout);
	}

	if (!is_completed)
		ret = -EBADE;
	else
		ret = hclgevf_cmd_check_retval(hw, desc, num, ntc);

	/* Clean the command send queue */
	handle = hclgevf_cmd_csq_clean(hw);
	if (handle < 0)
		ret = handle;
	else if (handle != num)
		dev_warn(&hdev->pdev->dev,
			 "cleaned %d, need to clean %d\n", handle, num);
	return ret;
}

>>>>>>> upstream/android-13
/* hclgevf_cmd_send - send command to command queue
 * @hw: pointer to the hw struct
 * @desc: prefilled descriptor for describing the command
 * @num : the number of descriptors to be sent
 *
 * This is the main send command for command queue, it
 * sends the queue, cleans the queue, etc
 */
int hclgevf_cmd_send(struct hclgevf_hw *hw, struct hclgevf_desc *desc, int num)
{
	struct hclgevf_dev *hdev = (struct hclgevf_dev *)hw->hdev;
<<<<<<< HEAD
	struct hclgevf_desc *desc_to_use;
	bool complete = false;
	u32 timeout = 0;
	int handle = 0;
	int status = 0;
	u16 retval;
	u16 opcode;
=======
	struct hclgevf_cmq_ring *csq = &hw->cmq.csq;
	int ret;
>>>>>>> upstream/android-13
	int ntc;

	spin_lock_bh(&hw->cmq.csq.lock);

<<<<<<< HEAD
	if (num > hclgevf_ring_space(&hw->cmq.csq)) {
=======
	if (test_bit(HCLGEVF_STATE_CMD_DISABLE, &hdev->state)) {
		spin_unlock_bh(&hw->cmq.csq.lock);
		return -EBUSY;
	}

	if (num > hclgevf_ring_space(&hw->cmq.csq)) {
		/* If CMDQ ring is full, SW HEAD and HW HEAD may be different,
		 * need update the SW HEAD pointer csq->next_to_clean
		 */
		csq->next_to_clean = hclgevf_read_dev(hw,
						      HCLGEVF_NIC_CSQ_HEAD_REG);
>>>>>>> upstream/android-13
		spin_unlock_bh(&hw->cmq.csq.lock);
		return -EBUSY;
	}

	/* Record the location of desc in the ring for this time
	 * which will be use for hardware to write back
	 */
	ntc = hw->cmq.csq.next_to_use;
<<<<<<< HEAD
	opcode = le16_to_cpu(desc[0].opcode);
	while (handle < num) {
		desc_to_use = &hw->cmq.csq.desc[hw->cmq.csq.next_to_use];
		*desc_to_use = desc[handle];
		(hw->cmq.csq.next_to_use)++;
		if (hw->cmq.csq.next_to_use == hw->cmq.csq.desc_num)
			hw->cmq.csq.next_to_use = 0;
		handle++;
	}
=======

	hclgevf_cmd_copy_desc(hw, desc, num);
>>>>>>> upstream/android-13

	/* Write to hardware */
	hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_TAIL_REG,
			  hw->cmq.csq.next_to_use);

<<<<<<< HEAD
	/* If the command is sync, wait for the firmware to write back,
	 * if multi descriptors to be sent, use the first one to check
	 */
	if (HCLGEVF_SEND_SYNC(le16_to_cpu(desc->flag))) {
		do {
			if (hclgevf_cmd_csq_done(hw))
				break;
			udelay(1);
			timeout++;
		} while (timeout < hw->cmq.tx_timeout);
	}

	if (hclgevf_cmd_csq_done(hw)) {
		complete = true;
		handle = 0;

		while (handle < num) {
			/* Get the result of hardware write back */
			desc_to_use = &hw->cmq.csq.desc[ntc];
			desc[handle] = *desc_to_use;

			if (likely(!hclgevf_is_special_opcode(opcode)))
				retval = le16_to_cpu(desc[handle].retval);
			else
				retval = le16_to_cpu(desc[0].retval);

			if ((enum hclgevf_cmd_return_status)retval ==
			    HCLGEVF_CMD_EXEC_SUCCESS)
				status = 0;
			else
				status = -EIO;
			hw->cmq.last_status = (enum hclgevf_cmd_status)retval;
			ntc++;
			handle++;
			if (ntc == hw->cmq.csq.desc_num)
				ntc = 0;
		}
	}

	if (!complete)
		status = -EAGAIN;

	/* Clean the command send queue */
	handle = hclgevf_cmd_csq_clean(hw);
	if (handle != num) {
		dev_warn(&hdev->pdev->dev,
			 "cleaned %d, need to clean %d\n", handle, num);
	}

	spin_unlock_bh(&hw->cmq.csq.lock);

	return status;
}

static int  hclgevf_cmd_query_firmware_version(struct hclgevf_hw *hw,
					       u32 *version)
{
=======
	ret = hclgevf_cmd_check_result(hw, desc, num, ntc);

	spin_unlock_bh(&hw->cmq.csq.lock);

	return ret;
}

static void hclgevf_set_default_capability(struct hclgevf_dev *hdev)
{
	struct hnae3_ae_dev *ae_dev = pci_get_drvdata(hdev->pdev);

	set_bit(HNAE3_DEV_SUPPORT_FD_B, ae_dev->caps);
	set_bit(HNAE3_DEV_SUPPORT_GRO_B, ae_dev->caps);
	set_bit(HNAE3_DEV_SUPPORT_FEC_B, ae_dev->caps);
}

static const struct hclgevf_caps_bit_map hclgevf_cmd_caps_bit_map0[] = {
	{HCLGEVF_CAP_UDP_GSO_B, HNAE3_DEV_SUPPORT_UDP_GSO_B},
	{HCLGEVF_CAP_INT_QL_B, HNAE3_DEV_SUPPORT_INT_QL_B},
	{HCLGEVF_CAP_TQP_TXRX_INDEP_B, HNAE3_DEV_SUPPORT_TQP_TXRX_INDEP_B},
	{HCLGEVF_CAP_HW_TX_CSUM_B, HNAE3_DEV_SUPPORT_HW_TX_CSUM_B},
	{HCLGEVF_CAP_UDP_TUNNEL_CSUM_B, HNAE3_DEV_SUPPORT_UDP_TUNNEL_CSUM_B},
	{HCLGEVF_CAP_RXD_ADV_LAYOUT_B, HNAE3_DEV_SUPPORT_RXD_ADV_LAYOUT_B},
};

static void hclgevf_parse_capability(struct hclgevf_dev *hdev,
				     struct hclgevf_query_version_cmd *cmd)
{
	struct hnae3_ae_dev *ae_dev = pci_get_drvdata(hdev->pdev);
	u32 caps, i;

	caps = __le32_to_cpu(cmd->caps[0]);
	for (i = 0; i < ARRAY_SIZE(hclgevf_cmd_caps_bit_map0); i++)
		if (hnae3_get_bit(caps, hclgevf_cmd_caps_bit_map0[i].imp_bit))
			set_bit(hclgevf_cmd_caps_bit_map0[i].local_bit,
				ae_dev->caps);
}

static __le32 hclgevf_build_api_caps(void)
{
	u32 api_caps = 0;

	hnae3_set_bit(api_caps, HCLGEVF_API_CAP_FLEX_RSS_TBL_B, 1);

	return cpu_to_le32(api_caps);
}

static int hclgevf_cmd_query_version_and_capability(struct hclgevf_dev *hdev)
{
	struct hnae3_ae_dev *ae_dev = pci_get_drvdata(hdev->pdev);
>>>>>>> upstream/android-13
	struct hclgevf_query_version_cmd *resp;
	struct hclgevf_desc desc;
	int status;

	resp = (struct hclgevf_query_version_cmd *)desc.data;

	hclgevf_cmd_setup_basic_desc(&desc, HCLGEVF_OPC_QUERY_FW_VER, 1);
<<<<<<< HEAD
	status = hclgevf_cmd_send(hw, &desc, 1);
	if (!status)
		*version = le32_to_cpu(resp->firmware);
=======
	resp->api_caps = hclgevf_build_api_caps();
	status = hclgevf_cmd_send(&hdev->hw, &desc, 1);
	if (status)
		return status;

	hdev->fw_version = le32_to_cpu(resp->firmware);

	ae_dev->dev_version = le32_to_cpu(resp->hardware) <<
				 HNAE3_PCI_REVISION_BIT_SIZE;
	ae_dev->dev_version |= hdev->pdev->revision;

	if (ae_dev->dev_version >= HNAE3_DEVICE_VERSION_V2)
		hclgevf_set_default_capability(hdev);

	hclgevf_parse_capability(hdev, resp);
>>>>>>> upstream/android-13

	return status;
}

<<<<<<< HEAD
int hclgevf_cmd_init(struct hclgevf_dev *hdev)
{
	u32 version;
	int ret;

	/* setup Tx write back timeout */
	hdev->hw.cmq.tx_timeout = HCLGEVF_CMDQ_TX_TIMEOUT;

	/* setup queue CSQ/CRQ rings */
	hdev->hw.cmq.csq.flag = HCLGEVF_TYPE_CSQ;
	ret = hclgevf_init_cmd_queue(hdev, &hdev->hw.cmq.csq);
	if (ret) {
		dev_err(&hdev->pdev->dev,
			"failed(%d) to initialize CSQ ring\n", ret);
		return ret;
	}

	hdev->hw.cmq.crq.flag = HCLGEVF_TYPE_CRQ;
	ret = hclgevf_init_cmd_queue(hdev, &hdev->hw.cmq.crq);
	if (ret) {
		dev_err(&hdev->pdev->dev,
			"failed(%d) to initialize CRQ ring\n", ret);
		goto err_csq;
	}

=======
int hclgevf_cmd_queue_init(struct hclgevf_dev *hdev)
{
	int ret;

	/* Setup the lock for command queue */
	spin_lock_init(&hdev->hw.cmq.csq.lock);
	spin_lock_init(&hdev->hw.cmq.crq.lock);

	hdev->hw.cmq.tx_timeout = HCLGEVF_CMDQ_TX_TIMEOUT;
	hdev->hw.cmq.csq.desc_num = HCLGEVF_NIC_CMQ_DESC_NUM;
	hdev->hw.cmq.crq.desc_num = HCLGEVF_NIC_CMQ_DESC_NUM;

	ret = hclgevf_alloc_cmd_queue(hdev, HCLGEVF_TYPE_CSQ);
	if (ret) {
		dev_err(&hdev->pdev->dev,
			"CSQ ring setup error %d\n", ret);
		return ret;
	}

	ret = hclgevf_alloc_cmd_queue(hdev, HCLGEVF_TYPE_CRQ);
	if (ret) {
		dev_err(&hdev->pdev->dev,
			"CRQ ring setup error %d\n", ret);
		goto err_csq;
	}

	return 0;
err_csq:
	hclgevf_free_cmd_desc(&hdev->hw.cmq.csq);
	return ret;
}

int hclgevf_cmd_init(struct hclgevf_dev *hdev)
{
	int ret;

	spin_lock_bh(&hdev->hw.cmq.csq.lock);
	spin_lock(&hdev->hw.cmq.crq.lock);

>>>>>>> upstream/android-13
	/* initialize the pointers of async rx queue of mailbox */
	hdev->arq.hdev = hdev;
	hdev->arq.head = 0;
	hdev->arq.tail = 0;
<<<<<<< HEAD
	hdev->arq.count = 0;

	/* get firmware version */
	ret = hclgevf_cmd_query_firmware_version(&hdev->hw, &version);
	if (ret) {
		dev_err(&hdev->pdev->dev,
			"failed(%d) to query firmware version\n", ret);
		goto err_crq;
	}
	hdev->fw_version = version;

	dev_info(&hdev->pdev->dev, "The firmware version is %08x\n", version);

	return 0;
err_crq:
	hclgevf_free_cmd_desc(&hdev->hw.cmq.crq);
err_csq:
	hclgevf_free_cmd_desc(&hdev->hw.cmq.csq);
=======
	atomic_set(&hdev->arq.count, 0);
	hdev->hw.cmq.csq.next_to_clean = 0;
	hdev->hw.cmq.csq.next_to_use = 0;
	hdev->hw.cmq.crq.next_to_clean = 0;
	hdev->hw.cmq.crq.next_to_use = 0;

	hclgevf_cmd_init_regs(&hdev->hw);

	spin_unlock(&hdev->hw.cmq.crq.lock);
	spin_unlock_bh(&hdev->hw.cmq.csq.lock);

	clear_bit(HCLGEVF_STATE_CMD_DISABLE, &hdev->state);

	/* Check if there is new reset pending, because the higher level
	 * reset may happen when lower level reset is being processed.
	 */
	if (hclgevf_is_reset_pending(hdev)) {
		ret = -EBUSY;
		goto err_cmd_init;
	}

	/* get version and device capabilities */
	ret = hclgevf_cmd_query_version_and_capability(hdev);
	if (ret) {
		dev_err(&hdev->pdev->dev,
			"failed to query version and capabilities, ret = %d\n", ret);
		goto err_cmd_init;
	}

	dev_info(&hdev->pdev->dev, "The firmware version is %lu.%lu.%lu.%lu\n",
		 hnae3_get_field(hdev->fw_version, HNAE3_FW_VERSION_BYTE3_MASK,
				 HNAE3_FW_VERSION_BYTE3_SHIFT),
		 hnae3_get_field(hdev->fw_version, HNAE3_FW_VERSION_BYTE2_MASK,
				 HNAE3_FW_VERSION_BYTE2_SHIFT),
		 hnae3_get_field(hdev->fw_version, HNAE3_FW_VERSION_BYTE1_MASK,
				 HNAE3_FW_VERSION_BYTE1_SHIFT),
		 hnae3_get_field(hdev->fw_version, HNAE3_FW_VERSION_BYTE0_MASK,
				 HNAE3_FW_VERSION_BYTE0_SHIFT));

	return 0;

err_cmd_init:
	set_bit(HCLGEVF_STATE_CMD_DISABLE, &hdev->state);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
void hclgevf_cmd_uninit(struct hclgevf_dev *hdev)
{
=======
static void hclgevf_cmd_uninit_regs(struct hclgevf_hw *hw)
{
	hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_BASEADDR_L_REG, 0);
	hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_BASEADDR_H_REG, 0);
	hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_DEPTH_REG, 0);
	hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_HEAD_REG, 0);
	hclgevf_write_dev(hw, HCLGEVF_NIC_CSQ_TAIL_REG, 0);
	hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_BASEADDR_L_REG, 0);
	hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_BASEADDR_H_REG, 0);
	hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_DEPTH_REG, 0);
	hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_HEAD_REG, 0);
	hclgevf_write_dev(hw, HCLGEVF_NIC_CRQ_TAIL_REG, 0);
}

void hclgevf_cmd_uninit(struct hclgevf_dev *hdev)
{
	set_bit(HCLGEVF_STATE_CMD_DISABLE, &hdev->state);
	/* wait to ensure that the firmware completes the possible left
	 * over commands.
	 */
	msleep(HCLGEVF_CMDQ_CLEAR_WAIT_TIME);
	spin_lock_bh(&hdev->hw.cmq.csq.lock);
	spin_lock(&hdev->hw.cmq.crq.lock);
	hclgevf_cmd_uninit_regs(&hdev->hw);
	spin_unlock(&hdev->hw.cmq.crq.lock);
	spin_unlock_bh(&hdev->hw.cmq.csq.lock);

>>>>>>> upstream/android-13
	hclgevf_free_cmd_desc(&hdev->hw.cmq.csq);
	hclgevf_free_cmd_desc(&hdev->hw.cmq.crq);
}
