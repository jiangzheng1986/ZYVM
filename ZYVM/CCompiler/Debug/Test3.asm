    org 00100000h

    jmp start

_c db ?

_max proc

    push ebp
    mov ebp,esp
    sub esp,1

_max_a equ byte ptr [ebp+9]

_max_b equ byte ptr [ebp+8]

_max_t0 equ byte ptr [ebp-1]

    mov al,_max_a
    mov bl,_max_b
    cmp al,bl
    jg _max_1
    mov al,0
    jmp short _max_2
_max_1:
    mov al,1
_max_2:
    mov _max_t0,al

    mov al,_max_t0
    cmp al,0
    jne _max_3
    jmp _max_4
_max_3:
    mov al,_max_a
    jmp _max_0

    jmp _max_5
_max_4:
    mov al,_max_b
    jmp _max_0

_max_5:

_max_0:
    mov esp,ebp
    pop ebp
    ret

_max endp

_main proc

    push ebp
    mov ebp,esp
    sub esp,6

_main_t0 equ byte ptr [ebp-2]

_main_t1 equ byte ptr [ebp-4]

_main_t2 equ byte ptr [ebp-5]

_main_t3 equ byte ptr [ebp-6]

_main_a equ byte ptr [ebp-1]

    mov al,97
    mov _main_a,al
    mov _main_t0,al

_main_b equ byte ptr [ebp-3]

    mov al,10
    mov _main_b,al
    mov _main_t1,al

    mov al,_main_a
    push al

    mov al,_main_b
    push al

    call _max
    add esp,2
    mov _main_t2,al

    mov al,_main_t2
    mov _c,al
    mov _main_t3,al

_main_0:
    mov esp,ebp
    pop ebp
    ret

_main endp

init proc

    mov al,50
    mov _c,al

    ret

init endp

start:
    mov esp,00200000h

    call init

    call _main

    hlt

