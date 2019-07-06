    org 00100000h

    jmp start

_c dd ?

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
    sub esp,23

_main_t0 equ byte ptr [ebp-2]

_main_t1 equ byte ptr [ebp-4]

_main_t2 equ byte ptr [ebp-6]

_main_t3 equ dword ptr [ebp-14]

_main_t4 equ byte ptr [ebp-15]

_main_t5 equ dword ptr [ebp-19]

_main_t6 equ dword ptr [ebp-23]

_main_a equ byte ptr [ebp-1]

    mov al,97
    mov _main_a,al
    mov _main_t0,al

_main_b equ byte ptr [ebp-3]

    mov al,10
    mov _main_b,al
    mov _main_t1,al

_main_d equ byte ptr [ebp-5]

    mov al,100
    mov _main_d,al
    mov _main_t2,al

_main_e equ dword ptr [ebp-10]

    mov eax,0
    mov _main_e,eax
    mov _main_t3,eax

    mov al,_main_a
    push al

    mov al,_main_b
    push al

    call _max
    add esp,2
    mov _main_t4,al

    mov eax,_main_t4
    mov _c,eax
    mov _main_t5,eax

    mov eax,_main_d
    mov _main_e,eax
    mov _main_t6,eax

_main_0:
    mov esp,ebp
    pop ebp
    ret

_main endp

init proc

    mov eax,50
    mov _c,eax

    ret

init endp

start:
    mov esp,00200000h

    call init

    call _main

    hlt

