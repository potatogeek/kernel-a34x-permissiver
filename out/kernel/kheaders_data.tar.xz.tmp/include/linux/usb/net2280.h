// SPDX-License-Identifier: GPL-2.0+


#ifndef __LINUX_USB_NET2280_H
#define __LINUX_USB_NET2280_H






struct net2280_regs {
	
	u32		devinit;
#define     LOCAL_CLOCK_FREQUENCY                               8
#define     FORCE_PCI_RESET                                     7
#define     PCI_ID                                              6
#define     PCI_ENABLE                                          5
#define     FIFO_SOFT_RESET                                     4
#define     CFG_SOFT_RESET                                      3
#define     PCI_SOFT_RESET                                      2
#define     USB_SOFT_RESET                                      1
#define     M8051_RESET                                         0
	u32		eectl;
#define     EEPROM_ADDRESS_WIDTH                                23
#define     EEPROM_CHIP_SELECT_ACTIVE                           22
#define     EEPROM_PRESENT                                      21
#define     EEPROM_VALID                                        20
#define     EEPROM_BUSY                                         19
#define     EEPROM_CHIP_SELECT_ENABLE                           18
#define     EEPROM_BYTE_READ_START                              17
#define     EEPROM_BYTE_WRITE_START                             16
#define     EEPROM_READ_DATA                                    8
#define     EEPROM_WRITE_DATA                                   0
	u32		eeclkfreq;
	u32		_unused0;
	

	u32		pciirqenb0;		
#define     SETUP_PACKET_INTERRUPT_ENABLE                       7
#define     ENDPOINT_F_INTERRUPT_ENABLE                         6
#define     ENDPOINT_E_INTERRUPT_ENABLE                         5
#define     ENDPOINT_D_INTERRUPT_ENABLE                         4
#define     ENDPOINT_C_INTERRUPT_ENABLE                         3
#define     ENDPOINT_B_INTERRUPT_ENABLE                         2
#define     ENDPOINT_A_INTERRUPT_ENABLE                         1
#define     ENDPOINT_0_INTERRUPT_ENABLE                         0
	u32		pciirqenb1;
#define     PCI_INTERRUPT_ENABLE                                31
#define     POWER_STATE_CHANGE_INTERRUPT_ENABLE                 27
#define     PCI_ARBITER_TIMEOUT_INTERRUPT_ENABLE                26
#define     PCI_PARITY_ERROR_INTERRUPT_ENABLE                   25
#define     PCI_MASTER_ABORT_RECEIVED_INTERRUPT_ENABLE          20
#define     PCI_TARGET_ABORT_RECEIVED_INTERRUPT_ENABLE          19
#define     PCI_TARGET_ABORT_ASSERTED_INTERRUPT_ENABLE          18
#define     PCI_RETRY_ABORT_INTERRUPT_ENABLE                    17
#define     PCI_MASTER_CYCLE_DONE_INTERRUPT_ENABLE              16
#define     GPIO_INTERRUPT_ENABLE                               13
#define     DMA_D_INTERRUPT_ENABLE                              12
#define     DMA_C_INTERRUPT_ENABLE                              11
#define     DMA_B_INTERRUPT_ENABLE                              10
#define     DMA_A_INTERRUPT_ENABLE                              9
#define     EEPROM_DONE_INTERRUPT_ENABLE                        8
#define     VBUS_INTERRUPT_ENABLE                               7
#define     CONTROL_STATUS_INTERRUPT_ENABLE                     6
#define     ROOT_PORT_RESET_INTERRUPT_ENABLE                    4
#define     SUSPEND_REQUEST_INTERRUPT_ENABLE                    3
#define     SUSPEND_REQUEST_CHANGE_INTERRUPT_ENABLE             2
#define     RESUME_INTERRUPT_ENABLE                             1
#define     SOF_INTERRUPT_ENABLE                                0
	u32		cpu_irqenb0;		
#define     SETUP_PACKET_INTERRUPT_ENABLE                       7
#define     ENDPOINT_F_INTERRUPT_ENABLE                         6
#define     ENDPOINT_E_INTERRUPT_ENABLE                         5
#define     ENDPOINT_D_INTERRUPT_ENABLE                         4
#define     ENDPOINT_C_INTERRUPT_ENABLE                         3
#define     ENDPOINT_B_INTERRUPT_ENABLE                         2
#define     ENDPOINT_A_INTERRUPT_ENABLE                         1
#define     ENDPOINT_0_INTERRUPT_ENABLE                         0
	u32		cpu_irqenb1;
#define     CPU_INTERRUPT_ENABLE                                31
#define     POWER_STATE_CHANGE_INTERRUPT_ENABLE                 27
#define     PCI_ARBITER_TIMEOUT_INTERRUPT_ENABLE                26
#define     PCI_PARITY_ERROR_INTERRUPT_ENABLE                   25
#define     PCI_INTA_INTERRUPT_ENABLE                           24
#define     PCI_PME_INTERRUPT_ENABLE                            23
#define     PCI_SERR_INTERRUPT_ENABLE                           22
#define     PCI_PERR_INTERRUPT_ENABLE                           21
#define     PCI_MASTER_ABORT_RECEIVED_INTERRUPT_ENABLE          20
#define     PCI_TARGET_ABORT_RECEIVED_INTERRUPT_ENABLE          19
#define     PCI_RETRY_ABORT_INTERRUPT_ENABLE                    17
#define     PCI_MASTER_CYCLE_DONE_INTERRUPT_ENABLE              16
#define     GPIO_INTERRUPT_ENABLE                               13
#define     DMA_D_INTERRUPT_ENABLE                              12
#define     DMA_C_INTERRUPT_ENABLE                              11
#define     DMA_B_INTERRUPT_ENABLE                              10
#define     DMA_A_INTERRUPT_ENABLE                              9
#define     EEPROM_DONE_INTERRUPT_ENABLE                        8
#define     VBUS_INTERRUPT_ENABLE                               7
#define     CONTROL_STATUS_INTERRUPT_ENABLE                     6
#define     ROOT_PORT_RESET_INTERRUPT_ENABLE                    4
#define     SUSPEND_REQUEST_INTERRUPT_ENABLE                    3
#define     SUSPEND_REQUEST_CHANGE_INTERRUPT_ENABLE             2
#define     RESUME_INTERRUPT_ENABLE                             1
#define     SOF_INTERRUPT_ENABLE                                0

	
	u32		_unused1;
	u32		usbirqenb1;
#define     USB_INTERRUPT_ENABLE                                31
#define     POWER_STATE_CHANGE_INTERRUPT_ENABLE                 27
#define     PCI_ARBITER_TIMEOUT_INTERRUPT_ENABLE                26
#define     PCI_PARITY_ERROR_INTERRUPT_ENABLE                   25
#define     PCI_INTA_INTERRUPT_ENABLE                           24
#define     PCI_PME_INTERRUPT_ENABLE                            23
#define     PCI_SERR_INTERRUPT_ENABLE                           22
#define     PCI_PERR_INTERRUPT_ENABLE                           21
#define     PCI_MASTER_ABORT_RECEIVED_INTERRUPT_ENABLE          20
#define     PCI_TARGET_ABORT_RECEIVED_INTERRUPT_ENABLE          19
#define     PCI_RETRY_ABORT_INTERRUPT_ENABLE                    17
#define     PCI_MASTER_CYCLE_DONE_INTERRUPT_ENABLE              16
#define     GPIO_INTERRUPT_ENABLE                               13
#define     DMA_D_INTERRUPT_ENABLE                              12
#define     DMA_C_INTERRUPT_ENABLE                              11
#define     DMA_B_INTERRUPT_ENABLE                              10
#define     DMA_A_INTERRUPT_ENABLE                              9
#define     EEPROM_DONE_INTERRUPT_ENABLE                        8
#define     VBUS_INTERRUPT_ENABLE                               7
#define     CONTROL_STATUS_INTERRUPT_ENABLE                     6
#define     ROOT_PORT_RESET_INTERRUPT_ENABLE                    4
#define     SUSPEND_REQUEST_INTERRUPT_ENABLE                    3
#define     SUSPEND_REQUEST_CHANGE_INTERRUPT_ENABLE             2
#define     RESUME_INTERRUPT_ENABLE                             1
#define     SOF_INTERRUPT_ENABLE                                0
	u32		irqstat0;
#define     INTA_ASSERTED                                       12
#define     SETUP_PACKET_INTERRUPT                              7
#define     ENDPOINT_F_INTERRUPT                                6
#define     ENDPOINT_E_INTERRUPT                                5
#define     ENDPOINT_D_INTERRUPT                                4
#define     ENDPOINT_C_INTERRUPT                                3
#define     ENDPOINT_B_INTERRUPT                                2
#define     ENDPOINT_A_INTERRUPT                                1
#define     ENDPOINT_0_INTERRUPT                                0
#define     USB3380_IRQSTAT0_EP_INTR_MASK_IN (0xF << 17)
#define     USB3380_IRQSTAT0_EP_INTR_MASK_OUT (0xF << 1)

	u32		irqstat1;
#define     POWER_STATE_CHANGE_INTERRUPT                        27
#define     PCI_ARBITER_TIMEOUT_INTERRUPT                       26
#define     PCI_PARITY_ERROR_INTERRUPT                          25
#define     PCI_INTA_INTERRUPT                                  24
#define     PCI_PME_INTERRUPT                                   23
#define     PCI_SERR_INTERRUPT                                  22
#define     PCI_PERR_INTERRUPT                                  21
#define     PCI_MASTER_ABORT_RECEIVED_INTERRUPT                 20
#define     PCI_TARGET_ABORT_RECEIVED_INTERRUPT                 19
#define     PCI_RETRY_ABORT_INTERRUPT                           17
#define     PCI_MASTER_CYCLE_DONE_INTERRUPT                     16
#define     SOF_DOWN_INTERRUPT                                  14
#define     GPIO_INTERRUPT                                      13
#define     DMA_D_INTERRUPT                                     12
#define     DMA_C_INTERRUPT                                     11
#define     DMA_B_INTERRUPT                                     10
#define     DMA_A_INTERRUPT                                     9
#define     EEPROM_DONE_INTERRUPT                               8
#define     VBUS_INTERRUPT                                      7
#define     CONTROL_STATUS_INTERRUPT                            6
#define     ROOT_PORT_RESET_INTERRUPT                           4
#define     SUSPEND_REQUEST_INTERRUPT                           3
#define     SUSPEND_REQUEST_CHANGE_INTERRUPT                    2
#define     RESUME_INTERRUPT                                    1
#define     SOF_INTERRUPT                                       0
	
	u32		idxaddr;
	u32		idxdata;
	u32		fifoctl;
#define     PCI_BASE2_RANGE                                     16
#define     IGNORE_FIFO_AVAILABILITY                            3
#define     PCI_BASE2_SELECT                                    2
#define     FIFO_CONFIGURATION_SELECT                           0
	u32		_unused2;
	
	u32		memaddr;
#define     START                                               28
#define     DIRECTION                                           27
#define     FIFO_DIAGNOSTIC_SELECT                              24
#define     MEMORY_ADDRESS                                      0
	u32		memdata0;
	u32		memdata1;
	u32		_unused3;
	
	u32		gpioctl;
#define     GPIO3_LED_SELECT                                    12
#define     GPIO3_INTERRUPT_ENABLE                              11
#define     GPIO2_INTERRUPT_ENABLE                              10
#define     GPIO1_INTERRUPT_ENABLE                              9
#define     GPIO0_INTERRUPT_ENABLE                              8
#define     GPIO3_OUTPUT_ENABLE                                 7
#define     GPIO2_OUTPUT_ENABLE                                 6
#define     GPIO1_OUTPUT_ENABLE                                 5
#define     GPIO0_OUTPUT_ENABLE                                 4
#define     GPIO3_DATA                                          3
#define     GPIO2_DATA                                          2
#define     GPIO1_DATA                                          1
#define     GPIO0_DATA                                          0
	u32		gpiostat;
#define     GPIO3_INTERRUPT                                     3
#define     GPIO2_INTERRUPT                                     2
#define     GPIO1_INTERRUPT                                     1
#define     GPIO0_INTERRUPT                                     0
} __attribute__ ((packed));


struct net2280_usb_regs {
	
	u32		stdrsp;
#define     STALL_UNSUPPORTED_REQUESTS                          31
#define     SET_TEST_MODE                                       16
#define     GET_OTHER_SPEED_CONFIGURATION                       15
#define     GET_DEVICE_QUALIFIER                                14
#define     SET_ADDRESS                                         13
#define     ENDPOINT_SET_CLEAR_HALT                             12
#define     DEVICE_SET_CLEAR_DEVICE_REMOTE_WAKEUP               11
#define     GET_STRING_DESCRIPTOR_2                             10
#define     GET_STRING_DESCRIPTOR_1                             9
#define     GET_STRING_DESCRIPTOR_0                             8
#define     GET_SET_INTERFACE                                   6
#define     GET_SET_CONFIGURATION                               5
#define     GET_CONFIGURATION_DESCRIPTOR                        4
#define     GET_DEVICE_DESCRIPTOR                               3
#define     GET_ENDPOINT_STATUS                                 2
#define     GET_INTERFACE_STATUS                                1
#define     GET_DEVICE_STATUS                                   0
	u32		prodvendid;
#define     PRODUCT_ID                                          16
#define     VENDOR_ID                                           0
	u32		relnum;
	u32		usbctl;
#define     SERIAL_NUMBER_INDEX                                 16
#define     PRODUCT_ID_STRING_ENABLE                            13
#define     VENDOR_ID_STRING_ENABLE                             12
#define     USB_ROOT_PORT_WAKEUP_ENABLE                         11
#define     VBUS_PIN                                            10
#define     TIMED_DISCONNECT                                    9
#define     SUSPEND_IMMEDIATELY                                 7
#define     SELF_POWERED_USB_DEVICE                             6
#define     REMOTE_WAKEUP_SUPPORT                               5
#define     PME_POLARITY                                        4
#define     USB_DETECT_ENABLE                                   3
#define     PME_WAKEUP_ENABLE                                   2
#define     DEVICE_REMOTE_WAKEUP_ENABLE                         1
#define     SELF_POWERED_STATUS                                 0
	
	u32		usbstat;
#define     HIGH_SPEED                                          7
#define     FULL_SPEED                                          6
#define     GENERATE_RESUME                                     5
#define     GENERATE_DEVICE_REMOTE_WAKEUP                       4
	u32		xcvrdiag;
#define     FORCE_HIGH_SPEED_MODE                               31
#define     FORCE_FULL_SPEED_MODE                               30
#define     USB_TEST_MODE                                       24
#define     LINE_STATE                                          16
#define     TRANSCEIVER_OPERATION_MODE                          2
#define     TRANSCEIVER_SELECT                                  1
#define     TERMINATION_SELECT                                  0
	u32		setup0123;
	u32		setup4567;
	
	u32		_unused0;
	u32		ouraddr;
#define     FORCE_IMMEDIATE                                     7
#define     OUR_USB_ADDRESS                                     0
	u32		ourconfig;
} __attribute__ ((packed));


struct net2280_pci_regs {
	
	u32		 pcimstctl;
#define     PCI_ARBITER_PARK_SELECT                             13
#define     PCI_MULTI LEVEL_ARBITER                             12
#define     PCI_RETRY_ABORT_ENABLE                              11
#define     DMA_MEMORY_WRITE_AND_INVALIDATE_ENABLE              10
#define     DMA_READ_MULTIPLE_ENABLE                            9
#define     DMA_READ_LINE_ENABLE                                8
#define     PCI_MASTER_COMMAND_SELECT                           6
#define         MEM_READ_OR_WRITE                                   0
#define         IO_READ_OR_WRITE                                    1
#define         CFG_READ_OR_WRITE                                   2
#define     PCI_MASTER_START                                    5
#define     PCI_MASTER_READ_WRITE                               4
#define         PCI_MASTER_WRITE                                    0
#define         PCI_MASTER_READ                                     1
#define     PCI_MASTER_BYTE_WRITE_ENABLES                       0
	u32		 pcimstaddr;
	u32		 pcimstdata;
	u32		 pcimststat;
#define     PCI_ARBITER_CLEAR                                   2
#define     PCI_EXTERNAL_ARBITER                                1
#define     PCI_HOST_MODE                                       0
} __attribute__ ((packed));


struct net2280_dma_regs {	
	
	u32		dmactl;
#define     DMA_SCATTER_GATHER_DONE_INTERRUPT_ENABLE            25
#define     DMA_CLEAR_COUNT_ENABLE                              21
#define     DESCRIPTOR_POLLING_RATE                             19
#define         POLL_CONTINUOUS                                     0
#define         POLL_1_USEC                                         1
#define         POLL_100_USEC                                       2
#define         POLL_1_MSEC                                         3
#define     DMA_VALID_BIT_POLLING_ENABLE                        18
#define     DMA_VALID_BIT_ENABLE                                17
#define     DMA_SCATTER_GATHER_ENABLE                           16
#define     DMA_OUT_AUTO_START_ENABLE                           4
#define     DMA_PREEMPT_ENABLE                                  3
#define     DMA_FIFO_VALIDATE                                   2
#define     DMA_ENABLE                                          1
#define     DMA_ADDRESS_HOLD                                    0
	u32		dmastat;
#define     DMA_ABORT_DONE_INTERRUPT                            27
#define     DMA_SCATTER_GATHER_DONE_INTERRUPT                   25
#define     DMA_TRANSACTION_DONE_INTERRUPT                      24
#define     DMA_ABORT                                           1
#define     DMA_START                                           0
	u32		_unused0[2];
	
