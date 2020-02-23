; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;  Server application (c) 2004 Mr.Clumsy
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

START:                          ; start of execution

    call init_server
    cmp  eax,0
    jne  quit

    call draw_window

    mov  ebp,0
still:
    mov  eax,53                 ; poll socket
    mov  ebx,2
    mov  ecx,[socket]
    int  0x40
    cmp  eax,0
    je   no_data

    mov  eax,53                ; read from socket
    mov  ebx,3
    mov  ecx,[socket]
    int  0x40
    mov  byte [text+ebp],bl
    inc  ebp
    cmp  ebp,160
    jne  not_zero
    mov  ebp,0
not_zero:

    call draw_window

    jmp  still

no_data:

    mov  eax,23                 ; wait here for event
    mov  ebx,10
    int  0x40

    cmp  eax,1                  ; redraw request ?
    jne  no_redraw

    call draw_window

    jmp  still

no_redraw:
    cmp  eax,0
    je   still

    call shutdown_server

quit:
    mov  eax,-1                 ; close this program
    int  0x40


init_server:
    mov  eax,52    ; get localhost ip
    mov  ebx,1
    int  0x40
    cmp  eax,0
    jne  loc_ip_ok
    mov  eax,1
    ret

loc_ip_ok:
    mov  [local_ip],eax
    mov  eax,53              ; check 83rd port
    mov  ebx,9               ; if it's busy already - bad :)
    mov  ecx,[local_port]
    int  0x40
    cmp  eax,1
    je   port_ok
    ret

port_ok:
    mov  eax,53            ; open tcp socket
    mov  ebx,5               
    mov  ecx,[local_port]  ; 83rd port
    mov  edx,0          ; remote port 0, because it's not necessary for us now
    mov  esi,0          ; remote ip address also are not necessary now
    mov  edi,0          ; mode: SOCKET_PASSIVE
    int  0x40
    cmp  eax,0xFFFFFFFF
    jne  socket_ok
    mov  eax,2
    ret

socket_ok:
    mov  [socket],eax
    mov  eax,0
    ret

shutdown_server:
    mov  eax,53          ; close socket
    mov  ebx,8
    mov  ecx,[socket]
    int  0x40
    ret


;   *********************************************
;   *******  WINDOW DEFINITIONS AND DRAW ********
;   *********************************************

draw_window:
    pusha

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
    mov  ecx,0x10ddeeff            ; font 1 & color ( 0xF0RRGGBB )
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

    mov  eax,47
    mov  ebx,0x00080100
    mov  ecx,[local_ip]
    mov  edx,20*65536+24
    mov  esi,0x00ff00
    int  0x40

    mov  ebx,20*65536+35           ; draw info text with function 4
    mov  ecx,0x0000ff00
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

    popa

    ret


; DATA AREA

socket     dd 0
local_port dd 83
remote_ip  dd 0
local_ip   dd 0

text_pointer dd 0

text:
    db '                                        '
    db '                                        '
    db '                                        '
    db '                                        '

    db 0

labelt:
     db   'EXAMPLE APPLICATION'
labellen:

I_END: