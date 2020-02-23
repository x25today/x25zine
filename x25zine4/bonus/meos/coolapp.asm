; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;  Nice window (c) 2004 Mr.Clumsy
; -------------------------------------------------------------------------
;
; This routine was coded special for x25zine#04. Feel free to study it. ;)
;
; -------------------------------------------------------------------------

use32

    org    0x0

    db   'MENUET01'             ; 8 byte id
    dd   0x01                   ; header version
    dd   START                  ; start of code
    dd   I_END                  ; size of image
    dd   0x100000               ; memory for app
    dd   0x7fff0                ; esp
    dd   0x0 , 0x0              ; I_Param , I_Icon

START:                          ; start of execution

    call draw_window
    call get_process_info
still:

    mov  eax,10                 ; wait here for event
    int  0x40

    cmp  eax,1                  ; redraw request ?
    je   red
    cmp  eax,2                  ; key in buffer ?
    je   key
    cmp  eax,3                  ; button in buffer ?
    je   button

    jmp  still

  red:                          ; redraw
    call get_process_info
    call draw_window
    jmp  still

  key:                          ; key
    mov  eax,2                  ; just read it and ignore
    int  0x40
    jmp  still

  button:                       ; button
    mov  eax,17                 ; get id
    int  0x40

    cmp  ah,1                   ; button id=1 ?
    jne  noclose

    mov  eax,-1                 ; close this program
    int  0x40
  noclose:

    jmp  still


;   *********************************************
;   *******  WINDOW DEFINITIONS AND DRAW ********
;   *********************************************

draw_window:
    mov  eax,12                    ; function 12:tell os about windowdraw
    mov  ebx,1                     ; 1, start of draw
    int  0x40
                                   ; DRAW WINDOW
    mov  eax,0                     ; function 0 : define and draw window
    mov  ebx,100*65536             ; [x start] *65536
    add  ebx,[process_window_width] ; + window width
    mov  ecx,100*65536             ; [y start] *65536
    add  ecx,[process_window_height] ; + window height
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
    mov  ebx,[process_window_width] ; window width
    sub  ebx,19                    ; -19
    shl  ebx,16                    ; * 65536
    add  ebx,12                    ; + button width
    mov  ecx,5*65536+12            ; [y start] *65536 + [y size]
    mov  edx,1                     ; button id
    mov  esi,0x6688dd              ; button color RRGGBB
    int  0x40

    mov  ebx,20*65536+35           ; draw info text with function 4
    mov  ecx,0x00ff00              ; color of text
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

get_process_info:
    mov  eax,9
    mov  ebx,PInfo
    mov  ecx,-1
    int  0x40
    ret

; DATA AREA


text:
    db 'Another one example for x25zine#04.     '
    db 'This example shows how to create a      '
    db 'window with "smart" close button in case'
    db 'of resize event.                        '
    db 0

labelt:
     db   'EXAMPLE APPLICATION'
labellen:

PInfo:
proc_use:
    dd 0
stack_pos:
    dw 0
stack_value:
    dd 0
process_name:
    db 0,0,0,0,0,0,0,0,0,0,0,0
process_address:
    dd 0
used_memory:
    dd 0
process_id:
    dd 0
process_x_coord:
    dd 0
process_y_coord:
    dd 0
process_window_width:
    dd 320
process_window_height:
    dd 240
    dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

I_END:
