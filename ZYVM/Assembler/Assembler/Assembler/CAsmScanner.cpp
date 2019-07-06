//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#include "CAsmScanner.hpp"

CAsmMacro::CAsmMacro()
{

}

CAsmMacro::~CAsmMacro()
{
	int i;
	for ( i = 0; i < (int)m_vecTokenList.size(); i++ )
	{
		DELETE_CPP(m_vecTokenList[i]);
	}
	m_vecTokenList.clear();
}

CAsmScanner::CAsmScanner(void)
{
	m_pszSource = NULL;
	m_nSourceLength = 0;
	m_nCharIndex = 0;
	m_nLineNumber = 0;
	m_nCharIndex = 0;
	m_eTokenType = enmAsmTokenType_Unknown;
	m_szTokenString[0] = '\0';
	m_nTokenStartLineNumber = 0;
	m_nTokenEndLineNumber = 0;
	m_nTokenStartCharIndex = 0;
	m_nTokenEndCharIndex = 0;
	BuildKeywordTable();
	m_nCurrentTokenIndex = 0;
}

CAsmScanner::~CAsmScanner(void)
{
	CloseSource();

	int i;
	for ( i = 0; i < (int)m_vecTokenList.size(); i++ )
	{
		DELETE_CPP(m_vecTokenList[i]);
	}
	m_vecTokenList.clear();

	std::map<std::string,CAsmMacro *>::iterator it;
	for ( it = m_vecMacroMap.begin(); it != m_vecMacroMap.end(); it++ )
	{
		DELETE_CPP(it->second);
	}
	m_vecMacroMap.clear();
}

