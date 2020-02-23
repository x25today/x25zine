; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;  The 3d stars (c) 2004 Mr.Clumsy
; -------------------------------------------------------------------------
;
; This routine was coded special for x25zine#04. Feel free to study it. ;)
;
; -------------------------------------------------------------------------

use32
        org   0x0

        db    'MENUET01'
        dd    0x01
        dd    START
        dd    I_END
        dd    0x100000
        dd    0x7ffff
        dd    0, 0

StarsNum      equ 200               ; amount of stars
Zmax          equ 256               ; max z-coord
win_width     equ 320               ; window width
win_height    equ 240               ; window height
ShadowWin     equ 0x90000           ; start address of shadow screen

START:

        call  Prepare_X             ; prepare x-coords of the stars
        call  Prepare_Y             ; do the same with y-coord
        call  Prepare_Z             ; and z-coords
        call  DrawWindow            ; draw application window
        push  ebx
MainLoop:
        pop   ebx

; drawing
        call  MoveStars             ; move stars (increase z-coords)
        call  ClearShadowWin        ; clear image in the shadow screen
        call  Draw3dStars           ; draw stars into the shadow screen
        call  DrawShadowWin         ; put shadow screen on application window
; end of draw

        push  ebx
        mov   eax,23                ; suspend process for 1/100
        mov   ebx,1
        int   0x40                  ; in eax returns type of event

        cmp   eax,1
        jne   NoRefresh
        call  DrawWindow
        jmp   MainLoop
NoRefresh:
        cmp   eax,0
        je    MainLoop
        mov   eax,-1
        int   0x40

DrawWindow:
        pusha
        mov   eax,12
        mov   ebx,1

        mov   eax,0
        mov   ebx,10*65536+win_width
        mov   ecx,10*65536+win_height
        mov   edx,0x02000000
        mov   esi,0x805080d0
        mov   edi,0x005080d0
        int   0x40

        mov   eax,4
        mov   ebx,8*65536+8
        mov   ecx,0x00ddeeff
        mov   edx,win_capt
        mov   esi,win_captlen-win_capt
        int   0x40

        mov   eax,8
        mov   ebx,(win_width-19)*65536+12
        mov   ecx,5*65536+12
        mov   edx,1
        mov   esi,0x6688dd
        int   0x40

        mov   eax,12
        mov   ebx,2
        int   0x40

        popa
        ret

DrawShadowWin:
        pusha
        mov   eax,7
        mov   ebx,ShadowWin
        mov   ecx,(win_width-8)*65536+(win_height-24)
        mov   edx,4*65536+20
        int   0x40

        popa
        ret

ClearShadowWin:
        pusha
        mov   edi,ShadowWin
        mov   ecx,win_width*win_height*3
        shr   ecx,2
        xor   eax,eax
        rep   stosd
        popa
        ret

Draw3dStars:
;
;
;                 (X(i) * Zmax)                          (Y(i) * Zmax)
;  X' = Xcenter + -------------    and    Y' = Ycenter + -------------
;                  (Zmax-Z(i))                            (Zmax-Z(i))
;
;  ,where Zmax=256 - think yourself why it's so.. ;)

        pusha
        xor   ebp,ebp
@@NextStar:
        mov   eax,[Y_coordDim+ebp]
        movsx dx,ah
        shl   eax,8
        mov   ecx,Zmax
        sub   ecx,[Z_coordDim+ebp]
        idiv  cx
        add   eax,[Ycenter]
        cmp   eax,win_height-24
        ja    @@NotSet

        mov   edi,eax
        mov   eax,[X_coordDim+ebp]
        movsx dx,ah
        shl   eax,8
        idiv  cx
        add   eax,[Xcenter]
        cmp   eax,win_width-8
        ja    @@NotSet

        pusha
        mov   ebx,eax
        mov   eax,edi
        mov   ecx,(win_width-8)*3
        mul   ecx
        add   eax,ebx               ; because image are used 3 bytes per pixel,
        add   eax,ebx               ; we should multiply width and height on 3
        add   eax,ebx               ;
        mov   edi,ShadowWin
        add   edi,eax
        mov   eax,[Z_coordDim+ebp]
        mov   byte [edi],al
        mov   byte [edi+1],al
        mov   byte [edi+2],al
        popa
@@NotSet:
        add   ebp,4
        cmp   ebp,StarsNum*4
        jb    @@NextStar
        popa
        ret

MoveStars:
        pusha
        xor   ebp,ebp
@@Moving:
        mov   eax,[Z_coordDim+ebp]
        cmp   eax,255
        jb    @@NotEnd
        mov   eax,1
        jmp   @@Load
@@NotEnd:
        inc   eax
@@Load: mov   [Z_coordDim+ebp],eax
        add   ebp,4
        cmp   ebp,StarsNum*4
        jb    @@Moving
        popa
        ret

Prepare_X:
        pusha
        xor   ebp,ebp
@@PrepareX:
        mov   eax,280
        call  Random
        sub   eax,140
        mov   [X_coordDim+ebp],eax
        add   ebp,4
        cmp   ebp,StarsNum*4
        jb    @@PrepareX
        popa
        ret

Prepare_Y:
        pusha
        xor   ebp,ebp
@@PrepareY:
        mov   eax,200
        call  Random
        sub   eax,100
        mov   [Y_coordDim+ebp],eax
        add   ebp,4
        cmp   ebp,StarsNum*4
        jb    @@PrepareY
        popa
        ret

Prepare_Z:
        pusha
        xor   ebp,ebp
@@PrepareZ:
        mov   eax,250
        call  Random
        or    eax,1
        mov   [Z_coordDim+ebp],eax
        add   ebp,4
        cmp   ebp,StarsNum*4
        jb    @@PrepareZ
        popa
        ret

; In:  AX - Range
; Out: AX - Value within 0 through AX-1
Random:
        push  ebx
        push  ecx
        push  esi
        push  edi
        mov   ecx,eax          ; save limit
        mov   eax,0
        mov   ebx,0
        mov   ax,word [RandSeed+2]
        mov   bx,word [RandSeed]
        mov   esi,eax
        mov   edi,ebx
        mov   dl,ah
        mov   ah,al
        mov   al,bh
        mov   bh,bl
        xor   bl,bl
        rcr   dl,1
        rcr   ax,1
        rcr   bx,1
        add   bx,di
        adc   ax,si
        add   bx,62e9h
        adc   ax,3619h
        mov   word [RandSeed],bx
        mov   word [RandSeed+2],ax
        xor   edx,edx
        div   cx
        mov   eax,edx
        pop   edi
        pop   esi
        pop   ecx
        pop   ebx
        ret

win_capt      db '3D stars for x25zine#04',0
win_captlen:

Xcenter       dd (win_width-8)/2
Ycenter       dd (win_height-24)/2
SinusPointer  dw 0
RandSeed      dd 0
X_coordDim    dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

Y_coordDim    dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

Z_coordDim    dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
              dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
I_END: