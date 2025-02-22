// SPDX-License-Identifier: GPL-2.0
/*
 * bpf-prologue.c
 *
 * Copyright (C) 2015 He Kuang <hekuang@huawei.com>
 * Copyright (C) 2015 Wang Nan <wangnan0@huawei.com>
 * Copyright (C) 2015 Huawei Inc.
 */

#include <bpf/libbpf.h>
<<<<<<< HEAD
#include "perf.h"
=======
>>>>>>> upstream/android-13
#include "debug.h"
#include "bpf-loader.h"
#include "bpf-prologue.h"
#include "probe-finder.h"
#include <errno.h>
<<<<<<< HEAD
=======
#include <stdlib.h>
>>>>>>> upstream/android-13
#include <dwarf-regs.h>
#include <linux/filter.h>

#define BPF_REG_SIZE		8

#define JMP_TO_ERROR_CODE	-1
#define JMP_TO_SUCCESS_CODE	-2
#define JMP_TO_USER_CODE	-3

struct bpf_insn_pos {
	struct bpf_insn *begin;
	struct bpf_insn *end;
	struct bpf_insn *pos;
};

static inline int
pos_get_cnt(struct bpf_insn_pos *pos)
{
	return pos->pos - pos->begin;
}

static int
append_insn(struct bpf_insn new_insn, struct bpf_insn_pos *pos)
{
	if (!pos->pos)
		return -BPF_LOADER_ERRNO__PROLOGUE2BIG;

	if (pos->pos + 1 >= pos->end) {
		pr_err("bpf prologue: prologue too long\n");
		pos->pos = NULL;
		return -BPF_LOADER_ERRNO__PROLOGUE2BIG;
	}

	*(pos->pos)++ = new_insn;
	return 0;
}

static int
check_pos(struct bpf_insn_pos *pos)
{
	if (!pos->pos || pos->pos >= pos->end)
		return -BPF_LOADER_ERRNO__PROLOGUE2BIG;
	return 0;
}

/*
 * Convert type string (u8/u16/u32/u64/s8/s16/s32/s64 ..., see
 * Documentation/trace/kprobetrace.rst) to size field of BPF_LDX_MEM
 * instruction (BPF_{B,H,W,DW}).
 */
static int
argtype_to_ldx_size(const char *type)
{
	int arg_size = type ? atoi(&type[1]) : 64;

	switch (arg_size) {
	case 8:
		return BPF_B;
	case 16:
		return BPF_H;
	case 32:
		return BPF_W;
	case 64:
	default:
		return BPF_DW;
	}
}

static const char *
insn_sz_to_str(int insn_sz)
{
	switch (insn_sz) {
	case BPF_B:
		return "BPF_B";
	case BPF_H:
		return "BPF_H";
	case BPF_W:
		return "BPF_W";
	case BPF_DW:
		return "BPF_DW";
	default:
		return "UNKNOWN";
	}
}

/* Give it a shorter name */
#define ins(i, p) append_insn((i), (p))

/*
 * Give a register name (in 'reg'), generate instruction to
 * load register into an eBPF register rd:
 *   'ldd target_reg, offset(ctx_reg)', where:
 * ctx_reg is pre initialized to pointer of 'struct pt_regs'.
 */
static int
gen_ldx_reg_from_ctx(struct bpf_insn_pos *pos, int ctx_reg,
		     const char *reg, int target_reg)
{
	int offset = regs_query_register_offset(reg);

	if (offset < 0) {
		pr_err("bpf: prologue: failed to get register %s\n",
		       reg);
		return offset;
	}
	ins(BPF_LDX_MEM(BPF_DW, target_reg, ctx_reg, offset), pos);

	return check_pos(pos);
}

/*
 * Generate a BPF_FUNC_probe_read function call.
 *
 * src_base_addr_reg is a register holding base address,
 * dst_addr_reg is a register holding dest address (on stack),
 * result is:
 *
 *  *[dst_addr_reg] = *([src_base_addr_reg] + offset)
 *
 * Arguments of BPF_FUNC_probe_read:
 *     ARG1: ptr to stack (dest)
 *     ARG2: size (8)
 *     ARG3: unsafe ptr (src)
 */
static int
gen_read_mem(struct bpf_insn_pos *pos,
	     int src_base_addr_reg,
	     int dst_addr_reg,
<<<<<<< HEAD
	     long offset)
