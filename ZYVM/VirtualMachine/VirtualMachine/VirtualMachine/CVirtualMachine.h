//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#pragma once

//IF无故被打开的问题 CMP的时候
//外中断判断IF的问题

//当前要实现的功能:
//1.C编译器
//2.链接器

//指令字:
//位7,6,5,4,3,2,1,0:指令码
//对于某些区分字长的指令 位1,0:00:字节操作
//                             01:字操作
//                             10:双字操作
//                             11:保留

//无操作数指令
//NOP                    OK
//CBW                    OK
//CWD                    OK
//CLI                    OK
//STI                    OK
//PUSHF                  OK
//POPF                   OK
//PUSHFD                 OK
//POPFD                  OK
//HLT                    OK

//单操作数指令
//PUSH X                 OK
//POP X                  OK
//INC X                  OK
//DEC X                  OK
//NEG X                  OK
//NOT X                  OK
//MUL X                  OK
//IMUL X                 OK
//DIV X                  OK
//IDIV X                 OK

//双操作数指令
//MOV X,Y                OK
//ADD X,Y                OK
//SUB X,Y                OK
//ADC X,Y                OK
//SBB X,Y                OK
//AND X,Y                OK
//OR X,Y                 OK
//XOR X,Y                OK
//SHL X,Y                OK
//SHR X,Y                OK
//XCHG X,Y               OK
//LEA X,Y                OK
//CMP X,Y                OK
//IN X,Y                 OK
//OUT X,Y                OK

//转移类指令
//JMP X                  OK
//JE X                   OK
//JNE X                  OK
//JB X                   OK
//JNB X                  OK
//JA X                   OK
//JNA X                  OK
//JL X                   OK
//JNL X                  OK
//JG X                   OK
//JNG X                  OK
//CALL X				 OK
//RET                    OK
//INT X					 OK
//IRET                   OK
//INT 3                  OK

//寻址方式字节:
//位7,6,5,4:0000:通用寄存器                                           OK
//			0001:段寄存器                                             OK
//			0010:立即数                                               OK
//			0011:内存                                                 OK
//          0100:偏移量                                               OK

//如果是通用寄存器:													  OK
//位2,1,0:000:EAX AX AL
//        001:ECX CX CL
//        010:EDX DX DL
//        011:EBX BX BL
//        100:ESP SP AH
//        101:EBP BP CH
//        110:ESI SI DH
//        111:EDI DI BH

//如果是段寄存器:													  OK
//位1,0:00:ES
//      01:CS
//      10:SS
//      11:DS

//如果是立即数:
//位3,2,1,0:0000:无扩展                                               OK
//          0001:无符号扩展字节                                       OK
//			0010:无符号扩展字                                         OK
//          0011:带符号扩展字节                                       OK
//			0100:带符号扩展字                                         OK

//如果是内存:
//位3,2,1,0:0000:[32位地址]                                           OK
//          0001:[基址寄存器]                                         OK
//          0010:[基址寄存器+8位偏移]                                 OK
//          0011:[基址寄存器+32位偏移]                                OK
//          0100:[变址寄存器*缩放因子]                                OK
//          0101:[变址寄存器*缩放因子+8位偏移]                        OK
//          0110:[变址寄存器*缩放因子+32位偏移]				          OK
//          0111:[基址寄存器+变址寄存器*缩放因子]                     OK
//          1000:[基址寄存器+变址寄存器*缩放因子+8位偏移]             OK
//          1001:[基址寄存器+变址寄存器*缩放因子+32位偏移]            OK

//基址变址缩放字节:													  OK
//位7,6:    变址寄存器缩放因子(1,2,4,8)
//位5,4,3:  变址寄存器
//位2,1,0:  基址寄存器

#define ASSERT(x) if(!(x)){int i=0;i=i/i;}

#define ZERO_STRUCT(x) memset(&(x),0,sizeof((x)));

enum enmOperation
{
	//简单
	enmOperation_NOP	= 0x00,
	enmOperation_CBW	= 0x01,
	enmOperation_CWD	= 0x02,
	enmOperation_CLI	= 0x03,
	enmOperation_STI	= 0x04,
	enmOperation_PUSHF	= 0x05,
	enmOperation_POPF	= 0x06,
	enmOperation_PUSHFD	= 0x07,
	enmOperation_POPFD	= 0x08,
	enmOperation_HLT	= 0x09,
	//一元
	enmOperation_PUSH	= 0x10,//10~13
	enmOperation_POP	= 0x14,//14~17
	enmOperation_INC	= 0x18,//18~1B
	enmOperation_DEC	= 0x1C,//1C~1F
	enmOperation_NEG	= 0x20,
	enmOperation_NOT	= 0x24,	
	enmOperation_MUL	= 0x28,
	enmOperation_IMUL	= 0x2C,
	enmOperation_DIV	= 0x30,
	enmOperation_IDIV	= 0x34,
	//二元
	enmOperation_MOV	= 0x50,
	enmOperation_ADD	= 0x54,
	enmOperation_SUB	= 0x58,
	enmOperation_AND	= 0x5C,
	enmOperation_OR		= 0x60,
	enmOperation_XOR	= 0x64,
	enmOperation_SHL	= 0x68,
	enmOperation_SHR	= 0x6C,
	enmOperation_SAR	= 0x70,
	enmOperation_XCHG	= 0x74,
	enmOperation_LEA	= 0x78,
	enmOperation_CMP	= 0x7C,
	enmOperation_IN		= 0x80,
	enmOperation_OUT	= 0x84,
	//转移
	enmOperation_JMP	= 0xA0,
	enmOperation_JE		= 0xA4,
	enmOperation_JNE	= 0xA8,
	enmOperation_JB		= 0xAC,
	enmOperation_JNB	= 0xB0,
	enmOperation_JA		= 0xB4,
	enmOperation_JNA	= 0xB8,
	enmOperation_JL		= 0xBC,
	enmOperation_JNL	= 0xC0,
	enmOperation_JG		= 0xC4,
	enmOperation_JNG	= 0xC8,
	enmOperation_CALL	= 0xCC,
	enmOperation_INT	= 0xD0,
	enmOperation_RET	= 0xD4,
	enmOperation_IRET	= 0xD5,
	enmOperation_INT_3	= 0xD6,
};

enum enmOprandLength
{
	enmOprandLength_Byte,
	enmOprandLength_Word,
	enmOprandLength_Dword,
};

enum enmByteRegister
{
	enmByteRegister_AL,
	enmByteRegister_CL,
	enmByteRegister_DL,
	enmByteRegister_BL,
	enmByteRegister_AH,
	enmByteRegister_CH,
	enmByteRegister_DH,
	enmByteRegister_BH,
	enmByteRegister_Count,
};

enum enmWordRegister
{
	enmWordRegister_AX,
	enmWordRegister_CX,
	enmWordRegister_DX,
	enmWordRegister_BX,
	enmWordRegister_SP,
	enmWordRegister_BP,
	enmWordRegister_SI,
	enmWordRegister_DI,
	enmWordRegister_IP,
	enmWordRegister_FLAGS,
	enmWordRegister_ES,
	enmWordRegister_CS,
	enmWordRegister_SS,
	enmWordRegister_DS,
	enmWordRegister_Count,
};

enum enmDwordRegister
{
	enmDwordRegister_EAX,
	enmDwordRegister_ECX,
	enmDwordRegister_EDX,
	enmDwordRegister_EBX,
	enmDwordRegister_ESP,
	enmDwordRegister_EBP,
	enmDwordRegister_ESI,
	enmDwordRegister_EDI,
	enmDwordRegister_EIP,
	enmDwordRegister_EFLAGS,
	enmDwordRegister_ES1,
	enmDwordRegister_CS1,
	enmDwordRegister_SS1,
	enmDwordRegister_DS1,
	enmDwordRegister_ES2,
	enmDwordRegister_CS2,
	enmDwordRegister_SS2,
	enmDwordRegister_DS2,
	enmDwordRegister_CR0,
	enmDwordRegister_CR1,
	enmDwordRegister_CR2,
	enmDwordRegister_CR3,
	enmDwordRegister_GDTR,
	enmDwordRegister_LDTR,
	enmDwordRegister_IDTR,
	enmDwordRegister_Count,
};

#define MEMADDR_BASE_REG	0x01

#define MEMADDR_INDEX_REG	0x02

#define MEMADDR_OFFSET_8	0x04

#define MEMADDR_OFFSET_32	0x08

#define EFLAGS_BIT_CF		0

#define EFLAGS_BIT_PF		2

#define EFLAGS_BIT_AF		4

#define EFLAGS_BIT_ZF		6

#define EFLAGS_BIT_SF		7

#define EFLAGS_BIT_TF		8

#define EFLAGS_BIT_IF		9

#define EFLAGS_BIT_DF		10

#define EFLAGS_BIT_OF		11

#define HELPER_FLAGS_MASK	0x000008D5

#define SYSTEM_MEMORY_SIZE	(64 * 1024 * 1024)

#define VIDEO_MEMORY_SIZE	(4 * 1024 * 1024)

#define VIDEO_MEMORY_BASE	0xB8000000

#define PORT_REGISTERS_COUNT USHRT_MAX

#define SECTOR_SIZE			512

