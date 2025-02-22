

#ifndef __XEN_PUBLIC_PHYSDEV_H__
#define __XEN_PUBLIC_PHYSDEV_H__




#define PHYSDEVOP_eoi			12
struct physdev_eoi {
	
	uint32_t irq;
};


#define PHYSDEVOP_pirq_eoi_gmfn_v1       17

#define PHYSDEVOP_pirq_eoi_gmfn_v2       28
struct physdev_pirq_eoi_gmfn {
    
    xen_ulong_t gmfn;
};


#define PHYSDEVOP_irq_status_query	 5
struct physdev_irq_status_query {
	
	uint32_t irq;
	
	uint32_t flags; 
};


#define _XENIRQSTAT_needs_eoi	(0)
#define	 XENIRQSTAT_needs_eoi	(1U<<_XENIRQSTAT_needs_eoi)


#define _XENIRQSTAT_shared	(1)
#define	 XENIRQSTAT_shared	(1U<<_XENIRQSTAT_shared)


#define PHYSDEVOP_set_iopl		 6
struct physdev_set_iopl {
	
	uint32_t iopl;
};


#define PHYSDEVOP_set_iobitmap		 7
struct physdev_set_iobitmap {
	
	uint8_t * bitmap;
	uint32_t nr_ports;
};


#define PHYSDEVOP_apic_read		 8
#define PHYSDEVOP_apic_write		 9
struct physdev_apic {
	
	unsigned long apic_physbase;
	uint32_t reg;
	
	uint32_t value;
};


#define PHYSDEVOP_alloc_irq_vector	10
#define PHYSDEVOP_free_irq_vector	11
struct physdev_irq {
	
	uint32_t irq;
	
	uint32_t vector;
};

#define MAP_PIRQ_TYPE_MSI		0x0
#define MAP_PIRQ_TYPE_GSI		0x1
#define MAP_PIRQ_TYPE_UNKNOWN		0x2
#define MAP_PIRQ_TYPE_MSI_SEG		0x3
#define MAP_PIRQ_TYPE_MULTI_MSI		0x4

#define PHYSDEVOP_map_pirq		13
struct physdev_map_pirq {
    domid_t domid;
    
    int type;
    
    int index;
    
    int pirq;
    
    int bus;
    
    int devfn;
    
    int entry_nr;
    
    uint64_t table_base;
};

#define PHYSDEVOP_unmap_pirq		14
struct physdev_unmap_pirq {
    domid_t domid;
    
    int pirq;
};

#define PHYSDEVOP_manage_pci_add	15
#define PHYSDEVOP_manage_pci_remove	16
struct physdev_manage_pci {
	
	uint8_t bus;
	uint8_t devfn;
};

#define PHYSDEVOP_restore_msi            19
struct physdev_restore_msi {
	
	uint8_t bus;
	uint8_t devfn;
};

#define PHYSDEVOP_manage_pci_add_ext	20
struct physdev_manage_pci_ext {
	
	uint8_t bus;
	uint8_t devfn;
	unsigned is_extfn;
	unsigned is_virtfn;
	struct {
		uint8_t bus;
		uint8_t devfn;
	} physfn;
};


struct physdev_op {
	uint32_t cmd;
	union {
		struct physdev_irq_status_query	     irq_status_query;
		struct physdev_set_iopl		     set_iopl;
		struct physdev_set_iobitmap	     set_iobitmap;
		struct physdev_apic		     apic_op;
		struct physdev_irq		     irq_op;
	} u;
};

#define PHYSDEVOP_setup_gsi    21
struct physdev_setup_gsi {
    int gsi;
    
    uint8_t triggering;
    
    uint8_t polarity;
    
};

#define PHYSDEVOP_get_nr_pirqs    22
struct physdev_nr_pirqs {
    
    uint32_t nr_pirqs;
};


#define PHYSDEVOP_get_free_pirq    23
struct physdev_get_free_pirq {
     
    int type;
    
    uint32_t pirq;
};

#define XEN_PCI_DEV_EXTFN              0x1
#define XEN_PCI_DEV_VIRTFN             0x2
#define XEN_PCI_DEV_PXM                0x4

#define XEN_PCI_MMCFG_RESERVED         0x1

#define PHYSDEVOP_pci_mmcfg_reserved    24
struct physdev_pci_mmcfg_reserved {
    uint64_t address;
    uint16_t segment;
    uint8_t start_bus;
    uint8_t end_bus;
    uint32_t flags;
};

#define PHYSDEVOP_pci_device_add        25
struct physdev_pci_device_add {
    
    uint16_t seg;
    uint8_t bus;
    uint8_t devfn;
    uint32_t flags;
    struct {
        uint8_t bus;
        uint8_t devfn;
    } physfn;
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    uint32_t optarr[];
#elif defined(__GNUC__)
    uint32_t optarr[0];
#endif
};

#define PHYSDEVOP_pci_device_remove     26
#define PHYSDEVOP_restore_msi_ext       27

#define PHYSDEVOP_prepare_msix          30
#define PHYSDEVOP_release_msix          31
struct physdev_pci_device {
    
    uint16_t seg;
    uint8_t bus;
    uint8_t devfn;
};

#define PHYSDEVOP_DBGP_RESET_PREPARE    1
#define PHYSDEVOP_DBGP_RESET_DONE       2

#define PHYSDEVOP_DBGP_BUS_UNKNOWN      0
#define PHYSDEVOP_DBGP_BUS_PCI          1

#define PHYSDEVOP_dbgp_op               29
struct physdev_dbgp_op {
    
    uint8_t op;
    uint8_t bus;
    union {
        struct physdev_pci_device pci;
    } u;
};


#define PHYSDEVOP_IRQ_UNMASK_NOTIFY	 4


#define PHYSDEVOP_IRQ_STATUS_QUERY	 PHYSDEVOP_irq_status_query
#define PHYSDEVOP_SET_IOPL		 PHYSDEVOP_set_iopl
#define PHYSDEVOP_SET_IOBITMAP		 PHYSDEVOP_set_iobitmap
#define PHYSDEVOP_APIC_READ		 PHYSDEVOP_apic_read
#define PHYSDEVOP_APIC_WRITE		 PHYSDEVOP_apic_write
#define PHYSDEVOP_ASSIGN_VECTOR		 PHYSDEVOP_alloc_irq_vector
#define PHYSDEVOP_FREE_VECTOR		 PHYSDEVOP_free_irq_vector
#define PHYSDEVOP_IRQ_NEEDS_UNMASK_NOTIFY XENIRQSTAT_needs_eoi
#define PHYSDEVOP_IRQ_SHARED		 XENIRQSTAT_shared

#endif 
