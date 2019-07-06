//---------------------------------------------------------------------------------------------------------
// Virtual Machine - C Compiler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Common.hpp"

//基于有限状态机的词法分析器

#define EOF_ANSI (char)0xFF

enum enmCTokenType
{
	enmCTokenType_Unknown,
	enmCTokenType_Sharp,
	enmCTokenType_Define,
	enmCTokenType_Int,
	enmCTokenType_Char,
	enmCTokenType_Void,
	enmCTokenType_If,
	enmCTokenType_Else,
	enmCTokenType_While,
	enmCTokenType_Do,
	enmCTokenType_For,
	enmCTokenType_Break,
	enmCTokenType_Continue,
	enmCTokenType_Return,
	enmCTokenType_Unsigned,
	enmCTokenType_Signed,
	enmCTokenType_Struct,
	enmCTokenType_Short,
	enmCTokenType_Identifier,
	enmCTokenType_Number,
	enmCTokenType_Character,
	enmCTokenType_String,
	enmCTokenType_Comma,
	enmCTokenType_SemiColon,
	enmCTokenType_LeftParen1,
	enmCTokenType_RightParen1,
	enmCTokenType_LeftParen2,
	enmCTokenType_RightParen2,
	enmCTokenType_LeftParen3,
	enmCTokenType_RightParen3,
	enmCTokenType_Plus,
	enmCTokenType_PlusPlus,
	enmCTokenType_Minus,
	enmCTokenType_MinusMinus,
	enmCTokenType_Times,
	enmCTokenType_Divide,
	enmCTokenType_Mod,
	enmCTokenType_Assign,
	enmCTokenType_Equal,
	enmCTokenType_Unequal,
	enmCTokenType_GreaterTo,
	enmCTokenType_LessTo,
	enmCTokenType_GreaterEqualTo,
	enmCTokenType_LessEqualTo,
	enmCTokenType_Or,
	enmCTokenType_And,
	enmCTokenType_Not,
	enmCTokenType_Dot,
	enmCTokenType_Reference,
	enmCTokenType_Pointer,
	enmCTokenType_Eof,
};

enum enmCScannerState
{
	enmCScannerState_Start,
	enmCScannerState_End,
	enmCScannerState_InComment1,
	enmCScannerState_InComment2,
	enmCScannerState_InIdentifier,
	enmCScannerState_InString,
	enmCScannerState_InNumber,
	enmCScannerState_InCharacter,
	enmCScannerState_InEqual,
	enmCScannerState_InUnequal,
	enmCScannerState_InGreaterEuqalTo,
	enmCScannerState_InLessEuqalTo,
	enmCScannerState_InOr,
	enmCScannerState_InAnd,
	enmCScannerState_InPlusPlus,
	enmCScannerState_InMinusMinus,
};

struct tagCToken
{
public:
	enmCTokenType eTokenType;
	std::string szTokenString;
	int nStartLineNumber;
	int nEndLineNumber;
	int nStartCharIndex;
	int nEndCharIndex;
};

class CCScanner
{
public:
	CCScanner(void);
	~CCScanner(void);
	BOOL OpenSource(const WCHAR *pszFilename);
	void CloseSource(void);
	void BuildTokenList(void);
	void PreProcess(void);
	int GetCurrentTokenIndex(void);
	void SetCurrentTokenIndex(int nCurrentTokenIndex);
	tagCToken *GetCurrentToken(void);
	void PrintTokenList(void);
	void PrintTokenType(enmCTokenType eTokenType, const char *pszTokenString);
private:
	void BuildKeywordTable(void);
	void ResetSource(void);
	char GetChar(void);
	void UnGetChar(void);
	enmCTokenType GetToken(void);
	char *m_pszSource;
	int m_nLineNumber;
	int m_nCharIndex;
	enmCTokenType m_eTokenType;
	char m_szTokenString[1000];
	int m_nTokenStartLineNumber;
	int m_nTokenEndLineNumber;
	int m_nTokenStartCharIndex;
	int m_nTokenEndCharIndex;
	std::map<std::string,enmCTokenType> m_mapKeywordMap;
	std::vector<tagCToken *> m_vecTokenList;
	int m_nCurrentTokenIndex;
};

CCScanner *CCScanner_GetInstance(void);