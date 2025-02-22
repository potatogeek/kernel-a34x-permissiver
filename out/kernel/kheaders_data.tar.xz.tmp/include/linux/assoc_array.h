

#ifndef _LINUX_ASSOC_ARRAY_H
#define _LINUX_ASSOC_ARRAY_H

#ifdef CONFIG_ASSOCIATIVE_ARRAY

#include <linux/types.h>

#define ASSOC_ARRAY_KEY_CHUNK_SIZE BITS_PER_LONG 


struct assoc_array {
	struct assoc_array_ptr	*root;		
	unsigned long		nr_leaves_on_tree;
};


struct assoc_array_ops {
	
	unsigned long (*get_key_chunk)(const void *index_key, int level);

	
	unsigned long (*get_object_key_chunk)(const void *object, int level);

	
	bool (*compare_object)(const void *object, const void *index_key);

	
	int (*diff_objects)(const void *object, const void *index_key);

	
	void (*free_object)(void *object);
};


struct assoc_array_edit;

static inline void assoc_array_init(struct assoc_array *array)
{
	array->root = NULL;
	array->nr_leaves_on_tree = 0;
}

extern int assoc_array_iterate(const struct assoc_array *array,
			       int (*iterator)(const void *object,
					       void *iterator_data),
			       void *iterator_data);
extern void *assoc_array_find(const struct assoc_array *array,
			      const struct assoc_array_ops *ops,
			      const void *index_key);
extern void assoc_array_destroy(struct assoc_array *array,
				const struct assoc_array_ops *ops);
extern struct assoc_array_edit *assoc_array_insert(struct assoc_array *array,
						   const struct assoc_array_ops *ops,
						   const void *index_key,
						   void *object);
extern void assoc_array_insert_set_object(struct assoc_array_edit *edit,
					  void *object);
extern struct assoc_array_edit *assoc_array_delete(struct assoc_array *array,
						   const struct assoc_array_ops *ops,
						   const void *index_key);
extern struct assoc_array_edit *assoc_array_clear(struct assoc_array *array,
						  const struct assoc_array_ops *ops);
extern void assoc_array_apply_edit(struct assoc_array_edit *edit);
extern void assoc_array_cancel_edit(struct assoc_array_edit *edit);
extern int assoc_array_gc(struct assoc_array *array,
			  const struct assoc_array_ops *ops,
			  bool (*iterator)(void *object, void *iterator_data),
			  void *iterator_data);

#endif 
#endif 
