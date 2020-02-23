;; nasm -f bin selfmade_elf.s

;; --- elf header --- 
        	db 0x7F,"ELF",1,2,1                 ;; ident
	   	db "x25zine3",0x0a                  ;; os info
                db 0,2                              ;; e_type
                db 0,4                              ;; e_machine
                db 0,0,0,1                          ;; e_version 
		db 0x80,0,0,0x4a                    ;; entry point
                db 0,0,0,0x2e                       ;; programm sections offset
                dd 0                                ;; symbol sections offset
		dd 0                                ;; e_flags
                db 0,0x2e                           ;; elf header size		    
                db 0,0x1c                           ;; e_phentsize
                db 0,1                              ;; e_phnum
                
;; --- programm section header ---
                db 0,0,0,1                          ;; LOAD
  		dd 0                                ;; offset from begin
		db 0x80,0,0,0                       ;; in memory at 80000000  
		db "evil"                           ;; only for history ;)
		db 0,0,0,0x5c                       ;; section size in file
		db 0,0,0,0x5c                       ;; section size in memory
                db 0,0,0,5                          ;; read & executable

;; --- real code ---
                db 0x70,0x04                        ;; movl #4,%d0
                db 0x72,0x01                        ;; movl #1,%d1 
                db 0x24,0x3c,0x80,0,0,0x07          ;; movel #80000007,%d2
                db 0x76,0x09                        ;; movl #9,%d3
                db 0x4e,0x40                        ;; trap #0    

                db 0x70,0x01                        ;; movl #1,%d0
                db 0x4e,0x40                        ;; trap #0
