//---------------------------------------------------------------------------------------------------------
// Virtual Machine - VM Runtime
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#include "Pch.h"

CVirtualMachine *CVirtualMachine_GetInstance()
{
	static CVirtualMachine s_VirtualMachine;

	return &s_VirtualMachine;
}

CVirtualMachine::CVirtualMachine()
{
	m_pbySystemMemory = NULL;

	m_pbyVideoMemory = NULL;

	m_pbyPortRegisters = NULL;

	m_dwWriteAddress = 0;

	m_bDivideOverflow = 0;

	Initialize();
}

CVirtualMachine::~CVirtualMachine()
{

}

void CVirtualMachine::Initialize()
{
	Reset();
}

void CVirtualMachine::Reset()
{
	int i;
	
	for ( i = 0; i < enmDwordRegister_Count; i++ )
	{
		m_adwRegisters[i] = 0;
	}

	SetRegister(enmOprandLength_Dword, enmDwordRegister_EIP, 0x0000400);

	SetRegister(enmOprandLength_Dword, enmDwordRegister_ESP, 0x0001000);

	if ( m_pbySystemMemory == NULL )
	{
		m_pbySystemMemory = new BYTE[SYSTEM_MEMORY_SIZE];

		memset(m_pbySystemMemory, 0, SYSTEM_MEMORY_SIZE);
	}

	if ( m_pbyVideoMemory == NULL )
	{
		m_pbyVideoMemory = new BYTE[VIDEO_MEMORY_SIZE];
	}

	if ( m_pbyPortRegisters == NULL )
	{
		m_pbyPortRegisters = new BYTE[PORT_REGISTERS_COUNT];
	}

	for ( i = 0; i < PORT_REGISTERS_COUNT; i++ )
	{
		m_pbyPortRegisters[i] = 0;
	}

	ReadDiskSector(0, 0x00000400);
}

DWORD CVirtualMachine::GetRegister(enmOprandLength eOprandLength, DWORD dwRegister)
{
	if ( eOprandLength == enmOprandLength_Dword )
	{
		if ( dwRegister >= enmDwordRegister_Count )
		{
			ASSERT(FALSE);
		}

		return m_adwRegisters[dwRegister];
	}
	else if ( eOprandLength == enmOprandLength_Word )
	{
		if ( dwRegister >= enmWordRegister_Count )
		{
			ASSERT(FALSE);
		}

		return *(WORD *)&(m_adwRegisters[dwRegister]);
	}
	else if ( eOprandLength == enmOprandLength_Byte )
	{
		if ( dwRegister >= enmByteRegister_Count )
		{
			ASSERT(FALSE);
		}

		if ( dwRegister < enmByteRegister_AH )
		{
			return *(BYTE *)&(m_adwRegisters[dwRegister]);
		}
		else
		{
			return *(((BYTE *)&(m_adwRegisters[dwRegister - enmByteRegister_AH])) + 1);
		}
	}

	ASSERT(FALSE);

	return 0;
}

