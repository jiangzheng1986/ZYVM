//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Common.hpp"

//基于有限状态机的词法分析器

#define EOF_ANSI (char)0xFF

enum enmAsmTokenType
{
	enmAsmTokenType_Unknown,
	enmAsmTokenType_EAX,
	enmAsmTokenType_ECX,
	enmAsmTokenType_EDX,
	enmAsmTokenType_EBX,
	enmAsmTokenType_ESP,
	enmAsmTokenType_EBP,
	enmAsmTokenType_ESI,
	enmAsmTokenType_EDI,
	enmAsmTokenType_AX,
	enmAsmTokenType_CX,
	enmAsmTokenType_DX,
	enmAsmTokenType_BX,
	enmAsmTokenType_SP,
	enmAsmTokenType_BP,
	enmAsmTokenType_SI,
	enmAsmTokenType_DI,
	enmAsmTokenType_AL,
	enmAsmTokenType_CL,
	enmAsmTokenType_DL,
	enmAsmTokenType_BL,
	enmAsmTokenType_AH,
	enmAsmTokenType_CH,
	enmAsmTokenType_DH,
	enmAsmTokenType_BH,
	enmAsmTokenType_ES,
	enmAsmTokenType_CS,
	enmAsmTokenType_SS,
	enmAsmTokenType_DS,
	enmAsmTokenType_ADD,
	enmAsmTokenType_ADC,
	enmAsmTokenType_SBB,
	enmAsmTokenType_SUB,
	enmAsmTokenType_CMP,
	enmAsmTokenType_TEST,
	enmAsmTokenType_AND,
	enmAsmTokenType_OR,
	enmAsmTokenType_XOR,
	enmAsmTokenType_MOV,
	enmAsmTokenType_XCHG,
	enmAsmTokenType_LEA,
	enmAsmTokenType_LES,
	enmAsmTokenType_LDS,
	enmAsmTokenType_INC,
	enmAsmTokenType_DEC,
	enmAsmTokenType_NOT,
	enmAsmTokenType_NEG,
	enmAsmTokenType_MUL,
	enmAsmTokenType_DIV,
	enmAsmTokenType_IMUL,
	enmAsmTokenType_IDIV,
	enmAsmTokenType_PUSH,
	enmAsmTokenType_POP,
	enmAsmTokenType_ROL,
	enmAsmTokenType_ROR,
	enmAsmTokenType_RCL,
	enmAsmTokenType_RCR,
	enmAsmTokenType_SHL,
	enmAsmTokenType_SHR,
	enmAsmTokenType_SAR,
	enmAsmTokenType_IN,
	enmAsmTokenType_OUT,
	enmAsmTokenType_MOVSB,
	enmAsmTokenType_MOVSW,
	enmAsmTokenType_CMPSB,
	enmAsmTokenType_CMPSW,
	enmAsmTokenType_STOSB,
	enmAsmTokenType_STOSW,
	enmAsmTokenType_LODSB,
	enmAsmTokenType_LODSW,
	enmAsmTokenType_SCASB,
	enmAsmTokenType_SCASW,
	enmAsmTokenType_NOP,
	enmAsmTokenType_XLAT,
	enmAsmTokenType_PUSHF,
	enmAsmTokenType_POPF,
	enmAsmTokenType_SAHF,
	enmAsmTokenType_LAHF,
	enmAsmTokenType_CBW,
	enmAsmTokenType_CWD,
	enmAsmTokenType_CMC,
	enmAsmTokenType_CLC,
	enmAsmTokenType_STC,
	enmAsmTokenType_CLI,
	enmAsmTokenType_STI,
	enmAsmTokenType_CLD,
	enmAsmTokenType_STD,
	enmAsmTokenType_DAA,
	enmAsmTokenType_AAA,
	enmAsmTokenType_DAS,
	enmAsmTokenType_AAS,
	enmAsmTokenType_AAM,
	enmAsmTokenType_AAD,
	enmAsmTokenType_IRET,
	enmAsmTokenType_INTO,
	enmAsmTokenType_WAIT,
	enmAsmTokenType_LOCK,
	enmAsmTokenType_REPNZ,
	enmAsmTokenType_REPNE,
	enmAsmTokenType_REPZ,
	enmAsmTokenType_REPE,
	enmAsmTokenType_REP,
	enmAsmTokenType_HLT,
	enmAsmTokenType_JO,
	enmAsmTokenType_JNO,
	enmAsmTokenType_JB,
	enmAsmTokenType_JNB,
	enmAsmTokenType_JZ,
	enmAsmTokenType_JE,
	enmAsmTokenType_JNZ,
	enmAsmTokenType_JNE,
	enmAsmTokenType_JNA,
	enmAsmTokenType_JBE,
	enmAsmTokenType_JA,
	enmAsmTokenType_JS,
	enmAsmTokenType_JNS,
	enmAsmTokenType_JP,
	enmAsmTokenType_JNP,
	enmAsmTokenType_JL,
	enmAsmTokenType_JNL,
	enmAsmTokenType_JGE,
	enmAsmTokenType_JNG,
	enmAsmTokenType_JLE,
	enmAsmTokenType_JG,
	enmAsmTokenType_LOOPNZ,
	enmAsmTokenType_LOOPNE,
	enmAsmTokenType_LOOPZ,
	enmAsmTokenType_LOOPE,
	enmAsmTokenType_LOOP,
	enmAsmTokenType_JCXZ,
	enmAsmTokenType_CALL,
	enmAsmTokenType_JMP,
	enmAsmTokenType_SHORT,
	enmAsmTokenType_NEAR,
	enmAsmTokenType_FAR,
	enmAsmTokenType_RET,
	enmAsmTokenType_RETF,	
	enmAsmTokenType_INT,
	enmAsmTokenType_ORG,
	enmAsmTokenType_EQU,
	enmAsmTokenType_LABEL,
	enmAsmTokenType_DB,
	enmAsmTokenType_DW,
	enmAsmTokenType_DD,
	enmAsmTokenType_BYTE,
	enmAsmTokenType_WORD,
	enmAsmTokenType_DWORD,
	enmAsmTokenType_PTR,
	enmAsmTokenType_OFFSET,
	enmAsmTokenType_PROC,
	enmAsmTokenType_ENDP,
	enmAsmTokenType_Identifier,
	enmAsmTokenType_Number,
	enmAsmTokenType_RandomNumber,
	enmAsmTokenType_Comma,
	enmAsmTokenType_Colon,
	enmAsmTokenType_Dot,
	enmAsmTokenType_Plus,
	enmAsmTokenType_Minus,
	enmAsmTokenType_Times,
	enmAsmTokenType_LeftParen2,
	enmAsmTokenType_RightParen2,
	enmAsmTokenType_NewLine,
	enmAsmTokenType_Eof,
};

