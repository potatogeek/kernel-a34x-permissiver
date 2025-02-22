/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */





#ifndef LINUX_ATM_ENI_H
#define LINUX_ATM_ENI_H

#include <linux/atmioc.h>


struct eni_multipliers {
	int tx,rx;	
};


#define ENI_MEMDUMP     _IOW('a',ATMIOC_SARPRV,struct atmif_sioc)
                                                
#define ENI_SETMULT	_IOW('a',ATMIOC_SARPRV+7,struct atmif_sioc)
						

#endif
