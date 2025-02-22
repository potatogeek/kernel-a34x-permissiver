

#ifndef _LINUX_ELF_FDPIC_H
#define _LINUX_ELF_FDPIC_H

#include <uapi/linux/elf-fdpic.h>


struct elf_fdpic_params {
	struct elfhdr			hdr;		
	struct elf_phdr			*phdrs;		
	struct elf32_fdpic_loadmap	*loadmap;	
	unsigned long			elfhdr_addr;	
	unsigned long			ph_addr;	
	unsigned long			map_addr;	
	unsigned long			entry_addr;	
	unsigned long			stack_size;	
	unsigned long			dynamic_addr;	
	unsigned long			load_addr;	
	unsigned long			flags;
#define ELF_FDPIC_FLAG_ARRANGEMENT	0x0000000f	
#define ELF_FDPIC_FLAG_INDEPENDENT	0x00000000	
#define ELF_FDPIC_FLAG_HONOURVADDR	0x00000001	
#define ELF_FDPIC_FLAG_CONSTDISP	0x00000002	
#define ELF_FDPIC_FLAG_CONTIGUOUS	0x00000003	
#define ELF_FDPIC_FLAG_EXEC_STACK	0x00000010	
#define ELF_FDPIC_FLAG_NOEXEC_STACK	0x00000020	
#define ELF_FDPIC_FLAG_EXECUTABLE	0x00000040	
#define ELF_FDPIC_FLAG_PRESENT		0x80000000	
};

#ifdef CONFIG_MMU
extern void elf_fdpic_arch_lay_out_mm(struct elf_fdpic_params *exec_params,
				      struct elf_fdpic_params *interp_params,
				      unsigned long *start_stack,
				      unsigned long *start_brk);
#endif

#endif 
