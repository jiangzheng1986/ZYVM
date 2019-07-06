    org 00100000h

    jmp start

_main proc

    push ebp
    mov ebp,esp
    sub esp,94

_main_t0 equ dword ptr [ebp-8]

_main_t1 equ dword ptr [ebp-16]

_main_t2 equ dword ptr [ebp-24]

_main_t3 equ dword ptr [ebp-28]

_main_t4 equ dword ptr [ebp-36]

_main_t5 equ dword ptr [ebp-40]

_main_t6 equ dword ptr [ebp-48]

_main_t7 equ dword ptr [ebp-52]

_main_t8 equ dword ptr [ebp-60]

_main_t9 equ dword ptr [ebp-64]

_main_t10 equ dword ptr [ebp-72]

_main_t11 equ dword ptr [ebp-76]

_main_t12 equ byte ptr [ebp-77]

_main_t13 equ byte ptr [ebp-78]

_main_t14 equ byte ptr [ebp-79]

_main_t15 equ byte ptr [ebp-80]

_main_t16 equ byte ptr [ebp-81]

_main_t17 equ byte ptr [ebp-82]

_main_t18 equ dword ptr [ebp-90]

_main_t19 equ dword ptr [ebp-94]

_main_a equ dword ptr [ebp-4]

    mov eax,20
    mov _main_a,eax
    mov _main_t0,eax

_main_b equ dword ptr [ebp-12]

    mov eax,12
    mov _main_b,eax
    mov _main_t1,eax

_main_c equ dword ptr [ebp-20]

    mov eax,_main_a
    mov ebx,_main_b
    add eax,ebx
    mov _main_t2,eax

    mov eax,_main_t2
    mov _main_c,eax
    mov _main_t3,eax

_main_d equ dword ptr [ebp-32]

    mov eax,_main_a
    mov ebx,_main_b
    sub eax,ebx
    mov _main_t4,eax

    mov eax,_main_t4
    mov _main_d,eax
    mov _main_t5,eax

_main_e equ dword ptr [ebp-44]

    mov eax,_main_a
    mov ebx,_main_b
    imul ebx
    mov _main_t6,eax

    mov eax,_main_t6
    mov _main_e,eax
    mov _main_t7,eax

_main_f equ dword ptr [ebp-56]

    mov eax,_main_a
    mov ebx,_main_b
    mov edx,0
    idiv ebx
    mov _main_t8,eax

    mov eax,_main_t8
    mov _main_f,eax
    mov _main_t9,eax

_main_g equ dword ptr [ebp-68]

    mov eax,_main_a
    mov ebx,_main_b
    mov edx,0
    idiv ebx
    mov eax,edx
    mov _main_t10,eax

    mov eax,_main_t10
    mov _main_g,eax
    mov _main_t11,eax

    mov eax,_main_a
    mov ebx,_main_b
    cmp eax,ebx
    jg _main_1
    mov al,0
    jmp short _main_2
_main_1:
    mov al,1
_main_2:
    mov _main_t12,al

    mov eax,_main_c
    mov ebx,_main_d
    cmp eax,ebx
    jg _main_3
    mov al,0
    jmp short _main_4
_main_3:
    mov al,1
_main_4:
    mov _main_t13,al

    mov al,_main_t12
    mov bl,_main_t13
    and al,bl
    mov _main_t14,al

    mov al,_main_t14
    xor al,1
    mov _main_t15,al

    mov eax,_main_e
    mov ebx,_main_f
    cmp eax,ebx
    jg _main_5
    mov al,0
    jmp short _main_6
_main_5:
    mov al,1
_main_6:
    mov _main_t16,al

    mov al,_main_t15
    mov bl,_main_t16
    or al,bl
    mov _main_t17,al

    mov al,_main_t17
    cmp al,0
    jne _main_7
    jmp _main_8
_main_7:
_main_h equ dword ptr [ebp-86]

    mov eax,_main_f
    mov ebx,_main_g
    add eax,ebx
    mov _main_t18,eax

    mov eax,_main_t18
    mov _main_h,eax
    mov _main_t19,eax

_main_8:

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

