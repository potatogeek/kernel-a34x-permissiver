/* SPDX-License-Identifier: GPL-2.0 */
void convert_to_tag_list(struct tag *tags);

#ifdef CONFIG_ATAGS
<<<<<<< HEAD
const struct machine_desc *setup_machine_tags(phys_addr_t __atags_pointer,
	unsigned int machine_nr);
#else
static inline const struct machine_desc *
setup_machine_tags(phys_addr_t __atags_pointer, unsigned int machine_nr)
=======
const struct machine_desc *setup_machine_tags(void *__atags_vaddr,
	unsigned int machine_nr);
#else
static inline const struct machine_desc * __init __noreturn
setup_machine_tags(void *__atags_vaddr, unsigned int machine_nr)
>>>>>>> upstream/android-13
{
	early_print("no ATAGS support: can't continue\n");
	while (true);
	unreachable();
}
#endif
