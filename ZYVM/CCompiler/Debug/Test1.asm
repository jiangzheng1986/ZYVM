    org 00100000h

    jmp start

_c dd ?

_max proc

    push ebp
    mov ebp,esp
    sub esp,1

_max_a equ dword ptr [ebp+12]

_max_b equ dword ptr [ebp+8]

_max_t0 equ byte ptr [ebp-1]

    mov eax,_max_a
    mov ebx,_max_b
    cmp eax,ebx
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
    mov eax,_max_a
    jmp _max_0

    jmp _max_5
_max_4:
    mov eax,_max_b
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
    sub esp,24

_main_t0 equ dword ptr [ebp-8]

_main_t1 equ dword ptr [ebp-16]

_main_t2 equ dword ptr [ebp-20]

_main_t3 equ dword ptr [ebp-24]

_main_a equ dword ptr [ebp-4]

    mov eax,10
    mov _main_a,eax
    mov _main_t0,eax

_main_b equ dword ptr [ebp-12]

    mov eax,20
    mov _main_b,eax
    mov _main_t1,eax

    mov eax,_main_a
    push eax

    mov eax,_main_b
    push eax

    call _max
    add esp,8
    mov _main_t2,eax

    mov eax,_main_t2
    mov _c,eax
    mov _main_t3,eax

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