void CVirtualMachine::SetRegister(enmOprandLength eOprandLength, DWORD dwRegister, DWORD dwValue)
{
	if ( eOprandLength == enmOprandLength_Dword )
	{
		if ( dwRegister >= enmDwordRegister_Count )
		{
			ASSERT(FALSE);
		}

		m_adwRegisters[dwRegister] = dwValue;
	}
	else if ( eOprandLength == enmOprandLength_Word )
	{
		if ( dwRegister >= enmWordRegister_Count )
		{
			ASSERT(FALSE);
		}

		*(WORD *)&(m_adwRegisters[dwRegister]) = (WORD)dwValue;
	}
	else if ( eOprandLength == enmOprandLength_Byte )
	{
		if ( dwRegister >= enmByteRegister_Count )
		{
			ASSERT(FALSE);
		}

		if ( dwRegister < enmByteRegister_AH )
		{
			*(BYTE *)&(m_adwRegisters[dwRegister]) = (BYTE)dwValue;
		}
		else
		{
			*(((BYTE *)&(m_adwRegisters[dwRegister - enmByteRegister_AH])) + 1) = (BYTE)dwValue;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
}

DWORD CVirtualMachine::GetEIP()
{
	return m_adwRegisters[enmDwordRegister_EIP];
}

void CVirtualMachine::SetEIP(DWORD dwEIP)
{
	m_adwRegisters[enmDwordRegister_EIP] = dwEIP;
}

DWORD CVirtualMachine::GetMemory(enmOprandLength eOprandLength, DWORD dwPhysicalAddress)
{
	DWORD dwValue = 0;

	if ( dwPhysicalAddress < SYSTEM_MEMORY_SIZE  )
	{
		dwValue = *(DWORD *)&(m_pbySystemMemory[dwPhysicalAddress]);
	}
	else if ( dwPhysicalAddress >= VIDEO_MEMORY_BASE && dwPhysicalAddress < VIDEO_MEMORY_BASE + VIDEO_MEMORY_SIZE )
	{
		dwValue = *(DWORD *)&(m_pbyVideoMemory[dwPhysicalAddress - VIDEO_MEMORY_BASE]);
	}
	else
	{
		ASSERT(FALSE);
	}

	if ( eOprandLength == enmOprandLength_Dword )
	{
		return dwValue;
	}
	else if ( eOprandLength == enmOprandLength_Word )
	{
		return (WORD)dwValue;
	}
	else if ( eOprandLength == enmOprandLength_Byte )
	{
		return (BYTE)dwValue;
	}

	ASSERT(FALSE);

	return 0;
}

void CVirtualMachine::SetMemory(enmOprandLength eOprandLength, DWORD dwPhysicalAddress, DWORD dwValue)
{
	if ( eOprandLength == enmOprandLength_Dword )
	{
		if ( dwPhysicalAddress < SYSTEM_MEMORY_SIZE  )
		{
			*(DWORD *)&(m_pbySystemMemory[dwPhysicalAddress]) = dwValue;
		}
		else if ( dwPhysicalAddress >= VIDEO_MEMORY_BASE && dwPhysicalAddress < VIDEO_MEMORY_BASE + VIDEO_MEMORY_SIZE )
		{
			*(DWORD *)&(m_pbyVideoMemory[dwPhysicalAddress - VIDEO_MEMORY_BASE]) = dwValue;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if ( eOprandLength == enmOprandLength_Word )
	{
		if ( dwPhysicalAddress < SYSTEM_MEMORY_SIZE  )
		{
			*(WORD *)&(m_pbySystemMemory[dwPhysicalAddress]) = (WORD)dwValue;
		}
		else if ( dwPhysicalAddress >= VIDEO_MEMORY_BASE && dwPhysicalAddress < VIDEO_MEMORY_BASE + VIDEO_MEMORY_SIZE )
		{
			*(WORD *)&(m_pbyVideoMemory[dwPhysicalAddress - VIDEO_MEMORY_BASE]) = (WORD)dwValue;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if ( eOprandLength == enmOprandLength_Byte )
	{
		if ( dwPhysicalAddress < SYSTEM_MEMORY_SIZE  )
		{
			*(BYTE *)&(m_pbySystemMemory[dwPhysicalAddress]) = (BYTE)dwValue;
		}
		else if ( dwPhysicalAddress >= VIDEO_MEMORY_BASE && dwPhysicalAddress < VIDEO_MEMORY_BASE + VIDEO_MEMORY_SIZE )
		{
			*(BYTE *)&(m_pbyVideoMemory[dwPhysicalAddress - VIDEO_MEMORY_BASE]) = (BYTE)dwValue;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else
	{
		ASSERT(FALSE);
	}
}

BYTE CVirtualMachine::FetchByteCode()
{
	DWORD dwEIP = GetEIP();

	BYTE byCode = (BYTE)GetMemory(enmOprandLength_Byte, dwEIP);

	dwEIP += 1;

	SetEIP(dwEIP);

	return byCode;
}

WORD CVirtualMachine::FetchWordCode()
{
	DWORD dwEIP = GetEIP();

	WORD wCode = (WORD)GetMemory(enmOprandLength_Word, dwEIP);

	dwEIP += 2;

	SetEIP(dwEIP);

	return wCode;
}

DWORD CVirtualMachine::FetchDwordCode()
{
	DWORD dwEIP = GetEIP();

	DWORD dwCode = (DWORD)GetMemory(enmOprandLength_Dword, dwEIP);

	dwEIP += 4;

	SetEIP(dwEIP);

	return dwCode;
}

enmOprandLength CVirtualMachine::GetOprandLength(BYTE byCode)
{
	return (enmOprandLength)(byCode & 0x03);
}

DWORD CVirtualMachine::GetMemoryAddress(BYTE byAddressCode)
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

	if ( byMemoryMode >= sizeof(arrMemoryAddressing) )
	{
		return 0;
	}

	BYTE byMemoryAddressing = arrMemoryAddressing[byMemoryMode];

	DWORD dwAddress = 0;

	if ( byMemoryAddressing == MEMADDR_OFFSET_32 )
	{
		dwAddress = FetchDwordCode();

		return dwAddress;
	}

	BYTE bySIB = FetchByteCode();
	
	if ( byMemoryAddressing & MEMADDR_BASE_REG )
	{
		BYTE byBaseReg = (bySIB & 0x07);

		DWORD dwBaseReg = GetRegister(enmOprandLength_Dword, byBaseReg);

		dwAddress += dwBaseReg;
	}

	if ( byMemoryAddressing & MEMADDR_INDEX_REG )
	{
		BYTE byIndexScale = (bySIB & 0xC0) >> 6;

		BYTE byIndexReg = (bySIB & 0x38) >> 3;

		int nIndexReg = GetRegister(enmOprandLength_Dword, byIndexReg);

		dwAddress += (int)(nIndexReg << byIndexScale);
	}

	if ( byMemoryAddressing & MEMADDR_OFFSET_8 )
	{
		BYTE byOffset = FetchByteCode();

		int nOffset = (int)(char)byOffset;

		dwAddress += nOffset;
	}

	if ( byMemoryAddressing & MEMADDR_OFFSET_32 )
	{
		DWORD dwOffset = FetchDwordCode();

		dwAddress += dwOffset;
	}

	return dwAddress;
}

DWORD CVirtualMachine::GetOprand(enmOprandLength eOprandLength)
{
	BYTE byAddressCode = FetchByteCode();

	BYTE byMode = (byAddressCode & 0xF0) >> 4;

	switch ( byMode )
	{
	case 0x00:
		{
			BYTE byReg = (byAddressCode & 0x07);

			return GetRegister(eOprandLength, byReg);
		}
		break;
	case 0x01:
		{
			BYTE bySeg = (byAddressCode & 0x07);

			return GetRegister(enmOprandLength_Word, enmWordRegister_ES + bySeg);
		}
		break;
	case 0x02:
		{
			BYTE byImmediateType = (byAddressCode & 0x0F);

			switch ( byImmediateType )
			{
			case 0x00:
				{
					switch ( eOprandLength )
					{
					case enmOprandLength_Byte:
						return FetchByteCode();
					case enmOprandLength_Word:
						return FetchWordCode();
					case enmOprandLength_Dword:
						return FetchDwordCode();
					default:
						ASSERT(FALSE);
						return 0;
					}
				}
				break;
			case 0x01:
				{
					return (DWORD)FetchByteCode();
				}
				break;
			case 0x02:
				{
					return (DWORD)FetchWordCode();
				}
				break;
			case 0x03:
				{
					return (DWORD)(int)(char)FetchByteCode();
				}
				break;
			case 0x04:
				{
					return (DWORD)(int)(short)FetchWordCode();
				}
				break;
			default:
				ASSERT(FALSE);
				return 0;
			}
		}
		break;
	case 0x03:
		{
			DWORD dwMemoryAddress = GetMemoryAddress(byAddressCode);

			return GetMemory(eOprandLength, dwMemoryAddress);
		}
		break;
	case 0x04:
		{
			int nOffset = 0;

			switch ( eOprandLength )
			{
			case enmOprandLength_Byte:
				nOffset = (int)(char)FetchByteCode();
				break;
			case enmOprandLength_Word:
				nOffset = (int)(short)FetchWordCode();
				break;
			case enmOprandLength_Dword:
				nOffset = (int)FetchDwordCode();
				break;
			default:
				ASSERT(FALSE);
				return 0;
			}

			DWORD dwEIP = GetRegister(enmOprandLength_Dword, enmDwordRegister_EIP);

			return dwEIP + (DWORD)nOffset;
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return 0;
}

void CVirtualMachine::SetOprand(enmOprandLength eOprandLength, DWORD dwValue)
{
	BYTE byAddressCode = FetchByteCode();

	BYTE byMode = (byAddressCode & 0xF0) >> 4;

	switch ( byMode )
	{
	case 0x00:
		{
			BYTE byReg = (byAddressCode & 0x07);

			SetRegister(eOprandLength, byReg, dwValue);
		}
		break;
	case 0x01:
		{
			BYTE bySeg = (byAddressCode & 0x07);

			SetRegister(enmOprandLength_Word, enmWordRegister_ES + bySeg, dwValue);
		}
		break;
	case 0x03:
		{
			DWORD dwMemoryAddress = GetMemoryAddress(byAddressCode);

			SetMemory(eOprandLength, dwMemoryAddress, dwValue);
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

DWORD CVirtualMachine::GetEffectiveAddressOfOprand()
{
	BYTE byAddressCode = FetchByteCode();

	BYTE byMode = (byAddressCode & 0xF0) >> 4;

	switch ( byMode )
	{
	case 0x03:
		{
			DWORD dwMemoryAddress = GetMemoryAddress(byAddressCode);

			return dwMemoryAddress;
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return 0;
}

int CVirtualMachine::GetSizeOfOprandLength(enmOprandLength eOprandLength)
{
	switch ( eOprandLength )
	{
	case enmOprandLength_Byte:
		return 1;
	case enmOprandLength_Word:
		return 2;
	case enmOprandLength_Dword:
		return 4;
	default:
		ASSERT(FALSE);
		return 0;
	}
}

void CVirtualMachine::PushValue(enmOprandLength eOprandLength, DWORD dwValue)
{
	DWORD dwESP = GetRegister(eOprandLength, enmDwordRegister_ESP);

	dwESP -= GetSizeOfOprandLength(eOprandLength);

	SetRegister(eOprandLength, enmDwordRegister_ESP, dwESP);

	SetMemory(eOprandLength, dwESP, dwValue);
}

DWORD CVirtualMachine::PopValue(enmOprandLength eOprandLength)
{
	DWORD dwESP = GetRegister(eOprandLength, enmDwordRegister_ESP);

	DWORD dwValue = GetMemory(eOprandLength, dwESP);
	
	dwESP += GetSizeOfOprandLength(eOprandLength);

	SetRegister(eOprandLength, enmDwordRegister_ESP, dwESP);

	return dwValue;
}

void CVirtualMachine::SetWritePosition(DWORD dwPhysicalAddress)
{
	m_dwWriteAddress = dwPhysicalAddress;
}

DWORD CVirtualMachine::GetWritePosition()
{
	return m_dwWriteAddress;
}

void CVirtualMachine::WriteByte(BYTE byCode)
{
	m_pbySystemMemory[m_dwWriteAddress] = byCode;

	m_dwWriteAddress++;
}

void CVirtualMachine::WriteWord(WORD wCode)
{
	*(WORD *)&(m_pbySystemMemory[m_dwWriteAddress]) = wCode;

	m_dwWriteAddress += 2;
}

void CVirtualMachine::WriteDword(DWORD dwCode)
{
	*(DWORD *)&(m_pbySystemMemory[m_dwWriteAddress]) = dwCode;

	m_dwWriteAddress += 4;
}

void CVirtualMachine::ClearEflagsBit(int nBit)
{
	DWORD dwEflags = GetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS);

	dwEflags &= (~(DWORD)(1 << nBit));

	SetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS, dwEflags);
}

void CVirtualMachine::SetEflagsBit(int nBit)
{
	DWORD dwEflags = GetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS);

	dwEflags |= (DWORD)(1 << nBit);

	SetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS, dwEflags);
}

bool CVirtualMachine::GetEflagsBit(int nBit)
{
	DWORD dwEflags = GetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS);

	if ( (dwEflags & (DWORD)(1 << nBit)) != 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

DWORD CVirtualMachine::InPort(enmOprandLength eOprandLength, WORD wPortNumber)
{
	switch ( eOprandLength )
	{
	case enmOprandLength_Byte:
		return m_pbyPortRegisters[wPortNumber];
	case enmOprandLength_Word:
		return *(WORD *)&(m_pbyPortRegisters[wPortNumber]);
	case enmOprandLength_Dword:
		return *(DWORD *)&(m_pbyPortRegisters[wPortNumber]);
	default:
		ASSERT(FALSE);
		return 0;
	}
}

void CVirtualMachine::OutPort(enmOprandLength eOprandLength, WORD wPortNumber, DWORD dwValue)
{
	switch ( eOprandLength )
	{
	case enmOprandLength_Byte:
		m_pbyPortRegisters[wPortNumber] = (BYTE)dwValue;
		break;
	case enmOprandLength_Word:
		*(WORD *)&(m_pbyPortRegisters[wPortNumber]) = (WORD)dwValue;
		break;
	case enmOprandLength_Dword:
		*(DWORD *)&(m_pbyPortRegisters[wPortNumber]) = (DWORD)dwValue;
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CVirtualMachine::ReadDiskSector(DWORD dwSector, DWORD dwAddress)
{
	FILE *pFileDisk = NULL;

	_wfopen_s(&pFileDisk, L"./Disk.bin", L"rb");

	ASSERT(pFileDisk);

	DWORD dwFilePointer = dwSector * SECTOR_SIZE;

	fseek(pFileDisk, dwFilePointer, SEEK_SET);

	fread(&(m_pbySystemMemory[dwAddress]), 1, SECTOR_SIZE, pFileDisk);

	fclose(pFileDisk);
}

void CVirtualMachine::PerformSimpleOperation(enmOperation eOperation, enmOprandLength eOprandLength)
{
	switch ( eOperation )
	{
	case enmOperation_NOP:
		break;
	case enmOperation_CBW:
		{
			BYTE byAL = (BYTE)GetRegister(enmOprandLength_Byte, enmByteRegister_AL);

			WORD wAX = (WORD)(SHORT)(CHAR)byAL;

			SetRegister(enmOprandLength_Word, enmWordRegister_AX, wAX);
		}
		break;
	case enmOperation_CWD:
		{
			WORD wAX = (WORD)GetRegister(enmOprandLength_Word, enmWordRegister_AX);

			WORD wDX = (wAX & 0x80) ? 0xFFFF : 0x0000;

			SetRegister(enmOprandLength_Word, enmWordRegister_DX, wDX);
		}
		break;
	case enmOperation_CLI:
		{
			ClearEflagsBit(EFLAGS_BIT_IF);
		}
		break;
	case enmOperation_STI:
		{
			SetEflagsBit(EFLAGS_BIT_IF);
		}
		break;
	case enmOperation_PUSHF:
		{
			WORD wFlags = (WORD)GetRegister(enmOprandLength_Word, enmWordRegister_FLAGS);

			PushValue(enmOprandLength_Word, wFlags);
		}
		break;
	case enmOperation_POPF:
		{
			WORD wValue = (WORD)PopValue(enmOprandLength_Word);

			SetRegister(enmOprandLength_Word, enmWordRegister_FLAGS, wValue);
		}
		break;
	case enmOperation_PUSHFD:
		{
			DWORD dwEflags = GetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS);

			PushValue(enmOprandLength_Dword, dwEflags);
		}
		break;
	case enmOperation_POPFD:
		{
			DWORD dwValue = PopValue(enmOprandLength_Dword);

			SetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS, dwValue);
		}
		break;
	}
}

void CVirtualMachine::ExecuteSimpleInstruction(enmOperation eOperation, enmOprandLength eOprandLength)
{
	PerformSimpleOperation(eOperation, eOprandLength);
}

void CVirtualMachine::PerformUnaryOperation_MUL_BYTE(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags, bool bSignedOperation)
{
	BYTE byMultiplier = (BYTE)dwOprand;

	BYTE byAL = (BYTE)GetRegister(enmOprandLength_Byte, enmByteRegister_AL);

	WORD wAX = 0;
	
	if ( bSignedOperation )
	{
		//Supports x86 configuration only
		__asm
		{
			push eax

			mov al, byAL
			imul byMultiplier
			mov wAX, ax

			pop eax
		}
	}
	else
	{
		__asm
		{
			push eax

			mov al, byAL
			mul byMultiplier
			mov wAX, ax

			pop eax
		}
	}

	SetRegister(enmOprandLength_Word, enmWordRegister_AX, wAX);
}

void CVirtualMachine::PerformUnaryOperation_MUL_WORD(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags, bool bSignedOperation)
{
	WORD wMultiplier = (WORD)dwOprand;

	WORD wAX = (WORD)GetRegister(enmOprandLength_Word, enmWordRegister_AX);

	WORD wDX = 0;
	
	if ( bSignedOperation )
	{
		__asm
		{
			push eax
			push edx

			mov ax, wAX
			imul wMultiplier
			mov wAX, ax
			mov wDX, dx

			pop edx
			pop eax
		}
	}
	else
	{
		__asm
		{
			push eax
			push edx

			mov ax, wAX
			mul wMultiplier
			mov wAX, ax
			mov wDX, dx

			pop edx
			pop eax
		}
	}

	SetRegister(enmOprandLength_Word, enmWordRegister_AX, wAX);
	SetRegister(enmOprandLength_Word, enmWordRegister_DX, wDX);
}

void CVirtualMachine::PerformUnaryOperation_MUL_DWORD(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags, bool bSignedOperation)
{
	DWORD dwMultiplier = (DWORD)dwOprand;

	DWORD dwEAX = (DWORD)GetRegister(enmOprandLength_Dword, enmDwordRegister_EAX);
	
	DWORD dwEDX = 0;
	
	if ( bSignedOperation )
	{
		__asm
		{
			push eax
			push edx

			mov eax, dwEAX
			imul dwMultiplier
			mov dwEAX, eax
			mov dwEDX, edx

			pop edx
			pop eax
		}
	}
	else
	{
		__asm
		{
			push eax
			push edx

			mov eax, dwEAX
			mul dwMultiplier
			mov dwEAX, eax
			mov dwEDX, edx

			pop edx
			pop eax
		}
	}

	SetRegister(enmOprandLength_Dword, enmDwordRegister_EAX, dwEAX);
	SetRegister(enmOprandLength_Dword, enmDwordRegister_EDX, dwEDX);
}

void CVirtualMachine::PerformUnaryOperation_DIV_BYTE(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags, bool bSignedOperation)
{
	BYTE byDivider = (BYTE)dwOprand;

	if ( byDivider == 0 )
	{
		m_bDivideOverflow = true;

		return;
	}

	WORD wAX = (WORD)GetRegister(enmOprandLength_Word, enmWordRegister_AX);
	
	if ( bSignedOperation )
	{
		__asm
		{
			push eax

			mov ax, wAX
			idiv byDivider
			mov wAX, ax

			pop eax
		}
	}
	else
	{
		__asm
		{
			push eax

			mov ax, wAX
			div byDivider
			mov wAX, ax

			pop eax
		}
	}

	SetRegister(enmOprandLength_Word, enmWordRegister_AX, wAX);
}

void CVirtualMachine::PerformUnaryOperation_DIV_WORD(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags, bool bSignedOperation)
{
	WORD wDivider = (WORD)dwOprand;

	if ( wDivider == 0 )
	{
		m_bDivideOverflow = true;

		return;
	}

	WORD wAX = (WORD)GetRegister(enmOprandLength_Word, enmWordRegister_AX);
	WORD wDX = (WORD)GetRegister(enmOprandLength_Word, enmWordRegister_DX);
	
	if ( bSignedOperation )
	{
		__asm
		{
			push eax
			push edx

			mov ax, wAX
			mov dx, wDX
			idiv wDivider
			mov wAX, ax
			mov wDX, dx

			pop edx
			pop eax
		}
	}
	else
	{
		__asm
		{
			push eax
			push edx

			mov ax, wAX
			mov dx, wDX
			div wDivider
			mov wAX, ax
			mov wDX, dx

			pop edx
			pop eax
		}
	}

	SetRegister(enmOprandLength_Word, enmWordRegister_AX, wAX);
	SetRegister(enmOprandLength_Word, enmWordRegister_DX, wDX);
}

void CVirtualMachine::PerformUnaryOperation_DIV_DWORD(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags, bool bSignedOperation)
{
	DWORD dwDivider = (DWORD)dwOprand;

	if ( dwDivider == 0 )
	{
		m_bDivideOverflow = true;

		return;
	}

	DWORD dwEAX = (DWORD)GetRegister(enmOprandLength_Dword, enmDwordRegister_EAX);
	DWORD dwEDX = (DWORD)GetRegister(enmOprandLength_Dword, enmDwordRegister_EDX);
	
	if ( bSignedOperation )
	{
		__asm
		{
			push eax
			push edx

			mov eax, dwEAX
			mov edx, dwEDX
			idiv dwDivider
			mov dwEAX, eax
			mov dwEDX, edx

			pop edx
			pop eax
		}
	}
	else
	{
		__asm
		{
			push eax
			push edx

			mov eax, dwEAX
			mov edx, dwEDX
			div dwDivider
			mov dwEAX, eax
			mov dwEDX, edx

			pop edx
			pop eax
		}
	}

	SetRegister(enmOprandLength_Dword, enmDwordRegister_EAX, dwEAX);
	SetRegister(enmOprandLength_Dword, enmDwordRegister_EDX, dwEDX);
}

void CVirtualMachine::PerformUnaryOperation(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand, DWORD &dwEflags)
{
	switch ( eOperation )
	{
	case enmOperation_INC:
		{
			dwOprand++;
		}
		break;
	case enmOperation_DEC:
		{
			dwOprand--;
		}
		break;
	case enmOperation_NEG:
		{
			dwOprand = (DWORD)(-(int)dwOprand);
		}
		break;
	case enmOperation_NOT:
		{
			dwOprand = ~dwOprand;
		}
		break;
	case enmOperation_MUL:
		{
			switch ( eOprandLength )
			{
			case enmOprandLength_Byte:
				{
					PerformUnaryOperation_MUL_BYTE(eOperation, eOprandLength, dwOprand, dwEflags, false);
				}
				break;
			case enmOprandLength_Word:
				{
					PerformUnaryOperation_MUL_WORD(eOperation, eOprandLength, dwOprand, dwEflags, false);
				}
				break;
			case enmOprandLength_Dword:
				{
					PerformUnaryOperation_MUL_DWORD(eOperation, eOprandLength, dwOprand, dwEflags, false);
				}
				break;
			default:
				ASSERT(FALSE);
				break;
			}
		}
		break;
	case enmOperation_IMUL:
		{
			switch ( eOprandLength )
			{
			case enmOprandLength_Byte:
				{
					PerformUnaryOperation_MUL_BYTE(eOperation, eOprandLength, dwOprand, dwEflags, true);
				}
				break;
			case enmOprandLength_Word:
				{
					PerformUnaryOperation_MUL_WORD(eOperation, eOprandLength, dwOprand, dwEflags, true);
				}
				break;
			case enmOprandLength_Dword:
				{
					PerformUnaryOperation_MUL_DWORD(eOperation, eOprandLength, dwOprand, dwEflags, true);
				}
				break;
			default:
				ASSERT(FALSE);
				break;
			}
		}
		break;
	case enmOperation_DIV:
		{
			switch ( eOprandLength )
			{
			case enmOprandLength_Byte:
				{
					PerformUnaryOperation_DIV_BYTE(eOperation, eOprandLength, dwOprand, dwEflags, false);
				}
				break;
			case enmOprandLength_Word:
				{
					PerformUnaryOperation_DIV_WORD(eOperation, eOprandLength, dwOprand, dwEflags, false);
				}
				break;
			case enmOprandLength_Dword:
				{
					PerformUnaryOperation_DIV_DWORD(eOperation, eOprandLength, dwOprand, dwEflags, false);
				}
				break;
			default:
				ASSERT(FALSE);
				break;
			}
		}
		break;
	case enmOperation_IDIV:
		{
			switch ( eOprandLength )
			{
			case enmOprandLength_Byte:
				{
					PerformUnaryOperation_DIV_BYTE(eOperation, eOprandLength, dwOprand, dwEflags, true);
				}
				break;
			case enmOprandLength_Word:
				{
					PerformUnaryOperation_DIV_WORD(eOperation, eOprandLength, dwOprand, dwEflags, true);
				}
				break;
			case enmOprandLength_Dword:
				{
					PerformUnaryOperation_DIV_DWORD(eOperation, eOprandLength, dwOprand, dwEflags, true);
				}
				break;
			default:
				ASSERT(FALSE);
				break;
			}
		}
		break;
	}
}

void CVirtualMachine::ExecuteUnaryInstruction(enmOperation eOperation, enmOprandLength eOprandLength)
{
	//INSTR X

	DWORD dwSavedEIP_1 = GetEIP();

	DWORD dwOprand = GetOprand(eOprandLength);

	DWORD dwEflags = GetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS);

	PerformUnaryOperation(eOperation, eOprandLength, dwOprand, dwEflags);

	SetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS, dwEflags);

	SetEIP(dwSavedEIP_1);

	SetOprand(eOprandLength, dwOprand);
}

void CVirtualMachine::ExecuteUnaryInstruction_PUSH(enmOperation eOperation, enmOprandLength eOprandLength)
{
	//PUSH X

	DWORD dwOprand1 = GetOprand(eOprandLength);

	PushValue(eOprandLength, dwOprand1);
}

void CVirtualMachine::ExecuteUnaryInstruction_POP(enmOperation eOperation, enmOprandLength eOprandLength)
{
	//POP X

	DWORD dwValue = PopValue(eOprandLength);

	SetOprand(eOprandLength, dwValue);
}

void CVirtualMachine::PerformBinaryOperation_CMP_BYTE(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand1, DWORD dwOprand2, DWORD &dwEflags)
{
	BYTE byOprand1_1 = (BYTE)dwOprand1;
	BYTE byOprand2_1 = (BYTE)dwOprand2;
	WORD wFlags = (WORD)(dwEflags & HELPER_FLAGS_MASK);

	__asm
	{
		push eax
		push ebx

		pushf
		pop ax
		and ax, ~HELPER_FLAGS_MASK
		or ax, wFlags
		push ax
		popf

		mov al, byOprand1_1
		mov bl, byOprand2_1
		cmp al, bl

		pushf
		pop wFlags

		pop ebx
		pop eax
	}

	dwEflags &= ~HELPER_FLAGS_MASK;
	dwEflags |= (DWORD)wFlags;
}

void CVirtualMachine::PerformBinaryOperation_CMP_WORD(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand1, DWORD dwOprand2, DWORD &dwEflags)
{
	WORD wOprand1_1 = (WORD)dwOprand1;
	WORD wOprand2_1 = (WORD)dwOprand2;					
	WORD wFlags = (WORD)(dwEflags & HELPER_FLAGS_MASK);

	__asm
	{
		push eax
		push ebx

		pushf
		pop ax
		and ax, ~HELPER_FLAGS_MASK
		or ax, wFlags
		push ax
		popf

		mov ax, wOprand1_1
		mov bx, wOprand2_1
		cmp ax, bx

		pushf
		pop wFlags

		pop ebx
		pop eax
	}

	dwEflags &= ~HELPER_FLAGS_MASK;
	dwEflags |= (DWORD)wFlags;
}

void CVirtualMachine::PerformBinaryOperation_CMP_DWORD(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand1, DWORD dwOprand2, DWORD &dwEflags)
{
	DWORD dwOprand1_1 = dwOprand1;
	DWORD dwOprand2_1 = dwOprand2;					
	WORD wFlags = (WORD)(dwEflags & HELPER_FLAGS_MASK);

	__asm
	{
		push eax
		push ebx

		pushf
		pop ax
		and ax, ~HELPER_FLAGS_MASK
		or ax, wFlags
		push ax
		popf

		mov eax, dwOprand1_1
		mov ebx, dwOprand2_1
		cmp eax, ebx

		pushf
		pop wFlags

		pop ebx
		pop eax
	}

	dwEflags &= ~HELPER_FLAGS_MASK;
	dwEflags |= (DWORD)wFlags;
}

void CVirtualMachine::PerformBinaryOperation(enmOperation eOperation, enmOprandLength eOprandLength, DWORD &dwOprand1, DWORD dwOprand2, DWORD &dwEflags)
{
	switch ( eOperation )
	{
	case enmOperation_MOV:
		{
			dwOprand1 = dwOprand2;
		}
		break;
	case enmOperation_ADD:
		{
			dwOprand1 += dwOprand2;
		}
		break;
	case enmOperation_SUB:
		{
			dwOprand1 -= dwOprand2;
		}
		break;
	case enmOperation_AND:
		{
			dwOprand1 &= dwOprand2;
		}
		break;
	case enmOperation_OR:
		{
			dwOprand1 |= dwOprand2;
		}
		break;
	case enmOperation_XOR:
		{
			dwOprand1 ^= dwOprand2;
		}
		break;
	case enmOperation_SHL:
		{
			dwOprand1 = dwOprand1 << dwOprand2;
		}
		break;
	case enmOperation_SHR:
		{
			dwOprand1 = dwOprand1 >> dwOprand2;
		}
		break;
	case enmOperation_SAR:
		{
			dwOprand1 = (DWORD)(((int)dwOprand1) >> dwOprand2);
		}
		break;
	case enmOperation_CMP:
		{
			switch ( eOprandLength )
			{
			case enmOprandLength_Byte:
				{
					PerformBinaryOperation_CMP_BYTE(eOperation, eOprandLength, dwOprand1, dwOprand2, dwEflags);
				}
				break;
			case enmOprandLength_Word:
				{
					PerformBinaryOperation_CMP_WORD(eOperation, eOprandLength, dwOprand1, dwOprand2, dwEflags);
				}
				break;
			case enmOprandLength_Dword:
				{
					PerformBinaryOperation_CMP_DWORD(eOperation, eOprandLength, dwOprand1, dwOprand2, dwEflags);
				}
				break;
			default:
				ASSERT(FALSE);
				break;
			}
		}
		break;
	}
}

void CVirtualMachine::ExecuteBinaryInstruction(enmOperation eOperation, enmOprandLength eOprandLength)
{
	//INSTR X,Y

	DWORD dwSavedEIP_1 = GetEIP();

	DWORD dwOprand1 = GetOprand(eOprandLength);

	DWORD dwOprand2 = GetOprand(eOprandLength);

	DWORD dwSavedEIP_2 = GetEIP();

	DWORD dwEflags = GetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS);

	PerformBinaryOperation(eOperation, eOprandLength, dwOprand1, dwOprand2, dwEflags);

	SetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS, dwEflags);

	SetEIP(dwSavedEIP_1);

	SetOprand(eOprandLength, dwOprand1);

	SetEIP(dwSavedEIP_2);
}

void CVirtualMachine::ExecuteBinaryInstruction_XCHG(enmOperation eOperation, enmOprandLength eOprandLength)
{
	//XCHG X,Y

	DWORD dwSavedEIP_1 = GetEIP();

	DWORD dwOprand1 = GetOprand(eOprandLength);

	DWORD dwOprand2 = GetOprand(eOprandLength);

	DWORD dwTemp = dwOprand1;

	dwOprand1 = dwOprand2;

	dwOprand2 = dwTemp;

	SetEIP(dwSavedEIP_1);

	SetOprand(eOprandLength, dwOprand1);

	SetOprand(eOprandLength, dwOprand2);
}

void CVirtualMachine::ExecuteBinaryInstruction_LEA(enmOperation eOperation, enmOprandLength eOprandLength)
{
	//LEA X,Y

	DWORD dwSavedEIP_1 = GetEIP();

	DWORD dwOprand1 = GetOprand(eOprandLength);

	DWORD dwEffectiveAddress = GetEffectiveAddressOfOprand();

	DWORD dwSavedEIP_2 = GetEIP();

	SetEIP(dwSavedEIP_1);

	SetOprand(eOprandLength, dwEffectiveAddress);

	SetEIP(dwSavedEIP_2);
}

void CVirtualMachine::ExecuteBinaryInstruction_IN(enmOperation eOperation, enmOprandLength eOprandLength)
{
	//IN X,Y

	DWORD dwSavedEIP_1 = GetEIP();

	DWORD dwOprand1 = GetOprand(eOprandLength);

	WORD wPortAddress = (WORD)GetOprand(eOprandLength);

	DWORD dwSavedEIP_2 = GetEIP();

	DWORD dwPortValue = InPort(eOprandLength, wPortAddress);

	SetEIP(dwSavedEIP_1);

	SetOprand(eOprandLength, dwPortValue);

	SetEIP(dwSavedEIP_2);
}

void CVirtualMachine::ExecuteBinaryInstruction_OUT(enmOperation eOperation, enmOprandLength eOprandLength)
{
	//OUT X,Y

	WORD wPortAddress = (WORD)GetOprand(eOprandLength);

	DWORD dwValue = GetOprand(eOprandLength);

	OutPort(eOprandLength, wPortAddress, dwValue);
}

void CVirtualMachine::ExecuteJumpInstruction(enmOperation eOperation, enmOprandLength eOprandLength)
{
	//JUMP X

	bool bCondition = false;

	switch ( eOperation )
	{
	case enmOperation_JMP:
		bCondition = true;
		break;
	case enmOperation_JE:
		bCondition = GetEflagsBit(EFLAGS_BIT_ZF);
		break;
	case enmOperation_JNE:
		bCondition = !GetEflagsBit(EFLAGS_BIT_ZF);
		break;
	case enmOperation_JB:
		bCondition = GetEflagsBit(EFLAGS_BIT_CF);
		break;
	case enmOperation_JNB:
		bCondition = !GetEflagsBit(EFLAGS_BIT_CF);
		break;
	case enmOperation_JA:
		bCondition = !GetEflagsBit(EFLAGS_BIT_CF) && !GetEflagsBit(EFLAGS_BIT_ZF);
		break;
	case enmOperation_JNA:
		bCondition = GetEflagsBit(EFLAGS_BIT_CF) || GetEflagsBit(EFLAGS_BIT_ZF);
		break;
	case enmOperation_JL:
		bCondition = GetEflagsBit(EFLAGS_BIT_SF) ^ GetEflagsBit(EFLAGS_BIT_OF);
		break;
	case enmOperation_JNL:
		bCondition = !(GetEflagsBit(EFLAGS_BIT_SF) ^ GetEflagsBit(EFLAGS_BIT_OF));
		break;
	case enmOperation_JG:
		bCondition = !(GetEflagsBit(EFLAGS_BIT_SF) ^ GetEflagsBit(EFLAGS_BIT_OF)) && !GetEflagsBit(EFLAGS_BIT_ZF);
		break;
	case enmOperation_JNG:
		bCondition = (GetEflagsBit(EFLAGS_BIT_SF) ^ GetEflagsBit(EFLAGS_BIT_OF)) || GetEflagsBit(EFLAGS_BIT_ZF);
		break;
	}

	DWORD dwNewEIP = GetOprand(eOprandLength);

	if ( bCondition )
	{
		SetRegister(enmOprandLength_Dword, enmDwordRegister_EIP, dwNewEIP);
	}
}

void CVirtualMachine::DoInterrupt(BYTE byInterruptNumber)
{
	if ( byInterruptNumber == 0x80 )
	{
		DoInterrupt_80H();

		return;
	}
	else if ( byInterruptNumber == 0x81 )
	{
		DoInterrupt_81H();

		return;
	}

	DWORD dwEflags = GetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS);

	PushValue(enmOprandLength_Dword, dwEflags);

	ClearEflagsBit(EFLAGS_BIT_IF);

	ClearEflagsBit(EFLAGS_BIT_TF);

	DWORD dwEIP = GetRegister(enmOprandLength_Dword, enmDwordRegister_EIP);

	PushValue(enmOprandLength_Dword, dwEIP);

	DWORD dwPhysicalAddress = byInterruptNumber * 4;

	DWORD dwNewEIP = GetMemory(enmOprandLength_Dword, dwPhysicalAddress);

	SetRegister(enmOprandLength_Dword, enmDwordRegister_EIP, dwNewEIP);
}

void CVirtualMachine::DoInterrupt_80H()
{
	DWORD dwFunctionID = GetRegister(enmOprandLength_Dword, enmDwordRegister_EAX);

	if ( dwFunctionID == 0x00000001 )
	{
		DWORD dwBufferAddress = GetRegister(enmOprandLength_Dword, enmDwordRegister_EBX);

		DWORD dwSectorNumber = GetRegister(enmOprandLength_Dword, enmDwordRegister_ECX);

		ReadDiskSector(dwSectorNumber, dwBufferAddress);
	}
}

void CVirtualMachine::DoInterrupt_81H()
{
	DWORD dwFunctionID = GetRegister(enmOprandLength_Dword, enmDwordRegister_EAX);

	if ( dwFunctionID == 0x00000001 )
	{
		DWORD dwClearColor = GetRegister(enmOprandLength_Dword, enmDwordRegister_EBX);

		CScreenWindow_GetInstance()->ClearScreen(dwClearColor);
	}
	else if ( dwFunctionID == 0x00000002 )
	{
		DWORD dwClearColor = GetRegister(enmOprandLength_Dword, enmDwordRegister_EBX);

		int nX = (int)GetRegister(enmOprandLength_Dword, enmDwordRegister_ECX);

		int nY = (int)GetRegister(enmOprandLength_Dword, enmDwordRegister_EDX);

		int nWidth = (int)GetRegister(enmOprandLength_Dword, enmDwordRegister_ESI);

		int nHeight = (int)GetRegister(enmOprandLength_Dword, enmDwordRegister_EDI);

		CScreenWindow_GetInstance()->FillRect(dwClearColor, nX, nY, nWidth, nHeight);
	}
	else if ( dwFunctionID == 0x00000003 )
	{
		CScreenWindow_GetInstance()->Invalidate();
	}
}

void CVirtualMachine::Print(bool bDisassemble)
{
	DWORD dwEAX = GetRegister(enmOprandLength_Dword, enmDwordRegister_EAX);

	DWORD dwECX = GetRegister(enmOprandLength_Dword, enmDwordRegister_ECX);

	DWORD dwEDX = GetRegister(enmOprandLength_Dword, enmDwordRegister_EDX);

	DWORD dwEBX = GetRegister(enmOprandLength_Dword, enmDwordRegister_EBX);

	DWORD dwESP = GetRegister(enmOprandLength_Dword, enmDwordRegister_ESP);

	DWORD dwEBP = GetRegister(enmOprandLength_Dword, enmDwordRegister_EBP);

	DWORD dwESI = GetRegister(enmOprandLength_Dword, enmDwordRegister_ESI);

	DWORD dwEDI = GetRegister(enmOprandLength_Dword, enmDwordRegister_EDI);

	WORD wES = (WORD)GetRegister(enmOprandLength_Word, enmWordRegister_ES);

	WORD wCS = (WORD)GetRegister(enmOprandLength_Word, enmWordRegister_CS);

	WORD wSS = (WORD)GetRegister(enmOprandLength_Word, enmWordRegister_SS);

	WORD wDS = (WORD)GetRegister(enmOprandLength_Word, enmWordRegister_DS);

	DWORD dwEIP = GetRegister(enmOprandLength_Dword, enmDwordRegister_EIP);

	DWORD dwEFLAGS = GetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS);

	printf("EAX=%08X EBX=%08X ECX=%08X EDX=%08X\n", dwEAX, dwEBX, dwECX, dwEDX);

	printf("ESP=%08X EBP=%08X ESI=%08X EDI=%08X\n", dwESP, dwEBP, dwESI, dwEDI);

	printf("DS=%04X ES=%04X SS=%04X CS=%04X EIP=%08X EFALGS=%08X ", wDS, wES, wSS, wCS, dwEIP, dwEFLAGS);

	bool bOF = GetEflagsBit(EFLAGS_BIT_OF);

	if ( bOF )
	{
		printf("O ");
	}

	bool bDF = GetEflagsBit(EFLAGS_BIT_DF);

	if ( bDF )
	{
		printf("D ");
	}

	bool bIF = GetEflagsBit(EFLAGS_BIT_IF);

	if ( bIF )
	{
		printf("I ");
	}

	bool bTF = GetEflagsBit(EFLAGS_BIT_TF);

	if ( bTF )
	{
		printf("T ");
	}

	bool bSF = GetEflagsBit(EFLAGS_BIT_SF);

	if ( bSF )
	{
		printf("S ");
	}

	bool bZF = GetEflagsBit(EFLAGS_BIT_ZF);

	if ( bZF )
	{
		printf("Z ");
	}

	bool bAF = GetEflagsBit(EFLAGS_BIT_AF);

	if ( bAF )
	{
		printf("A ");
	}

	bool bPF = GetEflagsBit(EFLAGS_BIT_PF);

	if ( bPF )
	{
		printf("P ");
	}

	bool bCF = GetEflagsBit(EFLAGS_BIT_CF);

	if ( bCF )
	{
		printf("C ");
	}

	if ( bDisassemble )
	{
		printf("\n");

		int nCodeBytes = 0;
		
		std::string szCode = CDisassembler_GetInstance()->Disassemble(dwEIP, &(m_pbySystemMemory[dwEIP]), nCodeBytes);

		printf("%s\n", szCode.c_str());
	}

	printf("\n\n");
}

BYTE *CVirtualMachine::GetSystemMemoryPointer()
{
	return m_pbySystemMemory;
}

bool CVirtualMachine::Execute()
{
	bool bContinue = true;
	
	BYTE byInstrCode = FetchByteCode();

	enmOprandLength eOprandLength = GetOprandLength(byInstrCode);

	enmOperation eOperation = (enmOperation)byInstrCode;

	switch ( eOperation )
	{
	case enmOperation_NOP:
	case enmOperation_CBW:
	case enmOperation_CWD:
	case enmOperation_CLI:
	case enmOperation_STI:
	case enmOperation_PUSHF:
	case enmOperation_POPF:
	case enmOperation_PUSHFD:
	case enmOperation_POPFD:
		{
			//INSTR

			ExecuteSimpleInstruction(eOperation, eOprandLength);
		}
		break;
	case enmOperation_HLT:
		{
			//HLT

			bContinue = false;
		}
		break;
	case enmOperation_RET:
		{
			//RET

			DWORD dwNewEIP = PopValue(enmOprandLength_Dword);

			SetRegister(enmOprandLength_Dword, enmDwordRegister_EIP, dwNewEIP);
		}
		break;
	case enmOperation_IRET:
		{
			//IRET

			DWORD dwNewEIP = PopValue(enmOprandLength_Dword);

			DWORD dwNewFlags = PopValue(enmOprandLength_Dword);

			SetRegister(enmOprandLength_Dword, enmDwordRegister_EFLAGS, dwNewFlags);

			SetRegister(enmOprandLength_Dword, enmDwordRegister_EIP, dwNewEIP);
		}
		break;
	case enmOperation_INT_3:
		{
			//INT 3

			BYTE byInterruptNumber = 3;

			DoInterrupt(byInterruptNumber);
		}
		break;
	default: //位1和位0为字长标志的指令统一在下面处理
		{
			byInstrCode &= 0xFC;

			eOperation = (enmOperation)byInstrCode;

			switch ( eOperation )
			{
			case enmOperation_PUSH:
				{
					//PUSH X

					ExecuteUnaryInstruction_PUSH(eOperation, eOprandLength);
				}
				break;
			case enmOperation_POP:
				{
					//POP X

					ExecuteUnaryInstruction_POP(eOperation, eOprandLength);
				}
				break;
			case enmOperation_INC:
			case enmOperation_DEC:
			case enmOperation_NEG:
			case enmOperation_NOT:
			case enmOperation_MUL:
			case enmOperation_IMUL:
			case enmOperation_DIV:
			case enmOperation_IDIV:
				{
					//INSTR X

					ExecuteUnaryInstruction(eOperation, eOprandLength);
				}
				break;
			case enmOperation_MOV:
			case enmOperation_ADD:
			case enmOperation_SUB:
			case enmOperation_AND:
			case enmOperation_OR:
			case enmOperation_XOR:
			case enmOperation_SHL:
			case enmOperation_SHR:
			case enmOperation_SAR:
			case enmOperation_CMP:
				{
					//INSTR X,Y

					ExecuteBinaryInstruction(eOperation, eOprandLength);
				}
				break;
			case enmOperation_XCHG:
				{
					//XCHG X,Y

					ExecuteBinaryInstruction_XCHG(eOperation, eOprandLength);
				}
				break;
			case enmOperation_LEA:
				{
					//LEA X,Y

					ExecuteBinaryInstruction_LEA(eOperation, eOprandLength);
				}
				break;
			case enmOperation_IN:
				{
					//IN X,Y

					ExecuteBinaryInstruction_IN(eOperation, eOprandLength);
				}
				break;
			case enmOperation_OUT:
				{
					//OUT X,Y

					ExecuteBinaryInstruction_OUT(eOperation, eOprandLength);
				}
				break;
			case enmOperation_JMP:
			case enmOperation_JE:
			case enmOperation_JNE:
			case enmOperation_JB:
			case enmOperation_JNB:
			case enmOperation_JA:
			case enmOperation_JNA:
			case enmOperation_JL:
			case enmOperation_JNL:
			case enmOperation_JG:
			case enmOperation_JNG:
				{
					//JUMP X

					ExecuteJumpInstruction(eOperation, eOprandLength);
				}
				break;
			case enmOperation_CALL:
				{
					//CALL X

					DWORD dwNewEIP = GetOprand(eOprandLength);

					DWORD dwEIP = GetRegister(enmOprandLength_Dword, enmDwordRegister_EIP);

					PushValue(enmOprandLength_Dword, dwEIP);

					SetRegister(enmOprandLength_Dword, enmDwordRegister_EIP, dwNewEIP);
				}
				break;
			case enmOperation_INT:
				{
					//INT X

					BYTE byInterruptNumber = (BYTE)GetOprand(eOprandLength);

					DoInterrupt(byInterruptNumber);
				}
				break;
			default:
				ASSERT(FALSE);
				break;
			}
		}
		break;
	}

	if ( m_bDivideOverflow )
	{
		BYTE byInterruptNumber = 0;

		DoInterrupt(byInterruptNumber);

		m_bDivideOverflow = false;
	}

	return bContinue;
}

