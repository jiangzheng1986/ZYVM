//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "CAsmParser.hpp"

CAsmParser::CAsmParser(void)
{
	m_eTokenType = enmAsmTokenType_Unknown;
	m_szTokenString[0] = '\0';
	m_nLineNumber = 0;
	m_bShowFirstErrorOnly = TRUE;
	m_bFirstErrorShowed = FALSE;
	m_bError = FALSE;
	m_bCurrentInstructionIsJump = FALSE;
	m_bBaseRegisterFound = FALSE;
	m_bIndexRegisterFound = FALSE;
}

CAsmParser::~CAsmParser(void)
{

}

void CAsmParser::UpdateCurrentToken(void)
{
	tagAsmToken *pToken = CAsmScanner_GetInstance()->GetCurrentToken();

	m_eTokenType = pToken->eTokenType;

	strcpy_s(m_szTokenString, 1000, pToken->szTokenString.c_str());

	m_nLineNumber = pToken->nStartLineNumber + 1;
}

void CAsmParser::NextToken(void)
{
	int nCurrentTokenIndex = CAsmScanner_GetInstance()->GetCurrentTokenIndex();

	CAsmScanner_GetInstance()->SetCurrentTokenIndex(nCurrentTokenIndex + 1);

	UpdateCurrentToken();
}

int CAsmParser::GetCurrentTokenIndex(void)
{
	return CAsmScanner_GetInstance()->GetCurrentTokenIndex();
}

void CAsmParser::SetCurrentTokenIndex(int nCurrentTokenIndex)
{
	CAsmScanner_GetInstance()->SetCurrentTokenIndex(nCurrentTokenIndex);

	UpdateCurrentToken();
}

void CAsmParser::MatchToken(enmAsmTokenType eExpectedTokenType)
{
	if ( m_eTokenType != eExpectedTokenType )
	{
		if ( m_bShowFirstErrorOnly )
		{
			if ( m_bFirstErrorShowed )
			{
				return;
			}
		}
		printf("Error(%d): Unexpected token type:", m_nLineNumber);
		CAsmScanner_GetInstance()->PrintTokenType(m_eTokenType, m_szTokenString);
		printf(",token string:");
		printf(m_szTokenString);
		printf(",expected token type:");
		CAsmScanner_GetInstance()->PrintTokenType(eExpectedTokenType, "");
		printf("\n");
		NextToken();
		m_bFirstErrorShowed = TRUE;

		ExitProcess(0);
	}
	else
	{
	}

	NextToken();
}

void CAsmParser::Error(const char *pszFormat, ...)
{
	va_list pArgs;
	char szBuffer[1000];

	va_start(pArgs, pszFormat);

	vsprintf_s(szBuffer, sizeof(szBuffer), pszFormat, pArgs);

	printf("%s", szBuffer);

	m_bError = TRUE;

	va_end(pArgs);
}

CAsmSyntaxTree *CAsmParser::Parse(void)
{
	CAsmScanner_GetInstance()->SetCurrentTokenIndex(0);
	UpdateCurrentToken();

	return ParseProgram();
}

bool CAsmParser::GetError()
{
	return (m_bError != FALSE);
}

CAsmSyntaxTree *CAsmParser::ParseProgram(void)
{
	return ParseStatementList();
}