class CVirtualMachine
{
public:
	CVirtualMachine();
	~CVirtualMachine();
	void Initialize();
	void Reset();
	DWORD GetRegister(enmOprandLength eOprandLength, DWORD dwRegister);
	void SetRegister(enmOprandLength eOprandLength, DWORD dwRegister, DWORD dwValue);
	DWORD GetEIP();
	void SetEIP(DWORD dwEIP);
	DWORD GetMemory(enmOprandLength eOprandLength, DWORD dwPhysicalAddress);
	void SetMemory(enmOprandLength eOprandLength, DWORD dwPhysicalAddress, DWORD dwValue);
	BYTE FetchByteCode();
	WORD FetchWordCode();
	DWORD FetchDwordCode();
	enmOprandLength GetOprandLength(BYTE byCode);
	DWORD GetMemoryAddress(BYTE byAddressCode);
	DWORD GetOprand(enmOprandLength eOprandLength);
	void SetOprand(enmOprandLength eOprandLength, DWORD dwValue);
	DWORD GetEffectiveAddressOfOprand();
	int GetSizeOfOprandLength(enmOprandLength eOprandLength);
	void PushValue(enmOprandLength eOprandLength, DWORD dwValue);
	DWORD PopValue(enmOprandLength eOprandLength);
	void ClearEflagsBit(int nBit);
	void SetEflagsBit(int nBit);
	bool GetEflagsBit(int nBit);
	DWORD InPort(enmOprandLength eOprandLength, WORD wPortNumber);
	void OutPort(enmOprandLength eOprandLength, WORD wPortNumber, DWORD dwValue);
	void ReadDiskSector(DWORD dwSector, DWORD dwAddress);
	void PerformSimpleOperation(enmOperation eOperation, enmOprandLength eOprandLength);
	void ExecuteSimpleInstruction(enmOperation eOperation, enmOprandLength eOprandLength);
	void PerformUnaryOperation_MUL_BYTE(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags, bool bSignedOperation);
	void PerformUnaryOperation_MUL_WORD(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags, bool bSignedOperation);
	void PerformUnaryOperation_MUL_DWORD(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags, bool bSignedOperation);
	void PerformUnaryOperation_DIV_BYTE(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags, bool bSignedOperation);
	void PerformUnaryOperation_DIV_WORD(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags, bool bSignedOperation);
	void PerformUnaryOperation_DIV_DWORD(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags, bool bSignedOperation);
	void PerformUnaryOperation(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags);
	void ExecuteUnaryInstruction(enmOperation eOperation, enmOprandLength eOprandLength);
	void ExecuteUnaryInstruction_PUSH(enmOperation eOperation, enmOprandLength eOprandLength);
	void ExecuteUnaryInstruction_POP(enmOperation eOperation, enmOprandLength eOprandLength);
	void PerformBinaryOperation_CMP_BYTE(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand1, DWORD dwOprand2, DWORD &dwEflags);
	void PerformBinaryOperation_CMP_WORD(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand1, DWORD dwOprand2, DWORD &dwEflags);
	void PerformBinaryOperation_CMP_DWORD(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand1, DWORD dwOprand2, DWORD &dwEflags);
	void PerformBinaryOperation(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand1, DWORD dwOprand2, DWORD &dwEflags);
	void ExecuteBinaryInstruction(enmOperation eOperation, enmOprandLength eOprandLength);
	void ExecuteBinaryInstruction_XCHG(enmOperation eOperation, enmOprandLength eOprandLength);
	void ExecuteBinaryInstruction_LEA(enmOperation eOperation, enmOprandLength eOprandLength);
	void ExecuteBinaryInstruction_IN(enmOperation eOperation, enmOprandLength eOprandLength);
	void ExecuteBinaryInstruction_OUT(enmOperation eOperation, enmOprandLength eOprandLength);
	void ExecuteJumpInstruction(enmOperation eOperation, enmOprandLength eOprandLength);
	void DoInterrupt(BYTE byInterruptNumber);
	void DoInterrupt_80H();
	void DoInterrupt_81H();
	void Print(bool bDisassemble);
	void SetWritePosition(DWORD dwPhysicalAddress);
	DWORD GetWritePosition();
	void WriteByte(BYTE byCode);
	void WriteWord(WORD wCode);
	void WriteDword(DWORD dwCode);
	BYTE *GetSystemMemoryPointer();
	bool Execute();
private:
	DWORD m_adwRegisters[enmDwordRegister_Count];
	BYTE *m_pbySystemMemory;
	BYTE *m_pbyVideoMemory;
	BYTE *m_pbyPortRegisters;
	DWORD m_dwWriteAddress;
	bool m_bDivideOverflow;
};

CVirtualMachine *CVirtualMachine_GetInstance();