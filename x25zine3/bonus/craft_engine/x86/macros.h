/* 
  ip_checksum_F - calculate ip checksum.
  input - no input
*/
.macro ip_checksum_F
        xorl %eax,%eax
	movl %eax,%ebx
        movw packet+14,%eax
        movw packet+16,%ebx
	addl %eax,%ebx
        movw packet+18,%eax
	addl %eax,%ebx
        movw packet+20,%eax
	addl %eax,%ebx
        movw packet+22,%eax
	addl %eax,%ebx
        movw packet+26,%eax
	addl %eax,%ebx
        movw packet+28,%eax
	addl %eax,%ebx
        movw packet+30,%eax
	addl %eax,%ebx
        movw packet+32,%eax
	addl %eax,%ebx           /* now %ebx have 16bit field summary. */

/* if 65535 > %d1, so 65535 - %d1 = checksum. */
        movl $65535,%eax
        cmpl %ebx,%eax    
	jg just_sub              /* if %eax > %ebx jmp on just_sub */
/* if 65535 < %d1, so %d1 - 65535 (in loop, for cmp on NEG result) */
agayn:
        subl %eax,%ebx           /* %ebx - %eax */        
        jns agayn                /* if not neg, jmp agayn */
                                 /* if neg - checksum already calculate */
        neg %ebx                 /* so, remove NEG */
        movw %ebx,packet+24      /* and paste result */
	jmp ok

just_sub:
        subl %ebx,%eax           /* now %eax have ip checksum! */
        movw %eax,packet+24
ok:
.endm

/*
 ip source address random generator
*/ 
.macro ip_source_generate_F, mult
      movl \mult,%d1
      movl rand,%d0
      mulsl %d0,%d1         /* result in %d1 */ 
      negl %d1

      movel %d1,rand        /* result ! */
      movl rand,packet+26   /* write in source addr */

      movl %d1,%d0
      movl #0x010,%d1
      lsrl %d1,%d0 
.data
rand: .long 0x00000001 
.endm

/* 
  print_F - write symbol to stdout.
  input - buffer,length
*/
.macro print_F, buffer,length
        movl $4,%eax           /* write  */
        movl $1,%ebx           /* stdout */
	movl \buffer,%ecx      /* buffer */
	movl \length,%edx      /* length */
	int  $128  
.endm

/*
  just exit_F from programm
  input - no input
*/
.macro exit_F
        xorl %edx,%edx
        xorl %ecx,%ecx
        xorl %ebx,%ebx
        movl $1,%eax           /* exit */
        int  $128        
.endm
