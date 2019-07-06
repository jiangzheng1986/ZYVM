//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Common.hpp"

//ZYVM指令集编码表

//NOP	00
//CBW	01
//CWD	02
//CLI	03
//STI	04
//PUSHF	05
//POPF	06
//PUSHFD 07
//POPFD	08
//HLT	09

//PUSH	10
//POP	14
//INC	18
//DEC	1C
//NEG	20
//NOT	24
//MUL	28
//IMUL	2C
//DIV	30
//IDIV	34

//MOV	50
//ADD	54
//SUB	58
//AND	5C
//OR	60
//XOR	64
//SHL	68
//SHR	6C
//SAR	70
//XCHG	74
//LEA	78
//CMP	7C
//IN	80
//OUT	84

//JMP	A0
//JE	A4
//JNE	A8
//JB	AC
//JNB	B0
//JA	B4
//JNA	B8
//JL	BC
//JNL	C0
//JG	C4
//JNG	C8
//CALL	CC
//INT	D0
//RET	D4
//IRET	D5
//INT_3	D6

enum enmAsmInstrType
{
	enmAsmInstrType_NoneOprand,
	enmAsmInstrType_SingleOprand,
	enmAsmInstrType_BinaryOprand,
};

enum enmAsmInstrAttr
{
	enmAsmInstrAttr_Dummy	= 0x00000001,
	enmAsmInstrAttr_SubCode = 0x00000002,
	enmAsmInstrAttr_InputOutput = 0x00000004,
	enmAsmInstrAttr_Jump	= 0x00000008,
};

enum enmAsmOprandType
{
	enmAsmOprandType_Nil,
	enmAsmOprandType_General,
	enmAsmOprandType_LabelRef,
};

enum enmAsmOprandAttr
{
	enmAsmOprandAttr_Byte	= 0x00000001,
	enmAsmOprandAttr_Word	= 0x00000002,
	enmAsmOprandAttr_Dword	= 0x00000004,
	enmAsmOprandAttr_Short	= 0x00000010,
	enmAsmOprandAttr_Near	= 0x00000020,
	enmAsmOprandAttr_Far	= 0x00000040,
};

class CAsmInstrItem
{
public:
	enmAsmInstrType m_eType;
	enmAsmInstrAttr m_eAttr;
	enmAsmOprandType m_eOprand1Type;
	enmAsmOprandAttr m_eOprand1Attr;
	enmAsmOprandType m_eOprand2Type;
	enmAsmOprandAttr m_eOprand2Attr;
	BYTE m_byCode;
	BYTE m_bySubCode;
};

class CAsmInstrItemList
{
public:
	std::vector<CAsmInstrItem> m_vecInstrItemList;
};

class CAsmInstrTable
{
public:
	CAsmInstrTable(void);
	~CAsmInstrTable(void);
	void AddInstrItem1(const char *pszInstr, enmAsmInstrType eType, enmAsmInstrAttr eAttr,
		enmAsmOprandType eOprand1Type, enmAsmOprandAttr eOprand1Attr, enmAsmOprandType eOprand2Type, enmAsmOprandAttr eOprand2Attr, BYTE byCode, BYTE bySubCode);
	void AddInstrItem2(const char *pszInstr, enmAsmInstrType eType, enmAsmInstrAttr eAttr,
		enmAsmOprandType eOprand1Type, enmAsmOprandAttr eOprand1Attr, BYTE byCode, BYTE bySubCode);
	void AddInstrItem3(const char *pszInstr, enmAsmInstrType eType, enmAsmInstrAttr eAttr,
		BYTE byCode, BYTE bySubCode);
	CAsmInstrItemList *FindInstrItemListOfInstr(const char *pszInstr);
	void AddInstrItem_NoneOprand_General(const char *pszInstr, BYTE byCode);
	void AddInstrItem_SingleOprand_General(const char *pszInstr, BYTE byCode);
	void AddInstrItem_BinaryOprand_General(const char *pszInstr, BYTE byCode);
	void BuildInstrTable(void);
protected:
	std::map<std::string,CAsmInstrItemList *> m_mapInstrTable;
};

CAsmInstrTable *CAsmInstrTable_GetInstance(void);