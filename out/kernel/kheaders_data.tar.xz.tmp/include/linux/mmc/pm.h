

#ifndef LINUX_MMC_PM_H
#define LINUX_MMC_PM_H



typedef unsigned int mmc_pm_flag_t;

#define MMC_PM_KEEP_POWER	(1 << 0)	
#define MMC_PM_WAKE_SDIO_IRQ	(1 << 1)	
#define MMC_PM_IGNORE_PM_NOTIFY	(1 << 2)	

#endif 
