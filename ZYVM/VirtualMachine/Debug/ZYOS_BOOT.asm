;===================================================================
; BOOT模块 - 位于磁盘0号扇区的512个字节程序，系统自启动程序
;===================================================================

	ORG 00000400H							;CPU加电后从00000400H处开始执行
	
	JMP SHORT BOOT_START
	
	;============== 复制磁盘上第1~15等15个扇区的内容到内存00000600H处 ==============
LoadKernelModule	PROC
	PUSH EAX
	PUSH EBX
	PUSH ECX
	
	MOV EAX, 1								;INT 80H,1号功能，磁盘扇区读取
	MOV EBX, 00000600H						;EBX=缓冲区地址
	MOV ECX, 1								;ECX=扇区号
S1:	INT 80H
	ADD EBX, 00000200H
	INC ECX
	CMP ECX, 15
	JNG S1
	
	POP EAX
	POP EBX
	POP ECX
	RET
LoadKernelModule	ENDP
	
	;============== 跳转到00000600H处加载的操作系统内核模块执行 ==============
BOOT_START:									;自启动程序开始地址

	;============== 初始化BOOT模块临时堆栈指针 ==============
	MOV ESP, 00100000H						;内存1M以内暂时为操作系统内存区
	CALL LoadKernelModule
	MOV EAX, 00000600H
	JMP EAX
	
	ORG 000005FEH
	
OS_TAG DB 0AAH, 55H							;操作系统标志
	