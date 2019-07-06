//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "CAsmCodeGenerator.hpp"

tagAsmOprandNodeStateBlock::tagAsmOprandNodeStateBlock(void)
{
	memset(this, 0, sizeof(tagAsmOprandNodeStateBlock));
}

CAsmCodeGenerator::CAsmCodeGenerator(void)
{
	m_dwOriginalAddress = 0x00000400;
	m_dwInstrStartAddress = m_dwOriginalAddress;
	m_bError = FALSE;
	m_bDisplaySyntaxTree = FALSE;
	m_bFirstOrgStatement = TRUE;
}

CAsmCodeGenerator::~CAsmCodeGenerator(void)
{

}

void CAsmCodeGenerator::OpenBinCodeFile(const WCHAR *pszFilename)
{
	m_pBinFile = NULL;
	_wfopen_s(&m_pBinFile, pszFilename, L"wb");
}

void CAsmCodeGenerator::CloseBinCodeFile(void)
{
	if ( m_pBinFile )
	{
		fclose(m_pBinFile);
	}
}

int CAsmCodeGenerator::GetBinCodeEmitPos(void)
{
	if ( m_pBinFile )
	{
		return ftell(m_pBinFile);
	}

	return 0;
}

void CAsmCodeGenerator::SetBinCodeEmitPos(int nPos)
{
	if ( m_pBinFile )
	{
		fseek(m_pBinFile, nPos, SEEK_SET);
	}
}

void CAsmCodeGenerator::PrintHexData(void *pData, int nLength)
{
	int i;

	for ( i = 0; i < nLength; i++ )
	{
		printf("%02X ", ((BYTE *)pData)[i]);
	}
}

void CAsmCodeGenerator::GetBinCodeAt(int nPos, void *pData, int nLength)
{
	if ( m_pBinFile )
	{
		int nSavedPos = ftell(m_pBinFile);
		fseek(m_pBinFile, nPos, SEEK_SET);
		fread(pData, sizeof(BYTE), nLength, m_pBinFile);
		fseek(m_pBinFile, nSavedPos, SEEK_SET);
	}
}

void CAsmCodeGenerator::EmitBinCodeAt(int nPos, void *pData, int nLength)
{
	if ( m_pBinFile )
	{
		int nSavedPos = ftell(m_pBinFile);
		fseek(m_pBinFile, nPos, SEEK_SET);
		fwrite(pData, sizeof(BYTE), nLength, m_pBinFile);
		fseek(m_pBinFile, nSavedPos, SEEK_SET);
	}
}

void CAsmCodeGenerator::EmitBinCode(void *pData, int nLength)
{
	if ( m_pBinFile )
	{
		fwrite(pData, sizeof(BYTE), nLength, m_pBinFile);

		PrintHexData(pData, nLength);
	}
	m_dwOriginalAddress += nLength;
}

void CAsmCodeGenerator::EmitBinCode_Byte(BYTE byData)
{
	EmitBinCode(&byData, sizeof(byData));
}

void CAsmCodeGenerator::EmitBinCode_Word(WORD wData)
{
	EmitBinCode(&wData, sizeof(wData));
}

void CAsmCodeGenerator::EmitBinCode_Dword(DWORD dwData)
{
	EmitBinCode(&dwData, sizeof(dwData));
}

void CAsmCodeGenerator::EmitBinCode_Byte_NotPrint(BYTE byData)
{
	if ( m_pBinFile )
	{
		fwrite(&byData, sizeof(BYTE), 1, m_pBinFile);
	}
	m_dwOriginalAddress += 1;
}

void CAsmCodeGenerator::EmitStatementStart(void)
{
	printf("%08X: ", m_dwOriginalAddress);
}

void CAsmCodeGenerator::EmitStatementEnd(void)
{
	printf("\n");
}

void CAsmCodeGenerator::Error(const char *pszFormat, ...)
{
	va_list args;
	char szBuffer[1000];

	va_start(args, pszFormat);

	vsprintf_s(szBuffer, sizeof(szBuffer), pszFormat, args);

	printf("%s", szBuffer);

	m_bError = TRUE;

	va_end(args);
}

void CAsmCodeGenerator::GenerateCode(CAsmSyntaxTree *pTree)
{
	m_dwOriginalAddress = 0x00000400;
	m_dwInstrStartAddress = m_dwOriginalAddress;
	m_bError = FALSE;

	GenerateCode_Recursive(pTree);
}

bool CAsmCodeGenerator::GetError()
{
	return (m_bError != FALSE);
}

void CAsmCodeGenerator::GenerateCode_Recursive(CAsmSyntaxTree *pTree)
{
	if ( pTree == NULL )
	{
		return;
	}

	std::string szType = pTree->GetType();

	if ( szType == "StatementList" )
	{
		GenerateCode_StatementList(pTree);
	}
	else if ( szType == "DataDefine" )
	{
		GenerateCode_DataDefine(pTree);
	}	
	else if ( szType == "Label" )
	{
		GenerateCode_Label(pTree);
	}	
	else if ( szType == "EndProc" )
	{
		GenerateCode_EndProc(pTree);
	}
	else if ( szType == "Statement" )
	{
		GenerateCode_Statement(pTree);
	}
}