CAsmSyntaxTree *CAsmParser::ParseStatementList(void)
{
	CAsmSyntaxTree *pTree = new CAsmSyntaxTree("StatementList", m_nLineNumber);

	while ( m_eTokenType != enmAsmTokenType_Eof )
	{		
		CAsmSyntaxTree *pChild = NULL;

		if ( m_eTokenType == enmAsmTokenType_Identifier )
		{
			int nSavedTokenIndex = GetCurrentTokenIndex();

			CAsmSyntaxTree *pTemp = ParseLabel1();
			DELETE_CPP(pTemp);

			enmAsmTokenType eTokenType1 = m_eTokenType;

			SetCurrentTokenIndex(nSavedTokenIndex);

			switch(eTokenType1)
			{
			case enmAsmTokenType_EQU:
				ParseMacroDefine(pTree);
				goto ParseStatementList_1;
			case enmAsmTokenType_PROC:
				ParseProcBegin(pTree);
				goto ParseStatementList_1;
			case enmAsmTokenType_ENDP:
				ParseProcEnd(pTree);
				goto ParseStatementList_1;
			case enmAsmTokenType_DB:
			case enmAsmTokenType_DW:
			case enmAsmTokenType_DD:
				ParseDataDefine(pTree);
				goto ParseStatementList_1;
			case enmAsmTokenType_LABEL:
				pChild=ParseDataLabelDefine();
				pTree->AddChild(pChild);
				goto ParseStatementList_1;
			case enmAsmTokenType_Colon:
				pChild=ParseStatementLabel();
				pTree->AddChild(pChild);
				break;
			}
		}

		if( m_eTokenType == enmAsmTokenType_REPNZ||
			m_eTokenType == enmAsmTokenType_REPNE||
			m_eTokenType == enmAsmTokenType_REPZ||
			m_eTokenType == enmAsmTokenType_REPE||
			m_eTokenType == enmAsmTokenType_REP)
		{
			pChild=ParseRepeatPrefix();

			pTree->AddChild(pChild);
		}

		pChild=ParseStatement();

		pTree->AddChild(pChild);

ParseStatementList_1:
		if(m_eTokenType == enmAsmTokenType_Eof)
		{
			break;
		}
		else
		{
			MatchToken(enmAsmTokenType_NewLine);
		}
	}

	return pTree;
}

void CAsmParser::ParseMacroDefine(CAsmSyntaxTree *pTree)
{
	CAsmMacro *pMacro = new CAsmMacro();
	
	pMacro->m_szMacroName = m_szTokenString;

	MatchToken(enmAsmTokenType_Identifier);

	MatchToken(enmAsmTokenType_EQU);

	while ( m_eTokenType != enmAsmTokenType_NewLine && m_eTokenType != enmAsmTokenType_Eof )
	{
		tagAsmToken *pToken = CAsmScanner_GetInstance()->GetCurrentToken();
		tagAsmToken *pToken1 = CAsmScanner_GetInstance()->CopyToken(pToken);
		pMacro->m_vecTokenList.push_back(pToken1);

		MatchToken(m_eTokenType);
	}

	CAsmScanner_GetInstance()->AddMacro(pMacro);
}

void CAsmParser::ParseProcBegin(CAsmSyntaxTree *pTree)
{
	CAsmSyntaxTree *pTree1 = ParseLabel1();

	MatchToken(enmAsmTokenType_PROC);

	pTree1->SetAttrValue("BeginProc", "1");

	pTree->AddChild(pTree1);
}

void CAsmParser::ParseProcEnd(CAsmSyntaxTree *pTree)
{
	CAsmSyntaxTree *pTree1 = new CAsmSyntaxTree("EndProc", m_nLineNumber);

	pTree1->SetAttrValue("ProcName", m_szTokenString);

	MatchToken(enmAsmTokenType_Identifier);

	MatchToken(enmAsmTokenType_ENDP);

	pTree->AddChild(pTree1);

	CAsmScanner_GetInstance()->ClearLocalMacros();
}

void CAsmParser::ParseDataDefine(CAsmSyntaxTree *pTree)
{
	CAsmSyntaxTree *pTree1 = ParseLabel1();

	if ( m_eTokenType == enmAsmTokenType_DB )
	{
		pTree1->SetAttrValue("Length", "byte");

		MatchToken(enmAsmTokenType_DB);
	}
	else if ( m_eTokenType == enmAsmTokenType_DW )
	{
		pTree1->SetAttrValue("Length", "word");

		MatchToken(enmAsmTokenType_DW);
	}
	else if ( m_eTokenType == enmAsmTokenType_DD )
	{
		pTree1->SetAttrValue("Length", "dword");

		MatchToken(enmAsmTokenType_DD);
	}
	else
	{
		Error("%d:Unknown type\n", m_nLineNumber);
	}

	pTree->AddChild(pTree1);

	CAsmSyntaxTree *pTree2 = new CAsmSyntaxTree("DataDefine", m_nLineNumber);

	pTree2->SetAttrValue("Length", pTree1->GetAttrValue("Length").c_str());

	ParseDataList(pTree2);

	pTree->AddChild(pTree2);
}

