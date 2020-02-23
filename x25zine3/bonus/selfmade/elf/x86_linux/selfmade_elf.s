; nasm -f bin selfmade_elf.s

;; --- elf header ---
	   db 0x7f,"ELF",1,1,1      	 ;; magic
           db "x25zine3",0x0a       	 ;; os info
           db 2,0                   	 ;; e_type
           db 3,0                   	 ;; e_machine Intel 80386
           db 1,0,0,0                    ;; version. must be 1   
           db 0x4e,0x80,0x04,0x08   	 ;; entry_point: 0x0804804e
	   db 0x2e,0,0,0            	 ;; programm section offset
           dd 0                     	 ;; symbol section offset 
           db 0x08,0,0,0            	 ;; e_flags
           db 0x2e,0                	 ;; elf header size
           db 0x20,0                	 ;; e_phentsize
           db 1,0                   	 ;; e_phnum

;; --- programm section header ---
           db 1,0,0,0               	 ;; LOAD
           dd 0                     	 ;; offset from begin
           db 0x00,0x80,0x04,0x08   	 ;; in memory at: x08048000
           db "evil"                	 ;; only for history ;)
           db 0x72,0,0,0            	 ;; section size in file  
           db 0x72,0,0,0            	 ;; section size in memory
	   db 5,0,0,0               	 ;; read & executable
           dd 0                     	 ;; align

;--- real code
           db 0x66,0xb8,0x04,0x00        ;;  mov ax,4
	   db 0x66,0xbb,0x01,0x00        ;;  mov bx,1   
	   db 0xb9,0x07,0x80,0x04,0x08   ;;  mov ecx,8048007h 
	   db 0x66,0xba,0x09,0x00        ;;  mov dx,9
	   db 0xcd,0x80                  ;;  int 80h
	   
	   db 0x66,0xb8,0x01,0x00        ;;  mov ax,1
	   db 0xcd,0x80                  ;;  int 80h     	   