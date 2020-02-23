;; nasm -f selfmade_lkm.s -o selfmade_lkm.o
;; insmod selfmade_lkm.o

;; --- elf header --- 
        	db 0x7F,"ELF",1,2,1  ;; ident
	   	db "x25zine3",0x0a   ;; os info
                db 0,1               ;; e_type = RELOCOTABLE
                db 0,4               ;; e_machine
                db 0,0,0,1           ;; e_version 
		db 0,0,0,0           ;; module not have entry_p
                db 0,0,0,0           ;; module not have prog_s
                db 0,0,0,0x53        ;; symbol headers sections offset
		db 0,0,0,0           ;; e_flags
                db 0,0x34            ;; elf header size		    
                db 0,0               ;; module not have e_phentsize
                db 0,0               ;; e_phnum
        
	        db 0,40              ;; soft header size (! must be 40 bytes !)
 		db 0,1               ;; how many headers
		db 0,0               ;; shstrndx 
 
;; --- code here ---
;; blablalbalba
;; --- end code ---
 
 

;; --- symbol table ---

                db "kernel_version=2.2.10",0        ;; =16h
                db ".modinfo",0                     ;; =9h

;; --- symbol section headers ---	
                
                db 0,0,0,0x16         ;; .modinfo (it mean after 0x16)
		db 0,0,0,0x1          ;; type of section = progbits
		db 0,0,0,0            ;; flags = 0
		db 0,0,0,0            ;; addr in memory = n/a
		db 0,0,0,0x34         ;; offset from file begining
		db 0,0,0,0x1f         ;; section size = 0x16 (22 bytes) 
                db 0,0,0,0            ;; not have header link table
                db 0,0,0,0            ;; 'extra' info
                db 0,0,0,0x1          ;; align constraints.
                db 0,0,0,0            ;; not have fixed-size table.

	