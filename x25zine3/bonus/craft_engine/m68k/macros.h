/* 
  ip_checksum_F - calculate ip checksum.
  input - no
*/
.macro ip_checksum_F
/* + ip header + data length calculate */ 
        movl #size_packet,%d0
	subl #14,%d0    
        move %d0,packet+16
/* real checksum calculate */
        eorl %d0,%d0
	movl %d0,%d1
        move packet+14,%d0
        move packet+16,%d1
	addl %d0,%d1
        move packet+18,%d0
	addl %d0,%d1
        move packet+20,%d0
	addl %d0,%d1
        move packet+22,%d0
	addl %d0,%d1
        move packet+26,%d0
	addl %d0,%d1
        move packet+28,%d0
	addl %d0,%d1
        move packet+30,%d0
	addl %d0,%d1
        move packet+32,%d0
        addl %d0,%d1           /* now %d1 have 16bit field summary. */
/* if 65535 > %d1, so 65535 - %d1 = checksum. */
        movel #65535,%d0
        cmpl %d1,%d0    
	bgt just_sub_ip    /* if %d0 > %d1 jmp on just_sub */
/* if 65535 < %d1, so %d1 - 65535 (in loop, for cmp on NEG result) */
agayn_ip:
        subl %d0,%d1        /* %d1 - %d0 */        
        bpl agayn_ip           /* if not neg, jmp agayn */
                            /* if neg - checksum already calculate */
        neg %d1             /* so, remove NEG */
        move %d1,packet+24  /* and paste result */
	jmp ok_ip
just_sub_ip:
        sub %d1,%d0       /* now %d0 have ip checksum! */
        move %d0,packet+24
ok_ip:
.endm

/*
 icmp checksum calculate 
*/ 
.macro icmp_checksum_F
        eorl %d0,%d0
        eorl %d1,%d1
        eorl %d2,%d2
        eorl %d3,%d3
	
        move packet+34,%d0
        move packet+38,%d1
	addl %d0,%d1
        move packet+40,%d0
	addl %d0,%d1               /*  %d1 have icmp header 16st summary */


/*---------------calculate 16st icmp data----------*/
        movl #size_packet,%d2 
        subl #42,%d2          /* in %d2 - size of udp data */
        cmpl #0,%d2           /* if %d0 = 0, sp no data - jmp on data_cicle */
        beq data_cicle_icmp 

	movl #2,%d3                /* /2 for 'how many 16st in data' */
        divsl %d3,%d2              /* in %d2 - how many '16st' in udp data */
                                   /* so, %d2 have - so many cicles need */

        eorl %d4,%d4  
	movl %d4,x     
aha:        
	movl x,%d0       			 
        addl #2,%d0            /* x+2 */  
        movl %d0,x             /* write data offset after ip/icmp header */
        addl #40,%d0           /* %d0 have real offset in icmp data */ 
	move packet(%d0),%d3    /* packet+offset,%d0 */
        addl %d3,%d4 

        subl #1,%d2 
        cmpl #0,%d2 
        bne aha       /* if %d2 = 0 - end cicle */

/* now, add %d1 (icmp header 16st sum) and %d4 (icmp data 16st sum) */
	addl %d4,%d1               /* %d1 have readly summary */
/*----------------=----=----------=---=-------------*/
/* if 65535 > %d1, so 65535 - %d1 = checksum. */
data_cicle_icmp:
        movel #65535,%d0 
        cmpl %d1,%d0    
	bgt just_sub_icmp    /* if %d0 > %d1 jmp on just_sub */
/* if 65535 < %d1, so %d1 - 65535 (in loop, for cmp on NEG result) */
agayn_icmp:
        subl %d0,%d1        /* %d1  - %d0 */        
        bpl agayn_icmp      /* if not neg, jmp agayn */
                            /* if neg - checksum already calculate */
        neg %d1             /* so, remove NEG */
        move %d1,packet+36  /* and paste result */
	jmp ok_icmp
just_sub_icmp:
        sub %d1,%d0       /* now %d0 have ip checksum! */
        move %d0,packet+36
ok_icmp:
.endm


/*
 udp checksum calculate ---------------------------------------------------
*/ 
.macro udp_checksum_F
udp:
        eorl %d0,%d0
        eorl %d1,%d1
        eorl %d2,%d2
        eorl %d3,%d3