void CAsmParser::ParseDataList(CAsmSyntaxTree *pTree)
{
	CAsmSyntaxTree *pChild = ParseData();

	pTree->AddChild(pChild);

	while ( m_eTokenType == enmAsmTokenType_Comma )
	{
		MatchToken(enmAsmTokenType_Comma);

		pChild = ParseData();

		pTree->AddChild(pChild);
	}
}

CAsmSyntaxTree *CAsmParser::ParseData(void)
{
	if ( m_eTokenType == enmAsmTokenType_RandomNumber )
	{
		return ParseRandomNumber();
	}
	else if ( m_eTokenType == enmAsmTokenType_Number )
	{
		return ParseNumber();
	}
	else
	{
		ASSERT(FALSE);
		return NULL;
	}
}

CAsmSyntaxTree *CAsmParser::ParseDataLabelDefine(void)
{
	CAsmSyntaxTree *pTree = ParseLabel1();

	MatchToken(enmAsmTokenType_LABEL);

	if ( m_eTokenType == enmAsmTokenType_BYTE )
	{
		pTree->SetAttrValue("Length", "byte");

		MatchToken(enmAsmTokenType_BYTE);
	}
	else if ( m_eTokenType == enmAsmTokenType_WORD )
	{
		pTree->SetAttrValue("Length", "word");

		MatchToken(enmAsmTokenType_WORD);
	}
	else if ( m_eTokenType == enmAsmTokenType_DWORD )
	{
		pTree->SetAttrValue("Length", "dword");

		MatchToken(enmAsmTokenType_DWORD);
	}
	else
	{
		Error("%d:Unknown type\n", m_nLineNumber);
	}

	return pTree;
}

CAsmSyntaxTree *CAsmParser::ParseStatementLabel(void)
{
	CAsmSyntaxTree *pTree = ParseLabel1();

	MatchToken(enmAsmTokenType_Colon);

	return pTree;
}

CAsmSyntaxTree *CAsmParser::ParseLabel1(void)
{
	CAsmSyntaxTree *pTree = new CAsmSyntaxTree("Label", m_nLineNumber);

	if ( m_eTokenType == enmAsmTokenType_Identifier )
	{
		pTree->SetAttrValue("Label", m_szTokenString);

		MatchToken(enmAsmTokenType_Identifier);
	}

	return pTree;
}

CAsmSyntaxTree *CAsmParser::ParseRepeatPrefix(void)
{
	CAsmSyntaxTree *pTree = NULL;

	if( m_eTokenType == enmAsmTokenType_REPNZ||
		m_eTokenType == enmAsmTokenType_REPNE||
		m_eTokenType == enmAsmTokenType_REPZ||
		m_eTokenType == enmAsmTokenType_REPE||
		m_eTokenType == enmAsmTokenType_REP)
	{
		pTree = new CAsmSyntaxTree("Statement", m_nLineNumber);

		pTree->SetAttrValue("Instr", m_szTokenString);

		MatchToken(m_eTokenType);
	}

	return pTree;
}

