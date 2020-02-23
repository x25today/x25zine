#include <stdio.h>
#include <stdlib.h>

main()
{
  char code[]=

// file header

 "\x83\x01"                          // magic
 "\x01\x00"                          // 1 section
 "\xa8\x7d\x2c\x40"                  // time/date 
 "x25zine4"                          // for evil 
 "\x0d\x0a\x00\x00"                  // evil too
 "\x50\x00"                          // sizeof(optheader)
 "\x07\x00"                          // flags

// opt header

 "\x0b\x01"                          // magic   
 "\x0d\x03"                          // version
 "\x0e\x00"                          // build rev
 "\x00\x00"                          // paddcell
 "\x00\x20\x00\x00\x00\x00\x00\x00"  // .text size (fuck it)
 "\x00\x00\x00\x00\x00\x00\x00\x00"  // data size
 "\x00\x00\x00\x00\x00\x00\x00\x00"  // bss size
 "\xa8\x00\x00\x20\x01\x00\x00\x00"  // entry point (need to change)
 "\x00\x00\x00\x20\x01\x00\x00\x00"  // start
 "\x00\x00\x00\x40\x01\x00\x00\x00"  // .data start
 "\x00\x00\x00\x40\x01\x00\x00\x00"  // .bss start
 "\x00\x00\x00\x00\x00\x00\x00\x00"  // fp reg. mask
 "\xf0\x7f\x00\x40\x01\x00\x00\x00"  // gp.


// .text header

 "\x2e\x74\x65\x78\x74\x00\x00\x00"   // .text
 "\xa8\x00\x00\x20\x01\x00\x00\x00"   // physical address (need to change)
 "\xa8\x00\x00\x20\x01\x00\x00\x00"   // virtual address (need to change)
 "\x10\x00\x00\x00\x00\x00\x00\x00"   // section size
 "\xa8\x00\x00\x00\x00\x00\x00\x00"   // file ptr to raw data for section   
 "\x00\x00\x00\x00\x00\x00\x00\x00"   // file ptr to relocation
 "\x00\x00\x00\x00\x00\x00\x00\x00"   // special purpose
 "\x00\x00"                           // number of relocation entries 
 "\x01\x00"                           // power-of-two section alignment 
 "\x20\x00\x00\x00"                   // flags


// .text section

   "\x01\x00\x1f\x22" // 221f0001  lda      $16,1($31)

   "\x00\x48\x3f\x26" // 263f4800  ldah     $17, 18432
   "\x51\x04\x3f\x42" // 423f0451  s4addq   $17, $31, $17 # mov 0x120000000,$17
   "\x12\x14\xe1\x47" // 47e11412  bis      $31, 0x8, $18 # mov  0x8,$18
   "\x11\x04\x32\x42" // 42320411  addq     $17, $18, $17 # in $18 0x120000000a

   "\x0a\x00\x5f\x22" // 225f000a  lda      $18, 0xa($31) 
   "\x04\x00\x1f\x20" // 201f0004  lda      $0, 4($31)
   "\x83\x00\x00\x00" // 00000083  call_pal 0x83

   "\x01\x00\x1f\x20" // 201f0001  lda      $0, 1($31)
   "\x83\x00\x00";    // 00000083  call_pal 0x83
                      // (+ \x00 for last opcode autopad by char[]/fwrite())


 FILE *little_coff;
 if ((little_coff = fopen ("little_coff","wb")) == NULL)
 { printf ("fuck,can't create\n"); };
 fwrite(code,sizeof(code),1,little_coff);
 fclose (little_coff);
}