/* total data udp header + length calculate */
        movl #size_packet,%d0
	subl #34,%d0    
        move %d0,packet+38     /* write */
/*  pseudo hdr */
        eorl %d0,%d0
        move packet+26,%d0     /*           */
	addl %d0,%d1           /* source ip */
        move packet+28,%d0     /*           */
      	addl %d0,%d1        
        move packet+30,%d0     /*           */
	addl %d0,%d1           /*  dest ip  */
        move packet+32,%d0     /*           */
        addl %d0,%d1        
        eorl %d0,%d0
        movb packet+23,%d0     /* 00+protocol */
        addl %d0,%d1        
        move packet+38,%d0     /* total length udp+data */
        addl %d0,%d1        
/* real hdr */
        move packet+34,%d0     /* source port */
        addl %d0,%d1        
        move packet+36,%d0     /* dest port */ 
	addl %d0,%d1
	move packet+38,%d0     /* agayn total length udp+data */
	addl %d0,%d1   

hmm:
	/* %d1 have pseudo/real udp 16st summary */
/* data */        
/*---------------calculate 16st udp data----------*/
        movl #size_packet,%d2
        subl #42,%d2          /* in %d2 - size of udp data */
        cmpl #0,%d2           /* if %d0 = 0, sp no data - jmp on data_cicle */
        beq data_cicle

	movl #2,%d3                /* /2 for 'how many 16st in data' */
        divsl %d3,%d2              /* in %d2 - how many '16st' in udp data */
                                   /* so, %d2 have - so many cicles need */
        eorl %d4,%d4
	movl %d4,x
aha_udp:
	movl x,%d0      			 
        addl #2,%d0            /* x+2 */  
        movl %d0,x             /* write data offset after ip/icmp header */
        addl #40,%d0           /* %d0 have real offset in icmp data */ 
	move packet(%d0),%d3   /* packet+offset,%d0 */
        addl %d3,%d4 

        subl #1,%d2
        cmpl #0,%d2
        bne aha_udp  /* if %d2 = 0 - end cicle */
/* now, add %d1 (udp header 16st sum) and %d4 (udp data 16st sum) */
	addl %d4,%d1             /* %d1 have really summary */
/*----------------=----=----------=---=-------------*/
/* if 65535 > %d1, so 65535 - %d1 = checksum. */

data_cicle:
        movel #65535,%d0 
        cmpl %d1,%d0    
	bgt just_sub_udp    /* if %d0 > %d1 jmp on just_sub */
/* if 65535 < %d1, so %d1 - 65535 (in loop, for cmp on NEG result) */
agayn_udp:
        subl %d0,%d1        /* %d1 - %d0 */        
        bpl agayn_udp      /* if not neg, jmp agayn */
                            /* if neg - checksum already calculate */
        neg %d1             /* so, remove NEG */
        move %d1,packet+40  /* and paste result */
	jmp ok_udp
just_sub_udp:
        sub %d1,%d0       /* now %d0 have ip checksum! */
        move %d0,packet+40
ok_udp:
.endm
        


/*
 ip source address random generator
*/ 
.macro ip_source_generate_F, mult
      movl \mult,%d1
      movl rand,%d0
      mulsl %d0,%d1  /* result in %d1 */ 
      negl %d1

      movel %d1,rand         /* result ! */
      movl rand,packet+26   /* write in source addr */

      movl %d1,%d0
      movl #0x010,%d1
      lsrl %d1,%d0 
.data
.endm

/*
  realy good ip_source generator 
*/  
.macro ip_src_dev_random_generator_F
        movl #5,%d0           /* open */
	movl #m1,%d1
	movl #0xc02,%d2
	trap #0
	
	movl %d0,%d1         /* descriptor in %d1 */
	movl #3,%d0          /* read */
	movl #packet+26,%d2
	movl #4,%d3
	trap #0
.endm

/* 
  print_F - write symbol to stdout.
  input - buffer,length
*/
.macro print_F, buffer,length
        movl #4,%d0           /* write  */
        movl #1,%d1           /* stdout */
	movl \buffer,%d2      /* buffer */
	movl \length,%d3      /* length */
	trap #0  
.endm

/*
  just exit_F from programm
  input - n/a
*/
.macro exit_F
        eorl %d3,%d3
        eorl %d2,%d2
        eorl %d1,%d1
        movl #1,%d0           /* exit */
        trap #0        
.endm
