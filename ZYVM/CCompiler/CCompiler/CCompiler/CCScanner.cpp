//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "CCScanner.hpp"

CCScanner::CCScanner(void)
{
	m_pszSource = NULL;
	m_nCharIndex = NULL;
	m_nLineNumber = 0;
	m_nCharIndex = 0;
	m_eTokenType = enmCTokenType_Unknown;
	m_szTokenString[0] = '\0';
	m_nTokenStartLineNumber = 0;
	m_nTokenEndLineNumber = 0;
	m_nTokenStartCharIndex = 0;
	m_nTokenEndCharIndex = 0;
	BuildKeywordTable();
	m_nCurrentTokenIndex = 0;
}

CCScanner::~CCScanner(void)
{
	CloseSource();
}

void CCScanner::BuildKeywordTable(void)
{
	m_mapKeywordMap["define"] = enmCTokenType_Define;
	m_mapKeywordMap["int"] = enmCTokenType_Int;
	m_mapKeywordMap["void"] = enmCTokenType_Void;
	m_mapKeywordMap["char"] = enmCTokenType_Char;
	m_mapKeywordMap["if"] = enmCTokenType_If;
	m_mapKeywordMap["else"] = enmCTokenType_Else;
	m_mapKeywordMap["while"] = enmCTokenType_While;
	m_mapKeywordMap["do"] = enmCTokenType_Do;
	m_mapKeywordMap["for"] = enmCTokenType_For;
	m_mapKeywordMap["break"] = enmCTokenType_Break;
	m_mapKeywordMap["continue"] = enmCTokenType_Continue;
	m_mapKeywordMap["return"] = enmCTokenType_Return;
	m_mapKeywordMap["unsigned"] = enmCTokenType_Unsigned;
	m_mapKeywordMap["signed"] = enmCTokenType_Signed;
	m_mapKeywordMap["struct"] = enmCTokenType_Struct;
	m_mapKeywordMap["short"] = enmCTokenType_Short;
}

BOOL CCScanner::OpenSource(const WCHAR *pszFilename)
{
	CloseSource();

	ResetSource();

	return ReadAnsiFile(pszFilename, &m_pszSource);
}

void CCScanner::CloseSource(void)
{
	if ( m_pszSource )
	{
		delete [] m_pszSource;
		m_pszSource = NULL;
	}
}

void CCScanner::ResetSource(void)
{
	m_nCharIndex = 0;
}

char CCScanner::GetChar(void)
{
	char ch = m_pszSource[m_nCharIndex];

	if ( ch != '\x00' )
	{
		m_nCharIndex++;

		if ( ch == '\x0A' )
		{
			m_nLineNumber++;
		}

		return ch;
	}
	else
	{
		return (char)EOF_ANSI;
	}
}

void CCScanner::UnGetChar(void)
{
	if ( m_nCharIndex > 0 )
	{
		m_nCharIndex--;
	}
}

enmCTokenType CCScanner::GetToken(void)
{
	enmCScannerState eScannerState = enmCScannerState_Start;

	m_eTokenType = enmCTokenType_Unknown;

	m_szTokenString[0] = '\0';

	int nTokenCharIndex = 0;

	m_nTokenStartLineNumber = m_nLineNumber;

	m_nTokenStartCharIndex = m_nCharIndex;

	while ( eScannerState != enmCScannerState_End )
	{
		char ch = GetChar();

		BOOL bSaveChar = FALSE;

		switch ( eScannerState )
		{
		case enmCScannerState_Start:
			if ( ch == ' ' || ch == '\t' || ch == '\x0D' || ch == '\n' )
			{
				break;
			}
			switch ( ch )
			{
			case '#':
				m_eTokenType = enmCTokenType_Sharp;
				eScannerState = enmCScannerState_End;
				break;
			case ',':
				m_eTokenType = enmCTokenType_Comma;
				eScannerState = enmCScannerState_End;
				break;
			case ';':
				m_eTokenType = enmCTokenType_SemiColon;
				eScannerState = enmCScannerState_End;
				break;
			case '(':
				m_eTokenType = enmCTokenType_LeftParen1;
				eScannerState = enmCScannerState_End;
				break;
			case ')':
				m_eTokenType = enmCTokenType_RightParen1;
				eScannerState = enmCScannerState_End;
				break;
			case '[':
				m_eTokenType = enmCTokenType_LeftParen2;
				eScannerState = enmCScannerState_End;
				break;
			case ']':
				m_eTokenType = enmCTokenType_RightParen2;
				eScannerState = enmCScannerState_End;
				break;
			case '{':
				m_eTokenType = enmCTokenType_LeftParen3;
				eScannerState = enmCScannerState_End;
				break;
			case '}':
				m_eTokenType = enmCTokenType_RightParen3;
				eScannerState = enmCScannerState_End;
				break;
			case '|':
				bSaveChar = TRUE;
				eScannerState = enmCScannerState_InOr;
				break;
			case '&':
				bSaveChar = TRUE;
				eScannerState = enmCScannerState_InAnd;
				break;
			case '*':
				bSaveChar = TRUE;
				m_eTokenType = enmCTokenType_Times;
				eScannerState = enmCScannerState_End;
				break;
			case '/':
				eScannerState = enmCScannerState_InComment1;
				break;
			case '%':
				bSaveChar = TRUE;
				m_eTokenType = enmCTokenType_Times;
				eScannerState = enmCScannerState_End;
				break;
			case '=':
				bSaveChar = TRUE;
				eScannerState = enmCScannerState_InEqual;
				break;
			case '!':
				bSaveChar = TRUE;
				eScannerState = enmCScannerState_InUnequal;
				break;
			case '>':
				bSaveChar = TRUE;
				eScannerState = enmCScannerState_InGreaterEuqalTo;
				break;
			case '<':
				bSaveChar = TRUE;
				eScannerState = enmCScannerState_InLessEuqalTo;
				break;
			case '\'':
				m_eTokenType = enmCTokenType_Character;
				eScannerState = enmCScannerState_InCharacter;
				break;
			case '"':
				m_eTokenType = enmCTokenType_String;
				eScannerState = enmCScannerState_InString;
				break;
			case '+':
				bSaveChar = TRUE;
				eScannerState = enmCScannerState_InPlusPlus;
				break;
			case '-':
				bSaveChar = TRUE;
				eScannerState = enmCScannerState_InMinusMinus;
				break;
			case '.':
				m_eTokenType = enmCTokenType_Dot;
				eScannerState = enmCScannerState_End;
				break;
			case EOF_ANSI:
				m_eTokenType = enmCTokenType_Eof;
				eScannerState = enmCScannerState_End;
				break;
			}
			if ( isalpha(ch) )
			{
				bSaveChar = TRUE;
				m_eTokenType = enmCTokenType_Identifier;
				eScannerState = enmCScannerState_InIdentifier;
			}
			else if ( isdigit(ch) )
			{
				bSaveChar = TRUE;
				m_eTokenType = enmCTokenType_Number;
				eScannerState = enmCScannerState_InNumber;
			}
			break;
		case enmCScannerState_InComment1:
			if ( ch == '/' )
			{
				eScannerState = enmCScannerState_InComment2;
			}
			else
			{
				m_szTokenString[nTokenCharIndex++] = '/';
				m_eTokenType = enmCTokenType_Divide;
				UnGetChar();
				eScannerState = enmCScannerState_End;
			}
			break;
		case enmCScannerState_InComment2:
			if ( ch == '\n' )
			{
				eScannerState = enmCScannerState_Start;
			}
			if ( ch == EOF_ANSI )
			{
				m_eTokenType = enmCTokenType_Eof;
				eScannerState = enmCScannerState_End;
			}
			break;
		case enmCScannerState_InIdentifier:
			if ( isalpha(ch) || isdigit(ch) || ch == '_' )
			{
				bSaveChar = TRUE;
				eScannerState = enmCScannerState_InIdentifier;
			}
			else
			{
				UnGetChar();
				eScannerState = enmCScannerState_End;
			}
			break;
		case enmCScannerState_InNumber:
			if ( isdigit(ch) || ch == '.' || ch == 'x' )
			{
				bSaveChar = TRUE;
				eScannerState = enmCScannerState_InNumber;
			}
			else
			{
				UnGetChar();
				eScannerState = enmCScannerState_End;
			}
			break;
		case enmCScannerState_InString:
			if ( ch != '"' )
			{
				bSaveChar = TRUE;
				eScannerState = enmCScannerState_InString;
			}
			else
			{
				eScannerState = enmCScannerState_End;
			}
			break;
		case enmCScannerState_InCharacter:
			if ( ch != '\'' )
			{
				bSaveChar = TRUE;
				eScannerState = enmCScannerState_InCharacter;
			}
			else
			{
				eScannerState = enmCScannerState_End;
			}
			break;
		case enmCScannerState_InEqual:
			if ( ch == '=' )
			{
				bSaveChar = TRUE;
				m_eTokenType = enmCTokenType_Equal;
				eScannerState = enmCScannerState_End;
			}
			else
			{
				m_eTokenType = enmCTokenType_Assign;
				UnGetChar();
				eScannerState = enmCScannerState_End;
			}
			break;
		case enmCScannerState_InUnequal:
			if ( ch == '=' )
			{
				bSaveChar = TRUE;
				m_eTokenType = enmCTokenType_Unequal;
				eScannerState = enmCScannerState_End;
			}
			else
			{
				m_eTokenType = enmCTokenType_Not;
				UnGetChar();
				eScannerState = enmCScannerState_End;
			}
			break;
		case enmCScannerState_InGreaterEuqalTo:
			if(ch=='=')
			{
				bSaveChar = TRUE;
				m_eTokenType = enmCTokenType_GreaterEqualTo;
				eScannerState = enmCScannerState_End;
			}
			else
			{
				m_eTokenType = enmCTokenType_GreaterTo;
				UnGetChar();
				eScannerState = enmCScannerState_End;
			}
			break;
		case enmCScannerState_InLessEuqalTo:
			if ( ch == '=' )
			{
				bSaveChar = TRUE;
				m_eTokenType = enmCTokenType_LessEqualTo;
				eScannerState = enmCScannerState_End;
			}
			else
			{
				m_eTokenType = enmCTokenType_LessTo;
				UnGetChar();
				eScannerState = enmCScannerState_End;
			}
			break;
		case enmCScannerState_InOr:
			if ( ch == '|' )
			{
				bSaveChar = TRUE;
				m_eTokenType = enmCTokenType_Or;
				eScannerState = enmCScannerState_End;
			}
			else
			{
				m_eTokenType = enmCTokenType_Unknown;
				UnGetChar();
				eScannerState = enmCScannerState_End;
			}
			break;
		case enmCScannerState_InAnd:
			if(ch=='&')
			{
				bSaveChar = TRUE;
				m_eTokenType = enmCTokenType_And;
				eScannerState = enmCScannerState_End;
			}
			else
			{
				m_eTokenType = enmCTokenType_Reference;
				UnGetChar();
				eScannerState = enmCScannerState_End;
			}
			break;
		case enmCScannerState_InPlusPlus:
			if ( ch == '+' )
			{
				bSaveChar = TRUE;
				m_eTokenType = enmCTokenType_PlusPlus;
				eScannerState = enmCScannerState_End;
			}
			else
			{
				m_eTokenType = enmCTokenType_Plus;
				UnGetChar();
				eScannerState = enmCScannerState_End;
			}
			break;
		case enmCScannerState_InMinusMinus:
			if ( ch == '-' )
			{
				bSaveChar = TRUE;
				m_eTokenType = enmCTokenType_MinusMinus;
				eScannerState = enmCScannerState_End;
			}
			else if ( ch == '>' )
			{
				bSaveChar = TRUE;
				m_eTokenType = enmCTokenType_Pointer;
				eScannerState = enmCScannerState_End;
			}
			else
			{
				m_eTokenType = enmCTokenType_Minus;
				UnGetChar();
				eScannerState = enmCScannerState_End;
			}
			break;
		}

		if ( bSaveChar )
		{
			m_szTokenString[nTokenCharIndex++] = ch;
		}
	}

	m_szTokenString[nTokenCharIndex++] = '\0';

	if ( m_eTokenType == enmCTokenType_Identifier )
	{
		std::map<std::string,enmCTokenType>::iterator it;
		std::string s = m_szTokenString;
		it = m_mapKeywordMap.find(s);
		if ( it != m_mapKeywordMap.end() )
		{
			m_eTokenType = it->second;
		}
	}

	m_nTokenEndLineNumber = m_nLineNumber;
 
	m_nTokenEndCharIndex = m_nCharIndex;

	return m_eTokenType;
}