void CAsmScanner::BuildKeywordTable(void)
{
	m_mapKeywordMap["eax"] = enmAsmTokenType_EAX;
	m_mapKeywordMap["ecx"] = enmAsmTokenType_ECX;
	m_mapKeywordMap["edx"] = enmAsmTokenType_EDX;
	m_mapKeywordMap["ebx"] = enmAsmTokenType_EBX;
	m_mapKeywordMap["esp"] = enmAsmTokenType_ESP;
	m_mapKeywordMap["ebp"] = enmAsmTokenType_EBP;
	m_mapKeywordMap["esi"] = enmAsmTokenType_ESI;
	m_mapKeywordMap["edi"] = enmAsmTokenType_EDI;
	m_mapKeywordMap["ax"] = enmAsmTokenType_AX;
	m_mapKeywordMap["cx"] = enmAsmTokenType_CX;
	m_mapKeywordMap["dx"] = enmAsmTokenType_DX;
	m_mapKeywordMap["bx"] = enmAsmTokenType_BX;
	m_mapKeywordMap["sp"] = enmAsmTokenType_SP;
	m_mapKeywordMap["bp"] = enmAsmTokenType_BP;
	m_mapKeywordMap["si"] = enmAsmTokenType_SI;
	m_mapKeywordMap["di"] = enmAsmTokenType_DI;
	m_mapKeywordMap["al"] = enmAsmTokenType_AL;
	m_mapKeywordMap["cl"] = enmAsmTokenType_CL;
	m_mapKeywordMap["dl"] = enmAsmTokenType_DL;
	m_mapKeywordMap["bl"] = enmAsmTokenType_BL;
	m_mapKeywordMap["ah"] = enmAsmTokenType_AH;
	m_mapKeywordMap["ch"] = enmAsmTokenType_CH;
	m_mapKeywordMap["dh"] = enmAsmTokenType_DH;
	m_mapKeywordMap["bh"] = enmAsmTokenType_BH;
	m_mapKeywordMap["es"] = enmAsmTokenType_ES;
	m_mapKeywordMap["cs"] = enmAsmTokenType_CS;
	m_mapKeywordMap["ss"] = enmAsmTokenType_SS;
	m_mapKeywordMap["ds"] = enmAsmTokenType_DS;
	m_mapKeywordMap["org"] = enmAsmTokenType_ORG;
	m_mapKeywordMap["add"] = enmAsmTokenType_ADD;
	m_mapKeywordMap["adc"] = enmAsmTokenType_ADC;
	m_mapKeywordMap["sbb"] = enmAsmTokenType_SBB;
	m_mapKeywordMap["sub"] = enmAsmTokenType_SUB;
	m_mapKeywordMap["cmp"] = enmAsmTokenType_CMP;
	m_mapKeywordMap["test"] = enmAsmTokenType_TEST;
	m_mapKeywordMap["and"] = enmAsmTokenType_AND;
	m_mapKeywordMap["or"] = enmAsmTokenType_OR;
	m_mapKeywordMap["xor"] = enmAsmTokenType_XOR;
	m_mapKeywordMap["mov"] = enmAsmTokenType_MOV;
	m_mapKeywordMap["xchg"] = enmAsmTokenType_XCHG;
	m_mapKeywordMap["lea"] = enmAsmTokenType_LEA;
	m_mapKeywordMap["les"] = enmAsmTokenType_LES;
	m_mapKeywordMap["lds"] = enmAsmTokenType_LDS;
	m_mapKeywordMap["inc"] = enmAsmTokenType_INC;
	m_mapKeywordMap["dec"] = enmAsmTokenType_DEC;
	m_mapKeywordMap["not"] = enmAsmTokenType_NOT;
	m_mapKeywordMap["neg"] = enmAsmTokenType_NEG;
	m_mapKeywordMap["mul"] = enmAsmTokenType_MUL;
	m_mapKeywordMap["div"] = enmAsmTokenType_DIV;
	m_mapKeywordMap["imul"] = enmAsmTokenType_IMUL;
	m_mapKeywordMap["idiv"] = enmAsmTokenType_IDIV;
	m_mapKeywordMap["push"] = enmAsmTokenType_PUSH;
	m_mapKeywordMap["pop"] = enmAsmTokenType_POP;
	m_mapKeywordMap["rol"] = enmAsmTokenType_ROL;
	m_mapKeywordMap["ror"] = enmAsmTokenType_ROR;
	m_mapKeywordMap["rcl"] = enmAsmTokenType_RCL;
	m_mapKeywordMap["rcr"] = enmAsmTokenType_RCR;
	m_mapKeywordMap["shl"] = enmAsmTokenType_SHL;
	m_mapKeywordMap["shr"] = enmAsmTokenType_SHR;
	m_mapKeywordMap["sar"] = enmAsmTokenType_SAR;
	m_mapKeywordMap["in"] = enmAsmTokenType_IN;
	m_mapKeywordMap["out"] = enmAsmTokenType_OUT;
	m_mapKeywordMap["movsb"] = enmAsmTokenType_MOVSB;
	m_mapKeywordMap["movsw"] = enmAsmTokenType_MOVSW;
	m_mapKeywordMap["cmpsb"] = enmAsmTokenType_CMPSB;
	m_mapKeywordMap["cmpsw"] = enmAsmTokenType_CMPSW;
	m_mapKeywordMap["stosb"] = enmAsmTokenType_STOSB;
	m_mapKeywordMap["stosw"] = enmAsmTokenType_STOSW;
	m_mapKeywordMap["lodsb"] = enmAsmTokenType_LODSB;
	m_mapKeywordMap["lodsw"] = enmAsmTokenType_LODSW;
	m_mapKeywordMap["scasb"] = enmAsmTokenType_SCASB;
	m_mapKeywordMap["scasw"] = enmAsmTokenType_SCASW;
	m_mapKeywordMap["nop"] = enmAsmTokenType_NOP;
	m_mapKeywordMap["xlat"] = enmAsmTokenType_XLAT;
	m_mapKeywordMap["pushf"] = enmAsmTokenType_PUSHF;
	m_mapKeywordMap["popf"] = enmAsmTokenType_POPF;
	m_mapKeywordMap["sahf"] = enmAsmTokenType_SAHF;
	m_mapKeywordMap["lahf"] = enmAsmTokenType_LAHF;
	m_mapKeywordMap["cbw"] = enmAsmTokenType_CBW;
	m_mapKeywordMap["cwd"] = enmAsmTokenType_CWD;
	m_mapKeywordMap["cmc"] = enmAsmTokenType_CMC;
	m_mapKeywordMap["clc"] = enmAsmTokenType_CLC;
	m_mapKeywordMap["stc"] = enmAsmTokenType_STC;
	m_mapKeywordMap["cli"] = enmAsmTokenType_CLI;
	m_mapKeywordMap["sti"] = enmAsmTokenType_STI;
	m_mapKeywordMap["cld"] = enmAsmTokenType_CLD;
	m_mapKeywordMap["std"] = enmAsmTokenType_STD;
	m_mapKeywordMap["daa"] = enmAsmTokenType_DAA;
	m_mapKeywordMap["aaa"] = enmAsmTokenType_AAA;
	m_mapKeywordMap["das"] = enmAsmTokenType_DAS;
	m_mapKeywordMap["aas"] = enmAsmTokenType_AAS;
	m_mapKeywordMap["aam"] = enmAsmTokenType_AAM;
	m_mapKeywordMap["aad"] = enmAsmTokenType_AAD;
	m_mapKeywordMap["iret"] = enmAsmTokenType_IRET;
	m_mapKeywordMap["into"] = enmAsmTokenType_INTO;
	m_mapKeywordMap["wait"] = enmAsmTokenType_WAIT;
	m_mapKeywordMap["lock"] = enmAsmTokenType_LOCK;
	m_mapKeywordMap["repnz"] = enmAsmTokenType_REPNZ;
	m_mapKeywordMap["repne"] = enmAsmTokenType_REPNE;
	m_mapKeywordMap["repz"] = enmAsmTokenType_REPZ;
	m_mapKeywordMap["repe"] = enmAsmTokenType_REPE;
	m_mapKeywordMap["rep"] = enmAsmTokenType_REP;
	m_mapKeywordMap["hlt"] = enmAsmTokenType_HLT;
	m_mapKeywordMap["jo"] = enmAsmTokenType_JO;
	m_mapKeywordMap["jno"] = enmAsmTokenType_JNO;
	m_mapKeywordMap["jb"] = enmAsmTokenType_JB;
	m_mapKeywordMap["jnb"] = enmAsmTokenType_JNB;
	m_mapKeywordMap["jz"] = enmAsmTokenType_JZ;
	m_mapKeywordMap["je"] = enmAsmTokenType_JE;
	m_mapKeywordMap["jnz"] = enmAsmTokenType_JNZ;
	m_mapKeywordMap["jne"] = enmAsmTokenType_JNE;
	m_mapKeywordMap["jna"] = enmAsmTokenType_JNA;
	m_mapKeywordMap["jbe"] = enmAsmTokenType_JBE;
	m_mapKeywordMap["ja"] = enmAsmTokenType_JA;
	m_mapKeywordMap["js"] = enmAsmTokenType_JS;
	m_mapKeywordMap["jns"] = enmAsmTokenType_JNS;
	m_mapKeywordMap["jp"] = enmAsmTokenType_JP;
	m_mapKeywordMap["jnp"] = enmAsmTokenType_JNP;
	m_mapKeywordMap["jl"] = enmAsmTokenType_JL;
	m_mapKeywordMap["jnl"] = enmAsmTokenType_JNL;
	m_mapKeywordMap["jge"] = enmAsmTokenType_JGE;
	m_mapKeywordMap["jng"] = enmAsmTokenType_JNG;
	m_mapKeywordMap["jle"] = enmAsmTokenType_JLE;
	m_mapKeywordMap["jg"] = enmAsmTokenType_JG;
	m_mapKeywordMap["loopnz"] = enmAsmTokenType_LOOPNZ;
	m_mapKeywordMap["loopne"] = enmAsmTokenType_LOOPNE;
	m_mapKeywordMap["loopz"] = enmAsmTokenType_LOOPZ;
	m_mapKeywordMap["loope"] = enmAsmTokenType_LOOPE;
	m_mapKeywordMap["loop"] = enmAsmTokenType_LOOP;
	m_mapKeywordMap["jcxz"] = enmAsmTokenType_JCXZ;
	m_mapKeywordMap["call"] = enmAsmTokenType_CALL;
	m_mapKeywordMap["jmp"] = enmAsmTokenType_JMP;
	m_mapKeywordMap["short"] = enmAsmTokenType_SHORT;
	m_mapKeywordMap["near"] = enmAsmTokenType_NEAR;
	m_mapKeywordMap["far"] = enmAsmTokenType_FAR;
	m_mapKeywordMap["ret"] = enmAsmTokenType_RET;
	m_mapKeywordMap["retf"] = enmAsmTokenType_RETF;
	m_mapKeywordMap["int"] = enmAsmTokenType_INT;
	m_mapKeywordMap["org"] = enmAsmTokenType_ORG;
	m_mapKeywordMap["equ"] = enmAsmTokenType_EQU;
	m_mapKeywordMap["label"] = enmAsmTokenType_LABEL;
	m_mapKeywordMap["db"] = enmAsmTokenType_DB;
	m_mapKeywordMap["dw"] = enmAsmTokenType_DW;
	m_mapKeywordMap["dd"] = enmAsmTokenType_DD;
	m_mapKeywordMap["byte"] = enmAsmTokenType_BYTE;
	m_mapKeywordMap["word"] = enmAsmTokenType_WORD;
	m_mapKeywordMap["dword"] = enmAsmTokenType_DWORD;
	m_mapKeywordMap["ptr"] = enmAsmTokenType_PTR;
	m_mapKeywordMap["offset"] = enmAsmTokenType_OFFSET;
	m_mapKeywordMap["proc"] = enmAsmTokenType_PROC;
	m_mapKeywordMap["endp"] = enmAsmTokenType_ENDP;
}

