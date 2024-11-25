/* SPDX-License-Identifier: GPL-2.0 */
#include <as-layout.h>

.section .__syscall_stub, "ax"

	.globl batch_syscall_stub
batch_syscall_stub:
<<<<<<< HEAD
	/* load pointer to first operation */
	mov	$(STUB_DATA+8), %esp

=======
	/* %esp comes in as "top of page" */
	mov %esp, %ecx
	/* %esp has pointer to first operation */
	add $8, %esp
>>>>>>> upstream/android-13
again:
	/* load length of additional data */
	mov	0x0(%esp), %eax

	/* if(length == 0) : end of list */
	/* write possible 0 to header */
<<<<<<< HEAD
	mov	%eax, STUB_DATA+4
=======
	mov	%eax, 0x4(%ecx)
>>>>>>> upstream/android-13
	cmpl	$0, %eax
	jz	done

	/* save current pointer */
<<<<<<< HEAD
	mov	%esp, STUB_DATA+4
=======
	mov	%esp, 0x4(%ecx)
>>>>>>> upstream/android-13

	/* skip additional data */
	add	%eax, %esp

	/* load syscall-# */
	pop	%eax

	/* load syscall params */
	pop	%ebx
	pop	%ecx
	pop	%edx
	pop	%esi
 	pop	%edi
	pop	%ebp

	/* execute syscall */
	int	$0x80

<<<<<<< HEAD
=======
	/* restore top of page pointer in %ecx */
	mov	%esp, %ecx
	andl	$(~UM_KERN_PAGE_SIZE) + 1, %ecx

>>>>>>> upstream/android-13
	/* check return value */
	pop	%ebx
	cmp	%ebx, %eax
	je	again

done:
	/* save return value */
<<<<<<< HEAD
	mov	%eax, STUB_DATA
=======
	mov	%eax, (%ecx)
>>>>>>> upstream/android-13

	/* stop */
	int3
