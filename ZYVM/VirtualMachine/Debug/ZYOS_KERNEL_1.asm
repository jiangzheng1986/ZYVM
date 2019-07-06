;===================================================================
; KERNELģ�� - λ�ڴ���1~15��������512*15���ֽ�,����ϵͳ���ĳ���
;===================================================================

					ORG		00000600H		;��BOOTģ����أ�����00000600H����ʼִ��
	
TRUE				EQU		1
FALSE				EQU		0
	
	JMP KERNEL_START
	
g_nKernelMemorySize	DD		0				;�����ڴ��С

g_bKeyPressed		DB		FALSE			;�����Ƿ񱻰���
	
;===================================================================
; KernelMemoryDetect - �����ڴ���
;===================================================================
KernelMemoryDetect	PROC

	RET
KernelMemoryDetect	ENDP

;===================================================================
; KernelMemoryAlloc - �����ڴ����
;===================================================================
KernelMemoryAlloc	PROC

	RET
KernelMemoryAlloc	ENDP

;===================================================================
; KernelMemoryAlloc - �����ڴ����
;===================================================================
KernelMemoryFree	PROC

	RET
KernelMemoryFree	ENDP

;===================================================================
; ClearScreen - �����Ļ
;===================================================================
ClearScreen			PROC
	PUSH EAX
	PUSH EBX
	
	MOV EAX, 1								;INT 81H,1�Ź��ܣ������Ļ
	MOV EBX, 00808080H						;EBX=��ɫ
	INT 81H
	
	POP EBX
	POP EAX
	RET
ClearScreen			ENDP

;===================================================================
; FillRectTest - �����β���
;===================================================================
FillRectTest		PROC
	PUSH EAX
	PUSH EBX
	PUSH ECX
	PUSH EDX
	PUSH ESI
	PUSH EDI
	
	MOV EAX, 2								;INT 81H,2�Ź��ܣ�������
	MOV EBX, 00FFFFFFH						;EBX=��ɫ
	MOV ECX, 362							;ECX=λ��X
	MOV EDX, 284							;EDX=λ��Y
	MOV ESI, 300							;ESI=���
	MOV EDI, 200							;EDI=�߶�
	INT 81H
	
	POP EDI
	POP ESI
	POP EDX
	POP ECX
	POP EBX
	POP EAX
	RET
FillRectTest		ENDP

;===================================================================
; SwapBuffer - ����������
;===================================================================
SwapBuffer			PROC
	PUSH EAX
	
	MOV EAX, 3								;INT 81H,3�Ź��ܣ�����������
	INT 81H
	
	POP EAX
	RET
SwapBuffer			ENDP

;===================================================================
; KeyInterruptHandler - �����жϴ������
;===================================================================
KeyInterruptHandler	PROC
	MOV g_bKeyPressed, TRUE
	IRET
KeyInterruptHandler	ENDP

;===================================================================
; SetupKeyInterruptHandler - ��װ�����жϴ������
;===================================================================
SetupKeyInterruptHandler PROC
	MOV EAX, 9								;9���жϣ������ж�
	SHL EAX, 2
	MOV DWORD PTR [EAX], OFFSET KeyInterruptHandler
	RET
SetupKeyInterruptHandler ENDP
	
KERNEL_START:
	CALL ClearScreen
	CALL FillRectTest
	CALL SwapBuffer
	CALL SetupKeyInterruptHandler
	STI
S1:	CMP g_bKeyPressed,TRUE
	JNE S1
	HLT
	
	ORG 00002400H