CAsmSyntaxTree *CAsmParser::ParseStatement(void)
{
	switch ( m_eTokenType )
	{
	case enmAsmTokenType_ORG:
	case enmAsmTokenType_ADD:
	case enmAsmTokenType_ADC:
	case enmAsmTokenType_SBB:
	case enmAsmTokenType_SUB:
	case enmAsmTokenType_CMP:
	case enmAsmTokenType_TEST:
	case enmAsmTokenType_AND:
	case enmAsmTokenType_OR:
	case enmAsmTokenType_XOR:
	case enmAsmTokenType_MOV:
	case enmAsmTokenType_XCHG:
	case enmAsmTokenType_LEA:
	case enmAsmTokenType_LES:
	case enmAsmTokenType_LDS:
	case enmAsmTokenType_ROL:
	case enmAsmTokenType_ROR:
	case enmAsmTokenType_RCL:
	case enmAsmTokenType_RCR:
	case enmAsmTokenType_SHL:
	case enmAsmTokenType_SHR:
	case enmAsmTokenType_SAR:
	case enmAsmTokenType_IN:
	case enmAsmTokenType_OUT:
	case enmAsmTokenType_INC:
	case enmAsmTokenType_DEC:
	case enmAsmTokenType_NOT:
	case enmAsmTokenType_NEG:
	case enmAsmTokenType_MUL:
	case enmAsmTokenType_DIV:
	case enmAsmTokenType_IMUL:
	case enmAsmTokenType_IDIV:
	case enmAsmTokenType_PUSH:
	case enmAsmTokenType_POP:
	case enmAsmTokenType_INT:
	case enmAsmTokenType_MOVSB:
	case enmAsmTokenType_MOVSW:
	case enmAsmTokenType_CMPSB:
	case enmAsmTokenType_CMPSW:
	case enmAsmTokenType_STOSB:
	case enmAsmTokenType_STOSW:
	case enmAsmTokenType_LODSB:
	case enmAsmTokenType_LODSW:
	case enmAsmTokenType_SCASB:
	case enmAsmTokenType_SCASW:
	case enmAsmTokenType_NOP:
	case enmAsmTokenType_XLAT:
	case enmAsmTokenType_PUSHF:
	case enmAsmTokenType_POPF:
	case enmAsmTokenType_SAHF:
	case enmAsmTokenType_LAHF:
	case enmAsmTokenType_CBW:
	case enmAsmTokenType_CWD:
	case enmAsmTokenType_CMC:
	case enmAsmTokenType_CLC:
	case enmAsmTokenType_STC:
	case enmAsmTokenType_CLI:
	case enmAsmTokenType_STI:
	case enmAsmTokenType_CLD:
	case enmAsmTokenType_STD:
	case enmAsmTokenType_DAA:
	case enmAsmTokenType_AAA:
	case enmAsmTokenType_DAS:
	case enmAsmTokenType_AAS:
	case enmAsmTokenType_AAM:
	case enmAsmTokenType_AAD:
	case enmAsmTokenType_IRET:
	case enmAsmTokenType_INTO:
	case enmAsmTokenType_WAIT:
	case enmAsmTokenType_LOCK:
	case enmAsmTokenType_HLT:
	case enmAsmTokenType_RET:
	case enmAsmTokenType_RETF:
		m_bCurrentInstructionIsJump = FALSE;
		return ParseStatement1();
	case enmAsmTokenType_JO:
	case enmAsmTokenType_JNO:
	case enmAsmTokenType_JB:
	case enmAsmTokenType_JNB:
	case enmAsmTokenType_JZ:
	case enmAsmTokenType_JE:
	case enmAsmTokenType_JNZ:
	case enmAsmTokenType_JNE:
	case enmAsmTokenType_JNA:
	case enmAsmTokenType_JBE:
	case enmAsmTokenType_JA:
	case enmAsmTokenType_JS:
	case enmAsmTokenType_JNS:
	case enmAsmTokenType_JP:
	case enmAsmTokenType_JNP:
	case enmAsmTokenType_JL:
	case enmAsmTokenType_JNL:
	case enmAsmTokenType_JGE:
	case enmAsmTokenType_JNG:
	case enmAsmTokenType_JLE:
	case enmAsmTokenType_JG:
	case enmAsmTokenType_LOOPNZ:
	case enmAsmTokenType_LOOPNE:
	case enmAsmTokenType_LOOPZ:
	case enmAsmTokenType_LOOPE:
	case enmAsmTokenType_LOOP:
	case enmAsmTokenType_JCXZ:
	case enmAsmTokenType_CALL:
	case enmAsmTokenType_JMP:
		m_bCurrentInstructionIsJump = TRUE;
		return ParseStatement1();
	}

	return NULL;
}

