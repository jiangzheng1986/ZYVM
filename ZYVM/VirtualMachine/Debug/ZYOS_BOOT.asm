;===================================================================
; BOOTģ�� - λ�ڴ���0��������512���ֽڳ���ϵͳ����������
;===================================================================

	ORG 00000400H							;CPU�ӵ���00000400H����ʼִ��
	
	JMP SHORT BOOT_START
	
	;============== ���ƴ����ϵ�1~15��15�����������ݵ��ڴ�00000600H�� ==============
LoadKernelModule	PROC
	PUSH EAX
	PUSH EBX
	PUSH ECX
	
	MOV EAX, 1								;INT 80H,1�Ź��ܣ�����������ȡ
	MOV EBX, 00000600H						;EBX=��������ַ
	MOV ECX, 1								;ECX=������
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
	
	;============== ��ת��00000600H�����صĲ���ϵͳ�ں�ģ��ִ�� ==============
BOOT_START:									;����������ʼ��ַ

	;============== ��ʼ��BOOTģ����ʱ��ջָ�� ==============
	MOV ESP, 00100000H						;�ڴ�1M������ʱΪ����ϵͳ�ڴ���
	CALL LoadKernelModule
	MOV EAX, 00000600H
	JMP EAX
	
	ORG 000005FEH
	
OS_TAG DB 0AAH, 55H							;����ϵͳ��־
	