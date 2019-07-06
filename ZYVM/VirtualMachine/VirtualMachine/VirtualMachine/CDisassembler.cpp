//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "Pch.h"

CDisassembler *CDisassembler_GetInstance()
{
	static CDisassembler s_Disassembler;

	return &s_Disassembler;
}

CDisassembler::CDisassembler()
{
	m_pbyCode = NULL;
	m_nReadPointer = 0;
}

CDisassembler::~CDisassembler()
{

}

BYTE CDisassembler::GetByteCode()
{
	BYTE byCode = m_pbyCode[m_nReadPointer];

	m_nReadPointer++;

	m_dwEIP++;

	return byCode;
}

WORD CDisassembler::GetWordCode()
{
	WORD wCode = *(WORD *)&(m_pbyCode[m_nReadPointer]);

	m_nReadPointer += 2;

	m_dwEIP += 2;

	return wCode;
}

DWORD CDisassembler::GetDwordCode()
{
	DWORD dwCode = *(DWORD *)&(m_pbyCode[m_nReadPointer]);

	m_nReadPointer += 4;
	
	m_dwEIP += 4;

	return dwCode;
}

std::string CDisassembler::GetGeneralRegister(BYTE byOprandLength, BYTE byRegister)
{
	if ( byOprandLength == 2 )
	{
		switch ( byRegister )
		{
		case 0x00:
			return "EAX";
		case 0x01:
			return "ECX";
		case 0x02:
			return "EDX";
		case 0x03:
			return "EBX";
		case 0x04:
			return "ESP";
		case 0x05:
			return "EBP";
		case 0x06:
			return "ESI";
		case 0x07:
			return "EDI";
		default:
			ASSERT(FALSE);
			return "";
		}
	}
	else if ( byOprandLength == 1 )
	{
		switch ( byRegister )
		{
		case 0x00:
			return "AX";
		case 0x01:
			return "CX";
		case 0x02:
			return "DX";
		case 0x03:
			return "BX";
		case 0x04:
			return "SP";
		case 0x05:
			return "BP";
		case 0x06:
			return "SI";
		case 0x07:
			return "DI";
		default:
			ASSERT(FALSE);
			return "";
		}
	}
	else if ( byOprandLength == 0 )
	{
		switch ( byRegister )
		{
		case 0x00:
			return "AL";
		case 0x01:
			return "CL";
		case 0x02:
			return "DL";
		case 0x03:
			return "BL";
		case 0x04:
			return "AH";
		case 0x05:
			return "CH";
		case 0x06:
			return "DH";
		case 0x07:
			return "BH";
		default:
			ASSERT(FALSE);
			return "";
		}
	}
	else
	{
		ASSERT(FALSE);
		return "";
	}
}

std::string CDisassembler::GetImmediate(BYTE byImmediateMode, BYTE byOprandLength)
{
	char szBuf[1000];

	if ( byImmediateMode == 0x00 )
	{
		if ( byOprandLength == OPRAND_LENGTH_BYTE )
		{
			sprintf_s(szBuf, "%02XH", GetByteCode());
		}
		else if ( byOprandLength == OPRAND_LENGTH_WORD )
		{
			sprintf_s(szBuf, "%04XH", GetWordCode());
		}
		else if ( byOprandLength == OPRAND_LENGTH_DWORD )
		{
			sprintf_s(szBuf, "%08XH", GetDwordCode());
		}
		else
		{
			ASSERT(FALSE);
			szBuf[0] = 0;
		}
	}
	else if ( byImmediateMode == 0x01 )
	{
		sprintf_s(szBuf, "%02XH", GetByteCode());
	}
	else if ( byImmediateMode == 0x02 )
	{
		sprintf_s(szBuf, "%04XH", GetWordCode());
	}
	else if ( byImmediateMode == 0x03 )
	{
		BYTE byCode = GetByteCode();
		CHAR chCode = (CHAR)byCode;
		if ( chCode >= 0 )
		{
			sprintf_s(szBuf, "+%02XH", chCode);
		}
		else
		{
			sprintf_s(szBuf, "-%02XH", -chCode);
		}
	}
	else if ( byImmediateMode == 0x04 )
	{
		WORD wCode = GetWordCode();
		SHORT sCode = (SHORT)wCode;
		if ( sCode >= 0 )
		{
			sprintf_s(szBuf, "+%04XH", sCode);
		}
		else
		{
			sprintf_s(szBuf, "-%04XH", -sCode);
		}
	}

	return szBuf;
}