enum enmAsmScannerState
{
	enmAsmScannerState_Start,
	enmAsmScannerState_End,
	enmAsmScannerState_InComment1,
	enmAsmScannerState_InIdentifier,
	enmAsmScannerState_InNumber,
};

class tagAsmToken
{
public:
	enmAsmTokenType eTokenType;
	std::string szTokenString;
	int nStartLineNumber;
	int nEndLineNumber;
	int nStartCharIndex;
	int nEndCharIndex;
};

class CAsmMacro
{
public:
	CAsmMacro();
	~CAsmMacro();
	std::string m_szMacroName;
	std::vector<tagAsmToken *> m_vecTokenList;
};

class CAsmScanner
{
public:
	CAsmScanner(void);
	~CAsmScanner(void);
	BOOL OpenSource(const WCHAR *pszFilename);
	void CloseSource(void);
	void BuildTokenList(void);
	int GetCurrentTokenIndex(void);
	void SetCurrentTokenIndex(int nCurrentTokenIndex);
	tagAsmToken *GetCurrentToken(void);
	void PrintTokenList(void);
	void PrintTokenType(enmAsmTokenType eTokenType, const char *pszTokenString);
	tagAsmToken *CopyToken(tagAsmToken *pToken);
	void AddMacro(CAsmMacro *pMacro);
	void ClearLocalMacros();
private:
	void BuildKeywordTable(void);
	void ResetSource(void);
	char GetChar(void);
	void UnGetChar(void);
	enmAsmTokenType GetToken(void);
	BOOL IsHexDigit(char chChar);
	CAsmMacro *FindMacro(const char *pszMacroName);
	void ProcessMacroExpand(void);
	char *m_pszSource;
	int m_nSourceLength;
	int m_nLineNumber;
	int m_nCharIndex;
	enmAsmTokenType m_eTokenType;
	char m_szTokenString[1000];
	int m_nTokenStartLineNumber;
	int m_nTokenEndLineNumber;
	int m_nTokenStartCharIndex;
	int m_nTokenEndCharIndex;
	std::map<std::string,enmAsmTokenType> m_mapKeywordMap;
	std::vector<tagAsmToken *> m_vecTokenList;
	int m_nCurrentTokenIndex;
	std::vector<CAsmMacro *> m_vecLocalMacroList;
	std::map<std::string,CAsmMacro *> m_vecMacroMap;
};

CAsmScanner *CAsmScanner_GetInstance(void);