void CCScanner::BuildTokenList(void)
{
	ResetSource();

	m_vecTokenList.clear();

	while ( TRUE )
	{
		enmCTokenType eTokenType = GetToken();

		tagCToken *pToken = new tagCToken();

		pToken->eTokenType = eTokenType;

		pToken->szTokenString = m_szTokenString;

		pToken->nStartLineNumber = m_nTokenStartLineNumber;

		pToken->nEndLineNumber = m_nTokenEndLineNumber;

		pToken->nStartCharIndex = m_nTokenStartCharIndex;

		pToken->nEndCharIndex = m_nTokenEndCharIndex;

		m_vecTokenList.push_back(pToken);

		if ( eTokenType == enmCTokenType_Eof )
		{
			break;
		}
	}

	m_nCurrentTokenIndex = 0;
}

void CCScanner::PreProcess(void)
{
	std::vector<tagCToken *> vecNewTokenList;
}

int CCScanner::GetCurrentTokenIndex(void)
{
	return m_nCurrentTokenIndex;
}

void CCScanner::SetCurrentTokenIndex(int nCurrentTokenIndex)
{
	this->m_nCurrentTokenIndex = nCurrentTokenIndex;
}

tagCToken *CCScanner::GetCurrentToken(void)
{
	return m_vecTokenList[m_nCurrentTokenIndex];
}

