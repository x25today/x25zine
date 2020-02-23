;; nasm -f bin selfmade_lkm.s -o selfmade_lkm.o
;; insmod selfmade_lkm.o

;; --- elf header ---

	   db 0x7f,"ELF",1,1,1     ;; magic line
           db "x25zine#3"          ;; os info
           db 1,0                  ;; relocatable
           db 3,0                  ;; intel 386
           db 1,0,0,0              ;; version
           db 0,0,0,0              ;; entry_point _not need for lkm_
	   db 0,0,0,0              ;; programm section _not need for lkm_
           db 0x55,0,0,0           ;; offset on header symbol sections
           db 0,0,0,0              ;; e_flags
           db 0x34,0               ;; elf header size
           db 0,0 		   ;; programm section _not need for lkm_
           db 0,0                  ;; programm section _not need for lkm_

           db 40,0                 ;; soft header size (!! must be 40 bytes !!)
	   db 1,0                  ;; how many headers
	   db 0,0                  ;; e_shstrndx. 


;; --- code here ---
;; blablabla             ;; super lkm code ;)
;; --- end code ---


;; --- symbol table ---

           db "kernel_version=2.4.18-3",0   ;; =18h   
	   db ".modinfo",0                  ;; =9h 
	   
;; --- symbol section header ---

           db 0x18,0,0,0           ;; sh_name (.modinfo)
	   db 0x1,0,0,0            ;; type - progbits
	   db 0,0,0,0              ;; not need flags
	   db 0,0,0,0              ;; not need address
	   db 0x34,0,0,0           ;; offset from file bigining on data
	   db 0x21,0,0,0           ;; size. 18h+9h=21h 
           db 0,0,0,0              ;; not have header link table
           db 0,0,0,0              ;; 'extra' info
           db 0x1,0,0,0            ;; align constraints.
           db 0,0,0,0              ;; not have fixed-size table.
