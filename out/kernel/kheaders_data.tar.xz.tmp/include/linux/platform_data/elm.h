

#ifndef __ELM_H
#define __ELM_H

enum bch_ecc {
	BCH4_ECC = 0,
	BCH8_ECC,
	BCH16_ECC,
};


#define ERROR_VECTOR_MAX		8


struct elm_errorvec {
	bool error_reported;
	bool error_uncorrectable;
	int error_count;
	int error_loc[16];
};

#if IS_ENABLED(CONFIG_MTD_NAND_OMAP_BCH)
void elm_decode_bch_error_page(struct device *dev, u8 *ecc_calc,
		struct elm_errorvec *err_vec);
int elm_config(struct device *dev, enum bch_ecc bch_type,
	int ecc_steps, int ecc_step_size, int ecc_syndrome_size);
#else
static inline void
elm_decode_bch_error_page(struct device *dev, u8 *ecc_calc,
			  struct elm_errorvec *err_vec)
{
}

static inline int elm_config(struct device *dev, enum bch_ecc bch_type,
			     int ecc_steps, int ecc_step_size,
			     int ecc_syndrome_size)
{
	return -ENOSYS;
}
#endif 

#endif 