std::string CDisassembler::GetOprandLengthPtr(BYTE byOprandLength)
{
	if ( byOprandLength == OPRAND_LENGTH_BYTE )
	{
		return "BYTE PTR";
	}
	else if ( byOprandLength == OPRAND_LENGTH_WORD )
	{
		return "WORD PTR";
	}
	else if ( byOprandLength == OPRAND_LENGTH_DWORD )
	{
		return "DWORD PTR";
	}
	else
	{
		ASSERT(FALSE);
		return "";
	}
}

std::string CDisassembler::GetImmediateOffset(BYTE byOprandLength)
{
	char szBuf[1000];

	if ( byOprandLength == OPRAND_LENGTH_BYTE )
	{
		BYTE byCode = GetByteCode();
		CHAR chCode = (CHAR)byCode;
		if ( chCode >= 0 )
		{
			sprintf_s(szBuf, "+%02XH", chCode);
		}
		else
		{
			sprintf_s(szBuf, "-%02XH", -chCode);
		}
	}
	else if ( byOprandLength == OPRAND_LENGTH_WORD )
	{
		WORD wCode = GetWordCode();
		SHORT sCode = (SHORT)wCode;
		if ( sCode >= 0 )
		{
			sprintf_s(szBuf, "+%04XH", sCode);
		}
		else
		{
			sprintf_s(szBuf, "-%04XH", -sCode);
		}
	}
	else if ( byOprandLength == OPRAND_LENGTH_DWORD )
	{
		DWORD dwCode = GetDwordCode();
		INT nCode = (INT)dwCode;
		if ( nCode >= 0 )
		{
			sprintf_s(szBuf, "+%08XH", nCode);
		}
		else
		{
			sprintf_s(szBuf, "-%08XH", -nCode);
		}
	}
	else
	{
		ASSERT(FALSE);
		szBuf[0] = 0;
	}

	return szBuf;
}

