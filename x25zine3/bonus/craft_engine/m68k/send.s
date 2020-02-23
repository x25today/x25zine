/*
  CraftPack
  by madcr (m68k)
*/

.globl _start
_start:
.text


/* socket(PF_INET,SOCK_PACKET,htons(ETH_P_ALL); ----- */

        movl #0x2,%sp@             /* PF_INET           */
	movl #10,%sp@(0x4)       /* SOCK_PACKET       */
	movl #0x0003,%sp@(0x8)   /* htons(ETH_P_ALL)  */
	
	movel %sp,%d2    /* put addr from %sp in %d2 (argv) */
	    	
	movl #0x66,%d0   /* socketcall (asm/unistd.h) */
	movl #0x1,%d1      /* sys_socket (linux/net.h)  */
	trap #0          /* go on vector 80 */

/* save the descriptor ! */
        movl %d0,%a5 
cicle:

/* sendto(sock,buffer,sizeof(buffer),0,&address,16); ------ */ 

    /*    ip_source_generate_F #0xf  */


/*        ip_src_dev_random_generator_F         */
	ip_checksum_F
#        icmp_checksum_F  

        udp_checksum_F 
	
/*1*/   movl %a5,%sp@         /* descriptor on socket as first argv in stack */
/*2*/   movl #packet,%sp@(0x4) 
/*3*/   movl #size_packet,%sp@(0x8)    /* 14 ether,20 ip header,8 icmp */
/*4*/   movl #0,%sp@(0xc)    
/*5*/   movl #sockaddr,%sp@(0x10)      
/*6*/   movl #16,%sp@(0x14)   
                              
        movl %sp,%d2
        movl #0x66,%d0        /* socketcall (asm/unistd.h)   */
	movl #11,%d1          /* sys_sendto   (linux/net.h)  */
	trap #0               /* go on vector 80             */

/* ---- exit() ---- */

       movl %a5,%d0   /* back the desriptor ! ;) */
#       jmp cicle
       exit_F     

.data
sockaddr: .byte 0x00,0x02
          .string "eth0"
m1:  .asciz "/dev/random"
x: .long 0x00000000
rand: .long 0x00000001


  