void CAsmCodeGenerator::GenerateCode_Children(CAsmSyntaxTree *pTree)
{
	int nChildCount = pTree->GetChildCount();

	int i;
	for ( i = 0; i < nChildCount; i++ )
	{
		GenerateCode_Recursive(pTree->GetChild(i));
	}
}

void CAsmCodeGenerator::GenerateCode_Other(CAsmSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);
}

void CAsmCodeGenerator::GenerateCode_StatementList(CAsmSyntaxTree *pTree)
{
	GenerateCode_Children(pTree);
}

void CAsmCodeGenerator::GenerateCode_DataDefine(CAsmSyntaxTree *pTree)
{
	EmitStatementStart();

	int nChildCount = pTree->GetChildCount();

	std::string szLength = pTree->GetAttrValue("Length");

	int i;
	for ( i = 0; i < nChildCount; i++ )
	{
		CAsmSyntaxTree *pChild = pTree->GetChild(i);

		std::string szType = pChild->GetType();

		if ( szType == "Number" )
		{
			std::string szLength1 = pChild->GetAttrValue("Length");

			int i1 = GetValueOfNumberNode(pChild);

			if ( szLength == "byte" )
			{
				if ( IsOprandNodeByte(pChild) == FALSE )
				{
					Error("%d:Invalid oprand size.\n", pChild->GetLineNumber());
				}
				EmitBinCode_Byte(i1);
			}
			else if ( szLength == "word" )
			{
				if ( IsOprandNodeByte(pChild) == FALSE && IsOprandNodeWord(pChild) == FALSE )
				{
					Error("%d:Invalid oprand size.\n", pChild->GetLineNumber());
				}
				EmitBinCode_Word(i1);
			}
			else if ( szLength == "dword" )
			{
				EmitBinCode_Dword(i1);
			}
		}
	}

	EmitStatementEnd();
}

void CAsmCodeGenerator::GenerateCode_Label(CAsmSyntaxTree *pTree)
{
	if ( m_bDisplaySyntaxTree )
	{
		pTree->Print();
	}

	std::string szLabel = pTree->GetAttrValue("Label");

	CAsmSymbolItem *pSymbolItem = CAsmSymbolTable_GetInstance()->SearchSymbolItem(szLabel.c_str());

	if ( pSymbolItem )
	{
		Error("%d:Label redifination.\n", pTree->GetLineNumber());
		return;
	}

	CAsmSymbolItem *pSymbolItem1=new CAsmSymbolItem();
	pSymbolItem1->m_szName = szLabel;
	pSymbolItem1->m_szType = "label";
	pSymbolItem1->m_nAddress = m_dwOriginalAddress;
	CAsmSymbolTable_GetInstance()->AddSymbolItem(pSymbolItem1);

	GenerateCode_UngeneratedJumpStatements(szLabel.c_str());
}

void CAsmCodeGenerator::GenerateCode_EndProc(CAsmSyntaxTree *pTree)
{
	if ( m_bDisplaySyntaxTree )
	{
		pTree->Print();
	}

	CAsmSymbolTable_GetInstance()->ClearLocalSymbols();
}

void CAsmCodeGenerator::GenerateCode_UngeneratedJumpStatements(const char *pLabel)
{
	int i;

	int n = CAsmLabelRefTable_GetInstance()->GetLabelRefCount();

	for ( i = 0; i < n; i++ )
	{
		CAsmLabelRef *pLabelRef = CAsmLabelRefTable_GetInstance()->GetLabelRef(i);
		if ( strcmp(pLabelRef->m_szLabel.c_str(), pLabel) == 0 )
		{
			int nSavedOriginalAddress = this->m_dwOriginalAddress;
			this->m_dwOriginalAddress = pLabelRef->m_nAddress;

			int nBinCodeEmitPos = GetBinCodeEmitPos();
			SetBinCodeEmitPos(pLabelRef->m_nBinCodeEmitPos);

			GenerateCode_Statement(pLabelRef->m_pTree);

			SetBinCodeEmitPos(nBinCodeEmitPos);

			this->m_dwOriginalAddress = nSavedOriginalAddress;
		}
	}

	CAsmLabelRefTable_GetInstance()->RemoveLabelRefsOfLabel(pLabel);
}

