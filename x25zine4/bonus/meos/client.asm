; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;  Client application (c) 2004 Mr.Clumsy
; -------------------------------------------------------------------------
;
; This routine was coded special for x25zine#04. Feel free to study it. ;)
;
; -------------------------------------------------------------------------

use32

    org  0x0

    db   'MENUET01'              ; 8 byte id
    dd   0x01                    ; header version
    dd   START                   ; start of code
    dd   I_END                   ; size of image
    dd   0x100000                ; memory for app
    dd   0x7fff0                 ; esp
    dd   0x0 , 0x0               ; I_Param , I_Icon

START:                           ; start of execution

    call init_client
    cmp  eax,0
    jne  quit
    call draw_window

still:

;no_data:

    mov  eax,10                 ; wait here for event
    int  0x40

    cmp  eax,1                  ; redraw request ?
    je   red
    cmp  eax,2                  ; key in buffer ?
    je   key
    cmp  eax,3                  ; button in buffer ?
    je   button

    jmp  still

red:                            ; redraw
    call draw_window
    jmp  still

key:                            ; key pressed
    mov  eax,2                  ; just read it
    int  0x40
    cmp  al,0
    jne  still

    mov  ebp,[text_pointer]

    mov  byte [text+ebp],ah

    mov  eax,53                 ; write symbol to socket
    mov  ebx,7
    mov  ecx,[socket]
    mov  edx,1
    mov  esi,text
    add  esi,ebp
    int  0x40

    inc  ebp
    cmp  ebp,160
    jne  not_full

    mov  ebp,0
not_full:
    mov  [text_pointer],ebp

    cmp  eax,0
    je   red
    mov  byte [text+ebp-1],'!'
    jmp  red

button:                         ; button
    mov  eax,17                 ; get id
    int  0x40

    cmp  ah,1                   ; button id=1 ?

    jne  noclose
    call shutdown_client

quit:
    mov  eax,-1                 ; close this program
    int  0x40

noclose:
    jmp  still


init_client:
    mov  eax,52                 ;
    mov  ebx,1                  ; get localhost ip
    int  0x40
    cmp  eax,0
    jne  loc_ip_ok
    mov  eax,1
    ret

loc_ip_ok:
    mov  [remote_ip],eax

    mov  eax,53    ; check local port
    mov  ebx,9     ; if it's busy already - bad :)
    mov  ecx,[local_port]
    int  0x40
    cmp  eax,1
    je   port_ok
    ret

port_ok:
    mov  eax,53    ; open tcp socket
    mov  ebx,5     ;
    mov  ecx,[local_port]  ; 3000th port
    mov  edx,[remote_port]    ; remote port 83
    mov  esi,[remote_ip]    ; remote ip
    mov  edi,1     ; mode: SOCKET_PASSIVE
    int  0x40
    cmp  eax,0xFFFFFFFF
    jne  socket_ok
    mov  eax,2
    ret

socket_ok:
    mov  [socket],eax
    mov  eax,0
    ret

shutdown_client:
    mov  eax,53          ; close socket
    mov  ebx,8
    mov  ecx,[socket]
    int  0x40
    ret



;   *********************************************
;   *******  WINDOW DEFINITIONS AND DRAW ********
;   *********************************************

draw_window:

    mov  eax,12                    ; function 12:tell os about windowdraw
    mov  ebx,1                     ; 1, start of draw
    int  0x40

                                   ; DRAW WINDOW
    mov  eax,0                     ; function 0 : define and draw window
    mov  ebx,100*65536+300         ; [x start] *65536 + [x size]
    mov  ecx,100*65536+120         ; [y start] *65536 + [y size]
    mov  edx,0x02000000            ; color of work area RRGGBB,8->color gl
    mov  esi,0x805080d0            ; color of grab bar  RRGGBB,8->color gl
    mov  edi,0x005080d0            ; color of frames    RRGGBB
    int  0x40

                                   ; WINDOW LABEL
    mov  eax,4                     ; function 4 : write text to window
    mov  ebx,8*65536+8             ; [x start] *65536 + [y start]
    mov  ecx,0x00ddeeff            ; font 1 & color ( 0xF0RRGGBB )
    mov  edx,labelt                ; pointer to text beginning
    mov  esi,labellen-labelt       ; text length
    int  0x40

                                   ; CLOSE BUTTON
    mov  eax,8                     ; function 8 : define and draw button
    mov  ebx,(300-19)*65536+12     ; [x start] *65536 + [x size]
    mov  ecx,5*65536+12            ; [y start] *65536 + [y size]
    mov  edx,1                     ; button id
    mov  esi,0x6688dd              ; button color RRGGBB
    int  0x40

    mov  ebx,20*65536+35           ; draw info text with function 4
    mov  ecx,0x00ff00
    mov  edx,text
    mov  esi,40

newline:
    mov  eax,4
    int  0x40
    add  ebx,10
    add  edx,40
    cmp  [edx],byte 0
    jne  newline

    mov  eax,12                    ; function 12:tell os about windowdraw
    mov  ebx,2                     ; 2, end of draw
    int  0x40

    ret


; DATA AREA

socket      dd 0
remote_ip   dd 0
remote_port dd 83
local_port  dd 3000

text_pointer dd 0

text:
    db '                                        '
    db '                                        '
    db '                                        '
    db '                                        '

    db 0

labelt:
     db   'Client application'
labellen:

I_END: