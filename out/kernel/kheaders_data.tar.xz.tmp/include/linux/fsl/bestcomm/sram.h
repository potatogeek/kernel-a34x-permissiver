

#ifndef __BESTCOMM_SRAM_H__
#define __BESTCOMM_SRAM_H__

#include <asm/rheap.h>
#include <asm/mmu.h>
#include <linux/spinlock.h>



	
struct bcom_sram {
	phys_addr_t		 base_phys;
	void 			*base_virt;
	unsigned int		 size;
	rh_info_t		*rh;
	spinlock_t		 lock;
};

extern struct bcom_sram *bcom_sram;



extern int  bcom_sram_init(struct device_node *sram_node, char *owner);
extern void bcom_sram_cleanup(void);

extern void* bcom_sram_alloc(int size, int align, phys_addr_t *phys);
extern void  bcom_sram_free(void *ptr);

static inline phys_addr_t bcom_sram_va2pa(void *va) {
	return bcom_sram->base_phys +
		(unsigned long)(va - bcom_sram->base_virt);
}

static inline void *bcom_sram_pa2va(phys_addr_t pa) {
	return bcom_sram->base_virt +
		(unsigned long)(pa - bcom_sram->base_phys);
}


#endif  

