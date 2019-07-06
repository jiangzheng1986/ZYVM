;===================================================================
; KERNEL模块 - 位于磁盘1~15号扇区的512*15个字节,操作系统核心程序
;===================================================================

					ORG		00000600H		;被BOOT模块加载，并从00000600H处开始执行
	
TRUE				EQU		1
FALSE				EQU		0
	
	JMP KERNEL_START
	
g_nKernelMemorySize	DD		0				;核心内存大小

g_bKeyPressed		DB		FALSE			;键盘是否被按下
	
;===================================================================
; KernelMemoryDetect - 核心内存检测
;===================================================================
KernelMemoryDetect	PROC

	RET
KernelMemoryDetect	ENDP

;===================================================================
; KernelMemoryAlloc - 核心内存分配
;===================================================================
KernelMemoryAlloc	PROC

	RET
KernelMemoryAlloc	ENDP

;===================================================================
; KernelMemoryAlloc - 核心内存分配
;===================================================================
KernelMemoryFree	PROC

	RET
KernelMemoryFree	ENDP

;===================================================================
; ClearScreen - 清除屏幕
;===================================================================
ClearScreen			PROC
	PUSH EAX
	PUSH EBX
	
	MOV EAX, 1								;INT 81H,1号功能，清除屏幕
	MOV EBX, 00808080H						;EBX=颜色
	INT 81H
	
	POP EBX
	POP EAX
	RET
ClearScreen			ENDP

;===================================================================
; FillRectTest - 填充矩形测试
;===================================================================
FillRectTest		PROC
	PUSH EAX
	PUSH EBX
	PUSH ECX
	PUSH EDX
	PUSH ESI
	PUSH EDI
	
	MOV EAX, 2								;INT 81H,2号功能，填充矩形
	MOV EBX, 00FFFFFFH						;EBX=颜色
	MOV ECX, 362							;ECX=位置X
	MOV EDX, 284							;EDX=位置Y
	MOV ESI, 300							;ESI=宽度
	MOV EDI, 200							;EDI=高度
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
; SwapBuffer - 交换缓冲区
;===================================================================
SwapBuffer			PROC
	PUSH EAX
	
	MOV EAX, 3								;INT 81H,3号功能，交换缓冲区
	INT 81H
	
	POP EAX
	RET
SwapBuffer			ENDP

;===================================================================
; KeyInterruptHandler - 键盘中断处理程序
;===================================================================
KeyInterruptHandler	PROC
	MOV g_bKeyPressed, TRUE
	IRET
KeyInterruptHandler	ENDP

;===================================================================
; SetupKeyInterruptHandler - 安装键盘中断处理程序
;===================================================================
SetupKeyInterruptHandler PROC
	MOV EAX, 9								;9号中断，键盘中断
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