=======
	     long offset,
	     int probeid)
>>>>>>> upstream/android-13
{
	/* mov arg3, src_base_addr_reg */
	if (src_base_addr_reg != BPF_REG_ARG3)
		ins(BPF_MOV64_REG(BPF_REG_ARG3, src_base_addr_reg), pos);
	/* add arg3, #offset */
	if (offset)
		ins(BPF_ALU64_IMM(BPF_ADD, BPF_REG_ARG3, offset), pos);

	/* mov arg2, #reg_size */
	ins(BPF_ALU64_IMM(BPF_MOV, BPF_REG_ARG2, BPF_REG_SIZE), pos);

	/* mov arg1, dst_addr_reg */
	if (dst_addr_reg != BPF_REG_ARG1)
		ins(BPF_MOV64_REG(BPF_REG_ARG1, dst_addr_reg), pos);

	/* Call probe_read  */
<<<<<<< HEAD
	ins(BPF_EMIT_CALL(BPF_FUNC_probe_read), pos);
=======
	ins(BPF_EMIT_CALL(probeid), pos);
>>>>>>> upstream/android-13
	/*
	 * Error processing: if read fail, goto error code,
	 * will be relocated. Target should be the start of
	 * error processing code.
	 */
	ins(BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, JMP_TO_ERROR_CODE),
	    pos);

	return check_pos(pos);
}

/*
 * Each arg should be bare register. Fetch and save them into argument
 * registers (r3 - r5).
 *
 * BPF_REG_1 should have been initialized with pointer to
 * 'struct pt_regs'.
 */
static int
gen_prologue_fastpath(struct bpf_insn_pos *pos,
		      struct probe_trace_arg *args, int nargs)
{
	int i, err = 0;

	for (i = 0; i < nargs; i++) {
		err = gen_ldx_reg_from_ctx(pos, BPF_REG_1, args[i].value,
					   BPF_PROLOGUE_START_ARG_REG + i);
		if (err)
			goto errout;
	}

	return check_pos(pos);
errout:
	return err;
}

/*
 * Slow path:
 *   At least one argument has the form of 'offset($rx)'.
 *
 * Following code first stores them into stack, then loads all of then
 * to r2 - r5.
 * Before final loading, the final result should be:
 *
 * low address
 * BPF_REG_FP - 24  ARG3
 * BPF_REG_FP - 16  ARG2
 * BPF_REG_FP - 8   ARG1
 * BPF_REG_FP
 * high address
 *
 * For each argument (described as: offn(...off2(off1(reg)))),
 * generates following code:
 *
 *  r7 <- fp
 *  r7 <- r7 - stack_offset  // Ideal code should initialize r7 using
 *                           // fp before generating args. However,
 *                           // eBPF won't regard r7 as stack pointer
 *                           // if it is generated by minus 8 from
 *                           // another stack pointer except fp.
 *                           // This is why we have to set r7
 *                           // to fp for each variable.
 *  r3 <- value of 'reg'-> generated using gen_ldx_reg_from_ctx()
 *  (r7) <- r3       // skip following instructions for bare reg
 *  r3 <- r3 + off1  . // skip if off1 == 0
 *  r2 <- 8           \
 *  r1 <- r7           |-> generated by gen_read_mem()
 *  call probe_read    /
 *  jnei r0, 0, err  ./
 *  r3 <- (r7)
 *  r3 <- r3 + off2  . // skip if off2 == 0
 *  r2 <- 8           \  // r2 may be broken by probe_read, so set again
 *  r1 <- r7           |-> generated by gen_read_mem()
 *  call probe_read    /
 *  jnei r0, 0, err  ./
 *  ...
 */
static int
gen_prologue_slowpath(struct bpf_insn_pos *pos,
		      struct probe_trace_arg *args, int nargs)
{
<<<<<<< HEAD
	int err, i;
=======
	int err, i, probeid;
>>>>>>> upstream/android-13