CAsmSyntaxTree *CAsmParser::ParseStatement1(void)
{
	CAsmSyntaxTree *pTree = new CAsmSyntaxTree("Statement", m_nLineNumber);

	std::string szInstrName = StringToLower(m_szTokenString);

	pTree->SetAttrValue("Instr", szInstrName.c_str());

	MatchToken(m_eTokenType);

	if ( m_eTokenType != enmAsmTokenType_Eof && m_eTokenType != enmAsmTokenType_NewLine )
	{
		CAsmSyntaxTree *pChild = ParseExpression();

		pChild->SetAttrValue("Tag", "Left");

		pTree->AddChild(pChild);

		if ( m_eTokenType == enmAsmTokenType_Comma )
		{
			MatchToken(enmAsmTokenType_Comma);

			pChild = ParseExpression();

			pChild->SetAttrValue("Tag", "Right");

			pTree->AddChild(pChild);
		}
	}

	return pTree;
}

CAsmSyntaxTree *CAsmParser::ParseExpression(void)
{
	CAsmSyntaxTree *pTree = NULL;

	if ( m_bCurrentInstructionIsJump == FALSE )
	{
		if ( m_eTokenType == enmAsmTokenType_Identifier )
		{
			pTree = ParseMemory();
			return pTree;
		}
	}

	if( m_eTokenType == enmAsmTokenType_Number ||
		m_eTokenType == enmAsmTokenType_Minus)
	{
		pTree = ParseNumber();
	}
	else if ( IsTokenRegister(m_eTokenType) )
	{
		pTree = ParseRegister();
	}
	else if ( IsTokenSegRegister(m_eTokenType) )
	{
		pTree = ParseSegRegister();
	}
	else if ( m_eTokenType == enmAsmTokenType_DWORD ||
			  m_eTokenType == enmAsmTokenType_WORD ||
		      m_eTokenType == enmAsmTokenType_BYTE ||
			  m_eTokenType == enmAsmTokenType_LeftParen2 )
	{
		pTree = ParseMemory();
	}
	else if ( m_eTokenType == enmAsmTokenType_SHORT ||
		      m_eTokenType == enmAsmTokenType_NEAR ||
			  m_eTokenType == enmAsmTokenType_FAR ||
			  m_eTokenType == enmAsmTokenType_Identifier )
	{
		pTree = ParseLabelRef();
	}
	else if ( m_eTokenType == enmAsmTokenType_OFFSET )
	{
		pTree = ParseLabelOffset();
	}

	return pTree;
}

CAsmSyntaxTree *CAsmParser::ParseRandomNumber(void)
{
	CAsmSyntaxTree *pTree = new CAsmSyntaxTree("Number", m_nLineNumber);

	char szBuf[1000];
	sprintf_s(szBuf, "%d", 0);
	pTree->SetAttrValue("Value", szBuf);

	MatchToken(enmAsmTokenType_RandomNumber);

	return pTree;
}

CAsmSyntaxTree *CAsmParser::ParseNumber(void)
{
	CAsmSyntaxTree *pTree = new CAsmSyntaxTree("Number", m_nLineNumber);

	BOOL bNeg = FALSE;

	if ( m_eTokenType == enmAsmTokenType_Minus )
	{
		bNeg = TRUE;

		MatchToken(enmAsmTokenType_Minus);
	}
	
	int i = 0;
	BOOL b1 = GetValueOfNumberString_Asm(m_szTokenString, &i);

	if ( bNeg )
	{
		i = -i;
	}

	if ( b1 == FALSE )
	{
		Error("%d:Invalid number string.\n", pTree->GetLineNumber());
	}

	if ( i < -0x8000 || i > 0xFFFF )
	{
		pTree->SetAttrValue("Length", "dword");
	}

	char szBuf[1000];
	if ( bNeg )
	{
		sprintf_s(szBuf, "%d", i);
	}
	else
	{
		sprintf_s(szBuf, "%u", i);
	}
	pTree->SetAttrValue("Value", szBuf);

	MatchToken(enmAsmTokenType_Number);

	return pTree;
}

