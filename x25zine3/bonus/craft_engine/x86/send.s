/*
  CraftPack
  by madcr (x86)
*/

.globl _start
_start:
.text

/* socket(PF_INET,SOCK_PACKET,htons(ETH_P_ALL); ----- */

        movl $0x2,(%esp)         # PF_INET          
	movl $10,0x4(%esp)       # SOCK_PACKET      
	movl $0x0300,0x8(%esp)   # htons(ETH_P_ALL) 
	
	movl %esp,%ecx           # put addr from %sp in %d2 (argv)
	    	
	movl $0x66,%eax          # socketcall (asm/unistd.h)
	movl $0x1,%ebx           # sys_socket (linux/net.h) 
	int  $128                # go on vector 80

        movl %eax,%edx           # save the descriptor ! ;)
cicle:

/* sendto(sock,buffer,sizeof(buffer),0,&address,16); ------ */ 

/*1*/   movl %eax,(%esp)          # descriptor on socket as first argv in stack
        ip_checksum_F

/*2*/   movl $packet,0x4(%esp) 
/*3*/   movl $size_packet,0x8(%esp)   
/*4*/   movl $0,0xc(%esp)    
/*5*/   movl $sockaddr,0x10(%esp)      
/*6*/   movl $16,0x14(%esp)   
                                  # %sp already in %ecx from first syscall using
        movl $0x66,%eax           # socketcall (asm/unistd.h)
	movl $11,%ebx             # sys_sendto (linux/net.h)  
	int  $128                 # go on vector 80             

        movl %edx,%eax            # back the desriptor ! ;)
        print_F $aga,$9
#        jmp cicle  


        exit_F    
.data
sockaddr: .byte 0x00,0x02
          .string "eth0"

aga: .ascii "huyarim!\n"