std::string CDisassembler::GetMemoryAddress(BYTE byAddressCode)
{
	BYTE byMemoryMode = (byAddressCode & 0x0F);

	static BYTE arrMemoryAddressing[10] = 
	{
		MEMADDR_OFFSET_32,
		MEMADDR_BASE_REG,
		MEMADDR_BASE_REG | MEMADDR_OFFSET_8,
		MEMADDR_BASE_REG | MEMADDR_OFFSET_32,
		MEMADDR_INDEX_REG,
		MEMADDR_INDEX_REG | MEMADDR_OFFSET_8,
		MEMADDR_INDEX_REG | MEMADDR_OFFSET_32,
		MEMADDR_BASE_REG | MEMADDR_INDEX_REG,
		MEMADDR_BASE_REG | MEMADDR_INDEX_REG | MEMADDR_OFFSET_8,
		MEMADDR_BASE_REG | MEMADDR_INDEX_REG | MEMADDR_OFFSET_32,
	};

	std::string szCode = "";

	if ( byMemoryMode >= sizeof(arrMemoryAddressing) )
	{
		return szCode;
	}

	BYTE byMemoryAddressing = arrMemoryAddressing[byMemoryMode];

	if ( byMemoryAddressing == MEMADDR_OFFSET_32 )
	{
		szCode += GetImmediate(0x00, OPRAND_LENGTH_DWORD);

		return szCode;
	}

	BYTE bySIB = GetByteCode();

	if ( byMemoryAddressing & MEMADDR_BASE_REG )
	{
		BYTE byBaseRegister = (bySIB & 0x07);				//00 000 111

		szCode += GetGeneralRegister(OPRAND_LENGTH_DWORD, byBaseRegister);
	}

	if ( byMemoryAddressing & MEMADDR_INDEX_REG )
	{
		if ( byMemoryAddressing & MEMADDR_BASE_REG )
		{
			szCode += "+";
		}

		BYTE byIndexRegister = (bySIB & 0x38) >> 3;			//00 111 000

		BYTE byIndexRegisterScale = (bySIB & 0xC0) >> 6;	//11 000 000

		szCode += GetGeneralRegister(OPRAND_LENGTH_DWORD, byIndexRegister);

		if ( byIndexRegisterScale != 0 )
		{
			char szBuf[1000];
			int nScale = (1<<byIndexRegisterScale);
			sprintf_s(szBuf, "*%d", nScale);
			szCode += szBuf;
		}
	}

	if ( byMemoryAddressing & MEMADDR_OFFSET_8 )
	{
		szCode += "+";

		szCode += GetImmediateOffset(OPRAND_LENGTH_BYTE);
	}

	if ( byMemoryAddressing & MEMADDR_OFFSET_32 )
	{
		szCode += "+";

		szCode += GetImmediateOffset(OPRAND_LENGTH_DWORD);
	}

	return szCode;
}

std::string CDisassembler::GetJumpLengthStr(BYTE byOprandLength)
{
	if ( byOprandLength == OPRAND_LENGTH_BYTE )
	{
		return "SHORT";
	}
	else if ( byOprandLength == OPRAND_LENGTH_WORD )
	{
		return "NEAR";
	}
	else if ( byOprandLength == OPRAND_LENGTH_DWORD )
	{
		return "FAR";
	}
	else
	{
		return "???";
	}
}

std::string CDisassembler::GetJumpOffset(BYTE byOprandLength)
{
	int nOffset = 0;

	if ( byOprandLength == OPRAND_LENGTH_BYTE )
	{
		nOffset = (int)(char)GetByteCode();
	}
	else if ( byOprandLength == OPRAND_LENGTH_WORD )
	{
		nOffset = (int)(short)GetWordCode();
	}
	else if ( byOprandLength == OPRAND_LENGTH_DWORD )
	{
		nOffset = (int)GetDwordCode();
	}
	else
	{
		nOffset = 0;
	}

	DWORD dwNewEIP = m_dwEIP + nOffset;

	char szBuf[1000];
	sprintf_s(szBuf, "%08X", dwNewEIP);

	return szBuf;
}

std::string CDisassembler::GetOprand(BYTE byOprandLength)
{
	BYTE byAddressing = GetByteCode();

	BYTE byMode = (byAddressing & 0xF0) >> 4;

	switch ( byMode )
	{
	case 0x00:
		{
			BYTE byRegister = (byAddressing & 0x07);

			return GetGeneralRegister(byOprandLength, byRegister);
		}
	case 0x02:
		{
			BYTE byImmediateMode = (byAddressing & 0x0F);

			return GetImmediate(byImmediateMode, byOprandLength);
		}
	case 0x03:
		{
			std::string szCode;
			szCode += GetOprandLengthPtr(byOprandLength);
			szCode += " [";
			szCode += GetMemoryAddress(byAddressing);
			szCode += "]";

			return szCode;
		}
	case 0x04:
		{
			std::string szCode;
			szCode += GetJumpLengthStr(byOprandLength);
			szCode += ' ';
			szCode += GetJumpOffset(byOprandLength);
			return szCode;
		}
	default:
		return "";
	}
}