BOOL CAsmScanner::OpenSource(const WCHAR *pszFilename)
{
	CloseSource();

	ResetSource();

	BOOL b = ReadAnsiFile(pszFilename, &m_pszSource);

	if ( m_pszSource )
	{
		m_nSourceLength = strlen(m_pszSource);
	}

	return b;
}

void CAsmScanner::CloseSource(void)
{
	if ( m_pszSource )
	{
		delete [] m_pszSource;
		m_pszSource = NULL;
	}
}

void CAsmScanner::ResetSource(void)
{
	m_nCharIndex = 0;
}

char CAsmScanner::GetChar(void)
{
	if ( m_nCharIndex >= m_nSourceLength )
	{
		return EOF_ANSI;
	}

	char chChar = m_pszSource[m_nCharIndex];

	m_nCharIndex++;

	if ( chChar == '\x0A' )
	{
		m_nLineNumber++;
	}

	return chChar;
}

void CAsmScanner::UnGetChar(void)
{
	if ( m_nCharIndex >= m_nSourceLength )
	{
		return;
	}

	if ( m_nCharIndex > 0 )
	{
		m_nCharIndex--;
	}
}

BOOL CAsmScanner::IsHexDigit(char chChar)
{
	if ( (chChar >= '0' && chChar <= '9') || (chChar >= 'a' && chChar <= 'f') || (chChar >= 'A' && chChar <= 'F') )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

enmAsmTokenType CAsmScanner::GetToken(void)
{
	enmAsmScannerState eScannerState = enmAsmScannerState_Start;

	m_eTokenType = enmAsmTokenType_Unknown;

	m_szTokenString[0] = '\0';

	int nTokenCharIndex = 0;

	m_nTokenStartLineNumber = m_nLineNumber;

	m_nTokenStartCharIndex = m_nCharIndex;

	while ( eScannerState != enmAsmScannerState_End )
	{
		char chChar = GetChar();

		BOOL bSaveChar = FALSE;

		switch ( eScannerState )
		{
		case enmAsmScannerState_Start:
			if ( chChar == ' ' || chChar == '\t' || chChar == '\x0D' )
			{
				break;
			}
			switch ( chChar )
			{
			case '?':
				m_eTokenType = enmAsmTokenType_RandomNumber;
				eScannerState = enmAsmScannerState_End;
				break;
			case ',':
				m_eTokenType = enmAsmTokenType_Comma;
				eScannerState = enmAsmScannerState_End;
				break;
			case ':':
				m_eTokenType = enmAsmTokenType_Colon;
				eScannerState = enmAsmScannerState_End;
				break;
			case '\n':
				m_eTokenType = enmAsmTokenType_NewLine;
				eScannerState = enmAsmScannerState_End;
				break;
			case ';':
				m_eTokenType = enmAsmTokenType_NewLine;
				eScannerState = enmAsmScannerState_InComment1;
				break;
			case '+':
				m_eTokenType = enmAsmTokenType_Plus;
				eScannerState = enmAsmScannerState_End;
				break;				
			case '-':
				m_eTokenType = enmAsmTokenType_Minus;
				eScannerState = enmAsmScannerState_End;
				break;				
			case '*':
				m_eTokenType = enmAsmTokenType_Times;
				eScannerState = enmAsmScannerState_End;
				break;				
			case '.':
				m_eTokenType = enmAsmTokenType_Dot;
				eScannerState = enmAsmScannerState_End;
				break;				
			case '[':
				m_eTokenType = enmAsmTokenType_LeftParen2;
				eScannerState = enmAsmScannerState_End;
				break;
			case ']':
				m_eTokenType = enmAsmTokenType_RightParen2;
				eScannerState = enmAsmScannerState_End;
				break;
			case EOF_ANSI:
				m_eTokenType = enmAsmTokenType_Eof;
				eScannerState = enmAsmScannerState_End;
				break;
			}
			if ( isalpha(chChar) || chChar == '@' )
			{
				bSaveChar = TRUE;
				m_eTokenType = enmAsmTokenType_Identifier;
				eScannerState = enmAsmScannerState_InIdentifier;
			}
			else if ( isdigit(chChar) )
			{
				bSaveChar = TRUE;
				m_eTokenType = enmAsmTokenType_Number;
				eScannerState = enmAsmScannerState_InNumber;
			}
			break;
		case enmAsmScannerState_InComment1:
			if ( chChar == '\n' )
			{
				m_eTokenType = enmAsmTokenType_NewLine;
				eScannerState = enmAsmScannerState_End;
			}
			else if ( chChar == EOF_ANSI )
			{
				m_eTokenType = enmAsmTokenType_Eof;
				eScannerState = enmAsmScannerState_End;
			}
			else
			{
				m_eTokenType = enmAsmTokenType_NewLine;
				eScannerState = enmAsmScannerState_InComment1;
			}
			break;
		case enmAsmScannerState_InIdentifier:
			if ( isalpha(chChar) || isdigit(chChar) || chChar == '_' )
			{
				bSaveChar = TRUE;
				eScannerState = enmAsmScannerState_InIdentifier;
			}
			else
			{
				UnGetChar();
				eScannerState = enmAsmScannerState_End;
			}
			break;
		case enmAsmScannerState_InNumber:
			if ( IsHexDigit(chChar) )
			{
				bSaveChar = TRUE;
				eScannerState = enmAsmScannerState_InNumber;
			}
			else if ( chChar == 'h' || chChar == 'H' )
			{
				bSaveChar = TRUE;
				eScannerState = enmAsmScannerState_End;
			}
			else if ( chChar == 'b' || chChar == 'B')
			{
				bSaveChar = TRUE;
				eScannerState = enmAsmScannerState_End;
			}
			else
			{
				UnGetChar();
				eScannerState = enmAsmScannerState_End;
			}
			break;
		}

		if ( bSaveChar )
		{
			m_szTokenString[nTokenCharIndex++] = chChar;
		}
	}

	m_szTokenString[nTokenCharIndex++] = '\0';

	if ( m_eTokenType == enmAsmTokenType_Identifier )
	{
		std::map<std::string,enmAsmTokenType>::iterator it;
		std::string szTokenString_Lower = StringToLower(m_szTokenString);
		it = m_mapKeywordMap.find(szTokenString_Lower);
		if ( it != m_mapKeywordMap.end() )
		{
			m_eTokenType = it->second;
		}
	}

	m_nTokenEndLineNumber = m_nLineNumber;

	m_nTokenEndCharIndex = m_nCharIndex;

	return m_eTokenType;
}

void CAsmScanner::BuildTokenList(void)
{
	ResetSource();

	m_vecTokenList.clear();

	while ( TRUE )
	{
		enmAsmTokenType eTokenType = GetToken();

		tagAsmToken *pToken = new tagAsmToken();

		pToken->eTokenType = eTokenType;

		pToken->szTokenString = m_szTokenString;

		pToken->nStartLineNumber = m_nTokenStartLineNumber;

		pToken->nEndLineNumber = m_nTokenEndLineNumber;

		pToken->nStartCharIndex = m_nTokenStartCharIndex;

		pToken->nEndCharIndex = m_nTokenEndCharIndex;

		m_vecTokenList.push_back(pToken);

		if ( eTokenType == enmAsmTokenType_Eof )
		{
			break;
		}
	}

	m_nCurrentTokenIndex = 0;
}

int CAsmScanner::GetCurrentTokenIndex(void)
{
	return m_nCurrentTokenIndex;
}

void CAsmScanner::SetCurrentTokenIndex(int nCurrentTokenIndex)
{
	this->m_nCurrentTokenIndex = nCurrentTokenIndex;
}

tagAsmToken *CAsmScanner::GetCurrentToken(void)
{
	if ( m_nCurrentTokenIndex < (int)m_vecTokenList.size() )
	{
		ProcessMacroExpand();

		return m_vecTokenList[m_nCurrentTokenIndex];
	}
	else
	{
		return m_vecTokenList[m_vecTokenList.size() - 1];
	}
}

void CAsmScanner::PrintTokenList(void)
{
	int i;

	for ( i = 0; i < (int)m_vecTokenList.size(); i++ )
	{
		tagAsmToken *pToken = m_vecTokenList[i];

		enmAsmTokenType eTokenType = pToken->eTokenType;

		char *pszTokenString = (char *)(pToken->szTokenString.c_str());

		PrintTokenType(eTokenType, pszTokenString);

		printf("\n");

		if ( eTokenType == enmAsmTokenType_Eof )
		{
			break;
		}
	}
}

void CAsmScanner::PrintTokenType(enmAsmTokenType eTokenType,const char *pszTokenString)
{
	switch ( eTokenType )
	{
	case enmAsmTokenType_EAX:
		printf("enmAsmTokenType_EAX");
		break;
	case enmAsmTokenType_ECX:
		printf("enmAsmTokenType_ECX");
		break;
	case enmAsmTokenType_EDX:
		printf("enmAsmTokenType_EDX");
		break;
	case enmAsmTokenType_EBX:
		printf("enmAsmTokenType_EBX");
		break;
	case enmAsmTokenType_ESP:
		printf("enmAsmTokenType_ESP");
		break;
	case enmAsmTokenType_EBP:
		printf("enmAsmTokenType_EBP");
		break;
	case enmAsmTokenType_ESI:
		printf("enmAsmTokenType_ESI");
		break;
	case enmAsmTokenType_EDI:
		printf("enmAsmTokenType_EDI");
		break;
	case enmAsmTokenType_AX:
		printf("enmAsmTokenType_AX");
		break;
	case enmAsmTokenType_CX:
		printf("enmAsmTokenType_CX");
		break;
	case enmAsmTokenType_DX:
		printf("enmAsmTokenType_DX");
		break;
	case enmAsmTokenType_BX:
		printf("enmAsmTokenType_BX");
		break;
	case enmAsmTokenType_SP:
		printf("enmAsmTokenType_SP");
		break;
	case enmAsmTokenType_BP:
		printf("enmAsmTokenType_BP");
		break;
	case enmAsmTokenType_SI:
		printf("enmAsmTokenType_SI");
		break;
	case enmAsmTokenType_DI:
		printf("enmAsmTokenType_DI");
		break;
	case enmAsmTokenType_AL:
		printf("enmAsmTokenType_AL");
		break;
	case enmAsmTokenType_CL:
		printf("enmAsmTokenType_CL");
		break;
	case enmAsmTokenType_DL:
		printf("enmAsmTokenType_DL");
		break;
	case enmAsmTokenType_BL:
		printf("enmAsmTokenType_BL");
		break;
	case enmAsmTokenType_AH:
		printf("enmAsmTokenType_AH");
		break;
	case enmAsmTokenType_CH:
		printf("enmAsmTokenType_CH");
		break;
	case enmAsmTokenType_DH:
		printf("enmAsmTokenType_DH");
		break;
	case enmAsmTokenType_BH:
		printf("enmAsmTokenType_BH");
		break;
	case enmAsmTokenType_ES:
		printf("enmAsmTokenType_ES");
		break;
	case enmAsmTokenType_CS:
		printf("enmAsmTokenType_CS");
		break;
	case enmAsmTokenType_SS:
		printf("enmAsmTokenType_SS");
		break;
	case enmAsmTokenType_DS:
		printf("enmAsmTokenType_DS");
		break;
	case enmAsmTokenType_ADD:
		printf("enmAsmTokenType_ADD");
		break;
	case enmAsmTokenType_ADC:
		printf("enmAsmTokenType_ADC");
		break;
	case enmAsmTokenType_SBB:
		printf("enmAsmTokenType_SBB");
		break;
	case enmAsmTokenType_SUB:
		printf("enmAsmTokenType_SUB");
		break;
	case enmAsmTokenType_CMP:
		printf("enmAsmTokenType_CMP");
		break;
	case enmAsmTokenType_TEST:
		printf("enmAsmTokenType_TEST");
		break;
	case enmAsmTokenType_AND:
		printf("enmAsmTokenType_AND");
		break;
	case enmAsmTokenType_OR:
		printf("enmAsmTokenType_OR");
		break;
	case enmAsmTokenType_XOR:
		printf("enmAsmTokenType_XOR");
		break;
	case enmAsmTokenType_MOV:
		printf("enmAsmTokenType_MOV");
		break;
	case enmAsmTokenType_XCHG:
		printf("enmAsmTokenType_XCHG");
		break;
	case enmAsmTokenType_LEA:
		printf("enmAsmTokenType_LEA");
		break;
	case enmAsmTokenType_LES:
		printf("enmAsmTokenType_LES");
		break;
	case enmAsmTokenType_LDS:
		printf("enmAsmTokenType_LDS");
		break;
	case enmAsmTokenType_INC:
		printf("enmAsmTokenType_INC");
		break;
	case enmAsmTokenType_DEC:
		printf("enmAsmTokenType_DEC");
		break;
	case enmAsmTokenType_NOT:
		printf("enmAsmTokenType_NOT");
		break;
	case enmAsmTokenType_NEG:
		printf("enmAsmTokenType_NEG");
		break;
	case enmAsmTokenType_MUL:
		printf("enmAsmTokenType_MUL");
		break;
	case enmAsmTokenType_DIV:
		printf("enmAsmTokenType_DIV");
		break;
	case enmAsmTokenType_IMUL:
		printf("enmAsmTokenType_IMUL");
		break;
	case enmAsmTokenType_IDIV:
		printf("enmAsmTokenType_IDIV");
		break;
	case enmAsmTokenType_PUSH:
		printf("enmAsmTokenType_PUSH");
		break;
	case enmAsmTokenType_POP:
		printf("enmAsmTokenType_POP");
		break;
	case enmAsmTokenType_ROL:
		printf("enmAsmTokenType_ROL");
		break;
	case enmAsmTokenType_ROR:
		printf("enmAsmTokenType_ROR");
		break;
	case enmAsmTokenType_RCL:
		printf("enmAsmTokenType_RCL");
		break;
	case enmAsmTokenType_RCR:
		printf("enmAsmTokenType_RCR");
		break;
	case enmAsmTokenType_SHL:
		printf("enmAsmTokenType_SHL");
		break;
	case enmAsmTokenType_SHR:
		printf("enmAsmTokenType_SHR");
		break;
	case enmAsmTokenType_SAR:
		printf("enmAsmTokenType_SAR");
		break;
	case enmAsmTokenType_IN:
		printf("enmAsmTokenType_IN");
		break;
	case enmAsmTokenType_OUT:
		printf("enmAsmTokenType_OUT");
		break;
	case enmAsmTokenType_MOVSB:
		printf("enmAsmTokenType_MOVSB");
		break;
	case enmAsmTokenType_MOVSW:
		printf("enmAsmTokenType_MOVSW");
		break;
	case enmAsmTokenType_CMPSB:
		printf("enmAsmTokenType_CMPSB");
		break;
	case enmAsmTokenType_CMPSW:
		printf("enmAsmTokenType_CMPSW");
		break;
	case enmAsmTokenType_STOSB:
		printf("enmAsmTokenType_STOSB");
		break;
	case enmAsmTokenType_STOSW:
		printf("enmAsmTokenType_STOSW");
		break;
	case enmAsmTokenType_LODSB:
		printf("enmAsmTokenType_LODSB");
		break;
	case enmAsmTokenType_LODSW:
		printf("enmAsmTokenType_LODSW");
		break;
	case enmAsmTokenType_SCASB:
		printf("enmAsmTokenType_SCASB");
		break;
	case enmAsmTokenType_SCASW:
		printf("enmAsmTokenType_SCASW");
		break;
	case enmAsmTokenType_NOP:
		printf("enmAsmTokenType_NOP");
		break;
	case enmAsmTokenType_XLAT:
		printf("enmAsmTokenType_XLAT");
		break;
	case enmAsmTokenType_PUSHF:
		printf("enmAsmTokenType_PUSHF");
		break;
	case enmAsmTokenType_POPF:
		printf("enmAsmTokenType_POPF");
		break;
	case enmAsmTokenType_SAHF:
		printf("enmAsmTokenType_SAHF");
		break;
	case enmAsmTokenType_LAHF:
		printf("enmAsmTokenType_LAHF");
		break;
	case enmAsmTokenType_CBW:
		printf("enmAsmTokenType_CBW");
		break;
	case enmAsmTokenType_CWD:
		printf("enmAsmTokenType_CWD");
		break;
	case enmAsmTokenType_CMC:
		printf("enmAsmTokenType_CMC");
		break;
	case enmAsmTokenType_CLC:
		printf("enmAsmTokenType_CLC");
		break;
	case enmAsmTokenType_STC:
		printf("enmAsmTokenType_STC");
		break;
	case enmAsmTokenType_CLI:
		printf("enmAsmTokenType_CLI");
		break;
	case enmAsmTokenType_STI:
		printf("enmAsmTokenType_STI");
		break;
	case enmAsmTokenType_CLD:
		printf("enmAsmTokenType_CLD");
		break;
	case enmAsmTokenType_STD:
		printf("enmAsmTokenType_STD");
		break;
	case enmAsmTokenType_DAA:
		printf("enmAsmTokenType_DAA");
		break;
	case enmAsmTokenType_AAA:
		printf("enmAsmTokenType_AAA");
		break;
	case enmAsmTokenType_DAS:
		printf("enmAsmTokenType_DAS");
		break;
	case enmAsmTokenType_AAS:
		printf("enmAsmTokenType_AAS");
		break;
	case enmAsmTokenType_AAM:
		printf("enmAsmTokenType_AAM");
		break;
	case enmAsmTokenType_AAD:
		printf("enmAsmTokenType_AAD");
		break;
	case enmAsmTokenType_IRET:
		printf("enmAsmTokenType_IRET");
		break;
	case enmAsmTokenType_INTO:
		printf("enmAsmTokenType_INTO");
		break;
	case enmAsmTokenType_WAIT:
		printf("enmAsmTokenType_WAIT");
		break;
	case enmAsmTokenType_LOCK:
		printf("enmAsmTokenType_LOCK");
		break;
	case enmAsmTokenType_REPNZ:
		printf("enmAsmTokenType_REPNZ");
		break;
	case enmAsmTokenType_REPNE:
		printf("enmAsmTokenType_REPNE");
		break;
	case enmAsmTokenType_REPZ:
		printf("enmAsmTokenType_REPZ");
		break;
	case enmAsmTokenType_REPE:
		printf("enmAsmTokenType_REPE");
		break;
	case enmAsmTokenType_REP:
		printf("enmAsmTokenType_REP");
		break;
	case enmAsmTokenType_HLT:
		printf("enmAsmTokenType_HLT");
		break;
	case enmAsmTokenType_JO:
		printf("enmAsmTokenType_JO");
		break;
	case enmAsmTokenType_JNO:
		printf("enmAsmTokenType_JNO");
		break;
	case enmAsmTokenType_JB:
		printf("enmAsmTokenType_JB");
		break;
	case enmAsmTokenType_JZ:
		printf("enmAsmTokenType_JZ");
		break;
	case enmAsmTokenType_JE:
		printf("enmAsmTokenType_JE");
		break;
	case enmAsmTokenType_JNZ:
		printf("enmAsmTokenType_JNZ");
		break;
	case enmAsmTokenType_JNE:
		printf("enmAsmTokenType_JNE");
		break;
	case enmAsmTokenType_JNA:
		printf("enmAsmTokenType_JNA");
		break;
	case enmAsmTokenType_JBE:
		printf("enmAsmTokenType_JBE");
		break;
	case enmAsmTokenType_JA:
		printf("enmAsmTokenType_JA");
		break;
	case enmAsmTokenType_JS:
		printf("enmAsmTokenType_JS");
		break;
	case enmAsmTokenType_JNS:
		printf("enmAsmTokenType_JNS");
		break;
	case enmAsmTokenType_JP:
		printf("enmAsmTokenType_JP");
		break;
	case enmAsmTokenType_JNP:
		printf("enmAsmTokenType_JNP");
		break;
	case enmAsmTokenType_JL:
		printf("enmAsmTokenType_JL");
		break;
	case enmAsmTokenType_JNL:
		printf("enmAsmTokenType_JNL");
		break;
	case enmAsmTokenType_JGE:
		printf("enmAsmTokenType_JGE");
		break;
	case enmAsmTokenType_JNG:
		printf("enmAsmTokenType_JNG");
		break;
	case enmAsmTokenType_JLE:
		printf("enmAsmTokenType_JLE");
		break;
	case enmAsmTokenType_JG:
		printf("enmAsmTokenType_JG");
		break;
	case enmAsmTokenType_LOOPNZ:
		printf("enmAsmTokenType_LOOPNZ");
		break;
	case enmAsmTokenType_LOOPNE:
		printf("enmAsmTokenType_LOOPNE");
		break;
	case enmAsmTokenType_LOOPZ:
		printf("enmAsmTokenType_LOOPZ");
		break;
	case enmAsmTokenType_LOOPE:
		printf("enmAsmTokenType_LOOPE");
		break;
	case enmAsmTokenType_LOOP:
		printf("enmAsmTokenType_LOOP");
		break;
	case enmAsmTokenType_JCXZ:
		printf("enmAsmTokenType_JCXZ");
		break;
	case enmAsmTokenType_CALL:
		printf("enmAsmTokenType_CALL");
		break;
	case enmAsmTokenType_JMP:
		printf("enmAsmTokenType_JMP");
		break;
	case enmAsmTokenType_SHORT:
		printf("enmAsmTokenType_SHORT");
		break;
	case enmAsmTokenType_NEAR:
		printf("enmAsmTokenType_NEAR");
		break;
	case enmAsmTokenType_FAR:
		printf("enmAsmTokenType_FAR");
		break;
	case enmAsmTokenType_RET:
		printf("enmAsmTokenType_RET");
		break;
	case enmAsmTokenType_RETF:
		printf("enmAsmTokenType_RETF");
		break;
	case enmAsmTokenType_INT:
		printf("enmAsmTokenType_INT");
		break;
	case enmAsmTokenType_ORG:
		printf("enmAsmTokenType_ORG");
		break;
	case enmAsmTokenType_EQU:
		printf("enmAsmTokenType_EQU");
		break;
	case enmAsmTokenType_LABEL:
		printf("enmAsmTokenType_LABEL");
		break;
	case enmAsmTokenType_DB:
		printf("enmAsmTokenType_DB");
		break;
	case enmAsmTokenType_DW:
		printf("enmAsmTokenType_DW");
		break;
	case enmAsmTokenType_DD:
		printf("enmAsmTokenType_DD");
		break;
	case enmAsmTokenType_BYTE:
		printf("enmAsmTokenType_BYTE");
		break;
	case enmAsmTokenType_WORD:
		printf("enmAsmTokenType_WORD");
		break;
	case enmAsmTokenType_DWORD:
		printf("enmAsmTokenType_DWORD");
		break;
	case enmAsmTokenType_PTR:
		printf("enmAsmTokenType_PTR");
		break;
	case enmAsmTokenType_OFFSET:
		printf("enmAsmTokenType_OFFSET");
		break;
	case enmAsmTokenType_PROC:
		printf("enmAsmTokenType_PROC");
		break;
	case enmAsmTokenType_ENDP:
		printf("enmAsmTokenType_ENDP");
		break;
	case enmAsmTokenType_Identifier:
		printf("enmAsmTokenType_Identifier:%s", pszTokenString);
		break;
	case enmAsmTokenType_Number:
		printf("enmAsmTokenType_Number:%s", pszTokenString);
		break;
	case enmAsmTokenType_RandomNumber:
		printf("enmAsmTokenType_RandomNumber");
		break;
	case enmAsmTokenType_Comma:
		printf("enmAsmTokenType_Comma");
		break;
	case enmAsmTokenType_Colon:
		printf("enmAsmTokenType_Colon");
		break;
	case enmAsmTokenType_Dot:
		printf("enmAsmTokenType_Dot");
		break;
	case enmAsmTokenType_Plus:
		printf("enmAsmTokenType_Plus");
		break;
	case enmAsmTokenType_Minus:
		printf("enmAsmTokenType_Minus");
		break;
	case enmAsmTokenType_Times:
		printf("enmAsmTokenType_Times");
		break;
	case enmAsmTokenType_LeftParen2:
		printf("enmAsmTokenType_LeftParen2");
		break;
	case enmAsmTokenType_RightParen2:
		printf("enmAsmTokenType_RightParen2");
		break;
	case enmAsmTokenType_NewLine:
		printf("enmAsmTokenType_NewLine");
		break;
	case enmAsmTokenType_Eof:
		printf("enmAsmTokenType_Eof");
		break;
	default:
		printf("enmAsmTokenType_Unknown");
		break;
	}
}

tagAsmToken *CAsmScanner::CopyToken(tagAsmToken *pToken)
{
	tagAsmToken *pToken1 = new tagAsmToken();

	pToken1->eTokenType = pToken->eTokenType;
	pToken1->szTokenString = pToken->szTokenString;
	pToken1->nStartLineNumber = pToken->nStartLineNumber;
	pToken1->nEndLineNumber = pToken->nEndLineNumber;
	pToken1->nStartCharIndex = pToken->nStartCharIndex;
	pToken1->nEndCharIndex = pToken->nEndCharIndex;

	return pToken1;
}

void CAsmScanner::AddMacro(CAsmMacro *pMacro)
{
	if ( pMacro->m_szMacroName.length() > 0 && pMacro->m_szMacroName[0] == '@' )
	{
		m_vecLocalMacroList.push_back(pMacro);
	}

	m_vecMacroMap[pMacro->m_szMacroName] = pMacro;
}

void CAsmScanner::ClearLocalMacros()
{
	int i;
	for ( i = m_vecLocalMacroList.size() - 1; i >= 0; i-- )
	{
		CAsmMacro *pMacro = m_vecLocalMacroList[i];

		m_vecMacroMap.erase(pMacro->m_szMacroName);

		delete pMacro;
	}

	m_vecLocalMacroList.clear();
}

CAsmMacro *CAsmScanner::FindMacro(const char *pszMacroName)
{
	std::map<std::string,CAsmMacro *>::iterator it;
	it = m_vecMacroMap.find(pszMacroName);
	if ( it != m_vecMacroMap.end() )
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void CAsmScanner::ProcessMacroExpand(void)
{
	//执行宏展开

	tagAsmToken *pToken = m_vecTokenList[m_nCurrentTokenIndex];

	CAsmMacro *pMacro = FindMacro(pToken->szTokenString.c_str());

	if ( pMacro )
	{
		int nStartLineNumber = pToken->nStartLineNumber;
		int nEndLineNumber = pToken->nEndLineNumber;
		int nStartCharIndex = pToken->nStartCharIndex;
		int nEndCharIndex = pToken->nEndCharIndex;

		m_vecTokenList.erase(m_vecTokenList.begin() + m_nCurrentTokenIndex);

		int i;
		for ( i = pMacro->m_vecTokenList.size() - 1; i >= 0; i-- )
		{
			tagAsmToken *pToken1 = CopyToken(pMacro->m_vecTokenList[i]);
			pToken1->nStartLineNumber = nStartLineNumber;
			pToken1->nEndLineNumber = nEndLineNumber;
			pToken1->nStartCharIndex = nStartCharIndex;
			pToken1->nEndCharIndex = nEndCharIndex;
			m_vecTokenList.insert(m_vecTokenList.begin() + m_nCurrentTokenIndex, pToken1);
		}
	}
}

CAsmScanner *CAsmScanner_GetInstance(void)
{
	static CAsmScanner s_Scnanner;

	return &s_Scnanner;
}