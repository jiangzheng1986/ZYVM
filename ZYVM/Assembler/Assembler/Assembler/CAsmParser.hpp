//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Assembler
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#pragma once

#include "Common.hpp"

#include "CAsmScanner.hpp"

#include "CAsmSyntaxTree.hpp"

#include "Utility.hpp"

//基于回溯的语法分析器

class CAsmParser
{
public:
	CAsmParser(void);
	~CAsmParser(void);
	CAsmSyntaxTree *Parse(void);
	bool GetError();
private:
	void UpdateCurrentToken(void);
	void NextToken(void);
	int GetCurrentTokenIndex(void);
	void SetCurrentTokenIndex(int nCurrentTokenIndex);
	void MatchToken(enmAsmTokenType eExpectedTokenType);
	void Error(const char *pszFormat, ...);
	CAsmSyntaxTree *ParseProgram(void);
	CAsmSyntaxTree *ParseStatementList(void);
	void ParseMacroDefine(CAsmSyntaxTree *pTree);
	void ParseProcBegin(CAsmSyntaxTree *pTree);
	void ParseProcEnd(CAsmSyntaxTree *pTree);
	void ParseDataDefine(CAsmSyntaxTree *pTree);
	void ParseDataList(CAsmSyntaxTree *pTree);
	CAsmSyntaxTree *ParseData(void);
	CAsmSyntaxTree *ParseDataLabelDefine(void);
	CAsmSyntaxTree *ParseStatementLabel(void);
	CAsmSyntaxTree *ParseLabel1(void);
	CAsmSyntaxTree *ParseRepeatPrefix(void);
	CAsmSyntaxTree *ParseStatement(void);
	CAsmSyntaxTree *ParseStatement1(void);
	CAsmSyntaxTree *ParseExpression(void);
	CAsmSyntaxTree *ParseRandomNumber(void);
	CAsmSyntaxTree *ParseNumber(void);
	CAsmSyntaxTree *ParseRegister(void);
	BOOL IsTokenRegister(enmAsmTokenType eTokenType);
	CAsmSyntaxTree *ParseSegRegister(void);
	void ParseSegPrefix(CAsmSyntaxTree *pTree);
	BOOL IsTokenSegRegister(enmAsmTokenType eTokenType);
	CAsmSyntaxTree *ParseMemory(void);
	void ParseMemory1(CAsmSyntaxTree *pTree);
	void ParseMemoryAddress(CAsmSyntaxTree *pTree);
	void ParseMemoryAddressElement(CAsmSyntaxTree *pTree);
	void ParseMemoryAddressElement_Minus(CAsmSyntaxTree *pTree);
	CAsmSyntaxTree *ParseLabelOffset(void);
	CAsmSyntaxTree *ParseLabelRef(void);
	enmAsmTokenType m_eTokenType;
	char m_szTokenString[1000];
	int m_nLineNumber;
	BOOL m_bShowFirstErrorOnly;
	BOOL m_bFirstErrorShowed;
	BOOL m_bError;
	BOOL m_bCurrentInstructionIsJump;
	bool m_bBaseRegisterFound;
	bool m_bIndexRegisterFound;
};

CAsmParser *CAsmParser_GetInstance(void);