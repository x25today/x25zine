;; nasm -f bin selfmade_elf.s

;; --- elf header ---

	   db 0x7f,"ELF",1,2,1             ;; magic
           db "x25zine3",0x0a              ;; os info
           db 0,2                          ;; e_type (for executable=2)
           db 0,2                          ;; e_machine (2 = sparc)
           db 0,0,0,1                      ;; version (always must be in 1)
           db 0x00,0x01,0x00,0x4c          ;; entry_point - 0x1004a
	   db 0,0,0,0x30                   ;; programm section offset
           dd 0                            ;; symbol section offset (not need) 
           dd 0                            ;; e_flags (not need)  
           db 0,0x30                       ;; elf header size
           db 0,0x1c 		           ;; size any programm section
           db 0,1                          ;; how many programm section
           db 0,0                          ;; e_shoff must be for sparc/sunos 

;; --- programm section header ---

           db 0,0,0,1                      ;; LOAD
           dd 0                            ;; offset from begin
           db 0x00,0x01,0x00,0x00          ;; in memory at 0x10000
           db "evil"                       ;; haha 
           db 0,0,0,0x6c                   ;; section size in file
           db 0,0,0,0x6c                   ;; section size in memory
           db 0,0,0,5                      ;; read & executable

;; --- real code ---

           db 0x82,0x10,0x20,0x04   ;; mov  4, %g1
           db 0x90,0x10,0x20,0x01   ;; mov  1, %o0
           db 0x13,0x00,0x00,0x40   ;; sethi  %hi(0x10000), %o1
     	   db 0x92,0x12,0x60,0x07   ;; or  %o1, 7, %o1 ! 10007 <*ABS*+0x10007>
           db 0x94,0x10,0x20,0x09   ;; mov  9, %o2
           db 0x91,0xd0,0x20,0x08   ;; ta  8

           db 0x82,0x10,0x20,0x01   ;; mov  1, %g1
           db 0x91,0xd0,0x20,0x08   ;; ta  8