void CAsmCodeGenerator::GenerateCode_Statement(CAsmSyntaxTree *pTree)
{
	if ( m_bDisplaySyntaxTree )
	{
		pTree->Print();
	}

	m_dwInstrStartAddress = m_dwOriginalAddress;

	std::string szInstr = pTree->GetAttrValue("Instr");

	if ( szInstr == "org" )
	{
		GenerateCode_Statement_Org(pTree);
		return;
	}

	CAsmSymbolFileGenerator_GetInstance()->AddSymbolFileItem(m_dwInstrStartAddress, pTree->GetLineNumber());

	CAsmInstrItemList *pAsmInstrItemList = CAsmInstrTable_GetInstance()->FindInstrItemListOfInstr(szInstr.c_str());

	if ( pAsmInstrItemList == NULL )
	{
		Error("%d:Instruction %s not supported.\n", pTree->GetLineNumber(), szInstr.c_str());
		return;
	}

	EmitStatementStart();

	int i;

	BOOL bCodeEmitted = FALSE;

	CAsmSyntaxTree *pLeft = pTree->GetChildByTag("Left");

	CAsmSyntaxTree *pRight = pTree->GetChildByTag("Right");

	tagAsmOprandNodeStateBlock stOprandNodeStateBlock;

	if ( pLeft )
	{
		DetermingLabelRefDistance(pLeft);

		stOprandNodeStateBlock.bIsLeftOprandNodeByte = IsOprandNodeByte(pLeft);
		stOprandNodeStateBlock.bIsLeftOprandNodeWord = IsOprandNodeWord(pLeft);
		stOprandNodeStateBlock.bIsLeftOprandNodeDword = IsOprandNodeDword(pLeft);
	}

	if ( pRight )
	{
		stOprandNodeStateBlock.bIsRightOprandNodeByte = IsOprandNodeByte(pRight);
		stOprandNodeStateBlock.bIsRightOprandNodeWord = IsOprandNodeWord(pRight);
		stOprandNodeStateBlock.bIsRightOprandNodeDword = IsOprandNodeDword(pRight);
	}	

	for ( i = 0; i < (int)pAsmInstrItemList->m_vecInstrItemList.size(); i++ )
	{
		CAsmInstrItem *pAsmInstrItem = &(pAsmInstrItemList->m_vecInstrItemList[i]);

		switch(pAsmInstrItem->m_eType)
		{
		case enmAsmInstrType_NoneOprand:
			bCodeEmitted = GenerateCode_Statement_NoneOprand(pAsmInstrItem, pTree, &stOprandNodeStateBlock);
			break;
		case enmAsmInstrType_SingleOprand:
			bCodeEmitted = GenerateCode_Statement_SingleOprand(pAsmInstrItem, pTree, &stOprandNodeStateBlock);
			break;
		case enmAsmInstrType_BinaryOprand:
			bCodeEmitted = GenerateCode_Statement_BinaryOprand(pAsmInstrItem, pTree, &stOprandNodeStateBlock);
			break;
		}

		if ( bCodeEmitted )
		{
			break;
		}
	}

	EmitStatementEnd();

	if ( pAsmInstrItemList == NULL )
	{
		Error("Error(%d):Unknown instructions or parameters.\n", pTree->GetLineNumber());
		return;
	}

	if ( !bCodeEmitted )
	{
		Error("Error(%d):Generate byCode failed.\n", pTree->GetLineNumber());
		return;
	}
}

BOOL CAsmCodeGenerator::GenerateCode_Statement_NoneOprand(CAsmInstrItem *pAsmInstrItem, CAsmSyntaxTree *pTree, tagAsmOprandNodeStateBlock *pOprandNodeStateBlock)
{
	CAsmSyntaxTree *pLeft = pTree->GetChildByTag("Left");
	CAsmSyntaxTree *pRight = pTree->GetChildByTag("Right");

	if ( pLeft != NULL || pRight != NULL )
	{
		return FALSE;
	}

	EmitBinCode_Byte(pAsmInstrItem->m_byCode);
	if ( pAsmInstrItem->m_eAttr & enmAsmInstrAttr_SubCode )
	{
		EmitBinCode_Byte(pAsmInstrItem->m_bySubCode);
	}

	return TRUE;
}

BOOL CAsmCodeGenerator::GenerateCode_Statement_SingleOprand(CAsmInstrItem *pAsmInstrItem, CAsmSyntaxTree *pTree, tagAsmOprandNodeStateBlock *pOprandNodeStateBlock)
{
	CAsmSyntaxTree *pLeft = pTree->GetChildByTag("Left");
	CAsmSyntaxTree *pRight = pTree->GetChildByTag("Right");

	if ( pLeft == NULL || pRight != NULL )
	{
		return FALSE;
	}

	BOOL bIsOprandNodeByte = pOprandNodeStateBlock->bIsLeftOprandNodeByte;
	BOOL bIsOprandNodeWord = pOprandNodeStateBlock->bIsLeftOprandNodeWord;
	BOOL bIsOprandNodeDword = pOprandNodeStateBlock->bIsLeftOprandNodeDword;

	if (  pAsmInstrItem->m_eOprand1Type == enmAsmOprandType_General &&
		pAsmInstrItem->m_eOprand1Attr == enmAsmOprandAttr_Byte)
	{
		if ( bIsOprandNodeByte )
		{
			BYTE byCode = pAsmInstrItem->m_byCode;

			byCode |= 0x00;		//BYTE

			EmitBinCode_Byte(byCode);

			GenerateCode_Oprand(pTree, pLeft, enmAsmWordLength_Byte);

			return TRUE;
		}
	}

	if (  pAsmInstrItem->m_eOprand1Type == enmAsmOprandType_General&&		
		pAsmInstrItem->m_eOprand1Attr == enmAsmOprandAttr_Word)
	{
		if ( bIsOprandNodeWord )
		{
			BYTE byCode = pAsmInstrItem->m_byCode;

			byCode |= 0x01;		//WORD

			EmitBinCode_Byte(byCode);

			GenerateCode_Oprand(pTree, pLeft, enmAsmWordLength_Word);

			return TRUE;
		}
	}

	if (  pAsmInstrItem->m_eOprand1Type == enmAsmOprandType_General&&		
		pAsmInstrItem->m_eOprand1Attr == enmAsmOprandAttr_Dword)
	{
		if ( bIsOprandNodeDword )
		{
			BYTE byCode = pAsmInstrItem->m_byCode;

			byCode |= 0x02;		//DWORD

			EmitBinCode_Byte(byCode);

			GenerateCode_Oprand(pTree, pLeft, enmAsmWordLength_Dword);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CAsmCodeGenerator::GenerateCode_Statement_BinaryOprand(CAsmInstrItem *pAsmInstrItem, CAsmSyntaxTree *pTree, tagAsmOprandNodeStateBlock *pOprandNodeStateBlock)
{
	CAsmSyntaxTree *pLeft = pTree->GetChildByTag("Left");
	CAsmSyntaxTree *pRight = pTree->GetChildByTag("Right");

	if ( pLeft == NULL || pRight == NULL )
	{
		return FALSE;
	}

	BOOL bIsLeftOprandNodeByte = pOprandNodeStateBlock->bIsLeftOprandNodeByte;
	BOOL bIsLeftOprandNodeWord = pOprandNodeStateBlock->bIsLeftOprandNodeWord;
	BOOL bIsLeftOprandNodeDword = pOprandNodeStateBlock->bIsLeftOprandNodeDword;

	BOOL bIsRightOprandNodeByte = pOprandNodeStateBlock->bIsRightOprandNodeByte;
	BOOL bIsRightOprandNodeWord = pOprandNodeStateBlock->bIsRightOprandNodeWord;
	BOOL bIsRightOprandNodeDword = pOprandNodeStateBlock->bIsRightOprandNodeDword;

	if ( pAsmInstrItem->m_eOprand1Type == enmAsmOprandType_General &&
		 pAsmInstrItem->m_eOprand1Attr == enmAsmOprandAttr_Byte &&
		 pAsmInstrItem->m_eOprand2Type == enmAsmOprandType_General &&
		 pAsmInstrItem->m_eOprand2Attr == enmAsmOprandAttr_Byte )
	{
		if ( bIsLeftOprandNodeByte && bIsRightOprandNodeByte )
		{
			BYTE byCode = pAsmInstrItem->m_byCode;

			byCode |= 0x00;		//BYTE

			EmitBinCode_Byte(byCode);

			GenerateCode_Oprand(pTree, pLeft, enmAsmWordLength_Byte);

			GenerateCode_Oprand(pTree, pRight, enmAsmWordLength_Byte);

			return TRUE;
		}
	}

	if ( pAsmInstrItem->m_eOprand1Type == enmAsmOprandType_General &&
		 pAsmInstrItem->m_eOprand1Attr == enmAsmOprandAttr_Word &&
		 pAsmInstrItem->m_eOprand2Type == enmAsmOprandType_General &&
		 pAsmInstrItem->m_eOprand2Attr == enmAsmOprandAttr_Word )
	{
		if ( bIsLeftOprandNodeWord && bIsRightOprandNodeWord )
		{
			BYTE byCode = pAsmInstrItem->m_byCode;

			byCode |= 0x01;		//WORD

			EmitBinCode_Byte(byCode);

			GenerateCode_Oprand(pTree, pLeft, enmAsmWordLength_Word);

			GenerateCode_Oprand(pTree, pRight, enmAsmWordLength_Word);

			return TRUE;
		}
	}

	if ( pAsmInstrItem->m_eOprand1Type == enmAsmOprandType_General &&
		 pAsmInstrItem->m_eOprand1Attr == enmAsmOprandAttr_Dword &&
		 pAsmInstrItem->m_eOprand2Type == enmAsmOprandType_General &&
		 pAsmInstrItem->m_eOprand2Attr == enmAsmOprandAttr_Dword )
	{
		if ( bIsLeftOprandNodeDword && bIsRightOprandNodeDword )
		{
			BYTE byCode = pAsmInstrItem->m_byCode;

			byCode |= 0x02;		//DWORD

			EmitBinCode_Byte(byCode);

			GenerateCode_Oprand(pTree, pLeft, enmAsmWordLength_Dword);

			GenerateCode_Oprand(pTree, pRight, enmAsmWordLength_Dword);

			return TRUE;
		}
	}

	return FALSE;
}

void CAsmCodeGenerator::GenerateCode_Immediate(CAsmSyntaxTree *pTree, enmAsmWordLength eInstrWordLength)
{
	bool bNumberNodeSigned = IsNumberNodeSigned(pTree);

	DWORD dwValue = (DWORD)GetValueOfNumberNode(pTree);

	enmAsmWordLength eNumberNodeWordLength = GetNumberNodeWordLength(pTree, bNumberNodeSigned, dwValue);

	BYTE byMode = 0x02;

	if ( eNumberNodeWordLength == eInstrWordLength )
	{
		BYTE byImmediateType = 0x00;

		BYTE byAddressing = ((byMode << 4) | byImmediateType);

		EmitBinCode_Byte(byAddressing);

		switch ( eInstrWordLength )
		{
		case enmAsmWordLength_Byte:
			{
				EmitBinCode_Byte((BYTE)dwValue);
			}
			break;
		case enmAsmWordLength_Word:
			{
				EmitBinCode_Word((WORD)dwValue);
			}
			break;
		case enmAsmWordLength_Dword:
			{
				EmitBinCode_Dword((DWORD)dwValue);
			}
			break;
		}
	}
	else if ( eNumberNodeWordLength < eInstrWordLength )
	{	
		if ( !bNumberNodeSigned )
		{
			if ( eNumberNodeWordLength == enmAsmWordLength_Byte )
			{
				BYTE byImmediateType = 0x01;

				BYTE byAddressing = ((byMode << 4) | byImmediateType);

				EmitBinCode_Byte(byAddressing);

				EmitBinCode_Byte((BYTE)dwValue);
			}
			else if ( eNumberNodeWordLength == enmAsmWordLength_Word )
			{
				BYTE byImmediateType = 0x02;

				BYTE byAddressing = ((byMode << 4) | byImmediateType);

				EmitBinCode_Byte(byAddressing);

				EmitBinCode_Word((WORD)dwValue);
			}
		}
		else
		{
			if ( eNumberNodeWordLength == enmAsmWordLength_Byte )
			{
				BYTE byImmediateType = 0x03;

				BYTE byAddressing = ((byMode << 4) | byImmediateType);

				EmitBinCode_Byte(byAddressing);

				EmitBinCode_Byte((BYTE)dwValue);
			}
			else if ( eNumberNodeWordLength == enmAsmWordLength_Word )
			{
				BYTE byImmediateType = 0x04;

				BYTE byAddressing = ((byMode << 4) | byImmediateType);

				EmitBinCode_Byte(byAddressing);

				EmitBinCode_Word((WORD)dwValue);
			}
		}
	}
}

BOOL CAsmCodeGenerator::GenerateCode_Oprand(CAsmSyntaxTree *pParent, CAsmSyntaxTree *pTree, enmAsmWordLength eWordLength)
{
	if ( pTree->GetType() == "Register" )
	{
		BYTE byMode = 0x00;

		BYTE byReg = GetCodeOfRegisterNode(pTree);

		BYTE byAddressing = (byMode << 4 | byReg);

		EmitBinCode_Byte(byAddressing);
	}
	else if ( pTree->GetType() == "Number" )
	{
		enmAsmWordLength eInstrWordLength = eWordLength;
		
		GenerateCode_Immediate(pTree, eInstrWordLength);
	}
	else if ( pTree->GetType() == "Memory" )
	{
		std::string szBaseRegister = pTree->GetAttrValue("BaseRegister");

		std::string szIndexRegister = pTree->GetAttrValue("IndexRegister");

		std::string szScale = pTree->GetAttrValue("Scale");

		int nOffset = 0;

		std::string szLabelOffset = pTree->GetAttrValue("LabelOffset");

		if ( szLabelOffset == "" )
		{
			std::string szOffset = pTree->GetAttrValue("Offset");
			int i = 0;
			BOOL b1 = GetValueOfNumberString_Asm((char *)szOffset.c_str(), &i);
			nOffset = i;
		}
		else
		{
			CAsmSymbolItem *pSymbolItem=CAsmSymbolTable_GetInstance()->SearchSymbolItem(szLabelOffset.c_str());

			if ( pSymbolItem == NULL )
			{
				ASSERT(FALSE);

				return FALSE;
			}

			nOffset = pSymbolItem->m_nAddress;
		}

		BYTE byMode = 0x03;

		if ( szBaseRegister == "" && szIndexRegister == "" )
		{
			BYTE byMemoryMode = 0x00;

			BYTE byAddressing = (byMode << 4 | byMemoryMode);

			EmitBinCode_Byte(byAddressing);

			EmitBinCode_Dword((DWORD)nOffset);

			return TRUE;
		}

		//Addressing Byte

		BYTE byMemoryMode = 0x01;

		if ( szBaseRegister != "" && szIndexRegister == "" )
		{
			if ( nOffset == 0 )
			{
				byMemoryMode = 0x01;
			}
			else if ( nOffset != 0 && nOffset <= SCHAR_MAX && nOffset >= SCHAR_MIN )
			{
				byMemoryMode = 0x02;
			}
			else
			{
				byMemoryMode = 0x03;
			}
		}
		else if ( szBaseRegister == "" && szIndexRegister != "" )
		{
			if ( nOffset == 0 )
			{
				byMemoryMode = 0x04;
			}
			else if ( nOffset != 0 && nOffset <= SCHAR_MAX && nOffset >= SCHAR_MIN )
			{
				byMemoryMode = 0x05;
			}
			else
			{
				byMemoryMode = 0x06;
			}
		}
		else if ( szBaseRegister != "" && szIndexRegister != "" )
		{
			if ( nOffset == 0 )
			{
				byMemoryMode = 0x07;
			}
			else if ( nOffset != 0 && nOffset <= SCHAR_MAX && nOffset >= SCHAR_MIN )
			{
				byMemoryMode = 0x08;
			}
			else
			{
				byMemoryMode = 0x09;
			}
		}

		BYTE byAddressing = (byMode << 4 | byMemoryMode);

		EmitBinCode_Byte(byAddressing);

		//SIB Byte

		BYTE byBaseReg = 0;

		if ( szBaseRegister != "" )
		{
			byBaseReg = GetCodeOfRegisterString(szBaseRegister.c_str());
		}

		BYTE byIndexReg = 0;

		if ( szIndexRegister != "" )
		{
			byIndexReg = GetCodeOfRegisterString(szIndexRegister.c_str());
		}

		BYTE byIndexRegScale = 0;

		if ( szScale != "" )
		{
			byIndexRegScale = GetCodeOfRegisterScale(szScale.c_str());
		}

		BYTE bySIB = byBaseReg | (byIndexReg << 3) | (byIndexRegScale << 6);

		EmitBinCode_Byte(bySIB);

		//Offset

		if ( nOffset == 0 )
		{
		}
		else if ( nOffset != 0 && nOffset <= SCHAR_MAX && nOffset >= SCHAR_MIN )
		{
			EmitBinCode_Byte((BYTE)(char)nOffset);
		}
		else
		{
			EmitBinCode_Dword(nOffset);
		}
	}
	else if ( pTree->GetType() == "LabelRef" )
	{
		BYTE byMode = 0x04;
		BYTE byAddressing = (byMode << 4);
		EmitBinCode_Byte(byAddressing);

		switch ( eWordLength )
		{
		case enmAsmWordLength_Byte:
			{
				int nLabelAddress = 0;
				BOOL b1 = GetAddressOfLabelRef(pTree, &nLabelAddress);

				if ( b1 == FALSE )
				{
					AddNotFoundLebelRefJumpStatement(pParent);

					EmitBinCode_Byte(ASM_CODE_NOP);

					return TRUE;
				}

				int nInstrEndAddress = m_dwOriginalAddress + 1;

				int nOffsetAddress = nLabelAddress - nInstrEndAddress;

				if ( nOffsetAddress < -128 || nOffsetAddress > 127 )
				{
					Error("%d:Short instr references a too far label.", pTree->GetLineNumber());
					return FALSE;
				}

				EmitBinCode_Byte((BYTE)nOffsetAddress);
			}
			break;
		case enmAsmWordLength_Word:
			{
				int nLabelAddress = 0;
				BOOL b1 = GetAddressOfLabelRef(pTree, &nLabelAddress);

				if ( b1 == FALSE )
				{
					AddNotFoundLebelRefJumpStatement(pParent);

					EmitBinCode_Byte(ASM_CODE_NOP);

					EmitBinCode_Byte(ASM_CODE_NOP);

					return TRUE;
				}

				int nInstrEndAddress = m_dwOriginalAddress + 2;

				int nOffsetAddress = nLabelAddress - nInstrEndAddress;

				if ( nOffsetAddress < -32768 || nOffsetAddress > 32767 )
				{
					Error("%d:Near instr references a too far label.", pTree->GetLineNumber());
					return FALSE;
				}

				EmitBinCode_Word((WORD)nOffsetAddress);
			}
			break;
		case enmAsmWordLength_Dword:
			{
				int nLabelAddress = 0;
				BOOL b1 = GetAddressOfLabelRef(pTree, &nLabelAddress);

				if ( b1 == FALSE )
				{
					AddNotFoundLebelRefJumpStatement(pParent);

					EmitBinCode_Byte(ASM_CODE_NOP);

					EmitBinCode_Byte(ASM_CODE_NOP);

					EmitBinCode_Byte(ASM_CODE_NOP);

					EmitBinCode_Byte(ASM_CODE_NOP);

					return FALSE;
				}

				int nInstrEndAddress = m_dwOriginalAddress + 4;

				int nOffsetAddress = nLabelAddress - nInstrEndAddress;

				EmitBinCode_Dword((DWORD)nOffsetAddress);
			}
			break;
		}
	}

	return TRUE;
}

void CAsmCodeGenerator::GenerateCode_Statement_Org(CAsmSyntaxTree *pTree)
{
	CAsmSyntaxTree *pLeft = pTree->GetChildByTag("Left");
	CAsmSyntaxTree *pRight = pTree->GetChildByTag("Right");

	if ( pLeft == NULL || pRight != NULL || pLeft->GetType() != "Number" )
	{
		Error("%d:Invalid parameter.\n", pTree->GetLineNumber());
		return;
	}

	if ( IsOprandNodeByte(pLeft) == FALSE && IsOprandNodeWord(pLeft) == FALSE )
	{
		Error("%d:Invalid oprand size.\n", pTree->GetLineNumber());
		return;
	}

	if ( m_bFirstOrgStatement )
	{
		this->m_dwOriginalAddress = (WORD)GetValueOfNumberNode(pLeft);

		m_bFirstOrgStatement = FALSE;
	}
	else
	{
		int nOriginalAddress1 = (WORD)GetValueOfNumberNode(pLeft);

		int i;
		int n = nOriginalAddress1 - m_dwOriginalAddress;
		for ( i = 0; i < n; i++ )
		{
			EmitBinCode_Byte_NotPrint(0);
		}
	}
}

enmAsmWordLength CAsmCodeGenerator::GetNumberNodeWordLength(CAsmSyntaxTree *pTree, bool bIsNumberNodeSigned, DWORD dwValue)
{
	if ( bIsNumberNodeSigned )
	{
		INT nValue = (INT)dwValue;

		if ( nValue <= SCHAR_MAX && nValue >= SCHAR_MIN )
		{
			return enmAsmWordLength_Byte;
		}

		if ( nValue <= SHRT_MAX && nValue >= SHRT_MIN )
		{
			return enmAsmWordLength_Word;
		}

		return enmAsmWordLength_Dword;
	}
	else
	{
		if ( dwValue <= UCHAR_MAX )
		{
			return enmAsmWordLength_Byte;
		}

		if ( dwValue <= USHRT_MAX )
		{
			return enmAsmWordLength_Word;
		}

		return enmAsmWordLength_Dword;
	}
}
		
bool CAsmCodeGenerator::IsNumberNodeSigned(CAsmSyntaxTree *pTree)
{
	std::string szLabelOffset = pTree->GetAttrValue("LabelOffset");

	if ( szLabelOffset == "" )
	{
		std::string szValue = pTree->GetAttrValue("Value");

		if ( szValue[0] == '-' )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

DWORD CAsmCodeGenerator::GetValueOfNumberNode(CAsmSyntaxTree *pTree)
{
	std::string szLabelOffset = pTree->GetAttrValue("LabelOffset");

	if ( szLabelOffset == "" )
	{
		std::string szValue = pTree->GetAttrValue("Value");

		int i=0;
		BOOL b1 = GetValueOfNumberString_Asm((char *)szValue.c_str(), &i);

		return (DWORD)i;
	}
	else
	{
		CAsmSymbolItem *pSymbolItem = CAsmSymbolTable_GetInstance()->SearchSymbolItem(szLabelOffset.c_str());

		if ( pSymbolItem == NULL )
		{
			ASSERT(FALSE);

			return 0;
		}

		return (DWORD)pSymbolItem->m_nAddress;
	}
}

BOOL CAsmCodeGenerator::IsOprandNodeByte(CAsmSyntaxTree *pTree)
{
	if ( pTree == NULL )
	{
		return FALSE;
	}

	if ( pTree->GetType() == "LabelRef" )
	{
		if ( pTree->GetAttrValue("Distance") == "short" )
		{
			return TRUE;
		}

		return FALSE;
	}

	if ( pTree->GetType() == "Number" )
	{
		bool bNumberNodeSigned = IsNumberNodeSigned(pTree);

		DWORD dwValue = (DWORD)GetValueOfNumberNode(pTree);

		enmAsmWordLength eNumberNodeWordLength = GetNumberNodeWordLength(pTree, bNumberNodeSigned, dwValue);

		if ( eNumberNodeWordLength >= enmAsmWordLength_Word )
		{
			return FALSE;
		}
	}

	if ( pTree->GetAttrValue("Length") == "byte" || pTree->GetAttrValue("Length") == "" )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CAsmCodeGenerator::IsOprandNodeWord(CAsmSyntaxTree *pTree)
{
	if ( pTree == NULL )
	{
		return FALSE;
	}

	if ( pTree->GetType() == "LabelRef" )
	{
		if ( pTree->GetAttrValue("Distance") == "near" )
		{
			return TRUE;
		}

		return FALSE;
	}

	if ( pTree->GetType() == "Number")
	{
		bool bNumberNodeSigned = IsNumberNodeSigned(pTree);

		DWORD dwValue = (DWORD)GetValueOfNumberNode(pTree);

		enmAsmWordLength eNumberNodeWordLength = GetNumberNodeWordLength(pTree, bNumberNodeSigned, dwValue);

		if ( eNumberNodeWordLength >= enmAsmWordLength_Dword )
		{
			return FALSE;
		}
	}

	if ( pTree->GetAttrValue("Length") == "word" || pTree->GetAttrValue("Length") == "" )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CAsmCodeGenerator::IsOprandNodeDword(CAsmSyntaxTree *pTree)
{
	if ( pTree == NULL )
	{
		return FALSE;
	}

	if ( pTree->GetType() == "LabelRef" )
	{
		if ( pTree->GetAttrValue("Distance") == "far" || pTree->GetAttrValue("Distance") == "far?" || pTree->GetAttrValue("Distance") == "" )
		{
			return TRUE;
		}

		return FALSE;
	}

	if ( pTree->GetAttrValue("Length") == "dword" || pTree->GetAttrValue("Length") == "" )
	{
		return TRUE;
	}

	return FALSE;
}

BYTE CAsmCodeGenerator::GetCodeOfRegisterNode(CAsmSyntaxTree *pTree)
{
	if ( pTree->GetType() != "Register" )
	{
		return 0;
	}

	std::string szReg = pTree->GetAttrValue("Reg");

	return GetCodeOfRegisterString(szReg.c_str());
}

BYTE CAsmCodeGenerator::GetCodeOfRegisterString(const char *pszReg1)
{
	std::string szReg = pszReg1;

	if ( szReg == "eax" || szReg == "ax" || szReg == "al" )
	{
		return 0;
	}
	else if ( szReg == "ecx" || szReg == "cx" || szReg == "cl" )
	{
		return 1;
	}
	else if ( szReg == "edx" || szReg == "dx" || szReg == "dl" )
	{
		return 2;
	}
	else if ( szReg == "ebx" || szReg == "bx" || szReg == "bl" )
	{
		return 3;
	}
	else if ( szReg == "esp" || szReg == "sp" || szReg == "ah" )
	{
		return 4;
	}
	else if ( szReg == "ebp" || szReg == "bp" || szReg == "ch" )
	{
		return 5;
	}
	else if ( szReg == "esi" || szReg == "si" || szReg == "dh" )
	{
		return 6;
	}
	else if ( szReg == "edi" || szReg == "di" || szReg == "bh" )
	{
		return 7;
	}

	ASSERT(FALSE);

	return 0;
}

BYTE CAsmCodeGenerator::GetCodeOfRegisterScale(const char *pszScale)
{
	std::string szScale1 = pszScale;

	if ( szScale1 == "1" )
	{
		return 0x00;
	}
	else if ( szScale1 == "2" )
	{
		return 0x01;
	}
	else if ( szScale1 == "4" )
	{
		return 0x02;
	}
	else if ( szScale1 == "8" )
	{
		return 0x03;
	}
	else
	{
		ASSERT(FALSE);
		return 0x00;
	}
}

BOOL CAsmCodeGenerator::GetAddressOfLabelRef(CAsmSyntaxTree *pTree, int *pnAddress)
{
	if ( pnAddress)
	{
		*pnAddress = m_dwOriginalAddress;
	}

	if ( pTree->GetType() != "LabelRef" )
	{
		return FALSE;
	}

	std::string szLabel = pTree->GetAttrValue("Label");

	CAsmSymbolItem *pSymbolItem = CAsmSymbolTable_GetInstance()->SearchSymbolItem(szLabel.c_str());

	if ( pSymbolItem == NULL )
	{
		return FALSE;
	}

	if ( pnAddress )
	{
		*pnAddress = pSymbolItem->m_nAddress;
	}

	return TRUE;
}

void CAsmCodeGenerator::DetermingLabelRefDistance(CAsmSyntaxTree *pTree)
{
	if ( pTree->GetType() != "LabelRef" )
	{
		return;
	}

	std::string szDistance = pTree->GetAttrValue("Distance");

	if ( szDistance != "far?" )
	{
		if ( szDistance != "" )
		{
			return;
		}
	}

	int nLabelAddress = 0;
	BOOL b1 = GetAddressOfLabelRef(pTree, &nLabelAddress);
	if ( b1 == FALSE)
	{
		pTree->SetAttrValue("Distance", "far?");
		return;
	}

	int nEstimatedInstrEndAddress = m_dwOriginalAddress + 3;
	int nOffsetAddress = nLabelAddress - nEstimatedInstrEndAddress;
	if ( nOffsetAddress >= -128 && nOffsetAddress <= 127 )
	{
		pTree->SetAttrValue("Distance", "short");
		return;
	}

	nEstimatedInstrEndAddress = m_dwOriginalAddress + 5;
	nOffsetAddress = nLabelAddress - nEstimatedInstrEndAddress;
	if ( nOffsetAddress >= -32768 && nOffsetAddress <= 32767 )
	{
		pTree->SetAttrValue("Distance", "near");
		return;
	}

	pTree->SetAttrValue("Distance", "far");
}

void CAsmCodeGenerator::AddNotFoundLebelRefJumpStatement(CAsmSyntaxTree *pTree)
{
	CAsmSyntaxTree *pLeft=pTree->GetChildByTag("Left");

	if ( pLeft == NULL )
	{
		return;
	}

	if ( pLeft->GetType() != "LabelRef" )
	{
		return;
	}

	std::string szLabel = pLeft->GetAttrValue("Label");

	CAsmLabelRefTable_GetInstance()->AddLabelRef(szLabel.c_str(), m_dwInstrStartAddress, GetBinCodeEmitPos() - 2, pTree);
}

CAsmCodeGenerator *CAsmCodeGenerator_GetInstance(void)
{
	static CAsmCodeGenerator s_CodeGenerator;

	return &s_CodeGenerator;
}