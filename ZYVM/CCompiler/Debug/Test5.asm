    org 00100000h

    jmp start

_main proc

    push ebp
    mov ebp,esp
    sub esp,29

_main_t0 equ dword ptr [ebp-8]

_main_t1 equ dword ptr [ebp-16]

_main_t2 equ byte ptr [ebp-17]

_main_t3 equ dword ptr [ebp-21]

_main_t4 equ dword ptr [ebp-25]

_main_t5 equ dword ptr [ebp-29]

_main_i equ dword ptr [ebp-4]

    mov eax,0
    mov _main_i,eax
    mov _main_t0,eax

_main_c equ dword ptr [ebp-12]

    mov eax,2
    mov _main_c,eax
    mov _main_t1,eax

_main_1:

    mov eax,_main_i
    mov ebx,_main_c
    cmp eax,ebx
    jl _main_4
    mov al,0
    jmp short _main_5
_main_4:
    mov al,1
_main_5:
    mov _main_t2,al

    mov al,_main_t2
    cmp al,0
    jne _main_2
    jmp _main_3
_main_2:

    mov al,1
    cbw
    mov _main_t3,eax

    mov eax,_main_i
    mov ebx,_main_t3
    add eax,ebx
    mov _main_t4,eax

    mov eax,_main_t4
    mov _main_i,eax
    mov _main_t5,eax

    jmp _main_1
_main_3:

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

