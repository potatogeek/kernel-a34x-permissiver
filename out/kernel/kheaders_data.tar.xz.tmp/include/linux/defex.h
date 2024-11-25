

#ifndef __CONFIG_SECURITY_DEFEX_H
#define __CONFIG_SECURITY_DEFEX_H


int task_defex_enforce(struct task_struct *p, struct file *f, int syscall, ...);
int task_defex_zero_creds(struct task_struct *tsk);
asmlinkage int defex_syscall_enter(long int syscallno, struct pt_regs *regs);
int task_defex_user_exec(const char *new_file);
void __init defex_load_rules(void);
#endif 