void CCScanner::PrintTokenList(void)
{
	int i;

	for ( i = 0; i < (int)m_vecTokenList.size(); i++ )
	{
		tagCToken *pToken = m_vecTokenList[i];

		enmCTokenType eTokenType = pToken->eTokenType;

		char *pszTokenString = (char *)(pToken->szTokenString.c_str());

		PrintTokenType(eTokenType, pszTokenString);

		printf("\n");

		if ( eTokenType == enmCTokenType_Eof )
		{
			break;
		}
	}
}

void CCScanner::PrintTokenType(enmCTokenType eTokenType, const char *pszTokenString)
{
	switch ( eTokenType )
	{
	case enmCTokenType_Define:
		printf("enmCTokenType_Define");
		break;
	case enmCTokenType_Int:
		printf("enmCTokenType_Int");
		break;
	case enmCTokenType_Char:
		printf("enmCTokenType_Char");
		break;
	case enmCTokenType_Void:
		printf("enmCTokenType_Void");
		break;
	case enmCTokenType_If:
		printf("enmCTokenType_If");
		break;
	case enmCTokenType_Else:
		printf("enmCTokenType_Else");
		break;
	case enmCTokenType_While:
		printf("enmCTokenType_While");
		break;
	case enmCTokenType_Do:
		printf("enmCTokenType_Do");
		break;
	case enmCTokenType_For:
		printf("enmCTokenType_For");
		break;
	case enmCTokenType_Break:
		printf("enmCTokenType_Break");
		break;
	case enmCTokenType_Continue:
		printf("enmCTokenType_Continue");
		break;
	case enmCTokenType_Return:
		printf("enmCTokenType_Return");
		break;
	case enmCTokenType_Unsigned:
		printf("enmCTokenType_Unsigned");
		break;
	case enmCTokenType_Signed:
		printf("enmCTokenType_Signed");
		break;
	case enmCTokenType_Struct:
		printf("enmCTokenType_Struct");
		break;
	case enmCTokenType_Short:
		printf("enmCTokenType_Short");
		break;
	case enmCTokenType_Identifier:
		printf("enmCTokenType_Identifier:%s", pszTokenString);
		break;
	case enmCTokenType_Number:
		printf("enmCTokenType_Number:%s", pszTokenString);
		break;
	case enmCTokenType_Character:
		printf("enmCTokenType_Character:%s", pszTokenString);
		break;
	case enmCTokenType_String:
		printf("enmCTokenType_String:%s", pszTokenString);
		break;
	case enmCTokenType_Comma:
		printf("enmCTokenType_Comma");
		break;
	case enmCTokenType_SemiColon:
		printf("enmCTokenType_SemiColon");
		break;
	case enmCTokenType_LeftParen1:
		printf("enmCTokenType_LeftParen1");
		break;
	case enmCTokenType_RightParen1:
		printf("enmCTokenType_RightParen1");
		break;
	case enmCTokenType_LeftParen2:
		printf("enmCTokenType_LeftParen2");
		break;
	case enmCTokenType_RightParen2:
		printf("enmCTokenType_RightParen2");
		break;
	case enmCTokenType_LeftParen3:
		printf("enmCTokenType_LeftParen3");
		break;
	case enmCTokenType_RightParen3:
		printf("enmCTokenType_RightParen3");
		break;
	case enmCTokenType_Plus:
		printf("enmCTokenType_Plus");
		break;
	case enmCTokenType_Minus:
		printf("enmCTokenType_Minus");
		break;
	case enmCTokenType_Times:
		printf("enmCTokenType_Times");
		break;
	case enmCTokenType_Divide:
		printf("enmCTokenType_Divide");
		break;
	case enmCTokenType_Mod:
		printf("enmCTokenType_Mod");
		break;
	case enmCTokenType_Assign:
		printf("enmCTokenType_Assign");
		break;
	case enmCTokenType_Equal:
		printf("enmCTokenType_Equal");
		break;
	case enmCTokenType_Unequal:
		printf("enmCTokenType_Unequal");
		break;
	case enmCTokenType_GreaterTo:
		printf("enmCTokenType_GreaterTo");
		break;
	case enmCTokenType_LessTo:
		printf("enmCTokenType_LessTo");
		break;
	case enmCTokenType_GreaterEqualTo:
		printf("enmCTokenType_GreaterEqualTo");
		break;
	case enmCTokenType_LessEqualTo:
		printf("enmCTokenType_LessEqualTo");
		break;
	case enmCTokenType_Or:
		printf("enmCTokenType_Or");
		break;
	case enmCTokenType_And:
		printf("enmCTokenType_And");
		break;
	case enmCTokenType_Not:
		printf("enmCTokenType_Not");
		break;
	case enmCTokenType_PlusPlus:
		printf("enmCTokenType_PlusPlus");
		break;
	case enmCTokenType_MinusMinus:
		printf("enmCTokenType_MinusMinus");
		break;
	case enmCTokenType_Dot:
		printf("enmCTokenType_Dot");
		break;
	case enmCTokenType_Reference:
		printf("enmCTokenType_Reference");
		break;
	case enmCTokenType_Pointer:
		printf("enmCTokenType_Pointer");
		break;
	case enmCTokenType_Eof:
		printf("enmCTokenType_Eof");
		break;
	default:
		printf("enmCTokenType_Unknown");
		break;
	}
}

CCScanner *CCScanner_GetInstance(void)
{
	static CCScanner s_CScnanner;

	return &s_CScnanner;
}