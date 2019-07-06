//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#pragma once

#define OPRAND_LENGTH_BYTE	0x00
#define OPRAND_LENGTH_WORD	0x01
#define OPRAND_LENGTH_DWORD	0x02

class CDisassembler
{
public:
	CDisassembler();
	~CDisassembler();
	std::string Disassemble(DWORD dwEIP, const BYTE *pbyCode, int &nCodeBytes);
private:
	BYTE GetByteCode();
	WORD GetWordCode();
	DWORD GetDwordCode();
	std::string GetGeneralRegister(BYTE byOprandLength, BYTE byRegister);
	std::string GetImmediate(BYTE byImmediateMode, BYTE byOprandLength);
	std::string GetOprandLengthPtr(BYTE byOprandLength);
	std::string GetImmediateOffset(BYTE byOprandLength);
	std::string GetScaleBaseIndex(BYTE byOprandLength);
	std::string GetMemoryAddress(BYTE byAddressCode);
	std::string GetJumpLengthStr(BYTE byOprandLength);
	std::string GetJumpOffset(BYTE byOprandLength);
	std::string GetOprand(BYTE byOprandLength);
	std::string Disassemble1(DWORD dwEIP, const BYTE *pbyCode, int &nCodeBytes);
private:
	DWORD m_dwEIP;
	BYTE *m_pbyCode;
	int m_nReadPointer;
};

CDisassembler *CDisassembler_GetInstance();