	for (i = 0; i < nargs; i++) {
		struct probe_trace_arg *arg = &args[i];
		const char *reg = arg->value;
		struct probe_trace_arg_ref *ref = NULL;
		int stack_offset = (i + 1) * -8;

		pr_debug("prologue: fetch arg %d, base reg is %s\n",
			 i, reg);

		/* value of base register is stored into ARG3 */
		err = gen_ldx_reg_from_ctx(pos, BPF_REG_CTX, reg,
					   BPF_REG_ARG3);
		if (err) {
			pr_err("prologue: failed to get offset of register %s\n",
			       reg);
			goto errout;
		}

		/* Make r7 the stack pointer. */
		ins(BPF_MOV64_REG(BPF_REG_7, BPF_REG_FP), pos);
		/* r7 += -8 */
		ins(BPF_ALU64_IMM(BPF_ADD, BPF_REG_7, stack_offset), pos);
		/*
		 * Store r3 (base register) onto stack
		 * Ensure fp[offset] is set.
		 * fp is the only valid base register when storing
		 * into stack. We are not allowed to use r7 as base
		 * register here.
		 */
		ins(BPF_STX_MEM(BPF_DW, BPF_REG_FP, BPF_REG_ARG3,
				stack_offset), pos);

		ref = arg->ref;
<<<<<<< HEAD
		while (ref) {
			pr_debug("prologue: arg %d: offset %ld\n",
				 i, ref->offset);
			err = gen_read_mem(pos, BPF_REG_3, BPF_REG_7,
					   ref->offset);
=======
		probeid = BPF_FUNC_probe_read_kernel;
		while (ref) {
			pr_debug("prologue: arg %d: offset %ld\n",
				 i, ref->offset);

			if (ref->user_access)
				probeid = BPF_FUNC_probe_read_user;

			err = gen_read_mem(pos, BPF_REG_3, BPF_REG_7,
					   ref->offset, probeid);
>>>>>>> upstream/android-13
			if (err) {
				pr_err("prologue: failed to generate probe_read function call\n");
				goto errout;
			}

			ref = ref->next;
			/*
			 * Load previous result into ARG3. Use
			 * BPF_REG_FP instead of r7 because verifier
			 * allows FP based addressing only.
			 */
			if (ref)
				ins(BPF_LDX_MEM(BPF_DW, BPF_REG_ARG3,
						BPF_REG_FP, stack_offset), pos);
		}
	}

	/* Final pass: read to registers */
	for (i = 0; i < nargs; i++) {
		int insn_sz = (args[i].ref) ? argtype_to_ldx_size(args[i].type) : BPF_DW;

		pr_debug("prologue: load arg %d, insn_sz is %s\n",
			 i, insn_sz_to_str(insn_sz));
		ins(BPF_LDX_MEM(insn_sz, BPF_PROLOGUE_START_ARG_REG + i,
				BPF_REG_FP, -BPF_REG_SIZE * (i + 1)), pos);
	}

	ins(BPF_JMP_IMM(BPF_JA, BPF_REG_0, 0, JMP_TO_SUCCESS_CODE), pos);

	return check_pos(pos);
errout:
	return err;
}

static int
prologue_relocate(struct bpf_insn_pos *pos, struct bpf_insn *error_code,
		  struct bpf_insn *success_code, struct bpf_insn *user_code)
{
	struct bpf_insn *insn;

	if (check_pos(pos))
		return -BPF_LOADER_ERRNO__PROLOGUE2BIG;

	for (insn = pos->begin; insn < pos->pos; insn++) {
		struct bpf_insn *target;
		u8 class = BPF_CLASS(insn->code);
		u8 opcode;

		if (class != BPF_JMP)
			continue;
		opcode = BPF_OP(insn->code);
		if (opcode == BPF_CALL)
			continue;

		switch (insn->off) {
		case JMP_TO_ERROR_CODE:
			target = error_code;
			break;
		case JMP_TO_SUCCESS_CODE:
			target = success_code;
			break;
		case JMP_TO_USER_CODE:
			target = user_code;
			break;
		default:
			pr_err("bpf prologue: internal error: relocation failed\n");
			return -BPF_LOADER_ERRNO__PROLOGUE;
		}

		insn->off = target - (insn + 1);
	}
	return 0;
}

int bpf__gen_prologue(struct probe_trace_arg *args, int nargs,
		      struct bpf_insn *new_prog, size_t *new_cnt,
		      size_t cnt_space)
{
	struct bpf_insn *success_code = NULL;
	struct bpf_insn *error_code = NULL;
	struct bpf_insn *user_code = NULL;
	struct bpf_insn_pos pos;
	bool fastpath = true;
	int err = 0, i;

	if (!new_prog || !new_cnt)
		return -EINVAL;

	if (cnt_space > BPF_MAXINSNS)
		cnt_space = BPF_MAXINSNS;

	pos.begin = new_prog;
	pos.end = new_prog + cnt_space;
	pos.pos = new_prog;

	if (!nargs) {
		ins(BPF_ALU64_IMM(BPF_MOV, BPF_PROLOGUE_FETCH_RESULT_REG, 0),
		    &pos);

		if (check_pos(&pos))
			goto errout;

		*new_cnt = pos_get_cnt(&pos);
		return 0;
	}

	if (nargs > BPF_PROLOGUE_MAX_ARGS) {
		pr_warning("bpf: prologue: %d arguments are dropped\n",
			   nargs - BPF_PROLOGUE_MAX_ARGS);
		nargs = BPF_PROLOGUE_MAX_ARGS;
	}

	/* First pass: validation */
	for (i = 0; i < nargs; i++) {
		struct probe_trace_arg_ref *ref = args[i].ref;

		if (args[i].value[0] == '@') {
			/* TODO: fetch global variable */
			pr_err("bpf: prologue: global %s%+ld not support\n",
				args[i].value, ref ? ref->offset : 0);
			return -ENOTSUP;
		}

		while (ref) {
			/* fastpath is true if all args has ref == NULL */
			fastpath = false;

			/*
			 * Instruction encodes immediate value using
			 * s32, ref->offset is long. On systems which
			 * can't fill long in s32, refuse to process if
			 * ref->offset too large (or small).
			 */
#ifdef __LP64__
#define OFFSET_MAX	((1LL << 31) - 1)
#define OFFSET_MIN	((1LL << 31) * -1)
			if (ref->offset > OFFSET_MAX ||
					ref->offset < OFFSET_MIN) {
				pr_err("bpf: prologue: offset out of bound: %ld\n",
				       ref->offset);
				return -BPF_LOADER_ERRNO__PROLOGUEOOB;
			}
#endif
			ref = ref->next;
		}
	}
	pr_debug("prologue: pass validation\n");

	if (fastpath) {
		/* If all variables are registers... */
		pr_debug("prologue: fast path\n");
		err = gen_prologue_fastpath(&pos, args, nargs);
		if (err)
			goto errout;
	} else {
		pr_debug("prologue: slow path\n");

		/* Initialization: move ctx to a callee saved register. */
		ins(BPF_MOV64_REG(BPF_REG_CTX, BPF_REG_ARG1), &pos);

		err = gen_prologue_slowpath(&pos, args, nargs);
		if (err)
			goto errout;
		/*
		 * start of ERROR_CODE (only slow pass needs error code)
		 *   mov r2 <- 1  // r2 is error number
		 *   mov r3 <- 0  // r3, r4... should be touched or
		 *                // verifier would complain
		 *   mov r4 <- 0
		 *   ...
		 *   goto usercode
		 */
		error_code = pos.pos;
		ins(BPF_ALU64_IMM(BPF_MOV, BPF_PROLOGUE_FETCH_RESULT_REG, 1),
		    &pos);

		for (i = 0; i < nargs; i++)
			ins(BPF_ALU64_IMM(BPF_MOV,
					  BPF_PROLOGUE_START_ARG_REG + i,
					  0),
			    &pos);
		ins(BPF_JMP_IMM(BPF_JA, BPF_REG_0, 0, JMP_TO_USER_CODE),
				&pos);
	}

	/*
	 * start of SUCCESS_CODE:
	 *   mov r2 <- 0
	 *   goto usercode  // skip
	 */
	success_code = pos.pos;
	ins(BPF_ALU64_IMM(BPF_MOV, BPF_PROLOGUE_FETCH_RESULT_REG, 0), &pos);

	/*
	 * start of USER_CODE:
	 *   Restore ctx to r1
	 */
	user_code = pos.pos;
	if (!fastpath) {
		/*
		 * Only slow path needs restoring of ctx. In fast path,
		 * register are loaded directly from r1.
		 */
		ins(BPF_MOV64_REG(BPF_REG_ARG1, BPF_REG_CTX), &pos);
		err = prologue_relocate(&pos, error_code, success_code,
					user_code);
		if (err)
			goto errout;
	}

	err = check_pos(&pos);
	if (err)
		goto errout;

	*new_cnt = pos_get_cnt(&pos);
	return 0;
errout:
	return err;
}
