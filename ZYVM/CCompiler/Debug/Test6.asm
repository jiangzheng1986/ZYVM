    org 00100000h

    jmp start

_main proc

    push ebp
    mov ebp,esp
    sub esp,65

_main_t0 equ dword ptr [ebp-8]

_main_t1 equ dword ptr [ebp-16]

_main_t2 equ dword ptr [ebp-20]

_main_t3 equ dword ptr [ebp-24]

_main_t4 equ dword ptr [ebp-28]

_main_t5 equ dword ptr [ebp-32]

_main_t6 equ dword ptr [ebp-36]

_main_t7 equ dword ptr [ebp-40]

_main_t8 equ dword ptr [ebp-44]

_main_t9 equ dword ptr [ebp-48]

_main_t10 equ dword ptr [ebp-52]

_main_t11 equ dword ptr [ebp-56]

_main_t12 equ dword ptr [ebp-60]

_main_t13 equ dword ptr [ebp-64]

_main_t14 equ byte ptr [ebp-65]

_main_i equ dword ptr [ebp-4]

    mov eax,0
    mov _main_i,eax
    mov _main_t0,eax

_main_c equ dword ptr [ebp-12]

    mov eax,2
    mov _main_c,eax
    mov _main_t1,eax

_main_1:

    mov al,1
    cbw
    mov _main_t2,eax

    mov eax,_main_i
    mov ebx,_main_t2
    add eax,ebx
    mov _main_t3,eax

    mov eax,_main_t3
    mov _main_i,eax
    mov _main_t4,eax

    mov eax,_main_i
    mov _main_t5,eax
    inc eax
    mov _main_i,eax

    mov eax,_main_i
    inc eax
    mov _main_t6,eax

    mov al,1
    cbw
    mov _main_t7,eax

    mov eax,_main_i
    mov ebx,_main_t7
    sub eax,ebx
    mov _main_t8,eax

    mov eax,_main_t8
    mov _main_i,eax
    mov _main_t9,eax

    mov eax,_main_i
    mov _main_t10,eax
    dec eax
    mov _main_i,eax

    mov eax,_main_i
    dec eax
    mov _main_t11,eax

    mov eax,_main_i
    neg eax
    mov _main_t12,eax

    mov eax,_main_t12
    mov _main_i,eax
    mov _main_t13,eax

    mov eax,_main_i
    mov ebx,_main_c
    cmp eax,ebx
    jl _main_3
    mov al,0
    jmp short _main_4
_main_3:
    mov al,1
_main_4:
    mov _main_t14,al

    mov al,_main_t14
    cmp al,0
    je _main_2
    jmp _main_1

_main_2:

_main_0:
    mov esp,ebp
    pop ebp
    ret

_main endp

init proc

    ret

init endp

start:
    mov esp,00200000h

    call init

    call _main

    hlt