CAsmSyntaxTree *CAsmParser::ParseRegister(void)
{
	CAsmSyntaxTree *pTree = new CAsmSyntaxTree("Register", m_nLineNumber);

	if( IsTokenRegister(m_eTokenType) )
	{
		std::string szRegisterName = StringToLower(m_szTokenString);

		pTree->SetAttrValue("Reg", szRegisterName.c_str());

		if ( m_eTokenType == enmAsmTokenType_AL ||
			 m_eTokenType == enmAsmTokenType_CL ||
			 m_eTokenType == enmAsmTokenType_DL ||
			 m_eTokenType == enmAsmTokenType_BL ||
			 m_eTokenType == enmAsmTokenType_AH ||
			 m_eTokenType == enmAsmTokenType_CH ||
			 m_eTokenType == enmAsmTokenType_DH ||
			 m_eTokenType == enmAsmTokenType_BH )
		{
			pTree->SetAttrValue("Length", "byte");
		}
		else if ( m_eTokenType == enmAsmTokenType_AX ||
				  m_eTokenType == enmAsmTokenType_CX ||
				  m_eTokenType == enmAsmTokenType_DX ||
				  m_eTokenType == enmAsmTokenType_BX ||
				  m_eTokenType == enmAsmTokenType_SP ||
				  m_eTokenType == enmAsmTokenType_BP ||
				  m_eTokenType == enmAsmTokenType_SI ||
				  m_eTokenType == enmAsmTokenType_DI )
		{
			pTree->SetAttrValue("Length", "word");
		}
		else if ( m_eTokenType == enmAsmTokenType_EAX ||
				  m_eTokenType == enmAsmTokenType_ECX ||
				  m_eTokenType == enmAsmTokenType_EDX ||
				  m_eTokenType == enmAsmTokenType_EBX ||
				  m_eTokenType == enmAsmTokenType_ESP ||
				  m_eTokenType == enmAsmTokenType_EBP ||
				  m_eTokenType == enmAsmTokenType_ESI ||
				  m_eTokenType == enmAsmTokenType_EDI )
		{
			pTree->SetAttrValue("Length", "dword");
		}

		MatchToken(m_eTokenType);
	}

	return pTree;
}

BOOL CAsmParser::IsTokenRegister(enmAsmTokenType eTokenType)
{
	if ( eTokenType == enmAsmTokenType_EAX ||
		 eTokenType == enmAsmTokenType_ECX ||
		 eTokenType == enmAsmTokenType_EDX ||
		 eTokenType == enmAsmTokenType_EBX ||
		 eTokenType == enmAsmTokenType_ESP ||
		 eTokenType == enmAsmTokenType_EBP ||
		 eTokenType == enmAsmTokenType_ESI ||
		 eTokenType == enmAsmTokenType_EDI ||		
		 eTokenType == enmAsmTokenType_AX ||
		 eTokenType == enmAsmTokenType_CX ||
		 eTokenType == enmAsmTokenType_DX ||
		 eTokenType == enmAsmTokenType_BX ||
		 eTokenType == enmAsmTokenType_SP ||
		 eTokenType == enmAsmTokenType_BP ||
		 eTokenType == enmAsmTokenType_SI ||
		 eTokenType == enmAsmTokenType_DI ||
		 eTokenType == enmAsmTokenType_AL ||
		 eTokenType == enmAsmTokenType_CL ||
		 eTokenType == enmAsmTokenType_DL ||
		 eTokenType == enmAsmTokenType_BL ||
		 eTokenType == enmAsmTokenType_AH ||
		 eTokenType == enmAsmTokenType_CH ||
		 eTokenType == enmAsmTokenType_DH ||
		 eTokenType == enmAsmTokenType_BH )
	{
		return TRUE;
	}

	return FALSE;
}

CAsmSyntaxTree *CAsmParser::ParseSegRegister(void)
{
	CAsmSyntaxTree *pTree = new CAsmSyntaxTree("SegRegister", m_nLineNumber);

	if ( IsTokenSegRegister(m_eTokenType) )
	{
		pTree->SetAttrValue("Seg", m_szTokenString);

		pTree->SetAttrValue("Length", "word");

		MatchToken(m_eTokenType);
	}

	return pTree;
}

void CAsmParser::ParseSegPrefix(CAsmSyntaxTree *pTree)
{
	CAsmSyntaxTree *pTree1 = ParseSegRegister();

	pTree->SetAttrValue("SegPrefix", pTree1->GetAttrValue("Seg").c_str());

	DELETE_CPP(pTree1);

	MatchToken(enmAsmTokenType_Colon);
}

BOOL CAsmParser::IsTokenSegRegister(enmAsmTokenType eTokenType)
{
	if ( eTokenType == enmAsmTokenType_ES ||
		 eTokenType == enmAsmTokenType_CS ||
		 eTokenType == enmAsmTokenType_SS ||
		 eTokenType == enmAsmTokenType_DS )
	{
		return TRUE;
	}

	return FALSE;
}

CAsmSyntaxTree *CAsmParser::ParseMemory(void)
{
	m_bBaseRegisterFound = false;

	m_bIndexRegisterFound = false;

	CAsmSyntaxTree *pTree = new CAsmSyntaxTree("Memory", m_nLineNumber);

	if ( m_eTokenType == enmAsmTokenType_DWORD )
	{
		pTree->SetAttrValue("Length", "dword");

		MatchToken(enmAsmTokenType_DWORD);

		MatchToken(enmAsmTokenType_PTR);

		ParseMemory1(pTree);
	}
	else if ( m_eTokenType == enmAsmTokenType_WORD )
	{
		pTree->SetAttrValue("Length", "word");

		MatchToken(enmAsmTokenType_WORD);

		MatchToken(enmAsmTokenType_PTR);
		
		ParseMemory1(pTree);
	}
	else if ( m_eTokenType == enmAsmTokenType_BYTE )
	{
		pTree->SetAttrValue("Length","byte");

		MatchToken(enmAsmTokenType_BYTE);

		MatchToken(enmAsmTokenType_PTR);

		ParseMemory1(pTree);
	}
	else if(m_eTokenType == enmAsmTokenType_LeftParen2)
	{
		ParseMemory1(pTree);
	}
	else if(m_eTokenType == enmAsmTokenType_Identifier)
	{
		ParseMemory1(pTree);
	}

	return pTree;
}

void CAsmParser::ParseMemory1(CAsmSyntaxTree *pTree)
{
	if ( IsTokenSegRegister(m_eTokenType) )
	{
		ParseSegPrefix(pTree);
	}

	do
	{
		if ( m_eTokenType == enmAsmTokenType_Identifier )
		{
			ParseMemoryAddress(pTree);
		}
		else if ( m_eTokenType == enmAsmTokenType_LeftParen2 )
		{
			MatchToken(enmAsmTokenType_LeftParen2);

			ParseMemoryAddress(pTree);

			MatchToken(enmAsmTokenType_RightParen2);
		}
		else if ( m_eTokenType == enmAsmTokenType_Dot )
		{
			MatchToken(enmAsmTokenType_Dot);

			ParseMemoryAddressElement(pTree);
		}
	}
	while ( m_eTokenType == enmAsmTokenType_LeftParen2 || m_eTokenType == enmAsmTokenType_Dot );
}

void CAsmParser::ParseMemoryAddress(CAsmSyntaxTree *pTree)
{
	ParseMemoryAddressElement(pTree);

	while ( m_eTokenType == enmAsmTokenType_Plus || m_eTokenType == enmAsmTokenType_Minus )
	{
		if ( m_eTokenType == enmAsmTokenType_Plus )
		{
			MatchToken(enmAsmTokenType_Plus);

			ParseMemoryAddressElement(pTree);
		}
		else
		{
			MatchToken(enmAsmTokenType_Minus);

			ParseMemoryAddressElement_Minus(pTree);
		}
	}
}

void CAsmParser::ParseMemoryAddressElement(CAsmSyntaxTree *pTree)
{
	switch ( m_eTokenType )
	{
	case enmAsmTokenType_EAX:
	case enmAsmTokenType_ECX:
	case enmAsmTokenType_EDX:
	case enmAsmTokenType_EBX:
	case enmAsmTokenType_ESP:
	case enmAsmTokenType_EBP:
	case enmAsmTokenType_ESI:
	case enmAsmTokenType_EDI:
		{
			if ( m_bBaseRegisterFound && m_bIndexRegisterFound )
			{
				Error("%d:Too much registers in a single memory block\n", m_nLineNumber);
			}

			std::string szRegisterName = StringToLower(m_szTokenString);
			
			MatchToken(m_eTokenType);

			if ( m_eTokenType == enmAsmTokenType_Times )
			{
				if ( m_bIndexRegisterFound )
				{
					Error("%d:Too much index registers in a single memory block\n", m_nLineNumber);
				}

				MatchToken(m_eTokenType);

				if ( m_eTokenType == enmAsmTokenType_Number )
				{
					pTree->SetAttrValue("IndexRegister", szRegisterName.c_str());

					int nScale = atoi(m_szTokenString);

					pTree->SetAttrValue("Scale", m_szTokenString);

					m_bIndexRegisterFound = true;
				}

				MatchToken(enmAsmTokenType_Number);
			}
			else
			{
				if ( m_bBaseRegisterFound )
				{
					pTree->SetAttrValue("IndexRegister", szRegisterName.c_str());

					pTree->SetAttrValue("Scale", "1");

					m_bIndexRegisterFound = true;
				}
				else
				{
					pTree->SetAttrValue("BaseRegister", szRegisterName.c_str());

					m_bBaseRegisterFound = true;
				}
			}
		}
		break;
	case enmAsmTokenType_Number:
		pTree->SetAttrValue("Offset", m_szTokenString);
		MatchToken(enmAsmTokenType_Number);
		break;
	case enmAsmTokenType_Identifier:
		pTree->SetAttrValue("LabelOffset", m_szTokenString);
		MatchToken(enmAsmTokenType_Identifier);
		break;
	}
}

void CAsmParser::ParseMemoryAddressElement_Minus(CAsmSyntaxTree *pTree)
{
	switch ( m_eTokenType )
	{
	case enmAsmTokenType_Number:
		{
			char szBuf[1000];
			sprintf_s(szBuf,"-%s", m_szTokenString);
			pTree->SetAttrValue("Offset", szBuf);
			MatchToken(enmAsmTokenType_Number);
		}
		break;
	}
}

CAsmSyntaxTree *CAsmParser::ParseLabelOffset(void)
{
	CAsmSyntaxTree *pTree = new CAsmSyntaxTree("Number", m_nLineNumber);

	MatchToken(enmAsmTokenType_OFFSET);

	pTree->SetAttrValue("Length", "dword");

	pTree->SetAttrValue("Value", "0");

	if ( m_eTokenType == enmAsmTokenType_Identifier )
	{
		pTree->SetAttrValue("LabelOffset", m_szTokenString);

		MatchToken(enmAsmTokenType_Identifier);
	}

	return pTree;
}

CAsmSyntaxTree *CAsmParser::ParseLabelRef(void)
{
	CAsmSyntaxTree *pTree = new CAsmSyntaxTree("LabelRef", m_nLineNumber);

	if ( m_eTokenType == enmAsmTokenType_SHORT ||
		 m_eTokenType == enmAsmTokenType_NEAR ||
		 m_eTokenType == enmAsmTokenType_FAR )
	{
		std::string szDistance = StringToLower(m_szTokenString);

		pTree->SetAttrValue("Distance", szDistance.c_str());

		MatchToken(m_eTokenType);

		if ( m_eTokenType == enmAsmTokenType_PTR )
		{
			MatchToken(enmAsmTokenType_PTR);
		}
	}

	if ( m_eTokenType == enmAsmTokenType_Identifier )
	{
		pTree->SetAttrValue("Label", m_szTokenString);

		MatchToken(enmAsmTokenType_Identifier);
	}

	return pTree;
}

CAsmParser *CAsmParser_GetInstance(void)
{
	static CAsmParser s_Parser;

	return &s_Parser;
}