std::string CDisassembler::Disassemble(DWORD dwEIP, const BYTE *pbyCode, int &nCodeBytes)
{
	std::string szCode1;

	std::string szCode = Disassemble1(dwEIP, pbyCode, nCodeBytes);

	char szBuf[1000];
	sprintf_s(szBuf, "%08X: ", dwEIP);
	szCode1 += szBuf;

	int i;
	for ( i = 0; i < nCodeBytes; i++ )
	{
		sprintf_s(szBuf, "%02X ", pbyCode[i]);
		szCode1 += szBuf;
	}

	for ( ; i < 9; i++ )
	{
		szCode1 += "   ";
	}

	szCode1 += szCode;

	return szCode1;
}

std::string CDisassembler::Disassemble1(DWORD dwEIP, const BYTE *pbyCode, int &nCodeBytes)
{
	m_dwEIP = dwEIP;

	m_pbyCode = (BYTE *)pbyCode;

	m_nReadPointer = 0;

	std::string szCode;

	BYTE byCode = GetByteCode();

	switch ( byCode )
	{
	case 0x00:
		szCode = "NOP";
		break;
	case 0x01:
		szCode = "CBW";
		break;
	case 0x02:
		szCode = "CWD";
		break;
	case 0x03:
		szCode = "CLI";
		break;
	case 0x04:
		szCode = "STI";
		break;
	case 0x05:
		szCode = "PUSHF";
		break;
	case 0x06:
		szCode = "POPF";
		break;
	case 0x07:
		szCode = "PUSHFD";
		break;
	case 0x08:
		szCode = "POPFD";
		break;
	case 0x09:
		szCode = "HLT";
		break;
	case 0xD4:
		szCode = "RET";
		break;
	case 0xD5:
		szCode = "IRET";
		break;
	case 0xD6:
		szCode = "INT 3";
		break;
	default:
		{
			BYTE byOprandLength = (byCode & 0x03);

			BYTE byCode1 = (byCode & ~0x03);

			switch ( byCode1 )
			{
			case 0x10:
				szCode += "PUSH";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x14:
				szCode += "POP";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x18:
				szCode += "INC";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x1C:
				szCode += "DEC";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x20:
				szCode += "NEG";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x24:
				szCode += "NOT";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x28:
				szCode += "MUL";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x2C:
				szCode += "IMUL";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x30:
				szCode += "DIV";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x34:
				szCode += "IDIV";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x50:
				szCode += "MOV";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;			
			case 0x54:
				szCode += "ADD";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x58:
				szCode += "SUB";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x5C:
				szCode += "AND";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x60:
				szCode += "OR";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x64:
				szCode += "XOR";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x68:
				szCode += "SHL";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x6C:
				szCode += "SHR";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x70:
				szCode += "SAR";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x74:
				szCode += "XCHG";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x78:
				szCode += "LEA";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x7C:
				szCode += "CMP";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x80:
				szCode += "IN";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;
			case 0x84:
				szCode += "OUT";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				szCode += ",";
				szCode += GetOprand(byOprandLength);
				break;
			case 0xA0:
				szCode += "JMP";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0xA4:
				szCode += "JE";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0xA8:
				szCode += "JNE";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0xAC:
				szCode += "JB";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0xB0:
				szCode += "JNB";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0xB4:
				szCode += "JA";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0xB8:
				szCode += "JNA";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0xBC:
				szCode += "JL";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0xC0:
				szCode += "JNL";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0xC4:
				szCode += "JG";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0xC8:
				szCode += "JNG";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0xCC:
				szCode += "CALL";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			case 0xD0:
				szCode += "INT";
				szCode += " ";
				szCode += GetOprand(byOprandLength);
				break;
			default:
				{
					CHAR szBuffer[1000];
					sprintf_s(szBuffer, "DB %02XH", byCode);
					szCode = szBuffer;
				}
				break;
			}
		}
		break;
	}

	nCodeBytes = m_nReadPointer;

	return szCode;
}