	u32		dmacount;
#define     VALID_BIT                                           31
#define     DMA_DIRECTION                                       30
#define     DMA_DONE_INTERRUPT_ENABLE                           29
#define     END_OF_CHAIN                                        28
#define         DMA_BYTE_COUNT_MASK                                 ((1<<24)-1)
#define     DMA_BYTE_COUNT                                      0
	u32		dmaaddr;
	u32		dmadesc;
	u32		_unused1;
} __attribute__ ((packed));



struct net2280_dep_regs {	
	
	u32		dep_cfg;
	
	u32		dep_rsp;
	u32		_unused[2];
} __attribute__ ((packed));


struct net2280_ep_regs {	
	
	u32		ep_cfg;
#define     ENDPOINT_BYTE_COUNT                                 16
#define     ENDPOINT_ENABLE                                     10
#define     ENDPOINT_TYPE                                       8
#define     ENDPOINT_DIRECTION                                  7
#define     ENDPOINT_NUMBER                                     0
	u32		ep_rsp;
#define     SET_NAK_OUT_PACKETS                                 15
#define     SET_EP_HIDE_STATUS_PHASE                            14
#define     SET_EP_FORCE_CRC_ERROR                              13
#define     SET_INTERRUPT_MODE                                  12
#define     SET_CONTROL_STATUS_PHASE_HANDSHAKE                  11
#define     SET_NAK_OUT_PACKETS_MODE                            10
#define     SET_ENDPOINT_TOGGLE                                 9
#define     SET_ENDPOINT_HALT                                   8
#define     CLEAR_NAK_OUT_PACKETS                               7
#define     CLEAR_EP_HIDE_STATUS_PHASE                          6
#define     CLEAR_EP_FORCE_CRC_ERROR                            5
#define     CLEAR_INTERRUPT_MODE                                4
#define     CLEAR_CONTROL_STATUS_PHASE_HANDSHAKE                3
#define     CLEAR_NAK_OUT_PACKETS_MODE                          2
#define     CLEAR_ENDPOINT_TOGGLE                               1
#define     CLEAR_ENDPOINT_HALT                                 0
	u32		ep_irqenb;
#define     SHORT_PACKET_OUT_DONE_INTERRUPT_ENABLE              6
#define     SHORT_PACKET_TRANSFERRED_INTERRUPT_ENABLE           5
#define     DATA_PACKET_RECEIVED_INTERRUPT_ENABLE               3
#define     DATA_PACKET_TRANSMITTED_INTERRUPT_ENABLE            2
#define     DATA_OUT_PING_TOKEN_INTERRUPT_ENABLE                1
#define     DATA_IN_TOKEN_INTERRUPT_ENABLE                      0
	u32		ep_stat;
#define     FIFO_VALID_COUNT                                    24
#define     HIGH_BANDWIDTH_OUT_TRANSACTION_PID                  22
#define     TIMEOUT                                             21
#define     USB_STALL_SENT                                      20
#define     USB_IN_NAK_SENT                                     19
#define     USB_IN_ACK_RCVD                                     18
#define     USB_OUT_PING_NAK_SENT                               17
#define     USB_OUT_ACK_SENT                                    16
#define     FIFO_OVERFLOW                                       13
#define     FIFO_UNDERFLOW                                      12
#define     FIFO_FULL                                           11
#define     FIFO_EMPTY                                          10
#define     FIFO_FLUSH                                          9
#define     SHORT_PACKET_OUT_DONE_INTERRUPT                     6
#define     SHORT_PACKET_TRANSFERRED_INTERRUPT                  5
#define     NAK_OUT_PACKETS                                     4
#define     DATA_PACKET_RECEIVED_INTERRUPT                      3
#define     DATA_PACKET_TRANSMITTED_INTERRUPT                   2
#define     DATA_OUT_PING_TOKEN_INTERRUPT                       1
#define     DATA_IN_TOKEN_INTERRUPT                             0
	
	u32		ep_avail;
	u32		ep_data;
	u32		_unused0[2];
} __attribute__ ((packed));

#endif 
